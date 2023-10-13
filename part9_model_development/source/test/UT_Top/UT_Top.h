/*
* Copyright(c) 2012 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#ifndef UT_TOP_H
#define UT_TOP_H 

#include "BusSlaveIf.h"
#include "BusMasterIf.h"

//************* prototype definition *************//

//************* User code *************//
#if 0
sc_signal<bool, SC_UNCHECKED_WRITERS>          high_sig;
#else
sc_signal<bool, SC_UNCHECKED_WRITERS>          reset_sig;
#endif

sc_dt::uint64 freq_default = (sc_dt::uint64)100000000;

//************* User code *************//

#endif // UT_TOP_H 