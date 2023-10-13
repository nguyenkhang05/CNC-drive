/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once
#include "./config.h"

template<class T>
using vecProcElement = std::vector<T*>;
#if defined(_UNIT_TEST)
#  include "mock_proc_element.h"
//#  define PeVec vecProcElement<MockProcElement>
//typedef vecProcElement<MockProcElement> PeVec;
using PeVec = vecProcElement<MockProcElement>;
#else
class ProcElement;
//#  define PeVec vecProcElement<ProcElement>
//typedef vecProcElement<ProcElement> PeVec;
using PeVec = vecProcElement<ProcElement>;
#endif
