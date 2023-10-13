// ---------------------------------------------------------------------
// $Id: PY_SPIDCTL.h,v 1.2 2021/11/18 07:22:52 minhquocha Exp $
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
// Description: Copyright of PY_SPIDCTL.h
// Ref: {SPIDCTL_UD_Copyright_001}

#ifndef __PY_SPIDCTL_H__
#define __PY_SPIDCTL_H__
#include <Python.h>
namespace PY_SPIDCTL
{
    static PyObject* SetMessageLevel_Py(PyObject* self, PyObject* args);
    static PyObject* DumpRegisterRW_Py(PyObject* self, PyObject* args);
    static PyObject* EnableRegisterMessage_Py(PyObject* self, PyObject* args);
    static PyObject* DumpStatusInfo_Py(PyObject* self, PyObject* args);
    static PyObject* AssertReset_Py(PyObject* self, PyObject* args);
    static PyObject* SetCLKFreq_Py(PyObject* self, PyObject* args);
    static PyObject* GetCLKFreq_Py(PyObject* self, PyObject* args);
    static PyObject* ForceRegister_Py(PyObject* self, PyObject* args);
    static PyObject* ReleaseRegister_Py(PyObject* self, PyObject* args);
    static PyObject* WriteRegister_Py(PyObject* self, PyObject* args);
    static PyObject* ReadRegister_Py(PyObject* self, PyObject* args);
    static PyObject* ListRegister_Py(PyObject* self, PyObject* args);
    static PyObject* Help_Py(PyObject* self, PyObject* args);

    void SetPyExtCmd(void);
}
#endif //__PY_SPIDCTL_H__
