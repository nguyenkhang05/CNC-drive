// -----------------------------------------------------------------------------
// $Id: apbfsguard_regif.h,v 1.11 2014/09/25 08:51:11 dungvannguyen Exp $
//
// Copyright(c) 2010-2013 Renesas Electronics Corporation
// Copyright(c) 2010-2013 Renesas Design Vietnam Co., Ltd.
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
// Input file : apbfsguard_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE APBFSGuard
//     #                   name            offset_size
//     %%REG_INSTANCE      reg_def             7
// 
// %REG_CHANNEL reg_def
//     %%TITLE     group   name    reg_name   wsize    rsize    length  offset   factor_start  factor_end   factor_index  factor_step   access   support  callback
//     %%REG       -       PROT    PROT       8|16|32  8|16|32  32      0x0      0             15           -             0x08          R|W      TRUE     -
//     %%REG       -       SPID    SPID       8|16|32  8|16|32  32      0x4      0             15           -             0x08          R|W      TRUE     -
// 
// %REG_NAME PROT
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    LOCK        31     31     0     W|R     TRUE     W
//     %%BIT    VM          26     26     0     W|R     TRUE     -
//     %%BIT    UM          25     25     0     W|R     TRUE     -
//     %%BIT    PEID        24     17     0     W|R     TRUE     -
//     %%BIT    VCID        16     9      0     W|R     TRUE     -
//     %%BIT    DEB         4      4      0     W|R     TRUE     -
//     %%BIT    RDPDEF      3      3      0     W|R     TRUE     -
//     %%BIT    WRPDEF      2      2      0     W|R     TRUE     -
//     %%BIT    RD          1      1      0     W|R     TRUE     -
//     %%BIT    WR          0      0      0     W|R     TRUE     -
// 
// %REG_NAME SPID
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    SPID        31     0     0      W|R     TRUE     -
////////////////////////////////////////////////////////////////////////////////
/// @file apbfsguard_regif.h
/// @brief Register IF class of model APBFSGUARD
/// $Id: apbfsguard_regif.h,v 1.11 2014/09/25 08:51:11 dungvannguyen Exp $
/// $Date: 2014/09/25 08:51:11 $
/// $Revison$
/// $Author: dungvannguyen $
////////////////////////////////////////////////////////////////////////////////
#ifndef __APBFSGUARD_REGIF_H__
#define __APBFSGUARD_REGIF_H__
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

/// Register IF class of APBFSGUARD model
class Capbfsguard_regif
: public vpcl::reg_super
{
protected:
    typedef const unsigned int cuint;
    typedef unsigned int uint;

    enum eVersion {
        emCC_CUBE = 0,
        emOTHERS = 1
    };
    enum eRegGroup {
        emNum_of_gr
    }; ///< Enumeration for register group index
    enum eRegIndex {
        emNUM_PROT = 16,
        emNUM_SPID = 16
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
    Capbfsguard_regif(std::string name,
                      uint buswidth,
                      uint version = 0,
                      uint filter_number = 1);
    ~Capbfsguard_regif();

protected:
    vpcl::re_register *PROT[16];
    vpcl::re_register *SPID[16];

    #ifdef CWR_SYSTEMC
    scml2::memory<uint> cwmem;
    scml2::reg<uint> *CW_PROT[16];
    scml2::reg<uint> *CW_SPID[16];
    #endif

    uint PROT_LOCK[16];
    uint PROT_VM [16];
    uint PROT_UM [16];
    uint PROT_PEID[16];
    uint PROT_VCID[16];
    uint PROT_SPID[16];
    uint PROT_DEB[16];
    uint PROT_RDPDEF[16];
    uint PROT_WRPDEF[16];
    uint PROT_RD [16];
    uint PROT_WR [16];
    uint SPID_SPID[16];

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

    virtual void cb_PROT_LOCK(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    unsigned int mMaskedBitNumber;
    unsigned int mVersion;
    unsigned int mFilterNumber;
    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Capbfsguard_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Capbfsguard_regif::*) (RegCBstr)> > mRdCbAPI;
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
#endif //__APBFSGUARD_REG_DEF_H__
