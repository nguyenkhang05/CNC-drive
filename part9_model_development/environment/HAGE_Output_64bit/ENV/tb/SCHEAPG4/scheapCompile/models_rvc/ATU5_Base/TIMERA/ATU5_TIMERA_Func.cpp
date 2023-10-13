// ---------------------------------------------------------------------
// $Id: ATU5_TIMERA_Func.cpp 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#include "ATU5_TIMERA_Func.h"
#include "ATU5_TIMERA_Base.h"

/*********************************
// Function     : ATU5_TIMERA_Func
// Description  : Constructor of ATU5_TIMERA_Func class
// Parameter    :
//      name            Module name
//      Parent          Indicate base class
// Return value : None
**********************************/
ATU5_TIMERA_Func::ATU5_TIMERA_Func(std::string name, ATU5_TIMERA_Base *ATU5_TIA_Base_ptr):
                             Catu5_timera_regif(name,32)
{//{{{
    assert(ATU5_TIA_Base_ptr != NULL);
    mATU5_TIA_Base = ATU5_TIA_Base_ptr;
    mATU5_TIA_Base->CommandInit(name);
    Catu5_timera_regif::set_instance_name(name);
    mModuleName = name;
}//}}}

/*********************************
// Function     : ~ATU5_TIMERA_Func
// Description  : Destructor of ATU5_TIMERA_Func class
// Parameter    : None
// Return value : None
**********************************/
ATU5_TIMERA_Func::~ATU5_TIMERA_Func (void)
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
void ATU5_TIMERA_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = (unsigned int)(trans.get_address()-0x200)&0x1FF;
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (addr == 0x18) { //Read TILRA
        (*TILRA) = mATU5_TIA_Base->GetTILRAVal();
    }
    if ((addr == 0x20)&&(mATU5_TIA_Base->IsTimerRunning())) { //Read TCNTA
        SetTCNTA(mATU5_TIA_Base->GetCounterValue());
    }
    for (unsigned int i = 0; i < 8; i++) {
        if ((addr == (0x40 + (i*0x20)))&&(mATU5_TIA_Base->IsNoiseCancelDuration(i))) { //Read NCNTA
            SetNCNTA(mATU5_TIA_Base->GetNoiseCounterValue(i),i);
        }
    }
    if (debug == false) {
        reg_rd((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
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
void ATU5_TIMERA_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = (unsigned int)(trans.get_address()-0x200)&0x1FF;
    if (debug == false) {
        reg_wr((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    }
    for (unsigned int i = 0; i < 8; i++) {
        if (addr == (0x40 + (i*0x20))) {
            cb_NCNTA_NCNTA(i);
        } else if (addr == (0x42 + (i*0x20))) {
            cb_NCRA_NCTA(i);
        }
    }
}//}}}

/*********************************
// Function     : RegisterHandler
// Description  : Set parameters or commands to Catu5_timera_regif class
// Parameter    :
//      args            Argument of parameter/command
// Return value : None
**********************************/
void ATU5_TIMERA_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Catu5_timera_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Catu5_timera_regif class
            msg_return = Catu5_timera_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/*********************************
// Function     : EnableReset
// Description  : Reset ATU5_TIMERA's registers
// Parameter    :
//      is_active           Active state of reset
// Return value : Register value
**********************************/
void ATU5_TIMERA_Func::EnableReset (const bool is_active)
{//{{{
    Catu5_timera_regif::EnableReset(is_active);
}//}}}

/*********************************
// Function     : GetCLKBusSel
// Description  : Return Clock bus selection
// Parameter    : None
// Return value : Clock bus selection
**********************************/
unsigned int ATU5_TIMERA_Func::GetCLKBusSel (void)
{//{{{
    return (unsigned int)(*TCR1A)["CKSELA"];
}//}}}

/*********************************
// Function     : GetNoiseCancelMode
// Description  : Return noise cancellation mode (individual channel setting)
// Parameter    :
//      chan_num            Channel number
// Return value : noise cancellation mode
//      0                   Premature-transition cancellation mode
//      1                   Minimum time-at-level cancellation mode
//      2                   Level-accumulated cancellation mode
**********************************/
unsigned int ATU5_TIMERA_Func::GetNoiseCancelMode (const unsigned int chan_num)
{//{{{
    unsigned int ret = 0;
    if ((((unsigned int)(*NCMCR1A)>>chan_num)&0x1) == 0x1){
        if ((((unsigned int)(*NCMCR2A)>>chan_num)&0x1) == 0) {
            ret = 1;
        } else {
            ret = 2;
        }
    }
    return ret;
}//}}}

/*********************************
// Function     : GetNoiseCLKSel
// Description  : Return noise cancellation clock selection (individual channel setting)
// Parameter    :
//      chan_num            Channel number
// Return value : noise cancellation clock selection
//      0                   nc_clk_2 is selected
//      1                   nc_clk_1 is selected
//      2                   clkbus_5 is selected
**********************************/
unsigned int ATU5_TIMERA_Func::GetNoiseCLKSel (const unsigned int chan_num)
{//{{{
    unsigned int ret = 0;
    unsigned int cur_nckga = (unsigned int)(*TIOR2A)["NCKGA"]&0xFF;
    unsigned int cur_ncka = (unsigned int)(*TIOR2A)["NCKA"]&0xFF;
    if (((cur_nckga>>chan_num)&0x1) == 0x0){
        if (((cur_ncka>>chan_num)&0x1) == 0) {
            ret = 1;
        } else {
            ret = 2;
        }
    }
    return ret;
}//}}}

/*********************************
// Function     : GetTCNTA
// Description  : Return TCNTA register value
// Parameter    : None
// Return value : TCNTA value
**********************************/
unsigned int ATU5_TIMERA_Func::GetTCNTA (void)
{//{{{
    return (unsigned int)(*TCNTA);
}//}}}

/*********************************
// Function     : GetNCNTA
// Description  : Return NCNTA setting
// Parameter    :
//      chan_num            Channel number
// Return value : NCNTA value
**********************************/
unsigned int ATU5_TIMERA_Func::GetNCNTA (const unsigned int chan_num)
{//{{{
    unsigned int ret = (unsigned int)(*NCNTA[chan_num]);
    return ret;
}//}}}

/*********************************
// Function     : GetICRA2SEL
// Description  : Return ICRA2SEL setting
// Parameter    : None
// Return value : ICRA2SEL value
**********************************/
unsigned int ATU5_TIMERA_Func::GetICRA2SEL (void)
{//{{{
    unsigned int ret = (unsigned int)(*TCR5A)["ICRA2SEL"];
    return ret;
}//}}}

/*********************************
// Function     : IsOutEvent
// Description  : Confirm whether event output or not
// Parameter    :
//      event_name          ev_out_1_0,ev_out_1_1,ev_out_1_2,ev_out_2A,ev_out_2B,ev_out_1B -> ev_out_1I
//      chan_num            Channel number
//      is_raisingedge      true: Raising edge of TIA signal; false: Falling edge of TIA signal
// Return value : true/false
**********************************/
bool ATU5_TIMERA_Func::IsOutEvent (const std::string event_name, const unsigned int chan_num, const bool is_raisingedge)
{//{{{
    bool ret = false;
    unsigned int edge_sel = (unsigned int)((*TIOR1A)>>(chan_num*2))&0x3;
    if ((edge_sel == 0x1 && is_raisingedge) || (edge_sel == 0x2 && !is_raisingedge) || (edge_sel == 0x3)){
        if (event_name == "ev_out_1_0") {
            if ((unsigned int)(*TCR2A)["EVOSELE1E"] == 1) {
                if ((((unsigned int)(*TCR2A)["EVOSELE1"]>>chan_num)&0x1) == 1) {
                    ret = true;
                }
            } else {
                unsigned int evosel = (unsigned int)(*TCR1A)["EVOSEL1"];
                if (((evosel == 0x1 || evosel == 0x3 || evosel == 0x5 || evosel == 0x7) && chan_num == 0) ||
                    ((evosel == 0x2 || evosel == 0x3 || evosel == 0x6 || evosel == 0x7) && chan_num == 1) ||
                    ((evosel == 0x4 || evosel == 0x5 || evosel == 0x6 || evosel == 0x7) && chan_num == 2)){
                    ret = true;
                }
            }
        } else if (event_name == "ev_out_1_1") {
            if ((((unsigned int)(*TCR2AS1)["EVOSELE1S1"]>>chan_num)&0x1) == 1) {
                ret = true;
            }
        } else if (event_name == "ev_out_1_2") {
            if ((((unsigned int)(*TCR2AS2)["EVOSELE1S2"]>>chan_num)&0x1) == 1) {
                ret = true;
            }
        } else if (event_name == "ev_out_2A") {
            if ((*TCR3A)["EVOSELE2AE"] == 1) {
                if ((((*TCR3A)["EVOSELE2A"]>>chan_num)&0x1) == 1) {
                    ret = true;
                }
            } else {
                bool evosel = (bool)(*TCR1A)["EVOSEL2A"];
                if ((!evosel && chan_num == 1) || (evosel && chan_num == 2)){
                    ret = true;
                }
            }
        } else if (event_name == "ev_out_2B") {
            if ((*TCR4A)["EVOSELE2BE"] == 1) {
                if ((((*TCR4A)["EVOSELE2B"]>>chan_num)&0x1) == 1) {
                    ret = true;
                }
            } else {
                bool evosel = (bool)(*TCR1A)["EVOSEL2B"];
                if ((!evosel && chan_num == 1) || (evosel && chan_num == 2)){
                    ret = true;
                }
            }
        } else if ((event_name == "ev_out_1B" && chan_num == 0) || (event_name == "ev_out_1C" && chan_num == 1) ||
                   (event_name == "ev_out_1D" && chan_num == 2) || (event_name == "ev_out_1E" && chan_num == 3) ||
                   (event_name == "ev_out_1F" && chan_num == 4) || (event_name == "ev_out_1G" && chan_num == 5) ||
                   (event_name == "ev_out_1H" && chan_num == 6) || (event_name == "ev_out_1I" && chan_num == 7)) {
            ret = true;
        } else if (event_name == "intr_icia") { //Interrupt ICIAx
            ret = true;
        }
    }
    return ret;
}//}}}

/*********************************
// Function     : IsNoiseCancelEn
// Description  : Return Noise canceler status
// Parameter    :
//      chan_num            Channel number
// Return value : Noise canceler status
**********************************/
bool ATU5_TIMERA_Func::IsNoiseCancelEn (const unsigned int chan_num)
{//{{{
    return (bool)(((*TIOR2A)["NCEA"]>>chan_num)&0x1);
}//}}}

/*********************************
// Function     : IsInterruptEnable
// Description  : Return true if interrupt is enable
// Parameter    :
//      bit_name            Bit name
// Return value : Interrupt enable flag
**********************************/
bool ATU5_TIMERA_Func::IsInterruptEnable (const std::string bit_name)
{//{{{
    return (bool)(*TIERA)[bit_name.c_str()];
}//}}}

/*********************************
// Function     : UpdateTimerStatus
// Description  : Return I/O control setting
// Parameter    :
//      is_captured         Timer status (true: input capture has occurred; false: overflow)
//      chan_num            Channel number
// Return value : None
**********************************/
void ATU5_TIMERA_Func::UpdateTimerStatus (const bool is_captured, const unsigned int chan_num)
{//{{{
    unsigned int reg_val = (unsigned int)(*TSRA);
    if (is_captured) {
        (*TSRA) = reg_val | (0x1<<chan_num);
    } else {
        (*TSRA)["OVFA"] = 1;
    }
}//}}}

/*********************************
// Function     : UpdateICRAVal
// Description  : Return I/O control setting
// Parameter    :
//      value               Captured value
//      chan_num            Channel number
//      is_icra2            Control updating to ICRA or ICRA2 register
// Return value : None
**********************************/
void ATU5_TIMERA_Func::UpdateICRAVal (const unsigned int value, const unsigned int chan_num, const bool is_icra2x)
{//{{{
    if (is_icra2x) {
        (*ICRA2[chan_num])["ICA2"] = value;
    } else {
        (*ICRA[chan_num])["ICA"] = value;
    }
}//}}}

/*********************************
// Function     : SetTCNTA
// Description  : Set counter value to TCNTA register
// Parameter    :
//      value               Counter value
// Return value : None
**********************************/
void ATU5_TIMERA_Func::SetTCNTA (unsigned int value)
{//{{{
    (*TCNTA) = value;
}//}}}

/*********************************
// Function     : SetNCNTA
// Description  : Set counter value to NCNTA register
// Parameter    :
//      value               Counter value
//      chan_num            Channel number
// Return value : None
**********************************/
void ATU5_TIMERA_Func::SetNCNTA (unsigned int value, const unsigned int chan_num)
{//{{{
    (*NCNTA[chan_num]) = value;
}//}}}

/*********************************
// Function     : cb_TCR1A_CKSELA
// Description  : Callback function when TCR1A register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void ATU5_TIMERA_Func::cb_TCR1A_CKSELA(RegCBstr str)
{//{{{
    if (mATU5_TIA_Base->IsTimerRunning()||((*TCR1A)["CKSELA"] > 5)) {
        mATU5_TIA_Base->print_msg("warning","Can not change selected counting clock.\n");
        (*TCR1A)["CKSELA"] = str.pre_data&0x7;
    }
}//}}}

/*********************************
// Function     : cb_TSCRA_ICFCA
// Description  : Callback function when TSCRA register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void ATU5_TIMERA_Func::cb_TSCRA_ICFCA(RegCBstr str)
{//{{{
    if ((unsigned int)(*TSCRA)["OVFCA"] == 1) {
        (*TSRA)["OVFA"] = 0;
    }
    if ((unsigned int)(*TSCRA)["ICFCA"] != 0) {
        unsigned int tscra_val = (unsigned int)(*TSCRA);
        unsigned int mask = tscra_val & 0xFF;
        unsigned int tsra_val = (unsigned int)(*TSRA);
        (*TSRA) = tsra_val & (~mask);
    }
    (*TSCRA) = 0;
}//}}}

/*********************************
// Function     : cb_NCMCR1A_NCM1A
// Description  : Callback function when NCMCR1A register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void ATU5_TIMERA_Func::cb_NCMCR1A_NCM1A(RegCBstr str)
{//{{{
    // only disable noise cancelling modes: premature-transition, minimum-at-level when NCNT = 0
    unsigned int pre_ncm1a = str.pre_data;
    for (unsigned int chan_num = 0; chan_num < 8; chan_num++) {
        unsigned int cur_ncm1a = (*NCMCR1A)["NCM1A"];
        if (((pre_ncm1a>>chan_num)&0x1) != ((cur_ncm1a>>chan_num)&0x1)) {
            (*NCMCR1A) = pre_ncm1a;// Set temporarily old value to register to get current noise cancel mode
            unsigned int noise_cancel_mode = mATU5_TIA_Base->GetNoiseCancelMode(chan_num);
            (*NCMCR1A) = cur_ncm1a;// Restore current value again.
            if ( ((noise_cancel_mode == 0) || (noise_cancel_mode == 1)) 
                && (mATU5_TIA_Base->GetNoiseCounterValue(chan_num) > 0) ){
                mATU5_TIA_Base->print_msg("warning","Can not change operation mode of noise canceler in channel %d.\n",chan_num);
                (*NCMCR1A)["NCM1A"] = (cur_ncm1a & (~((unsigned int)1 << chan_num)) & 0xFF) | (((pre_ncm1a>>chan_num)&0x1)<<chan_num);
            } else {
                mATU5_TIA_Base->SetNoiseCancelMode(chan_num);
            }
        }
    }
}//}}}

/*********************************
// Function     : cb_NCMCR2A_NCM2A
// Description  : Callback function when NCMCR2A register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void ATU5_TIMERA_Func::cb_NCMCR2A_NCM2A(RegCBstr str)
{//{{{
    // only disable noise cancelling modes: premature-transition, minimum-at-level when NCNT = 0
    unsigned int pre_ncm2a = str.pre_data;
    for (unsigned int chan_num = 0; chan_num < 8; chan_num++) {
        unsigned int cur_ncm2a = (unsigned int)(*NCMCR2A)["NCM2A"];
        if (((pre_ncm2a>>chan_num)&0x1) != ((cur_ncm2a>>chan_num)&0x1)) {
            (*NCMCR2A) = pre_ncm2a;// Set temporarily old value to register to get current noise cancel mode
            unsigned int noise_cancel_mode = mATU5_TIA_Base->GetNoiseCancelMode(chan_num);
            (*NCMCR2A) = cur_ncm2a;// Restore current value again.
            if ( ((noise_cancel_mode == 0) || (noise_cancel_mode == 1)) 
                && (mATU5_TIA_Base->GetNoiseCounterValue(chan_num) > 0) ){
                mATU5_TIA_Base->print_msg("warning","Can not change operation mode of noise canceler in channel %d.\n",chan_num);
                (*NCMCR2A)["NCM2A"] = (cur_ncm2a & (~((unsigned int)1 << chan_num)) & 0xFF) | (((pre_ncm2a>>chan_num)&0x1)<<chan_num);
            } else {
                mATU5_TIA_Base->SetNoiseCancelMode(chan_num);
            }
        }
    }
}//}}}

/*********************************
// Function     : cb_TIOR2A_NCEA
// Description  : Callback function when TIOR2A register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void ATU5_TIMERA_Func::cb_TIOR2A_NCEA(RegCBstr str)
{//{{{
    unsigned int pre_nckga = (str.pre_data>>16)&0xFF;
    unsigned int pre_ncka = (str.pre_data>>8)&0xFF;
    for (unsigned int chan_num = 0; chan_num < 8; chan_num++) {
        unsigned int cur_nckga = (unsigned int)(*TIOR2A)["NCKGA"]&0xFF;
        unsigned int cur_ncka = (unsigned int)(*TIOR2A)["NCKA"]&0xFF;
        if ((((str.pre_data>>chan_num)&0x1) == 0x1) && ((((*TIOR2A)["NCEA"]>>chan_num)&0x1) == 0x1)) {
            if ((((cur_nckga>>chan_num)&0x1) != ((pre_nckga>>chan_num)&0x1)) ||
                (((cur_ncka>>chan_num)&0x1) != ((pre_ncka>>chan_num)&0x1))) {
                mATU5_TIA_Base->print_msg("warning","Can not change selected counting clock of NCNTA%d when noise cancellation function for TIA%d is enabled.\n",chan_num, chan_num);
                (*TIOR2A)["NCKGA"] = (cur_nckga & (~((unsigned int)1 << chan_num)) & 0xFF) | (((pre_nckga>>chan_num)&0x1)<<chan_num);
                (*TIOR2A)["NCKA"] = (cur_ncka & (~((unsigned int)1 << chan_num)) & 0xFF) | (((pre_ncka>>chan_num)&0x1)<<chan_num);
            }
        } else if ((((*TIOR2A)["NCEA"]>>chan_num)&0x1) == 0x1) {
            mATU5_TIA_Base->EnableNoiseCancel(chan_num, true);
            mATU5_TIA_Base->SetNoiseCancelMode(chan_num);
        } else if ((((*TIOR2A)["NCEA"]>>chan_num)&0x1) == 0x0) {
            mATU5_TIA_Base->EnableNoiseCancel(chan_num, false);
        }
    }
}//}}}

/*********************************
// Function     : cb_TCNTA_CNTA
// Description  : Callback function when TCNTA register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void ATU5_TIMERA_Func::cb_TCNTA_CNTA(RegCBstr str)
{//{{{
    mATU5_TIA_Base->UpdateCounterValue();
}//}}}

/*********************************
// Function     : cb_NCNTA_NCNTA
// Description  : Callback function when NCNTA register is written
// Parameter    :
//      chan_num            Channel number
// Return value : None
**********************************/
void ATU5_TIMERA_Func::cb_NCNTA_NCNTA(const unsigned int chan_num)
{//{{{
    mATU5_TIA_Base->UpdateNoiseCounterValue(chan_num);
}//}}}

/*********************************
// Function     : cb_NCRA_NCTA
// Description  : Callback function when NCRA register is written
// Parameter    :
//      chan_num            Channel number
// Return value : None
**********************************/
void ATU5_TIMERA_Func::cb_NCRA_NCTA(const unsigned int chan_num)
{//{{{
    mATU5_TIA_Base->SetNoiseCORVal(chan_num, (unsigned int)(*NCRA[chan_num])["NCTA"]);
}//}}}

