/*************************************************************************
*
*  PY_HAGE.h
*
* Copyright(c) 2018-2021 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*
*************************************************************************/

#ifndef __PY_HAGE_H__
#define __PY_HAGE_H__

#ifndef PY_INITMODULE_NAME
#define PY_INITMODULE_NAME "SCHEAP"
#endif

#include <Python.h>

namespace PY_HAGE
{
    static PyObject* SetMessageLevel_Py (PyObject* self, PyObject* args);
    void SetPyExtCmd(void);
}
#endif /* __PY_HAGE_H__ */
