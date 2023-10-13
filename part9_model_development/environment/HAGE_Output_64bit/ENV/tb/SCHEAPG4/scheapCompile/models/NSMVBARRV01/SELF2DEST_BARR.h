/*************************************************************************
 *
 * SELF2DEST_BARR.h
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

#ifndef __SELF2DEST_BARR__
#define __SELF2DEST_BARR__

#include "OSCI2.h"
#include "global.h"
#include "SELF2DEST_BASE.h"

using namespace std;

class SELF2DEST_BARR 
: 
public SELF2DEST_BASE 
{
public:

//constructor destructor
	SC_HAS_PROCESS(SELF2DEST_BARR);

	SELF2DEST_BARR(sc_module_name name = "SELF2DEST_BARR"); 
	~SELF2DEST_BARR(void);

//pure virtual function


//func	
	bool convertSelfAdd (TlmTransactionType& trans);
	void restoreSelfAdd(TlmTransactionType& trans);
	void realignAdd(TlmTransactionType& trans);
	bool alignAdd(TlmTransactionType& trans);
	
private:
	map<TlmTransactionType*, ADDRESS_TYPE> mConvertAddInfo; //Argument is the pointer of transaction. Value is address of the previous to convert.
	map<TlmTransactionType*, unsigned int>  mAlignAddLength; //address length


};
#endif
