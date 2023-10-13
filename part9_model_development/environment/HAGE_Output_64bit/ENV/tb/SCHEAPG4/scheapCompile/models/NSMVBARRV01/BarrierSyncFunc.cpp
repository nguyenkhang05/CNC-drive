#include "BarrierSyncFunc.h"
#include "BARRIER_SYNC.h"
#include "NSMVBARRV01.h"
#include "CmErrMsg.h"

using namespace CmErrMsg;

#define BRNINIT_OFF_ADD 0x000
#define BRNEN_OFF_ADD 0x004
#define BRNCHK_OFF_ADD 0x800
#define BRNSYNC_OFF_ADD 0x804

const unsigned char BarrierSyncFunc::mDataForRead[G4BARR_MAX_DATA_LENGTH] = {0};

/// <summary>constructor</summary>
/// <param name="wrapper">pointer of wrapper</param>
BarrierSyncFunc::BarrierSyncFunc(BARRIER_SYNC* wrapper) :
BusSlaveFuncIf(),
mpWrapper(wrapper),
mRegBRnEN(0),
mRegBRnINIT(0),
mRegBRnCHK(0),
mRegBRnSYNC(0),
mRegBRnCHK0_7(0)
{

#if defined(BARRTEST) || defined(BARR_TST)
	tstFp = stdout;
#endif

}

/// <summary>destructor</summary>
BarrierSyncFunc::~BarrierSyncFunc(void)
{
	delete mRegBRnCHK;
	delete mRegBRnSYNC;
}

///<summary>TLM read function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
void BarrierSyncFunc::read( unsigned int     offsetAddress, 
		     TlmBasicPayload& trans, 
		     BusTime_t*       t,
		     bool             debug )
{
	unsigned char* data = trans.get_data_ptr();
	if (offsetAddress == BRNINIT_OFF_ADD)
	{
		*data = mRegBRnINIT;
	}
	//BRnEN
	else if (offsetAddress == BRNEN_OFF_ADD)
	{
		*data = mRegBRnEN;
	}
	//BRnCHK or BRnSYNCm or Offset Address don't exist in Barrier Sync
	else
	{
		char warning_msg_buf[1024];
		if (BRNCHK_OFF_ADD <= offsetAddress && offsetAddress < (unsigned int)(BRNCHK_OFF_ADD + mpWrapper->mPeNum * G4BARR_PE_OFF_ADD))
		{
			unsigned char pe_index = (offsetAddress - BRNCHK_OFF_ADD) / G4BARR_PE_OFF_ADD; 
			unsigned int off_add = offsetAddress - G4BARR_PE_OFF_ADD * pe_index;
			if (off_add  == BRNCHK_OFF_ADD)
			{
				*data = mRegBRnCHK[pe_index];
			}
			else if (off_add  == BRNSYNC_OFF_ADD)
			{ 
				*data = mRegBRnSYNC[pe_index];
			}
			else
			{
				if (! debug)
				{
					sprintf(warning_msg_buf, "The address 0x%llx is in reserved area.", trans.get_address());
					printWarningMessage(mpWrapper->name(), "read", warning_msg_buf);
				}
				memcpy(trans.get_data_ptr(), mDataForRead, trans.get_data_length());
			}
			

		}
		else
		{
			if (! debug)
			{
				sprintf(warning_msg_buf, "The address 0x%llx is in reserved area.", trans.get_address());
				printWarningMessage(mpWrapper->name(), "read", warning_msg_buf);
			}
			memcpy(trans.get_data_ptr(), mDataForRead, trans.get_data_length());
		}
	}


#ifdef BARR_TST
	if(tstFp)
   	{	fprintf(tstFp,"%s:",sc_time_stamp().to_string().c_str());
	    	fprintf(tstFp,"%s::BarrierSyncFunc::read() is called\n",
		    	mpWrapper->name());
	    	outputLog( &trans, offsetAddress, t );
	}
#endif //BARR_TST
	
#ifdef BARRTEST
	outputValue( offsetAddress, "READ");
#endif //BARRTEST
}

///<summary>TLM write function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
void BarrierSyncFunc::write( unsigned int     offsetAddress, 
                   TlmBasicPayload& trans, 
                   BusTime_t*       t,
                   bool             debug )
{
	unsigned char* data = trans.get_data_ptr();
	
	//BRnEN
	if (offsetAddress == BRNEN_OFF_ADD)
	{
		//After masking for reserved bit, write transaction to BRnEN 
		mRegBRnEN = ((0xff >> (PE_MAX_NUM - mpWrapper->mPeNum)) & *data); 
	}
	//BRnINIT
	else if ( offsetAddress == BRNINIT_OFF_ADD)
	{
		
		if ((*data & 0x1) == 0x1)
		{
			for (unsigned char j = 0; j < mpWrapper->mPeNum; j++)
			{
				mRegBRnCHK[j] = 0;
				mRegBRnSYNC[j] = 0;       
			}
			mRegBRnCHK0_7 = 0;
		}

	}
	//BRnCHK or BRnSYNCm or Offset Address don't exist in Barrier Sync
	else
	{
		char warning_msg_buf[1024];
		//if (BRNCHK_OFF_ADD <= offsetAddress && offsetAddress < BRNCHK_OFF_ADD + mpWrapper->mPeNum * G4BARR_PE_OFF_ADD - 1)
		if (BRNCHK_OFF_ADD <= offsetAddress && offsetAddress < (unsigned int)(BRNCHK_OFF_ADD + mpWrapper->mPeNum * G4BARR_PE_OFF_ADD))
		{
			unsigned char pe_index = (offsetAddress - BRNCHK_OFF_ADD) / G4BARR_PE_OFF_ADD;
			unsigned int off_add = offsetAddress - G4BARR_PE_OFF_ADD * pe_index;
			if (off_add  == BRNCHK_OFF_ADD)
			{

				mRegBRnCHK[pe_index] = 0x1;
				mRegBRnCHK0_7 |= (mRegBRnCHK[pe_index] << pe_index);

#ifdef BARRDEBUG
				printf ("%s::write BRnCHK\n  mRegBRnCHK[%d]=0x%x\tmRegBRnCHK0_7=0x%x\t~mRegBRnEN=0x%x\tmRegBRnCHK0_7 | (~mRegBRnEN) =0x%x\n", mpWrapper->name(), pe_index, mRegBRnCHK[pe_index], mRegBRnCHK0_7, ~mRegBRnEN, (mRegBRnCHK0_7 | (~mRegBRnEN) ) ); fflush(stdout);
#endif //BARRDEBUG


				if (((unsigned char)(mRegBRnCHK0_7 | (~mRegBRnEN) )) == 0xff)
				{
#ifdef BARRTEST
					outputValue(offsetAddress, "reached barrier");
#endif //BARRTEST

					for (unsigned char i = 0; i < mpWrapper->mPeNum; i++)
					{
						mRegBRnCHK[i] = 0x0;
					}
					mpWrapper->notify_writeBRnSYNCm();
					mRegBRnCHK0_7 = 0x0;
				}
			}
			else if (off_add  == BRNSYNC_OFF_ADD)
			{ 
				//After masking for reserved bit, write transaction to BRnSYNCm
				mRegBRnSYNC[pe_index] = (*data & 0x1);
			}
			else
			{
				sprintf(warning_msg_buf, "The address 0x%llx is in reserved area.", trans.get_address());
				printWarningMessage(mpWrapper->name(), "write", warning_msg_buf);
			}

		}
		else
		{
			sprintf(warning_msg_buf, "The address 0x%llx is in reserved area.", trans.get_address());
			printWarningMessage(mpWrapper->name(), "write", warning_msg_buf);
		}
	}
#ifdef BARR_TST
	if(tstFp)
   	{	fprintf(tstFp,"%s:",sc_time_stamp().to_string().c_str());
	    	fprintf(tstFp,"%s::BarrierSyncFunc::write() is called\n",
		    	mpWrapper->name());
	    	outputLog( &trans, offsetAddress, t );
	}
#endif //BARR_TST

#ifdef BARRTEST
     	char str[128];
     	sprintf(str, "WRITE\n");
     	outputValue( offsetAddress, str);       
#endif //BARRTEST
	
}

/// <summary>Write BRnSYNCm 1 cycle after BRnCHKm was cleared.</summary>
void BarrierSyncFunc::writeBRnSYNCm (void)
{	
	unsigned char brnen = mRegBRnEN;	
	for (unsigned char i = 0; i < mpWrapper->mPeNum; i++)
	{	if ((brnen & 0x1) == 0x1)
		{	
			 mRegBRnSYNC[i]= 0x1;
		}
		brnen >>= 1;
	}	
#ifdef BARRTEST
	outputValue( 0x0, "writeBRnSYNCm");	
#endif //BARRTEST

}

/// <summary>reset</summary>
void BarrierSyncFunc::reset (void)
{	
	mRegBRnINIT = 0;
	mRegBRnEN = 0;
	for (unsigned char i = 0; i < mpWrapper->mPeNum; i++)
	{
		mRegBRnCHK[i] = 0;
		mRegBRnSYNC[i] = 0;		
	}
	mRegBRnCHK0_7 = 0;
#ifdef BARRTEST
	outputValue(0, "reset");
#endif //BARRTEST
}

/// <summary>create mRegBRnCHK and mRegBRnSYNC</summary>
void BarrierSyncFunc::initParam (void)
{
	mRegBRnCHK = new unsigned char[mpWrapper->mPeNum];
	mRegBRnSYNC = new unsigned char[mpWrapper->mPeNum];
	for (unsigned char i = 0; i < mpWrapper->mPeNum; i++)
	{
		mRegBRnCHK[i] = 0;
		mRegBRnSYNC[i] = 0;
	}
}

#ifdef BARRTEST
void BarrierSyncFunc::outputBool (bool val, char str[8])
{
	if (val)
	{
		strcpy(str, "true");
	}
	else
	{
		strcpy(str, "false");
	}
}

void BarrierSyncFunc::outputValue (unsigned int offsetAddress, char *str)
{
	printf("\n%s:%s::%s\n", sc_time_stamp().to_string().c_str(),mpWrapper->name(), str);
	printf( "  offsetAddress = %x\n", offsetAddress);fflush(stdout);

	printf( "  BRnEN = %d\tBRnINIT = %d\n",mRegBRnEN, mRegBRnINIT);  fflush(stdout);
	for (unsigned char i = 0; i < mpWrapper->mPeNum; i++)
        {
		printf( "  BRnCHK%d = %d\tBRnSYNC%d = %d\n"
			, i
			, mRegBRnCHK[i]
			, i
			, mRegBRnSYNC[i] 
			);fflush(stdout);
	}	
}

void BarrierSyncFunc::outputMem (TlmTransactionType *trans)
{
	int data_length = trans->get_data_length();
	unsigned char *data = trans->get_data_ptr();
	printf("  transaction pointer=0x%llx\n", trans); fflush(stdout);
	printf("  address=0x%llx\n", trans->get_address()); fflush(stdout);
	for (int i = 0; i < data_length; i++)
	{
		printf("  data[%d] = %x\n", i, data[i]);fflush(stdout);
	}
}

#endif //BARRTEST

#ifdef BARR_TST
void BarrierSyncFunc::outputLog( TlmTransactionType *trans, 
				  unsigned int       offsetAddress, 
				  BusTime_t          *t )
{
  	fprintf(tstFp,"        offsetAddress     =0x%llx\n",
		  offsetAddress);
	fprintf(tstFp,"        t                 =%s\n",
	        t->to_string().c_str());
	fprintf(tstFp,"        transaction pointer=0x%llx\n",
	        trans);
	if( trans->get_command() == tlm::TLM_READ_COMMAND ){
	  fprintf(tstFp,"        TLM_READ_COMMAND\n");
	}
	else{
	  fprintf(tstFp,"        TLM_WRITE_COMMAND\n");
	}
	fprintf(tstFp,"        address     =0x%llx\n",
	        trans->get_address());
	int data_length = trans->get_data_length();
	fprintf(tstFp,"        data_length =0x%llx\n",
	        data_length);
	unsigned char *data = trans->get_data_ptr();
	for(int i=0; i<data_length; i++){
	  fprintf(tstFp,"        data[%d]     =0x%x\n",
	          i,data[i]);
	}
	if(trans->get_response_status() == tlm::TLM_OK_RESPONSE ){
	  fprintf(tstFp,"        status      =tlm::TLM_OK_RESPONSE\n" );
	}
	else{
	  fprintf(tstFp,"        status      =other\n" );
	}
	//for(int i=0; i<data_length; i++){
	//  fprintf(tstFp,"        mMem[%d]     =0x%x\n",
	//          i,mMem[i]);
	//}
	//TlmG3mExtension *G3mExt = NULL;
        //trans.get_extension(G3mExt);
        //unsigned char peid = G3mExt->getPeId();
	//fprintf(tstFp,"       peid\t= %d\n",  peid);
}
#endif //BARR_TEST
