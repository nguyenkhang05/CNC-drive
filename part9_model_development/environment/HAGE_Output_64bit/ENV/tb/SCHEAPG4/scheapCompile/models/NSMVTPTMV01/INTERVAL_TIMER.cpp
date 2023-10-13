/*************************************************************************
 *
 * INTERVAL_TIMER.cpp
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
#include "INTERVAL_TIMER.h"
#include "TptmFunc.h"
#include "TPTM.h"

/// <summary>constructor</summary>
/// <param name="name">module name</param>
INTERVAL_TIMER::INTERVAL_TIMER(sc_module_name name):
	G4_CommonPrototype(name, "cpu_clk")
	,sys_resetn("sys_resetn")
        ,TPTM_IRQ("TPTM_IRQ")
        ,mChNum(0)
	,mOutputSigVal(false)
	,mpRegTPTMSISTR(0)
        ,mpRegTPTMSIIEN(0)
        ,mpRegTPTMSIUSTR(0)
        ,mpRegTPTMSIDIV(0)
        ,mCountRegUpdateTime(0)
	,mInterDivCountTime(0)
        ,meStartCountDown()
        ,meRestartCountDown()
        ,meStopCountDown()
        ,meUnderflow()
        ,meOutputSig()
{
	mRegTPTMnICNTx.byte4 = 0x0;
        mRegTPTMnILDx.byte4 = 0xffffffff;

	SC_METHOD(startCountDown);
		sensitive << meStartCountDown;
		dont_initialize();

	SC_METHOD(restartCountDown);
		sensitive << meRestartCountDown;
		dont_initialize();

	SC_METHOD(stopCountDown);
		sensitive << meStopCountDown;
		dont_initialize();

	SC_METHOD(processUnderflow);
		sensitive << meUnderflow;
		dont_initialize();
	
	SC_METHOD(outputSig);
		sensitive << meOutputSig;
		dont_initialize();

        SC_METHOD(reset);
                sensitive << sys_resetn.neg();
                dont_initialize();

}

/// <summary>destructor</summary>
INTERVAL_TIMER::~INTERVAL_TIMER(void) 
{
}




/// <summary>Set channel index. This Func is called by TPTM constructor</summary>
/// <param name="ch_num">chnannel index</param>
void INTERVAL_TIMER::setMemFromTPTM(unsigned char ch_num)
{
	mChNum = ch_num;
}

/// <summary>Set pointers of 4 registers. This Func is called by TptmFunc constructor</summary>
/// <param name="reg_str">status register</param>
/// <param name="reg_en">interrupt enable register</param>
/// <param name="reg_ustr">underflow flag register</param>
/// <param name="reg_div">clock division register</param>
void INTERVAL_TIMER::setMemFromTptmFunc (unsigned char* reg_str
                                	,unsigned char* reg_en
                                	,unsigned char* reg_ustr
                                	,unsigned char* reg_div )
{
	mpRegTPTMSISTR = reg_str;
	mpRegTPTMSIIEN = reg_en;
	mpRegTPTMSIUSTR = reg_ustr;	
	mpRegTPTMSIDIV = reg_div;
}

/// <summary>Set current count value to count register</summary>
void INTERVAL_TIMER::setCountValue (void)
{
	
	//if timer is running, set value count register 
	if (((*mpRegTPTMSISTR >> mChNum) & 0x1) == 0x1)
	{
		sc_dt::uint64 now_time = sc_time_stamp().value();
		sc_dt::uint64 timer_period = ((sc_dt::uint64)*mpRegTPTMSIDIV 
					+ 1) * mCycle;
		sc_dt::uint64 past_time = now_time - mCountRegUpdateTime 
					+ mInterDivCountTime;

		//The max value of R.H.S is 32bit.
		unsigned int cnt = past_time / timer_period;
		sc_dt::uint64 int_div_cnt_time = past_time % timer_period;

		if (0 < cnt || 0 <int_div_cnt_time)
		{
			//For invoke underflow
                	//before called SC_METHOD(processUnderflow).
			if (mRegTPTMnICNTx.byte4 < cnt)
			{
				mRegTPTMnICNTx.byte4 = mRegTPTMnILDx.byte4;
			}
			else
			{
				mRegTPTMnICNTx.byte4 -= cnt;
			}

			mInterDivCountTime = int_div_cnt_time;
			mCountRegUpdateTime = now_time;
		}

#ifdef TPTM_DEBUG
	printf("\tmRegTPTMnICNTx.byte4=0x%x \n\tmInterDivCountTime=%d\n", mRegTPTMnICNTx.byte4, mInterDivCountTime); fflush(stdout); 
#endif //TPTM_DEBUG
	}
}

/// <summary>Read count register.This func is called by TptmFunc</summary>
/// <param name="data">pointer of transaction data</param>
/// <param name="len">transaction data length</param>
/// <param name="off_add">offset address</param>
void INTERVAL_TIMER::readCountReg (unsigned char*data
					,unsigned int len
                                	,unsigned int off_add)
{       
	setCountValue();

	//read count value for TptmFunc
	unsigned int off_add_bit2 = (off_add & 0x3);
        for (unsigned int i = 0; i < len; i++)
        {
		data[i] = mRegTPTMnICNTx.byte1[off_add_bit2++];
        }


#ifdef TPTM_DEBUG
	printf("\tmRegTPTMnICNTx.byte4=0x%x\n\n", mRegTPTMnICNTx.byte4); fflush(stdout); 
#endif //TPTM_DEBUG
}

/// <summary>Read load register.This func is called by TptmFunc</summary>
/// <param name="data">pointer of transaction data</param>
/// <param name="len">transaction data length</param>
/// <param name="off_add">offset address</param>
void INTERVAL_TIMER::readLoadReg (unsigned char*data
                                        ,unsigned int len
                                        ,unsigned int off_add)
{
	//read count value for TptmFunc
        unsigned int off_add_bit2 = (off_add & 0x3);
        for (unsigned int i = 0; i < len; i++)
        {
                data[i] = mRegTPTMnILDx.byte1[off_add_bit2++]; 
        }		
}

/// <summary>Write count register.This func is called by TptmFunc</summary>
/// <param name="data">pointer of transaction data</param>
/// <param name="len">transaction data length</param>
/// <param name="off_add">offset address</param>
void INTERVAL_TIMER::writeCountReg (unsigned char*data
                                        ,unsigned int len
					,unsigned int off_add)
{
	unsigned int off_add_bit2 = (off_add & 0x3);
	for(unsigned int i = 0; i < len; i++)
	{
		mRegTPTMnICNTx.byte1[off_add_bit2++] = data[i];
	}
}

/// <summary>Write load register.This func is called by TptmFunc</summary>
/// <param name="data">pointer of transaction data</param>
/// <param name="len">transaction data length</param>
/// <param name="off_add">offset address</param>
void INTERVAL_TIMER::writeLoadReg  (unsigned char*data
                                        ,unsigned int len
                                        ,unsigned int off_add)
{
	unsigned int off_add_bit2 = (off_add & 0x3);
        for(unsigned int i = 0; i < len; i++)
        {
		mRegTPTMnILDx.byte1[off_add_bit2++] = data[i]; 
        }	
}

/// <summary>start count down</summary>
void INTERVAL_TIMER::startCountDown (void)
{	
	meUnderflow.cancel();
	mRegTPTMnICNTx.byte4 = mRegTPTMnILDx.byte4;	
	meUnderflow.notify((double)((mCycle * 
					((sc_dt::uint64)*mpRegTPTMSIDIV + 1)) 
			 		* ((sc_dt::uint64)mRegTPTMnICNTx.byte4 
					+ 1))
					, glb_resolution_unit);
	mCountRegUpdateTime = sc_time_stamp().value();
	mInterDivCountTime = 0;
	
	//update status register
	*mpRegTPTMSISTR |= (0x1 << mChNum);


#ifdef TPTM_UT_TEST
	printFuncName(this->name(), "startCountDown");
#endif //TPTM_UT_TEST
#ifdef TPTM_DEBUG
	sc_dt::uint64 flow_time = ((mCycle * 
                                        ((sc_dt::uint64)*mpRegTPTMSIDIV + 1))
                                        * ((sc_dt::uint64)mRegTPTMnICNTx.byte4 + 1));
	double double_flow_time = (double)flow_time;
	cout << "\tflow_time=" << flow_time << endl;
	printf("\tdouble_flow_time=%e\n", double_flow_time);
	printf("\tload reg=0x%x\n", mRegTPTMnILDx.byte4);
	printf("\tmCountRegUpdateTime=%d\n", mCountRegUpdateTime);
	fflush(stdout);
#endif //TPTM_DEBUG
}


/// <summary>Restart count down</summary>
void INTERVAL_TIMER::restartCountDown (void)
{
	if (((*mpRegTPTMSISTR >> mChNum) & 0x1) == 0x0)
	{
		meUnderflow.notify((double)(
					((sc_dt::uint64)mRegTPTMnICNTx.byte4 
					+ 1) * mCycle 
					* ((sc_dt::uint64)*mpRegTPTMSIDIV + 1) 
					- mInterDivCountTime)
					, glb_resolution_unit);
		mCountRegUpdateTime = sc_time_stamp().value();
		//update status register
		*mpRegTPTMSISTR |= (0x1 << mChNum);

#ifdef TPTM_UT_TEST
        	printFuncName(this->name(), "restartCountDown");
#endif //TPTM_UT_TEST
	}
}

/// <summary>Stop count down</summary>
void INTERVAL_TIMER::stopCountDown (void)
{
#ifdef TPTM_UT_TEST
	printFuncName(this->name(), "stopCountDown");
#endif //TPTM_UT_TEST
	setCountValue();
	meUnderflow.cancel();
	
#ifdef TPTM_DEBUG
	printf("\tbefore status reg=0x%x\n\tchage val=0x%x\n", *mpRegTPTMSISTR, (~(0x1 << mChNum)));
#endif //TPTM_DEBUG

	//update status register
	*mpRegTPTMSISTR &= ~(0x1 << mChNum);

#ifdef TPTM_DEBUG
        printf("\tafter status reg=0x%x\n", *mpRegTPTMSISTR);
	fflush(stdout);
#endif //TPTM_DEBUG
}

/// <summary>Process to invoke Underflow </summary>
void INTERVAL_TIMER::processUnderflow (void)
{
#ifdef TPTM_UT_TEST
	printFuncName(this->name(), "processUnderflow");
#endif //TPTM_UT_TEST

	//write register for setting underflow flag
	*mpRegTPTMSIUSTR |= (0x1 << mChNum); 

	if (((*mpRegTPTMSIIEN >> mChNum) & 0x1) == 0x1)
	{
		notifyOutputSig(true);
	}
	
	//If processUnderflow invoke after stopCountDown,
	//startCountDown don't invoke
	if (((*mpRegTPTMSISTR >> mChNum) & 0x1) == 0x1)
	{
		//start count down
		startCountDown();
	}

}

/// <summary>output signal</summary>
void INTERVAL_TIMER::outputSig (void)
{
	TPTM_IRQ.write(mOutputSigVal);
}

/// <summary>reset</summary>
void INTERVAL_TIMER::reset (void)
{
#ifdef TPTM_UT_TEST
	printFuncName(this->name(), "reset");
#endif //TPTM_UT_TEST
	notifyOutputSig(false);
	//initialize channel register
	//initialize value of count register TPTMnICNTx is uncertain
	mRegTPTMnILDx.byte4 = 0xffffffff;

	//evnet cancel
        meStartCountDown.cancel();
        meRestartCountDown.cancel();
        meStopCountDown.cancel();
        meUnderflow.cancel();

	//clear time for timer
	mCountRegUpdateTime = 0;
	mInterDivCountTime = 0;
}


/// <summary>After one cycle, notify event for start count down</summary>
void INTERVAL_TIMER::notifyStartCountDown (void)
{
	meStartCountDown.notify((double)(mCycle), glb_resolution_unit);
}

/// <summary>After one cycle, notify event for restart count down</summary>
void INTERVAL_TIMER::notifyRestartCountDown (void)
{
	meRestartCountDown.notify((double)(mCycle), glb_resolution_unit);
}

/// <summary>After one cycle, notify event for stop count down</summary>
void INTERVAL_TIMER::notifyStopCountDown (void)
{
	meStopCountDown.notify((double)(mCycle), glb_resolution_unit);
}

/// <summary>notify event for output signal</summary>
/// <param name="sig_val">output signale value</param>
void INTERVAL_TIMER::notifyOutputSig (bool sig_val)
{
#ifdef TPTM_UT_TEST
        	printFuncName(this->name(), "notifyOutputSig");
#endif //TPTM_UT_TEST
	mOutputSigVal = sig_val;
        meOutputSig.notify(SC_ZERO_TIME);
}
#if 0
//This function is for checking output signal and using VCD.
//Currently this function is not used.
//This funcfion is remained because it is possible to use this function in the future.

// VCD output for debug
/// <summary>enable VCD-output</summary>
/// <param name="tf">pointer to sc_trace_file</param>
/// <returns>none</returns>
void INTERVAL_TIMER::setTraceEnable( sc_trace_file* tf )
{
	sc_trace( tf, TPTM_IRQ, TPTM_IRQ.name() );
}
#endif

#ifdef TPTM_UT_TEST
/// <summary>Debugging function</summary>
void INTERVAL_TIMER::printFuncName (const char *this_name
					, const char *func_name)
{
        printf("%s:%s::%s\n"
                ,sc_time_stamp().to_string().c_str()
                ,this_name
                ,func_name);
        fflush(stdout);
}
#endif //TPTM_UT_TEST

