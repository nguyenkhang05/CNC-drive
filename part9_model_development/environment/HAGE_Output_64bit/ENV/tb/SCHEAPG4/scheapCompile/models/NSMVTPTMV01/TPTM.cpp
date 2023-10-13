/*************************************************************************
 *
 * TPTM.cpp
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
#include "TPTM.h"
#include "TPTMG.h"
#include "INTERVAL_TIMER.h"
#include "FREE_RUN_TIMER.h"
#include "UP_COUNTER_TIMER.h"
#include "TptmFunc.h"

/// <summary>constructor</summary>
/// <param name="name">module name</param>
TPTM::TPTM(sc_module_name name):
	G4_CommonPrototype(name, "cpu_clk")
	,BusSlaveBase<BUS_WIDTH_VCI32,1>()
	,sys_resetn("sys_resetn")
	,TPTM_IRQ("TPTM_IRQ")
    	,ts(0)
    	,mpFunc( 0 )
	,mpFreeRunTimer(0)
	,mpOrForIntTimer(0)
	,mpOrForUpTimer()
{
#ifdef TPTM_U2A_DEBUG
	printFuncName(this->name(), "constructor");
#endif //TPTM_U2A_DEBUG
	//OrUnit instance for INTEVAL_TIMER. bind TPTM and OrUnit output port
	mpOrForIntTimer = new 
		OrUnit<bool, G4TPTM_INTERVAL_TIMER_CH_NUM_MAX>
		("TPTM_OrForIntTimer");
	(*(mpOrForIntTimer->out_port))(this->TPTM_IRQ);
	
	//OrUnit instance for UP_COUNTER_TIMER. bind TPTM and OrUnit output port
	for (unsigned char cmp_num = 0; 
		cmp_num < G4TPTM_UP_COUNTER_TIMER_CMP_NUM; cmp_num++)
	{ 
		char str[256];
		sprintf(str, "TPTM_OrUpTimerForUpTimer%d", cmp_num);
		mpOrForUpTimer[cmp_num] = new 
			OrUnit<bool, G4TPTM_UP_COUNTER_TIMER_CH_NUM>
			(str);
		(*(mpOrForUpTimer[cmp_num]->out_port))
			(this->INTTPTMU[cmp_num]);
	}

	//Interval timer instance
	for (unsigned char ch = 0; ch < G4TPTM_INTERVAL_TIMER_CH_NUM_MAX; ch++)
	{
		char str[256];
		sprintf(str, "INTERVAL_TIMER_ch%d", ch);
		mpIntervalTimer[ch] = new INTERVAL_TIMER(str);
		mpIntervalTimer[ch]->setMemFromTPTM(ch);

		//bind OrUnit input port and IntervalTimer output port
		sprintf(str, "tptm_irq_sig%d", ch);
		this->tptm_irq_sig[ch] = sc_signal<bool>(str);

		(*(mpOrForIntTimer->in_port[ch]))(this->tptm_irq_sig[ch]);
		(mpIntervalTimer[ch]->TPTM_IRQ)(this->tptm_irq_sig[ch]);
		mpIntervalTimer[ch]->notifyOutputSig(false);
		mpIntervalTimer[ch]->clk(this->clk);
		mpIntervalTimer[ch]->sys_resetn(this->sys_resetn);
#ifdef TPTM_U2A_DEBUG
		printf("\t%s sig_name=%s\n", mpIntervalTimer[ch]->name(), tptm_irq_sig[ch].name());
#endif //TPTM_U2A_DEBUG
	}

	//FreeRun timer instance
	mpFreeRunTimer = new FREE_RUN_TIMER("FREE_RUN_TIMER");
	mpFreeRunTimer->clk(this->clk);
	mpFreeRunTimer->sys_resetn(this->sys_resetn);
	
	//UpCounterTimer instance
	for (unsigned char ch = 0; ch < G4TPTM_UP_COUNTER_TIMER_CH_NUM; ch++)
	{
		char str[256];
		sprintf(str, "UP_COUNTER_TIMER_ch%d", ch);
		mpUpCounterTimer[ch] = new UP_COUNTER_TIMER(str);
		mpUpCounterTimer[ch]->setMemFromTPTM(ch);
		//bind OrUnit input port and UpCounterTimer output port
		for (unsigned char cmp_num = 0; 
			cmp_num < G4TPTM_UP_COUNTER_TIMER_CMP_NUM; cmp_num++)
		{ 
			sprintf(str, "inttptmu_sig%d%d", ch, cmp_num);
			this->inttptmu_sig[ch][cmp_num] = sc_signal<bool>(str);
			(*(mpOrForUpTimer[cmp_num]->in_port[ch]))
				(this->inttptmu_sig[ch][cmp_num]);
			
			(mpUpCounterTimer[ch]->INTTPTMU[cmp_num])
				(this->inttptmu_sig[ch][cmp_num]);
#ifdef TPTM_U2A_DEBUG
			printf("\t%s sig_name=%s\n", mpUpCounterTimer[ch]->name(), inttptmu_sig[ch][cmp_num].name());
#endif //TPTM_U2A_DEBUG
		}
		mpUpCounterTimer[ch]->clk(this->clk);
		mpUpCounterTimer[ch]->sys_resetn(this->sys_resetn);
	}
	
	// To need process when inheriting BusSlaveBase
	setSlaveResetPort32(&sys_resetn);
	setSlaveFreqPort32(&clk);
	setTargetSocket32("ts");
	ts = tSocket32[0];
	mBusSlaveIf32[0]->setBusProtocol( BUS_VCI );
	mpFunc = new TptmFunc(this);
	mBusSlaveIf32[0]->setFuncModulePtr( mpFunc );
	//Enable to check transaction length
	mBusSlaveIf32[0]->setEnableToCheckDataSize();
	
	// SystemC process
	SC_METHOD(reset);
		sensitive << sys_resetn.neg();
		dont_initialize();
}

/// <summary>destructor</summary>
TPTM::~TPTM(void) 
{
	for (unsigned char ch = 0; 
		ch < G4TPTM_INTERVAL_TIMER_CH_NUM_MAX;
		ch++)
	{
		delete mpIntervalTimer[ch];
	}
	delete mpOrForIntTimer;
	delete mpFreeRunTimer;
	for (unsigned char ch = 0; 
		ch < G4TPTM_UP_COUNTER_TIMER_CH_NUM;
		ch++)
	{
		delete mpUpCounterTimer[ch];
	}
	for (unsigned char cmp = 0; cmp < G4TPTM_UP_COUNTER_TIMER_CMP_NUM; cmp++)
	{
		delete mpOrForUpTimer[cmp];
	}
	delete mpFunc;
}

/// <summary>reset</summary>
void TPTM::reset (void)
{
	mpFunc->reset();	
}

///<summary>get pointer of mReg from mpFunc(call API of mpFunc)</summary>
/// <param name="off_add">off_add</param>
/// <returns>pointer of mReg[off_add] from mpFunc</returns>
unsigned char* TPTM::getRegPntr( unsigned int off_add)
{
	return(mpFunc->getRegPntr(off_add));	
}

///<summary>set the number of interval timer channel in mpFunc(call API of mpFunc)</summary>
/// <param name="ch">the number of interval timer channel</param>
void TPTM::setIntrvlTmrChNum( unsigned char ch)
{
	mpFunc->setIntrvlTmrChNum(ch);
}

///<summary>set Threshold for Waring Message of TPTMmCMPni</summary>
///<param name="threshold">threshold</param>
void TPTM::setThresholdForWrnCmpReg(unsigned int threshold)
{
	if (mpFunc != NULL)
	{	
		mpFunc->setThresholdForWrnCmpReg(threshold);
	}
}

#if 0
//This function is for checking output signal and using VCD.
//Currently this function is not used.
//This funcfion is remained because it is possible to use this function in the future.

// VCD output for debug
/// <summary>enable VCD-output</summary>
/// <param name="tf">pointer to sc_trace_file</param>
/// <returns>none</returns>
void TPTM::setTraceEnable( sc_trace_file* tf )
{
	for (unsigned char ch = 0; ch < 4; ch++)
	{
	        mpIntervalTimer[ch]->setTraceEnable(tf);
	}
}
#endif

#ifdef TPTM_UT_TEST
/// <summary>Debugging function</summary>
void TPTM::printFuncName (const char *this_name, const char *func_name)
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
void TPTM::setTPTMG(unsigned char ch, TPTMG* tptmg)
{
	mpTPTMG[ch] = tptmg;
}
#endif //TPTM_DEBUG_U2A

