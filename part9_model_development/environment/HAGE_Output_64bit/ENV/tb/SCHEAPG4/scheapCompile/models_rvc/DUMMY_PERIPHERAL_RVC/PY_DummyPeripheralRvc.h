// ---------------------------------------------------------------------
// $Id: PY_DummyPeripheralRvc.h 2255 2017-03-08 09:14:38Z ChinhTD $
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
#ifndef __PY_DummyPeripheralRvc_H__
#define __PY_DummyPeripheralRvc_H__
#include <Python.h>

namespace PY_DummyPeripheralRvc
{
    static PyObject* SetOutputPort_Py (PyObject* self, PyObject* args);
    static PyObject* CheckInputPort_Py (PyObject* self, PyObject* args);
    static PyObject* CountInterruptTimes_Py (PyObject* self, PyObject* args);
    static PyObject* CountDMATimes_Py (PyObject* self, PyObject* args);
    static PyObject* EnableDumpMessage_Py (PyObject* self, PyObject* args);

    static PyObject* CreateMemory_Py(PyObject* self, PyObject* args);
    static PyObject* SetMemoryData_Py(PyObject* self, PyObject* args);
    static PyObject* GetMemoryData_Py(PyObject* self, PyObject* args);

    void SetPyExtCmd(void);
}
#endif
