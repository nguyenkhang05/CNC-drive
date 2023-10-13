// ---------------------------------------------------------------------
// $Id: HWCORE_IRCTG3.cpp,v 1.9 2020/10/23 10:04:40 khoaanhnguyen Exp $
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
// Description: Copyright of HWCORE_IRCTG3.cpp
// Description: About the include header files of HWCORE_IRCTG3.cpp
// Ref: {HWCORE_IRCTG3_UD_Copyright_001}
#include "re_define.h"
#include "HWCORE_IRCTG3.h"
#include "HWCORE_IRCTG3_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
///*********************************
/// Function     : HWCORE_IRCTG3 
/// Description  : Constructor of HWCORE_IRCTG3 class
/// Parameters   : 
///                sc_module_name name: name of model
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_ATT1_001, HWCORE_IRCTG3_UD_ATT1_011}
HWCORE_IRCTG3::HWCORE_IRCTG3(sc_module_name name)
    : sc_module(name),
    HWCORE_IRCTG3_AgentController(),
    CRWInterface(),
    PCLK("PCLK"),
    PRESETn("PRESETn"),
    mip_prst_n("mip_prst_n"),
    //Input ports
    e_angle("e_angle"),
    //Output bool ports
    ir3_pat_u_r("ir3_pat_u_r"),
    ir3_pat_v_r("ir3_pat_v_r"),
    ir3_pat_w_r("ir3_pat_w_r"),
    //Interrupts
    ir3_int_err_r("ir3_int_err_r"),
    ir3_int_u_r("ir3_int_u_r"),
    ir3_int_v_r("ir3_int_v_r"),
    ir3_int_w_r("ir3_int_w_r"),
    ir3_calc_u_r("ir3_calc_u_r"),
    ir3_calc_v_r("ir3_calc_v_r"),
    ir3_calc_w_r("ir3_calc_w_r"),
    ir3_adc_u_r("ir3_adc_u_r"),
    ir3_adc_v_r("ir3_adc_v_r"),
    ir3_adc_w_r("ir3_adc_w_r")
{//{{{ 
    mpHWCORE_IRCTG3_Func = new HWCORE_IRCTG3_Func((std::string)name, this);
    if (mpHWCORE_IRCTG3_Func == NULL) {
        return;
    }

    // Initialize clock/reset variables and parameters
    mPCLKPeriod =           0;
    mPCLKOrgFreq =          0;
    mPCLKFreq =             0;
    mPCLKFreqUnit =         "Hz";
    mStartClkTime =         0;
    for (int i = 0; i < emNumReset; i++) {
        mResetCurVal[i] = true;
        mResetPreVal[i] = true;
        mIsResetHardActive[i] = false;
        mIsResetCmdReceive[i] = false;
        mIsResetCmdActive[i] = false;
        mResetCmdPeriod[i] = 0;
    }
    mTimeResolutionValue =  1;
    mTimeResolutionUnit =   SC_NS;
    mEnableRegisterMessage = true;
    mDumpInterrupt = true;
    mDumpStatInfo = false;

    // Initialize member variables
    Initialize();

    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    //Create map for interrupt ports
    mMapIntr[em_ir3_int_err_r] = &ir3_int_err_r;
    mMapIntr[em_ir3_int_u_r] = &ir3_int_u_r;
    mMapIntr[em_ir3_int_v_r] = &ir3_int_v_r;
    mMapIntr[em_ir3_int_w_r] = &ir3_int_w_r;
    mMapIntr[em_ir3_calc_u_r] = &ir3_calc_u_r;
    mMapIntr[em_ir3_calc_v_r] = &ir3_calc_v_r;
    mMapIntr[em_ir3_calc_w_r] = &ir3_calc_w_r;
    mMapIntr[em_ir3_adc_u_r] = &ir3_adc_u_r;
    mMapIntr[em_ir3_adc_v_r] = &ir3_adc_v_r;
    mMapIntr[em_ir3_adc_w_r] = &ir3_adc_w_r;

    //Create map for bool output ports
    mMapOut[em_ir3_pat_u_r] = &ir3_pat_u_r;
    mMapOut[em_ir3_pat_v_r] = &ir3_pat_v_r;
    mMapOut[em_ir3_pat_w_r] = &ir3_pat_w_r;

    // Methods for monitoring Clock port
    SC_METHOD(HandlePCLKSignalMethod);
    sensitive << PCLK;

    // Methods for monitoring PRESETn port
    SC_METHOD(HandleResetSignalMethod_PRESETn);
    sensitive << PRESETn;

    // Methods for monitoring mip_prst_n port
    SC_METHOD(HandleResetSignalMethod_mip_prst_n);
    sensitive << mip_prst_n;

    // Methods for handling hard reset events
    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_IRCTG3::HandleResetHardMethod, this, reset_id),
            sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    // Methods for handling command reset events
    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_IRCTG3::HandleResetCmdMethod, this, reset_id),
            sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    // Methods for canceling command reset events
    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_IRCTG3::CancelResetCmdMethod, this, reset_id),
            sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }

    // Methods for monitoring input port
    SC_METHOD(HandleSignalMethod_e_angle);
    sensitive << e_angle;

    // Methods for operation
    SC_METHOD(RegisterBlockOperationMethod);
    dont_initialize();
    sensitive << mRegisterBlockEvent;

    SC_METHOD(ZoneSeachAndInterruptOperationMethod);
    dont_initialize();
    sensitive << mZoneSearchAndIterruptEvent;

    SC_METHOD(UpdateOutputPortMethod);
    dont_initialize();
    sensitive << mUpdateOutputPortEvent;

    SC_METHOD(UpdateInterruptPortMethod);
    dont_initialize();
    sensitive << mUpdateInterruptPortEvent;

    //Variable to reflect port value, initialized 1 time
    mPrevAngle = 0;
    // Initialize output port
    //Output ports
    ir3_int_err_r.initialize(false);
    ir3_int_u_r.initialize(false);
    ir3_int_v_r.initialize(false);
    ir3_int_w_r.initialize(false);
    ir3_calc_u_r.initialize(false);
    ir3_calc_v_r.initialize(false);
    ir3_calc_w_r.initialize(false);
    ir3_adc_u_r.initialize(false);
    ir3_adc_v_r.initialize(false);
    ir3_adc_w_r.initialize(false);

    //Create map for bool output ports
    ir3_pat_u_r.initialize(false);
    ir3_pat_v_r.initialize(false);
    ir3_pat_w_r.initialize(false);
}//}}}

///*********************************
/// Function     : ~HWCORE_IRCTG3 
/// Description  : Destructor of HWCORE_IRCTG3 class
/// Parameters   : None
/// Return value : None
///*********************************
HWCORE_IRCTG3::~HWCORE_IRCTG3()
{//{{{
    //Clear map
    if (mMapIntr.size() != 0) {
        mMapIntr.clear();
    }
    if (mMapOut.size() != 0) {
        mMapOut.clear();
    }
    delete mpHWCORE_IRCTG3_Func;
}//}}}

///*********************************
/// Function     : Initialize
/// Description  : Initialize member variables when reset is activated
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_IRCTG3::Initialize(void)
{//{{{
    // Initialize member variables
    for (int i = 0; i < emOUTPUT_PORTS_NUM; i++) {
        mOutputValue[i] = false;
    }
    for (int i = 0; i < emINTERRUPT_NUM; i++) {
        mInterruptValue[i] = false;
    }
}//}}}

///*********************************
/// Function     : CancelEvents
/// Description  : Cancel operation events when clock is zero or reset is asserted
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_IRCTG3::CancelEvents(void)
{//{{{
    // Operation events are canceled
    mProcessAfterPCLKChangeEvent.cancel();
    mUpdateOutputPortEvent.cancel();
    mUpdateInterruptPortEvent.cancel();
    mRegisterBlockEvent.cancel();
    mZoneSearchAndIterruptEvent.cancel();
}//}}}

//========================================================
//============ Reset and clock  ==========================
//========================================================
// Ref: {HWCORE_IRCTG3_UD_Flow_001}
///*********************************
/// Function     : HandleResetSignalMethod_PRESETn
/// Description  : Monitor PRESETn reset port
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_IRCTG3::HandleResetSignalMethod_PRESETn(void)
{//{{{
    mResetCurVal[emPRESETnId] = PRESETn.read();
    if (mResetCurVal[emPRESETnId] != mResetPreVal[emPRESETnId]) { //mResetPreVal is "true" at initial state
        mResetPreVal[emPRESETnId] = mResetCurVal[emPRESETnId];
        // Reset signals are asynchronous
        mResetHardEvent[emPRESETnId].notify(SC_ZERO_TIME);
    }
}//}}}

///*********************************
/// Function     : HandleResetSignalMethod_mip_prst_n
/// Description  : Monitor mip_prst_n reset port
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_IRCTG3::HandleResetSignalMethod_mip_prst_n(void)
{//{{{
    mResetCurVal[emmip_prst_nId] = mip_prst_n.read();
    mResetPreVal[emmip_prst_nId] = mResetCurVal[emmip_prst_nId];
    // Reset signals are asynchronous
    mResetHardEvent[emmip_prst_nId].notify(SC_ZERO_TIME);
}//}}}

// Ref: {HWCORE_IRCTG3_UD_Flow_002}
///*********************************
/// Function     : HandlePCLKSignalMethod
/// Description  : Monitor PCLK clock port
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_IRCTG3::HandlePCLKSignalMethod(void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///*********************************
/// Function     : HandleResetHardMethod
/// Description  : Process reset function when reset port is active
/// Parameters   : 
///                const unsigned int reset_id: reset id (0: PRESETn reset port | 1: mip_prst_n reset port)
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Direction_045, HWCORE_IRCTG3_UD_Direction_046}
void HWCORE_IRCTG3::HandleResetHardMethod(const unsigned int reset_id)
{//{{{
    if (mResetCurVal[reset_id] == vpcl::emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == emPRESETnId) {
            re_printf("info", "The reset port PRESETn is asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id]);
        }
        else {
            re_printf("info", "The reset port mip_prst_n is asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id]);
        }
    }
    else {
        mIsResetHardActive[reset_id] = false;
        if (reset_id == emPRESETnId) {
            re_printf("info", "The reset port PRESETn is de-asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id]);
        }
        else {
            re_printf("info", "The reset port mip_prst_n is de-asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id]);
        }
    }

    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive[reset_id] && mIsResetHardActive[reset_id]) {
        mResetCmdEvent[reset_id].cancel();
        mResetCmdCancelEvent[reset_id].cancel();
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
    }
}//}}}

///*********************************
/// Function     : HandleResetCmdMethod
/// Description  : Process reset function when reset command is active
/// Parameters   : 
///                const unsigned int reset_id: reset id (0: PRESETn reset port | 1: mip_prst_n reset port)
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Direction_043}
void HWCORE_IRCTG3::HandleResetCmdMethod(const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
    }
    else { //reset_id == emmip_prst_nId
        reset_name = "mip_prst_n";
    }
    mIsResetCmdActive[reset_id] = true;
    re_printf("info", "The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id]);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

///*********************************
/// Function     : CancelResetCmdMethod
/// Description  : Cancel reset function when reset command is active
/// Parameters   : 
///                const unsigned int reset_id: reset id (0: PRESETn reset port | 1: mip_prst_n reset port)
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Direction_042}
void HWCORE_IRCTG3::CancelResetCmdMethod(const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
    }
    else {
        reset_name = "mip_prst_n";
    }
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info", "Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id]);
}//}}}

///*********************************
/// Function     : EnableReset
/// Description  : Reset model and its sub-instance if reset is actived
/// Parameters   : 
///                const bool is_active (0: de-assert reset | 1: assert reset)
/// Return value : None
///*********************************
// Ref:{HWCORE_IRCTG3_UD_Flow_007}
void HWCORE_IRCTG3::EnableReset(const bool is_active)
{//{{{
    //Ports, events are initialized/canceled only if mip_prst_n is asserted
    if (is_active) {
        if (GetResetStatus("mip_prst_n") == true) {
            // Cancel events
            CancelEvents();
            // Initialize member variables
            Initialize();
            // Write output ports zero
            mUpdateOutputPortEvent.notify(SC_ZERO_TIME);
            // Write interrupt ports zero
            mUpdateInterruptPortEvent.notify(SC_ZERO_TIME);
        }
        //Only reset port related to registers
        else {
            for (int i = 0; i < emOUTPUT_PORTS_NUM; i++) {
                mOutputValue[i] = false;
            }
            // Write output ports zero
            mUpdateOutputPortEvent.notify(SC_ZERO_TIME);
        }
    }
    mpHWCORE_IRCTG3_Func->EnableReset(is_active);
}//}}}

//========================================================
//============ Method monitoring input port ==============
//========================================================
///*********************************
/// Function     : HandleSignalMethod_e_angle
/// Description  : handling signal method for port e_angle
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_IRCTG3::HandleSignalMethod_e_angle() {
    unsigned short nCurAngle = e_angle.read();
    //Change electrical angle
    if (nCurAngle != mPrevAngle) {
        //Update previous angle value
        mPrevAngle = nCurAngle;
        //Set angle value for function block
        mpHWCORE_IRCTG3_Func->SetElectricalAngle(nCurAngle);
        //IR3CCTR.EN is 1, trigger startup is generated
        if (mpHWCORE_IRCTG3_Func->GetRegVal(REG_IR3CCTR, 0) == 1) {
            NotifyStartupTriggerGenerated();
        }
    }
}

//========================================================
//============ Operation Methods/Threads =================
//========================================================
///*********************************
/// Function     : RegisterBlockOperationMethod
/// Description  : Register Block Operation
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_IRCTG3::RegisterBlockOperationMethod() {
    mpHWCORE_IRCTG3_Func->RegisterBlockOperation();
    mZoneSearchAndIterruptEvent.notify(SC_ZERO_TIME);
}

///*********************************
/// Function     : ZoneSeachAndInterruptOperationMethod
/// Description  : Zone Seach And Interrupt Operation
/// Parameters   : None
/// Return value : bool ret: check result (true: clock > 0 | false: clock <=0)
///*********************************
void HWCORE_IRCTG3::ZoneSeachAndInterruptOperationMethod() {
    for (int i = 0; i < emNumPhase; i++) {
        mpHWCORE_IRCTG3_Func->ZoneSeachAndInterruptOperation(i);
    }

    //Clear variables for next time running
    mpHWCORE_IRCTG3_Func->ResetFlagAfterComplete();
    //Notify update outputs
    mUpdateOutputPortEvent.notify(SC_ZERO_TIME);
    //Notify raising interrupts
    mUpdateInterruptPortEvent.notify(SC_ZERO_TIME);
    //Dump statistical information
    if (mDumpStatInfo == true) {
        DumpStatInfo();
    }
}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
///*********************************
/// Function     : CheckClockPeriod
/// Description  : Check clock period
/// Parameters   : None
/// Return value : bool ret: check result (true: clock > 0 | false: clock <=0)
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Direction_041}
bool HWCORE_IRCTG3::CheckClockPeriod(void)
{//{{{
    bool clock_status = false;
    if (mPCLKFreq > 0) {
        clock_status = true;
    }
    else {
        re_printf("info", "PCLK frequency is zero\n");
    }
    return clock_status;
}//}}}

///*********************************
/// Function     : GetResetStatus
/// Description  : Get reset status
/// Parameters   :
///                const std::string reset_name: name of reset to be status checked.
/// Return value : 
///                bool ret: check result (true: reset is asserted | false: reset is not asserted)
///*********************************
bool HWCORE_IRCTG3::GetResetStatus(const std::string reset_name)
{//{{{
    bool reset_status = false;
    if (reset_name == "PRESETn") {
        if (mIsResetHardActive[emPRESETnId] || mIsResetCmdActive[emPRESETnId]) {
            reset_status = true;
        }
    }
    else if (reset_name == "mip_prst_n") {
        if (mIsResetHardActive[emmip_prst_nId] || mIsResetCmdActive[emmip_prst_nId]) {
            reset_status = true;
        }
    }
    return reset_status;
}//}}}

///*********************************
/// Function     : NotifyStartupTriggerGenerated
/// Description  : method for notifying that startup trigger is generated
/// Parameters   : None
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_009, HWCORE_IRCTG3_UD_Direction_055, HWCORE_IRCTG3_UD_Direction_058}
void HWCORE_IRCTG3::NotifyStartupTriggerGenerated()
{//{{{
    //Atleast one reset port is asserted or clock is zero.
    if (GetResetStatus("mip_prst_n") == true || CheckClockPeriod() == false) {
        re_printf("info", "Startup trigger cannot be generated when reset is asserted or clock is zero.\n");
        return;
    }
    re_printf("info", "Startup trigger is generated.\n");
    mRegisterBlockEvent.notify(SC_ZERO_TIME);
}//}}}

///*********************************
/// Function     : UpdateInterruptValue
/// Description  : update interrupt value
/// Parameters   : 
///                int InterruptIndex: index of interrupts
/// Return value : None
///*********************************
void HWCORE_IRCTG3::UpdateInterruptValue(unsigned int InterruptIndex, bool value)
{//{{{    
    mInterruptValue[InterruptIndex] = value;
}//}}}

 ///*********************************
 /// Function     : UpdateOutputValue
 /// Description  : method for update outputs from function block 
 /// Parameters   : 
 ///                int OutputIndex    : index of output
 ///                unsigned int value : value to be update to specified output port
 /// Return value : None
 ///*********************************
void HWCORE_IRCTG3::UpdateOutputValue(unsigned int OutputIndex, unsigned int value)
{//{{{
    mOutputValue[OutputIndex] = value;
}//}}}

//========================================================
//========= Overwrite func. of CRWInterface ==============
//========================================================
///*********************************
/// Function     : read
/// Description  : public API for reading registers of HWCORE_IRCTG3 model
/// Parameters   : 
///                unsigned int offset   : address offset of register to be read
///                   unsigned char* p_data : pointer to contain read data of register
///                   unsigned int size     : length of data to be read
///                   bool debug            : debug flag (0: normal read | 1: debug read)
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_004, HWCORE_IRCTG3_UD_ATT1_009}
void HWCORE_IRCTG3::read(unsigned int offset, unsigned char* p_data, unsigned int size, bool debug)
{//{{{
    if (p_data == NULL) {
        return;
    }
    mpHWCORE_IRCTG3_Func->read(offset, p_data, size, debug);
}//}}}

///*********************************
/// Function     : write
/// Description  : public API for writing registers of HWCORE_IRCTG3 model
/// Parameters   : 
///                unsigned int offset   : address offset of register to be written
///                   unsigned char* p_data : pointer to data to be written to register
///                   unsigned int size     : length of data to be written
///                   bool debug            : debug flag (0: normal write | 1: debug write)
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Flow_005}
void HWCORE_IRCTG3::write(unsigned int offset, unsigned char* p_data, unsigned int size, bool debug)
{//{{{
    if (p_data == NULL) {
        return;
    }
    mpHWCORE_IRCTG3_Func->write(offset, p_data, size, debug);
}//}}}

//========================================================
//==================== Outputs/Interrupts ================
//========================================================
//Ref: {HWCORE_IRCTG3_UD_Flow_006}
///*********************************
/// Function     : UpdateInterruptPortMethod
/// Description  : method for interrupt Assert/De-assert 
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_IRCTG3::UpdateInterruptPortMethod()
{//{{{
    double current_time = 0;
    double next_edge_time = 0;
    bool bIsInterruptRaised = false;

    current_time = sc_time_stamp().to_double();
    next_edge_time = CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mStartClkTime) + (double)mPCLKPeriod;

    for (int nInt = 0; nInt < emINTERRUPT_NUM; nInt++) {
        if (mInterruptValue[nInt] == false && mMapIntr[nInt]->read() == false) {
            continue;
        }
        mMapIntr[nInt]->write(mInterruptValue[nInt]);

        if (mInterruptValue[nInt] == false) {
            //Dump interrupt information when it is de-asserted
            if (mDumpInterrupt == true) {
                re_printf("info", "[INT][%s] De-assert\n", mMapIntr[nInt]->name());
            }
        }
        else {
            bIsInterruptRaised = true;
            //Clear interrupt status
            mInterruptValue[nInt] = false;
            //Dump interrupt information when it is asserted
            if (mDumpInterrupt == true) {
                re_printf("info", "[INT][%s] Assert\n", mMapIntr[nInt]->name());
            }
        }
    }
    //Atleast 1 interrupt raised
    if (bIsInterruptRaised) {
        mUpdateInterruptPortEvent.notify(next_edge_time, mTimeResolutionUnit);
    }
}//}}}

///*********************************
/// Function     : UpdateOutputPortMethod
/// Description  : method for updating outputs
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_IRCTG3::UpdateOutputPortMethod()
{//{{{
    for (int nOut = 0; nOut < emOUTPUT_PORTS_NUM; nOut++) {
        if (mOutputValue[nOut] == mMapOut[nOut]->read()) {
            continue;
        }
        mMapOut[nOut]->write(mOutputValue[nOut]);
    }
}//}}}

///*********************************
/// Function     : NotifyUpdateOutputPort
/// Description  : notify update output ports
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_IRCTG3::NotifyUpdateOutputPort()
{//{{{
    mUpdateOutputPortEvent.notify(SC_ZERO_TIME);
}//}}}

//========================================================
//===================== PYTHON IF ========================
//========================================================
///*********************************
/// Function     : SetMessageLevel
/// Description  : Set message level
/// Parameters   : 
///                const std::string msg_lv : message level
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Direction_016, HWCORE_IRCTG3_UD_Direction_019}
void HWCORE_IRCTG3::SetMessageLevel(const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }

    //Set message level for Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("MessageLevel");
    if (true == mEnableRegisterMessage) {
        SeparateString(cmd, msg_lv);
    }else{
        //Set message level for Register IF class (except fatal level)
        cmd.push_back("fatal");
    }
    mpHWCORE_IRCTG3_Func->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    if (true == mEnableRegisterMessage) {
        SeparateString(cmd, msg_lv);
    }else{
        //Set message level for Register IF class (except fatal level)
        cmd.push_back("fatal");
    }
    mpHWCORE_IRCTG3_Func->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : DumpRegisterRW
/// Description  : Enable/disable dumping message when users access registers
/// Parameters   : 
///                const std::string is_enable: string to determine whether enable or disable dumping read/write register message
/// Return value : None
///*********************************
void HWCORE_IRCTG3::DumpRegisterRW(const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpHWCORE_IRCTG3_Func->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : EnableRegisterMessage
/// Description  : Enable/disable info/warning/error message of register IF
/// Parameters   : 
///                const std::string is_enable: string to determine whether enable or disable dumping read/write register message
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Direction_037, HWCORE_IRCTG3_UD_Direction_048}
void HWCORE_IRCTG3::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        }else if(word_vector[0] == "false"){
            mEnableRegisterMessage = false;
        }else{
            re_printf("warning", "Invalid argument: HWCORE_IRCTG3_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    }else if(word_vector.size() == 0){
        std::string temp = "";
        if(mEnableRegisterMessage){
            temp = "true";
        }else{
            temp = "false";
        }
        re_printf("info", "HWCORE_IRCTG3_EnableRegisterMessage %s\n", temp.c_str());
    }else{
        re_printf("warning", "Invalid argument: HWCORE_IRCTG3_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

///*********************************
/// Function     : DumpInterrupt
/// Description  : Enable/disable dumping interrupt message
/// Parameters   : 
///                const std::string is_enable: string to determine whether enable or disable dumping interrupt message
/// Return value : None
///*********************************
void HWCORE_IRCTG3::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, is_enable);
    if (temp.size() == 1) {
        if (temp[0] == "true") {
            mDumpInterrupt = true;
        } else if (temp[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: HWCORE_IRCTG3_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (temp.size() == 0){
          std::string temp_arg = "";
          if(mDumpInterrupt == true){
              temp_arg = "true";
          }else{
              temp_arg = "false";
          }
        re_printf("info","HWCORE_IRCTG3_DumpInterrupt     %s\n",temp_arg.c_str());
    } else {
        re_printf("warning","Invalid argument: HWCORE_IRCTG3_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

 ///*********************************
 /// Function     : EnableStatInfo
 /// Description  : Enable/disable dumping statistical information
 /// Parameters   : 
 ///                const std::string is_enable: string to determine whether enable or disable dumping statistical information
 /// Return value : None
 ///*********************************
void HWCORE_IRCTG3::EnableStatInfo(const std::string is_enable)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, is_enable);
    if (temp.size() == 1) {
        if (temp[0] == "true") {
            mDumpStatInfo = true;
        }
        else if (temp[0] == "false") {
            mDumpStatInfo = false;
        }
        else {
            re_printf("warning", "Invalid argument: HWCORE_IRCTG3_EnableStatInfo %s\n", is_enable.c_str());
        }
    }
    else if (temp.size() == 0) {
        std::string temp_arg = "";
        if (mDumpStatInfo == true) {
            temp_arg = "true";
        }
        else {
            temp_arg = "false";
        }
        re_printf("info", "HWCORE_IRCTG3_EnableStatInfo     %s\n", temp_arg.c_str());
    }
    else {
        re_printf("warning", "Invalid argument: HWCORE_IRCTG3_EnableStatInfo %s\n", is_enable.c_str());
    }
}//}}}

///*********************************
/// Function     : DumpStatInfo
/// Description  : Dump status register's value
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_IRCTG3::DumpStatInfo(void)
{//{{{
    mpHWCORE_IRCTG3_Func->DumpStatInfo();
}//}}}

///*********************************
/// Function     : AssertReset
/// Description  : Command to assert reset
/// Parameters   : 
///                const std::string reset_name : name of reset to be asserted by command
///                const double start_time      : time to start reset
///                const double period          : reset period
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Direction_038, HWCORE_IRCTG3_UD_Direction_039, HWCORE_IRCTG3_UD_Direction_044}
void HWCORE_IRCTG3::AssertReset(const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsResetHardActive[emPRESETnId]) && (!mIsResetCmdReceive[emPRESETnId])) {
            mIsResetCmdReceive[emPRESETnId] = true;
            re_printf("info", "The model will be reset (PRESETn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emPRESETnId].notify(start_time, SC_NS);
            mResetCmdPeriod[emPRESETnId] = period;
        }
        else {
            re_printf("warning", "The software reset of PRESETn is called in the reset operation of the model. So it is ignored.\n");
        }
    }
    else if (reset_name == "mip_prst_n") {
        if ((!mIsResetHardActive[emmip_prst_nId]) && (!mIsResetCmdReceive[emmip_prst_nId])) {
            mIsResetCmdReceive[emmip_prst_nId] = true;
            re_printf("info", "The model will be reset (mip_prst_n) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emmip_prst_nId].notify(start_time, SC_NS);
            mResetCmdPeriod[emmip_prst_nId] = period;
        }
        else {
            re_printf("warning", "The software reset of mip_prst_n is called in the reset operation of the model. So it is ignored\n");
        }
    }
    else {
        re_printf("warning", "The reset name (%s) is wrong. It should be PRESETn or mip_prst_n", reset_name.c_str());
    }
}//}}}

///*********************************
/// Function     : SetCLKFreq
/// Description  : Set clock value and clock unit
/// Parameters   : 
///                const std::string clock_name : name of clock port to be set
///                const sc_dt::uint64 freq     : clock frequency to be set
///                const std::string unit       : unit of clock frequency to be set
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Direction_036}
void HWCORE_IRCTG3::SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;

    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq(mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1 / (double)mPCLKFreq) * (double)mTimeResolutionValue) + 0.5);
                mStartClkTime = sc_time_stamp().to_double();

                //Proceed to clear interrupt immediately at next positive edge when clock is changed
                mUpdateInterruptPortEvent.notify(SC_ZERO_TIME);
            }else{
                mPCLKPeriod = 0;
                // Prepare for next run
                CancelEvents();
            }
        }else{
            re_printf("warning", "Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    }else{
        re_printf("warning", "Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///*********************************
/// Function     : GetCLKFreq
/// Description  : Dump clock's value
/// Parameters   : 
///                const std::string clock_name : name of clock port to be dump value
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Direction_047}
void HWCORE_IRCTG3::GetCLKFreq(const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info", "PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        }else{
            re_printf("warning", "Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    }else{
        re_printf("warning", "Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ForceRegister
/// Description  : Force value to register
/// Parameters   : 
///                const std::string reg_name   : name of register to be forced value
///                const unsigned int reg_value : value to be forced to register
/// Return value : None
///*********************************
//Ref: {HWCORE_IRCTG3_UD_Direction_030}
void HWCORE_IRCTG3::ForceRegister(const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("force");
            std::ostringstream reg_value_str;
            reg_value_str << "0x" << std::hex << reg_value;
            cmd.push_back(reg_value_str.str());
            mpHWCORE_IRCTG3_Func->RegisterHandler(cmd);
        }else{
            re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ReleaseRegister
/// Description  : Release forced value after forcing registers
/// Parameters   : 
///                const std::string reg_name   : name of rgister to be released
/// Return value : None
///*********************************
void HWCORE_IRCTG3::ReleaseRegister(const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mpHWCORE_IRCTG3_Func->RegisterHandler(cmd);
        }else{
            re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : WriteRegister
/// Description  : Write value to registers by software
/// Parameters   : 
///                const std::string reg_name   : name of rgister to be written
///                const unsigned int reg_value : value to be written to registr
/// Return value : None
///*********************************
void HWCORE_IRCTG3::WriteRegister(const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str << "0x" << std::hex << reg_value;
            cmd.push_back(reg_value_str.str());
            mpHWCORE_IRCTG3_Func->RegisterHandler(cmd);
        }else{
            re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ReadRegister
/// Description  : Read value of register by software
/// Parameters   : 
///                const std::string reg_name   : name of rgister to be read
/// Return value : None
///*********************************
void HWCORE_IRCTG3::ReadRegister(const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mpHWCORE_IRCTG3_Func->RegisterHandler(cmd);
        }else{
            re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ListRegister
/// Description  : List all registers name
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_IRCTG3::ListRegister(void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpHWCORE_IRCTG3_Func->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : Help
/// Description  : Dump help message of all parameters or commands
/// Parameters   : 
///                const std::string type: type to be dump (parameter of command)
/// Return value : None
///*********************************
// Ref: {HWCORE_IRCTG3_UD_Direction_017, HWCORE_IRCTG3_UD_Direction_018, HWCORE_IRCTG3_UD_Direction_029}
void HWCORE_IRCTG3::Help(const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(), "HWCORE_IRCTG3_MessageLevel (\"HWCORE_IRCTG3 instance\", \"fatal|error|warning|info\")        Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(), "HWCORE_IRCTG3_DumpRegisterRW (\"HWCORE_IRCTG3 instance\", \"true/false\")                    Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(), "HWCORE_IRCTG3_EnableRegisterMessage (\"HWCORE_IRCTG3 instance\", \"true/false\")             Enable/disable info/warning/error message of register IF (Default: true)");
            SC_REPORT_INFO(this->basename(), "HWCORE_IRCTG3_DumpInterrupt(\"HWCORE_IRCTG3 instance\", \"true/false\")                      Enable/disable dumping interrupt information HWCORE_IRCTG3(Default: true)");
            SC_REPORT_INFO(this->basename(), "HWCORE_IRCTG3_EnableStatInfo(\"HWCORE_IRCTG3 instance\", \"true/false\")                     Enable/disable dumping statistical information HWCORE_IRCTG3(Default: false)");
        }else if (word_vector[0] == "commands"){
            SC_REPORT_INFO(this->basename(), "HWCORE_IRCTG3_DumpStatInfo (\"HWCORE_IRCTG3 instance\")                                      Dump the statistical info of model");
            SC_REPORT_INFO(this->basename(), "HWCORE_IRCTG3_AssertReset (\"HWCORE_IRCTG3 instance\", \"reset_name\", start_time, period)   Assert and de-assert reset signal to the HWCORE_IRCTG3 model");
            SC_REPORT_INFO(this->basename(), "HWCORE_IRCTG3_SetCLKFreq (\"HWCORE_IRCTG3 instance\", \"clock_name\", freq, \"unit\")        Set clock frequency value to HWCORE_IRCTG3 model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(), "HWCORE_IRCTG3_GetCLKFreq (\"HWCORE_IRCTG3 instance\", \"clock_name\")                        Get clock frequency value of HWCORE_IRCTG3 model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(), "HWCORE_IRCTG3_ForceRegister (\"HWCORE_IRCTG3 instance\", \"reg_name\", reg_value)            Force register with setting value");
            SC_REPORT_INFO(this->basename(), "HWCORE_IRCTG3_ReleaseRegister (\"HWCORE_IRCTG3 instance\", \"reg_name\")                     Release register from force value");
            SC_REPORT_INFO(this->basename(), "HWCORE_IRCTG3_WriteRegister (\"HWCORE_IRCTG3 instance\", \"reg_name\", reg_value)            Write a value to a register");
            SC_REPORT_INFO(this->basename(), "HWCORE_IRCTG3_ReadRegister (\"HWCORE_IRCTG3 instance\", \"reg_name\")                        Read a value from a register");
            SC_REPORT_INFO(this->basename(), "HWCORE_IRCTG3_ListRegister (\"HWCORE_IRCTG3 instance\")                                      Dump name of all registers");
        }else{
            re_printf("warning", "The name (%s) of HWCORE_IRCTG3_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    }else{
        re_printf("warning", "The name (%s) of HWCORE_IRCTG3_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :
