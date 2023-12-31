// -----------------------------------------------------------------------------
// $Id: pwgenc0_regif.h,v 1.3 2020/01/15 13:23:55 landang Exp $
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
// Input file : input/pwgenc0_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE pwgenc0
//     #              name     offset_size
//     %%REG_INSTANCE reg_def  6 
// 
// %REG_CHANNEL reg_def                                                       
//     %%TITLE    name        reg_name      wsize   rsize      length offset   access   init   support  callback
//     %%REG      PWG0CSDR    PWG0CSDR        16    8|16|32    32     0x00     R|W      0x0     TRUE     -
//     %%REG      PWG0CRDR    PWG0CRDR           32 8|16|32    32     0x04     R|W      0x0     TRUE     -
//     %%REG      PWG0CTDR    PWG0CTDR        16    8|16|32    32     0x08     R|W      0x0     TRUE     -
//     %%REG      PWG0RDT     PWG0RDT       8       8|16|32    32     0x0C     R|W      0x0     TRUE     -
//     %%REG      PWG0ST      PWG0ST          16    8|16|32    32     0x10     R|W      0x0     TRUE     -
//     %%REG      PWG0CNT     PWG0CNT         16    8|16|32    32     0x14     R|W      0xFFFF  TRUE     -
//     %%REG      PWG0TCR     PWG0TCR       8       8|16|32    32     0x18     R|W      0x1     TRUE     -
//     %%REG      PWG0TCBR    PWG0TCBR      8       8|16|32    32     0x1C     R|W      0x1     TRUE     -
//     %%REG      PWG0CTL     PWG0CTL         16    8|16|32    32     0x20     R|W      0x8000  TRUE     -
//     %%REG      PWG0CSBR    PWG0CSBR        16    8|16|32    32     0x24     R|W      0x0     TRUE     -
//     %%REG      PWG0CRBR    PWG0CRBR           32 8|16|32    32     0x28     R|W      0x0     TRUE     -
//     %%REG      PWG0CTBR    PWG0CTBR        16    8|16|32    32     0x2C     R|W      0x0     TRUE     -
//     %%REG      PWG0TE      PWG0TE        8       8|16|32    32     0x30     R        0x0     TRUE     -
//     %%REG      PWG0TS      PWG0TS        8       8|16|32    32     0x34     R|W      0x0     TRUE     -
//     %%REG      PWG0TT      PWG0TT        8       8|16|32    32     0x38     R|W      0x0     TRUE     -
//     %%REG      PWG0FOT     PWG0FOT       8       8|16|32    32     0x3C     R|W      0x0     TRUE     -
// 
// %REG_NAME PWG0CSDR
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    CSDR               15      0       0       R|W     TRUE        W
// 
// %REG_NAME PWG0CRDR
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    CRDR               16      0       0       R|W     TRUE        -
// 
// %REG_NAME PWG0CTDR
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    CTDR               15      0       0       R|W     TRUE        -
// 
// %REG_NAME PWG0RDT
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    RDTT               1       1       0       R|W     TRUE        -
//     %%BIT    RDT                0       0       0       R|W     TRUE        W
// 
// %REG_NAME PWG0ST
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    FOF                8       8       0       R|W     TRUE        -
//     %%BIT    RSFT               1       1       0       R|W     TRUE        -
//     %%BIT    RSF                0       0       0       R|W     TRUE        W
// 
// %REG_NAME PWG0CNT
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    CNT               15      0       0xFFFF   R|W     TRUE        W
// 
// %REG_NAME PWG0TCR
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    TOE                0       0       1       R|W     TRUE        W
// 
// %REG_NAME PWG0TCBR
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    TOBE               0       0       1       R|W     TRUE        W
// 
// %REG_NAME PWG0CTL
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    TFIE               15      15      1       R|W     TRUE        -
//     %%BIT    TRIE               14      14      0       R|W     TRUE        -
//     %%BIT    FOS                8       8       0       R|W     TRUE        -
//     %%BIT    OCL                7       7       0       R|W     TRUE        -
//     %%BIT    TCUT               5       4       0       R|W     TRUE        -
//     %%BIT    CKS                1       0       0       R|W     TRUE        W
// 
// %REG_NAME PWG0CSBR
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    CSBR               15      0       0       R|W     TRUE        W
// 
// %REG_NAME PWG0CRBR
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    CRBR               16      0       0       R|W     TRUE        W
// 
// %REG_NAME PWG0CTBR
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    CTBR               15      0       0       R|W     TRUE        W
// 
// %REG_NAME PWG0TE
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    TE                 0       0       0       R       TRUE        -
// 
// %REG_NAME PWG0TS
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    TS                 0       0       0       W       TRUE        W
// 
// %REG_NAME PWG0TT
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    TT                 0       0       0       W       TRUE        W
// 
// %REG_NAME PWG0FOT
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    FOT                0       0       0       W       TRUE        W
// 
////////////////////////////////////////////////////////////////////////////////
/// @file pwgenc0_regif.h
/// @brief Register IF class of model PWGENC0
/// $Id: pwgenc0_regif.h,v 1.3 2020/01/15 13:23:55 landang Exp $
/// $Date: 2020/01/15 13:23:55 $
/// $Revison$
/// $Author: landang $
////////////////////////////////////////////////////////////////////////////////
#ifndef __PWGENC0_REGIF_H__
#define __PWGENC0_REGIF_H__
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

/// Register IF class of PWGENC0 model
class Cpwgenc0_regif
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
    Cpwgenc0_regif(std::string name, uint buswidth);
    ~Cpwgenc0_regif();

protected:
    vpcl::re_register *PWG0CSDR;
    vpcl::re_register *PWG0CRDR;
    vpcl::re_register *PWG0CTDR;
    vpcl::re_register *PWG0RDT ;
    vpcl::re_register *PWG0ST  ;
    vpcl::re_register *PWG0CNT ;
    vpcl::re_register *PWG0TCR ;
    vpcl::re_register *PWG0TCBR;
    vpcl::re_register *PWG0CTL ;
    vpcl::re_register *PWG0CSBR;
    vpcl::re_register *PWG0CRBR;
    vpcl::re_register *PWG0CTBR;
    vpcl::re_register *PWG0TE  ;
    vpcl::re_register *PWG0TS  ;
    vpcl::re_register *PWG0TT  ;
    vpcl::re_register *PWG0FOT ;

    #ifdef USR_CWR_SYSTEMC
    scml2::memory<uint> cwmem;
    scml2::reg<uint> *CW_PWG0CSDR;
    scml2::reg<uint> *CW_PWG0CRDR;
    scml2::reg<uint> *CW_PWG0CTDR;
    scml2::reg<uint> *CW_PWG0RDT ;
    scml2::reg<uint> *CW_PWG0ST  ;
    scml2::reg<uint> *CW_PWG0CNT ;
    scml2::reg<uint> *CW_PWG0TCR ;
    scml2::reg<uint> *CW_PWG0TCBR;
    scml2::reg<uint> *CW_PWG0CTL ;
    scml2::reg<uint> *CW_PWG0CSBR;
    scml2::reg<uint> *CW_PWG0CRBR;
    scml2::reg<uint> *CW_PWG0CTBR;
    scml2::reg<uint> *CW_PWG0TE  ;
    scml2::reg<uint> *CW_PWG0TS  ;
    scml2::reg<uint> *CW_PWG0TT  ;
    scml2::reg<uint> *CW_PWG0FOT ;
    #endif

    uint PWG0CSDR_CSDR;
    uint PWG0CRDR_CRDR;
    uint PWG0CTDR_CTDR;
    uint PWG0RDT_RDTT;
    uint PWG0RDT_RDT;
    uint PWG0ST_FOF;
    uint PWG0ST_RSFT;
    uint PWG0ST_RSF;
    uint PWG0CNT_CNT;
    uint PWG0TCR_TOE;
    uint PWG0TCBR_TOBE;
    uint PWG0CTL_TFIE;
    uint PWG0CTL_TRIE;
    uint PWG0CTL_FOS;
    uint PWG0CTL_OCL;
    uint PWG0CTL_TCUT;
    uint PWG0CTL_CKS;
    uint PWG0CSBR_CSBR;
    uint PWG0CRBR_CRBR;
    uint PWG0CTBR_CTBR;
    uint PWG0TE_TE;
    uint PWG0TS_TS;
    uint PWG0TT_TT;
    uint PWG0FOT_FOT;

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

    virtual void cb_PWG0CSDR_CSDR(RegCBstr str) = 0;
    virtual void cb_PWG0RDT_RDT(RegCBstr str) = 0;
    virtual void cb_PWG0ST_RSF(RegCBstr str) = 0;
    virtual void cb_PWG0CNT_CNT(RegCBstr str) = 0;
    virtual void cb_PWG0TCR_TOE(RegCBstr str) = 0;
    virtual void cb_PWG0TCBR_TOBE(RegCBstr str) = 0;
    virtual void cb_PWG0CTL_CKS(RegCBstr str) = 0;
    virtual void cb_PWG0CSBR_CSBR(RegCBstr str) = 0;
    virtual void cb_PWG0CRBR_CRBR(RegCBstr str) = 0;
    virtual void cb_PWG0CTBR_CTBR(RegCBstr str) = 0;
    virtual void cb_PWG0TS_TS(RegCBstr str) = 0;
    virtual void cb_PWG0TT_TT(RegCBstr str) = 0;
    virtual void cb_PWG0FOT_FOT(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Cpwgenc0_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Cpwgenc0_regif::*) (RegCBstr)> > mRdCbAPI;
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
#endif //__PWGENC0_REG_DEF_H__
