/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once

enum ArgFormat {
    ARG_DR1_DR2,
    ARG_DR1_DR2_DR3,
    ARG_DR2_DR3,
    ARG_DR2_R3,
    ARG_NONE,
    ARG_R1,
    ARG_R1_DR3,
    ARG_R1_R2,
    ARG_R1_R2_DR3_DR4,
    ARG_R1_R2_R3,
    ARG_R1_R2_R3_SELF,
    ARG_R1_R2_SELF,
    ARG_R1_R3,
    ARG_R1_RS_SP_SELF,
    ARG_R1_SELF,
    ARG_R1_SYNC,
    ARG_R1_VR2,
    ARG_R2,
    ARG_R2_DR3,
    ARG_R2_EP,
    ARG_R2_R3,
    ARG_RH_RT_SP,
    ARG_RS_SP,
    ARG_RS_SP_EP,
    ARG_SELF,
    ARG_SYNC_R,
    ARG_SYNC_RW,
    ARG_VR1_VR2X_VR3,
    ARG_VR1_VR2X_VR3_VR4,
    ARG_VR1_VR2_VR3,
    ARG_VR2,
    ARG_VR2_VR3,
    ARG_ERROR
};


enum LoadType {
    LDTYPE_CACHE,
    LDTYPE_CALLT,
    LDTYPE_DISPOSE,
    LDTYPE_LDM,
    LDTYPE_POP,
    LDTYPE_PREF,
    LDTYPE_RESBANK,
    LDTYPE_RMW,
    LDTYPE_S16,
    LDTYPE_S8,
    LDTYPE_SWITCH,
    LDTYPE_SYSCALL,
    LDTYPE_U16,
    LDTYPE_U32,
    LDTYPE_U64,
    LDTYPE_U8,
    LDTYPE_UNDEF
};
