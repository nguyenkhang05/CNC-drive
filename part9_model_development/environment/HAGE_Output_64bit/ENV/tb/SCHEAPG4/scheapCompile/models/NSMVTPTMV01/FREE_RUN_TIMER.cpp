/*************************************************************************
 *
 * FREE_RUN_TIMER.cpp
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
#include "FREE_RUN_TIMER.h"
#include "TptmFunc.h"
#include "TPTM.h"

/// <summary>constructor</summary>
/// <param name="name">module name</param>
FREE_RUN_TIMER::FREE_RUN_TIMER(sc_module_name name):
	G4_CommonPrototype(name, "cpu_clk")
	,sys_resetn("sys_resetn")
        ,mpRegTPTMSFDIV(0)
        ,mpRegTPTMSFSTR(0)
        ,mCountRegUpdateTime(0)
	,mInterDivCountTime(0)
        ,meStartCountUp()
        ,meRestartCountUp()
        ,meStopCountUp()
        ,meOverflow()
{
	mRegTPTMnFCNTx.byte4 = 0x0;
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
FREE_RUN_TIMER::~FREE_RUN_TIMER(void) 
{
}

/// <summary>Set pointers of 4 registers. This Func is called by TptmFunc constructor</summary>
/// <param name="reg_str">status register</param>
/// <param name="">clock division register</param>
void FREE_RUN_TIMER::setMemFromTptmFunc(unsigned char* reg_str
                                	, unsigned char* reg_div) 
{
	mpRegTPTMSFSTR = reg_str;
	mpRegTPTMSFDIV = reg_div;
}

/// <summary>Set current count value to count register</summary>
void FREE_RUN_TIMER::setCountValue (void)
{

        //if timer is running, set value count register 
        if (*mpRegTPTMSFSTR == 0x1)
        {
                sc_dt::uint64 now_time = sc_time_stamp().value();
                sc_dt::uint64 timer_period = ((sc_dt::uint64)*mpRegTPTMSFDIV
                                        + 1) * mCycle;
                sc_dt::uint64 past_time = now_time - mCountRegUpdateTime
                                        + mInterDivCountTime;

		//The max value of R.H.S is 32bit.
		unsigned int cnt = past_time / timer_period;
                sc_dt::uint64 int_div_cnt_time = past_time % timer_period;

                if (0 < cnt || 0 <int_div_cnt_time)
                {
                	mRegTPTMnFCNTx.byte4 += cnt;

                	mInterDivCountTime = int_div_cnt_time;
                	mCountRegUpdateTime = now_time;
		}

#ifdef TPTM_DEBUG
        printf("\tmRegTPTMnFCNTx.byte4=0x%x \n\tmInterDivCountTime=%d\n", mRegTPTMnFCNTx.byte4, mInterDivCountTime); fflush(stdout);
#endif //TPTM_DEBUG
        }
}

/// <summary>Read count register.This func is called by TptmFunc</summary>
/// <param name="data">pointer of transaction data</param>
/// <param name="len">transaction data length</param>
/// <param name="off_add">offset address</param>
void FREE_RUN_TIMER::readCountReg (unsigned char*data
					,unsigned int len
					,unsigned int off_add)
{       
	setCountValue();

        //read count value for TptmFunc
        unsigned int off_add_bit2 = (off_add & 0x3);
        for (unsigned int i = 0; i < len; i++)
        {
                data[i] = mRegTPTMnFCNTx.byte1[off_add_bit2++];
        }

#ifdef TPTM_DEBUG
      	printf("\tmRegTPTMnFCNTx.byte4=0x%x\n", mRegTPTMnFCNTx.byte4); 
	fflush(stdout); 
#endif //TPTM_DEBUG
}

/// <summary>Write count register.This func is called by TptmFunc</summary>
/// <param name="data">pointer of transaction data</param>
/// <param name="len">transaction data length</param>
/// <param name="off_add">offset address</param>
void FREE_RUN_TIMER::writeCountReg (unsigned char*data
                                        ,unsigned int len
                                        ,unsigned int off_add)
{
        unsigned int off_add_bit2 = (off_add & 0x3);
        for(unsigned int i = 0; i < len; i++)
        {
                mRegTPTMnFCNTx.byte1[off_add_bit2++] = data[i];
        }
}



/// <summary>start count up</summary>
void FREE_RUN_TIMER::startCountUp (void)
{	
	meOverflow.cancel();
	mRegTPTMnFCNTx.byte4 = 0;
	meOverflow.notify((double)((mCycle *
                                        ((sc_dt::uint64)*mpRegTPTMSFDIV + 1))
                                        * ((sc_dt::uint64)0xffffffff + 1))
                                        , glb_resolution_unit);
	mCountRegUpdateTime = sc_time_stamp().value();
	mInterDivCountTime = 0;

	//update status register
        *mpRegTPTMSFSTR |= 0x1 ;

#ifdef TPTM_UT_TEST
	printFuncName(this->name(), "startCountUp");
#endif //TPTM_UT_TEST
#ifdef TPTM_DEBUG
	sc_dt::uint64 flow_time = ((mCycle *
                                        ((sc_dt::uint64)*mpRegTPTMSFDIV + 1))
                                        * ((sc_dt::uint64)0xffffffff + 1));
	double double_flow_time = (double)flow_time;
	cout << "\tflow_time=" << flow_time << endl;
        printf("\tdouble_flow_time=%e\n", double_flow_time);
        fflush(stdout);
#endif //TPTM_DEBUG
}

/// <summary>Restart count up</summary>
void FREE_RUN_TIMER::restartCountUp (void)
{
	if((*mpRegTPTMSFSTR & 0x1) == 0x0)
	{
		meOverflow.notify((double)((((sc_dt::uint64)(~mRegTPTMnFCNTx.byte4)) 
					+ 1) * mCycle
					* ((sc_dt::uint64)*mpRegTPTMSFDIV + 1)
        	                        - mInterDivCountTime)
        	                        , glb_resolution_unit);
		mCountRegUpdateTime = sc_time_stamp().value();

		//update status register
        	*mpRegTPTMSFSTR |= 0x1;

#ifdef TPTM_UT_TEST
        	printFuncName(this->name(), "restartCountUp");
        	fflush(stdout);
#endif //TPTM_UT_TEST
	}
}

/// <summary>Stop count up</summary>
void FREE_RUN_TIMER::stopCountUp (void)
{
	setCountValue();
	meOverflow.cancel();

        //update status register
        *mpRegTPTMSFSTR &= 0xfe;

#ifdef TPTM_UT_TEST
        printFuncName(this->name(), "stopCountUp");
#endif //TPTM_UT_TEST
}

/// <summary>Process to invoke Overflow </summary>
void FREE_RUN_TIMER::processOverflow (void)
{
#ifdef TPTM_UT_TEST
	printFuncName(this->name(), "processOverflow");
#endif //TPTM_UT_TEST

	if ((*mpRegTPTMSFSTR & 0x1) == 0x1)
	{
		//start count up
		startCountUp();
	}
}

/// <summary>reset</summary>
void FREE_RUN_TIMER::reset (void)
{
#ifdef TPTM_UT_TEST
        printFuncName(this->name(), "reset");
#endif //TPTM_UT_TEST
	//initialize channel register
	//initialize value of count register TPTMnFCNTx is uncertain


	//evnet cancel
        meStartCountUp.cancel();
        meRestartCountUp.cancel();
        meStopCountUp.cancel();
	meOverflow.cancel();

	//clear time for timer
	mCountRegUpdateTime = 0;
	mInterDivCountTime = 0;
}

/// <summary>After one cycle, notify event for start count up</summary>
void FREE_RUN_TIMER::notifyStartCountUp (void)
{
	meStartCountUp.notify((double)(mCycle), glb_resolution_unit);
}

/// <summary>After one cycle, notify event for restart count up</summary>
void FREE_RUN_TIMER::notifyRestartCountUp (void)
{
	meRestartCountUp.notify((double)(mCycle), glb_resolution_unit);
}

/// <summary>After one cycle, notify event for stop count up</summary>
void FREE_RUN_TIMER::notifyStopCountUp (void)
{
	meStopCountUp.notify((double)(mCycle), glb_resolution_unit);
}

#ifdef TPTM_UT_TEST
/// <summary>Debugging function</summary>
void FREE_RUN_TIMER::printFuncName (const char *this_name
                                        , const char *func_name)
{
        printf("%s:%s::%s\n"
                ,sc_time_stamp().to_string().c_str()
                ,this_name
                ,func_name);
        fflush(stdout);
}
#endif //TPTM_UT_TEST

