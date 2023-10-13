// ---------------------------------------------------------------------
// $Id: CRWInterface.h,v 1.1 2020/08/06 05:36:53 chuonghoangle Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: interface used for sub-systems that have a common target socket
#ifndef __CRWInterface_H__
#define __CRWInterface_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>
class CRWInterface
{
public:
    CRWInterface(void) { ; }
    virtual ~CRWInterface(void) { ; }
    virtual void read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug) = 0;
    virtual void write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug) = 0;
};
#endif //__CRWInterface_H__
