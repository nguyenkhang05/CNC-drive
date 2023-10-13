/*************************************************************************
 *
 * UP_COUNTER_TIMER.h
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
#ifndef __UP_COUNTER_TIMER__
#define __UP_COUNTER_TIMER__

#include "global.h"
#include "BusSlaveBase.h"
#include "G4_CommonPrototype.h"
#include "TPTM.h"
#include "TPTM_OUTPUT_SIG.h"

#include "TPTM_def.h"

class TPTM;
class TPTM_OUTPUT_SIG;


class UP_COUNTER_TIMER 
: 	public G4_CommonPrototype
{
public:

//constructor destructor
	SC_HAS_PROCESS(UP_COUNTER_TIMER);

	UP_COUNTER_TIMER(sc_module_name name = "UP_COUNTER_TIMER"); 
	~UP_COUNTER_TIMER(void);

//variable
public:
	sc_out<bool> INTTPTMU[G4TPTM_UP_COUNTER_TIMER_CMP_NUM]; // output compare match interrupt
	sc_in<bool> sys_resetn; // reset port 

private:
	union 
	{
		unsigned int byte4;
		unsigned char byte1[4];
	} mRegTPTMmUCNTx; // TPTM up couter timer count
	union 
	{
		unsigned int byte4[4];
		unsigned char byte1[16];
	} mRegTPTMmUCMPx; // TPTM up couter timer compare
	sc_event meStartCountUp; //event of start count down
	sc_event meRestartCountUp; //event of restart count down
	sc_event meStopCountUp;//event of stop count down
	sc_event meOverflow;//event of overflow
	unsigned char mChNum; //channel index

	unsigned char *mpRegTPTMSUSTR;	// pointer of TPTM up couter timer status register
	unsigned char *mpRegTPTMSUDIV;// pointer of TPTM up couter timer clock division register

	sc_dt::uint64 mCountRegUpdateTime; // This is the time to update count register
	sc_dt::uint64 mInterDivCountTime; // up couter clock division time

public:
	TPTM_OUTPUT_SIG* mTptmOutputSig[G4TPTM_UP_COUNTER_TIMER_CMP_NUM]; // output signal for compare match 

//function
private:
	void startCountUp (void);
	void restartCountUp (void);
	void stopCountUp (void);
	void processOverflow (void);

	void reset (void);
	inline void setCountValue(void);
public: 
	void setMemFromTPTM(unsigned char ch_num);
	void setMemFromTptmFunc (unsigned char* reg_str
				,unsigned char* reg_div 
				);
	void setAssertEnable(unsigned char data);
	void readCountReg(unsigned char*data
				,unsigned int len
				,unsigned int off_add);
	void readCmpReg(unsigned char*data
				,unsigned int len
				,unsigned int off_add);
	void writeCountReg (unsigned char*data
				,unsigned int len
				,unsigned int off_add);
	void writeCmpReg  (unsigned char*data
				,unsigned int len
				,unsigned int off_add);
	unsigned int getCountReg(void);
	unsigned int getCmpReg(unsigned int off_add);
	void notifyStartCountUp (void);
	void notifyStopCountUp (void);
	void notifyRestartCountUp (void);
#if 0
//This function is for checking output signal and using VCD.
//Currently this function is not used.
//This funcfion is remained because it is possible to use this function in the future.

	void setTraceEnable( sc_trace_file* tf );
#endif

#ifdef TPTM_UT_TEST
        void printFuncName (const char *this_name, const char *func_name);
#endif //TPTM_UT_TEST 
};

#endif
