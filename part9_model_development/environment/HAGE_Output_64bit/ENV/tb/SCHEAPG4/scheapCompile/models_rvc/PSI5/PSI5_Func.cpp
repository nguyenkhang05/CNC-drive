// ---------------------------------------------------------------------
// $Id: PSI5_Func.cpp,v 1.10 2020/04/23 07:12:11 hadinhtran Exp $
//
// Copyright(c) 2018 - 2020 Renesas Electronics Corporation
// Copyright(c) 2018 - 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include <cstdarg>
#include "PSI5_Func.h"
#include "global.h"

///*********************************
/// Function     : PSI5_Func 
/// Description  : Constructor of PSI5_Func class
///********************************* 
PSI5_Func::PSI5_Func (std::string name, 
                      PSI5_AgentController *PSI5_AgentPtr):Cpsi5_regif(name, 32), BusSlaveFuncIf()
{//{{{
    assert(PSI5_AgentPtr != NULL);
    mModuleName = name;
    mPSI5_Agent = PSI5_AgentPtr;
    mPSI5_Agent->CommandInit(name);
    Cpsi5_regif::set_instance_name(name);
    mIsSoftwareResetEnable = false;
    mIsChannelEnable = false;
    Initialize();
}//}}}

///*********************************
/// Function     : ~PSI5_Func 
/// Description  : Destructor of PSI5_Func class
///*********************************
PSI5_Func::~PSI5_Func (void)
{//{{{
}//}}}

///*********************************
/// Function     : SetChannelNumber 
/// Description  : Set channel number
///*********************************
void PSI5_Func::SetChannelNumber (const unsigned short channel_num)
{//{{{
    (*CHSTATUS)["CHANNELNUM"] = (unsigned int)channel_num;
    CHSTATUS->UpdateBitInitValue("CHANNELNUM",(unsigned int)channel_num);
}//}}}

///*********************************
/// Function     : RegisterHandler 
/// Description  : Handle commands of register IF class
///*********************************
void PSI5_Func::RegisterHandler (const std::vector<std::string>& args)
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
                msg_return = Cpsi5_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else if (args[1] == "DumpRegisterRW") {
                msg_return = Cpsi5_regif::reg_handle_command(args);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
        } else {
            msg_return = Cpsi5_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///*********************************
/// Function     : EnableReset 
/// Description  : Process reset function
///*********************************
void PSI5_Func::EnableReset (const bool is_active, const std::string clock_domain)
{//{{{
    if (clock_domain == "PCLK_domain") {
        unsigned int tmp = (unsigned int)*CHCTRL;
        Cpsi5_regif::EnableReset(is_active);
        if (mIsSoftwareResetEnable) {
            Cpsi5_regif::EnableReset(!is_active);
            *CHCTRL = tmp;
        }
    }
    if (is_active) {
        Initialize();
        if (!mIsSoftwareResetEnable) {
            mIsChannelEnable = false;
        }
    }
}//}}}

///*********************************
/// Function     : ProcessSyncPulseStart 
/// Description  : Assign TXST.syncst and assert int_syncst
///*********************************
void PSI5_Func::ProcessSyncPulseStart (void)
{//{{{
    (*TXST)["SYNCST"] = 0x1;
    if ((*TXSTINTEN)["SYNCSTINTEN"] == 0x1) {
        (*PSI5INT)["INT_SYNCST"] = 0x1;
        mPSI5_Agent->ControlInterruptPorts(emINT_SYNCST,true);
    }
}//}}}

///*********************************
/// Function     : ProcessSyncPulseEnd 
/// Description  : Assign TXST.synced and assert int_synced
///*********************************
void PSI5_Func::ProcessSyncPulseEnd (void)
{//{{{
    (*TXST)["SYNCED"] = 0x1;
    if ((*TXSTINTEN)["SYNCEDINTEN"] == 0x1) {
        (*PSI5INT)["INT_SYNCED"] = 0x1;
        mPSI5_Agent->ControlInterruptPorts(emINT_SYNCED,true);
    }
}//}}}

///*********************************
/// Function     : CalculateSyncPulseWidth 
/// Description  : Calculate synchronous pulse width according to TXSETTING register
///*********************************
double PSI5_Func::CalculateSyncPulseWidth (const bool is_one_bit)
{//{{{
    double sync_pulse_width = 0;
    unsigned int expiration_value = 0x0;
    if ((*TXSETTING)["PHYMODE"] == 0) {//Tooth Gap method
        expiration_value = (*TXSETTING)["SHORTCNT"];
    } else {//Pulse Width method
        if (is_one_bit) {
            expiration_value = (*TXSETTING)["LONGCNT"];
        } else {
            expiration_value = (*TXSETTING)["SHORTCNT"];
        }
    }
    sync_pulse_width = (double)((expiration_value + 1)*(mPSI5_Agent->GetCommunicationPeriod()));
    return sync_pulse_width;
}//}}}

///*********************************
/// Function     : CalculateReceptionBitsTotalTime 
/// Description  : Calculate total time of reception bits 
///*********************************
double PSI5_Func::CalculateReceptionBitsTotalTime (const unsigned int slot_index)
{//{{{
    double total_time = 0;
    unsigned int bitrate_cnt = (*OPMBITRATE)["BITRATECNT"];
    unsigned int reception_bits_total = 0;
    if ((*RXSSET[slot_index+1])["ERRDET"] == 0) {//Parity bit
        reception_bits_total = (*RXSSET[slot_index+1])["LENGTH"] + 1;
    } else {//CRC bit
        reception_bits_total = (*RXSSET[slot_index+1])["LENGTH"] + 3;
    }
    total_time = reception_bits_total*(bitrate_cnt + 1)*(mPSI5_Agent->GetCommunicationPeriod());
    return total_time;
}//}}}

///*********************************
/// Function     : CheckSlotEnable 
/// Description  : Check slot enable 
///*********************************
bool PSI5_Func::CheckSlotEnable (const unsigned int slot_index)
{//{{{
    return (bool)((*RXSSET[slot_index+1])["SLTEN"]);
}//}}}

///*********************************
/// Function     : CheckChannelEnable 
/// Description  : Check channel enable 
///*********************************
bool PSI5_Func::CheckChannelEnable (void)
{//{{{
    return mIsChannelEnable;
}//}}}

///*********************************
/// Function     : StartSlotTimer 
/// Description  : start timer for each slot
///*********************************
void PSI5_Func::StartSlotTimer (const unsigned int slot_index)
{//{{{
    mPSI5_Agent->SetTimerCounterIntialValue(slot_index,0x0);
    mPSI5_Agent->SetTimerCompareMatchValue(slot_index,(*RXSSET[slot_index+1])["OFFSETCNT"]+1);
    mPSI5_Agent->SetTimerCounterPeriod(slot_index,(double)(mPSI5_Agent->GetCommunicationPeriod()));
    mPSI5_Agent->SetTimerCounterMode(slot_index,false);//One-shot mode
    mPSI5_Agent->StartStopTimer(slot_index,true);
}//}}}

///*********************************
/// Function     : ProcessReceptionData 
/// Description  : Process reception data
///*********************************
void PSI5_Func::ProcessReceptionData (const unsigned int i_control_value
                                     ,const unsigned int i_data_value
                                     ,const unsigned int slot_index)
{//{{{
    unsigned int i_data_value_temp = i_data_value;
    unsigned int crc_length = GetCRCLength(slot_index);
    unsigned int data_length_without_crc = (*RXSSET[slot_index+1])["LENGTH"];
    unsigned int data_length = data_length_without_crc + crc_length;
    if ((*RXSSET[slot_index+1])["PASCMP"] == 0x1) {//PAS compatibility mode (MSB-first to LSB first)
        i_data_value_temp = 0x0;
        for (unsigned int bit_index = 0; bit_index < data_length; bit_index++) {//Rotate bits in I_DATA port
            i_data_value_temp = i_data_value_temp|(((i_data_value>>bit_index)&0x1)<<(data_length-bit_index-1));
        }
    }
    unsigned int received_data = i_data_value_temp&((0x1<<data_length_without_crc)-1);

    //Check data length
    unsigned int received_data_len = i_control_value&0x3F;
    bool is_data_len_err = false;
    if (received_data_len != data_length) {
        is_data_len_err = true;
        mPSI5_Agent->print_msg("warning","The model receives the data with wrong data length\n");
    }

    //Check start bit error
    bool is_start_bit_err = false;
    unsigned int start_bit_value = (i_control_value>>6)&0x3;
    if (start_bit_value != 0x1) {
        is_start_bit_err = true;
    }

    //Check CRC error
    unsigned int received_crc = (i_data_value_temp>>data_length_without_crc)&((0x1<<crc_length)-1); 
    if (crc_length == 0x3) {//Rotate 3 CRC bits
        received_crc = ((received_crc&0x4)>>2) | (received_crc&0x2) | ((received_crc&0x1)<<2);
    }
    unsigned int crc_code = CalculateCRC(received_data, data_length_without_crc, crc_length);
    bool is_crc_err = false;
    if (crc_code != received_crc) {
        is_crc_err = true;
    }

    UpdateReceptionDataStatus(slot_index, received_data, received_crc, is_start_bit_err, is_data_len_err, is_crc_err);
    
    //Process serial message
    bool is_msg_channel_enable = false;
    unsigned int rxmset_reg = (unsigned int)(*RXMSET);
    is_msg_channel_enable = (bool)((rxmset_reg >> slot_index)&0x1);
    if (is_msg_channel_enable) {
        ProcessSerialMessage(slot_index, received_data, is_crc_err);
    }
}//}}}

///*********************************
/// Function     : CalculateCRC 
/// Description  : Calculate CRC code
///*********************************
unsigned int PSI5_Func::CalculateCRC (const unsigned int data
                                      ,const unsigned int data_length
                                      ,const unsigned int crc_length)
{//{{{
    unsigned int crc_code = 0x0;
    if (crc_length == 0x1) {//1 CRC bit (Even parity bit
        unsigned int crc_0 = 0x0;
        for (unsigned int index = 0; index < data_length + crc_length; index++) {
            crc_0 = ((data>>index)&0x1)^crc_0; 
        }
        crc_code = crc_0;
    } else if (crc_length == 0x3) {//3 CRC bits
        unsigned int crc_0 = 0x1;
        unsigned int crc_1 = 0x1;
        unsigned int crc_2 = 0x1;
        for (unsigned int index = 0; index < data_length + crc_length; index++) {
            unsigned int crc_0_temp = ((data>>index)&0x1)^crc_2;
            unsigned int crc_1_temp = crc_0^crc_2;
            unsigned int crc_2_temp = crc_1;
            crc_0 = crc_0_temp;
            crc_1 = crc_1_temp;
            crc_2 = crc_2_temp;
        }
        crc_code = (crc_2<<2)|(crc_1<<1)|crc_0;
    } else {//6 CRC bits
        unsigned int crc_0 = 0x1;
        unsigned int crc_1 = 0x0;
        unsigned int crc_2 = 0x1;
        unsigned int crc_3 = 0x0;
        unsigned int crc_4 = 0x1;
        unsigned int crc_5 = 0x0;
        for (unsigned int index = 0; index < data_length + crc_length; index++) {
            unsigned int crc_0_temp = ((data>>index)&0x1)^crc_5;
            unsigned int crc_1_temp = crc_0;
            unsigned int crc_2_temp = crc_1;
            unsigned int crc_3_temp = crc_2^crc_5;
            unsigned int crc_4_temp = crc_3^crc_5;
            unsigned int crc_5_temp = crc_4;
            crc_0 = crc_0_temp;
            crc_1 = crc_1_temp;
            crc_2 = crc_2_temp;
            crc_3 = crc_3_temp;
            crc_4 = crc_4_temp;
            crc_5 = crc_5_temp;
        }
        crc_code = (crc_5<<5)|(crc_4<<4)|(crc_3<<3)|(crc_2<<2)|(crc_1<<1)|crc_0;
    }
    return crc_code;
}//}}}

///*********************************
/// Function     : SendSyncPulseCollision 
/// Description  : Process sync pulse collision
///*********************************
void PSI5_Func::SendSyncPulseCollision (void)
{//{{{
    (*RXMODST)["RXDSCNFERR"] = 0x1;
    if ((*RXMODSTINTEN)["RXDSCNFERRINTEN"] == 0x1) {
        (*PSI5INT)["INT_RXDSCNFERR"] = 0x1;
        mPSI5_Agent->ControlInterruptPorts(emINT_RXDSCNFERR, true);
    }
}//}}}

///*********************************
/// Function     : UpdateCompletedTransmissionStatus 
/// Description  : Update status bits when transmission is completed
///*********************************
void PSI5_Func::UpdateCompletedTransmissionStatus (void)
{//{{{
    mIsDataTransferring = false;
    (*TXST)["TXDEMPTY"] = 0x1;
    if ((*TXSTINTEN)["TXDEMPTYINTEN"] == 0x1) {
        (*PSI5INT)["INT_TXDEMPTY"] = 0x1;
        mPSI5_Agent->ControlInterruptPorts(emINT_TXDEMPTY, true);//Activate int_txdempty
    }
}//}}}

///*********************************
/// Function     : GetSyncPulseMode 
/// Description  : Return synchronous pulse mode according to TXSETTING register
///*********************************
bool PSI5_Func::GetSyncPulseMode (void)
{//{{{
    return (bool)((*TXSETTING)["PHYMODE"]);
}//}}}

///*********************************
/// Function     : GetSyncPulseDefault 
/// Description  : Return defaule value of sync pulse according to TXSETTING register
///*********************************
bool PSI5_Func::GetSyncPulseDefault (void)
{//{{{
    return (bool)((*TXSETTING)["DEFTXVAL"]);
}//}}}

///*********************************
/// Function     : GetCommunicationMode 
/// Description  : Return communication mode according to OPMCOMM register
///*********************************
unsigned int PSI5_Func::GetCommunicationMode (void)
{//{{{ 
    return ((unsigned int)((*OPMCOMM)["COMMODE"]));
}//}}}

///*********************************
/// Function     : UpdateIPTIMERValue 
/// Description  : Update the value of IPTIMER register
///*********************************
void PSI5_Func::UpdateIPTIMERValue (const unsigned int value)
{//{{{ 
    *IPTIMER = value;
}//}}}

///*********************************
/// Function     : GetIPTIMERValue 
/// Description  : Return the value of IPTIMER register
///*********************************
unsigned int PSI5_Func::GetIPTIMERValue (void)
{//{{{
    return ((unsigned int)(*IPTIMER));
}//}}}

///*********************************
/// Function     : CheckTimerEnable 
/// Description  : Return timer status according to IPTIMERCTRL register
///*********************************
bool PSI5_Func::CheckTimerEnable (void)
{//{{{ 
    return (bool)((*IPTIMERCTRL)["IPTIMEN"]);
}//}}}

///*********************************
/// Function     : IsMasterModeTimer 
/// Description  : Return timer mode
///*********************************
bool PSI5_Func::IsMasterModeTimer (void)
{//{{{
    return (bool)((*IPTIMERCTRL)["MSTSLV"]);
}//}}}

///*********************************
/// Function     : read 
/// Description  : Virtual function of BusSlaveFuncIf class
///*********************************
void PSI5_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    // Clear data buffer
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    
    // Zero clock or reset active
    if ((false == mPSI5_Agent->CheckClockPeriod("PCLK"))
            || (true == mPSI5_Agent->GetPresetnStatus())) {
        // Software debugger
        if (pExtInfo != NULL) {
            if (false == ((true == debug) && (DEBUG_MASTER_BMID == pExtInfo->getPeId()))) {
                re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
                return;
            }
        } else {
            re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
            return;
        }
    }
    if((offsetAddress&0x1FF) ==0x014)
    {
        *IPTIMER = mPSI5_Agent->GetStampTimeCounterValue();//The counter value
    }
    UpdateMirrorRegister(offsetAddress&0x1FF);

    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///*********************************
/// Function     : write 
/// Description  : Virtual function of BusSlaveFuncIf class
///*********************************
void PSI5_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    // Zero clock
    if (false == mPSI5_Agent->CheckClockPeriod("PCLK")) {
        // Software debugger
        if (pExtInfo != NULL) {
            if (false == ((true == debug) && (DEBUG_MASTER_BMID == pExtInfo->getPeId()))) {
                re_printf("warning", "Cannot access register when clock is 0.\n");
                return;
            }
        } else {
            re_printf("warning", "Cannot access register when clock is 0.\n");
            return;
        }
    }
    unsigned int addr = (unsigned int)trans.get_address() & 0x1FF;
    if ((bool)(mIsSoftwareResetEnable & (bool)(addr != 0x0))==true) {
        re_printf("warning", "Cannot write during reset period\n");
        return;
    }
    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///*********************************
/// Function     : Initialize 
/// Description  : Initialize variables used for error capturing/reporting
///*********************************
void PSI5_Func::Initialize (void)
{//{{{
    for (unsigned char index = 0; index < emFifoRegisterNum; index++) {
        while (mRXDFIFO[index].size()>0) {
            mRXDFIFO[index].pop();
        }
        while (mRXMFIFO[index].size()>0) {
            mRXMFIFO[index].pop();
        }
    }
    mIsDataTransferring = false;
    mIsRXDATAEmpty = true;
    mIsRXDSTEmpty = true;
    mIsRXDTIMEmpty = true;
    mIsRXMRXMSGEmpty = true;
    mIsRXMRXSTEmpty = true;
    mIsRXMRXTIMEmpty = true;
    mIsRXDFIFOOverflow = false;
    mIsDataUpdated = false;
    mRXDATA = 0x0;
    mRXDST = 0x0;
    mRXDTIM = 0x0;
    mIsMsgUpdated = false;
    mRXMRXMSG = 0x0;
    mRXMRXST = 0x0;
    mRXMRXTIM = 0x0;
    mIsRXMFIFOOverflow = false;
    mRxdFifoReadTimes = 0x0;
    mEmuRxdFifoReadTimes = 0x0;
    mRxmFifoReadTimes = 0x0;
    mEmuRxmFifoReadTimes = 0x0;
    for (unsigned char slot_num_index = 0; slot_num_index < emSlotNum; slot_num_index++) {
        mReceivedDataFrameNum[slot_num_index] = 0x0;
        for (unsigned char frame_index = 0; frame_index < emMessageFrameNum; frame_index++) {
            mM0Value[slot_num_index][frame_index] = 0x0;
            mM1Value[slot_num_index][frame_index] = 0x0;
        }
    }
}//}}}

///*********************************
/// Function     : GetCRCLength 
/// Description  : Get CRC length (3 CRC bits or 1 Parity bit)
///*********************************
unsigned int PSI5_Func::GetCRCLength (const unsigned int slot_index)
{//{{{
    unsigned int crc_length = 0x0;
    if ((*RXSSET[slot_index+1])["ERRDET"] == 0x1) {
        crc_length = 3;
    } else {
        crc_length = 1;
    }
    return crc_length;
}//}}}

///*********************************
/// Function     : UpdateReceptionDataStatus 
/// Description  : Get Data length
///*********************************
void PSI5_Func::UpdateReceptionDataStatus (const unsigned int slot_index
                                          ,const unsigned int received_data
                                          ,const unsigned int received_crc
                                          ,const bool is_start_bit_err
                                          ,const bool is_data_len_err
                                          ,const bool is_crc_err)
{//{{{
    //Retain the reception data in RXDFIFO
    unsigned int rxd_status = (received_crc << 8) | ((slot_index+1) << 4) | ((unsigned int)is_crc_err);
    unsigned int stamp_time = 0x0;
//    unsigned int cnt_value[2] = {0,0};
    stamp_time = mPSI5_Agent->GetStampTimeCounterValue();//The counter value
    UpdateIPTIMERValue(stamp_time);
    mRXDFIFO[emDATA].push(received_data);
    mRXDFIFO[emStatus].push(rxd_status);
    mRXDFIFO[emTime].push(stamp_time);
    if ((mRXDFIFO[emDATA].size() > emFifoDepth) && (mRXDFIFO[emStatus].size() > emFifoDepth)
        && (mRXDFIFO[emTime].size() > emFifoDepth)) {//If RXDFIFO is full
        mIsRXDFIFOOverflow = true;
        mRXDFIFO[emDATA].pop();//Remove oldest RXDATA
        mRXDFIFO[emStatus].pop();//Remove oldest RXDST
        mRXDFIFO[emTime].pop();//Remove oldest RXDTIM
        (*RXMODST)["RXDFOVF"] = 0x1;
        if ((*RXMODSTINTEN)["RXDFOVFINTEN"] == 0x1) {
            (*PSI5INT)["INT_RXDFOVF"] = 0x1;
            mPSI5_Agent->ControlInterruptPorts(emINT_RXDFOVF, true);
        }
    }
    if (mIsRXDATAEmpty && mIsRXDSTEmpty && mIsRXDTIMEmpty) {
        mRXDATA = mRXDFIFO[emDATA].front();
        mRXDST = mRXDFIFO[emStatus].front();
        mRXDTIM = mRXDFIFO[emTime].front();
        mIsDataUpdated = true;
    }
    mIsRXDATAEmpty = false;
    mIsRXDSTEmpty = false;
    mIsRXDTIMEmpty = false;
    mRxdFifoReadTimes = 0x0;
    mEmuRxdFifoReadTimes = 0x0;
        
    //Reception data error (start bit error, data length error, crc error)
    if (is_start_bit_err || is_data_len_err || is_crc_err) {
        (*RXMODST)["RXDERR"] = 0x1;
        if ((*RXMODSTINTEN)["RXDERRINTEN"] == 0x1) {
            (*PSI5INT)["INT_RXDERR"] = 0x1;
            mPSI5_Agent->ControlInterruptPorts(emINT_RXDERR, true);
        }
    }
    //Reception data exist
    (*RXMODST)["RXDEXIST"] = 0x1;
    if ((*RXMODSTINTEN)["RXDEXISTINTEN"] == 0x1) {
        (*PSI5INT)["INT_RXDEXIST"] = 0x1;
        mPSI5_Agent->ControlInterruptPorts(emINT_RXDEXIST, true);
    }
}//}}}

///*********************************
/// Function     : ProcessSerialMessage 
/// Description  : Process serial message
///*********************************
void PSI5_Func::ProcessSerialMessage (const unsigned int slot_index
                                     ,const unsigned int received_data
                                     ,const bool is_frame_crc_err)
{//{{{
    unsigned int frame_no = 0x0;
    mReceivedDataFrameNum[slot_index] = mReceivedDataFrameNum[slot_index] + 1;
    frame_no = mReceivedDataFrameNum[slot_index];
    mM0Value[slot_index][frame_no-1] = received_data&0x1;
    mM1Value[slot_index][frame_no-1] = (received_data>>0x1)&0x1;
    //Check Frame CRC error and sync bit error after start bit in the message
    if (frame_no > 6) {
        if (is_frame_crc_err || ((frame_no == 7) && (mM1Value[slot_index][frame_no-1] != 0x0))
        || ((frame_no == 13) && (mM1Value[slot_index][frame_no-1] != 0x0))
        || ((frame_no == 18) && (mM1Value[slot_index][frame_no-1] != 0x0))) {
            (*RXMRXST)["RXSTATUS"] = 0x1;
            unsigned int message_content = 0x0;
            for (unsigned int frame_index = 6; frame_index < frame_no; frame_index++) {
                message_content = message_content 
                                | (mM0Value[slot_index][frame_index]<<(2*(frame_index-6)))
                                | (mM1Value[slot_index][frame_index]<<(2*(frame_index-6) + 1));
                *RXMRXMSG = message_content;
            } 
        }
    }
    if (frame_no == 18) {//Store serial message
        mReceivedDataFrameNum[slot_index] = 0x0;//Reset the number of received data frame
        StoreSerialMessage(slot_index);
    }
}//}}}

///*********************************
/// Function     : StoreSerialMessage 
/// Description  : Store serial message
///*********************************
void PSI5_Func::StoreSerialMessage (const unsigned int slot_index)
{//{{{
    mRXMFIFO[emDATA].push(CollectMessageContent(slot_index));
    mRXMFIFO[emStatus].push(CollectMessageStatus(slot_index));
    unsigned int stamp_time = 0x0;
//    unsigned int cnt_value[2] = {0,0};
    stamp_time = mPSI5_Agent->GetStampTimeCounterValue();//The counter value
    UpdateIPTIMERValue(stamp_time);
    mRXMFIFO[emTime].push(stamp_time);
    if ((mRXMFIFO[emDATA].size() > emFifoDepth) && (mRXMFIFO[emStatus].size() > emFifoDepth)
     && (mRXMFIFO[emTime].size() > emFifoDepth)) {//If RXMFIFO is full
        mIsRXMFIFOOverflow = true;
        mRXMFIFO[emDATA].pop();//Remove oldest RXMRXMSG
        mRXMFIFO[emStatus].pop();//Remove oldest RXMRXST
        mRXMFIFO[emTime].pop();//Remove oldest RXMRXTIM
        (*RXMMST)["RXMFOVF"] = 0x1;
        if ((*RXMMSTINTEN)["RXMFOVFINTEN"] == 0x1) {
            (*PSI5INT)["INT_RXMFOVF"] = 0x1;
            mPSI5_Agent->ControlInterruptPorts(emINT_RXMFOVF, true);
        }
    }

    if (mIsRXMRXMSGEmpty && mIsRXMRXSTEmpty && mIsRXMRXTIMEmpty) {
        mRXMRXMSG = mRXMFIFO[emDATA].front();
        mRXMRXST = mRXMFIFO[emStatus].front();
        mRXMRXTIM = mRXMFIFO[emTime].front();
        mIsMsgUpdated = true;
    }
    mIsRXMRXMSGEmpty = false;
    mIsRXMRXSTEmpty = false;
    mIsRXMRXTIMEmpty = false;
    mRxmFifoReadTimes = 0x0;
    mEmuRxmFifoReadTimes = 0x0;

    (*RXMMST)["RXMEXIST"] = 0x1;
    if ((*RXMMSTINTEN)["RXMEXISTINTEN"] == 0x1) {
        (*PSI5INT)["INT_RXMEXIST"] = 0x1;
        mPSI5_Agent->ControlInterruptPorts(emINT_RXMEXIST, true);
    }
}//}}}

///*********************************
/// Function     : CollectMessageContent 
/// Description  : Update message content of RXMRXMSG register
///*********************************
unsigned int PSI5_Func::CollectMessageContent (const unsigned int slot_index)
{//{{{
    unsigned int configbit_value = mM1Value[slot_index][7];
    unsigned int serialid_value = 0x0;
    unsigned int datafield_value = 0x0;
    if (configbit_value == 0x0) {
        serialid_value = mM1Value[slot_index][16]
                  |(mM1Value[slot_index][15]<<1)
                  |(mM1Value[slot_index][14]<<2)
                  |(mM1Value[slot_index][13]<<3)
                  |(mM1Value[slot_index][11]<<4)
                  |(mM1Value[slot_index][10]<<5)
                  |(mM1Value[slot_index][9]<<6)
                  |(mM1Value[slot_index][8]<<7);
        datafield_value = mM0Value[slot_index][17]
                   |(mM0Value[slot_index][16]<<1)
                   |(mM0Value[slot_index][15]<<2)
                   |(mM0Value[slot_index][14]<<3)
                   |(mM0Value[slot_index][13]<<4)
                   |(mM0Value[slot_index][12]<<5)
                   |(mM0Value[slot_index][11]<<6)
                   |(mM0Value[slot_index][10]<<7)
                   |(mM0Value[slot_index][9]<<8)
                   |(mM0Value[slot_index][8]<<9)
                   |(mM0Value[slot_index][7]<<10)
                   |(mM0Value[slot_index][6]<<11);
    } else {
        serialid_value = mM1Value[slot_index][11]
                  |(mM1Value[slot_index][10]<<1)
                  |(mM1Value[slot_index][9]<<2)
                  |(mM1Value[slot_index][8]<<3);
        datafield_value = mM0Value[slot_index][17]
                   |(mM0Value[slot_index][16]<<1)
                   |(mM0Value[slot_index][15]<<2)
                   |(mM0Value[slot_index][14]<<3)
                   |(mM0Value[slot_index][13]<<4)
                   |(mM0Value[slot_index][12]<<5)
                   |(mM0Value[slot_index][11]<<6)
                   |(mM0Value[slot_index][10]<<7)
                   |(mM0Value[slot_index][9]<<8)
                   |(mM0Value[slot_index][8]<<9)
                   |(mM0Value[slot_index][7]<<10)
                   |(mM0Value[slot_index][6]<<11)
                   |(mM1Value[slot_index][16]<<12)
                   |(mM1Value[slot_index][15]<<13)
                   |(mM1Value[slot_index][14]<<14)
                   |(mM1Value[slot_index][13]<<15);
    }
    unsigned int rxmrxmsg_value = datafield_value|(serialid_value<<16)|(configbit_value<<31);
    return rxmrxmsg_value;
}//}}}

///*********************************
/// Function     : CollectMessageStatus 
/// Description  : Update message status of RXMRXST register
///*********************************
unsigned int PSI5_Func::CollectMessageStatus (const unsigned int slot_index)
{//{{{
    unsigned int rxsync_value = (mM1Value[slot_index][17]<<2)
                               |(mM1Value[slot_index][12]<<1)
                               | mM1Value[slot_index][6];
    unsigned int rxcrc_value = mM0Value[slot_index][5]
                             |(mM0Value[slot_index][4]<<1)
                             |(mM0Value[slot_index][3]<<2)
                             |(mM0Value[slot_index][2]<<3)
                             |(mM0Value[slot_index][1]<<4)
                             |(mM0Value[slot_index][0]<<5);
    unsigned int data_check_crc = 0x0;
    for (unsigned int frame_index = 6; frame_index < 18; frame_index++) {
        data_check_crc = data_check_crc
                        |(mM0Value[slot_index][frame_index]<<(2*(frame_index-6)))
                        |(mM1Value[slot_index][frame_index]<<(2*(frame_index-6) + 1));
    }
    unsigned int crc_code = CalculateCRC(data_check_crc, 24, 6);//24 bits data, 6 CRC bits
    unsigned int rxstatus_value = 0x0;
    if ((crc_code != rxcrc_value) || (rxsync_value != 0x0)) {//RX error status
        rxstatus_value = 0x1;
        (*RXMMST)["RXMERR"] = 0x1;
        if ((*RXMMSTINTEN)["RXMERRINTEN"] == 0x1) {
            (*PSI5INT)["INT_RXMERR"] = 0x1;
            mPSI5_Agent->ControlInterruptPorts(emINT_RXMERR, true);
        }
    }
    unsigned int rxmrxst_value = rxstatus_value|((slot_index+1)<<4)|(rxcrc_value<<8)|(rxsync_value<<16);
    return rxmrxst_value;
}//}}}

///*********************************
/// Function     : StartMasterTimer 
/// Description  : Start timer of master mode
///*********************************
void PSI5_Func::StartMasterTimer (void)
{//{{{
    mPSI5_Agent->Writepsi5_ts_tick_outPort(*OPMBITRATE, false);
    mPSI5_Agent->SetTimerCounterIntialValue(emStampTimeCounter,(unsigned int)*IPTIMER);
    double count_period = (double)(mPSI5_Agent->GetCommunicationPeriod()*((*OPMBITRATE) + 1));
    mPSI5_Agent->SetCounter_period_in_master_mode(count_period);
    mPSI5_Agent->SetTimerCounterPeriod(emStampTimeCounter,count_period);
    mPSI5_Agent->SetTimerCounterMode(emStampTimeCounter,true);//Free-run mode
    mPSI5_Agent->StartStopTimer(emStampTimeCounter,true);
}//}}}

///*********************************
/// Function     : cb_CHCTRL_chen
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_CHCTRL_CHEN(RegCBstr str)
{//{{{
    if ((((str.data>>16)&0x1) == 0x1) && (((str.pre_data>>16)&0x1) == 0x0)) {//Software reset is enable
        mIsSoftwareResetEnable = true;
        mPSI5_Agent->EnableSoftwareReset(mIsSoftwareResetEnable);
    } else if ((((str.data>>16)&0x1) == 0x0) && (((str.pre_data>>16)&0x1) == 0x1)) {//Software reset is disable
        mIsSoftwareResetEnable = false;
        mPSI5_Agent->EnableSoftwareReset(mIsSoftwareResetEnable);
    }
    if (!mPSI5_Agent->GetResetStatus() && !mIsSoftwareResetEnable) {
        if ((str.data&0x1) == 0x1) {//Channel is enable
            if (!mIsChannelEnable) {
                if (mPSI5_Agent->CheckClockPeriod("PCLK") && mPSI5_Agent->CheckClockPeriod("psi5_com_clk")) {
                    mPSI5_Agent->print_msg("info","PSI5 channel is enable\n");
                    mIsChannelEnable = true;
                    if (((*IPTIMERCTRL)["IPTIMEN"] == 0x1) && ((*IPTIMERCTRL)["MSTSLV"] == 0x1)) {
                        StartMasterTimer();
                    }
                    if (((*OPMCOMM)["COMMODE"] != 0x1) && ((*OPMCOMM)["COMMODE"] != 0x2)
                     && ((*OPMCOMM)["COMMODE"] != 0x3) && ((*OPMCOMM)["COMMODE"] != 0x4)) {//Async mode
                        mPSI5_Agent->SendSyncPulse(false,(*TXSETTING)["DEFTXVAL"]);
                    } else {//Sync mode
                        mPSI5_Agent->SendSyncPulse(true);//Send sync pulse
                        mPSI5_Agent->SetTimerCounterIntialValue(emSyncPulseTimer,0x0);
                        mPSI5_Agent->SetTimerCompareMatchValue(emSyncPulseTimer,(*OPMCYCT)["TTTTCNT"]+1);
                        mPSI5_Agent->SetTimerCounterPeriod(emSyncPulseTimer,(double)(mPSI5_Agent->GetCommunicationPeriod()));
                        mPSI5_Agent->SetTimerCounterMode(emSyncPulseTimer,true);//Free-run mode
                        mPSI5_Agent->StartStopTimer(emSyncPulseTimer,true);
                        if (((*TXSTINTEN)["TXDEMPTYINTEN"] == 0x1) && ((*TXST)["TXDEMPTY"] == 0x1)) {
                            (*PSI5INT)["INT_TXDEMPTY"] = 0x1;
                            mPSI5_Agent->ControlInterruptPorts(emINT_TXDEMPTY, true);//Activate int_txdempty
                        }
                    }
                }
            } else {
                mPSI5_Agent->print_msg("warning","PSI5 channel has already been enabled\n");
            }
        } else if (((str.data&0x1) == 0x0) && (mIsChannelEnable)) {
            mPSI5_Agent->print_msg("info","PSI5 channel is disable\n");
            mIsChannelEnable = false;
            mPSI5_Agent->SendSyncPulse(false, 0);//Clear sync pulse
            mPSI5_Agent->Writepsi5_ts_tick_outPort(0, true);//Clear psi5_ts_tick_out
            mPSI5_Agent->StartStopTimer(emSyncPulseTimer,false);//Stop Sync Pulse Timer
            mPSI5_Agent->StartStopTimer(emSlot1,false);//Stop Slot 1 Timer
            mPSI5_Agent->StartStopTimer(emSlot2,false);//Stop Slot 2 Timer
            mPSI5_Agent->StartStopTimer(emSlot3,false);//Stop Slot 3 Timer
            mPSI5_Agent->StartStopTimer(emSlot4,false);//Stop Slot 4 Timer
            mPSI5_Agent->StartStopTimer(emSlot5,false);//Stop Slot 5 Timer
            mPSI5_Agent->StartStopTimer(emSlot6,false);//Stop Slot 6 Timer
            mPSI5_Agent->StartStopTimer(emSlot7,false);//Stop Slot 7 Timer
            mPSI5_Agent->StartStopTimer(emSlot8,false);//Stop Slot 8 Timer
            mPSI5_Agent->StartStopTimer(emStampTimeCounter,false);//Stop Stamp Time Counter
        }
    } else {
        mPSI5_Agent->print_msg("warning","PSI5 model can not operate during reset operation");
    }
}//}}}

///*********************************
/// Function     : cb_IPTIMERCTRL_tsclr
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_IPTIMERCTRL_TSCLR(RegCBstr str)
{//{{{
    if (!mPSI5_Agent->GetResetStatus() && !mIsSoftwareResetEnable) {
        if (mPSI5_Agent->CheckClockPeriod("PCLK") && mPSI5_Agent->CheckClockPeriod("psi5_com_clk")) {
            if (((*CHCTRL)["CHEN"] == 0x1) && ((str.data&0x1) == 0x1) && (((str.data>>16)&0x1) == 0x1)) {
                mPSI5_Agent->print_msg("info","The timer is enable\n");
                StartMasterTimer();
                if (((str.data>>8)&0x1) == 0x1) {
                    UpdateIPTIMERValue(0x0);
                    mPSI5_Agent->SetTimerCounterIntialValue(emStampTimeCounter,0x0);
                    mPSI5_Agent->Sendpsi5_ts_clr_outPort(); 
                }
            } else if ((str.data&0x1) == 0x0) {//Timer is disable
                mPSI5_Agent->print_msg("info","The timer is disable\n");
                mPSI5_Agent->StartStopTimer(emStampTimeCounter, false);
                if (((str.data>>16)&0x1) == 0x1) {//Master mode
                    mPSI5_Agent->Writepsi5_ts_tick_outPort(0, true);
                }
            }
        }
    } else {
        mPSI5_Agent->print_msg("warning","PSI5 model can not operate during reset operation");
    }
    *IPTIMERCTRL = *IPTIMERCTRL&0xFFFFFEFF;
}//}}}

///*********************************
/// Function     : cb_IPTIMER_countval
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_IPTIMER_COUNTVAL(RegCBstr str)
{//{{{
    mPSI5_Agent->SetTimerCounterIntialValue(emStampTimeCounter,(unsigned int)*IPTIMER); 
}//}}}

///*********************************
/// Function     : cb_OPMCOMM_commode
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_OPMCOMM_COMMODE(RegCBstr str)
{//{{{
    if ((*CHCTRL)["CHEN"] == 0x1) {
        *OPMCOMM = str.pre_data;
        mPSI5_Agent->print_msg("warning","Can not write the value to OPMCOMM register while the channel is enable");
    }
}//}}}

///*********************************
/// Function     : cb_OPMBITRATE_bitratecnt
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_OPMBITRATE_BITRATECNT(RegCBstr str)
{//{{{
    if ((*CHCTRL)["CHEN"] == 0x1) {
        *OPMBITRATE = str.pre_data;
        mPSI5_Agent->print_msg("warning","Can not write the value to OPMBITRATE register while the channel is enable");
    }
}//}}}

///*********************************
/// Function     : cb_OPMCYCT_ttttcnt
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_OPMCYCT_TTTTCNT(RegCBstr str)
{//{{{
    if ((*CHCTRL)["CHEN"] == 0x1) {
        *OPMCYCT = str.pre_data;
        mPSI5_Agent->print_msg("warning","Can not write the value to OPMCYCT register while the channel is enable");
    }
}//}}}

///*********************************
/// Function     : cb_EMRXDFIFO_rxdst
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_EMRXDFIFO_RXDST(RegCBstr str)
{//{{{
    if (mPSI5_Agent->CheckClockPeriod("PCLK") && mPSI5_Agent->CheckClockPeriod("psi5_com_clk")) {
        mEmuRxdFifoReadTimes = mEmuRxdFifoReadTimes + 1;
        if (mEmuRxdFifoReadTimes == 0x1) {//First times
            *EMRXDFIFO = (unsigned int)(*RXDATA);
        } else if (mEmuRxdFifoReadTimes == 0x2) {//Second times
            *EMRXDFIFO = (unsigned int)(*RXDST);
        } else {//Third times
            mEmuRxdFifoReadTimes = 0x0;
            *EMRXDFIFO = (unsigned int)(*RXDTIM);
        }
    }
}//}}}


///*********************************
/// Function     : cb_EMRXMFIFO_rxmfifo
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_EMRXMFIFO_RXMFIFO(RegCBstr str)
{//{{{
    if (mPSI5_Agent->CheckClockPeriod("PCLK") && mPSI5_Agent->CheckClockPeriod("psi5_com_clk")) {
        mEmuRxmFifoReadTimes = mEmuRxmFifoReadTimes + 1;
        if (mEmuRxmFifoReadTimes == 0x1) {//First times
            *EMRXMFIFO = (unsigned int)(*RXMRXMSG);
        } else if (mEmuRxmFifoReadTimes == 0x2) {//Second times
            *EMRXMFIFO = (unsigned int)(*RXMRXST);
        } else {//Third times
            mEmuRxmFifoReadTimes = 0x0;
            *EMRXMFIFO = (unsigned int)(*RXMRXTIM);
        }
    }
}//}}}

///*********************************
/// Function     : cb_TXSETTING_shortcnt
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_TXSETTING_SHORTCNT(RegCBstr str)
{//{{{
    if ((*CHCTRL)["CHEN"] == 0x1) {
        *TXSETTING = str.pre_data;
        mPSI5_Agent->print_msg("warning","Can not write the value to TXSETTING register while the channel is enable");
    } else {
        if (((*TXSETTING)["PHYMODE"] == 1) && ((*TXSETTING)["SHORTCNT"] == (*TXSETTING)["LONGCNT"])) {
            mPSI5_Agent->print_msg("warning","Should not set 'short' sync pulse width equal to 'long' sync pulse width in Pulse Width method\n");
        }
    }
}//}}}

///*********************************
/// Function     : cb_SYNCCTRL_valtimsync
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_SYNCCTRL_VALTIMSYNC(RegCBstr str)
{//{{{
    if (mPSI5_Agent->CheckClockPeriod("PCLK") && mPSI5_Agent->CheckClockPeriod("psi5_com_clk")) {
        if ((str.data&0x1)==0x1) {
            if ((*OPMCOMM)["COMMODE"]==0x4) {//Variable time triggered sync mode
                mPSI5_Agent->SendSyncPulse(true);
                if (mIsDataTransferring) {
                    mPSI5_Agent->print_msg("warning","Should not issue sync pulse during data transferring\n");
                }
            } else {
                mPSI5_Agent->print_msg("warning","Don't write 1 in any modes which is different from variable time triggered synchronous mode\n");
            }
        }
    }
    *SYNCCTRL = 0x0;
}//}}}

///*********************************
/// Function     : cb_TXSTCLR_syncstclr
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_TXSTCLR_SYNCSTCLR(RegCBstr str)
{//{{{
    if (!mPSI5_Agent->GetResetStatus() && !mIsSoftwareResetEnable) {
        if (mPSI5_Agent->CheckClockPeriod("PCLK") && mPSI5_Agent->CheckClockPeriod("psi5_com_clk")) {
            if (((str.data>>8)&0x1) == 0x1) {
                (*TXST)["SYNCST"] = 0x0;
                (*PSI5INT)["INT_SYNCST"] = 0x0;
                mPSI5_Agent->ControlInterruptPorts(emINT_SYNCST,false);
            }
            if (((str.data>>9)&0x1) == 0x1) {
                (*TXST)["SYNCED"] = 0x0;
                (*PSI5INT)["INT_SYNCED"] = 0x0;
                mPSI5_Agent->ControlInterruptPorts(emINT_SYNCED,false);
            }
        }
    } else {
        mPSI5_Agent->print_msg("warning","PSI5 model can not operate during reset operation");
    }
    *TXSTCLR = 0x0;
}//}}}

///*********************************
/// Function     : cb_TXSTINTEN_txdemptyinten
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_TXSTINTEN_TXDEMPTYINTEN(RegCBstr str)
{//{{{
    if (!mPSI5_Agent->GetResetStatus() && !mIsSoftwareResetEnable) {
        if (mPSI5_Agent->CheckClockPeriod("PCLK") && mPSI5_Agent->CheckClockPeriod("psi5_com_clk")) {
            if (((str.data)&0x1) == 0x0) {
                (*PSI5INT)["INT_TXDEMPTY"] = 0x0;
                mPSI5_Agent->ControlInterruptPorts(emINT_TXDEMPTY, false);
            } else if (((str.data & 0x1) == 0x1) && ((str.pre_data & 0x1) == 0x0)) { // added vupham 01/21 to assert interrupt when bit is enabled
                if (((*TXST)["TXDEMPTY"] == 0x1) && CheckChannelEnable()) {
                    (*PSI5INT)["INT_TXDEMPTY"] = 0x1;
                    mPSI5_Agent->ControlInterruptPorts(emINT_TXDEMPTY, true);//Activate int_txdempty

                }                
            }
            if (((str.data>>8)&0x1) == 0x0) {
                (*PSI5INT)["INT_SYNCST"] = 0x0;
                mPSI5_Agent->ControlInterruptPorts(emINT_SYNCST, false);
            } else if ((((str.data>>8)&0x1) == 0x1) && (((str.pre_data>>8)&0x1) == 0x0)) {  // added vupham 01/21
                if (((*TXST)["SYNCST"] == 0x1) && CheckChannelEnable()) {
                    (*PSI5INT)["INT_SYNCST"] = 0x1;
                    mPSI5_Agent->ControlInterruptPorts(emINT_SYNCST,true);
                } 
            }
            if (((str.data>>9)&0x1) == 0x0) {
                (*PSI5INT)["INT_SYNCED"] = 0x0;
                mPSI5_Agent->ControlInterruptPorts(emINT_SYNCED, false);
            } else if ((((str.data>>9)&0x1) == 0x1) && (((str.pre_data>>9)&0x1) == 0x0)) {  // added vupham 01/21
                if (((*TXST)["SYNCED"] == 0x1) && CheckChannelEnable()) {
                    (*PSI5INT)["INT_SYNCED"] = 0x1;
                    mPSI5_Agent->ControlInterruptPorts(emINT_SYNCED,true);
                }
            }
        }
    } else {
        mPSI5_Agent->print_msg("warning","PSI5 model can not operate during reset operation");
    }
}//}}}

///*********************************
/// Function     : cb_TXDATA_txdata
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_TXDATA_TXDATA(RegCBstr str)
{//{{{
    if (!mPSI5_Agent->GetResetStatus() && !mIsSoftwareResetEnable) {
        if (mPSI5_Agent->CheckClockPeriod("PCLK") && mPSI5_Agent->CheckClockPeriod("psi5_com_clk")) {
            if ((*CHCTRL)["CHEN"] == 0x1) {
                if (((*OPMCOMM)["COMMODE"] == 0x1) || ((*OPMCOMM)["COMMODE"] == 0x2) 
                  ||((*OPMCOMM)["COMMODE"] == 0x3) || ((*OPMCOMM)["COMMODE"] == 0x4)) {//Sync mode
                    if (((*TXDCTRL)["FRMFORMAT"] != 0x4) || ((*TXSETTING)["PHYMODE"] != 0x0)) {
                        (*TXST)["TXDEMPTY"] = 0x0;
                        (*PSI5INT)["INT_TXDEMPTY"] = 0x0;
                        mPSI5_Agent->ControlInterruptPorts(emINT_TXDEMPTY, false);//Deactivate int_txdempty port
                        mPSI5_Agent->print_msg("info","The transmission is begin\n");
                        mPSI5_Agent->SendData((*TXDCTRL)["FRMFORMAT"],(*TXDATA)["TXDATA"]);
                        mIsDataTransferring = true;
                    } else {
                        mPSI5_Agent->print_msg("warning","Can not transmit the data with XXLong frame in Tooth Gap method\n");
                    }
                }
            }
        }
    } else {
        mPSI5_Agent->print_msg("warning","PSI5 model can not operate during reset operation");
    }
    *TXDATA = 0x0;
}//}}}

///*********************************
/// Function     : cb_RXSPLSET_smplprod
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_RXSPLSET_SMPLPROD(RegCBstr str)
{//{{{
    if ((*CHCTRL)["CHEN"] == 0x1) {
        *RXSPLSET = str.pre_data;
        mPSI5_Agent->print_msg("warning","Can not write the value to RXSPLSET register while the channel is enable");
    }
}//}}}

///*********************************
/// Function     : cb_RXWINSET_dwinstpos
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_RXWINSET_DWINSTPOS(RegCBstr str)
{//{{{
    if ((*CHCTRL)["CHEN"] == 0x1) {
        *RXWINSET = str.pre_data;
        mPSI5_Agent->print_msg("warning","Can not write the value to RXWINSET register while the channel is enable");
    }
}//}}}

///*********************************
/// Function     : cb_RXSSET_offsetcnt
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_RXSSET_OFFSETCNT(RegCBstr str)
{//{{{
    if ((*CHCTRL)["CHEN"] == 0x1) {
        *RXSSET[str.channel] = str.pre_data;
        mPSI5_Agent->print_msg("warning","Can not write the value to RXSSET%d register while the channel is enable", str.channel);
    } else {
        if ((((str.data>>25)&0x1) == 0x1) && (((str.data>>26)&0x1) == 0x1)) {
            mPSI5_Agent->print_msg("warning","CRC error detection scheme is set in PAS mode\n");
        }
    }
}//}}}

///*********************************
/// Function     : cb_RXDATA_rxdata
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_RXDATA_RXDATA(RegCBstr str)
{//{{{
    if (mPSI5_Agent->CheckClockPeriod("PCLK") && mPSI5_Agent->CheckClockPeriod("psi5_com_clk")) {
        mIsRXDATAEmpty = true;
        if (mIsRXDATAEmpty&&mIsRXDSTEmpty&&mIsRXDTIMEmpty) {
            if ((!mIsRXDFIFOOverflow) && (mIsDataUpdated)) {
                mRXDFIFO[emDATA].pop();
                mRXDFIFO[emStatus].pop();
                mRXDFIFO[emTime].pop();
            }
            if (mRXDFIFO[emDATA].size() > 0) {
                mIsRXDATAEmpty = false;
                mIsRXDSTEmpty = false;
                mIsRXDTIMEmpty = false;
                mRXDATA = mRXDFIFO[emDATA].front();
                mRXDST = mRXDFIFO[emStatus].front();
                mRXDTIM = mRXDFIFO[emTime].front();
                mRXDFIFO[emDATA].pop();
                mRXDFIFO[emStatus].pop();
                mRXDFIFO[emTime].pop();
            } else {
                mIsRXDATAEmpty = true;
                mIsRXDSTEmpty = true;
                mIsRXDTIMEmpty = true;
                mRXDATA = 0x0;
                mRXDST = 0x0;
                mRXDTIM = 0x0;
            }
            mIsDataUpdated = false;
            mIsRXDFIFOOverflow = false;
            if ((mRXDFIFO[emDATA].size() == 0x0)
             && (mRXDFIFO[emStatus].size() == 0x0)
             && (mRXDFIFO[emTime].size() == 0x0))  {
                (*RXMODST)["RXDEXIST"] = 0x0;
                (*PSI5INT)["INT_RXDEXIST"] = 0x0;
                mPSI5_Agent->ControlInterruptPorts(emINT_RXDEXIST,false);
            }
        }
    }
}//}}}

///*********************************
/// Function     : cb_RXDST_rxstatus
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_RXDST_RXSTATUS(RegCBstr str)
{//{{{
    if (mPSI5_Agent->CheckClockPeriod("PCLK") && mPSI5_Agent->CheckClockPeriod("psi5_com_clk")) {
        mIsRXDSTEmpty = true;
        if (mIsRXDATAEmpty&&mIsRXDSTEmpty&&mIsRXDTIMEmpty) {
            if ((!mIsRXDFIFOOverflow) && (mIsDataUpdated)) {
                mRXDFIFO[emDATA].pop();
                mRXDFIFO[emStatus].pop();
                mRXDFIFO[emTime].pop();
            }
            if (mRXDFIFO[emDATA].size() > 0) {
                mIsRXDATAEmpty = false;
                mIsRXDSTEmpty = false;
                mIsRXDTIMEmpty = false;
                mRXDATA = mRXDFIFO[emDATA].front();
                mRXDST = mRXDFIFO[emStatus].front();
                mRXDTIM = mRXDFIFO[emTime].front();
                mRXDFIFO[emDATA].pop();
                mRXDFIFO[emStatus].pop();
                mRXDFIFO[emTime].pop();
            } else {
                mIsRXDATAEmpty = true;
                mIsRXDSTEmpty = true;
                mIsRXDTIMEmpty = true;
                mRXDATA = 0x0;
                mRXDST = 0x0;
                mRXDTIM = 0x0;
            }
            mIsDataUpdated = false;
            mIsRXDFIFOOverflow = false;
            if ((mRXDFIFO[emDATA].size() == 0x0)
             && (mRXDFIFO[emStatus].size() == 0x0)
             && (mRXDFIFO[emTime].size() == 0x0))  {
                (*RXMODST)["RXDEXIST"] = 0x0;
                (*PSI5INT)["INT_RXDEXIST"] = 0x0;
                mPSI5_Agent->ControlInterruptPorts(emINT_RXDEXIST,false);
            }
        }
    }
}//}}}

///*********************************
/// Function     : cb_RXDTIM_rxdtim
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_RXDTIM_RXDTIM(RegCBstr str)
{//{{{
    if (mPSI5_Agent->CheckClockPeriod("PCLK") && mPSI5_Agent->CheckClockPeriod("psi5_com_clk")) {
        mIsRXDTIMEmpty = true;
        if (mIsRXDATAEmpty&&mIsRXDSTEmpty&&mIsRXDTIMEmpty) {
            if ((!mIsRXDFIFOOverflow) && (mIsDataUpdated)) {
                mRXDFIFO[emDATA].pop();
                mRXDFIFO[emStatus].pop();
                mRXDFIFO[emTime].pop();
            }
            if (mRXDFIFO[emDATA].size() > 0) {
                mIsRXDATAEmpty = false;
                mIsRXDSTEmpty = false;
                mIsRXDTIMEmpty = false;
                mRXDATA = mRXDFIFO[emDATA].front();
                mRXDST = mRXDFIFO[emStatus].front();
                mRXDTIM = mRXDFIFO[emTime].front();
                mRXDFIFO[emDATA].pop();
                mRXDFIFO[emStatus].pop();
                mRXDFIFO[emTime].pop();
            } else {
                mIsRXDATAEmpty = true;
                mIsRXDSTEmpty = true;
                mIsRXDTIMEmpty = true;
                mRXDATA = 0x0;
                mRXDST = 0x0;
                mRXDTIM = 0x0;
            }
            mIsDataUpdated = false;
            mIsRXDFIFOOverflow = false;
            if ((mRXDFIFO[emDATA].size() == 0x0)
             && (mRXDFIFO[emStatus].size() == 0x0)
             && (mRXDFIFO[emTime].size() == 0x0))  {
                (*RXMODST)["RXDEXIST"] = 0x0;
                (*PSI5INT)["INT_RXDEXIST"] = 0x0;
                mPSI5_Agent->ControlInterruptPorts(emINT_RXDEXIST,false);
            }
        }
    }
}//}}}

///*********************************
/// Function     : cb_RXDFIFO_rxdst
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_RXDFIFO_RXDST(RegCBstr str)
{//{{{
    if (mPSI5_Agent->CheckClockPeriod("PCLK") && mPSI5_Agent->CheckClockPeriod("psi5_com_clk")) {
        mRxdFifoReadTimes = mRxdFifoReadTimes + 1;
        if (mRxdFifoReadTimes == 0x1) {//First times
            mIsRXDATAEmpty = true;
            *RXDFIFO = mRXDATA;
        } else if (mRxdFifoReadTimes == 0x2) {//Second times
            mIsRXDSTEmpty = true;
            *RXDFIFO = mRXDST;
        } else {//Third times
            mIsRXDTIMEmpty = true;
            mRxdFifoReadTimes = 0x0;
            *RXDFIFO = mRXDTIM;
        }
        if (mIsRXDATAEmpty&&mIsRXDSTEmpty&&mIsRXDTIMEmpty) {
            if ((!mIsRXDFIFOOverflow) && (mIsDataUpdated)) {
                mRXDFIFO[emDATA].pop();
                mRXDFIFO[emStatus].pop();
                mRXDFIFO[emTime].pop();
            }
            if (mRXDFIFO[emDATA].size() > 0) {
                mIsRXDATAEmpty = false;
                mIsRXDSTEmpty = false;
                mIsRXDTIMEmpty = false;
                mRXDATA = mRXDFIFO[emDATA].front();
                mRXDST = mRXDFIFO[emStatus].front();
                mRXDTIM = mRXDFIFO[emTime].front();
                mRXDFIFO[emDATA].pop();
                mRXDFIFO[emStatus].pop();
                mRXDFIFO[emTime].pop();
            } else {
                mIsRXDATAEmpty = true;
                mIsRXDSTEmpty = true;
                mIsRXDTIMEmpty = true;
                mRXDATA = 0x0;
                mRXDST = 0x0;
                mRXDTIM = 0x0;
            }
            mIsDataUpdated = false;
            mIsRXDFIFOOverflow = false;
            if ((mRXDFIFO[emDATA].size() == 0x0)
             && (mRXDFIFO[emStatus].size() == 0x0)
             && (mRXDFIFO[emTime].size() == 0x0))  {
                (*RXMODST)["RXDEXIST"] = 0x0;
                (*PSI5INT)["INT_RXDEXIST"] = 0x0;
                mPSI5_Agent->ControlInterruptPorts(emINT_RXDEXIST,false);
            }
        }
    }
}//}}}

///*********************************
/// Function     : cb_RXMODSTCLR_rxdfovfclr
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_RXMODSTCLR_RXDFOVFCLR(RegCBstr str)
{//{{{
    if (mPSI5_Agent->CheckClockPeriod("PCLK") && mPSI5_Agent->CheckClockPeriod("psi5_com_clk")) {
        if (((str.data>>8)&0x1) == 0x1) {
            (*RXMODST)["RXDFOVF"] = 0x0;
            (*PSI5INT)["INT_RXDFOVF"] = 0x0;
            mPSI5_Agent->ControlInterruptPorts(emINT_RXDFOVF,false);
        }
        if (((str.data>>16)&0x1) == 0x1) {
            (*RXMODST)["RXDERR"] = 0x0;
            (*PSI5INT)["INT_RXDERR"] = 0x0;
            mPSI5_Agent->ControlInterruptPorts(emINT_RXDERR,false);
        }
        if (((str.data>>24)&0x1) == 0x1) {
            (*RXMODST)["RXDSCNFERR"] = 0x0;
            (*PSI5INT)["INT_RXDSCNFERR"] = 0x0;
            mPSI5_Agent->ControlInterruptPorts(emINT_RXDSCNFERR,false);
        }
    }
    *RXMODSTCLR = 0x0;
}//}}}

///*********************************
/// Function     : cb_RXMODSTINTEN_rxdexistinten
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_RXMODSTINTEN_RXDEXISTINTEN(RegCBstr str)
{//{{{
    if (!mPSI5_Agent->GetResetStatus() && !mIsSoftwareResetEnable) {
        if (mPSI5_Agent->CheckClockPeriod("PCLK") && mPSI5_Agent->CheckClockPeriod("psi5_com_clk")) {
            if (((str.data)&0x1) == 0x0) {
                (*PSI5INT)["INT_RXDEXIST"] = 0x0;
                mPSI5_Agent->ControlInterruptPorts(emINT_RXDEXIST, false);
            } else if ((((str.data)&0x1) == 0x1) && (((str.pre_data)&0x1) == 0x0)) {
                if (((*RXMODST)["RXDEXIST"] == 0x1) && CheckChannelEnable()) {
                    (*PSI5INT)["INT_RXDEXIST"] = 0x1;
                    mPSI5_Agent->ControlInterruptPorts(emINT_RXDEXIST, true);
                }         
            }
            if (((str.data>>8)&0x1) == 0x0) {
                (*PSI5INT)["INT_RXDFOVF"] = 0x0;
                mPSI5_Agent->ControlInterruptPorts(emINT_RXDFOVF, false);
            } else if ((((str.data>>8)&0x1) == 0x1) && (((str.pre_data>>8)&0x1) == 0x0)){
                if (((*RXMODST)["RXDFOVF"] == 0x1) && CheckChannelEnable()) {
                    (*PSI5INT)["INT_RXDFOVF"] = 0x1;
                    mPSI5_Agent->ControlInterruptPorts(emINT_RXDFOVF, true);
                }

            }
            if (((str.data>>16)&0x1) == 0x0) {
                (*PSI5INT)["INT_RXDERR"] = 0x0;
                mPSI5_Agent->ControlInterruptPorts(emINT_RXDERR, false);
            } else if ((((str.data>>16)&0x1) == 0x1) && (((str.pre_data>>16)&0x1) == 0x0)){
                if (((*RXMODST)["RXDERR"] == 0x1) && CheckChannelEnable()) {
                    (*PSI5INT)["INT_RXDERR"] = 0x1;
                    mPSI5_Agent->ControlInterruptPorts(emINT_RXDERR, true);
                }

            }
            if (((str.data>>24)&0x1) == 0x0) {
                (*PSI5INT)["INT_RXDSCNFERR"] = 0x0;
                mPSI5_Agent->ControlInterruptPorts(emINT_RXDSCNFERR, false);
            } else if ((((str.data>>24)&0x1) == 0x1) && (((str.pre_data>>24)&0x1) == 0x0)){
                if (((*RXMODST)["RXDSCNFERR"] == 0x1) && CheckChannelEnable()) {
                    (*PSI5INT)["INT_RXDSCNFERR"] = 0x1;
                    mPSI5_Agent->ControlInterruptPorts(emINT_RXDSCNFERR, true);
                }

            }
        }
    } else {
        mPSI5_Agent->print_msg("warning","PSI5 model can not operate during reset operation");
    }
}//}}}

///*********************************
/// Function     : cb_RXMSET_rxm1en
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_RXMSET_RXM1EN(RegCBstr str)
{//{{{
    if ((*CHCTRL)["CHEN"] == 0x1) {
        *RXMSET = str.pre_data;
        mPSI5_Agent->print_msg("warning","Can not write the value to RXMSET register while the channel is enable");
    }
}//}}}

///*********************************
/// Function     : cb_RXMRXMSG_datafield
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_RXMRXMSG_DATAFIELD(RegCBstr str)
{//{{{
    if (mPSI5_Agent->CheckClockPeriod("PCLK") && mPSI5_Agent->CheckClockPeriod("psi5_com_clk")) {
        mIsRXMRXMSGEmpty = true;
        if (mIsRXMRXMSGEmpty && mIsRXMRXSTEmpty && mIsRXMRXTIMEmpty) {
            if ((!mIsRXMFIFOOverflow) && (mIsMsgUpdated)) {
                mRXMFIFO[emDATA].pop();
                mRXMFIFO[emStatus].pop();
                mRXMFIFO[emTime].pop();
            }
            if (mRXMFIFO[emDATA].size() > 0) {
                mIsRXMRXMSGEmpty = false;
                mIsRXMRXSTEmpty = false;
                mIsRXMRXTIMEmpty = false;
                mRXMRXMSG = mRXMFIFO[emDATA].front();
                mRXMRXST = mRXMFIFO[emStatus].front();
                mRXMRXTIM = mRXMFIFO[emTime].front();
                mRXMFIFO[emDATA].pop();
                mRXMFIFO[emStatus].pop();
                mRXMFIFO[emTime].pop();
            } else {
                mIsRXMRXMSGEmpty = true;
                mIsRXMRXSTEmpty = true;
                mIsRXMRXTIMEmpty = true;
                mRXMRXMSG = 0x0;
                mRXMRXST = 0x0;
                mRXMRXTIM = 0x0;
            }
            mIsMsgUpdated = false;
            mIsRXMFIFOOverflow = false;
            if ((mRXMFIFO[emDATA].size() == 0x0)
             && (mRXMFIFO[emStatus].size() == 0x0)
             && (mRXMFIFO[emTime].size() == 0x0))  {
                (*RXMMST)["RXMEXIST"] = 0x0;
                (*PSI5INT)["INT_RXMEXIST"] = 0x0;
                mPSI5_Agent->ControlInterruptPorts(emINT_RXMEXIST,false);
            }
        }
    }
}//}}}

///*********************************
/// Function     : cb_RXMRXST_rxstatus
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_RXMRXST_RXSTATUS(RegCBstr str)
{//{{{
    if (mPSI5_Agent->CheckClockPeriod("PCLK") && mPSI5_Agent->CheckClockPeriod("psi5_com_clk")) {
        mIsRXMRXSTEmpty = true;
        if (mIsRXMRXMSGEmpty && mIsRXMRXSTEmpty && mIsRXMRXTIMEmpty) {
            if ((!mIsRXMFIFOOverflow) && (mIsMsgUpdated)) {
                mRXMFIFO[emDATA].pop();
                mRXMFIFO[emStatus].pop();
                mRXMFIFO[emTime].pop();
            }
            if (mRXMFIFO[emDATA].size() > 0) {
                mIsRXMRXMSGEmpty = false;
                mIsRXMRXSTEmpty = false;
                mIsRXMRXTIMEmpty = false;
                mRXMRXMSG = mRXMFIFO[emDATA].front();
                mRXMRXST = mRXMFIFO[emStatus].front();
                mRXMRXTIM = mRXMFIFO[emTime].front();
                mRXMFIFO[emDATA].pop();
                mRXMFIFO[emStatus].pop();
                mRXMFIFO[emTime].pop();
            } else {
                mIsRXMRXMSGEmpty = true;
                mIsRXMRXSTEmpty = true;
                mIsRXMRXTIMEmpty = true;
                mRXMRXMSG = 0x0;
                mRXMRXST = 0x0;
                mRXMRXTIM = 0x0;
            }
            mIsMsgUpdated = false;
            mIsRXMFIFOOverflow = false;
            if ((mRXMFIFO[emDATA].size() == 0x0)
             && (mRXMFIFO[emStatus].size() == 0x0)
             && (mRXMFIFO[emTime].size() == 0x0))  {
                (*RXMMST)["RXMEXIST"] = 0x0;
                (*PSI5INT)["INT_RXMEXIST"] = 0x0;
                mPSI5_Agent->ControlInterruptPorts(emINT_RXMEXIST,false);
            }
        }
    }
}//}}}

///*********************************
/// Function     : cb_RXMRXTIM_rxmtim
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_RXMRXTIM_RXMTIM(RegCBstr str)
{//{{{
    if (mPSI5_Agent->CheckClockPeriod("PCLK") && mPSI5_Agent->CheckClockPeriod("psi5_com_clk")) {
        mIsRXMRXTIMEmpty = true;
        if (mIsRXMRXMSGEmpty && mIsRXMRXSTEmpty && mIsRXMRXTIMEmpty) {
            if ((!mIsRXMFIFOOverflow) && (mIsMsgUpdated)) {
                mRXMFIFO[emDATA].pop();
                mRXMFIFO[emStatus].pop();
                mRXMFIFO[emTime].pop();
            }
            if (mRXMFIFO[emDATA].size() > 0) {
                mIsRXMRXMSGEmpty = false;
                mIsRXMRXSTEmpty = false;
                mIsRXMRXTIMEmpty = false;
                mRXMRXMSG = mRXMFIFO[emDATA].front();
                mRXMRXST = mRXMFIFO[emStatus].front();
                mRXMRXTIM = mRXMFIFO[emTime].front();
                mRXMFIFO[emDATA].pop();
                mRXMFIFO[emStatus].pop();
                mRXMFIFO[emTime].pop();
            } else {
                mIsRXMRXMSGEmpty = true;
                mIsRXMRXSTEmpty = true;
                mIsRXMRXTIMEmpty = true;
                mRXMRXMSG = 0x0;
                mRXMRXST = 0x0;
                mRXMRXTIM = 0x0;
            }
            mIsMsgUpdated = false;
            mIsRXMFIFOOverflow = false;
            if ((mRXMFIFO[emDATA].size() == 0x0)
             && (mRXMFIFO[emStatus].size() == 0x0)
             && (mRXMFIFO[emTime].size() == 0x0))  {
                (*RXMMST)["RXMEXIST"] = 0x0;
                (*PSI5INT)["INT_RXMEXIST"] = 0x0;
                mPSI5_Agent->ControlInterruptPorts(emINT_RXMEXIST,false);
            }
        }
    }
}//}}}

///*********************************
/// Function     : cb_RXMFIFO_rxmfifo
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_RXMFIFO_RXMFIFO(RegCBstr str)
{//{{{
    if (mPSI5_Agent->CheckClockPeriod("PCLK") && mPSI5_Agent->CheckClockPeriod("psi5_com_clk")) {
        mRxmFifoReadTimes = mRxmFifoReadTimes + 1;
        if (mRxmFifoReadTimes == 0x1) {//First times
            mIsRXMRXMSGEmpty = true;
            *RXMFIFO = mRXMRXMSG;
        } else if (mRxmFifoReadTimes == 0x2) {//Second times
            mIsRXMRXSTEmpty = true;
            *RXMFIFO = mRXMRXST;
        } else {//Third times
            mIsRXMRXTIMEmpty = true;
            mRxmFifoReadTimes = 0x0;
            *RXMFIFO = mRXMRXTIM;
        }
        if (mIsRXMRXMSGEmpty&&mIsRXMRXSTEmpty&&mIsRXMRXTIMEmpty) {
            if ((!mIsRXMFIFOOverflow) && (mIsMsgUpdated)) {
                mRXMFIFO[emDATA].pop();
                mRXMFIFO[emStatus].pop();
                mRXMFIFO[emTime].pop();
            }
            if (mRXMFIFO[emDATA].size() > 0) {
                mIsRXMRXMSGEmpty = false;
                mIsRXMRXSTEmpty = false;
                mIsRXMRXTIMEmpty = false;
                mRXMRXMSG = mRXMFIFO[emDATA].front();
                mRXMRXST = mRXMFIFO[emStatus].front();
                mRXMRXTIM = mRXMFIFO[emTime].front();
                mRXMFIFO[emDATA].pop();
                mRXMFIFO[emStatus].pop();
                mRXMFIFO[emTime].pop();
            } else {
                mIsRXMRXMSGEmpty = true;
                mIsRXMRXSTEmpty = true;
                mIsRXMRXTIMEmpty = true;
                mRXMRXMSG = 0x0;
                mRXMRXST = 0x0;
                mRXMRXTIM = 0x0;
            }
            mIsMsgUpdated = false;
            mIsRXMFIFOOverflow = false;
            if ((mRXMFIFO[emDATA].size() == 0x0)
             && (mRXMFIFO[emStatus].size() == 0x0)
             && (mRXMFIFO[emTime].size() == 0x0))  {
                (*RXMMST)["RXMEXIST"] = 0x0;
                (*PSI5INT)["INT_RXMEXIST"] = 0x0;
                mPSI5_Agent->ControlInterruptPorts(emINT_RXMEXIST,false);
            }
        }
    }
}//}}}

///*********************************
/// Function     : cb_RXMMSTCLR_rxmfovfclr
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_RXMMSTCLR_RXMFOVFCLR(RegCBstr str)
{//{{{
    if (mPSI5_Agent->CheckClockPeriod("PCLK") && mPSI5_Agent->CheckClockPeriod("psi5_com_clk")) {
        if (((str.data>>8)&0x1) == 0x1) {
            (*RXMMST)["RXMFOVF"] = 0x0;
            (*PSI5INT)["INT_RXMFOVF"] = 0x0;
            mPSI5_Agent->ControlInterruptPorts(emINT_RXMFOVF, false);
        }
        if (((str.data>>16)&0x1) == 0x1) {
            (*RXMMST)["RXMERR"] = 0x0;
            (*PSI5INT)["INT_RXMERR"] = 0x0;
            mPSI5_Agent->ControlInterruptPorts(emINT_RXMERR, false);
        }
    }
    *RXMMSTCLR = 0x0;
}//}}}

///*********************************
/// Function     : cb_RXMMSTINTEN_rxmexistinten
/// Description  : Virtual function of psi5_regif class
///*********************************
void PSI5_Func::cb_RXMMSTINTEN_RXMEXISTINTEN(RegCBstr str)
{//{{{
    if (!mPSI5_Agent->GetResetStatus() && !mIsSoftwareResetEnable) {
        if (mPSI5_Agent->CheckClockPeriod("PCLK") && mPSI5_Agent->CheckClockPeriod("psi5_com_clk")) {
            if (((str.data)&0x1) == 0x0) {
                (*PSI5INT)["INT_RXMEXIST"] = 0x0;
                mPSI5_Agent->ControlInterruptPorts(emINT_RXMEXIST, false);
            } else if ((((str.data)&0x1) == 0x1) && (((str.pre_data)&0x1) == 0x0)){  // added vupham 01/21
                if (((*RXMMST)["RXMEXIST"] == 0x1) && CheckChannelEnable()) {
                    (*PSI5INT)["INT_RXMEXIST"] = 0x1;
                    mPSI5_Agent->ControlInterruptPorts(emINT_RXMEXIST, true);
                } 

            }
            if (((str.data>>8)&0x1) == 0x0) {
                (*PSI5INT)["INT_RXMFOVF"] = 0x0;
                mPSI5_Agent->ControlInterruptPorts(emINT_RXMFOVF, false);
            } else if ((((str.data>>8)&0x1) == 0x1) && (((str.pre_data>>8)&0x1) == 0x0)){  // added vupham 01/21
                if (((*RXMMST)["RXMFOVF"] == 0x1) && CheckChannelEnable()) {
                    (*PSI5INT)["INT_RXMFOVF"] = 0x1;
                    mPSI5_Agent->ControlInterruptPorts(emINT_RXMFOVF, true);
                }

            }
            if (((str.data>>16)&0x1) == 0x0) {
                (*PSI5INT)["INT_RXMERR"] = 0x0;
                mPSI5_Agent->ControlInterruptPorts(emINT_RXMERR, false);
            } else if ((((str.data>>16)&0x1) == 0x1) && (((str.pre_data>>16)&0x1) == 0x0)) { // added vupham 01/21
                if (((*RXMMST)["RXMERR"] == 0x1) && CheckChannelEnable()) {
                    (*PSI5INT)["INT_RXMERR"] = 0x1;
                    mPSI5_Agent->ControlInterruptPorts(emINT_RXMERR, true);
                }

            }
        }
    } else {
        mPSI5_Agent->print_msg("warning","PSI5 model can not operate during reset operation");
    }
}//}}}
void PSI5_Func::UpdateMirrorRegister(unsigned int OffsetAdress)
{//{{{
    if(OffsetAdress == 0x40){
        *EMRXDATA = (unsigned int)(*RXDATA);
    }else if(OffsetAdress == 0x44){
        *EMRXDST = (unsigned int)(*RXDST);
    }else if(OffsetAdress == 0x48){
        *EMRXDTIM = (unsigned int)(*RXDTIM);
    }else if(OffsetAdress == 0x50){
        *EMRXMRXMSG = (unsigned int)(*RXMRXMSG);
    }else if(OffsetAdress == 0x54){
        *EMRXMRXST = (unsigned int)(*RXMRXST);
    }else if(OffsetAdress == 0x58){
        *EMRXMRXTIM = (unsigned int)(*RXMRXTIM);
    }
    else if(OffsetAdress == 0x128){
        *RXDATA = mRXDATA;
    }else if(OffsetAdress == 0x12C){
        *RXDST= mRXDST;
    }else if(OffsetAdress == 0x130){
        *RXDTIM = mRXDTIM;
    }else if(OffsetAdress == 0x184){
        *RXMRXMSG = mRXMRXMSG;
    }else if(OffsetAdress == 0x188){
        *RXMRXST = mRXMRXST;
    }else if(OffsetAdress == 0x18C){
        *RXMRXTIM = mRXMRXTIM;
    }
}//{{{

// vim600: set foldmethod=marker :
