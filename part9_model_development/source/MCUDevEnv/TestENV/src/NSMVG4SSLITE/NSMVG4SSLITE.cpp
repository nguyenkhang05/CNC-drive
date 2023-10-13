/***********************************************************************
*
* NSMVG4SSLITE.cpp
*
* Copyright(c) 2016-2021 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*
***********************************************************************/

#include "systemc.h"
#include "NSMVG4SSLITE.h"
#include "APISS.h"
#include "DummyMaster.h"
#include "DummySlave.h"

NSMVG4SSLITE::NSMVG4SSLITE(sc_module_name module_name, const char *config_file) :
    sc_module(module_name)
    , mMapFileName()
    , mIssMode(SIM_MODE_CA)
    , sys_reset("sys_reset")
    , GAPB_SG1_clk("GAPB_SG1_clk")
    , P_BUS(0)
    , isg_sg1("isg_sg1")
{
    read_config_file(config_file);
    P_BUS = new APISS("P_BUS", mMapFileName );

    /* APB dummy bind */
    mpDummyMaster = new DummyMaster<BUS_WIDTH_APB>();
    mpDummySlave = new DummySlave<BUS_WIDTH_APB>();
    mpDummyMaster->is(isg_sg1);
    isg_sg1(mpDummySlave->ts);
};

NSMVG4SSLITE::~NSMVG4SSLITE(void) {
    delete P_BUS;
    delete mpDummyMaster;
    delete mpDummySlave;
};

void NSMVG4SSLITE::read_config_file(const char *filename)
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
            strcpy(mMapFileName, fname);
            continue;
        }

        // simulation mode
        if (strcmp(token, "[SIM_MODE]") == 0) {
            token = strtok(0, seps);
            if(strcmp(token, "CAISS") == 0){
                mIssMode = SIM_MODE_CA;
            } else if(strcmp(token, "FASTISS") == 0){
                mIssMode = SIM_MODE_FAST;
            } else{
                char error_message_buf[1024];
                sprintf(error_message_buf,
                    "Unexpected token was specified [%s] on [SIM_MODE].",
                    token);
                printErrorMessage(name(), "read_config_file", error_message_buf);
            }
#if defined(PLTFRM_TST)
            cout << this->name() << "->token = " << token << endl;
#endif //PLTFRM_TST
            continue;
        }
    }  // end of configuration file read loop(infinite)
};
