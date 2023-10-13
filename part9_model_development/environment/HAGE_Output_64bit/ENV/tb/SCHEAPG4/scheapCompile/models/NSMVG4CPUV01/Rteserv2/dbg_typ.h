/*
 * Copyright(c) 2013-2014 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#ifndef DBG_TYP_H
#define DBG_TYP_H

namespace dbgTyp {

// command to communicate with rteserv2
const char CMD_NULL = '\0';
const char SUCCESS  =  '1';
const char FAILURE  =  '0';



// state of each HT
enum SIM_STATE_T {
  RTE_STATE_STEP = 1, // STEP
  RTE_STATE_HALT = 2, // HALT
  RTE_STATE_RUN  = 3  // RUN
};



// type of HW break(execution break or access break)
enum RTE_HWB_KIND_T {
  RTE_HWB_KIND_EXE =  0, // execution break
  RTE_HWB_KIND_ACC =  1  // access break
};



// attribute of HW break
enum RTE_HWB_ATTR_T {
  RTE_HWB_ATTR_R  = 1, // Read
  RTE_HWB_ATTR_W  = 2, // Write
  RTE_HWB_ATTR_RW = 3, // Read/Write
  RTE_HWB_ATTR_E  = 4  // Execute
};



// number of command from rteserv2
enum RTE_CMD_T {
//RTE_CMD_INITMSG       =  0, // for stubserv
//RTE_CMD_LOADPROGRAM   =  1, // for stubserv
  RTE_CMD_READREG       =  2, // Read registers
  RTE_CMD_WRITEREG      =  3, // Write registers
  RTE_CMD_READMEM       =  4, // Read memory
  RTE_CMD_WRITEMEM      =  5, // Write memory
  RTE_CMD_SETBP         =  6, // Set PC break
  RTE_CMD_REMOVEBP      =  7, // Remove break
  RTE_CMD_SINGLESTEP    =  8, // Step executio n
  RTE_CMD_CONT          =  9, // Continue(run)
  RTE_CMD_HALT          = 10, // Halt(stop)
//RTE_CMD_CHKSTAT       = 11, // for stubserv
//RTE_CMD_GETCPULIST    = 12, // for stubserv
//RTE_CMD_TERMINATE     = 13, // for stubserv
//RTE_CMD_DETACH        = 14, // for stubserv
  RTE_CMD_QUIT          = 15, // Quit
  RTE_CMD_WRITEBLOCKMEM = 16, // Write block-memory
  RTE_CMD_READBLOCKMEM  = 17, // Read block-memry
  RTE_CMD_CHECKSTAT     = 18, // Check state
  RTE_CMD_SETHWBP       = 19, // Set HW break
  RTE_CMD_REMOVEHWBP    = 20, // Remove HW break
  RTE_CMD_REMOVEALLHWBP = 21, // Remove all HW break
  RTE_CMD_INFOHWBP      = 22, // get information of HW break
  RTE_CMD_RESET         = 23  // Reset
};



// ID of registers for rteserv2
enum RTE_G3M_REG_NO {
  RTE_GREG_START = 0x000, // start ID of general register
  RTE_GREG_END   = 0x01F, // end ID of general register
  RTE_PC_ID      = 0x020, // ID of PC register
  RTE_VREG_START = 0x0FC, // start ID of vector register(252)
  RTE_VREG_END   = 0x13B, // end ID of vector register(315)
  RTE_SREG_START = 0x2BC, // start ID of system register(700)
  RTE_SREG_END   = 0x3BB  // end ID of system register(955)
};

} // namespace dbgTyp

#endif // DBG_TYP_H
