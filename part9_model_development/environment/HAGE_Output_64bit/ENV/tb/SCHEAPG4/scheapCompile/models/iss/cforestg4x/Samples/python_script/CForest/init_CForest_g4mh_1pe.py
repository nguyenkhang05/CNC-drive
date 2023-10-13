# -*- coding: utf-8 -*-
set_has_mpu ("on");
set_has_fxu ("on");
set_peripherals ("ipir",    "off");
set_peripherals ("mecnt",   "off");
set_peripherals ("barrier", "off");
set_peripherals ("intc1",   "on");
set_peripherals ("intc2",   "on");
set_peripherals ("simio",   "on"); # result_register
set_peripherals ("perf",    "on"); # perfcnt

# Allocate PEG
set_guards("peg", 0, "on", 0xFFC6C000)     # PEG for PE0CL0
set_guards("pegcapm", 0, "on", 0xFFC6C800) # PEGCAP_M for PE0CL0
set_guards("pegcaps", 0, "on", 0xFFC6C810) # PEGCAP_S for PE0CL0
set_guards("peg", 1, "on", 0xFFC6C100)     # PEG for PE1CL0
set_guards("pegcapm", 1, "on", 0xFFC6C820) # PEGCAP_M for PE1CL0
set_guards("pegcaps", 1, "on", 0xFFC6C830) # PEGCAP_S for PE1CL0

# Allocate CRG
set_guards("crg",      0, "on", 0xFFC6D000) # CRG_CLm for CL0
set_guards("crg",      1, "on", 0xFFC6D100) # CRG_CLm for CL1
set_guards("crgcapp",  0, "on", 0xFFC6D800) # CRGCAP_PEn for PE0CL0
set_guards("crgcapp",  1, "on", 0xFFC6D810) # CRGCAP_PEn for PE1CL0
set_guards("crgcapch", 0, "on", 0xFFC6D900) # CRGCAP_CRAMHCLm for CL0
set_guards("crgcapcl", 0, "on", 0xFFC6D910) # CRGCAP_CRAMLCLm for CL0
set_guards("crgcapsh", 0, "on", 0xFFC6D920) # CRGCAP_SX2MBHCLm for CL0
set_guards("crgcapsl", 0, "on", 0xFFC6D930) # CRGCAP_SX2MBLCLm for CL0
set_guards("crgcapch", 1, "on", 0xFFC6D940) # CRGCAP_CRAMHCLm for CL1
set_guards("crgcapcl", 1, "on", 0xFFC6D950) # CRGCAP_CRAMLCLm for CL1
set_guards("crgcapsh", 1, "on", 0xFFC6D960) # CRGCAP_SX2MBHCLm for CL1
set_guards("crgcapsl", 1, "on", 0xFFC6D970) # CRGCAP_SX2MBLCLm for CL1

#set_peinfo(peid, clid, core)
set_peinfo (0,    0,    'g4mh')
set_mpunum (24);

# set external pins
#set_rbase (0, "P0")  # RBASE.RBASE/DV/RINT
#set_icctrl (1, "P0") # ICCTRL.ICHEN
#set_peid (0, "P0")
#set_bmid (0, "P0")
#set_spidlist (0xFFFFFFFF, "P0")
#set_spid (0, "P0")
#set_mpudmdp (0, "P0")


#error_area_set (0x20000000, 0x2000FFFF)
#error_area_clear (0x20000000)

#                                   |  same cl   |  diff cl   |
#ms_cl|start     | end       | attr |  F,  R,  W |  F,  R,  W | clid       |outstand | repeat|
ms_cl (0x00000000, 0x01ffffff, "RX" , 12,  8,  8,  12,  8,  8, 0,           6,        4) #FlashROM
ms_cl (0xfe000000, 0xfeffffff, "RWX", 23,  5,  7,  23,  5,  7, 0,           8,        2) #CRAM @ CL0

#                                   |  same pe   |same cl     | diff cl   |
#ms_pe|start     | end       | attr |  F,  R,  W |  F,  R,  W | F,  R,  W | peid       |outstand| repeat|
ms_pe (0xfdc00000, 0xfddfffff, "RWX", 15,  2,  5,  15, 11,  9, 19, 16, 11,  0,          12,       1) #L1RAM PE0

self_set (0xfde00000, 0xfdffffff, -0x00200000) #L1RAM self
self_set (0xfffc0000, 0xfffc3fff,  0x00004000) #Local Peripheral self (INTC1)
self_set (0xfffe0000, 0xffffffff, -0x00002000) #self area for perfcnt and result_register
reset()
max(0x00FFFFFF)
max_same_pc (0x10)
load("")
run()
q()
