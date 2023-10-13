// ---------------------------------------------------------------------
// $Id: MSPI000_INT_FACTOR_Func.cpp,v 1.4 2020/04/03 05:00:07 ducla Exp $
//
// Copyright(c) 2019-2020 Renesas Electronics Corporation
// Copyright(c) 2019-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "MSPI000_INT_FACTOR_Func.h"
#include "MSPI000_AgentController.h"
//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
///*********************************
/// Function     : MSPI000_INT_FACTOR_Func
/// Description  : Constructor MSPI000_INT_FACTOR_Func
///*********************************
MSPI000_INT_FACTOR_Func::MSPI000_INT_FACTOR_Func(std::string name, unsigned int ch_num,  MSPI000_AgentController* AgentController) :
   Cint_factor_regif(name, 32, ch_num)
   , BusSlaveFuncIf()
{//{{{
   sc_assert(AgentController != NULL);
   mpAgentController = AgentController;
   mpAgentController->CommandInit(name);
   mModuleName = name;
   mNumChannel = ch_num;
   Cint_factor_regif::set_instance_name(name);
   // Initialize
   Initialize();
}//}}}
 ///*********************************
 /// Function     : ~MSPI000_INT_FACTOR_Func
 /// Description  : Destructor MSPI000_INT_FACTOR_Func
 ///*********************************
MSPI000_INT_FACTOR_Func::~MSPI000_INT_FACTOR_Func()
{//{{{

}//}}}
 ////////////////////////////////////////////////////////////
 ///*********************************
 /// Function     : Initialize
 /// Description  : Initialize variables
 ///*********************************
void MSPI000_INT_FACTOR_Func::Initialize(void)
{//{{{

}//}}}
 /// Process reset function
 ///*********************************
 /// Function     : EnableReset
 /// Description  : Enable Reset
 ///*********************************
void MSPI000_INT_FACTOR_Func::EnableReset(const bool is_active)
{//{{{
   if (is_active) {
      Initialize();
   }
   Cint_factor_regif::EnableReset(is_active);
}//}}}
 /// Handler for register IF
 ///*********************************
 /// Function     : RegisterHandler 
 /// Description  : Register Handler
 ///*********************************
void MSPI000_INT_FACTOR_Func::RegisterHandler(const std::vector<std::string> &args)
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
            msg_return = Cint_factor_regif::reg_handle_command(cmd);
            if (msg_return != "") {
               SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
            cmd.clear();
            reg_p = next_reg_object();
         }
      }
      else {//Set handle command to Cint_factor_regif class
         msg_return = Cint_factor_regif::reg_handle_command(args);
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
void MSPI000_INT_FACTOR_Func::read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
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

   if (!debug) {
      reg_rd(offsetAddress, trans.get_data_ptr(), trans.get_data_length());
   }
   else {
      reg_rd_dbg(offsetAddress, trans.get_data_ptr(), trans.get_data_length());
   }
}//}}}
 ///*********************************
 /// Function     : write 
 /// Description  : Write API (target socket)
 ///*********************************
void MSPI000_INT_FACTOR_Func::write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
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

   if (!debug) {
      reg_wr(offsetAddress, trans.get_data_ptr(), trans.get_data_length());
   }
   else {
      reg_wr_dbg(offsetAddress, trans.get_data_ptr(), trans.get_data_length());
   }

}//}}}
///*********************************
/// Function     : GetMSPInINTFk
/// Description  : GetMSPInINTFk
///*********************************
unsigned int MSPI000_INT_FACTOR_Func::GetMSPInINTFk(unsigned int chid)
{//{{{
    return (unsigned int)(*MSPInINTFk[chid])["MSPInINTFk"];
}//}}}
///*********************************
/// Function     : GetMSPInINTMSKk
/// Description  : GetMSPInINTMSKk
///*********************************
unsigned int MSPI000_INT_FACTOR_Func::GetMSPInINTMSKk(unsigned int chid)
{//{{{
    return (unsigned int)(*MSPInINTMSKk[chid])["MSPInINTMSKk"];
}//}}}
 ///*********************************
 /// Function     : cb_MSPInINTFCk_MSPInINTFCk
 /// Description  : cb_MSPInINTFCk_MSPInINTFCk
 ///*********************************
void MSPI000_INT_FACTOR_Func::cb_MSPInINTFCk_MSPInINTFCk(RegCBstr str)
{//{{{
 //write only read 0
    if(*MSPInINTFCk[str.channel] !=0){
        (*MSPInINTFk[str.channel])=(*MSPInINTFk[str.channel]) &(~(*MSPInINTFCk[str.channel]));
        if(str.channel==0){
            mpAgentController->NotifyEventOutputINT(emNumTX);
        }else if (str.channel==1){
            mpAgentController->NotifyEventOutputINT(emNumRX);
        }else if (str.channel==2){
            mpAgentController->NotifyEventOutputINT(emNumFE);
        }else{
            mpAgentController->NotifyEventOutputINT(emNumERR);
        }
    }
    *MSPInINTFCk[str.channel]=0;

}//}}}
 ///*********************************
 /// Function     : cb_MSPInINTMSKk_MSPInINTMSKk
 /// Description  : cb_MSPInINTMSKk_MSPInINTMSKk
 ///*********************************
void MSPI000_INT_FACTOR_Func::cb_MSPInINTMSKk_MSPInINTMSKk(RegCBstr str)
{//{{{
 //chid -> i
   if(*MSPInINTMSKk[str.channel] != str.pre_data)
   {
       if(str.channel==0)
       {
           mpAgentController->NotifyEventOutputINT(emNumTX);
       }
       else if(str.channel==1)
       {
           mpAgentController->NotifyEventOutputINT(emNumRX);
       }
       else if(str.channel==2)
       {
           mpAgentController->NotifyEventOutputINT(emNumFE);
       }
       else
       {
           mpAgentController->NotifyEventOutputINT(emNumERR);
       }
   }
}//}}}
 ///*********************************
 /// Function     : SetMSPInINTFk
 /// Description  : SetMSPInINTFk
 ///*********************************
void MSPI000_INT_FACTOR_Func::SetMSPInINTFk(unsigned int chid, unsigned int index)
{
    *MSPInINTFk[chid] = (*MSPInINTFk[chid] | (0x1<<index));
}
 // vim600: set foldmethod=marker :
