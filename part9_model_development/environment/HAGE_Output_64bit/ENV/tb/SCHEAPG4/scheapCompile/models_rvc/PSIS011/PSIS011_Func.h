// ---------------------------------------------------------------------
// $Id: PSIS011_Func.h,v 1.5 2017/12/16 09:57:23 chanle Exp $
//
// Copyright(c) 2017 Renesas Electronics Corporation
// Copyright(c) 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __PSIS011_FUNC_H__
#define __PSIS011_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "PSIS011_Ch_Reg.h"
#include "PSIS011_Cmn_Reg.h"
#include "PSIS011_AgentController.h"

class PSIS011_Func: public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (PSIS011_Func);
    PSIS011_Func(std::string name, PSIS011_AgentController* PSIS011AgentController);
    ~PSIS011_Func();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::string reg_area, const unsigned int chid, const std::vector<std::string> &args);
    void SoftwareReset();
    void ClearAllMailBoxData();
    void StopGenOutputClock();
    sc_dt::uint64 GetPCLKFreq();
    unsigned int GetACSTSBit();
    unsigned int GetMSTSBit();
    void ClearTimestamp(std::string timestamp_name);
    void ConfigTimestamp(std::string timestamp_name, bool clear_select_gtm, bool enable_select_gtm, bool clock_select_gtm);
    void SetTSInnerCycle(unsigned int inner_cycle);
    void InformChangeMode(unsigned int open, unsigned int opmd);
    void GetConfigInnerSyncPulse(unsigned int chid, unsigned int &select, unsigned int &prescaler, unsigned int &expired);
    void GetConfigWDT(unsigned int chid, unsigned int &syncmode, unsigned int &prescaler, unsigned int &expired, bool &wdt_enable);
    void GetConfigTimestamp(unsigned int chid, unsigned int &ch_en, unsigned int &ts_sel, unsigned int &ts_en, unsigned int &ts_trg_sel);
    unsigned int GetPSI5SPUPTSReg();
    void GetClockDivider(unsigned int &rx_divider, unsigned int &tx_divider);
    unsigned int GetMaxIdle();
    void CaptureTimestamp(unsigned int chid, unsigned int timestamp);
    void StoreMBData(unsigned int chid, unsigned int frame, unsigned int status_reg_val,
                unsigned int data_reg_val, unsigned int timestamp_reg_val);
    void StoreUARTData(unsigned int data, bool framing_error, bool parity_error);
    unsigned int GetRFCPSBit(unsigned int chid);
    void GetConfigPSI5RxFrame(unsigned int chid, unsigned int fid, unsigned int& fpkt, unsigned int& fpayload, unsigned int& rfcps);
    bool CheckMBDataWasRead(unsigned int chid, unsigned int frmid);
    unsigned int GetCapturedTimestamp(unsigned int chid);
    void SetTSInnerEnable(std::string timestamp_name, bool ts_inner_enable);
    void GetDDSRInfo(unsigned int chid, unsigned int &frame_type, unsigned int &address, unsigned int &data);
    void GetConfigCPUTxCom(unsigned int &numfrm, sc_dt::uint64 &tx_cpu_com_data);
    unsigned int GetCommandData(unsigned int chid);
    void NotifySendReqTxToArbiter(unsigned int reqid);
    void NotifyTransUART();
    unsigned int GetUTTDTBit();
    void SetTXSTSBit(unsigned int value);
    void ClearDDSRSTSBit(unsigned int chid);
    void AssertInterrupt(unsigned int intrid);
    void AssertDMARequestRX(unsigned int dmarxid);
    void AssertDMARequestTX(unsigned int dmatxid);
    void ChangeWDTEnableChannel(unsigned int chid, unsigned int enable);
    void InformChangeSCKENBit(unsigned int value);
    unsigned int GetNFSETBit();
    void SetPSI5SPCISReg(unsigned int chid, std::string status);
    void UpdatePSI5SUCTMReg(std::string bit_name, unsigned int value);
    void UpdateUTTFINBit(unsigned int value);
    void DumpStatusInfo();
    void SetPSI5SPTFISReg(unsigned int value);
    void SetPSI5SPRESReg(unsigned int chid, unsigned int frmid);
    void StopTransDDSR(unsigned int txreqid);
    void SetSWSTSBit(unsigned int value);
    bool GetLoopbackEnable();

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    // enum
    enum eRegInfo{
        emPSI5SPUCLBAddress         = 0x20
        ,emBaseOffsetCh0            = 0x100
        ,emBaseOffsetMBCh0          = 0x500
        ,emBaseOffsetReservedArea   = 0x740
        ,emSizeEachChannel          = 0x80
        ,emSizeEachMBChannel        = 0x48
    };
    enum eGeneral{
        emNumChannel = 8
    };

    // Variables
    PSIS011_AgentController *mpPSIS011AgentController;  // PSIS011 Agent controller pointer
    std::string             mModuleName;                // Module name
    bool                    mIsReset;                   // Indicate this object is reset by PRESETn
    unsigned int            mPreOpenBit;                // Indicate the previous value of OPEN bit
    PSIS011_Ch_Reg          *mpPSIS011ChReg[emNumChannel];  // 8 "PSIS011 registers channel" pointers
    PSIS011_Cmn_Reg         *mpPSIS011CmnReg;           // 1 "PSIS011 registers common" pointer

    // Function
    std::string FindRegAreaFromAddr(const unsigned int address);

};
#endif //__PSIS011_FUNC_H__
