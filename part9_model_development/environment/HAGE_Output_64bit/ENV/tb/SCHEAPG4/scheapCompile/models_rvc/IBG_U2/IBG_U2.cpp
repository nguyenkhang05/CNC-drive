// ---------------------------------------------------------------------
// $Id: IBG_U2.cpp,v 1.2 2020/02/26 06:51:33 duongphan Exp $
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

#include "IBG_U2.h"
#include "IBG_U2_Func.h"

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
IBG_U2::IBG_U2(sc_module_name iname, std::string itype, uint8_t ichanNum, uint32_t rLatency, uint32_t wLatency, SIM_MODE_T simmode)
: Ibg_u2BusBridgeModule(iname)
, mLineNum (0)
, chanNum (ichanNum)
//### modeling clock sources ###
, PCLK ("PCLK")
, cpu_clk ("cpu_clk")
, mPCLKPeriod  (0)
, mPCLKOrgFreq (0)
, mPCLKFreq    (0)
, mPCLKFreqUnit("Hz")
, mPCLKSyncTPoint (0)
, mPCLKZeroClkEvent ("PCLKZeroClkEvent")
, mcpu_clkPeriod  (0)
, mcpu_clkOrgFreq (0)
, mcpu_clkFreq    (0)
, mcpu_clkFreqUnit("Hz")
, mcpu_clkSyncTPoint (0)
, mcpu_clkZeroClkEvent ("cpu_clkZeroClkEvent")
//### modeling reset sources ###
, PRESETn            ("PRESETn")
//, mIsResetPortInit   (false)
, mIsResetHardActive (false)
, mIsResetCmdActive  (false)
, mIsResetCmdReceive (false)
, mIsPRESETnHardActive (false)
, mIsPRESETnCmdReceive (false)
, mIsPRESETnCmdActive  (false)
, mPRESETnCmdPeriod    (false)
, mResetCmdPeriod    (0)
, mResetCmdEvent       ("resetPortCmdEvent")
, mResetCmdCancelEvent ("resetPortCmdCancelEvent")
, mPRESETnCmdEvent     ("PRESETnCmdEvent")
, mPRESETnCmdCancelEvent ("PRESETnCmdCancelEvent")
//### time resolution ###
, mTimeResolutionValue (1)
, mTimeResolutionUnit  (SC_NS)
//### constructing the model ###
, pFuncModel (NULL)
//### Python interface ###
, mDumpInterrupt (false)
//### public interfaces ###
, ibgerr ("ibgerr")
// internal sub-modules, signals, variables
, ibgerr_val (false)
, pBusGuard (NULL)
{//{{{
    /// -# initialize map contains Guard Id 
    Ibg_u2ChanNum structGroup;
    structGroup.chanNumMin = 2;
    structGroup.chanNumMax = 9;
    mGuardIdMap.insert(std::pair<std::string, Ibg_u2ChanNum>("IBG_U2_IPIG", structGroup));
    structGroup.chanNumMin = 2;
    structGroup.chanNumMax = 17;
    mGuardIdMap.insert(std::pair<std::string, Ibg_u2ChanNum>("IBG_U2_BRG", structGroup));
    structGroup.chanNumMin = 3;
    structGroup.chanNumMax = 10;
    mGuardIdMap.insert(std::pair<std::string, Ibg_u2ChanNum>("IBG_U2_TPTG", structGroup));
    //-------------------------------------
    /// -# initialize constants
    // time resolution
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    //-------------------------------------
    /// -# instantiate input/output signals
    //Bind clock, reset signal and socket to Bus IF
    Ibg_u2BusSlave::setSlaveFreqPort32 (&PCLK);
    Ibg_u2BusSlave::setSlaveResetPort32(&PRESETn);
    Ibg_u2BusSlave::setTargetSocket32  ("ts");

    // BusBridgeSlaveBase
    Ibg_u2BusBridgeSlave::setSlaveFreqPort (&cpu_clk);
    Ibg_u2BusBridgeSlave::setSlaveResetPort(&resetPort);
    Ibg_u2BusBridgeSlave::setTargetSocket  ("tsp");
    // BusMasterBase
    Ibg_u2BusMaster::setMasterFreqPort32 (&cpu_clk);
    Ibg_u2BusMaster::setMasterResetPort32(&resetPort);
    Ibg_u2BusMaster::setInitiatorSocket32("isp");

    //-------------------------------------
    /// -# instantiate the function model
    ts  = Ibg_u2BusSlave::tSocket32[0];     //Alias for binding
    tsp = Ibg_u2BusBridgeSlave::tSocket[0]; //Alias for binding
    isp = Ibg_u2BusMaster::iSocket32[0];    //Alias for binding
    
    if (itype == "IPIG") {
        mGroupName = "IBG_U2_IPIG";
    }
    else if (itype == "BRG") {
        mGroupName = "IBG_U2_BRG";
    }
    else if (itype == "TPTG") {
        mGroupName = "IBG_U2_TPTG";
    }
    else {
        re_printf("warning", "Type of group name is wrong, so group name is assigned to IBG_U2_IPIG model\n");
        mGroupName = "IBG_U2_IPIG";
    }

    /// -# instantiate the function model
    std::map<std::string, Ibg_u2ChanNum>::iterator it;
    for (it = mGuardIdMap.begin(); it != mGuardIdMap.end(); it++) {
        if (it->first == mGroupName) {
            Ibg_u2ChanNum chanNumMap = it->second;
            if (chanNum > chanNumMap.chanNumMax) {
                re_printf("warning", "the number of channel is larger than upper limit of default, so it is assigned to upper limit of default\n");
                chanNum = chanNumMap.chanNumMax;
            }
            else if (chanNum < chanNumMap.chanNumMin) {
                re_printf("warning", "the number of channel is smaller than lower limit of default, so it is assigned to lower limit of default\n");
                chanNum = chanNumMap.chanNumMin;
            }
            break;
        }
    }
    pFuncModel = new IBG_U2_Func (mGroupName, this, chanNum, this);
    sc_assert(pFuncModel != NULL);

    Ibg_u2BusSlave::mBusSlaveIf32[0]->setFuncModulePtr(pFuncModel);
    Ibg_u2BusSlave::mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    Ibg_u2BusSlave::mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    Ibg_u2BusSlave::mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    Ibg_u2BusSlave::mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    // Instantiate the function model
    pBusGuard = new IBG_U2_CommonGuard (mGroupName                      // name
                                    ,this                               // Parent as Ibg_u2BusBridgeModule
                                    ,Ibg_u2BusBridgeSlave::mBusSlaveIf[0]  // SlaveIf
                                    ,Ibg_u2BusMaster::mBusMasterIf32[0]    // MasterIf
                                    ,simmode                            // simmode
                                    ,this                               // AgentController_ptr as IBG_U2_CommonGuardAgentControllerIf
                                    );
    sc_assert(pBusGuard != NULL);
    // BusBridgeModuleBase
    Ibg_u2BusBridgeModule::mpCore = pBusGuard;
    // BusBridgeSlaveBase
    Ibg_u2BusBridgeSlave::mBusSlaveIf[0]->setFuncModulePtr((BusSlaveFuncIf *)pBusGuard);
    Ibg_u2BusBridgeSlave::mBusSlaveIf[0]->setBusProtocol(BUS_VCI);
    Ibg_u2BusBridgeSlave::mBusSlaveIf[0]->setEnableToCheckDataSize();
    // BusMasterBase
    Ibg_u2BusMaster::mBusMasterIf32[0]->setFuncModulePtr((BusMasterFuncIf *)pBusGuard);
    Ibg_u2BusMaster::mBusMasterIf32[0]->setBusProtocol(BUS_VCI);

    //-------------------------------------
    /// -# instantiate internal signals, sub-modules, variables


    //-------------------------------------
    /// -# instantiate events and register processes
    // processes handle clocks
    SC_METHOD(PCLKMethod);
        dont_initialize();
        sensitive << PCLK;
    SC_METHOD(cpu_clkMethod);
        dont_initialize();
        sensitive << cpu_clk;

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
    SC_METHOD(HandlePRESETnCmdMethod);
        dont_initialize();
        sensitive << mPRESETnCmdEvent;
    SC_METHOD(CancelPRESETnCmdMethod);
        dont_initialize();
        sensitive << mPRESETnCmdCancelEvent;

    // processes handle ibgerr
    SC_METHOD(AssertIbgErrMethod);
        dont_initialize();
        sensitive << mAssertIbgErrEvent;
    SC_METHOD(DeassertIbgErrMethod);
        dont_initialize();
        sensitive << mDeassertIbgErrEvent;
    SC_METHOD(UpdateIbgErrMethod);
        dont_initialize();
        sensitive << mUpdateIbgErrEvent;
}//}}}
//------------------------------------------------------------------------------
/**@brief public destructor
 * @details
 * Detailed steps as follows:
 */
IBG_U2::~IBG_U2() 
{//{{{
    //-------------------------------------
    /// -# destruct function model
    if (pFuncModel != NULL) delete pFuncModel;

    //-------------------------------------
    /// -# destruct internal signals, sub-modules, variables, events
    if (pBusGuard != NULL) delete pBusGuard;
}//}}}
//##############################################################################
// PART 2: clock, reset and initialization
//##############################################################################

//------------------------------------------------------------------------------
/**@brief monitor PCLK clock
 */
void IBG_U2::PCLKMethod() 
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq(PCLK.basename(), freq_value, "Hz");
}//}}}
//------------------------------------------------------------------------------
/**@brief monitor cpu_clk clock
 */
void IBG_U2::cpu_clkMethod() 
{//{{{
    sc_dt::uint64 freq_value = cpu_clk.read();
    SetCLKFreq(cpu_clk.basename(), freq_value, "Hz");
}//}}}
//------------------------------------------------------------------------------
/**@brief Check if clock period value is greater than 0 or not
 * @return true if the clock under check is greater than 0
 */
bool IBG_U2::CheckClockPeriod (const std::string clock_name) 
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    if (clock_name == PCLK.basename()) {
        clock_period = mPCLKPeriod;
        if (clock_period > 0) {
            clock_status = true;
        } else {
            clock_status = false;
            re_printf("info","%s frequency is zero\n", clock_name.c_str());
        }
    }
    else if (clock_name == cpu_clk.basename()) {
        clock_period = mcpu_clkPeriod;
        if (clock_period > 0) {
            clock_status = true;
        } else {
            clock_status = false;
            re_printf("info","%s frequency is zero\n", clock_name.c_str());
        }
    }
    return clock_status;
}//}}}
//------------------------------------------------------------------------------
/**@brief Calculate synchronous time
 */
double IBG_U2::CalculateClockEdge (const std::string clock_name, const bool is_pos, const double time_point) 
{//{{{
    sc_dt::uint64 Period     = 0;
    double SyncTPoint        = 0;
    double period_num        = 0;
    double edge_point        = 0;

    if (clock_name == cpu_clk.basename()) {
        Period     = mcpu_clkPeriod;
        SyncTPoint = mcpu_clkSyncTPoint;
    }

    period_num = (time_point - SyncTPoint) / Period;
    sc_dt::uint64 mod_period_num = (sc_dt::uint64)period_num;
    if (is_pos) {
        if ((double)mod_period_num < period_num) {
            mod_period_num++;
        }
    }

    edge_point = mod_period_num * Period - (time_point - SyncTPoint);
    return edge_point;
}//}}}
//------------------------------------------------------------------------------
/**@brief Calculate synchronous time
 */
sc_time IBG_U2::CalculateClockEdge (const std::string clock_name) 
{//{{{
    return sc_time(CalculateClockEdge(clock_name
                  ,true
                  ,sc_time_stamp().to_double())
                  ,mTimeResolutionUnit);
}//}}}
//------------------------------------------------------------------------------
/**@brief monitor resetPort port
 */
void IBG_U2::resetPortMethod() 
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
    if (mIsResetCmdReceive && mIsResetHardActive) {
        mResetCmdEvent.cancel();
        mResetCmdCancelEvent.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief monitor PRESETn port
 */
void IBG_U2::PRESETnMethod() 
{//{{{
    bool reset = PRESETn.read();   
    if (reset == vpcl::emResetActive){
        mIsPRESETnHardActive = true;
        re_printf("info", "The PRESETn is asserted.\n");
        this->EnablePRESETnReset(true);
    }
    else {
        mIsPRESETnHardActive = false;
        re_printf("info", "The PRESETn is de-asserted.\n");
        this->EnablePRESETnReset(false);
    }
    
    //Cancel AssertReset command when PRESETn is active
    if (mIsPRESETnCmdReceive && mIsPRESETnHardActive) {
        mPRESETnCmdEvent.cancel();
        mPRESETnCmdCancelEvent.cancel();
        mIsPRESETnCmdReceive = false;
        mIsPRESETnCmdActive = false;
    }
}//}}}
//------------------------------------------------------------------------------
/** @brief Process reset function when reset command is active
 */
void IBG_U2::HandleResetCmdMethod () 
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", resetPort.basename());
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}
//------------------------------------------------------------------------------
/** @brief Process PRESETn reset function when PRESETn reset command is active
 */
void IBG_U2::HandlePRESETnCmdMethod () 
{//{{{
    mIsPRESETnCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", PRESETn.basename());
    this->EnablePRESETnReset(mIsPRESETnCmdActive);
    mPRESETnCmdCancelEvent.notify(mPRESETnCmdPeriod, SC_NS);
}//}}}
//------------------------------------------------------------------------------
/** @brief Cancel reset function when reset command is active
 */
void IBG_U2::CancelResetCmdMethod () 
{//{{{
    mIsResetCmdActive  = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of %s is over.\n", resetPort.basename());
    this->EnableReset(mIsResetCmdActive);
}//}}}
//------------------------------------------------------------------------------
/** @brief Cancel PRESETn reset function when PRESETn reset command is active
 */
void IBG_U2::CancelPRESETnCmdMethod () 
{//{{{
    mIsPRESETnCmdActive  = false;
    mIsPRESETnCmdReceive = false;
    re_printf("info","PRESETn reset period of %s is over.\n", PRESETn.basename());
    this->EnablePRESETnReset(mIsPRESETnCmdActive);
}//}}}
//------------------------------------------------------------------------------
/**@brief initialize outputs and internal variables, internal states,
 *        internal signals
 * @details
 *   This function is called when the model is reset or the model is reinitialized
 *   by command from CPU via register block
 * Detailed steps as follows:
 */
void IBG_U2::Initialize(void) 
{//{{{
    /// -# initialize all outputs
    ibgerr_val = false;
    mUpdateIbgErrEvent.notify(SC_ZERO_TIME);
}//}}}
//------------------------------------------------------------------------------
/**@brief cancel all scheduled events
 * @details
 *   This function is called when the model is reset
 * Detailed steps as follows:
 */
void IBG_U2::CancelEvents(void) 
{//{{{
    mAssertIbgErrEvent.cancel();
    mUpdateIbgErrEvent.cancel();
    mDeassertIbgErrEvent.cancel();
}//}}}
//------------------------------------------------------------------------------
/**@brief Reset model and its sub-instance if reset is active
 * @param is_active
 */
void IBG_U2::EnableReset(const bool isActive) 
{//{{{
    pFuncModel->EnableReset(isActive);
    if(true == isActive) {
        CancelEvents();
        Initialize();
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief if PRESET reset is active then enable PRESETn reset
 * @param is_active
 */
void IBG_U2::EnablePRESETnReset(const bool isActive) 
{//{{{
    pFuncModel->EnablePRESETnReset(isActive);
}//}}}
//------------------------------------------------------------------------------
/**@brief check status of the combination of hard reset and command reset of resetPort is active or not
 * @return true if at least one reset source is active
 */
bool IBG_U2::GetResetStatus (const std::string resetName) 
{//{{{
    bool resetStatus = false;
    if (resetName == resetPort.basename()) {
        resetStatus = (mIsResetHardActive || mIsResetCmdActive);
    }
    return resetStatus;
}//}}}
//------------------------------------------------------------------------------
/**@brief adjust the resolution of time
 * @param[inout] resolution_value adjusted value of time resolution
 * @param[inout] resolution_unit adjusted value of unit of resolution
 */
void IBG_U2::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit) 
{//{{{
    if (sc_get_time_resolution() < sc_time(1, SC_PS)) {
        resolution_value = 1000000000000000LL;
        resolution_unit = SC_FS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_NS)) {
        resolution_value = 1000000000000LL;
        resolution_unit = SC_PS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_US)) {
        resolution_value = 1000000000;
        resolution_unit = SC_NS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_MS)) {
        resolution_value = 1000000;
        resolution_unit = SC_US;
    } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)) {
        resolution_value = 1000;
        resolution_unit = SC_MS;
    } else {
        resolution_value = 1;
        resolution_unit = SC_SEC;
    }
}//}}}
//##############################################################################
// Part 3: PythonIF
//##############################################################################
//------------------------------------------------------------------------------
/**@brief Assert and deassert reset signal
 * @param reset_name std::string <reset_name>: name of reset signal ("resetPort" or "PRESETn")
 * @param start_time double <start_time>: the time until asserting reset signal from current time. The unit is "ns"
 * @param period double <period>: the time from asserting reset signal to de-assert it. The unit is "ns"
 */
void IBG_U2::AssertReset(const std::string reset_name, const double start_time, const double period) 
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
    else if (reset_name == PRESETn.basename()) {
        if ((!mIsPRESETnHardActive) && (!mIsPRESETnCmdReceive)) {
            mIsPRESETnCmdReceive = true;
            re_printf("info","The model will be reset (%s) for %f ns after %f ns.\n", PRESETn.basename(), period, start_time);
            mPRESETnCmdEvent.notify(start_time, SC_NS);
            mPRESETnCmdPeriod = period;
        }
        else {
            re_printf("warning", "The software reset of %s is called in the reset operation of the model. So it is ignored.\n", PRESETn.basename());
        }
    }
    else {
        re_printf("warning","The reset name (%s) is wrong. It should be %s or %s.\n", reset_name.c_str(), resetPort.basename(), PRESETn.basename());
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief Set frequency value to these blocks
 * @param clock_name std::string <clock_name>: name of clock signal
 * @param freq sc_dt::uint64 <freq>: clock frequency
 * @param unit std::string <unit>: frequency unit ("Hz", "KHz", "MHz" or "GHz")
 */
void IBG_U2::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit) 
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
                mPCLKZeroClkEvent.notify(SC_ZERO_TIME);
            }
        }
        else if (word_vector[0] == cpu_clk.basename()) {
            mcpu_clkOrgFreq = freq;
            ConvertClockFreq (mcpu_clkFreq, mcpu_clkFreqUnit, freq, unit);
            if (mcpu_clkFreq > 0) {
                mcpu_clkPeriod = (sc_dt::uint64)(((1/(double)mcpu_clkFreq)*(double)mTimeResolutionValue) + 0.5);
                mcpu_clkSyncTPoint = sc_time_stamp().to_double();
            } else {
                mcpu_clkPeriod = 0;
                mcpu_clkSyncTPoint = sc_time_stamp().to_double();
                mcpu_clkZeroClkEvent.notify(SC_ZERO_TIME);
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
void IBG_U2::GetCLKFreq(const std::string clock_name) 
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == PCLK.basename()) {
            re_printf("info","%s frequency is %0.0f %s\n", PCLK.basename(), (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        }
        else if (word_vector[0] == cpu_clk.basename()) {
            re_printf("info","%s frequency is %0.0f %s\n", cpu_clk.basename(), (double)mcpu_clkOrgFreq, mcpu_clkFreqUnit.c_str());
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
void IBG_U2::ForceRegister (const std::string reg_name, const unsigned int reg_value) 
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
void IBG_U2::ReleaseRegister (const std::string reg_name) 
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
void IBG_U2::WriteRegister (const std::string reg_name , const unsigned int reg_value) 
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
void IBG_U2::ReadRegister (const std::string reg_name) 
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
void IBG_U2::ListRegister (void) 
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    pFuncModel->RegisterHandler(cmd);
}//}}}
//------------------------------------------------------------------------------
/**@brief Dump help message of all parameters or commands
 */
void IBG_U2::Help (const std::string type) 
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"IBG_U2_MessageLevel (fatal|error|warning|info) Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"IBG_U2_DumpRegisterRW (true/false) Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"IBG_U2_DumpInterrupt (true/false) Enable/disable dumping interrupt information (Default: false)");
        }
        else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"IBG_U2_AssertReset (reset_name, start_time, period) Assert and de-assert reset signal to the IBG_U2 model");
            SC_REPORT_INFO(this->basename(),"IBG_U2_SetCLKFreq (clock_name, freq, unit) Set clock frequency value to IBG_U2 model");
            SC_REPORT_INFO(this->basename(),"IBG_U2_GetCLKFreq (clock_name) Get clock frequency value of IBG_U2 model");
            SC_REPORT_INFO(this->basename(),"IBG_U2_ForceRegister (unit_name, reg_name, reg_value) Force register with setting value");
            SC_REPORT_INFO(this->basename(),"IBG_U2_ReleaseRegister (unit_name, reg_name) Release register from force value");
            SC_REPORT_INFO(this->basename(),"IBG_U2_WriteRegister (unit_name, reg_name, reg_value) Write a value to a register");
            SC_REPORT_INFO(this->basename(),"IBG_U2_ReadRegister (unit_name, reg_name) Read a value from a register");
            SC_REPORT_INFO(this->basename(),"IBG_U2_ListRegister () Dump name of all registers");
            SC_REPORT_INFO(this->basename(),"IBG_U2_SetAreaAddr (channelId, start_addr, size) Set protected address range");
            SC_REPORT_INFO(this->basename(),"IBG_U2_DumpStatusInfo () Dump information of the status registers of the IBG_U2 model");
        } else {
            re_printf("warning","The name (%s) of IBG_U2_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of IBG_U2_Help argument is wrong (commands or parameters)\n", type.c_str());
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
void IBG_U2::SetMessageLevel (const std::string msg_lv) 
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(IBG_U2::basename(), msg_return.c_str());
    }

    std::string msg_ret = this->handleCommand(cmd);
    if (msg_ret != "") {
        SC_REPORT_INFO(this->basename(), msg_ret.c_str());
    }

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
void IBG_U2::DumpRegisterRW (const std::string is_enable) 
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
void IBG_U2::DumpInterrupt (const std::string is_enable) 
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: IBG_U2_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = (mDumpInterrupt) ? "true" : "false";
        re_printf("info","IBG_U2_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: IBG_U2_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief Separate a string to a vector of separated sub-strings
 * @details
 *  This is helper function called in Python command processing.
 * @note
 *  This function is NOT intend to be changed, keep this unchanged
 * @param[out] vtr output vector of separated sub-strings
 * @param[in] msg input message
 */
void IBG_U2::SeparateString (std::vector<std::string> &vtr, const std::string msg) 
{//{{{
    std::istringstream cmd;
    cmd.str(msg);
    std::string temp = "a"; // this operation makes temp to be not empty
    while (temp != "") {
        std::string temp1;
        cmd >> temp1;
        if (temp1 != "") {
            vtr.push_back(temp1);
        }
        temp = temp1;
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief Check frequency value and frequency unit
 * @details
 *  This is helper function called in Python command processing.
 * @note
 *  This function is NOT intend to be changed, keep this unchanged
 * @param[out] freq_out value of output frequency
 * @param[out] unit_out unit of output frequency
 * @param[in] freq_in value of frequency
 * @param[in] unit_in unit of the input frequency
 */
void IBG_U2::ConvertClockFreq(sc_dt::uint64 &freq_out, std::string &unit_out, sc_dt::uint64 freq_in, std::string unit_in) 
{//{{{
    if (unit_in == "GHz") {
        freq_out = freq_in * (sc_dt::uint64) (1.0e+9);
        unit_out = "GHz";
    } else if (unit_in == "MHz") {
        freq_out = freq_in * (sc_dt::uint64) (1.0e+6);
        unit_out = "MHz";
    } else if (unit_in == "KHz") {
        freq_out = freq_in * (sc_dt::uint64) (1.0e+3);
        unit_out = "KHz";
    } else if (unit_in == "Hz") {
        freq_out = freq_in;
        unit_out = "Hz";
    } else { // Frequency unit is wrong, frequency unit is set as Hz default.
        freq_out = freq_in;
        unit_out = "Hz";
        re_printf("warning","Frequency unit (%s) is wrong, frequency unit is set as unit Hz default.\n", unit_in.c_str());
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief print out value of boolean signal interrupt
 * @param intr_name name of the interrupt
 * @param value current value of interrupt signal
 */
void IBG_U2::DumpInterruptMsg (const std::string intr_name, const bool value)
{//{{{
    if (mDumpInterrupt) {
        if (true == value) {
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
/// @brief process asserts the *ibgerr* output according to clock edge
void IBG_U2::AssertIbgErrMethod() 
{//{{{
    ibgerr_val = true;
    mUpdateIbgErrEvent.notify(SC_ZERO_TIME);
    mDeassertIbgErrEvent.notify(CalculateClockEdge(cpu_clk.basename(), true, sc_time_stamp().to_double()) + mcpu_clkPeriod, mTimeResolutionUnit);
}//}}}
//------------------------------------------------------------------------------
/// @brief process deasserts the *ibgerr* output according to clock edge
void IBG_U2::DeassertIbgErrMethod() 
{//{{{
    ibgerr_val = false;
    mUpdateIbgErrEvent.notify(SC_ZERO_TIME);
}//}}}
//------------------------------------------------------------------------------
/// @brief process deasserts the *ibgerr* output according to clock edge
void IBG_U2::UpdateIbgErrMethod() 
{//{{{
    ibgerr.write(ibgerr_val);
    DumpInterruptMsg(ibgerr.basename(), ibgerr_val);
}//}}}
//------------------------------------------------------------------------------
// implement common guard agent controller interface
//------------------------------------------------------------------------------
/**@brief Check if period value of the clock clocking the bus guard is greater than 0 or not
 * @return true if the clock under check is greater than 0
 */
bool IBG_U2::CheckGuardClock (std::string clock_name) 
{//{{{
    bool clockStatus = false;
    if (clock_name == "PCLK") {
        clockStatus = CheckClockPeriod(PCLK.basename()); 
    }
    else if (clock_name == "cpu_clk") {
        clockStatus = CheckClockPeriod(cpu_clk.basename());
    }
    return clockStatus;
}//}}}
//------------------------------------------------------------------------------
/**@brief check status of the combination of hard reset and command reset
 *        that controls the bus guard is active or not
 * @return true if at least one reset source under check is active
 */
bool IBG_U2::GetGuardResetStatus() 
{//{{{
    return GetResetStatus(resetPort.basename());
}//}}}
//------------------------------------------------------------------------------
/**@brief function allows the bus guard to report diagnosis message for debug
 * @param[in] severity level of severity of the message
 * @param[in] msg the message
 */
void IBG_U2::printMsg(const char* severity, const char* msg) 
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
bool IBG_U2::SetAreaAddr(const uint32_t    channelId
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
bool IBG_U2::SetAreaAddr(const string      groupName
            ,const uint32_t    channelId
            ,const uint32_t    start_addr
            ,const uint32_t    size) 
{//{{{
    /// -# validate the given channel Id
    if (channelId >= chanNum) {
        if (mGroupName == "IBG_U2_IPIG") {
            if (channelId == 8) {
                if (!pBusGuard->SetAreaAddr(groupName, channelId, start_addr, size)) {
                    return false;
                }
                    return true;
            }
            else {
                re_printf("warning", "the given channel id is invalid\n");
                return false;
            }
        }
        else if (mGroupName == "IBG_U2_BRG") {
            if (channelId == 16) {
                if (!pBusGuard->SetAreaAddr(groupName, channelId, start_addr, size)) {
                    return false;
                }
                    return true;
            }
            else {
                re_printf("warning", "the given channel id is invalid\n");
                return false;
            }
        }
        else if (mGroupName == "IBG_U2_TPTG") {
            if (channelId == 8 || channelId == 9) {
                if (!pBusGuard->SetAreaAddr(groupName, channelId, start_addr, size)) {
                    return false;
                }
                return true;
            }
            else {
                re_printf("warning", "the given channel id is invalid\n");
                return false;
            }
        }
    }
    
    /// -# register the given address range into the bus guard
    if (!pBusGuard->SetAreaAddr(groupName, channelId, start_addr, size)) {
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
void IBG_U2::TransferErrInfo (const string    groupName
                ,const uint32_t  channelId
                ,const uint32_t  addr
                ,const bool      isDebug
                ,const bool      isUserMode
                ,const bool      isWrite
                ,const uint8_t   SPID
                ) 
{//{{{  
    //Update error transaction info
    pFuncModel->UpdateErrorTransInfo(addr, isDebug, isUserMode, isWrite, SPID);                                
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
bool IBG_U2::GetGuardSettings(const string   groupName
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
//------------------------------------------------------------------------------
/**@brief notify error to ECM
 */
void IBG_U2::NotifyError()
{//{{{
    mAssertIbgErrEvent.notify(CalculateClockEdge(cpu_clk.basename()));
}//}}}
//------------------------------------------------------------------------------
/**@brief get value of GOVFSTAT register 
 */
uint32_t IBG_U2::GetGOVFSTAT()
{//{{{
    return pFuncModel->GetGOVFSTAT();
}//}}}
//------------------------------------------------------------------------------
/**@brief get value of GSPIDERRSTAT register 
 */
uint32_t IBG_U2::GetGSPIDERRSTAT()
{//{{{
    return pFuncModel->GetGSPIDERRSTAT();
}//}}}
//------------------------------------------------------------------------------
/**@brief dump status info of registers
 */
void IBG_U2::DumpStatusInfo()
{//{{{
    std::string type = "StatInfo";
    printf("PROFILE(%s): IBG_U2: Info [%20s] (%10s):\n", type.c_str(), sc_time_stamp().to_string().c_str(), this->name());
    printf("PROFILE(%s): IBG_U2:   GOVFSTAT bits: 0x%08X\n", type.c_str(), this->GetGOVFSTAT());
    printf("PROFILE(%s): IBG_U2:   GSPIDERRSTAT bits: 0x%08X\n", type.c_str(), this->GetGSPIDERRSTAT());
}//}}}
//------------------------------------------------------------------------------
/**@brief get channel number
 */
uint8_t IBG_U2::GetChannelNum()
{//{{{
    return chanNum;
}//}}}
std::string IBG_U2::GetGroupName()
{//{{{
    return mGroupName;
}//}}}
// vim600: set foldmethod=marker :
