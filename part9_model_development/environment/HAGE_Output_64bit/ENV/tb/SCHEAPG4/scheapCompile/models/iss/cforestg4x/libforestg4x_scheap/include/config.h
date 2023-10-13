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

// ===============
// Global variable
// ===============
#define VERSION "20200306_g4mh20_scheap"
#define REVISION "7c57fe9"

// =============
// Global switch
// =============
// * API/PYTHON
/* #undef ENABLE_PYTHON */
// * Peripheral/SIMIO
/* #undef USE_SIM_IO */
// * Peripheral/TMU
/* #undef ENABLE_TMU */
// * Peripheral/UART
/* #undef ENABLE_UART */
// * Peripheral/PERFCNT
/* #undef ENABLE_PERFCNT */
// * API/GDB
/* #undef ENABLE_GDB */
// * Peripheral/ERRORAREA
#define ENABLE_ERROR_AREA
// * Peripheral/OSTM
/* #undef ENABLE_OSTM */
// * Core/SIMBREAK
/* #undef ENABLE_SAME_PC_BREAK */
// * Core/CEDAR
/* #undef FROG_API_IF */
/* #undef FROG_API_G4MH_UT */
/* #undef SUPPORT_CUBESUITE */
#define SUPPORT_SC_HEAP
/* #undef COMP_RUNNER */
/* #undef COMP_RUNNER_G4MH */
/* #undef ENABLE_MISR */

// * Core/INST
/* #undef DEBUG_CORE_INT */
#define MASK_INT_BY_GPID

// * Core/CYCLESIM
/* #undef DEBUG_CS */
/* #undef DEBUG_CS_LSU */
/* #undef DEBUG_CS_MEM */
/* #undef PRINT_FXU_PIPE */
#define SYNC_PE_LSU_TIME
/* #undef ENABLE_EXE_ORDER_CTRL */

// * Core/TRACE
/* #undef DEBUG_PRINT_STALL */

// * Core/DEBUGBREAK
/* #undef ENABLE_DEBUG */

// * API/SCHEAP
#if defined(BUILD_CFOREST)
/* #undef DLLEXPORT */
#endif // defined(BUILD_CFOREST)

// * API/CSPLUS
/* #undef ENABLE_BARRIER_CSP */

// * API/CompRunner
/* #undef ENABLE_COMP_DEBUG */
/* #undef ENABLE_TRACE_MDP_SUPPRESS */

// * Core/LSU
/* #undef ENABLE_ROM_W_ERROR */

// * Peripheral/LATENCYINFO
#define ENBALE_SUBBANK_LRAM
#define ENBALE_SUBBANK_CRAM

// ==============
// Local variable
// ==============
// * Core/BTB
#define BTAC_ENTRY_NUM 16
#define BTAC_WAY_NUM 1

// ============
// Local switch
// ============
// * Core/BTB
/* #undef ALLOW_MULTI_HIT */
/* #undef BTAC_RR_EACH_ENTRY */
#define STUDY_GHR_64BIT
#define USE_2BIT_COUNTER_BHT
#define USE_6BIT_GHR
/* #undef USE_BTAC_DIRECT_MAP */
/* #undef USE_BTAC_LRU */
/* #undef USE_BTAC_ROUNDROBIN */
#define USE_GSHARE_BHT
#define USE_HYBRID_BP
#define USE_TAKEN_GHT
// * Core/CEDAR
#define ENABLE_PIC
// * Core/DEBUGBREAK
/* #undef ENABLE_DBG_BREAK */
#define ENABLE_DBG_PMC
// * Core/PE
#define ENABLE_RB_MDP
#define ENABLE_ROM_IDCONFLICT
#define CHECK_LACK_DBREG
/* #undef USE_STALL_COUNT */
#define DISABLE_DESTRUCTOR_CYCLE
#define ENABLE_GRG
#define CCG3M_GRAM
#define CCG3M_DBUF
/* #undef ENABLE_TRACE_BUF */
/* #undef TEST_SC_HEAP */
/* #undef ENABLE_FLC_I */
/* #undef ENABLE_FLC_D */
#define ENABLE_PEID_POS_CTRL
