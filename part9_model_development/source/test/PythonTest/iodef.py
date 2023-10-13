import  string

DSADC_base = 0xFFF24000
DSADC00_base = 0xFFF24200
DSADC10_base = 0xFFF24400
DSADC20_base = 0xFFF24600
DSADC12_base = 0xFFF24800
DSADC13_base = 0xFFCA2000
DSADC11_base = 0xFFCA2200
DSADC15_base = 0xFFCA2400
DSADC14_base = 0xFFCA2600
DSADC22_base = 0xFFCA2800
DSADC21_base = 0xFFCA2A00


DSADTRG_PortValue =  0x7FF #initialized true


dict_unit = {"00":DSADC00_base,
             "10":DSADC10_base,
             "20":DSADC20_base,
             "12":DSADC12_base,
             "13":DSADC13_base,
             "11":DSADC11_base,
             "15":DSADC15_base,
             "14":DSADC14_base,
             "22":DSADC22_base,
             "21":DSADC21_base}

name_unit = {"00":0x1,
             "10":0x2,
             "20":0x4,
             "12":0x8,
             "13":0x10,
             "11":0x20,
             "15":0x40,
             "14":0x80,
             "22":0x0100,
             "21":0x0200}


# Common Register/Config
REG_DSADCSYNSTCR        = (DSADC_base + 0x00)
REG_DSADCADGCR          = (DSADC_base + 0x04)
REG_DSADCTDCR           = (DSADC_base + 0x0C)

def REG_DSADCVCR(m, n):
    addr = dict_unit[n] + m*0x04
    return addr
def REG_DSADCDIR(m ,n):
    addr = dict_unit[n] + 0x20 + m*0x04
    return addr
def REG_DSADCADSTCR(n):
    addr = dict_unit[n] + 0x40
    return addr
def REG_DSADCADENDCR(n):
    addr = dict_unit[n] + 0x44
    return addr
def REG_DSADCCLBSTCR(n):
    addr = dict_unit[n] + 0x48
    return addr
def REG_DSADCCLBEDCR(n):
    addr = dict_unit[n] + 0x4C
    return addr
def REG_DSADCADTCR(n):
    addr = dict_unit[n] + 0x50
    return addr
def REG_DSADCUCR(n):
    addr = dict_unit[n] + 0x54
    return addr     
def REG_DSADCVCPTRR(n):
    addr = dict_unit[n] + 0x58
    return addr
def REG_DSADCADSR(n):
    addr = dict_unit[n] + 0x60
    return addr
def REG_DSADCUDPTRR(n):
    addr = dict_unit[n] + 0x64
    return addr
def REG_DSADCTSVAL(n):        
    addr = dict_unit[n] + 0x6C
    return addr
def REG_DSADCSFTCR(n):       
    addr = dict_unit[n] + 0x70
    return addr
def REG_DSADCECR(n):         
    addr = dict_unit[n] + 0x74
    return addr
def REG_DSADCER(n):          
    addr = dict_unit[n] + 0x78
    return addr
def REG_DSADCTDLVR(n):       
    addr = dict_unit[n] + 0x7C
    return addr
def REG_DSADCTRCR(n):        
    addr = dict_unit[n] + 0xB0
    return addr
def REG_DSADCVBIASR(n):     
    addr = dict_unit[n] + 0xB4
    return addr
def REG_DSADCUDIR(n):        
    addr = dict_unit[n] + 0x68
    return addr
def REG_DSADCULTBR(m ,n):
    addr = dict_unit[n] +0x80 + m*0x04
    return addr
def ReleaseSWRst():
    return  
    
def SetPort(SCHEAP, name, port, value):
    global DSADTRG_PortValue
    if (value == 1):
        DSADTRG_PortValue = (DSADTRG_PortValue | name_unit[port[7:9]])
    else:
        DSADTRG_PortValue = (DSADTRG_PortValue & (~ name_unit[port[7:9]]))
    SCHEAP.DummyPeripheralRvc_SetOutputPort(name,port,DSADTRG_PortValue)
    return


# Reserved Area
RESERVEDAREA0_ADDR      = (DSADC_base + 0x5) # [0x5,0xC)
RESERVEDAREA1_ADDR      = (DSADC_base + 0xD) # [0xD,0x20)
RESERVEDAREA2_ADDR      = (DSADC_base + 0x24) # [0x24,0x40)
RESERVEDAREA3_ADDR      = (DSADC_base + 0x41) # [0x41,0x44)
RESERVEDAREA4_ADDR      = (DSADC_base + 0x45) # [0x45,0x48)
RESERVEDAREA5_ADDR      = (DSADC_base + 0x49) # [0x49,0x4C)
RESERVEDAREA6_ADDR      = (DSADC_base + 0x4D) # [0x4D,0x50)
RESERVEDAREA7_ADDR      = (DSADC_base + 0x51) # [0x51,0x54)
RESERVEDAREA8_ADDR      = (DSADC_base + 0x59) # [0x59,0x60)
RESERVEDAREA9_ADDR      = (DSADC_base + 0x61) # [0x61,0x64)
RESERVEDAREA10_ADDR     = (DSADC_base + 0x65) # [0x65,0x68)
RESERVEDAREA11_ADDR     = (DSADC_base + 0x71) # [0x71,0x74)
RESERVEDAREA12_ADDR     = (DSADC_base + 0x75) # [0x75,0x78)
RESERVEDAREA13_ADDR     = (DSADC_base + 0x7D) # [0x7D,0x80)
RESERVEDAREA14_ADDR     = (DSADC_base + 0x84) # [0x84,0xB0)
RESERVEDAREA15_ADDR     = (DSADC_base + 0xB1) # [0xB1,0xB4)
RESERVEDAREA16_ADDR     = (DSADC_base + 0xB5) # [0xB5,0x100)

# Debug Accessing
DEBUG11_ADDR            = (DSADC_base + 0x4)
DEBUG011_ADDR           = (DSADC_base + 0xB)
DEBUG022_ADDR           = (DSADC_base + 0x1E)
DEBUG22_ADDR            = (DSADC_base + 0x22)