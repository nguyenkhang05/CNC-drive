// -----------------------------------------------------------------------------
// $Id: general_reset.h,v 1.1.1.1 2012/12/17 05:54:50 chaunguyen Exp $
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

#ifndef __GENERAL_RESET_H__
#define __GENERAL_RESET_H__

#include "systemc.h"
#include <iterator>

#ifndef re_printf
#define re_printf get_fileline(__FILE__, __LINE__); _re_printf
#endif //re_printf


/// common utility class
class Cgeneral_reset: public sc_module
{
    
public:
    SC_HAS_PROCESS(Cgeneral_reset);
    Cgeneral_reset(sc_module_name name)
    {//{{{
        // Initialize reset variables
        mCmdResetPeriod = 0;
        mIsAssertResetReceive = false;
        mIsPortResetActive = false;
        mIsCmdResetActive = false;
        mIsInitialize = true;
        
        // Reset method
        SC_METHOD(CmdResetMethod);
        dont_initialize();
        sensitive << mCmdResetEvent;

        SC_METHOD(CancelCmdResetMethod);
        dont_initialize();
        sensitive << mCancelCmdResetEvent;
    }//}}}
    
    
    // Destructor
    ~Cgeneral_reset()
    {//{{{
    }//}}}
    
    
    // Return the time resolution of the system
    double GetTimeResolution(void) const
    {//{{{
        double tu_value = 0;
        if (sc_get_time_resolution() < sc_time(1, SC_PS)){
            tu_value = 1000000000000000LL;
        } else if (sc_get_time_resolution() < sc_time(1, SC_NS)){
            tu_value = 1000000000000LL;
        } else if (sc_get_time_resolution() < sc_time(1, SC_US)){
            tu_value = 1000000000;
        } else if (sc_get_time_resolution() < sc_time(1, SC_MS)){
            tu_value = 1000000;
        } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)){
            tu_value = 1000;
        } else {
            tu_value = 1;
        }
        return tu_value;
    }//}}}

    
    // Handle reset process issued by reset port
    void Com_ResetMethod(const bool is_PortReset)
    {//{{{
        //Check value of reset signal
        mIsPortResetActive = is_PortReset;
        if (mIsPortResetActive) {
            re_printf("info","Reset signal is asserted.\n");
        } else {
            if (!mIsInitialize) {
                re_printf("info","Reset signal is negated.\n");
            }
        }

        //Cancel handleCommand reset when reset port is asserted
        if (mIsAssertResetReceive) {
            mCmdResetEvent.cancel();
            mCancelCmdResetEvent.cancel();
            mIsAssertResetReceive = false;
            mIsCmdResetActive = false;
        }

        // Call to EnableReset function of development model
        EnableReset(mIsPortResetActive);
        
        // After simulation started, clear simulation start time flag
        if (mIsInitialize) {
            mIsInitialize = false;
        }
    }//}}}
    
    
    // Handle reset process issued by AssertReset handleCommand
    void Com_AssertReset(const double delay, const double period)
    {//{{{
        if ((!mIsPortResetActive) && (!mIsAssertResetReceive)) {
            // System is currently not in reset stage
            mIsAssertResetReceive = true;
            re_printf("info","Model will reset for %f ns after %f ns.\n", period, delay);
            mCmdResetPeriod = period;
            mCmdResetEvent.notify(delay, SC_NS);
        } else {
            re_printf("warning","Reset is in progress.\n");
        }
    }//}}}
    

    // Start reset period
    void CmdResetMethod(void)
    {//{{{
        mIsCmdResetActive = true;
        mCancelCmdResetEvent.notify(mCmdResetPeriod, SC_NS);
        EnableReset(mIsCmdResetActive);
    }//}}}
    
    
    // Cancel reset period
    void CancelCmdResetMethod(void)
    {//{{{
        mIsCmdResetActive = false;
        mIsAssertResetReceive = false;
        EnableReset(mIsCmdResetActive);
    }//}}}
    
    
    // Check the reset status of system
    bool IsResetPeriod(void) const
    {//{{{
        return (mIsPortResetActive || mIsCmdResetActive);
    }//}}}
    
    
    // Check the starting of simulation
    bool IsSimStarting(void) const
    {//{{{
        return mIsInitialize;
    }//}}}
    
    
    virtual void get_fileline(std::string filename, int line_number) {}
    virtual void _re_printf(std::string group, const char *message, ...) {}
    virtual void EnableReset(const bool is_reset) {}
    
private:
    // Reset variables
    double mCmdResetPeriod;
    bool mIsAssertResetReceive;
    bool mIsPortResetActive;
    bool mIsCmdResetActive;
    // Flag to check the start time of simulation
    bool mIsInitialize;
    
    // Reset events
    sc_event mCmdResetEvent;
    sc_event mCancelCmdResetEvent;
};

#endif //__GENERAL_RESET_H__

// vim600: set foldmethod=marker :
