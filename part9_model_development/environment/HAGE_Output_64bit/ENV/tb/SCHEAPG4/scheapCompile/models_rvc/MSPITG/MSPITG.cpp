// ---------------------------------------------------------------------
// $Id: MSPITG.cpp,v 1.5 2020/02/08 08:01:54 ducla Exp $
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
// Description: Copyright of MSPITG.cpp
// Ref: {MSPITG_DD_Copyright_001}
// Description: About the include header files of MSPITG.cpp
// Ref: {MSPITG_DD_ATT1_002, MSPITG_DD_Direction_028, MSPITG_DD_ATT1_004}
#include "MSPITG.h"
#include "MSPITG_Func.h"
using namespace std;
/// Constructor and Destructor
///*********************************
/// Function     : MSPITG
/// Description  : Constructor
///*********************************
// Description: Constructor of MSPITG class
// Ref: {MSPITG_DD_Summary_001, MSPITG_DD_Ports_001, MSPITG_DD_Ports_002, MSPITG_DD_Ports_003, MSPITG_DD_Ports_004, MSPITG_DD_Ports_005, MSPITG_DD_Ports_006, MSPITG_DD_BlockDiagram_001}
// Ref: {MSPITG_DD_Ports_007, MSPITG_DD_Ports_008, MSPITG_DD_Ports_009, MSPITG_DD_Ports_010, MSPITG_DD_Ports_011, MSPITG_DD_Ports_012, MSPITG_DD_Ports_013}
// Ref: {MSPITG_DD_Ports_014, MSPITG_DD_Ports_015, MSPITG_DD_Ports_016, MSPITG_DD_Ports_017, MSPITG_DD_Ports_018, MSPITG_DD_Ports_019, MSPITG_DD_Ports_020}
// Ref: {MSPITG_DD_Ports_021, MSPITG_DD_Ports_022, MSPITG_DD_Ports_023, MSPITG_DD_Ports_024, MSPITG_DD_Ports_025, MSPITG_DD_Ports_026, MSPITG_DD_Ports_027}
// Ref: {MSPITG_DD_Ports_028, MSPITG_DD_Ports_029, MSPITG_DD_Ports_030, MSPITG_DD_Ports_031, MSPITG_DD_Ports_032, MSPITG_DD_Ports_033, MSPITG_DD_Ports_034, MSPITG_DD_Ports_035}
MSPITG::MSPITG(sc_module_name name, const unsigned int rLatency, const unsigned int wLatency)
        :sc_module(name)
        ,MSPITG_AgentController()
        ,BusSlaveBase<32,1>()
        // Clock/Reset
        ,PCLK("PCLK")
        ,PRESETn("PRESETn")
{//{{{
    CommandInit((std::string)name);
    //Bind clock, reset signal and socket to Bus IF
    setSlaveFreqPort32(&PCLK);
    setSlaveResetPort32(&PRESETn);
    setTargetSocket32((char*)"ts");

    ts = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding
    mpMSPITG_Func = new MSPITG_Func((std::string)name, this);
    sc_assert(mpMSPITG_Func != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr((BusSlaveFuncIf *)mpMSPITG_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    // Initialize variables
    mPCLKPeriod = 0;
    mPCLKOrgFreq = 0;
    mPCLKFreq = 0;
    mPCLKFreqUnit = "Hz";
    mResetCurVal = true;
    mResetPreVal = true;
    mIsResetHardActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive = false;
    mResetCmdPeriod = 0;
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    mEnableRegisterMessage = true;
    mDumpInterrupt = false;
    mStartPCLKTime = 0;

    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    Initialize();

    // SC_THREAD/METHOD
    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(HandlePRESETnSignalMethod);
    dont_initialize();
    sensitive << PRESETn;
    
    SC_METHOD(HandleResetHardMethod);
    dont_initialize();
    sensitive << mResetHardEvent;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;

    for (unsigned int id = 0; id < MAX_DMAMSPI_NUM; id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteDMAMSPIEvent[id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::WriteDMAMSPIMethod, this, id),
                          sc_core::sc_gen_unique_name("WriteDMAMSPIMethod"), &opt);
    }

    for (unsigned int id = 0; id < MAX_DMAMSPI_NUM; id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssertDMAMSPIEvent[id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::AssertDMAMSPIMethod, this, id),
                          sc_core::sc_gen_unique_name("AssertDMAMSPIMethod"), &opt);
    }

    for (unsigned int id = 0; id < MAX_DMAMSPI_NUM; id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mDeAssertDMAMSPIEvent[id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::DeAssertDMAMSPIMethod, this, id),
                          sc_core::sc_gen_unique_name("DeAssertDMAMSPIMethod"), &opt);
    }
    for (unsigned int id = 0; id < MAX_DTSMSPI_NUM; id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteDTSMSPIEvent[id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::WriteDTSMSPIMethod, this, id),
                          sc_core::sc_gen_unique_name("WriteDTSMSPIMethod"), &opt);
    }

    for (unsigned int id = 0; id < MAX_DTSMSPI_NUM; id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssertDTSMSPIEvent[id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::AssertDTSMSPIMethod, this, id),
                          sc_core::sc_gen_unique_name("AssertDTSMSPIMethod"), &opt);
    }

    for (unsigned int id = 0; id < MAX_DTSMSPI_NUM; id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mDeAssertDTSMSPIEvent[id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::DeAssertDTSMSPIMethod, this, id),
                          sc_core::sc_gen_unique_name("DeAssertDTSMSPIMethod"), &opt);
    }

    for (unsigned int index = 0; index < 8; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI0TX[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI0InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI0InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 8; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI1TX[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI1InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI1InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 8; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI2TX[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI2InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI2InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 8; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI3TX[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI3InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI3InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 8; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI4TX[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI4InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI4InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 4; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI5TX[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI5InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI5InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 4; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI6TX[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI6InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI6InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 4; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI7TX[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI7InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI7InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 4; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI8TX[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI8InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI8InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 4; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI9TX[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI9InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI9InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 8; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI0RX[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI0InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI0InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 8; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI1RX[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI1InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI1InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 8; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI2RX[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI2InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI2InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 8; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI3RX[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI3InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI3InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 8; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI4RX[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI4InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI4InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 4; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI5RX[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI5InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI5InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 4; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI6RX[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI6InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI6InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 4; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI7RX[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI7InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI7InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 4; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI8RX[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI8InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI8InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 4; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI9RX[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI9InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI9InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 8; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI0FE[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI0InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI0InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 8; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI1FE[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI1InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI1InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 8; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI2FE[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI2InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI2InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 8; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI3FE[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI3InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI3InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 8; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI4FE[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI4InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI4InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 4; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI5FE[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI5InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI5InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 4; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI6FE[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI6InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI6InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 4; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI7FE[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI7InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI7InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 4; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI8FE[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI8InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI8InterruptMethod"), &opt);
    }
    for (unsigned int index = 0; index < 4; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&INTMSPI9FE[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPITG::HandleMSPI9InterruptMethod, this, index), 
                            sc_core::sc_gen_unique_name("HandleMSPI9InterruptMethod"), &opt);
    }
// initialize DMA/DTS port
    for (unsigned int i = 0; i < MAX_DMAMSPI_NUM; i++) {
        DMAMSPI[i].initialize(false);
    }
    for (unsigned int i = 0; i < MAX_DTSMSPI_NUM; i++) {
        DTSMSPI[i].initialize(false);
    }

}//}}} 

///*********************************
/// Function     : ~MSPITG
/// Description  : Destructor
///*********************************
MSPITG::~MSPITG()
{//{{{
    delete mpMSPITG_Func;
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
///*********************************
/// Function     : SetMessageLevel 
/// Description  : Set message level (fatal, error, warning, info)
///*********************************
// Description: SetMessageLevel
// Ref: {MSPITG_DD_Direction_001}
void MSPITG::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }
    if (true == mEnableRegisterMessage) {
        //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpMSPITG_Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpMSPITG_Func->RegisterHandler(cmd);
    } else {
    //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);
    
        mpMSPITG_Func->RegisterHandler(cmd);
    
        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);
    
        mpMSPITG_Func->RegisterHandler(cmd);
    }
}//}}}
///*********************************
/// Function     : DumpRegisterRW 
/// Description  : Enable/disable dumping message when users access registers
///*********************************
// Description: DumpRegisterRW
// Ref: {MSPITG_DD_Direction_002}
void MSPITG::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpMSPITG_Func->RegisterHandler(cmd);
}//}}}
///*********************************
/// Function     : DumpInterrupt 
/// Description  : Enable/disable dumping interrupt information
///*********************************
// Description: DumpInterrupt
// Ref: {MSPITG_DD_Direction_003, MSPITG_DD_Direction_047, MSPITG_DD_Direction_036}
void MSPITG::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: MSPITG_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "false";
        if(mDumpInterrupt){
            temp = "true";
        }else{
            temp = "false";
        }
        re_printf("info","MSPITG_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: MSPITG_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}
///*********************************
/// Function     : EnableRegisterMessage 
/// Description  : Enable/disable EnableRegisterMessage
///*********************************
// Description: EnableRegisterMessage
// Ref: {MSPITG_DD_Direction_004, MSPITG_DD_Flow_007, MSPITG_DD_Direction_049, MSPITG_DD_Direction_036}
void MSPITG::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: MSPITG_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        if (mEnableRegisterMessage) {
            re_printf("info","MSPITG_EnableRegisterMessage %s\n", "true"); 
        } else {
            re_printf("info","MSPITG_EnableRegisterMessage %s\n", "false"); 
        }

    } else {
        re_printf("warning","Invalid argument: MSPITG_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

///*********************************
/// Function     : AssertReset 
/// Description  : Command to assert reset
///*********************************
// Description: AssertReset
// Ref: {MSPITG_DD_Direction_005, MSPITG_DD_Flow_003, MSPITG_DD_Flow_007, MSPITG_DD_Direction_043, MSPITG_DD_Direction_038, MSPITG_DD_Direction_037}
void MSPITG::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsResetHardActive)&&(!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (PRESETn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of PRESETn is called in the reset operation of the model. So it is ignored\n");
        }
    }else{
        re_printf("warning","The reset name (%s) is wrong. It should be PRESETn",reset_name.c_str());
    }
}//}}}
///*********************************
/// Function     : SetCLKFreq 
/// Description  : Set clock value
///*********************************
// Description: SetCLKFreq
// Ref: {MSPITG_DD_Direction_006, MSPITG_DD_Flow_003, MSPITG_DD_Flow_007}
void MSPITG::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq) * (double)mTimeResolutionValue) + 0.5);
                mStartPCLKTime = sc_time_stamp().to_double();
            } else {
                mPCLKPeriod = 0;
                // Cancel events
                CancelEvents();
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}
///*********************************
/// Function     : GetCLKFreq 
/// Description  : Get clock value
///*********************************
// Description: GetCLKFreq
// Ref: {MSPITG_DD_Direction_007, MSPITG_DD_Flow_003, MSPITG_DD_Flow_007, MSPITG_DD_Direction_046, MSPITG_DD_Direction_035}
void MSPITG::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}
///*********************************
/// Function     : ForceRegister 
/// Description  : Force value to register
///*********************************
// Description: ForceRegister
// Ref: {MSPITG_DD_Direction_008, MSPITG_DD_Flow_003, MSPITG_DD_Flow_007}
void MSPITG::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            mpMSPITG_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}
///*********************************
/// Function     : ReleaseRegister 
/// Description  : Release forced value after forcing registers
// Description: ReleaseRegister
// Ref: {MSPITG_DD_Direction_009, MSPITG_DD_Flow_003, MSPITG_DD_Flow_007}
///*********************************
void MSPITG::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mpMSPITG_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}
///*********************************
/// Function     : WriteRegister 
/// Description  : Write value to registers by software
///*********************************
// Description: WriteRegister
// Ref: {MSPITG_DD_Direction_010, MSPITG_DD_Flow_003, MSPITG_DD_Flow_007}
void MSPITG::WriteRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            // Call RegisterHandler of accroding block name
            mpMSPITG_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}
///*********************************
/// Function     : ReadRegister 
/// Description  : Read value of register by software
///*********************************
// Description: ReadRegister
// Ref: {MSPITG_DD_Direction_011, MSPITG_DD_Flow_003, MSPITG_DD_Flow_007}
void MSPITG::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            // Call RegisterHandler of accroding block name
            mpMSPITG_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}
///*********************************
/// Function     : ListRegister 
/// Description  : List all registers name
///*********************************
// Description: ListRegister
// Ref: {MSPITG_DD_Direction_012, MSPITG_DD_Flow_003, MSPITG_DD_Flow_007}
void MSPITG::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpMSPITG_Func->RegisterHandler(cmd);
}//}}}
///*********************************
/// Function     : Help 
/// Description  : Dump help message of all parameters or commands
///*********************************
// Description: Help
// Ref: {MSPITG_DD_Direction_013, MSPITG_DD_Direction_014, MSPITG_DD_Direction_015, MSPITG_DD_Flow_003, MSPITG_DD_Flow_007, MSPITG_DD_Direction_026}
void MSPITG::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"MSPITG_MessageLevel (\"MSPITG instance\", \"fatal|error|warning|info\")                          Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"MSPITG_DumpRegisterRW (\"MSPITG instance\", \"true/false\")                                      Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"MSPITG_DumpInterrupt (\"MSPITG instance\", \"true/false\")                                       Enable/disable dumping interrupt info (Default: false)");
            SC_REPORT_INFO(this->basename(),"MSPITG_EnableRegisterMessage (\"MSPITG instance\", \"true/false\")                               Enable/disable info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"MSPITG_AssertReset (\"MSPITG instance\", \"reset_name\", start_time, period)                     Assert and de-assert reset signal to the MSPITG model");
            SC_REPORT_INFO(this->basename(),"MSPITG_SetCLKFreq (\"MSPITG instance\", \"clock_name\", freq, \"unit\")                          Set clock frequency value to MSPITG model. Valid clock_name is PCLK or MSPInCLK");
            SC_REPORT_INFO(this->basename(),"MSPITG_GetCLKFreq (\"MSPITG instance\", \"clock_name\")                                          Get clock frequency value of MSPITG model. Valid clock_name is PCLK or MSPInCLK");
            SC_REPORT_INFO(this->basename(),"MSPITG_ForceRegister (\"MSPITG instance\",\"reg_name\", reg_value)                               Force register with setting value");
            SC_REPORT_INFO(this->basename(),"MSPITG_ReleaseRegister (\"MSPITG instance\",\"reg_name\")                                        Release register from force value");
            SC_REPORT_INFO(this->basename(),"MSPITG_WriteRegister (\"MSPITG instance\",\"reg_name\", reg_value)                               Write a value to a register");
            SC_REPORT_INFO(this->basename(),"MSPITG_ReadRegister (\"MSPITG instance\",\"reg_name\")                                           Read a value from a register");
            SC_REPORT_INFO(this->basename(),"MSPITG_ListRegister (\"MSPITG instance\")                                                        Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of MSPITG_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of MSPITG_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

///*********************************
/// Function     : Initialize
/// Description  : Initialize variables
///*********************************
void MSPITG::Initialize(void)
{//{{{
    for (unsigned int i=0; i< MAX_MSPI_UNIT; i ++){
        mMSPITGCTL[i]=0;
    }
    for (unsigned int i=0; i< MAX_DMAMSPI_NUM; i ++){
        mDMAMSPI[i]=false;
    }
    for (unsigned int i=0; i< MAX_DTSMSPI_NUM; i ++){
        mDTSMSPI[i]=false;
    }
    mMSPITGDMAALT = 0;
    mMSPITGDTSALT = 0;
}//}}}

//========================================================
//============ Reset and clock  ==========================
//========================================================
///Execute reset operation
///*********************************
/// Function     : EnableReset 
/// Description  : Enable Reset
///*********************************
// Description: EnableReset
// Ref: {MSPITG_DD_Flow_006}
void MSPITG::EnableReset (const bool is_active)
{//{{{
    mpMSPITG_Func->EnableReset (is_active);
    if (is_active) {
        CancelEvents();
        Initialize();
    }
}//}}}

/// Cancel events
///*********************************
/// Function     : CancelEvents
/// Description  : Cancel all events
///*********************************
void MSPITG::CancelEvents()
{//{{{
    for (int i=0; i<MAX_DMAMSPI_NUM; i++){
        mAssertDMAMSPIEvent[i].cancel();    
        mDeAssertDMAMSPIEvent[i].cancel();  
        mWriteDMAMSPIEvent[i].cancel();     
    }
    for (int i=0; i<MAX_DTSMSPI_NUM; i++){
        mAssertDTSMSPIEvent[i].cancel();    
        mDeAssertDTSMSPIEvent[i].cancel();     
        mWriteDTSMSPIEvent[i].cancel();     
    }
}//}}}
///*********************************
/// Function     : DumpInterruptMsg
/// Description  : Dump Interrupt Message
///*********************************
// Description: DumpInterruptMsg
// Ref: {MSPITG_DD_Direction_048}
void MSPITG::DumpInterruptMsg (const std::string intr_name, const bool value)
{//{{{
   if (mDumpInterrupt) {
        if (value) {
            re_printf("info","[%s] is changed to %s.\n",intr_name.c_str(),"1");
        } else {
            re_printf("info","[%s] is changed to %s .\n",intr_name.c_str(),"0");
        }
   }
}//}}}
//========================================================
//============ Method/Threads               ===================
//========================================================
///*********************************
/// Function     : HandlePCLKSignalMethod 
/// Description  : Handle PCLK signal
///*********************************
// Description: HandleResetHardMethod
// Ref: {MSPITG_DD_Flow_002}
void MSPITG::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///*********************************
/// Function     : HandlePRESETnSignalMethod 
/// Description  : Handle PRESETn signal
///*********************************
void MSPITG::HandlePRESETnSignalMethod (void)
{//{{{
    mResetCurVal = PRESETn.read();
    if (mResetCurVal != mResetPreVal) {       //mResetPreVal is "true" at initial state
        mResetPreVal = mResetCurVal;
        mResetHardEvent.notify(SC_ZERO_TIME);
    }
}//}}}


///*********************************
/// Function     : HandleResetHardMethod 
/// Description  : Process reset function when reset port is active
///*********************************
// Description: HandleResetHardMethod
// Ref: {MSPITG_DD_Flow_001, MSPITG_DD_Direction_044, MSPITG_DD_Direction_045}

void MSPITG::HandleResetHardMethod (void)
{//{{{
    if (mResetCurVal == vpcl::emResetActive) {
        mIsResetHardActive = true;
        re_printf("info","The reset port PRESETn is asserted.\n");
        this->EnableReset(mIsResetHardActive);
    } else {
        mIsResetHardActive = false;
        re_printf("info","The reset port PRESETn is de-asserted.\n");
        this->EnableReset(mIsResetHardActive);
    }

    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive && mIsResetHardActive) {
        mResetCmdEvent.cancel();
        mResetCmdCancelEvent.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
    }
}//}}}

///*********************************
/// Function     : HandleResetCmdMethod 
/// Description  : Process reset function when reset command is active
///*********************************
// Description: HandleResetCmdMethod
// Ref: {MSPITG_DD_Direction_042}
void MSPITG::HandleResetCmdMethod (void)
{//{{{
    std::string reset_name = "PRESETn";
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

///Cancel reset function when reset command is active
///*********************************
/// Function     : CancelResetCmdMethod 
/// Description  : Cancel ResetCmd Method
///*********************************
// Description: CancelResetCmdMethod
// Ref: {MSPITG_DD_Direction_041}
void MSPITG::CancelResetCmdMethod (void)
{//{{{
    std::string reset_name = "PRESETn";
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive);
}//}}}



//========================================================
//============ Overwrite func. of agent ==================
//========================================================
///*********************************
/// Function     : CheckClockPeriod 
/// Description  : Check Clock Period
///*********************************
// Description: CheckClockPeriod
// Ref: {MSPITG_DD_Direction_040}
bool MSPITG::CheckClockPeriod (void)
{//{{{
    bool clock_status = false;
    if (mPCLKFreq > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","PCLK frequency is zero\n");
    }
    return clock_status;
}//}}}
///*********************************
/// Function     : GetResetStatus 
/// Description  : Get Reset Status
///*********************************
bool MSPITG::GetResetStatus(void)
{//{{{
   return (mIsResetCmdActive || mIsResetHardActive);
}//}}}
///*********************************
/// Function     : HandleMSPI0InterruptMethod 
/// Description  : Handle Interrupt of Unit 0 Method
///*********************************
// Description: HandleMSPI0InterruptMethod
// Ref: {MSPITG_DD_Flow_011}
void MSPITG::HandleMSPI0InterruptMethod(unsigned int ch_id){
    unsigned int TRGSELValue= 0;
    unsigned int SRCSELValue= 0;
    unsigned int DMAASValue=0;
    unsigned int DTSASValue=0;
    switch (ch_id){
        case 0:
        {
            TRGSELValue = mMSPITGCTL[0]&0x1;
            SRCSELValue = (mMSPITGCTL[0]>>1)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit0, emChanel0, emDMA0);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit0, emChanel0, emDMA0);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit0, emChanel0, emDMA0);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit0, emChanel0, emDMA1);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit0, emChanel0, emDMA1);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit0, emChanel0, emDMA1);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit0, emChanel0, emDMA0);
                    TriggerRXtoDMA(emUnit0, emChanel0, emDMA1);
                }
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit0, emChanel0, emDTS0);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit0, emChanel0, emDTS0);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit0, emChanel0, emDTS0);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit0, emChanel0, emDTS1);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit0, emChanel0, emDTS1);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit0, emChanel0, emDTS1);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit0, emChanel0, emDTS0);
                    TriggerRXtoDTS(emUnit0, emChanel0, emDTS1);
                }
            }
            break;
        }
        case 1:
        {
            TRGSELValue = (mMSPITGCTL[0]>>4)&0x1;
            SRCSELValue = (mMSPITGCTL[0]>>5)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit0, emChanel1, emDMA2);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit0, emChanel1, emDMA2);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit0, emChanel1, emDMA2);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit0, emChanel1, emDMA3);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit0, emChanel1, emDMA3);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit0, emChanel1, emDMA3);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit0, emChanel1, emDMA2);
                    TriggerRXtoDMA(emUnit0, emChanel1, emDMA3);
                }                
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit0, emChanel1, emDTS2);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit0, emChanel1, emDTS2);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit0, emChanel1, emDTS2);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit0, emChanel1, emDTS3);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit0, emChanel1, emDTS3);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit0, emChanel1, emDTS3);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit0, emChanel1, emDTS2);
                    TriggerRXtoDTS(emUnit0, emChanel1, emDTS3);
                }                
            }
            break;
        }
        case 2:
        {
            TRGSELValue = (mMSPITGCTL[0]>>8)&0x1;
            SRCSELValue = (mMSPITGCTL[0]>>9)&0x7;
            DMAASValue =   mMSPITGDMAALT&0x1; 
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit0, emChanel2, emDMA4);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit0, emChanel2, emDMA0) ;                       
                    }    
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit0, emChanel2, emDMA4);
                    if((bool)(DMAASValue))
                    {
                        TriggerRXtoDMA(emUnit0, emChanel2, emDMA0) ;                       
                    }
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit0, emChanel2, emDMA4);
                    if((bool)(DMAASValue))
                    {
                        TriggerFEtoDMA(emUnit0, emChanel2, emDMA0);                        
                    }
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit0, emChanel2, emDMA5);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit0, emChanel2, emDMA1);                        
                    }
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit0, emChanel2, emDMA5);
                    if((bool)(DMAASValue))
                    {
                        TriggerRXtoDMA(emUnit0, emChanel2, emDMA1);                        
                    }                    
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit0, emChanel2, emDMA5);
                    if((bool)(DMAASValue))
                    {
                        TriggerFEtoDMA(emUnit0, emChanel2, emDMA1);
                    }                    
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit0, emChanel2, emDMA4);
                    TriggerRXtoDMA(emUnit0, emChanel2, emDMA5);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit0, emChanel2, emDMA0) ;                         
                        TriggerRXtoDMA(emUnit0, emChanel2, emDMA1);                         
                    }                     
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit0, emChanel2, emDTS4);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit0, emChanel2, emDTS4);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit0, emChanel2, emDTS4);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit0, emChanel2, emDTS5);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit0, emChanel2, emDTS5);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit0, emChanel2, emDTS5);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit0, emChanel2, emDTS4);
                    TriggerRXtoDTS(emUnit0, emChanel2, emDTS5);
                }
            }
            break;
        }
        case 3:
        {
            TRGSELValue = (mMSPITGCTL[0]>>12)&0x1;
            SRCSELValue = (mMSPITGCTL[0]>>13)&0x7;
            DMAASValue =   (mMSPITGDMAALT>>1)&0x1; 
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit0, emChanel3, emDMA6);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit0, emChanel3, emDMA2) ;                       
                    }    
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit0, emChanel3, emDMA6);
                    if((bool)(DMAASValue))
                    {
                        TriggerRXtoDMA(emUnit0, emChanel3, emDMA2) ;                       
                    }
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit0, emChanel3, emDMA6);
                    if((bool)(DMAASValue))
                    {
                        TriggerFEtoDMA(emUnit0, emChanel3, emDMA2);                        
                    }
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit0, emChanel3, emDMA7);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit0, emChanel3, emDMA3);                        
                    }
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit0, emChanel3, emDMA7);
                    if((bool)(DMAASValue))
                    {
                        TriggerRXtoDMA(emUnit0, emChanel3, emDMA3);                        
                    }                    
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit0, emChanel3, emDMA7);
                    if((bool)(DMAASValue))
                    {
                        TriggerFEtoDMA(emUnit0, emChanel3, emDMA3);
                    }                    
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit0, emChanel3, emDMA6);
                    TriggerRXtoDMA(emUnit0, emChanel3, emDMA7);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit0, emChanel3, emDMA2) ;                         
                        TriggerRXtoDMA(emUnit0, emChanel3, emDMA3);                         
                    }                     
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit0, emChanel3, emDTS6);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit0, emChanel3, emDTS6);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit0, emChanel3, emDTS6);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit0, emChanel3, emDTS7);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit0, emChanel3, emDTS7);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit0, emChanel3, emDTS7);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit0, emChanel3, emDTS6);
                    TriggerRXtoDTS(emUnit0, emChanel3, emDTS7);
                }
            }                
            break;            
        }
        case 4:
        {
            TRGSELValue = (mMSPITGCTL[0]>>16)&0x1;
            SRCSELValue = (mMSPITGCTL[0]>>17)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit0, emChanel4, emDMA0);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit0, emChanel4, emDMA0);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit0, emChanel4, emDMA0);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit0, emChanel4, emDMA1);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit0, emChanel4, emDMA1);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit0, emChanel4, emDMA1);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit0, emChanel4, emDMA0);
                    TriggerRXtoDMA(emUnit0, emChanel4, emDMA1);
                }
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit0, emChanel4, emDTS8);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit0, emChanel4, emDTS8);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit0, emChanel4, emDTS8);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit0, emChanel4, emDTS9);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit0, emChanel4, emDTS9);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit0, emChanel4, emDTS9);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit0, emChanel4, emDTS8);
                    TriggerRXtoDTS(emUnit0, emChanel4, emDTS9);
                }
            }
            break;    
        }
        case 5:
        {
            TRGSELValue = (mMSPITGCTL[0]>>20)&0x1;
            SRCSELValue = (mMSPITGCTL[0]>>21)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit0, emChanel5, emDMA2);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit0, emChanel5, emDMA2);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit0, emChanel5, emDMA2);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit0, emChanel5, emDMA3);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit0, emChanel5, emDMA3);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit0, emChanel5, emDMA3);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit0, emChanel5, emDMA2);
                    TriggerRXtoDMA(emUnit0, emChanel5, emDMA3);
                }                
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit0, emChanel5, emDTS10);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit0, emChanel5, emDTS10);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit0, emChanel5, emDTS10);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit0, emChanel5, emDTS11);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit0, emChanel5, emDTS11);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit0, emChanel5, emDTS11);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit0, emChanel5, emDTS10);
                    TriggerRXtoDTS(emUnit0, emChanel5, emDTS11);
                }                
            }            

            break;   
        }
        case 6:
        {
            TRGSELValue = (mMSPITGCTL[0]>>24)&0x1;
            SRCSELValue = (mMSPITGCTL[0]>>25)&0x7;
            DMAASValue =   (mMSPITGDMAALT>>2)&0x1;
            DTSASValue =   mMSPITGDTSALT & 0x1;    
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit0, emChanel6, emDMA4);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit0, emChanel6, emDMA0) ;                       
                    }    
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit0, emChanel6, emDMA4);
                    if((bool)(DMAASValue))
                    {
                        TriggerRXtoDMA(emUnit0, emChanel6, emDMA0) ;                       
                    }
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit0, emChanel6, emDMA4);
                    if((bool)(DMAASValue))
                    {
                        TriggerFEtoDMA(emUnit0, emChanel6, emDMA0);                        
                    }
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit0, emChanel6, emDMA5);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit0, emChanel6, emDMA1);                        
                    }
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit0, emChanel6, emDMA5);
                    if((bool)(DMAASValue))
                    {
                        TriggerRXtoDMA(emUnit0, emChanel6, emDMA1);                        
                    }                    
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit0, emChanel6, emDMA5);
                    if((bool)(DMAASValue))
                    {
                        TriggerFEtoDMA(emUnit0, emChanel6, emDMA1);
                    }                    
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit0, emChanel6, emDMA4);
                    TriggerRXtoDMA(emUnit0, emChanel6, emDMA5);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit0, emChanel6, emDMA0) ;                         
                        TriggerRXtoDMA(emUnit0, emChanel6, emDMA1);                         
                    }                     
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit0, emChanel6, emDTS12);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit0, emChanel6, emDTS8);
                    }
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit0, emChanel6, emDTS12);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit0, emChanel6, emDTS8);
                    }
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit0, emChanel6, emDTS12);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit0, emChanel6, emDTS8);
                    }
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit0, emChanel6, emDTS13);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit0, emChanel6, emDTS9);
                    }
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit0, emChanel6, emDTS13);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit0, emChanel6, emDTS9);
                    }
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit0, emChanel6, emDTS13);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit0, emChanel6, emDTS9);
                    }
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit0, emChanel6, emDTS12);
                    TriggerRXtoDTS(emUnit0, emChanel6, emDTS13);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit0, emChanel6, emDTS8);
                        TriggerRXtoDTS(emUnit0, emChanel6, emDTS9);                        
                    }
                }
            }
            break;            

        }
        case 7:
        {
            TRGSELValue = (mMSPITGCTL[0]>>28)& 0x1;
            SRCSELValue = (mMSPITGCTL[0]>>29)& 0x7;
            DMAASValue =   (mMSPITGDMAALT>>3) & 0x1;
            DTSASValue =   (mMSPITGDTSALT>>1) & 0x1;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit0, emChanel7, emDMA6);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit0, emChanel7, emDMA2) ;                       
                    }    
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit0, emChanel7, emDMA6);
                    if((bool)(DMAASValue))
                    {
                        TriggerRXtoDMA(emUnit0, emChanel7, emDMA2) ;                       
                    }
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit0, emChanel7, emDMA6);
                    if((bool)(DMAASValue))
                    {
                        TriggerFEtoDMA(emUnit0, emChanel7, emDMA2);                        
                    }
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit0, emChanel7, emDMA7);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit0, emChanel7, emDMA3);                        
                    }
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit0, emChanel7, emDMA7);
                    if((bool)(DMAASValue))
                    {
                        TriggerRXtoDMA(emUnit0, emChanel7, emDMA3);                        
                    }                    
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit0, emChanel7, emDMA7);
                    if((bool)(DMAASValue))
                    {
                        TriggerFEtoDMA(emUnit0, emChanel7, emDMA3);
                    }                    
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit0, emChanel7, emDMA6);
                    TriggerRXtoDMA(emUnit0, emChanel7, emDMA7);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit0, emChanel7, emDMA2) ;                         
                        TriggerRXtoDMA(emUnit0, emChanel7, emDMA3);                         
                    }                     
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit0, emChanel7, emDTS14);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit0, emChanel7, emDTS10);
                    }
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit0, emChanel7, emDTS14);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit0, emChanel7, emDTS10);
                    }
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit0, emChanel7, emDTS14);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit0, emChanel7, emDTS10);
                    }
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit0, emChanel7, emDTS15);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit0, emChanel7, emDTS11);
                    }
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit0, emChanel7, emDTS15);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit0, emChanel7, emDTS11);
                    }
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit0, emChanel7, emDTS15);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit0, emChanel7, emDTS11);
                    }
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit0, emChanel7, emDTS14);
                    TriggerRXtoDTS(emUnit0, emChanel7, emDTS15);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit0, emChanel7, emDTS10);
                        TriggerRXtoDTS(emUnit0, emChanel7, emDTS11);                        
                    }
                }
            }
            break;             
        }
        default:
        {
            break;
        }
    }
}
///*********************************
/// Function     : HandleMSPI1InterruptMethod 
/// Description  : Handle Interrupt of Unit 1 Method
///*********************************
// Description: HandleMSPI1InterruptMethod
// Ref: {MSPITG_DD_Flow_011}
void MSPITG::HandleMSPI1InterruptMethod(unsigned int ch_id){
    unsigned int TRGSELValue= 0;
    unsigned int SRCSELValue= 0;
    unsigned int DMAASValue=0;
    unsigned int DTSASValue=0;
    switch (ch_id){
        case 0:
        {
            TRGSELValue = mMSPITGCTL[1]&0x1;
            SRCSELValue = (mMSPITGCTL[1]>>1)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit1, emChanel0, emDMA8);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit1, emChanel0, emDMA8);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit1, emChanel0, emDMA8);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit1, emChanel0, emDMA9);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit1, emChanel0, emDMA9);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit1, emChanel0, emDMA9);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit1, emChanel0, emDMA8);
                    TriggerRXtoDMA(emUnit1, emChanel0, emDMA9);
                }
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit1, emChanel0, emDTS16);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit1, emChanel0, emDTS16);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit1, emChanel0, emDTS16);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit1, emChanel0, emDTS17);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit1, emChanel0, emDTS17);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit1, emChanel0, emDTS17);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit1, emChanel0, emDTS16);
                    TriggerRXtoDTS(emUnit1, emChanel0, emDTS17);
                }
            }
            break;
        }
        case 1:
        {
            TRGSELValue = (mMSPITGCTL[1]>>4)&0x1;
            SRCSELValue = (mMSPITGCTL[1]>>5)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit1, emChanel1, emDMA10);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit1, emChanel1, emDMA10);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit1, emChanel1, emDMA10);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit1, emChanel1, emDMA11);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit1, emChanel1, emDMA11);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit1, emChanel1, emDMA11);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit1, emChanel1, emDMA10);
                    TriggerRXtoDMA(emUnit1, emChanel1, emDMA11);
                }                
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit1, emChanel1, emDTS18);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit1, emChanel1, emDTS18);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit1, emChanel1, emDTS18);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit1, emChanel1, emDTS19);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit1, emChanel1, emDTS19);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit1, emChanel1, emDTS19);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit1, emChanel1, emDTS18);
                    TriggerRXtoDTS(emUnit1, emChanel1, emDTS19);
                }                
            }
            break;
        }
        case 2:
        {
            TRGSELValue = (mMSPITGCTL[1]>>8)&0x1;
            SRCSELValue = (mMSPITGCTL[1]>>9)&0x7;
            DMAASValue =  (mMSPITGDMAALT>>4)&0x1; 
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit1, emChanel2, emDMA12);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit1, emChanel2, emDMA8) ;                       
                    }    
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit1, emChanel2, emDMA12);
                    if((bool)(DMAASValue))
                    {
                        TriggerRXtoDMA(emUnit1, emChanel2, emDMA8) ;                       
                    }
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit1, emChanel2, emDMA12);
                    if((bool)(DMAASValue))
                    {
                        TriggerFEtoDMA(emUnit1, emChanel2, emDMA8);                        
                    }
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit1, emChanel2, emDMA13);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit1, emChanel2, emDMA9);                        
                    }
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit1, emChanel2, emDMA13);
                    if((bool)(DMAASValue))
                    {
                        TriggerRXtoDMA(emUnit1, emChanel2, emDMA9);                        
                    }                    
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit1, emChanel2, emDMA13);
                    if((bool)(DMAASValue))
                    {
                        TriggerFEtoDMA(emUnit1, emChanel2, emDMA9);
                    }                    
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit1, emChanel2, emDMA12);
                    TriggerRXtoDMA(emUnit1, emChanel2, emDMA13);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit1, emChanel2, emDMA8) ;                         
                        TriggerRXtoDMA(emUnit1, emChanel2, emDMA9);                         
                    }                     
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit1, emChanel2, emDTS20);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit1, emChanel2, emDTS20);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit1, emChanel2, emDTS20);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit1, emChanel2, emDTS21);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit1, emChanel2, emDTS21);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit1, emChanel2, emDTS21);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit1, emChanel2, emDTS20);
                    TriggerRXtoDTS(emUnit1, emChanel2, emDTS21);
                }
            }
            break;
        }
        case 3:
        {
            TRGSELValue = (mMSPITGCTL[1]>>12)&0x1;
            SRCSELValue = (mMSPITGCTL[1]>>13)&0x7;
            DMAASValue =   (mMSPITGDMAALT>>5)&0x1; 
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit1, emChanel3, emDMA14);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit1, emChanel3, emDMA10) ;                       
                    }    
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit1, emChanel3, emDMA14);
                    if((bool)(DMAASValue))
                    {
                        TriggerRXtoDMA(emUnit1, emChanel3, emDMA10) ;                       
                    }
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit1, emChanel3, emDMA14);
                    if((bool)(DMAASValue))
                    {
                        TriggerFEtoDMA(emUnit1, emChanel3, emDMA10);                        
                    }
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit1, emChanel3, emDMA15);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit1, emChanel3, emDMA11);                        
                    }
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit1, emChanel3, emDMA15);
                    if((bool)(DMAASValue))
                    {
                        TriggerRXtoDMA(emUnit1, emChanel3, emDMA11);                        
                    }                    
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit1, emChanel3, emDMA15);
                    if((bool)(DMAASValue))
                    {
                        TriggerFEtoDMA(emUnit1, emChanel3, emDMA11);
                    }                    
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit1, emChanel3, emDMA14);
                    TriggerRXtoDMA(emUnit1, emChanel3, emDMA15);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit1, emChanel3, emDMA10) ;                         
                        TriggerRXtoDMA(emUnit1, emChanel3, emDMA11);                         
                    }                     
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit1, emChanel3, emDTS22);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit1, emChanel3, emDTS22);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit1, emChanel3, emDTS22);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit1, emChanel3, emDTS23);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit1, emChanel3, emDTS23);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit1, emChanel3, emDTS23);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit1, emChanel3, emDTS22);
                    TriggerRXtoDTS(emUnit1, emChanel3, emDTS23);
                }
            }                
            break;            
        }
        case 4:
        {
            TRGSELValue = (mMSPITGCTL[1]>>16)&0x1;
            SRCSELValue = (mMSPITGCTL[1]>>17)&0x7;
            DTSASValue =   (mMSPITGDTSALT>>3) & 0x1;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit1, emChanel4, emDMA8);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit1, emChanel4, emDMA8);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit1, emChanel4, emDMA8);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit1, emChanel4, emDMA9);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit1, emChanel4, emDMA9);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit1, emChanel4, emDMA9);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit1, emChanel4, emDMA8);
                    TriggerRXtoDMA(emUnit1, emChanel4, emDMA9);
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit1, emChanel4, emDTS24);
                    if((bool)(DTSASValue)){
                       TriggerTXtoDTS(emUnit1, emChanel4, emDTS16);;
                   }                   
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit1, emChanel4, emDTS24);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit1, emChanel4, emDTS16);
                    }                    
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit1, emChanel4, emDTS24);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit1, emChanel4, emDTS16);
                    }                      
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit1, emChanel4, emDTS25);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit1, emChanel4, emDTS17);
                    }  
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit1, emChanel4, emDTS25);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit1, emChanel4, emDTS17);
                    }  
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit1, emChanel4, emDTS25);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit1, emChanel4, emDTS17);
                    }  
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit1, emChanel4, emDTS24);
                    TriggerRXtoDTS(emUnit1, emChanel4, emDTS25);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit1, emChanel4, emDTS16);
                        TriggerRXtoDTS(emUnit1, emChanel4, emDTS17);
                    }  
                }
            }
            break;    
        }
        case 5:
        {
            TRGSELValue = (mMSPITGCTL[1]>>20)&0x1;
            SRCSELValue = (mMSPITGCTL[1]>>21)&0x7;
            DTSASValue =   (mMSPITGDTSALT>>3) & 0x1;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit1, emChanel5, emDMA10);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit1, emChanel5, emDMA10);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit1, emChanel5, emDMA10);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit1, emChanel5, emDMA11);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit1, emChanel5, emDMA11);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit1, emChanel5, emDMA11);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit1, emChanel5, emDMA10);
                    TriggerRXtoDMA(emUnit1, emChanel5, emDMA11);
                }                
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit1, emChanel5, emDTS26);
                   if((bool)(DTSASValue)){
                       TriggerTXtoDTS(emUnit1, emChanel5, emDTS18);
                   }
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit1, emChanel5, emDTS26);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit1, emChanel5, emDTS18);
                    }
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit1, emChanel5, emDTS26);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit1, emChanel5, emDTS18);
                    }
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit1, emChanel5, emDTS27);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit1, emChanel5, emDTS19);
                    }
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit1, emChanel5, emDTS27);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit1, emChanel5, emDTS19);
                    }
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit1, emChanel5, emDTS27);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit1, emChanel5, emDTS19);
                    }
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit1, emChanel5, emDTS26);
                    TriggerRXtoDTS(emUnit1, emChanel5, emDTS27);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit1, emChanel5, emDTS18);
                        TriggerRXtoDTS(emUnit1, emChanel5, emDTS19);
                    }
                }                
            }            

            break;   
        }
        case 6:
        {
            TRGSELValue = (mMSPITGCTL[1]>>24)&0x1;
            SRCSELValue = (mMSPITGCTL[1]>>25)&0x7;
            DMAASValue =   (mMSPITGDMAALT>>6)&0x1;
            DTSASValue =   (mMSPITGDTSALT>>4) & 0x1; 
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit1, emChanel6, emDMA12);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit1, emChanel6, emDMA8) ;                       
                    }    
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit1, emChanel6, emDMA12);
                    if((bool)(DMAASValue))
                    {
                        TriggerRXtoDMA(emUnit1, emChanel6, emDMA8) ;                       
                    }
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit1, emChanel6, emDMA12);
                    if((bool)(DMAASValue))
                    {
                        TriggerFEtoDMA(emUnit1, emChanel6, emDMA8);                        
                    }
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit1, emChanel6, emDMA13);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit1, emChanel6, emDMA9);                        
                    }
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit1, emChanel6, emDMA13);
                    if((bool)(DMAASValue))
                    {
                        TriggerRXtoDMA(emUnit1, emChanel6, emDMA9);                        
                    }                    
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit1, emChanel6, emDMA13);
                    if((bool)(DMAASValue))
                    {
                        TriggerFEtoDMA(emUnit1, emChanel6, emDMA9);
                    }                    
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit1, emChanel6, emDMA12);
                    TriggerRXtoDMA(emUnit1, emChanel6, emDMA13);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit1, emChanel6, emDMA8) ;                         
                        TriggerRXtoDMA(emUnit1, emChanel6, emDMA9);                         
                    }                     
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit1, emChanel6, emDTS28);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit1, emChanel6, emDTS20);
                    }
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit1, emChanel6, emDTS28);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit1, emChanel6, emDTS20);
                    }
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit1, emChanel6, emDTS28);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit1, emChanel6, emDTS20);
                    }
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit1, emChanel6, emDTS29);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit1, emChanel6, emDTS21);
                    }
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit1, emChanel6, emDTS29);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit1, emChanel6, emDTS21);
                    }
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit1, emChanel6, emDTS29);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit1, emChanel6, emDTS21);
                    }
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit1, emChanel6, emDTS28);
                    TriggerRXtoDTS(emUnit1, emChanel6, emDTS29);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit1, emChanel6, emDTS20);
                        TriggerRXtoDTS(emUnit1, emChanel6, emDTS21);                        
                    }
                }
            }
            break;            

        }
        case 7:
        {
            TRGSELValue = (mMSPITGCTL[1]>>28)& 0x1;
            SRCSELValue = (mMSPITGCTL[1]>>29)& 0x7;
            DMAASValue =   (mMSPITGDMAALT>>7) & 0x1;
            DTSASValue =   (mMSPITGDTSALT>>5) & 0x1;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit1, emChanel7, emDMA14);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit1, emChanel7, emDMA10) ;                       
                    }    
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit1, emChanel7, emDMA14);
                    if((bool)(DMAASValue))
                    {
                        TriggerRXtoDMA(emUnit1, emChanel7, emDMA10) ;                       
                    }
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit1, emChanel7, emDMA14);
                    if((bool)(DMAASValue))
                    {
                        TriggerFEtoDMA(emUnit1, emChanel7, emDMA10);                        
                    }
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit1, emChanel7, emDMA15);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit1, emChanel7, emDMA11);                        
                    }
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit1, emChanel7, emDMA15);
                    if((bool)(DMAASValue))
                    {
                        TriggerRXtoDMA(emUnit1, emChanel7, emDMA11);                        
                    }                    
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit1, emChanel7, emDMA15);
                    if((bool)(DMAASValue))
                    {
                        TriggerFEtoDMA(emUnit1, emChanel7, emDMA11);
                    }                    
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit1, emChanel7, emDMA14);
                    TriggerRXtoDMA(emUnit1, emChanel7, emDMA15);
                    if((bool)(DMAASValue))
                    {
                        TriggerTXtoDMA(emUnit1, emChanel7, emDMA10) ;                         
                        TriggerRXtoDMA(emUnit1, emChanel7, emDMA11);                         
                    }                     
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit1, emChanel7, emDTS30);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit1, emChanel7, emDTS22);
                    }
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit1, emChanel7, emDTS30);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit1, emChanel7, emDTS22);
                    }
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit1, emChanel7, emDTS30);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit1, emChanel7, emDTS22);
                    }
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit1, emChanel7, emDTS31);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit1, emChanel7, emDTS23);
                    }
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit1, emChanel7, emDTS31);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit1, emChanel7, emDTS23);
                    }
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit1, emChanel7, emDTS31);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit1, emChanel7, emDTS23);
                    }
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit1, emChanel7, emDTS30);
                    TriggerRXtoDTS(emUnit1, emChanel7, emDTS31);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit1, emChanel7, emDTS22);
                        TriggerRXtoDTS(emUnit1, emChanel7, emDTS23);                        
                    }
                }
            }
            break;             
        }
        default:
        {
            break;
        }
    }
}

///*********************************
/// Function     : HandleMSPI2InterruptMethod 
/// Description  : Handle Interrupt of Unit 2 Method
///*********************************
// Description: HandleMSPI2InterruptMethod
// Ref: {MSPITG_DD_Flow_011}
void MSPITG::HandleMSPI2InterruptMethod(unsigned int ch_id){
    unsigned int TRGSELValue= 0;
    unsigned int SRCSELValue= 0;
    unsigned int DTSASValue=0;
    switch (ch_id){
        case 0:
        {
            TRGSELValue = mMSPITGCTL[2]&0x1;
            SRCSELValue = (mMSPITGCTL[2]>>1)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit2, emChanel0, emDMA16);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit2, emChanel0, emDMA16);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit2, emChanel0, emDMA16);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit2, emChanel0, emDMA17);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit2, emChanel0, emDMA17);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit2, emChanel0, emDMA17);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit2, emChanel0, emDMA16);
                    TriggerRXtoDMA(emUnit2, emChanel0, emDMA17);
                }
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit2, emChanel0, emDTS32);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit2, emChanel0, emDTS32);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit2, emChanel0, emDTS32);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit2, emChanel0, emDTS33);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit2, emChanel0, emDTS33);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit2, emChanel0, emDTS33);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit2, emChanel0, emDTS32);
                    TriggerRXtoDTS(emUnit2, emChanel0, emDTS33);
                }
            }
            break;
        }
        case 1:
        {
            TRGSELValue = (mMSPITGCTL[2]>>4)&0x1;
            SRCSELValue = (mMSPITGCTL[2]>>5)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit2, emChanel1, emDMA18);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit2, emChanel1, emDMA18);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit2, emChanel1, emDMA18);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit2, emChanel1, emDMA19);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit2, emChanel1, emDMA19);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit2, emChanel1, emDMA19);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit2, emChanel1, emDMA18);
                    TriggerRXtoDMA(emUnit2, emChanel1, emDMA19);
                }                
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit2, emChanel1, emDTS34);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit2, emChanel1, emDTS34);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit2, emChanel1, emDTS34);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit2, emChanel1, emDTS35);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit2, emChanel1, emDTS35);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit2, emChanel1, emDTS35);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit2, emChanel1, emDTS34);
                    TriggerRXtoDTS(emUnit2, emChanel1, emDTS35);
                }                
            }
            break;
        }
        case 2:
        {
            TRGSELValue = (mMSPITGCTL[2]>>8)&0x1;
            SRCSELValue = (mMSPITGCTL[2]>>9)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit2, emChanel2, emDMA16); 
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit2, emChanel2, emDMA16);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit2, emChanel2, emDMA16);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit2, emChanel2, emDMA17);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit2, emChanel2, emDMA17);                 
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit2, emChanel2, emDMA17);                  
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit2, emChanel2, emDMA16);
                    TriggerRXtoDMA(emUnit2, emChanel2, emDMA17);                   
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit2, emChanel2, emDTS36);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit2, emChanel2, emDTS36);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit2, emChanel2, emDTS36);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit2, emChanel2, emDTS37);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit2, emChanel2, emDTS37);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit2, emChanel2, emDTS37);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit2, emChanel2, emDTS36);
                    TriggerRXtoDTS(emUnit2, emChanel2, emDTS37);
                }
            }
            break;
        }
        case 3:
        {
            TRGSELValue = (mMSPITGCTL[2]>>12)&0x1;
            SRCSELValue = (mMSPITGCTL[2]>>13)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit2, emChanel3, emDMA18); 
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit2, emChanel3, emDMA18);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit2, emChanel3, emDMA18);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit2, emChanel3, emDMA19);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit2, emChanel3, emDMA19);                   
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit2, emChanel3, emDMA19);                  
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit2, emChanel3, emDMA18);
                    TriggerRXtoDMA(emUnit2, emChanel3, emDMA19);                   
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit2, emChanel3, emDTS38);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit2, emChanel3, emDTS38);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit2, emChanel3, emDTS38);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit2, emChanel3, emDTS39);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit2, emChanel3, emDTS39);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit2, emChanel3, emDTS39);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit2, emChanel3, emDTS38);
                    TriggerRXtoDTS(emUnit2, emChanel3, emDTS39);
                }
            }                
            break;            
        }
        case 4:
        {
            TRGSELValue = (mMSPITGCTL[2]>>16)&0x1;
            SRCSELValue = (mMSPITGCTL[2]>>17)&0x7;
            DTSASValue =   (mMSPITGDTSALT>>6) & 0x1;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit2, emChanel4, emDMA16);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit2, emChanel4, emDMA16);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit2, emChanel4, emDMA16);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit2, emChanel4, emDMA17);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit2, emChanel4, emDMA17);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit2, emChanel4, emDMA17);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit2, emChanel4, emDMA16);
                    TriggerRXtoDMA(emUnit2, emChanel4, emDMA17);
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit2, emChanel4, emDTS40);
                    if((bool)(DTSASValue)){
                       TriggerTXtoDTS(emUnit2, emChanel4, emDTS32);;
                   }                   
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit2, emChanel4, emDTS40);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit2, emChanel4, emDTS32);
                    }                    
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit2, emChanel4, emDTS40);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit2, emChanel4, emDTS32);
                    }                      
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit2, emChanel4, emDTS41);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit2, emChanel4, emDTS33);
                    }  
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit2, emChanel4, emDTS41);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit2, emChanel4, emDTS33);
                    }  
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit2, emChanel4, emDTS41);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit2, emChanel4, emDTS33);
                    }  
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit2, emChanel4, emDTS40);
                    TriggerRXtoDTS(emUnit2, emChanel4, emDTS41);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit2, emChanel4, emDTS32);
                        TriggerRXtoDTS(emUnit2, emChanel4, emDTS33);
                    }  
                }
            }
            break;    
        }
        case 5:
        {
            TRGSELValue = (mMSPITGCTL[2]>>20)&0x1;
            SRCSELValue = (mMSPITGCTL[2]>>21)&0x7;
            DTSASValue =   (mMSPITGDTSALT>>7) & 0x1;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit2, emChanel5, emDMA18);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit2, emChanel5, emDMA18);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit2, emChanel5, emDMA18);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit2, emChanel5, emDMA19);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit2, emChanel5, emDMA19);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit2, emChanel5, emDMA19);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit2, emChanel5, emDMA18);
                    TriggerRXtoDMA(emUnit2, emChanel5, emDMA19);
                }                
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit2, emChanel5, emDTS42);
                   if((bool)(DTSASValue)){
                       TriggerTXtoDTS(emUnit2, emChanel5, emDTS34);
                   }
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit2, emChanel5, emDTS42);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit2, emChanel5, emDTS34);
                    }
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit2, emChanel5, emDTS42);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit2, emChanel5, emDTS34);
                    }
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit2, emChanel5, emDTS43);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit2, emChanel5, emDTS35);
                    }
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit2, emChanel5, emDTS43);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit2, emChanel5, emDTS35);
                    }
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit2, emChanel5, emDTS43);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit2, emChanel5, emDTS35);
                    }
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit2, emChanel5, emDTS42);
                    TriggerRXtoDTS(emUnit2, emChanel5, emDTS43);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit2, emChanel5, emDTS34);
                        TriggerRXtoDTS(emUnit2, emChanel5, emDTS35);
                    }
                }                
            }            

            break;   
        }
        case 6:
        {
            TRGSELValue = (mMSPITGCTL[2]>>24)&0x1;
            SRCSELValue = (mMSPITGCTL[2]>>25)&0x7;
            DTSASValue =   (mMSPITGDTSALT>>8) & 0x1; 
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit2, emChanel6, emDMA16);  
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit2, emChanel6, emDMA16);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit2, emChanel6, emDMA16);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit2, emChanel6, emDMA17);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit2, emChanel6, emDMA17);                  
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit2, emChanel6, emDMA17);                   
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit2, emChanel6, emDMA16);
                    TriggerRXtoDMA(emUnit2, emChanel6, emDMA17);                   
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit2, emChanel6, emDTS44);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit2, emChanel6, emDTS36);
                    }
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit2, emChanel6, emDTS44);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit2, emChanel6, emDTS36);
                    }
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit2, emChanel6, emDTS44);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit2, emChanel6, emDTS36);
                    }
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit2, emChanel6, emDTS45);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit2, emChanel6, emDTS37);
                    }
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit2, emChanel6, emDTS45);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit2, emChanel6, emDTS37);
                    }
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit2, emChanel6, emDTS45);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit2, emChanel6, emDTS37);
                    }
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit2, emChanel6, emDTS44);
                    TriggerRXtoDTS(emUnit2, emChanel6, emDTS45);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit2, emChanel6, emDTS36);
                        TriggerRXtoDTS(emUnit2, emChanel6, emDTS37);                        
                    }
                }
            }
            break;            

        }
        case 7:
        {
            TRGSELValue = (mMSPITGCTL[2]>>28)& 0x1;
            SRCSELValue = (mMSPITGCTL[2]>>29)& 0x7;
            DTSASValue =   (mMSPITGDTSALT>>9) & 0x1;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit2, emChanel7, emDMA18);    
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit2, emChanel7, emDMA18);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit2, emChanel7, emDMA18);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit2, emChanel7, emDMA19);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit2, emChanel7, emDMA19);                   
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit2, emChanel7, emDMA19);                 
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit2, emChanel7, emDMA18);
                    TriggerRXtoDMA(emUnit2, emChanel7, emDMA19);                     
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit2, emChanel7, emDTS46);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit2, emChanel7, emDTS38);
                    }
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit2, emChanel7, emDTS46);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit2, emChanel7, emDTS38);
                    }
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit2, emChanel7, emDTS46);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit2, emChanel7, emDTS38);
                    }
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit2, emChanel7, emDTS47);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit2, emChanel7, emDTS39);
                    }
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit2, emChanel7, emDTS47);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit2, emChanel7, emDTS39);
                    }
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit2, emChanel7, emDTS47);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit2, emChanel7, emDTS39);
                    }
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit2, emChanel7, emDTS46);
                    TriggerRXtoDTS(emUnit2, emChanel7, emDTS47);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit2, emChanel7, emDTS38);
                        TriggerRXtoDTS(emUnit2, emChanel7, emDTS39);                        
                    }
                }
            }
            break;             
        }
        default:
        {
            break;
        }
    }
}

///*********************************
/// Function     : HandleMSPI3InterruptMethod 
/// Description  : Handle Interrupt of Unit 3 Method
///*********************************
// Description: HandleMSPI3InterruptMethod
// Ref: {MSPITG_DD_Flow_011}
void MSPITG::HandleMSPI3InterruptMethod(unsigned int ch_id){
    unsigned int TRGSELValue= 0;
    unsigned int SRCSELValue= 0;
    unsigned int DTSASValue=0;
    switch (ch_id){
        case 0:
        {
            TRGSELValue = mMSPITGCTL[3]&0x1;
            SRCSELValue = (mMSPITGCTL[3]>>1)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit3, emChanel0, emDMA20);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit3, emChanel0, emDMA20);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit3, emChanel0, emDMA20);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit3, emChanel0, emDMA21);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit3, emChanel0, emDMA21);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit3, emChanel0, emDMA21);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit3, emChanel0, emDMA20);
                    TriggerRXtoDMA(emUnit3, emChanel0, emDMA21);
                }
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit3, emChanel0, emDTS48);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit3, emChanel0, emDTS48);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit3, emChanel0, emDTS48);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit3, emChanel0, emDTS49);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit3, emChanel0, emDTS49);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit3, emChanel0, emDTS49);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit3, emChanel0, emDTS48);
                    TriggerRXtoDTS(emUnit3, emChanel0, emDTS49);
                }
            }
            break;
        }
        case 1:
        {
            TRGSELValue = (mMSPITGCTL[3]>>4)&0x1;
            SRCSELValue = (mMSPITGCTL[3]>>5)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit3, emChanel1, emDMA22);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit3, emChanel1, emDMA22);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit3, emChanel1, emDMA22);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit3, emChanel1, emDMA23);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit3, emChanel1, emDMA23);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit3, emChanel1, emDMA23);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit3, emChanel1, emDMA22);
                    TriggerRXtoDMA(emUnit3, emChanel1, emDMA23);
                }                
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit3, emChanel1, emDTS50);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit3, emChanel1, emDTS50);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit3, emChanel1, emDTS50);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit3, emChanel1, emDTS51);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit3, emChanel1, emDTS51);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit3, emChanel1, emDTS51);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit3, emChanel1, emDTS50);
                    TriggerRXtoDTS(emUnit3, emChanel1, emDTS51);
                }                
            }
            break;
        }
        case 2:
        {
            TRGSELValue = (mMSPITGCTL[3]>>8)&0x1;
            SRCSELValue = (mMSPITGCTL[3]>>9)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit3, emChanel2, emDMA20); 
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit3, emChanel2, emDMA20);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit3, emChanel2, emDMA20);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit3, emChanel2, emDMA21);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit3, emChanel2, emDMA21);                 
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit3, emChanel2, emDMA21);                  
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit3, emChanel2, emDMA20);
                    TriggerRXtoDMA(emUnit3, emChanel2, emDMA21);                   
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit3, emChanel2, emDTS52);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit3, emChanel2, emDTS52);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit3, emChanel2, emDTS52);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit3, emChanel2, emDTS53);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit3, emChanel2, emDTS53);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit3, emChanel2, emDTS53);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit3, emChanel2, emDTS52);
                    TriggerRXtoDTS(emUnit3, emChanel2, emDTS53);
                }
            }
            break;
        }
        case 3:
        {
            TRGSELValue = (mMSPITGCTL[3]>>12)&0x1;
            SRCSELValue = (mMSPITGCTL[3]>>13)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit3, emChanel3, emDMA22); 
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit3, emChanel3, emDMA22);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit3, emChanel3, emDMA22);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit3, emChanel3, emDMA23);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit3, emChanel3, emDMA23);                   
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit3, emChanel3, emDMA23);                  
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit3, emChanel3, emDMA22);
                    TriggerRXtoDMA(emUnit3, emChanel3, emDMA23);                   
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit3, emChanel3, emDTS54);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit3, emChanel3, emDTS54);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit3, emChanel3, emDTS54);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit3, emChanel3, emDTS55);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit3, emChanel3, emDTS55);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit3, emChanel3, emDTS55);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit3, emChanel3, emDTS54);
                    TriggerRXtoDTS(emUnit3, emChanel3, emDTS55);
                }
            }                
            break;            
        }
        case 4:
        {
            TRGSELValue = (mMSPITGCTL[3]>>16)&0x1;
            SRCSELValue = (mMSPITGCTL[3]>>17)&0x7;
            DTSASValue =   (mMSPITGDTSALT>>10) & 0x1;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit3, emChanel4, emDMA20);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit3, emChanel4, emDMA20);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit3, emChanel4, emDMA20);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit3, emChanel4, emDMA21);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit3, emChanel4, emDMA21);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit3, emChanel4, emDMA21);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit3, emChanel4, emDMA20);
                    TriggerRXtoDMA(emUnit3, emChanel4, emDMA21);
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit3, emChanel4, emDTS56);
                    if((bool)(DTSASValue)){
                       TriggerTXtoDTS(emUnit3, emChanel4, emDTS48);;
                   }                   
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit3, emChanel4, emDTS56);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit3, emChanel4, emDTS48);
                    }                    
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit3, emChanel4, emDTS56);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit3, emChanel4, emDTS48);
                    }                      
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit3, emChanel4, emDTS57);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit3, emChanel4, emDTS49);
                    }  
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit3, emChanel4, emDTS57);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit3, emChanel4, emDTS49);
                    }  
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit3, emChanel4, emDTS57);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit3, emChanel4, emDTS49);
                    }  
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit3, emChanel4, emDTS56);
                    TriggerRXtoDTS(emUnit3, emChanel4, emDTS57);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit3, emChanel4, emDTS48);
                        TriggerRXtoDTS(emUnit3, emChanel4, emDTS49);
                    }  
                }
            }
            break;    
        }
        case 5:
        {
            TRGSELValue = (mMSPITGCTL[3]>>20)&0x1;
            SRCSELValue = (mMSPITGCTL[3]>>21)&0x7;
            DTSASValue =   (mMSPITGDTSALT>>11) & 0x1;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit3, emChanel5, emDMA22);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit3, emChanel5, emDMA22);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit3, emChanel5, emDMA22);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit3, emChanel5, emDMA23);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit3, emChanel5, emDMA23);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit3, emChanel5, emDMA23);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit3, emChanel5, emDMA22);
                    TriggerRXtoDMA(emUnit3, emChanel5, emDMA23);
                }                
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit3, emChanel5, emDTS58);
                   if((bool)(DTSASValue)){
                       TriggerTXtoDTS(emUnit3, emChanel5, emDTS50);
                   }
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit3, emChanel5, emDTS58);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit3, emChanel5, emDTS50);
                    }
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit3, emChanel5, emDTS58);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit3, emChanel5, emDTS50);
                    }
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit3, emChanel5, emDTS59);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit3, emChanel5, emDTS51);
                    }
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit3, emChanel5, emDTS59);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit3, emChanel5, emDTS51);
                    }
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit3, emChanel5, emDTS59);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit3, emChanel5, emDTS51);
                    }
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit3, emChanel5, emDTS58);
                    TriggerRXtoDTS(emUnit3, emChanel5, emDTS59);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit3, emChanel5, emDTS50);
                        TriggerRXtoDTS(emUnit3, emChanel5, emDTS51);
                    }
                }                
            }            

            break;   
        }
        case 6:
        {
            TRGSELValue = (mMSPITGCTL[3]>>24)&0x1;
            SRCSELValue = (mMSPITGCTL[3]>>25)&0x7;
            DTSASValue =   (mMSPITGDTSALT>>12) & 0x1; 
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit3, emChanel6, emDMA20);  
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit3, emChanel6, emDMA20);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit3, emChanel6, emDMA20);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit3, emChanel6, emDMA21);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit3, emChanel6, emDMA21);                  
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit3, emChanel6, emDMA21);                  
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit3, emChanel6, emDMA20);
                    TriggerRXtoDMA(emUnit3, emChanel6, emDMA21);                    
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit3, emChanel6, emDTS60);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit3, emChanel6, emDTS52);
                    }
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit3, emChanel6, emDTS60);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit3, emChanel6, emDTS52);
                    }
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit3, emChanel6, emDTS60);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit3, emChanel6, emDTS52);
                    }
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit3, emChanel6, emDTS61);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit3, emChanel6, emDTS53);
                    }
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit3, emChanel6, emDTS61);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit3, emChanel6, emDTS53);
                    }
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit3, emChanel6, emDTS61);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit3, emChanel6, emDTS53);
                    }
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit3, emChanel6, emDTS60);
                    TriggerRXtoDTS(emUnit3, emChanel6, emDTS61);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit3, emChanel6, emDTS52);
                        TriggerRXtoDTS(emUnit3, emChanel6, emDTS53);                        
                    }
                }
            }
            break;            

        }
        case 7:
        {
            TRGSELValue = (mMSPITGCTL[3]>>28)& 0x1;
            SRCSELValue = (mMSPITGCTL[3]>>29)& 0x7;
            DTSASValue =   (mMSPITGDTSALT>>13) & 0x1;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit3, emChanel7, emDMA22);  
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit3, emChanel7, emDMA22);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit3, emChanel7, emDMA22);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit3, emChanel7, emDMA23);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit3, emChanel7, emDMA23);                  
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit3, emChanel7, emDMA23);                
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit3, emChanel7, emDMA22);
                    TriggerRXtoDMA(emUnit3, emChanel7, emDMA23);                  
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit3, emChanel7, emDTS62);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit3, emChanel7, emDTS54);
                    }
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit3, emChanel7, emDTS62);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit3, emChanel7, emDTS54);
                    }
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit3, emChanel7, emDTS62);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit3, emChanel7, emDTS54);
                    }
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit3, emChanel7, emDTS63);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit3, emChanel7, emDTS55);
                    }
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit3, emChanel7, emDTS63);
                    if((bool)(DTSASValue)){
                        TriggerRXtoDTS(emUnit3, emChanel7, emDTS55);
                    }
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit3, emChanel7, emDTS63);
                    if((bool)(DTSASValue)){
                        TriggerFEtoDTS(emUnit3, emChanel7, emDTS55);
                    }
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit3, emChanel7, emDTS62);
                    TriggerRXtoDTS(emUnit3, emChanel7, emDTS63);
                    if((bool)(DTSASValue)){
                        TriggerTXtoDTS(emUnit3, emChanel7, emDTS54);
                        TriggerRXtoDTS(emUnit3, emChanel7, emDTS55);                        
                    }
                }
            }
            break;             
        }
        default:
        {
            break;
        }
    }
}
///*********************************
/// Function     : HandleMSPI4InterruptMethod 
/// Description  : Handle Interrupt of Unit 4 Method
///*********************************
// Description: HandleMSPI4InterruptMethod
// Ref: {MSPITG_DD_Flow_011}
void MSPITG::HandleMSPI4InterruptMethod(unsigned int ch_id){
    unsigned int TRGSELValue= 0;
    unsigned int SRCSELValue= 0;
    switch (ch_id){
        case 0:
        {
            TRGSELValue = mMSPITGCTL[4]&0x1;
            SRCSELValue = (mMSPITGCTL[4]>>1)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit4, emChanel0, emDMA24);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit4, emChanel0, emDMA24);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit4, emChanel0, emDMA24);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit4, emChanel0, emDMA25);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit4, emChanel0, emDMA25);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit4, emChanel0, emDMA25);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit4, emChanel0, emDMA24);
                    TriggerRXtoDMA(emUnit4, emChanel0, emDMA25);
                }
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit4, emChanel0, emDTS60);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit4, emChanel0, emDTS60);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit4, emChanel0, emDTS60);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit4, emChanel0, emDTS61);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit4, emChanel0, emDTS61);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit4, emChanel0, emDTS61);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit4, emChanel0, emDTS60);
                    TriggerRXtoDTS(emUnit4, emChanel0, emDTS61);
                }
            }
            break;
        }
        case 1:
        {
            TRGSELValue = (mMSPITGCTL[4]>>4)&0x1;
            SRCSELValue = (mMSPITGCTL[4]>>5)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit4, emChanel1, emDMA26);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit4, emChanel1, emDMA26);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit4, emChanel1, emDMA26);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit4, emChanel1, emDMA27);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit4, emChanel1, emDMA27);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit4, emChanel1, emDMA27);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit4, emChanel1, emDMA26);
                    TriggerRXtoDMA(emUnit4, emChanel1, emDMA27);
                }                
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit4, emChanel1, emDTS62);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit4, emChanel1, emDTS62);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit4, emChanel1, emDTS62);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit4, emChanel1, emDTS63);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit4, emChanel1, emDTS63);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit4, emChanel1, emDTS63);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit4, emChanel1, emDTS62);
                    TriggerRXtoDTS(emUnit4, emChanel1, emDTS63);
                }                
            }
            break;
        }
        case 2:
        {
            TRGSELValue = (mMSPITGCTL[4]>>8)&0x1;
            SRCSELValue = (mMSPITGCTL[4]>>9)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit4, emChanel2, emDMA24); 
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit4, emChanel2, emDMA24);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit4, emChanel2, emDMA24);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit4, emChanel2, emDMA25);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit4, emChanel2, emDMA25);                 
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit4, emChanel2, emDMA25);                  
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit4, emChanel2, emDMA24);
                    TriggerRXtoDMA(emUnit4, emChanel2, emDMA25);                   
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit4, emChanel2, emDTS60);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit4, emChanel2, emDTS60);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit4, emChanel2, emDTS60);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit4, emChanel2, emDTS61);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit4, emChanel2, emDTS61);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit4, emChanel2, emDTS61);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit4, emChanel2, emDTS60);
                    TriggerRXtoDTS(emUnit4, emChanel2, emDTS61);
                }
            }
            break;
        }
        case 3:
        {
            TRGSELValue = (mMSPITGCTL[4]>>12)&0x1;
            SRCSELValue = (mMSPITGCTL[4]>>13)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit4, emChanel3, emDMA26); 
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit4, emChanel3, emDMA26);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit4, emChanel3, emDMA26);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit4, emChanel3, emDMA27);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit4, emChanel3, emDMA27);                   
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit4, emChanel3, emDMA27);                  
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit4, emChanel3, emDMA26);
                    TriggerRXtoDMA(emUnit4, emChanel3, emDMA27);                   
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit4, emChanel3, emDTS62);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit4, emChanel3, emDTS62);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit4, emChanel3, emDTS62);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit4, emChanel3, emDTS63);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit4, emChanel3, emDTS63);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit4, emChanel3, emDTS63);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit4, emChanel3, emDTS62);
                    TriggerRXtoDTS(emUnit4, emChanel3, emDTS63);
                }
            }                
            break;            
        }
        case 4:
        {
            TRGSELValue = (mMSPITGCTL[4]>>16)&0x1;
            SRCSELValue = (mMSPITGCTL[4]>>17)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit4, emChanel4, emDMA24);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit4, emChanel4, emDMA24);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit4, emChanel4, emDMA24);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit4, emChanel4, emDMA25);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit4, emChanel4, emDMA25);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit4, emChanel4, emDMA25);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit4, emChanel4, emDMA24);
                    TriggerRXtoDMA(emUnit4, emChanel4, emDMA25);
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit4, emChanel4, emDTS44);                 
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit4, emChanel4, emDTS44);                 
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit4, emChanel4, emDTS44);                     
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit4, emChanel4, emDTS45); 
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit4, emChanel4, emDTS45);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit4, emChanel4, emDTS45);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit4, emChanel4, emDTS44);
                    TriggerRXtoDTS(emUnit4, emChanel4, emDTS45);  
                }
            }
            break;    
        }
        case 5:
        {
            TRGSELValue = (mMSPITGCTL[4]>>20)&0x1;
            SRCSELValue = (mMSPITGCTL[4]>>21)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit4, emChanel5, emDMA26);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit4, emChanel5, emDMA26);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit4, emChanel5, emDMA26);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit4, emChanel5, emDMA27);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit4, emChanel5, emDMA27);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit4, emChanel5, emDMA27);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit4, emChanel5, emDMA26);
                    TriggerRXtoDMA(emUnit4, emChanel5, emDMA27);
                }                
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit4, emChanel5, emDTS46);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit4, emChanel5, emDTS46);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit4, emChanel5, emDTS46);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit4, emChanel5, emDTS47);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit4, emChanel5, emDTS47);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit4, emChanel5, emDTS47);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit4, emChanel5, emDTS46);
                    TriggerRXtoDTS(emUnit4, emChanel5, emDTS47);
                }                
            }            

            break;   
        }
        case 6:
        {
            TRGSELValue = (mMSPITGCTL[4]>>24)&0x1;
            SRCSELValue = (mMSPITGCTL[4]>>25)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit4, emChanel6, emDMA24);  
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit4, emChanel6, emDMA24);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit4, emChanel6, emDMA24);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit4, emChanel6, emDMA25);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit4, emChanel6, emDMA25);                  
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit4, emChanel6, emDMA25);                  
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit4, emChanel6, emDMA24);
                    TriggerRXtoDMA(emUnit4, emChanel6, emDMA25);                    
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit4, emChanel6, emDTS44);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit4, emChanel6, emDTS44);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit4, emChanel6, emDTS44);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit4, emChanel6, emDTS45);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit4, emChanel6, emDTS45);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit4, emChanel6, emDTS45);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit4, emChanel6, emDTS44);
                    TriggerRXtoDTS(emUnit4, emChanel6, emDTS45);
                }
            }
            break;            

        }
        case 7:
        {
            TRGSELValue = (mMSPITGCTL[4]>>28)& 0x1;
            SRCSELValue = (mMSPITGCTL[4]>>29)& 0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit4, emChanel7, emDMA26);  
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit4, emChanel7, emDMA26);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit4, emChanel7, emDMA26);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit4, emChanel7, emDMA27);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit4, emChanel7, emDMA27);                  
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit4, emChanel7, emDMA27);                
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit4, emChanel7, emDMA26);
                    TriggerRXtoDMA(emUnit4, emChanel7, emDMA27);                  
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit4, emChanel7, emDTS46);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit4, emChanel7, emDTS46);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit4, emChanel7, emDTS46);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit4, emChanel7, emDTS47);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit4, emChanel7, emDTS47);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit4, emChanel7, emDTS47);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit4, emChanel7, emDTS46);
                    TriggerRXtoDTS(emUnit4, emChanel7, emDTS47);
                }
            }
            break;             
        }
        default:
        {
            break;
        }
    }

}
///*********************************
/// Function     : HandleMSPI5InterruptMethod 
/// Description  : Handle Interrupt of Unit 5 Method
///*********************************
// Description: HandleMSPI5InterruptMethod
// Ref: {MSPITG_DD_Flow_011}
void MSPITG::HandleMSPI5InterruptMethod(unsigned int ch_id){
    unsigned int TRGSELValue= 0;
    unsigned int SRCSELValue= 0;
    switch (ch_id){
        case 0:
        {
            TRGSELValue = mMSPITGCTL[5]&0x1;
            SRCSELValue = (mMSPITGCTL[5]>>1)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit5, emChanel0, emDMA28);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit5, emChanel0, emDMA28);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit5, emChanel0, emDMA28);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit5, emChanel0, emDMA29);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit5, emChanel0, emDMA29);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit5, emChanel0, emDMA29);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit5, emChanel0, emDMA28);
                    TriggerRXtoDMA(emUnit5, emChanel0, emDMA29);
                }
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit5, emChanel0, emDTS28);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit5, emChanel0, emDTS28);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit5, emChanel0, emDTS28);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit5, emChanel0, emDTS29);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit5, emChanel0, emDTS29);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit5, emChanel0, emDTS29);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit5, emChanel0, emDTS28);
                    TriggerRXtoDTS(emUnit5, emChanel0, emDTS29);
                }
            }
            break;
        }
        case 1:
        {
            TRGSELValue = (mMSPITGCTL[5]>>4)&0x1;
            SRCSELValue = (mMSPITGCTL[5]>>5)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit5, emChanel1, emDMA28);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit5, emChanel1, emDMA28);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit5, emChanel1, emDMA28);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit5, emChanel1, emDMA29);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit5, emChanel1, emDMA29);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit5, emChanel1, emDMA29);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit5, emChanel1, emDMA28);
                    TriggerRXtoDMA(emUnit5, emChanel1, emDMA29);
                }                
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit5, emChanel1, emDTS30);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit5, emChanel1, emDTS30);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit5, emChanel1, emDTS30);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit5, emChanel1, emDTS31);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit5, emChanel1, emDTS31);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit5, emChanel1, emDTS31);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit5, emChanel1, emDTS30);
                    TriggerRXtoDTS(emUnit5, emChanel1, emDTS31);
                }                
            }
            break;
        }
        case 2:
        {
            TRGSELValue = (mMSPITGCTL[5]>>8)&0x1;
            SRCSELValue = (mMSPITGCTL[5]>>9)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit5, emChanel2, emDMA28); 
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit5, emChanel2, emDMA28);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit5, emChanel2, emDMA28);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit5, emChanel2, emDMA29);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit5, emChanel2, emDMA29);                 
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit5, emChanel2, emDMA29);                  
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit5, emChanel2, emDMA28);
                    TriggerRXtoDMA(emUnit5, emChanel2, emDMA29);                   
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit5, emChanel2, emDTS28);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit5, emChanel2, emDTS28);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit5, emChanel2, emDTS28);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit5, emChanel2, emDTS29);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit5, emChanel2, emDTS29);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit5, emChanel2, emDTS29);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit5, emChanel2, emDTS28);
                    TriggerRXtoDTS(emUnit5, emChanel2, emDTS29);
                }
            }
            break;
        }
        case 3:
        {
            TRGSELValue = (mMSPITGCTL[5]>>12)&0x1;
            SRCSELValue = (mMSPITGCTL[5]>>13)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit5, emChanel3, emDMA28); 
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit5, emChanel3, emDMA28);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit5, emChanel3, emDMA28);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit5, emChanel3, emDMA29);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit5, emChanel3, emDMA29);                   
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit5, emChanel3, emDMA29);                  
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit5, emChanel3, emDMA28);
                    TriggerRXtoDMA(emUnit5, emChanel3, emDMA29);                   
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit5, emChanel3, emDTS30);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit5, emChanel3, emDTS30);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit5, emChanel3, emDTS30);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit5, emChanel3, emDTS31);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit5, emChanel3, emDTS31);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit5, emChanel3, emDTS31);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit5, emChanel3, emDTS30);
                    TriggerRXtoDTS(emUnit5, emChanel3, emDTS31);
                }
            }                
            break;            
        }
        default:
        {
            break;
        }
    }

}
///*********************************
/// Function     : HandleMSPI6InterruptMethod 
/// Description  : Handle Interrupt of Unit 6 Method
///*********************************
// Description: HandleMSPI6InterruptMethod
// Ref: {MSPITG_DD_Flow_011}
void MSPITG::HandleMSPI6InterruptMethod(unsigned int ch_id){
    unsigned int TRGSELValue= 0;
    unsigned int SRCSELValue= 0;
    switch (ch_id){
        case 0:
        {
            TRGSELValue = mMSPITGCTL[6]&0x1;
            SRCSELValue = (mMSPITGCTL[6]>>1)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit6, emChanel0, emDMA14);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit6, emChanel0, emDMA14);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit6, emChanel0, emDMA14);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit6, emChanel0, emDMA15);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit6, emChanel0, emDMA15);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit6, emChanel0, emDMA15);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit6, emChanel0, emDMA14);
                    TriggerRXtoDMA(emUnit6, emChanel0, emDMA15);
                }
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit6, emChanel0, emDTS12);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit6, emChanel0, emDTS12);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit6, emChanel0, emDTS12);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit6, emChanel0, emDTS13);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit6, emChanel0, emDTS13);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit6, emChanel0, emDTS13);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit6, emChanel0, emDTS12);
                    TriggerRXtoDTS(emUnit6, emChanel0, emDTS13);
                }
            }
            break;
        }
        case 1:
        {
            TRGSELValue = (mMSPITGCTL[6]>>4)&0x1;
            SRCSELValue = (mMSPITGCTL[6]>>5)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit6, emChanel1, emDMA14);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit6, emChanel1, emDMA14);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit6, emChanel1, emDMA14);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit6, emChanel1, emDMA15);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit6, emChanel1, emDMA15);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit6, emChanel1, emDMA15);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit6, emChanel1, emDMA14);
                    TriggerRXtoDMA(emUnit6, emChanel1, emDMA15);
                }                
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit6, emChanel1, emDTS14);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit6, emChanel1, emDTS14);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit6, emChanel1, emDTS14);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit6, emChanel1, emDTS15);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit6, emChanel1, emDTS15);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit6, emChanel1, emDTS15);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit6, emChanel1, emDTS14);
                    TriggerRXtoDTS(emUnit6, emChanel1, emDTS15);
                }                
            }
            break;
        }
        case 2:
        {
            TRGSELValue = (mMSPITGCTL[6]>>8)&0x1;
            SRCSELValue = (mMSPITGCTL[6]>>9)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit6, emChanel2, emDMA14); 
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit6, emChanel2, emDMA14);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit6, emChanel2, emDMA14);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit6, emChanel2, emDMA15);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit6, emChanel2, emDMA15);                 
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit6, emChanel2, emDMA15);                  
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit6, emChanel2, emDMA14);
                    TriggerRXtoDMA(emUnit6, emChanel2, emDMA15);                   
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit6, emChanel2, emDTS12);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit6, emChanel2, emDTS12);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit6, emChanel2, emDTS12);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit6, emChanel2, emDTS13);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit6, emChanel2, emDTS13);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit6, emChanel2, emDTS13);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit6, emChanel2, emDTS12);
                    TriggerRXtoDTS(emUnit6, emChanel2, emDTS13);
                }
            }
            break;
        }
        case 3:
        {
            TRGSELValue = (mMSPITGCTL[6]>>12)&0x1;
            SRCSELValue = (mMSPITGCTL[6]>>13)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit6, emChanel3, emDMA14); 
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit6, emChanel3, emDMA14);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit6, emChanel3, emDMA14);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit6, emChanel3, emDMA15);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit6, emChanel3, emDMA15);                   
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit6, emChanel3, emDMA15);                  
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit6, emChanel3, emDMA14);
                    TriggerRXtoDMA(emUnit6, emChanel3, emDMA15);                   
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit6, emChanel3, emDTS14);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit6, emChanel3, emDTS14);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit6, emChanel3, emDTS14);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit6, emChanel3, emDTS15);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit6, emChanel3, emDTS15);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit6, emChanel3, emDTS15);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit6, emChanel3, emDTS14);
                    TriggerRXtoDTS(emUnit6, emChanel3, emDTS15);
                }
            }                
            break;            
        }
        default:
        {
            break;
        }
    }
}
///*********************************
/// Function     : HandleMSPI7InterruptMethod 
/// Description  : Handle Interrupt of Unit 7 Method
///*********************************
// Description: HandleMSPI7InterruptMethod
// Ref: {MSPITG_DD_Flow_011}
void MSPITG::HandleMSPI7InterruptMethod(unsigned int ch_id){
    unsigned int TRGSELValue= 0;
    unsigned int SRCSELValue= 0;
    switch (ch_id){
        case 0:
        {
            TRGSELValue = mMSPITGCTL[7]&0x1;
            SRCSELValue = (mMSPITGCTL[7]>>1)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit7, emChanel0, emDMA12);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit7, emChanel0, emDMA12);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit7, emChanel0, emDMA12);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit7, emChanel0, emDMA13);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit7, emChanel0, emDMA13);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit7, emChanel0, emDMA13);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit7, emChanel0, emDMA12);
                    TriggerRXtoDMA(emUnit7, emChanel0, emDMA13);
                }
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit7, emChanel0, emDTS56);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit7, emChanel0, emDTS56);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit7, emChanel0, emDTS56);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit7, emChanel0, emDTS57);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit7, emChanel0, emDTS57);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit7, emChanel0, emDTS57);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit7, emChanel0, emDTS56);
                    TriggerRXtoDTS(emUnit7, emChanel0, emDTS57);
                }
            }
            break;
        }
        case 1:
        {
            TRGSELValue = (mMSPITGCTL[7]>>4)&0x1;
            SRCSELValue = (mMSPITGCTL[7]>>5)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit7, emChanel1, emDMA12);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit7, emChanel1, emDMA12);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit7, emChanel1, emDMA12);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit7, emChanel1, emDMA13);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit7, emChanel1, emDMA13);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit7, emChanel1, emDMA13);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit7, emChanel1, emDMA12);
                    TriggerRXtoDMA(emUnit7, emChanel1, emDMA13);
                }                
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit7, emChanel1, emDTS58);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit7, emChanel1, emDTS58);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit7, emChanel1, emDTS58);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit7, emChanel1, emDTS59);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit7, emChanel1, emDTS59);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit7, emChanel1, emDTS59);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit7, emChanel1, emDTS58);
                    TriggerRXtoDTS(emUnit7, emChanel1, emDTS59);
                }                
            }
            break;
        }
        case 2:
        {
            TRGSELValue = (mMSPITGCTL[7]>>8)&0x1;
            SRCSELValue = (mMSPITGCTL[7]>>9)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit7, emChanel2, emDMA12); 
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit7, emChanel2, emDMA12);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit7, emChanel2, emDMA12);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit7, emChanel2, emDMA13);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit7, emChanel2, emDMA13);                 
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit7, emChanel2, emDMA13);                  
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit7, emChanel2, emDMA12);
                    TriggerRXtoDMA(emUnit7, emChanel2, emDMA13);                   
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit7, emChanel2, emDTS56);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit7, emChanel2, emDTS56);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit7, emChanel2, emDTS56);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit7, emChanel2, emDTS57);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit7, emChanel2, emDTS57);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit7, emChanel2, emDTS57);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit7, emChanel2, emDTS56);
                    TriggerRXtoDTS(emUnit7, emChanel2, emDTS57);
                }
            }
            break;
        }
        case 3:
        {
            TRGSELValue = (mMSPITGCTL[7]>>12)&0x1;
            SRCSELValue = (mMSPITGCTL[7]>>13)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit7, emChanel3, emDMA12); 
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit7, emChanel3, emDMA12);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit7, emChanel3, emDMA12);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit7, emChanel3, emDMA13);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit7, emChanel3, emDMA13);                   
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit7, emChanel3, emDMA13);                  
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit7, emChanel3, emDMA12);
                    TriggerRXtoDMA(emUnit7, emChanel3, emDMA13);                   
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit7, emChanel3, emDTS58);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit7, emChanel3, emDTS58);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit7, emChanel3, emDTS58);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit7, emChanel3, emDTS59);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit7, emChanel3, emDTS59);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit7, emChanel3, emDTS59);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit7, emChanel3, emDTS58);
                    TriggerRXtoDTS(emUnit7, emChanel3, emDTS59);
                }
            }                
            break;            
        }
        default:
        {
            break;
        }
    }
}
///*********************************
/// Function     : HandleMSPI8InterruptMethod 
/// Description  : Handle Interrupt of Unit 8 Method
///*********************************
// Description: HandleMSPI8InterruptMethod
// Ref: {MSPITG_DD_Flow_011}
void MSPITG::HandleMSPI8InterruptMethod(unsigned int ch_id){
    unsigned int TRGSELValue= 0;
    unsigned int SRCSELValue= 0;
    switch (ch_id){
        case 0:
        {
            TRGSELValue = mMSPITGCTL[8]&0x1;
            SRCSELValue = (mMSPITGCTL[8]>>1)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit8, emChanel0, emDMA6);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit8, emChanel0, emDMA6);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit8, emChanel0, emDMA6);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit8, emChanel0, emDMA7);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit8, emChanel0, emDMA7);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit8, emChanel0, emDMA7);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit8, emChanel0, emDMA6);
                    TriggerRXtoDMA(emUnit8, emChanel0, emDMA7);
                }
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit8, emChanel0, emDTS40);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit8, emChanel0, emDTS40);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit8, emChanel0, emDTS40);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit8, emChanel0, emDTS41);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit8, emChanel0, emDTS41);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit8, emChanel0, emDTS41);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit8, emChanel0, emDTS40);
                    TriggerRXtoDTS(emUnit8, emChanel0, emDTS41);
                }
            }
            break;
        }
        case 1:
        {
            TRGSELValue = (mMSPITGCTL[8]>>4)&0x1;
            SRCSELValue = (mMSPITGCTL[8]>>5)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit8, emChanel1, emDMA6);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit8, emChanel1, emDMA6);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit8, emChanel1, emDMA6);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit8, emChanel1, emDMA7);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit8, emChanel1, emDMA7);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit8, emChanel1, emDMA7);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit8, emChanel1, emDMA6);
                    TriggerRXtoDMA(emUnit8, emChanel1, emDMA7);
                }                
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit8, emChanel1, emDTS42);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit8, emChanel1, emDTS42);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit8, emChanel1, emDTS42);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit8, emChanel1, emDTS43);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit8, emChanel1, emDTS43);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit8, emChanel1, emDTS43);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit8, emChanel1, emDTS42);
                    TriggerRXtoDTS(emUnit8, emChanel1, emDTS43);
                }                
            }
            break;
        }
        case 2:
        {
            TRGSELValue = (mMSPITGCTL[8]>>8)&0x1;
            SRCSELValue = (mMSPITGCTL[8]>>9)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit8, emChanel2, emDMA6); 
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit8, emChanel2, emDMA6);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit8, emChanel2, emDMA6);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit8, emChanel2, emDMA7);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit8, emChanel2, emDMA7);                 
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit8, emChanel2, emDMA7);                  
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit8, emChanel2, emDMA6);
                    TriggerRXtoDMA(emUnit8, emChanel2, emDMA7);                   
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit8, emChanel2, emDTS40);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit8, emChanel2, emDTS40);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit8, emChanel2, emDTS40);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit8, emChanel2, emDTS41);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit8, emChanel2, emDTS41);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit8, emChanel2, emDTS41);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit8, emChanel2, emDTS40);
                    TriggerRXtoDTS(emUnit8, emChanel2, emDTS41);
                }
            }
            break;
        }
        case 3:
        {
            TRGSELValue = (mMSPITGCTL[8]>>12)&0x1;
            SRCSELValue = (mMSPITGCTL[8]>>13)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit8, emChanel3, emDMA6); 
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit8, emChanel3, emDMA6);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit8, emChanel3, emDMA6);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit8, emChanel3, emDMA7);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit8, emChanel3, emDMA7);                   
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit8, emChanel3, emDMA7);                  
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit8, emChanel3, emDMA6);
                    TriggerRXtoDMA(emUnit8, emChanel3, emDMA7);                   
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit8, emChanel3, emDTS42);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit8, emChanel3, emDTS42);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit8, emChanel3, emDTS42);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit8, emChanel3, emDTS43);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit8, emChanel3, emDTS43);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit8, emChanel3, emDTS43);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit8, emChanel3, emDTS42);
                    TriggerRXtoDTS(emUnit8, emChanel3, emDTS43);
                }
            }                
            break;            
        }
        default:
        {
            break;
        }
    }
}
///*********************************
/// Function     : HandleMSPI9InterruptMethod 
/// Description  : Handle Interrupt of Unit 9 Method
///*********************************
// Description: HandleMSPI9InterruptMethod
// Ref: {MSPITG_DD_Flow_011}
void MSPITG::HandleMSPI9InterruptMethod(unsigned int ch_id){
    unsigned int TRGSELValue= 0;
    unsigned int SRCSELValue= 0;
    switch (ch_id){
        case 0:
        {
            TRGSELValue = mMSPITGCTL[9]&0x1;
            SRCSELValue = (mMSPITGCTL[9]>>1)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit9, emChanel0, emDMA4);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit9, emChanel0, emDMA4);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit9, emChanel0, emDMA4);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit9, emChanel0, emDMA5);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit9, emChanel0, emDMA5);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit9, emChanel0, emDMA5);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit9, emChanel0, emDMA4);
                    TriggerRXtoDMA(emUnit9, emChanel0, emDMA5);
                }
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit9, emChanel0, emDTS24);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit9, emChanel0, emDTS24);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit9, emChanel0, emDTS24);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit9, emChanel0, emDTS25);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit9, emChanel0, emDTS25);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit9, emChanel0, emDTS25);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit9, emChanel0, emDTS24);
                    TriggerRXtoDTS(emUnit9, emChanel0, emDTS25);
                }
            }
            break;
        }
        case 1:
        {
            TRGSELValue = (mMSPITGCTL[9]>>4)&0x1;
            SRCSELValue = (mMSPITGCTL[9]>>5)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit9, emChanel1, emDMA4);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit9, emChanel1, emDMA4);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit9, emChanel1, emDMA4);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit9, emChanel1, emDMA5);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit9, emChanel1, emDMA5);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit9, emChanel1, emDMA5);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit9, emChanel1, emDMA4);
                    TriggerRXtoDMA(emUnit9, emChanel1, emDMA5);
                }                
            }else{
                if(SRCSELValue ==1)
                {
                   TriggerTXtoDTS(emUnit9, emChanel1, emDTS26);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit9, emChanel1, emDTS26);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit9, emChanel1, emDTS26);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit9, emChanel1, emDTS27);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit9, emChanel1, emDTS27);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit9, emChanel1, emDTS27);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit9, emChanel1, emDTS26);
                    TriggerRXtoDTS(emUnit9, emChanel1, emDTS27);
                }                
            }
            break;
        }
        case 2:
        {
            TRGSELValue = (mMSPITGCTL[9]>>8)&0x1;
            SRCSELValue = (mMSPITGCTL[9]>>9)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit9, emChanel2, emDMA4); 
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit9, emChanel2, emDMA4);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit9, emChanel2, emDMA4);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit9, emChanel2, emDMA5);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit9, emChanel2, emDMA5);                 
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit9, emChanel2, emDMA5);                  
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit9, emChanel2, emDMA4);
                    TriggerRXtoDMA(emUnit9, emChanel2, emDMA5);                   
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit9, emChanel2, emDTS24);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit9, emChanel2, emDTS24);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit9, emChanel2, emDTS24);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit9, emChanel2, emDTS25);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit9, emChanel2, emDTS25);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit9, emChanel2, emDTS25);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit9, emChanel2, emDTS24);
                    TriggerRXtoDTS(emUnit9, emChanel2, emDTS25);
                }
            }
            break;
        }
        case 3:
        {
            TRGSELValue = (mMSPITGCTL[9]>>12)&0x1;
            SRCSELValue = (mMSPITGCTL[9]>>13)&0x7;
            if(TRGSELValue ==0){
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDMA(emUnit9, emChanel3, emDMA4); 
                }else if(SRCSELValue == 2){
                    TriggerRXtoDMA(emUnit9, emChanel3, emDMA4);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDMA(emUnit9, emChanel3, emDMA4);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDMA(emUnit9, emChanel3, emDMA5);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDMA(emUnit9, emChanel3, emDMA5);                   
                }else if (SRCSELValue ==6){
                    TriggerFEtoDMA(emUnit9, emChanel3, emDMA5);                  
                }else if (SRCSELValue == 7){
                    TriggerTXtoDMA(emUnit9, emChanel3, emDMA4);
                    TriggerRXtoDMA(emUnit9, emChanel3, emDMA5);                   
                }
            }else{
                if(SRCSELValue ==1)
                {
                    TriggerTXtoDTS(emUnit9, emChanel3, emDTS26);
                }else if(SRCSELValue == 2){
                    TriggerRXtoDTS(emUnit9, emChanel3, emDTS26);
                }else if (SRCSELValue == 3){
                    TriggerFEtoDTS(emUnit9, emChanel3, emDTS26);
                }else if (SRCSELValue ==4){
                    TriggerTXtoDTS(emUnit9, emChanel3, emDTS27);
                }else if (SRCSELValue == 5){
                    TriggerRXtoDTS(emUnit9, emChanel3, emDTS27);
                }else if (SRCSELValue ==6){
                    TriggerFEtoDTS(emUnit9, emChanel3, emDTS27);
                }else if (SRCSELValue == 7){
                    TriggerTXtoDTS(emUnit9, emChanel3, emDTS26);
                    TriggerRXtoDTS(emUnit9, emChanel3, emDTS27);
                }
            }                
            break;            
        }
        default:
        {
            break;
        }
    }
}
///*********************************
/// Function     : UpdateMSPITGCTLValue 
/// Description  : Update mMSPITGCTLValue
///*********************************
void MSPITG::UpdateMSPITGCTLValue(unsigned int ch_id, unsigned int value){
    mMSPITGCTL[ch_id] = value;
}
///*********************************
/// Function     : UpdateMSPITGDMAALTValue 
/// Description  : Update mMSPITGDMAALTValue
///*********************************
void MSPITG::UpdateMSPITGDMAALTValue(unsigned int value){
    mMSPITGDMAALT = value;
}
///*********************************
/// Function     : UpdateMSPITGDTSALTValue 
/// Description  : Update mMSPITGDTSALTValue
///*********************************
void MSPITG::UpdateMSPITGDTSALTValue(unsigned int value){
    mMSPITGDTSALT = value;
}
///*********************************
/// Function     : WriteDMAMSPIMethod 
/// Description  : Write DMAMSPI Method
///
// Description: WriteDMAMSPIMethod
// Ref: {MSPITG_DD_Feature_001}
void MSPITG::WriteDMAMSPIMethod(unsigned int id){
    std::ostringstream portName;
    portName.str("");
    portName << "DMAMSPI" << id;
    DMAMSPI[id].write(mDMAMSPI[id]);
    DumpInterruptMsg(portName.str(), mDMAMSPI[id]);
}
///*********************************
/// Function     : AssertDMAMSPIMethod 
/// Description  : Assert DMAMSPI Method 
///             
void MSPITG::AssertDMAMSPIMethod(unsigned int id){
    mDMAMSPI[id] = true;
    mWriteDMAMSPIEvent[id].notify();   
}
///*********************************
/// Function     : DeAssertDMAMSPIMethod 
/// Description  : DeAssert DMAMSPI Method 
///  
void MSPITG::DeAssertDMAMSPIMethod(unsigned int id){
    mDMAMSPI[id] = false;
    mWriteDMAMSPIEvent[id].notify();
}
///*********************************
/// Function     : WriteDTSMSPIMethod 
/// Description  : Write DTSMSPI Method
///
// Description: WriteDTSMSPIMethod
// Ref: {MSPITG_DD_Feature_001}  
void MSPITG::WriteDTSMSPIMethod(unsigned int id){
    std::ostringstream portName;
    portName.str("");
    portName << "DTSMSPI" << id;
    DTSMSPI[id].write(mDTSMSPI[id]);
    DumpInterruptMsg(portName.str(), mDTSMSPI[id]);  
}
///*********************************
/// Function     : AssertDTSMSPIMethod 
/// Description  : Assert DTSMSPI Method
///              
void MSPITG::AssertDTSMSPIMethod(unsigned int id){
    mDTSMSPI[id] = true;
    mWriteDTSMSPIEvent[id].notify();
}
///*********************************
/// Function     : DeAssertDTSMSPIMethod 
/// Description  : DeAssert DTSMSPI Method
///  
void MSPITG::DeAssertDTSMSPIMethod(unsigned int id){
    mDTSMSPI[id] = false;
    mWriteDTSMSPIEvent[id].notify(); 
}
///*********************************
/// Function     : TriggerTXtoDMA 
/// Description  : TriggerTXtoDMA DTSMSPI Method
///  
void MSPITG::TriggerTXtoDMA(unsigned int Unit, unsigned int TX_index, unsigned int DMA_index ){
    if(Unit==0){
        if(INTMSPI0TX[TX_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI0TX[TX_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==1){
        if(INTMSPI1TX[TX_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI1TX[TX_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==2){
        if(INTMSPI2TX[TX_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI2TX[TX_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==3){
        if(INTMSPI3TX[TX_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI3TX[TX_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==4){
        if(INTMSPI4TX[TX_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI4TX[TX_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==5){
        if(INTMSPI5TX[TX_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI5TX[TX_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==6){
        if(INTMSPI6TX[TX_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI6TX[TX_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==7){
        if(INTMSPI7TX[TX_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI7TX[TX_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==8){
        if(INTMSPI8TX[TX_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI8TX[TX_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else {
        if(INTMSPI9TX[TX_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI9TX[TX_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }
}
///*********************************
/// Function     : TriggerTXtoDTS 
/// Description  : TriggerTXtoDTS 
/// 
void MSPITG::TriggerTXtoDTS(unsigned int Unit,unsigned int TX_index, unsigned int DTS_index ){
    if(Unit==0){
        if(INTMSPI0TX[TX_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI0TX[TX_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==1){
        if(INTMSPI1TX[TX_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI1TX[TX_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==2){
        if(INTMSPI2TX[TX_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI2TX[TX_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==3){
        if(INTMSPI3TX[TX_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI3TX[TX_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==4){
        if(INTMSPI4TX[TX_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI4TX[TX_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==5){
        if(INTMSPI5TX[TX_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI5TX[TX_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==6){
        if(INTMSPI6TX[TX_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI6TX[TX_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==7){
        if(INTMSPI7TX[TX_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI7TX[TX_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==8){
        if(INTMSPI8TX[TX_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI8TX[TX_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else {
        if(INTMSPI9TX[TX_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI9TX[TX_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }
}
///*********************************
/// Function     : TriggerRXtoDMA 
/// Description  : TriggerRXtoDMA 
/// 
void MSPITG::TriggerRXtoDMA(unsigned int Unit, unsigned int RX_index, unsigned int DMA_index ){
    if(Unit==0){
        if(INTMSPI0RX[RX_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI0RX[RX_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==1){
        if(INTMSPI1RX[RX_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI1RX[RX_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==2){
        if(INTMSPI2RX[RX_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI2RX[RX_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==3){
        if(INTMSPI3RX[RX_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI3RX[RX_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==4){
        if(INTMSPI4RX[RX_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI4RX[RX_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==5){
        if(INTMSPI5RX[RX_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI5RX[RX_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==6){
        if(INTMSPI6RX[RX_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI6RX[RX_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==7){
        if(INTMSPI7RX[RX_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI7RX[RX_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==8){
        if(INTMSPI8RX[RX_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI8RX[RX_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else {
        if(INTMSPI9RX[RX_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI9RX[RX_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }
}
///*********************************
/// Function     : TriggerRXtoDTS 
/// Description  : TriggerRXtoDTS 
///
void MSPITG::TriggerRXtoDTS(unsigned int Unit,unsigned int RX_index, unsigned int DTS_index ){
    if(Unit==0){
        if(INTMSPI0RX[RX_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI0RX[RX_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==1){
        if(INTMSPI1RX[RX_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI1RX[RX_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==2){
        if(INTMSPI2RX[RX_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI2RX[RX_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==3){
        if(INTMSPI3RX[RX_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI3RX[RX_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==4){
        if(INTMSPI4RX[RX_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI4RX[RX_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==5){
        if(INTMSPI5RX[RX_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI5RX[RX_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==6){
        if(INTMSPI6RX[RX_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI6RX[RX_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==7){
        if(INTMSPI7RX[RX_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI7RX[RX_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==8){
        if(INTMSPI8RX[RX_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI8RX[RX_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else {
        if(INTMSPI9RX[RX_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI9RX[RX_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }
}
///*********************************
/// Function     : TriggerFEtoDMA 
/// Description  : TriggerFEtoDMA 
///
void MSPITG::TriggerFEtoDMA(unsigned int Unit, unsigned int FE_index, unsigned int DMA_index ){
    if(Unit==0){
        if(INTMSPI0FE[FE_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI0FE[FE_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==1){
        if(INTMSPI1FE[FE_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI1FE[FE_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==2){
        if(INTMSPI2FE[FE_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI2FE[FE_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==3){
        if(INTMSPI3FE[FE_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI3FE[FE_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==4){
        if(INTMSPI4FE[FE_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI4FE[FE_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==5){
        if(INTMSPI5FE[FE_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI5FE[FE_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==6){
        if(INTMSPI6FE[FE_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI6FE[FE_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==7){
        if(INTMSPI7FE[FE_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI7FE[FE_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else if(Unit==8){
        if(INTMSPI8FE[FE_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI8FE[FE_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }else {
        if(INTMSPI9FE[FE_index].read() != mDMAMSPI[DMA_index]){
          if(INTMSPI9FE[FE_index].read()){
            mAssertDMAMSPIEvent[DMA_index].notify(); 
          }else{
            mDeAssertDMAMSPIEvent[DMA_index].notify();  
          }
        }
    }
}
///*********************************
/// Function     : TriggerFEtoDTS 
/// Description  : TriggerFEtoDTS 
///
void MSPITG::TriggerFEtoDTS(unsigned int Unit,unsigned int FE_index, unsigned int DTS_index ){
    if(Unit==0){
        if(INTMSPI0FE[FE_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI0FE[FE_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==1){
        if(INTMSPI1FE[FE_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI1FE[FE_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==2){
        if(INTMSPI2FE[FE_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI2FE[FE_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==3){
        if(INTMSPI3FE[FE_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI3FE[FE_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==4){
        if(INTMSPI4FE[FE_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI4FE[FE_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==5){
        if(INTMSPI5FE[FE_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI5FE[FE_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==6){
        if(INTMSPI6FE[FE_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI6FE[FE_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==7){
        if(INTMSPI7FE[FE_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI7FE[FE_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else if(Unit==8){
        if(INTMSPI8FE[FE_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI8FE[FE_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }else {
        if(INTMSPI9FE[FE_index].read() != mDTSMSPI[DTS_index]){
          if(INTMSPI9FE[FE_index].read()){
            mAssertDTSMSPIEvent[DTS_index].notify(); 
          }else{
            mDeAssertDTSMSPIEvent[DTS_index].notify();  
          }
        }
    }
}


// vim600: set foldmethod=marker :
