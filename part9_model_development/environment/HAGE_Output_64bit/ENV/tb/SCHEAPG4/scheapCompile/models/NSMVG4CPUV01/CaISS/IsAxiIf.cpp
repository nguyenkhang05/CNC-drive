/*************************************************************************
 *
 *  IsAxiIf.cpp
 *
 * Copyright(c) 2011-2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

/* $Id:$ */

#include "BusMasterIf.h"
#include "BusMasterFuncIf.h"
#include "BusTimeBase.h"

#include "IsAxiIf.h"
#include "CaIss.h"

#include "CmErrMsg.h" // for WinMsg
#include "outstand.h"

using namespace CmErrMsg; // for WinMsg

/// <summary>constructor</summary>
/// <param name="pCaIss">pointer of CaIss's instance</param>
/// <param name="axiIf">pointer pointer of lapbIf</param>
/// <param name="num">I/F number</param>
IsAxiIf::IsAxiIf( CaIss *pCaIss, 
		  BusMasterIf<BUS_WIDTH_AXI> **axiIf,
		  unsigned int num ):
  mpCaIss( pCaIss ), 
  IsIfBase(),
  mTransId(1)
{

  for(int i=0; i<PE_MAX_NUM; i++){
    mpAxiIf[0] = *(axiIf + i);
  }

  for( int i=0; i<AXI_MAX_DATA; i++ ){
    if(i == 0) mDataDbg[i] = 0;
  }
  mDbgTrans.set_extension( &mDbgG3mExt );
  mDbgTrans.set_extension( &mDbgAxiExt );
}


/// <summary>destructor</summary>
IsAxiIf::~IsAxiIf(void)
{
}


/// <summary>notify the timing of transaction sending</summary>
/// <param name="trans">transaction</param>
/// <param name="t">transport time</param>
/// <returns>none</returns>
void IsAxiIf::notifyTransSend( TlmTransactionType &trans, BusTime_t &t )
{
  // notify sending
#ifdef BUSIF_TEST_LOG
  printf("\n");
  printf("IsAxiIf::notifyTransSend: time=%s\n", t.to_string().c_str());
  fflush(stdout);
#endif
}


/// <summary>notify the timing of transaction completed</summary>
/// <param name="trans">transaction</param>
/// <param name="t">transport time</param>
/// <returns>none</returns>
void IsAxiIf::notifyTransComp( TlmTransactionType &trans, BusTime_t &t )
{
#ifdef BUSIF_TEST_LOG
  printf("\n");
  printf("IsAxiIf::notifyTransComp: time=%s\n", t.to_string().c_str());
  fflush(stdout);
#endif
  std::list<trans_info*>::iterator it = mTransInfoQueue.begin();
  while( it != mTransInfoQueue.end() ){
    if((*it)->trans == &trans){
      if((*it)->acc_mode == IsIfBase::ACC_WRITE){
	BusTime_t l_time = sc_time_stamp() - (*it)->req_time + t;
#ifdef BUSIF_TEST_LOG
	printf("req time = %s, current time = %s\n", (*it)->req_time.to_string().c_str(), sc_time_stamp().to_string().c_str());
	printf("Call NfyTransComp(read) : sideband(peid(0x%x),tcid(0x%x),vcid(0x%x),spid(0x%x),vm(%d),um(%d),bitpos(0x%0x),bitop(0x%x)), addr=0x%08x, ", 
	       (*it)->sideband.GetPEID(), 
	       (*it)->sideband.GetTCID(), 
	       (*it)->sideband.GetVCID(), 
	       (*it)->sideband.GetSPID(), 
	       (*it)->sideband.GetVM(), 
	       (*it)->sideband.GetUM(), 
	       (*it)->sideband.GetBitPos(), 
	       (*it)->sideband.GetBitOp(), 
	       (*it)->addr);
	printf("%s, read)\n", l_time.to_string().c_str());
	fflush(stdout);
#endif
	mpCaIss->NfyTransComp((*it)->sideband, (*it)->addr, l_time, 0, (unsigned int)((*it)->acc_mode));
      }else if((*it)->acc_mode == IsIfBase::ACC_READ){
	BusTime_t l_time = sc_time_stamp() - (*it)->req_time + t;
#ifdef BUSIF_TEST_LOG
	printf("req time = %s, current time = %s\n", (*it)->req_time.to_string().c_str(), sc_time_stamp().to_string().c_str());
	printf("Call NfyTransComp(read) : sideband(peid(0x%x),tcid(0x%x),vcid(0x%x),spid(0x%x),vm(%d),um(%d),bitpos(0x%0x),bitop(0x%x)), addr=0x%08x, ", 
	       (*it)->sideband.GetPEID(), 
	       (*it)->sideband.GetTCID(), 
	       (*it)->sideband.GetVCID(), 
	       (*it)->sideband.GetSPID(), 
	       (*it)->sideband.GetVM(), 
	       (*it)->sideband.GetUM(), 
	       (*it)->sideband.GetBitPos(), 
	       (*it)->sideband.GetBitOp(), 
	       (*it)->addr);
	printf("%s, read)\n", l_time.to_string().c_str());
	fflush(stdout);
#endif
	mpCaIss->NfyTransComp((*it)->sideband, (*it)->addr, l_time, getTransDataVal(trans), (unsigned int)((*it)->acc_mode));
      }else if((*it)->acc_mode == IsIfBase::ACC_FETCH){
	BusTime_t l_time = sc_time_stamp() - (*it)->req_time + t;
#ifdef BUSIF_TEST_LOG
	printf("req time = %s, current time = %s\n", (*it)->req_time.to_string().c_str(), sc_time_stamp().to_string().c_str());
	printf("Call NfyTransComp(fetch) : sideband(peid(0x%x),tcid(0x%x),vcid(0x%x),spid(0x%x),vm(%d),um(%d),bitpos(0x%0x),bitop(0x%x)), addr=0x%08x, ", 
	       (*it)->sideband.GetPEID(), 
	       (*it)->sideband.GetTCID(), 
	       (*it)->sideband.GetVCID(), 
	       (*it)->sideband.GetSPID(), 
	       (*it)->sideband.GetVM(), 
	       (*it)->sideband.GetUM(), 
	       (*it)->sideband.GetBitPos(), 
	       (*it)->sideband.GetBitOp(), 
	       (*it)->addr);
	printf("%s, fetch)\n", l_time.to_string().c_str());
	fflush(stdout);
#endif
	mpCaIss->NfyTransComp((*it)->sideband, (*it)->addr, l_time, getTransDataVal(trans), (unsigned int)((*it)->acc_mode));
      }
      DeleteTrans(trans);
      delete *it;
      mTransInfoQueue.erase(it);
      break;
    }else{
      ++it;
    }
  }
}


/// <summary>update cycle(dummy)</summary>
/// <returns>none</returns>
void IsAxiIf::CyclicHandler (void)
{
}


/// <summary>request the write transaction</summary>
/// <param name="sideband">sideband</param>
/// <param name="addr">address</param>
/// <param name="p_data">data pointer</param>
/// <param name="size">data length</param>
/// <returns>none</returns>
bool IsAxiIf::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* p_data, ByteSize size)
{
  unsigned char peid = getPeId(sideband);

#ifdef BUSIF_TEST_LOG
  printf("\n");
  printf("IsAxiIf::TargetWrite(Normal_S): ");
  printf("sideband(peid(0x%x),tcid(0x%x),vcid(0x%x),spid(0x%x),vm(%d),um(%d),bitpos(0x%0x),bitop(0x%x)), ", 
	 sideband.GetPEID(), 
	 sideband.GetTCID(), 
	 sideband.GetVCID(), 
	 sideband.GetSPID(), 
	 sideband.GetVM(), 
	 sideband.GetUM(), 
	 sideband.GetBitPos(), 
	 sideband.GetBitOp());
  printf("address=0x%08x, size=%x\n", addr, size);
  fflush(stdout);
#endif

  // normal access ( to caluc the write time )
  BusTime_t t;
  bool isNotFull;

  //cleate transaction and data_area
  unsigned char   *data          = new unsigned char[AXI_MAX_DATA];
  TlmBasicPayload *basic_payload = new TlmBasicPayload;
  TlmG3mExtension *g3m_ext       = new TlmG3mExtension;
  TlmAxiExtension *axi_ext       = new TlmAxiExtension;
  trans_info      *t_info        = new trans_info;

  basic_payload->set_extension( g3m_ext );
  basic_payload->set_extension( axi_ext );

  for(int i=0; i<size; i++){
    *(data+i) = *(p_data+i);
  }

  setTransBasic( addr, size, data, *basic_payload );
  setTransG3m( getVmId(sideband), 
	       getTcId(sideband), 
	       peid,
	       getSpId(sideband),
	       getUserMode(sideband), 
	       getVirtualMode(sideband), 
               *basic_payload, 
	       *g3m_ext );
  setTransAxi( TlmAxiExtension::BURST_INCR,  // burst type
	       getLockMode(sideband),        // lock
	       false,                        // cachable
	       false,                        // bufferable
	       false,                        // secure
               mTransId,                     // transaction ID
	       getBitOpType(sideband),       // bit operation type
	       getBitOpPos(sideband),          // bit operation pos
	       *basic_payload,               // Basic Payload 
	       *axi_ext );                   // Axi Extension

  if(mTransId != 0xffffffff) mTransId++;
  else                       mTransId = 1;

  t = SC_ZERO_TIME;
  t_info->sideband   = sideband;
  t_info->addr       = addr;
  t_info->size       = size;
  t_info->acc_mode   = IsIfBase::ACC_WRITE;
  t_info->trans      = basic_payload;
  t_info->req_time   = sc_time_stamp();

  mTransInfoQueue.push_back(t_info);

  isNotFull = mpAxiIf[peid-1]->write( *basic_payload,   // payload pointer
				      false,            // debug?
				      t,                // local time
				      false );          // with wait?

#ifdef BUSIF_TEST_LOG
  printf("IsAxiIf::TargetWrite(Normal_E): Under operating transaction num=%d\n", mTransInfoQueue.size());
  printf("     ");
  for (int i=0; i<size; i++){
    printf("data%d=0x%08x, ", i, data[i]);
  }
  printf("\n\n");
  fflush(stdout);
#endif

  return(SYSERR_NONE);
}


/// <summary>request the read transaction</summary>
/// <param name="sideband">sideband</param>
/// <param name="addr">address</param>
/// <param name="p_data">data pointer</param>
/// <param name="size">data length</param>
/// <returns>none</returns>
bool IsAxiIf::TargetRead  (SideBand sideband, PhysAddr addr, uint8_t* p_data, ByteSize size)
{
#ifdef BUSIF_TEST_LOG
  printf("\n");
  printf("IsAxiIf::TargetRead(Debug_S): ");
  printf("sideband(peid(0x%x),tcid(0x%x),vcid(0x%x),spid(0x%x),vm(%d),um(%d),bitpos(0x%0x),bitop(0x%x)), ", 
	 sideband.GetPEID(), 
	 sideband.GetTCID(), 
	 sideband.GetVCID(), 
	 sideband.GetSPID(), 
	 sideband.GetVM(), 
	 sideband.GetUM(), 
	 sideband.GetBitPos(), 
	 sideband.GetBitOp());
  printf("address=0x%08x, size=%x\n", addr, size);
  fflush(stdout);
#endif

  unsigned char peid = getPeId(sideband);

  // debug access ( to get the value for trace data)
  setTransBasic( addr, size, p_data, mDbgTrans );
  setTransG3m( getVmId(sideband), 
	       getTcId(sideband), 
	       peid,
	       getSpId(sideband),
	       getUserMode(sideband), 
	       getVirtualMode(sideband), 
               mDbgTrans,
	       mDbgG3mExt );
  setTransAxi( TlmAxiExtension::BURST_INCR,  // burst type
	       getLockMode(sideband),        // lock
	       false,                        // cachable
	       false,                        // bufferable
	       false,                        // secure
               0,                            // transaction ID
	       getBitOpType(sideband),       // bit operation type
	       getBitOpPos(sideband),        // bit operation pos
               mDbgTrans,                    // Basic Payload
	       mDbgAxiExt );                 // Axi Extension
  mpAxiIf[peid-1]->read( mDbgTrans,    // payload pointer
			 true );       // debug?

#ifdef BUSIF_TEST_LOG
  printf("IsAxiIf::TargetRead(Debug_E): ");
  for (int i=0; i<size; i++){
    printf("data%d=0x%08x, ", i, *(p_data+i));
  }
  printf("\n");
  printf("IsAxiIf::TargetRead(Normal_S): ");
  printf("sideband(peid(0x%x),tcid(0x%x),vcid(0x%x),spid(0x%x),vm(%d),um(%d),bitpos(0x%0x),bitop(0x%x)), ", 
	 sideband.GetPEID(), 
	 sideband.GetTCID(), 
	 sideband.GetVCID(), 
	 sideband.GetSPID(), 
	 sideband.GetVM(), 
	 sideband.GetUM(), 
	 sideband.GetBitPos(), 
	 sideband.GetBitOp());
  printf("address=0x%08x, size=%x\n", addr, size);
  fflush(stdout);
#endif

  // normal access ( to caluc the read time )
  BusTime_t t;
  bool isNotFull;

  //cleate transaction and data_area
  unsigned char   *data          = new unsigned char[AXI_MAX_DATA];
  TlmBasicPayload *basic_payload = new TlmBasicPayload;
  TlmG3mExtension *g3m_ext       = new TlmG3mExtension;
  TlmAxiExtension *axi_ext       = new TlmAxiExtension;
  trans_info      *t_info        = new trans_info;

  basic_payload->set_extension( g3m_ext );
  basic_payload->set_extension( axi_ext );

  for(int i=0; i<size; i++){
    *(data+i) = 0;
  }

  setTransBasic( addr, size, data, *basic_payload );
  setTransG3m( getVmId(sideband), 
	       getTcId(sideband), 
	       peid,
	       getSpId(sideband),
	       getUserMode(sideband), 
	       getVirtualMode(sideband), 
               *basic_payload,
	       *g3m_ext );
  setTransAxi( TlmAxiExtension::BURST_INCR,  // burst type
	       getLockMode(sideband),        // lock
	       false,                        // cachable
	       false,                        // bufferable
	       false,                        // secure
               mTransId,                     // transaction ID
	       getBitOpType(sideband),       // bit operation type
	       getBitOpPos(sideband),        // bit operation pos
               *basic_payload,               // Basic Payload
	       *axi_ext);                    // Axi Extension

  if(mTransId != 0xffffffff) mTransId++;
  else                       mTransId = 1;

  t = SC_ZERO_TIME;
  t_info->sideband   = sideband;
  t_info->addr       = addr;
  t_info->size       = size;
  t_info->acc_mode   = IsIfBase::ACC_READ;
  t_info->trans      = basic_payload;
  t_info->req_time   = sc_time_stamp();

  mTransInfoQueue.push_back(t_info);

  isNotFull = mpAxiIf[peid-1]->read( *basic_payload,   // payload pointer
				     false,            // debug?
				     t,                // local time
				     false );          // with wait?

#ifdef BUSIF_TEST_LOG
  printf("IsAxiIf::TargetRead(Normal_E): Under operating transaction num=%d\n", mTransInfoQueue.size()");
  printf("\n");
  fflush(stdout);
#endif

  return(SYSERR_NONE);
}


/// <summary>request the debug write transaction</summary>
/// <param name="addr">address</param>
/// <param name="p_data">data pointer</param>
/// <param name="size">data length</param>
/// <returns>none</returns>
bool IsAxiIf::TargetWriteDebug (PhysAddr addr, uint8_t* p_data, ByteSize size)
{
  setTransBasic( addr, size, p_data, mDbgTrans );
  setTransG3m( 0, 0, DEBUG_MASTER_BMID, 0, 0, 0, mDbgTrans, mDbgG3mExt );
  setTransAxi( TlmAxiExtension::BURST_INCR, 0, false, false, false, 0, 0, 0, mDbgTrans, mDbgAxiExt );
  mpAxiIf[0]->write(  mDbgTrans,       // payload pointer
		      true );          // debug?

  return(true);
}


/// <summary>request the debug read transaction</summary>
/// <param name="addr">address</param>
/// <param name="p_data">data pointer</param>
/// <param name="size">data length</param>
/// <returns>none</returns>
bool IsAxiIf::TargetReadDebug  (PhysAddr addr, uint8_t* p_data, ByteSize size)
{
  setTransBasic( addr, size, p_data, mDbgTrans );
  setTransG3m( 0, 0, DEBUG_MASTER_BMID, 0, 0, 0, mDbgTrans, mDbgG3mExt );
  setTransAxi( TlmAxiExtension::BURST_INCR, 0, false, false, false, 0, 0, 0, mDbgTrans, mDbgAxiExt );
  mpAxiIf[0]->read(  mDbgTrans,       // payload pointer
		     true );          // debug?

  return(true);
}


/// <summary>reset operation</summary>
/// <returns>none</returns>
void IsAxiIf::HardReset (void)
{
  for( int j=0; j<AXI_MAX_DATA; j++ ){
    mDataDbg[j] = 0;
  }
}


/// <summary>get bit-operation type from sideband data</summary>
/// <param name="sideband">sideband</param>
/// <returns>AXI bit-operation type</returns>
TlmAxiExtension::AxiBitop_t IsAxiIf::getBitOpType (SideBand sideband)
{
  unsigned int type = getBitOp(sideband);
  switch(type){
  case SideBand::BITOP_NONE: return(TlmAxiExtension::BITOP_NON);
  case SideBand::BITOP_SET:  return(TlmAxiExtension::BITOP_SET1);
  case SideBand::BITOP_CLR:  return(TlmAxiExtension::BITOP_CLR1);
  case SideBand::BITOP_NOT:  return(TlmAxiExtension::BITOP_NOT1);
  }
}


/// <summary>get mask bit of bit-operation from sideband data</summary>
/// <param name="sideband">sideband</param>
/// <returns>mask bit of bit-operation</returns>
unsigned char IsAxiIf::getBitOpPos (SideBand sideband)
{
  return((unsigned char)(getBitPos(sideband));
}


/// <summary>get lock mode from sideband data</summary>
/// <param name="sideband">sideband</param>
/// <returns>lock mode</returns>
bool IsAxiIf::getLockMode (SideBand sideband)
{
  if(((sideband >> 20) & 0x3) != TlmAxiExtension::BITOP_NON){
    return (true);
  }else{
    return (false);
  }
}


/// <summary>delete transaction instance</summary>
/// <param name="trans">transaction</param>
/// <returns>none</returns>
void IsAxiIf::DeleteTrans ( TlmTransactionType &trans )
{
  unsigned char* data_ptr  = trans.get_data_ptr();
  delete [] data_ptr;

  TlmG3mExtension *g3m_ext = (TlmG3mExtension *)0;
  trans.get_extension( g3m_ext);
  trans.release_extension( g3m_ext );

  TlmAxiExtension *axi_ext = (TlmAxiExtension *)0;
  trans.get_extension( axi_ext);
  trans.release_extension( axi_ext );
 
  delete &trans;
}
