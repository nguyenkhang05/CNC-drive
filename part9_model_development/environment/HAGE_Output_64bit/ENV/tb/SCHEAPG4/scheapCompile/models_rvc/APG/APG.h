// ---------------------------------------------------------------------
// $Id: APG.h,v 1.11 2014/03/03 06:33:04 dungvannguyen Exp $
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

#ifndef __APG_H__
#define __APG_H__
#include "BusBridgeModuleBase.h"
#include "BusBridgeSlaveBase.h"
#include "BusMasterBase.h" 
#include "BusSlaveBase.h"
#include "APBFSGuard.h" 
#include "APBERRSLV.h" 
#include "ApgAgentController.h"

template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusBridgeSlaveBase;
template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusMasterBase;
template<unsigned int BUSWIDTH> class TlmTargetSocket;
template<unsigned int BUSWIDTH> class TlmInitiatorSocket;
 
class ApgDataHandler;

class APG : private ApgAgentController,
            public BusBridgeModuleBase<32,32>,
            public BusBridgeSlaveBase<32,1>,
            public BusMasterBase<32,1>,
            public BusSlaveBase<32,2>
{
public:
    SC_HAS_PROCESS(APG);
    APG(sc_module_name name, 
        unsigned int rLatency,
        unsigned int wLatency,
        unsigned int APG_VERSION = 0,
        bool P_LOCK_SUPPORT = true, //LOCK bit function is support
        unsigned int P_FILTER_NO = 1); //Filter number is 1
    ~APG(void);

    void SetERRSLVPointer(APBERRSLV *ptr);
    APBERRSLV* GetERRSLVPointer(void);

    void AssertReset(const double start_time, const double period);
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
    sc_out<bool> INTAPB;
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

    APBFSGuard *mAPBFSGuard;
    APBERRSLV *mAPBERRSLV;
    APBERRSLV *mAPBERRSLV_Org;
    ApgDataHandler *mApgDataHandler;

    sc_event mHandleErrorInfoEvent;
    sc_event mWriteIntrEvent;
    sc_event mClearIntrEvent;
    sc_event mResetEvent;
    sc_event mCmdResetEvent;
    sc_event mCancelCmdResetEvent;
    sc_event mTransferCompleteEvent;
   
    unsigned int mVersion; 
    bool mLockSupport;
    unsigned int mFilterNumber;
    sc_dt::uint64 mClkPeriod;
    sc_dt::uint64 mTimeUnitValue;
    sc_time_unit mTimeUnit;
    sc_dt::uint64 mClkFreq;
    sc_dt::uint64 mClkOrgFreq;
    std::string mClkUnit;
    bool mIntrValue;
    bool mDumpInterrupt;

    bool mRstPortValue;
    bool mIsHardResetActive;
    bool mIsCmdResetActive;
    bool mIsAssertResetReceive;
    double mCmdResetPeriod;
    bool mIsConstruct;

    void HandlePCLKMethod(void);
    void HandleResetPortMethod(void);
    void ResetMethod(void);
    void CmdResetMethod(void);
    void CancelCmdResetMethod(void);
    void WriteINTAPBMethod(void);
    void ClearINTAPBMethod(void);
    void NotifyTranferCompleteMethod(void);
    void HandleErrorInfoMethod(void);

    bool CheckPCLKClock(void);
    double PosEdgeCalc(const bool is_reset, const double time_point);
    void EnableRst(const bool is_active);
    void SeparateString(std::vector<std::string> &vtr, const std::string msg);
    void GetResolutionTime(sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    void ControlIntr(void);
    void WaitPosEdgeClk(void);
    // virtual functions of AgentController class
    bool CheckAccessPermission(TlmBasicPayload &trans, const bool debug, bool &is_legal_trans);
    void TransferErrInfo(TlmBasicPayload &trans);
    bool GetResetStatus(void);
    void NotifyTransComp(void);
};
#endif
