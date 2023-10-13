// ---------------------------------------------------------------------
// $Id: MSPITG_Func.cpp,v 1.2 2020/02/05 05:30:17 ducla Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of MSPITG_Func.cpp
// Ref: {MSPITG_DD_Copyright_001}
#include "MSPITG_Func.h"
#include "MSPITG_AgentController.h"
//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
///*********************************
/// Function     : MSPITG_Func
/// Description  : Constructor MSPITG_Func
///*********************************
MSPITG_Func::MSPITG_Func(std::string name, MSPITG_AgentController* AgentController) :
   Cmspitg_regif(name, 32)
   , BusSlaveFuncIf()
{//{{{
   sc_assert(AgentController != NULL);
   mpAgentController = AgentController;
   mpAgentController->CommandInit(name);
   mModuleName = name;
   Cmspitg_regif::set_instance_name(name);
   // Initialize
   Initialize();
}//}}}
 ///*********************************
 /// Function     : ~MSPITG_Func
 /// Description  : Destructor MSPITG_Func
 ///*********************************
MSPITG_Func::~MSPITG_Func()
{//{{{

}//}}}
 ////////////////////////////////////////////////////////////
 ///*********************************
 /// Function     : Initialize
 /// Description  : Initialize variables
 ///*********************************
void MSPITG_Func::Initialize(void)
{//{{{

}//}}}
 /// Process reset function
 ///*********************************
 /// Function     : EnableReset
 /// Description  : Enable Reset
 ///*********************************
void MSPITG_Func::EnableReset(const bool is_active)
{//{{{
   if (is_active) {
      Initialize();
   }
   Cmspitg_regif::EnableReset(is_active);
}//}}}
 /// Handler for register IF
 ///*********************************
 /// Function     : RegisterHandler 
 /// Description  : Register Handler
 ///*********************************
void MSPITG_Func::RegisterHandler(const std::vector<std::string> &args)
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
            msg_return = Cmspitg_regif::reg_handle_command(cmd);
            if (msg_return != "") {
               SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
            cmd.clear();
            reg_p = next_reg_object();
         }
      }
      else {//Set handle command to Cmspitg_regif class
         msg_return = Cmspitg_regif::reg_handle_command(args);
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
// Ref: {MSPITG_DD_Flow_004, MSPITG_DD_Direction_031, MSPITG_DD_ATT1_005}
void MSPITG_Func::read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
        // Zero clock or reset active
    if ( (!mpAgentController->CheckClockPeriod()) || (mpAgentController->GetResetStatus())) {
        if (pExtInfo != NULL) {
            if (!(debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID))) {
                re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
                return;
            }
        }
        else {
            re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
            return;
        }
    }

   if (!debug) {
      reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
   }
   else {
      reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
   }
}//}}}
 ///*********************************
 /// Function     : write 
 /// Description  : Write API (target socket)
 ///*********************************
// Description: write register
// Ref: {MSPITG_DD_Flow_005, MSPITG_DD_Direction_030}
void MSPITG_Func::write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    if (!mpAgentController->CheckClockPeriod()) {
        // Software debugger
        if ((pExtInfo != NULL)) {
            if ( !( debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID)) ) {
                re_printf("warning", "Cannot write register when clock is 0.\n");
                return;
            }
        }
        else {
            re_printf("warning", "Cannot write register when clock is 0.\n");
            return;
        }
    }

   if (!debug) {
      reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
   }
   else {
      reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
   }

}//}}}
/////*********************************
///// Function     : cb_MSPITGCTLn_TRGSEL0
///// Description  : cb_MSPITGCTLn_TRGSEL0
/////*********************************
// Description: cb_MSPITGCTLn_TRGSEL0
// Ref: {MSPITG_DD_Flow_008}
void MSPITG_Func::cb_MSPITGCTLn_TRGSEL0(RegCBstr str)
{
    mpAgentController->UpdateMSPITGCTLValue((unsigned int)(str.channel),(unsigned int)(*MSPITGCTLn[str.channel]));
}
/////*********************************
///// Function     : cb_MSPITGDMAALT_DMAAS02
///// Description  : cb_MSPITGDMAALT_DMAAS02
/////*********************************
// Description: cb_MSPITGDMAALT_DMAAS02
// Ref: {MSPITG_DD_Flow_009}
void MSPITG_Func::cb_MSPITGDMAALT_DMAAS02(RegCBstr str)
{
    mpAgentController->UpdateMSPITGDMAALTValue( (unsigned int)(*MSPITGDMAALT));
}
/////*********************************
///// Function     : cb_MSPITGDTSALT_DTSAS06
///// Description  : cb_MSPITGDTSALT_DTSAS06
/////*********************************
// Description: cb_MSPITGDTSALT_DTSAS06
// Ref: {MSPITG_DD_Flow_010}
void MSPITG_Func::cb_MSPITGDTSALT_DTSAS06(RegCBstr str)
{
    mpAgentController->UpdateMSPITGDTSALTValue((unsigned int)(*MSPITGDTSALT));
}

 // vim600: set foldmethod=marker :
