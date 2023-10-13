/*
 * Copyright(c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#ifndef DBG_DEF_H
#define DBG_DEF_H


// struct of socket address defined in
// netinet/in.h(Linux), winsock2.h(Windows)
#define SA       struct sockaddr



/*
             MSB                      LSB
  new pid：  [ 0 ][V/N][TCID][VCID][PEID]
             17bit 1bit 8bit  3bit  3bit

      PEID：ID of PE
      VCID：ID of VM(virtual machine)
      TCID：ID of HT(HW thread)
      V/N : NC=0, VC=1
*/
#define RTE_PEID(id)  (unsigned int)(id & 0x7)
#define RTE_VCID(id)  (unsigned int)((id >>  3) & 0x7)
#define RTE_TCID(id)  (unsigned int)((id >>  6) & 0xFF)
#define RTE_VN(id)    (bool)((id >> 14) & 1)
#define RTE_CPUID(peid, vcid, tcid, vn)  \
          (((vn&1) << 14) | ((tcid&0xFF) << 6) | ((vcid&7) << 3) | (peid&0x7))


// for wait n-cycle
#define WAIT_CYCLE(n) wait((double)n/(double)glb_freq, SC_SEC)

// for debug output
#ifdef DEBUG_MESSAGE_OUTPUT
#  include <fstream>
#  define dfopen(x) dmfout.open(x);
#  define dmsg(...) dmfout << std::hex << sc_time_stamp() << ": " << __VA_ARGS__ << std::endl;
#  define dfclose() dmfout.close();
#else  // not defined DEBUG_MESSAGE_OUTPUT
#  define dfopen(x)
#  define dmsg(...)
#  define dfclose()
#endif // DEBUG_MESSAGE_OUTPUT



// define NO_SCHEAP when using without SC-HEAP
#ifdef NO_SCHEAP
#  define Terminate() exit(0)
#  define ErrMsg(x, y, z)  \
     cerr << "Error : " << x << "  " << y << "  " << z << endl; \
     exit(1);
#else  // not defined NO_SCHEAP
#  define Terminate() scstopDoneFlag = true; sc_stop();
#  include "CmErrMsg.h"
#  define ErrMsg(x, y, z) \
     CmErrMsg::printErrorMessage(x, y, z)
#endif // NO_SCHEAP


#endif // DBG_DEF_H
