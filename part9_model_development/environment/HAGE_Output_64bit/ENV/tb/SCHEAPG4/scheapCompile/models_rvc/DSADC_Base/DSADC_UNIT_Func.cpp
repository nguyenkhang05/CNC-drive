// ---------------------------------------------------------------------
// $Id: DSADC_UNIT_Func.cpp 1415 2019-11-29 09:08:35Z chuonghoangle $
//
// Copyright(c) 2018-2019 Renesas Electronics Corporation
// Copyright(c) 2018-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "DSADC_UNIT_Func.h"
#include "DSADC_UNIT_AgentController.h"
#include "DSADC_Base.h"          

/// Constructor of DSADC_UNIT_Func class
DSADC_UNIT_Func::DSADC_UNIT_Func (std::string name, DSADC_UNIT_AgentController *DSADC_UNIT_AgentControllerPtr, unsigned int ch_num):
                Cdsadc_unit_regif(name, 32, ch_num),
                BusSlaveFuncIf()
{//{{{
    assert(DSADC_UNIT_AgentControllerPtr != NULL);
    mDSADC_UNIT_AgentController = DSADC_UNIT_AgentControllerPtr;
    mDSADC_UNIT_AgentController->CommandInit(name);
    Cdsadc_unit_regif::set_instance_name(name);
    mModuleName = name;
    
    /// Initialize variables
    Initialize();
    
    mFilterSettingMap.insert(std::pair<unsigned int, unsigned int>(emCase1, 400));
    mFilterSettingMap.insert(std::pair<unsigned int, unsigned int>(emCase2, 200));
    mFilterSettingMap.insert(std::pair<unsigned int, unsigned int>(emCase3, 200));
    mFilterSettingMap.insert(std::pair<unsigned int, unsigned int>(emCase4, 400));
    mFilterSettingMap.insert(std::pair<unsigned int, unsigned int>(emCase5, 200));
    mFilterSettingMap.insert(std::pair<unsigned int, unsigned int>(emCase6, 200));
    mFilterSettingMap.insert(std::pair<unsigned int, unsigned int>(emCase7, 100));
    mFilterSettingMap.insert(std::pair<unsigned int, unsigned int>(emCase8, 25));
    mFilterSettingMap.insert(std::pair<unsigned int, unsigned int>(emCase9, 50));
}//}}}

///Destructor of DSADC_UNIT_Func class
DSADC_UNIT_Func::~DSADC_UNIT_Func (void)
{//{{{
}//}}}

void DSADC_UNIT_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    bool resetStatus = mDSADC_UNIT_AgentController->mDSADC->GetResetStatus("PRESETn");
    TlmG3mExtension *extension;
    trans.get_extension(extension);
    bool is_debug_master_access = false;
    if (extension != NULL){
        is_debug_master_access = (extension->getPeId() == DEBUG_MASTER_BMID);
    }
    /// When clock = 0 or assert reset, only bebug master read.
    if (!mDSADC_UNIT_AgentController->mDSADC->CheckClockPeriod("CLK_LSB") || resetStatus) {
        if (!debug || (debug && !is_debug_master_access)) {
            if (resetStatus) {
                re_printf("warning", "Cannot read during reset period\n");
            } else {
                re_printf("warning", "Cannot access register when clock is 0.\n");
            }
            return;
        } else {
            /// do nothing
        }
    }

    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        
        unsigned int addr = trans.get_address() & 0xFF;
        if ((0x20 <= addr) && (addr <= 0x3F)) {
            /// DSADnDIRj is read
            unsigned int vc_id = (addr - 0x20) >> 2;
            if (vc_id < mDSADC_UNIT_AgentController->mChannelNum) {
                Callback_DIR(vc_id);
            }
        } else if ((0x68 <= addr) && (addr <= 0x6B)) {
            /// DSADnUDIR is read
            Callback_UDIR();
        }
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

void DSADC_UNIT_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    /// Get extension infor
    TlmG3mExtension *extension;
    trans.get_extension(extension);
    bool is_debug_master_access = false;
    if (extension != NULL){
        is_debug_master_access = (extension->getPeId() == DEBUG_MASTER_BMID);
    }
    /// When clock = 0, only bebug master write.
    if (!mDSADC_UNIT_AgentController->mDSADC->CheckClockPeriod("CLK_LSB")) {
        if (!debug || (debug && !is_debug_master_access)) {
            re_printf("warning", "Cannot access register when clock is 0.\n");
            return;
        } else {
            /// do nothing
        }
    }
    /// Access write to register
    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
    unsigned int addr = trans.get_address() & 0xFF;
    if (GetStatusFlag(emADCT) || GetStatusFlag(emdCLBACT)) {
        if (addr <= 0x1F) {
            /// DSADnVCRj is written
            unsigned int vc_id = addr >> 2;
            if (vc_id < mDSADC_UNIT_AgentController->mChannelNum) {
                mDSADC_UNIT_AgentController->print_msg("warning","DSADnVCR%d is written while ADACT/CLBACT is set.\n", vc_id);
            }
        } else if ((0x54 <= addr) && (addr <= 0x57)) {
            /// DSADnUCR is written
            mDSADC_UNIT_AgentController->print_msg("warning","DSADnUCR is written while ADACT/CLBACT is set.\n");
        } else if ((0x70 <= addr) && (addr <= 0x73)) {
            /// DSADnSFTCR is written
            mDSADC_UNIT_AgentController->print_msg("warning","DSADnSFTCR is written while ADACT/CLBACT is set.\n");
        } else if ((0x7C <= addr) && (addr <= 0x7F)) {
            /// DSADnTDLVR is written
            mDSADC_UNIT_AgentController->print_msg("warning","DSADnTDLVR is written while ADACT/CLBACT is set.\n");
        } else if ((0x80 <= addr) && (addr <= 0x8F)) {
            /// DSADnULTBRm is written
            unsigned int id = (addr - 0x80) >> 2;
            mDSADC_UNIT_AgentController->print_msg("warning","DSADnULTBR%d is written while ADACT/CLBACT is set.\n", id);
        }
    }
    if (GetStatusExterTrigger(emSTTRGE) || GetStatusExterTrigger(emENDTRGE) || GetADSTTEvalue())
    {
        if ((0x58 <= addr) && (addr <= 0x5C))
        {
            mDSADC_UNIT_AgentController->print_msg("warning","DSADVCPTRR is written while ADSTTE/ENDTRGE/STTRGE is set.\n");
        }
    }
    if (GetStatusExterTrigger(emSTTRGE) || GetStatusExterTrigger(emENDTRGE)){
        if (addr <= 0x1F) {
            /// DSADnVCRj is written
            unsigned int vc_id = addr >> 2;
            if (vc_id < mDSADC_UNIT_AgentController->mChannelNum) {
                mDSADC_UNIT_AgentController->print_msg("warning","DSADnVCR%d is written while STTRGE/ENDTRGE is set.\n", vc_id);
            }
         } else if ((0x54 <= addr) && (addr <= 0x57)){
             /// DSADnUCR is written
             mDSADC_UNIT_AgentController->print_msg("warning","DSADnUCR is written while STTRGE/ENDTRGE is set.\n");
         }
    }
}//}}}

/// Reset operation of DSADC_UNIT_Func class
void DSADC_UNIT_Func::Reset (const bool is_active)
{//{{{
    Cdsadc_unit_regif::EnableReset(is_active);
}//}}}

/// Register handler command to Cdsadc_unit_regif class
void DSADC_UNIT_Func::RegisterHandler (const std::vector<std::string> &args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) { /// Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    } else {
        if (args[1] == "*") {   /// Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase(cmd.begin()+1);
                cmd.insert(cmd.begin()+1, reg_p->name());
                msg_return = Cdsadc_unit_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {    /// Set handle command to Cdsadc_unit_regif class
            msg_return = Cdsadc_unit_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// Virtual function of DSADnADSTCR register in Cdsadc_unit_regif class
void DSADC_UNIT_Func::cb_DSADnADSTCR_ADST (RegCBstr str)
{//{{{
    double mCalibTimePoint = mDSADC_UNIT_AgentController->GetCalibTimeStop();
    double mTimePoint = sc_time_stamp().to_double();
    if (!mDSADC_UNIT_AgentController->mDSADC->GetResetStatus("resetad_n") && mDSADC_UNIT_AgentController->mDSADC->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*DSADnADSTCR)["ADST"]) {
            if (mCalibTimePoint == 0)
            {
                mDSADC_UNIT_AgentController->NotifyEvent(DSADC_UNIT_AgentController::emStartTrigger, 0);
                if (GetStatusExterTrigger(true) || GetStatusExterTrigger(false))
                {
                mDSADC_UNIT_AgentController->print_msg("warning","It isn't recommend to set ADSTART when STTRGE/ENDTRGE is active.\n");
                }
                if (GetStatusFlag(true) || GetStatusFlag(false))
                {
                mDSADC_UNIT_AgentController->print_msg("warning","ADST is active when CLBACT/ADACT is set.\n");
                }

            } else {
                    if ((mTimePoint - mCalibTimePoint- 6 * mDSADC_UNIT_AgentController->mDSADC->mCLK_LSBPeriod) >= 0){
                        mDSADC_UNIT_AgentController->NotifyEvent(DSADC_UNIT_AgentController::emStartTrigger, 0);
                        mDSADC_UNIT_AgentController->ClearCalibTimeStop();
                        if (GetStatusExterTrigger(true) || GetStatusExterTrigger(false)){
                            mDSADC_UNIT_AgentController->print_msg("warning","It isn't recommend to set ADSTART when STTRGE/ENDTRGE is active.\n");
                        }
                    } else {
                        mDSADC_UNIT_AgentController->print_msg("warning","ADST trigger is less than 6 PCLK after calibration stop.\n");
                    }
                    }
        }
    }
    (*DSADnADSTCR)["ADST"] = 0;
}//}}}

/// Virtual function of DSADnADENDCR register in Cdsadc_unit_regif class
void DSADC_UNIT_Func::cb_DSADnADENDCR_ADEND (RegCBstr str)
{//{{{
    if (!mDSADC_UNIT_AgentController->mDSADC->GetResetStatus("resetad_n") && mDSADC_UNIT_AgentController->mDSADC->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*DSADnADENDCR)["ADEND"]) {
            mDSADC_UNIT_AgentController->NotifyEvent(DSADC_UNIT_AgentController::emEndTrigger, 0);
            if (GetStatusExterTrigger(true) || GetStatusExterTrigger(false))
            {
                mDSADC_UNIT_AgentController->print_msg("warning","It isn't recommend to set ADEND when STTRGE/ENDTRGE is active.\n");
            }

        }
    }
    (*DSADnADENDCR)["ADEND"] = 0;
}//}}}

/// Virtual function of DSADnCLBSTCR register in Cdsadc_unit_regif class
void DSADC_UNIT_Func::cb_DSADnCLBSTCR_CLBST (RegCBstr str)
{//{{{
    double mCalibTimePoint = mDSADC_UNIT_AgentController->GetCalibTimeStop();
    double mTimePoint = sc_time_stamp().to_double();
    mDSADC_UNIT_AgentController->ClearCalibTimeStop();
    if (!mDSADC_UNIT_AgentController->mDSADC->GetResetStatus("resetad_n") && mDSADC_UNIT_AgentController->mDSADC->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*DSADnCLBSTCR)["CLBST"]) {
            if (mCalibTimePoint == 0)
            {
                mDSADC_UNIT_AgentController->NotifyEvent(DSADC_UNIT_AgentController::emCalibStart, 0);
            } else
                {
                if ((mTimePoint - mCalibTimePoint- 6 * mDSADC_UNIT_AgentController->mDSADC->mCLK_LSBPeriod) >= 0)
                {
                    mDSADC_UNIT_AgentController->NotifyEvent(DSADC_UNIT_AgentController::emCalibStart, 0);
                } else
                    {
                        mDSADC_UNIT_AgentController->print_msg("warning","CLBST trigger is less than 6 PCLK after calibration stop.\n");
                    }
                }
    }
    }
    (*DSADnCLBSTCR)["CLBST"] = 0;
}//}}}

/// Virtual function of DSADnCLBEDCR register in Cdsadc_unit_regif class
void DSADC_UNIT_Func::cb_DSADnCLBEDCR_CLBEND (RegCBstr str)
{//{{{
    if (!mDSADC_UNIT_AgentController->mDSADC->GetResetStatus("resetad_n") && mDSADC_UNIT_AgentController->mDSADC->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*DSADnCLBEDCR)["CLBEND"]) {
            if (GetStatusFlag(false)) {
                // CLBACT = 1
                mDSADC_UNIT_AgentController->NotifyEvent(DSADC_UNIT_AgentController::emCalibStop, 0);
            }
        }
    }
    (*DSADnCLBEDCR)["CLBEND"] = 0;
}//}}}

/// Virtual function of DSADnVCPTRR register in Cdsadc_unit_regif class
void DSADC_UNIT_Func::cb_DSADnVCPTRR_VCPTR (RegCBstr str)
{//{{{
    if (GetStatusFlag(true) || GetStatusFlag(false)) {
        mDSADC_UNIT_AgentController->print_msg("warning","write to DSADnVCPTRR while DSADC is converting/calibrating \n");
        (*DSADnVCPTRR) = (unsigned int)(str.pre_data);
    }
}//}}}

/// Virtual function of DSADnECR register in Cdsadc_unit_regif class
void DSADC_UNIT_Func::cb_DSADnECR_PEC (RegCBstr str)
{//{{{
    if ((bool)(*DSADnECR)["ULEC"]) {
        (*DSADnER)["ULE"] = 0;
        (*DSADnER)["ULECAP"] = 0;
    }
    if ((bool)(*DSADnECR)["LLEC"]) {
        (*DSADnER)["LLE"] = 0;
        (*DSADnER)["ULECAP"] = 0;
    }
    if ((bool)(*DSADnECR)["OWEC"]) {
        (*DSADnER)["OWE"] = 0;
        (*DSADnER)["OWECAP"] = 0;
    }
    if ((bool)(*DSADnECR)["PEC"]) {
        (*DSADnER)["PE"] = 0;
        (*DSADnER)["PECAP"] = 0;
    }
    (*DSADnECR) = 0;
}//}}}

/// Get DSADnADTCR.ADSTTE value
bool DSADC_UNIT_Func::IsSyncStartEnable (void)
{//{{{
    return (bool)(*DSADnADTCR)["ADSTTE"];
}//}}}

/// Get DSADnADSR.ADACT/CLBACT value
bool DSADC_UNIT_Func::GetStatusFlag (const bool is_adact)
{//{{{
    if (is_adact) {
        return (bool)(*DSADnADSR)["ADACT"];
    } else {
        return (bool)(*DSADnADSR)["CLBACT"];
    }
}//}}}

/// Set DSADnADSR.ADACT/CLBACT value
void DSADC_UNIT_Func::SetStatusFlag (const bool is_adact, const bool is_active)
{//{{{
    if (is_adact) {
        (*DSADnADSR)["ADACT"] = is_active;
    } else {
        (*DSADnADSR)["CLBACT"] = is_active;
    }
}//}}}

/// Get DSADnADTCR.STTRGE/ENDTRGE value-add by trong
bool DSADC_UNIT_Func::GetStatusExterTrigger(const bool is_sttrge)
{//{{{
    if (is_sttrge) {
        return (bool)(*DSADnADTCR)["STTRGE"];
    } else {
        return (bool)(*DSADnADTCR)["ENDTRGE"];
    }
}//}}}

/// Update  DIRj/UCR registers
void DSADC_UNIT_Func::UpdateConversionResult (void)
{//{{{
    unsigned int vc_no = (unsigned int)(*DSADnVCPTRR)["VCPTR"];
    mIsDSADEInterrupt = false;
    
    // Check OWE error
    if ((unsigned int)(*DSADnDIR[vc_no])["WFLG"] == 1) {
        if ((unsigned int)(*DSADnER)["OWE"] == 0) {
            (*DSADnER)["OWE"] = 1;
            (*DSADnER)["OWECAP"] = vc_no;
            if ((bool)(*DSADnSFTCR)["OWEIE"]) {
                mIsDSADEInterrupt = true;
            }
        }
    }
    
    /// Check IDE error
    unsigned int idef = 1;
    if (mDSADC_UNIT_AgentController->mDSADC->mIDError[mDSADC_UNIT_AgentController->mAdId]) {
        idef = 0;
            if ((bool)(*DSADnSFTCR)["IDEIE"]) {
                mIsDSADEInterrupt = true;
            }
    }
    
    /// DIR register
    (*DSADnDIR[vc_no])["IDEF"] = idef;
    (*DSADnDIR[vc_no])["WFLG"] = 1;
    /// DIR.DR
    unsigned int dr_unmask = 0;
    double result = CalculateConvertValue(vc_no);
    unsigned int result_sign = 0;
    if (result > ((double)emSignMax/(double)emSignFmtConst)) {
        result_sign = (unsigned int)emSignMax;
    } else if (result <= (double)emMinValue) {
        result_sign = (unsigned int)emSignMin;
    } else {
        result_sign = (unsigned int)((unsigned int)(result * emSignFmtConst) & 0xFFFF);
    }
    mStampSignValue = result_sign;
    bool is_sign = false;
    if ((mDSADC_UNIT_AgentController->mDSADC->GetIsUnsignConfig()) && ((unsigned int)(*DSADnVCR[vc_no])["CNVCLS"] == 0)) {
        /// Unsigned fixed-point format
        unsigned int result_unsign = 0;
        if (result > ((double)emUnsignMax/(double)emUnsignFmtConst)) {
            result_unsign = (unsigned int)emUnsignMax;
        } else if (result <= (double)0) {
            result_unsign = 0;
        } else {
            result_unsign = (unsigned int)((unsigned int)(result * emUnsignFmtConst) & 0xFFFF);
        }
        dr_unmask = result_unsign;
    } else {
        /// Signed fixed-point format
        is_sign = true;
        dr_unmask = result_sign;
    }
    unsigned int dr_mask = dr_unmask & (~(unsigned int)((1 << (*DSADnUCR)["DFMT"]) - 1));
    (*DSADnDIR[vc_no])["DR"] = dr_mask;
    /// DIR.PRTY
    unsigned int prty = (unsigned int)((*DSADnDIR[vc_no])["IDEF"] & 0x1);
    for (unsigned int loop = 0; loop < emDRbit; loop++) {
        prty ^= (dr_mask >> loop) & 0x1;
    }
    if (mDSADC_UNIT_AgentController->mDSADC->mParityError[mDSADC_UNIT_AgentController->mAdId]) {
        prty = ~prty;
    }
    prty &= 0x1;
    (*DSADnDIR[vc_no])["PRTY"] = prty;
    
    /// For DFE
    mDrSignMask = result_sign & (~(unsigned int)((1 << (*DSADnUCR)["DFMT"]) - 1));

    /// UDIR register
    (*DSADnUDIR)["CHNUM"] = vc_no;
    (*DSADnUDIR)["UIDEF"] = idef;
    (*DSADnUDIR)["UWFLG"] = 1;
    for (unsigned int loop = 0; loop < emCHNUMbit; loop++) {
        prty ^= (vc_no >> loop) & 0x1;
    }
    prty &= 0x1;
    (*DSADnUDIR)["UPRTY"] = prty;
    (*DSADnUDIR)["UDR"] = dr_mask;
    /// UDPTR register
    (*DSADnUDPTRR)["UDPTR"] = vc_no;
    
    /// Check ULE/LLE errors & Boundary Flag Generating Signal
    mDSADC_UNIT_AgentController->NotifyEvent(DSADC_UNIT_AgentController::emAssertDSADVCI, vc_no);
    unsigned int table_no = (unsigned int)(*DSADnVCR[vc_no])["VCULLMTBS"];
    if ((bool)(*DSADnVCR[vc_no])["VCULME"]) {
        if (((is_sign) && ((short)dr_unmask > (short)(*DSADnULTBR[table_no])["ULMTB"])) ||
        ((!is_sign) && ((unsigned short)dr_unmask > (unsigned short)(*DSADnULTBR[table_no])["ULMTB"]))) {
            mDSADC_UNIT_AgentController->NotifyEvent(DSADC_UNIT_AgentController::emAssertDSADUE, 0);
            if (((unsigned int)(*DSADnER)["ULE"] == 0) && ((unsigned int)(*DSADnER)["LLE"] == 0)) {
                (*DSADnER)["ULE"] = 1;
                (*DSADnER)["ULECAP"] = vc_no;
                if ((bool)(*DSADnVCR[vc_no])["ULEIE"]) {
                    mIsDSADEInterrupt = true;
                }
            }
        }
    }
    if ((bool)(*DSADnVCR[vc_no])["VCLLME"]) {
        if (((is_sign) && ((short)dr_unmask < (short)(*DSADnULTBR[table_no])["LLMTB"])) ||((!is_sign) && 
           ((unsigned short)dr_unmask < (unsigned short)(*DSADnULTBR[table_no])["LLMTB"]))) {
            mDSADC_UNIT_AgentController->NotifyEvent(DSADC_UNIT_AgentController::emAssertDSADLE, 0);
            if (((unsigned int)(*DSADnER)["LLE"] == 0) && ((unsigned int)(*DSADnER)["ULE"] == 0)) {
                (*DSADnER)["LLE"] = 1;
                (*DSADnER)["ULECAP"] = vc_no;
                if ((bool)(*DSADnVCR[vc_no])["ULEIE"]) {
                    mIsDSADEInterrupt = true;
                }
            }
        }
    }
}//}}}

/// Get VCRj.ADIE value
bool DSADC_UNIT_Func::IsADInterruptEnable (const bool dsadrgt_value)
{//{{{
    unsigned int vc_no = (unsigned int)(*DSADnVCPTRR)["VCPTR"];
    bool ret = false;
    if ((bool)(*DSADnVCR[vc_no])["ADIE"]) {
        if (((unsigned int)(*DSADnUCR)["RDMA"] == 0) ||
        (((unsigned int)(*DSADnUCR)["RDMA"] == 1) && dsadrgt_value)) {
            ret = true;
        }
    }
    return ret;
}//}}}

/// Check DSADE Interrupt condition
bool DSADC_UNIT_Func::IsErrorInterrupt (void)
{//{{{
    return mIsDSADEInterrupt;
}//}}}

/// Update  VCPTR registers
void DSADC_UNIT_Func::UpdateVCPointer (const bool is_end_trigger)
{//{{{
    unsigned int current_vc = (unsigned int)(*DSADnVCPTRR)["VCPTR"];
    unsigned int next_vc = 0;
    if ((is_end_trigger && (bool)(*DSADnUCR)["VPRSTE"]) || (current_vc == (unsigned int)(*DSADnUCR)["VCEP"])) {
        next_vc = 0;
    } else {
        next_vc = current_vc + 1;
    }
    (*DSADnVCPTRR)["VCPTR"] = next_vc;
}//}}}

/// Get ADTCR.ENDTRGE/STTRGE value
unsigned int DSADC_UNIT_Func::TriggerType (void)
{//{{{
    return (unsigned int)(*DSADnADTCR) & 0x3;
}//}}}

/// Update DSADnTSVAL register
void DSADC_UNIT_Func::UpdateTimeStamp (const bool is_active, const unsigned int tx_value)
{//{{{
       if (is_active) {
            /// DSADRGT is asserted
            (*DSADnTSVAL)["TSVAL"] = tx_value;
            unsigned int vc_no = (unsigned int)(*DSADnVCPTRR)["VCPTR"];
            if (vc_no < mDSADC_UNIT_AgentController->mChannelNum) {
                (*DSADnTSVAL)["TSDR"] = mStampSignValue;
            }
        } else {
            /// AD conversion stopped
            (*DSADnTSVAL)["TSVAL"] = 0x7F;
            (*DSADnTSVAL)["TSDR"] = 0;
        }
}//}}}

/// Check registers setting
bool DSADC_UNIT_Func::CheckSetting (const bool is_converting)
{//{{{
    bool convert_enable = true;
    unsigned int vc_no = 0;
    unsigned int vcptr = (unsigned int)(*DSADnVCPTRR)["VCPTR"];
    unsigned int vcep = (unsigned int)(*DSADnUCR)["VCEP"];
    if (is_converting) {
        /// Check setting for next Virtual Channel
        if ((vcptr == vcep) || (vcep == 0)) {
            vc_no = 0;
        } else {
            vc_no = vcptr + 1;
        }
    } else {
        /// Check setting for current Virtual Channel in DSADnVCPTRR.VCPTR
        vc_no = vcptr;
    }
    
    if (vc_no >= mDSADC_UNIT_AgentController->mChannelNum) {
        return false;
    }
    
    unsigned int cnvcls = (unsigned int)(*DSADnVCR[vc_no])["CNVCLS"];
    bool tde = mDSADC_UNIT_AgentController->mDSADC->GetIsPinDiagEnable();
    
    /// Check DSADCADGCR.ODDE/ODE values
    unsigned int odde_ode = mDSADC_UNIT_AgentController->mDSADC->GetCheckDisconnectDetectSetup();
    if (((odde_ode & 0x1) == 1) && (tde || (cnvcls != 0))) {
        mDSADC_UNIT_AgentController->print_msg("warning","Prohibited setting of DSADCADGCR.ODE & DSADCTDCR.TDE or DSADnVCR%d.CNVCLS.\n", vc_no);
        convert_enable = false;
    }
    
    /// Check DSADCTDCR.TDE value
    if (tde && (cnvcls == 3)) {
        mDSADC_UNIT_AgentController->print_msg("warning","Prohibited setting of DSADCTDCR.TDE & DSADnVCR%d.CNVCLS.\n", vc_no);
        convert_enable = false;
    }

    /// Check DSADnVCRj.ORT/TPVSL/DSDFTYP values
    unsigned int filter_setting = ((unsigned int)(*DSADnVCR[vc_no]) >> 16) & 0xFF;
    std::map<unsigned int, unsigned int>::iterator it;
    bool is_filter_match = false;
    bool is_tS_match = false;
    for (it = mFilterSettingMap.begin(); it != mFilterSettingMap.end(); it++) {
        if (it->first == filter_setting) {
            is_filter_match = true;
            if (it->second == mDSADC_UNIT_AgentController->mDSADC->mtS) {
                is_tS_match = true;
            }
            break;
        }
    }
    if (!is_filter_match) {
        mDSADC_UNIT_AgentController->print_msg("warning","Invalid setting of DSADnVCR%d.ORT/TPVSL/DSDFTYP bits.\n", vc_no);
    } else {
        if (!is_tS_match) {
            mDSADC_UNIT_AgentController->print_msg("warning","tS value is not matched with DSADnVCR%d.ORT/TPVSL/DSDFTYP setting.\n", vc_no);
        }
        if (mDSADC_UNIT_AgentController->mDSADC->mFosFreq != emValidFosFreq) {
            mDSADC_UNIT_AgentController->print_msg("warning","Fos frequency is not matched with DSADnVCR%d.ORT/TPVSL/DSDFTYP setting.\n", vc_no);
        }
    }
    
    /// Check DSADnVCRj.CNVCLS/GCTRL values
    unsigned int gctrl = (unsigned int)(*DSADnVCR[vc_no])["GCTRL"];
    unsigned int gain = (unsigned int)(*DSADnVCR[vc_no])["GAIN"];
    unsigned int reso0 = (unsigned int)(*DSADnUCR)["RESO0"];
    if (((cnvcls <= 0x2) && (gctrl >= mDSADC_UNIT_AgentController->mChannelNum)) ||
    ((cnvcls == 0x3) && (gctrl != 0x0) && (gctrl != 0x4) && (gctrl != 0x7) && (gctrl != 0x8) && (gctrl != 0xC))) {
        mDSADC_UNIT_AgentController->print_msg("warning","Prohibited setting of DSADnVCR%d.CNVCLS/GCTRL bits.\n", vc_no);
        convert_enable = false;
    } else if ((cnvcls == 0x3) && (((reso0 == 0x0) && (gain > 0x1)) || ((reso0 == 0x1) && (gain == 0x3)))) {
        mDSADC_UNIT_AgentController->print_msg("warning","Prohibited setting of DSADnUCR.RESO0 & DSADnVCR%d.GAIN in Self-diagnosis mode.\n", vc_no);
    }
    
    /// Check DSADnUCR.DFMT value
    if ((unsigned int)(*DSADnUCR)["DFMT"] > 0x8) {
        mDSADC_UNIT_AgentController->print_msg("warning","Prohibited setting of DSADnUCR.DFMT bit.\n");
        convert_enable = false;
    }
    /// Check DSADnUCR.VCEP value
    if ((unsigned int)(*DSADnUCR)["VCEP"] >= mDSADC_UNIT_AgentController->mChannelNum) {
        mDSADC_UNIT_AgentController->print_msg("warning","Prohibited setting of DSADnUCR.VCEP bit.\n");
        convert_enable = false;
    }
    /// Check DSADnVCPTRR.VCPTR value
    if ((unsigned int)(*DSADnVCPTRR)["VCPTR"] > (unsigned int)(*DSADnUCR)["VCEP"]) {
        mDSADC_UNIT_AgentController->print_msg("warning","Setting DSADnVCPTRR.VCPTR > DSADnUCR.VCEP is prohibited.\n");
        convert_enable = false;
    }
    
    /// Check ADSVREFH
    double adsvrefh = mDSADC_UNIT_AgentController->GetAnalogValue(true, 0);
    if (((0 < adsvrefh) && (adsvrefh < 4.5)) || (mDSADC_UNIT_AgentController->mDSADC->mADSVCC < adsvrefh)) {
        mDSADC_UNIT_AgentController->print_msg("warning","ADSVREFH value is out of guarantee range.\n");
    } else if (adsvrefh <= 0) {
        mDSADC_UNIT_AgentController->print_msg("warning","ADSVREFH value is less than or equal 0.\n");
        convert_enable = false;
    }
    
    /// Check tS
    if (mDSADC_UNIT_AgentController->mDSADC->mtS == 0) {
        mDSADC_UNIT_AgentController->print_msg("warning","tS value is 0.\n");
        convert_enable = false;
    }
    
    /// Check ULMTB/LLMTB
    bool is_sign = true;
    if ((mDSADC_UNIT_AgentController->mDSADC->GetIsUnsignConfig()) && ((unsigned int)(*DSADnVCR[vc_no])["CNVCLS"] == 0)) {
        is_sign = false;
    }
    unsigned int table_no = (unsigned int)(*DSADnVCR[vc_no])["VCULLMTBS"];
    if (((bool)(*DSADnVCR[vc_no])["VCULME"]) && ((bool)(*DSADnVCR[vc_no])["VCLLME"])) {
        if (((is_sign) && ((short)(*DSADnULTBR[table_no])["LLMTB"] > (short)(*DSADnULTBR[table_no])["ULMTB"])) || 
           ((!is_sign) && ((unsigned short)(*DSADnULTBR[table_no])["LLMTB"] > (unsigned short)(*DSADnULTBR[table_no])["ULMTB"]))) {
            mDSADC_UNIT_AgentController->print_msg("warning","DSADnULTBR%d.LLMTB is greater than DSADnULTBR%d.ULMTB.\n", table_no, table_no);
            convert_enable = false;
        }
    }
    
    return convert_enable;
}//}}}

/// DSADnTDLVR.ANmkLV value
unsigned int DSADC_UNIT_Func::GetPinDiagBitValue (const unsigned int an_no)
{//{{{
    return ((unsigned int)(*DSADnTDLVR) >> an_no) & 0x1;
}//}}}

/// Get DFE info
bool DSADC_UNIT_Func::IsDFEEntry (unsigned int &req, unsigned int &tag, unsigned int &data)
{//{{{
    bool ret = false;
    unsigned int vc_no = (unsigned int)(*DSADnVCPTRR)["VCPTR"];
    if ((bool)(*DSADnVCR[vc_no])["DFENT"]) {
        ret = true;
        req = (unsigned int)(*DSADnUCR)["DFES"];
        tag = (unsigned int)(*DSADnVCR[vc_no])["DFTAG"];
        data = mDrSignMask;
    }
    return ret;
}//}}}

/// Update Calibration result
void DSADC_UNIT_Func::UpdateCalibResult (void)
{//{{{
    mCalibFinish = true;
    mCalibValue = mDSADC_UNIT_AgentController->mDSADC->mtCoeff;
}//}}}

/// Initialize variables
void DSADC_UNIT_Func::Initialize (void)
{//{{{
    mDrSignMask = 0;
    mIsDSADEInterrupt = false;
    mCalibFinish = false;
    mCalibValue = 1;
    mStampSignValue = 0;
}//}}}

/// Called after DSADnDIRj is read
void DSADC_UNIT_Func::Callback_DIR (const unsigned int vc_no)
{//{{{
    if ((bool)(*DSADnSFTCR)["RDCLRE"]) {
        *DSADnDIR[vc_no] = 0;
        if ((unsigned int)(*DSADnUDIR)["CHNUM"] == vc_no) {
            (*DSADnUDIR) = 0;
        }
    }
    (*DSADnDIR[vc_no])["WFLG"] = 0;
    if ((unsigned int)(*DSADnUDIR)["CHNUM"] == vc_no) {
        (*DSADnUDIR)["UWFLG"] = 0;
    }
    CheckParityError(vc_no);
}//}}}

/// Called after DSADnUDIR is read
void DSADC_UNIT_Func::Callback_UDIR (void)
{//{{{
    unsigned int vc_no = (unsigned int)(*DSADnUDIR)["CHNUM"];
    if ((bool)(*DSADnSFTCR)["RDCLRE"]) {
        (*DSADnUDIR) = 0;
        (*DSADnDIR[vc_no]) = 0;
    }
    (*DSADnUDIR)["UWFLG"] = 0;
    (*DSADnDIR[vc_no])["WFLG"] = 0;
    
    CheckParityError(vc_no);
}//}}}

/// Calculate AD conversion result
double DSADC_UNIT_Func::CalculateConvertValue (const unsigned int vc_no)
{//{{{
    /// Get analog value
    unsigned int cnvcls = (unsigned int)(*DSADnVCR[vc_no])["CNVCLS"];
    unsigned int gctrl = (unsigned int)(*DSADnVCR[vc_no])["GCTRL"];
    unsigned int an_p_no = 0;
    unsigned int an_n_no = 0;
    double an_p_val = 0;
    double an_n_val = 0;
    /// Get analog value
    switch (cnvcls) {
        case emSingleVrefL :
        case emSingleVrefL_2 :
            an_p_no = gctrl;
            an_p_val = mDSADC_UNIT_AgentController->GetAnalogValue(false, an_p_no);
            break;
        case emDifferential :
            an_p_no = gctrl & 0x6;
            an_p_val = mDSADC_UNIT_AgentController->GetAnalogValue(false, an_p_no);
            an_n_no = an_p_no + 1;
            an_n_val = mDSADC_UNIT_AgentController->GetAnalogValue(false, an_n_no);
            break;
        default:    // emSelfDiag
                break;
    }
    
    // Calculate conversion value
    double refh_val = mDSADC_UNIT_AgentController->GetAnalogValue(true, 0);
    unsigned int gain_val = (unsigned int)(0x1 << (unsigned int)(*DSADnVCR[vc_no])["GAIN"]);
    double result = 0;
    unsigned int odde_ode = mDSADC_UNIT_AgentController->mDSADC->GetCheckDisconnectDetectSetup();
    if (((odde_ode & 0x1) == 1) && (((odde_ode >> 1) == 1) || (mDSADC_UNIT_AgentController->mDSADC->mDisconnectError[mDSADC_UNIT_AgentController->mAdId]))) {
        if (cnvcls == 0) {
            /// Disconnect is detected
            result = 0;
        }
    } else {
        switch (cnvcls) {
            case emSingleVrefL :
                result = (an_p_val / refh_val) * gain_val;
                break;
            case emSingleVrefL_2 :
                result = (an_p_val / refh_val - 0.5) * gain_val;
                break;
            case emDifferential :
                result = ((an_p_val - an_n_val) / refh_val) * gain_val;
                break;
            default:  /// emSelfDiag
                switch (gctrl) {
                    case 0 :
                        result = 0;
                        break;
                    case 0x4 :
                        result = 0.5;
                        break;
                    case 0x7 :
                        result = 1;
                        break;
                    case 0x8 :
                        result = -1;
                        break;
                    default :   /// 0xC 
                        result = -0.5;
                        break;
                }
                break;
        }
    }
    
    if(mCalibFinish) {
        /// Apply Calibration
        result *= mCalibValue;
    }
    
    return result;
}//}}}

/// Check Parity error
void DSADC_UNIT_Func::CheckParityError (const unsigned int vc_no)
{//{{{
    if (mDSADC_UNIT_AgentController->mDSADC->mParityError[mDSADC_UNIT_AgentController->mAdId]) {
        /// Parity error occur
        if ((unsigned int)(*DSADnER)["PE"] == 0) {
            (*DSADnER)["PE"] = 1;
            (*DSADnER)["PECAP"] = vc_no;
            if ((bool)(*DSADnSFTCR)["PEIE"]) {
                mDSADC_UNIT_AgentController->NotifyEvent(DSADC_UNIT_AgentController::emAssertDSADPE, 0);
            }
        }
    }
}//}}}

//Get ADSTTE value
bool DSADC_UNIT_Func::GetADSTTEvalue()
{///{{{
    return (bool)(*DSADnADTCR)["ADSTTE"];
}///}}}

/// Get DSADDIR value
uint32_t DSADC_UNIT_Func::GetDSADDIR()
{///{{{
    for (unsigned int i = 0; i< mDSADC_UNIT_AgentController->mChannelNum; i++){
    unsigned int temp = (uint32_t) (*DSADnDIR[i]);
    mDSADC_UNIT_AgentController->print_msg("info","Channel %d : 0x%08X\n", i, temp);
    }
    ///<None return
    return 1;
}///}}}

/// Get DSADADSR value
uint32_t DSADC_UNIT_Func::GetDSADADSR()
{///{{{
    return (uint32_t) (*DSADnADSR);
}///}}}

/// Get DSADUDPTRR value
uint32_t DSADC_UNIT_Func::GetDSADUDPTRR()
{///{{{
    return (uint32_t) (*DSADnUDPTRR);
}///}}}

/// Get DSADADSR value
uint32_t DSADC_UNIT_Func::GetDSADUDIR()
{///{{{
    return (uint32_t) (*DSADnUDIR);
}///}}}

/// Get DSADTSVAL value
uint32_t DSADC_UNIT_Func::GetDSADTSVAL()
{///{{{
    return (uint32_t) (*DSADnTSVAL);
}///}}}

/// Get DSADER value
uint32_t DSADC_UNIT_Func::GetDSADER()
{///{{{
    return (uint32_t) (*DSADnER);
}///}}}
// vim600: set foldmethod=marker :
