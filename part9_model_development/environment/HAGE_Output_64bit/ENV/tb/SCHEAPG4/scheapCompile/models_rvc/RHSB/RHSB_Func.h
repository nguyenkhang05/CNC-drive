// ---------------------------------------------------------------------
// $Id: RHSB_Func.h,v 1.3 2018/08/11 07:54:56 ngathunguyen Exp $
//
// Copyright(c) 2016 - 2018 Renesas Electronics Corporation
// Copyright(c) 2016 - 2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __RHSB_FUNC_H__
#define __RHSB_FUNC_H__
#include <queue>
#include "BusSlaveFuncIf.h"
#include "rhsb_regif.h"
#include "RHSB_define.h"

using namespace rhsb_def;

class RHSB;

class RHSB_Func: public Crhsb_regif
                ,public BusSlaveFuncIf
{
public:
    RHSB_Func (std::string name, RHSB *RHSB_ptr);
    ~RHSB_Func (void);

    void RegisterHandler (const std::vector<std::string>& args);
    void EnableReset (const bool is_active);
    std::string GetCurrentState (void);
    bool IsDataTransmissionEnable (void);
    bool IsUpstreamEnable (void);
    bool IsTimeOutEnable (void);
    bool IsDedicatedUpstreamMode (void);
    bool IsTriggerDownstreamMode (void);
    bool IsDataAvailable (void);
    bool IsMultiPeriodDownstreamMode (void);
    bool IsCommandRequestPending (void);
    bool IsTransmissionStop (void);
    bool IsTimeSlotMethod (void);
    bool IsImmediateMethod (void);
    bool IsCommandFrameTimeOverTimeStick (void);
    bool IsInterruptStatusActive (const std::string bit_name);
    bool IsInterruptEnable (const std::string bit_name);
    bool IsDownstreamCommandDMAEnable (void);
    bool IsDownstreamDataDMAEnable (void);
    bool IsUpstreamDMAEnable (void);
    bool IsEmergencyLevelSensitive (void);
    bool IsClockSerialActive (void);
    bool IsResetState (void);
    bool IsCmdTransDelay (void);
    bool IsPERR (void);
    void SetUpstreamActivityStatus (const unsigned int value);
    void SetInterruptStatus (std::string bit_name);
    void SetDownstreamActivity (const unsigned int value);
    void SetEmergencySignalRaisingFlag (void);
    void ClearDataTransmissionEnable (void);
    unsigned int GetActiveChannel (void);
    unsigned int GetSeqquenceLength (const unsigned int period_num);
    unsigned int GetRepetitionPeriodLength (void);
    unsigned int GetDataTransmissionSelectedSlave (const unsigned int dfte_num);
    unsigned int GetCommandTransmissionSelectedSlave (void);
    unsigned int GetDataBitNum (const unsigned int dfte_num);
    unsigned int GetCommandBitNum (void);
    unsigned int GetCPS (const unsigned int slave_num);
    unsigned int GetSOLP (const unsigned int slave_num);
    unsigned int GetCLP (void);
    unsigned int GetAssertionPhaseLength (const unsigned int slave_num);
    unsigned int GetDeassertionPhaseLength (const unsigned int slave_num);
    unsigned int GetPassivePhaseLength (void);
    unsigned int GetTimeOutCounter (void);
    unsigned int GetTransmissionPeriod (void);
    unsigned int GetEmergencyPolarity (void);
    unsigned int GetEmergencyEnableValue (void);
    void SwitchTransmissionPeriod (void);
    void DataFrameAssemble (const bool is_emergency, unsigned short *data, const unsigned int period_num);
    void CommandFrameAssemble (sc_dt::uint64 &data, const unsigned int bit_len);
    bool GetChipSelectionPolarity (const unsigned int slave_num);
    double CalculateReceptionBitsTotalTime(const unsigned int line_id);
    unsigned int GetStopBitNum(const unsigned int line_id);
    unsigned int GetContentBitNum(const unsigned int line_id);
    void StoreReceptionDataFrame (const unsigned int line_id,
                                  const unsigned int reception_data,
                                  const bool is_frame_error);
    void SetTimeOutOperation (const unsigned int line_id);
    double GetDownstreamBitTime (void);
    unsigned long long int GetDownstreamBitRate (void);
    double GetUpstreamBitTime(const unsigned int line_id);
    double GetCountDownTime(const unsigned int line_id);
    void ClearCommandTransmissionRequest (void);
    void FinishConfChange (void);

    //Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    RHSB *mRHSB_ptr;

    //Declare private members
    std::string mModuleName;
    std::string GetStateName(const unsigned int ops);
    std::string GetBitName(const std::string name, const unsigned int line_id);
    void ConfigureRHSB(void);
    bool CheckParityError(const unsigned int line_id, const unsigned int data);
    unsigned int GetFrameType(const unsigned int line_id);
    void DecodeReceptionData (const unsigned int line_id,
                              const unsigned int reception_data,
                              const bool is_frame_error);

    //Declare virtual function of rhsb_regif class
    void cb_RHSBGC_OPS(RegCBstr str);
    void cb_RHSBDCR_DMS(RegCBstr str);
    void cb_RHSBDEC_NDB3(RegCBstr str);
    void cb_RHSBSDC_DPL1(RegCBstr str);
    void cb_RHSBDEBA_DSS0(RegCBstr str);
    void cb_RHSBEBE0_EBE23_16(RegCBstr str);
    void cb_RHSBEBE1_EBE55_48(RegCBstr str);
    void cb_RHSBDTC_DTE(RegCBstr str);
    void cb_RHSBDCD_CB(RegCBstr str);
    void cb_RHSBDDR0_DB31_16(RegCBstr str);
    void cb_RHSBDDR1_DB63_48(RegCBstr str);
    void cb_RHSBDED0_EB31_16(RegCBstr str);
    void cb_RHSBDED1_EB63_48(RegCBstr str);
    void cb_RHSBUCR_FSM(RegCBstr str);
    void cb_RHSBUCC_UBR1(RegCBstr str);
    void cb_RHSBUCS_ACC(RegCBstr str);
    void cb_RHSBUDR_DF(RegCBstr str);
    void cb_RHSBIC_DTSIE(RegCBstr str);
    void cb_RHSBIS_ERF(RegCBstr str);
    void cb_RHSBDCR1_SLS1(RegCBstr str);
};
#endif
