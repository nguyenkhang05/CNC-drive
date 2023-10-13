/*************************************************************************
 *
 *  NSMVRH850V02.cpp
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

#include "systemc.h"
#include <string.h>  // for strcpy, strtok, strcmp, strncmp
#include <stdio.h>   // for sprintf
#include <errno.h>   // for errno
#include <sstream>   // for istringstream

#include "CmErrMsg.h" // for WinMsg
#include "Common.h"

#include "NSMVG4SSV01.h"
#include "NSMVRH850V02.h"
#include "PFRH850.hpp"
//#include "main.h"

//#include "pltfrm.h"
//#include "pltfrmGND.h"
//#include "pltfrmRH850.h"

using namespace CmErrMsg; // for WinMsg

extern void pltfrmFC( NSMVG4SSV01 *G4SS, const char *config_file );
extern void pltfrmRH850( NSMVG4SSV01 *G4SS, const char *config_file );
extern void pltfrm( NSMVG4SSV01 *G4SS, PFRH850 *pfRh850, const char *config_file );
extern void pltfrmSmpils( NSMVG4SSV01 *G4SS, PFRH850 *pfRh850, const char *config_file );
extern void pltfrmGND( NSMVG4SSV01 *G4SS, PFRH850 *pfRh850, const char *config_file );
extern void pltfrmFCDelete();
extern void pltfrmDelete();
extern void pltfrmRH850Delete();
extern void pltfrmSmpilsDelete();
extern void pltfrmGNDDelete();

extern PFRH850 *PFRH850_0;

/// <summary>constructer</summary>
/// <param name="moduleName">sc_module_name</param>
/// <param name="configFile">const char *</param>
NSMVRH850V02::
NSMVRH850V02(sc_module_name moduleName, const char *configFile ):
  sc_module(moduleName),
  G4SS(0),
  mPltfrmSize(0),
  mPeripheral(0)
{

  read_config_file( configFile );

  //----- instantiation pointer -----
  // ----------------------------------------
  G4SS = new NSMVG4SSV01( "G4SS", configFile );

  //priority : hight
  funcPltfrm[0].fpltfrm        = NULL;
  funcPltfrm[0].fpltfrmFC      = pltfrmFC;
  funcPltfrm[0].fpltfrmDelete = pltfrmFCDelete;
  funcPltfrm[0].peripheral     = PERIPHERAL_DEFAULT;
  funcPltfrm[1].fpltfrm        = NULL;
  funcPltfrm[1].fpltfrmFC      = pltfrmRH850;
  funcPltfrm[1].fpltfrmDelete = pltfrmRH850Delete;
  funcPltfrm[1].peripheral     = PERIPHERAL_RH850;
  funcPltfrm[2].fpltfrm        = pltfrm;
  funcPltfrm[2].fpltfrmFC      = NULL;
  funcPltfrm[2].fpltfrmDelete = pltfrmDelete;
  funcPltfrm[2].peripheral     = PERIPHERAL_DEFAULT;
  funcPltfrm[3].fpltfrm        = pltfrmSmpils;
  funcPltfrm[3].fpltfrmFC      = NULL;
  funcPltfrm[3].fpltfrmDelete = pltfrmSmpilsDelete;
  funcPltfrm[3].peripheral     = PERIPHERAL_SMPILS;
#ifdef VLAB
  funcPltfrm[4].fpltfrm        = NULL;
#else
  funcPltfrm[4].fpltfrm        = pltfrmGND;
#endif
  funcPltfrm[4].fpltfrmFC      = NULL;
  funcPltfrm[4].fpltfrmDelete = pltfrmGNDDelete;
  funcPltfrm[4].peripheral     = PERIPHERAL_DEFAULT;
  //priority : low

  /****** connect by User ******/
  PFRH850 *pfss = NULL;
  mPeripheral |= PERIPHERAL_DEFAULT;
  mPltfrmSize = sizeof(funcPltfrm) / sizeof(funcPltfrm[0]);
  for (unsigned int i=0; i<mPltfrmSize; i++) {
    if (mPeripheral & funcPltfrm[i].peripheral) {
      if (funcPltfrm[i].fpltfrm) {
        funcPltfrm[i].fpltfrm(G4SS, pfss, configFile);
      } else if (funcPltfrm[i].fpltfrmFC) {
        funcPltfrm[i].fpltfrmFC(G4SS, configFile);
      }
    }

    if (funcPltfrm[i].peripheral == PERIPHERAL_RH850) {
      pfss = PFRH850_0;
    }
  }

}


/// <summary>destructer</summary>
NSMVRH850V02::
~NSMVRH850V02(void)
{
  for (unsigned int i=0; i<mPltfrmSize; i++) {
    if (mPeripheral & funcPltfrm[i].peripheral) {
      funcPltfrm[i].fpltfrmDelete();
    }
  }
  // Explicit call to destructors
  // ----------------------------------------
  delete G4SS;
}


/// <summary>for reading the configulation file and initialization</summary>
void
NSMVRH850V02::
read_config_file( const char *filename ) {
  char config_word[512];
  char config_seps[]=" ,=()\t\n\r";
  char *config_token;
  ifstream configFile( filename );
  
  // read the config file
  while(1){
    configFile.getline(config_word, 512, '\n');
    config_token = strtok(config_word, config_seps);
    if (configFile.eof()) {
      break;
    }
    if ((config_token == NULL) || (strncmp(config_token, "//", 2) == 0)) {
      continue;
    }
    if (strcmp(config_token, "[END]") == 0){
      break;
    }
    if (strcmp(config_token, "[PERIPHERAL]") == 0) {
      mPeripheral = get_cfg_PERIPHERAL(strtok(0, config_seps), this->name(), "read_config_file");
      continue;
    }
  }
}

/// <summary>get_cfg_PERIPHERAL</summary>
/// <param name="param_str">sc_module_name</param>
/// <param name="mod_name">const char *</param>
/// <param name="func_name">const char *</param>
/// <returns>peripheral connection condition</returns>
unsigned int
NSMVRH850V02::
get_cfg_PERIPHERAL(char *param_str, const char *mod_name, const char *func_name) {
  if (!param_str) {
    printWarningMessage(mod_name, func_name,
      "No token was specified on [PERIPHERAL]. Treat NONE.\n");
    return PERIPHERAL_NONE;
  }

  if (strcmp(param_str, "NONE") == 0) {
    return PERIPHERAL_NONE;
  }

  unsigned int peripheral = PERIPHERAL_NONE;
  std::string token, param(param_str);
  std::istringstream iss(param);
  while (std::getline(iss, token, '&')) {   //devided by '&'
    if (token == "NONE") {
      continue;   //skip
    } else if (token == "PFRH850") {
      peripheral |= PERIPHERAL_RH850;   //or
    } else if (token == "SMPILS") {
      peripheral |= PERIPHERAL_SMPILS;  //or
    } else {
      char buf[1024];
      sprintf(buf,
        "Unexpected token was specified [%s] on [PERIPHERAL]. Treat NONE.\n",
        token.c_str());
      printWarningMessage(mod_name, func_name, buf);
      return PERIPHERAL_NONE;
    }
  }

  //check value and combination.
  if (peripheral != PERIPHERAL_NONE &&
      !(peripheral & PERIPHERAL_RH850) &&
      !(peripheral & PERIPHERAL_SMPILS)) {
    char buf[1024];
    sprintf(buf,
      "Unexpected value was specified [%s] on [PERIPHERAL]. Treat NONE.\n",
      param_str);
    printWarningMessage(mod_name, func_name, buf);
    return PERIPHERAL_NONE;
  }

  return peripheral;
}

