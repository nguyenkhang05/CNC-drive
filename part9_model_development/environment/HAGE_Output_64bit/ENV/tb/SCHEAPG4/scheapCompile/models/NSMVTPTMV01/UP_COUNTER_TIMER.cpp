/*************************************************************************
 *
 * UP_COUNTER_TIMER.cpp
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
#include "UP_COUNTER_TIMER.h"
#include "TptmFunc.h"


/// <summary>constructor</summary>
/// <param name="name">module name</param>
UP_COUNTER_TIMER::UP_COUNTER_TIMER(sc_module_name name):
	G4_CommonPrototype(name, "cpu_clk")
	,sys_resetn("sys_resetn")
	,INTTPTMU()
        ,mChNum(0)
	,mpRegTPTMSUSTR(0)
        ,mpRegTPTMSUDIV(0)
        ,mCountRegUpdateTime(0)
{
	mRegTPTMmUCNTx.byte4 = 0x0;
	for (unsigned int cmp_id = 0; 
		cmp_id < G4TPTM_UP_COUNTER_TIMER_CMP_NUM; cmp_id++)
	{
		mRegTPTMmUCMPx.byte4[cmp_id] = 0x0;

		char str[32];
		sprintf(str, "INTTPTMU%d", cmp_id);
		mTptmOutputSig[cmp_id] = new TPTM_OUTPUT_SIG(str);
		(mTptmOutputSig[cmp_id]->output)
			(this->INTTPTMU[cmp_id]);
		(mTptmOutputSig[cmp_id]->sys_resetn)
			(sys_resetn);
		(mTptmOutputSig[cmp_id]->clk)
			(clk);
	}

	SC_METHOD(startCountUp);
		sensitive << meStartCountUp;
		dont_initialize();

	SC_METHOD(restartCountUp);
		sensitive << meRestartCountUp;
		dont_initialize();

	SC_METHOD(stopCountUp);
		sensitive << meStopCountUp;
		dont_initialize();

	SC_METHOD(processOverflow);
		sensitive << meOverflow;
		dont_initialize();
	
        SC_METHOD(reset);
                sensitive << sys_resetn.neg();
                dont_initialize();

}

/// <summary>destructor</summary>
UP_COUNTER_TIMER::~UP_COUNTER_TIMER(void) 
{
	 
	for (unsigned char cmp_id = 0; 
		cmp_id < G4TPTM_UP_COUNTER_TIMER_CMP_NUM; cmp_id++)
	{
		delete mTptmOutputSig[cmp_id];
	}
}




/// <summary>Set channel index. This Func is called by TPTM constructor</summary>
/// <param name="ch_num">chnannel index</param>
void UP_COUNTER_TIMER::setMemFromTPTM(unsigned char ch_num)
{
	mChNum = ch_num;
}

/// <summary>Set assert enable for mTptmOutputSig. This Func is called by TptmFunc write()</summary>
/// <param name="data">transaction data</param>
void UP_COUNTER_TIMER::setAssertEnable(unsigned char data)
{
#ifdef TPTM_DEBUG_U2A
	printFuncName(this->name(), "setAssertEnable");
#endif //TPTM_DEBUG_U2A
	for (unsigned char cmp_id = 0; 
		cmp_id < G4TPTM_UP_COUNTER_TIMER_CMP_NUM;
		cmp_id++)
	{
		mTptmOutputSig[cmp_id]
			->setAssertEnable((bool)((data >> cmp_id) & 0x1));	
	}
}

/// <summary>Set pointers of 4 registers. This Func is called by TptmFunc constructor</summary>
/// <param name="reg_str">status register</param>
/// <param name="reg_div">clock division register</param>
void UP_COUNTER_TIMER::setMemFromTptmFunc (unsigned char* reg_str
                                	,unsigned char* reg_div
					)
{
	mpRegTPTMSUSTR = reg_str;
	mpRegTPTMSUDIV = reg_div;
}

/// <summary>Set current count value to count register</summary>
void UP_COUNTER_TIMER::setCountValue (void)
{
	
	//if timer is running, set value count register 
	if (((*mpRegTPTMSUSTR >> mChNum) & 0x1) == 0x1)
	{
		sc_dt::uint64 now_time = sc_time_stamp().value();
		sc_dt::uint64 timer_period = ((sc_dt::uint64)*mpRegTPTMSUDIV 
					+ 1) * mCycle;
		sc_dt::uint64 past_time = now_time - mCountRegUpdateTime 
					+ mInterDivCountTime;

		//The max value of R.H.S is 32bit.
		unsigned int cnt = past_time / timer_period;
		sc_dt::uint64 int_div_cnt_time = past_time % timer_period;

		if (0 < cnt || 0 <int_div_cnt_time)
		{
			mRegTPTMmUCNTx.byte4 += cnt;
			mInterDivCountTime = int_div_cnt_time;
			mCountRegUpdateTime = now_time;
		}

#ifdef TPTM_DEBUG
	printf("\tmRegTPTMmUCNTx.byte4=0x%x \n\tmInterDivCountTime=%d\n", mRegTPTMmUCNTx.byte4, mInterDivCountTime); fflush(stdout); 
#endif //TPTM_DEBUG
	}
}

/// <summary>Read count register.This func is called by TptmFunc</summary>
/// <param name="data">pointer of transaction data</param>
/// <param name="len">transaction data length</param>
/// <param name="off_add">offset address</param>
void UP_COUNTER_TIMER::readCountReg (unsigned char*data
					,unsigned int len
                                	,unsigned int off_add)
{       
#ifdef TPTM_DEBUG_U2A
	printFuncName(this->name(), "readCountReg");
#endif //TPTM_DEBUG_U2A
	setCountValue();

	//read count value for TptmFunc
	unsigned int off_add_bit2 = (off_add & 0x3);
        for (unsigned int i = 0; i < len; i++)
        {
		data[i] = mRegTPTMmUCNTx.byte1[off_add_bit2++];
        }


#ifdef TPTM_DEBUG_U2A
	printf("\tmRegTPTMmUCNTx.byte4=0x%x\n\n", mRegTPTMmUCNTx.byte4); fflush(stdout); 
#endif //TPTM_DEBUG
}

/// <summary>Read comparative value register.This func is called by TptmFunc</summary>
/// <param name="data">pointer of transaction data</param>
/// <param name="len">transaction data length</param>
/// <param name="off_add">offset address</param>
void UP_COUNTER_TIMER::readCmpReg (unsigned char*data
                                        ,unsigned int len
                                        ,unsigned int off_add)
{
	unsigned int index = off_add - 0x4;
	//read count value for TptmFunc
        for (unsigned int i = 0; i < len; i++)
        {
		data[i] = mRegTPTMmUCMPx.byte1[index++];
        }		
}

/// <summary>Write count register.This func is called by TptmFunc</summary>
/// <param name="data">pointer of transaction data</param>
/// <param name="len">transaction data length</param>
/// <param name="off_add">offset address</param>
void UP_COUNTER_TIMER::writeCountReg (unsigned char*data
                                        ,unsigned int len
					,unsigned int off_add)
{
#ifdef TPTM_DEBUG_U2A
	printFuncName(this->name(), "writeCountReg");
#endif //TPTM_DEBUG_U2A
	unsigned int off_add_bit2 = (off_add & 0x3);
	for(unsigned int i = 0; i < len; i++)
	{
		mRegTPTMmUCNTx.byte1[off_add_bit2++] = data[i];
	}
#ifdef TPTM_DEBUG_U2A
	printf("\tmRegTPTMmUCNTx.byte4=0x%x\n\n", mRegTPTMmUCNTx.byte4); fflush(stdout); 
#endif //TPTM_DEBUG

}

/// <summary>Write comparative value register.This func is called by TptmFunc</summary>
/// <param name="data">pointer of transaction data</param>
/// <param name="len">transaction data length</param>
/// <param name="off_add">offset address</param>
void UP_COUNTER_TIMER::writeCmpReg  (unsigned char*data
                                        ,unsigned int len
                                        ,unsigned int off_add)
{
	setCountValue();
	unsigned int reg_index = off_add - 0x4;
	unsigned int cmp_index = (0x3 & (reg_index >> 0x2));

	//read count value for TptmFunc
        for (unsigned int i = 0; i < len; i++)
        {
		mRegTPTMmUCMPx.byte1[reg_index++] = data[i];
        }

	//if timer is running, re-schedule cmp interrupt event 
	if (((*mpRegTPTMSUSTR >> mChNum) & 0x1) == 0x1)
	{
		mTptmOutputSig[cmp_index]->cancelProcessOutputSig();
		if (mRegTPTMmUCNTx.byte4 <= mRegTPTMmUCMPx.byte4[cmp_index])
		{
			sc_dt::uint64 cmp_mtch_intrrpt_tm 
				= ((mCycle 
				* (sc_dt::uint64)((*mpRegTPTMSUDIV + 1) 
				* ((mRegTPTMmUCMPx.byte4[cmp_index] + 1) 
				- mRegTPTMmUCNTx.byte4))) 
				- mInterDivCountTime) ;
			mTptmOutputSig[cmp_index]->pulseOutput((double)cmp_mtch_intrrpt_tm);
		}
	}
}
/// <summary>get counter value register.This func is called by TptmFunc</summary>
/// <returns>counter value register</returns>
unsigned int UP_COUNTER_TIMER::getCountReg  (void)
{
	return mRegTPTMmUCNTx.byte4;
}

/// <summary>get comparative value register.This func is called by TptmFunc</summary>
/// <param name="off_add">offset address</param>
/// <returns>comparative value register</returns>
unsigned int UP_COUNTER_TIMER::getCmpReg  (unsigned int off_add)
{
	unsigned int cmp_index = (0x3 & ((off_add - 0x4) >> 0x2));

	return mRegTPTMmUCMPx.byte4[cmp_index];
}

/// <summary>start count down</summary>
void UP_COUNTER_TIMER::startCountUp (void)
{	
	meOverflow.cancel();


	meOverflow.notify((double)((mCycle *
                                        ((sc_dt::uint64)*mpRegTPTMSUDIV + 1))
                                        * ((sc_dt::uint64)0xffffffff + 1))
                                        , glb_resolution_unit);
	setCountValue();
	for (unsigned char cmp_id = 0; 
		cmp_id < G4TPTM_UP_COUNTER_TIMER_CMP_NUM; cmp_id++)
	{
		if (!( mRegTPTMmUCNTx.byte4 - 1 == mRegTPTMmUCMPx.byte4[cmp_id] 
			&& mInterDivCountTime == 0))
		{
			mTptmOutputSig[cmp_id]->cancelProcessOutputSig();
		}	
		mTptmOutputSig[cmp_id]->pulseOutput( 
				(double)((mCycle * 
				((sc_dt::uint64)*mpRegTPTMSUDIV + 1)) 
			 	* ((sc_dt::uint64)mRegTPTMmUCMPx.byte4[cmp_id] 
				+ 1)));
	}
	mRegTPTMmUCNTx.byte4 = 0;
	mCountRegUpdateTime = sc_time_stamp().value();
	mInterDivCountTime = 0;
	
	//update status register
	*mpRegTPTMSUSTR |= (0x1 << mChNum);


#ifdef TPTM_UT_TEST
	printFuncName(this->name(), "startCountUp");
#endif //TPTM_UT_TEST
#ifdef TPTM_DEBUG
	for (unsigned char cmp_id = 0; cmp_id < G4TPTM_UP_COUNTER_TIMER_CMP_NUM; cmp_id++)
	{
		printf("TPTMnUMP ch=%d cmp=%d val=%x\n", mChNum, cmp_id, mRegTPTMmUCMPx.byte4[cmp_id]);
		sc_dt::uint64 cmp_mtch_intrrpt_tm = ((mCycle * 
				((sc_dt::uint64)*mpRegTPTMSUDIV + 1)) 
			 	* ((sc_dt::uint64)mRegTPTMmUCMPx.byte4[cmp_id] 
				+ 1));
	}

	sc_dt::uint64 flow_time = ((mCycle * 
                                        ((sc_dt::uint64)*mpRegTPTMSUDIV + 1))
                                        * ((sc_dt::uint64)(mRegTPTMmUCNTx.byte4) + 1));
	double double_flow_time = (double)flow_time;
	cout << "\tflow_time=" << flow_time << endl;
	printf("\tdouble_flow_time=%e\n", double_flow_time);
	printf("\tmCountRegUpdateTime=%d\n", mCountRegUpdateTime);
	fflush(stdout);
#endif //TPTM_DEBUG

}


/// <summary>Restart count down</summary>
void UP_COUNTER_TIMER::restartCountUp (void)
{
#ifdef TPTM_UT_TEST
        	printFuncName(this->name(), "restartCountUp");
#endif //TPTM_UT_TEST
	if (((*mpRegTPTMSUSTR >> mChNum) & 0x1) == 0x0)
	{
		meOverflow.notify((double)((
				((sc_dt::uint64)(~mRegTPTMmUCNTx.byte4)) 
				+ 1) * mCycle
				* ((sc_dt::uint64)*mpRegTPTMSUDIV + 1)
        	                - mInterDivCountTime)
        	                , glb_resolution_unit);
		for(unsigned char cmp_id = 0; 
			cmp_id < G4TPTM_UP_COUNTER_TIMER_CMP_NUM; cmp_id++)
		{
			if (mRegTPTMmUCNTx.byte4 
				<= mRegTPTMmUCMPx.byte4[cmp_id])
			{
				
				sc_dt::uint64 cmp_mtch_intrrpt_tm 
					= ((mCycle 
					* (sc_dt::uint64)((*mpRegTPTMSUDIV + 1) 
					* ((mRegTPTMmUCMPx.byte4[cmp_id] + 1) 
					- mRegTPTMmUCNTx.byte4))) 
					- mInterDivCountTime) ;
				mTptmOutputSig[cmp_id]->pulseOutput((double)cmp_mtch_intrrpt_tm);
			}	
		}
		
		mCountRegUpdateTime = sc_time_stamp().value();
		//update status register
		*mpRegTPTMSUSTR |= (0x1 << mChNum);

	}
}

/// <summary>Stop count down</summary>
void UP_COUNTER_TIMER::stopCountUp (void)
{
#ifdef TPTM_UT_TEST
	printFuncName(this->name(), "stopCountUp");
#endif //TPTM_UT_TEST
	setCountValue();
	meOverflow.cancel();

	for(unsigned char cmp_id = 0; 
		cmp_id < G4TPTM_UP_COUNTER_TIMER_CMP_NUM; cmp_id++)
	{
		if (!( mRegTPTMmUCNTx.byte4 - 1 == mRegTPTMmUCMPx.byte4[cmp_id] 
			&& mInterDivCountTime == 0))
		{
			mTptmOutputSig[cmp_id]->cancelProcessOutputSig();
		}	
	}
	
#ifdef TPTM_DEBUG
	printf("\tbefore status reg=0x%x\n\tchage val=0x%x\n", *mpRegTPTMSUSTR, (~(0x1 << mChNum)));
#endif //TPTM_DEBUG

	//update status register
	*mpRegTPTMSUSTR &= ~(0x1 << mChNum);

#ifdef TPTM_DEBUG
        printf("\tafter status reg=0x%x\n", *mpRegTPTMSUSTR);
	fflush(stdout);
#endif //TPTM_DEBUG
}

/// <summary>Process to invoke Overflow </summary>
void UP_COUNTER_TIMER::processOverflow (void)
{
#ifdef TPTM_UT_TEST
	printFuncName(this->name(), "processOverflow");
#endif //TPTM_UT_TEST

	//If processOverflow invoke after stopCountUp,
	//startCountUp don't invoke
	if (((*mpRegTPTMSUSTR >> mChNum) & 0x1) == 0x1)
	{
		//start count down
		startCountUp();
	}

}


/// <summary>reset</summary>
void UP_COUNTER_TIMER::reset (void)
{
#ifdef TPTM_UT_TEST
	printFuncName(this->name(), "reset");
#endif //TPTM_UT_TEST
	//initialize channel register
	//initialize value of count register TPTMnUCNTx is uncertain
	for (unsigned int cmp = 0; cmp < G4TPTM_UP_COUNTER_TIMER_CMP_NUM; 
		cmp++)
	{
		mRegTPTMmUCMPx.byte4[cmp] = 0x0;
	}

	//evnet cancel
        meStartCountUp.cancel();
        meRestartCountUp.cancel();
        meStopCountUp.cancel();
        meOverflow.cancel();

	//clear time for timer
	mCountRegUpdateTime = 0;
	mInterDivCountTime = 0;
}


/// <summary>After one cycle, notify event for start count down</summary>
void UP_COUNTER_TIMER::notifyStartCountUp (void)
{
	meStartCountUp.notify((double)(mCycle), glb_resolution_unit);
}

/// <summary>After one cycle, notify event for restart count down</summary>
void UP_COUNTER_TIMER::notifyRestartCountUp (void)
{
	meRestartCountUp.notify((double)(mCycle), glb_resolution_unit);
}

/// <summary>After one cycle, notify event for stop count down</summary>
void UP_COUNTER_TIMER::notifyStopCountUp (void)
{
	meStopCountUp.notify((double)(mCycle), glb_resolution_unit);
}


#if 0
//This function is for checking output signal and using VCD.
//Currently this function is not used.
//This funcfion is remained because it is possible to use this function in the future.

// VCD output for debug
/// <summary>enable VCD-output</summary>
/// <param name="tf">pointer to sc_trace_file</param>
/// <returns>none</returns>
void UP_COUNTER_TIMER::setTraceEnable( sc_trace_file* tf )
{
	sc_trace( tf, TPTM_IRQ, TPTM_IRQ.name() );
}
#endif

#ifdef TPTM_UT_TEST
/// <summary>Debugging function</summary>
void UP_COUNTER_TIMER::printFuncName (const char *this_name
					, const char *func_name)
{
        printf("%s:%s::%s\n"
                ,sc_time_stamp().to_string().c_str()
                ,this_name
                ,func_name);
        fflush(stdout);
}
#endif //TPTM_UT_TEST

