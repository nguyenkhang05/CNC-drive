// -----------------------------------------------------------------------------
// $Id: intc2g_u2_regif.h,v 1.7 2020/03/20 02:14:26 synguyen Exp $
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
// Input file : input/intc2g/intc2g_u2_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE INTC2G_U2
// 
// %%REG_INSTANCE  reg_def 14
// 
// %REG_CHANNEL reg_def
//     %%TITLE     name                reg_name           wsize     rsize     length      offset      access      init           factor_start    factor_end      factor_step     support     callback
//     %%REG       INTC2GOVFCLR        INTC2GOVFCLR       8|32      8|16|32   32          0x0         R|W1:0      0x00000000     -               -               -               TRUE        W
//     %%REG       INTC2GOVFSTAT       INTC2GOVFSTAT      -         8|16|32   32          0x4         R           0x00000000     -               -               -               TRUE        -
//     %%REG       INTC2GERRADDR       INTC2GERRADDR      -         8|16|32   32          0x8         R           0x00000000     -               -               -               TRUE        -
//     %%REG       INTC2GERRTYPE       INTC2GERRTYPE      -         8|16|32   32          0xC         R           0x00000000     -               -               -               TRUE        -
//     %%REG       INTC2GSPIDERRCLR    INTC2GSPIDERRCLR   32        8|16|32   32          0x10        W1:0        0x00000000     -               -               -               TRUE        W
//     %%REG       INTC2GSPIDERRSTAT   INTC2GSPIDERRSTAT  -         8|16|32   32          0x14        R           0x00000000     -               -               -               TRUE        -
//     %%REG       INTC2GKCPROT        INTC2GKCPROT       32        8|16|32   32          0x18        R|W         0x00000000     -               -               -               TRUE        W
//     %%REG       INTC2GMPID          INTC2GMPID         8|32      8|16|32   32          0x40        R|W         0x00000000     0               15              -               TRUE        -
//     %%REG       INTC2GPROT_GR       INTC2GPROT_GR      32        8|16|32   32          0xF0        R|W         0x00000000     -               -               -               TRUE        -
//     %%REG       INTC2GPROT_IMR      INTC2GPROT_IMR     32        8|16|32   32          0xF4        R|W         0x00000000     -               -               -               TRUE        -
//     %%REG       INTC2GPROT_         INTC2GPROT_        32        8|16|32   32          0x100       R|W         0x00000000     32              2047            0x04            TRUE        -
//     
// 
// 
// %REG_NAME INTC2GOVFCLR
//     %%TITLE     name        upper   lower   init    access  support     callback
//     %%BIT       CLRO        1       1       0       R|W1:0  TRUE        W
// 
// %REG_NAME INTC2GOVFSTAT
//     %%TITLE     name        upper   lower   init    access  support     callback
//     %%BIT       OVF         1       1       0       R       TRUE        -
// 
// %REG_NAME INTC2GERRADDR
//     %%TITLE     name        upper   lower   init    access  support     callback
//     %%BIT       ADDR        31      0       0       R       TRUE        -
// 
// %REG_NAME INTC2GERRTYPE
//     %%TITLE     name        upper   lower   init    access  support     callback
//     %%BIT       SEC         13      13      0       R       TRUE        -
//     %%BIT       DBG         12      12      0       R       TRUE        -
//     %%BIT       UM          11      11      0       R       TRUE        -
//     %%BIT       SPID        10      6       0       R       TRUE        -
//     %%BIT       WRITE       0       0       0       R       TRUE        -
//     
// %REG_NAME INTC2GSPIDERRCLR
//     %%TITLE     name        upper   lower   init    access  support     callback
//     %%BIT       SPIDCLR     31       0       0       W1:0      TRUE        W
// 
// %REG_NAME INTC2GSPIDERRSTAT
//     %%TITLE     name        upper   lower   init    access  support     callback
//     %%BIT       SPIDERR     31       0       0       R      TRUE        -
// 
// 
// %REG_NAME INTC2GKCPROT
//     %%TITLE     name        upper   lower   init    access  support     callback
//     %%BIT       KCPROT      31      1       0       R|W     TRUE        W
//     %%BIT       KCE         0       0       0       R|W     TRUE        -
// 
// %REG_NAME INTC2GMPID
//     %%TITLE     name        upper   lower   init    access  support     callback
//     %%BIT       SPID        4       0       0       R|W     TRUE        W|R
// 
// %REG_NAME INTC2GPROT_GR
//     %%TITLE     name        upper   lower   init    access  support     callback
//     %%BIT       MPID        31      16      0       R|W     TRUE        W
//     %%BIT       GEN         8       8       0       R|W     TRUE        -
//     %%BIT       DBG         6       6       0       R|W     TRUE        -
//     %%BIT       UM          4       4       0       R|W     TRUE        -
//     %%BIT       WG          1       1       0       R|W     TRUE        -
//     %%BIT       RG          0       0       0       R|W     TRUE        -
// 
// %REG_NAME INTC2GPROT_IMR
//     %%TITLE     name        upper   lower   init    access  support     callback
//     %%BIT       MPID        31      16      0       R|W     TRUE        W
//     %%BIT       GEN         8       8       0       R|W     TRUE        -
//     %%BIT       DBG         6       6       0       R|W     TRUE        -
//     %%BIT       UM          4       4       0       R|W     TRUE        -
//     %%BIT       WG          1       1       0       R|W     TRUE        -
//     %%BIT       RG          0       0       0       R|W     TRUE        -
// 
// %REG_NAME INTC2GPROT_
//     %%TITLE     name        upper   lower   init    access  support     callback
//     %%BIT       MPID        31      16      0       R|W     TRUE        W
//     %%BIT       GEN         8       8       0       R|W     TRUE        -
//     %%BIT       DBG         6       6       0       R|W     TRUE        -
//     %%BIT       UM          4       4       0       R|W     TRUE        -
//     %%BIT       WG          1       1       0       R|W     TRUE        -
//     %%BIT       RG          0       0       0       R|W     TRUE        -
////////////////////////////////////////////////////////////////////////////////
/// @file intc2g_u2_regif.h
/// @brief Register IF class of model INTC2G_U2
/// $Id: intc2g_u2_regif.h,v 1.7 2020/03/20 02:14:26 synguyen Exp $
/// $Date: 2020/03/20 02:14:26 $
/// $Revison$
/// $Author: synguyen $
////////////////////////////////////////////////////////////////////////////////
#ifndef __INTC2G_U2_REGIF_H__
#define __INTC2G_U2_REGIF_H__
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

/// Register IF class of INTC2G_U2 model
class Cintc2g_u2_regif
: public vpcl::reg_super
{
protected:
    typedef const unsigned int cuint;
    typedef unsigned int uint;

    enum eRegGroup {
        emNum_of_gr
    }; ///< Enumeration for register group index
    enum eRegIndex {
        emNUM_INTC2GMPID        = 16,
        emNUM_INTC2GPROT_       = 2016
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
    uint mNumOfCh;

public:
    Cintc2g_u2_regif(std::string name, uint buswidth, uint numOfCh);
    ~Cintc2g_u2_regif();

protected:
    vpcl::re_register *INTC2GOVFCLR     ;
    vpcl::re_register *INTC2GOVFSTAT    ;
    vpcl::re_register *INTC2GERRADDR    ;
    vpcl::re_register *INTC2GERRTYPE    ;
    vpcl::re_register *INTC2GSPIDERRCLR ;
    vpcl::re_register *INTC2GSPIDERRSTAT;
    vpcl::re_register *INTC2GKCPROT     ;
    vpcl::re_register *INTC2GMPID       [16];
    vpcl::re_register *INTC2GPROT_GR    ;
    vpcl::re_register *INTC2GPROT_IMR   ;
    vpcl::re_register *INTC2GPROT_      [2048];

    #ifdef USR_CWR_SYSTEMC
    scml2::memory<uint> cwmem;
    scml2::reg<uint> *CW_INTC2GOVFCLR     ;
    scml2::reg<uint> *CW_INTC2GOVFSTAT    ;
    scml2::reg<uint> *CW_INTC2GERRADDR    ;
    scml2::reg<uint> *CW_INTC2GERRTYPE    ;
    scml2::reg<uint> *CW_INTC2GSPIDERRCLR ;
    scml2::reg<uint> *CW_INTC2GSPIDERRSTAT;
    scml2::reg<uint> *CW_INTC2GKCPROT     ;
    scml2::reg<uint> *CW_INTC2GMPID       [16];
    scml2::reg<uint> *CW_INTC2GPROT_GR    ;
    scml2::reg<uint> *CW_INTC2GPROT_IMR   ;
    scml2::reg<uint> *CW_INTC2GPROT_      [2048];
    #endif

    uint INTC2GOVFCLR_CLRO;
    uint INTC2GOVFSTAT_OVF;
    uint INTC2GERRADDR_ADDR;
    uint INTC2GERRTYPE_SEC;
    uint INTC2GERRTYPE_DBG;
    uint INTC2GERRTYPE_UM ;
    uint INTC2GERRTYPE_SPID;
    uint INTC2GERRTYPE_WRITE;
    uint INTC2GSPIDERRCLR_SPIDCLR;
    uint INTC2GSPIDERRSTAT_SPIDERR;
    uint INTC2GKCPROT_KCPROT;
    uint INTC2GKCPROT_KCE ;
    uint INTC2GMPID_SPID  [16];
    uint INTC2GPROT_GR_MPID;
    uint INTC2GPROT_GR_GEN;
    uint INTC2GPROT_GR_DBG;
    uint INTC2GPROT_GR_UM ;
    uint INTC2GPROT_GR_WG ;
    uint INTC2GPROT_GR_RG ;
    uint INTC2GPROT_IMR_MPID;
    uint INTC2GPROT_IMR_GEN;
    uint INTC2GPROT_IMR_DBG;
    uint INTC2GPROT_IMR_UM;
    uint INTC2GPROT_IMR_WG;
    uint INTC2GPROT_IMR_RG;
    uint INTC2GPROT__MPID [2048];
    uint INTC2GPROT__GEN  [2048];
    uint INTC2GPROT__DBG  [2048];
    uint INTC2GPROT__UM   [2048];
    uint INTC2GPROT__WG   [2048];
    uint INTC2GPROT__RG   [2048];

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

    virtual void cb_INTC2GOVFCLR_CLRO(RegCBstr str) = 0;
    virtual void cb_INTC2GSPIDERRCLR_SPIDCLR(RegCBstr str) = 0;
    virtual void cb_INTC2GKCPROT_KCPROT(RegCBstr str) = 0;
    virtual void cb_INTC2GMPID_SPID(RegCBstr str) = 0;
    virtual void cb_INTC2GPROT_GR_MPID(RegCBstr str) = 0;
    virtual void cb_INTC2GPROT_IMR_MPID(RegCBstr str) = 0;
    virtual void cb_INTC2GPROT__MPID(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Cintc2g_u2_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Cintc2g_u2_regif::*) (RegCBstr)> > mRdCbAPI;
    std::string mFileName;
    std::string mInstName;
    int mLineNum;
    uint mFactorIndexINTC2GPROT_[emNUM_INTC2GPROT_];

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
#endif //__INTC2G_U2_REG_DEF_H__
