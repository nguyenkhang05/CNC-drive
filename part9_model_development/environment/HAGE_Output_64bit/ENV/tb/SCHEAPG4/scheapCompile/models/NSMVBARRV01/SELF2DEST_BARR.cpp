/*************************************************************************
 *
 * SELF2DEST_BARR.cpp
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

#include <map>

#include "CmErrMsg.h"
#include "SELF2DEST_BARR.h"

using namespace CmErrMsg;


/// <summary>constructor</summary>
/// <param name="name">module name</param>
SELF2DEST_BARR::SELF2DEST_BARR(sc_module_name name): 
SELF2DEST_BASE(name)
{

}

/// <summary>destructor</summary>
SELF2DEST_BARR::~SELF2DEST_BARR(void) 
{
}



/// <summary>align address</summary>
/// <param name="trans">transaction</param>
/// <returns>successful or unsuccessful</returns>
bool SELF2DEST_BARR::alignAdd(TlmTransactionType& trans)
{
	ADDRESS_TYPE pre_add = trans.get_address();
	unsigned char data_length = trans.get_data_length();
	unsigned char *data = trans.get_data_ptr();
	if (4 < data_length)
	{
		printWarningMessage(this->name(), "alignAdd", " Barrier Sync register cannot be received transaction of which data length is 5 byte and over.");
		return(false);
	}
        else
	{ 
		if(data_length > 1)
        	{
        	        unsigned int diff = ((pre_add + data_length - 1) & 0xFFFFFFFC) - pre_add;
        	        if( diff < data_length )
        	        {
        	                trans.set_address(pre_add + diff);
				mAlignAddLength[&trans] = data_length;
        	                trans.set_data_length(1);
			
				trans.set_data_ptr(&data[diff]);
			
				mConvertAddInfo[&trans] = pre_add;
        	        }
        	}
		return (true);
	}
}

/// <summary>realign address</summary>
/// <param name="trans">transaction</param>
void SELF2DEST_BARR::realignAdd(TlmTransactionType& trans)
{

	if (mConvertAddInfo.find(&trans) != mConvertAddInfo.end() && mAlignAddLength.find(&trans) != mAlignAddLength.end())
        {
		ADDRESS_TYPE  align_add = trans.get_address();
		unsigned int pre_add = mConvertAddInfo[&trans];
		unsigned int pre_data_length = mAlignAddLength[&trans];		
		unsigned char diff = align_add - pre_add;
	
		unsigned char* data = trans.get_data_ptr();
                trans.set_data_ptr(&data[-diff]);

		if (trans.get_command() == tlm::TLM_READ_COMMAND)
		{
			data = trans.get_data_ptr();
			for (unsigned int i = 0; i < pre_data_length; i++)
			{
				if (i != diff)
				{
					data[i] = 0;
				}
			}
		}


		trans.set_data_length(pre_data_length);
		mAlignAddLength.erase(&trans);
                trans.set_address(mConvertAddInfo[&trans]);
                mConvertAddInfo.erase(&trans);
        }
}

/// <summary>convert self address to destination address</summary>
/// <param name="trans">transaction</param>
/// <returns>successful or unseccessful</returns>
bool SELF2DEST_BARR::convertSelfAdd(TlmTransactionType& trans)
{

	if (! alignAdd(trans))
	{
		return (false);
	}	

	return(SELF2DEST_BASE::convertSelfAdd(trans));
}

/// <summary>restore address converted</summary>
/// <param name="trans">transaction</param>
/// <returns>none</returns>
void SELF2DEST_BARR::restoreSelfAdd(TlmTransactionType& trans)
{	
	realignAdd(trans);
	SELF2DEST_BASE::restoreSelfAdd(trans);
}


#ifdef SELF2DEST_BARR_TEST
void SELF2DEST_BARR::outputAdd (TlmTransactionType& trans, char str[128])
{
	printf("%s:",sc_time_stamp().to_string().c_str()); fflush(stdout);
	printf("%s::%s\n", name(), str); fflush(stdout);
	if( trans.get_command() == tlm::TLM_READ_COMMAND )
        {
                printf("  TLM_READ_COMMAND\n");fflush(stdout);
        }
        else
        {
                printf("  TLM_WRITE_COMMAND\n");fflush(stdout);
        }
	int data_length = trans.get_data_length();
        unsigned char *data = trans.get_data_ptr();

	TlmG3mExtension *G3mExt;
        trans.get_extension(G3mExt);
        unsigned char bmid = G3mExt->getPeId();

        printf("  transaction=0x%llx\n", &trans, bmid); fflush(stdout);
        printf("  address=0x%llx\n  BMID=0x%x\n", trans.get_address(), bmid); fflush(stdout);
	for (unsigned int i = 0; i < data_length; i++)
	{
		printf("  data[%d]=0x%x\n",i, data[i]); fflush(stdout);
	}
}
#endif //SELF2DEST_BARR_TEST
