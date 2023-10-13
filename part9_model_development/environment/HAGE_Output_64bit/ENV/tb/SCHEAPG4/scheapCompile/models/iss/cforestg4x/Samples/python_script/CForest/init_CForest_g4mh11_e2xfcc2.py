# -*- coding: utf-8 -*-
set_has_mpu ("on");
set_has_fxu ("on");
set_peripherals ("ipir",    "on");
set_peripherals ("mecnt",   "on");
set_peripherals ("barrier", "on");
set_peripherals ("intc1",   "on");
set_peripherals ("intc2",   "on");
set_peripherals ("simio",   "on"); # result_register
set_peripherals ("perf",    "on"); # perfcnt

# Allocate PEG
# When PEG isn't used, please set "off" instead of "on" for simulation speed.
# set_guards ("peg_name", PEID, {"on"|"off"}, addr) for PEG
set_guards("peg", 0, "on", 0xFFC6C000)     # PEG for PE0CL0
set_guards("pegcapm", 0, "on", 0xFFC6C800) # PEGCAP_M for PE0CL0
set_guards("pegcaps", 0, "on", 0xFFC6C810) # PEGCAP_S for PE0CL0
set_guards("peg", 1, "on", 0xFFC6C100)     # PEG for PE1CL0
set_guards("pegcapm", 1, "on", 0xFFC6C820) # PEGCAP_M for PE1CL0
set_guards("pegcaps", 1, "on", 0xFFC6C830) # PEGCAP_S for PE1CL0
set_guards("peg", 2, "on", 0xFFC6C200)     # PEG for PE2CL1
set_guards("pegcapm", 2, "on", 0xFFC6C840) # PEGCAP_M for PE2CL1
set_guards("pegcaps", 2, "on", 0xFFC6C850) # PEGCAP_S for PE2CL1
set_guards("peg", 3, "on", 0xFFC6C300)     # PEG for PE3CL1
set_guards("pegcapm", 3, "on", 0xFFC6C860) # PEGCAP_M for PE3CL1
set_guards("pegcaps", 3, "on", 0xFFC6C870) # PEGCAP_S for PE3CL1
set_guards("peg", 4, "on", 0xFFC6C400)     # PEG for PE4CL2
set_guards("pegcapm", 4, "on", 0xFFC6C880) # PEGCAP_M for PE4CL2
set_guards("pegcaps", 4, "on", 0xFFC6C890) # PEGCAP_S for PE4CL2
set_guards("peg", 5, "on", 0xFFC6C500)     # PEG for PE5CL2
set_guards("pegcapm", 5, "on", 0xFFC6C8A0) # PEGCAP_M for PE5CL2
set_guards("pegcaps", 5, "on", 0xFFC6C8B0) # PEGCAP_S for PE5CL2

# Allocate CRG
# When CRG isn't used, please set "off" instead of "on" for simulation speed.
# set_guards ("crg_name", CLID, {"on"|"off"}, addr) for CRG
# set_guards ("crg_name", PEID, {"on"|"off"}, addr) only for CRGCAP_PEnCLm
set_guards("crg",      0, "on", 0xFFC6D000) # CRG_CLm for CL0
set_guards("crg",      1, "on", 0xFFC6D100) # CRG_CLm for CL1
set_guards("crg",      2, "on", 0xFFC6D200) # CRG_CLm for CL2
set_guards("crgcapp",  0, "on", 0xFFC6D800) # CRGCAP_PEnCLm for PE0CL0
set_guards("crgcapp",  1, "on", 0xFFC6D810) # CRGCAP_PEnCLm for PE1CL0
set_guards("crgcapp",  2, "on", 0xFFC6D820) # CRGCAP_PEnCLm for PE2CL1
set_guards("crgcapp",  3, "on", 0xFFC6D830) # CRGCAP_PEnCLm for PE3CL1
set_guards("crgcapp",  4, "on", 0xFFC6D840) # CRGCAP_PEnCLm for PE4CL2
set_guards("crgcapp",  5, "on", 0xFFC6D850) # CRGCAP_PEnCLm for PE5CL2
set_guards("crgcapch", 0, "on", 0xFFC6D900) # CRGCAP_CRAMHCLm for CL0
set_guards("crgcapcl", 0, "on", 0xFFC6D910) # CRGCAP_CRAMLCLm for CL0
set_guards("crgcapsh", 0, "on", 0xFFC6D920) # CRGCAP_SX2MBHCLm for CL0
set_guards("crgcapsl", 0, "on", 0xFFC6D930) # CRGCAP_SX2MBLCLm for CL0
set_guards("crgcapch", 1, "on", 0xFFC6D940) # CRGCAP_CRAMHCLm for CL1
set_guards("crgcapcl", 1, "on", 0xFFC6D950) # CRGCAP_CRAMLCLm for CL1
set_guards("crgcapsh", 1, "on", 0xFFC6D960) # CRGCAP_SX2MBHCLm for CL1
set_guards("crgcapsl", 1, "on", 0xFFC6D970) # CRGCAP_SX2MBLCLm for CL1
set_guards("crgcapch", 2, "on", 0xFFC6D980) # CRGCAP_CRAMHCLm for CL2
set_guards("crgcapcl", 2, "on", 0xFFC6D990) # CRGCAP_CRAMLCLm for CL2
set_guards("crgcapsh", 2, "on", 0xFFC6D9A0) # CRGCAP_SX2MBHCLm for CL2
set_guards("crgcapsl", 2, "on", 0xFFC6D9B0) # CRGCAP_SX2MBLCLm for CL2

#set_peinfo(peid, clid, core)
set_peinfo (0,    0,    'g4mh1.1') # PE0CL0
set_peinfo (1,    0,    'g4mh1.1') # PE1CL0
set_peinfo (2,    1,    'g4mh1.1') # PE2CL1
set_peinfo (3,    1,    'g4mh1.1') # PE3CL1
set_peinfo (4,    2,    'g4mh1.1') # PE4CL2
set_peinfo (5,    2,    'g4mh1.1') # PE5CL2

# set resource parameter
set_mpunum (24) # The num of MPU entry # 24 in G4MH1.1
set_l1rlinknum (2) # The num of L1RAM LINK information # 2 in G4MH1.1
set_icsize (16, 4) # Size of Instruction cache # 16KB, 4wayPBS in E2xFCC2
set_fxu_existence (1, "P0") # PID[11] for PE0 : FXU is 0=Unimplemented, 1=Implemented
set_fxu_existence (1, "P1") # PID[11] for PE1 : FXU is 0=Unimplemented, 1=Implemented
set_fxu_existence (0, "P2") # PID[11] for PE2 : FXU is 0=Unimplemented, 1=Implemented
set_fxu_existence (0, "P3") # PID[11] for PE3 : FXU is 0=Unimplemented, 1=Implemented
set_fxu_existence (0, "P4") # PID[11] for PE4 : FXU is 0=Unimplemented, 1=Implemented
set_fxu_existence (0, "P5") # PID[11] for PE5 : FXU is 0=Unimplemented, 1=Implemented

# set external pins (initial value of register)
set_rbase (0x00000000)  # RBASE.RBASE/DV/RINT
set_icctrl (1) # ICCTRL.ICHEN
#set_peid (0, "P0") # PEID.PEID # Default value is same as PEID in E2xFCC2
#set_bmid (0, "P0") # BMID.BMID # Default value is same as PEID in E2xFCC2
#set_spid (0, "P0") # SPID.SPID # Default value is same as PEID in E2xFCC2
set_spidlist (0xFFFFFFFF) # SPIDLIST

#                                   |  same cl   |  diff cl   |
#ms_cl|start     | end       | attr |  F,  R,  W |  F,  R,  W | clid       |outstand | repeat|
ms_cl (0x00000000, 0x002fffff, "RX" , 13,  9,  9,  15, 11, 11, 0,           6,        4) #Code flash BankA @ CL0
ms_cl (0x00300000, 0x005fffff, "RX" , 13,  9,  9,  15, 11, 11, 0,           6,        4) #Code flash BankB @ CL0
ms_cl (0x00600000, 0x008fffff, "RX" , 13,  9,  9,  15, 11, 11, 1,           6,        4) #Code flash BankC @ CL1
ms_cl (0x00900000, 0x00bfffff, "RX" , 13,  9,  9,  15, 11, 11, 1,           6,        4) #Code flash BankD @ CL1
ms_cl (0x00c00000, 0x00dfffff, "RX" , 13,  9,  9,  15, 11, 11, 2,           6,        4) #Code flash BankE @ CL2
ms_cl (0x00e00000, 0x00ffffff, "RX" , 13,  9,  9,  15, 11, 11, 2,           6,        4) #Code flash BankF @ CL2
ms_cl (0xfe000000, 0xfe07ffff, "RWX", 23,  5,  8,  23,  8,  9, 0,           8,        2) #ClusterRAM @ CL0
ms_cl (0xfe080000, 0xfe0fffff, "RWX", 23,  5,  8,  23,  8,  9, 1,           8,        2) #ClusterRAM @ CL1
ms_cl (0xfe100000, 0xfe19ffff, "RWX", 23,  5,  8,  23,  8,  9, 2,           8,        2) #ClusterRAM @ CL2

#                                   |  same pe   |same cl     | diff cl   |
#ms_pe|start     | end       | attr |  F,  R,  W |  F,  R,  W | F,  R,  W | peid       |outstand| repeat|
ms_pe (0xfdc00000, 0xfdc0ffff, "RWX", 15,  2,  5,  15, 11,  9, 19, 15, 11,  0,          12,       1) #LocalRAM PE0
ms_pe (0xfda00000, 0xfda0ffff, "RWX", 15,  2,  5,  15, 11,  9, 19, 15, 11,  0,          12,       1) #LocalRAM PE1
ms_pe (0xfd800000, 0xfd80ffff, "RWX", 15,  2,  5,  15, 11,  9, 19, 15, 11,  0,          12,       1) #LocalRAM PE2
ms_pe (0xfd600000, 0xfd60ffff, "RWX", 15,  2,  5,  15, 11,  9, 19, 15, 11,  0,          12,       1) #LocalRAM PE3
ms_pe (0xfd400000, 0xfd40ffff, "RWX", 15,  2,  5,  15, 11,  9, 19, 15, 11,  0,          12,       1) #LocalRAM PE4
ms_pe (0xfd200000, 0xfd20ffff, "RWX", 15,  2,  5,  15, 11,  9, 19, 15, 11,  0,          12,       1) #LocalRAM PE5
self_set (0xfde00000, 0xfdffffff, -0x00200000) #L1RAM self
self_set (0xfffc0000, 0xfffc3fff,  0x00004000) #Local Peripheral self (INTC1)
self_set (0xfffe0000, 0xffffffff, -0x00002000) #[CForest special] self area for perfcnt and result_register (Reseved area in E2xFCC2)

reset()

#max(0x00FFFFFF) # Stop simulation when the number of executed instructions of any PE exceeds max count.
# max_same_pc (0x10) # Stop simulation when same PC is executed the designated times (to prevent inf loop).

load("")
run()
q()
