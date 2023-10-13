/*************************************************************************
 *
 * BARRIER_SYNC.h
 *
 * Copyright(c) 2016 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/
#ifndef __BARRIER_SYNC__
#define __BARRIER_SYNC__

#include "global.h"
#include "BusSlaveBase.h"

#define G4BARR_PE_OFF_ADD 0x100
#define G4BARR_PE_NUM_INIT 2 //this value is different to other model

class BarrierSyncFunc;

class BARRIER_SYNC 
: public sc_module, public BusSlaveBase<BUS_WIDTH_VCI32,1>
{
public:

//constructor destructor
	SC_HAS_PROCESS(BARRIER_SYNC);

	BARRIER_SYNC(sc_module_name name = "BARRIER_SYNC"); 
	~BARRIER_SYNC(void);


//variable
public:
	TlmTargetSocket<BUS_WIDTH_VCI32> *ts; // target socket 
	sc_in<bool> sys_resetn; // reset port 
	sc_in<sc_dt::uint64> cpu_clk; // clock port
	unsigned char mPeNum;	 // the number of PE
	
private:
	
	BarrierSyncFunc *mpFunc; //pointer of functional part ince
	sc_event meWriteBRnSYNCm; //event of write BRnSYNCm
	sc_time mCycle; //one cycle
//function
public: 
	
//function
	//METHOD;
	void writeBRnSYNCm(void) ;
	void calCycle (void);
	void reset (void);
	//func
	void notify_writeBRnSYNCm (void);
	void setPeNum (unsigned char penum);
};
#endif
