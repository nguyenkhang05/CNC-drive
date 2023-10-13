/*************************************************************************
 *
 * $Id: G4_Intc2Func.cpp,v 1.11 2020/03/24 10:02:04 hadinhtran Exp $
 *
 * Copyright(c) 2016-2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

#include <algorithm>
#include "G4_intc_typ.h"
#include "G4_intc_def.h"
#include "sideband.h"
#include "G4_Intc2Func.h"
#include "G4_Intc2FuncUsr.h"
#include "NSMVINTC2V02.h"
#include "BusSlaveFuncIf.h"
#include "OSCI2.h"

// for debug output
#ifdef DEBUG_MESSAGE_OUTPUT
# include <fstream>
  extern std::ofstream difout;
#endif // DEBUG_MESSAGE_OUTPUT

/// <summary>constructor</summary>
/// <param name="pPeidInfo">pointer to PeidInfo vector</param>
/// <param name="baseAddr">base address</param>
/// <param name="pIntc2">pointer to INTC2 module</param>
/// <returns>none</returns>
G4_Intc2Func::G4_Intc2Func( std::vector<PeidInfo*>* pPeidInfo,
                      unsigned int            baseAddr,
                      NSMVINTC2V02*           pIntc2,
                      Intc2ImplType           intc2_type ):
  CIntc2( pPeidInfo, (FsBus*)0, intc2_type ),
  BusSlaveFuncIf(),
  mpIntc2( pIntc2 ),
  mpPeidInfo( pPeidInfo ),
  mEicWriteFlag( false ),
  mEicUpdateFlag( false ),
  mEicPeid( 0 ),
  mEicTcid( 0 ),
  mEicUpdateCh( 0 ),
  mEeicWriteFlag( false ),
  mEeicUpdateFlag( false ),
  mEeicPeid( 0 ),
  mEeicTcid( 0 ),
  mEeicUpdateCh( 0 ),
  mEibdUpdateFlag(0),
  mEibgUpdateFlag(0)
{
  assert(pIntc2 != NULL);
  dmsgsc( mpIntc2->name() << ".G4_Intc2Func.G4_Intc2Func()" );
  mpIntc2FuncUsr = new G4_Intc2FuncUsr;
  mpIntc2FuncUsr->setWrapperPtr( pIntc2 );
}

/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
G4_Intc2Func::~G4_Intc2Func( void )
{
  dmsgsc( "G4_Intc2Func.~G4_Intc2Func()" );
  delete mpIntc2FuncUsr;
}

///<summary>function to reset</summary>
///<param name="">none</param>
///<returns>none</returns>
void
G4_Intc2Func::reset( void )
{
  dmsgsc( mpIntc2->name() << ".Intc1Func.reset()" );
  mEicWriteFlag  = false;
  mEicUpdateFlag = false;
  mEicPeid       = 0;
  mEicTcid       = 0;
  mEicUpdateCh   = 0;
  mEeicWriteFlag  = false;
  mEeicUpdateFlag = false;
  mEeicPeid       = 0;
  mEeicTcid       = 0;
  mEeicUpdateCh   = 0;
  mEibdUpdateFlag = 0;
  mEibgUpdateFlag = 0;
  HardReset();
}


///<summary>TLM read function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void
G4_Intc2Func::read (
  unsigned int     offsetAddress, 
  TlmBasicPayload& trans, 
  BusTime_t*       t,
  bool             debug )
{
  assert(t != NULL);
  dmsgsc( mpIntc2->name() << ".G4_Intc2Func.read()" );
  mpIntc2FuncUsr->read_precallout( trans, t, debug );

  unsigned char* pData = trans.get_data_ptr();
  unsigned int   size  = trans.get_data_length();
  assert(pData != NULL);
  for ( unsigned int i = 0; i < size; i++ ) {
    pData[i] = 0;
  }

  // no need to check the range of the address
  // because  CForest uses dummy register object
  // when the address is out of range.
  if ( debug ) {
    dmsgsc( "  TargetReadDebug()" );
    if ( (4 == size) &&
         (G4IntcTyp::INTC2_EIC_OFFSET <= offsetAddress) &&
         ((G4IntcTyp::INTC2_EIC_OFFSET+0xFBE) >= offsetAddress) ) {
      TargetReadDebug( offsetAddress,   pData,   2 );
      TargetReadDebug( offsetAddress+2, pData+2, 2 );
    }
    else {
      TargetReadDebug( offsetAddress, pData, size );
    }
  }
  else {
    SideBand         sb;
    TlmG3mExtension* pG3mExtension;

    trans.get_extension( pG3mExtension );
    sb.SetBMID(pG3mExtension->getPeId());
    sb.SetSPID(pG3mExtension->getSpId());
    sb.SetUM(pG3mExtension->isUserMode());
    sb.SetGPID(pG3mExtension->getVmId());
    sb.SetGM((unsigned int)(pG3mExtension->isVirtualMode()));
    TargetRead( sb, offsetAddress, pData, size );
  }

  dmsgsc( "  address:0x" << trans.get_address() );
  for ( unsigned int i = 0; i < size; i++ ) {
    dmsgsc( "  data:0x" << (int)pData[i] );
  }

  mpIntc2FuncUsr->read_postcallout( trans, t, debug );
}

///<summary>TLM write function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void
G4_Intc2Func::write(
  unsigned int     offsetAddress, 
  TlmBasicPayload& trans, 
  BusTime_t*       t,
  bool             debug )
{
  assert(t != NULL);
  dmsgsc( mpIntc2->name() << ".G4_Intc2Func.write()" );
  mpIntc2FuncUsr->write_precallout( trans, t, debug );

  unsigned char* pData = trans.get_data_ptr();
  unsigned int   size  = trans.get_data_length();
  unsigned int   pre_GM_bit = 0;
  unsigned int   pre_GPID_bit = 0;
  unsigned int   pre_BGPR_bit = 0;
  unsigned int   channel = 0;
  unsigned int   peid = 0;
  bool           Eibd_write_flag = false;
  bool           Eibg_write_flag = false;
  assert(pData != NULL);
  
  dmsgsc("  size:0x" << size);
  for ( unsigned int i = 0; i < size; i++ ) {
    dmsgsc( "  data:0x" << (int)pData[i] );
  }

  if ( (G4IntcTyp::INTC2_EIC_OFFSET <= offsetAddress) &&
       ((G4IntcTyp::INTC2_EIC_OFFSET+0xFBE) >= offsetAddress) ) {
    mEicWriteFlag = true;
  }

  if ( (G4IntcTyp::INTC2_EEIC_OFFSET <= offsetAddress) &&
       ((G4IntcTyp::INTC2_EEIC_OFFSET+0x1F7F) >= offsetAddress) ) {
    mEeicWriteFlag = true;
  }

  if ( (G4IntcTyp::INTC2_EIBD_OFFSET <= offsetAddress) &&
       ((G4IntcTyp::INTC2_EIBD_OFFSET+0x1F7F) >= offsetAddress) ) {
    Eibd_write_flag = true;
    channel = (offsetAddress - G4IntcTyp::INTC2_EIBD_OFFSET) / 4;
    TRegEIBD*    pEibd  = GetEIBDPtr( channel + INTC1_CH_NUM);
    pre_GM_bit = pEibd->GetGM();
    pre_GPID_bit = pEibd->GetGPID();
  }

  if ( (G4IntcTyp::INTC2_I2EIBG_OFFSET <= offsetAddress) &&
       ((G4IntcTyp::INTC2_I2EIBG_OFFSET+0x1F) >= offsetAddress) ) {
    Eibg_write_flag = true;
    peid = (offsetAddress - G4IntcTyp::INTC2_I2EIBG_OFFSET) / 4;
    TRegEIBG*    pEibg  = GetEIBGPtr( peid);
    pre_BGPR_bit = pEibg->GetBGPR();
  }

  if ( (G4IntcTyp::INTC2_EIBD_OFFSET <= offsetAddress) &&
       ((G4IntcTyp::INTC2_EIBD_OFFSET+0x1F7C) >= offsetAddress) ) {
    if ( mpIntc2->eiint_type[(offsetAddress-G4IntcTyp::INTC2_EIBD_OFFSET)/4]->read() ) {
      // when the detection type is high level, EIBDn.CST cannot be set 1.
      if ( (1 == size) && (3 == (offsetAddress % 4)) ) {
         dmsgsc("  *pData:0x" << (int)*pData);
         *pData &= 0x7F;
         dmsgsc("  *pData:0x" << (int)*pData);
      }
      else if ( (2 == size) && (2 == (offsetAddress % 4)) ) {
         dmsgsc("  pData[1]:0x" << (int)*pData);
         pData[1] &= 0x7F;
         dmsgsc("  pData[1]:0x" << (int)*pData);
      }
      else if ( (4 == size) && (0 == (offsetAddress % 4)) ) {
         dmsgsc("  pData[3]:0x" << (int)*pData);
         pData[3] &= 0x7F;
         dmsgsc("  pData[3]:0x" << (int)*pData);
      }
    }
  }

  // no need to check the range of the address
  // because  CForest uses dummy register object
  // when the address is out of range.
  if ( debug ) {
    dmsgsc( "  TargetWriteDebug()" );
    if ( (4 == size) &&
         (G4IntcTyp::INTC2_EIC_OFFSET <= offsetAddress) &&
         ((G4IntcTyp::INTC2_EIC_OFFSET+0xFBE) >= offsetAddress) ) {
      TargetWriteDebug( offsetAddress,   pData,   2 );
      TargetWriteDebug( offsetAddress+2, pData+2, 2 );
    }
    else {
      TargetWriteDebug( offsetAddress, pData, size );
    }
  }
  else {
    SideBand         sb;
    TlmG3mExtension* pG3mExtension;

    trans.get_extension( pG3mExtension );
    sb.SetBMID(pG3mExtension->getPeId());
    sb.SetSPID(pG3mExtension->getSpId());
    sb.SetUM(pG3mExtension->isUserMode());
    sb.SetGPID(pG3mExtension->getVmId());
    sb.SetGM((unsigned int)(pG3mExtension->isVirtualMode()));

    if ( (G4IntcTyp::INTC2_EIC_OFFSET <= offsetAddress) &&
         ((G4IntcTyp::INTC2_EIC_OFFSET+0xFBE) >= offsetAddress) ) {
      dmsgsc("  eiint_type[0x" << (offsetAddress-G4IntcTyp::INTC2_EIC_OFFSET)/2
              << "]: 0x" << mpIntc2->eiint_type[(offsetAddress-G4IntcTyp::INTC2_EIC_OFFSET)/2]->read());
      if ( mpIntc2->eiint_type[(offsetAddress-G4IntcTyp::INTC2_EIC_OFFSET)/2]->read() ) {
        // when the detection type is high level, EICn.EIRF cannot be set 1.
        if ( (1 == size) && (1 == (offsetAddress % 2)) ) {
           // ignore
        }
        else if ( (2 == size) && (0 == (offsetAddress % 2)) ) {
          TargetWrite( sb, offsetAddress, pData, 1 );
        }
        else if ( (4 == size) && (0 == (offsetAddress % 4)) ) {
          TargetWrite( sb, offsetAddress, pData, 1 );
          TargetWrite( sb, offsetAddress+2, pData+2, 1 );
        }
        else {
          TargetWrite( sb, offsetAddress, pData, size );
        }
      }
      else {
        TargetWrite( sb, offsetAddress, pData, size );
      }
    }
    else {
      TargetWrite( sb, offsetAddress, pData, size );
    }
  }

  dmsgsc( "  address:0x" << trans.get_address() );
  for ( unsigned int i = 0; i < size; i++ ) {
    dmsgsc( "  data:0x" << (int)pData[i] );
  }

  mEicWriteFlag = false; // set 'false' before calling 'UpdateHighPriorityOfIntc1()' function
  if ( mEicUpdateFlag ) { // EIC register has been updated
    UpdateHighPriorityOfIntc1( mEicPeid, mEicUpdateCh );
    mEicUpdateFlag = false;
  }

  mEeicWriteFlag = false; // set 'false' before calling 'UpdateHighPriorityOfIntc1()' function
  if ( mEeicUpdateFlag ) { // EIC register has been updated
    UpdateHighPriorityOfIntc1( mEeicPeid, mEeicUpdateCh );
    mEeicUpdateFlag = false;
  }

  if ( Eibd_write_flag ) { // EIBD reg is written
    TRegEIBD*    pEibd  = GetEIBDPtr( channel + INTC1_CH_NUM ); // updated by Yoshinaga
    if ( (pEibd->GetGM() != pre_GM_bit) || (pEibd->GetGPID() != pre_GPID_bit) ){
      mEibdUpdateFlag = true;
      UpdateHighPriorityOfIntc1( pEibd->GetPEID(), channel + INTC1_CH_NUM ); // updated by Yoshinaga
    }else{
      mEibdUpdateFlag = false;
    }
  }

  if ( Eibg_write_flag ) { // EIBG reg is written
    TRegEIBG*    pEibg  = GetEIBGPtr( peid );
    if (pEibg->GetBGPR() != pre_BGPR_bit){
      mEibgUpdateFlag = true;
      G4IntcTyp::EiintReq     eiint( mpIntc2->i2_g_eiint[peid]->read() );
      UpdateHighPriorityOfIntc1( peid, eiint.bit_val.ch );
    }else{
      mEibgUpdateFlag = false;
    }
  }

  mpIntc2FuncUsr->write_postcallout( trans, t, debug );
}


// functions
///<summary>function to get size of queue in INTC2 module</summary>
///<param name="">none</param>
///<returns>queue size</returns>
unsigned int
G4_Intc2Func::getQueueSize( void )
{
  dmsgsc( mpIntc2->name() << ".G4_Intc2Func.getQueueSize() size:0x" << GetQueueSize() );
  return GetQueueSize();
}


// override functions
///<param name="">none</param>
///<returns>none</returns>
void
G4_Intc2Func::StartCyclicHandler( void )
{
  dmsgsc( mpIntc2->name() << ".G4_Intc2Func.StartCyclicHandler()" );
  mpIntc2->notifyExecEvent();
}

///<summary>function to request EIINT</summary>
///<param name="peid">PEID</param>
///<param name="channel">number of channel</param>
///<param name="reqinfo">pointer to IntReqInfoBase(declared in CForest INTC2)</param>
///<returns>none</returns>
void
G4_Intc2Func::ReqIntToIntc1( uint32_t peid, uint32_t channel, IntReqInfoBase* reqinfo )
{
  dmsgsc( mpIntc2->name() << ".G4_Intc2Func.ReqIntToIntc1()" );
  G4IntcTyp::EiintReq     eiint;
  G4IntcTyp::EiintReqBind bind;
  
  assert(reqinfo != NULL);
  bool cpu_gm = mpIntc2->getCPUGM(peid);            // GM of CPU
  unsigned int cpu_gpid = mpIntc2->getCPUGPID(peid);// GPID of CPU
  unsigned int bit_gm = reqinfo->GetGM();           // GM bit value
  unsigned int bit_gpid = reqinfo->GetGPID();       // GPID bit value
  unsigned int bit_bgpr = reqinfo->GetBGPR();       // BGPR bit value
  unsigned int priority = static_cast<unsigned int>(reqinfo->GetPriority());
  bool has_request = false;

  // CPU GUEST MODE
  if (cpu_gm){
    // Bind to Guest
    if (bit_gm == 1){
      if (cpu_gpid == bit_gpid){
        // send GMEIINT
        eiint.bit_val.gm   = 1;
        has_request = true;
      }else{
        if (priority < bit_bgpr){
          // send BGEIINT
          eiint.bit_val.bg   = 1;
          eiint.bit_val.bgpr = bit_bgpr;
          has_request = true;
        }else{
          // not request any interrupt
        }
      }
    // Bind to Host
    }else{
      // send EIINT
      has_request = true;
    }
  // CPU HOST MODE
  }else{
    // Bind to Guest
    if (bit_gm == 1){
      // not request any interrupt
    // Bind to Host
    }else{
      // send EIINT
      has_request = true;
    }
  }

  if (has_request){
    eiint.bit_val.req  = true;
    eiint.bit_val.ch   = channel;
    eiint.bit_val.pri  = priority;
    eiint.bit_val.tb   = reqinfo->GetEITB();
    bind.bit_val.peid  = reqinfo->GetPEID();
    eiint.bit_val.gpid = bit_gpid;
    dmsgsc("  channel:0x" << channel << " pri:0x" << eiint.bit_val.pri
           << " tb:" << eiint.bit_val.tb << " PEID:0x" << bind.bit_val.peid 
           << " gm:" << bit_gm << " gpid:0x" << bit_gpid << " bg:" << eiint.bit_val.bg << " bgpr:" << bit_bgpr );

    mpIntc2->setEiintReqPort( peid, eiint.val, G4IntcTyp::PRI_EXEC_PROC );

    mpIntc2FuncUsr->ReqEiintToIntc1_callout( peid, channel );
  }
}

///<summary>function to cancel EIINT</summary>
///<param name="peid">PEID</param>
///<param name="channel">number of channel</param>
///<returns>none</returns>
void
G4_Intc2Func::CancelIntToIntc1( uint32_t peid, uint32_t channel )
{
  dmsgsc( mpIntc2->name() << ".G4_Intc2Func.CancelIntToIntc1()" );
  G4IntcTyp::EiintReq eiint;
  TRegEIBD*         pEibd = GetEIBDPtr( channel );

  eiint.bit_val.ch = channel;
  mpIntc2->setEiintReqPort( peid, eiint.val, G4IntcTyp::PRI_CANCEL_PROC );

  std::set<unsigned int>::iterator it;
  it = std::find( mpIntc2->mReEntryReqList.begin(), mpIntc2->mReEntryReqList.end(), channel );
  if ( it != mpIntc2->mReEntryReqList.end()  ) {
     mpIntc2->mReEntryReqList.erase( it );
  }

  mpIntc2FuncUsr->CancelEiintToIntc1_callout( peid, channel );
  if (mpIntc2->resendEiint2Forbid == true) {
     mpIntc2->resendEiint2Forbid = false;
  } else {
     mpIntc2->notifyResendEiintReqEvent();
  }
  
}

///<summary>function to cancel EIINT</summary>
///<param name="channel">number of channel</param>
///<returns>none</returns>
void
G4_Intc2Func::CancelIntByMask( uint32_t channel )
{
  dmsgsc( mpIntc2->name() << ".G4_Intc2Func.CancelIntByMask()" );

  CancelIntToIntc1( 0, channel ); // peid is not used
}

///<summary>function to update priority of queue</summary>
///<param name="peid">peid</param>
///<returns>none</returns>
void
G4_Intc2Func::UpdateHighPriority( uint32_t peid )
{
  dmsgsc( mpIntc2->name() << ".G4_Intc2Func.UpdateHighPriority(), pe: " << peid );
  if ( !IsValidPeId(peid) ) {
    return;
  }
  else {
    CIntc2::UpdateHighPriority( peid );
    uint32_t channel = GetHighPriorityChannel(peid);
    TRegEIC* pEic    = GetEICPtr( channel );
    dmsgsc("  peid:0x" << peid << " channel:0x" << channel << " EIMK:" << pEic->GetEIMK() << " EIRF:" << pEic->GetEIRF());
    unsigned int NO_REQ_CHANNEL = 0xFFFFFFFF; // same with NO_REQ_CHANNEL in cforest/Device/Peripheral/INTC2/include/intc2.h
    if (channel == NO_REQ_CHANNEL){ 
      // Do nothing because there is no pending channel to sort priority
    }else{
      if ( (0 == pEic->GetEIMK()) && (1 == pEic->GetEIRF()) ) {
        // to drive the INTC2 when EIMK is unset.
        // (there are some requests in the queue but INTC2 has not requested yet.)
        mpIntc2->notifyExecEvent();
      }
    }
  }
}

///<summary>function to update priority of EIINT to INTC1</summary>
///<param name="peid">EIBDn.PEID</param>
///<param name="et">EIBDn.ET</param>
///<param name="tcid">thread context id</param>
///<returns>none</returns>
void
G4_Intc2Func::UpdateHighPriorityOfIntc1( uint32_t peid, uint32_t channel )
{
  dmsgsc( mpIntc2->name() << ".G4_Intc2Func.UpdateHighPriorityOfIntc1(), mEicWriteFlag: "<< mEicWriteFlag << ", mEeicWriteFlag: "<< mEeicWriteFlag );

  if ( mEicWriteFlag ) {
    mEicPeid       = peid;
    mEicUpdateCh   = channel;
    mEicUpdateFlag = true;
  }else if ( mEeicWriteFlag ) {
    mEeicPeid       = peid;
    mEeicUpdateCh   = channel;
    mEeicUpdateFlag = true;
  }else {
    bool      w_flag = false;
    TRegEEIC*  pEeic   = GetEEICPtr ( channel );
    TRegEIBD* pEibd  = GetEIBDPtr( channel );
    TRegEIBG* pEibg  = GetEIBGPtr(peid);
    G4IntcTyp::EiintReq     eiint( mpIntc2->i2_g_eiint[peid]->read() );
    G4IntcTyp::EiintReqBind bind ( eiint.bit_val.bind );

    if ( 1 == pEeic->GetEIMK() ) {
      eiint.val        = 0;
      eiint.bit_val.ch = channel;
      w_flag           = true;
    }
    else if ( ((eiint.bit_val.pri > pEeic->GetEIP()) && (1 == eiint.bit_val.req) && (pEeic->GetEIRF() == 1)) ||
             ((0 == eiint.bit_val.req) && (pEeic->GetEIRF() == 1)) || //If previous req is 0 and current req is 1
             ((1 == eiint.bit_val.req) && (eiint.bit_val.ch == channel) && (pEeic->GetEIRF() == 1) && ((eiint.bit_val.pri != pEeic->GetEIP()) || mEibdUpdateFlag || mEibgUpdateFlag)) ){ // change priority, GM, GPID, EIBG bit
      bool cpu_gm = mpIntc2->getCPUGM(peid);            // GM of CPU
      unsigned int cpu_gpid = mpIntc2->getCPUGPID(peid);// GPID of CPU
      unsigned int bit_gm = pEibd->GetGM();           // GM bit value
      unsigned int bit_gpid = pEibd->GetGPID();       // GPID bit value
      unsigned int bit_bgpr = pEibg->GetBGPR();       // BGPR bit value
      unsigned int priority = pEeic->GetEIP();
      bool has_request = false;

      // CPU GUEST MODE
      if (cpu_gm){
        // Bind to Guest
        if (bit_gm == 1){
          if (cpu_gpid == bit_gpid){
            // send GMEIINT
            eiint.bit_val.gm   = 1;
            has_request = true;
          }else{
            if (priority < bit_bgpr){
              // send BGEIINT
              eiint.bit_val.bg   = 1;
              eiint.bit_val.bgpr = bit_bgpr;
              has_request = true;
            }else{
              if (eiint.bit_val.req == 1){
                eiint.bit_val.req = 0;
                eiint.bit_val.ch = channel;
                w_flag = 1;
              }
            }
          }
        // Bind to Host
        }else{
          // send EIINT
          has_request = true;
        }

      // CPU HOST MODE
      }else{
        // Bind to Guest
        if (bit_gm == 1){
          if (eiint.bit_val.req == 1){
            eiint.bit_val.req = 0;
            eiint.bit_val.ch = channel;
            w_flag = 1;
          }
        // Bind to Host
        }else{
          // send EIINT
          has_request = true;
        }
      }

      if (has_request){
        eiint.bit_val.req  = true;
        eiint.bit_val.ch   = channel;
        eiint.bit_val.pri  = priority;
        eiint.bit_val.tb   =  pEeic->GetEITB();
        bind.bit_val.peid  = peid;
        eiint.bit_val.gpid = bit_gpid;
        w_flag = true;
      }
    }
    dmsgsc("  w_flag:" << w_flag);
    if ( w_flag ) {
      mpIntc2->setEiintReqPort( peid, eiint.val, G4IntcTyp::PRI_CANCEL_PROC );
    }
  }
}

///<summary>function to check EIINT has been requested to INTC1 or not</summary>
///<param name="peid">PEID</param>
///<param name="channel">number of channel</param>
///<returns>none</returns>
bool
G4_Intc2Func::IsIntReqInQueueOfIntc1( uint32_t peid, uint32_t channel )
{
  dmsgsc( mpIntc2->name() << ".G4_Intc2Func.IsIntReqInQueueOfIntc1()" );
  TRegEIBD*         pEibd = GetEIBDPtr( channel );
  bool              ret   = false;
  G4IntcTyp::EiintReq eiint( mpIntc2->i2_g_eiint[peid]->read() );

  if ( (1 == eiint.bit_val.req) && (channel == eiint.bit_val.ch) ) {
    ret = true;
  }

  dmsgsc( "  ret:" << ret );
  return ret;
}

///<summary>function to request BCAST</summary>
///<param name="channel">number of channel</param>
///<returns>none</returns>
void
G4_Intc2Func::AddBcasIntInfo( uint32_t channel )
{
  dmsgsc( mpIntc2->name() << ".G4_Intc2Func.AddBcasIntInfo()" );
  TRegEIC*  pEic  = GetEICPtr ( channel );
  TRegEEIC*  pEeic  = GetEEICPtr ( channel );
  TRegEIBD* pEibd = GetEIBDPtr( channel );
  assert(pEic !=NULL);
  assert(pEeic !=NULL);
  assert(pEibd !=NULL);
  
  mpIntc2->setBcastReqPort( pEibd->GetBCP(), true, G4IntcTyp::PRI_EXEC_PROC, false );
  pEic->SetEIRF( 0 );
  pEeic->SetEIRF( 0 );
}
