/*************************************************************************
 *
 * TptmFunc.cpp
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
 
#include "TptmFunc.h"
#include "TPTM.h"
#include "NSMVTPTMV01.h"
#include "CmErrMsg.h"
#include "INTERVAL_TIMER.h"
#include "FREE_RUN_TIMER.h"
#include "UP_COUNTER_TIMER.h"

using namespace CmErrMsg;

#define G4TPTM_MAX_DATA_LEN	4

#define TPTM_RUN_OFF_ADD	0x000
#define TPTM_RRUN_OFF_ADD	0x004
#define TPTM_STP_OFF_ADD	0x008
#define TPTM_STR_OFF_ADD	0x00C
#define TPTM_EN_OFF_ADD		0x010	//only free run timer is div reg 
#define TPTM_USTR_OFF_ADD	0x014	//only interval timer exist
#define TPTM_DIV_OFF_ADD	0x018	//only free run timer don't exist
#define TPTM_TRG_OFF_ADD	0x01C	//only up counter exist 

#define TPTM_FCNT_OFF_ADD	0x000
#define TPTM_UCNT_OFF_ADD	0x000
#define TPTM_UCMP_START_OFF_ADD	0x004
#define TPTM_UCMP_END_OFF_ADD	0x013


#define TPTM_INT_TMR_GL_ADD	0x00	
#define TPTM_FRRN_TMR_GL_ADD	0x20	
#define TPTM_UPCNT_TMR_GL_ADD	0x40	
#define TPTM_INT_TMR_CH_ADD	0x80	
#define TPTM_FRRN_TMR_CH_ADD	0xa0	
#define TPTM_UPCNT_TMR_CH_ADD(x)	(0xc0 + 0x20 * x)


#define TPTM_END_GLOBAL_REG_OFF_ADD 	0x5f
#define TPTM_START_CH_REG_OFF_ADD	0x80
#define TPTM_END_CH_REG_OFF_ADD		0xff

#define TPTM_FREE_RUN_TIMER_NUM	4

/// <summary>constructor</summary>
/// <param name="wrapper">pointer of wrapper</param>
TptmFunc::TptmFunc(TPTM* wrapper) :
BusSlaveFuncIf(),
mpWrapper(wrapper),
mThresholdForWrnCmpReg(THRESHOLD_FOR_WRN_CMP_REG_INIT)
{
	for (unsigned int i = 0; i < TPTM_REG_SIZE; i++)
	{
		mReg[i] = 0;
	}
	
	for (unsigned char ch = 0; ch < G4TPTM_INTERVAL_TIMER_CH_NUM_MAX; ch++)
	{
		mpWrapper->mpIntervalTimer[ch]->
			setMemFromTptmFunc(&mReg[TPTMSISTR_OFF_ADD] 
					,&mReg[TPTMSIIEN_OFF_ADD] 
					,&mReg[TPTMSIUSTR_OFF_ADD]
					,&mReg[TPTMSIDIV_OFF_ADD]);
	}
	mpWrapper->mpFreeRunTimer->
		setMemFromTptmFunc(&mReg[TPTMSFSTR_OFF_ADD] 
				,&mReg[TPTMSFDIV_OFF_ADD]);
	for (unsigned char ch = 0; ch < G4TPTM_UP_COUNTER_TIMER_CH_NUM; ch++)
	{
		mpWrapper->mpUpCounterTimer[ch]->
			setMemFromTptmFunc(&mReg[TPTMSUSTR_OFF_ADD] 
					,&mReg[TPTMSUDIV_OFF_ADD]);
	}
}

/// <summary>destructor</summary>
TptmFunc::~TptmFunc(void)
{
}

///<summary>get pointer of mReg</summary>
/// <param name="off_add">offset address</param>
/// <returns>pointer of mReg[off_add]</returns>
unsigned char* TptmFunc::getRegPntr( unsigned int off_add)
{
	return(&mReg[off_add]);	
}

///<summary>set the number of interval timer channel in mpFunc(call API of mpFunc)</summary>
/// <param name="ch">the number of interval timer channel</param>
void TptmFunc::setIntrvlTmrChNum( unsigned char ch)
{
	mIntrvlTmrChNum = ch;
}

///<summary>set Threshold for Waring Message of TPTMmCMPni</summary>
///<param name="threshold">threshold</param>
void TptmFunc::setThresholdForWrnCmpReg(unsigned int threshold)
{
	mThresholdForWrnCmpReg = threshold;
}

///<summary>TLM read function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
void TptmFunc::read( unsigned int     offsetAddress, 
		     TlmBasicPayload& trans, 
		     BusTime_t*       t,
		     bool             debug )
{
#ifdef TPTM_UT_TEST
        mpWrapper->printFuncName(mpWrapper->name(), "read");
#endif //TPTM_UT_TEST


	unsigned int len = trans.get_data_length();
	judgeMissAlign(offsetAddress, len);
	unsigned int reg_off_add = (offsetAddress & 0x1f);
	unsigned int timer_off_add = (offsetAddress & 0xe0);


	if (TPTM_START_CH_REG_OFF_ADD <= offsetAddress
		&& offsetAddress <= TPTM_END_CH_REG_OFF_ADD)
	{
		//interval timer
		if(timer_off_add == TPTM_INT_TMR_CH_ADD)
		{
			unsigned char ch = ((offsetAddress >> 3) & 0x3);
			//check interval timer channel
			if (ch < mIntrvlTmrChNum)
			{
				//count register
				if ((reg_off_add & 0x04) == 0x0)
				{
					mpWrapper->mpIntervalTimer[ch]
						->readCountReg(
							trans.get_data_ptr()
							,len ,offsetAddress);	
				}
				//load register
				else
				{
					mpWrapper->mpIntervalTimer[ch]
						->readLoadReg(
							trans.get_data_ptr()
							,len ,offsetAddress);
				}
			}
		}
		//free run timer
		else if (timer_off_add == TPTM_FRRN_TMR_CH_ADD)
		{
			//count register
			if (TPTM_FCNT_OFF_ADD <= reg_off_add
			&& reg_off_add < TPTM_FCNT_OFF_ADD + 4)
			{
				mpWrapper->mpFreeRunTimer
					->readCountReg(trans.get_data_ptr()
						,len ,offsetAddress);	
			}
			//reserved area
			else
			{
				memcpy(trans.get_data_ptr()
					, &mReg[offsetAddress], len);
			}
			
		}
		//up counter timer and reserved area
		else
		{
			bool is_reserved = true;
			for (unsigned char ch = 0; 
				ch < G4TPTM_UP_COUNTER_TIMER_CH_NUM; ch++)
			{
				if (timer_off_add == TPTM_UPCNT_TMR_CH_ADD(ch))
				{	
					//count register
					if (TPTM_UCNT_OFF_ADD <= reg_off_add
					&& reg_off_add < TPTM_UCNT_OFF_ADD + 4)
					{
                        		         mpWrapper->mpUpCounterTimer[ch]
                        		                ->readCountReg(
							trans.get_data_ptr()
                        		                ,len ,offsetAddress);
						is_reserved = false;
					}
					//load register
					else if (TPTM_UCMP_START_OFF_ADD							<= reg_off_add 
					&& reg_off_add 
						<= TPTM_UCMP_END_OFF_ADD)
					{
						mpWrapper->mpUpCounterTimer[ch]
                        			->readCmpReg(
						 trans.get_data_ptr()
                        			,len ,reg_off_add);	
						is_reserved = false;
					}
				}
			}
			if(is_reserved)
			{
				memcpy(trans.get_data_ptr()
					, &mReg[offsetAddress], len);
			}
			
		}
	}
	else
	{
		memcpy(trans.get_data_ptr(), &mReg[offsetAddress], len);
	}


#ifdef TPTM_UT_TEST
	outputLog (offsetAddress, &trans, false);
#endif //TPTM_UT_TEST

}

///<summary>TLM write function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
void TptmFunc::write( unsigned int     offsetAddress, 
                   TlmBasicPayload& trans, 
                   BusTime_t*       t,
                   bool             debug )
{
#ifdef TPTM_UT_TEST
        mpWrapper->printFuncName(mpWrapper->name(), "write");
#endif //TPTM_UT_TEST
	unsigned int len = trans.get_data_length();

	unsigned char *data = trans.get_data_ptr();


	judgeMissAlign(offsetAddress, len);

	unsigned int reg_off_add = (offsetAddress & 0x1f);
	unsigned int timer_off_add = (offsetAddress & 0xe0);

	if (offsetAddress <= TPTM_END_GLOBAL_REG_OFF_ADD)
	{
		//start register
		if (reg_off_add == TPTM_RUN_OFF_ADD)	
		{
			//interval timer
			if (timer_off_add == TPTM_INT_TMR_GL_ADD)
			{
				///start count down?
				for (unsigned char ch = 0;
                		ch < mIntrvlTmrChNum;
                		ch++)
        			{
        			        if (((data[0] >> ch) & 0x1) == 0x1)
        			        {
						mpWrapper->mpIntervalTimer[ch]
                		                ->notifyStartCountDown();
					}
				}

			}
			//free run timer
			else if (timer_off_add == TPTM_FRRN_TMR_GL_ADD)
			{
        			if ((data[0] & 0x1) == 0x1)
        			{
					mpWrapper->mpFreeRunTimer
                		        ->notifyStartCountUp();
				}
			}
			//up counter timer
			else if (timer_off_add == TPTM_UPCNT_TMR_GL_ADD)
			{
				for (unsigned char ch = 0;
                		ch <  G4TPTM_UP_COUNTER_TIMER_CH_NUM;
                		ch++)
        			{
        			        if (((data[0] >> ch) & 0x1) == 0x1
					&& (mReg[TPTMSUTRG_OFF_ADD + ch] 
						& 0x80) == 0x0)
        			        {
						mpWrapper->mpUpCounterTimer[ch]
						->notifyStartCountUp();
					}
				}
			}
		}
		//restart register
		else if (reg_off_add == TPTM_RRUN_OFF_ADD)
		{
			//interval timer
			if (timer_off_add == TPTM_INT_TMR_GL_ADD)
			{
				//		//start count down?
				for (unsigned char ch = 0;
                		ch < mIntrvlTmrChNum;
                		ch++)
        			{
        			        if (((data[0] >> ch) & 0x1) == 0x1)
        			        {
						mpWrapper->mpIntervalTimer[ch]
                		                ->notifyRestartCountDown();
					}
				}

			}
			//free run timer
			else if (timer_off_add == TPTM_FRRN_TMR_GL_ADD)
			{
        			if ((data[0] & 0x1) == 0x1)
        			{
					mpWrapper->mpFreeRunTimer
                		        ->notifyRestartCountUp();
				}
			}
			//up counter timer
			else if (timer_off_add == TPTM_UPCNT_TMR_GL_ADD)
			{
				for (unsigned char ch = 0;
                		ch <  G4TPTM_UP_COUNTER_TIMER_CH_NUM;
                		ch++)
        			{
        			        if (((data[0] >> ch) & 0x1) == 0x1
					&& (mReg[TPTMSUTRG_OFF_ADD + ch] 
						& 0x80) == 0x0)
        			        {
						mpWrapper->mpUpCounterTimer[ch]
                		                ->notifyRestartCountUp();
					}
				}
			}
		}
		//stop register
		else if (reg_off_add == TPTM_STP_OFF_ADD)
		{
			//interval timer
			if (timer_off_add == TPTM_INT_TMR_GL_ADD)
			{
				for (unsigned char ch = 0;
                		ch < mIntrvlTmrChNum;
                		ch++)
        			{
        			        if (((data[0] >> ch) & 0x1) == 0x1)
        			        {
						mpWrapper->mpIntervalTimer[ch]
                		                ->notifyStopCountDown();
					}
				}

			}
			//free run timer
			else if (timer_off_add == TPTM_FRRN_TMR_GL_ADD)
			{
        			if ((data[0] & 0x1) == 0x1)
        			{
					mpWrapper->mpFreeRunTimer
                		        ->notifyStopCountUp();
				}
			}
			//up counter timer
			else if (timer_off_add == TPTM_UPCNT_TMR_GL_ADD)
			{
				for (unsigned char ch = 0;
                		ch <  G4TPTM_UP_COUNTER_TIMER_CH_NUM;
                		ch++)
        			{
        			        if (((data[0] >> ch) & 0x1) == 0x1
					&& (mReg[TPTMSUTRG_OFF_ADD + ch] 
						& 0x80) == 0x0)
        			        {
						mpWrapper->mpUpCounterTimer[ch]
                		                ->notifyStopCountUp();
					}
				}
			}
		}
		//status register
		else if (reg_off_add == TPTM_STR_OFF_ADD)
		{
			//interval timer
			if (timer_off_add == TPTM_INT_TMR_GL_ADD)
			{
				//warning
				printWarningMessage(mpWrapper->name(), "write"
					, "TPTMmISTR is read only register.");
			}
			//free run timer
			else if (timer_off_add == TPTM_FRRN_TMR_GL_ADD)
			{
				//warning
				printWarningMessage(mpWrapper->name(), "write"
					, "TPTMmFSTR is read only register.");
			}
			//up counter timer
			else if (timer_off_add == TPTM_UPCNT_TMR_GL_ADD)
			{
				//warning
				printWarningMessage(mpWrapper->name(), "write"
					, "TPTMmUSTR is read only register.");
			}
		}
		//enable register(interval&up counter tiemr)
		//div register(free run timer)
		else if (TPTM_EN_OFF_ADD <= reg_off_add
			&& reg_off_add < TPTM_USTR_OFF_ADD)
		{
			//interval timer
			unsigned int low_order_4bit = (offsetAddress & 0x3);
			if (timer_off_add == TPTM_INT_TMR_GL_ADD)
			{
				if (low_order_4bit == 0)
				{
					mReg[TPTMSIIEN_OFF_ADD] 
						= (data[0] & 
						((1 << mIntrvlTmrChNum) - 1));
					//output signal?
					for (unsigned char ch = 0;
					  ch < mIntrvlTmrChNum;
					  ch++)
					{
						unsigned char reg_ien_and_ustr 
						= (data[0] 
						& mReg[TPTMSIUSTR_OFF_ADD]);
					        if (((reg_ien_and_ustr >> ch) 
							& 0x1) == 0x1)
					        {
							mpWrapper
							->mpIntervalTimer[ch]
							->notifyOutputSig(true);
						}
					}
				}
			}
			//free run timer
			else if (timer_off_add == TPTM_FRRN_TMR_GL_ADD)
			{
				//warning
				if (mReg[TPTMSFSTR_OFF_ADD] > 0)
                		{
                		        printWarningMessage(
					mpWrapper->name(), "write",
"Stop free run timer at writing TPTMmFDIV due to non-guaranteed operation.");
                		}	
				judgeBitOpForDivReg(trans, 
					"write", "TPTMmFDIV");
				if (low_order_4bit == 0)
				{
					mReg[TPTMSFDIV_OFF_ADD] = data[0];
				}
			}
			//up counter timer
			else if (timer_off_add == TPTM_UPCNT_TMR_GL_ADD)
			{
				unsigned char ch = (reg_off_add & 0x3);
				for (unsigned char i = 0; i < len; i++)
				{	
					if (ch < G4TPTM_UP_COUNTER_TIMER_CH_NUM)
					{
					
						mpWrapper->mpUpCounterTimer[ch]
						->setAssertEnable(data[i]);
						mReg[offsetAddress + i] 
							= (0x0f & data[i]);
					}
					ch++;
				}
				
			}
		}
		//underflow status register(interval timer only)
		else if (reg_off_add == TPTM_USTR_OFF_ADD)
		{
			//interval timer
			if (timer_off_add == TPTM_INT_TMR_GL_ADD)
			{
				//negert output signal
				for (unsigned char ch = 0;
                		ch < mIntrvlTmrChNum;
                		ch++)
        			{
        			        if (((data[0] >> ch) & 0x1) == 0x0 
					&& (mReg[TPTMSIUSTR_OFF_ADD] >> ch) 
						== 0x1)
        			        {
						mpWrapper->mpIntervalTimer[ch]
                		                ->notifyOutputSig(false);
					}
				}
				//update this register
				//cannot write value 1 to TPTMSUSTR bit0~3
				mReg[TPTMSIUSTR_OFF_ADD] &= (data[0] & 0x0f);
			}
		}
		//div register(interval&up counter timer)
		else if (reg_off_add == TPTM_DIV_OFF_ADD)
		{
			//interval timer
			if (timer_off_add == TPTM_INT_TMR_GL_ADD)
			{
				if (mReg[TPTMSISTR_OFF_ADD] > 0)
				{
					//warning
                			printWarningMessage(mpWrapper->name()
					, "write", 
"Stop all interval timer at writing TPTMmIDIV due to non-guaranteed operation."
							);
				}		
				judgeBitOpForDivReg(trans, 
					"write", "TPTMmIDIV");
				mReg[TPTMSIDIV_OFF_ADD] = data[0];
			}
			//up counter timer
			else if (timer_off_add == TPTM_UPCNT_TMR_GL_ADD)
			{
				if (mReg[TPTMSUSTR_OFF_ADD] > 0)
				{
					//warning
                			printWarningMessage(mpWrapper->name()
					, "write", 
"Stop all up counter timer at writing TPTMmUDIV due to non-guaranteed operation.");
				}		
				judgeBitOpForDivReg(trans, 
					"write", "TPTMmUDIV");
				mReg[TPTMSUDIV_OFF_ADD] = data[0];
			}
		}
		//trriger select register(only up counter tiemr
		else if (TPTM_TRG_OFF_ADD <= reg_off_add 
			&& reg_off_add < TPTM_TRG_OFF_ADD + 4)
		{
			if (timer_off_add == TPTM_UPCNT_TMR_GL_ADD)
			{
				unsigned char ch = (reg_off_add & 0x3);
				for (unsigned char i = 0; i < len; i++)
				{
					if (ch < G4TPTM_UP_COUNTER_TIMER_CH_NUM)
					{
						mReg[offsetAddress + i] 
							= (data[i] & 0x81);
					}
					ch++;
				}
			}
		}
	}
	//channel register
	else if (TPTM_START_CH_REG_OFF_ADD <= offsetAddress
		&& offsetAddress <= TPTM_END_CH_REG_OFF_ADD)
	{
		//interval timer
		if(timer_off_add == TPTM_INT_TMR_CH_ADD)
		{
			unsigned char ch = ((offsetAddress >> 3) & 0x3);
			//check interval timer channel
			if (ch < mIntrvlTmrChNum)
			{
				//count register
				if ((offsetAddress & 0x04) == 0x0)
				{
                        	        //Warning
                        	        if (((mReg[TPTMSISTR_OFF_ADD] >> ch)                                                    & 0x1) == 0x1)
                        	        {
                        	                char str[1024];
                        	                sprintf(str,
"Stop interval timer ch%d at writing TPTMmICNTx due to non-guaranteed operation."
						, ch);
                                        	printWarningMessage(
                                                	mpWrapper->name()
                                                	, "write",str);
                                	}
                        	         mpWrapper->mpIntervalTimer[ch]
                        	                ->writeCountReg(&data[0]
                        	                                ,len ,offsetAddress);
					
				}
				//load register
				else
				{
                        		//Warning
                        		if (((mReg[TPTMSISTR_OFF_ADD] >> ch)                                                    & 0x1) == 0x1)
                        		{
                        	        	char str[1024];
                        	        	sprintf(str,
"Stop interval timer ch%d at writing TPTMmILDx due to non-guaranteed operation."
                	        	                , ch);

                	        	        printWarningMessage(
                	        	                mpWrapper->name()
                	        	                , "write", str);
                	        	}
					
	        	                mpWrapper->mpIntervalTimer[ch]
        		                        ->writeLoadReg(&data[0] ,len 
							,offsetAddress);
				}
			}
		}
		//free run timer
		else if (timer_off_add == TPTM_FRRN_TMR_CH_ADD)
		{
			//count register
			if (TPTM_FCNT_OFF_ADD <= reg_off_add
				&& reg_off_add < TPTM_FCNT_OFF_ADD + 4)
			{
				//Warning
				if ((mReg[TPTMSFSTR_OFF_ADD] & 0x1)
				        == 0x1)
				{
				        printWarningMessage(
				                mpWrapper->name()
				                , "write",
"Stop free run timer at writing TPTMmFCNTx due to non-guaranteed operation.");
				}
				mpWrapper->mpFreeRunTimer
				        ->writeCountReg(&data[0]
				                        ,len ,offsetAddress);
			}
			
		}
		//up counter timer
		else
		{
			for (unsigned char ch = 0; 
				ch < G4TPTM_UP_COUNTER_TIMER_CH_NUM; ch++)
			{
				if (timer_off_add == TPTM_UPCNT_TMR_CH_ADD(ch))
				{
					//count register
					if (TPTM_UCNT_OFF_ADD <= reg_off_add
					&& reg_off_add < TPTM_UCNT_OFF_ADD + 4)
					{
                        		        //Warning
                        		        if (((mReg[TPTMSUSTR_OFF_ADD] 
							>> ch) & 0x1) == 0x1)
                        		        {
                        		                char str[1024];
                        		                sprintf(str,"Stop up counter timer ch%d at writing TPTMmUCNTx due to non-guaranteed operation."
                        		                        , ch);


                        		                printWarningMessage(
                        		                mpWrapper->name()
                        		                        , "write",str);
                        		        }
                        		         mpWrapper->mpUpCounterTimer[ch]
                        		                ->writeCountReg(&data[0]
                        		                ,len ,offsetAddress);
					}
					//cmp register
					else if (TPTM_UCMP_START_OFF_ADD							<= reg_off_add 
					&& reg_off_add 
						<= TPTM_UCMP_END_OFF_ADD)
					{
						//Warning
                        		        if (((mReg[TPTMSUSTR_OFF_ADD] 
							>> ch) & 0x1) == 0x1)
                        		        {
							judgeOutputWrnCmpReg(ch, offsetAddress);
                        		        }

						mpWrapper->mpUpCounterTimer[ch]
                        			->writeCmpReg(&data[0]
                        			 ,len ,reg_off_add);	
					}
				}
			}
		}
	}
				

#ifdef TPTM_UT_TEST
	outputLog (offsetAddress, &trans, true);
#endif //TPTM_UT_TEST
}



/// <summary>reset</summary>
void TptmFunc::reset (void)
{	
	for(unsigned int i = 0; i < TPTM_REG_SIZE; i++)
	{
		mReg[i] = 0;
	}
}

/// <summary>Judge miss align. If access by miss align, print error and exit</summary>
/// <param name="off_add">offset address</param>
/// <param name="len">data length</param>
void TptmFunc::judgeMissAlign (unsigned int off_add
				, unsigned int len)
{
	if (0x0 < (off_add & (len - 1)))
	{
		printErrorMessage(mpWrapper->name(), "judgeMissAlign",
		"TPTM doesn't support miss align access");
		exit(1);
	}
}

/// <summary>Judge bit-operation for divisor register. </summary>
///<param name="trans">basic payload</param>
///<param name="func_name">function name</param>
///<param name="reg_name">register name</param>
void TptmFunc::judgeBitOpForDivReg(TlmBasicPayload& trans, const char* func_name, const char* reg_name)
{
#ifdef TPTM_UT_TEST
        mpWrapper->printFuncName(mpWrapper->name(), "judgeBitOpForDivReg");
#endif //TPTM_UT_TEST
	TlmG3mExtension *G3mExt;
        trans.get_extension(G3mExt);
	if (G3mExt->getPeId() != DEBUG_MASTER_BMID)
	{
		TlmVciExtension *VciExt;
		trans.get_extension(VciExt);
		if(VciExt != NULL)
		{
			TlmVciExtension::VciRequest_t vci_req = VciExt->getRequestType();
			if (vci_req == TlmVciExtension::CMD_BITOP_SET 
				|| vci_req == TlmVciExtension::CMD_BITOP_AND 
				|| vci_req == TlmVciExtension::CMD_BITOP_OR 
				|| vci_req == TlmVciExtension::CMD_BITOP_XOR)
			{
				char error_msg_buf[1024];
				sprintf(error_msg_buf, "It is prohibited to access %s by bit-manipulation instruction.", reg_name);
				printWarningMessage(mpWrapper->name(), func_name, error_msg_buf);
			}
		}
#ifdef TPTM_UT_TEST
		else
		{
			printf("\tVciExt == NULL\n");
			fflush(stdout);
		}
#endif //TPTM_UT_TEST
	}
}

/// <summary>Judge outputting Warning Message for compare register </summary>
///<param name="ch">the number of up counter timer channel</param>
///<param name="off_add">off_add</param>
void TptmFunc::judgeOutputWrnCmpReg(unsigned char ch, unsigned int off_add)
{
	unsigned int cmp = mpWrapper->mpUpCounterTimer[ch]->getCmpReg(off_add);
	unsigned int cnt = mpWrapper->mpUpCounterTimer[ch]->getCountReg();
	if (((cmp < cnt)?(cnt - cmp):(cmp - cnt)) < mThresholdForWrnCmpReg)
	{
        	char str[1024];
        	sprintf(str,"Stop up counter timer ch%d at writing TPTMmUCMPni due to non-guaranteed operation.", ch);
                printWarningMessage(mpWrapper->name(), "write",str);
	}
}

#ifdef TPTM_UT_TEST
/// <summary>Debugging function</summary>
void TptmFunc::outputLog (unsigned int offAdd, 
			TlmTransactionType* trans, 
			bool w_or_r)
{
	unsigned char *data = trans->get_data_ptr();

	
	
	printf ("\taddress=0x%llx\n", trans->get_address());
	fflush(stdout);
	for (unsigned char i = 0; i <  trans->get_data_length(); i++)
	{
		printf("\tmReg[0x%x]=0x%x\n", offAdd + i, data[i]);	
		fflush(stdout);
	}

}
#endif //TPTM_UT_TEST
