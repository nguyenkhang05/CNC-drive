import sys
sys.path.append('./')
import iodef
SCHEAP.setFreq(100,"MHz")
SCHEAP.ATU6_MessageLevel("RH850.atu6","info|error|warning|fatal")
SCHEAP.ATU6_DumpInterrupt("RH850.atu6","true")
SCHEAP.ATU6_DumpRegisterRW("RH850.atu6","true")

SCHEAP.sc_start(50000)