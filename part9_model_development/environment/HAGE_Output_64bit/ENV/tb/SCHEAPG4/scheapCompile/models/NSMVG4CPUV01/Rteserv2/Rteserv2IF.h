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

class Rteserv2IF { // HT���ɗp�ӂ���K�v�����鏈���B
  struct HwBpInfTyp; // 1��HW�u���[�N�|�C���g�̏������\����

public:
  Rteserv2IF( void );
  ~Rteserv2IF( void );

  void setRteserv2ControllerPtr( Rteserv2Controller* pRteserv2Controller );
  void setCpuInfo( unsigned int peId, unsigned int tcId );
  // VCID�́A�G���{���[�V�������ɂ͂܂��m�肵�Ȃ����ߐݒ肵�Ȃ��B

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
  NSMVG4CPUV01*      mpG4Cpu;       // NSMVG4CPUV01�K�w�̃|�C���^
  Rteserv2Controller* mpRteserv2Controller; // Rteserv2Controller�̃|�C���^
  unsigned int        mCpuId; // �����W���[����CPU ID(peid, vcid, tcid, v/n ��������������)
  ForestUtilApi*      mpForestUtil;   // CForest �� API�N���X�̃|�C���^
/*  FastIssIF*          mpFastIssIf;  FastISS �� API�N���X�̃|�C���^ for FastISS */
  BreakInfo*          mpBreakInfoCA;  // CForest �̃u���[�N���N���X�̃|�C���^
/*  BreakInfo*          mpBreakInfoFast;  FastISS �̃u���[�N���N���X�̃|�C���^ for FastISS */
  unsigned int        mPeId;          // �S������X���b�h����������PEID
  unsigned int        mVcId;          // �S������X���b�h����������VCID
  unsigned int        mTcId;          // �S������X���b�h��TCID
  bool                mVNFlag;        // �S������X���b�h�̃t���O(0:�l�C�e�B�u�A 1:���z)
  unsigned int        mHwBpIdCnt;     // HW�u���[�N�ɐV����ID�ԍ�������ׂ̃J�E���^
  unsigned int        mHitHwBpId;     // �q�b�g����HW�u���[�N�̃u���[�N�|�C���g��ID
  unsigned int        mHitHwBpAttr;   // �q�b�g����HW�u���[�N�̑���(Read/Write/RW/���s)
  bool                mHitPcBpFlag;   // PC�u���[�N�Ƀq�b�g�������������t���O
  bool                mHitHwBpExFlag; // HW�u���[�N(���s)�Ƀq�b�g�������������t���O
  bool                mHitHwBpAcFlag; // HW�u���[�N(�A�N�Z�X)�Ƀq�b�g�������������t���O
  dbgTyp::SIM_STATE_T mExecStatus;    // �S������X���b�h�̃X�e�[�^�X

  std::set<unsigned int>             mPcBpLst; // �ݒ肵��PC�u���[�N�̃��X�g
  std::map<unsigned int, HwBpInfTyp> mHwBpLst; // �ݒ肵��HW�u���[�N�̃��X�g

  bool                checkExecBreak  ( unsigned int      addr );
  bool                checkAccessBreak( unsigned int      addr,
                                        BreakHitAttribute attr,
                                        unsigned int      size );
};

#endif // RTESERV2_IF_H
