import sys
sys.path.append('./')
import iodef   # load address define
import simmode # indicate simmode is LT/AT
SCHEAP.DummyMasterRvc_SetSimMode("RH850.dummy_master",simmode.SIM_MODE)
SCHEAP.DummyMasterRvc_EnableDumpMessage("RH850.dummy_master",1)
SCHEAP.setFreq(1,"MHz")

SCHEAP.HAGE_SetMessageLevel("RH850.hage","info|error|warning|fatal")
SCHEAP.sc_start(0)

SCHEAP.DummyPeripheralRvc_SetOutputPort("RH850.dummy_peripheral","PRESETn",0)
SCHEAP.DummyPeripheralRvc_SetOutputPort("RH850.dummy_peripheral","PCLK",20000000)
SCHEAP.sc_start(50)	

#SCHEAP.DummyMasterRvc_IssueTransaction("RH850.dummy_master",0x1,0xFFBF0000,0x4,0x01020304,0,0x0)
SCHEAP.DummyMasterRvc_IssueTransaction("RH850.dummy_master",0x1,0xFFBF0018,0x4, 4 , 0, 0x0)
SCHEAP.sc_start(20)

SCHEAP.DummyMasterRvc_IssueTransaction("RH850.dummy_master",0x1,0xFFBF0010,0x4, 0x400<<2 , 0, 0x0)
SCHEAP.sc_start(20)

SCHEAP.DummyMasterRvc_IssueTransaction("RH850.dummy_master",0x0,0xFFBF0000,0x4,0x01020304,0,0x0)
SCHEAP.sc_start(20)

SCHEAP.DummyPeripheralRvc_CreateMemory("RH850.dummy_peripheral", 0x400, 0x100)
SCHEAP.sc_start(10)
SCHEAP.DummyPeripheralRvc_SetMemoryData("RH850.dummy_peripheral", 0x400, 0xAAAAAAAA)
SCHEAP.sc_start(10)

SCHEAP.DummyMasterRvc_IssueTransaction("RH850.dummy_master",0x1,0xFFBF0000,0x4, (1<<24) | (1<<1) | (1<<2) , 0, 0x0)
SCHEAP.sc_start(20)

SCHEAP.DummyMasterRvc_SetTMPass("RH850.dummy_master")
SCHEAP.sc_start(1000)
