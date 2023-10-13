// ---------------------------------------------------------------------
// $Id: G4_PEG_Wrapper.cpp,v 1.12 2020/01/14 02:39:30 nhutminhdang Exp $
//
// Copyright(c) 2016-2020 Renesas Electronics Corporation
// Copyright(c) 2016-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "G4_PEG_Wrapper.h"
#include "G4_PEG.h"
#include "G4_PEGCAP.h"
#include <stdio.h>  

/*********************************
// Function     : G4_PEG_Wrapper
// Description  : Constructor of G4_PEG_Wrapper class
// Parameter    : 
//      name               module name
//      configfile_name    name of config file
//      rLatency           read latency
//      wLatency           write latency
//      PEG_ID             ID of PE
// Return value : None
**********************************/
G4_PEG_Wrapper::G4_PEG_Wrapper (sc_module_name name, const char *configfile_name, unsigned int rLatency, unsigned int wLatency, unsigned int PEG_ID)
                                :tsp("tsp")
                                ,ts_s("ts_s")
                                ,ts_m("ts_m")
                                ,tsv("tsv")
                                ,is("is")
                                ,PCLK("PCLK")
                                ,presetz("presetz")
                                ,ERR_INFO("ERR_INFO")
                                ,PEG_ERR_M("PEG_ERR_M")
                                ,GRDERR_S("GRDERR_S")
                                ,GRDERR_M("GRDERR_M")
                                ,rvc_common_model()
{//{{{
     /// Initialize variables
    mPCLKPeriod   = 0;
    mPCLKFreq     = 0;
    mPCLKOrgFreq  = 0;
    mPCLKFreqUnit = "Hz";
    
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    mIsResetCmdReceive     = false;
    mIsResetCmdActive      = false;
    mIsResetHardActive     = false;
    mResetCmdPeriod        = 0;
    mDumpInterrupt         = false;
    mEnableRegisterMessage = true;
    mPEID                  = PEG_ID;
    mIspresetzInit         = true;
     
    if (mPEID > 7) 
    {
        re_printf("warning","PEID (%d) is wrong, it is set to the default value (0) automatically.\n", mPEID);
        mPEID = 0;
    } else {
        re_printf("info","PEID is set to %d\n", mPEID);
    }

    GRDERR_M.initialize(false);
    GRDERR_S.initialize(false);
    
    mPEG_ERR_M.initialize();
    PEG_ERR_M.initialize(mPEG_ERR_M);
    PEG_ERR_S_sig = mPEG_ERR_M;
     
    std::string  mKind = "G4MH";
    if(configfile_name != NULL) {
        ifstream  configfile(configfile_name);
        char      word[512];
        char      seps[] = " ,=();\t\n\r";
        char      *token;
        
        while (!configfile.eof()) {
            configfile.getline(word, 512, '\n');
            token = strtok(word, seps);
            /// Skip commented line
            if ((token == NULL) || (strncmp(token, "//", 2) == 0)) {
                continue;
            }
            /// Detect End marker
            if (strcmp(token, "[END]") == 0) {
                break;
            }
        
            if ( 0 == strcmp(token, "[G4CPU_PE_TYPE]") ) {
                token = strtok( 0, seps );
                token = strtok( 0, seps );
                if (token != NULL) 
                {
                    mKind = token;
                } 
                continue;
            }
        }
        
        if(strcmp(mKind.c_str(), "G4MH_V20") == 0x0) {
            mKind = "U2A";
            mChannelNum = 8;
            re_printf("info", "The current platform is U2A\n");
        } else {
            mKind = "E2x";
            mChannelNum = 4;
            re_printf("info", "The current platform is E2x\n");
        }
    } 

    /// Connection in G4_PEG interface
    mG4_PEG =  new G4_PEG("g4_peg", mKind, 0, 0, mPEID, mChannelNum);
    sc_assert(mG4_PEG != NULL);
    mG4_PEG->resetPort(presetz);
    mG4_PEG->PCLK(PCLK);
    mG4_PEG->PEG_ERR_S(PEG_ERR_S_sig);
    mG4_PEG->PEG_ERR_M(PEG_ERR_M);
    tsp(*(mG4_PEG->tsp));            
    tsv(*(mG4_PEG->tsv));            
    (*(mG4_PEG->is))(is);            

    /// Connection in G4_PEGCAP_S interface
    mG4_PEGCAP_S =  new G4_PEGCAP("g4_pegcap_s", mKind, 0, 0);
    sc_assert(mG4_PEGCAP_S != NULL);
    mG4_PEGCAP_S->PCLK(PCLK);
    mG4_PEGCAP_S->presetz(presetz);
    mG4_PEGCAP_S->GRDERR(GRDERR_S);
    mG4_PEGCAP_S->ERR_INFO(PEG_ERR_S_sig);
    ts_s(*(mG4_PEGCAP_S->ts));           

    /// Connection in G4_PEGCAP_M interface
    mG4_PEGCAP_M =  new G4_PEGCAP("g4_pegcap_m", mKind, 0, 0);
    sc_assert(mG4_PEGCAP_M != NULL);
    mG4_PEGCAP_M->PCLK(PCLK);
    mG4_PEGCAP_M->presetz(presetz);
    mG4_PEGCAP_M->GRDERR(GRDERR_M);
    mG4_PEGCAP_M->ERR_INFO(ERR_INFO);
    ts_m(*(mG4_PEGCAP_M->ts));         
     
    /// SC_METHOD
    SC_METHOD(MonitoPCLKMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(MonitorpresetzMethod);
    sensitive << presetz;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;
}//}}}

/*********************************
// Function     : ~G4_PEG_Wrapper
// Description  : Destructor of G4_PEG_Wrapper class
// Parameter    : None
// Return value : None
**********************************/
G4_PEG_Wrapper::~G4_PEG_Wrapper (void)
{//{{{
    delete mG4_PEG;
    delete mG4_PEGCAP_S;
    delete mG4_PEGCAP_M;
}//}}}

/*********************************
// Function     : RegisterHandler
// Description  : Handle commands access to register.
// Parameter    : 
//     module_name   name of block register 
//     cmd           command access to register
// Return value : None
**********************************/
void G4_PEG_Wrapper::RegisterHandler (const std::string module_name, const std::vector<std::string> cmd)
{//{{{
    if(module_name == "G4_PEG") {
        mG4_PEG->RegisterHandler(cmd);
    }
    else if(module_name == "G4_PEGCAP_M") {
        mG4_PEGCAP_M->RegisterHandler(cmd);
    }
    else if(module_name == "G4_PEGCAP_S") {
        mG4_PEGCAP_S->RegisterHandler(cmd);
    }
    else {
        re_printf("warning","Module name (%s) is invalid\n", module_name.c_str());
    }
}//}}}

/*********************************
// Function     : EnableReset
// Description  : Enable/ Disable reset
// Parameter    : 
//    is_active   true: reset, false: not reset
// Return value : None
**********************************/
void G4_PEG_Wrapper::EnableReset (const bool is_active)
{//{{{
    mG4_PEG->EnableReset(is_active);
    mG4_PEGCAP_S->EnableReset(is_active);
    mG4_PEGCAP_M->EnableReset(is_active);
}//}}}

/*********************************
// Function     : HandleResetCmdMethod
// Description  : Process reset function when reset command is active
// Parameter    : None
// Return value : None
**********************************/
void G4_PEG_Wrapper::HandleResetCmdMethod (void)
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of presetz.\n");
    this->EnableReset(mIsResetCmdActive);
    // Update reset staus
    UpdateResetStatus();
    // Cancel after persiod is over
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

/*********************************
// Function     : CancelResetCmdMethod
// Description  : Cancel reset function when reset command is active
// Parameter    : None
// Return value : None
**********************************/
void G4_PEG_Wrapper::CancelResetCmdMethod (void)
{//{{{
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of presetz is over.\n");
    this->EnableReset(mIsResetCmdActive);
    // Update reset staus
    UpdateResetStatus();
}//}}}

/*********************************
// Function     : MonitorpresetzMethod
// Description  : Handle presetz signal
// Parameter    : None
// Return value : None
**********************************/
void G4_PEG_Wrapper::MonitorpresetzMethod (void)
{//{
    bool reset = presetz.read();
    if (reset == vpcl::emResetActive){
        mIspresetzInit = false;
        mIsResetHardActive = true;
        
        /// Cancel AssertReset command when reset port is active
        mResetCmdEvent.cancel();
        mResetCmdCancelEvent.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
        
        re_printf("info", "The reset port presetz is asserted.\n");
        EnableReset(true);
    }else {
        if (mIspresetzInit){
            mIspresetzInit = false;
        }else {
            mIsResetHardActive = false;
            re_printf("info", "The reset port presetz is de-asserted.\n");
            EnableReset(false);
        }
    }

    // Update reset staus
    UpdateResetStatus();
}//}}}

/*********************************
// Function     : HandlePCLKSignalMethod
// Description  : Handle PCLK clock
// Parameter    : None
// Return value : None
**********************************/
void G4_PEG_Wrapper::MonitoPCLKMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

/*********************************
// Function     : PCLKUpdate
// Description  : Update PCLK clock for other modules
// Parameter    : None
// Return value : None
**********************************/
void G4_PEG_Wrapper::PCLKUpdate (void)
{//{{{
    if(mPCLKPeriod == 0)
    {
        mG4_PEG->CancelEvents();
        mG4_PEGCAP_S->CancelEvents();
        mG4_PEGCAP_M->CancelEvents();
    } 
    double current_time = sc_time_stamp().to_double();
    mG4_PEG->SetCLKPeriod(mPCLKPeriod, current_time);
    mG4_PEGCAP_S->SetCLKPeriod(mPCLKPeriod, current_time);
    mG4_PEGCAP_M->SetCLKPeriod(mPCLKPeriod, current_time);
}//}}}

/*********************************
// Function     : UpdateResetStatus
// Description  : Update reset status for other modules
// Parameter    : None
// Return value : None
**********************************/
void G4_PEG_Wrapper::UpdateResetStatus(void)
{//{{{
    mG4_PEG->SetResetStatus(mIsResetCmdActive, mIsResetHardActive);
    mG4_PEGCAP_S->SetResetStatus(mIsResetCmdActive, mIsResetHardActive);
    mG4_PEGCAP_M->SetResetStatus(mIsResetCmdActive, mIsResetHardActive);
}//}}}

/*********************************
// Function     : DumpInterrupt
// Description  : Enable/ Disable dumping interrupt information
// Parameter    : 
//     is_enable  true: dump info; false: not dump info
// Return value : None
**********************************/
void G4_PEG_Wrapper::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
            mG4_PEGCAP_M->DumpInterrupt(mDumpInterrupt);
            mG4_PEGCAP_S->DumpInterrupt(mDumpInterrupt);
            mG4_PEG->DumpInterrupt(mDumpInterrupt);
        } else if (word_vector[0] == "false") {
                mDumpInterrupt = false;
                mG4_PEGCAP_M->DumpInterrupt(mDumpInterrupt);
                mG4_PEGCAP_S->DumpInterrupt(mDumpInterrupt);
                mG4_PEG->DumpInterrupt(mDumpInterrupt);
            } else {
                re_printf("warning","Invalid argument: G4_PEG_Wrapper_DumpInterrupt %s\n", is_enable.c_str());
            }
    } else if (word_vector.size() == 0){
            if (mDumpInterrupt) {
                re_printf("info","G4_PEG_Wrapper_DumpInterrupt %s\n", "true");
            } else {
                re_printf("info","G4_PEG_Wrapper_DumpInterrupt %s\n", "false");
            }
        } else {
            re_printf("warning","Invalid argument: G4_PEG_Wrapper_DumpInterrupt %s\n", is_enable.c_str());
        }
}//}}}

/*********************************
// Function     : DumpRegisterRW
// Description  : Enable/ Disable dumping message when users access registersn
// Parameter    : 
//     is_enable  true: dump info; false: not dump info
// Return value : None
**********************************/
void G4_PEG_Wrapper::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mG4_PEG->RegisterHandler(cmd);
    mG4_PEGCAP_S->RegisterHandler(cmd);
    mG4_PEGCAP_M->RegisterHandler(cmd);
}//}}}

/*********************************
// Function     : SetMessageLevel
// Description  : Set message level (fatal, error, warning, info)
// Parameter    : 
//     msg_lv     input argument for SetMessageLevel command
// Return value : None
**********************************/
void G4_PEG_Wrapper::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }

    if (true == mEnableRegisterMessage) {
        /// Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
         SeparateString(cmd, msg_lv);
        mG4_PEG->RegisterHandler(cmd);
        mG4_PEGCAP_S->RegisterHandler(cmd);
        mG4_PEGCAP_M->RegisterHandler(cmd);

        /// Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
         SeparateString(cmd, msg_lv);
        mG4_PEG->RegisterHandler(cmd);
        mG4_PEGCAP_S->RegisterHandler(cmd);
        mG4_PEGCAP_M->RegisterHandler(cmd);
    } else {
        //Set message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");
        mG4_PEG->RegisterHandler(cmd);
        mG4_PEGCAP_S->RegisterHandler(cmd);
        mG4_PEGCAP_M->RegisterHandler(cmd);

        //Set message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");
        mG4_PEG->RegisterHandler(cmd);
        mG4_PEGCAP_S->RegisterHandler(cmd);
        mG4_PEGCAP_M->RegisterHandler(cmd);
    }
}//}}}

/*********************************
// Function     : Help
// Description  : Dump help message of all parameters or commands
// Parameter    : 
//     type       parameters/ commands
// Return value : None
**********************************/
void G4_PEG_Wrapper::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
     SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"G4_PEG_Wrapper_MessageLevel (fatal|error|warning|info)                       Set debug message level (Default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"G4_PEG_Wrapper_DumpRegisterRW (true/false)                                   Enable/disable dumping access register (Default: false).");
            SC_REPORT_INFO(this->basename(),"G4_PEG_Wrapper_DumpInterrupt (true/false)                                    Enable/disable dumping interrupt information (Default: false).");
            SC_REPORT_INFO(this->basename(),"G4_PEG_WrapperEnableRegisterMessage (\"G4_PEG_Wrapper instance\", \"true/false\")             Enable/disable info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"G4_PEG_Wrapper_AssertReset (reset_name, start_time, period)                  Assert and de-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"G4_PEG_Wrapper_SetCLKFreq (clock_name, freq, unit)                           Set clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"G4_PEG_Wrapper_GetCLKFreq (clock_name)                                       Get clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"G4_PEG_Wrapper_ForceRegister (module_name, reg_name, reg_value)              Force register of PEG with setting value.");
            SC_REPORT_INFO(this->basename(),"G4_PEG_Wrapper_ReleaseRegister (module_name, reg_name)                       Release register of PEG from force value.");
            SC_REPORT_INFO(this->basename(),"G4_PEG_Wrapper_WriteRegister (module_name, reg_name, reg_value)              Write a value to a register of PEG.");
            SC_REPORT_INFO(this->basename(),"G4_PEG_Wrapper_ReadRegister (module_name, reg_name)                          Read a value from a register of PEG.");
            SC_REPORT_INFO(this->basename(),"G4_PEG_Wrapper_ListRegister ()                                               Dump name of registers in model.");
            SC_REPORT_INFO(this->basename(),"G4_PEG_Wrapper_SetPeId (peid)                                                Set PEID.");
        } else {
            re_printf("warning","The name (%s) of G4_PEG_Wrapper_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of G4_PEG_Wrapper_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

/*********************************
// Function     : ListRegister
// Description  : List registers name of whole model 
// Parameter    : None
// Return value : None
**********************************/
void G4_PEG_Wrapper::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mG4_PEG->RegisterHandler(cmd);
    mG4_PEGCAP_S->RegisterHandler(cmd);
    mG4_PEGCAP_M->RegisterHandler(cmd);
}//}}}

/*********************************
// Function     : ReadRegister
// Description  : Read value of register by software
// Parameter    : 
//    module_id   name of module of register (G4_PEG/ G4_PEGCAP_S/ G4_PEGCAP_M)
//    reg_name    name of register
// Return value : None
**********************************/
void G4_PEG_Wrapper::ReadRegister (const std::string module_id, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            RegisterHandler(module_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

/*********************************
// Function     : WriteRegister
// Description  : Write value to registers by software
// Parameter    : 
//    module_id   name of module of register (G4_PEG/ G4_PEGCAP_S/ G4_PEGCAP_M)
//    reg_name    name of register
//    reg_value   value need to write to register
// Return value : None
**********************************/
void G4_PEG_Wrapper::WriteRegister (const std::string module_id, const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str << "0x" << std::hex <<reg_value;
            cmd.push_back(reg_value_str.str());
            RegisterHandler(module_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

/*********************************
// Function     : ReleaseRegister
// Description  : Release forced value after forcing registers
// Parameter    : 
//    module_id   name of module of register (G4_PEG/ G4_PEGCAP_S/ G4_PEGCAP_M)
//    reg_name    name of register
// Return value : None
**********************************/
void G4_PEG_Wrapper::ReleaseRegister (const std::string module_id, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            RegisterHandler(module_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

/*********************************
// Function     : ForceRegister
// Description  : Force value to register
// Parameter    : 
//    module_id   name of module of register (G4_PEG/ G4_PEGCAP_S/ G4_PEGCAP_M)
//    reg_name    name of register
//    reg_value   value need to force to register
// Return value : None
**********************************/
void G4_PEG_Wrapper::ForceRegister (const std::string module_id, const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("force");
            std::ostringstream reg_value_str;
            reg_value_str << "0x" << std::hex <<reg_value;
            cmd.push_back(reg_value_str.str());
            RegisterHandler(module_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

/*********************************
// Function     : GetCLKFreq
// Description  : Get clock value
// Parameter    : 
//    clock_name   name of clock (PCLK)
// Return value : None
**********************************/
void G4_PEG_Wrapper::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/*********************************
// Function     : SetCLKFreq
// Description  : Set clock value and clock unit
// Parameter    : 
//    clock_name   name of clock (PCLK)
//    freq         new value frequency 
//    unit         new unit of frequency
// Return value : None
**********************************/
void G4_PEG_Wrapper::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mPCLKPeriod = 0;
            }
            PCLKUpdate();
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/*********************************
// Function     : AssertReset
// Description  : Assert reset by software
// Parameter    : 
//    reset_name   name of reset (presetz)
//    start_time   after <start_time> model will be rese
//    period       period of reset
// Return value : None
**********************************/
void G4_PEG_Wrapper::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "presetz") 
    {
        if ((!mIsResetHardActive)&&(!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (presetz) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of presetz is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be presetz\n",reset_name.c_str());
    }
}//}}}

/*********************************
// Function     : SetPeId
// Description  : Set PE ID for G4_PEG module
// Parameter    : 
//       peid     new value for PE ID
// Return value : None
**********************************/
void G4_PEG_Wrapper::SetPeId (unsigned int peid) 
{//{{{
    if (peid > 7) 
    {
        re_printf("info","PEID (%d) is wrong, it is set to the default value (0) automatically.\n", mPEID);
        mPEID = 0;
    } else {
        mPEID = peid;
        mG4_PEG->SetPeId(peid);
    }
}//}}}

/*********************************
// Function     : EnableRegisterMessage
// Description  : Enable/disable info/warning/error message of register IF
// Parameter    : 
//    is_enable     true: enable; false: disable
// Return value : None
**********************************/
void G4_PEG_Wrapper::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
     SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: G4_PEG_Wrapper_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
                if (mEnableRegisterMessage) {
                    re_printf("info","G4_PEG_Wrapper_EnableRegisterMessage %s\n", "true");
                } else {
                    re_printf("info","G4_PEG_Wrapper_EnableRegisterMessage %s\n", "false");
                }
    } else {
        re_printf("warning","Invalid argument: G4_PEG_Wrapper_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}
// vim600: set foldmethod=marker :
