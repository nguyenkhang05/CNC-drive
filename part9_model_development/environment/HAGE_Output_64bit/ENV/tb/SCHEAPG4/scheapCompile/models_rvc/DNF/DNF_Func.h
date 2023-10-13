// ---------------------------------------------------------------------
// $Id: DNF_Func.h,v 1.3 2020/02/05 04:31:04 ngathunguyen Exp $
//
// Copyright(c) 2018 - 2020 Renesas Electronics Corporation
// Copyright(c) 2018 - 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __DNF_Func_H__
#define __DNF_Func_H__
#include "BusSlaveFuncIf.h"
#include "dnf_regif.h"
#include "global.h"

class DNF_AgentController;
/// DNF_Func model class
class DNF_Func:    public Cdnf_regif,
                   public BusSlaveFuncIf
{
private:
    struct ChannelInfo {
        bool cur_stt;
        bool sample_val;
        unsigned int sample_counter;
        ChannelInfo(void) {
            Init();
        }
        bool CurrentStatus(void) {
            return this->cur_stt;
        }
        bool UpdateStt(const bool new_stt) {
            if (this->cur_stt != new_stt) {
                this->cur_stt = new_stt;
                return true;
            } else {
                return false;
            }
        }
        bool UpdateSampleVal(const bool new_val) {
            if (this->sample_val != new_val) {
                this->sample_val = new_val;
                return true;
            } else {
                return false;
            }
        }
        bool GetCurrentSampleVal(void) {
            return this->sample_val;
        }
        void CountUp(void) {
            this->sample_counter++;
        }
        bool CompareWithThreshold(const unsigned int threshold_val) {
            if (this->sample_counter >= threshold_val) {
                return true;
            } else {
                return false;
            }
        }
        void ResetCounter(void) {
            this->sample_counter = 0;
        }
        void Init(void) {
            this->cur_stt = false;
            this->sample_val = false;
            this->sample_counter = 0;
        }
    };
    
    std::string mModuleName;
    DNF_AgentController *mAgentController;
    
    ChannelInfo* mChannelInfo;
    unsigned int mChannelNum;

    void cb_DNFAEN_NFENL(RegCBstr str);
    void cb_DNFAENH_NFENH(RegCBstr str);
    void cb_DNFAENL_NFENL(RegCBstr str);
    
    void ControlChannel(void);
    void EnableChannel(const unsigned int channel_id);
    void DisableChannel(const unsigned int channel_id);
    void CountUpSamplingTime(const unsigned int channel_id, const bool is_counter_reset);
    void CheckSamplingTime(const unsigned int channel_id);
public:
    DNF_Func(std::string name, const unsigned int channel_num, DNF_AgentController *AgentControllerPtr);
    ~DNF_Func(void);

    void Reset(const std::string rst_name, const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    bool IsChannelEnabled(const unsigned int channel_id);
    void HandleInputSample(const unsigned int channel_id, const bool sample);
    unsigned int GetClockPrescaler(void);

    //virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

};
#endif //__DNF_Func_H__
// vim600: set foldmethod=marker :
