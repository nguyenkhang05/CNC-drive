// ---------------------------------------------------------------------
// $Id: INTC2G_Func.cpp,v 1.4 2019/09/23 09:08:06 ngathunguyen Exp $
//
// Copyright(c) 2017-2019 Renesas Electronics Corporation
// Copyright(c) 2017-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include"INTC2G_Func.h"

/**@brief constructor of INTC2G function class
 * @details
 * @note
 */
INTC2G_Func::INTC2G_Func(string iname, INTC2G* ipWrapper, uint32_t INTC2GPROT_m_num)
:Cintc2g_regif(iname,32)
,pWrapper(ipWrapper)
,KEY_CODE_PROT((0xA5A5A500>>1))
,mModuleName(iname)
,mINTC2GPROT_num(INTC2GPROT_m_num)
{//{{{
    /// -# connect to the wrapper
    sc_assert(pWrapper != NULL);
    pWrapper->CommandInit(pWrapper->basename());
    /// -# initialize the *register block* base class
    Cintc2g_regif::set_instance_name(pWrapper->basename());
}//}}}

/**@brief destructor of INTC2G function class
 * @details
 * @note
 */
INTC2G_Func::~INTC2G_Func() {}

/**@brief handle command passed into the register block via Python interface
 * @details
 * @note this function is kept unchanged from other models
 */
void INTC2G_Func::RegisterHandler (const std::vector<std::string>& args)
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
                msg_return = Cintc2g_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Ctsn_regif class
            msg_return = Cintc2g_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/**@brief implement read function BusSlaveFuncIf::read via BusSlaveFuncIf
 * @details
 * @note
 */
void INTC2G_Func::read  (uint32_t            offsetAddress
                        ,TlmBasicPayload&    trans
                        ,BusTime_t*          t
                        ,bool                debug)

{//{{{
    std::memset(trans.get_data_ptr()
               ,0
               ,trans.get_data_length()
               );
    /// Get extension info
    TlmG3mExtension *extension;
    trans.get_extension(extension);
    /// get status of reset
    bool mResetStatus = pWrapper->GetGuardResetStatus(); 
    /// When clock == 0 or assert reset, only bebug master read.
    if (!pWrapper->CheckGuardClock() || mResetStatus) {
        if (!debug || (extension == NULL) || (debug && extension->getPeId( ) != DEBUG_MASTER_BMID)) {
            if (mResetStatus) { /// during reset
                re_printf("warning", "Cannot read during reset period\n");
            } else {
                re_printf("warning", "Cannot access register when clock is 0.\n");
            }
            return;
        }
    }               
    if ((offsetAddress <= (0x100 + (2048 *4))) && (offsetAddress > (0x100 + (mINTC2GPROT_num *4)))) {
        re_printf("error", "Invalid access address 0x%08X\n", (uint32_t)trans.get_address());
        return;
    }
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

/**@brief implement write function BusSlaveFuncIf::write via BusSlaveFuncIf
 * @details
 * @note
 */
void INTC2G_Func::write (uint32_t            offsetAddress
                        ,TlmBasicPayload&    trans
                        ,BusTime_t*          t
                        ,bool                debug)
{//{{{
    /// Get extension infor
    TlmG3mExtension *extension;
    trans.get_extension(extension);
    
    /// When clock = 0, only bebug master write.
    if (!pWrapper->CheckGuardClock()) {
        if (!debug || (extension == NULL) || (debug && extension->getPeId() != DEBUG_MASTER_BMID)){
            re_printf("warning", "Can not access to register during zero clock\n");
            return;
        }
    }
    if ((offsetAddress <= (0x100 + (2048 *4))) && (offsetAddress > (0x100 + (mINTC2GPROT_num *4)))) {
        re_printf("error", "Invalid access address 0x%08X\n.", (uint32_t)trans.get_address());
        return;
    }
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

/**@brief get value of INTC2GERRSTAT register
 */
uint32_t INTC2G_Func::GetINTC2GERRSTAT()
{//{{{
    return (uint32_t)(*INTC2GERRSTAT);
}//}}}

/**@brief set number of INTC2GPROT_n register
 */
void INTC2G_Func::SetINTC2GPROTmNum(unsigned int channelnum)
{//{{{
    mINTC2GPROT_num = channelnum;
}//}}}

/**@brief Callback function
 * @details
 * @note
 */
void INTC2G_Func::cb_INTC2GKCPROT_KCE(RegCBstr str)
{//{{{
    uint8_t KCE_cur = bit_select(str.data
                                ,(*INTC2GKCPROT)["KCE"].mStartAddr
                                ,(*INTC2GKCPROT)["KCE"].mEndAddr);
    uint8_t KCE_pre = bit_select(str.pre_data
                                ,(*INTC2GKCPROT)["KCE"].mStartAddr
                                ,(*INTC2GKCPROT)["KCE"].mEndAddr);

    uint32_t KCPROT_value = bit_select(str.data
                                ,(*INTC2GKCPROT)["KCPROT"].mStartAddr
                                ,(*INTC2GKCPROT)["KCPROT"].mEndAddr);

    if(KCPROT_value == KEY_CODE_PROT){
        (*INTC2GKCPROT)["KCE"] = KCE_cur;
    } else {
        (*INTC2GKCPROT)["KCE"] = KCE_pre;
    }
}//}}}

/**@brief Callback function when write to clear status register
 * @details
 * @note
 */
void INTC2G_Func::cb_INTC2GERRCLR_CLRE(RegCBstr str)
{//{{{
    uint8_t CLRE_cur = bit_select(str.data
                                ,(*INTC2GERRCLR)["CLRE"].mStartAddr
                                ,(*INTC2GERRCLR)["CLRE"].mEndAddr);
    uint8_t CLRO_cur = bit_select(str.data
                                ,(*INTC2GERRCLR)["CLRO"].mStartAddr
                                ,(*INTC2GERRCLR)["CLRO"].mEndAddr);
    if (1 == CLRE_cur) {
        (*INTC2GERRSTAT)["ERR"] = 0;
    }
    if (1 == CLRO_cur) {
        (*INTC2GERRSTAT)["OVF"] = 0;        
    }
}//}}}

void INTC2G_Func::cb_INTC2GMPID_SPID(RegCBstr str)
{//{{{
    uint8_t SPID_pre = bit_select(str.pre_data
                                 ,(*INTC2GMPID[str.channel])["SPID"].mStartAddr
                                 ,(*INTC2GMPID[str.channel])["SPID"].mEndAddr);
    uint8_t SPID_cur = bit_select(str.data
                                 ,(*INTC2GMPID[str.channel])["SPID"].mStartAddr
                                 ,(*INTC2GMPID[str.channel])["SPID"].mEndAddr);

    if ((*INTC2GKCPROT)["KCE"] == 1) {
        (*INTC2GMPID[str.channel])["SPID"] = SPID_cur;
    } else {
        (*INTC2GMPID[str.channel])["SPID"] = SPID_pre;
    }
}//}}}

/**@brief Callback function when write to Guard Control Register
 * @details
 * @note
 */
void INTC2G_Func::cb_INTC2GPROT_GR_RG(RegCBstr str)
{//{{{
    /// -# these fields are changed only if *key code enable*
    ///    field on *key code protect* register is enable.
    uint8_t  RG_pre = bit_select(str.pre_data
                                ,(*INTC2GPROT_GR)["RG"].mStartAddr
                                ,(*INTC2GPROT_GR)["RG"].mEndAddr);
    uint8_t  WG_pre = bit_select(str.pre_data
                                ,(*INTC2GPROT_GR)["WG"].mStartAddr
                                ,(*INTC2GPROT_GR)["WG"].mEndAddr);
    uint8_t  UM_pre = bit_select(str.pre_data
                                ,(*INTC2GPROT_GR)["UM"].mStartAddr
                                ,(*INTC2GPROT_GR)["UM"].mEndAddr);
    uint8_t  DBG_pre= bit_select(str.pre_data
                                ,(*INTC2GPROT_GR)["DBG"].mStartAddr
                                ,(*INTC2GPROT_GR)["DBG"].mEndAddr);
    uint8_t  GEN_pre = bit_select(str.pre_data
                                ,(*INTC2GPROT_GR)["GEN"].mStartAddr
                                ,(*INTC2GPROT_GR)["GEN"].mEndAddr);
    uint8_t  MPID_pre = bit_select(str.pre_data
                                ,(*INTC2GPROT_GR)["MPID"].mStartAddr
                                ,(*INTC2GPROT_GR)["MPID"].mEndAddr);

    uint8_t  RG_cur = bit_select(str.data
                                ,(*INTC2GPROT_GR)["RG"].mStartAddr
                                ,(*INTC2GPROT_GR)["RG"].mEndAddr);
    uint8_t  WG_cur = bit_select(str.data
                                ,(*INTC2GPROT_GR)["WG"].mStartAddr
                                ,(*INTC2GPROT_GR)["WG"].mEndAddr);
    uint8_t  UM_cur = bit_select(str.data
                                ,(*INTC2GPROT_GR)["UM"].mStartAddr
                                ,(*INTC2GPROT_GR)["UM"].mEndAddr);
    uint8_t  DBG_cur= bit_select(str.data
                                ,(*INTC2GPROT_GR)["DBG"].mStartAddr
                                ,(*INTC2GPROT_GR)["DBG"].mEndAddr);
    uint8_t  GEN_cur = bit_select(str.data
                                ,(*INTC2GPROT_GR)["GEN"].mStartAddr
                                ,(*INTC2GPROT_GR)["GEN"].mEndAddr);
    uint8_t  MPID_cur = bit_select(str.data
                                ,(*INTC2GPROT_GR)["MPID"].mStartAddr
                                ,(*INTC2GPROT_GR)["MPID"].mEndAddr);
    if ((*INTC2GKCPROT)["KCE"] == 1) {
        (*INTC2GPROT_GR)["RG" ] = RG_cur;
        (*INTC2GPROT_GR)["WG" ] = WG_cur;
        (*INTC2GPROT_GR)["UM" ] = UM_cur;
        (*INTC2GPROT_GR)["DBG"] = DBG_cur;
        (*INTC2GPROT_GR)["GEN"] = GEN_cur;
        (*INTC2GPROT_GR)["MPID"] = MPID_cur;
    } else {
        (*INTC2GPROT_GR)["RG" ] = RG_pre;
        (*INTC2GPROT_GR)["WG" ] = WG_pre;
        (*INTC2GPROT_GR)["UM" ] = UM_pre;
        (*INTC2GPROT_GR)["DBG"] = DBG_pre;
        (*INTC2GPROT_GR)["GEN"] = GEN_pre;
        (*INTC2GPROT_GR)["MPID"] = MPID_pre;
    }
}//}}}

/**@brief Callback function when write to Guard Control Register
 * @details
 * @note
 */
void INTC2G_Func::cb_INTC2GPROT_RG(RegCBstr str)
{//{{{

    /// -# these fields are changed only if *key code enable*
    ///    field on *key code protect* register is enable.
    uint8_t  RG_pre = bit_select(str.pre_data
                                ,(*INTC2GPROT[str.channel])["RG"].mStartAddr
                                ,(*INTC2GPROT[str.channel])["RG"].mEndAddr);
    uint8_t  WG_pre = bit_select(str.pre_data
                                ,(*INTC2GPROT[str.channel])["WG"].mStartAddr
                                ,(*INTC2GPROT[str.channel])["WG"].mEndAddr);
    uint8_t  UM_pre = bit_select(str.pre_data
                                ,(*INTC2GPROT[str.channel])["UM"].mStartAddr
                                ,(*INTC2GPROT[str.channel])["UM"].mEndAddr);
    uint8_t  DBG_pre =bit_select(str.pre_data
                                ,(*INTC2GPROT[str.channel])["DBG"].mStartAddr
                                ,(*INTC2GPROT[str.channel])["DBG"].mEndAddr);
    uint8_t  GEN_pre = bit_select(str.pre_data
                                ,(*INTC2GPROT[str.channel])["GEN"].mStartAddr
                                ,(*INTC2GPROT[str.channel])["GEN"].mEndAddr);
    uint8_t  MPID_pre = bit_select(str.pre_data
                                ,(*INTC2GPROT[str.channel])["MPID"].mStartAddr
                                ,(*INTC2GPROT[str.channel])["MPID"].mEndAddr);

    uint8_t  RG_cur = bit_select(str.data
                                ,(*INTC2GPROT[str.channel])["RG"].mStartAddr
                                ,(*INTC2GPROT[str.channel])["RG"].mEndAddr);
    uint8_t  WG_cur = bit_select(str.data
                                ,(*INTC2GPROT[str.channel])["WG"].mStartAddr
                                ,(*INTC2GPROT[str.channel])["WG"].mEndAddr);
    uint8_t  UM_cur = bit_select(str.data
                                ,(*INTC2GPROT[str.channel])["UM"].mStartAddr
                                ,(*INTC2GPROT[str.channel])["UM"].mEndAddr);
    uint8_t  DBG_cur = bit_select(str.data
                                ,(*INTC2GPROT[str.channel])["DBG"].mStartAddr
                                ,(*INTC2GPROT[str.channel])["DBG"].mEndAddr);
    uint8_t  GEN_cur = bit_select(str.data
                                ,(*INTC2GPROT[str.channel])["GEN"].mStartAddr
                                ,(*INTC2GPROT[str.channel])["GEN"].mEndAddr);
    uint8_t  MPID_cur = bit_select(str.data
                                ,(*INTC2GPROT[str.channel])["MPID"].mStartAddr
                                ,(*INTC2GPROT[str.channel])["MPID"].mEndAddr);
    if ((*INTC2GKCPROT)["KCE"] == 1) {
        (*INTC2GPROT[str.channel])["RG" ] = RG_cur;
        (*INTC2GPROT[str.channel])["WG" ] = WG_cur;
        (*INTC2GPROT[str.channel])["UM" ] = UM_cur;
        (*INTC2GPROT[str.channel])["DBG"] = DBG_cur;
        (*INTC2GPROT[str.channel])["GEN"] = GEN_cur;
        (*INTC2GPROT[str.channel])["MPID"] = MPID_cur;
    } else {
        (*INTC2GPROT[str.channel])["RG" ] = RG_pre;
        (*INTC2GPROT[str.channel])["WG" ] = WG_pre;
        (*INTC2GPROT[str.channel])["UM" ] = UM_pre;
        (*INTC2GPROT[str.channel])["DBG"] = DBG_pre;
        (*INTC2GPROT[str.channel])["GEN"] = GEN_pre;
        (*INTC2GPROT[str.channel])["MPID"] = MPID_pre;
    }
}//}}}

// vim600: set foldmethod=marker :
