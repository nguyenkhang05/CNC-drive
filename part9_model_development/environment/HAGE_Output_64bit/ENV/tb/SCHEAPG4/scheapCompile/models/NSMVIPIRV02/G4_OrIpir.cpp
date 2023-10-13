/*************************************************************************
 *
 * G4_OrIpir.cpp
 *
 * Copyright(c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

#include <cstdio>
#include "Common.h"
#include "CmErrMsg.h"
#include "G4_intc_def.h"
#include "G4_OrIpir.h"
#include "OrUnit.h"

using namespace CmErrMsg;

// for debug output
#ifdef DEBUG_MESSAGE_OUTPUT
# include <fstream>
  extern std::ofstream difout;
#endif // DEBUG_MESSAGE_OUTPUT


/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <param name="pCfgFileName">configuration file name</param>
/// <returns>none</returns>
G4_OrIpir::G4_OrIpir( 
  const char* name,
  const char* pCfgFileName )
{
  mpModName = name;
  dmsgsc( mpModName << ".G4_OrIpir()" );
  char         inst_name[32];
  unsigned int pe_num, ch_num, ipir_num; 

  // get configuration parameter
  readCfgFile( pCfgFileName );

  // creat the instances of ports
  for ( ipir_num = 0; ipir_num < PE_MAX_NUM; ipir_num++ ) { // IPIR
    for ( ch_num = 0; ch_num < INTC_IPIR_CH_NUM; ch_num++ ) { // channel
      for ( pe_num = 0; pe_num < PE_MAX_NUM; pe_num++ ) { // PE(bit)
        sprintf( inst_name, "in_pe%d_ch%d_bit%d", ipir_num+1, ch_num+1, pe_num );
        in_pe_ch_bit[ipir_num][ch_num][pe_num] = new sc_in<bool>( inst_name );
      }
      sprintf( inst_name, "in_pe%d_ch%d_ext", ipir_num+1, ch_num );
      in_pe_ch_ext[ipir_num][ch_num] = new sc_in<bool>( inst_name );
      sprintf( inst_name, "out_pe%d_ch%d", ipir_num+1, ch_num );
      out_pe_ch[ipir_num][ch_num] = new sc_out<bool>( inst_name );
    }
  }

  // create OR logic circuit
  for ( pe_num = 0; pe_num < mPeNum; pe_num++ ) { // PE
    for ( ch_num = 0; ch_num < INTC_IPIR_CH_NUM; ch_num++ ) { // channel
      sprintf( inst_name, "OR_%d_ch%d", pe_num+1, ch_num );
      mpOrUnit[pe_num][ch_num] = new OrUnit<bool, PE_MAX_NUM+1>( inst_name );
    }
  }

  // port connection
  for ( pe_num = 0; pe_num < mPeNum; pe_num++ ) { // PE(bit)
    for ( ch_num = 0; ch_num < INTC_IPIR_CH_NUM; ch_num++ ) { // channel
      for ( ipir_num = 0; ipir_num < PE_MAX_NUM+1; ipir_num++ ) { // IPIR + external interrupt
        if ( ipir_num == mPeNum ) { // external interrupt port
          mpOrUnit[pe_num][ch_num]->in_port[ipir_num]->bind( *in_pe_ch_ext[pe_num][ch_num] );
        }
        else { // IPIR port
          mpOrUnit[pe_num][ch_num]->in_port[ipir_num]->bind( *in_pe_ch_bit[ipir_num][ch_num][pe_num] );
        }
      }
    }
  }
  for ( pe_num = 0; pe_num < mPeNum; pe_num++ ) { // PE
    for ( ch_num = 0; ch_num < INTC_IPIR_CH_NUM; ch_num++ ) { // channel
      mpOrUnit[pe_num][ch_num]->out_port->bind( *out_pe_ch[pe_num][ch_num] );
    }
  }
}

/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
G4_OrIpir::~G4_OrIpir( void )
{
  dmsgsc( mpModName << ".~G4_OrIpir()" );
  unsigned int pe_num, ch_num, ipir_num; 

  for ( ipir_num = 0; ipir_num < PE_MAX_NUM; ipir_num++ ) {
    for ( ch_num = 0; ch_num < INTC_IPIR_CH_NUM; ch_num++ ) {
      for ( pe_num = 0; pe_num < PE_MAX_NUM; pe_num++ ) {
        delete in_pe_ch_bit[ipir_num][ch_num][pe_num];
      }
      delete in_pe_ch_ext[ipir_num][ch_num];
      delete out_pe_ch[ipir_num][ch_num];
    }
  }

  for ( pe_num = 0; pe_num < mPeNum; pe_num++ ) { // PE
    for ( ch_num = 0; ch_num < INTC_IPIR_CH_NUM; ch_num++ ) { // channel
      delete mpOrUnit[pe_num][ch_num];
    }
  }
}


// private functions
/// <summary>set parameters from configuration file</summary>
/// <param name="filename">configuration file name</param>
/// <returns>none</returns>
void
G4_OrIpir::readCfgFile( const char *filename )
{
  dmsgsc( mpModName << ".readCfgFile()" );
  char    word[512];
  char    seps[] = " ,=();\t\n\r";
  char*   token;

  ifstream  config_file(filename);
  // read the config file
  while( 1 ) {
    config_file.getline( word, 512, '\n' );
    token = strtok( word, seps );

    // ----------------------------------------
    // For EOF
    if ( config_file.eof() ) {
      break;
    }
    else { // not EOF
      // do nothing
    }

    // ----------------------------------------
    // For comment
    if ( (NULL == token) || (0 == strncmp(token, "//", 2)) ) {
      continue;
    }
    else { // not comment
      // do nothing
    }

    // ----------------------------------------
    // detect end marker
    // NOTICE:You have to check whether token is NULL first.
    if ( 0 == strcmp(token, "[END]") ) {
      break;
    }
    else { // not [END]
      // do nothing
    }

    // ----------------------------------------
    // get token
    // ----------------------------------------
    // For miscellaneous
    if ( 0 == strcmp(token, "[PE_NUM]") ) {
      token = strtok( 0, seps );
      if (token != NULL) {
        mPeNum = token_to_uint( token, "G4_OrIpir", "[PE_NUM]" );
        dmsgsc( "  mPeNum:0x" << mPeNum );
        if ( mPeNum > PE_MAX_NUM ) {
          char msg[1024];
          // for WinMsg
          sprintf( msg,
                   "[PE_NUM] must be under %d, but specified (%d).",
                   PE_MAX_NUM+1, mPeNum);
          printErrorMessage( mpModName, "readCfgFile", msg );
          exit( 1 );
        }
        else { // mPeNum is under PE_MAX_NUM
          // do nothing
        }
      }
      else { // token is NULL
        // do nothing
      }
      continue;
    }
    else { // token is not [PE_NUM]
      // do nothing
    }
  }  // end of configuration file read loop(infinite)
}  // end of readCfgFile()
