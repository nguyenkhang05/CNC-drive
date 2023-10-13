// ---------------------------------------------------------------------
// $Id: CADC_UNIT_Func.cpp,v 1.4 2019/09/27 08:40:42 ngathunguyen Exp $
//
// Copyright(c) 2016-2018 Renesas Electronics Corporation
// Copyright(c) 2016-2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "CADC_UNIT_Func.h"
#include "CADC_UNIT_AgentController.h"
#include "CADC.h"
#include "CADC_Func.h"

///Constructor of CADC_UNIT_Func class
CADC_UNIT_Func::CADC_UNIT_Func (std::string name, CADC_UNIT_AgentController *CADC_UNIT_AgentControllerPtr):
                Ccadc_unit_regif(name, 32)
{//{{{
    assert(CADC_UNIT_AgentControllerPtr != NULL);
    mCADC_UNIT_AgentController = CADC_UNIT_AgentControllerPtr;
    mCADC_UNIT_AgentController->CommandInit(name);
    Ccadc_unit_regif::set_instance_name(name);
    mModuleName = name;
    
    // Initialize variables
    Initialize();
    
}//}}}

///Destructor of CADC_UNIT_Func class
CADC_UNIT_Func::~CADC_UNIT_Func (void)
{//{{{
}//}}}

// Virtual function of Ccadc_uint_regif class
void CADC_UNIT_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (!debug) {
        unsigned int addr = trans.get_address() & 0xFF;
        if ((0x6C <= addr) && (addr <= 0x6F)) {
            Callback_SMPCR();
        }    
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        if ((0x20 <= addr) && (addr <= 0x3F)) {
            // CADCnDIRj is read
            unsigned int vc_id = (addr - 0x20) >> 2;
            Callback_DIR(vc_id);
        } else if ((0x68 <= addr) && (addr <= 0x6B)) {
            // CADCnUDIR is read
            Callback_UDIR();
        }    
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

// Virtual function of Ccadc_uint_regif class
void CADC_UNIT_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
    
    if (GetStatusFlag(true)) {
        unsigned int addr = trans.get_address() & 0xFF;
        if ((0x0 <= addr) && (addr <= 0x1F)) {
            // CADCnVCRj is written
            unsigned int vc_id = addr >> 2;
            mCADC_UNIT_AgentController->print_msg("warning","CADCnVCR%d is written while CADC is converting.\n", vc_id);
        } else if ((0x54 <= addr) && (addr <= 0x57)) {
            // CADCnUCR is written
            mCADC_UNIT_AgentController->print_msg("warning","CADCnUCR is written while CADC is converting.\n");
        } else if ((0x70 <= addr) && (addr <= 0x73)) {
            // CADCnSFTCR is written
            mCADC_UNIT_AgentController->print_msg("warning","CADCnSFTCR is written while CADC is converting.\n");
        } else if ((0x7C <= addr) && (addr <= 0x7F)) {
            // CADCnTDLVR is written
            mCADC_UNIT_AgentController->print_msg("warning","CADCnTDLVR is written while CADC is converting.\n");
        } else if ((0x80 <= addr) && (addr <= 0x8F)) {
            // CADCnULTBRm is written
            unsigned int id = (addr - 0x80) >> 2;
            mCADC_UNIT_AgentController->print_msg("warning","CADCnULTBR%d is written while CADC is converting.\n", id);
        }
    }
}//}}}

///Reset operation of CADC_UNIT_Func class
void CADC_UNIT_Func::Reset (const bool is_active)
{//{{{
    Ccadc_unit_regif::EnableReset(is_active);
}//}}}

///Register handler command to Ccadc_unit_regif class
void CADC_UNIT_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Ccadc_unit_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Ccadc_unit_regif class
            msg_return = Ccadc_unit_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// Virtual function of CADCnADSTCR register in Ccadc_unit_regif class
void CADC_UNIT_Func::cb_CADCnADSTCR_CADC00ADST (RegCBstr str)
{//{{{
    if (!mCADC_UNIT_AgentController->mCADC->GetResetStatus("resetad_n") && mCADC_UNIT_AgentController->mCADC->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*CADCnADSTCR)["CADC00ADST"]) {
            mCADC_UNIT_AgentController->NotifyEvent(CADC_UNIT_AgentController::emStartTrigger, 0);
        }
    }
    (*CADCnADSTCR)["CADC00ADST"] = 0;
}//}}}

/// Virtual function of CADCnADENDCR register in Ccadc_unit_regif class
void CADC_UNIT_Func::cb_CADCnADENDCR_CADC00ADEND (RegCBstr str)
{//{{{
    if (!mCADC_UNIT_AgentController->mCADC->GetResetStatus("resetad_n") && mCADC_UNIT_AgentController->mCADC->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*CADCnADENDCR)["CADC00ADEND"]) {
            mCADC_UNIT_AgentController->NotifyEvent(CADC_UNIT_AgentController::emEndTrigger, 0);
        }
    }
    (*CADCnADENDCR)["CADC00ADEND"] = 0;
}//}}}

/// Virtual function of CADCnCLBST register in Ccadc_unit_regif class
void CADC_UNIT_Func::cb_CADCnCLBSTCR_CADC00CLBST (RegCBstr str)
{//{{{
    if (!mCADC_UNIT_AgentController->mCADC->GetResetStatus("resetad_n") && mCADC_UNIT_AgentController->mCADC->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*CADCnCLBSTCR)["CADC00CLBST"]) {
            mCADC_UNIT_AgentController->NotifyEvent(CADC_UNIT_AgentController::emCalibStart, 0);
        }
    }
    (*CADCnCLBSTCR)["CADC00CLBST"] = 0;
}//}}}

/// Virtual function of CADCnCLBEND register in Ccadc_unit_regif class
void CADC_UNIT_Func::cb_CADCnCLBEDCR_CADC00CLBEND (RegCBstr str)
{//{{{
    if (!mCADC_UNIT_AgentController->mCADC->GetResetStatus("resetad_n") && mCADC_UNIT_AgentController->mCADC->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*CADCnCLBEDCR)["CADC00CLBEND"]) {
            if (GetStatusFlag(false)) {
                // CLBACT = 1
                mCADC_UNIT_AgentController->NotifyEvent(CADC_UNIT_AgentController::emCalibStop, 0);
            }
        }
    }
    (*CADCnCLBEDCR)["CADC00CLBEND"] = 0;
}//}}}

/// Virtual function of CADCnVCPTR register in Ccadc_unit_regif class
void CADC_UNIT_Func::cb_CADCnVCPTRR_CADC00VCPTR (RegCBstr str)
{//{{{
    if (GetStatusFlag(true)) {
        mCADC_UNIT_AgentController->print_msg("warning","Cannot write to CADCnVCPTRR while CADC is converting.\n");
        (*CADCnVCPTRR) = (unsigned int)(str.pre_data);
    }
}//}}}

/// Virtual function of CADCnECR register in Ccadc_unit_regif class
void CADC_UNIT_Func::cb_CADCnECR_CADC00ULEC (RegCBstr str)
{//{{{
    if ((bool)(*CADCnECR)["CADC00ULEC"]) {
        (*CADCnER)["CADC00ULE"] = 0;
        (*CADCnER)["CADC00ULECAP"] = 0;
    }
    if ((bool)(*CADCnECR)["CADC00LLEC"]) {
        (*CADCnER)["CADC00LLE"] = 0;
        (*CADCnER)["CADC00ULECAP"] = 0;
    }
    if ((bool)(*CADCnECR)["CADC00OWEC"]) {
        (*CADCnER)["CADC00OWE"] = 0;
        (*CADCnER)["CADC00OWECAP"] = 0;
    }
    if ((bool)(*CADCnECR)["CADC00PEC"]) {
        (*CADCnER)["CADC00PE"] = 0;
        (*CADCnER)["CADC00PECAP"] = 0;
    }
    (*CADCnECR) = 0;
}//}}}

/// Get CADCnADSR.CADC00ADACT/CADC00CLBACT value
bool CADC_UNIT_Func::GetStatusFlag (const bool is_adact)
{//{{{
    if (is_adact) {
        return (*CADCnADSR)["CADC00ADACT"];
    } else {
        return (*CADCnADSR)["CADC00CLBACT"];
    }
}//}}}

/// Set CADCnADSR.CADC00ADACT/CADC00CLBACT value
void CADC_UNIT_Func::SetStatusFlag (const bool is_adact, const bool is_active)
{//{{{
    if (is_adact) {
        (*CADCnADSR)["CADC00ADACT"] = is_active;
    } else {
        (*CADCnADSR)["CADC00CLBACT"] = is_active;
    }
}//}}}

/// Update  DIRj/UCR registers
void CADC_UNIT_Func::UpdateConversionResult (void)
{//{{{
    unsigned int vc_no = (*CADCnVCPTRR)["CADC00VCPTR"];
    
    mIsDIRSetting = true;
    mIsUDIRSetting = true;
    mCADC_UNIT_AgentController->NotifyEvent(CADC_UNIT_AgentController::emUpdateDIR, vc_no);
    
}//}}}

/// Get VCRj.CADC00ADIE value
bool CADC_UNIT_Func::IsADInterruptEnable (const bool dsadrgt_value)
{//{{{
    unsigned int vc_no = (*CADCnVCPTRR)["CADC00VCPTR"];
    unsigned ret = false;
    if ((bool)(*CADCnVCR[vc_no])["CADC00ADIE"]) {
        if (((*CADCnUCR)["CADC00RDMA"] == 0) || (((*CADCnUCR)["CADC00RDMA"] == 1) && dsadrgt_value)) {
            ret = true;
        }
    }
    return ret;
}//}}}

/// Check CADCE Interrupt condition
bool CADC_UNIT_Func::IsErrorInterrupt (void)
{//{{{
    return mIsCADCEInterrupt;
}//}}}

/// Update  CADCnVCPTRR registers
void CADC_UNIT_Func::UpdateVCPointer (const bool is_end_trigger)
{//{{{
    unsigned int current_vc = (*CADCnVCPTRR)["CADC00VCPTR"];
    unsigned int next_vc = 0;
    if ((is_end_trigger && (bool)(*CADCnUCR)["CADC00VPRSTE"]) || (current_vc == (*CADCnUCR)["CADC00VCEP"])) {
        next_vc = 0;
    } else {
        next_vc = current_vc + 1;
    }
    (*CADCnVCPTRR)["CADC00VCPTR"] = next_vc;
}//}}}

/// Get ADTCR.ENDTRGE/STTRGE value
unsigned int CADC_UNIT_Func::TriggerType (void)
{//{{{
    return (*CADCnADTCR) & 0x3;
}//}}}

/// Get UCR.CADC00VCEP value
unsigned int CADC_UNIT_Func::GetVCEndPointer (void)
{//{{{
    return (*CADCnUCR)["CADC00VCEP"];
}//}}}

/// Check registers setting
bool CADC_UNIT_Func::CheckSetting (const bool is_converting)
{//{{{
    bool convert_enable = true;
    unsigned int vc_no = 0;
    unsigned int vc_temp = (*CADCnVCPTRR)["CADC00VCPTR"];
    if (is_converting) {
        // Check setting for next Virtual Channel
        if (vc_temp == (*CADCnUCR)["CADC00VCEP"]) {
            vc_no = 0;
        } else {
            vc_no = vc_temp + 1;
        }
    } else {
        // Check setting for current Virtual Channel in CADCnVCPTRR.CADC00VCPTR
        vc_no = vc_temp;
    }
    unsigned int cnvcls = (*CADCnVCR[vc_no])["CADC00CNVCLS"];

    // Check setting for A/D conversion Ongoing Virtual Channel Number
    if (vc_temp > (*CADCnUCR)["CADC00VCEP"]) {
            mCADC_UNIT_AgentController->print_msg("warning","A setting VCPTR exceeding the end pointer is prohibited.\n");
            convert_enable = false;
    }
    
    // Check CADCTDCR/CADCTDE values
    bool value = mCADC_UNIT_AgentController->mCADC->mCADC_Func->ValueODE();
    bool value_TDE = mCADC_UNIT_AgentController->mCADC->mCADC_Func->IsPinDiagEnable();
    if ((value == 0x1) && (cnvcls != 0x0)) {
        mCADC_UNIT_AgentController->print_msg("warning","When setting CADCADGCR.CADCODE as 1, if CADCnVCRj.CADCnCNVCLS[1:0] value is  set to anything but 0H, A/D conversion is prohibited.\n");
        convert_enable = false;
    }

    if ((value == 0x1) && (value_TDE == 0x1)) {
        mCADC_UNIT_AgentController->print_msg("warning","Setting CADCODE bit in 1 simultaneously with pin level self-diagnosis (CADCTDE=1) is prohibited.\n");
        convert_enable = false;
    }

     //Check CADCnVCRj.CADC00CNVCLS/CADC00GCTRL values
    unsigned int gctrl = (*CADCnVCR[vc_no])["CADC00GCTRL"];
    if (((cnvcls <= 0x2) && (gctrl >= mCADC_UNIT_AgentController->mChannelNum)) || ((cnvcls == 0x3) && (gctrl != 0x0) && (gctrl != 0x4) && (gctrl != 0x7) && (gctrl != 0x8) && (gctrl != 0xC))) {
        mCADC_UNIT_AgentController->print_msg("warning","Prohibited setting of CADCnVCR%d.CADC00CNVCLS/CADC00GCTRL bits.\n", vc_no);
        convert_enable = false;
    }
    
    // Check CADCnUCR.CADC00DFMT value
    if ((*CADCnUCR)["CADC00DFMT"] > 0x8) {
        mCADC_UNIT_AgentController->print_msg("warning","Prohibited setting of CADCnUCR.CADC00DFMT bit.\n");
        convert_enable = false;
    }
    // Check CADCnUCR.CADC00VCEP value
    if ((*CADCnUCR)["CADC00VCEP"] >= mCADC_UNIT_AgentController->mChannelNum) {
        mCADC_UNIT_AgentController->print_msg("warning","Prohibited setting of CADCnUCR.CADC00VCEP bit.\n");
        convert_enable = false;
    }
    // Check CADCnVCPTRR.CADC00VCPTR value
    if ((*CADCnVCPTRR)["CADC00VCPTR"] > (*CADCnUCR)["CADC00VCEP"]) {
        mCADC_UNIT_AgentController->print_msg("warning","Setting CADCnVCPTRR.CADC00VCPTR > CADCnUCR.CADC00VCEP is prohibited.\n");
        convert_enable = false;
    }
    
    // Check ADSVREFH
    double adsvrefh = mCADC_UNIT_AgentController->GetAnalogValue(true, 0);
    if (((0 < adsvrefh) && (adsvrefh < 4.5)) || (mCADC_UNIT_AgentController->mCADC->mADSVCC < adsvrefh)) {
        mCADC_UNIT_AgentController->print_msg("warning","ADSVREFH value is out of guarantee range.\n");
    } else if (adsvrefh <= 0) {
        mCADC_UNIT_AgentController->print_msg("warning","ADSVREFH value is less than or equal 0.\n");
        convert_enable = false;
    }
    
    // Check CADC00ULMTB/CADC00LLMTB
    bool is_sign = true;
    if ((mCADC_UNIT_AgentController->mCADC->mCADC_Func->IsUnsignConfig()) && ((*CADCnVCR[vc_no])["CADC00CNVCLS"] == 0)) {
        is_sign = false;
    }
    unsigned int table_no = (*CADCnVCR[vc_no])["CADC00VCULLMTBS"];
    if (((bool)(*CADCnVCR[vc_no])["CADC00VCULME"]) && ((bool)(*CADCnVCR[vc_no])["CADC00VCLLME"])) {
        if (((is_sign) && ((short)(*CADCnULTBR[table_no])["CADC00LLMTB"] > (short)(*CADCnULTBR[table_no])["CADC00ULMTB"])) || ((!is_sign) && ((unsigned short)(*CADCnULTBR[table_no])["CADC00LLMTB"] > (unsigned short)(*CADCnULTBR[table_no])["CADC00ULMTB"]))) {
            mCADC_UNIT_AgentController->print_msg("warning","CADCnULTBR%d.CADC00LLMTB is greater than CADCnULTBR%d.CADC00ULMTB.\n", table_no, table_no);
            convert_enable = false;
        }
    }
    
    return convert_enable;
}//}}}

/// CADCnTDLVR.ANmkLV value
unsigned int CADC_UNIT_Func::GetPinDiagBitValue (const unsigned int an_no)
{//{{{
    return ((*CADCnTDLVR) >> an_no) & 0x1;
}//}}}

/// Get DFE info
bool CADC_UNIT_Func::IsDFEEntry (unsigned int &tag, unsigned int &data)
{//{{{
    unsigned ret = false;
    unsigned int vc_no = (*CADCnVCPTRR)["CADC00VCPTR"];
    if ((bool)(*CADCnVCR[vc_no])["CADC00DFENT"]) {
        ret = true;
        tag = (*CADCnVCR[vc_no])["CADC00DFTAG"];
        data = mDrSignMask;
    }
    return ret;
}//}}}

/// Get output data averaging
unsigned int CADC_UNIT_Func::GetAver ()
{//{{{
    return (*CADCnSMPCR)["CADC00ODAV"] & 0x1;
}//}}}

/// Update Calibration result
void CADC_UNIT_Func::UpdateCalibResult (void)
{//{{{
    mCalibFinish = true;
    mCalibValue = mCADC_UNIT_AgentController->mCADC->mtCoeff;
}//}}}

/// Initialize variables
void CADC_UNIT_Func::Initialize (void)
{//{{{
    mDrSignMask = 0;
    mIsCADCEInterrupt = false;
    mCalibFinish = false;
    mCalibValue = 1;
    mIsDIRSetting = 0;
    mIsUDIRSetting = 0;
    mIsDIRClear = 0;
    mIsUDIRClear = 0;
}//}}}

/// Update DIR and UDR register
void CADC_UNIT_Func::UpdateDIR (unsigned int vc_no)
{//{{{
    if (mIsDIRSetting == true) {
        mIsCADCEInterrupt = false;

        // Check CADC00OWE error
        if ((*CADCnDIR[vc_no])["CADC00WFLG"] == 1) {
            if ((*CADCnER)["CADC00OWE"] == 0) {
                (*CADCnER)["CADC00OWE"] = 1;
                (*CADCnER)["CADC00OWECAP"] = vc_no;
                if ((bool)(*CADCnSFTCR)["CADC00OWEIE"]) {
                    mIsCADCEInterrupt = true;
                    mCADC_UNIT_AgentController->print_msg("warning","CADC00OWE.\n");
                }
            }
        }

        // Check IDE error
        unsigned int idef = 1;
        if (mCADC_UNIT_AgentController->mCADC->mIDError[mCADC_UNIT_AgentController->mAdId]) {
            idef = 0;
                if ((bool)(*CADCnSFTCR)["CADC00IDEIE"]) {
                    mIsCADCEInterrupt = true;
                }
        }
        /// DIR register
            (*CADCnDIR[vc_no])["CADC00IDEF"] = idef;
            (*CADCnDIR[vc_no])["CADC00WFLG"] = 1;
            // DIR.DR
            unsigned int dr_unmask = 0;
            double result = CalculateConvertValue(vc_no);
            unsigned int result_sign = 0;
            if (result > ((double)emSignMax/(double)emSignFmtConst)) {
                result_sign = (unsigned int)emSignMax;
            } else if (result <= (double)emMinValue) {
                result_sign = (unsigned int)emSignMin;
            } else {
                result_sign = (unsigned int)((int)(result * emSignFmtConst) & 0xFFFF);
            }
            bool is_sign = false;
            if ((mCADC_UNIT_AgentController->mCADC->mCADC_Func->IsUnsignConfig()) && ((*CADCnVCR[vc_no])["CADC00CNVCLS"] == 0)) {
                // Unsigned fixed-point format
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
                // Signed fixed-point format
                is_sign = true;
                dr_unmask = result_sign;
            }
            unsigned int dr_mask = dr_unmask & (~(unsigned int)((1 << (*CADCnUCR)["CADC00DFMT"]) - 1));
            (*CADCnDIR[vc_no])["CADC00DR"] = dr_mask;
            // DIR.CADC00PRTY
            unsigned int prty = (unsigned int)((*CADCnDIR[vc_no])["CADC00IDEF"] & 0x1);
            unsigned int udptr = (unsigned int)((*CADCnUDPTRR)["CADC00UDPTR"] & 0x7);
            prty ^= udptr;
            for (unsigned int loop = 0; loop < emDRbit; loop++) {
                prty ^= (dr_mask >> loop) & 0x1;
            }
            if (mCADC_UNIT_AgentController->mCADC->mParityError[mCADC_UNIT_AgentController->mAdId]) {
                prty = ~prty;
            }
            prty &= 0x1;
            (*CADCnDIR[vc_no])["CADC00PRTY"] = prty;

            // For DFE
            mDrSignMask = result_sign & (~(unsigned int)((1 << (*CADCnUCR)["CADC00DFMT"]) - 1));

            /// UDIR register
            (*CADCnUDIR)["CADC00CHNUM"] = vc_no;
            (*CADCnUDIR)["CADC00UIDEF"] = idef;
            (*CADCnUDIR)["CADC00UWFLG"] = 1;
            (*CADCnUDIR)["CADC00UPRTY"] = prty;
            (*CADCnUDIR)["CADC00UDR"] = dr_mask;
            /// UDPTRR register
            (*CADCnUDPTRR)["CADC00UDPTR"] = vc_no;

            // Check CADC00ULE/CADC00LLE errors & Boundary Flag Generating Signal
            mCADC_UNIT_AgentController->NotifyEvent(CADC_UNIT_AgentController::emAssertCADCVCI, vc_no);
            unsigned int table_no = (*CADCnVCR[vc_no])["CADC00VCULLMTBS"];
            if ((bool)(*CADCnVCR[vc_no])["CADC00VCULME"]) {
                if (((is_sign) && ((short)dr_unmask > (short)(*CADCnULTBR[table_no])["CADC00ULMTB"])) ||
                ((!is_sign) && ((unsigned short)dr_unmask > (unsigned short)(*CADCnULTBR[table_no])["CADC00ULMTB"]))) {
                    mCADC_UNIT_AgentController->NotifyEvent(CADC_UNIT_AgentController::emAssertCADUE, 0);
                    if (((*CADCnER)["CADC00ULE"] == 0) && ((*CADCnER)["CADC00LLE"] == 0)) {
                        (*CADCnER)["CADC00ULE"] = 1;
                        (*CADCnER)["CADC00ULECAP"] = vc_no;
                        if ((bool)(*CADCnVCR[vc_no])["CADC00ULEIE"]) {
                            mIsCADCEInterrupt = true;
                            mCADC_UNIT_AgentController->print_msg("warning","ULE.\n");
                        }
                    }
                }
            }
            if ((bool)(*CADCnVCR[vc_no])["CADC00VCLLME"]) {
                if (((is_sign) && ((short)dr_unmask < (short)(*CADCnULTBR[table_no])["CADC00LLMTB"])) || ((!is_sign) && ((unsigned short)dr_unmask < (unsigned short)(*CADCnULTBR[table_no])["CADC00LLMTB"]))) {
                    mCADC_UNIT_AgentController->NotifyEvent(CADC_UNIT_AgentController::emAssertCADLE, 0);
                    if (((*CADCnER)["CADC00LLE"] == 0) && ((*CADCnER)["CADC00ULE"] == 0)) {
                        (*CADCnER)["CADC00LLE"] = 1;
                        (*CADCnER)["CADC00ULECAP"] = vc_no;
                        if ((bool)(*CADCnVCR[vc_no])["CADC00ULEIE"]) {
                            mIsCADCEInterrupt = true;
                            mCADC_UNIT_AgentController->print_msg("warning","LLE.\n");
                        }
                    }
                }
            }
        if (mIsDIRSetting) mIsDIRSetting = false;
        if (mIsUDIRSetting) mIsUDIRSetting = false;
        if(mIsDIRClear) mIsDIRClear = false;
        if(mIsUDIRClear) mIsUDIRClear = false;
        mCADC_UNIT_AgentController->NotifyEvent(CADC_UNIT_AgentController::emCheckFinish, 0);
    }
    else if ((mIsDIRSetting == false)&&(mIsDIRClear == true)) {
            if ((bool)(*CADCnSFTCR)["CADC00RDCLRE"]) {
                (*CADCnDIR[vc_no]) = 0;
                if ((*CADCnUDIR)["CADC00CHNUM"] == vc_no) {
                    (*CADCnUDIR) &= 0xE0000000;
                    }
                }
            (*CADCnDIR[vc_no])["CADC00WFLG"] = 0;
            if ((*CADCnUDIR)["CADC00CHNUM"] == vc_no) {
                (*CADCnUDIR)["CADC00UWFLG"] = 0;
                }
            if (mIsUDIRClear)mIsUDIRClear = false;
            CheckParityError(vc_no);
    }
    else if ((mIsUDIRSetting == false)&&(mIsUDIRClear == true)) {
            if ((bool)(*CADCnSFTCR)["CADC00RDCLRE"]) {
                (*CADCnUDIR) &= 0xE0000000;
                (*CADCnDIR[vc_no]) = 0;
            }
            (*CADCnUDIR)["CADC00UWFLG"] = 0;
            (*CADCnDIR[vc_no])["CADC00WFLG"] = 0;
            if (mIsUDIRClear) mIsUDIRClear = false;
            CheckParityError(vc_no);
    }
}//}}}


/// Called after CADCnDIRj is read
void CADC_UNIT_Func::Callback_DIR (const unsigned int vc_no)
{//{{{
    // notify here
    mIsDIRClear = true;
    mCADC_UNIT_AgentController->NotifyEvent(CADC_UNIT_AgentController::emUpdateDIR, vc_no);
}//}}}

/// Called after CADCnUDIR is read
void CADC_UNIT_Func::Callback_UDIR (void)
{//{{{
    unsigned int vc_no = (*CADCnUDIR)["CADC00CHNUM"];
    mIsUDIRClear = true;
    mCADC_UNIT_AgentController->NotifyEvent(CADC_UNIT_AgentController::emUpdateDIR, vc_no);
    
}//}}}

/// Called after CADCnSMPCR is read
void CADC_UNIT_Func::Callback_SMPCR (void)
{//{{{
    if ((bool)(*CADCnSMPCR)["CADC00ODAV"]) {
        (*CADCnSMPCR) = 0x00010707;
    } else{
        (*CADCnSMPCR) = 0x00000707;
    }    
}//}}}

/// Calculate AD conversion result
double CADC_UNIT_Func::CalculateConvertValue (const unsigned int vc_no)
{//{{{
    // Get analog value
    unsigned int cnvcls = (*CADCnVCR[vc_no])["CADC00CNVCLS"];
    unsigned int gctrl = (*CADCnVCR[vc_no])["CADC00GCTRL"];
    unsigned int an_p_no = 0;
    unsigned int an_n_no = 0;
    double an_p_val = 0;
    double an_n_val = 0;
    // Get analog value
    switch (cnvcls) {
        case emSingleVrefL :
        case emSingleVrefL_2 :
            an_p_no = gctrl;
            an_p_val = mCADC_UNIT_AgentController->GetAnalogValue(false, an_p_no);
            break;
        case emDifferential :
            an_p_no = gctrl & 0x6;
            an_p_val = mCADC_UNIT_AgentController->GetAnalogValue(false, an_p_no);
            an_n_no = an_p_no + 1;
            an_n_val = mCADC_UNIT_AgentController->GetAnalogValue(false, an_n_no);
            break;
        default:    // emSelfDiag
                break;
    }
    
    // Calculate conversion value
    double refh_val = mCADC_UNIT_AgentController->GetAnalogValue(true, 0);
    double result = 0;
    unsigned int odde_ode = mCADC_UNIT_AgentController->mCADC->mCADC_Func->CheckDisconnectDetectSetup();
    if (((odde_ode & 0x1) == 1) && ((((odde_ode >> 1) & 0x1) == 1) || 
     (mCADC_UNIT_AgentController->mCADC->mDisconnectError[mCADC_UNIT_AgentController->mAdId]))) {
        if (cnvcls == 0) {
            // Disconnect is detected
            result = 0;
        }
    } else {
        switch (cnvcls) {
            case emSingleVrefL :
                result = (an_p_val / refh_val);
                break;
            case emSingleVrefL_2 :
                result = (an_p_val / refh_val - 0.5);
                break;
            case emDifferential :
                result = ((an_p_val - an_n_val) / refh_val);
                break;
            default:  // emSelfDiag
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
                    default :   // 0xC 
                        result = -0.5;
                        break;
                }
                break;
        }
    }
    
    if(mCalibFinish) {
        // Apply Calibration
        result *= mCalibValue;
    }
    return result;
}//}}}

/// Check Parity error
void CADC_UNIT_Func::CheckParityError (const unsigned int vc_no)
{//{{{
    if (mCADC_UNIT_AgentController->mCADC->mParityError[mCADC_UNIT_AgentController->mAdId]) {
        // Parity error occur
        if ((*CADCnER)["CADC00PE"] == 0) {
            (*CADCnER)["CADC00PE"] = 1;
            (*CADCnER)["CADC00PECAP"] = vc_no;
            if ((bool)(*CADCnSFTCR)["CADC00PEIE"]) { //TBD
                mCADC_UNIT_AgentController->NotifyEvent(CADC_UNIT_AgentController::emAssertCADPE, 0);
            }
        }
    }
}//}}}

// vim600: set foldmethod=marker :
