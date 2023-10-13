/*************************************************************************
 *
 * TPTMG.cpp
 *
 * Copyright(c) 2017-2018 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/
#include "TPTM.h"
#include "TPTMG.h"
#include "TptmgFunc.h"

/// <summary>constructor</summary>
/// <param name="name">module name</param>
TPTMG::TPTMG(sc_module_name name):
	G4_CommonPrototype(name, "cpu_clk")
	,BusSlaveBase<BUS_WIDTH_VCI32,1>()
	,sys_resetn("sys_resetn")
    	,ts(0)
    	,mpFunc( 0 )
{


	// To need process when inheriting BusSlaveBase
	setSlaveResetPort32(&sys_resetn);
	setSlaveFreqPort32(&clk);
	setTargetSocket32("ts");
	ts = tSocket32[0];
	mBusSlaveIf32[0]->setBusProtocol( BUS_VCI );
	mpFunc = new TptmgFunc(this);
	mBusSlaveIf32[0]->setFuncModulePtr( mpFunc );
	//Enable to check transaction length
	mBusSlaveIf32[0]->setEnableToCheckDataSize();
	
	// SystemC process
	SC_METHOD(reset);
		sensitive << sys_resetn.neg();
		dont_initialize();
}

/// <summary>destructor</summary>
TPTMG::~TPTMG(void) 
{
	delete mpFunc;
}

/// <summary>reset</summary>
void TPTMG::reset (void)
{
	mpFunc->reset();	
}

/// <summary>set PE_NUM and pointer of TPTM object</summary>
/// <param name="penum">the number of PE</param>
/// <param name="ch_tptmg">channle ID of TPTMG</param>
/// <param name="tptm">pointer of TPTM object</param>
void TPTMG::setMemVar(unsigned char penum, unsigned char ch_tptmg, TPTM** tptm)
{
	mpFunc->setMemVar(penum, ch_tptmg, tptm);
}

/// <summary>set TPTMnUTRG(which is included in TptmFunc) from NSMVTPTMV01 </summary>
/// <param name="pe_id">PE ID</param>
/// <param name="ch">channel ID</param>
/// <param name="reg_utrg">pointer of TPTMnUTRG register</param>
void TPTMG::setTPTMnUTRG(unsigned char pe_id, unsigned char ch, unsigned char* reg_utrg)
{
	mpFunc->setTPTMnUTRG(pe_id, ch, reg_utrg);
}


#if 0
//This function is for checking output signal and using VCD.
//Currently this function is not used.
//This funcfion is remained because it is possible to use this function in the future.

// VCD output for debug
/// <summary>enable VCD-output</summary>
/// <param name="tf">pointer to sc_trace_file</param>
/// <returns>none</returns>
void TPTMG::setTraceEnable( sc_trace_file* tf )
{
	for (unsigned char ch = 0; ch < 4; ch++)
	{
	        mpIntervalTimer[ch]->setTraceEnable(tf);
	}
}
#endif

#ifdef TPTM_UT_TEST
/// <summary>Debugging function</summary>
void TPTMG::printFuncName (const char *this_name, const char *func_name)
{
        printf("%s:%s::%s\n"
                ,sc_time_stamp().to_string().c_str()
                ,this_name
                ,func_name);
        fflush(stdout);
}

#endif //TPTM_UT_TEST

#ifdef TPTM_DEBUG_U2A
/// <summary>Debugging function</summary>
void TPTMG::printTPTMnUTRG (unsigned char pe, unsigned char ch)
{
	mpFunc->printTPTMnUTRG(pe, ch);
}
#endif //TPTM_DEBUG_U2A
