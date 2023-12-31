// -----------------------------------------------------------------------------
// $Id: ibg_u2_regif.h,v 1.2 2020/02/26 06:51:33 duongphan Exp $
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
// Input file : /home/truongvv2/RHSIF/GenSrc/ibg_input/ibg_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE IBG_U2
//     # the IBG_U2 contains 4 parts of registers with base addresses
//     #    32'hFFC6_A000 BARR
//     # consider these part as a continous range, the range of the whole IBG_U2
//     # becomes from 32'hFFC6_A000 to 32'hFFC6_A1FF, with base address 
//     # is 32'hFFC6_A000 and offset part specifies the range from 0x000 to 0x1FF
//     # size of offset part (0x1FF = 9'b111111111) should be 9 bit
//     #                   name            offset_size
//     %%REG_INSTANCE      reg_def         9
// %REG_CHANNEL reg_def
// #######################################################################################################################################################
// #  Module Name  Register Name                               Symbol                  Address                               Access      Access Protection
// #  GUARD_BARR  Guard Error Overflow Clear Register          GOVFCLR                 <GUARD_base> + 00H                    8|32
// #              Guard Error Overflow Status Register         GOVFSTAT                <GUARD_base> + 04H                    8|32
// #              Guard Error Address Register                 GERRADDR                <GUARD_base> + 08H                    32
// #              Guard Error Access Information Register      GERRTYPE                <GUARD_base> + 0CH                    16|32
// #              Guard Error SPID Error Clear Register        GSPIDERRCLR             <GUARD_base> + 10H                    32
// #              Guard Error SPID Error  Status Register      GSPIDERRSTAT            <GUARD_base> + 14H                    32
// #              Guard Key Code Protection Register           GKCPROT                 <GUARD_base> + 18H                    32
// #              Guard Protection Control Register            GPROT0_n (n = 0-16)     <GUARD_base> + 80H  + (n*08H)         8|16|32     GKCPROT
// #              Guard SPID Setting Register                  GPROT1_n (n = 0-16)     <GUARD_base> + 84H  + (n*08H)         8|16|32     GKCPROT
// #######################################################################################################################################################
// %%TITLE    name          reg_name      wsize     rsize      length    offset    factor_start    factor_end    factor_index    factor_step    access     init             support     callback
// %%REG      GOVFCLR       GOVFCLR       8|32      8|16|32    32        0x000     -               -             -               -              R|W1:0     0x00000000       TRUE        -
// %%REG      GOVFSTAT      GOVFSTAT      8|32      8|16|32    32        0x004     -               -             -               -              R          0x00000000       TRUE        -
// %%REG      GERRADDR      GERRADDR      32        8|16|32    32        0x008     -               -             -               -              R          0x00000000       TRUE        -
// %%REG      GERRTYPE      GERRTYPE      16|32     8|16|32    32        0x00C     -               -             -               -              R          0x00000000       TRUE        -
// %%REG      GSPIDERRCLR   GSPIDERRCLR   32        8|16|32    32        0x010     -               -             -               -              R|W1:0     0x00000000       TRUE        -
// %%REG      GSPIDERRSTAT  GSPIDERRSTAT  32        8|16|32    32        0x014     -               -             -               -              R          0x00000000       TRUE        -
// %%REG      GKCPROT       GKCPROT       32        8|16|32    32        0x018     -               -             -               -              R|W        0x00000000       TRUE        -
// %%REG      GPROT0        GPROT0_       8|16|32   8|16|32    32        0x080     0               16            -               0x08           R|W        0x00000000       TRUE        -
// %%REG      GPROT1        GPROT1_       8|16|32   8|16|32    32        0x084     0               16            -               0x08           R|W        0x00000000       TRUE        -
// 
// #-------------------------------------------------------------------------------
// # Guard Error Overflow Clear Register
// #-------------------------------------------------------------------------------
// %REG_NAME GOVFCLR
// %%TITLE    name    upper    lower    init    access    support    callback
// %%BIT      CLRO    1        1        0       R|W1:0    TRUE       W
// 
// #-------------------------------------------------------------------------------
// #  Guard Error Overflow Status Register
// #-------------------------------------------------------------------------------
// %REG_NAME GOVFSTAT
// %%TITLE    name    upper    lower    init    access    support    callback
// %%BIT      OVF     1        1        0       R         TRUE       -
// 
// #-------------------------------------------------------------------------------
// #  Guard Error Address Register
// #-------------------------------------------------------------------------------
// %REG_NAME GERRADDR
// %%TITLE    name    upper    lower    init    access    support    callback
// %%BIT      ADDR    31       0        0       R         TRUE       -
// 
// #-------------------------------------------------------------------------------
// #  Guard Error Access Information Register
// #-------------------------------------------------------------------------------
// %REG_NAME GERRTYPE
// %%TITLE    name    upper    lower    init    access    support    callback
// %%BIT      WRITE   0        0        0       R         TRUE       -
// %%BIT      SPID    10       6        0       R         TRUE       -
// %%BIT      UM      11       11       0       R         TRUE       -
// %%BIT      DBG     12       12       0       R         TRUE       -
// %%BIT      SEC     13       13       0       R         TRUE       -
// 
// #-------------------------------------------------------------------------------
// #  Guard Error SPID Error Clear Register
// #-------------------------------------------------------------------------------
// %REG_NAME GSPIDERRCLR
// %%TITLE    name    upper    lower    init    access    support    callback
// %%BIT      SPIDCLR 31       0        0       R|W1:0    TRUE       W
// 
// #-------------------------------------------------------------------------------
// #  Guard Error SPID Error  Status Register
// #-------------------------------------------------------------------------------
// %REG_NAME GSPIDERRSTAT
// %%TITLE    name    upper    lower    init    access    support    callback
// %%BIT      SPIDERR 31       0        0       R         TRUE       -
// 
// #-------------------------------------------------------------------------------
// #  Guard Key Code Protection Register
// #-------------------------------------------------------------------------------
// %REG_NAME GKCPROT
// %%TITLE    name    upper    lower    init    access    support    callback
// %%BIT      KCE     0        0        0       R|W       TRUE       W
// %%BIT      KCPROT  31       1        0       R|W1:0    TRUE       -
// 
// 
// #-------------------------------------------------------------------------------
// #  Guard Protection Control Register
// #-------------------------------------------------------------------------------
// %REG_NAME GPROT0
// %%TITLE    name    upper    lower    init    access    support    callback
// %%BIT      RG      0        0        0       R|W       TRUE       W
// %%BIT      WG      1        1        0       R|W       TRUE       -
// %%BIT      UM      4        4        0       R|W       TRUE       -
// %%BIT      DBG     6        6        0       R|W       TRUE       -
// %%BIT      GEN     8        8        0       R|W       TRUE       -
// 
// #-------------------------------------------------------------------------------
// #  Guard SPID Setting Register
// #-------------------------------------------------------------------------------
// %REG_NAME GPROT1
// %%TITLE    name    upper    lower    init    access    support    callback
// %%BIT      SPID    31       0        0       R|W       TRUE       W
// 
// #######################################################################################################################################################
// # vim:nowrap
// #######################################################################################################################################################
////////////////////////////////////////////////////////////////////////////////
/// @file ibg_u2_regif.h
/// @brief Register IF class of model IBG_U2
/// $Id: ibg_u2_regif.h,v 1.2 2020/02/26 06:51:33 duongphan Exp $
/// $Date: 2020/02/26 06:51:33 $
/// $Revison$
/// $Author: duongphan $
////////////////////////////////////////////////////////////////////////////////
#ifndef __IBG_U2_REGIF_H__
#define __IBG_U2_REGIF_H__
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

/// Register IF class of IBG_U2 model
class Cibg_u2_regif
: public vpcl::reg_super
{
protected:
    typedef const unsigned int cuint;
    typedef unsigned int uint;

    enum eRegGroup {
        emNum_of_gr
    }; ///< Enumeration for register group index
    enum eRegIndex {
        emNUM_GPROT0_      = 17,
        emNUM_GPROT1_      = 17
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
    uint32_t chanNum;
    std::string strModelName;

public:
    Cibg_u2_regif(std::string name, uint buswidth, uint ichanNum);
    ~Cibg_u2_regif();

protected:
    vpcl::re_register *GOVFCLR     ;
    vpcl::re_register *GOVFSTAT    ;
    vpcl::re_register *GERRADDR    ;
    vpcl::re_register *GERRTYPE    ;
    vpcl::re_register *GSPIDERRCLR ;
    vpcl::re_register *GSPIDERRSTAT;
    vpcl::re_register *GKCPROT     ;
    vpcl::re_register *GPROT0      [17];
    vpcl::re_register *GPROT1      [17];

    #ifdef CWR_SYSTEMC
    scml2::memory<uint> cwmem;
    scml2::reg<uint> *CW_GOVFCLR     ;
    scml2::reg<uint> *CW_GOVFSTAT    ;
    scml2::reg<uint> *CW_GERRADDR    ;
    scml2::reg<uint> *CW_GERRTYPE    ;
    scml2::reg<uint> *CW_GSPIDERRCLR ;
    scml2::reg<uint> *CW_GSPIDERRSTAT;
    scml2::reg<uint> *CW_GKCPROT     ;
    scml2::reg<uint> *CW_GPROT0      [17];
    scml2::reg<uint> *CW_GPROT1      [17];
    #endif

    uint GOVFCLR_CLRO;
    uint GOVFSTAT_OVF;
    uint GERRADDR_ADDR;
    uint GERRTYPE_WRITE;
    uint GERRTYPE_SPID;
    uint GERRTYPE_UM ;
    uint GERRTYPE_DBG;
    uint GERRTYPE_SEC;
    uint GSPIDERRCLR_SPIDCLR;
    uint GSPIDERRSTAT_SPIDERR;
    uint GKCPROT_KCE ;
    uint GKCPROT_KCPROT;
    uint GPROT0_RG   [17];
    uint GPROT0_WG   [17];
    uint GPROT0_UM   [17];
    uint GPROT0_DBG  [17];
    uint GPROT0_GEN  [17];
    uint GPROT1_SPID [17];

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

    virtual void cb_GOVFCLR_CLRO(RegCBstr str) = 0;
    virtual void cb_GSPIDERRCLR_SPIDCLR(RegCBstr str) = 0;
    virtual void cb_GKCPROT_KCE(RegCBstr str) = 0;
    virtual void cb_GPROT0_RG(RegCBstr str) = 0;
    virtual void cb_GPROT1_SPID(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Cibg_u2_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Cibg_u2_regif::*) (RegCBstr)> > mRdCbAPI;
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
#endif //__IBG_U2_REG_DEF_H__
