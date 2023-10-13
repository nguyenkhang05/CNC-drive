// ---------------------------------------------------------------------
// $Id: UTILITY.cpp,v 1.2 2021/01/25 10:13:42 thiepnguyen Exp $
//
// Copyright(c) 2021 Renesas Electronics Corporation
// Copyright(c) 2021 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of UTILITY.cpp

#include "UTILITY.h"
#include "re_define.h"

// Description: Constructor of UTILITY class
UTILITY::UTILITY(sc_module_name name)
    :sc_module(name)
{
    CommandInit((std::string)name);
    mDumpMessage = false;
}

// Description: Destructor
UTILITY::~UTILITY(void)
{
    std::vector<sc_event*>::iterator i;
    for (i = v_event.begin(); i != v_event.end(); ++i)
    {
        delete *i;
    }
}

void UTILITY::start_of_simulation(void) {
    std::stringstream ss;

    pModule = PyImport_ImportModule("iodef");
    if (pModule == NULL) {
        ss << "Import iodef is FAIL";
        DumpMsg(ss.str(), false);
        return;
    }
    // Get attribute string of python function which verify user breakpoints
    pFunc = PyObject_GetAttrString(pModule, "GetInfoFromModel");
    if (pFunc == NULL) {
        ss.str("");
        ss << "Getting GetInfoFromModel function is FAIL";
        DumpMsg(ss.str(), false);
        return;
    }
}

void UTILITY::TimingProcess(const char* str) {

    std::stringstream ss;
    DEBUG_MODE mMode = map_SignalMode[str].first;

    long long actual_value = GetValueSignalPort(str);
    ss << str << ": Actual Value = " << std::dec << actual_value << " (0x" << std::hex << actual_value << ")";
    DumpMsg(ss.str(), true);

    if (mMode == em_ONE_SHOT) { // one-shot measure mode
        bool timeout_flag = map_SignalFlag[str].first;
        if (timeout_flag == true) {
            double previous_cycles = map_SignalTiming[str].first;
            double mNumberPeriod = map_SignalTiming[str].second;
            double expected_cycles = map_SignalFlag[str].second;
            long long expected_value = map_SignalMode[str].second;

            sc_time        mPeriod(mNumberPeriod, SC_PS);
            double current_cycles = static_cast<double>(sc_time_stamp() / mPeriod);
            double actual_delay_cycles = current_cycles - previous_cycles;

            ss.str("");
            ss << str << ": Actual Delay Cycle = " << actual_delay_cycles << "\tExpected Delay Cycle = " << expected_cycles;
            DumpMsg(ss.str(), true);
            ss.str("");
            ss << str << ": Actual Value = " << std::dec << actual_value << " (0x" << std::hex << actual_value << ")\tExpected Value = " << std::dec << expected_value << " (0x" << std::hex << expected_value << ")";
            DumpMsg(ss.str(), true);

            if (((actual_delay_cycles - expected_cycles) != 0) || ((actual_value - expected_value) != 0)) {
                this->Judgement(false);
            }
            map_SignalFlag[str].first = false;
        }
    }
    else {    // python debug mode
        pArgs = Py_BuildValue("sddL", str, map_SignalTiming[str].first, sc_time_stamp().to_double(), actual_value);
        if ((pArgs != NULL) && (pFunc != NULL)) {
            PyObject_CallObject(pFunc, pArgs);
        }
        else {
            ss.str("");
            ss << "Py_BuildValue is FAIL";
            DumpMsg(ss.str(), false);
        }
    }
}

long long UTILITY::GetValueSignalPort(const char* str)
{
    // Native C/C++ types:
    // bool, char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long,
    // float, double

    long long value = 0;
    std::stringstream ss;

    sc_object *obj = sc_find_object(str);
    if (obj == NULL) {
        ss << str << " is not available";
        DumpMsg(ss.str(), false);
    }

    //(Name, TriggerTime, RaisingTime, Value)
    if (!strcmp(obj->kind(), "sc_out")) {
        if (sc_out<bool>* tmp = dynamic_cast<sc_out<bool>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_out<char>* tmp = dynamic_cast<sc_out<char>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_out<unsigned char>* tmp = dynamic_cast<sc_out<unsigned char>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_out<short>* tmp = dynamic_cast<sc_out<short>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_out<unsigned short>* tmp = dynamic_cast<sc_out<unsigned short>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_out<int>* tmp = dynamic_cast<sc_out<int>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_out<unsigned int>* tmp = dynamic_cast<sc_out<unsigned int>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_out<long>* tmp = dynamic_cast<sc_out<long>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_out<unsigned long>* tmp = dynamic_cast<sc_out<unsigned long>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_out<long long>* tmp = dynamic_cast<sc_out<long long>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_out<unsigned long long>* tmp = dynamic_cast<sc_out<unsigned long long>*>(obj)) {
            value = (long long)(tmp->read());
        }
        //else if (sc_out<float>* tmp = dynamic_cast<sc_out<float>*>(obj)) {
        //    value = (long long)(tmp->read());
        //}
        //else if (sc_out<double>* tmp = dynamic_cast<sc_out<double>*>(obj)) {
        //    value = (long long)(tmp->read());
        //}
        else {
            std::stringstream ss;
            ss << "Data type of object name " << obj->name() << " is not supported. Then value return = 0";
            DumpMsg(ss.str(), false);
        }
    }
    else if (!strcmp(obj->kind(), "sc_in")) {
        if (sc_in<bool>* tmp = dynamic_cast<sc_in<bool>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_in<char>* tmp = dynamic_cast<sc_in<char>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_in<unsigned char>* tmp = dynamic_cast<sc_in<unsigned char>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_in<short>* tmp = dynamic_cast<sc_in<short>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_in<unsigned short>* tmp = dynamic_cast<sc_in<unsigned short>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_in<int>* tmp = dynamic_cast<sc_in<int>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_in<unsigned int>* tmp = dynamic_cast<sc_in<unsigned int>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_in<long>* tmp = dynamic_cast<sc_in<long>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_in<unsigned long>* tmp = dynamic_cast<sc_in<unsigned long>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_in<long long>* tmp = dynamic_cast<sc_in<long long>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_in<unsigned long long>* tmp = dynamic_cast<sc_in<unsigned long long>*>(obj)) {
            value = (long long)(tmp->read());
        }
        //else if (sc_in<float>* tmp = dynamic_cast<sc_in<float>*>(obj)) {
        //    value = (long long)(tmp->read());
        //}
        //else if (sc_in<double>* tmp = dynamic_cast<sc_in<double>*>(obj)) {
        //    value = (long long)(tmp->read());
        //}
        else {
            std::stringstream ss;
            ss << "Data type of object name " << obj->name() << " is not supported. Then value return = 0";
            DumpMsg(ss.str(), false);
        }
    }
    else if (!strcmp(obj->kind(), "sc_signal"))
    {
        if (sc_signal<bool>* tmp = dynamic_cast<sc_signal<bool>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_signal<char>* tmp = dynamic_cast<sc_signal<char>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_signal<unsigned char>* tmp = dynamic_cast<sc_signal<unsigned char>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_signal<short>* tmp = dynamic_cast<sc_signal<short>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_signal<unsigned short>* tmp = dynamic_cast<sc_signal<unsigned short>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_signal<int>* tmp = dynamic_cast<sc_signal<int>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_signal<unsigned int>* tmp = dynamic_cast<sc_signal<unsigned int>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_signal<long>* tmp = dynamic_cast<sc_signal<long>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_signal<unsigned long>* tmp = dynamic_cast<sc_signal<unsigned long>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_signal<long long>* tmp = dynamic_cast<sc_signal<long long>*>(obj)) {
            value = (long long)(tmp->read());
        }
        else if (sc_signal<unsigned long long>* tmp = dynamic_cast<sc_signal<unsigned long long>*>(obj)) {
            value = (long long)(tmp->read());
        }
        //else if (sc_signal<float>* tmp = dynamic_cast<sc_signal<float>*>(obj)) {
        //    value = (long long)(tmp->read());
        //}
        //else if (sc_signal<double>* tmp = dynamic_cast<sc_signal<double>*>(obj)) {
        //    value = (long long)(tmp->read());
        //}
        else {
            std::stringstream ss;
            ss << "Data type of object name " << obj->name() << " is not supported. Then value return = 0";
            DumpMsg(ss.str(), false);
        }
    }
    else
    {
        std::stringstream ss;
        ss << "Type of object name " << obj->name() << " is not supported to read";
        DumpMsg(ss.str(), false);
    }
    return value;
}

void UTILITY::SetValueSignalPort(const char* str, long long value)
{
    // Native C/C++ types:
    // bool, char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long,

    std::stringstream ss;

    sc_object *obj = sc_find_object(str);
    if (obj == NULL) {
        ss << str << " is not available";
        DumpMsg(ss.str(), false);
    }
    //else {
    //    ss << str << " is available with kind " << obj->kind();
    //    DumpMsg(ss.str(), true);
    //}

    //(Name, TriggerTime, RaisingTime, Value)
    if (!strcmp(obj->kind(), "sc_out")) {
        if (sc_out<bool>* tmp = dynamic_cast<sc_out<bool>*>(obj)) {
            tmp->write((bool)value);
        }
        else if (sc_out<char>* tmp = dynamic_cast<sc_out<char>*>(obj)) {
            tmp->write((char)value);
        }
        else if (sc_out<unsigned char>* tmp = dynamic_cast<sc_out<unsigned char>*>(obj)) {
            tmp->write((unsigned char)value);
        }
        else if (sc_out<short>* tmp = dynamic_cast<sc_out<short>*>(obj)) {
            tmp->write((short)value);
        }
        else if (sc_out<unsigned short>* tmp = dynamic_cast<sc_out<unsigned short>*>(obj)) {
            tmp->write((unsigned short)value);
        }
        else if (sc_out<int>* tmp = dynamic_cast<sc_out<int>*>(obj)) {
            tmp->write((int)value);
        }
        else if (sc_out<unsigned int>* tmp = dynamic_cast<sc_out<unsigned int>*>(obj)) {
            tmp->write((unsigned int)value);
        }
        else if (sc_out<long>* tmp = dynamic_cast<sc_out<long>*>(obj)) {
            tmp->write((long)value);
        }
        else if (sc_out<unsigned long>* tmp = dynamic_cast<sc_out<unsigned long>*>(obj)) {
            tmp->write((unsigned long)value);
        }
        else if (sc_out<long long>* tmp = dynamic_cast<sc_out<long long>*>(obj)) {
            tmp->write((long long)value);
        }
        else if (sc_out<unsigned long long>* tmp = dynamic_cast<sc_out<unsigned long long>*>(obj)) {
            tmp->write((unsigned long long)value);
        }
        else {
            std::stringstream ss;
            ss << "Data type of object name " << obj->name() << " is not supported. Write is fail";
            DumpMsg(ss.str(), false);
            Judgement(false);
        }
    }
    else if (!strcmp(obj->kind(), "sc_signal"))
    {
        if (sc_signal<bool>* tmp = dynamic_cast<sc_signal<bool>*>(obj)) {
            tmp->write((bool)value);
        }
        else if (sc_signal<char>* tmp = dynamic_cast<sc_signal<char>*>(obj)) {
            tmp->write((bool)value);
        }
        else if (sc_signal<unsigned char>* tmp = dynamic_cast<sc_signal<unsigned char>*>(obj)) {
            tmp->write((unsigned char)value);
        }
        else if (sc_signal<short>* tmp = dynamic_cast<sc_signal<short>*>(obj)) {
            tmp->write((short)value);
        }
        else if (sc_signal<unsigned short>* tmp = dynamic_cast<sc_signal<unsigned short>*>(obj)) {
            tmp->write((unsigned short)value);
        }
        else if (sc_signal<int>* tmp = dynamic_cast<sc_signal<int>*>(obj)) {
            tmp->write((int)value);
        }
        else if (sc_signal<unsigned int>* tmp = dynamic_cast<sc_signal<unsigned int>*>(obj)) {
            tmp->write((unsigned int)value);
        }
        else if (sc_signal<long>* tmp = dynamic_cast<sc_signal<long>*>(obj)) {
            tmp->write((long)value);
        }
        else if (sc_signal<unsigned long>* tmp = dynamic_cast<sc_signal<unsigned long>*>(obj)) {
            tmp->write((unsigned long)value);
        }
        else if (sc_signal<long long>* tmp = dynamic_cast<sc_signal<long long>*>(obj)) {
            tmp->write((long long)value);
        }
        else if (sc_signal<unsigned long long>* tmp = dynamic_cast<sc_signal<unsigned long long>*>(obj)) {
            tmp->write((unsigned long long)value);
        }
        else {
            std::stringstream ss;
            ss << "Data type of object name " << obj->name() << " is not supported. Write is fail";
            DumpMsg(ss.str(), false);
            Judgement(false);
        }
    }
    else
    {
        std::stringstream ss;
        ss << "Type of object name " << obj->name() << " is not supported to write";
        DumpMsg(ss.str(), false);
        Judgement(false);
    }
}

void UTILITY::TimingSignalPort(DEBUG_MODE mode, char* str, long long expected_value, unsigned int expected_cycles, long long freq_syn) {

    sc_object* obj;
    obj = sc_find_object(str);
    if (obj == NULL) {
        std::stringstream ss;
        ss << str << " is not exist";
        DumpMsg(ss.str(), false);
        return;
    }
    bool isSetOK = false;

    if (mode == em_TRACE_ALL) {
        sc_module* mptr = dynamic_cast<sc_module*>(obj);
        std::vector<sc_object*> r_children = mptr->get_child_objects();
        std::vector<sc_object*>::iterator i;
        for (i = r_children.begin(); i != r_children.end(); ++i)
        {
            if (SetSensitivity(*i)) {
                map_SignalMode[(*i)->name()] = std::pair<DEBUG_MODE, long long>(mode, 0);
                map_SignalFlag[(*i)->name()] = std::pair< bool, double >(false, 0);
                map_SignalTiming[(*i)->name()] = std::pair< double, double >(sc_time_stamp().to_double(), 0);
            }
        }
    }
    else {
        if (SetSensitivity(obj)) {
            if (mode == em_ONE_SHOT) { // one-shot measure mode
                double period = (double)(1.0e+12 / (double)freq_syn);

                /**** create event to check timeout ****/
                unsigned int time_out = expected_cycles + 1;
                sc_spawn_options ev_obj;
                sc_event *p_ev = new sc_event;
                v_event.push_back(p_ev);

                ev_obj.set_sensitivity(p_ev);
                ev_obj.spawn_method();
                ev_obj.dont_initialize();
                sc_spawn(sc_bind(&UTILITY::TimeOutHandle, this, (obj)->name()), sc_core::sc_gen_unique_name("event"), &(ev_obj));
                p_ev->notify((double)(time_out*period), SC_PS);
                /***************************************/

                sc_time mPeriod(period, SC_PS);

                double previous_cycles = static_cast<double>(sc_time_stamp() / mPeriod);

                // Insert information to map<>
                map_SignalMode[obj->name()] = std::pair<DEBUG_MODE, long long>(mode, expected_value);
                map_SignalFlag[obj->name()] = std::pair< bool, double >(true, (double)expected_cycles);
                map_SignalTiming[obj->name()] = std::pair< double, double >(previous_cycles, period);
            }
            else // python debug mode: trace one
            {
                map_SignalMode[obj->name()] = std::pair<DEBUG_MODE, long long>(mode, 0);
                map_SignalFlag[obj->name()] = std::pair< bool, double >(false, 0);
                map_SignalTiming[obj->name()] = std::pair< double, double >(sc_time_stamp().to_double(), 0);
            }
        }
    }
}

bool UTILITY::SetSensitivity(sc_object* obj) {
    std::stringstream ss;
    bool isSetOK = false;
    if (map_SignalMode.find(obj->name()) == map_SignalMode.end()) {
        // not found
        if (!strcmp((obj)->kind(), "sc_out") || !strcmp((obj)->kind(), "sc_in"))
        {
            ss << "Set sensitility for " << obj->name();
            DumpMsg(ss.str(), true);

            sc_spawn_options port_obj;
            port_obj.set_sensitivity(dynamic_cast<sc_port_base*>(obj));
            port_obj.spawn_method();
            port_obj.dont_initialize();
            sc_spawn(sc_bind(&UTILITY::TimingProcess, this, (obj)->name()), sc_core::sc_gen_unique_name((obj)->basename()), &(port_obj));
            isSetOK = true;
        }
        if (!strcmp((obj)->kind(), "sc_signal"))
        {
            ss << "Set sensitility for " << obj->name();
            DumpMsg(ss.str(), true);

            sc_spawn_options signal_obj;
            signal_obj.set_sensitivity(dynamic_cast<sc_interface*>(obj));
            signal_obj.spawn_method();
            signal_obj.dont_initialize();
            sc_spawn(sc_bind(&UTILITY::TimingProcess, this, (obj)->name()), sc_core::sc_gen_unique_name((obj)->basename()), &(signal_obj));
            isSetOK = true;
        }
    }
    else {
        isSetOK = true;
    }
    return isSetOK;
}

void UTILITY::SetDumpMessage(const std::string msg)
{
    if (!strcmp(msg.c_str(), "true")) {
        re_printf("info", "Enable dump info|warning message");
        mDumpMessage = true;
    }
    else if (!strcmp(msg.c_str(), "false")) {
        re_printf("info", "Disable dump info|warning message");
        mDumpMessage = false;
    }
    else {
        re_printf("warning", "The name (%s) of Utility_SetDumpMessage argument is wrong", msg.c_str());
    }
}

void UTILITY::DumpMsg(const std::string msg, bool level)
{
    if (mDumpMessage) {
        if (level) {
            re_printf("info", msg.c_str());
        }
        else {
            re_printf("warning", msg.c_str());
        }
    }
}

void UTILITY::TimeOutHandle(const char* str) {
    std::stringstream ss;
    bool timeout_flag = map_SignalFlag[str].first;
    if (timeout_flag) {
        ss << "Timeout check for " << str << " is FAIL";
        DumpMsg(ss.str(), true);
        this->Judgement(false);
    }
    else {
        ss << "Timeout check for " << str << " is PASS";
        DumpMsg(ss.str(), true);
    }
}

void UTILITY::Judgement(bool judge_value)
{
    if (judge_value == false) {
        printf("--------------------------------\n");
        printf("-          TM is FAIL          -\n");
        printf("--------------------------------\n");
        exit(1);
    }
}

void UTILITY::ListSignalPort(const char* model_name, const char* type)
{
    sc_object* obj;
    std::stringstream ss, ss_info;
    long long value = 0;
    obj = sc_find_object(model_name);
    if (obj == NULL) {
        ss << "The object name (" << model_name << ") of ListSignalPort argument is not supported";
        DumpMsg(ss.str(), false);
        return;
    }
    sc_module* mptr = dynamic_cast<sc_module*>(obj);
    std::vector<sc_object*> r_children = mptr->get_child_objects();
    std::vector<sc_object*>::iterator i;

    ss << std::left << std::setw(60) << "|Name"
        << std::left << std::setw(15) << "|Object Kind"
        << std::left << std::setw(15) << "|Value (dec)"
        << std::left << std::setw(9) << "(hex)" << "|" << endl;
    for (i = r_children.begin(); i != r_children.end(); ++i)
    {
        if (!strcmp(type, "all")) {
            if (!strcmp((*i)->kind(), "sc_in") || !strcmp((*i)->kind(), "sc_out") || !strcmp((*i)->kind(), "sc_signal")) {
                value = GetValueSignalPort((*i)->name());
                ss  << std::left << std::setw(60) << (*i)->name()
                    << std::left << std::setw(15) << (*i)->kind()
                    << std::left << std::setw(15) << std::dec << value
                    << "0x" << std::hex << value << endl;
            }
        }
        else if (!strcmp(type, "sc_in") || !strcmp(type, "sc_out") || !strcmp(type, "sc_signal")) {
            if (!strcmp((*i)->kind(), type)) {
                value = GetValueSignalPort((*i)->name());
                ss  << std::left << std::setw(60) << (*i)->name()
                    << std::left << std::setw(15) << (*i)->kind()
                    << std::left << std::setw(15) << std::dec << value
                    << "0x" << std::hex << value << endl;
            }
        }
        else {
            ss.str("");
            ss << "The type (" << type << ") of ListSignalPort argument is not supported";
            DumpMsg(ss.str(), false);
            return;
        }
    }
    ss_info << "List [" << type << "] of [" << model_name << "] \n";
    DumpMsg(ss_info.str(), true);
    cout << ss.str();
}

void UTILITY::CheckSignalPort(const char* model_name, const char* port_name, long long ex_val) {
    sc_object* obj;
    long long value = 0;
    bool is_read_ok = false;
    std::stringstream ss;

    obj = sc_find_object(model_name);
    if (obj == NULL) {
        ss << "The model name [" << model_name <<  "] is not found";
        DumpMsg(ss.str(), false);
        Judgement(false);
    }
    sc_module* mptr = dynamic_cast<sc_module*>(obj);
    std::vector<sc_object*> r_children = mptr->get_child_objects();
    std::vector<sc_object*>::iterator i;

    for (i = r_children.begin(); i != r_children.end(); ++i)
    {
        if (!strcmp((*i)->basename(), port_name)) {
            value = GetValueSignalPort((*i)->name());
            
            ss.str("");
            ss << "Check value of [" << port_name << "] at model [" << model_name << "]: \n";
            ss << "Actual value = " << std::dec << value << " (0x" << std::hex << value 
                << ")\tExpected value = " << std::dec << ex_val << " (0x" << std::hex << ex_val << ")";
            DumpMsg(ss.str(), true);

            is_read_ok = true;
            if ((value - ex_val) != 0) {
                Judgement(false);
            }
        }
    }
    if (!is_read_ok) {
        ss.str(""); 
        ss << "[" << port_name << "] is not found at [" << model_name <<  "]";
        DumpMsg(ss.str(), false);
        Judgement(false);
    }
}

long long UTILITY::ReadSignalPort(const char* model_name, const char* port_name) {
    sc_object* obj;
    long long value = 0;
    bool is_read_ok = false;
    std::stringstream ss;

    obj = sc_find_object(model_name);
    if (obj == NULL) {
        ss << "The model name (" << model_name << ") is not found";
        DumpMsg(ss.str(), false);
        Judgement(false);
    }
    sc_module* mptr = dynamic_cast<sc_module*>(obj);
    std::vector<sc_object*> r_children = mptr->get_child_objects();
    std::vector<sc_object*>::iterator i;

    for (i = r_children.begin(); i != r_children.end(); ++i)
    {
        if (!strcmp((*i)->basename(), port_name)) {
            value = GetValueSignalPort((*i)->name());
            
            ss.str("");
            ss << "Read value of [" << port_name << "] at model [" << model_name << "]: "
               << std::dec << value << " (0x" << std::hex << value << ")";
            DumpMsg(ss.str(), true);

            is_read_ok = true;
            return value;
        }
    }
    if (!is_read_ok) {
        ss.str("");
        ss << "[" << port_name << "] is not found at [" << model_name << "]";
        DumpMsg(ss.str(), false);
        Judgement(false);
    }
}

void UTILITY::WriteSignalPort(const char* model_name, const char* port_name, long long value) {
    sc_object* obj;
    bool is_write_ok = false;
    std::stringstream ss;

    obj = sc_find_object(model_name);
    if (obj == NULL) {
        ss << "The model name (" << model_name << ") is not found";
        DumpMsg(ss.str(), false);
        Judgement(false);
    }
    sc_module* mptr = dynamic_cast<sc_module*>(obj);
    std::vector<sc_object*> r_children = mptr->get_child_objects();
    std::vector<sc_object*>::iterator i;

    for (i = r_children.begin(); i != r_children.end(); ++i)
    {
        if (!strcmp((*i)->basename(), port_name)) {
            SetValueSignalPort((*i)->name(), value);

            ss.str("");
            ss << "Write value "<< std::dec << value << " (0x" << std::hex << value << ")" 
                << " to [" << port_name << "] at model [" << model_name << "]";
            DumpMsg(ss.str(), true);

            is_write_ok = true;
        }
    }
    if (!is_write_ok) {
        ss.str("");
        ss << "[" << port_name << "] is not found at [" << model_name << "]";
        DumpMsg(ss.str(), false);
        Judgement(false);
    }
}

// vim600: set foldmethod=marker :