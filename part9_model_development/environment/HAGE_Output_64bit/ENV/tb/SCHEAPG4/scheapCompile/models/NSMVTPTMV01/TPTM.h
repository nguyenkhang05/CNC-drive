/*************************************************************************
 *
 * TPTM.h
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
#ifndef __TPTM__
#define __TPTM__

#include <queue>

#include "global.h"
#include "BusSlaveBase.h"
#include "OrUnit.h"
#include "G4_CommonPrototype.h"

#include "TPTM_def.h"

#ifdef TPTM_DEBUG_U2A
#include "TPTMG.h"
#endif // TPTM_DEBUG_U2A


class INTERVAL_TIMER;
class FREE_RUN_TIMER;
class UP_COUNTER_TIMER;
class TptmFunc;


class TPTM 
: public G4_CommonPrototype, public BusSlaveBase<BUS_WIDTH_VCI32,1>
{
public:

//constructor destructor
	SC_HAS_PROCESS(TPTM);

	TPTM(sc_module_name name = "TPTM"); 
	~TPTM(void);


//variable
public:
	TlmTargetSocket<BUS_WIDTH_VCI32> *ts; // target socket 
	sc_in<bool> sys_resetn; // reset port 
	sc_out<bool> TPTM_IRQ; // output interrupt port for INTERVAL_TIMER
	sc_out<bool> INTTPTMU[G4TPTM_UP_COUNTER_TIMER_CMP_NUM]; // output interrupt port for UP_COUNTER_TIMER
	
	INTERVAL_TIMER *mpIntervalTimer[G4TPTM_INTERVAL_TIMER_CH_NUM_MAX]; // pointer of INTERVAL_TIMER instance
	FREE_RUN_TIMER *mpFreeRunTimer; // pointer of FREE_RUN_TIMER instance
	UP_COUNTER_TIMER *mpUpCounterTimer[G4TPTM_UP_COUNTER_TIMER_CH_NUM]; // pointer of UP_COUNTER_TIMER instance
		
private:
	OrUnit<bool, G4TPTM_INTERVAL_TIMER_CH_NUM_MAX>* mpOrForIntTimer; //pointer of OrUnit instance for INTERVAL_TIMER 
	OrUnit<bool, G4TPTM_UP_COUNTER_TIMER_CH_NUM >* mpOrForUpTimer[G4TPTM_UP_COUNTER_TIMER_CMP_NUM]; //pointer of OrUnit instance for UP_COUNTER_TIMER 
	
	TptmFunc *mpFunc; //pointer of functional part ince

	sc_signal<bool> tptm_irq_sig[G4TPTM_INTERVAL_TIMER_CH_NUM_MAX]; //signal for binding INTERVAL_TIMER and OrUnit
	sc_signal<bool> inttptmu_sig[G4TPTM_UP_COUNTER_TIMER_CH_NUM][G4TPTM_UP_COUNTER_TIMER_CMP_NUM]; //signal for binding UP_COUNTER_TIMER and OrUnit

//function
public: 
	
//function

	//METHOD
	void reset (void);
	//func
	unsigned char* getRegPntr( unsigned int off_add);
	void setIntrvlTmrChNum( unsigned char ch);	
	void setThresholdForWrnCmpReg(unsigned int threshold);

#if 0
//This function is for checking output signal and using VCD.
//Currently this function is not used.
//This funcfion is remained because it is possible to use this function in the future.
	void setTraceEnable( sc_trace_file* tf );
#endif

#ifdef TPTM_UT_TEST
	void printFuncName (const char *this_name, const char *func_name);
#endif //TPTM_UT_TEST 

#ifdef TPTM_DEBUG_U2A
public :
	TPTMG* mpTPTMG[2]; //Debugging member variable
	void setTPTMG(unsigned char ch, TPTMG* tptmg);
#endif //TPTM_DEBUG_U2A
};
#endif
