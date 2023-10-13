// ---------------------------------------------------------------------
// $Id: IBG_Func.cpp,v 1.6 2019/05/30 01:49:33 nhuvo Exp $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "IBG_Func.h"

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
IBG_Func::IBG_Func(string iname, IBG*  ipWrapper, uint iChannel)
    : Cibg_regif(iname, 32, iChannel)
    // internal attributes
    , KEY_CODE_PROT ((0xA5A5A500 >> 1))
    , pWrapper    (ipWrapper)
    , mModuleName (iname)
{//{{{
    /// -# connect to the wrapper
    sc_assert(pWrapper != NULL);
    pWrapper->CommandInit(pWrapper->basename());

    /// -# initialize the *register block* base class
    Cibg_regif::set_instance_name(pWrapper->basename());

    /// -# initialize internal status attributes
    Initialize ();

    validate_configuration();
}//}}}
//------------------------------------------------------------------------------
/**@brief public destructor
*/
IBG_Func::~IBG_Func() {}
//------------------------------------------------------------------------------
/**@brief handle command passed into the register block via Python interface
* @details
* @note this function is kept unchanged from other models
*/
void IBG_Func::RegisterHandler (const std::vector<std::string>& args)
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
                msg_return = Cibg_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Ctsn_regif class
            msg_return = Cibg_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief read access implementation BusSlaveFuncIf::read via BusSlaveFuncIf
*/
void IBG_Func::read (uint32_t         offsetAddress ///< [in]    offset address
                     ,TlmBasicPayload& trans         ///< [inout] transaction
                     ,BusTime_t*       t             ///< [inout] time
                     ,bool             debug         ///< [in]    debug access
                     )
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    if (pExtInfo != NULL) {
        int mPEID = pExtInfo->getPeId();
        memset(trans.get_data_ptr(), 0, trans.get_data_length());
        if ((false == pWrapper->CheckGuardClock()) || (true == pWrapper->GetGuardResetStatus())) {
            // Software debugger
            if ((true == debug) && (DEBUG_MASTER_BMID == mPEID)) {
                reg_rd_dbg((uint32_t)trans.get_address()
                    ,          trans.get_data_ptr()
                    ,          trans.get_data_length()
                    );
            } else {
                re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
            }
        } else {
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
        }
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief write access implementation of BusSlaveFuncIf::write via BusSlaveFuncIf
*/
void IBG_Func::write(uint32_t          offsetAddress ///< [in] offset address
                     ,TlmBasicPayload&  trans         ///< [inout] transaction
                     ,BusTime_t*        t             ///< [inout] time
                     ,bool              debug         ///< [in] debug access
                     )
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    if(pExtInfo != NULL) {
        int mPEID = pExtInfo->getPeId();
        if (pWrapper->CheckGuardClock()){
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
        } else {
            // Software debugger
            if ((true == debug) && (DEBUG_MASTER_BMID == mPEID)) {
                reg_wr_dbg((uint32_t)trans.get_address()
                    ,          trans.get_data_ptr()
                    ,          trans.get_data_length()
                    );
            } else {
                re_printf("warning", "Cannot write to register block during zero clock\n");
            }
        }
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief validate assignment between DFE channels and FIFO channels
*/
bool IBG_Func::validate_configuration() {//{{{
    bool isPass = true;

    return (isPass);
}//}}}
//------------------------------------------------------------------------------
/**@brief initialize internal variables, called when construction process or reset process
*/
void IBG_Func::Initialize () {}
//------------------------------------------------------------------------------
/**@brief cancel all internal events, called in reset process
*/
void IBG_Func::CancelEvent() {}
//------------------------------------------------------------------------------
/**@brief API support reset from the outside
*/
void IBG_Func::EnableReset(const bool& isActive) {//{{{
    Cibg_regif::EnableReset(isActive);
    CancelEvent();
    Initialize();
}//}}}
//------------------------------------------------------------------------------
/**@brief API support PRESETn reset from the outside
*/
void IBG_Func::EnablePRESETnReset(const bool& isActive) {//{{{
    Cibg_regif::mIsReset = isActive;
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *clear error status* field
*        of *guard error status clear* register
*/
void IBG_Func::cb_GERRCLR_CLRE(RegCBstr str) {//{{{
    if (str.is_wr) {
        uint8_t CLRE_cur = bit_select(str.data
            ,(*GERRCLR)["CLRE"].mStartAddr
            ,(*GERRCLR)["CLRE"].mEndAddr);
        if (1 == CLRE_cur)
            (*GERRSTAT)["ERR"] = 0;
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *clear overflow status* field
*        of *guard error status clear* register
*/
void IBG_Func::cb_GERRCLR_CLRO(RegCBstr str) {//{{{
    if (str.is_wr) {
        uint8_t CLRO_cur = bit_select(str.data
            ,(*GERRCLR)["CLRO"].mStartAddr
            ,(*GERRCLR)["CLRO"].mEndAddr);
        if (1 == CLRO_cur)
            (*GERRSTAT)["OVF"] = 0;
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *key code enable* field of
*        *key code protect* register
*/
void IBG_Func::cb_GKCPROT_KCE(RegCBstr str) 
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
        if (KEY_CODE_PROT == KCPROT_cur)
            (*GKCPROT)["KCE"] = KCE_cur;
        else
            (*GKCPROT)["KCE"] = KCE_pre;
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *key code protection* field
*        of *Key code protect* register
*/
void IBG_Func::cb_GKCPROT_KCPROT(RegCBstr str) 
{//{{{
    if (str.is_wr) {
        /// -# the key code is never changed
        uint32_t KCPROT_pre = bit_select(str.pre_data
            ,(*GKCPROT)["KCPROT"].mStartAddr
            ,(*GKCPROT)["KCPROT"].mEndAddr);
        (*GKCPROT)["KCPROT"] = KCPROT_pre;
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *read-global enable* field
*        of *protection setting 0* register
*/
void IBG_Func::cb_GPROT0_RG(RegCBstr str)
{//{{{
    if (str.is_wr) {
        /// -# the *read-global enable* field is changed only if *key code enable*
        ///    field on *key code protect* register is enable.
        uint8_t  RG_pre = bit_select(str.pre_data
            ,(*GPROT0[str.channel])["RG"].mStartAddr
            ,(*GPROT0[str.channel])["RG"].mEndAddr);
        uint8_t  RG_cur = bit_select(str.data
            ,(*GPROT0[str.channel])["RG"].mStartAddr
            ,(*GPROT0[str.channel])["RG"].mEndAddr);
        if ((*GKCPROT)["KCE"] == 1)
            (*GPROT0[str.channel])["RG"] = RG_cur;
        else
            (*GPROT0[str.channel])["RG"] = RG_pre;
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *write-global enable*,
*        *user-mode*, *debug-mode*, *global-enable* fields of *protection
*        setting 0* register
*/
void IBG_Func::cb_GPROT0_WG(RegCBstr str) 
{//{{{
    if (str.is_wr) {
        /// -# these fields are changed only if *key code enable*
        ///    field on *key code protect* register is enable.
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
        if ((*GKCPROT)["KCE"] == 1) {
            (*GPROT0[str.channel])["WG" ] = WG_cur;
            (*GPROT0[str.channel])["UM" ] = UM_cur;
            (*GPROT0[str.channel])["DBG"] = DBG_cur;
            (*GPROT0[str.channel])["GEN"] = GEN_cur;
        } else {
            (*GPROT0[str.channel])["WG" ] = WG_pre;
            (*GPROT0[str.channel])["UM" ] = UM_pre;
            (*GPROT0[str.channel])["DBG"] = DBG_pre;
            (*GPROT0[str.channel])["GEN"] = GEN_pre;
        }
    }
}//}}}

/**@brief get value of GERRSTAT register
*/
uint32_t IBG_Func::GetGERRSTAT()
{//{{{
    return (uint32_t)(*GERRSTAT);
}//}}}

//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *SPID* fields
*        of *protection setting 1* register
*/
void IBG_Func::cb_GPROT1_SPID(RegCBstr str) 
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
        if ((*GKCPROT)["KCE"] == 1)
            (*GPROT1[str.channel])["SPID"] = SPID_cur;
        else
            (*GPROT1[str.channel])["SPID"] = SPID_pre;
    }
}//}}}
// vim600: set foldmethod=marker :
//------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------

