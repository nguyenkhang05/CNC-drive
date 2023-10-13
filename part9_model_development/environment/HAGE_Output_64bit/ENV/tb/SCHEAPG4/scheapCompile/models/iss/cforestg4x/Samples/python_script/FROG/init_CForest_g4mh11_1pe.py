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

#set_peinfo(peid, clid, core)
set_peinfo (0,    0,    'g4mh1.1')
set_mpunum (24);
set_l1rlinknum (2)
set_icsize (32, 4)

# set external pins
#set_rbase (0, "P0NC")  # RBASE.RBASE/DV/RINT
#set_icctrl (1, "P0NC") # ICCTRL.ICHEN
#set_peid (0, "P0NC")
#set_bmid (0, "P0NC")
#set_spidlist (0xFFFFFFFF, "P0NC")
#set_spid (0, "P0NC")
#set_mpudmdp (0, "P0NC")


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
