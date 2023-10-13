// ---------------------------------------------------------------------
// $Id: $
//
// Copyright(c) 2016 - 2017 Renesas Electronics Corporation
// Copyright(c) 2016 - 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ADCH_ABFG_CORE_H__
#define __ADCH_ABFG_CORE_H__

#include "abfg_regif.h"

class Cabfg;
class Cabfg_core: public Cabfg_regif
{
friend class Cabfg;
public:
    Cabfg_core(std::string name, Cabfg *ABFGPtr);
    ~Cabfg_core (void);

    //Read/Write transaction
    bool read (const bool is_rd_dbg, unsigned int addr, unsigned char *p_data, unsigned int size);
    bool write (const bool is_wr_dbg, unsigned int addr, unsigned char *p_data, unsigned int size);

private:
    Cabfg                          *mABFG;
    std::string                     mModuleName;
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

    unsigned int GetChannelSelect (const unsigned int chn_sel);
    void NotifyFilterControl(const unsigned int chn_sel, bool is_upper, bool is_lower);
    void BoundaryFlagControl(const unsigned int chn_sel, const bool is_err);

    void cb_CNTCR_ENB(RegCBstr str);
    void cb_BFSR_BFS(RegCBstr str);

    bool strmatch(const char *ptn, const char *str);
};
#endif
