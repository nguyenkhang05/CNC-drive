//----------------------------------------------------------------------
//* File: PY_DummyPeripheralRvc.cpp
//
// Copyright(c) 2019 Renesas Electronics Corporation
// Copyright(c) 2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of PY_DummyPeripheralRvc.cpp
// Ref: {SPIDCTL_UT_VRFCopyRight_001}

#include "PY_DummyPeripheralRvc.h"
#include "DummyPeripheralRvc.h"

namespace PY_DummyPeripheralRvc
{
    static PyMethodDef mShApiDef[] = {
        {"DummyPeripheralRvc_SetOutputPort", SetOutputPort_Py, METH_VARARGS, ""},
        {"DummyPeripheralRvc_CheckInputPort", CheckInputPort_Py, METH_VARARGS, ""},
        {"DummyPeripheralRvc_EnableDumpMessage", EnableDumpMessage_Py, METH_VARARGS, ""},
        {NULL, NULL, 0, NULL}
    };

    PyObject* SetOutputPort_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* name;
        unsigned long long value;

        if (PyArg_ParseTuple(args, "ssK", &token, &name, &value) == true) {
            sc_assert((token != NULL) && (name != NULL));
			DummyPeripheralRvc* obj;

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
        unsigned int Index;

        if (PyArg_ParseTuple(args, "ssIK", &token, &name, &Index, &exp_value) == true) {
            sc_assert((token != NULL) && (name != NULL));
			DummyPeripheralRvc* obj;

			if ((obj = dynamic_cast<DummyPeripheralRvc*>(sc_find_object(token))) != NULL) {
				obj->CheckInputPort(name, Index, exp_value);
			} else {
				SC_REPORT_ERROR("PY_DummyPeripheralRvc","Can not find the object of DummyPeripheralRvc class");
			}
        } else {
            SC_REPORT_WARNING("PY_DummyPeripheralRvc","The arguments of DummyPeripheralRvc_CheckInputPort are wrong");
        }
        return Py_BuildValue("");
    }//}}}
    
    PyObject* EnableDumpMessage_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        unsigned int enable;
        if (PyArg_ParseTuple(args, "sI", &token, &enable) == true) {
            sc_assert((token != NULL));
			DummyPeripheralRvc* obj;

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

    void SetPyExtCmd(void)
    {//{{{
        Py_InitModule("SCHEAP", mShApiDef);
    }//}}}
}
// vim600: set foldmethod=marker :

