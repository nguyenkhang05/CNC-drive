/*************************************************************************
 *
 *  ShPythonAPI.h
 *
 * Copyright(c) 2012 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

#ifndef SH_PYTHON_API_H_DEF
#define SH_PYTHON_API_H_DEF

#include    <Python.h>  

class ShPythonAPI {
public:
    static void StartPy(int argc, char* argv[]);
    static void DestructorPy(void);
    static void LoadShCommandDefPy();
};

#endif //#ifndef SH_PYTHON_API_H_DEF
