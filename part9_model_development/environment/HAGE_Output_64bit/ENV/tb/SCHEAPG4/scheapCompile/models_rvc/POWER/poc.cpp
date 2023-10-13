// -----------------------------------------------------------------------------
// $Id: poc.cpp,v 1.1.1.1 2012/12/19 03:21:03 lamdangpham Exp $
//
// Copyright(c) 2012 Renesas Electronics Corporation
// Copyright(c) 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------


#include "poc.h"

/// Constructor of POC class
Cpoc::Cpoc(sc_module_name name):   Cgeneral_reset(name)
                                  ,CLK_V00("CLK_V00")
                                  // Input ports
                                  ,CLR_POF("CLR_POF")
                                  // Output ports
                                  ,poradet_n_v50("poradet_n_v50")
                                  ,pocadet_n_v50("pocadet_n_v50")
                                  ,pocdet_n_v50("pocdet_n_v50")
                                  ,det0det_v50("det0det_v50")
                                  ,DET0DET_MSK("DET0DET_MSK")
                                  ,POCADET_N_MSK("POCADET_N_MSK")
                                  ,POCDET_N_MSK("POCDET_N_MSK")
                                  ,REQ_POF("REQ_POF")
{//{{{
    // Initialize handleCommand  
    CommandInit(this->name());
    
    // Initialize variables
    mStartSimFlag = false;
    Initialize();
    
    // Initialize output ports
    poradet_n_v50.initialize(true);
    pocadet_n_v50.initialize(true);
    pocdet_n_v50.initialize(true);
    det0det_v50.initialize(true);
    DET0DET_MSK.initialize(true);
    POCADET_N_MSK.initialize(true);
    POCDET_N_MSK.initialize(true);
    REQ_POF.initialize(false);
    
    // Clock Method
    SC_METHOD(Clk8MMethod);
    dont_initialize();
    sensitive << CLK_V00;
   
    SC_METHOD(PoraProcessMethod);
    dont_initialize();
    sensitive << mWritePoraEvent;
   
    SC_METHOD(PocaProcessMethod);
    dont_initialize();
    sensitive << mWritePocaEvent;
   
    SC_METHOD(CmplProcessMethod);
    dont_initialize();
    sensitive << mWriteCmplEvent;
   
    SC_METHOD(PocProcessMethod);
    dont_initialize();
    sensitive << mWritePocEvent;
    sensitive << pocadet_n_v50;
   
    SC_METHOD(PocreszProcessMethod);
    dont_initialize();
    sensitive << mWritePocEvent;
    sensitive << pocadet_n_v50;
    sensitive << pocdet_n_v50;
    sensitive << det0det_v50;
   
    SC_METHOD(POFControlMethod);
    dont_initialize();
    sensitive << mWriteReqPofEvent;
    sensitive << CLR_POF;
   
    SC_METHOD(PocbProcessMethod);
    dont_initialize();
    sensitive << mWriteDet0Event;
   
    SC_METHOD(StartSimMethod);
}//}}}

// Destructor of Cpoc class
Cpoc::~Cpoc()
{//{{{
}//}}}

// Called by PclkMethod() or SetCLKfreq handleCommand
void Cpoc::SetCLKfreq(const std::string clk_name, const double clk_freq)
{//{{{
    // Clock name must be "CLK_V00"
    if (clk_name == "CLK_V00") {
        // The frequency should be greater than 0
        if (clk_freq > 0) {
            // Calculate the clock period to check the resolution
            double clk_period = (double)(emNanoSecond / clk_freq);
            double time_unit = (double)(emNanoSecond / Cgeneral_reset::GetTimeResolution());
            if (time_unit > clk_period) {
                re_printf("warning","The CLK_V00 period is less than 1 unit time of system.\n");
                return;
            } else {
                Tpof = (clk_period*(emCellStageNumber + emSyncDepth));
                re_printf("info","CLK_V00 is set with a frequency as %f.\n", clk_freq);
            }
        } else {
            re_printf("error","CLK_V00 must be greater than 0.\n");
        }
    } else {
        re_printf("warning","Clock name %s is invalid.\n", clk_name.c_str());
    }
}//}}}

void Cpoc::Clk8MMethod(void)
{//{{{ 
    if(Cgeneral_reset::IsResetPeriod()) {
        return;
    }
    SetCLKfreq("CLK_V00",(double)(CLK_V00.read()));
}//}}}

void Cpoc::AssertReset(const double delay, const double period)
{//{{{
    // Call assert reset function of Cgeneral_reset class
    Cgeneral_reset::Com_AssertReset(delay, period);
}//}}}

void Cpoc::EnableReset(const bool is_active)
{//{{{
    if (is_active) {
        // Initialize all variables
        Initialize();

        // Cancel all events
        mWritePocEvent.cancel();
        mWritePoraEvent.cancel_all();
        mWritePocaEvent.cancel_all();
        mWriteCmplEvent.cancel_all();
        mWriteDet0Event.cancel_all();
        mWriteReqPofEvent.cancel_all();

        // Set default value to all output ports
        mWritePocEvent.notify();
        mWritePoraEvent.notify(0, SC_NS);
        mWritePocaEvent.notify(0, SC_NS);
        mWriteDet0Event.notify(0, SC_NS);
        mWriteReqPofEvent.notify(0, SC_NS);
    }
}//}}}

void Cpoc::Initialize(void)
{//{{{
    // Initialize variables
    mVCCDetectAmount = 0;
    mVCCADetectAmount = 0;
    mReqPofVal = false;
    mCmplVal = true;
    Tpof = 0;
    mVccPoraQueue.clear(); 
    mVccPocaQueue.clear(); 
    mVccCmplQueue.clear(); 
    mVccaPocbQueue.clear(); 

    // Initialize handleCommand variable
    mVCC   = 5.0;
    mVCCA  = 5.0;
    mRefPOCA = 2.8;
    mRefCMPL = 2.8;
    mRefPOCB = 2.8;
    mSELDETMODE = true;
    mTpora = 1000;
    mTpoca = 1500;
    mTcmpl = 1000;
    mTpocb = 1000;
}//}}}

double Cpoc::GetDetectVolt(std::vector<double> &det_queue, double det_volt)
{//{{{
    double volt = det_volt;
    if (det_queue.size() != 0) {
        volt = det_queue.front();
        det_queue.erase(det_queue.begin());
    }
    return volt;
}//}}}

void Cpoc::StartSimMethod(void)
{//{{{
    if (!mStartSimFlag) {
        mStartSimFlag = true;
        // Update default output ports when start simulation
        // Update with default VCC
        mVccPoraQueue.push_back(VCC);
        mVccPocaQueue.push_back(VCC);
        mVccCmplQueue.push_back(VCC);
        mWritePoraEvent.notify(mTpora,SC_NS);  // Schedule to update poradet_n_v50 after latency time Tpora
        mWritePocaEvent.notify(mTpora + mTpoca,SC_NS);  // Schedule to update pocadet_n_v50 after latency time Tpora + Tpoca
        mWriteCmplEvent.notify(mTcmpl,SC_NS);  // Schedule to update cmplout after latency time Tcmpl
        // Update with default VCCA
        mVccaPocbQueue.push_back(VCCA);
        mWriteDet0Event.notify(mTpocb,SC_NS);  // Schedule to update det0det_v50 after latency time Tpocb
        // Update with default SELDETMODE
        mWritePocEvent.notify();
    }
}//}}}

void Cpoc::PoraProcessMethod(void) 
{//{{{
    if (Cgeneral_reset::IsResetPeriod()) {
        poradet_n_v50.write(true);
        return;
    } else {
        double det_volt = GetDetectVolt (mVccPoraQueue, mVCC);
        // Compare VCC with RefPOCA then write to poradet_n_v50
        poradet_n_v50.write(!(det_volt < mRefPOCA));
        DumpVoltageInfo("VCC", "RefPOCA", mRefPOCA, det_volt);
    }
}//}}}

void Cpoc::PocaProcessMethod(void) 
{//{{{
    if (Cgeneral_reset::IsResetPeriod()) {
        pocadet_n_v50.write(true);
        return;
    } else {
        double det_volt = GetDetectVolt (mVccPocaQueue, mVCC);
        // Compare VCC with RefPOCA then write to pocadet_n_v50
        pocadet_n_v50.write(!(det_volt < mRefPOCA));
    }
}//}}}

void Cpoc::CmplProcessMethod(void) 
{//{{{
    if (Cgeneral_reset::IsResetPeriod()) {
        return;
    } else {
        double det_volt = GetDetectVolt (mVccCmplQueue, mVCC);
        // Compare VCC with RefCMPL then update mCmplVal
        mCmplVal = !(det_volt < mRefCMPL);
        mWritePocEvent.notify(); 
        DumpVoltageInfo("VCC", "RefCMPL", mRefCMPL, det_volt);
    }
}//}}}

void Cpoc::PocProcessMethod(void) 
{//{{{
    if (Cgeneral_reset::IsResetPeriod()) {
        pocdet_n_v50.write(true);
        return;
    } else {
        // Update pocdet_n_v50 
        pocdet_n_v50.write((pocadet_n_v50.read()) && (mCmplVal));
    }
}//}}}

void Cpoc::PocreszProcessMethod(void) 
{//{{{
    if (Cgeneral_reset::IsResetPeriod()) {
        DET0DET_MSK.write(true);
        POCADET_N_MSK.write(true);
        POCDET_N_MSK.write(true);
        return;
    } else {
        bool pocresz = true;
        if (POCOFFMODE) {
            DET0DET_MSK.write(true);
            POCADET_N_MSK.write(true);
            POCDET_N_MSK.write(true);
        } else {
            DET0DET_MSK.write(det0det_v50.read());
            POCADET_N_MSK.write(pocadet_n_v50.read());
            POCDET_N_MSK.write(pocdet_n_v50.read());
            pocresz = (mSELDETMODE)? (pocdet_n_v50.read() & det0det_v50.read()):pocadet_n_v50.read();
        }
        if (pocresz) {
            if (!mReqPofVal) {
                mReqPofVal = true;
                mWriteReqPofEvent.notify(Tpof,SC_NS);  // Schedule to update REQ_POF after latency time Tpof when POCRESZ is changed from 0 to 1
            }
        } else {
            mReqPofVal = false;
            mWriteReqPofEvent.notify(0,SC_NS);     // Clear REQ_POF immediately when POCRESZ = false
        }
    }
}//}}}

void Cpoc::POFControlMethod(void) 
{//{{{
    if (Cgeneral_reset::IsResetPeriod()) {
        REQ_POF.write(false);
        return;
    } else {
        if (CLR_POF.read()) {
            mReqPofVal = false;     // Clear REQ_POF when CLR_POF = true
        } 
        REQ_POF.write(mReqPofVal);
    }
}//}}}

void Cpoc::PocbProcessMethod(void) 
{//{{{
    if (Cgeneral_reset::IsResetPeriod()) {
        det0det_v50.write(true);
        return;
    } else {
        double det_volt = GetDetectVolt(mVccaPocbQueue, mVCCA);
        // Compare VCCA with RefPOCB then write to det0det_v50
        det0det_v50.write(!(det_volt < mRefPOCB));
        DumpVoltageInfo("VCCA","RefPOCB", mRefPOCB, det_volt);
    }
}//}}}

std::string Cpoc::CommandCB(const std::vector<std::string>& args) 
{//{{{
    std::vector<std::string> _args = args;
    std::string ret = "";
    if ((int)_args.size() == 2) {
        if (_args[0] == "VCC") {
            if (VCC != mVCC) {
                mVCC = VCC;
                if (mStartSimFlag) {
                    mVCCDetectAmount++; // Increase number of VCC detection
                    mVccPoraQueue.push_back(VCC);
                    mVccPocaQueue.push_back(VCC);
                    mVccCmplQueue.push_back(VCC);
                    mWritePoraEvent.notify(mTpora,SC_NS);  // Schedule to update poradet_n_v50 after latency time Tpora
                    mWritePocaEvent.notify(mTpora + mTpoca,SC_NS);  // Schedule to update pocadet_n_v50 after latency time Tpora + Tpoca
                    mWriteCmplEvent.notify(mTcmpl,SC_NS);  // Schedule to update cmplout after latency time Tcmpl
                }
            }
        } else if (_args[0] == "VCCA") {
            if (VCCA != mVCCA) {
                mVCCA = VCCA;
                if (mStartSimFlag) {
                    mVCCADetectAmount++; // Increase number of VCCA detection
                    mVccaPocbQueue.push_back(VCCA);
                    mWriteDet0Event.notify(mTpocb,SC_NS);  // Schedule to update det0det_v50 after latency time Tpocb
                }
            }
        } else if (_args[0] == "POCOFFMODE") {
            if (mStartSimFlag) {
                mWritePocEvent.notify();
            }
        } else if (_args[0] == "SELDETMODE") {
            if (!mStartSimFlag) {
                mSELDETMODE = SELDETMODE;
            } else {
                re_printf("warning","Parameter SELDETMODE can not be updated after start simulation.\n");
                SELDETMODE = mSELDETMODE;
            }
        } else if (_args[0] == "RefPOCA") {
            CheckParam(_args[0], RefPOCA, mRefPOCA);
        } else if (_args[0] == "RefCMPL") {
            CheckParam(_args[0], RefCMPL, mRefCMPL);
        } else if (_args[0] == "RefPOCB") {
            CheckParam(_args[0], RefPOCB, mRefPOCB);
        } else if (_args[0] == "Tpora") {
            CheckParam(_args[0], Tpora, mTpora);
        } else if (_args[0] == "Tpoca") {
            CheckParam(_args[0], Tpoca, mTpoca);
        } else if (_args[0] == "Tcmpl") {
            CheckParam(_args[0], Tcmpl, mTcmpl);
        } else if (_args[0] == "Tpocb") {
            CheckParam(_args[0], Tpocb, mTpocb);
        }
    }
    return ret;
}//}}}

void Cpoc::CheckParam(std::string param_name, double &param, double &backup)
{//{{{
    if (!mStartSimFlag) {
        if (param < 0) {
            re_printf("warning","Reference voltage %s must be greater or equal to 0.\n", param_name.c_str());
            param = backup;
        } else {
            backup = param;
        }
    } else {
        re_printf("warning","Parameter %s can not be updated after start simulation.\n", param_name.c_str());
        param = backup;
    }
}//}}}

void Cpoc::CheckParam(std::string param_name, unsigned int &param, double &backup)
{//{{{
    if (!mStartSimFlag) {
        backup = (double) param;
    } else {
        re_printf("warning","Parameter %s can not be updated after start simulation.\n", param_name.c_str());
        param = (unsigned int) backup;
    }
}//}}}

void Cpoc::DumpStatInfo(void)
{//{{{
    const char *type = "StatInfo";
    DumpInfo(type, "Info [%20dns] (%10s):\n", (unsigned int) ((sc_time_stamp().value())*emNanoSecond/GetTimeResolution()), this->name());
    // Dump statistical information about voltage detection activity
    DumpInfo(type, "POC voltage level detection information:\n");
    DumpInfo(type, "   Total VCC   detection: %d time(s)\n", mVCCDetectAmount);
    DumpInfo(type, "   Total VCCA  detection: %d time(s)\n", mVCCADetectAmount);
    DumpInfo(type, "EndInfo.\n");
    // Reset statistical amount counter
    mVCCDetectAmount = 0;
    mVCCADetectAmount = 0;
}//}}}

void Cpoc::DumpInfo(const char *type, const char *message, ...)
{//{{{
    if ((message != NULL) && (type != NULL)) {
        printf ("PROFILE(%s): POC: ", type);
        // Print body
        va_list argptr;
        va_start(argptr, message);
        vprintf(message, argptr);
        va_end(argptr);
    }
}//}}}

void Cpoc::DumpVoltageInfo(std::string voltage, std::string refname, double refvolt, double detvolt)
{//{{{
    if (EnableVoltageInfo) {
        const char *type = "VoltageInfo";
        // Dump message when POC detect the changing of monitored voltage level 
        DumpInfo(type, "Info [%20dns] (%10s):\n", (unsigned int) ((sc_time_stamp().value())*emNanoSecond/GetTimeResolution()), this->name());
        DumpInfo(type, "POC model detects the voltage level %s:\n", voltage.c_str());
        DumpInfo(type, "  Time: %dns\n", (unsigned int) ((sc_time_stamp().value())*emNanoSecond/GetTimeResolution()));
        DumpInfo(type, "  Detection information:\n");
        DumpInfo(type, "    . Reference voltage level %7s = %f\n", refname.c_str(), refvolt);
        DumpInfo(type, "    . Detected voltage level          = %f\n", detvolt);
        DumpInfo(type, "EndInfo.\n");
    }
}//}}}

// vim600: set foldmethod=marker :
