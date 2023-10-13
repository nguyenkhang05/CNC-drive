// ---------------------------------------------------------------------
// $Id: PEG_LT.h,v 1.7 2015/06/12 08:19:58 dungvannguyen Exp $
//
// Copyright(c) 2014-2015 Renesas Electronics Corporation
// Copyright(c) 2014-2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __PEG_LT_H__
#define __PEG_LT_H__
#include "BusBridgeModuleBase.h"
#include "BusBridgeSlaveBase.h"
#include "BusMasterBase.h" 
#include "BusSlaveBase.h"
#include "PEG_AgentController.h"
#include "ERR_INFO.h"
#include "SysErrVCI.h"

template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusBridgeSlaveBase;
template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusMasterBase;
template<unsigned int BUSWIDTH> class TlmTargetSocket;
template<unsigned int BUSWIDTH> class TlmInitiatorSocket;
class PEG_DataHandler;
class PEGUARD;

class PEG : public PEG_AgentController,
            public BusBridgeModuleBase<32,32>,
            public BusBridgeSlaveBase<32,1>,
            public BusMasterBase<32,1>,
            public BusSlaveBase<32,1>
{
public:
    SC_HAS_PROCESS(PEG);
    PEG(sc_module_name name, 
        unsigned int rLatency,
        unsigned int wLatency,
        unsigned int PEG_ID = 1,
        std::string PEG_VERSION = "G3M_V30");
    ~PEG(void);

    void SetAreaAddress (const std::string area_name, const unsigned int start_addr, const unsigned int size, const std::string pe_name);

    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);
    void SetAccessMode (const std::string mode, const std::string is_enable);
    void SetG0InitialVal (const std::string bit_name, const unsigned int value);
    void Help (const std::string type);
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);

    sc_in<uint64> pclk;
    sc_in<uint64> lvcclk;
    sc_in<bool> presetz;
    sc_in<bool> lvcreset;
    sc_out<Cerrinfo> PEG_ERR;
    sc_out<Csyserrvci> VciErrInfo;
    sc_signal<bool> mInternalRstSig;

    TlmTargetSocket<32> *tsp;
    TlmTargetSocket<32> *tsv;
    TlmInitiatorSocket<32> *is;
    
private:
    enum eVersion {
        emCC_CUBE = 0,
        emOTHERS = 1
    };
    enum eResetActiveLevel {
        emPresetzActive = 0,
        emPresetzInactive = 1,
        emLvcresetActive = 1,
        emLvcresetInactive = 0
    };

    PEGUARD *mPEGUARD;

    sc_dt::uint64 mPclkPeriod;//Period value of Pclk clock
    sc_dt::uint64 mLvcclkPeriod;//Period value of Lvcclk clock
    sc_dt::uint64 mPclkFreq;//Frequency value of Pclk clock
    sc_dt::uint64 mLvcclkFreq;//Frequency value of Lvcclk clock
    sc_dt::uint64 mPclkOrgFreq;//Orginal frequency value of Pclk clock
    sc_dt::uint64 mLvcclkOrgFreq;//Orginal frequency value of Lvcclk clock
    std::string mPclkFreqUnit;//Frequency unit of Pclk clock
    std::string mLvcclkFreqUnit;//Frequency unit of Lvcclk clock
    sc_dt::uint64 mTimeResolutionValue;//Time resolution value of simulation
    sc_time_unit mTimeResolutionUnit;//Time resolution unit of simulation

    bool mIsStartSimulation;
    std::string mVersion; 

    Cerrinfo mPEG_ERRValue;
    Csyserrvci mVciErrInfoValue;
    bool mIsAreaNumberSet;
    unsigned int mNumberOfArea;
    std::vector<std::string> mNameOfSetArea;
    unsigned int mAreaCounter;

    bool mPresetzSignalCurrentValue;//Store current value of Presetz signal
    bool mPresetzSignalPreviousValue;//Store previous value of Presetz signal
    bool mIsPresetzHardActive;//Reset operation status of Presetz signal
    bool mIsPresetzCmdReceive;//Receive status of AssertReset command of Presetz signal
    bool mIsPresetzCmdActive;//Active status of AssertReset command of Presetz signal
    double mPresetzCmdPeriod;//Period of AssertReset command of Presetz signal

    bool mLvcresetSignalCurrentValue;//Store current value of Lvcreset signal
    bool mLvcresetSignalPreviousValue;//Store previous value of Lvcreset signal
    bool mIsLvcresetHardActive;//Reset operation status of Lvcreset signal
    bool mIsLvcresetCmdReceive;//Receive status of AssertReset command of Lvcreset signal
    bool mIsLvcresetCmdActive;//Active status of AssertReset command of Lvcreset signal
    double mLvcresetCmdPeriod;//Period of AssertReset command of Lvcreset signal

    sc_event mPresetzHardEvent;//Call HandlePresetzHardMethod() when Presetz is active
    sc_event mPresetzCmdEvent;//Call HandlePresetzCmdMethod() when AssertReset of Presetz is called
    sc_event mPresetzCmdCancelEvent;//Call CancelPresetzCmdMethod() when AssertReset of Presetz is over

    sc_event mLvcresetHardEvent;//Call HandleLvcresetHardMethod() when Lvcreset is active
    sc_event mLvcresetCmdEvent;//Call HandleLvcresetCmdMethod() when AssertReset of Lvcreset is called
    sc_event mLvcresetCmdCancelEvent;//Call CancelLvcresetCmdMethod() when AssertReset of Lvcreset is over

    sc_event mUpdatePEG_ERREvent;//Call UpdatePEG_ERRMethod() when the model control error output signal
    sc_event mClearPEG_ERREvent;//Call ClearPEG_ERRMethod() when the model clear error output signal
    sc_event mWritePEG_ERREvent;//Call WritePEG_ERRMethod() when the model write the value to error output signal
    sc_event mUpdateVciErrInfoEvent;//Call UpdateVciErrInfoMethod() when the model control VciErrInfo signal
    sc_event mClearVciErrInfoEvent;//Call ClearVciErrInfoMethod() when the model clear VciErrInfo signal
    sc_event mWriteVciErrInfoEvent;//Call WriteVciErrInfoMethod() when the model write the value to VciErrInfo signal

    void HandlePclkSignalMethod (void);
    void HandleLvcclkSignalMethod (void);
    void HandlePresetzSignalMethod (void);
    void HandleLvcresetSignalMethod (void);
    void HandlePresetzHardMethod (void);
    void HandleLvcresetHardMethod (void);
    void HandlePresetzCmdMethod (void);
    void HandleLvcresetCmdMethod (void);
    void CancelPresetzCmdMethod (void);
    void CancelLvcresetCmdMethod (void);

    void UpdatePEG_ERRMethod(void);
    void ClearPEG_ERRMethod(void);
    void WritePEG_ERRMethod(void);
    void UpdateVciErrInfoMethod(void);
    void ClearVciErrInfoMethod(void);
    void WriteVciErrInfoMethod(void);

    void start_of_simulation(void);
    void ControlOutput(void);
    void EnableReset (const std::string reset_name, const bool is_active);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                           sc_dt::uint64 freq_in, std::string unit_in);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    double CalculateCLKPosEdge (const std::string clock_name, const double time_point);

    // virtual functions of AgentController class
    bool CheckAccessPermission(TlmBasicPayload &trans);
    bool CheckClock(const std::string clock_name);
    bool GetResetStatus(void);
};
#endif
