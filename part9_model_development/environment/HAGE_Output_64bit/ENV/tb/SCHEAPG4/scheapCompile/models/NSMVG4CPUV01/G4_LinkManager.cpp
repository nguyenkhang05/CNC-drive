/*************************************************************************
 *
 *  G4_LinkManager.cpp
 *
 * Copyright(c) 2016, 2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

/* $Id:$ */

#include <string.h>  // for strcpy, strtok, strcmp, strncmp
#include <stdio.h>   // for sprintf
#include <errno.h>   // for errno
#include <sstream>
#include <iostream>

#include "G4_LinkManager.h"
#include "CmErrMsg.h" // for WinMsg

using namespace CmErrMsg; // for WinMsg

///<summary>constructor</summary>
///<returns>none</returns>
G4_LinkManager::G4_LinkManager( void ):
  mPeNum(PE_MAX_NUM)
{
  ResetLink();
}


///<summary>destructor</summary>
///<returns>none</returns>
G4_LinkManager::~G4_LinkManager( void )
{
}


///<summary>set effective PE number</summary>
///<param name="pe_num">effective PE number</param>
///<returns>none</returns>
void G4_LinkManager::SetPeNum(unsigned int pe_num)
{
  mPeNum = pe_num;
}


///<summary>reset operation</summary>
///<returns>none</returns>
void G4_LinkManager::ResetLink( void )
{
  for(int i=0; i<PE_MAX_NUM; i++){
    mLLbit[i].ll_tcm = false;
    mLLbit[i].ll_vci = false;

    for(int j=0; j<TCM_ENTRY_NUM; j++){
      mTcmMon[i].valid[j] = false;
    }
  }

  for(int i=0; i<CRAM_BANK_NUM; i++){
    for(int j=0; j<PE_MAX_NUM; j++){
      mCramMon[i].valid[j] = false;
    }
  }
}


///<summary>set link</summary>
///<param name="peid">PE ID</param>
///<param name="addr">address</param>
///<param name="acc_size">access size</param>
///<returns>none</returns>
void G4_LinkManager::SetLink(unsigned char peid, unsigned int addr, unsigned int acc_size)
{
  if(peid>PE_MAX_NUM){
#ifdef TEST_LINK_DBG
    printf("%s: G4_LinkManager::SetLink: peid=%d is invalid id\n", 
	   sc_time_stamp().to_string().c_str(), peid);
    fflush(stdout);
#endif
    return;
  }

  MEM_AREA_T area_type = GetAddressArea(addr);
  if(area_type == MEM_OTHER){
    // not set link
#ifdef TEST_LINK_DBG
    printf("%s: G4_LinkManager::SetLink: access master PE%d: 0x%08x is not TCM and CRAM\n", 
	   sc_time_stamp().to_string().c_str(), peid, addr);
    fflush(stdout);
#endif

  }else if((area_type == MEM_CRAM_BK0) || (area_type == MEM_CRAM_BK1)){
#ifdef TEST_LINK_DBG
    printf("%s: G4_LinkManager::SetLink: access master PE%d: 0x%08x is CRAM area\n", 
	   sc_time_stamp().to_string().c_str(), peid, addr);
    fflush(stdout);
#endif
    unsigned int bk_num;
    unsigned int entry;

    if(area_type == MEM_CRAM_BK0) bk_num = 0;
    else                          bk_num = 1;
    
    // set ll-vci
    mLLbit[peid].ll_vci = true;
    
    // get entry number of CRAM monitor
    entry = GetCramHitEntry(bk_num, addr);
    if(entry == ENTRY_NONE){
      // the address isn't registerd at CRAM monitor
      entry = GetCramEntry(peid, bk_num);
    }

    // set Cram Monitor
    mCramMon[bk_num].peid[entry]  = peid;
    mCramMon[bk_num].htid[entry]  = 0;
    mCramMon[bk_num].addr[entry]  = addr;
    mCramMon[bk_num].acc_size[entry]  = acc_size;
    mCramMon[bk_num].valid[entry] = true;

  }else{
#ifdef TEST_LINK_DBG
    printf("%s: G4_LinkManager::SetLink: access master PE%d: 0x%08x is PE%d's TCM area\n", 
	   sc_time_stamp().to_string().c_str(), peid, addr, area_type);
    fflush(stdout);
#endif
    unsigned char tcm_master;;
    if((area_type == MEM_TCM_SLF) || (area_type == peid)){
      // set ll-tcm
      mLLbit[peid].ll_tcm = true;
      tcm_master = peid; 
    }else{
      // set ll-vci
      mLLbit[peid].ll_vci = true;
      tcm_master = area_type;
    }

    // get entry number of TCM monitor
    unsigned int entry = GetTcmHitEntry(tcm_master, addr);
    if(entry == ENTRY_NONE){
      // the address isn't registerd at TCM monitor
      entry = GetTcmEntry(tcm_master, peid);
    }

    // set TCM monitor
    mTcmMon[tcm_master].peid[entry]  = peid;
    mTcmMon[tcm_master].htid[entry]  = 0;
    mTcmMon[tcm_master].addr[entry]  = addr;
    mTcmMon[tcm_master].acc_size[entry]  = acc_size;
    mTcmMon[tcm_master].valid[entry] = true;
  }  
}


///<summary>get link status</summary>
///<param name="peid">PE ID</param>
///<param name="addr">address</param>
///<param name="acc_size">access size</param>
///<returns>link status(true/false)</returns>
bool G4_LinkManager::IsLinkValid(unsigned char peid, unsigned int addr, unsigned int acc_size)
{
  if(peid>PE_MAX_NUM){
#ifdef TEST_LINK_DBG
    printf("%s: G4_LinkManager::IsLinkValid: peid=%d is invalid id\n", 
	   sc_time_stamp().to_string().c_str(), peid);
    fflush(stdout);
#endif
    return(false);
  }

  MEM_AREA_T area_type = GetAddressArea(addr);
  if(area_type == MEM_OTHER){
#ifdef TEST_LINK_DBG
    printf("%s: G4_LinkManager::IsLinkValid: access master PE%d: 0x%08x is not TCM and CRAM\n", 
	   sc_time_stamp().to_string().c_str(), peid, addr);
    fflush(stdout);
#endif
    // not set link
    return(false);
  }else if((area_type == MEM_CRAM_BK0) || (area_type == MEM_CRAM_BK1)){
#ifdef TEST_LINK_DBG
    printf("%s: G4_LinkManager::IsLinkValid: access master PE%d: 0x%08x is CRAM area\n", 
	   sc_time_stamp().to_string().c_str(), peid, addr);
    fflush(stdout);
#endif
    if(!mLLbit[peid].ll_vci){
      // link is invalid
#ifdef TEST_LINK_DBG
      printf("%s: G4_LinkManager::IsLinkValid: PE%d: ll-vci(CRAM) is invalid\n", 
	     sc_time_stamp().to_string().c_str(), peid);
      fflush(stdout);
#endif
      return(false);
    }else{
      // ll-vci is valid
      unsigned int bk_num;
      unsigned int entry;
      if(area_type == MEM_CRAM_BK0) bk_num = 0;
      else                          bk_num = 1;

      entry = GetCramHitEntry(bk_num, addr);
      if((entry != ENTRY_NONE) && (mCramMon[bk_num].peid[entry] == peid) && (mCramMon[bk_num].acc_size[entry] == acc_size)){
	// link is valid
#ifdef TEST_LINK_DBG
	printf("%s: G4_LinkManager::IsLinkValid: PE%d: CRAM link is valid(Hit!!)\n", 
	       sc_time_stamp().to_string().c_str(), peid);
	fflush(stdout);
#endif
	return(true);
      }
      // link is invalid
#ifdef TEST_LINK_DBG
      printf("%s: G4_LinkManager::IsLinkValid: PE%d: CRAM link is invalid\n", 
	     sc_time_stamp().to_string().c_str(), peid);
      fflush(stdout);
#endif
      return(false);
    }
  }else{
    unsigned char tcm_master;;
    if((area_type == MEM_TCM_SLF) || (area_type == peid)){
#ifdef TEST_LINK_DBG
      printf("%s: G4_LinkManager::IsLinkValid: access master PE%d: 0x%08x is TCM area\n", 
	     sc_time_stamp().to_string().c_str(), peid, addr);
      fflush(stdout);
#endif
      if(!mLLbit[peid].ll_tcm){
#ifdef TEST_LINK_DBG
	printf("%s: G4_LinkManager::IsLinkValid: PE%d: ll-tcm is invalid\n", 
	       sc_time_stamp().to_string().c_str(), peid);
	fflush(stdout);
#endif
	// link is invalid
	return(false);
      }else{
#ifdef TEST_LINK_DBG
	printf("%s: G4_LinkManager::IsLinkValid: PE%d: ll-tcm is valid\n", 
	       sc_time_stamp().to_string().c_str(), peid);
	fflush(stdout);
#endif
	// ll-tcm is valid
	tcm_master = peid;
      }
    }else{
      if(!mLLbit[peid].ll_vci){
#ifdef TEST_LINK_DBG
	printf("%s: G4_LinkManager::IsLinkValid: PE%d: ll-vci(TCM) is invalid\n", 
	       sc_time_stamp().to_string().c_str(), peid);
	fflush(stdout);
#endif
	// link is invalid
	return(false);
      }else{
#ifdef TEST_LINK_DBG
	printf("%s: G4_LinkManager::IsLinkValid: PE%d: ll-vci(TCM) is valid\n", 
	       sc_time_stamp().to_string().c_str(), peid);
	fflush(stdout);
#endif
	// ll-vci is valid
	tcm_master = area_type;
      }
    }
    unsigned int entry = GetTcmHitEntry(tcm_master, addr);
    if((entry != ENTRY_NONE) && (mTcmMon[tcm_master].peid[entry] == peid) && (mTcmMon[tcm_master].acc_size[entry] == acc_size)){
#ifdef TEST_LINK_DBG
      printf("%s: G4_LinkManager::IsLinkValid: PE%d: TCM link is valid(Hit!!)\n", 
	     sc_time_stamp().to_string().c_str(), peid);
      fflush(stdout);
#endif
      // link is valid
      return(true);
    }
#ifdef TEST_LINK_DBG
    printf("%s: G4_LinkManager::IsLinkValid: PE%d: TCM link is invalid\n", 
	   sc_time_stamp().to_string().c_str(), peid);
      fflush(stdout);
#endif
    // link is invalid
    return(false);
  }  
}


///<summary>clear the link</summary>
///<param name="peid">PE ID</param>
///<param name="addr">address</param>
///<param name="acc_size">access address</param>
///<returns>none</returns>
void G4_LinkManager::ClearLink(unsigned char peid, unsigned int addr, unsigned int acc_size)
{
  if(peid>PE_MAX_NUM){
#ifdef TEST_LINK_DBG
    printf("%s: G4_LinkManager::ClearLink: peid=%d is invalid id\n", 
	   sc_time_stamp().to_string().c_str(), peid);
    fflush(stdout);
#endif
    return;
  }

  MEM_AREA_T area_type = GetAddressArea(addr);
  if(area_type == MEM_OTHER){
#ifdef TEST_LINK_DBG
    printf("%s: G4_LinkManager::ClearLink: access master PE%d: 0x%08x is not TCM and CRAM\n", 
	   sc_time_stamp().to_string().c_str(), peid, addr);
    fflush(stdout);
#endif
    // not set link
    return;
  }else if((area_type == MEM_CRAM_BK0) || (area_type == MEM_CRAM_BK1)){
#ifdef TEST_LINK_DBG
    printf("%s: G4_LinkManager::ClearLink: access master PE%d: 0x%08x is CRAM area\n", 
	   sc_time_stamp().to_string().c_str(), peid, addr);
    fflush(stdout);
#endif
    if(mLLbit[peid].ll_vci){
      unsigned int bk_num;
      unsigned int entry;

      mLLbit[peid].ll_vci = false;
#ifdef TEST_LINK_DBG
      printf("%s: G4_LinkManager::ClearLink: PE%d: clear the mLLbit[%d].ll_vci\n", 
	     sc_time_stamp().to_string().c_str(), peid, peid);
      fflush(stdout);
#endif

      if(area_type == MEM_CRAM_BK0) bk_num = 0;
      else                          bk_num = 1;
      entry = GetCramHitEntry(bk_num, addr);
      if(entry != ENTRY_NONE){
	if((mCramMon[bk_num].peid[entry] == peid) && (mCramMon[bk_num].acc_size[entry] == acc_size)){
	  ClearCramMonitor(bk_num, addr);
	}
      }
    }
  }else{
#ifdef TEST_LINK_DBG
    printf("%s: G4_LinkManager::ClearLink: access master PE%d: 0x%08x is TCM area\n", 
	   sc_time_stamp().to_string().c_str(), peid, addr);
    fflush(stdout);
#endif
    unsigned char tcm_master;;
    if((area_type == MEM_TCM_SLF) || (area_type == peid)){
      tcm_master = peid; 
      if(mLLbit[peid].ll_tcm){
	mLLbit[peid].ll_tcm = false;
#ifdef TEST_LINK_DBG
	printf("%s: G4_LinkManager::ClearLink: PE%d: clear the mLLbit[%d].ll_tcm\n", 
	       sc_time_stamp().to_string().c_str(), peid, peid);
	fflush(stdout);
#endif
      }else{
	return;
      }
    }else{
      tcm_master = area_type;
      if(mLLbit[peid].ll_vci){
	mLLbit[peid].ll_vci = false;
#ifdef TEST_LINK_DBG
	printf("%s: G4_LinkManager::ClearLink: PE%d: clear the mLLbit[%d].ll_vci(TCM)\n", 
	       sc_time_stamp().to_string().c_str(), peid, peid);
	fflush(stdout);
#endif
      }else{
	return;
      }
    }

    unsigned int entry = GetTcmHitEntry(tcm_master, addr);
    if(entry != ENTRY_NONE){
      if((mTcmMon[tcm_master].peid[entry] == peid) && (mTcmMon[tcm_master].acc_size[entry] == acc_size)){
	ClearTcmMonitor(tcm_master, addr);
      }
    }
  }  
}


///<summary>clear the peid's ll-bit</summary>
///<param name="peid">PE ID</param>
///<returns>none</returns>
void G4_LinkManager::ClearLlbit(unsigned char peid)
{
  if(peid>PE_MAX_NUM){
#ifdef TEST_LINK_DBG
    printf("%s: G4_LinkManager::ClearLlbit: peid=%d is invalid id\n", 
	   sc_time_stamp().to_string().c_str(), peid);
    fflush(stdout);
#endif
    return;
  }

#ifdef TEST_LINK_DBG
  printf("%s: G4_LinkManager::ClearLlbit: PE%d: clear the mLLbit[%d].ll_vci and ll_tcm\n", 
	 sc_time_stamp().to_string().c_str(), peid, peid);
  fflush(stdout);
#endif
  mLLbit[peid].ll_tcm = false;
  mLLbit[peid].ll_vci = false;
}


///<summary>clear the link Monitor</summary>
///<param name="peid">PE ID</param>
///<param name="addr">address</param>
///<param name="size">access size(byte)</param>
///<returns>none</returns>
void G4_LinkManager::ClearMonitor(unsigned char peid, unsigned int addr, unsigned int size)
{
  bool isDword = false;

  if(peid>PE_MAX_NUM){
#ifdef TEST_LINK_DBG
    printf("%s: G4_LinkManager::ClearMonitor: peid=%d is invalid id\n", 
	   sc_time_stamp().to_string().c_str(), peid);
    fflush(stdout);
#endif
    return;
  }

  MEM_AREA_T area_type = GetAddressArea(addr);

  if(size > 4) isDword = true;

  if(area_type == MEM_OTHER){
    // not set link
#ifdef TEST_LINK_DBG
    printf("%s: G4_LinkManager::ClearMonitor: 0x%08x is not TCM and CRAM\n", 
	   sc_time_stamp().to_string().c_str(), addr);
    fflush(stdout);
#endif
  }else if((area_type == MEM_CRAM_BK0) || (area_type == MEM_CRAM_BK1)){
    unsigned int bk_num;

    if(area_type == MEM_CRAM_BK0) bk_num = 0;
    else                          bk_num = 1;
    ClearCramMonitor(bk_num, addr);
    if(isDword){
      ClearCramMonitor(bk_num, addr+4);
    }
  }else{
    unsigned char tcm_master;;
    if(area_type == MEM_TCM_SLF){
      tcm_master = peid; 
    }else{
      tcm_master = area_type;
    }
    ClearTcmMonitor(tcm_master, addr);
    if(isDword){
      ClearTcmMonitor(tcm_master, addr+4);
    }
  }
}


///<summary>get address area</summary>
///<param name="addr">address</param>
///<returns>address area(TCM/CRAM/OTHER)</returns>
G4_LinkManager::MEM_AREA_T G4_LinkManager::GetAddressArea(unsigned int addr)
{
  if((0xfe000000 <= addr) && (addr < 0xff000000)){
    return(MEM_CRAM_BK0);
//}else if((0xfef00000 <= addr) && (addr < 0xff000000)){
//  return(MEM_CRAM_BK1);
  }else if((0xfde00000 <= addr) && (addr < 0xfe000000)){
    return(MEM_TCM_SLF);
  }else if((0xfdc00000 <= addr) && (addr < 0xfde00000)){
    return(MEM_TCM_PE0);
  }else if((0xfda00000 <= addr) && (addr < 0xfdc00000)){
    return(MEM_TCM_PE1);
  }else if((0xfd800000 <= addr) && (addr < 0xfda00000)){
    return(MEM_TCM_PE2);
  }else if((0xfd600000 <= addr) && (addr < 0xfd800000)){
    return(MEM_TCM_PE3);
  }else if((0xfd400000 <= addr) && (addr < 0xfd600000)){
    return(MEM_TCM_PE4);
  }else if((0xfd200000 <= addr) && (addr < 0xfd400000)){
    return(MEM_TCM_PE5);
  }else if((0xfd000000 <= addr) && (addr < 0xfd200000)){
    return(MEM_TCM_PE6);
  }else if((0xfce00000 <= addr) && (addr < 0xfd000000)){
    return(MEM_TCM_PE7);
  }else{
    return(MEM_OTHER);
  }
}


///<summary>get entry number to set the new entry</summary>
///<param name="peid">access master's PE ID</param>
///<param name="bk_num">bank number</param>
///<returns>entry number</returns>
unsigned int G4_LinkManager::GetCramEntry(unsigned char peid, unsigned int bk_num)
{
  unsigned int invalid_entry  = ENTRY_NONE; 
  unsigned int existing_entry = ENTRY_NONE;
  for(unsigned int i=0; i<mPeNum; i++){
    if((mCramMon[bk_num].valid[i] == false) &&
       (invalid_entry == ENTRY_NONE)){
      invalid_entry = i;
    }
    if((mCramMon[bk_num].peid[i] == peid) &&
       (mCramMon[bk_num].valid[i] == true)){
      existing_entry = i;
      break;
    }
  }
  if(existing_entry < mPeNum){
#ifdef TEST_LINK_DBG
    printf("%s: G4_LinkManager::GetCramEntry: PE%d is registered at entry No.%d of mCramMon[%d].\n", 
	   sc_time_stamp().to_string().c_str(), peid, existing_entry, bk_num);
    fflush(stdout);
#endif
    return(existing_entry);
  }else if(invalid_entry < mPeNum){
#ifdef TEST_LINK_DBG
    printf("%s: G4_LinkManager::GetCramEntry: PE%d isn't registered in mCramMon[%d]. Invalid entry is %d.\n", 
	   sc_time_stamp().to_string().c_str(), peid, bk_num, invalid_entry);
    fflush(stdout);
#endif
    return(invalid_entry);
  }else{
#ifdef TEST_LINK_DBG
    printf("%s: G4_LinkManager::GetCramEntry: PE%d isn't registered in mCramMon[%d]. Invalid entry is None. Set entry 0.\n", 
	   sc_time_stamp().to_string().c_str(), peid, bk_num);
    fflush(stdout);
#endif
    return(0);
  }
}


///<summary>get entry number to set the new entry</summary>
///<param name="tcm_m_peid">TCM master's PE ID</param>
///<param name="acc_m_peid">access master's PE ID</param>
///<returns>entry number</returns>
unsigned int G4_LinkManager::GetTcmEntry(unsigned char tcm_m_peid, unsigned char acc_m_peid)
{
  unsigned int invalid_entry  = ENTRY_NONE;
  unsigned int existing_entry = ENTRY_NONE;
  for(int i=0; i<TCM_ENTRY_NUM; i++){
    if((mTcmMon[tcm_m_peid].valid[i] == false) &&
       (invalid_entry == ENTRY_NONE)){
      invalid_entry = i;
    }
    if((mTcmMon[tcm_m_peid].peid[i] == acc_m_peid) &&
       (mTcmMon[tcm_m_peid].valid[i] == true)){
      existing_entry = i;
      break;
    }
  }
  if(existing_entry < TCM_ENTRY_NUM){
#ifdef TEST_LINK_DBG
    printf("%s: G4_LinkManager::GetTcmEntry: PE%d is registered at entry No.%d of mTcmMon[%d].\n", 
	   sc_time_stamp().to_string().c_str(), acc_m_peid, existing_entry, tcm_m_peid);
    fflush(stdout);
#endif
    return(existing_entry);
  }else if(invalid_entry < TCM_ENTRY_NUM){
#ifdef TEST_LINK_DBG
    printf("%s: G4_LinkManager::GetTcmEntry: PE%d isn't registered in mTcmMon[%d]. Invalid entry is %d.\n", 
	   sc_time_stamp().to_string().c_str(), acc_m_peid, tcm_m_peid, invalid_entry);
    fflush(stdout);
#endif
    return(invalid_entry);
  }else{
#ifdef TEST_LINK_DBG
    printf("%s: G4_LinkManager::GetTcmEntry: PE%d isn't registered in mTcmMon[%d]. Invalid entry is None. Set entry 0.\n", 
	   sc_time_stamp().to_string().c_str(), acc_m_peid, tcm_m_peid);
    fflush(stdout);
#endif
    return(0);
  }
}


///<summary>get entry number of address hit</summary>
///<param name="bk_num">bank number</param>
///<param name="addr">address</param>
///<returns>entry number</returns>
unsigned int G4_LinkManager::GetCramHitEntry(unsigned int bk_num, unsigned int addr)
{
  unsigned int entry = ENTRY_NONE;
  for(unsigned int i=0; i<mPeNum; i++){
    if((mCramMon[bk_num].addr[i] == addr) &&
       (mCramMon[bk_num].valid[i] == true)){
#ifdef TEST_LINK_DBG
      printf("%s: G4_LinkManager::GetCramHitEntry: 0x%08x is registered at entry No.%d of mCramMon[%d].\n", 
	     sc_time_stamp().to_string().c_str(), addr, i , bk_num);
      fflush(stdout);
#endif
      return(i);
    }
  }
#ifdef TEST_LINK_DBG
  printf("%s: G4_LinkManager::GetCramHitEntry: 0x%08x isn't registered in mCramMon[%d].\n", 
	 sc_time_stamp().to_string().c_str(), addr, bk_num);
  fflush(stdout);
#endif
  return(ENTRY_NONE);
}


///<summary>get entry number of address hit</summary>
///<param name="peid">TCM master's PE ID</param>
///<param name="addr">address</param>
///<returns>entry number</returns>
unsigned int G4_LinkManager::GetTcmHitEntry(unsigned char peid, unsigned int addr)
{
  unsigned int entry = ENTRY_NONE;
  for(int i=0; i<TCM_ENTRY_NUM; i++){
    if((mTcmMon[peid].addr[i] == addr) &&
       (mTcmMon[peid].valid[i] == true)){
#ifdef TEST_LINK_DBG
      printf("%s: G4_LinkManager::GetTcmHitEntry: 0x%08x is registered at entry No.%d of mTcmMon[%d].\n", 
	     sc_time_stamp().to_string().c_str(), addr, i , peid);
      fflush(stdout);
#endif
      return(i);
    }
  }
#ifdef TEST_LINK_DBG
  printf("%s: G4_LinkManager::GetTcmHitEntry: 0x%08x isn't registered in mTcmMon[%d].\n", 
	 sc_time_stamp().to_string().c_str(), addr, peid);
  fflush(stdout);
#endif
  return(ENTRY_NONE);
}


///<summary>clear the Cluster RAM Monitor</summary>
///<param name="bknum">bank number</param>
///<param name="addr">address</param>
///<returns>none</returns>
void G4_LinkManager::ClearCramMonitor(unsigned int bknum, unsigned int addr)
{
#ifdef TEST_LINK_DBG
  printf("%s: G4_LinkManager::ClearCramMonitor: check 0x%08x\n", 
	 sc_time_stamp().to_string().c_str(), addr);
  fflush(stdout);
#endif
  for(unsigned int i=0; i<mPeNum; i++){
    if((((mCramMon[bknum].addr[i] & 0xffffffe0) <= addr) && (addr < ((mCramMon[bknum].addr[i] & 0xffffffe0) + 0x20))) &&
       (mCramMon[bknum].valid[i] == true)){
#ifdef TEST_LINK_DBG
      printf("     The entry No.%d of mCramMon[%d].\n", i, bknum);
      fflush(stdout);
#endif
      mCramMon[bknum].valid[i] = false;
      mCramMon[bknum].peid[i]  = 0;
    }
  }
}

///<summary>clear the TCM Monitor</summary>
///<param name="peid">PE ID</param>
///<param name="addr">address</param>
///<returns>none</returns>
void G4_LinkManager::ClearTcmMonitor(unsigned char peid, unsigned int addr)
{
#ifdef TEST_LINK_DBG
  printf("%s: G4_LinkManager::ClearTcmMonitor: check 0x%08x\n", 
	 sc_time_stamp().to_string().c_str(), addr);
  fflush(stdout);
#endif
  for(int i=0; i<TCM_ENTRY_NUM; i++){
    if((((mTcmMon[peid].addr[i] & 0xffffffe0) <= addr) && (addr < ((mTcmMon[peid].addr[i] & 0xffffffe0) + 0x20))) &&
       (mTcmMon[peid].valid[i] == true)){
#ifdef TEST_LINK_DBG
      printf("     The entry No.%d of mTcmMon[%d].\n", i, peid);
      fflush(stdout);
#endif
      mTcmMon[peid].valid[i] = false;
      mTcmMon[peid].peid[i]  = 0;
    }
  }
}
