/*************************************************************************
 *
 * TptmFunc.h
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
#ifndef __TptmFunc__
#define __TptmFunc__

#include "BusSlaveFuncIf.h"
#include "OSCI2.h"
#include "global.h"

#include "TPTM_def.h"

#include <map>


#define TPTM_REG_SIZE 0x100

class TPTM;

class TptmFunc : public BusSlaveFuncIf
{
public :

//constructor destructor
	TptmFunc(TPTM* wrapper);
	~TptmFunc();

//function
	void read( unsigned int     offsetAddress, 
	     	TlmBasicPayload& trans, 
	     	BusTime_t*       t,
	     	bool             debug=0 );
  
  	void write( unsigned int     offsetAddress, 
	     	TlmBasicPayload& trans, 
	     	BusTime_t*       t,
	     	bool             debug=0 );

#if 0
  	unsigned char *getMemPtr(void){} //use only for DMI
#endif 

 	 void setfunc( ADDRESS_TYPE size ){}
	void reset (void);
	unsigned char* getRegPntr( unsigned int off_add);
	void setIntrvlTmrChNum( unsigned char ch);
	void setThresholdForWrnCmpReg(unsigned int threshold);

private :
	inline void judgeMissAlign(unsigned int off_add, unsigned int len);
	inline void judgeBitOpForDivReg(TlmBasicPayload& trans, const char* func_name, const char* reg_name);
	inline void judgeOutputWrnCmpReg(unsigned char ch, unsigned int off_add);



	//variable
	TPTM* mpWrapper; // pointer of wrapper
	unsigned char mReg[TPTM_REG_SIZE]; // register	
	unsigned char mIntrvlTmrChNum; //the number of interval timer channel
	unsigned int mThresholdForWrnCmpReg; //Threshold for Waring Message of TPTMmCMPni

#ifdef TPTM_UT_TEST
	void outputLog (unsigned int offAdd, 
			TlmTransactionType* trans, bool w_or_r);
#endif //TPTM_UT_TEST

};
#endif //__TptmFunc__
