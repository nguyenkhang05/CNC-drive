/*************************************************************************
 *
 * FREE_RUN_TIMER.h
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
#ifndef __FREE_RUN_TIMER__
#define __FREE_RUN_TIMER__

#include "global.h"
#include "BusSlaveBase.h"
#include "G4_CommonPrototype.h"

#include "TPTM_def.h"

class TPTM;

class FREE_RUN_TIMER 
: public G4_CommonPrototype
{
public:

//constructor destructor
	SC_HAS_PROCESS(FREE_RUN_TIMER);

	FREE_RUN_TIMER(sc_module_name name = "FREE_RUN_TIMER"); 
	~FREE_RUN_TIMER(void);

//variable
        sc_in<bool> sys_resetn; // reset port 
private:
	union
        {
                unsigned int byte4;
                unsigned char byte1[4];
        } mRegTPTMnFCNTx; // TPTM free timer count register
	sc_event meStartCountUp;  //event of start count up
	sc_event meRestartCountUp; //event of restart count up
	sc_event meStopCountUp; //event of stop count up
	sc_event meOverflow; //event of underflow
	
	unsigned char *mpRegTPTMSFDIV; // pointer of TPTM free run timer clock division register
	unsigned char *mpRegTPTMSFSTR;  // pointer of TPTM free run timer status register

	sc_dt::uint64 mCountRegUpdateTime; // this is the time to update count register
	sc_dt::uint64 mInterDivCountTime; // free run clock division time for setCountValue() 

//function
private:
	void startCountUp (void);
	void restartCountUp (void);
	void stopCountUp (void);
	void processOverflow(void);
	void reset(void);
	inline void setCountValue(void);
public: 
	void setMemFromTptmFunc (unsigned char* reg_str
				, unsigned char* reg_div); 
	void readCountReg(unsigned char*data
                                ,unsigned int len
                                ,unsigned int off_add);
	void writeCountReg (unsigned char*data
                                ,unsigned int len
                                ,unsigned int off_add);
	void notifyStartCountUp (void);
	void notifyStopCountUp (void);
	void notifyRestartCountUp (void);

#ifdef TPTM_UT_TEST
        void printFuncName (const char *this_name, const char *func_name);
#endif //TPTM_UT_TEST 
	
};
#endif
