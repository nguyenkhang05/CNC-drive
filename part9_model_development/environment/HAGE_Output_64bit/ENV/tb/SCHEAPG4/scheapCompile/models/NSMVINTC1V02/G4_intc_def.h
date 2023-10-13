/*
 * $Id: G4_intc_def.h,v 1.4 2020/03/24 09:58:12 hadinhtran Exp $
 * Copyright(c) 2016-2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#ifndef G4_INTC_DEF_H
#define G4_INTC_DEF_H

#define INTC1_CH_NUM      32
#define INTC1_FEINT_CH_NUM      16
#define INTC2_MAX_NUM      1
#define INTC2_GR_MAX_NUM   4
#define INTC2_CH_MAX_NUM   2016
#define INTC_IPIR_CH_NUM   4
#define INTC_BCAST_NUM     4

// for debug output
#ifdef DEBUG_MESSAGE_OUTPUT
#  include <fstream>
#  define dfopen(x) {difout.open(x);}
#  define dmsgsc(...) \
     difout << std::hex << sc_time_stamp() << ": " << __VA_ARGS__ << std::endl;
#  define dmsg(...) \
     difout << std::hex << __VA_ARGS__ << std::endl;
#  define dfclose() difout.close();
#else  // not defined DEBUG_MESSAGE_OUTPUT
#  define dfopen(x)
#  define dmsg(...)
#  define dmsgsc(...)
#  define dfclose()
#endif // DEBUG_MESSAGE_OUTPUT

#endif // G4_INTC_DEF_H
