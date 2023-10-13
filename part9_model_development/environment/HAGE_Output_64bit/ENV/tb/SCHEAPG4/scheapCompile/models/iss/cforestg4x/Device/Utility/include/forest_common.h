/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */
/*! @file forest_common.h */

#pragma once


#include <stdio.h>
#include "./forest_common_api.h"



#if defined(_MSC_VER) && (_MSC_VER <= 1500)
typedef char                int8_t;
typedef unsigned char       uint8_t;
typedef short               int16_t;
typedef unsigned short      uint16_t;
typedef long                int32_t;
typedef unsigned long       uint32_t;
typedef long long           int64_t;
typedef unsigned long long  uint64_t;
#else
/* gcc or VC10 */
#include <stdint.h>
#endif


#if defined(_MSC_VER) && (_MSC_VER < 1800)
    #if defined (_WIN64)
        // 64bit env
        #define MOD_LENGTGH64 "l"
    #else
        //32bit env
        #define MOD_LENGTGH64 "ll"
    #endif
    #define PRIx64 MOD_LENGTGH64 "x"
    #define PRId64 MOD_LENGTGH64 "d"
    #define PRIu64 MOD_LENGTGH64 "u"
#else
/* gcc or VC2013 */
#if !defined (__STDC_FORMAT_MACROS)
    #define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>
#endif


#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 96)
#define likely(c)    __builtin_expect(!!(int)(c), 1)
#define unlikely(c)  __builtin_expect(!!(int)(c), 0)
#else
#define likely(cond) (cond)
#define unlikely(cond) (cond)
#endif


static const PhysAddr INVALID_VADDR = 0x01U;   //!< Invalid virtual address.  : 無効仮想アドレス
static const PhysAddr INVALID_PADDR = 0x01UL;  //!< Invalid physical address. : 無効物理アドレス

//! Run mode.
enum RunMode {
    RUN_MODE = 0,
    STEP_MODE = 1,
    STEP_ANY_MODE = 2
};


enum NotifySimState {
    SIMULATE_ON      = 0x0,  //!< CPU is on operation(Not Halt/snooze state & Clock active).
    SIMULATE_HALT    = 0x1,  //!< Halt status.
    SIMULATE_STOP    = 0x2,  //!< Standby control SFRs supported in peripheral sim.
    SIMULATE_RESET   = 0x3,  //!< RESET status(DBGRESET, reset pin, internal reset)
    SIMULATE_SNOOZE  = 0x4,  //!< Snooze status.
    SIMULATE_DISABLE = 0x5   //!< Context is disabled.
};


//! To trace the type of Instruction.
enum TraceInstType {
    TRACE_DBCP   = 0x0, //!< DBCP instruction
    TRACE_DBTAG  = 0x1, //!< DBTAG instruction
    TRACE_DBPUSH = 0x2  //!< DBPUSH instruction
};

//! Machine mode.
enum MachineMode {
    MACHINE_FIX_NM = 0,
    MACHINE_FIX_VM = 1
};


enum CpuOperationMode {
    HOST_MODE = 0x0,        // CPU is on Host mode
    GUEST_MODE = 0x1,       // CPU is on Guest mode
    CONVENTIONAL_MODE = 0x2 // CPU is on Conventional mode (HV is unsupported/disabled)
};


enum SetIORBaseAddrID {
    BaseAddr_INTC1 = 0x00, //!< Base Address of the INTC1
    BaseAddr_INTC2 = 0x01, //!< Base Address of the INTC2
    BaseAddr_MECNT = 0x02, //!< Base Address of the MECNT
    BaseAddr_IPIR  = 0x03, //!< Base Address of the IPIR
    BaseAddr_BAR   = 0x04, //!< Base Address of the Barrier
    BaseAddr_UART  = 0x05, //!< Base Address of the UART
    BaseAddr_MISR  = 0x06, //!< Base Address of the MISR
    BaseAddr_PERF  = 0x07, //!< Base Address of the PerfCounter
    BaseAddr_TMU   = 0x08, //!< Base Address of the TMU
    BaseAddr_SIMIO = 0x09, //!< Base Address of the SIMIO
    BaseAddr_SIMIOG3K = 0x0a, //!< Base Address of the SIMIO for G3K
    BaseAddr_NUM   = 0x0b
};


enum GuardModuleID {
    GMID_PEG_PEnCLm           = 0x0,  //!< PEID
    GMID_PEGCAP_M_PEnCLm      = 0x1,  //!< PEID
    GMID_PEGCAP_S_PEnCLm      = 0x2,  //!< PEID
    GMID_CRG_CLm              = 0x3,  //!< CLID
    GMID_CRGCAP_PEnCLm        = 0x4,  //!< PEID
    GMID_CRGCAP_CRAMHCLm      = 0x5,  //!< CLID
    GMID_CRGCAP_CRAMLCLm      = 0x6,  //!< CLID
    GMID_CRGCAP_SX2MBHCLm     = 0x7,  //!< CLID
    GMID_CRGCAP_SX2MBLCLm     = 0x8,  //!< CLID
    GMID_MDPCAP_PEnCLm        = 0x9,  //!< PEID
    GMID_CFG                  = 0xa,  //!< ignored
    GMID_CFGCAP_IPEnCLm       = 0xb,  //!< PEID
    GMID_CFGCAP_DPEnCLm       = 0xc,  //!< PEID
    GMID_CFGCAP_SX2FX         = 0xd,  //!< ignored
    GMID_CRG_IVC_CLm          = 0xe,  //!< CLID
    GMID_CRG_CSG_CLm          = 0xf,  //!< CLID
    GMID_NUM                  = 0x10
};

//! GuardParamID
enum GuardParamID {
    GPID_CRG_ADDR_WIDTH = 0x0,
    GPID_CRG_CRG_CH_NUM = 0x1,
    GPID_CRG_CSG_CH_NUM = 0x2,
    GPID_CRG_KEYCODE    = 0x3,
    GPID_NUM            = 0x4
};


// メモリアクセスモード
//! Memory access mode.
enum MemAccessMode {
    MACC_NORMAL = 0,
    MACC_DEBUG  = 1,
};


// ミスアラインタイプ
//! The type of miss-align.
enum MissAlignType {
    //! MCTL.MA=0 fix. MAE by all miss align.
    MAA_TYPE1,  // MCTL.MA=0固定：全てのミスアラインでMAE発行
    //! if MCTL.MA=1, MAE by the miss align by LDDW/STDW/CAXI/LDLW/STCW.
    MAA_TYPE2,  // MCTL.MA=1時はLDDW/STDW/CAXI/LDLW/STCWのミスアラインでMAE発行
    //! if MCTL.MA=1, MAE by the miss align by CAXI/LDLW/STCW.
    MAA_TYPE3   // MCTL.MA=1時はCAXI/LDLW/STCWのミスアラインでMAE発行};
};

//! The type of cache way.
enum CacheWayType {
    WAY_PBS,
    WAY4,
    WAY2,
    WAY1,
    WAY0
};


typedef uint32_t BreakEvent; //!< It can be #BREAK_NONE, #BREAK_PC etc.
static const BreakEvent BREAK_NONE       = 0;  //!< Not break.(initial) : ブレークしていない（初期値）
static const BreakEvent BREAK_PC         = 1;  //!< Break point
static const BreakEvent BREAK_ACC_READ   = 2;  //!< Access Break (read)
static const BreakEvent BREAK_ACC_WRITE  = 4;  //!< Access Break (write)
#ifdef SUPPORT_SC_HEAP
static const BreakEvent BREAK_TIMEOUT    = 8;
static const BreakEvent BREAK_ACCEPT_INT = 16;
static const BreakEvent BREAK_TRACE_FULL = 32;
static const BreakEvent BREAK_PC_MATCH   = 64;
static const BreakEvent BREAK_GUARD_ERR  = 128; //!< CFG/GRG illegal access error or guard error
static const BreakEvent BREAK_STEP_EXE   = 256;
static const BreakEvent BREAK_CHANGE_PSWH = 512;
#endif // SUPPORT_SC_HEAP
static const BreakEvent BREAK_MAX        = 1024; //!< Max Break


const uint32_t TARGET_IS_SCHEAP = 0xFFFFFFFF;

// プログラムレジスタ
//! Program register.
static const CReg GR_NONE = -1;  //!< Sentinel. : 番兵
// ベクタレジスタ用オフセット (0x40 - 0x5fまでをベクトルレジスタとして予約)
//! Offset for the vector register.(vector registers are in 0x40 - 0x5f)
static const CReg WREG_BASE = 0x20;
static const CReg GREG_NUM = 0x20;  //!< Sentinel for all registers. : 全レジスタ用番兵
static const CReg WREG_NUM = 0x20;  //!< Sentinel for the vector register. : ベクトルレジスタ用番兵
//! Last ID that should be checked the dependency for the register.
static const CReg DEP_REG_MAX = 0x3f;  // レジスタ依存判定が必要な最終ID
// 00-0x1F(GREG), 0x20-0x3f(WREG) がレジスタの有効範囲
//! 00-0x1F(GREG), 0x20-0x3f(WREG) are valid range of the registers.
static const CReg WREG_MAX = WREG_BASE + WREG_NUM;
static const CReg DATA_REG_MAX = 0x3f;
static const CReg HMPSW_Z  = 0x800, HMPSW_S  = 0x801, HMPSW_OV = 0x802, HMPSW_CY = 0x803;
static const CReg HMPSW_SAT= 0x804, HMPSW_ID = 0x805, HMPSW_EP = 0x806, HMPSW_NP = 0x807;
static const CReg HMPSW_CM = 0x809, HMPSW_SS = 0x80B, HMPSW_EBV= 0x80F, HMPSW_HVC= 0x813;
static const CReg HMPSW_EIMASK=0x814, HMPSW_UM = 0x81E, HMPSW_VM = 0x81F;

static const CReg GMPSW_Z  = 0x820, GMPSW_S  = 0x821, GMPSW_OV = 0x822, GMPSW_CY = 0x823;
static const CReg GMPSW_SAT= 0x824, GMPSW_ID = 0x825, GMPSW_EP = 0x826, GMPSW_NP = 0x827;
static const CReg GMPSW_CM = 0x829, GMPSW_SS = 0x82B, GMPSW_EBV= 0x82F, GMPSW_HVC= 0x833;
static const CReg GMPSW_EIMASK=0x834, GMPSW_UM = 0x83E, GMPSW_VM = 0x83F;

//static const CReg SESR_OV = 0x820, SESR_SOV = 0x821;
static const CReg BIT_PRINT_MIN = HMPSW_Z;
static const CReg BIT_PRINT_MAX = GMPSW_VM;
//! To display the dependency for ISSUE_RATE.
static const CReg CF_ISSUE = 0x900;   // ISSUE_RATEに対する依存表現用
//! To save the result of R, that is from RMW checking.
static const CReg CF_RMW_BUF = 0x901; // RMWのR結果格納用temporal


//! Simulation mode
enum SimulationMode {
    FS_MODE = 0,    //!< function simulation
    CS_MODE = 1     //!< cycle simulation
};


typedef bool LsabType; //!< #LSAB_EIINT or #LSAB_NORMAL .
static const LsabType LSAB_EIINT = true;   //!< LSAB (Suspended) 0xB6
static const LsabType LSAB_NORMAL = false; //!< LSAB (Re-Exec) 0xB5

// システムレジスタ
#include "./sreg_enum.h"


// レジスタ/メモリアクセス記録数の上限
//! Maximum number for the records of the access for the registers or memories.
static const uint8_t REC_RREG_NUM = 128;
#ifdef ENABLE_DBG_BREAK
static const uint16_t REC_WREG_NUM = 2450;
#else // ENABLE_DBG_BREAK
static const uint8_t REC_WREG_NUM = 128;
#endif // ENABLE_DBG_BREAK
static const uint8_t REC_RVREG_NUM = 32;
static const uint8_t REC_WVREG_NUM = 32;
static const uint8_t REC_RMEM_NUM = 96; //!< LDM.MP 32ch
static const uint8_t REC_WMEM_NUM = 96; //!< STM.MP 32ch
static const uint8_t REC_FETCH_NUM = 2;
//! +2 is the access of Cache instruction.
static const uint8_t REC_IC_NUM = REC_FETCH_NUM + 2; // +2 はCache命令によるICアクセス
static const uint8_t WREG_WAY_NUM = 4;
#ifdef FROG_API_IF
static const uint8_t REC_LLBIT_NUM = 32;
#endif //FROG_API_IF


// 分岐命令タイプ
//! The type of the branch instruction.
enum BranchType {
    BR_NOT, BR_NORM, BR_PRED
};


// 例外コード
//! Exception code.
enum ExpCode {
    EXP_NONE = 0,       //!< no exception / interruption
    EXP_RESET,          //!< reset
    EXP_DBNMI,          //!< DBNMI
    EXP_RLB,            //!< RLB
    EXP_DBINT,          //!< DBINT
    EXP_FENMI,          //!< FENMI
    EXP_SYSERR_HM,      //!< SYSERR (operand access on host mode or conventional mode)
    EXP_SYSERR_RB,      //!< SYSERR (push to register bank)
    EXP_FEINT,          //!< FEINT
    EXP_GMFEINT,        //!< GMFEINT
    EXP_BGFEINT,        //!< BGFEINT
    EXP_FPI,            //!< FPU Exception (Imprecise)
    EXP_EIINT,          //!< EIINT
    EXP_GMEIINT,        //!< GMEIINT
    EXP_BGEIINT,        //!< BGEIINT
    EXP_SYSERR_GM,      //!< SYSERR (operand access on Guest mode)
    EXP_LSAB_EIINT,     //!< LSAB on EIINT
    EXP_MDPHM_EIINT,    //!< MDP (Host entry) on EIINT
    EXP_MDPHM_GMEIINT,  //!< MDP (Host entry) on GMEIINT
    EXP_MDPGM_EIINT,    //!< MDP (Guest entry or conventional mode) on EIINT
    EXP_MDPGM_GMEIINT,  //!< MDP (Guest entry) on GMEIINT
    EXP_PCB_DB,         //!< PCB
    EXP_LSAB_DB,        //!< LSAB
    EXP_AE,             //!< AE
    EXP_MIPHM,          //!< MIP (Host entry)
    EXP_MIPGM,          //!< MIP (Guest entry or conventional mode)
    EXP_SYSERR_IF,      //!< SYSERR (instruction fetch)
    EXP_SYSERR_EX,      //!< SYSERR (pop from register bank)
    EXP_RIE,            //!< RIE
    EXP_UCPOP0,         //!< UCPOP0
    EXP_UCPOP1,         //!< UCPOP1
    EXP_UCPOP2,         //!< UCPOP2
    EXP_PIE,            //!< PIE
    EXP_MAE,            //!< MAE
    EXP_MDPHM,          //!< MDP (Host entry)
    EXP_MDPGM,          //!< MDP (Guest entry or conventional mode)
    EXP_FPE,            //!< FPE
    EXP_FXE,            //!< FXE
    EXP_DBTRAP,         //!< DBTRAP
    EXP_HVTRAP,         //!< HVTRAP
    EXP_SYSCALL,        //!< SYSCALL
    EXP_FETRAP,         //!< FETRAP
    EXP_TRAP0,          //!< TRAP0
    EXP_TRAP1,          //!< TRAP1
    EXP_GUARD,          //!< guard bit (no useable)
};

// エラーコード
//! Error code.
enum ErrorCode {
    E_OK                   = 0,   //!< Normal operation
    E_MEMORY_NOT_MAPPED    = 1,   //!< inst access to unallocated memory area
    E_MEMORY_WRITE_PROTECT = 2,   //!< RO area is written
    E_MEMORY_FETCH_PROTECT = 4,   //!< Fetch to area that can not be fetched
    E_MEMORY_READ_PROTECT  = 5,   //!< Read to area that can not be read
#ifdef SUPPORT_CUBESUITE
#else // SUPPORT_CUBESUITE
    E_MEMORY_NOT_INITIALIZED = 6  //!< Read to uninitialized memory
#endif // SUPPORT_CUBESUITE
};


// CForest終了ステータス
// Exit status of CForest.
static const int EXIT_PASS     = 0;  //!< Exit with the setting RESULT of zero. : RESULTに0がセットされて終了
static const int EXIT_FAIL     = 1;  //!< Exit with the setting RESULT of non-zero. : RESULTに非0がセットされて終了
static const int EXIT_NOTSET   = 2;  //!< Exit without the setting RESULT. : RESULTがセットされずに終了
static const int EXIT_SIGINT   = 3;  //!< Exit with SIGINT. : SIGINTで終了
static const int EXIT_MAX      = 4;  //!< Exit with MAX. : MAXで終了
static const int EXIT_NORMAL   = 5;  //!< Nornal exit with expectations. : CForest通常終了(想定された動作によるexit)
static const int EXIT_ABNORMAL = 6;  //!< Abnormal exit.(For example, by the internal error.) : CForest異常終了(内部エラーなど)
static const int EXIT_BAD_ALLOC = 7; //!< Exits with the exception of bad_alloc fail of 'new'. :  new失敗によるbad_alloc例外終了

//! It can be constant variable of NC, TC0, VC0 or ERR_CONTEXT_ID
typedef int32_t ContextId;
static const ContextId NC  = -1;
static const ContextId TC0 =  0;
static const ContextId VC0 =  0;
static const ContextId ERR_CONTEXT_ID = 0x0000FFFE;

// メッセージレベル(非公開分)
static const MessageLevel MSG_INF = 3; //!< Message level(non-disclosure).


// デバッグモード(非公開分)
static const DebugMode DBG_DEP  = 0x00000008;  //!< Debug mode(non-disclosure).
static const DebugMode DBG_BRA  = 0x00000010;  //!< Debug mode(non-disclosure).
static const DebugMode DBG_TIME = 0x00000020;  //!< Debug mode(non-disclosure).
static const DebugMode DBG_CACHE= 0x00000040;  //!< Debug mode(non-disclosure).
static const DebugMode DBG_STALL= 0x00000080;  //!< Debug mode(non-disclosure).
static const DebugMode DBG_TIMER = 0x00000100; //!< Debug mode(non-disclosure).
static const DebugMode DBG_PERF = 0x00000200;  //!< Debug mode(non-disclosure).
static const DebugMode DBG_FPUT = 0x00000400;  //!< Debug mode(non-disclosure).
static const DebugMode DBG_INTC = 0x00000800;  //!< Debug mode(non-disclosure).
static const DebugMode DBG_MASK = 0x0000FFF;   //!< Debug mode(non-disclosure).

//! Context privilege level.
typedef uint32_t PrivLevel; //!< PRIV_HAS_UM, PRIV_HAS_SV, etc.
static const PrivLevel PRIV_HAS_UM   = 0x01;  //!< Privilege is UM.    : UM権限
static const PrivLevel PRIV_HAS_DBUM = 0x02;  //!< Privilege is DB.    : DB権限
static const PrivLevel PRIV_HAS_SV   = 0x04;  //!< Privilege is SV.    : SV権限
static const PrivLevel PRIV_HAS_DBSV = 0x08;  //!< Privilege is DB&SV. : DB&SV権限
static const PrivLevel PRIV_HAS_HV   = 0x10;  //!< Privilege is HV.    : HV権限
static const PrivLevel PRIV_HAS_DBHV = 0x20;  //!< Privilege is DB&HV. : DB&HV権限


// Required privilege level by resource.
// bit [5] = DB&HV
// bit [4] = HV
// bit [3] = DB&SV
// bit [2] = SV
// bit [1] = DB&UM
// bit [0] = UM
static const PrivLevel PRIV_PERMIT_UM   = 0x3F;  //!< UM (no need)
static const PrivLevel PRIV_PERMIT_SV   = 0x3C;  //!< SV privilege
static const PrivLevel PRIV_PERMIT_HV   = 0x30;  //!< HV privilege
static const PrivLevel PRIV_PERMIT_DBUM = 0x2A;  //!< DB privilege
static const PrivLevel PRIV_PERMIT_DBSV = 0x28;  //!< DB&SV privilege
static const PrivLevel PRIV_PERMIT_DBHV = 0x20;  //!< DB&HV privilege
static const PrivLevel PRIV_PERMIT_NONE = 0x0;   //!< Unpermit


// PIE checking.
// If none-DB mode accesses DB registers, access is treated as undefined reg access (=SV reg access)
// If DB mode accesses DB registers, access is checked as usual.
static const PrivLevel PRIV_PIE_UM   = 0x3F;  //!< UM (no need)
static const PrivLevel PRIV_PIE_SV   = 0x3C;  //!< Permit DBHV,HV,DBSV,SV
static const PrivLevel PRIV_PIE_HV   = 0x30;  //!< Permit DBHV,HV
static const PrivLevel PRIV_PIE_DBUM = 0x3E;  //!< Permit DBHV,HV,DBSV,SV,DBUM
static const PrivLevel PRIV_PIE_DBSV = 0x3C;  //!< Permit DBHV,HV,DBSV,SV
static const PrivLevel PRIV_PIE_DBHV = 0x34;  //!< Permit DBHV,HV,SV


//! Co-Processor type. : コプロセッサタイプ
typedef uint32_t CoproType;
static const CoproType COPRO_NO  = 0;
static const CoproType COPRO_FPU  = 1;
static const CoproType COPRO_SIMD = 2;

//! Time elapsed or not.
typedef bool IsElapsedTime;
static const IsElapsedTime TIME_ELAPSED = true;
static const IsElapsedTime TIME_NOT_ELAPSED = false;


//! For CubeSuite.
enum APIErrorCode {
    API_E_OK = 0,
    API_E_PARAM = 1,
    API_E_MULTI_ALLOC = 2,
    API_E_NOT_MAPPED = 3,
    API_E_MULTI_INTREQ = 4,
    API_F_NO_WORKAREA = 0x100
};


const ExpCause EXP_CAUSE_NONE = 0;


const SysErrCause SYSERR_NONE    = 0x00;
const SysErrCause SYSERR_MACHINE = 0x10;  //!< Reserved. : 予約
const SysErrCause SYSERR_ICFB    = 0x11;
const SysErrCause SYSERR_ICACHE  = 0x12;
const SysErrCause SYSERR_IBUS    = 0x13;
const SysErrCause SYSERR_DBUS    = 0x14;
const SysErrCause SYSERR_DCFB    = 0x15;
const SysErrCause SYSERR_LRAM    = 0x16;
const SysErrCause SYSERR_GRAM    = 0x17;  //!< Reserved. : 予約
const SysErrCause SYSERR_LBSU    = 0x18;  //!< Reserved. : 予約
const SysErrCause SYSERR_GBUS    = 0x19;
const SysErrCause SYSERR_EXT     = 0x1A;
const SysErrCause SYSERR_L2C     = 0x1B;
const SysErrCause SYSERR_INPUT0  = 0x1E;
const SysErrCause SYSERR_INPUT1  = 0x1F;
const SysErrCause SYSERR_NOP     = 0xFF; //!< Not SYSERR. Access will be NOP. : SYSERRにならない。アクセスNOP化

const SysErrCause SYSERR_MASK    = 0xFF; //!< If SYSERR_*>=0xFF, it's original spec. of CForest. : 0xFF以上はCForestの独自仕様。

// 本当のSYSERR要因とORされる。PEG起因のSYSERR（ECM通知対象)
const SysErrCause SYSERR_PEG     = 0x100;
const SysErrCause SYSERR_CRG     = 0x200;
const SysErrCause SYSERR_REC_MASTER  = 0x400;
const SysErrCause SYSERR_REC_SLAVE   = 0x800;

const SysErrCause SYSERR_PEG_M = SYSERR_PEG | SYSERR_REC_MASTER; //!< PEG error captured by PEGCAP_M
const SysErrCause SYSERR_PEG_S = SYSERR_PEG | SYSERR_REC_SLAVE;  //!< PEG error captured by PEGCAP_S

// G4MH2.0 syserr events
const SysErrCause SYSERR_SAFETY = 0x1A;   //!< Error response by Safety function error
const SysErrCause SYSERR_D_GUEST  = 0x19; //!< Error response by Guest privilege error on Data access
const SysErrCause SYSERR_BSLAVE = 0x18;   //!< Error response by Bus Slave
const SysErrCause SYSERR_I_GUEST  = 0x12; //!< Error response by Guest privilege error on Fetch access

//! Accessable from self only.
const MemRestrictType MRT_SELF_ONLY    = 0x10; // selfでのみアクセス可能
//! Accessable from specified PE only. Unsupported.
const MemRestrictType MRT_OWN_PE_ONLY  = 0x20; // 指定PEのみアクセス可能 // @@ 未対応

const int32_t NOT_SEL_THREAD = -1;

//! RtypeAccType
enum RtypeAccType {
    RTYPE_NORMAL    = 0,
    RTYPE_BITOP_SET = 1,
    RTYPE_BITOP_AND = 2,
    RTYPE_BITOP_OR  = 3,
    RTYPE_BITOP_XOR = 4,
    RTYPE_SYNC      = 5,
    RTYPE_CAXI      = 6,
    RTYPE_ATOMIC    = 7
};

//! VciReqCmd
enum VciReqCmd {
    VCI_REQ_READ  = 0,
    VCI_REQ_WRITE = 1,
    VCI_REQ_BITOP = 2,
    VCI_REQ_SYNC  = 3,
    VCI_REQ_CAXI  = 4,
    VCI_REQ_LLSC  = 5,
};

//! ArchClass
enum ArchClass {
    E3V5_F,
    E3V5_V,
    E3V5_S,
};


//! CallBack functions
enum ContextAttr {
    ATTR_NC = 0,
    ATTR_VC = 1,
    ATTR_TC = 2
};

//! DbufPreloadType
enum DbufPreloadType {
    DBUF_PRLD_NO = 0,
    DBUF_PRLD_PLUS = 1,
    DBUF_PRLD_MINUS = 2
};

enum MemResourceType {
    MEM_RES_LRAM = 0, // LRAM
    MEM_RES_CRAM = 1, // CRAM
    MEM_RES_FROM = 2, // FlashROM
    MEM_RES_OTHERS = 3, // Peripherals
};

const int32_t INTC1_CH_MAX = 31;
const int32_t INTC2_CH_MAX = 2047;
const int32_t INTC_NUM_FEINT = 16;
const int32_t INTC_CH_MAX = INTC2_CH_MAX + 1 + INTC_NUM_FEINT; //!< 2047ch + FEINT + FENMI
const int32_t INTC_CH_FENMI = INTC_CH_MAX;
const int32_t INTC_CH_FEINT = INTC_CH_MAX - 1;
const uint32_t INTC_EIINT_CHANNEL_OFFSET = 0;

const uint32_t SPID_MASK = 0x1F;
const uint32_t BMID_MASK = 0x1F;
const uint32_t PEID_MASK = 0x07;
const uint32_t CLID_MASK = 0x07;

const uint32_t CF_INVALID_PEID = PEID_MASK + 1;

// Cluster ID
const uint32_t CL_PEID_MASK  = 0x0000FFFF;
const uint32_t CL_PEID_WIDTH = 16;
const uint32_t CL_CLID_MASK  = 0xFFFF0000;
const uint32_t CL_CLID_WIDTH = 16;
const uint32_t CL_CLID_NONE  = 0;
