/*
 * Copyright(c) 2012 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <cstring>
#include "SysRegInfo.h"


///<summary>�R���X�g���N�^</summary>
///<param name="�Ȃ�"></param>
///<returns>�Ȃ�</returns>
SysRegInfo::SysRegInfo( void )
{
}



///<summary>�f�X�g���N�^</summary>
///<param name="�Ȃ�"></param>
///<returns>�Ȃ�</returns>
SysRegInfo::~SysRegInfo( void )
{
}



///<summary>�V�X�e�����W�X�^���ݒ�֐�</summary>
///<param name="ghsRegId">rteserv2���Œ�`���ꂽ���W�X�^ID</param>
///<param name="issRegId">ISS���Œ�`���ꂽ���W�X�^ID</param>
///<param name="pName">���W�X�^��</param>
///<param name="pContext">�R���e�L�X�g</param>
///<returns>�Ȃ�</returns>
void
SysRegInfo::setSysRegInfo(
  unsigned int ghsRegId,
  unsigned int issRegId,
  char*        pName,
  char*        pContext )
{
  SysRegData sRegData;

  sRegData.mIssRegId = issRegId;
  strcpy(sRegData.mName, pName);
  strcpy(sRegData.mContext, pContext);

  mSysRegDataBase.insert( std::map<unsigned int, SysRegData>::value_type(ghsRegId, sRegData) );
}



///<summary>�V�X�e�����W�X�^����Ԃ��֐�</summary>
///<param name="ghsRegId">rteserv2���Œ�`���ꂽ���W�X�^ID</param>
///<param name="issRegId">ISS���Œ�`���ꂽ���W�X�^ID</param>
///<param name="pName">���W�X�^��</param>
///<param name="pContext">�R���e�L�X�g</param>
///<returns>�Ȃ�</returns>
void
SysRegInfo::getSysRegInfo(
  unsigned int  ghsRegId,
  unsigned int* pIssRegId,
  char*         pName,
  char*         pContext )
{
  std::map<unsigned int, SysRegData>::iterator it = mSysRegDataBase.find( ghsRegId );

  *pIssRegId = it->second.mIssRegId;
  strcpy(pName, it->second.mName);
  strcpy(pContext, it->second.mContext);
}



///<summary>�V�X�e�����W�X�^����Ԃ��֐�</summary>
///<param name="ghsRegId">rteserv2���Œ�`���ꂽ���W�X�^ID</param>
///<returns>�V�X�e�����W�X�^��</returns>
char*
SysRegInfo::getSysRegName( unsigned int ghsRegId )
{
  char* ret = "";
  std::map<unsigned int, SysRegData>::iterator it = mSysRegDataBase.find( ghsRegId );
  
  if ( mSysRegDataBase.end() != it ) {
    ret = it->second.mName;
  }

  return ret;
}



///<summary>�V�X�e�����W�X�^�̃R���e�L�X�g��Ԃ��֐�</summary>
///<param name="ghsRegId">rteserv2���Œ�`���ꂽ���W�X�^ID</param>
///<returns>�V�X�e�����W�X�^�̃R���e�L�X�g</returns>
char*
SysRegInfo::getSysRegContext( unsigned int ghsRegId )
{
  char* ret = "";
  std::map<unsigned int, SysRegData>::iterator it = mSysRegDataBase.find( ghsRegId );
  
  if ( mSysRegDataBase.end() != it ) {
    ret = it->second.mContext;
  }

  return ret;
}



///<summary>ISS���Œ�`���ꂽ�V�X�e�����W�X�^��ID��Ԃ��֐�</summary>
///<param name="ghsRegId">rteserv2���Œ�`���ꂽ���W�X�^ID</param>
///<returns>ISS���Œ�`���ꂽ�V�X�e�����W�X�^��ID</returns>
unsigned int
SysRegInfo::getIssSysRegId( unsigned int ghsRegId )
{
  unsigned int ret = 0;
  std::map<unsigned int, SysRegData>::iterator it = mSysRegDataBase.find( ghsRegId );

  if ( it == mSysRegDataBase.end() ) {
    ret = 0;
  }
  else {
    ret = it->second.mIssRegId;
  }

  return ret;
}



///<summary>�ݒ肵���V�X�e�����W�X�^�̑�����Ԃ��֐�</summary>
///<param name="�Ȃ�"></param>
///<returns>�ݒ肵���V�X�e�����W�X�^�̑���</returns>
unsigned int
SysRegInfo::getSysRegTotalNum( void )
{
  return mSysRegDataBase.size();
}

