// -----------------------------------------------------------------------------
// $Id: hi20fsgd2_regif.h,v 1.3 2019/11/28 09:56:42 hadinhtran Exp $
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
// Input file : input/hi20fsgd2_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE HI20FSGD2
// 
// %%REG_INSTANCE  reg_def 7
// 
// %REG_CHANNEL reg_def
//     %%TITLE     name          reg_name     wsize     rsize      length      offset      access      init           factor_start    factor_end      factor_step     support     callback
//     %%REG       PBGPROT0      PBGPROT0    8|16|32    8|16|32    32          0x00        R|W         0x00000141     0               15              0x08            TRUE        W
//     %%REG       PBGPROT1      PBGPROT1    8|16|32    8|16|32    32          0x04        R|W         0x0000000F     0               15              0x08            TRUE        -
// 
// 
// %REG_NAME PBGPROT0
//     %%TITLE     name        upper   lower   init    access  support     callback
//     %%BIT       LOCK        31      31       0       R|W     TRUE        W
//     %%BIT       GEN          8       8       1       R|W     TRUE        -
//     %%BIT       SEC          7       7       0       R|W     TRUE        -
//     %%BIT       DBG          6       6       1       R|W     TRUE        -
//     %%BIT       UM           4       4       0       R|W     TRUE        -
//     %%BIT       WG           1       1       0       R|W     TRUE        -
//     %%BIT       RG           0       0       1       R|W     TRUE        -
// 
// %REG_NAME PBGPROT1
//     %%TITLE     name        upper   lower   init         access  support     callback
//     %%BIT       SPID        31       0      0x0000000F    R|W     TRUE        W
// 
////////////////////////////////////////////////////////////////////////////////
/// @file hi20fsgd2_regif.h
/// @brief Register IF class of model HI20FSGD2
/// $Id: hi20fsgd2_regif.h,v 1.3 2019/11/28 09:56:42 hadinhtran Exp $
/// $Date: 2019/11/28 09:56:42 $
/// $Revison$
/// $Author: hadinhtran $
////////////////////////////////////////////////////////////////////////////////
#ifndef __HI20FSGD2_REGIF_H__
#define __HI20FSGD2_REGIF_H__
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

/// Register IF class of HI20FSGD2 model
class Chi20fsgd2_regif
: public vpcl::reg_super
{
protected:
    typedef const unsigned int cuint;
    typedef unsigned int uint;

    enum eRegGroup {
        emNum_of_gr
    }; ///< Enumeration for register group index
    enum eRegIndex {
        emNUM_PBGPROT0 = 16,
        emNUM_PBGPROT1 = 16
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
    Chi20fsgd2_regif(std::string name, uint buswidth, uint mCh_num);
    ~Chi20fsgd2_regif();

protected:
    vpcl::re_register *PBGPROT0[16];
    vpcl::re_register *PBGPROT1[16];

    #ifdef USR_CWR_SYSTEMC
    scml2::memory<uint> cwmem;
    scml2::reg<uint> *CW_PBGPROT0[16];
    scml2::reg<uint> *CW_PBGPROT1[16];
    #endif

    uint PBGPROT0_LOCK[16];
    uint PBGPROT0_GEN[16];
    uint PBGPROT0_SEC[16];
    uint PBGPROT0_DBG[16];
    uint PBGPROT0_UM [16];
    uint PBGPROT0_WG [16];
    uint PBGPROT0_RG [16];
    uint PBGPROT1_SPID[16];

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

    virtual void cb_PBGPROT0_LOCK(RegCBstr str) = 0;
    virtual void cb_PBGPROT1_SPID(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Chi20fsgd2_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Chi20fsgd2_regif::*) (RegCBstr)> > mRdCbAPI;
    std::string mFileName;
    std::string mInstName;
    int mLineNum;
    uint mCh_num;

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
#endif //__HI20FSGD2_REG_DEF_H__
