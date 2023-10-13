// -----------------------------------------------------------------------------
// $Id: HWCORE_INPUT_Func.cpp,v 1.7 2020/10/23 02:37:18 quynhnhunguyen Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------
// Description: Copyright of HWCORE_INPUT_Func.cpp
// Ref: {HWCORE_INPUT_UD_Copyright_001}
#include "HWCORE_INPUT_Func.h"
#include "HWCORE_INPUT_AgentController.h"
#ifndef func_re_printf
#define func_re_printf mpAgentController->get_fileline(__FILE__, __LINE__); mpAgentController->_re_printf
#endif//func_re_printf
//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
///*********************************
/// Function     : HWCORE_INPUT_Func
/// Description  : Constructor HWCORE_INPUT_Func
///*********************************
HWCORE_INPUT_Func::HWCORE_INPUT_Func(std::string name, HWCORE_INPUT_AgentController* AgentController) :
    Chwcore_input_regif(name, 32)
{//{{{
    if (AgentController == NULL) {
        return;
   }
   mpAgentController = AgentController;
   mpAgentController->CommandInit(name);
   mModuleName = name;
   Chwcore_input_regif::set_instance_name(name);
  // Initialize
   Initialize();

   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("CTRINMD"         , (CTRINMD)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("THTESFT"         , (THTESFT)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("THTEIBUF"        , (THTEIBUF)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("EARD"            , (EARD)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("THTESEL"         , (THTESEL)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("AD0IBUF"         , (AD0IBUF)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("AD1IBUF"         , (AD1IBUF)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("AD2IBUF"         , (AD2IBUF)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("AD0OFS"          , (AD0OFS)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("AD1OFS"          , (AD1OFS)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("AD2OFS"          , (AD2OFS)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("DIVLSB"          , (DIVLSB)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IUFIX"           , (IUFIX)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IVFIX"           , (IVFIX)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IWFIX"           , (IWFIX)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("KCLCTR"          , (KCLCTR)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("KCLJUD"          , (KCLJUD)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("KCLSTAT"         , (KCLSTAT)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("KCLSTATC"        , (KCLSTATC)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("KCLCNT"          , (KCLCNT)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("SR2"             , (SR2)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IDFIX"           , (IDFIX)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IQFIX"           , (IQFIX)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IUFIXOBUF"       , (IUFIXOBUF)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IVFIXOBUF"       , (IVFIXOBUF)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IWFIXOBUF"       , (IWFIXOBUF)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IDFIXOBUF"       , (IDFIXOBUF)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IQFIXOBUF"       , (IQFIXOBUF)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("THTRESFIXIN"     , (THTRESFIXIN)));
}//}}}
///*********************************
/// Function     : ~HWCORE_INPUT_Func
/// Description  : Destructor HWCORE_INPUT_Func
///*********************************
HWCORE_INPUT_Func::~HWCORE_INPUT_Func()
{//{{{
    mRegisterMap.clear();
}//}}}
///*********************************
/// Function     : Initialize
/// Description  : Initialize variables
///*********************************
void HWCORE_INPUT_Func::Initialize(void)
{//{{{

}//}}}
/// Process reset function
///*********************************
/// Function     : EnableReset
/// Description  : Enable Reset
///*********************************
void HWCORE_INPUT_Func::EnableReset(const bool is_active)
{//{{{
   if (is_active) {
      Initialize();
   }
   Chwcore_input_regif::EnableReset(is_active);
}//}}}
/// Handler for register IF
///*********************************
/// Function     : RegisterHandler 
/// Description  : Register Handler
///*********************************
void HWCORE_INPUT_Func::RegisterHandler(const std::vector<std::string> &args)
{//{{{
   std::string msg_return = "";
   if (args.size() == 1) {//Dump registers name
      vpcl::re_register *reg_p = first_reg_object();
      while (reg_p != NULL) {
         SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
         reg_p = next_reg_object();
      }
   }
   else {
      if (args[1] == "*") {//Set message level for each register
         std::vector<std::string> cmd;
         vpcl::re_register *reg_p = first_reg_object();
         while (reg_p != NULL) {
            cmd = args;
            cmd.erase(cmd.begin() + 1);
            cmd.insert(cmd.begin() + 1, reg_p->name());
            msg_return = Chwcore_input_regif::reg_handle_command(cmd);
            if (msg_return != "") {
               SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
            cmd.clear();
            reg_p = next_reg_object();
         }
      }
      else {//Set handle command to Chwcore_input_regif class
         msg_return = Chwcore_input_regif::reg_handle_command(args);
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
// Description: read register
// Ref: {HWCORE_INPUT_UD_Flow_004, HWCORE_INPUT_UD_ATT1_005}
void HWCORE_INPUT_Func::read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    if (data_ptr == NULL) {
        return;
    }
    if (!debug) {
        reg_rd(offsetAddress, data_ptr, size);
    }
    else {
        reg_rd_dbg(offsetAddress, data_ptr, size);
    }
}//}}}

///*********************************
/// Function     : write 
/// Description  : Write API (target socket)
///*********************************
// Description: write register
// Ref: {HWCORE_INPUT_UD_Flow_005}
void HWCORE_INPUT_Func::write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    if (data_ptr == NULL) {
        return;
    }
    if (!debug) {
        reg_wr(offsetAddress, data_ptr, size);
    }
    else {
        reg_wr_dbg(offsetAddress, data_ptr, size);
    }
}//}}}

/////*********************************
///// Function     : cb_KCLSTATC_CLR
///// Description  : callback KCLSTATC_CLR
/////*********************************
// Description: cb_KCLSTATC_CLR
// Ref: {HWCORE_INPUT_UD_Flow_008}
void HWCORE_INPUT_Func::cb_KCLSTATC_CLR(RegCBstr str)
{//{{{
    if((bool)((*KCLSTATC)["CLR"]) == true){
        (*KCLSTAT)["ST"] = 0;
        (*KCLCNT) = 0;
    }
    (*KCLSTATC)["CLR"] = 0;
}//}}}

/////*********************************
///// Function     : cb_CTRINMD_INSTCTR
///// Description  : callback CTRINMD_INSTCTR
/////*********************************
// Description: cb_CTRINMD_INSTCTR
// Ref: {HWCORE_INPUT_UD_Flow_009}
void HWCORE_INPUT_Func::cb_CTRINMD_INSTCTR(RegCBstr str)
{//{{{
   unsigned char instctr_iirdq = (str.data >> 6) & 0xFF;
   unsigned char cmuvw = (*CTRINMD)["CMUVW"];
   if (instctr_iirdq != ((str.pre_data >> 6) & 0xFF)){
      mpAgentController->NotifyUpdateOutputPorts("instctr_iirdq", instctr_iirdq);
   }
}//}}}

/////*********************************
///// Function     : cb_THTESFT_DATA
///// Description  : callback THTESFT_DATA
/////*********************************
// Description: cb_THTESFT_DATA
void HWCORE_INPUT_Func::cb_THTESEL_DATA(RegCBstr str)
{//{{{
    if (str.data != str.pre_data) {
        mpAgentController->NotifyUpdateOutputPorts("theta_e_sel",(str.data & 0xFFF));
    }
}//}}}

/////*********************************
///// Function     : cb_IUFIX_DATA
///// Description  : callback IUFIX_DATA
/////*********************************
// Description: cb_IUFIX_DATA
void HWCORE_INPUT_Func::cb_IUFIX_DATA(RegCBstr str)
{//{{{
    if (str.data != str.pre_data) {
        mpAgentController->NotifyUpdateOutputPorts("ia_u_lo_fix_reg", str.data);
    }
}//}}}

/////*********************************
///// Function     : cb_IVFIX_DATA
///// Description  : callback IVFIX_DATA
/////*********************************
// Description: cb_IVFIX_DATA
void HWCORE_INPUT_Func::cb_IVFIX_DATA(RegCBstr str)
{//{{{
    if (str.data != str.pre_data) {
        mpAgentController->NotifyUpdateOutputPorts("ia_v_lo_fix_reg", str.data);
    }
}//}}}

/////*********************************
///// Function     : cb_IWFIX_DATA
///// Description  : callback IWFIX_DATA
/////*********************************
// Description: cb_IWFIX_DATA
void HWCORE_INPUT_Func::cb_IWFIX_DATA(RegCBstr str)
{//{{{
    if (str.data != str.pre_data) {
        mpAgentController->NotifyUpdateOutputPorts("ia_w_lo_fix_reg", str.data);
    }
}//}}}

/////*********************************
///// Function     : cb_IDFIXOBUF_DATA
///// Description  : callback IDFIXOBUF_DATA
/////*********************************
// Description: cb_IDFIXOBUF_DATA
void HWCORE_INPUT_Func::cb_IDFIXOBUF_DATA(RegCBstr str)
{//{{{
    if (str.data != str.pre_data) {
        mpAgentController->NotifyUpdateOutputPorts("rdata_em2iidfixobuf", str.data);
    }
}//}}}

/////*********************************
///// Function     : cb_IQFIXOBUF_DATA
///// Description  : callback IQFIXOBUF_DATA
/////*********************************
// Description: cb_IQFIXOBUF_DATA
void HWCORE_INPUT_Func::cb_IQFIXOBUF_DATA(RegCBstr str)
{//{{{
    if (str.data != str.pre_data) {
        mpAgentController->NotifyUpdateOutputPorts("rdata_em2iiqfixobuf", str.data);
    }
}//}}}

/////*********************************
///// Function     : cb_AD0IBUF_DATA
///// Description  : callback AD0IBUF_DATA
/////*********************************
// Description: cb_AD0IBUF_DATA
void HWCORE_INPUT_Func::cb_AD0IBUF_DATA(RegCBstr str)
{//{{{
    if (str.data != str.pre_data) {
        mpAgentController->NotifyUpdateOutputPorts("ia_1ch_lo_org_sel", (str.data & 0xFFF));
    }
}//}}}

/////*********************************
///// Function     : cb_AD1IBUF_DATA
///// Description  : callback AD1IBUF_DATA
/////*********************************
// Description: cb_AD1IBUF_DATA
void HWCORE_INPUT_Func::cb_AD1IBUF_DATA(RegCBstr str)
{//{{{
    if (str.data != str.pre_data) {
        mpAgentController->NotifyUpdateOutputPorts("ia_2ch_lo_org_sel", (str.data & 0xFFF));
    }
}//}}}

/////*********************************
///// Function     : cb_AD2IBUF_DATA
///// Description  : callback AD2IBUF_DATA
/////*********************************
// Description: cb_AD2IBUF_DATA
void HWCORE_INPUT_Func::cb_AD2IBUF_DATA(RegCBstr str)
{//{{{
    if (str.data != str.pre_data) {
        mpAgentController->NotifyUpdateOutputPorts("ia_3ch_lo_org_sel", (str.data & 0xFFF));
    }
}//}}}

/////*********************************
///// Function     : GetRegUnsignedVal
///// Description  : Get Register Value with unsigned int Type
/////*********************************
unsigned int HWCORE_INPUT_Func::GetRegUnsignedVal(std::string RegName)
{//{{{
    return ((unsigned int)(*(mRegisterMap[RegName.c_str()])));
}//}}}

/////*********************************
///// Function     : GetRegSignedVal
///// Description  : Get Register Value with signed int Type
/////*********************************
signed int HWCORE_INPUT_Func::GetRegSignedVal(std::string RegName)
{//{{{
    return ((signed int)(*(mRegisterMap[RegName.c_str()])));
}//}}}

/////*********************************
///// Function     : SetRegUnsignedVal
///// Description  : Set Register Value with unsigned int Type
/////*********************************
void HWCORE_INPUT_Func::SetRegUnsignedVal(std::string RegName, unsigned int val)
{//{{{
    (*(mRegisterMap[RegName.c_str()])) = val;
}//}}}

/////*********************************
///// Function     : SetRegSignedVal
///// Description  : Set Register Value with signed int Type
/////*********************************
void HWCORE_INPUT_Func::SetRegSignedVal(std::string RegName, signed int val)
{//{{{
    (*(mRegisterMap[RegName.c_str()])) = val;
}//}}}

// vim600: set foldmethod=marker :
