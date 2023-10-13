// ---------------------------------------------------------------------
// $Id: ATU5_TIMERB_Func.cpp 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "ATU5_TIMERB_Func.h"
#include "ATU5_TIMERB.h"

/*********************************
// Function     : ATU5_TIMERB_Func
// Description  : Constructor of ATU5_TIMERB_Func class
// Parameter    :
//      name            Module name
//      Parent          Indicate base class
// Return value : None
**********************************/
ATU5_TIMERB_Func::ATU5_TIMERB_Func(std::string name, ATU5_TIMERB *ATU5_TIB_ptr):
                             Catu5_timerb_regif(name,32)
{//{{{
    assert(ATU5_TIB_ptr != NULL);
    mATU5_TIB = ATU5_TIB_ptr;
    mATU5_TIB->CommandInit(name);
    Catu5_timerb_regif::set_instance_name(name);
    mModuleName = name;
    mIsCMFB6M = false;
    for (unsigned int i = 0; i < 24; i++) {
        mIsProhibitRewriteOCRB[i] = false;
        mIsProhibitRewriteTEPCFB[i] = false;
    }
}//}}}

/*********************************
// Function     : ~ATU5_TIMERB_Func
// Description  : Destructor of ATU5_TIMERB_Func class
// Parameter    : None
// Return value : None
**********************************/
ATU5_TIMERB_Func::~ATU5_TIMERB_Func (void)
{//{{{
}//}}}

/*********************************
// Function     : read
//                - virtual function in BusSlaveFuncIf
// Description  : Access to read value of Dummy Peripheral's registers
// Parameter    :
//      offsetAddress       Register's address offset
//      trans               Transaction
//      t                   Access time
//      debug               Debug access mode
// Return value : None
**********************************/
///Virtual function of BusSlaveFuncIf class
void ATU5_TIMERB_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = (unsigned int)(trans.get_address()-0x400)&0x1FF;
    PreReadCall(addr);
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (debug == false) {
        reg_rd((unsigned int)(trans.get_address()),trans.get_data_ptr(),trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)(trans.get_address()),trans.get_data_ptr(),trans.get_data_length());
    }
}//}}}

/*********************************
// Function     : write
//                - virtual function in BusSlaveFuncIf
// Description  : Access to write value to Dummy Master's registers
// Parameter    :
//      offsetAddress       Register's address offset
//      trans               Transaction
//      t                   Access time
//      debug               Debug access mode
// Return value : None
**********************************/
void ATU5_TIMERB_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = (unsigned int)(trans.get_address()-0x400)&0x1FF;
    if (GetWriteCondition(addr)) {
        if (debug == false) {
            reg_wr((unsigned int)(trans.get_address()),trans.get_data_ptr(),trans.get_data_length());
        } else {
            reg_wr_dbg((unsigned int)(trans.get_address()),trans.get_data_ptr(),trans.get_data_length());
        }
    }
}//}}}

/*********************************
// Function     : PreReadCall
// Description  : Update register value before reading
// Parameter    :
//      addr            register address offset
// Return value : None
**********************************/
void ATU5_TIMERB_Func::PreReadCall (const unsigned int addr)
{//{{{
    if ((addr == 0x40)&&(mATU5_TIB->IsTCNTB0Running(1))) { //Read TCNTB0S1
        SetTCNTB0(1, mATU5_TIB->GetCounterValue("TCNTB0", 1));
    } else if ((addr == 0x60)&&(mATU5_TIB->IsTCNTB0Running(2))) { //Read TCNTB0S2
        SetTCNTB0(2, mATU5_TIB->GetCounterValue("TCNTB0", 2));
    } else if ((addr == 0x80)&&(mATU5_TIB->IsTCNTB0Running(0))) { //Read TCNTB0
        SetTCNTB0(0, mATU5_TIB->GetCounterValue("TCNTB0", 0));
    } else if (addr == 0x108) { //Read TCNTB2
        SetTCNTB2(mATU5_TIB->GetCounterValue("TCNTB2", 0));
    } else if ((addr == 0x1C0)) { //Read DICRB0
        if (GetChannelSel() == 0) {
            (*DICRB0) = (unsigned int)(*MIICRB0);
        } else if (GetChannelSel() == 1) {
            (*DICRB0) = (unsigned int)(*ICRB0S1);
        } else if (GetChannelSel() == 2) {
            (*DICRB0) = (unsigned int)(*ICRB0S2);
        }
    } else if ((addr == 0x84)) { //Read ICRB0
        if (GetChannelSel() == 0) {
            (*ICRB0) = (unsigned int)(*MIICRB0);
        } else if (GetChannelSel() == 1) {
            (*ICRB0) = (unsigned int)(*ICRB0S1);
        } else if (GetChannelSel() == 2) {
            (*ICRB0) = (unsigned int)(*ICRB0S2);
        }
    } else if (addr == 0x188) { //Read TCNTB5
        SetTCNTB5(mATU5_TIB->GetCounterValue("TCNTB5", 0));
    }
}//}}}

/*********************************
// Function     : RegisterHandler
// Description  : Set parameters or commands to Catu5_timerb_regif class
// Parameter    :
//      args            Argument of parameter/command
// Return value : None
**********************************/
void ATU5_TIMERB_Func::RegisterHandler (const std::vector<std::string> &args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {//Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    } else {
        if (args[1] == "*") {//Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase(cmd.begin()+1);
                cmd.insert(cmd.begin()+1, reg_p->name());
                msg_return = Catu5_timerb_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Catu5_timerb_regif class
            msg_return = Catu5_timerb_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/*********************************
// Function     : EnableReset
// Description  : Reset ATU5_TIMERB's registers
// Parameter    :
//      is_active           Active state of reset
// Return value : Register value
**********************************/
void ATU5_TIMERB_Func::EnableReset (const bool is_active)
{//{{{
    Catu5_timerb_regif::EnableReset(is_active);
    mIsCMFB6M = false;
    for (unsigned int i = 0; i < 24; i++) {
        mIsProhibitRewriteOCRB[i] = false;
        mIsProhibitRewriteTEPCFB[i] = false;
    }
}//}}}

/*********************************
// Function     : GetCLKBusSel
// Description  : Return Clock bus selection
// Parameter    : None
// Return value : Clock bus selection
**********************************/
unsigned int ATU5_TIMERB_Func::GetCLKBusSel (void)
{//{{{
    return (unsigned int)(*TCRB)["CKSELB"];
}//}}}

/*********************************
// Function     : GetChannelSel
// Description  : Return Timer B channel selection
// Parameter    : None
// Return value : Clock bus selection
**********************************/
unsigned int ATU5_TIMERB_Func::GetChannelSel (void)
{//{{{
    unsigned int ret = (unsigned int)(*CHSELBR)["CHSELB"];
    if (ret == 0x3) {
        ret = 0;
    }
    return ret;
}//}}}

/*********************************
// Function     : GetTCNTB0
// Description  : Return TCNTB0 register value
// Parameter    :
//      block_num           Block number 0 -> 2
// Return value : TCNTB0 value
**********************************/
unsigned int ATU5_TIMERB_Func::GetTCNTB0 (const unsigned int block_num)
{//{{{
    if (block_num == 2) {
        return (unsigned int)(*TCNTB0S2);
    } else if (block_num == 1) {
        return (unsigned int)(*TCNTB0S1);
    } else {
        return (unsigned int)(*TCNTB0);
    }
}//}}}

/*********************************
// Function     : GetTCNTB1
// Description  : Return TCNTB1 register value
// Parameter    :
//      block_num           Block number 0 -> 2
// Return value : TCNTB1 value
**********************************/
unsigned int ATU5_TIMERB_Func::GetTCNTB1 (const unsigned int block_num)
{//{{{
    if (block_num == 2) {
        return (unsigned int)(*TCNTB1S2);
    } else if (block_num == 1) {
        return (unsigned int)(*TCNTB1S1);
    } else {
        return (unsigned int)(*TCNTB1);
    }
}//}}}

/*********************************
// Function     : GetTCNTB2
// Description  : Return TCNTB2 register value
// Parameter    : None
// Return value : TCNTB2 value
**********************************/
unsigned int ATU5_TIMERB_Func::GetTCNTB2 (void)
{//{{{
    return (unsigned int)(*TCNTB2)["CNTB2"];
}//}}}

/*********************************
// Function     : GetTCNTB3
// Description  : Return TCNTB3 register value
// Parameter    : None
// Return value : TCNTB3 value
**********************************/
unsigned int ATU5_TIMERB_Func::GetTCNTB3 (void)
{//{{{
    return (unsigned int)(*TCNTB3)["CNTB3"];
}//}}}

/*********************************
// Function     : GetTCNTB4
// Description  : Return TCNTB4 register value
// Parameter    : None
// Return value : TCNTB4 value
**********************************/
unsigned int ATU5_TIMERB_Func::GetTCNTB4 (void)
{//{{{
    return (unsigned int)(*TCNTB4)["CNTB4"];
}//}}}

/*********************************
// Function     : GetTCNTB5
// Description  : Return TCNTB5 register value
// Parameter    : None
// Return value : TCNTB5 value
**********************************/
unsigned int ATU5_TIMERB_Func::GetTCNTB5 (void)
{//{{{
    return (unsigned int)(*TCNTB5)["CNTB5"];
}//}}}

/*********************************
// Function     : GetTCCLRB
// Description  : Return TCCLRB register value
// Parameter    : None
// Return value : TCCLRB value
**********************************/
unsigned int ATU5_TIMERB_Func::GetTCCLRB (void)
{//{{{
    return (unsigned int)(*TCCLRB)["CCLRB"];
}//}}}

/*********************************
// Function     : GetOCRB1
// Description  : Return OCRB1 register value
// Parameter    :
//      block_num           Block number 0 -> 2
// Return value : OCRB1 value
**********************************/
unsigned int ATU5_TIMERB_Func::GetOCRB1 (const unsigned int block_num)
{//{{{
    if (block_num == 2) {
        return (unsigned int)(*OCRB1S2);
    } else if (block_num == 1) {
        return (unsigned int)(*OCRB1S1);
    } else {
        return (unsigned int)(*OCRB1);
    }
}//}}}

/*********************************
// Function     : GetOCRB10
// Description  : Return OCRB10 register value
// Parameter    :
//      block_num           Block number 0 -> 2
// Return value : OCRB10 value
**********************************/
unsigned int ATU5_TIMERB_Func::GetOCRB10 (const unsigned int block_num)
{//{{{
    if (block_num == 2) {
        return (unsigned int)(*OCRB10S2);
    } else if (block_num == 1) {
        return (unsigned int)(*OCRB10S1);
    } else {
        return (unsigned int)(*OCRB10);
    }
}//}}}

/*********************************
// Function     : GetOCRB11
// Description  : Return OCRB11 register value
// Parameter    :
//      block_num           Block number 0 -> 2
// Return value : OCRB11 value
**********************************/
unsigned int ATU5_TIMERB_Func::GetOCRB11 (const unsigned int block_num)
{//{{{
    if (block_num == 2) {
        return (unsigned int)(*OCRB11S2);
    } else if (block_num == 1) {
        return (unsigned int)(*OCRB11S1);
    } else {
        return (unsigned int)(*OCRB11);
    }
}//}}}

/*********************************
// Function     : GetOCRB12
// Description  : Return OCRB12 register value
// Parameter    :
//      block_num           Block number 0 -> 2
// Return value : OCRB12 value
**********************************/
unsigned int ATU5_TIMERB_Func::GetOCRB12 (const unsigned int block_num)
{//{{{
    if (block_num == 2) {
        return (unsigned int)(*OCRB12S2);
    } else if (block_num == 1) {
        return (unsigned int)(*OCRB12S1);
    } else {
        return (unsigned int)(*OCRB12);
    }
}//}}}

/*********************************
// Function     : GetICRB0
// Description  : Return ICRB0 register value
// Parameter    :
//      block_num           Block number 0 -> 2
// Return value : OCRB1 value
**********************************/
unsigned int ATU5_TIMERB_Func::GetICRB0 (const unsigned int block_num)
{//{{{
    if (block_num == 2) {
        return (unsigned int)(*ICRB0S2);
    } else if (block_num == 1) {
        return (unsigned int)(*ICRB0S1);
    } else {
        return (unsigned int)(*MIICRB0);
    }
}//}}}

/*********************************
// Function     : GetICRB1
// Description  : Return ICRB1 register value
// Parameter    :
//      block_num           Block number 0 -> 2
// Return value : OCRB1 value
**********************************/
unsigned int ATU5_TIMERB_Func::GetICRB1 (const unsigned int block_num)
{//{{{
    if (block_num == 2) {
        return (unsigned int)(*ICRB1S2);
    } else if (block_num == 1) {
        return (unsigned int)(*ICRB1S1);
    } else {
        return (unsigned int)(*ICRB1);
    }
}//}}}

/*********************************
// Function     : GetOCRB20_43
// Description  : Return OCRB20 -> 43 register value
// Parameter    :
//      state               The state of sequencer (0 -> 24)
// Return value : OCRB20 -> 43 value
**********************************/
unsigned int ATU5_TIMERB_Func::GetOCRB20_43 (const unsigned int state)
{//{{{
    unsigned int ret = 0;
    if (state > 0) {
        ret = (unsigned int)(*OCRB[state+19]);
    }
    return ret;
}//}}}

/*********************************
// Function     : GetTEPCFB
// Description  : Return TEPCFB0 -> 23 register value
// Parameter    :
//      state               The state of sequencer (0 -> 24)
// Return value : TEPCFB0 -> 23 value
**********************************/
unsigned int ATU5_TIMERB_Func::GetTEPCFB (const unsigned int state)
{//{{{
    unsigned int ret = 0;
    if (state > 0) {
        ret = (unsigned int)(*TEPCFB[state-1]);
    }
    return ret;
}//}}}

/*********************************
// Function     : GetPIM
// Description  : Return PIM value
// Parameter    : None
// Return value : PIM value
**********************************/
unsigned int ATU5_TIMERB_Func::GetPIM (void)
{//{{{
    unsigned int ret = (unsigned int)(*PIMR1)["PIM1"];
    if (((bool)(*TCRB)["PIMRSEL"]) && (mATU5_TIB->IsTCNTB11COR(GetChannelSel()))) {
        ret = (unsigned int)(*PIMR2)["PIM2"];
    }
    return ret;
}//}}}

unsigned int ATU5_TIMERB_Func::GetPIM1 (void)
{//{{{
    return (unsigned int)(*PIMR1)["PIM1"];
}//}}}

/*********************************
// Function     : GetRLDB
// Description  : Return RLDB value
// Parameter    : None
// Return value : RLDB value
**********************************/
unsigned int ATU5_TIMERB_Func::GetRLDB (void)
{//{{{
    unsigned int ret = (unsigned int)(*RLDB)["RLDVAL"];
    return ret;
}//}}}

/*********************************
// Function     : GetLDVal
// Description  : Return load value which is loaded to TCNTB2 and RLDB
// Parameter    : None
// Return value : load value
**********************************/
unsigned int ATU5_TIMERB_Func::GetLDVal (void)
{//{{{
    unsigned int ret = GetICRB0(GetChannelSel());
    if ((*TIORB)["LDSEL"] == 1) {
        ret = (unsigned int)(*LDB)["LDVAL"];
    } else if ((*TSEQCRB)["EVSEQENB"]) {
        ret = (unsigned int)(*TEPCVALRB0);
    }
    return ret;
}//}}}

/*********************************
// Function     : GetTICRBVal
// Description  : Return TICRB register value
// Parameter    : None
// Return value : load value
**********************************/
unsigned int ATU5_TIMERB_Func::GetTICRBVal (void)
{//{{{
    return (unsigned int)(*TICRB);
}//}}}

/*********************************
// Function     : GetTIORBVal
// Description  : Return TIORB register value
// Parameter    : None
// Return value : load value
**********************************/
unsigned int ATU5_TIMERB_Func::GetTIORBVal (void)
{//{{{
    return (unsigned int)(*TIORB);
}//}}}

/*********************************
// Function     : GetTSEQNENB
// Description  : Return TSEQNENBx value
// Parameter    :
//      state               The state of sequencer (1 -> 24)
// Return value : TSEQNENBx value
**********************************/
bool ATU5_TIMERB_Func::GetTSEQNENB (const unsigned int state)
{//{{{
    bool ret = false;
    if (state > 0 && state <= 8) {
        ret = (bool)(((*TSEQENB[0])>>(state-1))&0x1);
    } else if (state > 8 && state <= 16) {
        ret = (bool)(((*TSEQENB[1])>>(state-9))&0x1);
    } else if (state > 16 && state <= 24) {
        ret = (bool)(((*TSEQENB[2])>>(state-17))&0x1);
    }
    return ret;
}//}}}

/*********************************
// Function     : GetFON
// Description  : Return FONx value
// Parameter    :
//      state               The state of sequencer (0 -> 24)
// Return value : FONx value
**********************************/
bool ATU5_TIMERB_Func::GetFON (const unsigned int state)
{//{{{
    bool ret = false;
    if (state > 0 && state <= 8) {
        ret = (bool)(((*TEPCFENB[0])>>(state-1))&0x1);
    } else if (state > 8 && state <= 16) {
        ret = (bool)(((*TEPCFENB[1])>>(state-9))&0x1);
    } else if (state > 16 && state <= 24) {
        ret = (bool)(((*TEPCFENB[2])>>(state-17))&0x1);
    }
    return ret;
}//}}}

/*********************************
// Function     : GetWriteCondition
// Description  : Write condition
// Parameter    :
//      addr            register address offset
// Return value : Write condition
**********************************/
bool ATU5_TIMERB_Func::GetWriteCondition (const unsigned int addr)
{//{{{
    bool ret = true;
    if (addr == 0x4C || addr == 0x6C || addr == 0xC4) { //Write to TCNTB1S1/TCNTB1S2/TCNTB1
        if (((unsigned int)(*TSEQCRB)["EVSEQENB"]) == 1) {
            ret = false;
            mATU5_TIB->re_printf("warning","Writing to TCNTB1/TCNTB1S1/TCNTB1S2 when TSEQCRB.EVSEQENB = 1 is prohibited\n");
        }
    }
    return ret;
}//}}}

/*********************************
// Function     : GetTimerStatus
// Description  : Return timer status stored in TSRA register (for CMIB6 and CMIB6M)
// Parameter    :
//      interrupt_id        Interrupt ID (according ATU5_TIMERB::eInterruptId)
// Return value : Timer status stored in TSRB register
**********************************/
bool ATU5_TIMERB_Func::GetTimerStatus (const unsigned int interrupt_id)
{//{{{
    if (interrupt_id == ATU5_TIMERB::emCMIB6) {
        return (bool)(*TSRB)["CMFB6"];
    } else { // if (interrupt_id == ATU5_TIMERB::emCMIB6M)
        return (bool)(*TSRB)["CMFB6M"];
    }
}//}}}

/*********************************
// Function     : IsInterruptEnable
// Description  : Return true if interrupt is enable
// Parameter    :
//      bit_name            Bit name
// Return value : Interrupt enable flag
**********************************/
bool ATU5_TIMERB_Func::IsInterruptEnable (const std::string bit_name)
{//{{{
    return (bool)(*TIERB)[bit_name.c_str()];
}//}}}

/*********************************
// Function     : IsEventInputEnable
// Description  : Return true if interrupt is enable
// Parameter    : None
// Return value : Event input enable flag
**********************************/
bool ATU5_TIMERB_Func::IsEventInputEnable (void)
{//{{{
    return (bool)(*TIORB)["EVCNTB"];
}//}}}

/*********************************
// Function     : GetLDEN
// Description  : Return TIORB.LDEN bit value
// Parameter    : None
// Return value : TCNTB2 and RLDB are updated on ICRB0 input capture or not
**********************************/
bool ATU5_TIMERB_Func::GetLDEN (void)
{//{{{
    bool ret = false;
    ret = (bool)(*TIORB)["LDEN"];
    return ret;
}//}}}

/*********************************
// Function     : CalCycleCorrection
// Description  : Return timer event input cycle correction
// Parameter    :
//      state               The state of sequencer (0 -> 24)
// Return value : Timer event input cycle correction
**********************************/
double ATU5_TIMERB_Func::CalCycleCorrection (const unsigned int state)
{//{{{
    unsigned int icrb0 = GetICRB0(GetChannelSel());
    unsigned int fon_val = (unsigned int)(GetFON(state));
    unsigned int tepcfb_val = GetTEPCFB(state);
    double halfen = 1 - ((double)((tepcfb_val>>12)&0x1))/2;
    double coeff = 0.25 + 0.25*(double)((tepcfb_val>>8)&0x7);
    unsigned int crfit1_val = icrb0;
    unsigned int crfit2_val = icrb0;
    unsigned int crfit1 = ((tepcfb_val>>4)&0x7);
    unsigned int crfit2 = ((tepcfb_val)&0x7);
    if (crfit1 > 0 && crfit1 < 7) {
        crfit1_val = (unsigned int)(*RECRB[crfit1]);
    }
    if (crfit2 > 0 && crfit2 < 7) {
        crfit2_val = (unsigned int)(*RECRB[crfit2]);
    }
    double correct_result = (double)icrb0 + ((double)fon_val * (((double)crfit1_val-(double)crfit2_val) * coeff * halfen));
    return correct_result;
}//}}}

/*********************************
// Function     : SetTCNTB0
// Description  : Set counter value to TCNTB0 register
// Parameter    :
//      block_num           Block number 0 -> 2
//      value               Counter value
// Return value : None
**********************************/
void ATU5_TIMERB_Func::SetTCNTB0 (const unsigned int block_num, const unsigned int value)
{//{{{
    if (block_num == 2) {
        (*TCNTB0S2) = value;
    } else if (block_num == 1) {
        (*TCNTB0S1) = value;
    } else {
        (*TCNTB0) = value;
    }
}//}}}

/*********************************
// Function     : SetTCNTB1
// Description  : Set counter value to TCNTB1 register
// Parameter    :
//      block_num           Block number 0 -> 2
//      clear_trig          Clear condition
//      clrb1sel            true: Compare match TCNTB6 and OCRB6 occurred
//                          false: Compare match TCNTB1 and OCRB10 occurred
// Return value : None
**********************************/
void ATU5_TIMERB_Func::SetTCNTB1 (const unsigned int block_num, const bool clear_trig, const bool clrb1sel)
{//{{{
    unsigned int tcntb1_val = 0;
    if (block_num == 2) {
        if (clear_trig && ((*TCRBS2)["CLRB1S2"] == 1) && (clrb1sel == ((bool)(*TCRBS2)["CLRB1SELS2"]))) {
            (*TCNTB1S2) = 1;
        } else {
            tcntb1_val = (unsigned int)(*TCNTB1S2);
            (*TCNTB1S2) = (tcntb1_val + 1)&0xFF;
        }
    } else if (block_num == 1) {
        if (clear_trig && ((*TCRBS1)["CLRB1S1"] == 1) && (clrb1sel == ((bool)(*TCRBS1)["CLRB1SELS1"]))) {
            (*TCNTB1S1) = 1;
        } else {
            tcntb1_val = (unsigned int)(*TCNTB1S1);
            (*TCNTB1S1) = (tcntb1_val + 1)&0xFF;
        }
    } else {
        if (clear_trig && ((*TCRB)["CLRB1"] == 1) && (clrb1sel == ((bool)(*TCRB)["CLRB1SEL"]))) {
            (*TCNTB1) = 1;
        } else {
            tcntb1_val = (unsigned int)(*TCNTB1);
            (*TCNTB1) = (tcntb1_val + 1)&0xFF;
        }
    }
}//}}}

/*********************************
// Function     : SetTCNTB2
// Description  : Set counter value to TCNTB2 register
// Parameter    :
//      value               Set value
// Return value : None
**********************************/
void ATU5_TIMERB_Func::SetTCNTB2 (const unsigned int value)
{//{{{
    (*TCNTB2)["CNTB2"] = value;
}//}}}

/*********************************
// Function     : SetTCNTB3
// Description  : Set counter value to TCNTB3 register
// Parameter    :
//      clear_trig          Clear condition
//      clrb3sel            0: Compare match TCNTB1 and OCRB10 occurred
//                          1: Compare match TCNTB6 and OCRB6 occurred
//                          2: Compare match TCNTB3 and OCRB8 occurred
// Return value : None
**********************************/
void ATU5_TIMERB_Func::SetTCNTB3 (const bool clear_trig, const unsigned int clrb3sel)
{//{{{
    if (clear_trig) {
        if (((bool)(*TCRB)["CLRB3"]) && (clrb3sel == ((unsigned int)(*TCRB)["CLRB3SEL"]))) {
            (*TCNTB3) = 0;
        }
    } else {
        (*TCNTB4)["CNTB4"] = (unsigned int)(*TCNTB3)["CNTB3"];
        unsigned int tcntb3_val = (unsigned int)(*TCNTB3)["CNTB3"];
        unsigned int pim_val = (unsigned int)(*PIMR1)["PIM1"];
        (*TCNTB3)["CNTB3"] = (tcntb3_val + pim_val)&0xFFFFF;
        if ((unsigned int)(*TCNTB3)["CNTB3"] == (unsigned int)(*OCRB8)["OCB8"]) {
            mATU5_TIB->NotifyTCNTB3_8();
        }
    }
}//}}}

/*********************************
// Function     : SetTCNTB4
// Description  : Set counter value to TCNTB4 register
// Parameter    : None
// Return value : None
**********************************/
bool ATU5_TIMERB_Func::SetTCNTB4 (void)
{//{{{
    unsigned int tcntb4_val = (unsigned int)(*TCNTB4)["CNTB4"];
    unsigned int tcntb3_val = (unsigned int)(*TCNTB3)["CNTB3"];
    if (((((*TCNT4CRB)["CU4SEL"] == 1) && (tcntb4_val == (tcntb3_val-1))) || (((*TCNT4CRB)["CU4SEL"] == 0) && (tcntb4_val == tcntb3_val))) && ((*TIORB)["CCS"]==1)) {
        return false;
    } else {
        (*TCNTB4)["CNTB4"] = (tcntb4_val + 1)&0xFFFFF;
        return true;
    }
}//}}}

/*********************************
// Function     : SetTCNTB5
// Description  : Set counter value to TCNTB5 register
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB_Func::SetTCNTB5 (const unsigned int value)
{//{{{
    (*TCNTB5)["CNTB5"] = value;
}//}}}

/*********************************
// Function     : SetTCNTB6
// Description  : Set counter value to TCNTB6 register
// Parameter    :
//      clear_trig          Clear condition
// Return value : None
**********************************/
void ATU5_TIMERB_Func::SetTCNTB6 (const bool clear_trig)
{//{{{
    unsigned int reg = (unsigned int)(*TCNTB6)["CNTB6"];
    unsigned int regm = (unsigned int)(*TCNTB6M)["CNTB6M"];
    unsigned int rarb6 = (unsigned int)(*RARB6)["RARB6"];
    if (clear_trig) {
        double icrb6 = (double)(*ICRB6)["ICB6"];
        double tcntb6m = ((double)(*TCNTB6M)["CNTB6M"])/64;
        if (icrb6 > tcntb6m) {
            mATU5_TIB->NotifyCMFB6M();
            mIsCMFB6M = true;
        }
        (*ICRB6) = (unsigned int)(*TCNTB6);
        (*TCNTB6) = 0;
        (*TCNTB6M) = 0;
    } else {
        (*TCNTB6)["CNTB6"] = reg + 1;
        (*TCNTB6M)["CNTB6M"] = regm + rarb6;
        if (((*TCNTB6)["CNTB6"] == (*OCRB6)["OCB6"]) && ((*TIORB)["IOB6"] == 1)) {
            mATU5_TIB->NotifyTCNTB6COR();
        }
        if ((reg == (*OCRB7)["OCB7"]) && ((*TICRB)["IREGB6"] == 2)) {
            if ((*TICRB)["IREGB6"] == 2) {
                (*TSRB)["CMFB6"] = 0;
            }
        }
    }
}//}}}

/*********************************
// Function     : SetICRB0
// Description  : Set counter value to ICRB0 register
// Parameter    :
//      block_num           Block number 0 -> 2
//      agck_trig           true: external event input; false: no event
//      value               Counter value
// Return value : None
**********************************/
void ATU5_TIMERB_Func::SetICRB0 (const unsigned int block_num, const bool agck_trig, const unsigned int value)
{//{{{
    if (agck_trig) {
        (*RECRB[6]) = (unsigned int)(*RECRB[5]);
        (*RECRB[5]) = (unsigned int)(*RECRB[4]);
        (*RECRB[4]) = (unsigned int)(*RECRB[3]);
        (*RECRB[3]) = (unsigned int)(*RECRB[2]);
        (*RECRB[2]) = (unsigned int)(*RECRB[1]);
        if (block_num == 2) {
            (*RECRB[1]) = (unsigned int)(*ICRB0S2);
            (*ICRB0S2) = value;
        } else if (block_num == 1) {
            (*RECRB[1]) = (unsigned int)(*ICRB0S1);
            (*ICRB0S1) = value;
        } else {
            (*RECRB[1]) = (unsigned int)(*MIICRB0);
            (*MIICRB0) = value;
        }
        (*DRECRB1) = (unsigned int)(*RECRB[1]);
    }
}//}}}

/*********************************
// Function     : SetICRB1
// Description  : Set counter value to ICRB1 register
// Parameter    :
//      block_num           Block number 0 -> 2
//      clear_trig          true: Compare match between TCNTB1 and OCRB1 occurs; false: no event
//      agck_trig           true: external event input; false: no event
//      value               Counter value
// Return value : None
**********************************/
void ATU5_TIMERB_Func::SetICRB1 (const unsigned int block_num, const bool clear_trig, const bool agck_trig, const unsigned int value)
{//{{{
    unsigned int icrb1_val = 0;
    if (block_num == 2) {
        icrb1_val = (unsigned int)(*ICRB1S2);
        if (!clear_trig && agck_trig) {
            (*ICRB1S2) = icrb1_val + value;
        } else if (clear_trig) {
            (*ICRB1S2) = 0;
        }
    } else if (block_num == 1) {
        icrb1_val = (unsigned int)(*ICRB1S1);
        if (!clear_trig && agck_trig) {
            (*ICRB1S1) = icrb1_val + value;
        } else if (clear_trig) {
            (*ICRB1S1) = 0;
        }
    } else {
        icrb1_val = (unsigned int)(*ICRB1);
        if (!clear_trig && agck_trig) {
            (*ICRB1) = icrb1_val + value;
        } else if (clear_trig) {
            (*ICRB1) = 0;
        }
    }
}//}}}

/*********************************
// Function     : SetICRB2
// Description  : Set counter value to ICRB1 register
// Parameter    :
//      block_num           Block number 0 -> 2
//      value               Counter value
// Return value : None
**********************************/
void ATU5_TIMERB_Func::SetICRB2 (const unsigned int block_num, const unsigned int value)
{//{{{
    if (block_num == 2) {
        (*ICRB2S2) = value;
    } else if (block_num == 1) {
        (*ICRB2S1) = value;
    } else {
        (*ICRB2) = value;
    }
}//}}}

/*********************************
// Function     : SetICRB3
// Description  : Set counter value to ICRB3x register
// Parameter    :
//      event_id            Event ID 0 -> 7
//      value               TCNTB1 counter value
// Return value : None
**********************************/
void ATU5_TIMERB_Func::SetICRB3 (const unsigned int event_id, const unsigned int value)
{//{{{
    (*ICRB[event_id])["ICB"] = value;
}//}}}

/*********************************
// Function     : SetTSEQRB
// Description  : Set value to TSEQRB register
// Parameter    :
//      state               The state of sequencer (0 -> 24)
// Return value : None
**********************************/
void ATU5_TIMERB_Func::SetTSEQRB (const unsigned int state)
{//{{{
    if ((state > 0) && (state <= 24)) {
        (*TSEQRB)["FONS"] = GetFON(state);
        (*TSEQRB)["SEQB"] = state;
    } else if (state == 0) {
        (*TSEQRB) = 0;
    }
}//}}}

/*********************************
// Function     : GetTSEQRB
// Description  : Get value to TSEQRB register
// Parameter    :
// Return value : The state of sequencer (0 -> 24)
**********************************/
unsigned int ATU5_TIMERB_Func::GetTSEQRB (void)
{
    return (unsigned int)(*TSEQRB)["SEQB"];
}

/*********************************
// Function     : SetTEPCVALRB0
// Description  : Set value to TEPCVALRB0 register
// Parameter    :
//      state               The state of sequencer (0 -> 24)
// Return value : None
**********************************/
void ATU5_TIMERB_Func::SetTEPCVALRB0 (const unsigned int state)
{//{{{
    (*TEPCRECRB1) = (unsigned int)(*TEPCVALRB0);
    (*TEPCVALRB0) = (unsigned int)CalCycleCorrection(state);
}//}}}

/*********************************
// Function     : UpdateTimerStatus
// Description  : Return I/O control setting
// Parameter    :
//      interrupt_id        Interrupt ID (according ATU5_TIMERB::eInterruptId)
// Return value : None
**********************************/
void ATU5_TIMERB_Func::UpdateTimerStatus (const unsigned int interrupt_id)
{//{{{
    if (interrupt_id == ATU5_TIMERB::emCMIB0) {
        (*TSRB)["CMFB0"] = 1;
    } else if (interrupt_id == ATU5_TIMERB::emICIB0) {
        (*TSRB)["ICFB0"] = 1;
    } else if (interrupt_id == ATU5_TIMERB::emCMIB1) {
        (*TSRB)["CMFB1"] = 1;
    } else if (interrupt_id == ATU5_TIMERB::emCMIB6) {
        (*TSRB)["CMFB6"] = 1;
    } else if (interrupt_id == ATU5_TIMERB::emCMIB10) {
        (*TSRB)["CMFB10"] = 1;
    } else if (interrupt_id == ATU5_TIMERB::emCMIB11) {
        (*TSRB)["CMFB11"] = 1;
    } else if (interrupt_id == ATU5_TIMERB::emCMIB12) {
        (*TSRB)["CMFB12"] = 1;
    } else if (interrupt_id == ATU5_TIMERB::emCMIB6M) {
        (*TSRB)["CMFB6M"] = 1;
        mIsCMFB6M = false;
    } else if (interrupt_id == ATU5_TIMERB::emCMIB6E) {
        (*TSRB)["CMFB6E"] = 1;
    }
}//}}}

/*********************************
// Function     : SetRLDB
// Description  : Set RLDB value
// Parameter    :
//      value               RLDB value
// Return value : None
**********************************/
void ATU5_TIMERB_Func::SetRLDB (const unsigned int value)
{//{{{
    (*RLDB)["RLDVAL"] = value;
}//}}}

/*********************************
// Function     : SetTCCLFB
// Description  : Set TCCLFRB.TCCLFB bit value
// Parameter    :
//      value               TCCLFB value
//      clear_trig          true: TCNTB3 = 0 and external event input
// Return value : None
**********************************/
void ATU5_TIMERB_Func::SetTCCLFB (const unsigned int value, const bool clear_trig)
{//{{{
    (*TCCLFRB)["TCCLFB"] = value & 0x1;
    if (clear_trig) {
        // CU4SEL clears TCNTB5 in 0000 1000H at 0. CU4SEL clears TCNTB5 in 0000 0000H at 1
        ClearTCNTB5();
    }
}//}}}

/*********************************
// Function     : ClearTCNTB5
// Description  : Clear TCNTB5 basing on CU4SEL
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB_Func::ClearTCNTB5 (void)
{//{{{
    // CU4SEL clears TCNTB5 in 0000 1000H at 0. CU4SEL clears TCNTB5 in 0000 0000H at 1
    if ((bool)(*TCNT4CRB)["CU4SEL"] == false) {
        (*TCNTB5) = 0x1000;
    } else {
        (*TCNTB5) = 0x0;
    }
}//}}}

/*********************************
// Function     : GetB5UpperLimit
// Description  : Return TCNTB5 upper limit min(b4, tcclrb)
// Parameter    : None
// Return value : unsigned int
**********************************/
unsigned int ATU5_TIMERB_Func::GetB5UpperLimit (void)
{//{{{
    unsigned int ret = 0;

    unsigned int b4 = GetTCNTB4();
    unsigned int tcclrb = GetTCCLRB();
    
    if (b4 < tcclrb) {
        ret = b4;
    }
    else {
        ret = tcclrb;
    }

    return ret;
}//}}}

/*********************************
// Function     : cb_TCRB_CKSELB
// Description  : Callback function when TCRB register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void ATU5_TIMERB_Func::cb_TCRB_CKSELB(RegCBstr str)
{//{{{
    unsigned int ckselb_val = (unsigned int)(*TCRB)["CKSELB"];
    unsigned int pre_ckselb_val = (unsigned int)(str.pre_data & 0x3);
    if ((ckselb_val != pre_ckselb_val)&&(mATU5_TIB->IsTIBEn())) {
        mATU5_TIB->re_printf("warning","Can not change selected counting clock\n");
        (*TCRB)["CKSELB"] = pre_ckselb_val;
    }
    if ((unsigned int)(*TCRB)["CLRB3SEL"] == 0x3) {
        mATU5_TIB->re_printf("warning","Setting prohibited. There is no operation for this setting\n");
    }
}//}}}

/*********************************
// Function     : cb_TSCRB_CMFCB0
// Description  : Callback function when TSCRB register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void ATU5_TIMERB_Func::cb_TSCRB_CMFCB0(RegCBstr str)
{//{{{
    (*TSRB) = (*TSRB) & (0x37F & ~(*TSCRB));
    (*TSCRB) = 0;
}//}}}

/*********************************
// Function     : cb_TICRB_IREGB6
// Description  : Callback function when TICRB register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void ATU5_TIMERB_Func::cb_TICRB_IREGB6(RegCBstr str)
{//{{{
    if (mATU5_TIB->IsTIBEn()) {
        mATU5_TIB->re_printf("warning","Modifying register setting while the Timer B is enabled does not guarantee function correctly\n");
    }
    if (((unsigned int)(*TICRB)["IREGB6"] == 0x3)||((unsigned int)(*TICRB)["IREGB6E"] == 0x3)) {
        mATU5_TIB->re_printf("warning","Setting prohibited. There is no operation for this setting\n");
    }
}//}}}

/*********************************
// Function     : cb_TCNTB0S1_CNTB0S1
// Description  : Callback function when TCNTB0S1 register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void ATU5_TIMERB_Func::cb_TCNTB0S1_CNTB0S1(RegCBstr str)
{//{{{
    mATU5_TIB->NotifyUpdatedCntVal("TCNTB0", 1);
}//}}}

/*********************************
// Function     : cb_OCRB0S1_OCB0S1
// Description  : Callback function when OCRB0S1 register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void ATU5_TIMERB_Func::cb_OCRB0S1_OCB0S1(RegCBstr str)
{//{{{
    mATU5_TIB->SetTCNTB0CORVal(1,(unsigned int)(*OCRB0S1)["OCB0S1"]);
}//}}}

/*********************************
// Function     : cb_TCNTB0S2_CNTB0S2
// Description  : Callback function when TCNTB0S2 register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void ATU5_TIMERB_Func::cb_TCNTB0S2_CNTB0S2(RegCBstr str)
{//{{{
    mATU5_TIB->NotifyUpdatedCntVal("TCNTB0", 2);
}//}}}

/*********************************
// Function     : cb_OCRB0S2_OCB0S2
// Description  : Callback function when OCRB0S2 register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void ATU5_TIMERB_Func::cb_OCRB0S2_OCB0S2(RegCBstr str)
{//{{{
    mATU5_TIB->SetTCNTB0CORVal(2,(unsigned int)(*OCRB0S2)["OCB0S2"]);
}//}}}

/*********************************
// Function     : cb_TCNTB0_CNTB0
// Description  : Callback function when TCNTB0 register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void ATU5_TIMERB_Func::cb_TCNTB0_CNTB0(RegCBstr str)
{//{{{
    mATU5_TIB->NotifyUpdatedCntVal("TCNTB0", 0);
}//}}}

/*********************************
// Function     : cb_OCRB0_OCB0
// Description  : Callback function when OCRB0 register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void ATU5_TIMERB_Func::cb_OCRB0_OCB0(RegCBstr str)
{//{{{
    mATU5_TIB->SetTCNTB0CORVal(0,(unsigned int)(*OCRB0)["OCB0"]);
}//}}}

/*********************************
// Function     : cb_TCNTB2_CNTB2
// Description  : Callback function when TCNTB2 register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void ATU5_TIMERB_Func::cb_TCNTB2_CNTB2(RegCBstr str)
{//{{{
    if (str.is_wr) {
        mATU5_TIB->NotifyUpdatedCntVal("TCNTB2", 0);
    } else {
        (*TCNTB2)["CNTB2"] = (unsigned int)mATU5_TIB->GetB2Val();
    }
}//}}}

/*********************************
// Function     : cb_PIMR1_PIM1
// Description  : Callback function when PIMR1 register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void ATU5_TIMERB_Func::cb_PIMR1_PIM1(RegCBstr str)
{//{{{
    if (((*TCRB)["PIMRSEL"] == 0) || (!mATU5_TIB->IsTCNTB11COR(GetChannelSel()))) {
        mATU5_TIB->NotifyUpdatedPIMVal();
    }
}//}}}

/*********************************
// Function     : cb_TSEQCRB_EVSEQENB
// Description  : Callback function when TSEQCRB register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void ATU5_TIMERB_Func::cb_TSEQCRB_EVSEQENB(RegCBstr str)
{//{{{
    bool seq_en = (bool)(*TSEQCRB)["EVSEQENB"];
    if (seq_en != (bool)(str.pre_data & 0x1)) {
        mATU5_TIB->EnableSeq(seq_en);

        // After disabling SEQ, it's enable to write OCRB and TEPCFB registers
        if (!seq_en) {
            for (unsigned int i = 0; i < 24; i++) {
                mIsProhibitRewriteOCRB[i] = false;
                mIsProhibitRewriteTEPCFB[i] = false;
            }
        }
    }
    
}//}}}

/*********************************
// Function     : cb_TCNTB5_CNTB5
// Description  : Callback function when TCNTB5 register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void ATU5_TIMERB_Func::cb_TCNTB5_CNTB5(RegCBstr str)
{//{{{
    if (str.is_wr) {
        mATU5_TIB->NotifyUpdatedCntVal("TCNTB5", 0);
    } else {
        (*TCNTB5)["CNTB5"] = (unsigned int)mATU5_TIB->GetB5Val();
    }
}//}}}

void ATU5_TIMERB_Func::UpdateRBURB (void)
{//{{{
    if (GetChannelSel() == 0) {
        (*RBURB[0]) = (unsigned int)(*MIICRB0);
    } else if (GetChannelSel() == 1) {
        (*RBURB[0]) = (unsigned int)(*ICRB0S1);
    } else if (GetChannelSel() == 2) {
        (*RBURB[0]) = (unsigned int)(*ICRB0S2);
    }
    for (unsigned int i = 1; i <= 6; i++) {
        (*RBURB[i]) = (unsigned int)(*RECRB[i]);
    }
}//}}}

void ATU5_TIMERB_Func::cb_OCRB_OCB(RegCBstr str)
{//{{{
    // OCRB20 -> OCRB43
    // mIsProhibitRewriteOCRB[0] -> mIsProhibitRewriteOCRB[23]
    // GetTSEQNENB(1) -> GetTSEQNENB(24)
    
    // While the sequencer is running, it is prohibited to rewrite the compare registers 
    // corresponding to the sequence numbers enabled by the timer sequencer enable bits TSEQNEN[23:0].
    if (((bool)(*TSEQCRB)["EVSEQENB"]) && (GetTSEQNENB(str.channel - 19))) {
        if (mIsProhibitRewriteOCRB[str.channel - 20]) {
            mATU5_TIB->re_printf("warning","Rewrite to OCRB%d register while sequencer is running is prohibited\n", str.channel);
            (*OCRB[str.channel]) = (unsigned int)str.pre_data;
        } else {
            mIsProhibitRewriteOCRB[str.channel - 20] = true;
        }
    }
}//}}}

void ATU5_TIMERB_Func::cb_TEPCFB_CRFIT2(RegCBstr str)
{//{{{
    // TEPCFB0 -> TEPCFB23
    // mIsProhibitRewriteTEPCFB[0] -> mIsProhibitRewriteTEPCFB[23]
    // GetTSEQNENB(1) -> GetTSEQNENB(24)

    // While the sequencer is running, it is prohibited to rewrite the compare registers 
    // corresponding to the sequence numbers enabled by the timer sequencer enable bits TSEQNEN[23:0].
    if (((bool)(*TSEQCRB)["EVSEQENB"]) && (GetTSEQNENB(str.channel + 1))) {
        if (mIsProhibitRewriteTEPCFB[str.channel]) {
            mATU5_TIB->re_printf("warning","Rewrite to TEPCFB%d register while sequencer is running is prohibited\n", str.channel);
            (*TEPCFB[str.channel]) = (unsigned int)str.pre_data;
        } else {
            mIsProhibitRewriteTEPCFB[str.channel] = true;
        }
    }
}//}}}

void ATU5_TIMERB_Func::cb_TCCLRB_CCLRB(RegCBstr str)
{//{{{
    mATU5_TIB->NotifyUpdatedTCCLRBVal((unsigned int)(*TCCLRB)["CCLRB"]);
}//}}}

void ATU5_TIMERB_Func::cb_TCNTB4_CNTB4(RegCBstr str)
{//{{{
    mATU5_TIB->NotifyUpdatedCntVal("TCNTB4", 0);
}//}}}

void ATU5_TIMERB_Func::cb_TIORB_LDSEL(RegCBstr str)
{//{{{
    if (((*TIORB)["CTCNTB5"] == 0x1) && ((((unsigned int)str.pre_data >> 6) & 0x1) == 0x0)) {
        // Setting this bit to 1 stops TCNTB5 and the multiplied-and-corrected clock to be output to other timers.
        // Even if the counter is stopped, it is not cleared.

        // do nothing because the stop operation is done in ATU5_TIMERB.cpp
    } else if (((*TIORB)["CTCNTB5"] == 0x0) && ((((unsigned int)str.pre_data >> 6) & 0x1) == 0x1)) {
        mATU5_TIB->ResumingB5();
    }
}//}}}

// vim600: set foldmethod=marker :
