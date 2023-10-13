/*
* Copyright(c) 2012-2017 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#ifndef __MEMORY_FUNCTION64_H__
#define __MEMORY_FUNCTION64_H__

#include "BusSlaveFuncIf.h"
#include "OSCI2.h"

class MemoryFunction64 : public BusSlaveFuncIf{
  
 public:
#ifdef BUSIF_TST
  MemoryFunction64(FILE *fp):BusSlaveFuncIf(),tstFp(fp){}
#else  //BUSIF_TST
  MemoryFunction64(void):BusSlaveFuncIf(){}
#endif //BUSIF_TST
  
  ~MemoryFunction64(void){}
  
  void read( unsigned int     offsetAddress, 
	     TlmBasicPayload& trans, 
	     BusTime_t*       t,
	     bool             debug=0)
    {
      memcpy( trans.get_data_ptr(), 
	      &mMem[offsetAddress], 
	      trans.get_data_length() );
      if( !debug ){
	// Use t or other members of trans if necessary
      }
#ifdef BUSIF_TST
      if(tstFp){
	fprintf(tstFp,"%s:",sc_time_stamp().to_string().c_str());
	fprintf(tstFp,"MemoryFunction::read() is called\n" );
	outputLog( &trans, offsetAddress, t );
      }
#endif //BUSIF_TST
    }
  
  void write( unsigned int     offsetAddress, 
	      TlmBasicPayload& trans, 
	      BusTime_t*       t,
	      bool             debug=0)
    {
      memcpy( &mMem[offsetAddress], 
	      trans.get_data_ptr(), 
	      trans.get_data_length() );
      if( !debug ){
	// Use t or other members of trans if necessary
      }
#ifdef BUSIF_TST
      if(tstFp){
	fprintf(tstFp,"%s:",sc_time_stamp().to_string().c_str());
	fprintf(tstFp,"MemoryFunction::write() is called\n" );
	outputLog( &trans, offsetAddress, t );
      }
#endif //BUSIF_TST
    }
  
  void setfunc( ADDRESS_TYPE size )
    {
      mMem = new unsigned char [size];
      for( unsigned int i=0; i<size; i++ )
	mMem[i] = 0;
    }
  
 private:
  unsigned char *mMem;
#ifdef BUSIF_TST
  void outputLog( TlmTransactionType *trans, 
		  unsigned int       offsetAddress, 
		  BusTime_t          *t )
  {
    fprintf(tstFp,"        offsetAddress     =0x%x\n",
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
    fprintf(tstFp,"        data_length =0x%x\n",
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
    for(int i=0; i<data_length; i++){
      fprintf(tstFp,"        mMem[%d]     =0x%x\n",
	      i,mMem[i]);
    }
  }
  
  FILE *tstFp;
  
#endif //BUSIF_TST
  
};

#endif // __MEMORY_FUNCTION64_H__
