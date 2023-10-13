/*
 * Copyright(c) 2016-2017 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#ifndef G4_MEV_DEF_H
#define G4_MEV_DEF_H

#define MEV_REG_NUM  32

// for debug output
#ifdef DEBUG_MESSAGE_OUTPUT
#  include <fstream>
#  define dmvfopen(x) dmvfout.open(x);
#  ifdef DEBUG_MESSAGE_OUTPUT_SEC
#    define dmsgmv(...) \
       dmvfout << std::hex << (sc_time_stamp()).to_seconds() << " s: " << __VA_ARGS__ << std::endl;
#  else
#    define dmsgmv(...) \
       dmvfout << std::hex << sc_time_stamp() << ": " << __VA_ARGS__ << std::endl;
#  endif
#  define dmvfclose() dmvfout.close();
#else  // not defined DEBUG_MESSAGE_OUTPUT
#  define dmvfopen(x)
#  define dmsgmv(...)
#  define dmvfclose()
#endif // DEBUG_MESSAGE_OUTPUT

#endif // G4_MEV_DEF_H
