//----------------------------------------------------------------------
//* File: PY_DummyPeripheralRvc.h
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
// Description: Copyright of PY_DummyPeripheralRvc.h
// Ref: {SPIDCTL_UT_VRFCopyRight_001}
#ifndef __PY_DummyPeripheralRvc_H__
#define __PY_DummyPeripheralRvc_H__
#include <Python.h>
#include <string.h>

namespace PY_DummyPeripheralRvc
{
    static PyObject* SetOutputPort_Py (PyObject* self, PyObject* args);
    static PyObject* CheckInputPort_Py (PyObject* self, PyObject* args);	
    static PyObject* EnableDumpMessage_Py (PyObject* self, PyObject* args);	
    void SetPyExtCmd(void);
}
#endif
