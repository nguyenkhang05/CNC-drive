// ---------------------------------------------------------------------
// $Id: IBG_U2_Func.cpp,v 1.3 2020/02/26 06:51:33 duongphan Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "IBG_U2_Func.h"

#include <string>
#include <sstream>
// Added by Quang
#include "IBG_U2_CommonGuardAgentControllerIf.h"
//
using std::ostringstream;
#include <map>
using std::cout;
using std::cerr;
using std::endl;

//#ifndef func_re_printf
//#define func_re_printf mpIBG_U2_CommonGuardAgentControllerIf->get_fileline(__FILE__, __LINE__); mpIBG_U2_CommonGuardAgentControllerIf->_re_printf
//#endif//func_re_printf

//------------------------------------------------------------------------------
/**@brief public constructor
 * Detailed steps as follows:
 */
IBG_U2_Func::IBG_U2_Func(string iname, IBG_U2*  ipWrapper, uint8_t ichanNum, IBG_U2_CommonGuardAgentControllerIf *IBG_U2_CommonGuardAgentControllerIfPtr)
: Cibg_u2_regif(iname, 32, ichanNum)
// internal attributes
, KEY_CODE_PROT ((0xA5A5A500 >> 1))
, pWrapper    (ipWrapper)
, mModuleName (iname)
{//{{{
    /// -# connect to the wrapper
    //Added by Quang
    assert(IBG_U2_CommonGuardAgentControllerIfPtr != NULL);
    mIBG_U2_CommonGuardAgentControllerIf = IBG_U2_CommonGuardAgentControllerIfPtr;
    //mpIBG_U2_CommonGuardAgentControllerIf->CommandInit(name);
    //
    sc_assert(pWrapper != NULL);
    pWrapper->CommandInit(iname);

    /// -# initialize the *register block* base class
    Cibg_u2_regif::set_instance_name(iname);
}//}}}
//------------------------------------------------------------------------------
/**@brief public destructor
 */
IBG_U2_Func::~IBG_U2_Func() 
{//{{{
}//}}}
//------------------------------------------------------------------------------
/**@brief handle command passed into the register block via Python interface
 * @details
 * @note this function is kept unchanged from other models
 */
void IBG_U2_Func::RegisterHandler (const std::vector<std::string>& args)
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
                msg_return = Cibg_u2_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Ctsn_regif class
            msg_return = Cibg_u2_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief read access implementation BusSlaveFuncIf::read via BusSlaveFuncIf
 */
void IBG_U2_Func::read (uint32_t         offsetAddress ///< [in]    offset address
                    ,TlmBasicPayload& trans         ///< [inout] transaction
                    ,BusTime_t*       t             ///< [inout] time
                    ,bool             debug         ///< [in]    debug access
                    )
{//{{{
    // Added by Quang
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    //

    std::memset(trans.get_data_ptr()
                ,0
                ,trans.get_data_length()
                );
    //Added by Quang
        // When clock = 0, or reset state, only debug master can read registermpIBG_U2_CommonGuardAgentControllerIf
    if ( (!mIBG_U2_CommonGuardAgentControllerIf->CheckGuardClock("PCLK")) || (mIBG_U2_CommonGuardAgentControllerIf->GetGuardResetStatus()) ){
        if (pExtInfo != NULL){
            if (! (debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID)) ){
                re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
                return;
            }
        }else{
            re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
            return;
        }
    }
    //

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
void IBG_U2_Func::write(uint32_t          offsetAddress ///< [in] offset address
                    ,TlmBasicPayload&  trans         ///< [inout] transaction
                    ,BusTime_t*        t             ///< [inout] time
                    ,bool              debug         ///< [in] debug access
                    )
{//{{{
        // Added by Quang
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    //
    // if (! (pWrapper->CheckGuardClock("PCLK")) ) {
    //     re_printf("warning", "Cannot access register when PCLK clock is 0.\n");
    //     return;
    // }
    //Added by Quang
    // Not need to check reset status because regif class prevent write register in reset state
    // When clock = 0, only debug master write
    if ((!mIBG_U2_CommonGuardAgentControllerIf->CheckGuardClock("PCLK"))){
        if (pExtInfo != NULL){
            if (! (debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID)) ){
                re_printf("warning", "Cannot write register when clock is 0.\n");
                return;
            }
        }else{
            re_printf("warning", "Cannot write register when clock is 0.\n");
            return;
        }
    }   
    //
    if (!debug) {
        reg_wr((uint32_t)trans.get_address()
                ,trans.get_data_ptr()
                ,trans.get_data_length()
                );
    } else {
        reg_wr_dbg((uint32_t)trans.get_address()
                ,trans.get_data_ptr()
                ,trans.get_data_length()
                );
    }
}//}}}

//------------------------------------------------------------------------------
/**@brief API support reset from the outside
 */
void IBG_U2_Func::EnableReset(const bool& isActive) 
{//{{{
    Cibg_u2_regif::EnableReset(isActive);
}//}}}
//------------------------------------------------------------------------------
/**@brief API support PRESETn reset from the outside
 */
void IBG_U2_Func::EnablePRESETnReset(const bool& isActive) 
{//{{{
    Cibg_u2_regif::mIsReset = isActive;
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *clear overflow status* field
 *        of *guard error status clear* register
 */
void IBG_U2_Func::cb_GOVFCLR_CLRO(RegCBstr str) 
{//{{{
    if (str.is_wr) {
        uint8_t CLRO_cur = bit_select(str.data
                                    ,(*GOVFCLR)["CLRO"].mStartAddr
                                    ,(*GOVFCLR)["CLRO"].mEndAddr);
        if (1 == CLRO_cur)
            (*GOVFSTAT)["OVF"] = 0;
    }
}//}}}

void IBG_U2_Func::cb_GSPIDERRCLR_SPIDCLR(RegCBstr str) 
{//{{{
    if (str.is_wr) {
        uint32_t SPIDCLR_cur = bit_select(str.data
                                    ,(*GSPIDERRCLR)["SPIDCLR"].mStartAddr
                                    ,(*GSPIDERRCLR)["SPIDCLR"].mEndAddr);
        uint32_t SPIDERR_cur = (*GSPIDERRSTAT)["SPIDERR"];
        uint32_t mask = SPIDCLR_cur ^ 0xFFFFFFFF;
        (*GSPIDERRSTAT)["SPIDERR"] = mask & SPIDERR_cur;
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *key code enable* field of
 *        *key code protect* register
 */
void IBG_U2_Func::cb_GKCPROT_KCE(RegCBstr str) 
{//{{{
    if (str.is_wr) {
        /// -# the *key code enable* field is changed if the *key code protec*
        ///    field is match with a predefined code.
        uint32_t KCPROT_cur = bit_select(str.data
                                        ,(*GKCPROT)["KCPROT"].mStartAddr
                                        ,(*GKCPROT)["KCPROT"].mEndAddr);
        uint8_t  KCE_pre    = bit_select(str.pre_data
                                        ,(*GKCPROT)["KCE"].mStartAddr
                                        ,(*GKCPROT)["KCE"].mEndAddr);
        uint8_t  KCE_cur    = bit_select(str.data
                                        ,(*GKCPROT)["KCE"].mStartAddr
                                        ,(*GKCPROT)["KCE"].mEndAddr);
        if (KEY_CODE_PROT == KCPROT_cur) {
            (*GKCPROT)["KCE"] = KCE_cur;
        }
        else {
            (*GKCPROT)["KCE"] = KCE_pre;
        }
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *read-global enable* field
 *        of *protection setting 0* register
 */
void IBG_U2_Func::cb_GPROT0_RG(RegCBstr str) 
{//{{{
    if (str.is_wr) {
        /// -# the *read-global enable* field is changed only if *key code enable*
        ///    field on *key code protect* register is enable.
        uint8_t  RG_pre = bit_select(str.pre_data
                                    ,(*GPROT0[str.channel])["RG"].mStartAddr
                                    ,(*GPROT0[str.channel])["RG"].mEndAddr);
        uint8_t  WG_pre = bit_select(str.pre_data
                                    ,(*GPROT0[str.channel])["WG"].mStartAddr
                                    ,(*GPROT0[str.channel])["WG"].mEndAddr);
        uint8_t  UM_pre = bit_select(str.pre_data
                                    ,(*GPROT0[str.channel])["UM"].mStartAddr
                                    ,(*GPROT0[str.channel])["UM"].mEndAddr);
        uint8_t  DBG_pre= bit_select(str.pre_data
                                    ,(*GPROT0[str.channel])["DBG"].mStartAddr
                                    ,(*GPROT0[str.channel])["DBG"].mEndAddr);
        uint8_t  GEN_pre = bit_select(str.pre_data
                                    ,(*GPROT0[str.channel])["GEN"].mStartAddr
                                    ,(*GPROT0[str.channel])["GEN"].mEndAddr);
                                    
        uint8_t  RG_cur = bit_select(str.data
                                    ,(*GPROT0[str.channel])["RG"].mStartAddr
                                    ,(*GPROT0[str.channel])["RG"].mEndAddr);
        uint8_t  WG_cur = bit_select(str.data
                                    ,(*GPROT0[str.channel])["WG"].mStartAddr
                                    ,(*GPROT0[str.channel])["WG"].mEndAddr);
        uint8_t  UM_cur = bit_select(str.data
                                    ,(*GPROT0[str.channel])["UM"].mStartAddr
                                    ,(*GPROT0[str.channel])["UM"].mEndAddr);
        uint8_t  DBG_cur= bit_select(str.data
                                    ,(*GPROT0[str.channel])["DBG"].mStartAddr
                                    ,(*GPROT0[str.channel])["DBG"].mEndAddr);
        uint8_t  GEN_cur = bit_select(str.data
                                    ,(*GPROT0[str.channel])["GEN"].mStartAddr
                                    ,(*GPROT0[str.channel])["GEN"].mEndAddr);
            
        if ((uint32_t)(*GKCPROT)["KCE"] == 1) {
            (*GPROT0[str.channel])["RG"] = RG_cur;
            (*GPROT0[str.channel])["WG" ] = WG_cur;
            (*GPROT0[str.channel])["UM" ] = UM_cur;
            (*GPROT0[str.channel])["DBG"] = DBG_cur;
            (*GPROT0[str.channel])["GEN"] = GEN_cur;
        } else {
            (*GPROT0[str.channel])["RG"] = RG_pre;
            (*GPROT0[str.channel])["WG" ] = WG_pre;
            (*GPROT0[str.channel])["UM" ] = UM_pre;
            (*GPROT0[str.channel])["DBG"] = DBG_pre;
            (*GPROT0[str.channel])["GEN"] = GEN_pre;
        }
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *SPID* fields
 *        of *protection setting 1* register
 */
void IBG_U2_Func::cb_GPROT1_SPID(RegCBstr str) 
{//{{{
    if (str.is_wr) {
        /// -# the *SPID* field is changed only if *key code enable*
        ///    field on *key code protect* register is enable.
        uint32_t  SPID_pre = bit_select(str.pre_data
                                       ,(*GPROT1[str.channel])["SPID"].mStartAddr
                                       ,(*GPROT1[str.channel])["SPID"].mEndAddr);
        uint32_t  SPID_cur = bit_select(str.data
                                       ,(*GPROT1[str.channel])["SPID"].mStartAddr
                                       ,(*GPROT1[str.channel])["SPID"].mEndAddr);
        if ((uint32_t)(*GKCPROT)["KCE"] == 1)
            (*GPROT1[str.channel])["SPID"] = SPID_cur;
        else
            (*GPROT1[str.channel])["SPID"] = SPID_pre;
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief update error transaction information
 */
void IBG_U2_Func::UpdateErrorTransInfo(const uint32_t  addr
                              ,const bool      isDebug
                              ,const bool      isUserMode
                              ,const bool      isWrite
                              ,const uint8_t   SPID
                              )
{//{{{
    uint32_t SPIDERR_cur = (uint32_t)(*GSPIDERRSTAT)["SPIDERR"];
    uint32_t SPID_cur_temp = SPIDERR_cur; 
    //Update Guard Error Address Register
    if (SPIDERR_cur == 0) {
        (*GERRADDR)["ADDR" ] = addr;
        //Update Guard Error Access Information Register
        (*GERRTYPE)["DBG"  ] = isDebug;
        (*GERRTYPE)["UM"   ] = isUserMode;
        (*GERRTYPE)["SPID" ] = SPID;
        (*GERRTYPE)["WRITE"] = isWrite;
    }
    (*GSPIDERRSTAT)["SPIDERR"] = SPIDERR_cur | (1 << SPID);
    
    if (SPIDERR_cur != 0) {
        if (0 == (*GOVFSTAT)["OVF"]) {
            (*GOVFSTAT)["OVF"] = 1;
        }
    }
    
    if ((SPID_cur_temp & (1 << SPID)) != (1 << SPID)) {
        //Notify error to ECM
        this->NotifyError();
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief notify error to ECM
 */
void IBG_U2_Func::NotifyError()
{//{{{
    pWrapper->NotifyError();
}//}}}
//------------------------------------------------------------------------------
/**@brief get value of GSPIDERRSTAT register
 */
uint32_t IBG_U2_Func::GetGSPIDERRSTAT()
{//{{{
    return (uint32_t)(*GSPIDERRSTAT);
}//}}}
//------------------------------------------------------------------------------
/**@brief get value of GOVFSTAT register
 */
uint32_t IBG_U2_Func::GetGOVFSTAT()
{//{{{
    return (uint32_t)(*GOVFSTAT);
}//}}}
// vim600: set foldmethod=marker :

