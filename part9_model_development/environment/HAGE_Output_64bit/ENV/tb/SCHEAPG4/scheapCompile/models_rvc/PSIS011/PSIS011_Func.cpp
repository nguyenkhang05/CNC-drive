// ---------------------------------------------------------------------
// $Id: PSIS011_Func.cpp,v 1.5 2017/12/16 09:57:23 chanle Exp $
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
#include "PSIS011_Func.h"
#ifndef func_re_printf
#define func_re_printf mpPSIS011AgentController->get_fileline(__FILE__, __LINE__); mpPSIS011AgentController->_re_printf
#endif//func_re_printf

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
PSIS011_Func::PSIS011_Func (std::string name, PSIS011_AgentController* PSIS011AgentController):
            BusSlaveFuncIf()
{//{{{
    for (unsigned int i = 0; i < emNumChannel; i++){
    	std::ostringstream inst_name;
	inst_name << name << "_channel" << i;
        mpPSIS011ChReg[i] = new PSIS011_Ch_Reg(inst_name.str(), i, this);
    }
    std::ostringstream inst_name;
    inst_name << name << "common";
    mpPSIS011CmnReg = new PSIS011_Cmn_Reg(inst_name.str(), this);
    mIsReset = false;
    mPreOpenBit = 0;

    // Initialize
    assert(PSIS011AgentController != NULL);
    mpPSIS011AgentController = PSIS011AgentController;
    mpPSIS011AgentController->CommandInit(name);
    mModuleName = name;
}//}}}

/// Destructor
PSIS011_Func::~PSIS011_Func()
{//{{{
    for (unsigned int i = 0; i < emNumChannel; i++){
        delete mpPSIS011ChReg[i];
    }
    delete mpPSIS011CmnReg;
}//}}}

/// Reset Func and registers
void PSIS011_Func::EnableReset(const bool is_active)
{//{{{
    mIsReset = is_active;
    mPreOpenBit = 0;
    mpPSIS011CmnReg->EnableReset(is_active);
    for (unsigned int i = 0; i < emNumChannel; i++){
        mpPSIS011ChReg[i]->EnableReset(is_active);
    }
}//}}}

/// Forward Software reset to PSIS011
void PSIS011_Func::SoftwareReset()
{//{{{
    mpPSIS011AgentController->SoftwareReset();
}//}}}

/// Clear all MailBox data
void PSIS011_Func::ClearAllMailBoxData()
{//{{{
    // Clear all mail box data in each channel
    for (unsigned int i = 0; i < emNumChannel; i++){
        mpPSIS011ChReg[i]->ClearAllMailBoxData();
    }
}//}}}

/// Stop generate output clock
void PSIS011_Func::InformChangeSCKENBit(unsigned int value)
{//{{{
    mpPSIS011AgentController->InformChangeSCKENBit(value);
}//}}}

/// Get PCLK frequency
sc_dt::uint64 PSIS011_Func::GetPCLKFreq()
{//{{{
    return mpPSIS011AgentController->GetPCLKFreq();
}//}}}

/// Get ACST bit
unsigned int PSIS011_Func::GetACSTSBit()
{//{{{
    return mpPSIS011CmnReg->GetACSTSBit();
}//}}}

/// Get ACST bit
unsigned int PSIS011_Func::GetMSTSBit()
{//{{{
    return mpPSIS011CmnReg->GetMSTSBit();
}//}}}

/// Clear Timestamp
void PSIS011_Func::ClearTimestamp(std::string timestamp_name)
{//{{{
    assert ((timestamp_name == "A") || (timestamp_name == "B"));
    mpPSIS011AgentController->ClearTimestamp(timestamp_name);
}//}}}

/// Config timestamp A/B 
void PSIS011_Func::ConfigTimestamp(std::string timestamp_name, bool clear_select_gtm, bool enable_select_gtm, bool clock_select_gtm)
{//{{{
    mpPSIS011AgentController->ConfigTimestamp(timestamp_name, clear_select_gtm, enable_select_gtm, clock_select_gtm);
}//}}}

/// Set timestamp enable or not via setting of inner register
void PSIS011_Func::SetTSInnerEnable(std::string timestamp_name, bool ts_inner_enable)
{//{{{
    mpPSIS011AgentController->SetTSInnerEnable(timestamp_name, ts_inner_enable);
}//}}}

/// Set timestamp inner cycle (PCLK) -> generate inner clock for timestamp couting up
void PSIS011_Func::SetTSInnerCycle(unsigned int inner_cycle)
{//{{{
    mpPSIS011AgentController->SetTSInnerCycle(inner_cycle);
}//}}}

/// Inform the mode going to change
void PSIS011_Func::InformChangeMode(unsigned int open, unsigned int opmd)
{//{{{
    mpPSIS011AgentController->InformChangeMode(open, opmd);
    if ((mPreOpenBit == 1) && (open == 0)){// config mode
        mpPSIS011CmnReg->Initialize();
        mpPSIS011CmnReg->ClearAllStatus();
        for (unsigned int i = 0; i < emNumChannel; i++){
            mpPSIS011ChReg[i]->Initialize();
            mpPSIS011ChReg[i]->ClearAllStatus();
        }
    }
    mPreOpenBit = open;
}//}}}

/// Config internal sync pulse info
void PSIS011_Func::GetConfigInnerSyncPulse(unsigned int chid, unsigned int &select, unsigned int &prescaler, unsigned int &expired)
{//{{{
    mpPSIS011ChReg[chid]->GetConfigInnerSyncPulse(select, prescaler, expired);
}//}}}

/// Config internal WDT info
void PSIS011_Func::GetConfigWDT(unsigned int chid, unsigned int &syncmode, unsigned int &prescaler, unsigned int &expired, bool &wdt_enable)
{//{{{
    mpPSIS011ChReg[chid]->GetConfigWDT(syncmode, prescaler, expired, wdt_enable);
}//}}}

/// Store UART data and update related status bit
void PSIS011_Func::StoreUARTData(unsigned int data, bool framing_error, bool parity_error)
{//{{{
    if (! mIsReset){
        mpPSIS011CmnReg->StoreUARTData(data, framing_error, parity_error);
    }
}//}}}

/// Get FmPKT bit, FmPAYLOAD, RFCPS from a channel/frame
void PSIS011_Func::GetConfigPSI5RxFrame(unsigned int chid, unsigned int fid, unsigned int& fpkt, unsigned int& fpayload, unsigned int& rfcps)
{//{{{
    mpPSIS011ChReg[chid]->GetConfigPSI5RxFrame(fid, fpkt, fpayload, rfcps);
}//}}}

/// Get RFCPS bit in a channel
unsigned int PSIS011_Func::GetRFCPSBit(unsigned int chid)
{//{{{
    return mpPSIS011ChReg[chid]->GetRFCPSBit();
}//}}}

/// Get Max IDLE time between PSI5 packet
unsigned int PSIS011_Func::GetMaxIdle()
{//{{{
    return mpPSIS011ChReg[0]->GetMaxIdle();
}//}}}

/// Get UART Rx/Tx parity option
unsigned int PSIS011_Func::GetPSI5SPUPTSReg()
{//{{{
    return mpPSIS011CmnReg->GetPSI5SPUPTSReg();
}//}}}

/// Get clock divider (number of cycle to transmit/receive 1 bit)
void PSIS011_Func::GetClockDivider(unsigned int &rx_divider, unsigned int &tx_divider)
{//{{{
    return mpPSIS011CmnReg->GetClockDivider(rx_divider, tx_divider);
}//}}}

/// Store MB data in according channel/frame
void PSIS011_Func::StoreMBData(unsigned int chid, unsigned int frame, unsigned int status_reg_val, 
                    unsigned int data_reg_val, unsigned int timestamp_reg_val)
{//{{{
    if (! mIsReset){
        mpPSIS011ChReg[chid]->StoreMBData(frame, status_reg_val, data_reg_val, timestamp_reg_val);
    }
}//}}}

/// Get config time stamp of a channel
void PSIS011_Func::GetConfigTimestamp(unsigned int chid, unsigned int &ch_en, unsigned int &ts_sel, unsigned int &ts_en, unsigned int &ts_trg_sel)
{//{{{
    mpPSIS011ChReg[chid]->GetConfigTimestamp(ch_en, ts_sel, ts_en, ts_trg_sel);
}//}}}

/// Capture timestamp of a channel from timestamp a/b
void PSIS011_Func::CaptureTimestamp(unsigned int chid, unsigned int timestamp)
{//{{{
    mpPSIS011ChReg[chid]->CaptureTimestamp(timestamp);
}//}}}

/// Get capture timestamp of a channel
unsigned int PSIS011_Func::GetCapturedTimestamp(unsigned int chid)
{//{{{
    return mpPSIS011ChReg[chid]->GetCapturedTimestamp();
}//}}}

/// Check MB data was read or not
bool PSIS011_Func::CheckMBDataWasRead(unsigned int chid, unsigned int frmid)
{//{{{
    return mpPSIS011ChReg[chid]->CheckMBDataWasRead(frmid);
}//}}}

/// Get DDSR info (frame type, address, data)
void PSIS011_Func::GetDDSRInfo(unsigned int chid, unsigned int &frame_type, unsigned int &address, unsigned int &data)
{//{{{
    mpPSIS011ChReg[chid]->GetDDSRInfo(frame_type, address, data);
}//}}}

/// Get configuration about CPU Tx command data: number of frame, Alternate command, command
void PSIS011_Func::GetConfigCPUTxCom(unsigned int &numfrm, sc_dt::uint64 &tx_cpu_com_data)
{//{{{
    mpPSIS011CmnReg->GetConfigCPUTxCom(numfrm, tx_cpu_com_data);
}//}}}

/// Get CPU Tx command data of a channel
unsigned int PSIS011_Func::GetCommandData(unsigned int chid)
{//{{{
    return mpPSIS011ChReg[chid]->GetCommandData();
}//}}}

/// Notify PSIS011 to send request Tx to Arbiter in PSI5 mode
void PSIS011_Func::NotifySendReqTxToArbiter(unsigned int reqid)
{//{{{
    mpPSIS011AgentController->NotifySendReqTxToArbiter(reqid);
}//}}}

/// Notify transfer UART frame in UART mode
void PSIS011_Func::NotifyTransUART()
{//{{{
    mpPSIS011AgentController->NotifyTransUART();
}//}}}

/// Get data to transfer in UART mode (get UTTDT bit)
unsigned int PSIS011_Func::GetUTTDTBit()
{//{{{
    return mpPSIS011CmnReg->GetUTTDTBit();
}//}}}

/// Update TXSTS bit in common register
void PSIS011_Func::SetTXSTSBit(unsigned int value)
{//{{{
    mpPSIS011CmnReg->SetTXSTSBit(value);
}//}}}

/// Update DDSRSTS bit of channel 1-7
void PSIS011_Func::ClearDDSRSTSBit(unsigned int chid)
{//{{{
    if (! mIsReset){
        mpPSIS011ChReg[chid]->ClearDDSRSTSBit();
    }
}//}}}

/// Inform upper layer to assert interrupt
void PSIS011_Func::AssertInterrupt(unsigned int intrid)
{//{{{
    mpPSIS011AgentController->AssertInterrupt(intrid);
}//}}}

/// Inform upper layer to assert DMA request RX
void PSIS011_Func::AssertDMARequestRX(unsigned int dmarxid)
{//{{{
    mpPSIS011AgentController->AssertDMARequestRX(dmarxid);
}//}}}

/// Inform upper layer to assert DMA request TX
void PSIS011_Func::AssertDMARequestTX(unsigned int dmatxid)
{//{{{
    mpPSIS011AgentController->AssertDMARequestTX(dmatxid);
}//}}}

/// Inform PSIS011 change WDT enable attribute
void PSIS011_Func::ChangeWDTEnableChannel(unsigned int chid, unsigned int enable)
{//{{{
    mpPSIS011AgentController->ChangeWDTEnableChannel(chid, enable);
}//}}}

/// Get NFSET bit to know use Noise Filter or not
unsigned int PSIS011_Func::GetNFSETBit()
{//{{{
    return mpPSIS011CmnReg->GetNFSETBit();
}//}}}

/// Update UTTFIN bit for UART transmit
void PSIS011_Func::UpdateUTTFINBit(unsigned int value)
{//{{{
    if (! mIsReset){
        mpPSIS011CmnReg->UpdateUTTFINBit(value);
    }
}//}}}

/// Update PSI5SPCIS register in specified channel
void PSIS011_Func::SetPSI5SPCISReg(unsigned int chid, std::string status)
{//{{{
    if (! mIsReset){
        mpPSIS011ChReg[chid]->SetPSI5SPCISReg(status);
    }
}//}}}

/// Update PSI5SUCTM register at specified bit
void PSIS011_Func::UpdatePSI5SUCTMReg(std::string bit_name, unsigned int value)
{//{{{
    mpPSIS011CmnReg->UpdatePSI5SUCTMReg(bit_name, value);
}//}}}

/// Dump status info
void PSIS011_Func::DumpStatusInfo()
{//{{{
    mpPSIS011CmnReg->DumpStatusInfo();
    for (unsigned int i = 0; i < emNumChannel; i++){
        mpPSIS011ChReg[i]->DumpStatusInfo();
    }
}//}}}

/// Update TX FIFO status in common register
void PSIS011_Func::SetPSI5SPTFISReg(unsigned int value)
{//{{{
    mpPSIS011CmnReg->SetPSI5SPTFISReg(value);
}//}}}

/// Set status in PSI5SPRES register in a specified channel accoring frame ID
void PSIS011_Func::SetPSI5SPRESReg(unsigned int chid, unsigned int frmid)
{//{{{
    mpPSIS011ChReg[chid]->SetPSI5SPRESReg(frmid);
}//}}}

/// Call to stop transmit DDSR from a channel
void PSIS011_Func::StopTransDDSR(unsigned int txreqid)
{//{{{
    mpPSIS011AgentController->StopTransDDSR(txreqid);
}//}}}

/// Set Software reset status bit
void PSIS011_Func::SetSWSTSBit(unsigned int value)
{//{{{
    mpPSIS011CmnReg->SetSWSTSBit(value);
}//}}}

/// Get attribute loopback enable or not
bool PSIS011_Func::GetLoopbackEnable()
{//{{{
    return mpPSIS011CmnReg->GetLoopbackEnable();
}//}}}

/// Read API (target socket)
void PSIS011_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int address = (unsigned int)trans.get_address();
    unsigned int masked_address = (unsigned int)trans.get_address() & 0xFFF;
    std::string area = FindRegAreaFromAddr(address);

    if (area == "common"){
        mpPSIS011CmnReg->read(address, trans.get_data_ptr(), trans.get_data_length(), debug);
    }else if (area == "channel"){
        unsigned int chid = (masked_address - emBaseOffsetCh0) / emSizeEachChannel;
        mpPSIS011ChReg[chid]->read(area, address, trans.get_data_ptr(), trans.get_data_length(), debug);
    }else if (area == "mailbox"){
        unsigned int chid = (masked_address - emBaseOffsetMBCh0) / emSizeEachMBChannel;
        mpPSIS011ChReg[chid]->read(area, address, trans.get_data_ptr(), trans.get_data_length(), debug);
    }else{// inhibit
        // NOTE: Do nothing for reserved area
    }
}//}}}

/// Write API (target socket)
void PSIS011_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{ 
    unsigned int masked_address = (unsigned int)trans.get_address() & 0xFFF;

    if (! (mpPSIS011AgentController->CheckClockPeriod("PCLK")) ){
        func_re_printf("warning", "Cannot write register when clock PCLK is 0.\n");
        return;
    }
    if (mpPSIS011AgentController->GetResetStatus("PRESETn")){
        func_re_printf("warning", "Cannot write register during reset period of PRESETn.\n");
        return;
    }

    unsigned int address = (unsigned int)trans.get_address();
    std::string area = FindRegAreaFromAddr(address);

    if (area == "common"){
        mpPSIS011CmnReg->write(address, trans.get_data_ptr(), trans.get_data_length(), debug);
    }else if (area == "channel"){
        unsigned int chid = (masked_address - emBaseOffsetCh0) / emSizeEachChannel;
        mpPSIS011ChReg[chid]->write(area, address, trans.get_data_ptr(), trans.get_data_length(), debug);
    }else if (area == "mailbox"){
        unsigned int chid = (masked_address - emBaseOffsetMBCh0) / emSizeEachMBChannel;
        mpPSIS011ChReg[chid]->write(area, address, trans.get_data_ptr(), trans.get_data_length(), debug);
    }else{// inhibit
        func_re_printf("warning", "Cannot write to reserved area (%08X).\n", address);
    }

    // Clear all steps of writing to PUCLB register
    if ((masked_address != emPSI5SPUCLBAddress) && (mpPSIS011CmnReg->GetPUCLBStepDone() != 0)){
        mpPSIS011CmnReg->ClearPUCLBStepDone();
    }
}//}}}

/// Handler for register IF
void PSIS011_Func::RegisterHandler (const std::string reg_area, const unsigned int chid, const std::vector<std::string> &args)
{//{{{
    assert ((reg_area == "common") || (reg_area == "channel") || (reg_area == "mailbox") || (reg_area == "all"));

    if (reg_area == "common"){
        mpPSIS011CmnReg->RegisterHandler(args);
    }else if ((reg_area == "channel") || (reg_area == "mailbox")){
        // Dump warning if invalid chid
        if (chid > 7){
            func_re_printf("warning", "Invalid channel index (%d). It must be from 0 to 7.\n", chid);

        // Process if valid chid
        }else{
            mpPSIS011ChReg[chid]->RegisterHandler(reg_area, args);
        }
    }else{// reg_area == "all"
        mpPSIS011CmnReg->RegisterHandler(args);
        for (unsigned int i = 0; i < emNumChannel; i++){
            mpPSIS011ChReg[i]->RegisterHandler(reg_area, args);
        }
    }
}//}}}

std::string PSIS011_Func::FindRegAreaFromAddr(const unsigned int address)
{//{{{
    std::string area = "";
    unsigned int masked_addres = address & 0xFFF;

    if (masked_addres < emBaseOffsetCh0){
        area = "common";
    }else if (masked_addres < emBaseOffsetMBCh0){
        area = "channel";
    }else if (masked_addres < emBaseOffsetReservedArea){
        area = "mailbox";
    }else{
        area = "inhibit";
    }
    return area;
}//}}}

// vim600: set foldmethod=marker :
