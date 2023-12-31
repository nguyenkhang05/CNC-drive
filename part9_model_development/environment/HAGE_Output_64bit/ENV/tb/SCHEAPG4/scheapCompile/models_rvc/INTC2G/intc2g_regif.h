// -----------------------------------------------------------------------------
// $Id: intc2g_regif.h,v 1.3 2019/09/19 10:16:17 ngathunguyen Exp $
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
// Input file : input/intc2g/intc2g_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE INTC2G
//     # the INTC2G registers with base addresses
//     #    32'hFFC6_4000
//     # consider these part as a continous range, the range of the whole INTC2G
//     # becomes from 32'hFFC6_4000 to 32'hFFC6_48FF, with base address 
//     # is 32'hFFC6_4000 and offset part specifies the range from 0x000 to 0xB60
//     # size of offset part should be 12 bit
//     
//     #                   name            offset_size
//     %%REG_INSTANCE      reg_def         14
//     %REG_CHANNEL reg_def
//     
// %%TITLE    name               reg_name              wsize  rsize      length    offset    factor_start    factor_end    factor_index    factor_step    access     init             support     callback
// %%REG      INTC2GKCPROT       INTC2GKCPROT          32     8|16|32    32        0x000     -               -             -               -              R|W        0x00000000       TRUE         W
// %%REG      INTC2GERRCLR       INTC2GERRCLR          32     8|16|32    32        0x004     -               -             -               -              R|W1:0     0x00000000       TRUE         W
// %%REG      INTC2GERRSTAT      INTC2GERRSTAT          -     8|16|32    32        0x008     -               -             -               -              R          0x00000000       TRUE         -
// %%REG      INTC2GERRADDR      INTC2GERRADDR          -     8|16|32    32        0x00C     -               -             -               -              R          0x00000000       TRUE         -
// %%REG      INTC2GERRTYPE      INTC2GERRTYPE          -     8|16|32    32        0x010     -               -             -               -              R          0x00000000       TRUE         -
// %%REG      INTC2GMPID         INTC2GMPID          8|32     8|16|32    32        0x040     0               7             -               0x04           R|W        0x00000000       TRUE         W
// %%REG      INTC2GPROT_GR      INTC2GPROT_GR         32     8|16|32    32        0x0F0     -               -             -               -              R|W        0x00000000       TRUE         W
// %%REG      INTC2GPROT         INTC2GPROT            32     8|16|32    32        0x180     32              2048          -               0x04           R|W        0x00000000       TRUE         W
// 
// #-------------------------------------------------------------------------------
// # Guard error clear register
// #-------------------------------------------------------------------------------
// %REG_NAME INTC2GERRCLR
// %%TITLE    name    upper    lower    init    access    support    callback
// %%BIT      CLRE    0        0        0        W1:0      TRUE        W
// %%BIT      CLRO    1        1        0        W1:0      TRUE        -
// 
// #-------------------------------------------------------------------------------
// #  Guard error status register
// #-------------------------------------------------------------------------------
// %REG_NAME INTC2GERRSTAT
// %%TITLE    name    upper    lower    init    access    support    callback
// %%BIT      ERR     0        0        0       R         TRUE       -
// %%BIT      OVF     1        1        0       R         TRUE       -
// 
// #-------------------------------------------------------------------------------
// #  Guard error address register
// #-------------------------------------------------------------------------------
// %REG_NAME INTC2GERRADDR
// %%TITLE    name    upper    lower    init    access    support    callback
// %%BIT      ADDR    31       0        0       R         TRUE       -
// 
// #-------------------------------------------------------------------------------
// #  Guard error access information register
// #-------------------------------------------------------------------------------
// %REG_NAME INTC2GERRTYPE
// %%TITLE    name    upper    lower    init    access    support    callback
// %%BIT      WRITE   0        0        0       R         TRUE       -
// %%BIT      SPID    10       6        0       R         TRUE       -
// %%BIT      UM      11       11       0       R         TRUE       -
// %%BIT      DBG     12       12       0       R         TRUE       -
// %%BIT      SEC     13       13       0       R         TRUE       -
// 
// #-------------------------------------------------------------------------------
// #  Key code protection register
// #-------------------------------------------------------------------------------
// %REG_NAME INTC2GKCPROT
// %%TITLE    name    upper    lower    init    access    support    callback
// %%BIT      KCE     0        0        0       R|W       TRUE        W
// %%BIT      KCPROT  31       1        0       W1:0      TRUE        -
// 
// #-------------------------------------------------------------------------------
// #  SPID Setting Register 
// #-------------------------------------------------------------------------------
// %REG_NAME INTC2GMPID
// %%TITLE    name    upper    lower    init    access    support    callback
// %%BIT      SPID    4        0        0       R|W        TRUE        W
// 
// 
// #-------------------------------------------------------------------------------
// #  Channel protection control register
// #-------------------------------------------------------------------------------
// %REG_NAME INTC2GPROT
// %%TITLE    name    upper    lower    init    access    support    callback
// %%BIT      RG      0        0        0       R|W       TRUE       W
// %%BIT      WG      1        1        0       R|W       TRUE       -
// %%BIT      UM      4        4        0       R|W       TRUE       -
// %%BIT      DBG     6        6        0       R|W       TRUE       -
// %%BIT      GEN     8        8        0       R|W       TRUE       -
// %%BIT      MPID   23       16        0       R|W       TRUE       -
// 
// #-------------------------------------------------------------------------------
// #  Channel protection control register
// #-------------------------------------------------------------------------------
// %REG_NAME INTC2GPROT_GR
// %%TITLE    name    upper    lower    init    access    support    callback
// %%BIT      RG      0        0        0       R|W       TRUE       W
// %%BIT      WG      1        1        0       R|W       TRUE       -
// %%BIT      UM      4        4        0       R|W       TRUE       -
// %%BIT      DBG     6        6        0       R|W       TRUE       -
// %%BIT      GEN     8        8        0       R|W       TRUE       -
// %%BIT      MPID   23       16        0       R|W       TRUE       -
// 
// #######################################################################################################################################################
// # vim:nowrap
// #######################################################################################################################################################
////////////////////////////////////////////////////////////////////////////////
/// @file intc2g_regif.h
/// @brief Register IF class of model INTC2G
/// $Id: intc2g_regif.h,v 1.3 2019/09/19 10:16:17 ngathunguyen Exp $
/// $Date: 2019/09/19 10:16:17 $
/// $Revison$
/// $Author: ngathunguyen $
////////////////////////////////////////////////////////////////////////////////
#ifndef __INTC2G_REGIF_H__
#define __INTC2G_REGIF_H__
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

/// Register IF class of INTC2G model
class Cintc2g_regif
: public vpcl::reg_super
{
protected:
    typedef const unsigned int cuint;
    typedef unsigned int uint;

    enum eRegGroup {
        emNum_of_gr
    }; ///< Enumeration for register group index
    enum eRegIndex {
        emNUM_INTC2GMPID    = 8,
        emNUM_INTC2GPROT    = 2017
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
    Cintc2g_regif(std::string name, uint buswidth);
    ~Cintc2g_regif();

protected:
    vpcl::re_register *INTC2GKCPROT ;
    vpcl::re_register *INTC2GERRCLR ;
    vpcl::re_register *INTC2GERRSTAT;
    vpcl::re_register *INTC2GERRADDR;
    vpcl::re_register *INTC2GERRTYPE;
    vpcl::re_register *INTC2GMPID   [8];
    vpcl::re_register *INTC2GPROT_GR;
    vpcl::re_register *INTC2GPROT   [2049];

    #ifdef CWR_SYSTEMC
    scml2::memory<uint> cwmem;
    scml2::reg<uint> *CW_INTC2GKCPROT ;
    scml2::reg<uint> *CW_INTC2GERRCLR ;
    scml2::reg<uint> *CW_INTC2GERRSTAT;
    scml2::reg<uint> *CW_INTC2GERRADDR;
    scml2::reg<uint> *CW_INTC2GERRTYPE;
    scml2::reg<uint> *CW_INTC2GMPID   [8];
    scml2::reg<uint> *CW_INTC2GPROT_GR;
    scml2::reg<uint> *CW_INTC2GPROT   [2049];
    #endif

    uint INTC2GKCPROT_KCE;
    uint INTC2GKCPROT_KCPROT;
    uint INTC2GERRCLR_CLRE;
    uint INTC2GERRCLR_CLRO;
    uint INTC2GERRSTAT_ERR;
    uint INTC2GERRSTAT_OVF;
    uint INTC2GERRADDR_ADDR;
    uint INTC2GERRTYPE_WRITE;
    uint INTC2GERRTYPE_SPID;
    uint INTC2GERRTYPE_UM;
    uint INTC2GERRTYPE_DBG;
    uint INTC2GERRTYPE_SEC;
    uint INTC2GMPID_SPID [8];
    uint INTC2GPROT_GR_RG;
    uint INTC2GPROT_GR_WG;
    uint INTC2GPROT_GR_UM;
    uint INTC2GPROT_GR_DBG;
    uint INTC2GPROT_GR_GEN;
    uint INTC2GPROT_GR_MPID;
    uint INTC2GPROT_RG   [2049];
    uint INTC2GPROT_WG   [2049];
    uint INTC2GPROT_UM   [2049];
    uint INTC2GPROT_DBG  [2049];
    uint INTC2GPROT_GEN  [2049];
    uint INTC2GPROT_MPID [2049];

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

    virtual void cb_INTC2GKCPROT_KCE(RegCBstr str) = 0;
    virtual void cb_INTC2GERRCLR_CLRE(RegCBstr str) = 0;
    virtual void cb_INTC2GMPID_SPID(RegCBstr str) = 0;
    virtual void cb_INTC2GPROT_GR_RG(RegCBstr str) = 0;
    virtual void cb_INTC2GPROT_RG(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Cintc2g_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Cintc2g_regif::*) (RegCBstr)> > mRdCbAPI;
    std::string mFileName;
    std::string mInstName;
    int mLineNum;
    uint mFactorIndexINTC2GPROT[emNUM_INTC2GPROT];

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
#endif //__INTC2G_REG_DEF_H__
