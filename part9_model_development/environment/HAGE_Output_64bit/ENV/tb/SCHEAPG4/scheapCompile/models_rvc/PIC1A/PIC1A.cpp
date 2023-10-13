// ---------------------------------------------------------------------
// $Id: PIC1A.cpp,v 1.2 2014/12/31 06:33:44 dungvannguyen Exp $
//
// Copyright(c) 2014 Renesas Electronics Corporation
// Copyright(c) 2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "PIC1A.h"
#include "PIC1A_Func.h" 

/// Constructor of PIC_Port class
PIC_Port::PIC_Port (sc_module_name name, PIC1A *PIC_Ptr):
        sc_module(name)
{//{{{
    sc_assert(PIC_Ptr != NULL);
    mPIC_Ptr = PIC_Ptr;
    
    // Initialize variables
    Initialize();
    
    std::ostringstream port_name;
    for (unsigned int i = 0; i < PIC1A_AgentController::emInputNum; i++) {
        port_name.str("");
        port_name << "InputPort" << i;
        InputPort[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(InputPort[i] != NULL);
    }
    
    for (unsigned int i = 0; i < PIC1A_AgentController::emOutputNum; i++) {
        port_name.str("");
        port_name << "OutputPort" << i;
        OutputPort[i] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(OutputPort[i] != NULL);
        
        OutputPort[i]->initialize(false);
    }
    
    for (unsigned int i = 0; i < PIC1A_AgentController::emSyncOutputNum; i++) {
        port_name.str("");
        port_name << "SyncOutputPort" << i;
        SyncOutputPort[i] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(SyncOutputPort[i] != NULL);
        
        SyncOutputPort[i]->initialize(false);
    }
    
    for (unsigned int i = 0; i < PIC1A_AgentController::emInputNum; i++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(InputPort[i]);
        opt.set_sensitivity(&mUpdateInputEvent);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PIC_Port::HandleInputPortMethod, this, i),
                          sc_core::sc_gen_unique_name("HandleInputPortMethod"), &opt);
    }
    
    for (unsigned int i = 0; i < PIC1A_AgentController::emInputNum; i++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mHandleInputChangeEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PIC_Port::HandleInputChangeMethod, this, i),
                          sc_core::sc_gen_unique_name("HandleInputChangeMethod"), &opt);
    }
    
    for (unsigned int i = 0; i < PIC1A_AgentController::emOutputNum; i++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteOutputPortEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PIC_Port::WriteOutputPortMethod, this, i),
                          sc_core::sc_gen_unique_name("WriteOutputPortMethod"), &opt);
    }
    
    for (unsigned int i = 0; i < PIC1A_AgentController::emSyncOutputNum; i++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssertSyncOutputEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PIC_Port::AssertSyncOutputPortMethod, this, i),
                          sc_core::sc_gen_unique_name("AssertSyncOutputPortMethod"), &opt);
    }
    
    for (unsigned int i = 0; i < PIC1A_AgentController::emSyncOutputNum; i++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mDeassertSyncOutputEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PIC_Port::DeassertSyncOutputPortMethod, this, i),
                          sc_core::sc_gen_unique_name("DeassertSyncOutputPortMethod"), &opt);
    }
    
    for (unsigned int i = 0; i < PIC1A_AgentController::emSyncOutputNum; i++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteSyncOutputEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PIC_Port::WriteSyncOutputPortMethod, this, i),
                          sc_core::sc_gen_unique_name("WriteSyncOutputPortMethod"), &opt);
    }
}//}}}

/// Destructor of PIC_Port class
PIC_Port::~PIC_Port (void)
{//{{{
    for (unsigned char i = 0; i < PIC1A_AgentController::emInputNum; i++) {
        delete InputPort[i];
    }
    for (unsigned char i = 0; i < PIC1A_AgentController::emOutputNum; i++) {
        delete OutputPort[i];
    }
    for (unsigned char i = 0; i < PIC1A_AgentController::emSyncOutputNum; i++) {
        delete SyncOutputPort[i];
    }
}//}}}

/// Detect the changing of Input ports
void PIC_Port::HandleInputPortMethod (const unsigned int index)
{//{{{
    if (!mPIC_Ptr->GetResetStatus()){
        if (mPIC_Ptr->CheckClockPeriod("PCLK")) {
            if (((PIC1A_AgentController::emANFESO0 <= index) && (index <= PIC1A_AgentController::emANFESO3)) ||
            ((PIC1A_AgentController::emRSVHIZIN0 <= index) && (index <= PIC1A_AgentController::emRSVHIZIN5))) {
                // ANFESOn & RSVHIZINn are asynchronous
                mHandleInputChangeEvent[index].notify(SC_ZERO_TIME);
            } else {
                // Other ports are synchronous with PCLK
                double current_time = sc_time_stamp().to_double();
                mHandleInputChangeEvent[index].notify(mPIC_Ptr->CalculateCLKPosEdge("PCLK", current_time), mPIC_Ptr->mTimeResolutionUnit);
            }
        }
    }
}//}}}

/// Process the changing of Input ports
void PIC_Port::HandleInputChangeMethod (const unsigned int index)
{//{{{
    if (!mPIC_Ptr->GetResetStatus()){
        if (mPIC_Ptr->CheckClockPeriod("PCLK")) {
            // Update internal variables of PIC1A_Func
            bool value = InputPort[index]->read();
            mPIC_Ptr->mPIC1A_Func->UpdateInputPort(index, value);

            switch (index) {
                // ENCA0
                case PIC1A_AgentController::emENCAT0IOV:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_0607();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_23();
                    break;
                case PIC1A_AgentController::emENCAT0IEC:
                    // Detect falling edge
                    if ((value == false) && (mDT_Input[PIC1A_AgentController::emDT00] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcDT((unsigned int)PIC1A_AgentController::emDT00, false);
                    }
                    mDT_Input[PIC1A_AgentController::emDT00] = value;
                    break;
                case PIC1A_AgentController::emENCAT0INT0:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_23();
                    break;
                case PIC1A_AgentController::emENCAT0EQ0:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_1011();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG32_0203();
                    if ((value == false) && (mDT_Input[PIC1A_AgentController::emDT01] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcDT((unsigned int)PIC1A_AgentController::emDT01, false);
                    }
                    mDT_Input[PIC1A_AgentController::emDT01] = value;
                    break;
                case PIC1A_AgentController::emENCAT0ECS:
                case PIC1A_AgentController::emENCAT1ECS:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_04(0);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_04(1);
                    break;
                case PIC1A_AgentController::emENCAT0EQ1:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_1011();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG32_0203();
                    if ((value == false) && (mDT_Input[PIC1A_AgentController::emDT02] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcDT((unsigned int)PIC1A_AgentController::emDT02, false);
                    }
                    mDT_Input[PIC1A_AgentController::emDT02] = value;
                    break;
                case PIC1A_AgentController::emENCAT0INT1:
                case PIC1A_AgentController::emENCAT2INT1:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_08(0);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_08(1);
                    break;
                case PIC1A_AgentController::emENCAT0IUD:
                case PIC1A_AgentController::emTSG1TST2PUD:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_0809();
                    break;
                // ENCA1
                case PIC1A_AgentController::emENCAT1IOV:
                case PIC1A_AgentController::emENCAT1INT0:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_24();
                    break;
                case PIC1A_AgentController::emENCAT1IEC:
                    if ((value == false) && (mDT_Input[PIC1A_AgentController::emDT10] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcDT((unsigned int)PIC1A_AgentController::emDT10, false);
                    }
                    mDT_Input[PIC1A_AgentController::emDT10] = value;
                    break;
                case PIC1A_AgentController::emENCAT1EQ0:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG32_1011();
                    if ((value == false) && (mDT_Input[PIC1A_AgentController::emDT11] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcDT((unsigned int)PIC1A_AgentController::emDT11, false);
                    }
                    mDT_Input[PIC1A_AgentController::emDT11] = value;
                    break;
                case PIC1A_AgentController::emENCAT1EQ1:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG32_1011();
                    if ((value == false) && (mDT_Input[PIC1A_AgentController::emDT12] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcDT((unsigned int)PIC1A_AgentController::emDT12, false);
                    }
                    mDT_Input[PIC1A_AgentController::emDT12] = value;
                    break;
                case PIC1A_AgentController::emENCAT1INT1:
                case PIC1A_AgentController::emENCAT3INT1:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_10(0);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_10(1);
                    break;
                // ENCA2
                case PIC1A_AgentController::emENCAT2IOV:
                case PIC1A_AgentController::emENCAT2INT0:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG32_05();
                    break;
                // ENCA3
                case PIC1A_AgentController::emENCAT3IOV:
                case PIC1A_AgentController::emENCAT3INT0:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG32_13();
                    break;
                // TAUD0
                case PIC1A_AgentController::emTAUD0CDEN2:
                    if ((value == false) && (mRS_SetInput[PIC1A_AgentController::emRS01] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS01, true);
                    }
                    mRS_SetInput[PIC1A_AgentController::emRS01] = value;
                    break;
                case PIC1A_AgentController::emTAUD0CDEN3:
                    if ((value == false) && (mRS_ResetInput[PIC1A_AgentController::emRS01] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS01, false);
                    }
                    mRS_ResetInput[PIC1A_AgentController::emRS01] = value;
                    break;
                case PIC1A_AgentController::emTAUD0CDEN4:
                    if ((value == false) && (mRS_SetInput[PIC1A_AgentController::emRS02] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS02, true);
                    }
                    mRS_SetInput[PIC1A_AgentController::emRS02] = value;
                    break;
                case PIC1A_AgentController::emTAUD0CDEN5:
                    if ((value == false) && (mRS_ResetInput[PIC1A_AgentController::emRS02] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS02, false);
                    }
                    mRS_ResetInput[PIC1A_AgentController::emRS02] = value;
                    break;
                case PIC1A_AgentController::emTAUD0CDEN6:
                    if ((value == false) && (mRS_SetInput[PIC1A_AgentController::emRS03] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS03, true);
                    }
                    mRS_SetInput[PIC1A_AgentController::emRS03] = value;
                    break;
                case PIC1A_AgentController::emTAUD0CDEN7:
                    if ((value == false) && (mRS_ResetInput[PIC1A_AgentController::emRS03] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS03, false);
                    }
                    mRS_ResetInput[PIC1A_AgentController::emRS03] = value;
                    break;
                case PIC1A_AgentController::emTAUD0CDEN8:
                    if ((value == false) && (mRS_SetInput[PIC1A_AgentController::emRS04] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS04, true);
                    }
                    mRS_SetInput[PIC1A_AgentController::emRS04] = value;
                    break;
                case PIC1A_AgentController::emTAUD0CDEN9:
                    if ((value == false) && (mRS_ResetInput[PIC1A_AgentController::emRS04] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS04, false);
                    }
                    mRS_ResetInput[PIC1A_AgentController::emRS04] = value;
                    break;
                case PIC1A_AgentController::emTAUD0CDEN10:
                    if ((value == false) && (mRS_SetInput[PIC1A_AgentController::emRS05] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS05, true);
                    }
                    mRS_SetInput[PIC1A_AgentController::emRS05] = value;
                    break;
                case PIC1A_AgentController::emTAUD0CDEN11:
                    if ((value == false) && (mRS_ResetInput[PIC1A_AgentController::emRS05] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS05, false);
                    }
                    mRS_ResetInput[PIC1A_AgentController::emRS05] = value;
                    break;
                case PIC1A_AgentController::emTAUD0CDEN12:
                    if ((value == false) && (mRS_SetInput[PIC1A_AgentController::emRS06] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS06, true);
                    }
                    mRS_SetInput[PIC1A_AgentController::emRS06] = value;
                    break;
                case PIC1A_AgentController::emTAUD0CDEN13:
                    if ((value == false) && (mRS_ResetInput[PIC1A_AgentController::emRS06] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS06, false);
                    }
                    mRS_ResetInput[PIC1A_AgentController::emRS06] = value;
                    break;
                case PIC1A_AgentController::emTAUD0CDEN14:
                    if ((value == false) && (mRS_SetInput[PIC1A_AgentController::emRS07] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS07, true);
                    }
                    mRS_SetInput[PIC1A_AgentController::emRS07] = value;
                    break;
                case PIC1A_AgentController::emTAUD0CDEN15:
                    if ((value == false) && (mRS_ResetInput[PIC1A_AgentController::emRS07] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS07, false);
                    }
                    mRS_ResetInput[PIC1A_AgentController::emRS07] = value;
                    break;
                case PIC1A_AgentController::emTAUD0TOUT2:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG200_18();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG201_0001();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG201_0405();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG201_0809();
                    mPIC_Ptr->mPIC1A_Func->CalcPFN(0, PIC1A_AgentController::emPFN01, PIC1A_AgentController::emPFNTOUT2Input);
                    mPIC_Ptr->mPIC1A_Func->CalcPFN(0, PIC1A_AgentController::emPFN23, PIC1A_AgentController::emPFNTOUT2Input);
                    mPIC_Ptr->mPIC1A_Func->CalcPFN(0, PIC1A_AgentController::emPFN45, PIC1A_AgentController::emPFNTOUT2Input);
                    break;
                case PIC1A_AgentController::emTAUD0TOUT3:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG50_0102();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG201_0203();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG201_0607();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG201_1011();
                    break;
                case PIC1A_AgentController::emTAUD0TOUT4:
                    mPIC_Ptr->mPIC1A_Func->UpdateFNInput(0, PIC1A_AgentController::emFN0, false, value);
                    mPIC_Ptr->mPIC1A_Func->UpdateREG514_FFInput(0, PIC1A_AgentController::emFFU);
                    mPIC_Ptr->mPIC1A_Func->UpdateREG514_FFInput(0, PIC1A_AgentController::emFFUB);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG201_0001();
                    break;
                case PIC1A_AgentController::emTAUD0TOUT5:
                    mPIC_Ptr->mPIC1A_Func->UpdateREG514_FFInput(0, PIC1A_AgentController::emFFV);
                    mPIC_Ptr->mPIC1A_Func->UpdateREG514_FFInput(0, PIC1A_AgentController::emFFVB);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG202_02();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG201_0203();
                    break;
                case PIC1A_AgentController::emTAUD0TOUT6:
                    mPIC_Ptr->mPIC1A_Func->UpdateFNInput(0, PIC1A_AgentController::emFN2, false, value);
                    mPIC_Ptr->mPIC1A_Func->UpdateREG514_FFInput(0, PIC1A_AgentController::emFFW);
                    mPIC_Ptr->mPIC1A_Func->UpdateREG514_FFInput(0, PIC1A_AgentController::emFFWB);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG201_0405();
                    break;
                case PIC1A_AgentController::emTAUD0TOUT7:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG202_03();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG201_0607();
                    break;
                case PIC1A_AgentController::emTAUD0TOUT8:
                    mPIC_Ptr->mPIC1A_Func->UpdateFNInput(0, PIC1A_AgentController::emFN4, false, value);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG201_0809();
                    break;
                case PIC1A_AgentController::emTAUD0TOUT9:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG202_04();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG201_1011();
                    break;
                case PIC1A_AgentController::emTAUD0TOUT10:
                    mPIC_Ptr->mPIC1A_Func->ProcessWNG(0, PIC1A_AgentController::emWNG0, PIC1A_AgentController::emWNGfInput);
                    mPIC_Ptr->mPIC1A_Func->UpdatePFN_In1Input (0, PIC1A_AgentController::emPFN01, value);
                    break;
                case PIC1A_AgentController::emTAUD0TOUT11:
                    mPIC_Ptr->mPIC1A_Func->ProcessWNG(0, PIC1A_AgentController::emWNG0, PIC1A_AgentController::emWNGhInput);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG202_05();
                    break;
                case PIC1A_AgentController::emTAUD0TOUT12:
                    mPIC_Ptr->mPIC1A_Func->ProcessWNG(0, PIC1A_AgentController::emWNG1, PIC1A_AgentController::emWNGfInput);
                    mPIC_Ptr->mPIC1A_Func->UpdatePFN_In1Input (0, PIC1A_AgentController::emPFN23, value);
                    break;
                case PIC1A_AgentController::emTAUD0TOUT13:
                    mPIC_Ptr->mPIC1A_Func->ProcessWNG(0, PIC1A_AgentController::emWNG1, PIC1A_AgentController::emWNGhInput);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG202_06();
                    break;
                case PIC1A_AgentController::emTAUD0TOUT14:
                    mPIC_Ptr->mPIC1A_Func->ProcessWNG(0, PIC1A_AgentController::emWNG2, PIC1A_AgentController::emWNGfInput);
                    mPIC_Ptr->mPIC1A_Func->UpdatePFN_In1Input (0, PIC1A_AgentController::emPFN45, value);
                    break;
                case PIC1A_AgentController::emTAUD0TOUT15:
                    mPIC_Ptr->mPIC1A_Func->ProcessWNG(0, PIC1A_AgentController::emWNG2, PIC1A_AgentController::emWNGhInput);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG202_07();
                    break;
                case PIC1A_AgentController::emTAUD0UDC0:
                case PIC1A_AgentController::emTAUD0UDC8:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG200_2425(PIC1A_AgentController::emTAPATUDCM0);
                    break;
                case PIC1A_AgentController::emTAUD0UDC2:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG200_2425(PIC1A_AgentController::emTAPATUDCM0);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG200_18();
                    break;
                case PIC1A_AgentController::emTAUD0INT0:
                case PIC1A_AgentController::emTAUD0INT1:
                case PIC1A_AgentController::emTAUD0INT2:
                case PIC1A_AgentController::emTAUD0INT3:
                case PIC1A_AgentController::emTAUD0INT4:
                case PIC1A_AgentController::emTAUD0INT5:
                case PIC1A_AgentController::emTAUD0INT6:
                case PIC1A_AgentController::emTAUD0INT7:
                case PIC1A_AgentController::emTAUD0INT8:
                case PIC1A_AgentController::emTAUD0INT9:
                case PIC1A_AgentController::emTAUD0INT10:
                case PIC1A_AgentController::emTAUD0INT11:
                case PIC1A_AgentController::emTAUD0INT12:
                case PIC1A_AgentController::emTAUD0INT13:
                case PIC1A_AgentController::emTAUD0INT14:
                case PIC1A_AgentController::emTAUD0INT15:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0ENCSEL40_0003(0);
                    if ((PIC1A_AgentController::emTAUD0INT4 <= index) && (index <= PIC1A_AgentController::emTAUD0INT9)) {
                        mPIC_Ptr->mPIC1A_Func->CalcTRG(0);
                    }
                    if ((index == PIC1A_AgentController::emTAUD0INT0) || (index == PIC1A_AgentController::emTAUD0INT2)) {
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG200_2425(PIC1A_AgentController::emTAPATSIM0);
                    } else if (index == PIC1A_AgentController::emTAUD0INT1) {
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG20_17(0);
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG31_0910();
                    } else if (index == PIC1A_AgentController::emTAUD0INT4) {
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_03(0, PIC1A_AgentController::emUS);
                    } else if (index == PIC1A_AgentController::emTAUD0INT5) {
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_0506(0);
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_03(0, PIC1A_AgentController::emUC);
                    } else if (index == PIC1A_AgentController::emTAUD0INT6) {
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_03(0, PIC1A_AgentController::emVS);
                    } else if (index == PIC1A_AgentController::emTAUD0INT7) {
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_07(0);
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_03(0, PIC1A_AgentController::emVC);
                    } else if (index == PIC1A_AgentController::emTAUD0INT8) {
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG200_2425(PIC1A_AgentController::emTAPATSIM0);
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_03(0, PIC1A_AgentController::emWS);
                    } else if (index == PIC1A_AgentController::emTAUD0INT9) {
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_03(0, PIC1A_AgentController::emWC);
                    }
                    break;
                case PIC1A_AgentController::emTAUD0CKEN10:
                    if ((value == false) && (mWNG_CKENInput[0][PIC1A_AgentController::emWNG0] == true)) {
                        mPIC_Ptr->mPIC1A_Func->ProcessWNG(0, (unsigned int)PIC1A_AgentController::emWNG0, PIC1A_AgentController::emWNGTrigger);
                    }
                    mWNG_CKENInput[0][PIC1A_AgentController::emWNG0] = value;
                    break;
                case PIC1A_AgentController::emTAUD0CKEN12:
                    if ((value == false) && (mWNG_CKENInput[0][PIC1A_AgentController::emWNG1] == true)) {
                        mPIC_Ptr->mPIC1A_Func->ProcessWNG(0, (unsigned int)PIC1A_AgentController::emWNG1, PIC1A_AgentController::emWNGTrigger);
                    }
                    mWNG_CKENInput[0][PIC1A_AgentController::emWNG1] = value;
                    break;
                case PIC1A_AgentController::emTAUD0CKEN14:
                    if ((value == false) && (mWNG_CKENInput[0][PIC1A_AgentController::emWNG2] == true)) {
                        mPIC_Ptr->mPIC1A_Func->ProcessWNG(0, (unsigned int)PIC1A_AgentController::emWNG2, PIC1A_AgentController::emWNGTrigger);
                    }
                    mWNG_CKENInput[0][PIC1A_AgentController::emWNG2] = value;
                    break;
                // TAUD1
                case PIC1A_AgentController::emTAUD1CDEN2:
                    if ((value == false) && (mRS_SetInput[PIC1A_AgentController::emRS11] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS11, true);
                    }
                    mRS_SetInput[PIC1A_AgentController::emRS11] = value;
                    break;
                case PIC1A_AgentController::emTAUD1CDEN3:
                    if ((value == false) && (mRS_ResetInput[PIC1A_AgentController::emRS11] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS11, false);
                    }
                    mRS_ResetInput[PIC1A_AgentController::emRS11] = value;
                    break;
                case PIC1A_AgentController::emTAUD1CDEN4:
                    if ((value == false) && (mRS_SetInput[PIC1A_AgentController::emRS12] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS12, true);
                    }
                    mRS_SetInput[PIC1A_AgentController::emRS12] = value;
                    break;
                case PIC1A_AgentController::emTAUD1CDEN5:
                    if ((value == false) && (mRS_ResetInput[PIC1A_AgentController::emRS12] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS12, false);
                    }
                    mRS_ResetInput[PIC1A_AgentController::emRS12] = value;
                    break;
                case PIC1A_AgentController::emTAUD1CDEN6:
                    if ((value == false) && (mRS_SetInput[PIC1A_AgentController::emRS13] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS13, true);
                    }
                    mRS_SetInput[PIC1A_AgentController::emRS13] = value;
                    break;
                case PIC1A_AgentController::emTAUD1CDEN7:
                    if ((value == false) && (mRS_ResetInput[PIC1A_AgentController::emRS13] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS13, false);
                    }
                    mRS_ResetInput[PIC1A_AgentController::emRS13] = value;
                    break;
                case PIC1A_AgentController::emTAUD1CDEN8:
                    if ((value == false) && (mRS_SetInput[PIC1A_AgentController::emRS14] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS14, true);
                    }
                    mRS_SetInput[PIC1A_AgentController::emRS14] = value;
                    break;
                case PIC1A_AgentController::emTAUD1CDEN9:
                    if ((value == false) && (mRS_ResetInput[PIC1A_AgentController::emRS14] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS14, false);
                    }
                    mRS_ResetInput[PIC1A_AgentController::emRS14] = value;
                    break;
                case PIC1A_AgentController::emTAUD1CDEN10:
                    if ((value == false) && (mRS_SetInput[PIC1A_AgentController::emRS15] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS15, true);
                    }
                    mRS_SetInput[PIC1A_AgentController::emRS15] = value;
                    break;
                case PIC1A_AgentController::emTAUD1CDEN11:
                    if ((value == false) && (mRS_ResetInput[PIC1A_AgentController::emRS15] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS15, false);
                    }
                    mRS_ResetInput[PIC1A_AgentController::emRS15] = value;
                    break;
                case PIC1A_AgentController::emTAUD1CDEN12:
                    if ((value == false) && (mRS_SetInput[PIC1A_AgentController::emRS16] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS16, true);
                    }
                    mRS_SetInput[PIC1A_AgentController::emRS16] = value;
                    break;
                case PIC1A_AgentController::emTAUD1CDEN13:
                    if ((value == false) && (mRS_ResetInput[PIC1A_AgentController::emRS16] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS16, false);
                    }
                    mRS_ResetInput[PIC1A_AgentController::emRS16] = value;
                    break;
                case PIC1A_AgentController::emTAUD1CDEN14:
                    if ((value == false) && (mRS_SetInput[PIC1A_AgentController::emRS17] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS17, true);
                    }
                    mRS_SetInput[PIC1A_AgentController::emRS17] = value;
                    break;
                case PIC1A_AgentController::emTAUD1CDEN15:
                    if ((value == false) && (mRS_ResetInput[PIC1A_AgentController::emRS17] == true)) {
                        mPIC_Ptr->mPIC1A_Func->CalcRS((unsigned int)PIC1A_AgentController::emRS17, false);
                    }
                    mRS_ResetInput[PIC1A_AgentController::emRS17] = value;
                    break;
                case PIC1A_AgentController::emTAUD1TOUT2:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG210_18();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG211_0001();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG211_0405();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG211_0809();
                    mPIC_Ptr->mPIC1A_Func->CalcPFN(1, PIC1A_AgentController::emPFN01, PIC1A_AgentController::emPFNTOUT2Input);
                    mPIC_Ptr->mPIC1A_Func->CalcPFN(1, PIC1A_AgentController::emPFN23, PIC1A_AgentController::emPFNTOUT2Input);
                    mPIC_Ptr->mPIC1A_Func->CalcPFN(1, PIC1A_AgentController::emPFN45, PIC1A_AgentController::emPFNTOUT2Input);
                    break;
                case PIC1A_AgentController::emTAUD1TOUT3:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG51_0102();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG211_0203();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG211_0607();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG211_1011();
                    break;
                case PIC1A_AgentController::emTAUD1TOUT4:
                    mPIC_Ptr->mPIC1A_Func->UpdateFNInput(1, PIC1A_AgentController::emFN0, false, value);
                    mPIC_Ptr->mPIC1A_Func->UpdateREG514_FFInput(1, PIC1A_AgentController::emFFU);
                    mPIC_Ptr->mPIC1A_Func->UpdateREG514_FFInput(1, PIC1A_AgentController::emFFUB);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG211_0001();
                    break;
                case PIC1A_AgentController::emTAUD1TOUT5:
                    mPIC_Ptr->mPIC1A_Func->UpdateREG514_FFInput(1, PIC1A_AgentController::emFFV);
                    mPIC_Ptr->mPIC1A_Func->UpdateREG514_FFInput(1, PIC1A_AgentController::emFFVB);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG212_02();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG211_0203();
                    break;
                case PIC1A_AgentController::emTAUD1TOUT6:
                    mPIC_Ptr->mPIC1A_Func->UpdateFNInput(1, PIC1A_AgentController::emFN2, false, value);
                    mPIC_Ptr->mPIC1A_Func->UpdateREG514_FFInput(1, PIC1A_AgentController::emFFW);
                    mPIC_Ptr->mPIC1A_Func->UpdateREG514_FFInput(1, PIC1A_AgentController::emFFWB);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG211_0405();
                    break;
                case PIC1A_AgentController::emTAUD1TOUT7:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG212_03();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG211_0607();
                    break;
                case PIC1A_AgentController::emTAUD1TOUT8:
                    mPIC_Ptr->mPIC1A_Func->UpdateFNInput(1, PIC1A_AgentController::emFN4, false, value);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG211_0809();
                    break;
                case PIC1A_AgentController::emTAUD1TOUT9:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG212_04();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG211_1011();
                    break;
                case PIC1A_AgentController::emTAUD1TOUT10:
                    mPIC_Ptr->mPIC1A_Func->ProcessWNG(1, PIC1A_AgentController::emWNG0, PIC1A_AgentController::emWNGfInput);
                    mPIC_Ptr->mPIC1A_Func->UpdatePFN_In1Input (1, PIC1A_AgentController::emPFN01, value);
                    break;
                case PIC1A_AgentController::emTAUD1TOUT11:
                    mPIC_Ptr->mPIC1A_Func->ProcessWNG(1, PIC1A_AgentController::emWNG0, PIC1A_AgentController::emWNGhInput);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG212_05();
                    break;
                case PIC1A_AgentController::emTAUD1TOUT12:
                    mPIC_Ptr->mPIC1A_Func->ProcessWNG(1, PIC1A_AgentController::emWNG1, PIC1A_AgentController::emWNGfInput);
                    mPIC_Ptr->mPIC1A_Func->UpdatePFN_In1Input (1, PIC1A_AgentController::emPFN23, value);
                    break;
                case PIC1A_AgentController::emTAUD1TOUT13:
                    mPIC_Ptr->mPIC1A_Func->ProcessWNG(1, PIC1A_AgentController::emWNG1, PIC1A_AgentController::emWNGhInput);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG212_06();
                    break;
                case PIC1A_AgentController::emTAUD1TOUT14:
                    mPIC_Ptr->mPIC1A_Func->ProcessWNG(1, PIC1A_AgentController::emWNG2, PIC1A_AgentController::emWNGfInput);
                    mPIC_Ptr->mPIC1A_Func->UpdatePFN_In1Input (1, PIC1A_AgentController::emPFN45, value);
                    break;
                case PIC1A_AgentController::emTAUD1TOUT15:
                    mPIC_Ptr->mPIC1A_Func->ProcessWNG(1, PIC1A_AgentController::emWNG2, PIC1A_AgentController::emWNGhInput);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG212_07();
                case PIC1A_AgentController::emTAUD1UDC0:
                case PIC1A_AgentController::emTAUD1UDC8:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG210_2425(PIC1A_AgentController::emTAPATUDCM0);
                    break;
                case PIC1A_AgentController::emTAUD1UDC2:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG210_2425(PIC1A_AgentController::emTAPATUDCM0);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG210_18();
                    break;
                case PIC1A_AgentController::emTAUD1INT0:
                case PIC1A_AgentController::emTAUD1INT1:
                case PIC1A_AgentController::emTAUD1INT2:
                case PIC1A_AgentController::emTAUD1INT3:
                case PIC1A_AgentController::emTAUD1INT4:
                case PIC1A_AgentController::emTAUD1INT5:
                case PIC1A_AgentController::emTAUD1INT6:
                case PIC1A_AgentController::emTAUD1INT7:
                case PIC1A_AgentController::emTAUD1INT8:
                case PIC1A_AgentController::emTAUD1INT9:
                case PIC1A_AgentController::emTAUD1INT10:
                case PIC1A_AgentController::emTAUD1INT11:
                case PIC1A_AgentController::emTAUD1INT12:
                case PIC1A_AgentController::emTAUD1INT13:
                case PIC1A_AgentController::emTAUD1INT14:
                case PIC1A_AgentController::emTAUD1INT15:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0ENCSEL40_0003(1);
                    if ((PIC1A_AgentController::emTAUD1INT4 <= index) && (index <= PIC1A_AgentController::emTAUD1INT9)) {
                        mPIC_Ptr->mPIC1A_Func->CalcTRG(1);
                    }
                    if ((index == PIC1A_AgentController::emTAUD1INT0) || (index == PIC1A_AgentController::emTAUD1INT2)) {
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG210_2425(PIC1A_AgentController::emTAPATSIM0);
                    } else if (index == PIC1A_AgentController::emTAUD1INT1) {
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG20_17(1);
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG31_1819();
                    } else if (index == PIC1A_AgentController::emTAUD1INT4) {
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_03(1, PIC1A_AgentController::emUS);
                    } else if (index == PIC1A_AgentController::emTAUD1INT5) {
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_0506(1);
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_03(1, PIC1A_AgentController::emUC);
                    } else if (index == PIC1A_AgentController::emTAUD1INT6) {
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_03(1, PIC1A_AgentController::emVS);
                    } else if (index == PIC1A_AgentController::emTAUD1INT7) {
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_07(1);
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_03(1, PIC1A_AgentController::emVC);
                    } else if (index == PIC1A_AgentController::emTAUD1INT8) {
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG210_2425(PIC1A_AgentController::emTAPATSIM0);
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_03(1, PIC1A_AgentController::emWS);
                    } else if (index == PIC1A_AgentController::emTAUD1INT9) {
                        mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_03(1, PIC1A_AgentController::emWC);
                    }
                    break;
                case PIC1A_AgentController::emTAUD1CKEN10:
                    if ((value == false) && (mWNG_CKENInput[1][PIC1A_AgentController::emWNG0] == true)) {
                        mPIC_Ptr->mPIC1A_Func->ProcessWNG(1, (unsigned int)PIC1A_AgentController::emWNG0, PIC1A_AgentController::emWNGTrigger);
                    }
                    mWNG_CKENInput[1][PIC1A_AgentController::emWNG0] = value;
                    break;
                case PIC1A_AgentController::emTAUD1CKEN12:
                    if ((value == false) && (mWNG_CKENInput[1][PIC1A_AgentController::emWNG1] == true)) {
                        mPIC_Ptr->mPIC1A_Func->ProcessWNG(1, (unsigned int)PIC1A_AgentController::emWNG1, PIC1A_AgentController::emWNGTrigger);
                    }
                    mWNG_CKENInput[1][PIC1A_AgentController::emWNG1] = value;
                    break;
                case PIC1A_AgentController::emTAUD1CKEN14:
                    if ((value == false) && (mWNG_CKENInput[1][PIC1A_AgentController::emWNG2] == true)) {
                        mPIC_Ptr->mPIC1A_Func->ProcessWNG(1, (unsigned int)PIC1A_AgentController::emWNG2, PIC1A_AgentController::emWNGTrigger);
                    }
                    mWNG_CKENInput[1][PIC1A_AgentController::emWNG2] = value;
                    break;
                // TAUJn , DNF
                case PIC1A_AgentController::emTAUJ0TOUT3:
                case PIC1A_AgentController::emDNFTSG0TSTCKE:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG50_0102();
                    break;
                case PIC1A_AgentController::emTAUJ1TOUT3:
                case PIC1A_AgentController::emDNFTSG1TSTCKE:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG51_0102();
                    break;
                // TPBA0
                case PIC1A_AgentController::emTPB0INTPAT:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG33(0);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG33(1);
                    break;
                // TSG0
                case PIC1A_AgentController::emTSG0TSTPTE:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG200_0809();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG200_1011();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG200_1213();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG31_0608();
                    break;
                case PIC1A_AgentController::emTSG0TS2PEC:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_00();
                    break;
                case PIC1A_AgentController::emTSG0TST2PUD:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_01();
                    break;
                case PIC1A_AgentController::emTSG0TO1:
                    mPIC_Ptr->mPIC1A_Func->UpdateREG513_FFInput(0, PIC1A_AgentController::emFFU);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_13(0, PIC1A_AgentController::emFFU);
                    break;
                case PIC1A_AgentController::emTSG0TO2:
                    mPIC_Ptr->mPIC1A_Func->UpdateREG513_FFInput(0, PIC1A_AgentController::emFFUB);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_13(0, PIC1A_AgentController::emFFUB);
                    break;
                case PIC1A_AgentController::emTSG0TO3:
                    mPIC_Ptr->mPIC1A_Func->UpdateREG513_FFInput(0, PIC1A_AgentController::emFFV);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_13(0, PIC1A_AgentController::emFFV);
                    break;
                case PIC1A_AgentController::emTSG0TO4:
                    mPIC_Ptr->mPIC1A_Func->UpdateREG513_FFInput(0, PIC1A_AgentController::emFFVB);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_13(0, PIC1A_AgentController::emFFVB);
                    break;
                case PIC1A_AgentController::emTSG0TO5:
                    mPIC_Ptr->mPIC1A_Func->UpdateREG513_FFInput(0, PIC1A_AgentController::emFFW);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_13(0, PIC1A_AgentController::emFFW);
                    break;
                case PIC1A_AgentController::emTSG0TO6:
                    mPIC_Ptr->mPIC1A_Func->UpdateREG513_FFInput(0, PIC1A_AgentController::emFFWB);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_13(0, PIC1A_AgentController::emFFWB);
                    break;
                case PIC1A_AgentController::emTSG0INT0:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_12(0, PIC1A_AgentController::emI4);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_12(0, PIC1A_AgentController::emI5);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_12(0, PIC1A_AgentController::emI6);
                    break;
                case PIC1A_AgentController::emTSG0INT2:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_12(0, PIC1A_AgentController::emI4);
                    break;
                case PIC1A_AgentController::emTSG0INT6:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_12(0, PIC1A_AgentController::emI5);
                    break;
                case PIC1A_AgentController::emTSG0INT10:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_12(0, PIC1A_AgentController::emI6);
                    break;
                // TSG1
                case PIC1A_AgentController::emTSG1TSTPTE:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG210_0809();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG210_1011();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG210_1213();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG31_1517();
                    break;
                case PIC1A_AgentController::emTSG1TS2PEC:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_0607();
                    break;
                case PIC1A_AgentController::emTSG1TO1:
                    mPIC_Ptr->mPIC1A_Func->UpdateREG513_FFInput(1, PIC1A_AgentController::emFFU);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_13(1, PIC1A_AgentController::emFFU);
                    break;
                case PIC1A_AgentController::emTSG1TO2:
                    mPIC_Ptr->mPIC1A_Func->UpdateREG513_FFInput(1, PIC1A_AgentController::emFFUB);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_13(1, PIC1A_AgentController::emFFUB);
                    break;
                case PIC1A_AgentController::emTSG1TO3:
                    mPIC_Ptr->mPIC1A_Func->UpdateREG513_FFInput(1, PIC1A_AgentController::emFFV);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_13(1, PIC1A_AgentController::emFFV);
                    break;
                case PIC1A_AgentController::emTSG1TO4:
                    mPIC_Ptr->mPIC1A_Func->UpdateREG513_FFInput(1, PIC1A_AgentController::emFFVB);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_13(1, PIC1A_AgentController::emFFVB);
                    break;
                case PIC1A_AgentController::emTSG1TO5:
                    mPIC_Ptr->mPIC1A_Func->UpdateREG513_FFInput(1, PIC1A_AgentController::emFFW);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_13(1, PIC1A_AgentController::emFFW);
                    break;
                case PIC1A_AgentController::emTSG1TO6:
                    mPIC_Ptr->mPIC1A_Func->UpdateREG513_FFInput(1, PIC1A_AgentController::emFFWB);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_13(1, PIC1A_AgentController::emFFWB);
                    break;
                case PIC1A_AgentController::emTSG1INT0:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_12(1, PIC1A_AgentController::emI4);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_12(1, PIC1A_AgentController::emI5);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_12(1, PIC1A_AgentController::emI6);
                    break;
                case PIC1A_AgentController::emTSG1INT2:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_12(1, PIC1A_AgentController::emI4);
                    break;
                case PIC1A_AgentController::emTSG1INT6:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_12(1, PIC1A_AgentController::emI5);
                    break;
                case PIC1A_AgentController::emTSG1INT10:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_12(1, PIC1A_AgentController::emI6);
                    break;
                // TSGn, HiZ, ECM
                case PIC1A_AgentController::emRSVHIZIN0:
                case PIC1A_AgentController::emRSVHIZIN1:
                case PIC1A_AgentController::emRSVHIZIN2:
                case PIC1A_AgentController::emRSVHIZIN3:
                case PIC1A_AgentController::emRSVHIZIN4:
                case PIC1A_AgentController::emRSVHIZIN5:
                case PIC1A_AgentController::emTSG0TSTIER:
                case PIC1A_AgentController::emTSG1TSTIER:
                case PIC1A_AgentController::emERROROUTZ:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0HIZCEN(0);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0HIZCEN(1);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0HIZCEN(2);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0HIZCEN(3);
                    break;
                // PIC2
                case PIC1A_AgentController::emADCATTOUT00:
                case PIC1A_AgentController::emADCATTOUT01:
                case PIC1A_AgentController::emADCATTOUT02:
                case PIC1A_AgentController::emADCATTOUT03:
                case PIC1A_AgentController::emADCATTOUT04:
                case PIC1A_AgentController::emADCATTOUT10:
                case PIC1A_AgentController::emADCATTOUT11:
                case PIC1A_AgentController::emADCATTOUT12:
                case PIC1A_AgentController::emADCATTOUT13:
                case PIC1A_AgentController::emADCATTOUT14:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_0205();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_1215();
                    break;
                // DNF
                case PIC1A_AgentController::emDNFTAUD0TIN0:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG31_0608();
                    break;
                case PIC1A_AgentController::emDNFTAUD0TIN1:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG31_11();
                    break;
                case PIC1A_AgentController::emDNFTAUD0TIN2:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG31_1213();
                    break;
                case PIC1A_AgentController::emDNFTAUD0TIN3:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG31_14();
                    break;
                case PIC1A_AgentController::emDNFTAUD0TIN4:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG20_00(0);
                    break;
                case PIC1A_AgentController::emDNFTAUD0TIN5:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG20_01(0);
                    break;
                case PIC1A_AgentController::emDNFTAUD0TIN6:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG20_02(0);
                    break;
                case PIC1A_AgentController::emDNFTAUD0TIN7:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG20_03(0);
                    break;
                case PIC1A_AgentController::emDNFTAUD0TIN8:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG200_04();
                    break;
                case PIC1A_AgentController::emDNFTAUD0TIN9:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG20_05(0);
                    break;
                case PIC1A_AgentController::emDNFTAUD0TIN10:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG22_1617(0);
                case PIC1A_AgentController::emDNFTAUD0TIN11:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG22_1819(0);
                    break;
                case PIC1A_AgentController::emDNFTAUD0TIN12:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG22_2021(0);
                    break;
                case PIC1A_AgentController::emDNFTAUD0TIN13:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG22_2223(0);
                    break;
                case PIC1A_AgentController::emDNFTAUD0TIN14:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG22_2425(0);
                    break;
                case PIC1A_AgentController::emDNFTAUD0TIN15:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG22_2627(0);
                    break;
                case PIC1A_AgentController::emDNFTAUD1TIN0:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG31_1517();
                    break;
                case PIC1A_AgentController::emDNFTAUD1TIN1:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG31_20();
                    break;
                case PIC1A_AgentController::emDNFTAUD1TIN2:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG31_2122();
                    break;
                case PIC1A_AgentController::emDNFTAUD1TIN3:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG31_23();
                    break;
                case PIC1A_AgentController::emDNFTAUD1TIN4:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG20_00(1);
                    break;
                case PIC1A_AgentController::emDNFTAUD1TIN5:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG20_01(1);
                    break;
                case PIC1A_AgentController::emDNFTAUD1TIN6:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG20_02(1);
                    break;
                case PIC1A_AgentController::emDNFTAUD1TIN7:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG20_03(1);
                    break;
                case PIC1A_AgentController::emDNFTAUD1TIN8:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG210_04();
                    break;
                case PIC1A_AgentController::emDNFTAUD1TIN9:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG20_05(1);
                    break;
                case PIC1A_AgentController::emDNFTAUD1TIN10:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG22_1617(1);
                    break;
                case PIC1A_AgentController::emDNFTAUD1TIN11:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG22_1819(1);
                    break;
                case PIC1A_AgentController::emDNFTAUD1TIN12:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG22_2021(1);
                    break;
                case PIC1A_AgentController::emDNFTAUD1TIN13:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG22_2223(1);
                    break;
                case PIC1A_AgentController::emDNFTAUD1TIN14:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG22_2425(1);
                    break;
                case PIC1A_AgentController::emDNFTAUD1TIN15:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG22_2627(1);
                    break;
                case PIC1A_AgentController::emDNFENCAT0AIN:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG200_0809();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_1617(PIC1A_AgentController::emG1);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_00(0, PIC1A_AgentController::emI1);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_00(1, PIC1A_AgentController::emI1);
                    break;
                case PIC1A_AgentController::emDNFENCAT0BIN:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG200_1011();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_1617(PIC1A_AgentController::emG2);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_00(0, PIC1A_AgentController::emI2);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_00(1, PIC1A_AgentController::emI2);
                    break;
                case PIC1A_AgentController::emDNFENCAT0TIN1:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_18();
                    break;
                case PIC1A_AgentController::emDNFENCAT0ZIN:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG200_1213();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_1617(PIC1A_AgentController::emG3);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_00(0, PIC1A_AgentController::emI3);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_00(1, PIC1A_AgentController::emI3);
                    break;
                case PIC1A_AgentController::emDNFENCAT1AIN:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG210_0809();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_1920(PIC1A_AgentController::emG1);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_00(0, PIC1A_AgentController::emI1);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_00(1, PIC1A_AgentController::emI1);
                    break;
                case PIC1A_AgentController::emDNFENCAT1BIN:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG210_1011();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_1920(PIC1A_AgentController::emG2);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_00(0, PIC1A_AgentController::emI2);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_00(1, PIC1A_AgentController::emI2);
                    break;
                case PIC1A_AgentController::emDNFENCAT1TIN1:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_21();
                    break;
                case PIC1A_AgentController::emDNFENCAT1ZIN:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG210_1213();
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_1920(PIC1A_AgentController::emG3);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_00(0, PIC1A_AgentController::emI3);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG5_00(1, PIC1A_AgentController::emI3);
                    break;
                case PIC1A_AgentController::emDNFTAUJ0TIN0:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG31_00();
                    break;
                case PIC1A_AgentController::emDNFTAUJ0TIN1:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG31_0102();
                    break;
                case PIC1A_AgentController::emDNFTAUJ0TIN2:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG31_03();
                    break;
                case PIC1A_AgentController::emDNFTAUJ0TIN3:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG31_0405();
                    break;
                case PIC1A_AgentController::emDNFSIF0IREF:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG33(0);
                    break;
                case PIC1A_AgentController::emDNFSIF1IREF:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG33(1);
                    break;
                case PIC1A_AgentController::emDNFENCAT2TIN1:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG32_04();
                    break;
                case PIC1A_AgentController::emDNFENCAT3TIN1:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG32_12();
                    break;
                case PIC1A_AgentController::emANFESO0:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0HIZCEN(0);
                    break;
                case PIC1A_AgentController::emANFESO1:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0HIZCEN(1);
                    break;
                case PIC1A_AgentController::emANFESO2:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0HIZCEN(2);
                    break;
                case PIC1A_AgentController::emANFESO3:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0HIZCEN(3);
                    break;
                // PORT(TSG0 Hall Sensor)
                case PIC1A_AgentController::emPORTTSG0TSTAPT0:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0HALLSEL(0, PIC1A_AgentController::emI1);
                    break;
                case PIC1A_AgentController::emPORTTSG0TSTAPT1:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0HALLSEL(0, PIC1A_AgentController::emI2);
                    break;
                case PIC1A_AgentController::emPORTTSG0TSTAPT2:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0HALLSEL(0, PIC1A_AgentController::emI3);
                    break;
                // PORT(TSG1 Hall Sensor)
                case PIC1A_AgentController::emPORTTSG1TSTAPT0:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0HALLSEL(1, PIC1A_AgentController::emI1);
                    break;
                case PIC1A_AgentController::emPORTTSG1TSTAPT1:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0HALLSEL(1, PIC1A_AgentController::emI2);
                    break;
                case PIC1A_AgentController::emPORTTSG1TSTAPT2:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0HALLSEL(1, PIC1A_AgentController::emI3);
                    break;
                // RDCn
                case PIC1A_AgentController::emSAT0AOUT:
                case PIC1A_AgentController::emSAT1AOUT:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_1617(PIC1A_AgentController::emG1);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_1920(PIC1A_AgentController::emG1);
                    break;
                case PIC1A_AgentController::emSAT0BOUT:
                case PIC1A_AgentController::emSAT1BOUT:
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_1617(PIC1A_AgentController::emG2);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_1920(PIC1A_AgentController::emG2);
                    break;
                case PIC1A_AgentController::emSAT0ZOUT:
                default:// PIC1A_AgentController::emSAT1ZOUT
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_1617(PIC1A_AgentController::emG3);
                    mPIC_Ptr->mPIC1A_Func->CalcMUX_PIC0REG30_1920(PIC1A_AgentController::emG3);
            }
        }
    }
}//}}}

/// Write value to output port
void PIC_Port::WriteOutputPortMethod (const unsigned int index)
{//{{{
    OutputPort[index]->write(mWriteValue[index]);
}//}}}

/// Write value to Sync output port
void PIC_Port::WriteSyncOutputPortMethod (const unsigned int index)
{//{{{
    SyncOutputPort[index]->write(mWriteSyncValue[index]);
}//}}}

/// Assert Sync Start outputs
void PIC_Port::AssertSyncOutputPortMethod (const unsigned int index)
{//{{{
    if (!mPIC_Ptr->GetResetStatus()){
        if (mPIC_Ptr->CheckClockPeriod("PCLK")) {
            mWriteSyncValue[index] = true;
            mWriteSyncOutputEvent[index].notify(SC_ZERO_TIME);
            mDeassertSyncOutputEvent[index].notify(mPIC_Ptr->mSync_chsb_clsbPeriod, mPIC_Ptr->mTimeResolutionUnit);
        }
    }
}//}}}

/// Deassert Sync Start outputs
void PIC_Port::DeassertSyncOutputPortMethod (const unsigned int index)
{//{{{
    if (!mPIC_Ptr->GetResetStatus()){
        if (mPIC_Ptr->CheckClockPeriod("PCLK")) {
            mWriteSyncValue[index] = false;
            mWriteSyncOutputEvent[index].notify(SC_ZERO_TIME);
        }
    }
}//}}}

/// Initialize internal variables
void PIC_Port::Initialize (void)
{//{{{
    for (unsigned int i = 0; i < PIC1A_AgentController::emOutputNum; i++) {
        mWriteValue[i] = false;
    }
    for (unsigned int i = 0; i < PIC1A_AgentController::emSyncOutputNum; i++) {
        mWriteSyncValue[i] = false;
    }
    for (unsigned int i = 0; i < PIC1A_AgentController::emDTnum; i++) {
        mDT_Input[i] = false;
    }
    for (unsigned int i = 0; i < PIC1A_AgentController::emRSnum; i++) {
        mRS_SetInput[i] = false;
        mRS_ResetInput[i] = false;
    }
    for (unsigned int i = 0; i < 2; i++) {
        for (unsigned int j = 0; j < PIC1A_AgentController::emWNGnum; j++) {
            mWNG_CKENInput[i][j] = false;
        }
    }
}//}}}


///Constructor of PIC class
PIC1A::PIC1A (sc_module_name name,
            unsigned int rLatency,
            unsigned int wLatency):
    sc_module(name),
    PIC1A_AgentController(),
    BusSlaveBase<32,1>(),
    PCLK("PCLK"),
    PRESETZ("PRESETZ"),
    sync_chsb_clsb("sync_chsb_clsb"),
    // Input ports
    // ENCA0
    ENCAT0IOV("ENCAT0IOV"),
    ENCAT0IEC("ENCAT0IEC"),
    ENCAT0INT0("ENCAT0INT0"),
    ENCAT0EQ0("ENCAT0EQ0"),
    ENCAT0ECS("ENCAT0ECS"),
    ENCAT0EQ1("ENCAT0EQ1"),
    ENCAT0INT1("ENCAT0INT1"),
    ENCAT0IUD("ENCAT0IUD"),
    // ENCA1
    ENCAT1IOV("ENCAT1IOV"),
    ENCAT1IEC("ENCAT1IEC"),
    ENCAT1INT0("ENCAT1INT0"),
    ENCAT1EQ0("ENCAT1EQ0"),
    ENCAT1ECS("ENCAT1ECS"),
    ENCAT1EQ1("ENCAT1EQ1"),
    ENCAT1INT1("ENCAT1INT1"),
    // ENCA2
    ENCAT2IOV("ENCAT2IOV"),
    ENCAT2INT0("ENCAT2INT0"),
    ENCAT2INT1("ENCAT2INT1"),
    // ENCA3
    ENCAT3IOV("ENCAT3IOV"),
    ENCAT3INT0("ENCAT3INT0"),
    ENCAT3INT1("ENCAT3INT1"),
    // TAUD0
    TAUD0UDC0("TAUD0UDC0"),
    TAUD0UDC2("TAUD0UDC2"),
    TAUD0UDC8("TAUD0UDC8"),
    TAUD0CKEN10("TAUD0CKEN10"),
    TAUD0CKEN12("TAUD0CKEN12"),
    TAUD0CKEN14("TAUD0CKEN14"),
    // TAUD0
    TAUD1UDC0("TAUD1UDC0"),
    TAUD1UDC2("TAUD1UDC2"),
    TAUD1UDC8("TAUD1UDC8"),
    TAUD1CKEN10("TAUD1CKEN10"),
    TAUD1CKEN12("TAUD1CKEN12"),
    TAUD1CKEN14("TAUD1CKEN14"),
    // TAUJ0
    TAUJ0TOUT3("TAUJ0TOUT3"),
    // TAUJ1
    TAUJ1TOUT3("TAUJ1TOUT3"),
    // TPBA0
    TPB0INTPAT("TPB0INTPAT"),
    // TSG0
    TSG0TSTPTE("TSG0TSTPTE"),
    TSG0TS2PEC("TSG0TS2PEC"),
    TSG0TST2PUD("TSG0TST2PUD"),
    TSG0TO1("TSG0TO1"),
    TSG0TO2("TSG0TO2"),
    TSG0TO3("TSG0TO3"),
    TSG0TO4("TSG0TO4"),
    TSG0TO5("TSG0TO5"),
    TSG0TO6("TSG0TO6"),
    TSG0INT0("TSG0INT0"),
    TSG0INT2("TSG0INT2"),
    TSG0INT6("TSG0INT6"),
    TSG0INT10("TSG0INT10"),
    TSG0TSTIER("TSG0TSTIER"),
    // TSG1
    TSG1TSTPTE("TSG1TSTPTE"),
    TSG1TS2PEC("TSG1TS2PEC"),
    TSG1TST2PUD("TSG1TST2PUD"),
    TSG1TO1("TSG1TO1"),
    TSG1TO2("TSG1TO2"),
    TSG1TO3("TSG1TO3"),
    TSG1TO4("TSG1TO4"),
    TSG1TO5("TSG1TO5"),
    TSG1TO6("TSG1TO6"),
    TSG1INT0("TSG1INT0"),
    TSG1INT2("TSG1INT2"),
    TSG1INT6("TSG1INT6"),
    TSG1INT10("TSG1INT10"),
    TSG1TSTIER("TSG1TSTIER"),
    // DNF
    DNFENCAT0AIN("DNFENCAT0AIN"),
    DNFENCAT0BIN("DNFENCAT0BIN"),
    DNFENCAT0TIN1("DNFENCAT0TIN1"),
    DNFENCAT0ZIN("DNFENCAT0ZIN"),
    DNFENCAT1AIN("DNFENCAT1AIN"),
    DNFENCAT1BIN("DNFENCAT1BIN"),
    DNFENCAT1TIN1("DNFENCAT1TIN1"),
    DNFENCAT1ZIN("DNFENCAT1ZIN"),
    DNFTSG0TSTCKE("DNFTSG0TSTCKE"),
    DNFTSG1TSTCKE("DNFTSG1TSTCKE"),
    DNFSIF0IREF("DNFSIF0IREF"),
    DNFSIF1IREF("DNFSIF1IREF"),
    DNFENCAT2TIN1("DNFENCAT2TIN1"),
    DNFENCAT3TIN1("DNFENCAT3TIN1"),
    // ECM
    ERROROUTZ("ERROROUTZ"),
    // RDC0
    SAT0AOUT("SAT0AOUT"),
    SAT0BOUT("SAT0BOUT"),
    SAT0ZOUT("SAT0ZOUT"),
    // RDC1
    SAT1AOUT("SAT1AOUT"),
    SAT1BOUT("SAT1BOUT"),
    SAT1ZOUT("SAT1ZOUT"),
    // Output ports
    // ENCA0
    ENCAT0TSST("ENCAT0TSST"),
    ENCAT0AIN("ENCAT0AIN"),
    ENCAT0BIN("ENCAT0BIN"),
    ENCAT0ZIN("ENCAT0ZIN"),
    ENCAT0TIN1("ENCAT0TIN1"),
    // ENCA1
    ENCAT1TSST("ENCAT1TSST"),
    ENCAT1AIN("ENCAT1AIN"),
    ENCAT1BIN("ENCAT1BIN"),
    ENCAT1ZIN("ENCAT1ZIN"),
    ENCAT1TIN1("ENCAT1TIN1"),
    // ENCA2
    ENCAT2TSST("ENCAT2TSST"),
    ENCAT2AIN("ENCAT2AIN"),
    ENCAT2BIN("ENCAT2BIN"),
    ENCAT2ZIN("ENCAT2ZIN"),
    ENCAT2TIN1("ENCAT2TIN1"),
    // ENCA3
    ENCAT3TSST("ENCAT3TSST"),
    ENCAT3AIN("ENCAT3AIN"),
    ENCAT3BIN("ENCAT3BIN"),
    ENCAT3ZIN("ENCAT3ZIN"),
    ENCAT3TIN1("ENCAT3TIN1"),
    // TAPA0
    TOP0TAPATHASIN("TOP0TAPATHASIN"),
    TOP0TAPATSIM0("TOP0TAPATSIM0"),
    TOP0TAPATUDCM0("TOP0TAPATUDCM0"),
    // TAPA1
    TOP1TAPATHASIN("TOP1TAPATHASIN"),
    TOP1TAPATSIM0("TOP1TAPATSIM0"),
    TOP1TAPATUDCM0("TOP1TAPATUDCM0"),
    // TAPA2
    TOP2TAPATHASIN("TOP2TAPATHASIN"),
    // TAPA3
    TOP3TAPATHASIN("TOP3TAPATHASIN"),
    // TPBA0
    TPB0TSST("TPB0TSST"),
    // TPBA1
    TPB1TSST("TPB1TSST"),
    // TSG0
    TSG0TSST("TSG0TSST"),
    TSG0TSTOPS("TSG0TSTOPS"),
    TSG0TSTOPC0("TSG0TSTOPC0"),
    TSG0TSTOPC1("TSG0TSTOPC1"),
    TSG0TSTCKE("TSG0TSTCKE"),
    TSG0TSTIUS("TSG0TSTIUS"),
    TSG0TSTIUC("TSG0TSTIUC"),
    TSG0TSTIVS("TSG0TSTIVS"),
    TSG0TSTIVC("TSG0TSTIVC"),
    TSG0TSTIWS("TSG0TSTIWS"),
    TSG0TSTIWC("TSG0TSTIWC"),
    // TSG1
    TSG1TSST("TSG1TSST"),
    TSG1TSTOPS("TSG1TSTOPS"),
    TSG1TSTOPC0("TSG1TSTOPC0"),
    TSG1TSTOPC1("TSG1TSTOPC1"),
    TSG1TSTCKE("TSG1TSTCKE"),
    TSG1TSTIUS("TSG1TSTIUS"),
    TSG1TSTIUC("TSG1TSTIUC"),
    TSG1TSTIVS("TSG1TSTIVS"),
    TSG1TSTIVC("TSG1TSTIVC"),
    TSG1TSTIWS("TSG1TSTIWS"),
    TSG1TSTIWC("TSG1TSTIWC"),
    // OSTM0
    OST0TSST("OST0TSST"),
    // OSTM1
    OST1TSST("OST1TSST"),
    // OSTM2
    OST2TSST("OST2TSST"),
    // OSTM3
    OST3TSST("OST3TSST"),
    // INTC
    INTENC0CM0("INTENC0CM0"),
    INTENC1CM0("INTENC1CM0"),
    INTENC2CM0("INTENC2CM0"),
    INTENC3CM0("INTENC3CM0"),
    // PORT(TAUD0)
    TOP0U("TOP0U"),
    TOP0UB("TOP0UB"),
    TOP0V("TOP0V"),
    TOP0VB("TOP0VB"),
    TOP0W("TOP0W"),
    TOP0WB("TOP0WB"),
    // PORT(TAUD1)
    TOP1U("TOP1U"),
    TOP1UB("TOP1UB"),
    TOP1V("TOP1V"),
    TOP1VB("TOP1VB"),
    TOP1W("TOP1W"),
    TOP1WB("TOP1WB"),
    // PORT(TSG0)
    TOP2U("TOP2U"),
    TOP2UB("TOP2UB"),
    TOP2V("TOP2V"),
    TOP2VB("TOP2VB"),
    TOP2W("TOP2W"),
    TOP2WB("TOP2WB"),
    // PORT(TSG1)
    TOP3U("TOP3U"),
    TOP3UB("TOP3UB"),
    TOP3V("TOP3V"),
    TOP3VB("TOP3VB"),
    TOP3W("TOP3W"),
    TOP3WB("TOP3WB")
{//{{{
    //Bind clock, reset signal and target socket to Bus IF
    setSlaveFreqPort32(&PCLK);
    setSlaveResetPort32(&PRESETZ);
    setTargetSocket32("ts");
    ts = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding

    mPIC1A_Func = new PIC1A_Func((std::string)name, this);
    sc_assert(mPIC1A_Func != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mPIC1A_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(1);

    // Create ports
    std::ostringstream port_name;
    // TAUDn, DNF
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumTAUD; i++) {
        port_name.str("");
        port_name << "TAUD0CDEN" << i;
        TAUD0CDEN[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(TAUD0CDEN[i] != NULL);

        port_name.str("");
        port_name << "TAUD0TOUT" << i;
        TAUD0TOUT[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(TAUD0TOUT[i] != NULL);
        
        port_name.str("");
        port_name << "TAUD0INT" << i;
        TAUD0INT[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(TAUD0INT[i] != NULL);
        
        port_name.str("");
        port_name << "TAUD0SST" << i;
        TAUD0SST[i] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(TAUD0SST[i] != NULL);
        
        port_name.str("");
        port_name << "TAUD0TIN" << i;
        TAUD0TIN[i] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(TAUD0TIN[i] != NULL);
        
        port_name.str("");
        port_name << "TAUD1CDEN" << i;
        TAUD1CDEN[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(TAUD1CDEN[i] != NULL);

        port_name.str("");
        port_name << "TAUD1TOUT" << i;
        TAUD1TOUT[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(TAUD1TOUT[i] != NULL);
        
        port_name.str("");
        port_name << "TAUD1INT" << i;
        TAUD1INT[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(TAUD1INT[i] != NULL);
        
        port_name.str("");
        port_name << "TAUD1SST" << i;
        TAUD1SST[i] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(TAUD1SST[i] != NULL);
        
        port_name.str("");
        port_name << "TAUD1TIN" << i;
        TAUD1TIN[i] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(TAUD1TIN[i] != NULL);
        
        port_name.str("");
        port_name << "DNFTAUD0TIN" << i;
        DNFTAUD0TIN[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(DNFTAUD0TIN[i] != NULL);
        
        port_name.str("");
        port_name << "DNFTAUD1TIN" << i;
        DNFTAUD1TIN[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(DNFTAUD1TIN[i] != NULL);
    }
    // PIC2
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumPIC2; i++) {
        port_name.str("");
        port_name << "ADCATTOUT0" << i;
        ADCATTOUT0[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(ADCATTOUT0[i] != NULL);
        
        port_name.str("");
        port_name << "ADCATTOUT1" << i;
        ADCATTOUT1[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(ADCATTOUT1[i] != NULL);
    }
    // TAUJn, DNF
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumTAUJ; i++) {
        port_name.str("");
        port_name << "DNFTAUJ0TIN" << i;
        DNFTAUJ0TIN[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(DNFTAUJ0TIN[i] != NULL);
        
        port_name.str("");
        port_name << "TAUJ0TSST" << i;
        TAUJ0TSST[i] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(TAUJ0TSST[i] != NULL);
        
        port_name.str("");
        port_name << "TAUJ0TIN" << i;
        TAUJ0TIN[i] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(TAUJ0TIN[i] != NULL);
        
        port_name.str("");
        port_name << "TAUJ1TSST" << i;
        TAUJ1TSST[i] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(TAUJ1TSST[i] != NULL);
    }
    // DNF
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumANFESO; i++) {
        port_name.str("");
        port_name << "ANFESO" << i;
        ANFESO[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(ANFESO[i] != NULL);
    }
    // HiZ Source
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumHiZ; i++) {
        port_name.str("");
        port_name << "RSVHIZIN" << i;
        RSVHIZIN[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(RSVHIZIN[i] != NULL);
    }
    // PORT(TSGn Hall Sensor), TSGn
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumTSG; i++) {
        port_name.str("");
        port_name << "PORTTSG0TSTAPT" << i;
        PORTTSG0TSTAPT[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(PORTTSG0TSTAPT[i] != NULL);
        
        port_name.str("");
        port_name << "PORTTSG1TSTAPT" << i;
        PORTTSG1TSTAPT[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(PORTTSG1TSTAPT[i] != NULL);
        
        port_name.str("");
        port_name << "TSG0TSTAPT" << i;
        TSG0TSTAPT[i] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(TSG0TSTAPT[i] != NULL);
        
        port_name.str("");
        port_name << "TSG1TSTAPT" << i;
        TSG1TSTAPT[i] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(TSG1TSTAPT[i] != NULL);
    }
    
    // Initialize output signals
    // ENCA0
    ENCAT0TSST.initialize(false);
    ENCAT0AIN.initialize(false);
    ENCAT0BIN.initialize(false);
    ENCAT0ZIN.initialize(false);
    ENCAT0TIN1.initialize(false);
    // ENCA1
    ENCAT1TSST.initialize(false);
    ENCAT1AIN.initialize(false);
    ENCAT1BIN.initialize(false);
    ENCAT1ZIN.initialize(false);
    ENCAT1TIN1.initialize(false);
    // ENCA2
    ENCAT2TSST.initialize(false);
    ENCAT2AIN.initialize(false);
    ENCAT2BIN.initialize(false);
    ENCAT2ZIN.initialize(false);
    ENCAT2TIN1.initialize(false);
    // ENCA3
    ENCAT3TSST.initialize(false);
    ENCAT3AIN.initialize(false);
    ENCAT3BIN.initialize(false);
    ENCAT3ZIN.initialize(false);
    ENCAT3TIN1.initialize(false);
    // TAPA0
    TOP0TAPATHASIN.initialize(false);
    TOP0TAPATSIM0.initialize(false);
    TOP0TAPATUDCM0.initialize(false);
    // TAPA1
    TOP1TAPATHASIN.initialize(false);
    TOP1TAPATSIM0.initialize(false);
    TOP1TAPATUDCM0.initialize(false);
    // TAPA2
    TOP2TAPATHASIN.initialize(false);
    // TAPA3
    TOP3TAPATHASIN.initialize(false);
    // TAUDn
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumTAUD; i++) {
        TAUD0SST[i]->initialize(false);
        TAUD0TIN[i]->initialize(false);
        TAUD1SST[i]->initialize(false);
        TAUD1TIN[i]->initialize(false);
    }
    // TAUJn
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumTAUJ; i++) {
        TAUJ0TSST[i]->initialize(false);
        TAUJ0TIN[i]->initialize(false);
        TAUJ1TSST[i]->initialize(false);
    }
    // TPBA0
    TPB0TSST.initialize(false);
    // TPBA1
    TPB1TSST.initialize(false);
    // TSGn
    TSG0TSST.initialize(false);
    TSG0TSTOPS.initialize(false);
    TSG0TSTOPC0.initialize(false);
    TSG0TSTOPC1.initialize(false);
    TSG0TSTCKE.initialize(false);
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumTSG; i++) {
        TSG0TSTAPT[i]->initialize(false);
        TSG1TSTAPT[i]->initialize(false);
    }
    TSG0TSTIUS.initialize(false);
    TSG0TSTIUC.initialize(false);
    TSG0TSTIVS.initialize(false);
    TSG0TSTIVC.initialize(false);
    TSG0TSTIWS.initialize(false);
    TSG0TSTIWC.initialize(false);
    TSG1TSST.initialize(false);
    TSG1TSTOPS.initialize(false);
    TSG1TSTOPC0.initialize(false);
    TSG1TSTOPC1.initialize(false);
    TSG1TSTCKE.initialize(false);
    TSG1TSTIUS.initialize(false);
    TSG1TSTIUC.initialize(false);
    TSG1TSTIVS.initialize(false);
    TSG1TSTIVC.initialize(false);
    TSG1TSTIWS.initialize(false);
    TSG1TSTIWC.initialize(false);
    // OSTM0
    OST0TSST.initialize(false);
    // OSTM1
    OST1TSST.initialize(false);
    // OSTM2
    OST2TSST.initialize(false);
    // OSTM3
    OST3TSST.initialize(false);
    // INTC
    INTENC0CM0.initialize(false);
    INTENC1CM0.initialize(false);
    INTENC2CM0.initialize(false);
    INTENC3CM0.initialize(false);
    // PORT(TAUD0)
    TOP0U.initialize(false);
    TOP0UB.initialize(false);
    TOP0V.initialize(false);
    TOP0VB.initialize(false);
    TOP0W.initialize(false);
    TOP0WB.initialize(false);
    // PORT(TAUD1)
    TOP1U.initialize(false);
    TOP1UB.initialize(false);
    TOP1V.initialize(false);
    TOP1VB.initialize(false);
    TOP1W.initialize(false);
    TOP1WB.initialize(false);
    // PORT(TSG0)
    TOP2U.initialize(false);
    TOP2UB.initialize(false);
    TOP2V.initialize(false);
    TOP2VB.initialize(false);
    TOP2W.initialize(false);
    TOP2WB.initialize(false);
    // PORT(TSG1)
    TOP3U.initialize(false);
    TOP3UB.initialize(false);
    TOP3V.initialize(false);
    TOP3VB.initialize(false);
    TOP3W.initialize(false);
    TOP3WB.initialize(false);

    mPIC_Port = new PIC_Port("PIC_Port", this);
    sc_assert(mPIC_Port != NULL);
    
    // Ports binding
    // Input ports
    // ENCA0
    mPIC_Port->InputPort[PIC1A_AgentController::emENCAT0IOV]->bind(ENCAT0IOV);
    mPIC_Port->InputPort[PIC1A_AgentController::emENCAT0IEC]->bind(ENCAT0IEC);
    mPIC_Port->InputPort[PIC1A_AgentController::emENCAT0INT0]->bind(ENCAT0INT0);
    mPIC_Port->InputPort[PIC1A_AgentController::emENCAT0EQ0]->bind(ENCAT0EQ0);
    mPIC_Port->InputPort[PIC1A_AgentController::emENCAT0ECS]->bind(ENCAT0ECS);
    mPIC_Port->InputPort[PIC1A_AgentController::emENCAT0EQ1]->bind(ENCAT0EQ1);
    mPIC_Port->InputPort[PIC1A_AgentController::emENCAT0INT1]->bind(ENCAT0INT1);
    mPIC_Port->InputPort[PIC1A_AgentController::emENCAT0IUD]->bind(ENCAT0IUD);
    // ENCA1
    mPIC_Port->InputPort[PIC1A_AgentController::emENCAT1IOV]->bind(ENCAT1IOV);
    mPIC_Port->InputPort[PIC1A_AgentController::emENCAT1IEC]->bind(ENCAT1IEC);
    mPIC_Port->InputPort[PIC1A_AgentController::emENCAT1INT0]->bind(ENCAT1INT0);
    mPIC_Port->InputPort[PIC1A_AgentController::emENCAT1EQ0]->bind(ENCAT1EQ0);
    mPIC_Port->InputPort[PIC1A_AgentController::emENCAT1ECS]->bind(ENCAT1ECS);
    mPIC_Port->InputPort[PIC1A_AgentController::emENCAT1EQ1]->bind(ENCAT1EQ1);
    mPIC_Port->InputPort[PIC1A_AgentController::emENCAT1INT1]->bind(ENCAT1INT1);
    // ENCA2
    mPIC_Port->InputPort[PIC1A_AgentController::emENCAT2IOV]->bind(ENCAT2IOV);
    mPIC_Port->InputPort[PIC1A_AgentController::emENCAT2INT0]->bind(ENCAT2INT0);
    mPIC_Port->InputPort[PIC1A_AgentController::emENCAT2INT1]->bind(ENCAT2INT1);
    // ENCA3
    mPIC_Port->InputPort[PIC1A_AgentController::emENCAT3IOV]->bind(ENCAT3IOV);
    mPIC_Port->InputPort[PIC1A_AgentController::emENCAT3INT0]->bind(ENCAT3INT0);
    mPIC_Port->InputPort[PIC1A_AgentController::emENCAT3INT1]->bind(ENCAT3INT1);
    // TAUDn, DNF
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumTAUD; i++) {
        mPIC_Port->InputPort[PIC1A_AgentController::emTAUD0CDEN0 + i]->bind(*TAUD0CDEN[i]);
        mPIC_Port->InputPort[PIC1A_AgentController::emTAUD0TOUT0 + i]->bind(*TAUD0TOUT[i]);
        mPIC_Port->InputPort[PIC1A_AgentController::emTAUD0INT0 + i]->bind(*TAUD0INT[i]);
        mPIC_Port->InputPort[PIC1A_AgentController::emTAUD1CDEN0 + i]->bind(*TAUD1CDEN[i]);
        mPIC_Port->InputPort[PIC1A_AgentController::emTAUD1TOUT0 + i]->bind(*TAUD1TOUT[i]);
        mPIC_Port->InputPort[PIC1A_AgentController::emTAUD1INT0 + i]->bind(*TAUD1INT[i]);
        mPIC_Port->InputPort[PIC1A_AgentController::emDNFTAUD0TIN0 + i]->bind(*DNFTAUD0TIN[i]);
        mPIC_Port->InputPort[PIC1A_AgentController::emDNFTAUD1TIN0 + i]->bind(*DNFTAUD1TIN[i]);
    }
    mPIC_Port->InputPort[PIC1A_AgentController::emTAUD0UDC0]->bind(TAUD0UDC0);
    mPIC_Port->InputPort[PIC1A_AgentController::emTAUD0UDC2]->bind(TAUD0UDC2);
    mPIC_Port->InputPort[PIC1A_AgentController::emTAUD0UDC8]->bind(TAUD0UDC8);
    mPIC_Port->InputPort[PIC1A_AgentController::emTAUD0CKEN10]->bind(TAUD0CKEN10);
    mPIC_Port->InputPort[PIC1A_AgentController::emTAUD0CKEN12]->bind(TAUD0CKEN12);
    mPIC_Port->InputPort[PIC1A_AgentController::emTAUD0CKEN14]->bind(TAUD0CKEN14);
    mPIC_Port->InputPort[PIC1A_AgentController::emTAUD1UDC0]->bind(TAUD1UDC0);
    mPIC_Port->InputPort[PIC1A_AgentController::emTAUD1UDC2]->bind(TAUD1UDC2);
    mPIC_Port->InputPort[PIC1A_AgentController::emTAUD1UDC8]->bind(TAUD1UDC8);
    mPIC_Port->InputPort[PIC1A_AgentController::emTAUD1CKEN10]->bind(TAUD1CKEN10);
    mPIC_Port->InputPort[PIC1A_AgentController::emTAUD1CKEN12]->bind(TAUD1CKEN12);
    mPIC_Port->InputPort[PIC1A_AgentController::emTAUD1CKEN14]->bind(TAUD1CKEN14);
    // TAUJn
    mPIC_Port->InputPort[PIC1A_AgentController::emTAUJ0TOUT3]->bind(TAUJ0TOUT3);
    mPIC_Port->InputPort[PIC1A_AgentController::emTAUJ1TOUT3]->bind(TAUJ1TOUT3);
    // TPBA0
    mPIC_Port->InputPort[PIC1A_AgentController::emTPB0INTPAT]->bind(TPB0INTPAT);
    // TSG0
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG0TSTPTE]->bind(TSG0TSTPTE);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG0TS2PEC]->bind(TSG0TS2PEC);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG0TST2PUD]->bind(TSG0TST2PUD);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG0TO1]->bind(TSG0TO1);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG0TO2]->bind(TSG0TO2);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG0TO3]->bind(TSG0TO3);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG0TO4]->bind(TSG0TO4);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG0TO5]->bind(TSG0TO5);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG0TO6]->bind(TSG0TO6);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG0INT0]->bind(TSG0INT0);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG0INT2]->bind(TSG0INT2);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG0INT6]->bind(TSG0INT6);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG0INT10]->bind(TSG0INT10);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG0TSTIER]->bind(TSG0TSTIER);
    // TSG1
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG1TSTPTE]->bind(TSG1TSTPTE);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG1TS2PEC]->bind(TSG1TS2PEC);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG1TST2PUD]->bind(TSG1TST2PUD);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG1TO1]->bind(TSG1TO1);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG1TO2]->bind(TSG1TO2);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG1TO3]->bind(TSG1TO3);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG1TO4]->bind(TSG1TO4);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG1TO5]->bind(TSG1TO5);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG1TO6]->bind(TSG1TO6);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG1INT0]->bind(TSG1INT0);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG1INT2]->bind(TSG1INT2);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG1INT6]->bind(TSG1INT6);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG1INT10]->bind(TSG1INT10);
    mPIC_Port->InputPort[PIC1A_AgentController::emTSG1TSTIER]->bind(TSG1TSTIER);
    // PIC2
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumPIC2; i++) {
        mPIC_Port->InputPort[PIC1A_AgentController::emADCATTOUT00 + i]->bind(*ADCATTOUT0[i]);
        mPIC_Port->InputPort[PIC1A_AgentController::emADCATTOUT10 + i]->bind(*ADCATTOUT1[i]);
    }
    // DNF
    mPIC_Port->InputPort[PIC1A_AgentController::emDNFENCAT0AIN]->bind(DNFENCAT0AIN);
    mPIC_Port->InputPort[PIC1A_AgentController::emDNFENCAT0BIN]->bind(DNFENCAT0BIN);
    mPIC_Port->InputPort[PIC1A_AgentController::emDNFENCAT0TIN1]->bind(DNFENCAT0TIN1);
    mPIC_Port->InputPort[PIC1A_AgentController::emDNFENCAT0ZIN]->bind(DNFENCAT0ZIN);
    mPIC_Port->InputPort[PIC1A_AgentController::emDNFENCAT1AIN]->bind(DNFENCAT1AIN);
    mPIC_Port->InputPort[PIC1A_AgentController::emDNFENCAT1BIN]->bind(DNFENCAT1BIN);
    mPIC_Port->InputPort[PIC1A_AgentController::emDNFENCAT1TIN1]->bind(DNFENCAT1TIN1);
    mPIC_Port->InputPort[PIC1A_AgentController::emDNFENCAT1ZIN]->bind(DNFENCAT1ZIN);
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumTAUJ; i++) {
        mPIC_Port->InputPort[PIC1A_AgentController::emDNFTAUJ0TIN0 + i]->bind(*DNFTAUJ0TIN[i]);
    }
    mPIC_Port->InputPort[PIC1A_AgentController::emDNFTSG0TSTCKE]->bind(DNFTSG0TSTCKE);
    mPIC_Port->InputPort[PIC1A_AgentController::emDNFTSG1TSTCKE]->bind(DNFTSG1TSTCKE);
    mPIC_Port->InputPort[PIC1A_AgentController::emDNFSIF0IREF]->bind(DNFSIF0IREF);
    mPIC_Port->InputPort[PIC1A_AgentController::emDNFSIF1IREF]->bind(DNFSIF1IREF);
    mPIC_Port->InputPort[PIC1A_AgentController::emDNFENCAT2TIN1]->bind(DNFENCAT2TIN1);
    mPIC_Port->InputPort[PIC1A_AgentController::emDNFENCAT3TIN1]->bind(DNFENCAT3TIN1);
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumANFESO; i++) {
        mPIC_Port->InputPort[PIC1A_AgentController::emANFESO0 + i]->bind(*ANFESO[i]);
    }
    // ECM
    mPIC_Port->InputPort[PIC1A_AgentController::emERROROUTZ]->bind(ERROROUTZ);
    // HiZ Source
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumHiZ; i++) {
        mPIC_Port->InputPort[PIC1A_AgentController::emRSVHIZIN0 + i]->bind(*RSVHIZIN[i]);
    }
    // PORT(TSGn Hall Sensor)
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumTSG; i++) {
        mPIC_Port->InputPort[PIC1A_AgentController::emPORTTSG0TSTAPT0 + i]->bind(*PORTTSG0TSTAPT[i]);
        mPIC_Port->InputPort[PIC1A_AgentController::emPORTTSG1TSTAPT0 + i]->bind(*PORTTSG1TSTAPT[i]);
    }
    // RDC0
    mPIC_Port->InputPort[PIC1A_AgentController::emSAT0AOUT]->bind(SAT0AOUT);
    mPIC_Port->InputPort[PIC1A_AgentController::emSAT0BOUT]->bind(SAT0BOUT);
    mPIC_Port->InputPort[PIC1A_AgentController::emSAT0ZOUT]->bind(SAT0ZOUT);
    // RDC1
    mPIC_Port->InputPort[PIC1A_AgentController::emSAT1AOUT]->bind(SAT1AOUT);
    mPIC_Port->InputPort[PIC1A_AgentController::emSAT1BOUT]->bind(SAT1BOUT);
    mPIC_Port->InputPort[PIC1A_AgentController::emSAT1ZOUT]->bind(SAT1ZOUT);
    
    // Output ports
    // ENCA0
    mPIC_Port->OutputPort[PIC1A_AgentController::emENCAT0AIN]->bind(ENCAT0AIN);
    mPIC_Port->OutputPort[PIC1A_AgentController::emENCAT0BIN]->bind(ENCAT0BIN);
    mPIC_Port->OutputPort[PIC1A_AgentController::emENCAT0ZIN]->bind(ENCAT0ZIN);
    mPIC_Port->OutputPort[PIC1A_AgentController::emENCAT0TIN1]->bind(ENCAT0TIN1);
    // ENCA1
    mPIC_Port->OutputPort[PIC1A_AgentController::emENCAT1AIN]->bind(ENCAT1AIN);
    mPIC_Port->OutputPort[PIC1A_AgentController::emENCAT1BIN]->bind(ENCAT1BIN);
    mPIC_Port->OutputPort[PIC1A_AgentController::emENCAT1ZIN]->bind(ENCAT1ZIN);
    mPIC_Port->OutputPort[PIC1A_AgentController::emENCAT1TIN1]->bind(ENCAT1TIN1);
    // ENCA2
    mPIC_Port->OutputPort[PIC1A_AgentController::emENCAT2AIN]->bind(ENCAT2AIN);
    mPIC_Port->OutputPort[PIC1A_AgentController::emENCAT2BIN]->bind(ENCAT2BIN);
    mPIC_Port->OutputPort[PIC1A_AgentController::emENCAT2ZIN]->bind(ENCAT2ZIN);
    mPIC_Port->OutputPort[PIC1A_AgentController::emENCAT2TIN1]->bind(ENCAT2TIN1);
    // ENCA3
    mPIC_Port->OutputPort[PIC1A_AgentController::emENCAT3AIN]->bind(ENCAT3AIN);
    mPIC_Port->OutputPort[PIC1A_AgentController::emENCAT3BIN]->bind(ENCAT3BIN);
    mPIC_Port->OutputPort[PIC1A_AgentController::emENCAT3ZIN]->bind(ENCAT3ZIN);
    mPIC_Port->OutputPort[PIC1A_AgentController::emENCAT3TIN1]->bind(ENCAT3TIN1);
    // TAPA0
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP0TAPATHASIN]->bind(TOP0TAPATHASIN);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP0TAPATSIM0]->bind(TOP0TAPATSIM0);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP0TAPATUDCM0]->bind(TOP0TAPATUDCM0);
    // TAPA1
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP1TAPATHASIN]->bind(TOP1TAPATHASIN);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP1TAPATSIM0]->bind(TOP1TAPATSIM0);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP1TAPATUDCM0]->bind(TOP1TAPATUDCM0);
    // TAPA2
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP2TAPATHASIN]->bind(TOP2TAPATHASIN);
    // TAPA3
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP3TAPATHASIN]->bind(TOP3TAPATHASIN);
    // TAUDn
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumTAUD; i++) {
        mPIC_Port->OutputPort[PIC1A_AgentController::emTAUD0TIN0 + i]->bind(*TAUD0TIN[i]);
        mPIC_Port->OutputPort[PIC1A_AgentController::emTAUD1TIN0 + i]->bind(*TAUD1TIN[i]);
    }
    // TAUJ0
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumTAUJ; i++) {
        mPIC_Port->OutputPort[PIC1A_AgentController::emTAUJ0TIN0 + i]->bind(*TAUJ0TIN[i]);
    }
    // TSGn
    mPIC_Port->OutputPort[PIC1A_AgentController::emTSG0TSTOPS]->bind(TSG0TSTOPS);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTSG0TSTOPC0]->bind(TSG0TSTOPC0);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTSG0TSTOPC1]->bind(TSG0TSTOPC1);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTSG0TSTCKE]->bind(TSG0TSTCKE);
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumTSG; i++) {
        mPIC_Port->OutputPort[PIC1A_AgentController::emTSG0TSTAPT0 + i]->bind(*TSG0TSTAPT[i]);
        mPIC_Port->OutputPort[PIC1A_AgentController::emTSG1TSTAPT0 + i]->bind(*TSG1TSTAPT[i]);
    }
    mPIC_Port->OutputPort[PIC1A_AgentController::emTSG0TSTIUS]->bind(TSG0TSTIUS);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTSG0TSTIUC]->bind(TSG0TSTIUC);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTSG0TSTIVS]->bind(TSG0TSTIVS);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTSG0TSTIVC]->bind(TSG0TSTIVC);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTSG0TSTIWS]->bind(TSG0TSTIWS);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTSG0TSTIWC]->bind(TSG0TSTIWC);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTSG1TSTOPS]->bind(TSG1TSTOPS);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTSG1TSTOPC0]->bind(TSG1TSTOPC0);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTSG1TSTOPC1]->bind(TSG1TSTOPC1);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTSG1TSTCKE]->bind(TSG1TSTCKE);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTSG1TSTIUS]->bind(TSG1TSTIUS);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTSG1TSTIUC]->bind(TSG1TSTIUC);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTSG1TSTIVS]->bind(TSG1TSTIVS);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTSG1TSTIVC]->bind(TSG1TSTIVC);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTSG1TSTIWS]->bind(TSG1TSTIWS);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTSG1TSTIWC]->bind(TSG1TSTIWC);
    // INTC
    mPIC_Port->OutputPort[PIC1A_AgentController::emINTENC0CM0]->bind(INTENC0CM0);
    mPIC_Port->OutputPort[PIC1A_AgentController::emINTENC1CM0]->bind(INTENC1CM0);
    mPIC_Port->OutputPort[PIC1A_AgentController::emINTENC2CM0]->bind(INTENC2CM0);
    mPIC_Port->OutputPort[PIC1A_AgentController::emINTENC3CM0]->bind(INTENC3CM0);
    // PORT(TAUD0)
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP0U]->bind(TOP0U);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP0UB]->bind(TOP0UB);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP0V]->bind(TOP0V);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP0VB]->bind(TOP0VB);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP0W]->bind(TOP0W);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP0WB]->bind(TOP0WB);
    // PORT(TAUD1)
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP1U]->bind(TOP1U);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP1UB]->bind(TOP1UB);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP1V]->bind(TOP1V);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP1VB]->bind(TOP1VB);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP1W]->bind(TOP1W);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP1WB]->bind(TOP1WB);
    // PORT(TSG0)
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP2U]->bind(TOP2U);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP2UB]->bind(TOP2UB);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP2V]->bind(TOP2V);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP2VB]->bind(TOP2VB);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP2W]->bind(TOP2W);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP2WB]->bind(TOP2WB);
    // PORT(TSG1)
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP3U]->bind(TOP3U);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP3UB]->bind(TOP3UB);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP3V]->bind(TOP3V);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP3VB]->bind(TOP3VB);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP3W]->bind(TOP3W);
    mPIC_Port->OutputPort[PIC1A_AgentController::emTOP3WB]->bind(TOP3WB);
    
    // Synchronous Start Output ports
    // ENCAn
    mPIC_Port->SyncOutputPort[PIC1A_AgentController::emENCAT0TSST]->bind(ENCAT0TSST);
    mPIC_Port->SyncOutputPort[PIC1A_AgentController::emENCAT1TSST]->bind(ENCAT1TSST);
    mPIC_Port->SyncOutputPort[PIC1A_AgentController::emENCAT2TSST]->bind(ENCAT2TSST);
    mPIC_Port->SyncOutputPort[PIC1A_AgentController::emENCAT3TSST]->bind(ENCAT3TSST);
    // TAUDn
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumTAUD; i++) {
        mPIC_Port->SyncOutputPort[PIC1A_AgentController::emTAUD0SST0 + i]->bind(*TAUD0SST[i]);
        mPIC_Port->SyncOutputPort[PIC1A_AgentController::emTAUD1SST0 + i]->bind(*TAUD1SST[i]);
    }
    // TAUJn
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumTAUJ; i++) {
        mPIC_Port->SyncOutputPort[PIC1A_AgentController::emTAUJ0TSST0 + i]->bind(*TAUJ0TSST[i]);
        mPIC_Port->SyncOutputPort[PIC1A_AgentController::emTAUJ1TSST0 + i]->bind(*TAUJ1TSST[i]);
    }
    // TPBAn
    mPIC_Port->SyncOutputPort[PIC1A_AgentController::emTPB0TSST]->bind(TPB0TSST);
    mPIC_Port->SyncOutputPort[PIC1A_AgentController::emTPB1TSST]->bind(TPB1TSST);
    // TSGn
    mPIC_Port->SyncOutputPort[PIC1A_AgentController::emTSG0TSST]->bind(TSG0TSST);
    mPIC_Port->SyncOutputPort[PIC1A_AgentController::emTSG1TSST]->bind(TSG1TSST);
    // OSTMn
    mPIC_Port->SyncOutputPort[PIC1A_AgentController::emOST0TSST]->bind(OST0TSST);
    mPIC_Port->SyncOutputPort[PIC1A_AgentController::emOST1TSST]->bind(OST1TSST);
    mPIC_Port->SyncOutputPort[PIC1A_AgentController::emOST2TSST]->bind(OST2TSST);
    mPIC_Port->SyncOutputPort[PIC1A_AgentController::emOST3TSST]->bind(OST3TSST);

    
    //Initialize variables
    mPCLKPeriod = 0;
    mPCLKFreq = 0;
    mPCLKOrgFreq = 0;
    mPCLKFreqUnit = "Hz"; 
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    mPRESETZSignalCurrentValue = true;
    mPRESETZSignalPreviousValue = true;
    mIsPRESETZHardActive = false;
    mIsPRESETZCmdReceive = false;
    mIsPRESETZCmdActive = false;
    mPRESETZCmdPeriod =  0;

    mSync_chsb_clsbPeriod = 0;
    mSync_chsb_clsbFreq = 0;

    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << PCLK;
    
    SC_METHOD(HandleSync_chsb_clsbSignalMethod);
    dont_initialize();
    sensitive << sync_chsb_clsb;

    SC_METHOD(HandlePRESETZSignalMethod);
    sensitive << PRESETZ;

    SC_METHOD(HandlePRESETZHardMethod);
    dont_initialize();
    sensitive << mPRESETZHardEvent;

    SC_METHOD(HandlePRESETZCmdMethod);
    dont_initialize();
    sensitive << mPRESETZCmdEvent;

    SC_METHOD(CancelPRESETZCmdMethod);
    dont_initialize();
    sensitive << mPRESETZCmdCancelEvent;
    
    for (unsigned int i = 0; i < 2; i++) {
        for (unsigned int j = 0; j < PIC1A_AgentController::emWNGnum; j++) {
            sc_core::sc_spawn_options opt;
            opt.spawn_method();
            opt.set_sensitivity(&mUpdateFlagEvent[i][j]);
            opt.dont_initialize();
            sc_core::sc_spawn(sc_bind(&PIC1A::UpdateFlagMethod, this, i, j),
                            sc_core::sc_gen_unique_name("UpdateFlagMethod"), &opt);
        }
    }
    
    for (unsigned int i = 0; i < 2; i++) {
        for (unsigned int j = 0; j < PIC1A_AgentController::emWNGnum; j++) {
            sc_core::sc_spawn_options opt;
            opt.spawn_method();
            opt.set_sensitivity(&mUpdateWNG_FFiOuputEvent[i][j]);
            opt.dont_initialize();
            sc_core::sc_spawn(sc_bind(&PIC1A::UpdateWNG_FFiOuputMethod, this, i, j),
                              sc_core::sc_gen_unique_name("UpdateWNG_FFiOuputMethod"), &opt);
        }
    }
    
    for (unsigned int i = 0; i < 2; i++) {
        for (unsigned int j = 0; j < PIC1A_AgentController::emWNGnum; j++) {
            sc_core::sc_spawn_options opt;
            opt.spawn_method();
            opt.set_sensitivity(&mUpdateWNG_FFgOuputEvent[i][j]);
            opt.dont_initialize();
            sc_core::sc_spawn(sc_bind(&PIC1A::UpdateWNG_FFgOuputMethod, this, i, j),
                              sc_core::sc_gen_unique_name("UpdateWNG_FFgOuputMethod"), &opt);
        }
    }
    
    for (unsigned int i = 0; i < 2; i++) {
        for (unsigned int j = 0; j < PIC1A_AgentController::emFFnum; j++) {
            sc_core::sc_spawn_options opt;
            opt.spawn_method();
            opt.set_sensitivity(&mUpdateREG513_FFOuputEvent[i][j]);
            opt.dont_initialize();
            sc_core::sc_spawn(sc_bind(&PIC1A::UpdateREG513_FFOuputMethod, this, i, j),
                              sc_core::sc_gen_unique_name("UpdateREG513_FFOuputMethod"), &opt);
        }
    }
    
    for (unsigned int i = 0; i < 2; i++) {
        for (unsigned int j = 0; j < PIC1A_AgentController::emFFnum; j++) {
            sc_core::sc_spawn_options opt;
            opt.spawn_method();
            opt.set_sensitivity(&mUpdateREG514_FFOuputEvent[i][j]);
            opt.dont_initialize();
            sc_core::sc_spawn(sc_bind(&PIC1A::UpdateREG514_FFOuputMethod, this, i, j),
                              sc_core::sc_gen_unique_name("UpdateREG514_FFOuputMethod"), &opt);
        }
    }
    
    for (unsigned int i = 0; i < 2; i++) {
        for (unsigned int j = 0; j < PIC1A_AgentController::emFFnum; j++) {
            sc_core::sc_spawn_options opt;
            opt.spawn_method();
            opt.set_sensitivity(&mUpdateTSGOUTCTR_FFOuputEvent[i][j]);
            opt.dont_initialize();
            sc_core::sc_spawn(sc_bind(&PIC1A::UpdateTSGOUTCTR_FFOuputMethod, this, i, j),
                              sc_core::sc_gen_unique_name("UpdateTSGOUTCTR_FFOuputMethod"), &opt);
        }
    }
    
    for (unsigned int i = 0; i < PIC1A_AgentController::emRSnum; i++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mHandleResetRSEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PIC1A::HandleResetRSMethod, this, i),
                          sc_core::sc_gen_unique_name("HandleResetRSMethod"), &opt);
    }
    
    for (unsigned int i = 0; i < PIC1A_AgentController::emDTnum; i++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mHandleResetDTEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PIC1A::HandleResetDTMethod, this, i),
                          sc_core::sc_gen_unique_name("HandleResetDTMethod"), &opt);
    }
    
    for (unsigned int i = 0; i < 2; i++) {
        for (unsigned int j = 0; j < PIC1A_AgentController::emRTOnum; j++) {
            sc_core::sc_spawn_options opt;
            opt.spawn_method();
            opt.set_sensitivity(&mHandleResetRTOEvent[i][j]);
            opt.dont_initialize();
            sc_core::sc_spawn(sc_bind(&PIC1A::HandleResetRTOMethod, this, i, j),
                              sc_core::sc_gen_unique_name("HandleResetRTOMethod"), &opt);
        }
    }
}//}}}

///Destructor of PIC class
PIC1A::~PIC1A (void)
{//{{{
    delete mPIC1A_Func;
    delete mPIC_Port;
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumTAUD; i++) {
        delete TAUD0CDEN[i];
        delete TAUD0TOUT[i];
        delete TAUD0INT[i];
        delete TAUD1CDEN[i];
        delete TAUD1TOUT[i];
        delete TAUD1INT[i];
        delete DNFTAUD0TIN[i];
        delete DNFTAUD1TIN[i];
        delete TAUD0SST[i];
        delete TAUD0TIN[i];
        delete TAUD1SST[i];
        delete TAUD1TIN[i];
    }
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumPIC2; i++) {
        delete ADCATTOUT0[i];
        delete ADCATTOUT1[i];
    }
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumTAUJ; i++) {
        delete DNFTAUJ0TIN[i];
        delete TAUJ0TSST[i];
        delete TAUJ0TIN[i];
        delete TAUJ1TSST[i];
    }
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumANFESO; i++) {
        delete ANFESO[i];
    }
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumHiZ; i++) {
        delete RSVHIZIN[i];
    }
    for (unsigned int i = 0; i < PIC1A_AgentController::emPortNumTSG; i++) {
        delete PORTTSG0TSTAPT[i];
        delete PORTTSG1TSTAPT[i];
        delete TSG0TSTAPT[i];
        delete TSG1TSTAPT[i];
    }
}//}}}

///Assert reset by software for PRESETZ
void PIC1A::AssertReset (const double start_time, const double period)
{//{{{
    if ((!mIsPRESETZHardActive)&&(!mIsPRESETZCmdReceive)) {
        mIsPRESETZCmdReceive = true;
        re_printf("info","The model will be reset for %f ns after %f ns\n", period, start_time);
        mPRESETZCmdEvent.notify(start_time, SC_NS);
        mPRESETZCmdPeriod = period;
    } else {
        re_printf("warning","The software reset is called in the reset operation of the model. So it is ignored\n");
    }
}//}}}

///Set clock value and clock unit for PCLK and PCLK
void PIC1A::SetCLKFreq (const sc_dt::uint64 freq, const std::string unit)
{//{{{
    mPCLKOrgFreq = freq;
    ConvertClockFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
    if (mPCLKFreq > 0) {
        mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq)*(double)mTimeResolutionValue) + 0.5);
    } else {
        mPCLKPeriod = 0;
    }
}//}}}

///Get clock value of PCLK and PCLK
void PIC1A::GetCLKFreq (void)
{//{{{
    re_printf("info","PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
}//}}}

///Force value to register
void PIC1A::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mPIC1A_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Release forced value after forcing registers
void PIC1A::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mPIC1A_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Write value to registers by software
void PIC1A::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mPIC1A_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Read value of register by software
void PIC1A::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mPIC1A_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///List all reigsters name
void PIC1A::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mPIC1A_Func->RegisterHandler(cmd);
}//}}}

///Dump help message of all parameters or commands
void PIC1A::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"PIC_MessageLevel (fatal|error|warning|info)       Select debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"PIC_DumpRegisterRW (true/false)                   Select dump information about register reading and writing (Default: false)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"PIC_AssertReset (start_time, period)              Assert and de-assert reset signal to the PIC model");
            SC_REPORT_INFO(this->basename(),"PIC_SetCLKFreq (freq, unit)                       Set frequency value to PIC model.");
            SC_REPORT_INFO(this->basename(),"PIC_GetCLKFreq ()                                 Get frequency value of PIC model.");
            SC_REPORT_INFO(this->basename(),"PIC_ForceRegister (reg_name, reg_value)           Force register with setting value");
            SC_REPORT_INFO(this->basename(),"PIC_ReleaseRegister (reg_name)                    Release register from force value");
            SC_REPORT_INFO(this->basename(),"PIC_WriteRegister (reg_name, reg_value)           Write a value to a register");
            SC_REPORT_INFO(this->basename(),"PIC_ReadRegister (reg_name)                       Read a value from a register");
            SC_REPORT_INFO(this->basename(),"PIC_ListRegister                                  Dump register names of PIC1A_Func block");
        } else {
            re_printf("warning","The name (%s) of PIC_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of PIC_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info)
void PIC1A::SetMessageLevel (const std::string msg_lv)
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
    SeparateString(cmd, msg_lv);
    mPIC1A_Func->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mPIC1A_Func->RegisterHandler(cmd);
}//}}}

///Enable/disable dumping message when users access registers
void PIC1A::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mPIC1A_Func->RegisterHandler(cmd);
}//}}}

///Handle PCLK signal
void PIC1A::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq(freq_value, "Hz");
}//}}}

///Handle sync_chsb_clsb signal
void PIC1A::HandleSync_chsb_clsbSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = sync_chsb_clsb.read();
    mSync_chsb_clsbFreq = freq_value;
    if (mSync_chsb_clsbFreq > 0) {
        mSync_chsb_clsbPeriod = (sc_dt::uint64)(((1/(double)mSync_chsb_clsbFreq)*(double)mTimeResolutionValue) + 0.5);
    } else {
        mSync_chsb_clsbPeriod = 0;
    }
}//}}}

///Handle PRESETZ signal
void PIC1A::HandlePRESETZSignalMethod (void)
{//{{{
    mPRESETZSignalCurrentValue = PRESETZ.read();
    if (mPRESETZSignalCurrentValue != mPRESETZSignalPreviousValue) {//mPRESETZSignalPreviousValue is "true" at initial state
        mPRESETZSignalPreviousValue = mPRESETZSignalCurrentValue;
        if (mPRESETZSignalCurrentValue) {
            // Negate is synchronous with PCLK
            if (CheckClockPeriod("PCLK")) {
                double current_time = sc_time_stamp().to_double();
                mPRESETZHardEvent.notify(CalculateCLKPosEdge("PCLK", current_time), mTimeResolutionUnit);
            } else {
                mPRESETZHardEvent.notify(SC_ZERO_TIME);
            }
        } else {
            // Assert is asynchronous
            mPRESETZHardEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Process reset function when PRESETZ is active
void PIC1A::HandlePRESETZHardMethod (void)
{//{{{
    if (mPRESETZSignalCurrentValue == emPRESETZActive) {
        mIsPRESETZHardActive = true;
        re_printf("info","The reset port is asserted\n");
        this->EnableReset(mIsPRESETZHardActive);
    } else if (mIsPRESETZHardActive) {
        mIsPRESETZHardActive = false;
        this->EnableReset(mIsPRESETZHardActive);
        re_printf("info","The reset port is de-asserted\n");
    }
    //Cancel AssertReset command when PRESETZ is active
    if (mIsPRESETZCmdReceive) {
        mPRESETZCmdEvent.cancel();
        mPRESETZCmdCancelEvent.cancel();
        mIsPRESETZCmdReceive = false;
        mIsPRESETZCmdActive = false;
    }
}//}}}

///Process reset function when PRESETZ command is active
void PIC1A::HandlePRESETZCmdMethod (void)
{//{{{
    mIsPRESETZCmdActive = true;
    re_printf("info","The model is reset by AssertReset command\n");
    this->EnableReset(mIsPRESETZCmdActive);
    mPRESETZCmdCancelEvent.notify(mPRESETZCmdPeriod, SC_NS);
}//}}}

///Cancel reset function when PRESETZ command is active
void PIC1A::CancelPRESETZCmdMethod (void)
{//{{{
    mIsPRESETZCmdActive = false;
    mIsPRESETZCmdReceive = false;
    re_printf("info","Reset period is over\n");
    this->EnableReset(mIsPRESETZCmdActive);
}//}}}

///Execute reset operation
void PIC1A::EnableReset (const bool is_active)
{//{{{
    mPIC1A_Func->Reset(is_active);
    if (is_active) {
        // Initialize variables
        mPIC_Port->Initialize();
        
        // Initialize output ports
        for (unsigned int i = 0; i < PIC1A_AgentController::emOutputNum; i++) {
            mPIC_Port->mWriteOutputPortEvent[i].notify(SC_ZERO_TIME);
        }
        for (unsigned int i = 0; i < PIC1A_AgentController::emSyncOutputNum; i++) {
            mPIC_Port->mWriteSyncOutputEvent[i].notify(SC_ZERO_TIME);
        }
        
        // Cancel events
        for (unsigned int i = 0; i < PIC1A_AgentController::emInputNum; i++) {
            mPIC_Port->mHandleInputChangeEvent[i].cancel();
        }
        for (unsigned int i = 0; i < PIC1A_AgentController::emSyncOutputNum; i++) {
            mPIC_Port->mAssertSyncOutputEvent[i].cancel();
            mPIC_Port->mDeassertSyncOutputEvent[i].cancel();
        }
        for (unsigned int i = 0; i < PIC1A_AgentController::emInputNum; i++) {
            mPIC_Port->mHandleInputChangeEvent[i].cancel();
        }
        for (unsigned int i = 0; i < 2; i++) {
            for (unsigned int j = 0; j < PIC1A_AgentController::emWNGnum; j++) {
                mUpdateFlagEvent[i][j].cancel();
                mUpdateWNG_FFiOuputEvent[i][j].cancel();
                mUpdateWNG_FFgOuputEvent[i][j].cancel();
            }
        }
        for (unsigned int i = 0; i < 2; i++) {
            for (unsigned int j = 0; j < PIC1A_AgentController::emFFnum; j++) {
                mUpdateREG513_FFOuputEvent[i][j].cancel();
                mUpdateREG514_FFOuputEvent[i][j].cancel();
                mUpdateTSGOUTCTR_FFOuputEvent[i][j].cancel();
            }
        }
        for (unsigned int i = 0; i < PIC1A_AgentController::emRSnum; i++) {
            mHandleResetRSEvent[i].cancel();
        }
        for (unsigned int i = 0; i < PIC1A_AgentController::emDTnum; i++) {
            mHandleResetDTEvent[i].cancel();
        }
        for (unsigned int i = 0; i < 2; i++) {
            for (unsigned int j = 0; j < PIC1A_AgentController::emRTOnum; j++) {
                mHandleResetRTOEvent[i][j].cancel();
            }
        }
    } else {
        // Update input ports after reset
        mPIC_Port->mUpdateInputEvent.notify(SC_ZERO_TIME);
    }
}//}}}

/// Update WNGFn flag
void PIC1A::UpdateFlagMethod (const unsigned int module, const unsigned int index)
{//{{{
    if (!GetResetStatus()) {
        if (CheckClockPeriod("PCLK")) {
            mPIC1A_Func->UpdateFlag(module, index);
        }
    }
}//}}}

/// Update WNG_FFi output
void PIC1A::UpdateWNG_FFiOuputMethod (const unsigned int module, const unsigned int index)
{//{{{
    if (!GetResetStatus()) {
        if (CheckClockPeriod("PCLK")) {
            mPIC1A_Func->UpdateWNG_FFiOuput(module, index);
        }
    }
}//}}}

/// Update WNG_FFg output
void PIC1A::UpdateWNG_FFgOuputMethod (const unsigned int module, const unsigned int index)
{//{{{
    if (!GetResetStatus()) {
        if (CheckClockPeriod("PCLK")) {
            mPIC1A_Func->UpdateWNG_FFgOuput(module, index);
        }
    }
}//}}}

/// Update REG513_FFg output
void PIC1A::UpdateREG513_FFOuputMethod (const unsigned int module, const unsigned int index)
{//{{{
    if (!GetResetStatus()) {
        if (CheckClockPeriod("PCLK")) {
            mPIC1A_Func->UpdateREG513_FFOuput(module, index);
        }
    }
}//}}}

/// Update REG514_FFg output
void PIC1A::UpdateREG514_FFOuputMethod (const unsigned int module, const unsigned int index)
{//{{{
    if (!GetResetStatus()) {
        if (CheckClockPeriod("PCLK")) {
            mPIC1A_Func->UpdateREG514_FFOuput(module, index);
        }
    }
}//}}}

/// Update TSGOUTCTR_FFg output
void PIC1A::UpdateTSGOUTCTR_FFOuputMethod (const unsigned int module, const unsigned int index)
{//{{{
    if (!GetResetStatus()) {
        if (CheckClockPeriod("PCLK")) {
            mPIC1A_Func->UpdateTSGOUTCTR_FFOuput(module, index);
        }
    }
}//}}}

/// Reset RSnm
void PIC1A::HandleResetRSMethod (const unsigned int index)
{//{{{
    if (!GetResetStatus()) {
        if (CheckClockPeriod("PCLK")) {
            mPIC1A_Func->CalcRS(index, false);
        }
    }
}//}}}

/// Reset DTnm
void PIC1A::HandleResetDTMethod (const unsigned int index)
{//{{{
    if (!GetResetStatus()) {
        if (CheckClockPeriod("PCLK")) {
            mPIC1A_Func->CalcDT(index, true);
        }
    }
}//}}}

/// Reset RTOnm
void PIC1A::HandleResetRTOMethod (const unsigned int module, const unsigned int index)
{//{{{
    if (!GetResetStatus()) {
        if (CheckClockPeriod("PCLK")) {
            mPIC1A_Func->CalcRTO(module, index, PIC1A_AgentController::emRTOReset);
        }
    }
}//}}}

/// Handle Synchronous Start Trigger function
void PIC1A::TriggerSyncStart (const unsigned int index)
{//{{{
    double current_time = sc_time_stamp().to_double();
    if ((PIC1A_AgentController::emOST0TSST <= index) && (index <= PIC1A_AgentController::emOST3TSST)) {
        mPIC_Port->mAssertSyncOutputEvent[index].notify(CalculateCLKPosEdge("sync_chsb_clsb", current_time) + 3*mSync_chsb_clsbPeriod, mTimeResolutionUnit);
    } else {
        mPIC_Port->mAssertSyncOutputEvent[index].notify(CalculateCLKPosEdge("sync_chsb_clsb", current_time) + 4*mSync_chsb_clsbPeriod, mTimeResolutionUnit);
    }
}//}}}

/// Update value of output ports
void PIC1A::UpdateOutputPort (const unsigned int index, const bool value)
{//{{{
    mPIC_Port->mWriteValue[index] = value;
    mPIC_Port->mWriteOutputPortEvent[index].notify(SC_ZERO_TIME);
}//}}}

/// Trigger the process to update flag
void PIC1A::TriggerSettingFlag (const unsigned int module, const unsigned int index)
{//{{{
    mUpdateFlagEvent[module][index].notify(mPCLKPeriod, mTimeResolutionUnit);
}//}}}

/// Trigger the process to update WNG_FFi output
void PIC1A::WNG_FFiChange (const unsigned int module, const unsigned int index)
{//{{{
    double current_time = sc_time_stamp().to_double();
    mUpdateWNG_FFiOuputEvent[module][index].notify(CalculateCLKPosEdge("PCLK", current_time) + mPCLKPeriod, mTimeResolutionUnit);
}//}}}

/// Trigger the process to update WNG_FFg output
void PIC1A::WNG_FFgChange (const unsigned int module, const unsigned int index)
{//{{{
    double current_time = sc_time_stamp().to_double();
    mUpdateWNG_FFgOuputEvent[module][index].notify(CalculateCLKPosEdge("PCLK", current_time) + mPCLKPeriod, mTimeResolutionUnit);
}//}}}

/// Trigger the process to update REG513_FF output
void PIC1A::REG513_FFChange (const unsigned int module, const unsigned int index)
{//{{{
    double current_time = sc_time_stamp().to_double();
    mUpdateREG513_FFOuputEvent[module][index].notify(CalculateCLKPosEdge("PCLK", current_time) + mPCLKPeriod, mTimeResolutionUnit);
}//}}}

/// Trigger the process to update REG514_FF output
void PIC1A::REG514_FFChange (const unsigned int module, const unsigned int index)
{//{{{
    double current_time = sc_time_stamp().to_double();
    mUpdateREG514_FFOuputEvent[module][index].notify(CalculateCLKPosEdge("PCLK", current_time) + mPCLKPeriod, mTimeResolutionUnit);
}//}}}

/// Trigger the process to update TSGOUTCTR_FF output
void PIC1A::TSGOUTCTR_FFChange (const unsigned int module, const unsigned int index)
{//{{{
    double current_time = sc_time_stamp().to_double();
    mUpdateTSGOUTCTR_FFOuputEvent[module][index].notify(CalculateCLKPosEdge("PCLK", current_time) + mPCLKPeriod, mTimeResolutionUnit);
}//}}}

/// Trigger the process to reset RSnm sub-module
void PIC1A::ResetRS (const unsigned int index)
{//{{{
    double current_time = sc_time_stamp().to_double();
    mHandleResetRSEvent[index].notify(CalculateCLKPosEdge("PCLK", current_time) + mPCLKPeriod, mTimeResolutionUnit);
}//}}}

/// Trigger the process to reset DTnm sub-module
void PIC1A::ResetDT (const unsigned int index)
{//{{{
    double current_time = sc_time_stamp().to_double();
    mHandleResetDTEvent[index].notify(CalculateCLKPosEdge("PCLK", current_time) + mPCLKPeriod, mTimeResolutionUnit);
}//}}}

/// Trigger the process to reset RTOnm sub-module
void PIC1A::ResetRTO (const unsigned int module, const unsigned int index)
{//{{{
    double current_time = sc_time_stamp().to_double();
    mHandleResetRTOEvent[module][index].notify(CalculateCLKPosEdge("PCLK", current_time) + mPCLKPeriod, mTimeResolutionUnit);
}//}}}

///Check frequency value and frequency unit of PCLK clock
void PIC1A::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                             sc_dt::uint64 freq_in, std::string unit_in)
{//{{{
    if(unit_in == "GHz"){
        freq_out = freq_in * (sc_dt::uint64)(1.0e+9);
        unit_out = "GHz";
    } else if (unit_in == "MHz") {
        freq_out = freq_in * (sc_dt::uint64)(1.0e+6);
        unit_out = "MHz";
    } else if (unit_in == "KHz") {
        freq_out = freq_in * (sc_dt::uint64)(1.0e+3);
        unit_out = "KHz";
    } else if (unit_in == "Hz") {
        freq_out = freq_in;
        unit_out = "Hz";
    } else {// Frequency unit is wrong, frequency unit is set as Hz default.
        freq_out = freq_in;
        unit_out = "Hz";
        re_printf("warning","Frequency unit (%s) is wrong, frequency unit is set as unit Hz default.\n", unit_in.c_str());
    }
}//}}}

///Separate some words from a string to store a vector
void PIC1A::SeparateString (std::vector<std::string> &vtr, const std::string msg)
{//{{{
    std::istringstream cmd;
    cmd.str(msg);
    std::string temp = "a"; //Make temp to be not empty
    while (temp != "") {
        std::string command;
        cmd >> command;
        if (command != "") {
            vtr.push_back(command);
        }
        temp = command;
    }
}//}}}

///Get time resolution
void PIC1A::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
{//{{{
  if (sc_get_time_resolution() < sc_time(1, SC_PS)) {
      resolution_value = 1000000000000000LL;
      resolution_unit = SC_FS;
  } else if (sc_get_time_resolution() < sc_time(1, SC_NS)) {
      resolution_value = 1000000000000LL;
      resolution_unit = SC_PS;
  } else if (sc_get_time_resolution() < sc_time(1, SC_US)) {
      resolution_value = 1000000000;
      resolution_unit = SC_NS;
  } else if (sc_get_time_resolution() < sc_time(1, SC_MS)) {
      resolution_value = 1000000;
      resolution_unit = SC_US;
  } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)) {
      resolution_value = 1000;
      resolution_unit = SC_MS;
  } else {
      resolution_value = 1;
      resolution_unit = SC_SEC;
  }
}//}}}

///Calculate synchronous time with PCLK
double PIC1A::CalculateCLKPosEdge (const std::string clock_name, const double time_point)
{//{{{
    double period_num = 0;
    if (clock_name == "PCLK") {
        period_num = time_point / (double)mPCLKPeriod;
    } else {//clock_name is sync_chsb_clsb
        period_num = time_point / (double)mSync_chsb_clsbPeriod;
    }
    unsigned int mod_period_num = (unsigned int)period_num;
    if ((double)mod_period_num < period_num) {
        mod_period_num++;
    }
    double pos_edge_point = 0;
    if (clock_name == "PCLK") {
        pos_edge_point = (double)mod_period_num * (double)mPCLKPeriod - time_point;
    } else {//clock_name is sync_chsb_clsb
        pos_edge_point = (double)mod_period_num * (double)mSync_chsb_clsbPeriod - time_point;
    }
    return pos_edge_point;
}//}}}

///Check reset status
bool PIC1A::GetResetStatus (void)
{//{{{
    bool reset_status = false;
    if (mIsPRESETZHardActive || mIsPRESETZCmdActive) {
        reset_status = true;
    }
    return reset_status;
}//}}}

///Check period value of PCLK, PCLK clock
bool PIC1A::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    clock_period = (clock_name == "PCLK") ? mPCLKPeriod : mSync_chsb_clsbPeriod;
    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

// vim600: set foldmethod=marker :
