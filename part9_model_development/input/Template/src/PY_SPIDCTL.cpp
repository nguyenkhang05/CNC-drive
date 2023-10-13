// ---------------------------------------------------------------------
// $Id: PY_SPIDCTL.cpp,v 1.2 2021/11/18 07:22:52 minhquocha Exp $
//
// Copyright(c) 2020-2021 Renesas Electronics Corporation
// Copyright(c) 2020-2021 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of PY_SPIDCTL.cpp
// Ref: {SPIDCTL_UD_Copyright_001}
// Description: Command IF generator PY_SPIDCTL.cpp
// Ref: {SPIDCTL_UD_ATT1_002}
#include "PY_SPIDCTL.h"
#include "SPIDCTL.h"
namespace PY_SPIDCTL
{
    static PyMethodDef mShApiDef[] = {
        {"SPIDCTL_MessageLevel", SetMessageLevel_Py, METH_VARARGS, ""},
        {"SPIDCTL_DumpRegisterRW", DumpRegisterRW_Py, METH_VARARGS, ""},
        {"SPIDCTL_EnableRegisterMessage", EnableRegisterMessage_Py, METH_VARARGS, ""},
        {"SPIDCTL_DumpStatusInfo", DumpStatusInfo_Py, METH_VARARGS, ""},
        {"SPIDCTL_AssertReset", AssertReset_Py, METH_VARARGS, ""},
        {"SPIDCTL_SetCLKFreq", SetCLKFreq_Py, METH_VARARGS, ""},
        {"SPIDCTL_GetCLKFreq", GetCLKFreq_Py, METH_VARARGS, ""},
        {"SPIDCTL_ForceRegister", ForceRegister_Py, METH_VARARGS, ""},
        {"SPIDCTL_ReleaseRegister", ReleaseRegister_Py, METH_VARARGS, ""},
        {"SPIDCTL_WriteRegister", WriteRegister_Py, METH_VARARGS, ""},
        {"SPIDCTL_ReadRegister", ReadRegister_Py, METH_VARARGS, ""},
        {"SPIDCTL_ListRegister", ListRegister_Py, METH_VARARGS, ""},
        {"SPIDCTL_Help", Help_Py, METH_VARARGS, ""},
        {NULL, NULL, 0, NULL}
    };

    // Description: Set Message Level
    // Ref: {SPIDCTL_UD_Direction_001, SPIDCTL_UD_Direction_015, SPIDCTL_UD_Direction_018, SPIDCTL_UD_Direction_024, SPIDCTL_UD_Flow_003, SPIDCTL_UD_Flow_012}
    PyObject* SetMessageLevel_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* level;
        SPIDCTL* obj;
        if ((bool)PyArg_ParseTuple(args, "ss", &token, &level) == true) {
            sc_assert((token != NULL) && (level != NULL));
            if ((obj = dynamic_cast<SPIDCTL*>(sc_find_object(token))) != NULL) {
                obj->SetMessageLevel(level);
            } else {
                SC_REPORT_ERROR("PY_SPIDCTL","Cannot find the object of SPIDCTL class");
            }
        } else {
            SC_REPORT_WARNING("PY_SPIDCTL","The arguments of SPIDCTL_MessageLevel are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    // Description: Dump Register RW enable
    // Ref: {SPIDCTL_UD_Direction_002, SPIDCTL_UD_Direction_015, SPIDCTL_UD_Direction_018, SPIDCTL_UD_Direction_024, SPIDCTL_UD_Flow_003, SPIDCTL_UD_Flow_012}
    PyObject* DumpRegisterRW_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* is_enable;
        SPIDCTL* obj;
        if ((bool)PyArg_ParseTuple(args, "ss", &token, &is_enable) == true) {
            sc_assert((token != NULL) && (is_enable != NULL));
            if ((obj = dynamic_cast<SPIDCTL*>(sc_find_object(token))) != NULL) {
                obj->DumpRegisterRW(is_enable);
            } else {
                SC_REPORT_ERROR("PY_SPIDCTL","Cannot find the object of SPIDCTL class");
            }
        } else {
            SC_REPORT_WARNING("PY_SPIDCTL","The arguments of SPIDCTL_DumpRegisterRW are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    // Description: Enable Register Message
    // Ref: {SPIDCTL_UD_Direction_003, SPIDCTL_UD_Direction_015, SPIDCTL_UD_Direction_018, SPIDCTL_UD_Direction_024, SPIDCTL_UD_Flow_003, SPIDCTL_UD_Flow_012}
    PyObject* EnableRegisterMessage_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* is_enable;
        SPIDCTL* obj;
        if ((bool)PyArg_ParseTuple(args, "ss", &token, &is_enable) == true) {
            sc_assert((token != NULL) && (is_enable != NULL));
            if ((obj = dynamic_cast<SPIDCTL*>(sc_find_object(token))) != NULL) {
                obj->EnableRegisterMessage(is_enable);
            } else {
                SC_REPORT_ERROR("PY_SPIDCTL","Cannot find the object of SPIDCTL class");
            }
        } else {
            SC_REPORT_WARNING("PY_SPIDCTL","The arguments of SPIDCTL_EnableRegisterMessage are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    // Description: Dump Status Info
    // Ref: {SPIDCTL_UD_Direction_004, SPIDCTL_UD_Direction_015, SPIDCTL_UD_Direction_018, SPIDCTL_UD_Direction_024, SPIDCTL_UD_Flow_003, SPIDCTL_UD_Flow_012}
    PyObject* DumpStatusInfo_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        SPIDCTL* obj;
        if ((bool)PyArg_ParseTuple(args, "s", &token) == true) {
            sc_assert((token != NULL));
            if ((obj = dynamic_cast<SPIDCTL*>(sc_find_object(token))) != NULL) {
                obj->DumpStatusInfo();
            } else {
                SC_REPORT_ERROR("PY_SPIDCTL","Cannot find the object of SPIDCTL class");
            }
        } else {
            SC_REPORT_WARNING("PY_SPIDCTL","The arguments of SPIDCTL_DumpStatusInfo are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    // Description: Assert reset
    // Ref: {SPIDCTL_UD_Direction_005, SPIDCTL_UD_Direction_015, SPIDCTL_UD_Direction_018, SPIDCTL_UD_Direction_024, SPIDCTL_UD_Flow_003, SPIDCTL_UD_Flow_012}
    PyObject* AssertReset_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* reset_name;
        double start_time;
        double period;
        SPIDCTL* obj;
        if ((bool)PyArg_ParseTuple(args, "ssdd", &token, &reset_name, &start_time, &period) == true) {
            sc_assert((token != NULL)&&(reset_name != NULL));
            if ((obj = dynamic_cast<SPIDCTL*>(sc_find_object(token))) != NULL) {
                obj->AssertReset(reset_name, start_time, period);
            } else {
                SC_REPORT_ERROR("PY_SPIDCTL","Cannot find the object of SPIDCTL class");
            }
        } else {
            SC_REPORT_WARNING("PY_SPIDCTL","The arguments of SPIDCTL_AssertReset are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    // Description: Set clock frequency value
    // Ref: {SPIDCTL_UD_Direction_006, SPIDCTL_UD_Direction_015, SPIDCTL_UD_Direction_018, SPIDCTL_UD_Direction_024, SPIDCTL_UD_Flow_003, SPIDCTL_UD_Flow_012}
    PyObject* SetCLKFreq_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* clock_name;
        unsigned long long freq;
        char* unit;
        SPIDCTL* obj;
        if ((bool)PyArg_ParseTuple(args, "ssKs", &token, &clock_name, &freq, &unit) == true) {
            sc_assert((token != NULL)&&(clock_name != NULL)&&(unit != NULL));
            if ((obj = dynamic_cast<SPIDCTL*>(sc_find_object(token))) != NULL) {
                obj->SetCLKFreq(clock_name, freq, unit);
            } else {
                SC_REPORT_ERROR("PY_SPIDCTL","Cannot find the object of SPIDCTL class");
            }
        } else {
            SC_REPORT_WARNING("PY_SPIDCTL","The arguments of SPIDCTL_SetCLKFreq are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    // Description: Get clock frequency value
    // Ref: {SPIDCTL_UD_Direction_007, SPIDCTL_UD_Direction_015, SPIDCTL_UD_Direction_018, SPIDCTL_UD_Direction_024, SPIDCTL_UD_Flow_003, SPIDCTL_UD_Flow_012}
    PyObject* GetCLKFreq_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* clock_name;
        SPIDCTL* obj;
        if ((bool)PyArg_ParseTuple(args, "ss", &token, &clock_name) == true) {
            sc_assert((token != NULL)&&(clock_name != NULL));
            if ((obj = dynamic_cast<SPIDCTL*>(sc_find_object(token))) != NULL) {
                obj->GetCLKFreq(clock_name);
            } else {
                SC_REPORT_ERROR("PY_SPIDCTL","Cannot find the object of SPIDCTL class");
            }
        } else {
            SC_REPORT_WARNING("PY_SPIDCTL","The arguments of SPIDCTL_GetCLKFreq are wrong");
        }
        return Py_BuildValue("");
    }//}}} 

    // Description: List register
    // Ref: {SPIDCTL_UD_Direction_012, SPIDCTL_UD_Direction_015, SPIDCTL_UD_Direction_018, SPIDCTL_UD_Direction_023, SPIDCTL_UD_Flow_003, SPIDCTL_UD_Flow_012}
    PyObject* ListRegister_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        SPIDCTL* obj;
        if ((bool)PyArg_ParseTuple(args, "s", &token) == true) {
            sc_assert((token != NULL));
            if ((obj = dynamic_cast<SPIDCTL*>(sc_find_object(token))) != NULL) {
                obj->ListRegister();
            } else {
                SC_REPORT_ERROR("PY_SPIDCTL","Cannot find the object of SPIDCTL class");
            }
        } else {
            SC_REPORT_WARNING("PY_SPIDCTL","The SPIDCTL_ListRegister has not any arguments");
        }
        return Py_BuildValue("");
    }//}}}

    // Description: Print command and parameter help
    // Ref: {SPIDCTL_UD_Direction_013, SPIDCTL_UD_Direction_015, SPIDCTL_UD_Direction_018, SPIDCTL_UD_Direction_024, SPIDCTL_UD_Flow_003, SPIDCTL_UD_Flow_012}
    PyObject* Help_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* kind;
        SPIDCTL* obj;
        if ((bool)PyArg_ParseTuple(args, "ss", &token, &kind) == true) {
            sc_assert((token != NULL) && (kind != NULL));
            if ((obj = dynamic_cast<SPIDCTL*>(sc_find_object(token))) != NULL) {
                obj->Help(kind);
            } else {
                SC_REPORT_ERROR("PY_SPIDCTL","Cannot find the object of SPIDCTL class");
            }
        } else {
            SC_REPORT_WARNING("PY_SPIDCTL","The arguments of SPIDCTL_Help are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    // Description: Force value to register
    // Ref: {SPIDCTL_UD_Direction_008, SPIDCTL_UD_Direction_015, SPIDCTL_UD_Direction_018, SPIDCTL_UD_Direction_024, SPIDCTL_UD_Flow_003, SPIDCTL_UD_Flow_012}
    PyObject* ForceRegister_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* reg_name;
        unsigned int reg_value;
        SPIDCTL* obj;
        if ((bool)PyArg_ParseTuple(args, "ssI", &token, &reg_name, &reg_value) == true) {
            sc_assert((token != NULL) && (reg_name != NULL));
            if ((obj = dynamic_cast<SPIDCTL*>(sc_find_object(token))) != NULL) {
                obj->ForceRegister(reg_name, reg_value);
            } else {
                SC_REPORT_ERROR("PY_SPIDCTL","Cannot find the object of SPIDCTL class");
            }
        } else {
            SC_REPORT_WARNING("PY_SPIDCTL","The arguments of SPIDCTL_ForceRegister are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    // Description: Release register from forced value
    // Ref: {SPIDCTL_UD_Direction_009, SPIDCTL_UD_Direction_015, SPIDCTL_UD_Direction_018, SPIDCTL_UD_Direction_024, SPIDCTL_UD_Flow_003, SPIDCTL_UD_Flow_012}
    PyObject* ReleaseRegister_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* reg_name;
        SPIDCTL* obj;
        if ((bool)PyArg_ParseTuple(args, "ss", &token, &reg_name) == true) {
            sc_assert((token != NULL) && (reg_name != NULL));
            if ((obj = dynamic_cast<SPIDCTL*>(sc_find_object(token))) != NULL) {
                obj->ReleaseRegister(reg_name);
            } else {
                SC_REPORT_ERROR("PY_SPIDCTL","Cannot find the object of SPIDCTL class");
            }
        } else {
            SC_REPORT_WARNING("PY_SPIDCTL","The arguments of SPIDCTL_ReleaseRegister are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    // Description: Write value to register
    // Ref: {SPIDCTL_UD_Direction_010, SPIDCTL_UD_Direction_015, SPIDCTL_UD_Direction_018, SPIDCTL_UD_Direction_024, SPIDCTL_UD_Flow_003, SPIDCTL_UD_Flow_012}
    PyObject* WriteRegister_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* reg_name;
        unsigned int reg_value;
        SPIDCTL* obj;
        if ((bool)PyArg_ParseTuple(args, "ssI", &token, &reg_name, &reg_value) == true) {
            sc_assert((token != NULL) && (reg_name != NULL));
            if ((obj = dynamic_cast<SPIDCTL*>(sc_find_object(token))) != NULL) {
                obj->WriteRegister(reg_name, reg_value);
            } else {
                SC_REPORT_ERROR("PY_SPIDCTL","Cannot find the object of SPIDCTL class");
            }
        } else {
            SC_REPORT_WARNING("PY_SPIDCTL","The arguments of SPIDCTL_WriteRegister are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    // Description: Read value from register
    // Ref: {SPIDCTL_UD_Direction_011, SPIDCTL_UD_Direction_015, SPIDCTL_UD_Direction_018, SPIDCTL_UD_Direction_024, SPIDCTL_UD_Flow_003, SPIDCTL_UD_Flow_012}
    PyObject* ReadRegister_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert((self != NULL) && (args != NULL));
        char* token;
        char* reg_name;
        SPIDCTL* obj;
        if ((bool)PyArg_ParseTuple(args, "ss", &token, &reg_name) == true) {
            sc_assert((token != NULL) && (reg_name != NULL));
            if ((obj = dynamic_cast<SPIDCTL*>(sc_find_object(token))) != NULL) {
                obj->ReadRegister(reg_name);
            } else {
                SC_REPORT_ERROR("PY_SPIDCTL","Cannot find the object of SPIDCTL class");
            }
        } else {
            SC_REPORT_WARNING("PY_SPIDCTL","The arguments of SPIDCTL_ReadRegister are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    // Description: Initialize module
    void SetPyExtCmd(void)
    {//{{{
        Py_InitModule("SCHEAP", mShApiDef);
    }//}}}
}
// vim600: set foldmethod=marker :
