// ---------------------------------------------------------------------
// $Id: HWCORE_RCTG_Func.cpp,v 1.6 2020/11/03 08:33:20 quanganhtran Exp $
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
// Description: Copyright of HWCORE_RCTG_Func.cpp
// Ref: {HWCORE_RCTG_UD_Copyright_001}
// Description: About the include header files of HWCORE_RCTG_Func.cpp
// Ref: {HWCORE_RCTG_UD_ATT1_002}
#include "HWCORE_RCTG_AgentController.h"
#include "HWCORE_RCTG_Func.h"

#ifndef func_re_printf
#define func_re_printf get_fileline(__FILE__, __LINE__); mpRCTG_AgentController->_re_printf
#endif//func_re_printf

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
///*********************************
/// Function     : HWCORE_RCTG_Func 
/// Description  : Constructor of HWCORE_RCTG_Func class
/// Parameters   : 
///                sc_module_name name: name of model
///                HWCORE_RCTG_AgentController* HWCORE_RCTG_AgentControllerPtr: pointer to interface to communicate with HWCORE_RCTG class
/// Return value : None
///*********************************
HWCORE_RCTG_Func::HWCORE_RCTG_Func(std::string name, HWCORE_RCTG_AgentController* HWCORE_RCTG_AgentControllerPtr) :
    Chwcore_rctg_regif(name, 32)
{//{{{
    assert(HWCORE_RCTG_AgentControllerPtr!= NULL);
    mpRCTG_AgentController = HWCORE_RCTG_AgentControllerPtr;
    mpRCTG_AgentController->CommandInit(name);
    mModuleName = name;

    Chwcore_rctg_regif::set_instance_name(name);
    Initialize();
    
    // Create register map
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("RECCTR"           ,(RECCTR)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PTNN"             ,(PTNN)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PTNAB"            ,(PTNAB)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PTNCD"            ,(PTNCD)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PTNEF"            ,(PTNEF)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("CMP0"             ,(CMP0)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("CMP1"             ,(CMP1)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PHQSFT"           ,(PHQSFT)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PSWSFT"           ,(PSWSFT)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PSW"              ,(PSW)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IPCMP0"           ,(IPCMP0)));
}//}}}

///*********************************
/// Function     : ~HWCORE_RCTG_Func 
/// Description  : Destructor of HWCORE_RCTG_Func class
/// Parameters   : None
/// Return value : None
///*********************************
HWCORE_RCTG_Func::~HWCORE_RCTG_Func()
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
void HWCORE_RCTG_Func::EnableReset(const bool is_active)
{//{{{
    if (is_active) {
        //rst_n is asserted
        if (mpRCTG_AgentController->GetResetStatus("rst_n") == true){
            Initialize();
        }      
    }
    Chwcore_rctg_regif::EnableReset(is_active);
}//}}}

///*********************************
/// Function     : Initialize
/// Description  : Initialize member variables
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_RCTG_Func::Initialize(void)
{//{{{
    //Initialize pattern
    for (int i = 0; i < emNumPhase; i++) {
        mRectPtn[i] = 0;
    }
    if (mBufferMap.size() != 0) {
        mBufferMap.clear();
    }
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
// Ref: {HWCORE_RCTG_UD_Direction_030, HWCORE_RCTG_UD_Flow_004, HWCORE_RCTG_UD_ATT1_014}
void HWCORE_RCTG_Func::read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
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
// Ref: {HWCORE_RCTG_UD_Direction_029, HWCORE_RCTG_UD_Flow_005, HWCORE_RCTG_UD_Registers_012}
void HWCORE_RCTG_Func::write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
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
void HWCORE_RCTG_Func::RegisterHandler(const std::vector<std::string> &args)
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
                msg_return = Chwcore_rctg_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        }
        else {//Set handle command to Chwcore_rctg_regif class
            msg_return = Chwcore_rctg_regif::reg_handle_command(args);
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
///                unsigned int val    : value to be set to specified registers
/// Return value : None
///*********************************
void HWCORE_RCTG_Func::SetRegVal(std::string RegName, unsigned int val)
{//{{{
    int check_exist = mRegisterMap.count(RegName);
    //Register not exists in map
    if (check_exist < 0) {
        return;
    }
    *(mRegisterMap[RegName.c_str()]) = val;
}//}}}

///*********************************
/// Function     : GetRegVal
/// Description  : get value of bits of register
/// Parameters   : 
///                std::string RegName : name of register
///                std::string BitName : name of bits of register
/// Return value : 
///                unsigned int ret    : value of specified bits of register
///*********************************
unsigned int HWCORE_RCTG_Func::GetRegVal(std::string RegName, std::string BitName)
{//{{{
    int check_exist = mRegisterMap.count(RegName);
    unsigned int ret = 0;
    //Register not exists in map
    if (check_exist < 0) {
        return 0;
    }
    ret = (unsigned int)((*(mRegisterMap[RegName.c_str()]))[BitName.c_str()]);
    return ret;
}//}}}

///*********************************
/// Function     : GetRegVal
/// Description  : get value of whole register
/// Parameters   : 
///                std::string RegName : name of register
/// Return value : 
///                unsigned int ret    : value of specified register
///*********************************
unsigned int HWCORE_RCTG_Func::GetRegVal(std::string RegName)
{//{{{
    int check_exist = mRegisterMap.count(RegName);
    unsigned int ret = 0;
    //Register not exists in map
    if (check_exist < 0) {
        return 0;
    }
    ret = (unsigned int)((*(mRegisterMap[RegName.c_str()])));
    return ret;
}//}}}

//========================================================
//==================== Operation APIs ====================
//========================================================
///*********************************
/// Function     : GetRegBufferVal
/// Description  : get value of buffer of whole register
/// Parameters   : 
///                std::string BufferName : name of buffer to be get
/// Return value : 
///                unsigned int ret    : value of specified buffer
///*********************************
unsigned int HWCORE_RCTG_Func::GetRegBufferVal(std::string BufferName)
{//{{{
    int check_exist = mRegisterMap.count(BufferName);
    unsigned int ret = 0;
    //Buffer not exists in map
    if (check_exist < 0) {
        return 0;
    }
    ret = (unsigned int)(mBufferMap[BufferName.c_str()]);
    return ret;
}//}}}

///*********************************
/// Function     : UpdateRegBuffer
/// Description  : notify update value of registers when rectangle IP is triggered start
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_RCTG_Func::UpdateRegBuffer()
{//{{{
    //Clear buffer when it already contains value
    if (mBufferMap.size() != 0) {
        mBufferMap.clear();
    }
    // Store only bits/registers support buffer
    mBufferMap.insert(std::pair<std::string, unsigned int>("RECCTR.FDRCT"     ,GetRegVal("RECCTR","FDRCT")));
    mBufferMap.insert(std::pair<std::string, unsigned int>("RECCTR.FIPPOSI"   ,GetRegVal("RECCTR","FIPPOSI")));
    mBufferMap.insert(std::pair<std::string, unsigned int>("PHQSFT"           ,GetRegVal("PHQSFT")));
    mBufferMap.insert(std::pair<std::string, unsigned int>("PSWSFT"           ,GetRegVal("PSWSFT")));
}//}}}

///*********************************
/// Function     : RectangleIPProcess
/// Description  : rectangle IP's process
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_RCTG_Func::RectangleIPProcess()
{//{{{
    //Local variables
    unsigned short ipcomp0 = 0;
    unsigned char  psw = 0;
    bool Pattern[emNumPhase] = { 0 };

    //Core-process
    RectangleIPCoreProcess(&psw, &ipcomp0, Pattern);

    //Post-process
    //Set registers, member variables
    SetRegVal("PSW", psw);                //Next instruction
    SetRegVal("IPCMP0", ipcomp0);         //Set value for use in comparison 0 of angle generation IP
    mRectPtn[emU_PHASE] = Pattern[emU_PHASE]; //Store pattern value for batch rectangle IP
    mRectPtn[emV_PHASE] = Pattern[emV_PHASE];
    mRectPtn[emW_PHASE] = Pattern[emW_PHASE];

    //Set interrupts, output ports
    mpRCTG_AgentController->NotifyUpdateOutputPorts(HWCORE_RCTG_AgentController::emRDATA_EM2IICMP0, ipcomp0);        //Value for use in comparison 0 of Angle Generation IP
    mpRCTG_AgentController->NotifyAssert_DeassertInterrupt(HWCORE_RCTG_AgentController::emOUTPUT_RCTG_END_SP_REG);   //Completed trigger
}//}}}

///*********************************
/// Function     : BRectangleIPProcess
/// Description  : batch rectangle IP's process
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_RCTG_Func::BRectangleIPProcess()
{//{{{
    //Local variables
    bool Pattern[emNumPhase] = { 0 };

    //Core-process
    BRectangleIPCoreProcess(Pattern);

    //Set output ports
    mpRCTG_AgentController->NotifyUpdateOutputPorts(HWCORE_RCTG_AgentController::emRCTG_U_80M_REG, Pattern[emU_PHASE]);        //Output wave pattern of U-phase
    mpRCTG_AgentController->NotifyUpdateOutputPorts(HWCORE_RCTG_AgentController::emRCTG_V_80M_REG, Pattern[emV_PHASE]);        //Output wave pattern of V-phase
    mpRCTG_AgentController->NotifyUpdateOutputPorts(HWCORE_RCTG_AgentController::emRCTG_W_80M_REG, Pattern[emW_PHASE]);        //Output wave pattern of W-phase
}//}}}

///*********************************
/// Function     : BRectangleIPCoreProcess
/// Description  : batch rectangle IP's core process
/// Parameters   : 
///                bool ptn[PHASE_NUM]: array of rectangle wave value of 3 phases
/// Return value : None
///*********************************
void HWCORE_RCTG_Func::BRectangleIPCoreProcess(bool* ptn)
{//{{{
    bool ptn_u = false;
    bool ptn_v = false;
    bool ptn_w = false;
    bool recctr_setrec = false;

    //Check null pointer
    if (ptn == NULL) {
        return;
    }

    //Inputs
    recctr_setrec = (bool)GetRegVal("RECCTR", "SETREC");

    //Calculations
    //Software processing
    if (recctr_setrec == false) {
        ptn_u = (bool)GetRegVal("PTNN", "UPTN");
        ptn_v = (bool)GetRegVal("PTNN", "VPTN");
        ptn_w = (bool)GetRegVal("PTNN", "WPTN");
    }
    else { //Hardware processing
        ptn_u = (bool)mRectPtn[emU_PHASE];
        ptn_v = (bool)mRectPtn[emV_PHASE];
        ptn_w = (bool)mRectPtn[emW_PHASE];
    }

    //Outputs
    ptn[emU_PHASE] = ptn_u;
    ptn[emV_PHASE] = ptn_v;
    ptn[emW_PHASE] = ptn_w;
}//}}}

///*********************************
/// Function     : RectangleIPCoreProcess
/// Description  : rectangle IP's core process
/// Parameters   : 
///                unsigned char* psw       : switching instruction
///                unsigned short* ipcomp0  : switch angle for use in comparision 0
///                bool ptn[PHASE_NUM]      : array of rectangle wave value of 3 phases
/// Return value : None
///*********************************
void HWCORE_RCTG_Func::RectangleIPCoreProcess(unsigned char* psw, unsigned short* ipcomp0, bool* ptn)
{//{{{
    signed short phqsft = 0;
    unsigned char cur_psw = 0;
    unsigned char next_psw = 0;
    unsigned short switch_angle = 0;
    bool ptn_u = false;
    bool ptn_v = false;
    bool ptn_w = false;
    bool recctr_fdrct = false;
    bool recctr_fipposi = false;

    //Check null pointer
    if (psw == NULL || ipcomp0 == NULL || ptn == NULL) {
        return;
    }

    //Inputs
    //Buffers
    recctr_fdrct = (bool)GetRegBufferVal("RECCTR.FDRCT");     //Rotational direction select bit
    recctr_fipposi = (bool)GetRegBufferVal("RECCTR.FIPPOSI"); //Switching instruction select bit
    cur_psw = (unsigned char)GetRegBufferVal("PSWSFT");       //Current instruction
    phqsft = (signed short)GetRegBufferVal("PHQSFT");         //Reference Voltage Phase Software input value

    if (recctr_fipposi == 0) {
        cur_psw = (unsigned char)GetRegVal("PSW");       //Uses the value produced in the rectangle IP
    }

    if (recctr_fdrct == false){
        switch (cur_psw)
        {
        case 0:
            next_psw = 1;
            switch_angle = 0x2AA - phqsft;
            ptn_u = (bool)GetRegVal("PTNAB", "UPTNA");  // use UPTNA bit
            ptn_v = (bool)GetRegVal("PTNAB", "VPTNA");  // use VPTNA bit
            ptn_w = (bool)GetRegVal("PTNAB", "WPTNA");  // use WPTNA bit
            break;
        case 1:
            next_psw = 2;
            switch_angle = 0x555 - phqsft;
            ptn_u = (bool)GetRegVal("PTNAB", "UPTNB");  // use UPTNB bit
            ptn_v = (bool)GetRegVal("PTNAB", "VPTNB");  // use VPTNB bit
            ptn_w = (bool)GetRegVal("PTNAB", "WPTNB");  // use WPTNB bit
            break;

        case 2:
            next_psw = 3;
            switch_angle = 0x800 - phqsft;
            ptn_u = (bool)GetRegVal("PTNCD", "UPTNC");  // use UPTNC bit
            ptn_v = (bool)GetRegVal("PTNCD", "VPTNC");  // use VPTNC bit
            ptn_w = (bool)GetRegVal("PTNCD", "WPTNC");  // use WPTNC bit
            break;

        case 3:
            next_psw = 4;
            switch_angle = 0xAAA - phqsft;
            ptn_u = (bool)GetRegVal("PTNCD", "UPTND");  // use UPTND bit
            ptn_v = (bool)GetRegVal("PTNCD", "VPTND");  // use VPTND bit
            ptn_w = (bool)GetRegVal("PTNCD", "WPTND");  // use WPTND bit
            break;

        case 4:
            next_psw = 5;
            switch_angle = 0xD55 - phqsft;
            ptn_u = (bool)GetRegVal("PTNEF", "UPTNE");  // use UPTNE bit
            ptn_v = (bool)GetRegVal("PTNEF", "VPTNE");  // use VPTNE bit
            ptn_w = (bool)GetRegVal("PTNEF", "WPTNE");  // use WPTNE bit
            break;

        case 5:
            next_psw = 0;
            switch_angle = 0x000 - phqsft;
            ptn_u = (bool)GetRegVal("PTNEF", "UPTNF");  // use UPTNF bit
            ptn_v = (bool)GetRegVal("PTNEF", "VPTNF");  // use VPTNF bit
            ptn_w = (bool)GetRegVal("PTNEF", "WPTNF");  // use WPTNF bit
            break;

        default:
            break;
        }
    }else{ // Reverse rotation
        switch (cur_psw)
        {
        case 0:
            next_psw = 5;
            switch_angle = 0x800 + phqsft;
            ptn_u = (bool)GetRegVal("PTNEF", "UPTNE");  // use UPTNE bit
            ptn_v = (bool)GetRegVal("PTNEF", "VPTNE");  // use VPTNE bit
            ptn_w = (bool)GetRegVal("PTNEF", "WPTNE");  // use WPTNE bit
            break;
        case 1:
            next_psw = 0;
            switch_angle = 0xAAA + phqsft;
            ptn_u = (bool)GetRegVal("PTNEF", "UPTNF");  // use UPTNF bit
            ptn_v = (bool)GetRegVal("PTNEF", "VPTNF");  // use VPTNF bit
            ptn_w = (bool)GetRegVal("PTNEF", "WPTNF");  // use WPTNF bit
            break;

        case 2:
            next_psw = 1;
            switch_angle = 0xD55 + phqsft;
            ptn_u = (bool)GetRegVal("PTNAB", "UPTNA");  // use UPTNA bit
            ptn_v = (bool)GetRegVal("PTNAB", "VPTNA");  // use VPTNA bit
            ptn_w = (bool)GetRegVal("PTNAB", "WPTNA");  // use WPTNA bit
            break;

        case 3:
            next_psw = 2;
            switch_angle = 0x000 + phqsft;
            ptn_u = (bool)GetRegVal("PTNAB", "UPTNB");  // use UPTNB bit
            ptn_v = (bool)GetRegVal("PTNAB", "VPTNB");  // use VPTNB bit
            ptn_w = (bool)GetRegVal("PTNAB", "WPTNB");  // use WPTNB bit
            break;

        case 4:
            next_psw = 3;
            switch_angle = 0x2AA + phqsft;
            ptn_u = (bool)GetRegVal("PTNCD", "UPTNC");  // use UPTNC bit
            ptn_v = (bool)GetRegVal("PTNCD", "VPTNC");  // use VPTNC bit
            ptn_w = (bool)GetRegVal("PTNCD", "WPTNC");  // use WPTNC bit
            break;

        case 5:
            next_psw = 4;
            switch_angle = 0x555 + phqsft;
            ptn_u = (bool)GetRegVal("PTNCD", "UPTND");  // use UPTND bit
            ptn_v = (bool)GetRegVal("PTNCD", "VPTND");  // use VPTND bit
            ptn_w = (bool)GetRegVal("PTNCD", "WPTND");  // use WPTND bit
            break;

        default:
            break;
        }
    }

    //Outputs
    *ipcomp0 = (unsigned short)(switch_angle & 0xFFF);
    *psw = next_psw;
    ptn[emU_PHASE] = ptn_u;
    ptn[emV_PHASE] = ptn_v;
    ptn[emW_PHASE] = ptn_w;
}//}}}

///*********************************
/// Function     : CalcSignExt
/// Description  : process for padding signed bit
/// Parameters   : 
///                unsigned short Reg_value     : value of register
///                unsigned char signbit_index  : index of signed bit
/// Return value : 
///                unsigned short ret           : value after padding signed bit
///*********************************
unsigned short HWCORE_RCTG_Func::CalcSignExt(unsigned short Reg_value, unsigned char signbit_index)
{//{{{
    unsigned short ret = 0;
    unsigned short value_mask = (1 << signbit_index) - 1;
    unsigned short signedbits_mask = MAX_SHORT - value_mask; //Signed bits (signbit_index -> 15)
    unsigned short regVal = Reg_value & value_mask; //Value bits (0 -> signbit_index - 1)

    //Signed bit is 1
    if (((Reg_value >> signbit_index) & 0x1) == 1) {
        ret = signedbits_mask | regVal;
    }//Signed bit is 0
    else {
        ret = (unsigned short)(~signedbits_mask) & regVal;
    }

    return ret;
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
///*********************************
/// Function     : cb_RECCTR_SLCTCMP0
/// Description  : callback function for bit SLCTCMP0 of register RECCTR
/// Parameters   : 
///                RegCBstr str : register's callback struct
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Flow_008, HWCORE_RCTG_UD_Direction_051}
void HWCORE_RCTG_Func::cb_RECCTR_SLCTCMP0(RegCBstr str)
{//{{{
    bool slctcmp0 = 0;
    
    if(str.pre_data == str.data){
        return;
    }
    //Setting of SLCTCMP0 is 1 and SETREC is 0 simultaneously is prohibited
    if((str.data & 0x3) == 0x1){
        func_re_printf("warning", "Cannot set RECCTR.SLCTCMP0 = 0 and RECCTR.SETREC = 1 simutaneously.\n");
        SetRegVal("RECCTR", (unsigned char)(str.pre_data & 0xFF));
        return;
    }
    slctcmp0 = (bool)((str.data >> 1) & 0x1);
    mpRCTG_AgentController->NotifyUpdateOutputPorts(HWCORE_RCTG_AgentController::emEM2IBRECCTR_SCLTCMP0_REG, slctcmp0);
}//}}}

///*********************************
/// Function     : cb_CMP0_DATA
/// Description  : callback function for bit DATA of register CMP0
/// Parameters   : 
///                RegCBstr str : register's callback struct
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Flow_006}
void HWCORE_RCTG_Func::cb_CMP0_DATA(RegCBstr str)
{//{{{
    // Raise interrupt when CMP0 is written
    mpRCTG_AgentController->NotifyAssert_DeassertInterrupt(HWCORE_RCTG_AgentController::emWC_EM2ICMP0);
    if(str.pre_data != str.data){
        // Update output port when register value is changed
        mpRCTG_AgentController->NotifyUpdateOutputPorts(HWCORE_RCTG_AgentController::emEM2ICMP0, (unsigned short)(str.data & 0xFFF));
    }
}//}}}

///*********************************
/// Function     : cb_CMP1_DATA
/// Description  : callback function for bit DATA of register CMP1
/// Parameters   : 
///                RegCBstr str : register's callback struct
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Flow_007}
void HWCORE_RCTG_Func::cb_CMP1_DATA(RegCBstr str)
{//{{{
    // Raise interrupt when CMP1 is written
    mpRCTG_AgentController->NotifyAssert_DeassertInterrupt(HWCORE_RCTG_AgentController::emWC_EM2ICMP1);
    if (str.pre_data != str.data) {
        // Update output port when register value is changed
        mpRCTG_AgentController->NotifyUpdateOutputPorts(HWCORE_RCTG_AgentController::emEM2ICMP1, (unsigned short)(str.data & 0xFFF));
    }
}//}}}

///*********************************
/// Function     : cb_PHQSFT_DATA
/// Description  : callback function for bit DATA of register PHQSFT
/// Parameters   : 
///                RegCBstr str : register's callback struct
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Flow_009}
void HWCORE_RCTG_Func::cb_PHQSFT_DATA(RegCBstr str)
{//{{{
    // Set value of bits b15->b12 equal to value of bit b11
    str.data = (CalcSignExt((unsigned short)str.data, SIGNBIT_IDX) & 0xFFFF);
    SetRegVal("PHQSFT", (unsigned short)(str.data));
}//}}}

///*********************************
/// Function     : cb_PSWSFT_DATA
/// Description  : callback function for bit DATA of register PSWSFT
/// Parameters   : 
///                RegCBstr str : register's callback struct
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Flow_010}
void HWCORE_RCTG_Func::cb_PSWSFT_DATA(RegCBstr str)
{//{{{
    if (str.data == str.pre_data) {
        return;
    }
    // Written value is less than or equal than 5
    if ((unsigned char)(str.data & 0x7) <= 0x5) {
        return;
    }
    SetRegVal("PSWSFT", (unsigned char)(str.pre_data));
}//}}}
//// vim600: set foldmethod=marker :
