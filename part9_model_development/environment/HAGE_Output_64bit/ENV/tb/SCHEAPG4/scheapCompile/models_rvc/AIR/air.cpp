// ----------------------------------------------------------------------
// $Id: $
//
// Copyright(c) 2016 - 2017 Renesas System Design Co., Ltd.
// Copyright(c) 2016 - 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ----------------------------------------------------------------------

#include "air.h"
#include "air_core.h"
#ifndef FRACTION_TO_INT
#define FRACTION_TO_INT(d) ((unsigned int)((d - (unsigned long long)(d))*1000000))
#endif

// Constructor of Cair class
Cair::Cair(sc_module_name name):
     vpcl::tlm_tgt_if<32> (name)
    ,pclk("pclk")
    ,presetn("presetn")
    ,ADMPXI0("ADMPXI0")
    ,ADMPXI1("ADMPXI1")
    ,ADMPXI2("ADMPXI2")
    ,ADMPXI3("ADMPXI3")
    ,ADI00("ADI00")
    ,ADI01("ADI01")
    ,ADI02("ADI02")
    ,ADI03("ADI03")
    ,ADI04("ADI04")
    ,ADI10("ADI10")
    ,ADI11("ADI11")
    ,ADI12("ADI12")
    ,ADI13("ADI13")
    ,ADI14("ADI14")
    ,ADI20("ADI20")
    ,ADI21("ADI21")
    ,ADI22("ADI22")
    ,ADI23("ADI23")
    ,ADI24("ADI24")
    ,ADI30("ADI30")
    ,ADI31("ADI31")
    ,ADI32("ADI32")
    ,ADI33("ADI33")
    ,ADI34("ADI34")
    ,ADE0("ADE0")
    ,ADE1("ADE1")
    ,ADE2("ADE2")
    ,ADE3("ADE3")
    ,DSADE00("DSADE00")
    ,DSADE10("DSADE10")
    ,DSADE20("DSADE20")
    ,DSADE11("DSADE11")
    ,DSADE12("DSADE12")
    ,DSADE13("DSADE13")
    ,DSADI00("DSADI00")
    ,DSADI10("DSADI10")
    ,DSADI20("DSADI20")
    ,DSADI11("DSADI11")
    ,DSADI12("DSADI12")
    ,DSADI13("DSADI13")
    ,CADE00("CADE00")
    ,CADI00("CADI00")
    ,ASI0("ASI0")
    ,ASI1("ASI1")
    ,ASI2("ASI2")
    ,ASI3("ASI3")
    ,ASI4("ASI4")
    ,ASI5("ASI5")
    ,ASI6("ASI6")
    ,ASI7("ASI7")
    ,ASI8("ASI8")
    ,ASI9("ASI9")
    ,ASI10("ASI10")
    ,ASI11("ASI11")
    ,ASI12("ASI12")
    ,ASI13("ASI13")
    ,ASI14("ASI14")
    ,ASI15("ASI15")
    ,BFP0("BFP0")
    ,BFP1("BFP1")
    ,BFP2("BFP2")
    ,BFP3("BFP3")
    ,BFP4("BFP4")
    ,BFP5("BFP5")
    ,BFP6("BFP6")
    ,BFP7("BFP7")
    ,BFP8("BFP8")
    ,BFP9("BFP9")
    ,BFP10("BFP10")
    ,BFP11("BFP11")
    ,BFP12("BFP12")
    ,BFP13("BFP13")
    ,BFP14("BFP14")
    ,BFP15("BFP15")
    ,BFP16("BFP16")
    ,BFP17("BFP17")
    ,BFP18("BFP18")
    ,BFP19("BFP19")
    ,BFP20("BFP20")
    ,BFP21("BFP21")
    ,BFP22("BFP22")
    ,BFP23("BFP23")
    ,BFP24("BFP24")
    ,BFP25("BFP25")
    ,BFP26("BFP26")
    ,BFP27("BFP27")
    ,BFP28("BFP28")
    ,BFP29("BFP29")
    ,BFP30("BFP30")
    ,BFP31("BFP31")
    ,BFP32("BFP32")
    ,BFP33("BFP33")
    ,BFP34("BFP34")
    ,BFP35("BFP35")
    ,BFP36("BFP36")
    ,BFP37("BFP37")
    ,BFP38("BFP38")
    ,BFP39("BFP39")
    ,DSADE14("DSADE14")
    ,DSADE15("DSADE15")
    ,DSADE21("DSADE21")
    ,DSADE22("DSADE22")
    ,DSADI14("DSADI14")
    ,DSADI15("DSADI15")
    ,DSADI21("DSADI21")
    ,DSADI22("DSADI22")
    ,BFP40("BFP40")
    ,BFP41("BFP41")
    ,BFP42("BFP42")
    ,BFP43("BFP43")
    ,BFP44("BFP44")
    ,BFP45("BFP45")
    ,BFP46("BFP46")
    ,BFP47("BFP47")
{//{{{
    CommandInit(this->name());
   
    pAirCore = new Cair_core(this->name(), this);
    // Assigned external sc_in to internal sc_in port
    mInputPort[0].bind(ADMPXI0);
    mInputPort[1].bind(ADMPXI1);
    mInputPort[2].bind(ADMPXI2);
    mInputPort[3].bind(ADMPXI3);
    mInputPort[4].bind(ADI00);
    mInputPort[5].bind(ADI01);
    mInputPort[6].bind(ADI02);
    mInputPort[7].bind(ADI03);
    mInputPort[8].bind(ADI04);
    mInputPort[9].bind(ADI10);
    mInputPort[10].bind(ADI11);
    mInputPort[11].bind(ADI12);
    mInputPort[12].bind(ADI13);
    mInputPort[13].bind(ADI14);
    mInputPort[14].bind(ADI20);
    mInputPort[15].bind(ADI21);
    mInputPort[16].bind(ADI22);
    mInputPort[17].bind(ADI23);
    mInputPort[18].bind(ADI24);
    mInputPort[19].bind(ADI30);
    mInputPort[20].bind(ADI31);
    mInputPort[21].bind(ADI32);
    mInputPort[22].bind(ADI33);
    mInputPort[23].bind(ADI34);
    mInputPort[24].bind(ADE0);
    mInputPort[25].bind(ADE1);
    mInputPort[26].bind(ADE2);
    mInputPort[27].bind(ADE3);
    mInputPort[28].bind(DSADE00);
    mInputPort[29].bind(DSADE10);
    mInputPort[30].bind(DSADE20);
    mInputPort[31].bind(DSADE11);
    mInputPort[32].bind(DSADE12);
    mInputPort[33].bind(DSADE13);
    mInputPort[34].bind(DSADI00);
    mInputPort[35].bind(DSADI10);
    mInputPort[36].bind(DSADI20);
    mInputPort[37].bind(DSADI11);
    mInputPort[38].bind(DSADI12);
    mInputPort[39].bind(DSADI13);
    mInputPort[40].bind(CADE00);
    mInputPort[41].bind(CADI00);
    mInputPort[42].bind(ASI0);
    mInputPort[43].bind(ASI1);
    mInputPort[44].bind(ASI2);
    mInputPort[45].bind(ASI3);
    mInputPort[46].bind(ASI4);
    mInputPort[47].bind(ASI5);
    mInputPort[48].bind(ASI6);
    mInputPort[49].bind(ASI7);
    mInputPort[50].bind(ASI8);
    mInputPort[51].bind(ASI9);
    mInputPort[52].bind(ASI10);
    mInputPort[53].bind(ASI11);
    mInputPort[54].bind(ASI12);
    mInputPort[55].bind(ASI13);
    mInputPort[56].bind(ASI14);
    mInputPort[57].bind(ASI15);
    mInputPort[58].bind(BFP0);
    mInputPort[59].bind(BFP1);
    mInputPort[60].bind(BFP2);
    mInputPort[61].bind(BFP3);
    mInputPort[62].bind(BFP4);
    mInputPort[63].bind(BFP5);
    mInputPort[64].bind(BFP6);
    mInputPort[65].bind(BFP7);
    mInputPort[66].bind(BFP8);
    mInputPort[67].bind(BFP9);
    mInputPort[68].bind(BFP10);
    mInputPort[69].bind(BFP11);
    mInputPort[70].bind(BFP12);
    mInputPort[71].bind(BFP13);
    mInputPort[72].bind(BFP14);
    mInputPort[73].bind(BFP15);
    mInputPort[74].bind(BFP16);
    mInputPort[75].bind(BFP17);
    mInputPort[76].bind(BFP18);
    mInputPort[77].bind(BFP19);
    mInputPort[78].bind(BFP20);
    mInputPort[79].bind(BFP21);
    mInputPort[80].bind(BFP22);
    mInputPort[81].bind(BFP23);
    mInputPort[82].bind(BFP24);
    mInputPort[83].bind(BFP25);
    mInputPort[84].bind(BFP26);
    mInputPort[85].bind(BFP27);
    mInputPort[86].bind(BFP28);
    mInputPort[87].bind(BFP29);
    mInputPort[88].bind(BFP30);
    mInputPort[89].bind(BFP31);
    mInputPort[90].bind(BFP32);
    mInputPort[91].bind(BFP33);
    mInputPort[92].bind(BFP34);
    mInputPort[93].bind(BFP35);
    mInputPort[94].bind(BFP36);
    mInputPort[95].bind(BFP37);
    mInputPort[96].bind(BFP38);
    mInputPort[97].bind(BFP39);
    //Supported for E2x_FCC2
    mInputPort[98].bind(DSADE14);
    mInputPort[99].bind(DSADE15);
    mInputPort[100].bind(DSADE21);
    mInputPort[101].bind(DSADE22);
    mInputPort[102].bind(DSADI14);
    mInputPort[103].bind(DSADI15);
    mInputPort[104].bind(DSADI21);
    mInputPort[105].bind(DSADI22);
    mInputPort[106].bind(BFP40);
    mInputPort[107].bind(BFP41);
    mInputPort[108].bind(BFP42);
    mInputPort[109].bind(BFP43);
    mInputPort[110].bind(BFP44);
    mInputPort[111].bind(BFP45);
    mInputPort[112].bind(BFP46);
    mInputPort[113].bind(BFP47);

    mInputPortName[0] = std::string("ADMPXI0");
    mInputPortName[1] = std::string("ADMPXI1");
    mInputPortName[2] = std::string("ADMPXI2");
    mInputPortName[3] = std::string("ADMPXI3");
    mInputPortName[4] = std::string("ADI00");
    mInputPortName[5] = std::string("ADI01");
    mInputPortName[6] = std::string("ADI02");
    mInputPortName[7] = std::string("ADI03");
    mInputPortName[8] = std::string("ADI04");
    mInputPortName[9] = std::string("ADI10");
    mInputPortName[10] = std::string("ADI11");
    mInputPortName[11] = std::string("ADI12");
    mInputPortName[12] = std::string("ADI13");
    mInputPortName[13] = std::string("ADI14");
    mInputPortName[14] = std::string("ADI20");
    mInputPortName[15] = std::string("ADI21");
    mInputPortName[16] = std::string("ADI22");
    mInputPortName[17] = std::string("ADI23");
    mInputPortName[18] = std::string("ADI24");
    mInputPortName[19] = std::string("ADI30");
    mInputPortName[20] = std::string("ADI31");
    mInputPortName[21] = std::string("ADI32");
    mInputPortName[22] = std::string("ADI33");
    mInputPortName[23] = std::string("ADI34");
    mInputPortName[24] = std::string("ADE0");
    mInputPortName[25] = std::string("ADE1");
    mInputPortName[26] = std::string("ADE2");
    mInputPortName[27] = std::string("ADE3");
    mInputPortName[28] = std::string("DSADE00");
    mInputPortName[29] = std::string("DSADE10");
    mInputPortName[30] = std::string("DSADE20");
    mInputPortName[31] = std::string("DSADE11");
    mInputPortName[32] = std::string("DSADE12");
    mInputPortName[33] = std::string("DSADE13");
    mInputPortName[34] = std::string("DSADI00");
    mInputPortName[35] = std::string("DSADI10");
    mInputPortName[36] = std::string("DSADI20");
    mInputPortName[37] = std::string("DSADI11");
    mInputPortName[38] = std::string("DSADI12");
    mInputPortName[39] = std::string("DSADI13");
    mInputPortName[40] = std::string("CADE00");
    mInputPortName[41] = std::string("CADI00");
    mInputPortName[42] = std::string("ASI0");
    mInputPortName[43] = std::string("ASI1");
    mInputPortName[44] = std::string("ASI2");
    mInputPortName[45] = std::string("ASI3");
    mInputPortName[46] = std::string("ASI4");
    mInputPortName[47] = std::string("ASI5");
    mInputPortName[48] = std::string("ASI6");
    mInputPortName[49] = std::string("ASI7");
    mInputPortName[50] = std::string("ASI8");
    mInputPortName[51] = std::string("ASI9");
    mInputPortName[52] = std::string("ASI10");
    mInputPortName[53] = std::string("ASI11");
    mInputPortName[54] = std::string("ASI12");
    mInputPortName[55] = std::string("ASI13");
    mInputPortName[56] = std::string("ASI14");
    mInputPortName[57] = std::string("ASI15");
    mInputPortName[58] = std::string("BFP0");
    mInputPortName[59] = std::string("BFP1");
    mInputPortName[60] = std::string("BFP2");
    mInputPortName[61] = std::string("BFP3");
    mInputPortName[62] = std::string("BFP4");
    mInputPortName[63] = std::string("BFP5");
    mInputPortName[64] = std::string("BFP6");
    mInputPortName[65] = std::string("BFP7");
    mInputPortName[66] = std::string("BFP8");
    mInputPortName[67] = std::string("BFP9");
    mInputPortName[68] = std::string("BFP10");
    mInputPortName[69] = std::string("BFP11");
    mInputPortName[70] = std::string("BFP12");
    mInputPortName[71] = std::string("BFP13");
    mInputPortName[72] = std::string("BFP14");
    mInputPortName[73] = std::string("BFP15");
    mInputPortName[74] = std::string("BFP16");
    mInputPortName[75] = std::string("BFP17");
    mInputPortName[76] = std::string("BFP18");
    mInputPortName[77] = std::string("BFP19");
    mInputPortName[78] = std::string("BFP20");
    mInputPortName[79] = std::string("BFP21");
    mInputPortName[80] = std::string("BFP22");
    mInputPortName[81] = std::string("BFP23");
    mInputPortName[82] = std::string("BFP24");
    mInputPortName[83] = std::string("BFP25");
    mInputPortName[84] = std::string("BFP26");
    mInputPortName[85] = std::string("BFP27");
    mInputPortName[86] = std::string("BFP28");
    mInputPortName[87] = std::string("BFP29");
    mInputPortName[88] = std::string("BFP30");
    mInputPortName[89] = std::string("BFP31");
    mInputPortName[90] = std::string("BFP32");
    mInputPortName[91] = std::string("BFP33");
    mInputPortName[92] = std::string("BFP34");
    mInputPortName[93] = std::string("BFP35");
    mInputPortName[94] = std::string("BFP36");
    mInputPortName[95] = std::string("BFP37");
    mInputPortName[96] = std::string("BFP38");
    mInputPortName[97] = std::string("BFP39");
    //Supported for E2x_FCC2
    mInputPortName[98] = std::string("DSADE14");
    mInputPortName[99] = std::string("DSADE15");
    mInputPortName[100] = std::string("DSADE21");
    mInputPortName[101] = std::string("DSADE22");
    mInputPortName[102] = std::string("DSADI14");
    mInputPortName[103] = std::string("DSADI15");
    mInputPortName[104] = std::string("DSADI21");
    mInputPortName[105] = std::string("DSADI22");
    mInputPortName[106] = std::string("BFP40");
    mInputPortName[107] = std::string("BFP41");
    mInputPortName[108] = std::string("BFP42");
    mInputPortName[109] = std::string("BFP43");
    mInputPortName[110] = std::string("BFP44");
    mInputPortName[111] = std::string("BFP45");
    mInputPortName[112] = std::string("BFP46");
    mInputPortName[113] = std::string("BFP47");
   
    mPortReset = false;
    mCmdReset  = false;
    mIsCmdResetStatus = false;
    mIsInitialize = true;
    mState = emOperateState;
    mPCLKPeriod = 0;
    mResetPeriod = 0;
    // Initialize output ports
    for(unsigned int index = 0; index < emNumOfOutputPort; index++){
      INTAIRINTREQ[index].initialize(0);
      INTAIRDMAREQ[index].initialize(0);
    }
    Initialize();

    for(unsigned int i = 0; i < emNumOfInputPort; i++){
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mInputPort[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cair::ReceiveInputMethod, this, i), sc_core::sc_gen_unique_name("ReceiveInputMethod"), &opt);
    }
    SC_METHOD(WriteINTAIRDMAREQMethod)
    dont_initialize();
    sensitive << mWriteINTAIRDMAREQEvent;

    SC_METHOD(WriteINTAIRINTREQMethod)
    dont_initialize();
    sensitive << mWriteINTAIRINTREQEvent;

    SC_METHOD(PCLKPeriodMethod)
    dont_initialize();
    sensitive << pclk;

    SC_METHOD(PortResetMethod)
    dont_initialize();
    sensitive << presetn;

    SC_METHOD(CmdResetMethod)
    dont_initialize();
    sensitive << mCmdResetEvent;

    SC_METHOD(CancelCmdResetMethod)
    dont_initialize();
    sensitive << mCancelCmdResetEvent;

    SetLatency_TLM (0, true);
}//}}}

// Destructor of Cair class
Cair::~Cair()
{//{{{
    delete pAirCore;
}//}}}

void Cair::Initialize(void)
{//{{{
    for(unsigned int i = 0; i < emNumOfOutputPort; i++){
        mNumDMAReq[i] = 0;
        mNumINTReq[i] = 0;
        mINTOut[i] = false;
        mDMAOut[i] = false;
        mPreINTOut[i] = false;
        mPreDMAOut[i] = false;
    }
}//}}}

void Cair::DumpStatInfo(void)
{//{{{
    printf ("PROFILE(StatInfo): AIR: ############# START OF STATISTIC INFO OF AIR MODEL #######################################\n");
    printf ("PROFILE(StatInfo): AIR: Index of INTAIRINTREQ/INTAIRDMAREQ    Numbers of INTAIRINTREQ      Numbers of INTAIRDMAREQ\n");
    for(unsigned int i=0; i<emNumOfOutputPort; i++){
        std::ostringstream str_tmp;
        str_tmp<<std::setw(38)<<std::left<<i<<std::setw(29)<<std::left<<mNumINTReq[i]<<std::setw(30)<<std::left<<mNumDMAReq[i]<<"\n";
        printf ("PROFILE(StatInfo): AIR: %s", str_tmp.str().c_str());
        mNumDMAReq[i] = 0;
        mNumINTReq[i] = 0;
    }
    printf ("PROFILE(StatInfo): AIR: ############# END OF STATISTIC INFO OF AIR MODEL #########################################\n");
}//}}}

void Cair::PCLKPeriodMethod(void)
{//{{{
    SetCLKfreq("pclk", sc_dt::uint64_to_double(pclk.read()));
}//}}}

void Cair::SetCLKfreq(std::string clk_name, double clk_freq)
{//{{{
    if (clk_name != "pclk") {
        re_printf("warning", "Clock name %s is invalid.\n", clk_name.c_str());
    }
    double clk_period = 0;
    if (clk_freq > 0) {
        clk_period = ((double)emNanoSecond)/clk_freq;
        double time_unit  =  ((double)emNanoSecond)/ GetTimeResolution();
        if (time_unit > clk_period) {
            re_printf("warning", "The %s period is less than 1 unit time of system.\n", clk_name.c_str());
        }
    } else {
        clk_period = 0;
        mWriteINTAIRINTREQEvent.cancel();
        mWriteINTAIRDMAREQEvent.cancel();
    }

    mPCLKPeriod = clk_period;
    SetLatency_TLM(clk_period, false);
    re_printf("info", "The %s is set with a frequency as %lld.%06d\n", clk_name.c_str(), (unsigned long long) clk_freq, FRACTION_TO_INT(clk_freq));
}//}}}

double Cair::GetTimeResolution(void)
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

void Cair::PortResetMethod(void)
{//{{{
    bool is_cancel_assertreset = false;
    if (!presetn.read()) {
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

    // Cancel handleCommand reset when presetn is asserted
    if (is_cancel_assertreset) {
        mCmdResetEvent.cancel();
        mCancelCmdResetEvent.cancel();
        mIsCmdResetStatus = false;
        mCmdReset = false;
    }
}//}}}

void Cair::EnableReset(const bool is_active)
{//{{{
    if (is_active) {
        mState = emResetState;
        mWriteINTAIRINTREQEvent.cancel();
        mWriteINTAIRDMAREQEvent.cancel();
        Initialize();
    } else {
        mState = emOperateState;
    }
    pAirCore->ResetRegister(is_active);
}//}}}

void Cair::tgt_acc(unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t)
{//{{{
    // This function is not used because there are 1 TLM target socket
    // It is implemented to fix 1Team issue
    tgt_acc(trans, t);
}//}}}
void Cair::tgt_acc( tlm::tlm_generic_payload &trans, sc_time &t)
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
        status = pAirCore->ReadRegister((unsigned int)addr, p_data, size, false);
    // Write access
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        status = pAirCore->WriteRegister((unsigned int)addr, p_data, size, false);
    } else {
        status = true;
    }

    trans.set_response_status(tlm::TLM_OK_RESPONSE);
}//}}}

unsigned int Cair::tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans)
{//{{{
    // This function is not used because there are 1 TLM target socket
    // It is implemented to fix 1Team issue
    return tgt_acc_dbg(trans);
}//}}}
unsigned int Cair::tgt_acc_dbg( tlm::tlm_generic_payload &trans)
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
        status = pAirCore->ReadRegister((unsigned int)addr, p_data, size, true);
    // Write access
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        status = pAirCore->WriteRegister((unsigned int)addr, p_data, size, true);
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

void Cair::AssertReset(double delay, double period)
{//{{{
    // if presetn is negated, AIR waits delay time then notifies handleCommand reset event
    if (!(mPortReset || mIsCmdResetStatus)) {
        mIsCmdResetStatus = true;
        re_printf("info", "AIR will reset for %lld.%06d ns after %lld.%06d ns.\n", (unsigned long long) period, FRACTION_TO_INT(period), (unsigned long long) delay, FRACTION_TO_INT(delay));
        mCmdResetEvent.notify(delay, SC_NS);
        mResetPeriod = period;
    } else {
        re_printf("warning", "Reset is in progress.\n");
    }

}//}}}

void Cair::CmdResetMethod(void)
{//{{{
    mCmdReset = true;
    this->EnableReset(mCmdReset);
    mCancelCmdResetEvent.notify(mResetPeriod, SC_NS);
}//}}}

void Cair::CancelCmdResetMethod(void)
{//{{{
    mCmdReset = false;
    mIsCmdResetStatus = false;
    this->EnableReset(mCmdReset);
}//}}}

// Set input port value to pAirCore, notity to output interrupt/DMA request
void Cair::ReceiveInputMethod(const unsigned int i)
{//{{{
    if(mPCLKPeriod == 0){
        return;
    }
    if(mState == emOperateState){
        if(pAirCore->GetInputPort(i) !=  mInputPort[i].read()){
            re_printf("info","Input port %s is %s\n", mInputPortName[i].c_str(), (mInputPort[i].read())?"ACTIVE":"DEACTIVE");
            pAirCore->NotifyInputPortChange(i, mInputPort[i].read()); 
        }
        mWriteINTAIRDMAREQEvent.notify(mPCLKPeriod, SC_NS);
        mWriteINTAIRINTREQEvent.notify(mPCLKPeriod, SC_NS);
    }
}//}}}

//Write DMA request output port
void Cair::WriteINTAIRDMAREQMethod(void)
{//{{{
    for(unsigned int i = 0; i< emNumOfOutputPort; i++){
        if(mDMAOut[i] != mPreDMAOut[i]) {
            re_printf("info","INTAIRDMAREQ[%d] is %s\n", i, mDMAOut[i] ? "ACTIVE" : "DEACTIVE");
            INTAIRDMAREQ[i].write(mDMAOut[i]);
            mPreDMAOut[i] = mDMAOut[i];
        }
    }
}//}}}

//Write interrupt request output port
void Cair::WriteINTAIRINTREQMethod(void)
{//{{{{
    for(unsigned int i = 0; i< emNumOfOutputPort; i++){
        if(mINTOut[i] != mPreINTOut[i]) {
            re_printf("info","INTAIRINTREQ[%d] is %s\n", i, mINTOut[i] ? "ACTIVE" : "DEACTIVE");
            INTAIRINTREQ[i].write(mINTOut[i]);
            mPreINTOut[i] = mINTOut[i];
        }
    }
}//}}}

void Cair::WriteINTPortOut(unsigned int index, bool is_active)
{//{{{
    sc_assert(index < emNumOfOutputPort);
    mINTOut[index] = is_active;
    if (is_active) {
        mNumINTReq[index]++;
    }
}//}}}

void Cair::WriteDMAPortOut(unsigned int index, bool is_active)
{//{{{
    sc_assert(index < emNumOfOutputPort);
    mDMAOut[index] = is_active;
    if (is_active) {
        mNumDMAReq[index]++;
    }
}//}}}

void Cair::SetLatency_TLM(const double pclk_period, const bool is_constructor)
{//{{{ 
    vpcl::tlm_if_tgt_parameter tgt_param = vpcl::tlm_tgt_if<32>::tgt_get_param();
    if(is_constructor){
        tgt_param.fw_req_phase = tlm::END_REQ;
    }
    sc_time new_clock(pclk_period, SC_NS);
    tgt_param.bus_clk = new_clock;
    tgt_param.rd_latency = tgt_param.rd_req_latency + 2 * tgt_param.bus_clk;
    tgt_param.wr_latency = tgt_param.wr_req_latency + 2 * tgt_param.bus_clk;
    vpcl::tlm_tgt_if<32>::tgt_set_param(&tgt_param);
}//}}}

// vim600: set foldmethod=marker :
