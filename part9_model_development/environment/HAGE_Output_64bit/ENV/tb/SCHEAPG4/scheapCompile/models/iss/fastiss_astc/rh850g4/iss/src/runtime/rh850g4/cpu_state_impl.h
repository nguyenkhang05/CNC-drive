//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2020.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file cpu_state_impl.h
 * \brief Defines the CPU state implementation class
 */

#ifndef _RH850G4_CPU_STATE_IMPL_H_
#define _RH850G4_CPU_STATE_IMPL_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850g4/cpu_state_if.h"

//------------------------------------------------------------------------------
// Class pre-declarations
//------------------------------------------------------------------------------

namespace fastiss
{
  class i1_t;
  class i8_t;
  class i16_t;
  class i32_t;
  class i64_t;
  class block_function_t;
  class code_gen_if_t;
  class struct_type_t;
  class func_type_t;
  class func_arg_t;
}

//------------------------------------------------------------------------------
// rh850g4 CPU state definition
//------------------------------------------------------------------------------

/**
 * \brief Contains rh850g4 specific components
 */
namespace rh850g4
{
  /**
   * \brief rh850g4 CPU state structure indices
   */
  typedef enum
  {
    // Instruction count
    INDEX_INSTRUCTION_COUNT = 0,

    // Time
    INDEX_LOCAL_TIME = 1,
    INDEX_ACCRUED_TIME = 2,

    // Cycle period
    INDEX_CYCLE_PERIOD = 3,

    // Core instance pointer
    INDEX_CORE_PTR = 4,
    
    // Check watchpoint hit flag
    INDEX_CHECK_WATCHPOINT_HIT = 5,

    // Watchpoint hit flag
    INDEX_WATCHPOINT_HIT = 6,
    
    // Last access address
    INDEX_LAST_ACCESS_ADDRESS = 7,
    
    // Last access size
    INDEX_LAST_ACCESS_SIZE = 8,
    
    // Last access data
    INDEX_LAST_ACCESS_DATA = 9,
    
    // Registers and register fields
    INDEX_TRANS_PC = 10,
    INDEX_R0 = 11,
    INDEX_R1 = 12,
    INDEX_R2 = 13,
    INDEX_R3 = 14,
    INDEX_R4 = 15,
    INDEX_R5 = 16,
    INDEX_R6 = 17,
    INDEX_R7 = 18,
    INDEX_R8 = 19,
    INDEX_R9 = 20,
    INDEX_R10 = 21,
    INDEX_R11 = 22,
    INDEX_R12 = 23,
    INDEX_R13 = 24,
    INDEX_R14 = 25,
    INDEX_R15 = 26,
    INDEX_R16 = 27,
    INDEX_R17 = 28,
    INDEX_R18 = 29,
    INDEX_R19 = 30,
    INDEX_R20 = 31,
    INDEX_R21 = 32,
    INDEX_R22 = 33,
    INDEX_R23 = 34,
    INDEX_R24 = 35,
    INDEX_R25 = 36,
    INDEX_R26 = 37,
    INDEX_R27 = 38,
    INDEX_R28 = 39,
    INDEX_R29 = 40,
    INDEX_R30 = 41,
    INDEX_R31 = 42,
    INDEX_PC = 43,
    INDEX_WR0 = 44,
    INDEX_WR0_U = 45,
    INDEX_WR1 = 46,
    INDEX_WR1_U = 47,
    INDEX_WR2 = 48,
    INDEX_WR2_U = 49,
    INDEX_WR3 = 50,
    INDEX_WR3_U = 51,
    INDEX_WR4 = 52,
    INDEX_WR4_U = 53,
    INDEX_WR5 = 54,
    INDEX_WR5_U = 55,
    INDEX_WR6 = 56,
    INDEX_WR6_U = 57,
    INDEX_WR7 = 58,
    INDEX_WR7_U = 59,
    INDEX_WR8 = 60,
    INDEX_WR8_U = 61,
    INDEX_WR9 = 62,
    INDEX_WR9_U = 63,
    INDEX_WR10 = 64,
    INDEX_WR10_U = 65,
    INDEX_WR11 = 66,
    INDEX_WR11_U = 67,
    INDEX_WR12 = 68,
    INDEX_WR12_U = 69,
    INDEX_WR13 = 70,
    INDEX_WR13_U = 71,
    INDEX_WR14 = 72,
    INDEX_WR14_U = 73,
    INDEX_WR15 = 74,
    INDEX_WR15_U = 75,
    INDEX_WR16 = 76,
    INDEX_WR16_U = 77,
    INDEX_WR17 = 78,
    INDEX_WR17_U = 79,
    INDEX_WR18 = 80,
    INDEX_WR18_U = 81,
    INDEX_WR19 = 82,
    INDEX_WR19_U = 83,
    INDEX_WR20 = 84,
    INDEX_WR20_U = 85,
    INDEX_WR21 = 86,
    INDEX_WR21_U = 87,
    INDEX_WR22 = 88,
    INDEX_WR22_U = 89,
    INDEX_WR23 = 90,
    INDEX_WR23_U = 91,
    INDEX_WR24 = 92,
    INDEX_WR24_U = 93,
    INDEX_WR25 = 94,
    INDEX_WR25_U = 95,
    INDEX_WR26 = 96,
    INDEX_WR26_U = 97,
    INDEX_WR27 = 98,
    INDEX_WR27_U = 99,
    INDEX_WR28 = 100,
    INDEX_WR28_U = 101,
    INDEX_WR29 = 102,
    INDEX_WR29_U = 103,
    INDEX_WR30 = 104,
    INDEX_WR30_U = 105,
    INDEX_WR31 = 106,
    INDEX_WR31_U = 107,
    INDEX_EIPC = 108,
    INDEX_EIPSW = 109,
    INDEX_FEPC = 110,
    INDEX_FEPSW = 111,
    INDEX_PSW_Z = 112,
    INDEX_PSW_S = 113,
    INDEX_PSW_OV = 114,
    INDEX_PSW_CY = 115,
    INDEX_PSW_SAT = 116,
    INDEX_PSW_ID = 117,
    INDEX_PSW_EP = 118,
    INDEX_PSW_NP = 119,
    INDEX_PSW_EBV = 120,
    INDEX_PSW_CU0 = 121,
    INDEX_PSW_CU1 = 122,
    INDEX_PSW_CU2 = 123,
    INDEX_PSW_EIMASK = 124,
    INDEX_PSW_UM = 125,
    INDEX_FPSR_CC = 126,
    INDEX_FPSR_FN = 127,
    INDEX_FPSR_IF = 128,
    INDEX_FPSR_PEM = 129,
    INDEX_FPSR_RM = 130,
    INDEX_FPSR_FS = 131,
    INDEX_FPSR_XC = 132,
    INDEX_FPSR_XE = 133,
    INDEX_FPSR_XP = 134,
    INDEX_FPEPC = 135,
    INDEX_FPST_XC = 136,
    INDEX_FPST_IF = 137,
    INDEX_FPST_XP = 138,
    INDEX_FPCC_CC = 139,
    INDEX_FPCFG_RM = 140,
    INDEX_FPCFG_XE = 141,
    INDEX_EIIC = 142,
    INDEX_FEIC = 143,
    INDEX_PSWH_GPID = 144,
    INDEX_PSWH_GM = 145,
    INDEX_CTPC = 146,
    INDEX_CTPSW = 147,
    INDEX_EIPSWH_GPID = 148,
    INDEX_EIPSWH_GM = 149,
    INDEX_FEPSWH_GPID = 150,
    INDEX_FEPSWH_GM = 151,
    INDEX_CTBP = 152,
    INDEX_SNZCFG = 153,
    INDEX_EIWR = 154,
    INDEX_FEWR = 155,
    INDEX_SPID = 156,
    INDEX_SPIDLIST = 157,
    INDEX_RBASE_RBASE = 158,
    INDEX_RBASE_DV = 159,
    INDEX_RBASE_RINT = 160,
    INDEX_EBASE_EBASE = 161,
    INDEX_EBASE_DV = 162,
    INDEX_EBASE_RINT = 163,
    INDEX_INTBP = 164,
    INDEX_MCTL_UIC = 165,
    INDEX_PID = 166,
    INDEX_SVLOCK = 167,
    INDEX_SCCFG_SIZE = 168,
    INDEX_SCBP = 169,
    INDEX_HVCFG_HVE = 170,
    INDEX_GMCFG_GMP = 171,
    INDEX_GMCFG_HMP = 172,
    INDEX_GMCFG_GSYSE = 173,
    INDEX_GMCFG_GCU0 = 174,
    INDEX_GMCFG_GCU1 = 175,
    INDEX_GMCFG_GCU2 = 176,
    INDEX_HVSB = 177,
    INDEX_PEID = 178,
    INDEX_BMID_BMID = 179,
    INDEX_MEA = 180,
    INDEX_MEI = 181,
    INDEX_RBCR0_MD = 182,
    INDEX_RBCR0_BE = 183,
    INDEX_RBCR1_NC = 184,
    INDEX_RBNR_BN = 185,
    INDEX_RBIP_RBIP = 186,
    INDEX_ISPR_ISP = 187,
    INDEX_IMSR_EEIM = 188,
    INDEX_IMSR_EPLM = 189,
    INDEX_IMSR_EID = 190,
    INDEX_IMSR_ENP = 191,
    INDEX_IMSR_FNP = 192,
    INDEX_IMSR_HEEIM = 193,
    INDEX_IMSR_HEPLM = 194,
    INDEX_IMSR_HEID = 195,
    INDEX_IMSR_HENP = 196,
    INDEX_IMSR_HFNP = 197,
    INDEX_ICSR_PMEI = 198,
    INDEX_INTCFG_ISPC = 199,
    INDEX_INTCFG_EPL = 200,
    INDEX_INTCFG_ULNR = 201,
    INDEX_PLMR_PLM = 202,
    INDEX_FXSR_FN = 203,
    INDEX_FXSR_IF = 204,
    INDEX_FXSR_PEM = 205,
    INDEX_FXSR_RM = 206,
    INDEX_FXSR_FS = 207,
    INDEX_FXSR_XC = 208,
    INDEX_FXSR_XE = 209,
    INDEX_FXSR_XP = 210,
    INDEX_FXST_XC = 211,
    INDEX_FXST_IF = 212,
    INDEX_FXST_XP = 213,
    INDEX_FXINFO_NFPU = 214,
    INDEX_FXINFO_RSIZE = 215,
    INDEX_FXCFG_RM = 216,
    INDEX_FXCFG_XE = 217,
    INDEX_FXXC_XC3 = 218,
    INDEX_FXXC_XC2 = 219,
    INDEX_FXXC_XC1 = 220,
    INDEX_FXXC_XC0 = 221,
    INDEX_FXXP_XP3 = 222,
    INDEX_FXXP_XP2 = 223,
    INDEX_FXXP_XP1 = 224,
    INDEX_FXXP_XP0 = 225,
    INDEX_GMEIPC = 226,
    INDEX_GMEIPSW = 227,
    INDEX_GMFEPC = 228,
    INDEX_GMFEPSW = 229,
    INDEX_GMPSW_Z = 230,
    INDEX_GMPSW_S = 231,
    INDEX_GMPSW_OV = 232,
    INDEX_GMPSW_CY = 233,
    INDEX_GMPSW_SAT = 234,
    INDEX_GMPSW_ID = 235,
    INDEX_GMPSW_EP = 236,
    INDEX_GMPSW_NP = 237,
    INDEX_GMPSW_EBV = 238,
    INDEX_GMPSW_CU0 = 239,
    INDEX_GMPSW_CU1 = 240,
    INDEX_GMPSW_CU2 = 241,
    INDEX_GMPSW_EIMASK = 242,
    INDEX_GMPSW_UM = 243,
    INDEX_GMMEA = 244,
    INDEX_GMMEI = 245,
    INDEX_GMEIIC = 246,
    INDEX_GMFEIC = 247,
    INDEX_GMSPID = 248,
    INDEX_GMSPIDLIST = 249,
    INDEX_GMEBASE_EBASE = 250,
    INDEX_GMEBASE_DV = 251,
    INDEX_GMEBASE_RINT = 252,
    INDEX_GMINTBP = 253,
    INDEX_GMINTCFG_ISPC = 254,
    INDEX_GMINTCFG_EPL = 255,
    INDEX_GMINTCFG_ULNR = 256,
    INDEX_GMPLMR_PLM = 257,
    INDEX_GMSVLOCK = 258,
    INDEX_GMMPM = 259,
    INDEX_GMEIWR = 260,
    INDEX_GMFEWR = 261,
    INDEX_GMPEID = 262,
    INDEX_MPM = 263,
    INDEX_MPCFG_NMPUE = 264,
    INDEX_MPCFG_HBE = 265,
    INDEX_MPCFG_ARCH = 266,
    INDEX_MPCFG_NBK = 267,
    INDEX_MPCFG_DMDP = 268,
    INDEX_MCA = 269,
    INDEX_MCS = 270,
    INDEX_MCC = 271,
    INDEX_MCR_HSXE = 272,
    INDEX_MCR_HSWE = 273,
    INDEX_MCR_HSRE = 274,
    INDEX_MCR_HUXE = 275,
    INDEX_MCR_HUWE = 276,
    INDEX_MCR_HURE = 277,
    INDEX_MCR_GSXE = 278,
    INDEX_MCR_GSWE = 279,
    INDEX_MCR_GSRE = 280,
    INDEX_MCR_GUXE = 281,
    INDEX_MCR_GUWE = 282,
    INDEX_MCR_GURE = 283,
    INDEX_MCR_OV = 284,
    INDEX_MCR_SXE = 285,
    INDEX_MCR_SWE = 286,
    INDEX_MCR_SRE = 287,
    INDEX_MCR_UXE = 288,
    INDEX_MCR_UWE = 289,
    INDEX_MCR_URE = 290,
    INDEX_MCI_SPID = 291,
    INDEX_MPIDX_IDX = 292,
    INDEX_MPBK_BK = 293,
    INDEX_MPLA = 294,
    INDEX_MPUA = 295,
    INDEX_MPAT_WMPID7 = 296,
    INDEX_MPAT_WMPID6 = 297,
    INDEX_MPAT_WMPID5 = 298,
    INDEX_MPAT_WMPID4 = 299,
    INDEX_MPAT_WMPID3 = 300,
    INDEX_MPAT_WMPID2 = 301,
    INDEX_MPAT_WMPID1 = 302,
    INDEX_MPAT_WMPID0 = 303,
    INDEX_MPAT_RMPID7 = 304,
    INDEX_MPAT_RMPID6 = 305,
    INDEX_MPAT_RMPID5 = 306,
    INDEX_MPAT_RMPID4 = 307,
    INDEX_MPAT_RMPID3 = 308,
    INDEX_MPAT_RMPID2 = 309,
    INDEX_MPAT_RMPID1 = 310,
    INDEX_MPAT_RMPID0 = 311,
    INDEX_MPAT_WG = 312,
    INDEX_MPAT_RG = 313,
    INDEX_MPAT_E = 314,
    INDEX_MPAT_SX = 315,
    INDEX_MPAT_SW = 316,
    INDEX_MPAT_SR = 317,
    INDEX_MPAT_UX = 318,
    INDEX_MPAT_UW = 319,
    INDEX_MPAT_UR = 320,
    INDEX_MPID0_SPID = 321,
    INDEX_MPID1_SPID = 322,
    INDEX_MPID2_SPID = 323,
    INDEX_MPID3_SPID = 324,
    INDEX_MPID4_SPID = 325,
    INDEX_MPID5_SPID = 326,
    INDEX_MPID6_SPID = 327,
    INDEX_MPID7_SPID = 328,
    INDEX_ICTAGL_LPN = 329,
    INDEX_ICTAGL_LRU = 330,
    INDEX_ICTAGL_L = 331,
    INDEX_ICTAGL_V = 332,
    INDEX_ICTAGH_WD = 333,
    INDEX_ICTAGH_PD = 334,
    INDEX_ICTAGH_WT = 335,
    INDEX_ICTAGH_PT = 336,
    INDEX_ICTAGH_DATAECC = 337,
    INDEX_ICTAGH_TAGECC = 338,
    INDEX_ICDATL = 339,
    INDEX_ICDATH = 340,
    INDEX_ICCTRL_PBS = 341,
    INDEX_ICCTRL_ICHCLR = 342,
    INDEX_ICCTRL_ICHEIV = 343,
    INDEX_ICCTRL_ICHEMK = 344,
    INDEX_ICCTRL_ICHEN = 345,
    INDEX_ICCFG_ICHSIZE = 346,
    INDEX_ICCFG_ICHLINE = 347,
    INDEX_ICCFG_ICHWAY = 348,
    INDEX_ICERR_CISTW = 349,
    INDEX_ICERR_ESAFE = 350,
    INDEX_ICERR_ESMH = 351,
    INDEX_ICERR_ESPBSE = 352,
    INDEX_ICERR_ESTE1 = 353,
    INDEX_ICERR_ESTE2 = 354,
    INDEX_ICERR_ESDC = 355,
    INDEX_ICERR_ESDE = 356,
    INDEX_ICERR_ERMAFE = 357,
    INDEX_ICERR_ERMMH = 358,
    INDEX_ICERR_ERMPBSE = 359,
    INDEX_ICERR_ERMTE1 = 360,
    INDEX_ICERR_ERMTE2 = 361,
    INDEX_ICERR_ERMDC = 362,
    INDEX_ICERR_ERMDE = 363,
    INDEX_ICERR_ICHEWY = 364,
    INDEX_ICERR_ICHEIX = 365,
    INDEX_ICERR_ICHERQ = 366,
    INDEX_ICERR_ICHED = 367,
    INDEX_ICERR_ICHET = 368,
    INDEX_ICERR_ICHERR = 369,
    INDEX_TSCOUNTL = 370,
    INDEX_TSCOUNTH = 371,
    INDEX_TSCTRL = 372,
    INDEX_PMUMCTRL = 373,
    INDEX_PMGMCTRL = 374,
    INDEX_PMCOUNT0 = 375,
    INDEX_PMCTRL0 = 376,
    INDEX_PMCOUNT1 = 377,
    INDEX_PMCTRL1 = 378,
    INDEX_PMCOUNT2 = 379,
    INDEX_PMCTRL2 = 380,
    INDEX_PMCOUNT3 = 381,
    INDEX_PMCTRL3 = 382,
    INDEX_PMCTRL4 = 383,
    INDEX_PMCTRL5 = 384,
    INDEX_PMCTRL6 = 385,
    INDEX_PMCTRL7 = 386,
    INDEX_PMCOUNT4 = 387,
    INDEX_PMCOUNT5 = 388,
    INDEX_PMCOUNT6 = 389,
    INDEX_PMCOUNT7 = 390,
    INDEX_PMSUBCND0 = 391,
    INDEX_PMSUBCND1 = 392,
    INDEX_PMSUBCND2 = 393,
    INDEX_PMSUBCND3 = 394,
    INDEX_PMSUBCND4 = 395,
    INDEX_PMSUBCND5 = 396,
    INDEX_PMSUBCND6 = 397,
    INDEX_PMSUBCND7 = 398,
    INDEX_DBGEN = 399,
    INDEX_DBPSWH = 400,
    INDEX_DBIC = 401,
    INDEX_DBPC = 402,
    INDEX_DBPSW = 403,
    INDEX_DIR0 = 404,
    INDEX_DIR1 = 405,
    INDEX_BPC = 406,
    INDEX_BPAV = 407,
    INDEX_BPAM = 408,
    INDEX_DBCFG = 409,
    INDEX_DBWR = 410,
    INDEX_LSTEST0 = 411,
    INDEX_LSTEST1 = 412,
    INDEX_LSCFG = 413,
    INDEX_ICBKEY = 414,
    INDEX_IFCR = 415,
    INDEX_IFCR1 = 416,
    INDEX_BRPCTRL0 = 417,
    INDEX_BRPCTRL1 = 418,
    INDEX_BRPCFG = 419,
    INDEX_BRPACTRL = 420,
    INDEX_BRPDATA = 421,
    INDEX_DCBKEY = 422,
    INDEX_LSUCR = 423,
    INDEX_LSULNK0 = 424,
    INDEX_LSULNK1 = 425,
    INDEX_L1RLCR = 426,
    INDEX_L1RLNK0 = 427,
    INDEX_L1RLNK1 = 428,
    INDEX_L1RCFG = 429,
    INDEX_DECFG = 430,
    INDEX_DECTRL = 431,
    INDEX_DEVDS = 432,
    INDEX_RDBCR = 433,
    INDEX_RDBACR = 434,
    INDEX_RDBATAG = 435,
    INDEX_RDBADAT0 = 436,
    INDEX_RDBADAT1 = 437,
    INDEX_RDBADAT2 = 438,
    INDEX_RDBADAT3 = 439,
    INDEX_RDBSTAT = 440,
    INDEX_HV_FUNC_SUPPORT = 441,
    INDEX_SWD0PCAD_EXEC_COUNT = 442,
    INDEX_SWD0PCAD_VAL = 443,
    INDEX_MEM_ACC_TYPE = 444,
    NUM_INDICES = 445
  } struct_index_t;
  
  /**
   * \brief Internal rh850g4 CPU state storage
   */
  class cpu_state_t
  {
  public:
    /**
     * \brief Instruction count
     */
    unsigned int m_instruction_count;
    /**
     * \brief Core local time
     */
    unsigned long long m_local_time;
    /**
     * \brief That part of local time which has been accrued to global time in the current quantum.
     */
    unsigned long long m_accrued_time;
    /**
     * \brief Cycle period
     */
    unsigned long long m_cycle_period;
    /**
     * \brief Pointer to the core instance
     */
    void* m_core_ptr;
    /**
     * \brief Check watchpoint hit flag
     */
    unsigned char m_check_watchpoint_hit;
    /**
     * \brief Watchpoint hit flag
     */
    unsigned char m_watchpoint_hit;
    /**
     * \brief Last access address
     */
    unsigned int m_last_access_address;
    /**
     * \brief Last access size
     */
    unsigned char m_last_access_size;
    /**
     * \brief Last access data
     */
    unsigned long long m_last_access_data;
    /**
     *  \brief Translation time PC
     */
    unsigned int m_trans_PC;
    /**
     *  \brief r0
     */
    unsigned int m_r0;
    /**
     *  \brief r1
     */
    unsigned int m_r1;
    /**
     *  \brief r2
     */
    unsigned int m_r2;
    /**
     *  \brief r3
     */
    unsigned int m_r3;
    /**
     *  \brief r4
     */
    unsigned int m_r4;
    /**
     *  \brief r5
     */
    unsigned int m_r5;
    /**
     *  \brief r6
     */
    unsigned int m_r6;
    /**
     *  \brief r7
     */
    unsigned int m_r7;
    /**
     *  \brief r8
     */
    unsigned int m_r8;
    /**
     *  \brief r9
     */
    unsigned int m_r9;
    /**
     *  \brief r10
     */
    unsigned int m_r10;
    /**
     *  \brief r11
     */
    unsigned int m_r11;
    /**
     *  \brief r12
     */
    unsigned int m_r12;
    /**
     *  \brief r13
     */
    unsigned int m_r13;
    /**
     *  \brief r14
     */
    unsigned int m_r14;
    /**
     *  \brief r15
     */
    unsigned int m_r15;
    /**
     *  \brief r16
     */
    unsigned int m_r16;
    /**
     *  \brief r17
     */
    unsigned int m_r17;
    /**
     *  \brief r18
     */
    unsigned int m_r18;
    /**
     *  \brief r19
     */
    unsigned int m_r19;
    /**
     *  \brief r20
     */
    unsigned int m_r20;
    /**
     *  \brief r21
     */
    unsigned int m_r21;
    /**
     *  \brief r22
     */
    unsigned int m_r22;
    /**
     *  \brief r23
     */
    unsigned int m_r23;
    /**
     *  \brief r24
     */
    unsigned int m_r24;
    /**
     *  \brief r25
     */
    unsigned int m_r25;
    /**
     *  \brief r26
     */
    unsigned int m_r26;
    /**
     *  \brief r27
     */
    unsigned int m_r27;
    /**
     *  \brief r28
     */
    unsigned int m_r28;
    /**
     *  \brief r29
     */
    unsigned int m_r29;
    /**
     *  \brief r30
     */
    unsigned int m_r30;
    /**
     *  \brief r31
     */
    unsigned int m_r31;
    /**
     *  \brief PC
     */
    unsigned int m_PC;
    /**
     *  \brief wr0
     */
    unsigned long long m_wr0;
    /**
     *  \brief wr0_u
     */
    unsigned long long m_wr0_u;
    /**
     *  \brief wr1
     */
    unsigned long long m_wr1;
    /**
     *  \brief wr1_u
     */
    unsigned long long m_wr1_u;
    /**
     *  \brief wr2
     */
    unsigned long long m_wr2;
    /**
     *  \brief wr2_u
     */
    unsigned long long m_wr2_u;
    /**
     *  \brief wr3
     */
    unsigned long long m_wr3;
    /**
     *  \brief wr3_u
     */
    unsigned long long m_wr3_u;
    /**
     *  \brief wr4
     */
    unsigned long long m_wr4;
    /**
     *  \brief wr4_u
     */
    unsigned long long m_wr4_u;
    /**
     *  \brief wr5
     */
    unsigned long long m_wr5;
    /**
     *  \brief wr5_u
     */
    unsigned long long m_wr5_u;
    /**
     *  \brief wr6
     */
    unsigned long long m_wr6;
    /**
     *  \brief wr6_u
     */
    unsigned long long m_wr6_u;
    /**
     *  \brief wr7
     */
    unsigned long long m_wr7;
    /**
     *  \brief wr7_u
     */
    unsigned long long m_wr7_u;
    /**
     *  \brief wr8
     */
    unsigned long long m_wr8;
    /**
     *  \brief wr8_u
     */
    unsigned long long m_wr8_u;
    /**
     *  \brief wr9
     */
    unsigned long long m_wr9;
    /**
     *  \brief wr9_u
     */
    unsigned long long m_wr9_u;
    /**
     *  \brief wr10
     */
    unsigned long long m_wr10;
    /**
     *  \brief wr10_u
     */
    unsigned long long m_wr10_u;
    /**
     *  \brief wr11
     */
    unsigned long long m_wr11;
    /**
     *  \brief wr11_u
     */
    unsigned long long m_wr11_u;
    /**
     *  \brief wr12
     */
    unsigned long long m_wr12;
    /**
     *  \brief wr12_u
     */
    unsigned long long m_wr12_u;
    /**
     *  \brief wr13
     */
    unsigned long long m_wr13;
    /**
     *  \brief wr13_u
     */
    unsigned long long m_wr13_u;
    /**
     *  \brief wr14
     */
    unsigned long long m_wr14;
    /**
     *  \brief wr14_u
     */
    unsigned long long m_wr14_u;
    /**
     *  \brief wr15
     */
    unsigned long long m_wr15;
    /**
     *  \brief wr15_u
     */
    unsigned long long m_wr15_u;
    /**
     *  \brief wr16
     */
    unsigned long long m_wr16;
    /**
     *  \brief wr16_u
     */
    unsigned long long m_wr16_u;
    /**
     *  \brief wr17
     */
    unsigned long long m_wr17;
    /**
     *  \brief wr17_u
     */
    unsigned long long m_wr17_u;
    /**
     *  \brief wr18
     */
    unsigned long long m_wr18;
    /**
     *  \brief wr18_u
     */
    unsigned long long m_wr18_u;
    /**
     *  \brief wr19
     */
    unsigned long long m_wr19;
    /**
     *  \brief wr19_u
     */
    unsigned long long m_wr19_u;
    /**
     *  \brief wr20
     */
    unsigned long long m_wr20;
    /**
     *  \brief wr20_u
     */
    unsigned long long m_wr20_u;
    /**
     *  \brief wr21
     */
    unsigned long long m_wr21;
    /**
     *  \brief wr21_u
     */
    unsigned long long m_wr21_u;
    /**
     *  \brief wr22
     */
    unsigned long long m_wr22;
    /**
     *  \brief wr22_u
     */
    unsigned long long m_wr22_u;
    /**
     *  \brief wr23
     */
    unsigned long long m_wr23;
    /**
     *  \brief wr23_u
     */
    unsigned long long m_wr23_u;
    /**
     *  \brief wr24
     */
    unsigned long long m_wr24;
    /**
     *  \brief wr24_u
     */
    unsigned long long m_wr24_u;
    /**
     *  \brief wr25
     */
    unsigned long long m_wr25;
    /**
     *  \brief wr25_u
     */
    unsigned long long m_wr25_u;
    /**
     *  \brief wr26
     */
    unsigned long long m_wr26;
    /**
     *  \brief wr26_u
     */
    unsigned long long m_wr26_u;
    /**
     *  \brief wr27
     */
    unsigned long long m_wr27;
    /**
     *  \brief wr27_u
     */
    unsigned long long m_wr27_u;
    /**
     *  \brief wr28
     */
    unsigned long long m_wr28;
    /**
     *  \brief wr28_u
     */
    unsigned long long m_wr28_u;
    /**
     *  \brief wr29
     */
    unsigned long long m_wr29;
    /**
     *  \brief wr29_u
     */
    unsigned long long m_wr29_u;
    /**
     *  \brief wr30
     */
    unsigned long long m_wr30;
    /**
     *  \brief wr30_u
     */
    unsigned long long m_wr30_u;
    /**
     *  \brief wr31
     */
    unsigned long long m_wr31;
    /**
     *  \brief wr31_u
     */
    unsigned long long m_wr31_u;
    /**
     *  \brief EIPC
     */
    unsigned int m_EIPC;
    /**
     *  \brief EIPSW
     */
    unsigned int m_EIPSW;
    /**
     *  \brief FEPC
     */
    unsigned int m_FEPC;
    /**
     *  \brief FEPSW
     */
    unsigned int m_FEPSW;
    /**
     *  \brief PSW_Z
     */
    unsigned char m_PSW_Z;
    /**
     *  \brief PSW_S
     */
    unsigned char m_PSW_S;
    /**
     *  \brief PSW_OV
     */
    unsigned char m_PSW_OV;
    /**
     *  \brief PSW_CY
     */
    unsigned char m_PSW_CY;
    /**
     *  \brief PSW_SAT
     */
    unsigned char m_PSW_SAT;
    /**
     *  \brief PSW_ID
     */
    unsigned char m_PSW_ID;
    /**
     *  \brief PSW_EP
     */
    unsigned char m_PSW_EP;
    /**
     *  \brief PSW_NP
     */
    unsigned char m_PSW_NP;
    /**
     *  \brief PSW_EBV
     */
    unsigned char m_PSW_EBV;
    /**
     *  \brief PSW_CU0
     */
    unsigned char m_PSW_CU0;
    /**
     *  \brief PSW_CU1
     */
    unsigned char m_PSW_CU1;
    /**
     *  \brief PSW_CU2
     */
    unsigned char m_PSW_CU2;
    /**
     *  \brief PSW_EIMASK
     */
    unsigned char m_PSW_EIMASK;
    /**
     *  \brief PSW_UM
     */
    unsigned char m_PSW_UM;
    /**
     *  \brief FPSR_CC
     */
    unsigned char m_FPSR_CC;
    /**
     *  \brief FPSR_FN
     */
    unsigned char m_FPSR_FN;
    /**
     *  \brief FPSR_IF
     */
    unsigned char m_FPSR_IF;
    /**
     *  \brief FPSR_PEM
     */
    unsigned char m_FPSR_PEM;
    /**
     *  \brief FPSR_RM
     */
    unsigned char m_FPSR_RM;
    /**
     *  \brief FPSR_FS
     */
    unsigned char m_FPSR_FS;
    /**
     *  \brief FPSR_XC
     */
    unsigned char m_FPSR_XC;
    /**
     *  \brief FPSR_XE
     */
    unsigned char m_FPSR_XE;
    /**
     *  \brief FPSR_XP
     */
    unsigned char m_FPSR_XP;
    /**
     *  \brief FPEPC
     */
    unsigned int m_FPEPC;
    /**
     *  \brief FPST_XC
     */
    unsigned char m_FPST_XC;
    /**
     *  \brief FPST_IF
     */
    unsigned char m_FPST_IF;
    /**
     *  \brief FPST_XP
     */
    unsigned char m_FPST_XP;
    /**
     *  \brief FPCC_CC
     */
    unsigned char m_FPCC_CC;
    /**
     *  \brief FPCFG_RM
     */
    unsigned char m_FPCFG_RM;
    /**
     *  \brief FPCFG_XE
     */
    unsigned char m_FPCFG_XE;
    /**
     *  \brief EIIC
     */
    unsigned int m_EIIC;
    /**
     *  \brief FEIC
     */
    unsigned int m_FEIC;
    /**
     *  \brief PSWH_GPID
     */
    unsigned char m_PSWH_GPID;
    /**
     *  \brief PSWH_GM
     */
    unsigned char m_PSWH_GM;
    /**
     *  \brief CTPC
     */
    unsigned int m_CTPC;
    /**
     *  \brief CTPSW
     */
    unsigned int m_CTPSW;
    /**
     *  \brief EIPSWH_GPID
     */
    unsigned char m_EIPSWH_GPID;
    /**
     *  \brief EIPSWH_GM
     */
    unsigned char m_EIPSWH_GM;
    /**
     *  \brief FEPSWH_GPID
     */
    unsigned char m_FEPSWH_GPID;
    /**
     *  \brief FEPSWH_GM
     */
    unsigned char m_FEPSWH_GM;
    /**
     *  \brief CTBP
     */
    unsigned int m_CTBP;
    /**
     *  \brief SNZCFG
     */
    unsigned int m_SNZCFG;
    /**
     *  \brief EIWR
     */
    unsigned int m_EIWR;
    /**
     *  \brief FEWR
     */
    unsigned int m_FEWR;
    /**
     *  \brief SPID
     */
    unsigned int m_SPID;
    /**
     *  \brief SPIDLIST
     */
    unsigned int m_SPIDLIST;
    /**
     *  \brief RBASE_RBASE
     */
    unsigned int m_RBASE_RBASE;
    /**
     *  \brief RBASE_DV
     */
    unsigned char m_RBASE_DV;
    /**
     *  \brief RBASE_RINT
     */
    unsigned char m_RBASE_RINT;
    /**
     *  \brief EBASE_EBASE
     */
    unsigned int m_EBASE_EBASE;
    /**
     *  \brief EBASE_DV
     */
    unsigned char m_EBASE_DV;
    /**
     *  \brief EBASE_RINT
     */
    unsigned char m_EBASE_RINT;
    /**
     *  \brief INTBP
     */
    unsigned int m_INTBP;
    /**
     *  \brief MCTL_UIC
     */
    unsigned char m_MCTL_UIC;
    /**
     *  \brief PID
     */
    unsigned int m_PID;
    /**
     *  \brief SVLOCK
     */
    unsigned int m_SVLOCK;
    /**
     *  \brief SCCFG_SIZE
     */
    unsigned char m_SCCFG_SIZE;
    /**
     *  \brief SCBP
     */
    unsigned int m_SCBP;
    /**
     *  \brief HVCFG_HVE
     */
    unsigned char m_HVCFG_HVE;
    /**
     *  \brief GMCFG_GMP
     */
    unsigned char m_GMCFG_GMP;
    /**
     *  \brief GMCFG_HMP
     */
    unsigned char m_GMCFG_HMP;
    /**
     *  \brief GMCFG_GSYSE
     */
    unsigned char m_GMCFG_GSYSE;
    /**
     *  \brief GMCFG_GCU0
     */
    unsigned char m_GMCFG_GCU0;
    /**
     *  \brief GMCFG_GCU1
     */
    unsigned char m_GMCFG_GCU1;
    /**
     *  \brief GMCFG_GCU2
     */
    unsigned char m_GMCFG_GCU2;
    /**
     *  \brief HVSB
     */
    unsigned int m_HVSB;
    /**
     *  \brief PEID
     */
    unsigned int m_PEID;
    /**
     *  \brief BMID_BMID
     */
    unsigned char m_BMID_BMID;
    /**
     *  \brief MEA
     */
    unsigned int m_MEA;
    /**
     *  \brief MEI
     */
    unsigned int m_MEI;
    /**
     *  \brief RBCR0_MD
     */
    unsigned char m_RBCR0_MD;
    /**
     *  \brief RBCR0_BE
     */
    unsigned short m_RBCR0_BE;
    /**
     *  \brief RBCR1_NC
     */
    unsigned short m_RBCR1_NC;
    /**
     *  \brief RBNR_BN
     */
    unsigned char m_RBNR_BN;
    /**
     *  \brief RBIP_RBIP
     */
    unsigned int m_RBIP_RBIP;
    /**
     *  \brief ISPR_ISP
     */
    unsigned short m_ISPR_ISP;
    /**
     *  \brief IMSR_EEIM
     */
    unsigned char m_IMSR_EEIM;
    /**
     *  \brief IMSR_EPLM
     */
    unsigned char m_IMSR_EPLM;
    /**
     *  \brief IMSR_EID
     */
    unsigned char m_IMSR_EID;
    /**
     *  \brief IMSR_ENP
     */
    unsigned char m_IMSR_ENP;
    /**
     *  \brief IMSR_FNP
     */
    unsigned char m_IMSR_FNP;
    /**
     *  \brief IMSR_HEEIM
     */
    unsigned char m_IMSR_HEEIM;
    /**
     *  \brief IMSR_HEPLM
     */
    unsigned char m_IMSR_HEPLM;
    /**
     *  \brief IMSR_HEID
     */
    unsigned char m_IMSR_HEID;
    /**
     *  \brief IMSR_HENP
     */
    unsigned char m_IMSR_HENP;
    /**
     *  \brief IMSR_HFNP
     */
    unsigned char m_IMSR_HFNP;
    /**
     *  \brief ICSR_PMEI
     */
    unsigned char m_ICSR_PMEI;
    /**
     *  \brief INTCFG_ISPC
     */
    unsigned char m_INTCFG_ISPC;
    /**
     *  \brief INTCFG_EPL
     */
    unsigned char m_INTCFG_EPL;
    /**
     *  \brief INTCFG_ULNR
     */
    unsigned char m_INTCFG_ULNR;
    /**
     *  \brief PLMR_PLM
     */
    unsigned char m_PLMR_PLM;
    /**
     *  \brief FXSR_FN
     */
    unsigned char m_FXSR_FN;
    /**
     *  \brief FXSR_IF
     */
    unsigned char m_FXSR_IF;
    /**
     *  \brief FXSR_PEM
     */
    unsigned char m_FXSR_PEM;
    /**
     *  \brief FXSR_RM
     */
    unsigned char m_FXSR_RM;
    /**
     *  \brief FXSR_FS
     */
    unsigned char m_FXSR_FS;
    /**
     *  \brief FXSR_XC
     */
    unsigned char m_FXSR_XC;
    /**
     *  \brief FXSR_XE
     */
    unsigned char m_FXSR_XE;
    /**
     *  \brief FXSR_XP
     */
    unsigned char m_FXSR_XP;
    /**
     *  \brief FXST_XC
     */
    unsigned char m_FXST_XC;
    /**
     *  \brief FXST_IF
     */
    unsigned char m_FXST_IF;
    /**
     *  \brief FXST_XP
     */
    unsigned char m_FXST_XP;
    /**
     *  \brief FXINFO_NFPU
     */
    unsigned char m_FXINFO_NFPU;
    /**
     *  \brief FXINFO_RSIZE
     */
    unsigned char m_FXINFO_RSIZE;
    /**
     *  \brief FXCFG_RM
     */
    unsigned char m_FXCFG_RM;
    /**
     *  \brief FXCFG_XE
     */
    unsigned char m_FXCFG_XE;
    /**
     *  \brief FXXC_XC3
     */
    unsigned char m_FXXC_XC3;
    /**
     *  \brief FXXC_XC2
     */
    unsigned char m_FXXC_XC2;
    /**
     *  \brief FXXC_XC1
     */
    unsigned char m_FXXC_XC1;
    /**
     *  \brief FXXC_XC0
     */
    unsigned char m_FXXC_XC0;
    /**
     *  \brief FXXP_XP3
     */
    unsigned char m_FXXP_XP3;
    /**
     *  \brief FXXP_XP2
     */
    unsigned char m_FXXP_XP2;
    /**
     *  \brief FXXP_XP1
     */
    unsigned char m_FXXP_XP1;
    /**
     *  \brief FXXP_XP0
     */
    unsigned char m_FXXP_XP0;
    /**
     *  \brief GMEIPC
     */
    unsigned int m_GMEIPC;
    /**
     *  \brief GMEIPSW
     */
    unsigned int m_GMEIPSW;
    /**
     *  \brief GMFEPC
     */
    unsigned int m_GMFEPC;
    /**
     *  \brief GMFEPSW
     */
    unsigned int m_GMFEPSW;
    /**
     *  \brief GMPSW_Z
     */
    unsigned char m_GMPSW_Z;
    /**
     *  \brief GMPSW_S
     */
    unsigned char m_GMPSW_S;
    /**
     *  \brief GMPSW_OV
     */
    unsigned char m_GMPSW_OV;
    /**
     *  \brief GMPSW_CY
     */
    unsigned char m_GMPSW_CY;
    /**
     *  \brief GMPSW_SAT
     */
    unsigned char m_GMPSW_SAT;
    /**
     *  \brief GMPSW_ID
     */
    unsigned char m_GMPSW_ID;
    /**
     *  \brief GMPSW_EP
     */
    unsigned char m_GMPSW_EP;
    /**
     *  \brief GMPSW_NP
     */
    unsigned char m_GMPSW_NP;
    /**
     *  \brief GMPSW_EBV
     */
    unsigned char m_GMPSW_EBV;
    /**
     *  \brief GMPSW_CU0
     */
    unsigned char m_GMPSW_CU0;
    /**
     *  \brief GMPSW_CU1
     */
    unsigned char m_GMPSW_CU1;
    /**
     *  \brief GMPSW_CU2
     */
    unsigned char m_GMPSW_CU2;
    /**
     *  \brief GMPSW_EIMASK
     */
    unsigned char m_GMPSW_EIMASK;
    /**
     *  \brief GMPSW_UM
     */
    unsigned char m_GMPSW_UM;
    /**
     *  \brief GMMEA
     */
    unsigned int m_GMMEA;
    /**
     *  \brief GMMEI
     */
    unsigned int m_GMMEI;
    /**
     *  \brief GMEIIC
     */
    unsigned int m_GMEIIC;
    /**
     *  \brief GMFEIC
     */
    unsigned int m_GMFEIC;
    /**
     *  \brief GMSPID
     */
    unsigned int m_GMSPID;
    /**
     *  \brief GMSPIDLIST
     */
    unsigned int m_GMSPIDLIST;
    /**
     *  \brief GMEBASE_EBASE
     */
    unsigned int m_GMEBASE_EBASE;
    /**
     *  \brief GMEBASE_DV
     */
    unsigned char m_GMEBASE_DV;
    /**
     *  \brief GMEBASE_RINT
     */
    unsigned char m_GMEBASE_RINT;
    /**
     *  \brief GMINTBP
     */
    unsigned int m_GMINTBP;
    /**
     *  \brief GMINTCFG_ISPC
     */
    unsigned char m_GMINTCFG_ISPC;
    /**
     *  \brief GMINTCFG_EPL
     */
    unsigned char m_GMINTCFG_EPL;
    /**
     *  \brief GMINTCFG_ULNR
     */
    unsigned char m_GMINTCFG_ULNR;
    /**
     *  \brief GMPLMR_PLM
     */
    unsigned char m_GMPLMR_PLM;
    /**
     *  \brief GMSVLOCK
     */
    unsigned int m_GMSVLOCK;
    /**
     *  \brief GMMPM
     */
    unsigned int m_GMMPM;
    /**
     *  \brief GMEIWR
     */
    unsigned int m_GMEIWR;
    /**
     *  \brief GMFEWR
     */
    unsigned int m_GMFEWR;
    /**
     *  \brief GMPEID
     */
    unsigned int m_GMPEID;
    /**
     *  \brief MPM
     */
    unsigned int m_MPM;
    /**
     *  \brief MPCFG_NMPUE
     */
    unsigned char m_MPCFG_NMPUE;
    /**
     *  \brief MPCFG_HBE
     */
    unsigned char m_MPCFG_HBE;
    /**
     *  \brief MPCFG_ARCH
     */
    unsigned char m_MPCFG_ARCH;
    /**
     *  \brief MPCFG_NBK
     */
    unsigned char m_MPCFG_NBK;
    /**
     *  \brief MPCFG_DMDP
     */
    unsigned char m_MPCFG_DMDP;
    /**
     *  \brief MCA
     */
    unsigned int m_MCA;
    /**
     *  \brief MCS
     */
    unsigned int m_MCS;
    /**
     *  \brief MCC
     */
    unsigned int m_MCC;
    /**
     *  \brief MCR_HSXE
     */
    unsigned char m_MCR_HSXE;
    /**
     *  \brief MCR_HSWE
     */
    unsigned char m_MCR_HSWE;
    /**
     *  \brief MCR_HSRE
     */
    unsigned char m_MCR_HSRE;
    /**
     *  \brief MCR_HUXE
     */
    unsigned char m_MCR_HUXE;
    /**
     *  \brief MCR_HUWE
     */
    unsigned char m_MCR_HUWE;
    /**
     *  \brief MCR_HURE
     */
    unsigned char m_MCR_HURE;
    /**
     *  \brief MCR_GSXE
     */
    unsigned char m_MCR_GSXE;
    /**
     *  \brief MCR_GSWE
     */
    unsigned char m_MCR_GSWE;
    /**
     *  \brief MCR_GSRE
     */
    unsigned char m_MCR_GSRE;
    /**
     *  \brief MCR_GUXE
     */
    unsigned char m_MCR_GUXE;
    /**
     *  \brief MCR_GUWE
     */
    unsigned char m_MCR_GUWE;
    /**
     *  \brief MCR_GURE
     */
    unsigned char m_MCR_GURE;
    /**
     *  \brief MCR_OV
     */
    unsigned char m_MCR_OV;
    /**
     *  \brief MCR_SXE
     */
    unsigned char m_MCR_SXE;
    /**
     *  \brief MCR_SWE
     */
    unsigned char m_MCR_SWE;
    /**
     *  \brief MCR_SRE
     */
    unsigned char m_MCR_SRE;
    /**
     *  \brief MCR_UXE
     */
    unsigned char m_MCR_UXE;
    /**
     *  \brief MCR_UWE
     */
    unsigned char m_MCR_UWE;
    /**
     *  \brief MCR_URE
     */
    unsigned char m_MCR_URE;
    /**
     *  \brief MCI_SPID
     */
    unsigned char m_MCI_SPID;
    /**
     *  \brief MPIDX_IDX
     */
    unsigned char m_MPIDX_IDX;
    /**
     *  \brief MPBK_BK
     */
    unsigned char m_MPBK_BK;
    /**
     *  \brief MPLA
     */
    unsigned int m_MPLA;
    /**
     *  \brief MPUA
     */
    unsigned int m_MPUA;
    /**
     *  \brief MPAT_WMPID7
     */
    unsigned char m_MPAT_WMPID7;
    /**
     *  \brief MPAT_WMPID6
     */
    unsigned char m_MPAT_WMPID6;
    /**
     *  \brief MPAT_WMPID5
     */
    unsigned char m_MPAT_WMPID5;
    /**
     *  \brief MPAT_WMPID4
     */
    unsigned char m_MPAT_WMPID4;
    /**
     *  \brief MPAT_WMPID3
     */
    unsigned char m_MPAT_WMPID3;
    /**
     *  \brief MPAT_WMPID2
     */
    unsigned char m_MPAT_WMPID2;
    /**
     *  \brief MPAT_WMPID1
     */
    unsigned char m_MPAT_WMPID1;
    /**
     *  \brief MPAT_WMPID0
     */
    unsigned char m_MPAT_WMPID0;
    /**
     *  \brief MPAT_RMPID7
     */
    unsigned char m_MPAT_RMPID7;
    /**
     *  \brief MPAT_RMPID6
     */
    unsigned char m_MPAT_RMPID6;
    /**
     *  \brief MPAT_RMPID5
     */
    unsigned char m_MPAT_RMPID5;
    /**
     *  \brief MPAT_RMPID4
     */
    unsigned char m_MPAT_RMPID4;
    /**
     *  \brief MPAT_RMPID3
     */
    unsigned char m_MPAT_RMPID3;
    /**
     *  \brief MPAT_RMPID2
     */
    unsigned char m_MPAT_RMPID2;
    /**
     *  \brief MPAT_RMPID1
     */
    unsigned char m_MPAT_RMPID1;
    /**
     *  \brief MPAT_RMPID0
     */
    unsigned char m_MPAT_RMPID0;
    /**
     *  \brief MPAT_WG
     */
    unsigned char m_MPAT_WG;
    /**
     *  \brief MPAT_RG
     */
    unsigned char m_MPAT_RG;
    /**
     *  \brief MPAT_E
     */
    unsigned char m_MPAT_E;
    /**
     *  \brief MPAT_SX
     */
    unsigned char m_MPAT_SX;
    /**
     *  \brief MPAT_SW
     */
    unsigned char m_MPAT_SW;
    /**
     *  \brief MPAT_SR
     */
    unsigned char m_MPAT_SR;
    /**
     *  \brief MPAT_UX
     */
    unsigned char m_MPAT_UX;
    /**
     *  \brief MPAT_UW
     */
    unsigned char m_MPAT_UW;
    /**
     *  \brief MPAT_UR
     */
    unsigned char m_MPAT_UR;
    /**
     *  \brief MPID0_SPID
     */
    unsigned char m_MPID0_SPID;
    /**
     *  \brief MPID1_SPID
     */
    unsigned char m_MPID1_SPID;
    /**
     *  \brief MPID2_SPID
     */
    unsigned char m_MPID2_SPID;
    /**
     *  \brief MPID3_SPID
     */
    unsigned char m_MPID3_SPID;
    /**
     *  \brief MPID4_SPID
     */
    unsigned char m_MPID4_SPID;
    /**
     *  \brief MPID5_SPID
     */
    unsigned char m_MPID5_SPID;
    /**
     *  \brief MPID6_SPID
     */
    unsigned char m_MPID6_SPID;
    /**
     *  \brief MPID7_SPID
     */
    unsigned char m_MPID7_SPID;
    /**
     *  \brief ICTAGL_LPN
     */
    unsigned int m_ICTAGL_LPN;
    /**
     *  \brief ICTAGL_LRU
     */
    unsigned char m_ICTAGL_LRU;
    /**
     *  \brief ICTAGL_L
     */
    unsigned char m_ICTAGL_L;
    /**
     *  \brief ICTAGL_V
     */
    unsigned char m_ICTAGL_V;
    /**
     *  \brief ICTAGH_WD
     */
    unsigned char m_ICTAGH_WD;
    /**
     *  \brief ICTAGH_PD
     */
    unsigned char m_ICTAGH_PD;
    /**
     *  \brief ICTAGH_WT
     */
    unsigned char m_ICTAGH_WT;
    /**
     *  \brief ICTAGH_PT
     */
    unsigned char m_ICTAGH_PT;
    /**
     *  \brief ICTAGH_DATAECC
     */
    unsigned char m_ICTAGH_DATAECC;
    /**
     *  \brief ICTAGH_TAGECC
     */
    unsigned char m_ICTAGH_TAGECC;
    /**
     *  \brief ICDATL
     */
    unsigned int m_ICDATL;
    /**
     *  \brief ICDATH
     */
    unsigned int m_ICDATH;
    /**
     *  \brief ICCTRL_PBS
     */
    unsigned char m_ICCTRL_PBS;
    /**
     *  \brief ICCTRL_ICHCLR
     */
    unsigned char m_ICCTRL_ICHCLR;
    /**
     *  \brief ICCTRL_ICHEIV
     */
    unsigned char m_ICCTRL_ICHEIV;
    /**
     *  \brief ICCTRL_ICHEMK
     */
    unsigned char m_ICCTRL_ICHEMK;
    /**
     *  \brief ICCTRL_ICHEN
     */
    unsigned char m_ICCTRL_ICHEN;
    /**
     *  \brief ICCFG_ICHSIZE
     */
    unsigned char m_ICCFG_ICHSIZE;
    /**
     *  \brief ICCFG_ICHLINE
     */
    unsigned char m_ICCFG_ICHLINE;
    /**
     *  \brief ICCFG_ICHWAY
     */
    unsigned char m_ICCFG_ICHWAY;
    /**
     *  \brief ICERR_CISTW
     */
    unsigned char m_ICERR_CISTW;
    /**
     *  \brief ICERR_ESAFE
     */
    unsigned char m_ICERR_ESAFE;
    /**
     *  \brief ICERR_ESMH
     */
    unsigned char m_ICERR_ESMH;
    /**
     *  \brief ICERR_ESPBSE
     */
    unsigned char m_ICERR_ESPBSE;
    /**
     *  \brief ICERR_ESTE1
     */
    unsigned char m_ICERR_ESTE1;
    /**
     *  \brief ICERR_ESTE2
     */
    unsigned char m_ICERR_ESTE2;
    /**
     *  \brief ICERR_ESDC
     */
    unsigned char m_ICERR_ESDC;
    /**
     *  \brief ICERR_ESDE
     */
    unsigned char m_ICERR_ESDE;
    /**
     *  \brief ICERR_ERMAFE
     */
    unsigned char m_ICERR_ERMAFE;
    /**
     *  \brief ICERR_ERMMH
     */
    unsigned char m_ICERR_ERMMH;
    /**
     *  \brief ICERR_ERMPBSE
     */
    unsigned char m_ICERR_ERMPBSE;
    /**
     *  \brief ICERR_ERMTE1
     */
    unsigned char m_ICERR_ERMTE1;
    /**
     *  \brief ICERR_ERMTE2
     */
    unsigned char m_ICERR_ERMTE2;
    /**
     *  \brief ICERR_ERMDC
     */
    unsigned char m_ICERR_ERMDC;
    /**
     *  \brief ICERR_ERMDE
     */
    unsigned char m_ICERR_ERMDE;
    /**
     *  \brief ICERR_ICHEWY
     */
    unsigned char m_ICERR_ICHEWY;
    /**
     *  \brief ICERR_ICHEIX
     */
    unsigned char m_ICERR_ICHEIX;
    /**
     *  \brief ICERR_ICHERQ
     */
    unsigned char m_ICERR_ICHERQ;
    /**
     *  \brief ICERR_ICHED
     */
    unsigned char m_ICERR_ICHED;
    /**
     *  \brief ICERR_ICHET
     */
    unsigned char m_ICERR_ICHET;
    /**
     *  \brief ICERR_ICHERR
     */
    unsigned char m_ICERR_ICHERR;
    /**
     *  \brief TSCOUNTL
     */
    unsigned int m_TSCOUNTL;
    /**
     *  \brief TSCOUNTH
     */
    unsigned int m_TSCOUNTH;
    /**
     *  \brief TSCTRL
     */
    unsigned int m_TSCTRL;
    /**
     *  \brief PMUMCTRL
     */
    unsigned int m_PMUMCTRL;
    /**
     *  \brief PMGMCTRL
     */
    unsigned int m_PMGMCTRL;
    /**
     *  \brief PMCOUNT0
     */
    unsigned int m_PMCOUNT0;
    /**
     *  \brief PMCTRL0
     */
    unsigned int m_PMCTRL0;
    /**
     *  \brief PMCOUNT1
     */
    unsigned int m_PMCOUNT1;
    /**
     *  \brief PMCTRL1
     */
    unsigned int m_PMCTRL1;
    /**
     *  \brief PMCOUNT2
     */
    unsigned int m_PMCOUNT2;
    /**
     *  \brief PMCTRL2
     */
    unsigned int m_PMCTRL2;
    /**
     *  \brief PMCOUNT3
     */
    unsigned int m_PMCOUNT3;
    /**
     *  \brief PMCTRL3
     */
    unsigned int m_PMCTRL3;
    /**
     *  \brief PMCTRL4
     */
    unsigned int m_PMCTRL4;
    /**
     *  \brief PMCTRL5
     */
    unsigned int m_PMCTRL5;
    /**
     *  \brief PMCTRL6
     */
    unsigned int m_PMCTRL6;
    /**
     *  \brief PMCTRL7
     */
    unsigned int m_PMCTRL7;
    /**
     *  \brief PMCOUNT4
     */
    unsigned int m_PMCOUNT4;
    /**
     *  \brief PMCOUNT5
     */
    unsigned int m_PMCOUNT5;
    /**
     *  \brief PMCOUNT6
     */
    unsigned int m_PMCOUNT6;
    /**
     *  \brief PMCOUNT7
     */
    unsigned int m_PMCOUNT7;
    /**
     *  \brief PMSUBCND0
     */
    unsigned int m_PMSUBCND0;
    /**
     *  \brief PMSUBCND1
     */
    unsigned int m_PMSUBCND1;
    /**
     *  \brief PMSUBCND2
     */
    unsigned int m_PMSUBCND2;
    /**
     *  \brief PMSUBCND3
     */
    unsigned int m_PMSUBCND3;
    /**
     *  \brief PMSUBCND4
     */
    unsigned int m_PMSUBCND4;
    /**
     *  \brief PMSUBCND5
     */
    unsigned int m_PMSUBCND5;
    /**
     *  \brief PMSUBCND6
     */
    unsigned int m_PMSUBCND6;
    /**
     *  \brief PMSUBCND7
     */
    unsigned int m_PMSUBCND7;
    /**
     *  \brief DBGEN
     */
    unsigned int m_DBGEN;
    /**
     *  \brief DBPSWH
     */
    unsigned int m_DBPSWH;
    /**
     *  \brief DBIC
     */
    unsigned int m_DBIC;
    /**
     *  \brief DBPC
     */
    unsigned int m_DBPC;
    /**
     *  \brief DBPSW
     */
    unsigned int m_DBPSW;
    /**
     *  \brief DIR0
     */
    unsigned int m_DIR0;
    /**
     *  \brief DIR1
     */
    unsigned int m_DIR1;
    /**
     *  \brief BPC
     */
    unsigned int m_BPC;
    /**
     *  \brief BPAV
     */
    unsigned int m_BPAV;
    /**
     *  \brief BPAM
     */
    unsigned int m_BPAM;
    /**
     *  \brief DBCFG
     */
    unsigned int m_DBCFG;
    /**
     *  \brief DBWR
     */
    unsigned int m_DBWR;
    /**
     *  \brief LSTEST0
     */
    unsigned int m_LSTEST0;
    /**
     *  \brief LSTEST1
     */
    unsigned int m_LSTEST1;
    /**
     *  \brief LSCFG
     */
    unsigned int m_LSCFG;
    /**
     *  \brief ICBKEY
     */
    unsigned int m_ICBKEY;
    /**
     *  \brief IFCR
     */
    unsigned int m_IFCR;
    /**
     *  \brief IFCR1
     */
    unsigned int m_IFCR1;
    /**
     *  \brief BRPCTRL0
     */
    unsigned int m_BRPCTRL0;
    /**
     *  \brief BRPCTRL1
     */
    unsigned int m_BRPCTRL1;
    /**
     *  \brief BRPCFG
     */
    unsigned int m_BRPCFG;
    /**
     *  \brief BRPACTRL
     */
    unsigned int m_BRPACTRL;
    /**
     *  \brief BRPDATA
     */
    unsigned int m_BRPDATA;
    /**
     *  \brief DCBKEY
     */
    unsigned int m_DCBKEY;
    /**
     *  \brief LSUCR
     */
    unsigned int m_LSUCR;
    /**
     *  \brief LSULNK0
     */
    unsigned int m_LSULNK0;
    /**
     *  \brief LSULNK1
     */
    unsigned int m_LSULNK1;
    /**
     *  \brief L1RLCR
     */
    unsigned int m_L1RLCR;
    /**
     *  \brief L1RLNK0
     */
    unsigned int m_L1RLNK0;
    /**
     *  \brief L1RLNK1
     */
    unsigned int m_L1RLNK1;
    /**
     *  \brief L1RCFG
     */
    unsigned int m_L1RCFG;
    /**
     *  \brief DECFG
     */
    unsigned int m_DECFG;
    /**
     *  \brief DECTRL
     */
    unsigned int m_DECTRL;
    /**
     *  \brief DEVDS
     */
    unsigned int m_DEVDS;
    /**
     *  \brief RDBCR
     */
    unsigned int m_RDBCR;
    /**
     *  \brief RDBACR
     */
    unsigned int m_RDBACR;
    /**
     *  \brief RDBATAG
     */
    unsigned int m_RDBATAG;
    /**
     *  \brief RDBADAT0
     */
    unsigned int m_RDBADAT0;
    /**
     *  \brief RDBADAT1
     */
    unsigned int m_RDBADAT1;
    /**
     *  \brief RDBADAT2
     */
    unsigned int m_RDBADAT2;
    /**
     *  \brief RDBADAT3
     */
    unsigned int m_RDBADAT3;
    /**
     *  \brief RDBSTAT
     */
    unsigned int m_RDBSTAT;
    /**
     *  \brief hv_func_support
     */
    unsigned char m_hv_func_support;
    /**
     *  \brief swd0pcad_exec_count
     */
    unsigned int m_swd0pcad_exec_count;
    /**
     *  \brief swd0pcad_val
     */
    unsigned int m_swd0pcad_val;
    /**
     *  \brief mem_acc_type
     */
    unsigned int m_mem_acc_type;
  };
  
  /**
   * \brief rh850g4 CPU state implementation
   */
  class cpu_state_impl_t : public cpu_state_if_t
  {
  public:
    /**
     * \brief Constructor
     */
    cpu_state_impl_t();
    
    /**
     * \brief Destructor
     */
    virtual ~cpu_state_impl_t();

    /**
     * \brief Initialisation operation
     * \param code_gen_impl Handle to code generator implementation
     * \param count_instructions Boolean flag that indicates if instructions
     * should be counted (if true) or not (if false)
     */
    virtual void initialise(fastiss::code_gen_if_t *code_gen_impl, bool count_instructions);
    
    /**
     * \brief Initialises a block function
     * \param block_fn Block function to be initialised
     */
    virtual void initialise_block_function(fastiss::block_function_t *block_fn);

    /**
     * \brief Get handle to CPU state
     * \return Handle to the internal CPU state
     */
    cpu_state_t *get_cpu_state(void);

    // register name operations
    /**
     * \brief Generates string representing name of the rh850g4 r0 register
     * \return String representing the name of the rh850g4 r0 register
     */
    virtual std::string register_name_r0(void);
    /**
     * \brief Generates string representing name of the rh850g4 r1 register
     * \return String representing the name of the rh850g4 r1 register
     */
    virtual std::string register_name_r1(void);
    /**
     * \brief Generates string representing name of the rh850g4 r2 register
     * \return String representing the name of the rh850g4 r2 register
     */
    virtual std::string register_name_r2(void);
    /**
     * \brief Generates string representing name of the rh850g4 r3 register
     * \return String representing the name of the rh850g4 r3 register
     */
    virtual std::string register_name_r3(void);
    /**
     * \brief Generates string representing name of the rh850g4 r4 register
     * \return String representing the name of the rh850g4 r4 register
     */
    virtual std::string register_name_r4(void);
    /**
     * \brief Generates string representing name of the rh850g4 r5 register
     * \return String representing the name of the rh850g4 r5 register
     */
    virtual std::string register_name_r5(void);
    /**
     * \brief Generates string representing name of the rh850g4 r6 register
     * \return String representing the name of the rh850g4 r6 register
     */
    virtual std::string register_name_r6(void);
    /**
     * \brief Generates string representing name of the rh850g4 r7 register
     * \return String representing the name of the rh850g4 r7 register
     */
    virtual std::string register_name_r7(void);
    /**
     * \brief Generates string representing name of the rh850g4 r8 register
     * \return String representing the name of the rh850g4 r8 register
     */
    virtual std::string register_name_r8(void);
    /**
     * \brief Generates string representing name of the rh850g4 r9 register
     * \return String representing the name of the rh850g4 r9 register
     */
    virtual std::string register_name_r9(void);
    /**
     * \brief Generates string representing name of the rh850g4 r10 register
     * \return String representing the name of the rh850g4 r10 register
     */
    virtual std::string register_name_r10(void);
    /**
     * \brief Generates string representing name of the rh850g4 r11 register
     * \return String representing the name of the rh850g4 r11 register
     */
    virtual std::string register_name_r11(void);
    /**
     * \brief Generates string representing name of the rh850g4 r12 register
     * \return String representing the name of the rh850g4 r12 register
     */
    virtual std::string register_name_r12(void);
    /**
     * \brief Generates string representing name of the rh850g4 r13 register
     * \return String representing the name of the rh850g4 r13 register
     */
    virtual std::string register_name_r13(void);
    /**
     * \brief Generates string representing name of the rh850g4 r14 register
     * \return String representing the name of the rh850g4 r14 register
     */
    virtual std::string register_name_r14(void);
    /**
     * \brief Generates string representing name of the rh850g4 r15 register
     * \return String representing the name of the rh850g4 r15 register
     */
    virtual std::string register_name_r15(void);
    /**
     * \brief Generates string representing name of the rh850g4 r16 register
     * \return String representing the name of the rh850g4 r16 register
     */
    virtual std::string register_name_r16(void);
    /**
     * \brief Generates string representing name of the rh850g4 r17 register
     * \return String representing the name of the rh850g4 r17 register
     */
    virtual std::string register_name_r17(void);
    /**
     * \brief Generates string representing name of the rh850g4 r18 register
     * \return String representing the name of the rh850g4 r18 register
     */
    virtual std::string register_name_r18(void);
    /**
     * \brief Generates string representing name of the rh850g4 r19 register
     * \return String representing the name of the rh850g4 r19 register
     */
    virtual std::string register_name_r19(void);
    /**
     * \brief Generates string representing name of the rh850g4 r20 register
     * \return String representing the name of the rh850g4 r20 register
     */
    virtual std::string register_name_r20(void);
    /**
     * \brief Generates string representing name of the rh850g4 r21 register
     * \return String representing the name of the rh850g4 r21 register
     */
    virtual std::string register_name_r21(void);
    /**
     * \brief Generates string representing name of the rh850g4 r22 register
     * \return String representing the name of the rh850g4 r22 register
     */
    virtual std::string register_name_r22(void);
    /**
     * \brief Generates string representing name of the rh850g4 r23 register
     * \return String representing the name of the rh850g4 r23 register
     */
    virtual std::string register_name_r23(void);
    /**
     * \brief Generates string representing name of the rh850g4 r24 register
     * \return String representing the name of the rh850g4 r24 register
     */
    virtual std::string register_name_r24(void);
    /**
     * \brief Generates string representing name of the rh850g4 r25 register
     * \return String representing the name of the rh850g4 r25 register
     */
    virtual std::string register_name_r25(void);
    /**
     * \brief Generates string representing name of the rh850g4 r26 register
     * \return String representing the name of the rh850g4 r26 register
     */
    virtual std::string register_name_r26(void);
    /**
     * \brief Generates string representing name of the rh850g4 r27 register
     * \return String representing the name of the rh850g4 r27 register
     */
    virtual std::string register_name_r27(void);
    /**
     * \brief Generates string representing name of the rh850g4 r28 register
     * \return String representing the name of the rh850g4 r28 register
     */
    virtual std::string register_name_r28(void);
    /**
     * \brief Generates string representing name of the rh850g4 r29 register
     * \return String representing the name of the rh850g4 r29 register
     */
    virtual std::string register_name_r29(void);
    /**
     * \brief Generates string representing name of the rh850g4 r30 register
     * \return String representing the name of the rh850g4 r30 register
     */
    virtual std::string register_name_r30(void);
    /**
     * \brief Generates string representing name of the rh850g4 r31 register
     * \return String representing the name of the rh850g4 r31 register
     */
    virtual std::string register_name_r31(void);
    /**
     * \brief Generates string representing name of the rh850g4 PC register
     * \return String representing the name of the rh850g4 PC register
     */
    virtual std::string register_name_PC(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr0 register
     * \return String representing the name of the rh850g4 wr0 register
     */
    virtual std::string register_name_wr0(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr0_u register
     * \return String representing the name of the rh850g4 wr0_u register
     */
    virtual std::string register_name_wr0_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr1 register
     * \return String representing the name of the rh850g4 wr1 register
     */
    virtual std::string register_name_wr1(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr1_u register
     * \return String representing the name of the rh850g4 wr1_u register
     */
    virtual std::string register_name_wr1_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr2 register
     * \return String representing the name of the rh850g4 wr2 register
     */
    virtual std::string register_name_wr2(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr2_u register
     * \return String representing the name of the rh850g4 wr2_u register
     */
    virtual std::string register_name_wr2_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr3 register
     * \return String representing the name of the rh850g4 wr3 register
     */
    virtual std::string register_name_wr3(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr3_u register
     * \return String representing the name of the rh850g4 wr3_u register
     */
    virtual std::string register_name_wr3_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr4 register
     * \return String representing the name of the rh850g4 wr4 register
     */
    virtual std::string register_name_wr4(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr4_u register
     * \return String representing the name of the rh850g4 wr4_u register
     */
    virtual std::string register_name_wr4_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr5 register
     * \return String representing the name of the rh850g4 wr5 register
     */
    virtual std::string register_name_wr5(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr5_u register
     * \return String representing the name of the rh850g4 wr5_u register
     */
    virtual std::string register_name_wr5_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr6 register
     * \return String representing the name of the rh850g4 wr6 register
     */
    virtual std::string register_name_wr6(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr6_u register
     * \return String representing the name of the rh850g4 wr6_u register
     */
    virtual std::string register_name_wr6_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr7 register
     * \return String representing the name of the rh850g4 wr7 register
     */
    virtual std::string register_name_wr7(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr7_u register
     * \return String representing the name of the rh850g4 wr7_u register
     */
    virtual std::string register_name_wr7_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr8 register
     * \return String representing the name of the rh850g4 wr8 register
     */
    virtual std::string register_name_wr8(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr8_u register
     * \return String representing the name of the rh850g4 wr8_u register
     */
    virtual std::string register_name_wr8_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr9 register
     * \return String representing the name of the rh850g4 wr9 register
     */
    virtual std::string register_name_wr9(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr9_u register
     * \return String representing the name of the rh850g4 wr9_u register
     */
    virtual std::string register_name_wr9_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr10 register
     * \return String representing the name of the rh850g4 wr10 register
     */
    virtual std::string register_name_wr10(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr10_u register
     * \return String representing the name of the rh850g4 wr10_u register
     */
    virtual std::string register_name_wr10_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr11 register
     * \return String representing the name of the rh850g4 wr11 register
     */
    virtual std::string register_name_wr11(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr11_u register
     * \return String representing the name of the rh850g4 wr11_u register
     */
    virtual std::string register_name_wr11_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr12 register
     * \return String representing the name of the rh850g4 wr12 register
     */
    virtual std::string register_name_wr12(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr12_u register
     * \return String representing the name of the rh850g4 wr12_u register
     */
    virtual std::string register_name_wr12_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr13 register
     * \return String representing the name of the rh850g4 wr13 register
     */
    virtual std::string register_name_wr13(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr13_u register
     * \return String representing the name of the rh850g4 wr13_u register
     */
    virtual std::string register_name_wr13_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr14 register
     * \return String representing the name of the rh850g4 wr14 register
     */
    virtual std::string register_name_wr14(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr14_u register
     * \return String representing the name of the rh850g4 wr14_u register
     */
    virtual std::string register_name_wr14_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr15 register
     * \return String representing the name of the rh850g4 wr15 register
     */
    virtual std::string register_name_wr15(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr15_u register
     * \return String representing the name of the rh850g4 wr15_u register
     */
    virtual std::string register_name_wr15_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr16 register
     * \return String representing the name of the rh850g4 wr16 register
     */
    virtual std::string register_name_wr16(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr16_u register
     * \return String representing the name of the rh850g4 wr16_u register
     */
    virtual std::string register_name_wr16_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr17 register
     * \return String representing the name of the rh850g4 wr17 register
     */
    virtual std::string register_name_wr17(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr17_u register
     * \return String representing the name of the rh850g4 wr17_u register
     */
    virtual std::string register_name_wr17_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr18 register
     * \return String representing the name of the rh850g4 wr18 register
     */
    virtual std::string register_name_wr18(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr18_u register
     * \return String representing the name of the rh850g4 wr18_u register
     */
    virtual std::string register_name_wr18_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr19 register
     * \return String representing the name of the rh850g4 wr19 register
     */
    virtual std::string register_name_wr19(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr19_u register
     * \return String representing the name of the rh850g4 wr19_u register
     */
    virtual std::string register_name_wr19_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr20 register
     * \return String representing the name of the rh850g4 wr20 register
     */
    virtual std::string register_name_wr20(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr20_u register
     * \return String representing the name of the rh850g4 wr20_u register
     */
    virtual std::string register_name_wr20_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr21 register
     * \return String representing the name of the rh850g4 wr21 register
     */
    virtual std::string register_name_wr21(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr21_u register
     * \return String representing the name of the rh850g4 wr21_u register
     */
    virtual std::string register_name_wr21_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr22 register
     * \return String representing the name of the rh850g4 wr22 register
     */
    virtual std::string register_name_wr22(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr22_u register
     * \return String representing the name of the rh850g4 wr22_u register
     */
    virtual std::string register_name_wr22_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr23 register
     * \return String representing the name of the rh850g4 wr23 register
     */
    virtual std::string register_name_wr23(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr23_u register
     * \return String representing the name of the rh850g4 wr23_u register
     */
    virtual std::string register_name_wr23_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr24 register
     * \return String representing the name of the rh850g4 wr24 register
     */
    virtual std::string register_name_wr24(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr24_u register
     * \return String representing the name of the rh850g4 wr24_u register
     */
    virtual std::string register_name_wr24_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr25 register
     * \return String representing the name of the rh850g4 wr25 register
     */
    virtual std::string register_name_wr25(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr25_u register
     * \return String representing the name of the rh850g4 wr25_u register
     */
    virtual std::string register_name_wr25_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr26 register
     * \return String representing the name of the rh850g4 wr26 register
     */
    virtual std::string register_name_wr26(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr26_u register
     * \return String representing the name of the rh850g4 wr26_u register
     */
    virtual std::string register_name_wr26_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr27 register
     * \return String representing the name of the rh850g4 wr27 register
     */
    virtual std::string register_name_wr27(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr27_u register
     * \return String representing the name of the rh850g4 wr27_u register
     */
    virtual std::string register_name_wr27_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr28 register
     * \return String representing the name of the rh850g4 wr28 register
     */
    virtual std::string register_name_wr28(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr28_u register
     * \return String representing the name of the rh850g4 wr28_u register
     */
    virtual std::string register_name_wr28_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr29 register
     * \return String representing the name of the rh850g4 wr29 register
     */
    virtual std::string register_name_wr29(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr29_u register
     * \return String representing the name of the rh850g4 wr29_u register
     */
    virtual std::string register_name_wr29_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr30 register
     * \return String representing the name of the rh850g4 wr30 register
     */
    virtual std::string register_name_wr30(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr30_u register
     * \return String representing the name of the rh850g4 wr30_u register
     */
    virtual std::string register_name_wr30_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr31 register
     * \return String representing the name of the rh850g4 wr31 register
     */
    virtual std::string register_name_wr31(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr31_u register
     * \return String representing the name of the rh850g4 wr31_u register
     */
    virtual std::string register_name_wr31_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 EIPC register
     * \return String representing the name of the rh850g4 EIPC register
     */
    virtual std::string register_name_EIPC(void);
    /**
     * \brief Generates string representing name of the rh850g4 EIPSW register
     * \return String representing the name of the rh850g4 EIPSW register
     */
    virtual std::string register_name_EIPSW(void);
    /**
     * \brief Generates string representing name of the rh850g4 FEPC register
     * \return String representing the name of the rh850g4 FEPC register
     */
    virtual std::string register_name_FEPC(void);
    /**
     * \brief Generates string representing name of the rh850g4 FEPSW register
     * \return String representing the name of the rh850g4 FEPSW register
     */
    virtual std::string register_name_FEPSW(void);
    /**
     * \brief Generates string representing name of the rh850g4 PSW register
     * \return String representing the name of the rh850g4 PSW register
     */
    virtual std::string register_name_PSW(void);
    /**
     * \brief Generates string representing name of the rh850g4 FPSR register
     * \return String representing the name of the rh850g4 FPSR register
     */
    virtual std::string register_name_FPSR(void);
    /**
     * \brief Generates string representing name of the rh850g4 FPEPC register
     * \return String representing the name of the rh850g4 FPEPC register
     */
    virtual std::string register_name_FPEPC(void);
    /**
     * \brief Generates string representing name of the rh850g4 FPST register
     * \return String representing the name of the rh850g4 FPST register
     */
    virtual std::string register_name_FPST(void);
    /**
     * \brief Generates string representing name of the rh850g4 FPCC register
     * \return String representing the name of the rh850g4 FPCC register
     */
    virtual std::string register_name_FPCC(void);
    /**
     * \brief Generates string representing name of the rh850g4 FPCFG register
     * \return String representing the name of the rh850g4 FPCFG register
     */
    virtual std::string register_name_FPCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 EIIC register
     * \return String representing the name of the rh850g4 EIIC register
     */
    virtual std::string register_name_EIIC(void);
    /**
     * \brief Generates string representing name of the rh850g4 FEIC register
     * \return String representing the name of the rh850g4 FEIC register
     */
    virtual std::string register_name_FEIC(void);
    /**
     * \brief Generates string representing name of the rh850g4 PSWH register
     * \return String representing the name of the rh850g4 PSWH register
     */
    virtual std::string register_name_PSWH(void);
    /**
     * \brief Generates string representing name of the rh850g4 CTPC register
     * \return String representing the name of the rh850g4 CTPC register
     */
    virtual std::string register_name_CTPC(void);
    /**
     * \brief Generates string representing name of the rh850g4 CTPSW register
     * \return String representing the name of the rh850g4 CTPSW register
     */
    virtual std::string register_name_CTPSW(void);
    /**
     * \brief Generates string representing name of the rh850g4 EIPSWH register
     * \return String representing the name of the rh850g4 EIPSWH register
     */
    virtual std::string register_name_EIPSWH(void);
    /**
     * \brief Generates string representing name of the rh850g4 FEPSWH register
     * \return String representing the name of the rh850g4 FEPSWH register
     */
    virtual std::string register_name_FEPSWH(void);
    /**
     * \brief Generates string representing name of the rh850g4 CTBP register
     * \return String representing the name of the rh850g4 CTBP register
     */
    virtual std::string register_name_CTBP(void);
    /**
     * \brief Generates string representing name of the rh850g4 SNZCFG register
     * \return String representing the name of the rh850g4 SNZCFG register
     */
    virtual std::string register_name_SNZCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 EIWR register
     * \return String representing the name of the rh850g4 EIWR register
     */
    virtual std::string register_name_EIWR(void);
    /**
     * \brief Generates string representing name of the rh850g4 FEWR register
     * \return String representing the name of the rh850g4 FEWR register
     */
    virtual std::string register_name_FEWR(void);
    /**
     * \brief Generates string representing name of the rh850g4 SPID register
     * \return String representing the name of the rh850g4 SPID register
     */
    virtual std::string register_name_SPID(void);
    /**
     * \brief Generates string representing name of the rh850g4 SPIDLIST register
     * \return String representing the name of the rh850g4 SPIDLIST register
     */
    virtual std::string register_name_SPIDLIST(void);
    /**
     * \brief Generates string representing name of the rh850g4 RBASE register
     * \return String representing the name of the rh850g4 RBASE register
     */
    virtual std::string register_name_RBASE(void);
    /**
     * \brief Generates string representing name of the rh850g4 EBASE register
     * \return String representing the name of the rh850g4 EBASE register
     */
    virtual std::string register_name_EBASE(void);
    /**
     * \brief Generates string representing name of the rh850g4 INTBP register
     * \return String representing the name of the rh850g4 INTBP register
     */
    virtual std::string register_name_INTBP(void);
    /**
     * \brief Generates string representing name of the rh850g4 MCTL register
     * \return String representing the name of the rh850g4 MCTL register
     */
    virtual std::string register_name_MCTL(void);
    /**
     * \brief Generates string representing name of the rh850g4 PID register
     * \return String representing the name of the rh850g4 PID register
     */
    virtual std::string register_name_PID(void);
    /**
     * \brief Generates string representing name of the rh850g4 SVLOCK register
     * \return String representing the name of the rh850g4 SVLOCK register
     */
    virtual std::string register_name_SVLOCK(void);
    /**
     * \brief Generates string representing name of the rh850g4 SCCFG register
     * \return String representing the name of the rh850g4 SCCFG register
     */
    virtual std::string register_name_SCCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 SCBP register
     * \return String representing the name of the rh850g4 SCBP register
     */
    virtual std::string register_name_SCBP(void);
    /**
     * \brief Generates string representing name of the rh850g4 HVCFG register
     * \return String representing the name of the rh850g4 HVCFG register
     */
    virtual std::string register_name_HVCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMCFG register
     * \return String representing the name of the rh850g4 GMCFG register
     */
    virtual std::string register_name_GMCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 HVSB register
     * \return String representing the name of the rh850g4 HVSB register
     */
    virtual std::string register_name_HVSB(void);
    /**
     * \brief Generates string representing name of the rh850g4 PEID register
     * \return String representing the name of the rh850g4 PEID register
     */
    virtual std::string register_name_PEID(void);
    /**
     * \brief Generates string representing name of the rh850g4 BMID register
     * \return String representing the name of the rh850g4 BMID register
     */
    virtual std::string register_name_BMID(void);
    /**
     * \brief Generates string representing name of the rh850g4 MEA register
     * \return String representing the name of the rh850g4 MEA register
     */
    virtual std::string register_name_MEA(void);
    /**
     * \brief Generates string representing name of the rh850g4 MEI register
     * \return String representing the name of the rh850g4 MEI register
     */
    virtual std::string register_name_MEI(void);
    /**
     * \brief Generates string representing name of the rh850g4 RBCR0 register
     * \return String representing the name of the rh850g4 RBCR0 register
     */
    virtual std::string register_name_RBCR0(void);
    /**
     * \brief Generates string representing name of the rh850g4 RBCR1 register
     * \return String representing the name of the rh850g4 RBCR1 register
     */
    virtual std::string register_name_RBCR1(void);
    /**
     * \brief Generates string representing name of the rh850g4 RBNR register
     * \return String representing the name of the rh850g4 RBNR register
     */
    virtual std::string register_name_RBNR(void);
    /**
     * \brief Generates string representing name of the rh850g4 RBIP register
     * \return String representing the name of the rh850g4 RBIP register
     */
    virtual std::string register_name_RBIP(void);
    /**
     * \brief Generates string representing name of the rh850g4 ISPR register
     * \return String representing the name of the rh850g4 ISPR register
     */
    virtual std::string register_name_ISPR(void);
    /**
     * \brief Generates string representing name of the rh850g4 IMSR register
     * \return String representing the name of the rh850g4 IMSR register
     */
    virtual std::string register_name_IMSR(void);
    /**
     * \brief Generates string representing name of the rh850g4 ICSR register
     * \return String representing the name of the rh850g4 ICSR register
     */
    virtual std::string register_name_ICSR(void);
    /**
     * \brief Generates string representing name of the rh850g4 INTCFG register
     * \return String representing the name of the rh850g4 INTCFG register
     */
    virtual std::string register_name_INTCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 PLMR register
     * \return String representing the name of the rh850g4 PLMR register
     */
    virtual std::string register_name_PLMR(void);
    /**
     * \brief Generates string representing name of the rh850g4 FXSR register
     * \return String representing the name of the rh850g4 FXSR register
     */
    virtual std::string register_name_FXSR(void);
    /**
     * \brief Generates string representing name of the rh850g4 FXST register
     * \return String representing the name of the rh850g4 FXST register
     */
    virtual std::string register_name_FXST(void);
    /**
     * \brief Generates string representing name of the rh850g4 FXINFO register
     * \return String representing the name of the rh850g4 FXINFO register
     */
    virtual std::string register_name_FXINFO(void);
    /**
     * \brief Generates string representing name of the rh850g4 FXCFG register
     * \return String representing the name of the rh850g4 FXCFG register
     */
    virtual std::string register_name_FXCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 FXXC register
     * \return String representing the name of the rh850g4 FXXC register
     */
    virtual std::string register_name_FXXC(void);
    /**
     * \brief Generates string representing name of the rh850g4 FXXP register
     * \return String representing the name of the rh850g4 FXXP register
     */
    virtual std::string register_name_FXXP(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMEIPC register
     * \return String representing the name of the rh850g4 GMEIPC register
     */
    virtual std::string register_name_GMEIPC(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMEIPSW register
     * \return String representing the name of the rh850g4 GMEIPSW register
     */
    virtual std::string register_name_GMEIPSW(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMFEPC register
     * \return String representing the name of the rh850g4 GMFEPC register
     */
    virtual std::string register_name_GMFEPC(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMFEPSW register
     * \return String representing the name of the rh850g4 GMFEPSW register
     */
    virtual std::string register_name_GMFEPSW(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMPSW register
     * \return String representing the name of the rh850g4 GMPSW register
     */
    virtual std::string register_name_GMPSW(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMMEA register
     * \return String representing the name of the rh850g4 GMMEA register
     */
    virtual std::string register_name_GMMEA(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMMEI register
     * \return String representing the name of the rh850g4 GMMEI register
     */
    virtual std::string register_name_GMMEI(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMEIIC register
     * \return String representing the name of the rh850g4 GMEIIC register
     */
    virtual std::string register_name_GMEIIC(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMFEIC register
     * \return String representing the name of the rh850g4 GMFEIC register
     */
    virtual std::string register_name_GMFEIC(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMSPID register
     * \return String representing the name of the rh850g4 GMSPID register
     */
    virtual std::string register_name_GMSPID(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMSPIDLIST register
     * \return String representing the name of the rh850g4 GMSPIDLIST register
     */
    virtual std::string register_name_GMSPIDLIST(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMEBASE register
     * \return String representing the name of the rh850g4 GMEBASE register
     */
    virtual std::string register_name_GMEBASE(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMINTBP register
     * \return String representing the name of the rh850g4 GMINTBP register
     */
    virtual std::string register_name_GMINTBP(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMINTCFG register
     * \return String representing the name of the rh850g4 GMINTCFG register
     */
    virtual std::string register_name_GMINTCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMPLMR register
     * \return String representing the name of the rh850g4 GMPLMR register
     */
    virtual std::string register_name_GMPLMR(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMSVLOCK register
     * \return String representing the name of the rh850g4 GMSVLOCK register
     */
    virtual std::string register_name_GMSVLOCK(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMMPM register
     * \return String representing the name of the rh850g4 GMMPM register
     */
    virtual std::string register_name_GMMPM(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMEIWR register
     * \return String representing the name of the rh850g4 GMEIWR register
     */
    virtual std::string register_name_GMEIWR(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMFEWR register
     * \return String representing the name of the rh850g4 GMFEWR register
     */
    virtual std::string register_name_GMFEWR(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMPEID register
     * \return String representing the name of the rh850g4 GMPEID register
     */
    virtual std::string register_name_GMPEID(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPM register
     * \return String representing the name of the rh850g4 MPM register
     */
    virtual std::string register_name_MPM(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPCFG register
     * \return String representing the name of the rh850g4 MPCFG register
     */
    virtual std::string register_name_MPCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 MCA register
     * \return String representing the name of the rh850g4 MCA register
     */
    virtual std::string register_name_MCA(void);
    /**
     * \brief Generates string representing name of the rh850g4 MCS register
     * \return String representing the name of the rh850g4 MCS register
     */
    virtual std::string register_name_MCS(void);
    /**
     * \brief Generates string representing name of the rh850g4 MCC register
     * \return String representing the name of the rh850g4 MCC register
     */
    virtual std::string register_name_MCC(void);
    /**
     * \brief Generates string representing name of the rh850g4 MCR register
     * \return String representing the name of the rh850g4 MCR register
     */
    virtual std::string register_name_MCR(void);
    /**
     * \brief Generates string representing name of the rh850g4 MCI register
     * \return String representing the name of the rh850g4 MCI register
     */
    virtual std::string register_name_MCI(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPIDX register
     * \return String representing the name of the rh850g4 MPIDX register
     */
    virtual std::string register_name_MPIDX(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPBK register
     * \return String representing the name of the rh850g4 MPBK register
     */
    virtual std::string register_name_MPBK(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPLA register
     * \return String representing the name of the rh850g4 MPLA register
     */
    virtual std::string register_name_MPLA(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPUA register
     * \return String representing the name of the rh850g4 MPUA register
     */
    virtual std::string register_name_MPUA(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPAT register
     * \return String representing the name of the rh850g4 MPAT register
     */
    virtual std::string register_name_MPAT(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPID0 register
     * \return String representing the name of the rh850g4 MPID0 register
     */
    virtual std::string register_name_MPID0(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPID1 register
     * \return String representing the name of the rh850g4 MPID1 register
     */
    virtual std::string register_name_MPID1(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPID2 register
     * \return String representing the name of the rh850g4 MPID2 register
     */
    virtual std::string register_name_MPID2(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPID3 register
     * \return String representing the name of the rh850g4 MPID3 register
     */
    virtual std::string register_name_MPID3(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPID4 register
     * \return String representing the name of the rh850g4 MPID4 register
     */
    virtual std::string register_name_MPID4(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPID5 register
     * \return String representing the name of the rh850g4 MPID5 register
     */
    virtual std::string register_name_MPID5(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPID6 register
     * \return String representing the name of the rh850g4 MPID6 register
     */
    virtual std::string register_name_MPID6(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPID7 register
     * \return String representing the name of the rh850g4 MPID7 register
     */
    virtual std::string register_name_MPID7(void);
    /**
     * \brief Generates string representing name of the rh850g4 ICTAGL register
     * \return String representing the name of the rh850g4 ICTAGL register
     */
    virtual std::string register_name_ICTAGL(void);
    /**
     * \brief Generates string representing name of the rh850g4 ICTAGH register
     * \return String representing the name of the rh850g4 ICTAGH register
     */
    virtual std::string register_name_ICTAGH(void);
    /**
     * \brief Generates string representing name of the rh850g4 ICDATL register
     * \return String representing the name of the rh850g4 ICDATL register
     */
    virtual std::string register_name_ICDATL(void);
    /**
     * \brief Generates string representing name of the rh850g4 ICDATH register
     * \return String representing the name of the rh850g4 ICDATH register
     */
    virtual std::string register_name_ICDATH(void);
    /**
     * \brief Generates string representing name of the rh850g4 ICCTRL register
     * \return String representing the name of the rh850g4 ICCTRL register
     */
    virtual std::string register_name_ICCTRL(void);
    /**
     * \brief Generates string representing name of the rh850g4 ICCFG register
     * \return String representing the name of the rh850g4 ICCFG register
     */
    virtual std::string register_name_ICCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 ICERR register
     * \return String representing the name of the rh850g4 ICERR register
     */
    virtual std::string register_name_ICERR(void);
    /**
     * \brief Generates string representing name of the rh850g4 TSCOUNTL register
     * \return String representing the name of the rh850g4 TSCOUNTL register
     */
    virtual std::string register_name_TSCOUNTL(void);
    /**
     * \brief Generates string representing name of the rh850g4 TSCOUNTH register
     * \return String representing the name of the rh850g4 TSCOUNTH register
     */
    virtual std::string register_name_TSCOUNTH(void);
    /**
     * \brief Generates string representing name of the rh850g4 TSCTRL register
     * \return String representing the name of the rh850g4 TSCTRL register
     */
    virtual std::string register_name_TSCTRL(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMUMCTRL register
     * \return String representing the name of the rh850g4 PMUMCTRL register
     */
    virtual std::string register_name_PMUMCTRL(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMGMCTRL register
     * \return String representing the name of the rh850g4 PMGMCTRL register
     */
    virtual std::string register_name_PMGMCTRL(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT0 register
     * \return String representing the name of the rh850g4 PMCOUNT0 register
     */
    virtual std::string register_name_PMCOUNT0(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL0 register
     * \return String representing the name of the rh850g4 PMCTRL0 register
     */
    virtual std::string register_name_PMCTRL0(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT1 register
     * \return String representing the name of the rh850g4 PMCOUNT1 register
     */
    virtual std::string register_name_PMCOUNT1(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL1 register
     * \return String representing the name of the rh850g4 PMCTRL1 register
     */
    virtual std::string register_name_PMCTRL1(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT2 register
     * \return String representing the name of the rh850g4 PMCOUNT2 register
     */
    virtual std::string register_name_PMCOUNT2(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL2 register
     * \return String representing the name of the rh850g4 PMCTRL2 register
     */
    virtual std::string register_name_PMCTRL2(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT3 register
     * \return String representing the name of the rh850g4 PMCOUNT3 register
     */
    virtual std::string register_name_PMCOUNT3(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL3 register
     * \return String representing the name of the rh850g4 PMCTRL3 register
     */
    virtual std::string register_name_PMCTRL3(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL4 register
     * \return String representing the name of the rh850g4 PMCTRL4 register
     */
    virtual std::string register_name_PMCTRL4(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL5 register
     * \return String representing the name of the rh850g4 PMCTRL5 register
     */
    virtual std::string register_name_PMCTRL5(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL6 register
     * \return String representing the name of the rh850g4 PMCTRL6 register
     */
    virtual std::string register_name_PMCTRL6(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL7 register
     * \return String representing the name of the rh850g4 PMCTRL7 register
     */
    virtual std::string register_name_PMCTRL7(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT4 register
     * \return String representing the name of the rh850g4 PMCOUNT4 register
     */
    virtual std::string register_name_PMCOUNT4(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT5 register
     * \return String representing the name of the rh850g4 PMCOUNT5 register
     */
    virtual std::string register_name_PMCOUNT5(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT6 register
     * \return String representing the name of the rh850g4 PMCOUNT6 register
     */
    virtual std::string register_name_PMCOUNT6(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT7 register
     * \return String representing the name of the rh850g4 PMCOUNT7 register
     */
    virtual std::string register_name_PMCOUNT7(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND0 register
     * \return String representing the name of the rh850g4 PMSUBCND0 register
     */
    virtual std::string register_name_PMSUBCND0(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND1 register
     * \return String representing the name of the rh850g4 PMSUBCND1 register
     */
    virtual std::string register_name_PMSUBCND1(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND2 register
     * \return String representing the name of the rh850g4 PMSUBCND2 register
     */
    virtual std::string register_name_PMSUBCND2(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND3 register
     * \return String representing the name of the rh850g4 PMSUBCND3 register
     */
    virtual std::string register_name_PMSUBCND3(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND4 register
     * \return String representing the name of the rh850g4 PMSUBCND4 register
     */
    virtual std::string register_name_PMSUBCND4(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND5 register
     * \return String representing the name of the rh850g4 PMSUBCND5 register
     */
    virtual std::string register_name_PMSUBCND5(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND6 register
     * \return String representing the name of the rh850g4 PMSUBCND6 register
     */
    virtual std::string register_name_PMSUBCND6(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND7 register
     * \return String representing the name of the rh850g4 PMSUBCND7 register
     */
    virtual std::string register_name_PMSUBCND7(void);
    /**
     * \brief Generates string representing name of the rh850g4 DBGEN register
     * \return String representing the name of the rh850g4 DBGEN register
     */
    virtual std::string register_name_DBGEN(void);
    /**
     * \brief Generates string representing name of the rh850g4 DBPSWH register
     * \return String representing the name of the rh850g4 DBPSWH register
     */
    virtual std::string register_name_DBPSWH(void);
    /**
     * \brief Generates string representing name of the rh850g4 DBIC register
     * \return String representing the name of the rh850g4 DBIC register
     */
    virtual std::string register_name_DBIC(void);
    /**
     * \brief Generates string representing name of the rh850g4 DBPC register
     * \return String representing the name of the rh850g4 DBPC register
     */
    virtual std::string register_name_DBPC(void);
    /**
     * \brief Generates string representing name of the rh850g4 DBPSW register
     * \return String representing the name of the rh850g4 DBPSW register
     */
    virtual std::string register_name_DBPSW(void);
    /**
     * \brief Generates string representing name of the rh850g4 DIR0 register
     * \return String representing the name of the rh850g4 DIR0 register
     */
    virtual std::string register_name_DIR0(void);
    /**
     * \brief Generates string representing name of the rh850g4 DIR1 register
     * \return String representing the name of the rh850g4 DIR1 register
     */
    virtual std::string register_name_DIR1(void);
    /**
     * \brief Generates string representing name of the rh850g4 BPC register
     * \return String representing the name of the rh850g4 BPC register
     */
    virtual std::string register_name_BPC(void);
    /**
     * \brief Generates string representing name of the rh850g4 BPAV register
     * \return String representing the name of the rh850g4 BPAV register
     */
    virtual std::string register_name_BPAV(void);
    /**
     * \brief Generates string representing name of the rh850g4 BPAM register
     * \return String representing the name of the rh850g4 BPAM register
     */
    virtual std::string register_name_BPAM(void);
    /**
     * \brief Generates string representing name of the rh850g4 DBCFG register
     * \return String representing the name of the rh850g4 DBCFG register
     */
    virtual std::string register_name_DBCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 DBWR register
     * \return String representing the name of the rh850g4 DBWR register
     */
    virtual std::string register_name_DBWR(void);
    /**
     * \brief Generates string representing name of the rh850g4 LSTEST0 register
     * \return String representing the name of the rh850g4 LSTEST0 register
     */
    virtual std::string register_name_LSTEST0(void);
    /**
     * \brief Generates string representing name of the rh850g4 LSTEST1 register
     * \return String representing the name of the rh850g4 LSTEST1 register
     */
    virtual std::string register_name_LSTEST1(void);
    /**
     * \brief Generates string representing name of the rh850g4 LSCFG register
     * \return String representing the name of the rh850g4 LSCFG register
     */
    virtual std::string register_name_LSCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 ICBKEY register
     * \return String representing the name of the rh850g4 ICBKEY register
     */
    virtual std::string register_name_ICBKEY(void);
    /**
     * \brief Generates string representing name of the rh850g4 IFCR register
     * \return String representing the name of the rh850g4 IFCR register
     */
    virtual std::string register_name_IFCR(void);
    /**
     * \brief Generates string representing name of the rh850g4 IFCR1 register
     * \return String representing the name of the rh850g4 IFCR1 register
     */
    virtual std::string register_name_IFCR1(void);
    /**
     * \brief Generates string representing name of the rh850g4 BRPCTRL0 register
     * \return String representing the name of the rh850g4 BRPCTRL0 register
     */
    virtual std::string register_name_BRPCTRL0(void);
    /**
     * \brief Generates string representing name of the rh850g4 BRPCTRL1 register
     * \return String representing the name of the rh850g4 BRPCTRL1 register
     */
    virtual std::string register_name_BRPCTRL1(void);
    /**
     * \brief Generates string representing name of the rh850g4 BRPCFG register
     * \return String representing the name of the rh850g4 BRPCFG register
     */
    virtual std::string register_name_BRPCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 BRPACTRL register
     * \return String representing the name of the rh850g4 BRPACTRL register
     */
    virtual std::string register_name_BRPACTRL(void);
    /**
     * \brief Generates string representing name of the rh850g4 BRPDATA register
     * \return String representing the name of the rh850g4 BRPDATA register
     */
    virtual std::string register_name_BRPDATA(void);
    /**
     * \brief Generates string representing name of the rh850g4 DCBKEY register
     * \return String representing the name of the rh850g4 DCBKEY register
     */
    virtual std::string register_name_DCBKEY(void);
    /**
     * \brief Generates string representing name of the rh850g4 LSUCR register
     * \return String representing the name of the rh850g4 LSUCR register
     */
    virtual std::string register_name_LSUCR(void);
    /**
     * \brief Generates string representing name of the rh850g4 LSULNK0 register
     * \return String representing the name of the rh850g4 LSULNK0 register
     */
    virtual std::string register_name_LSULNK0(void);
    /**
     * \brief Generates string representing name of the rh850g4 LSULNK1 register
     * \return String representing the name of the rh850g4 LSULNK1 register
     */
    virtual std::string register_name_LSULNK1(void);
    /**
     * \brief Generates string representing name of the rh850g4 L1RLCR register
     * \return String representing the name of the rh850g4 L1RLCR register
     */
    virtual std::string register_name_L1RLCR(void);
    /**
     * \brief Generates string representing name of the rh850g4 L1RLNK0 register
     * \return String representing the name of the rh850g4 L1RLNK0 register
     */
    virtual std::string register_name_L1RLNK0(void);
    /**
     * \brief Generates string representing name of the rh850g4 L1RLNK1 register
     * \return String representing the name of the rh850g4 L1RLNK1 register
     */
    virtual std::string register_name_L1RLNK1(void);
    /**
     * \brief Generates string representing name of the rh850g4 L1RCFG register
     * \return String representing the name of the rh850g4 L1RCFG register
     */
    virtual std::string register_name_L1RCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 DECFG register
     * \return String representing the name of the rh850g4 DECFG register
     */
    virtual std::string register_name_DECFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 DECTRL register
     * \return String representing the name of the rh850g4 DECTRL register
     */
    virtual std::string register_name_DECTRL(void);
    /**
     * \brief Generates string representing name of the rh850g4 DEVDS register
     * \return String representing the name of the rh850g4 DEVDS register
     */
    virtual std::string register_name_DEVDS(void);
    /**
     * \brief Generates string representing name of the rh850g4 RDBCR register
     * \return String representing the name of the rh850g4 RDBCR register
     */
    virtual std::string register_name_RDBCR(void);
    /**
     * \brief Generates string representing name of the rh850g4 RDBACR register
     * \return String representing the name of the rh850g4 RDBACR register
     */
    virtual std::string register_name_RDBACR(void);
    /**
     * \brief Generates string representing name of the rh850g4 RDBATAG register
     * \return String representing the name of the rh850g4 RDBATAG register
     */
    virtual std::string register_name_RDBATAG(void);
    /**
     * \brief Generates string representing name of the rh850g4 RDBADAT0 register
     * \return String representing the name of the rh850g4 RDBADAT0 register
     */
    virtual std::string register_name_RDBADAT0(void);
    /**
     * \brief Generates string representing name of the rh850g4 RDBADAT1 register
     * \return String representing the name of the rh850g4 RDBADAT1 register
     */
    virtual std::string register_name_RDBADAT1(void);
    /**
     * \brief Generates string representing name of the rh850g4 RDBADAT2 register
     * \return String representing the name of the rh850g4 RDBADAT2 register
     */
    virtual std::string register_name_RDBADAT2(void);
    /**
     * \brief Generates string representing name of the rh850g4 RDBADAT3 register
     * \return String representing the name of the rh850g4 RDBADAT3 register
     */
    virtual std::string register_name_RDBADAT3(void);
    /**
     * \brief Generates string representing name of the rh850g4 RDBSTAT register
     * \return String representing the name of the rh850g4 RDBSTAT register
     */
    virtual std::string register_name_RDBSTAT(void);
    /**
     * \brief Generates string representing name of the rh850g4 hv_func_support register
     * \return String representing the name of the rh850g4 hv_func_support register
     */
    virtual std::string register_name_hv_func_support(void);
    /**
     * \brief Generates string representing name of the rh850g4 swd0pcad_exec_count register
     * \return String representing the name of the rh850g4 swd0pcad_exec_count register
     */
    virtual std::string register_name_swd0pcad_exec_count(void);
    /**
     * \brief Generates string representing name of the rh850g4 swd0pcad_val register
     * \return String representing the name of the rh850g4 swd0pcad_val register
     */
    virtual std::string register_name_swd0pcad_val(void);
    /**
     * \brief Generates string representing name of the rh850g4 mem_acc_type register
     * \return String representing the name of the rh850g4 mem_acc_type register
     */
    virtual std::string register_name_mem_acc_type(void);

    // register group name operations
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 GR register group
     * \param index Index value of the required register from the rh850g4 GR register group
     * \return String representing the name of the specified rh850g4 GR group register
     */
    virtual std::string register_name_GR(rh850g4::GR_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 WR register group
     * \param index Index value of the required register from the rh850g4 WR register group
     * \return String representing the name of the specified rh850g4 WR group register
     */
    virtual std::string register_name_WR(rh850g4::WR_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_0 group register
     */
    virtual std::string register_name_SR_SELID_0(rh850g4::SR_SELID_0_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_1 group register
     */
    virtual std::string register_name_SR_SELID_1(rh850g4::SR_SELID_1_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_2 group register
     */
    virtual std::string register_name_SR_SELID_2(rh850g4::SR_SELID_2_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_3 group register
     */
    virtual std::string register_name_SR_SELID_3(rh850g4::SR_SELID_3_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_4 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_4 group register
     */
    virtual std::string register_name_SR_SELID_4(rh850g4::SR_SELID_4_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_5 group register
     */
    virtual std::string register_name_SR_SELID_5(rh850g4::SR_SELID_5_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_9 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_9 group register
     */
    virtual std::string register_name_SR_SELID_9(rh850g4::SR_SELID_9_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_10 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_10 group register
     */
    virtual std::string register_name_SR_SELID_10(rh850g4::SR_SELID_10_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_11 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_11 group register
     */
    virtual std::string register_name_SR_SELID_11(rh850g4::SR_SELID_11_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_12 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_12 group register
     */
    virtual std::string register_name_SR_SELID_12(rh850g4::SR_SELID_12_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_13 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_13 group register
     */
    virtual std::string register_name_SR_SELID_13(rh850g4::SR_SELID_13_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_14 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_14 group register
     */
    virtual std::string register_name_SR_SELID_14(rh850g4::SR_SELID_14_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_15 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_15 group register
     */
    virtual std::string register_name_SR_SELID_15(rh850g4::SR_SELID_15_index_t index);

    // immediate instruction count operations
    /**
     * \brief Queries the number of instructions executed in the current quanta
     * \return Unsigned integer value representing the number of instructions executed in the current quanta 
     */
    unsigned int read_instruction_count(void);
    /**
     * \brief Set the number of instructions executed in the current quanta
     * \param value Unsigned integer value representing the new number of instructions executed in the current quanta 
     */
    void write_instruction_count(unsigned int value);

    // immediate time operations
    /**
     * \brief Queries the time executed in the current quanta
     * \return Value representing the time executed in the current quanta
     */
    unsigned long long read_time(void);
    /**
     * \brief Set the time executed in the current quanta
     * \param value Value representing the new time executed in the current quanta
     */
    void write_time(unsigned long long value);
    /**
     * \brief Increment the time executed in the current quanta by the specified amount
     * \param increment Amount by which to increment the time executed in the current quanta
     */
    void increment_time(unsigned long long increment);
    /**
     * \brief Decrement the time executed in the current quanta by the specified amount
     * \param decrement Amount by which to decrement the time in the current quanta
     */
    void decrement_time(unsigned long long decrement);
    /**
     * \brief Update the accrued time value. The accrued time value is used
     * during TLM transactions to keep track of the part of local time that has
     * been 'accrued' against global time.
     * \param accrued_time New accrued time value
     */
    void write_accrued_time(unsigned long long accrued_time);
    /**
     * \brief Read the accrued time value. The accrued time value is used
     * during TLM transactions to keep track of the part of local time that has
     * been 'accrued' against global time.
     * \return Value representing the accrued time
     */
    unsigned long long read_accrued_time(void);
    
    // immediate cycle period operations
    /**
     * \brief Queries the period of time taken to execute a cycle
     * \return Value representing the period of time taken to execute a cycle
     */
    unsigned long long read_cycle_period(void);
    /**
     * \brief Set the period of time taken to execute a cycle
     * \param value Value representing the new period of time taken to execute a cycle
     */
    void write_cycle_period(unsigned long long value);
    /**
     * \brief Increment the period of time taken to execute a cycle
     * \param increment Amount by which to increment the period of time taken to execute a cycle
     */
    void increment_cycle_period(unsigned long long increment);
    /**
     * \brief Decrement the period of time taken to execute a cycle
     * \param decrement Amount by which to decrement the period of time taken to execute a cycle
     */
    void decrement_cycle_period(unsigned long long decrement);

    // immediate core pointer operations
    /**
     * \brief Queries the value of the pointer to the core instance
     * \return Value representing the pointer to the core instance
     */
    void* read_core_ptr(void);
    /**
     * \brief Set the value of the pointer to the core instance
     * \param value Value representing the pointer to the core instance
     */
    void write_core_ptr(void* value);
    
    // immediate watchpoint operations
    /**
     * \brief Queries the value of the check watchpoint hit flag
     * \return Value representing the check watchpoint hit flag
     */
    unsigned char read_check_watchpoint_hit(void);
    /**
     * \brief Set the value of the check watchpoint hit flag
     * \param value Value representing the check watchpoint hit flag
     */
    void write_check_watchpoint_hit(unsigned char value);
    /**
     * \brief Queries the value of the watchpoint hit flag
     * \return Value representing the watchpoint hit flag
     */
    unsigned char read_watchpoint_hit(void);
    /**
     * \brief Set the value of the watchpoint hit flag
     * \param value Value representing the watchpoint hit flag
     */
    void write_watchpoint_hit(unsigned char value);
    
    // immediate register and register field read operations
    /**
     * \brief Queries the value of the rh850g4 translation time PC register
     * \return Value of the rh850g4 translation time PC register
     */
    virtual unsigned int read_trans_PC(void);
    /**
     * \brief Queries the value of the rh850g4 r0 register
     * \return Value of the rh850g4 r0 register
     */
    virtual unsigned int read_r0(void);
    /**
     * \brief Queries the value of the rh850g4 r1 register
     * \return Value of the rh850g4 r1 register
     */
    virtual unsigned int read_r1(void);
    /**
     * \brief Queries the value of the rh850g4 r2 register
     * \return Value of the rh850g4 r2 register
     */
    virtual unsigned int read_r2(void);
    /**
     * \brief Queries the value of the rh850g4 r3 register
     * \return Value of the rh850g4 r3 register
     */
    virtual unsigned int read_r3(void);
    /**
     * \brief Queries the value of the rh850g4 r4 register
     * \return Value of the rh850g4 r4 register
     */
    virtual unsigned int read_r4(void);
    /**
     * \brief Queries the value of the rh850g4 r5 register
     * \return Value of the rh850g4 r5 register
     */
    virtual unsigned int read_r5(void);
    /**
     * \brief Queries the value of the rh850g4 r6 register
     * \return Value of the rh850g4 r6 register
     */
    virtual unsigned int read_r6(void);
    /**
     * \brief Queries the value of the rh850g4 r7 register
     * \return Value of the rh850g4 r7 register
     */
    virtual unsigned int read_r7(void);
    /**
     * \brief Queries the value of the rh850g4 r8 register
     * \return Value of the rh850g4 r8 register
     */
    virtual unsigned int read_r8(void);
    /**
     * \brief Queries the value of the rh850g4 r9 register
     * \return Value of the rh850g4 r9 register
     */
    virtual unsigned int read_r9(void);
    /**
     * \brief Queries the value of the rh850g4 r10 register
     * \return Value of the rh850g4 r10 register
     */
    virtual unsigned int read_r10(void);
    /**
     * \brief Queries the value of the rh850g4 r11 register
     * \return Value of the rh850g4 r11 register
     */
    virtual unsigned int read_r11(void);
    /**
     * \brief Queries the value of the rh850g4 r12 register
     * \return Value of the rh850g4 r12 register
     */
    virtual unsigned int read_r12(void);
    /**
     * \brief Queries the value of the rh850g4 r13 register
     * \return Value of the rh850g4 r13 register
     */
    virtual unsigned int read_r13(void);
    /**
     * \brief Queries the value of the rh850g4 r14 register
     * \return Value of the rh850g4 r14 register
     */
    virtual unsigned int read_r14(void);
    /**
     * \brief Queries the value of the rh850g4 r15 register
     * \return Value of the rh850g4 r15 register
     */
    virtual unsigned int read_r15(void);
    /**
     * \brief Queries the value of the rh850g4 r16 register
     * \return Value of the rh850g4 r16 register
     */
    virtual unsigned int read_r16(void);
    /**
     * \brief Queries the value of the rh850g4 r17 register
     * \return Value of the rh850g4 r17 register
     */
    virtual unsigned int read_r17(void);
    /**
     * \brief Queries the value of the rh850g4 r18 register
     * \return Value of the rh850g4 r18 register
     */
    virtual unsigned int read_r18(void);
    /**
     * \brief Queries the value of the rh850g4 r19 register
     * \return Value of the rh850g4 r19 register
     */
    virtual unsigned int read_r19(void);
    /**
     * \brief Queries the value of the rh850g4 r20 register
     * \return Value of the rh850g4 r20 register
     */
    virtual unsigned int read_r20(void);
    /**
     * \brief Queries the value of the rh850g4 r21 register
     * \return Value of the rh850g4 r21 register
     */
    virtual unsigned int read_r21(void);
    /**
     * \brief Queries the value of the rh850g4 r22 register
     * \return Value of the rh850g4 r22 register
     */
    virtual unsigned int read_r22(void);
    /**
     * \brief Queries the value of the rh850g4 r23 register
     * \return Value of the rh850g4 r23 register
     */
    virtual unsigned int read_r23(void);
    /**
     * \brief Queries the value of the rh850g4 r24 register
     * \return Value of the rh850g4 r24 register
     */
    virtual unsigned int read_r24(void);
    /**
     * \brief Queries the value of the rh850g4 r25 register
     * \return Value of the rh850g4 r25 register
     */
    virtual unsigned int read_r25(void);
    /**
     * \brief Queries the value of the rh850g4 r26 register
     * \return Value of the rh850g4 r26 register
     */
    virtual unsigned int read_r26(void);
    /**
     * \brief Queries the value of the rh850g4 r27 register
     * \return Value of the rh850g4 r27 register
     */
    virtual unsigned int read_r27(void);
    /**
     * \brief Queries the value of the rh850g4 r28 register
     * \return Value of the rh850g4 r28 register
     */
    virtual unsigned int read_r28(void);
    /**
     * \brief Queries the value of the rh850g4 r29 register
     * \return Value of the rh850g4 r29 register
     */
    virtual unsigned int read_r29(void);
    /**
     * \brief Queries the value of the rh850g4 r30 register
     * \return Value of the rh850g4 r30 register
     */
    virtual unsigned int read_r30(void);
    /**
     * \brief Queries the value of the rh850g4 r31 register
     * \return Value of the rh850g4 r31 register
     */
    virtual unsigned int read_r31(void);
    /**
     * \brief Queries the value of the rh850g4 PC register
     * \return Value of the rh850g4 PC register
     */
    virtual unsigned int read_PC(void);
    /**
     * \brief Queries the value of the rh850g4 wr0 register
     * \return Value of the rh850g4 wr0 register
     */
    virtual unsigned long long read_wr0(void);
    /**
     * \brief Queries the value of the rh850g4 wr0_u register
     * \return Value of the rh850g4 wr0_u register
     */
    virtual unsigned long long read_wr0_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr1 register
     * \return Value of the rh850g4 wr1 register
     */
    virtual unsigned long long read_wr1(void);
    /**
     * \brief Queries the value of the rh850g4 wr1_u register
     * \return Value of the rh850g4 wr1_u register
     */
    virtual unsigned long long read_wr1_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr2 register
     * \return Value of the rh850g4 wr2 register
     */
    virtual unsigned long long read_wr2(void);
    /**
     * \brief Queries the value of the rh850g4 wr2_u register
     * \return Value of the rh850g4 wr2_u register
     */
    virtual unsigned long long read_wr2_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr3 register
     * \return Value of the rh850g4 wr3 register
     */
    virtual unsigned long long read_wr3(void);
    /**
     * \brief Queries the value of the rh850g4 wr3_u register
     * \return Value of the rh850g4 wr3_u register
     */
    virtual unsigned long long read_wr3_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr4 register
     * \return Value of the rh850g4 wr4 register
     */
    virtual unsigned long long read_wr4(void);
    /**
     * \brief Queries the value of the rh850g4 wr4_u register
     * \return Value of the rh850g4 wr4_u register
     */
    virtual unsigned long long read_wr4_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr5 register
     * \return Value of the rh850g4 wr5 register
     */
    virtual unsigned long long read_wr5(void);
    /**
     * \brief Queries the value of the rh850g4 wr5_u register
     * \return Value of the rh850g4 wr5_u register
     */
    virtual unsigned long long read_wr5_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr6 register
     * \return Value of the rh850g4 wr6 register
     */
    virtual unsigned long long read_wr6(void);
    /**
     * \brief Queries the value of the rh850g4 wr6_u register
     * \return Value of the rh850g4 wr6_u register
     */
    virtual unsigned long long read_wr6_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr7 register
     * \return Value of the rh850g4 wr7 register
     */
    virtual unsigned long long read_wr7(void);
    /**
     * \brief Queries the value of the rh850g4 wr7_u register
     * \return Value of the rh850g4 wr7_u register
     */
    virtual unsigned long long read_wr7_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr8 register
     * \return Value of the rh850g4 wr8 register
     */
    virtual unsigned long long read_wr8(void);
    /**
     * \brief Queries the value of the rh850g4 wr8_u register
     * \return Value of the rh850g4 wr8_u register
     */
    virtual unsigned long long read_wr8_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr9 register
     * \return Value of the rh850g4 wr9 register
     */
    virtual unsigned long long read_wr9(void);
    /**
     * \brief Queries the value of the rh850g4 wr9_u register
     * \return Value of the rh850g4 wr9_u register
     */
    virtual unsigned long long read_wr9_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr10 register
     * \return Value of the rh850g4 wr10 register
     */
    virtual unsigned long long read_wr10(void);
    /**
     * \brief Queries the value of the rh850g4 wr10_u register
     * \return Value of the rh850g4 wr10_u register
     */
    virtual unsigned long long read_wr10_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr11 register
     * \return Value of the rh850g4 wr11 register
     */
    virtual unsigned long long read_wr11(void);
    /**
     * \brief Queries the value of the rh850g4 wr11_u register
     * \return Value of the rh850g4 wr11_u register
     */
    virtual unsigned long long read_wr11_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr12 register
     * \return Value of the rh850g4 wr12 register
     */
    virtual unsigned long long read_wr12(void);
    /**
     * \brief Queries the value of the rh850g4 wr12_u register
     * \return Value of the rh850g4 wr12_u register
     */
    virtual unsigned long long read_wr12_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr13 register
     * \return Value of the rh850g4 wr13 register
     */
    virtual unsigned long long read_wr13(void);
    /**
     * \brief Queries the value of the rh850g4 wr13_u register
     * \return Value of the rh850g4 wr13_u register
     */
    virtual unsigned long long read_wr13_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr14 register
     * \return Value of the rh850g4 wr14 register
     */
    virtual unsigned long long read_wr14(void);
    /**
     * \brief Queries the value of the rh850g4 wr14_u register
     * \return Value of the rh850g4 wr14_u register
     */
    virtual unsigned long long read_wr14_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr15 register
     * \return Value of the rh850g4 wr15 register
     */
    virtual unsigned long long read_wr15(void);
    /**
     * \brief Queries the value of the rh850g4 wr15_u register
     * \return Value of the rh850g4 wr15_u register
     */
    virtual unsigned long long read_wr15_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr16 register
     * \return Value of the rh850g4 wr16 register
     */
    virtual unsigned long long read_wr16(void);
    /**
     * \brief Queries the value of the rh850g4 wr16_u register
     * \return Value of the rh850g4 wr16_u register
     */
    virtual unsigned long long read_wr16_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr17 register
     * \return Value of the rh850g4 wr17 register
     */
    virtual unsigned long long read_wr17(void);
    /**
     * \brief Queries the value of the rh850g4 wr17_u register
     * \return Value of the rh850g4 wr17_u register
     */
    virtual unsigned long long read_wr17_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr18 register
     * \return Value of the rh850g4 wr18 register
     */
    virtual unsigned long long read_wr18(void);
    /**
     * \brief Queries the value of the rh850g4 wr18_u register
     * \return Value of the rh850g4 wr18_u register
     */
    virtual unsigned long long read_wr18_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr19 register
     * \return Value of the rh850g4 wr19 register
     */
    virtual unsigned long long read_wr19(void);
    /**
     * \brief Queries the value of the rh850g4 wr19_u register
     * \return Value of the rh850g4 wr19_u register
     */
    virtual unsigned long long read_wr19_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr20 register
     * \return Value of the rh850g4 wr20 register
     */
    virtual unsigned long long read_wr20(void);
    /**
     * \brief Queries the value of the rh850g4 wr20_u register
     * \return Value of the rh850g4 wr20_u register
     */
    virtual unsigned long long read_wr20_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr21 register
     * \return Value of the rh850g4 wr21 register
     */
    virtual unsigned long long read_wr21(void);
    /**
     * \brief Queries the value of the rh850g4 wr21_u register
     * \return Value of the rh850g4 wr21_u register
     */
    virtual unsigned long long read_wr21_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr22 register
     * \return Value of the rh850g4 wr22 register
     */
    virtual unsigned long long read_wr22(void);
    /**
     * \brief Queries the value of the rh850g4 wr22_u register
     * \return Value of the rh850g4 wr22_u register
     */
    virtual unsigned long long read_wr22_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr23 register
     * \return Value of the rh850g4 wr23 register
     */
    virtual unsigned long long read_wr23(void);
    /**
     * \brief Queries the value of the rh850g4 wr23_u register
     * \return Value of the rh850g4 wr23_u register
     */
    virtual unsigned long long read_wr23_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr24 register
     * \return Value of the rh850g4 wr24 register
     */
    virtual unsigned long long read_wr24(void);
    /**
     * \brief Queries the value of the rh850g4 wr24_u register
     * \return Value of the rh850g4 wr24_u register
     */
    virtual unsigned long long read_wr24_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr25 register
     * \return Value of the rh850g4 wr25 register
     */
    virtual unsigned long long read_wr25(void);
    /**
     * \brief Queries the value of the rh850g4 wr25_u register
     * \return Value of the rh850g4 wr25_u register
     */
    virtual unsigned long long read_wr25_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr26 register
     * \return Value of the rh850g4 wr26 register
     */
    virtual unsigned long long read_wr26(void);
    /**
     * \brief Queries the value of the rh850g4 wr26_u register
     * \return Value of the rh850g4 wr26_u register
     */
    virtual unsigned long long read_wr26_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr27 register
     * \return Value of the rh850g4 wr27 register
     */
    virtual unsigned long long read_wr27(void);
    /**
     * \brief Queries the value of the rh850g4 wr27_u register
     * \return Value of the rh850g4 wr27_u register
     */
    virtual unsigned long long read_wr27_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr28 register
     * \return Value of the rh850g4 wr28 register
     */
    virtual unsigned long long read_wr28(void);
    /**
     * \brief Queries the value of the rh850g4 wr28_u register
     * \return Value of the rh850g4 wr28_u register
     */
    virtual unsigned long long read_wr28_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr29 register
     * \return Value of the rh850g4 wr29 register
     */
    virtual unsigned long long read_wr29(void);
    /**
     * \brief Queries the value of the rh850g4 wr29_u register
     * \return Value of the rh850g4 wr29_u register
     */
    virtual unsigned long long read_wr29_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr30 register
     * \return Value of the rh850g4 wr30 register
     */
    virtual unsigned long long read_wr30(void);
    /**
     * \brief Queries the value of the rh850g4 wr30_u register
     * \return Value of the rh850g4 wr30_u register
     */
    virtual unsigned long long read_wr30_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr31 register
     * \return Value of the rh850g4 wr31 register
     */
    virtual unsigned long long read_wr31(void);
    /**
     * \brief Queries the value of the rh850g4 wr31_u register
     * \return Value of the rh850g4 wr31_u register
     */
    virtual unsigned long long read_wr31_u(void);
    /**
     * \brief Queries the value of the rh850g4 EIPC register
     * \return Value of the rh850g4 EIPC register
     */
    virtual unsigned int read_EIPC(void);
    /**
     * \brief Queries the value of the rh850g4 EIPSW register
     * \return Value of the rh850g4 EIPSW register
     */
    virtual unsigned int read_EIPSW(void);
    /**
     * \brief Queries the value of the rh850g4 FEPC register
     * \return Value of the rh850g4 FEPC register
     */
    virtual unsigned int read_FEPC(void);
    /**
     * \brief Queries the value of the rh850g4 FEPSW register
     * \return Value of the rh850g4 FEPSW register
     */
    virtual unsigned int read_FEPSW(void);
    /**
     * \brief Queries the value of the rh850g4 PSW register
     * \return Value of the rh850g4 PSW register
     */
    virtual unsigned int read_PSW(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_Z register field
     * \return Value of the rh850g4 PSW_Z register field
     */
    virtual unsigned char read_PSW_Z(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_S register field
     * \return Value of the rh850g4 PSW_S register field
     */
    virtual unsigned char read_PSW_S(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_OV register field
     * \return Value of the rh850g4 PSW_OV register field
     */
    virtual unsigned char read_PSW_OV(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_CY register field
     * \return Value of the rh850g4 PSW_CY register field
     */
    virtual unsigned char read_PSW_CY(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_SAT register field
     * \return Value of the rh850g4 PSW_SAT register field
     */
    virtual unsigned char read_PSW_SAT(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_ID register field
     * \return Value of the rh850g4 PSW_ID register field
     */
    virtual unsigned char read_PSW_ID(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_EP register field
     * \return Value of the rh850g4 PSW_EP register field
     */
    virtual unsigned char read_PSW_EP(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_NP register field
     * \return Value of the rh850g4 PSW_NP register field
     */
    virtual unsigned char read_PSW_NP(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_EBV register field
     * \return Value of the rh850g4 PSW_EBV register field
     */
    virtual unsigned char read_PSW_EBV(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_CU0 register field
     * \return Value of the rh850g4 PSW_CU0 register field
     */
    virtual unsigned char read_PSW_CU0(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_CU1 register field
     * \return Value of the rh850g4 PSW_CU1 register field
     */
    virtual unsigned char read_PSW_CU1(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_CU2 register field
     * \return Value of the rh850g4 PSW_CU2 register field
     */
    virtual unsigned char read_PSW_CU2(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_EIMASK register field
     * \return Value of the rh850g4 PSW_EIMASK register field
     */
    virtual unsigned char read_PSW_EIMASK(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_UM register field
     * \return Value of the rh850g4 PSW_UM register field
     */
    virtual unsigned char read_PSW_UM(void);
    /**
     * \brief Queries the value of the rh850g4 FPSR register
     * \return Value of the rh850g4 FPSR register
     */
    virtual unsigned int read_FPSR(void);
    /**
     * \brief Queries the value of the rh850g4 FPSR_CC register field
     * \return Value of the rh850g4 FPSR_CC register field
     */
    virtual unsigned char read_FPSR_CC(void);
    /**
     * \brief Queries the value of the rh850g4 FPSR_FN register field
     * \return Value of the rh850g4 FPSR_FN register field
     */
    virtual unsigned char read_FPSR_FN(void);
    /**
     * \brief Queries the value of the rh850g4 FPSR_IF register field
     * \return Value of the rh850g4 FPSR_IF register field
     */
    virtual unsigned char read_FPSR_IF(void);
    /**
     * \brief Queries the value of the rh850g4 FPSR_PEM register field
     * \return Value of the rh850g4 FPSR_PEM register field
     */
    virtual unsigned char read_FPSR_PEM(void);
    /**
     * \brief Queries the value of the rh850g4 FPSR_RM register field
     * \return Value of the rh850g4 FPSR_RM register field
     */
    virtual unsigned char read_FPSR_RM(void);
    /**
     * \brief Queries the value of the rh850g4 FPSR_FS register field
     * \return Value of the rh850g4 FPSR_FS register field
     */
    virtual unsigned char read_FPSR_FS(void);
    /**
     * \brief Queries the value of the rh850g4 FPSR_XC register field
     * \return Value of the rh850g4 FPSR_XC register field
     */
    virtual unsigned char read_FPSR_XC(void);
    /**
     * \brief Queries the value of the rh850g4 FPSR_XE register field
     * \return Value of the rh850g4 FPSR_XE register field
     */
    virtual unsigned char read_FPSR_XE(void);
    /**
     * \brief Queries the value of the rh850g4 FPSR_XP register field
     * \return Value of the rh850g4 FPSR_XP register field
     */
    virtual unsigned char read_FPSR_XP(void);
    /**
     * \brief Queries the value of the rh850g4 FPEPC register
     * \return Value of the rh850g4 FPEPC register
     */
    virtual unsigned int read_FPEPC(void);
    /**
     * \brief Queries the value of the rh850g4 FPST register
     * \return Value of the rh850g4 FPST register
     */
    virtual unsigned int read_FPST(void);
    /**
     * \brief Queries the value of the rh850g4 FPST_XC register field
     * \return Value of the rh850g4 FPST_XC register field
     */
    virtual unsigned char read_FPST_XC(void);
    /**
     * \brief Queries the value of the rh850g4 FPST_IF register field
     * \return Value of the rh850g4 FPST_IF register field
     */
    virtual unsigned char read_FPST_IF(void);
    /**
     * \brief Queries the value of the rh850g4 FPST_XP register field
     * \return Value of the rh850g4 FPST_XP register field
     */
    virtual unsigned char read_FPST_XP(void);
    /**
     * \brief Queries the value of the rh850g4 FPCC register
     * \return Value of the rh850g4 FPCC register
     */
    virtual unsigned int read_FPCC(void);
    /**
     * \brief Queries the value of the rh850g4 FPCC_CC register field
     * \return Value of the rh850g4 FPCC_CC register field
     */
    virtual unsigned char read_FPCC_CC(void);
    /**
     * \brief Queries the value of the rh850g4 FPCFG register
     * \return Value of the rh850g4 FPCFG register
     */
    virtual unsigned int read_FPCFG(void);
    /**
     * \brief Queries the value of the rh850g4 FPCFG_RM register field
     * \return Value of the rh850g4 FPCFG_RM register field
     */
    virtual unsigned char read_FPCFG_RM(void);
    /**
     * \brief Queries the value of the rh850g4 FPCFG_XE register field
     * \return Value of the rh850g4 FPCFG_XE register field
     */
    virtual unsigned char read_FPCFG_XE(void);
    /**
     * \brief Queries the value of the rh850g4 EIIC register
     * \return Value of the rh850g4 EIIC register
     */
    virtual unsigned int read_EIIC(void);
    /**
     * \brief Queries the value of the rh850g4 FEIC register
     * \return Value of the rh850g4 FEIC register
     */
    virtual unsigned int read_FEIC(void);
    /**
     * \brief Queries the value of the rh850g4 PSWH register
     * \return Value of the rh850g4 PSWH register
     */
    virtual unsigned int read_PSWH(void);
    /**
     * \brief Queries the value of the rh850g4 PSWH_GPID register field
     * \return Value of the rh850g4 PSWH_GPID register field
     */
    virtual unsigned char read_PSWH_GPID(void);
    /**
     * \brief Queries the value of the rh850g4 PSWH_GM register field
     * \return Value of the rh850g4 PSWH_GM register field
     */
    virtual unsigned char read_PSWH_GM(void);
    /**
     * \brief Queries the value of the rh850g4 CTPC register
     * \return Value of the rh850g4 CTPC register
     */
    virtual unsigned int read_CTPC(void);
    /**
     * \brief Queries the value of the rh850g4 CTPSW register
     * \return Value of the rh850g4 CTPSW register
     */
    virtual unsigned int read_CTPSW(void);
    /**
     * \brief Queries the value of the rh850g4 EIPSWH register
     * \return Value of the rh850g4 EIPSWH register
     */
    virtual unsigned int read_EIPSWH(void);
    /**
     * \brief Queries the value of the rh850g4 EIPSWH_GPID register field
     * \return Value of the rh850g4 EIPSWH_GPID register field
     */
    virtual unsigned char read_EIPSWH_GPID(void);
    /**
     * \brief Queries the value of the rh850g4 EIPSWH_GM register field
     * \return Value of the rh850g4 EIPSWH_GM register field
     */
    virtual unsigned char read_EIPSWH_GM(void);
    /**
     * \brief Queries the value of the rh850g4 FEPSWH register
     * \return Value of the rh850g4 FEPSWH register
     */
    virtual unsigned int read_FEPSWH(void);
    /**
     * \brief Queries the value of the rh850g4 FEPSWH_GPID register field
     * \return Value of the rh850g4 FEPSWH_GPID register field
     */
    virtual unsigned char read_FEPSWH_GPID(void);
    /**
     * \brief Queries the value of the rh850g4 FEPSWH_GM register field
     * \return Value of the rh850g4 FEPSWH_GM register field
     */
    virtual unsigned char read_FEPSWH_GM(void);
    /**
     * \brief Queries the value of the rh850g4 CTBP register
     * \return Value of the rh850g4 CTBP register
     */
    virtual unsigned int read_CTBP(void);
    /**
     * \brief Queries the value of the rh850g4 SNZCFG register
     * \return Value of the rh850g4 SNZCFG register
     */
    virtual unsigned int read_SNZCFG(void);
    /**
     * \brief Queries the value of the rh850g4 EIWR register
     * \return Value of the rh850g4 EIWR register
     */
    virtual unsigned int read_EIWR(void);
    /**
     * \brief Queries the value of the rh850g4 FEWR register
     * \return Value of the rh850g4 FEWR register
     */
    virtual unsigned int read_FEWR(void);
    /**
     * \brief Queries the value of the rh850g4 SPID register
     * \return Value of the rh850g4 SPID register
     */
    virtual unsigned int read_SPID(void);
    /**
     * \brief Queries the value of the rh850g4 SPIDLIST register
     * \return Value of the rh850g4 SPIDLIST register
     */
    virtual unsigned int read_SPIDLIST(void);
    /**
     * \brief Queries the value of the rh850g4 RBASE register
     * \return Value of the rh850g4 RBASE register
     */
    virtual unsigned int read_RBASE(void);
    /**
     * \brief Queries the value of the rh850g4 RBASE_RBASE register field
     * \return Value of the rh850g4 RBASE_RBASE register field
     */
    virtual unsigned int read_RBASE_RBASE(void);
    /**
     * \brief Queries the value of the rh850g4 RBASE_DV register field
     * \return Value of the rh850g4 RBASE_DV register field
     */
    virtual unsigned char read_RBASE_DV(void);
    /**
     * \brief Queries the value of the rh850g4 RBASE_RINT register field
     * \return Value of the rh850g4 RBASE_RINT register field
     */
    virtual unsigned char read_RBASE_RINT(void);
    /**
     * \brief Queries the value of the rh850g4 EBASE register
     * \return Value of the rh850g4 EBASE register
     */
    virtual unsigned int read_EBASE(void);
    /**
     * \brief Queries the value of the rh850g4 EBASE_EBASE register field
     * \return Value of the rh850g4 EBASE_EBASE register field
     */
    virtual unsigned int read_EBASE_EBASE(void);
    /**
     * \brief Queries the value of the rh850g4 EBASE_DV register field
     * \return Value of the rh850g4 EBASE_DV register field
     */
    virtual unsigned char read_EBASE_DV(void);
    /**
     * \brief Queries the value of the rh850g4 EBASE_RINT register field
     * \return Value of the rh850g4 EBASE_RINT register field
     */
    virtual unsigned char read_EBASE_RINT(void);
    /**
     * \brief Queries the value of the rh850g4 INTBP register
     * \return Value of the rh850g4 INTBP register
     */
    virtual unsigned int read_INTBP(void);
    /**
     * \brief Queries the value of the rh850g4 MCTL register
     * \return Value of the rh850g4 MCTL register
     */
    virtual unsigned int read_MCTL(void);
    /**
     * \brief Queries the value of the rh850g4 MCTL_UIC register field
     * \return Value of the rh850g4 MCTL_UIC register field
     */
    virtual unsigned char read_MCTL_UIC(void);
    /**
     * \brief Queries the value of the rh850g4 PID register
     * \return Value of the rh850g4 PID register
     */
    virtual unsigned int read_PID(void);
    /**
     * \brief Queries the value of the rh850g4 SVLOCK register
     * \return Value of the rh850g4 SVLOCK register
     */
    virtual unsigned int read_SVLOCK(void);
    /**
     * \brief Queries the value of the rh850g4 SCCFG register
     * \return Value of the rh850g4 SCCFG register
     */
    virtual unsigned int read_SCCFG(void);
    /**
     * \brief Queries the value of the rh850g4 SCCFG_SIZE register field
     * \return Value of the rh850g4 SCCFG_SIZE register field
     */
    virtual unsigned char read_SCCFG_SIZE(void);
    /**
     * \brief Queries the value of the rh850g4 SCBP register
     * \return Value of the rh850g4 SCBP register
     */
    virtual unsigned int read_SCBP(void);
    /**
     * \brief Queries the value of the rh850g4 HVCFG register
     * \return Value of the rh850g4 HVCFG register
     */
    virtual unsigned int read_HVCFG(void);
    /**
     * \brief Queries the value of the rh850g4 HVCFG_HVE register field
     * \return Value of the rh850g4 HVCFG_HVE register field
     */
    virtual unsigned char read_HVCFG_HVE(void);
    /**
     * \brief Queries the value of the rh850g4 GMCFG register
     * \return Value of the rh850g4 GMCFG register
     */
    virtual unsigned int read_GMCFG(void);
    /**
     * \brief Queries the value of the rh850g4 GMCFG_GMP register field
     * \return Value of the rh850g4 GMCFG_GMP register field
     */
    virtual unsigned char read_GMCFG_GMP(void);
    /**
     * \brief Queries the value of the rh850g4 GMCFG_HMP register field
     * \return Value of the rh850g4 GMCFG_HMP register field
     */
    virtual unsigned char read_GMCFG_HMP(void);
    /**
     * \brief Queries the value of the rh850g4 GMCFG_GSYSE register field
     * \return Value of the rh850g4 GMCFG_GSYSE register field
     */
    virtual unsigned char read_GMCFG_GSYSE(void);
    /**
     * \brief Queries the value of the rh850g4 GMCFG_GCU0 register field
     * \return Value of the rh850g4 GMCFG_GCU0 register field
     */
    virtual unsigned char read_GMCFG_GCU0(void);
    /**
     * \brief Queries the value of the rh850g4 GMCFG_GCU1 register field
     * \return Value of the rh850g4 GMCFG_GCU1 register field
     */
    virtual unsigned char read_GMCFG_GCU1(void);
    /**
     * \brief Queries the value of the rh850g4 GMCFG_GCU2 register field
     * \return Value of the rh850g4 GMCFG_GCU2 register field
     */
    virtual unsigned char read_GMCFG_GCU2(void);
    /**
     * \brief Queries the value of the rh850g4 HVSB register
     * \return Value of the rh850g4 HVSB register
     */
    virtual unsigned int read_HVSB(void);
    /**
     * \brief Queries the value of the rh850g4 PEID register
     * \return Value of the rh850g4 PEID register
     */
    virtual unsigned int read_PEID(void);
    /**
     * \brief Queries the value of the rh850g4 BMID register
     * \return Value of the rh850g4 BMID register
     */
    virtual unsigned int read_BMID(void);
    /**
     * \brief Queries the value of the rh850g4 BMID_BMID register field
     * \return Value of the rh850g4 BMID_BMID register field
     */
    virtual unsigned char read_BMID_BMID(void);
    /**
     * \brief Queries the value of the rh850g4 MEA register
     * \return Value of the rh850g4 MEA register
     */
    virtual unsigned int read_MEA(void);
    /**
     * \brief Queries the value of the rh850g4 MEI register
     * \return Value of the rh850g4 MEI register
     */
    virtual unsigned int read_MEI(void);
    /**
     * \brief Queries the value of the rh850g4 RBCR0 register
     * \return Value of the rh850g4 RBCR0 register
     */
    virtual unsigned int read_RBCR0(void);
    /**
     * \brief Queries the value of the rh850g4 RBCR0_MD register field
     * \return Value of the rh850g4 RBCR0_MD register field
     */
    virtual unsigned char read_RBCR0_MD(void);
    /**
     * \brief Queries the value of the rh850g4 RBCR0_BE register field
     * \return Value of the rh850g4 RBCR0_BE register field
     */
    virtual unsigned short read_RBCR0_BE(void);
    /**
     * \brief Queries the value of the rh850g4 RBCR1 register
     * \return Value of the rh850g4 RBCR1 register
     */
    virtual unsigned int read_RBCR1(void);
    /**
     * \brief Queries the value of the rh850g4 RBCR1_NC register field
     * \return Value of the rh850g4 RBCR1_NC register field
     */
    virtual unsigned short read_RBCR1_NC(void);
    /**
     * \brief Queries the value of the rh850g4 RBNR register
     * \return Value of the rh850g4 RBNR register
     */
    virtual unsigned int read_RBNR(void);
    /**
     * \brief Queries the value of the rh850g4 RBNR_BN register field
     * \return Value of the rh850g4 RBNR_BN register field
     */
    virtual unsigned char read_RBNR_BN(void);
    /**
     * \brief Queries the value of the rh850g4 RBIP register
     * \return Value of the rh850g4 RBIP register
     */
    virtual unsigned int read_RBIP(void);
    /**
     * \brief Queries the value of the rh850g4 RBIP_RBIP register field
     * \return Value of the rh850g4 RBIP_RBIP register field
     */
    virtual unsigned int read_RBIP_RBIP(void);
    /**
     * \brief Queries the value of the rh850g4 ISPR register
     * \return Value of the rh850g4 ISPR register
     */
    virtual unsigned int read_ISPR(void);
    /**
     * \brief Queries the value of the rh850g4 ISPR_ISP register field
     * \return Value of the rh850g4 ISPR_ISP register field
     */
    virtual unsigned short read_ISPR_ISP(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR register
     * \return Value of the rh850g4 IMSR register
     */
    virtual unsigned int read_IMSR(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR_EEIM register field
     * \return Value of the rh850g4 IMSR_EEIM register field
     */
    virtual unsigned char read_IMSR_EEIM(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR_EPLM register field
     * \return Value of the rh850g4 IMSR_EPLM register field
     */
    virtual unsigned char read_IMSR_EPLM(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR_EID register field
     * \return Value of the rh850g4 IMSR_EID register field
     */
    virtual unsigned char read_IMSR_EID(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR_ENP register field
     * \return Value of the rh850g4 IMSR_ENP register field
     */
    virtual unsigned char read_IMSR_ENP(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR_FNP register field
     * \return Value of the rh850g4 IMSR_FNP register field
     */
    virtual unsigned char read_IMSR_FNP(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR_HEEIM register field
     * \return Value of the rh850g4 IMSR_HEEIM register field
     */
    virtual unsigned char read_IMSR_HEEIM(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR_HEPLM register field
     * \return Value of the rh850g4 IMSR_HEPLM register field
     */
    virtual unsigned char read_IMSR_HEPLM(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR_HEID register field
     * \return Value of the rh850g4 IMSR_HEID register field
     */
    virtual unsigned char read_IMSR_HEID(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR_HENP register field
     * \return Value of the rh850g4 IMSR_HENP register field
     */
    virtual unsigned char read_IMSR_HENP(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR_HFNP register field
     * \return Value of the rh850g4 IMSR_HFNP register field
     */
    virtual unsigned char read_IMSR_HFNP(void);
    /**
     * \brief Queries the value of the rh850g4 ICSR register
     * \return Value of the rh850g4 ICSR register
     */
    virtual unsigned int read_ICSR(void);
    /**
     * \brief Queries the value of the rh850g4 ICSR_PMEI register field
     * \return Value of the rh850g4 ICSR_PMEI register field
     */
    virtual unsigned char read_ICSR_PMEI(void);
    /**
     * \brief Queries the value of the rh850g4 INTCFG register
     * \return Value of the rh850g4 INTCFG register
     */
    virtual unsigned int read_INTCFG(void);
    /**
     * \brief Queries the value of the rh850g4 INTCFG_ISPC register field
     * \return Value of the rh850g4 INTCFG_ISPC register field
     */
    virtual unsigned char read_INTCFG_ISPC(void);
    /**
     * \brief Queries the value of the rh850g4 INTCFG_EPL register field
     * \return Value of the rh850g4 INTCFG_EPL register field
     */
    virtual unsigned char read_INTCFG_EPL(void);
    /**
     * \brief Queries the value of the rh850g4 INTCFG_ULNR register field
     * \return Value of the rh850g4 INTCFG_ULNR register field
     */
    virtual unsigned char read_INTCFG_ULNR(void);
    /**
     * \brief Queries the value of the rh850g4 PLMR register
     * \return Value of the rh850g4 PLMR register
     */
    virtual unsigned int read_PLMR(void);
    /**
     * \brief Queries the value of the rh850g4 PLMR_PLM register field
     * \return Value of the rh850g4 PLMR_PLM register field
     */
    virtual unsigned char read_PLMR_PLM(void);
    /**
     * \brief Queries the value of the rh850g4 FXSR register
     * \return Value of the rh850g4 FXSR register
     */
    virtual unsigned int read_FXSR(void);
    /**
     * \brief Queries the value of the rh850g4 FXSR_FN register field
     * \return Value of the rh850g4 FXSR_FN register field
     */
    virtual unsigned char read_FXSR_FN(void);
    /**
     * \brief Queries the value of the rh850g4 FXSR_IF register field
     * \return Value of the rh850g4 FXSR_IF register field
     */
    virtual unsigned char read_FXSR_IF(void);
    /**
     * \brief Queries the value of the rh850g4 FXSR_PEM register field
     * \return Value of the rh850g4 FXSR_PEM register field
     */
    virtual unsigned char read_FXSR_PEM(void);
    /**
     * \brief Queries the value of the rh850g4 FXSR_RM register field
     * \return Value of the rh850g4 FXSR_RM register field
     */
    virtual unsigned char read_FXSR_RM(void);
    /**
     * \brief Queries the value of the rh850g4 FXSR_FS register field
     * \return Value of the rh850g4 FXSR_FS register field
     */
    virtual unsigned char read_FXSR_FS(void);
    /**
     * \brief Queries the value of the rh850g4 FXSR_XC register field
     * \return Value of the rh850g4 FXSR_XC register field
     */
    virtual unsigned char read_FXSR_XC(void);
    /**
     * \brief Queries the value of the rh850g4 FXSR_XE register field
     * \return Value of the rh850g4 FXSR_XE register field
     */
    virtual unsigned char read_FXSR_XE(void);
    /**
     * \brief Queries the value of the rh850g4 FXSR_XP register field
     * \return Value of the rh850g4 FXSR_XP register field
     */
    virtual unsigned char read_FXSR_XP(void);
    /**
     * \brief Queries the value of the rh850g4 FXST register
     * \return Value of the rh850g4 FXST register
     */
    virtual unsigned int read_FXST(void);
    /**
     * \brief Queries the value of the rh850g4 FXST_XC register field
     * \return Value of the rh850g4 FXST_XC register field
     */
    virtual unsigned char read_FXST_XC(void);
    /**
     * \brief Queries the value of the rh850g4 FXST_IF register field
     * \return Value of the rh850g4 FXST_IF register field
     */
    virtual unsigned char read_FXST_IF(void);
    /**
     * \brief Queries the value of the rh850g4 FXST_XP register field
     * \return Value of the rh850g4 FXST_XP register field
     */
    virtual unsigned char read_FXST_XP(void);
    /**
     * \brief Queries the value of the rh850g4 FXINFO register
     * \return Value of the rh850g4 FXINFO register
     */
    virtual unsigned int read_FXINFO(void);
    /**
     * \brief Queries the value of the rh850g4 FXINFO_NFPU register field
     * \return Value of the rh850g4 FXINFO_NFPU register field
     */
    virtual unsigned char read_FXINFO_NFPU(void);
    /**
     * \brief Queries the value of the rh850g4 FXINFO_RSIZE register field
     * \return Value of the rh850g4 FXINFO_RSIZE register field
     */
    virtual unsigned char read_FXINFO_RSIZE(void);
    /**
     * \brief Queries the value of the rh850g4 FXCFG register
     * \return Value of the rh850g4 FXCFG register
     */
    virtual unsigned int read_FXCFG(void);
    /**
     * \brief Queries the value of the rh850g4 FXCFG_RM register field
     * \return Value of the rh850g4 FXCFG_RM register field
     */
    virtual unsigned char read_FXCFG_RM(void);
    /**
     * \brief Queries the value of the rh850g4 FXCFG_XE register field
     * \return Value of the rh850g4 FXCFG_XE register field
     */
    virtual unsigned char read_FXCFG_XE(void);
    /**
     * \brief Queries the value of the rh850g4 FXXC register
     * \return Value of the rh850g4 FXXC register
     */
    virtual unsigned int read_FXXC(void);
    /**
     * \brief Queries the value of the rh850g4 FXXC_XC3 register field
     * \return Value of the rh850g4 FXXC_XC3 register field
     */
    virtual unsigned char read_FXXC_XC3(void);
    /**
     * \brief Queries the value of the rh850g4 FXXC_XC2 register field
     * \return Value of the rh850g4 FXXC_XC2 register field
     */
    virtual unsigned char read_FXXC_XC2(void);
    /**
     * \brief Queries the value of the rh850g4 FXXC_XC1 register field
     * \return Value of the rh850g4 FXXC_XC1 register field
     */
    virtual unsigned char read_FXXC_XC1(void);
    /**
     * \brief Queries the value of the rh850g4 FXXC_XC0 register field
     * \return Value of the rh850g4 FXXC_XC0 register field
     */
    virtual unsigned char read_FXXC_XC0(void);
    /**
     * \brief Queries the value of the rh850g4 FXXP register
     * \return Value of the rh850g4 FXXP register
     */
    virtual unsigned int read_FXXP(void);
    /**
     * \brief Queries the value of the rh850g4 FXXP_XP3 register field
     * \return Value of the rh850g4 FXXP_XP3 register field
     */
    virtual unsigned char read_FXXP_XP3(void);
    /**
     * \brief Queries the value of the rh850g4 FXXP_XP2 register field
     * \return Value of the rh850g4 FXXP_XP2 register field
     */
    virtual unsigned char read_FXXP_XP2(void);
    /**
     * \brief Queries the value of the rh850g4 FXXP_XP1 register field
     * \return Value of the rh850g4 FXXP_XP1 register field
     */
    virtual unsigned char read_FXXP_XP1(void);
    /**
     * \brief Queries the value of the rh850g4 FXXP_XP0 register field
     * \return Value of the rh850g4 FXXP_XP0 register field
     */
    virtual unsigned char read_FXXP_XP0(void);
    /**
     * \brief Queries the value of the rh850g4 GMEIPC register
     * \return Value of the rh850g4 GMEIPC register
     */
    virtual unsigned int read_GMEIPC(void);
    /**
     * \brief Queries the value of the rh850g4 GMEIPSW register
     * \return Value of the rh850g4 GMEIPSW register
     */
    virtual unsigned int read_GMEIPSW(void);
    /**
     * \brief Queries the value of the rh850g4 GMFEPC register
     * \return Value of the rh850g4 GMFEPC register
     */
    virtual unsigned int read_GMFEPC(void);
    /**
     * \brief Queries the value of the rh850g4 GMFEPSW register
     * \return Value of the rh850g4 GMFEPSW register
     */
    virtual unsigned int read_GMFEPSW(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW register
     * \return Value of the rh850g4 GMPSW register
     */
    virtual unsigned int read_GMPSW(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_Z register field
     * \return Value of the rh850g4 GMPSW_Z register field
     */
    virtual unsigned char read_GMPSW_Z(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_S register field
     * \return Value of the rh850g4 GMPSW_S register field
     */
    virtual unsigned char read_GMPSW_S(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_OV register field
     * \return Value of the rh850g4 GMPSW_OV register field
     */
    virtual unsigned char read_GMPSW_OV(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_CY register field
     * \return Value of the rh850g4 GMPSW_CY register field
     */
    virtual unsigned char read_GMPSW_CY(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_SAT register field
     * \return Value of the rh850g4 GMPSW_SAT register field
     */
    virtual unsigned char read_GMPSW_SAT(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_ID register field
     * \return Value of the rh850g4 GMPSW_ID register field
     */
    virtual unsigned char read_GMPSW_ID(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_EP register field
     * \return Value of the rh850g4 GMPSW_EP register field
     */
    virtual unsigned char read_GMPSW_EP(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_NP register field
     * \return Value of the rh850g4 GMPSW_NP register field
     */
    virtual unsigned char read_GMPSW_NP(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_EBV register field
     * \return Value of the rh850g4 GMPSW_EBV register field
     */
    virtual unsigned char read_GMPSW_EBV(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_CU0 register field
     * \return Value of the rh850g4 GMPSW_CU0 register field
     */
    virtual unsigned char read_GMPSW_CU0(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_CU1 register field
     * \return Value of the rh850g4 GMPSW_CU1 register field
     */
    virtual unsigned char read_GMPSW_CU1(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_CU2 register field
     * \return Value of the rh850g4 GMPSW_CU2 register field
     */
    virtual unsigned char read_GMPSW_CU2(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_EIMASK register field
     * \return Value of the rh850g4 GMPSW_EIMASK register field
     */
    virtual unsigned char read_GMPSW_EIMASK(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_UM register field
     * \return Value of the rh850g4 GMPSW_UM register field
     */
    virtual unsigned char read_GMPSW_UM(void);
    /**
     * \brief Queries the value of the rh850g4 GMMEA register
     * \return Value of the rh850g4 GMMEA register
     */
    virtual unsigned int read_GMMEA(void);
    /**
     * \brief Queries the value of the rh850g4 GMMEI register
     * \return Value of the rh850g4 GMMEI register
     */
    virtual unsigned int read_GMMEI(void);
    /**
     * \brief Queries the value of the rh850g4 GMEIIC register
     * \return Value of the rh850g4 GMEIIC register
     */
    virtual unsigned int read_GMEIIC(void);
    /**
     * \brief Queries the value of the rh850g4 GMFEIC register
     * \return Value of the rh850g4 GMFEIC register
     */
    virtual unsigned int read_GMFEIC(void);
    /**
     * \brief Queries the value of the rh850g4 GMSPID register
     * \return Value of the rh850g4 GMSPID register
     */
    virtual unsigned int read_GMSPID(void);
    /**
     * \brief Queries the value of the rh850g4 GMSPIDLIST register
     * \return Value of the rh850g4 GMSPIDLIST register
     */
    virtual unsigned int read_GMSPIDLIST(void);
    /**
     * \brief Queries the value of the rh850g4 GMEBASE register
     * \return Value of the rh850g4 GMEBASE register
     */
    virtual unsigned int read_GMEBASE(void);
    /**
     * \brief Queries the value of the rh850g4 GMEBASE_EBASE register field
     * \return Value of the rh850g4 GMEBASE_EBASE register field
     */
    virtual unsigned int read_GMEBASE_EBASE(void);
    /**
     * \brief Queries the value of the rh850g4 GMEBASE_DV register field
     * \return Value of the rh850g4 GMEBASE_DV register field
     */
    virtual unsigned char read_GMEBASE_DV(void);
    /**
     * \brief Queries the value of the rh850g4 GMEBASE_RINT register field
     * \return Value of the rh850g4 GMEBASE_RINT register field
     */
    virtual unsigned char read_GMEBASE_RINT(void);
    /**
     * \brief Queries the value of the rh850g4 GMINTBP register
     * \return Value of the rh850g4 GMINTBP register
     */
    virtual unsigned int read_GMINTBP(void);
    /**
     * \brief Queries the value of the rh850g4 GMINTCFG register
     * \return Value of the rh850g4 GMINTCFG register
     */
    virtual unsigned int read_GMINTCFG(void);
    /**
     * \brief Queries the value of the rh850g4 GMINTCFG_ISPC register field
     * \return Value of the rh850g4 GMINTCFG_ISPC register field
     */
    virtual unsigned char read_GMINTCFG_ISPC(void);
    /**
     * \brief Queries the value of the rh850g4 GMINTCFG_EPL register field
     * \return Value of the rh850g4 GMINTCFG_EPL register field
     */
    virtual unsigned char read_GMINTCFG_EPL(void);
    /**
     * \brief Queries the value of the rh850g4 GMINTCFG_ULNR register field
     * \return Value of the rh850g4 GMINTCFG_ULNR register field
     */
    virtual unsigned char read_GMINTCFG_ULNR(void);
    /**
     * \brief Queries the value of the rh850g4 GMPLMR register
     * \return Value of the rh850g4 GMPLMR register
     */
    virtual unsigned int read_GMPLMR(void);
    /**
     * \brief Queries the value of the rh850g4 GMPLMR_PLM register field
     * \return Value of the rh850g4 GMPLMR_PLM register field
     */
    virtual unsigned char read_GMPLMR_PLM(void);
    /**
     * \brief Queries the value of the rh850g4 GMSVLOCK register
     * \return Value of the rh850g4 GMSVLOCK register
     */
    virtual unsigned int read_GMSVLOCK(void);
    /**
     * \brief Queries the value of the rh850g4 GMMPM register
     * \return Value of the rh850g4 GMMPM register
     */
    virtual unsigned int read_GMMPM(void);
    /**
     * \brief Queries the value of the rh850g4 GMEIWR register
     * \return Value of the rh850g4 GMEIWR register
     */
    virtual unsigned int read_GMEIWR(void);
    /**
     * \brief Queries the value of the rh850g4 GMFEWR register
     * \return Value of the rh850g4 GMFEWR register
     */
    virtual unsigned int read_GMFEWR(void);
    /**
     * \brief Queries the value of the rh850g4 GMPEID register
     * \return Value of the rh850g4 GMPEID register
     */
    virtual unsigned int read_GMPEID(void);
    /**
     * \brief Queries the value of the rh850g4 MPM register
     * \return Value of the rh850g4 MPM register
     */
    virtual unsigned int read_MPM(void);
    /**
     * \brief Queries the value of the rh850g4 MPCFG register
     * \return Value of the rh850g4 MPCFG register
     */
    virtual unsigned int read_MPCFG(void);
    /**
     * \brief Queries the value of the rh850g4 MPCFG_NMPUE register field
     * \return Value of the rh850g4 MPCFG_NMPUE register field
     */
    virtual unsigned char read_MPCFG_NMPUE(void);
    /**
     * \brief Queries the value of the rh850g4 MPCFG_HBE register field
     * \return Value of the rh850g4 MPCFG_HBE register field
     */
    virtual unsigned char read_MPCFG_HBE(void);
    /**
     * \brief Queries the value of the rh850g4 MPCFG_ARCH register field
     * \return Value of the rh850g4 MPCFG_ARCH register field
     */
    virtual unsigned char read_MPCFG_ARCH(void);
    /**
     * \brief Queries the value of the rh850g4 MPCFG_NBK register field
     * \return Value of the rh850g4 MPCFG_NBK register field
     */
    virtual unsigned char read_MPCFG_NBK(void);
    /**
     * \brief Queries the value of the rh850g4 MPCFG_DMDP register field
     * \return Value of the rh850g4 MPCFG_DMDP register field
     */
    virtual unsigned char read_MPCFG_DMDP(void);
    /**
     * \brief Queries the value of the rh850g4 MCA register
     * \return Value of the rh850g4 MCA register
     */
    virtual unsigned int read_MCA(void);
    /**
     * \brief Queries the value of the rh850g4 MCS register
     * \return Value of the rh850g4 MCS register
     */
    virtual unsigned int read_MCS(void);
    /**
     * \brief Queries the value of the rh850g4 MCC register
     * \return Value of the rh850g4 MCC register
     */
    virtual unsigned int read_MCC(void);
    /**
     * \brief Queries the value of the rh850g4 MCR register
     * \return Value of the rh850g4 MCR register
     */
    virtual unsigned int read_MCR(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_HSXE register field
     * \return Value of the rh850g4 MCR_HSXE register field
     */
    virtual unsigned char read_MCR_HSXE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_HSWE register field
     * \return Value of the rh850g4 MCR_HSWE register field
     */
    virtual unsigned char read_MCR_HSWE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_HSRE register field
     * \return Value of the rh850g4 MCR_HSRE register field
     */
    virtual unsigned char read_MCR_HSRE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_HUXE register field
     * \return Value of the rh850g4 MCR_HUXE register field
     */
    virtual unsigned char read_MCR_HUXE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_HUWE register field
     * \return Value of the rh850g4 MCR_HUWE register field
     */
    virtual unsigned char read_MCR_HUWE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_HURE register field
     * \return Value of the rh850g4 MCR_HURE register field
     */
    virtual unsigned char read_MCR_HURE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_GSXE register field
     * \return Value of the rh850g4 MCR_GSXE register field
     */
    virtual unsigned char read_MCR_GSXE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_GSWE register field
     * \return Value of the rh850g4 MCR_GSWE register field
     */
    virtual unsigned char read_MCR_GSWE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_GSRE register field
     * \return Value of the rh850g4 MCR_GSRE register field
     */
    virtual unsigned char read_MCR_GSRE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_GUXE register field
     * \return Value of the rh850g4 MCR_GUXE register field
     */
    virtual unsigned char read_MCR_GUXE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_GUWE register field
     * \return Value of the rh850g4 MCR_GUWE register field
     */
    virtual unsigned char read_MCR_GUWE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_GURE register field
     * \return Value of the rh850g4 MCR_GURE register field
     */
    virtual unsigned char read_MCR_GURE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_OV register field
     * \return Value of the rh850g4 MCR_OV register field
     */
    virtual unsigned char read_MCR_OV(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_SXE register field
     * \return Value of the rh850g4 MCR_SXE register field
     */
    virtual unsigned char read_MCR_SXE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_SWE register field
     * \return Value of the rh850g4 MCR_SWE register field
     */
    virtual unsigned char read_MCR_SWE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_SRE register field
     * \return Value of the rh850g4 MCR_SRE register field
     */
    virtual unsigned char read_MCR_SRE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_UXE register field
     * \return Value of the rh850g4 MCR_UXE register field
     */
    virtual unsigned char read_MCR_UXE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_UWE register field
     * \return Value of the rh850g4 MCR_UWE register field
     */
    virtual unsigned char read_MCR_UWE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_URE register field
     * \return Value of the rh850g4 MCR_URE register field
     */
    virtual unsigned char read_MCR_URE(void);
    /**
     * \brief Queries the value of the rh850g4 MCI register
     * \return Value of the rh850g4 MCI register
     */
    virtual unsigned int read_MCI(void);
    /**
     * \brief Queries the value of the rh850g4 MCI_SPID register field
     * \return Value of the rh850g4 MCI_SPID register field
     */
    virtual unsigned char read_MCI_SPID(void);
    /**
     * \brief Queries the value of the rh850g4 MPIDX register
     * \return Value of the rh850g4 MPIDX register
     */
    virtual unsigned int read_MPIDX(void);
    /**
     * \brief Queries the value of the rh850g4 MPIDX_IDX register field
     * \return Value of the rh850g4 MPIDX_IDX register field
     */
    virtual unsigned char read_MPIDX_IDX(void);
    /**
     * \brief Queries the value of the rh850g4 MPBK register
     * \return Value of the rh850g4 MPBK register
     */
    virtual unsigned int read_MPBK(void);
    /**
     * \brief Queries the value of the rh850g4 MPBK_BK register field
     * \return Value of the rh850g4 MPBK_BK register field
     */
    virtual unsigned char read_MPBK_BK(void);
    /**
     * \brief Queries the value of the rh850g4 MPLA register
     * \return Value of the rh850g4 MPLA register
     */
    virtual unsigned int read_MPLA(void);
    /**
     * \brief Queries the value of the rh850g4 MPUA register
     * \return Value of the rh850g4 MPUA register
     */
    virtual unsigned int read_MPUA(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT register
     * \return Value of the rh850g4 MPAT register
     */
    virtual unsigned int read_MPAT(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID7 register field
     * \return Value of the rh850g4 MPAT_WMPID7 register field
     */
    virtual unsigned char read_MPAT_WMPID7(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID6 register field
     * \return Value of the rh850g4 MPAT_WMPID6 register field
     */
    virtual unsigned char read_MPAT_WMPID6(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID5 register field
     * \return Value of the rh850g4 MPAT_WMPID5 register field
     */
    virtual unsigned char read_MPAT_WMPID5(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID4 register field
     * \return Value of the rh850g4 MPAT_WMPID4 register field
     */
    virtual unsigned char read_MPAT_WMPID4(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID3 register field
     * \return Value of the rh850g4 MPAT_WMPID3 register field
     */
    virtual unsigned char read_MPAT_WMPID3(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID2 register field
     * \return Value of the rh850g4 MPAT_WMPID2 register field
     */
    virtual unsigned char read_MPAT_WMPID2(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID1 register field
     * \return Value of the rh850g4 MPAT_WMPID1 register field
     */
    virtual unsigned char read_MPAT_WMPID1(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID0 register field
     * \return Value of the rh850g4 MPAT_WMPID0 register field
     */
    virtual unsigned char read_MPAT_WMPID0(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID7 register field
     * \return Value of the rh850g4 MPAT_RMPID7 register field
     */
    virtual unsigned char read_MPAT_RMPID7(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID6 register field
     * \return Value of the rh850g4 MPAT_RMPID6 register field
     */
    virtual unsigned char read_MPAT_RMPID6(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID5 register field
     * \return Value of the rh850g4 MPAT_RMPID5 register field
     */
    virtual unsigned char read_MPAT_RMPID5(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID4 register field
     * \return Value of the rh850g4 MPAT_RMPID4 register field
     */
    virtual unsigned char read_MPAT_RMPID4(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID3 register field
     * \return Value of the rh850g4 MPAT_RMPID3 register field
     */
    virtual unsigned char read_MPAT_RMPID3(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID2 register field
     * \return Value of the rh850g4 MPAT_RMPID2 register field
     */
    virtual unsigned char read_MPAT_RMPID2(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID1 register field
     * \return Value of the rh850g4 MPAT_RMPID1 register field
     */
    virtual unsigned char read_MPAT_RMPID1(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID0 register field
     * \return Value of the rh850g4 MPAT_RMPID0 register field
     */
    virtual unsigned char read_MPAT_RMPID0(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_WG register field
     * \return Value of the rh850g4 MPAT_WG register field
     */
    virtual unsigned char read_MPAT_WG(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_RG register field
     * \return Value of the rh850g4 MPAT_RG register field
     */
    virtual unsigned char read_MPAT_RG(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_E register field
     * \return Value of the rh850g4 MPAT_E register field
     */
    virtual unsigned char read_MPAT_E(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_SX register field
     * \return Value of the rh850g4 MPAT_SX register field
     */
    virtual unsigned char read_MPAT_SX(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_SW register field
     * \return Value of the rh850g4 MPAT_SW register field
     */
    virtual unsigned char read_MPAT_SW(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_SR register field
     * \return Value of the rh850g4 MPAT_SR register field
     */
    virtual unsigned char read_MPAT_SR(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_UX register field
     * \return Value of the rh850g4 MPAT_UX register field
     */
    virtual unsigned char read_MPAT_UX(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_UW register field
     * \return Value of the rh850g4 MPAT_UW register field
     */
    virtual unsigned char read_MPAT_UW(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_UR register field
     * \return Value of the rh850g4 MPAT_UR register field
     */
    virtual unsigned char read_MPAT_UR(void);
    /**
     * \brief Queries the value of the rh850g4 MPID0 register
     * \return Value of the rh850g4 MPID0 register
     */
    virtual unsigned int read_MPID0(void);
    /**
     * \brief Queries the value of the rh850g4 MPID0_SPID register field
     * \return Value of the rh850g4 MPID0_SPID register field
     */
    virtual unsigned char read_MPID0_SPID(void);
    /**
     * \brief Queries the value of the rh850g4 MPID1 register
     * \return Value of the rh850g4 MPID1 register
     */
    virtual unsigned int read_MPID1(void);
    /**
     * \brief Queries the value of the rh850g4 MPID1_SPID register field
     * \return Value of the rh850g4 MPID1_SPID register field
     */
    virtual unsigned char read_MPID1_SPID(void);
    /**
     * \brief Queries the value of the rh850g4 MPID2 register
     * \return Value of the rh850g4 MPID2 register
     */
    virtual unsigned int read_MPID2(void);
    /**
     * \brief Queries the value of the rh850g4 MPID2_SPID register field
     * \return Value of the rh850g4 MPID2_SPID register field
     */
    virtual unsigned char read_MPID2_SPID(void);
    /**
     * \brief Queries the value of the rh850g4 MPID3 register
     * \return Value of the rh850g4 MPID3 register
     */
    virtual unsigned int read_MPID3(void);
    /**
     * \brief Queries the value of the rh850g4 MPID3_SPID register field
     * \return Value of the rh850g4 MPID3_SPID register field
     */
    virtual unsigned char read_MPID3_SPID(void);
    /**
     * \brief Queries the value of the rh850g4 MPID4 register
     * \return Value of the rh850g4 MPID4 register
     */
    virtual unsigned int read_MPID4(void);
    /**
     * \brief Queries the value of the rh850g4 MPID4_SPID register field
     * \return Value of the rh850g4 MPID4_SPID register field
     */
    virtual unsigned char read_MPID4_SPID(void);
    /**
     * \brief Queries the value of the rh850g4 MPID5 register
     * \return Value of the rh850g4 MPID5 register
     */
    virtual unsigned int read_MPID5(void);
    /**
     * \brief Queries the value of the rh850g4 MPID5_SPID register field
     * \return Value of the rh850g4 MPID5_SPID register field
     */
    virtual unsigned char read_MPID5_SPID(void);
    /**
     * \brief Queries the value of the rh850g4 MPID6 register
     * \return Value of the rh850g4 MPID6 register
     */
    virtual unsigned int read_MPID6(void);
    /**
     * \brief Queries the value of the rh850g4 MPID6_SPID register field
     * \return Value of the rh850g4 MPID6_SPID register field
     */
    virtual unsigned char read_MPID6_SPID(void);
    /**
     * \brief Queries the value of the rh850g4 MPID7 register
     * \return Value of the rh850g4 MPID7 register
     */
    virtual unsigned int read_MPID7(void);
    /**
     * \brief Queries the value of the rh850g4 MPID7_SPID register field
     * \return Value of the rh850g4 MPID7_SPID register field
     */
    virtual unsigned char read_MPID7_SPID(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGL register
     * \return Value of the rh850g4 ICTAGL register
     */
    virtual unsigned int read_ICTAGL(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGL_LPN register field
     * \return Value of the rh850g4 ICTAGL_LPN register field
     */
    virtual unsigned int read_ICTAGL_LPN(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGL_LRU register field
     * \return Value of the rh850g4 ICTAGL_LRU register field
     */
    virtual unsigned char read_ICTAGL_LRU(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGL_L register field
     * \return Value of the rh850g4 ICTAGL_L register field
     */
    virtual unsigned char read_ICTAGL_L(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGL_V register field
     * \return Value of the rh850g4 ICTAGL_V register field
     */
    virtual unsigned char read_ICTAGL_V(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGH register
     * \return Value of the rh850g4 ICTAGH register
     */
    virtual unsigned int read_ICTAGH(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGH_WD register field
     * \return Value of the rh850g4 ICTAGH_WD register field
     */
    virtual unsigned char read_ICTAGH_WD(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGH_PD register field
     * \return Value of the rh850g4 ICTAGH_PD register field
     */
    virtual unsigned char read_ICTAGH_PD(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGH_WT register field
     * \return Value of the rh850g4 ICTAGH_WT register field
     */
    virtual unsigned char read_ICTAGH_WT(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGH_PT register field
     * \return Value of the rh850g4 ICTAGH_PT register field
     */
    virtual unsigned char read_ICTAGH_PT(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGH_DATAECC register field
     * \return Value of the rh850g4 ICTAGH_DATAECC register field
     */
    virtual unsigned char read_ICTAGH_DATAECC(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGH_TAGECC register field
     * \return Value of the rh850g4 ICTAGH_TAGECC register field
     */
    virtual unsigned char read_ICTAGH_TAGECC(void);
    /**
     * \brief Queries the value of the rh850g4 ICDATL register
     * \return Value of the rh850g4 ICDATL register
     */
    virtual unsigned int read_ICDATL(void);
    /**
     * \brief Queries the value of the rh850g4 ICDATH register
     * \return Value of the rh850g4 ICDATH register
     */
    virtual unsigned int read_ICDATH(void);
    /**
     * \brief Queries the value of the rh850g4 ICCTRL register
     * \return Value of the rh850g4 ICCTRL register
     */
    virtual unsigned int read_ICCTRL(void);
    /**
     * \brief Queries the value of the rh850g4 ICCTRL_PBS register field
     * \return Value of the rh850g4 ICCTRL_PBS register field
     */
    virtual unsigned char read_ICCTRL_PBS(void);
    /**
     * \brief Queries the value of the rh850g4 ICCTRL_ICHCLR register field
     * \return Value of the rh850g4 ICCTRL_ICHCLR register field
     */
    virtual unsigned char read_ICCTRL_ICHCLR(void);
    /**
     * \brief Queries the value of the rh850g4 ICCTRL_ICHEIV register field
     * \return Value of the rh850g4 ICCTRL_ICHEIV register field
     */
    virtual unsigned char read_ICCTRL_ICHEIV(void);
    /**
     * \brief Queries the value of the rh850g4 ICCTRL_ICHEMK register field
     * \return Value of the rh850g4 ICCTRL_ICHEMK register field
     */
    virtual unsigned char read_ICCTRL_ICHEMK(void);
    /**
     * \brief Queries the value of the rh850g4 ICCTRL_ICHEN register field
     * \return Value of the rh850g4 ICCTRL_ICHEN register field
     */
    virtual unsigned char read_ICCTRL_ICHEN(void);
    /**
     * \brief Queries the value of the rh850g4 ICCFG register
     * \return Value of the rh850g4 ICCFG register
     */
    virtual unsigned int read_ICCFG(void);
    /**
     * \brief Queries the value of the rh850g4 ICCFG_ICHSIZE register field
     * \return Value of the rh850g4 ICCFG_ICHSIZE register field
     */
    virtual unsigned char read_ICCFG_ICHSIZE(void);
    /**
     * \brief Queries the value of the rh850g4 ICCFG_ICHLINE register field
     * \return Value of the rh850g4 ICCFG_ICHLINE register field
     */
    virtual unsigned char read_ICCFG_ICHLINE(void);
    /**
     * \brief Queries the value of the rh850g4 ICCFG_ICHWAY register field
     * \return Value of the rh850g4 ICCFG_ICHWAY register field
     */
    virtual unsigned char read_ICCFG_ICHWAY(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR register
     * \return Value of the rh850g4 ICERR register
     */
    virtual unsigned int read_ICERR(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_CISTW register field
     * \return Value of the rh850g4 ICERR_CISTW register field
     */
    virtual unsigned char read_ICERR_CISTW(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ESAFE register field
     * \return Value of the rh850g4 ICERR_ESAFE register field
     */
    virtual unsigned char read_ICERR_ESAFE(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ESMH register field
     * \return Value of the rh850g4 ICERR_ESMH register field
     */
    virtual unsigned char read_ICERR_ESMH(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ESPBSE register field
     * \return Value of the rh850g4 ICERR_ESPBSE register field
     */
    virtual unsigned char read_ICERR_ESPBSE(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ESTE1 register field
     * \return Value of the rh850g4 ICERR_ESTE1 register field
     */
    virtual unsigned char read_ICERR_ESTE1(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ESTE2 register field
     * \return Value of the rh850g4 ICERR_ESTE2 register field
     */
    virtual unsigned char read_ICERR_ESTE2(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ESDC register field
     * \return Value of the rh850g4 ICERR_ESDC register field
     */
    virtual unsigned char read_ICERR_ESDC(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ESDE register field
     * \return Value of the rh850g4 ICERR_ESDE register field
     */
    virtual unsigned char read_ICERR_ESDE(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ERMAFE register field
     * \return Value of the rh850g4 ICERR_ERMAFE register field
     */
    virtual unsigned char read_ICERR_ERMAFE(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ERMMH register field
     * \return Value of the rh850g4 ICERR_ERMMH register field
     */
    virtual unsigned char read_ICERR_ERMMH(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ERMPBSE register field
     * \return Value of the rh850g4 ICERR_ERMPBSE register field
     */
    virtual unsigned char read_ICERR_ERMPBSE(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ERMTE1 register field
     * \return Value of the rh850g4 ICERR_ERMTE1 register field
     */
    virtual unsigned char read_ICERR_ERMTE1(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ERMTE2 register field
     * \return Value of the rh850g4 ICERR_ERMTE2 register field
     */
    virtual unsigned char read_ICERR_ERMTE2(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ERMDC register field
     * \return Value of the rh850g4 ICERR_ERMDC register field
     */
    virtual unsigned char read_ICERR_ERMDC(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ERMDE register field
     * \return Value of the rh850g4 ICERR_ERMDE register field
     */
    virtual unsigned char read_ICERR_ERMDE(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ICHEWY register field
     * \return Value of the rh850g4 ICERR_ICHEWY register field
     */
    virtual unsigned char read_ICERR_ICHEWY(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ICHEIX register field
     * \return Value of the rh850g4 ICERR_ICHEIX register field
     */
    virtual unsigned char read_ICERR_ICHEIX(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ICHERQ register field
     * \return Value of the rh850g4 ICERR_ICHERQ register field
     */
    virtual unsigned char read_ICERR_ICHERQ(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ICHED register field
     * \return Value of the rh850g4 ICERR_ICHED register field
     */
    virtual unsigned char read_ICERR_ICHED(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ICHET register field
     * \return Value of the rh850g4 ICERR_ICHET register field
     */
    virtual unsigned char read_ICERR_ICHET(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ICHERR register field
     * \return Value of the rh850g4 ICERR_ICHERR register field
     */
    virtual unsigned char read_ICERR_ICHERR(void);
    /**
     * \brief Queries the value of the rh850g4 TSCOUNTL register
     * \return Value of the rh850g4 TSCOUNTL register
     */
    virtual unsigned int read_TSCOUNTL(void);
    /**
     * \brief Queries the value of the rh850g4 TSCOUNTH register
     * \return Value of the rh850g4 TSCOUNTH register
     */
    virtual unsigned int read_TSCOUNTH(void);
    /**
     * \brief Queries the value of the rh850g4 TSCTRL register
     * \return Value of the rh850g4 TSCTRL register
     */
    virtual unsigned int read_TSCTRL(void);
    /**
     * \brief Queries the value of the rh850g4 PMUMCTRL register
     * \return Value of the rh850g4 PMUMCTRL register
     */
    virtual unsigned int read_PMUMCTRL(void);
    /**
     * \brief Queries the value of the rh850g4 PMGMCTRL register
     * \return Value of the rh850g4 PMGMCTRL register
     */
    virtual unsigned int read_PMGMCTRL(void);
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT0 register
     * \return Value of the rh850g4 PMCOUNT0 register
     */
    virtual unsigned int read_PMCOUNT0(void);
    /**
     * \brief Queries the value of the rh850g4 PMCTRL0 register
     * \return Value of the rh850g4 PMCTRL0 register
     */
    virtual unsigned int read_PMCTRL0(void);
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT1 register
     * \return Value of the rh850g4 PMCOUNT1 register
     */
    virtual unsigned int read_PMCOUNT1(void);
    /**
     * \brief Queries the value of the rh850g4 PMCTRL1 register
     * \return Value of the rh850g4 PMCTRL1 register
     */
    virtual unsigned int read_PMCTRL1(void);
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT2 register
     * \return Value of the rh850g4 PMCOUNT2 register
     */
    virtual unsigned int read_PMCOUNT2(void);
    /**
     * \brief Queries the value of the rh850g4 PMCTRL2 register
     * \return Value of the rh850g4 PMCTRL2 register
     */
    virtual unsigned int read_PMCTRL2(void);
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT3 register
     * \return Value of the rh850g4 PMCOUNT3 register
     */
    virtual unsigned int read_PMCOUNT3(void);
    /**
     * \brief Queries the value of the rh850g4 PMCTRL3 register
     * \return Value of the rh850g4 PMCTRL3 register
     */
    virtual unsigned int read_PMCTRL3(void);
    /**
     * \brief Queries the value of the rh850g4 PMCTRL4 register
     * \return Value of the rh850g4 PMCTRL4 register
     */
    virtual unsigned int read_PMCTRL4(void);
    /**
     * \brief Queries the value of the rh850g4 PMCTRL5 register
     * \return Value of the rh850g4 PMCTRL5 register
     */
    virtual unsigned int read_PMCTRL5(void);
    /**
     * \brief Queries the value of the rh850g4 PMCTRL6 register
     * \return Value of the rh850g4 PMCTRL6 register
     */
    virtual unsigned int read_PMCTRL6(void);
    /**
     * \brief Queries the value of the rh850g4 PMCTRL7 register
     * \return Value of the rh850g4 PMCTRL7 register
     */
    virtual unsigned int read_PMCTRL7(void);
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT4 register
     * \return Value of the rh850g4 PMCOUNT4 register
     */
    virtual unsigned int read_PMCOUNT4(void);
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT5 register
     * \return Value of the rh850g4 PMCOUNT5 register
     */
    virtual unsigned int read_PMCOUNT5(void);
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT6 register
     * \return Value of the rh850g4 PMCOUNT6 register
     */
    virtual unsigned int read_PMCOUNT6(void);
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT7 register
     * \return Value of the rh850g4 PMCOUNT7 register
     */
    virtual unsigned int read_PMCOUNT7(void);
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND0 register
     * \return Value of the rh850g4 PMSUBCND0 register
     */
    virtual unsigned int read_PMSUBCND0(void);
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND1 register
     * \return Value of the rh850g4 PMSUBCND1 register
     */
    virtual unsigned int read_PMSUBCND1(void);
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND2 register
     * \return Value of the rh850g4 PMSUBCND2 register
     */
    virtual unsigned int read_PMSUBCND2(void);
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND3 register
     * \return Value of the rh850g4 PMSUBCND3 register
     */
    virtual unsigned int read_PMSUBCND3(void);
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND4 register
     * \return Value of the rh850g4 PMSUBCND4 register
     */
    virtual unsigned int read_PMSUBCND4(void);
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND5 register
     * \return Value of the rh850g4 PMSUBCND5 register
     */
    virtual unsigned int read_PMSUBCND5(void);
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND6 register
     * \return Value of the rh850g4 PMSUBCND6 register
     */
    virtual unsigned int read_PMSUBCND6(void);
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND7 register
     * \return Value of the rh850g4 PMSUBCND7 register
     */
    virtual unsigned int read_PMSUBCND7(void);
    /**
     * \brief Queries the value of the rh850g4 DBGEN register
     * \return Value of the rh850g4 DBGEN register
     */
    virtual unsigned int read_DBGEN(void);
    /**
     * \brief Queries the value of the rh850g4 DBPSWH register
     * \return Value of the rh850g4 DBPSWH register
     */
    virtual unsigned int read_DBPSWH(void);
    /**
     * \brief Queries the value of the rh850g4 DBIC register
     * \return Value of the rh850g4 DBIC register
     */
    virtual unsigned int read_DBIC(void);
    /**
     * \brief Queries the value of the rh850g4 DBPC register
     * \return Value of the rh850g4 DBPC register
     */
    virtual unsigned int read_DBPC(void);
    /**
     * \brief Queries the value of the rh850g4 DBPSW register
     * \return Value of the rh850g4 DBPSW register
     */
    virtual unsigned int read_DBPSW(void);
    /**
     * \brief Queries the value of the rh850g4 DIR0 register
     * \return Value of the rh850g4 DIR0 register
     */
    virtual unsigned int read_DIR0(void);
    /**
     * \brief Queries the value of the rh850g4 DIR1 register
     * \return Value of the rh850g4 DIR1 register
     */
    virtual unsigned int read_DIR1(void);
    /**
     * \brief Queries the value of the rh850g4 BPC register
     * \return Value of the rh850g4 BPC register
     */
    virtual unsigned int read_BPC(void);
    /**
     * \brief Queries the value of the rh850g4 BPAV register
     * \return Value of the rh850g4 BPAV register
     */
    virtual unsigned int read_BPAV(void);
    /**
     * \brief Queries the value of the rh850g4 BPAM register
     * \return Value of the rh850g4 BPAM register
     */
    virtual unsigned int read_BPAM(void);
    /**
     * \brief Queries the value of the rh850g4 DBCFG register
     * \return Value of the rh850g4 DBCFG register
     */
    virtual unsigned int read_DBCFG(void);
    /**
     * \brief Queries the value of the rh850g4 DBWR register
     * \return Value of the rh850g4 DBWR register
     */
    virtual unsigned int read_DBWR(void);
    /**
     * \brief Queries the value of the rh850g4 LSTEST0 register
     * \return Value of the rh850g4 LSTEST0 register
     */
    virtual unsigned int read_LSTEST0(void);
    /**
     * \brief Queries the value of the rh850g4 LSTEST1 register
     * \return Value of the rh850g4 LSTEST1 register
     */
    virtual unsigned int read_LSTEST1(void);
    /**
     * \brief Queries the value of the rh850g4 LSCFG register
     * \return Value of the rh850g4 LSCFG register
     */
    virtual unsigned int read_LSCFG(void);
    /**
     * \brief Queries the value of the rh850g4 ICBKEY register
     * \return Value of the rh850g4 ICBKEY register
     */
    virtual unsigned int read_ICBKEY(void);
    /**
     * \brief Queries the value of the rh850g4 IFCR register
     * \return Value of the rh850g4 IFCR register
     */
    virtual unsigned int read_IFCR(void);
    /**
     * \brief Queries the value of the rh850g4 IFCR1 register
     * \return Value of the rh850g4 IFCR1 register
     */
    virtual unsigned int read_IFCR1(void);
    /**
     * \brief Queries the value of the rh850g4 BRPCTRL0 register
     * \return Value of the rh850g4 BRPCTRL0 register
     */
    virtual unsigned int read_BRPCTRL0(void);
    /**
     * \brief Queries the value of the rh850g4 BRPCTRL1 register
     * \return Value of the rh850g4 BRPCTRL1 register
     */
    virtual unsigned int read_BRPCTRL1(void);
    /**
     * \brief Queries the value of the rh850g4 BRPCFG register
     * \return Value of the rh850g4 BRPCFG register
     */
    virtual unsigned int read_BRPCFG(void);
    /**
     * \brief Queries the value of the rh850g4 BRPACTRL register
     * \return Value of the rh850g4 BRPACTRL register
     */
    virtual unsigned int read_BRPACTRL(void);
    /**
     * \brief Queries the value of the rh850g4 BRPDATA register
     * \return Value of the rh850g4 BRPDATA register
     */
    virtual unsigned int read_BRPDATA(void);
    /**
     * \brief Queries the value of the rh850g4 DCBKEY register
     * \return Value of the rh850g4 DCBKEY register
     */
    virtual unsigned int read_DCBKEY(void);
    /**
     * \brief Queries the value of the rh850g4 LSUCR register
     * \return Value of the rh850g4 LSUCR register
     */
    virtual unsigned int read_LSUCR(void);
    /**
     * \brief Queries the value of the rh850g4 LSULNK0 register
     * \return Value of the rh850g4 LSULNK0 register
     */
    virtual unsigned int read_LSULNK0(void);
    /**
     * \brief Queries the value of the rh850g4 LSULNK1 register
     * \return Value of the rh850g4 LSULNK1 register
     */
    virtual unsigned int read_LSULNK1(void);
    /**
     * \brief Queries the value of the rh850g4 L1RLCR register
     * \return Value of the rh850g4 L1RLCR register
     */
    virtual unsigned int read_L1RLCR(void);
    /**
     * \brief Queries the value of the rh850g4 L1RLNK0 register
     * \return Value of the rh850g4 L1RLNK0 register
     */
    virtual unsigned int read_L1RLNK0(void);
    /**
     * \brief Queries the value of the rh850g4 L1RLNK1 register
     * \return Value of the rh850g4 L1RLNK1 register
     */
    virtual unsigned int read_L1RLNK1(void);
    /**
     * \brief Queries the value of the rh850g4 L1RCFG register
     * \return Value of the rh850g4 L1RCFG register
     */
    virtual unsigned int read_L1RCFG(void);
    /**
     * \brief Queries the value of the rh850g4 DECFG register
     * \return Value of the rh850g4 DECFG register
     */
    virtual unsigned int read_DECFG(void);
    /**
     * \brief Queries the value of the rh850g4 DECTRL register
     * \return Value of the rh850g4 DECTRL register
     */
    virtual unsigned int read_DECTRL(void);
    /**
     * \brief Queries the value of the rh850g4 DEVDS register
     * \return Value of the rh850g4 DEVDS register
     */
    virtual unsigned int read_DEVDS(void);
    /**
     * \brief Queries the value of the rh850g4 RDBCR register
     * \return Value of the rh850g4 RDBCR register
     */
    virtual unsigned int read_RDBCR(void);
    /**
     * \brief Queries the value of the rh850g4 RDBACR register
     * \return Value of the rh850g4 RDBACR register
     */
    virtual unsigned int read_RDBACR(void);
    /**
     * \brief Queries the value of the rh850g4 RDBATAG register
     * \return Value of the rh850g4 RDBATAG register
     */
    virtual unsigned int read_RDBATAG(void);
    /**
     * \brief Queries the value of the rh850g4 RDBADAT0 register
     * \return Value of the rh850g4 RDBADAT0 register
     */
    virtual unsigned int read_RDBADAT0(void);
    /**
     * \brief Queries the value of the rh850g4 RDBADAT1 register
     * \return Value of the rh850g4 RDBADAT1 register
     */
    virtual unsigned int read_RDBADAT1(void);
    /**
     * \brief Queries the value of the rh850g4 RDBADAT2 register
     * \return Value of the rh850g4 RDBADAT2 register
     */
    virtual unsigned int read_RDBADAT2(void);
    /**
     * \brief Queries the value of the rh850g4 RDBADAT3 register
     * \return Value of the rh850g4 RDBADAT3 register
     */
    virtual unsigned int read_RDBADAT3(void);
    /**
     * \brief Queries the value of the rh850g4 RDBSTAT register
     * \return Value of the rh850g4 RDBSTAT register
     */
    virtual unsigned int read_RDBSTAT(void);
    /**
     * \brief Queries the value of the rh850g4 hv_func_support register
     * \return Value of the rh850g4 hv_func_support register
     */
    virtual unsigned char read_hv_func_support(void);
    /**
     * \brief Queries the value of the rh850g4 swd0pcad_exec_count register
     * \return Value of the rh850g4 swd0pcad_exec_count register
     */
    virtual unsigned int read_swd0pcad_exec_count(void);
    /**
     * \brief Queries the value of the rh850g4 swd0pcad_val register
     * \return Value of the rh850g4 swd0pcad_val register
     */
    virtual unsigned int read_swd0pcad_val(void);
    /**
     * \brief Queries the value of the rh850g4 mem_acc_type register
     * \return Value of the rh850g4 mem_acc_type register
     */
    virtual unsigned int read_mem_acc_type(void);

    // immediate register group read operations
    /**
     * \brief Queries the value of the specified register from the rh850g4 GR register group
     * \param index Index value of the required register from the rh850g4 GR register group
     * \return Value of the specified rh850g4 GR group register
     */
    virtual unsigned int read_GR(rh850g4::GR_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 WR register group
     * \param index Index value of the required register from the rh850g4 WR register group
     * \return Value of the specified rh850g4 WR group register
     */
    virtual unsigned long long read_WR(rh850g4::WR_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \return Value of the specified rh850g4 SR_SELID_0 group register
     */
    virtual unsigned int read_SR_SELID_0(rh850g4::SR_SELID_0_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \return Value of the specified rh850g4 SR_SELID_1 group register
     */
    virtual unsigned int read_SR_SELID_1(rh850g4::SR_SELID_1_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \return Value of the specified rh850g4 SR_SELID_2 group register
     */
    virtual unsigned int read_SR_SELID_2(rh850g4::SR_SELID_2_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \return Value of the specified rh850g4 SR_SELID_3 group register
     */
    virtual unsigned int read_SR_SELID_3(rh850g4::SR_SELID_3_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_4 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \return Value of the specified rh850g4 SR_SELID_4 group register
     */
    virtual unsigned int read_SR_SELID_4(rh850g4::SR_SELID_4_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \return Value of the specified rh850g4 SR_SELID_5 group register
     */
    virtual unsigned int read_SR_SELID_5(rh850g4::SR_SELID_5_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_9 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \return Value of the specified rh850g4 SR_SELID_9 group register
     */
    virtual unsigned int read_SR_SELID_9(rh850g4::SR_SELID_9_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_10 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \return Value of the specified rh850g4 SR_SELID_10 group register
     */
    virtual unsigned int read_SR_SELID_10(rh850g4::SR_SELID_10_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_11 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \return Value of the specified rh850g4 SR_SELID_11 group register
     */
    virtual unsigned int read_SR_SELID_11(rh850g4::SR_SELID_11_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_12 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \return Value of the specified rh850g4 SR_SELID_12 group register
     */
    virtual unsigned int read_SR_SELID_12(rh850g4::SR_SELID_12_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_13 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \return Value of the specified rh850g4 SR_SELID_13 group register
     */
    virtual unsigned int read_SR_SELID_13(rh850g4::SR_SELID_13_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_14 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \return Value of the specified rh850g4 SR_SELID_14 group register
     */
    virtual unsigned int read_SR_SELID_14(rh850g4::SR_SELID_14_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_15 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \return Value of the specified rh850g4 SR_SELID_15 group register
     */
    virtual unsigned int read_SR_SELID_15(rh850g4::SR_SELID_15_index_t index);

    // immediate register and register field write operations
    /**
     * \brief Sets the value of the rh850g4 translation time PC register
     * \param value New value of the rh850g4 translation time PC register
     */
    virtual void write_trans_PC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r0 register
     * \param value New value of the rh850g4 r0 register
     */
    virtual void write_r0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r1 register
     * \param value New value of the rh850g4 r1 register
     */
    virtual void write_r1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r2 register
     * \param value New value of the rh850g4 r2 register
     */
    virtual void write_r2(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r3 register
     * \param value New value of the rh850g4 r3 register
     */
    virtual void write_r3(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r4 register
     * \param value New value of the rh850g4 r4 register
     */
    virtual void write_r4(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r5 register
     * \param value New value of the rh850g4 r5 register
     */
    virtual void write_r5(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r6 register
     * \param value New value of the rh850g4 r6 register
     */
    virtual void write_r6(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r7 register
     * \param value New value of the rh850g4 r7 register
     */
    virtual void write_r7(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r8 register
     * \param value New value of the rh850g4 r8 register
     */
    virtual void write_r8(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r9 register
     * \param value New value of the rh850g4 r9 register
     */
    virtual void write_r9(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r10 register
     * \param value New value of the rh850g4 r10 register
     */
    virtual void write_r10(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r11 register
     * \param value New value of the rh850g4 r11 register
     */
    virtual void write_r11(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r12 register
     * \param value New value of the rh850g4 r12 register
     */
    virtual void write_r12(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r13 register
     * \param value New value of the rh850g4 r13 register
     */
    virtual void write_r13(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r14 register
     * \param value New value of the rh850g4 r14 register
     */
    virtual void write_r14(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r15 register
     * \param value New value of the rh850g4 r15 register
     */
    virtual void write_r15(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r16 register
     * \param value New value of the rh850g4 r16 register
     */
    virtual void write_r16(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r17 register
     * \param value New value of the rh850g4 r17 register
     */
    virtual void write_r17(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r18 register
     * \param value New value of the rh850g4 r18 register
     */
    virtual void write_r18(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r19 register
     * \param value New value of the rh850g4 r19 register
     */
    virtual void write_r19(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r20 register
     * \param value New value of the rh850g4 r20 register
     */
    virtual void write_r20(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r21 register
     * \param value New value of the rh850g4 r21 register
     */
    virtual void write_r21(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r22 register
     * \param value New value of the rh850g4 r22 register
     */
    virtual void write_r22(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r23 register
     * \param value New value of the rh850g4 r23 register
     */
    virtual void write_r23(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r24 register
     * \param value New value of the rh850g4 r24 register
     */
    virtual void write_r24(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r25 register
     * \param value New value of the rh850g4 r25 register
     */
    virtual void write_r25(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r26 register
     * \param value New value of the rh850g4 r26 register
     */
    virtual void write_r26(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r27 register
     * \param value New value of the rh850g4 r27 register
     */
    virtual void write_r27(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r28 register
     * \param value New value of the rh850g4 r28 register
     */
    virtual void write_r28(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r29 register
     * \param value New value of the rh850g4 r29 register
     */
    virtual void write_r29(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r30 register
     * \param value New value of the rh850g4 r30 register
     */
    virtual void write_r30(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r31 register
     * \param value New value of the rh850g4 r31 register
     */
    virtual void write_r31(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PC register
     * \param value New value of the rh850g4 PC register
     */
    virtual void write_PC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 wr0 register
     * \param value New value of the rh850g4 wr0 register
     */
    virtual void write_wr0(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr0_u register
     * \param value New value of the rh850g4 wr0_u register
     */
    virtual void write_wr0_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr1 register
     * \param value New value of the rh850g4 wr1 register
     */
    virtual void write_wr1(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr1_u register
     * \param value New value of the rh850g4 wr1_u register
     */
    virtual void write_wr1_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr2 register
     * \param value New value of the rh850g4 wr2 register
     */
    virtual void write_wr2(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr2_u register
     * \param value New value of the rh850g4 wr2_u register
     */
    virtual void write_wr2_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr3 register
     * \param value New value of the rh850g4 wr3 register
     */
    virtual void write_wr3(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr3_u register
     * \param value New value of the rh850g4 wr3_u register
     */
    virtual void write_wr3_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr4 register
     * \param value New value of the rh850g4 wr4 register
     */
    virtual void write_wr4(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr4_u register
     * \param value New value of the rh850g4 wr4_u register
     */
    virtual void write_wr4_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr5 register
     * \param value New value of the rh850g4 wr5 register
     */
    virtual void write_wr5(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr5_u register
     * \param value New value of the rh850g4 wr5_u register
     */
    virtual void write_wr5_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr6 register
     * \param value New value of the rh850g4 wr6 register
     */
    virtual void write_wr6(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr6_u register
     * \param value New value of the rh850g4 wr6_u register
     */
    virtual void write_wr6_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr7 register
     * \param value New value of the rh850g4 wr7 register
     */
    virtual void write_wr7(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr7_u register
     * \param value New value of the rh850g4 wr7_u register
     */
    virtual void write_wr7_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr8 register
     * \param value New value of the rh850g4 wr8 register
     */
    virtual void write_wr8(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr8_u register
     * \param value New value of the rh850g4 wr8_u register
     */
    virtual void write_wr8_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr9 register
     * \param value New value of the rh850g4 wr9 register
     */
    virtual void write_wr9(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr9_u register
     * \param value New value of the rh850g4 wr9_u register
     */
    virtual void write_wr9_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr10 register
     * \param value New value of the rh850g4 wr10 register
     */
    virtual void write_wr10(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr10_u register
     * \param value New value of the rh850g4 wr10_u register
     */
    virtual void write_wr10_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr11 register
     * \param value New value of the rh850g4 wr11 register
     */
    virtual void write_wr11(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr11_u register
     * \param value New value of the rh850g4 wr11_u register
     */
    virtual void write_wr11_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr12 register
     * \param value New value of the rh850g4 wr12 register
     */
    virtual void write_wr12(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr12_u register
     * \param value New value of the rh850g4 wr12_u register
     */
    virtual void write_wr12_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr13 register
     * \param value New value of the rh850g4 wr13 register
     */
    virtual void write_wr13(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr13_u register
     * \param value New value of the rh850g4 wr13_u register
     */
    virtual void write_wr13_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr14 register
     * \param value New value of the rh850g4 wr14 register
     */
    virtual void write_wr14(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr14_u register
     * \param value New value of the rh850g4 wr14_u register
     */
    virtual void write_wr14_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr15 register
     * \param value New value of the rh850g4 wr15 register
     */
    virtual void write_wr15(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr15_u register
     * \param value New value of the rh850g4 wr15_u register
     */
    virtual void write_wr15_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr16 register
     * \param value New value of the rh850g4 wr16 register
     */
    virtual void write_wr16(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr16_u register
     * \param value New value of the rh850g4 wr16_u register
     */
    virtual void write_wr16_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr17 register
     * \param value New value of the rh850g4 wr17 register
     */
    virtual void write_wr17(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr17_u register
     * \param value New value of the rh850g4 wr17_u register
     */
    virtual void write_wr17_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr18 register
     * \param value New value of the rh850g4 wr18 register
     */
    virtual void write_wr18(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr18_u register
     * \param value New value of the rh850g4 wr18_u register
     */
    virtual void write_wr18_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr19 register
     * \param value New value of the rh850g4 wr19 register
     */
    virtual void write_wr19(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr19_u register
     * \param value New value of the rh850g4 wr19_u register
     */
    virtual void write_wr19_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr20 register
     * \param value New value of the rh850g4 wr20 register
     */
    virtual void write_wr20(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr20_u register
     * \param value New value of the rh850g4 wr20_u register
     */
    virtual void write_wr20_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr21 register
     * \param value New value of the rh850g4 wr21 register
     */
    virtual void write_wr21(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr21_u register
     * \param value New value of the rh850g4 wr21_u register
     */
    virtual void write_wr21_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr22 register
     * \param value New value of the rh850g4 wr22 register
     */
    virtual void write_wr22(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr22_u register
     * \param value New value of the rh850g4 wr22_u register
     */
    virtual void write_wr22_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr23 register
     * \param value New value of the rh850g4 wr23 register
     */
    virtual void write_wr23(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr23_u register
     * \param value New value of the rh850g4 wr23_u register
     */
    virtual void write_wr23_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr24 register
     * \param value New value of the rh850g4 wr24 register
     */
    virtual void write_wr24(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr24_u register
     * \param value New value of the rh850g4 wr24_u register
     */
    virtual void write_wr24_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr25 register
     * \param value New value of the rh850g4 wr25 register
     */
    virtual void write_wr25(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr25_u register
     * \param value New value of the rh850g4 wr25_u register
     */
    virtual void write_wr25_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr26 register
     * \param value New value of the rh850g4 wr26 register
     */
    virtual void write_wr26(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr26_u register
     * \param value New value of the rh850g4 wr26_u register
     */
    virtual void write_wr26_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr27 register
     * \param value New value of the rh850g4 wr27 register
     */
    virtual void write_wr27(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr27_u register
     * \param value New value of the rh850g4 wr27_u register
     */
    virtual void write_wr27_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr28 register
     * \param value New value of the rh850g4 wr28 register
     */
    virtual void write_wr28(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr28_u register
     * \param value New value of the rh850g4 wr28_u register
     */
    virtual void write_wr28_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr29 register
     * \param value New value of the rh850g4 wr29 register
     */
    virtual void write_wr29(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr29_u register
     * \param value New value of the rh850g4 wr29_u register
     */
    virtual void write_wr29_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr30 register
     * \param value New value of the rh850g4 wr30 register
     */
    virtual void write_wr30(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr30_u register
     * \param value New value of the rh850g4 wr30_u register
     */
    virtual void write_wr30_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr31 register
     * \param value New value of the rh850g4 wr31 register
     */
    virtual void write_wr31(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr31_u register
     * \param value New value of the rh850g4 wr31_u register
     */
    virtual void write_wr31_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 EIPC register
     * \param value New value of the rh850g4 EIPC register
     */
    virtual void write_EIPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 EIPSW register
     * \param value New value of the rh850g4 EIPSW register
     */
    virtual void write_EIPSW(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FEPC register
     * \param value New value of the rh850g4 FEPC register
     */
    virtual void write_FEPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FEPSW register
     * \param value New value of the rh850g4 FEPSW register
     */
    virtual void write_FEPSW(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PSW register
     * \param value New value of the rh850g4 PSW register
     */
    virtual void write_PSW(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PSW_Z register field
     * \param value New value of the rh850g4 PSW_Z register field
     */
    virtual void write_PSW_Z(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_S register field
     * \param value New value of the rh850g4 PSW_S register field
     */
    virtual void write_PSW_S(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_OV register field
     * \param value New value of the rh850g4 PSW_OV register field
     */
    virtual void write_PSW_OV(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_CY register field
     * \param value New value of the rh850g4 PSW_CY register field
     */
    virtual void write_PSW_CY(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_SAT register field
     * \param value New value of the rh850g4 PSW_SAT register field
     */
    virtual void write_PSW_SAT(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_ID register field
     * \param value New value of the rh850g4 PSW_ID register field
     */
    virtual void write_PSW_ID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_EP register field
     * \param value New value of the rh850g4 PSW_EP register field
     */
    virtual void write_PSW_EP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_NP register field
     * \param value New value of the rh850g4 PSW_NP register field
     */
    virtual void write_PSW_NP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_EBV register field
     * \param value New value of the rh850g4 PSW_EBV register field
     */
    virtual void write_PSW_EBV(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_CU0 register field
     * \param value New value of the rh850g4 PSW_CU0 register field
     */
    virtual void write_PSW_CU0(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_CU1 register field
     * \param value New value of the rh850g4 PSW_CU1 register field
     */
    virtual void write_PSW_CU1(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_CU2 register field
     * \param value New value of the rh850g4 PSW_CU2 register field
     */
    virtual void write_PSW_CU2(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_EIMASK register field
     * \param value New value of the rh850g4 PSW_EIMASK register field
     */
    virtual void write_PSW_EIMASK(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_UM register field
     * \param value New value of the rh850g4 PSW_UM register field
     */
    virtual void write_PSW_UM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPSR register
     * \param value New value of the rh850g4 FPSR register
     */
    virtual void write_FPSR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FPSR_CC register field
     * \param value New value of the rh850g4 FPSR_CC register field
     */
    virtual void write_FPSR_CC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPSR_FN register field
     * \param value New value of the rh850g4 FPSR_FN register field
     */
    virtual void write_FPSR_FN(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPSR_IF register field
     * \param value New value of the rh850g4 FPSR_IF register field
     */
    virtual void write_FPSR_IF(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPSR_PEM register field
     * \param value New value of the rh850g4 FPSR_PEM register field
     */
    virtual void write_FPSR_PEM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPSR_RM register field
     * \param value New value of the rh850g4 FPSR_RM register field
     */
    virtual void write_FPSR_RM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPSR_FS register field
     * \param value New value of the rh850g4 FPSR_FS register field
     */
    virtual void write_FPSR_FS(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPSR_XC register field
     * \param value New value of the rh850g4 FPSR_XC register field
     */
    virtual void write_FPSR_XC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPSR_XE register field
     * \param value New value of the rh850g4 FPSR_XE register field
     */
    virtual void write_FPSR_XE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPSR_XP register field
     * \param value New value of the rh850g4 FPSR_XP register field
     */
    virtual void write_FPSR_XP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPEPC register
     * \param value New value of the rh850g4 FPEPC register
     */
    virtual void write_FPEPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FPST register
     * \param value New value of the rh850g4 FPST register
     */
    virtual void write_FPST(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FPST_XC register field
     * \param value New value of the rh850g4 FPST_XC register field
     */
    virtual void write_FPST_XC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPST_IF register field
     * \param value New value of the rh850g4 FPST_IF register field
     */
    virtual void write_FPST_IF(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPST_XP register field
     * \param value New value of the rh850g4 FPST_XP register field
     */
    virtual void write_FPST_XP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPCC register
     * \param value New value of the rh850g4 FPCC register
     */
    virtual void write_FPCC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FPCC_CC register field
     * \param value New value of the rh850g4 FPCC_CC register field
     */
    virtual void write_FPCC_CC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPCFG register
     * \param value New value of the rh850g4 FPCFG register
     */
    virtual void write_FPCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FPCFG_RM register field
     * \param value New value of the rh850g4 FPCFG_RM register field
     */
    virtual void write_FPCFG_RM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPCFG_XE register field
     * \param value New value of the rh850g4 FPCFG_XE register field
     */
    virtual void write_FPCFG_XE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 EIIC register
     * \param value New value of the rh850g4 EIIC register
     */
    virtual void write_EIIC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FEIC register
     * \param value New value of the rh850g4 FEIC register
     */
    virtual void write_FEIC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PSWH register
     * \param value New value of the rh850g4 PSWH register
     */
    virtual void write_PSWH(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PSWH_GPID register field
     * \param value New value of the rh850g4 PSWH_GPID register field
     */
    virtual void write_PSWH_GPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSWH_GM register field
     * \param value New value of the rh850g4 PSWH_GM register field
     */
    virtual void write_PSWH_GM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 CTPC register
     * \param value New value of the rh850g4 CTPC register
     */
    virtual void write_CTPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 CTPSW register
     * \param value New value of the rh850g4 CTPSW register
     */
    virtual void write_CTPSW(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 EIPSWH register
     * \param value New value of the rh850g4 EIPSWH register
     */
    virtual void write_EIPSWH(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 EIPSWH_GPID register field
     * \param value New value of the rh850g4 EIPSWH_GPID register field
     */
    virtual void write_EIPSWH_GPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 EIPSWH_GM register field
     * \param value New value of the rh850g4 EIPSWH_GM register field
     */
    virtual void write_EIPSWH_GM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FEPSWH register
     * \param value New value of the rh850g4 FEPSWH register
     */
    virtual void write_FEPSWH(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FEPSWH_GPID register field
     * \param value New value of the rh850g4 FEPSWH_GPID register field
     */
    virtual void write_FEPSWH_GPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FEPSWH_GM register field
     * \param value New value of the rh850g4 FEPSWH_GM register field
     */
    virtual void write_FEPSWH_GM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 CTBP register
     * \param value New value of the rh850g4 CTBP register
     */
    virtual void write_CTBP(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 SNZCFG register
     * \param value New value of the rh850g4 SNZCFG register
     */
    virtual void write_SNZCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 EIWR register
     * \param value New value of the rh850g4 EIWR register
     */
    virtual void write_EIWR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FEWR register
     * \param value New value of the rh850g4 FEWR register
     */
    virtual void write_FEWR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 SPID register
     * \param value New value of the rh850g4 SPID register
     */
    virtual void write_SPID(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 SPIDLIST register
     * \param value New value of the rh850g4 SPIDLIST register
     */
    virtual void write_SPIDLIST(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RBASE register
     * \param value New value of the rh850g4 RBASE register
     */
    virtual void write_RBASE(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RBASE_RBASE register field
     * \param value New value of the rh850g4 RBASE_RBASE register field
     */
    virtual void write_RBASE_RBASE(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RBASE_DV register field
     * \param value New value of the rh850g4 RBASE_DV register field
     */
    virtual void write_RBASE_DV(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 RBASE_RINT register field
     * \param value New value of the rh850g4 RBASE_RINT register field
     */
    virtual void write_RBASE_RINT(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 EBASE register
     * \param value New value of the rh850g4 EBASE register
     */
    virtual void write_EBASE(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 EBASE_EBASE register field
     * \param value New value of the rh850g4 EBASE_EBASE register field
     */
    virtual void write_EBASE_EBASE(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 EBASE_DV register field
     * \param value New value of the rh850g4 EBASE_DV register field
     */
    virtual void write_EBASE_DV(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 EBASE_RINT register field
     * \param value New value of the rh850g4 EBASE_RINT register field
     */
    virtual void write_EBASE_RINT(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 INTBP register
     * \param value New value of the rh850g4 INTBP register
     */
    virtual void write_INTBP(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MCTL register
     * \param value New value of the rh850g4 MCTL register
     */
    virtual void write_MCTL(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MCTL_UIC register field
     * \param value New value of the rh850g4 MCTL_UIC register field
     */
    virtual void write_MCTL_UIC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PID register
     * \param value New value of the rh850g4 PID register
     */
    virtual void write_PID(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 SVLOCK register
     * \param value New value of the rh850g4 SVLOCK register
     */
    virtual void write_SVLOCK(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 SCCFG register
     * \param value New value of the rh850g4 SCCFG register
     */
    virtual void write_SCCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 SCCFG_SIZE register field
     * \param value New value of the rh850g4 SCCFG_SIZE register field
     */
    virtual void write_SCCFG_SIZE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 SCBP register
     * \param value New value of the rh850g4 SCBP register
     */
    virtual void write_SCBP(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 HVCFG register
     * \param value New value of the rh850g4 HVCFG register
     */
    virtual void write_HVCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 HVCFG_HVE register field
     * \param value New value of the rh850g4 HVCFG_HVE register field
     */
    virtual void write_HVCFG_HVE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMCFG register
     * \param value New value of the rh850g4 GMCFG register
     */
    virtual void write_GMCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMCFG_GMP register field
     * \param value New value of the rh850g4 GMCFG_GMP register field
     */
    virtual void write_GMCFG_GMP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMCFG_HMP register field
     * \param value New value of the rh850g4 GMCFG_HMP register field
     */
    virtual void write_GMCFG_HMP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMCFG_GSYSE register field
     * \param value New value of the rh850g4 GMCFG_GSYSE register field
     */
    virtual void write_GMCFG_GSYSE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMCFG_GCU0 register field
     * \param value New value of the rh850g4 GMCFG_GCU0 register field
     */
    virtual void write_GMCFG_GCU0(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMCFG_GCU1 register field
     * \param value New value of the rh850g4 GMCFG_GCU1 register field
     */
    virtual void write_GMCFG_GCU1(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMCFG_GCU2 register field
     * \param value New value of the rh850g4 GMCFG_GCU2 register field
     */
    virtual void write_GMCFG_GCU2(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 HVSB register
     * \param value New value of the rh850g4 HVSB register
     */
    virtual void write_HVSB(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PEID register
     * \param value New value of the rh850g4 PEID register
     */
    virtual void write_PEID(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 BMID register
     * \param value New value of the rh850g4 BMID register
     */
    virtual void write_BMID(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 BMID_BMID register field
     * \param value New value of the rh850g4 BMID_BMID register field
     */
    virtual void write_BMID_BMID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MEA register
     * \param value New value of the rh850g4 MEA register
     */
    virtual void write_MEA(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MEI register
     * \param value New value of the rh850g4 MEI register
     */
    virtual void write_MEI(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RBCR0 register
     * \param value New value of the rh850g4 RBCR0 register
     */
    virtual void write_RBCR0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RBCR0_MD register field
     * \param value New value of the rh850g4 RBCR0_MD register field
     */
    virtual void write_RBCR0_MD(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 RBCR0_BE register field
     * \param value New value of the rh850g4 RBCR0_BE register field
     */
    virtual void write_RBCR0_BE(unsigned short value);
    /**
     * \brief Sets the value of the rh850g4 RBCR1 register
     * \param value New value of the rh850g4 RBCR1 register
     */
    virtual void write_RBCR1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RBCR1_NC register field
     * \param value New value of the rh850g4 RBCR1_NC register field
     */
    virtual void write_RBCR1_NC(unsigned short value);
    /**
     * \brief Sets the value of the rh850g4 RBNR register
     * \param value New value of the rh850g4 RBNR register
     */
    virtual void write_RBNR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RBNR_BN register field
     * \param value New value of the rh850g4 RBNR_BN register field
     */
    virtual void write_RBNR_BN(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 RBIP register
     * \param value New value of the rh850g4 RBIP register
     */
    virtual void write_RBIP(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RBIP_RBIP register field
     * \param value New value of the rh850g4 RBIP_RBIP register field
     */
    virtual void write_RBIP_RBIP(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ISPR register
     * \param value New value of the rh850g4 ISPR register
     */
    virtual void write_ISPR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ISPR_ISP register field
     * \param value New value of the rh850g4 ISPR_ISP register field
     */
    virtual void write_ISPR_ISP(unsigned short value);
    /**
     * \brief Sets the value of the rh850g4 IMSR register
     * \param value New value of the rh850g4 IMSR register
     */
    virtual void write_IMSR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 IMSR_EEIM register field
     * \param value New value of the rh850g4 IMSR_EEIM register field
     */
    virtual void write_IMSR_EEIM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 IMSR_EPLM register field
     * \param value New value of the rh850g4 IMSR_EPLM register field
     */
    virtual void write_IMSR_EPLM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 IMSR_EID register field
     * \param value New value of the rh850g4 IMSR_EID register field
     */
    virtual void write_IMSR_EID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 IMSR_ENP register field
     * \param value New value of the rh850g4 IMSR_ENP register field
     */
    virtual void write_IMSR_ENP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 IMSR_FNP register field
     * \param value New value of the rh850g4 IMSR_FNP register field
     */
    virtual void write_IMSR_FNP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 IMSR_HEEIM register field
     * \param value New value of the rh850g4 IMSR_HEEIM register field
     */
    virtual void write_IMSR_HEEIM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 IMSR_HEPLM register field
     * \param value New value of the rh850g4 IMSR_HEPLM register field
     */
    virtual void write_IMSR_HEPLM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 IMSR_HEID register field
     * \param value New value of the rh850g4 IMSR_HEID register field
     */
    virtual void write_IMSR_HEID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 IMSR_HENP register field
     * \param value New value of the rh850g4 IMSR_HENP register field
     */
    virtual void write_IMSR_HENP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 IMSR_HFNP register field
     * \param value New value of the rh850g4 IMSR_HFNP register field
     */
    virtual void write_IMSR_HFNP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICSR register
     * \param value New value of the rh850g4 ICSR register
     */
    virtual void write_ICSR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICSR_PMEI register field
     * \param value New value of the rh850g4 ICSR_PMEI register field
     */
    virtual void write_ICSR_PMEI(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 INTCFG register
     * \param value New value of the rh850g4 INTCFG register
     */
    virtual void write_INTCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 INTCFG_ISPC register field
     * \param value New value of the rh850g4 INTCFG_ISPC register field
     */
    virtual void write_INTCFG_ISPC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 INTCFG_EPL register field
     * \param value New value of the rh850g4 INTCFG_EPL register field
     */
    virtual void write_INTCFG_EPL(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 INTCFG_ULNR register field
     * \param value New value of the rh850g4 INTCFG_ULNR register field
     */
    virtual void write_INTCFG_ULNR(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PLMR register
     * \param value New value of the rh850g4 PLMR register
     */
    virtual void write_PLMR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PLMR_PLM register field
     * \param value New value of the rh850g4 PLMR_PLM register field
     */
    virtual void write_PLMR_PLM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXSR register
     * \param value New value of the rh850g4 FXSR register
     */
    virtual void write_FXSR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FXSR_FN register field
     * \param value New value of the rh850g4 FXSR_FN register field
     */
    virtual void write_FXSR_FN(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXSR_IF register field
     * \param value New value of the rh850g4 FXSR_IF register field
     */
    virtual void write_FXSR_IF(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXSR_PEM register field
     * \param value New value of the rh850g4 FXSR_PEM register field
     */
    virtual void write_FXSR_PEM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXSR_RM register field
     * \param value New value of the rh850g4 FXSR_RM register field
     */
    virtual void write_FXSR_RM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXSR_FS register field
     * \param value New value of the rh850g4 FXSR_FS register field
     */
    virtual void write_FXSR_FS(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXSR_XC register field
     * \param value New value of the rh850g4 FXSR_XC register field
     */
    virtual void write_FXSR_XC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXSR_XE register field
     * \param value New value of the rh850g4 FXSR_XE register field
     */
    virtual void write_FXSR_XE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXSR_XP register field
     * \param value New value of the rh850g4 FXSR_XP register field
     */
    virtual void write_FXSR_XP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXST register
     * \param value New value of the rh850g4 FXST register
     */
    virtual void write_FXST(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FXST_XC register field
     * \param value New value of the rh850g4 FXST_XC register field
     */
    virtual void write_FXST_XC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXST_IF register field
     * \param value New value of the rh850g4 FXST_IF register field
     */
    virtual void write_FXST_IF(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXST_XP register field
     * \param value New value of the rh850g4 FXST_XP register field
     */
    virtual void write_FXST_XP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXINFO register
     * \param value New value of the rh850g4 FXINFO register
     */
    virtual void write_FXINFO(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FXINFO_NFPU register field
     * \param value New value of the rh850g4 FXINFO_NFPU register field
     */
    virtual void write_FXINFO_NFPU(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXINFO_RSIZE register field
     * \param value New value of the rh850g4 FXINFO_RSIZE register field
     */
    virtual void write_FXINFO_RSIZE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXCFG register
     * \param value New value of the rh850g4 FXCFG register
     */
    virtual void write_FXCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FXCFG_RM register field
     * \param value New value of the rh850g4 FXCFG_RM register field
     */
    virtual void write_FXCFG_RM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXCFG_XE register field
     * \param value New value of the rh850g4 FXCFG_XE register field
     */
    virtual void write_FXCFG_XE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXXC register
     * \param value New value of the rh850g4 FXXC register
     */
    virtual void write_FXXC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FXXC_XC3 register field
     * \param value New value of the rh850g4 FXXC_XC3 register field
     */
    virtual void write_FXXC_XC3(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXXC_XC2 register field
     * \param value New value of the rh850g4 FXXC_XC2 register field
     */
    virtual void write_FXXC_XC2(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXXC_XC1 register field
     * \param value New value of the rh850g4 FXXC_XC1 register field
     */
    virtual void write_FXXC_XC1(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXXC_XC0 register field
     * \param value New value of the rh850g4 FXXC_XC0 register field
     */
    virtual void write_FXXC_XC0(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXXP register
     * \param value New value of the rh850g4 FXXP register
     */
    virtual void write_FXXP(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FXXP_XP3 register field
     * \param value New value of the rh850g4 FXXP_XP3 register field
     */
    virtual void write_FXXP_XP3(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXXP_XP2 register field
     * \param value New value of the rh850g4 FXXP_XP2 register field
     */
    virtual void write_FXXP_XP2(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXXP_XP1 register field
     * \param value New value of the rh850g4 FXXP_XP1 register field
     */
    virtual void write_FXXP_XP1(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXXP_XP0 register field
     * \param value New value of the rh850g4 FXXP_XP0 register field
     */
    virtual void write_FXXP_XP0(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMEIPC register
     * \param value New value of the rh850g4 GMEIPC register
     */
    virtual void write_GMEIPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMEIPSW register
     * \param value New value of the rh850g4 GMEIPSW register
     */
    virtual void write_GMEIPSW(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMFEPC register
     * \param value New value of the rh850g4 GMFEPC register
     */
    virtual void write_GMFEPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMFEPSW register
     * \param value New value of the rh850g4 GMFEPSW register
     */
    virtual void write_GMFEPSW(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW register
     * \param value New value of the rh850g4 GMPSW register
     */
    virtual void write_GMPSW(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_Z register field
     * \param value New value of the rh850g4 GMPSW_Z register field
     */
    virtual void write_GMPSW_Z(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_S register field
     * \param value New value of the rh850g4 GMPSW_S register field
     */
    virtual void write_GMPSW_S(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_OV register field
     * \param value New value of the rh850g4 GMPSW_OV register field
     */
    virtual void write_GMPSW_OV(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_CY register field
     * \param value New value of the rh850g4 GMPSW_CY register field
     */
    virtual void write_GMPSW_CY(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_SAT register field
     * \param value New value of the rh850g4 GMPSW_SAT register field
     */
    virtual void write_GMPSW_SAT(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_ID register field
     * \param value New value of the rh850g4 GMPSW_ID register field
     */
    virtual void write_GMPSW_ID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_EP register field
     * \param value New value of the rh850g4 GMPSW_EP register field
     */
    virtual void write_GMPSW_EP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_NP register field
     * \param value New value of the rh850g4 GMPSW_NP register field
     */
    virtual void write_GMPSW_NP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_EBV register field
     * \param value New value of the rh850g4 GMPSW_EBV register field
     */
    virtual void write_GMPSW_EBV(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_CU0 register field
     * \param value New value of the rh850g4 GMPSW_CU0 register field
     */
    virtual void write_GMPSW_CU0(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_CU1 register field
     * \param value New value of the rh850g4 GMPSW_CU1 register field
     */
    virtual void write_GMPSW_CU1(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_CU2 register field
     * \param value New value of the rh850g4 GMPSW_CU2 register field
     */
    virtual void write_GMPSW_CU2(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_EIMASK register field
     * \param value New value of the rh850g4 GMPSW_EIMASK register field
     */
    virtual void write_GMPSW_EIMASK(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_UM register field
     * \param value New value of the rh850g4 GMPSW_UM register field
     */
    virtual void write_GMPSW_UM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMMEA register
     * \param value New value of the rh850g4 GMMEA register
     */
    virtual void write_GMMEA(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMMEI register
     * \param value New value of the rh850g4 GMMEI register
     */
    virtual void write_GMMEI(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMEIIC register
     * \param value New value of the rh850g4 GMEIIC register
     */
    virtual void write_GMEIIC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMFEIC register
     * \param value New value of the rh850g4 GMFEIC register
     */
    virtual void write_GMFEIC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMSPID register
     * \param value New value of the rh850g4 GMSPID register
     */
    virtual void write_GMSPID(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMSPIDLIST register
     * \param value New value of the rh850g4 GMSPIDLIST register
     */
    virtual void write_GMSPIDLIST(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMEBASE register
     * \param value New value of the rh850g4 GMEBASE register
     */
    virtual void write_GMEBASE(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMEBASE_EBASE register field
     * \param value New value of the rh850g4 GMEBASE_EBASE register field
     */
    virtual void write_GMEBASE_EBASE(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMEBASE_DV register field
     * \param value New value of the rh850g4 GMEBASE_DV register field
     */
    virtual void write_GMEBASE_DV(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMEBASE_RINT register field
     * \param value New value of the rh850g4 GMEBASE_RINT register field
     */
    virtual void write_GMEBASE_RINT(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMINTBP register
     * \param value New value of the rh850g4 GMINTBP register
     */
    virtual void write_GMINTBP(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMINTCFG register
     * \param value New value of the rh850g4 GMINTCFG register
     */
    virtual void write_GMINTCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMINTCFG_ISPC register field
     * \param value New value of the rh850g4 GMINTCFG_ISPC register field
     */
    virtual void write_GMINTCFG_ISPC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMINTCFG_EPL register field
     * \param value New value of the rh850g4 GMINTCFG_EPL register field
     */
    virtual void write_GMINTCFG_EPL(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMINTCFG_ULNR register field
     * \param value New value of the rh850g4 GMINTCFG_ULNR register field
     */
    virtual void write_GMINTCFG_ULNR(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPLMR register
     * \param value New value of the rh850g4 GMPLMR register
     */
    virtual void write_GMPLMR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMPLMR_PLM register field
     * \param value New value of the rh850g4 GMPLMR_PLM register field
     */
    virtual void write_GMPLMR_PLM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMSVLOCK register
     * \param value New value of the rh850g4 GMSVLOCK register
     */
    virtual void write_GMSVLOCK(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMMPM register
     * \param value New value of the rh850g4 GMMPM register
     */
    virtual void write_GMMPM(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMEIWR register
     * \param value New value of the rh850g4 GMEIWR register
     */
    virtual void write_GMEIWR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMFEWR register
     * \param value New value of the rh850g4 GMFEWR register
     */
    virtual void write_GMFEWR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMPEID register
     * \param value New value of the rh850g4 GMPEID register
     */
    virtual void write_GMPEID(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPM register
     * \param value New value of the rh850g4 MPM register
     */
    virtual void write_MPM(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPCFG register
     * \param value New value of the rh850g4 MPCFG register
     */
    virtual void write_MPCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPCFG_NMPUE register field
     * \param value New value of the rh850g4 MPCFG_NMPUE register field
     */
    virtual void write_MPCFG_NMPUE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPCFG_HBE register field
     * \param value New value of the rh850g4 MPCFG_HBE register field
     */
    virtual void write_MPCFG_HBE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPCFG_ARCH register field
     * \param value New value of the rh850g4 MPCFG_ARCH register field
     */
    virtual void write_MPCFG_ARCH(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPCFG_NBK register field
     * \param value New value of the rh850g4 MPCFG_NBK register field
     */
    virtual void write_MPCFG_NBK(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPCFG_DMDP register field
     * \param value New value of the rh850g4 MPCFG_DMDP register field
     */
    virtual void write_MPCFG_DMDP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCA register
     * \param value New value of the rh850g4 MCA register
     */
    virtual void write_MCA(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MCS register
     * \param value New value of the rh850g4 MCS register
     */
    virtual void write_MCS(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MCC register
     * \param value New value of the rh850g4 MCC register
     */
    virtual void write_MCC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MCR register
     * \param value New value of the rh850g4 MCR register
     */
    virtual void write_MCR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MCR_HSXE register field
     * \param value New value of the rh850g4 MCR_HSXE register field
     */
    virtual void write_MCR_HSXE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_HSWE register field
     * \param value New value of the rh850g4 MCR_HSWE register field
     */
    virtual void write_MCR_HSWE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_HSRE register field
     * \param value New value of the rh850g4 MCR_HSRE register field
     */
    virtual void write_MCR_HSRE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_HUXE register field
     * \param value New value of the rh850g4 MCR_HUXE register field
     */
    virtual void write_MCR_HUXE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_HUWE register field
     * \param value New value of the rh850g4 MCR_HUWE register field
     */
    virtual void write_MCR_HUWE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_HURE register field
     * \param value New value of the rh850g4 MCR_HURE register field
     */
    virtual void write_MCR_HURE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_GSXE register field
     * \param value New value of the rh850g4 MCR_GSXE register field
     */
    virtual void write_MCR_GSXE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_GSWE register field
     * \param value New value of the rh850g4 MCR_GSWE register field
     */
    virtual void write_MCR_GSWE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_GSRE register field
     * \param value New value of the rh850g4 MCR_GSRE register field
     */
    virtual void write_MCR_GSRE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_GUXE register field
     * \param value New value of the rh850g4 MCR_GUXE register field
     */
    virtual void write_MCR_GUXE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_GUWE register field
     * \param value New value of the rh850g4 MCR_GUWE register field
     */
    virtual void write_MCR_GUWE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_GURE register field
     * \param value New value of the rh850g4 MCR_GURE register field
     */
    virtual void write_MCR_GURE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_OV register field
     * \param value New value of the rh850g4 MCR_OV register field
     */
    virtual void write_MCR_OV(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_SXE register field
     * \param value New value of the rh850g4 MCR_SXE register field
     */
    virtual void write_MCR_SXE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_SWE register field
     * \param value New value of the rh850g4 MCR_SWE register field
     */
    virtual void write_MCR_SWE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_SRE register field
     * \param value New value of the rh850g4 MCR_SRE register field
     */
    virtual void write_MCR_SRE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_UXE register field
     * \param value New value of the rh850g4 MCR_UXE register field
     */
    virtual void write_MCR_UXE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_UWE register field
     * \param value New value of the rh850g4 MCR_UWE register field
     */
    virtual void write_MCR_UWE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_URE register field
     * \param value New value of the rh850g4 MCR_URE register field
     */
    virtual void write_MCR_URE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCI register
     * \param value New value of the rh850g4 MCI register
     */
    virtual void write_MCI(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MCI_SPID register field
     * \param value New value of the rh850g4 MCI_SPID register field
     */
    virtual void write_MCI_SPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPIDX register
     * \param value New value of the rh850g4 MPIDX register
     */
    virtual void write_MPIDX(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPIDX_IDX register field
     * \param value New value of the rh850g4 MPIDX_IDX register field
     */
    virtual void write_MPIDX_IDX(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPBK register
     * \param value New value of the rh850g4 MPBK register
     */
    virtual void write_MPBK(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPBK_BK register field
     * \param value New value of the rh850g4 MPBK_BK register field
     */
    virtual void write_MPBK_BK(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPLA register
     * \param value New value of the rh850g4 MPLA register
     */
    virtual void write_MPLA(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPUA register
     * \param value New value of the rh850g4 MPUA register
     */
    virtual void write_MPUA(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPAT register
     * \param value New value of the rh850g4 MPAT register
     */
    virtual void write_MPAT(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID7 register field
     * \param value New value of the rh850g4 MPAT_WMPID7 register field
     */
    virtual void write_MPAT_WMPID7(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID6 register field
     * \param value New value of the rh850g4 MPAT_WMPID6 register field
     */
    virtual void write_MPAT_WMPID6(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID5 register field
     * \param value New value of the rh850g4 MPAT_WMPID5 register field
     */
    virtual void write_MPAT_WMPID5(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID4 register field
     * \param value New value of the rh850g4 MPAT_WMPID4 register field
     */
    virtual void write_MPAT_WMPID4(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID3 register field
     * \param value New value of the rh850g4 MPAT_WMPID3 register field
     */
    virtual void write_MPAT_WMPID3(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID2 register field
     * \param value New value of the rh850g4 MPAT_WMPID2 register field
     */
    virtual void write_MPAT_WMPID2(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID1 register field
     * \param value New value of the rh850g4 MPAT_WMPID1 register field
     */
    virtual void write_MPAT_WMPID1(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID0 register field
     * \param value New value of the rh850g4 MPAT_WMPID0 register field
     */
    virtual void write_MPAT_WMPID0(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID7 register field
     * \param value New value of the rh850g4 MPAT_RMPID7 register field
     */
    virtual void write_MPAT_RMPID7(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID6 register field
     * \param value New value of the rh850g4 MPAT_RMPID6 register field
     */
    virtual void write_MPAT_RMPID6(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID5 register field
     * \param value New value of the rh850g4 MPAT_RMPID5 register field
     */
    virtual void write_MPAT_RMPID5(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID4 register field
     * \param value New value of the rh850g4 MPAT_RMPID4 register field
     */
    virtual void write_MPAT_RMPID4(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID3 register field
     * \param value New value of the rh850g4 MPAT_RMPID3 register field
     */
    virtual void write_MPAT_RMPID3(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID2 register field
     * \param value New value of the rh850g4 MPAT_RMPID2 register field
     */
    virtual void write_MPAT_RMPID2(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID1 register field
     * \param value New value of the rh850g4 MPAT_RMPID1 register field
     */
    virtual void write_MPAT_RMPID1(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID0 register field
     * \param value New value of the rh850g4 MPAT_RMPID0 register field
     */
    virtual void write_MPAT_RMPID0(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_WG register field
     * \param value New value of the rh850g4 MPAT_WG register field
     */
    virtual void write_MPAT_WG(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_RG register field
     * \param value New value of the rh850g4 MPAT_RG register field
     */
    virtual void write_MPAT_RG(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_E register field
     * \param value New value of the rh850g4 MPAT_E register field
     */
    virtual void write_MPAT_E(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_SX register field
     * \param value New value of the rh850g4 MPAT_SX register field
     */
    virtual void write_MPAT_SX(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_SW register field
     * \param value New value of the rh850g4 MPAT_SW register field
     */
    virtual void write_MPAT_SW(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_SR register field
     * \param value New value of the rh850g4 MPAT_SR register field
     */
    virtual void write_MPAT_SR(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_UX register field
     * \param value New value of the rh850g4 MPAT_UX register field
     */
    virtual void write_MPAT_UX(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_UW register field
     * \param value New value of the rh850g4 MPAT_UW register field
     */
    virtual void write_MPAT_UW(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_UR register field
     * \param value New value of the rh850g4 MPAT_UR register field
     */
    virtual void write_MPAT_UR(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPID0 register
     * \param value New value of the rh850g4 MPID0 register
     */
    virtual void write_MPID0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID0_SPID register field
     * \param value New value of the rh850g4 MPID0_SPID register field
     */
    virtual void write_MPID0_SPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPID1 register
     * \param value New value of the rh850g4 MPID1 register
     */
    virtual void write_MPID1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID1_SPID register field
     * \param value New value of the rh850g4 MPID1_SPID register field
     */
    virtual void write_MPID1_SPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPID2 register
     * \param value New value of the rh850g4 MPID2 register
     */
    virtual void write_MPID2(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID2_SPID register field
     * \param value New value of the rh850g4 MPID2_SPID register field
     */
    virtual void write_MPID2_SPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPID3 register
     * \param value New value of the rh850g4 MPID3 register
     */
    virtual void write_MPID3(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID3_SPID register field
     * \param value New value of the rh850g4 MPID3_SPID register field
     */
    virtual void write_MPID3_SPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPID4 register
     * \param value New value of the rh850g4 MPID4 register
     */
    virtual void write_MPID4(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID4_SPID register field
     * \param value New value of the rh850g4 MPID4_SPID register field
     */
    virtual void write_MPID4_SPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPID5 register
     * \param value New value of the rh850g4 MPID5 register
     */
    virtual void write_MPID5(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID5_SPID register field
     * \param value New value of the rh850g4 MPID5_SPID register field
     */
    virtual void write_MPID5_SPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPID6 register
     * \param value New value of the rh850g4 MPID6 register
     */
    virtual void write_MPID6(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID6_SPID register field
     * \param value New value of the rh850g4 MPID6_SPID register field
     */
    virtual void write_MPID6_SPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPID7 register
     * \param value New value of the rh850g4 MPID7 register
     */
    virtual void write_MPID7(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID7_SPID register field
     * \param value New value of the rh850g4 MPID7_SPID register field
     */
    virtual void write_MPID7_SPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGL register
     * \param value New value of the rh850g4 ICTAGL register
     */
    virtual void write_ICTAGL(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGL_LPN register field
     * \param value New value of the rh850g4 ICTAGL_LPN register field
     */
    virtual void write_ICTAGL_LPN(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGL_LRU register field
     * \param value New value of the rh850g4 ICTAGL_LRU register field
     */
    virtual void write_ICTAGL_LRU(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGL_L register field
     * \param value New value of the rh850g4 ICTAGL_L register field
     */
    virtual void write_ICTAGL_L(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGL_V register field
     * \param value New value of the rh850g4 ICTAGL_V register field
     */
    virtual void write_ICTAGL_V(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGH register
     * \param value New value of the rh850g4 ICTAGH register
     */
    virtual void write_ICTAGH(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGH_WD register field
     * \param value New value of the rh850g4 ICTAGH_WD register field
     */
    virtual void write_ICTAGH_WD(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGH_PD register field
     * \param value New value of the rh850g4 ICTAGH_PD register field
     */
    virtual void write_ICTAGH_PD(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGH_WT register field
     * \param value New value of the rh850g4 ICTAGH_WT register field
     */
    virtual void write_ICTAGH_WT(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGH_PT register field
     * \param value New value of the rh850g4 ICTAGH_PT register field
     */
    virtual void write_ICTAGH_PT(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGH_DATAECC register field
     * \param value New value of the rh850g4 ICTAGH_DATAECC register field
     */
    virtual void write_ICTAGH_DATAECC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGH_TAGECC register field
     * \param value New value of the rh850g4 ICTAGH_TAGECC register field
     */
    virtual void write_ICTAGH_TAGECC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICDATL register
     * \param value New value of the rh850g4 ICDATL register
     */
    virtual void write_ICDATL(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICDATH register
     * \param value New value of the rh850g4 ICDATH register
     */
    virtual void write_ICDATH(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICCTRL register
     * \param value New value of the rh850g4 ICCTRL register
     */
    virtual void write_ICCTRL(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICCTRL_PBS register field
     * \param value New value of the rh850g4 ICCTRL_PBS register field
     */
    virtual void write_ICCTRL_PBS(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICCTRL_ICHCLR register field
     * \param value New value of the rh850g4 ICCTRL_ICHCLR register field
     */
    virtual void write_ICCTRL_ICHCLR(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICCTRL_ICHEIV register field
     * \param value New value of the rh850g4 ICCTRL_ICHEIV register field
     */
    virtual void write_ICCTRL_ICHEIV(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICCTRL_ICHEMK register field
     * \param value New value of the rh850g4 ICCTRL_ICHEMK register field
     */
    virtual void write_ICCTRL_ICHEMK(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICCTRL_ICHEN register field
     * \param value New value of the rh850g4 ICCTRL_ICHEN register field
     */
    virtual void write_ICCTRL_ICHEN(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICCFG register
     * \param value New value of the rh850g4 ICCFG register
     */
    virtual void write_ICCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICCFG_ICHSIZE register field
     * \param value New value of the rh850g4 ICCFG_ICHSIZE register field
     */
    virtual void write_ICCFG_ICHSIZE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICCFG_ICHLINE register field
     * \param value New value of the rh850g4 ICCFG_ICHLINE register field
     */
    virtual void write_ICCFG_ICHLINE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICCFG_ICHWAY register field
     * \param value New value of the rh850g4 ICCFG_ICHWAY register field
     */
    virtual void write_ICCFG_ICHWAY(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR register
     * \param value New value of the rh850g4 ICERR register
     */
    virtual void write_ICERR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_CISTW register field
     * \param value New value of the rh850g4 ICERR_CISTW register field
     */
    virtual void write_ICERR_CISTW(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ESAFE register field
     * \param value New value of the rh850g4 ICERR_ESAFE register field
     */
    virtual void write_ICERR_ESAFE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ESMH register field
     * \param value New value of the rh850g4 ICERR_ESMH register field
     */
    virtual void write_ICERR_ESMH(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ESPBSE register field
     * \param value New value of the rh850g4 ICERR_ESPBSE register field
     */
    virtual void write_ICERR_ESPBSE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ESTE1 register field
     * \param value New value of the rh850g4 ICERR_ESTE1 register field
     */
    virtual void write_ICERR_ESTE1(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ESTE2 register field
     * \param value New value of the rh850g4 ICERR_ESTE2 register field
     */
    virtual void write_ICERR_ESTE2(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ESDC register field
     * \param value New value of the rh850g4 ICERR_ESDC register field
     */
    virtual void write_ICERR_ESDC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ESDE register field
     * \param value New value of the rh850g4 ICERR_ESDE register field
     */
    virtual void write_ICERR_ESDE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ERMAFE register field
     * \param value New value of the rh850g4 ICERR_ERMAFE register field
     */
    virtual void write_ICERR_ERMAFE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ERMMH register field
     * \param value New value of the rh850g4 ICERR_ERMMH register field
     */
    virtual void write_ICERR_ERMMH(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ERMPBSE register field
     * \param value New value of the rh850g4 ICERR_ERMPBSE register field
     */
    virtual void write_ICERR_ERMPBSE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ERMTE1 register field
     * \param value New value of the rh850g4 ICERR_ERMTE1 register field
     */
    virtual void write_ICERR_ERMTE1(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ERMTE2 register field
     * \param value New value of the rh850g4 ICERR_ERMTE2 register field
     */
    virtual void write_ICERR_ERMTE2(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ERMDC register field
     * \param value New value of the rh850g4 ICERR_ERMDC register field
     */
    virtual void write_ICERR_ERMDC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ERMDE register field
     * \param value New value of the rh850g4 ICERR_ERMDE register field
     */
    virtual void write_ICERR_ERMDE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ICHEWY register field
     * \param value New value of the rh850g4 ICERR_ICHEWY register field
     */
    virtual void write_ICERR_ICHEWY(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ICHEIX register field
     * \param value New value of the rh850g4 ICERR_ICHEIX register field
     */
    virtual void write_ICERR_ICHEIX(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ICHERQ register field
     * \param value New value of the rh850g4 ICERR_ICHERQ register field
     */
    virtual void write_ICERR_ICHERQ(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ICHED register field
     * \param value New value of the rh850g4 ICERR_ICHED register field
     */
    virtual void write_ICERR_ICHED(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ICHET register field
     * \param value New value of the rh850g4 ICERR_ICHET register field
     */
    virtual void write_ICERR_ICHET(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ICHERR register field
     * \param value New value of the rh850g4 ICERR_ICHERR register field
     */
    virtual void write_ICERR_ICHERR(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 TSCOUNTL register
     * \param value New value of the rh850g4 TSCOUNTL register
     */
    virtual void write_TSCOUNTL(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 TSCOUNTH register
     * \param value New value of the rh850g4 TSCOUNTH register
     */
    virtual void write_TSCOUNTH(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 TSCTRL register
     * \param value New value of the rh850g4 TSCTRL register
     */
    virtual void write_TSCTRL(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMUMCTRL register
     * \param value New value of the rh850g4 PMUMCTRL register
     */
    virtual void write_PMUMCTRL(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMGMCTRL register
     * \param value New value of the rh850g4 PMGMCTRL register
     */
    virtual void write_PMGMCTRL(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT0 register
     * \param value New value of the rh850g4 PMCOUNT0 register
     */
    virtual void write_PMCOUNT0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCTRL0 register
     * \param value New value of the rh850g4 PMCTRL0 register
     */
    virtual void write_PMCTRL0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT1 register
     * \param value New value of the rh850g4 PMCOUNT1 register
     */
    virtual void write_PMCOUNT1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCTRL1 register
     * \param value New value of the rh850g4 PMCTRL1 register
     */
    virtual void write_PMCTRL1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT2 register
     * \param value New value of the rh850g4 PMCOUNT2 register
     */
    virtual void write_PMCOUNT2(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCTRL2 register
     * \param value New value of the rh850g4 PMCTRL2 register
     */
    virtual void write_PMCTRL2(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT3 register
     * \param value New value of the rh850g4 PMCOUNT3 register
     */
    virtual void write_PMCOUNT3(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCTRL3 register
     * \param value New value of the rh850g4 PMCTRL3 register
     */
    virtual void write_PMCTRL3(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCTRL4 register
     * \param value New value of the rh850g4 PMCTRL4 register
     */
    virtual void write_PMCTRL4(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCTRL5 register
     * \param value New value of the rh850g4 PMCTRL5 register
     */
    virtual void write_PMCTRL5(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCTRL6 register
     * \param value New value of the rh850g4 PMCTRL6 register
     */
    virtual void write_PMCTRL6(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCTRL7 register
     * \param value New value of the rh850g4 PMCTRL7 register
     */
    virtual void write_PMCTRL7(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT4 register
     * \param value New value of the rh850g4 PMCOUNT4 register
     */
    virtual void write_PMCOUNT4(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT5 register
     * \param value New value of the rh850g4 PMCOUNT5 register
     */
    virtual void write_PMCOUNT5(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT6 register
     * \param value New value of the rh850g4 PMCOUNT6 register
     */
    virtual void write_PMCOUNT6(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT7 register
     * \param value New value of the rh850g4 PMCOUNT7 register
     */
    virtual void write_PMCOUNT7(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND0 register
     * \param value New value of the rh850g4 PMSUBCND0 register
     */
    virtual void write_PMSUBCND0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND1 register
     * \param value New value of the rh850g4 PMSUBCND1 register
     */
    virtual void write_PMSUBCND1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND2 register
     * \param value New value of the rh850g4 PMSUBCND2 register
     */
    virtual void write_PMSUBCND2(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND3 register
     * \param value New value of the rh850g4 PMSUBCND3 register
     */
    virtual void write_PMSUBCND3(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND4 register
     * \param value New value of the rh850g4 PMSUBCND4 register
     */
    virtual void write_PMSUBCND4(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND5 register
     * \param value New value of the rh850g4 PMSUBCND5 register
     */
    virtual void write_PMSUBCND5(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND6 register
     * \param value New value of the rh850g4 PMSUBCND6 register
     */
    virtual void write_PMSUBCND6(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND7 register
     * \param value New value of the rh850g4 PMSUBCND7 register
     */
    virtual void write_PMSUBCND7(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DBGEN register
     * \param value New value of the rh850g4 DBGEN register
     */
    virtual void write_DBGEN(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DBPSWH register
     * \param value New value of the rh850g4 DBPSWH register
     */
    virtual void write_DBPSWH(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DBIC register
     * \param value New value of the rh850g4 DBIC register
     */
    virtual void write_DBIC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DBPC register
     * \param value New value of the rh850g4 DBPC register
     */
    virtual void write_DBPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DBPSW register
     * \param value New value of the rh850g4 DBPSW register
     */
    virtual void write_DBPSW(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DIR0 register
     * \param value New value of the rh850g4 DIR0 register
     */
    virtual void write_DIR0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DIR1 register
     * \param value New value of the rh850g4 DIR1 register
     */
    virtual void write_DIR1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 BPC register
     * \param value New value of the rh850g4 BPC register
     */
    virtual void write_BPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 BPAV register
     * \param value New value of the rh850g4 BPAV register
     */
    virtual void write_BPAV(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 BPAM register
     * \param value New value of the rh850g4 BPAM register
     */
    virtual void write_BPAM(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DBCFG register
     * \param value New value of the rh850g4 DBCFG register
     */
    virtual void write_DBCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DBWR register
     * \param value New value of the rh850g4 DBWR register
     */
    virtual void write_DBWR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 LSTEST0 register
     * \param value New value of the rh850g4 LSTEST0 register
     */
    virtual void write_LSTEST0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 LSTEST1 register
     * \param value New value of the rh850g4 LSTEST1 register
     */
    virtual void write_LSTEST1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 LSCFG register
     * \param value New value of the rh850g4 LSCFG register
     */
    virtual void write_LSCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICBKEY register
     * \param value New value of the rh850g4 ICBKEY register
     */
    virtual void write_ICBKEY(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 IFCR register
     * \param value New value of the rh850g4 IFCR register
     */
    virtual void write_IFCR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 IFCR1 register
     * \param value New value of the rh850g4 IFCR1 register
     */
    virtual void write_IFCR1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 BRPCTRL0 register
     * \param value New value of the rh850g4 BRPCTRL0 register
     */
    virtual void write_BRPCTRL0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 BRPCTRL1 register
     * \param value New value of the rh850g4 BRPCTRL1 register
     */
    virtual void write_BRPCTRL1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 BRPCFG register
     * \param value New value of the rh850g4 BRPCFG register
     */
    virtual void write_BRPCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 BRPACTRL register
     * \param value New value of the rh850g4 BRPACTRL register
     */
    virtual void write_BRPACTRL(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 BRPDATA register
     * \param value New value of the rh850g4 BRPDATA register
     */
    virtual void write_BRPDATA(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DCBKEY register
     * \param value New value of the rh850g4 DCBKEY register
     */
    virtual void write_DCBKEY(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 LSUCR register
     * \param value New value of the rh850g4 LSUCR register
     */
    virtual void write_LSUCR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 LSULNK0 register
     * \param value New value of the rh850g4 LSULNK0 register
     */
    virtual void write_LSULNK0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 LSULNK1 register
     * \param value New value of the rh850g4 LSULNK1 register
     */
    virtual void write_LSULNK1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 L1RLCR register
     * \param value New value of the rh850g4 L1RLCR register
     */
    virtual void write_L1RLCR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 L1RLNK0 register
     * \param value New value of the rh850g4 L1RLNK0 register
     */
    virtual void write_L1RLNK0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 L1RLNK1 register
     * \param value New value of the rh850g4 L1RLNK1 register
     */
    virtual void write_L1RLNK1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 L1RCFG register
     * \param value New value of the rh850g4 L1RCFG register
     */
    virtual void write_L1RCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DECFG register
     * \param value New value of the rh850g4 DECFG register
     */
    virtual void write_DECFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DECTRL register
     * \param value New value of the rh850g4 DECTRL register
     */
    virtual void write_DECTRL(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DEVDS register
     * \param value New value of the rh850g4 DEVDS register
     */
    virtual void write_DEVDS(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RDBCR register
     * \param value New value of the rh850g4 RDBCR register
     */
    virtual void write_RDBCR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RDBACR register
     * \param value New value of the rh850g4 RDBACR register
     */
    virtual void write_RDBACR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RDBATAG register
     * \param value New value of the rh850g4 RDBATAG register
     */
    virtual void write_RDBATAG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RDBADAT0 register
     * \param value New value of the rh850g4 RDBADAT0 register
     */
    virtual void write_RDBADAT0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RDBADAT1 register
     * \param value New value of the rh850g4 RDBADAT1 register
     */
    virtual void write_RDBADAT1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RDBADAT2 register
     * \param value New value of the rh850g4 RDBADAT2 register
     */
    virtual void write_RDBADAT2(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RDBADAT3 register
     * \param value New value of the rh850g4 RDBADAT3 register
     */
    virtual void write_RDBADAT3(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RDBSTAT register
     * \param value New value of the rh850g4 RDBSTAT register
     */
    virtual void write_RDBSTAT(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 hv_func_support register
     * \param value New value of the rh850g4 hv_func_support register
     */
    virtual void write_hv_func_support(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 swd0pcad_exec_count register
     * \param value New value of the rh850g4 swd0pcad_exec_count register
     */
    virtual void write_swd0pcad_exec_count(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 swd0pcad_val register
     * \param value New value of the rh850g4 swd0pcad_val register
     */
    virtual void write_swd0pcad_val(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 mem_acc_type register
     * \param value New value of the rh850g4 mem_acc_type register
     */
    virtual void write_mem_acc_type(unsigned int value);

    // immediate register group write operations
    /**
     * \brief Sets the value of the specified register from the rh850g4 GR register group
     * \param index Index value of the required register from the rh850g4 GR register group
     * \param value New value of the specified rh850g4 GR group register
     */
    virtual void write_GR(rh850g4::GR_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 WR register group
     * \param index Index value of the required register from the rh850g4 WR register group
     * \param value New value of the specified rh850g4 WR group register
     */
    virtual void write_WR(rh850g4::WR_index_t index, unsigned long long value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \param value New value of the specified rh850g4 SR_SELID_0 group register
     */
    virtual void write_SR_SELID_0(rh850g4::SR_SELID_0_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \param value New value of the specified rh850g4 SR_SELID_1 group register
     */
    virtual void write_SR_SELID_1(rh850g4::SR_SELID_1_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \param value New value of the specified rh850g4 SR_SELID_2 group register
     */
    virtual void write_SR_SELID_2(rh850g4::SR_SELID_2_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \param value New value of the specified rh850g4 SR_SELID_3 group register
     */
    virtual void write_SR_SELID_3(rh850g4::SR_SELID_3_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_4 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \param value New value of the specified rh850g4 SR_SELID_4 group register
     */
    virtual void write_SR_SELID_4(rh850g4::SR_SELID_4_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \param value New value of the specified rh850g4 SR_SELID_5 group register
     */
    virtual void write_SR_SELID_5(rh850g4::SR_SELID_5_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_9 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \param value New value of the specified rh850g4 SR_SELID_9 group register
     */
    virtual void write_SR_SELID_9(rh850g4::SR_SELID_9_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_10 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \param value New value of the specified rh850g4 SR_SELID_10 group register
     */
    virtual void write_SR_SELID_10(rh850g4::SR_SELID_10_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_11 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \param value New value of the specified rh850g4 SR_SELID_11 group register
     */
    virtual void write_SR_SELID_11(rh850g4::SR_SELID_11_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_12 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \param value New value of the specified rh850g4 SR_SELID_12 group register
     */
    virtual void write_SR_SELID_12(rh850g4::SR_SELID_12_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_13 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \param value New value of the specified rh850g4 SR_SELID_13 group register
     */
    virtual void write_SR_SELID_13(rh850g4::SR_SELID_13_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_14 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \param value New value of the specified rh850g4 SR_SELID_14 group register
     */
    virtual void write_SR_SELID_14(rh850g4::SR_SELID_14_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_15 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \param value New value of the specified rh850g4 SR_SELID_15 group register
     */
    virtual void write_SR_SELID_15(rh850g4::SR_SELID_15_index_t index, unsigned int value);

    // immediate register and register field increment operations
    /**
     * \brief Increments the value of the rh850g4 translation time PC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 translation time PC register
     */
    virtual void increment_trans_PC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r0 register
     */
    virtual void increment_r0(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r1 register
     */
    virtual void increment_r1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r2 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r2 register
     */
    virtual void increment_r2(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r3 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r3 register
     */
    virtual void increment_r3(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r4 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r4 register
     */
    virtual void increment_r4(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r5 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r5 register
     */
    virtual void increment_r5(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r6 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r6 register
     */
    virtual void increment_r6(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r7 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r7 register
     */
    virtual void increment_r7(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r8 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r8 register
     */
    virtual void increment_r8(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r9 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r9 register
     */
    virtual void increment_r9(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r10 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r10 register
     */
    virtual void increment_r10(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r11 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r11 register
     */
    virtual void increment_r11(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r12 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r12 register
     */
    virtual void increment_r12(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r13 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r13 register
     */
    virtual void increment_r13(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r14 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r14 register
     */
    virtual void increment_r14(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r15 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r15 register
     */
    virtual void increment_r15(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r16 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r16 register
     */
    virtual void increment_r16(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r17 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r17 register
     */
    virtual void increment_r17(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r18 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r18 register
     */
    virtual void increment_r18(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r19 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r19 register
     */
    virtual void increment_r19(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r20 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r20 register
     */
    virtual void increment_r20(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r21 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r21 register
     */
    virtual void increment_r21(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r22 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r22 register
     */
    virtual void increment_r22(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r23 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r23 register
     */
    virtual void increment_r23(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r24 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r24 register
     */
    virtual void increment_r24(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r25 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r25 register
     */
    virtual void increment_r25(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r26 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r26 register
     */
    virtual void increment_r26(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r27 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r27 register
     */
    virtual void increment_r27(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r28 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r28 register
     */
    virtual void increment_r28(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r29 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r29 register
     */
    virtual void increment_r29(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r30 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r30 register
     */
    virtual void increment_r30(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r31 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r31 register
     */
    virtual void increment_r31(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PC register
     */
    virtual void increment_PC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 wr0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr0 register
     */
    virtual void increment_wr0(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr0_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr0_u register
     */
    virtual void increment_wr0_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr1 register
     */
    virtual void increment_wr1(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr1_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr1_u register
     */
    virtual void increment_wr1_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr2 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr2 register
     */
    virtual void increment_wr2(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr2_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr2_u register
     */
    virtual void increment_wr2_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr3 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr3 register
     */
    virtual void increment_wr3(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr3_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr3_u register
     */
    virtual void increment_wr3_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr4 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr4 register
     */
    virtual void increment_wr4(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr4_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr4_u register
     */
    virtual void increment_wr4_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr5 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr5 register
     */
    virtual void increment_wr5(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr5_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr5_u register
     */
    virtual void increment_wr5_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr6 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr6 register
     */
    virtual void increment_wr6(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr6_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr6_u register
     */
    virtual void increment_wr6_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr7 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr7 register
     */
    virtual void increment_wr7(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr7_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr7_u register
     */
    virtual void increment_wr7_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr8 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr8 register
     */
    virtual void increment_wr8(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr8_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr8_u register
     */
    virtual void increment_wr8_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr9 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr9 register
     */
    virtual void increment_wr9(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr9_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr9_u register
     */
    virtual void increment_wr9_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr10 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr10 register
     */
    virtual void increment_wr10(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr10_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr10_u register
     */
    virtual void increment_wr10_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr11 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr11 register
     */
    virtual void increment_wr11(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr11_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr11_u register
     */
    virtual void increment_wr11_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr12 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr12 register
     */
    virtual void increment_wr12(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr12_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr12_u register
     */
    virtual void increment_wr12_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr13 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr13 register
     */
    virtual void increment_wr13(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr13_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr13_u register
     */
    virtual void increment_wr13_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr14 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr14 register
     */
    virtual void increment_wr14(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr14_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr14_u register
     */
    virtual void increment_wr14_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr15 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr15 register
     */
    virtual void increment_wr15(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr15_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr15_u register
     */
    virtual void increment_wr15_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr16 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr16 register
     */
    virtual void increment_wr16(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr16_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr16_u register
     */
    virtual void increment_wr16_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr17 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr17 register
     */
    virtual void increment_wr17(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr17_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr17_u register
     */
    virtual void increment_wr17_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr18 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr18 register
     */
    virtual void increment_wr18(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr18_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr18_u register
     */
    virtual void increment_wr18_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr19 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr19 register
     */
    virtual void increment_wr19(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr19_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr19_u register
     */
    virtual void increment_wr19_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr20 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr20 register
     */
    virtual void increment_wr20(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr20_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr20_u register
     */
    virtual void increment_wr20_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr21 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr21 register
     */
    virtual void increment_wr21(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr21_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr21_u register
     */
    virtual void increment_wr21_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr22 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr22 register
     */
    virtual void increment_wr22(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr22_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr22_u register
     */
    virtual void increment_wr22_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr23 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr23 register
     */
    virtual void increment_wr23(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr23_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr23_u register
     */
    virtual void increment_wr23_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr24 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr24 register
     */
    virtual void increment_wr24(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr24_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr24_u register
     */
    virtual void increment_wr24_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr25 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr25 register
     */
    virtual void increment_wr25(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr25_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr25_u register
     */
    virtual void increment_wr25_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr26 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr26 register
     */
    virtual void increment_wr26(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr26_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr26_u register
     */
    virtual void increment_wr26_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr27 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr27 register
     */
    virtual void increment_wr27(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr27_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr27_u register
     */
    virtual void increment_wr27_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr28 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr28 register
     */
    virtual void increment_wr28(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr28_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr28_u register
     */
    virtual void increment_wr28_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr29 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr29 register
     */
    virtual void increment_wr29(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr29_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr29_u register
     */
    virtual void increment_wr29_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr30 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr30 register
     */
    virtual void increment_wr30(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr30_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr30_u register
     */
    virtual void increment_wr30_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr31 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr31 register
     */
    virtual void increment_wr31(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr31_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr31_u register
     */
    virtual void increment_wr31_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 EIPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 EIPC register
     */
    virtual void increment_EIPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 EIPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 EIPSW register
     */
    virtual void increment_EIPSW(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 FEPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FEPC register
     */
    virtual void increment_FEPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 FEPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FEPSW register
     */
    virtual void increment_FEPSW(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PSW_EIMASK register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PSW_EIMASK register field
     */
    virtual void increment_PSW_EIMASK(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FPSR_CC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPSR_CC register field
     */
    virtual void increment_FPSR_CC(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FPSR_RM register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPSR_RM register field
     */
    virtual void increment_FPSR_RM(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FPSR_XC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPSR_XC register field
     */
    virtual void increment_FPSR_XC(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FPSR_XE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPSR_XE register field
     */
    virtual void increment_FPSR_XE(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FPSR_XP register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPSR_XP register field
     */
    virtual void increment_FPSR_XP(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FPEPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPEPC register
     */
    virtual void increment_FPEPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 FPST_XC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPST_XC register field
     */
    virtual void increment_FPST_XC(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FPST_XP register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPST_XP register field
     */
    virtual void increment_FPST_XP(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FPCC_CC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPCC_CC register field
     */
    virtual void increment_FPCC_CC(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FPCFG_RM register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPCFG_RM register field
     */
    virtual void increment_FPCFG_RM(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FPCFG_XE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPCFG_XE register field
     */
    virtual void increment_FPCFG_XE(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 EIIC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 EIIC register
     */
    virtual void increment_EIIC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 FEIC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FEIC register
     */
    virtual void increment_FEIC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PSWH_GPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PSWH_GPID register field
     */
    virtual void increment_PSWH_GPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 CTPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 CTPC register
     */
    virtual void increment_CTPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 CTPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 CTPSW register
     */
    virtual void increment_CTPSW(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 EIPSWH_GPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 EIPSWH_GPID register field
     */
    virtual void increment_EIPSWH_GPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FEPSWH_GPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FEPSWH_GPID register field
     */
    virtual void increment_FEPSWH_GPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 CTBP register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 CTBP register
     */
    virtual void increment_CTBP(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 SNZCFG register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 SNZCFG register
     */
    virtual void increment_SNZCFG(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 EIWR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 EIWR register
     */
    virtual void increment_EIWR(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 FEWR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FEWR register
     */
    virtual void increment_FEWR(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 SPID register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 SPID register
     */
    virtual void increment_SPID(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 SPIDLIST register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 SPIDLIST register
     */
    virtual void increment_SPIDLIST(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 RBASE_RBASE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RBASE_RBASE register field
     */
    virtual void increment_RBASE_RBASE(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 EBASE_EBASE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 EBASE_EBASE register field
     */
    virtual void increment_EBASE_EBASE(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 INTBP register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 INTBP register
     */
    virtual void increment_INTBP(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PID register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PID register
     */
    virtual void increment_PID(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 SVLOCK register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 SVLOCK register
     */
    virtual void increment_SVLOCK(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 SCCFG_SIZE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 SCCFG_SIZE register field
     */
    virtual void increment_SCCFG_SIZE(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 SCBP register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 SCBP register
     */
    virtual void increment_SCBP(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 HVSB register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 HVSB register
     */
    virtual void increment_HVSB(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PEID register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PEID register
     */
    virtual void increment_PEID(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 BMID_BMID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BMID_BMID register field
     */
    virtual void increment_BMID_BMID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MEA register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MEA register
     */
    virtual void increment_MEA(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 MEI register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MEI register
     */
    virtual void increment_MEI(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 RBCR0_BE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RBCR0_BE register field
     */
    virtual void increment_RBCR0_BE(unsigned short value);
    /**
     * \brief Increments the value of the rh850g4 RBCR1_NC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RBCR1_NC register field
     */
    virtual void increment_RBCR1_NC(unsigned short value);
    /**
     * \brief Increments the value of the rh850g4 RBNR_BN register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RBNR_BN register field
     */
    virtual void increment_RBNR_BN(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 RBIP_RBIP register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RBIP_RBIP register field
     */
    virtual void increment_RBIP_RBIP(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 ISPR_ISP register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ISPR_ISP register field
     */
    virtual void increment_ISPR_ISP(unsigned short value);
    /**
     * \brief Increments the value of the rh850g4 INTCFG_ULNR register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 INTCFG_ULNR register field
     */
    virtual void increment_INTCFG_ULNR(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 PLMR_PLM register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PLMR_PLM register field
     */
    virtual void increment_PLMR_PLM(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXSR_RM register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXSR_RM register field
     */
    virtual void increment_FXSR_RM(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXSR_XC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXSR_XC register field
     */
    virtual void increment_FXSR_XC(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXSR_XE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXSR_XE register field
     */
    virtual void increment_FXSR_XE(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXSR_XP register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXSR_XP register field
     */
    virtual void increment_FXSR_XP(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXST_XC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXST_XC register field
     */
    virtual void increment_FXST_XC(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXST_XP register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXST_XP register field
     */
    virtual void increment_FXST_XP(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXCFG_RM register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXCFG_RM register field
     */
    virtual void increment_FXCFG_RM(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXCFG_XE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXCFG_XE register field
     */
    virtual void increment_FXCFG_XE(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXXC_XC3 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXC_XC3 register field
     */
    virtual void increment_FXXC_XC3(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXXC_XC2 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXC_XC2 register field
     */
    virtual void increment_FXXC_XC2(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXXC_XC1 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXC_XC1 register field
     */
    virtual void increment_FXXC_XC1(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXXC_XC0 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXC_XC0 register field
     */
    virtual void increment_FXXC_XC0(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXXP_XP3 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXP_XP3 register field
     */
    virtual void increment_FXXP_XP3(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXXP_XP2 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXP_XP2 register field
     */
    virtual void increment_FXXP_XP2(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXXP_XP1 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXP_XP1 register field
     */
    virtual void increment_FXXP_XP1(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXXP_XP0 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXP_XP0 register field
     */
    virtual void increment_FXXP_XP0(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 GMEIPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMEIPC register
     */
    virtual void increment_GMEIPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMEIPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMEIPSW register
     */
    virtual void increment_GMEIPSW(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMFEPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMFEPC register
     */
    virtual void increment_GMFEPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMFEPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMFEPSW register
     */
    virtual void increment_GMFEPSW(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMPSW_EIMASK register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMPSW_EIMASK register field
     */
    virtual void increment_GMPSW_EIMASK(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 GMMEA register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMMEA register
     */
    virtual void increment_GMMEA(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMMEI register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMMEI register
     */
    virtual void increment_GMMEI(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMEIIC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMEIIC register
     */
    virtual void increment_GMEIIC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMFEIC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMFEIC register
     */
    virtual void increment_GMFEIC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMSPID register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMSPID register
     */
    virtual void increment_GMSPID(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMSPIDLIST register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMSPIDLIST register
     */
    virtual void increment_GMSPIDLIST(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMEBASE_EBASE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMEBASE_EBASE register field
     */
    virtual void increment_GMEBASE_EBASE(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMINTBP register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMINTBP register
     */
    virtual void increment_GMINTBP(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMINTCFG_ULNR register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMINTCFG_ULNR register field
     */
    virtual void increment_GMINTCFG_ULNR(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 GMPLMR_PLM register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMPLMR_PLM register field
     */
    virtual void increment_GMPLMR_PLM(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 GMSVLOCK register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMSVLOCK register
     */
    virtual void increment_GMSVLOCK(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMMPM register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMMPM register
     */
    virtual void increment_GMMPM(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMEIWR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMEIWR register
     */
    virtual void increment_GMEIWR(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMFEWR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMFEWR register
     */
    virtual void increment_GMFEWR(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMPEID register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMPEID register
     */
    virtual void increment_GMPEID(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 MPM register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPM register
     */
    virtual void increment_MPM(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 MPCFG_NMPUE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPCFG_NMPUE register field
     */
    virtual void increment_MPCFG_NMPUE(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPCFG_HBE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPCFG_HBE register field
     */
    virtual void increment_MPCFG_HBE(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPCFG_ARCH register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPCFG_ARCH register field
     */
    virtual void increment_MPCFG_ARCH(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPCFG_NBK register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPCFG_NBK register field
     */
    virtual void increment_MPCFG_NBK(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MCA register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MCA register
     */
    virtual void increment_MCA(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 MCS register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MCS register
     */
    virtual void increment_MCS(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 MCC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MCC register
     */
    virtual void increment_MCC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 MCI_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MCI_SPID register field
     */
    virtual void increment_MCI_SPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPIDX_IDX register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPIDX_IDX register field
     */
    virtual void increment_MPIDX_IDX(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPBK_BK register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPBK_BK register field
     */
    virtual void increment_MPBK_BK(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPLA register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPLA register
     */
    virtual void increment_MPLA(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 MPUA register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPUA register
     */
    virtual void increment_MPUA(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 MPID0_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID0_SPID register field
     */
    virtual void increment_MPID0_SPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPID1_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID1_SPID register field
     */
    virtual void increment_MPID1_SPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPID2_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID2_SPID register field
     */
    virtual void increment_MPID2_SPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPID3_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID3_SPID register field
     */
    virtual void increment_MPID3_SPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPID4_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID4_SPID register field
     */
    virtual void increment_MPID4_SPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPID5_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID5_SPID register field
     */
    virtual void increment_MPID5_SPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPID6_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID6_SPID register field
     */
    virtual void increment_MPID6_SPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPID7_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID7_SPID register field
     */
    virtual void increment_MPID7_SPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 ICTAGL_LPN register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICTAGL_LPN register field
     */
    virtual void increment_ICTAGL_LPN(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 ICTAGL_LRU register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICTAGL_LRU register field
     */
    virtual void increment_ICTAGL_LRU(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 ICTAGH_DATAECC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICTAGH_DATAECC register field
     */
    virtual void increment_ICTAGH_DATAECC(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 ICTAGH_TAGECC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICTAGH_TAGECC register field
     */
    virtual void increment_ICTAGH_TAGECC(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 ICDATL register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICDATL register
     */
    virtual void increment_ICDATL(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 ICDATH register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICDATH register
     */
    virtual void increment_ICDATH(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 ICCFG_ICHSIZE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICCFG_ICHSIZE register field
     */
    virtual void increment_ICCFG_ICHSIZE(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 ICCFG_ICHLINE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICCFG_ICHLINE register field
     */
    virtual void increment_ICCFG_ICHLINE(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 ICCFG_ICHWAY register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICCFG_ICHWAY register field
     */
    virtual void increment_ICCFG_ICHWAY(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 ICERR_ICHEWY register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICERR_ICHEWY register field
     */
    virtual void increment_ICERR_ICHEWY(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 ICERR_ICHEIX register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICERR_ICHEIX register field
     */
    virtual void increment_ICERR_ICHEIX(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 TSCOUNTL register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 TSCOUNTL register
     */
    virtual void increment_TSCOUNTL(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 TSCOUNTH register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 TSCOUNTH register
     */
    virtual void increment_TSCOUNTH(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 TSCTRL register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 TSCTRL register
     */
    virtual void increment_TSCTRL(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMUMCTRL register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMUMCTRL register
     */
    virtual void increment_PMUMCTRL(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMGMCTRL register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMGMCTRL register
     */
    virtual void increment_PMGMCTRL(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT0 register
     */
    virtual void increment_PMCOUNT0(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCTRL0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL0 register
     */
    virtual void increment_PMCTRL0(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT1 register
     */
    virtual void increment_PMCOUNT1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCTRL1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL1 register
     */
    virtual void increment_PMCTRL1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT2 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT2 register
     */
    virtual void increment_PMCOUNT2(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCTRL2 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL2 register
     */
    virtual void increment_PMCTRL2(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT3 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT3 register
     */
    virtual void increment_PMCOUNT3(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCTRL3 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL3 register
     */
    virtual void increment_PMCTRL3(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCTRL4 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL4 register
     */
    virtual void increment_PMCTRL4(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCTRL5 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL5 register
     */
    virtual void increment_PMCTRL5(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCTRL6 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL6 register
     */
    virtual void increment_PMCTRL6(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCTRL7 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL7 register
     */
    virtual void increment_PMCTRL7(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT4 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT4 register
     */
    virtual void increment_PMCOUNT4(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT5 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT5 register
     */
    virtual void increment_PMCOUNT5(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT6 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT6 register
     */
    virtual void increment_PMCOUNT6(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT7 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT7 register
     */
    virtual void increment_PMCOUNT7(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND0 register
     */
    virtual void increment_PMSUBCND0(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND1 register
     */
    virtual void increment_PMSUBCND1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND2 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND2 register
     */
    virtual void increment_PMSUBCND2(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND3 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND3 register
     */
    virtual void increment_PMSUBCND3(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND4 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND4 register
     */
    virtual void increment_PMSUBCND4(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND5 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND5 register
     */
    virtual void increment_PMSUBCND5(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND6 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND6 register
     */
    virtual void increment_PMSUBCND6(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND7 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND7 register
     */
    virtual void increment_PMSUBCND7(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DBGEN register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DBGEN register
     */
    virtual void increment_DBGEN(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DBPSWH register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DBPSWH register
     */
    virtual void increment_DBPSWH(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DBIC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DBIC register
     */
    virtual void increment_DBIC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DBPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DBPC register
     */
    virtual void increment_DBPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DBPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DBPSW register
     */
    virtual void increment_DBPSW(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DIR0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DIR0 register
     */
    virtual void increment_DIR0(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DIR1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DIR1 register
     */
    virtual void increment_DIR1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 BPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BPC register
     */
    virtual void increment_BPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 BPAV register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BPAV register
     */
    virtual void increment_BPAV(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 BPAM register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BPAM register
     */
    virtual void increment_BPAM(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DBCFG register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DBCFG register
     */
    virtual void increment_DBCFG(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DBWR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DBWR register
     */
    virtual void increment_DBWR(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 LSTEST0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 LSTEST0 register
     */
    virtual void increment_LSTEST0(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 LSTEST1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 LSTEST1 register
     */
    virtual void increment_LSTEST1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 LSCFG register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 LSCFG register
     */
    virtual void increment_LSCFG(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 ICBKEY register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICBKEY register
     */
    virtual void increment_ICBKEY(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 IFCR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 IFCR register
     */
    virtual void increment_IFCR(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 IFCR1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 IFCR1 register
     */
    virtual void increment_IFCR1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 BRPCTRL0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BRPCTRL0 register
     */
    virtual void increment_BRPCTRL0(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 BRPCTRL1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BRPCTRL1 register
     */
    virtual void increment_BRPCTRL1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 BRPCFG register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BRPCFG register
     */
    virtual void increment_BRPCFG(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 BRPACTRL register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BRPACTRL register
     */
    virtual void increment_BRPACTRL(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 BRPDATA register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BRPDATA register
     */
    virtual void increment_BRPDATA(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DCBKEY register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DCBKEY register
     */
    virtual void increment_DCBKEY(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 LSUCR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 LSUCR register
     */
    virtual void increment_LSUCR(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 LSULNK0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 LSULNK0 register
     */
    virtual void increment_LSULNK0(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 LSULNK1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 LSULNK1 register
     */
    virtual void increment_LSULNK1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 L1RLCR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 L1RLCR register
     */
    virtual void increment_L1RLCR(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 L1RLNK0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 L1RLNK0 register
     */
    virtual void increment_L1RLNK0(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 L1RLNK1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 L1RLNK1 register
     */
    virtual void increment_L1RLNK1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 L1RCFG register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 L1RCFG register
     */
    virtual void increment_L1RCFG(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DECFG register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DECFG register
     */
    virtual void increment_DECFG(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DECTRL register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DECTRL register
     */
    virtual void increment_DECTRL(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DEVDS register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DEVDS register
     */
    virtual void increment_DEVDS(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 RDBCR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBCR register
     */
    virtual void increment_RDBCR(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 RDBACR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBACR register
     */
    virtual void increment_RDBACR(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 RDBATAG register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBATAG register
     */
    virtual void increment_RDBATAG(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 RDBADAT0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBADAT0 register
     */
    virtual void increment_RDBADAT0(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 RDBADAT1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBADAT1 register
     */
    virtual void increment_RDBADAT1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 RDBADAT2 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBADAT2 register
     */
    virtual void increment_RDBADAT2(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 RDBADAT3 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBADAT3 register
     */
    virtual void increment_RDBADAT3(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 RDBSTAT register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBSTAT register
     */
    virtual void increment_RDBSTAT(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 hv_func_support register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 hv_func_support register
     */
    virtual void increment_hv_func_support(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 swd0pcad_exec_count register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 swd0pcad_exec_count register
     */
    virtual void increment_swd0pcad_exec_count(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 swd0pcad_val register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 swd0pcad_val register
     */
    virtual void increment_swd0pcad_val(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 mem_acc_type register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 mem_acc_type register
     */
    virtual void increment_mem_acc_type(unsigned int value);

    // immediate register group increment operations
    /**
     * \brief Increments the value of the specified register from the rh850g4 GR register group
     * \param index Index value of the required register from the rh850g4 GR register group
     * \param value Amount by which to increment the value of the specified rh850g4 GR group register
     */
    virtual void increment_GR(rh850g4::GR_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 WR register group
     * \param index Index value of the required register from the rh850g4 WR register group
     * \param value Amount by which to increment the value of the specified rh850g4 WR group register
     */
    virtual void increment_WR(rh850g4::WR_index_t index, unsigned long long value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_0 group register
     */
    virtual void increment_SR_SELID_0(rh850g4::SR_SELID_0_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_1 group register
     */
    virtual void increment_SR_SELID_1(rh850g4::SR_SELID_1_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_2 group register
     */
    virtual void increment_SR_SELID_2(rh850g4::SR_SELID_2_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_3 group register
     */
    virtual void increment_SR_SELID_3(rh850g4::SR_SELID_3_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_4 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_4 group register
     */
    virtual void increment_SR_SELID_4(rh850g4::SR_SELID_4_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_5 group register
     */
    virtual void increment_SR_SELID_5(rh850g4::SR_SELID_5_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_9 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_9 group register
     */
    virtual void increment_SR_SELID_9(rh850g4::SR_SELID_9_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_10 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_10 group register
     */
    virtual void increment_SR_SELID_10(rh850g4::SR_SELID_10_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_11 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_11 group register
     */
    virtual void increment_SR_SELID_11(rh850g4::SR_SELID_11_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_12 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_12 group register
     */
    virtual void increment_SR_SELID_12(rh850g4::SR_SELID_12_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_13 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_13 group register
     */
    virtual void increment_SR_SELID_13(rh850g4::SR_SELID_13_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_14 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_14 group register
     */
    virtual void increment_SR_SELID_14(rh850g4::SR_SELID_14_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_15 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_15 group register
     */
    virtual void increment_SR_SELID_15(rh850g4::SR_SELID_15_index_t index, unsigned int value);

    // immediate register and register field decrement operations
    /**
     * \brief Decrements the value of the rh850g4 translation time PC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 translation time PC register
     */
    virtual void decrement_trans_PC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r0 register
     */
    virtual void decrement_r0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r1 register
     */
    virtual void decrement_r1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r2 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r2 register
     */
    virtual void decrement_r2(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r3 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r3 register
     */
    virtual void decrement_r3(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r4 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r4 register
     */
    virtual void decrement_r4(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r5 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r5 register
     */
    virtual void decrement_r5(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r6 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r6 register
     */
    virtual void decrement_r6(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r7 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r7 register
     */
    virtual void decrement_r7(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r8 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r8 register
     */
    virtual void decrement_r8(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r9 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r9 register
     */
    virtual void decrement_r9(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r10 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r10 register
     */
    virtual void decrement_r10(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r11 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r11 register
     */
    virtual void decrement_r11(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r12 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r12 register
     */
    virtual void decrement_r12(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r13 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r13 register
     */
    virtual void decrement_r13(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r14 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r14 register
     */
    virtual void decrement_r14(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r15 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r15 register
     */
    virtual void decrement_r15(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r16 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r16 register
     */
    virtual void decrement_r16(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r17 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r17 register
     */
    virtual void decrement_r17(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r18 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r18 register
     */
    virtual void decrement_r18(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r19 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r19 register
     */
    virtual void decrement_r19(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r20 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r20 register
     */
    virtual void decrement_r20(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r21 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r21 register
     */
    virtual void decrement_r21(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r22 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r22 register
     */
    virtual void decrement_r22(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r23 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r23 register
     */
    virtual void decrement_r23(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r24 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r24 register
     */
    virtual void decrement_r24(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r25 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r25 register
     */
    virtual void decrement_r25(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r26 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r26 register
     */
    virtual void decrement_r26(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r27 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r27 register
     */
    virtual void decrement_r27(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r28 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r28 register
     */
    virtual void decrement_r28(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r29 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r29 register
     */
    virtual void decrement_r29(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r30 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r30 register
     */
    virtual void decrement_r30(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r31 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r31 register
     */
    virtual void decrement_r31(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PC register
     */
    virtual void decrement_PC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 wr0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr0 register
     */
    virtual void decrement_wr0(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr0_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr0_u register
     */
    virtual void decrement_wr0_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr1 register
     */
    virtual void decrement_wr1(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr1_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr1_u register
     */
    virtual void decrement_wr1_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr2 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr2 register
     */
    virtual void decrement_wr2(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr2_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr2_u register
     */
    virtual void decrement_wr2_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr3 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr3 register
     */
    virtual void decrement_wr3(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr3_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr3_u register
     */
    virtual void decrement_wr3_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr4 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr4 register
     */
    virtual void decrement_wr4(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr4_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr4_u register
     */
    virtual void decrement_wr4_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr5 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr5 register
     */
    virtual void decrement_wr5(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr5_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr5_u register
     */
    virtual void decrement_wr5_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr6 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr6 register
     */
    virtual void decrement_wr6(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr6_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr6_u register
     */
    virtual void decrement_wr6_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr7 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr7 register
     */
    virtual void decrement_wr7(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr7_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr7_u register
     */
    virtual void decrement_wr7_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr8 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr8 register
     */
    virtual void decrement_wr8(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr8_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr8_u register
     */
    virtual void decrement_wr8_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr9 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr9 register
     */
    virtual void decrement_wr9(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr9_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr9_u register
     */
    virtual void decrement_wr9_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr10 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr10 register
     */
    virtual void decrement_wr10(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr10_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr10_u register
     */
    virtual void decrement_wr10_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr11 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr11 register
     */
    virtual void decrement_wr11(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr11_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr11_u register
     */
    virtual void decrement_wr11_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr12 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr12 register
     */
    virtual void decrement_wr12(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr12_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr12_u register
     */
    virtual void decrement_wr12_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr13 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr13 register
     */
    virtual void decrement_wr13(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr13_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr13_u register
     */
    virtual void decrement_wr13_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr14 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr14 register
     */
    virtual void decrement_wr14(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr14_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr14_u register
     */
    virtual void decrement_wr14_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr15 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr15 register
     */
    virtual void decrement_wr15(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr15_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr15_u register
     */
    virtual void decrement_wr15_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr16 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr16 register
     */
    virtual void decrement_wr16(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr16_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr16_u register
     */
    virtual void decrement_wr16_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr17 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr17 register
     */
    virtual void decrement_wr17(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr17_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr17_u register
     */
    virtual void decrement_wr17_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr18 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr18 register
     */
    virtual void decrement_wr18(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr18_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr18_u register
     */
    virtual void decrement_wr18_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr19 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr19 register
     */
    virtual void decrement_wr19(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr19_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr19_u register
     */
    virtual void decrement_wr19_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr20 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr20 register
     */
    virtual void decrement_wr20(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr20_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr20_u register
     */
    virtual void decrement_wr20_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr21 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr21 register
     */
    virtual void decrement_wr21(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr21_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr21_u register
     */
    virtual void decrement_wr21_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr22 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr22 register
     */
    virtual void decrement_wr22(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr22_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr22_u register
     */
    virtual void decrement_wr22_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr23 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr23 register
     */
    virtual void decrement_wr23(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr23_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr23_u register
     */
    virtual void decrement_wr23_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr24 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr24 register
     */
    virtual void decrement_wr24(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr24_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr24_u register
     */
    virtual void decrement_wr24_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr25 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr25 register
     */
    virtual void decrement_wr25(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr25_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr25_u register
     */
    virtual void decrement_wr25_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr26 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr26 register
     */
    virtual void decrement_wr26(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr26_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr26_u register
     */
    virtual void decrement_wr26_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr27 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr27 register
     */
    virtual void decrement_wr27(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr27_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr27_u register
     */
    virtual void decrement_wr27_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr28 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr28 register
     */
    virtual void decrement_wr28(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr28_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr28_u register
     */
    virtual void decrement_wr28_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr29 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr29 register
     */
    virtual void decrement_wr29(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr29_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr29_u register
     */
    virtual void decrement_wr29_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr30 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr30 register
     */
    virtual void decrement_wr30(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr30_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr30_u register
     */
    virtual void decrement_wr30_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr31 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr31 register
     */
    virtual void decrement_wr31(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr31_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr31_u register
     */
    virtual void decrement_wr31_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 EIPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 EIPC register
     */
    virtual void decrement_EIPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 EIPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 EIPSW register
     */
    virtual void decrement_EIPSW(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 FEPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FEPC register
     */
    virtual void decrement_FEPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 FEPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FEPSW register
     */
    virtual void decrement_FEPSW(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PSW_EIMASK register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PSW_EIMASK register field
     */
    virtual void decrement_PSW_EIMASK(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FPSR_CC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPSR_CC register field
     */
    virtual void decrement_FPSR_CC(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FPSR_RM register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPSR_RM register field
     */
    virtual void decrement_FPSR_RM(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FPSR_XC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPSR_XC register field
     */
    virtual void decrement_FPSR_XC(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FPSR_XE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPSR_XE register field
     */
    virtual void decrement_FPSR_XE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FPSR_XP register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPSR_XP register field
     */
    virtual void decrement_FPSR_XP(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FPEPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPEPC register
     */
    virtual void decrement_FPEPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 FPST_XC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPST_XC register field
     */
    virtual void decrement_FPST_XC(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FPST_XP register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPST_XP register field
     */
    virtual void decrement_FPST_XP(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FPCC_CC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPCC_CC register field
     */
    virtual void decrement_FPCC_CC(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FPCFG_RM register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPCFG_RM register field
     */
    virtual void decrement_FPCFG_RM(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FPCFG_XE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPCFG_XE register field
     */
    virtual void decrement_FPCFG_XE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 EIIC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 EIIC register
     */
    virtual void decrement_EIIC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 FEIC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FEIC register
     */
    virtual void decrement_FEIC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PSWH_GPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PSWH_GPID register field
     */
    virtual void decrement_PSWH_GPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 CTPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 CTPC register
     */
    virtual void decrement_CTPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 CTPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 CTPSW register
     */
    virtual void decrement_CTPSW(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 EIPSWH_GPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 EIPSWH_GPID register field
     */
    virtual void decrement_EIPSWH_GPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FEPSWH_GPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FEPSWH_GPID register field
     */
    virtual void decrement_FEPSWH_GPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 CTBP register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 CTBP register
     */
    virtual void decrement_CTBP(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 SNZCFG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 SNZCFG register
     */
    virtual void decrement_SNZCFG(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 EIWR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 EIWR register
     */
    virtual void decrement_EIWR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 FEWR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FEWR register
     */
    virtual void decrement_FEWR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 SPID register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 SPID register
     */
    virtual void decrement_SPID(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 SPIDLIST register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 SPIDLIST register
     */
    virtual void decrement_SPIDLIST(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 RBASE_RBASE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RBASE_RBASE register field
     */
    virtual void decrement_RBASE_RBASE(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 EBASE_EBASE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 EBASE_EBASE register field
     */
    virtual void decrement_EBASE_EBASE(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 INTBP register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 INTBP register
     */
    virtual void decrement_INTBP(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PID register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PID register
     */
    virtual void decrement_PID(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 SVLOCK register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 SVLOCK register
     */
    virtual void decrement_SVLOCK(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 SCCFG_SIZE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 SCCFG_SIZE register field
     */
    virtual void decrement_SCCFG_SIZE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 SCBP register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 SCBP register
     */
    virtual void decrement_SCBP(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 HVSB register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 HVSB register
     */
    virtual void decrement_HVSB(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PEID register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PEID register
     */
    virtual void decrement_PEID(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 BMID_BMID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BMID_BMID register field
     */
    virtual void decrement_BMID_BMID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MEA register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MEA register
     */
    virtual void decrement_MEA(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 MEI register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MEI register
     */
    virtual void decrement_MEI(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 RBCR0_BE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RBCR0_BE register field
     */
    virtual void decrement_RBCR0_BE(unsigned short value);
    /**
     * \brief Decrements the value of the rh850g4 RBCR1_NC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RBCR1_NC register field
     */
    virtual void decrement_RBCR1_NC(unsigned short value);
    /**
     * \brief Decrements the value of the rh850g4 RBNR_BN register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RBNR_BN register field
     */
    virtual void decrement_RBNR_BN(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 RBIP_RBIP register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RBIP_RBIP register field
     */
    virtual void decrement_RBIP_RBIP(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 ISPR_ISP register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ISPR_ISP register field
     */
    virtual void decrement_ISPR_ISP(unsigned short value);
    /**
     * \brief Decrements the value of the rh850g4 INTCFG_ULNR register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 INTCFG_ULNR register field
     */
    virtual void decrement_INTCFG_ULNR(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 PLMR_PLM register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PLMR_PLM register field
     */
    virtual void decrement_PLMR_PLM(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXSR_RM register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXSR_RM register field
     */
    virtual void decrement_FXSR_RM(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXSR_XC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXSR_XC register field
     */
    virtual void decrement_FXSR_XC(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXSR_XE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXSR_XE register field
     */
    virtual void decrement_FXSR_XE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXSR_XP register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXSR_XP register field
     */
    virtual void decrement_FXSR_XP(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXST_XC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXST_XC register field
     */
    virtual void decrement_FXST_XC(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXST_XP register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXST_XP register field
     */
    virtual void decrement_FXST_XP(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXCFG_RM register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXCFG_RM register field
     */
    virtual void decrement_FXCFG_RM(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXCFG_XE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXCFG_XE register field
     */
    virtual void decrement_FXCFG_XE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXXC_XC3 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXC_XC3 register field
     */
    virtual void decrement_FXXC_XC3(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXXC_XC2 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXC_XC2 register field
     */
    virtual void decrement_FXXC_XC2(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXXC_XC1 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXC_XC1 register field
     */
    virtual void decrement_FXXC_XC1(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXXC_XC0 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXC_XC0 register field
     */
    virtual void decrement_FXXC_XC0(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXXP_XP3 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXP_XP3 register field
     */
    virtual void decrement_FXXP_XP3(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXXP_XP2 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXP_XP2 register field
     */
    virtual void decrement_FXXP_XP2(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXXP_XP1 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXP_XP1 register field
     */
    virtual void decrement_FXXP_XP1(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXXP_XP0 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXP_XP0 register field
     */
    virtual void decrement_FXXP_XP0(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 GMEIPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMEIPC register
     */
    virtual void decrement_GMEIPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMEIPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMEIPSW register
     */
    virtual void decrement_GMEIPSW(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMFEPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMFEPC register
     */
    virtual void decrement_GMFEPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMFEPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMFEPSW register
     */
    virtual void decrement_GMFEPSW(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMPSW_EIMASK register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMPSW_EIMASK register field
     */
    virtual void decrement_GMPSW_EIMASK(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 GMMEA register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMMEA register
     */
    virtual void decrement_GMMEA(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMMEI register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMMEI register
     */
    virtual void decrement_GMMEI(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMEIIC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMEIIC register
     */
    virtual void decrement_GMEIIC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMFEIC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMFEIC register
     */
    virtual void decrement_GMFEIC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMSPID register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMSPID register
     */
    virtual void decrement_GMSPID(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMSPIDLIST register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMSPIDLIST register
     */
    virtual void decrement_GMSPIDLIST(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMEBASE_EBASE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMEBASE_EBASE register field
     */
    virtual void decrement_GMEBASE_EBASE(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMINTBP register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMINTBP register
     */
    virtual void decrement_GMINTBP(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMINTCFG_ULNR register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMINTCFG_ULNR register field
     */
    virtual void decrement_GMINTCFG_ULNR(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 GMPLMR_PLM register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMPLMR_PLM register field
     */
    virtual void decrement_GMPLMR_PLM(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 GMSVLOCK register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMSVLOCK register
     */
    virtual void decrement_GMSVLOCK(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMMPM register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMMPM register
     */
    virtual void decrement_GMMPM(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMEIWR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMEIWR register
     */
    virtual void decrement_GMEIWR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMFEWR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMFEWR register
     */
    virtual void decrement_GMFEWR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMPEID register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMPEID register
     */
    virtual void decrement_GMPEID(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 MPM register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPM register
     */
    virtual void decrement_MPM(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 MPCFG_NMPUE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPCFG_NMPUE register field
     */
    virtual void decrement_MPCFG_NMPUE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPCFG_HBE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPCFG_HBE register field
     */
    virtual void decrement_MPCFG_HBE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPCFG_ARCH register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPCFG_ARCH register field
     */
    virtual void decrement_MPCFG_ARCH(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPCFG_NBK register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPCFG_NBK register field
     */
    virtual void decrement_MPCFG_NBK(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MCA register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MCA register
     */
    virtual void decrement_MCA(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 MCS register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MCS register
     */
    virtual void decrement_MCS(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 MCC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MCC register
     */
    virtual void decrement_MCC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 MCI_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MCI_SPID register field
     */
    virtual void decrement_MCI_SPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPIDX_IDX register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPIDX_IDX register field
     */
    virtual void decrement_MPIDX_IDX(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPBK_BK register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPBK_BK register field
     */
    virtual void decrement_MPBK_BK(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPLA register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPLA register
     */
    virtual void decrement_MPLA(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 MPUA register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPUA register
     */
    virtual void decrement_MPUA(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 MPID0_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID0_SPID register field
     */
    virtual void decrement_MPID0_SPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPID1_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID1_SPID register field
     */
    virtual void decrement_MPID1_SPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPID2_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID2_SPID register field
     */
    virtual void decrement_MPID2_SPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPID3_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID3_SPID register field
     */
    virtual void decrement_MPID3_SPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPID4_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID4_SPID register field
     */
    virtual void decrement_MPID4_SPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPID5_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID5_SPID register field
     */
    virtual void decrement_MPID5_SPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPID6_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID6_SPID register field
     */
    virtual void decrement_MPID6_SPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPID7_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID7_SPID register field
     */
    virtual void decrement_MPID7_SPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 ICTAGL_LPN register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICTAGL_LPN register field
     */
    virtual void decrement_ICTAGL_LPN(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 ICTAGL_LRU register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICTAGL_LRU register field
     */
    virtual void decrement_ICTAGL_LRU(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 ICTAGH_DATAECC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICTAGH_DATAECC register field
     */
    virtual void decrement_ICTAGH_DATAECC(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 ICTAGH_TAGECC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICTAGH_TAGECC register field
     */
    virtual void decrement_ICTAGH_TAGECC(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 ICDATL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICDATL register
     */
    virtual void decrement_ICDATL(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 ICDATH register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICDATH register
     */
    virtual void decrement_ICDATH(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 ICCFG_ICHSIZE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICCFG_ICHSIZE register field
     */
    virtual void decrement_ICCFG_ICHSIZE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 ICCFG_ICHLINE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICCFG_ICHLINE register field
     */
    virtual void decrement_ICCFG_ICHLINE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 ICCFG_ICHWAY register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICCFG_ICHWAY register field
     */
    virtual void decrement_ICCFG_ICHWAY(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 ICERR_ICHEWY register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICERR_ICHEWY register field
     */
    virtual void decrement_ICERR_ICHEWY(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 ICERR_ICHEIX register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICERR_ICHEIX register field
     */
    virtual void decrement_ICERR_ICHEIX(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 TSCOUNTL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 TSCOUNTL register
     */
    virtual void decrement_TSCOUNTL(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 TSCOUNTH register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 TSCOUNTH register
     */
    virtual void decrement_TSCOUNTH(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 TSCTRL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 TSCTRL register
     */
    virtual void decrement_TSCTRL(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMUMCTRL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMUMCTRL register
     */
    virtual void decrement_PMUMCTRL(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMGMCTRL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMGMCTRL register
     */
    virtual void decrement_PMGMCTRL(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT0 register
     */
    virtual void decrement_PMCOUNT0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL0 register
     */
    virtual void decrement_PMCTRL0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT1 register
     */
    virtual void decrement_PMCOUNT1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL1 register
     */
    virtual void decrement_PMCTRL1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT2 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT2 register
     */
    virtual void decrement_PMCOUNT2(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL2 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL2 register
     */
    virtual void decrement_PMCTRL2(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT3 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT3 register
     */
    virtual void decrement_PMCOUNT3(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL3 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL3 register
     */
    virtual void decrement_PMCTRL3(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL4 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL4 register
     */
    virtual void decrement_PMCTRL4(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL5 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL5 register
     */
    virtual void decrement_PMCTRL5(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL6 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL6 register
     */
    virtual void decrement_PMCTRL6(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL7 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL7 register
     */
    virtual void decrement_PMCTRL7(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT4 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT4 register
     */
    virtual void decrement_PMCOUNT4(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT5 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT5 register
     */
    virtual void decrement_PMCOUNT5(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT6 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT6 register
     */
    virtual void decrement_PMCOUNT6(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT7 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT7 register
     */
    virtual void decrement_PMCOUNT7(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND0 register
     */
    virtual void decrement_PMSUBCND0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND1 register
     */
    virtual void decrement_PMSUBCND1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND2 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND2 register
     */
    virtual void decrement_PMSUBCND2(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND3 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND3 register
     */
    virtual void decrement_PMSUBCND3(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND4 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND4 register
     */
    virtual void decrement_PMSUBCND4(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND5 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND5 register
     */
    virtual void decrement_PMSUBCND5(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND6 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND6 register
     */
    virtual void decrement_PMSUBCND6(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND7 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND7 register
     */
    virtual void decrement_PMSUBCND7(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DBGEN register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DBGEN register
     */
    virtual void decrement_DBGEN(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DBPSWH register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DBPSWH register
     */
    virtual void decrement_DBPSWH(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DBIC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DBIC register
     */
    virtual void decrement_DBIC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DBPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DBPC register
     */
    virtual void decrement_DBPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DBPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DBPSW register
     */
    virtual void decrement_DBPSW(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DIR0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DIR0 register
     */
    virtual void decrement_DIR0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DIR1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DIR1 register
     */
    virtual void decrement_DIR1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 BPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BPC register
     */
    virtual void decrement_BPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 BPAV register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BPAV register
     */
    virtual void decrement_BPAV(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 BPAM register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BPAM register
     */
    virtual void decrement_BPAM(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DBCFG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DBCFG register
     */
    virtual void decrement_DBCFG(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DBWR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DBWR register
     */
    virtual void decrement_DBWR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 LSTEST0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 LSTEST0 register
     */
    virtual void decrement_LSTEST0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 LSTEST1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 LSTEST1 register
     */
    virtual void decrement_LSTEST1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 LSCFG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 LSCFG register
     */
    virtual void decrement_LSCFG(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 ICBKEY register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICBKEY register
     */
    virtual void decrement_ICBKEY(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 IFCR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 IFCR register
     */
    virtual void decrement_IFCR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 IFCR1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 IFCR1 register
     */
    virtual void decrement_IFCR1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 BRPCTRL0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BRPCTRL0 register
     */
    virtual void decrement_BRPCTRL0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 BRPCTRL1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BRPCTRL1 register
     */
    virtual void decrement_BRPCTRL1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 BRPCFG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BRPCFG register
     */
    virtual void decrement_BRPCFG(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 BRPACTRL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BRPACTRL register
     */
    virtual void decrement_BRPACTRL(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 BRPDATA register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BRPDATA register
     */
    virtual void decrement_BRPDATA(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DCBKEY register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DCBKEY register
     */
    virtual void decrement_DCBKEY(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 LSUCR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 LSUCR register
     */
    virtual void decrement_LSUCR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 LSULNK0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 LSULNK0 register
     */
    virtual void decrement_LSULNK0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 LSULNK1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 LSULNK1 register
     */
    virtual void decrement_LSULNK1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 L1RLCR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 L1RLCR register
     */
    virtual void decrement_L1RLCR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 L1RLNK0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 L1RLNK0 register
     */
    virtual void decrement_L1RLNK0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 L1RLNK1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 L1RLNK1 register
     */
    virtual void decrement_L1RLNK1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 L1RCFG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 L1RCFG register
     */
    virtual void decrement_L1RCFG(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DECFG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DECFG register
     */
    virtual void decrement_DECFG(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DECTRL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DECTRL register
     */
    virtual void decrement_DECTRL(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DEVDS register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DEVDS register
     */
    virtual void decrement_DEVDS(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 RDBCR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBCR register
     */
    virtual void decrement_RDBCR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 RDBACR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBACR register
     */
    virtual void decrement_RDBACR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 RDBATAG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBATAG register
     */
    virtual void decrement_RDBATAG(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 RDBADAT0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBADAT0 register
     */
    virtual void decrement_RDBADAT0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 RDBADAT1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBADAT1 register
     */
    virtual void decrement_RDBADAT1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 RDBADAT2 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBADAT2 register
     */
    virtual void decrement_RDBADAT2(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 RDBADAT3 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBADAT3 register
     */
    virtual void decrement_RDBADAT3(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 RDBSTAT register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBSTAT register
     */
    virtual void decrement_RDBSTAT(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 hv_func_support register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 hv_func_support register
     */
    virtual void decrement_hv_func_support(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 swd0pcad_exec_count register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 swd0pcad_exec_count register
     */
    virtual void decrement_swd0pcad_exec_count(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 swd0pcad_val register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 swd0pcad_val register
     */
    virtual void decrement_swd0pcad_val(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 mem_acc_type register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 mem_acc_type register
     */
    virtual void decrement_mem_acc_type(unsigned int value);

    // immediate register group decrement operations
    /**
     * \brief Decrements the value of the specified register from the rh850g4 GR register group
     * \param index Index value of the required register from the rh850g4 GR register group
     * \param value Amount by which to decrement the value of the specified rh850g4 GR group register
     */
    virtual void decrement_GR(rh850g4::GR_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 WR register group
     * \param index Index value of the required register from the rh850g4 WR register group
     * \param value Amount by which to decrement the value of the specified rh850g4 WR group register
     */
    virtual void decrement_WR(rh850g4::WR_index_t index, unsigned long long value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_0 group register
     */
    virtual void decrement_SR_SELID_0(rh850g4::SR_SELID_0_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_1 group register
     */
    virtual void decrement_SR_SELID_1(rh850g4::SR_SELID_1_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_2 group register
     */
    virtual void decrement_SR_SELID_2(rh850g4::SR_SELID_2_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_3 group register
     */
    virtual void decrement_SR_SELID_3(rh850g4::SR_SELID_3_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_4 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_4 group register
     */
    virtual void decrement_SR_SELID_4(rh850g4::SR_SELID_4_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_5 group register
     */
    virtual void decrement_SR_SELID_5(rh850g4::SR_SELID_5_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_9 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_9 group register
     */
    virtual void decrement_SR_SELID_9(rh850g4::SR_SELID_9_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_10 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_10 group register
     */
    virtual void decrement_SR_SELID_10(rh850g4::SR_SELID_10_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_11 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_11 group register
     */
    virtual void decrement_SR_SELID_11(rh850g4::SR_SELID_11_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_12 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_12 group register
     */
    virtual void decrement_SR_SELID_12(rh850g4::SR_SELID_12_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_13 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_13 group register
     */
    virtual void decrement_SR_SELID_13(rh850g4::SR_SELID_13_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_14 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_14 group register
     */
    virtual void decrement_SR_SELID_14(rh850g4::SR_SELID_14_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_15 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_15 group register
     */
    virtual void decrement_SR_SELID_15(rh850g4::SR_SELID_15_index_t index, unsigned int value);

    // generative instruction count operations
    /**
     * \brief Generates code to increment the number of instructions executed in the current quanta by one
     */
    void gen_increment_instruction_count(void);

    // generative time operations
    /**
     * \brief Generates code to query the time executed in the current quanta
     * \return Object representing the time executed in the current quanta
     */
    fastiss::i64_t* gen_read_time(void);

    /**
     * \brief Generates code to set the time executed in the current quanta
     * \param value Object representing the new time executed in the current quanta
     */
    void gen_write_time(fastiss::i64_t* value);

    /**
     * \brief Generates code to increment the time executed in the current quanta by the specified amount
     * \param increment Object representing the amount by which to increment the time executed in the current quanta
     */
    void gen_increment_time(fastiss::i64_t *increment);

    /**
     * \brief Generates code to decrement the time executed in the current quanta by the specified amount
     * \param decrement Object representing the amount by which to decrement the time executed in the current quanta
     */
    void gen_decrement_time(fastiss::i64_t *decrement);

    // generative cycle period operations
    /**
     * \brief Generates code to query the period of time taken to execute a cycle
     * \return Object representing the period of time taken to execute a cycle
     */
    fastiss::i64_t* gen_read_cycle_period(void);

    /**
     * \brief Generates code to set the period of time taken to execute a cycle
     * \param value Object representing the new period of time taken to execute a cycle
     */
    void gen_write_cycle_period(fastiss::i64_t* value);

    /**
     * \brief Generates code to increment the period of time taken to execute a cycle
     * \param increment Object representing the amount by which to increment the period of time taken to execute a cycle
     */
    void gen_increment_cycle_period(fastiss::i64_t *increment);

    /**
     * \brief Generates code to decrement the period of time taken to execute a cycle
     * \param decrement Object representing the amount by which to decrement the period of time taken to execute a cycle
     */
    void gen_decrement_cycle_period(fastiss::i64_t *decrement);

    // generative core pointer operations
    /**
     * \brief Generates code to query the value of the pointer to the core instance
     * \return Object representing the value of the pointer to the core instance
     */
    fastiss::i8_ptr_t* gen_read_core_ptr(void);

    // generative watchpoint operations
    /**
     * \brief Generates code to query the value of the watchpoint hit flag
     * \return Object representing the value of the watchpoint hit flag
     */
    fastiss::i8_t* gen_read_watchpoint_hit(void);
    /**
     * \brief Generates code to set the value of the watchpoint hit flag
     * \param value Object representing the value of the watchpoint hit flag
     */
    void gen_write_watchpoint_hit(fastiss::i8_t* value);

    // immediate memory instrumentation query operations
    /**
     * \brief Query the last transaction address.
     * Note that this only valid within an analysis callback associated with
     * a memory transaction.
     * \return Last transaction address.
     */
    unsigned int read_last_access_addr();
    
    /**
     * \brief Query the last transaction size.
     * Note that this only valid within an analysis callback associated with
     * a memory transaction.
     * \return Last transaction size.
     */
    unsigned char read_last_access_size();
    
    /**
     * \brief Query the last transaction data.
     * Note that this only valid within an analysis callback associated with
     * a memory transaction.
     * \return Last transaction data.
     */
    unsigned long long read_last_access_data();
    
    // internal memory instrumentation operations
    /**
     * \brief Set the last transaction address. This operation is intended for
     * internal use only.
     * \param addr Transaction address.
     */
    void write_last_access_addr(unsigned int addr);
    
    /**
     * \brief Set the last transaction size. This operation is intended for
     * internal use only.
     * \param size Transaction size.
     */
    void write_last_access_size(unsigned char size);
    
    /**
     * \brief Set the last transaction data. This operation is intended for
     * internal use only.
     * \param data Transaction data.
     */
    void write_last_access_data(unsigned long long data);
    
    /**
     * \brief Generate the code to set the last transaction address. This
     * operation is intended for internal use only.
     * \param value Transaction address value.
     */
    void gen_write_last_access_addr(fastiss::i32_t* value);

    /**
     * \brief Generate the code to set the last transaction size. This
     * operation is intended for internal use only.
     * \param value Transaction size value.
     */
    void gen_write_last_access_size(fastiss::i8_t* value);

    /**
     * \brief Generate the code to set the last transaction data. This
     * operation is intended for internal use only.
     * \param value Transaction data value.
     */
    void gen_write_last_access_data(fastiss::i64_t* value);
    
    // generative register and register field read operations
    /**
     * \brief Generates code to query the value of the rh850g4 r0 register
     * \return Object representing the value of the rh850g4 r0 register
     */
    virtual fastiss::i32_t *gen_read_r0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r1 register
     * \return Object representing the value of the rh850g4 r1 register
     */
    virtual fastiss::i32_t *gen_read_r1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r2 register
     * \return Object representing the value of the rh850g4 r2 register
     */
    virtual fastiss::i32_t *gen_read_r2(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r3 register
     * \return Object representing the value of the rh850g4 r3 register
     */
    virtual fastiss::i32_t *gen_read_r3(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r4 register
     * \return Object representing the value of the rh850g4 r4 register
     */
    virtual fastiss::i32_t *gen_read_r4(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r5 register
     * \return Object representing the value of the rh850g4 r5 register
     */
    virtual fastiss::i32_t *gen_read_r5(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r6 register
     * \return Object representing the value of the rh850g4 r6 register
     */
    virtual fastiss::i32_t *gen_read_r6(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r7 register
     * \return Object representing the value of the rh850g4 r7 register
     */
    virtual fastiss::i32_t *gen_read_r7(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r8 register
     * \return Object representing the value of the rh850g4 r8 register
     */
    virtual fastiss::i32_t *gen_read_r8(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r9 register
     * \return Object representing the value of the rh850g4 r9 register
     */
    virtual fastiss::i32_t *gen_read_r9(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r10 register
     * \return Object representing the value of the rh850g4 r10 register
     */
    virtual fastiss::i32_t *gen_read_r10(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r11 register
     * \return Object representing the value of the rh850g4 r11 register
     */
    virtual fastiss::i32_t *gen_read_r11(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r12 register
     * \return Object representing the value of the rh850g4 r12 register
     */
    virtual fastiss::i32_t *gen_read_r12(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r13 register
     * \return Object representing the value of the rh850g4 r13 register
     */
    virtual fastiss::i32_t *gen_read_r13(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r14 register
     * \return Object representing the value of the rh850g4 r14 register
     */
    virtual fastiss::i32_t *gen_read_r14(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r15 register
     * \return Object representing the value of the rh850g4 r15 register
     */
    virtual fastiss::i32_t *gen_read_r15(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r16 register
     * \return Object representing the value of the rh850g4 r16 register
     */
    virtual fastiss::i32_t *gen_read_r16(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r17 register
     * \return Object representing the value of the rh850g4 r17 register
     */
    virtual fastiss::i32_t *gen_read_r17(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r18 register
     * \return Object representing the value of the rh850g4 r18 register
     */
    virtual fastiss::i32_t *gen_read_r18(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r19 register
     * \return Object representing the value of the rh850g4 r19 register
     */
    virtual fastiss::i32_t *gen_read_r19(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r20 register
     * \return Object representing the value of the rh850g4 r20 register
     */
    virtual fastiss::i32_t *gen_read_r20(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r21 register
     * \return Object representing the value of the rh850g4 r21 register
     */
    virtual fastiss::i32_t *gen_read_r21(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r22 register
     * \return Object representing the value of the rh850g4 r22 register
     */
    virtual fastiss::i32_t *gen_read_r22(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r23 register
     * \return Object representing the value of the rh850g4 r23 register
     */
    virtual fastiss::i32_t *gen_read_r23(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r24 register
     * \return Object representing the value of the rh850g4 r24 register
     */
    virtual fastiss::i32_t *gen_read_r24(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r25 register
     * \return Object representing the value of the rh850g4 r25 register
     */
    virtual fastiss::i32_t *gen_read_r25(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r26 register
     * \return Object representing the value of the rh850g4 r26 register
     */
    virtual fastiss::i32_t *gen_read_r26(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r27 register
     * \return Object representing the value of the rh850g4 r27 register
     */
    virtual fastiss::i32_t *gen_read_r27(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r28 register
     * \return Object representing the value of the rh850g4 r28 register
     */
    virtual fastiss::i32_t *gen_read_r28(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r29 register
     * \return Object representing the value of the rh850g4 r29 register
     */
    virtual fastiss::i32_t *gen_read_r29(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r30 register
     * \return Object representing the value of the rh850g4 r30 register
     */
    virtual fastiss::i32_t *gen_read_r30(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r31 register
     * \return Object representing the value of the rh850g4 r31 register
     */
    virtual fastiss::i32_t *gen_read_r31(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PC register
     * \return Object representing the value of the rh850g4 PC register
     */
    virtual fastiss::i32_t *gen_read_PC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr0 register
     * \return Object representing the value of the rh850g4 wr0 register
     */
    virtual fastiss::i64_t *gen_read_wr0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr0_u register
     * \return Object representing the value of the rh850g4 wr0_u register
     */
    virtual fastiss::i64_t *gen_read_wr0_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr1 register
     * \return Object representing the value of the rh850g4 wr1 register
     */
    virtual fastiss::i64_t *gen_read_wr1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr1_u register
     * \return Object representing the value of the rh850g4 wr1_u register
     */
    virtual fastiss::i64_t *gen_read_wr1_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr2 register
     * \return Object representing the value of the rh850g4 wr2 register
     */
    virtual fastiss::i64_t *gen_read_wr2(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr2_u register
     * \return Object representing the value of the rh850g4 wr2_u register
     */
    virtual fastiss::i64_t *gen_read_wr2_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr3 register
     * \return Object representing the value of the rh850g4 wr3 register
     */
    virtual fastiss::i64_t *gen_read_wr3(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr3_u register
     * \return Object representing the value of the rh850g4 wr3_u register
     */
    virtual fastiss::i64_t *gen_read_wr3_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr4 register
     * \return Object representing the value of the rh850g4 wr4 register
     */
    virtual fastiss::i64_t *gen_read_wr4(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr4_u register
     * \return Object representing the value of the rh850g4 wr4_u register
     */
    virtual fastiss::i64_t *gen_read_wr4_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr5 register
     * \return Object representing the value of the rh850g4 wr5 register
     */
    virtual fastiss::i64_t *gen_read_wr5(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr5_u register
     * \return Object representing the value of the rh850g4 wr5_u register
     */
    virtual fastiss::i64_t *gen_read_wr5_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr6 register
     * \return Object representing the value of the rh850g4 wr6 register
     */
    virtual fastiss::i64_t *gen_read_wr6(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr6_u register
     * \return Object representing the value of the rh850g4 wr6_u register
     */
    virtual fastiss::i64_t *gen_read_wr6_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr7 register
     * \return Object representing the value of the rh850g4 wr7 register
     */
    virtual fastiss::i64_t *gen_read_wr7(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr7_u register
     * \return Object representing the value of the rh850g4 wr7_u register
     */
    virtual fastiss::i64_t *gen_read_wr7_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr8 register
     * \return Object representing the value of the rh850g4 wr8 register
     */
    virtual fastiss::i64_t *gen_read_wr8(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr8_u register
     * \return Object representing the value of the rh850g4 wr8_u register
     */
    virtual fastiss::i64_t *gen_read_wr8_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr9 register
     * \return Object representing the value of the rh850g4 wr9 register
     */
    virtual fastiss::i64_t *gen_read_wr9(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr9_u register
     * \return Object representing the value of the rh850g4 wr9_u register
     */
    virtual fastiss::i64_t *gen_read_wr9_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr10 register
     * \return Object representing the value of the rh850g4 wr10 register
     */
    virtual fastiss::i64_t *gen_read_wr10(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr10_u register
     * \return Object representing the value of the rh850g4 wr10_u register
     */
    virtual fastiss::i64_t *gen_read_wr10_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr11 register
     * \return Object representing the value of the rh850g4 wr11 register
     */
    virtual fastiss::i64_t *gen_read_wr11(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr11_u register
     * \return Object representing the value of the rh850g4 wr11_u register
     */
    virtual fastiss::i64_t *gen_read_wr11_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr12 register
     * \return Object representing the value of the rh850g4 wr12 register
     */
    virtual fastiss::i64_t *gen_read_wr12(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr12_u register
     * \return Object representing the value of the rh850g4 wr12_u register
     */
    virtual fastiss::i64_t *gen_read_wr12_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr13 register
     * \return Object representing the value of the rh850g4 wr13 register
     */
    virtual fastiss::i64_t *gen_read_wr13(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr13_u register
     * \return Object representing the value of the rh850g4 wr13_u register
     */
    virtual fastiss::i64_t *gen_read_wr13_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr14 register
     * \return Object representing the value of the rh850g4 wr14 register
     */
    virtual fastiss::i64_t *gen_read_wr14(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr14_u register
     * \return Object representing the value of the rh850g4 wr14_u register
     */
    virtual fastiss::i64_t *gen_read_wr14_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr15 register
     * \return Object representing the value of the rh850g4 wr15 register
     */
    virtual fastiss::i64_t *gen_read_wr15(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr15_u register
     * \return Object representing the value of the rh850g4 wr15_u register
     */
    virtual fastiss::i64_t *gen_read_wr15_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr16 register
     * \return Object representing the value of the rh850g4 wr16 register
     */
    virtual fastiss::i64_t *gen_read_wr16(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr16_u register
     * \return Object representing the value of the rh850g4 wr16_u register
     */
    virtual fastiss::i64_t *gen_read_wr16_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr17 register
     * \return Object representing the value of the rh850g4 wr17 register
     */
    virtual fastiss::i64_t *gen_read_wr17(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr17_u register
     * \return Object representing the value of the rh850g4 wr17_u register
     */
    virtual fastiss::i64_t *gen_read_wr17_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr18 register
     * \return Object representing the value of the rh850g4 wr18 register
     */
    virtual fastiss::i64_t *gen_read_wr18(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr18_u register
     * \return Object representing the value of the rh850g4 wr18_u register
     */
    virtual fastiss::i64_t *gen_read_wr18_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr19 register
     * \return Object representing the value of the rh850g4 wr19 register
     */
    virtual fastiss::i64_t *gen_read_wr19(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr19_u register
     * \return Object representing the value of the rh850g4 wr19_u register
     */
    virtual fastiss::i64_t *gen_read_wr19_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr20 register
     * \return Object representing the value of the rh850g4 wr20 register
     */
    virtual fastiss::i64_t *gen_read_wr20(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr20_u register
     * \return Object representing the value of the rh850g4 wr20_u register
     */
    virtual fastiss::i64_t *gen_read_wr20_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr21 register
     * \return Object representing the value of the rh850g4 wr21 register
     */
    virtual fastiss::i64_t *gen_read_wr21(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr21_u register
     * \return Object representing the value of the rh850g4 wr21_u register
     */
    virtual fastiss::i64_t *gen_read_wr21_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr22 register
     * \return Object representing the value of the rh850g4 wr22 register
     */
    virtual fastiss::i64_t *gen_read_wr22(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr22_u register
     * \return Object representing the value of the rh850g4 wr22_u register
     */
    virtual fastiss::i64_t *gen_read_wr22_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr23 register
     * \return Object representing the value of the rh850g4 wr23 register
     */
    virtual fastiss::i64_t *gen_read_wr23(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr23_u register
     * \return Object representing the value of the rh850g4 wr23_u register
     */
    virtual fastiss::i64_t *gen_read_wr23_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr24 register
     * \return Object representing the value of the rh850g4 wr24 register
     */
    virtual fastiss::i64_t *gen_read_wr24(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr24_u register
     * \return Object representing the value of the rh850g4 wr24_u register
     */
    virtual fastiss::i64_t *gen_read_wr24_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr25 register
     * \return Object representing the value of the rh850g4 wr25 register
     */
    virtual fastiss::i64_t *gen_read_wr25(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr25_u register
     * \return Object representing the value of the rh850g4 wr25_u register
     */
    virtual fastiss::i64_t *gen_read_wr25_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr26 register
     * \return Object representing the value of the rh850g4 wr26 register
     */
    virtual fastiss::i64_t *gen_read_wr26(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr26_u register
     * \return Object representing the value of the rh850g4 wr26_u register
     */
    virtual fastiss::i64_t *gen_read_wr26_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr27 register
     * \return Object representing the value of the rh850g4 wr27 register
     */
    virtual fastiss::i64_t *gen_read_wr27(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr27_u register
     * \return Object representing the value of the rh850g4 wr27_u register
     */
    virtual fastiss::i64_t *gen_read_wr27_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr28 register
     * \return Object representing the value of the rh850g4 wr28 register
     */
    virtual fastiss::i64_t *gen_read_wr28(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr28_u register
     * \return Object representing the value of the rh850g4 wr28_u register
     */
    virtual fastiss::i64_t *gen_read_wr28_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr29 register
     * \return Object representing the value of the rh850g4 wr29 register
     */
    virtual fastiss::i64_t *gen_read_wr29(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr29_u register
     * \return Object representing the value of the rh850g4 wr29_u register
     */
    virtual fastiss::i64_t *gen_read_wr29_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr30 register
     * \return Object representing the value of the rh850g4 wr30 register
     */
    virtual fastiss::i64_t *gen_read_wr30(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr30_u register
     * \return Object representing the value of the rh850g4 wr30_u register
     */
    virtual fastiss::i64_t *gen_read_wr30_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr31 register
     * \return Object representing the value of the rh850g4 wr31 register
     */
    virtual fastiss::i64_t *gen_read_wr31(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr31_u register
     * \return Object representing the value of the rh850g4 wr31_u register
     */
    virtual fastiss::i64_t *gen_read_wr31_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 EIPC register
     * \return Object representing the value of the rh850g4 EIPC register
     */
    virtual fastiss::i32_t *gen_read_EIPC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 EIPSW register
     * \return Object representing the value of the rh850g4 EIPSW register
     */
    virtual fastiss::i32_t *gen_read_EIPSW(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FEPC register
     * \return Object representing the value of the rh850g4 FEPC register
     */
    virtual fastiss::i32_t *gen_read_FEPC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FEPSW register
     * \return Object representing the value of the rh850g4 FEPSW register
     */
    virtual fastiss::i32_t *gen_read_FEPSW(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PSW register
     * \return Object representing the value of the rh850g4 PSW register
     */
    virtual fastiss::i32_t *gen_read_PSW(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_Z register field
    * \return Object representing the value of the rh850g4 PSW_Z register field
    */
    virtual fastiss::i1_t *gen_read_PSW_Z(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_S register field
    * \return Object representing the value of the rh850g4 PSW_S register field
    */
    virtual fastiss::i1_t *gen_read_PSW_S(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_OV register field
    * \return Object representing the value of the rh850g4 PSW_OV register field
    */
    virtual fastiss::i1_t *gen_read_PSW_OV(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_CY register field
    * \return Object representing the value of the rh850g4 PSW_CY register field
    */
    virtual fastiss::i1_t *gen_read_PSW_CY(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_SAT register field
    * \return Object representing the value of the rh850g4 PSW_SAT register field
    */
    virtual fastiss::i1_t *gen_read_PSW_SAT(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_ID register field
    * \return Object representing the value of the rh850g4 PSW_ID register field
    */
    virtual fastiss::i1_t *gen_read_PSW_ID(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_EP register field
    * \return Object representing the value of the rh850g4 PSW_EP register field
    */
    virtual fastiss::i1_t *gen_read_PSW_EP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_NP register field
    * \return Object representing the value of the rh850g4 PSW_NP register field
    */
    virtual fastiss::i1_t *gen_read_PSW_NP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_EBV register field
    * \return Object representing the value of the rh850g4 PSW_EBV register field
    */
    virtual fastiss::i1_t *gen_read_PSW_EBV(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_CU0 register field
    * \return Object representing the value of the rh850g4 PSW_CU0 register field
    */
    virtual fastiss::i1_t *gen_read_PSW_CU0(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_CU1 register field
    * \return Object representing the value of the rh850g4 PSW_CU1 register field
    */
    virtual fastiss::i1_t *gen_read_PSW_CU1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_CU2 register field
    * \return Object representing the value of the rh850g4 PSW_CU2 register field
    */
    virtual fastiss::i1_t *gen_read_PSW_CU2(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_EIMASK register field
    * \return Object representing the value of the rh850g4 PSW_EIMASK register field
    */
    virtual fastiss::i8_t *gen_read_PSW_EIMASK(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_UM register field
    * \return Object representing the value of the rh850g4 PSW_UM register field
    */
    virtual fastiss::i1_t *gen_read_PSW_UM(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FPSR register
     * \return Object representing the value of the rh850g4 FPSR register
     */
    virtual fastiss::i32_t *gen_read_FPSR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_CC register field
    * \return Object representing the value of the rh850g4 FPSR_CC register field
    */
    virtual fastiss::i8_t *gen_read_FPSR_CC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_FN register field
    * \return Object representing the value of the rh850g4 FPSR_FN register field
    */
    virtual fastiss::i1_t *gen_read_FPSR_FN(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_IF register field
    * \return Object representing the value of the rh850g4 FPSR_IF register field
    */
    virtual fastiss::i1_t *gen_read_FPSR_IF(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_PEM register field
    * \return Object representing the value of the rh850g4 FPSR_PEM register field
    */
    virtual fastiss::i1_t *gen_read_FPSR_PEM(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_RM register field
    * \return Object representing the value of the rh850g4 FPSR_RM register field
    */
    virtual fastiss::i8_t *gen_read_FPSR_RM(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_FS register field
    * \return Object representing the value of the rh850g4 FPSR_FS register field
    */
    virtual fastiss::i1_t *gen_read_FPSR_FS(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_XC register field
    * \return Object representing the value of the rh850g4 FPSR_XC register field
    */
    virtual fastiss::i8_t *gen_read_FPSR_XC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_XE register field
    * \return Object representing the value of the rh850g4 FPSR_XE register field
    */
    virtual fastiss::i8_t *gen_read_FPSR_XE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_XP register field
    * \return Object representing the value of the rh850g4 FPSR_XP register field
    */
    virtual fastiss::i8_t *gen_read_FPSR_XP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FPEPC register
     * \return Object representing the value of the rh850g4 FPEPC register
     */
    virtual fastiss::i32_t *gen_read_FPEPC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FPST register
     * \return Object representing the value of the rh850g4 FPST register
     */
    virtual fastiss::i32_t *gen_read_FPST(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPST_XC register field
    * \return Object representing the value of the rh850g4 FPST_XC register field
    */
    virtual fastiss::i8_t *gen_read_FPST_XC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPST_IF register field
    * \return Object representing the value of the rh850g4 FPST_IF register field
    */
    virtual fastiss::i1_t *gen_read_FPST_IF(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPST_XP register field
    * \return Object representing the value of the rh850g4 FPST_XP register field
    */
    virtual fastiss::i8_t *gen_read_FPST_XP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FPCC register
     * \return Object representing the value of the rh850g4 FPCC register
     */
    virtual fastiss::i32_t *gen_read_FPCC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPCC_CC register field
    * \return Object representing the value of the rh850g4 FPCC_CC register field
    */
    virtual fastiss::i8_t *gen_read_FPCC_CC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FPCFG register
     * \return Object representing the value of the rh850g4 FPCFG register
     */
    virtual fastiss::i32_t *gen_read_FPCFG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPCFG_RM register field
    * \return Object representing the value of the rh850g4 FPCFG_RM register field
    */
    virtual fastiss::i8_t *gen_read_FPCFG_RM(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPCFG_XE register field
    * \return Object representing the value of the rh850g4 FPCFG_XE register field
    */
    virtual fastiss::i8_t *gen_read_FPCFG_XE(void);
    /**
     * \brief Generates code to query the value of the rh850g4 EIIC register
     * \return Object representing the value of the rh850g4 EIIC register
     */
    virtual fastiss::i32_t *gen_read_EIIC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FEIC register
     * \return Object representing the value of the rh850g4 FEIC register
     */
    virtual fastiss::i32_t *gen_read_FEIC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PSWH register
     * \return Object representing the value of the rh850g4 PSWH register
     */
    virtual fastiss::i32_t *gen_read_PSWH(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSWH_GPID register field
    * \return Object representing the value of the rh850g4 PSWH_GPID register field
    */
    virtual fastiss::i8_t *gen_read_PSWH_GPID(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSWH_GM register field
    * \return Object representing the value of the rh850g4 PSWH_GM register field
    */
    virtual fastiss::i1_t *gen_read_PSWH_GM(void);
    /**
     * \brief Generates code to query the value of the rh850g4 CTPC register
     * \return Object representing the value of the rh850g4 CTPC register
     */
    virtual fastiss::i32_t *gen_read_CTPC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 CTPSW register
     * \return Object representing the value of the rh850g4 CTPSW register
     */
    virtual fastiss::i32_t *gen_read_CTPSW(void);
    /**
     * \brief Generates code to query the value of the rh850g4 EIPSWH register
     * \return Object representing the value of the rh850g4 EIPSWH register
     */
    virtual fastiss::i32_t *gen_read_EIPSWH(void);
    /**
    * \brief Generates code to query the value of the rh850g4 EIPSWH_GPID register field
    * \return Object representing the value of the rh850g4 EIPSWH_GPID register field
    */
    virtual fastiss::i8_t *gen_read_EIPSWH_GPID(void);
    /**
    * \brief Generates code to query the value of the rh850g4 EIPSWH_GM register field
    * \return Object representing the value of the rh850g4 EIPSWH_GM register field
    */
    virtual fastiss::i1_t *gen_read_EIPSWH_GM(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FEPSWH register
     * \return Object representing the value of the rh850g4 FEPSWH register
     */
    virtual fastiss::i32_t *gen_read_FEPSWH(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FEPSWH_GPID register field
    * \return Object representing the value of the rh850g4 FEPSWH_GPID register field
    */
    virtual fastiss::i8_t *gen_read_FEPSWH_GPID(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FEPSWH_GM register field
    * \return Object representing the value of the rh850g4 FEPSWH_GM register field
    */
    virtual fastiss::i1_t *gen_read_FEPSWH_GM(void);
    /**
     * \brief Generates code to query the value of the rh850g4 CTBP register
     * \return Object representing the value of the rh850g4 CTBP register
     */
    virtual fastiss::i32_t *gen_read_CTBP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 SNZCFG register
     * \return Object representing the value of the rh850g4 SNZCFG register
     */
    virtual fastiss::i32_t *gen_read_SNZCFG(void);
    /**
     * \brief Generates code to query the value of the rh850g4 EIWR register
     * \return Object representing the value of the rh850g4 EIWR register
     */
    virtual fastiss::i32_t *gen_read_EIWR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FEWR register
     * \return Object representing the value of the rh850g4 FEWR register
     */
    virtual fastiss::i32_t *gen_read_FEWR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 SPID register
     * \return Object representing the value of the rh850g4 SPID register
     */
    virtual fastiss::i32_t *gen_read_SPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 SPIDLIST register
     * \return Object representing the value of the rh850g4 SPIDLIST register
     */
    virtual fastiss::i32_t *gen_read_SPIDLIST(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RBASE register
     * \return Object representing the value of the rh850g4 RBASE register
     */
    virtual fastiss::i32_t *gen_read_RBASE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 RBASE_RBASE register field
    * \return Object representing the value of the rh850g4 RBASE_RBASE register field
    */
    virtual fastiss::i32_t *gen_read_RBASE_RBASE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 RBASE_DV register field
    * \return Object representing the value of the rh850g4 RBASE_DV register field
    */
    virtual fastiss::i1_t *gen_read_RBASE_DV(void);
    /**
    * \brief Generates code to query the value of the rh850g4 RBASE_RINT register field
    * \return Object representing the value of the rh850g4 RBASE_RINT register field
    */
    virtual fastiss::i1_t *gen_read_RBASE_RINT(void);
    /**
     * \brief Generates code to query the value of the rh850g4 EBASE register
     * \return Object representing the value of the rh850g4 EBASE register
     */
    virtual fastiss::i32_t *gen_read_EBASE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 EBASE_EBASE register field
    * \return Object representing the value of the rh850g4 EBASE_EBASE register field
    */
    virtual fastiss::i32_t *gen_read_EBASE_EBASE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 EBASE_DV register field
    * \return Object representing the value of the rh850g4 EBASE_DV register field
    */
    virtual fastiss::i1_t *gen_read_EBASE_DV(void);
    /**
    * \brief Generates code to query the value of the rh850g4 EBASE_RINT register field
    * \return Object representing the value of the rh850g4 EBASE_RINT register field
    */
    virtual fastiss::i1_t *gen_read_EBASE_RINT(void);
    /**
     * \brief Generates code to query the value of the rh850g4 INTBP register
     * \return Object representing the value of the rh850g4 INTBP register
     */
    virtual fastiss::i32_t *gen_read_INTBP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MCTL register
     * \return Object representing the value of the rh850g4 MCTL register
     */
    virtual fastiss::i32_t *gen_read_MCTL(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCTL_UIC register field
    * \return Object representing the value of the rh850g4 MCTL_UIC register field
    */
    virtual fastiss::i1_t *gen_read_MCTL_UIC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PID register
     * \return Object representing the value of the rh850g4 PID register
     */
    virtual fastiss::i32_t *gen_read_PID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 SVLOCK register
     * \return Object representing the value of the rh850g4 SVLOCK register
     */
    virtual fastiss::i32_t *gen_read_SVLOCK(void);
    /**
     * \brief Generates code to query the value of the rh850g4 SCCFG register
     * \return Object representing the value of the rh850g4 SCCFG register
     */
    virtual fastiss::i32_t *gen_read_SCCFG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 SCCFG_SIZE register field
    * \return Object representing the value of the rh850g4 SCCFG_SIZE register field
    */
    virtual fastiss::i8_t *gen_read_SCCFG_SIZE(void);
    /**
     * \brief Generates code to query the value of the rh850g4 SCBP register
     * \return Object representing the value of the rh850g4 SCBP register
     */
    virtual fastiss::i32_t *gen_read_SCBP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 HVCFG register
     * \return Object representing the value of the rh850g4 HVCFG register
     */
    virtual fastiss::i32_t *gen_read_HVCFG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 HVCFG_HVE register field
    * \return Object representing the value of the rh850g4 HVCFG_HVE register field
    */
    virtual fastiss::i1_t *gen_read_HVCFG_HVE(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMCFG register
     * \return Object representing the value of the rh850g4 GMCFG register
     */
    virtual fastiss::i32_t *gen_read_GMCFG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMCFG_GMP register field
    * \return Object representing the value of the rh850g4 GMCFG_GMP register field
    */
    virtual fastiss::i1_t *gen_read_GMCFG_GMP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMCFG_HMP register field
    * \return Object representing the value of the rh850g4 GMCFG_HMP register field
    */
    virtual fastiss::i1_t *gen_read_GMCFG_HMP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMCFG_GSYSE register field
    * \return Object representing the value of the rh850g4 GMCFG_GSYSE register field
    */
    virtual fastiss::i1_t *gen_read_GMCFG_GSYSE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMCFG_GCU0 register field
    * \return Object representing the value of the rh850g4 GMCFG_GCU0 register field
    */
    virtual fastiss::i1_t *gen_read_GMCFG_GCU0(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMCFG_GCU1 register field
    * \return Object representing the value of the rh850g4 GMCFG_GCU1 register field
    */
    virtual fastiss::i1_t *gen_read_GMCFG_GCU1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMCFG_GCU2 register field
    * \return Object representing the value of the rh850g4 GMCFG_GCU2 register field
    */
    virtual fastiss::i1_t *gen_read_GMCFG_GCU2(void);
    /**
     * \brief Generates code to query the value of the rh850g4 HVSB register
     * \return Object representing the value of the rh850g4 HVSB register
     */
    virtual fastiss::i32_t *gen_read_HVSB(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PEID register
     * \return Object representing the value of the rh850g4 PEID register
     */
    virtual fastiss::i32_t *gen_read_PEID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 BMID register
     * \return Object representing the value of the rh850g4 BMID register
     */
    virtual fastiss::i32_t *gen_read_BMID(void);
    /**
    * \brief Generates code to query the value of the rh850g4 BMID_BMID register field
    * \return Object representing the value of the rh850g4 BMID_BMID register field
    */
    virtual fastiss::i8_t *gen_read_BMID_BMID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MEA register
     * \return Object representing the value of the rh850g4 MEA register
     */
    virtual fastiss::i32_t *gen_read_MEA(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MEI register
     * \return Object representing the value of the rh850g4 MEI register
     */
    virtual fastiss::i32_t *gen_read_MEI(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RBCR0 register
     * \return Object representing the value of the rh850g4 RBCR0 register
     */
    virtual fastiss::i32_t *gen_read_RBCR0(void);
    /**
    * \brief Generates code to query the value of the rh850g4 RBCR0_MD register field
    * \return Object representing the value of the rh850g4 RBCR0_MD register field
    */
    virtual fastiss::i1_t *gen_read_RBCR0_MD(void);
    /**
    * \brief Generates code to query the value of the rh850g4 RBCR0_BE register field
    * \return Object representing the value of the rh850g4 RBCR0_BE register field
    */
    virtual fastiss::i16_t *gen_read_RBCR0_BE(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RBCR1 register
     * \return Object representing the value of the rh850g4 RBCR1 register
     */
    virtual fastiss::i32_t *gen_read_RBCR1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 RBCR1_NC register field
    * \return Object representing the value of the rh850g4 RBCR1_NC register field
    */
    virtual fastiss::i16_t *gen_read_RBCR1_NC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RBNR register
     * \return Object representing the value of the rh850g4 RBNR register
     */
    virtual fastiss::i32_t *gen_read_RBNR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 RBNR_BN register field
    * \return Object representing the value of the rh850g4 RBNR_BN register field
    */
    virtual fastiss::i8_t *gen_read_RBNR_BN(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RBIP register
     * \return Object representing the value of the rh850g4 RBIP register
     */
    virtual fastiss::i32_t *gen_read_RBIP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 RBIP_RBIP register field
    * \return Object representing the value of the rh850g4 RBIP_RBIP register field
    */
    virtual fastiss::i32_t *gen_read_RBIP_RBIP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 ISPR register
     * \return Object representing the value of the rh850g4 ISPR register
     */
    virtual fastiss::i32_t *gen_read_ISPR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ISPR_ISP register field
    * \return Object representing the value of the rh850g4 ISPR_ISP register field
    */
    virtual fastiss::i16_t *gen_read_ISPR_ISP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 IMSR register
     * \return Object representing the value of the rh850g4 IMSR register
     */
    virtual fastiss::i32_t *gen_read_IMSR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_EEIM register field
    * \return Object representing the value of the rh850g4 IMSR_EEIM register field
    */
    virtual fastiss::i1_t *gen_read_IMSR_EEIM(void);
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_EPLM register field
    * \return Object representing the value of the rh850g4 IMSR_EPLM register field
    */
    virtual fastiss::i1_t *gen_read_IMSR_EPLM(void);
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_EID register field
    * \return Object representing the value of the rh850g4 IMSR_EID register field
    */
    virtual fastiss::i1_t *gen_read_IMSR_EID(void);
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_ENP register field
    * \return Object representing the value of the rh850g4 IMSR_ENP register field
    */
    virtual fastiss::i1_t *gen_read_IMSR_ENP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_FNP register field
    * \return Object representing the value of the rh850g4 IMSR_FNP register field
    */
    virtual fastiss::i1_t *gen_read_IMSR_FNP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_HEEIM register field
    * \return Object representing the value of the rh850g4 IMSR_HEEIM register field
    */
    virtual fastiss::i1_t *gen_read_IMSR_HEEIM(void);
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_HEPLM register field
    * \return Object representing the value of the rh850g4 IMSR_HEPLM register field
    */
    virtual fastiss::i1_t *gen_read_IMSR_HEPLM(void);
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_HEID register field
    * \return Object representing the value of the rh850g4 IMSR_HEID register field
    */
    virtual fastiss::i1_t *gen_read_IMSR_HEID(void);
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_HENP register field
    * \return Object representing the value of the rh850g4 IMSR_HENP register field
    */
    virtual fastiss::i1_t *gen_read_IMSR_HENP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_HFNP register field
    * \return Object representing the value of the rh850g4 IMSR_HFNP register field
    */
    virtual fastiss::i1_t *gen_read_IMSR_HFNP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 ICSR register
     * \return Object representing the value of the rh850g4 ICSR register
     */
    virtual fastiss::i32_t *gen_read_ICSR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICSR_PMEI register field
    * \return Object representing the value of the rh850g4 ICSR_PMEI register field
    */
    virtual fastiss::i1_t *gen_read_ICSR_PMEI(void);
    /**
     * \brief Generates code to query the value of the rh850g4 INTCFG register
     * \return Object representing the value of the rh850g4 INTCFG register
     */
    virtual fastiss::i32_t *gen_read_INTCFG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 INTCFG_ISPC register field
    * \return Object representing the value of the rh850g4 INTCFG_ISPC register field
    */
    virtual fastiss::i1_t *gen_read_INTCFG_ISPC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 INTCFG_EPL register field
    * \return Object representing the value of the rh850g4 INTCFG_EPL register field
    */
    virtual fastiss::i1_t *gen_read_INTCFG_EPL(void);
    /**
    * \brief Generates code to query the value of the rh850g4 INTCFG_ULNR register field
    * \return Object representing the value of the rh850g4 INTCFG_ULNR register field
    */
    virtual fastiss::i8_t *gen_read_INTCFG_ULNR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PLMR register
     * \return Object representing the value of the rh850g4 PLMR register
     */
    virtual fastiss::i32_t *gen_read_PLMR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PLMR_PLM register field
    * \return Object representing the value of the rh850g4 PLMR_PLM register field
    */
    virtual fastiss::i8_t *gen_read_PLMR_PLM(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FXSR register
     * \return Object representing the value of the rh850g4 FXSR register
     */
    virtual fastiss::i32_t *gen_read_FXSR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_FN register field
    * \return Object representing the value of the rh850g4 FXSR_FN register field
    */
    virtual fastiss::i1_t *gen_read_FXSR_FN(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_IF register field
    * \return Object representing the value of the rh850g4 FXSR_IF register field
    */
    virtual fastiss::i1_t *gen_read_FXSR_IF(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_PEM register field
    * \return Object representing the value of the rh850g4 FXSR_PEM register field
    */
    virtual fastiss::i1_t *gen_read_FXSR_PEM(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_RM register field
    * \return Object representing the value of the rh850g4 FXSR_RM register field
    */
    virtual fastiss::i8_t *gen_read_FXSR_RM(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_FS register field
    * \return Object representing the value of the rh850g4 FXSR_FS register field
    */
    virtual fastiss::i1_t *gen_read_FXSR_FS(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_XC register field
    * \return Object representing the value of the rh850g4 FXSR_XC register field
    */
    virtual fastiss::i8_t *gen_read_FXSR_XC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_XE register field
    * \return Object representing the value of the rh850g4 FXSR_XE register field
    */
    virtual fastiss::i8_t *gen_read_FXSR_XE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_XP register field
    * \return Object representing the value of the rh850g4 FXSR_XP register field
    */
    virtual fastiss::i8_t *gen_read_FXSR_XP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FXST register
     * \return Object representing the value of the rh850g4 FXST register
     */
    virtual fastiss::i32_t *gen_read_FXST(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXST_XC register field
    * \return Object representing the value of the rh850g4 FXST_XC register field
    */
    virtual fastiss::i8_t *gen_read_FXST_XC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXST_IF register field
    * \return Object representing the value of the rh850g4 FXST_IF register field
    */
    virtual fastiss::i1_t *gen_read_FXST_IF(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXST_XP register field
    * \return Object representing the value of the rh850g4 FXST_XP register field
    */
    virtual fastiss::i8_t *gen_read_FXST_XP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FXINFO register
     * \return Object representing the value of the rh850g4 FXINFO register
     */
    virtual fastiss::i32_t *gen_read_FXINFO(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXINFO_NFPU register field
    * \return Object representing the value of the rh850g4 FXINFO_NFPU register field
    */
    virtual fastiss::i1_t *gen_read_FXINFO_NFPU(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXINFO_RSIZE register field
    * \return Object representing the value of the rh850g4 FXINFO_RSIZE register field
    */
    virtual fastiss::i1_t *gen_read_FXINFO_RSIZE(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FXCFG register
     * \return Object representing the value of the rh850g4 FXCFG register
     */
    virtual fastiss::i32_t *gen_read_FXCFG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXCFG_RM register field
    * \return Object representing the value of the rh850g4 FXCFG_RM register field
    */
    virtual fastiss::i8_t *gen_read_FXCFG_RM(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXCFG_XE register field
    * \return Object representing the value of the rh850g4 FXCFG_XE register field
    */
    virtual fastiss::i8_t *gen_read_FXCFG_XE(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FXXC register
     * \return Object representing the value of the rh850g4 FXXC register
     */
    virtual fastiss::i32_t *gen_read_FXXC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXXC_XC3 register field
    * \return Object representing the value of the rh850g4 FXXC_XC3 register field
    */
    virtual fastiss::i8_t *gen_read_FXXC_XC3(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXXC_XC2 register field
    * \return Object representing the value of the rh850g4 FXXC_XC2 register field
    */
    virtual fastiss::i8_t *gen_read_FXXC_XC2(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXXC_XC1 register field
    * \return Object representing the value of the rh850g4 FXXC_XC1 register field
    */
    virtual fastiss::i8_t *gen_read_FXXC_XC1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXXC_XC0 register field
    * \return Object representing the value of the rh850g4 FXXC_XC0 register field
    */
    virtual fastiss::i8_t *gen_read_FXXC_XC0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FXXP register
     * \return Object representing the value of the rh850g4 FXXP register
     */
    virtual fastiss::i32_t *gen_read_FXXP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXXP_XP3 register field
    * \return Object representing the value of the rh850g4 FXXP_XP3 register field
    */
    virtual fastiss::i8_t *gen_read_FXXP_XP3(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXXP_XP2 register field
    * \return Object representing the value of the rh850g4 FXXP_XP2 register field
    */
    virtual fastiss::i8_t *gen_read_FXXP_XP2(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXXP_XP1 register field
    * \return Object representing the value of the rh850g4 FXXP_XP1 register field
    */
    virtual fastiss::i8_t *gen_read_FXXP_XP1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXXP_XP0 register field
    * \return Object representing the value of the rh850g4 FXXP_XP0 register field
    */
    virtual fastiss::i8_t *gen_read_FXXP_XP0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMEIPC register
     * \return Object representing the value of the rh850g4 GMEIPC register
     */
    virtual fastiss::i32_t *gen_read_GMEIPC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMEIPSW register
     * \return Object representing the value of the rh850g4 GMEIPSW register
     */
    virtual fastiss::i32_t *gen_read_GMEIPSW(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMFEPC register
     * \return Object representing the value of the rh850g4 GMFEPC register
     */
    virtual fastiss::i32_t *gen_read_GMFEPC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMFEPSW register
     * \return Object representing the value of the rh850g4 GMFEPSW register
     */
    virtual fastiss::i32_t *gen_read_GMFEPSW(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMPSW register
     * \return Object representing the value of the rh850g4 GMPSW register
     */
    virtual fastiss::i32_t *gen_read_GMPSW(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_Z register field
    * \return Object representing the value of the rh850g4 GMPSW_Z register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_Z(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_S register field
    * \return Object representing the value of the rh850g4 GMPSW_S register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_S(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_OV register field
    * \return Object representing the value of the rh850g4 GMPSW_OV register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_OV(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_CY register field
    * \return Object representing the value of the rh850g4 GMPSW_CY register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_CY(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_SAT register field
    * \return Object representing the value of the rh850g4 GMPSW_SAT register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_SAT(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_ID register field
    * \return Object representing the value of the rh850g4 GMPSW_ID register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_ID(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_EP register field
    * \return Object representing the value of the rh850g4 GMPSW_EP register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_EP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_NP register field
    * \return Object representing the value of the rh850g4 GMPSW_NP register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_NP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_EBV register field
    * \return Object representing the value of the rh850g4 GMPSW_EBV register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_EBV(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_CU0 register field
    * \return Object representing the value of the rh850g4 GMPSW_CU0 register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_CU0(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_CU1 register field
    * \return Object representing the value of the rh850g4 GMPSW_CU1 register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_CU1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_CU2 register field
    * \return Object representing the value of the rh850g4 GMPSW_CU2 register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_CU2(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_EIMASK register field
    * \return Object representing the value of the rh850g4 GMPSW_EIMASK register field
    */
    virtual fastiss::i8_t *gen_read_GMPSW_EIMASK(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_UM register field
    * \return Object representing the value of the rh850g4 GMPSW_UM register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_UM(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMMEA register
     * \return Object representing the value of the rh850g4 GMMEA register
     */
    virtual fastiss::i32_t *gen_read_GMMEA(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMMEI register
     * \return Object representing the value of the rh850g4 GMMEI register
     */
    virtual fastiss::i32_t *gen_read_GMMEI(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMEIIC register
     * \return Object representing the value of the rh850g4 GMEIIC register
     */
    virtual fastiss::i32_t *gen_read_GMEIIC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMFEIC register
     * \return Object representing the value of the rh850g4 GMFEIC register
     */
    virtual fastiss::i32_t *gen_read_GMFEIC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMSPID register
     * \return Object representing the value of the rh850g4 GMSPID register
     */
    virtual fastiss::i32_t *gen_read_GMSPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMSPIDLIST register
     * \return Object representing the value of the rh850g4 GMSPIDLIST register
     */
    virtual fastiss::i32_t *gen_read_GMSPIDLIST(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMEBASE register
     * \return Object representing the value of the rh850g4 GMEBASE register
     */
    virtual fastiss::i32_t *gen_read_GMEBASE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMEBASE_EBASE register field
    * \return Object representing the value of the rh850g4 GMEBASE_EBASE register field
    */
    virtual fastiss::i32_t *gen_read_GMEBASE_EBASE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMEBASE_DV register field
    * \return Object representing the value of the rh850g4 GMEBASE_DV register field
    */
    virtual fastiss::i1_t *gen_read_GMEBASE_DV(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMEBASE_RINT register field
    * \return Object representing the value of the rh850g4 GMEBASE_RINT register field
    */
    virtual fastiss::i1_t *gen_read_GMEBASE_RINT(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMINTBP register
     * \return Object representing the value of the rh850g4 GMINTBP register
     */
    virtual fastiss::i32_t *gen_read_GMINTBP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMINTCFG register
     * \return Object representing the value of the rh850g4 GMINTCFG register
     */
    virtual fastiss::i32_t *gen_read_GMINTCFG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMINTCFG_ISPC register field
    * \return Object representing the value of the rh850g4 GMINTCFG_ISPC register field
    */
    virtual fastiss::i1_t *gen_read_GMINTCFG_ISPC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMINTCFG_EPL register field
    * \return Object representing the value of the rh850g4 GMINTCFG_EPL register field
    */
    virtual fastiss::i1_t *gen_read_GMINTCFG_EPL(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMINTCFG_ULNR register field
    * \return Object representing the value of the rh850g4 GMINTCFG_ULNR register field
    */
    virtual fastiss::i8_t *gen_read_GMINTCFG_ULNR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMPLMR register
     * \return Object representing the value of the rh850g4 GMPLMR register
     */
    virtual fastiss::i32_t *gen_read_GMPLMR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPLMR_PLM register field
    * \return Object representing the value of the rh850g4 GMPLMR_PLM register field
    */
    virtual fastiss::i8_t *gen_read_GMPLMR_PLM(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMSVLOCK register
     * \return Object representing the value of the rh850g4 GMSVLOCK register
     */
    virtual fastiss::i32_t *gen_read_GMSVLOCK(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMMPM register
     * \return Object representing the value of the rh850g4 GMMPM register
     */
    virtual fastiss::i32_t *gen_read_GMMPM(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMEIWR register
     * \return Object representing the value of the rh850g4 GMEIWR register
     */
    virtual fastiss::i32_t *gen_read_GMEIWR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMFEWR register
     * \return Object representing the value of the rh850g4 GMFEWR register
     */
    virtual fastiss::i32_t *gen_read_GMFEWR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMPEID register
     * \return Object representing the value of the rh850g4 GMPEID register
     */
    virtual fastiss::i32_t *gen_read_GMPEID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPM register
     * \return Object representing the value of the rh850g4 MPM register
     */
    virtual fastiss::i32_t *gen_read_MPM(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPCFG register
     * \return Object representing the value of the rh850g4 MPCFG register
     */
    virtual fastiss::i32_t *gen_read_MPCFG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPCFG_NMPUE register field
    * \return Object representing the value of the rh850g4 MPCFG_NMPUE register field
    */
    virtual fastiss::i8_t *gen_read_MPCFG_NMPUE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPCFG_HBE register field
    * \return Object representing the value of the rh850g4 MPCFG_HBE register field
    */
    virtual fastiss::i8_t *gen_read_MPCFG_HBE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPCFG_ARCH register field
    * \return Object representing the value of the rh850g4 MPCFG_ARCH register field
    */
    virtual fastiss::i8_t *gen_read_MPCFG_ARCH(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPCFG_NBK register field
    * \return Object representing the value of the rh850g4 MPCFG_NBK register field
    */
    virtual fastiss::i8_t *gen_read_MPCFG_NBK(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPCFG_DMDP register field
    * \return Object representing the value of the rh850g4 MPCFG_DMDP register field
    */
    virtual fastiss::i1_t *gen_read_MPCFG_DMDP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MCA register
     * \return Object representing the value of the rh850g4 MCA register
     */
    virtual fastiss::i32_t *gen_read_MCA(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MCS register
     * \return Object representing the value of the rh850g4 MCS register
     */
    virtual fastiss::i32_t *gen_read_MCS(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MCC register
     * \return Object representing the value of the rh850g4 MCC register
     */
    virtual fastiss::i32_t *gen_read_MCC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MCR register
     * \return Object representing the value of the rh850g4 MCR register
     */
    virtual fastiss::i32_t *gen_read_MCR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_HSXE register field
    * \return Object representing the value of the rh850g4 MCR_HSXE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_HSXE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_HSWE register field
    * \return Object representing the value of the rh850g4 MCR_HSWE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_HSWE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_HSRE register field
    * \return Object representing the value of the rh850g4 MCR_HSRE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_HSRE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_HUXE register field
    * \return Object representing the value of the rh850g4 MCR_HUXE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_HUXE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_HUWE register field
    * \return Object representing the value of the rh850g4 MCR_HUWE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_HUWE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_HURE register field
    * \return Object representing the value of the rh850g4 MCR_HURE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_HURE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_GSXE register field
    * \return Object representing the value of the rh850g4 MCR_GSXE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_GSXE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_GSWE register field
    * \return Object representing the value of the rh850g4 MCR_GSWE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_GSWE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_GSRE register field
    * \return Object representing the value of the rh850g4 MCR_GSRE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_GSRE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_GUXE register field
    * \return Object representing the value of the rh850g4 MCR_GUXE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_GUXE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_GUWE register field
    * \return Object representing the value of the rh850g4 MCR_GUWE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_GUWE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_GURE register field
    * \return Object representing the value of the rh850g4 MCR_GURE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_GURE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_OV register field
    * \return Object representing the value of the rh850g4 MCR_OV register field
    */
    virtual fastiss::i1_t *gen_read_MCR_OV(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_SXE register field
    * \return Object representing the value of the rh850g4 MCR_SXE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_SXE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_SWE register field
    * \return Object representing the value of the rh850g4 MCR_SWE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_SWE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_SRE register field
    * \return Object representing the value of the rh850g4 MCR_SRE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_SRE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_UXE register field
    * \return Object representing the value of the rh850g4 MCR_UXE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_UXE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_UWE register field
    * \return Object representing the value of the rh850g4 MCR_UWE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_UWE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_URE register field
    * \return Object representing the value of the rh850g4 MCR_URE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_URE(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MCI register
     * \return Object representing the value of the rh850g4 MCI register
     */
    virtual fastiss::i32_t *gen_read_MCI(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCI_SPID register field
    * \return Object representing the value of the rh850g4 MCI_SPID register field
    */
    virtual fastiss::i8_t *gen_read_MCI_SPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPIDX register
     * \return Object representing the value of the rh850g4 MPIDX register
     */
    virtual fastiss::i32_t *gen_read_MPIDX(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPIDX_IDX register field
    * \return Object representing the value of the rh850g4 MPIDX_IDX register field
    */
    virtual fastiss::i8_t *gen_read_MPIDX_IDX(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPBK register
     * \return Object representing the value of the rh850g4 MPBK register
     */
    virtual fastiss::i32_t *gen_read_MPBK(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPBK_BK register field
    * \return Object representing the value of the rh850g4 MPBK_BK register field
    */
    virtual fastiss::i8_t *gen_read_MPBK_BK(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPLA register
     * \return Object representing the value of the rh850g4 MPLA register
     */
    virtual fastiss::i32_t *gen_read_MPLA(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPUA register
     * \return Object representing the value of the rh850g4 MPUA register
     */
    virtual fastiss::i32_t *gen_read_MPUA(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPAT register
     * \return Object representing the value of the rh850g4 MPAT register
     */
    virtual fastiss::i32_t *gen_read_MPAT(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID7 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID7 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_WMPID7(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID6 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID6 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_WMPID6(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID5 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID5 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_WMPID5(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID4 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID4 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_WMPID4(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID3 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID3 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_WMPID3(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID2 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID2 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_WMPID2(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID1 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID1 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_WMPID1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID0 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID0 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_WMPID0(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID7 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID7 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_RMPID7(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID6 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID6 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_RMPID6(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID5 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID5 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_RMPID5(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID4 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID4 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_RMPID4(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID3 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID3 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_RMPID3(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID2 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID2 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_RMPID2(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID1 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID1 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_RMPID1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID0 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID0 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_RMPID0(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WG register field
    * \return Object representing the value of the rh850g4 MPAT_WG register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_WG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RG register field
    * \return Object representing the value of the rh850g4 MPAT_RG register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_RG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_E register field
    * \return Object representing the value of the rh850g4 MPAT_E register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_E(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_SX register field
    * \return Object representing the value of the rh850g4 MPAT_SX register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_SX(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_SW register field
    * \return Object representing the value of the rh850g4 MPAT_SW register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_SW(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_SR register field
    * \return Object representing the value of the rh850g4 MPAT_SR register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_SR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_UX register field
    * \return Object representing the value of the rh850g4 MPAT_UX register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_UX(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_UW register field
    * \return Object representing the value of the rh850g4 MPAT_UW register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_UW(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_UR register field
    * \return Object representing the value of the rh850g4 MPAT_UR register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_UR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPID0 register
     * \return Object representing the value of the rh850g4 MPID0 register
     */
    virtual fastiss::i32_t *gen_read_MPID0(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPID0_SPID register field
    * \return Object representing the value of the rh850g4 MPID0_SPID register field
    */
    virtual fastiss::i8_t *gen_read_MPID0_SPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPID1 register
     * \return Object representing the value of the rh850g4 MPID1 register
     */
    virtual fastiss::i32_t *gen_read_MPID1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPID1_SPID register field
    * \return Object representing the value of the rh850g4 MPID1_SPID register field
    */
    virtual fastiss::i8_t *gen_read_MPID1_SPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPID2 register
     * \return Object representing the value of the rh850g4 MPID2 register
     */
    virtual fastiss::i32_t *gen_read_MPID2(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPID2_SPID register field
    * \return Object representing the value of the rh850g4 MPID2_SPID register field
    */
    virtual fastiss::i8_t *gen_read_MPID2_SPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPID3 register
     * \return Object representing the value of the rh850g4 MPID3 register
     */
    virtual fastiss::i32_t *gen_read_MPID3(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPID3_SPID register field
    * \return Object representing the value of the rh850g4 MPID3_SPID register field
    */
    virtual fastiss::i8_t *gen_read_MPID3_SPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPID4 register
     * \return Object representing the value of the rh850g4 MPID4 register
     */
    virtual fastiss::i32_t *gen_read_MPID4(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPID4_SPID register field
    * \return Object representing the value of the rh850g4 MPID4_SPID register field
    */
    virtual fastiss::i8_t *gen_read_MPID4_SPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPID5 register
     * \return Object representing the value of the rh850g4 MPID5 register
     */
    virtual fastiss::i32_t *gen_read_MPID5(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPID5_SPID register field
    * \return Object representing the value of the rh850g4 MPID5_SPID register field
    */
    virtual fastiss::i8_t *gen_read_MPID5_SPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPID6 register
     * \return Object representing the value of the rh850g4 MPID6 register
     */
    virtual fastiss::i32_t *gen_read_MPID6(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPID6_SPID register field
    * \return Object representing the value of the rh850g4 MPID6_SPID register field
    */
    virtual fastiss::i8_t *gen_read_MPID6_SPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPID7 register
     * \return Object representing the value of the rh850g4 MPID7 register
     */
    virtual fastiss::i32_t *gen_read_MPID7(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPID7_SPID register field
    * \return Object representing the value of the rh850g4 MPID7_SPID register field
    */
    virtual fastiss::i8_t *gen_read_MPID7_SPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 ICTAGL register
     * \return Object representing the value of the rh850g4 ICTAGL register
     */
    virtual fastiss::i32_t *gen_read_ICTAGL(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGL_LPN register field
    * \return Object representing the value of the rh850g4 ICTAGL_LPN register field
    */
    virtual fastiss::i32_t *gen_read_ICTAGL_LPN(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGL_LRU register field
    * \return Object representing the value of the rh850g4 ICTAGL_LRU register field
    */
    virtual fastiss::i8_t *gen_read_ICTAGL_LRU(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGL_L register field
    * \return Object representing the value of the rh850g4 ICTAGL_L register field
    */
    virtual fastiss::i1_t *gen_read_ICTAGL_L(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGL_V register field
    * \return Object representing the value of the rh850g4 ICTAGL_V register field
    */
    virtual fastiss::i1_t *gen_read_ICTAGL_V(void);
    /**
     * \brief Generates code to query the value of the rh850g4 ICTAGH register
     * \return Object representing the value of the rh850g4 ICTAGH register
     */
    virtual fastiss::i32_t *gen_read_ICTAGH(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGH_WD register field
    * \return Object representing the value of the rh850g4 ICTAGH_WD register field
    */
    virtual fastiss::i1_t *gen_read_ICTAGH_WD(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGH_PD register field
    * \return Object representing the value of the rh850g4 ICTAGH_PD register field
    */
    virtual fastiss::i1_t *gen_read_ICTAGH_PD(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGH_WT register field
    * \return Object representing the value of the rh850g4 ICTAGH_WT register field
    */
    virtual fastiss::i1_t *gen_read_ICTAGH_WT(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGH_PT register field
    * \return Object representing the value of the rh850g4 ICTAGH_PT register field
    */
    virtual fastiss::i1_t *gen_read_ICTAGH_PT(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGH_DATAECC register field
    * \return Object representing the value of the rh850g4 ICTAGH_DATAECC register field
    */
    virtual fastiss::i8_t *gen_read_ICTAGH_DATAECC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGH_TAGECC register field
    * \return Object representing the value of the rh850g4 ICTAGH_TAGECC register field
    */
    virtual fastiss::i8_t *gen_read_ICTAGH_TAGECC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 ICDATL register
     * \return Object representing the value of the rh850g4 ICDATL register
     */
    virtual fastiss::i32_t *gen_read_ICDATL(void);
    /**
     * \brief Generates code to query the value of the rh850g4 ICDATH register
     * \return Object representing the value of the rh850g4 ICDATH register
     */
    virtual fastiss::i32_t *gen_read_ICDATH(void);
    /**
     * \brief Generates code to query the value of the rh850g4 ICCTRL register
     * \return Object representing the value of the rh850g4 ICCTRL register
     */
    virtual fastiss::i32_t *gen_read_ICCTRL(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICCTRL_PBS register field
    * \return Object representing the value of the rh850g4 ICCTRL_PBS register field
    */
    virtual fastiss::i1_t *gen_read_ICCTRL_PBS(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICCTRL_ICHCLR register field
    * \return Object representing the value of the rh850g4 ICCTRL_ICHCLR register field
    */
    virtual fastiss::i1_t *gen_read_ICCTRL_ICHCLR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICCTRL_ICHEIV register field
    * \return Object representing the value of the rh850g4 ICCTRL_ICHEIV register field
    */
    virtual fastiss::i1_t *gen_read_ICCTRL_ICHEIV(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICCTRL_ICHEMK register field
    * \return Object representing the value of the rh850g4 ICCTRL_ICHEMK register field
    */
    virtual fastiss::i1_t *gen_read_ICCTRL_ICHEMK(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICCTRL_ICHEN register field
    * \return Object representing the value of the rh850g4 ICCTRL_ICHEN register field
    */
    virtual fastiss::i1_t *gen_read_ICCTRL_ICHEN(void);
    /**
     * \brief Generates code to query the value of the rh850g4 ICCFG register
     * \return Object representing the value of the rh850g4 ICCFG register
     */
    virtual fastiss::i32_t *gen_read_ICCFG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICCFG_ICHSIZE register field
    * \return Object representing the value of the rh850g4 ICCFG_ICHSIZE register field
    */
    virtual fastiss::i8_t *gen_read_ICCFG_ICHSIZE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICCFG_ICHLINE register field
    * \return Object representing the value of the rh850g4 ICCFG_ICHLINE register field
    */
    virtual fastiss::i8_t *gen_read_ICCFG_ICHLINE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICCFG_ICHWAY register field
    * \return Object representing the value of the rh850g4 ICCFG_ICHWAY register field
    */
    virtual fastiss::i8_t *gen_read_ICCFG_ICHWAY(void);
    /**
     * \brief Generates code to query the value of the rh850g4 ICERR register
     * \return Object representing the value of the rh850g4 ICERR register
     */
    virtual fastiss::i32_t *gen_read_ICERR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_CISTW register field
    * \return Object representing the value of the rh850g4 ICERR_CISTW register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_CISTW(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ESAFE register field
    * \return Object representing the value of the rh850g4 ICERR_ESAFE register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ESAFE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ESMH register field
    * \return Object representing the value of the rh850g4 ICERR_ESMH register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ESMH(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ESPBSE register field
    * \return Object representing the value of the rh850g4 ICERR_ESPBSE register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ESPBSE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ESTE1 register field
    * \return Object representing the value of the rh850g4 ICERR_ESTE1 register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ESTE1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ESTE2 register field
    * \return Object representing the value of the rh850g4 ICERR_ESTE2 register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ESTE2(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ESDC register field
    * \return Object representing the value of the rh850g4 ICERR_ESDC register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ESDC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ESDE register field
    * \return Object representing the value of the rh850g4 ICERR_ESDE register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ESDE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ERMAFE register field
    * \return Object representing the value of the rh850g4 ICERR_ERMAFE register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ERMAFE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ERMMH register field
    * \return Object representing the value of the rh850g4 ICERR_ERMMH register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ERMMH(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ERMPBSE register field
    * \return Object representing the value of the rh850g4 ICERR_ERMPBSE register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ERMPBSE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ERMTE1 register field
    * \return Object representing the value of the rh850g4 ICERR_ERMTE1 register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ERMTE1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ERMTE2 register field
    * \return Object representing the value of the rh850g4 ICERR_ERMTE2 register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ERMTE2(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ERMDC register field
    * \return Object representing the value of the rh850g4 ICERR_ERMDC register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ERMDC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ERMDE register field
    * \return Object representing the value of the rh850g4 ICERR_ERMDE register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ERMDE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ICHEWY register field
    * \return Object representing the value of the rh850g4 ICERR_ICHEWY register field
    */
    virtual fastiss::i8_t *gen_read_ICERR_ICHEWY(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ICHEIX register field
    * \return Object representing the value of the rh850g4 ICERR_ICHEIX register field
    */
    virtual fastiss::i8_t *gen_read_ICERR_ICHEIX(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ICHERQ register field
    * \return Object representing the value of the rh850g4 ICERR_ICHERQ register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ICHERQ(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ICHED register field
    * \return Object representing the value of the rh850g4 ICERR_ICHED register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ICHED(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ICHET register field
    * \return Object representing the value of the rh850g4 ICERR_ICHET register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ICHET(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ICHERR register field
    * \return Object representing the value of the rh850g4 ICERR_ICHERR register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ICHERR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 TSCOUNTL register
     * \return Object representing the value of the rh850g4 TSCOUNTL register
     */
    virtual fastiss::i32_t *gen_read_TSCOUNTL(void);
    /**
     * \brief Generates code to query the value of the rh850g4 TSCOUNTH register
     * \return Object representing the value of the rh850g4 TSCOUNTH register
     */
    virtual fastiss::i32_t *gen_read_TSCOUNTH(void);
    /**
     * \brief Generates code to query the value of the rh850g4 TSCTRL register
     * \return Object representing the value of the rh850g4 TSCTRL register
     */
    virtual fastiss::i32_t *gen_read_TSCTRL(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMUMCTRL register
     * \return Object representing the value of the rh850g4 PMUMCTRL register
     */
    virtual fastiss::i32_t *gen_read_PMUMCTRL(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMGMCTRL register
     * \return Object representing the value of the rh850g4 PMGMCTRL register
     */
    virtual fastiss::i32_t *gen_read_PMGMCTRL(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT0 register
     * \return Object representing the value of the rh850g4 PMCOUNT0 register
     */
    virtual fastiss::i32_t *gen_read_PMCOUNT0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL0 register
     * \return Object representing the value of the rh850g4 PMCTRL0 register
     */
    virtual fastiss::i32_t *gen_read_PMCTRL0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT1 register
     * \return Object representing the value of the rh850g4 PMCOUNT1 register
     */
    virtual fastiss::i32_t *gen_read_PMCOUNT1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL1 register
     * \return Object representing the value of the rh850g4 PMCTRL1 register
     */
    virtual fastiss::i32_t *gen_read_PMCTRL1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT2 register
     * \return Object representing the value of the rh850g4 PMCOUNT2 register
     */
    virtual fastiss::i32_t *gen_read_PMCOUNT2(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL2 register
     * \return Object representing the value of the rh850g4 PMCTRL2 register
     */
    virtual fastiss::i32_t *gen_read_PMCTRL2(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT3 register
     * \return Object representing the value of the rh850g4 PMCOUNT3 register
     */
    virtual fastiss::i32_t *gen_read_PMCOUNT3(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL3 register
     * \return Object representing the value of the rh850g4 PMCTRL3 register
     */
    virtual fastiss::i32_t *gen_read_PMCTRL3(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL4 register
     * \return Object representing the value of the rh850g4 PMCTRL4 register
     */
    virtual fastiss::i32_t *gen_read_PMCTRL4(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL5 register
     * \return Object representing the value of the rh850g4 PMCTRL5 register
     */
    virtual fastiss::i32_t *gen_read_PMCTRL5(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL6 register
     * \return Object representing the value of the rh850g4 PMCTRL6 register
     */
    virtual fastiss::i32_t *gen_read_PMCTRL6(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL7 register
     * \return Object representing the value of the rh850g4 PMCTRL7 register
     */
    virtual fastiss::i32_t *gen_read_PMCTRL7(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT4 register
     * \return Object representing the value of the rh850g4 PMCOUNT4 register
     */
    virtual fastiss::i32_t *gen_read_PMCOUNT4(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT5 register
     * \return Object representing the value of the rh850g4 PMCOUNT5 register
     */
    virtual fastiss::i32_t *gen_read_PMCOUNT5(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT6 register
     * \return Object representing the value of the rh850g4 PMCOUNT6 register
     */
    virtual fastiss::i32_t *gen_read_PMCOUNT6(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT7 register
     * \return Object representing the value of the rh850g4 PMCOUNT7 register
     */
    virtual fastiss::i32_t *gen_read_PMCOUNT7(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND0 register
     * \return Object representing the value of the rh850g4 PMSUBCND0 register
     */
    virtual fastiss::i32_t *gen_read_PMSUBCND0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND1 register
     * \return Object representing the value of the rh850g4 PMSUBCND1 register
     */
    virtual fastiss::i32_t *gen_read_PMSUBCND1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND2 register
     * \return Object representing the value of the rh850g4 PMSUBCND2 register
     */
    virtual fastiss::i32_t *gen_read_PMSUBCND2(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND3 register
     * \return Object representing the value of the rh850g4 PMSUBCND3 register
     */
    virtual fastiss::i32_t *gen_read_PMSUBCND3(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND4 register
     * \return Object representing the value of the rh850g4 PMSUBCND4 register
     */
    virtual fastiss::i32_t *gen_read_PMSUBCND4(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND5 register
     * \return Object representing the value of the rh850g4 PMSUBCND5 register
     */
    virtual fastiss::i32_t *gen_read_PMSUBCND5(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND6 register
     * \return Object representing the value of the rh850g4 PMSUBCND6 register
     */
    virtual fastiss::i32_t *gen_read_PMSUBCND6(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND7 register
     * \return Object representing the value of the rh850g4 PMSUBCND7 register
     */
    virtual fastiss::i32_t *gen_read_PMSUBCND7(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DBGEN register
     * \return Object representing the value of the rh850g4 DBGEN register
     */
    virtual fastiss::i32_t *gen_read_DBGEN(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DBPSWH register
     * \return Object representing the value of the rh850g4 DBPSWH register
     */
    virtual fastiss::i32_t *gen_read_DBPSWH(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DBIC register
     * \return Object representing the value of the rh850g4 DBIC register
     */
    virtual fastiss::i32_t *gen_read_DBIC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DBPC register
     * \return Object representing the value of the rh850g4 DBPC register
     */
    virtual fastiss::i32_t *gen_read_DBPC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DBPSW register
     * \return Object representing the value of the rh850g4 DBPSW register
     */
    virtual fastiss::i32_t *gen_read_DBPSW(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DIR0 register
     * \return Object representing the value of the rh850g4 DIR0 register
     */
    virtual fastiss::i32_t *gen_read_DIR0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DIR1 register
     * \return Object representing the value of the rh850g4 DIR1 register
     */
    virtual fastiss::i32_t *gen_read_DIR1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 BPC register
     * \return Object representing the value of the rh850g4 BPC register
     */
    virtual fastiss::i32_t *gen_read_BPC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 BPAV register
     * \return Object representing the value of the rh850g4 BPAV register
     */
    virtual fastiss::i32_t *gen_read_BPAV(void);
    /**
     * \brief Generates code to query the value of the rh850g4 BPAM register
     * \return Object representing the value of the rh850g4 BPAM register
     */
    virtual fastiss::i32_t *gen_read_BPAM(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DBCFG register
     * \return Object representing the value of the rh850g4 DBCFG register
     */
    virtual fastiss::i32_t *gen_read_DBCFG(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DBWR register
     * \return Object representing the value of the rh850g4 DBWR register
     */
    virtual fastiss::i32_t *gen_read_DBWR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 LSTEST0 register
     * \return Object representing the value of the rh850g4 LSTEST0 register
     */
    virtual fastiss::i32_t *gen_read_LSTEST0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 LSTEST1 register
     * \return Object representing the value of the rh850g4 LSTEST1 register
     */
    virtual fastiss::i32_t *gen_read_LSTEST1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 LSCFG register
     * \return Object representing the value of the rh850g4 LSCFG register
     */
    virtual fastiss::i32_t *gen_read_LSCFG(void);
    /**
     * \brief Generates code to query the value of the rh850g4 ICBKEY register
     * \return Object representing the value of the rh850g4 ICBKEY register
     */
    virtual fastiss::i32_t *gen_read_ICBKEY(void);
    /**
     * \brief Generates code to query the value of the rh850g4 IFCR register
     * \return Object representing the value of the rh850g4 IFCR register
     */
    virtual fastiss::i32_t *gen_read_IFCR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 IFCR1 register
     * \return Object representing the value of the rh850g4 IFCR1 register
     */
    virtual fastiss::i32_t *gen_read_IFCR1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 BRPCTRL0 register
     * \return Object representing the value of the rh850g4 BRPCTRL0 register
     */
    virtual fastiss::i32_t *gen_read_BRPCTRL0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 BRPCTRL1 register
     * \return Object representing the value of the rh850g4 BRPCTRL1 register
     */
    virtual fastiss::i32_t *gen_read_BRPCTRL1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 BRPCFG register
     * \return Object representing the value of the rh850g4 BRPCFG register
     */
    virtual fastiss::i32_t *gen_read_BRPCFG(void);
    /**
     * \brief Generates code to query the value of the rh850g4 BRPACTRL register
     * \return Object representing the value of the rh850g4 BRPACTRL register
     */
    virtual fastiss::i32_t *gen_read_BRPACTRL(void);
    /**
     * \brief Generates code to query the value of the rh850g4 BRPDATA register
     * \return Object representing the value of the rh850g4 BRPDATA register
     */
    virtual fastiss::i32_t *gen_read_BRPDATA(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DCBKEY register
     * \return Object representing the value of the rh850g4 DCBKEY register
     */
    virtual fastiss::i32_t *gen_read_DCBKEY(void);
    /**
     * \brief Generates code to query the value of the rh850g4 LSUCR register
     * \return Object representing the value of the rh850g4 LSUCR register
     */
    virtual fastiss::i32_t *gen_read_LSUCR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 LSULNK0 register
     * \return Object representing the value of the rh850g4 LSULNK0 register
     */
    virtual fastiss::i32_t *gen_read_LSULNK0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 LSULNK1 register
     * \return Object representing the value of the rh850g4 LSULNK1 register
     */
    virtual fastiss::i32_t *gen_read_LSULNK1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 L1RLCR register
     * \return Object representing the value of the rh850g4 L1RLCR register
     */
    virtual fastiss::i32_t *gen_read_L1RLCR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 L1RLNK0 register
     * \return Object representing the value of the rh850g4 L1RLNK0 register
     */
    virtual fastiss::i32_t *gen_read_L1RLNK0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 L1RLNK1 register
     * \return Object representing the value of the rh850g4 L1RLNK1 register
     */
    virtual fastiss::i32_t *gen_read_L1RLNK1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 L1RCFG register
     * \return Object representing the value of the rh850g4 L1RCFG register
     */
    virtual fastiss::i32_t *gen_read_L1RCFG(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DECFG register
     * \return Object representing the value of the rh850g4 DECFG register
     */
    virtual fastiss::i32_t *gen_read_DECFG(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DECTRL register
     * \return Object representing the value of the rh850g4 DECTRL register
     */
    virtual fastiss::i32_t *gen_read_DECTRL(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DEVDS register
     * \return Object representing the value of the rh850g4 DEVDS register
     */
    virtual fastiss::i32_t *gen_read_DEVDS(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RDBCR register
     * \return Object representing the value of the rh850g4 RDBCR register
     */
    virtual fastiss::i32_t *gen_read_RDBCR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RDBACR register
     * \return Object representing the value of the rh850g4 RDBACR register
     */
    virtual fastiss::i32_t *gen_read_RDBACR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RDBATAG register
     * \return Object representing the value of the rh850g4 RDBATAG register
     */
    virtual fastiss::i32_t *gen_read_RDBATAG(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RDBADAT0 register
     * \return Object representing the value of the rh850g4 RDBADAT0 register
     */
    virtual fastiss::i32_t *gen_read_RDBADAT0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RDBADAT1 register
     * \return Object representing the value of the rh850g4 RDBADAT1 register
     */
    virtual fastiss::i32_t *gen_read_RDBADAT1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RDBADAT2 register
     * \return Object representing the value of the rh850g4 RDBADAT2 register
     */
    virtual fastiss::i32_t *gen_read_RDBADAT2(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RDBADAT3 register
     * \return Object representing the value of the rh850g4 RDBADAT3 register
     */
    virtual fastiss::i32_t *gen_read_RDBADAT3(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RDBSTAT register
     * \return Object representing the value of the rh850g4 RDBSTAT register
     */
    virtual fastiss::i32_t *gen_read_RDBSTAT(void);
    /**
     * \brief Generates code to query the value of the rh850g4 hv_func_support register
     * \return Object representing the value of the rh850g4 hv_func_support register
     */
    virtual fastiss::i8_t *gen_read_hv_func_support(void);
    /**
     * \brief Generates code to query the value of the rh850g4 swd0pcad_exec_count register
     * \return Object representing the value of the rh850g4 swd0pcad_exec_count register
     */
    virtual fastiss::i32_t *gen_read_swd0pcad_exec_count(void);
    /**
     * \brief Generates code to query the value of the rh850g4 swd0pcad_val register
     * \return Object representing the value of the rh850g4 swd0pcad_val register
     */
    virtual fastiss::i32_t *gen_read_swd0pcad_val(void);
    /**
     * \brief Generates code to query the value of the rh850g4 mem_acc_type register
     * \return Object representing the value of the rh850g4 mem_acc_type register
     */
    virtual fastiss::i32_t *gen_read_mem_acc_type(void);

    // generative register group read operations
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 GR register group
     * \param index Index value of the required register from the rh850g4 GR register group
     * \return Object representing the value of the specified rh850g4 GR group register
     */
    virtual fastiss::i32_t *gen_read_GR(rh850g4::GR_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 WR register group
     * \param index Index value of the required register from the rh850g4 WR register group
     * \return Object representing the value of the specified rh850g4 WR group register
     */
    virtual fastiss::i64_t *gen_read_WR(rh850g4::WR_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_0 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_0(rh850g4::SR_SELID_0_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_1 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_1(rh850g4::SR_SELID_1_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_2 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_2(rh850g4::SR_SELID_2_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_3 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_3(rh850g4::SR_SELID_3_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_4 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_4 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_4(rh850g4::SR_SELID_4_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_5 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_5(rh850g4::SR_SELID_5_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_9 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_9 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_9(rh850g4::SR_SELID_9_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_10 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_10 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_10(rh850g4::SR_SELID_10_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_11 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_11 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_11(rh850g4::SR_SELID_11_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_12 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_12 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_12(rh850g4::SR_SELID_12_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_13 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_13 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_13(rh850g4::SR_SELID_13_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_14 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_14 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_14(rh850g4::SR_SELID_14_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_15 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_15 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_15(rh850g4::SR_SELID_15_index_t index);

    // generative register and register field write operations
    /**
     * \brief Generates code to set the value of the rh850g4 r0 register
     * \param value Object representing the new value of the rh850g4 r0 register
     */
    virtual void gen_write_r0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r1 register
     * \param value Object representing the new value of the rh850g4 r1 register
     */
    virtual void gen_write_r1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r2 register
     * \param value Object representing the new value of the rh850g4 r2 register
     */
    virtual void gen_write_r2(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r3 register
     * \param value Object representing the new value of the rh850g4 r3 register
     */
    virtual void gen_write_r3(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r4 register
     * \param value Object representing the new value of the rh850g4 r4 register
     */
    virtual void gen_write_r4(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r5 register
     * \param value Object representing the new value of the rh850g4 r5 register
     */
    virtual void gen_write_r5(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r6 register
     * \param value Object representing the new value of the rh850g4 r6 register
     */
    virtual void gen_write_r6(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r7 register
     * \param value Object representing the new value of the rh850g4 r7 register
     */
    virtual void gen_write_r7(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r8 register
     * \param value Object representing the new value of the rh850g4 r8 register
     */
    virtual void gen_write_r8(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r9 register
     * \param value Object representing the new value of the rh850g4 r9 register
     */
    virtual void gen_write_r9(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r10 register
     * \param value Object representing the new value of the rh850g4 r10 register
     */
    virtual void gen_write_r10(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r11 register
     * \param value Object representing the new value of the rh850g4 r11 register
     */
    virtual void gen_write_r11(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r12 register
     * \param value Object representing the new value of the rh850g4 r12 register
     */
    virtual void gen_write_r12(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r13 register
     * \param value Object representing the new value of the rh850g4 r13 register
     */
    virtual void gen_write_r13(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r14 register
     * \param value Object representing the new value of the rh850g4 r14 register
     */
    virtual void gen_write_r14(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r15 register
     * \param value Object representing the new value of the rh850g4 r15 register
     */
    virtual void gen_write_r15(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r16 register
     * \param value Object representing the new value of the rh850g4 r16 register
     */
    virtual void gen_write_r16(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r17 register
     * \param value Object representing the new value of the rh850g4 r17 register
     */
    virtual void gen_write_r17(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r18 register
     * \param value Object representing the new value of the rh850g4 r18 register
     */
    virtual void gen_write_r18(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r19 register
     * \param value Object representing the new value of the rh850g4 r19 register
     */
    virtual void gen_write_r19(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r20 register
     * \param value Object representing the new value of the rh850g4 r20 register
     */
    virtual void gen_write_r20(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r21 register
     * \param value Object representing the new value of the rh850g4 r21 register
     */
    virtual void gen_write_r21(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r22 register
     * \param value Object representing the new value of the rh850g4 r22 register
     */
    virtual void gen_write_r22(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r23 register
     * \param value Object representing the new value of the rh850g4 r23 register
     */
    virtual void gen_write_r23(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r24 register
     * \param value Object representing the new value of the rh850g4 r24 register
     */
    virtual void gen_write_r24(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r25 register
     * \param value Object representing the new value of the rh850g4 r25 register
     */
    virtual void gen_write_r25(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r26 register
     * \param value Object representing the new value of the rh850g4 r26 register
     */
    virtual void gen_write_r26(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r27 register
     * \param value Object representing the new value of the rh850g4 r27 register
     */
    virtual void gen_write_r27(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r28 register
     * \param value Object representing the new value of the rh850g4 r28 register
     */
    virtual void gen_write_r28(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r29 register
     * \param value Object representing the new value of the rh850g4 r29 register
     */
    virtual void gen_write_r29(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r30 register
     * \param value Object representing the new value of the rh850g4 r30 register
     */
    virtual void gen_write_r30(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r31 register
     * \param value Object representing the new value of the rh850g4 r31 register
     */
    virtual void gen_write_r31(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PC register
     * \param value Object representing the new value of the rh850g4 PC register
     */
    virtual void gen_write_PC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr0 register
     * \param value Object representing the new value of the rh850g4 wr0 register
     */
    virtual void gen_write_wr0(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr0_u register
     * \param value Object representing the new value of the rh850g4 wr0_u register
     */
    virtual void gen_write_wr0_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr1 register
     * \param value Object representing the new value of the rh850g4 wr1 register
     */
    virtual void gen_write_wr1(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr1_u register
     * \param value Object representing the new value of the rh850g4 wr1_u register
     */
    virtual void gen_write_wr1_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr2 register
     * \param value Object representing the new value of the rh850g4 wr2 register
     */
    virtual void gen_write_wr2(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr2_u register
     * \param value Object representing the new value of the rh850g4 wr2_u register
     */
    virtual void gen_write_wr2_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr3 register
     * \param value Object representing the new value of the rh850g4 wr3 register
     */
    virtual void gen_write_wr3(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr3_u register
     * \param value Object representing the new value of the rh850g4 wr3_u register
     */
    virtual void gen_write_wr3_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr4 register
     * \param value Object representing the new value of the rh850g4 wr4 register
     */
    virtual void gen_write_wr4(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr4_u register
     * \param value Object representing the new value of the rh850g4 wr4_u register
     */
    virtual void gen_write_wr4_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr5 register
     * \param value Object representing the new value of the rh850g4 wr5 register
     */
    virtual void gen_write_wr5(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr5_u register
     * \param value Object representing the new value of the rh850g4 wr5_u register
     */
    virtual void gen_write_wr5_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr6 register
     * \param value Object representing the new value of the rh850g4 wr6 register
     */
    virtual void gen_write_wr6(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr6_u register
     * \param value Object representing the new value of the rh850g4 wr6_u register
     */
    virtual void gen_write_wr6_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr7 register
     * \param value Object representing the new value of the rh850g4 wr7 register
     */
    virtual void gen_write_wr7(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr7_u register
     * \param value Object representing the new value of the rh850g4 wr7_u register
     */
    virtual void gen_write_wr7_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr8 register
     * \param value Object representing the new value of the rh850g4 wr8 register
     */
    virtual void gen_write_wr8(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr8_u register
     * \param value Object representing the new value of the rh850g4 wr8_u register
     */
    virtual void gen_write_wr8_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr9 register
     * \param value Object representing the new value of the rh850g4 wr9 register
     */
    virtual void gen_write_wr9(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr9_u register
     * \param value Object representing the new value of the rh850g4 wr9_u register
     */
    virtual void gen_write_wr9_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr10 register
     * \param value Object representing the new value of the rh850g4 wr10 register
     */
    virtual void gen_write_wr10(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr10_u register
     * \param value Object representing the new value of the rh850g4 wr10_u register
     */
    virtual void gen_write_wr10_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr11 register
     * \param value Object representing the new value of the rh850g4 wr11 register
     */
    virtual void gen_write_wr11(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr11_u register
     * \param value Object representing the new value of the rh850g4 wr11_u register
     */
    virtual void gen_write_wr11_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr12 register
     * \param value Object representing the new value of the rh850g4 wr12 register
     */
    virtual void gen_write_wr12(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr12_u register
     * \param value Object representing the new value of the rh850g4 wr12_u register
     */
    virtual void gen_write_wr12_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr13 register
     * \param value Object representing the new value of the rh850g4 wr13 register
     */
    virtual void gen_write_wr13(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr13_u register
     * \param value Object representing the new value of the rh850g4 wr13_u register
     */
    virtual void gen_write_wr13_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr14 register
     * \param value Object representing the new value of the rh850g4 wr14 register
     */
    virtual void gen_write_wr14(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr14_u register
     * \param value Object representing the new value of the rh850g4 wr14_u register
     */
    virtual void gen_write_wr14_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr15 register
     * \param value Object representing the new value of the rh850g4 wr15 register
     */
    virtual void gen_write_wr15(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr15_u register
     * \param value Object representing the new value of the rh850g4 wr15_u register
     */
    virtual void gen_write_wr15_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr16 register
     * \param value Object representing the new value of the rh850g4 wr16 register
     */
    virtual void gen_write_wr16(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr16_u register
     * \param value Object representing the new value of the rh850g4 wr16_u register
     */
    virtual void gen_write_wr16_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr17 register
     * \param value Object representing the new value of the rh850g4 wr17 register
     */
    virtual void gen_write_wr17(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr17_u register
     * \param value Object representing the new value of the rh850g4 wr17_u register
     */
    virtual void gen_write_wr17_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr18 register
     * \param value Object representing the new value of the rh850g4 wr18 register
     */
    virtual void gen_write_wr18(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr18_u register
     * \param value Object representing the new value of the rh850g4 wr18_u register
     */
    virtual void gen_write_wr18_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr19 register
     * \param value Object representing the new value of the rh850g4 wr19 register
     */
    virtual void gen_write_wr19(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr19_u register
     * \param value Object representing the new value of the rh850g4 wr19_u register
     */
    virtual void gen_write_wr19_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr20 register
     * \param value Object representing the new value of the rh850g4 wr20 register
     */
    virtual void gen_write_wr20(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr20_u register
     * \param value Object representing the new value of the rh850g4 wr20_u register
     */
    virtual void gen_write_wr20_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr21 register
     * \param value Object representing the new value of the rh850g4 wr21 register
     */
    virtual void gen_write_wr21(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr21_u register
     * \param value Object representing the new value of the rh850g4 wr21_u register
     */
    virtual void gen_write_wr21_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr22 register
     * \param value Object representing the new value of the rh850g4 wr22 register
     */
    virtual void gen_write_wr22(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr22_u register
     * \param value Object representing the new value of the rh850g4 wr22_u register
     */
    virtual void gen_write_wr22_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr23 register
     * \param value Object representing the new value of the rh850g4 wr23 register
     */
    virtual void gen_write_wr23(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr23_u register
     * \param value Object representing the new value of the rh850g4 wr23_u register
     */
    virtual void gen_write_wr23_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr24 register
     * \param value Object representing the new value of the rh850g4 wr24 register
     */
    virtual void gen_write_wr24(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr24_u register
     * \param value Object representing the new value of the rh850g4 wr24_u register
     */
    virtual void gen_write_wr24_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr25 register
     * \param value Object representing the new value of the rh850g4 wr25 register
     */
    virtual void gen_write_wr25(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr25_u register
     * \param value Object representing the new value of the rh850g4 wr25_u register
     */
    virtual void gen_write_wr25_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr26 register
     * \param value Object representing the new value of the rh850g4 wr26 register
     */
    virtual void gen_write_wr26(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr26_u register
     * \param value Object representing the new value of the rh850g4 wr26_u register
     */
    virtual void gen_write_wr26_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr27 register
     * \param value Object representing the new value of the rh850g4 wr27 register
     */
    virtual void gen_write_wr27(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr27_u register
     * \param value Object representing the new value of the rh850g4 wr27_u register
     */
    virtual void gen_write_wr27_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr28 register
     * \param value Object representing the new value of the rh850g4 wr28 register
     */
    virtual void gen_write_wr28(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr28_u register
     * \param value Object representing the new value of the rh850g4 wr28_u register
     */
    virtual void gen_write_wr28_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr29 register
     * \param value Object representing the new value of the rh850g4 wr29 register
     */
    virtual void gen_write_wr29(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr29_u register
     * \param value Object representing the new value of the rh850g4 wr29_u register
     */
    virtual void gen_write_wr29_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr30 register
     * \param value Object representing the new value of the rh850g4 wr30 register
     */
    virtual void gen_write_wr30(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr30_u register
     * \param value Object representing the new value of the rh850g4 wr30_u register
     */
    virtual void gen_write_wr30_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr31 register
     * \param value Object representing the new value of the rh850g4 wr31 register
     */
    virtual void gen_write_wr31(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr31_u register
     * \param value Object representing the new value of the rh850g4 wr31_u register
     */
    virtual void gen_write_wr31_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EIPC register
     * \param value Object representing the new value of the rh850g4 EIPC register
     */
    virtual void gen_write_EIPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EIPSW register
     * \param value Object representing the new value of the rh850g4 EIPSW register
     */
    virtual void gen_write_EIPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FEPC register
     * \param value Object representing the new value of the rh850g4 FEPC register
     */
    virtual void gen_write_FEPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FEPSW register
     * \param value Object representing the new value of the rh850g4 FEPSW register
     */
    virtual void gen_write_FEPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW register
     * \param value Object representing the new value of the rh850g4 PSW register
     */
    virtual void gen_write_PSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_Z register field
     * \param value Object representing the new value of the rh850g4 PSW_Z register field
     */
    virtual void gen_write_PSW_Z(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_S register field
     * \param value Object representing the new value of the rh850g4 PSW_S register field
     */
    virtual void gen_write_PSW_S(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_OV register field
     * \param value Object representing the new value of the rh850g4 PSW_OV register field
     */
    virtual void gen_write_PSW_OV(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_CY register field
     * \param value Object representing the new value of the rh850g4 PSW_CY register field
     */
    virtual void gen_write_PSW_CY(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_SAT register field
     * \param value Object representing the new value of the rh850g4 PSW_SAT register field
     */
    virtual void gen_write_PSW_SAT(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_ID register field
     * \param value Object representing the new value of the rh850g4 PSW_ID register field
     */
    virtual void gen_write_PSW_ID(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_EP register field
     * \param value Object representing the new value of the rh850g4 PSW_EP register field
     */
    virtual void gen_write_PSW_EP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_NP register field
     * \param value Object representing the new value of the rh850g4 PSW_NP register field
     */
    virtual void gen_write_PSW_NP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_EBV register field
     * \param value Object representing the new value of the rh850g4 PSW_EBV register field
     */
    virtual void gen_write_PSW_EBV(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_CU0 register field
     * \param value Object representing the new value of the rh850g4 PSW_CU0 register field
     */
    virtual void gen_write_PSW_CU0(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_CU1 register field
     * \param value Object representing the new value of the rh850g4 PSW_CU1 register field
     */
    virtual void gen_write_PSW_CU1(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_CU2 register field
     * \param value Object representing the new value of the rh850g4 PSW_CU2 register field
     */
    virtual void gen_write_PSW_CU2(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_EIMASK register field
     * \param value Object representing the new value of the rh850g4 PSW_EIMASK register field
     */
    virtual void gen_write_PSW_EIMASK(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_UM register field
     * \param value Object representing the new value of the rh850g4 PSW_UM register field
     */
    virtual void gen_write_PSW_UM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR register
     * \param value Object representing the new value of the rh850g4 FPSR register
     */
    virtual void gen_write_FPSR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_CC register field
     * \param value Object representing the new value of the rh850g4 FPSR_CC register field
     */
    virtual void gen_write_FPSR_CC(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_FN register field
     * \param value Object representing the new value of the rh850g4 FPSR_FN register field
     */
    virtual void gen_write_FPSR_FN(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_IF register field
     * \param value Object representing the new value of the rh850g4 FPSR_IF register field
     */
    virtual void gen_write_FPSR_IF(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_PEM register field
     * \param value Object representing the new value of the rh850g4 FPSR_PEM register field
     */
    virtual void gen_write_FPSR_PEM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_RM register field
     * \param value Object representing the new value of the rh850g4 FPSR_RM register field
     */
    virtual void gen_write_FPSR_RM(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_FS register field
     * \param value Object representing the new value of the rh850g4 FPSR_FS register field
     */
    virtual void gen_write_FPSR_FS(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_XC register field
     * \param value Object representing the new value of the rh850g4 FPSR_XC register field
     */
    virtual void gen_write_FPSR_XC(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_XE register field
     * \param value Object representing the new value of the rh850g4 FPSR_XE register field
     */
    virtual void gen_write_FPSR_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_XP register field
     * \param value Object representing the new value of the rh850g4 FPSR_XP register field
     */
    virtual void gen_write_FPSR_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPEPC register
     * \param value Object representing the new value of the rh850g4 FPEPC register
     */
    virtual void gen_write_FPEPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPST register
     * \param value Object representing the new value of the rh850g4 FPST register
     */
    virtual void gen_write_FPST(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPST_XC register field
     * \param value Object representing the new value of the rh850g4 FPST_XC register field
     */
    virtual void gen_write_FPST_XC(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPST_IF register field
     * \param value Object representing the new value of the rh850g4 FPST_IF register field
     */
    virtual void gen_write_FPST_IF(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPST_XP register field
     * \param value Object representing the new value of the rh850g4 FPST_XP register field
     */
    virtual void gen_write_FPST_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPCC register
     * \param value Object representing the new value of the rh850g4 FPCC register
     */
    virtual void gen_write_FPCC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPCC_CC register field
     * \param value Object representing the new value of the rh850g4 FPCC_CC register field
     */
    virtual void gen_write_FPCC_CC(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPCFG register
     * \param value Object representing the new value of the rh850g4 FPCFG register
     */
    virtual void gen_write_FPCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPCFG_RM register field
     * \param value Object representing the new value of the rh850g4 FPCFG_RM register field
     */
    virtual void gen_write_FPCFG_RM(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPCFG_XE register field
     * \param value Object representing the new value of the rh850g4 FPCFG_XE register field
     */
    virtual void gen_write_FPCFG_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EIIC register
     * \param value Object representing the new value of the rh850g4 EIIC register
     */
    virtual void gen_write_EIIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FEIC register
     * \param value Object representing the new value of the rh850g4 FEIC register
     */
    virtual void gen_write_FEIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSWH register
     * \param value Object representing the new value of the rh850g4 PSWH register
     */
    virtual void gen_write_PSWH(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSWH_GPID register field
     * \param value Object representing the new value of the rh850g4 PSWH_GPID register field
     */
    virtual void gen_write_PSWH_GPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSWH_GM register field
     * \param value Object representing the new value of the rh850g4 PSWH_GM register field
     */
    virtual void gen_write_PSWH_GM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 CTPC register
     * \param value Object representing the new value of the rh850g4 CTPC register
     */
    virtual void gen_write_CTPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 CTPSW register
     * \param value Object representing the new value of the rh850g4 CTPSW register
     */
    virtual void gen_write_CTPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EIPSWH register
     * \param value Object representing the new value of the rh850g4 EIPSWH register
     */
    virtual void gen_write_EIPSWH(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EIPSWH_GPID register field
     * \param value Object representing the new value of the rh850g4 EIPSWH_GPID register field
     */
    virtual void gen_write_EIPSWH_GPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EIPSWH_GM register field
     * \param value Object representing the new value of the rh850g4 EIPSWH_GM register field
     */
    virtual void gen_write_EIPSWH_GM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FEPSWH register
     * \param value Object representing the new value of the rh850g4 FEPSWH register
     */
    virtual void gen_write_FEPSWH(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FEPSWH_GPID register field
     * \param value Object representing the new value of the rh850g4 FEPSWH_GPID register field
     */
    virtual void gen_write_FEPSWH_GPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FEPSWH_GM register field
     * \param value Object representing the new value of the rh850g4 FEPSWH_GM register field
     */
    virtual void gen_write_FEPSWH_GM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 CTBP register
     * \param value Object representing the new value of the rh850g4 CTBP register
     */
    virtual void gen_write_CTBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 SNZCFG register
     * \param value Object representing the new value of the rh850g4 SNZCFG register
     */
    virtual void gen_write_SNZCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EIWR register
     * \param value Object representing the new value of the rh850g4 EIWR register
     */
    virtual void gen_write_EIWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FEWR register
     * \param value Object representing the new value of the rh850g4 FEWR register
     */
    virtual void gen_write_FEWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 SPID register
     * \param value Object representing the new value of the rh850g4 SPID register
     */
    virtual void gen_write_SPID(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 SPIDLIST register
     * \param value Object representing the new value of the rh850g4 SPIDLIST register
     */
    virtual void gen_write_SPIDLIST(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBASE register
     * \param value Object representing the new value of the rh850g4 RBASE register
     */
    virtual void gen_write_RBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBASE_RBASE register field
     * \param value Object representing the new value of the rh850g4 RBASE_RBASE register field
     */
    virtual void gen_write_RBASE_RBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBASE_DV register field
     * \param value Object representing the new value of the rh850g4 RBASE_DV register field
     */
    virtual void gen_write_RBASE_DV(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBASE_RINT register field
     * \param value Object representing the new value of the rh850g4 RBASE_RINT register field
     */
    virtual void gen_write_RBASE_RINT(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EBASE register
     * \param value Object representing the new value of the rh850g4 EBASE register
     */
    virtual void gen_write_EBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EBASE_EBASE register field
     * \param value Object representing the new value of the rh850g4 EBASE_EBASE register field
     */
    virtual void gen_write_EBASE_EBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EBASE_DV register field
     * \param value Object representing the new value of the rh850g4 EBASE_DV register field
     */
    virtual void gen_write_EBASE_DV(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EBASE_RINT register field
     * \param value Object representing the new value of the rh850g4 EBASE_RINT register field
     */
    virtual void gen_write_EBASE_RINT(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 INTBP register
     * \param value Object representing the new value of the rh850g4 INTBP register
     */
    virtual void gen_write_INTBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCTL register
     * \param value Object representing the new value of the rh850g4 MCTL register
     */
    virtual void gen_write_MCTL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCTL_UIC register field
     * \param value Object representing the new value of the rh850g4 MCTL_UIC register field
     */
    virtual void gen_write_MCTL_UIC(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PID register
     * \param value Object representing the new value of the rh850g4 PID register
     */
    virtual void gen_write_PID(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 SVLOCK register
     * \param value Object representing the new value of the rh850g4 SVLOCK register
     */
    virtual void gen_write_SVLOCK(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 SCCFG register
     * \param value Object representing the new value of the rh850g4 SCCFG register
     */
    virtual void gen_write_SCCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 SCCFG_SIZE register field
     * \param value Object representing the new value of the rh850g4 SCCFG_SIZE register field
     */
    virtual void gen_write_SCCFG_SIZE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 SCBP register
     * \param value Object representing the new value of the rh850g4 SCBP register
     */
    virtual void gen_write_SCBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 HVCFG register
     * \param value Object representing the new value of the rh850g4 HVCFG register
     */
    virtual void gen_write_HVCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 HVCFG_HVE register field
     * \param value Object representing the new value of the rh850g4 HVCFG_HVE register field
     */
    virtual void gen_write_HVCFG_HVE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMCFG register
     * \param value Object representing the new value of the rh850g4 GMCFG register
     */
    virtual void gen_write_GMCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMCFG_GMP register field
     * \param value Object representing the new value of the rh850g4 GMCFG_GMP register field
     */
    virtual void gen_write_GMCFG_GMP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMCFG_HMP register field
     * \param value Object representing the new value of the rh850g4 GMCFG_HMP register field
     */
    virtual void gen_write_GMCFG_HMP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMCFG_GSYSE register field
     * \param value Object representing the new value of the rh850g4 GMCFG_GSYSE register field
     */
    virtual void gen_write_GMCFG_GSYSE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMCFG_GCU0 register field
     * \param value Object representing the new value of the rh850g4 GMCFG_GCU0 register field
     */
    virtual void gen_write_GMCFG_GCU0(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMCFG_GCU1 register field
     * \param value Object representing the new value of the rh850g4 GMCFG_GCU1 register field
     */
    virtual void gen_write_GMCFG_GCU1(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMCFG_GCU2 register field
     * \param value Object representing the new value of the rh850g4 GMCFG_GCU2 register field
     */
    virtual void gen_write_GMCFG_GCU2(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 HVSB register
     * \param value Object representing the new value of the rh850g4 HVSB register
     */
    virtual void gen_write_HVSB(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PEID register
     * \param value Object representing the new value of the rh850g4 PEID register
     */
    virtual void gen_write_PEID(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 BMID register
     * \param value Object representing the new value of the rh850g4 BMID register
     */
    virtual void gen_write_BMID(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 BMID_BMID register field
     * \param value Object representing the new value of the rh850g4 BMID_BMID register field
     */
    virtual void gen_write_BMID_BMID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MEA register
     * \param value Object representing the new value of the rh850g4 MEA register
     */
    virtual void gen_write_MEA(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MEI register
     * \param value Object representing the new value of the rh850g4 MEI register
     */
    virtual void gen_write_MEI(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBCR0 register
     * \param value Object representing the new value of the rh850g4 RBCR0 register
     */
    virtual void gen_write_RBCR0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBCR0_MD register field
     * \param value Object representing the new value of the rh850g4 RBCR0_MD register field
     */
    virtual void gen_write_RBCR0_MD(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBCR0_BE register field
     * \param value Object representing the new value of the rh850g4 RBCR0_BE register field
     */
    virtual void gen_write_RBCR0_BE(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBCR1 register
     * \param value Object representing the new value of the rh850g4 RBCR1 register
     */
    virtual void gen_write_RBCR1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBCR1_NC register field
     * \param value Object representing the new value of the rh850g4 RBCR1_NC register field
     */
    virtual void gen_write_RBCR1_NC(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBNR register
     * \param value Object representing the new value of the rh850g4 RBNR register
     */
    virtual void gen_write_RBNR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBNR_BN register field
     * \param value Object representing the new value of the rh850g4 RBNR_BN register field
     */
    virtual void gen_write_RBNR_BN(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBIP register
     * \param value Object representing the new value of the rh850g4 RBIP register
     */
    virtual void gen_write_RBIP(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBIP_RBIP register field
     * \param value Object representing the new value of the rh850g4 RBIP_RBIP register field
     */
    virtual void gen_write_RBIP_RBIP(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ISPR register
     * \param value Object representing the new value of the rh850g4 ISPR register
     */
    virtual void gen_write_ISPR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ISPR_ISP register field
     * \param value Object representing the new value of the rh850g4 ISPR_ISP register field
     */
    virtual void gen_write_ISPR_ISP(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR register
     * \param value Object representing the new value of the rh850g4 IMSR register
     */
    virtual void gen_write_IMSR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_EEIM register field
     * \param value Object representing the new value of the rh850g4 IMSR_EEIM register field
     */
    virtual void gen_write_IMSR_EEIM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_EPLM register field
     * \param value Object representing the new value of the rh850g4 IMSR_EPLM register field
     */
    virtual void gen_write_IMSR_EPLM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_EID register field
     * \param value Object representing the new value of the rh850g4 IMSR_EID register field
     */
    virtual void gen_write_IMSR_EID(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_ENP register field
     * \param value Object representing the new value of the rh850g4 IMSR_ENP register field
     */
    virtual void gen_write_IMSR_ENP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_FNP register field
     * \param value Object representing the new value of the rh850g4 IMSR_FNP register field
     */
    virtual void gen_write_IMSR_FNP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_HEEIM register field
     * \param value Object representing the new value of the rh850g4 IMSR_HEEIM register field
     */
    virtual void gen_write_IMSR_HEEIM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_HEPLM register field
     * \param value Object representing the new value of the rh850g4 IMSR_HEPLM register field
     */
    virtual void gen_write_IMSR_HEPLM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_HEID register field
     * \param value Object representing the new value of the rh850g4 IMSR_HEID register field
     */
    virtual void gen_write_IMSR_HEID(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_HENP register field
     * \param value Object representing the new value of the rh850g4 IMSR_HENP register field
     */
    virtual void gen_write_IMSR_HENP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_HFNP register field
     * \param value Object representing the new value of the rh850g4 IMSR_HFNP register field
     */
    virtual void gen_write_IMSR_HFNP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICSR register
     * \param value Object representing the new value of the rh850g4 ICSR register
     */
    virtual void gen_write_ICSR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICSR_PMEI register field
     * \param value Object representing the new value of the rh850g4 ICSR_PMEI register field
     */
    virtual void gen_write_ICSR_PMEI(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 INTCFG register
     * \param value Object representing the new value of the rh850g4 INTCFG register
     */
    virtual void gen_write_INTCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 INTCFG_ISPC register field
     * \param value Object representing the new value of the rh850g4 INTCFG_ISPC register field
     */
    virtual void gen_write_INTCFG_ISPC(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 INTCFG_EPL register field
     * \param value Object representing the new value of the rh850g4 INTCFG_EPL register field
     */
    virtual void gen_write_INTCFG_EPL(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 INTCFG_ULNR register field
     * \param value Object representing the new value of the rh850g4 INTCFG_ULNR register field
     */
    virtual void gen_write_INTCFG_ULNR(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PLMR register
     * \param value Object representing the new value of the rh850g4 PLMR register
     */
    virtual void gen_write_PLMR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PLMR_PLM register field
     * \param value Object representing the new value of the rh850g4 PLMR_PLM register field
     */
    virtual void gen_write_PLMR_PLM(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR register
     * \param value Object representing the new value of the rh850g4 FXSR register
     */
    virtual void gen_write_FXSR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_FN register field
     * \param value Object representing the new value of the rh850g4 FXSR_FN register field
     */
    virtual void gen_write_FXSR_FN(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_IF register field
     * \param value Object representing the new value of the rh850g4 FXSR_IF register field
     */
    virtual void gen_write_FXSR_IF(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_PEM register field
     * \param value Object representing the new value of the rh850g4 FXSR_PEM register field
     */
    virtual void gen_write_FXSR_PEM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_RM register field
     * \param value Object representing the new value of the rh850g4 FXSR_RM register field
     */
    virtual void gen_write_FXSR_RM(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_FS register field
     * \param value Object representing the new value of the rh850g4 FXSR_FS register field
     */
    virtual void gen_write_FXSR_FS(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_XC register field
     * \param value Object representing the new value of the rh850g4 FXSR_XC register field
     */
    virtual void gen_write_FXSR_XC(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_XE register field
     * \param value Object representing the new value of the rh850g4 FXSR_XE register field
     */
    virtual void gen_write_FXSR_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_XP register field
     * \param value Object representing the new value of the rh850g4 FXSR_XP register field
     */
    virtual void gen_write_FXSR_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXST register
     * \param value Object representing the new value of the rh850g4 FXST register
     */
    virtual void gen_write_FXST(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXST_XC register field
     * \param value Object representing the new value of the rh850g4 FXST_XC register field
     */
    virtual void gen_write_FXST_XC(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXST_IF register field
     * \param value Object representing the new value of the rh850g4 FXST_IF register field
     */
    virtual void gen_write_FXST_IF(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXST_XP register field
     * \param value Object representing the new value of the rh850g4 FXST_XP register field
     */
    virtual void gen_write_FXST_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXINFO register
     * \param value Object representing the new value of the rh850g4 FXINFO register
     */
    virtual void gen_write_FXINFO(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXINFO_NFPU register field
     * \param value Object representing the new value of the rh850g4 FXINFO_NFPU register field
     */
    virtual void gen_write_FXINFO_NFPU(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXINFO_RSIZE register field
     * \param value Object representing the new value of the rh850g4 FXINFO_RSIZE register field
     */
    virtual void gen_write_FXINFO_RSIZE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXCFG register
     * \param value Object representing the new value of the rh850g4 FXCFG register
     */
    virtual void gen_write_FXCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXCFG_RM register field
     * \param value Object representing the new value of the rh850g4 FXCFG_RM register field
     */
    virtual void gen_write_FXCFG_RM(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXCFG_XE register field
     * \param value Object representing the new value of the rh850g4 FXCFG_XE register field
     */
    virtual void gen_write_FXCFG_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXXC register
     * \param value Object representing the new value of the rh850g4 FXXC register
     */
    virtual void gen_write_FXXC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXXC_XC3 register field
     * \param value Object representing the new value of the rh850g4 FXXC_XC3 register field
     */
    virtual void gen_write_FXXC_XC3(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXXC_XC2 register field
     * \param value Object representing the new value of the rh850g4 FXXC_XC2 register field
     */
    virtual void gen_write_FXXC_XC2(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXXC_XC1 register field
     * \param value Object representing the new value of the rh850g4 FXXC_XC1 register field
     */
    virtual void gen_write_FXXC_XC1(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXXC_XC0 register field
     * \param value Object representing the new value of the rh850g4 FXXC_XC0 register field
     */
    virtual void gen_write_FXXC_XC0(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXXP register
     * \param value Object representing the new value of the rh850g4 FXXP register
     */
    virtual void gen_write_FXXP(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXXP_XP3 register field
     * \param value Object representing the new value of the rh850g4 FXXP_XP3 register field
     */
    virtual void gen_write_FXXP_XP3(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXXP_XP2 register field
     * \param value Object representing the new value of the rh850g4 FXXP_XP2 register field
     */
    virtual void gen_write_FXXP_XP2(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXXP_XP1 register field
     * \param value Object representing the new value of the rh850g4 FXXP_XP1 register field
     */
    virtual void gen_write_FXXP_XP1(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXXP_XP0 register field
     * \param value Object representing the new value of the rh850g4 FXXP_XP0 register field
     */
    virtual void gen_write_FXXP_XP0(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMEIPC register
     * \param value Object representing the new value of the rh850g4 GMEIPC register
     */
    virtual void gen_write_GMEIPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMEIPSW register
     * \param value Object representing the new value of the rh850g4 GMEIPSW register
     */
    virtual void gen_write_GMEIPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMFEPC register
     * \param value Object representing the new value of the rh850g4 GMFEPC register
     */
    virtual void gen_write_GMFEPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMFEPSW register
     * \param value Object representing the new value of the rh850g4 GMFEPSW register
     */
    virtual void gen_write_GMFEPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW register
     * \param value Object representing the new value of the rh850g4 GMPSW register
     */
    virtual void gen_write_GMPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_Z register field
     * \param value Object representing the new value of the rh850g4 GMPSW_Z register field
     */
    virtual void gen_write_GMPSW_Z(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_S register field
     * \param value Object representing the new value of the rh850g4 GMPSW_S register field
     */
    virtual void gen_write_GMPSW_S(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_OV register field
     * \param value Object representing the new value of the rh850g4 GMPSW_OV register field
     */
    virtual void gen_write_GMPSW_OV(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_CY register field
     * \param value Object representing the new value of the rh850g4 GMPSW_CY register field
     */
    virtual void gen_write_GMPSW_CY(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_SAT register field
     * \param value Object representing the new value of the rh850g4 GMPSW_SAT register field
     */
    virtual void gen_write_GMPSW_SAT(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_ID register field
     * \param value Object representing the new value of the rh850g4 GMPSW_ID register field
     */
    virtual void gen_write_GMPSW_ID(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_EP register field
     * \param value Object representing the new value of the rh850g4 GMPSW_EP register field
     */
    virtual void gen_write_GMPSW_EP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_NP register field
     * \param value Object representing the new value of the rh850g4 GMPSW_NP register field
     */
    virtual void gen_write_GMPSW_NP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_EBV register field
     * \param value Object representing the new value of the rh850g4 GMPSW_EBV register field
     */
    virtual void gen_write_GMPSW_EBV(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_CU0 register field
     * \param value Object representing the new value of the rh850g4 GMPSW_CU0 register field
     */
    virtual void gen_write_GMPSW_CU0(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_CU1 register field
     * \param value Object representing the new value of the rh850g4 GMPSW_CU1 register field
     */
    virtual void gen_write_GMPSW_CU1(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_CU2 register field
     * \param value Object representing the new value of the rh850g4 GMPSW_CU2 register field
     */
    virtual void gen_write_GMPSW_CU2(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_EIMASK register field
     * \param value Object representing the new value of the rh850g4 GMPSW_EIMASK register field
     */
    virtual void gen_write_GMPSW_EIMASK(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_UM register field
     * \param value Object representing the new value of the rh850g4 GMPSW_UM register field
     */
    virtual void gen_write_GMPSW_UM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMMEA register
     * \param value Object representing the new value of the rh850g4 GMMEA register
     */
    virtual void gen_write_GMMEA(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMMEI register
     * \param value Object representing the new value of the rh850g4 GMMEI register
     */
    virtual void gen_write_GMMEI(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMEIIC register
     * \param value Object representing the new value of the rh850g4 GMEIIC register
     */
    virtual void gen_write_GMEIIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMFEIC register
     * \param value Object representing the new value of the rh850g4 GMFEIC register
     */
    virtual void gen_write_GMFEIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMSPID register
     * \param value Object representing the new value of the rh850g4 GMSPID register
     */
    virtual void gen_write_GMSPID(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMSPIDLIST register
     * \param value Object representing the new value of the rh850g4 GMSPIDLIST register
     */
    virtual void gen_write_GMSPIDLIST(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMEBASE register
     * \param value Object representing the new value of the rh850g4 GMEBASE register
     */
    virtual void gen_write_GMEBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMEBASE_EBASE register field
     * \param value Object representing the new value of the rh850g4 GMEBASE_EBASE register field
     */
    virtual void gen_write_GMEBASE_EBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMEBASE_DV register field
     * \param value Object representing the new value of the rh850g4 GMEBASE_DV register field
     */
    virtual void gen_write_GMEBASE_DV(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMEBASE_RINT register field
     * \param value Object representing the new value of the rh850g4 GMEBASE_RINT register field
     */
    virtual void gen_write_GMEBASE_RINT(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMINTBP register
     * \param value Object representing the new value of the rh850g4 GMINTBP register
     */
    virtual void gen_write_GMINTBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMINTCFG register
     * \param value Object representing the new value of the rh850g4 GMINTCFG register
     */
    virtual void gen_write_GMINTCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMINTCFG_ISPC register field
     * \param value Object representing the new value of the rh850g4 GMINTCFG_ISPC register field
     */
    virtual void gen_write_GMINTCFG_ISPC(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMINTCFG_EPL register field
     * \param value Object representing the new value of the rh850g4 GMINTCFG_EPL register field
     */
    virtual void gen_write_GMINTCFG_EPL(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMINTCFG_ULNR register field
     * \param value Object representing the new value of the rh850g4 GMINTCFG_ULNR register field
     */
    virtual void gen_write_GMINTCFG_ULNR(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPLMR register
     * \param value Object representing the new value of the rh850g4 GMPLMR register
     */
    virtual void gen_write_GMPLMR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPLMR_PLM register field
     * \param value Object representing the new value of the rh850g4 GMPLMR_PLM register field
     */
    virtual void gen_write_GMPLMR_PLM(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMSVLOCK register
     * \param value Object representing the new value of the rh850g4 GMSVLOCK register
     */
    virtual void gen_write_GMSVLOCK(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMMPM register
     * \param value Object representing the new value of the rh850g4 GMMPM register
     */
    virtual void gen_write_GMMPM(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMEIWR register
     * \param value Object representing the new value of the rh850g4 GMEIWR register
     */
    virtual void gen_write_GMEIWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMFEWR register
     * \param value Object representing the new value of the rh850g4 GMFEWR register
     */
    virtual void gen_write_GMFEWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPEID register
     * \param value Object representing the new value of the rh850g4 GMPEID register
     */
    virtual void gen_write_GMPEID(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPM register
     * \param value Object representing the new value of the rh850g4 MPM register
     */
    virtual void gen_write_MPM(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPCFG register
     * \param value Object representing the new value of the rh850g4 MPCFG register
     */
    virtual void gen_write_MPCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPCFG_NMPUE register field
     * \param value Object representing the new value of the rh850g4 MPCFG_NMPUE register field
     */
    virtual void gen_write_MPCFG_NMPUE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPCFG_HBE register field
     * \param value Object representing the new value of the rh850g4 MPCFG_HBE register field
     */
    virtual void gen_write_MPCFG_HBE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPCFG_ARCH register field
     * \param value Object representing the new value of the rh850g4 MPCFG_ARCH register field
     */
    virtual void gen_write_MPCFG_ARCH(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPCFG_NBK register field
     * \param value Object representing the new value of the rh850g4 MPCFG_NBK register field
     */
    virtual void gen_write_MPCFG_NBK(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPCFG_DMDP register field
     * \param value Object representing the new value of the rh850g4 MPCFG_DMDP register field
     */
    virtual void gen_write_MPCFG_DMDP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCA register
     * \param value Object representing the new value of the rh850g4 MCA register
     */
    virtual void gen_write_MCA(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCS register
     * \param value Object representing the new value of the rh850g4 MCS register
     */
    virtual void gen_write_MCS(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCC register
     * \param value Object representing the new value of the rh850g4 MCC register
     */
    virtual void gen_write_MCC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR register
     * \param value Object representing the new value of the rh850g4 MCR register
     */
    virtual void gen_write_MCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_HSXE register field
     * \param value Object representing the new value of the rh850g4 MCR_HSXE register field
     */
    virtual void gen_write_MCR_HSXE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_HSWE register field
     * \param value Object representing the new value of the rh850g4 MCR_HSWE register field
     */
    virtual void gen_write_MCR_HSWE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_HSRE register field
     * \param value Object representing the new value of the rh850g4 MCR_HSRE register field
     */
    virtual void gen_write_MCR_HSRE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_HUXE register field
     * \param value Object representing the new value of the rh850g4 MCR_HUXE register field
     */
    virtual void gen_write_MCR_HUXE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_HUWE register field
     * \param value Object representing the new value of the rh850g4 MCR_HUWE register field
     */
    virtual void gen_write_MCR_HUWE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_HURE register field
     * \param value Object representing the new value of the rh850g4 MCR_HURE register field
     */
    virtual void gen_write_MCR_HURE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_GSXE register field
     * \param value Object representing the new value of the rh850g4 MCR_GSXE register field
     */
    virtual void gen_write_MCR_GSXE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_GSWE register field
     * \param value Object representing the new value of the rh850g4 MCR_GSWE register field
     */
    virtual void gen_write_MCR_GSWE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_GSRE register field
     * \param value Object representing the new value of the rh850g4 MCR_GSRE register field
     */
    virtual void gen_write_MCR_GSRE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_GUXE register field
     * \param value Object representing the new value of the rh850g4 MCR_GUXE register field
     */
    virtual void gen_write_MCR_GUXE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_GUWE register field
     * \param value Object representing the new value of the rh850g4 MCR_GUWE register field
     */
    virtual void gen_write_MCR_GUWE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_GURE register field
     * \param value Object representing the new value of the rh850g4 MCR_GURE register field
     */
    virtual void gen_write_MCR_GURE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_OV register field
     * \param value Object representing the new value of the rh850g4 MCR_OV register field
     */
    virtual void gen_write_MCR_OV(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_SXE register field
     * \param value Object representing the new value of the rh850g4 MCR_SXE register field
     */
    virtual void gen_write_MCR_SXE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_SWE register field
     * \param value Object representing the new value of the rh850g4 MCR_SWE register field
     */
    virtual void gen_write_MCR_SWE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_SRE register field
     * \param value Object representing the new value of the rh850g4 MCR_SRE register field
     */
    virtual void gen_write_MCR_SRE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_UXE register field
     * \param value Object representing the new value of the rh850g4 MCR_UXE register field
     */
    virtual void gen_write_MCR_UXE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_UWE register field
     * \param value Object representing the new value of the rh850g4 MCR_UWE register field
     */
    virtual void gen_write_MCR_UWE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_URE register field
     * \param value Object representing the new value of the rh850g4 MCR_URE register field
     */
    virtual void gen_write_MCR_URE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCI register
     * \param value Object representing the new value of the rh850g4 MCI register
     */
    virtual void gen_write_MCI(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCI_SPID register field
     * \param value Object representing the new value of the rh850g4 MCI_SPID register field
     */
    virtual void gen_write_MCI_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPIDX register
     * \param value Object representing the new value of the rh850g4 MPIDX register
     */
    virtual void gen_write_MPIDX(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPIDX_IDX register field
     * \param value Object representing the new value of the rh850g4 MPIDX_IDX register field
     */
    virtual void gen_write_MPIDX_IDX(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPBK register
     * \param value Object representing the new value of the rh850g4 MPBK register
     */
    virtual void gen_write_MPBK(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPBK_BK register field
     * \param value Object representing the new value of the rh850g4 MPBK_BK register field
     */
    virtual void gen_write_MPBK_BK(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPLA register
     * \param value Object representing the new value of the rh850g4 MPLA register
     */
    virtual void gen_write_MPLA(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPUA register
     * \param value Object representing the new value of the rh850g4 MPUA register
     */
    virtual void gen_write_MPUA(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT register
     * \param value Object representing the new value of the rh850g4 MPAT register
     */
    virtual void gen_write_MPAT(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID7 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID7 register field
     */
    virtual void gen_write_MPAT_WMPID7(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID6 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID6 register field
     */
    virtual void gen_write_MPAT_WMPID6(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID5 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID5 register field
     */
    virtual void gen_write_MPAT_WMPID5(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID4 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID4 register field
     */
    virtual void gen_write_MPAT_WMPID4(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID3 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID3 register field
     */
    virtual void gen_write_MPAT_WMPID3(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID2 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID2 register field
     */
    virtual void gen_write_MPAT_WMPID2(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID1 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID1 register field
     */
    virtual void gen_write_MPAT_WMPID1(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID0 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID0 register field
     */
    virtual void gen_write_MPAT_WMPID0(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID7 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID7 register field
     */
    virtual void gen_write_MPAT_RMPID7(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID6 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID6 register field
     */
    virtual void gen_write_MPAT_RMPID6(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID5 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID5 register field
     */
    virtual void gen_write_MPAT_RMPID5(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID4 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID4 register field
     */
    virtual void gen_write_MPAT_RMPID4(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID3 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID3 register field
     */
    virtual void gen_write_MPAT_RMPID3(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID2 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID2 register field
     */
    virtual void gen_write_MPAT_RMPID2(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID1 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID1 register field
     */
    virtual void gen_write_MPAT_RMPID1(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID0 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID0 register field
     */
    virtual void gen_write_MPAT_RMPID0(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WG register field
     * \param value Object representing the new value of the rh850g4 MPAT_WG register field
     */
    virtual void gen_write_MPAT_WG(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RG register field
     * \param value Object representing the new value of the rh850g4 MPAT_RG register field
     */
    virtual void gen_write_MPAT_RG(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_E register field
     * \param value Object representing the new value of the rh850g4 MPAT_E register field
     */
    virtual void gen_write_MPAT_E(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_SX register field
     * \param value Object representing the new value of the rh850g4 MPAT_SX register field
     */
    virtual void gen_write_MPAT_SX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_SW register field
     * \param value Object representing the new value of the rh850g4 MPAT_SW register field
     */
    virtual void gen_write_MPAT_SW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_SR register field
     * \param value Object representing the new value of the rh850g4 MPAT_SR register field
     */
    virtual void gen_write_MPAT_SR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_UX register field
     * \param value Object representing the new value of the rh850g4 MPAT_UX register field
     */
    virtual void gen_write_MPAT_UX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_UW register field
     * \param value Object representing the new value of the rh850g4 MPAT_UW register field
     */
    virtual void gen_write_MPAT_UW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_UR register field
     * \param value Object representing the new value of the rh850g4 MPAT_UR register field
     */
    virtual void gen_write_MPAT_UR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID0 register
     * \param value Object representing the new value of the rh850g4 MPID0 register
     */
    virtual void gen_write_MPID0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID0_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID0_SPID register field
     */
    virtual void gen_write_MPID0_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID1 register
     * \param value Object representing the new value of the rh850g4 MPID1 register
     */
    virtual void gen_write_MPID1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID1_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID1_SPID register field
     */
    virtual void gen_write_MPID1_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID2 register
     * \param value Object representing the new value of the rh850g4 MPID2 register
     */
    virtual void gen_write_MPID2(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID2_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID2_SPID register field
     */
    virtual void gen_write_MPID2_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID3 register
     * \param value Object representing the new value of the rh850g4 MPID3 register
     */
    virtual void gen_write_MPID3(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID3_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID3_SPID register field
     */
    virtual void gen_write_MPID3_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID4 register
     * \param value Object representing the new value of the rh850g4 MPID4 register
     */
    virtual void gen_write_MPID4(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID4_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID4_SPID register field
     */
    virtual void gen_write_MPID4_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID5 register
     * \param value Object representing the new value of the rh850g4 MPID5 register
     */
    virtual void gen_write_MPID5(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID5_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID5_SPID register field
     */
    virtual void gen_write_MPID5_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID6 register
     * \param value Object representing the new value of the rh850g4 MPID6 register
     */
    virtual void gen_write_MPID6(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID6_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID6_SPID register field
     */
    virtual void gen_write_MPID6_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID7 register
     * \param value Object representing the new value of the rh850g4 MPID7 register
     */
    virtual void gen_write_MPID7(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID7_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID7_SPID register field
     */
    virtual void gen_write_MPID7_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGL register
     * \param value Object representing the new value of the rh850g4 ICTAGL register
     */
    virtual void gen_write_ICTAGL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGL_LPN register field
     * \param value Object representing the new value of the rh850g4 ICTAGL_LPN register field
     */
    virtual void gen_write_ICTAGL_LPN(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGL_LRU register field
     * \param value Object representing the new value of the rh850g4 ICTAGL_LRU register field
     */
    virtual void gen_write_ICTAGL_LRU(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGL_L register field
     * \param value Object representing the new value of the rh850g4 ICTAGL_L register field
     */
    virtual void gen_write_ICTAGL_L(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGL_V register field
     * \param value Object representing the new value of the rh850g4 ICTAGL_V register field
     */
    virtual void gen_write_ICTAGL_V(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGH register
     * \param value Object representing the new value of the rh850g4 ICTAGH register
     */
    virtual void gen_write_ICTAGH(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGH_WD register field
     * \param value Object representing the new value of the rh850g4 ICTAGH_WD register field
     */
    virtual void gen_write_ICTAGH_WD(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGH_PD register field
     * \param value Object representing the new value of the rh850g4 ICTAGH_PD register field
     */
    virtual void gen_write_ICTAGH_PD(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGH_WT register field
     * \param value Object representing the new value of the rh850g4 ICTAGH_WT register field
     */
    virtual void gen_write_ICTAGH_WT(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGH_PT register field
     * \param value Object representing the new value of the rh850g4 ICTAGH_PT register field
     */
    virtual void gen_write_ICTAGH_PT(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGH_DATAECC register field
     * \param value Object representing the new value of the rh850g4 ICTAGH_DATAECC register field
     */
    virtual void gen_write_ICTAGH_DATAECC(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGH_TAGECC register field
     * \param value Object representing the new value of the rh850g4 ICTAGH_TAGECC register field
     */
    virtual void gen_write_ICTAGH_TAGECC(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICDATL register
     * \param value Object representing the new value of the rh850g4 ICDATL register
     */
    virtual void gen_write_ICDATL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICDATH register
     * \param value Object representing the new value of the rh850g4 ICDATH register
     */
    virtual void gen_write_ICDATH(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICCTRL register
     * \param value Object representing the new value of the rh850g4 ICCTRL register
     */
    virtual void gen_write_ICCTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICCTRL_PBS register field
     * \param value Object representing the new value of the rh850g4 ICCTRL_PBS register field
     */
    virtual void gen_write_ICCTRL_PBS(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICCTRL_ICHCLR register field
     * \param value Object representing the new value of the rh850g4 ICCTRL_ICHCLR register field
     */
    virtual void gen_write_ICCTRL_ICHCLR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICCTRL_ICHEIV register field
     * \param value Object representing the new value of the rh850g4 ICCTRL_ICHEIV register field
     */
    virtual void gen_write_ICCTRL_ICHEIV(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICCTRL_ICHEMK register field
     * \param value Object representing the new value of the rh850g4 ICCTRL_ICHEMK register field
     */
    virtual void gen_write_ICCTRL_ICHEMK(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICCTRL_ICHEN register field
     * \param value Object representing the new value of the rh850g4 ICCTRL_ICHEN register field
     */
    virtual void gen_write_ICCTRL_ICHEN(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICCFG register
     * \param value Object representing the new value of the rh850g4 ICCFG register
     */
    virtual void gen_write_ICCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICCFG_ICHSIZE register field
     * \param value Object representing the new value of the rh850g4 ICCFG_ICHSIZE register field
     */
    virtual void gen_write_ICCFG_ICHSIZE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICCFG_ICHLINE register field
     * \param value Object representing the new value of the rh850g4 ICCFG_ICHLINE register field
     */
    virtual void gen_write_ICCFG_ICHLINE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICCFG_ICHWAY register field
     * \param value Object representing the new value of the rh850g4 ICCFG_ICHWAY register field
     */
    virtual void gen_write_ICCFG_ICHWAY(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR register
     * \param value Object representing the new value of the rh850g4 ICERR register
     */
    virtual void gen_write_ICERR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_CISTW register field
     * \param value Object representing the new value of the rh850g4 ICERR_CISTW register field
     */
    virtual void gen_write_ICERR_CISTW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ESAFE register field
     * \param value Object representing the new value of the rh850g4 ICERR_ESAFE register field
     */
    virtual void gen_write_ICERR_ESAFE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ESMH register field
     * \param value Object representing the new value of the rh850g4 ICERR_ESMH register field
     */
    virtual void gen_write_ICERR_ESMH(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ESPBSE register field
     * \param value Object representing the new value of the rh850g4 ICERR_ESPBSE register field
     */
    virtual void gen_write_ICERR_ESPBSE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ESTE1 register field
     * \param value Object representing the new value of the rh850g4 ICERR_ESTE1 register field
     */
    virtual void gen_write_ICERR_ESTE1(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ESTE2 register field
     * \param value Object representing the new value of the rh850g4 ICERR_ESTE2 register field
     */
    virtual void gen_write_ICERR_ESTE2(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ESDC register field
     * \param value Object representing the new value of the rh850g4 ICERR_ESDC register field
     */
    virtual void gen_write_ICERR_ESDC(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ESDE register field
     * \param value Object representing the new value of the rh850g4 ICERR_ESDE register field
     */
    virtual void gen_write_ICERR_ESDE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ERMAFE register field
     * \param value Object representing the new value of the rh850g4 ICERR_ERMAFE register field
     */
    virtual void gen_write_ICERR_ERMAFE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ERMMH register field
     * \param value Object representing the new value of the rh850g4 ICERR_ERMMH register field
     */
    virtual void gen_write_ICERR_ERMMH(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ERMPBSE register field
     * \param value Object representing the new value of the rh850g4 ICERR_ERMPBSE register field
     */
    virtual void gen_write_ICERR_ERMPBSE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ERMTE1 register field
     * \param value Object representing the new value of the rh850g4 ICERR_ERMTE1 register field
     */
    virtual void gen_write_ICERR_ERMTE1(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ERMTE2 register field
     * \param value Object representing the new value of the rh850g4 ICERR_ERMTE2 register field
     */
    virtual void gen_write_ICERR_ERMTE2(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ERMDC register field
     * \param value Object representing the new value of the rh850g4 ICERR_ERMDC register field
     */
    virtual void gen_write_ICERR_ERMDC(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ERMDE register field
     * \param value Object representing the new value of the rh850g4 ICERR_ERMDE register field
     */
    virtual void gen_write_ICERR_ERMDE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ICHEWY register field
     * \param value Object representing the new value of the rh850g4 ICERR_ICHEWY register field
     */
    virtual void gen_write_ICERR_ICHEWY(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ICHEIX register field
     * \param value Object representing the new value of the rh850g4 ICERR_ICHEIX register field
     */
    virtual void gen_write_ICERR_ICHEIX(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ICHERQ register field
     * \param value Object representing the new value of the rh850g4 ICERR_ICHERQ register field
     */
    virtual void gen_write_ICERR_ICHERQ(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ICHED register field
     * \param value Object representing the new value of the rh850g4 ICERR_ICHED register field
     */
    virtual void gen_write_ICERR_ICHED(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ICHET register field
     * \param value Object representing the new value of the rh850g4 ICERR_ICHET register field
     */
    virtual void gen_write_ICERR_ICHET(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ICHERR register field
     * \param value Object representing the new value of the rh850g4 ICERR_ICHERR register field
     */
    virtual void gen_write_ICERR_ICHERR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 TSCOUNTL register
     * \param value Object representing the new value of the rh850g4 TSCOUNTL register
     */
    virtual void gen_write_TSCOUNTL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 TSCOUNTH register
     * \param value Object representing the new value of the rh850g4 TSCOUNTH register
     */
    virtual void gen_write_TSCOUNTH(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 TSCTRL register
     * \param value Object representing the new value of the rh850g4 TSCTRL register
     */
    virtual void gen_write_TSCTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMUMCTRL register
     * \param value Object representing the new value of the rh850g4 PMUMCTRL register
     */
    virtual void gen_write_PMUMCTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMGMCTRL register
     * \param value Object representing the new value of the rh850g4 PMGMCTRL register
     */
    virtual void gen_write_PMGMCTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT0 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT0 register
     */
    virtual void gen_write_PMCOUNT0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL0 register
     * \param value Object representing the new value of the rh850g4 PMCTRL0 register
     */
    virtual void gen_write_PMCTRL0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT1 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT1 register
     */
    virtual void gen_write_PMCOUNT1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL1 register
     * \param value Object representing the new value of the rh850g4 PMCTRL1 register
     */
    virtual void gen_write_PMCTRL1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT2 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT2 register
     */
    virtual void gen_write_PMCOUNT2(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL2 register
     * \param value Object representing the new value of the rh850g4 PMCTRL2 register
     */
    virtual void gen_write_PMCTRL2(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT3 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT3 register
     */
    virtual void gen_write_PMCOUNT3(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL3 register
     * \param value Object representing the new value of the rh850g4 PMCTRL3 register
     */
    virtual void gen_write_PMCTRL3(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL4 register
     * \param value Object representing the new value of the rh850g4 PMCTRL4 register
     */
    virtual void gen_write_PMCTRL4(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL5 register
     * \param value Object representing the new value of the rh850g4 PMCTRL5 register
     */
    virtual void gen_write_PMCTRL5(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL6 register
     * \param value Object representing the new value of the rh850g4 PMCTRL6 register
     */
    virtual void gen_write_PMCTRL6(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL7 register
     * \param value Object representing the new value of the rh850g4 PMCTRL7 register
     */
    virtual void gen_write_PMCTRL7(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT4 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT4 register
     */
    virtual void gen_write_PMCOUNT4(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT5 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT5 register
     */
    virtual void gen_write_PMCOUNT5(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT6 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT6 register
     */
    virtual void gen_write_PMCOUNT6(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT7 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT7 register
     */
    virtual void gen_write_PMCOUNT7(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND0 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND0 register
     */
    virtual void gen_write_PMSUBCND0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND1 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND1 register
     */
    virtual void gen_write_PMSUBCND1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND2 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND2 register
     */
    virtual void gen_write_PMSUBCND2(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND3 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND3 register
     */
    virtual void gen_write_PMSUBCND3(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND4 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND4 register
     */
    virtual void gen_write_PMSUBCND4(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND5 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND5 register
     */
    virtual void gen_write_PMSUBCND5(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND6 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND6 register
     */
    virtual void gen_write_PMSUBCND6(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND7 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND7 register
     */
    virtual void gen_write_PMSUBCND7(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DBGEN register
     * \param value Object representing the new value of the rh850g4 DBGEN register
     */
    virtual void gen_write_DBGEN(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DBPSWH register
     * \param value Object representing the new value of the rh850g4 DBPSWH register
     */
    virtual void gen_write_DBPSWH(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DBIC register
     * \param value Object representing the new value of the rh850g4 DBIC register
     */
    virtual void gen_write_DBIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DBPC register
     * \param value Object representing the new value of the rh850g4 DBPC register
     */
    virtual void gen_write_DBPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DBPSW register
     * \param value Object representing the new value of the rh850g4 DBPSW register
     */
    virtual void gen_write_DBPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DIR0 register
     * \param value Object representing the new value of the rh850g4 DIR0 register
     */
    virtual void gen_write_DIR0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DIR1 register
     * \param value Object representing the new value of the rh850g4 DIR1 register
     */
    virtual void gen_write_DIR1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 BPC register
     * \param value Object representing the new value of the rh850g4 BPC register
     */
    virtual void gen_write_BPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 BPAV register
     * \param value Object representing the new value of the rh850g4 BPAV register
     */
    virtual void gen_write_BPAV(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 BPAM register
     * \param value Object representing the new value of the rh850g4 BPAM register
     */
    virtual void gen_write_BPAM(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DBCFG register
     * \param value Object representing the new value of the rh850g4 DBCFG register
     */
    virtual void gen_write_DBCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DBWR register
     * \param value Object representing the new value of the rh850g4 DBWR register
     */
    virtual void gen_write_DBWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 LSTEST0 register
     * \param value Object representing the new value of the rh850g4 LSTEST0 register
     */
    virtual void gen_write_LSTEST0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 LSTEST1 register
     * \param value Object representing the new value of the rh850g4 LSTEST1 register
     */
    virtual void gen_write_LSTEST1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 LSCFG register
     * \param value Object representing the new value of the rh850g4 LSCFG register
     */
    virtual void gen_write_LSCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICBKEY register
     * \param value Object representing the new value of the rh850g4 ICBKEY register
     */
    virtual void gen_write_ICBKEY(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IFCR register
     * \param value Object representing the new value of the rh850g4 IFCR register
     */
    virtual void gen_write_IFCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IFCR1 register
     * \param value Object representing the new value of the rh850g4 IFCR1 register
     */
    virtual void gen_write_IFCR1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 BRPCTRL0 register
     * \param value Object representing the new value of the rh850g4 BRPCTRL0 register
     */
    virtual void gen_write_BRPCTRL0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 BRPCTRL1 register
     * \param value Object representing the new value of the rh850g4 BRPCTRL1 register
     */
    virtual void gen_write_BRPCTRL1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 BRPCFG register
     * \param value Object representing the new value of the rh850g4 BRPCFG register
     */
    virtual void gen_write_BRPCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 BRPACTRL register
     * \param value Object representing the new value of the rh850g4 BRPACTRL register
     */
    virtual void gen_write_BRPACTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 BRPDATA register
     * \param value Object representing the new value of the rh850g4 BRPDATA register
     */
    virtual void gen_write_BRPDATA(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DCBKEY register
     * \param value Object representing the new value of the rh850g4 DCBKEY register
     */
    virtual void gen_write_DCBKEY(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 LSUCR register
     * \param value Object representing the new value of the rh850g4 LSUCR register
     */
    virtual void gen_write_LSUCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 LSULNK0 register
     * \param value Object representing the new value of the rh850g4 LSULNK0 register
     */
    virtual void gen_write_LSULNK0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 LSULNK1 register
     * \param value Object representing the new value of the rh850g4 LSULNK1 register
     */
    virtual void gen_write_LSULNK1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 L1RLCR register
     * \param value Object representing the new value of the rh850g4 L1RLCR register
     */
    virtual void gen_write_L1RLCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 L1RLNK0 register
     * \param value Object representing the new value of the rh850g4 L1RLNK0 register
     */
    virtual void gen_write_L1RLNK0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 L1RLNK1 register
     * \param value Object representing the new value of the rh850g4 L1RLNK1 register
     */
    virtual void gen_write_L1RLNK1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 L1RCFG register
     * \param value Object representing the new value of the rh850g4 L1RCFG register
     */
    virtual void gen_write_L1RCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DECFG register
     * \param value Object representing the new value of the rh850g4 DECFG register
     */
    virtual void gen_write_DECFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DECTRL register
     * \param value Object representing the new value of the rh850g4 DECTRL register
     */
    virtual void gen_write_DECTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DEVDS register
     * \param value Object representing the new value of the rh850g4 DEVDS register
     */
    virtual void gen_write_DEVDS(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RDBCR register
     * \param value Object representing the new value of the rh850g4 RDBCR register
     */
    virtual void gen_write_RDBCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RDBACR register
     * \param value Object representing the new value of the rh850g4 RDBACR register
     */
    virtual void gen_write_RDBACR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RDBATAG register
     * \param value Object representing the new value of the rh850g4 RDBATAG register
     */
    virtual void gen_write_RDBATAG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RDBADAT0 register
     * \param value Object representing the new value of the rh850g4 RDBADAT0 register
     */
    virtual void gen_write_RDBADAT0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RDBADAT1 register
     * \param value Object representing the new value of the rh850g4 RDBADAT1 register
     */
    virtual void gen_write_RDBADAT1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RDBADAT2 register
     * \param value Object representing the new value of the rh850g4 RDBADAT2 register
     */
    virtual void gen_write_RDBADAT2(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RDBADAT3 register
     * \param value Object representing the new value of the rh850g4 RDBADAT3 register
     */
    virtual void gen_write_RDBADAT3(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RDBSTAT register
     * \param value Object representing the new value of the rh850g4 RDBSTAT register
     */
    virtual void gen_write_RDBSTAT(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 hv_func_support register
     * \param value Object representing the new value of the rh850g4 hv_func_support register
     */
    virtual void gen_write_hv_func_support(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 swd0pcad_exec_count register
     * \param value Object representing the new value of the rh850g4 swd0pcad_exec_count register
     */
    virtual void gen_write_swd0pcad_exec_count(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 swd0pcad_val register
     * \param value Object representing the new value of the rh850g4 swd0pcad_val register
     */
    virtual void gen_write_swd0pcad_val(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 mem_acc_type register
     * \param value Object representing the new value of the rh850g4 mem_acc_type register
     */
    virtual void gen_write_mem_acc_type(fastiss::i32_t *value);

    // generative register group write operations
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 GR register group
     * \param index Index value of the required register from the rh850g4 GR register group
     * \param value Object representing the new value of the specified rh850g4 GR group register
     */
    virtual void gen_write_GR(rh850g4::GR_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 WR register group
     * \param index Index value of the required register from the rh850g4 WR register group
     * \param value Object representing the new value of the specified rh850g4 WR group register
     */
    virtual void gen_write_WR(rh850g4::WR_index_t index, fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_0 group register
     */
    virtual void gen_write_SR_SELID_0(rh850g4::SR_SELID_0_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_1 group register
     */
    virtual void gen_write_SR_SELID_1(rh850g4::SR_SELID_1_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_2 group register
     */
    virtual void gen_write_SR_SELID_2(rh850g4::SR_SELID_2_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_3 group register
     */
    virtual void gen_write_SR_SELID_3(rh850g4::SR_SELID_3_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_4 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_4 group register
     */
    virtual void gen_write_SR_SELID_4(rh850g4::SR_SELID_4_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_5 group register
     */
    virtual void gen_write_SR_SELID_5(rh850g4::SR_SELID_5_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_9 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_9 group register
     */
    virtual void gen_write_SR_SELID_9(rh850g4::SR_SELID_9_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_10 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_10 group register
     */
    virtual void gen_write_SR_SELID_10(rh850g4::SR_SELID_10_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_11 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_11 group register
     */
    virtual void gen_write_SR_SELID_11(rh850g4::SR_SELID_11_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_12 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_12 group register
     */
    virtual void gen_write_SR_SELID_12(rh850g4::SR_SELID_12_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_13 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_13 group register
     */
    virtual void gen_write_SR_SELID_13(rh850g4::SR_SELID_13_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_14 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_14 group register
     */
    virtual void gen_write_SR_SELID_14(rh850g4::SR_SELID_14_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_15 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_15 group register
     */
    virtual void gen_write_SR_SELID_15(rh850g4::SR_SELID_15_index_t index, fastiss::i32_t *value);

    // generative register and register field increment operations
    /**
     * \brief Generates code to increment the value of the rh850g4 r0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r0 register
     */
    virtual void gen_increment_r0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r1 register
     */
    virtual void gen_increment_r1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r2 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r2 register
     */
    virtual void gen_increment_r2(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r3 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r3 register
     */
    virtual void gen_increment_r3(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r4 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r4 register
     */
    virtual void gen_increment_r4(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r5 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r5 register
     */
    virtual void gen_increment_r5(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r6 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r6 register
     */
    virtual void gen_increment_r6(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r7 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r7 register
     */
    virtual void gen_increment_r7(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r8 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r8 register
     */
    virtual void gen_increment_r8(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r9 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r9 register
     */
    virtual void gen_increment_r9(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r10 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r10 register
     */
    virtual void gen_increment_r10(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r11 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r11 register
     */
    virtual void gen_increment_r11(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r12 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r12 register
     */
    virtual void gen_increment_r12(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r13 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r13 register
     */
    virtual void gen_increment_r13(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r14 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r14 register
     */
    virtual void gen_increment_r14(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r15 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r15 register
     */
    virtual void gen_increment_r15(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r16 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r16 register
     */
    virtual void gen_increment_r16(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r17 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r17 register
     */
    virtual void gen_increment_r17(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r18 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r18 register
     */
    virtual void gen_increment_r18(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r19 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r19 register
     */
    virtual void gen_increment_r19(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r20 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r20 register
     */
    virtual void gen_increment_r20(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r21 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r21 register
     */
    virtual void gen_increment_r21(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r22 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r22 register
     */
    virtual void gen_increment_r22(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r23 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r23 register
     */
    virtual void gen_increment_r23(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r24 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r24 register
     */
    virtual void gen_increment_r24(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r25 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r25 register
     */
    virtual void gen_increment_r25(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r26 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r26 register
     */
    virtual void gen_increment_r26(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r27 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r27 register
     */
    virtual void gen_increment_r27(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r28 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r28 register
     */
    virtual void gen_increment_r28(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r29 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r29 register
     */
    virtual void gen_increment_r29(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r30 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r30 register
     */
    virtual void gen_increment_r30(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r31 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r31 register
     */
    virtual void gen_increment_r31(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PC register
     */
    virtual void gen_increment_PC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr0 register
     */
    virtual void gen_increment_wr0(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr0_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr0_u register
     */
    virtual void gen_increment_wr0_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr1 register
     */
    virtual void gen_increment_wr1(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr1_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr1_u register
     */
    virtual void gen_increment_wr1_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr2 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr2 register
     */
    virtual void gen_increment_wr2(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr2_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr2_u register
     */
    virtual void gen_increment_wr2_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr3 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr3 register
     */
    virtual void gen_increment_wr3(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr3_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr3_u register
     */
    virtual void gen_increment_wr3_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr4 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr4 register
     */
    virtual void gen_increment_wr4(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr4_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr4_u register
     */
    virtual void gen_increment_wr4_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr5 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr5 register
     */
    virtual void gen_increment_wr5(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr5_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr5_u register
     */
    virtual void gen_increment_wr5_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr6 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr6 register
     */
    virtual void gen_increment_wr6(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr6_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr6_u register
     */
    virtual void gen_increment_wr6_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr7 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr7 register
     */
    virtual void gen_increment_wr7(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr7_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr7_u register
     */
    virtual void gen_increment_wr7_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr8 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr8 register
     */
    virtual void gen_increment_wr8(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr8_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr8_u register
     */
    virtual void gen_increment_wr8_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr9 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr9 register
     */
    virtual void gen_increment_wr9(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr9_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr9_u register
     */
    virtual void gen_increment_wr9_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr10 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr10 register
     */
    virtual void gen_increment_wr10(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr10_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr10_u register
     */
    virtual void gen_increment_wr10_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr11 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr11 register
     */
    virtual void gen_increment_wr11(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr11_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr11_u register
     */
    virtual void gen_increment_wr11_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr12 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr12 register
     */
    virtual void gen_increment_wr12(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr12_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr12_u register
     */
    virtual void gen_increment_wr12_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr13 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr13 register
     */
    virtual void gen_increment_wr13(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr13_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr13_u register
     */
    virtual void gen_increment_wr13_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr14 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr14 register
     */
    virtual void gen_increment_wr14(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr14_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr14_u register
     */
    virtual void gen_increment_wr14_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr15 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr15 register
     */
    virtual void gen_increment_wr15(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr15_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr15_u register
     */
    virtual void gen_increment_wr15_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr16 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr16 register
     */
    virtual void gen_increment_wr16(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr16_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr16_u register
     */
    virtual void gen_increment_wr16_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr17 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr17 register
     */
    virtual void gen_increment_wr17(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr17_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr17_u register
     */
    virtual void gen_increment_wr17_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr18 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr18 register
     */
    virtual void gen_increment_wr18(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr18_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr18_u register
     */
    virtual void gen_increment_wr18_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr19 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr19 register
     */
    virtual void gen_increment_wr19(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr19_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr19_u register
     */
    virtual void gen_increment_wr19_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr20 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr20 register
     */
    virtual void gen_increment_wr20(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr20_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr20_u register
     */
    virtual void gen_increment_wr20_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr21 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr21 register
     */
    virtual void gen_increment_wr21(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr21_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr21_u register
     */
    virtual void gen_increment_wr21_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr22 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr22 register
     */
    virtual void gen_increment_wr22(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr22_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr22_u register
     */
    virtual void gen_increment_wr22_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr23 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr23 register
     */
    virtual void gen_increment_wr23(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr23_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr23_u register
     */
    virtual void gen_increment_wr23_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr24 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr24 register
     */
    virtual void gen_increment_wr24(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr24_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr24_u register
     */
    virtual void gen_increment_wr24_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr25 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr25 register
     */
    virtual void gen_increment_wr25(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr25_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr25_u register
     */
    virtual void gen_increment_wr25_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr26 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr26 register
     */
    virtual void gen_increment_wr26(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr26_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr26_u register
     */
    virtual void gen_increment_wr26_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr27 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr27 register
     */
    virtual void gen_increment_wr27(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr27_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr27_u register
     */
    virtual void gen_increment_wr27_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr28 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr28 register
     */
    virtual void gen_increment_wr28(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr28_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr28_u register
     */
    virtual void gen_increment_wr28_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr29 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr29 register
     */
    virtual void gen_increment_wr29(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr29_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr29_u register
     */
    virtual void gen_increment_wr29_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr30 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr30 register
     */
    virtual void gen_increment_wr30(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr30_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr30_u register
     */
    virtual void gen_increment_wr30_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr31 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr31 register
     */
    virtual void gen_increment_wr31(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr31_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr31_u register
     */
    virtual void gen_increment_wr31_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 EIPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 EIPC register
     */
    virtual void gen_increment_EIPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 EIPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 EIPSW register
     */
    virtual void gen_increment_EIPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 FEPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 FEPC register
     */
    virtual void gen_increment_FEPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 FEPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 FEPSW register
     */
    virtual void gen_increment_FEPSW(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 PSW_EIMASK register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 PSW_EIMASK register field
    */
    virtual void gen_increment_PSW_EIMASK(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FPSR_CC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPSR_CC register field
    */
    virtual void gen_increment_FPSR_CC(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FPSR_RM register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPSR_RM register field
    */
    virtual void gen_increment_FPSR_RM(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FPSR_XC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPSR_XC register field
    */
    virtual void gen_increment_FPSR_XC(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FPSR_XE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPSR_XE register field
    */
    virtual void gen_increment_FPSR_XE(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FPSR_XP register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPSR_XP register field
    */
    virtual void gen_increment_FPSR_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 FPEPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 FPEPC register
     */
    virtual void gen_increment_FPEPC(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FPST_XC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPST_XC register field
    */
    virtual void gen_increment_FPST_XC(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FPST_XP register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPST_XP register field
    */
    virtual void gen_increment_FPST_XP(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FPCC_CC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPCC_CC register field
    */
    virtual void gen_increment_FPCC_CC(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FPCFG_RM register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPCFG_RM register field
    */
    virtual void gen_increment_FPCFG_RM(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FPCFG_XE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPCFG_XE register field
    */
    virtual void gen_increment_FPCFG_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 EIIC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 EIIC register
     */
    virtual void gen_increment_EIIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 FEIC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 FEIC register
     */
    virtual void gen_increment_FEIC(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 PSWH_GPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 PSWH_GPID register field
    */
    virtual void gen_increment_PSWH_GPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 CTPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 CTPC register
     */
    virtual void gen_increment_CTPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 CTPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 CTPSW register
     */
    virtual void gen_increment_CTPSW(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 EIPSWH_GPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 EIPSWH_GPID register field
    */
    virtual void gen_increment_EIPSWH_GPID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FEPSWH_GPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FEPSWH_GPID register field
    */
    virtual void gen_increment_FEPSWH_GPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 CTBP register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 CTBP register
     */
    virtual void gen_increment_CTBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 SNZCFG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 SNZCFG register
     */
    virtual void gen_increment_SNZCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 EIWR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 EIWR register
     */
    virtual void gen_increment_EIWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 FEWR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 FEWR register
     */
    virtual void gen_increment_FEWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 SPID register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 SPID register
     */
    virtual void gen_increment_SPID(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 SPIDLIST register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 SPIDLIST register
     */
    virtual void gen_increment_SPIDLIST(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 RBASE_RBASE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 RBASE_RBASE register field
    */
    virtual void gen_increment_RBASE_RBASE(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 EBASE_EBASE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 EBASE_EBASE register field
    */
    virtual void gen_increment_EBASE_EBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 INTBP register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 INTBP register
     */
    virtual void gen_increment_INTBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PID register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PID register
     */
    virtual void gen_increment_PID(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 SVLOCK register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 SVLOCK register
     */
    virtual void gen_increment_SVLOCK(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 SCCFG_SIZE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 SCCFG_SIZE register field
    */
    virtual void gen_increment_SCCFG_SIZE(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 SCBP register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 SCBP register
     */
    virtual void gen_increment_SCBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 HVSB register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 HVSB register
     */
    virtual void gen_increment_HVSB(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PEID register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PEID register
     */
    virtual void gen_increment_PEID(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 BMID_BMID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 BMID_BMID register field
    */
    virtual void gen_increment_BMID_BMID(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 MEA register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MEA register
     */
    virtual void gen_increment_MEA(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 MEI register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MEI register
     */
    virtual void gen_increment_MEI(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 RBCR0_BE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 RBCR0_BE register field
    */
    virtual void gen_increment_RBCR0_BE(fastiss::i16_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 RBCR1_NC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 RBCR1_NC register field
    */
    virtual void gen_increment_RBCR1_NC(fastiss::i16_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 RBNR_BN register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 RBNR_BN register field
    */
    virtual void gen_increment_RBNR_BN(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 RBIP_RBIP register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 RBIP_RBIP register field
    */
    virtual void gen_increment_RBIP_RBIP(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 ISPR_ISP register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ISPR_ISP register field
    */
    virtual void gen_increment_ISPR_ISP(fastiss::i16_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 INTCFG_ULNR register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 INTCFG_ULNR register field
    */
    virtual void gen_increment_INTCFG_ULNR(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 PLMR_PLM register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 PLMR_PLM register field
    */
    virtual void gen_increment_PLMR_PLM(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXSR_RM register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXSR_RM register field
    */
    virtual void gen_increment_FXSR_RM(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXSR_XC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXSR_XC register field
    */
    virtual void gen_increment_FXSR_XC(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXSR_XE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXSR_XE register field
    */
    virtual void gen_increment_FXSR_XE(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXSR_XP register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXSR_XP register field
    */
    virtual void gen_increment_FXSR_XP(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXST_XC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXST_XC register field
    */
    virtual void gen_increment_FXST_XC(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXST_XP register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXST_XP register field
    */
    virtual void gen_increment_FXST_XP(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXCFG_RM register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXCFG_RM register field
    */
    virtual void gen_increment_FXCFG_RM(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXCFG_XE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXCFG_XE register field
    */
    virtual void gen_increment_FXCFG_XE(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXC_XC3 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXC_XC3 register field
    */
    virtual void gen_increment_FXXC_XC3(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXC_XC2 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXC_XC2 register field
    */
    virtual void gen_increment_FXXC_XC2(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXC_XC1 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXC_XC1 register field
    */
    virtual void gen_increment_FXXC_XC1(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXC_XC0 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXC_XC0 register field
    */
    virtual void gen_increment_FXXC_XC0(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXP_XP3 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXP_XP3 register field
    */
    virtual void gen_increment_FXXP_XP3(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXP_XP2 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXP_XP2 register field
    */
    virtual void gen_increment_FXXP_XP2(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXP_XP1 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXP_XP1 register field
    */
    virtual void gen_increment_FXXP_XP1(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXP_XP0 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXP_XP0 register field
    */
    virtual void gen_increment_FXXP_XP0(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMEIPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMEIPC register
     */
    virtual void gen_increment_GMEIPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMEIPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMEIPSW register
     */
    virtual void gen_increment_GMEIPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMFEPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMFEPC register
     */
    virtual void gen_increment_GMFEPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMFEPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMFEPSW register
     */
    virtual void gen_increment_GMFEPSW(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 GMPSW_EIMASK register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 GMPSW_EIMASK register field
    */
    virtual void gen_increment_GMPSW_EIMASK(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMMEA register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMMEA register
     */
    virtual void gen_increment_GMMEA(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMMEI register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMMEI register
     */
    virtual void gen_increment_GMMEI(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMEIIC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMEIIC register
     */
    virtual void gen_increment_GMEIIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMFEIC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMFEIC register
     */
    virtual void gen_increment_GMFEIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMSPID register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMSPID register
     */
    virtual void gen_increment_GMSPID(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMSPIDLIST register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMSPIDLIST register
     */
    virtual void gen_increment_GMSPIDLIST(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 GMEBASE_EBASE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 GMEBASE_EBASE register field
    */
    virtual void gen_increment_GMEBASE_EBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMINTBP register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMINTBP register
     */
    virtual void gen_increment_GMINTBP(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 GMINTCFG_ULNR register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 GMINTCFG_ULNR register field
    */
    virtual void gen_increment_GMINTCFG_ULNR(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 GMPLMR_PLM register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 GMPLMR_PLM register field
    */
    virtual void gen_increment_GMPLMR_PLM(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMSVLOCK register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMSVLOCK register
     */
    virtual void gen_increment_GMSVLOCK(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMMPM register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMMPM register
     */
    virtual void gen_increment_GMMPM(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMEIWR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMEIWR register
     */
    virtual void gen_increment_GMEIWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMFEWR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMFEWR register
     */
    virtual void gen_increment_GMFEWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMPEID register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMPEID register
     */
    virtual void gen_increment_GMPEID(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 MPM register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MPM register
     */
    virtual void gen_increment_MPM(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPCFG_NMPUE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPCFG_NMPUE register field
    */
    virtual void gen_increment_MPCFG_NMPUE(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPCFG_HBE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPCFG_HBE register field
    */
    virtual void gen_increment_MPCFG_HBE(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPCFG_ARCH register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPCFG_ARCH register field
    */
    virtual void gen_increment_MPCFG_ARCH(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPCFG_NBK register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPCFG_NBK register field
    */
    virtual void gen_increment_MPCFG_NBK(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 MCA register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MCA register
     */
    virtual void gen_increment_MCA(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 MCS register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MCS register
     */
    virtual void gen_increment_MCS(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 MCC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MCC register
     */
    virtual void gen_increment_MCC(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MCI_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MCI_SPID register field
    */
    virtual void gen_increment_MCI_SPID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPIDX_IDX register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPIDX_IDX register field
    */
    virtual void gen_increment_MPIDX_IDX(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPBK_BK register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPBK_BK register field
    */
    virtual void gen_increment_MPBK_BK(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 MPLA register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MPLA register
     */
    virtual void gen_increment_MPLA(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 MPUA register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MPUA register
     */
    virtual void gen_increment_MPUA(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID0_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID0_SPID register field
    */
    virtual void gen_increment_MPID0_SPID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID1_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID1_SPID register field
    */
    virtual void gen_increment_MPID1_SPID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID2_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID2_SPID register field
    */
    virtual void gen_increment_MPID2_SPID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID3_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID3_SPID register field
    */
    virtual void gen_increment_MPID3_SPID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID4_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID4_SPID register field
    */
    virtual void gen_increment_MPID4_SPID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID5_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID5_SPID register field
    */
    virtual void gen_increment_MPID5_SPID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID6_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID6_SPID register field
    */
    virtual void gen_increment_MPID6_SPID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID7_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID7_SPID register field
    */
    virtual void gen_increment_MPID7_SPID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 ICTAGL_LPN register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICTAGL_LPN register field
    */
    virtual void gen_increment_ICTAGL_LPN(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 ICTAGL_LRU register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICTAGL_LRU register field
    */
    virtual void gen_increment_ICTAGL_LRU(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 ICTAGH_DATAECC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICTAGH_DATAECC register field
    */
    virtual void gen_increment_ICTAGH_DATAECC(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 ICTAGH_TAGECC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICTAGH_TAGECC register field
    */
    virtual void gen_increment_ICTAGH_TAGECC(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 ICDATL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 ICDATL register
     */
    virtual void gen_increment_ICDATL(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 ICDATH register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 ICDATH register
     */
    virtual void gen_increment_ICDATH(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 ICCFG_ICHSIZE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICCFG_ICHSIZE register field
    */
    virtual void gen_increment_ICCFG_ICHSIZE(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 ICCFG_ICHLINE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICCFG_ICHLINE register field
    */
    virtual void gen_increment_ICCFG_ICHLINE(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 ICCFG_ICHWAY register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICCFG_ICHWAY register field
    */
    virtual void gen_increment_ICCFG_ICHWAY(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 ICERR_ICHEWY register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICERR_ICHEWY register field
    */
    virtual void gen_increment_ICERR_ICHEWY(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 ICERR_ICHEIX register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICERR_ICHEIX register field
    */
    virtual void gen_increment_ICERR_ICHEIX(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 TSCOUNTL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 TSCOUNTL register
     */
    virtual void gen_increment_TSCOUNTL(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 TSCOUNTH register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 TSCOUNTH register
     */
    virtual void gen_increment_TSCOUNTH(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 TSCTRL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 TSCTRL register
     */
    virtual void gen_increment_TSCTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMUMCTRL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMUMCTRL register
     */
    virtual void gen_increment_PMUMCTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMGMCTRL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMGMCTRL register
     */
    virtual void gen_increment_PMGMCTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT0 register
     */
    virtual void gen_increment_PMCOUNT0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL0 register
     */
    virtual void gen_increment_PMCTRL0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT1 register
     */
    virtual void gen_increment_PMCOUNT1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL1 register
     */
    virtual void gen_increment_PMCTRL1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT2 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT2 register
     */
    virtual void gen_increment_PMCOUNT2(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL2 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL2 register
     */
    virtual void gen_increment_PMCTRL2(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT3 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT3 register
     */
    virtual void gen_increment_PMCOUNT3(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL3 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL3 register
     */
    virtual void gen_increment_PMCTRL3(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL4 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL4 register
     */
    virtual void gen_increment_PMCTRL4(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL5 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL5 register
     */
    virtual void gen_increment_PMCTRL5(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL6 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL6 register
     */
    virtual void gen_increment_PMCTRL6(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL7 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL7 register
     */
    virtual void gen_increment_PMCTRL7(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT4 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT4 register
     */
    virtual void gen_increment_PMCOUNT4(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT5 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT5 register
     */
    virtual void gen_increment_PMCOUNT5(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT6 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT6 register
     */
    virtual void gen_increment_PMCOUNT6(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT7 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT7 register
     */
    virtual void gen_increment_PMCOUNT7(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND0 register
     */
    virtual void gen_increment_PMSUBCND0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND1 register
     */
    virtual void gen_increment_PMSUBCND1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND2 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND2 register
     */
    virtual void gen_increment_PMSUBCND2(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND3 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND3 register
     */
    virtual void gen_increment_PMSUBCND3(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND4 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND4 register
     */
    virtual void gen_increment_PMSUBCND4(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND5 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND5 register
     */
    virtual void gen_increment_PMSUBCND5(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND6 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND6 register
     */
    virtual void gen_increment_PMSUBCND6(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND7 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND7 register
     */
    virtual void gen_increment_PMSUBCND7(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DBGEN register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DBGEN register
     */
    virtual void gen_increment_DBGEN(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DBPSWH register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DBPSWH register
     */
    virtual void gen_increment_DBPSWH(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DBIC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DBIC register
     */
    virtual void gen_increment_DBIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DBPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DBPC register
     */
    virtual void gen_increment_DBPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DBPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DBPSW register
     */
    virtual void gen_increment_DBPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DIR0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DIR0 register
     */
    virtual void gen_increment_DIR0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DIR1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DIR1 register
     */
    virtual void gen_increment_DIR1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 BPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BPC register
     */
    virtual void gen_increment_BPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 BPAV register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BPAV register
     */
    virtual void gen_increment_BPAV(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 BPAM register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BPAM register
     */
    virtual void gen_increment_BPAM(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DBCFG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DBCFG register
     */
    virtual void gen_increment_DBCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DBWR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DBWR register
     */
    virtual void gen_increment_DBWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 LSTEST0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 LSTEST0 register
     */
    virtual void gen_increment_LSTEST0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 LSTEST1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 LSTEST1 register
     */
    virtual void gen_increment_LSTEST1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 LSCFG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 LSCFG register
     */
    virtual void gen_increment_LSCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 ICBKEY register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 ICBKEY register
     */
    virtual void gen_increment_ICBKEY(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 IFCR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 IFCR register
     */
    virtual void gen_increment_IFCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 IFCR1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 IFCR1 register
     */
    virtual void gen_increment_IFCR1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 BRPCTRL0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BRPCTRL0 register
     */
    virtual void gen_increment_BRPCTRL0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 BRPCTRL1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BRPCTRL1 register
     */
    virtual void gen_increment_BRPCTRL1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 BRPCFG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BRPCFG register
     */
    virtual void gen_increment_BRPCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 BRPACTRL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BRPACTRL register
     */
    virtual void gen_increment_BRPACTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 BRPDATA register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BRPDATA register
     */
    virtual void gen_increment_BRPDATA(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DCBKEY register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DCBKEY register
     */
    virtual void gen_increment_DCBKEY(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 LSUCR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 LSUCR register
     */
    virtual void gen_increment_LSUCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 LSULNK0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 LSULNK0 register
     */
    virtual void gen_increment_LSULNK0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 LSULNK1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 LSULNK1 register
     */
    virtual void gen_increment_LSULNK1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 L1RLCR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 L1RLCR register
     */
    virtual void gen_increment_L1RLCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 L1RLNK0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 L1RLNK0 register
     */
    virtual void gen_increment_L1RLNK0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 L1RLNK1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 L1RLNK1 register
     */
    virtual void gen_increment_L1RLNK1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 L1RCFG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 L1RCFG register
     */
    virtual void gen_increment_L1RCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DECFG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DECFG register
     */
    virtual void gen_increment_DECFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DECTRL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DECTRL register
     */
    virtual void gen_increment_DECTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DEVDS register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DEVDS register
     */
    virtual void gen_increment_DEVDS(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBCR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBCR register
     */
    virtual void gen_increment_RDBCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBACR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBACR register
     */
    virtual void gen_increment_RDBACR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBATAG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBATAG register
     */
    virtual void gen_increment_RDBATAG(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBADAT0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBADAT0 register
     */
    virtual void gen_increment_RDBADAT0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBADAT1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBADAT1 register
     */
    virtual void gen_increment_RDBADAT1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBADAT2 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBADAT2 register
     */
    virtual void gen_increment_RDBADAT2(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBADAT3 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBADAT3 register
     */
    virtual void gen_increment_RDBADAT3(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBSTAT register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBSTAT register
     */
    virtual void gen_increment_RDBSTAT(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 hv_func_support register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 hv_func_support register
     */
    virtual void gen_increment_hv_func_support(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 swd0pcad_exec_count register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 swd0pcad_exec_count register
     */
    virtual void gen_increment_swd0pcad_exec_count(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 swd0pcad_val register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 swd0pcad_val register
     */
    virtual void gen_increment_swd0pcad_val(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 mem_acc_type register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 mem_acc_type register
     */
    virtual void gen_increment_mem_acc_type(fastiss::i32_t *value);

    // generative register group increment operations
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 GR register group by the specified amount
     * \param index Index value of the required register from the rh850g4 GR register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 GR group register
     */
    virtual void gen_increment_GR(rh850g4::GR_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 WR register group by the specified amount
     * \param index Index value of the required register from the rh850g4 WR register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 WR group register
     */
    virtual void gen_increment_WR(rh850g4::WR_index_t index, fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_0 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_0 group register
     */
    virtual void gen_increment_SR_SELID_0(rh850g4::SR_SELID_0_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_1 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_1 group register
     */
    virtual void gen_increment_SR_SELID_1(rh850g4::SR_SELID_1_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_2 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_2 group register
     */
    virtual void gen_increment_SR_SELID_2(rh850g4::SR_SELID_2_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_3 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_3 group register
     */
    virtual void gen_increment_SR_SELID_3(rh850g4::SR_SELID_3_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_4 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_4 group register
     */
    virtual void gen_increment_SR_SELID_4(rh850g4::SR_SELID_4_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_5 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_5 group register
     */
    virtual void gen_increment_SR_SELID_5(rh850g4::SR_SELID_5_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_9 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_9 group register
     */
    virtual void gen_increment_SR_SELID_9(rh850g4::SR_SELID_9_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_10 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_10 group register
     */
    virtual void gen_increment_SR_SELID_10(rh850g4::SR_SELID_10_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_11 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_11 group register
     */
    virtual void gen_increment_SR_SELID_11(rh850g4::SR_SELID_11_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_12 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_12 group register
     */
    virtual void gen_increment_SR_SELID_12(rh850g4::SR_SELID_12_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_13 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_13 group register
     */
    virtual void gen_increment_SR_SELID_13(rh850g4::SR_SELID_13_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_14 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_14 group register
     */
    virtual void gen_increment_SR_SELID_14(rh850g4::SR_SELID_14_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_15 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_15 group register
     */
    virtual void gen_increment_SR_SELID_15(rh850g4::SR_SELID_15_index_t index, fastiss::i32_t *value);

    // generative register and register field decrement operations
    /**
     * \brief Generates code to decrement the value of the rh850g4 r0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r0 register
     */
    virtual void gen_decrement_r0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r1 register
     */
    virtual void gen_decrement_r1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r2 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r2 register
     */
    virtual void gen_decrement_r2(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r3 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r3 register
     */
    virtual void gen_decrement_r3(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r4 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r4 register
     */
    virtual void gen_decrement_r4(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r5 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r5 register
     */
    virtual void gen_decrement_r5(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r6 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r6 register
     */
    virtual void gen_decrement_r6(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r7 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r7 register
     */
    virtual void gen_decrement_r7(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r8 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r8 register
     */
    virtual void gen_decrement_r8(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r9 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r9 register
     */
    virtual void gen_decrement_r9(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r10 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r10 register
     */
    virtual void gen_decrement_r10(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r11 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r11 register
     */
    virtual void gen_decrement_r11(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r12 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r12 register
     */
    virtual void gen_decrement_r12(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r13 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r13 register
     */
    virtual void gen_decrement_r13(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r14 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r14 register
     */
    virtual void gen_decrement_r14(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r15 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r15 register
     */
    virtual void gen_decrement_r15(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r16 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r16 register
     */
    virtual void gen_decrement_r16(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r17 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r17 register
     */
    virtual void gen_decrement_r17(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r18 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r18 register
     */
    virtual void gen_decrement_r18(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r19 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r19 register
     */
    virtual void gen_decrement_r19(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r20 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r20 register
     */
    virtual void gen_decrement_r20(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r21 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r21 register
     */
    virtual void gen_decrement_r21(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r22 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r22 register
     */
    virtual void gen_decrement_r22(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r23 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r23 register
     */
    virtual void gen_decrement_r23(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r24 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r24 register
     */
    virtual void gen_decrement_r24(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r25 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r25 register
     */
    virtual void gen_decrement_r25(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r26 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r26 register
     */
    virtual void gen_decrement_r26(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r27 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r27 register
     */
    virtual void gen_decrement_r27(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r28 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r28 register
     */
    virtual void gen_decrement_r28(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r29 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r29 register
     */
    virtual void gen_decrement_r29(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r30 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r30 register
     */
    virtual void gen_decrement_r30(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r31 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r31 register
     */
    virtual void gen_decrement_r31(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PC register
     */
    virtual void gen_decrement_PC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr0 register
     */
    virtual void gen_decrement_wr0(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr0_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr0_u register
     */
    virtual void gen_decrement_wr0_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr1 register
     */
    virtual void gen_decrement_wr1(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr1_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr1_u register
     */
    virtual void gen_decrement_wr1_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr2 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr2 register
     */
    virtual void gen_decrement_wr2(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr2_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr2_u register
     */
    virtual void gen_decrement_wr2_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr3 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr3 register
     */
    virtual void gen_decrement_wr3(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr3_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr3_u register
     */
    virtual void gen_decrement_wr3_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr4 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr4 register
     */
    virtual void gen_decrement_wr4(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr4_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr4_u register
     */
    virtual void gen_decrement_wr4_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr5 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr5 register
     */
    virtual void gen_decrement_wr5(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr5_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr5_u register
     */
    virtual void gen_decrement_wr5_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr6 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr6 register
     */
    virtual void gen_decrement_wr6(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr6_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr6_u register
     */
    virtual void gen_decrement_wr6_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr7 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr7 register
     */
    virtual void gen_decrement_wr7(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr7_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr7_u register
     */
    virtual void gen_decrement_wr7_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr8 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr8 register
     */
    virtual void gen_decrement_wr8(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr8_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr8_u register
     */
    virtual void gen_decrement_wr8_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr9 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr9 register
     */
    virtual void gen_decrement_wr9(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr9_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr9_u register
     */
    virtual void gen_decrement_wr9_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr10 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr10 register
     */
    virtual void gen_decrement_wr10(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr10_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr10_u register
     */
    virtual void gen_decrement_wr10_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr11 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr11 register
     */
    virtual void gen_decrement_wr11(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr11_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr11_u register
     */
    virtual void gen_decrement_wr11_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr12 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr12 register
     */
    virtual void gen_decrement_wr12(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr12_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr12_u register
     */
    virtual void gen_decrement_wr12_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr13 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr13 register
     */
    virtual void gen_decrement_wr13(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr13_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr13_u register
     */
    virtual void gen_decrement_wr13_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr14 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr14 register
     */
    virtual void gen_decrement_wr14(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr14_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr14_u register
     */
    virtual void gen_decrement_wr14_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr15 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr15 register
     */
    virtual void gen_decrement_wr15(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr15_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr15_u register
     */
    virtual void gen_decrement_wr15_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr16 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr16 register
     */
    virtual void gen_decrement_wr16(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr16_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr16_u register
     */
    virtual void gen_decrement_wr16_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr17 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr17 register
     */
    virtual void gen_decrement_wr17(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr17_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr17_u register
     */
    virtual void gen_decrement_wr17_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr18 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr18 register
     */
    virtual void gen_decrement_wr18(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr18_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr18_u register
     */
    virtual void gen_decrement_wr18_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr19 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr19 register
     */
    virtual void gen_decrement_wr19(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr19_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr19_u register
     */
    virtual void gen_decrement_wr19_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr20 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr20 register
     */
    virtual void gen_decrement_wr20(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr20_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr20_u register
     */
    virtual void gen_decrement_wr20_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr21 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr21 register
     */
    virtual void gen_decrement_wr21(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr21_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr21_u register
     */
    virtual void gen_decrement_wr21_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr22 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr22 register
     */
    virtual void gen_decrement_wr22(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr22_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr22_u register
     */
    virtual void gen_decrement_wr22_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr23 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr23 register
     */
    virtual void gen_decrement_wr23(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr23_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr23_u register
     */
    virtual void gen_decrement_wr23_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr24 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr24 register
     */
    virtual void gen_decrement_wr24(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr24_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr24_u register
     */
    virtual void gen_decrement_wr24_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr25 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr25 register
     */
    virtual void gen_decrement_wr25(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr25_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr25_u register
     */
    virtual void gen_decrement_wr25_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr26 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr26 register
     */
    virtual void gen_decrement_wr26(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr26_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr26_u register
     */
    virtual void gen_decrement_wr26_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr27 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr27 register
     */
    virtual void gen_decrement_wr27(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr27_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr27_u register
     */
    virtual void gen_decrement_wr27_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr28 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr28 register
     */
    virtual void gen_decrement_wr28(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr28_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr28_u register
     */
    virtual void gen_decrement_wr28_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr29 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr29 register
     */
    virtual void gen_decrement_wr29(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr29_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr29_u register
     */
    virtual void gen_decrement_wr29_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr30 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr30 register
     */
    virtual void gen_decrement_wr30(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr30_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr30_u register
     */
    virtual void gen_decrement_wr30_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr31 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr31 register
     */
    virtual void gen_decrement_wr31(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr31_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr31_u register
     */
    virtual void gen_decrement_wr31_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 EIPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 EIPC register
     */
    virtual void gen_decrement_EIPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 EIPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 EIPSW register
     */
    virtual void gen_decrement_EIPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FEPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FEPC register
     */
    virtual void gen_decrement_FEPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FEPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FEPSW register
     */
    virtual void gen_decrement_FEPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PSW_EIMASK register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PSW_EIMASK register field
     */
    virtual void gen_decrement_PSW_EIMASK(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPSR_CC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPSR_CC register field
     */
    virtual void gen_decrement_FPSR_CC(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPSR_RM register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPSR_RM register field
     */
    virtual void gen_decrement_FPSR_RM(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPSR_XC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPSR_XC register field
     */
    virtual void gen_decrement_FPSR_XC(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPSR_XE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPSR_XE register field
     */
    virtual void gen_decrement_FPSR_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPSR_XP register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPSR_XP register field
     */
    virtual void gen_decrement_FPSR_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPEPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPEPC register
     */
    virtual void gen_decrement_FPEPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPST_XC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPST_XC register field
     */
    virtual void gen_decrement_FPST_XC(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPST_XP register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPST_XP register field
     */
    virtual void gen_decrement_FPST_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPCC_CC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPCC_CC register field
     */
    virtual void gen_decrement_FPCC_CC(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPCFG_RM register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPCFG_RM register field
     */
    virtual void gen_decrement_FPCFG_RM(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPCFG_XE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPCFG_XE register field
     */
    virtual void gen_decrement_FPCFG_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 EIIC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 EIIC register
     */
    virtual void gen_decrement_EIIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FEIC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FEIC register
     */
    virtual void gen_decrement_FEIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PSWH_GPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PSWH_GPID register field
     */
    virtual void gen_decrement_PSWH_GPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 CTPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 CTPC register
     */
    virtual void gen_decrement_CTPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 CTPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 CTPSW register
     */
    virtual void gen_decrement_CTPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 EIPSWH_GPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 EIPSWH_GPID register field
     */
    virtual void gen_decrement_EIPSWH_GPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FEPSWH_GPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FEPSWH_GPID register field
     */
    virtual void gen_decrement_FEPSWH_GPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 CTBP register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 CTBP register
     */
    virtual void gen_decrement_CTBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 SNZCFG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 SNZCFG register
     */
    virtual void gen_decrement_SNZCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 EIWR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 EIWR register
     */
    virtual void gen_decrement_EIWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FEWR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FEWR register
     */
    virtual void gen_decrement_FEWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 SPID register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 SPID register
     */
    virtual void gen_decrement_SPID(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 SPIDLIST register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 SPIDLIST register
     */
    virtual void gen_decrement_SPIDLIST(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RBASE_RBASE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RBASE_RBASE register field
     */
    virtual void gen_decrement_RBASE_RBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 EBASE_EBASE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 EBASE_EBASE register field
     */
    virtual void gen_decrement_EBASE_EBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 INTBP register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 INTBP register
     */
    virtual void gen_decrement_INTBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PID register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PID register
     */
    virtual void gen_decrement_PID(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 SVLOCK register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 SVLOCK register
     */
    virtual void gen_decrement_SVLOCK(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 SCCFG_SIZE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 SCCFG_SIZE register field
     */
    virtual void gen_decrement_SCCFG_SIZE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 SCBP register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 SCBP register
     */
    virtual void gen_decrement_SCBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 HVSB register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 HVSB register
     */
    virtual void gen_decrement_HVSB(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PEID register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PEID register
     */
    virtual void gen_decrement_PEID(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 BMID_BMID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BMID_BMID register field
     */
    virtual void gen_decrement_BMID_BMID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MEA register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MEA register
     */
    virtual void gen_decrement_MEA(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MEI register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MEI register
     */
    virtual void gen_decrement_MEI(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RBCR0_BE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RBCR0_BE register field
     */
    virtual void gen_decrement_RBCR0_BE(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RBCR1_NC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RBCR1_NC register field
     */
    virtual void gen_decrement_RBCR1_NC(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RBNR_BN register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RBNR_BN register field
     */
    virtual void gen_decrement_RBNR_BN(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RBIP_RBIP register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RBIP_RBIP register field
     */
    virtual void gen_decrement_RBIP_RBIP(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ISPR_ISP register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ISPR_ISP register field
     */
    virtual void gen_decrement_ISPR_ISP(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 INTCFG_ULNR register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 INTCFG_ULNR register field
     */
    virtual void gen_decrement_INTCFG_ULNR(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PLMR_PLM register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PLMR_PLM register field
     */
    virtual void gen_decrement_PLMR_PLM(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXSR_RM register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXSR_RM register field
     */
    virtual void gen_decrement_FXSR_RM(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXSR_XC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXSR_XC register field
     */
    virtual void gen_decrement_FXSR_XC(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXSR_XE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXSR_XE register field
     */
    virtual void gen_decrement_FXSR_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXSR_XP register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXSR_XP register field
     */
    virtual void gen_decrement_FXSR_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXST_XC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXST_XC register field
     */
    virtual void gen_decrement_FXST_XC(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXST_XP register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXST_XP register field
     */
    virtual void gen_decrement_FXST_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXCFG_RM register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXCFG_RM register field
     */
    virtual void gen_decrement_FXCFG_RM(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXCFG_XE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXCFG_XE register field
     */
    virtual void gen_decrement_FXCFG_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXC_XC3 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXC_XC3 register field
     */
    virtual void gen_decrement_FXXC_XC3(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXC_XC2 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXC_XC2 register field
     */
    virtual void gen_decrement_FXXC_XC2(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXC_XC1 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXC_XC1 register field
     */
    virtual void gen_decrement_FXXC_XC1(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXC_XC0 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXC_XC0 register field
     */
    virtual void gen_decrement_FXXC_XC0(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXP_XP3 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXP_XP3 register field
     */
    virtual void gen_decrement_FXXP_XP3(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXP_XP2 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXP_XP2 register field
     */
    virtual void gen_decrement_FXXP_XP2(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXP_XP1 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXP_XP1 register field
     */
    virtual void gen_decrement_FXXP_XP1(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXP_XP0 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXP_XP0 register field
     */
    virtual void gen_decrement_FXXP_XP0(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMEIPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMEIPC register
     */
    virtual void gen_decrement_GMEIPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMEIPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMEIPSW register
     */
    virtual void gen_decrement_GMEIPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMFEPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMFEPC register
     */
    virtual void gen_decrement_GMFEPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMFEPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMFEPSW register
     */
    virtual void gen_decrement_GMFEPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMPSW_EIMASK register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMPSW_EIMASK register field
     */
    virtual void gen_decrement_GMPSW_EIMASK(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMMEA register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMMEA register
     */
    virtual void gen_decrement_GMMEA(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMMEI register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMMEI register
     */
    virtual void gen_decrement_GMMEI(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMEIIC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMEIIC register
     */
    virtual void gen_decrement_GMEIIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMFEIC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMFEIC register
     */
    virtual void gen_decrement_GMFEIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMSPID register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMSPID register
     */
    virtual void gen_decrement_GMSPID(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMSPIDLIST register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMSPIDLIST register
     */
    virtual void gen_decrement_GMSPIDLIST(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMEBASE_EBASE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMEBASE_EBASE register field
     */
    virtual void gen_decrement_GMEBASE_EBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMINTBP register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMINTBP register
     */
    virtual void gen_decrement_GMINTBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMINTCFG_ULNR register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMINTCFG_ULNR register field
     */
    virtual void gen_decrement_GMINTCFG_ULNR(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMPLMR_PLM register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMPLMR_PLM register field
     */
    virtual void gen_decrement_GMPLMR_PLM(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMSVLOCK register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMSVLOCK register
     */
    virtual void gen_decrement_GMSVLOCK(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMMPM register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMMPM register
     */
    virtual void gen_decrement_GMMPM(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMEIWR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMEIWR register
     */
    virtual void gen_decrement_GMEIWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMFEWR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMFEWR register
     */
    virtual void gen_decrement_GMFEWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMPEID register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMPEID register
     */
    virtual void gen_decrement_GMPEID(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPM register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPM register
     */
    virtual void gen_decrement_MPM(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPCFG_NMPUE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPCFG_NMPUE register field
     */
    virtual void gen_decrement_MPCFG_NMPUE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPCFG_HBE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPCFG_HBE register field
     */
    virtual void gen_decrement_MPCFG_HBE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPCFG_ARCH register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPCFG_ARCH register field
     */
    virtual void gen_decrement_MPCFG_ARCH(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPCFG_NBK register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPCFG_NBK register field
     */
    virtual void gen_decrement_MPCFG_NBK(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MCA register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MCA register
     */
    virtual void gen_decrement_MCA(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MCS register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MCS register
     */
    virtual void gen_decrement_MCS(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MCC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MCC register
     */
    virtual void gen_decrement_MCC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MCI_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MCI_SPID register field
     */
    virtual void gen_decrement_MCI_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPIDX_IDX register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPIDX_IDX register field
     */
    virtual void gen_decrement_MPIDX_IDX(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPBK_BK register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPBK_BK register field
     */
    virtual void gen_decrement_MPBK_BK(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPLA register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPLA register
     */
    virtual void gen_decrement_MPLA(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPUA register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPUA register
     */
    virtual void gen_decrement_MPUA(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID0_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID0_SPID register field
     */
    virtual void gen_decrement_MPID0_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID1_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID1_SPID register field
     */
    virtual void gen_decrement_MPID1_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID2_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID2_SPID register field
     */
    virtual void gen_decrement_MPID2_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID3_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID3_SPID register field
     */
    virtual void gen_decrement_MPID3_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID4_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID4_SPID register field
     */
    virtual void gen_decrement_MPID4_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID5_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID5_SPID register field
     */
    virtual void gen_decrement_MPID5_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID6_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID6_SPID register field
     */
    virtual void gen_decrement_MPID6_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID7_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID7_SPID register field
     */
    virtual void gen_decrement_MPID7_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICTAGL_LPN register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICTAGL_LPN register field
     */
    virtual void gen_decrement_ICTAGL_LPN(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICTAGL_LRU register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICTAGL_LRU register field
     */
    virtual void gen_decrement_ICTAGL_LRU(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICTAGH_DATAECC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICTAGH_DATAECC register field
     */
    virtual void gen_decrement_ICTAGH_DATAECC(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICTAGH_TAGECC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICTAGH_TAGECC register field
     */
    virtual void gen_decrement_ICTAGH_TAGECC(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICDATL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICDATL register
     */
    virtual void gen_decrement_ICDATL(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICDATH register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICDATH register
     */
    virtual void gen_decrement_ICDATH(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICCFG_ICHSIZE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICCFG_ICHSIZE register field
     */
    virtual void gen_decrement_ICCFG_ICHSIZE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICCFG_ICHLINE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICCFG_ICHLINE register field
     */
    virtual void gen_decrement_ICCFG_ICHLINE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICCFG_ICHWAY register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICCFG_ICHWAY register field
     */
    virtual void gen_decrement_ICCFG_ICHWAY(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICERR_ICHEWY register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICERR_ICHEWY register field
     */
    virtual void gen_decrement_ICERR_ICHEWY(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICERR_ICHEIX register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICERR_ICHEIX register field
     */
    virtual void gen_decrement_ICERR_ICHEIX(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 TSCOUNTL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 TSCOUNTL register
     */
    virtual void gen_decrement_TSCOUNTL(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 TSCOUNTH register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 TSCOUNTH register
     */
    virtual void gen_decrement_TSCOUNTH(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 TSCTRL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 TSCTRL register
     */
    virtual void gen_decrement_TSCTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMUMCTRL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMUMCTRL register
     */
    virtual void gen_decrement_PMUMCTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMGMCTRL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMGMCTRL register
     */
    virtual void gen_decrement_PMGMCTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT0 register
     */
    virtual void gen_decrement_PMCOUNT0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL0 register
     */
    virtual void gen_decrement_PMCTRL0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT1 register
     */
    virtual void gen_decrement_PMCOUNT1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL1 register
     */
    virtual void gen_decrement_PMCTRL1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT2 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT2 register
     */
    virtual void gen_decrement_PMCOUNT2(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL2 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL2 register
     */
    virtual void gen_decrement_PMCTRL2(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT3 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT3 register
     */
    virtual void gen_decrement_PMCOUNT3(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL3 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL3 register
     */
    virtual void gen_decrement_PMCTRL3(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL4 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL4 register
     */
    virtual void gen_decrement_PMCTRL4(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL5 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL5 register
     */
    virtual void gen_decrement_PMCTRL5(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL6 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL6 register
     */
    virtual void gen_decrement_PMCTRL6(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL7 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL7 register
     */
    virtual void gen_decrement_PMCTRL7(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT4 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT4 register
     */
    virtual void gen_decrement_PMCOUNT4(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT5 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT5 register
     */
    virtual void gen_decrement_PMCOUNT5(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT6 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT6 register
     */
    virtual void gen_decrement_PMCOUNT6(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT7 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT7 register
     */
    virtual void gen_decrement_PMCOUNT7(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND0 register
     */
    virtual void gen_decrement_PMSUBCND0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND1 register
     */
    virtual void gen_decrement_PMSUBCND1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND2 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND2 register
     */
    virtual void gen_decrement_PMSUBCND2(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND3 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND3 register
     */
    virtual void gen_decrement_PMSUBCND3(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND4 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND4 register
     */
    virtual void gen_decrement_PMSUBCND4(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND5 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND5 register
     */
    virtual void gen_decrement_PMSUBCND5(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND6 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND6 register
     */
    virtual void gen_decrement_PMSUBCND6(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND7 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND7 register
     */
    virtual void gen_decrement_PMSUBCND7(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DBGEN register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DBGEN register
     */
    virtual void gen_decrement_DBGEN(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DBPSWH register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DBPSWH register
     */
    virtual void gen_decrement_DBPSWH(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DBIC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DBIC register
     */
    virtual void gen_decrement_DBIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DBPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DBPC register
     */
    virtual void gen_decrement_DBPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DBPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DBPSW register
     */
    virtual void gen_decrement_DBPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DIR0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DIR0 register
     */
    virtual void gen_decrement_DIR0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DIR1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DIR1 register
     */
    virtual void gen_decrement_DIR1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 BPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BPC register
     */
    virtual void gen_decrement_BPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 BPAV register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BPAV register
     */
    virtual void gen_decrement_BPAV(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 BPAM register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BPAM register
     */
    virtual void gen_decrement_BPAM(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DBCFG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DBCFG register
     */
    virtual void gen_decrement_DBCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DBWR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DBWR register
     */
    virtual void gen_decrement_DBWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 LSTEST0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 LSTEST0 register
     */
    virtual void gen_decrement_LSTEST0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 LSTEST1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 LSTEST1 register
     */
    virtual void gen_decrement_LSTEST1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 LSCFG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 LSCFG register
     */
    virtual void gen_decrement_LSCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICBKEY register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICBKEY register
     */
    virtual void gen_decrement_ICBKEY(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 IFCR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 IFCR register
     */
    virtual void gen_decrement_IFCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 IFCR1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 IFCR1 register
     */
    virtual void gen_decrement_IFCR1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 BRPCTRL0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BRPCTRL0 register
     */
    virtual void gen_decrement_BRPCTRL0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 BRPCTRL1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BRPCTRL1 register
     */
    virtual void gen_decrement_BRPCTRL1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 BRPCFG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BRPCFG register
     */
    virtual void gen_decrement_BRPCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 BRPACTRL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BRPACTRL register
     */
    virtual void gen_decrement_BRPACTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 BRPDATA register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BRPDATA register
     */
    virtual void gen_decrement_BRPDATA(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DCBKEY register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DCBKEY register
     */
    virtual void gen_decrement_DCBKEY(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 LSUCR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 LSUCR register
     */
    virtual void gen_decrement_LSUCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 LSULNK0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 LSULNK0 register
     */
    virtual void gen_decrement_LSULNK0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 LSULNK1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 LSULNK1 register
     */
    virtual void gen_decrement_LSULNK1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 L1RLCR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 L1RLCR register
     */
    virtual void gen_decrement_L1RLCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 L1RLNK0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 L1RLNK0 register
     */
    virtual void gen_decrement_L1RLNK0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 L1RLNK1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 L1RLNK1 register
     */
    virtual void gen_decrement_L1RLNK1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 L1RCFG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 L1RCFG register
     */
    virtual void gen_decrement_L1RCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DECFG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DECFG register
     */
    virtual void gen_decrement_DECFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DECTRL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DECTRL register
     */
    virtual void gen_decrement_DECTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DEVDS register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DEVDS register
     */
    virtual void gen_decrement_DEVDS(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBCR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBCR register
     */
    virtual void gen_decrement_RDBCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBACR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBACR register
     */
    virtual void gen_decrement_RDBACR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBATAG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBATAG register
     */
    virtual void gen_decrement_RDBATAG(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBADAT0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBADAT0 register
     */
    virtual void gen_decrement_RDBADAT0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBADAT1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBADAT1 register
     */
    virtual void gen_decrement_RDBADAT1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBADAT2 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBADAT2 register
     */
    virtual void gen_decrement_RDBADAT2(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBADAT3 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBADAT3 register
     */
    virtual void gen_decrement_RDBADAT3(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBSTAT register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBSTAT register
     */
    virtual void gen_decrement_RDBSTAT(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 hv_func_support register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 hv_func_support register
     */
    virtual void gen_decrement_hv_func_support(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 swd0pcad_exec_count register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 swd0pcad_exec_count register
     */
    virtual void gen_decrement_swd0pcad_exec_count(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 swd0pcad_val register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 swd0pcad_val register
     */
    virtual void gen_decrement_swd0pcad_val(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 mem_acc_type register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 mem_acc_type register
     */
    virtual void gen_decrement_mem_acc_type(fastiss::i32_t *value);

    // generative register group decrement operations
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 GR register group by the specified amount
     * \param index Index value of the required register from the rh850g4 GR register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 GR group register
     */
    virtual void gen_decrement_GR(rh850g4::GR_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 WR register group by the specified amount
     * \param index Index value of the required register from the rh850g4 WR register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 WR group register
     */
    virtual void gen_decrement_WR(rh850g4::WR_index_t index, fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_0 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_0 group register
     */
    virtual void gen_decrement_SR_SELID_0(rh850g4::SR_SELID_0_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_1 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_1 group register
     */
    virtual void gen_decrement_SR_SELID_1(rh850g4::SR_SELID_1_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_2 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_2 group register
     */
    virtual void gen_decrement_SR_SELID_2(rh850g4::SR_SELID_2_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_3 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_3 group register
     */
    virtual void gen_decrement_SR_SELID_3(rh850g4::SR_SELID_3_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_4 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_4 group register
     */
    virtual void gen_decrement_SR_SELID_4(rh850g4::SR_SELID_4_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_5 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_5 group register
     */
    virtual void gen_decrement_SR_SELID_5(rh850g4::SR_SELID_5_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_9 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_9 group register
     */
    virtual void gen_decrement_SR_SELID_9(rh850g4::SR_SELID_9_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_10 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_10 group register
     */
    virtual void gen_decrement_SR_SELID_10(rh850g4::SR_SELID_10_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_11 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_11 group register
     */
    virtual void gen_decrement_SR_SELID_11(rh850g4::SR_SELID_11_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_12 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_12 group register
     */
    virtual void gen_decrement_SR_SELID_12(rh850g4::SR_SELID_12_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_13 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_13 group register
     */
    virtual void gen_decrement_SR_SELID_13(rh850g4::SR_SELID_13_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_14 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_14 group register
     */
    virtual void gen_decrement_SR_SELID_14(rh850g4::SR_SELID_14_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_15 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_15 group register
     */
    virtual void gen_decrement_SR_SELID_15(rh850g4::SR_SELID_15_index_t index, fastiss::i32_t *value);

  protected:
    /**
     * Handle to code generator instance
     */
    fastiss::code_gen_if_t *m_code_gen_impl;
    /**
     * Type specifications for CPU state structure
     */
    fastiss::struct_type_t *m_cpu_state_type;
    /**
     * Type specification for block functions
     */
    fastiss::func_type_t *m_block_function_type;
    /**
     * CPU state storage
     */
    rh850g4::cpu_state_t m_cpu_state;
    /**
     * Handle to cpu state argument to current block function
     */
    fastiss::func_arg_t *m_cpu_state_arg;
    /**
     * Boolean flag that indicates if instructions should be counted (if true)
     * or not (if false)
     */
    bool m_count_instructions;
  };
}

#endif // _RH850G4_CPU_STATE_IMPL_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
