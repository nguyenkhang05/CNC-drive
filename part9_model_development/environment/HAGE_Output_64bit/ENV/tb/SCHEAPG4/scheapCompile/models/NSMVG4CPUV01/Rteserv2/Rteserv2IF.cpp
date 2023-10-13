/*
 * Copyright(c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

// デバッグ出力用
#ifdef DEBUG_MESSAGE_OUTPUT
# include <fstream>
  extern std::ofstream dmfout;
#endif // DEBUG_MESSAGE_OUTPUT

#include "NSMVG4CPUV01.h"
#include "G4_NoDbgIF.h" 
#include "forest_utility_api.h"
//#include "FastIssIF.h"
#include "dbg_typ.h"
#include "dbg_def.h"
#include "Rteserv2Controller.h"
#include "Rteserv2IF.h"

using namespace dbgTyp;


// HWブレークポイント情報管理用の構造体
struct Rteserv2IF::HwBpInfTyp {
  unsigned int   cpuId;    // CPU ID(peid, vcid, tcid, v/n を結合したもの)
  RTE_HWB_KIND_T kind;     // ブレークの種類(実行orアクセス)
  unsigned int   addr;     // アクセスアドレス
  unsigned int   addrMask; // アドレスマスク
  AccessAttr     attr;     // アクセス属性(Read/Write/RW)
  unsigned int   size;     // アクセスサイズ(1/2/4)
  unsigned int   count;    // ヒット回数条件
  unsigned int   hitCount; // 実際にヒットした回数
  unsigned int   hwBpId;   // HWブレークのブレークポイントのID

  bool checkAttrCondition( BreakHitAttribute bAttr ) const;
};



///<summary>アクセスブレーク属性チェック関数</summary>
///<param name="bAttr">アクセスブレーク属性</param>
///<returns>false(不適合)/true(適合)</returns>
bool
Rteserv2IF::HwBpInfTyp::checkAttrCondition( BreakHitAttribute bAttr ) const
{
  bool ret = false;

  switch ( attr ) {
  case ACC_READ:
    ret = ( BRK_DATA_READ == bAttr );
    break;
  case ACC_WRITE:
    ret = ( BRK_DATA_WRITE == bAttr );
    break;
  case ACC_READWRITE:
    ret = ( (BRK_DATA_READ == bAttr) | (BRK_DATA_WRITE == bAttr) );
    break;
  default:
    char msg[32];
    sprintf(msg, "Invalid HW break attr(%d).", attr);
    ErrMsg("Rteserv2IF", "HwBpInfTyp::checkAttrCondition", msg);
    break;
  }

  return ret;
}



///<summary>コンストラクタ</summary>
///<param name="なし"></param>
///<returns>なし</returns>
Rteserv2IF::Rteserv2IF( void ):
    mpRteserv2Controller( 0 ),
    mCpuId( 0 ),
    mpForestUtil( 0 ),
//    mpFastIssIf( 0 ), // for FastISS
    mpBreakInfoCA( 0 ),
//    mpBreakInfoFast( 0 ), // for FastISS
    mPeId( 0 ),
    mVcId( 0 ),
    mTcId( 0 ),
    mHwBpIdCnt( 0 ),
    mHitHwBpId( 0 ),
    mHitHwBpAttr( 0 ),
    mHitPcBpFlag( false ),
    mHitHwBpExFlag( false ),
    mHitHwBpAcFlag( false ),
    mExecStatus( RTE_STATE_HALT )
{
  dmsg("Rteserv2IF::Rteserv2IF");
}



///<summary>デストラクタ</summary>
///<param name="なし"></param>
///<returns>なし</returns>
Rteserv2IF::~Rteserv2IF( void )
{
  dmsg("Rteserv2IF::~Rteserv2IF");
}



///<summary>Rteserv2Controllerのポインタを設定する関数</summary>
///<param name="pRteserv2Controller">Rteserv2Controllerのポインタ</param>
///<returns>なし</returns>
void
Rteserv2IF::setRteserv2ControllerPtr( Rteserv2Controller* pRteserv2Controller )
{
  dmsg("Rteserv2IF::setRteserv2ControllerPtr");
  mpRteserv2Controller = pRteserv2Controller;
  mpG4Cpu             = mpRteserv2Controller->getG4CpuPtr();
  dmsg("  mpG4Cpu:" << mpG4Cpu);
  mpForestUtil         = mpRteserv2Controller->getForestUtilPtr();
  mpBreakInfoCA        = mpForestUtil->GetBreakInfo();
/* for FastISS
  mpFastIssIf          = mpRteserv2Controller->getFastIssPtr();
  mpBreakInfoFast      = mpFastIssIf->getBreakInfo();
*/
}



///<summary>PEID/TCIDの情報を設定する関数</summary>
///<param name="peId">PEID</param>
///<param name="tcId">TCID</param>
///<returns>なし</returns>
void
Rteserv2IF::setCpuInfo( unsigned int peId, unsigned int tcId )
{
  dmsg("Rteserv2IF::setCpuInfo");
  dmsg("  peId:" << peId << " tcId:" << tcId);
  mPeId   = peId;
  mTcId   = tcId;
  //mVNFlag = (NC != tcId); // false:NC, true:VC
  mVNFlag = (0 != tcId); // false:NC, true:VC
}



///<summary>SWブレークポイントを設定する関数</summary>
///<param name="addr">ブレークするPCアドレス</param>
///<returns>なし</returns>
void
Rteserv2IF::setPcBreak( unsigned int addr )
{
  dmsg("Rteserv2IF::setPcBreak addr:" << addr);

  mPcBpLst.insert( addr );
  mpForestUtil->AddBreakNextPc( mPeId, addr, 0xFFFFFFFF );
}



///<summary>設定したSWブレークポイントを削除する関数</summary>
///<param name="addr">削除するブレークポイントのPCアドレス</param>
///<returns>なし</returns>
void
Rteserv2IF::rmvPcBreak( unsigned int addr )
{
  dmsg("Rteserv2IF::remPcBreak addr:" << addr);
  std::set<unsigned int>::iterator it = mPcBpLst.find( addr );

  if ( mPcBpLst.end() == it ) {
    char msg[32];
    sprintf( msg, "Can not find PC break(PC=0x%x).", addr );
    ErrMsg("Rteserv2IF",  "remPcBreak", msg );
  }
  else {
    mPcBpLst.erase( it );
    mpForestUtil->ClearBreakNextPc( mPeId, addr, 0xFFFFFFFF );
  }
}



///<summary>HWブレークポイント(実行)を設定する関数</summary>
///<param name="cpuId">CPU ID(peid, vcid, tcid, v/n を結合したもの)</param>
///<param name="addr">アクセスアドレス</param>
///<param name="addrMask">アドレスマスク</param>
///<param name="count">ヒット回数条件</param>
///<param name="pHwBrkId">HWブレークのブレークポイントのID</param>
///<returns>なし</returns>
void
Rteserv2IF::setHwBreakEx( unsigned int   cpuId,
                          unsigned int   addr,
                          unsigned int   addrMask,
                          unsigned int   count,
                          unsigned int*  pHwBrkId )
{
  dmsg("Rteserv2IF::setHwBreakEx addr:" << hex << addr);
  unsigned int peid   = RTE_PEID( cpuId );
  bool         vnFlag = RTE_VN( cpuId );
  HwBpInfTyp   hwbp;

  hwbp.hwBpId   = mHwBpIdCnt;
  hwbp.cpuId    = cpuId;
  hwbp.kind     = RTE_HWB_KIND_EXE;
  hwbp.addr     = addr;
  hwbp.addrMask = addrMask;
  hwbp.count    = count;
  hwbp.hitCount = 0;

  // HW実行ブレークは、マスクとカウントもチェックする。(R/Wは不要)
  mpForestUtil->AddBreakNextPc( peid, addr, addrMask );

  mHwBpLst.insert( std::map<unsigned int, HwBpInfTyp>::value_type( mHwBpIdCnt, hwbp ) );
  *pHwBrkId = mHwBpIdCnt;
  mHwBpIdCnt++;
}



///<summary>HWブレークポイント(アクセス)を設定する関数</summary>
///<param name="cpuId">CPU ID(peid, vcid, tcid, v/n を結合したもの)</param>
///<param name="addr">アクセスアドレス</param>
///<param name="addrMask">アドレスマスク</param>
///<param name="attr">アクセス属性(Read/Write/RW)</param>
///<param name="size">アクセスサイズ(1/2/4)</param>
///<param name="count">ヒット回数条件</param>
///<param name="pHwBrkId">HWブレークのブレークポイントのID</param>
///<returns>なし</returns>
void
Rteserv2IF::setHwBreakAc( unsigned int   cpuId,
                          unsigned int   addr,
                          unsigned int   addrMask,
                          AccessAttr     attr,
                          unsigned int   size,
                          unsigned int   count,
                          unsigned int*  pHwBrkId )
{
  dmsg("Rteserv2IF::setHwBreakAc addr:" << hex << addr);
  unsigned int peid   = RTE_PEID( cpuId );
  bool         vnFlag = RTE_VN( cpuId );
  HwBpInfTyp   hwbp;

  hwbp.hwBpId   = mHwBpIdCnt;
  hwbp.cpuId    = cpuId;
  hwbp.kind     = RTE_HWB_KIND_ACC;
  hwbp.addr     = addr;
  hwbp.addrMask = addrMask;
  hwbp.attr     = attr;
  hwbp.size     = size;
  hwbp.count    = count;
  hwbp.hitCount = 0;

  mpForestUtil->AddBreakAccess( peid, addr, addr, attr, addrMask );
  mHwBpLst.insert( std::map<unsigned int, HwBpInfTyp>::value_type( mHwBpIdCnt, hwbp ) );
  *pHwBrkId = mHwBpIdCnt;
  mHwBpIdCnt++;
}



///<summary>設定したHWブレークポイントを削除する関数</summary>
///<param name="addr">削除するブレークポイントのID</param>
///<returns>false(成功)/true(失敗)</returns>
bool
Rteserv2IF::rmvHwBreak( unsigned int hwBrkId )
{
  dmsg("Rteserv2IF::rmvHwBreak hwBrkId:" << hwBrkId);
  bool ret = false;
  std::map<unsigned int, HwBpInfTyp>::iterator it = mHwBpLst.find( hwBrkId );

  if ( it == mHwBpLst.end() ) {
    ret = true;
  }
  else {
    unsigned int peid   = RTE_PEID( it->second.cpuId );
    bool         vnFlag = RTE_VN( it->second.cpuId );
    mHitHwBpExFlag      = false;
    if( RTE_HWB_KIND_EXE == it->second.kind ) { // 実行ブレーク
      mpForestUtil->ClearBreakNextPc( peid,
                                      it->second.addr,
                                      it->second.addrMask );
      mHwBpLst.erase( it );
    }
    else if ( RTE_HWB_KIND_ACC == it->second.kind ) { // アクセスブレーク
      mpForestUtil->ClearBreakAccess( peid,
                                      it->second.addr,
                                      it->second.addr,
                                      it->second.attr,
                                      it->second.addrMask );
      mHwBpLst.erase( it );
    }
    else {
      // do nothing  「it->second.kind」は、セット時にチェックされている為。
    }
  }

  return ret;
}



///<summary>設定したHWブレークポイントを全て削除する関数</summary>
///<param name="なし"></param>
///<returns>false(成功)/true(失敗)</returns>
bool
Rteserv2IF::rmvAllHwBreak( void )
{
  dmsg("Rteserv2IF::rmvAllHwBreak");
  bool ret = false;
  std::map<unsigned int, HwBpInfTyp>::iterator it = mHwBpLst.begin();

  while ( mHwBpLst.end() != it ) {
    unsigned int peid   = RTE_PEID( it->second.cpuId );
    bool         vnFlag = RTE_VN( it->second.cpuId );

    if( RTE_HWB_KIND_EXE == it->second.kind ) {
      mpForestUtil->ClearBreakNextPc( peid,
                                      it->second.addr,
                                      it->second.addrMask );
    }
    else if ( RTE_HWB_KIND_ACC == it->second.kind ) {
      mpForestUtil->ClearBreakAccess( peid,
                                      it->second.addr,
                                      it->second.addr,
                                      it->second.attr,
                                      it->second.addrMask );
    }
    else {
      ret = true;
      break;
    }
    it++;
  }

  if ( !ret ) {
    mHitHwBpExFlag = false;
    mHwBpLst.clear();
  }

  return ret;
}



///<summary>ブレークにヒットしているか確認する関数</summary>
///<param name="なし"></param>
///<returns>false(ヒットなし)/true(ヒット)</returns>
bool
Rteserv2IF::checkBreak( void )
{
  dmsg("Rteserv2IF::checkBreak");
  BreakInfo* pBrkInf = 0;
  bool       hitFlag = false;

  dmsg("  mPeId:0x" << hex << mPeId << " mTcId:0x" << mTcId);
  dmsg("  mpG4Cpu:" << mpG4Cpu);

  SIM_MODE_T simMode = mpG4Cpu->GetSimMode();
  if ( SIM_MODE_CA == simMode ) {
    pBrkInf = mpBreakInfoCA;
  }
/* For FastISS
  else if ( SIM_MODE_FAST == simMode ) {
    pBrkInf = mpBreakInfoFast;
  }
*/
  else {
    // ErrMsg("Rteserv2IF", "checkBreak", "Invalid simulation mode has been specified.");
    // Rteserv2Controller側のcheckBreak()内で
    // チェックされている為、不要。
  }

  if ( 0 != pBrkInf ) {
    if ( BRK_INST_EXE == pBrkInf->attr ) { // 実行ブレーク
      hitFlag = checkExecBreak( pBrkInf->inst_addr );
    }
    else {
      if ( SIM_MODE_CA == simMode ) {
        // アクセスブレーク
        if ( 0 != ((BRK_DATA_READ | BRK_DATA_WRITE) & pBrkInf->attr) ) {
          hitFlag = checkAccessBreak( pBrkInf->acc_addr, pBrkInf->attr, pBrkInf->size );
        }
      }
      else { // FastISSはHWブレークをサポートしない
        // do nothing
      }
    }
  }

  if ( hitFlag ) {
    setHaltState();
  }

  return hitFlag;
}



///<summary>Rteserv2IFの状態をRUN実行状態にする関数</summary>
///<param name="なし"></param>
///<returns>なし</returns>
void
Rteserv2IF::setRunState( void )
{
  dmsg("Rteserv2IF::setRunState");
  mExecStatus = RTE_STATE_RUN;
}



///<summary>Rteserv2IFの状態をSTEP実行状態にする関数</summary>
///<param name="なし"></param>
///<returns>なし</returns>
void
Rteserv2IF::setStepState( void )
{
  dmsg("Rteserv2IF::setStepState");
  mExecStatus = RTE_STATE_STEP;
}



///<summary>Rteserv2IFの状態を停止状態にする関数</summary>
///<param name="なし"></param>
///<returns>なし</returns>
void
Rteserv2IF::setHaltState( void )
{
  dmsg("Rteserv2IF::setHaltState");
  mExecStatus = RTE_STATE_HALT;
}



///<summary>Rteserv2IFのID(peid, vcid, tcid, v/n を結合したもの)を返す関数</summary>
///<param name="なし"></param>
///<returns>Rteserv2IFのID(peid, vcid, tcid, v/n を結合したもの)</returns>
unsigned int
Rteserv2IF::getId( void )
{
  dmsg("Rteserv2IF::getId");

  if ( !mVNFlag ) { // NC
    mVcId = 0;
  }
  else {
#if 0 // G4x doesn't have VCID and HTCFG0 register.
    // SR_HTCFG0レジスタからVCIDを取得する。
    // mPeId, mTcIdは、set関数で設定される。
    // mPeId: 自PEの番号(peid)
    // mTcId: 自PE内の自スレッドの番号(tcid)
    RegData   data   = 0;
    RegData64 data64 = 0;
    unsigned int SR_HTCFG0 = CF_SR_HTCFG0 + 0x100; // REC_SREG_OFFSET==0x100
    mpForestUtil->RegReadDebug( mPeId, SR_HTCFG0, &data64 );
    data = data64 & 0xFFFFFFFFUL;
    dmsg("  data:" << data);
    mVcId = (unsigned int)(((data) >> 8) & 0x7);
    dmsg("  mVcId:" << mVcId);
#else
    mVcId = 0;
#endif
  }
  mCpuId = (unsigned int)(RTE_CPUID( mPeId, mVcId, mTcId, mVNFlag ));
  dmsg("  mCpuId:" << mCpuId);

  return mCpuId;
}



///<summary>PCレジスタの値を返す関数</summary>
///<param name="なし"></param>
///<returns>PCレジスタの値</returns>
unsigned int
Rteserv2IF::getPC( void )
{
  RegData64 data64 = 0;
  mpForestUtil->RegReadDebug( mPeId, PC, &data64 );
  dmsg("Rteserv2IF::getPC  PC:" << (RegData)(data64 & 0xFFFFFFFFUL));

  return (data64 & 0xFFFFFFFFUL);
}



///<summary>Rteserv2IFの状態を返す関数</summary>
///<param name="なし"></param>
///<returns>Rteserv2IFの状態</returns>
SIM_STATE_T
Rteserv2IF::getSimState( void )
{
  //dmsg("Rteserv2IF::getSimState: " << mExecStatus);
  return mExecStatus;
}



///<summary>SWブレークポイントにヒットしているか確認する関数</summary>
///<param name="なし"></param>
///<returns>false(ヒットなし)/true(ヒット)</returns>
bool
Rteserv2IF::isHitPcBp( void )
{
  dmsg("Rteserv2IF::isHitPcBp:" << mHitPcBpFlag);
  return mHitPcBpFlag;
}



///<summary>対象のアドレスにSWブレークポイントが設定されているか確認する関数</summary>
///<param name="アドレス">addr</param>
///<returns>false(設定なし)/true(設定中)</returns>
bool
Rteserv2IF::isSetPcBp( unsigned int addr )
{
  dmsg("Rteserv2IF::isSetPcBp");

  return ( mPcBpLst.end() != mPcBpLst.find( addr ) );
}



///<summary>HWブレークポイント(実行)にヒットしているか確認する関数</summary>
///<param name="なし"></param>
///<returns>false(ヒットなし)/true(ヒット)</returns>
bool
Rteserv2IF::isHitHwBpEx( void )
{
  dmsg("Rteserv2IF::isHitHwBpEx:" << mHitHwBpExFlag);
  return mHitHwBpExFlag;
}



///<summary>HWブレークポイント(アクセス)にヒットしているか確認する関数</summary>
///<param name="なし"></param>
///<returns>false(ヒットなし)/true(ヒット)</returns>
bool
Rteserv2IF::isHitHwBpAc( void )
{
  dmsg("Rteserv2IF::isHitHwBpAc:" << mHitHwBpAcFlag);
  return mHitHwBpAcFlag;
}



///<summary>HWブレークポイントにヒットしているか確認する関数</summary>
///<param name="なし"></param>
///<returns>false(ヒットなし)/true(ヒット)</returns>
bool
Rteserv2IF::isHitHwBp( unsigned int* pHwBrkId, unsigned int* pAttr )
{
  dmsg("Rteserv2IF::isHitHwBp");
  bool ret = false;

  if ( mHitHwBpExFlag ) {
    mHitHwBpExFlag = false;
    *pHwBrkId      = mHitHwBpId;
    dmsg("  mHitHwBpId: " << mHitHwBpId);
    *pAttr         = 4; // 実行ブレーク
    ret            = true;
  }
  else if ( mHitHwBpAcFlag ) {
    mHitHwBpAcFlag = false;
    *pHwBrkId      = mHitHwBpId;
    dmsg("  mHitHwBpId: " << mHitHwBpId);
    *pAttr         = mHitHwBpAttr;
    ret            = true;
  }
  else {
    ret = false;
  }

  return ret;
}



///<summary>Rteserv2IFの状態がHALTかを確認する関数</summary>
///<param name="なし"></param>
///<returns>false(HALT以外)/true(HALT)</returns>
bool
Rteserv2IF::isHaltState( void )
{
  dmsg("Rteserv2IF::isHaltState:"
       << (RTE_STATE_HALT == mExecStatus));
  return (RTE_STATE_HALT == mExecStatus);
}



///<summary>SWブレークポイントにヒットしたことを示すフラグを操作する関数</summary>
///<param name="val">false(ヒットなし)/true(ヒット)</param>
///<returns>なし</returns>
void
Rteserv2IF::setPcBpFlag( bool val )
{
  dmsg("Rteserv2IF::setPcBpFlag:" << val);
  mHitPcBpFlag = val;
}



///<summary>HWブレークポイント(実行)にヒットしたことを示すフラグを操作する関数</summary>
///<param name="val">false(ヒットなし)/true(ヒット)</param>
///<returns>なし</returns>
void
Rteserv2IF::setHwBpExFlag( bool val )
{
  dmsg("Rteserv2IF::setHwBpExFlag:" << val);
  mHitHwBpExFlag = val;
}



///<summary>HWブレークポイント(アクセス)にヒットしたことを示すフラグを操作する関数</summary>
///<param name="val">false(ヒットなし)/true(ヒット)</param>
///<returns>なし</returns>
void
Rteserv2IF::setHwBpAcFlag( bool val )
{
  dmsg("Rteserv2IF::setHwBpAcFlag:" << val);
  mHitHwBpAcFlag = val;
}





///<summary>実行ブレークにヒットしているか確認する関数</summary>
///<param name="addr">ブレーク時の実行アドレス</param>
///<returns>false(ヒットなし)/true(ヒット)</returns>
bool
Rteserv2IF::checkExecBreak( unsigned int addr )
{
  dmsg("Rteserv2IF::checkExecBreak");
  std::map<unsigned int, HwBpInfTyp>::iterator it = mHwBpLst.begin();

  mHitPcBpFlag   = false;
  mHitHwBpExFlag = false;
  if ( mPcBpLst.end() != mPcBpLst.find( addr ) ) { // SWブレーク
    dmsg("  SWブレーク addr:" << addr);
    mHitPcBpFlag = true;
  }
  else { // HWブレーク(実行属性)
    while ( mHwBpLst.end() != it ) {
      unsigned int bpAddr  = (unsigned int)(((it->second).addr)&((it->second).addrMask));
      unsigned int hitAddr = (unsigned int)((addr)&((it->second).addrMask));
      if ( RTE_HWB_KIND_EXE == (it->second).kind &&  // 実行属性
           bpAddr == hitAddr ) {
        (it->second).hitCount++;
        dmsg("  (it->second).hitCount:" << (it->second).hitCount << "  (it->second).count:" << (it->second).count);
        if ( (it->second).hitCount >= (it->second).count ) {
          mHitHwBpExFlag        = true;
          mHitHwBpId            = (it->second).hwBpId;
          mHitHwBpAttr          = 4; // 実行ブレーク
          (it->second).hitCount = 0;
          dmsg("  mHitHwBpId: " << mHitHwBpId << "  addr:0x" << hex << addr);
          break;
        }
      }
      it++;
    }
  }

  dmsg("  return " << (mHitPcBpFlag || mHitHwBpExFlag));
  return mHitPcBpFlag || mHitHwBpExFlag;
}



///<summary>アクセスブレークにヒットしているか確認する関数</summary>
///<param name="addr">ブレーク時のアクセスアドレス</param>
///<param name="bAttr">ブレーク時のアクセス属性(Read/Write/RW)</param>
///<param name="size">ブレーク時のアクセスサイズ</param>
///<returns>false(ヒットなし)/true(ヒット)</returns>
bool
Rteserv2IF::checkAccessBreak( unsigned int addr, BreakHitAttribute bAttr, unsigned int size )
{
  dmsg("Rteserv2IF::checkAccessBreak");
  std::map<unsigned int, HwBpInfTyp>::iterator it = mHwBpLst.begin();

  while ( mHwBpLst.end() != it ) {
    if ( RTE_HWB_KIND_ACC == (it->second).kind &&      // アクセスブレーク
         (it->second).checkAttrCondition( bAttr ) &&   // R/W属性の確認
         addr == (unsigned int)((it->second).addr) &&  // アドレスマスクはCForest側で考慮する。
         size == (unsigned int)((it->second).size) ) { // アクセスサイズの確認
      (it->second).hitCount++;
      if ( (it->second).hitCount >= (it->second).count ) {
        mHitHwBpAcFlag = true;
        mHitHwBpId     = (it->second).hwBpId;
        switch( bAttr ) {
        case BRK_DATA_READ:  mHitHwBpAttr = 1; break; // Read
        case BRK_DATA_WRITE: mHitHwBpAttr = 2; break; // Write
        default: 
          char msg[32];
          sprintf(msg, "Invalid HW break attr(%d).", bAttr);
          ErrMsg("Rteserv2IF", "Rteserv2IF::checkAccessBreak", msg);
          break;
        }
        (it->second).hitCount = 0;
        break;
      }
    }
    it++;
  }

  dmsg("  return " << mHitHwBpAcFlag);
  return mHitHwBpAcFlag;
}
