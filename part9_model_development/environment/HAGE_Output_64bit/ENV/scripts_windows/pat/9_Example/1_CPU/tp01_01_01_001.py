import sys
sys.path.append('./')
import iodef
import simmode #indicate simmode is LT/AT

SCHEAP.setFreq(100,"MHz")

print ("--------------------------- Start sample ---------------------------")
SCHEAP.writeMemValue("RH850.G4SS.G4PE.G4CPU", 0, 0x00000004, 0x0102030405060708, 8)
SCHEAP.sc_start(20)
value = SCHEAP.readMemValue("RH850.G4SS.G4PE.G4CPU", 0, 0x00000004, 4)
SCHEAP.sc_start(20)
print ("Read value: %s" % hex(value))
value = SCHEAP.readMemValue("RH850.G4SS.G4PE.G4CPU", 0, 0x00000008, 4)
SCHEAP.sc_start(20)
print ("Read value: %s" % hex(value))
SCHEAP.getCurrentPc("RH850.G4SS.G4PE.G4CPU", 0)
SCHEAP.dispExInstNum("RH850.G4SS.G4PE.G4CPU", 0)
print ("--------------------------- End sample   ---------------------------")
SCHEAP.sc_start(20)
