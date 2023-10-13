// ---------------------------------------------------------------------
// $Id: MSPI000_Func.cpp,v 1.16 2020/03/11 00:52:43 ducla Exp $
//
// Copyright(c) 2017-2020 Renesas Electronics Corporation
// Copyright(c) 2017-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "MSPI000_Func.h"
#include "MSPI000_AgentController.h"
//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
///*********************************
/// Function     : MSPI000_Func
/// Description  : Constructor MSPI000_Func
///*********************************
MSPI000_Func::MSPI000_Func (std::string name, unsigned int ch_num, unsigned int cs_num, MSPI000_AgentController* AgentController):
                Cmspi000_regif(name, 32, ch_num , cs_num)
                ,BusSlaveFuncIf()
{//{{{
    sc_assert(AgentController != NULL);
    mpAgentController = AgentController;
    mpAgentController->CommandInit(name);
    mModuleName = name;
    mNumChannel = ch_num;
    Cmspi000_regif::set_instance_name(name);
    // Initialize
    Initialize();
    mDMEM = new unsigned char[emDMEMSize];
    memset(mDMEM, 0, emDMEMSize);
}//}}}
///*********************************
/// Function     : ~MSPI000_Func
/// Description  : Destructor MSPI000_Func
///*********************************
MSPI000_Func::~MSPI000_Func()
{//{{{
    if (mDMEM != NULL) {
        delete [] mDMEM;
    }
}//}}}
////////////////////////////////////////////////////////////
///*********************************
/// Function     : Initialize
/// Description  : Initialize variables
///*********************************
void MSPI000_Func::Initialize(void)
{//{{{
    mIPEN = false;
    mIsMaster = true;
    mIsLoopBack = false;
    mIsDataConsistency = false;
    for (unsigned int channel = 0; channel < MAX_CH_NUM; channel++){
        mIsCHEN[channel] = false;
        mIsSafeSPI[channel] = false;
        mSizes[channel] = 1;
        mpCom[channel] = emNo_Comunication;
        mpMem[channel] = emDirectMem;
        mMSPInSCKbaudarate[channel] = 0.5;
        mStartTX[channel] = 0;// Just mean offset
        mCurTX[channel] = 0;
        mCurTX1[channel] = 0;
        mStartRX[channel] = 0;// Just mean offset
        mCurRX[channel] = 0;
        mCurRX1[channel] = 0;
    }
    mMemSize = 128;
    mAccessRAM = true;
    for (unsigned int channel = 0; channel < MAX_CH_NUM; channel++){
        mHalfFIFOFilled[channel] = false;
        mCount_read_rx[channel] = 0;
    }
}//}}}
/// Process reset function
///*********************************
/// Function     : EnableReset
/// Description  : Enable Reset
///*********************************
void MSPI000_Func::EnableReset (const bool is_active)
{//{{{
    if (is_active){
        Initialize();
    }
    Cmspi000_regif::EnableReset(is_active);
}//}}}
/// Handler for register IF
///*********************************
/// Function     : RegisterHandler 
/// Description  : Register Handler
///*********************************
void MSPI000_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cmspi000_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Cmspi000_regif class
            msg_return = Cmspi000_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}
///*********************************
/// Function     : read 
/// Description  : Read API (target socket)
///*********************************
void MSPI000_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int full_addr = (unsigned int)trans.get_address();
    unsigned int addr = (unsigned int)trans.get_address() & 0x1FFF;
    unsigned int size = (unsigned int)trans.get_data_length();
    memset(trans.get_data_ptr(), 0, size);
    
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    // Zero clock or reset active
    if ( (!mpAgentController->CheckClockPeriod("PCLK")) || (mpAgentController->GetResetStatus())) {
        if (pExtInfo != NULL) {
            if (!(debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID))) {
                re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
                return;
            }
        }
        else {
            re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
            return;
        }
    }

    // Read register - note: process for non-existed register (num channel <= 16)
    if (addr < emStartOffsetDMEM){
        // Check non-existed reserved register
        if ((addr == 0xC)
            || (addr >= 0x14 && addr <= 0x7C)
            || ((addr - 0x94) % 0x80 == 0)
            || ((addr - 0xC4) % 0x80 == 0)
            || ((addr - 0xC8) % 0x80 == 0)
            || ((addr - 0xD4) % 0x80 == 0)
            || ((addr - 0xF0) % 0x80 == 0)
            || ((addr - 0xF4) % 0x80 == 0)
            || ((addr - 0xF8) % 0x80 == 0)
            || ((addr - 0xFC) % 0x80 == 0)
        ){
            re_printf("warning", "Should not read from the reservation area 0x%08X.\n", full_addr);
        // Normal
        }else{
            if (!debug) {
                reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
            } else {
                reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
            }
        }
    // Read DMEM - support read all size 1/2/4
    }else if ((emStartOffsetDMEM <= addr) && (addr < (emStartOffsetDMEM + mMemSize * 4))){
        if (CheckValidAccessMem(emDMEMType, addr, size, false, debug)){
            memcpy(trans.get_data_ptr(), mDMEM + addr - emStartOffsetDMEM, size);
        }else{
            re_printf("warning", "Cannot read from address 0x%08X in Data memory due to invalid access size %d byte(s) or invalid address.\n", full_addr, size);
        }
    }else{
            re_printf("warning", "Should not read from the reservation area 0x%08X.\n", full_addr);
    }
}//}}}
///*********************************
/// Function     : write 
/// Description  : Write API (target socket)
///*********************************
void MSPI000_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    if (!mpAgentController->CheckClockPeriod("PCLK")) {
        // Software debugger
        if ((pExtInfo != NULL)) {
            if ( !( debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID)) ) {
                re_printf("warning", "Cannot access register when clock is 0.\n");
                return;
            }
        }
        else {
            re_printf("warning", "Cannot access register when clock is 0.\n");
            return;
        }
    }
    unsigned int full_addr = (unsigned int)trans.get_address();
    unsigned int addr = (unsigned int)trans.get_address() & 0x1FFF;
    unsigned int size = (unsigned int)trans.get_data_length();
    // Write register - note: process for non-existed register (num channel <= 16)
    if (addr < emStartOffsetDMEM){
        // Check non-existed
        if ((addr == 0xC)
            || (addr >= 0x14 && addr <= 0x7C)
            || ((addr - 0x94) % 0x80 == 0)
            || ((addr - 0xC4) % 0x80 == 0)
            || ((addr - 0xC8) % 0x80 == 0)
            || ((addr - 0xD4) % 0x80 == 0)
            || ((addr - 0xF0) % 0x80 == 0)
            || ((addr - 0xF4) % 0x80 == 0)
            || ((addr - 0xF8) % 0x80 == 0)
            || ((addr - 0xFC) % 0x80 == 0)
        ){
            re_printf("warning", "Should not write to the reservation area 0x%08X.\n", full_addr);
        // Normal
        }else{
            if (!debug) {
                reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
            } else {
                reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
            }
        }
    // Write DMEM - support read all size 1/2/4
    }else if ((emStartOffsetDMEM <= addr) && (addr < (emStartOffsetDMEM + mMemSize * 4))){
        if (CheckValidAccessMem(emDMEMType, addr, size, true, debug)){
            memcpy(mDMEM + addr - emStartOffsetDMEM, trans.get_data_ptr(), size);
        }else{
            re_printf("warning", "Cannot write to address 0x%08X in Data memory due to invalid access size %d byte(s) or invalid address.\n", full_addr, size);
        }
    }else{
        re_printf("warning", "Should not write to the reservation area 0x%08X.\n", full_addr);
    }
}//}}}
///*********************************
/// Function     : CheckValidAccessMem
/// Description  : Check Valid Access Mem
///*********************************
bool MSPI000_Func::CheckValidAccessMem(eMEMInfo mem_type, unsigned int addr, unsigned int size, bool is_wr, bool debug)
{//{{{
    bool ret = false;
    if (!mAccessRAM) {
        re_printf("warning", "The access signal to the RAM is deactivated.");
        return ret;
    }
    sc_assert(mem_type == emDMEMType);
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
    return ret;
}//}}}
///*********************************
/// Function     : SetDataMem
/// Description  : Set Data Mem
///*********************************
bool MSPI000_Func::SetDataMem(unsigned int offset, unsigned int value, unsigned int size)
{//{{{
     bool ret = false;
     if ((offset + size) <= mMemSize * 4){
         if (mAccessRAM) {
             memcpy(mDMEM + offset, &value, size);
             ret = true;
         } else {
             ret = false;
             re_printf("warning", "The access signal to the RAM is deactivated.");
         }
     } else {
         ret = false;
         re_printf("warning", "The access to the RAM is out of boundary MEM_SIZE = %d", mMemSize);
     }
     return ret;
}//}}}
///*********************************
/// Function     : GetDataMem
/// Description  : Get Data Mem
///*********************************
bool MSPI000_Func::GetDataMem(unsigned int offset, unsigned int& data, unsigned int size)
{//{{{
     bool ret = false;
     if ((offset + size) <= mMemSize * 4){
         if (mAccessRAM) {
             memcpy(&data, mDMEM + offset, size);
             ret = true;
         } else {
             ret = false;
             re_printf("warning", "The access signal to the RAM is deactivated.");
         }
     } else {
         ret = false;
         re_printf("warning", "The access to the RAM is out of boundary MEM_SIZE = %d", mMemSize);
     }
     return ret;
}//}}}
///*********************************
/// Function     : SetMemSize
/// Description  : Set MSPI_MEM_SIZE
///*********************************
void MSPI000_Func::SetMemSize (unsigned int id)
{//{{{
    if ( id > 6) {
        re_printf("warning", "Sets a non-existent RAM area, the access signal to the RAM is deactivated.");
        mAccessRAM = false;
        id = 3; 
    } else {
        mAccessRAM = true;
    }
    if (id == 0) {
        mMemSize = 0;
    } else { 
        mMemSize = 32 * (1 << (id-1));
    }
}//}}}
///*********************************
/// Function     : Get_SPI_Control_Setting
/// Description  : Get SPI_Control Setting
///*********************************
unsigned int MSPI000_Func::Get_SPI_Control_Setting(unsigned int chid)
{//{{{
    unsigned int spi_control_val = 0;
    spi_control_val |= GetMSPInMSSEL()       << 14;          //MSTR
    spi_control_val |= GetMSPInSAMP()        << 13;          //SAMP
    spi_control_val |= GetMSPInCPOLm(chid)   << 12;          //CPOL
    spi_control_val |= GetMSPInCPHAm(chid)   << 11;          //CPHA
    spi_control_val |= GetMSPInDIRm(chid)    << 10;          //DIR

    spi_control_val |= GetMSPInFLENm(chid);                  //SIZE
    return spi_control_val;
}//}}}
//Get/Set Registers
///*********************************
/// Function     : GetMSPInCSP
/// Description  : GetMSPInCSP
///*********************************
unsigned int MSPI000_Func::GetMSPInCSP()
{//{{{
    return (unsigned int) (*MSPInCTL1)["MSPInCSP"];
}//}}}
///*********************************
/// Function     : GetMSPInCKR
/// Description  : GetMSPInCKR
///*********************************
unsigned int MSPI000_Func::GetMSPInCKR()
{//{{{
    return (unsigned int) (*MSPInCTL1)["MSPInCKR"];
}//}}}
///*********************************
/// Function     : GetMSPInSOLS
/// Description  : GetMSPInSOLS
///*********************************
unsigned int MSPI000_Func::GetMSPInSOLS()
{//{{{
    return (unsigned int) (*MSPInCTL1)["MSPInSOLS"];
}//}}}
///*********************************
/// Function     : GetMSPInMSSEL
/// Description  : GetMSPInMSSEL
///*********************************
unsigned int MSPI000_Func::GetMSPInMSSEL()
{//{{{
    return (unsigned int) (*MSPInCTL1)["MSPInMSSEL"];
}//}}}
///*********************************
/// Function     : GetMSPInSAMP
/// Description  : GetMSPInSAMP
///*********************************
unsigned int MSPI000_Func::GetMSPInSAMP()
{//{{{
    return (unsigned int) (*MSPInCTL1)["MSPInSAMP"];
}//}}}
///*********************************
/// Function     : GetMSPInCSIE
/// Description  : GetMSPInCSIE
///*********************************
unsigned int MSPI000_Func::GetMSPInCSIE()
{//{{{
    return (unsigned int) (*MSPInCTL1)["MSPInCSIE"];
}//}}}
///*********************************
/// Function     : GetMSPInCPOLm
/// Description  : GetMSPInCPOLm
///*********************************
unsigned int MSPI000_Func::GetMSPInCPOLm(unsigned int chid)
{//{{{
    return (unsigned int) (*MSPInCFGm1[chid])["MSPInCPOLm"];
}//}}}
///*********************************
/// Function     : GetMSPInCPHAm
/// Description  : GetMSPInCPHAm
///*********************************
unsigned int MSPI000_Func::GetMSPInCPHAm(unsigned int chid)
{//{{{
    return (unsigned int) (*MSPInCFGm1[chid])["MSPInCPHAm"];
}//}}}
///*********************************
/// Function     : GetMSPInDIRm
/// Description  : GetMSPInDIRm
///*********************************
unsigned int MSPI000_Func::GetMSPInDIRm(unsigned int chid)
{//{{{
    return (unsigned int) (*MSPInCFGm1[chid])["MSPInDIRm"];
}//}}}
///*********************************
/// Function     : GetMSPInFLENm
/// Description  : GetMSPInFLENm
///*********************************
unsigned int MSPI000_Func::GetMSPInFLENm(unsigned int chid)
{//{{{
    return (unsigned int) (*MSPInCFGm2[chid])["MSPInFLENm"];
}//}}}
///*********************************
/// Function     : GetMSPInIEREm
/// Description  : GetMSPInIEREm
///*********************************
unsigned int MSPI000_Func::GetMSPInIEREm(unsigned int chid)
{//{{{
    return (unsigned int) (*MSPInCFGm0[chid])["MSPInIEREm"];
}//}}}
///*********************************
/// Function     : GetMSPInIFEEm
/// Description  : GetMSPInIFEEm
///*********************************
unsigned int MSPI000_Func::GetMSPInIFEEm(unsigned int chid)
{//{{{
    return (unsigned int) (*MSPInCFGm0[chid])["MSPInIFEEm"];
}//}}}
///*********************************
/// Function     : GetMSPInIRXEm
/// Description  : GetMSPInIRXEm
///*********************************
unsigned int MSPI000_Func::GetMSPInIRXEm(unsigned int chid)
{//{{{
    return (unsigned int) (*MSPInCFGm0[chid])["MSPInIRXEm"];
}//}}}
///*********************************
/// Function     : GetMSPInITXEm
/// Description  : GetMSPInITXEm
///*********************************
unsigned int MSPI000_Func::GetMSPInITXEm(unsigned int chid)
{//{{{
    return (unsigned int) (*MSPInCFGm0[chid])["MSPInITXEm"];
}//}}}
///*********************************
/// Function     : GetMSPInCSRm
/// Description  : GetMSPInCSRm
///*********************************
unsigned int MSPI000_Func::GetMSPInCSRm(unsigned int chid)
{//{{{
    return (unsigned int) (*MSPInSSELm[chid])["MSPInCSRm"];
}//}}}
///*********************************
/// Function     : GetMSPInCFSETm
/// Description  : GetMSPInCFSETm
///*********************************
unsigned int MSPI000_Func::GetMSPInCFSETm(unsigned int chid)
{//{{{
    return (unsigned int) (*MSPInCFSETm[chid])["MSPInCFSETm"];
}//}}}
///*********************************
/// Function     : GetMSPInLOCKm
/// Description  : GetMSPInLOCKm
///*********************************
unsigned int MSPI000_Func::GetMSPInLOCKm(unsigned int chid)
{//{{{
    return (unsigned int) (*MSPInCFGm0[chid])["MSPInLOCKm"];
}//}}}
///*********************************
/// Function     : GetMSPInJOBENm
/// Description  : GetMSPInJOBENm
///*********************************
unsigned int MSPI000_Func::GetMSPInJOBENm(unsigned int chid)
{//{{{
    return (unsigned int) (*MSPInSSELm[chid])["MSPInJOBENm"];
}//}}}
///*********************************
/// Function     : GetMSPInPSm
/// Description  : GetMSPInPSm
///*********************************
unsigned int MSPI000_Func::GetMSPInPSm(unsigned int chid)
{//{{{
    return (unsigned int) (*MSPInCFGm1[chid])["MSPInPSm"];
}//}}}
///*********************************
/// Function     : GetMSPInDECHKm
/// Description  : GetMSPInDECHKm
///*********************************
unsigned int MSPI000_Func::GetMSPInDECHKm(unsigned int chid)
{//{{{
    return (unsigned int) (*MSPInCFGm1[chid])["MSPInDECHKm"];
}//}}}
///*********************************
/// Function     : GetMSPInSAFSm
/// Description  : GetMSPInSAFSm
///*********************************
unsigned int MSPI000_Func::GetMSPInSAFSm(unsigned int chid)
{//{{{
    return (unsigned int) (*MSPInCFGm1[chid])["MSPInSAFSm"];
}//}}}
///*********************************
/// Function     : GetMSPInSAFCMm
/// Description  : GetMSPInSAFCMm
///*********************************
unsigned int MSPI000_Func::GetMSPInSAFCMm(unsigned int chid)
{//{{{
    return (unsigned int) (*MSPInCFGm1[chid])["MSPInSAFCMm"];
}//}}}
///*********************************
/// Function     : GetMSPInCFCNTm
/// Description  : GetMSPInCFCNTm
///*********************************
unsigned int MSPI000_Func::GetMSPInCFCNTm(unsigned int chid)
{//{{{
    return (unsigned int) (*MSPInCFCNTm[chid])["MSPInCFCNTm"];
}//}}}
///*********************************
/// Function     : GetMSPInFCCEm
/// Description  : GetMSPInFCCEm
///*********************************
unsigned int MSPI000_Func::GetMSPInFCCEm(unsigned int chid)
{//{{{
    return (unsigned int) (*MSPInCFGm0[chid])["MSPInFCCEm"];
}//}}}
///*********************************
/// Function     : GetMSPInCSRIm
/// Description  : GetMSPInCSRIm
///*********************************
unsigned int MSPI000_Func::GetMSPInCSRIm(unsigned int chid)
{//{{{
    return (unsigned int) (*MSPInCFGm1[chid])["MSPInCSRIm"];
}//}}}
///*********************************
/// Function     : GetMSPInRXRQFm
/// Description  : GetMSPInRXRQFm
///*********************************
unsigned int MSPI000_Func::GetMSPInRXRQFm(unsigned int chid)
{//{{{
    return (unsigned int) (*MSPInCSTRm[chid])["MSPInRXRQFm"];
}//}}}
///*********************************
/// Function     : SetMSPInTXRQFm
/// Description  : SetMSPInTXRQFm
///*********************************
void MSPI000_Func::SetMSPInTXRQFm(unsigned int chid, unsigned int value)
{//{{{
    (*MSPInCSTRm[chid])["MSPInTXRQFm"] = value;
}//}}}
///*********************************
/// Function     : SetMSPInRXRQFm
/// Description  : SetMSPInRXRQFm
///*********************************
void MSPI000_Func::SetMSPInRXRQFm(unsigned int chid, unsigned int value)
{//{{{
    (*MSPInCSTRm[chid])["MSPInRXRQFm"] = value;
    if (value != 0) {
        if (mpMem[chid] == emFixedFifoMem) {
            unsigned int size = (unsigned int)(*MSPInCFGm4[chid])["MSPInSIZEm"];
            mCount_read_rx[chid] = (8 << size) / 2; //a half of FIFO stage
        } else {                                 //(mpMem[chid] == emDirectMem)
            mCount_read_rx[chid] = 1;
        }
    }
}//}}}
///*********************************
/// Function     : SetMSPInCFCNTm
/// Description  : SetMSPInCFCNTm
///*********************************
void MSPI000_Func::SetMSPInCFCNTm(unsigned int chid,  unsigned int value)
{//{{{
    (*MSPInCFCNTm[chid])["MSPInCFCNTm"] = value;
}//}}}
///*********************************
/// Function     : SetMSPInRXDAm0
/// Description  : SetMSPInRXDAm0
///*********************************
void MSPI000_Func::SetMSPInRXDAm0(unsigned int chid, unsigned int value)
{//{{{
    (*MSPInFSRXm[chid])["MSPInRXDAm0"] = value & 0xFFFF;
    (*MSPInRXDAm0[chid])["MSPInRXDAm0"] = value;
}//}}}
///*********************************
/// Function     : SetMSPInCESTm
/// Description  : SetMSPInCESTm
///*********************************
void MSPI000_Func::SetMSPInCESTm(unsigned int chid, bool ovree, bool ovwre, bool ovrue, bool dce, bool ce, bool pe)
{//{{{
    if(ovree){
        (*MSPInCESTm[chid])["MSPInOVREEm"] = 1;
    }else{
        (*MSPInCESTm[chid])["MSPInOVREEm"] = 0;
    }
    if(ovwre){
        (*MSPInCESTm[chid])["MSPInOVWREm"] = 1;
    }else{
        (*MSPInCESTm[chid])["MSPInOVWREm"] = 0;
    }
    if(ovrue){
        (*MSPInCESTm[chid])["MSPInOVRUEm"] = 1;
    }else{
        (*MSPInCESTm[chid])["MSPInOVRUEm"] = 0;
    }
    if(dce){
      (*MSPInCESTm[chid])["MSPInDCEm"] = 1;
    }else{
      (*MSPInCESTm[chid])["MSPInDCEm"] = 0;
    }
    if(ce){
        (*MSPInCESTm[chid])["MSPInCEm"] = 1;
    }else{
        (*MSPInCESTm[chid])["MSPInCEm"] = 0;
    }
    if(pe){
        (*MSPInCESTm[chid])["MSPInPEm"] = 1;
    }else{
        (*MSPInCESTm[chid])["MSPInPEm"] = 0;
    }
}//}}}
///*********************************
/// Function     : SetMSPInCHENm
/// Description  : SetMSPInCHENm
///*********************************
void MSPI000_Func::SetMSPInCHENm(unsigned int chid,  unsigned int value)
{//{{{
    (*MSPInCSTRm[chid])["MSPInCHENm"] = value;
    (*MSPInCSTRm[chid])["MSPInACTFm"] = 0;
    (*MSPInCSTRm[chid])["MSPInTXRQFm"] = 0;
    //channel enable bit is set to 0
    mpAgentController->SetChannelState(chid, 0, mSizes[chid]);
    mIsCHEN[chid] = false;
}//}}}
///*********************************
/// Function     : SetMSPInCSF
/// Description  : SetMSPInCSF
///*********************************
void MSPI000_Func::SetMSPInCSF(unsigned int value)
{//{{{
    (*MSPInSTR0)["MSPInCSF"] = value;
}//}}}
///*********************************
/// Function     : SetMSPInCNUMF
/// Description  : SetMSPInCNUMF
///*********************************
void MSPI000_Func::SetMSPInCNUMF(unsigned int value)
{//{{{
    (*MSPInSTR0)["MSPInCNUMF"] = value;
}//}}}
///*********************************
/// Function     : SetMSPInFIRXNm
/// Description  : SetMSPInFIRXNm
///*********************************
void MSPI000_Func::SetMSPInFIRXNm(unsigned int chid)
{//{{{
    (*MSPInCSTRm[chid])["MSPInFIRXNm"] = mCount_read_rx[chid];
}//}}}
///*********************************
/// Function     : SetMSPInDCEFSTm
/// Description  : SetMSPInDCEFSTm
///*********************************
void MSPI000_Func::SetMSPInDCEFSTm(unsigned int chid, unsigned int value)
{//{{{
    (*MSPInFSRXm[chid])["MSPInDCEFSTm"] = value;
    (*MSPInFRERSTm[chid])["MSPInDCEFSTm"] = value;
}//}}}
///*********************************
/// Function     : SetMSPInCEFSTm
/// Description  : SetMSPInCEFSTm
///*********************************
void MSPI000_Func::SetMSPInCEFSTm(unsigned int chid, unsigned int value)
{//{{{
    (*MSPInFSRXm[chid])["MSPInCEFSTm"] = value;
    (*MSPInFRERSTm[chid])["MSPInCEFSTm"] = value;
}//}}}
///*********************************
/// Function     : SetMSPInPEFSTm
/// Description  : SetMSPInPEFSTm
///*********************************
void MSPI000_Func::SetMSPInPEFSTm(unsigned int chid, unsigned int value)
{//{{{
    (*MSPInFSRXm[chid])["MSPInPEFSTm"] = value;
    (*MSPInFRERSTm[chid])["MSPInPEFSTm"] = value;
}//}}}
//Get Set Private variables
///*********************************
/// Function     : GetmMSPInSCKbaudarate 
/// Description  : GetmMSPInSCKbaudarate 
///*********************************
double MSPI000_Func::GetmMSPInSCKbaudarate (unsigned int chid)
{//{{{
    return mMSPInSCKbaudarate[chid];
}//}}}
///*********************************
/// Function     : GetMSPInTXDAm0  
/// Description  : GetMSPInTXDAm0  
///*********************************
unsigned int MSPI000_Func::GetMSPInTXDAm0  (unsigned int chid)
{//{{{
    return (unsigned int)(*MSPInTXDAm0[chid])["MSPInTXDAm0"];

}//}}}
///*********************************
/// Function     : GetRAMValue 
/// Description  : GetRAMValue 
///*********************************
unsigned int MSPI000_Func::GetRAMValue (unsigned int chid, unsigned int size)
{//{{{
    unsigned int data = 0;
    if (GetDataMem(mCurTX1[chid], data, size)) {
        mCurTX1[chid] += size;
        return data;
    }
    return 0;
}//}}}
///*********************************
/// Function     : SetRAMValue
/// Description  : Set RAM Value
///*********************************
void MSPI000_Func::SetRAMValue (unsigned int chid, unsigned int value, unsigned int size)
{//{{{
    if (SetDataMem(mCurRX1[chid], value, size)) {
        mCurRX1[chid] += size;
        if (mpMem[chid] == emFixedFifoMem) {
            unsigned int stage = (unsigned int)(*MSPInCFGm4[chid])["MSPInSIZEm"];
            if ((mCurRX1[chid] - mStartRX[chid]) / size == ((8 << stage) / 2) || (mCurRX1[chid] - mStartRX[chid]) / size == (8 << stage)) { //half FIFO filled, FIFO filled
                //Asser RX interrupt
                mpAgentController->AsserINTMSPIn(chid, "RX", "");
            }
        }
    }
}//}}}
///*********************************
/// Function     : GetMSPInPRIOm
/// Description  : GetMSPInPRIOm
///*********************************
unsigned int MSPI000_Func::GetMSPInPRIOm(unsigned int chid)
{//{{{
    return (unsigned int)(*MSPInCFGm0[chid])["MSPInPRIOm"];
}//}}}
///*********************************
/// Function     : GetMSPInFIDLm  
/// Description  : GetMSPInFIDLm  
///*********************************
unsigned int MSPI000_Func::GetMSPInFIDLm(unsigned int chid)
{//{{{
    return (unsigned int)(*MSPInCFGm1[chid])["MSPInFIDLm"];
}//}}}
///*********************************
/// Function     : GetMSPInICLSm
/// Description  : GetMSPInICLSm
///*********************************
unsigned int MSPI000_Func::GetMSPInICLSm(unsigned int chid)
{//{{{
    return (unsigned int)(*MSPInCFGm1[chid])["MSPInICLSm"];
}//}}}
///*********************************
/// Function     : GetDelayValue
/// Description  : Get Delay Value
///*********************************
void MSPI000_Func::GetDelayValue(unsigned int chid, unsigned int delay_value[4])
{//{{{
        sc_assert(delay_value!=NULL);
        delay_value[0] = (unsigned int)(*MSPInSEUPm[chid])["MSPInSEUPm"];
        delay_value[1] = (unsigned int)(*MSPInHOLDm[chid])["MSPInHOLDm"];
        delay_value[2] = (unsigned int)(*MSPInIDLEm[chid])["MSPInIDLEm"];
        delay_value[3] = (unsigned int)(*MSPInINDAm[chid])["MSPInINDAm"];
}//}}}
///*********************************
/// Function     : GetIsLoopBack
/// Description  : Get IsLoopBack
///*********************************
bool MSPI000_Func::GetIsLoopBack(void)
{//{{{
    return mIsLoopBack;
}//}}}
///*********************************
/// Function     : GetIsDataConsistency
/// Description  : Get IsDataConsistency
///*********************************
bool MSPI000_Func::GetIsDataConsistency(void)
{//{{{
    return mIsDataConsistency;
}//}}}
///*********************************
/// Function     : GetmHalfFIFOFilled
/// Description  : Get mHalfFIFOFilled
///*********************************
bool MSPI000_Func::GetmHalfFIFOFilled(unsigned int chid)
{//{{{
    return mHalfFIFOFilled[chid];
}//}}}
///*********************************
/// Function     : GetmIsSafeSPI
/// Description  : Getm IsSafeSPI
///*********************************
bool MSPI000_Func::GetmIsSafeSPI(unsigned int chid)
{//{{{
    return mIsSafeSPI[chid];
}//}}}

//============ Callback function  ========================
//========================================================
///*********************************
/// Function     : cb_MSPInCTL0_MSPInEN
/// Description  : cb_MSPInCTL0_MSPInEN
///*********************************
void MSPI000_Func::cb_MSPInCTL0_MSPInEN(RegCBstr str)
{//{{{
    // write only
    if (mpAgentController->CheckClockPeriod("PCLK")) {
        if (((str.data&0x1) == 0x1) && ((str.pre_data&0x1) == 0x0)) { //0->1
            re_printf("info","IP is enable");
            mIPEN = true;
            //List of Ports or Bits Initialized when MSPInEN is Set from 0 to 1
            if (mIsMaster && mpAgentController->CheckClockPeriod("MSPInCLK")) {
                mpAgentController->InactiveOutputPorts(GetMSPInCSP(), GetMSPInCKR(), GetMSPInSOLS());
            }
            for (unsigned int ch_id = 0; ch_id < mNumChannel; ch_id++) {
                (*(MSPInCESTm[ch_id]))["MSPInOVREEm"] = 1;
                (*(MSPInCESTm[ch_id]))["MSPInOVWREm"] = 1;
                (*(MSPInCESTm[ch_id]))["MSPInOVRUEm"] = 1;
                (*(MSPInCESTm[ch_id]))["MSPInDCEm"] = 1;
                (*(MSPInCESTm[ch_id]))["MSPInCEm"] = 1;
                (*(MSPInCESTm[ch_id]))["MSPInPEm"] = 1;

                (*(MSPInFRERSTm[ch_id]))["MSPInDCEFSTm"] = 1;
                (*(MSPInFRERSTm[ch_id]))["MSPInCEFSTm"] = 1;
                (*(MSPInFRERSTm[ch_id]))["MSPInPEFSTm"] = 1;
                SetMSPInCFCNTm(ch_id, 0);
            }
        } else if (((str.data&0x1) == 0x0) && ((str.pre_data&0x1) == 0x1)) {//1->0
            mIPEN = false;
            //All channel operation can be stopped immediately by using the IP enable bit (MSPInCTL0.MSPInEN).
            //When MSPInEN is cleared to 0, the MSPInCSTRm.MSPInCHENm bit is cleared to 0, and the communication is stops immediately.
            //List of Ports or Bits Initialized when MSPInEN is Cleared from 1 to 0.
            mpAgentController->InactiveOutputPorts(GetMSPInCSP(), GetMSPInCKR(), GetMSPInSOLS());
            (*MSPInSTR0)["MSPInCNUMF"] = 0;
            (*MSPInSTR0)["MSPInCSF"] = 0;
            for (unsigned int ch_id = 0; ch_id < mNumChannel; ch_id++) {
                (*MSPInCSTRm[ch_id])["MSPInTXRQFm"] = 0;
                (*MSPInCSTRm[ch_id])["MSPInACTFm"] = 0;
                (*MSPInCSTRm[ch_id])["MSPInCHENm"] = 0;
                //channel enable bit is set to 0
                mpAgentController->SetChannelState(ch_id, 0, mSizes[ch_id]);
            }
        }
        //Set IP state
        mpAgentController->SetIPState(mIPEN);
    }
}//}}}
///*********************************
/// Function     : cb_MSPInCTL1_MSPInMSSEL
/// Description  : cb_MSPInCTL1_MSPInMSSEL
///*********************************
void MSPI000_Func::cb_MSPInCTL1_MSPInMSSEL(RegCBstr str)
{//{{{
    //write only
    unsigned int mssel = (str.data >> 31) & 0x1;
    unsigned int csie = (str.data >> 29) & 0x1;
    unsigned int samp = (str.data >> 28) & 0x1;
    unsigned int sols = (str.data >> 24) & 0x3;
    unsigned int csp = str.data & 0x7FFF;
    if (mpAgentController->CheckClockPeriod("PCLK")) {
        // Check consider register can be written
        if (!mIPEN){ // can be written
            mIsMaster = (mssel == 0);
            mpAgentController->SetOperationMode(mIsMaster);
            if (mIsMaster){// in the master mode
                if(csie != 0){
                    re_printf("warning", "In the master mode(MSPInMSSEL=0), MSPInCSIE must be set to 0.");
                    (*MSPInCTL1)["MSPInCSIE"] = 0;
                }
            }
            else {// in the slave mode
                if(samp != 0){
                    re_printf("warning", "In the slave mode(MSPInMSSEL=1), MSPInSAMP must be set to 0.");
                    (*MSPInCTL1)["MSPInSAMP"] = 0;
                }
                if(sols != 0){
                    re_printf("warning", "In the slave mode(MSPInMSSEL=1), MSPInSOLS[1:0] must be set to 00.");
                    (*MSPInCTL1)["MSPInSOLS"] = 0;
                }
                if(csp != 0){
                    (*MSPInCTL1)["MSPInCSP"] = 0;
                    re_printf("warning", "In the slave mode(MSPInMSSEL=1), MSPInCSP[14:0] must be set to 0000H.");
                }
            }
        }else{ // can not written
            re_printf("warning", "This register can be written only if MSPInCTL0.MSPInEN = 0.");
            (*MSPInCTL1)= str.pre_data;
        }
    }
}//}}}
///*********************************
/// Function     : cb_MSPInCTL2_MSPInLBM
/// Description  : cb_MSPInCTL2_MSPInLBM
///*********************************
void MSPI000_Func::cb_MSPInCTL2_MSPInLBM(RegCBstr str)
{//{{{
    //write only
    if (mpAgentController->CheckClockPeriod("PCLK")) {
        if(!mIPEN){// can be written
            mIsLoopBack = ((str.data & 0x1)  != 0);
            mIsDataConsistency = (((str.data >> 1)& 0x1) != 0);
            if (!mIsMaster){ // In slave mode(
                if(mIsLoopBack != 0){
                    (*MSPInCTL2)["MSPInLBM"] = 0;
                    mIsLoopBack = false;
                    re_printf("warning", "In slave mode(MSPInMSSEL=1), MSPInLBM must be set to 0.");
                }
            }
        } else{ // can not written
            re_printf("warning", "This register can be written only if MSPInCTL0.MSPInEN = 0.");
            (*MSPInCTL2)= str.pre_data;
        }
    }
}//}}}
///*********************************
/// Function     : cb_MSPInCSTSm_MSPInACTFSm
/// Description  : cb_MSPInCSTSm_MSPInACTFSm
///*********************************
void MSPI000_Func::cb_MSPInCSTSm_MSPInACTFSm(RegCBstr str)
{//{{{
    //write only/ read 0
    sc_dt::uint64 hwts  = (sc_dt::uint64) (*MSPInCFGm4[str.channel])["MSPInHWTSm"];

    unsigned int actfs   = bit_select(str.data
                            ,(*MSPInCSTSm[str.channel])["MSPInACTFSm"].mStartAddr
                            ,(*MSPInCSTSm[str.channel])["MSPInACTFSm"].mEndAddr);
    unsigned int chens   = bit_select(str.data
                            ,(*MSPInCSTSm[str.channel])["MSPInCHENSm"].mStartAddr
                            ,(*MSPInCSTSm[str.channel])["MSPInCHENSm"].mEndAddr);

    if (mpAgentController->CheckClockPeriod("PCLK") && ((mpAgentController->CheckClockPeriod("MSPInCLK") && mIsMaster) || !mIsMaster)) {
        if (chens == 1) { // Sets MSPInCHENm to 1.
            (*MSPInCSTRm[str.channel])["MSPInCHENm"] = 1;
            //List of Bits Initialized when MSPInCHENm is Set from 0 to 1.
            (*(MSPInCESTm[str.channel]))["MSPInOVREEm"] = 0;
            (*(MSPInCESTm[str.channel]))["MSPInOVWREm"] = 0;
            (*(MSPInCESTm[str.channel]))["MSPInOVRUEm"] = 0;
            (*(MSPInCESTm[str.channel]))["MSPInDCEm"] = 0;
            (*(MSPInCESTm[str.channel]))["MSPInCEm"] = 0;
            (*(MSPInCESTm[str.channel]))["MSPInPEm"] = 0;
            (*(MSPInFRERSTm[str.channel]))["MSPInDCEFSTm"] = 0;
            (*(MSPInFRERSTm[str.channel]))["MSPInCEFSTm"] = 0;
            (*(MSPInFRERSTm[str.channel]))["MSPInPEFSTm"] = 0;
             
            mpAgentController->ResetBufferCounter(str.channel); 
            mIsCHEN[str.channel] = true;
            (*MSPInSTR0)["MSPInCSF"] = 1;  //1. Write MSPInCSTSm.MSPInCHENSm to 1
            //todo Calculate RAM Area
            unsigned int txe = (unsigned int)(*MSPInCFGm0[str.channel])["MSPInTXEm"];
            unsigned int rxe = (unsigned int)(*MSPInCFGm0[str.channel])["MSPInRXEm"];
            unsigned int flen = (unsigned int)(*MSPInCFGm2[str.channel])["MSPInFLENm"];
            unsigned int cfset = (unsigned int)(*MSPInCFSETm[str.channel])["MSPInCFSETm"];
            unsigned int rastad = (unsigned int)(*MSPInRASTADm[str.channel])["MSPInRASTAD"];
            unsigned int size = (unsigned int)(*MSPInCFGm4[str.channel])["MSPInSIZEm"];
            rastad = rastad << 2;
            if (mIsLoopBack) {
                txe = 1;
                rxe = 1;
            }
            if (mpMem[str.channel] == emFixedBufferMem) {
                unsigned int tmp = 0;
                if (flen > 32) {
                    tmp = flen/32;
                    if (flen % 32 != 0) {
                        tmp += 1;
                    }
                    tmp *= (4*cfset);
                } else {
                    tmp = flen/8;
                    if (flen % 8 != 0) {
                        tmp = flen/8 + 1;
                    }
                    tmp *= cfset;
                    if (tmp % 4 != 0) {
                        tmp = (tmp/4 + 1)*4;
                    }
                }
                if (txe == 1) {
                    mStartTX[str.channel] = rastad;
                    if (rxe == 1) {
                        mStartRX[str.channel] = mStartTX[str.channel] + tmp;
                    }
                } else if (rxe == 1) {
                    mStartRX[str.channel] = rastad;
                }
            } else if (mpMem[str.channel] == emFixedFifoMem){
                unsigned int tmp = 0;
                if (flen > 32) {
                    tmp = flen/32;
                    if (flen % 32 != 0) {
                        tmp += 1;
                    }
                    tmp = tmp * 4 * 8 * (1 << size);
                } else {
                    tmp = flen/8;
                    if (flen % 8 != 0) {
                        tmp = flen/8 + 1;
                    }
                    tmp = tmp * 8 * (1 << size);
                }
                if (txe == 1) {
                    mStartTX[str.channel] = rastad;
                    if (rxe == 1) {
                        mStartRX[str.channel] = mStartTX[str.channel] + tmp;
                    }
                } else if (rxe == 1) {
                    mStartRX[str.channel] = rastad;
                }
            }
        }

        if ((actfs == 1 && mIsCHEN[str.channel]) || (actfs == 1 && chens == 1) || (((hwts & mpAgentController->GetHWTRGValue()) != 0) && mIsCHEN[str.channel])) { // Sets MSPInACTFm to 1.
            (*MSPInCSTRm[str.channel])["MSPInACTFm"] = 1;
            //channel enable bit is set to 1
            if (mIPEN) {
                //        When using the Safe-SPI protocol function, The following configuration must be set.
                //        -Transmission/Reception
                //        -Frame length: 32bits
                //        -Communication direction : MSB
                //        -The parity check is invalid.
                unsigned int safe = (unsigned int)(*MSPInCFGm1[str.channel])["MSPInSAFEm"];
                unsigned int txe = (unsigned int)(*MSPInCFGm0[str.channel])["MSPInTXEm"];
                unsigned int rxe = (unsigned int)(*MSPInCFGm0[str.channel])["MSPInRXEm"];
                unsigned int flen = (unsigned int)(*MSPInCFGm2[str.channel])["MSPInFLENm"];
                unsigned int dir = (unsigned int)(*MSPInCFGm1[str.channel])["MSPInDIRm"];
                unsigned int dechk = (unsigned int)(*MSPInCFGm1[str.channel])["MSPInDECHKm"];
                if (safe == 1 && (txe != 1 || rxe != 1 || flen != 32 || dir != 0 || dechk != 0)) {
                    re_printf("warning", "When using the Safe-SPI protocol function, the following configuration must be set. txe = %d != 1 || rxe = %d != 1 || flen  = %d != 32 || dir = %d  != 0 || dechk = %d  != 0", txe, rxe, flen, dir, dechk);
                    mIsSafeSPI[str.channel] = false;
                    re_printf("warning", " Channel %d Safe-SPI function is disable.", str.channel);
                } else if (safe == 1) {
                    mIsSafeSPI[str.channel] = true;
                }
                if (flen > 32) {
                    mSizes[str.channel] = 4;
                } else {
                    mSizes[str.channel] = flen / 8;
                    if (flen % 8 != 0) {
                        mSizes[str.channel]++;
                    }
                }
                mpAgentController->SetChannelState(str.channel, 1, mSizes[str.channel]);
                mCurTX[str.channel] = mStartTX[str.channel];
                mCurTX1[str.channel] = mStartTX[str.channel];
                mCurRX[str.channel] = mStartRX[str.channel];
                mCurRX1[str.channel] = mStartRX[str.channel];
                mHalfFIFOFilled[str.channel] = false;
            }
        }
    }
    ///// notice that when read, the value 00H is always returned. because of the configuration R|W1:0
}//}}}
///*********************************
/// Function     : cb_MSPInCSTCm_MSPInCHENCm
/// Description  : cb_MSPInCSTCm_MSPInCHENCm
///*********************************
void MSPI000_Func::cb_MSPInCSTCm_MSPInCHENCm(RegCBstr str)
{//{{{
    //write only // read 0
    if (mpAgentController->CheckClockPeriod("PCLK")) {
        unsigned int chenc   = bit_select(str.data
                            ,(*MSPInCSTCm[str.channel])["MSPInCHENCm"].mStartAddr
                            ,(*MSPInCSTCm[str.channel])["MSPInCHENCm"].mEndAddr);
        if (chenc == 0x1) { // Sets MSPInCHENm to 0.
            mpAgentController->SetChannelStop(str.channel);
        }
    }
}//}}}
///*********************************
/// Function     : cb_MSPInCFGm0_MSPInTXEm
/// Description  : cb_MSPInCFGm0_MSPInTXEm
///*********************************
void MSPI000_Func::cb_MSPInCFGm0_MSPInTXEm(RegCBstr str)
{//{{{
    //write only
    unsigned int txe = (str.data >> 29) & 0x1;
    unsigned int rxe = (str.data >> 28) & 0x1;
    unsigned int mode = (str.data >> 24) & 0x3;
    unsigned int fcce = (str.data >> 12) & 0x1;
    if (mpAgentController->CheckClockPeriod("PCLK")) {
        if (mIsLoopBack) {
            txe = 1;
            rxe = 1;
        }
        if (!mIsCHEN[str.channel]) { //CHENm = 0
            if (txe == 1 && rxe == 1) {
                mpCom[str.channel] = emTrans_Rep;
            } else if (txe == 1 && rxe == 0){
                mpCom[str.channel] = emTrans_Only;
            } else if (txe == 0 && rxe == 1) {
                mpCom[str.channel] = emRep_Only;
            } else {
                mpCom[str.channel] = emNo_Comunication;
            }
            mpAgentController->SetCommunicationMode(str.channel, mpCom[str.channel]);
            if (mode == 0) {
                mpMem[str.channel] = emDirectMem;
            } else if (mode == 1) {
                mpMem[str.channel] = emFixedBufferMem;
            } else if (mode == 2) {
                mpMem[str.channel] = emFixedFifoMem;
            } else {
                re_printf("warning", "This setting is prohibited MSPInMDm[1:0] = 3. MSPInMDm[1:0] is set to 0 (default value)");
                mpMem[str.channel] = emDirectMem;
                (*MSPInCFGm0[str.channel])["MSPInMDm"] = 0;
            }
            mpAgentController->SetMemoryMode(str.channel, mpMem[str.channel]);
            if (mpMem[str.channel] == emFixedBufferMem || mpMem[str.channel] == emFixedFifoMem) {
                if (fcce != 0) {
                    (*MSPInCFGm0[str.channel])["MSPInFCCEm"] = 0;
                    re_printf("warning", "Set MSPInFCCEm to 0 in the fixed buffer memory mode and fixed FIFO memory mode.");
                }
            } else if (mpMem[str.channel] == emDirectMem && mpCom[str.channel] == emRep_Only) {
                if (fcce != 0) {
                    (*MSPInCFGm0[str.channel])["MSPInFCCEm"] = 0;
                    re_printf("warning", "Set MSPInFCCEm to 0 in mode reception only setting of the direct memory mode.");
                }
            }
            //todo Set MSPInCPOLm to 0 in the slave mode. (wrong spec)
        } else { //CHENm = 1
            re_printf("warning", "This register can be written only if MSPInCSTRm.MSPInCHENm = 0.");
            (*MSPInCFGm0[str.channel])= str.pre_data;
        }
    }
}//}}}
///*********************************
/// Function     : cb_MSPInCFGm1_MSPInCPOLm
/// Description  : cb_MSPInCFGm1_MSPInCPOLm
///*********************************
void MSPI000_Func::cb_MSPInCFGm1_MSPInCPOLm(RegCBstr str)
{//{{{
    //write only
    unsigned int icls = (str.data >> 18) & 0x1;
    unsigned int cpol = (str.data >> 25) & 0x1;
    unsigned int ps = (str.data >> 4) & 0x3;
    if (mpAgentController->CheckClockPeriod("PCLK")) {
        if (!mIsCHEN[str.channel]) { //CHENm = 0
            if (!mIsMaster) {
                if (cpol != 0) {
                    (*MSPInCFGm1[str.channel])["MSPInCPOLm"] = 0;
                    re_printf("warning", "Set MSPInCPOLm to 0 in the slave mode.");
                }
            }
            if (mpMem[str.channel] == emFixedBufferMem || mpMem[str.channel] == emFixedFifoMem) {
                if (icls != 0) {
                    (*MSPInCFGm1[str.channel])["MSPInICLSm"] = 0;
                    re_printf("warning", "Set MSPInICLSm to 0 in the fixed buffer memory mode and fixed FIFO memory mode.");
                }
            } else if (mpMem[str.channel] == emDirectMem && mpCom[str.channel] == emRep_Only) {
                if (icls != 0) {
                    (*MSPInCFGm1[str.channel])["MSPInICLSm"] = 0;
                    re_printf("warning", "Set MSPInICLSm to 0 in mode reception only setting of the direct memory mode.");
                }
            }
            if (ps == 3) {
                re_printf("warning", "This setting is prohibited MSPInPSm[1:0] = 3. MSPInPSm[1:0] is set to 0 (default value)");
                (*MSPInCFGm1[str.channel])["MSPInPSm"] = 0;
            }
        } else { //CHENm = 1
            re_printf("warning", "This register can be written only if MSPInCSTRm.MSPInCHENm = 0.");
            (*MSPInCFGm1[str.channel])= str.pre_data;
        }
    }
}//}}}
///*********************************
/// Function     : cb_MSPInCFGm2_MSPInFLENm
/// Description  : cb_MSPInCFGm2_MSPInFLENm
///*********************************
void MSPI000_Func::cb_MSPInCFGm2_MSPInFLENm(RegCBstr str)
{//{{{
    //write only
    unsigned int flen = (str.data) & 0xFF;
    if (mpAgentController->CheckClockPeriod("PCLK")) {
        if (!mIsCHEN[str.channel]) { //CHENm = 0
            if (flen < 2 || flen > 128) {
                re_printf("warning", "This setting is prohibited MSPInFLENm[7:0] = 0,1,>128. The frame length is set to 32 (default value)");
                (*MSPInCFGm2[str.channel])=32;
            }
        } else { //CHENm = 1
            re_printf("warning", "This register can be written only if MSPInCSTRm.MSPInCHENm = 0.");
            (*MSPInCFGm2[str.channel])= str.pre_data;
        }
    }
}//}}}
///*********************************
/// Function     : cb_MSPInCFGm3_MSPInPRCSm
/// Description  : cb_MSPInCFGm3_MSPInPRCSm
///*********************************
void MSPI000_Func::cb_MSPInCFGm3_MSPInPRCSm(RegCBstr str)
{//{{{
    //write only
    if (mpAgentController->CheckClockPeriod("PCLK")) {
        if (!mIsCHEN[str.channel]) { //CHENm = 0
            if (!mIsMaster) {
                if ((unsigned int) str.data != 1) {
                    (*MSPInCFGm3[str.channel]) = 1;
                    re_printf("warning", "In the slave mode(MSPInMSSEL=1), MSPInCFGm3 must be set to 0001H(default value).");
                }
            }
            unsigned int prcs = (str.data >> 8) & 0x3;
            unsigned int cdiv = (str.data) & 0x1F;
            //calculate baud rate
            mMSPInSCKbaudarate[str.channel] = (double)1 / ((1 << (prcs*2)) * cdiv * 2); // 4 ^ prcs
        } else { //CHENm = 1
            re_printf("warning", "This register can be written only if MSPInCSTRm.MSPInCHENm = 0.");
            (*MSPInCFGm3[str.channel])= str.pre_data;
        }
    }
}//}}}
///*********************************
/// Function     : cb_MSPInCFGm4_MSPInHWTSm
/// Description  : cb_MSPInCFGm4_MSPInHWTSm
///*********************************
void MSPI000_Func::cb_MSPInCFGm4_MSPInHWTSm(RegCBstr str)
{//{{{
    //write only
    unsigned int size = (str.data) & 0x3;
    if (mpAgentController->CheckClockPeriod("PCLK")) {
        if (!mIsCHEN[str.channel]) { //CHENm = 0
            if (size == 3) {
                re_printf("warning", "This setting is prohibited MSPInSIZEm[1:0] = 3. MSPInSIZEm[1:0] is set to 0 (default value)");
                (*MSPInCFGm4[str.channel])["MSPInSIZEm"] = 0;
            }
        } else { //CHENm = 1
            re_printf("warning", "This register can be written only if MSPInCSTRm.MSPInCHENm = 0.");
            (*MSPInCFGm4[str.channel])= str.pre_data;
        }
    }
}//}}}
///*********************************
/// Function     : cb_MSPInRASTADm_MSPInRASTAD
/// Description  : cb_MSPInRASTADm_MSPInRASTAD
///*********************************
void MSPI000_Func::cb_MSPInRASTADm_MSPInRASTAD(RegCBstr str)
{//{{{
    //write only
    unsigned int rastad = (str.data >> 2) & 0x3FF;
    if (mpAgentController->CheckClockPeriod("PCLK")) {
        if (!mIsCHEN[str.channel]) { //CHENm = 0
            if (mpMem[str.channel] == emDirectMem) {
                if (rastad != 0) {
                    (*MSPInRASTADm[str.channel])["MSPInRASTAD"] = 0;
                    re_printf("warning", "Set MSPInRASTADm[11:0] to 0000H in the direct memory mode.");
                }
            }
        } else { //CHENm = 1
            re_printf("warning", "This register can be written only if MSPInCSTRm.MSPInCHENm = 0.");
            (*MSPInRASTADm[str.channel])= str.pre_data;
        }
    }
}//}}}
///*********************************
/// Function     : cb_MSPInSEUPm_MSPInSEUPm
/// Description  : cb_MSPInSEUPm_MSPInSEUPm
///*********************************
void MSPI000_Func::cb_MSPInSEUPm_MSPInSEUPm(RegCBstr str)
{//{{{
    //write only
    unsigned int seup = (str.data) & 0xFFF;
    if (mpAgentController->CheckClockPeriod("PCLK")) {
        if (!mIsCHEN[str.channel]) { //CHENm = 0
            if (seup == 0) {
                re_printf("warning", "This setting is prohibited MSPInSEUPm [11:0] = 0. MSPInSEUPm [11:0] is set to 1 (default value)");
                (*MSPInSEUPm[str.channel])= 1;
            }
            else if (seup > 1) {
                if (!mIsMaster) {
                    (*MSPInSEUPm[str.channel])["MSPInSEUPm"] = 1;
                    re_printf("warning", "Set MSPInSEUPm to 0001H in the slave mode.");
                }
            }
        } else { //CHENm = 1
            re_printf("warning", "This register can be written only if MSPInCSTRm.MSPInCHENm = 0.");
            (*MSPInSEUPm[str.channel])= str.pre_data;
        }
    }
}//}}}
///*********************************
/// Function     : cb_MSPInHOLDm_MSPInHOLDm
/// Description  : cb_MSPInHOLDm_MSPInHOLDm
///*********************************
void MSPI000_Func::cb_MSPInHOLDm_MSPInHOLDm(RegCBstr str)
{//{{{
    //write only
    unsigned int hold = (str.data) & 0xFFF;
    if (mpAgentController->CheckClockPeriod("PCLK")) {
        if (!mIsCHEN[str.channel]) { //CHENm = 0
            if (hold == 0) {
                re_printf("warning", "This setting is prohibited MSPInHOLDm [11:0] = 0. MSPInHOLDm [11:0] is set to 1 (default value)");
                (*MSPInHOLDm[str.channel])= 1;
            }
            else if (hold > 1) {
                if (!mIsMaster) {
                    (*MSPInHOLDm[str.channel])["MSPInHOLDm"] = 1;
                    re_printf("warning", "Set MSPInHOLDm to 0001H in the slave mode.");
                }
            }
        } else { //CHENm = 1
            re_printf("warning", "This register can be written only if MSPInCSTRm.MSPInCHENm = 0.");
            (*MSPInHOLDm[str.channel])= str.pre_data;
        }
    }
}//}}}
///*********************************
/// Function     : cb_MSPInIDLEm_MSPInIDLEm
/// Description  : cb_MSPInIDLEm_MSPInIDLEm
///*********************************
void MSPI000_Func::cb_MSPInIDLEm_MSPInIDLEm(RegCBstr str)
{//{{{
    //write only
    unsigned int idle = (str.data) & 0xFFF;
    if (mpAgentController->CheckClockPeriod("PCLK")) {
        if (!mIsCHEN[str.channel]) { //CHENm = 0
            if (idle == 0) {
                re_printf("warning", "This setting is prohibited MSPInIDLEm [11:0] = 0. MSPInIDLEm [11:0] is set to 1 (default value)");
                (*MSPInIDLEm[str.channel])= 1;
            }
            else if (idle > 1) {
                if (!mIsMaster) {
                    (*MSPInIDLEm[str.channel])["MSPInIDLEm"] = 1;
                    re_printf("warning", "Set MSPInIDLEm to 0001H in the slave mode.");
                }
            }
        } else { //CHENm = 1
            re_printf("warning", "This register can be written only if MSPInCSTRm.MSPInCHENm = 0.");
            (*MSPInIDLEm[str.channel])= str.pre_data;
        }
    }
}//}}}
///*********************************
/// Function     : cb_MSPInINDAm_MSPInINDAm
/// Description  : cb_MSPInINDAm_MSPInINDAm
///*********************************
void MSPI000_Func::cb_MSPInINDAm_MSPInINDAm(RegCBstr str)
{//{{{
    //write only
    unsigned int inda = (str.data) & 0xFFF;
    if (mpAgentController->CheckClockPeriod("PCLK")) {
        if (!mIsCHEN[str.channel]) { //CHENm = 0
            if (inda > 0) {
                if (!mIsMaster) {
                    (*MSPInINDAm[str.channel])["MSPInINDAm"] = 0;
                    re_printf("warning", "Set MSPInINDAm to 0000H in the slave mode.");
                }
            }
        } else { //CHENm = 1
            re_printf("warning", "This register can be written only if MSPInCSTRm.MSPInCHENm = 0.");
            (*MSPInINDAm[str.channel])= str.pre_data;
        }
    }
}//}}}
///*********************************
/// Function     : cb_MSPInCFSETm_MSPInCFSETm
/// Description  : cb_MSPInCFSETm_MSPInCFSETm
///*********************************
void MSPI000_Func::cb_MSPInCFSETm_MSPInCFSETm(RegCBstr str)
{//{{{
    //write only
    unsigned int cfset = (str.data) & 0xFFFF;
    if (mpAgentController->CheckClockPeriod("PCLK")) {
        if (!mIsCHEN[str.channel]) { //CHENm = 0
            if (cfset == 0) {
                re_printf("warning", "This setting is prohibited MSPInCFSETm [15:0] = 0. MSPInCFSETm [15:0] is set to 1 (default value)");
                (*MSPInCFSETm[str.channel])= 1;
            }
        } else { //CHENm = 1
            re_printf("warning", "This register can be written only if MSPInCSTRm.MSPInCHENm = 0.");
            (*MSPInCFSETm[str.channel])= str.pre_data;
        }
    }
}//}}}
///*********************************
/// Function     : cb_MSPInSSELm_MSPInJOBENm
/// Description  : cb_MSPInSSELm_MSPInJOBENm
///*********************************
void MSPI000_Func::cb_MSPInSSELm_MSPInJOBENm(RegCBstr str)
{//{{{
    //write only
    //Ignore This register can be written only if MSPInCSTRm.MSPInCHENm = 0.
    unsigned int joben = (str.data >> 15) & 0x1;
    unsigned int csr = (str.data) & 0x7FFF;
    if (mpAgentController->CheckClockPeriod("PCLK")) {
        if (mpMem[str.channel] == emFixedBufferMem || mpMem[str.channel] == emFixedFifoMem) {
            if (joben != 0) {
                (*MSPInSSELm[str.channel])["MSPInJOBENm"] = 0;
                re_printf("warning", "Set MSPInJOBENm to 0 in the fixed buffer memory mode and fixed FIFO memory mode.");
            }
        }
        if (!mIsMaster && str.data != 0) {
            re_printf("warning", "Set MSPInSSELm to 0000H in the slave mode.");
            (*MSPInSSELm[str.channel]) = 0;
        }
        //update mirror
        (*MSPInSSTXm[str.channel])["MSPInJOBENm"] = joben;
        (*MSPInSSTXm[str.channel])["MSPInCSRm"] = csr;
    }
}//}}}
///*********************************
/// Function     : cb_MSPInTXDAm0_MSPInTXDAm0
/// Description  : cb_MSPInTXDAm0_MSPInTXDAm0
///*********************************
void MSPI000_Func::cb_MSPInTXDAm0_MSPInTXDAm0(RegCBstr str)
{//{{{
    //Transmission data written from CPU/DMA to the MSPInTXDAm0 register is automatically stored in MSPI RAM memory (Fixed FIFO RAM memory)
    //write
    if (mpAgentController->CheckClockPeriod("PCLK")) {
        unsigned int flen = GetMSPInFLENm(str.channel);
        unsigned int tmp = flen/32;
        if (flen % 32 != 0) {
            tmp += 1;
        }
        if (mpMem[str.channel] == emFixedFifoMem) {
            unsigned int size = (unsigned int)(*MSPInCFGm4[str.channel])["MSPInSIZEm"];
            if (SetDataMem(mCurTX[str.channel], str.data, mSizes[str.channel])) {
                mCurTX[str.channel] += mSizes[str.channel];
            }
            if ((mCurTX[str.channel] - mStartTX[str.channel]) / mSizes[str.channel] == ((8 << size) / 2)) { //half FIFO filled
                mHalfFIFOFilled[str.channel] = true;
            }

            if ((mCurTX[str.channel] - mStartTX[str.channel]) / mSizes[str.channel] == (tmp * GetMSPInCFSETm(str.channel))) { //end frame
                SetMSPInTXRQFm(str.channel, 0);
            }
            if ((mCurTX[str.channel] - mStartTX[str.channel]) / mSizes[str.channel] > (tmp * GetMSPInCFSETm(str.channel))) { //An over-write error occurs when transmission data is written to the MSPInTXDAm0 register while the FIFO buffer is full.
                mpAgentController->AsserINTMSPIn(str.channel, "ER", "Over-write");
            } else {
                mpAgentController->UpdateRequestNumber(str.channel);
                mHalfFIFOFilled[str.channel] = false;
            }
        }
        if (mpMem[str.channel] == emDirectMem) {
            if ((*MSPInCSTRm[str.channel])["MSPInTXRQFm"] == 1) {
                mpAgentController->UpdateRequestNumber(str.channel);
                SetMSPInTXRQFm(str.channel, 0);
            }
        }
        //update mirror
        (*MSPInSSTXm[str.channel])["MSPInTXDAm0"] = str.data & 0xFFFF;
    }
}//}}}
///*********************************
/// Function     : cb_MSPInFRERSTCm_MSPInDCEFSCm
/// Description  : cb_MSPInFRERSTCm_MSPInDCEFSCm
///*********************************
void MSPI000_Func::cb_MSPInFRERSTCm_MSPInDCEFSCm(RegCBstr str)
{//{{{
    //write only //read 0
    unsigned int pe   = bit_select(str.data
                            ,(*MSPInFRERSTCm[str.channel])["MSPInPEFSCm"].mStartAddr
                            ,(*MSPInFRERSTCm[str.channel])["MSPInPEFSCm"].mEndAddr);
    unsigned int ce   = bit_select(str.data
                             ,(*MSPInFRERSTCm[str.channel])["MSPInCEFSCm"].mStartAddr
                             ,(*MSPInFRERSTCm[str.channel])["MSPInCEFSCm"].mEndAddr);
    unsigned int dce   = bit_select(str.data
                             ,(*MSPInFRERSTCm[str.channel])["MSPInDCEFSCm"].mStartAddr
                             ,(*MSPInFRERSTCm[str.channel])["MSPInDCEFSCm"].mEndAddr);
    if (mpAgentController->CheckClockPeriod("PCLK")) {
        if (pe == 1) {
            (*MSPInFRERSTm[str.channel])["MSPInPEFSTm"] = 0;
            (*MSPInFSRXm[str.channel])["MSPInPEFSTm"] = 0;
        }
        if (ce == 1) {
            (*MSPInFRERSTm[str.channel])["MSPInCEFSTm"] = 0;
            (*MSPInFSRXm[str.channel])["MSPInCEFSTm"] = 0;
        }
        if (dce == 1) {
            (*MSPInFRERSTm[str.channel])["MSPInDCEFSTm"] = 0;
            (*MSPInFSRXm[str.channel])["MSPInDCEFSTm"] = 0;
        }
    }
}//}}}
///*********************************
/// Function     : cb_MSPInRXDAm0_MSPInRXDAm0
/// Description  : cb_MSPInRXDAm0_MSPInRXDAm0
///*********************************
void MSPI000_Func::cb_MSPInRXDAm0_MSPInRXDAm0(RegCBstr str)
{//{{{
    //write code here
    //call back read this register
    if ((*MSPInCSTRm[str.channel])["MSPInRXRQFm"] == 1) {
        mCount_read_rx[str.channel]--;
        if (mCount_read_rx[str.channel] == 0) { //if read a half FIFO stage or 1
            SetMSPInRXRQFm(str.channel, 0);
            mpAgentController->AsserINTMSPIn(str.channel, "ReadMSPInRXDAm0", "");
        }
        if (mpMem[str.channel] == emFixedFifoMem) {
            unsigned int data = 0;
            if (GetDataMem(mCurRX[str.channel], data, mSizes[str.channel])) {
                mCurRX[str.channel] += mSizes[str.channel];
            }
            SetMSPInRXDAm0(str.channel, data);
        }
    } else { //An over-read error occurs when MSPInRXDAm0 register is read while the FIFO buffer is empty.
        if (mpMem[str.channel] == emFixedFifoMem) {
            mpAgentController->AsserINTMSPIn(str.channel, "ER", "Over-read");
        }
    }
}//}}}
///*********************************
/// Function     : cb_MSPInSSTXm_MSPInTXDAm0
/// Description  : cb_MSPInSSTXm_MSPInTXDAm0
///*********************************
void MSPI000_Func::cb_MSPInSSTXm_MSPInTXDAm0(RegCBstr str)
{//{{{
    //write code here
    if (mpAgentController->CheckClockPeriod("PCLK")) {
        unsigned int flen = GetMSPInFLENm(str.channel);
        unsigned int tmp = flen/32;
        if (flen % 32 != 0) {
            tmp += 1;
        }
        if (mpMem[str.channel] == emDirectMem) {
            if ((*MSPInCSTRm[str.channel])["MSPInTXRQFm"] == 1) {
                mpAgentController->UpdateRequestNumber(str.channel);
                SetMSPInTXRQFm(str.channel, 0);
            }
        }
        else if (mpMem[str.channel] == emFixedFifoMem) {
            unsigned int size = (unsigned int)(*MSPInCFGm4[str.channel])["MSPInSIZEm"];
            if (SetDataMem(mCurTX[str.channel], (str.data & 0xFFFF), mSizes[str.channel])) {
                mCurTX[str.channel] += mSizes[str.channel];
            }
            if ((mCurTX[str.channel] - mStartTX[str.channel]) / mSizes[str.channel] == ((8 << size) / 2)) { //half FIFO filled
                mHalfFIFOFilled[str.channel] = true;
            }

            if ((mCurTX[str.channel] - mStartTX[str.channel]) / mSizes[str.channel] == (tmp * GetMSPInCFSETm(str.channel))) { //end frame
                SetMSPInTXRQFm(str.channel, 0);
            }
            if ((mCurTX[str.channel] - mStartTX[str.channel]) / mSizes[str.channel] > (tmp * GetMSPInCFSETm(str.channel))) { //An over-write error occurs when transmission data is written to the MSPInTXDAm0 register while the FIFO buffer is full.
                mpAgentController->AsserINTMSPIn(str.channel, "ER", "Over-write");
            } else {
                mpAgentController->UpdateRequestNumber(str.channel);
                mHalfFIFOFilled[str.channel] = false;
            }
        }
        unsigned int joben = (str.data >> 31) & 0x1;
        if (mpMem[str.channel] == emFixedBufferMem || mpMem[str.channel] == emFixedFifoMem) {
            if (joben != 0) {
                (*MSPInSSTXm[str.channel])["MSPInJOBENm"] = 0;
                re_printf("warning", "Set MSPInJOBENm to 0 in the fixed buffer memory mode and fixed FIFO memory mode.");
            }
        }
        if (!mIsMaster && (str.data >> 16) != 0) {
            re_printf("warning", "Set MSPInSSTXm[MSPInJOBENm] and MSPInSSTXm[MSPInCSRm] to 0000H in the slave mode.");
            (*MSPInSSTXm[str.channel])["MSPInJOBENm"] = 0;
            (*MSPInSSTXm[str.channel])["MSPInCSRm"] = 0;
        }
        //update ogrinal register
        (*MSPInSSELm[str.channel]) = str.data >> 16;
        (*MSPInTXDAm0[str.channel]) = (((*MSPInTXDAm0[str.channel]) & 0xFFFF0000) | (str.data & 0xFFFF));
    }
}//}}}
///*********************************
/// Function     : cb_MSPInFSRXm_MSPInRXDAm0
/// Description  : cb_MSPInFSRXm_MSPInRXDAm0
///*********************************
void MSPI000_Func::cb_MSPInFSRXm_MSPInRXDAm0(RegCBstr str)
{//{{{
    //read call-back
    if ((*MSPInCSTRm[str.channel])["MSPInRXRQFm"] == 1) {
        mCount_read_rx[str.channel]--;
        if (mCount_read_rx[str.channel] == 0) { //if read a half FIFO stage or 1
            SetMSPInRXRQFm(str.channel, 0);
        }
        if (mpMem[str.channel] == emFixedFifoMem) {
            unsigned int data = 0;
            if (GetDataMem(mCurRX[str.channel], data, mSizes[str.channel])) {
                mCurRX[str.channel] += mSizes[str.channel];
            }
            SetMSPInRXDAm0(str.channel, data);
        }
    } else { //An over-read error occurs when MSPInRXDAm0 register is read while the FIFO buffer is empty.
        if (mpMem[str.channel] == emFixedFifoMem) {
            mpAgentController->AsserINTMSPIn(str.channel, "ER", "Over-read");
        }
    }
}//}}}
///*********************************
/// Function     : cb_MSPInCESTCm_MSPInOVREECm
/// Description  : cb_MSPInCESTCm_MSPInOVREECm
///*********************************
void MSPI000_Func::cb_MSPInCESTCm_MSPInOVREECm(RegCBstr str)
{//{{{
    //write only read 0
    unsigned int cest1   = bit_select(str.data
                       ,(*MSPInCESTCm[str.channel])["MSPInOVREECm"].mStartAddr
                       ,(*MSPInCESTCm[str.channel])["MSPInOVREECm"].mEndAddr);
    unsigned int cest2   = bit_select(str.data
                       ,(*MSPInCESTCm[str.channel])["MSPInOVWRECm"].mStartAddr
                       ,(*MSPInCESTCm[str.channel])["MSPInOVWRECm"].mEndAddr);
    unsigned int cest3   = bit_select(str.data
                       ,(*MSPInCESTCm[str.channel])["MSPInOVRUECm"].mStartAddr
                       ,(*MSPInCESTCm[str.channel])["MSPInOVRUECm"].mEndAddr);
    unsigned int cest4   = bit_select(str.data
                       ,(*MSPInCESTCm[str.channel])["MSPInDCECm"].mStartAddr
                       ,(*MSPInCESTCm[str.channel])["MSPInDCECm"].mEndAddr);
    unsigned int cest5   = bit_select(str.data
                       ,(*MSPInCESTCm[str.channel])["MSPInCECm"].mStartAddr
                       ,(*MSPInCESTCm[str.channel])["MSPInCECm"].mEndAddr);
    unsigned int cest6   = bit_select(str.data
                       ,(*MSPInCESTCm[str.channel])["MSPInPECm"].mStartAddr
                       ,(*MSPInCESTCm[str.channel])["MSPInPECm"].mEndAddr);
    if (mpAgentController->CheckClockPeriod("PCLK")) {
        if (cest1 == 1) {
            (*MSPInCESTm[str.channel])["MSPInOVREEm"] = 0;
        }
        if (cest2 == 1) {
            (*MSPInCESTm[str.channel])["MSPInOVWREm"] = 0;
        }
        if (cest3 == 1) {
            (*MSPInCESTm[str.channel])["MSPInOVRUEm"] = 0;
        }
        if (cest4 == 1) {
            (*MSPInCESTm[str.channel])["MSPInDCEm"] = 0;
        }
        if (cest5 == 1) {
            (*MSPInCESTm[str.channel])["MSPInCEm"] = 0;
        }
        if (cest6 == 1) {
            (*MSPInCESTm[str.channel])["MSPInPEm"] = 0;
        }
    }
}//}}}
///*********************************
/// Function     : CheckSameValue
/// Description  : Check Same Value
///*********************************
void MSPI000_Func::CheckSameValue(unsigned int i, unsigned int chid)
{//{{{
    //chid -> i
    if (
       (*MSPInCFGm1[chid])["MSPInCPOLm"] != (*MSPInCFGm1[i])["MSPInCPOLm"]
    || (*MSPInCFGm1[chid])["MSPInCPHAm"] != (*MSPInCFGm1[i])["MSPInCPHAm"]
    || (*MSPInCFGm1[chid])["MSPInDIRm"] != (*MSPInCFGm1[i])["MSPInDIRm"]
    || (*MSPInCFGm1[chid])["MSPInFIDLm"] != (*MSPInCFGm1[i])["MSPInFIDLm"]
    || (*MSPInCFGm1[chid])["MSPInSAFCMm"] != (*MSPInCFGm1[i])["MSPInSAFCMm"]
    || (*MSPInCFGm1[chid])["MSPInSAFSm"] != (*MSPInCFGm1[i])["MSPInSAFSm"]
    || (*MSPInCFGm1[chid])["MSPInSAFEm"] != (*MSPInCFGm1[i])["MSPInSAFEm"]
    || (*MSPInCFGm1[chid])["MSPInPSm"] != (*MSPInCFGm1[i])["MSPInPSm"]
    || (*MSPInCFGm1[chid])["MSPInDECHKm"] != (*MSPInCFGm1[i])["MSPInDECHKm"]
    || (*MSPInCFGm2[chid])["MSPInFLENm"] != (*MSPInCFGm2[i])["MSPInFLENm"]
    || (*MSPInCFGm3[chid])["MSPInPRCSm"] != (*MSPInCFGm3[i])["MSPInPRCSm"]
    || (*MSPInCFGm3[chid])["MSPInCDIVm"] != (*MSPInCFGm3[i])["MSPInCDIVm"]
    || (*MSPInSEUPm[chid])["MSPInSEUPm"] != (*MSPInSEUPm[i])["MSPInSEUPm"]
    || (*MSPInHOLDm[chid])["MSPInHOLDm"] != (*MSPInHOLDm[i])["MSPInHOLDm"]
    || (*MSPInIDLEm[chid])["MSPInIDLEm"] != (*MSPInIDLEm[i])["MSPInIDLEm"]
    || (*MSPInINDAm[chid])["MSPInINDAm"] != (*MSPInINDAm[i])["MSPInINDAm"]
    ) {
        re_printf("warning", "If MSPInCSRm of plural channels are the exact same, following parameters have to be set the same value. Channel %d and %d", i, chid);
    }
}//}}}

// vim600: set foldmethod=marker :
