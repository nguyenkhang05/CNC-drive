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

#set_peinfo(peid, clid, core)
set_peinfo (0,    0,    'g4mh2.0')

# set external pins and parameters
set_mpunum (32); # The number of MPU entry (MPCFG.NMPUE+1)
set_l1rlinknum (2) # L1RCFG.L1RLNKE
set_l1rsize (0x20) # L1RCFG.L1RSIZE
set_icsize (32, 4) # IC size[KB], way
set_lscfg (0) # LSCFG.LSE
#set_rbase (0, "P0NC")  # RBASE.RBASE/DV/RINT
#set_icctrl (1, "P0NC") # ICCTRL.ICHEN
#set_peid (0, "P0NC") # PEID.PEID
#set_bmid (0, "P0NC") # BMID.BMID
#set_spidlist (0xFFFFFFFF, "P0NC") # SPIDLIST
#set_spid (0, "P0NC") # SPID.SPID
#set_mpudmdp (0, "P0NC") # MPCFG.DMDP
#set_mpubank (1); # The number of MPU bank (MPCFG.NBK+1)
#set_snooze (32) # SNZCFG.SNZCNT
#set_hvcfg (1) # HVCFG.HVE, This setting is valid when set_has_hv("on")
#set_dbgen (0) # DBGEN.HE/GE

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
load("")
run()
q()
