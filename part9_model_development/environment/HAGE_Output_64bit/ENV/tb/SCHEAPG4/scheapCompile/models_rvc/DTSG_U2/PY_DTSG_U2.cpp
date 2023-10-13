// ---------------------------------------------------------------------
// $Id: PY_DTSG_U2.cpp,v 1.3 2020/01/07 04:00:23 synguyen Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "PY_DTSG_U2.h"
#include "DTSG_U2.h"
namespace PY_DTSG_U2
{
    static PyMethodDef mShApiDef[] = {
        { // [insert indent]
          "DTSG_U2_AssertReset", AssertReset_Py, METH_VARARGS, ""}
        ,
        { // [insert indent]
          "DTSG_U2_SetCLKFreq", SetCLKFreq_Py, METH_VARARGS, ""}
        ,
        { // [insert indent]
          "DTSG_U2_GetCLKFreq", GetCLKFreq_Py, METH_VARARGS, ""}
        ,
        { // [insert indent]
          "DTSG_U2_ForceRegister", ForceRegister_Py, METH_VARARGS, ""}
        ,
        { // [insert indent]
          "DTSG_U2_ReleaseRegister", ReleaseRegister_Py, METH_VARARGS, ""}
        ,
        { // [insert indent]
          "DTSG_U2_WriteRegister", WriteRegister_Py, METH_VARARGS, ""}
        ,
        { // [insert indent]
          "DTSG_U2_ReadRegister", ReadRegister_Py, METH_VARARGS, ""}
        ,
        { // [insert indent]
          "DTSG_U2_ListRegister", ListRegister_Py, METH_VARARGS, ""}
        ,
        { // [insert indent]
          "DTSG_U2_Help", Help_Py, METH_VARARGS, ""}
        ,
        { // [insert indent]
          "DTSG_U2_MessageLevel", SetMessageLevel_Py, METH_VARARGS, ""}
        ,
        { // [insert indent]
          "DTSG_U2_DumpRegisterRW", DumpRegisterRW_Py, METH_VARARGS, ""}
        ,
        { // [insert indent]
          "DTSG_U2_DumpInterrupt", DumpInterrupt_Py, METH_VARARGS, ""}
        ,
        { // [insert indent]
          "DTSG_U2_EnableRegisterMessage", EnableRegisterMessage_Py, METH_VARARGS, ""}
        ,
        { // [insert indent]
          "DTSG_U2_SetAreaAddress", SetAreaAddress_Py, METH_VARARGS, ""}
        ,
        { // [insert indent]
          NULL, NULL, 0, NULL}
    } // [insert indent]
    ;

    PyObject* AssertReset_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* reset_name;
        double start_time;
        double period;
        DTSG_U2* obj;
        if ((bool)PyArg_ParseTuple(args, "ssdd", &token, &reset_name, &start_time, &period) == true) {
            sc_assert((token != NULL)&&(reset_name != NULL));
            if ((obj = dynamic_cast<DTSG_U2*>(sc_find_object(token))) != NULL) {
                obj->AssertReset(reset_name, start_time, period);
            } // [insert indent]
             
            else {
                SC_REPORT_ERROR("PY_DTSG_U2","Cannot find the object of DTSG_U2 class");
            }
        } // [insert indent]
         
        else {
            SC_REPORT_WARNING("PY_DTSG_U2","The arguments of DTSG_U2_AssertReset are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    PyObject* SetCLKFreq_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* clock_name;
        unsigned long long freq;
        char* unit;
        DTSG_U2* obj;
        if ((bool)PyArg_ParseTuple(args, "ssKs", &token, &clock_name, &freq, &unit) == true) {
            sc_assert((token != NULL)&&(clock_name != NULL)&&(unit != NULL));
            if ((obj = dynamic_cast<DTSG_U2*>(sc_find_object(token))) != NULL) {
                obj->SetCLKFreq(clock_name, freq, unit);
            } // [insert indent]
             
            else {
                SC_REPORT_ERROR("PY_DTSG_U2","Cannot find the object of DTSG_U2 class");
            }
        } // [insert indent]
         
        else {
            SC_REPORT_WARNING("PY_DTSG_U2","The arguments of DTSG_U2_SetCLKFreq are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    PyObject* GetCLKFreq_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* clock_name;
        DTSG_U2* obj;
        if ((bool)PyArg_ParseTuple(args, "ss", &token, &clock_name) == true) {
            sc_assert((token != NULL)&&(clock_name != NULL));
            if ((obj = dynamic_cast<DTSG_U2*>(sc_find_object(token))) != NULL) {
                obj->GetCLKFreq(clock_name);
            } // [insert indent]
             
            else {
                SC_REPORT_ERROR("PY_DTSG_U2","Cannot find the object of DTSG_U2 class");
            }
        } // [insert indent]
         
        else {
            SC_REPORT_WARNING("PY_DTSG_U2","The arguments of DTSG_U2_GetCLKFreq are wrong");
        }
        return Py_BuildValue("");
    }//}}}

        PyObject* ForceRegister_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* reg_name;
        unsigned int reg_value;
        DTSG_U2* obj;
        if ((bool)PyArg_ParseTuple(args, "ssI", &token, &reg_name, &reg_value) == true) {
            sc_assert((token != NULL) && (reg_name != NULL));
            if ((obj = dynamic_cast<DTSG_U2*>(sc_find_object(token))) != NULL) {
                obj->ForceRegister(reg_name, reg_value);
            } // [insert indent]
             
            else {
                SC_REPORT_ERROR("PY_DTSG_U2","Cannot find the object of DTSG_U2 class");
            }
        } // [insert indent]
         
        else {
            SC_REPORT_WARNING("PY_DTSG_U2","The arguments of DTSG_U2_ForceRegister are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    PyObject* ReleaseRegister_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* reg_name;
        DTSG_U2* obj;
        if ((bool)PyArg_ParseTuple(args, "ss", &token, &reg_name) == true) {
            sc_assert((token != NULL) && (reg_name != NULL));
            if ((obj = dynamic_cast<DTSG_U2*>(sc_find_object(token))) != NULL) {
                obj->ReleaseRegister(reg_name);
            } // [insert indent]
             
            else {
                SC_REPORT_ERROR("PY_DTSG_U2","Cannot find the object of DTSG_U2 class");
            }
        } // [insert indent]
         
        else {
            SC_REPORT_WARNING("PY_DTSG_U2","The arguments of DTSG_U2_ReleaseRegister are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    PyObject* WriteRegister_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* reg_name;
        unsigned int reg_value;
        DTSG_U2* obj;
        if ((bool)PyArg_ParseTuple(args, "ssI", &token, &reg_name, &reg_value) == true) {
            sc_assert((token != NULL) && (reg_name != NULL));
            if ((obj = dynamic_cast<DTSG_U2*>(sc_find_object(token))) != NULL) {
                obj->WriteRegister(reg_name, reg_value);
            } // [insert indent]
             
            else {
                SC_REPORT_ERROR("PY_DTSG_U2","Cannot find the object of DTSG_U2 class");
            }
        } // [insert indent]
         
        else {
            SC_REPORT_WARNING("PY_DTSG_U2","The arguments of DTSG_U2_WriteRegister are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    PyObject* ReadRegister_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* reg_name;
        DTSG_U2* obj;
        if ((bool)PyArg_ParseTuple(args, "ss", &token, &reg_name) == true) {
            sc_assert((token != NULL) && (reg_name != NULL));
            if ((obj = dynamic_cast<DTSG_U2*>(sc_find_object(token))) != NULL) {
                obj->ReadRegister(reg_name);
            } // [insert indent]
             
            else {
                SC_REPORT_ERROR("PY_DTSG_U2","Cannot find the object of DTSG_U2 class");
            }
        } // [insert indent]
         
        else {
            SC_REPORT_WARNING("PY_DTSG_U2","The arguments of DTSG_U2_ReadRegister are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    PyObject* ListRegister_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        DTSG_U2* obj;
        if ((bool)PyArg_ParseTuple(args, "s", &token) == true) {
            sc_assert((token != NULL));
            if ((obj = dynamic_cast<DTSG_U2*>(sc_find_object(token))) != NULL) {
                obj->ListRegister();
            } // [insert indent]
             
            else {
                SC_REPORT_ERROR("PY_DTSG_U2","Cannot find the object of DTSG_U2 class");
            }
        } // [insert indent]
         
        else {
            SC_REPORT_WARNING("PY_DTSG_U2","The DTSG_U2_ListRegister has not any arguments");
        }
        return Py_BuildValue("");
    }//}}}

    PyObject* Help_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* kind;
        DTSG_U2* obj;
        if ((bool)PyArg_ParseTuple(args, "ss", &token, &kind) == true) {
            sc_assert((token != NULL) && (kind != NULL));
            if ((obj = dynamic_cast<DTSG_U2*>(sc_find_object(token))) != NULL) {
                obj->Help(kind);
            } // [insert indent]
             
            else {
                SC_REPORT_ERROR("PY_DTSG_U2","Cannot find the object of DTSG_U2 class");
            }
        } // [insert indent]
         
        else {
            SC_REPORT_WARNING("PY_DTSG_U2","The arguments of DTSG_U2_Help are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    PyObject* SetMessageLevel_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* level;
        DTSG_U2* obj;
        if ((bool)PyArg_ParseTuple(args, "ss", &token, &level) == true) {
            sc_assert((token != NULL) && (level != NULL));
            if ((obj = dynamic_cast<DTSG_U2*>(sc_find_object(token))) != NULL) {
                obj->SetMessageLevel(level);
            } // [insert indent]
             
            else {
                SC_REPORT_ERROR("PY_DTSG_U2","Cannot find the object of DTSG_U2 class");
            }
        } // [insert indent]
         
        else {
            SC_REPORT_WARNING("PY_DTSG_U2","The arguments of DTSG_U2_MessageLevel are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    PyObject* DumpRegisterRW_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* is_enable;
        DTSG_U2* obj;
        if ((bool)PyArg_ParseTuple(args, "ss", &token, &is_enable) == true) {
            sc_assert((token != NULL) && (is_enable != NULL));
            if ((obj = dynamic_cast<DTSG_U2*>(sc_find_object(token))) != NULL) {
                obj->DumpRegisterRW(is_enable);
            } // [insert indent]
             
            else {
                SC_REPORT_ERROR("PY_DTSG_U2","Cannot find the object of DTSG_U2 class");
            }
        } // [insert indent]
         
        else {
            SC_REPORT_WARNING("PY_DTSG_U2","The arguments of DTSG_U2_DumpRegisterRW are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    PyObject* DumpInterrupt_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* is_enable;
        DTSG_U2* obj;
        if ((bool)PyArg_ParseTuple(args, "ss", &token, &is_enable) == true) {
            sc_assert((token != NULL) && (is_enable != NULL));
            if ((obj = dynamic_cast<DTSG_U2*>(sc_find_object(token))) != NULL) {
                obj->DumpInterrupt(is_enable);
            } // [insert indent]
             
            else {
                SC_REPORT_ERROR("PY_DTSG_U2","Cannot find the object of DTSG_U2 class");
            }
        } // [insert indent]
         
        else {
            SC_REPORT_WARNING("PY_DTSG_U2","The arguments of DTSG_U2_DumpInterrupt are wrong");
        }
        return Py_BuildValue("");
    }//}}}
    
    PyObject* EnableRegisterMessage_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* is_enable;
        DTSG_U2* obj;
        if ((bool)PyArg_ParseTuple(args, "ss", &token, &is_enable) == true) {
            sc_assert((token != NULL) && (is_enable != NULL));
            if ((obj = dynamic_cast<DTSG_U2*>(sc_find_object(token))) != NULL) {
                obj->EnableRegisterMessage(is_enable);
            } // [insert indent]
             
            else {
                SC_REPORT_ERROR("PY_DTSG_U2","Cannot find the object of DTSG_U2 class");
            }
        } // [insert indent]
         
        else {
            SC_REPORT_WARNING("PY_DTSG_U2","The arguments of DTSG_U2_EnableRegisterMessage are wrong");
        }
        return Py_BuildValue("");
    }//}}}


    PyObject* SetAreaAddress_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert(args != NULL);
        char* token;
        unsigned int start_addr;
        unsigned int size;
        unsigned int ch_id;
        DTSG_U2* obj;
        if ((bool)PyArg_ParseTuple(args, "sIII", &token, &start_addr, &size, &ch_id) == true) {
            sc_assert((token != NULL));
            if ((obj = dynamic_cast<DTSG_U2*>(sc_find_object(token))) != NULL) {
                obj->SetAreaAddress(start_addr, size, ch_id);
            } // [insert indent]
             
            else {
                SC_REPORT_ERROR("PY_DTSG_U2","Cannot find the object of DTSG_U2 class");
            }
        } // [insert indent]
         
        else {
            SC_REPORT_ERROR("PY_DTSG_U2","The arguments of DTSG_U2_SetAreaAddress are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    void SetPyExtCmd(void)
    {//{{{
        Py_InitModule("SCHEAP", mShApiDef);
    }//}}}
}
// vim600: set foldmethod=marker :
