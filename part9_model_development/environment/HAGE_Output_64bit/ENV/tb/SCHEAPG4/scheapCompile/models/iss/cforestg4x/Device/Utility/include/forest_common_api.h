/*
 * (c) 2011-2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */
/*! @file */

#pragma once


#include <stdint.h>
#include "./config.h"

#if defined(_WIN32) && defined(SUPPORT_SC_HEAP)
#ifdef DLLEXPORT
#define DLL_EXPORT_IMPORT __declspec(dllexport)
#else
#define DLL_EXPORT_IMPORT __declspec(dllimport)
#endif
#else
#define DLL_EXPORT_IMPORT
#endif

// Common type.
typedef uint32_t PhysAddr;                   //!< Common type.
typedef int32_t  PhysOffset;                 //!< Common type.
typedef uint32_t RegData;                    //!< Common type.
typedef uint64_t RegData64;                  //!< Common type.
typedef uint8_t  MemData8;                   //!< Common type.
typedef uint16_t MemData16;                  //!< Common type.
typedef uint32_t MemData;                    //!< Common type.
typedef uint64_t MemData64;                  //!< Common type.
typedef int32_t  CReg;                       //!< Common type.
typedef uint32_t ExpCause;                   //!< Common type.
typedef uint32_t SysErrCause;                //!< Common type.

//! Access size.
typedef int32_t ByteSize;
static const ByteSize SIZE_1BYTE = 1;
static const ByteSize SIZE_2BYTE = 2;
static const ByteSize SIZE_4BYTE = 4;
static const ByteSize SIZE_6BYTE = 6;
static const ByteSize SIZE_8BYTE = 8;
static const ByteSize SIZE_16BYTE = 16;
static const ByteSize SIZE_8BIT  = 1;
static const ByteSize SIZE_16BIT = 2;
static const ByteSize SIZE_32BIT = 4;
static const ByteSize SIZE_48BIT = 6;
static const ByteSize SIZE_64BIT = 8;
static const ByteSize SIZE_128BIT = 16;

//! Message level.
typedef uint32_t MessageLevel;
static const MessageLevel MSG_ERR = 0;
static const MessageLevel MSG_WAR = 1;
static const MessageLevel MSG_EXP = 2;

//! MDP cause code partition.
typedef uint32_t MdpCauseCode;
static const MdpCauseCode MDP_NONE = 0x00000000;
static const MdpCauseCode MDP_HOST = 0x00000099;
static const MdpCauseCode MDP_GUEST = 0x00000091;
static const MdpCauseCode MDP_U_R = 0x00010000;
static const MdpCauseCode MDP_U_W = 0x00020000;
static const MdpCauseCode MDP_S_R = 0x00080000;
static const MdpCauseCode MDP_S_W = 0x00100000;


//! debug mode.
typedef uint32_t DebugMode;
static const DebugMode DBG_OFF  = 0x00000000;
static const DebugMode DBG_ON   = 0x00000001;
static const DebugMode DBG_RW   = 0x00000002;
static const DebugMode DBG_PIPE = 0x00000004;


// NC/VC/TC contest id.
//typedef int32_t ContextId;
//static const ContextId NC  = -1;
//static const ContextId TC0 =  0;
//static const ContextId VC0 =  0;

//! CF_API_OK or CF_API_NG.
typedef uint32_t CfApiRetern;
static const CfApiRetern CF_API_OK = 0;
static const CfApiRetern CF_API_NG = 1;


//! Memory access restrict type.
typedef uint32_t MemRestrictType;
const MemRestrictType MRT_ACCESS_DISABLE = 0x0;
const MemRestrictType MRT_READ_ENABLE  = 0x1;
const MemRestrictType MRT_WRITE_ENABLE = 0x2;
const MemRestrictType MRT_FETCH_ENABLE = 0x4;

//! Global, Internal or External.
typedef uint32_t MemLocationType;
const MemLocationType MEM_LOCATE_GLOBAL = 0xFFFF;

const bool MemLocationInternal = true;
const bool MemLocationExternal = false;

const uint32_t CF_INVALID_ID = 0xFFFFFFFF;

//! NONE-ERROR, ERROR, INVALID, etc.
typedef uint32_t GuardErrType;
const GuardErrType GD_NONE_ERR   = 0;
const GuardErrType GD_CFG_ERR    = 1;
const GuardErrType GD_CFG_ILLACC = 2;
const GuardErrType GD_CRG_ERR    = 4;
const GuardErrType GD_CRG_ILLACC = 8;
const GuardErrType GD_PEG_ERR_M  = 16;
const GuardErrType GD_PEG_ERR_S  = 32;
const GuardErrType GD_INVALID    = 64;


// プログラムレジスタ
/*! @typedef typedef int32_t  CReg;
 *  @brief Program register.
 *  @details This typdef has Constant variables as R0...R31, WR0...WR31, PC.
 */
const CReg R0  = 0x00, R1  = 0x01, R2  = 0x02, R3  = 0x03;
const CReg R4  = 0x04, R5  = 0x05, R6  = 0x06, R7  = 0x07;
const CReg R8  = 0x08, R9  = 0x09, R10 = 0x0a, R11 = 0x0b;
const CReg R12 = 0x0c, R13 = 0x0d, R14 = 0x0e, R15 = 0x0f;
const CReg R16 = 0x10, R17 = 0x11, R18 = 0x12, R19 = 0x13;
const CReg R20 = 0x14, R21 = 0x15, R22 = 0x16, R23 = 0x17;
const CReg R24 = 0x18, R25 = 0x19, R26 = 0x1a, R27 = 0x1b;
const CReg R28 = 0x1c, R29 = 0x1d, R30 = 0x1e, R31 = 0x1f;
const CReg WR0  = 0x20, WR1  = 0x21, WR2  = 0x22, WR3  = 0x23;
const CReg WR4  = 0x24, WR5  = 0x25, WR6  = 0x26, WR7  = 0x27;
const CReg WR8  = 0x28, WR9  = 0x29, WR10 = 0x2a, WR11 = 0x2b;
const CReg WR12 = 0x2c, WR13 = 0x2d, WR14 = 0x2e, WR15 = 0x2f;
const CReg WR16 = 0x30, WR17 = 0x31, WR18 = 0x32, WR19 = 0x33;
const CReg WR20 = 0x34, WR21 = 0x35, WR22 = 0x36, WR23 = 0x37;
const CReg WR24 = 0x38, WR25 = 0x39, WR26 = 0x3a, WR27 = 0x3b;
const CReg WR28 = 0x3c, WR29 = 0x3d, WR30 = 0x3e, WR31 = 0x3f;
const CReg PC  = 0xFF;

enum MemMappedArea {
    MI_CF_SELF      = 0x0100, //!< Self area
    MI_CF_IOR       = 0x1000, //!< Global APB IOR area
    MI_CF_LOCAL_IOR = 0x1001, //!< Local APB IOR area
    MI_CF_FROM      = 0x2001, //!< FlashROM area
    MI_CF_LRAM      = 0x2002, //!< LocalRAM area
    MI_CF_CRAM      = 0x2003, //!< ClusterRAM area
    MI_CF_OTHER     = 0x2004, //!< External memory or peripherals
};


//! Access attribute.
enum AccessAttr {
    ACC_READ      = 0,
    ACC_WRITE     = 1,
    ACC_READWRITE = 2
};


//! Memory access atribute.
enum MemAccessCommand {
    INST_READ    = 0,
    DATA_READ    = 1,
    DATA_WRITE   = 2,
    DATA_RMW     = 3, //!< Not Supported.
    NO_MEMACC    = 4
};


//! Guard IP type.
enum GuardAttr {
    GUARD_NONE = 0x0, //!< None guard
    GUARD_PEG  = 0x1, //!< PEG
    GUARD_CRG  = 0x2, //!< CRG
    GUARD_CFG  = 0x3, //!< CFG
};


//! CPU core type.
enum CoreType {
    CORE_G4MH,    //!< G4MH
    CORE_G4KH,    //!< G4MH
};


// CPU core version.
enum CoreVersion {
    CORE_VER_10 = 0x10,   //!< Ver 1.0
    CORE_VER_11 = 0x11,   //!< Ver 1.1
    CORE_VER_20 = 0x20,   //!< Ver 2.0
    CORE_VER_21 = 0x21,   //!< Ver 2.1
};


//! External pin.
enum ExtPinType {
    EXT_PIN_PEID,         //!< PEID.PEID
    EXT_PIN_SPID,         //!< SPID
    EXT_PIN_SPIDLIST,     //!< SPIDLIST
    EXT_PIN_BMID,         //!< BMID
    EXT_PIN_RBASE,        //!< RBASE.RBASE
    EXT_PIN_DV,           //!< RBASE.DV
    EXT_PIN_RINT,         //!< RBASE.RINT
    EXT_PIN_ICHEN,        //!< ICCTRL.ICHEN
    EXT_PIN_HAS_FPU,      //!< Has FPU HW
    EXT_PIN_HAS_FXU,      //!< Has FXU HW
    EXT_PIN_HAS_MPU,      //!< Has MPU HW
    EXT_PIN_MPTRGN,       //!< MPTRGN.MPTRGN
    EXT_PIN_MPBANK,       //!< MPCFG.NBK
    EXT_PIN_MDP_DMDP,     //!< MPCFG.DMDP
    EXT_PIN_SNZ_PERIOD,   //!< SNOOZE Period
    EXT_PIN_NUM_L1RLINK,  //!< Number of L1RAM LINK
    EXT_PIN_SIZE_L1R,     //!< Size of L1RAM for L1RCFG.L1RSIZE
    EXT_PIN_HAS_HV,       //!< Has HV HW
    EXT_PIN_HVE,          //!< HVCFG.HVE
    EXT_PIN_LSCFG_LSE,    //!< LSCFG.LSE
    EXT_PIN_DBGEN,        //!< DBGEN.HE/GE
    EXT_PIN_NONE          //!< Dummy
};


//! Error code for interrupt API
enum IntErrorCode {
    INT_E_OK           = 0,  //!< No error
    INT_E_CHANNEL      = 1,  //!< Illegal channel id
    INT_E_NOT_RECEIVED = 2,  //!< Interrupt request to no-response channel
    INT_E_SYSERR_CODE  = 3   //!< Illegal syserr code
};

//! Error code for IPIR API
enum IPIRErrorCode {
    IPI_E_OK           = 0,  //!< No error
    IPI_E_CHANNEL      = 1,  //!< Illegal channel id
};



//! Executin type
enum ExecType {
    EXEC_TYPE_RUN  = 0,
    EXEC_TYPE_STEP = 1
};


//! Break attribute
typedef uint32_t BreakHitAttribute;
static const BreakHitAttribute BRK_NONE        = 0;
static const BreakHitAttribute BRK_INST_EXE    = 1;
static const BreakHitAttribute BRK_DATA_READ   = 2;
static const BreakHitAttribute BRK_DATA_WRITE  = 4;
static const BreakHitAttribute BRK_TIMEOUT     = 8;
static const BreakHitAttribute BRK_ACCEPT_INT  = 16;
static const BreakHitAttribute BRK_TRACE_FULL  = 32;
static const BreakHitAttribute BRK_PC_MATCH    = 64;
static const BreakHitAttribute BRK_GUARD_ERR   = 128;
static const BreakHitAttribute BRK_STEP_EXE    = 256;
static const BreakHitAttribute BRK_CHANGE_PSWH = 512;


//! Break information
struct BreakInfo {
    uint32_t          peid;
    PhysAddr          inst_addr;
    PhysAddr          acc_addr;
    BreakHitAttribute attr;
    ByteSize          size;
    GuardErrType      gd_err;
};

