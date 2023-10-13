// -----------------------------------------------------------------------------
// $Id: rhsb_regif.h,v 1.2 2018/08/02 08:08:50 ngathunguyen Exp $
//
// Copyright(c) 2016 - 2018 Renesas Electronics Corporation
// Copyright(c) 2016 - 2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------
// This file is generated by Register I/F generator
//    gen_regif.py 1.9 2013/12/04 09:51:36 sontran
//    gen_regif_class.py 1.60 2014/10/24 09:42:15 ducduong
//    regif_h.skl 1.30 2014/10/13 03:14:25 sontran
//
// Input file : rhsb_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE RHSB
//     #                   name            offset_size
//     %%REG_INSTANCE      reg_def         7
// 
// %REG_CHANNEL reg_def
//     %%TITLE  group  name          reg_name     wsize    rsize    length  offset  factor_start  factor_end  factor_index  factor_step  access  init        support  callback
//     %%REG    -      RHSBGC        RHSBGC       8|16|32  8|16|32  32      0x00    -             -           -             -            W|R     0x0         TRUE     -  
//     %%REG    -      RHSBMSR       RHSBMSR      8|16|32  8|16|32  32      0x08    -             -           -             -            R       0x10000     TRUE     -  
//     %%REG    -      RHSBDCR       RHSBDCR        16|32  8|16|32  32      0x10    -             -           -             -            W|R     0x11FF00    TRUE     -  
//     %%REG    -      RHSBDEC       RHSBDEC      8|16|32  8|16|32  32      0x14    -             -           -             -            W|R     0x0         TRUE     -  
//     %%REG    -      RHSBSDC       RHSBSDC        16|32  8|16|32  32      0x18    -             -           -             -            W|R     0x0         TRUE     -  
//     %%REG    -      RHSBDEBA      RHSBDEBA       16|32  8|16|32  32      0x20    0             3           -             0x4          W|R     0x0         TRUE     -  
//     %%REG    -      RHSBEBE0      RHSBEBE0       16|32  8|16|32  32      0x30    -             -           -             -            W|R     0x0         TRUE     -  
//     %%REG    -      RHSBEBE1      RHSBEBE1       16|32  8|16|32  32      0x34    -             -           -             -            W|R     0x0         TRUE     -  
//     %%REG    -      RHSBDTC       RHSBDTC      8|16|32  8|16|32  32      0x38    -             -           -             -            W|R     0x0         TRUE     -  
//     %%REG    -      RHSBDCD       RHSBDCD        16|32  8|16|32  32      0x3C    -             -           -             -            W|R     0x0         TRUE     -  
//     %%REG    -      RHSBDDR0      RHSBDDR0       16|32  8|16|32  32      0x40    -             -           -             -            W|R     0x0         TRUE     -  
//     %%REG    -      RHSBDDR1      RHSBDDR1       16|32  8|16|32  32      0x44    -             -           -             -            W|R     0x0         TRUE     -  
//     %%REG    -      RHSBDED0      RHSBDED0       16|32  8|16|32  32      0x48    -             -           -             -            W|R     0x0         TRUE     -  
//     %%REG    -      RHSBDED1      RHSBDED1       16|32  8|16|32  32      0x4C    -             -           -             -            W|R     0x0         TRUE     -  
//     %%REG    -      RHSBUCR       RHSBUCR      8|16|32  8|16|32  32      0x50    -             -           -             -            W|R     0x1800      TRUE     -  
//     %%REG    -      RHSBUCC       RHSBUCC      8|16|32  8|16|32  32      0x54    -             -           -             -            W|R     0xF0F0F0F   TRUE     -  
//     %%REG    -      RHSBUCS       RHSBUCS      8|16|32  8|16|32  32      0x58    -             -           -             -            W|R     0x0         TRUE     -  
//     %%REG    -      RHSBUDR       RHSBUDR      8|16|32  8|16|32  32      0x5C    -             -           -             -              R     0x0         TRUE     -  
//     %%REG    -      RHSBUD        RHSBUD       8|16|32  8|16|32  32      0x60    0             1           -             0x4         W0|R     0x0         TRUE     -  
//     %%REG    -      RHSBUSS       RHSBUSS      8|16|32  8|16|32  32      0x70    -             -           -             -              R     0x0         TRUE     -  
//     %%REG    -      RHSBIC        RHSBIC       8|16|32  8|16|32  32      0x74    -             -           -             -            W|R     0x0         TRUE     -  
//     %%REG    -      RHSBIS        RHSBIS       8|16|32  8|16|32  32      0x78    -             -           -             -            W0|R    0x0         TRUE     -  
//     %%REG    -      RHSBDCR1      RHSBDCR1     8|16|32  8|16|32  32      0x7C    -             -           -             -            W|R     0x0         TRUE     -  
// 
// %REG_NAME RHSBGC
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    OPS              1      0      0         W|R     TRUE     W
//     %%BIT    UDE              16     16     0         W|R     TRUE     -
//     %%BIT    DDE              17     17     0         W|R     TRUE     -
//     %%BIT    DCDE             18     18     0         W|R     TRUE     -
// 
// %REG_NAME RHSBMSR
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    UFA              0      0      0         R       TRUE     -
//     %%BIT    TPS              16     16     0x1       R       TRUE     -
//     %%BIT    DFA              26     24     0         R       TRUE     -
// 
// %REG_NAME RHSBDCR
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    DMS              1      0      0         W|R     TRUE     W
//     %%BIT    EIP              2      2      0         W|R     TRUE     -
//     %%BIT    EIM              3      3      0         W|R     TRUE     -
//     %%BIT    EE               5      4      0         W|R     TRUE     -
//     %%BIT    CAC              6      6      0         W|R     TRUE     -
//     %%BIT    CLP              7      7      0         W|R     TRUE     -
//     %%BIT    REP              16     8      0x1FF     W|R     TRUE     -
//     %%BIT    CTD              17     17     0         W|R     TRUE     -
//     %%BIT    CIM              19     18     0         W|R     TRUE     -
//     %%BIT    DBR              23     20     1         W|R     TRUE     -
//     %%BIT    DFP              29     24     0         W|R     TRUE     -
//     %%BIT    SLS              31     30     0         W|R     TRUE     -
// 
// %REG_NAME RHSBDEC
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    NDB3             3      0      0         W|R     TRUE     W
//     %%BIT    SSD3             5      4      0         W|R     TRUE     -
//     %%BIT    NDB2             11     8      0         W|R     TRUE     -
//     %%BIT    SSD2             13     12     0         W|R     TRUE     -
//     %%BIT    NDB1             19     16     0         W|R     TRUE     -
//     %%BIT    SSD1             21     20     0         W|R     TRUE     -
//     %%BIT    NDB0             27     24     0         W|R     TRUE     -
//     %%BIT    SSD0             29     28     0         W|R     TRUE     -
// 
// %REG_NAME RHSBSDC
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    DPL1             2      0      0         W|R     TRUE     W
//     %%BIT    CPS1             3      3      0         W|R     TRUE     -
//     %%BIT    SOLP1            4      4      0         W|R     TRUE     -
//     %%BIT    CSLP1            5      5      0         W|R     TRUE     -
//     %%BIT    APL1             10     8      0         W|R     TRUE     -
//     %%BIT    DPL0             18     16     0         W|R     TRUE     -
//     %%BIT    CPS0             19     19     0         W|R     TRUE     -
//     %%BIT    SOLP0            20     20     0         W|R     TRUE     -
//     %%BIT    CSLP0            21     21     0         W|R     TRUE     -
//     %%BIT    APL0             26     24     0         W|R     TRUE     -
// 
// %REG_NAME RHSBDEBA
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    DSS0             1      0      0         W|R     TRUE     W
//     %%BIT    DSS1             3      2      0         W|R     TRUE     -
//     %%BIT    DSS2             5      4      0         W|R     TRUE     -
//     %%BIT    DSS3             7      6      0         W|R     TRUE     -
//     %%BIT    DSS4             9      8      0         W|R     TRUE     -
//     %%BIT    DSS5             11     10     0         W|R     TRUE     -
//     %%BIT    DSS6             13     12     0         W|R     TRUE     -
//     %%BIT    DSS7             15     14     0         W|R     TRUE     -
//     %%BIT    DSS8             17     16     0         W|R     TRUE     -
//     %%BIT    DSS9             19     18     0         W|R     TRUE     -
//     %%BIT    DSS10            21     20     0         W|R     TRUE     -
//     %%BIT    DSS11            23     22     0         W|R     TRUE     -
//     %%BIT    DSS12            25     24     0         W|R     TRUE     -
//     %%BIT    DSS13            27     26     0         W|R     TRUE     -
//     %%BIT    DSS14            29     28     0         W|R     TRUE     -
//     %%BIT    DSS15            31     30     0         W|R     TRUE     -
// 
// %REG_NAME RHSBEBE0
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    EBE23_16         7      0      0         W|R     TRUE     W
//     %%BIT    EBE31_24         15     8      0         W|R     TRUE     -
//     %%BIT    EBE7_0           23     16     0         W|R     TRUE     -
//     %%BIT    EBE15_8          31     24     0         W|R     TRUE     -
// 
// %REG_NAME RHSBEBE1
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    EBE55_48         7      0      0         W|R     TRUE     W
//     %%BIT    EBE63_56         15     8      0         W|R     TRUE     -
//     %%BIT    EBE39_32         23     16     0         W|R     TRUE     -
//     %%BIT    EBE47_40         31     24     0         W|R     TRUE     -
// 
// %REG_NAME RHSBDTC
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    DTE              0      0      0         W|R     TRUE     W
//     %%BIT    TSR              1      1      0         W|R     TRUE     -
//     %%BIT    CTR              17     16     0         W|R     TRUE     -
//     %%BIT    SSCF             21     20     0         W|R     TRUE     -
//     %%BIT    NCB              28     24     0         W|R     TRUE     -
// 
// %REG_NAME RHSBDCD
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    CB               31     0      0         W|R     TRUE     W
// 
// %REG_NAME RHSBDDR0
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    DB31_16          15     0      0         W|R     TRUE     W
//     %%BIT    DB15_0           31     16     0         W|R     TRUE     -
// 
// %REG_NAME RHSBDDR1
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    DB63_48          15     0      0         W|R     TRUE     W
//     %%BIT    DB47_32          31     16     0         W|R     TRUE     -
// 
// %REG_NAME RHSBED0
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    EB31_16          15     0      0         W|R     TRUE     W
//     %%BIT    EB15_0           31     16     0         W|R     TRUE     -
// 
// %REG_NAME RHSBDED0
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    EB31_16          15     0      0         W|R     TRUE     W
//     %%BIT    EB15_0           31     16     0         W|R     TRUE     -
// 
// %REG_NAME RHSBDED1
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    EB63_48          15     0      0         W|R     TRUE     W
//     %%BIT    EB47_32          31     16     0         W|R     TRUE     -
// 
// %REG_NAME RHSBUCR
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    FSM              0      0      0         W|R     TRUE     W
//     %%BIT    UMS              1      1      0         W|R     TRUE     -
//     %%BIT    TOE              2      2      0         W|R     TRUE     -
//     %%BIT    UE               3      3      0         W|R     TRUE     -
//     %%BIT    RTO              12     8      0x18        R     TRUE     -
// 
// %REG_NAME RHSBUCC
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    Reserved         15     0      0x0F0F    R       TRUE     -
//     %%BIT    UBR1             19     16     0xF       W|R     TRUE     W
//     %%BIT    PC1              20     20     0         W|R     TRUE     -
//     %%BIT    ILP1             21     21     0         W|R     TRUE     -
//     %%BIT    SBN1             22     22     0         W|R     TRUE     -
//     %%BIT    UFT1             23     23     0         W|R     TRUE     -
//     %%BIT    UBR0             27     24     0xF       W|R     TRUE     -
//     %%BIT    PC0              28     28     0         W|R     TRUE     -
//     %%BIT    ILP0             29     29     0         W|R     TRUE     -
//     %%BIT    SBN0             30     30     0         W|R     TRUE     -
//     %%BIT    UFT0             31     31     0         W|R     TRUE     -
// 
// %REG_NAME RHSBUCS
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    ACC              25     24     0         W|R     TRUE     W
//     %%BIT    BSY              31     31     0           R     TRUE     -
// 
// %REG_NAME RHSBUDR
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    DF               7      0      0           R     TRUE     R
//     %%BIT    EDF              11     8      0           R     TRUE     -
//     %%BIT    ND               16     16     0           R     TRUE     -
//     %%BIT    PERR             17     17     0           R     TRUE     -
//     %%BIT    FERR             18     18     0           R     TRUE     -
//     %%BIT    TO               19     19     0           R     TRUE     -
//     %%BIT    DL               20     20     0           R     TRUE     -
//     %%BIT    LUD              25     24     0           R     TRUE     -
//     %%BIT    NDS1             30     30     0           R     TRUE     -
//     %%BIT    NDS0             31     31     0           R     TRUE     -
// 
// %REG_NAME RHSBUD
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    DF               7      0      0           R     TRUE     -
//     %%BIT    EDF              11     8      0           R     TRUE     -
//     %%BIT    ND               16     16     0        W0|R     TRUE     -
//     %%BIT    PERR             17     17     0        W0|R     TRUE     -
//     %%BIT    FERR             18     18     0        W0|R     TRUE     -
//     %%BIT    TO               19     19     0        W0|R     TRUE     -
//     %%BIT    DL               20     20     0        W0|R     TRUE     -
// 
// %REG_NAME RHSBUSS
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    ND1              16     16      0           R     TRUE     -
//     %%BIT    PERR1            17     17      0           R     TRUE     -
//     %%BIT    FERR1            18     18      0           R     TRUE     -
//     %%BIT    TO1              19     19      0           R     TRUE     -
//     %%BIT    DL1              20     20      0           R     TRUE     -
//     %%BIT    ND0              24     24      0           R     TRUE     -
//     %%BIT    PERR0            25     25      0           R     TRUE     -
//     %%BIT    FERR0            26     26      0           R     TRUE     -
//     %%BIT    TO0              27     27      0           R     TRUE     -
//     %%BIT    DL0              28     28      0           R     TRUE     -
// 
// %REG_NAME RHSBIC
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    DTSIE            16     16      0         W|R     TRUE     W
//     %%BIT    DTIE             17     17      0         W|R     TRUE     -
//     %%BIT    CTIE             18     18      0         W|R     TRUE     -
//     %%BIT    ETIE             19     19      0         W|R     TRUE     -
//     %%BIT    TSIE             20     20      0         W|R     TRUE     -
//     %%BIT    DRIE             24     24      0         W|R     TRUE     -
//     %%BIT    UEIE             25     25      0         W|R     TRUE     -
//     %%BIT    TOIE             26     26      0         W|R     TRUE     -
//     %%BIT    DLIE             27     27      0         W|R     TRUE     -
// 
// %REG_NAME RHSBIS
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    ERF              8      8       0        W0|R     TRUE     W
//     %%BIT    DTSF             16     16      0        W0|R     TRUE     -
//     %%BIT    DTF              17     17      0        W0|R     TRUE     -
//     %%BIT    CTF              18     18      0        W0|R     TRUE     -
//     %%BIT    ETF              19     19      0        W0|R     TRUE     -
//     %%BIT    TSF              20     20      0        W0|R     TRUE     -
//     %%BIT    DRF              24     24      0        W0|R     TRUE     -
//     %%BIT    UEF              25     25      0        W0|R     TRUE     -
//     %%BIT    TOF              26     26      0        W0|R     TRUE     -
//     %%BIT    DLF              27     27      0        W0|R     TRUE     -
// 
// %REG_NAME RHSBDCR1
//     %%TITLE  name             upper  lower  init      access  support  callback                                     
//     %%BIT    SLS1             31     30     0         W|R     TRUE     W
////////////////////////////////////////////////////////////////////////////////
/// @file rhsb_regif.h
/// @brief Register IF class of model RHSB
/// $Id: rhsb_regif.h,v 1.2 2018/08/02 08:08:50 ngathunguyen Exp $
/// $Date: 2018/08/02 08:08:50 $
/// $Revison$
/// $Author: ngathunguyen $
////////////////////////////////////////////////////////////////////////////////
#ifndef __RHSB_REGIF_H__
#define __RHSB_REGIF_H__
#include <string>
#include <map>
#include <list>
#include <cstdarg>
#include <cerrno>
#include <iomanip>
#include <sstream>
#ifdef REGIF_NOT_USE_SYSTEMC
#include <cassert>
#else
#include "systemc.h"
#endif
#include "re_register.h"
#ifdef CWR_SYSTEMC
#include "scml.h"
#endif

/// Register IF class of RHSB model
class Crhsb_regif
: public vpcl::reg_super
{
protected:
    typedef const unsigned int cuint;
    typedef unsigned int uint;

    enum eRegGroup {
        emNum_of_gr
    }; ///< Enumeration for register group index
    enum eRegIndex {
        emNUM_RHSBDEBA = 4,
        emNUM_RHSBUD   = 2
    }; ///< Enumeration for register factor index

    struct RegCBstr {
        uint channel;
        bool is_wr;
        uint size;
        uint pre_data;
        uint data;
        RegCBstr (uint channel, bool is_wr, uint size, uint pre_data, uint data)
        {
            this-> channel = channel;
            this-> is_wr   = is_wr;
            this-> size    = size;
            this-> pre_data= pre_data;
            this-> data    = data;
        }
    };
    struct SRegList {
        vpcl::re_register *my_p;
        SRegList *prev;
        uint channel;
        uint length;
        std::string wacc_size;
        std::string racc_size;
        bool block;
        SRegList( vpcl::re_register *_my_p, SRegList *_prev, uint _channel, uint _length, std::string _wacc_size, std::string _racc_size, bool _block = false) {
            this->my_p = NULL;
            if (_my_p != NULL) {
                this->my_p = _my_p;
            }
            this->prev = NULL;
            if (_prev != NULL) {
                this->prev = _prev;
            }
            this->channel = _channel;
            this->length  = _length;
            this->wacc_size = _wacc_size;
            this->racc_size = _racc_size;
            this->block = _block;
        }
    } *mRegList, *mCurReg, **mRegArray;

    uint *mRegMap;
    uint mTotalRegNum;

public:
    Crhsb_regif(std::string name, uint buswidth);
    ~Crhsb_regif();

protected:
    vpcl::re_register *RHSBGC  ;
    vpcl::re_register *RHSBMSR ;
    vpcl::re_register *RHSBDCR ;
    vpcl::re_register *RHSBDEC ;
    vpcl::re_register *RHSBSDC ;
    vpcl::re_register *RHSBDEBA[4];
    vpcl::re_register *RHSBEBE0;
    vpcl::re_register *RHSBEBE1;
    vpcl::re_register *RHSBDTC ;
    vpcl::re_register *RHSBDCD ;
    vpcl::re_register *RHSBDDR0;
    vpcl::re_register *RHSBDDR1;
    vpcl::re_register *RHSBDED0;
    vpcl::re_register *RHSBDED1;
    vpcl::re_register *RHSBUCR ;
    vpcl::re_register *RHSBUCC ;
    vpcl::re_register *RHSBUCS ;
    vpcl::re_register *RHSBUDR ;
    vpcl::re_register *RHSBUD  [2];
    vpcl::re_register *RHSBUSS ;
    vpcl::re_register *RHSBIC  ;
    vpcl::re_register *RHSBIS  ;
    vpcl::re_register *RHSBDCR1;

    #ifdef CWR_SYSTEMC
    scml2::memory<uint> cwmem;
    scml2::reg<uint> *CW_RHSBGC  ;
    scml2::reg<uint> *CW_RHSBMSR ;
    scml2::reg<uint> *CW_RHSBDCR ;
    scml2::reg<uint> *CW_RHSBDEC ;
    scml2::reg<uint> *CW_RHSBSDC ;
    scml2::reg<uint> *CW_RHSBDEBA[4];
    scml2::reg<uint> *CW_RHSBEBE0;
    scml2::reg<uint> *CW_RHSBEBE1;
    scml2::reg<uint> *CW_RHSBDTC ;
    scml2::reg<uint> *CW_RHSBDCD ;
    scml2::reg<uint> *CW_RHSBDDR0;
    scml2::reg<uint> *CW_RHSBDDR1;
    scml2::reg<uint> *CW_RHSBDED0;
    scml2::reg<uint> *CW_RHSBDED1;
    scml2::reg<uint> *CW_RHSBUCR ;
    scml2::reg<uint> *CW_RHSBUCC ;
    scml2::reg<uint> *CW_RHSBUCS ;
    scml2::reg<uint> *CW_RHSBUDR ;
    scml2::reg<uint> *CW_RHSBUD  [2];
    scml2::reg<uint> *CW_RHSBUSS ;
    scml2::reg<uint> *CW_RHSBIC  ;
    scml2::reg<uint> *CW_RHSBIS  ;
    scml2::reg<uint> *CW_RHSBDCR1;
    #endif

    uint RHSBGC_OPS ;
    uint RHSBGC_UDE ;
    uint RHSBGC_DDE ;
    uint RHSBGC_DCDE;
    uint RHSBMSR_UFA;
    uint RHSBMSR_TPS;
    uint RHSBMSR_DFA;
    uint RHSBDCR_DMS;
    uint RHSBDCR_EIP;
    uint RHSBDCR_EIM;
    uint RHSBDCR_EE ;
    uint RHSBDCR_CAC;
    uint RHSBDCR_CLP;
    uint RHSBDCR_REP;
    uint RHSBDCR_CTD;
    uint RHSBDCR_CIM;
    uint RHSBDCR_DBR;
    uint RHSBDCR_DFP;
    uint RHSBDCR_SLS;
    uint RHSBDEC_NDB3;
    uint RHSBDEC_SSD3;
    uint RHSBDEC_NDB2;
    uint RHSBDEC_SSD2;
    uint RHSBDEC_NDB1;
    uint RHSBDEC_SSD1;
    uint RHSBDEC_NDB0;
    uint RHSBDEC_SSD0;
    uint RHSBSDC_DPL1;
    uint RHSBSDC_CPS1;
    uint RHSBSDC_SOLP1;
    uint RHSBSDC_CSLP1;
    uint RHSBSDC_APL1;
    uint RHSBSDC_DPL0;
    uint RHSBSDC_CPS0;
    uint RHSBSDC_SOLP0;
    uint RHSBSDC_CSLP0;
    uint RHSBSDC_APL0;
    uint RHSBDEBA_DSS0[4];
    uint RHSBDEBA_DSS1[4];
    uint RHSBDEBA_DSS2[4];
    uint RHSBDEBA_DSS3[4];
    uint RHSBDEBA_DSS4[4];
    uint RHSBDEBA_DSS5[4];
    uint RHSBDEBA_DSS6[4];
    uint RHSBDEBA_DSS7[4];
    uint RHSBDEBA_DSS8[4];
    uint RHSBDEBA_DSS9[4];
    uint RHSBDEBA_DSS10[4];
    uint RHSBDEBA_DSS11[4];
    uint RHSBDEBA_DSS12[4];
    uint RHSBDEBA_DSS13[4];
    uint RHSBDEBA_DSS14[4];
    uint RHSBDEBA_DSS15[4];
    uint RHSBEBE0_EBE23_16;
    uint RHSBEBE0_EBE31_24;
    uint RHSBEBE0_EBE7_0;
    uint RHSBEBE0_EBE15_8;
    uint RHSBEBE1_EBE55_48;
    uint RHSBEBE1_EBE63_56;
    uint RHSBEBE1_EBE39_32;
    uint RHSBEBE1_EBE47_40;
    uint RHSBDTC_DTE;
    uint RHSBDTC_TSR;
    uint RHSBDTC_CTR;
    uint RHSBDTC_SSCF;
    uint RHSBDTC_NCB;
    uint RHSBDCD_CB ;
    uint RHSBDDR0_DB31_16;
    uint RHSBDDR0_DB15_0;
    uint RHSBDDR1_DB63_48;
    uint RHSBDDR1_DB47_32;
    uint RHSBDED0_EB31_16;
    uint RHSBDED0_EB15_0;
    uint RHSBDED1_EB63_48;
    uint RHSBDED1_EB47_32;
    uint RHSBUCR_FSM;
    uint RHSBUCR_UMS;
    uint RHSBUCR_TOE;
    uint RHSBUCR_UE ;
    uint RHSBUCR_RTO;
    uint RHSBUCC_Reserved;
    uint RHSBUCC_UBR1;
    uint RHSBUCC_PC1;
    uint RHSBUCC_ILP1;
    uint RHSBUCC_SBN1;
    uint RHSBUCC_UFT1;
    uint RHSBUCC_UBR0;
    uint RHSBUCC_PC0;
    uint RHSBUCC_ILP0;
    uint RHSBUCC_SBN0;
    uint RHSBUCC_UFT0;
    uint RHSBUCS_ACC;
    uint RHSBUCS_BSY;
    uint RHSBUDR_DF ;
    uint RHSBUDR_EDF;
    uint RHSBUDR_ND ;
    uint RHSBUDR_PERR;
    uint RHSBUDR_FERR;
    uint RHSBUDR_TO ;
    uint RHSBUDR_DL ;
    uint RHSBUDR_LUD;
    uint RHSBUDR_NDS1;
    uint RHSBUDR_NDS0;
    uint RHSBUD_DF  [2];
    uint RHSBUD_EDF [2];
    uint RHSBUD_ND  [2];
    uint RHSBUD_PERR[2];
    uint RHSBUD_FERR[2];
    uint RHSBUD_TO  [2];
    uint RHSBUD_DL  [2];
    uint RHSBUSS_ND1;
    uint RHSBUSS_PERR1;
    uint RHSBUSS_FERR1;
    uint RHSBUSS_TO1;
    uint RHSBUSS_DL1;
    uint RHSBUSS_ND0;
    uint RHSBUSS_PERR0;
    uint RHSBUSS_FERR0;
    uint RHSBUSS_TO0;
    uint RHSBUSS_DL0;
    uint RHSBIC_DTSIE;
    uint RHSBIC_DTIE;
    uint RHSBIC_CTIE;
    uint RHSBIC_ETIE;
    uint RHSBIC_TSIE;
    uint RHSBIC_DRIE;
    uint RHSBIC_UEIE;
    uint RHSBIC_TOIE;
    uint RHSBIC_DLIE;
    uint RHSBIS_ERF ;
    uint RHSBIS_DTSF;
    uint RHSBIS_DTF ;
    uint RHSBIS_CTF ;
    uint RHSBIS_ETF ;
    uint RHSBIS_TSF ;
    uint RHSBIS_DRF ;
    uint RHSBIS_UEF ;
    uint RHSBIS_TOF ;
    uint RHSBIS_DLF ;
    uint RHSBDCR1_SLS1;

    void EnableReset(const bool is_active);
    uint bit_select(cuint val, cuint start, cuint end);
    bool reg_wr(cuint addr, const unsigned char *p_data, cuint size);
    bool reg_rd(cuint addr, unsigned char *p_data, cuint size);
    bool reg_wr_dbg(cuint addr, const unsigned char *p_data, cuint size);
    bool reg_rd_dbg(cuint addr, unsigned char *p_data, cuint size);
    std::string reg_handle_command(const std::vector<std::string>& args);
    int get_reg_index(cuint addr);
    vpcl::re_register *first_reg_object();
    vpcl::re_register *next_reg_object();

    void wr_cb(cuint addr, uint data);
    uint rd_cb(cuint addr);

    virtual void cb_RHSBGC_OPS(RegCBstr str) = 0;
    virtual void cb_RHSBDCR_DMS(RegCBstr str) = 0;
    virtual void cb_RHSBDEC_NDB3(RegCBstr str) = 0;
    virtual void cb_RHSBSDC_DPL1(RegCBstr str) = 0;
    virtual void cb_RHSBDEBA_DSS0(RegCBstr str) = 0;
    virtual void cb_RHSBEBE0_EBE23_16(RegCBstr str) = 0;
    virtual void cb_RHSBEBE1_EBE55_48(RegCBstr str) = 0;
    virtual void cb_RHSBDTC_DTE(RegCBstr str) = 0;
    virtual void cb_RHSBDCD_CB(RegCBstr str) = 0;
    virtual void cb_RHSBDDR0_DB31_16(RegCBstr str) = 0;
    virtual void cb_RHSBDDR1_DB63_48(RegCBstr str) = 0;
    virtual void cb_RHSBDED0_EB31_16(RegCBstr str) = 0;
    virtual void cb_RHSBDED1_EB63_48(RegCBstr str) = 0;
    virtual void cb_RHSBUCR_FSM(RegCBstr str) = 0;
    virtual void cb_RHSBUCC_UBR1(RegCBstr str) = 0;
    virtual void cb_RHSBUCS_ACC(RegCBstr str) = 0;
    virtual void cb_RHSBUDR_DF(RegCBstr str) = 0;
    virtual void cb_RHSBIC_DTSIE(RegCBstr str) = 0;
    virtual void cb_RHSBIS_ERF(RegCBstr str) = 0;
    virtual void cb_RHSBDCR1_SLS1(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Crhsb_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Crhsb_regif::*) (RegCBstr)> > mRdCbAPI;
    std::string mFileName;
    std::string mInstName;
    int mLineNum;

    bool reg_wr_process(cuint addr, const unsigned char *p_data, cuint size, bool IsDbgFunc);
    bool reg_rd_process(cuint addr, unsigned char *p_data, cuint size, bool IsDbgFunc);
    bool reg_wr_func(cuint addr, const unsigned char *p_data, cuint size, cuint reg_index, bool IsDbgFunc);
    bool reg_rd_func(cuint addr, unsigned char *p_data, cuint size, cuint reg_index, bool IsDbgFunc);
    void _re_printf(const std::string msg_level, const char *format, ...);
    void get_fileline(std::string filename, int line_number);
    void CommandInit();
    bool ChkSize(std::string expect_size, cuint detect_size, cuint addr);
    std::vector<std::string> Str2Vec(std::string str, const char sep);
    std::string Num2HexStr(cuint num, cuint num_length, bool space_strip);
    void DumpRegMsg(const std::string operation, const std::string RegName, const std::string BitName, cuint size, cuint addr, cuint wr_data, cuint pre_data, cuint data, cuint reg_length);
    std::string AccessRegCommand(const std::vector<std::string>& args, vpcl::re_register *Register, bool &BlockReg);
    void InitLocalVal();
    void UpdateLocalVal(cuint addr);
    void UpdateRegVal(cuint addr);

    #ifdef CWR_SYSTEMC
    uint cw_rd_cb(tlm::tlm_generic_payload& trans, int tag);
    uint cw_wr_cb(tlm::tlm_generic_payload& trans, int tag);
    void cw_instantiate_reg(scml2::reg<uint> * reg, std::string reg_name, int offset);
    #endif
};
#endif //__RHSB_REG_DEF_H__