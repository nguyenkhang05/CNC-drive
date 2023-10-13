/*
 * Copyright(c) 2012 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#ifndef SYSREG_INFO_H
#define SYSREG_INFO_H

#include <map>

// �V�X�e�����W�X�^���N���X
class SysRegInfo {
public:
  SysRegInfo( void );
  ~SysRegInfo( void );
  void setSysRegInfo( unsigned int  ghsRegId,
                      unsigned int  issRegId,
                      char*         pName,
                      char*         pContext);
  void getSysRegInfo( unsigned int  ghsRegId,
                      unsigned int* pIssRegId,
                      char*         pName,
                      char*         pContext);
  char*        getSysRegName( unsigned int ghsRegId );
  char*        getSysRegContext( unsigned int ghsRegId );
  unsigned int getIssSysRegId( unsigned int ghsRegId );
  unsigned int getSysRegTotalNum( void );

private:
  struct SysRegData {
    unsigned int mIssRegId;   // ISS�̃��W�X�^ID
    char         mName[32];   // ���W�X�^��
    char         mContext[1]; // �R���e�L�X�g(V/T/-)
  };
  // �V�X�e�����W�X�^����ێ�����f�[�^�x�[�X
  std::map<unsigned int, SysRegData> mSysRegDataBase;
};

#endif // SYSREG_INFO_H
