// ---------------------------------------------------------------------
// $Id: DFE0051_Func.cpp,v 1.3 2017/11/15 08:48:22 chanle Exp $
//
// Copyright(c) 2016-2017 Renesas Electronics Corporation
// Copyright(c) 2016-2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "DFE0051_Func.h"
#include "DFE0051_AgentController.h"

//========================================================
//============ Constructor and Destructor=================
//========================================================
Cdfe0051_func::Cdfe0051_func (std::string name, unsigned int num_channel, Cdfe0051_agent_controller* DFE0051AgentController):
                Cdfe0051_regif(name, 32)
                ,BusSlaveFuncIf()
{//{{{
    assert(DFE0051AgentController != NULL);
    mDFE0051AgentController = DFE0051AgentController;
    mDFE0051AgentController->CommandInit(name);
    Cdfe0051_regif::set_instance_name(name);
    mNumChannel = num_channel;
    mModuleName = name;
    // Initialize
    Initialize();
    memset(mDMEM, 0, 0x800);
    memset(mCMEM, 0, 0x400);
}//}}}

Cdfe0051_func::~Cdfe0051_func()
{//{{{
}//}}}

void Cdfe0051_func::Initialize(void)
{//{{{
    mCurErrStatus = false;
    mPreErrStatus = false;
    for (unsigned int channel = 0; channel < mNumChannel; channel++){
        mWasReadHOLCH[channel] = false;
        mWasReadHOHCH[channel] = false;
        mStartDataPtr[channel] = 0;// Just mean offset, not = channel*emCHDMEMSize;
        mCurDataPtr[channel] = mStartDataPtr[channel];
        mPrePHCH[channel] = 0;
        mPrePhase[channel] = 1;
        mPHCHPureVal[channel] = 0x80000000;
    }
    mPrePH20 = 0;
    mPrePH30 = 0;
    mPH20PureVal = 0x80000000;
    mPH30PureVal = 0x80000000;
    memset(mDMEM, 0, 0x800);
    memset(mCMEM, 0, 0x400);
}//}}}

void Cdfe0051_func::EnableReset(const bool is_active)
{//{{{
    if (is_active){
        Initialize();
    }
    Cdfe0051_regif::EnableReset(is_active);
}//}}}

void Cdfe0051_func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int full_addr = (unsigned int)trans.get_address();
    unsigned int addr = (unsigned int)trans.get_address() & 0x3FFF;
    unsigned int size = (unsigned int)trans.get_data_length();
    memset(trans.get_data_ptr(), 0, size);
    // Read register - note: process for non-existed register (num channel < 12)
    if ((0 <= addr) && (addr < emStartOffsetCMEM)){
        // Check non-existed
        if ((((4*mNumChannel) <= addr) && (addr < 0x40)) // DFEjCTLACH
        || (((0x40+4*mNumChannel) <= addr) && (addr < 0x80))   // DFEjCTLBCH
        || (((0x80+4*mNumChannel) <= addr) && (addr < 0xC0))   // DFEjDOCH
        || (((0xC0+4*mNumChannel) <= addr) && (addr < 0x100))  // DFEjPHCH
        || (((0x100+4*mNumChannel) <= addr) && (addr < 0x140)) // DFEjPHINDCH
        || (((0x140+4*mNumChannel) <= addr) && (addr < 0x180)) // DFEjSTCH
        || (((0x180+4*mNumChannel) <= addr) && (addr < 0x1C0)) // DFEjCLRSTCH
        || (((0x1C0+4*mNumChannel) <= addr) && (addr < 0x200)) // DFEjERMCH
        || (((0x200+4*mNumChannel) <= addr) && (addr < 0x240)) // DFEjTRGCH
        || (((0x240+4*mNumChannel) <= addr) && (addr < 0x280)) // DFEjTRHCH
        || (((0x320+4*mNumChannel) <= addr) && (addr < 0x360)) // DFEjHOHCH and DFEjHOLCH
        || (((0x360+2*mNumChannel) <= addr) && (addr < 0x380)) // DFEjHOLMCH
        || (((0x380+4*mNumChannel) <= addr) && (addr < 0x3C0)) // DFEjGAINCH
        || (((0x3C0+4*mNumChannel) <= addr) && (addr < 0x400))){ // DFEjTMTRGCH
            re_printf("warning", "Should not read from the reservation area 0x%08X.\n", full_addr);
        // Normal
        }else{
            if (!debug) {
                reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
            } else {
                reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
            }
        }
    // Read CMEM - support read all size 1/2/4
    }else if ((emStartOffsetCMEM <= addr) && (addr < (emStartOffsetCMEM + mNumChannel*emCHCMEMSize))){
        if (CheckValidAccessMem(emCMEMType, addr, size, false, debug)){
            memcpy(trans.get_data_ptr(), mCMEM + addr - emStartOffsetCMEM, size);
        }else{
            re_printf("warning", "Cannot read from address 0x%08X in Coefficient memory due to invalid access size %d byte(s) or invalid address.\n", full_addr, size);
        }
    // Read DMEM - support read all size 1/2/4
    }else if ((emStartOffsetDMEM <= addr) && (addr < (emStartOffsetDMEM + mNumChannel*emCHDMEMSize))){
        if (CheckValidAccessMem(emDMEMType, addr, size, false, debug)){
            memcpy(trans.get_data_ptr(), mDMEM + addr - emStartOffsetDMEM, size);
        }else{
            re_printf("warning", "Cannot read from address 0x%08X in Data memory due to invalid access size %d byte(s) or invalid address.\n", full_addr, size);
        }
    }else{
        re_printf("warning", "Should not read from the reservation area 0x%08X.\n", full_addr);
    }
}//}}}

void Cdfe0051_func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int full_addr = (unsigned int)trans.get_address();
    unsigned int addr = (unsigned int)trans.get_address() & 0x3FFF;
    unsigned int size = (unsigned int)trans.get_data_length();
    // Write register - note: process for non-existed register (num channel < 12)
    if ((0 <= addr) && (addr < emStartOffsetCMEM)){
        // Check non-existed
        if ((((4*mNumChannel) <= addr) && (addr < 0x40)) // DFEjCTLACH
        || (((0x40+4*mNumChannel) <= addr) && (addr < 0x80))   // DFEjCTLBCH
        || (((0x80+4*mNumChannel) <= addr) && (addr < 0xC0))   // DFEjDOCH
        || (((0xC0+4*mNumChannel) <= addr) && (addr < 0x100))  // DFEjPHCH
        || (((0x100+4*mNumChannel) <= addr) && (addr < 0x140)) // DFEjPHINDCH
        || (((0x140+4*mNumChannel) <= addr) && (addr < 0x180)) // DFEjSTCH
        || (((0x180+4*mNumChannel) <= addr) && (addr < 0x1C0)) // DFEjCLRSTCH
        || (((0x1C0+4*mNumChannel) <= addr) && (addr < 0x200)) // DFEjERMCH
        || (((0x200+4*mNumChannel) <= addr) && (addr < 0x240)) // DFEjTRGCH
        || (((0x240+4*mNumChannel) <= addr) && (addr < 0x280)) // DFEjTRHCH
        || (((0x320+4*mNumChannel) <= addr) && (addr < 0x360)) // DFEjHOHCH and DFEjHOLCH
        || (((0x360+2*mNumChannel) <= addr) && (addr < 0x380)) // DFEjHOLMCH
        || (((0x380+4*mNumChannel) <= addr) && (addr < 0x3C0)) // DFEjGAINCH
        || (((0x3C0+4*mNumChannel) <= addr) && (addr < 0x400))){ // DFEjTMTRGCH
            re_printf("warning", "Should not write to the reservation area 0x%08X.\n", full_addr);
        // Normal
        }else{
            if (!debug) {
                reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
            } else {
                reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
            }
        }
    // Write CMEM - support read all size 1/2/4
    }else if ((emStartOffsetCMEM <= addr) && (addr < (emStartOffsetCMEM + mNumChannel*emCHCMEMSize))){
        if (CheckValidAccessMem(emCMEMType, addr, size, true, debug)){
            memcpy(mCMEM + addr - emStartOffsetCMEM, trans.get_data_ptr(), size);
        }else{
            re_printf("warning", "Cannot write to address 0x%08X in Coefficient memory due to invalid access size %d byte(s) or invalid address.\n", full_addr, size);
        }
    // Write DMEM - support read all size 1/2/4
    }else if ((emStartOffsetDMEM <= addr) && (addr < (emStartOffsetDMEM + mNumChannel*emCHDMEMSize))){
        if (CheckValidAccessMem(emDMEMType, addr, size, true, debug)){
            memcpy(mDMEM + addr - emStartOffsetDMEM, trans.get_data_ptr(), size);
        }else{
            re_printf("warning", "Cannot write to address 0x%08X in Data memory due to invalid access size %d byte(s) or invalid address.\n", full_addr, size);
        }
    }else{
        re_printf("warning", "Should not write to the reservation area 0x%08X.\n", full_addr);
    }
}//}}}

void Cdfe0051_func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cdfe0051_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Catu5_timera_regif class
            msg_return = Cdfe0051_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

bool Cdfe0051_func::CheckValidAccessMem(eMEMInfo mem_type, unsigned int addr, unsigned int size, bool is_wr, bool debug)
{//{{{
    bool ret = false;
    sc_assert((mem_type == emCMEMType) || (mem_type == emDMEMType));
    if (mem_type == emCMEMType){
        // Check validity of Writing Coefficient memory
        if (is_wr){
            if (((size == 4) && (addr%size == 0)) || debug){
                ret = true;
            }
        // Check validity of Writing Coefficient memory
        }else{
            if ((((size == 1) || (size == 2) || (size == 4)) && (addr%size == 0)) || debug){
                ret = true;
            }
        }
    }else{
        // Check validity of Writing Data memory
        if (is_wr){
            if ((((size == 4) || (size == 2)) && (addr%size == 0)) || debug){
                ret = true;
            }
        // Check validity of Writing Data memory
        }else{
            if ((((size == 1) || (size == 2) || (size == 4)) && (addr%size == 0)) || debug){
                ret = true;
            }
        }
    }
    return ret;
}//}}}

//========================================================
//============ Dispatch circuit ==========================
//========================================================
unsigned int Cdfe0051_func::GetENBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjCTLACHn[channel])["EN"];
}//}}}

void Cdfe0051_func::SetTRHCHReg(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjTRHCHn[channel]) = value;
}//}}}

void Cdfe0051_func::SetFITSBit(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjTRHCHn[channel])["FITS"] = value;
}//}}}

void Cdfe0051_func::SetMETSBit(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjTRHCHn[channel])["METS"] = value;
}//}}}

void Cdfe0051_func::SetMITSBit(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjTRHCHn[channel])["MITS"] = value;
}//}}}

void Cdfe0051_func::SetPETSBit(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjTRHCHn[channel])["PETS"] = value;
}//}}}

void Cdfe0051_func::SetPITSBit(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjTRHCHn[channel])["PITS"] = value;
}//}}}

void Cdfe0051_func::SetPMETSBit(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjTRHCHn[channel])["PMETS"] = value;
}//}}}

void Cdfe0051_func::SetPMITSBit(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjTRHCHn[channel])["PMITS"] = value;
}//}}}

void Cdfe0051_func::SetVALIDBit(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjSTCHn[channel])["VALID"] = value;
    UpdateDFEjST();
}//}}}

unsigned int Cdfe0051_func::GetVALIDBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjSTCHn[channel])["VALID"];
}//}}}

void Cdfe0051_func::SetDIOWBit(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjSTCHn[channel])["DIOW"] = value;
    UpdateErrorStatus();
    UpdateDFEjST();
}//}}}

void Cdfe0051_func::SetDOOWBit(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjSTCHn[channel])["DOOW"] = value;
    UpdateErrorStatus();
    UpdateDFEjST();
}//}}}

unsigned int Cdfe0051_func::GetSWInputTag()
{//{{{
    return (unsigned int)(*DFEjDI)["TAG"];
}//}}}

unsigned int Cdfe0051_func::GetSWInputData()
{//{{{
    return (unsigned int)(*DFEjDI)["DI"];
}//}}}

bool Cdfe0051_func::CheckMatchTag(unsigned int in_tag, unsigned int channel_id)
{//{{{
    unsigned int channel_tag = (unsigned int)((*DFEjCTLACHn[channel_id])["TAG"]);
    return (channel_tag == in_tag);
}//}}}

//========================================================
//============ Filter circuit ============================
//========================================================

// Get Coefficient for FIR 8, 16, 24, 32, 64
unsigned short Cdfe0051_func::GetCoefficient(unsigned int channel_id, unsigned int index)
{//{{{
    unsigned short ret = 0;
    memcpy(&ret, mCMEM + emCHCMEMSize*channel_id + index*2, 2);
    return ret;
}//}}}

bool Cdfe0051_func::SetCoeffMem(unsigned int offset, unsigned int value)
{//{{{
    bool ret = false;
    if ((offset + 4) <= (mNumChannel*0x40)){
        memcpy(mCMEM + offset, &value, 4);
        ret = true;
    }
    return ret;
}//}}}

bool Cdfe0051_func::GetCoeffMem(unsigned int offset, unsigned int& data)
{//{{{
    bool ret = false;
    if ((offset + 4) <= (mNumChannel*0x40)){
        memcpy(&data, mCMEM + offset, 4);
        ret = true;
    }
    return ret;
}//}}}

bool Cdfe0051_func::SetDataMem(unsigned int offset, unsigned int value)
{//{{{
    bool ret = false;
    if ((offset + 4) <= (mNumChannel*0x80)){
        memcpy(mDMEM + offset, &value, 4);
        ret = true;
    }
    return ret;
}//}}}

bool Cdfe0051_func::GetDataMem(unsigned int offset, unsigned int& data)
{//{{{ 
    bool ret = false;
    if ((offset + 4) <= (mNumChannel*0x80)){
        memcpy(&data, mDMEM + offset, 4);
        ret = true;
    }
    return ret;
}//}}}

unsigned short Cdfe0051_func::GetFIRBuffer(unsigned int channel_id, unsigned int num_taps)
{//{{{
    unsigned short ret = 0;
    mCurDataPtr[channel_id] += 2;// Increase first then get the data, due to store xn after finish
    if (mCurDataPtr[channel_id] >= num_taps*2){
        mCurDataPtr[channel_id] = 0;
    }
    memcpy(&ret, mDMEM + emCHDMEMSize*channel_id + mCurDataPtr[channel_id], 2);
    return ret;
}//}}}

unsigned int Cdfe0051_func::GetIIRCirBuffer(unsigned int channel_id, unsigned int index)
{//{{{
    unsigned int ret = 0;
    memcpy(&ret, mDMEM + emCHDMEMSize*channel_id + mStartDataPtr[channel_id] + index*4, 4);
    return ret;
}//}}}

void Cdfe0051_func::CopyInitBufToCirBuf(unsigned int channel)
{//{{{
    memcpy(mDMEM + emCHDMEMSize*channel, mDMEM + emCHDMEMSize*channel + emBaseOffsetIniBuf, emIIRCHInitMemSize);
    mStartDataPtr[channel] = 0;
}//}}}

unsigned int Cdfe0051_func::GetGAIN2Bit(unsigned int channel_id)
{//{{{
    return (unsigned int)(*DFEjGAINCHn[channel_id])["GAIN2"];
}//}}}

unsigned int Cdfe0051_func::GetGAIN3Bit(unsigned int channel_id)
{//{{{
    return (unsigned int)(*DFEjGAINCHn[channel_id])["GAIN3"];
}//}}}

// Shift IIR buffer (circular) for 1 stage
void Cdfe0051_func::UpdateIIRBufferOneStage(unsigned int channel_id, unsigned int input)
{//{{{
    // Move back mStartDataPtr
    if (mStartDataPtr[channel_id] < 4){
        mStartDataPtr[channel_id] = mStartDataPtr[channel_id] + emIIRCHCirMemSize - 4;
    }else{
        mStartDataPtr[channel_id] -= 4;
    }
    // Store data to next data pointer
    memcpy(mDMEM + emCHDMEMSize*channel_id + mStartDataPtr[channel_id], &input, 4);
}//}}}

// Shift IIR buffer (circular) for 2 stage
void Cdfe0051_func::UpdateIIRBufferTwoStage(unsigned int channel, unsigned int input1, unsigned int input2)
{//{{{
    // Get w11, w21
    unsigned int w11 = 0;
    unsigned int w21 = 0;
    memcpy(&w11, mDMEM + emCHDMEMSize*channel + mStartDataPtr[channel], 4);
    memcpy(&w21, mDMEM + emCHDMEMSize*channel + ((mStartDataPtr[channel] + 8)% emIIRCHCirMemSize), 4);

    // Move back mStartDataPtr
    if (mStartDataPtr[channel] < 16){
        mStartDataPtr[channel] = mStartDataPtr[channel] + emIIRCHCirMemSize - 16;
    }else{
        mStartDataPtr[channel] -= 16;
    }

    // Store data to next data pointer
    memcpy(mDMEM + emCHDMEMSize*channel + mStartDataPtr[channel], &input1, 4);
    memcpy(mDMEM + emCHDMEMSize*channel + ((mStartDataPtr[channel]+4)%emIIRCHCirMemSize), &w11, 4);
    memcpy(mDMEM + emCHDMEMSize*channel + ((mStartDataPtr[channel]+8)%emIIRCHCirMemSize), &input2, 4);
    memcpy(mDMEM + emCHDMEMSize*channel + ((mStartDataPtr[channel]+0xC)%emIIRCHCirMemSize), &w21, 4);
}//}}}

// Shift IIR buffer (circular) for 3 stage
void Cdfe0051_func::UpdateIIRBufferThreeStage(unsigned int channel, unsigned int input1, unsigned int input2, unsigned int input3)
{//{{{
    // Get w11, w21
    unsigned int w11 = 0;
    unsigned int w21 = 0;
    unsigned int w31 = 0;
    memcpy(&w11, mDMEM + emCHDMEMSize*channel + mStartDataPtr[channel], 4);
    memcpy(&w21, mDMEM + emCHDMEMSize*channel + ((mStartDataPtr[channel] + 8)% emIIRCHCirMemSize), 4);
    memcpy(&w31, mDMEM + emCHDMEMSize*channel + ((mStartDataPtr[channel] + 0x10)% emIIRCHCirMemSize), 4);

    // Move back mStartDataPtr
    if (mStartDataPtr[channel] < 24){
        mStartDataPtr[channel] = mStartDataPtr[channel] + emIIRCHCirMemSize - 24;
    }else{
        mStartDataPtr[channel] -= 24;
    }

    // Store data to next data pointer
    memcpy(mDMEM + emCHDMEMSize*channel + mStartDataPtr[channel], &input1, 4);
    memcpy(mDMEM + emCHDMEMSize*channel + ((mStartDataPtr[channel]+4)%emIIRCHCirMemSize), &w11, 4);
    memcpy(mDMEM + emCHDMEMSize*channel + ((mStartDataPtr[channel]+8)%emIIRCHCirMemSize), &input2, 4);
    memcpy(mDMEM + emCHDMEMSize*channel + ((mStartDataPtr[channel]+0xC)%emIIRCHCirMemSize), &w21, 4);
    memcpy(mDMEM + emCHDMEMSize*channel + ((mStartDataPtr[channel]+0x10)%emIIRCHCirMemSize), &input3, 4);
    memcpy(mDMEM + emCHDMEMSize*channel + ((mStartDataPtr[channel]+0x14)%emIIRCHCirMemSize), &w31, 4);
}//}}}

bool Cdfe0051_func::IsFIRFixedPoint(unsigned int channel_id)
{//{{{
    return ((unsigned int)(*DFEjCTLACHn[channel_id])["FMT"] == 0);
}//}}}

void Cdfe0051_func::UpdateFIRBuffer(unsigned int channel_id, unsigned short xn, unsigned int num_taps)
{//{{{
    // Store data to next data pointer
    memcpy(mDMEM + emCHDMEMSize*channel_id + mStartDataPtr[channel_id], &xn, 2);
    // Move back mStartDataPtr
    if (mStartDataPtr[channel_id] == 0){
        mStartDataPtr[channel_id] = num_taps*2 - 2;
    }else{
        mStartDataPtr[channel_id] -= 2;
    }
    mCurDataPtr[channel_id] = mStartDataPtr[channel_id];
}//}}}

bool Cdfe0051_func::IsIntermediateConvertFloat(unsigned int channel_id)
{//{{{
    // Check hoft and prcsc
    bool ret = false;
    unsigned int hoft = (unsigned int)(*DFEjCTLBCHn[channel_id])["HOFS"];
    unsigned int prcsc = (unsigned int)(*DFEjCTLBCHn[channel_id])["PRCSC"];

    if ((hoft == 1) && (prcsc == 0)){
        ret = true;
    }
    return ret;
}//}}}

bool Cdfe0051_func::IsFIRSelect(unsigned int channel)
{//{{{
    unsigned int cmd = (unsigned int)(*DFEjCTLACHn[channel])["CMD"];
    if ((0 <= cmd) && (cmd <= 0x7)){
        return true;
    }
    return false;
}//}}}

bool Cdfe0051_func::IsIIRSelect(unsigned int channel)
{//{{{
    unsigned int cmd = (unsigned int)(*DFEjCTLACHn[channel])["CMD"];
    if (0x8 <= cmd){
        return true;
    }
    return false;
}//}}}

unsigned int Cdfe0051_func::GetFEBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjTRGCHn[channel])["FE"];
}//}}}

unsigned int Cdfe0051_func::GetFITMTRGBit(unsigned int trigger)
{//{{{
    unsigned int ret = 0;
    if (trigger == 0){
        ret = (unsigned int)(*DFEjFITRG)["FITMTRG0"];
    }else if (trigger == 1){
        ret = (unsigned int)(*DFEjFITRG)["FITMTRG1"];
    }else if (trigger == 2){
        ret = (unsigned int)(*DFEjFITRG)["FITMTRG2"];
    }else if (trigger == 3){
        ret = (unsigned int)(*DFEjFITRG)["FITMTRG3"];
    }else if (trigger == 4){
        ret = (unsigned int)(*DFEjFITRG1)["FITMTRG4"];
    }else if (trigger == 5){
        ret = (unsigned int)(*DFEjFITRG1)["FITMTRG5"];
    }
    return ret;
}//}}}

int Cdfe0051_func::GetFTBit(unsigned int channel)
{//{ {{
    int ret = 0;
    unsigned int fe = (unsigned int)(*DFEjTRGCHn[channel])["FE"];
    if (fe == 0){// Filter Initialize flag not generated
        ret = -1;
    }else if (fe == 1){// 0-3 chosen from DFEjTRGCHn.FT
        ret = (unsigned int)(*DFEjTRGCHn[channel])["FT"];
    }else if (fe == 2){// SW trigger
        ret = -1;
    }else if (fe == 3){// 0-5 chosen from DFEjTMTRGCHn
        ret = (unsigned int)(*DFEjTMTRGCHn[channel])["FT"];
    }
    return ret;
}//}}}

unsigned int Cdfe0051_func::GetCMDBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjCTLACHn[channel])["CMD"];
}//}}}

unsigned int Cdfe0051_func::GetFMTBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjCTLACHn[channel])["FMT"];
}//}}}

//========================================================
//============ Accumulate circuit ========================
//========================================================
bool Cdfe0051_func::IsAbsolutePerform(unsigned int channel)
{//{{{
    unsigned int abs = (unsigned int)(*DFEjCTLBCHn[channel])["ABS"];
    return (abs == 1);
}//}}}

void Cdfe0051_func::SetAERBit(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjSTCHn[channel])["AER"] = value;
    UpdateErrorStatus();
    UpdateDFEjST();
}//}}}

unsigned int Cdfe0051_func::GetAccumExpectedCount(unsigned int channel)
{//{{{
    unsigned int sela = (unsigned int)(*DFEjCTLBCHn[channel])["SELA"];
    unsigned int ret = 0;
    if (sela == 0){
        ret = (unsigned int)(*DFEjACA);
    }else if(sela == 1){
        ret = (unsigned int)(*DFEjACB);
    }else if(sela == 2){
        ret = (unsigned int)(*DFEjACC);
    }else{ // == 3
        ret = (unsigned int)(*DFEjACD);
    }
    return ret;
}//}}}

bool Cdfe0051_func::IsAccumEnable(unsigned int channel)
{//{{{ 
    unsigned int en = (unsigned int)(*DFEjCTLACHn[channel])["EN"];
    unsigned int prcsa = (unsigned int)(*DFEjCTLBCHn[channel])["PRCSA"];
    return ((en == 1) && (prcsa == 1));
}//}}}

bool Cdfe0051_func::IsDecimateEnable(unsigned int channel)
{//{{{ 
    unsigned int en = (unsigned int)(*DFEjCTLACHn[channel])["EN"];
    unsigned int prcsa = (unsigned int)(*DFEjCTLBCHn[channel])["PRCSA"];
    return ((en == 1) && (prcsa == 2));
}//}}}

unsigned int Cdfe0051_func::GetDeciCATAGBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjCTLACHn[channel])["CATAG"];
}//}}}

void Cdfe0051_func::SetCERBit(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjSTCHn[channel])["CER"] = value;
    UpdateErrorStatus();
    UpdateDFEjST();
}//}}}

void Cdfe0051_func::SetGERBit(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjSTCHn[channel])["GER"] = value;
    UpdateErrorStatus();
    UpdateDFEjST();
}//}}}

void Cdfe0051_func::SetMERBit(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjSTCHn[channel])["MER"] = value;
    UpdateErrorStatus();
    UpdateDFEjST();
}//}}}

unsigned int Cdfe0051_func::GetAEBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjTRGCHn[channel])["AE"];
}//}}}

unsigned int Cdfe0051_func::GetAFEBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjTRGCHn[channel])["AFE"];
}//}}}

unsigned int Cdfe0051_func::GetMITMTRGBit(unsigned int trigger)
{//{{{
	unsigned int ret = 0;
    if (trigger == 0){
        ret = (unsigned int)(*DFEjMITRG)["MITMTRG0"];
    }else if (trigger == 1){
        ret = (unsigned int)(*DFEjMITRG)["MITMTRG1"];
    }else if (trigger == 2){
        ret = (unsigned int)(*DFEjMITRG)["MITMTRG2"];
    }else if (trigger == 3){
        ret = (unsigned int)(*DFEjMITRG)["MITMTRG3"];
    }else if (trigger == 4){
        ret = (unsigned int)(*DFEjMITRG1)["MITMTRG4"];
    }else if (trigger == 5){
        ret = (unsigned int)(*DFEjMITRG1)["MITMTRG5"];
    }
	return ret;
}//}}}

int Cdfe0051_func::GetATBit(unsigned int channel)
{//{{{
    int ret = 0;
    unsigned int ae = (unsigned int)(*DFEjTRGCHn[channel])["AE"];
    if (ae == 0){// Accum/Deci Initialize flag not generated
        ret = -1;
    }else if (ae == 1){// 0-3 chosen from DFEjTRGCHn.AT
        ret = (unsigned int)(*DFEjTRGCHn[channel])["AT"];
    }else if (ae == 2){// SW trigger
        ret = -1;
    }else if (ae == 3){// 0-5 chosen from DFEjTMTRGCHn
        ret = (unsigned int)(*DFEjTMTRGCHn[channel])["AT"];
    }
    return ret;
}//}}}

unsigned int Cdfe0051_func::GetCAENBit(unsigned int channel)
{//{{{
    return ((unsigned int)(*DFEjCTLACHn[channel])["CAEN"]);
}//}}}

unsigned int Cdfe0051_func::GetPRCSABit(unsigned int channel)
{//{{{
    return ((unsigned int)(*DFEjCTLBCHn[channel])["PRCSA"]);
}//}}}

//========================================================
//============ Subtraction circuit =======================
//========================================================
void Cdfe0051_func::SetSUBTRHCHReg(unsigned int sub_id, unsigned int value)
{//{{{
    (*DFEjSUBTRHCHn[sub_id]) = value;
}//}}}

unsigned int Cdfe0051_func::GetMinuendChannelNum(unsigned int sub_id)
{//{{{
    return (unsigned int)(*DFEjSUBCTLCHn[sub_id])["MINCH"];
}//}}}

unsigned int Cdfe0051_func::GetSubtrahendChannelNum(unsigned int sub_id)
{//{{{
    return (unsigned int)(*DFEjSUBCTLCHn[sub_id])["SUBCH"];
}//}}}

void Cdfe0051_func::SetSUBFBit(unsigned int sub_id, unsigned int value)
{//{{{
    (*DFEjSUBSTCHn[sub_id])["SUBF"] = value;
}//}}}

unsigned int Cdfe0051_func::GetSIEOBit(unsigned int sub_id)
{//{{{
    return (unsigned int)(*DFEjSUBCTLCHn[sub_id])["SIEO"];
}//}}}

unsigned int Cdfe0051_func::GetSUBFBit(unsigned int sub_id)
{//{{{
    return (unsigned int)(*DFEjSUBSTCHn[sub_id])["SUBF"];
}//}}}

void Cdfe0051_func::SetMINFBit(unsigned int sub_id, unsigned int value)
{//{{{
    (*DFEjSUBSTCHn[sub_id])["MINF"] = value;
}//}}}

unsigned int Cdfe0051_func::GetMINFBit(unsigned int sub_id)
{//{{{
    return (unsigned int)(*DFEjSUBSTCHn[sub_id])["MINF"];
}//}}}

void Cdfe0051_func::SetSubDataOutReg(unsigned int sub_id, unsigned int value)
{//{{{
    (*DFEjSUBDOCHn[sub_id]) = value;
}//}}}

unsigned int Cdfe0051_func::GetSubCAEN(unsigned int sub_id)
{//{{{
    return (unsigned int)(*DFEjSUBCTLCHn[sub_id])["CAEN"];
}//}}}

bool Cdfe0051_func::IsFPConvertSubValue(unsigned int sub_id)
{//{{{
    return ((unsigned int)(*DFEjSUBCTLCHn[sub_id])["SFMT"] == 1);
}//}}}

void Cdfe0051_func::SetSDOENBit(unsigned int sub_id, unsigned int value)
{//{{{
    (*DFEjSUBSTCHn[sub_id])["SDOEN"] = value;
}//}}}

unsigned int Cdfe0051_func::GetSDOENBit(unsigned int sub_id)
{//{{{
    return (unsigned int)(*DFEjSUBSTCHn[sub_id])["SDOEN"];
}//}}}

void Cdfe0051_func::SetSDOOWBit(unsigned int sub_id, unsigned int value)
{//{{{
    (*DFEjSUBSTCHn[sub_id])["SDOOW"] = value;
    UpdateErrorStatus();
    UpdateDFEjEST();
}//}}}

void Cdfe0051_func::SetSCERBit(unsigned int sub_id, unsigned int value)
{//{{{
    (*DFEjSUBSTCHn[sub_id])["SCER"] = value;
    UpdateErrorStatus();
    UpdateDFEjEST();
}//}}}

void Cdfe0051_func::SetSGERBit(unsigned int sub_id, unsigned int value)
{//{{{
    (*DFEjSUBSTCHn[sub_id])["SGER"] = value;
    UpdateErrorStatus();
    UpdateDFEjEST();
}//}}}

unsigned int Cdfe0051_func::GetSubCATAGBit(unsigned int sub_id)
{//{{{
    return (unsigned int)(*DFEjSUBCTLCHn[sub_id])["CATAG"];
}//}}}

unsigned int Cdfe0051_func::GetSBEBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjSUBTRGCHn[channel])["SBE"];
}//}}}

unsigned int Cdfe0051_func::GetSBFEBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjSUBTRGCHn[channel])["SBFE"];
}//}}}

unsigned int Cdfe0051_func::GetSBTBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjSUBTRGCHn[channel])["SBT"];
}//}}}

unsigned int Cdfe0051_func::GetSUBTMTRGBit(unsigned int trigger)
{//{{{
	unsigned int ret = 0;
    if (trigger == 0){
        ret = (unsigned int)(*DFEjSUBTRG0)["SUBTMTRG0"];
    }else if(trigger == 1){
        ret = (unsigned int)(*DFEjSUBTRG0)["SUBTMTRG1"];
    }else if (trigger ==2){
        ret = (unsigned int)(*DFEjSUBTRG0)["SUBTMTRG2"];
    }
	return ret;
}//}}}

unsigned int Cdfe0051_func::GetSENBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjSUBCTLCHn[channel])["SEN"];
}//}}}

void Cdfe0051_func::SetSITSBit(unsigned int subid, unsigned int value)
{//{{{
    (*DFEjSUBTRHCHn[subid])["SITS"] = value;
}//}}}

void Cdfe0051_func::SetSETSBit(unsigned int subid, unsigned int value)
{//{{{
    (*DFEjSUBTRHCHn[subid])["SETS"] = value;
}//}}}

bool Cdfe0051_func::CheckSubValidSetting(unsigned int subid)
{//{{{
    bool can_run = true;
    can_run = CheckSettingDFEjSUBCTLCHn(subid) && can_run;    // Check valid setting of DFEjSUBCTLCHn
    can_run = CheckSettingDFEjSUBTRGCHn(subid) && can_run;    // Check valid setting of DFEjSUBTRGCHn
    can_run = CheckSettingDFEjSUBTRG() && can_run;            // Check valid setting of DFEjSUBTRG
    return can_run;
}//}}}

//========================================================
//============ Capture circuit    ========================
//========================================================
unsigned int Cdfe0051_func::GetCENBit(unsigned int capid)
{//{{{
    return (unsigned int)(*DFEjCAPCTLCHn[capid])["CEN"];
}//}}}

unsigned int Cdfe0051_func::GetCAPCHBit(unsigned int capid)
{//{{{
    return (unsigned int)(*DFEjCAPCTLCHn[capid])["CAPCH"];
}//}}}

void Cdfe0051_func::CaptureToCAPDOCHReg(unsigned int channel, unsigned int capid)
{//{{{
    unsigned int value = (unsigned int)(*DFEjDOCHn[channel]);
    unsigned int cdoen = (unsigned int)(*DFEjCAPSTCHn[capid])["CDOEN"];
    (*DFEjCAPDOCHn[capid]) = value;
    if (cdoen == 1){
        (*DFEjCAPSTCHn[capid])["CDOOW"] = 1;
        UpdateErrorStatus();
        UpdateDFEjEST();
    }else{
        (*DFEjCAPSTCHn[capid])["CDOEN"] = 1;
    }
}//}}}

//========================================================
//============ Intermediate value output circuit =========
//========================================================
void Cdfe0051_func::StoreIntermediateValue(unsigned int channel_id, unsigned int high, unsigned int low)
{//{{{
    assert(channel_id %2 == 0);
    switch(channel_id){
    case 0:
        // Store high, low
        (*DFEjHOHCH0) = high;
        (*DFEjHOLCH0) = low;
        // Store mirror(low)
        (*DFEjHOLMCH0) = low;
        break;
    case 2:
        // Store high, low
        (*DFEjHOHCH2) = high;
        (*DFEjHOLCH2) = low;
        // Store mirror(low)
        (*DFEjHOLMCH2) = low;
        break;
    case 4:
        // Store high, low
        (*DFEjHOHCH4) = high;
        (*DFEjHOLCH4) = low;
        // Store mirror(low)
        (*DFEjHOLMCH4) = low;
        break;
    case 6:
        // Store high, low
        (*DFEjHOHCH6) = high;
        (*DFEjHOLCH6) = low;
        // Store mirror(low)
        (*DFEjHOLMCH6) = low;
        break;
    case 8:
        // Store high, low
        (*DFEjHOHCH8) = high;
        (*DFEjHOLCH8) = low;
        // Store mirror(low)
        (*DFEjHOLMCH8) = low;
        break;
    case 10:
        // Store high, low
        (*DFEjHOHCH10) = high;
        (*DFEjHOLCH10) = low;
        // Store mirror(low)
        (*DFEjHOLMCH10) = low;
        break;
    case 12:
        // Store high, low
        (*DFEjHOHCH12) = high;
        (*DFEjHOLCH12) = low;
        // Store mirror(low)
        (*DFEjHOLMCH12) = low;
        break;
    default://case 14:
        // Store high, low
        (*DFEjHOHCH14) = high;
        (*DFEjHOLCH14) = low;
        // Store mirror(low)
        (*DFEjHOLMCH14) = low;
        break;
    }
}//}}}

bool Cdfe0051_func::CheckCapValidSetting(unsigned int capid)
{//{{{
    unsigned int capch = (unsigned int)(*DFEjCAPCTLCHn[capid])["CAPCH"];
    // DFEjCAPCTLCHn - Check valid setting when EN = 1
    if (capch > mNumChannel){
        re_printf("warning", "Do not set DFEjCAPCTLCH%d.CAPCH to 0x%X due to prohibited setting.\n", capid, capch);
        (*DFEjCAPCTLCHn[capid])["CAPCH"] = 0;
        return false;
    }
    return true;
}//}}}

//========================================================
//============ Output circuit ============================
//========================================================
bool Cdfe0051_func::IsFPConvertOutputValue(unsigned int channel)
{//{{{
    return ((unsigned int)(*DFEjCTLBCHn[channel])["PRCSC"] == 1);
}//}}}

unsigned int Cdfe0051_func::GetIEOBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjCTLACHn[channel])["IEO"];
}//}}}

void Cdfe0051_func::SetDOCHReg(unsigned int channel, unsigned int data)
{//{{{
    (*DFEjDOCHn[channel]) = data;
}//}}}

unsigned int Cdfe0051_func::GetDOCHReg(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjDOCHn[channel]);
}//}}}

void Cdfe0051_func::SetDOENBit(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjSTCHn[channel])["DOEN"] = value;
}//}}}

unsigned int Cdfe0051_func::GetDOENBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjSTCHn[channel])["DOEN"];
}//}}}
//========================================================
//============ PH circuit ================================
//========================================================
void Cdfe0051_func::SetPHEBit(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjSTCHn[channel])["PHE"] = value;
}//}}}

unsigned int Cdfe0051_func::GetIEPBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjCTLACHn[channel])["IEP"];
}//}}}

void Cdfe0051_func::SetDISBBit(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjCTLBCHn[channel])["DISB"] = value;
}//}}}

void Cdfe0051_func::SetDISABit(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjCTLBCHn[channel])["DISA"] = value;
}//}}}

unsigned int Cdfe0051_func::GetPRCSBBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjCTLBCHn[channel])["PRCSB"];
}//}}}

unsigned int Cdfe0051_func::GetPHInitVal(unsigned int phase, unsigned int channel)
{//{{{
    unsigned int prcsb = (unsigned int)(*DFEjCTLBCHn[channel])["PRCSB"];
    unsigned int select_bit = 0;
    unsigned int ret = 0;
    if (phase == 1){
        if (prcsb == 3){// PH and Compare simultaneously
            select_bit = (unsigned int)(*DFEjCTLBCHn[channel])["PHSLB2"];
        }else if (prcsb == 1){
            select_bit = (unsigned int)(*DFEjCTLBCHn[channel])["SELB2"];
        }
    }else if (phase == 2){
        select_bit = (unsigned int)(*DFEjPH2CTL0)["PH2SLB2"];
    }else if (phase == 3){
        select_bit = (unsigned int)(*DFEjPH3CTL0)["PH3SLB2"];
    }

    if (select_bit == 0){
        ret = (unsigned int)(*DFEjPHIA);
    }else if (select_bit == 1){
        ret = (unsigned int)(*DFEjPHIB);
    }else if (select_bit == 2){
        ret = (unsigned int)(*DFEjPHIC);
    }else{// (select_bit == 3){
        ret = (unsigned int)(*DFEjPHID);
    }// select_bit >= 4 prohibited.

    return ret;
}//}}}

unsigned int Cdfe0051_func::GetPHPSBit(unsigned int ph_phase, unsigned int channel)
{//{{{
    unsigned int phps = 0;
    if (ph_phase == 1){
        phps = (unsigned int)(*DFEjCTLBCHn[channel])["PHPS"];
    }else if (ph_phase == 2){
        phps = (unsigned int)(*DFEjPH2CTL0)["PHPS"];
    }else if (ph_phase == 3){
        phps = (unsigned int)(*DFEjPH3CTL0)["PHPS"];
    }
    return phps;
}//}}}

unsigned int Cdfe0051_func::GetPHUPDCHBit(unsigned int phuid)
{//{{{
    return (unsigned int)(*DFEjPHUPDCn[phuid])["PHUPDCH"];
}//}}}

unsigned int Cdfe0051_func::GetCNSLEBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjCTLACHn[channel])["CNSLE"];
}//}}}

unsigned int Cdfe0051_func::GetCNSLBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjCTLACHn[channel])["CNSL"];
}//}}}

unsigned int Cdfe0051_func::GetOEPHUPDBit(unsigned int phuid)
{//{{{
    return (unsigned int)(*DFEjPHUPDCn[phuid])["OEPHUPD"];
}//}}}

unsigned int Cdfe0051_func::GetPH23SLBit(unsigned int phuid)
{//{{{
    return (unsigned int)(*DFEjPHUPDCn[phuid])["PH23SL"];
}//}}}

unsigned int Cdfe0051_func::GetPH23EBit(unsigned int phuid)
{//{{{
    return (unsigned int)(*DFEjPHUPDCn[phuid])["PH23E"];
}//}}}

void Cdfe0051_func::SetPHCHReg(unsigned int phase, unsigned int channel, unsigned int value)
{//{{{
    if (phase == 1){
        (*DFEjPHCHn[channel]) = value;
    }else if (phase == 2){
        (*DFEjPH20) = value;
    }else if (phase == 3){
        (*DFEjPH30) = value;
    }
}//}}}

void Cdfe0051_func::SetPHCHPureVal(unsigned int phase, unsigned int channel, unsigned int value)
{//{{{
    if (phase == 1){
        mPHCHPureVal[channel] = value;
    }else if (phase == 2){
        mPH20PureVal = value;
    }else if (phase == 3){
        mPH30PureVal = value;
    }
}//}}}

void Cdfe0051_func::SetPrePHCH(unsigned int phase, unsigned int channel)
{//{{{
    if (phase == 1){
        mPrePHCH[channel] = mPHCHPureVal[channel];
    }else if (phase == 2){
        mPrePH20 = mPH20PureVal;
    }else if (phase == 3){
        mPrePH30 = mPH30PureVal;
    }
    mPrePhase[channel] = phase;
}//}}}

unsigned int Cdfe0051_func::GetPrePHCHReg(unsigned int channel)
{//{{{ 
    unsigned int ret = 0;
    if (mPrePhase[channel] == 1){
        ret = mPrePHCH[channel];
    }else if (mPrePhase[channel] == 2){
        ret = mPrePH20;
    }else if (mPrePhase[channel] == 3){
        ret = mPrePH30;
    }
    return ret;
}//}}}

unsigned int Cdfe0051_func::GetCompareOperator(unsigned int phase, unsigned int channel)
{//{{{ 
    unsigned int ret = 0;
    if (phase == 1){
        ret = (unsigned int)(*DFEjCTLBCHn[channel])["SELB2"];
    }else if (phase == 2){
        ret = (unsigned int)(*DFEjPH2CTL0)["CN2SLB2"];
    }else if (phase == 3){
        ret = (unsigned int)(*DFEjPH3CTL0)["CN3SLB2"];
    }
    return ret;
}//}}}

void Cdfe0051_func::SetPHINDBit(unsigned int ph_phase, unsigned int channel, unsigned int value)
{//{{{
    if (ph_phase == 1){
        (*DFEjPHINDCHn[channel])["PHIND"] = value;
        (*DFEjPHINDCHn[channel])["PHIOW"] = 1;
    }else if (ph_phase == 2){
        (*DFEjPH2IND0)["PH2IND"] = value;
        (*DFEjPH2IND0)["PH2IOW"] = 1;
    }else if (ph_phase == 3){
        (*DFEjPH3IND0)["PH3IND"] = value;
        (*DFEjPH3IND0)["PH3IOW"] = 1;
    }
}//}}}

void Cdfe0051_func::SetPHIOFBit(unsigned int ph_phase, unsigned int channel, unsigned int value)
{//{{{
    if (ph_phase == 1){
        (*DFEjPHINDCHn[channel])["PHIOF"] = value;
    }else if (ph_phase == 2){
        (*DFEjPH2IND0)["PH2IOF"] = value;
    }else if (ph_phase == 3){
        (*DFEjPH3IND0)["PH3IOF"] = value;
    }
}//}}}

void Cdfe0051_func::SetPHINDCHReg(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjPHINDCHn[channel]) = value;
}//}}}

void Cdfe0051_func::SetPH2IND0Reg(unsigned int value)
{//{{{
    (*DFEjPH2IND0) = value;
}//}}}

void Cdfe0051_func::SetPH3IND0Reg(unsigned int value)
{//{{{
    (*DFEjPH3IND0) = value;
}//}}}

unsigned int Cdfe0051_func::GetPHPFMTBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjCTLBCHn[channel])["PFMT"];
}//}}}

unsigned int Cdfe0051_func::GetPH23PENBit()
{//{{{
    return (unsigned int)(*DFEjPH23CCTL0)["PEN"];
}//}}}

unsigned int Cdfe0051_func::GetPH23CHSBit()
{//{{{
    return (unsigned int)(*DFEjPH23CCTL0)["CHS"];
}//}}}

unsigned int Cdfe0051_func::GetPH23STBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjSTCHn[channel])["PH23ST"];
}//}}}

void Cdfe0051_func::SetPH23STBit(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjSTCHn[channel])["PH23ST"] = value;
}//}}}

void Cdfe0051_func::SetPH20Reg(unsigned int value)
{//{{{
    (*DFEjPH20) = value;
}//}}}

void Cdfe0051_func::SetPH30Reg(unsigned int value)
{//{{{
    (*DFEjPH30) = value;
}//}}}

unsigned int Cdfe0051_func::GetCPCSBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjCTLBCHn[channel])["CPCS"];
}//}}}

unsigned int Cdfe0051_func::GetAlpha(unsigned int ph_phase, unsigned int channel)
{//{{{
    unsigned int ofsl = 0;
    // Find ofsl according ph23 phase
    if (ph_phase == 1){
        ofsl = (unsigned int)(*DFEjCTLBCHn[channel])["OFSL"];
    }else if (ph_phase == 2){
        ofsl = (unsigned int)(*DFEjPH2CTL0)["OFSL"];
    }else if (ph_phase == 3){
        ofsl = (unsigned int)(*DFEjPH3CTL0)["OFSL"];
    }
    // Determine alpha
    return (unsigned int)(*DFEjCPOFSTn[ofsl]);
}//}}}

unsigned int Cdfe0051_func::GetPMFEBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjTRGCHn[channel])["PMFE"];
}//}}}

unsigned int Cdfe0051_func::GetPMEBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjTRGCHn[channel])["PME"];
}//}}}

unsigned int Cdfe0051_func::GetPEBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjTRGCHn[channel])["PE"];
}//}}}

unsigned int Cdfe0051_func::GetPFEBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjTRGCHn[channel])["PFE"];
}//}}}

unsigned int Cdfe0051_func::GetPMTBit(unsigned int channel)
{//{{{
    return (unsigned int)(*DFEjTMTRGCHn[channel])["PMT"];
}//}}}

unsigned int Cdfe0051_func::GetPMITMTRGBit(unsigned int trigger)
{//{{{
	unsigned int ret = 0;
    if (trigger == 0){
        ret = (unsigned int)(*DFEjPMITRG0)["PMITMTRG0"];
    }else if (trigger == 1){
        ret = (unsigned int)(*DFEjPMITRG0)["PMITMTRG1"];
    }else if (trigger == 2){
        ret = (unsigned int)(*DFEjPMITRG0)["PMITMTRG2"];
    }else if (trigger == 3){
        ret = (unsigned int)(*DFEjPMITRG0)["PMITMTRG3"];
    }else if (trigger == 4){
        ret = (unsigned int)(*DFEjPMITRG1)["PMITMTRG4"];
    }else if (trigger == 5){
        ret = (unsigned int)(*DFEjPMITRG1)["PMITMTRG5"];
    }
	return ret;
}//}}}

unsigned int Cdfe0051_func::GetPITMTRGBit(unsigned int trigger)
{//{{{
    unsigned int ret = 0;
    if (trigger == 0){
        ret = (unsigned int)(*DFEjPITRG)["PITMTRG0"];
    }else if (trigger == 1){
        ret = (unsigned int)(*DFEjPITRG)["PITMTRG1"];
    }else if (trigger == 2){
        ret = (unsigned int)(*DFEjPITRG)["PITMTRG2"];
    }else if (trigger == 3){
        ret = (unsigned int)(*DFEjPITRG)["PITMTRG3"];
    }else if (trigger == 4){
        ret = (unsigned int)(*DFEjPITRG1)["PITMTRG4"];
    }else if (trigger == 5){
        ret = (unsigned int)(*DFEjPITRG1)["PITMTRG5"];
    }
    return ret;
}//}}}

int Cdfe0051_func::GetPTBit(unsigned int channel)
{//{{{
    int ret = 0;
    unsigned int pe = (unsigned int)(*DFEjTRGCHn[channel])["PE"];
    if (pe == 0){// PH Initialize flag not generated
        ret = -1;
    }else if (pe == 1){// 0-3 chosen from DFEjTRGCHn.PT
        ret = (unsigned int)(*DFEjTRGCHn[channel])["PT"];
    }else if (pe == 2){// SW trigger
        ret = -1;
    }else if (pe == 3){// 0-5 chosen from DFEjTMTRGCHn
        ret = (unsigned int)(*DFEjTMTRGCHn[channel])["PT"];
    }
    return ret;
}//}}}

void Cdfe0051_func::SetCNDBit(unsigned int channel, unsigned int value)
{//{{{
    (*DFEjSTCHn[channel])["CND"] = value;
}//}}}

unsigned int Cdfe0051_func::GetCPRegWithSELB1Bit(unsigned int channel)
{//{{{
    unsigned int selb1 = (unsigned int)(*DFEjCTLBCHn[channel])["SELB1"];
    unsigned int ret = 0;
    switch(selb1){
    case 0://DFEjCPA
        ret = (unsigned int)(*DFEjCPA);
        break;
    case 1://DFEjCPB
        ret = (unsigned int)(*DFEjCPB);
        break;
    case 2://DFEjCPC
        ret = (unsigned int)(*DFEjCPC);
        break;
    default://DFEjCPD
        ret = (unsigned int)(*DFEjCPD);
        break;
    }
    return ret;
}//}}}

unsigned int Cdfe0051_func::GetCHSBit()
{//{{{
    return (unsigned int)(*DFEjPH23CCTL0)["CHS"];
}//}}}
//========================================================
//============ Callbacks and common ======================
//========================================================
void Cdfe0051_func::UpdateErrorStatus()
{//{{{
    // Check error from channel
    bool cherr = false;
    for(unsigned int i = 0; i < mNumChannel; i++){
        bool cherr_ele = false;
        cherr_ele = cherr_ele || ((bool)(*DFEjSTCHn[i])["AER"]  && !((bool)(*DFEjERMCHn[i])["MSKAER"]));
        cherr_ele = cherr_ele || ((bool)(*DFEjSTCHn[i])["CER"]  && !((bool)(*DFEjERMCHn[i])["MSKCER"]));
        cherr_ele = cherr_ele || ((bool)(*DFEjSTCHn[i])["MER"]  && !((bool)(*DFEjERMCHn[i])["MSKMER"]));
        cherr_ele = cherr_ele || ((bool)(*DFEjSTCHn[i])["GER"]  && !((bool)(*DFEjERMCHn[i])["MSKGER"]));
        cherr_ele = cherr_ele || ((bool)(*DFEjSTCHn[i])["DOOW"] && !((bool)(*DFEjERMCHn[i])["MSKDOOW"]));
        cherr_ele = cherr_ele || ((bool)(*DFEjSTCHn[i])["DIOW"] && !((bool)(*DFEjERMCHn[i])["MSKDIOW"]));
        cherr_ele = cherr_ele && ((bool)(*DFEjCTLACHn[i])["IEE"]);
        cherr = cherr || cherr_ele;
    }
    // Check error from subtraction
    bool suberr = false;
    for (unsigned int i = 0; i < 3; i++){
        bool suberr_ele = false;
        suberr_ele = suberr_ele || ((bool)(*DFEjSUBSTCHn[i])["SCER"]  && !((bool)(*DFEjSUBERMCHn[i])["MSKSCER"]));
        suberr_ele = suberr_ele || ((bool)(*DFEjSUBSTCHn[i])["SGER"]  && !((bool)(*DFEjSUBERMCHn[i])["MSKSGER"]));
        suberr_ele = suberr_ele || ((bool)(*DFEjSUBSTCHn[i])["SDOOW"] && !((bool)(*DFEjSUBERMCHn[i])["MSKSDOOW"]));
        suberr_ele = suberr_ele && ((bool)(*DFEjSUBCTLCHn[i])["SIEE"]);
        suberr = suberr || suberr_ele;
    }
    // Check error from capture
    bool caperr = false;
    for (unsigned int i = 0; i < 3; i++){
        bool caperr_ele = false;
        caperr_ele = caperr_ele || ((bool)(*DFEjCAPSTCHn[i])["CDOOW"] && !((bool)(*DFEjCAPERMCHn[i])["MSKCDOOW"]));
        caperr_ele = caperr_ele && ((bool)(*DFEjCAPCTLCHn[i])["CIEE"]);
        caperr = caperr || caperr_ele;
    }

    bool error_status = cherr || suberr || caperr;

    // Update error status
    mPreErrStatus = mCurErrStatus;
    mCurErrStatus = error_status;
    if (mCurErrStatus && (!mPreErrStatus)){
        mDFE0051AgentController->RaiseINTDFE0051ERR();
    }
}//}}}

bool Cdfe0051_func::CheckSettingDFEjCTLACHn(unsigned int channel)
{//{{{
    bool can_run = true;
    unsigned int caen = (unsigned int)(*DFEjCTLACHn[channel])["CAEN"];
    unsigned int cmd = (unsigned int)(*DFEjCTLACHn[channel])["CMD"];

    if (caen == 1){
        re_printf("warning", "Do not set DFEjCTLACH%d.CAEN to 0x%X due to prohibited setting.\n", channel, caen);
        (*DFEjCTLACHn[channel])["CAEN"] = 0;
    }
    if ((cmd == 0x4) || (cmd == 0x5) || (cmd == 0x6) || (cmd == 0xB) || (cmd == 0xC) || (cmd == 0xF)){
        re_printf("warning", "Do not set DFEjCTLACH%d.CMD to 0x%X due to prohibited setting.\n", channel, cmd);
        (*DFEjCTLACHn[channel])["CMD"] = 0;
        can_run = can_run && false;
    }
    return can_run;
}//}}}

bool Cdfe0051_func::CheckSettingDFEjCTLBCHn(unsigned int channel)
{//{{{
    bool can_run = true;
    unsigned int fmt = (unsigned int)(*DFEjCTLACHn[channel])["FMT"];
    unsigned int ofsl = (unsigned int)(*DFEjCTLBCHn[channel])["OFSL"];
    unsigned int phslb2 = (unsigned int)(*DFEjCTLBCHn[channel])["PHSLB2"];
    unsigned int selb2 = (unsigned int)(*DFEjCTLBCHn[channel])["SELB2"];
    unsigned int prcsa = (unsigned int)(*DFEjCTLBCHn[channel])["PRCSA"];
    unsigned int prcsb = (unsigned int)(*DFEjCTLBCHn[channel])["PRCSB"];
    unsigned int prcsc = (unsigned int)(*DFEjCTLBCHn[channel])["PRCSC"];
    unsigned int cpcs = (unsigned int)(*DFEjCTLBCHn[channel])["CPCS"];
    unsigned int selb1 = (unsigned int)(*DFEjCTLBCHn[channel])["SELB1"];
    unsigned int hofs = (unsigned int)(*DFEjCTLBCHn[channel])["HOFS"];
    unsigned int pfmt = (unsigned int)(*DFEjCTLBCHn[channel])["PFMT"];
    
    // Prohibited value
    if (!((0x0 <= ofsl) && (ofsl <= 0x2))){
        re_printf("warning", "Do not set DFEjCTLBCH%d.OFSL to 0x%X due to prohibited setting.\n", channel, ofsl);
        (*DFEjCTLBCHn[channel])["OFSL"] = 0;
    }
    if (!((0x0 <= phslb2) && (phslb2 <= 0x3))){
        re_printf("warning", "Do not set DFEjCTLBCH%d.PHSLB2 to 0x%X due to prohibited setting.\n", channel, phslb2);
        (*DFEjCTLBCHn[channel])["PHSLB2"] = 0;
    }
    if (!((0x0 <= prcsa) && (prcsa <= 0x2))){
        re_printf("warning", "Do not set DFEjCTLBCH%d.PRCSA to 0x%X due to prohibited setting.\n", channel, prcsa);
        (*DFEjCTLBCHn[channel])["PRCSA"] = 0;
    }
    if ((prcsb == 1) && (selb2 >= 0x4)){
        re_printf("warning", "Do not set DFEjCTLBCH%d.SELB2 to 0x%X due to prohibited setting.\n", channel, selb2);
        (*DFEjCTLBCHn[channel])["SELB2"] = 0;
    }
    if (((prcsb == 2) || (prcsb == 3)) && (selb2 >= 0x5)){
        re_printf("warning", "Do not set DFEjCTLBCH%d.SELB2 to 0x%X due to prohibited setting.\n", channel, selb2);
        (*DFEjCTLBCHn[channel])["SELB2"] = 0;
    }
    // PRCSC bit not set 1 if input is integer format
    if ((fmt == 1) && (prcsc == 1) && IsFIRSelect(channel)){
        re_printf("warning", "Do not set DFEjCTLBCH%d.PRCSC to 0x1 if input is integer format and FIR is selected.\n", channel);
        (*DFEjCTLBCHn[channel])["PRCSC"] = 0;
    }
    // SELB1 bit
    if ((prcsb == 3) && (cpcs == 1) && (selb1 >=1)){
        re_printf("warning", "Do not set DFEjCTLBCH%d.SELB1 to 0x%X due to prohibited setting.\n", channel, selb1);
        (*DFEjCTLBCHn[channel])["SELB1"] = 0;
    }
    // HOFS bit
    if ((fmt == 1) && (prcsc == 1) && (hofs == 1) && IsFIRSelect(channel)){
        re_printf("warning", "Do not set DFEjCTLBCH%d.HOFS to 0x1 if input is integer format, output data register floating-point converted, and FIR is selected.\n", channel);
        (*DFEjCTLBCHn[channel])["HOFS"] = 0;
    }
    // PFMT bit
    if (((prcsb == 1) || (prcsb == 3)) && IsFIRSelect(channel) && (fmt == 1) && (pfmt == 1)){
        re_printf("warning", "Do not set DFEjCTLBCH%d.PFMT to 0x1 if input is integer format and FIR is selected.\n", channel);
        (*DFEjCTLBCHn[channel])["PFMT"] = 0;
    }
    return can_run;
}//}}}

bool Cdfe0051_func::CheckSettingDFEjTRGCHn(unsigned int channel)
{//{{{
    bool can_run = true;
    unsigned int pmfe = (unsigned int)(*DFEjTRGCHn[channel])["PMFE"];
    unsigned int pme = (unsigned int)(*DFEjTRGCHn[channel])["PME"];
    unsigned int pe = (unsigned int)(*DFEjTRGCHn[channel])["PE"];
    unsigned int pfe = (unsigned int)(*DFEjTRGCHn[channel])["PFE"];
    unsigned int afe = (unsigned int)(*DFEjTRGCHn[channel])["AFE"];
    unsigned int ae = (unsigned int)(*DFEjTRGCHn[channel])["AE"];

    if (!((0x0 <= pmfe) && (pmfe <= 0x2))){
        re_printf("warning", "Do not set DFEjTRGCH%d.PMFE to 0x%X due to prohibited setting.\n", channel, pmfe);
        (*DFEjTRGCHn[channel])["PMFE"] = 0;
    }
    if (!((0x0 <= pme) && (pme <= 0x2))){
        re_printf("warning", "Do not set DFEjTRGCH%d.PME to 0x%X due to prohibited setting.\n", channel, pme);
        (*DFEjTRGCHn[channel])["PME"] = 0;
    }
    if (!((0x0 <= pfe) && (pfe <= 0x2))){
        re_printf("warning", "Do not set DFEjTRGCH%d.PFE to 0x%X due to prohibited setting.\n", channel, pfe);
        (*DFEjTRGCHn[channel])["PFE"] = 0;
    }
    if (!((0x0 <= afe) && (afe <= 0x2))){
        re_printf("warning", "Do not set DFEjTRGCH%d.AFE to 0x%X due to prohibited setting.\n", channel, afe);
        (*DFEjTRGCHn[channel])["AFE"] = 0;
    }
    // Check not set both init and end/disable
    if ((pmfe == 2) && (pme == 2)){
        re_printf("warning", "Do not set both DFEjTRGCH%d.PMFE and DFEjTRGCH%d.PME to 0x%X due to prohibited setting.\n", channel, channel, pmfe);
        (*DFEjTRGCHn[channel])["PMFE"] = 0;
        (*DFEjTRGCHn[channel])["PME"] = 0;
        can_run = can_run && false;
    }
    if ((pfe == 2) && (pe == 2)){
        re_printf("warning", "Do not set both DFEjTRGCH%d.PFE and DFEjTRGCH%d.PE to 0x%X due to prohibited setting.\n", channel, channel, pfe);
        (*DFEjTRGCHn[channel])["PFE"] = 0;
        (*DFEjTRGCHn[channel])["PE"] = 0;
        can_run = can_run && false;
    }
    if ((afe == 2) && (ae == 2)){
        re_printf("warning", "Do not set both DFEjTRGCH%d.AFE and DFEjTRGCH%d.AE to 0x%X due to prohibited setting.\n", channel, channel, afe);
        (*DFEjTRGCHn[channel])["AFE"] = 0;
        (*DFEjTRGCHn[channel])["AE"] = 0;
        can_run = can_run && false;
    }
    return can_run;
}//}}} 

bool Cdfe0051_func::CheckSettingDFEjPMITRG()
{//{{{
    bool can_run = true;
    unsigned int pmitmtrg5 = (unsigned int)(*DFEjPMITRG1)["PMITMTRG5"];
    unsigned int pmitmtrg4 = (unsigned int)(*DFEjPMITRG1)["PMITMTRG4"];
    unsigned int pmitmtrg3 = (unsigned int)(*DFEjPMITRG0)["PMITMTRG3"];
    unsigned int pmitmtrg2 = (unsigned int)(*DFEjPMITRG0)["PMITMTRG2"];
    unsigned int pmitmtrg1 = (unsigned int)(*DFEjPMITRG0)["PMITMTRG1"];
    unsigned int pmitmtrg0 = (unsigned int)(*DFEjPMITRG0)["PMITMTRG0"];

    if (!((0x0 <= pmitmtrg5) && (pmitmtrg5 <= 0x13))){
        re_printf("warning", "Do not set DFEjPMITRG1.PMITMTRG5 to 0x%X due to prohibited setting.\n",  pmitmtrg5);
        (*DFEjPMITRG1)["PMITMTRG5"] = 0;
    }
    if (!((0x0 <= pmitmtrg4) && (pmitmtrg4 <= 0x13))){
        re_printf("warning", "Do not set DFEjPMITRG1.PMITMTRG4 to 0x%X due to prohibited setting.\n",  pmitmtrg4);
        (*DFEjPMITRG1)["PMITMTRG4"] = 0;
    }
    if (!((0x0 <= pmitmtrg3) && (pmitmtrg3 <= 0x13))){
        re_printf("warning", "Do not set DFEjPMITRG0.PMITMTRG3 to 0x%X due to prohibited setting.\n",  pmitmtrg3);
        (*DFEjPMITRG0)["PMITMTRG3"] = 0;
    }
    if (!((0x0 <= pmitmtrg2) && (pmitmtrg2 <= 0x13))){
        re_printf("warning", "Do not set DFEjPMITRG0.PMITMTRG2 to 0x%X due to prohibited setting.\n",  pmitmtrg2);
        (*DFEjPMITRG0)["PMITMTRG2"] = 0;
    }
    if (!((0x0 <= pmitmtrg1) && (pmitmtrg1 <= 0x13))){
        re_printf("warning", "Do not set DFEjPMITRG0.PMITMTRG1 to 0x%X due to prohibited setting.\n",  pmitmtrg1);
        (*DFEjPMITRG0)["PMITMTRG1"] = 0;
    }
    if (!((0x0 <= pmitmtrg0) && (pmitmtrg0 <= 0x13))){
        re_printf("warning", "Do not set DFEjPMITRG0.PMITMTRG0 to 0x%X due to prohibited setting.\n",  pmitmtrg0);
        (*DFEjPMITRG0)["PMITMTRG0"] = 0;
    }
    return can_run;
}//}}}

bool Cdfe0051_func::CheckSettingDFEjPITRG()
{//{{{
    bool can_run = true;
    unsigned int pitmtrg5 = (unsigned int)(*DFEjPITRG1)["PITMTRG5"];
    unsigned int pitmtrg4 = (unsigned int)(*DFEjPITRG1)["PITMTRG4"];
    unsigned int pitmtrg3 = (unsigned int)(*DFEjPITRG)["PITMTRG3"];
    unsigned int pitmtrg2 = (unsigned int)(*DFEjPITRG)["PITMTRG2"];
    unsigned int pitmtrg1 = (unsigned int)(*DFEjPITRG)["PITMTRG1"];
    unsigned int pitmtrg0 = (unsigned int)(*DFEjPITRG)["PITMTRG0"];

    if (!((0x0 <= pitmtrg5) && (pitmtrg5 <= 0x13))){
        re_printf("warning", "Do not set DFEjPITRG1.PITMTRG5 to 0x%X due to prohibited setting.\n",  pitmtrg5);
        (*DFEjPITRG1)["PITMTRG5"] = 0;
    }
    if (!((0x0 <= pitmtrg4) && (pitmtrg4 <= 0x13))){
        re_printf("warning", "Do not set DFEjPITRG1.PITMTRG4 to 0x%X due to prohibited setting.\n",  pitmtrg4);
        (*DFEjPITRG1)["PITMTRG4"] = 0;
    }
    if (!((0x0 <= pitmtrg3) && (pitmtrg3 <= 0x13))){
        re_printf("warning", "Do not set DFEjPITRG.PITMTRG3 to 0x%X due to prohibited setting.\n",  pitmtrg3);
        (*DFEjPITRG)["PITMTRG3"] = 0;
    }
    if (!((0x0 <= pitmtrg2) && (pitmtrg2 <= 0x13))){
        re_printf("warning", "Do not set DFEjPITRG.PITMTRG2 to 0x%X due to prohibited setting.\n",  pitmtrg2);
        (*DFEjPITRG)["PITMTRG2"] = 0;
    }
    if (!((0x0 <= pitmtrg1) && (pitmtrg1 <= 0x13))){
        re_printf("warning", "Do not set DFEjPITRG.PITMTRG1 to 0x%X due to prohibited setting.\n",  pitmtrg1);
        (*DFEjPITRG)["PITMTRG1"] = 0;
    }
    if (!((0x0 <= pitmtrg0) && (pitmtrg0 <= 0x13))){
        re_printf("warning", "Do not set DFEjPITRG.PITMTRG0 to 0x%X due to prohibited setting.\n",  pitmtrg0);
        (*DFEjPITRG)["PITMTRG0"] = 0;
    }
    return can_run;
}//}}}

bool Cdfe0051_func::CheckSettingDFEjMITRG()
{//{{{
    bool can_run = true;
    unsigned int mitmtrg5 = (unsigned int)(*DFEjMITRG1)["MITMTRG5"];
    unsigned int mitmtrg4 = (unsigned int)(*DFEjMITRG1)["MITMTRG4"];
    unsigned int mitmtrg3 = (unsigned int)(*DFEjMITRG)["MITMTRG3"];
    unsigned int mitmtrg2 = (unsigned int)(*DFEjMITRG)["MITMTRG2"];
    unsigned int mitmtrg1 = (unsigned int)(*DFEjMITRG)["MITMTRG1"];
    unsigned int mitmtrg0 = (unsigned int)(*DFEjMITRG)["MITMTRG0"];

    if (!((0x0 <= mitmtrg5) && (mitmtrg5 <= 0x13))){
        re_printf("warning", "Do not set DFEjMITRG1.MITMTRG5 to 0x%X due to prohibited setting.\n",  mitmtrg5);
        (*DFEjMITRG1)["MITMTRG5"] = 0;
    }
    if (!((0x0 <= mitmtrg4) && (mitmtrg4 <= 0x13))){
        re_printf("warning", "Do not set DFEjMITRG1.MITMTRG4 to 0x%X due to prohibited setting.\n",  mitmtrg4);
        (*DFEjMITRG1)["MITMTRG4"] = 0;
    }
    if (!((0x0 <= mitmtrg3) && (mitmtrg3 <= 0x13))){
        re_printf("warning", "Do not set DFEjMITRG.MITMTRG3 to 0x%X due to prohibited setting.\n",  mitmtrg3);
        (*DFEjMITRG)["MITMTRG3"] = 0;
    }
    if (!((0x0 <= mitmtrg2) && (mitmtrg2 <= 0x13))){
        re_printf("warning", "Do not set DFEjMITRG.MITMTRG2 to 0x%X due to prohibited setting.\n",  mitmtrg2);
        (*DFEjMITRG)["MITMTRG2"] = 0;
    }
    if (!((0x0 <= mitmtrg1) && (mitmtrg1 <= 0x13))){
        re_printf("warning", "Do not set DFEjMITRG.MITMTRG1 to 0x%X due to prohibited setting.\n",  mitmtrg1);
        (*DFEjMITRG)["MITMTRG1"] = 0;
    }
    if (!((0x0 <= mitmtrg0) && (mitmtrg0 <= 0x13))){
        re_printf("warning", "Do not set DFEjMITRG.MITMTRG0 to 0x%X due to prohibited setting.\n",  mitmtrg0);
        (*DFEjMITRG)["MITMTRG0"] = 0;
    }
    return can_run;
}//}}}

bool Cdfe0051_func::CheckSettingDFEjFITRG()
{//{{{
    bool can_run = true;
    unsigned int fitmtrg5 = (unsigned int)(*DFEjFITRG1)["FITMTRG5"];
    unsigned int fitmtrg4 = (unsigned int)(*DFEjFITRG1)["FITMTRG4"];
    unsigned int fitmtrg3 = (unsigned int)(*DFEjFITRG)["FITMTRG3"];
    unsigned int fitmtrg2 = (unsigned int)(*DFEjFITRG)["FITMTRG2"];
    unsigned int fitmtrg1 = (unsigned int)(*DFEjFITRG)["FITMTRG1"];
    unsigned int fitmtrg0 = (unsigned int)(*DFEjFITRG)["FITMTRG0"];

    if (!((0x0 <= fitmtrg5) && (fitmtrg5 <= 0x13))){
        re_printf("warning", "Do not set DFEjFITRG1.FITMTRG5 to 0x%X due to prohibited setting.\n", fitmtrg5);
        (*DFEjFITRG1)["FITMTRG5"] = 0;
    }
    if (!((0x0 <= fitmtrg4) && (fitmtrg4 <= 0x13))){
        re_printf("warning", "Do not set DFEjFITRG1.FITMTRG4 to 0x%X due to prohibited setting.\n",  fitmtrg4);
        (*DFEjFITRG1)["FITMTRG4"] = 0;
    }
    if (!((0x0 <= fitmtrg3) && (fitmtrg3 <= 0x13))){
        re_printf("warning", "Do not set DFEjFITRG.FITMTRG3 to 0x%X due to prohibited setting.\n",  fitmtrg3);
        (*DFEjFITRG)["FITMTRG3"] = 0;
    }
    if (!((0x0 <= fitmtrg2) && (fitmtrg2 <= 0x13))){
        re_printf("warning", "Do not set DFEjFITRG.FITMTRG2 to 0x%X due to prohibited setting.\n",  fitmtrg2);
        (*DFEjFITRG)["FITMTRG2"] = 0;
    }
    if (!((0x0 <= fitmtrg1) && (fitmtrg1 <= 0x13))){
        re_printf("warning", "Do not set DFEjFITRG.FITMTRG1 to 0x%X due to prohibited setting.\n",  fitmtrg1);
        (*DFEjFITRG)["FITMTRG1"] = 0;
    }
    if (!((0x0 <= fitmtrg0) && (fitmtrg0 <= 0x13))){
        re_printf("warning", "Do not set DFEjFITRG.FITMTRG0 to 0x%X due to prohibited setting.\n",  fitmtrg0);
        (*DFEjFITRG)["FITMTRG0"] = 0;
    }
    return can_run;
}//}}}

bool Cdfe0051_func::CheckSettingDFEjPHUPDCn()
{//{{{
    bool can_run = true;
    for (unsigned int i = 0; i < 4; i++){
        unsigned int phupdch = (unsigned int)(*DFEjPHUPDCn[i])["PHUPDCH"];

        if (phupdch > mNumChannel){
            re_printf("warning", "Do not set DFEjPHUPDC%d.PHUPDCH to 0x%X due to prohibited setting.\n", i, phupdch);
            (*DFEjPHUPDCn[i])["PHUPDCH"] = 0;
            can_run = can_run && false;
        }
    }
    return can_run;
}//}}}

bool Cdfe0051_func::CheckSettingDFEjTMTRGCHn(unsigned int channel)
{//{{{
    bool can_run = true;
    unsigned int pmt = (unsigned int)(*DFEjTMTRGCHn[channel])["PMT"];
    unsigned int pt = (unsigned int)(*DFEjTMTRGCHn[channel])["PT"];
    unsigned int at = (unsigned int)(*DFEjTMTRGCHn[channel])["AT"];
    unsigned int ft = (unsigned int)(*DFEjTMTRGCHn[channel])["FT"];

    if (!((0x0 <= pmt) && (pmt <= 0x5))){
        re_printf("warning", "Do not set DFEjTMTRGCH%d.PMT to 0x%X due to prohibited setting.\n", channel, pmt);
        (*DFEjTMTRGCHn[channel])["PMT"] = 0;
    }
    if (!((0x0 <= pt) && (pt <= 0x5))){
        re_printf("warning", "Do not set DFEjTMTRGCH%d.PT to 0x%X due to prohibited setting.\n", channel, pt);
        (*DFEjTMTRGCHn[channel])["PT"] = 0;
    }
    if (!((0x0 <= at) && (at <= 0x5))){
        re_printf("warning", "Do not set DFEjTMTRGCH%d.AT to 0x%X due to prohibited setting.\n", channel, at);
        (*DFEjTMTRGCHn[channel])["AT"] = 0;
    }
    if (!((0x0 <= ft) && (ft <= 0x5))){
        re_printf("warning", "Do not set DFEjTMTRGCH%d.FT to 0x%X due to prohibited setting.\n", channel, ft);
        (*DFEjTMTRGCHn[channel])["FT"] = 0;
    }
    return can_run;
}//}}}

bool Cdfe0051_func::CheckSettingDFEjSUBCTLCHn(unsigned int channel)
{//{{{
    bool can_run = true;
    unsigned int caen = (unsigned int)(*DFEjSUBCTLCHn[channel])["CAEN"];
    unsigned int minch = (unsigned int)(*DFEjSUBCTLCHn[channel])["MINCH"];
    unsigned int subch = (unsigned int)(*DFEjSUBCTLCHn[channel])["SUBCH"];

    if (caen == 0x1){
        re_printf("warning", "Do not set DFEjSUBCTLCH%d.CAEN to 0x%X due to prohibited setting.\n", channel, caen);
        (*DFEjSUBCTLCHn[channel])["CAEN"] = 0;
    }
    if (minch > mNumChannel){
        re_printf("warning", "Do not set DFEjSUBCTLCH%d.MINCH to 0x%X due to prohibited setting.\n", channel, minch);
        (*DFEjSUBCTLCHn[channel])["MINCH"] = 0;
    }
    if (subch > mNumChannel){
        re_printf("warning", "Do not set DFEjSUBCTLCH%d.SUBCH to 0x%X due to prohibited setting.\n", channel, subch);
        (*DFEjSUBCTLCHn[channel])["SUBCH"] = 0;
    }
    // Not set same channel in MINCH and SUBCH
    if (minch == subch){
        re_printf("warning", "Do not set DFEjSUBCTLCH%d.SUBCH and DFEjSUBCTLCH%d.MINCH same value due to prohibited setting.\n", channel, channel);
        (*DFEjSUBCTLCHn[channel])["SUBCH"] = 0;
        (*DFEjSUBCTLCHn[channel])["MINCH"] = 0;
        can_run = can_run && false;
    }

    unsigned int fmt_minuend = (unsigned int)(*DFEjCTLACHn[minch])["FMT"];
    unsigned int fmt_subtrahend = (unsigned int)(*DFEjCTLACHn[subch])["FMT"];

    if ((fmt_minuend == 1) && (fmt_subtrahend == 1) && IsFIRSelect(minch) && IsFIRSelect(subch) && ((unsigned int)(*DFEjSUBCTLCHn[channel])["SFMT"] == 1)){
        re_printf("warning", "Do not set DFEjSUBCTLCH%d.SFMT to 0x1 if input of both SUBCH(=%d) and MINCH(=%d) are integer and FIR is selected.", channel, subch, minch);
        (*DFEjSUBCTLCHn[channel])["SFMT"] = 0;
    }
    return can_run;
}//}}}

bool Cdfe0051_func::CheckSettingDFEjSUBTRGCHn(unsigned int channel)
{//{{{
    bool can_run = true;
    unsigned int sbfe = (unsigned int)(*DFEjSUBTRGCHn[channel])["SBFE"];
    unsigned int sbe = (unsigned int)(*DFEjSUBTRGCHn[channel])["SBE"];
    unsigned int sbt = (unsigned int)(*DFEjSUBTRGCHn[channel])["SBT"];

    if (!((0x0 <= sbfe) && (sbfe <= 0x2))){
        re_printf("warning", "Do not set DFEjSUBTRGCH%d.SBFE to 0x%X due to prohibited setting.\n", channel, sbfe);
        (*DFEjSUBTRGCHn[channel])["SBFE"] = 0;
    }
    if (!((0x0 <= sbe) && (sbe <= 0x2))){
        re_printf("warning", "Do not set DFEjSUBTRGCH%d.SBE to 0x%X due to prohibited setting.\n", channel, sbe);
        (*DFEjSUBTRGCHn[channel])["SBE"] = 0;
    }
    if (!((0x0 <= sbt) && (sbt <= 0x2))){
        re_printf("warning", "Do not set DFEjSUBTRGCH%d.SBT to 0x%X due to prohibited setting.\n", channel, sbt);
        (*DFEjSUBTRGCHn[channel])["SBT"] = 0;
    }
    if ((sbfe == 2) && (sbe == 2)){
        re_printf("warning", "Do not set both DFEjSUBTRGCH%d.SBFE bit and DFEjSUBTRGCH%d.SBE bit to 0x%X due to prohibited setting.\n", channel, channel, sbfe);
        (*DFEjSUBTRGCHn[channel])["SBFE"] = 0;
        (*DFEjSUBTRGCHn[channel])["SBE"] = 0;
    }
    return can_run;
}//}}}

bool Cdfe0051_func::CheckSettingDFEjSUBTRG()
{//{{{
    bool can_run = true;
    unsigned int subtmtrg2 = (unsigned int)(*DFEjSUBTRG0)["SUBTMTRG2"];
    unsigned int subtmtrg1 = (unsigned int)(*DFEjSUBTRG0)["SUBTMTRG1"];
    unsigned int subtmtrg0 = (unsigned int)(*DFEjSUBTRG0)["SUBTMTRG0"];

    if (!((0x0 <= subtmtrg2) && (subtmtrg2 <= 0x13))){
        re_printf("warning", "Do not set DFEjSUBTRG0.SUBTMTRG2 to 0x%X due to prohibited setting.\n", subtmtrg2);
        (*DFEjSUBTRG0)["SUBTMTRG2"] = 0;
    }
    if (!((0x0 <= subtmtrg1) && (subtmtrg1 <= 0x13))){
        re_printf("warning", "Do not set DFEjSUBTRG0.SUBTMTRG1 to 0x%X due to prohibited setting.\n", subtmtrg1);
        (*DFEjSUBTRG0)["SUBTMTRG1"] = 0;
    }
    if (!((0x0 <= subtmtrg0) && (subtmtrg0 <= 0x13))){
        re_printf("warning", "Do not set DFEjSUBTRG0.SUBTMTRG0 to 0x%X due to prohibited setting.\n", subtmtrg0);
        (*DFEjSUBTRG0)["SUBTMTRG0"] = 0;
    }
    return can_run;
}//}}}

void Cdfe0051_func::CommonCallbackDFE0051HOHCHn(unsigned int channel)
{//{{{
    unsigned int hofs = (unsigned int)(*DFEjCTLBCHn[channel])["HOFS"];
    // Clear DOEN if in FIR, HOFS = 0, read both HOHCH, HOLCH
    if ((IsFIRSelect(channel)) && (hofs == 0)){
        if (mWasReadHOLCH[channel]){
            (*DFEjSTCHn[channel])["DOEN"] = 0;
            mWasReadHOLCH[channel] = false;
        }else{
            mWasReadHOHCH[channel] = true;
        }
    }
}//}}}

void Cdfe0051_func::CommonCallbackDFE0051HOLCHn(unsigned int channel)
{//{{{
    if (IsFIRSelect(channel)){
        // Clear DOEN if in FIR, HOFS = 0, read both HOHCH, HOLCH
        if ((unsigned int)(*DFEjCTLBCHn[channel])["HOFS"] == 0){
            if (mWasReadHOHCH[channel]){
                (*DFEjSTCHn[channel])["DOEN"] = 0;
                mWasReadHOHCH[channel] = false;
            }else{
                mWasReadHOLCH[channel] = true;
            }
        // Clear DOEN if in FIR, HOFS = 1, read HOLCH
        }else{// hofs == 1
            (*DFEjSTCHn[channel])["DOEN"] = 0;
        }
    }
}//}}}

void Cdfe0051_func::UpdateDFEjST()
{//{{{
    bool has_error = false;
    for (unsigned int channel = 0; channel < mNumChannel; channel++){
        unsigned int valid = (unsigned int)(*DFEjSTCHn[channel])["VALID"];
        unsigned int err = 0;
        if (((unsigned int)(*DFEjSTCHn[channel]) & 0x3F) != 0){
            err = 1;
            has_error = true;
        }
        unsigned int value = (valid << 1) | err;

        std::ostringstream str_chs;
        str_chs << "CH"<<channel<<"S";
        (*DFEjST)[str_chs.str().c_str()] = value;
    }
}//}}}

void Cdfe0051_func::UpdateDFEjEST()
{//{{{
    bool has_error = false;
    for (unsigned int index = 0; index < 3; index++){
        bool cap_err = ((unsigned int)(*DFEjCAPSTCHn[index])["CDOOW"] == 1);
        unsigned int sub_status = (unsigned int)(*DFEjSUBSTCHn[index]);
        bool sub_err = ((sub_status & 0x3F) != 0);

        if (cap_err || sub_err){
            has_error = true;
        }

        std::ostringstream str_cps;
        std::ostringstream str_sbs;
        str_cps << "CP"<<index<<"S";
        str_sbs << "SB"<<index<<"S";
        (*DFEjEST)[str_cps.str().c_str()] = (unsigned int)cap_err;
        (*DFEjEST)[str_sbs.str().c_str()] = (unsigned int)sub_err;
    }
}//}}}

void Cdfe0051_func::cb_DFEjCTLACHn_EN(RegCBstr str)
{//{{{
    bool can_run = true;

    unsigned int cmd = (unsigned int)(*DFEjCTLACHn[str.channel])["CMD"];
    unsigned int caen = (unsigned int)(*DFEjCTLACHn[str.channel])["CAEN"];
    unsigned int aime = (unsigned int)(*DFEjCTLACHn[str.channel])["AIME"];
    unsigned int en = str.data & 0x1;
    unsigned int en_aime = str.data & 0x3;

    unsigned int pre_en = str.pre_data & 0x1;
    unsigned int pre_en_aime = str.pre_data & 0x3;

    unsigned int valid = (unsigned int)(*DFEjSTCHn[str.channel])["VALID"];

    if (en == 0){
        // Not stop now. Wait finish remain task then stop. Cdfe0051 class will update variable at the end of operation.
    }else{// en == 1
        // While EN = 1, cannot writing to other bit
        if (pre_en == 1){
            if (((str.data >> 1) != (str.pre_data >> 1)) || (valid == 1)){
                re_printf("warning", "Do not write to other bits in DFEjCTLACH%d while DFEjCTLACH%d.EN is 0x1 or DFEjSTCH%d.VALID is 0x1.\n", str.channel, str.channel, str.channel);
                (*DFEjCTLACHn[str.channel]) = str.pre_data;
                return;
            }
        }

        can_run = CheckSettingDFEjCTLACHn(str.channel) && can_run;  // Check valid of setting DFEjCTLACHn
        can_run = CheckSettingDFEjCTLBCHn(str.channel) && can_run;  // Check valid of setting DFEjCTLBCHn
        can_run = CheckSettingDFEjTRGCHn(str.channel) && can_run;   // Check valid of settting DFEjTRGCHn
        can_run = CheckSettingDFEjPMITRG() && can_run;              // Check valid of settting DFEjPMITRG
        can_run = CheckSettingDFEjPITRG() && can_run;               // Check valid of settting DFEjPITRG
        can_run = CheckSettingDFEjMITRG() && can_run;               // Check valid of settting DFEjMITRG
        can_run = CheckSettingDFEjFITRG() && can_run;               // Check valid of settting DFEjFITRG
        can_run = CheckSettingDFEjPHUPDCn() && can_run;             // Check valid of settting DFEjPHUPDCn
        can_run = CheckSettingDFEjTMTRGCHn(str.channel) && can_run; // Check valid of settting DFEjTMTRGCHn
        
        mDFE0051AgentController->UpdateChannelEnable(str.channel, can_run);
    }

    // Auto initialize => must put after invoking UpdateChannelEnable() IMPORTANT
    if ((pre_en_aime != en_aime) && (en_aime == 0x3)){
        mDFE0051AgentController->AutoInitialize(str.channel);
    }
}//}}}

void Cdfe0051_func::cb_DFEjCTLBCHn_PRCSA(RegCBstr str)
{//{{{
    unsigned int valid = (unsigned int)(*DFEjSTCHn[str.channel])["VALID"];
    unsigned int en = (unsigned int)(*DFEjCTLACHn[str.channel])["EN"];
    unsigned int doen = (unsigned int)(*DFEjSTCHn[str.channel])["DOEN"];
    unsigned int hofs = (str.data >> 7) & 0x1;
    unsigned int pre_hofs = (str.pre_data >> 7) & 0x1;

    if ((str.data != str.pre_data) && ((valid == 1) || (en == 1))){
        re_printf("warning", "Do not write to DFEjCTLBCH%d while DFEjCTLACH%d.EN is 0x1 or DFEjSTCH%d.VALID is 0x1.\n", str.channel, str.channel, str.channel);
        (*DFEjCTLBCHn[str.channel]) = str.pre_data;
        return;
    }
}//}}}

void Cdfe0051_func::cb_DFEjDOCHn_DFEjDOCHn(RegCBstr str)
{//{{{
    // FIR
    unsigned int hofs = (unsigned int)(*DFEjCTLBCHn[str.channel])["HOFS"];
    if (IsFIRSelect(str.channel)){
        (*DFEjSTCHn[str.channel])["DOEN"] = 0; // Clear DOEN when read DOCH and HOFS = 0
    }

    // IIR
    if (IsIIRSelect(str.channel)){
        (*DFEjSTCHn[str.channel])["DOEN"] = 0; // Clear DOEN when read DOCH
    }
}//}}}

void Cdfe0051_func::cb_DFEjPHINDCHn_PHIOF(RegCBstr str)
{//{{{
    // PHIOW always 0 in peak-hold index retention mode.
    if ((unsigned int)(*DFEjCTLBCHn[str.channel])["PICS"] == 1){
        (*DFEjPHINDCHn[str.channel])["PHIOW"] = 0;
    }
}//}}}

void Cdfe0051_func::cb_DFEjCLRSTCHn_CLRDIOW(RegCBstr str)
{//{{{
    if (str.is_wr){
        // CLRPHIOW bit
        if (((str.data >> 15)&0x1) == 1){
            (*DFEjPHINDCHn[str.channel])["PHIOW"] = 0;
            (*DFEjPH2IND0)["PH2IOW"] = 0;
            (*DFEjPH3IND0)["PH3IOW"] = 0;
        }
        // CLRPHE bit
        if (((str.data >> 10)&0x1) == 1){
            (*DFEjSTCHn[str.channel])["PHE"] = 0;
        }
        // CLRCND bit
        if (((str.data >> 9)&0x1) == 1){
            (*DFEjSTCHn[str.channel])["CND"] = 0;
        }
        // CLRDOEN bit
        if (((str.data >> 8)&0x1) == 1){
            (*DFEjSTCHn[str.channel])["DOEN"] = 0;
        }
        // CLRCER bit
        if (((str.data >> 5)&0x1) == 1){
            (*DFEjSTCHn[str.channel])["CER"] = 0;
        }
        // CLRAER bit
        if (((str.data >> 4)&0x1) == 1){
            (*DFEjSTCHn[str.channel])["AER"] = 0;
        }
        // CLRMER bit
        if (((str.data >> 3)&0x1) == 1){
            (*DFEjSTCHn[str.channel])["MER"] = 0;
        }
        // CLRGER bit
        if (((str.data >> 2)&0x1) == 1){
            (*DFEjSTCHn[str.channel])["GER"] = 0;
        }
        // CLRDOOW bit
        if (((str.data >> 1)&0x1) == 1){
            (*DFEjSTCHn[str.channel])["DOOW"] = 0;
        }
        // CLRDIOW bit
        if ((str.data&0x1) == 1){
            (*DFEjSTCHn[str.channel])["DIOW"] = 0;
        }
    }else{
        (*DFEjCLRSTCHn[str.channel]) = 0;// Read always 0
    }
}//}}}

void Cdfe0051_func::cb_DFEjDI_DI(RegCBstr str)
{//{{{
    mDFE0051AgentController->StartSWInput();
}//}}}

void Cdfe0051_func::cb_DFEjTRG_TRGA(RegCBstr str)
{//{{{
    if (str.is_wr){
        if ((str.data & 0x1) == 1){
            mDFE0051AgentController->IssueSWTrigger();
        }
    }else{
        (*DFEjTRG) = 0; // Always read 0
    }
}//}}}

void Cdfe0051_func::cb_DFEjHOLCH0_DFEjHOLCH0(RegCBstr str)
{//{{{
    CommonCallbackDFE0051HOLCHn(0);
}//}}} 

void Cdfe0051_func::cb_DFEjHOHCH0_GURD(RegCBstr str)
{//{{{
    CommonCallbackDFE0051HOHCHn(0);
}//}}} 

void Cdfe0051_func::cb_DFEjHOLCH2_DFEjHOLCH2(RegCBstr str)
{//{{{
    CommonCallbackDFE0051HOLCHn(2);
}//}}} 

void Cdfe0051_func::cb_DFEjHOHCH2_GURD(RegCBstr str)
{//{{{
    CommonCallbackDFE0051HOHCHn(2);
}//}}} 

void Cdfe0051_func::cb_DFEjHOLCH4_DFEjHOLCH4(RegCBstr str)
{//{{{
    CommonCallbackDFE0051HOLCHn(4);
}//}}} 

void Cdfe0051_func::cb_DFEjHOHCH4_GURD(RegCBstr str)
{//{{{
    CommonCallbackDFE0051HOHCHn(4);
}//}}} 

void Cdfe0051_func::cb_DFEjHOLCH6_DFEjHOLCH6(RegCBstr str)
{//{{{
    CommonCallbackDFE0051HOLCHn(6);
}//}}} 

void Cdfe0051_func::cb_DFEjHOHCH6_GURD(RegCBstr str)
{//{{{
    CommonCallbackDFE0051HOHCHn(6);
}//}}} 

void Cdfe0051_func::cb_DFEjHOLCH8_DFEjHOLCH8(RegCBstr str)
{//{{{
    CommonCallbackDFE0051HOLCHn(8);
}//}}} 

void Cdfe0051_func::cb_DFEjHOHCH8_GURD(RegCBstr str)
{//{{{
    CommonCallbackDFE0051HOHCHn(8);
}//}}} 

void Cdfe0051_func::cb_DFEjHOLCH10_DFEjHOLCH10(RegCBstr str)
{//{{{
    CommonCallbackDFE0051HOLCHn(10);
}//}}} 

void Cdfe0051_func::cb_DFEjHOHCH10_GURD(RegCBstr str)
{//{{{
    CommonCallbackDFE0051HOHCHn(10);
}//}}} 

void Cdfe0051_func::cb_DFEjHOLCH12_DFEjHOLCH12(RegCBstr str)
{//{{{
    CommonCallbackDFE0051HOLCHn(12);
}//}}} 

void Cdfe0051_func::cb_DFEjHOHCH12_GURD(RegCBstr str)
{//{{{
    CommonCallbackDFE0051HOHCHn(12);
}//}}} 

void Cdfe0051_func::cb_DFEjHOLCH14_DFEjHOLCH14(RegCBstr str)
{//{{{
    CommonCallbackDFE0051HOLCHn(14);
}//}}} 

void Cdfe0051_func::cb_DFEjHOHCH14_GURD(RegCBstr str)
{//{{{
    CommonCallbackDFE0051HOHCHn(14);
}//}}} 

void Cdfe0051_func::cb_DFEjPH23CCTL0_PEN(RegCBstr str)
{//{{{
    unsigned int chs = (str.data >> 4) & 0xF;
    unsigned int pfmt = (str.data >> 3) & 0x1;
    unsigned int pen = str.data & 0x1;
    unsigned int cn3slb2 = (unsigned int)(*DFEjPH3CTL0)["CN3SLB2"];
    unsigned int ph3slb2 = (unsigned int)(*DFEjPH3CTL0)["PH3SLB2"];
    unsigned int ofsl2 = (unsigned int)(*DFEjPH2CTL0)["OFSL"];
    unsigned int cn2slb2 = (unsigned int)(*DFEjPH2CTL0)["CN2SLB2"];
    unsigned int ph2slb2 = (unsigned int)(*DFEjPH2CTL0)["PH2SLB2"];
    unsigned int ofsl3 = (unsigned int)(*DFEjPH3CTL0)["OFSL"];

    unsigned int pre_chs = (str.pre_data >> 4) & 0xF;
    unsigned int pre_pfmt = (str.pre_data >> 3) & 0x1;
    unsigned int pre_pen = str.pre_data & 0x1;
    
    // Disable PH23, clear PH23ST
    if (pen == 0){
        unsigned int chs = (unsigned int)(*DFEjPH23CCTL0)["CHS"];
        (*DFEjSTCHn[chs])["PH23ST"] = 0;
    }else{// pen == 1 => check valid setting related to PH23
        // DFEjPH23CCTL0 - Check valid setting when PEN = 1
        if (chs > mNumChannel){
            re_printf("warning", "Do not set DFEjPH23CCTL0.CHS to 0x%X due to prohibited setting.\n", chs);
            (*DFEjPH23CCTL0)["CHS"] = 0;
        }
        // DFEjPH2CTL0 - Check valid setting when PEN = 1
        if (!((0x0 <= ofsl2) && (ofsl2 <= 0x2))){
            re_printf("warning", "Do not set DFEjPH2CTL0.OFSL to 0x%X due to prohibited setting.\n", ofsl2);
            (*DFEjPH2CTL0)["OFSL"] = 0;
        }
        if (!((0x0 <= ph2slb2) && (ph2slb2 <= 0x3))){
            re_printf("warning", "Do not set DFEjPH2CTL0.PH2SLB2 to 0x%X due to prohibited setting.\n", ph2slb2);
            (*DFEjPH2CTL0)["PH2SLB2"] = 0;
        }
        if (!((0x0 <= cn2slb2) && (cn2slb2 <= 0x4))){
            re_printf("warning", "Do not set DFEjPH2CTL0.CN2SLB2 to 0x%X due to prohibited setting.\n", cn2slb2);
            (*DFEjPH2CTL0)["CN2SLB2"] = 0;
        }
        // DFEjPH3CTL0 - Check valid setting when PEN = 1
        if (!((0x0 <= ofsl3) && (ofsl3 <= 0x2))){
            re_printf("warning", "Do not set DFEjPH3CTL0.OFSL to 0x%X due to prohibited setting.\n", ofsl3);
            (*DFEjPH3CTL0)["OFSL"] = 0;
        }
        if (!((0x0 <= ph3slb2) && (ph3slb2 <= 0x3))){
            re_printf("warning", "Do not set DFEjPH3CTL0.PH3SLB2 to 0x%X due to prohibited setting.\n", ph3slb2);
            (*DFEjPH3CTL0)["PH3SLB2"] = 0;
        }
        if (!((0x0 <= cn3slb2) && (cn3slb2 <= 0x4))){
            re_printf("warning", "Do not set DFEjPH3CTL0.CN3SLB2 to 0x%X due to prohibited setting.\n", cn3slb2);
            (*DFEjPH3CTL0)["CN3SLB2"] = 0;
        }
        // While PEN = 1 (operating PH23), cannot write other bits
        if (pre_pen == 1){
            if (chs != pre_chs){
                re_printf("warning", "Do not change DFEjPH23CCTL0.CHS bit while PH23 is operating.\n");
                (*DFEjPH23CCTL0)["CHS"] = pre_chs;
            }
            if (pfmt != pre_pfmt){
                re_printf("warning", "Do not change DFEjPH23CCTL0.PFMT bit while PH23 is operating.\n");
                (*DFEjPH23CCTL0)["PFMT"] = pre_pfmt;
            }
        }
    }
}//}}}

void Cdfe0051_func::cb_DFEjPH2CTL0_CN2SLB2(RegCBstr str)
{//{{{ 
    unsigned int pen = (unsigned int)(*DFEjPH23CCTL0)["PEN"];

    if ((str.data != str.pre_data) && (pen == 1)){
        re_printf("warning", "Do not write to DFEjPH2CTL0 while DFEjPH23CCTL0.PEN is 0x1.\n");
        (*DFEjPH2CTL0) = str.pre_data;
    }
}//}}}

void Cdfe0051_func::cb_DFEjPH3CTL0_CN3SLB2(RegCBstr str)
{//{{{ 
    unsigned int pen = (unsigned int)(*DFEjPH23CCTL0)["PEN"];

    if ((str.data != str.pre_data) && (pen == 1)){
        re_printf("warning", "Do not write to DFEjPH3CTL0 while DFEjPH23CCTL0.PEN is 0x1.\n");
        (*DFEjPH3CTL0) = str.pre_data;
    }
}//}}}

void Cdfe0051_func::cb_DFEjSUBCTLCHn_SEN(RegCBstr str)
{//{{{
    bool can_run = true;
    unsigned int minch = (unsigned int)(*DFEjSUBCTLCHn[str.channel])["MINCH"];
    unsigned int subch = (unsigned int)(*DFEjSUBCTLCHn[str.channel])["SUBCH"];
    unsigned int en_minuend = (unsigned int)(*DFEjCTLACHn[minch])["EN"];
    unsigned int en_subtrahend = (unsigned int)(*DFEjCTLACHn[subch])["EN"];
    unsigned int sen = str.data & 0x1;
    
    if (sen == 0){
        // Not stop now. Cdfe0051 will stop after finish operation (for channel)
    }else{// sen == 1
        if (((en_minuend == 1) || (en_subtrahend == 1)) && ((str.pre_data >> 1) != (str.data >> 1))){
            re_printf("warning", "Do not write to DFEjSUBCTLCH%d (other than SEN bit) while DFEjCTLACHn.EN of minuend or subtarhend is 0x1.\n", str.channel);
            (*DFEjSUBCTLCHn[str.channel]) = str.pre_data;
            return;
        }
    }
}//}}}

void Cdfe0051_func::cb_DFEjSUBDOCHn_DFEjSUBDOCHn(RegCBstr str)
{//{{{
    (*DFEjSUBSTCHn[str.channel])["SDOEN"] = 0;
}//}}}

void Cdfe0051_func::cb_DFEjSUBCLRSTCHn_CLRSDOOW(RegCBstr str)
{//{{{
    if (str.is_wr){
        // CLRSDOEN bit
        if (((str.data >> 8)&0x1) == 1){
            (*DFEjSUBSTCHn[str.channel])["SDOEN"] = 0;
        }
        // CLRSCER bit
        if (((str.data >> 5)&0x1) == 1){
            (*DFEjSUBSTCHn[str.channel])["SCER"] = 0;
        }
        // CLRSGER bit
        if (((str.data >> 2)&0x1) == 1){
            (*DFEjSUBSTCHn[str.channel])["SGER"] = 0;
        }
        // CLRSDOOW bit
        if (((str.data >> 1)&0x1) == 1){
            (*DFEjSUBSTCHn[str.channel])["SDOOW"] = 0;
        }
    }else{
        (*DFEjSUBCLRSTCHn[str.channel]) = 0; // Always read 0
    }
}//}}}

void Cdfe0051_func::cb_DFEjSUBTRGCHn_SBT(RegCBstr str)
{//{{{
    unsigned int sbe = (unsigned int)(*DFEjSUBTRGCHn[str.channel])["SBE"];
    unsigned int pre_sbe = (str.pre_data >> 6) & 0x3;
    if ((pre_sbe == 0) && (sbe != 0)){
        mDFE0051AgentController->DisableSubtractionFlag(str.channel);
    }
}//}}}

void Cdfe0051_func::cb_DFEjCAPCTLCHn_CEN(RegCBstr str)
{//{{{
    unsigned int cen = str.data & 0x1;
    unsigned int pre_cen = str.pre_data & 0x1;
    unsigned int capch = (str.data >> 4) & 0xF;

    if (cen == 0){
        // Not stop now. Cdfe0051 will stop after finish operation (channel)
    }else{// cen == 1
        if ((pre_cen == 1) && ((str.pre_data >> 1) != (str.data >> 1))){
            re_printf("warning", "Do not write to other bits in DFEjCAPCTLCH%d while DFEjCAPCTLCH%d.CEN is 0x1.\n", str.channel, str.channel);
            (*DFEjCAPCTLCHn[str.channel]) = str.pre_data;
            return;
        }
        mDFE0051AgentController->UpdateCaptureEnable(str.channel, true);
    }
}//}}}

void Cdfe0051_func::cb_DFEjCAPDOCHn_DFEjCAPDOCHn(RegCBstr str)
{//{{{
    (*DFEjCAPSTCHn[str.channel])["CDOEN"] = 0;
}//}}}

void Cdfe0051_func::cb_DFEjCAPCLRSTCHn_CLRCDOOW(RegCBstr str)
{//{{{
    if (str.is_wr){
        // CLRCDOEN bit
        if (((str.data >> 8)&0x1) == 1){
            (*DFEjCAPSTCHn[str.channel])["CDOEN"] = 0;
        }
        // CLRCDOOW bit
        if (((str.data >> 1)&0x1) == 1){
            (*DFEjCAPSTCHn[str.channel])["CDOOW"] = 0;
        }
    }else{
        (*DFEjCAPCLRSTCHn[str.channel]) = 0; // Always read 0
    }
}//}}}

// vim600: set foldmethod=marker :
