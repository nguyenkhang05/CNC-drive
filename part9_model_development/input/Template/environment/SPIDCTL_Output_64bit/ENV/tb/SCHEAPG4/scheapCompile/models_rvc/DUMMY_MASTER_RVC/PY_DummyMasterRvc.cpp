//----------------------------------------------------------------------
//* File: PY_DummyMasterRvc.cpp
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
// Description: Copyright of PY_DummyMasterRvc.cpp
// Ref: {SPIDCTL_UT_VRFCopyRight_001}

#include "PY_DummyMasterRvc.h"
#include "DummyMasterRvc.h"
namespace PY_DummyMasterRvc
{
    static PyMethodDef mShApiDef[] = {
        {"DummyMasterRvc_IssueTransaction", IssueTransaction_Py, METH_VARARGS, ""},
        {"DummyMasterRvc_ReadReceivedData", ReadReceivedData_Py, METH_VARARGS, ""},
        {"DummyMasterRvc_SetSimMode", SetSimMode_Py, METH_VARARGS, ""},
        {"DummyMasterRvc_SetTMPass", SetTMPass_Py, METH_VARARGS, ""},
        {"DummyMasterRvc_EnableDumpMessage", EnableDumpMessage_Py, METH_VARARGS, ""},
        {NULL, NULL, 0, NULL}
    };

   PyObject* IssueTransaction_Py(PyObject* self, PyObject* args)
   {//{{{
       sc_assert((self != NULL) && (args != NULL));
       char* token;
       unsigned int command;
       unsigned int addr; 
       unsigned int write_size;
       unsigned int data; 
       unsigned int debug;
       unsigned int extenstion;
       char* nullextenstion;
       DummyMasterRvc* obj;
       
       if (PyArg_ParseTuple(args, "sIIIIII", &token, &command ,&addr, &write_size, &data, &debug, &extenstion) == true) {
           sc_assert((token != NULL));
           if ((obj = dynamic_cast<DummyMasterRvc*>(sc_find_object(token))) != NULL) {
               obj->IssueTransaction(command, addr, write_size, data, debug, extenstion);
           } else {
               SC_REPORT_ERROR("PY_DummyMasterRvc","Can not find the object of DummyMasterRvc class");
           }
       // Set extenstion of issue transaction to NULL. 
       } else if (PyArg_ParseTuple(args, "sIIIIIs", &token, &command ,&addr, &write_size, &data, &debug, &nullextenstion) == true) {
           sc_assert((token != NULL));
           if ((obj = dynamic_cast<DummyMasterRvc*>(sc_find_object(token))) != NULL) {
               obj->IssueTransactionextnull(command, addr, write_size, data, debug, nullextenstion);
           } else {
               SC_REPORT_ERROR("PY_DummyMasterRvc","Can not find the object of DummyMasterRvc class");
           }            
       } else {
           SC_REPORT_WARNING("PY_DummyMasterRvc","The arguments of DummyMasterRvc_IssueTransaction are wrong");
       }
       return Py_BuildValue("");
   }//}}}

   PyObject* ReadReceivedData_Py(PyObject* self, PyObject* args)
   {//{{{
       sc_assert((self != NULL) && (args != NULL));
       char* token;
       unsigned int ex_val;
       DummyMasterRvc* obj;
       if (PyArg_ParseTuple(args, "sI", &token, &ex_val) == true) {
           sc_assert((token != NULL));
           if ((obj = dynamic_cast<DummyMasterRvc*>(sc_find_object(token))) != NULL) {
               obj->ReadReceivedData(ex_val);
           } else {
               SC_REPORT_ERROR("PY_DummyMasterRvc","Can not find the object of DummyMasterRvc class");
           }
       } else {
           SC_REPORT_WARNING("PY_DummyMasterRvc","The arguments of DummyMasterRvc_ReadReceivedData are wrong");
       }
       return Py_BuildValue("");
   }//}}}

   PyObject* SetSimMode_Py(PyObject* self, PyObject* args)
   {//{{{
       sc_assert((self != NULL) && (args != NULL));
       char* token;
       char* mode;
       DummyMasterRvc* obj;
       if (PyArg_ParseTuple(args, "ss", &token, &mode) == true) {
           sc_assert((token != NULL) && (mode != NULL));
           if ((obj = dynamic_cast<DummyMasterRvc*>(sc_find_object(token))) != NULL) {
               obj->SetSimMode(mode);
           } else {
               SC_REPORT_ERROR("PY_DummyMasterRvc","Can not find the object of DummyMasterRvc class");
           }
       } else {
           SC_REPORT_WARNING("PY_DummyMasterRvc","The arguments of DummyMasterRvc_SetSimMode are wrong");
       }
       return Py_BuildValue("");
   }//}}}

    PyObject* SetTMPass_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        DummyMasterRvc* obj;
        if (PyArg_ParseTuple(args, "s", &token) == true) {
            sc_assert((token != NULL));
            if ((obj = dynamic_cast<DummyMasterRvc*>(sc_find_object(token))) != NULL) {
                obj->SetTMPass();
            } else {
                SC_REPORT_ERROR("PY_DummyMasterRvc","Can not find the object of DummyMasterRvc class");
                printf("Info:     Object name (%s) is wrong", token);
            }
        } else {
            SC_REPORT_WARNING("PY_DummyMasterRvc","The arguments of DummyMasterRvc_SetTMPass are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    PyObject* EnableDumpMessage_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        unsigned int enable;
        DummyMasterRvc* obj;
        if (PyArg_ParseTuple(args, "sI", &token, &enable) == true) {
            sc_assert((token != NULL));
            if ((obj = dynamic_cast<DummyMasterRvc*>(sc_find_object(token))) != NULL) {
                obj->EnableDumpMessage((bool)enable);
            } else {
                SC_REPORT_ERROR("PY_DummyMasterRvc","Can not find the object of DummyMasterRvc class");
                printf("Info:     Object name (%s) is wrong", token);
            }
        } else {
            SC_REPORT_WARNING("PY_DummyMasterRvc","The arguments of DummyMasterRvc_EnableDumpMessage are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    void SetPyExtCmd(void)
    {//{{{
        Py_InitModule("SCHEAP", mShApiDef);
    }//}}}
}
// vim600: set foldmethod=marker :

