/*************************************************************************
*
*  pltfrm.cpp
*
* Copyright(c) 2016 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*
*************************************************************************/
//#define USE_G4SS

#ifdef USE_G4SS
#include "NSMVG4SSV01.h"
#include "PFRH850.hpp"
#else
#include "BV_TestENV.h"
#include "NSMVG4SSLITE.h"
typedef NSMVG4SSLITE NSMVG4SSV01;
typedef void PFRH850;
#endif
#include "DummyMasterRvc.h" 
#include "DummyPeripheralRvc.h"
#include "APISS.h"
#ifdef USE_G4SS
#include "NSMVG4CPUV01.h"
#endif
#include "I_BRIDGE.h"
#include "OSCI2DCDR.h"
#include "HAGE.h"

///
#ifdef LINUX_DEF
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#endif

/// <summary>platform connection function for module which is connected before RH850 peripherals</summary>
/// <param name="G4SS">G4SS pointer</param>
/// <param name="config_file">configration file name</param>

extern sc_signal<sc_dt::uint64, sc_core::SC_UNCHECKED_WRITERS> *glb_pe_clk_sig;

#ifdef LINUX_DEF
void handler(int sig) {
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}
#endif

namespace RVC_DEFINE
{
    SIM_MODE_T                              mSimMode;
    char                                    bus_map_file[1024];
    DummyMasterRvc                          *dummy_master;
    DummyPeripheralRvc                      *dummy_peripheral;
    HAGE                                    *hage;

    I_BRIDGE<BUS_WIDTH_APB, BUS_WIDTH_VCI>  *pbus_bridge;
    OSCI2DCDR<BUS_WIDTH_APB>                *local_pbus_APB;
    OSCI2DCDR<BUS_WIDTH_FLSH>               *local_pbus_AXI;

    // Clocks
    sc_signal<sc_dt::uint64>                PCLK_sig;
    // Resets
    sc_signal<bool>                         PRESET_sig;
    // Ports
    sc_signal<bool>                         EINT_sig;

}
using namespace RVC_DEFINE;

void read_config_file(const char *filename)
{
    char	  word[512];
    char	  seps[] = " ,=();\t\n\r";
    char	  *token;

    ifstream  config_file(filename);

    // read the config file
    while(1) {
        config_file.getline(word, 512, '\n');
        token = strtok(word, seps);

        // ----------------------------------------
        // For EOF
        if (config_file.eof()) {
            break;
        }

        // ----------------------------------------
        // For comment
        if ((token == NULL) || (strncmp(token, "//", 2) == 0)) {
            continue;
        }

        // ----------------------------------------
        // detect end marker
        // NOTICE:You have to check whether token is NULL first.
        if (strcmp(token, "[END]") == 0) {
            break;
        }

        // ----------------------------------------
        // get token
        // ----------------------------------------
        // bus map file name
        if( strcmp( token, "[BUS_MAP_FILE]" ) == 0 ) {
            token = strtok(0, seps);
            const char *fname = (!token)? "" : token;
            strcpy(bus_map_file, fname);
            continue;
        }
    }  // end of configuration file read loop(infinite)
};


void pltfrmFC( NSMVG4SSV01 *G4SS, const char *config_file=NULL )
{

#if defined(PLTFRM_TST)
    cout << "pltfrmFC() was called." << endl;
#endif //PLTFRM_TST

}

/// <summary>platform connection function for module which is connected after RH850 peripherals</summary>
/// <param name="G4SS">G4SS pointer</param>
/// <param name="pfRh850">PFRH850 pointer</param>
/// <param name="config_file">configration file name</param>
void pltfrm( NSMVG4SSV01 *G4SS, PFRH850 *pfRh850, const char *config_file=NULL )
{

#if defined(PLTFRM_TST)
    cout << "pltfrm() was called." << endl;
#endif //PLTFRM_TST

#ifdef LINUX_DEF
    signal(SIGSEGV, handler);   // install our handler
#endif
    sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated", sc_core::SC_DO_NOTHING );

    read_config_file(config_file);

    /* Instance model */
    dummy_master     = new DummyMasterRvc("dummy_master", 0, 0);
    dummy_peripheral = new DummyPeripheralRvc("dummy_peripheral", 0, 0);
    hage             = new HAGE("hage", config_file, 0, 0, mSimMode);
    pbus_bridge      = new I_BRIDGE<BUS_WIDTH_APB, BUS_WIDTH_VCI>("pbus_bridge");
    local_pbus_APB   = new OSCI2DCDR<BUS_WIDTH_APB>("local_pbus_APB", bus_map_file);
    local_pbus_AXI   = new OSCI2DCDR<BUS_WIDTH_FLSH>("local_pbus_AXI", bus_map_file);

    /* Connect P_BUS to bridge */
    (G4SS->P_BUS->is)(pbus_bridge->ts);
    (pbus_bridge->is)(local_pbus_APB->ts);
    /* Connect dummy_master */
    dummy_master->freqPortApb(G4SS->GAPB_SG1_clk);
    dummy_master->resetPort(G4SS->sys_reset);
    (*(dummy_master->is))(G4SS->P_BUS->ts);
    /* Connect dummy_peripheral */
    dummy_peripheral->freqPortApb(G4SS->GAPB_SG1_clk);
    dummy_peripheral->resetPort(G4SS->sys_reset);
    dummy_peripheral->PCLK(PCLK_sig);
    dummy_peripheral->PRESETn(PRESET_sig);
    dummy_peripheral->EINT(EINT_sig);
    (local_pbus_AXI->is)(*(dummy_peripheral->ts));

    /* Connect hage */
    hage->PCLK(PCLK_sig);
    hage->PRESET(PRESET_sig);
    hage->EINT(EINT_sig);
    (local_pbus_APB->is)(*(hage->ts));
    (*(hage->is))(local_pbus_AXI->ts);
}

/// <summary>delete function for module which is connected before RH850 peripherals</summary>
void pltfrmFCDelete()
{

#if defined(PLTFRM_TST)
    cout << "pltfrmFCDelete() was called." << endl;
#endif //PLTFRM_TST

}

/// <summary>delete function for module which is connected after RH850 peripherals</summary>
void pltfrmDelete()
{

#if defined(PLTFRM_TST)
    cout << "pltfrmDelete() was called." << endl;
#endif //PLTFRM_TST
    //Added by NganTran ---------------
    delete dummy_master;
    delete dummy_peripheral;
    delete hage;
    delete pbus_bridge;
    delete local_pbus_APB;
    delete local_pbus_AXI;
    //---------------------------------
}
