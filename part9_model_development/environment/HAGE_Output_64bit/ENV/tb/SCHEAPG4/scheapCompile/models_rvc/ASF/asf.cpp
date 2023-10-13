// ----------------------------------------------------------------------
// $Id: $
//
// Copyright(c) 2016 Renesas System Design Co., Ltd.
// Copyright(c) 2016 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ----------------------------------------------------------------------

#include "asf.h"
#ifndef FRACTION_TO_INT
#define FRACTION_TO_INT(d) ((unsigned int)((d - (unsigned long long)(d))*1000000))
#endif

// Constructor of Casf class
Casf::Casf(sc_module_name name):
     vpcl::tlm_tgt_if<32> (name)
    ,Casf_regif((std::string)name, 32)
    ,pclk("pclk")
    ,preset_n("preset_n")
    ,ASREQ("ASREQ")
    ,DFTAG("DFTAG")
    ,DFDATA("DFDATA")
    ,INT_ACE("INT_ACE")
{//{{{
    CommandInit(this->name());
    Casf_regif::set_instance_name(this->name()); //set hierarchy name

    INT_ACE.initialize(0);
    mDFData = 0;
    mDFTag = 0;

    mIsInitialize = true;
    mRTDBit = 0;
    mSTBit = 0;
    mPortReset = 0;
    mCmdReset = 0;
    mIsCmdResetStatus = false;
    mIsRTDOperate = 0;

    mFreqPCLK = 0;
    mPCLKPeriod = 0;
    mResetPeriod = 0; // handleCommand Reset period time
    mAccSelect = 0;
    mAccCTAG = 0;
    mAccCNT = 0;
    mDataBitNum = 0;

    this->Initialize();

    SC_METHOD(PCLKPeriodMethod)
    dont_initialize();
    sensitive << pclk;

    SC_METHOD(PortResetMethod);
    dont_initialize();
    sensitive << preset_n;

    SC_METHOD(DFTAGInputMethod);
    dont_initialize();
    sensitive << DFTAG;

    SC_METHOD(DFDATAInputMethod);
    dont_initialize();
    sensitive << DFDATA;

    SC_METHOD(AccumulationMethod);
    dont_initialize();
    sensitive << mASREQEvent;

    SC_METHOD(ASREQMethod);
    dont_initialize();
    sensitive << ASREQ.pos();

    SC_METHOD(UpdateDataRegMethod);
    dont_initialize();
    sensitive << mUpdateDRegEvent;

    SC_THREAD(GenINTThread);
    dont_initialize();
    sensitive << mGenINTEvent;

    SC_METHOD(UpdateInterValMethod);
    dont_initialize();
    sensitive << mUpdateInterValEvent;

    SC_METHOD(ResetAccumulationMethod);
    dont_initialize();
    sensitive << mResetAccEvent;

    SC_METHOD(CmdResetMethod);
    dont_initialize();
    sensitive << mCmdResetEvent;

    SC_METHOD(CancelCmdResetMethod);
    dont_initialize();
    sensitive << mCancelCmdResetEvent;

    SC_METHOD(AccumulatorProcessMethod);
    dont_initialize();
    sensitive << mAccEvent;

    SetLatency_TLM(emTgtId, 0, true);

}//}}}

// Destructor of Casf class
Casf::~Casf()
{//{{{
    mINTEnable.clear();
    mChannelEnable.clear();
    mCMPMatch.clear();
}//}}}

void Casf::Initialize(void)
{//{{{
    mRTDBit = false;
    mSTBit = false;
    //mIsCmdResetStatus = false; // 2019.02.25 DucD remove to fix bug of reset #88069

    mAccSelect = 4;
    mAccCTAG = 0;
    mAccCNT = 0;

    mDataBitNum = 0;
    mIsRTDOperate  = false;
    mAccVal.clear();
    mINTEnable.clear();
    mChannelEnable.clear();
    mCMPMatch.clear();
    //mPCLKPeriod = 0;  // 2019.02.18 DucD remove to fix bug of reset #88069

}//}}}

void Casf::cb_CMP_CMP(RegCBstr str)
{//{{{
        std::map<unsigned int, unsigned int>::iterator iterINT; // Pointer of mINTEnable
        std::map<unsigned int, unsigned int>::iterator iterEN;  // Pointer of mChannelEnable
        std::map<unsigned int, unsigned int>::iterator iterCMP; // Pointer of mCMPMatch
        // CMP.ASIIE
        iterINT = mINTEnable.find(str.channel);
        if (iterINT != mINTEnable.end()) {
            iterINT->second = (unsigned int)(*(CMP [str.channel]))["ASIIE"];
        } else {
            mINTEnable.insert(std::pair<unsigned int, unsigned int>(str.channel, (*(CMP [str.channel]))["ASIIE"]));
        }
        // CMP.CHE
        iterEN = mChannelEnable.find(str.channel);
        if (iterEN != mChannelEnable.end()) {
            if ((iterEN->second == 1) && ((*(CMP [str.channel]))["CHE"] == 0)) {
                re_printf("info", "Channel [%d] is disabled.\n", str.channel);
                std::map<unsigned int, unsigned int>::iterator iterAccVal = mAccVal.find(str.channel);; // Pointer of mAccVal
                // Clear accumulator
                if(iterAccVal != mAccVal.end()){
                    iterAccVal->second = 0;
                }
                // Clear intermediate data value
                (*(RTDR [str.channel]))["RTDR"] = 0;
            }
            if ((iterEN->second == 0) && ((*(CMP [str.channel]))["CHE"] == 1)) {
                re_printf("info", "Channel [%d] is enabled.\n", str.channel);
            }
            iterEN->second = (unsigned int)(*(CMP [str.channel]))["CHE"];
        } else {
            mChannelEnable.insert(std::pair<unsigned int, unsigned int>(str.channel, (*(CMP [str.channel]))["CHE"]));
        }
        // CMP.CMP
        iterCMP = mCMPMatch.find(str.channel);
        if (mSTBit && ((str.pre_data & 0x1F) != (str.data & 0x1F))) {
            re_printf("warning", "CMP%d.CMP bit must not be modified during operation (while CTL1.ST = 1).\n", str.channel);
            (*(CMP [str.channel]))["CMP"] = str.pre_data & 0x1F;
        } else {
            if (iterCMP != mCMPMatch.end()) {
                iterCMP->second = (unsigned int)(*(CMP [str.channel]))["CMP"];
            } else {
                mCMPMatch.insert(std::pair<unsigned int, unsigned int>(str.channel, (*(CMP [str.channel]))["CMP"]));
            }
        }
}//}}}

void Casf::cb_RTDR_RTCNT(RegCBstr str)
{//{{{
    if (!mRTDBit) {
        re_printf("warning", "The reading value of register RTDR cannot guarantee.\n");
    }
}//}}}

void Casf::cb_CTL0_CTAG(RegCBstr str)
{//{{{
    if (mSTBit) {
        if (str.pre_data != str.data) {
            re_printf("warning", "Register CTL0 must not be modified during operation (while CTL1.ST = 1).\n");
            (*CTL0) = str.pre_data;
        }
    } else {
        // CTL0.RTD
        mRTDBit = (bool)(*CTL0)["RTD"];
        // CTL0.ACSL
        unsigned int bit_val = (unsigned int)(*CTL0)["ACSL"];
        mAccSelect  = 1 << (bit_val + 2);
        mDataBitNum = 14 + bit_val;
        // CTL0.CTAG
        mAccCTAG = (unsigned int)(*CTL0)["CTAG"];
    }
}//}}}

void Casf::cb_CTL1_ST(RegCBstr str)
{//{{{
    mSTBit = str.data & 0x1;
    
    if (mSTBit) {
        std::map<unsigned int, unsigned int>::iterator iter; // Pointer of mChannelEnable
        for (uint i = 0; i < emChannelNum; i++) {
            iter = mChannelEnable.find(i);
            if ((iter != mChannelEnable.end()) && (iter->second == 1)) { 
               re_printf("info", "Accumulator of channel [%d] is started.\n", i);
            }
        }
    }

    if (str.pre_data == 1 && str.data == 0) {
        mResetAccEvent.notify(); // Reset accumulator + accumulation counter when ST = 0
    }

}//}}}

void Casf::cb_CNT_CNT(RegCBstr str)
{//{{{
    (*CNT)["CNT"] = mAccCNT; // Update CNT register
    if (mRTDBit) {
        re_printf("warning", "The reading value of register CNT cannot guarantee.\n");
    }
}//}}}

void Casf::DFTAGInputMethod(void)
{//{{{
    mDFTag = (unsigned int)DFTAG.read();
}//}}}

void Casf::DFDATAInputMethod(void)
{//{{{
    unsigned int dfdata = (unsigned int)DFDATA.read();
    mDFData = (dfdata & 0x7FFF) >> 3; // get 12-bit value (bit14 to bit3)
}//}}}

void Casf::ASREQMethod(void)
{//{{{
    mASREQEvent.notify(mPCLKPeriod, SC_NS);  
}//}}}

void Casf::AccumulationMethod(void)
{//{{{
    if (mFreqPCLK == 0) {
        return;
    }
    // Check CTL1.ST
    if ((*CTL1)["ST"] == 1) {
        if (mRTDBit) {
            if (mIsRTDOperate) {
                // Start accumulator
                mAccEvent.notify();
                this->Count();
                // Start accumulator if channel is enable
                std::map<unsigned int, unsigned int>::iterator iterEN = mChannelEnable.find(mDFTag);
                if ((iterEN != mChannelEnable.end()) && (iterEN->second == 1)) {
                    if ((*(RTDR [mDFTag]))["RTCNT"] == mAccSelect-1) {
                        // Generate interrupt
                        mGenINTEvent.notify(mPCLKPeriod, SC_NS);
                        // Update Data register
                        mUpdateDRegEvent.notify();
                        // Clear intermediate data value
                        (*(RTDR [mDFTag]))["RTDR"] = 0;
                        (*(RTDR [mDFTag]))["RTCNT"] = 0;
                    } else {
                        mUpdateInterValEvent.notify();
                    }
                }
            }

            if (mDFTag == mAccCTAG) {
                mIsRTDOperate = true;
            }
        } else {
            // Start accumulator
            mAccEvent.notify();
            // Accumulation Counter
            this->DataCond(); // Generate interrupt + update register
            this->Count(); // Count up
        }
    }
    
}//}}}

void Casf::AccumulatorProcessMethod(void)
{//{{{
    std::map<unsigned int, unsigned int>::iterator iter = mChannelEnable.find(mDFTag);
    if ((iter != mChannelEnable.end()) && (iter->second == 1)) {
        std::map<unsigned int, unsigned int>::iterator pAccVal; // Pointer of mAccVal
        pAccVal = mAccVal.find(mDFTag);
        if (pAccVal != mAccVal.end()) {
            pAccVal->second += mDFData;
        } else {
            mAccVal.insert(std::pair<unsigned int, unsigned int>(mDFTag, mDFData));
        }
    }
}//}}}

void Casf::Count(void)
{//{{{
    if (mDFTag == (*CTL0)["CTAG"]) { // Tag match --> count up
        mAccCNT++;
        if (mAccCNT == mAccSelect){
            mAccCNT = 0;
            re_printf("info", "Accumulation clear to : %d.", mAccCNT);
        }
        if ((DumpINTCNTInfo == 1) && (mAccCNT != 0)) {
            re_printf("info", "Accumulation count up : %d.", mAccCNT);
        }
    } else {
        re_printf("warning", "TAG for DFE does not equal CTAG.\n", mDFTag);
    }
}//}}}

void Casf::DataCond(void)
{//{{{
    std::map<unsigned int, unsigned int>::iterator iter = mChannelEnable.find(mDFTag);
    if ((iter != mChannelEnable.end()) && (iter->second == 1)) {
        std::map<unsigned int, unsigned int>::iterator pCMPMatch; // Pointer of mCMPMatch
        pCMPMatch = mCMPMatch.find(mDFTag);
        if (mAccCNT == pCMPMatch->second) { // Accumulation counter equals compare match value
            if (DumpINTCNTInfo) {
                re_printf("info", "Accumulation counter equals compare match value of channel [%d].", mDFTag);
            }
            mGenINTEvent.notify(mPCLKPeriod, SC_NS);
            mUpdateDRegEvent.notify();
        }
    }
}//}}}

void Casf::UpdateDataRegMethod(void)
{//{{{
    // Update data register of channel [mDFTag]
    std::map<unsigned int, unsigned int>::iterator iter;
    iter = mChannelEnable.find(mDFTag);
    (*(DR [mDFTag]))["DR"] = this->ModifyAccData();
    std::map<unsigned int, unsigned int>::iterator pAccVal;
    pAccVal = mAccVal.find(mDFTag);
    pAccVal->second = 0;
}//}}}

unsigned int Casf::ModifyAccData(void)
{//{{{
    unsigned int data_reg_val = 0;
    std::map<unsigned int, unsigned int>::iterator pAccVal;
    pAccVal = mAccVal.find(mDFTag);
    data_reg_val = pAccVal->second << (21 - mDataBitNum);
    return data_reg_val;
}//}}}

void Casf::GenINTThread(void)
{//{{{
    while(true) {
        if (mPortReset || mCmdReset) {
            INT_ACE.write(0);
        } else {
            std::map<unsigned int, unsigned int>::iterator pINTEnable; // Pointer of mINTEnable
            pINTEnable = mINTEnable.find(mDFTag);
            sc_uint<16> pre_int_val = INT_ACE.read();
            if (pINTEnable != mINTEnable.end() && pINTEnable->second == 1) {
                // Generate interrupt of channel [mDFTag]
                pre_int_val = pre_int_val | (sc_uint<16>)(1 << mDFTag);
                INT_ACE.write(pre_int_val);
                if (DumpINTCNTInfo) {
                    re_printf("info", "Interrupt request of channel [%d] assert INT_ACE : 0x%08X.\n", mDFTag, (unsigned int)pre_int_val);
                }
                wait(sc_time(mPCLKPeriod, SC_NS));
                pre_int_val = pre_int_val & (sc_uint<16>)(~(1 << mDFTag));
                INT_ACE.write(pre_int_val);
                if (DumpINTCNTInfo) {
                    re_printf("info", "Interrupt request of channel [%d] de-assert INT_ACE : 0x%08X.\n", mDFTag, (unsigned int)pre_int_val);
                }
            } else {
                // Do nothing
            }
        }
        wait();
    }
}//}}}

void Casf::UpdateInterValMethod(void)
{//{{{
    std::map<unsigned int, unsigned int>::iterator iter = mChannelEnable.find(mDFTag);
    // Update intermediate value of RTDR of channel [mDFTag]
    (*(RTDR [mDFTag]))["RTDR"]  = this->ModifyAccData();
    if (mDFTag == mAccCTAG) {
        (*(RTDR [mDFTag]))["RTCNT"] = mAccCNT ;
    } else {
        (*(RTDR [mDFTag]))["RTCNT"] = (*(RTDR [mDFTag]))["RTCNT"] + 1 ;
    }
}//}}}

void Casf::ResetAccumulationMethod(void)
{//{{{
    mAccCNT = 0;
    mAccVal.clear();
    (*CNT)["CNT"] = 0;
    std::map<unsigned int, unsigned int>::iterator iter;
    for (iter = mChannelEnable.begin(); iter != mChannelEnable.end(); ++iter) {
        (*(RTDR [iter->first])) = 0;
    }
    mIsRTDOperate = false;
}//}}}

void Casf::PCLKPeriodMethod(void)
{//{{{
    SetCLKfreq("pclk", sc_dt::uint64_to_double(pclk.read()));
}//}}}

void Casf::SetCLKfreq(std::string clk_name, double clk_freq)
{//{{{
    if (clk_name != "pclk") {
        re_printf("error", "Clock name %s is invalid.\n", clk_name.c_str());
    }
    double clk_period = 0;
    if (clk_freq <= 0) {
    } else {
        clk_period = (double)(emNanoSecond / clk_freq);
        double time_unit  =  (double)(emNanoSecond / GetTimeResolution());
        if (time_unit > clk_period) {
            re_printf("warning", "The %s period is less than 1 unit time of system.\n", clk_name.c_str());
        }
    }

    mFreqPCLK = clk_freq;
    mPCLKPeriod = clk_period;
    SetLatency_TLM(emTgtId, clk_period, false);
    re_printf("info", "The %s is set with a frequency as %lld.%06d\n", clk_name.c_str(), (unsigned long long) clk_freq, FRACTION_TO_INT(clk_freq));
}//}}}

double Casf::GetTimeResolution(void)
{//{{{
    double tu_value = 0;
    if (sc_get_time_resolution() < sc_time(1, SC_PS)) {
        tu_value = 1000000000000000LL;
    } else if (sc_get_time_resolution() < sc_time(1, SC_NS)) {
        tu_value = 1000000000000LL;
    } else if (sc_get_time_resolution() < sc_time(1, SC_US)) {
        tu_value = 1000000000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_MS)) {
        tu_value = 1000000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)) {
        tu_value = 1000;
    } else {
        tu_value = 1;
    }
    return tu_value;
}//}}}

void Casf::PortResetMethod(void)
{//{{{
    bool is_cancel_assertreset = false;
    if (!preset_n.read()) {
        mPortReset = true;
    } else {
        mPortReset = false;
    }

    if (mPortReset) {
        mIsInitialize = false;
        re_printf("info", "Reset signal is asserted.\n");
        this->EnableReset(mPortReset);
        is_cancel_assertreset = true;
    } else {
        if (mIsInitialize) {
            mIsInitialize = false;
            is_cancel_assertreset = false;
        } else {
            re_printf("info", "Reset signal is negated.\n");
            this->EnableReset(mPortReset);
            is_cancel_assertreset = true;
        }
    }

    // Cancel handleCommand reset when preset_n is asserted
    if (is_cancel_assertreset) {
        mCmdResetEvent.cancel();
        mCancelCmdResetEvent.cancel();
        mIsCmdResetStatus = false;
        mCmdReset = false;
    }
}//}}}

void Casf::EnableReset(const bool is_active)
{//{{{
    if (is_active) {
        // Port
        Initialize();
        mGenINTEvent.notify();
    }
    Casf_regif::EnableReset(is_active);
}//}}}

void Casf::tgt_acc(unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t)
{//{{{
    // get information
    tlm::tlm_command command;
    sc_dt::uint64 addr = 0;
    unsigned char *p_data = NULL;
    unsigned int size = 0;
    bool status = this->tgt_get_gp_attribute(command, addr, p_data, size, trans, false);
    if (!status) {
        trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
        return;
    }
    sc_assert(p_data != NULL);

    // Read access
    if (command == tlm::TLM_READ_COMMAND) {
        memset(p_data, 0, size);
        status = reg_rd((unsigned int)addr, p_data, size);
    // Write access
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        status = reg_wr((unsigned int)addr, p_data, size);
    } else {
        status = true;
    }

    trans.set_response_status(tlm::TLM_OK_RESPONSE);
}//}}}

unsigned int Casf::tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans)
{//{{{
    // get information
    tlm::tlm_command command;
    sc_dt::uint64 addr = 0;
    unsigned char *p_data = NULL;
    unsigned int size = 0;
    bool status = this->tgt_get_gp_attribute(command, addr, p_data, size, trans, true);
    sc_assert(p_data != NULL);
    
    // Read access
    if (command == tlm::TLM_READ_COMMAND) {
        memset(p_data, 0, size);
        status = reg_rd_dbg((unsigned int)addr, p_data, size);
    // Write access
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        status = reg_wr_dbg((unsigned int)addr, p_data, size);
    // No read/write access
    } else {
        status = true;
        size = 0;
    }

    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    if (status) {
        return size;
    } else {
        return 0;
    }
}//}}}

void Casf::DumpStatInfo(void)
{//{{{
    std::map<unsigned int, unsigned int>::iterator pChannelEnMap; // Pointer of mChannelEnable
    printf("[ASF] StartInfo.\n");
    printf("[ASF] ST  : %d.\n", mSTBit);
    printf("[ASF] RTD : %d.\n", mRTDBit);
    for(unsigned int i = 0; i< emChannelNum; i++){
        pChannelEnMap = mChannelEnable.find(i);
        if(pChannelEnMap != mChannelEnable.end()){
            printf("[ASF] Channel : %2d  CHE : %d  RTDR : 0x%08X  DR :  0x%08X   RTCNT : %d\n", pChannelEnMap->first, pChannelEnMap->second, (unsigned int)(*(RTDR)[pChannelEnMap->first])["RTDR"], (unsigned int)(*(DR)[pChannelEnMap->first])["DR"], (unsigned int)(*(RTDR)[pChannelEnMap->first])["RTCNT"]);
        }else{
            printf("[ASF] Channel : %2d  CHE : %d  RTDR : 0x%08X  DR :  0x%08X   RTCNT : %d\n", i, 0, 0, 0, 0);
        }
    }
    printf("[ASF] Accumulation counter : %d\n", mAccCNT);
    printf("[ASF] EndInfo.\n");
}//}}}

void Casf::AssertReset(double delay, double period)
{//{{{
    // if preset_n is negated, ASF waits delay time then notifies handleCommand reset event
    if (!(mPortReset || mIsCmdResetStatus)) {
        mIsCmdResetStatus = true;
        re_printf("info", "ASF will reset for %lld.%06d ns after %lld.%06d ns.\n", (unsigned long long) period, FRACTION_TO_INT(period), (unsigned long long) delay, FRACTION_TO_INT(delay));
        mCmdResetEvent.notify(delay, SC_NS);
        mResetPeriod = period;
    } else {
        re_printf("warning", "Reset is in progress.\n");
    }

}//}}}

void Casf::CmdResetMethod(void)
{//{{{
    mCmdReset = true;
    this->EnableReset(mCmdReset);
    mCancelCmdResetEvent.notify(mResetPeriod, SC_NS);
}//}}}

void Casf::CancelCmdResetMethod(void)
{//{{{
    mCmdReset = false;
    mIsCmdResetStatus = false;
    this->EnableReset(mCmdReset);
}//}}}

void Casf::SetLatency_TLM(const unsigned int id, const double pclk_period, const bool is_constructor)
{//{{{ 
    vpcl::tlm_if_tgt_parameter tgt_param = vpcl::tlm_tgt_if<32>::tgt_get_param(id);
    if(is_constructor){
        tgt_param.fw_req_phase = tlm::END_REQ;
    }
    sc_time new_clock(pclk_period, SC_NS);
    tgt_param.bus_clk = new_clock;
    tgt_param.rd_latency = tgt_param.rd_req_latency + 2 * tgt_param.bus_clk;
    tgt_param.wr_latency = tgt_param.wr_req_latency + 2 * tgt_param.bus_clk;
    vpcl::tlm_tgt_if<32>::tgt_set_param(id, tgt_param);
}//}}}

// vim600: set foldmethod=marker :
