/*************************************************************************
 *
 * $Id: G4_Intc1Func.cpp,v 1.14 2020/03/24 09:58:12 hadinhtran Exp $
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
#include "G4_Intc1Func.h"
#include "G4_Intc1FuncUsr.h"
#include "NSMVINTC1V02.h"
#include "BusSlaveFuncIf.h"
#include "OSCI2.h"
#include "sideband.h"

// for debug output
#ifdef DEBUG_MESSAGE_OUTPUT
# include <fstream>
  extern std::ofstream difout;
#endif // DEBUG_MESSAGE_OUTPUT


/// <summary>constructor</summary>
/// <param name="peid">PEID</param>
/// <param name="baseAddr">base address</param>
/// <param name="pIntc1">pointer to INTC1 module</param>
/// <returns>none</returns>
G4_Intc1Func::G4_Intc1Func(
  unsigned int  peid,
  unsigned int  baseAddr,
  NSMVINTC1V02* pIntc1,
  unsigned int  core_ver):
    CIntc1( 0, peid, 0, CORE_G4MH, (CoreVersion)core_ver),
    BusSlaveFuncIf(),
    mpIntc1( pIntc1 ),
    mEicWriteFlag( false ),
    mEicUpdateFlag( false ),
    mEeicWriteFlag( false ),
    mEeicUpdateFlag( false ),
    mEicUpdateCh( 0 ),
    mEeicUpdateCh( 0 ),
    mEibdUpdateFlag(0),
    mEibgUpdateFlag(0),
    mCoreVersion(core_ver),
    mPeid(peid)
{
  assert(pIntc1 != NULL);
  dmsgsc( mpIntc1->name() << ".G4_Intc1Func.G4_Intc1Func()" );
  mpIntc1FuncUsr = new G4_Intc1FuncUsr;
  mpIntc1FuncUsr->setWrapperPtr( pIntc1 );
}

/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
G4_Intc1Func::~G4_Intc1Func( void )
{
  dmsgsc( "G4_Intc1Func.~G4_Intc1Func()" );
  delete mpIntc1FuncUsr;
}

///<summary>function to reset</summary>
///<param name="">none</param>
///<returns>none</returns>
void
G4_Intc1Func::reset( void )
{
  dmsgsc( mpIntc1->name() << ".G4_Intc1Func.reset()" );
  mEicWriteFlag  = false;
  mEicUpdateFlag = false;
  mEeicWriteFlag  = false;
  mEeicUpdateFlag = false;
  mEicUpdateCh   = 0;
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
G4_Intc1Func::read (
  unsigned int     offsetAddress, 
  TlmBasicPayload& trans, 
  BusTime_t*       t,
  bool             debug )
{
  assert(t != NULL);
  dmsgsc( mpIntc1->name() << ".G4_Intc1Func.read()" );
  mpIntc1FuncUsr->read_precallout( trans, t, debug );

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
         ( ((G4IntcTyp::INTC1_EIC_OFFSET <= offsetAddress) &&
             ((G4IntcTyp::INTC1_EIC_OFFSET+0x3E) >= offsetAddress)) ||
           (G4IntcTyp::INTC1_FNC_OFFSET == offsetAddress) ||
           ((G4IntcTyp::INTC1_FIC_OFFSET <= offsetAddress) &&
            ((G4IntcTyp::INTC1_FIC_OFFSET+0x1E) >= offsetAddress))) ||
           (G4IntcTyp::INTC1_DBMK_OFFSET == offsetAddress) ) { 
      TargetReadDebug( offsetAddress,   pData,   2 ); // <-- Process access 4 bytes to register size 2 bytes
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

    dmsgsc( "  TargetRead()" );
    TargetRead( sb, offsetAddress, pData, size );
  }

  dmsgsc( "  address:0x" << trans.get_address() );
  for ( unsigned int i = 0; i < size; i++ ) {
    dmsgsc( "  data:0x" << (int)pData[i] );
  }

  mpIntc1FuncUsr->read_postcallout( trans, t, debug );
}

///<summary>TLM write function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void
G4_Intc1Func::write(
  unsigned int     offsetAddress, 
  TlmBasicPayload& trans, 
  BusTime_t*       t,
  bool             debug )
{
  assert(t != NULL);
  dmsgsc( mpIntc1->name() << ".G4_Intc1Func.write()" );
  mpIntc1FuncUsr->write_precallout( trans, t, debug );

  unsigned char* pData = trans.get_data_ptr();
  assert(pData != NULL);
  unsigned int   size  = trans.get_data_length();
  TRegFNC*       pFnc  = GetFNCPtr();
  TRegFIC*       pFic[INTC1_FEINT_CH_NUM];
  bool           isReadOnly, isDebugOnly;
  unsigned int   pre_GM_bit = 0;
  unsigned int   pre_GPID_bit = 0;
  unsigned int   pre_BGPR_bit = 0;
  unsigned int   channel = 0;
  bool           Eibd_write_flag = false;
  bool           Eibg_write_flag = false;

  //RegData prevFimk[INTC1_FEINT_CH_NUM];//synguyen
  
  for ( unsigned int i = 0; i < INTC1_FEINT_CH_NUM; i++) {
    pFic[i] = GetFICPtr(i);
    assert(pFic != NULL);
  }
  
  // Check if writing to register has write-callback
  if ( (G4IntcTyp::INTC1_EIC_OFFSET <= offsetAddress) &&
       ((G4IntcTyp::INTC1_EIC_OFFSET+0x3F) >= offsetAddress) ) {
    mEicWriteFlag = true;
    mEicUpdateFlag = true;
       }

  if ( (G4IntcTyp::INTC1_EEIC_OFFSET <= offsetAddress) &&
       ((G4IntcTyp::INTC1_EEIC_OFFSET+0x7F) >= offsetAddress) ) {
    mEeicWriteFlag = true;
    mEeicUpdateFlag = true;
  }

  if ( (G4IntcTyp::INTC1_EIBD_OFFSET <= offsetAddress) &&
       ((G4IntcTyp::INTC1_EIBD_OFFSET+0x7F) >= offsetAddress) ) {
    Eibd_write_flag = true;
    channel = (offsetAddress - G4IntcTyp::INTC1_EIBD_OFFSET) / 4;
    TRegEIBD*    pEibd  = GetEIBDPtr( channel );
    pre_GM_bit = pEibd->GetGM();
    pre_GPID_bit = pEibd->GetGPID();
  }

  if ( (G4IntcTyp::INTC1_EIBG_OFFSET <= offsetAddress) &&
       ((G4IntcTyp::INTC1_EIBG_OFFSET+0x3) >= offsetAddress) ) {
    Eibg_write_flag = true;
    TRegEIBG*    pEibg  = GetEIBGPtr();
    pre_BGPR_bit = pEibg->GetBGPR();
  }

  // Check if writing to read-only register
  if ( (G4IntcTyp::INTC1_FNBD_OFFSET <= offsetAddress) &&
       ((G4IntcTyp::INTC1_FNBD_OFFSET+4) > offsetAddress) ) {
    isReadOnly = true;
  }
  else {
    isReadOnly = false;
  }

  // Write to register has no callback
  if ( ((G4IntcTyp::INTC1_FNC_OFFSET <= offsetAddress) &&
        ((G4IntcTyp::INTC1_FNC_OFFSET+2) > offsetAddress)) ||
       ((G4IntcTyp::INTC1_FIC_OFFSET <= offsetAddress) &&
        ((G4IntcTyp::INTC1_FIC_OFFSET+0x1F) > offsetAddress)) ||
       (G4IntcTyp::INTC1_DBMK_OFFSET == offsetAddress) ) { 
    isDebugOnly = true;
  }
  else {
    isDebugOnly = false;
  }

  // no need to check the range of the address
  // because  CForest uses dummy register object
  // when the address is out of range.
  if ( isReadOnly ) {
    // do nothing when writing to read-only register
  }
  else if ( debug ) {
    dmsgsc( "  TargetWriteDebug()" );
    if ( (4 == size) &&
         ( ((G4IntcTyp::INTC1_EIC_OFFSET <= offsetAddress) &&
             ((G4IntcTyp::INTC1_EIC_OFFSET+0x3E) >= offsetAddress)) ||
           (G4IntcTyp::INTC1_FNC_OFFSET == offsetAddress) ||
           ((G4IntcTyp::INTC1_FIC_OFFSET <= offsetAddress) &&
            ((G4IntcTyp::INTC1_FIC_OFFSET+0x1E) >= offsetAddress))) ||
           (G4IntcTyp::INTC1_DBMK_OFFSET == offsetAddress) ) { 
      TargetWriteDebug( offsetAddress,   pData,   2 );
      TargetWriteDebug( offsetAddress+2, pData+2, 2 );
    }
    else {
      TargetWriteDebug( offsetAddress, pData, size );
    }
  }
  else if ( isDebugOnly ) {
    // do nothing
  }
  else {
    dmsgsc( "  TargetWrite()" );
    SideBand         sb;
    TlmG3mExtension* pG3mExtension;

    trans.get_extension( pG3mExtension );
    sb.SetBMID(pG3mExtension->getPeId());
    sb.SetSPID(pG3mExtension->getSpId());
    sb.SetUM(pG3mExtension->isUserMode());
    sb.SetGPID(pG3mExtension->getVmId());
    sb.SetGM((unsigned int)(pG3mExtension->isVirtualMode()));

    dmsgsc( "  offsetAddress:0x" << offsetAddress );
    TargetWrite( sb, offsetAddress, pData, size );
  }

  dmsgsc( "  address:0x" << trans.get_address() << " isReadOnly:" << isReadOnly );
  for ( unsigned int i = 0; i < size; i++ ) {
    dmsgsc( "  data:0x" << (int)pData[i] );
  }

  mEicWriteFlag = false; // set 'false' before calling 'UpdateInterruptPort()' function
  if ( mEicUpdateFlag ) { // EIC register has been updated
    UpdateInterruptPort( mEicUpdateCh );
    mEicUpdateFlag = false;
  }

  mEeicWriteFlag = false; // set 'false' before calling 'UpdateInterruptPort()' function
  if ( mEeicUpdateFlag ) { // EEIC register has been updated
    UpdateInterruptPort( mEeicUpdateCh );
    mEeicUpdateFlag = false;
  }

  if ( Eibd_write_flag ) { // EIBD reg is written
    TRegEIBD*    pEibd  = GetEIBDPtr( channel );
    if ( (pEibd->GetGM() != pre_GM_bit) || (pEibd->GetGPID() != pre_GPID_bit) ){
      mEibdUpdateFlag = true;
      UpdateInterruptPort( channel );
    }else{
      mEibdUpdateFlag = false;
    }
  }

  if ( Eibg_write_flag ) { // EIBG reg is written
    TRegEIBG*    pEibg  = GetEIBGPtr();
    if (pEibg->GetBGPR() != pre_BGPR_bit){
      mEibgUpdateFlag = true;
      G4IntcTyp::EiintReq eiint( mpIntc1->t_eiint_1l.read() );
      UpdateInterruptPort( eiint.bit_val.ch );
    }else{
      mEibgUpdateFlag = false;
    }
  }

  mpIntc1FuncUsr->write_postcallout( trans, t, debug );
}

///<summary>function to get request information in queue</summary>
///<param name="channel">number of channel</param>
///<returns>pointer to IntReqInfoBase(declared in CForest INTC2)</returns>
IntReqInfoBase*
G4_Intc1Func::getQueueReqInfo( uint32_t channel )
{
  dmsgsc( mpIntc1->name() << ".G4_Intc1Func.getQueueReqInfo()" );
  return(GetQueueReqInfo(channel));
}

///<summary>function to get size of queue in INTC1 module</summary>
///<param name="">none</param>
///<returns>queue size</returns>
unsigned int
G4_Intc1Func::getQueueSize( void )
{
  dmsgsc( mpIntc1->name() << ".G4_Intc1Func.getQueueSize() size:0x" << GetQueueSize() );
  return GetQueueSize();
}


// override function
///<summary>function to notify ExecProc</summary>
///<param name="">none</param>
///<returns>none</returns>
void
G4_Intc1Func::StartCyclicHandler( void )
{
  dmsgsc( mpIntc1->name() << ".G4_Intc1Func.StartCyclicHandler()" );
  mpIntc1->notifyExecEvent();
}

///<summary>function to request FENMI</summary>
///<param name="tcid">thread context id</param>
///<returns>none</returns>
void
G4_Intc1Func::ReqFenmiToCpu( void (*resfunc)(CIntc1* intc1) )
{
  dmsgsc( mpIntc1->name() << ".G4_Intc1Func.ReqFenmiToCpu()" );
  mpIntc1->setFenmiReqPort( true, G4IntcTyp::PRI_EXEC_PROC );
  mpIntc1FuncUsr->ReqFenmiToCpu_callout();
}

///<summary>function to request FEINT</summary>
///<param name="channel">number of channel</param>
///<param name="bindmode">select guest mode or host mode</param>
///<param name="gpid">partition identifier</param>
///<param name="bge">backing ground enable</param>
///<param name="(*resfunc)(uint32_t channel, CIntc1* p_intc)">CForest callback function.(not used in SC-HEAP)</param>
///<returns>none</returns>
void
G4_Intc1Func::ReqFeintToCpu(uint32_t channel, bool bindmode, uint32_t gpid, uint32_t bge, void (*resfunc)(uint32_t channel, CIntc1* intc))
{
  dmsgsc( mpIntc1->name() << ".G4_Intc1Func.ReqFeintToCpu()" );

  G4IntcTyp::FeintReq feint;
  bool has_request = false;

  bool cpu_gm = mpIntc1->getCPUGM();             // GM of CPU
  unsigned int cpu_gpid = mpIntc1->getCPUGPID(); // GPID of CPU
  unsigned int ihve = GetIHVEFromIHVCFG();       // Get IHVCFG.IHVE bit
  unsigned int cpu_ihve = GetIHVEFromCPU();      // Setting from config file
  unsigned int cpu_has_hv = GetHasHVromCPU();    // Setting from config file

  // HARDWARE-ASSISTED VIRTUALIZATION
  if ( (cpu_has_hv == 1) && (cpu_ihve == 1) && (ihve == 1) ){
    // CPU operation mode: GUEST MODE
    if (cpu_gm){
      if (bindmode){ // Bind to guest mode
        if (gpid == cpu_gpid){
          // Generate GMFEINT
          feint.bit_val.gm = 1;
          feint.bit_val.gpid = gpid;
          has_request = true;

        }else{
          if ( ((bge >> channel) & 0x1) == 1 ){
            // Generate BGFEINT
            feint.bit_val.bg = 1;
            feint.bit_val.gpid = gpid;
            has_request = true;

          }else{
            // not request any interrupt
          }
        }
      }else{ // Bind to host mode
        // Generate FEINT
        has_request = true;
      }

    // CPU operation mode: HOST MODE
    }else{
      if (bindmode){ // Bind to guest mode
        // not request any interrupt
      }else{ // Bind to host mode
        // Generate FEINT
        has_request = true;
      }
    }

  // NORMAL (NONE HV)
  }else{
    has_request = true;
  }

  // Send interrupt request if any
  if (has_request){
    feint.bit_val.req = 1;
    feint.bit_val.ch  = channel;
    m_req_feint_channel = channel;
    mpIntc1->setFeintReqPort( feint.val, G4IntcTyp::PRI_EXEC_PROC );
    mpIntc1FuncUsr->ReqFeintToCpu_callout(channel, bindmode, gpid, bge);
  }
}

///<summary>function to request EIINT</summary>
///<param name="priority">priority</param>
///<param name="channel">number of channel</param>
///<param name="eitb">value of eitb</param>
///<param name="bindmode">select guest mode or host mode</param>
///<param name="gpid">partition identifier</param>
///<param name="bge">backing ground enable</param>
///<param name="(*resfunc)(uint32_t channel, CIntc1* p_intc)">CForest callback function.(not used in SC-HEAP)</param>
///<returns>none</returns>
void
G4_Intc1Func::ReqEiintToCpu(
    uint32_t priority,
    uint32_t channel, 
    bool eitb,
    bool bindmode,
    uint32_t gpid,
    uint32_t bgpr,
    void (*resfunc)(uint32_t channel, CIntc1* intc))
{

  dmsgsc( mpIntc1->name() << ".G4_Intc1Func.ReqEiintToCpu()" );

  G4IntcTyp::EiintReq eiint;
  bool has_request = false;

  bool cpu_gm = mpIntc1->getCPUGM();             // GM of CPU
  unsigned int cpu_gpid = mpIntc1->getCPUGPID(); // GPID of CPU
  unsigned int ihve = GetIHVEFromIHVCFG();       // Get IHVCFG.IHVE bit
  unsigned int cpu_ihve = GetIHVEFromCPU();      // Setting from config file
  unsigned int cpu_has_hv = GetHasHVromCPU();    // Setting from config file
  dmsgsc("        channel: " << channel);
  dmsgsc("        priority: " << priority);
  dmsgsc("        bgpr: " << bgpr);
  dmsgsc("        gpid: " << gpid);
  dmsgsc("        cpu_gpid: " << cpu_gpid);
  dmsgsc("        cpu_gm: " << cpu_gm);
  dmsgsc("        ihve: " << ihve);
  dmsgsc("        cpu_ihve: " << cpu_ihve);
  dmsgsc("        cpu_has_hv: " << cpu_has_hv);
  dmsgsc("        bindmode: " << bindmode);
  // ==== EIINT of INTC1 ===
  if (INTC1_CH_NUM > channel) {
    // HARDWARE-ASSISTED VIRTUALIZATION
    if ( (cpu_has_hv == 1) && (cpu_ihve == 1) && (ihve == 1) ){
      // CPU operation mode: GUEST MODE
      if (cpu_gm){
        if (bindmode){ // Bind to guest mode
          if (gpid == cpu_gpid){
            // Generate GMEIINT
            eiint.bit_val.gm  = 1;
            eiint.bit_val.gpid  = gpid;
            has_request = true;
          }else{
            if ( priority < bgpr ){
              // Generate BGEIINT
              eiint.bit_val.bg  = 1;
              eiint.bit_val.bgpr  = bgpr;
              eiint.bit_val.gpid  = gpid;
              has_request = true;
            }else{
              // not request any interrupt
            }
          }
        }else{ // Bind to host mode
          // Generate EIINT
          has_request = true;
        }

      // CPU operation mode: HOST MODE
      }else{
        if (bindmode){ // Bind to guest mode
          // not request any interrupt
        }else{ // Bind to host mode
          // Generate EIINT
          has_request = true;
        }
      }

    // NORMAL (NONE HV)
    }else{
      has_request = true;
    }
  // ==== EIINT sent from INTC2 ===
  }else{
    if (bindmode){// GM or BG
      if (bgpr > 0){
        eiint.bit_val.bg  = 1;
        eiint.bit_val.bgpr  = bgpr;
        eiint.bit_val.gpid  = gpid;
      }else {
        eiint.bit_val.gm  = 1;
        eiint.bit_val.gpid  = gpid;
      }
    }
    has_request = true;
  }
  
  if (has_request){
    eiint.bit_val.req = 1;
    eiint.bit_val.ch  = channel;
    eiint.bit_val.pri = priority;
    eiint.bit_val.tb  = eitb;
    dmsgsc( "  channel:0x" << channel << " priority:0x" << priority << " tb:" << eiint.bit_val.tb 
      << " gm:" << eiint.bit_val.gm << " gpid:" << eiint.bit_val.gpid << " bg:" << eiint.bit_val.bg);

    mpIntc1->setEiintReqPort( eiint.val, G4IntcTyp::PRI_EXEC_PROC );
    mpIntc1FuncUsr->ReqEiintToCpu_callout( channel, bindmode, gpid, bgpr);
  }
}

///<summary>function to drop EIINT request</summary>
///<param name="channel">number of channel</param>
///<param name="tcid">thread context id</param>
///<returns>none</returns>
void
G4_Intc1Func::CancelIntToCpu( uint32_t channel )
{
  dmsgsc( mpIntc1->name() << ".G4_Intc1Func.CancelIntToCpu() channel: 0x" << channel);
  G4IntcTyp::EiintReq eiint;
  TRegEEIC* pEeic;
  if (INTC1_CH_NUM > channel){
      pEeic = GetEEICPtr( channel );
  } 
  else {
      pEeic = mpIntc1->mpIntc2ReqInfo->getEeicPtr( channel );
  }
  
  assert(pEeic != NULL);
  eiint.bit_val.ch  = channel; // for CPU 
  eiint.bit_val.pri = pEeic->GetEIP(); // for CPU
  mpIntc1->setEiintReqPort( eiint.val, G4IntcTyp::PRI_CANCEL_PROC );

  std::set<unsigned int>::iterator it;
  it = std::find( mpIntc1->mReEntryReqList.begin(), mpIntc1->mReEntryReqList.end(), channel );
  if (it != mpIntc1->mReEntryReqList.end()) {
    mpIntc1->mReEntryReqList.erase( it );
  }
  mpIntc1FuncUsr->CancelEiintToCpu_callout( channel );

  if (channel < INTC1_CH_NUM) {
    if (mpIntc1->resendEiintForbid == true) { 
      mpIntc1->resendEiintForbid = false;
    } else {
      mpIntc1->resendEiintReqPort();
    }
  }
}

///<summary>function to drop EIINT request</summary>
///<param name="channel">number of channel</param>
///<param name="dummy">indicate where calls this function. true: called from CyclicHandler; false: called from UpdateHighPriorityFromCpuMode()</param>
///<param name="tcid">thread context id</param>
///<returns>none</returns>
void
G4_Intc1Func::CancelIntToCpu( uint32_t channel, bool dummy )
{
  dmsgsc( mpIntc1->name() << ".G4_Intc1Func.CancelIntToCpu() channel: 0x" << channel << " dummy: 0x" << dummy );
  if (dummy){
    // Do nothing
  }else{ // Not process Eiint if cpu mode is host, GM bit = 1, HVIE = 1 (in G4MH2.0)
    if (mCoreVersion == CORE_VER_20){
      TRegEIBD* pEibd;
      if (INTC1_CH_NUM > channel){
          pEibd = GetEIBDPtr( channel );
      }
      else {
          pEibd = mpIntc1->mpIntc2ReqInfo->getEibdPtr( channel );
      }
      if ((pEibd->GetGM() == 1) && (GetIHVEFromIHVCFG() > 0)){
        G4IntcTyp::EiintReq eiint;
        TRegEEIC* pEeic;
        if (INTC1_CH_NUM > channel){
            pEeic = GetEEICPtr( channel );
        }
        else {
            pEeic = mpIntc1->mpIntc2ReqInfo->getEeicPtr( channel );
        }
        eiint.bit_val.ch  = channel; // for CPU 
        eiint.bit_val.pri = pEeic->GetEIP(); // for CPU
        dmsgsc( "        ->cancelled eiint channel: 0x" << channel);
        mpIntc1->setEiintReqPort( eiint.val, G4IntcTyp::PRI_CANCEL_PROC );
        mpIntc1FuncUsr->CancelEiintToCpu_callout( channel );
        mpIntc1->resendEiintReqPort();
      }
    }
  }
}

///<summary>function to drop FENMI request</summary>
///<param name="tcid">thread context id</param>
///<returns>none</returns>
void
G4_Intc1Func::CancelFenmiToCpu( void  )
{
  dmsgsc( mpIntc1->name() << ".G4_Intc1Func.CancelFenmiToCpu()" );
  mpIntc1->setFenmiReqPort(false, G4IntcTyp::PRI_CANCEL_PROC );
  StartCyclicHandler();

  mpIntc1FuncUsr->CancelFenmiToCpu_callout();
}

///<summary>function to drop FEINT request</summary>
///<param name="channel">feint channel</param>
///<param name="dummy">indicate where calls this function. false: called from UpdateHighPriorityFromCpuMode(); true: not used this value yet</param>
///<returns>none</returns>
void
G4_Intc1Func::CancelFeintToCpu( uint32_t channel, bool dummy )
{
  dmsgsc( mpIntc1->name() << ".G4_Intc1Func.CancelFeintToCpu() channel: 0x"<< channel << ", dummy: 0x"<< dummy );
  if (dummy){
    // Do nothing (not use dummy = false yet)
  }else{ // called from UpdateHighPriorityFromCpuMode() to not process Feint if cpu mode is host, GM bit = 1, HVIE = 1
    unsigned int feint_channel = channel - FENMI_CHANNEL - 1;
    TRegFIBD* pFibd = GetFIBDPtr( feint_channel );
    if ((pFibd->GetGM() == 1) && (GetIHVEFromIHVCFG() > 0)){
      mpIntc1->setFeintForceCanceFlag(false); 
      mpIntc1->setFeintReqPort( 0, G4IntcTyp::PRI_CANCEL_PROC );

      mpIntc1FuncUsr->CancelFeintToCpu_callout();
      mpIntc1->resendFeintReqPort();
    }
  }
}

///<summary>function to drop FEINT request</summary>
///<param name="tcid">thread context id</param>
///<returns>none</returns>
void
G4_Intc1Func::CancelFeintToCpu( void )
{
  dmsgsc( mpIntc1->name() << ".G4_Intc1Func.CancelFeintToCpu(void) ");
  mpIntc1->setFeintForceCanceFlag(false); 
  mpIntc1->setFeintReqPort( 0, G4IntcTyp::PRI_CANCEL_PROC );
  // StartCyclicHandler();

  mpIntc1FuncUsr->CancelFeintToCpu_callout();
  if(mpIntc1->resendFeintForbid == true){
     mpIntc1->resendFeintForbid = false;
  } else {
     mpIntc1->resendFeintReqPort();
  }

}

///<summary>function to drop FEINT request </summary>
///<param name="tcid">thread context id</param>
///<returns>none</returns>
void
G4_Intc1Func::CancelFeintToCpu( bool dummy )
{ 
  dmsgsc( mpIntc1->name() << ".G4_Intc1Func.CancelFeintToCpu(dummy) ");
}


///<summary>function to update EIINT port value</summary>
///<param name="channel">number of channel</param>
///<returns>none</returns>
void
G4_Intc1Func::UpdateInterruptPort( uint32_t channel )
{
  dmsgsc( mpIntc1->name() << ".G4_Intc1Func.UpdateInterruptPort( " << channel << " )  mEicWriteFlag: " << mEicWriteFlag << ", mEeicWriteFlag: " << mEeicWriteFlag);
  if ( mEicWriteFlag ) {
    mEicUpdateCh   = channel;
    mEicUpdateFlag = true;
  }else if (mEeicWriteFlag){
    mEeicUpdateCh = channel;
    mEeicUpdateFlag = true;
  } else {
    bool         w_flag = false;
    bool has_request = false;
    TRegEIC* pEic;
    if (INTC1_CH_NUM > channel) {
        pEic = GetEICPtr( channel );
    } else {
        pEic = mpIntc1->mpIntc2ReqInfo->getEicPtr( channel );
    }
    TRegEEIC* pEeic;
    if (INTC1_CH_NUM > channel) {
        pEeic = GetEEICPtr( channel );
    } else {
        pEeic = mpIntc1->mpIntc2ReqInfo->getEeicPtr( channel );
    }
    TRegEIBD*    pEibd;
    if (INTC1_CH_NUM > channel) {
        pEibd = GetEIBDPtr( channel );
    } else {
        pEibd = mpIntc1->mpIntc2ReqInfo->getEibdPtr( channel );
    }
    TRegEIBG*    pEibg;
    if (INTC1_CH_NUM > channel) {
        pEibg = GetEIBGPtr();
    } else {
        pEibg = mpIntc1->mpIntc2ReqInfo->getEibgPtr( mPeid );
    }
    G4IntcTyp::EiintReq eiint( mpIntc1->t_eiint_1l.read() );

    dmsgsc( "  eiint.bit_val.pri:0x" << eiint.bit_val.pri << " pEeic->GetEIP():0x" << pEeic->GetEIP() << " pEeic->GetEIMK():0x" << pEeic->GetEIMK() << " eiint.bit_val.req:" << (int)eiint.bit_val.req << ", pEeic->GetEIRF(): "<< pEeic->GetEIRF());
    if ( 1 == pEic->GetEIMK() ) {
      eiint.val         = 0;
      eiint.bit_val.ch  = channel; // for CPU
      eiint.bit_val.pri = pEeic->GetEIP();
      w_flag            = true;
    }
    else if ( ((eiint.bit_val.pri > pEeic->GetEIP()) && (1 == eiint.bit_val.req) && (pEeic->GetEIRF() == 1)) ||
              ((0 == eiint.bit_val.req) && (pEeic->GetEIRF() == 1)) || //If previous req is 0 and current req is 1
              ((1 == eiint.bit_val.req) && (eiint.bit_val.ch == channel) && (pEeic->GetEIRF() == 1) && ((eiint.bit_val.pri != pEeic->GetEIP()) || mEibdUpdateFlag || mEibgUpdateFlag)) ){ // change priority, GM, GPID, EIBG bit
      bool cpu_gm = mpIntc1->getCPUGM();             // GM of CPU
      unsigned int cpu_gpid = mpIntc1->getCPUGPID(); // GPID of CPU
      unsigned int ihve = GetIHVEFromIHVCFG();       // Get IHVCFG.IHVE bit
      unsigned int cpu_ihve = GetIHVEFromCPU();      // Setting from config file
      unsigned int cpu_has_hv = GetHasHVromCPU();    // Setting from config file
      unsigned int bindmode = pEibd->GetGM();
      unsigned int gpid = pEibd->GetGPID();
      unsigned int priority = pEeic->GetEIP();
      unsigned int bgpr = pEibg->GetBGPR();
      dmsgsc ("        channel: "<< channel);
      dmsgsc ("        cpu_gm: "<< cpu_gm);
      dmsgsc ("        cpu_gpid: "<< cpu_gpid);
      dmsgsc ("        ihve: "<< ihve);
      dmsgsc ("        cpu_ihve: "<< cpu_ihve);
      dmsgsc ("        cpu_has_hv: "<< cpu_has_hv);
      dmsgsc ("        bindmode: "<< bindmode);
      dmsgsc ("        gpid: "<< gpid);
      dmsgsc ("        priority: "<< priority);
      dmsgsc ("        bgpr: "<< bgpr);
      // HARDWARE-ASSISTED VIRTUALIZATION
      if ( (cpu_has_hv == 1) && (cpu_ihve == 1) && (ihve == 1) ){
        // CPU operation mode: GUEST MODE
        if (cpu_gm){
          if ((bool)bindmode){ // Bind to guest mode
            if (gpid == cpu_gpid){
              // Generate GMEIINT
              eiint.bit_val.gm  = 1;
              eiint.bit_val.gpid  = gpid;
              has_request = true;
            }else{
              if ( priority < bgpr ){
                // Generate BGEIINT
                eiint.bit_val.bg  = 1;
                eiint.bit_val.bgpr  = bgpr;
                eiint.bit_val.gpid  = gpid;
                has_request = true;
              }else{
                if (eiint.bit_val.req == 1){
                  eiint.bit_val.req = 0;
                  eiint.bit_val.ch  = channel;
                  w_flag = 1;
                }
              }
            }
          }else{ // Bind to host mode
            // Generate EIINT
            has_request = true;
          }

        // CPU operation mode: HOST MODE
        }else{
          if ((bool)bindmode){ // Bind to guest mode
            if (eiint.bit_val.req == 1){
              eiint.bit_val.req = 0;
              eiint.bit_val.ch  = channel;
              w_flag = 1;
            }
          }else{ // Bind to host mode
            // Generate EIINT
            has_request = true;
          }
        }

      // NORMAL (NONE HV)
      }else{
        has_request = true;
      }
      if (has_request){
        eiint.bit_val.req = 1;
        eiint.bit_val.ch  = channel;
        eiint.bit_val.pri = priority;
        eiint.bit_val.tb  = pEeic->GetEITB();
        w_flag = true;
      }

      dmsgsc("  req:" << eiint.bit_val.req << " ch:0x" << eiint.bit_val.ch << "  pri:0x" << eiint.bit_val.pri << " tb:" << eiint.bit_val.tb );
    }

    dmsgsc("  w_flag:" << w_flag);
    if ( w_flag ) {
      if (has_request){
        mpIntc1->setEiintReqPort( 0, G4IntcTyp::PRI_CANCEL_PROC );
        mpIntc1->setEiintReqPort( eiint.val, G4IntcTyp::PRI_CANCEL_PROC );
      }else{
        mpIntc1->setEiintReqPort( eiint.val, G4IntcTyp::PRI_CANCEL_PROC );
      }
    }
  }
}

///<summary>function to initialize priority-map(in cforest)</summary>
///<param name="">none</param>
///<returns>none</returns>
void
G4_Intc1Func::InitPriorityInfo( void )
{
  if ( IsIntc1PriorityEmpty() == true ) {
    Intc1PriorityInsert(NC);
  }
}
