// ---------------------------------------------------------------------
// $Id: UTILITY.h,v 1.2 2021/01/25 10:13:42 thiepnguyen Exp $
//
// Copyright(c) 2021 Renesas Electronics Corporation
// Copyright(c) 2021 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "Python.h"
#include "OSCI2.h"
#include "global.h"
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>
#include <iomanip>
#include <sstream>
// Description: Declaration of Utility class
class UTILITY : public sc_module
{
#include "UTILITY_cmdif.h"
public:
    SC_HAS_PROCESS(UTILITY);
    //Constructor/Destructor
    UTILITY(sc_module_name name);
    ~UTILITY(void);

    enum DEBUG_MODE {
        em_ONE_SHOT = 0,
        em_TRACE_ONE = 1,
        em_TRACE_ALL = 2,
    };

    // Python IF
    void TimingSignalPort(DEBUG_MODE mode, char* str, long long expected_value = 0, unsigned int expected_cycles = 0, long long freq_syn = 0);
    void Judgement(bool judge_value);
    void SetDumpMessage(const std::string msg);
    void ListSignalPort(const char* model_name, const char* type);
    void CheckSignalPort(const char* model_name, const char* port_name, long long ex_val);
    void WriteSignalPort(const char* model_name, const char* port_name, long long value);
    long long ReadSignalPort(const char* model_name, const char* port_name);
private:

    std::vector<sc_event*> v_event;
    bool mDumpMessage;
    PyObject *pModule, *pFunc, *pArgs;

    std::vector<sc_object*> vsignal_object;
    std::vector<sc_object*> vport_in_object;
    std::vector<sc_object*> vport_out_object;

    std::map<std::string, std::pair<DEBUG_MODE, long long> > map_SignalMode; //<obj_name, <mode, value_expected> 
    std::map<std::string, std::pair<bool, double> > map_SignalFlag; //<obj_name, <flag, cycles_expected> 
    std::map<std::string, std::pair<double, double> > map_SignalTiming; //<obj_name, <current_time, period>

    void start_of_simulation(void);
    void TimingProcess(const char* str);
    long long GetValueSignalPort(const char* str);
    void TimeOutHandle(const char* str);
    bool SetSensitivity(sc_object* obj);
    void DumpMsg(const std::string msg, bool level);
    void SetValueSignalPort(const char* str, long long value);
};
#endif // __UTILITY_H__
