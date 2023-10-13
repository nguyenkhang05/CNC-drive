// ---------------------------------------------------------------------
// $Id: PY_DTSG_U2.h,v 1.3 2020/01/07 04:00:23 synguyen Exp $
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
#ifndef __PY_DTSG_U2_H__
#define __PY_DTSG_U2_H__
#include <Python.h>

namespace PY_DTSG_U2
{
    static PyObject* AssertReset_Py (PyObject* self, PyObject* args);
    static PyObject* SetCLKFreq_Py (PyObject* self, PyObject* args);
    static PyObject* GetCLKFreq_Py (PyObject* self, PyObject* args);
    static PyObject* ForceRegister_Py (PyObject* self, PyObject* args);
    static PyObject* ReleaseRegister_Py (PyObject* self, PyObject* args);
    static PyObject* WriteRegister_Py (PyObject* self, PyObject* args);
    static PyObject* ReadRegister_Py (PyObject* self, PyObject* args);
    static PyObject* ListRegister_Py (PyObject* self, PyObject* args);
    static PyObject* Help_Py (PyObject* self, PyObject* args);
    static PyObject* SetMessageLevel_Py (PyObject* self, PyObject* args);
    static PyObject* DumpRegisterRW_Py (PyObject* self, PyObject* args);
    static PyObject* DumpInterrupt_Py (PyObject* self, PyObject* args);
    static PyObject* EnableRegisterMessage_Py (PyObject* self, PyObject* args);
    static PyObject* SetAreaAddress_Py (PyObject* self, PyObject* args);
    
    void SetPyExtCmd(void);
}
#endif
