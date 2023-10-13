// ---------------------------------------------------------------------
// $Id: PY_UTILITY.h,v 1.2 2021/01/25 10:13:42 thiepnguyen Exp $
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

#ifndef __PY_UTILITY_H__
#define __PY_UTILITY_H__
#include <Python.h>
#include <string.h>

namespace PY_UTILITY
{
    static PyObject* Utility_SetDumpMessage_Py(PyObject* self, PyObject* args);
    static PyObject* Utility_TraceOne_Py(PyObject* self, PyObject* args);
    static PyObject* Utility_TraceAll_Py(PyObject* self, PyObject* args);
    static PyObject* Utility_CheckExactTime_Py(PyObject* self, PyObject* args);
    static PyObject* Utility_Judgment_Py(PyObject* self, PyObject* args);
    static PyObject* Utility_ListSignalPort_Py(PyObject* self, PyObject* args);
    static PyObject* Utility_CheckPort_Py(PyObject* self, PyObject* args);
    static PyObject* Utility_WritePort_Py(PyObject* self, PyObject* args);
    static PyObject* Utility_ReadPort_Py(PyObject* self, PyObject* args);
    void SetPyExtCmd(void);
}
#endif
