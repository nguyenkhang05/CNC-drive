// ---------------------------------------------------------------------
// $Id: PY_UTILITY.cpp,v 1.2 2021/01/25 10:13:42 thiepnguyen Exp $
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

#include "PY_UTILITY.h"
#include "UTILITY.h"

namespace PY_UTILITY
{
    // python extended command definition
    static PyMethodDef mShApiDef[] = {
      {"Utility_SetDumpMessage", Utility_SetDumpMessage_Py, METH_VARARGS, "Utility_SetDumpMessage" },
      {"Utility_TraceOne", Utility_TraceOne_Py, METH_VARARGS, "Utility_TraceOne" },
      {"Utility_TraceAll", Utility_TraceAll_Py, METH_VARARGS, "Utility_TraceAll" },
      {"Utility_CheckExactTime", Utility_CheckExactTime_Py, METH_VARARGS, "Utility_CheckExactTime" },
      {"Utility_ListSignalPort", Utility_ListSignalPort_Py, METH_VARARGS, "Utility_ListSignalPort" },
      {"Utility_CheckPort", Utility_CheckPort_Py, METH_VARARGS, "Utility_CheckPort" },
      {"Utility_WritePort", Utility_WritePort_Py, METH_VARARGS, "Utility_WritePort" },
      {"Utility_ReadPort", Utility_ReadPort_Py, METH_VARARGS, "Utility_ReadPort" },
      {"Utility_Judgment", Utility_Judgment_Py, METH_VARARGS, "Utility_Judgment" },
      {NULL, NULL, 0, NULL}
    };

    PyObject* Utility_SetDumpMessage_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* level;
        UTILITY* obj;
        // Description: Set level = true/false to enable/disable dump message
        if ((bool)PyArg_ParseTuple(args, "ss", &token, &level) == true) {
            sc_assert((token != NULL) && (level != NULL));
            if ((obj = dynamic_cast<UTILITY*>(sc_find_object(token))) != NULL) {
                obj->SetDumpMessage(level);
            }
            else {
                SC_REPORT_ERROR("PY_UTILITY", "Cannot find the object of UTILITY class");
            }
        }
        else {
            SC_REPORT_WARNING("PY_UTILITY", "The arguments of UTILITY_MessageLevel are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    PyObject* Utility_TraceOne_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* signal_name;

        UTILITY* obj;
        if ((bool)PyArg_ParseTuple(args, "ss", &token, &signal_name) == true) {
            sc_assert((token != NULL) && (signal_name != NULL));
            if ((obj = dynamic_cast<UTILITY*>(sc_find_object(token))) != NULL) {
                obj->TimingSignalPort(obj->em_TRACE_ONE, signal_name);
            } else {
                SC_REPORT_ERROR("PY_UTILITY","Cannot find the object of UTILITY class");
            }
        } else {
            SC_REPORT_WARNING("PY_UTILITY","The arguments of UTILITY_TraceOne are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    PyObject* Utility_TraceAll_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* model_name;

        UTILITY* obj;
        if ((bool)PyArg_ParseTuple(args, "ss", &token, &model_name) == true) {
            sc_assert((token != NULL) && (model_name != NULL));
            if ((obj = dynamic_cast<UTILITY*>(sc_find_object(token))) != NULL) {
                obj->TimingSignalPort(obj->em_TRACE_ALL, model_name);
            }
            else {
                SC_REPORT_ERROR("PY_UTILITY", "Cannot find the object of UTILITY class");
            }
        }
        else {
            SC_REPORT_WARNING("PY_UTILITY", "The arguments of UTILITY_TraceAll are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    PyObject* Utility_CheckExactTime_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* signal_name;
        unsigned int expected_cycles = 0;
        unsigned int time_out = 0;
        long long freq_syn = 0;
        long long expected_value = 0;

        UTILITY* obj;
        if ((bool)PyArg_ParseTuple(args, "ssLIL", &token, &signal_name, &expected_value, &expected_cycles, &freq_syn) == true) {
            sc_assert((token != NULL) && (signal_name != NULL));
            if ((obj = dynamic_cast<UTILITY*>(sc_find_object(token))) != NULL) {
                obj->TimingSignalPort(obj->em_ONE_SHOT, signal_name, expected_value, expected_cycles, freq_syn);
            }
            else {
                SC_REPORT_ERROR("PY_UTILITY", "Cannot find the object of UTILITY class");
            }
        }
        else {
            SC_REPORT_WARNING("PY_UTILITY", "The arguments of Utility_OneShotMeasure are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    PyObject* Utility_Judgment_Py(PyObject* self, PyObject* args)
    {
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        unsigned int judge_value;
        unsigned int expected_cycles = 0;
        double period_number;
        UTILITY* obj;
        if ((bool)PyArg_ParseTuple(args, "sI", &token, &judge_value) == true) {
            sc_assert((token != NULL));
            if ((obj = dynamic_cast<UTILITY*>(sc_find_object(token))) != NULL) {
                obj->Judgement((bool)judge_value);
            } else {
                SC_REPORT_ERROR("PY_UTILITY","Cannot find the object of UTILITY class");
            }
        } else {
            SC_REPORT_WARNING("PY_UTILITY","The arguments of  Utility_Judgment are wrong");
        }
        return Py_BuildValue("");        
    } 

    PyObject* Utility_ListSignalPort_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* model_name;
        char* object_type;
        UTILITY* obj;

        if ((bool)PyArg_ParseTuple(args, "sss", &token, &model_name, &object_type) == true) {
            sc_assert((token != NULL) && (model_name != NULL) && (object_type != NULL));
            if ((obj = dynamic_cast<UTILITY*>(sc_find_object(token))) != NULL) {
                obj->ListSignalPort(model_name, object_type);
            }
            else {
                SC_REPORT_ERROR("PY_UTILITY", "Cannot find the object of UTILITY class");
            }
        }
        else {
            SC_REPORT_WARNING("PY_UTILITY", "The arguments of Utility_ListSignalPort are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    PyObject* Utility_CheckPort_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* model_name;
        char* port_name;
        long long ex_val = 0;

        UTILITY* obj;
        if ((bool)PyArg_ParseTuple(args, "sssL", &token, &model_name, &port_name, &ex_val) == true) {
            sc_assert((token != NULL) && (model_name != NULL) && (port_name != NULL) && (ex_val != NULL));
            if ((obj = dynamic_cast<UTILITY*>(sc_find_object(token))) != NULL) {
                obj->CheckSignalPort(model_name, port_name, ex_val);
            }
            else {
                SC_REPORT_ERROR("PY_UTILITY", "Cannot find the object of UTILITY class");
            }
        }
        else {
            SC_REPORT_WARNING("PY_UTILITY", "The arguments of Utility_CheckPort are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    PyObject* Utility_ReadPort_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* model_name;
        char* port_name;
        long long value = 0;

        UTILITY* obj;
        if ((bool)PyArg_ParseTuple(args, "sss", &token, &model_name, &port_name) == true) {
            sc_assert((token != NULL) && (model_name != NULL) && (port_name != NULL));
            if ((obj = dynamic_cast<UTILITY*>(sc_find_object(token))) != NULL) {
                value = obj->ReadSignalPort(model_name, port_name);
            }
            else {
                SC_REPORT_ERROR("PY_UTILITY", "Cannot find the object of UTILITY class");
            }
        }
        else {
            SC_REPORT_WARNING("PY_UTILITY", "The arguments of Utility_ReadPort are wrong");
        }
        return Py_BuildValue("i", (int)value);
    }//}}}

    PyObject* Utility_WritePort_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* model_name;
        char* port_name;
        long long value = 0;

        UTILITY* obj;
        if ((bool)PyArg_ParseTuple(args, "sssL", &token, &model_name, &port_name, &value) == true) {
            sc_assert((token != NULL) && (model_name != NULL) && (port_name != NULL) && (value != NULL));
            if ((obj = dynamic_cast<UTILITY*>(sc_find_object(token))) != NULL) {
                obj->WriteSignalPort(model_name, port_name, value);
            }
            else {
                SC_REPORT_ERROR("PY_UTILITY", "Cannot find the object of UTILITY class");
            }
        }
        else {
            SC_REPORT_WARNING("PY_UTILITY", "The arguments of Utility_WritePort are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    void SetPyExtCmd(void)
    {//{{{
        Py_InitModule("SCHEAP", mShApiDef);
    }//}}}
}
