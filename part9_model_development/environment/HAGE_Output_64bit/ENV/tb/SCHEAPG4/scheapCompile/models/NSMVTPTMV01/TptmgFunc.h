/*************************************************************************
 *
 * TptmgFunc.h
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
#ifndef __TptmgFunc__
#define __TptmgFunc__

#include "BusSlaveFuncIf.h"
#include "OSCI2.h"
#include "global.h"

#include "TPTM_def.h"

#include <map>


#define TPTMG_REG_SIZE 0x100

class TPTM;
class TPTMG;

class TptmgFunc : public BusSlaveFuncIf
{
public :

//constructor destructor
	TptmgFunc(TPTMG* wrapper);
	~TptmgFunc();

//function
	void read( unsigned int     offsetAddress, 
	     	TlmBasicPayload& trans, 
	     	BusTime_t*       t,
	     	bool             debug=0 );
  
  	void write( unsigned int     offsetAddress, 
	     	TlmBasicPayload& trans, 
	     	BusTime_t*       t,
	     	bool             debug=0 );

	void setMemVar(unsigned char penum, unsigned char ch_tptmg, TPTM** tptm);
	void setTPTMnUTRG(unsigned char pe_id, unsigned char ch, unsigned char* reg_utrg);

#if 0
  	unsigned char *getMemPtr(void){} //use only for DMI
#endif 

 	 void setfunc( ADDRESS_TYPE size ){}
	void reset (void);

private :
	inline void judgeMissAlign(unsigned int off_add, unsigned int len);



	//variable
	TPTMG* mpWrapper; // pointer of wrapper
	TPTM** mpTPTM; // pointer of TPTM
	unsigned char mPeNum; // the number of PE
	unsigned char mReg[TPTMG_REG_SIZE]; // register
	unsigned char ***mpTPTMnUTRG; //pointer of TPTMnUTRG(trigger select register)
	unsigned char mValUTRG; //TPTMnUTRG value when TPTMGx can cotorol Simultaneous 	

#ifdef TPTM_UT_TEST
	void outputLog (unsigned int offAdd, 
			TlmTransactionType* trans, bool w_or_r);
public:
	void printTPTMnUTRG(unsigned char pe, unsigned char ch);
#endif //TPTM_UT_TEST

};
#endif //__TptmgFunc__
