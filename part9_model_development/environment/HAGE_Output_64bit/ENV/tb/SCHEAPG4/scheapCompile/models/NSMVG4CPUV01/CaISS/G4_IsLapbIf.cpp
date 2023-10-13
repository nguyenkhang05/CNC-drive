/*************************************************************************
 *
 *  G4_IsLapbIf.cpp
 *
 * Copyright(c) 2016-2018 Renesas Electronics Corporation
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
#if defined(_MSC_VER) // for cforestg4x (cforestg4x uses std::min and max)
#undef min
#undef max
#endif

#include "G4_IsLapbIf.h"
#include "G4_CaIss.h"

#include "CmErrMsg.h" // for WinMsg
#include "outstand.h"

using namespace CmErrMsg; // for WinMsg

/// <summary>constructor</summary>
/// <param name="pCaIss">pointer of G4_CaIss's instance</param>
/// <param name="lapbIf">pointer pointer of lapbIf</param>
/// <param name="num">I/F number</param>
G4_IsLapbIf::G4_IsLapbIf( G4_CaIss *pCaIss, 
			  BusMasterIf<BUS_WIDTH_VCI> **lapbIf,
			  unsigned int num ):
  mpCaIss( pCaIss ), 
  G4_IsIfBase()
{
  mpLApbIf[0] = *lapbIf;

  for( int i=0; i<APB_MAX_DATA; i++ ){
    mDataDbg[i] = 0;
  }
  mDbgTrans.set_extension( &mDbgG3mExt );
  mDbgTrans.set_extension( &mDbgVciExt );
}


/// <summary>destructor</summary>
G4_IsLapbIf::~G4_IsLapbIf(void)
{
  mDbgTrans.clear_extension(&mDbgG3mExt);
  mDbgTrans.clear_extension(&mDbgVciExt);
}


/// <summary>notify the timing of transaction sending</summary>
/// <param name="trans">transaction</param>
/// <param name="t">transport time</param>
/// <returns>none</returns>
void G4_IsLapbIf::notifyTransSend( TlmTransactionType &trans, BusTime_t &t )
{
#ifdef BUSIF_TEST_LOG
  printf("\n");
  printf("G4_IsLapbIf::notifyTransSend: time=%s\n", t.to_string().c_str());
  fflush(stdout);
#endif
}


/// <summary>notify the timing of transaction completed</summary>
/// <param name="trans">transaction</param>
/// <param name="t">transport time</param>
/// <returns>none</returns>
void G4_IsLapbIf::notifyTransComp( TlmTransactionType &trans, BusTime_t &t )
{
#ifdef BUSIF_TEST_LOG
  printf("\n");
  printf("G4_IsLapbIf::notifyTransComp: time=%s\n", t.to_string().c_str());
  fflush(stdout);
#endif
  std::list<trans_info*>::iterator it = mTransInfoQueue.begin();
  while( it != mTransInfoQueue.end() ){
    if((*it)->trans == &trans){
      if((*it)->acc_mode == G4_IsIfBase::ACC_WRITE){
	BusTime_t l_time = sc_time_stamp() - (*it)->req_time + t;
#ifdef BUSIF_TEST_LOG
	printf("req time = %s, current time = %s\n", (*it)->req_time.to_string().c_str(), sc_time_stamp().to_string().c_str());
	printf("Call NfyTransComp(write): sideband(peid(0x%x),spid(0x%x),um(%d),bitpos(0x%0x),bitop(0x%x)), addr=0x%08x, ", 
	       (*it)->sideband.GetBMID(), 
	       (*it)->sideband.GetSPID(), 
	       (*it)->sideband.GetUM(), 
	       (*it)->sideband.GetBitPos(), 
	       (*it)->sideband.GetBitOp(), 
	       (*it)->addr);
	printf("%s, write)\n", l_time.to_string().c_str());
	fflush(stdout);
#endif
	mpCaIss->NfyTransComp((*it)->sideband, (*it)->addr, l_time, 0, (unsigned int)((*it)->acc_mode));
      }else if((*it)->acc_mode == G4_IsIfBase::ACC_READ){
	BusTime_t l_time = sc_time_stamp() - (*it)->req_time + t;
#ifdef BUSIF_TEST_LOG
	printf("req time = %s, current time = %s\n", (*it)->req_time.to_string().c_str(), sc_time_stamp().to_string().c_str());
	printf("Call NfyTransComp(read) : sideband(peid(0x%x),spid(0x%x),um(%d),bitpos(0x%0x),bitop(0x%x)), addr=0x%08x, ", 
	       (*it)->sideband.GetBMID(), 
	       (*it)->sideband.GetSPID(), 
	       (*it)->sideband.GetUM(), 
	       (*it)->sideband.GetBitPos(), 
	       (*it)->sideband.GetBitOp(), 
	       (*it)->addr);
	printf("%s, read)\n", l_time.to_string().c_str());
	fflush(stdout);
#endif
	mpCaIss->NfyTransComp((*it)->sideband, (*it)->addr, l_time, getTransDataVal(trans), (unsigned int)((*it)->acc_mode));
      }else if((*it)->acc_mode == G4_IsIfBase::ACC_FETCH){
	BusTime_t l_time = sc_time_stamp() - (*it)->req_time + t;
#ifdef BUSIF_TEST_LOG
	printf("req time = %s, current time = %s\n", (*it)->req_time.to_string().c_str(), sc_time_stamp().to_string().c_str());
	printf("Call NfyTransComp(fetch) : sideband(peid(0x%x),spid(0x%x),um(%d),bitpos(0x%0x),bitop(0x%x)), addr=0x%08x, ", 
	       (*it)->sideband.GetBMID(), 
	       (*it)->sideband.GetSPID(), 
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
void G4_IsLapbIf::CyclicHandler (void)
{
}


/// <summary>request the write transaction</summary>
/// <param name="sideband">sideband</param>
/// <param name="addr">address</param>
/// <param name="p_data">data pointer</param>
/// <param name="size">data length</param>
/// <returns>SYSERR cause</returns>
SysErrCause G4_IsLapbIf::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* p_data, ByteSize size)
{
  unsigned char peid      = getBmId(sideband);

#ifdef BUSIF_TEST_LOG
  printf("\n");
  printf("%s:G4_IsLapbIf::TargetWrite(Normal_S): ", sc_time_stamp().to_string().c_str());
  printf("sideband(peid(0x%x),tcid(0x%x),vcid(0x%x),spid(0x%x),vm(%d),um(%d),bitpos(0x%0x),bitop(0x%x)), ", 
	 sideband.GetBMID(), 
	 0, 
	 0, 
	 sideband.GetSPID(), 
	 0, 
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
  unsigned char   *data          = new unsigned char[APB_MAX_DATA];
  TlmBasicPayload *basic_payload = new TlmBasicPayload;
  TlmG3mExtension *g3m_ext       = new TlmG3mExtension;
  TlmVciExtension *vci_ext       = new TlmVciExtension;
  trans_info      *t_info        = new trans_info;

  basic_payload->set_extension( g3m_ext );
  basic_payload->set_extension( vci_ext );

  for(int i=0; i<size; i++){
    *(data+i) = *(p_data+i);
  }

  setTransBasic( addr, size, data, *basic_payload );
  setTransG3m( getGpid(sideband), 
	       0, 
	       peid,
	       getSpId(sideband),
	       getUserMode(sideband), 
	       getGm(sideband),
	       getSpcLd(sideband),
               *basic_payload, 
	       *g3m_ext );
  setTransVci( getReqType(sideband, TlmVciExtension::CMD_WRITE), // request type
               *basic_payload,             // Basic Payload
	       *vci_ext );                 // Vci Extension

  if(mTransId != 0xffffffff) mTransId++;
  else                       mTransId = 1;

  t = SC_ZERO_TIME;
  t_info->sideband   = sideband;
  t_info->addr       = addr;
  t_info->size       = size;
  t_info->acc_mode   = G4_IsIfBase::ACC_WRITE;
  t_info->trans      = basic_payload;
  t_info->req_time   = sc_time_stamp();

  mTransInfoQueue.push_back(t_info);

  isNotFull = mpLApbIf[0]->write( *basic_payload,   // payload pointer
				  false,            // debug?
				  t,                // local time
				  false );          // with wait?

#ifdef BUSIF_TEST_LOG
  printf("%s:G4_IsLapbIf::TargetWrite(Normal_E): Under operating transaction num=%d\n", sc_time_stamp().to_string().c_str(), mTransInfoQueue.size());
  printf("     ");
  for (int i=0; i<size; i++){
    printf("data%d=0x%08x, ", i, data[i]);
  }
  printf("\n\n");
  fflush(stdout);
  printf("G4_IsLapbIf::TargetWrite(Normal_E): VCI Exp Payload: requset type=%d\n", (unsigned int)vci_ext->getRequestType());
  fflush(stdout);
#endif

  return(SYSERR_NONE);
}


/// <summary>request the read transaction</summary>
/// <param name="sideband">sideband</param>
/// <param name="addr">address</param>
/// <param name="p_data">data pointer</param>
/// <param name="size">data length</param>
// <returns>SYSERR cause</returns>
SysErrCause G4_IsLapbIf::TargetRead  (SideBand sideband, PhysAddr addr, uint8_t* p_data, ByteSize size)
{
  unsigned char peid      = getBmId(sideband);

#ifdef BUSIF_TEST_LOG
  printf("\n");
  printf("%s:G4_IsLapbIf::TargetRead(Debug_S): ", sc_time_stamp().to_string().c_str());
  printf("sideband(peid(0x%x),tcid(0x%x),vcid(0x%x),spid(0x%x),vm(%d),um(%d),bitpos(0x%0x),bitop(0x%x)), ", 
	 sideband.GetBMID(), 
	 0, 
	 0, 
	 sideband.GetSPID(), 
	 0, 
	 sideband.GetUM(), 
	 sideband.GetBitPos(), 
	 sideband.GetBitOp());
  printf("address=0x%08x, size=%x\n", addr, size);
  fflush(stdout);
#endif

  // debug access ( to get the value for trace data)
  setTransBasic( addr, size, p_data, mDbgTrans );
  setTransG3m( getGpid(sideband), 
	       0, 
	       peid,
	       getSpId(sideband),
	       getUserMode(sideband), 
	       getGm(sideband), 
	       getSpcLd(sideband),
               mDbgTrans,
	       mDbgG3mExt );
  setTransVci( getReqType(sideband, TlmVciExtension::CMD_READ), // request type
               mDbgTrans,                  // Basic Payload
	       mDbgVciExt );               // Vci Extension
  mpLApbIf[0]->read( mDbgTrans,       // payload pointer
		     true );          // debug?
  
#ifdef BUSIF_TEST_LOG
  printf("G4_IsLapbIf::TargetRead(Debug_E): ");
  for (int i=0; i<size; i++){
    printf("data%d=0x%08x, ", i, *(p_data+i));
  }
  printf("\n");
  printf("%s:G4_IsLapbIf::TargetRead(Normal_S): ", sc_time_stamp().to_string().c_str());
  printf("sideband(peid(0x%x),tcid(0x%x),vcid(0x%x),spid(0x%x),vm(%d),um(%d),bitpos(0x%0x),bitop(0x%x)), ", 
	 sideband.GetBMID(), 
	 0, 
	 0, 
	 sideband.GetSPID(), 
	 0, 
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
  unsigned char   *data          = new unsigned char[APB_MAX_DATA];
  TlmBasicPayload *basic_payload = new TlmBasicPayload;
  TlmG3mExtension *g3m_ext       = new TlmG3mExtension;
  TlmVciExtension *vci_ext       = new TlmVciExtension;
  trans_info      *t_info        = new trans_info;

  basic_payload->set_extension( g3m_ext );
  basic_payload->set_extension( vci_ext );

  for(int i=0; i<size; i++){
    *(data+i) = 0;
  }

  setTransBasic( addr, size, data, *basic_payload );
  setTransG3m( getGpid(sideband), 
	       0, 
	       peid,
	       getSpId(sideband),
	       getUserMode(sideband), 
	       getGm(sideband), 
	       getSpcLd(sideband),
               *basic_payload,
	       *g3m_ext );
  setTransVci( getReqType(sideband, TlmVciExtension::CMD_READ), // request type
               *basic_payload,              // Basic Payload
	       *vci_ext );                  // Vci Extension

  if(mTransId != 0xffffffff) mTransId++;
  else                       mTransId = 1;

  t = SC_ZERO_TIME;
  t_info->sideband   = sideband;
  t_info->addr       = addr;
  t_info->size       = size;
  t_info->acc_mode   = G4_IsIfBase::ACC_READ;
  t_info->trans      = basic_payload;
  t_info->req_time   = sc_time_stamp();

  mTransInfoQueue.push_back(t_info);

  isNotFull = mpLApbIf[0]->read( *basic_payload,   // payload pointer
				 false,            // debug?
				 t,                // local time
				 false );          // with wait?

#ifdef BUSIF_TEST_LOG
  printf("%s:G4_IsLapbIf::TargetRead(Normal_E): Under operating transaction num=%d\n", sc_time_stamp().to_string().c_str(), mTransInfoQueue.size());
  printf("\n");
  fflush(stdout);
  printf("G4_IsLapbIf::TargetRead(Normal_E): VCI Exp Payload: requset type=%d\n", (unsigned int)vci_ext->getRequestType());
  fflush(stdout);
#endif

  return(SYSERR_NONE);
}


/// <summary>request the fetch transaction</summary>
/// <param name="sideband">sideband</param>
/// <param name="addr">address</param>
/// <param name="p_data">data pointer</param>
/// <param name="size">data length</param>
/// <returns>SYSERR cause</returns>
SysErrCause G4_IsLapbIf::TargetFetch  (SideBand sideband, PhysAddr addr, uint8_t* p_data, ByteSize size)
{  
  unsigned char peid = getBmId(sideband);
  for(int i=0; i<size; i++){
    *(p_data+i) = 0;
  }
  return(SYSERR_NONE);
}


/// <summary>request the debug write transaction</summary>
/// <param name="addr">address</param>
/// <param name="p_data">data pointer</param>
/// <param name="size">data length</param>
/// <returns>SYSERR cause</returns>
SysErrCause G4_IsLapbIf::TargetWriteDebug (PhysAddr addr, uint8_t* p_data, ByteSize size)
{
  setTransBasic( addr, size, p_data, mDbgTrans );
  setTransG3m( 0, 0, DEBUG_MASTER_BMID, 0, 0, 0, 0, mDbgTrans, mDbgG3mExt );
  setTransVci( TlmVciExtension::CMD_WRITE, mDbgTrans, mDbgVciExt );
  mpLApbIf[0]->write( mDbgTrans, true );

  return(SYSERR_NONE);
}


/// <summary>request the debug read transaction</summary>
/// <param name="addr">address</param>
/// <param name="p_data">data pointer</param>
/// <param name="size">data length</param>
/// <returns>SYSERR cause</returns>
SysErrCause G4_IsLapbIf::TargetReadDebug  (PhysAddr addr, uint8_t* p_data, ByteSize size)
{
  setTransBasic( addr, size, p_data, mDbgTrans );
  setTransG3m( 0, 0, DEBUG_MASTER_BMID, 0, 0, 0, 0, mDbgTrans, mDbgG3mExt );
  setTransVci( TlmVciExtension::CMD_READ, mDbgTrans, mDbgVciExt );
  mpLApbIf[0]->read( mDbgTrans, true );

  return(SYSERR_NONE);
}


/// <summary>reset operation</summary>
/// <returns>none</returns>
void G4_IsLapbIf::HardReset (void)
{
#ifdef TEST_RESET_DBG
  printf("%s G4_IsLapbIf::HardReset\n", sc_time_stamp().to_string().c_str()); 
  fflush(stdout);
#endif
  for( int j=0; j<APB_MAX_DATA; j++ ){
    mDataDbg[j] = 0;
  }
}

/// <summary>delete transaction instance</summary>
/// <param name="trans">transaction</param>
/// <returns>none</returns>
void G4_IsLapbIf::DeleteTrans ( TlmTransactionType &trans )
{
  unsigned char* data_ptr  = trans.get_data_ptr();
  delete [] data_ptr;

  TlmG3mExtension *g3m_ext = (TlmG3mExtension *)0;
  trans.get_extension( g3m_ext);
  trans.release_extension( g3m_ext );

  TlmVciExtension *vci_ext = (TlmVciExtension *)0;
  trans.get_extension( vci_ext);
  trans.release_extension( vci_ext );

  delete &trans;
}

/// <summary>get vci transaction request type </summary>
/// <param name="sideband">sideband</param>
/// <returns>vci transaction requset type</returns>
TlmVciExtension::VciRequest_t G4_IsLapbIf::getReqType ( SideBand sideband, TlmVciExtension::VciRequest_t def_req )
{
  unsigned int bit_op = getBitOp(sideband);
  if((bit_op == SideBand::BITOP_SET) || (bit_op == SideBand::BITOP_CLR)){
    return TlmVciExtension::CMD_BITOP_SET;
  }else if((bit_op == SideBand::BITOP_NOT)){
    return TlmVciExtension::CMD_BITOP_XOR;
  }else if((bit_op == SideBand::BITOP_CAXI)){
    return TlmVciExtension::CMD_CAXI;
  }else{
    return def_req;
  }
}
