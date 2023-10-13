// ---------------------------------------------------------------------
// $Id: EMU3S_HWCORE_Func.cpp,v 1.8 2020/11/19 06:36:58 huyquocpham Exp $
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
// Description: Copyright of .EMU3S_HWCORE_Func.cpp
// Ref: {EMU3S_HWCORE_UD_Copyright_001}

#include "EMU3S_HWCORE_Func.h"
#include "EMU3S_HWCORE_AgentController.h"
#ifndef func_re_printf
#define func_re_printf mpEMU3S_HWCORE_AgentController->get_fileline(__FILE__, __LINE__); mpEMU3S_HWCORE_AgentController->_re_printf
#endif//func_re_printf
//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
// Description: Constructor of EMU3S_HWCORE_Func class
// Ref: {}
EMU3S_HWCORE_Func::EMU3S_HWCORE_Func(std::string name, EMU3S_HWCORE_AgentController* pEMU3S_HWCORE_AgentController) :
            Cemu3s_hwcore_regif(name, 32)
            ,BusSlaveFuncIf()
{
    if (pEMU3S_HWCORE_AgentController == NULL)
    {
        return;
    }
    mpEMU3S_HWCORE_AgentController = pEMU3S_HWCORE_AgentController;
    mpEMU3S_HWCORE_AgentController->CommandInit(name);
    mModuleName = name;
    Cemu3s_hwcore_regif::set_instance_name(name);
    // Initialize
    Initialize();
    // Description: Common Registr Map
    // Ref: {EMU3S_HWCORE_UD_Registers_001, EMU3S_HWCORE_UD_Registers_002, EMU3S_HWCORE_UD_Registers_003}
    // Ref: {EMU3S_HWCORE_UD_Registers_004, EMU3S_HWCORE_UD_Registers_005, EMU3S_HWCORE_UD_Registers_006}
    // Ref: {EMU3S_HWCORE_UD_Registers_007, EMU3S_HWCORE_UD_Registers_008, EMU3S_HWCORE_UD_Registers_009}
    // Ref: {EMU3S_HWCORE_UD_Registers_010, EMU3S_HWCORE_UD_Registers_011, EMU3S_HWCORE_UD_Registers_012}
    // Ref: {EMU3S_HWCORE_UD_Registers_013, EMU3S_HWCORE_UD_Registers_014, EMU3S_HWCORE_UD_Registers_015}
    // Ref: {EMU3S_HWCORE_UD_Registers_016, EMU3S_HWCORE_UD_Registers_017, EMU3S_HWCORE_UD_Registers_018}
    // Ref: {EMU3S_HWCORE_UD_Registers_019, EMU3S_HWCORE_UD_Registers_020, EMU3S_HWCORE_UD_Registers_021}
    // Ref: {EMU3S_HWCORE_UD_Registers_022, EMU3S_HWCORE_UD_Registers_023, EMU3S_HWCORE_UD_Registers_024}
    // Ref: {EMU3S_HWCORE_UD_Registers_025, EMU3S_HWCORE_UD_Registers_026, EMU3S_HWCORE_UD_Registers_027}
    // Ref: {EMU3S_HWCORE_UD_Registers_028, EMU3S_HWCORE_UD_Registers_029, EMU3S_HWCORE_UD_Registers_030}
    // Ref: {EMU3S_HWCORE_UD_Registers_031, EMU3S_HWCORE_UD_Registers_032, EMU3S_HWCORE_UD_Registers_033}
    // Ref: {EMU3S_HWCORE_UD_Registers_034, EMU3S_HWCORE_UD_Registers_035, EMU3S_HWCORE_UD_Registers_036}
    // Ref: {EMU3S_HWCORE_UD_Registers_037}
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PRT"             ,(PRT)               ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("CTR"             ,(CTR)               ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("REFCTR"          ,(REFCTR)            ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("AHBPRT"          ,(AHBPRT)            ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("AHBPRI"          ,(AHBPRI)            ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("AHBPRI"          ,(APBPRI)            ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IPTRG"           ,(IPTRG)             ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IPSFT"           ,(IPSFT)             ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("TBLADR"          ,(TBLADR)            ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IPTRGCTR"        ,(IPTRGCTR)          ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IPTRGNUM"        ,(IPTRGNUM)          ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IPTRGMON"        ,(IPTRGMON)          ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("FUNCIDLEGRPA0"   ,(FUNCIDLEGRPA0)     ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("FUNCIDLEGRPA1"   ,(FUNCIDLEGRPA1)     ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("FUNCIDLEGRPA2"   ,(FUNCIDLEGRPA2)     ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("FUNCFINGRPA"     ,(FUNCFINGRPA)       ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("FUNCIDLEGRPB"    ,(FUNCIDLEGRPB)      ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("FUNCFINGRPB"     ,(FUNCFINGRPB)       ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("FUNCWAITGRPA"    ,(FUNCWAITGRPA)      ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("FUNCWAITGRPB"    ,(FUNCWAITGRPB)      ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("FUNCSTGRPA"      ,(FUNCSTGRPA)        ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("FUNCSTGRPB"      ,(FUNCSTGRPB)        ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("FSMSTGRPA"       ,(FSMSTGRPA)         ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("FSMSTGRPB"       ,(FSMSTGRPB)         ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("FUNCFLGRPA0"     ,(FUNCFLGRPA0)       ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("FUNCFLGRPA1"     ,(FUNCFLGRPA1)       ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("FUNCFLGRPA2"     ,(FUNCFLGRPA2)       ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("FUNCFLGRPB"      ,(FUNCFLGRPB)        ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("RECMD"           ,(RECMD)             ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ADRDCSL"         ,(ADRDCSL)           ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("EDCCTR"          ,(EDCCTR)            ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("SFERC"           ,(SFERC)             ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("EDCSERR"         ,(EDCSERR)           ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("EDCDERR"         ,(EDCDERR)           ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("EDCSECLR"        ,(EDCSECLR)          ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("EDCDECLR"        ,(EDCDECLR)          ));
  
}

// Description: Destructor of EMU3S_HWCORE_Func class
// Ref: {}
EMU3S_HWCORE_Func::~EMU3S_HWCORE_Func()
{
    mRegisterMap.clear();
}
// Description: Initialize variables
// Ref: {}
void EMU3S_HWCORE_Func::Initialize(void)
{
    mIsPRTWritePermitted = false;
}
// Description: Reset Func and registers
// Ref: {EMU3S_HWCORE_UD_Direction_059}
void EMU3S_HWCORE_Func::EnableReset(const bool is_active)
{
    if (is_active) 
    {
        Initialize();
    }
    Cemu3s_hwcore_regif::EnableReset(is_active);
}
// Description: Software Reset 
// Ref: {EMU3S_HWCORE_UD_Direction_059} 
void EMU3S_HWCORE_Func::EnableSWReset(const bool is_active)
{
    if (is_active) 
    {
      bool mIsPRTWritePermitted_pre = mIsPRTWritePermitted;
      // Initialize variables
      Initialize();
      // Backup registers  
      unsigned int mPRT_pre_val      = (unsigned int) (*PRT);
      unsigned int mCTR_pre_val      = (unsigned int) (*CTR);
      unsigned int mEDCCTR_pre_val   = (unsigned int) (*EDCCTR);
      unsigned int mSFERC_pre_val    = (unsigned int) (*SFERC);
      unsigned int mEDCSERR_pre_val  = (unsigned int) (*EDCSERR);
      unsigned int mEDCDERR_pre_val  = (unsigned int) (*EDCDERR);
      unsigned int mEDCSECLR_pre_val = (unsigned int) (*EDCSECLR);
      unsigned int mEDCDECLR_pre_val = (unsigned int) (*EDCDECLR);
      Cemu3s_hwcore_regif::EnableReset(is_active);
      // Release reset in regif to allow release SW reset by writing to CTR register 
      Cemu3s_hwcore_regif::EnableReset(false);
      mIsPRTWritePermitted = mIsPRTWritePermitted_pre;
      (*PRT)            = mPRT_pre_val;
      (*CTR)            = mCTR_pre_val;
      (*EDCCTR)         = mEDCCTR_pre_val;
      (*SFERC)          = mSFERC_pre_val;
      (*EDCSERR)        = mEDCSERR_pre_val;
      (*EDCDERR)        = mEDCDERR_pre_val;
      (*EDCSECLR)       = mEDCSECLR_pre_val;
      (*EDCDECLR)       = mEDCDECLR_pre_val;
    }
    else
    {
       Cemu3s_hwcore_regif::EnableReset(is_active);
    }
}

// Description: Read API (target socket)
// Ref: {EMU3S_HWCORE_UD_Direction_020, EMU3S_HWCORE_UD_Direction_039, EMU3S_HWCORE_UD_Direction_040}
// Ref: {EMU3S_HWCORE_UD_Direction_047, EMU3S_HWCORE_UD_Direction_048, EMU3S_HWCORE_UD_Direction_049}
// Ref: {EMU3S_HWCORE_UD_Direction_051, EMU3S_HWCORE_UD_Direction_052}
// Ref: {EMU3S_HWCORE_UD_Direction_078, EMU3S_HWCORE_UD_Direction_079}
// Ref: {EMU3S_HWCORE_UD_Flow_010, EMU3S_HWCORE_UD_ATT1_005, EMU3S_HWCORE_UD_Registers_092}
void EMU3S_HWCORE_Func::read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{
    bool isSupported = true;
    
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    // Clear data buffer
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    
    // When clock = 0, or reset state, only debug master can read register
    if ((!mpEMU3S_HWCORE_AgentController->CheckClockPeriod("PCLK")) || (mpEMU3S_HWCORE_AgentController->GetResetStatus("PRESETn"))
       || (mpEMU3S_HWCORE_AgentController->GetResetStatus("EMUST") && (offsetAddress != 0x000) && (offsetAddress != 0x004)))
    {
        if (pExtInfo != NULL) 
        {
            if (!(debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID))) 
            {
                func_re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
                return;
            }
        }
        else {
            func_re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
            return;
        }
    }

    // Decode the read
    if (offsetAddress <= 0x0B9) 
    { // EMU3S Hardware Registers (General/Common)
        if (!debug) {
            reg_rd(offsetAddress, trans.get_data_ptr(), trans.get_data_length());
        }
        else {
            reg_rd_dbg(offsetAddress, trans.get_data_ptr(), trans.get_data_length());
        }
    }
    // Others
    else 
    {
        CRWInterface* sub_ip = mpEMU3S_HWCORE_AgentController->GetInstance(offsetAddress);
        if (sub_ip == NULL) 
        {
            isSupported = false;
        }
        else 
        {
            // EMU3S_HWCORE will decide which sub-IPs will be called
            sub_ip->read(offsetAddress, trans.get_data_ptr(), trans.get_data_length(), debug);
        }
    }

    // Handle unsupported register
    if (!isSupported) 
    {
        func_re_printf("info", "The registers are unsupported\n");
        return;
    }
}

// Description: Write API (target socket)
// Ref: {EMU3S_HWCORE_UD_Direction_020, EMU3S_HWCORE_UD_Direction_039, EMU3S_HWCORE_UD_Direction_040}
// Ref: {EMU3S_HWCORE_UD_Direction_047, EMU3S_HWCORE_UD_Direction_048, EMU3S_HWCORE_UD_Direction_050}
// Ref: {EMU3S_HWCORE_UD_Direction_051, EMU3S_HWCORE_UD_Direction_053, EMU3S_HWCORE_UD_Direction_056}
// Ref: {EMU3S_HWCORE_UD_Direction_058, EMU3S_HWCORE_UD_Direction_078, EMU3S_HWCORE_UD_Direction_079}
// Ref: {EMU3S_HWCORE_UD_Direction_080}
// Ref: {EMU3S_HWCORE_UD_Flow_011, EMU3S_HWCORE_UD_ATT1_005, EMU3S_HWCORE_UD_Registers_092}
void EMU3S_HWCORE_Func::write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{
    bool isSupported = true;
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    // Not need to check reset status because regif class prevent write register in reset state
    // When clock = 0, only debug master write
    if (!(mpEMU3S_HWCORE_AgentController->CheckClockPeriod("PCLK")))
    {
        if (pExtInfo != NULL) 
        {
            if (!(debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID))) 
            {
                func_re_printf("warning", "Cannot access register when clock is 0.\n");
                return;
            }
        }
        else {
            func_re_printf("warning", "Cannot access register when clock is 0.\n");
            return;
        }
    }

    if (mpEMU3S_HWCORE_AgentController->GetResetStatus("EMUST") && (offsetAddress != 0x000) && (offsetAddress != 0x004))
    {
        func_re_printf("warning", "Cannot access register when SW Reset (EMUST bit) is active.\n");
        return;
    }
    else if (mpEMU3S_HWCORE_AgentController->GetResetStatus("PRESETn") && ((offsetAddress == 0x000) || (offsetAddress == 0x004)))
    {
        func_re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
        return;
    }
    
    // Decode the write
    if (offsetAddress <= 0x0B9) 
    { // EMU3S Hardware Registers (General/Common)
        if (!debug) 
        {
            reg_wr(offsetAddress, trans.get_data_ptr(), trans.get_data_length());
        }
        else 
        {
            reg_wr_dbg(offsetAddress, trans.get_data_ptr(), trans.get_data_length());
        }
    }
    else 
    {
        CRWInterface* sub_ip = mpEMU3S_HWCORE_AgentController->GetInstance(offsetAddress);
        if (sub_ip == NULL) 
        {
            isSupported = false;
        }
        else 
        {
            // EMU3S_HWCORE will decide which sub-IPs will be called
            sub_ip->write(offsetAddress, trans.get_data_ptr(), trans.get_data_length(), debug);
        }
    }

    // Handle unsupported register
    if (!isSupported) 
    {
        func_re_printf("info", "The registers are unsupported\n");
        return;
    }
}

// Description: Handler for register IF
// Ref: {EMU3S_HWCORE_UD_Direction_024}
void EMU3S_HWCORE_Func::RegisterHandler(const std::vector<std::string> &args)
{
    std::string msg_return = "";
    if (args.size() == 1) { // Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    }
    else {
        if (args[1] == "*") { // Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase(cmd.begin() + 1);
                cmd.insert(cmd.begin() + 1, reg_p->name());
                msg_return = Cemu3s_hwcore_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        }
        else { // Set handle command to regif class
            msg_return = Cemu3s_hwcore_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
            else {
                /// There is no error occurs
            }
        }
    }
}
// Description: Write value to register from core
// Ref: {} 
void EMU3S_HWCORE_Func::SetRegVal(std::string RegName, unsigned int val)
{
    (*(mRegisterMap[RegName.c_str()])) = val;
}
// Description: Write value to register from core
// Ref: {} 
void EMU3S_HWCORE_Func::SetRegVal(std::string RegName, std::string BitName, unsigned int val)
{
    (*(mRegisterMap[RegName.c_str()]))[BitName.c_str()] = val;
}
// Description: Read value of register from core
// Ref: {}
unsigned int EMU3S_HWCORE_Func::GetRegVal(std::string RegName)
{
    return ((unsigned int)(*(mRegisterMap[RegName.c_str()])));
}
// Description: Read value of register from core
// Ref: {}
unsigned int EMU3S_HWCORE_Func::GetRegVal(std::string RegName, std::string BitName)
{
    return ((unsigned int)((*(mRegisterMap[RegName.c_str()]))[BitName.c_str()]));
}

//========================================================
//============ Callback function  ========================
//========================================================
// Description: PRT register callback
// Ref: {EMU3S_HWCORE_UD_Flow_032} 
void EMU3S_HWCORE_Func::cb_PRT_MEM0PRT(RegCBstr str)
{
    unsigned int PRTCT = (str.data & 0x1);
    if (PRTCT == 0x1)
    {
        mIsPRTWritePermitted = true;
        unsigned int MEM0PRT_pre = ((str.pre_data >> 4) & 0x1);
        unsigned int MEM0PRT_cur = ((str.data >> 4) & 0x1);
        if ((MEM0PRT_pre == 1) && (MEM0PRT_cur == 0))
        {
            mpEMU3S_HWCORE_AgentController->NotifyWriteTBLADR();
        }
    }
    else
    {
        unsigned int reg_val = (str.pre_data & 0xFE);
        *(PRT) = reg_val;
        mIsPRTWritePermitted = false;
    }
}
// Description: CTR register callback
// Ref: {EMU3S_HWCORE_UD_Flow_012}          
void EMU3S_HWCORE_Func::cb_CTR_EMUST(RegCBstr str)
{
    if (!mIsPRTWritePermitted)
    {
        *(CTR) = str.pre_data;
    }
    else
    {
        unsigned char reg_val =  (unsigned char) (str.data & 0xFF);
        unsigned char EMUST = (reg_val & 0x1);
        mpEMU3S_HWCORE_AgentController->NotifyWriteCTR();
        mpEMU3S_HWCORE_AgentController->WriteSWReset(!((bool)EMUST));
    }
}

// Description: REFCTR register callback
// Ref: {EMU3S_HWCORE_UD_Flow_015}          
void EMU3S_HWCORE_Func::cb_REFCTR_FPWMREFPER(RegCBstr str)
{
    mpEMU3S_HWCORE_AgentController->NotifyWriteREFCTR();
}

// Description: AHBPRT register callback
// Ref: {EMU3S_HWCORE_UD_Flow_039}
void EMU3S_HWCORE_Func::cb_AHBPRT_HPRT(RegCBstr str)
{
    mpEMU3S_HWCORE_AgentController->NotifyWriteAHBPRT();
}        
// Description: IPSFT register callback
// Ref: {EMU3S_HWCORE_UD_Flow_013}  
void EMU3S_HWCORE_Func::cb_IPSFT_INIPSFT(RegCBstr str)
{
    unsigned char reg_val = (unsigned char) str.data;
    unsigned char RECIPSFT = ((reg_val >> 3) & 0x01);
    unsigned char PWMIPSFT = ((reg_val >> 2) & 0x01);
    unsigned char PIIPSFT = ((reg_val >> 1) & 0x01);
    unsigned char INIPSFT = reg_val & 0x01;
    
    if ((RECIPSFT == 1) && ((*(IPTRG))["RECIPTRG"] == 0))
    {
        mpEMU3S_HWCORE_AgentController->NotifySWTrigger(emRCTG);
    }
    if ((PWMIPSFT == 1) && ((*(IPTRG))["PWMIPTRG"] == 0))
    {
        mpEMU3S_HWCORE_AgentController->NotifySWTrigger(emPWM);
    }
    if ((PIIPSFT == 1) && ((*(IPTRG))["PIIPTRG"] == 0))
    {
        mpEMU3S_HWCORE_AgentController->NotifySWTrigger(emPI);
    }
    if ((INIPSFT == 1) && ((*(IPTRG))["INIPTRG"] == 0))
    {
        mpEMU3S_HWCORE_AgentController->NotifySWTrigger(emINPUT);
    }
} 
// Description: TBLADR register callback
// Ref: {EMU3S_HWCORE_UD_Flow_029} 
void EMU3S_HWCORE_Func::cb_TBLADR_DATA(RegCBstr str)
{
    // Operation unsupported in phase 1
    if (!mIsPRTWritePermitted)
    {
        *(TBLADR) = str.pre_data;
    }
}    
// Description: IPTRGCTR register callback
// Ref: {EMU3S_HWCORE_UD_Flow_030, EMU3S_HWCORE_UD_Flow_039} 
void EMU3S_HWCORE_Func::cb_IPTRGCTR_TNEN(RegCBstr str)
{
    unsigned char reg_val = (unsigned char) str.data;
    if (reg_val == 0x0)
    {
        *(IPTRGMON) = 0x0;
    }
    mpEMU3S_HWCORE_AgentController->NotifyWriteIPTRGCTR();
}  
// Description: IPTRGNUM register callback
// Ref: {EMU3S_HWCORE_UD_Flow_039, EMU3S_HWCORE_UD_Flow_049} 
void EMU3S_HWCORE_Func::cb_IPTRGNUM_ADTRGNUM(RegCBstr str)
{
    mpEMU3S_HWCORE_AgentController->NotifyWriteIPTRGNUM();
}    
// Description: IPTRGMON register callback
// Ref: {EMU3S_HWCORE_UD_Flow_014} 
void EMU3S_HWCORE_Func::cb_IPTRGMON_ADTRGNUM(RegCBstr str)
{
   unsigned int PWMTRGNUM = mpEMU3S_HWCORE_AgentController->GetTRGNUM(emPWM);
   unsigned int PITRGNUM = mpEMU3S_HWCORE_AgentController->GetTRGNUM(emPI);
   unsigned int INTRGNUM = mpEMU3S_HWCORE_AgentController->GetTRGNUM(emINPUT);  
   unsigned int ADTRGNUM = mpEMU3S_HWCORE_AgentController->GetTRGNUM(emADTRG);
   (*IPTRGMON)["PWMTRGNUM"] = PWMTRGNUM;
   (*IPTRGMON)["PITRGNUM"] = PITRGNUM;
   (*IPTRGMON)["INTRGNUM"] = INTRGNUM;
   (*IPTRGMON)["ADTRGNUM"] = ADTRGNUM;
}
// Description: FUNCIDLEGRPA0 register callback
// Ref: {EMU3S_HWCORE_UD_Flow_017}  
void EMU3S_HWCORE_Func::cb_FUNCIDLEGRPA0_SFTEN(RegCBstr str)
{
    mpEMU3S_HWCORE_AgentController->NotifyWriteFUNCIDLEGRPA0();
}
// Description: FUNCIDLEGRPA1 register callback
// Ref: {EMU3S_HWCORE_UD_Flow_017}
void EMU3S_HWCORE_Func::cb_FUNCIDLEGRPA1_SFTEN(RegCBstr str)
{
    mpEMU3S_HWCORE_AgentController->NotifyWriteFUNCIDLEGRPA1();
}  
// Description: FUNCIDLEGRPA2 register callback
// Ref: {EMU3S_HWCORE_UD_Flow_017}
void EMU3S_HWCORE_Func::cb_FUNCIDLEGRPA2_SFTEN(RegCBstr str)
{
    mpEMU3S_HWCORE_AgentController->NotifyWriteFUNCIDLEGRPA2();
}  
// Description: FUNCIDLEGRPB register callback
// Ref: {EMU3S_HWCORE_UD_Flow_017}
void EMU3S_HWCORE_Func::cb_FUNCIDLEGRPB_IP(RegCBstr str)
{
    mpEMU3S_HWCORE_AgentController->NotifyWriteFUNCIDLEGRPB();
}
// Description: FUNCWAITGRPA register callback
// Ref: {EMU3S_HWCORE_UD_Flow_018} 
void EMU3S_HWCORE_Func::cb_FUNCWAITGRPA_IP(RegCBstr str)
{
    mpEMU3S_HWCORE_AgentController->NotifyWriteFUNCWAITGRPA();
}
// Description: FUNCWAITGRPB register callback
// Ref: {EMU3S_HWCORE_UD_Flow_018}
void EMU3S_HWCORE_Func::cb_FUNCWAITGRPB_IP(RegCBstr str)
{
    mpEMU3S_HWCORE_AgentController->NotifyWriteFUNCWAITGRPB();
}      
// Description: FUNCSTGRPA register callback
// Ref: {EMU3S_HWCORE_UD_Flow_020}
void EMU3S_HWCORE_Func::cb_FUNCSTGRPA_INPUT0(RegCBstr str)
{
    unsigned int reg_val = (str.pre_data & str.data);
    *(FUNCSTGRPA) = reg_val;
}
// Description: FUNCSTGRPB register callback
// Ref: {EMU3S_HWCORE_UD_Flow_020}
void EMU3S_HWCORE_Func::cb_FUNCSTGRPB_ANG0(RegCBstr str)
{
    unsigned int reg_val = ((str.pre_data & str.data) & 0xFFFF);
    *(FUNCSTGRPB) = reg_val;
}
// Description: FUNCFLGRPA0 register callback
// Ref: {EMU3S_HWCORE_UD_Flow_019} 
void EMU3S_HWCORE_Func::cb_FUNCFLGRPA0_INPUT0(RegCBstr str)
{
    mpEMU3S_HWCORE_AgentController->NotifyWriteFUNCFLGRPA0();
}
// Description: FUNCFLGRPA1 register callback
// Ref: {EMU3S_HWCORE_UD_Flow_019}
void EMU3S_HWCORE_Func::cb_FUNCFLGRPA1_PI0(RegCBstr str)
{
    mpEMU3S_HWCORE_AgentController->NotifyWriteFUNCFLGRPA1();
}
// Description: FUNCFLGRPA2 register callback
// Ref: {EMU3S_HWCORE_UD_Flow_019}      
void EMU3S_HWCORE_Func::cb_FUNCFLGRPA2_PWM0(RegCBstr str)
{
    mpEMU3S_HWCORE_AgentController->NotifyWriteFUNCFLGRPA2();
}     
// Description: FUNCFLGRPB register callback
// Ref: {EMU3S_HWCORE_UD_Flow_019} 
void EMU3S_HWCORE_Func::cb_FUNCFLGRPB_ANG0(RegCBstr str)
{
    mpEMU3S_HWCORE_AgentController->NotifyWriteFUNCFLGRPB();
}   
// Description: ADRDCSL register callback
// Ref: {EMU3S_HWCORE_UD_Flow_039} 
void EMU3S_HWCORE_Func::cb_ADRDCSL_ADSEL(RegCBstr str)
{
    mpEMU3S_HWCORE_AgentController->NotifyWriteADSEL();
}   
// Description: EDCCTR register callback
// Ref: {EMU3S_HWCORE_UD_Flow_031} 
void EMU3S_HWCORE_Func::cb_EDCCTR_EDCDIS(RegCBstr str)
{
    if (!mIsPRTWritePermitted)
    {
        *(EDCCTR) = str.pre_data;
    }
}
// Description: SFERC register callback
// Ref: {EMU3S_HWCORE_UD_Flow_028} 
void EMU3S_HWCORE_Func::cb_SFERC_SENE(RegCBstr str)
{
    if (!mIsPRTWritePermitted)
    {
        *(SFERC) = str.pre_data;
    }
    else
    {
        mpEMU3S_HWCORE_AgentController->NotifyWriteSFERC();
    }
}
// Description: EDCSECLR register callback
// Ref: {EMU3S_HWCORE_UD_Feature_022, EMU3S_HWCORE_UD_Feature_023, EMU3S_HWCORE_UD_Flow_033} 
void EMU3S_HWCORE_Func::cb_EDCSECLR_SESCLR(RegCBstr str)
{
    if ((str.data & 0x1) == 0x1)
    {
        (*EDCSERR)["SES"] = 0;
    }
}
// Description: EDCDECLR register callback
// Ref: {EMU3S_HWCORE_UD_Feature_022, EMU3S_HWCORE_UD_Feature_023, EMU3S_HWCORE_UD_Flow_034} 
void EMU3S_HWCORE_Func::cb_EDCDECLR_DESCLR(RegCBstr str)
{
    if ((str.data & 0x1) == 0x1)
    {
        (*EDCDERR)["DES"] = 0;
    }
}
// vim600: set foldmethod=marker :
