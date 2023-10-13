/*************************************************************************
 *
 *  G4_TsIf.cpp
 *
 * Copyright(c) 2015-2016 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

/* $Id:$ */

#include "BusSlaveBase.h"
#include "BusSlaveFuncIf.h"
#if defined(_MSC_VER) // for cforestg4x (cforestg4x uses std::min and max)
#undef min
#undef max
#endif

#include "G4_TsIf.h"
#include "G4_CaIss.h"
#include "forest_common.h"

#include "CmErrMsg.h" // for WinMsg

using namespace CmErrMsg; // for WinMsg


/// <summary>constructor</summary>
/// <param name="pCaIss">pointer of CaIss's instance</param>
G4_TsIf::G4_TsIf( G4_CaIss *pCaIss ):
  BusSlaveFuncIf(),
  mpCaIss( pCaIss )
{
}


/// <summary>destructor</summary>
G4_TsIf::~G4_TsIf(void)
{
}


/// <summary>read operation</summary>
/// <param name="offsetAddress">offset address</param>
/// <param name="trans">transaction</param>
/// <param name="t">transport time</param>
/// <param name="debug">access condition (normal or debug)</param>
/// <returns>none</returns>
void G4_TsIf::read(unsigned int     offsetAddress, 
		   TlmBasicPayload& trans, 
		   sc_time*         t,
		   bool             debug)
{
#ifdef EX_MASTER_TEST_LOG
  printf("%s:G4_TsIf::read (start): address=0x%08x, size=%d, local tansport time t=%s\n", 
         sc_time_stamp().to_string().c_str(), (unsigned int)trans.get_address(), trans.get_data_length(), (*t).to_string().c_str());
  fflush(stdout);
#endif
  mpCaIss->MemReadToCForest(trans, debug, t);
#ifdef EX_MASTER_TEST_LOG
  printf("%s:G4_TsIf::read (end): address=0x%08x, size=%d, local tansport time t=%s\n", 
         sc_time_stamp().to_string().c_str(), (unsigned int)trans.get_address(), trans.get_data_length(), (*t).to_string().c_str());
  fflush(stdout);
#endif
}


/// <summary>write operation</summary>
/// <param name="offsetAddress">offset address</param>
/// <param name="trans">transaction</param>
/// <param name="t">transport time</param>
/// <param name="debug">access condition (normal or debug)</param>
/// <returns>none</returns>
void G4_TsIf::write(unsigned int     offsetAddress, 
		    TlmBasicPayload& trans, 
		    sc_time*         t,
		    bool             debug)
{
#ifdef EX_MASTER_TEST_LOG
  printf("%s:G4_TsIf::write (start): address=0x%08x, size=%d, local tansport time t=%s\n", 
         sc_time_stamp().to_string().c_str(), (unsigned int)trans.get_address(), trans.get_data_length(), (*t).to_string().c_str());
  fflush(stdout);
#endif
  mpCaIss->MemWriteToCForest(trans, debug, t);
#ifdef EX_MASTER_TEST_LOG
  printf("%s:G4_TsIf::write (end): address=0x%08x, size=%d, local tansport time t=%s\n", 
         sc_time_stamp().to_string().c_str(), (unsigned int)trans.get_address(), trans.get_data_length(), (*t).to_string().c_str());
  fflush(stdout);
#endif
}


/// <summary>set memory size</summary>
/// <param name="size">memory size</param>
/// <returns>none</returns>
void G4_TsIf::setfunc( ADDRESS_TYPE size )
{
}

