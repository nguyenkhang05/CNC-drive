import sys
sys.path.append('./')
import iodef   # load address define
import simmode # indicate simmode is LT/AT
SCHEAP.DummyMasterRvc_SetSimMode("dummy_master",simmode.SIM_MODE)
SCHEAP.setFreq(100,"MHz")

SCHEAP.PSIS011_MessageLevel("psis011","info|error|warning|fatal")
SCHEAP.PSIS011_DumpRegisterRW("psis011","true")
SCHEAP.PSIS011_DumpInterrupt("psis011","true")
#*******************************************
#- 1. Set clocks frequency
#*******************************************
SCHEAP.sc_start(10)
SCHEAP.DummyPeripheralRvc_SetOutputPort("dummy_peripheral","PCLK",100000000) #Set PCLK


SCHEAP.sc_start (50)
SCHEAP.DummyMasterRvc_IssueTransaction("RH850.dummy_master", 0x1, iodef.REG_HAGEDL, 0x4, 0xFFFFFFFF, 0x0, 0x0)
SCHEAP.sc_start (50)
SCHEAP.DummyMasterRvc_IssueTransaction("RH850.dummy_master", 0x0, iodef.REG_HAGEDL, 0x4, 0x0, 0x0, 0x0)
SCHEAP.sc_start (50)
SCHEAP.DummyMasterRvc_ReadReceivedData("RH850.dummy_master", 0xFFFFFFFF)
SCHEAP.sc_start (50)
SCHEAP.DummyPeripheralRvc_SetOutputPort("RH850.dummy_peripheral", "PCLK", 0) #Set PCLK is 0
SCHEAP.sc_start (50)
SCHEAP.DummyMasterRvc_IssueTransaction("RH850.dummy_master", 0x0, iodef.REG_HAGEDL, 0x4, 0x0, 0x0, 0x0) # Can not read
SCHEAP.sc_start (50)
SCHEAP.DummyMasterRvc_ReadReceivedData("RH850.dummy_master", 0x0)
SCHEAP.sc_start (50)
SCHEAP.DummyMasterRvc_IssueTransaction("RH850.dummy_master", 0x1, iodef.REG_HAGEDL, 0x4, 0xAAAAAAAA, 0x0, 0x0) # Can not write
SCHEAP.sc_start (50)
SCHEAP.DummyMasterRvc_IssueTransaction("RH850.dummy_master", 0x0, iodef.REG_HAGEDL, 0x4, 0x0, 0x0, 0x0) # Can not read
SCHEAP.sc_start (50)
SCHEAP.DummyMasterRvc_ReadReceivedData("RH850.dummy_master", 0x0)
SCHEAP.sc_start (50)
SCHEAP.DummyPeripheralRvc_SetOutputPort("RH850.dummy_peripheral", "PCLK", 100000000) #Set PCLK is 100 MHz
SCHEAP.sc_start (50)
SCHEAP.DummyMasterRvc_IssueTransaction("RH850.dummy_master", 0x0, iodef.REG_HAGEDL, 0x4, 0x0, 0x0, 0x0)
SCHEAP.sc_start (50)
SCHEAP.DummyMasterRvc_ReadReceivedData("RH850.dummy_master", 0xFFFFFFFF)
SCHEAP.sc_start (50)
SCHEAP.DummyMasterRvc_IssueTransaction("RH850.dummy_master", 0x1, iodef.REG_HAGEDL, 0x4, 0x55555555, 0x0, 0x0)
SCHEAP.sc_start (50)
SCHEAP.DummyMasterRvc_IssueTransaction("RH850.dummy_master", 0x0, iodef.REG_HAGEDL, 0x4, 0x0, 0x0, 0x0)
SCHEAP.sc_start (50)
SCHEAP.DummyMasterRvc_ReadReceivedData("RH850.dummy_master", 0x55555555)

SCHEAP.sc_start(2)
SCHEAP.DummyMasterRvc_SetTMPass("dummy_master")
SCHEAP.sc_start(10000)
