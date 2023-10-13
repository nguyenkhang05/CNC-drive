// ---------------------------------------------------------------------
// $Id: $
//
// Copyright(c) 2016 Renesas Electronics Corporation
// Copyright(c) 2016 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ADCH_AVSEG_CORE_H__
#define __ADCH_AVSEG_CORE_H__

#include "avseg_regif.h"

class Cavseg;
class Cavseg_core: public Cavseg_regif
{
friend class Cavseg;
public:
    Cavseg_core(std::string name, Cavseg *AVSEGPtr);
    ~Cavseg_core (void);

    //Read/Write transaction
    bool read (const bool is_rd_dbg, unsigned int addr, unsigned char *p_data, unsigned int size);
    bool write (const bool is_wr_dbg, unsigned int addr, unsigned char *p_data, unsigned int size);

private:
    Cavseg                          *mAVSEG;
    std::string                     mModuleName;
    unsigned int mVCENDNum;
    struct NoiseFilter
    {//{{{
        bool                        filt_enb;
        unsigned int                filt_nrmcnt;
        unsigned int                filt_errcnt;
        unsigned int                recov_counter;
        unsigned int                error_counter;
        unsigned int                ex_bound;
        NoiseFilter(bool _filt_enb=false,
                    unsigned int _filt_nrmcnt=1,
                    unsigned int _filt_errcnt=1,
                    unsigned int _recov_counter=1,
                    unsigned int _error_counter=1,
                    unsigned int _ex_bound=0) {
            this->filt_enb = _filt_enb;
            this->filt_nrmcnt = _filt_nrmcnt;
            this->filt_errcnt = _filt_errcnt;
            this->recov_counter = _recov_counter;
            this->error_counter = _error_counter;
            this->ex_bound = _ex_bound;
        };
        void Initialize (void) {
            filt_enb = false;
            filt_nrmcnt = 1;
            filt_errcnt = 1;
            recov_counter = 1;
            error_counter = 1;
            ex_bound = 0;//0: no err; 1: upper err; 2: lower err
        }
        void rec_res (void) {
            recov_counter = filt_nrmcnt;
        }
        void err_res (void) {
            error_counter = filt_errcnt;
        }
    };//}}}
    NoiseFilter *mNoiseFilter;

    bool GetWriteCondition (const unsigned int addr);
    void PreReadCall (const unsigned int addr);
    void EnableReset (const bool is_active, const unsigned int reset_id);
    void Initialize (void);
    std::string RegisterHandler (const std::vector<std::string> &args);
    std::string mTargetProduct;

    unsigned int GetChannelSelect (const unsigned int chn_sel);
    void NotifyFilterControl(const unsigned int chn_sel, const bool is_upper, const bool is_lower);
    void UpdateErrorRegister(const unsigned int chn_sel, const bool is_upper, const bool is_lower);

    //Virtual function of Cavseg_regif class
    void cb_VCCCHSCR_CHS(RegCBstr str);
    void cb_VCCCNTCR_ENB(RegCBstr str);
    void cb_EVCCCHSCR_CHS(RegCBstr str);
    void cb_EVCCCNTCR_ENB(RegCBstr str);
    void cb_AWOVDDCHSCR_CHS(RegCBstr str);
    void cb_AWOVDDCNTCR_ENB(RegCBstr str);
    void cb_ISOVDDCHSCR_CHS(RegCBstr str);
    void cb_ISOVDDCNTCR_ENB(RegCBstr str);
    void cb_SECVMONCLR_VCC_LLMEC(RegCBstr str);

    bool strmatch(const char *ptn, const char *str);
};
#endif
