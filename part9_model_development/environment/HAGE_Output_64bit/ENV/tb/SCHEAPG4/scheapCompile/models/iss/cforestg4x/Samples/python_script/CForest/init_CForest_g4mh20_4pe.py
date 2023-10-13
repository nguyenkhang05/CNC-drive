# -*- coding: utf-8 -*-
set_has_mpu ("on");
set_has_fxu ("on");
set_has_hv ("on")
set_peripherals ("ipir",    "off");
set_peripherals ("mecnt",   "off");
set_peripherals ("barrier", "off");
set_peripherals ("intc1",   "on");
set_peripherals ("intc2",   "on");
set_peripherals ("simio",   "on"); # result_register
set_peripherals ("perf",    "off"); # perfcnt

# Allocate PEG
set_guards("peg",     0, "on", 0xFFC6C000)  # PEG for PE0CL0
set_guards("pegcapm", 0, "on", 0xFFC6C800)  # PEGCAP_M for PE0CL0
set_guards("pegcaps", 0, "on", 0xFFC6C820)  # PEGCAP_S for PE0CL0
set_guards("peg",     1, "on", 0xFFC6C100)  # PEG for PE1CL0
set_guards("pegcapm", 1, "on", 0xFFC6C840)  # PEGCAP_M for PE1CL0
set_guards("pegcaps", 1, "on", 0xFFC6C860)  # PEGCAP_S for PE1CL0
set_guards("peg",     2, "on", 0xFFC6C200)  # PEG for PE2CL1
set_guards("pegcapm", 2, "on", 0xFFC6C880)  # PEGCAP_M for PE2CL1
set_guards("pegcaps", 2, "on", 0xFFC6C8A0)  # PEGCAP_S for PE2CL1
set_guards("peg",     3, "on", 0xFFC6C300)  # PEG for PE3CL1
set_guards("pegcapm", 3, "on", 0xFFC6C8C0)  # PEGCAP_M for PE3CL1
set_guards("pegcaps", 3, "on", 0xFFC6C8E0)  # PEGCAP_S for PE3CL1

# Allocate CRG
set_guards("crg",      0, "on", 0xFFC6D000) # CRG_CLm for CL0
set_guards("crg",      1, "on", 0xFFC6D200) # CRG_CLm for CL1
set_guards("crg",      2, "on", 0xFFC6D400) # CRG_CLm for CL2
set_guards("crg",      3, "on", 0xFFC6D800) # CRG_CLm for CL3
set_guards("crgivc",   0, "on", 0xFFC6E800) # CRG_IVC_CLm for CL0
set_guards("crgivc",   1, "on", 0xFFC6EA00) # CRG_IVC_CLm for CL1
set_guards("crgivc",   2, "on", 0xFFC6EC00) # CRG_IVC_CLm for CL2
set_guards("crgivc",   3, "on", 0xFFC6EE00) # CRG_IVC_CLm for CL3
set_guards("crgcsg",   0, "on", 0xFFC6E000) # CRG_CSG_CLm for CL0
set_guards("crgcsg",   1, "on", 0xFFC6E200) # CRG_CSG_CLm for CL1
set_guards("crgcsg",   2, "on", 0xFFC6E400) # CRG_CSG_CLm for CL2
set_guards("crgcsg",   3, "on", 0xFFC6E600) # CRG_CSG_CLm for CL3
set_guards("crgcapp",  0, "on", 0xFFC6D800) # CRGCAP_PEn for PE0CL0
set_guards("crgcapp",  1, "on", 0xFFC6D820) # CRGCAP_PEn for PE1CL0
set_guards("crgcapp",  2, "on", 0xFFC6D840) # CRGCAP_PEn for PE2CL1
set_guards("crgcapp",  3, "on", 0xFFC6D860) # CRGCAP_PEn for PE3CL1
set_guards("crgcapch", 0, "on", 0xFFC6DA00) # CRGCAP_CRAMHCLm for CL0
set_guards("crgcapcl", 0, "on", 0xFFC6DA20) # CRGCAP_CRAMLCLm for CL0
set_guards("crgcapsh", 0, "on", 0xFFC6DA40) # CRGCAP_SX2MBHCLm for CL0
set_guards("crgcapsl", 0, "on", 0xFFC6DA60) # CRGCAP_SX2MBLCLm for CL0
set_guards("crgcapch", 1, "on", 0xFFC6DA80) # CRGCAP_CRAMHCLm for CL1
set_guards("crgcapcl", 1, "on", 0xFFC6DAA0) # CRGCAP_CRAMLCLm for CL1
set_guards("crgcapsh", 1, "on", 0xFFC6DAC0) # CRGCAP_SX2MBHCLm for CL1
set_guards("crgcapsl", 1, "on", 0xFFC6DAE0) # CRGCAP_SX2MBLCLm for CL1
set_guards("crgcapch", 2, "on", 0xFFC6DB00) # CRGCAP_CRAMHCLm for CL2
set_guards("crgcapcl", 2, "on", 0xFFC6DB20) # CRGCAP_CRAMLCLm for CL2
set_guards("crgcapsh", 2, "on", 0xFFC6DB40) # CRGCAP_SX2MBHCLm for CL2
set_guards("crgcapsl", 2, "on", 0xFFC6DB60) # CRGCAP_SX2MBLCLm for CL2
set_guards("crgcapch", 3, "on", 0xFFC6DB80) # CRGCAP_CRAMHCLm for CL3
set_guards("crgcapcl", 3, "on", 0xFFC6DBA0) # CRGCAP_CRAMLCLm for CL3
set_guards("crgcapsh", 3, "on", 0xFFC6DBC0) # CRGCAP_SX2MBHCLm for CL3
set_guards("crgcapsl", 3, "on", 0xFFC6DBE0) # CRGCAP_SX2MBLCLm for CL3
# CRG settings:
# Not supported yet #set_guards_param ("crg", 0, "crgcrgch", 8)  # Set 8  for CRG:CRG channel of CRG_CL0
# Not supported yet #set_guards_param ("crg", 0, "crgcsgch", 4)  # Set 4  for CRG:CSG channel of CRG_CL0
# Not supported yet #set_guards_param ("crg", 0, "crgkeycode", 0x00000000) # Set keycode of CRG_CL0
set_guards_param ("crg", 0, "crgaddrw", 19) # Set 19 (512K) for ADDRW of CRG_CL0
set_guards_param ("crg", 1, "crgaddrw", 19) # Set 19 (512K) for ADDRW of CRG_CL1
set_guards_param ("crg", 2, "crgaddrw", 21) # Set 21 (2048K) for ADDRW of CRG_CL2
set_guards_param ("crg", 3, "crgaddrw", 18) # Set 18 (256K) for ADDRW of CRG_CL3

#set_peinfo(peid, clid, core)
set_peinfo (0,    0,    'g4mh2.0') # PE0CL0
set_peinfo (1,    0,    'g4mh2.0') # PE1CL0
set_peinfo (2,    1,    'g4mh2.0') # PE2CL1
set_peinfo (3,    1,    'g4mh2.0') # PE3CL1

# set external pins and parameters
# When PEID string is omitted, param is applied to all PE.
set_mpunum (32); # The number of MPU entry (MPCFG.NMPUE+1)
set_l1rlinknum (2) # L1RCFG.L1RLNKE
set_l1rsize (0x20) # L1RCFG.L1RSIZE
set_icsize (16, 4) # IC size[KB], way
set_lscfg (0) # LSCFG.LSE
#set_rbase (0, "P0")  # RBASE.RBASE/DV/RINT
#set_icctrl (1, "P0") # ICCTRL.ICHEN
#set_peid (0, "P0") # PEID.PEID
#set_bmid (0, "P0") # BMID.BMID
#set_spidlist (0xFFFFFFFF, "P0") # SPIDLIST
#set_spid (0, "P0") # SPID.SPID
#set_mpudmdp (0, "P0") # MPCFG.DMDP
#set_mpubank (1); # The number of MPU bank (MPCFG.NBK+1)
#set_snooze (32) # SNZCFG.SNZCNT
#set_hvcfg (1) # HVCFG.HVE, This setting is valid when set_has_hv("on")
#set_dbgen (0) # DBGEN.HE/GE

#error_area_set (0x20000000, 0x2000FFFF)
#error_area_clear (0x20000000)

#                                   |  same cl   |  diff cl   |
#ms_cl|start     | end       | attr |  F,  R,  W |  F,  R,  W | clid       |outstand | repeat|
ms_cl (0x00000000, 0x007fffff, "RX" , 12,  8,  8,  12, 10, 10, 0,           6,        4) #FlashROM 8192K @ CL0
ms_cl (0x00800000, 0x00ffffff, "RX" , 12,  8,  8,  12, 10, 10, 1,           6,        4) #FlashROM 8192K @ CL1
ms_cl (0xfe000000, 0xfe07ffff, "RWX", 23,  5,  7,  23,  8,  9, 0,           8,        2) #CRAM 512K @ CL0
ms_cl (0xfe100000, 0xfe17ffff, "RWX", 23,  5,  7,  23,  8,  9, 1,           8,        2) #CRAM 512K @ CL1
ms_cl (0xfe400000, 0xfe5fffff, "RWX", 23,  5,  7,  23,  8,  9, 2,           8,        2) #CRAM 2048K @ CL2
ms_cl (0xfe800000, 0xfe83ffff, "RWX", 23,  5,  7,  23,  8,  9, 3,           8,        2) #CRAM 256K @ CL3

#                                   |  same pe   |same cl     | diff cl   |
#ms_pe|start     | end       | attr |  F,  R,  W |  F,  R,  W | F,  R,  W | peid       |outstand| repeat|
ms_pe (0xfdc00000, 0xfdc0ffff, "RWX", 15,  2,  5,  15, 11,  9, 19, 16, 11,  0,          12,       1) #L1RAM 64K PE0
ms_pe (0xfda00000, 0xfda0ffff, "RWX", 15,  2,  5,  15, 11,  9, 19, 16, 11,  1,          12,       1) #L1RAM 64K PE1
ms_pe (0xfd800000, 0xfd80ffff, "RWX", 15,  2,  5,  15, 11,  9, 19, 16, 11,  2,          12,       1) #L1RAM 64K PE2
ms_pe (0xfd600000, 0xfd60ffff, "RWX", 15,  2,  5,  15, 11,  9, 19, 16, 11,  3,          12,       1) #L1RAM 64K PE3

self_set (0xfde00000, 0xfdffffff, -0x00200000) #L1RAM self
self_set (0xfffc0000, 0xfffc3fff,  0x00004000) #Local Peripheral self (INTC1)
self_set (0xfffe0000, 0xffffffff, -0x00002000) #self area for perfcnt and result_register
reset()
max(0x00FFFFFF)
max_same_pc (0x10) # To prevent infinit-loop, stop simulation when same PC is executed "n" times continuously.
load("")
run()
q()
