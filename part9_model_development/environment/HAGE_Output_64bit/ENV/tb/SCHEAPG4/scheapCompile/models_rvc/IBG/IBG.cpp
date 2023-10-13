// ---------------------------------------------------------------------
// $Id: IBG.cpp,v 1.13 2019/07/15 08:43:08 nhuvo Exp $
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

#include "IBG.h"
#include "IBG_Func.h"

/// utility libraries
#include <stdint.h>
#include <string>
using std::string;

//##############################################################################
// PART 1: elaborate model and
//##############################################################################

//------------------------------------------------------------------------------
/**@brief public constructor
* @details
* Detailed steps as follows:
*/
IBG::IBG(sc_module_name iname, uint8_t iCHAN_NUM, uint32_t rLatency, uint32_t wLatency, SIM_MODE_T simmode)
    : IbgBusBridgeModule(iname)
    ,rvc_common_model()
    , mLineNum (0)
    , CHAN_NUM (iCHAN_NUM)
    //### modeling clock sources ###
    , PCLK ("PCLK")
    , mPCLKPeriod  (0)
    , mPCLKOrgFreq (0)
    , mPCLKFreq    (0)
    , mPCLKFreqUnit("Hz")
    , mPCLKSyncTPoint (0)
    //### modeling reset sources ###
    , PRESETn            ("PRESETn")
    , mIsResetHardActive (false)
    , mIsResetCmdActive  (false)
    , mIsResetCmdReceive (false)
    , mResetCmdPeriod    (0)
    , mResetCmdEvent       ("resetPortCmdEvent")
    , mResetCmdCancelEvent ("resetPortCmdCancelEvent")
    //### time resolution ###
    , mTimeResolutionValue (1)
    , mTimeResolutionUnit  (SC_NS)
    //### constructing the model ###
    , pFuncModel (NULL)
    //### Python interface ###
    , mDumpInterrupt (false)
    , mEnableRegisterMessage (true)
    //### public interfaces ###
    , err_irq ("err_irq")
    // internal sub-modules, signals, variables
    , err_irq_val (false)
    , pBusGuard (NULL)
{//{{{
    if(iCHAN_NUM < CHANNEL_MIN_NUM || iCHAN_NUM > CHANNEL_MAX_NUM) {
        re_printf("warning", "This setting of channel number is prohibited. Channel number is set to default (1).\n");
        CHAN_NUM = 1;   
    }
    //-------------------------------------
    /// -# initialize constants
    // time resolution
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    //-------------------------------------
    /// -# instantiate input/output signals
    //Bind clock, reset signal and socket to Bus IF
    IbgBusSlave::setSlaveFreqPort32 (&PCLK);
    IbgBusSlave::setSlaveResetPort32(&PRESETn);
    IbgBusSlave::setTargetSocket32  ("ts");

    // BusBridgeSlaveBase
    IbgBusBridgeSlave::setSlaveFreqPort (&PCLK);
    IbgBusBridgeSlave::setSlaveResetPort(&resetPort);
    IbgBusBridgeSlave::setTargetSocket  ("tsp");
    // BusMasterBase
    IbgBusMaster::setMasterFreqPort32 (&PCLK);
    IbgBusMaster::setMasterResetPort32(&resetPort);
    IbgBusMaster::setInitiatorSocket32("isp");

    //-------------------------------------
    /// -# instantiate the function model
    ts  = IbgBusSlave::tSocket32[0];     //Alias for binding
    tsp = IbgBusBridgeSlave::tSocket[0]; //Alias for binding
    isp = IbgBusMaster::iSocket32[0];    //Alias for binding

    /// -# instantiate the function model
    pFuncModel = new IBG_Func ((string)iname, this, CHAN_NUM);
    sc_assert(pFuncModel != NULL);

    IbgBusSlave::mBusSlaveIf32[0]->setFuncModulePtr(pFuncModel);
    IbgBusSlave::mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    IbgBusSlave::mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    IbgBusSlave::mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    IbgBusSlave::mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    // Instantiate the function model
    pBusGuard = new IBG_CommonGuard (string(iname)                      // name
        ,this                               // Parent as IbgBusBridgeModule
        ,IbgBusBridgeSlave::mBusSlaveIf[0]  // SlaveIf
    ,IbgBusMaster::mBusMasterIf32[0]    // MasterIf
    ,simmode                            // simmode
        ,this                               // AgentController_ptr as IBG_CommonGuardAgentControllerIf
        );
    sc_assert(pBusGuard != NULL);
    // BusBridgeModuleBase
    IbgBusBridgeModule::mpCore = pBusGuard;
    // BusBridgeSlaveBase
    IbgBusBridgeSlave::mBusSlaveIf[0]->setFuncModulePtr((BusSlaveFuncIf *)pBusGuard);
    IbgBusBridgeSlave::mBusSlaveIf[0]->setBusProtocol(BUS_VCI);
    IbgBusBridgeSlave::mBusSlaveIf[0]->setEnableToCheckDataSize();
    // BusMasterBase
    IbgBusMaster::mBusMasterIf32[0]->setFuncModulePtr((BusMasterFuncIf *)pBusGuard);
    IbgBusMaster::mBusMasterIf32[0]->setBusProtocol(BUS_VCI);

    //-------------------------------------
    /// -# instantiate internal signals, sub-modules, variables
    err_irq.initialize(false);

    //-------------------------------------
    /// -# instantiate events and register processes
    // processes handle clocks
    SC_METHOD(PCLKMethod);
    dont_initialize();
    sensitive << PCLK;

    // processes handle reset sources
    SC_METHOD(resetPortMethod);
    dont_initialize();
    sensitive << resetPort;
    SC_METHOD(PRESETnMethod);
    dont_initialize();
    sensitive << PRESETn;
    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;
    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;

    // processes handle err_irq
    SC_METHOD(AssertErr_IrqMethod);
    dont_initialize();
    sensitive << mAssertErr_IrqEvent;
    SC_METHOD(DeassertErr_IrqMethod);
    dont_initialize();
    sensitive << mDeassertErr_IrqEvent;
    SC_METHOD(UpdateErr_IrqMethod);
    dont_initialize();
    sensitive << mUpdateErr_IrqEvent;

    //-------------------------------------
    /// -# initialize internal variables, signals, states
    Initialize();
}//}}}
//------------------------------------------------------------------------------
/**@brief public destructor
* @details
* Detailed steps as follows:
*/
IBG::~IBG() 
{//{{{
    //-------------------------------------
    /// -# destruct input/output signals

    //-------------------------------------
    /// -# destruct function model
    if (pFuncModel != NULL) delete pFuncModel;

    //-------------------------------------
    /// -# destruct internal signals, sub-modules, variables, events
    if (pBusGuard != NULL) delete pBusGuard;

    //-------------------------------------
    /// -# destruct configration constants

}//}}}
//##############################################################################
// PART 2: clock, reset and initialization
//##############################################################################

//------------------------------------------------------------------------------
/**@brief monitor clock
*/
void IBG::PCLKMethod() 
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq(PCLK.basename(), freq_value, "Hz");
}//}}}
//------------------------------------------------------------------------------
/**@brief Check if clock period value is greater than 0 or not
* @return true if the clock under check is greater than 0
*/
bool IBG::CheckClockPeriod (const std::string clock_name) 
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    if (clock_name == PCLK.basename()) {
        clock_period = mPCLKPeriod;
    } else {
        clock_period = 0;
    }
    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }
    return clock_status;
}//}}}
//------------------------------------------------------------------------------
/**@brief monitor resetPort port
*/
void IBG::resetPortMethod()
{//{{{
    bool reset = resetPort.read();
    if (reset == vpcl::emResetActive){
        mIsResetHardActive = true;
        re_printf("info", "The reset port %s is asserted.\n", resetPort.basename());
        EnableReset(true);
    }else{
        mIsResetHardActive = false;
        re_printf("info", "The reset port %s is de-asserted.\n", resetPort.basename());
        EnableReset(false);
    }
    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive) {
        mResetCmdEvent.cancel();
        mResetCmdCancelEvent.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief monitor PRESETn port
*/
void IBG::PRESETnMethod() 
{//{{{
    bool reset = PRESETn.read();   
    if (reset == vpcl::emResetActive){
        re_printf("info", "The PRESETn is asserted.\n");
        pFuncModel->EnablePRESETnReset(true);
    }
    else {
        re_printf("info", "The PRESETn is de-asserted.\n");
        pFuncModel->EnablePRESETnReset(false);
    }
}//}}}
//------------------------------------------------------------------------------
/** @brief Process reset function when reset command is active
*/
void IBG::HandleResetCmdMethod () 
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", resetPort.basename());
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}
//------------------------------------------------------------------------------
/** @brief Cancel reset function when reset command is active
*/
void IBG::CancelResetCmdMethod () 
{//{{{
    mIsResetCmdActive  = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of %s is over.\n", resetPort.basename());
    this->EnableReset(mIsResetCmdActive);
}//}}}
//------------------------------------------------------------------------------
/**@brief initialize outputs and internal variables, internal states,
*        internal signals
* @details
*   This function is called when the model is reset or the model is reinitialized
*   by command from CPU via register block
* Detailed steps as follows:
*/
void IBG::Initialize(void) 
{//{{{
    /// -# initialize all outputs
    err_irq_val = false;
    mUpdateErr_IrqEvent.notify(SC_ZERO_TIME);
}//}}}
//------------------------------------------------------------------------------
/**@brief cancel all scheduled events
* @details
*   This function is called when the model is reset
* Detailed steps as follows:
*/
void IBG::CancelEvents(void) 
{//{{{
    mAssertErr_IrqEvent.cancel();
    mDeassertErr_IrqEvent.cancel();
}//}}}
//------------------------------------------------------------------------------
/**@brief Reset model and its sub-instance if reset is active
* @param is_active
*/
void IBG::EnableReset(const bool isActive) 
{//{{{
    pFuncModel->EnableReset(isActive);
    Initialize();
    CancelEvents();
}//}}}
//##############################################################################
// Part 3: PythonIF
//##############################################################################
//------------------------------------------------------------------------------
/**@brief Assert and deassert reset signal
* @param reset_name std::string <reset_name>: name of reset signal ("presetn" or "rstn")
* @param start_time double <start_time>: the time until asserting reset signal from current time. The unit is "ns"
* @param period double <period>: the time from asserting reset signal to de-assert it. The unit is "ns"
*/
void IBG::AssertReset(const std::string reset_name, const double start_time, const double period) 
{//{{{
    if (reset_name == resetPort.basename()) {
        if ((!mIsResetHardActive) && (!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (%s) for %f ns after %f ns.\n", resetPort.basename(), period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        }
        else {
            re_printf("warning", "The software reset of %s is called in the reset operation of the model. So it is ignored.\n", resetPort.basename());
        }
    }
    else {
        re_printf("warning","The reset name (%s) is wrong. It should be %s.\n", reset_name.c_str(), resetPort.basename());
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief Set frequency value to these blocks
* @param clock_name std::string <clock_name>: name of clock signal
* @param freq sc_dt::uint64 <freq>: clock frequency
* @param unit std::string <unit>: frequency unit ("Hz", "KHz", "MHz" or "GHz")
*/
void IBG::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit) 
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == PCLK.basename()) {
            mPCLKOrgFreq = freq;
            ConvertClockFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq)*(double)mTimeResolutionValue) + 0.5);
                mPCLKSyncTPoint = sc_time_stamp().to_double();
            } else {
                mPCLKPeriod = 0;
                mPCLKSyncTPoint = sc_time_stamp().to_double();
            }
        }
        else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief Get frequency value of these blocks
* @details
*  This method implements Python-command called by SC-HEAP
* @note
*  This function is intend to be changed slightly when the number of clock
*  or name of clock sources are changed.
* @param clock_name std::string <clock_name>: name of clock signal
*/
void IBG::GetCLKFreq(const std::string clock_name) 
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == PCLK.basename()) {
            re_printf("info","%s frequency is %0.0f %s\n", PCLK.basename(), (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        }
        else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief Force value to register by the software via Python interface
* @details
*  This method implements Python-command called by SC-HEAP
* @note
*  This function is NOT intend to be changed, keep this unchanged
* @param reg_name std::string <reg_name>: name of register
* @param reg_value unsigned int <value>: value which is set to register
*/
void IBG::ForceRegister (const std::string reg_name, const unsigned int reg_value) 
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
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            pFuncModel->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief Release forced value after forcing registers by the software
*        via Python interface
* @details
*  This method implements Python-command called by SC-HEAP
* @note
*  This function is NOT intend to be changed, keep this unchanged
* @param reg_name std::string <reg_name>: name of register
*/
void IBG::ReleaseRegister (const std::string reg_name) 
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            pFuncModel->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief Write value to registers by software by the software
*        via Python interface
* @details
*  This method implements Python-command called by SC-HEAP
* @note
*  This function is NOT intend to be changed, keep this unchanged
* @param reg_name std::string <reg_name>: name of register
* @param reg_value unsigned int <value>: value which is set to register
*/
void IBG::WriteRegister (const std::string reg_name , const unsigned int reg_value) 
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            pFuncModel->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief Read value of register by software by the software
*        via Python interface
* @details
*  This method implements Python-command called by SC-HEAP
* @note
*  This function is NOT intend to be changed, keep this unchanged
* @param[in] reg_name std::string <reg_name>: name of register
*/
void IBG::ReadRegister (const std::string reg_name) 
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            pFuncModel->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief List all registers name by software by the software
*        via Python interface
* @details
*  This method implements Python-command called by SC-HEAP
* @note
*  This function is NOT intend to be changed, keep this unchanged
*/
void IBG::ListRegister (void) 
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    pFuncModel->RegisterHandler(cmd);
}//}}}
//------------------------------------------------------------------------------
/**@brief Dump help message of all parameters or commands
*/
void IBG::Help (const std::string type) 
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"IBG_MessageLevel (fatal|error|warning|info) Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"IBG_DumpRegisterRW (true/false) Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"IBG_DumpInterrupt (true/false) Enable/disable dumping interrupt information (Default: false)");
            SC_REPORT_INFO(this->basename(),"IBG_EnableRegisterMessage (\"IBG instance\", \"true/false\") Enable/disable info/warning/error message of register IF (Default: true)");

        }
        else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"IBG_AssertReset (reset_name, start_time, period) Assert and de-assert reset signal to the IBG model");
            SC_REPORT_INFO(this->basename(),"IBG_SetCLKFreq (clock_name, freq, unit) Set clock frequency value to IBG model");
            SC_REPORT_INFO(this->basename(),"IBG_GetCLKFreq (clock_name) Get clock frequency value of IBG model");
            SC_REPORT_INFO(this->basename(),"IBG_ForceRegister (unit_name, reg_name, reg_value) Force register with setting value");
            SC_REPORT_INFO(this->basename(),"IBG_ReleaseRegister (unit_name, reg_name) Release register from force value");
            SC_REPORT_INFO(this->basename(),"IBG_WriteRegister (unit_name, reg_name, reg_value) Write a value to a register");
            SC_REPORT_INFO(this->basename(),"IBG_ReadRegister (unit_name, reg_name) Read a value from a register");
            SC_REPORT_INFO(this->basename(),"IBG_ListRegister () Dump name of all registers");
            SC_REPORT_INFO(this->basename(),"IBG_DumpStatusInfo () Dump information of the status registers of the IBG model");
            SC_REPORT_INFO(this->basename(),"IBG_SetAreaAddr (channelId, start_addr, size) Set protected address range");
        } else {
            re_printf("warning","The name (%s) of IBG_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of IBG_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief Select debug message level ("fatal", "error", "warning", "info").
*        One or more than levels can be connected by vertical bar.
* @details
*  This method implements Python-command called by SC-HEAP
* @note
*  This function is NOT intend to be changed, keep this unchanged
* @param msg_lv message level setting string. Example "fatal|error"
*/
void IBG::SetMessageLevel(const std::string msg_lv)
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
        //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        pFuncModel->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        pFuncModel->RegisterHandler(cmd);
    } else {
        //Set message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        pFuncModel->RegisterHandler(cmd);

        //Set message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        pFuncModel->RegisterHandler(cmd);
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief Dump register access information when registers are accessed.
* @details
*  This method implements Python-command called by SC-HEAP
* @note
*  This function is NOT intend to be changed, keep this unchanged
* @param is_enable false: Not dump register access information,
*                  true: Dump register access information
*/
void IBG::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    pFuncModel->RegisterHandler(cmd);
}//}}}
//------------------------------------------------------------------------------
/**@brief Enable/disable dumping interrupt information
*/
void IBG::DumpInterrupt (const std::string is_enable) 
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: IBG_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        if (mDumpInterrupt) {
            re_printf("info","IBG_DumpInterrupt %s\n", "true");
        } else {
            re_printf("info","IBG_DumpInterrupt %s\n", "false");
        }
    } else {
        re_printf("warning","Invalid argument: IBG_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

void IBG::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: IBG_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
                if (mEnableRegisterMessage) {
                    re_printf("info","IBG_EnableRegisterMessage %s\n", "true");
                } else {
                    re_printf("info","IBG_EnableRegisterMessage %s\n", "false");
                }
    } else {
        re_printf("warning","Invalid argument: IBG_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

//------------------------------------------------------------------------------
/**@brief print out value of boolean signal interrupt
* @param intr_name name of the interrupt
* @param value current value of interrupt signal
*/
void IBG::DumpInterruptMsg (const std::string intr_name, const bool value) 
{//{{{
    if (mDumpInterrupt) {
        if (value) {
            re_printf("info","INT [%s] Assert.\n",intr_name.c_str());
        } else {
            re_printf("info","INT [%s] De-assert.\n",intr_name.c_str());
        }
    }
}//}}}
//##############################################################################
// PART 3: custom code
//##############################################################################
//------------------------------------------------------------------------------
// helper methods
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// @brief process asserts the *err_irq* output according to clock edge
void IBG::AssertErr_IrqMethod() 
{//{{{
    err_irq_val = true;
    mUpdateErr_IrqEvent.notify(SC_ZERO_TIME);
    double current_time = sc_time_stamp().to_double();
    mDeassertErr_IrqEvent.notify(CalculateCLKPosEdge(mPCLKPeriod, true, current_time, mPCLKSyncTPoint), mTimeResolutionUnit);
}//}}}
//------------------------------------------------------------------------------
/// @brief process deasserts the *err_irq* output according to clock edge
void IBG::DeassertErr_IrqMethod() 
{//{{{
    err_irq_val = false;
    mUpdateErr_IrqEvent.notify(SC_ZERO_TIME);
}//}}}
//------------------------------------------------------------------------------
/// @brief process deasserts the *err_irq* output according to clock edge
void IBG::UpdateErr_IrqMethod() 
{//{{{
    err_irq.write(err_irq_val);
    DumpInterruptMsg(err_irq.basename(), err_irq_val);
}//}}}
//------------------------------------------------------------------------------
// implement common guard agent controller interface
//------------------------------------------------------------------------------
/**@brief Check if period value of the clock clocking the bus guard is greater than 0 or not
* @return true if the clock under check is greater than 0
*/
bool IBG::CheckGuardClock () 
{//{{{
    return CheckClockPeriod(PCLK.basename());
}//}}}
//------------------------------------------------------------------------------
/**@brief check status of the combination of hard reset and command reset
*        that controls the bus guard is active or not
* @return true if at least one reset source under check is active
*/
bool IBG::GetGuardResetStatus() 
{//{{{
    return (mIsResetHardActive || mIsResetCmdActive);
}//}}}
//------------------------------------------------------------------------------
/**@brief function allows the bus guard to report diagnosis message for debug
* @param[in] severity level of severity of the message
* @param[in] msg the message
*/
void IBG::printMsg(const char* severity, const char* msg) 
{//{{{
    _re_printf(severity, msg);
}//}}}
//------------------------------------------------------------------------------
/**@brief add a address range into the map of protected range. It is invoked via
*        Python interface.
* @details
*  The new range must not overlap any range which has been added before.
* @param[in] ch_id id of the channel to which the specified address range belongs
* @param[in] start_addr start address of the desired address range
* @param[in] size size of the desired address range
* @return true if the desired address does not overlap any address range
*         which has already exist in the map. Otherwise, the new address wont
*         be added into the map.
*/
bool IBG::SetAreaAddr(const uint32_t    channelId
                      ,const uint32_t    start_addr
                      ,const uint32_t    size) 
{//{{{
    return SetAreaAddr("", channelId, start_addr, size);
}//}}}
//------------------------------------------------------------------------------
/**@brief add a address range into the map of protected range. It is invoked via
*        Python interface.
* @details
*  The new range must not overlap any range which has been added before.
* @param[in] groupName name of the group to which the specified address range belongs
* @param[in] ch_id id of the channel to which the specified address range belongs
* @param[in] start_addr start address of the desired address range
* @param[in] size size of the desired address range
* @return true if the desired address does not overlap any address range
*         which has already exist in the map. Otherwise, the new address wont
*         be added into the map.
*/
bool IBG::SetAreaAddr(const string      groupName
                      ,const uint32_t    channelId
                      ,const uint32_t    start_addr
                      ,const uint32_t    size) 
{//{{{
    /// -# validate the given channel Id
    if (channelId >= CHAN_NUM) {
        re_printf("warning", "the given channel id is invalid\n");
        return false;
    }

    /// -# register the given address range into the bus guard
    if (!pBusGuard->SetAreaAddr(groupName, channelId, start_addr, size)) {
        re_printf("warning", "the given address range is invalid\n");
        return false;
    }
    return true;
}//}}}
//------------------------------------------------------------------------------
/**@brief the function API allows the bus guard to report error to the rest of
*        system whenever it detects an illegel transaction
* @details
*        Whenever an illegal transaction is detect, the ibg reports error to
*        the EMC and logs the additional information associated with errors
*        into a set of registers.
* @param[in] groupName name of group of the protected address range to which the transaction is sent
* @param[in] channelId the identifier of the channel of the protected address range to which the transaction is sent
* @param[in] addr target address of the transaction
* @param[in] isDebug whether the transaction is a debug transaction or not
* @param[in] isUserMode whether the transaction is a user-mode transaction or not
* @param[in] isWrite whether the transaction is read access or write access
* @param[in] SPID spid field of the transaction
*/
void IBG::TransferErrInfo (const string    groupName
                           ,const uint32_t  channelId
                           ,const uint32_t  addr
                           ,const bool      isDebug
                           ,const bool      isUserMode
                           ,const bool      isWrite
                           ,const uint8_t   SPID) 
{//{{{
    /// -# save additional information of the error into registers
    if ((*pFuncModel->GERRSTAT)["ERR"] == false) {
        (*pFuncModel->GERRADDR)["ADDR" ] = addr;
        (*pFuncModel->GERRTYPE)["DBG"  ] = isDebug;
        (*pFuncModel->GERRTYPE)["UM"   ] = isUserMode;
        (*pFuncModel->GERRTYPE)["SPID" ] = SPID;
        (*pFuncModel->GERRTYPE)["WRITE"] = isWrite;
        (*pFuncModel->GERRSTAT)["ERR"]  = true;
        /// -# send error event here with timed codes
        double current_time = sc_time_stamp().to_double();
        mAssertErr_IrqEvent.notify(CalculateCLKPosEdge(mPCLKPeriod, false, current_time, mPCLKSyncTPoint), mTimeResolutionUnit);

    } else {
        (*pFuncModel->GERRSTAT)["OVF"]  = true;
    } 

}//}}}

/**@brief get value of GOVFSTAT register 
*/
uint32_t IBG::GetGERRSTAT()
{//{{{
    return pFuncModel->GetGERRSTAT();
}//}}}

//------------------------------------------------------------------------------
/**@brief dump status info of registers
*/
void IBG::DumpStatusInfo()
{//{{{
    std::string type = "StatInfo";
    printf("PROFILE(%s): IBG: Info [%20s] (%10s):\n", type.c_str(), sc_time_stamp().to_string().c_str(), this->name());
    printf("PROFILE(%s): IBG:   GOVFSTAT bits: 0x%08X\n", type.c_str(), this->GetGERRSTAT());
}//}}}

//------------------------------------------------------------------------------
/**@brief the APIs allows the bus guard to get information of validation settings
*        from registers in the register block.
* @param[in] groupName name of group of the protected address range to which the transaction is sent
* @param[in] channelId the identifier of the channel of the protected address range to which the transaction is sent
* @param[out] GEN whether the bus guard is enable or not
* @param[out] DBG whether debug transactions are always forwarded or still need further checks
* @param[out] UM whether user-mode transactions are always discarded or passed to further checks
* @param[out] WG whether write-transactions from any initiator are forwarded (global-write) or need to be check the initiators via SPID fields
* @param[out] RG whether read-transactions from any initiator are forwarded (global-read) or need to be check the initiators via SPID fields
* @param[out] SPID lists of authorized initiators
* @return true if the specified group name and channelId are valid and the settings can be find out in the setting registers
*/
bool IBG::GetGuardSettings(const string   groupName
                           ,const uint32_t channelId
                           ,bool&     GEN
                           ,bool&     DBG
                           ,bool&     UM
                           ,bool&     WG
                           ,bool&     RG
                           ,uint32_t& SPID
                           ) 
{//{{{
    /// -# get settings from the register block 
    GEN  = (*pFuncModel->GPROT0[channelId])["GEN" ];
    DBG  = (*pFuncModel->GPROT0[channelId])["DBG" ];
    UM   = (*pFuncModel->GPROT0[channelId])["UM"  ];
    WG   = (*pFuncModel->GPROT0[channelId])["WG"  ];
    RG   = (*pFuncModel->GPROT0[channelId])["RG"  ];
    SPID = (*pFuncModel->GPROT1[channelId])["SPID"];
    return true;
}//}}}
// vim600: set foldmethod=marker :
//##############################################################################
// EOF
//##############################################################################

