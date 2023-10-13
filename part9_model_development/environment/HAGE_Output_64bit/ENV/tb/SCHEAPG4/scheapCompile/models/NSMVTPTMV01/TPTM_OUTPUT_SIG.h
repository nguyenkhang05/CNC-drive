/*************************************************************************
 *
 * TPTM_OUTPUT_SIG.h
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
#ifndef __TPTM_OUTPUT_SIG__
#define __TPTM_OUTPUT_SIG__

#include "global.h"
#include "BusSlaveBase.h"
#include "G4_CommonPrototype.h"

class TPTM_OUTPUT_SIG 
: public G4_CommonPrototype
{
public:

//constructor destructor
	SC_HAS_PROCESS(TPTM_OUTPUT_SIG);

	TPTM_OUTPUT_SIG(sc_module_name name = "TPTM_OUTPUT_SIG"); 
	~TPTM_OUTPUT_SIG(void);

//variable
public:
	sc_out<bool> output; // output compare match interrupt
	sc_in<bool> sys_resetn; // reset port 

protected:
	sc_event meOutputSig;//event of output signal
	sc_event meAssertSig;//event of assert signal
	sc_event meNegertSig;//event of negert signal
	bool mOutputSigVal; // ouput signal value
	bool mIsAssertEnable; //output enable

//function
protected:

	void outputSig(void);	
	void assertSig (void);
	void negertSig (void);

	void reset (void);
public: 
	void pulseOutput (double assert_time);
	void cancelProcessOutputSig(void);
	void setAssertEnable(bool);
#if 0
//This function is for checking output signal and using VCD.
//Currently this function is not used.
//This funcfion is remained because it is possible to use this function in the future.

	void setTraceEnable( sc_trace_file* tf );
#endif

#ifdef TPTM_UT_TEST
        void printFuncName (const char *this_name, const char *func_name);
#endif //TPTM_UT_TEST 

#ifdef TPTM_DEBUG
	void check_mIsAssertEnable(void);
#endif //TPTM_DEBUG
};
#endif
