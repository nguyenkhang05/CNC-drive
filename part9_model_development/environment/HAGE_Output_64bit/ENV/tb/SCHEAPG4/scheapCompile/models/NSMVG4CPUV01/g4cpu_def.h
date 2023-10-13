/*
 * Copyright(c) 2016, 2018 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#ifndef G4CPU_DEF_H
#define G4CPU_DEF_H

#define FROM_INIT_F_LATENCY    12
#define FROM_INIT_R_LATENCY     8
#define FROM_INIT_W_LATENCY     8
#define FROM_INIT_CL_F_LATENCY  FROM_INIT_F_LATENCY
#define FROM_INIT_CL_R_LATENCY  FROM_INIT_R_LATENCY
#define FROM_INIT_CL_W_LATENCY  FROM_INIT_W_LATENCY
#define FROM_INIT_EX_F_LATENCY 12
#define FROM_INIT_EX_R_LATENCY  8
#define FROM_INIT_EX_W_LATENCY  8
#define CRAM_INIT_F_LATENCY    23
#define CRAM_INIT_R_LATENCY     5
#define CRAM_INIT_W_LATENCY     7
#define CRAM_INIT_CL_F_LATENCY  CRAM_INIT_F_LATENCY
#define CRAM_INIT_CL_R_LATENCY  CRAM_INIT_R_LATENCY
#define CRAM_INIT_CL_W_LATENCY  CRAM_INIT_W_LATENCY
#define CRAM_INIT_EX_F_LATENCY 23
#define CRAM_INIT_EX_R_LATENCY  5
#define CRAM_INIT_EX_W_LATENCY  7
#define LRAM_INIT_F_LATENCY    15
#define LRAM_INIT_R_LATENCY     2
#define LRAM_INIT_W_LATENCY     5
#define LRAM_INIT_CL_F_LATENCY 15
#define LRAM_INIT_CL_R_LATENCY 11
#define LRAM_INIT_CL_W_LATENCY  9
#define LRAM_INIT_EX_F_LATENCY 19
#define LRAM_INIT_EX_R_LATENCY 16
#define LRAM_INIT_EX_W_LATENCY 11

#define FROM_INIT_BASE 0x00000000
#define FROM_MAX_SIZE  0x10000000 // for max size
#define FROM_INIT_SIZE 0x00800000 // for E2xFCC1 size
#define CRAM_INIT_BASE 0xFE000000
#define CRAM_MAX_SIZE  0x01000000 // for max size
#define CRAM_INIT_SIZE 0x000B0000 // for E2xFCC1 size
#define LRAM_INIT_BASE 0xFDE00000
#define LRAM_INIT_SIZE 0x00200000
#define LRAM_INIT_BODY_SIZE 0x00008000
#define LRSRC_INIT_BASE  0xFFFC0000ULL
#define LRSRC_INIT_SIZE  0x00004000ULL
#define CRG_INIT_BASE  0xFFC6D000
#define PEG_INIT_BASE  0xFFC6C000


#define FROM_OUTSTAND  6
#define FROM_REPEAT    4
#define CRAM_OUTSTAND  8
#define CRAM_REPEAT    2
#define LRAM_OUTSTAND 12
#define LRAM_REPEAT    1

#endif // G4CPU_DEF_H
