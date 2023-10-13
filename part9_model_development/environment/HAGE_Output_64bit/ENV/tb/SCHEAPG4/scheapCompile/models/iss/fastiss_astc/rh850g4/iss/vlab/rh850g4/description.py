#!/usr/bin/env python

#------------------------------------------------------------------------------
# Copyright (C) Australian Semiconductor Technology Company. 2020.
# All Rights Reserved.
#
# This is unpublished proprietary source code of the Australian Semiconductor
# Technology Company (ASTC).  The copyright notice does not evidence any actual
# or intended publication of such source code.
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Imports
#------------------------------------------------------------------------------

import vlab

vlab.properties(doc="rh850g4 FastISS Core Model",
                kind="core",
                module="rh850g4.core",
                bit_endianness="lsb0",
                extensions={
                  "disassemblers": ["rh850e3"],
                  "memory_spaces": ["default", "physical"],
                  "physical_memory_space": "physical",
                  "physical_memory_at_zero": True})

vlab.register(name='r0', width=32, extensions={'id': 0}, groups=['GR'])
vlab.register(name='r1', width=32, extensions={'id': 1}, groups=['GR'])
vlab.register(name='r2', width=32, extensions={'id': 2}, groups=['GR'])
vlab.register(name='r3', width=32, extensions={'id': 3}, groups=['GR'])
vlab.register(name='r4', width=32, extensions={'id': 4}, groups=['GR'])
vlab.register(name='r5', width=32, extensions={'id': 5}, groups=['GR'])
vlab.register(name='r6', width=32, extensions={'id': 6}, groups=['GR'])
vlab.register(name='r7', width=32, extensions={'id': 7}, groups=['GR'])
vlab.register(name='r8', width=32, extensions={'id': 8}, groups=['GR'])
vlab.register(name='r9', width=32, extensions={'id': 9}, groups=['GR'])
vlab.register(name='r10', width=32, extensions={'id': 10}, groups=['GR'])
vlab.register(name='r11', width=32, extensions={'id': 11}, groups=['GR'])
vlab.register(name='r12', width=32, extensions={'id': 12}, groups=['GR'])
vlab.register(name='r13', width=32, extensions={'id': 13}, groups=['GR'])
vlab.register(name='r14', width=32, extensions={'id': 14}, groups=['GR'])
vlab.register(name='r15', width=32, extensions={'id': 15}, groups=['GR'])
vlab.register(name='r16', width=32, extensions={'id': 16}, groups=['GR'])
vlab.register(name='r17', width=32, extensions={'id': 17}, groups=['GR'])
vlab.register(name='r18', width=32, extensions={'id': 18}, groups=['GR'])
vlab.register(name='r19', width=32, extensions={'id': 19}, groups=['GR'])
vlab.register(name='r20', width=32, extensions={'id': 20}, groups=['GR'])
vlab.register(name='r21', width=32, extensions={'id': 21}, groups=['GR'])
vlab.register(name='r22', width=32, extensions={'id': 22}, groups=['GR'])
vlab.register(name='r23', width=32, extensions={'id': 23}, groups=['GR'])
vlab.register(name='r24', width=32, extensions={'id': 24}, groups=['GR'])
vlab.register(name='r25', width=32, extensions={'id': 25}, groups=['GR'])
vlab.register(name='r26', width=32, extensions={'id': 26}, groups=['GR'])
vlab.register(name='r27', width=32, extensions={'id': 27}, groups=['GR'])
vlab.register(name='r28', width=32, extensions={'id': 28}, groups=['GR'])
vlab.register(name='r29', width=32, extensions={'id': 29}, groups=['GR'])
vlab.register(name='r30', width=32, extensions={'id': 30}, groups=['GR'])
vlab.register(name='r31', width=32, extensions={'id': 31}, groups=['GR'])
vlab.register(name='PC', width=32, extensions={'id': 32, 'is_pc': True})
vlab.register(name='wr0', width=64, extensions={'id': 33}, groups=['WR'])
vlab.register(name='wr0_u', width=64, extensions={'id': 34}, groups=['WR'])
vlab.register(name='wr1', width=64, extensions={'id': 35}, groups=['WR'])
vlab.register(name='wr1_u', width=64, extensions={'id': 36}, groups=['WR'])
vlab.register(name='wr2', width=64, extensions={'id': 37}, groups=['WR'])
vlab.register(name='wr2_u', width=64, extensions={'id': 38}, groups=['WR'])
vlab.register(name='wr3', width=64, extensions={'id': 39}, groups=['WR'])
vlab.register(name='wr3_u', width=64, extensions={'id': 40}, groups=['WR'])
vlab.register(name='wr4', width=64, extensions={'id': 41}, groups=['WR'])
vlab.register(name='wr4_u', width=64, extensions={'id': 42}, groups=['WR'])
vlab.register(name='wr5', width=64, extensions={'id': 43}, groups=['WR'])
vlab.register(name='wr5_u', width=64, extensions={'id': 44}, groups=['WR'])
vlab.register(name='wr6', width=64, extensions={'id': 45}, groups=['WR'])
vlab.register(name='wr6_u', width=64, extensions={'id': 46}, groups=['WR'])
vlab.register(name='wr7', width=64, extensions={'id': 47}, groups=['WR'])
vlab.register(name='wr7_u', width=64, extensions={'id': 48}, groups=['WR'])
vlab.register(name='wr8', width=64, extensions={'id': 49}, groups=['WR'])
vlab.register(name='wr8_u', width=64, extensions={'id': 50}, groups=['WR'])
vlab.register(name='wr9', width=64, extensions={'id': 51}, groups=['WR'])
vlab.register(name='wr9_u', width=64, extensions={'id': 52}, groups=['WR'])
vlab.register(name='wr10', width=64, extensions={'id': 53}, groups=['WR'])
vlab.register(name='wr10_u', width=64, extensions={'id': 54}, groups=['WR'])
vlab.register(name='wr11', width=64, extensions={'id': 55}, groups=['WR'])
vlab.register(name='wr11_u', width=64, extensions={'id': 56}, groups=['WR'])
vlab.register(name='wr12', width=64, extensions={'id': 57}, groups=['WR'])
vlab.register(name='wr12_u', width=64, extensions={'id': 58}, groups=['WR'])
vlab.register(name='wr13', width=64, extensions={'id': 59}, groups=['WR'])
vlab.register(name='wr13_u', width=64, extensions={'id': 60}, groups=['WR'])
vlab.register(name='wr14', width=64, extensions={'id': 61}, groups=['WR'])
vlab.register(name='wr14_u', width=64, extensions={'id': 62}, groups=['WR'])
vlab.register(name='wr15', width=64, extensions={'id': 63}, groups=['WR'])
vlab.register(name='wr15_u', width=64, extensions={'id': 64}, groups=['WR'])
vlab.register(name='wr16', width=64, extensions={'id': 65}, groups=['WR'])
vlab.register(name='wr16_u', width=64, extensions={'id': 66}, groups=['WR'])
vlab.register(name='wr17', width=64, extensions={'id': 67}, groups=['WR'])
vlab.register(name='wr17_u', width=64, extensions={'id': 68}, groups=['WR'])
vlab.register(name='wr18', width=64, extensions={'id': 69}, groups=['WR'])
vlab.register(name='wr18_u', width=64, extensions={'id': 70}, groups=['WR'])
vlab.register(name='wr19', width=64, extensions={'id': 71}, groups=['WR'])
vlab.register(name='wr19_u', width=64, extensions={'id': 72}, groups=['WR'])
vlab.register(name='wr20', width=64, extensions={'id': 73}, groups=['WR'])
vlab.register(name='wr20_u', width=64, extensions={'id': 74}, groups=['WR'])
vlab.register(name='wr21', width=64, extensions={'id': 75}, groups=['WR'])
vlab.register(name='wr21_u', width=64, extensions={'id': 76}, groups=['WR'])
vlab.register(name='wr22', width=64, extensions={'id': 77}, groups=['WR'])
vlab.register(name='wr22_u', width=64, extensions={'id': 78}, groups=['WR'])
vlab.register(name='wr23', width=64, extensions={'id': 79}, groups=['WR'])
vlab.register(name='wr23_u', width=64, extensions={'id': 80}, groups=['WR'])
vlab.register(name='wr24', width=64, extensions={'id': 81}, groups=['WR'])
vlab.register(name='wr24_u', width=64, extensions={'id': 82}, groups=['WR'])
vlab.register(name='wr25', width=64, extensions={'id': 83}, groups=['WR'])
vlab.register(name='wr25_u', width=64, extensions={'id': 84}, groups=['WR'])
vlab.register(name='wr26', width=64, extensions={'id': 85}, groups=['WR'])
vlab.register(name='wr26_u', width=64, extensions={'id': 86}, groups=['WR'])
vlab.register(name='wr27', width=64, extensions={'id': 87}, groups=['WR'])
vlab.register(name='wr27_u', width=64, extensions={'id': 88}, groups=['WR'])
vlab.register(name='wr28', width=64, extensions={'id': 89}, groups=['WR'])
vlab.register(name='wr28_u', width=64, extensions={'id': 90}, groups=['WR'])
vlab.register(name='wr29', width=64, extensions={'id': 91}, groups=['WR'])
vlab.register(name='wr29_u', width=64, extensions={'id': 92}, groups=['WR'])
vlab.register(name='wr30', width=64, extensions={'id': 93}, groups=['WR'])
vlab.register(name='wr30_u', width=64, extensions={'id': 94}, groups=['WR'])
vlab.register(name='wr31', width=64, extensions={'id': 95}, groups=['WR'])
vlab.register(name='wr31_u', width=64, extensions={'id': 96}, groups=['WR'])
vlab.register(name='EIPC', width=32, extensions={'id': 97}, groups=['SR_SELID_0'])
vlab.register(name='EIPSW', width=32, extensions={'id': 98}, groups=['SR_SELID_0'])
vlab.register(name='FEPC', width=32, extensions={'id': 99}, groups=['SR_SELID_0'])
vlab.register(name='FEPSW', width=32, extensions={'id': 100}, groups=['SR_SELID_0'])
vlab.register(name='PSW', width=32, extensions={'id': 101}, groups=['SR_SELID_0'])
vlab.field(name='PSW_Z', register='PSW', offset=0, width=1)
vlab.field(name='PSW_S', register='PSW', offset=1, width=1)
vlab.field(name='PSW_OV', register='PSW', offset=2, width=1)
vlab.field(name='PSW_CY', register='PSW', offset=3, width=1)
vlab.field(name='PSW_SAT', register='PSW', offset=4, width=1)
vlab.field(name='PSW_ID', register='PSW', offset=5, width=1)
vlab.field(name='PSW_EP', register='PSW', offset=6, width=1)
vlab.field(name='PSW_NP', register='PSW', offset=7, width=1)
vlab.field(name='PSW_EBV', register='PSW', offset=15, width=1)
vlab.field(name='PSW_CU0', register='PSW', offset=16, width=1)
vlab.field(name='PSW_CU1', register='PSW', offset=17, width=1)
vlab.field(name='PSW_CU2', register='PSW', offset=18, width=1)
vlab.field(name='PSW_EIMASK', register='PSW', offset=20, width=8)
vlab.field(name='PSW_UM', register='PSW', offset=30, width=1)
vlab.register(name='FPSR', width=32, extensions={'id': 102}, groups=['SR_SELID_0'])
vlab.field(name='FPSR_CC', register='FPSR', offset=24, width=8)
vlab.field(name='FPSR_FN', register='FPSR', offset=23, width=1)
vlab.field(name='FPSR_IF', register='FPSR', offset=22, width=1)
vlab.field(name='FPSR_PEM', register='FPSR', offset=21, width=1)
vlab.field(name='FPSR_RM', register='FPSR', offset=18, width=2)
vlab.field(name='FPSR_FS', register='FPSR', offset=17, width=1)
vlab.field(name='FPSR_XC', register='FPSR', offset=10, width=6)
vlab.field(name='FPSR_XE', register='FPSR', offset=5, width=5)
vlab.field(name='FPSR_XP', register='FPSR', offset=0, width=5)
vlab.register(name='FPEPC', width=32, extensions={'id': 103}, groups=['SR_SELID_0'])
vlab.register(name='FPST', width=32, extensions={'id': 104}, groups=['SR_SELID_0'])
vlab.field(name='FPST_XC', register='FPST', offset=8, width=6)
vlab.field(name='FPST_IF', register='FPST', offset=5, width=1)
vlab.field(name='FPST_XP', register='FPST', offset=0, width=5)
vlab.register(name='FPCC', width=32, extensions={'id': 105}, groups=['SR_SELID_0'])
vlab.field(name='FPCC_CC', register='FPCC', offset=0, width=8)
vlab.register(name='FPCFG', width=32, extensions={'id': 106}, groups=['SR_SELID_0'])
vlab.field(name='FPCFG_RM', register='FPCFG', offset=8, width=2)
vlab.field(name='FPCFG_XE', register='FPCFG', offset=0, width=5)
vlab.register(name='EIIC', width=32, extensions={'id': 107}, groups=['SR_SELID_0'])
vlab.register(name='FEIC', width=32, extensions={'id': 108}, groups=['SR_SELID_0'])
vlab.register(name='PSWH', width=32, extensions={'id': 109}, groups=['SR_SELID_0'])
vlab.field(name='PSWH_GPID', register='PSWH', offset=8, width=3)
vlab.field(name='PSWH_GM', register='PSWH', offset=31, width=1)
vlab.register(name='CTPC', width=32, extensions={'id': 110}, groups=['SR_SELID_0'])
vlab.register(name='CTPSW', width=32, extensions={'id': 111}, groups=['SR_SELID_0'])
vlab.register(name='EIPSWH', width=32, extensions={'id': 112}, groups=['SR_SELID_0'])
vlab.field(name='EIPSWH_GPID', register='EIPSWH', offset=8, width=3)
vlab.field(name='EIPSWH_GM', register='EIPSWH', offset=31, width=1)
vlab.register(name='FEPSWH', width=32, extensions={'id': 113}, groups=['SR_SELID_0'])
vlab.field(name='FEPSWH_GPID', register='FEPSWH', offset=8, width=3)
vlab.field(name='FEPSWH_GM', register='FEPSWH', offset=31, width=1)
vlab.register(name='CTBP', width=32, extensions={'id': 114}, groups=['SR_SELID_0'])
vlab.register(name='SNZCFG', width=32, extensions={'id': 115}, groups=['SR_SELID_0'])
vlab.register(name='EIWR', width=32, extensions={'id': 116}, groups=['SR_SELID_0'])
vlab.register(name='FEWR', width=32, extensions={'id': 117}, groups=['SR_SELID_0'])
vlab.register(name='SPID', width=32, extensions={'id': 118}, groups=['SR_SELID_1'])
vlab.register(name='SPIDLIST', width=32, extensions={'id': 119}, groups=['SR_SELID_1'])
vlab.register(name='RBASE', width=32, extensions={'id': 120}, groups=['SR_SELID_1'])
vlab.field(name='RBASE_RBASE', register='RBASE', offset=9, width=23)
vlab.field(name='RBASE_DV', register='RBASE', offset=1, width=1)
vlab.field(name='RBASE_RINT', register='RBASE', offset=0, width=1)
vlab.register(name='EBASE', width=32, extensions={'id': 121}, groups=['SR_SELID_1'])
vlab.field(name='EBASE_EBASE', register='EBASE', offset=9, width=23)
vlab.field(name='EBASE_DV', register='EBASE', offset=1, width=1)
vlab.field(name='EBASE_RINT', register='EBASE', offset=0, width=1)
vlab.register(name='INTBP', width=32, extensions={'id': 122}, groups=['SR_SELID_1'])
vlab.register(name='MCTL', width=32, extensions={'id': 123}, groups=['SR_SELID_1'])
vlab.field(name='MCTL_UIC', register='MCTL', offset=0, width=1)
vlab.register(name='PID', width=32, extensions={'id': 124}, groups=['SR_SELID_1'])
vlab.register(name='SVLOCK', width=32, extensions={'id': 125}, groups=['SR_SELID_1'])
vlab.register(name='SCCFG', width=32, extensions={'id': 126}, groups=['SR_SELID_1'])
vlab.field(name='SCCFG_SIZE', register='SCCFG', offset=0, width=8)
vlab.register(name='SCBP', width=32, extensions={'id': 127}, groups=['SR_SELID_1'])
vlab.register(name='HVCFG', width=32, extensions={'id': 128}, groups=['SR_SELID_1'])
vlab.field(name='HVCFG_HVE', register='HVCFG', offset=0, width=1)
vlab.register(name='GMCFG', width=32, extensions={'id': 129}, groups=['SR_SELID_1'])
vlab.field(name='GMCFG_GMP', register='GMCFG', offset=0, width=1)
vlab.field(name='GMCFG_HMP', register='GMCFG', offset=1, width=1)
vlab.field(name='GMCFG_GSYSE', register='GMCFG', offset=4, width=1)
vlab.field(name='GMCFG_GCU0', register='GMCFG', offset=16, width=1)
vlab.field(name='GMCFG_GCU1', register='GMCFG', offset=17, width=1)
vlab.field(name='GMCFG_GCU2', register='GMCFG', offset=18, width=1)
vlab.register(name='HVSB', width=32, extensions={'id': 130}, groups=['SR_SELID_1'])
vlab.register(name='PEID', width=32, extensions={'id': 131}, groups=['SR_SELID_2'])
vlab.register(name='BMID', width=32, extensions={'id': 132}, groups=['SR_SELID_2'])
vlab.field(name='BMID_BMID', register='BMID', offset=0, width=5)
vlab.register(name='MEA', width=32, extensions={'id': 133}, groups=['SR_SELID_2'])
vlab.register(name='MEI', width=32, extensions={'id': 134}, groups=['SR_SELID_2'])
vlab.register(name='RBCR0', width=32, extensions={'id': 135}, groups=['SR_SELID_2'])
vlab.field(name='RBCR0_MD', register='RBCR0', offset=16, width=1)
vlab.field(name='RBCR0_BE', register='RBCR0', offset=0, width=16)
vlab.register(name='RBCR1', width=32, extensions={'id': 136}, groups=['SR_SELID_2'])
vlab.field(name='RBCR1_NC', register='RBCR1', offset=0, width=16)
vlab.register(name='RBNR', width=32, extensions={'id': 137}, groups=['SR_SELID_2'])
vlab.field(name='RBNR_BN', register='RBNR', offset=0, width=8)
vlab.register(name='RBIP', width=32, extensions={'id': 138}, groups=['SR_SELID_2'])
vlab.field(name='RBIP_RBIP', register='RBIP', offset=4, width=28)
vlab.register(name='ISPR', width=32, extensions={'id': 139}, groups=['SR_SELID_2'])
vlab.field(name='ISPR_ISP', register='ISPR', offset=0, width=16)
vlab.register(name='IMSR', width=32, extensions={'id': 140}, groups=['SR_SELID_2'])
vlab.field(name='IMSR_EEIM', register='IMSR', offset=0, width=1)
vlab.field(name='IMSR_EPLM', register='IMSR', offset=1, width=1)
vlab.field(name='IMSR_EID', register='IMSR', offset=2, width=1)
vlab.field(name='IMSR_ENP', register='IMSR', offset=3, width=1)
vlab.field(name='IMSR_FNP', register='IMSR', offset=4, width=1)
vlab.field(name='IMSR_HEEIM', register='IMSR', offset=8, width=1)
vlab.field(name='IMSR_HEPLM', register='IMSR', offset=9, width=1)
vlab.field(name='IMSR_HEID', register='IMSR', offset=10, width=1)
vlab.field(name='IMSR_HENP', register='IMSR', offset=11, width=1)
vlab.field(name='IMSR_HFNP', register='IMSR', offset=12, width=1)
vlab.register(name='ICSR', width=32, extensions={'id': 141}, groups=['SR_SELID_2'])
vlab.field(name='ICSR_PMEI', register='ICSR', offset=0, width=1)
vlab.register(name='INTCFG', width=32, extensions={'id': 142}, groups=['SR_SELID_2'])
vlab.field(name='INTCFG_ISPC', register='INTCFG', offset=0, width=1)
vlab.field(name='INTCFG_EPL', register='INTCFG', offset=1, width=1)
vlab.field(name='INTCFG_ULNR', register='INTCFG', offset=16, width=8)
vlab.register(name='PLMR', width=32, extensions={'id': 143}, groups=['SR_SELID_2'])
vlab.field(name='PLMR_PLM', register='PLMR', offset=0, width=8)
vlab.register(name='FXSR', width=32, extensions={'id': 144}, groups=['SR_SELID_10'])
vlab.field(name='FXSR_FN', register='FXSR', offset=23, width=1)
vlab.field(name='FXSR_IF', register='FXSR', offset=22, width=1)
vlab.field(name='FXSR_PEM', register='FXSR', offset=21, width=1)
vlab.field(name='FXSR_RM', register='FXSR', offset=18, width=2)
vlab.field(name='FXSR_FS', register='FXSR', offset=17, width=1)
vlab.field(name='FXSR_XC', register='FXSR', offset=10, width=6)
vlab.field(name='FXSR_XE', register='FXSR', offset=5, width=5)
vlab.field(name='FXSR_XP', register='FXSR', offset=0, width=5)
vlab.register(name='FXST', width=32, extensions={'id': 145}, groups=['SR_SELID_10'])
vlab.field(name='FXST_XC', register='FXST', offset=8, width=6)
vlab.field(name='FXST_IF', register='FXST', offset=5, width=1)
vlab.field(name='FXST_XP', register='FXST', offset=0, width=5)
vlab.register(name='FXINFO', width=32, extensions={'id': 146}, groups=['SR_SELID_10'])
vlab.field(name='FXINFO_NFPU', register='FXINFO', offset=1, width=1)
vlab.field(name='FXINFO_RSIZE', register='FXINFO', offset=0, width=1)
vlab.register(name='FXCFG', width=32, extensions={'id': 147}, groups=['SR_SELID_10'])
vlab.field(name='FXCFG_RM', register='FXCFG', offset=8, width=2)
vlab.field(name='FXCFG_XE', register='FXCFG', offset=0, width=5)
vlab.register(name='FXXC', width=32, extensions={'id': 148}, groups=['SR_SELID_10'])
vlab.field(name='FXXC_XC3', register='FXXC', offset=24, width=6)
vlab.field(name='FXXC_XC2', register='FXXC', offset=16, width=6)
vlab.field(name='FXXC_XC1', register='FXXC', offset=8, width=6)
vlab.field(name='FXXC_XC0', register='FXXC', offset=0, width=6)
vlab.register(name='FXXP', width=32, extensions={'id': 149}, groups=['SR_SELID_10'])
vlab.field(name='FXXP_XP3', register='FXXP', offset=24, width=5)
vlab.field(name='FXXP_XP2', register='FXXP', offset=16, width=5)
vlab.field(name='FXXP_XP1', register='FXXP', offset=8, width=5)
vlab.field(name='FXXP_XP0', register='FXXP', offset=0, width=5)
vlab.register(name='GMEIPC', width=32, extensions={'id': 150}, groups=['SR_SELID_9'])
vlab.register(name='GMEIPSW', width=32, extensions={'id': 151}, groups=['SR_SELID_9'])
vlab.register(name='GMFEPC', width=32, extensions={'id': 152}, groups=['SR_SELID_9'])
vlab.register(name='GMFEPSW', width=32, extensions={'id': 153}, groups=['SR_SELID_9'])
vlab.register(name='GMPSW', width=32, extensions={'id': 154}, groups=['SR_SELID_9'])
vlab.field(name='GMPSW_Z', register='GMPSW', offset=0, width=1)
vlab.field(name='GMPSW_S', register='GMPSW', offset=1, width=1)
vlab.field(name='GMPSW_OV', register='GMPSW', offset=2, width=1)
vlab.field(name='GMPSW_CY', register='GMPSW', offset=3, width=1)
vlab.field(name='GMPSW_SAT', register='GMPSW', offset=4, width=1)
vlab.field(name='GMPSW_ID', register='GMPSW', offset=5, width=1)
vlab.field(name='GMPSW_EP', register='GMPSW', offset=6, width=1)
vlab.field(name='GMPSW_NP', register='GMPSW', offset=7, width=1)
vlab.field(name='GMPSW_EBV', register='GMPSW', offset=15, width=1)
vlab.field(name='GMPSW_CU0', register='GMPSW', offset=16, width=1)
vlab.field(name='GMPSW_CU1', register='GMPSW', offset=17, width=1)
vlab.field(name='GMPSW_CU2', register='GMPSW', offset=18, width=1)
vlab.field(name='GMPSW_EIMASK', register='GMPSW', offset=20, width=8)
vlab.field(name='GMPSW_UM', register='GMPSW', offset=30, width=1)
vlab.register(name='GMMEA', width=32, extensions={'id': 155}, groups=['SR_SELID_9'])
vlab.register(name='GMMEI', width=32, extensions={'id': 156}, groups=['SR_SELID_9'])
vlab.register(name='GMEIIC', width=32, extensions={'id': 157}, groups=['SR_SELID_9'])
vlab.register(name='GMFEIC', width=32, extensions={'id': 158}, groups=['SR_SELID_9'])
vlab.register(name='GMSPID', width=32, extensions={'id': 159}, groups=['SR_SELID_9'])
vlab.register(name='GMSPIDLIST', width=32, extensions={'id': 160}, groups=['SR_SELID_9'])
vlab.register(name='GMEBASE', width=32, extensions={'id': 161}, groups=['SR_SELID_9'])
vlab.field(name='GMEBASE_EBASE', register='GMEBASE', offset=9, width=23)
vlab.field(name='GMEBASE_DV', register='GMEBASE', offset=1, width=1)
vlab.field(name='GMEBASE_RINT', register='GMEBASE', offset=0, width=1)
vlab.register(name='GMINTBP', width=32, extensions={'id': 162}, groups=['SR_SELID_9'])
vlab.register(name='GMINTCFG', width=32, extensions={'id': 163}, groups=['SR_SELID_9'])
vlab.field(name='GMINTCFG_ISPC', register='GMINTCFG', offset=0, width=1)
vlab.field(name='GMINTCFG_EPL', register='GMINTCFG', offset=1, width=1)
vlab.field(name='GMINTCFG_ULNR', register='GMINTCFG', offset=16, width=8)
vlab.register(name='GMPLMR', width=32, extensions={'id': 164}, groups=['SR_SELID_9'])
vlab.field(name='GMPLMR_PLM', register='GMPLMR', offset=0, width=8)
vlab.register(name='GMSVLOCK', width=32, extensions={'id': 165}, groups=['SR_SELID_9'])
vlab.register(name='GMMPM', width=32, extensions={'id': 166}, groups=['SR_SELID_9'])
vlab.register(name='GMEIWR', width=32, extensions={'id': 167}, groups=['SR_SELID_9'])
vlab.register(name='GMFEWR', width=32, extensions={'id': 168}, groups=['SR_SELID_9'])
vlab.register(name='GMPEID', width=32, extensions={'id': 169}, groups=['SR_SELID_9'])
vlab.register(name='MPM', width=32, extensions={'id': 170}, groups=['SR_SELID_5'])
vlab.register(name='MPCFG', width=32, extensions={'id': 171}, groups=['SR_SELID_5'])
vlab.field(name='MPCFG_NMPUE', register='MPCFG', offset=0, width=5)
vlab.field(name='MPCFG_HBE', register='MPCFG', offset=8, width=6)
vlab.field(name='MPCFG_ARCH', register='MPCFG', offset=16, width=4)
vlab.field(name='MPCFG_NBK', register='MPCFG', offset=20, width=2)
vlab.field(name='MPCFG_DMDP', register='MPCFG', offset=24, width=1)
vlab.register(name='MCA', width=32, extensions={'id': 172}, groups=['SR_SELID_5'])
vlab.register(name='MCS', width=32, extensions={'id': 173}, groups=['SR_SELID_5'])
vlab.register(name='MCC', width=32, extensions={'id': 174}, groups=['SR_SELID_5'])
vlab.register(name='MCR', width=32, extensions={'id': 175}, groups=['SR_SELID_5'])
vlab.field(name='MCR_HSXE', register='MCR', offset=29, width=1)
vlab.field(name='MCR_HSWE', register='MCR', offset=28, width=1)
vlab.field(name='MCR_HSRE', register='MCR', offset=27, width=1)
vlab.field(name='MCR_HUXE', register='MCR', offset=26, width=1)
vlab.field(name='MCR_HUWE', register='MCR', offset=25, width=1)
vlab.field(name='MCR_HURE', register='MCR', offset=24, width=1)
vlab.field(name='MCR_GSXE', register='MCR', offset=21, width=1)
vlab.field(name='MCR_GSWE', register='MCR', offset=20, width=1)
vlab.field(name='MCR_GSRE', register='MCR', offset=19, width=1)
vlab.field(name='MCR_GUXE', register='MCR', offset=18, width=1)
vlab.field(name='MCR_GUWE', register='MCR', offset=17, width=1)
vlab.field(name='MCR_GURE', register='MCR', offset=16, width=1)
vlab.field(name='MCR_OV', register='MCR', offset=8, width=1)
vlab.field(name='MCR_SXE', register='MCR', offset=5, width=1)
vlab.field(name='MCR_SWE', register='MCR', offset=4, width=1)
vlab.field(name='MCR_SRE', register='MCR', offset=3, width=1)
vlab.field(name='MCR_UXE', register='MCR', offset=2, width=1)
vlab.field(name='MCR_UWE', register='MCR', offset=1, width=1)
vlab.field(name='MCR_URE', register='MCR', offset=0, width=1)
vlab.register(name='MCI', width=32, extensions={'id': 176}, groups=['SR_SELID_5'])
vlab.field(name='MCI_SPID', register='MCI', offset=0, width=5)
vlab.register(name='MPIDX', width=32, extensions={'id': 177}, groups=['SR_SELID_5'])
vlab.field(name='MPIDX_IDX', register='MPIDX', offset=0, width=5)
vlab.register(name='MPBK', width=32, extensions={'id': 178}, groups=['SR_SELID_5'])
vlab.field(name='MPBK_BK', register='MPBK', offset=0, width=2)
vlab.register(name='MPLA', width=32, extensions={'id': 179}, groups=['SR_SELID_5'])
vlab.register(name='MPUA', width=32, extensions={'id': 180}, groups=['SR_SELID_5'])
vlab.register(name='MPAT', width=32, extensions={'id': 181}, groups=['SR_SELID_5'])
vlab.field(name='MPAT_WMPID7', register='MPAT', offset=31, width=1)
vlab.field(name='MPAT_WMPID6', register='MPAT', offset=30, width=1)
vlab.field(name='MPAT_WMPID5', register='MPAT', offset=29, width=1)
vlab.field(name='MPAT_WMPID4', register='MPAT', offset=28, width=1)
vlab.field(name='MPAT_WMPID3', register='MPAT', offset=27, width=1)
vlab.field(name='MPAT_WMPID2', register='MPAT', offset=26, width=1)
vlab.field(name='MPAT_WMPID1', register='MPAT', offset=25, width=1)
vlab.field(name='MPAT_WMPID0', register='MPAT', offset=24, width=1)
vlab.field(name='MPAT_RMPID7', register='MPAT', offset=23, width=1)
vlab.field(name='MPAT_RMPID6', register='MPAT', offset=22, width=1)
vlab.field(name='MPAT_RMPID5', register='MPAT', offset=21, width=1)
vlab.field(name='MPAT_RMPID4', register='MPAT', offset=20, width=1)
vlab.field(name='MPAT_RMPID3', register='MPAT', offset=19, width=1)
vlab.field(name='MPAT_RMPID2', register='MPAT', offset=18, width=1)
vlab.field(name='MPAT_RMPID1', register='MPAT', offset=17, width=1)
vlab.field(name='MPAT_RMPID0', register='MPAT', offset=16, width=1)
vlab.field(name='MPAT_WG', register='MPAT', offset=15, width=1)
vlab.field(name='MPAT_RG', register='MPAT', offset=14, width=1)
vlab.field(name='MPAT_E', register='MPAT', offset=7, width=1)
vlab.field(name='MPAT_SX', register='MPAT', offset=5, width=1)
vlab.field(name='MPAT_SW', register='MPAT', offset=4, width=1)
vlab.field(name='MPAT_SR', register='MPAT', offset=3, width=1)
vlab.field(name='MPAT_UX', register='MPAT', offset=2, width=1)
vlab.field(name='MPAT_UW', register='MPAT', offset=1, width=1)
vlab.field(name='MPAT_UR', register='MPAT', offset=0, width=1)
vlab.register(name='MPID0', width=32, extensions={'id': 182}, groups=['SR_SELID_5'])
vlab.field(name='MPID0_SPID', register='MPID0', offset=0, width=5)
vlab.register(name='MPID1', width=32, extensions={'id': 183}, groups=['SR_SELID_5'])
vlab.field(name='MPID1_SPID', register='MPID1', offset=0, width=5)
vlab.register(name='MPID2', width=32, extensions={'id': 184}, groups=['SR_SELID_5'])
vlab.field(name='MPID2_SPID', register='MPID2', offset=0, width=5)
vlab.register(name='MPID3', width=32, extensions={'id': 185}, groups=['SR_SELID_5'])
vlab.field(name='MPID3_SPID', register='MPID3', offset=0, width=5)
vlab.register(name='MPID4', width=32, extensions={'id': 186}, groups=['SR_SELID_5'])
vlab.field(name='MPID4_SPID', register='MPID4', offset=0, width=5)
vlab.register(name='MPID5', width=32, extensions={'id': 187}, groups=['SR_SELID_5'])
vlab.field(name='MPID5_SPID', register='MPID5', offset=0, width=5)
vlab.register(name='MPID6', width=32, extensions={'id': 188}, groups=['SR_SELID_5'])
vlab.field(name='MPID6_SPID', register='MPID6', offset=0, width=5)
vlab.register(name='MPID7', width=32, extensions={'id': 189}, groups=['SR_SELID_5'])
vlab.field(name='MPID7_SPID', register='MPID7', offset=0, width=5)
vlab.register(name='ICTAGL', width=32, extensions={'id': 190}, groups=['SR_SELID_4'])
vlab.field(name='ICTAGL_LPN', register='ICTAGL', offset=11, width=21)
vlab.field(name='ICTAGL_LRU', register='ICTAGL', offset=4, width=2)
vlab.field(name='ICTAGL_L', register='ICTAGL', offset=2, width=1)
vlab.field(name='ICTAGL_V', register='ICTAGL', offset=0, width=1)
vlab.register(name='ICTAGH', width=32, extensions={'id': 191}, groups=['SR_SELID_4'])
vlab.field(name='ICTAGH_WD', register='ICTAGH', offset=31, width=1)
vlab.field(name='ICTAGH_PD', register='ICTAGH', offset=30, width=1)
vlab.field(name='ICTAGH_WT', register='ICTAGH', offset=29, width=1)
vlab.field(name='ICTAGH_PT', register='ICTAGH', offset=28, width=1)
vlab.field(name='ICTAGH_DATAECC', register='ICTAGH', offset=16, width=8)
vlab.field(name='ICTAGH_TAGECC', register='ICTAGH', offset=8, width=8)
vlab.register(name='ICDATL', width=32, extensions={'id': 192}, groups=['SR_SELID_4'])
vlab.register(name='ICDATH', width=32, extensions={'id': 193}, groups=['SR_SELID_4'])
vlab.register(name='ICCTRL', width=32, extensions={'id': 194}, groups=['SR_SELID_4'])
vlab.field(name='ICCTRL_PBS', register='ICCTRL', offset=16, width=1)
vlab.field(name='ICCTRL_ICHCLR', register='ICCTRL', offset=8, width=1)
vlab.field(name='ICCTRL_ICHEIV', register='ICCTRL', offset=2, width=1)
vlab.field(name='ICCTRL_ICHEMK', register='ICCTRL', offset=1, width=1)
vlab.field(name='ICCTRL_ICHEN', register='ICCTRL', offset=0, width=1)
vlab.register(name='ICCFG', width=32, extensions={'id': 195}, groups=['SR_SELID_4'])
vlab.field(name='ICCFG_ICHSIZE', register='ICCFG', offset=8, width=7)
vlab.field(name='ICCFG_ICHLINE', register='ICCFG', offset=4, width=4)
vlab.field(name='ICCFG_ICHWAY', register='ICCFG', offset=0, width=4)
vlab.register(name='ICERR', width=32, extensions={'id': 196}, groups=['SR_SELID_4'])
vlab.field(name='ICERR_CISTW', register='ICERR', offset=31, width=1)
vlab.field(name='ICERR_ESAFE', register='ICERR', offset=30, width=1)
vlab.field(name='ICERR_ESMH', register='ICERR', offset=29, width=1)
vlab.field(name='ICERR_ESPBSE', register='ICERR', offset=28, width=1)
vlab.field(name='ICERR_ESTE1', register='ICERR', offset=27, width=1)
vlab.field(name='ICERR_ESTE2', register='ICERR', offset=26, width=1)
vlab.field(name='ICERR_ESDC', register='ICERR', offset=25, width=1)
vlab.field(name='ICERR_ESDE', register='ICERR', offset=24, width=1)
vlab.field(name='ICERR_ERMAFE', register='ICERR', offset=22, width=1)
vlab.field(name='ICERR_ERMMH', register='ICERR', offset=21, width=1)
vlab.field(name='ICERR_ERMPBSE', register='ICERR', offset=20, width=1)
vlab.field(name='ICERR_ERMTE1', register='ICERR', offset=19, width=1)
vlab.field(name='ICERR_ERMTE2', register='ICERR', offset=18, width=1)
vlab.field(name='ICERR_ERMDC', register='ICERR', offset=17, width=1)
vlab.field(name='ICERR_ERMDE', register='ICERR', offset=16, width=1)
vlab.field(name='ICERR_ICHEWY', register='ICERR', offset=13, width=3)
vlab.field(name='ICERR_ICHEIX', register='ICERR', offset=5, width=8)
vlab.field(name='ICERR_ICHERQ', register='ICERR', offset=4, width=1)
vlab.field(name='ICERR_ICHED', register='ICERR', offset=3, width=1)
vlab.field(name='ICERR_ICHET', register='ICERR', offset=2, width=1)
vlab.field(name='ICERR_ICHERR', register='ICERR', offset=0, width=1)
vlab.register(name='TSCOUNTL', width=32, extensions={'id': 197}, groups=['SR_SELID_11'])
vlab.register(name='TSCOUNTH', width=32, extensions={'id': 198}, groups=['SR_SELID_11'])
vlab.register(name='TSCTRL', width=32, extensions={'id': 199}, groups=['SR_SELID_11'])
vlab.register(name='PMUMCTRL', width=32, extensions={'id': 200}, groups=['SR_SELID_11'])
vlab.register(name='PMGMCTRL', width=32, extensions={'id': 201}, groups=['SR_SELID_11'])
vlab.register(name='PMCOUNT0', width=32, extensions={'id': 202}, groups=['SR_SELID_11'])
vlab.register(name='PMCTRL0', width=32, extensions={'id': 203}, groups=['SR_SELID_11'])
vlab.register(name='PMCOUNT1', width=32, extensions={'id': 204}, groups=['SR_SELID_11'])
vlab.register(name='PMCTRL1', width=32, extensions={'id': 205}, groups=['SR_SELID_11'])
vlab.register(name='PMCOUNT2', width=32, extensions={'id': 206}, groups=['SR_SELID_11'])
vlab.register(name='PMCTRL2', width=32, extensions={'id': 207}, groups=['SR_SELID_11'])
vlab.register(name='PMCOUNT3', width=32, extensions={'id': 208}, groups=['SR_SELID_11'])
vlab.register(name='PMCTRL3', width=32, extensions={'id': 209}, groups=['SR_SELID_11'])
vlab.register(name='PMCTRL4', width=32, extensions={'id': 210}, groups=['SR_SELID_14'])
vlab.register(name='PMCTRL5', width=32, extensions={'id': 211}, groups=['SR_SELID_14'])
vlab.register(name='PMCTRL6', width=32, extensions={'id': 212}, groups=['SR_SELID_14'])
vlab.register(name='PMCTRL7', width=32, extensions={'id': 213}, groups=['SR_SELID_14'])
vlab.register(name='PMCOUNT4', width=32, extensions={'id': 214}, groups=['SR_SELID_14'])
vlab.register(name='PMCOUNT5', width=32, extensions={'id': 215}, groups=['SR_SELID_14'])
vlab.register(name='PMCOUNT6', width=32, extensions={'id': 216}, groups=['SR_SELID_14'])
vlab.register(name='PMCOUNT7', width=32, extensions={'id': 217}, groups=['SR_SELID_14'])
vlab.register(name='PMSUBCND0', width=32, extensions={'id': 218}, groups=['SR_SELID_15'])
vlab.register(name='PMSUBCND1', width=32, extensions={'id': 219}, groups=['SR_SELID_15'])
vlab.register(name='PMSUBCND2', width=32, extensions={'id': 220}, groups=['SR_SELID_15'])
vlab.register(name='PMSUBCND3', width=32, extensions={'id': 221}, groups=['SR_SELID_15'])
vlab.register(name='PMSUBCND4', width=32, extensions={'id': 222}, groups=['SR_SELID_15'])
vlab.register(name='PMSUBCND5', width=32, extensions={'id': 223}, groups=['SR_SELID_15'])
vlab.register(name='PMSUBCND6', width=32, extensions={'id': 224}, groups=['SR_SELID_15'])
vlab.register(name='PMSUBCND7', width=32, extensions={'id': 225}, groups=['SR_SELID_15'])
vlab.register(name='DBGEN', width=32, extensions={'id': 226}, groups=['SR_SELID_3'])
vlab.register(name='DBPSWH', width=32, extensions={'id': 227}, groups=['SR_SELID_3'])
vlab.register(name='DBIC', width=32, extensions={'id': 228}, groups=['SR_SELID_3'])
vlab.register(name='DBPC', width=32, extensions={'id': 229}, groups=['SR_SELID_3'])
vlab.register(name='DBPSW', width=32, extensions={'id': 230}, groups=['SR_SELID_3'])
vlab.register(name='DIR0', width=32, extensions={'id': 231}, groups=['SR_SELID_3'])
vlab.register(name='DIR1', width=32, extensions={'id': 232}, groups=['SR_SELID_3'])
vlab.register(name='BPC', width=32, extensions={'id': 233}, groups=['SR_SELID_3'])
vlab.register(name='BPAV', width=32, extensions={'id': 234}, groups=['SR_SELID_3'])
vlab.register(name='BPAM', width=32, extensions={'id': 235}, groups=['SR_SELID_3'])
vlab.register(name='DBCFG', width=32, extensions={'id': 236}, groups=['SR_SELID_3'])
vlab.register(name='DBWR', width=32, extensions={'id': 237}, groups=['SR_SELID_3'])
vlab.register(name='LSTEST0', width=32, extensions={'id': 238}, groups=['SR_SELID_12'])
vlab.register(name='LSTEST1', width=32, extensions={'id': 239}, groups=['SR_SELID_12'])
vlab.register(name='LSCFG', width=32, extensions={'id': 240}, groups=['SR_SELID_12'])
vlab.register(name='ICBKEY', width=32, extensions={'id': 241}, groups=['SR_SELID_12'])
vlab.register(name='IFCR', width=32, extensions={'id': 242}, groups=['SR_SELID_12'])
vlab.register(name='IFCR1', width=32, extensions={'id': 243}, groups=['SR_SELID_12'])
vlab.register(name='BRPCTRL0', width=32, extensions={'id': 244}, groups=['SR_SELID_12'])
vlab.register(name='BRPCTRL1', width=32, extensions={'id': 245}, groups=['SR_SELID_12'])
vlab.register(name='BRPCFG', width=32, extensions={'id': 246}, groups=['SR_SELID_12'])
vlab.register(name='BRPACTRL', width=32, extensions={'id': 247}, groups=['SR_SELID_12'])
vlab.register(name='BRPDATA', width=32, extensions={'id': 248}, groups=['SR_SELID_12'])
vlab.register(name='DCBKEY', width=32, extensions={'id': 249}, groups=['SR_SELID_13'])
vlab.register(name='LSUCR', width=32, extensions={'id': 250}, groups=['SR_SELID_13'])
vlab.register(name='LSULNK0', width=32, extensions={'id': 251}, groups=['SR_SELID_13'])
vlab.register(name='LSULNK1', width=32, extensions={'id': 252}, groups=['SR_SELID_13'])
vlab.register(name='L1RLCR', width=32, extensions={'id': 253}, groups=['SR_SELID_13'])
vlab.register(name='L1RLNK0', width=32, extensions={'id': 254}, groups=['SR_SELID_13'])
vlab.register(name='L1RLNK1', width=32, extensions={'id': 255}, groups=['SR_SELID_13'])
vlab.register(name='L1RCFG', width=32, extensions={'id': 256}, groups=['SR_SELID_13'])
vlab.register(name='DECFG', width=32, extensions={'id': 257}, groups=['SR_SELID_13'])
vlab.register(name='DECTRL', width=32, extensions={'id': 258}, groups=['SR_SELID_13'])
vlab.register(name='DEVDS', width=32, extensions={'id': 259}, groups=['SR_SELID_13'])
vlab.register(name='RDBCR', width=32, extensions={'id': 260}, groups=['SR_SELID_13'])
vlab.register(name='RDBACR', width=32, extensions={'id': 261}, groups=['SR_SELID_13'])
vlab.register(name='RDBATAG', width=32, extensions={'id': 262}, groups=['SR_SELID_13'])
vlab.register(name='RDBADAT0', width=32, extensions={'id': 263}, groups=['SR_SELID_13'])
vlab.register(name='RDBADAT1', width=32, extensions={'id': 264}, groups=['SR_SELID_13'])
vlab.register(name='RDBADAT2', width=32, extensions={'id': 265}, groups=['SR_SELID_13'])
vlab.register(name='RDBADAT3', width=32, extensions={'id': 266}, groups=['SR_SELID_13'])
vlab.register(name='RDBSTAT', width=32, extensions={'id': 267}, groups=['SR_SELID_13'])
vlab.register(name='hv_func_support', width=8, extensions={'id': 268})
vlab.register(name='swd0pcad_exec_count', width=32, extensions={'id': 269})
vlab.register(name='swd0pcad_val', width=32, extensions={'id': 270})
vlab.register(name='mem_acc_type', width=32, extensions={'id': 271})

#------------------------------------------------------------------------------
# End of file
#------------------------------------------------------------------------------