// ---------------------------------------------------------------------
// $Id: DFE_FIFO_Func.cpp,v 1.2 2018/09/06 10:17:09 huepham Exp $
//
// Copyright(c) 2018 Renesas Electronics Corporation
// Copyright(c) 2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "DFE_FIFO_Func.h"

#include <string>
#include <sstream>
using std::ostringstream;
#include <map>
using std::cout;
using std::cerr;
using std::endl;
//------------------------------------------------------------------------------
/**@brief public constructor
 * Detailed steps as follows:
 */
DFE_FIFO_Func::DFE_FIFO_Func(string     iname
                            ,DFE_FIFO*  pWrapper)
: Cdfe_fifo_regif(iname, 32)
// internal attributes
, pWrapper    (pWrapper)
, mModuleName (iname)
{
    /// -# connect to the wrapper
    sc_assert(pWrapper != NULL);
    /// @todo: ask Binh-san for the command interface
    // pWrapper->CommandInit(pWrapper->basename());

    /// -# initialize the *register block* base class
    Cdfe_fifo_regif::set_instance_name(string(pWrapper->basename()));

    /// -# initialize internal status attributes
    Initialize ();
}
//------------------------------------------------------------------------------
/**@brief public destructor
 */
DFE_FIFO_Func::~DFE_FIFO_Func() 
{
}
//------------------------------------------------------------------------------
/**@brief handle command passed into the register block via Python interface
 *
 * @details
 *
 * @note this function is kept unchanged from other models
 */
void DFE_FIFO_Func::RegisterHandler (const std::vector<std::string>& args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {
        //Dump registers name
        vpcl::re_register* reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    } else {
        if (args[1] == "*") {
            //Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register* reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase (cmd.begin()+1);
                cmd.insert(cmd.begin()+1, reg_p->name());
                msg_return = Cdfe_fifo_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Ctsn_regif class
            msg_return = Cdfe_fifo_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief read access implementation BusSlaveFuncIf::read via BusSlaveFuncIf
 */
void DFE_FIFO_Func::read (uint32_t         offsetAddress ///< [in]    offset address
                         ,TlmBasicPayload& trans         ///< [inout] transaction
                         ,BusTime_t*       t             ///< [inout] time
                         ,bool             debug         ///< [in]    debug access
                         )
{//{{{

    std::memset(trans.get_data_ptr()
               ,0
               ,trans.get_data_length()
               );
    if (!debug) {
        reg_rd((uint32_t)trans.get_address()
              ,          trans.get_data_ptr()
              ,          trans.get_data_length()
              );
    } else {
        reg_rd_dbg((uint32_t)trans.get_address()
                  ,          trans.get_data_ptr()
                  ,          trans.get_data_length()
                  );
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief write access implementation of BusSlaveFuncIf::write via BusSlaveFuncIf
 */
void DFE_FIFO_Func::write(uint32_t          offsetAddress ///< [in] offset address
                         ,TlmBasicPayload&  trans         ///< [inout] transaction
                         ,BusTime_t*        t             ///< [inout] time
                         ,bool              debug         ///< [in] debug access
                         )
{//{{{
    if (!debug) {
        reg_wr((uint32_t)trans.get_address()
              ,          trans.get_data_ptr()
              ,          trans.get_data_length()
              );
    } else {
        reg_wr_dbg((uint32_t)trans.get_address()
                  ,          trans.get_data_ptr()
                  ,          trans.get_data_length()
                  );
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief initialize internal variables
 */
void DFE_FIFO_Func::Initialize () 
{//{{{
}//}}}
void DFE_FIFO_Func::CancelEvent() 
{//{{{
}//}}}
void DFE_FIFO_Func::EnableReset(const bool& isActive) 
{//{{{
    Cdfe_fifo_regif::EnableReset(isActive);
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *Buffer A enable* field of
 *        *Buffer A Common control register*
 */
void DFE_FIFO_Func::cb_DFBFACCTL_AEN   (RegCBstr str) 
{//{{{
    if (str.is_wr) {
        uint8_t AEN_pre = bit_select(str.pre_data
                                    ,(*DFBFACCTL)["AEN"].mStartAddr
                                    ,(*DFBFACCTL)["AEN"].mEndAddr);
        uint8_t AEN_cur = bit_select(str.data
                                    ,(*DFBFACCTL)["AEN"].mStartAddr
                                    ,(*DFBFACCTL)["AEN"].mEndAddr);
        if (AEN_pre == 1 && AEN_cur == 0)
            /// Buffer A is changed from being enable to being disable
            for (uint8_t fci = 0; fci < pWrapper->DFBA_CHN_NUM; ++fci)
                pWrapper->init_FFA(str.channel);
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *Buffer A Interrupt overflow enable* field of
 *        *Buffer A Common control register*
 */
void DFE_FIFO_Func::cb_DFBFACCTL_AIEO  (RegCBstr str) 
{//{{{
    if (str.is_wr) {
        uint8_t AEN_pre  = bit_select(str.pre_data
                                     ,(*DFBFACCTL)["AEN"].mStartAddr
                                     ,(*DFBFACCTL)["AEN"].mEndAddr);
        uint8_t AEN_cur  = bit_select(str.data
                                     ,(*DFBFACCTL)["AEN"].mStartAddr
                                     ,(*DFBFACCTL)["AEN"].mEndAddr);
        uint8_t AIEO_pre = bit_select(str.pre_data
                                     ,(*DFBFACCTL)["AIEO"].mStartAddr
                                     ,(*DFBFACCTL)["AIEO"].mEndAddr);
        uint8_t AIEO_cur = bit_select(str.data
                                     ,(*DFBFACCTL)["AIEO"].mStartAddr
                                     ,(*DFBFACCTL)["AIEO"].mEndAddr);

        if (AEN_pre == 1 && AEN_cur == 1 && AIEO_pre != AIEO_cur)
            /// Buffer A has been enable before, disable writing new data
            (*DFBFACCTL)["AIEO"] = AIEO_pre;

    }
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *Buffer A Interrupt overflow enable* field of
 *        *Buffer A Common control register*
 */
void DFE_FIFO_Func::cb_DFBFACCTL_AIEE  (RegCBstr str) 
{//{{{
    if (str.is_wr) {
        uint8_t AEN_pre  = bit_select(str.pre_data
                                     ,(*DFBFACCTL)["AEN"].mStartAddr
                                     ,(*DFBFACCTL)["AEN"].mEndAddr);
        uint8_t AEN_cur  = bit_select(str.data
                                     ,(*DFBFACCTL)["AEN"].mStartAddr
                                     ,(*DFBFACCTL)["AEN"].mEndAddr);
        uint8_t AIEE_pre = bit_select(str.pre_data
                                     ,(*DFBFACCTL)["AIEE"].mStartAddr
                                     ,(*DFBFACCTL)["AIEE"].mEndAddr);
        uint8_t AIEE_cur = bit_select(str.data
                                     ,(*DFBFACCTL)["AIEE"].mStartAddr
                                     ,(*DFBFACCTL)["AIEE"].mEndAddr);

        if (AEN_pre == 1 && AEN_cur == 1 && AIEE_pre != AIEE_cur)
            /// Buffer A has been enable before, disable writing new data
            (*DFBFACCTL)["AIEE"] = AIEE_pre;
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *Buffer A Interrupt overflow enable* field of
 *        *Buffer A Common control register*
 */
void DFE_FIFO_Func::cb_DFBFACCTL_AUNE  (RegCBstr str) 
{//{{{
    if (str.is_wr) {
        uint8_t AEN_pre  = bit_select(str.pre_data
                                     ,(*DFBFACCTL)["AEN"].mStartAddr
                                     ,(*DFBFACCTL)["AEN"].mEndAddr);
        uint8_t AEN_cur  = bit_select(str.data
                                     ,(*DFBFACCTL)["AEN"].mStartAddr
                                     ,(*DFBFACCTL)["AEN"].mEndAddr);
        uint8_t AUNE_pre = bit_select(str.pre_data
                                     ,(*DFBFACCTL)["AUNE"].mStartAddr
                                     ,(*DFBFACCTL)["AUNE"].mEndAddr);
        uint8_t AUNE_cur = bit_select(str.data
                                     ,(*DFBFACCTL)["AUNE"].mStartAddr
                                     ,(*DFBFACCTL)["AUNE"].mEndAddr);

        if (AEN_pre == 1 && AEN_cur == 1 && AUNE_pre != AUNE_cur)
            /// Buffer A has been enable before, disable writing new data
            (*DFBFACCTL)["AUNE"] = AUNE_pre;

        for (uint8_t fci = 0; fci < 6; ++fci)
            pWrapper->calc_ba_di(fci);
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *FIFO channel of Buffer A enable* field of
 *        *Buffer A control register for a channel*
 */
void DFE_FIFO_Func::cb_DFBFACTLCH_CHEN (RegCBstr str) 
{//{{{
    if (str.is_wr) {
        uint8_t CHEN_pre = bit_select(str.pre_data
                                     ,(*DFBFACTLCH[str.channel])["CHEN"].mStartAddr
                                     ,(*DFBFACTLCH[str.channel])["CHEN"].mEndAddr);
        uint8_t CHEN_cur = bit_select(str.data
                                     ,(*DFBFACTLCH[str.channel])["CHEN"].mStartAddr
                                     ,(*DFBFACTLCH[str.channel])["CHEN"].mEndAddr);

        /// - prevent *CHEN* field from being changed when *AEN* is 1
        if (1 == (*DFBFACCTL)["AEN"]) {
            (*DFBFACTLCH[str.channel])["CHEN"] = CHEN_pre;
            return;
        }

        /// -# Initialize the FIFO of Buffer A when *CHEN* is changed from being enable to being disable
        if (CHEN_pre == 1 && CHEN_cur == 0)
            pWrapper->init_FFA(str.channel);
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *FIFO channel of Buffer A DFE macro selection* field of
 *        *Buffer A control register for a channel*
 */
void DFE_FIFO_Func::cb_DFBFACTLCH_ADSL (RegCBstr str) 
{//{{{
    if (str.is_wr) {
        uint8_t ADSL_pre = bit_select(str.pre_data
                                     ,(*DFBFACTLCH[str.channel])["ADSL"].mStartAddr
                                     ,(*DFBFACTLCH[str.channel])["ADSL"].mEndAddr);
        uint8_t ADSL_cur = bit_select(str.data
                                     ,(*DFBFACTLCH[str.channel])["ADSL"].mStartAddr
                                     ,(*DFBFACTLCH[str.channel])["ADSL"].mEndAddr);

        /// - prevent *ADSL* field from being changed when *AEN* is 1
        if (1 == (*DFBFACCTL)["AEN"]) {
            (*DFBFACTLCH[str.channel])["ADSL"] = ADSL_pre;
            return;
        }

        validate_configuration();

        /// -# update value of *ba_di* signal
        if (ADSL_pre != ADSL_cur)
            pWrapper->calc_ba_di(str.channel);
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *FIFO channel of Buffer A DFE channel selection* field of
 *        *Buffer A control register for a channel*
 */
void DFE_FIFO_Func::cb_DFBFACTLCH_BFACH(RegCBstr str) 
{//{{{
    if (str.is_wr) {
        uint8_t BFACH_pre = bit_select(str.pre_data
                                      ,(*DFBFACTLCH[str.channel])["BFACH"].mStartAddr
                                      ,(*DFBFACTLCH[str.channel])["BFACH"].mEndAddr);
        uint8_t BFACH_cur = bit_select(str.data
                                      ,(*DFBFACTLCH[str.channel])["BFACH"].mStartAddr
                                      ,(*DFBFACTLCH[str.channel])["BFACH"].mEndAddr);

        /// - prevent *BFACH* field from being changed when *AEN* is 1
        if (1 == (*DFBFACCTL)["AEN"]) {
            (*DFBFACTLCH[str.channel])["BFACH"] = BFACH_pre;
            return;
        }

        validate_configuration();

        /// -# update value of *ba_di* signal
        if (BFACH_pre != BFACH_cur)
            pWrapper->calc_ba_di(str.channel);
    }
}//}}}
//------------------------------------------------------------------------------
bool DFE_FIFO_Func::validate_configuration() 
{//{{{
    bool isPass = true;

    ostringstream oss;

    using std::pair;
    using std::map;
    typedef pair<uint8_t, uint8_t> Key_t;      ///< pair of a DFE macro index and a DFE channel index
    typedef pair<uint8_t, uint8_t> Val_t;      ///< pair of a FIFO circuit index and FIFO channel index
    typedef map<Key_t, Val_t>      ChanelMap_t;///< Assign map between a DFE channel of a DFE macro and a FIFO channel of a FIFO circuit
    ChanelMap_t channelMap;

    /// -# add Buffer B channel map first if it is already enable
    if (1 == (*DFBFBCTL)["BEN"]) {
        uint8_t dmi_b = (*DFBFBCTL)["BDSL" ];
        uint8_t dci_b = (*DFBFBCTL)["BFBCH"];
        channelMap[Key_t(dmi_b, dci_b)] = Val_t(1, 0);
    }

    for (uint8_t fci = 0; fci < (pWrapper->DFBA_CHN_NUM-1); ++fci) {
        uint8_t dmi_a = (*DFBFACTLCH[fci])["ADSL" ];
        uint8_t dci_a = (*DFBFACTLCH[fci])["BFACH"];
        if ((dmi_a >= pWrapper->DFE_MACRO_NUMB) || (dci_a >= pWrapper->DFE_CHN_NUMB[dmi_a])) {
            oss.clear(); oss.str("");
            oss << std::dec << std::fixed << std::setprecision(0) << "[" << DFE_FIFO_Utils::Time::toDouble(sc_time_stamp(), SC_PS) << " ps]";
            oss  << "DFE_FIFO Buffer A circuit channel "
                 << "[" << (uint32_t)fci << "]: "
                 << "DFE channel exceeds the limitation. "
                 << "Selected DFE macro: ["   << uint32_t(dmi_a) << "]. "
                 << "Selected DFE channel: [" << uint32_t(dci_a) << "]. "
                 << endl;
            cout << oss.str();
            isPass = false;
        }

        uint8_t CHEN    = (*DFBFACTLCH[fci])  ["CHEN"];
        uint8_t CHEN_nx = (*DFBFACTLCH[fci+1])["CHEN"];
        if (CHEN == 0 && CHEN_nx == 1) {
            oss.clear(); oss.str("");
            oss << std::dec << std::fixed << std::setprecision(0) << "[" << DFE_FIFO_Utils::Time::toDouble(sc_time_stamp(), SC_PS) << " ps]";
            oss  << "DFE_FIFO Buffer A circuit channel "
                 << "[" << (uint32_t)fci << "]: "
                 << "channels must be enable in a consecutive sequence start from 0"
                 << endl;
            cout << oss.str();
            isPass = false;
        }

        if (CHEN) {
            ChanelMap_t::iterator searchResult = channelMap.find(Key_t(dmi_a, dci_a));
            if (searchResult != channelMap.end()) {
                uint8_t fmi_ex = searchResult->second.first;
                uint8_t fci_ex = searchResult->second.second;
                oss.clear(); oss.str("");
                oss << std::dec << std::fixed << std::setprecision(0) << "[" << DFE_FIFO_Utils::Time::toDouble(sc_time_stamp(), SC_PS) << " ps]";
                oss  << "DFE_FIFO Buffer A circuit channel "
                     << "[" << (uint32_t)fci << "]: "
                     << "DFE macro [" << uint32_t(dmi_a) << "] "
                     << "channel ["   << uint32_t(dci_a) << "] "
                     << "has already been assigned to buffer circuit "
                     << "[" << (fmi_ex == 0 ? "A" : "B") << "] "
                     << "channel [" << uint32_t(fci_ex) << "]"
                     << endl;
                cout << oss.str();
                isPass = false;
            } else {
                channelMap[Key_t(dmi_a,dci_a)] = Val_t(0, fci);
            }
        }
    }

    do {
        uint8_t dmi_b = (*DFBFBCTL)["BDSL" ];
        uint8_t dci_b = (*DFBFBCTL)["BFBCH"];

        if ((dmi_b >= pWrapper->DFE_MACRO_NUMB) || (dci_b >= pWrapper->DFE_CHN_NUMB[dmi_b])) {
            oss.clear(); oss.str("");
            oss << std::dec << std::fixed << std::setprecision(0) << "[" << DFE_FIFO_Utils::Time::toDouble(sc_time_stamp(), SC_PS) << " ps]";
            oss  << "DFE_FIFO Buffer B circuit:"
                 << "DFE channel exceeds the limitation. "
                 << "Selected DFE macro: ["   << uint32_t(dmi_b) << "]. "
                 << "Selected DFE channel: [" << uint32_t(dci_b) << "]. "
                 << endl;
            cout << oss.str();
            isPass = false;
        }

        ChanelMap_t::iterator searchResult = channelMap.find(Key_t(dmi_b, dci_b));
        if (searchResult != channelMap.end()) {
            uint8_t fmi_ex = searchResult->second.first;
            uint8_t fci_ex = searchResult->second.second;
            if (fmi_ex != 1) {
                oss.clear(); oss.str("");
                oss << std::dec << std::fixed << std::setprecision(0) << "[" << DFE_FIFO_Utils::Time::toDouble(sc_time_stamp(), SC_PS) << " ps]";
                oss  << "DFE_FIFO Buffer B circuit: "
                     << "DFE macro [" << uint32_t(dmi_b) << "] "
                     << "channel ["   << uint32_t(dci_b) << "] "
                     << "has already been assigned to buffer circuit "
                     << "[" << (fmi_ex == 0 ? "A" : "B") << "] "
                     << "channel [" << uint32_t(fci_ex) << "]"
                     << endl;
                cout << oss.str();
                isPass = false;
            }
        }
    } while (0);

    return (isPass);
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a call-back of read access into *Buffer A DFE channel output data*
 */
void DFE_FIFO_Func::cb_DFBFADOCH_ADO   (RegCBstr str) 
{//{{{
    if (!str.is_wr)
        if (!pWrapper->pFFAs[str.channel].empty()) {
            (*DFBFADOCH[str.channel])["ADO"] = pWrapper->pFFAs[str.channel].front();
            pWrapper->pFFAs[str.channel].pop();
        }
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *Buffer A clear*
 */
void DFE_FIFO_Func::cb_DFBFACLR_CLRA   (RegCBstr str) 
{//{{{
    if (str.is_wr) {
        uint8_t CLRA_cur = bit_select(str.data
                                    ,(*DFBFACLR)["CLRA"].mStartAddr
                                    ,(*DFBFACLR)["CLRA"].mEndAddr);
        if (1 == CLRA_cur) {
            for (uint8_t fci = 0; fci < pWrapper->DFBA_CHN_NUM; ++fci)
                pWrapper->init_FFA(fci);
            
            //Get current EMPAn, OVFAn
            uint8_t EMPA_val =  ((uint32_t)(this->GetDFBFACST()) >> 16) & 0x3F;
            uint8_t OVFA_val =  (uint32_t)(this->GetDFBFACST()) & 0x3F;
            //Clear EMPAn bits to 0x3F
            if (EMPA_val != 0x3F) {
                (*DFBFACST)["EMPA"] = 0x3F;
            }
            //Clear OVFA bits to 0
            if (OVFA_val != 0) {
                (*DFBFACST)["OVFA"] = 0x0;
            }
            for (uint8_t fci = 0; fci < pWrapper->DFBA_CHN_NUM; ++fci){
                //Clear ADO bit of DFBFADOCH register
                (*DFBFADOCH[fci])["ADO"] = (1 == (*DFBFACCTL)["AUNE"])
                                                 ? 1
                                                 : 0;
            }
        }
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a call-back of read access into *FIFO channel of Buffer A overflow*
 *        field on register *Buffer A circuit common status register*
 */
void DFE_FIFO_Func::cb_DFBFACST_OVFA   (RegCBstr str) 
{//{{{
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a call-back of read access into *FIFO channel of Buffer A empty*
 *        field on register *Buffer A circuit common status register*
 */
void DFE_FIFO_Func::cb_DFBFACST_EMPA   (RegCBstr str) 
{//{{{
    if (!str.is_wr) {
        uint8_t EMPA = 0;
        for (uint8_t fci = 0; fci < pWrapper->DFBA_CHN_NUM; ++fci)
            EMPA |= (pWrapper->pFFAs[fci].empty() << fci);
        (*DFBFACST)["EMPA"] = EMPA;
    }
}//}}}
//------------------------------------------------------------------------------
void DFE_FIFO_Func::cb_DFBFBCTL_BEN    (RegCBstr str) {
    if (str.is_wr) {
        uint8_t BEN_pre = bit_select(str.pre_data
                                    ,(*DFBFBCTL)["BEN"].mStartAddr
                                    ,(*DFBFBCTL)["BEN"].mEndAddr);
        uint8_t BEN_cur = bit_select(str.data
                                    ,(*DFBFBCTL)["BEN"].mStartAddr
                                    ,(*DFBFBCTL)["BEN"].mEndAddr);
        /// -# initialize Buffer B when being changed from enable to disable
        if (BEN_pre == 1 && BEN_cur == 0)
            pWrapper->init_FFB();
    }
}//}}}
//------------------------------------------------------------------------------
void DFE_FIFO_Func::cb_DFBFBCTL_BIEO   (RegCBstr str) 
{//{{{
    if (str.is_wr) {
        uint8_t BEN_pre = bit_select(str.pre_data
                                    ,(*DFBFBCTL)["BEN"].mStartAddr
                                    ,(*DFBFBCTL)["BEN"].mEndAddr);
        uint8_t BEN_cur = bit_select(str.data
                                    ,(*DFBFBCTL)["BEN"].mStartAddr
                                    ,(*DFBFBCTL)["BEN"].mEndAddr);
        uint8_t BIEO_pre = bit_select(str.pre_data
                                     ,(*DFBFBCTL)["BIEO"].mStartAddr
                                     ,(*DFBFBCTL)["BIEO"].mEndAddr);
        uint8_t BIEO_cur = bit_select(str.data
                                     ,(*DFBFBCTL)["BIEO"].mStartAddr
                                     ,(*DFBFBCTL)["BIEO"].mEndAddr);

        if (BEN_pre == 1 && BEN_cur == 1 && BIEO_pre != BIEO_cur)
            /// Buffer B has been enable before, disable writing new data
            (*DFBFBCTL)["BIEO"] = BIEO_pre;
    }
}//}}}
//------------------------------------------------------------------------------
void DFE_FIFO_Func::cb_DFBFBCTL_BIEE   (RegCBstr str) 
{//{{{
    if (str.is_wr) {
        uint8_t BEN_pre = bit_select(str.pre_data
                                    ,(*DFBFBCTL)["BEN"].mStartAddr
                                    ,(*DFBFBCTL)["BEN"].mEndAddr);
        uint8_t BEN_cur = bit_select(str.data
                                    ,(*DFBFBCTL)["BEN"].mStartAddr
                                    ,(*DFBFBCTL)["BEN"].mEndAddr);
        uint8_t BIEE_pre = bit_select(str.pre_data
                                     ,(*DFBFBCTL)["BIEE"].mStartAddr
                                     ,(*DFBFBCTL)["BIEE"].mEndAddr);
        uint8_t BIEE_cur = bit_select(str.data
                                     ,(*DFBFBCTL)["BIEE"].mStartAddr
                                     ,(*DFBFBCTL)["BIEE"].mEndAddr);

        if (BEN_pre == 1 && BEN_cur == 1 && BIEE_pre != BIEE_cur)
            /// Buffer B has been enable before, disable writing new data
            (*DFBFBCTL)["BIEE"] = BIEE_pre;
    }
}//}}}
//------------------------------------------------------------------------------
void DFE_FIFO_Func::cb_DFBFBCTL_BDSL   (RegCBstr str) 
{//{{{
    if (str.is_wr) {
        uint8_t BEN_pre = bit_select(str.pre_data
                                    ,(*DFBFBCTL)["BEN"].mStartAddr
                                    ,(*DFBFBCTL)["BEN"].mEndAddr);
        uint8_t BEN_cur = bit_select(str.data
                                    ,(*DFBFBCTL)["BEN"].mStartAddr
                                    ,(*DFBFBCTL)["BEN"].mEndAddr);
        uint8_t BDSL_pre = bit_select(str.pre_data
                                     ,(*DFBFBCTL)["BDSL"].mStartAddr
                                     ,(*DFBFBCTL)["BDSL"].mEndAddr);
        uint8_t BDSL_cur = bit_select(str.data
                                     ,(*DFBFBCTL)["BDSL"].mStartAddr
                                     ,(*DFBFBCTL)["BDSL"].mEndAddr);

        /// -# Check if Buffer B has been enable before, disable writing new data
        if (BEN_pre == 1 && BEN_cur == 1 && BDSL_pre != BDSL_cur) {
            (*DFBFBCTL)["BDSL"] = BDSL_pre;
            return;
        }

        validate_configuration();

        if (BDSL_pre != BDSL_cur)
            pWrapper->calc_bb_di();
    }
}//}}}
//------------------------------------------------------------------------------
void DFE_FIFO_Func::cb_DFBFBCTL_BFBCH  (RegCBstr str) 
{//{{{
    if (str.is_wr) {
        uint8_t BEN_pre = bit_select(str.pre_data
                                    ,(*DFBFBCTL)["BEN"].mStartAddr
                                    ,(*DFBFBCTL)["BEN"].mEndAddr);
        uint8_t BEN_cur = bit_select(str.data
                                    ,(*DFBFBCTL)["BEN"].mStartAddr
                                    ,(*DFBFBCTL)["BEN"].mEndAddr);
        uint8_t BFBCH_pre = bit_select(str.pre_data
                                     ,(*DFBFBCTL)["BFBCH"].mStartAddr
                                     ,(*DFBFBCTL)["BFBCH"].mEndAddr);
        uint8_t BFBCH_cur = bit_select(str.data
                                     ,(*DFBFBCTL)["BFBCH"].mStartAddr
                                     ,(*DFBFBCTL)["BFBCH"].mEndAddr);

        /// -# Check if Buffer B has been enable before, disable writing new data
        if (BEN_pre == 1 && BEN_cur == 1 && BFBCH_pre != BFBCH_cur) {
            (*DFBFBCTL)["BFBCH"] = BFBCH_pre;
            return;
        }

        validate_configuration();

        if (BFBCH_pre != BFBCH_cur)
            pWrapper->calc_bb_di();
    }
}//}}}
//------------------------------------------------------------------------------
void DFE_FIFO_Func::cb_DFBFBDO_BDO     (RegCBstr str) 
{//{{{
    if (!str.is_wr)
        if (!pWrapper->FFB.empty()) {
            (*DFBFBDO)["BDO"] = pWrapper->FFB.front();
            pWrapper->FFB.pop();
            if (pWrapper->FFB.empty())
                pWrapper->bb_state = DFE_FIFO::FROM_EMPTY;
        }
}//}}}

//------------------------------------------------------------------------------
void DFE_FIFO_Func::cb_DFBFBCLR_CLRB   (RegCBstr str) 
{//{{{
    if (str.is_wr) {
        uint8_t CLRB_pre = bit_select(str.pre_data
                                     ,(*DFBFBCLR)["CLRB"].mStartAddr
                                     ,(*DFBFBCLR)["CLRB"].mEndAddr);
        uint8_t CLRB_cur = bit_select(str.data
                                     ,(*DFBFBCLR)["CLRB"].mStartAddr
                                     ,(*DFBFBCLR)["CLRB"].mEndAddr);
        uint8_t CLRB_reg = (*DFBFBCLR)["CLRB"];

        /// -# clear buffer B 
        if (CLRB_pre == 0 && CLRB_cur == 1) {
            pWrapper->init_FFB();
            //Get EMPB, OVFB bit value
            uint8_t EMPB_val = ((uint32_t)(this->GetDFBFBST()) >> 1) & 0x1;
            uint8_t OVFB_val = (uint32_t)(this->GetDFBFBST()) & 0x1;
            if (EMPB_val == 0) {
                (*DFBFBST)["EMPB"] = 1;
            }
            if (OVFB_val == 1) {
                (*DFBFBST)["OVFB"] = 0;
            }
            //Clear bit BDO to 0
            (*DFBFBDO)["BDO"] = 0;
        }   
    }
}//}}}

//------------------------------------------------------------------------------
void DFE_FIFO_Func::cb_DFBFBST_OVFB    (RegCBstr str) 
{//{{{
}//}}}
//------------------------------------------------------------------------------
void DFE_FIFO_Func::cb_DFBFBST_EMPB    (RegCBstr str) 
{//{{{
    if (!str.is_wr)
        (*DFBFBST)["EMPB"] = pWrapper->FFB.empty();
}//}}}

//------------------------------------------------------------------------------

unsigned int DFE_FIFO_Func::GetDFBFACST()
{//{{{
    return (unsigned int)(*DFBFACST);
}//}}}

unsigned int DFE_FIFO_Func::GetDFBFBST()
{//{{{
    return (unsigned int)(*DFBFBST);
}//}}}
// vim600: set foldmethod=marker :