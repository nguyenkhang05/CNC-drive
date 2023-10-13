// -----------------------------------------------------------------------------
// $Id: qlapbfictlav1_sub_regif.h,v 1.3 2020/03/24 07:16:33 duongphan Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
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
// Input file : /home/longnt20/girf/GenSrc/pbg_input/qlapbfictlav1_sub_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE QLAPBFICTLAV1_Sub
//     #              name      offset_size
//     %%REG_INSTANCE  reg_def   2
// 
// %REG_CHANNEL reg_def
//     %%TITLE     name             reg_name        wsize     rsize     length      offset      access      init    factor_start    factor_end      factor_step     support     callback
//     %%REG       FCLACTL         FCLACTL            8        8|16|32     8          0x00        R|W        0x0           -               -               -          TRUE         -
// 
// 
// %REG_NAME FCLACTL
//     %%TITLE     name        upper   lower   init    access  support     callback
//     %%BIT       BYPS   7       7       0       R|W     TRUE        W
//     %%BIT       INTL   2       2       0       R|W     TRUE        W
//     %%BIT       INTF   1       1       0       R|W     TRUE        W
//     %%BIT       INTR   0       0       0       R|W     TRUE        W
// 
// 
// 
////////////////////////////////////////////////////////////////////////////////
/// @file qlapbfictlav1_sub_regif.h
/// @brief Register IF class of model QLAPBFICTLAV1_SUB
/// $Id: qlapbfictlav1_sub_regif.h,v 1.3 2020/03/24 07:16:33 duongphan Exp $
/// $Date: 2020/03/24 07:16:33 $
/// $Revison$
/// $Author: duongphan $
////////////////////////////////////////////////////////////////////////////////
#ifndef __QLAPBFICTLAV1_SUB_REGIF_H__
#define __QLAPBFICTLAV1_SUB_REGIF_H__
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

/// Register IF class of QLAPBFICTLAV1_SUB model
class Cqlapbfictlav1_sub_regif
: public vpcl::reg_super
{
protected:
    typedef const unsigned int cuint;
    typedef unsigned int uint;

    enum eRegGroup {
        emNum_of_gr
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
    Cqlapbfictlav1_sub_regif(std::string name, uint buswidth);
    ~Cqlapbfictlav1_sub_regif();

protected:
    vpcl::re_register *FCLACTL;

    #ifdef USR_CWR_SYSTEMC
    scml2::memory<unsigned char> cwmem;
    scml2::reg<unsigned char> *CW_FCLACTL;
    #endif

    uint FCLACTL_BYPS;
    uint FCLACTL_INTL;
    uint FCLACTL_INTF;
    uint FCLACTL_INTR;

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

    virtual void cb_FCLACTL_BYPS(RegCBstr str) = 0;
    virtual void cb_FCLACTL_INTL(RegCBstr str) = 0;
    virtual void cb_FCLACTL_INTF(RegCBstr str) = 0;
    virtual void cb_FCLACTL_INTR(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Cqlapbfictlav1_sub_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Cqlapbfictlav1_sub_regif::*) (RegCBstr)> > mRdCbAPI;
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
    unsigned char cw_rd_cb(tlm::tlm_generic_payload& trans, int tag);
    unsigned char cw_wr_cb(tlm::tlm_generic_payload& trans, int tag);
    void cw_instantiate_reg(scml2::reg<unsigned char> * reg, std::string reg_name, int offset);
    #endif
};
#endif //__QLAPBFICTLAV1_SUB_REG_DEF_H__