# ---------------------------------------------------------
# Peripheral setting:
# * INTC1
# * INTC2
# ---------------------------------------------------------
set_has_mpu ("on");
set_has_fxu ("on");
set_peripherals ("ipir",    "off");
set_peripherals ("mecnt",   "off");
set_peripherals ("barrier", "off");
set_peripherals ("intc1",   "on");
set_peripherals ("intc2",   "on");
set_peripherals ("simio",   "on"); # result_register
set_peripherals ("perf",    "on"); # perfcnt

# ---------------------------------------------------------
# Create new PE instance.
# ---------------------------------------------------------
#set_peinfo(peid, clid, core)
set_peinfo (0,    0,    'g4mh')

# ---------------------------------------------------------
# System register setting
# ---------------------------------------------------------
set_mpunum (24);
#set_rbase (0, "P0NC")  # RBASE.RBASE/DV/RINT
#set_icctrl (1, "P0NC") # ICCTRL.ICHEN
#set_peid (0, "P0NC")
#set_bmid (0, "P0NC")
#set_spidlist (0xFFFFFFFF, "P0NC")
#set_spid (0, "P0NC")
#set_mpudmdp (0, "P0NC")

#if force_maaen == 1:
#    set_mctl (0x80000002)
#else:
#    set_mctl (0x80000000)

# ---------------------------------------------------------
# Create local ram instance
# ---------------------------------------------------------
#                                   |  same pe   |same cl     | diff cl   |
#ms_pe|start     | end       | attr |  F,  R,  W |  F,  R,  W | F,  R,  W | peid       |outstand| repeat|
ms_pe (0xFEA00000, 0xFEBFFFFF, "RWX", 15,  2,  5,  15, 11,  9, 19, 16, 11,  0,          12,       1) #L1RAM PE0
#                                   |  same cl   |  diff cl   |
#ms_cl|start     | end       | attr |  F,  R,  W |  F,  R,  W | clid       |outstand | repeat|
ms_cl (0x10000000, 0x1FEFFFFF, "RWX", 23,  5,  7,  23,  5,  7, 0,           8,        2)

# ---------------------------------------------------------
# Create rom instance.
# ---------------------------------------------------------
#                                   |  same cl   |  diff cl   |
#ms_cl|start     | end       | attr |  F,  R,  W |  F,  R,  W | clid       |outstand | repeat|
ms_cl (0x00000000, 0x01FFFFFF, "RX" , 12,  8,  8,  12,  8,  8, 0,           6,         4) #FlashROM

# ---------------------------------------------------------
# Create gapb ram instance.
# ---------------------------------------------------------
#                                   |  same cl   |  diff cl   |
#ms_cl|start     | end       | attr |  F,  R,  W |  F,  R,  W | clid       |outstand | repeat|
ms_cl (0xFEE00000, 0xFEFFFFFF, "RWX", 23,  5,  7,  23,  5,  7, 0,           8,        2) # GRAM

# ---------------------------------------------------------
# Set address table to each PE
# ---------------------------------------------------------
# PE 1
# Self LRAM 192KB
self_set (0xFEC00000, 0xFEDFFFFF, -0x00200000)
self_set (0xfffc0000, 0xfffc3fff,  0x00004000) #Local Peripheral self (INTC1)
self_set (0xfffe0000, 0xffffffff, -0x00002000) #self area for perfcnt and result_register

# Other configuration
# show memory information
mi()

# CForest standalone
#debug ("on")

# download hex file
load('', 0x00000000)

# set simluation max steps
#max(200000)

reset()

# Xdump setting
#xdump_fpu(212, 1) # recipf.d
#xdump_fpu(213, 1) # recipf.s
xdump_fpu(222, 1) # rsqrtf.d
xdump_fpu(223, 1) # rsqrtf.s
xdump_fpu(262, 1) # recipf.s4
xdump_fpu(263, 1) # rsqrtf.s4

# xdump_data (start_addr, end_addr, mask)
xdump_data (0xffff7ff0, 0xffff7fff, 0xffffffff)
xdump_data (0xfffeedf0, 0xfffeedff, 0xffffffff)
xdump_data (0xefffe000, 0xefffefff, 0xffffffff)
xdump_data (0xeffff000, 0xeffff0ff, 0xffffffff)

# CForest standalone
# start simulation
#run()

# exit
#q()
