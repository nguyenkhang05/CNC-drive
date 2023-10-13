/*************************************************************************
 *
 * TptmgFunc.cpp
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

#include "TptmgFunc.h"
#include "TPTMG.h"
#include "TPTM.h"
#include "UP_COUNTER_TIMER.h"
#include "NSMVTPTMV01.h"
#include "CmErrMsg.h"

using namespace CmErrMsg;

#define G4TPTM_MAX_DATA_LEN	4
#define TPTMGxURUN_OFF_ADD	0x40
#define TPTMGxURRUN_OFF_ADD	0x44
#define TPTMGxUSTP_OFF_ADD	0x48


/// <summary>constructor</summary>
/// <param name="wrapper">pointer of wrapper</param>
TptmgFunc::TptmgFunc(TPTMG* wrapper) :
	BusSlaveFuncIf()
	,mpWrapper(wrapper)
	,mpTPTM(0)
	,mPeNum(G4TPTM_PE_NUM_INIT)
	,mpTPTMnUTRG(0)
{
	for (unsigned int i = 0; i < TPTMG_REG_SIZE; i++)
	{
		mReg[i] = 0;
	}
}

/// <summary>destructor</summary>
TptmgFunc::~TptmgFunc(void)
{
	for (unsigned char pe = 0; pe < mPeNum; pe++)
	{
		delete mpTPTMnUTRG[pe];
	}
	delete mpTPTMnUTRG;
}

///<summary>TLM read function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
void TptmgFunc::read( unsigned int     offsetAddress, 
		     TlmBasicPayload& trans, 
		     BusTime_t*       t,
		     bool             debug )
{
#ifdef TPTM_UT_TEST
        mpWrapper->printFuncName(mpWrapper->name(), "read");
#endif //TPTM_UT_TEST

	unsigned int len = trans.get_data_length();
	judgeMissAlign(offsetAddress, len);

	memcpy(trans.get_data_ptr(),&mReg[offsetAddress],len);

#ifdef TPTM_UT_TEST
	outputLog (offsetAddress, &trans, false);
#endif //TPTM_UT_TEST

}

///<summary>TLM write function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
void TptmgFunc::write( unsigned int     offsetAddress, 
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

	if (offsetAddress ==  TPTMGxURUN_OFF_ADD && (data[0] & 0x1) == 1)
	{
		for (unsigned char pe = 0; pe < mPeNum; pe++)
		{
			for (unsigned char ch = 0; 
				ch < G4TPTM_UP_COUNTER_TIMER_CH_NUM; ch++)
			{
				if ((*mpTPTMnUTRG[pe][ch] & 0x81) == mValUTRG)
				{
					mpTPTM[pe]->mpUpCounterTimer[ch]
						->notifyStartCountUp();
				}
			}
		}
	}
	else if (offsetAddress == TPTMGxURRUN_OFF_ADD && (data[0] & 0x1) == 1)
	{
		for (unsigned char pe = 0; pe < mPeNum; pe++)
		{
			for (unsigned char ch = 0; 
				ch < G4TPTM_UP_COUNTER_TIMER_CH_NUM; ch++)
			{
				if ((*mpTPTMnUTRG[pe][ch] & 0x81) == mValUTRG)
				{
					mpTPTM[pe]->mpUpCounterTimer[ch]
						->notifyRestartCountUp();
				}
			}
		}
	}
	else if (offsetAddress == TPTMGxUSTP_OFF_ADD && (data[0] & 0x1) == 1)
	{
		for (unsigned char pe = 0; pe < mPeNum; pe++)
		{
			for (unsigned char ch = 0; 
				ch < G4TPTM_UP_COUNTER_TIMER_CH_NUM; ch++)
			{
				if ((*mpTPTMnUTRG[pe][ch] & 0x81) == mValUTRG)
				{
					mpTPTM[pe]->mpUpCounterTimer[ch]
						->notifyStopCountUp();
				}
			}
		}
	}
	

#ifdef TPTM_UT_TEST
	outputLog (offsetAddress, &trans, true);
#endif //TPTM_UT_TEST
}



/// <summary>reset</summary>
void TptmgFunc::reset (void)
{	
	for(unsigned int i = 0; i < TPTMG_REG_SIZE; i++)
	{
		mReg[i] = 0;
	}
}

/// <summary>set mChNum and mPeNum from NSMVTPTMV01 constructor</summary>
/// <param name="penum">the number of PE</param>
/// <param name="ch_tptmg">channle ID of TPTMG</param>
/// <param name="tptm">object of TPTM class</param>
void TptmgFunc::setMemVar(unsigned char penum, unsigned char ch_tptmg, TPTM** tptm)
{
	mPeNum = penum;
	mValUTRG = (0x80 | ch_tptmg);
	mpTPTMnUTRG = new unsigned char**[penum];
	for (unsigned char pe = 0; pe < mPeNum; pe++)
	{
		mpTPTMnUTRG[pe] = 
			new unsigned char*[G4TPTM_UP_COUNTER_TIMER_CH_NUM];
		for (unsigned char ch = 0; ch < G4TPTM_UP_COUNTER_TIMER_CH_NUM; ch++)
		{
			mpTPTMnUTRG[pe][ch] = new unsigned char;
		}
	}

	mpTPTM = tptm;
}

/// <summary>set TPTMnUTRG(which is included in TptmFunc) from NSMVTPTM </summary>
/// <param name="pe_id">PE ID</param>
/// <param name="ch">channel index ID</param>
/// <param name="reg_utrg">pointer of TPTMnUTRG(trriger select register)</param>
void TptmgFunc::setTPTMnUTRG(unsigned char pe_id, unsigned char ch, unsigned char* reg_utrg)
{
	mpTPTMnUTRG[pe_id][ch] = reg_utrg;
}


/// <summary>Judge miss align. If access by miss align, print error and exit</summary>
/// <param name="off_add">offset address</param>
/// <param name="len">data length</param>
void TptmgFunc::judgeMissAlign (unsigned int off_add
				, unsigned int len)
{
	if (0x0 < (off_add & (len - 1)))
	{
		printErrorMessage(mpWrapper->name(), "judgeMissAlign",
		"TPTM doesn't support miss align access");
		exit(1);
	}
}

#ifdef TPTM_UT_TEST
/// <summary>Debugging function</summary>
void TptmgFunc::outputLog (unsigned int offAdd, 
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

#ifdef TPTM_DEBUG_U2A
/// <summary>Debugging function</summary>
void TptmgFunc::printTPTMnUTRG (unsigned char pe, unsigned char ch)
{
	printf("%s::printTPTMnUTRG\n",mpWrapper->name());
	printf("\tpe=%d ch=%d mTPTMnUTRG=0x%x\n", pe, ch, *mpTPTMnUTRG[pe][ch]);
	fflush(stdout);
}
#endif // TPTM_DEBUG_U2A
