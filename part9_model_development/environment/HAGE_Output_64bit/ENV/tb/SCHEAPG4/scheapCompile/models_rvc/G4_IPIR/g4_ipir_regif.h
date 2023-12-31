// -----------------------------------------------------------------------------
// $Id: g4_ipir_regif.h,v 1.2 2019/07/10 11:25:13 nhuvo Exp $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
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
// Input file : g4_ipir_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE G4_IPIR
// #               name    offset_size     offset_start    offset_skip     offset_times
// %%REG_INSTANCE  pe      12              0x00            0x100           8
// 
// %REG_CHANNEL pe
//   %%TITLE           name    offset  offset_skip offset_times    
//   %%REG_INSTANCE    ch      0x00    0x20        4               
// 
// %REG_CHANNEL ch
//   %%TITLE           name        rsize   wsize   length  offset  init    access  support callback
//   %%REG             IPInENm     8|16|32 8|16|32 8       0x800   0x0     R|W     TRUE    -
//   %%REG             IPInFLGm    8|16|32 8|16|32 8       0x804   0x0     R       TRUE    -
//   %%REG             IPInFCLRm   8|16|32 8|16|32 8       0x808   0x0     W       TRUE    W
//   %%REG             IPInREQm    8|16|32 8|16|32 8       0x810   0x0     R|W     TRUE    W
//   %%REG             IPInRCLRm   8|16|32 8|16|32 8       0x814   0x0     W       TRUE    W
////////////////////////////////////////////////////////////////////////////////
/// @file g4_ipir_regif.h
/// @brief Register IF class of model G4_IPIR
/// $Id: g4_ipir_regif.h,v 1.2 2019/07/10 11:25:13 nhuvo Exp $
/// $Date: 2019/07/10 11:25:13 $
/// $Revison$
/// $Author: nhuvo $
////////////////////////////////////////////////////////////////////////////////
#ifndef __G4_IPIR_REGIF_H__
#define __G4_IPIR_REGIF_H__
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
#ifdef USR_CWR_SYSTEMC
#include "scml.h"
#endif

/// Register IF class of G4_IPIR model
class Cg4_ipir_regif
: public vpcl::reg_super
{
protected:
    typedef const unsigned int cuint;
    typedef unsigned int uint;

    enum eRegGroup {
        empe0_ch0, empe0_ch1, empe0_ch2, empe0_ch3, empe1_ch0, empe1_ch1, empe1_ch2, empe1_ch3, empe2_ch0, empe2_ch1, empe2_ch2, empe2_ch3, empe3_ch0, empe3_ch1, empe3_ch2, empe3_ch3, empe4_ch0, empe4_ch1, empe4_ch2, empe4_ch3, empe5_ch0, empe5_ch1, empe5_ch2, empe5_ch3, empe6_ch0, empe6_ch1, empe6_ch2, empe6_ch3, empe7_ch0, empe7_ch1, empe7_ch2, empe7_ch3, emNum_of_gr
    }; ///< Enumeration for register group index

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
    Cg4_ipir_regif(std::string name, uint buswidth);
    ~Cg4_ipir_regif();

protected:
    vpcl::re_register *IPInENm  [emNum_of_gr];
    vpcl::re_register *IPInFLGm [emNum_of_gr];
    vpcl::re_register *IPInFCLRm[emNum_of_gr];
    vpcl::re_register *IPInREQm [emNum_of_gr];
    vpcl::re_register *IPInRCLRm[emNum_of_gr];

    #ifdef USR_CWR_SYSTEMC
    scml2::memory<uint> cwmem;
    scml2::reg<uint>      *CW_IPInENm_empe0_ch0_;
    scml2::reg<uint>      *CW_IPInFLGm_empe0_ch0_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe0_ch0_;
    scml2::reg<uint>      *CW_IPInREQm_empe0_ch0_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe0_ch0_;
    scml2::reg<uint>      *CW_IPInENm_empe0_ch1_;
    scml2::reg<uint>      *CW_IPInFLGm_empe0_ch1_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe0_ch1_;
    scml2::reg<uint>      *CW_IPInREQm_empe0_ch1_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe0_ch1_;
    scml2::reg<uint>      *CW_IPInENm_empe0_ch2_;
    scml2::reg<uint>      *CW_IPInFLGm_empe0_ch2_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe0_ch2_;
    scml2::reg<uint>      *CW_IPInREQm_empe0_ch2_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe0_ch2_;
    scml2::reg<uint>      *CW_IPInENm_empe0_ch3_;
    scml2::reg<uint>      *CW_IPInFLGm_empe0_ch3_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe0_ch3_;
    scml2::reg<uint>      *CW_IPInREQm_empe0_ch3_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe0_ch3_;
    scml2::reg<uint>      *CW_IPInENm_empe1_ch0_;
    scml2::reg<uint>      *CW_IPInFLGm_empe1_ch0_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe1_ch0_;
    scml2::reg<uint>      *CW_IPInREQm_empe1_ch0_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe1_ch0_;
    scml2::reg<uint>      *CW_IPInENm_empe1_ch1_;
    scml2::reg<uint>      *CW_IPInFLGm_empe1_ch1_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe1_ch1_;
    scml2::reg<uint>      *CW_IPInREQm_empe1_ch1_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe1_ch1_;
    scml2::reg<uint>      *CW_IPInENm_empe1_ch2_;
    scml2::reg<uint>      *CW_IPInFLGm_empe1_ch2_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe1_ch2_;
    scml2::reg<uint>      *CW_IPInREQm_empe1_ch2_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe1_ch2_;
    scml2::reg<uint>      *CW_IPInENm_empe1_ch3_;
    scml2::reg<uint>      *CW_IPInFLGm_empe1_ch3_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe1_ch3_;
    scml2::reg<uint>      *CW_IPInREQm_empe1_ch3_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe1_ch3_;
    scml2::reg<uint>      *CW_IPInENm_empe2_ch0_;
    scml2::reg<uint>      *CW_IPInFLGm_empe2_ch0_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe2_ch0_;
    scml2::reg<uint>      *CW_IPInREQm_empe2_ch0_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe2_ch0_;
    scml2::reg<uint>      *CW_IPInENm_empe2_ch1_;
    scml2::reg<uint>      *CW_IPInFLGm_empe2_ch1_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe2_ch1_;
    scml2::reg<uint>      *CW_IPInREQm_empe2_ch1_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe2_ch1_;
    scml2::reg<uint>      *CW_IPInENm_empe2_ch2_;
    scml2::reg<uint>      *CW_IPInFLGm_empe2_ch2_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe2_ch2_;
    scml2::reg<uint>      *CW_IPInREQm_empe2_ch2_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe2_ch2_;
    scml2::reg<uint>      *CW_IPInENm_empe2_ch3_;
    scml2::reg<uint>      *CW_IPInFLGm_empe2_ch3_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe2_ch3_;
    scml2::reg<uint>      *CW_IPInREQm_empe2_ch3_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe2_ch3_;
    scml2::reg<uint>      *CW_IPInENm_empe3_ch0_;
    scml2::reg<uint>      *CW_IPInFLGm_empe3_ch0_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe3_ch0_;
    scml2::reg<uint>      *CW_IPInREQm_empe3_ch0_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe3_ch0_;
    scml2::reg<uint>      *CW_IPInENm_empe3_ch1_;
    scml2::reg<uint>      *CW_IPInFLGm_empe3_ch1_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe3_ch1_;
    scml2::reg<uint>      *CW_IPInREQm_empe3_ch1_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe3_ch1_;
    scml2::reg<uint>      *CW_IPInENm_empe3_ch2_;
    scml2::reg<uint>      *CW_IPInFLGm_empe3_ch2_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe3_ch2_;
    scml2::reg<uint>      *CW_IPInREQm_empe3_ch2_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe3_ch2_;
    scml2::reg<uint>      *CW_IPInENm_empe3_ch3_;
    scml2::reg<uint>      *CW_IPInFLGm_empe3_ch3_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe3_ch3_;
    scml2::reg<uint>      *CW_IPInREQm_empe3_ch3_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe3_ch3_;
    scml2::reg<uint>      *CW_IPInENm_empe4_ch0_;
    scml2::reg<uint>      *CW_IPInFLGm_empe4_ch0_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe4_ch0_;
    scml2::reg<uint>      *CW_IPInREQm_empe4_ch0_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe4_ch0_;
    scml2::reg<uint>      *CW_IPInENm_empe4_ch1_;
    scml2::reg<uint>      *CW_IPInFLGm_empe4_ch1_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe4_ch1_;
    scml2::reg<uint>      *CW_IPInREQm_empe4_ch1_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe4_ch1_;
    scml2::reg<uint>      *CW_IPInENm_empe4_ch2_;
    scml2::reg<uint>      *CW_IPInFLGm_empe4_ch2_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe4_ch2_;
    scml2::reg<uint>      *CW_IPInREQm_empe4_ch2_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe4_ch2_;
    scml2::reg<uint>      *CW_IPInENm_empe4_ch3_;
    scml2::reg<uint>      *CW_IPInFLGm_empe4_ch3_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe4_ch3_;
    scml2::reg<uint>      *CW_IPInREQm_empe4_ch3_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe4_ch3_;
    scml2::reg<uint>      *CW_IPInENm_empe5_ch0_;
    scml2::reg<uint>      *CW_IPInFLGm_empe5_ch0_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe5_ch0_;
    scml2::reg<uint>      *CW_IPInREQm_empe5_ch0_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe5_ch0_;
    scml2::reg<uint>      *CW_IPInENm_empe5_ch1_;
    scml2::reg<uint>      *CW_IPInFLGm_empe5_ch1_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe5_ch1_;
    scml2::reg<uint>      *CW_IPInREQm_empe5_ch1_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe5_ch1_;
    scml2::reg<uint>      *CW_IPInENm_empe5_ch2_;
    scml2::reg<uint>      *CW_IPInFLGm_empe5_ch2_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe5_ch2_;
    scml2::reg<uint>      *CW_IPInREQm_empe5_ch2_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe5_ch2_;
    scml2::reg<uint>      *CW_IPInENm_empe5_ch3_;
    scml2::reg<uint>      *CW_IPInFLGm_empe5_ch3_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe5_ch3_;
    scml2::reg<uint>      *CW_IPInREQm_empe5_ch3_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe5_ch3_;
    scml2::reg<uint>      *CW_IPInENm_empe6_ch0_;
    scml2::reg<uint>      *CW_IPInFLGm_empe6_ch0_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe6_ch0_;
    scml2::reg<uint>      *CW_IPInREQm_empe6_ch0_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe6_ch0_;
    scml2::reg<uint>      *CW_IPInENm_empe6_ch1_;
    scml2::reg<uint>      *CW_IPInFLGm_empe6_ch1_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe6_ch1_;
    scml2::reg<uint>      *CW_IPInREQm_empe6_ch1_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe6_ch1_;
    scml2::reg<uint>      *CW_IPInENm_empe6_ch2_;
    scml2::reg<uint>      *CW_IPInFLGm_empe6_ch2_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe6_ch2_;
    scml2::reg<uint>      *CW_IPInREQm_empe6_ch2_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe6_ch2_;
    scml2::reg<uint>      *CW_IPInENm_empe6_ch3_;
    scml2::reg<uint>      *CW_IPInFLGm_empe6_ch3_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe6_ch3_;
    scml2::reg<uint>      *CW_IPInREQm_empe6_ch3_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe6_ch3_;
    scml2::reg<uint>      *CW_IPInENm_empe7_ch0_;
    scml2::reg<uint>      *CW_IPInFLGm_empe7_ch0_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe7_ch0_;
    scml2::reg<uint>      *CW_IPInREQm_empe7_ch0_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe7_ch0_;
    scml2::reg<uint>      *CW_IPInENm_empe7_ch1_;
    scml2::reg<uint>      *CW_IPInFLGm_empe7_ch1_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe7_ch1_;
    scml2::reg<uint>      *CW_IPInREQm_empe7_ch1_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe7_ch1_;
    scml2::reg<uint>      *CW_IPInENm_empe7_ch2_;
    scml2::reg<uint>      *CW_IPInFLGm_empe7_ch2_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe7_ch2_;
    scml2::reg<uint>      *CW_IPInREQm_empe7_ch2_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe7_ch2_;
    scml2::reg<uint>      *CW_IPInENm_empe7_ch3_;
    scml2::reg<uint>      *CW_IPInFLGm_empe7_ch3_;
    scml2::reg<uint>      *CW_IPInFCLRm_empe7_ch3_;
    scml2::reg<uint>      *CW_IPInREQm_empe7_ch3_;
    scml2::reg<uint>      *CW_IPInRCLRm_empe7_ch3_;
    scml2::bitfield<uint> *CW_IPInENm  [emNum_of_gr];
    scml2::bitfield<uint> *CW_IPInFLGm [emNum_of_gr];
    scml2::bitfield<uint> *CW_IPInFCLRm[emNum_of_gr];
    scml2::bitfield<uint> *CW_IPInREQm [emNum_of_gr];
    scml2::bitfield<uint> *CW_IPInRCLRm[emNum_of_gr];
    #endif

    uint IPInENm_IPInENm_empe0_ch0;
    uint IPInFLGm_IPInFLGm_empe0_ch0;
    uint IPInFCLRm_IPInFCLRm_empe0_ch0;
    uint IPInREQm_IPInREQm_empe0_ch0;
    uint IPInRCLRm_IPInRCLRm_empe0_ch0;
    uint IPInENm_IPInENm_empe0_ch1;
    uint IPInFLGm_IPInFLGm_empe0_ch1;
    uint IPInFCLRm_IPInFCLRm_empe0_ch1;
    uint IPInREQm_IPInREQm_empe0_ch1;
    uint IPInRCLRm_IPInRCLRm_empe0_ch1;
    uint IPInENm_IPInENm_empe0_ch2;
    uint IPInFLGm_IPInFLGm_empe0_ch2;
    uint IPInFCLRm_IPInFCLRm_empe0_ch2;
    uint IPInREQm_IPInREQm_empe0_ch2;
    uint IPInRCLRm_IPInRCLRm_empe0_ch2;
    uint IPInENm_IPInENm_empe0_ch3;
    uint IPInFLGm_IPInFLGm_empe0_ch3;
    uint IPInFCLRm_IPInFCLRm_empe0_ch3;
    uint IPInREQm_IPInREQm_empe0_ch3;
    uint IPInRCLRm_IPInRCLRm_empe0_ch3;
    uint IPInENm_IPInENm_empe1_ch0;
    uint IPInFLGm_IPInFLGm_empe1_ch0;
    uint IPInFCLRm_IPInFCLRm_empe1_ch0;
    uint IPInREQm_IPInREQm_empe1_ch0;
    uint IPInRCLRm_IPInRCLRm_empe1_ch0;
    uint IPInENm_IPInENm_empe1_ch1;
    uint IPInFLGm_IPInFLGm_empe1_ch1;
    uint IPInFCLRm_IPInFCLRm_empe1_ch1;
    uint IPInREQm_IPInREQm_empe1_ch1;
    uint IPInRCLRm_IPInRCLRm_empe1_ch1;
    uint IPInENm_IPInENm_empe1_ch2;
    uint IPInFLGm_IPInFLGm_empe1_ch2;
    uint IPInFCLRm_IPInFCLRm_empe1_ch2;
    uint IPInREQm_IPInREQm_empe1_ch2;
    uint IPInRCLRm_IPInRCLRm_empe1_ch2;
    uint IPInENm_IPInENm_empe1_ch3;
    uint IPInFLGm_IPInFLGm_empe1_ch3;
    uint IPInFCLRm_IPInFCLRm_empe1_ch3;
    uint IPInREQm_IPInREQm_empe1_ch3;
    uint IPInRCLRm_IPInRCLRm_empe1_ch3;
    uint IPInENm_IPInENm_empe2_ch0;
    uint IPInFLGm_IPInFLGm_empe2_ch0;
    uint IPInFCLRm_IPInFCLRm_empe2_ch0;
    uint IPInREQm_IPInREQm_empe2_ch0;
    uint IPInRCLRm_IPInRCLRm_empe2_ch0;
    uint IPInENm_IPInENm_empe2_ch1;
    uint IPInFLGm_IPInFLGm_empe2_ch1;
    uint IPInFCLRm_IPInFCLRm_empe2_ch1;
    uint IPInREQm_IPInREQm_empe2_ch1;
    uint IPInRCLRm_IPInRCLRm_empe2_ch1;
    uint IPInENm_IPInENm_empe2_ch2;
    uint IPInFLGm_IPInFLGm_empe2_ch2;
    uint IPInFCLRm_IPInFCLRm_empe2_ch2;
    uint IPInREQm_IPInREQm_empe2_ch2;
    uint IPInRCLRm_IPInRCLRm_empe2_ch2;
    uint IPInENm_IPInENm_empe2_ch3;
    uint IPInFLGm_IPInFLGm_empe2_ch3;
    uint IPInFCLRm_IPInFCLRm_empe2_ch3;
    uint IPInREQm_IPInREQm_empe2_ch3;
    uint IPInRCLRm_IPInRCLRm_empe2_ch3;
    uint IPInENm_IPInENm_empe3_ch0;
    uint IPInFLGm_IPInFLGm_empe3_ch0;
    uint IPInFCLRm_IPInFCLRm_empe3_ch0;
    uint IPInREQm_IPInREQm_empe3_ch0;
    uint IPInRCLRm_IPInRCLRm_empe3_ch0;
    uint IPInENm_IPInENm_empe3_ch1;
    uint IPInFLGm_IPInFLGm_empe3_ch1;
    uint IPInFCLRm_IPInFCLRm_empe3_ch1;
    uint IPInREQm_IPInREQm_empe3_ch1;
    uint IPInRCLRm_IPInRCLRm_empe3_ch1;
    uint IPInENm_IPInENm_empe3_ch2;
    uint IPInFLGm_IPInFLGm_empe3_ch2;
    uint IPInFCLRm_IPInFCLRm_empe3_ch2;
    uint IPInREQm_IPInREQm_empe3_ch2;
    uint IPInRCLRm_IPInRCLRm_empe3_ch2;
    uint IPInENm_IPInENm_empe3_ch3;
    uint IPInFLGm_IPInFLGm_empe3_ch3;
    uint IPInFCLRm_IPInFCLRm_empe3_ch3;
    uint IPInREQm_IPInREQm_empe3_ch3;
    uint IPInRCLRm_IPInRCLRm_empe3_ch3;
    uint IPInENm_IPInENm_empe4_ch0;
    uint IPInFLGm_IPInFLGm_empe4_ch0;
    uint IPInFCLRm_IPInFCLRm_empe4_ch0;
    uint IPInREQm_IPInREQm_empe4_ch0;
    uint IPInRCLRm_IPInRCLRm_empe4_ch0;
    uint IPInENm_IPInENm_empe4_ch1;
    uint IPInFLGm_IPInFLGm_empe4_ch1;
    uint IPInFCLRm_IPInFCLRm_empe4_ch1;
    uint IPInREQm_IPInREQm_empe4_ch1;
    uint IPInRCLRm_IPInRCLRm_empe4_ch1;
    uint IPInENm_IPInENm_empe4_ch2;
    uint IPInFLGm_IPInFLGm_empe4_ch2;
    uint IPInFCLRm_IPInFCLRm_empe4_ch2;
    uint IPInREQm_IPInREQm_empe4_ch2;
    uint IPInRCLRm_IPInRCLRm_empe4_ch2;
    uint IPInENm_IPInENm_empe4_ch3;
    uint IPInFLGm_IPInFLGm_empe4_ch3;
    uint IPInFCLRm_IPInFCLRm_empe4_ch3;
    uint IPInREQm_IPInREQm_empe4_ch3;
    uint IPInRCLRm_IPInRCLRm_empe4_ch3;
    uint IPInENm_IPInENm_empe5_ch0;
    uint IPInFLGm_IPInFLGm_empe5_ch0;
    uint IPInFCLRm_IPInFCLRm_empe5_ch0;
    uint IPInREQm_IPInREQm_empe5_ch0;
    uint IPInRCLRm_IPInRCLRm_empe5_ch0;
    uint IPInENm_IPInENm_empe5_ch1;
    uint IPInFLGm_IPInFLGm_empe5_ch1;
    uint IPInFCLRm_IPInFCLRm_empe5_ch1;
    uint IPInREQm_IPInREQm_empe5_ch1;
    uint IPInRCLRm_IPInRCLRm_empe5_ch1;
    uint IPInENm_IPInENm_empe5_ch2;
    uint IPInFLGm_IPInFLGm_empe5_ch2;
    uint IPInFCLRm_IPInFCLRm_empe5_ch2;
    uint IPInREQm_IPInREQm_empe5_ch2;
    uint IPInRCLRm_IPInRCLRm_empe5_ch2;
    uint IPInENm_IPInENm_empe5_ch3;
    uint IPInFLGm_IPInFLGm_empe5_ch3;
    uint IPInFCLRm_IPInFCLRm_empe5_ch3;
    uint IPInREQm_IPInREQm_empe5_ch3;
    uint IPInRCLRm_IPInRCLRm_empe5_ch3;
    uint IPInENm_IPInENm_empe6_ch0;
    uint IPInFLGm_IPInFLGm_empe6_ch0;
    uint IPInFCLRm_IPInFCLRm_empe6_ch0;
    uint IPInREQm_IPInREQm_empe6_ch0;
    uint IPInRCLRm_IPInRCLRm_empe6_ch0;
    uint IPInENm_IPInENm_empe6_ch1;
    uint IPInFLGm_IPInFLGm_empe6_ch1;
    uint IPInFCLRm_IPInFCLRm_empe6_ch1;
    uint IPInREQm_IPInREQm_empe6_ch1;
    uint IPInRCLRm_IPInRCLRm_empe6_ch1;
    uint IPInENm_IPInENm_empe6_ch2;
    uint IPInFLGm_IPInFLGm_empe6_ch2;
    uint IPInFCLRm_IPInFCLRm_empe6_ch2;
    uint IPInREQm_IPInREQm_empe6_ch2;
    uint IPInRCLRm_IPInRCLRm_empe6_ch2;
    uint IPInENm_IPInENm_empe6_ch3;
    uint IPInFLGm_IPInFLGm_empe6_ch3;
    uint IPInFCLRm_IPInFCLRm_empe6_ch3;
    uint IPInREQm_IPInREQm_empe6_ch3;
    uint IPInRCLRm_IPInRCLRm_empe6_ch3;
    uint IPInENm_IPInENm_empe7_ch0;
    uint IPInFLGm_IPInFLGm_empe7_ch0;
    uint IPInFCLRm_IPInFCLRm_empe7_ch0;
    uint IPInREQm_IPInREQm_empe7_ch0;
    uint IPInRCLRm_IPInRCLRm_empe7_ch0;
    uint IPInENm_IPInENm_empe7_ch1;
    uint IPInFLGm_IPInFLGm_empe7_ch1;
    uint IPInFCLRm_IPInFCLRm_empe7_ch1;
    uint IPInREQm_IPInREQm_empe7_ch1;
    uint IPInRCLRm_IPInRCLRm_empe7_ch1;
    uint IPInENm_IPInENm_empe7_ch2;
    uint IPInFLGm_IPInFLGm_empe7_ch2;
    uint IPInFCLRm_IPInFCLRm_empe7_ch2;
    uint IPInREQm_IPInREQm_empe7_ch2;
    uint IPInRCLRm_IPInRCLRm_empe7_ch2;
    uint IPInENm_IPInENm_empe7_ch3;
    uint IPInFLGm_IPInFLGm_empe7_ch3;
    uint IPInFCLRm_IPInFCLRm_empe7_ch3;
    uint IPInREQm_IPInREQm_empe7_ch3;
    uint IPInRCLRm_IPInRCLRm_empe7_ch3;

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

    virtual void cb_IPInFCLRm_IPInFCLRm(RegCBstr str) = 0;
    virtual void cb_IPInREQm_IPInREQm(RegCBstr str) = 0;
    virtual void cb_IPInRCLRm_IPInRCLRm(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Cg4_ipir_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Cg4_ipir_regif::*) (RegCBstr)> > mRdCbAPI;
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

    #ifdef USR_CWR_SYSTEMC
    uint cw_rd_cb(tlm::tlm_generic_payload& trans, int tag);
    uint cw_wr_cb(tlm::tlm_generic_payload& trans, int tag);
    void cw_instantiate_reg(scml2::reg<uint> * reg, std::string reg_name, int offset);
    #endif
};
#endif //__G4_IPIR_REG_DEF_H__
