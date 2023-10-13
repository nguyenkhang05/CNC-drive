/*************************************************************************
 *
 * BarrierSyncFunc.h
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
#ifndef __BarrierSyncFunc__
#define __BarrierSyncFunc__

#include "BusSlaveFuncIf.h"
#include "OSCI2.h"
#include "global.h"

#include <map>

#define G4BARR_MAX_DATA_LENGTH 4

class BARRIER_SYNC;

class BarrierSyncFunc : public BusSlaveFuncIf
{
public :

//constructor destructor
	BarrierSyncFunc(BARRIER_SYNC* wrappe);
	~BarrierSyncFunc();

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
	void writeBRnSYNCm(void);
	void reset (void);
	void initParam(void);

private :
	BARRIER_SYNC* mpWrapper; // pointer of wrapper
	#if 0
	bool mFlagBRnEN; // flag for BRnEN to be written
	bool mFlagBRnINIT; //flag for BRnINI to be written 	
	bool mStartBarrierSync; //start Barrier-Sync
	#endif	

	//register
	unsigned char mRegBRnEN; // BRnEN  register value
	unsigned char mRegBRnINIT; // BRnINIT  register value
	unsigned char* mRegBRnCHK; // BRnCHKm  register value
	unsigned char* mRegBRnSYNC; // BRnSYNC  register value
	unsigned char mRegBRnCHK0_7; //(m)th bit = BRnCHK(m)

	static const unsigned char mDataForRead[G4BARR_MAX_DATA_LENGTH]; 
#ifdef BARR_TST
	void outputLog(	TlmTransactionType *trans, 
			unsigned int       offsetAddress, 
			BusTime_t          *t );
  	FILE *tstFp;
#endif

#ifdef BARRTEST
	void outputBool (bool val, char str[8]);
	void outputValue ( unsigned int offsetAddress, char* str);
	void outputMem (TlmTransactionType *trans);
#endif //BARRTEST
};
#endif //__BarrierSyncFunc__
