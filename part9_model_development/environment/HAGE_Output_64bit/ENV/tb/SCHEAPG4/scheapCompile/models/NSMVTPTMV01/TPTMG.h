/*************************************************************************
 *
 * TPTMG.h
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
#ifndef __TPTMG__
#define __TPTMG__

#include <queue>

#include "global.h"
#include "BusSlaveBase.h"
#include "G4_CommonPrototype.h"

#include "TPTM_def.h"

class TPTM;
class TptmgFunc;

class TPTMG 
: public G4_CommonPrototype, public BusSlaveBase<BUS_WIDTH_VCI32,1>
{
public:

//constructor destructor
	SC_HAS_PROCESS(TPTMG);

	TPTMG(sc_module_name name = "TPTMG"); 
	~TPTMG(void);


//variable
public:
	TlmTargetSocket<BUS_WIDTH_VCI32> *ts; // target socket 
	sc_in<bool> sys_resetn; // reset port 
	
private:
	TptmgFunc *mpFunc; //pointer of functional part ince

//function
public: 
	
//function

	//METHOD
	void reset (void);
	//func
	void setMemVar(unsigned char penum, unsigned char ch_tptmg, TPTM** tptm);
	void setTPTMnUTRG(unsigned char pe_id, unsigned char ch, unsigned char* reg_utrg);

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
public:
	void printTPTMnUTRG(unsigned char pe, unsigned char ch);
#endif //TPTM_DEBUG_U2A
};
#endif
