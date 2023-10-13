// -----------------------------------------------------------------------------
// $Id: cvm_awo.h,v 1.8 2012/12/13 08:45:22 trongtruong Exp $
//
// Copyright(c) 2012 Renesas Electronics Corporation
// Copyright(c) 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------

#ifndef __CVM_AWO_H__
#define __CVM_AWO_H__
#include "re_define.h"
#include <cstdarg>
#include <map>
#include "general_reset.h"
#include <sstream>
class Ccvm_awo: public Cgeneral_reset
{//{{{
#include "cvm_awo_cmdif.h"
    public:
        SC_HAS_PROCESS(Ccvm_awo);
        Ccvm_awo(sc_module_name name);
        ~Ccvm_awo();
        //Ports declaration
        sc_in<bool> diag_mode;
        sc_in<bool> cvmde_wen;
        sc_in<bool> cvmdmask1_wen;
        sc_in<bool> cvmdmask2_wen;
        sc_in<bool> cvmtrim0_in;
        sc_in<bool> cvmtrim1_in;
        sc_in<bool> cvmtrim2_in;
        sc_in<bool> cvmtrim3_in;
        sc_in<bool> cvmtrim4_in;
        sc_in<bool> hold_cvmtrim0;
        sc_in<bool> hold_cvmtrim1;
        sc_in<bool> hold_cvmtrim2;
        sc_in<bool> hold_cvmtrim3;
        sc_in<bool> hold_cvmtrim4;
        sc_in<sc_dt::uint64> clk;
        sc_in<sc_dt::uint64> clk_roscl;
        sc_in<bool> moscen;
        sc_in<bool> pllen;
        sc_in<bool> cpuclks0;
        sc_in<bool> cpuclks1;
        sc_in<bool> cpuclks2;
        sc_in<bool> pocdet_n;
        sc_in<bool> resz12;
        sc_in<bool> reset_fct;
        sc_in<bool> mstprqz;
        sc_in<bool> stop;
        sc_in<bool> off;
        sc_in<bool> cyclic;
        sc_in<bool> isolate;
        sc_in<bool> vccholdtrg;
        sc_in<bool> haltst;
        sc_in<bool> pdcvm2bgr_t;
        sc_in<bool> pdcvm2bufo_t;
        sc_in<bool> errcvm2isoh_t;
        sc_in<bool> errcvm2isol_t;
        sc_in<bool> errcvm2awoh_t;
        sc_in<bool> errcvm2awol_t;
        sc_in<bool> tapkeep;
        sc_in<bool> cvm2tmsk;
        sc_out<bool> pdcvm2bgr;
        sc_out<bool> pdcvm2bufo;
        sc_out<bool> errcvm2isoh;
        sc_out<bool> errcvm2isol;
        sc_out<bool> errcvm2awoh;
        sc_out<bool> errcvm2awol;
        sc_out<bool> cvmtrim0;
        sc_out<bool> cvmtrim1;
        sc_out<bool> cvmtrim2;
        sc_out<bool> cvmtrim3;
        sc_out<bool> cvmtrim4;
        sc_out<bool> cvm_disable;
        sc_out<bool> cvm_latchen;
        sc_out<bool> cvmde_latchen;
        sc_out<bool> cvmdmask1_latchen;
        sc_out<bool> cvmdmask2_latchen;
        sc_out<bool> cvmtrim_latchen;
        sc_out<bool> cvmen_select;
        sc_out<bool> cvmmsk_cold;
        sc_out<bool> mode_trans;
        sc_out<bool> test_cvm2mask;
    private:
        //Enum
        enum eCONSTANCE{
            emOffUpdateCycle = 3
           ,emReset_fctUpdateCycle = 3
           ,emCvmmsk_ColdUpdateCycle = 4
           ,emLatchenUpdateCycle = 2
           ,emLatchenSelectColdOffUpdateCycle = 6
           ,emOffSelectUpdateCycle = 3
           ,emStop_maskUpdateCycle = 52
           ,emNanoSecond = 1000000000
        };
        //Variable declaration
        bool mResetFlag;
        bool mPocdetRstFlag;
        bool mOffArr[3];
        bool mReset_fctArr[3];
        double mCLKPeriod;
        double mCLKROSCLPeriod;
        bool mOff_rise_ed;
        bool mResreq_rise_ed;
        bool mCvmmsk_coldArr[4];
        bool mCvmen_selectArr[3];
        bool mOff_maskArr[3];
        bool mCvm_latchenArr[2];
        bool mPreLatchen;
        bool mDFF4Off_mask;
        bool mHalt_mask;
        bool mMoscen_mask;
        bool mPllen_mask;
        bool mStop_mask;
        bool mCpuclks0_mask;
        bool mCpuclks1_mask;
        bool mCpuclks2_mask;        
        bool mDFF7Halt_mask;
        bool mDFF7Moscen_mask;
        bool mDFF7Pllen_mask;
        bool mDFF7Cpuclks0_mask;
        bool mDFF7Cpuclks1_mask;
        bool mDFF7Cpuclks2_mask;
        bool mHaltFlag;
        bool mMoscenFlag;
        bool mPllenFlag;
        bool mCpuclks0Flag;
        bool mCpuclks1Flag;
        bool mCpuclks2Flag;
        std::vector<bool> mHaltstQueue;
        std::vector<bool> mMoscenQueue;
        std::vector<bool> mPllenQueue;
        std::vector<bool> mCpuclks0Queue;
        std::vector<bool> mCpuclks1Queue;
        std::vector<bool> mCpuclks2Queue;
        double mLastAccessHaltst;
        double mLastAccessMoscen;
        double mLastAccessPllen;
        double mLastAccessCpuclks0;
        double mLastAccessCpuclks1;
        double mLastAccessCpuclks2;
        bool mStopArr[52];
        bool mStop;
        bool mCvmen_selectRstFlag;
        //Events
        sc_event mPocdetRstEvent;
        sc_event mResetEvent;
        sc_event mSynResetEvent;
        sc_event mLatchenEvent;
        sc_event mReadPortEvent;
        sc_event mHalt_maskEvent;
        sc_event mMoscen_maskEvent;
        sc_event mPllen_maskEvent;
        sc_event mCpuclks0_maskEvent;
        sc_event mCpuclks1_maskEvent;
        sc_event mCpuclks2_maskEvent;
        sc_event_queue mHalt_mask7ClockEvent;
        sc_event_queue mMoscen_mask7ClockEvent;
        sc_event_queue mPllen_mask7ClockEvent;
        sc_event_queue mCpuclks0_mask7ClockEvent;
        sc_event_queue mCpuclks1_mask7ClockEvent;
        sc_event_queue mCpuclks2_mask7ClockEvent;
        sc_event mCvmen_selectRstEvent;
        sc_event mStop_maskEvent;
        sc_event mMode_transEvent;
        //Functions
        void Initialize();
        double CalculateRisingEdgeTime(double period);
        bool VccholdtrgFunc(bool *arr, unsigned int length, bool isReset, bool &resetFlag, bool initValue, bool curValue);
        void MaskFunc(double &lastAccess, std::vector<bool> &queue, bool inputValue, unsigned int choose);
        bool ResetPort_maskFunc();
        bool GetValueFromQueue(std::vector<bool> &queue);
        void SetCLKfreq(const std::string clk_name, const double clk_freq);
        void AssertReset(const double delay, const double period);
        void EnableReset(const bool is_active);
        //Methods
        void ErrCvmtrimMethod();
        void Pocdet_nMethod();
        void OffThread();
        void Reset_fctThread();
        void LatchenCvmen_selectCvmmsk_coldOff_maskThread();
        void LatchenMethod();
        void HaltstMethod();
        void Halt_maskMethod();
        void MoscenMethod();
        void Moscen_maskMethod();
        void PllenMethod();
        void Pllen_maskMethod();
        void Cpuclks0Method();
        void Cpuclks0_maskMethod();
        void Cpuclks1Method();
        void Cpuclks1_maskMethod();
        void Cpuclks2Method();
        void Cpuclks2_maskMethod();
        void StopThread();
        void Stop_maskMethod();
        void Mode_transMethod();
        void Cvm_disableMethod();
        void ClkMethod();
        void Clk_rosclMethod();
};//}}}

#endif //__CVM_AWO_H__
// vim600: set foldmethod=marker :
