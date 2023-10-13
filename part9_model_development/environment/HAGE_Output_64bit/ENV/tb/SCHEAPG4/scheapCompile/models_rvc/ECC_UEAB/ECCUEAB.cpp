// ---------------------------------------------------------------------
// $Id: ECCUEAB.cpp,v 1.6 2013/11/15 08:43:52 dungvannguyen Exp $
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

#include "ECCUEAB.h"

///Constructor of ECCUEAB class
ECCUEAB::ECCUEAB (sc_module_name name,
                unsigned int rLatency,
                unsigned int wLatency,
                unsigned int CapDepth,
                unsigned int ERRINTInitial)
    :sc_module(name),
     ECCUEABAgent(),
     BusSlaveBase<32,1>(),
     pclk("pclk"),
     sys_clk("sys_clk"),
     bus_clk("bus_clk"),
     presetz("presetz"),
     sys_resetz("sys_resetz"),
     errovfout("errovfout"),
     adederrout("adederrout"),
     asederrout("asederrout"),
     apeerrout("apeerrout"),
     dederrout("dederrout"),
     sederrout("sederrout")
{//{{{
    //Register input,output signals
    errovfout.initialize(false);
    adederrout.initialize(false);
    asederrout.initialize(false);
    apeerrout.initialize(false);
    dederrout.initialize(false);
    sederrout.initialize(false);
    std::ostringstream port_name;
    for (unsigned int index = 0; index < emSignalInGroupNum; index++) {
        port_name.str("");
        port_name << adederrin_sys << index;
        adederrin_sys[index] = new sc_in<bool>(port_name.str().c_str());
        port_name.str("");
        port_name << asederrin_sys << index;
        asederrin_sys[index] = new sc_in<bool>(port_name.str().c_str());
        port_name.str("");
        port_name << apeerrin_sys << index;
        apeerrin_sys[index] = new sc_in<bool>(port_name.str().c_str());
        port_name.str("");
        port_name << dederrin_sys << index;
        dederrin_sys[index] = new sc_in<bool>(port_name.str().c_str());
        port_name.str("");
        port_name << sederrin_sys << index;
        sederrin_sys[index] = new sc_in<bool>(port_name.str().c_str());
        port_name.str("");
        port_name << addressin_sys << index;
        addressin_sys[index] = new sc_in<unsigned int>(port_name.str().c_str());

        port_name.str("");
        port_name << adederrin_bus << index;
        adederrin_bus[index] = new sc_in<bool>(port_name.str().c_str());
        port_name.str("");
        port_name << asederrin_bus << index;
        asederrin_bus[index] = new sc_in<bool>(port_name.str().c_str());
        port_name.str("");
        port_name << apeerrin_bus << index;
        apeerrin_bus[index] = new sc_in<bool>(port_name.str().c_str());
        port_name.str("");
        port_name << dederrin_bus << index;
        dederrin_bus[index] = new sc_in<bool>(port_name.str().c_str());
        port_name.str("");
        port_name << sederrin_bus << index;
        sederrin_bus[index] = new sc_in<bool>(port_name.str().c_str());
        port_name.str("");
        port_name << addressin_bus << index;
        addressin_bus[index] = new sc_in<unsigned int>(port_name.str().c_str());
    }

    //Initialize variables
    mPCLKPeriod = 0;
    mSYS_CLKPeriod = 0;
    mBUS_CLKPeriod = 0;
    mPCLKFreq = 0;
    mPCLKOrgFreq = 0;
    mSYS_CLKFreq = 0;
    mSYS_CLKOrgFreq = 0;
    mBUS_CLKFreq = 0;
    mBUS_CLKOrgFreq = 0;
    mTimeResolutionValue = 1;
    mPCLKFreqUnit = "Hz"; 
    mSYS_CLKFreqUnit = "Hz";
    mBUS_CLKFreqUnit = "Hz";
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    Initialize();//Intialize variables

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        mResetSignalCurrentValue[reset_id] = true;
        mResetSignalPreviousValue[reset_id] = true;
        mIsResetHardActive[reset_id] = false;
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
        mResetCmdPeriod[reset_id] =  0;
    }
    
    //Bind clock, reset signal and target socket to Bus IF
    setSlaveFreqPort32(&pclk);
    setSlaveResetPort32(&presetz);
    setTargetSocket32("ts");
    ts = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding

    mECCUEABFunc = new ECCUEABFunc((std::string)name, this, CapDepth, ERRINTInitial);
    sc_assert(mECCUEABFunc != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mECCUEABFunc);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0xFFFFFFFF);

    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << pclk;

    SC_METHOD(HandleSYS_CLKSignalMethod);
    dont_initialize();
    sensitive << sys_clk;

    SC_METHOD(HandleBUS_CLKSignalMethod);
    dont_initialize();
    sensitive << bus_clk;
 
    SC_METHOD(HandleECC1BitErrorInputSignalsMethod);
    dont_initialize();
    for (unsigned int index = 0; index < emSignalInGroupNum; index++) {
        sensitive << sederrin_sys[index]->pos();
        sensitive << sederrin_bus[index]->pos();
    }
 
    SC_METHOD(HandleDoubleErrorInputSignalsMethod);
    dont_initialize();
    for (unsigned int index = 0; index < emSignalInGroupNum; index++) {
        sensitive << adederrin_sys[index]->pos();
        sensitive << asederrin_sys[index]->pos();
        sensitive << apeerrin_sys[index]->pos();
        sensitive << dederrin_sys[index]->pos();
        sensitive << adederrin_bus[index]->pos();
        sensitive << asederrin_bus[index]->pos();
        sensitive << apeerrin_bus[index]->pos();
        sensitive << dederrin_bus[index]->pos();
    }

    SC_METHOD(CaptureDoubleErrorMethod);
    dont_initialize();
    sensitive << mDoubleErrorCaptureEvent; 

    for (unsigned int signal_group = 0; signal_group  < emSignalGroupNum; signal_group++) {
        for (unsigned int signal_id = 0; signal_id < emSignalInGroupNum; signal_id++) {
            sc_core::sc_spawn_options opt;
            opt.spawn_method();
            opt.set_sensitivity(&mEachDataECC1BitErrCaptureEvent[signal_group][signal_id]);
            opt.dont_initialize();
            sc_core::sc_spawn(sc_bind(&ECCUEAB::CaptureEachDataECC1BitErrMethod, this, signal_group, signal_id),
                              sc_core::sc_gen_unique_name("CaptureEachDataECC1BitErrMethod"), &opt);
        }
    }

    SC_METHOD(CaptureAllDataECC1BitErrMethod);
    dont_initialize();
    sensitive << mAllDataECC1BitErrCaptureEvent;

    SC_METHOD(ReportAddr2BitErrorMethod);
    dont_initialize();
    for (unsigned int signal_group = 0; signal_group  < emSignalGroupNum; signal_group++) {
        for (unsigned int signal_id = 0; signal_id < emSignalInGroupNum; signal_id++) {
            sensitive << mADEDERROUTReportEvent[signal_group][signal_id];
        }
    }

    SC_METHOD(ReportAddr1BitErrorMethod);
    dont_initialize();
    for (unsigned int signal_group = 0; signal_group  < emSignalGroupNum; signal_group++) {
        for (unsigned int signal_id = 0; signal_id < emSignalInGroupNum; signal_id++) {
            sensitive << mASEDERROUTReportEvent[signal_group][signal_id];
        }
    }

    SC_METHOD(ReportAddrParityErrorMethod);
    dont_initialize();
    for (unsigned int signal_group = 0; signal_group  < emSignalGroupNum; signal_group++) {
        for (unsigned int signal_id = 0; signal_id < emSignalInGroupNum; signal_id++) {
            sensitive << mAPEERROUTReportEvent[signal_group][signal_id];
        }
    }

    SC_METHOD(ReportData2BitErrorMethod);
    dont_initialize();
    for (unsigned int signal_group = 0; signal_group  < emSignalGroupNum; signal_group++) {
        for (unsigned int signal_id = 0; signal_id < emSignalInGroupNum; signal_id++) {
            sensitive << mDEDERROUTReportEvent[signal_group][signal_id];
        }
    }

    SC_METHOD(ReportData1BitErrorMethod);
    dont_initialize();
    for (unsigned int signal_group = 0; signal_group  < emSignalGroupNum; signal_group++) {
        for (unsigned int signal_id = 0; signal_id < emSignalInGroupNum; signal_id++) {
            sensitive << mSEDERROUTReportEvent[signal_group][signal_id];
        }
    }

    SC_METHOD(ReportOverflowSignalMethod);
    dont_initialize();
    for (unsigned int signal_group = 0; signal_group  < emSignalGroupNum; signal_group++) {
        for (unsigned int signal_id = 0; signal_id < emSignalInGroupNum; signal_id++) {
            sensitive << mOverflowSignalReportEvent[signal_group][signal_id];
        }
    }

    for (unsigned int signal_id = 0; signal_id < emOutputErrorSignalNum; signal_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mOutputErrorSignalsClearEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECCUEAB::ClearOutputErrorSignalsMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("ClearOutputErrorSignalsMethod"), &opt);
    }

    SC_METHOD(ClearOverflowSignalMethod);
    dont_initialize();
    sensitive << mOverflowSignalClearEvent;

    for (unsigned int signal_id = 0; signal_id < emOutputErrorSignalNum; signal_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mOutputErrorSignalsWriteEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECCUEAB::WriteOutputErrorSignalsMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("WriteOutputErrorSignalsMethod"), &opt);
    }

    SC_METHOD(WriteOverflowSignalMethod);
    dont_initialize();
    sensitive << mOverflowSignalWriteEvent;

    SC_METHOD(HandlePRESETZSignalMethod);
    sensitive << presetz;

    SC_METHOD(HandleSYS_RESETZSignalMethod);
    sensitive << sys_resetz;

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECCUEAB::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECCUEAB::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECCUEAB::CancelResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }
}//}}}

///Destructor of ECCUEAB class
ECCUEAB::~ECCUEAB (void)
{//{{{
    delete mECCUEABFunc;
    for (unsigned int index = 0; index < emSignalInGroupNum; index++) {
        delete adederrin_sys[index];
        delete asederrin_sys[index];
        delete apeerrin_sys[index];
        delete dederrin_sys[index];
        delete sederrin_sys[index];
        delete addressin_sys[index];
        delete adederrin_bus[index];
        delete asederrin_bus[index];
        delete apeerrin_bus[index];
        delete dederrin_bus[index];
        delete sederrin_bus[index];
        delete addressin_bus[index];
    }
}//}}}

///Handle pclk signal
void ECCUEAB::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = pclk.read();
    SetCLKFreq("pclk", freq_value, "Hz");
}//}}}

///Handle sys_clk signal
void ECCUEAB::HandleSYS_CLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = sys_clk.read();
    SetCLKFreq("sys_clk", freq_value, "Hz");
}//}}}

///Handle bus_clk signal
void ECCUEAB::HandleBUS_CLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = bus_clk.read();
    SetCLKFreq("bus_clk", freq_value, "Hz");
}//}}}

///Handle input signals of ecc 1bit error
void ECCUEAB::HandleECC1BitErrorInputSignalsMethod (void)
{//{{{
    if ((mPCLKPeriod > 0)&&(mSYS_CLKPeriod > 0)&&(mBUS_CLKPeriod > 0)) {
        if ((!mIsResetCmdActive[0])&&(!mIsResetCmdActive[1])&&(!mIsResetHardActive[0])&&(!mIsResetHardActive[1])) {
            for (unsigned int i = 0; i < emSignalInGroupNum; i++) {//Read input signals
                mDataECC1BitErrInfo[0][i].data_ecc_1bit_error_status = (bool)(sederrin_sys[i]->read());
                mDataECC1BitErrInfo[0][i].error_addr = (unsigned int)(addressin_sys[i]->read());
                mDataECC1BitErrInfo[1][i].data_ecc_1bit_error_status = (bool)(sederrin_bus[i]->read());
                mDataECC1BitErrInfo[1][i].error_addr = (unsigned int)(addressin_bus[i]->read());
            }
            for (unsigned int i = 0; i < emSignalGroupNum; i++) {//Check whether the addresses are the same in same signal group
                for (unsigned int j = 0; j < (emSignalInGroupNum - 1); j++) {
                    if ((mDataECC1BitErrInfo[i][j].data_ecc_1bit_error_status)&&(mDataECC1BitErrInfo[i][j+1].data_ecc_1bit_error_status)) {
                        if (mDataECC1BitErrInfo[i][j].error_addr == mDataECC1BitErrInfo[i][j+1].error_addr) {
                            mDataECC1BitErrInfo[i][j+1].data_ecc_1bit_error_status = false;
                        }
                    }
                }
            }
            for (unsigned int i = 0; i < emSignalInGroupNum; i++) {//Check whether the addresses are the same in different signal group
                for (unsigned int j = 0; j < emSignalInGroupNum; j++) {
                    if ((mDataECC1BitErrInfo[0][i].data_ecc_1bit_error_status)&&(mDataECC1BitErrInfo[1][j].data_ecc_1bit_error_status)) {
                        if (mDataECC1BitErrInfo[0][i].error_addr == mDataECC1BitErrInfo[1][j].error_addr) {
                            mDataECC1BitErrInfo[1][j].data_ecc_1bit_error_status = false;
                        }
                    } 
                }
            }
            double current_time = sc_time_stamp().to_double();
            double syn_time[emSignalGroupNum] = {CalculateCLKPosEdge("sys_clk", false, current_time), CalculateCLKPosEdge("bus_clk", false, current_time)};
            double spend_time[emSignalGroupNum] = {syn_time[0] + (double)(3*mSYS_CLKPeriod), syn_time[1] + (double)mBUS_CLKPeriod + (double)(2*mSYS_CLKPeriod)};
            for (unsigned int i = 0; i < emSignalGroupNum; i++) {
                for (unsigned int j = 0; j < emSignalInGroupNum; j++) {
                    if (mDataECC1BitErrInfo[i][j].data_ecc_1bit_error_status) {
                        if (mDataECC1BitErrInfo[i][j].error_addr != 0x0) {
                            mEachDataECC1BitErrCaptureEvent[i][j].notify(spend_time[i], mTimeResolutionUnit);//Capture error information
                        } else {
                            re_printf("error","The detection error address equal to 0\n");
                        }
                    }
                }
            }
        } else {
            re_printf("warning","Can not capture an error during reset operation");
        }
    } else {
        re_printf("warning","Can not capture an error when the clocks are stopped");
    }
}//}}}

///Handle input signals for double error
void ECCUEAB::HandleDoubleErrorInputSignalsMethod (void)
{//{{{
    if ((mPCLKPeriod > 0)&&(mSYS_CLKPeriod > 0)&&(mBUS_CLKPeriod > 0)) {
        if ((!mIsResetCmdActive[0])&&(!mIsResetCmdActive[1])&&(!mIsResetHardActive[0])&&(!mIsResetHardActive[1])) {
            for (unsigned int i = 0; i < emSignalInGroupNum; i++) {
                 mDoubleErrInfo[0][i].addr_ecc_2bit_error_status = (bool)(adederrin_sys[i]->read());
                 mDoubleErrInfo[0][i].addr_ecc_1bit_error_status = (bool)(asederrin_sys[i]->read());
                 mDoubleErrInfo[0][i].addr_parity_error_status = (bool)(apeerrin_sys[i]->read());
                 mDoubleErrInfo[0][i].data_ecc_2bit_error_status = (bool)(dederrin_sys[i]->read());
                if (mDoubleErrInfo[0][i].addr_ecc_2bit_error_status || mDoubleErrInfo[0][i].addr_ecc_1bit_error_status ||
                    mDoubleErrInfo[0][i].addr_parity_error_status || mDoubleErrInfo[0][i].data_ecc_2bit_error_status) {//If DEDs are detected
                    mDoubleErrInfo[0][i].error_addr = (unsigned int)(addressin_sys[i]->read());
                    double current_time = sc_time_stamp().to_double();
                    double sys_clk_syn_time = CalculateCLKPosEdge("sys_clk", false, current_time);
                    double pclk_syn_time = CalculateCLKPosEdge("pclk", true, current_time + sys_clk_syn_time + (double)(3*mSYS_CLKPeriod));
                    double spend_time = sys_clk_syn_time + (double)(3*mSYS_CLKPeriod) + pclk_syn_time + (double)mPCLKPeriod;
                    if (mDoubleErrInfo[0][i].error_addr != 0x0) {
                        if ((mIsCapturingDoubleError == false) && (mECCUEABFunc->CheckCapturedDoubleErrorInfo()) == false) {
                            mIsCapturingDoubleError = true;
                            mDoubleErrorCapturedInfo = mDoubleErrInfo[0][i];
                            mDoubleErrorCaptureEvent.notify(sys_clk_syn_time + (double)(3*mSYS_CLKPeriod), mTimeResolutionUnit);
                        }
                        if (mDoubleErrInfo[0][i].addr_ecc_2bit_error_status) {mADEDERROUTReportEvent[0][i].notify(spend_time, mTimeResolutionUnit);}
                        if (mDoubleErrInfo[0][i].addr_ecc_1bit_error_status) {mASEDERROUTReportEvent[0][i].notify(spend_time, mTimeResolutionUnit);}
                        if (mDoubleErrInfo[0][i].addr_parity_error_status) {mAPEERROUTReportEvent[0][i].notify(spend_time, mTimeResolutionUnit);}
                        if (mDoubleErrInfo[0][i].data_ecc_2bit_error_status) {mDEDERROUTReportEvent[0][i].notify(spend_time, mTimeResolutionUnit);}
                    } else {
                        mDoubleErrInfo[0][i].addr_ecc_2bit_error_status = false;
                        mDoubleErrInfo[0][i].addr_ecc_1bit_error_status = false;
                        mDoubleErrInfo[0][i].addr_parity_error_status = false;
                        mDoubleErrInfo[0][i].data_ecc_2bit_error_status = false;
                        re_printf("error","The detection error address equal to 0\n");
                    }
                }
            }
            for (unsigned int i = 0; i < emSignalInGroupNum; i++) {
                mDoubleErrInfo[1][i].addr_ecc_2bit_error_status = (bool)(adederrin_bus[i]->read());
                mDoubleErrInfo[1][i].addr_ecc_1bit_error_status = (bool)(asederrin_bus[i]->read());
                mDoubleErrInfo[1][i].addr_parity_error_status = (bool)(apeerrin_bus[i]->read());
                mDoubleErrInfo[1][i].data_ecc_2bit_error_status = (bool)(dederrin_bus[i]->read());
                if (mDoubleErrInfo[1][i].addr_ecc_2bit_error_status || mDoubleErrInfo[1][i].addr_ecc_1bit_error_status ||
                    mDoubleErrInfo[1][i].addr_parity_error_status || mDoubleErrInfo[1][i].data_ecc_2bit_error_status) {//If DEDs are detected
                    mDoubleErrInfo[1][i].error_addr = (unsigned int)(addressin_bus[i]->read());
                    double current_time = sc_time_stamp().to_double();
                    double bus_clk_syn_time = CalculateCLKPosEdge("bus_clk", false, current_time);
                    double pclk_syn_time = CalculateCLKPosEdge("pclk", true, current_time + bus_clk_syn_time + (double)mBUS_CLKPeriod + (double)(2*mSYS_CLKPeriod));
                    double spend_time = bus_clk_syn_time + (double)mBUS_CLKPeriod + (double)(2*mSYS_CLKPeriod) + pclk_syn_time + (double)mPCLKPeriod;
                    if (mDoubleErrInfo[1][i].error_addr != 0x0) {
                        if ((mIsCapturingDoubleError == false) && (mECCUEABFunc->CheckCapturedDoubleErrorInfo()) == false) {
                            mIsCapturingDoubleError = true;
                            mDoubleErrorCapturedInfo = mDoubleErrInfo[1][i];
                            mDoubleErrorCaptureEvent.notify(bus_clk_syn_time + (double)mBUS_CLKPeriod + (double)(2*mSYS_CLKPeriod), mTimeResolutionUnit);
                        }
                        if (mDoubleErrInfo[1][i].addr_ecc_2bit_error_status) {mADEDERROUTReportEvent[1][i].notify(spend_time, mTimeResolutionUnit);}
                        if (mDoubleErrInfo[1][i].addr_ecc_1bit_error_status) {mASEDERROUTReportEvent[1][i].notify(spend_time, mTimeResolutionUnit);}
                        if (mDoubleErrInfo[1][i].addr_parity_error_status) {mAPEERROUTReportEvent[1][i].notify(spend_time, mTimeResolutionUnit);}
                        if (mDoubleErrInfo[1][i].data_ecc_2bit_error_status) {mDEDERROUTReportEvent[1][i].notify(spend_time, mTimeResolutionUnit);}
                    } else {
                        mDoubleErrInfo[1][i].addr_ecc_2bit_error_status = false;
                        mDoubleErrInfo[1][i].addr_ecc_1bit_error_status = false;
                        mDoubleErrInfo[1][i].addr_parity_error_status = false;
                        mDoubleErrInfo[1][i].data_ecc_2bit_error_status = false;
                        re_printf("error","The detection error address equal to 0\n");
                    }
                }
            }
            
        } else {
            re_printf("warning","Can not capture an error during reset operation");
        }
    } else {
        re_printf("warning","Can not capture an error when the clocks are stopped");
    }
}//}}}

///Capture double error information
void ECCUEAB::CaptureDoubleErrorMethod (void)
{//{{{
     mIsCapturingDoubleError = false;
     re_printf("info","The information of double error is captured\n");
     mECCUEABFunc->StoreDoubleErrorInfo(mDoubleErrorCapturedInfo);
}//}}}

///Capture each data ecc 1bit error information
void ECCUEAB::CaptureEachDataECC1BitErrMethod (const unsigned int signal_group, const unsigned int signal_id)
{//{{{
    mDataECC1BitErrInputsInfo[signal_group][signal_id] = true;
    mAllDataECC1BitErrCaptureEvent.notify();
}//}}}

///Capture each data ecc 1bit error information
void ECCUEAB::CaptureAllDataECC1BitErrMethod (void)
{//{{{
    bool overflow = false;
    unsigned int store_pos = 0;
    for (unsigned int i = 0; i < emSignalGroupNum; i++) {
        for (unsigned int j = 0; j < emSignalInGroupNum; j++) {
            if (mDataECC1BitErrInputsInfo[i][j]) {
                mECCUEABFunc->CheckCapturedDataECC1BitErrorInfo(mDataECC1BitErrInfo[i][j]);
                if (mDataECC1BitErrInfo[i][j].data_ecc_1bit_error_status) {
                    double current_time = sc_time_stamp().to_double();
                    double pclk_syn_time = CalculateCLKPosEdge("pclk", true, current_time);
                    double spend_time = pclk_syn_time + (double)mPCLKPeriod;
                    mSEDERROUTReportEvent[i][j].notify(spend_time, mTimeResolutionUnit);
                    mECCUEABFunc->CheckOverflow(overflow, store_pos);
                    if (!overflow) {
                        re_printf("info","The information of data ecc 1bit error is captured\n");
                        mECCUEABFunc->StoreDataECC1BitErrorInfo(mDataECC1BitErrInfo[i][j], store_pos);
                    } else {//Overflow occurs
                        re_printf("info","The overflow flag of data ecc 1bit error is set\n");
                        mECCUEABFunc->SetOverFlowStatus();
                        mDataECC1BitErrInfo[i][j].data_ecc_1bit_error_status = false;//Ignore the error if overflow occurs
                        mOverflowSignalReportEvent[i][j].notify(spend_time, mTimeResolutionUnit);
                    }
                }
                mDataECC1BitErrInputsInfo[i][j] = false;
            }
        }
    }
}//}}}

///Report address 2bit error
void ECCUEAB::ReportAddr2BitErrorMethod (void)
{//{{{
    if (mECCUEABFunc->GetERRINTValue("ADEDIE") == 1) {
        mOutputErrorSignalsVal[emADEDERROUT] = true;
        mOutputErrorSignalsWriteEvent[emADEDERROUT].notify();
        mOutputErrorSignalsClearEvent[emADEDERROUT].notify((double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}} 

///Report address 1bit error
void ECCUEAB::ReportAddr1BitErrorMethod (void)
{//{{{
    if (mECCUEABFunc->GetERRINTValue("ASEDIE") == 1) {
        mOutputErrorSignalsVal[emASEDERROUT] = true;
        mOutputErrorSignalsWriteEvent[emASEDERROUT].notify(); 
        mOutputErrorSignalsClearEvent[emASEDERROUT].notify((double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}} 

///Report address parity error
void ECCUEAB::ReportAddrParityErrorMethod (void)
{//{{{
    if (mECCUEABFunc->GetERRINTValue("APEIE") == 1) {
        mOutputErrorSignalsVal[emAPEERROUT] = true;
        mOutputErrorSignalsWriteEvent[emAPEERROUT].notify(); 
        mOutputErrorSignalsClearEvent[emAPEERROUT].notify((double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}} 

///Report data 2bit error
void ECCUEAB::ReportData2BitErrorMethod (void)
{//{{{
    if (mECCUEABFunc->GetERRINTValue("DEDIE") == 1) {
        mOutputErrorSignalsVal[emDEDERROUT] = true;
        mOutputErrorSignalsWriteEvent[emDEDERROUT].notify(); 
        mOutputErrorSignalsClearEvent[emDEDERROUT].notify((double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}} 

///Report data 1bit error
void ECCUEAB::ReportData1BitErrorMethod (void)
{//{{{
    if (mECCUEABFunc->GetERRINTValue("SEDIE") == 1) {
        mOutputErrorSignalsVal[emSEDERROUT] = true;
        mOutputErrorSignalsWriteEvent[emSEDERROUT].notify(); 
        mOutputErrorSignalsClearEvent[emSEDERROUT].notify((double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}} 

///Report error overflow
void ECCUEAB::ReportOverflowSignalMethod (void)
{//{{{
    if (mECCUEABFunc->GetERRINTValue("SEOVFIE") == 1) {
        mOverflowSignalVal = true;
        mOverflowSignalWriteEvent.notify(); 
        mOverflowSignalClearEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}} 

///Clear output error signals
void ECCUEAB::ClearOutputErrorSignalsMethod (const unsigned int signal_id)
{//{{{
    mOutputErrorSignalsVal[signal_id] = false;
    mOutputErrorSignalsWriteEvent[signal_id].notify(); 
}//}}}

///Clear error overflow signal
void ECCUEAB::ClearOverflowSignalMethod (void)
{//{{{
    mOverflowSignalVal = false;
    mOverflowSignalWriteEvent.notify();
}//}}}

///Write the value to output error signals
void ECCUEAB::WriteOutputErrorSignalsMethod (const unsigned int signal_id)
{//{{{
    switch (signal_id) {
        case emADEDERROUT:
            adederrout.write(mOutputErrorSignalsVal[emADEDERROUT]);
            break;
        case emASEDERROUT:
            asederrout.write(mOutputErrorSignalsVal[emASEDERROUT]);
            break;
        case emAPEERROUT:
            apeerrout.write(mOutputErrorSignalsVal[emAPEERROUT]);
            break;
        case emDEDERROUT:
            dederrout.write(mOutputErrorSignalsVal[emDEDERROUT]);
            break;
        default://emSEDERROUT
            sederrout.write(mOutputErrorSignalsVal[emSEDERROUT]);
    }
}//}}} 

///Write the value to output error signals
void ECCUEAB::WriteOverflowSignalMethod (void)
{//{{{
    errovfout.write(mOverflowSignalVal);
}//}}}

///Handle presetz signal
void ECCUEAB::HandlePRESETZSignalMethod (void)
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
void ECCUEAB::HandleSYS_RESETZSignalMethod (void)
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

///Process reset function when presetz or sys_resetz is active
void ECCUEAB::HandleResetHardMethod (const unsigned int reset_id)
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
    // Cancel AssertReset command when sys_resetz is active
    if (mIsResetCmdReceive[reset_id]) {
        mResetCmdEvent[reset_id].cancel();
        mResetCmdCancelEvent[reset_id].cancel();
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
    }
}//}}}

///Process reset function when presetz or sys_resetz command is active
void ECCUEAB::HandleResetCmdMethod (const unsigned int reset_id)
{//{{{
    mIsResetCmdActive[reset_id] = true;
    re_printf("info","The model is reset by AssertReset command.\n");
    this->EnableReset(mIsResetCmdActive[reset_id], reset_id);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

///Cancel reset function when presetz or sys_resetz command is active
void ECCUEAB::CancelResetCmdMethod (const unsigned int reset_id)
{//{{{
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info","Reset period is over.\n");
    this->EnableReset(mIsResetCmdActive[reset_id], reset_id);
}//}}}

///Assert reset by software for presetz, sys_resetz
void ECCUEAB::AssertReset (const std::string reset_name, const double start_time, const double period)
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
        re_printf("error","The name of reset signal is wrong (presetz, sys_resetz)\n");
    }
}//}}}

///Set clock value and clock unit for pclk, sys_clk, bus_clk
void ECCUEAB::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
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
                mDoubleErrorCaptureEvent.cancel();
                mAllDataECC1BitErrCaptureEvent.cancel();
                mOverflowSignalClearEvent.cancel();
                mOverflowSignalWriteEvent.cancel();
                for (unsigned int i = 0; i < emSignalGroupNum; i++) {
                    for (unsigned int j = 0; j < emSignalInGroupNum; j++) {
                        mEachDataECC1BitErrCaptureEvent[i][j].cancel();
                        mADEDERROUTReportEvent[i][j].cancel();
                        mASEDERROUTReportEvent[i][j].cancel();
                        mAPEERROUTReportEvent[i][j].cancel();
                        mDEDERROUTReportEvent[i][j].cancel();
                        mSEDERROUTReportEvent[i][j].cancel();
                        mOverflowSignalReportEvent[i][j].cancel();
                    }
                }
                for (unsigned int i = 0; i < emOutputErrorSignalNum; i++) {
                    mOutputErrorSignalsClearEvent[i].cancel();
                    mOutputErrorSignalsWriteEvent[i].cancel();
                }
            }
        } else if (word_vector[0] == "sys_clk") {
            CheckFreq (mSYS_CLKFreq, mSYS_CLKFreqUnit, freq, unit);
            mSYS_CLKOrgFreq = freq;
            if (mSYS_CLKFreq > 0) {
                mSYS_CLKPeriod = (sc_dt::uint64)(((1/(double)mSYS_CLKFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mSYS_CLKPeriod = 0;
                mDoubleErrorCaptureEvent.cancel();
                mAllDataECC1BitErrCaptureEvent.cancel();
                mOverflowSignalClearEvent.cancel();
                mOverflowSignalWriteEvent.cancel();
                for (unsigned int i = 0; i < emSignalGroupNum; i++) {
                    for (unsigned int j = 0; j < emSignalInGroupNum; j++) {
                        mEachDataECC1BitErrCaptureEvent[i][j].cancel();
                        mADEDERROUTReportEvent[i][j].cancel();
                        mASEDERROUTReportEvent[i][j].cancel();
                        mAPEERROUTReportEvent[i][j].cancel();
                        mDEDERROUTReportEvent[i][j].cancel();
                        mSEDERROUTReportEvent[i][j].cancel();
                        mOverflowSignalReportEvent[i][j].cancel();
                    }
                }
                for (unsigned int i = 0; i < emOutputErrorSignalNum; i++) {
                    mOutputErrorSignalsClearEvent[i].cancel();
                    mOutputErrorSignalsWriteEvent[i].cancel();
                }
            }
        } else if (word_vector[0] == "bus_clk") {
            CheckFreq (mBUS_CLKFreq, mBUS_CLKFreqUnit, freq, unit);
            mBUS_CLKOrgFreq = freq;
            if (mBUS_CLKFreq > 0) {
                mBUS_CLKPeriod = (sc_dt::uint64)(((1/(double)mBUS_CLKFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mBUS_CLKPeriod = 0;
                mDoubleErrorCaptureEvent.cancel();
                mAllDataECC1BitErrCaptureEvent.cancel();
                mOverflowSignalClearEvent.cancel();
                mOverflowSignalWriteEvent.cancel();
                for (unsigned int i = 0; i < emSignalGroupNum; i++) {
                    for (unsigned int j = 0; j < emSignalInGroupNum; j++) {
                        mEachDataECC1BitErrCaptureEvent[i][j].cancel();
                        mADEDERROUTReportEvent[i][j].cancel();
                        mASEDERROUTReportEvent[i][j].cancel();
                        mAPEERROUTReportEvent[i][j].cancel();
                        mDEDERROUTReportEvent[i][j].cancel();
                        mSEDERROUTReportEvent[i][j].cancel();
                        mOverflowSignalReportEvent[i][j].cancel();
                    }
                }
                for (unsigned int i = 0; i < emOutputErrorSignalNum; i++) {
                    mOutputErrorSignalsClearEvent[i].cancel();
                    mOutputErrorSignalsWriteEvent[i].cancel();
                }
            }
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", word_vector[0].c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Get clock value of pclk, sys_clk, bus_clk
void ECCUEAB::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "pclk") {
            re_printf("info","pclk frequency is %0.0f %s.\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else if (word_vector[0] == "sys_clk") {
            re_printf("info","sys_clk frequency is %0.0f %s.\n", (double)mSYS_CLKOrgFreq, mSYS_CLKFreqUnit.c_str());
        } else if (word_vector[0] == "bus_clk") {
            re_printf("info","bus_clk frequency is %0.0f %s.\n", (double)mBUS_CLKOrgFreq, mBUS_CLKFreqUnit.c_str());
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", word_vector[0].c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Force value to register
void ECCUEAB::ForceRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    cmd.push_back("force");
    std::ostringstream reg_value_str;
    reg_value_str<<"0x"<<std::hex<<reg_value;
    cmd.push_back(reg_value_str.str());
    mECCUEABFunc->RegisterHandler(cmd);
}//}}}

///Release forced value after forcing registers
void ECCUEAB::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    cmd.push_back("release");
    mECCUEABFunc->RegisterHandler(cmd);
}//}}}

///Write value to registers by software
void ECCUEAB::WriteRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    std::ostringstream reg_value_str;
    reg_value_str<<"0x"<<std::hex<<reg_value;
    cmd.push_back(reg_value_str.str());
    mECCUEABFunc->RegisterHandler(cmd);
}//}}}

///Read value of register by software
void ECCUEAB::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    mECCUEABFunc->RegisterHandler(cmd);
}//}}}

///List all reigsters name
void ECCUEAB::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mECCUEABFunc->RegisterHandler(cmd);
}//}}}

///Dump help message of all parameters or commands
void ECCUEAB::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"ECCUEAB_MessageLevel (fatal|error|warning|info)       Set debug message level (default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"ECCUEAB_DumpRegisterRW (true/false)                   Enable/disable dumping access register (default: false).");
            SC_REPORT_INFO(this->basename(),"ECCUEAB_DumpFileNameLineNum (true/false)              Select dump information about file name and line number. (default: false).");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"ECCUEAB_AssertReset (reset_name, start_time, period)  Assert/De-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"ECCUEAB_SetCLKFreq (clock_name, freq, unit)           Set clock frequency to pclk, sys_clk, bus_clk clock.");
            SC_REPORT_INFO(this->basename(),"ECCUEAB_GetCLKFreq (clock_name)                       Get clock frequency of pclk, sys_clk, bus_clk clock.");
            SC_REPORT_INFO(this->basename(),"ECCUEAB_ForceRegister (reg_name, reg_value)           Force a value to a register.");
            SC_REPORT_INFO(this->basename(),"ECCUEAB_ReleaseRegister (reg_name)                    Release forced value of a register.");
            SC_REPORT_INFO(this->basename(),"ECCUEAB_WriteRegister (reg_name, reg_value)           Write a value to a register.");
            SC_REPORT_INFO(this->basename(),"ECCUEAB_ReadRegister (reg_name)                       Read a value from a register.");
            SC_REPORT_INFO(this->basename(),"ECCUEAB_ListRegister                                  List all registers of the model.");
        } else {
            re_printf("error","The name (%s) of ECCUEAB_Help argument is wrong (commands or parameters).\n", type.c_str());
        }
    } else {
        re_printf("error","The name (%s) of ECCUEAB_Help argument is wrong (commands or parameters).\n", type.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info)
void ECCUEAB::SetMessageLevel (const std::string msg_lv)
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
    mECCUEABFunc->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mECCUEABFunc->RegisterHandler(cmd);
}//}}}

///Enable/disable dumping message when users access registers
void ECCUEAB::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mECCUEABFunc->RegisterHandler(cmd);
}//}}}

///Enable/disable dump file name and line number
void ECCUEAB::DumpFileNameLineNum (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpFileNameLineNum");
    SeparateString(cmd, is_enable);
    mECCUEABFunc->RegisterHandler(cmd);
}//}}}

///Initialize variables
void ECCUEAB::Initialize (void)
{//{{{
     for (unsigned int i = 0; i < emSignalGroupNum; i++) {
         for (unsigned int j = 0; j < emSignalInGroupNum; j++) {
             mDoubleErrInfo[i][j].Initialize();
             mDataECC1BitErrInfo[i][j].Initialize();
             mDataECC1BitErrInputsInfo[i][j] = false;
         }
     }
     mDoubleErrorCapturedInfo.Initialize();
     mOverflowSignalVal = false;
     for (unsigned int i = 0; i < emOutputErrorSignalNum; i++) {
         mOutputErrorSignalsVal[i] = false;
     }
     mIsCapturingDoubleError = false;
}//}}}

///Initialize output signals
void ECCUEAB::InitializeOutputSignals (void)
{//{{{
    for (unsigned int i = 0; i < emOutputErrorSignalNum; i++) {
        mOutputErrorSignalsVal[i] = false;
        mOutputErrorSignalsClearEvent[i].notify();
    }
    mOverflowSignalVal = false;
    mOverflowSignalClearEvent.notify();
}//}}}

///Separate some words from s string to store a vector
void ECCUEAB::SeparateString (std::vector<std::string> &vtr, const std::string msg)
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
void ECCUEAB::CheckFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
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

///Calculate synchronous time with pclk, sys_clk or bus_clk
double ECCUEAB::CalculateCLKPosEdge (const std::string clock_name, const bool add_period, const double time_point)
{//{{{
    //calcualte the time point of next pos edge of PCLK
    double period_num = 0;
    if (clock_name == "pclk") {
        period_num = time_point / (double)mPCLKPeriod;
    } else if (clock_name == "sys_clk") {
        period_num = time_point / (double)mSYS_CLKPeriod;
    } else {// bus_clk
        period_num = time_point / (double)mBUS_CLKPeriod;
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
    } else if (clock_name == "sys_clk") {
        pos_edge_point = (double)mod_period_num * (double)mSYS_CLKPeriod - time_point;
    } else {// bus_clk
        pos_edge_point = (double)mod_period_num * (double)mBUS_CLKPeriod - time_point;
    }
    return pos_edge_point;
}//}}}

///Get time resolution
void ECCUEAB::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
{//{{{
  if (sc_get_time_resolution() < sc_time(1, SC_PS)) {
      resolution_value = 1000000000000000LL;
      resolution_unit = SC_FS;
  }
  else if (sc_get_time_resolution() < sc_time(1, SC_NS)) {
      resolution_value = 1000000000000LL;
      resolution_unit = SC_PS;
  }
  else if (sc_get_time_resolution() < sc_time(1, SC_US)) {
      resolution_value = 1000000000;
      resolution_unit = SC_NS;
  }
  else if (sc_get_time_resolution() < sc_time(1, SC_MS)) {
      resolution_value = 1000000;
      resolution_unit = SC_US;
  }
  else if (sc_get_time_resolution() < sc_time(1, SC_SEC)) {
      resolution_value = 1000;
      resolution_unit = SC_MS;
  }
  else {
      resolution_value = 1;
      resolution_unit = SC_SEC;
  }
}//}}}

///Process reset function
void ECCUEAB::EnableReset (const bool is_active, const unsigned int reset_id)
{//{{{
    if (reset_id == 0) {
        mECCUEABFunc->EnableReset(is_active, "pclk_domain");
    } else {
        mECCUEABFunc->EnableReset(is_active, "sys_clk_domain");
    }
    if (is_active) {//Clear output signals
        Initialize();
        InitializeOutputSignals();
        mDoubleErrorCaptureEvent.cancel();
        mAllDataECC1BitErrCaptureEvent.cancel();
        for (unsigned int i = 0; i < emSignalGroupNum; i++) {
            for (unsigned int j = 0; j < emSignalInGroupNum; j++) {
                mEachDataECC1BitErrCaptureEvent[i][j].cancel();
                mADEDERROUTReportEvent[i][j].cancel();
                mASEDERROUTReportEvent[i][j].cancel();
                mAPEERROUTReportEvent[i][j].cancel();
                mDEDERROUTReportEvent[i][j].cancel();
                mSEDERROUTReportEvent[i][j].cancel();
                mOverflowSignalReportEvent[i][j].cancel();
            }
        }
        for (unsigned int i = 0; i < emOutputErrorSignalNum; i++) {
            mOutputErrorSignalsClearEvent[i].cancel();
            mOutputErrorSignalsWriteEvent[i].cancel();
        }
        mOverflowSignalClearEvent.cancel();
        mOverflowSignalWriteEvent.cancel();
    }
}//}}}

///Virtual function of ECCUEABAgent class
///Check clock period of pclk, sys_clk, bus_clk
bool ECCUEAB::CheckClockPeriod (std::string clock_name)
{//{{{
    bool status = false;
    if (clock_name == "pclk") {
        if (mPCLKPeriod > 0) {
            status = true;
        }
    } else if (clock_name == "sys_clk") {
        if (mSYS_CLKPeriod > 0) {
            status = true;
        }
    } else {// bus_clk
        if (mBUS_CLKPeriod > 0) {
            status = true;
        }
    }
    if (!status) {
        re_printf("info","%s frequency is zero.\n", clock_name.c_str());
    }
    return status;
}//}}}
// vim600: set foldmethod=marker :
