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


///<summary>コンストラクタ</summary>
///<param name="なし"></param>
///<returns>なし</returns>
SysRegInfo::SysRegInfo( void )
{
}



///<summary>デストラクタ</summary>
///<param name="なし"></param>
///<returns>なし</returns>
SysRegInfo::~SysRegInfo( void )
{
}



///<summary>システムレジスタ情報設定関数</summary>
///<param name="ghsRegId">rteserv2側で定義されたレジスタID</param>
///<param name="issRegId">ISS側で定義されたレジスタID</param>
///<param name="pName">レジスタ名</param>
///<param name="pContext">コンテキスト</param>
///<returns>なし</returns>
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



///<summary>システムレジスタ情報を返す関数</summary>
///<param name="ghsRegId">rteserv2側で定義されたレジスタID</param>
///<param name="issRegId">ISS側で定義されたレジスタID</param>
///<param name="pName">レジスタ名</param>
///<param name="pContext">コンテキスト</param>
///<returns>なし</returns>
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



///<summary>システムレジスタ名を返す関数</summary>
///<param name="ghsRegId">rteserv2側で定義されたレジスタID</param>
///<returns>システムレジスタ名</returns>
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



///<summary>システムレジスタのコンテキストを返す関数</summary>
///<param name="ghsRegId">rteserv2側で定義されたレジスタID</param>
///<returns>システムレジスタのコンテキスト</returns>
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



///<summary>ISS側で定義されたシステムレジスタのIDを返す関数</summary>
///<param name="ghsRegId">rteserv2側で定義されたレジスタID</param>
///<returns>ISS側で定義されたシステムレジスタのID</returns>
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



///<summary>設定したシステムレジスタの総数を返す関数</summary>
///<param name="なし"></param>
///<returns>設定したシステムレジスタの総数</returns>
unsigned int
SysRegInfo::getSysRegTotalNum( void )
{
  return mSysRegDataBase.size();
}

