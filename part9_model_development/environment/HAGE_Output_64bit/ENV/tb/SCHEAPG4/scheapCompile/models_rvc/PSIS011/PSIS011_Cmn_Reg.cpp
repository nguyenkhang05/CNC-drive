// ---------------------------------------------------------------------
// $Id: PSIS011_Cmn_Reg.cpp,v 1.5 2017/12/16 09:57:23 chanle Exp $
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
#include "PSIS011_Cmn_Reg.h"
#include "PSIS011_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
PSIS011_Cmn_Reg::PSIS011_Cmn_Reg (std::string name, PSIS011_Func* PSIS011Func):
                Cpsis011_cmn_regif(name, 32)
{//{{{
    assert(PSIS011Func != NULL);
    mpPSIS011Func = PSIS011Func;
    mModuleName = name;

    Cpsis011_cmn_regif::set_instance_name(name);
    Initialize();
}//}}}

/// Destructor
PSIS011_Cmn_Reg::~PSIS011_Cmn_Reg()
{//{{{
}//}}}

/// Reset Func and registers
void PSIS011_Cmn_Reg::EnableReset(const bool is_active)
{//{{{
    Initialize();
    Cpsis011_cmn_regif::EnableReset(is_active);
}//}}}

/// Initialize internal variables
void PSIS011_Cmn_Reg::Initialize()
{//{{{
    mPUCLBStepDone = 0;
    mLoopbackEnable = false;
    mWasReadUARTRxData = true;
}//}}}

/// Set mPUCLBStepDone variable
void PSIS011_Cmn_Reg::ClearPUCLBStepDone()
{//{{{
    mPUCLBStepDone = 0;
}//}}}

/// Get mPUCLBStepDone variable
unsigned int PSIS011_Cmn_Reg::GetPUCLBStepDone()
{//{{{
    return mPUCLBStepDone;
}//}}}

/// Read API (target socket)
void PSIS011_Cmn_Reg::read (unsigned int address, unsigned char *p_data, unsigned int size, bool debug)
{//{{{
    if (!debug) {
        reg_rd(address, p_data, size);
    } else {
        reg_rd_dbg(address, p_data, size);
    }
}//}}}

/// Write API (target socket)
void PSIS011_Cmn_Reg::write (unsigned int address, unsigned char *p_data, unsigned int size, bool debug)
{//{{{
    if (!debug) {
        reg_wr(address, p_data, size);
    } else {
        reg_wr_dbg(address, p_data, size);
    }
}//}}}

/// Handler for register IF
void PSIS011_Cmn_Reg::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cpsis011_cmn_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to regif
            msg_return = Cpsis011_cmn_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// Update PSI5SPUOS register
void PSIS011_Cmn_Reg::UpdatePUOSReg()
{//{{{
    unsigned int open = (unsigned int)(*PSI5SPUOEB)["OPEN"];
    unsigned int opmd = (unsigned int)(*PSI5SPUOMD)["OPMD"];

    // Update MSTS bit
    if (open == 1){
        if (opmd ==1){
            (*PSI5SPUOS)["MSTS"] = 1;
        }else{
            (*PSI5SPUOS)["MSTS"] = 0;
        }
    }
    // Update ACSTS bit
    (*PSI5SPUOS)["ACSTS"] = open;
}//}}}

/// Get ACSTS bit
unsigned int PSIS011_Cmn_Reg::GetACSTSBit()
{//{{{
    return (unsigned int)(*PSI5SPUOS)["ACSTS"];
}//}}}

/// Get MSTS bit
unsigned int PSIS011_Cmn_Reg::GetMSTSBit()
{//{{{
    return (unsigned int)(*PSI5SPUOS)["MSTS"];
}//}}}

/// Config time stamp
void PSIS011_Cmn_Reg::ConfigTimestamp(std::string timestamp_name)
{//{{{
    assert ((timestamp_name == "A") || (timestamp_name == "B"));

    if (timestamp_name == "A"){
        unsigned int clear_select_gtm = (unsigned int)(*PSI5SPTCAS)["TSCACLS"];
        unsigned int enable_select_gtm = (unsigned int)(*PSI5SPTCAS)["TSCAEBS"];
        unsigned int clock_select_gtm = (unsigned int)(*PSI5SPTCAS)["TSCACKS"];
        mpPSIS011Func->ConfigTimestamp("A", (bool)clear_select_gtm, (bool)enable_select_gtm, (bool)clock_select_gtm);
    }else {// timestamp_name == "B"
        unsigned int clear_select_gtm = (unsigned int)(*PSI5SPTCBS)["TSCBCLS"];
        unsigned int enable_select_gtm = (unsigned int)(*PSI5SPTCBS)["TSCBEBS"];
        unsigned int clock_select_gtm = (unsigned int)(*PSI5SPTCBS)["TSCBCKS"];
        mpPSIS011Func->ConfigTimestamp("B", (bool)clear_select_gtm, (bool)enable_select_gtm, (bool)clock_select_gtm);
    }
}//}}}

/// Store UART data and update related status bits
void PSIS011_Cmn_Reg::StoreUARTData(unsigned int data, bool framing_error, bool parity_error)
{//{{{
    // Update UTROE bit if overrun
    bool overrun_error = false;
    if (!mWasReadUARTRxData){
        (*PSI5SUCRS)["UTROE"] = 1;
        overrun_error = true;
        if ((unsigned int)(*PSI5SUCRIE)["IEROE"] == 1){
            mpPSIS011Func->AssertInterrupt(0);
        }
    }

    // Update UTRFIN bit
    if (!(framing_error || parity_error || overrun_error)){
        (*PSI5SUCRS)["UTRFIN"] = 1;
        if ((unsigned int)(*PSI5SUCRIE)["IERFIN"] == 1){
            mpPSIS011Func->AssertInterrupt(0);
        }
        if ((unsigned int)(*PSI5SUCDRE)["DRQEURFN"] == 1){
            mpPSIS011Func->AssertDMARequestRX(7);
        }
    }else{
        (*PSI5SUCRS)["UTRFIN"] = 0;
    }

    // Set UTRFE, UTRPE bit
    if (framing_error){
        (*PSI5SUCRS)["UTRFE"] = 1;
        if ((unsigned int)(*PSI5SUCRIE)["IERFE"] == 1){
            mpPSIS011Func->AssertInterrupt(0);
        }
    }
    if (parity_error){
        (*PSI5SUCRS)["UTRPE"] = 1;
        if ((unsigned int)(*PSI5SUCRIE)["IERPE"] == 1){
            mpPSIS011Func->AssertInterrupt(0);
        }
    }

    // Update UTRFE, UTRPE bit again in priority overrun_error > framing > parity
    if (overrun_error){
        (*PSI5SUCRS)["UTRPE"] = 0;
        (*PSI5SUCRS)["UTRFE"] = 0;
    }else if (framing_error){
        (*PSI5SUCRS)["UTRPE"] = 0;
    }

    // Update UCRD reg, update variable indicating not read yet
    (*PSI5SUCRD) = data;
    mWasReadUARTRxData = false;
}//}}}

/// Get clock divider (number of cycle to transmit/receive 1 bit)
void PSIS011_Cmn_Reg::GetClockDivider(unsigned int &rx_divider, unsigned int &tx_divider)
{//{{{
    unsigned int sckprs = (unsigned int)(*PSI5SPUBPR)["SCKPRS"];
    unsigned int sckdiv = (unsigned int)(*PSI5SPUBPR)["SCKDIV"];
    unsigned int rxosmp = (unsigned int)(*PSI5SPUBPR)["RXOSMP"];

    rx_divider = (sckprs + 1) * (sckdiv + 1) * (rxosmp + 1);
    tx_divider = (sckprs + 1) * (sckdiv + 1);
}//}}}

/// Get UART Rx/Tx parity option
unsigned int PSIS011_Cmn_Reg::GetPSI5SPUPTSReg()
{//{{{
    return (unsigned int)(*PSI5SPUPTS);
}//}}}

/// Get configuration about CPU Tx command data: number of frame, Alternate command, command
void PSIS011_Cmn_Reg::GetConfigCPUTxCom(unsigned int &numfrm, sc_dt::uint64 &tx_cpu_com_data)
{//{{{
    numfrm = (unsigned int)(*PSI5SPTFNM)["TXNUM"] + 1;
    unsigned int ptfd1 = (unsigned int)(*PSI5SPTFD1);
    unsigned int ptfd2 = (unsigned int)(*PSI5SPTFD2);
    tx_cpu_com_data = ((sc_dt::uint64)ptfd2 << 32) | ((sc_dt::uint64)ptfd1);
}//}}}

/// Get data to transfer in UART mode (get UTTDT bit)
unsigned int PSIS011_Cmn_Reg::GetUTTDTBit()
{//{{{
    return (unsigned int)(*PSI5SUCTD)["UTTDT"];
}//}}}

/// Set TXSTS bit
void PSIS011_Cmn_Reg::SetTXSTSBit(unsigned int value)
{//{{{
    (*PSI5SPTFS)["TXSTS"] = value;
}//}}}

/// Get NFSET bit
unsigned int PSIS011_Cmn_Reg::GetNFSETBit()
{//{{{
    return (unsigned int)(*PSI5SPUNFST)["NFSET"];
}//}}}

/// Update UTTFIN bit for UART transmit
void PSIS011_Cmn_Reg::UpdateUTTFINBit(unsigned int value)
{//{{{
    (*PSI5SUCTS)["UTTFIN"] = value;
    if (value == 1){
        if ((unsigned int)(*PSI5SUCTIE)["IETFIN"] == 1){
            mpPSIS011Func->AssertInterrupt(1);
        }
        if ((unsigned int)(*PSI5SUCDRE)["DRQEUTFN"] == 1){
            mpPSIS011Func->AssertDMARequestTX(6);// dmatxid = 6 mean channel 7
        }
    }
}//}}}

/// Update PSI5SUCTM register for UART transmit
void PSIS011_Cmn_Reg::UpdatePSI5SUCTMReg(std::string bit_name, unsigned int value)
{//{{{
    assert ((bit_name == "UTTBBF") || (bit_name == "UTTF"));
    if (bit_name == "UTTBBF"){
        (*PSI5SUCTM)["UTTBBF"] = value;
    }else{
        (*PSI5SUCTM)["UTTF"] = value;
    }
}//}}}

/// Dump value of status registers
void PSIS011_Cmn_Reg::DumpStatusInfo()
{//{{{
    printf("Status info of common registers:\n");
    printf("    PSI5SUCRS = %08X\n", (unsigned int)(*PSI5SUCRS));
    printf("    PSI5SUCTS = %08X\n", (unsigned int)(*PSI5SUCTS));
}//}}}

/// Clear all status register when mode is changed to inactive (configuration mode)
void PSIS011_Cmn_Reg::ClearAllStatus()
{//{{{
    (*PSI5SUCRS) = 0;
    (*PSI5SPTFS) = 0;
    (*PSI5SPTFIS) = 0;
    (*PSI5SUCTM) = 0;
    (*PSI5SUCTS) = 0;
}//}}}

/// Set PSI5SPTFIS register with specified value
void PSIS011_Cmn_Reg::SetPSI5SPTFISReg(unsigned int value)
{//{{{
    (*PSI5SPTFIS) = value;
}//}}}

/// Set PSI5SPUOS.SWSTS bit
void PSIS011_Cmn_Reg::SetSWSTSBit(unsigned int value)
{//{{{
    (*PSI5SPUOS)["SWSTS"] = value;
}//}}}

/// Get loopback enable attribute
bool PSIS011_Cmn_Reg::GetLoopbackEnable()
{//{{{
    return mLoopbackEnable;
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================

/// Callback of PSI5SPUOEB register
void PSIS011_Cmn_Reg::cb_PSI5SPUOEB_OPEN(RegCBstr str)
{//{{{
    unsigned int open = (unsigned int)(*PSI5SPUOEB)["OPEN"];
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int pre_open = (unsigned int)(str.pre_data) & 0x1;

    // Cannot write 1 when ACSTS != 0 (UART mode or PSI5 mode)
    if ((open == 1) && (acsts == 1)){
        re_printf("warning", "Cannot write 1 to PSI5SPUOEB register in UART mode or PSI5 mode.\n");
        (*PSI5SPUOEB)["OPEN"] = pre_open;
    }else{
        UpdatePUOSReg();
        mpPSIS011Func->InformChangeMode(open, (unsigned int)(*PSI5SPUOMD)["OPMD"]);
    }
}//}}}

/// Callback of PSI5SPUOMD register
void PSIS011_Cmn_Reg::cb_PSI5SPUOMD_OPMD(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int pre_opmd = (unsigned int)(str.pre_data) & 0x1;

    if (acsts == 1){
        re_printf("warning", "Cannot write PSI5SPUOMD register in UART mode or PSI5 mode.\n");
        (*PSI5SPUOMD)["OPMD"] = pre_opmd;
    }else{
        UpdatePUOSReg();
        mpPSIS011Func->InformChangeMode((unsigned int)(*PSI5SPUOEB)["OPEN"], (unsigned int)(*PSI5SPUOMD)["OPMD"]);
    }
}//}}}

/// Callback of PSI5SPUNFST register
void PSIS011_Cmn_Reg::cb_PSI5SPUNFST_NFSET(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int pre_nfset = (unsigned int)(str.pre_data) & 0x1;

    if (acsts == 1){
        re_printf("warning", "Cannot write PSI5SPUNFST register in UART mode or PSI5 mode.\n");
        (*PSI5SPUNFST)["NFSET"] = pre_nfset;
    }
}//}}}

/// Callback of PSI5SPUSWR register
void PSIS011_Cmn_Reg::cb_PSI5SPUSWR_SWRST(RegCBstr str)
{//{{{
    if ((unsigned int)(*PSI5SPUSWR)["SWRST"] == 1){
        mpPSIS011Func->SoftwareReset();
    }
}//}}}

/// Callback of PSI5SPRMBC register
void PSIS011_Cmn_Reg::cb_PSI5SPRMBC_MBCLR(RegCBstr str)
{//{{{
    // Clear all Mail Box data if writing 1
    if ((unsigned int)(*PSI5SPRMBC)["MBCLR"] == 1){
        mpPSIS011Func->ClearAllMailBoxData();
    }
}//}}}

/// Callback of PSI5SPUCLB register
void PSIS011_Cmn_Reg::cb_PSI5SPUCLB_LBEN(RegCBstr str)
{//{{{
    // Write full 3 steps to enable loopback. No write wrong value, no write different address in midway
    mLoopbackEnable = false;

    switch (mPUCLBStepDone){
        case 0: // beginning
            if ((unsigned int)(*PSI5SPUCLB) == 0xA7){
                mPUCLBStepDone++;
            }else{
                (*PSI5SPUCLB) = str.pre_data;
                mPUCLBStepDone = 0;
            }
            break;
        case 1:
            if ((unsigned int)(*PSI5SPUCLB) == 0x58){
                mPUCLBStepDone++;
            }else{
                (*PSI5SPUCLB) = str.pre_data;
                mPUCLBStepDone = 0;
            }
            break;
        default://case 2:
            if ((unsigned int)(*PSI5SPUCLB) == 0x1){
                mLoopbackEnable = true;
            }else{
                (*PSI5SPUCLB) = str.pre_data;
            }
            mPUCLBStepDone = 0;
            break;
    }
    // TMKV always read as 0, LBEN = 1 when loopback enable
    (*PSI5SPUCLB) = 0;
    if (mLoopbackEnable){
        (*PSI5SPUCLB)["LBEN"] = 1;
    }
}//}}}

/// Callback of PSI5SPUPTS.UTPRTY bit
void PSIS011_Cmn_Reg::cb_PSI5SPUPTS_UTPRTY(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int pre_utprty = ((unsigned int)(str.pre_data) >> 8) & 0x3;

    if (acsts == 1){
        re_printf("warning", "Cannot write PSI5SPUPTS.UTPRTY bit in UART mode or PSI5 mode.\n");
        (*PSI5SPUPTS)["UTPRTY"] = pre_utprty;
    }
}//}}}

/// Callback of PSI5SPUPTS.URPRTY bit
void PSIS011_Cmn_Reg::cb_PSI5SPUPTS_URPRTY(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int pre_urprty = (unsigned int)(str.pre_data) & 0x3;

    if (acsts == 1){
        re_printf("warning", "Cannot write PSI5SPUPTS.URPRTY bit in UART mode or PSI5 mode.\n");
        (*PSI5SPUPTS)["URPRTY"] = pre_urprty;
    }
}//}}}

/// Callback of PSI5SPUBCE register
void PSIS011_Cmn_Reg::cb_PSI5SPUBCE_SCKEN(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int scken = (unsigned int)(*PSI5SPUBCE)["SCKEN"];
    unsigned int pre_scken = (unsigned int)(str.pre_data) & 0x1;

    if (acsts == 1){
        re_printf("warning", "Cannot write PSI5SPUBCE register in UART mode or PSI5 mode.\n");
        (*PSI5SPUBCE)["SCKEN"] = pre_scken;
        return;
    }
    
    // Inform value to PSIS011
    mpPSIS011Func->InformChangeSCKENBit(scken);
}//}}}

/// Callback of PSI5SPUBPR.RXOSMP bit
void PSIS011_Cmn_Reg::cb_PSI5SPUBPR_RXOSMP(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int rxosmp = (unsigned int)(*PSI5SPUBPR)["RXOSMP"];
    unsigned int pre_rxosmp = ((unsigned int)(str.pre_data) >> 16) & 0xF;

    if (acsts == 1){
        re_printf("warning", "Cannot write PSI5SPUBPR.RXOSMP bit in UART mode or PSI5 mode.\n");
        (*PSI5SPUBPR)["RXOSMP"] = pre_rxosmp;
        return;
    }

    // Round up if written value < 4
    if (rxosmp < 4){
        (*PSI5SPUBPR)["RXOSMP"] = 4;
    }
}//}}}

/// Callback of PSI5SPUBPR.SCKDIV bit and PSI5SPUBPR.SCKPRS bit
void PSIS011_Cmn_Reg::cb_PSI5SPUBPR_SCKPRS(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int scken = (unsigned int)(*PSI5SPUBCE)["SCKEN"];
    unsigned int pre_sckdiv = ((unsigned int)(str.pre_data) >> 8) & 0xFF;
    unsigned int pre_sckprs = (unsigned int)(str.pre_data) & 0x7F;

    if (acsts == 1){
        re_printf("warning", "Cannot write PSI5SPUBPR.SCKDIV bit and PSI5SPUBPR.SCKPRS bit in UART mode or PSI5 mode.\n");
        (*PSI5SPUBPR)["SCKDIV"] = pre_sckdiv;
        (*PSI5SPUBPR)["SCKPRS"] = pre_sckprs;
        return;
    }

    if (scken == 1){
        re_printf("warning", "Cannot write PSI5SPUBPR.SCKDIV bit and PSI5SPUBPR.SCKPRS bit when PSI5SPUBCE.SCKEN bit is 1.\n");
        (*PSI5SPUBPR)["SCKDIV"] = pre_sckdiv;
        (*PSI5SPUBPR)["SCKPRS"] = pre_sckprs;
        return;
    }
}//}}}

/// Callback of PSI5SPTPS register
void PSIS011_Cmn_Reg::cb_PSI5SPTPS_TSPRSL(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int tsprsl = (unsigned int)(*PSI5SPTPS)["TSPRSL"];
    unsigned int tsprsu = (unsigned int)(*PSI5SPTPS)["TSPRSU"];
    unsigned int pre_tsprsu = ((unsigned int)(str.pre_data) >> 16) & 0x3FF;
    unsigned int pre_tsprsl = (unsigned int)(str.pre_data) & 0x7F;

    if (acsts == 1){
        re_printf("warning", "Cannot write PSI5SPTPS register in UART mode or PSI5 mode.\n");
        (*PSI5SPTPS)["TSPRSU"] = pre_tsprsu;
        (*PSI5SPTPS)["TSPRSL"] = pre_tsprsl;
        return;
    }

    // Check setting TSPRSL for 1us
    unsigned int expected_tsprsl = (unsigned int)(mpPSIS011Func->GetPCLKFreq()/1000000);// convert Hz to MHz
    if (expected_tsprsl != tsprsl){
        re_printf("warning", "Wrong setting for PSI5SPTPS.TSPRSL bit (= %d) makes malfunction. It should be %d (because PCLK = %dMHz).\n", tsprsl, expected_tsprsl, expected_tsprsl);
    }

    // Check max = 1ms (=1000us) for TSPRSU
    if (tsprsu > 1000){
        re_printf("warning", "Wrong setting for PSI5SPTPS.TSPRSU bit (= %d > 1000). Value 1000 is written to this bit (due to max 1ms).\n", tsprsu);
        (*PSI5SPTPS)["TSPRSU"] = 1000;
    }

    // Set Timestamp Inner cycle (PCLK) -> generate inner clock for timestamp counting up
    mpPSIS011Func->SetTSInnerCycle(tsprsu*tsprsl);
}//}}}

/// Callback of PSI5SPTCAS.TSCACLS bit
void PSIS011_Cmn_Reg::cb_PSI5SPTCAS_TSCACLS(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int pre_tscacls = ((unsigned int)(str.pre_data) >> 16) & 0x1;

    if ((acsts == 1) && (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPTCAS.TSCACLS bit in UART mode.\n");
        (*PSI5SPTCAS)["TSCACLS"] = pre_tscacls;
    }else{
        ConfigTimestamp("A");
    }
}//}}}

/// Callback of PSI5SPTCAS.TSCAEBS bit
void PSIS011_Cmn_Reg::cb_PSI5SPTCAS_TSCAEBS(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int pre_tscaebs = ((unsigned int)(str.pre_data) >> 8) & 0x1;

    if ((acsts == 1) && (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPTCAS.TSCAEBS bit in UART mode.\n");
        (*PSI5SPTCAS)["TSCAEBS"] = pre_tscaebs;
    }else{
        ConfigTimestamp("A");
    }
}//}}}

/// Callback of PSI5SPTCAS.TSCACKS bit
void PSIS011_Cmn_Reg::cb_PSI5SPTCAS_TSCACKS(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int pre_tscacks = (unsigned int)(str.pre_data) & 0x1;

    if ((acsts == 1) && (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPTCAS.TSCACKS bit in UART mode.\n");
        (*PSI5SPTCAS)["TSCACKS"] = pre_tscacks;
    }else{
        ConfigTimestamp("A");
    }
}//}}}

/// Callback of PSI5SPTCBS.TSCBCLS bit
void PSIS011_Cmn_Reg::cb_PSI5SPTCBS_TSCBCLS(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int pre_tscbcls = ((unsigned int)(str.pre_data) >> 16) & 0x1;

    if ((acsts == 1) && (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPTCBS.TSCBCLS bit in UART mode.\n");
        (*PSI5SPTCBS)["TSCBCLS"] = pre_tscbcls;
    }else{
        ConfigTimestamp("B");
    }
}//}}}

/// Callback of PSI5SPTCBS.TSCBEBS bit
void PSIS011_Cmn_Reg::cb_PSI5SPTCBS_TSCBEBS(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int pre_tscbebs = ((unsigned int)(str.pre_data) >> 8) & 0x1;

    if ((acsts == 1) && (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPTCBS.TSCBEBS bit in UART mode.\n");
        (*PSI5SPTCBS)["TSCBEBS"] = pre_tscbebs;
    }else{
        ConfigTimestamp("B");
    }
}//}}}

/// Callback of PSI5SPTCBS.TSCBCKS bit
void PSIS011_Cmn_Reg::cb_PSI5SPTCBS_TSCBCKS(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int pre_tscbcks = (unsigned int)(str.pre_data) & 0x1;

    if ((acsts == 1) && (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPTCBS.TSCBCKS bit in UART mode.\n");
        (*PSI5SPTCBS)["TSCBCKS"] = pre_tscbcks;
    }else{
        ConfigTimestamp("B");
    }
}//}}}

/// Callback of PSI5SPTCAE register
void PSIS011_Cmn_Reg::cb_PSI5SPTCAE_TSCAEB(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int pre_tscaeb = (unsigned int)(str.pre_data) & 0x1;

    if ((acsts == 1) && (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPTCAE register in UART mode.\n");
        (*PSI5SPTCAE)["TSCAEB"] = pre_tscaeb;
    }else{
        mpPSIS011Func->SetTSInnerEnable("A", ((bool)(*PSI5SPATCE)["ATSCEB"]) || ((bool)(*PSI5SPTCAE)["TSCAEB"]));
    }
}//}}}

/// Callback of PSI5SPTCAC register
void PSIS011_Cmn_Reg::cb_PSI5SPTCAC_TSCACLR(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int tscacls = (unsigned int)(*PSI5SPTCAS)["TSCACLS"];
    unsigned int tscaclr = (unsigned int)(*PSI5SPTCAC)["TSCACLR"];

    if ((acsts == 1) && (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPTCAC register in UART mode.\n");
    }else{
        if ((tscaclr == 1) && (tscacls == 0)){
            mpPSIS011Func->ClearTimestamp("A");
        }
    }

    // Always read as 0
    (*PSI5SPTCAC)["TSCACLR"] = 0;
}//}}}

/// Callback of PSI5SPTCBE register
void PSIS011_Cmn_Reg::cb_PSI5SPTCBE_TSCBEB(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int pre_tscbeb = (unsigned int)(str.pre_data) & 0x1;

    if ((acsts == 1) && (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPTCBE register in UART mode.\n");
        (*PSI5SPTCBE)["TSCBEB"] = pre_tscbeb;
    }else{
        mpPSIS011Func->SetTSInnerEnable("B", ((bool)(*PSI5SPATCE)["ATSCEB"]) || ((bool)(*PSI5SPTCBE)["TSCBEB"]));
    }
}//}}}

/// Callback of PSI5SPTCBC register
void PSIS011_Cmn_Reg::cb_PSI5SPTCBC_TSCBCLR(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int tscbcls = (unsigned int)(*PSI5SPTCBS)["TSCBCLS"];
    unsigned int tscbclr = (unsigned int)(*PSI5SPTCBC)["TSCBCLR"];

    if ((acsts == 1) && (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPTCBC register in UART mode.\n");
    }else{
        if ((tscbclr == 1) && (tscbcls == 0)){
            mpPSIS011Func->ClearTimestamp("B");
        }
    }

    // Always read as 0
    (*PSI5SPTCBC)["TSCBCLR"] = 0;
}//}}}

/// Callback of PSI5SPATCE register
void PSIS011_Cmn_Reg::cb_PSI5SPATCE_ATSCEB(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int pre_atsceb = (unsigned int)(str.pre_data) & 0x1;

    if ((acsts == 1) && (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPATCE register in UART mode.\n");
        (*PSI5SPATCE)["ATSCEB"] = pre_atsceb;
    }else{
        mpPSIS011Func->SetTSInnerEnable("A", ((bool)(*PSI5SPATCE)["ATSCEB"]) || ((bool)(*PSI5SPTCAE)["TSCAEB"]));
        mpPSIS011Func->SetTSInnerEnable("B", ((bool)(*PSI5SPATCE)["ATSCEB"]) || ((bool)(*PSI5SPTCBE)["TSCBEB"]));
    }
}//}}}

/// Callback of PSI5SPATCC register
void PSIS011_Cmn_Reg::cb_PSI5SPATCC_ATSCCLR(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int tscacls = (unsigned int)(*PSI5SPTCAS)["TSCACLS"];
    unsigned int tscbcls = (unsigned int)(*PSI5SPTCBS)["TSCBCLS"];
    unsigned int atscclr = (unsigned int)(*PSI5SPATCC)["ATSCCLR"];

    if ((acsts == 1) && (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPATCC register in UART mode.\n");
    }else{
        if ((atscclr == 1) && (tscacls == 0)){
            mpPSIS011Func->ClearTimestamp("A");
        }
        if ((atscclr == 1) && (tscbcls == 0)){
            mpPSIS011Func->ClearTimestamp("B");
        }
    }

    // Always read as 0
    (*PSI5SPATCC)["ATSCCLR"] = 0;
}//}}}

/// Callback of PSI5SUCRIE register
void PSIS011_Cmn_Reg::cb_PSI5SUCRIE_IERPE(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];

    if ((acsts == 1) && (msts == 1)) {
        re_printf("warning", "Cannot write PSI5SUCRIE register in PSI5 mode.\n");
        (*PSI5SUCRIE) = (unsigned int)(str.pre_data);
    }
}//}}}

/// Callback of PSI5SUCTIE register
void PSIS011_Cmn_Reg::cb_PSI5SUCTIE_IETOWE(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];

    if ((acsts == 1) && (msts == 1)) {
        re_printf("warning", "Cannot write PSI5SUCTIE register in PSI5 mode.\n");
        (*PSI5SUCTIE) = (unsigned int)(str.pre_data);
    }
}//}}}

/// Callback of PSI5SUCDRE register
void PSIS011_Cmn_Reg::cb_PSI5SUCDRE_DRQEURFN(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];

    if ((acsts == 1) && (msts == 1)) {
        re_printf("warning", "Cannot write PSI5SUCDRE register in PSI5 mode.\n");
        (*PSI5SUCDRE) = (unsigned int)(str.pre_data);
    }
}//}}}

/// Callback of PSI5SUCRD register (callback read)
void PSIS011_Cmn_Reg::cb_PSI5SUCRD_UTRDT(RegCBstr str)
{//{{{
    mWasReadUARTRxData = true;
}//}}}

/// Callback of PSI5SUCRSC register
void PSIS011_Cmn_Reg::cb_PSI5SUCRSC_UTRPECL(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];

    if ((acsts == 0) || (msts == 1)) {
        re_printf("warning", "Cannot write PSI5SUCRSC register in PSI5 mode or Configuration mode.\n");
        (*PSI5SUCRSC) = 0;
        return;
    }

    // Clear according bits
    if ((unsigned int)(*PSI5SUCRSC)["UTRFINCL"] == 1){
        (*PSI5SUCRS)["UTRFIN"] = 0;
    }
    if ((unsigned int)(*PSI5SUCRSC)["UTROECL"] == 1){
        (*PSI5SUCRS)["UTROE"] = 0;
    }
    if ((unsigned int)(*PSI5SUCRSC)["UTRFECL"] == 1){
        (*PSI5SUCRS)["UTRFE"] = 0;
    }
    if ((unsigned int)(*PSI5SUCRSC)["UTRPECL"] == 1){
        (*PSI5SUCRS)["UTRPE"] = 0;
    }

    // Always read as 0
    (*PSI5SUCRSC) = 0;
}//}}}

/// Callback of PSI5SPTFST register
void PSIS011_Cmn_Reg::cb_PSI5SPTFST_TXST(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];

    if ((acsts == 0) || (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPTFST register in UART mode or Configuration mode.\n");
    }else{
        if ((unsigned int)(*PSI5SPTFST)["TXST"] == 1){
            (*PSI5SPTFS)["TXSTS"] = 1;
            // Send request to arbiter (for Tx command data)
            mpPSIS011Func->NotifySendReqTxToArbiter(7);// Priority of Tx command data is 7 (lower than ch1-7 (reqid 0-6))
        }
    }

    // Always read as 0
    (*PSI5SPTFST) = 0;
}//}}}

/// Callback of PSI5SPTFNM register
void PSIS011_Cmn_Reg::cb_PSI5SPTFNM_TXNUM(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int pre_txnum = (unsigned int)(str.pre_data) & 0x7;

    if ((acsts == 1) && (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPTFNM register in UART mode.\n");
        (*PSI5SPTFNM)["TXNUM"] = pre_txnum;
    }
}//}}}

/// Callback of PSI5SPTFD1.TDT4 bit
void PSIS011_Cmn_Reg::cb_PSI5SPTFD1_TDT4(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int txsts = (unsigned int)(*PSI5SPTFS)["TXSTS"];
    unsigned int pre_tdt4 = ((unsigned int)(str.pre_data) >> 24) & 0xFF;

    if ((acsts == 1) && (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPTFD1 register in UART mode.\n");
        (*PSI5SPTFD1)["TDT4"] = pre_tdt4;
    }
    if (txsts == 1){
        re_printf("warning", "Cannot write PSI5SPTFD1 register when PSI5SPTFS.TXSTS is 1.\n");
        (*PSI5SPTFD1)["TDT4"] = pre_tdt4;
    }
}//}}}

/// Callback of PSI5SPTFD1.TDT3 bit
void PSIS011_Cmn_Reg::cb_PSI5SPTFD1_TDT3(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int txsts = (unsigned int)(*PSI5SPTFS)["TXSTS"];
    unsigned int pre_tdt3 = ((unsigned int)(str.pre_data) >> 16) & 0xFF;

    if ((acsts == 1) && (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPTFD1 register in UART mode.\n");
        (*PSI5SPTFD1)["TDT3"] = pre_tdt3;
    }
    if (txsts == 1){
        re_printf("warning", "Cannot write PSI5SPTFD1 register when PSI5SPTFS.TXSTS is 1.\n");
        (*PSI5SPTFD1)["TDT3"] = pre_tdt3;
    }
}//}}}

/// Callback of PSI5SPTFD1.TDT2 bit
void PSIS011_Cmn_Reg::cb_PSI5SPTFD1_TDT2(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int txsts = (unsigned int)(*PSI5SPTFS)["TXSTS"];
    unsigned int pre_tdt2 = ((unsigned int)(str.pre_data) >> 8) & 0xFF;

    if ((acsts == 1) && (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPTFD1 register in UART mode.\n");
        (*PSI5SPTFD1)["TDT2"] = pre_tdt2;
    }
    if (txsts == 1){
        re_printf("warning", "Cannot write PSI5SPTFD1 register when PSI5SPTFS.TXSTS is 1.\n");
        (*PSI5SPTFD1)["TDT2"] = pre_tdt2;
    }
}//}}}

/// Callback of PSI5SPTFD1.TDT1 bit
void PSIS011_Cmn_Reg::cb_PSI5SPTFD1_TDT1(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int txsts = (unsigned int)(*PSI5SPTFS)["TXSTS"];
    unsigned int pre_tdt1 = (unsigned int)(str.pre_data) & 0xFF;

    if ((acsts == 1) && (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPTFD1 register in UART mode.\n");
        (*PSI5SPTFD1)["TDT1"] = pre_tdt1;
    }
    if (txsts == 1){
        re_printf("warning", "Cannot write PSI5SPTFD1 register when PSI5SPTFS.TXSTS is 1.\n");
        (*PSI5SPTFD1)["TDT1"] = pre_tdt1;
    }
}//}}}

/// Callback of PSI5SPTFD2.TDT8 bit
void PSIS011_Cmn_Reg::cb_PSI5SPTFD2_TDT8(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int txsts = (unsigned int)(*PSI5SPTFS)["TXSTS"];
    unsigned int pre_tdt8 = ((unsigned int)(str.pre_data) >> 24) & 0xFF;

    if ((acsts == 1) && (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPTFD2 register in UART mode.\n");
        (*PSI5SPTFD2)["TDT8"] = pre_tdt8;
    }
    if (txsts == 1){
        re_printf("warning", "Cannot write PSI5SPTFD2 register when PSI5SPTFS.TXSTS is 1.\n");
        (*PSI5SPTFD2)["TDT8"] = pre_tdt8;
    }
}//}}}

/// Callback of PSI5SPTFD2.TDT7 bit
void PSIS011_Cmn_Reg::cb_PSI5SPTFD2_TDT7(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int txsts = (unsigned int)(*PSI5SPTFS)["TXSTS"];
    unsigned int pre_tdt7 = ((unsigned int)(str.pre_data) >> 16) & 0xFF;

    if ((acsts == 1) && (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPTFD2 register in UART mode.\n");
        (*PSI5SPTFD2)["TDT7"] = pre_tdt7;
    }
    if (txsts == 1){
        re_printf("warning", "Cannot write PSI5SPTFD2 register when PSI5SPTFS.TXSTS is 1.\n");
        (*PSI5SPTFD2)["TDT7"] = pre_tdt7;
    }
}//}}}

/// Callback of PSI5SPTFD2.TDT6 bit
void PSIS011_Cmn_Reg::cb_PSI5SPTFD2_TDT6(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int txsts = (unsigned int)(*PSI5SPTFS)["TXSTS"];
    unsigned int pre_tdt6 = ((unsigned int)(str.pre_data) >> 8) & 0xFF;

    if ((acsts == 1) && (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPTFD2 register in UART mode.\n");
        (*PSI5SPTFD2)["TDT6"] = pre_tdt6;
    }
    if (txsts == 1){
        re_printf("warning", "Cannot write PSI5SPTFD2 register when PSI5SPTFS.TXSTS is 1.\n");
        (*PSI5SPTFD2)["TDT6"] = pre_tdt6;
    }
}//}}}

/// Callback of PSI5SPTFD2.TDT5 bit
void PSIS011_Cmn_Reg::cb_PSI5SPTFD2_TDT5(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int txsts = (unsigned int)(*PSI5SPTFS)["TXSTS"];
    unsigned int pre_tdt5 = (unsigned int)(str.pre_data) & 0xFF;

    if ((acsts == 1) && (msts == 0)) {
        re_printf("warning", "Cannot write PSI5SPTFD2 register in UART mode.\n");
        (*PSI5SPTFD2)["TDT5"] = pre_tdt5;
    }
    if (txsts == 1){
        re_printf("warning", "Cannot write PSI5SPTFD2 register when PSI5SPTFS.TXSTS is 1.\n");
        (*PSI5SPTFD2)["TDT5"] = pre_tdt5;
    }
}//}}}

/// Callback of PSI5SUCTD register
void PSIS011_Cmn_Reg::cb_PSI5SUCTD_UTTDT(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];
    unsigned int uttbbf = (unsigned int)(*PSI5SUCTM)["UTTBBF"];
    unsigned int pre_uttdt = (unsigned int)(str.pre_data) & 0xFF;

    // Cannot write
    if ((acsts == 1) && (msts == 1)) {
        re_printf("warning", "Cannot write PSI5SUCTD register in PSI5 mode.\n");
        (*PSI5SUCTD) = str.pre_data;
    // Can write
    }else if ((acsts == 1) && (msts == 0)){
        if (uttbbf == 1){
            (*PSI5SUCTS)["UTTOWE"] = 1;
            (*PSI5SUCTD) = str.pre_data;
            if ((unsigned int)(*PSI5SUCTIE)["IETOWE"] == 1){
                mpPSIS011Func->AssertInterrupt(1);// interrupt on channel 1
            }
        }else{
            mpPSIS011Func->NotifyTransUART();
            (*PSI5SUCTM)["UTTBBF"] = 1;
        }
    }
}//}}}

/// Callback of PSI5SUCTSC register
void PSIS011_Cmn_Reg::cb_PSI5SUCTSC_UTTOWECL(RegCBstr str)
{//{{{
    unsigned int acsts = (unsigned int)(*PSI5SPUOS)["ACSTS"];
    unsigned int msts = (unsigned int)(*PSI5SPUOS)["MSTS"];

    if ((acsts == 0) || (msts == 1)) {
        re_printf("warning", "Cannot write PSI5SUCTSC register in PSI5 mode or Configuration mode.\n");
        (*PSI5SUCTSC) = 0;
        return;
    }

    // Clear according bits
    if ((unsigned int)(*PSI5SUCTSC)["UTTFINCL"] == 1){
        (*PSI5SUCTS)["UTTFIN"] = 0;
    }
    if ((unsigned int)(*PSI5SUCTSC)["UTTOWECL"] == 1){
        (*PSI5SUCTS)["UTTOWE"] = 0;
    }

    // Always read as 0
    (*PSI5SUCTSC) = 0;
}//}}}

// vim600: set foldmethod=marker :
