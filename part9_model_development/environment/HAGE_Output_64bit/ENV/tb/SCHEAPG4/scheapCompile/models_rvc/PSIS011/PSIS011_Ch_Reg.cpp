// ---------------------------------------------------------------------
// $Id: PSIS011_Ch_Reg.cpp,v 1.6 2018/07/12 06:20:36 chanle Exp $
//
// Copyright(c) 2017 Renesas Electronics Corporation
// Copyright(c) 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "PSIS011_Ch_Reg.h"
#include "PSIS011_Func.h"
#ifndef ch_re_printf
#define ch_re_printf Cpsis011_ch_regif::get_fileline(__FILE__, __LINE__); Cpsis011_ch_regif::_re_printf
#endif//ch_re_printf

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
PSIS011_Ch_Reg::PSIS011_Ch_Reg (std::string name, unsigned int chid, PSIS011_Func* PSIS011Func):
                Cpsis011_ch_regif(name, 32, chid)
                ,Cpsis011_mbch_regif(name, 32, chid)
{//{{{
    assert(PSIS011Func != NULL);
    mpPSIS011Func = PSIS011Func;
    mModuleName = name;
    mChID = chid;

    Cpsis011_ch_regif::set_instance_name(name);
    Cpsis011_mbch_regif::set_instance_name(name);
    Initialize();
}//}}}

/// Destructor
PSIS011_Ch_Reg::~PSIS011_Ch_Reg()
{//{{{
}//}}}

/// Reset Func and registers
void PSIS011_Ch_Reg::EnableReset(const bool is_active)
{//{{{
    Initialize();
    Cpsis011_ch_regif::EnableReset(is_active);
    Cpsis011_mbch_regif::EnableReset(is_active);
}//}}}

/// Initialize internal variables
void PSIS011_Ch_Reg::Initialize()
{//{{{
    for (unsigned int i = 0; i < emNumMBFrameInChannel; i++){
        mWasReadMBFrm[i] = true;
    }
}//}}}

/// Read API (target socket)
void PSIS011_Ch_Reg::read (std::string area, unsigned int address, unsigned char *p_data, unsigned int size, bool debug)
{//{{{
    assert ((area == "channel") || (area == "mailbox"));

    if (area == "channel"){
        if (!debug) {
            Cpsis011_ch_regif::reg_rd(address, p_data, size);
        } else {
            Cpsis011_ch_regif::reg_rd_dbg(address, p_data, size);
        }
    }else{// area == mailbox
        if (!debug) {
            Cpsis011_mbch_regif::reg_rd(address, p_data, size);
        } else {
            Cpsis011_mbch_regif::reg_rd_dbg(address, p_data, size);
        }
        // Mask that this MB frame was read
        unsigned int masked_address = address & 0x7FF;
        unsigned int frmid = (masked_address - emBaseOffsetMBCh0 - emSizeEachMBChannel*mChID)/emSizeEachMBFrame;
        mWasReadMBFrm[frmid] = true;
    }
}//}}}

/// Write API (target socket)
void PSIS011_Ch_Reg::write (std::string area, unsigned int address, unsigned char *p_data, unsigned int size, bool debug)
{//{{{
    assert ((area == "channel") || (area == "mailbox"));

    if (area == "channel"){
        if (!debug) {
            Cpsis011_ch_regif::reg_wr(address, p_data, size);
        } else {
            Cpsis011_ch_regif::reg_wr_dbg(address, p_data, size);
        }
    }else{// area == mailbox
        if (!debug) {
            Cpsis011_mbch_regif::reg_wr(address, p_data, size);
        } else {
            Cpsis011_mbch_regif::reg_wr_dbg(address, p_data, size);
        }
    }
}//}}}

/// Handler for register IF
void PSIS011_Ch_Reg::RegisterHandler (const std::string reg_area, const std::vector<std::string> &args)
{//{{{
    assert((reg_area == "channel") || (reg_area == "mailbox") || (reg_area == "all"));

    // Process channel registers
    if ((reg_area == "channel") || (reg_area == "all")){
        std::string msg_return = "";
        if (args.size() == 1) {//Dump registers name
            vpcl::re_register *reg_p = Cpsis011_ch_regif::first_reg_object();
            while (reg_p != NULL) {
                SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
                reg_p = Cpsis011_ch_regif::next_reg_object();
            }
        } else {
            if (args[1] == "*") {//Set message level for each register
                std::vector<std::string> cmd;
                vpcl::re_register *reg_p = Cpsis011_ch_regif::first_reg_object();
                while (reg_p != NULL) {
                    cmd = args;
                    cmd.erase(cmd.begin()+1);
                    cmd.insert(cmd.begin()+1, reg_p->name());
                    msg_return = Cpsis011_ch_regif::reg_handle_command(cmd);
                    if (msg_return != "") {
                        SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                    }
                    cmd.clear();
                    reg_p = Cpsis011_ch_regif::next_reg_object();
                }
            } else {//Set handle command to Cpsis011_ch_regif class
                msg_return = Cpsis011_ch_regif::reg_handle_command(args);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
            }
        }

    // Process Mail Box registers
    }
    if ((reg_area == "mailbox") || (reg_area == "all")){
        std::string msg_return = "";
        if (args.size() == 1) {//Dump registers name
            vpcl::re_register *reg_p = Cpsis011_mbch_regif::first_reg_object();
            while (reg_p != NULL) {
                SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
                reg_p = Cpsis011_mbch_regif::next_reg_object();
            }
        } else {
            if (args[1] == "*") {//Set message level for each register
                std::vector<std::string> cmd;
                vpcl::re_register *reg_p = Cpsis011_mbch_regif::first_reg_object();
                while (reg_p != NULL) {
                    cmd = args;
                    cmd.erase(cmd.begin()+1);
                    cmd.insert(cmd.begin()+1, reg_p->name());
                    msg_return = Cpsis011_mbch_regif::reg_handle_command(cmd);
                    if (msg_return != "") {
                        SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                    }
                    cmd.clear();
                    reg_p = Cpsis011_mbch_regif::next_reg_object();
                }
            } else {//Set handle command to Cpsis011_mbch_regif class
                msg_return = Cpsis011_mbch_regif::reg_handle_command(args);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
            }
        }
    }
}//}}}

/// Get config for Inner Sync Pulse generator
void PSIS011_Ch_Reg::GetConfigInnerSyncPulse (unsigned int &select, unsigned int &prescaler, unsigned int &expired)
{//{{{
    select = (unsigned int)(*PSI5SPSTSn)["STSEL"];
    prescaler = (unsigned int)(*PSI5SPSTPn)["STPRS"];
    expired = (unsigned int)(*PSI5SPSTEVn)["STEX"];
}//}}}

/// Get config for WDT
void PSIS011_Ch_Reg::GetConfigWDT (unsigned int &syncmode, unsigned int &prescaler, unsigned int &expired, bool &wdt_enable)
{//{{{  
    if (mChID == 0){
        syncmode = 1;// channel 0 always operate in async mode
    }else{
        syncmode = (unsigned int)(*PSI5SPRCF1n)["SYSEL"];
    }
    prescaler = (unsigned int)(*PSI5SPWDPn)["WDTPRS"];
    expired = (unsigned int)(*PSI5SPWDEVn)["WDTEX"];
    wdt_enable = ((unsigned int)(*PSI5SPWDEn)["WDTEB"] == 1);
}//}}}

/// Get FnPKT, FnPAYLOAD, RFCPS bit of a PSI5 frame
void PSIS011_Ch_Reg::GetConfigPSI5RxFrame (unsigned int fid, unsigned int& fpkt, unsigned int& fpayload, unsigned int& rfcps)
{//{{{
    unsigned int prcf1 = (unsigned int)(*PSI5SPRCF1n);
    unsigned int prcf2 = (unsigned int)(*PSI5SPRCF2n);
    fpkt = (prcf1 >> (6 + ((fid) * 3))) & 0x7;
    fpayload = (prcf2 >> ((fid)*5)) & 0x1F;
    rfcps = (unsigned int)(*PSI5SPRCF1n)["RFCPS"];
}//}}}

/// Get RFCPS bit
unsigned int PSIS011_Ch_Reg::GetRFCPSBit()
{//{{{
    return (unsigned int)(*PSI5SPRCF1n)["RFCPS"];
}//}}}

/// Get Max IDLE time between PSI5 packet
unsigned int PSIS011_Ch_Reg::GetMaxIdle()
{//{{{
    return (unsigned int)(*PSI5SPRCF1n)["PFRMIDLE"];
}//}}}

/// Store MB data into frame
void PSIS011_Ch_Reg::StoreMBData(unsigned int frame, unsigned int status_reg_val, unsigned int data_reg_val, unsigned int timestamp_reg_val)
{//{{{
    assert (frame <= 6);
    unsigned int frmid = frame - 1;// frame 1-6, mWasReadMBFrm index from 0-5

    // Process for channel 0 (has only frm1,2)
    if (mChID == 0){
        if (frame > 2){
            frame = 2;
        }
    }
    if (mWasReadMBFrm[frmid]){ 
        SetPSI5SPCISReg("RFN");
    }else{
        SetPSI5SPCISReg("ROV");
    }
    mWasReadMBFrm[frmid] = false;
    switch (frame){
        case 1:
            (*PSI5SPMBn1S) = status_reg_val;
            (*PSI5SPMBn1D) = data_reg_val;
            (*PSI5SPMBn1T) = timestamp_reg_val;
            break;
        case 2:
            (*PSI5SPMBn2S) = status_reg_val;
            (*PSI5SPMBn2D) = data_reg_val;
            (*PSI5SPMBn2T) = timestamp_reg_val;
            break;
        case 3:
            (*PSI5SPMBn3S) = status_reg_val;
            (*PSI5SPMBn3D) = data_reg_val;
            (*PSI5SPMBn3T) = timestamp_reg_val;
            break;
        case 4:
            (*PSI5SPMBn4S) = status_reg_val;
            (*PSI5SPMBn4D) = data_reg_val;
            (*PSI5SPMBn4T) = timestamp_reg_val;
            break;
        case 5:
            (*PSI5SPMBn5S) = status_reg_val;
            (*PSI5SPMBn5D) = data_reg_val;
            (*PSI5SPMBn5T) = timestamp_reg_val;
            break;
        default: //case 6:
            (*PSI5SPMBn6S) = status_reg_val;
            (*PSI5SPMBn6D) = data_reg_val;
            (*PSI5SPMBn6T) = timestamp_reg_val;
            break;
    };
}//}}}

/// Get timestamp config
void PSIS011_Ch_Reg::GetConfigTimestamp(unsigned int &ch_en, unsigned int &ts_sel, unsigned int &ts_en, unsigned int &ts_trg_sel)
{//{{{
    ch_en = (unsigned int)(*PSI5SPRCF1n)["CHEN"];
    ts_sel = (unsigned int)(*PSI5SPRCF1n)["TSCS"];
    ts_en = (unsigned int)(*PSI5SPRCF1n)["TSEN"];
    if (mChID > 0){
        ts_trg_sel = (unsigned int)(*PSI5SPRCF1n)["TSCTS"];
    }
}//}}}

/// Capture timestamp into PSI5SPTCDTn register
void PSIS011_Ch_Reg::CaptureTimestamp(unsigned int timestamp)
{//{{{
    (*PSI5SPTCDTn)["TSCD"] = timestamp;
}//}}}

/// Get captured timestamp from PSI5SPTCDTn register
unsigned int PSIS011_Ch_Reg::GetCapturedTimestamp()
{//{{{
    return (unsigned int)(*PSI5SPTCDTn)["TSCD"];
}//}}}

/// Check MB data was read or not
bool PSIS011_Ch_Reg::CheckMBDataWasRead(unsigned int frmid)
{//{{{
    return mWasReadMBFrm[frmid-1];// frmid 1-6, mWasReadMBFrm index from 0-5
}//}}}

/// Get DDSR info about frame type, address, data based on setting in register 
void PSIS011_Ch_Reg::GetDDSRInfo(unsigned int &frame_type, unsigned int &address, unsigned int &data)
{//{{{
    frame_type = (unsigned int)(*PSI5SPDDTPn);

    if (frame_type == 0){// frame 1 -> get 3 bits address, 3 bits data
        address = (unsigned int)(*PSI5SPDDDn)["DDSRADR"] & 0x7;
        data = (unsigned int)(*PSI5SPDDDn)["DDSRDT"] & 0x7;
    }else if (frame_type == 1){// frame 2 -> get 3 bits address, 13 bits data
        address = (unsigned int)(*PSI5SPDDDn)["DDSRADR"] & 0x7;
        data = (unsigned int)(*PSI5SPDDDn)["DDSRDT"] & 0x1FFF;
    }else if (frame_type == 2){// frame 3 -> get 3 bits address, 19 bits data
        address = (unsigned int)(*PSI5SPDDDn)["DDSRADR"] & 0x7;
        data = (unsigned int)(*PSI5SPDDDn)["DDSRDT"] & 0x7FFFF;
    }else{// frame 4 -> get 4 bits address, 20 bits data
        address = (unsigned int)(*PSI5SPDDDn)["DDSRADR"] & 0xF;
        data = (unsigned int)(*PSI5SPDDDn)["DDSRDT"] & 0xFFFFF;
    }
}//}}}

/// Get CPU Tx command data
unsigned int PSIS011_Ch_Reg::GetCommandData()
{//{{{
    return (unsigned int)(*PSI5SPTCDn);
}//}}}

/// Update DDSRSTS bit 
void PSIS011_Ch_Reg::ClearDDSRSTSBit()
{//{{{
    (*PSI5SPDDSn)["DDSRSTS"] = 0;
}//}}}

/// Clear all mailbox data
void PSIS011_Ch_Reg::ClearAllMailBoxData()
{//{{{
    if (mChID == 0){
        (*PSI5SPMBn1S) = 0;
        (*PSI5SPMBn1D) = 0;
        (*PSI5SPMBn1T) = 0;
        (*PSI5SPMBn2S) = 0;
        (*PSI5SPMBn2D) = 0;
        (*PSI5SPMBn2T) = 0;
    }else{
        (*PSI5SPMBn1S) = 0;
        (*PSI5SPMBn1D) = 0;
        (*PSI5SPMBn1T) = 0;
        (*PSI5SPMBn2S) = 0;
        (*PSI5SPMBn2D) = 0;
        (*PSI5SPMBn2T) = 0;
        (*PSI5SPMBn3S) = 0;
        (*PSI5SPMBn3D) = 0;
        (*PSI5SPMBn3T) = 0;
        (*PSI5SPMBn4S) = 0;
        (*PSI5SPMBn4D) = 0;
        (*PSI5SPMBn4T) = 0;
        (*PSI5SPMBn5S) = 0;
        (*PSI5SPMBn5D) = 0;
        (*PSI5SPMBn5T) = 0;
        (*PSI5SPMBn6S) = 0;
        (*PSI5SPMBn6D) = 0;
        (*PSI5SPMBn6T) = 0;
    }
}//}}}

/// Set PSI5SPCIS register at a specified bit name
void PSIS011_Ch_Reg::SetPSI5SPCISReg(std::string status)
{//{{{
    assert ((status == "CTFN") || (status == "DDSFN") || (status == "DDSOW") || (status == "RFN") || (status == "RFEX")
        || (status == "RFLK") || (status == "ROV") || (status == "RWDT") || (status == "UTFR") || (status == "UTPT")
        || (status == "TRST") || (status == "PT") || (status == "CRC") || (status == "XCRC"));
    std::ostringstream int_bit_name, en_int_bit_name;
    int_bit_name << "IST" << status;
    en_int_bit_name << "IEB" << status;

    // Update status bit 
    (*PSI5SPCISn)[int_bit_name.str().c_str()] = 1;
    // Assert interrupt
    if ((unsigned int)(*PSI5SPCIEn)[en_int_bit_name.str().c_str()] == 1){
        mpPSIS011Func->AssertInterrupt(mChID);
    }

    // DMA if any
    if ((status == "DDSFN") && ((unsigned int)(*PSI5SPDREn)["DRQETFN"] == 1)){
        mpPSIS011Func->AssertDMARequestTX(mChID - 1); // index of DMA TX is 0-6 for channel 1-7
    }
    if ( ((status == "RWDT") && ((unsigned int)(*PSI5SPDREn)["DRQEWDT"] == 1))
        || ((status == "RFN") && ((unsigned int)(*PSI5SPDREn)["DRQERFN"] == 1)) ){
        mpPSIS011Func->AssertDMARequestRX(mChID);
    }
}//}}}

/// Dump value of status registers
void PSIS011_Ch_Reg::DumpStatusInfo()
{//{{{
    printf("Status info of status registers of channel %d:\n", mChID);
    printf("    PSI5SPRESn = %08X\n", (unsigned int)(*PSI5SPRESn));
    printf("    PSI5SPCISn = %08X\n", (unsigned int)(*PSI5SPCISn));
    if (mChID > 0){
        printf("    PSI5SPDDSn = %08X\n", (unsigned int)(*PSI5SPDDSn));
    }
}//}}}

/// Clear status when mode is change to inactive
void PSIS011_Ch_Reg::ClearAllStatus()
{//{{{
    (*PSI5SPRESn) = 0;
    (*PSI5SPCISn) = 0;
    if (mChID > 0){
        (*PSI5SPDDSn) = 0;
    }
}//}}}

/// Set status in PSI5SPRES register at a specified frame ID
void PSIS011_Ch_Reg::SetPSI5SPRESReg(unsigned int frmid)
{//{{{
    unsigned int value = (unsigned int)(*PSI5SPRESn);
    value |= (1 << (frmid -1));
    (*PSI5SPRESn) = value;
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================

/// Callback of PSI5SPRCF1n register
void PSIS011_Ch_Reg::cb_PSI5SPRCF1n_CHEN(Cpsis011_ch_regif::RegCBstr str)
{//{{{
    unsigned int acsts = mpPSIS011Func->GetACSTSBit();

    if (acsts == 1){
        ch_re_printf("warning", "Cannot write PSI5SPRCF1%d register in UART mode or PSI5 mode.\n", mChID);
        (*PSI5SPRCF1n) = str.pre_data;
        return;
    }

    // Check FiPKT bits
    for (unsigned int i = 0; i < 6; i++){
        unsigned int fipkt = (str.data >> (6 + i*3)) & 0x7;
        std::ostringstream bit_name;
        bit_name << "F" << (i+1) << "PKT";
        if ((0< fipkt) && (fipkt < 3)){
            (*PSI5SPRCF1n)[bit_name.str().c_str()] = 3;
            ch_re_printf("warning", "Cannot write prohibited value (%d) to PSI5SPRCF1%d.%s bit. Value 3 is written to this bit.\n", fipkt, mChID, bit_name.str().c_str());
        }else if (fipkt > 6){
            (*PSI5SPRCF1n)[bit_name.str().c_str()] = 6;
            ch_re_printf("warning", "Cannot write prohibited value (%d) to PSI5SPRCF1%d.%s bit. Value 6 is written to this bit.\n", fipkt, mChID, bit_name.str().c_str());
        }
    }

    // Clear TSCD if write 0 to TSEN of each channel
    if ((unsigned int)(*PSI5SPRCF1n)["TSEN"] == 0){
        (*PSI5SPTCDTn)["TSCD"] = 0;
    }
}//}}}

/// Callback of PSI5SPRCF2n register
void PSIS011_Ch_Reg::cb_PSI5SPRCF2n_F1PAYLD(Cpsis011_ch_regif::RegCBstr str)
{//{{{
    unsigned int acsts = mpPSIS011Func->GetACSTSBit();

    if (acsts == 1){
        ch_re_printf("warning", "Cannot write PSI5SPRCF2%d register in UART mode or PSI5 mode.\n", mChID);
        (*PSI5SPRCF2n) = str.pre_data;
    }

    // Check FiPAYLD bits
    for (unsigned int i = 0; i < 6; i++){
        unsigned int fipayld = (str.data >> (i*5)) & 0x1F;
        std::ostringstream bit_name;
        bit_name << "F" << (i+1) << "PAYLD";
        if (fipayld < 8){
            (*PSI5SPRCF2n)[bit_name.str().c_str()] = 8;
            ch_re_printf("warning", "Cannot write prohibited value (%d) to PSI5SPRCF2%d.%s bit. Value 8 is written to this bit.\n", fipayld, mChID, bit_name.str().c_str());
        }else if (fipayld > 28){
            (*PSI5SPRCF2n)[bit_name.str().c_str()] = 28;
            ch_re_printf("warning", "Cannot write prohibited value (%d) to PSI5SPRCF2%d.%s bit. Value 28 is written to this bit.\n", fipayld, mChID, bit_name.str().c_str());
        }
    }
}//}}}

/// Callback of PSI5SPWDEn register
void PSIS011_Ch_Reg::cb_PSI5SPWDEn_WDTEB(Cpsis011_ch_regif::RegCBstr str)
{//{{{
    unsigned int acsts = mpPSIS011Func->GetACSTSBit();
    unsigned int msts = mpPSIS011Func->GetMSTSBit();

    if ((acsts == 1) && (msts == 0)){
        ch_re_printf("warning", "Cannot write PSI5SPWDE%d register in UART mode.\n", mChID);
        (*PSI5SPWDEn) = str.pre_data;
    }else{
        mpPSIS011Func->ChangeWDTEnableChannel(mChID, (unsigned int)(*PSI5SPWDEn)["WDTEB"]);
    }
}//}}}

/// Callback of PSI5SPWDPn register
void PSIS011_Ch_Reg::cb_PSI5SPWDPn_WDTPRS(Cpsis011_ch_regif::RegCBstr str)
{//{{{
    unsigned int acsts = mpPSIS011Func->GetACSTSBit();

    if (acsts == 1){
        ch_re_printf("warning", "Cannot write PSI5SPWDP%d register in UART mode or PSI5 mode.\n", mChID);
        (*PSI5SPWDPn) = str.pre_data;
    }
}//}}}

/// Callback of PSI5SPWDEVn register
void PSIS011_Ch_Reg::cb_PSI5SPWDEVn_WDTEX(Cpsis011_ch_regif::RegCBstr str)
{//{{{
    unsigned int acsts = mpPSIS011Func->GetACSTSBit();

    if (acsts == 1){
        ch_re_printf("warning", "Cannot write PSI5SPWDEV%d register in UART mode or PSI5 mode.\n", mChID);
        (*PSI5SPWDEVn) = str.pre_data;
    }
}//}}}

/// Callback of PSI5SPTCDn register
void PSIS011_Ch_Reg::cb_PSI5SPTCDn_CHID(Cpsis011_ch_regif::RegCBstr str)
{//{{{
    unsigned int acsts = mpPSIS011Func->GetACSTSBit();

    if (acsts == 1){
        ch_re_printf("warning", "Cannot write PSI5SPTCD%d register in UART mode or PSI5 mode.\n", mChID);
        (*PSI5SPTCDn) = str.pre_data;
    }
}//}}}

/// Callback of PSI5SPCIEn register
void PSIS011_Ch_Reg::cb_PSI5SPCIEn_IEBCRC(Cpsis011_ch_regif::RegCBstr str)
{//{{{
    unsigned int acsts = mpPSIS011Func->GetACSTSBit();
    unsigned int msts = mpPSIS011Func->GetMSTSBit();

    if ((acsts == 1) && (msts == 0)){
        ch_re_printf("warning", "Cannot write PSI5SPCIE%d register in UART mode.\n", mChID);
        (*PSI5SPCIEn) = str.pre_data;
    }
}//}}}

/// Callback of PSI5SPDREn register
void PSIS011_Ch_Reg::cb_PSI5SPDREn_DRQERFN(Cpsis011_ch_regif::RegCBstr str)
{//{{{
    unsigned int acsts = mpPSIS011Func->GetACSTSBit();
    unsigned int msts = mpPSIS011Func->GetMSTSBit();

    if ((acsts == 1) && (msts == 0)){
        ch_re_printf("warning", "Cannot write PSI5SPDRE%d register in UART mode.\n", mChID);
        (*PSI5SPDREn) = str.pre_data;
    }
}//}}}

/// Callback of PSI5SPSTPn register
void PSIS011_Ch_Reg::cb_PSI5SPSTPn_STPRS(Cpsis011_ch_regif::RegCBstr str)
{//{{{
    unsigned int acsts = mpPSIS011Func->GetACSTSBit();
    unsigned int msts = mpPSIS011Func->GetMSTSBit();

    if (acsts == 1){
        ch_re_printf("warning", "Cannot write PSI5SPSTP%d register in UART mode or PSI5 mode.\n", mChID);
        (*PSI5SPSTPn) = str.pre_data;
    }
}//}}}

/// Callback of PSI5SPSTEVn register
void PSIS011_Ch_Reg::cb_PSI5SPSTEVn_STEX(Cpsis011_ch_regif::RegCBstr str)
{//{{{ 
    unsigned int acsts = mpPSIS011Func->GetACSTSBit();

    if (acsts == 1){
        ch_re_printf("warning", "Cannot write PSI5SPSTEV%d register in UART mode or PSI5 mode.\n", mChID);
        (*PSI5SPSTEVn) = str.pre_data;
    }
}//}}}

/// Callback of PSI5SPSTSn register
void PSIS011_Ch_Reg::cb_PSI5SPSTSn_STSEL(Cpsis011_ch_regif::RegCBstr str)
{//{{{
    unsigned int acsts = mpPSIS011Func->GetACSTSBit();

    if (acsts == 1){
        ch_re_printf("warning", "Cannot write PSI5SPSTS%d register in UART mode or PSI5 mode.\n", mChID);
        (*PSI5SPSTSn) = str.pre_data;
    }
}//}}}

/// Callback of PSI5SPRESCn register
void PSIS011_Ch_Reg::cb_PSI5SPRESCn_RERRCLF1(Cpsis011_ch_regif::RegCBstr str)
{//{{{
    unsigned int acsts = mpPSIS011Func->GetACSTSBit();
    unsigned int msts = mpPSIS011Func->GetMSTSBit();

    if ((acsts == 0) || (msts == 0)){
        ch_re_printf("warning", "Cannot write PSI5SPRESC%d register in UART mode or Configuration mode.\n", mChID);
        (*PSI5SPRESCn) = 0;
        return;
    }

    // Clear according bits
    unsigned int num_frame = 6;
    if (mChID == 0){
        num_frame = 2;
    }
    for (unsigned int i = 1; i <= num_frame; i++){
        std::ostringstream bit_clear_name, bit_status_name;
        bit_clear_name << "RERRCLF" << i;
        bit_status_name << "RERRF" << i;
        if ((unsigned int)(*PSI5SPRESCn)[bit_clear_name.str().c_str()] == 1){
            (*PSI5SPRESn)[bit_status_name.str().c_str()] = 0;
        }
    }

    // Always read as 0
    (*PSI5SPRESCn) = 0;
}//}}}

/// Callback of PSI5SPTCDCn register
void PSIS011_Ch_Reg::cb_PSI5SPTCDCn_TSCCLR(Cpsis011_ch_regif::RegCBstr str)
{//{{{
    unsigned int acsts = mpPSIS011Func->GetACSTSBit();
    unsigned int msts = mpPSIS011Func->GetMSTSBit();

    if ((acsts == 1) && (msts == 0)){
        ch_re_printf("warning", "Cannot write PSI5SPTCDC%d register in UART mode.\n", mChID);
        (*PSI5SPTCDCn) = 0;
        return;
    }

    // Clear timestamp
    if ((unsigned int)(*PSI5SPTCDCn)["TSCCLR"] == 1){
        (*PSI5SPTCDTn)["TSCD"] = 0;
    }

    // Always read as 0
    (*PSI5SPTCDCn) = 0;
}//}}}

/// Callback of PSI5SPDDTPn register
void PSIS011_Ch_Reg::cb_PSI5SPDDTPn_DDSRTYPE(Cpsis011_ch_regif::RegCBstr str)
{//{{{
    unsigned int acsts = mpPSIS011Func->GetACSTSBit();
    unsigned int msts = mpPSIS011Func->GetMSTSBit();

    if ((acsts == 1) && (msts == 0)){
        ch_re_printf("warning", "Cannot write PSI5SPDDTP%d register in UART mode.\n", mChID);
        (*PSI5SPDDTPn) = str.pre_data;
    }

    // Writing is prohibited when DDSRSTS = 1
    if ((unsigned int)(*PSI5SPDDSn)["DDSRSTS"] == 1){
        ch_re_printf("warning", "Cannot write PSI5SPDDTP%d register when PSI5SPDDS%d.DDSRSTS is 1.\n", mChID, mChID);
        (*PSI5SPDDTPn) = str.pre_data;
    }
}//}}}

/// Callback of PSI5SPDDDn register
void PSIS011_Ch_Reg::cb_PSI5SPDDDn_DDSRADR(Cpsis011_ch_regif::RegCBstr str)
{//{{{
    unsigned int acsts = mpPSIS011Func->GetACSTSBit();
    unsigned int msts = mpPSIS011Func->GetMSTSBit();
    unsigned int syncmode = (unsigned int)(*PSI5SPRCF1n)["SYSEL"];
    unsigned int ch_en = (unsigned int)(*PSI5SPRCF1n)["CHEN"];

    if ((acsts == 1) && (msts == 0)){
        ch_re_printf("warning", "Cannot write PSI5SPDDD%d register in UART mode.\n", mChID);
        (*PSI5SPDDDn) = str.pre_data;
        return;
    }

    // Cannot write when DDSRSTS = 1
    if ((unsigned int)(*PSI5SPDDSn)["DDSRSTS"] == 1){
        ch_re_printf("warning", "Cannot write PSI5SPDDD%d register when PSI5SPDDS%d.DDSRSTS is 1.\n", mChID, mChID);
        (*PSI5SPDDDn) = str.pre_data;
        SetPSI5SPCISReg("DDSOW");
        return;
    }

    // Can write -> set status bit
    if ((acsts == 1) && (msts == 1) && (syncmode == 0) && (ch_en == 1)){
        (*PSI5SPDDSn)["DDSRSTS"] = 1;
        // Send request to arbiter
        mpPSIS011Func->NotifySendReqTxToArbiter(mChID-1);
    }
}//}}}

/// Callback of PSI5SPDDSPn register
void PSIS011_Ch_Reg::cb_PSI5SPDDSPn_DDSRSTP(Cpsis011_ch_regif::RegCBstr str)
{//{{{
    unsigned int acsts = mpPSIS011Func->GetACSTSBit();
    unsigned int msts = mpPSIS011Func->GetMSTSBit();

    if ((acsts == 0) || (msts == 0)){
        ch_re_printf("warning", "Cannot write PSI5SPDDSP%d register in UART mode or Configuration mode.\n", mChID);
        (*PSI5SPDDSPn) = 0;
        return;
    }

    // Write 1 -> clear status bit, set all DDSR-SHIFT to all 1.
    if ((unsigned int)(*PSI5SPDDSPn)["DDSRSTP"] == 1){
        (*PSI5SPDDSn)["DDSRSTS"] = 0;
        mpPSIS011Func->StopTransDDSR(mChID - 1);// txreqid = chid - 1
    }

    // Always read as 0
    (*PSI5SPDDSPn) = 0;
}//}}}

/// Callback of PSI5SPCISCn register
void PSIS011_Ch_Reg::cb_PSI5SPCISCn_ISTCCRC(Cpsis011_ch_regif::RegCBstr str)
{//{{{
    unsigned int acsts = mpPSIS011Func->GetACSTSBit();
    unsigned int msts = mpPSIS011Func->GetMSTSBit();

    if ((acsts == 0) || (msts == 0)){
        ch_re_printf("warning", "Cannot write PSI5SPCISC%d register in UART mode or Configuration mode.\n", mChID);
        (*PSI5SPCISCn) = 0;
        return;
    }

    // Clear status bit according
    if (mChID == 0){
        if ((unsigned int)(*PSI5SPCISCn)["ISTCCTFN"] == 1){
            (*PSI5SPCISn)["ISTCTFN"] = 0;
        }
    }
    if (mChID > 0){
        if ((unsigned int)(*PSI5SPCISCn)["ISTCDDSFN"] == 1){
            (*PSI5SPCISn)["ISTDDSFN"] = 0;
        }
        if ((unsigned int)(*PSI5SPCISCn)["ISTCDDSOW"] == 1){
            (*PSI5SPCISn)["ISTDDSOW"] = 0;
        }
    }
    if ((unsigned int)(*PSI5SPCISCn)["ISTCRFN"] == 1){
        (*PSI5SPCISn)["ISTRFN"] = 0;
    }
    if ((unsigned int)(*PSI5SPCISCn)["ISTCRFEX"] == 1){
        (*PSI5SPCISn)["ISTRFEX"] = 0;
    }
    if ((unsigned int)(*PSI5SPCISCn)["ISTCRFLK"] == 1){
        (*PSI5SPCISn)["ISTRFLK"] = 0;
    }
    if ((unsigned int)(*PSI5SPCISCn)["ISTCROV"] == 1){
        (*PSI5SPCISn)["ISTROV"] = 0;
    }
    if ((unsigned int)(*PSI5SPCISCn)["ISTCRWDT"] == 1){
        (*PSI5SPCISn)["ISTRWDT"] = 0;
    }
    if (mChID == 0){
        if ((unsigned int)(*PSI5SPCISCn)["ISTCUTFR"] == 1){
            (*PSI5SPCISn)["ISTUTFR"] = 0;
        }
        if ((unsigned int)(*PSI5SPCISCn)["ISTCUTPT"] == 1){
            (*PSI5SPCISn)["ISTUTPT"] = 0;
        }
    }
    if ((unsigned int)(*PSI5SPCISCn)["ISTCTRST"] == 1){
        (*PSI5SPCISn)["ISTTRST"] = 0;
    }
    if ((unsigned int)(*PSI5SPCISCn)["ISTCPT"] == 1){
        (*PSI5SPCISn)["ISTPT"] = 0;
    }
    if ((unsigned int)(*PSI5SPCISCn)["ISTCCRC"] == 1){
        (*PSI5SPCISn)["ISTCRC"] = 0;
    }
    if (mChID == 0){
        if ((unsigned int)(*PSI5SPCISCn)["ISTCXCRC"] == 1){
            (*PSI5SPCISn)["ISTXCRC"] = 0;
        }
    }

    // Always read as 0
    (*PSI5SPCISCn) = 0;
}//}}}


// vim600: set foldmethod=marker :
