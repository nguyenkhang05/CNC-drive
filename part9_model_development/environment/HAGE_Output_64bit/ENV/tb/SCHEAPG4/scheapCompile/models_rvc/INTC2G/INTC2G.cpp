// ---------------------------------------------------------------------
// $Id: INTC2G.cpp,v 1.5 2019/09/19 10:16:17 ngathunguyen Exp $
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
#include <stdint.h>
#include <string>
#include "re_define.h"
#include "INTC2G.h"

using std::string;

INTC2G::INTC2G(    sc_module_name   name
               , uint32_t         rLatency
               , uint32_t         wLatency
               , SIM_MODE_T       simmode)
:CHAN_NUM_START(32)
,MPIDBitsNumber(8)
,mLineNum(0)
,PCLK("PCLK")
,mPCLKFreqUnit("Hz")
,mPCLKPeriod(0)
,mPCLKOrgFreq(0)
,mPCLKFreq(0)
,mPCLKSyncTPoint(0)
,mIsResetPortActive(false)
,mIsResetCmdReceive(false)
,mIsResetCmdActive(false)
,mResetCmdEvent("mResetCmdEvent")
,mCancelResetCmdEvent("mCancelResetCmdEvent")
,mResetCmdPeriod(0)
,mTimeResolutionValue(0)
,mTimeResolutionUnit(SC_NS)
,mDumpInterrupt(false)
,pFuncModel(NULL)
,pIntc2Guard(NULL)
,ts(NULL)
,tsp(NULL)
,isp(NULL)
,err_irq("err_irq")
,INTC2G_BusBridgeModule(name)
{//{{{
    
    CHAN_NUM_END = 695;
    //-------------------------------------
    /// -# initialize constants
    mEnableRegisterMessage = true;
    // time resolution
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    //-------------------------------------
    /// -# instantiate input/output signals
    //Bind clock, reset signal and socket to Bus IF
    INTC2G_BusSlave::setSlaveFreqPort32 (&PCLK);
    INTC2G_BusSlave::setSlaveResetPort32(&resetPort);
    INTC2G_BusSlave::setTargetSocket32  ((char*)"ts");

    // BusBridgeSlaveBase
    INTC2G_BusBridgeSlave::setSlaveFreqPort (&PCLK);
    INTC2G_BusBridgeSlave::setSlaveResetPort(&resetPort);
    INTC2G_BusBridgeSlave::setTargetSocket  ((char*)"tsp");

    // BusMasterBase
    INTC2G_BusMaster::setMasterFreqPort32 (&PCLK);
    INTC2G_BusMaster::setMasterResetPort32(&resetPort);
    INTC2G_BusMaster::setInitiatorSocket32((char*)"isp");

    //-------------------------------------
    /// -# instantiate the function model
    ts  = INTC2G_BusSlave::tSocket32[0];     //Alias for binding
    tsp = INTC2G_BusBridgeSlave::tSocket[0]; //Alias for binding
    isp = INTC2G_BusMaster::iSocket32[0];    //Alias for binding

    /// -# instantiate the function model
    pFuncModel = new INTC2G_Func ((string)name, this, CHAN_NUM_END);
    sc_assert(pFuncModel != NULL);

    INTC2G_BusSlave::mBusSlaveIf32[0]->setFuncModulePtr(pFuncModel);
    INTC2G_BusSlave::mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    INTC2G_BusSlave::mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    INTC2G_BusSlave::mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    INTC2G_BusSlave::mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    // Instantiate the function model
    pIntc2Guard = new INTC2G_CommonGuard (string(name)                        // name
                                    ,this                                   // Parent as Intc2gBusBridgeModule
                                    ,INTC2G_BusBridgeSlave::mBusSlaveIf[0]  // SlaveIf
                                    ,INTC2G_BusMaster::mBusMasterIf32[0]    // MasterIf
                                    ,simmode                                // simmode
                                    ,this                                   // AgentController_ptr as Intc2g_CommonGuardAgentControllerIf
                                    );
    sc_assert(pIntc2Guard != NULL);

    // BusBridgeModuleBase
    INTC2G_BusBridgeModule::mpCore = pIntc2Guard;

    // BusBridgeSlaveBase
    INTC2G_BusBridgeSlave::mBusSlaveIf[0]->setFuncModulePtr((BusSlaveFuncIf *)pIntc2Guard);
    INTC2G_BusBridgeSlave::mBusSlaveIf[0]->setBusProtocol(BUS_APB);
    INTC2G_BusBridgeSlave::mBusSlaveIf[0]->setEnableToCheckDataSize();

    // BusMasterBase
    INTC2G_BusMaster::mBusMasterIf32[0]->setFuncModulePtr((BusMasterFuncIf *)pIntc2Guard);
    INTC2G_BusMaster::mBusMasterIf32[0]->setBusProtocol(BUS_APB);

    //-------------------------------------
    /// -# instantiate internal signals, sub-modules, variables
    // processes handle clocks
    SC_METHOD(PCLKMethod);
        dont_initialize();
        sensitive << PCLK;
    SC_METHOD(UpdateSysClockMethod);
        dont_initialize();
        sensitive << mClockChangeEvent;        
    // processes handle reset sources
    SC_METHOD(resetPortHandleMethod);
        dont_initialize();
        sensitive << resetPort;
    SC_METHOD(resetCmdHandleMethod);
        dont_initialize();
        sensitive << mResetCmdEvent;
    SC_METHOD(CancelResetCmdMethod);
        dont_initialize();
        sensitive << mCancelResetCmdEvent;

    //-------------------------------------
    /// -# instantiate events and register processes
    SC_METHOD(AssertErr_IrqMethod);
        dont_initialize();
        sensitive << mAssertErr_IrqEvent;

    SC_METHOD(DeassertErr_IrqMethod);
        dont_initialize();
        sensitive << mDeassertErr_IrqEvent;

    SC_METHOD(UpdateErr_IrqMethod);
        dont_initialize();
        sensitive << mUpdateErr_IrqEvent;
        
    //Initialize signal, state
    Initialize();    
    err_irq.initialize(false);
    CommandInit((string)name);
}//}}}

//------------------------------------------------------------------------------
/**@brief public destructor
 * @details
 * Detailed steps as follows:
 */
INTC2G::~INTC2G()
{//{{{
    //-------------------------------------
    /// -# destruct input/output signals

    //-------------------------------------
    /// -# destruct function model
    if (pFuncModel != NULL) delete pFuncModel;

    //-------------------------------------
    /// -# destruct internal signals, sub-modules, variables, events
    if (pIntc2Guard != NULL) delete pIntc2Guard;

    //-------------------------------------
    /// -# destruct configration constants

}//}}}

//##############################################################################
// PART 2: clock, reset and initialization
//##############################################################################

//------------------------------------------------------------------------------
/**@brief monitor clock
 */
void INTC2G::PCLKMethod()
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq(PCLK.basename(), freq_value, "Hz");
}//}}}
//------------------------------------------------------------------------------
/**@brief Check if clock period value is greater than 0 or not
 * @return true if the clock under check is greater than 0
 */
bool INTC2G::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    if (mPCLKPeriod > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

//------------------------------------------------------------------------------
/**@brief Calculate synchronous time
 */
sc_time INTC2G::CalculateClockEdge()
{//{{{
    double current_time = sc_time_stamp().to_double();
    return sc_time(CalculateCLKPosEdge((double) mPCLKPeriod, false
                  ,current_time, mPCLKSyncTPoint)
                  ,mTimeResolutionUnit);
}//}}}

//------------------------------------------------------------------------------
/**@brief Reset function when reset command is active
 */
void INTC2G::resetCmdHandleMethod()
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", resetPort.basename());
    this->EnableReset(mIsResetCmdActive);
    mCancelResetCmdEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

//------------------------------------------------------------------------------
/** @brief Cancel reset function when reset command is active
 */
void INTC2G::CancelResetCmdMethod ()
{//{{{
    mIsResetCmdActive  = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of %s is over.\n", resetPort.basename());
    this->EnableReset(mIsResetCmdActive);
}//}}}

//------------------------------------------------------------------------------
/**@brief resetport Handle method
 * When "resetport" is asserted, model is initialized
 */
void INTC2G::resetPortHandleMethod()
{//{{{
    bool resetport_state = resetPort.read();

    if (resetport_state == vpcl::emResetActive){
        mIsResetPortActive = true;
        re_printf("info", "The reset port %s is asserted.\n", resetPort.basename());        
        this->EnableReset(mIsResetPortActive);
    } else{ //reset de-active
        mIsResetPortActive = false;
        re_printf("info", "The reset port %s is de-asserted.\n", resetPort.basename());        
        pFuncModel->EnableReset(mIsResetPortActive);
    }
    //todo: when resetport is active, cancel reset command
    if(mIsResetPortActive && mIsResetCmdReceive){
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
        mResetCmdEvent.cancel();
        mCancelResetCmdEvent.cancel();
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief Process when hard reset is asserted
 *
 */
void INTC2G::EnableReset(bool isActive)
{//{{{
    pFuncModel->EnableReset(isActive);
    Initialize();
    CancelEvents();
    mUpdateErr_IrqEvent.notify(SC_ZERO_TIME);
    pFuncModel->SetINTC2GPROTmNum(CHAN_NUM_END);
}//}}}

//------------------------------------------------------------------------------
/**@brief Initialize model when hard reset is asserted
 *
 */
void INTC2G::Initialize()
{//{{{ 
    err_irq_val = false;
}//}}}

//------------------------------------------------------------------------------
/**@brief Cancel events
 *
 */
void INTC2G::CancelEvents()
{//{{{
    mUpdateErr_IrqEvent.cancel();
    mAssertErr_IrqEvent.cancel();
    mDeassertErr_IrqEvent.cancel();
}//}}}

//------------------------------------------------------------------------------
/**@brief Get reset status
 *
 */
bool INTC2G::GetResetStatus() const
{//{{{
    return (mIsResetPortActive || mIsResetCmdActive);
}//}}}

//------------------------------------------------------------------------------
/// @brief process asserts the *err_irq* output according to clock edge
void INTC2G::AssertErr_IrqMethod()
{//{{{
    err_irq_val = true;
    mUpdateErr_IrqEvent.notify(SC_ZERO_TIME);
    mDeassertErr_IrqEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
}//}}}
//------------------------------------------------------------------------------
/// @brief process deasserts the *err_irq* output according to clock edge
void INTC2G::DeassertErr_IrqMethod()
{//{{{
    err_irq_val = false;
    mUpdateErr_IrqEvent.notify(SC_ZERO_TIME);
}//}}}
//------------------------------------------------------------------------------
/// @brief process deasserts the *err_irq* output according to clock edge
//  a method to pass WRITE_CHECK
void INTC2G::UpdateErr_IrqMethod()
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
bool INTC2G::CheckGuardClock ()
{//{{{
    return CheckClockPeriod(PCLK.basename());
}//}}}

//------------------------------------------------------------------------------
/**@brief check status of the combination of hard reset and command reset
 *        that controls the bus guard is active or not
 * @return true if at least one reset source under check is active
 */
bool INTC2G::GetGuardResetStatus()
{//{{{
    return GetResetStatus();
}//}}}

//------------------------------------------------------------------------------
/**@brief function allows the bus guard to report diagnosis message for debug
 * @param[in] severity level of severity of the message
 * @param[in] msg the message
 */
void INTC2G::printMsg(const char* severity, const char* msg)
{//{{{
    _re_printf(severity, msg);
}//}}}

//------------------------------------------------------------------------------
/**@brief add a address range into the map of protected range. It is invoked via
 *        Python interface.
 * @details
 *  The new range must not overlap any range which has been added before.
 * @param[in] channelId id of the channel to which the specified address range belongs
 * @param[in] startAddr start address of the desired address range
 * @param[in] size size of the desired address range                            
 * @return true if the desired address does not overlap any address range
 *         which has already exist in the map. Otherwise, the new address wont
 *         be added into the map.
 */

bool INTC2G::SetAreaAddr(const uint32_t    channelId
                        ,const uint32_t    startAddr
                        ,const uint32_t    size)
{//{{{
    if ((channelId > CHAN_NUM_END + 1) || (channelId < CHAN_NUM_START)) {
        re_printf("warning", "the given channel id is invalid\n");
        return false;
    } 
    /// -# register the given address range into the bus guard
    if (!pIntc2Guard->SetAreaAddr(channelId, startAddr, size)) {
        re_printf("warning", "the given address range is invalid\n");
        return false;
    }
    return true;
}//}}}

/**@brief Function allows the bus guard to report error whenever it detects an
 *          illegel transaction
 * @details
 *  The new range must not overlap any range which has been added before.
 * @param[in] channelId the identifier of the channel of the protected address range
 *              to which the transaction is sent
 * @param[in] addr start address of transaction
 * @param[in] isDebug transaction is a debug transaction or not
 * @param[in] isUserMode transaction is a user transaction or not
 * @param[in] isWrite transaction is  read or write
 * @param[in] SPID spid field of the transaction
 */

void INTC2G::TransferErrInfo(const uint32_t     channelId
                            ,const uint32_t     addr
                            ,const bool         isDebug
                            ,const bool         isUserMode
                            ,const bool         isWrite
                            ,const uint8_t      SPID)
{//{{{
    if ((*(pFuncModel->INTC2GERRSTAT))["ERR"] == false) {
        (*pFuncModel->INTC2GERRADDR)["ADDR" ] = addr;
        (*pFuncModel->INTC2GERRTYPE)["DBG"  ] = isDebug;
        (*pFuncModel->INTC2GERRTYPE)["UM"   ] = isUserMode;
        (*pFuncModel->INTC2GERRTYPE)["SPID" ] = SPID;
        (*pFuncModel->INTC2GERRTYPE)["WRITE"] = isWrite;
        (*(pFuncModel->INTC2GERRSTAT))["ERR"] = true;
    /// -# send error event here with timed codes
        mAssertErr_IrqEvent.notify(CalculateClockEdge());
    } else {
        (*(pFuncModel->INTC2GERRSTAT))["OVF"] = true;
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief the APIs allows the bus guard to get information of validation settings
 *        from registers in the register block.
 * @param[in] channelId the identifier of the channel of the protected address range to which the transaction is sent
 * @param[out] GEN whether the bus guard is enable or not
 * @param[out] DBG whether debug transactions are always forwarded or still need further checks
 * @param[out] UM whether user-mode transactions are always discarded or passed to further checks
 * @param[out] WG whether write-transactions from any initiator are forwarded (global-write) or need to be check the initiators via SPID fields
 * @param[out] RG whether read-transactions from any initiator are forwarded (global-read) or need to be check the initiators via SPID fields
 * @param[out] SPID lists of authorized initiators
 * @return true if the specified channelId are valid and the settings can be find out in the setting registers
 */
bool INTC2G::GetGuardSettings( const uint32_t channelId
                              ,bool&     GEN
                              ,bool&     DBG
                              ,bool&     UM
                              ,bool&     WG
                              ,bool&     RG
                              ,uint32_t& SPID
                          )
{//{{{
    /// -# validate the given channel Id

    uint8_t MPID = 0;
    /// -# get settings from the register block
    if (channelId == (CHAN_NUM_END + 1)) { // PROT_GR
        GEN  = (*pFuncModel->INTC2GPROT_GR)["GEN" ];
        DBG  = (*pFuncModel->INTC2GPROT_GR)["DBG" ];
        UM   = (*pFuncModel->INTC2GPROT_GR)["UM"  ];
        WG   = (*pFuncModel->INTC2GPROT_GR)["WG"  ];
        RG   = (*pFuncModel->INTC2GPROT_GR)["RG"  ];
        MPID = (*pFuncModel->INTC2GPROT_GR)["MPID"];
    } else {//PROT_n
        GEN  = (*pFuncModel->INTC2GPROT[channelId])["GEN" ];
        DBG  = (*pFuncModel->INTC2GPROT[channelId])["DBG" ];
        UM   = (*pFuncModel->INTC2GPROT[channelId])["UM"  ];
        WG   = (*pFuncModel->INTC2GPROT[channelId])["WG"  ];
        RG   = (*pFuncModel->INTC2GPROT[channelId])["RG"  ];
        MPID = (*pFuncModel->INTC2GPROT[channelId])["MPID"];
    }
    SPID = spidDecode(MPID);

    return true;
}//}}}
//------------------------------------------------------------------------------
/**@brief get value of INTC2GERRSTAT register
 */
uint32_t INTC2G::GetINTC2GERRSTAT()
{//{{{
    return pFuncModel->GetINTC2GERRSTAT();
}//}}}
//------------------------------------------------------------------------------
/**@brief Decode SPID (onehot code) from MPID value and register INTC2GMPIDn
 * @param[in] mpidValue value of MPID bits (onehot code)
 * @return[out] SPID in onehot code
 */
uint32_t INTC2G::spidDecode(uint8_t mpidValue)
{//{{{
    uint32_t spidOneHot = 0;
    if (mpidValue == 0)
        return 0;
    else {
        uint8_t spidValue;
        for(uint8_t index = 0; index < MPIDBitsNumber; index++){
            if (((mpidValue >> index)&0x1) == true) {
                spidValue = (*pFuncModel->INTC2GMPID[index])["SPID"];
                spidOneHot = spidOneHot | (0x1 << spidValue);
            }
        }
    }

    return spidOneHot;
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
void INTC2G::AssertReset(const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == resetPort.basename()) {
        if ((!mIsResetPortActive) && (!mIsResetCmdReceive)) {
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
void INTC2G::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
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
                mClockChangeEvent.notify(SC_ZERO_TIME);
            } else {
                mPCLKPeriod = 0;
                mPCLKSyncTPoint = sc_time_stamp().to_double();
                CancelEvents();
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
/**@brief Update interrupt sys with clock. 
 */
void INTC2G::UpdateSysClockMethod ()
{//{{{
    if (err_irq_val == true) {
        mDeassertErr_IrqEvent.notify(SC_ZERO_TIME);
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
void INTC2G::GetCLKFreq(const std::string clock_name)
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
void INTC2G::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
void INTC2G::ReleaseRegister (const std::string reg_name)
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
void INTC2G::WriteRegister (const std::string reg_name , const unsigned int reg_value)
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
void INTC2G::ReadRegister (const std::string reg_name)
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
void INTC2G::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    pFuncModel->RegisterHandler(cmd);
}//}}}
//------------------------------------------------------------------------------
/**@brief Dump help message of all parameters or commands
 */
void INTC2G::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"INTC2G_MessageLevel (fatal|error|warning|info) Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"INTC2G_DumpRegisterRW (true/false) Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"INTC2G_DumpInterrupt (true/false) Enable/disable dumping interrupt information (Default: false)");
            SC_REPORT_INFO(this->basename(),"INTC2G_EnableRegisterMessage (\"INTC2G instance\", \"true/false\")             Enable/disable info/warning/error message of register IF (Default: true)");            
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"INTC2G_AssertReset (reset_name, start_time, period) Assert and de-assert reset signal to the INTC2G model");
            SC_REPORT_INFO(this->basename(),"INTC2G_SetCLKFreq (clock_name, freq, unit) Set clock frequency value to INTC2G model");
            SC_REPORT_INFO(this->basename(),"INTC2G_GetCLKFreq (clock_name) Get clock frequency value of INTC2G model");
            SC_REPORT_INFO(this->basename(),"INTC2G_ForceRegister (unit_name, reg_name, reg_value) Force register with setting value");
            SC_REPORT_INFO(this->basename(),"INTC2G_ReleaseRegister (unit_name, reg_name) Release register from force value");
            SC_REPORT_INFO(this->basename(),"INTC2G_WriteRegister (unit_name, reg_name, reg_value) Write a value to a register");
            SC_REPORT_INFO(this->basename(),"INTC2G_ReadRegister (unit_name, reg_name) Read a value from a register");
            SC_REPORT_INFO(this->basename(),"INTC2G_ListRegister () Dump name of all registers");
            SC_REPORT_INFO(this->basename(),"INTC2G_SetAreaAddr (channelId, start_addr, size) Set protected address range");
        } else {
            re_printf("warning","The name (%s) of INTC2G_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of INTC2G_Help argument is wrong (commands or parameters)\n", type.c_str());
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
void INTC2G::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(INTC2G::basename(), msg_return.c_str());
    }

    std::string msg_ret = this->handleCommand(cmd);
    if (msg_ret != "") {
        SC_REPORT_INFO(this->basename(), msg_ret.c_str());
    } else {
        // do not thing
    }
    //Set message level for Register IF class    
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
void INTC2G::DumpRegisterRW (const std::string is_enable)
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
void INTC2G::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: INTC2G_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "false";
        if (mDumpInterrupt){
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","INTC2G_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: INTC2G_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief dump status info of registers
 */
void INTC2G::DumpStatusInfo()
{//{{{
    std::string type = "StatInfo";
    printf("PROFILE(%s): INTC2G: Info [%20s] (%10s):\n", type.c_str(), sc_time_stamp().to_string().c_str(), this->name());
    printf("PROFILE(%s): INTC2G: INTC2GERRSTAT bits: 0x%08X\n", type.c_str(), this->GetINTC2GERRSTAT());
}//}}}

//------------------------------------------------------------------------------
/**@brief print out value of boolean signal interrupt
 * @param intr_name name of the interrupt
 * @param value current value of interrupt signal
 */
void INTC2G::DumpInterruptMsg (const std::string intr_name, const bool value)
{//{{{
    if (mDumpInterrupt) {
         if (value) {
             re_printf("info","INT [%s] Assert.\n",intr_name.c_str());
         } else {
             re_printf("info","INT [%s] De-assert.\n",intr_name.c_str());
         }
    }
}//}}}

// Description: Enable/disable info/warning/error message of register IF
void INTC2G::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: INTC2G_EnableRegisterMessage %s.\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        std::string temp = "false";
        if(mEnableRegisterMessage){
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","INTC2G_EnableRegisterMessage %s\n", temp.c_str());
    } else {
        re_printf("warning","Invalid argument: INTC2G_EnableRegisterMessage %s.\n", is_enable.c_str());
    }
}//}}}

// Description: Set number of INTC2GPROT register can be used
void INTC2G::SetINTC2GPROTmNum(unsigned int channelnum)
{
    if(2048 < channelnum || channelnum < 32) {
        re_printf("warning","INTC2G's number of INTC2GPROT_m is wrong, it is set to the default 695 automatically. \n");
        CHAN_NUM_END = 695;
    } else {
        CHAN_NUM_END = channelnum;
        re_printf("info","Set number of INTC2GPROT_m is %d.\n", channelnum);
    }
}
// vim600: set foldmethod=marker :
