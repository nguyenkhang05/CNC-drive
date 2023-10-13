/*************************************************************************
 *
 * INTERVAL_TIMER.h
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
#ifndef __INTERVAL_TIMER__
#define __INTERVAL_TIMER__

#include "global.h"
#include "BusSlaveBase.h"
#include "G4_CommonPrototype.h"

#include "TPTM_def.h"

class TPTM;

class INTERVAL_TIMER 
: public G4_CommonPrototype
{
public:

//constructor destructor
	SC_HAS_PROCESS(INTERVAL_TIMER);

	INTERVAL_TIMER(sc_module_name name = "INTERVAL_TIMER"); 
	~INTERVAL_TIMER(void);

//variable
public:
	sc_out<bool> TPTM_IRQ; // output underflow interrupt
	sc_in<bool> sys_resetn; // reset port 

private:
	union 
	{
		unsigned int byte4;
		unsigned char byte1[4];
	} mRegTPTMnICNTx, mRegTPTMnILDx; // TPTM interval timer count and load register
	sc_event meStartCountDown; //event of start count down
	sc_event meRestartCountDown; //event of restart count down
	sc_event meStopCountDown;//event of stop count down
	sc_event meUnderflow;//event of underflow
	sc_event meOutputSig;//event of output signal
	unsigned char mChNum; //channel index
	bool mOutputSigVal; //output signal value

	unsigned char *mpRegTPTMSISTR;	// pointer of TPTM interval timer status register
	unsigned char *mpRegTPTMSIIEN; // pointer of TPTM interval timer interrupt enable  register
	unsigned char *mpRegTPTMSIUSTR;// pointer of TPTM interval timer underflow status register
	unsigned char *mpRegTPTMSIDIV;// pointer of TPTM interval timer clock division register

	sc_dt::uint64 mCountRegUpdateTime; // This is the time to update count register
	sc_dt::uint64 mInterDivCountTime; // interval clock division time 
//function
private:
	void startCountDown (void);
	void restartCountDown (void);
	void stopCountDown (void);
	void processUnderflow (void);
	void outputSig(void);
	void reset (void);
	inline void setCountValue(void);
public: 
	void setMemFromTPTM(unsigned char ch_num);
	void setMemFromTptmFunc (unsigned char* reg_str
				,unsigned char* reg_en
				,unsigned char* reg_ustr
				,unsigned char* reg_div );
	void readCountReg(unsigned char*data
				,unsigned int len
				,unsigned int off_add);
	void readLoadReg(unsigned char*data
				,unsigned int len
				,unsigned int off_add);
	void writeCountReg (unsigned char*data
				,unsigned int len
				,unsigned int off_add);
	void writeLoadReg  (unsigned char*data
				,unsigned int len
				,unsigned int off_add);
	void notifyStartCountDown (void);
	void notifyStopCountDown (void);
	void notifyRestartCountDown (void);
	void notifyOutputSig (bool sig_val);
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
