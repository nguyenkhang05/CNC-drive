/*************************************************************************
 *
 * BARRIER_SYNC.cpp
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
#include "BARRIER_SYNC.h"
#include "BarrierSyncFunc.h"

/// <summary>constructor</summary>
/// <param name="name">module name</param>
BARRIER_SYNC::BARRIER_SYNC(sc_module_name name):
	sc_module(name),
	BusSlaveBase<BUS_WIDTH_VCI32,1>(),
	sys_resetn("sys_resetn"),
	cpu_clk("cpu_clk"),
    	ts(0),
    	mpFunc( 0 ),
	meWriteBRnSYNCm(),
	mPeNum(G4BARR_PE_NUM_INIT)
{
	mCycle = sc_time(0, glb_resolution_unit);
		
	// To need process when inheriting BusSlaveBase
	setSlaveResetPort32(&sys_resetn);
	setSlaveFreqPort32(&cpu_clk);
	setTargetSocket32("ts");
	ts = tSocket32[0];
	mBusSlaveIf32[0]->setBusProtocol( BUS_VCI );
	mpFunc = new BarrierSyncFunc(this);
	mBusSlaveIf32[0]->setFuncModulePtr( mpFunc );
	

	SC_METHOD(writeBRnSYNCm);
		sensitive << meWriteBRnSYNCm;
		dont_initialize();
	SC_METHOD(calCycle);
		sensitive << cpu_clk;
		dont_initialize();
	SC_METHOD(reset);
		sensitive << sys_resetn.neg();;
		dont_initialize();

#ifdef BARRDEBUG
	printf("BARRIER_SYNC\tinstance name\t%s\n", this->name()); fflush(stdout);
#endif //BARRDEBUG
}

/// <summary>destructor</summary>
BARRIER_SYNC::~BARRIER_SYNC(void) 
{
	delete mpFunc;
}

/// <summary>Set the number of PE which is got by reading configulation file in NSMVG4BARRV01 </summary>
/// <param name="penum">the number of PE</param>
void BARRIER_SYNC::setPeNum (unsigned char penum)
{	
	mPeNum = penum;
	mpFunc->initParam();
}

/// <summary>Call mpFunc->writeBRnSYNC</summary>
void BARRIER_SYNC::writeBRnSYNCm(void) 
{	mpFunc->writeBRnSYNCm();	
}

/// <summary>calculate 1 cycle</summary>
void BARRIER_SYNC::calCycle (void)
{
	sc_dt::uint64 tmp_freq;
	if( cpu_clk.read() > 0 )
	{
      		switch(glb_resolution_unit)
		{
      			case SC_SEC:
			tmp_freq = (uint64)(1.0)/cpu_clk.read();
			break;
      			case SC_MS:
      		  	tmp_freq = (uint64)(1.0e+3)/cpu_clk.read();
      		  	break;
      			case SC_US:
      		  	tmp_freq = (uint64)(1.0e+6)/cpu_clk.read();
      		  	break;
      			case SC_NS:
      		  	tmp_freq = (uint64)(1.0e+9)/cpu_clk.read();
      		  	break;
      			case SC_PS:
      		  	tmp_freq = (uint64)(1.0e+12)/cpu_clk.read();
      		  	break;
      			case SC_FS:
			tmp_freq = (uint64)(1.0e+15)/cpu_clk.read();
			break;
      		}
		mCycle = sc_time((double)(tmp_freq) , glb_resolution_unit);
    	}

}

/// <summary>reset</summary>
void BARRIER_SYNC::reset (void)
{
	//mCycle = sc_time(0, glb_resolution_unit);
	meWriteBRnSYNCm.cancel();
	mpFunc->reset();	
	
#ifdef BARRTEST
	printf("\n%s:%s::%s\n", sc_time_stamp().to_string().c_str(),name(),"reset");
	printf("  mCycle=%s\n", mCycle.to_string().c_str());
	fflush(stdout);
#endif //BARRTEST
}

/// <summary>Notify event to write BRnSYNCm</summary>
void BARRIER_SYNC::notify_writeBRnSYNCm (void)
{	
	meWriteBRnSYNCm.notify(mCycle);	
}
