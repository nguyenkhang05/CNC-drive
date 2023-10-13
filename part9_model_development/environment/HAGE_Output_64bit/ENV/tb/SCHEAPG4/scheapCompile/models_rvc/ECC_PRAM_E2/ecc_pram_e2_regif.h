// -----------------------------------------------------------------------------
// $Id: ecc_pram_e2_regif.h,v 1.5 2019/09/06 08:49:51 landang Exp $
//
// Copyright(c) 2019 Renesas Electronics Corporation
// Copyright(c) 2019 Renesas Design Vietnam Co., Ltd.
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
// Input file : input/PRAM/ecc_pram_e2_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE ecc_pram_e2
//     #              name     offset_size
//     %%REG_INSTANCE reg_def  5 
// 
// %REG_CHANNEL reg_def                                                       
//     %%TITLE    name         reg_name            wsize   rsize        length offset  factor_start    factor_end  factor_index    factor_step access   init   support  callback
//     %%REG      ExCTL        ExCTL               8|16|32 8|16|32      32     0x00    -               -           -               -           R/W      0x0     TRUE     -
//     %%REG      ExTMC        ExTMC               8|16    8|16|32      16     0x04    -               -           -               -           R/W      0x0     TRUE     -
//     %%REG      ExTRC        ExTRC               8|16|32 8|16|32      32     0x08    -               -           -               -           R/W      0x0     TRUE     -
//     %%REG      ExTED        ExTED               8|16|32 8|16|32      32     0x0C    -               -           -               -           R/W      0x0     TRUE     -
//     %%REG      ExEADn       ExEADn              32      8|16|32      32     0x10    0               3           -               4           R        0x0     TRUE     -
//     
// %REG_NAME ExCTL
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    ECDEDF7            31       31     0       R       TRUE        -
//     %%BIT    ECSEDF7            30       30     0       R       TRUE        -
//     %%BIT    ECDEDF6            29       29     0       R       TRUE        -
//     %%BIT    ECSEDF6            28       28     0       R       TRUE        -
//     %%BIT    ECDEDF5            27       27     0       R       TRUE        -
//     %%BIT    ECSEDF5            26       26     0       R       TRUE        -
//     %%BIT    ECDEDF4            25       25     0       R       TRUE        -
//     %%BIT    ECSEDF4            24       24     0       R       TRUE        -
//     %%BIT    ECDEDF3            23       23     0       R       TRUE        -
//     %%BIT    ECSEDF3            22       22     0       R       TRUE        -
//     %%BIT    ECDEDF2            21       21     0       R       TRUE        -
//     %%BIT    ECSEDF2            20       20     0       R       TRUE        -
//     %%BIT    ECDEDF1            19       19     0       R       TRUE        -
//     %%BIT    ECSEDF1            18       18     0       R       TRUE        -
//     %%BIT    ECDEDF0            17       17     0       R       TRUE        -
//     %%BIT    ECSEDF0            16       16     0       R       TRUE        -
//     %%BIT    EMCA               15       14     0       R|W     TRUE        -
//     %%BIT    ECOVFF             11       11     0       R       TRUE        -  
//     %%BIT    ECER2C             10       10     0       R|W     TRUE        -
//     %%BIT    ECER1C              9        9     0       R|W     TRUE        W
//     %%BIT    ECTHM               7        7     0       R|W     TRUE        W
//     %%BIT    ECERVF              6        6     0       R|W     TRUE        -
//     %%BIT    EC1ECP              5        5     0       R|W     TRUE        -
//     %%BIT    EC2EDIC             4        4     1       R|W     TRUE        -
//     %%BIT    EC1EDIC             3        3     0       R|W     TRUE        -
//     %%BIT    ECER2F              2        2     0       R       TRUE        -
//     %%BIT    ECER1F              1        1     0       R       TRUE        -
//     %%BIT    ECEMF               0        0     0       R       TRUE        -
//     
// %REG_NAME ExTMC
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    ETMA               15       14     0       R|W     TRUE        -
//     %%BIT    ECTMCE              7        7     0       R|W     TRUE        -
//     %%BIT    ECTRRS              4        4     0       R|W     TRUE        -
//     %%BIT    ECREOS              3        3     0       R|W     TRUE        -
//     %%BIT    ECENS               2        2     0       R|W     TRUE        -
//     %%BIT    ECDCS               1        1     0       R|W     TRUE        -
//     %%BIT    ECREIS              0        0     0       R|W     TRUE        -
//     
// %REG_NAME ExTRC
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    SYND               30       24     0       R       TRUE        -
//     %%BIT    HORD               22       16     0       R       TRUE        -
//     %%BIT    ECRD               14        8     0       R       TRUE        -
//     %%BIT    ERDB                6        0     0       R|W     TRUE        -
//     
// %REG_NAME ExTED
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    ECEDB              31       0      0       R|W     TRUE        -
//     
////////////////////////////////////////////////////////////////////////////////
/// @file ecc_pram_e2_regif.h
/// @brief Register IF class of model ECC_PRAM_E2
/// $Id: ecc_pram_e2_regif.h,v 1.5 2019/09/06 08:49:51 landang Exp $
/// $Date: 2019/09/06 08:49:51 $
/// $Revison$
/// $Author: landang $
////////////////////////////////////////////////////////////////////////////////
#ifndef __ECC_PRAM_E2_REGIF_H__
#define __ECC_PRAM_E2_REGIF_H__
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

/// Register IF class of ECC_PRAM_E2 model
class Cecc_pram_e2_regif
: public vpcl::reg_super
{
protected:
    typedef const unsigned int cuint;
    typedef unsigned int uint;

    enum eRegGroup {
        emNum_of_gr
    }; ///< Enumeration for register group index
    enum eRegIndex {
        emNUM_ExEADn = 4
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
    Cecc_pram_e2_regif(std::string name, uint buswidth);
    ~Cecc_pram_e2_regif();

protected:
    vpcl::re_register *ExCTL ;
    vpcl::re_register *ExTMC ;
    vpcl::re_register *ExTRC ;
    vpcl::re_register *ExTED ;
    vpcl::re_register *ExEADn[4];

    #ifdef CWR_SYSTEMC
    scml2::memory<uint> cwmem;
    scml2::reg<uint>      *CW_ExTMC_;
    scml2::reg<uint>      *CW_ExCTL ;
    scml2::bitfield<uint> *CW_ExTMC ;
    scml2::reg<uint>      *CW_ExTRC ;
    scml2::reg<uint>      *CW_ExTED ;
    scml2::reg<uint>      *CW_ExEADn[4];
    #endif

    uint ExCTL_ECDEDF7;
    uint ExCTL_ECSEDF7;
    uint ExCTL_ECDEDF6;
    uint ExCTL_ECSEDF6;
    uint ExCTL_ECDEDF5;
    uint ExCTL_ECSEDF5;
    uint ExCTL_ECDEDF4;
    uint ExCTL_ECSEDF4;
    uint ExCTL_ECDEDF3;
    uint ExCTL_ECSEDF3;
    uint ExCTL_ECDEDF2;
    uint ExCTL_ECSEDF2;
    uint ExCTL_ECDEDF1;
    uint ExCTL_ECSEDF1;
    uint ExCTL_ECDEDF0;
    uint ExCTL_ECSEDF0;
    uint ExCTL_EMCA;
    uint ExCTL_ECOVFF;
    uint ExCTL_ECER2C;
    uint ExCTL_ECER1C;
    uint ExCTL_ECTHM;
    uint ExCTL_ECERVF;
    uint ExCTL_EC1ECP;
    uint ExCTL_EC2EDIC;
    uint ExCTL_EC1EDIC;
    uint ExCTL_ECER2F;
    uint ExCTL_ECER1F;
    uint ExCTL_ECEMF;
    uint ExTMC_ETMA;
    uint ExTMC_ECTMCE;
    uint ExTMC_ECTRRS;
    uint ExTMC_ECREOS;
    uint ExTMC_ECENS;
    uint ExTMC_ECDCS;
    uint ExTMC_ECREIS;
    uint ExTRC_SYND;
    uint ExTRC_HORD;
    uint ExTRC_ECRD;
    uint ExTRC_ERDB;
    uint ExTED_ECEDB;
    uint ExEADn_ExEADn[4];

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

    virtual void cb_ExCTL_ECER1C(RegCBstr str) = 0;
    virtual void cb_ExCTL_ECTHM(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Cecc_pram_e2_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Cecc_pram_e2_regif::*) (RegCBstr)> > mRdCbAPI;
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
#endif //__ECC_PRAM_E2_REG_DEF_H__