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

#include "PY_HAGE.h"
#include "HAGE.h"

namespace PY_HAGE
{
    PyMethodDef mShApiDef[] = {
        {"HAGE_SetMessageLevel", SetMessageLevel_Py, METH_VARARGS, ""},
        {NULL, NULL, 0, NULL}
    };

    PyObject* SetMessageLevel_Py(PyObject* self, PyObject* args)
    {//{{{
        sc_assert(args != NULL);

        char* token;
        char* message_level;
        HAGE* obj;

        if (PyArg_ParseTuple(args, "ss", &token, &message_level) != 0) {
            sc_assert((token != NULL) && (message_level != NULL));

            if ((obj = dynamic_cast<HAGE*>(sc_find_object(token))) != NULL) {
                obj->SetMessageLevel(message_level);
            }
            else {
                SC_REPORT_ERROR("PY_HAGE", "Can not find the object of HAGE class");
            }
        }
        else {
            SC_REPORT_WARNING("PY_HAGE", "The arguments of HAGE_SetMessageLevel are wrong");
        }
        return Py_BuildValue("");
    }//}}}

    void SetPyExtCmd(void)
    {//{{{
        Py_InitModule(PY_INITMODULE_NAME, mShApiDef);
    }//}}}
}