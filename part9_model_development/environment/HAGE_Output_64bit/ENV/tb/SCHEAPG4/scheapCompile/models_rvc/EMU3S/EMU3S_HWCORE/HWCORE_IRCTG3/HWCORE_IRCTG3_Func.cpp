// ---------------------------------------------------------------------
// $Id: HWCORE_IRCTG3_Func.cpp,v 1.10 2020/10/29 14:32:14 khoaanhnguyen Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of HWCORE_IRCTG3_Func.cpp
// Description: About the include header files of HWCORE_IRCTG3_Func.cpp
#include "HWCORE_IRCTG3_AgentController.h"
#include "HWCORE_IRCTG3_Func.h"

#ifndef func_re_printf
#define func_re_printf get_fileline(__FILE__, __LINE__); mpIRCTG3_AgentController->_re_printf
#endif//func_re_printf

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
///*********************************
/// Function     : HWCORE_IRCTG3_Func 
/// Description  : Constructor of HWCORE_IRCTG3_Func class
/// Parameters   : 
///                sc_module_name name: name of model
///                HWCORE_IRCTG3_AgentController* HWCORE_IRCTG3_AgentControllerPtr: pointer to interface to communicate with HWCORE_IRCTG3 class
/// Return value : None
///*********************************
HWCORE_IRCTG3_Func::HWCORE_IRCTG3_Func(std::string name, HWCORE_IRCTG3_AgentController* HWCORE_IRCTG3_AgentControllerPtr) :
    Chwcore_irctg3_regif(name, 32)
{//{{{
    assert(HWCORE_IRCTG3_AgentControllerPtr!= NULL);
    mpIRCTG3_AgentController = HWCORE_IRCTG3_AgentControllerPtr;
    mpIRCTG3_AgentController->CommandInit(name);
    mModuleName = name;

    Chwcore_irctg3_regif::set_instance_name(name);
    Initialize();
    
    // Create register map
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3CCTR           ,(&IR3CCTR      )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3CMODE          ,(&IR3CMODE     )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3RDI            ,(&IR3RDI       )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3TRSMODE        ,(&IR3TRSMODE   )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3TRG            ,(&IR3TRG       )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3INTCLEAR       ,(&IR3INTCLEAR  )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3INTEN          ,(&IR3INTEN     )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3INTST          ,(&IR3INTST     )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3UINTMEM        ,(&IR3UINTMEM   )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3VINTMEM        ,(&IR3VINTMEM   )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3WINTMEM        ,(&IR3WINTMEM   )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3ADCCLEAR       ,(&IR3ADCCLEAR  )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3ADCEN          ,(&IR3ADCEN     )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3ADCST          ,(&IR3ADCST     )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3UADCMEM        ,(&IR3UADCMEM   )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3VADCMEM        ,(&IR3VADCMEM   )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3WADCMEM        ,(&IR3WADCMEM   )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3COFSALL        ,(&IR3COFSALL   )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3COFSU          ,(&IR3COFSU     )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3COFSV          ,(&IR3COFSV     )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3COFSW          ,(&IR3COFSW     )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3CMPWRMD        ,(&IR3CMPWRMD   )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3CMPRDMD        ,(&IR3CMPRDMD   )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3VALN           ,(&IR3VALN      )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3INT            ,(IR3INT        )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3ADC            ,(IR3ADC        )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register**>(REG_IR3CMP            ,(IR3CMP        )));

    //Table of valid zone mask
    m_VldZoneMask[HWCORE_IRCTG3_AgentController::emU_PHASE] = 0x7F;
    m_VldZoneMask[HWCORE_IRCTG3_AgentController::emV_PHASE] = 0x3F;
    m_VldZoneMask[HWCORE_IRCTG3_AgentController::emW_PHASE] = 0x3F;
    //Table of max valid zone
    m_MaxVldZone[HWCORE_IRCTG3_AgentController::emCommon][emSawtooth] = MAX_VLD_U;
    m_MaxVldZone[HWCORE_IRCTG3_AgentController::emCommon][emTriangle] = MAX_VLD_U / 2;
    m_MaxVldZone[HWCORE_IRCTG3_AgentController::emCommon][emSawtoothW] = MAX_VLD_U / 2;
    m_MaxVldZone[HWCORE_IRCTG3_AgentController::emIndependent][emSawtooth] = MAX_VLD_V;
    m_MaxVldZone[HWCORE_IRCTG3_AgentController::emIndependent][emTriangle] = MAX_VLD_V / 2;
    m_MaxVldZone[HWCORE_IRCTG3_AgentController::emIndependent][emSawtoothW] = MAX_VLD_V / 2;
    //Initialize angle related buffers
    m_bIsAngChange = false;
    m_bIsOfsAngChange = false;
    memset(m_InpAng, 0, sizeof(m_InpAng));
    memset(m_OffAng, 0, sizeof(m_OffAng));
}//}}}

///*********************************
/// Function     : ~HWCORE_IRCTG3_Func 
/// Description  : Destructor of HWCORE_IRCTG3_Func class
/// Parameters   : None
/// Return value : None
///*********************************
HWCORE_IRCTG3_Func::~HWCORE_IRCTG3_Func()
{//{{{
    if (mRegisterMap.size() != 0) {
        mRegisterMap.clear();
    }
}//}}}

///*********************************
/// Function     : EnableReset
/// Description  : reset member variables and registers
/// Parameters   : 
///                const bool is_active: reset flag (true: reset, false: not reset)
/// Return value : None
///*********************************
// Description: Reset Func and registers
void HWCORE_IRCTG3_Func::EnableReset(const bool is_active)
{//{{{
    if (is_active) {
        //mip_prst_n is asserted
        if (mpIRCTG3_AgentController->GetResetStatus("mip_prst_n") == true) {
            Initialize();
        }
        //PRESETn is asserted
        else if (mpIRCTG3_AgentController->GetResetStatus("PRESETn") == true){
            m_State = emStopped;
            m_IsTrigger = false;
            m_pCurZone = NULL;
            m_pPreZone = NULL;
            memset(m_RetBuff, 0, sizeof(m_RetBuff));
            memset(m_TransMode, emBatch, sizeof(m_TransMode));
            memset(m_DetectZoneNo, MAX_CHAR, sizeof(m_DetectZoneNo));
        }
    }
    Chwcore_irctg3_regif::EnableReset(is_active);
}//}}}

///*********************************
/// Function     : Initialize
/// Description  : Initialize member variables
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_IRCTG3_Func::Initialize(void)
{//{{{
    //Initialize reference buffers' value
    m_RefBuff_CMODE = 0;
    m_RefBuff_INTEN = 0;
    m_RefBuff_ADCEN = 0;
    m_RefBuff_OFSAL = 0;
    m_RefBuff_OFSU = 0;
    m_RefBuff_OFSV = 0;
    m_RefBuff_OFSW = 0;
    m_RefBuff_VALN = 0;
    memset(m_RefBuff_INT, false, sizeof(m_RefBuff_INT));
    memset(m_RefBuff_ADC, false, sizeof(m_RefBuff_ADC));
    memset(m_RefBuff_CMP, false, sizeof(m_RefBuff_CMP));
    //Initialize retention buffers' value
    memset(m_RetBuff, 0, sizeof(m_RetBuff));
    //Initialize other member variables
    m_IsTrigger = false;
    m_CompMode = HWCORE_IRCTG3_AgentController::emCommon;
    m_WaveMode = emSawtooth;
    m_SetRotDir = emPositive;
    m_ActualRotDir = emPositive;
    m_Phase = HWCORE_IRCTG3_AgentController::emU_PHASE;
    m_State = emStopped;
    memset(m_bIntEn, false, sizeof(m_bIntEn));
    memset(m_bADCEn, false, sizeof(m_bADCEn));
    memset(m_TransMode, emBatch, sizeof(m_TransMode));
    //Initalize Valid zone pointer
    m_pCurZone = NULL;
    m_pPreZone = NULL;
    memset(m_DetectZoneNo, MAX_CHAR, sizeof(m_DetectZoneNo));
    m_VldZoneNum = 0;
}//}}}

///*********************************
/// Function     : read
/// Description  : read API
/// Parameters   : 
///                unsigned int offsetAddress : address offset of register to be read
///                unsigned char* data_ptr    : pointer to contain read data from register
///                unsigned int size          : length of data to be read
///                bool debug                 : debug flag (true: debug read, false: normal read)
/// Return value : None
///*********************************
void HWCORE_IRCTG3_Func::read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    //Check null pointer
    if (data_ptr == NULL) {
        return;
    }

    if (!debug) {
        reg_rd(offsetAddress, data_ptr, size);
    }
    else {
        reg_rd_dbg(offsetAddress, data_ptr, size);
    }
}//}}}

///*********************************
/// Function     : write
/// Description  : write API
/// Parameters   : 
///                unsigned int offsetAddress : address offset of register to be written
///                unsigned char* data_ptr    : pointer to data to be written to register
///                unsigned int size          : length of data to be read
///                bool debug                 : debug flag (true: debug read, false: normal read)
/// Return value : None
///*********************************
void HWCORE_IRCTG3_Func::write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    //Check null pointer
    if (data_ptr == NULL) {
        return;
    }

    if (!debug) {
        reg_wr(offsetAddress, data_ptr, size);
    }
    else {
        reg_wr_dbg(offsetAddress, data_ptr, size);
    }
}//}}}

///*********************************
/// Function     : RegisterHandler
/// Description  : Handler for register IF
/// Parameters   : 
///                const std::vector<std::string> &args : vector of arguments
/// Return value : None
///*********************************
void HWCORE_IRCTG3_Func::RegisterHandler(const std::vector<std::string> &args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {//Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    }
    else {
        if (args[1] == "*") {//Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase(cmd.begin() + 1);
                cmd.insert(cmd.begin() + 1, reg_p->name());
                msg_return = Chwcore_irctg3_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        }
        else {//Set handle command to Chwcore_irctg3_regif class
            msg_return = Chwcore_irctg3_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///*********************************
/// Function     : SetRegVal
/// Description  : set value for whole register
/// Parameters   : 
///                std::string RegName : name of register
///                unsigned int RegIdx    : index of register to be set
///                unsigned int val    : value to be set to specified registers
/// Return value : None
///*********************************
void HWCORE_IRCTG3_Func::SetRegVal(std::string RegName, unsigned int RegIdx, unsigned int val)
{//{{{
    int check_exist = mRegisterMap.count(RegName);
    //Register not exists in map
    if (check_exist < 0) {
        return;
    }
    (*(*((mRegisterMap[RegName.c_str()]) + RegIdx))) = val;
}//}}}

///*********************************
/// Function     : SetRegVal
/// Description  : set value for whole register
/// Parameters   : 
///                std::string RegName : name of register
///                unsigned int RegIdx    : index of register to be set
///                std::string BitName    : name of bit
///                unsigned int val    : value to be set to specified registers
/// Return value : None
///*********************************
void HWCORE_IRCTG3_Func::SetRegVal(std::string RegName, unsigned int RegIdx, std::string BitName, unsigned int val)
{//{{{
    int check_exist = mRegisterMap.count(RegName);
    //Register not exists in map
    if (check_exist < 0) {
        return;
    }
    (*(*((mRegisterMap[RegName.c_str()] + RegIdx))))[BitName.c_str()] = val;
}//}}}

///*********************************
/// Function     : GetRegVal
/// Description  : get value of bits of register
/// Parameters   : 
///                std::string RegName : name of register
///                unsigned int RegIdx    : index of register to be set
///                std::string BitName : name of bits of register
/// Return value : 
///                unsigned int ret    : value of specified bits of register
///*********************************
unsigned int HWCORE_IRCTG3_Func::GetRegVal(std::string RegName, unsigned int RegIdx, std::string BitName)
{//{{{
    int check_exist = mRegisterMap.count(RegName);
    unsigned int ret = 0;
    //Register not exists in map
    if (check_exist < 0) {
        return 0;
    }
    ret = (unsigned int)((*(*(mRegisterMap[RegName.c_str()] + RegIdx)))[BitName.c_str()]);
    return ret;
}//}}}

///*********************************
/// Function     : GetRegVal
/// Description  : get value of whole register
/// Parameters   : 
///                std::string RegName : name of register
///                unsigned int RegIdx    : index of register to be set
/// Return value : 
///                unsigned int ret    : value of specified register
///*********************************
unsigned int HWCORE_IRCTG3_Func::GetRegVal(std::string RegName, unsigned int RegIdx)
{//{{{
    int check_exist = mRegisterMap.count(RegName);
    unsigned int ret = 0;
    //Register not exists in map
    if (check_exist < 0) {
        return 0;
    }
    ret = (unsigned int)((*(*(mRegisterMap[RegName.c_str()] + RegIdx))));
    return ret;
}//}}}

///*********************************
/// Function     : SetElectricalAngle
/// Description  : set value for electrical angle
/// Parameters   : 
///                unsigned short val: value to be set to electrical angle
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Direction_059}
void HWCORE_IRCTG3_Func::SetElectricalAngle(unsigned short val)
{//{{{
    val = val & BOUNDARY_MASK;
    for (int i = 0; i < HWCORE_IRCTG3_AgentController::emNumPhase; i++) {
        //Model is in running state
        if (m_State == emRunning) {
            m_InpAng[emBuff_Old][i] = m_InpAng[emBuff_Cur][i];
            //Set flag to determine that model is trigger by changing angle
        }
        m_InpAng[emBuff_Cur][i] = val;
    }
    m_bIsAngChange = true;
    func_re_printf("info", "Input electrical angle is 0x%03X.\n", (unsigned int)val);
}//}}}

//========================================================
//==================== Operation APIs ====================
//========================================================
///*********************************
/// Function     : RegisterBlockOperation
/// Description  : notify update value of registers when rectangle IP is triggered start
/// Parameters   : None
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_010, HWCORE_IRCTG3_UD_Direction_060}
void HWCORE_IRCTG3_Func::RegisterBlockOperation()
{//{{{
    bool bPCT = false;

    //Always immediate reflected mode registers
    m_RefBuff_CMODE = GetRegVal(REG_IR3CMODE, 0);
    m_RefBuff_INTEN = GetRegVal(REG_IR3INTEN, 0);
    m_RefBuff_ADCEN = GetRegVal(REG_IR3ADCEN, 0);
    //Offset registers
    if (m_IsTrigger == true || m_TransMode[emOFS] == emImmediate) {
        m_RefBuff_OFSAL = (unsigned short)GetRegVal(REG_IR3COFSALL, 0);
        m_RefBuff_OFSU = (unsigned short)GetRegVal(REG_IR3COFSU, 0);
        m_RefBuff_OFSV = (unsigned short)GetRegVal(REG_IR3COFSV, 0);
        m_RefBuff_OFSW = (unsigned short)GetRegVal(REG_IR3COFSW, 0);
    }
    //Valid zone register
    if (m_IsTrigger == true || m_TransMode[emCMP] == emImmediate) {
        m_RefBuff_VALN = GetRegVal(REG_IR3VALN, 0);
    }
    bPCT = (bool)((m_RefBuff_CMODE >> BIT_PCT_POS) & 0x1);
    //Prevent updating reference buffers of retention buffers, interrupt and A/D trigger registers when PCT = true in latter running times (EN is already 1)
    if ((bPCT == false) || (m_DetectZoneNo[HWCORE_IRCTG3_AgentController::emU_PHASE] == MAX_CHAR)) {
        //Compare registers
        if (m_IsTrigger == true || m_TransMode[emCMP] == emImmediate) {
            memcpy(m_RefBuff_CMP, m_RetBuff, sizeof(m_RetBuff));
        }
        //Interrupt registers
        if (m_IsTrigger == true || m_TransMode[emINT] == emImmediate) {
            for (int i = 0; i < HWCORE_IRCTG3_AgentController::emNumPhase; i++) {
                m_RefBuff_INT[i] = GetRegVal(REG_IR3INT, i);
            }
        }
        //A/D trigger registers
        if (m_IsTrigger == true || m_TransMode[emADC] == emImmediate) {
            for (int i = 0; i < HWCORE_IRCTG3_AgentController::emNumPhase; i++) {
                m_RefBuff_ADC[i] = GetRegVal(REG_IR3ADC, i);
            }
        }
    }
    else {
        func_re_printf("info", "Cannot update reference buffers of retention buffers, IR3INTk, IR3ADCk (k=0~2) when IR3CMODE.PCT is 1 and IR3CCTR.EN is 1.\n");
    }

    //Clear trigger flag
    m_IsTrigger = false;
    SetRegVal(REG_IR3TRG, 0, m_IsTrigger);
    //Update member variable for easy use
    m_WaveMode = (unsigned char)(m_RefBuff_CMODE & 0x3);
    m_CompMode = (unsigned char)((m_RefBuff_CMODE >> BIT_CMD_POS) & 0x1);
    m_SetRotDir = GetRegVal(REG_IR3RDI, 0);
    for (int i = 0; i < MAX_ZONE; i++) {
        m_bIntEn[i] = (bool)((m_RefBuff_INT[i / MAX_ZONE_PER_PHASE] >> (i % MAX_ZONE_PER_PHASE)) & 0x1);
        m_bADCEn[i] = (bool)((m_RefBuff_ADC[i / MAX_ZONE_PER_PHASE] >> (i % MAX_ZONE_PER_PHASE)) & 0x1);
    }
}//}}}

///*********************************
/// Function     : ZoneSeachAndInterruptOperation
/// Description  : Zone Seach And Interrupt Operation
/// Parameters   : unsigned char cPhase
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_011, HWCORE_IRCTG3_UD_Direction_054}
void HWCORE_IRCTG3_Func::ZoneSeachAndInterruptOperation(unsigned char cPhase) {
    ZoneList* pZoneList = NULL;
    bool bCheck = false;

    //Update phase
    m_Phase = cPhase;

    // Offset addition
    OffsetAddition();

    // Zone segmentation
    pZoneList = ZoneSegmentation();

    // Zone determination
    bCheck = ZoneDetermination(pZoneList);

    // Zone determined OK and not first time running, transition direction is determined
    if (bCheck == true && m_DetectZoneNo[m_Phase] != MAX_CHAR) {
        bCheck = ZoneTransDetect();
    }

    // Zone determine OK or zone transition detection OK
    if (bCheck == true) {
        func_re_printf("info", "[%s] Zone transition detected.\n", GetPhaseNameString().c_str());
        UpdateDetectZoneInfo(pZoneList);

        OutputGeneration();
        //Store new position
        m_InpAng[emBuff_Old][m_Phase] = m_InpAng[emBuff_Cur][m_Phase];
        m_OffAng[emBuff_Old][m_Phase] = m_OffAng[emBuff_Cur][m_Phase];
    }

    UpdateSettingZoneRegister(pZoneList);
    //Initialize variable for next time running
    m_pCurZone = NULL;
    m_pPreZone = NULL;
    ReleaseZoneListMemory(pZoneList);
    pZoneList = NULL;
}

///*********************************
/// Function     : DumpStatInfo
/// Description  : Dump statistical information
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_IRCTG3_Func::DumpStatInfo() {
    func_re_printf("info", "Reference buffer:\n");
    func_re_printf("info", "    IR3CMODE   : 0x%08X.\n", m_RefBuff_CMODE);
    func_re_printf("info", "    IR3INTEN   : 0x%08X.\n", m_RefBuff_INTEN);
    func_re_printf("info", "    IR3ADCEN   : 0x%08X.\n", m_RefBuff_ADCEN);
    func_re_printf("info", "    IR3INT0    : 0x%08X.\n", m_RefBuff_INT[0]);
    func_re_printf("info", "    IR3INT1    : 0x%08X.\n", m_RefBuff_INT[1]);
    func_re_printf("info", "    IR3INT2    : 0x%08X.\n", m_RefBuff_INT[2]);
    func_re_printf("info", "    IR3ADC0    : 0x%08X.\n", m_RefBuff_ADC[0]);
    func_re_printf("info", "    IR3ADC1    : 0x%08X.\n", m_RefBuff_ADC[1]);
    func_re_printf("info", "    IR3ADC2    : 0x%08X.\n", m_RefBuff_ADC[2]);
    func_re_printf("info", "    IR3COFSALL : 0x%08X.\n", m_RefBuff_OFSAL);
    func_re_printf("info", "    IR3COFSU   : 0x%08X.\n", m_RefBuff_OFSU);
    func_re_printf("info", "    IR3COFSV   : 0x%08X.\n", m_RefBuff_OFSV);
    func_re_printf("info", "    IR3COFSW   : 0x%08X.\n", m_RefBuff_OFSW);
    func_re_printf("info", "    IR3VALN    : 0x%08X.\n", m_RefBuff_VALN);
    for (int i = 0; i < MAX_ZONE_PER_PHASE; i++) {
        func_re_printf("info", "    Retention Buffer %d  : 0x%08X.\n", i, m_RefBuff_CMP[i]);
    }
}

///*********************************
/// Function     : OffsetAddition
/// Description  : Offset Addition
/// Parameters   : None
/// Return value : ZoneList* pZoneList
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_012}
void HWCORE_IRCTG3_Func::OffsetAddition() {
    unsigned short sOffAll = 0;
    unsigned short sOffPhase = 0;
    unsigned short nTotalOff = 0;

    //No proceed if both angle and offset are not changed
    if (m_bIsAngChange == false && m_bIsOfsAngChange == false) {
        return;
    }

    sOffAll = (unsigned short)GetRegVal(REG_IR3COFSALL, 0);

    switch (m_Phase)
    {
    case HWCORE_IRCTG3_AgentController::emU_PHASE:
        sOffPhase = (unsigned short)GetRegVal(REG_IR3COFSU, 0);
        break;
    case HWCORE_IRCTG3_AgentController::emV_PHASE:
        sOffPhase = (unsigned short)GetRegVal(REG_IR3COFSV, 0);
        break;
    case HWCORE_IRCTG3_AgentController::emW_PHASE:
        sOffPhase = (unsigned short)GetRegVal(REG_IR3COFSW, 0);
        break;
    default:
        break;
    }

    nTotalOff = (unsigned short)((sOffAll + sOffPhase) & BOUNDARY_MASK);
    //Re-calculate previous after-offset angle with new offset value if input angle is changed
    if (m_bIsAngChange == true) {
        m_OffAng[emBuff_Old][m_Phase] = (m_InpAng[emBuff_Old][m_Phase] + nTotalOff) & 0xFFF;
    }
    //Only backup previous after-offset angle if input angle is not changed, offset is changed
    else {
        m_OffAng[emBuff_Old][m_Phase] = m_OffAng[emBuff_Cur][m_Phase];
    }

    m_OffAng[emBuff_Cur][m_Phase] = (unsigned short)((m_InpAng[emBuff_Cur][m_Phase] + nTotalOff) & BOUNDARY_MASK);
}

///*********************************
/// Function     : ZoneSegmentation
/// Description  : Zone Segmentation
/// Parameters   : unsigned char cPhase
/// Return value : ZoneList* pZoneList: list of zones
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_013}
ZoneList* HWCORE_IRCTG3_Func::ZoneSegmentation() {
    ZoneList* pZoneList = NULL;

    if (m_CompMode == HWCORE_IRCTG3_AgentController::emCommon) {
        m_VldZoneNum = (m_RefBuff_VALN >> (8 * HWCORE_IRCTG3_AgentController::emU_PHASE)) & m_VldZoneMask[HWCORE_IRCTG3_AgentController::emU_PHASE];
        pZoneList = ZoneSegmentation(HWCORE_IRCTG3_AgentController::emU_PHASE);
    }
    else {
        m_VldZoneNum = (m_RefBuff_VALN >> (8 * m_Phase)) & m_VldZoneMask[m_Phase];
        pZoneList = ZoneSegmentation(m_Phase);
    }

    return pZoneList;
}

///*********************************
/// Function     : ZoneSegmentation
/// Description  : Zone Segmentation
/// Parameters   : unsigned char cPhase
/// Return value : ZoneList* pZoneList: list of zones
///*********************************

ZoneList* HWCORE_IRCTG3_Func::ZoneSegmentation(unsigned char cPhase) {
    ZoneList* pHead = NULL;
    ZoneList* pZone = NULL;
    unsigned short nLow = 0;
    unsigned short nHigh = 0;
    bool bInt = false;
    bool bADC = false;
    bool bPtn = false;
    unsigned char cIdx = 0;

    if (ValidateValidZoneNum(cPhase) == false) {
        return NULL;
    }

    for (int nZone = 0; nZone < m_VldZoneNum; nZone++) {
        //Calculate index
        cIdx = cPhase * MAX_ZONE_PER_PHASE + nZone;
        //Boundary information
        nHigh = m_RefBuff_CMP[cIdx] & BOUNDARY_MASK;
        //Triangle wave mode and sawtooth wave mode operation is wrapped arround 180 degrees
        if (m_WaveMode == emTriangle) {
            if (nZone == m_VldZoneNum - 1) {
                nHigh = DEGREES_180;
            }
        }

        bPtn = (bool)((m_RefBuff_CMP[cIdx] >> BIT_PTN_POS) & 0x1);
        bInt = (bool)((m_bIntEn[cIdx]) & 0x1);
        bADC = (bool)((m_bADCEn[cIdx]) & 0x1);

        //First zone create
        if (pZone == NULL) {
            pZone = new ZoneList(nZone, pZone, nHigh, nLow, bInt, bADC, bPtn);
            pHead = pZone;
        }
        else {
            pZone->pNxtZone = new ZoneList(nZone, pZone, nHigh, nLow, bInt, bADC, bPtn);
            pZone = pZone->pNxtZone;
        }
        nLow = nHigh;
    }

    if (ZoneSegmentationCheckBoundary(pHead) == true) {
        //Automatically generate zone for triangle wave
        if (m_WaveMode == emTriangle) {
            ZoneSegmentationTriangle_Auto(pHead);
        }
        //Automatically generate zone for SawtoothW wave
        else if (m_WaveMode == emSawtoothW) {
            ZoneSegmentationSawtoothW_Auto(pHead);
        }

        //Attach tail to head to create doubly cycling linked list
        pZone = pHead;
        while (pZone != NULL) {
            if (pZone->pNxtZone == NULL) {
                pHead->Low = pZone->High;
                pZone->pNxtZone = pHead;
                pHead->pPreZone = pZone;
                break;
            }
            pZone = pZone->pNxtZone;
        }
    }
    else {
        //Attach tail to head to create doubly cycling linked list
        pZone = pHead;
        while (pZone != NULL) {
            if (pZone->pNxtZone == NULL) {
                pHead->Low = pZone->High;
                pZone->pNxtZone = pHead;
                pHead->pPreZone = pZone;
                break;
            }
            pZone = pZone->pNxtZone;
        }

        ReleaseZoneListMemory(pHead);
        pHead = NULL;
    }

    return pHead;
}

///*********************************
/// Function     : ZoneSegmentationTriangle_Auto
/// Description  : Automaticlly zone segmentation for triangle wave mode
/// Parameters   : ZoneList* pZoneList: list of zone segmented by user setting
/// Return value : None
///*********************************
void HWCORE_IRCTG3_Func::ZoneSegmentationTriangle_Auto(ZoneList* pZoneList) {
    ZoneList* pCurZone = NULL;
    ZoneList* pAutoZone = NULL;
    unsigned short nLow = 0;
    unsigned short nHigh = 0;
    bool bIsInt = false;
    bool bIsADC = false;
    bool bPtn = false;
    std::vector<ZoneList*> vt_zone;
    unsigned char nZone = 0;

    if (pZoneList == NULL) {
        return;
    }
    pCurZone = pZoneList;
    nZone = m_MaxVldZone[m_CompMode][emSawtooth] - 1;

    //Start at zone 1
    pCurZone = pCurZone->pNxtZone;
    //Attach to current zone list
    //Point to last segmented zone
    while (pCurZone->pNxtZone != NULL) {
        nHigh = BOUNDARY_MASK & (BOUNDARY_MASK - (pCurZone->Low));
        nLow = BOUNDARY_MASK & (BOUNDARY_MASK - (pCurZone->High));

        bIsInt = pCurZone->bInt;
        bIsADC = pCurZone->bADC;
        bPtn = pCurZone->bPtn;
        
        if (pAutoZone == NULL){
            pAutoZone = new ZoneList(nZone, NULL, nHigh, nLow, bIsInt, bIsADC, bPtn);
        }else{
            pAutoZone = new ZoneList(nZone, pAutoZone, nHigh, nLow, bIsInt, bIsADC, bPtn);
        }
        vt_zone.push_back(pAutoZone);
        nZone--;
        pCurZone = pCurZone->pNxtZone;
    }
    //Set high of last setting zone to low to first automatically created zone.
    if (vt_zone.size() != 0) {
        pCurZone->High = vt_zone.back()->Low;
        vt_zone.back()->pPreZone = pCurZone;
        while (vt_zone.size() != 0) {
            pCurZone->pNxtZone = vt_zone.back();
            vt_zone.back()->pPreZone = pCurZone;
            pCurZone = pCurZone->pNxtZone;
            vt_zone.pop_back();
        }
    }
}

///*********************************
/// Function     : ZoneSegmentationSawtoothW_Auto
/// Description  : Automaticlly zone segmentation for sawtooth w wave mode
/// Parameters   : ZoneList* pZoneList: list of zone segmented by user setting
/// Return value : None
///*********************************
void HWCORE_IRCTG3_Func::ZoneSegmentationSawtoothW_Auto(ZoneList* pZoneList) {
    ZoneList* pCurZone = NULL;
    ZoneList* pCurZoneTail = NULL;
    ZoneList* pAutoZone = NULL;
    ZoneList* pAutoZoneHead = NULL;
    unsigned short nLow = 0;
    unsigned short nHigh = 0;
    bool bIsInt = false;
    bool bIsADC = false;
    bool bPtn = false;
    unsigned char nZone = 0;

    if (pZoneList == NULL) {
        return;
    }

    pCurZone = pZoneList;
    nZone = m_MaxVldZone[m_CompMode][emSawtoothW];

    while (pCurZone != NULL) {
        nHigh = (pCurZone->High + DEGREES_180) & BOUNDARY_MASK;
        nLow = (pCurZone->Low + DEGREES_180) & BOUNDARY_MASK;

        bIsInt = pCurZone->bInt;
        bIsADC = pCurZone->bADC;
        bPtn = pCurZone->bPtn;

        if (pAutoZone == NULL) {
            pAutoZone = new ZoneList(nZone, pAutoZone, nHigh, nLow, bIsInt, bIsADC, bPtn);
            pAutoZoneHead = pAutoZone;
        }
        else {
            pAutoZone->pNxtZone = new ZoneList(nZone, pAutoZone, nHigh, nLow, bIsInt, bIsADC, bPtn);
            pAutoZone = pAutoZone->pNxtZone;
        }

        nZone++;
        if (pCurZone->pNxtZone == NULL) {
            pCurZoneTail = pCurZone;
        }
        pCurZone = pCurZone->pNxtZone;
    }

    //Attach to current zone list
    if (pAutoZoneHead != NULL && pCurZoneTail != NULL) {
        pAutoZoneHead->Low = pCurZoneTail->High;
        pAutoZoneHead->pPreZone = pCurZoneTail;
        pCurZoneTail->pNxtZone = pAutoZoneHead;
    }
}

///*********************************
/// Function     : ZoneSegmentationCheckBoundary
/// Description  : Zone segmentation boundary check
/// Parameters   : ZoneList*& pZoneList: reference to list of zones
/// Return value : bool check result
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Direction_051}
bool HWCORE_IRCTG3_Func::ZoneSegmentationCheckBoundary(ZoneList* pZoneList) {
    ZoneList* pCurZone = NULL;
    unsigned short High = 0;
    unsigned short Low = 0;
    unsigned char nCurNo = 0;

    if (pZoneList == NULL) {
        return false;
    }

    //Start checking from zone 1.
    pCurZone = pZoneList->pNxtZone;
    if (pCurZone == NULL){
        return false;
    }
    
    while (pCurZone != NULL) {
            High = pCurZone->High;
            Low = pCurZone->Low;
            nCurNo = pCurZone->No;

            if (High <= Low) {
                func_re_printf("info", "[%s] The operation is stopped because value of boundary %u (0x%08X) is not greater than value of boundary %u (0x%08X).\n", GetPhaseNameString().c_str(), nCurNo, High, pCurZone->pPreZone->No, Low);
                mpIRCTG3_AgentController->UpdateInterruptValue(HWCORE_IRCTG3_AgentController::em_ir3_int_err_r, true);
                return false;
            }
            pCurZone = pCurZone->pNxtZone;
    }

    return true;
}

///*********************************
/// Function     : ZoneDetermination
/// Description  : Zone determination
/// Parameters   : ZoneList* pZoneList: list of zones
/// Return value : bool true: determine OK
///                     false: determine not OK
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_014, HWCORE_IRCTG3_UD_Direction_052}
bool HWCORE_IRCTG3_Func::ZoneDetermination(ZoneList* pZoneList) {
    ZoneList* pZone = NULL;
    ZoneList* pDetectZone = NULL;
    bool bPCT = false;
    unsigned short nCurOffAng = m_OffAng[emBuff_Cur][m_Phase];
    unsigned char cDetectZoneNo = 0;

    if (pZoneList == NULL) {
        return false;
    }
    pZone = pZoneList;
    bPCT = (bool)((m_RefBuff_CMODE >> BIT_PCT_POS) & 0x1);
    cDetectZoneNo = m_DetectZoneNo[m_Phase];

    while (pDetectZone == NULL) {
        //Positive rotation
        if (m_SetRotDir == emPositive) {
            if ((pZone->No == 0) && ((pZone->Low <= nCurOffAng) || (nCurOffAng < pZone->High))) {
                pDetectZone = pZone;
            }else if ((pZone->Low <= nCurOffAng) && (nCurOffAng < pZone->High)) {
                pDetectZone = pZone;
            }
        }
        //Negative rotation
        else {
            if ((pZone->No == 0) && ((pZone->Low < nCurOffAng) || (nCurOffAng <= pZone->High))) {
                pDetectZone = pZone;
            } else if ((pZone->Low < nCurOffAng) && (nCurOffAng <= pZone->High)) {
                pDetectZone = pZone;
            }
        }
        //Point to next zone
        pZone = pZone->pNxtZone;
    }
    //Not first time running
    if (cDetectZoneNo != MAX_CHAR) {
        //Check whether zone transition occurs
        if (pDetectZone->No == cDetectZoneNo) {
            func_re_printf("info", "[%s] No zone transition detected.\n", GetPhaseNameString().c_str());
            //When disable reverse transition detection
            if (bPCT == true) {
                //Only get the maximum value of previous angle
                if (m_SetRotDir == emPositive) {
                    //Revert when current angle is less than previous angle
                    if (nCurOffAng < m_OffAng[emBuff_Old][m_Phase]) {
                        m_InpAng[emBuff_Cur][m_Phase] = m_InpAng[emBuff_Old][m_Phase];
                        m_OffAng[emBuff_Cur][m_Phase] = m_OffAng[emBuff_Old][m_Phase];
                    }
                }
                //Only get the minimum value of previous angle
                else {
                    //Revert when current angle is larger than previous angle
                    if (nCurOffAng > m_OffAng[emBuff_Old][m_Phase]) {
                        m_InpAng[emBuff_Cur][m_Phase] = m_InpAng[emBuff_Old][m_Phase];
                        m_OffAng[emBuff_Cur][m_Phase] = m_OffAng[emBuff_Old][m_Phase];
                    }
                }
            }
            return false;
        }
        else {
            //Store detected zone
            m_pCurZone = pDetectZone;
        }
    }
    //First time running, always detected as zone transition
    else {
        //Store detected zone
        m_pCurZone = pDetectZone;
    }

    return true;
}

///*********************************
/// Function     : ZoneTransDetect
/// Description  : Zone transition detection
/// Parameters   : None
/// Return value : bool true: transition detected
///                     false: transition not detected
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_015, HWCORE_IRCTG3_UD_Direction_053}
bool HWCORE_IRCTG3_Func::ZoneTransDetect() {
    bool bPCT = false;
    unsigned short nPreOffAng = m_OffAng[emBuff_Old][m_Phase];
    unsigned short nCurOffAng = m_OffAng[emBuff_Cur][m_Phase];

    bPCT = (bool)((m_RefBuff_CMODE >> BIT_PCT_POS) & 0x1);
    m_ActualRotDir = emPositive;
    //Disable reversed transition
    //0 <= Angle < 180 degrees
    if (nPreOffAng < DEGREES_180) {
        if ((nCurOffAng < nPreOffAng) || (nCurOffAng > nPreOffAng + DEGREES_180)) {
            m_ActualRotDir = emNegative;
        }
        else if (nCurOffAng == nPreOffAng + DEGREES_180) {
            m_ActualRotDir = m_SetRotDir;
        }
    }
    //180 <= Angle < 360 degrees
    else {
        if ((nCurOffAng < nPreOffAng) && (nCurOffAng > nPreOffAng - DEGREES_180)) {
            m_ActualRotDir = emNegative;
        }
        else if (nCurOffAng == nPreOffAng - DEGREES_180) {
            m_ActualRotDir = m_SetRotDir;
        }
    }

    if (bPCT == true) {
        //In case actual different from setting direction
        if (m_ActualRotDir != m_SetRotDir) {
            //Reset to previous angle if reverse zone transition is not detected.
            m_InpAng[emBuff_Cur][m_Phase] = m_InpAng[emBuff_Old][m_Phase];
            m_OffAng[emBuff_Cur][m_Phase] = m_OffAng[emBuff_Old][m_Phase];
            func_re_printf("info", "[%s] Reverse transition detection is off and zone is inversely transitted, no zone transition detected.\n", GetPhaseNameString().c_str());
            return false;
        }
    }

    return true;
}

///*********************************
/// Function     : OutputGeneration
/// Description  : Generate Output
/// Parameters   : None
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_016}
void HWCORE_IRCTG3_Func::OutputGeneration() {
    bool bPtn = false;
    bool bInt = false;
    bool bAdc = false;
    unsigned char cIntZoneNo = 0;
    unsigned char cADCZoneNo = 0;
    bool bIsRvtPattern = false;

    if (m_pCurZone == NULL) {
        return;
    }

    if (m_WaveMode != emSawtooth) {
        bIsRvtPattern = (bool)((GetRegVal(REG_IR3CMODE, 0) >> BIT_RVS_POS) & 0x1);
        if (m_OffAng[emBuff_Cur][m_Phase] < DEGREES_180) {
            bIsRvtPattern = false;
        }
    }

    bPtn = m_pCurZone->bPtn ^ (bIsRvtPattern);
    bInt = InterruptGeneration(cIntZoneNo);
    bAdc = ADTriggerGeneration(cADCZoneNo);

    mpIRCTG3_AgentController->UpdateOutputValue(HWCORE_IRCTG3_AgentController::em_ir3_pat_u_r + m_Phase, bPtn);
    mpIRCTG3_AgentController->UpdateInterruptValue(HWCORE_IRCTG3_AgentController::em_ir3_calc_u_r + m_Phase, true);
    if (bInt == true) {
        mpIRCTG3_AgentController->UpdateInterruptValue(HWCORE_IRCTG3_AgentController::em_ir3_int_u_r + m_Phase, bInt);
        SetINTStatus(bInt, cIntZoneNo);
    }
    if (bAdc == true) {
        mpIRCTG3_AgentController->UpdateInterruptValue(HWCORE_IRCTG3_AgentController::em_ir3_adc_u_r + m_Phase, bAdc);
        SetADCStatus(bAdc, cADCZoneNo);
    }
}

//========================================================
//============ Support function  ========================
//========================================================
///*********************************
/// Function     : ReleaseZoneListMemory
/// Description  : release zone list memory
/// Parameters   : ZoneList* pZoneList
/// Return value : None
///*********************************
void HWCORE_IRCTG3_Func::ReleaseZoneListMemory(ZoneList* pZoneList) {
    //Delete dynamically allocated variables
    while (pZoneList != NULL) {
        ZoneList* pNext = pZoneList->pNxtZone;
        ZoneList* pPrev = pZoneList->pPreZone;
        delete pZoneList;
        pZoneList = NULL;
        if (pNext != NULL) {
            pNext->pPreZone = NULL;
        }
        if (pPrev != NULL) {
            pPrev->pNxtZone = NULL;
        }
        pZoneList = pNext;
    }
}

///*********************************
/// Function     : ValidateValidZoneNum
/// Description  : validate zone number
/// Parameters   : unsigned char
/// Return value : bool check result
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Direction_050, HWCORE_IRCTG3_UD_Direction_049, HWCORE_IRCTG3_UD_Direction_061}
bool HWCORE_IRCTG3_Func::ValidateValidZoneNum(unsigned char cPhase) {
    unsigned char cStartIdx = 0;
    bool bCheck = true;

    //Valid zone number less than 2
    if (m_VldZoneNum < 2) {
        func_re_printf("info", "[%s] The pattern is fixed to 0 because number of valid zone is %u, less than 2.\n", GetPhaseNameString().c_str(), m_VldZoneNum);
        mpIRCTG3_AgentController->UpdateOutputValue(HWCORE_IRCTG3_AgentController::em_ir3_pat_u_r + m_Phase, false);
        return false;
    }
    //Valid zone number exceed limitation
    else if (m_VldZoneNum > m_MaxVldZone[m_CompMode][m_WaveMode]) {
        func_re_printf("info", "[%s] The operation is stopped because number of valid zone is %u, exceeds upper limitation.\n", GetPhaseNameString().c_str(), m_VldZoneNum);
        return false;
    }

    //Independent
    if (m_MaxVldZone[m_CompMode][m_WaveMode] <= MAX_ZONE_PER_PHASE) {
        cStartIdx = cPhase * MAX_ZONE_PER_PHASE;
    }//For common mode, start at 0

    //For triangle and sawtooth w modes, only use settings of zone less than 180 degrees
    if (m_WaveMode != emSawtooth) {
        //Modify boudary settings, clear all zone boundary setting of invalid zone
        for (int i = cStartIdx + m_VldZoneNum - 1; i >= 0; i--) {
            //Valid zones
            if ((m_RefBuff_CMP[i] & BOUNDARY_MASK) >= DEGREES_180) {
                func_re_printf("info", "[%s] The operation is stopped because current mode is not Sawtooth and at least 1 valid zone has boundary >= 180 degrees.\n", GetPhaseNameString().c_str());
                return false;
            }
            //Break at first < 180 degrees zone
            else {
                break;
            }
        }
    }

    //For triangle wave mode, 1 more zone is allowed to acquire pattern, interrupt, A/D trigger settings
    if (m_WaveMode == emTriangle) {
        m_VldZoneNum++;
    }

    return bCheck;
}

///*********************************
/// Function     : InterruptGeneration
/// Description  : Generate interrupt
/// Parameters   : unsigned char* pIntZoneNo: number of zone where interrupt occurs
/// Return value : bool interrupt
///*********************************
bool HWCORE_IRCTG3_Func::InterruptGeneration(unsigned char& refIntZoneNo) {
    bool bINT = false;
    bool bLOE = false;
    bool bHOE = false;
    ZoneList* pZone = NULL;
    unsigned short nAngle = 0;
    bool bIsAllow = false;
    unsigned char cIntNo = 0;
    unsigned char cEn = 0;

    if (m_pCurZone == NULL) {
        return false;
    }

    //Reflect value of m_RefBuff_INTEN to local variables
    bLOE = (bool)((m_RefBuff_INTEN >> BIT_LOE_POS) & 0x1);
    bHOE = (bool)((m_RefBuff_INTEN >> BIT_HOE_POS) & 0x1);
    cEn = (unsigned char)(m_RefBuff_INTEN & BIT_EN_MSK);

    nAngle = m_OffAng[emBuff_Cur][m_Phase];
    if (m_pPreZone != NULL) {
        if (m_ActualRotDir == emPositive) {
            //Get next zone of previous zone
            pZone = m_pPreZone->pNxtZone;
        }
        else {
            //Get previous zone of previous zone
            pZone = m_pPreZone->pPreZone;
        }
    }
    else {
        //Run first time, no previous zone
        pZone = m_pCurZone;
    }

    //interrupt is allowed whhen (angle < 180 degrees and LOE is 1) or (angle >= 180 degrees and HOE is 1) in triangle wave mode or sawtooth w wave mode
    bIsAllow = (((nAngle >= DEGREES_180 && bHOE == true) || (nAngle < DEGREES_180 && bLOE == true)) || (m_WaveMode == emSawtooth));
    if (m_CompMode == HWCORE_IRCTG3_AgentController::emCommon && (cEn != m_Phase && cEn != BIT_EN_MSK)) {
        bIsAllow = false;
    }

    while (pZone != NULL) {
        bINT |= pZone->bInt;
        if (bINT == true) {
            cIntNo = pZone->No;
            if (cIntNo >= m_MaxVldZone[m_CompMode][m_WaveMode]) {
                //For triangle and sawtooth W wave mode, when automacally generated zone generates interrupt, corresponding setting zone number is store instead.
                //For sawtooth wave mode, cIntNo is always less than max valid zone number
                if (m_WaveMode == emTriangle) {
                    cIntNo = m_MaxVldZone[m_CompMode][emSawtooth] - cIntNo;
                }
                else {
                    cIntNo = cIntNo - m_MaxVldZone[m_CompMode][emSawtoothW];
                }
            }
        }

        if (pZone->No != m_pCurZone->No) {
            if (m_ActualRotDir == emPositive) {
                //Get next zone of previous zone
                pZone = pZone->pNxtZone;
            }
            else {
                //Get previous zone of previous zone
                pZone = pZone->pPreZone;
            }
        }else{
            break;
        }
    }

    //Set output
    refIntZoneNo = cIntNo;
    bINT &= bIsAllow;

    return bINT;
}

///*********************************
/// Function     : ADTriggerGeneration
/// Description  : Generate A/D trigger
/// Parameters   : unsigned char* pADCZoneNo: number of zone where ad trigger occurs
/// Return value : bool A/D trigger
///*********************************
bool HWCORE_IRCTG3_Func::ADTriggerGeneration(unsigned char& refADCZoneNo) {
    bool bADC = false;
    bool bLOE = false;
    bool bHOE = false;
    ZoneList* pZone = NULL;
    unsigned short nAngle = 0;
    bool bIsAllow = false;
    unsigned char cADCNo = 0;
    unsigned char cEn = 0;

    if (m_pCurZone == NULL) {
        return false;
    }

    //Reflect value of m_RefBuff_INTEN to local variables
    bLOE = (bool)((m_RefBuff_ADCEN >> BIT_LOE_POS) & 0x1);
    bHOE = (bool)((m_RefBuff_ADCEN >> BIT_HOE_POS) & 0x1);
    cEn = (unsigned char)(m_RefBuff_ADCEN & BIT_EN_MSK);

    nAngle = m_OffAng[emBuff_Cur][m_Phase];
    if (m_pPreZone != NULL) {
        if (m_ActualRotDir == emPositive) {
            //Get next zone of previous zone
            pZone = m_pPreZone->pNxtZone;
        }
        else {
            //Get previous zone of previous zone
            pZone = m_pPreZone->pPreZone;
        }
    }
    else {
        //Run first time, no previous zone
        pZone = m_pCurZone;
    }

    //interrupt is allowed whhen (angle < 180 degrees and LOE is 1) or (angle >= 180 degrees and HOE is 1) in triangle wave mode or sawtooth w wave mode
    bIsAllow = (((nAngle >= DEGREES_180 && bHOE == true) || (nAngle < DEGREES_180 && bLOE == true)) || (m_WaveMode == emSawtooth));
    if (m_CompMode == HWCORE_IRCTG3_AgentController::emCommon && (cEn != m_Phase && cEn != BIT_EN_MSK)) {
        bIsAllow = false;
    }

    while (pZone != NULL) {
        bADC |= pZone->bADC;
        if (bADC == true) {
            cADCNo = pZone->No;
            if (cADCNo != 0) {
                //For triangle and sawtooth W wave mode, when automacally generated zone generates interrupt, corresponding setting zone number is store instead.
                if (m_WaveMode == emTriangle) {
                    if (pZone->Low >= DEGREES_180) {
                        cADCNo = m_MaxVldZone[m_CompMode][emSawtooth] - pZone->No;
                    }
                }
                else if (m_WaveMode == emSawtoothW) {
                    if (pZone->High >= DEGREES_180) {
                        cADCNo = pZone->No - m_MaxVldZone[m_CompMode][emSawtoothW];
                    }
                }
            }
        }

        if (pZone->No != m_pCurZone->No) {
            if (m_ActualRotDir == emPositive) {
                //Get next zone of previous zone
                pZone = pZone->pNxtZone;
            }
            else {
                //Get previous zone of previous zone
                pZone = pZone->pPreZone;
            }
        }else{
            break;
        }
    }

    //Set output
    refADCZoneNo = cADCNo;
    bADC &= bIsAllow;

    return bADC;
}

///*********************************
/// Function     : SetINTStatus
/// Description  : set interrupt , AD trigger status
/// Parameters   : unsigned char cZoneNo
/// Return value : None
///*********************************
void HWCORE_IRCTG3_Func::SetINTStatus(bool bInt, unsigned char cZoneNo) {
    std::string RegStoreName = "";
    std::string BitSttName = "";
    bool bCurStt = false;

    if (m_Phase == HWCORE_IRCTG3_AgentController::emU_PHASE) {
        RegStoreName = REG_IR3UINTMEM;
        BitSttName = BIT_UIC;
    }
    else if (m_Phase == HWCORE_IRCTG3_AgentController::emV_PHASE) {
        RegStoreName = REG_IR3VINTMEM;
        BitSttName = BIT_VIC;
    }
    else {
        RegStoreName = REG_IR3WINTMEM;
        BitSttName = BIT_WIC;
    }

    bCurStt = (bool)GetRegVal(REG_IR3INTST, 0, BitSttName);

    if (bCurStt == false) {
        //Store zone number which interrupt is generated
        SetRegVal(RegStoreName, 0, cZoneNo);

        //Set status registers
        SetRegVal(REG_IR3INTST, 0, BitSttName, 1);
    }
}

///*********************************
/// Function     : SetADCStatus
/// Description  : set interrupt , AD trigger status
/// Parameters   : unsigned char cZoneNo
/// Return value : None
///*********************************
void HWCORE_IRCTG3_Func::SetADCStatus(bool bAdc, unsigned char cZoneNo) {
    std::string RegStoreName = "";
    std::string BitSttName = "";
    bool bCurStt = false;

    if (m_Phase == HWCORE_IRCTG3_AgentController::emU_PHASE) {
        RegStoreName = REG_IR3UADCMEM;
        BitSttName = BIT_UIC;
    }
    else if (m_Phase == HWCORE_IRCTG3_AgentController::emV_PHASE) {
        RegStoreName = REG_IR3VADCMEM;
        BitSttName = BIT_VIC;
    }
    else {
        RegStoreName = REG_IR3WADCMEM;
        BitSttName = BIT_WIC;
    }

    bCurStt = (bool)GetRegVal(REG_IR3ADCST, 0, BitSttName);

    if (bCurStt == false) {
        //Store zone number which A/D trigger is generated
        SetRegVal(RegStoreName, 0, cZoneNo);

        //Set status registers
        SetRegVal(REG_IR3ADCST, 0, BitSttName, 1);
    }
}

///*********************************
/// Function     : UpdateSettingZoneRegister
/// Description  : clear retention buffer k, INTk, ADIk of invalid zones
/// Parameters   : ZoneList* pZoneList
/// Return value : None
///*********************************
void HWCORE_IRCTG3_Func::UpdateSettingZoneRegister(ZoneList* pZoneList) {
    unsigned char mod = 0;
    unsigned char div = 0;
    unsigned char cnt = 0;
    unsigned char idx = 0;
    unsigned int Buff_INT[HWCORE_IRCTG3_AgentController::emNumPhase] = { 0 };
    unsigned int Buff_ADC[HWCORE_IRCTG3_AgentController::emNumPhase] = { 0 };
    unsigned short Buff_CMP[MAX_ZONE] = { 0 };
    unsigned int mask_low = MAX_SHORT;
    unsigned int mask_high = (unsigned int)MAX_SHORT << 16;

    if (pZoneList == NULL) {
        return;
    }

    //Only for sawtooth W and triangle wave mode
    if (m_WaveMode == emSawtooth) {
        return;
    }

    if (m_CompMode == HWCORE_IRCTG3_AgentController::emCommon) {
        //For common mode, only proceed updating buffers, registers when phase phase W is done process (end of process)
        if (m_Phase != HWCORE_IRCTG3_AgentController::emW_PHASE) {
            return;
        }
    }

    div = m_Phase;
    while (cnt == 0) {
        if (m_CompMode == HWCORE_IRCTG3_AgentController::emCommon) {
            div = pZoneList->No / MAX_ZONE_PER_PHASE;
        }
        mod = pZoneList->No % MAX_ZONE_PER_PHASE;
        idx = div * MAX_ZONE_PER_PHASE + mod;

        Buff_INT[div] |= (unsigned int)(pZoneList->bInt) << mod;
        Buff_ADC[div] |= (unsigned int)(pZoneList->bADC) << mod;
        //For triangle wave, ignore boundary of last valid zone
        Buff_CMP[idx] = ((unsigned int)(pZoneList->bPtn) << BIT_PTN_POS);
        if (m_VldZoneNum <= m_MaxVldZone[m_CompMode][m_WaveMode]) {
            if (m_WaveMode != emTriangle || pZoneList->No != m_VldZoneNum - 1) {
                Buff_CMP[idx] |= pZoneList->High;
            }
            else {
                if (m_CompMode == HWCORE_IRCTG3_AgentController::emCommon) {
                    Buff_CMP[pZoneList->pNxtZone->No - 1] |= pZoneList->High;
                }
                else {
                    Buff_CMP[div * MAX_ZONE_PER_PHASE + pZoneList->pNxtZone->No - 1] |= pZoneList->High;
                }
            }
        }
        else {
            Buff_CMP[idx] |= pZoneList->High;
        }

        pZoneList = pZoneList->pNxtZone;
        if (pZoneList->No == 0) {
            cnt++;
        }
    }

    //Only updated setting of invalid zones
    if (m_CompMode == HWCORE_IRCTG3_AgentController::emCommon) {
        memcpy(m_RetBuff + (MAX_ZONE / 2), Buff_CMP + (MAX_ZONE / 2), sizeof(unsigned short) * (MAX_ZONE / 2));
        SetRegVal(REG_IR3INT, HWCORE_IRCTG3_AgentController::emW_PHASE, Buff_INT[HWCORE_IRCTG3_AgentController::emW_PHASE]);
        SetRegVal(REG_IR3ADC, HWCORE_IRCTG3_AgentController::emW_PHASE, Buff_ADC[HWCORE_IRCTG3_AgentController::emW_PHASE]);
        SetRegVal(REG_IR3INT, HWCORE_IRCTG3_AgentController::emV_PHASE, (GetRegVal(REG_IR3INT, HWCORE_IRCTG3_AgentController::emV_PHASE) & mask_low) | (Buff_INT[HWCORE_IRCTG3_AgentController::emV_PHASE] & mask_high));
        SetRegVal(REG_IR3ADC, HWCORE_IRCTG3_AgentController::emV_PHASE, (GetRegVal(REG_IR3ADC, HWCORE_IRCTG3_AgentController::emV_PHASE) & mask_low) | (Buff_ADC[HWCORE_IRCTG3_AgentController::emV_PHASE] & mask_high));
    }
    else {
        memcpy(m_RetBuff + div * MAX_ZONE_PER_PHASE + MAX_ZONE_PER_PHASE / 2, Buff_CMP + div * MAX_ZONE_PER_PHASE + MAX_ZONE_PER_PHASE / 2, sizeof(unsigned short) * (MAX_ZONE_PER_PHASE / 2));
        SetRegVal(REG_IR3INT, div, (GetRegVal(REG_IR3INT, div) & mask_low) | (Buff_INT[div] & mask_high));
        SetRegVal(REG_IR3ADC, div, (GetRegVal(REG_IR3ADC, div) & mask_low) | (Buff_ADC[div] & mask_high));
    }
}

///*********************************
/// Function     : UpdateDetectZoneInfo
/// Description  : update detect zone information when zone transition is actually detected
/// Parameters   : ZoneList* zone list
/// Return value : None
///*********************************
void HWCORE_IRCTG3_Func::UpdateDetectZoneInfo(ZoneList* pZoneList) {
    unsigned char pre_zone = 0;
    
    if (pZoneList == NULL){
        return;
    }
    
    pre_zone = m_DetectZoneNo[m_Phase];
    m_DetectZoneNo[m_Phase] = m_pCurZone->No;
    m_pPreZone = NULL;
    ZoneList* pZone = pZoneList;
    if (pre_zone != MAX_CHAR) {
        while (pZone != pZoneList->pPreZone) {
            if (pZone->No == pre_zone) {
                break;
            }
            pZone = pZone->pNxtZone;
        }
        if (pZone->No == pre_zone) {
            m_pPreZone = pZone;
        }
    }
}

///*********************************
/// Function     : GetPhaseNameString
/// Description  : Get string of phase name
/// Parameters   : None
/// Return value : std::string phase name string
///*********************************
std::string HWCORE_IRCTG3_Func::GetPhaseNameString(){
    std::string PhaseName = "";

    switch (m_Phase) {
    case HWCORE_IRCTG3_AgentController::emU_PHASE:
        PhaseName = U_PHASE_NAME;
        break;
    case HWCORE_IRCTG3_AgentController::emV_PHASE:
        PhaseName = V_PHASE_NAME;
        break;
    case HWCORE_IRCTG3_AgentController::emW_PHASE:
        PhaseName = W_PHASE_NAME;
        break;
    default:
        break;
    }

    return PhaseName;
}

///*********************************
/// Function     : ResetFlagAfterComplete
/// Description  : reset falg after complete
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_IRCTG3_Func::ResetFlagAfterComplete() {
    m_bIsAngChange = false;
    m_bIsOfsAngChange = false;
}

//========================================================
//============ Callback function  ========================
//========================================================
///*********************************
/// Function     : cb_IR3CCTR_EN
/// Description  : callback function for bit EN of register IR3CCTR
/// Parameters   : 
///                RegCBstr str : register's callback struct
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_017}
void HWCORE_IRCTG3_Func::cb_IR3CCTR_EN(RegCBstr str)
{//{{{
    //Written value is same as previous value
    if (str.data == str.pre_data) {
        return;
    }

    //Written value is different from previous value
    //Change IRCCTR.EN from 0 to 1
    if (str.data == 1) {
        m_State = emRunning;
        mpIRCTG3_AgentController->NotifyStartupTriggerGenerated();
    }
    //Change IR3CCTR.EN from 1 to 0
    else {
        //Clear operation variables for next time running.
        m_State = emStopped;
        //When EN set from 0 to 1, always detected as zone transition, outputs are
        //Updated base on current zone settings, no need information of previous running time.
        m_bIsAngChange = false;
        m_pCurZone = NULL;
        m_pPreZone = NULL;
        memset(m_DetectZoneNo, MAX_CHAR, sizeof(m_DetectZoneNo));
        for (int i = 0; i < HWCORE_IRCTG3_AgentController::emNumPhase; i++) {
            //Fixed output to low
            mpIRCTG3_AgentController->UpdateOutputValue(HWCORE_IRCTG3_AgentController::em_ir3_pat_u_r + i, 0);
        }
        mpIRCTG3_AgentController->NotifyUpdateOutputPort();
    }
}//}}}

///*********************************
/// Function     : cb_IR3CMODE_MODE
/// Description  : callback function for bit MODE of register IR3CMODE
/// Parameters   : 
///                RegCBstr str : register's callback struct
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_030, HWCORE_IRCTG3_UD_Direction_056}
void HWCORE_IRCTG3_Func::cb_IR3CMODE_MODE(RegCBstr str)
{//{{{
    //Written value is same as previous value
    if (str.data == str.pre_data) {
        return;
    }

    //Write inhibited value
    if ((str.data & BIT_MODE_MSK) == BIT_MODE_MSK) {
        SetRegVal(REG_IR3CMODE, 0, (str.data & (REG_IR3CMODE_MSK - BIT_MODE_MSK)));
    }

    //Write valid value, but TRG is currently 1
    if (m_IsTrigger == true) {
        func_re_printf("info", "Register %s cannot be written when IR3TRG.TRG is 1.\n", REG_IR3CMODE);
        SetRegVal(REG_IR3CMODE, 0, str.pre_data);
        return;
    }

    //Written value is different from previous value
    //Model is not running
    if (m_State == emRunning) {
        //Trigger startup
        mpIRCTG3_AgentController->NotifyStartupTriggerGenerated();
    }
}//}}}

///*********************************
/// Function     : cb_IR3TRSMODE_CMP
/// Description  : callback function for bit CMP of register IR3TRSMODE
/// Parameters   : 
///                RegCBstr str : register's callback struct
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_018}
void HWCORE_IRCTG3_Func::cb_IR3TRSMODE_CMP(RegCBstr str)
{//{{{
    //Written value is same as previous value
    if (str.data == str.pre_data) {
        return;
    }

    //Written value is different from previous value
    for (int i = 0; i < emNumTransModeBit; i++) {
        m_TransMode[i] = (unsigned char)((str.data >> i) & 0x1);
    }
}//}}}

///*********************************
/// Function     : cb_IR3TRG_TRG
/// Description  : callback function for bit TRG of register IR3TRG
/// Parameters   : 
///                RegCBstr str : register's callback struct
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_023}
void HWCORE_IRCTG3_Func::cb_IR3TRG_TRG(RegCBstr str) {
    //Written value is same as previous value
    if (str.data == str.pre_data) {
        return;
    }
    //Written value is different from previous value
    m_IsTrigger = (bool)str.data;
    //Model is running and set IR3TRG.TRG from 0 to 1
    if ((m_State == emRunning) && (m_IsTrigger == true)) {
        for (int i = 0; i < emNumTransModeBit; i++) {
            //Register is in immediate reflected mode
            if (m_TransMode[i] == emImmediate) {
                continue;
            }
            //Atleast 1 register is in batch transfer mode
            //Trigger startup
            mpIRCTG3_AgentController->NotifyStartupTriggerGenerated();
            break;
        }
    }
}

///*********************************
/// Function     : cb_IR3INTCLEAR_UCLR
/// Description  : callback function for bit UCLR of register IR3INTCLEAR
/// Parameters   : 
///                RegCBstr str : register's callback struct
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_019}
void HWCORE_IRCTG3_Func::cb_IR3INTCLEAR_UCLR(RegCBstr str)
{//{{{
    unsigned char cIntStt = 0;
    //Written value is same as previous value
    if (str.data == str.pre_data) {
        return;
    }
    //Written value is different from previous value
    //Get interrupt status
    cIntStt = (unsigned char)GetRegVal(REG_IR3INTST, 0);
    //Update interrupt status flag
    cIntStt = cIntStt & (0x7 ^ str.data);
    //Set interrupt status after clearing
    SetRegVal(REG_IR3INTST, 0, (unsigned int)cIntStt);
    //Clear all clear flags
    SetRegVal(REG_IR3INTCLEAR, 0, 0);
}//}}}

///*********************************
/// Function     : cb_IR3INTEN_EN
/// Description  : callback function for bit EN of register IR3INTEN
/// Parameters   : 
///                RegCBstr str : register's callback struct
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_031}
void HWCORE_IRCTG3_Func::cb_IR3INTEN_EN(RegCBstr str)
{//{{{
    //Written value is same as previous value
    if (str.data == str.pre_data) {
        return;
    }
    //Written value is different from previous value
    if (m_IsTrigger == true) {
        func_re_printf("info", "Register %s cannot be written when IR3TRG.TRG is 1.\n", REG_IR3INTEN);
        SetRegVal(REG_IR3INTEN, 0, str.pre_data);
        return;
    }

    //Model is running
    if (m_State == emRunning) {
        //Trigger startup
        mpIRCTG3_AgentController->NotifyStartupTriggerGenerated();
    }
}//}}}

///*********************************
/// Function     : cb_IR3INT_INT
/// Description  : debug callback function for bit INT of register IR3INT
/// Parameters   : 
///                RegCBstr str : register's callback struct
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_028}
void HWCORE_IRCTG3_Func::cb_IR3INT_INT(RegCBstr str)
{//{{{
    //Written value is same as previous value
    if (str.data == str.pre_data) {
        return;
    }

    //Written value is different from previous value
    if (m_IsTrigger == true) {
        func_re_printf("info", "Register %s%d cannot be written when IR3TRG.TRG is 1.\n", REG_IR3INT, str.channel);
        SetRegVal(REG_IR3INT, str.channel, str.pre_data);
        return;
    }

    //Written value is different from previous value
    //Model is running and Immediate transfer mode flag
    if (m_State == emRunning && m_TransMode[emINT] == emImmediate) {
        //Trigger startup
        mpIRCTG3_AgentController->NotifyStartupTriggerGenerated();
    }
}//}}}

///*********************************
/// Function     : cb_IR3ADCCLEAR_UCLR
/// Description  : callback function for bit UCLR of register IR3ADCCLEAR
/// Parameters   : 
///                RegCBstr str : register's callback struct
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_020}
void HWCORE_IRCTG3_Func::cb_IR3ADCCLEAR_UCLR(RegCBstr str)
{//{{{
    unsigned char cADCTrigStt = 0;
    //Written value is same as previous value
    if (str.data == str.pre_data) {
        return;
    }
    //Written value is different from previous value
    //Get A/D trigger status
    cADCTrigStt = (unsigned char)GetRegVal(REG_IR3ADCST, 0);
    //Update A/D trigger status flag
    cADCTrigStt = cADCTrigStt & (0x7 ^ str.data);
    //Set A/D trigger status flag
    SetRegVal(REG_IR3ADCST, 0, (unsigned int)cADCTrigStt);
    //Clear all clear flags
    SetRegVal(REG_IR3ADCCLEAR, 0, 0);
}//}}}

///*********************************
/// Function     : cb_IR3ADCEN_EN
/// Description  : callback function for bit EN of register IR3ADCEN
/// Parameters   : 
///                RegCBstr str : register's callback struct
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_032}
void HWCORE_IRCTG3_Func::cb_IR3ADCEN_EN(RegCBstr str)
{//{{{
    //Written value is same as previous value
    if (str.data == str.pre_data) {
        return;
    }
    //Written value is different from previous value
    if (m_IsTrigger == true) {
        func_re_printf("info", "Register %s cannot be written when IR3TRG.TRG is 1.\n", REG_IR3ADCEN);
        SetRegVal(REG_IR3ADCEN, 0, str.pre_data);
        return;
    }

    //Model is running
    if (m_State == emRunning) {
        //Trigger startup
        mpIRCTG3_AgentController->NotifyStartupTriggerGenerated();
    }
}//}}}

///*********************************
/// Function     : cb_IR3ADC_ADI
/// Description  : debug callback function for bit ADI of register IR3ADC
/// Parameters   : 
///                RegCBstr str : register's callback struct
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_029}
void HWCORE_IRCTG3_Func::cb_IR3ADC_ADI(RegCBstr str)
{//{{{
    //Written value is same as previous value
    if (str.data == str.pre_data) {
        return;
    }
    //Written value is different from previous value
    if (m_IsTrigger == true) {
        func_re_printf("info", "Register %s%d cannot be written when IR3TRG.TRG is 1.\n", REG_IR3ADC, str.channel);
        SetRegVal(REG_IR3ADC, str.channel, str.pre_data);
        return;
    }

    //Model is running and Immediate transfer mode flag
    if (m_State == emRunning && m_TransMode[emADC] == emImmediate) {
        mpIRCTG3_AgentController->NotifyStartupTriggerGenerated();
    }
}//}}}

///*********************************
/// Function     : cb_IR3COFSALL_DATA
/// Description  : callback function for bit DATA of register IR3COFSALL
/// Parameters   : 
///                RegCBstr str : register's callback struct
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_024}
void HWCORE_IRCTG3_Func::cb_IR3COFSALL_DATA(RegCBstr str)
{//{{{
    //Written value is same as previous value
    if (str.data == str.pre_data) {
        return;
    }
    //Written value is different from previous value
    if (m_IsTrigger == true) {
        func_re_printf("info", "Register %s cannot be written when IR3TRG.TRG is 1.\n", REG_IR3COFSALL);
        SetRegVal(REG_IR3COFSALL, 0, str.pre_data);
        return;
    }

    //Model is running and Immediate transfer mode flag
    m_bIsOfsAngChange = true;
    if (m_State == emRunning && m_TransMode[emOFS] == emImmediate) {
        //Trigger startup
        mpIRCTG3_AgentController->NotifyStartupTriggerGenerated();
    }
}//}}}

///*********************************
/// Function     : cb_IR3COFSU_DATA
/// Description  : callback function for bit DATA of register IR3COFSU
/// Parameters   : 
///                RegCBstr str : register's callback struct
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_025}
void HWCORE_IRCTG3_Func::cb_IR3COFSU_DATA(RegCBstr str)
{//{{{
 //Written value is same as previous value
    if (str.data == str.pre_data) {
        return;
    }
    //Written value is different from previous value
    if (m_IsTrigger == true) {
        func_re_printf("info", "Register %s cannot be written when IR3TRG.TRG is 1.\n", REG_IR3COFSU);
        SetRegVal(REG_IR3COFSU, 0, str.pre_data);
        return;
    }

    //Model is running and Immediate transfer mode flag
    m_bIsOfsAngChange = true;
    if (m_State == emRunning && m_TransMode[emOFS] == emImmediate) {
        //Trigger startup
        mpIRCTG3_AgentController->NotifyStartupTriggerGenerated();
    }
}//}}}

///*********************************
/// Function     : cb_IR3COFSV_DATA
/// Description  : callback function for bit DATA of register IR3COFSV
/// Parameters   : 
///                RegCBstr str : register's callback struct
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_026}
void HWCORE_IRCTG3_Func::cb_IR3COFSV_DATA(RegCBstr str)
{//{{{
    //Written value is same as previous value
    if (str.data == str.pre_data) {
        return;
    }
    //Written value is different from previous value
    if (m_IsTrigger == true) {
        func_re_printf("info", "Register %s cannot be written when IR3TRG.TRG is 1.\n", REG_IR3COFSV);
        SetRegVal(REG_IR3COFSV, 0, str.pre_data);
        return;
    }

    //Model is running Immediate transfer mode flag
    m_bIsOfsAngChange = true;
    if (m_State == emRunning && m_TransMode[emOFS] == emImmediate) {
        //Trigger startup
        mpIRCTG3_AgentController->NotifyStartupTriggerGenerated();
    }
}//}}}

///*********************************
/// Function     : cb_IR3COFSW_DATA
/// Description  : callback function for bit DATA of register IR3COFSW
/// Parameters   : 
///                RegCBstr str : register's callback struct
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_027}
void HWCORE_IRCTG3_Func::cb_IR3COFSW_DATA(RegCBstr str)
{//{{{
    //Written value is same as previous value
    if (str.data == str.pre_data) {
        return;
    }
    //Written value is different from previous value
    if (m_IsTrigger == true) {
        func_re_printf("info", "Register %s cannot be written when IR3TRG.TRG is 1.\n", REG_IR3COFSW);
        SetRegVal(REG_IR3COFSW, 0, str.pre_data);
        return;
    }

    //Model is running and Immediate transfer mode flag
    m_bIsOfsAngChange = true;
    if (m_State == emRunning && m_TransMode[emOFS] == emImmediate) {
        //Trigger startup
        mpIRCTG3_AgentController->NotifyStartupTriggerGenerated();
    }
}//}}}

///*********************************
/// Function     : cb_IR3CMP_CMP
/// Description  : callback function for bit CMP of register IR3CMP
/// Parameters   : 
///                RegCBstr str : register's callback struct
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_021, HWCORE_IRCTG3_UD_Direction_057}
void HWCORE_IRCTG3_Func::cb_IR3CMP_CMP(RegCBstr str)
{//{{{
    unsigned char cAccessIdx = str.channel;
    unsigned char cBuffWrSelect = (unsigned char)GetRegVal(REG_IR3CMPWRMD, 0);
    unsigned char cBuffRdSelect = (unsigned char)GetRegVal(REG_IR3CMPRDMD, 0);
    unsigned char cIdx = 0;

    //Write callback
    if (str.is_wr == true) {
        //Written value is different from previous value
        for (int i = 0; i < HWCORE_IRCTG3_AgentController::emNumPhase; i++) {
            //i'th retention buffer block is selected
            if ((bool)((cBuffWrSelect >> i) & 0x1) == true) {
                cIdx = i * MAX_ZONE_PER_PHASE + cAccessIdx;
                if (m_IsTrigger == true) {
                    func_re_printf("info", "Retention Buffer %d cannot be written when IR3TRG.TRG is 1.\n", (unsigned int)cIdx);
                    return;
                }
                //Update selected retention buffer
                m_RetBuff[cIdx] = (str.data & COMPARE_REG_MASK);
            }
        }
        // Model is running and compare registers are in immediate transfer mode
        if (m_State == emRunning && m_TransMode[emCMP] == emImmediate) {
            mpIRCTG3_AgentController->NotifyStartupTriggerGenerated();
        }
    }
    //Read callback
    else {
        for (int i = 0; i < HWCORE_IRCTG3_AgentController::emNumPhase; i++) {
            if (i == cBuffRdSelect) {
                //Set register with selected retention buffer's value
                SetRegVal(REG_IR3CMP, cAccessIdx, m_RetBuff[i * MAX_ZONE_PER_PHASE + cAccessIdx]);
                break;
            }
        }
    }
}//}}}

///*********************************
/// Function     : cb_IR3VALN_UVL
/// Description  : callback function for bit UVL of register IR3VALN
/// Parameters   : 
///                RegCBstr str : register's callback struct
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_022}
void HWCORE_IRCTG3_Func::cb_IR3VALN_UVL(RegCBstr str)
{//{{{
    //Written value is same as previous value
    if (str.data == str.pre_data) {
        return;
    }
    //Written value is different from previous value
    if (m_IsTrigger == true) {
        func_re_printf("info", "Register %s cannot be written when IR3TRG.TRG is 1.\n", REG_IR3VALN);
        SetRegVal(REG_IR3VALN, 0, str.pre_data);
        return;
    }

    // Model is running and compare register is in immediate transfer mode
    if (m_State == emRunning && m_TransMode[emCMP] == emImmediate) {
        mpIRCTG3_AgentController->NotifyStartupTriggerGenerated();
    }
}//}}}

//// vim600: set foldmethod=marker :
