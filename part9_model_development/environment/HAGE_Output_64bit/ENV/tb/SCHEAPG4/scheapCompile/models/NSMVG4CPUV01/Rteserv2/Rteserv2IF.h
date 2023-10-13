/*
 * Copyright(c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#ifndef RTESERV2_IF_H
#define RTESERV2_IF_H
#include <set>
#include <map>

class  NSMVG4CPUV01;
class  Rteserv2Controller;
class  ForestUtilApi;
//class  FastIssIF; // for FastISS
struct BreakInfo;

class Rteserv2IF { // HT毎に用意する必要がある処理。
  struct HwBpInfTyp; // 1つのHWブレークポイントの情報をもつ構造体

public:
  Rteserv2IF( void );
  ~Rteserv2IF( void );

  void setRteserv2ControllerPtr( Rteserv2Controller* pRteserv2Controller );
  void setCpuInfo( unsigned int peId, unsigned int tcId );
  // VCIDは、エラボレーション時にはまだ確定しないため設定しない。

  void                setPcBreak( unsigned int addr );
  void                rmvPcBreak( unsigned int addr );
  void                setHwBreakEx( unsigned int  cpuId,
                                    unsigned int  addr,
                                    unsigned int  addrMask,
                                    unsigned int  count,
                                    unsigned int* pHwBrkId );
  void                setHwBreakAc( unsigned int  cpuId,
                                    unsigned int  addr,
                                    unsigned int  addrMask,
                                    AccessAttr    attr,
                                    unsigned int  size,
                                    unsigned int  count,
                                    unsigned int* pHwBrkId );
  bool                rmvHwBreak( unsigned int  hwBrkId );
  bool                rmvAllHwBreak( void );
  bool                checkBreak( void );
  void                setRunState( void );
  void                setStepState( void );
  void                setHaltState( void );
  unsigned int        getId( void );
  unsigned int        getPC( void );
  dbgTyp::SIM_STATE_T getSimState( void );
  bool                isHitPcBp( void );
  bool                isSetPcBp( unsigned int addr );
  bool                isHitHwBpEx( void );
  bool                isHitHwBpAc( void );
  bool                isHitHwBp( unsigned int* pHwBrkId, unsigned int* pAttr );
  bool                isHaltState( void );
  void                setPcBpFlag( bool val );
  void                setHwBpExFlag( bool val );
  void                setHwBpAcFlag( bool val );

private:
  NSMVG4CPUV01*      mpG4Cpu;       // NSMVG4CPUV01階層のポインタ
  Rteserv2Controller* mpRteserv2Controller; // Rteserv2Controllerのポインタ
  unsigned int        mCpuId; // 自モジュールのCPU ID(peid, vcid, tcid, v/n を結合したもの)
  ForestUtilApi*      mpForestUtil;   // CForest の APIクラスのポインタ
/*  FastIssIF*          mpFastIssIf;  FastISS の APIクラスのポインタ for FastISS */
  BreakInfo*          mpBreakInfoCA;  // CForest のブレーク情報クラスのポインタ
/*  BreakInfo*          mpBreakInfoFast;  FastISS のブレーク情報クラスのポインタ for FastISS */
  unsigned int        mPeId;          // 担当するスレッドが所属するPEID
  unsigned int        mVcId;          // 担当するスレッドが所属するVCID
  unsigned int        mTcId;          // 担当するスレッドのTCID
  bool                mVNFlag;        // 担当するスレッドのフラグ(0:ネイティブ、 1:仮想)
  unsigned int        mHwBpIdCnt;     // HWブレークに新しいID番号をつける為のカウンタ
  unsigned int        mHitHwBpId;     // ヒットしたHWブレークのブレークポイントのID
  unsigned int        mHitHwBpAttr;   // ヒットしたHWブレークの属性(Read/Write/RW/実行)
  bool                mHitPcBpFlag;   // PCブレークにヒットした事を示すフラグ
  bool                mHitHwBpExFlag; // HWブレーク(実行)にヒットした事を示すフラグ
  bool                mHitHwBpAcFlag; // HWブレーク(アクセス)にヒットした事を示すフラグ
  dbgTyp::SIM_STATE_T mExecStatus;    // 担当するスレッドのステータス

  std::set<unsigned int>             mPcBpLst; // 設定したPCブレークのリスト
  std::map<unsigned int, HwBpInfTyp> mHwBpLst; // 設定したHWブレークのリスト

  bool                checkExecBreak  ( unsigned int      addr );
  bool                checkAccessBreak( unsigned int      addr,
                                        BreakHitAttribute attr,
                                        unsigned int      size );
};

#endif // RTESERV2_IF_H
