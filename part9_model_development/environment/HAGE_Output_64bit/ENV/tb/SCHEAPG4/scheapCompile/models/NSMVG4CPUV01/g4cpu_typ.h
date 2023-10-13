/*
 * Copyright(c) 2015-2018 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#ifndef G4CPU_TYP_H
#define G4CPU_TYP_H

typedef struct {
  unsigned int  peid_ival;     // Initial value of PEID (default value = index value)
  unsigned int  bmid_ival;     // Initial value of Bus Master ID (default value = index value)
  unsigned int  spid_ival;     // Initial value of SPID (default value = index value)
  unsigned int  gmspid_ival;   // Initial value of GMSPID (default value = index value)
  unsigned int  spidlist_ival; // Initial value of SPIDLIST (default value = 0)
  unsigned int  rbase_ival;    // Initial value of RBASE (default value = 0)
  unsigned int  snzcfg_ival;   // Initial value of SNZCFG (default value = 32)
  unsigned int  hvcfg_ival;    // Initial value of HVCFG (default value = 0)
  unsigned int  ic_size;       // I-Cache size (default value = 16)
  unsigned int  mpu_entry;     // MPU entry number (default value = 24)
}g4cpu_pe_info;

typedef struct {
  unsigned int  cramif_width;
  unsigned char crg_ch_num;
  unsigned char csg_ch_num;
  unsigned int  keycode;
}g4cpu_crg_info;

typedef enum {
  CPU_G4MH_1_0,
  CPU_G4MH_1_1,
  CPU_G4MH_2_0
} CPU_KIND_T;



#endif // G4CPU_TYP_H
