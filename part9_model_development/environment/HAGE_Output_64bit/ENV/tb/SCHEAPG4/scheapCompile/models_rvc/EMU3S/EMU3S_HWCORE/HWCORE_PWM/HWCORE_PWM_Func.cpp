// -----------------------------------------------------------------------------
// $Id: HWCORE_PWM_Func.cpp,v 1.7 2020/11/17 06:54:29 ducla Exp $
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
// Description: Copyright of HWCORE_PWM_Func.cpp
// Ref: {HWCORE_PWM_UD_Copyright_001}
#include "HWCORE_PWM_Func.h"
#include "HWCORE_PWM_AgentController.h"
#ifndef func_re_printf
#define func_re_printf mpAgentController->get_fileline(__FILE__, __LINE__); mpAgentController->_re_printf
#endif//func_re_printf
//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
///*********************************
/// Function     : HWCORE_PWM_Func
/// Description  : Constructor HWCORE_PWM_Func
///*********************************
HWCORE_PWM_Func::HWCORE_PWM_Func(std::string name, HWCORE_PWM_AgentController* AgentController) :
    Chwcore_pwm_regif(name, 32)
{//{{{
    if (AgentController == NULL) {
        return;
   }
   mpAgentController = AgentController;
   mpAgentController->CommandInit(name);
   mModuleName = name;
   Chwcore_pwm_regif::set_instance_name(name);
   // Initialize
   Initialize();
   
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PWMCTR",(PWMCTR)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PWMDT",(PWMDT)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VDCRCT",(VDCRCT)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VQCRCT",(VQCRCT)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VEL",(VEL)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VELSFT",(VELSFT)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("DECVELG",(DECVELG)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("DECFLUX",(DECFLUX)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("DECLD",(DECLD)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("DECLQ",(DECLQ)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VD2MAX",(VD2MAX)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VQ2MAX",(VQ2MAX)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VD2",(VD2)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VQ2",(VQ2)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("GTHT",(GTHT)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PHI",(PHI)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("THTFORESFT",(THTFORESFT)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("THTEPWM",(THTEPWM)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("THTVSFT",(THTVSFT)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VDQSFT",(VDQSFT)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("SR23",(SR23)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VU",(VU)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VV",(VV)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VW",(VW)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("TMAX",(TMAX)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VU0",(VU0)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VV0",(VV0)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VW0",(VW0)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PWMK1",(PWMK1)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VOLV",(VOLV)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VU1",(VU1)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VV1",(VV1)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VW1",(VW1)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VUOFS",(VUOFS)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VVOFS",(VVOFS)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VWOFS",(VWOFS)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VU2",(VU2)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VV2",(VV2)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VW2",(VW2)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("DTUL",(DTUL)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("DTLL",(DTLL)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VUFIX",(VUFIX)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VVFIX",(VVFIX)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VWFIX",(VWFIX)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PWMK2",(PWMK2)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("DTT",(DTT)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("CARR",(CARR)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("CARRBUF",(CARRBUF)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PWMU0",(PWMU0)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PWMV0",(PWMV0)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PWMW0",(PWMW0)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("DTOTH",(DTOTH)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("DTOPV",(DTOPV)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("DTONV",(DTONV)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PWMUDT",(PWMUDT)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PWMVDT",(PWMVDT)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PWMWDT",(PWMWDT)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PWMUL",(PWMUL)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PWMLL",(PWMLL)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PWMUIP",(PWMUIP)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PWMVIP",(PWMVIP)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PWMWIP",(PWMWIP)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PWMU",(PWMU)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PWMV",(PWMV)));
   mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PWMW",(PWMW)));

}//}}}
 ///*********************************
 /// Function     : ~HWCORE_PWM_Func
 /// Description  : Destructor HWCORE_PWM_Func
 ///*********************************
HWCORE_PWM_Func::~HWCORE_PWM_Func()
{//{{{
    mRegisterMap.clear();
}//}}}
 ///*********************************
 /// Function     : Initialize
 /// Description  : Initialize variables
 ///*********************************
void HWCORE_PWM_Func::Initialize(void)
{//{{{
    mPWMCTR_buf_value = 0x0;
    mVDCRCT_buf_value = 0x0;
    mVQCRCT_buf_value = 0x0;
    mDECVELG_buf_value = 0x0;
    mDECFLUX_buf_value = 0x0;
    mDECLD_buf_value = 0x0;
    mDECLQ_buf_value = 0x0;
    mPHI_buf_value = 0x0;
    mGTHT_buf_value = 0x0;
    mTHTFORESFT_buf_value = 0x0;
    mTHTVSFT_buf_value = 0x0;
    mVDQSFT_buf_value = 0x0;
    mTMAX_buf_value = 0x0;
    mVOLV_buf_value = 0x0;
    mVUOFS_buf_value = 0x0;
    mVVOFS_buf_value = 0x0;
    mVWOFS_buf_value = 0x0;
    mCARR_buf_value = 0x0;
}//}}}

 /// Process reset function
 ///*********************************
 /// Function     : EnableReset
 /// Description  : Enable Reset
 ///*********************************
void HWCORE_PWM_Func::EnableReset(const bool is_active)
{//{{{
   if (is_active) {
      Initialize();
   }
   Chwcore_pwm_regif::EnableReset(is_active);
}//}}}

 /// Process reset function
 ///*********************************
 /// Function     : EnableReset
 /// Description  : Enable Reset
 ///*********************************
void HWCORE_PWM_Func::EnableResetRegister(const bool is_active)
{//{{{
   Chwcore_pwm_regif::EnableReset(is_active);
}//}}}

 /// Handler for register IF
 ///*********************************
 /// Function     : RegisterHandler 
 /// Description  : Register Handler
 ///*********************************
void HWCORE_PWM_Func::RegisterHandler(const std::vector<std::string> &args)
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
            msg_return = Chwcore_pwm_regif::reg_handle_command(cmd);
            if (msg_return != "") {
               SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
            cmd.clear();
            reg_p = next_reg_object();
         }
      }
      else {//Set handle command to Chwcore_pwm_regif class
         msg_return = Chwcore_pwm_regif::reg_handle_command(args);
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
// Ref: {HWCORE_PWM_UD_Flow_004, HWCORE_PWM_UD_Direction_031, HWCORE_PWM_UD_ATT1_005}
void HWCORE_PWM_Func::read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
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
// Ref: {HWCORE_PWM_UD_Flow_005, HWCORE_PWM_UD_Direction_032}
void HWCORE_PWM_Func::write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
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
///// Function     : cb_PWMCTR_PWMOP
///// Description  : callback PWMCTR_PWMOP
/////*********************************
// Description: cb_PWMCTR_PWMOP
// Ref: {HWCORE_PWM_UD_Flow_009, HWCORE_PWM_UD_Direction_060, HWCORE_PWM_UD_Direction_061, HWCORE_PWM_UD_Direction_062}
void HWCORE_PWM_Func::cb_PWMCTR_PWMOP(RegCBstr str)
{//{{{
    unsigned char pwmop = (unsigned char)(*PWMCTR)["PWMOP"];
    unsigned char decidq = (unsigned char)(*PWMCTR)["DECIDQ"];
    unsigned char flininip = (unsigned char)(*PWMCTR)["FLININIP"];
    unsigned char setharm = (unsigned char)(*PWMCTR)["SETHARM"];
    bool vdqsel = (bool)(*PWMCTR)["VDQSEL"];
    unsigned int pwmctr = (unsigned int)(str.pre_data);
    
    if (pwmop == 3) {
        func_re_printf("warning", "The config value 0x%02x to PWMCTR.PWMOP is prohibited. \n", pwmop);
        (*PWMCTR)["PWMOP"] = (pwmctr >> 13) & 0x3;;
    }

    if (decidq == 3) {
        func_re_printf("warning", "The config value 0x%02x to PWMCTR.DECIDQ is prohibited. \n", decidq);
        (*PWMCTR)["DECIDQ"] = (pwmctr >> 10) & 0x3;
    }

    if (flininip == 3) {
        func_re_printf("warning", "The config value 0x%02x to PWMCTR.FLININIP is prohibited. \n", flininip);
        (*PWMCTR)["FLININIP"] = (pwmctr >> 4) & 0x3;
    }
    
    if (setharm == 7 && vdqsel == false) {
        func_re_printf("warning", "The config value 0x%02x to PWMCTR.SETHARM when PWMCTR.VDQSEL = 0 is prohibited. \n", setharm);
        //(*PWMCTR)["SETHARM"] = (pwmctr >> 1) & 0x7;
    }
}//}}}
/////*********************************
///// Function     : cb_CTRINMD_INSTCTR
///// Description  : callback CTRINMD_INSTCTR
/////*********************************
// Description: cb_CTRINMD_INSTCTR
// Ref: {HWCORE_PWM_UD_Flow_008}
void HWCORE_PWM_Func::cb_PWMDT_PWMDT(RegCBstr str)
{//{{{
   unsigned char value = (str.data) & 0x1;
   if (value == 1){
      mpAgentController->Handle_PWMDT();
   }
}//}}}

/////*********************************
///// Function     : GetRegVal
///// Description  : Get Register Value with unsigned int Type
/////*********************************
unsigned int HWCORE_PWM_Func::GetRegVal(std::string RegName)
{//{{{
    return ((unsigned int)(*(mRegisterMap[RegName.c_str()])));
}//}}}

 /////*********************************
 ///// Function     : GetRegVal
 ///// Description  : Get bit value
 /////*********************************
unsigned int HWCORE_PWM_Func::GetRegVal(std::string RegName, std::string BitName)
{//{{{
    return ((unsigned int)((*(mRegisterMap[RegName.c_str()]))[BitName.c_str()]));
}//}}}

 /////*********************************
 ///// Function     : SetRegVal
 ///// Description  : Set Register Value with unsigned int Type
 /////*********************************
void HWCORE_PWM_Func::SetRegVal(std::string RegName, unsigned int val)
{//{{{
    (*(mRegisterMap[RegName.c_str()])) = val;
}//}}}
 // vim600: set foldmethod=marker :
