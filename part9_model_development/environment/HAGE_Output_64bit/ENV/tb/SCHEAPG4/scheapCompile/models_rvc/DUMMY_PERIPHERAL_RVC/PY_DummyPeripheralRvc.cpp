// ---------------------------------------------------------------------
// $Id: PY_DummyPeripheralRvc.cpp 2424 2017-03-20 03:21:00Z ChinhTD $
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

#include "PY_DummyPeripheralRvc.h"
#include "DummyPeripheralRvc.h"
namespace PY_DummyPeripheralRvc
{
    static PyMethodDef mShApiDef[] = {
        {"DummyPeripheralRvc_SetOutputPort", SetOutputPort_Py, METH_VARARGS, ""},
        {"DummyPeripheralRvc_CheckInputPort", CheckInputPort_Py, METH_VARARGS, ""},
        {"DummyPeripheralRvc_CountInterruptTimes", CountInterruptTimes_Py, METH_VARARGS, ""},
        {"DummyPeripheralRvc_CountDMATimes", CountDMATimes_Py, METH_VARARGS, ""},
        {"DummyPeripheralRvc_EnableDumpMessage", EnableDumpMessage_Py, METH_VARARGS, ""},
        {"DummyPeripheralRvc_CreateMemory", CreateMemory_Py, METH_VARARGS, ""},
        {"DummyPeripheralRvc_SetMemoryData", SetMemoryData_Py, METH_VARARGS, ""},
        {"DummyPeripheralRvc_GetMemoryData", GetMemoryData_Py, METH_VARARGS, ""},
        {NULL, NULL, 0, NULL}
    };

    PyObject* SetOutputPort_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* name;
        unsigned long long value;
        DummyPeripheralRvc* obj;
        if (PyArg_ParseTuple(args, "ssK", &token, &name, &value) == true) {
            sc_assert((token != NULL) && (name != NULL));
            if ((obj = dynamic_cast<DummyPeripheralRvc*>(sc_find_object(token))) != NULL) {
                obj->SetOutputPort(name, value);
            } else {
                SC_REPORT_ERROR("PY_DummyPeripheralRvc","Can not find the object of DummyPeripheralRvc class");
            }
        } else {
            SC_REPORT_WARNING("PY_DummyPeripheralRvc","The arguments of DummyPeripheralRvc_SetOutputPort are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    PyObject* CheckInputPort_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* name;
        unsigned long long exp_value;
        DummyPeripheralRvc* obj;
        if (PyArg_ParseTuple(args, "ssK", &token, &name, &exp_value) == true) {
            sc_assert((token != NULL) && (name != NULL));
            if ((obj = dynamic_cast<DummyPeripheralRvc*>(sc_find_object(token))) != NULL) {
                obj->CheckInputPort(name, exp_value);
            } else {
                SC_REPORT_ERROR("PY_DummyPeripheralRvc","Can not find the object of DummyPeripheralRvc class");
            }
        } else {
            SC_REPORT_WARNING("PY_DummyPeripheralRvc","The arguments of DummyPeripheralRvc_CheckInputPort are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    PyObject* CountInterruptTimes_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* name;
        unsigned int exp_count;
        DummyPeripheralRvc* obj;
        if (PyArg_ParseTuple(args, "ssI", &token, &name, &exp_count) == true) {
            sc_assert((token != NULL) && (name != NULL));
            if ((obj = dynamic_cast<DummyPeripheralRvc*>(sc_find_object(token))) != NULL) {
                obj->CountInterruptTimes(name, exp_count);
            } else {
                SC_REPORT_ERROR("PY_DummyPeripheralRvc","Can not find the object of DummyPeripheralRvc class");
            }
        } else {
            SC_REPORT_WARNING("PY_DummyPeripheralRvc","The arguments of DummyPeripheralRvc_CountInterruptTimes are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    PyObject* CountDMATimes_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* name;
        unsigned int exp_count;
        DummyPeripheralRvc* obj;
        if (PyArg_ParseTuple(args, "ssI", &token, &name, &exp_count) == true) {
            sc_assert((token != NULL) && (name != NULL));
            if ((obj = dynamic_cast<DummyPeripheralRvc*>(sc_find_object(token))) != NULL) {
                obj->CountDMATimes(name, exp_count);
            } else {
                SC_REPORT_ERROR("PY_DummyPeripheralRvc","Can not find the object of DummyPeripheralRvc class");
            }
        } else {
            SC_REPORT_WARNING("PY_DummyPeripheralRvc","The arguments of DummyPeripheralRvc_CountDMATimes are wrong");
        }
        return Py_BuildValue("");
    }//}}}
    
    PyObject* EnableDumpMessage_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        unsigned int enable;
        DummyPeripheralRvc* obj;
        if (PyArg_ParseTuple(args, "sI", &token, &enable) == true) {
            sc_assert((token != NULL));
            if ((obj = dynamic_cast<DummyPeripheralRvc*>(sc_find_object(token))) != NULL) {
                obj->EnableDumpMessage((bool)enable);
            } else {
                SC_REPORT_ERROR("PY_DummyPeripheralRvc","Can not find the object of DummyPeripheralRvc class");
            }
        } else {
            SC_REPORT_WARNING("PY_DummyPeripheralRvc","The arguments of DummyPeripheralRvc_EnableDumpMessage are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    PyObject* CreateMemory_Py(PyObject* self, PyObject* args)
    {
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        unsigned int addr;
        unsigned int write_size;
        DummyPeripheralRvc* obj;
        if (PyArg_ParseTuple(args, "sII", &token, &addr, &write_size) == true) {
            sc_assert((token != NULL) && (addr != NULL) && (write_size != NULL));
            if ((obj = dynamic_cast<DummyPeripheralRvc*>(sc_find_object(token))) != NULL) {
                obj->CreateMemory(addr, write_size);
            }
            else {
                SC_REPORT_ERROR("PY_Memory", "Can not find the object of MEMORY class");
            }
        }
        else {
            SC_REPORT_WARNING("PY_Memory", "The arguments of Memory_CreateMemory are wrong");
        }
        return Py_BuildValue("");

    }

    PyObject* SetMemoryData_Py(PyObject* self, PyObject* args)
    {
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        unsigned int addr;
        unsigned int data;
        DummyPeripheralRvc* obj;
        if (PyArg_ParseTuple(args, "sII", &token, &addr, &data) == true) {
            sc_assert((token != NULL) && (addr != NULL) && (data != NULL));
            if ((obj = dynamic_cast<DummyPeripheralRvc*>(sc_find_object(token))) != NULL) {
                obj->SetMemoryData(addr, data);
            }
            else {
                SC_REPORT_ERROR("PY_Memory", "Can not find the object of MEMORY class");
            }
        }
        else {
            SC_REPORT_WARNING("PY_Memory", "The arguments of Memory_SetMemory are wrong");
        }
        return Py_BuildValue("");
    }
    PyObject* GetMemoryData_Py(PyObject* self, PyObject* args)
    {
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        unsigned int addr;
        unsigned int ex_val;
        DummyPeripheralRvc* obj;
        if (PyArg_ParseTuple(args, "sII", &token, &addr, &ex_val) == true) {
            sc_assert((token != NULL) && (addr != NULL) && (ex_val != NULL));
            if ((obj = dynamic_cast<DummyPeripheralRvc*>(sc_find_object(token))) != NULL) {
                obj->GetMemoryData(addr, ex_val);
            }
            else {
                SC_REPORT_ERROR("PY_Memory", "Can not find the object of MEMORY class");
            }
        }
        else {
            SC_REPORT_WARNING("PY_Memory", "The arguments of Memory_GetMemory are wrong");
        }
        return Py_BuildValue("");
    }

    void SetPyExtCmd(void)
    {//{{{
        Py_InitModule("SCHEAP", mShApiDef);
    }//}}}
}
// vim600: set foldmethod=marker :

