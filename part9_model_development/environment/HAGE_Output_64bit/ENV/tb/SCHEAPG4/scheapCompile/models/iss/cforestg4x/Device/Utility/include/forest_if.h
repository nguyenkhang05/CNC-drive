/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */
/*! @file */
#pragma once

#include <stdio.h>
#include "./config.h"

#ifdef _MSC_VER
    #define DLLEXPORT   __declspec(dllexport)
#else // _MSC_VER
    #define DLLEXPORT
#endif // _MSC_VER

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

typedef int32_t  CReg;
typedef uint32_t PhysAddr;
typedef uint32_t RegData;
typedef uint64_t RegData64;
typedef uint32_t MemData;
typedef uint64_t MemData64;
typedef int32_t  ByteSize;
typedef uint32_t MemRestrictType;
typedef uint32_t DebugMode;

/* メモリアクセス列挙型 */
//! Memory Access.
enum MemAccessCommand {
    INST_READ    = 0,
    DATA_READ    = 1,
    DATA_WRITE   = 2,
    NO_MEMACC    = 3
};

/* メモリ割当て領域列挙型 */
//! Memory mapping Area.
enum MemMappedArea {
    MI_CF_SELF      = 0x0100, //!< Self area
    MI_CF_IOR       = 0x1000, //!< Global APB IOR area
    MI_CF_LOCAL_IOR = 0x1001, //!< Local APB IOR area
    MI_CF_FROM      = 0x2001, //!< FlashROM area
    MI_CF_LRAM      = 0x2002, //!< LocalRAM area
    MI_CF_CRAM      = 0x2003, //!< ClusterRAM area
    MI_CF_OTHER     = 0x2004, //!< External memory
};

enum NotifySimState {
    SIMULATE_ON      = 0x0,  //!< CPU is in operation(Not Halt/snooze state & Clock active).
    SIMULATE_HALT    = 0x1,  //!< Halt state.
    SIMULATE_STOP    = 0x2,  //!< Standby control SFRs supported in peripheral sim.
    SIMULATE_RESET   = 0x3,  //!< RESET state(DBGRESET, reset pin, internal reset).
    SIMULATE_SNOOZE  = 0x4,  //!< Snooze state.
    SIMULATE_DISABLE = 0x5   //!< Context is disabled.
};

//! TraceInstType in forest_if.h.
enum TraceInstType {
    TRACE_DBCP   = 0x0, //!< DBCP instruction
    TRACE_DBTAG  = 0x1, //!< DBTAG instruction
    TRACE_DBPUSH = 0x2  //!< DBPUSH instruction
};


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

//! API Error Code.
enum APIErrorCode {
    API_E_OK = 0,
    API_E_PARAM = 1,
    API_E_MULTI_ALLOC = 2,
    API_E_NOT_MAPPED = 3,
    API_E_MULTI_INTREQ = 4,
    API_F_NO_WORKAREA = 0x100
};

//! Cache Size.
enum CacheSize {
    CACHE_SIZE_0   = 0,
    CACHE_SIZE_1K  = 1,
    CACHE_SIZE_2K  = 2,
    CACHE_SIZE_4K  = 3,
    CACHE_SIZE_8K  = 4,
    CACHE_SIZE_16K = 5,
    CACHE_SIZE_32K = 6,
    CACHE_SIZE_64K = 7
};

//! Cache Way.
enum CacheWay {
    CACHE_WAY_0 = 0,
    CACHE_WAY_1 = 1,
    CACHE_WAY_2 = 2,
    CACHE_WAY_4 = 4
};

//! Cache ECC.
enum CacheECC {
    ECC_NO_EXIST = 0,
    ECC_EXIST    = 1
};

//! Sim Attribute.
enum SimAttr {
    ALL_PE = 0,
    S_ATTR_PE = 1
};

enum SetPEInfoID {
    SNOOZE_COUNT = 0x0,
    FPU_EXIST    = 0x10,
    FXU_EXIST    = 0x21,
    MPU_EXIST    = 0x40,
    MPU_REGION_NUMBER = 0x43,
    MPU_MDP_SUPPRESSED = 0x44,
    MPU_BANK_NUMBER    = 0x45, //!< The number of MPU bank (MPCFG.NBK+1)
    L1RAM_LINK_NUMBER  = 0x50, //!< The number of L1RAM Link resource (The value of L1RCFG.L1RLNKE)
    L1RAM_SIZE   = 0x51, //!< The size of L1RAM (The value of L1RCFG.L1RSIZE)
    LSCFG_LSE    = 0x58, //!< Supportance of Lockstep functioon (The vale of LSCFG.LSE)
    HV_EXIST     = 0x60, //!< 1: HV function is exist, 0:Not exist
    HV_HVE       = 0x61, //!< The value of HVCFG.HVE
    HV_DBGEN     = 0x62, //!< The value of DBGEN
    NC_RBASE     = 0x101,
    NC_ICCTRL    = 0x104,
    NC_PEID      = 0x105,
    NC_SPID      = 0x106,
    NC_SPIDLIST  = 0x107,
    NC_BMID      = 0x108
};

//! GetPEInfoID
enum GetPEInfoID {
    PIPELINE_STAGE_NUMBER = 0x80
};

enum CoreType {
    G4MH   = 0x0,   //!< G4MH1.0
    G4MH11 = 0x1,   //!< G4MH1.1
    G4MH20 = 0x10,  //!< G4MH2.0
};

enum ErrorCode {
    // エラー
    E_OK = 0,                     //!< Normal operation
    E_MEMORY_NOT_MAPPED = 1,      //!< inst access to unallocated memory area
    E_MEMORY_WRITE_PROTECT = 2,   //!< RO area is written
    E_MEMORY_FETCH_PROTECT = 4,   //!< Fetch to area that can not be fetched
    E_MEMORY_READ_PROTECT  = 5    //!< Read to area that can not be fetched
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
#ifdef ENABLE_BARRIER_CSP
    BaseAddr_BAR   = 0x04, // Base Address of the Barrier
#endif // #ifdef ENABLE_BARRIER_CSP

    // When SetIORBaseAddrID is updated,
    // CubeSuiteIF::UpdateLocalIOROffsetAddr must be updated also.
};


static const DebugMode DBG_OFF  = 0x00000000;
static const DebugMode DBG_ON   = 0x00000001;
static const DebugMode DBG_RW   = 0x00000002;

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
static const uint8_t REC_IC_NUM = REC_FETCH_NUM + 2;
static const uint8_t WREG_WAY_NUM = 4;
static const uint32_t ALLOC_GLOBAL = 0xFFFF;
#ifdef FROG_API_IF
static const uint8_t REC_LLBIT_NUM = 32;
#endif //FROG_API_IF

struct MemoryBlock {
    MemRestrictType  mblock_restrict_type;
    uint32_t    mblock_fetch_latency;
    uint32_t    mblock_read_latency;
    uint32_t    mblock_write_latency;
    PhysAddr    start_address;
    PhysAddr    end_address;
    MemMappedArea   mblock_area;
    int32_t     offset;
    uint32_t    pe_id;
    uint32_t    cl_id;
    uint32_t    cl_fetch_latency;        //!< fetch access latency from different PE but same CL
    uint32_t    cl_read_latency;         //!< read access latency from different PE but same CL
    uint32_t    cl_write_latency;        //!< write access latency for different PE but same CL
    uint32_t    exo_cl_fetch_latency;    //!< fetch access latency from different CL
    uint32_t    exo_cl_read_latency;     //!< read access latency from different CL
    uint32_t    exo_cl_write_latency;    //!< write access latency for different CL
    uint32_t    num_outstanding;
    uint32_t    repeat_rate;
    ByteSize*   bank_size;
    int32_t     bank_size_count;
};

struct CacheSpec {
    CacheSize size;
    CacheWay way;
    CacheECC ecc;
};

struct CacheInfo {
    uint64_t    Miss_count;
    uint64_t    Hit_count;
};


struct ExtTraceInfo {
    uint64_t glbl_systime;
    uint64_t ctxt_systime;
};


//! TraceInfo
typedef uint32_t ExpCause;
const ExpCause EXP_CAUSE_NONE = 0;
//! Context privilege level
typedef uint32_t PrivLevel;
#include "./trace_info.h"

class CForestIF;

extern "C"
{
    DLLEXPORT CForestIF* MakeCoreModule (const wchar_t* dll_name);
    DLLEXPORT APIErrorCode DestroyCoreModule (void);
}

class CForestIF {
public:
    virtual APIErrorCode AddPEInfo (uint32_t cl_id, uint32_t pe_id, CoreType core_type) = 0;
    virtual APIErrorCode InstantiatePE (void) = 0;
    virtual APIErrorCode SetPEInfo (uint32_t pe_id, SetPEInfoID pe_info_id,
                                    uint32_t info_data) = 0;
    virtual APIErrorCode GetPEInfo (uint32_t pe_id, GetPEInfoID pe_info_id,
                                    uint32_t* info_data) = 0;
    virtual APIErrorCode StepExecute (TraceInfo** trace_info, ExtTraceInfo** ext_trace_info) = 0;
    virtual APIErrorCode HardReset (SimAttr attr, uint32_t pe_id) = 0;
    virtual APIErrorCode VMemWriteDebug (uint32_t pe_id, PhysAddr addr,
                                         uint32_t data, ByteSize size) = 0;
    virtual APIErrorCode VMemReadDebug  (uint32_t pe_id, PhysAddr addr,
                                         ByteSize size, uint32_t* data) = 0;
    virtual APIErrorCode PMemWriteDebug (PhysAddr addr, uint32_t data,
                                         ByteSize size) = 0;
    virtual APIErrorCode PMemReadDebug  (PhysAddr addr, ByteSize size,
                                         uint32_t* data) = 0;
    virtual APIErrorCode SetMemBlock (uint32_t id, MemoryBlock mem_block) = 0;
    virtual APIErrorCode GetMemBlock (uint32_t id, MemoryBlock* mem_block) = 0;
    virtual APIErrorCode DeleteMemBlock (uint32_t id) = 0;
    virtual APIErrorCode RegWriteDebug (uint32_t pe_id, CReg regno,
                                        const uint32_t* data) = 0;
    virtual APIErrorCode RegReadDebug  (uint32_t pe_id, CReg regno,
                                        uint32_t* data) = 0;
    virtual APIErrorCode SetICacheSpec (uint32_t pe_id, CacheSpec  cachespec) = 0;
    virtual APIErrorCode GetICacheSpec (uint32_t pe_id, CacheSpec* cachespec) = 0;
    virtual APIErrorCode GetICacheInfo (uint32_t pe_id, CacheInfo* cacheinfo) = 0;
    virtual APIErrorCode SetDebugMode  (DebugMode mode) = 0;
    virtual APIErrorCode SetFileOut (const wchar_t* file_pathname) = 0;
    virtual APIErrorCode GetTraceInfo  (TraceInfo** trace) = 0;
    virtual APIErrorCode GetPeTraceInfo (uint32_t pe_id, TraceInfo** trace) = 0;
    virtual APIErrorCode SetModeFrequency(uint32_t pe_id, uint32_t ratio) = 0;
    virtual APIErrorCode SetInterruptToIntc1 (uint32_t pe_id, uint32_t channel) = 0;
    virtual APIErrorCode SetInterruptToIntc2 (uint32_t channel) = 0;
    virtual APIErrorCode SetFeintToIntc1 (uint32_t pe_id, uint32_t channel) = 0;
    virtual APIErrorCode SetFenmiToIntc1 (uint32_t pe_id) = 0;
    virtual APIErrorCode SetIORBaseAddr (SetIORBaseAddrID peripheral_id,
                                         PhysAddr base_addr) = 0;
    virtual APIErrorCode SetLocalIOROffsetAddr (PhysAddr start_address,
                                                PhysAddr end_addr,
                                                int32_t offset) = 0;
    virtual APIErrorCode ReqPseudoEiint (uint32_t pe_id, uint32_t channel,
                                         uint32_t priority, bool eitb) = 0;
    virtual APIErrorCode ReqPseudoFenmi (uint32_t pe_id) = 0;
    virtual APIErrorCode ReqPseudoFeint (uint32_t pe_id, uint32_t channel) = 0;
    virtual APIErrorCode ReqPseudoSyserr (uint32_t pe_id, uint32_t syserr_cause) = 0;
    virtual APIErrorCode ReqPseudoOperandSyserr (uint32_t peid, uint32_t syserr_cause,
                                                 bool is_gm, uint32_t gpid) = 0;
    virtual ~CForestIF () {}
};
