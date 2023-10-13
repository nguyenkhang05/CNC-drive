// ---------------------------------------------------------------------
// $Id: ECCCom.cpp,v 1.7 2014/05/30 04:39:23 dungvannguyen Exp $
//
// Copyright(c) 2013 Renesas Electronics Corporation
// Copyright(c) 2013 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "ECCCom.h"

///Constructor of ECCCom class
ECCCom::ECCCom (sc_module_name name,
                unsigned int rLatency,
                unsigned int wLatency,
                unsigned int CapDepth,
                unsigned int ECCCTLInitial,
                unsigned int ERRINTInitial,
                bool SupDupsed,
                bool UeabMode):
    sc_module(name),
    ECCComAgent(),
    BusSlaveBase<32,1>(),
    pclk("pclk"),
    sys_clk("sys_clk"),
    presetz("presetz"),
    sys_resetz("sys_resetz"),
    aeccdisable("aeccdisable"),
    asecdisable("asecdisable"),
    eccdisable("eccdisable"),
    secdisable("secdisable"),
    adederrout("adederrout"),
    asederrout("asederrout"),
    dederrout("dederrout"),
    sederrout("sederrout"),
    aperrout("aperrout"),
    bperrout("bperrout"),
    errovfout("errovfout")
{//{{{
    //Register input,output signals
    aeccdisable.initialize(false);
    asecdisable.initialize(false);
    eccdisable.initialize(false);
    secdisable.initialize(false);
    adederrout.initialize(false);
    asederrout.initialize(false);
    dederrout.initialize(false);
    sederrout.initialize(false);
    aperrout.initialize(false);
    bperrout.initialize(false);
    errovfout.initialize(false);
    std::ostringstream port_name;
    for (unsigned int index = 0; index < emBankNo; index++) {
        port_name.str("");
        port_name << "adederrout_sys" << index;
        adederrout_sys[index] = new sc_out<bool>(port_name.str().c_str());
        port_name.str("");
        port_name << "asederrout_sys" << index;
        asederrout_sys[index] = new sc_out<bool>(port_name.str().c_str());
        port_name.str("");
        port_name << "dederrout_sys" << index;
        dederrout_sys[index] = new sc_out<bool>(port_name.str().c_str());
        port_name.str("");
        port_name << "sederrout_sys" << index;
        sederrout_sys[index] = new sc_out<bool>(port_name.str().c_str());
        port_name.str("");
        port_name << "aperrout_sys" << index;
        aperrout_sys[index] = new sc_out<bool> (port_name.str().c_str());
        port_name.str("");
        port_name << "bperrout_sys" << index;
        bperrout_sys[index] = new sc_out<bool> (port_name.str().c_str());
        port_name.str("");
        port_name << "addressout_sys" << index;
        addressout_sys[index] = new sc_out<unsigned int>(port_name.str().c_str());
        adederrout_sys[index]->initialize(false);
        asederrout_sys[index]->initialize(false);
        dederrout_sys[index]->initialize(false);
        sederrout_sys[index]->initialize(false);
        aperrout_sys[index]->initialize(false);
        bperrout_sys[index]->initialize(false);
        addressout_sys[index]->initialize(0x0);
    }

    //Initialize variables
    mPCLKPeriod = 0;
    mSYS_CLKPeriod = 0;
    mPCLKFreq = 0;
    mPCLKOrgFreq = 0;
    mSYS_CLKFreq = 0;
    mSYS_CLKOrgFreq = 0;
    mTimeResolutionValue = 1;
    mPCLKFreqUnit = "Hz"; 
    mSYS_CLKFreqUnit = "Hz";
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        mResetSignalCurrentValue[reset_id] = true;
        mResetSignalPreviousValue[reset_id] = true;
        mIsResetHardActive[reset_id] = false;
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
        mResetCmdPeriod[reset_id] =  0;
    }

    Initialize();//Intialize variables
    
    //Bind clock, reset signal and target socket to Bus IF
    setSlaveFreqPort32(&pclk);
    setSlaveResetPort32(&presetz);
    setTargetSocket32("ts");
    ts = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding

    mECCComFunc = new ECCComFunc((std::string)name, this, CapDepth, ECCCTLInitial, ERRINTInitial, SupDupsed, UeabMode);
    sc_assert(mECCComFunc != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mECCComFunc);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0xFFFFFFFF);

    for (unsigned int signal_id = 0; signal_id  < emMaxSignalID; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mPCLKErrorOutputActiveEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECCCom::ActivatePCLKErrorOutputSignalsMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("ActivatePCLKErrorOutputSignalsMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id  < emMaxSignalID-1; signal_id ++) {
        for (unsigned int bank_no = 0; bank_no < emBankNo; bank_no++) {
            sc_core::sc_spawn_options opt;
            opt.spawn_method();
            opt.set_sensitivity(&mSYS_CLKErrorOutputActiveEvent[signal_id][bank_no]);
            opt.dont_initialize();
            sc_core::sc_spawn(sc_bind(&ECCCom::ActivateSYS_CLKErrorOutputSignalsMethod, this, signal_id, bank_no),
                              sc_core::sc_gen_unique_name("ActivateSYS_CLKErrorOutputSignalsMethod"), &opt);
        }
    }

    for (unsigned int signal_id = 0; signal_id  < emMaxSignalID; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mPCLKErrorOutputInactiveEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECCCom::DeactivatePCLKErrorOutputSignalsMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("DeactivatePCLKErrorOutputSignalsMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id  < emMaxSignalID-1; signal_id ++) {
        for (unsigned int bank_no = 0; bank_no < emBankNo; bank_no++) {
            sc_core::sc_spawn_options opt;
            opt.spawn_method();
            opt.set_sensitivity(&mSYS_CLKErrorOutputInactiveEvent[signal_id][bank_no]);
            opt.dont_initialize();
            sc_core::sc_spawn(sc_bind(&ECCCom::DeactivateSYS_CLKErrorOutputSignalsMethod, this, signal_id, bank_no),
                              sc_core::sc_gen_unique_name("DeactivateSYS_CLKErrorOutputSignalsMethod"), &opt);
        }
    }

    for (unsigned int signal_id = 0; signal_id  < emMaxSignalID; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mPCLKErrorOutputWriteEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECCCom::WritePCLKErrorOutputSignalsMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("WritePCLKErrorOutputSignalsMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id  < emMaxSignalID-1; signal_id ++) {
        for (unsigned int bank_no = 0; bank_no < emBankNo; bank_no++) {
            sc_core::sc_spawn_options opt;
            opt.spawn_method();
            opt.set_sensitivity(&mSYS_CLKErrorOutputWriteEvent[signal_id][bank_no]);
            opt.dont_initialize();
            sc_core::sc_spawn(sc_bind(&ECCCom::WriteSYS_CLKErrorOutputSignalsMethod, this, signal_id, bank_no),
                              sc_core::sc_gen_unique_name("WriteSYS_CLKErrorOutputSignalsMethod"), &opt);
        }
    }
    for (unsigned int bank_no = 0; bank_no < emBankNo; bank_no++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mErrorAddrOutputEvent[bank_no]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECCCom::WriteErrorAddrOutputMethod, this, bank_no),
                          sc_core::sc_gen_unique_name("WriteErrorAddrOutputMethod"), &opt);
    }

    for (unsigned int bank_no = 0; bank_no < emBankNo; bank_no++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mErrorInputEvent[bank_no]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECCCom::CaptureErrorInputMethod, this, bank_no),
                          sc_core::sc_gen_unique_name("CaptureErrorInputMethod"), &opt);
    }

    for (unsigned int bank_no = 0; bank_no < emBankNo; bank_no++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mIncreaseReqIdEvent[bank_no]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECCCom::IncreaseReqIdMethod, this, bank_no),
                          sc_core::sc_gen_unique_name("IncreaseReqIdMethod"), &opt);
    }

    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << pclk;

    SC_METHOD(HandleSYS_CLKSignalMethod);
    dont_initialize();
    sensitive << sys_clk;

    SC_METHOD(HandlePRESETZSignalMethod);
    sensitive << presetz;

    SC_METHOD(HandleSYS_RESETZSignalMethod);
    sensitive << sys_resetz;

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECCCom::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandlePRESETZHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECCCom::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandlePRESETZCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECCCom::CancelResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }

    SC_METHOD(ControlECCDisableSignalsMethod);
    sensitive << mECCDisableSignalsControlEvent;

}//}}}

///Destructor of ECCCom class
ECCCom::~ECCCom (void)
{//{{{
    delete mECCComFunc;
    for (unsigned int index = 0; index < emBankNo; index++) {
        delete adederrout_sys[index];
        delete asederrout_sys[index];
        delete dederrout_sys[index];
        delete sederrout_sys[index];
        delete aperrout_sys[index];
        delete bperrout_sys[index];
        delete addressout_sys[index];
    }
}//}}}

///Activate error output signals in pclk domain
void ECCCom::ActivatePCLKErrorOutputSignalsMethod (const unsigned int signal_id)
{//{{{
    mPCLKErrorOutputVal[signal_id] = true;
    mPCLKErrorOutputWriteEvent[signal_id] .notify();
    mPCLKErrorOutputInactiveEvent[signal_id].notify((double)mPCLKPeriod, mTimeResolutionUnit);
}//}}}

///Activate error output signals in sys_clk domain
void ECCCom::ActivateSYS_CLKErrorOutputSignalsMethod (const unsigned int signal_id, const unsigned int bank_no)
{//{{{
    mSYS_CLKErrorOutputVal[signal_id][bank_no] = true;
    mSYS_CLKErrorOutputWriteEvent[signal_id][bank_no].notify(); 
    mSYS_CLKErrorOutputInactiveEvent[signal_id][bank_no].notify((double)mSYS_CLKPeriod, mTimeResolutionUnit);
}//}}}

///Deactivate error output signals in pclk domain
void ECCCom::DeactivatePCLKErrorOutputSignalsMethod (const unsigned int signal_id)
{//{{{
    mPCLKErrorOutputVal[signal_id] = false;
    mPCLKErrorOutputWriteEvent[signal_id] .notify();
}//}}}

///Deactivate error output signals in sys_clk domain
void ECCCom::DeactivateSYS_CLKErrorOutputSignalsMethod (const unsigned int signal_id, const unsigned int bank_no)
{//{{{
    mSYS_CLKErrorOutputVal[signal_id][bank_no] = false;
    mSYS_CLKErrorOutputWriteEvent[signal_id][bank_no].notify(); 
}//}}}

///Deactivate error output signals in pclk domain
void ECCCom::WritePCLKErrorOutputSignalsMethod (const unsigned int signal_id)
{//{{{
    switch (signal_id) {
        case emADEDERROUT:
            adederrout.write(mPCLKErrorOutputVal[signal_id]);
            break;
        case emASEDERROUT:
            asederrout.write(mPCLKErrorOutputVal[signal_id]);
            break;
        case emDEDERROUT:
            dederrout.write(mPCLKErrorOutputVal[signal_id]);
            break;
        case emSEDERROUT:
            sederrout.write(mPCLKErrorOutputVal[signal_id]);
            break;
        case emAPERROUT:
            aperrout.write(mPCLKErrorOutputVal[signal_id]);
            break;
        case emBPERROUT:
            bperrout.write(mPCLKErrorOutputVal[signal_id]);
            break;
        default://emERROVFOUT
            errovfout.write(mPCLKErrorOutputVal[signal_id]);
    }
}//}}}

///Deactivate error output signals in sys_clk domain
void ECCCom::WriteSYS_CLKErrorOutputSignalsMethod (const unsigned int signal_id, const unsigned int bank_no)
{//{{{
    switch (signal_id) {
        case emADEDERROUT:
            adederrout_sys[bank_no]->write(mSYS_CLKErrorOutputVal[signal_id][bank_no]);        
            break;
        case emASEDERROUT:
            asederrout_sys[bank_no]->write(mSYS_CLKErrorOutputVal[signal_id][bank_no]);    
            break;
        case emDEDERROUT:
            dederrout_sys[bank_no]->write(mSYS_CLKErrorOutputVal[signal_id][bank_no]); 
            break;
        case emSEDERROUT:
            sederrout_sys[bank_no]->write(mSYS_CLKErrorOutputVal[signal_id][bank_no]);        
            break;
        case emAPERROUT:
            aperrout_sys[bank_no]->write(mSYS_CLKErrorOutputVal[signal_id][bank_no]);
            break;
        default://emBPERROUT:
            bperrout_sys[bank_no]->write(mSYS_CLKErrorOutputVal[signal_id][bank_no]);
    }
}//}}}

///Write error address output signal
void ECCCom::WriteErrorAddrOutputMethod (const unsigned int bank_no)
{//{{{
    addressout_sys[bank_no]->write(mErrorAddrOutput[bank_no]);
}//}}}

///Capture error input
void ECCCom::CaptureErrorInputMethod (const unsigned int bank_no)
{//{{{
    if (mErrorInfo[bank_no].is_ready) { //If there is an detected error
        if (mRespId[bank_no] >= 3) {
            mRespId[bank_no] = 0;
        } else {
            mRespId[bank_no] = mRespId[bank_no] + 1;
        }
        mECCComFunc->ProcessErrorInfo(mErrorInfo[bank_no],bank_no);
        //Clear error information
        mErrorInfo[bank_no].Initialize();
    }
}//}}}

///Capture error address
void ECCCom::IncreaseReqIdMethod (const unsigned int bank_no)
{//{{{
    if (mReqId[bank_no] >= 3) {
        mReqId[bank_no] = 0;
    } else {
        mReqId[bank_no] = mReqId[bank_no] + 1;
    }
}//}}}

///Handle pclk signal
void ECCCom::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = pclk.read();
    SetCLKFreq("pclk", freq_value, "Hz");
}//}}}

///Handle sys_clk signal
void ECCCom::HandleSYS_CLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = sys_clk.read();
    SetCLKFreq("sys_clk", freq_value, "Hz");
}//}}}

///Handle presetz signal
void ECCCom::HandlePRESETZSignalMethod (void)
{//{{{
    mResetSignalCurrentValue[0] = presetz.read();
    if (mResetSignalCurrentValue[0] != mResetSignalPreviousValue[0]) {//mResetSignalPreviousValue is "true" at initial state
        mResetSignalPreviousValue[0] = mResetSignalCurrentValue[0];
        if (mPCLKPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mResetHardEvent[0].notify(CalculateCLKPosEdge("pclk", false, current_time), mTimeResolutionUnit);
        } else {
            mResetHardEvent[0].notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Handle sys_resetz signal
void ECCCom::HandleSYS_RESETZSignalMethod (void)
{//{{{
    mResetSignalCurrentValue[1] = sys_resetz.read();
    if (mResetSignalCurrentValue[1] != mResetSignalPreviousValue[1]) {//mResetSignalPreviousValue is "true" at initial state
        mResetSignalPreviousValue[1] = mResetSignalCurrentValue[1];
        if (mSYS_CLKPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mResetHardEvent[1].notify(CalculateCLKPosEdge("sys_clk", false, current_time), mTimeResolutionUnit);
        } else {
            mResetHardEvent[1].notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Process reset function when presetz or sys_reset is active
void ECCCom::HandleResetHardMethod (const unsigned int reset_id)
{//{{{
    if (mResetSignalCurrentValue[reset_id] == emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == 0) {
            re_printf("info","The reset signal presetz is asserted.\n");
        } else {
            re_printf("info","The reset signal sys_resetz is asserted.\n");
        }
        this->EnableReset(mIsResetHardActive[reset_id], reset_id);
    } else if (mIsResetHardActive[reset_id]) {
        mIsResetHardActive[reset_id] = false;
        this->EnableReset(mIsResetHardActive[reset_id], reset_id);
        if (reset_id == 0) {
            re_printf("info","The reset signal presetz is de-asserted.\n");
        } else {
            re_printf("info","The reset signal sys_resetz is de-asserted.\n");
        }
    }
    //Cancel AssertReset command when presetz is active
    if (mIsResetCmdReceive[reset_id]) {
        mResetCmdEvent[reset_id].cancel();
        mResetCmdCancelEvent[reset_id].cancel();
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
    }
}//}}}

///Process reset function when presetz or sys_reset command is active
void ECCCom::HandleResetCmdMethod (const unsigned int reset_id)
{//{{{
    mIsResetCmdActive[reset_id] = true;
    re_printf("info","The model is reset by AssertReset command.\n");
    this->EnableReset(mIsResetCmdActive[reset_id], reset_id);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

///Cancel reset function when presetz or sys_reset command is active
void ECCCom::CancelResetCmdMethod (const unsigned int reset_id)
{//{{{
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info","Reset period is over.\n");
    this->EnableReset(mIsResetCmdActive[reset_id], reset_id); 
}//}}}

///Control ECC disable signals
void ECCCom::ControlECCDisableSignalsMethod (void)
{//{{{
   ControlECCDisableSignals(mECCComFunc->GetECCCTLValue()); 
}//}}}

///Initialize variables at initial state or when reset is active
void ECCCom::Initialize (void)
{//{{{
    for (unsigned int i = 0; i < emBankNo; i++) {
        mReqId[i] = 0;
        mRespId[i] = 0;
        for (unsigned int j = 0; j < emFifoDepth; j++) {
            mErrorAddrInput[i][j] = 0x0;
        }
        mErrorAddrOutput[i] = 0x0;
        mErrorInfo[i].Initialize();
    }
    for (unsigned int i = 0; i < emMaxSignalID; i++) {
        mPCLKErrorOutputVal[i] = false;
    }
    for (unsigned int i = 0; i < (emMaxSignalID-1); i++) {
        for (unsigned int j = 0; j < (emMaxSignalID-1); j++) {
            mSYS_CLKErrorOutputVal[i][j] =  false;
        }
    }
}//}}}

///Initialize output signals
void ECCCom::InitializeOutputSignals (void)
{//{{{
    adederrout.write(false);
    asederrout.write(false);
    dederrout.write(false);
    sederrout.write(false);
    aperrout.write(false);
    bperrout.write(false);
    errovfout.write(false);
    for (unsigned int i = 0; i < emBankNo; i++) {
        adederrout_sys[i]->write(false);
        asederrout_sys[i]->write(false);
        dederrout_sys[i]->write(false);
        sederrout_sys[i]->write(false);
        aperrout_sys[i]->write(false);
        bperrout_sys[i]->write(false);
        addressout_sys[i]->write(0x0);
    }
}//}}}

///Issue an error
void ECCCom::IssueError (const unsigned int bank_no, const unsigned int error_type)
{//{{{
    if ((!mIsResetCmdActive[0])&&(!mIsResetCmdActive[1])&&(!mIsResetHardActive[0])&&(!mIsResetHardActive[1])) {
        if ((bank_no < emBankNo)&&(error_type < emErrorTypeNum)) {// Check whether the arguments are valid
            if ((mPCLKPeriod > 0)&&(mSYS_CLKPeriod > 0)) {
                if (mErrorAddrInput[bank_no][mRespId[bank_no]] != 0x0) {
                    if ((error_type == emAddrECC1BitError)||(error_type == emDataECC1BitError)||
                        (error_type == emParityBitError)||(error_type == emDataECC1BitErrorExtension)) {// Single Error or Parity Bit Error
                        mErrorInfo[bank_no].is_ready = true;
                        switch (error_type) {
                            case emAddrECC1BitError: mErrorInfo[bank_no].addr_ecc_1bit_error_status = true;
                                 break;
                            case emDataECC1BitError: mErrorInfo[bank_no].data_ecc_1bit_error_status = true;
                                 break;
                            case emParityBitError: mErrorInfo[bank_no].parity_bit_error_status = true;
                                 break;
                            default: mErrorInfo[bank_no].data_ecc_1bit_error_ext_status = true;
                                 break;
                        }
                    } else {// Double Error or Address Parity Error
                        mErrorInfo[bank_no].is_ready = true;
                        switch (error_type) {
                            case emAddrECC2BitError: mErrorInfo[bank_no].addr_ecc_2bit_error_status = true;
                                 break;
                            case emDataECC2BitError: mErrorInfo[bank_no].data_ecc_2bit_error_status = true;
                                 break;
                            case emAddrParityErrorRead: mErrorInfo[bank_no].addr_parity_error_read_status = true;
                                 break;
                            case emAddrParityErrorWrite: mErrorInfo[bank_no].addr_parity_error_write_status = true;
                                 break;
                            default: mErrorInfo[bank_no].data_ecc_2bit_error_ext_status = true;
                                 break;
                        }
                    }
                    mErrorInfo[bank_no].error_addr = mErrorAddrInput[bank_no][mRespId[bank_no]];
                    mErrorInputEvent[bank_no].notify(SC_ZERO_TIME);
                } else {
                    re_printf("error","The error detection address must be different from value 0\n");
                }
            } else {
                re_printf("error","Can not issue a error when the clocks are stopped\n");
            }
        } else {
            re_printf("error","The arguments of the IssueError command are invalid\n");
        }
    } else {
        re_printf("warning","Can not issue an error during reset operation");
    }
}//}}}

///Set error detection address
void ECCCom::SetErrorAddr (const unsigned int bank_no, const unsigned int addr_value)
{//{{{
    if ((!mIsResetCmdActive[0])&&(!mIsResetCmdActive[1])&&(!mIsResetHardActive[0])&&(!mIsResetHardActive[1])) {
        if (bank_no < emBankNo) {
            mErrorAddrInput[bank_no][mReqId[bank_no]] = addr_value;
            mIncreaseReqIdEvent[bank_no].notify(SC_ZERO_TIME);
        } else {
            re_printf("error","The argument about memory bank no of the SetErrorAddr command is invalid\n");
        }
    } else {
        re_printf("warning","Can not set the error detection address during reset operation");
    }
}//}}}

///Assert reset by software for presetz, sys_resetz
void ECCCom::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "presetz") {
        if ((!mIsResetHardActive[0])&&(!mIsResetCmdReceive[0])) {
            mIsResetCmdReceive[0] = true;
            re_printf("info","The model will be reset (presetz) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[0].notify(start_time, SC_NS);
            mResetCmdPeriod[0] = period;
        } else {
            re_printf("warning","The software reset of presetz is called in the reset operation of the model. So it is ignored\n");
        }
    } else if (reset_name == "sys_resetz") {
        if ((!mIsResetHardActive[1])&&(!mIsResetCmdReceive[1])) {
            mIsResetCmdReceive[1] = true;
            re_printf("info","The model will be reset (sys_resetz) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[1].notify(start_time, SC_NS);
            mResetCmdPeriod[1] = period;
        } else {
            re_printf("warning","The software reset of sys_resetz is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("error", "The name of reset signal is wrong (presetz or sys_resetz)\n");
    }
}//}}}

///Set clock value and clock unit for pclk, sys_clk
void ECCCom::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "pclk") {
            CheckFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            mPCLKOrgFreq = freq;
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mPCLKPeriod = 0;
            }
        } else if (word_vector[0] == "sys_clk") {
            CheckFreq (mSYS_CLKFreq, mSYS_CLKFreqUnit, freq, unit);
            mSYS_CLKOrgFreq = freq;
            if (mSYS_CLKFreq > 0) {
                mSYS_CLKPeriod = (sc_dt::uint64)(((1/(double)mSYS_CLKFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mSYS_CLKPeriod = 0;
                for (unsigned int i = 0; i < emMaxSignalID; i++) {
                    mPCLKErrorOutputActiveEvent[i].cancel();
                    mPCLKErrorOutputInactiveEvent[i].cancel();
                    mPCLKErrorOutputWriteEvent[i].cancel();
                }
                for (unsigned int i = 0; i < emBankNo; i++) {
                    for (unsigned int j = 0; j < emMaxSignalID-1; j++) {
                        mSYS_CLKErrorOutputActiveEvent[j][i].cancel();
                        mSYS_CLKErrorOutputInactiveEvent[j][i].cancel();
                        mSYS_CLKErrorOutputWriteEvent[j][i].cancel();
                    }
                    mErrorAddrOutputEvent[i].cancel();
                    mErrorInputEvent[i].cancel();
                }
            }
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", word_vector[0].c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Get clock value of pclk, sys_clk
void ECCCom::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "pclk") {
            re_printf("info","pclk frequency is %0.0f %s.\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else if (word_vector[0] == "sys_clk") {
            re_printf("info","sys_clk frequency is %0.0f %s.\n", (double)mSYS_CLKOrgFreq, mSYS_CLKFreqUnit.c_str());
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", word_vector[0].c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Force value to register
void ECCCom::ForceRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    cmd.push_back("force");
    std::ostringstream reg_value_str;
    reg_value_str<<"0x"<<std::hex<<reg_value;
    cmd.push_back(reg_value_str.str());
    mECCComFunc->RegisterHandler(cmd);
}//}}}

///Release forced value after forcing registers
void ECCCom::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    cmd.push_back("release");
    mECCComFunc->RegisterHandler(cmd);
}//}}}

///Write value to registers by software
void ECCCom::WriteRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    std::ostringstream reg_value_str;
    reg_value_str<<"0x"<<std::hex<<reg_value;
    cmd.push_back(reg_value_str.str());
    mECCComFunc->RegisterHandler(cmd);
}//}}}

///Read value of register by software
void ECCCom::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    mECCComFunc->RegisterHandler(cmd);
}//}}}

///List all reigsters name
void ECCCom::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mECCComFunc->RegisterHandler(cmd);
}//}}}

///Dump help message of all parameters or commands
void ECCCom::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"ECCCom_MessageLevel (fatal|error|warning|info)       Set debug message level (default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"ECCCom_DumpRegisterRW (true/false)                   Enable/disable dumping access register (default: false).");
            SC_REPORT_INFO(this->basename(),"ECCCom_DumpFileNameLineNum (true/false)              Select dump information about file name and line number. (default: false).");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"ECCCom_IssueError (bank_no, type)                    Issue an error.");
            SC_REPORT_INFO(this->basename(),"ECCCom_SetErrorAddr (addr_value)                     Set error detection address.");
            SC_REPORT_INFO(this->basename(),"ECCCom_AssertReset (reset_name, start_time, period)  Assert/De-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"ECCCom_SetCLKFreq (clock_name, freq, unit)           Set clock frequency to pclk, sys_clk clock.");
            SC_REPORT_INFO(this->basename(),"ECCCom_GetCLKFreq (clock_name)                       Get clock frequency of pclk, sys_clk clock.");
            SC_REPORT_INFO(this->basename(),"ECCCom_ForceRegister (reg_name, reg_value)           Force a value to a register.");
            SC_REPORT_INFO(this->basename(),"ECCCom_ReleaseRegister (reg_name)                    Release forced value of a register.");
            SC_REPORT_INFO(this->basename(),"ECCCom_WriteRegister (reg_name, reg_value)           Write a value to a register.");
            SC_REPORT_INFO(this->basename(),"ECCCom_ReadRegister (reg_name)                       Read a value from a register.");
            SC_REPORT_INFO(this->basename(),"ECCCom_ListRegister                                  List all registers of the model.");
        } else {
            re_printf("error","The name (%s) of ECCCom_Help argument is wrong (commands or parameters).\n", type.c_str());
        }
    } else {
        re_printf("error","The name (%s) of ECCCom_Help argument is wrong (commands or parameters).\n", type.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info)
void ECCCom::SetMessageLevel (const std::string msg_lv)
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
    mECCComFunc->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mECCComFunc->RegisterHandler(cmd);
}//}}}

///Enable/disable dumping message when users access registers
void ECCCom::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mECCComFunc->RegisterHandler(cmd);
}//}}}

///Enable/disable dump file name and line number
void ECCCom::DumpFileNameLineNum (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpFileNameLineNum");
    SeparateString(cmd, is_enable);
    mECCComFunc->RegisterHandler(cmd);
}//}}}

///Separate some words from s string to store a vector
void ECCCom::SeparateString (std::vector<std::string> &vtr, const std::string msg)
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

///Check frequency value and frequency unit
void ECCCom::CheckFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
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

///Calculate synchronous time with pclk or sys_clk
double ECCCom::CalculateCLKPosEdge (const std::string clock_name, const bool add_period, const double time_point)
{//{{{
    //calcualte the time point of next pos edge of PCLK
    double period_num = 0;
    if (clock_name == "pclk") {
        period_num = time_point / (double)mPCLKPeriod;
    } else {//sys_clk
        period_num = time_point / (double)mSYS_CLKPeriod;
    }
    unsigned int mod_period_num = (unsigned int)period_num;
    if (!add_period) {
        if ((double)mod_period_num < period_num) {
            mod_period_num++;
        }
    } else {
        mod_period_num++;
    }
    double pos_edge_point = 0;
    if (clock_name == "pclk") {
        pos_edge_point = (double)mod_period_num * (double)mPCLKPeriod - time_point;
    } else {//sys_clk
        pos_edge_point = (double)mod_period_num * (double)mSYS_CLKPeriod - time_point;
    }
    return pos_edge_point;
}//}}}

///Get time resolution
void ECCCom::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
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

///Process reset function
void ECCCom::EnableReset (const bool is_active, const unsigned int reset_id)
{//{{{
    if (reset_id == 0) {
        mECCComFunc->EnableReset(is_active, "pclk_domain");
    } else {
        mECCComFunc->EnableReset(is_active, "sys_clk_domain");
    }
    if (is_active) {//Clear output signals
        Initialize();
        InitializeOutputSignals();
        ControlECCDisableSignals(mECCComFunc->GetECCCTLValue());
        for (unsigned int i = 0; i < emMaxSignalID; i++) {
            mPCLKErrorOutputActiveEvent[i].cancel();
            mPCLKErrorOutputInactiveEvent[i].cancel();
            mPCLKErrorOutputWriteEvent[i].cancel(); 
        }
        for (unsigned int i = 0; i < emMaxSignalID-1; i++) {
            for (unsigned int j = 0; j < emBankNo; j++) {
                mSYS_CLKErrorOutputActiveEvent[i][j].cancel();
                mSYS_CLKErrorOutputInactiveEvent[i][j].cancel();
                mSYS_CLKErrorOutputWriteEvent[i][j].cancel();
                mErrorAddrOutputEvent[j].cancel();
            }
        }
        for (unsigned int i = 0; i < emBankNo; i++) {
            mErrorInputEvent[i].cancel();
        }
        mECCDisableSignalsControlEvent.cancel();
    }
}//}}}

///Control ECC disable signals
void ECCCom::ControlECCDisableSignals (const unsigned int eccctl_value)
{//{{{
    aeccdisable.write((bool)((eccctl_value>>2)&0x1));
    asecdisable.write((bool)((eccctl_value>>3)&0x1));
    eccdisable.write((bool)(eccctl_value&0x1));
    secdisable.write((bool)((eccctl_value>>1)&0x1));
}//}}}

///Virtual function of ECCComAgent class
///Control error output signals in pclk domain
void ECCCom::ControlPCLKErrorOutputSignals (const std::string signal_name)
{//{{{
    double current_time = sc_time_stamp().to_double();
    double sys_clk_syn_time = CalculateCLKPosEdge("sys_clk", true, current_time);
    double pclk_syn_time = CalculateCLKPosEdge("pclk", true, current_time + sys_clk_syn_time);
    double spend_time = sys_clk_syn_time + pclk_syn_time;
    if (signal_name == "adederrout") {
        mPCLKErrorOutputActiveEvent[emADEDERROUT].notify(spend_time, mTimeResolutionUnit);
    } else if (signal_name == "asederrout") {
        mPCLKErrorOutputActiveEvent[emASEDERROUT].notify(spend_time, mTimeResolutionUnit);
    } else if (signal_name == "dederrout") {
        mPCLKErrorOutputActiveEvent[emDEDERROUT].notify(spend_time, mTimeResolutionUnit);
    } else if (signal_name == "sederrout") {
        mPCLKErrorOutputActiveEvent[emSEDERROUT].notify(spend_time, mTimeResolutionUnit);
    } else if (signal_name == "aperrout") {
        mPCLKErrorOutputActiveEvent[emAPERROUT].notify(spend_time, mTimeResolutionUnit);
    } else if (signal_name == "bperrout") {
        mPCLKErrorOutputActiveEvent[emBPERROUT].notify(spend_time, mTimeResolutionUnit);
    } else if (signal_name == "errovfout") {
        mPCLKErrorOutputActiveEvent[emERROVFOUT].notify(spend_time, mTimeResolutionUnit);
    } else {
        SC_REPORT_ERROR(this->basename(),"The signal name is invalid");
    }
}//}}}

///Virtual function of ECCComAgent class
///Control error output signals in sys_clk domain
void ECCCom::ControlSYS_CLKErrorOutputSignals (const std::string signal_name, const unsigned int bank_no, const unsigned int error_addr)
{//{{{
    double current_time = sc_time_stamp().to_double();
    double sys_clk_syn_time = CalculateCLKPosEdge("sys_clk", true, current_time);
    double spend_time = sys_clk_syn_time;
    if (signal_name == "adederrout") {
        mErrorAddrOutput[bank_no] = error_addr;
        mErrorAddrOutputEvent[bank_no].notify(spend_time, mTimeResolutionUnit);
        mSYS_CLKErrorOutputActiveEvent[emADEDERROUT][bank_no].notify(spend_time, mTimeResolutionUnit);
    } else if (signal_name == "asederrout") {
        mErrorAddrOutput[bank_no] = error_addr;
        mErrorAddrOutputEvent[bank_no].notify(spend_time, mTimeResolutionUnit);
        mSYS_CLKErrorOutputActiveEvent[emASEDERROUT][bank_no].notify(spend_time, mTimeResolutionUnit);
    } else if (signal_name == "dederrout") {
        mErrorAddrOutput[bank_no] = error_addr;
        mErrorAddrOutputEvent[bank_no].notify(spend_time, mTimeResolutionUnit);
        mSYS_CLKErrorOutputActiveEvent[emDEDERROUT][bank_no].notify(spend_time, mTimeResolutionUnit);
    } else if (signal_name == "sederrout") {
        mErrorAddrOutput[bank_no] = error_addr;
        mErrorAddrOutputEvent[bank_no].notify(spend_time, mTimeResolutionUnit);
        mSYS_CLKErrorOutputActiveEvent[emSEDERROUT][bank_no].notify(spend_time, mTimeResolutionUnit);
    } else if (signal_name == "aperrout") {
        mErrorAddrOutput[bank_no] = error_addr;
        mErrorAddrOutputEvent[bank_no].notify(spend_time, mTimeResolutionUnit);
        mSYS_CLKErrorOutputActiveEvent[emAPERROUT][bank_no].notify(spend_time, mTimeResolutionUnit);
    } else if (signal_name == "bperrout") {
        mErrorAddrOutput[bank_no] = error_addr;
        mErrorAddrOutputEvent[bank_no].notify(spend_time, mTimeResolutionUnit);
        mSYS_CLKErrorOutputActiveEvent[emBPERROUT][bank_no].notify(spend_time, mTimeResolutionUnit);
    } else {
        SC_REPORT_ERROR(this->basename(),"The signal name is invalid");
    }
}//}}}

///Virtual function of ECCComAgent class
///Wait posedge of PCLK to control ECC disable signals
void ECCCom::WaitPCLKPosEdgeToControlECCDisableSignals (void)
{//{{{
    if (CheckClockPeriod("pclk")) {
        double current_time = sc_time_stamp().to_double();
        mECCDisableSignalsControlEvent.notify(CalculateCLKPosEdge("pclk", false, current_time), mTimeResolutionUnit);
    }
}//}}}

///Virtual function of ECCComAgent class
///Check clock period of pclk, sys_clk
bool ECCCom::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool status = false;
    if (clock_name == "pclk") {
        if (mPCLKPeriod > 0) {
            status = true;
        }
    } else {//sys_clk
        if (mSYS_CLKPeriod > 0) {
            status = true;
        }
    }
    if (!status) {
        re_printf("info","%s frequency is zero.\n", clock_name.c_str());
    }
    return status;
}//}}}
// vim600: set foldmethod=marker :
