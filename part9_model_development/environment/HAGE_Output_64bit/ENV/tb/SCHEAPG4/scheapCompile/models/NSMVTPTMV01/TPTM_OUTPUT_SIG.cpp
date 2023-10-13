/*************************************************************************
 *
 * TPTM_OUTPUT_SIG.cpp
 *
 * Copyright(c) 2017-2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/
#include "TPTM_OUTPUT_SIG.h"
#include "TptmFunc.h"

using namespace std;

/// <summary>constructor</summary>
/// <param name="name">module name</param>
TPTM_OUTPUT_SIG::TPTM_OUTPUT_SIG(sc_module_name name):
	G4_CommonPrototype(name, "cpu_clk")
	,output("output")
	,sys_resetn("sys_resetn")
	,mOutputSigVal(false)
	,mIsAssertEnable(false)
{

	SC_METHOD(outputSig);
		sensitive << meOutputSig;
		dont_initialize();

	SC_METHOD(assertSig);
		sensitive << meAssertSig;
		dont_initialize();

	SC_METHOD(negertSig);
		sensitive << meNegertSig;
		dont_initialize();

        SC_METHOD(reset);
                sensitive << sys_resetn.neg();
                dont_initialize();

}

/// <summary>destructor</summary>
TPTM_OUTPUT_SIG::~TPTM_OUTPUT_SIG(void) 
{
}


/// <summary>set mIsAssertEnable</summary>
/// <param name="enable">enable or disable</param>
void TPTM_OUTPUT_SIG::setAssertEnable (bool enable)
{
	mIsAssertEnable = enable;
#ifdef TPTM_UT_TEST
        	printFuncName(this->name(), "setAssertEnable");
		fflush(stdout);
#endif //TPTM_UT_TEST
}

/// <summary>output signal</summary>
void TPTM_OUTPUT_SIG::outputSig (void)
{
#ifdef TPTM_UT_TEST
        	printFuncName(this->name(), "outputSig");
		printf("\tmOutputSigVal=%d\n", mOutputSigVal);fflush(stdout);
#endif //TPTM_UT_TEST
	output.write(mOutputSigVal);
}

/// <summary>reset</summary>
void TPTM_OUTPUT_SIG::reset (void)
{
	meAssertSig.cancel();
	meNegertSig.cancel();
	mOutputSigVal = false;
	meOutputSig.notify();
}

/// <summary>notify event for output signal</summary>
/// <param name="assert_time">output signale value</param>
void TPTM_OUTPUT_SIG::pulseOutput (double assert_time)
{
#ifdef TPTM_UT_TEST
        	printFuncName(this->name(), "pulseOutput");
#endif //TPTM_UT_TEST
	meAssertSig.notify(assert_time, glb_resolution_unit);
	meNegertSig.notify(assert_time + (double)mCycle, glb_resolution_unit);
}


/// <summary>notify output sig event for assert </summary>
void TPTM_OUTPUT_SIG::assertSig (void)
{
#ifdef TPTM_UT_TEST
        	printFuncName(this->name(), "assertSig");
		fflush(stdout);
#endif //TPTM_UT_TEST
	if (mIsAssertEnable)
	{
		mOutputSigVal = true;
		meOutputSig.notify(SC_ZERO_TIME);
	}
}

/// <summary>notify output sig event for negert </summary>
void TPTM_OUTPUT_SIG::negertSig (void)
{
#ifdef TPTM_UT_TEST
        	printFuncName(this->name(), "negertSig");
#endif //TPTM_UT_TEST
	mOutputSigVal = false;
	meOutputSig.notify(SC_ZERO_TIME);
}

/// <summary>cancel event for output signal</summary>
void TPTM_OUTPUT_SIG::cancelProcessOutputSig(void)
{
#ifdef TPTM_UT_TEST
        	printFuncName(this->name(), "cancelProcessOutputSig");
#endif //TPTM_UT_TEST
	mOutputSigVal = false;
	meOutputSig.cancel();
	meAssertSig.cancel();
	meNegertSig.cancel();
}

#if 0
//This function is for checking output signal and using VCD.
//Currently this function is not used.
//This funcfion is remained because it is possible to use this function in the future.

// VCD output for debug
/// <summary>enable VCD-output</summary>
/// <param name="tf">pointer to sc_trace_file</param>
/// <returns>none</returns>
void TPTM_OUTPUT_SIG::setTraceEnable( sc_trace_file* tf )
{
	sc_trace( tf, TPTM_IRQ, TPTM_IRQ.name() );
}
#endif

#ifdef TPTM_UT_TEST
/// <summary>Debugging function</summary>
void TPTM_OUTPUT_SIG::printFuncName (const char *this_name
					, const char *func_name)
{
        printf("%s:%s::%s\n"
                ,sc_time_stamp().to_string().c_str()
                ,this_name
                ,func_name);
        fflush(stdout);
}
#endif //TPTM_UT_TEST

#ifdef TPTM_DEBUG
/// <summary>Debugging function</summary>
void TPTM_OUTPUT_SIG::check_mIsAssertEnable (void)
{
        printf("%s:%s::check_mIsAssertEnable = %d\n"
                ,sc_time_stamp().to_string().c_str()
		,this->name()
		,mIsAssertEnable);
        fflush(stdout);
}
#endif //TPTM_DEBUG

