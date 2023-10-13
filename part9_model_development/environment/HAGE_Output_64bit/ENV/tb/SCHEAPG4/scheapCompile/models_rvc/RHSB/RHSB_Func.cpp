// ---------------------------------------------------------------------
// $Id: RHSB_Func.cpp,v 1.4 2018/08/11 07:54:56 ngathunguyen Exp $
//
// Copyright(c) 2016 - 2018 Renesas Electronics Corporation
// Copyright(c) 2016 - 2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include <cstdarg>
#include "RHSB_Func.h"
#include "RHSB.h"

///Constructor of RHSB_Func class
RHSB_Func::RHSB_Func (std::string name, RHSB *RHSB_ptr):
           Crhsb_regif(name, 32),
           BusSlaveFuncIf()
{//{{{
    mRHSB_ptr = RHSB_ptr;
    mModuleName = name;
    mRHSB_ptr->CommandInit(name);
    Crhsb_regif::set_instance_name(name);
}//}}}

///Destructor of RHSB_Func class
RHSB_Func::~RHSB_Func (void)
{//{{{
    

}//}}}

///Handle commands of register IF class
void RHSB_Func::RegisterHandler (const std::vector<std::string>& args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {
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
                msg_return = Crhsb_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {
            msg_return = Crhsb_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Process reset function
void RHSB_Func::EnableReset (const bool is_active)
{//{{{
    Crhsb_regif::EnableReset(is_active);
}//}}}

///Return current state
std::string RHSB_Func::GetCurrentState (void)
{//{{{
    unsigned int ops_value = (unsigned int)(*RHSBGC)["OPS"];
    return GetStateName(ops_value).c_str();
}//}}}

///Return true if upstream is enable
bool RHSB_Func::IsUpstreamEnable (void)
{//{{{
    if ((unsigned int)(*RHSBUCR)["UE"] == 1) {
        return true;
    }
    return false;
}//}}}

///Return true if time out is enable
bool RHSB_Func::IsTimeOutEnable (void)
{//{{{
    if ((unsigned int)(*RHSBUCR)["TOE"] == 1) {
        return true;
    }
    return false;
}//}}}

///Get enable status of data transmission
bool RHSB_Func::IsDataTransmissionEnable (void)
{//{{{
    return ((bool) ((*RHSBDTC)["DTE"]));
}//}}}

///Return true if upstream mode is dedicated mode
bool RHSB_Func::IsDedicatedUpstreamMode (void)
{//{{{i
    if ((unsigned int)(*RHSBUCR)["UMS"] == 0) {
        return true;
    }
    return false;
}//}}}

///Return true if downstream mode is trigger mode
bool RHSB_Func::IsTriggerDownstreamMode (void)
{//{{{
    bool is_trigger_downstream_mode = false;
    if ((unsigned int)(*RHSBDCR)["DMS"] == 0x1) {
        is_trigger_downstream_mode = true;
    }
    return is_trigger_downstream_mode;
}//}}}

///Return true if downstream mode is trigger mode
bool RHSB_Func::IsDataAvailable (void)
{//{{{
    bool is_data_available = false;
    if ((unsigned int)(*RHSBIS)["DTSF"] == 0x0) {
        is_data_available = true;
    }
    return is_data_available;
}//}}}

///Return true if downstream mode is multi period mode
bool RHSB_Func::IsMultiPeriodDownstreamMode (void)
{//{{{
    bool is_multi_period_downstream_mode = false;
    if ((unsigned int)(*RHSBDCR)["DMS"] == 0x2) {
        is_multi_period_downstream_mode = true;
    }
    return is_multi_period_downstream_mode;
}//}}}

///Return true if command request is pending
bool RHSB_Func::IsCommandRequestPending (void)
{//{{{
    bool is_command_request_pending = false;
    if ((unsigned int)(*RHSBDTC)["CTR"] != 0) {
        is_command_request_pending = true;
    }
    return is_command_request_pending;
}//}}}

///Return true if transmission request is stopped
bool RHSB_Func::IsTransmissionStop (void)
{//{{{
    return (bool)((*RHSBDTC)["TSR"]);
}//}}}

///Return true if command frame insertion method is time-slot method
bool RHSB_Func::IsTimeSlotMethod (void)
{//{{{
    bool time_slot_method = false;
    if ((unsigned int)(*RHSBDCR)["CIM"] == 0x0) {
        time_slot_method = true;
    }
    return time_slot_method;
}//}}}

///Return true if command frame insertion method is immediate method
bool RHSB_Func::IsImmediateMethod (void)
{//{{{
    bool immediate_method = false;
    if ((unsigned int)(*RHSBDCR)["CIM"] == 0x1) {
        immediate_method = true;
    }
    return immediate_method;
}//}}}

///Return true if command frame time is over time stick
bool RHSB_Func::IsCommandFrameTimeOverTimeStick (void)
{//{{{
    bool is_over = false;
    unsigned int remained_time_stick = GetRepetitionPeriodLength() - mRHSB_ptr->GetCounterValue();
    if (remained_time_stick < mRHSB_ptr->GetFrameLength(true, 0)) {//If command frame length is over remained time stick
        is_over = true;
    }
    return is_over;
}//}}}

///Return true if interrupt status is active
bool RHSB_Func::IsInterruptStatusActive (const std::string bit_name)
{//{{{
    return ((bool)(*RHSBIS)[bit_name.c_str()]);
}//}}}

///Return true if interrupt is enable
bool RHSB_Func::IsInterruptEnable (const std::string bit_name)
{//{{{
    return ((bool)(*RHSBIC)[bit_name.c_str()]);
}//}}}

///Return true if downstream command DMS is enable
bool RHSB_Func::IsDownstreamCommandDMAEnable (void)
{//{{{
    return (bool)((*RHSBGC)["DCDE"]);
}//}}}

///Return true if downstream data DMA is enable
bool RHSB_Func::IsDownstreamDataDMAEnable (void)
{//{{{
    return (bool)((*RHSBGC)["DDE"]);
}//}}}

///Return true if upstream DMA is enable
bool RHSB_Func::IsUpstreamDMAEnable (void)
{//{{{
    return (bool)((*RHSBGC)["UDE"]);
}//}}}

///Return true if emergency input method is edge sensitive
bool RHSB_Func::IsEmergencyLevelSensitive (void)
{//{{{
   return (bool)((*RHSBDCR)["EIM"]); 
}//}}}

///Return true if clock serial output is active
bool RHSB_Func::IsClockSerialActive (void)
{//{{{
    return (bool)((*RHSBDCR)["CAC"]);
}//}}}

///Set activity status of upstream
void RHSB_Func::SetUpstreamActivityStatus (const unsigned int value)
{//{{{
    (*RHSBMSR)["UFA"] = value;
}//}}}

///Set interrupt status
void RHSB_Func::SetInterruptStatus (std::string bit_name)
{//{{{
    (*RHSBIS)[bit_name.c_str()] = 0x1;
}//}}}

///Set downstream activity
void RHSB_Func::SetDownstreamActivity (const unsigned int value)
{//{{{
    if(GetDownstreamBitRate() <= 5000000) {
        mRHSB_ptr->re_printf("info","Downstream frame activity RHSBMSR.DFA is changed to 0x%X\n", value);
        (*RHSBMSR)["DFA"] = value;
    }
}//}}}

///Set emergency signal raising fpag
void RHSB_Func::SetEmergencySignalRaisingFlag (void)
{//{{{
    (*RHSBIS)["ERF"] = 0x1;
}//}}}

///Clear enable status of data transmission
void RHSB_Func::ClearDataTransmissionEnable (void)
{//{{{
    (*RHSBDTC)["DTE"] = 0x0;
    (*RHSBDTC)["TSR"] = 0x0;
    (*RHSBIS)["DTSF"] = 0x0;
    mRHSB_ptr->ControlInterruptPorts(false, emINTDTSF);
}//}}}

///Return active channel
unsigned int RHSB_Func::GetActiveChannel (void)
{//{{{
    return (unsigned int)(*RHSBUCS)["ACC"];
}//}}}

///Return sequence length
unsigned int RHSB_Func::GetSeqquenceLength (const unsigned int period_num)
{//{{{
    unsigned int sequence_length = 0x0;
    if (period_num == 0) {
        sequence_length = (unsigned int)(*RHSBDCR)["SLS"];
    } else {
        sequence_length = (unsigned int)(*RHSBDCR1)["SLS1"];
    }
    return sequence_length;
}//}}}

///Return repetition period length
unsigned int RHSB_Func::GetRepetitionPeriodLength (void)
{//{{{
    return (unsigned int)((*RHSBDCR)["REP"] + 1);
}//}}}

///Return selected slave of DFTE
unsigned int RHSB_Func::GetDataTransmissionSelectedSlave (const unsigned int dfte_num)
{//{{{
    unsigned int slave_num = (unsigned int)(((*RHSBDEC)>>(28-8*dfte_num))&0x3);
    return slave_num;
}//}}}

///Return selected slave of command transmission
unsigned int RHSB_Func::GetCommandTransmissionSelectedSlave (void)
{//{{{
    return (unsigned int)(*RHSBDTC)["SSCF"];
}//}}}

///Return data bit number
unsigned int RHSB_Func::GetDataBitNum (const unsigned int dfte_num)
{//{{{
    unsigned int data_bit_num = (unsigned int)((((*RHSBDEC) >> (24-8*dfte_num)) & 0xF) + 1);
    return data_bit_num;
}//}}}

///Return command bit number
unsigned int RHSB_Func::GetCommandBitNum (void)
{//{{{
    return (unsigned int)(((*RHSBDTC)["NCB"])+1);
}//}}}

///Get selection bit
unsigned int RHSB_Func::GetCPS (const unsigned int slave_num)
{//{{{
    return (unsigned int)(((*RHSBSDC) >> (19-16*slave_num))&0x1); 
}//}}}

///Return assertion phase length
unsigned int RHSB_Func::GetAssertionPhaseLength (const unsigned int slave_num)
{//{{{
    unsigned int assertion_phase_length = (unsigned int)(((*RHSBSDC) >> (24-16*slave_num))&0x7);
    return assertion_phase_length; 
}//}}}

///Return deassertion phase length
unsigned int RHSB_Func::GetDeassertionPhaseLength (const unsigned int slave_num)
{//{{{
    unsigned int deassertion_phase_length = (unsigned int)(((*RHSBSDC) >> (16-16*slave_num))&0x7);
    return deassertion_phase_length; 
}//}}}

///Return passive phase length
unsigned int RHSB_Func::GetPassivePhaseLength (void)
{//{{{
    return (unsigned int)((*RHSBDCR)["DFP"] + 1); 
}//}}}

///Get time out counter
unsigned int RHSB_Func::GetTimeOutCounter (void)
{//{{{
    return (unsigned int)(((*RHSBUCR)["RTO"] + 1)*8);
}//}}}

///Get transmission period
unsigned int RHSB_Func::GetTransmissionPeriod (void)
{//{{{
    return (unsigned int)(*RHSBMSR)["TPS"];
}//}}}

///Get emergency polarity
unsigned int RHSB_Func::GetEmergencyPolarity (void)
{//{{{
    return (unsigned int)(*RHSBDCR)["EIP"];
}//}}}

///Get emergency enable value
unsigned int RHSB_Func::GetEmergencyEnableValue (void)
{//{{{
    return (unsigned int)(*RHSBDCR)["EE"];
}//}}}

bool RHSB_Func::IsResetState (void)
{//{{{
    if ((unsigned int)(*RHSBGC)["OPS"] == emRESET){
        return true;
    }
    return false;
}//}}}

bool RHSB_Func::IsCmdTransDelay (void)
{//{{{
    return (bool)((*RHSBDCR)["CTD"]);
}//}}}

bool RHSB_Func::IsPERR (void)
{//{{{
    return (bool)((*RHSBUDR)["PERR"]);
}//}}}

///Switch transmission period
void RHSB_Func::SwitchTransmissionPeriod (void)
{//{{{
    if ((unsigned int)(*RHSBMSR)["TPS"] == 0x0) {
        (*RHSBMSR)["TPS"] = 0x1;
    } else {
        (*RHSBMSR)["TPS"] = 0x0;
    }
    mRHSB_ptr->re_printf("info","Current transmission period is %d\n", (unsigned int)(*RHSBMSR)["TPS"]);
}//}}}

///Assemble data frame
void RHSB_Func::DataFrameAssemble (const bool is_emergency, unsigned short *data, const unsigned int period_num)
{//{{{
    unsigned int data_sequence_length = GetSeqquenceLength(period_num);
    for (unsigned int dfte = 0; dfte <= data_sequence_length; dfte++) {//Check each DFTE
        for (unsigned int bit_pos = 0; bit_pos < 16; bit_pos++) {
            unsigned int data_source_selection = ((unsigned int)(*RHSBDEBA[dfte])>>(2*bit_pos))&0x3;
            if (data_source_selection == 0x0) {//Bit from timer input
                sc_dt::uint64 timer_input_value = (sc_dt::uint64)mRHSB_ptr->GetTimerInputPort();
                data[dfte] = data[dfte] | ((unsigned short)((timer_input_value>>(16*dfte)) & (1<<bit_pos)));
            } else if (data_source_selection == 0x1) {//Inverted bit from timer input
                sc_dt::uint64 inverted_timer_input_value = (sc_dt::uint64)(~mRHSB_ptr->GetTimerInputPort());
                data[dfte] = data[dfte] | ((unsigned short)((inverted_timer_input_value>>(16*dfte)) & (1<<bit_pos)));
            } else if (data_source_selection == 0x2) {//Bit from downstream data register
                unsigned short ddr_value = 0x0;
                if ((dfte == 0) || (dfte == 1)) {
                    ddr_value = (unsigned short)((*RHSBDDR0)>>(16-16*dfte)) & 0xFFFF;
                } else {
                    ddr_value = (unsigned short)((*RHSBDDR1)>>(16-16*(dfte-2))) & 0xFFFF;
                }
                data[dfte] = data[dfte] | (ddr_value & (1<<bit_pos));
            }
            if (is_emergency) {//emergency is enable
                unsigned short ebe_value = 0x0;
                unsigned short ded_value = 0x0;
                if ((dfte == 0) || (dfte == 1)) {
                    ebe_value = (unsigned short)((*RHSBEBE0)>>(16-16*dfte)) & 0xFFFF;
                    ded_value = (unsigned short)((*RHSBDED0)>>(16-16*dfte)) & 0xFFFF;
                } else {
                    ebe_value = (unsigned short)((*RHSBEBE1)>>(16-16*(dfte-2))) & 0xFFFF;
                    ded_value = (unsigned short)((*RHSBDED1)>>(16-16*(dfte-2))) & 0xFFFF;
                }
                data[dfte] = (data[dfte]&(~ebe_value))|(ded_value&ebe_value);
            }
        } 
    }    
}//}}}

///Assemble command frame
void RHSB_Func::CommandFrameAssemble (sc_dt::uint64 &data, const unsigned int bit_len)
{//{{{
    data = (sc_dt::uint64)((*RHSBDCD) & ((0x1ULL<<bit_len) - 1));
}//}}}

///Return chip selection polarity
bool RHSB_Func::GetChipSelectionPolarity (const unsigned int slave_num)
{//{{{
    bool chip_selection_polarity = !((bool)(((*RHSBSDC)>>(21-16*slave_num)) & 0x1));
    return chip_selection_polarity;
}//}}}

///Return serial out line polarity
unsigned int RHSB_Func::GetSOLP (const unsigned int slave_num)
{//{{{
    return (unsigned int)(((*RHSBSDC)>>(20-16*slave_num)) & 0x1);
}//}}}

///Return clock line phase
unsigned int RHSB_Func::GetCLP (void)
{//{{{
    return (unsigned int)(*RHSBDCR)["CLP"];
}//}}}

///Return total time of data reception
double RHSB_Func::CalculateReceptionBitsTotalTime (const unsigned int line_id)
{//{{{
    double total_time = 0;
    total_time = GetContentBitNum(line_id)*GetUpstreamBitTime(line_id);
    return total_time;
}//}}}

///Return number content bits of data reception
unsigned int RHSB_Func::GetContentBitNum (const unsigned int line_id)
{//{{{
    unsigned int reception_bits_total = 0;
    if ((unsigned int)(*RHSBUCC)[GetBitName("UFT", line_id).c_str()] == 0) {
        reception_bits_total = 9;
    } else {
        reception_bits_total = 13;
    }
    return reception_bits_total;
}//}}}

///Return number stop bits of data reception
unsigned int RHSB_Func::GetStopBitNum (const unsigned int line_id)
{//{{{
    unsigned int stop_bits_total = 0;
    if ((unsigned int)(*RHSBUCC)[GetBitName("SBN", line_id).c_str()] == 0) {
        stop_bits_total = 2;
    } else {
        stop_bits_total = 3;
    }
    return stop_bits_total;
}//}}}

///Decode reception data
void RHSB_Func::StoreReceptionDataFrame (const unsigned int line_id, const unsigned int reception_data, const bool is_frame_error)
{//{{{
    if ((unsigned int)(*RHSBUCR)["FSM"] == 0) {//Normal storage method
        DecodeReceptionData(line_id, reception_data, is_frame_error);
    } else {//Addressed storage method
        if (GetFrameType(line_id) == 8) {//Frame type is 8-bits ([Parity][Data])
            DecodeReceptionData(emSlave0Channel, reception_data, is_frame_error);
        } else {//Frame type is 12-bits ([Parity][Extend][Data])
            unsigned int received_extended_datai_bits3_2 = (reception_data>>2)&0x3;
            if (received_extended_datai_bits3_2 == 0) {
                DecodeReceptionData(emSlave0Channel, reception_data, is_frame_error);
            } else if (received_extended_datai_bits3_2 == 1) {
                DecodeReceptionData(emSlave1Channel, reception_data, is_frame_error);
            } else {
                mRHSB_ptr->re_printf("warning","Reception data is not decoded because the received extended data field is wrong\n");
            }
        }
    }
}//}}}

///Set time out operation
void RHSB_Func::SetTimeOutOperation (const unsigned int line_id)
{//{{{
    (*RHSBUD[line_id])["TO"] = 1;
    if (line_id == (unsigned int)(*RHSBUDR)["LUD"]) {
        (*RHSBUDR)["TO"] = 1;
    }
    (*RHSBUSS)[GetBitName("TO", line_id).c_str()] = 1;
    SetInterruptStatus("TOF");
    mRHSB_ptr->ControlInterruptPorts(false, emINTTOF);
}//}}}

///Return transmission time of one bit on downstream communication
double RHSB_Func::GetDownstreamBitTime(void)
{//{{{
    double down_bit_time = 0;
    switch ((unsigned int)(*RHSBDCR)["DBR"]) {
        case 1: down_bit_time = mRHSB_ptr->GetCLK_RHSBPeriod()*2;
                break;
        case 3: down_bit_time = mRHSB_ptr->GetCLK_RHSBPeriod()*4;
                break;
        case 4: down_bit_time = mRHSB_ptr->GetCLK_RHSBPeriod()*5;
                break;
        case 7: down_bit_time = mRHSB_ptr->GetCLK_RHSBPeriod()*8;
                break;
        case 8: down_bit_time = mRHSB_ptr->GetCLK_RHSBPeriod()*16;
                break;
        case 9: down_bit_time = mRHSB_ptr->GetCLK_RHSBPeriod()*32;
                break;
        case 10: down_bit_time = mRHSB_ptr->GetCLK_RHSBPeriod()*64;
                break;
        case 11: down_bit_time = mRHSB_ptr->GetCLK_RHSBPeriod()*128;
                break;
        default: down_bit_time = mRHSB_ptr->GetCLK_RHSBPeriod()*256;
    }
    return down_bit_time;
}//}}}

///Return downstream bit rate
unsigned long long int RHSB_Func::GetDownstreamBitRate(void)
{//{{{
    unsigned long long int down_bit_rate = 0;
    switch ((unsigned int)(*RHSBDCR)["DBR"]) {
        case 1: down_bit_rate = (unsigned long long int)mRHSB_ptr->GetCLK_RHSBFreq()/2;
                break;
        case 3: down_bit_rate = (unsigned long long int)mRHSB_ptr->GetCLK_RHSBFreq()/4;
                break;
        case 4: down_bit_rate = (unsigned long long int)mRHSB_ptr->GetCLK_RHSBFreq()/5;
                break;
        case 7: down_bit_rate = (unsigned long long int)mRHSB_ptr->GetCLK_RHSBFreq()/8;
                break;
        case 8: down_bit_rate = (unsigned long long int)mRHSB_ptr->GetCLK_RHSBFreq()/16;
                break;
        case 9: down_bit_rate = (unsigned long long int)mRHSB_ptr->GetCLK_RHSBFreq()/32;
                break;
        case 10: down_bit_rate = (unsigned long long int)mRHSB_ptr->GetCLK_RHSBFreq()/64;
                break;
        case 11: down_bit_rate = (unsigned long long int)mRHSB_ptr->GetCLK_RHSBFreq()/128;
                break;
        default: down_bit_rate = (unsigned long long int)mRHSB_ptr->GetCLK_RHSBFreq()/256;
    }
    return down_bit_rate;
}//}}}

///Return transmission time of one bit on upstream communication
double RHSB_Func::GetUpstreamBitTime(const unsigned int line_id)
{//{{{
    double up_bit_time = 0;
    unsigned int ubr_bit = (unsigned int)((*RHSBUCC)[GetBitName("UBR", line_id).c_str()]);
    if ((ubr_bit >= 2) && (ubr_bit <= 8)) {
        up_bit_time = GetDownstreamBitTime()*(0x1<<ubr_bit);
    } else {
        up_bit_time = GetDownstreamBitTime()*(0x1<<9);
    }
    return up_bit_time;
}//}}}

double RHSB_Func::GetCountDownTime(const unsigned int line_id)
{//{{{
    double countdown_time = GetUpstreamBitTime(line_id)/8;
    return countdown_time;
}//}}}

///Clear command transmission request
void RHSB_Func::ClearCommandTransmissionRequest (void)
{//{{{
    (*RHSBDTC)["CTR"] = 0x0;
}//}}}

///Virtual function of BusSlaveFuncIf class
void RHSB_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void RHSB_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = trans.get_address() & 0xFF;
    if ((IsResetState()) && (addr!=0x0)){
        mRHSB_ptr->re_printf("warning","Cannot write due to model in RESET state (RHSBGC.OPS = 0)\n");
    } else {
        if (!debug) {
            reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    }
}//}}}

///Get operation state
std::string RHSB_Func::GetStateName(const unsigned int ops)
{//{{{
    std::string ops_name = "";
    switch (ops) {
        case emRESET:
            ops_name = "RESET";
            break;
        case emCONFIG:
            ops_name = "CONFIG";
            break;
        case emACTIVE:
            ops_name = "ACTIVE";
            break;
        default: ops_name = "TEST";
    }
    return ops_name.c_str();
}//}}}

///Return bit name
std::string RHSB_Func::GetBitName(const std::string name, const unsigned int line_id)
{//{{{
    std::ostringstream bit_name;
    bit_name.str("");
    bit_name << name << line_id;
    return bit_name.str().c_str();
}//}}}

void RHSB_Func::FinishConfChange (void)
{//{{{
    re_printf("warning", "BSY = 0");
    (*RHSBUCS)["BSY"] = 0x0;
}//}}}

///Configure RHSB
void RHSB_Func::ConfigureRHSB(void)
{//{{{
    (*RHSBMSR)["DFA"] = 0x0;
    (*RHSBMSR)["TPS"] = 0x1;
    (*RHSBDTC)["CTR"] = 0x0;
    (*RHSBDTC)["DTE"] = 0x0;
    (*RHSBUCS)["BSY"] = 0x0;
    for (unsigned int index = 0; index < 2; index++) {
        (*(RHSBUD[index]))["DL"] = 0x0;
        (*(RHSBUD[index]))["TO"] = 0x0;
        (*(RHSBUD[index]))["FERR"] = 0x0;
        (*(RHSBUD[index]))["PERR"] = 0x0;
        (*(RHSBUD[index]))["ND"] = 0x0;
        (*RHSBUSS)[GetBitName("DL", index).c_str()] = 0;
        (*RHSBUSS)[GetBitName("TO", index).c_str()] = 0;
        (*RHSBUSS)[GetBitName("FERR", index).c_str()] = 0;
        (*RHSBUSS)[GetBitName("PERR", index).c_str()] = 0;
        (*RHSBUSS)[GetBitName("ND", index).c_str()] = 0;
        if (index == (unsigned int)(*RHSBUDR)["LUD"]) {
            (*RHSBUDR)["DL"] = 0;
            (*RHSBUDR)["TO"] = 0;
            (*RHSBUDR)["FERR"] = 0;
            (*RHSBUDR)["PERR"] = 0;
            (*RHSBUDR)["ND"] = 0;
        }
        (*RHSBUDR)[GetBitName("NDS", index).c_str()] = 0;
    }
    (*RHSBIS) = 0x0;
}//}}}

///Return parity error status
bool RHSB_Func::CheckParityError (const unsigned int line_id, const unsigned int data)
{//{{{
    bool parity_error_status = false;
    unsigned int parity_control_value = (unsigned int)(*RHSBUCC)[GetBitName("PC", line_id).c_str()];
    unsigned int parity_bit = 0;
    for (unsigned int index = 0; index <= GetFrameType(line_id); index++) {
        parity_bit = parity_bit ^ ((data >> index)&0x1);
    }
    if (parity_bit != parity_control_value) {
        parity_error_status = true;
    }
    return parity_error_status;
}//}}}

///Return frame type
unsigned int RHSB_Func::GetFrameType(const unsigned int line_id)
{//{{{
    unsigned int frame_type = 0x0;
    if ((unsigned int)((*RHSBUCC)[GetBitName("UFT", line_id).c_str()]) == 0x0) {//Frame type is 8 bits
        frame_type = 8;
    } else {//Frame type is 12 bits
        frame_type = 12;
    }
    return frame_type;
}//}}}

///Storage reception data
void RHSB_Func::DecodeReceptionData (const unsigned int line_id, const unsigned int reception_data, const bool is_frame_error)
{//{{{
    if (is_frame_error) {//Stop bits error
        (*RHSBUD[line_id])["FERR"] = 0x1;
        if (line_id == (unsigned int)(*RHSBUDR)["LUD"]) {
            (*RHSBUDR)["FERR"] = 0x1;
        }
        (*RHSBUSS)[GetBitName("FERR", line_id).c_str()] = 0x1;
        SetInterruptStatus("UEF");
        mRHSB_ptr->ControlInterruptPorts(false, emINTUEF);
    } else {//No stop bits error
        bool parity_error = CheckParityError(line_id, reception_data);
        if (parity_error) {//Parity error
            (*RHSBUD[line_id])["PERR"] = 0x1;
            if (line_id == (unsigned int)(*RHSBUDR)["LUD"]) {
                (*RHSBUDR)["PERR"] = 0x1;
            }
            (*RHSBUSS)[GetBitName("PERR", line_id).c_str()] = 0x1;
            SetInterruptStatus("UEF");
            mRHSB_ptr->ControlInterruptPorts(false, emINTUEF);
        } else {//Valid reception data
            if (((!(IsDedicatedUpstreamMode())) && (GetFrameType(emSlave0Channel) == 12)) //upstream mode is shared mode -> check UFT of channel 0
             || ((IsDedicatedUpstreamMode()) && (GetFrameType(line_id) == 12))) {// upstream mode is dedicated mode -> check UFT of channel
                (*RHSBUD[line_id])["DF"] = (reception_data>>0x4)&0xFF;
                (*RHSBUD[line_id])["EDF"] = reception_data&0xF;
            } else {
                (*RHSBUD[line_id])["DF"] = reception_data&0xFF;
            }
            if ((unsigned int)(*RHSBUD[line_id])["ND"] == 1) {//Data lost detected
                (*RHSBUD[line_id])["DL"] = 1;
                (*RHSBUSS)[GetBitName("DL", line_id).c_str()] = 0x1;
                SetInterruptStatus("DLF");
                mRHSB_ptr->ControlInterruptPorts(false, emINTDLF);
            }
            (*RHSBUD[line_id])["ND"] = 1;
            (*RHSBUDR)["LUD"] = line_id;
            (*RHSBUDR)["ND"] = 1;
            (*RHSBUDR)[GetBitName("NDS", line_id).c_str()] = 1;
            (*RHSBUSS)[GetBitName("ND", line_id).c_str()] = 1;
            (*RHSBUDR)["TO"] = (unsigned int)(*RHSBUD[line_id])["TO"];
            (*RHSBUDR)["DL"] = (unsigned int)(*RHSBUD[line_id])["DL"];
            (*RHSBUDR)["FERR"] = (unsigned int)(*RHSBUD[line_id])["FERR"];
            (*RHSBUDR)["PERR"] = (unsigned int)(*RHSBUD[line_id])["PERR"];
            (*RHSBUDR)["EDF"] = (unsigned int)(*RHSBUD[line_id])["EDF"];
            (*RHSBUDR)["DF"] = (unsigned int)(*RHSBUD[line_id])["DF"];
            SetInterruptStatus("DRF");
            mRHSB_ptr->ControlInterruptPorts(false, emINTL3);
            mRHSB_ptr->ControlInterruptPorts(false, emUpDMA);
        }
    }
}//}}}

///Define call-back function of RHSBGC register in rhsb_regif class
void RHSB_Func::cb_RHSBGC_OPS(RegCBstr str)
{//{{{
    unsigned int pre_ops = str.pre_data & 0x3;
    unsigned int cur_ops = str.data & 0x3;
    if ((pre_ops == emRESET) && (cur_ops == emACTIVE) || //Change from RESET state to ACTICE state
        (pre_ops == emRESET) && (cur_ops == emTEST)   || //Change from RESET state to TEST state
        (pre_ops == emACTIVE) && (cur_ops == emTEST)  || //Change from ACTIVE state to TEST state 
        (pre_ops == emTEST) && (cur_ops == emACTIVE)) {  //Change from TEST state to ACTIVE state
        mRHSB_ptr->re_printf("warning","Can not change from %s state to %s state\n", GetStateName(pre_ops).c_str(), GetStateName(cur_ops).c_str());
        (*RHSBGC)["OPS"] = str.pre_data&0x3;
    } else {
        if (((unsigned int)(*RHSBIC)["CTIE"] == 1) && ((str.data >> 18)&0x1) == 1) {//Write 1 to DCDE bit is ignored when RHSBIC.CTIE = 1
            mRHSB_ptr->re_printf("warning","Write 1 to DCDE bit is ignored when RHSBIC.CTIE = 1\n");
            (*RHSBGC)["DCDE"] = (str.pre_data >> 18)&0x1;
        }
        if (((unsigned int)(*RHSBIC)["DTSIE"] == 1) && ((str.data >> 17)&0x1) == 1) {//Write 1 to DDE bit is ignored when RHSBIC.DTSIE = 1
            mRHSB_ptr->re_printf("warning","Write 1 to DDE bit is ignored when RHSBIC.DTSIE = 1\n");
            (*RHSBGC)["DDE"] = (str.pre_data >> 17)&0x1;
        }
        if (((unsigned int)(*RHSBIC)["DRIE"] == 1) && ((str.data >> 16)&0x1) == 1) {//Write 1 to UDE bit is ignored when RHSBIC.DRIE = 1
            mRHSB_ptr->re_printf("warning","Write 1 to UDE bit is ignored when RHSBIC.DRIE = 1\n");
            (*RHSBGC)["UDE"] = (str.pre_data >> 16)&0x1;
        }
        if (cur_ops == emCONFIG) {//Go to CONFIG state
            ConfigureRHSB();
        } else {
            re_printf("warning", "BSY = 1");
            (*RHSBUCS)["BSY"] = 1;
            mRHSB_ptr->ACCChangeDone();
            if (cur_ops == emRESET) {
                mRHSB_ptr->EnableReset(true);
                mRHSB_ptr->EnableReset(false);
            }
        }
        if ((cur_ops == emACTIVE) || (cur_ops == emTEST)) {//Go to ACTIVE state or TEST state
            if (cur_ops == emTEST) {
                mRHSB_ptr->mSOVal = 0xFFFFFFFFFFFFFFFFULL;
                mRHSB_ptr->mSOPortEvent.notify(1,SC_FS);
                mRHSB_ptr->mFCLVal = 0;
                mRHSB_ptr->mFCLPortEvent.notify(1,SC_FS);
                mRHSB_ptr->mCSD0Val = GetChipSelectionPolarity(0);
                mRHSB_ptr->mCSDPortEvent[0].notify(1,SC_FS);
                mRHSB_ptr->mCSD1Val = GetChipSelectionPolarity(1);
                mRHSB_ptr->mCSDPortEvent[1].notify(1,SC_FS);
            } else {//ACTIVE state
                if (IsClockSerialActive()) {
                    mRHSB_ptr->mFCLVal = GetDownstreamBitRate();
                    mRHSB_ptr->mFCLPortEvent.notify(1,SC_FS);
                }
            }
            if (((unsigned int)(*RHSBDCR)["DMS"] == 0x0) || ((unsigned int)(*RHSBDCR)["DMS"] == 0x2)) {//Single-period mode or multi-period mode
                mRHSB_ptr->SetCounterPeriod(GetDownstreamBitTime());
                mRHSB_ptr->SetCompareMatchValue(GetRepetitionPeriodLength());
                mRHSB_ptr->StartStopTimer(true);
            }
        }
    }
}//}}}

///Define call-back function of RHSBDCR register in rhsb_regif class
void RHSB_Func::cb_RHSBDCR_DMS(RegCBstr str)
{//{{{
    if ((unsigned int)(*RHSBGC)["OPS"] == emCONFIG) { 
        if (((unsigned int)(*RHSBDCR)["DBR"] == 0) || ((unsigned int)(*RHSBDCR)["DBR"] == 2) || ((unsigned int)(*RHSBDCR)["DBR"] == 5) || ((unsigned int)(*RHSBDCR)["DBR"] == 6)) {
            mRHSB_ptr->re_printf("warning","Invalid value is set to DBR bit\n");
            (*RHSBDCR)["DBR"] = (str.pre_data>>20)&0xF;
        }
        if ((unsigned int)(*RHSBDCR)["CIM"] == 3) {
            mRHSB_ptr->re_printf("warning","Invalid value is set to CIM bit\n");
            (*RHSBDCR)["CIM"] = (str.pre_data>>18)&0x3;
        }
        if ((unsigned int)(*RHSBDCR)["EE"] == 2) {
            mRHSB_ptr->re_printf("warning","Invalid value is set to EE bit\n");
            (*RHSBDCR)["EE"] = (str.pre_data>>4)&0x3;
        }
        if ((unsigned int)(*RHSBDCR)["DMS"] == 3) {
            mRHSB_ptr->re_printf("warning","Invalid value is set to DMS bit\n");
            (*RHSBDCR)["DMS"] = str.pre_data&0x3;
        }
    } else {
        mRHSB_ptr->re_printf("warning","Cannot write a value to bits of RHSBDCR register (except CAC bit) if operation state is different from CONFIG state\n");
        *RHSBDCR = (str.pre_data & (~(0x1<<6))) | (str.data&0x40);
    }
}//}}}

///Define call-back function of RHSBDEC register in rhsb_regif class
void RHSB_Func::cb_RHSBDEC_NDB3(RegCBstr str)
{//{{{
    if ((unsigned int)(*RHSBGC)["OPS"] == emCONFIG) {
        unsigned int ssd0 = (str.data>>28)&0x3;
        unsigned int ssd1 = (str.data>>20)&0x3;
        unsigned int ssd2 = (str.data>>12)&0x3;
        unsigned int ssd3 = (str.data>>4)&0x3;
        if ((ssd0 == 0x2) || (ssd0 == 0x3)) {
            mRHSB_ptr->re_printf("warning","Write value 0x2 or 0x3 to SSD0 bit is prohibited\n");
            (*RHSBDEC)["SSD0"] = (str.pre_data>>28)&0x3;
        }
        if ((ssd1 == 0x2) || (ssd1 == 0x3)) {
            mRHSB_ptr->re_printf("warning","Write value 0x2 or 0x3 to SSD1 bit is prohibited\n");
            (*RHSBDEC)["SSD1"] = (str.pre_data>>20)&0x3;
        }
        if ((ssd2 == 0x2) || (ssd2 == 0x3)) {
            mRHSB_ptr->re_printf("warning","Write value 0x2 or 0x3 to SSD2 bit is prohibited\n");
            (*RHSBDEC)["SSD2"] = (str.pre_data>>12)&0x3;
        }
        if ((ssd3 == 0x2) || (ssd3 == 0x3)) {
            mRHSB_ptr->re_printf("warning","Write value 0x2 or 0x3 to SSD3 bit is prohibited\n");
            (*RHSBDEC)["SSD3"] = (str.pre_data>>4)&0x3;
        }
    } else {
        mRHSB_ptr->re_printf("warning","Cannot write a value to RHSBDEC register if operation state is different from CONFIG state\n");
        *RHSBDEC = str.pre_data;
    }
}//}}}

///Define call-back function of RHSBSDC register in rhsb_regif class
void RHSB_Func::cb_RHSBSDC_DPL1(RegCBstr str)
{//{{{
    if ((unsigned int)(*RHSBGC)["OPS"] != emCONFIG) {
        mRHSB_ptr->re_printf("warning","Cannot write a value to RHSBSDC register if operation state is different from CONFIG state\n");
        *RHSBSDC = str.pre_data;
    } else {//In configure state, chip selection inactive     
         mRHSB_ptr->mCSD0Val = !GetChipSelectionPolarity(0);
         mRHSB_ptr->mCSDPortEvent[0].notify(1,SC_NS);
         mRHSB_ptr->mCSD1Val = !GetChipSelectionPolarity(1);
         mRHSB_ptr->mCSDPortEvent[1].notify(2,SC_NS);
    }
}//}}}

///Define call-back function of RHSBDEBA register in rhsb_regif class
void RHSB_Func::cb_RHSBDEBA_DSS0(RegCBstr str)
{//{{{
    if ((unsigned int)(*RHSBGC)["OPS"] != emCONFIG) {
        mRHSB_ptr->re_printf("warning","Cannot write a value to RHSBDEBA register if operation state is different from CONFIG state\n");
        *RHSBDEBA[str.channel] = str.pre_data;
    } else {
        for (unsigned int index = 0; index < 16; index++) {
            std::ostringstream bit_name;
            bit_name.str("");
            bit_name << "DSS" << index;
            if ((unsigned int)(*RHSBDEBA[str.channel])[bit_name.str().c_str()] == 0x3) {//Invalid value
                (*RHSBDEBA[str.channel])[bit_name.str().c_str()] = (str.pre_data>>(2*index)) & 0x3;
            }
        }
    }
}//}}}

///Define call-back function of RHSBEBE0 register in rhsb_regif class
void RHSB_Func::cb_RHSBEBE0_EBE23_16(RegCBstr str)
{//{{{
    if ((unsigned int)(*RHSBGC)["OPS"] != emCONFIG) {
        mRHSB_ptr->re_printf("warning","Cannot write a value to RHSBEBE0 register if operation state is different from CONFIG state\n");
        *RHSBEBE0 = str.pre_data;
    }
}//}}}

///Define call-back function of RHSBEBE1 register in rhsb_regif class
void RHSB_Func::cb_RHSBEBE1_EBE55_48(RegCBstr str)
{//{{{
    if ((unsigned int)(*RHSBGC)["OPS"] != emCONFIG) {
        mRHSB_ptr->re_printf("warning","Cannot write a value to RHSBEBE1 register if operation state is different from CONFIG state\n");
        *RHSBEBE1 = str.pre_data;
    }
}//}}}

///Define call-back function of RHSBDTC register in rhsb_regif class
void RHSB_Func::cb_RHSBDTC_DTE(RegCBstr str)
{//{{{
    //Check writing to NCB and SSCF bits
    unsigned int pre_ctr_bit = (str.pre_data>>16)&0x3;
    unsigned int cur_ctr_bit = (str.data>>16)&0x3;
    if ((pre_ctr_bit != 0x0) || ((unsigned int)(*RHSBGC)["DCDE"] == 1)) {//Check previous CTR bit and RHSBGC.DCDE bit
        mRHSB_ptr->re_printf("warning","Cannot write a value to NCB and SSCF bits\n");
        (*RHSBDTC)["NCB"] = (str.pre_data>>24)&0x1F;
        (*RHSBDTC)["SSCF"] = (str.pre_data>>20)&0x3;
    } else {
        unsigned int cur_sscf = (str.data>>20)&0x3;
        if ((cur_sscf == 0x2) || (cur_sscf == 0x3)) {
            mRHSB_ptr->re_printf("warning","Write value 0x2 or 0x3 to SSCF bit is prohibited\n");
            (*RHSBDTC)["SSCF"] = (str.pre_data>>20)&0x3;
        }
    }

    //Check writing to CTR bit and DTE bit
    bool is_downstream_transmission = false;
    if ((unsigned int)(*RHSBGC)["OPS"] != emCONFIG) {//If operation state is different from CONFIG state
        if (pre_ctr_bit == 0x0) {//Check previous CTR bit
            if (cur_ctr_bit != 0x2) {//Valid value
                if (cur_ctr_bit == 0x1) {//If CTR bit is 1
                    is_downstream_transmission = true;
                    (*RHSBIS)["CTF"] = 0; //When command transmission request -> bit set 0
                    mRHSB_ptr->ControlInterruptPorts(false, emINTCTF);
                    mRHSB_ptr->NotifyCmdRequest();
                } else if (cur_ctr_bit == 0x3) {//If CTR bit is 3
                    if ((unsigned int)(*RHSBUCS)["BSY"] == 0x0) {
                        (*RHSBUCS)["ACC"] = (unsigned int)(*RHSBDTC)["SSCF"];
                        is_downstream_transmission = true;
                        (*RHSBIS)["CTF"] = 0; //When command transmission request -> bit set 0
                        mRHSB_ptr->ControlInterruptPorts(false, emINTCTF);
                        mRHSB_ptr->NotifyCmdRequest();
                    } else {//BSY bit is 0x1
                        mRHSB_ptr->re_printf("warning","Cannot write value 0x3 to CTR bit while RHSBUCS.BSY is 0x1\n");
                        (*RHSBDTC)["CTR"] = (str.pre_data>>16)&0x3;
                    }
                }
                if ((unsigned int)(*RHSBGC)["DCDE"] == 1) { //add by QN
                    mRHSB_ptr->NotifyDMAResponse();
                }
            } else {//Invalid value
                mRHSB_ptr->re_printf("warning","Cannot write value 0x2 to CTR bit\n");
                (*RHSBDTC)["CTR"] = pre_ctr_bit;
            }
        } else {
            mRHSB_ptr->re_printf("warning","Cannot write a value to CTR bit\n");
            (*RHSBDTC)["CTR"] = (str.pre_data>>16)&0x3;
        }

        if ((str.data&0x1) == 1) {//If write 1 to DTE bit
            if ((str.pre_data&0x1) == 0){//If DTE bit changes from 0 to 1
                is_downstream_transmission = true;
                if (IsTriggerDownstreamMode()) {
                    (*RHSBIS)["DTSF"] = 1;
                    mRHSB_ptr->ControlInterruptPorts(false, emINTDTSF);
                }
                if (IsMultiPeriodDownstreamMode()) {
                    (*RHSBMSR)["TPS"] = 0x1;
                    mRHSB_ptr->mIsFstData = true;
                }
            }
        } else {
            mRHSB_ptr->re_printf("warning","Cannot write 0 to DTE bit\n");
            (*RHSBDTC)["DTE"] = (str.pre_data)&0x1;
        }
    } else {
        mRHSB_ptr->re_printf("warning","Cannot write a value to CTR and DTE bits while operation state is CONFIG state\n");
        (*RHSBDTC)["CTR"] = (str.pre_data>>16)&0x3;
        (*RHSBDTC)["DTE"] = (str.pre_data)&0x1;
    }

    //Check writing to TSR bit
    if (((str.data>>1)&0x1) == 0) {//If user write 0 to TSR bit
        mRHSB_ptr->re_printf("warning","Can not write 0 to TSR bit\n");
        (*RHSBDTC)["TSR"] = (str.pre_data>>1)&0x1;
    }
    if ((((str.data>>1)&0x1) == 1)&&((unsigned int)(*RHSBDTC)["DTE"] == 0)) {//If user write 1 while DTE bit is 0
        mRHSB_ptr->re_printf("warning","Can not write 1 to TSR bit while DTE is 0\n");
        (*RHSBDTC)["TSR"] = (str.pre_data>>1)&0x1;
    }
    if ((unsigned int)(*RHSBDTC)["TSR"] == 1) {
        is_downstream_transmission = true;
        mRHSB_ptr->NotifyStopDataTrans();
    }
    if (is_downstream_transmission) {
        mRHSB_ptr->NotifyDownstreamTransmission();
    }
}//}}}

///Define call-back function of RHSBDCD register in rhsb_regif class
void RHSB_Func::cb_RHSBDCD_CB(RegCBstr str)
{//{{{
    if ((unsigned int)(*RHSBDTC)["CTR"] != 0) {
        mRHSB_ptr->re_printf("warning","Can not write a value to CB bit\n");
        *RHSBDCD = str.pre_data;
    }
}//}}}

///Define call-back function of RHSBDDR0 register in rhsb_regif class
void RHSB_Func::cb_RHSBDDR0_DB31_16(RegCBstr str)
{//{{{
    if (((unsigned int)(*RHSBIS)["DTSF"] == 0) && ((unsigned int)(*RHSBDTC)["DTE"] == 1)) {
        mRHSB_ptr->re_printf("warning","Can not write a value to RHSBDDR0 register\n");
        *RHSBDDR0 = str.pre_data;
    }
    if ((unsigned int)(*RHSBIS)["DTSF"] == 1) {
        if (((GetSeqquenceLength(0) == 0)&&((unsigned int)(*RHSBDDR0)["DB15_0"] != ((str.pre_data >> 15)&0xFFFF))) || 
            ((GetSeqquenceLength(0) == 1)&&((unsigned int)(*RHSBDDR0)["DB31_16"] != (str.pre_data&0xFFFF)))) {
            (*RHSBIS)["DTSF"] = 0;
            (*RHSBIS)["DTF"] = 0;
            mRHSB_ptr->ControlInterruptPorts(false, emINTDTSF);
            mRHSB_ptr->ControlInterruptPorts(false, emINTDTF);
            if (IsTriggerDownstreamMode()) {//DTSF change from 1 to 0 in trigger mode
                mRHSB_ptr->NotifyDownstreamTransmission();
            }
            if ((unsigned int)(*RHSBGC)["DDE"] == 1) { //add by QN
                mRHSB_ptr->NotifyDMAResponse();
            }
        }
    }
}//}}}

///Define call-back function of RHSBDDR1 register in rhsb_regif class
void RHSB_Func::cb_RHSBDDR1_DB63_48(RegCBstr str)
{//{{{
    if (((unsigned int)(*RHSBIS)["DTSF"] == 0) && ((unsigned int)(*RHSBDTC)["DTE"] == 1)) {
        mRHSB_ptr->re_printf("warning","Can not write a value to RHSBDDR1 register\n");
        *RHSBDDR1 = str.pre_data;
    }
    if ((unsigned int)(*RHSBIS)["DTSF"] == 1) {
        if (((GetSeqquenceLength(0) == 2)&&((unsigned int)(*RHSBDDR1)["DB47_32"] != ((str.pre_data >> 15)&0xFFFF))) || 
            ((GetSeqquenceLength(0) == 3)&&((unsigned int)(*RHSBDDR1)["DB63_48"] != (str.pre_data&0xFFFF)))) {
            (*RHSBIS)["DTSF"] = 0;
            (*RHSBIS)["DTF"] = 0;
            mRHSB_ptr->ControlInterruptPorts(false, emINTDTSF);
            mRHSB_ptr->ControlInterruptPorts(false, emINTDTF);
            if (IsTriggerDownstreamMode()) {//DTSF change from 1 to 0 in trigger mode
                mRHSB_ptr->NotifyDownstreamTransmission();
            }
            if ((unsigned int)(*RHSBGC)["DDE"] == 1) { //add by QN
                mRHSB_ptr->NotifyDMAResponse();
            }
        }
    }
}//}}}

///Define call-back function of RHSBDED0 register in rhsb_regif class
void RHSB_Func::cb_RHSBDED0_EB31_16(RegCBstr str)
{//{{{
    if ((unsigned int)(*RHSBGC)["OPS"] != emCONFIG) {
        mRHSB_ptr->re_printf("warning","Cannot write a value to RHSBDED0 register while operation state is different from CONFIG state\n");
        *RHSBDED0 = str.pre_data;
    }
}//}}}

///Define call-back function of RHSBDED1 register in rhsb_regif class
void RHSB_Func::cb_RHSBDED1_EB63_48(RegCBstr str)
{//{{{
    if ((unsigned int)(*RHSBGC)["OPS"] != emCONFIG) {
        mRHSB_ptr->re_printf("warning","Cannot write a value to RHSBDED1 register while operation state is different from CONFIG state\n");
        *RHSBDED1 = str.pre_data;
    }
}//}}}

///Define call-back function of RHSBUCR register in rhsb_regif class
void RHSB_Func::cb_RHSBUCR_FSM(RegCBstr str)
{//{{{
    if ((unsigned int)(*RHSBGC)["OPS"] != emCONFIG) {
        mRHSB_ptr->re_printf("warning","Cannot write a value to RHSBUCR register while operation state is different from CONFIG state\n");
        *RHSBUCR = str.pre_data;
    }
}//}}}

///Define call-back function of RHSBUCC register in rhsb_regif class
void RHSB_Func::cb_RHSBUCC_UBR1(RegCBstr str)
{//{{{
    if ((unsigned int)(*RHSBGC)["OPS"] != emCONFIG) {
        mRHSB_ptr->re_printf("warning","Cannot write a value to RHSBUCC register while operation state is different from CONFIG state\n");
        *RHSBUCC = str.pre_data;
    } else {
        unsigned int ubr0 = (str.data>>24)&0xF;
        unsigned int ubr1 = (str.data>>16)&0xF;
        if (ubr0 < 0x3) {
            mRHSB_ptr->re_printf("warning","Write value 0x%x to UBR0 bit is prohibited\n",ubr0);
            (*RHSBUCC)["UBR0"] = (str.pre_data>>24)&0xF;
        }
        if (ubr1 < 0x3) {
            mRHSB_ptr->re_printf("warning","Write value 0x%x to UBR1 bit is prohibited\n",ubr1);
            (*RHSBUCC)["UBR1"] = (str.pre_data>>16)&0xF;
        }
    }
}//}}}

///Define call-back function of RHSBUCS register in rhsb_regif class
void RHSB_Func::cb_RHSBUCS_ACC(RegCBstr str)
{//{{{
    unsigned int cur_acc = (str.data>>24)&0x3;
    (*RHSBUCS)["BSY"] = (str.pre_data>>31)&0x1;//BSY is read only bit
    if (((unsigned int)(*RHSBUCS)["BSY"] == 1) || (cur_acc == 0x2) || (cur_acc == 0x3)) {
        (*RHSBUCS)["ACC"] = (str.pre_data>>24)&0x3;
        mRHSB_ptr->re_printf("warning","Cannot write a value to ACC bit while processing configuration change or invalid value\n");
    }
    if (((unsigned int)(*RHSBUCS)["ACC"] != (str.pre_data>>24)&0x3) && ((unsigned int)(*RHSBGC)["OPS"] != emCONFIG)) {
        (*RHSBUCS)["BSY"] = 1;
        mRHSB_ptr->ACCChangeDone();
    }
}//}}}

///Define call-back function of RHSBUDR register in rhsb_regif class
void RHSB_Func::cb_RHSBUDR_DF(RegCBstr str)
{//{{{
    if ((unsigned int)(*RHSBGC)["UDE"] == 0x1) {
        mRHSB_ptr->re_printf("warning","Reading RHSBUDR register is prohibited\n");
    } else {
        unsigned int channel = (*RHSBUDR)["LUD"];
        (*RHSBIS)["DRF"] = 0x0;
        mRHSB_ptr->ControlInterruptPorts(false, emINTL3);
        (*RHSBUD[channel])["ND"] = 0x0;
        (*RHSBUDR)["ND"] = 0;
        (*RHSBUDR)[GetBitName("NDS", channel).c_str()] = 0;
        (*RHSBUSS)[GetBitName("ND", channel).c_str()] = 0;
    }
}//}}}

///Define call-back function of RHSBIC register in rhsb_regif class
void RHSB_Func::cb_RHSBIC_DTSIE(RegCBstr str)
{//{{{
    //Check writing to DTSIE bit
    unsigned int cur_dtsie = (str.data>>16)&0x1;
    if ((cur_dtsie == 1) && ((unsigned int)(*RHSBGC)["DDE"] == 1)) {
        mRHSB_ptr->re_printf("warning","Cannot write value 1 to DTSIE while downstream data DMA support is enabled\n");
        (*RHSBIC)["DTSIE"] = (str.pre_data>>16)&0x1;
    }
    //Check writing to CTIE bit
    unsigned int cur_ctie = (str.data>>18)&0x1;
    if ((cur_ctie == 1) && ((unsigned int)(*RHSBGC)["DCDE"] == 1)) {
        mRHSB_ptr->re_printf("warning","Cannot write value 1 to CTIE while downstream command DMA support is enabled\n");
        (*RHSBIC)["CTIE"] = (str.pre_data>>18)&0x1;
    }
    //Check writing to DRIE bit
    unsigned int cur_drie = (str.data>>24)&0x1;
    if ((cur_drie == 1) && ((unsigned int)(*RHSBGC)["UDE"] == 1)) {
        mRHSB_ptr->re_printf("warning","Cannot write value 1 to DRIE while downstream command DMA support is enabled\n");
        (*RHSBIC)["DRIE"] = (str.pre_data>>24)&0x1;
    }
}//}}}

///Define call-back function of RHSBIS register in rhsb_regif class
void RHSB_Func::cb_RHSBIS_ERF(RegCBstr str)
{//{{{
    unsigned int cur_drf = (unsigned int)(*RHSBIS)["DRF"];
    unsigned int cur_ctf = (unsigned int)(*RHSBIS)["CTF"];
    //Checking writing to DRF bit
    if ((unsigned int)(*RHSBGC)["UDE"] == 0x1) {
        (*RHSBIS)["DRF"] = (str.pre_data >> 24) & 0x1;
        mRHSB_ptr->re_printf("warning","Cannot write value 0 to DRF bit while upstream DMA is enable\n");
    }
    if ((unsigned int)(*RHSBGC)["DCDE"] == 0x1) {
        (*RHSBIS)["CTF"] = (str.pre_data >> 18) & 0x1;
        mRHSB_ptr->re_printf("warning","Cannot write value 0 to CTF bit while downstream command DMA is enable\n");
    }
    if ((unsigned int)(*RHSBGC)["DDE"] == 0x1) {
        (*RHSBIS)["DTSF"] = (str.pre_data >> 16) & 0x1;
        mRHSB_ptr->re_printf("warning","Cannot write value 0 to DTSF bit while downstream data DMA is enable\n");
    }
    //Checking writing to CTF bit
    mRHSB_ptr->ControlInterruptPorts(false, emINTDTSF);
    mRHSB_ptr->ControlInterruptPorts(false, emINTDTF);
    mRHSB_ptr->ControlInterruptPorts(false, emINTCTF);
    mRHSB_ptr->ControlInterruptPorts(false, emINTTSF);
    mRHSB_ptr->ControlInterruptPorts(false, emINTL2);
    mRHSB_ptr->ControlInterruptPorts(false, emINTL3);
    mRHSB_ptr->ControlInterruptPorts(false, emINTUEF);
    mRHSB_ptr->ControlInterruptPorts(false, emINTTOF);
    mRHSB_ptr->ControlInterruptPorts(false, emINTDLF);
    if ((((str.pre_data>>16)&0x1) == 1) && (((str.data>>16)&0x1) == 0) && IsTriggerDownstreamMode()) {//DTSF change from 1 to 0 in trigger mode
        mRHSB_ptr->NotifyDownstreamTransmission();
    }
    if ((((str.pre_data >> 8) & 0x1) == 1) && ((unsigned int)(*RHSBIS)["ERF"] == 0) && (!IsEmergencyLevelSensitive())){
        if(mRHSB_ptr->mIsDataTransferring){
            mRHSB_ptr->re_printf("warning","ERF bit is changed during an emergency frame transmission with edge sensitive input mode\n");
        }
        mRHSB_ptr->mIsEmergencyEnabled = false;
        mRHSB_ptr->mIsEmergencyAutoStop = false;
    }
}//}}}

///Define call-back function of RHSBDCR1 register in rhsb_regif class
void RHSB_Func::cb_RHSBDCR1_SLS1(RegCBstr str)
{//{{{
    if ((unsigned int)(*RHSBGC)["OPS"] != emCONFIG) {
        mRHSB_ptr->re_printf("warning","Cannot write a value to RHSBDCR1 register while operation state is different from CONFIG state\n");
        *RHSBDCR1 = str.pre_data;
    }
}//}}}
// vim600: set foldmethod=marker :
