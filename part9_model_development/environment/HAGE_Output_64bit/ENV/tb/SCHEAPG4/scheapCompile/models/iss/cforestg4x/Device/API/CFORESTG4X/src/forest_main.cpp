/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

/*! \mainpage
    CForestG3M is G3M configuration of the instruction set simulator
    for V850 E3v5 architecture.
*/

#include "./config.h"
#ifdef ENABLE_PYTHON
    // to prevent to use pythonxx_d.lib in Python.h(pyconfig.h)
    #if defined(_DEBUG)
        /* Use debug wrappers with the Python release dll */
        #undef _DEBUG
        #include <Python.h>
        #define _DEBUG
    #else // #if defined(_DEBUG)
        #include <Python.h>
    #endif // #if defined(_DEBUG)
#endif // ENABLE_PYTHON

#include <stdlib.h>
#include <signal.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "./forest.h"
#include "./forest_utility.h"
#include "./forest_cmdopt.h"
#include "./forest_setup.h"

#ifdef ENABLE_PYTHON
#include "./forest_python.h"
#endif // ENABLE_PYTHON

#ifdef ENABLE_GDB
#include "./forest_gdb_stub.h"
#endif // ENABLE_GDB

#ifdef ENABLE_LICENSE_AUTH
#include "cforest_extension_license_checker.hpp"
#endif // ENABLE_LICENSE_AUTH

typedef void (*sighandler_t)(int);

int main (int argc, char *argv[])
{
    int exit_code;

#ifdef ENABLE_LICENSE_AUTH
    char *tmpkey = std::getenv(AUTH_ENVIRONMENT_VARIABLE);
    if (tmpkey == (char *)NULL) {
      printf("<Error: Can't find Product License key.>\n");
      exit (EXIT_NORMAL);
    }
    uint32_t key = atoi(tmpkey);
    uint32_t now = (uint32_t)time(NULL);
    CforestExtension::LicenseChecker *l = new CforestExtension::LicenseChecker();
    if (l->LicenseChecker::hasLicense(key, now) == false) {
      printf("<Error: Product License has expired.>\n");
      delete l;
      exit (EXIT_NORMAL);
    }
#endif

    ForestUtil* forest_util = NULL;
    ForestCmdOpt* cmdopt = NULL;
    ForestSetup* forest_setup = NULL;

    std::vector<FILE*> logs;
    FILE* fp;
    try {
        forest_util = new ForestUtil ();

        // オプション解析
        cmdopt = new ForestCmdOpt (forest_util);
        cmdopt->AnalyzeOption (argc, argv);
        forest_util->SetMessageLevel (cmdopt->GetMessageLevel ());

        // メッセージ出力用のポインタを初めに初期化する (for CfMsg::DPrint)
        fp = forest_util->OpenFileOut (cmdopt->GetOutFname ());
        if (fp == NULL) {
            exit (EXIT_NORMAL);
        }

        forest_util->PrintVersion (fp);

        // CreateForest以前に、
        // SetMessageLevel/OpenFileOut/PrintVersion以外のAPIを呼び出してはいけない
        forest_util->CreateForest ();

        forest_setup = new ForestSetup (forest_util, cmdopt);

        if ( (cmdopt->GetScriptName () == "")
          && ((cmdopt->IsSetGdbCmdopt () == true) || (cmdopt->GetHexName () != "")) ) {
            // Pythonモードに到達しない場合だけ、デフォルト値でPEを生成する
            uint32_t pe_num = cmdopt->GetPeNumFromDbg ();
            CoreType core_type = cmdopt->GetCoreType ();
            CoreVersion core_verson = cmdopt->GetCoreVersion ();
            for (uint32_t peid = 0; peid < pe_num; peid++) {
                uint32_t clid = 0; // @@ TODO : Only CL0 is supported
                forest_util->SetPe (clid, peid, core_type, core_verson);
            }

            // PE作成以降のデフォルト設定は共通関数に任せる
            forest_setup->DefaultSetup ();
        }

        if (cmdopt->GetScriptName () != "") {
#ifdef ENABLE_PYTHON
            // scrを指定済みならPythonモード
            // ただしscr内のHEX/GDBはコマンドラインオプションで指定された値で上書き
            PythonAPI::StartPy (forest_util, cmdopt, forest_setup, argc, argv);
#endif // ENABLE_PYTHON
        } else if (cmdopt->IsSetGdbCmdopt () == true) {
            // GDBを指定済みならGDBに接続
#ifdef ENABLE_GDB
            GdbStub gdb_stub (forest_util, cmdopt);
            gdb_stub.ConnectGDB (cmdopt->GetGdbPort ());
#endif // ENABLE_GDB
        } else if (cmdopt->GetHexName () != "") {
            // HEXを指定済みならhexをloadしてrun
            if (forest_util->LoadSrec (cmdopt->GetHexName ().c_str(), 0) != false) {
                // Ctrl-C をフック
                sighandler_t sigint_bak = signal (SIGINT, ForestUtil::ReqSimStop);

                forest_util->StepExecute<RUN_MODE> (0, 0);

                // Ctrl-Cのをフックを解除
                signal (SIGINT, sigint_bak);
            }
        } else {
#ifdef ENABLE_PYTHON
            // 指定なしならPythonモード
            PythonAPI::StartPy (forest_util, cmdopt, forest_setup, argc, argv);
#endif // ENABLE_PYTHON
        }

#ifdef DISABLE_DESTRUCTOR_CYCLE
#else // DISABLE_DESTRUCTOR_CYCLE
        if (forest_util->GetShowStatistics () == true) {
            for (uint32_t lpeid = 0; lpeid <= forest_util->GetPeLastId (); lpeid++) {
                if (forest_util->IsValidPeId (lpeid)) {
                    forest_util->ShowPerfInfo (lpeid);
                }
            }
        }
#endif // DISABLE_DESTRUCTOR_CYCLE

        exit_code = forest_util->GetExitStatus();
    }
    catch(std::bad_alloc& exception) {
        printf("<Error: bad_alloc: Failed to allocate memory space.>\n");
        exit_code = EXIT_BAD_ALLOC;
    }

    delete forest_setup;
    delete cmdopt;
    delete forest_util;

    return (exit_code);
}
