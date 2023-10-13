// -----------------------------------------------------------------------------
// $Id: pic1_e2_regif.h 3395 2017-06-29 08:02:34Z ChinhTD $
//
// Copyright(c) 2017 Renesas Electronics Corporation
// Copyright(c) 2017 Renesas Design Vietnam Co., Ltd.
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
// Input file : pic1_e2_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE PIC1_E2
//     #                   name            offset_size
//     %%REG_INSTANCE      reg_def         5
// 
// %REG_CHANNEL reg_def
//     %%TITLE  name       reg_name   wsize  rsize    length  offset  factor_start  factor_end  factor_index  factor_step  access  init        support  callback
//     %%REG    PIC1SST    PIC1SST     8     8|16|32  8       0x0004  -             -           -             -             W|R      0           TRUE     W
//     %%REG    PIC1SSER0  PIC1SSER0  16     8|16|32  16      0x0010  -             -           -             -             W|R      0           TRUE     -
// 
// %REG_NAME PIC1SST
//     %%TITLE  name        upper  lower  init  access  support  callback
//     %%BIT    SYNCTRG      0      0      0     W|R     TRUE     W
// 
// %REG_NAME PIC1SSER0
//     %%TITLE  name        upper  lower  init  access  support  callback
//     %%BIT    PIC1SSER006  6      6      0     W|R     TRUE     -
//     %%BIT    PIC1SSER005  5      5      0     W|R     TRUE     -
//     %%BIT    PIC1SSER004  4      4      0     W|R     TRUE     -
//     %%BIT    PIC1SSER003  3      3      0     W|R     TRUE     -
//     %%BIT    PIC1SSER002  2      2      0     W|R     TRUE     -
//     %%BIT    PIC1SSER001  1      1      0     W|R     TRUE     -
//     %%BIT    PIC1SSER000  0      0      0     W|R     TRUE     -
////////////////////////////////////////////////////////////////////////////////
/// @file pic1_e2_regif.h
/// @brief Register IF class of model PIC1_E2
/// $Id: pic1_e2_regif.h 3395 2017-06-29 08:02:34Z ChinhTD $
/// $Date: 2017-06-29 15:02:34 +0700 (Thu, 29 Jun 2017) $
/// $Revison$
/// $Author: ChinhTD $
////////////////////////////////////////////////////////////////////////////////
#ifndef __PIC1_E2_REGIF_H__
#define __PIC1_E2_REGIF_H__
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

/// Register IF class of PIC1_E2 model
class Cpic1_e2_regif
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
    Cpic1_e2_regif(std::string name, uint buswidth);
    ~Cpic1_e2_regif();

protected:
    vpcl::re_register *PIC1SST  ;
    vpcl::re_register *PIC1SSER0;

    #ifdef CWR_SYSTEMC
    scml2::memory<unsigned short> cwmem;
    scml2::reg<unsigned short>      *CW_PIC1SST_ ;
    scml2::bitfield<unsigned short> *CW_PIC1SST  ;
    scml2::reg<unsigned short>      *CW_PIC1SSER0;
    #endif

    uint PIC1SST_SYNCTRG;
    uint PIC1SSER0_PIC1SSER006;
    uint PIC1SSER0_PIC1SSER005;
    uint PIC1SSER0_PIC1SSER004;
    uint PIC1SSER0_PIC1SSER003;
    uint PIC1SSER0_PIC1SSER002;
    uint PIC1SSER0_PIC1SSER001;
    uint PIC1SSER0_PIC1SSER000;

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

    virtual void cb_PIC1SST_SYNCTRG(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Cpic1_e2_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Cpic1_e2_regif::*) (RegCBstr)> > mRdCbAPI;
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
    unsigned short cw_rd_cb(tlm::tlm_generic_payload& trans, int tag);
    unsigned short cw_wr_cb(tlm::tlm_generic_payload& trans, int tag);
    void cw_instantiate_reg(scml2::reg<unsigned short> * reg, std::string reg_name, int offset);
    #endif
};
#endif //__PIC1_E2_REG_DEF_H__