/*
* Copyright(c) 2016 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

/* $Id:$ */

#ifndef G4_LINKMANAGER_H
#define G4_LINKMANAGER_H

#include "global.h"

#define TCM_ENTRY_NUM           4
#define CRAM_BANK_NUM           2
#define ENTRY_NONE     0xffffffff

class G4_LinkManager
{
  
 public:

 protected:
  
 private:
  typedef enum {
    MEM_TCM_PE0 = 0,
    MEM_TCM_PE1 = 1,
    MEM_TCM_PE2 = 2,
    MEM_TCM_PE3 = 3,
    MEM_TCM_PE4 = 4,
    MEM_TCM_PE5 = 5,
    MEM_TCM_PE6 = 6,
    MEM_TCM_PE7 = 7,
    MEM_TCM_SLF = 8,
    MEM_CRAM_BK0 = 9,
    MEM_CRAM_BK1 = 10,
    MEM_OTHER    = 100
  } MEM_AREA_T;

  typedef struct{
    bool ll_tcm; // llbit for TCM
    bool ll_vci; // llbit for VCI
  }llbit_info;

  typedef struct{
    unsigned char peid[TCM_ENTRY_NUM];     // TCM monitor's entry for peid
    unsigned char htid[TCM_ENTRY_NUM];     // TCM monitor's entry for htid
    unsigned int  addr[TCM_ENTRY_NUM];     // TCM monitor's entry for address
    unsigned int  acc_size[TCM_ENTRY_NUM]; // TCM monitor's entry for access size
    bool          valid[TCM_ENTRY_NUM];    // TCM monitor's entry for valid flag
  }tcm_mon_info;

  typedef struct{
    unsigned char peid[PE_MAX_NUM];     // CRAM monitor's entry for peid
    unsigned char htid[PE_MAX_NUM];     // CRAM monitor's entry for htid
    unsigned int  addr[PE_MAX_NUM];     // CRAM monitor's entry for address
    unsigned int  acc_size[PE_MAX_NUM]; // CRAM monitor's entry for access size
    bool          valid[PE_MAX_NUM];    // CRAM monitor's entry for valid flag
  }cram_mon_info;

  llbit_info    mLLbit[PE_MAX_NUM];      // LLbit for link
  tcm_mon_info  mTcmMon[PE_MAX_NUM];     // TCM area monitor for link
  cram_mon_info mCramMon[CRAM_BANK_NUM]; // Cram area monitor for link

  unsigned int mPeNum; // effective PE number

  MEM_AREA_T   GetAddressArea(unsigned int addr);
  unsigned int GetCramHitEntry(unsigned int bk_num, unsigned int addr);
  unsigned int GetTcmHitEntry(unsigned char peid, unsigned int addr);
  unsigned int GetCramEntry(unsigned char peid, unsigned int bk_num);
  unsigned int GetTcmEntry(unsigned char tcm_m_peid, unsigned char acc_m_peid);
  void ClearCramMonitor(unsigned int bknum, unsigned int addr);
  void ClearTcmMonitor(unsigned char peid, unsigned int addr);

 public:
  G4_LinkManager( void );
  ~G4_LinkManager( void );

  void SetPeNum( unsigned int pe_num );
  void ResetLink( void );
  void SetLink(unsigned char peid, unsigned int addr, unsigned int acc_size=4);
  bool IsLinkValid(unsigned char peid, unsigned int addr, unsigned int acc_size=4);  
  void ClearLink(unsigned char peid, unsigned int addr, unsigned int acc_size=4);
  void ClearLlbit(unsigned char peid);
  void ClearMonitor(unsigned char peid, unsigned int addr, unsigned int size);
  
}; // end of G4_LinkManager definition

#endif /* G4_LINKMANAGER_H */
