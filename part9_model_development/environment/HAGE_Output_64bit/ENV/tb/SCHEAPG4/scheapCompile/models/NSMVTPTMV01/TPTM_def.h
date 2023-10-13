/*************************************************************************
 *
 * TPTM_def.h
 *
 * Copyright(c) 2018-2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

#ifndef __TPTM_DEF_H__
#define __TPTM_DEF_H__

#define G4TPTM_PE_NUM_INIT 1
#define G4TPTM_INTERVAL_TIMER_CH_NUM_INIT 4
#define G4TPTM_INTERVAL_TIMER_CH_NUM_MAX 4
#define G4TPTM_UP_COUNTER_TIMER_CH_NUM 2
#define G4TPTM_UP_COUNTER_TIMER_CMP_NUM 4
#define THRESHOLD_FOR_WRN_CMP_REG_INIT 0

#define TPTMSIRUN_OFF_ADD      	0x00
#define TPTMSIRRUN_OFF_ADD     	0x04
#define TPTMSISTP_OFF_ADD      	0x08
#define TPTMSISTR_OFF_ADD	0x0C
#define TPTMSIIEN_OFF_ADD      	0x10
#define TPTMSIUSTR_OFF_ADD     	0x14
#define TPTMSIDIV_OFF_ADD      	0x18
#define TPTMSFRUN_OFF_ADD      	0x20
#define TPTMSFRRUN_OFF_ADD     	0x24
#define TPTMSFSTP_OFF_ADD      	0x28
#define TPTMSFSTR_OFF_ADD      	0x2C
#define TPTMSFDIV_OFF_ADD      	0x30
#define TPTMSURUN_OFF_ADD	0x40
#define TPTMSURRUN_OFF_ADD	0x44
#define TPTMSUSTP_OFF_ADD	0x48
#define TPTMSUSTR_OFF_ADD	0x4C
#define TPTMSUIEN_OFF_ADD	0x50
#define TPTMSUDIV_OFF_ADD	0x58
#define TPTMSUTRG_OFF_ADD	0x5C
#define TPTMSICNT0_OFF_ADD     	0x80
#define TPTMSILD0_OFF_ADD      	0x84
#define TPTMSICNT1_OFF_ADD     	0x88
#define TPTMSILD1_OFF_ADD     	0x8C
#define TPTMSICNT2_OFF_ADD     	0x90
#define TPTMSILD2_OFF_ADD      	0x94
#define TPTMSICNT3_OFF_ADD     	0x98
#define TPTMSILD3_OFF_ADD    	0x9C
#define TPTMSFCNT0_OFF_ADD  	0xA0
#define TPTMSUCNT0_OFF_ADD	0xC0
#define TPTMSUCMP00_OFF_ADD	0xC4
#define TPTMSUCMP01_OFF_ADD	0xC8
#define TPTMSUCMP02_OFF_ADD	0xCC
#define TPTMSUCMP03_OFF_ADD	0xD0
#define TPTMSUCNT1_OFF_ADD	0xE0
#define TPTMSUCMP10_OFF_ADD	0xE4
#define TPTMSUCMP11_OFF_ADD	0xE8
#define TPTMSUCMP12_OFF_ADD	0xEC
#define TPTMSUCMP13_OFF_ADD	0xF0



#endif //__TPTM_DEF_H__
