// -----------------------------------------------------------------------------
// $Id: ipg_regif.h,v 1.3 2015/06/12 08:19:09 dungvannguyen Exp $
//
// Copyright(c) 2014-2015 Renesas Electronics Corporation
// Copyright(c) 2014-2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------
// This file is generated by Register I/F generator
//    gen_regif.py 1.9 2013/12/04 09:51:36 sontran
//    gen_regif_class.py 1.59 2014/05/19 02:28:17 sontran
//    regif_h.skl 1.29 2014/05/09 02:57:13 sontran
//
// Input file : ipg_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE IPG
//     #                   name            offset_size
//     %%REG_INSTANCE      reg_def         10
// 
// %REG_CHANNEL reg_def
//     %%TITLE  group  name       reg_name   wsize  rsize    length  offset factor_start  factor_end  factor_index  factor_step  access  init  support  callback
//     %%REG    -      IPGECRUM   IPGECRUM   16     8|16|32  16      0x002  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGADRUM   IPGADRUM   32     8|16|32  32      0x008  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGENUM    IPGENUM    8      8|16|32  8       0x00D  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGPMTUM   IPGPMTUM   8      8|16|32  8       0x020  0             4           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGECRSV0  IPGECRSV0  16     8|16|32  16      0x040  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGECRSV1  IPGECRSV1  16     8|16|32  16      0x080  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGECRSV2  IPGECRSV2  16     8|16|32  16      0x0C0  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGECRSV3  IPGECRSV3  16     8|16|32  16      0x100  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGECRUM0  IPGECRUM0  16     8|16|32  16      0x042  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGECRUM1  IPGECRUM1  16     8|16|32  16      0x082  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGECRUM2  IPGECRUM2  16     8|16|32  16      0x0C2  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGECRUM3  IPGECRUM3  16     8|16|32  16      0x102  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGADRSV0  IPGADRSV0  32     8|16|32  32      0x044  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGADRSV1  IPGADRSV1  32     8|16|32  32      0x084  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGADRSV2  IPGADRSV2  32     8|16|32  32      0x0C4  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGADRSV3  IPGADRSV3  32     8|16|32  32      0x104  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGADRUM0  IPGADRUM0  32     8|16|32  32      0x048  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGADRUM1  IPGADRUM1  32     8|16|32  32      0x088  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGADRUM2  IPGADRUM2  32     8|16|32  32      0x0C8  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGADRUM3  IPGADRUM3  32     8|16|32  32      0x108  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGENSV0   IPGENSV0   8      8|16|32  8       0x04C  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGENSV1   IPGENSV1   8      8|16|32  8       0x08C  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGENSV2   IPGENSV2   8      8|16|32  8       0x0CC  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGENSV3   IPGENSV3   8      8|16|32  8       0x10C  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGENUM0   IPGENUM0   8      8|16|32  8       0x04D  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGENUM1   IPGENUM1   8      8|16|32  8       0x08D  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGENUM2   IPGENUM2   8      8|16|32  8       0x0CD  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGENUM3   IPGENUM3   8      8|16|32  8       0x10D  -             -           -             -            W|R     0     TRUE     -  
//     %%REG    -      IPGPMTSV0  IPGPMTSV0  8      8|16|32  8       0x050  0             3           -             0x40         W|R     0     TRUE     -  
//     %%REG    -      IPGPMTSV1  IPGPMTSV1  8      8|16|32  8       0x051  0             3           -             0x40         W|R     0     TRUE     -  
//     %%REG    -      IPGPMTSV2  IPGPMTSV2  8      8|16|32  8       0x052  0             3           -             0x40         W|R     0     TRUE     -  
//     %%REG    -      IPGPMTSV3  IPGPMTSV3  8      8|16|32  8       0x053  0             3           -             0x40         W|R     0     TRUE     -  
//     %%REG    -      IPGPMTSV4  IPGPMTSV4  8      8|16|32  8       0x054  0             3           -             0x40         W|R     0     TRUE     -  
//     %%REG    -      IPGPMTUM0  IPGPMTUM0  8      8|16|32  8       0x060  0             3           -             0x40         W|R     0     TRUE     -  
//     %%REG    -      IPGPMTUM1  IPGPMTUM1  8      8|16|32  8       0x061  0             3           -             0x40         W|R     0     TRUE     -  
//     %%REG    -      IPGPMTUM2  IPGPMTUM2  8      8|16|32  8       0x062  0             3           -             0x40         W|R     0     TRUE     -  
//     %%REG    -      IPGPMTUM3  IPGPMTUM3  8      8|16|32  8       0x063  0             3           -             0x40         W|R     0     TRUE     -  
//     %%REG    -      IPGPMTUM4  IPGPMTUM4  8      8|16|32  8       0x064  0             3           -             0x40         W|R     0     TRUE     -  
// 
// %REG_NAME IPGECRUM
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    VD          0      0      0     W|R     TRUE     -
//     %%BIT    RD          1      1      0     W|R     TRUE     -
//     %%BIT    WR          2      2      0     W|R     TRUE     -
//     %%BIT    EX          3      3      0     W|R     TRUE     -
//     %%BIT    DS          7      4      0     W|R     TRUE     -
//     %%BIT    TCID        13     8      0     W|R     TRUE     -
// 
// %REG_NAME IPGADRUM
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    EADR        31     0      0     W|R     TRUE     -
// 
// %REG_NAME IPGENUM
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    E           0      0      0     W|R     TRUE     -
//     %%BIT    IRE         1      1      0     W|R     TRUE     -
// 
// %REG_NAME IPGPMTUM
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    R0          0      0      0     W|R     TRUE     -
//     %%BIT    W0          1      1      0     W|R     TRUE     -
//     %%BIT    X0          2      2      0     W|R     TRUE     -
//     %%BIT    R1          4      4      0     W|R     TRUE     -
//     %%BIT    W1          5      5      0     W|R     TRUE     -
//     %%BIT    X1          6      6      0     W|R     TRUE     -
// 
// %REG_NAME IPGECRSV0
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    VD          0      0      0     W|R     TRUE     -
//     %%BIT    RD          1      1      0     W|R     TRUE     -
//     %%BIT    WR          2      2      0     W|R     TRUE     -
//     %%BIT    EX          3      3      0     W|R     TRUE     -
//     %%BIT    DS          7      4      0     W|R     TRUE     -
//     %%BIT    TCID        13     8      0     W|R     TRUE     -
// 
// %REG_NAME IPGECRSV1
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    VD          0      0      0     W|R     TRUE     -
//     %%BIT    RD          1      1      0     W|R     TRUE     -
//     %%BIT    WR          2      2      0     W|R     TRUE     -
//     %%BIT    EX          3      3      0     W|R     TRUE     -
//     %%BIT    DS          7      4      0     W|R     TRUE     -
//     %%BIT    TCID        13     8      0     W|R     TRUE     -
// 
// %REG_NAME IPGECRSV2
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    VD          0      0      0     W|R     TRUE     -
//     %%BIT    RD          1      1      0     W|R     TRUE     -
//     %%BIT    WR          2      2      0     W|R     TRUE     -
//     %%BIT    EX          3      3      0     W|R     TRUE     -
//     %%BIT    DS          7      4      0     W|R     TRUE     -
//     %%BIT    TCID        13     8      0     W|R     TRUE     -
// 
// %REG_NAME IPGECRSV3
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    VD          0      0      0     W|R     TRUE     -
//     %%BIT    RD          1      1      0     W|R     TRUE     -
//     %%BIT    WR          2      2      0     W|R     TRUE     -
//     %%BIT    EX          3      3      0     W|R     TRUE     -
//     %%BIT    DS          7      4      0     W|R     TRUE     -
//     %%BIT    TCID        13     8      0     W|R     TRUE     -
// 
// %REG_NAME IPGECRUM0
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    VD          0      0      0     W|R     TRUE     -
//     %%BIT    RD          1      1      0     W|R     TRUE     -
//     %%BIT    WR          2      2      0     W|R     TRUE     -
//     %%BIT    EX          3      3      0     W|R     TRUE     -
//     %%BIT    DS          7      4      0     W|R     TRUE     -
//     %%BIT    TCID        13     8      0     W|R     TRUE     -
// 
// %REG_NAME IPGECRUM1
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    VD          0      0      0     W|R     TRUE     -
//     %%BIT    RD          1      1      0     W|R     TRUE     -
//     %%BIT    WR          2      2      0     W|R     TRUE     -
//     %%BIT    EX          3      3      0     W|R     TRUE     -
//     %%BIT    DS          7      4      0     W|R     TRUE     -
//     %%BIT    TCID        13     8      0     W|R     TRUE     -
// 
// %REG_NAME IPGECRUM2
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    VD          0      0      0     W|R     TRUE     -
//     %%BIT    RD          1      1      0     W|R     TRUE     -
//     %%BIT    WR          2      2      0     W|R     TRUE     -
//     %%BIT    EX          3      3      0     W|R     TRUE     -
//     %%BIT    DS          7      4      0     W|R     TRUE     -
//     %%BIT    TCID        13     8      0     W|R     TRUE     -
// 
// %REG_NAME IPGECRUM3
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    VD          0      0      0     W|R     TRUE     -
//     %%BIT    RD          1      1      0     W|R     TRUE     -
//     %%BIT    WR          2      2      0     W|R     TRUE     -
//     %%BIT    EX          3      3      0     W|R     TRUE     -
//     %%BIT    DS          7      4      0     W|R     TRUE     -
//     %%BIT    TCID        13     8      0     W|R     TRUE     -
// 
// %REG_NAME IPGADRSV0
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    EADR        31     0      0     W|R     TRUE     -
// 
// %REG_NAME IPGADRSV1
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    EADR        31     0      0     W|R     TRUE     -
// 
// %REG_NAME IPGADRSV2
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    EADR        31     0      0     W|R     TRUE     -
// 
// %REG_NAME IPGADRSV3
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    EADR        31     0      0     W|R     TRUE     -
// 
// %REG_NAME IPGADRUM0
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    EADR        31     0      0     W|R     TRUE     -
// 
// %REG_NAME IPGADRUM1
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    EADR        31     0      0     W|R     TRUE     -
// 
// %REG_NAME IPGADRUM2
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    EADR        31     0      0     W|R     TRUE     -
// 
// %REG_NAME IPGADRUM3
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    EADR        31     0      0     W|R     TRUE     -
// 
// %REG_NAME IPGENSV0
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    E           0      0      0     W|R     TRUE     -
//     %%BIT    IRE         1      1      0     W|R     TRUE     -
// 
// %REG_NAME IPGENSV1
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    E           0      0      0     W|R     TRUE     -
//     %%BIT    IRE         1      1      0     W|R     TRUE     -
// 
// %REG_NAME IPGENSV2
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    E           0      0      0     W|R     TRUE     -
//     %%BIT    IRE         1      1      0     W|R     TRUE     -
// 
// %REG_NAME IPGENSV3
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    E           0      0      0     W|R     TRUE     -
//     %%BIT    IRE         1      1      0     W|R     TRUE     -
// 
// %REG_NAME IPGENUM0
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    E           0      0      0     W|R     TRUE     -
//     %%BIT    IRE         1      1      0     W|R     TRUE     -
// 
// %REG_NAME IPGENUM1
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    E           0      0      0     W|R     TRUE     -
//     %%BIT    IRE         1      1      0     W|R     TRUE     -
// 
// %REG_NAME IPGENUM2
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    E           0      0      0     W|R     TRUE     -
//     %%BIT    IRE         1      1      0     W|R     TRUE     -
// 
// %REG_NAME IPGENUM3
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    E           0      0      0     W|R     TRUE     -
//     %%BIT    IRE         1      1      0     W|R     TRUE     -
// 
// %REG_NAME IPGPMTSV0
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    R0          0      0      0     W|R     TRUE     -
//     %%BIT    W0          1      1      0     W|R     TRUE     -
//     %%BIT    X0          2      2      0     W|R     TRUE     -
//     %%BIT    R1          4      4      0     W|R     TRUE     -
//     %%BIT    W1          5      5      0     W|R     TRUE     -
//     %%BIT    X1          6      6      0     W|R     TRUE     -
// 
// %REG_NAME IPGPMTSV1
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    X0          2      2      0     W|R     TRUE     -
//     %%BIT    X1          6      6      0     W|R     TRUE     -
// 
// %REG_NAME IPGPMTSV2
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    R0          0      0      0     W|R     TRUE     -
//     %%BIT    W0          1      1      0     W|R     TRUE     -
//     %%BIT    X0          2      2      0     W|R     TRUE     -
//     %%BIT    R1          4      4      0     W|R     TRUE     -
//     %%BIT    W1          5      5      0     W|R     TRUE     -
//     %%BIT    X1          6      6      0     W|R     TRUE     -
// 
// %REG_NAME IPGPMTSV3
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    R0          0      0      0     W|R     TRUE     -
//     %%BIT    W0          1      1      0     W|R     TRUE     -
//     %%BIT    R1          4      4      0     W|R     TRUE     -
//     %%BIT    W1          5      5      0     W|R     TRUE     -
//     %%BIT    X1          6      6      0     W|R     TRUE     -
// 
// %REG_NAME IPGPMTSV4
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    R0          0      0      0     W|R     TRUE     -
//     %%BIT    W0          1      1      0     W|R     TRUE     -
//     %%BIT    R1          4      4      0     W|R     TRUE     -
//     %%BIT    W1          5      5      0     W|R     TRUE     -
//     %%BIT    X1          6      6      0     W|R     TRUE     -
// 
// %REG_NAME IPGPMTUM0
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    R0          0      0      0     W|R     TRUE     -
//     %%BIT    W0          1      1      0     W|R     TRUE     -
//     %%BIT    X0          2      2      0     W|R     TRUE     -
//     %%BIT    R1          4      4      0     W|R     TRUE     -
//     %%BIT    W1          5      5      0     W|R     TRUE     -
//     %%BIT    X1          6      6      0     W|R     TRUE     -
// 
// %REG_NAME IPGPMTUM1
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    X0          2      2      0     W|R     TRUE     -
//     %%BIT    X1          6      6      0     W|R     TRUE     -
// 
// %REG_NAME IPGPMTUM2
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    R0          0      0      0     W|R     TRUE     -
//     %%BIT    W0          1      1      0     W|R     TRUE     -
//     %%BIT    X0          2      2      0     W|R     TRUE     -
//     %%BIT    R1          4      4      0     W|R     TRUE     -
//     %%BIT    W1          5      5      0     W|R     TRUE     -
//     %%BIT    X1          6      6      0     W|R     TRUE     -
// 
// %REG_NAME IPGPMTUM3
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    R0          0      0      0     W|R     TRUE     -
//     %%BIT    W0          1      1      0     W|R     TRUE     -
//     %%BIT    R1          4      4      0     W|R     TRUE     -
//     %%BIT    W1          5      5      0     W|R     TRUE     -
//     %%BIT    X1          6      6      0     W|R     TRUE     -
// 
// %REG_NAME IPGPMTUM4
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    R0          0      0      0     W|R     TRUE     -
//     %%BIT    W0          1      1      0     W|R     TRUE     -
//     %%BIT    R1          4      4      0     W|R     TRUE     -
//     %%BIT    W1          5      5      0     W|R     TRUE     -
//     %%BIT    X1          6      6      0     W|R     TRUE     -
////////////////////////////////////////////////////////////////////////////////
/// @file ipg_regif.h
/// @brief Register IF class of model IPG
/// $Id: ipg_regif.h,v 1.3 2015/06/12 08:19:09 dungvannguyen Exp $
/// $Date: 2015/06/12 08:19:09 $
/// $Revison$
/// $Author: dungvannguyen $
////////////////////////////////////////////////////////////////////////////////
#ifndef __IPG_REGIF_H__
#define __IPG_REGIF_H__
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

/// Register IF class of IPG model
class Cipg_regif
: public vpcl::reg_super
{
protected:
    typedef const unsigned int cuint;
    typedef unsigned int uint;

    enum eRegGroup {
        emNum_of_gr
    }; ///< Enumeration for register group index
    enum eRegIndex {
        emNUM_IPGPMTUM  = 5,
        emNUM_IPGPMTSV0 = 4,
        emNUM_IPGPMTSV1 = 4,
        emNUM_IPGPMTSV2 = 4,
        emNUM_IPGPMTSV3 = 4,
        emNUM_IPGPMTSV4 = 4,
        emNUM_IPGPMTUM0 = 4,
        emNUM_IPGPMTUM1 = 4,
        emNUM_IPGPMTUM2 = 4,
        emNUM_IPGPMTUM3 = 4,
        emNUM_IPGPMTUM4 = 4
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
    Cipg_regif(std::string name, uint buswidth, std::string ipg_version = "G3M_V30");
    ~Cipg_regif();

protected:
    vpcl::re_register *IPGECRUM ;
    vpcl::re_register *IPGADRUM ;
    vpcl::re_register *IPGENUM  ;
    vpcl::re_register *IPGPMTUM [5];
    vpcl::re_register *IPGECRSV0;
    vpcl::re_register *IPGECRSV1;
    vpcl::re_register *IPGECRSV2;
    vpcl::re_register *IPGECRSV3;
    vpcl::re_register *IPGECRUM0;
    vpcl::re_register *IPGECRUM1;
    vpcl::re_register *IPGECRUM2;
    vpcl::re_register *IPGECRUM3;
    vpcl::re_register *IPGADRSV0;
    vpcl::re_register *IPGADRSV1;
    vpcl::re_register *IPGADRSV2;
    vpcl::re_register *IPGADRSV3;
    vpcl::re_register *IPGADRUM0;
    vpcl::re_register *IPGADRUM1;
    vpcl::re_register *IPGADRUM2;
    vpcl::re_register *IPGADRUM3;
    vpcl::re_register *IPGENSV0 ;
    vpcl::re_register *IPGENSV1 ;
    vpcl::re_register *IPGENSV2 ;
    vpcl::re_register *IPGENSV3 ;
    vpcl::re_register *IPGENUM0 ;
    vpcl::re_register *IPGENUM1 ;
    vpcl::re_register *IPGENUM2 ;
    vpcl::re_register *IPGENUM3 ;
    vpcl::re_register *IPGPMTSV0[4];
    vpcl::re_register *IPGPMTSV1[4];
    vpcl::re_register *IPGPMTSV2[4];
    vpcl::re_register *IPGPMTSV3[4];
    vpcl::re_register *IPGPMTSV4[4];
    vpcl::re_register *IPGPMTUM0[4];
    vpcl::re_register *IPGPMTUM1[4];
    vpcl::re_register *IPGPMTUM2[4];
    vpcl::re_register *IPGPMTUM3[4];
    vpcl::re_register *IPGPMTUM4[4];

    #ifdef CWR_SYSTEMC
    scml2::memory<uint> cwmem;
    scml2::reg<uint>      *CW_IPGECRUM_;
    scml2::reg<uint>      *CW_IPGENUM_ ;
    scml2::reg<uint>      *CW_IPGPMTUM_0_IPGPMTUM_1_IPGPMTUM_2_IPGPMTUM_3;
    scml2::reg<uint>      *CW_IPGPMTUM_[5];
    scml2::reg<uint>      *CW_IPGECRSV0_IPGECRUM0;
    scml2::reg<uint>      *CW_IPGENSV0_IPGENUM0;
    scml2::reg<uint>      *CW_IPGPMTSV0_0_IPGPMTSV1_0_IPGPMTSV2_0_IPGPMTSV3_0;
    scml2::reg<uint>      *CW_IPGPMTSV4_[4];
    scml2::reg<uint>      *CW_IPGPMTUM0_0_IPGPMTUM1_0_IPGPMTUM2_0_IPGPMTUM3_0;
    scml2::reg<uint>      *CW_IPGPMTUM4_[4];
    scml2::reg<uint>      *CW_IPGECRSV1_IPGECRUM1;
    scml2::reg<uint>      *CW_IPGENSV1_IPGENUM1;
    scml2::reg<uint>      *CW_IPGPMTSV0_1_IPGPMTSV1_1_IPGPMTSV2_1_IPGPMTSV3_1;
    scml2::reg<uint>      *CW_IPGPMTUM0_1_IPGPMTUM1_1_IPGPMTUM2_1_IPGPMTUM3_1;
    scml2::reg<uint>      *CW_IPGECRSV2_IPGECRUM2;
    scml2::reg<uint>      *CW_IPGENSV2_IPGENUM2;
    scml2::reg<uint>      *CW_IPGPMTSV0_2_IPGPMTSV1_2_IPGPMTSV2_2_IPGPMTSV3_2;
    scml2::reg<uint>      *CW_IPGPMTUM0_2_IPGPMTUM1_2_IPGPMTUM2_2_IPGPMTUM3_2;
    scml2::reg<uint>      *CW_IPGECRSV3_IPGECRUM3;
    scml2::reg<uint>      *CW_IPGENSV3_IPGENUM3;
    scml2::reg<uint>      *CW_IPGPMTSV0_3_IPGPMTSV1_3_IPGPMTSV2_3_IPGPMTSV3_3;
    scml2::reg<uint>      *CW_IPGPMTUM0_3_IPGPMTUM1_3_IPGPMTUM2_3_IPGPMTUM3_3;
    scml2::bitfield<uint> *CW_IPGECRUM ;
    scml2::reg<uint>      *CW_IPGADRUM ;
    scml2::bitfield<uint> *CW_IPGENUM  ;
    scml2::bitfield<uint> *CW_IPGPMTUM [5];
    scml2::bitfield<uint> *CW_IPGECRSV0;
    scml2::bitfield<uint> *CW_IPGECRSV1;
    scml2::bitfield<uint> *CW_IPGECRSV2;
    scml2::bitfield<uint> *CW_IPGECRSV3;
    scml2::bitfield<uint> *CW_IPGECRUM0;
    scml2::bitfield<uint> *CW_IPGECRUM1;
    scml2::bitfield<uint> *CW_IPGECRUM2;
    scml2::bitfield<uint> *CW_IPGECRUM3;
    scml2::reg<uint>      *CW_IPGADRSV0;
    scml2::reg<uint>      *CW_IPGADRSV1;
    scml2::reg<uint>      *CW_IPGADRSV2;
    scml2::reg<uint>      *CW_IPGADRSV3;
    scml2::reg<uint>      *CW_IPGADRUM0;
    scml2::reg<uint>      *CW_IPGADRUM1;
    scml2::reg<uint>      *CW_IPGADRUM2;
    scml2::reg<uint>      *CW_IPGADRUM3;
    scml2::bitfield<uint> *CW_IPGENSV0 ;
    scml2::bitfield<uint> *CW_IPGENSV1 ;
    scml2::bitfield<uint> *CW_IPGENSV2 ;
    scml2::bitfield<uint> *CW_IPGENSV3 ;
    scml2::bitfield<uint> *CW_IPGENUM0 ;
    scml2::bitfield<uint> *CW_IPGENUM1 ;
    scml2::bitfield<uint> *CW_IPGENUM2 ;
    scml2::bitfield<uint> *CW_IPGENUM3 ;
    scml2::bitfield<uint> *CW_IPGPMTSV0[4];
    scml2::bitfield<uint> *CW_IPGPMTSV1[4];
    scml2::bitfield<uint> *CW_IPGPMTSV2[4];
    scml2::bitfield<uint> *CW_IPGPMTSV3[4];
    scml2::bitfield<uint> *CW_IPGPMTSV4[4];
    scml2::bitfield<uint> *CW_IPGPMTUM0[4];
    scml2::bitfield<uint> *CW_IPGPMTUM1[4];
    scml2::bitfield<uint> *CW_IPGPMTUM2[4];
    scml2::bitfield<uint> *CW_IPGPMTUM3[4];
    scml2::bitfield<uint> *CW_IPGPMTUM4[4];
    #endif

    uint IPGECRUM_VD ;
    uint IPGECRUM_RD ;
    uint IPGECRUM_WR ;
    uint IPGECRUM_EX ;
    uint IPGECRUM_DS ;
    uint IPGECRUM_BY ;
    uint IPGECRUM_HW ;
    uint IPGECRUM_WD ;
    uint IPGECRUM_TCID;
    uint IPGECRUM_PEID;
    uint IPGADRUM_EADR;
    uint IPGENUM_E   ;
    uint IPGENUM_IRE ;
    uint IPGPMTUM_R0 [5];
    uint IPGPMTUM_W0 [5];
    uint IPGPMTUM_X0 [5];
    uint IPGPMTUM_R1 [5];
    uint IPGPMTUM_W1 [5];
    uint IPGPMTUM_X1 [5];
    uint IPGECRSV0_VD;
    uint IPGECRSV0_RD;
    uint IPGECRSV0_WR;
    uint IPGECRSV0_EX;
    uint IPGECRSV0_DS;
    uint IPGECRSV0_TCID;
    uint IPGECRSV0_PEID;
    uint IPGECRSV1_VD;
    uint IPGECRSV1_RD;
    uint IPGECRSV1_WR;
    uint IPGECRSV1_EX;
    uint IPGECRSV1_DS;
    uint IPGECRSV1_TCID;
    uint IPGECRSV1_PEID;
    uint IPGECRSV2_VD;
    uint IPGECRSV2_RD;
    uint IPGECRSV2_WR;
    uint IPGECRSV2_EX;
    uint IPGECRSV2_DS;
    uint IPGECRSV2_TCID;
    uint IPGECRSV2_PEID;
    uint IPGECRSV3_VD;
    uint IPGECRSV3_RD;
    uint IPGECRSV3_WR;
    uint IPGECRSV3_EX;
    uint IPGECRSV3_DS;
    uint IPGECRSV3_TCID;
    uint IPGECRSV3_PEID;
    uint IPGECRUM0_VD;
    uint IPGECRUM0_RD;
    uint IPGECRUM0_WR;
    uint IPGECRUM0_EX;
    uint IPGECRUM0_DS;
    uint IPGECRUM0_TCID;
    uint IPGECRUM0_PEID;
    uint IPGECRUM1_VD;
    uint IPGECRUM1_RD;
    uint IPGECRUM1_WR;
    uint IPGECRUM1_EX;
    uint IPGECRUM1_DS;
    uint IPGECRUM1_TCID;
    uint IPGECRUM1_PEID;
    uint IPGECRUM2_VD;
    uint IPGECRUM2_RD;
    uint IPGECRUM2_WR;
    uint IPGECRUM2_EX;
    uint IPGECRUM2_DS;
    uint IPGECRUM2_TCID;
    uint IPGECRUM2_PEID;
    uint IPGECRUM3_VD;
    uint IPGECRUM3_RD;
    uint IPGECRUM3_WR;
    uint IPGECRUM3_EX;
    uint IPGECRUM3_DS;
    uint IPGECRUM3_TCID;
    uint IPGECRUM3_PEID;
    uint IPGADRSV0_EADR;
    uint IPGADRSV1_EADR;
    uint IPGADRSV2_EADR;
    uint IPGADRSV3_EADR;
    uint IPGADRUM0_EADR;
    uint IPGADRUM1_EADR;
    uint IPGADRUM2_EADR;
    uint IPGADRUM3_EADR;
    uint IPGENSV0_E  ;
    uint IPGENSV0_IRE;
    uint IPGENSV1_E  ;
    uint IPGENSV1_IRE;
    uint IPGENSV2_E  ;
    uint IPGENSV2_IRE;
    uint IPGENSV3_E  ;
    uint IPGENSV3_IRE;
    uint IPGENUM0_E  ;
    uint IPGENUM0_IRE;
    uint IPGENUM1_E  ;
    uint IPGENUM1_IRE;
    uint IPGENUM2_E  ;
    uint IPGENUM2_IRE;
    uint IPGENUM3_E  ;
    uint IPGENUM3_IRE;
    uint IPGPMTSV0_R0[4];
    uint IPGPMTSV0_W0[4];
    uint IPGPMTSV0_X0[4];
    uint IPGPMTSV0_R1[4];
    uint IPGPMTSV0_W1[4];
    uint IPGPMTSV0_X1[4];
    uint IPGPMTSV1_X0[4];
    uint IPGPMTSV1_X1[4];
    uint IPGPMTSV2_R0[4];
    uint IPGPMTSV2_W0[4];
    uint IPGPMTSV2_X0[4];
    uint IPGPMTSV2_R1[4];
    uint IPGPMTSV2_W1[4];
    uint IPGPMTSV2_X1[4];
    uint IPGPMTSV3_R0[4];
    uint IPGPMTSV3_W0[4];
    uint IPGPMTSV3_X0[4];
    uint IPGPMTSV3_R1[4];
    uint IPGPMTSV3_W1[4];
    uint IPGPMTSV3_X1[4];
    uint IPGPMTSV4_R0[4];
    uint IPGPMTSV4_W0[4];
    uint IPGPMTSV4_R1[4];
    uint IPGPMTSV4_W1[4];
    uint IPGPMTSV4_X1[4];
    uint IPGPMTUM0_R0[4];
    uint IPGPMTUM0_W0[4];
    uint IPGPMTUM0_X0[4];
    uint IPGPMTUM0_R1[4];
    uint IPGPMTUM0_W1[4];
    uint IPGPMTUM0_X1[4];
    uint IPGPMTUM1_X0[4];
    uint IPGPMTUM1_X1[4];
    uint IPGPMTUM2_R0[4];
    uint IPGPMTUM2_W0[4];
    uint IPGPMTUM2_X0[4];
    uint IPGPMTUM2_R1[4];
    uint IPGPMTUM2_W1[4];
    uint IPGPMTUM2_X1[4];
    uint IPGPMTUM3_R0[4];
    uint IPGPMTUM3_W0[4];
    uint IPGPMTUM3_X0[4];
    uint IPGPMTUM3_R1[4];
    uint IPGPMTUM3_W1[4];
    uint IPGPMTUM3_X1[4];
    uint IPGPMTUM4_R0[4];
    uint IPGPMTUM4_W0[4];
    uint IPGPMTUM4_R1[4];
    uint IPGPMTUM4_W1[4];
    uint IPGPMTUM4_X1[4];

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


    void set_instance_name(std::string InstName);

    std::string mVersion;
    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Cipg_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Cipg_regif::*) (RegCBstr)> > mRdCbAPI;
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
#endif //__IPG_REG_DEF_H__