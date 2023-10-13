// ---------------------------------------------------------------------
// $Id: AHG.h,v 1.5 2014/03/03 06:28:41 dungvannguyen Exp $
//
// Copyright(c) 2012 Renesas Electronics Corporation
// Copyright(c) 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __AHG_H__
#define __AHG_H__
#include "BusBridgeModuleBase.h"
#include "BusBridgeSlaveBase.h"
#include "BusMasterBase.h" 
#include "BusSlaveBase.h"
#include "AHBFSGuard.h" 
#include "AHBERRSLV.h" 
#include "AhgAgentController.h"

template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusBridgeSlaveBase;
template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusMasterBase;
template<unsigned int BUSWIDTH> class TlmTargetSocket;
template<unsigned int BUSWIDTH> class TlmInitiatorSocket;
 
class AhgDataHandler;

class AHG : private AhgAgentController,
            public BusBridgeModuleBase<32,32>,
            public BusBridgeSlaveBase<32,1>,
            public BusMasterBase<32,1>,
            public BusSlaveBase<32,2>
{
public:
    SC_HAS_PROCESS(AHG);
    AHG(sc_module_name name, 
        unsigned int rLatency,
        unsigned int wLatency,
        unsigned int AHG_VERSION = 0,
        bool P_LOCK_SUPPORT = true, //LOCK bit function is support
        unsigned int P_FILTER_NO = 1); // Filter number is 1
    ~AHG(void);

    void SetERRSLVPointer(AHBERRSLV *ptr);
    AHBERRSLV* GetERRSLVPointer(void);

    void AssertReset(const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit);
    sc_dt::uint64 GetCLKFreq(const std::string clock_name);
    void SetFilterInfo(const std::string is_set, const unsigned int id, 
                       const unsigned int start_addr, const long long size, const std::string property);
    void SetMessageLevel(const std::string msg_lv);
    void DumpInterrupt(const std::string is_enable);
    void DumpStatusInfo(const std::string block_name);
    void DumpRegisterRW(const std::string is_enable);
    void DumpFileNameLineNum(const std::string is_enable);
    void ForceRegister(const std::string reg_name, const std::string reg_value);
    void ReleaseRegister(const std::string reg_name);
    void AccessRegister(const std::string reg_name, const std::string reg_value);
    void ListRegister(const std::string block_name);
    void SetRegInitVal(const unsigned int filter_id, const std::string reg_name, const unsigned int value);
    void Help(const std::string type);

    sc_in<uint64> PCLK;
    sc_in<uint64> HCLK;
    sc_in<bool> HRESETZ;
    sc_out<bool> INTAHB;
    TlmTargetSocket<32> *tsg;
    TlmTargetSocket<32> *tss;
    TlmTargetSocket<32> *tse;
    TlmInitiatorSocket<32> *iss;
    
private:
    enum eVersion {
        emCC_CUBE = 0,
        emOTHERS = 1
    };
    enum eResetActiveLevel {
        emResetActive = 0,
        emResetInactive = 1
    };

    AHBFSGuard *mAHBFSGuard;
    AHBERRSLV *mAHBERRSLV;
    AHBERRSLV *mAHBERRSLV_Org;
    AhgDataHandler *mAhgDataHandler;

    sc_event mErrorAccessIgnoreEvent;
    sc_event mStatusClearEvent;
    sc_event mHandleErrorInfoEvent;
    sc_event mWriteIntrEvent;
    sc_event mClearIntrEvent;
    sc_event mHRESETZHardEvent;
    sc_event mPRESETZHardEvent;
    sc_event mHRESETZCmdEvent;
    sc_event mPRESETZCmdEvent;
    sc_event mCancelHRESETZCmdEvent;
    sc_event mCancelPRESETZCmdEvent;
    sc_event mTransferCompleteEvent;
   
    unsigned int mVersion; 
    bool mLockSupport;
    unsigned int mFilterNumber;
    sc_dt::uint64 mPCLKPeriod;
    sc_dt::uint64 mHCLKPeriod;
    sc_dt::uint64 mPCLKFreq;
    sc_dt::uint64 mPCLKOrgFreq;
    sc_dt::uint64 mHCLKFreq;
    sc_dt::uint64 mHCLKOrgFreq;
    sc_dt::uint64 mTimeUnitValue;
    sc_time_unit mTimeUnit;
    std::string mPCLKFreqUnit;
    std::string mHCLKFreqUnit;
    bool mIntrValue;
    bool mDumpInterrupt;

    bool mPRESETZValue;
    bool mHRESETZValue;
    bool mIsHRESETZHardActive;
    bool mIsPRESETZHardActive;
    bool mIsHRESETZCmdActive;
    bool mIsPRESETZCmdActive;
    bool mIsHRESETZCmdReceive;
    bool mIsPRESETZCmdReceive;
    double mCmdHRESETZResetPeriod;
    double mCmdPRESETZResetPeriod;
    bool mIsConstructPRESETZ;
    bool mIsConstructHRESETZ;
    bool mErrorAccessIgnore;
    bool mStatusClearIgnore;
    bool mClearOnlyOVF;

    void HandlePCLKMethod(void);
    void HandleHCLKMethod(void);
    void ReceivePRESETZHardMethod(void);
    void ReceiveHRESETZHardMethod(void);
    void HandlePRESETZHardMethod(void);
    void HandleHRESETZHardMethod(void);
    void HandlePRESETZCmdMethod(void);
    void HandleHRESETZCmdMethod(void);
    void CancelPRESETZCmdMethod(void);
    void CancelHRESETZCmdMethod(void);
    void WriteINTAHBMethod(void);
    void ClearINTAHBMethod(void);
    void HandleErrorInfoMethod(void);
    void ErrorAccessIgnoreMethod(void);
    void StatusClearMethod(void);
    void NotifyTranferCompleteMethod(void);

    void CheckFreq(sc_dt::uint64 &freq_out, std::string &unit_out, sc_dt::uint64 freq_in, std::string unit_in);
    double PosEdgeCalc(const std::string clock_name, const bool add_period, const double time_point);
    void EnableHRESETZ(void);
    void EnablePRESETZ(bool is_active);
    void SeparateString(std::vector<std::string> &vtr, const std::string msg);
    void GetResolutionTime(sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    void ControlIntr(void);
    void WaitPosEdgeClk(void);
    // virtual functions of AgentController class
    bool CheckClock(std::string clock_name);
    bool CheckAccessPermission(TlmBasicPayload &trans, const bool debug, bool &is_legal_trans);
    void TransferErrInfo(TlmBasicPayload &trans);
    void WaitTimeToClearErrorFlags(bool ovf_flag);
    bool GetResetStatus(void);
    void NotifyTransComp(void);
};
#endif
