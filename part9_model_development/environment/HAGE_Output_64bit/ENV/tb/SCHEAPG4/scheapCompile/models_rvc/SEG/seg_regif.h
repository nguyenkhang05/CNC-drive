// -----------------------------------------------------------------------------
// $Id: seg_regif.h,v 1.6 2015/06/12 08:20:40 dungvannguyen Exp $
//
// Copyright(c) 2014-2015 Renesas Electronics Corporation
// Copyright(c) 2014-2015 Renesas Design Vietnam Co., Ltd.
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
// Input file : seg_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE SEG
//     #                   name            offset_size
//     %%REG_INSTANCE      reg_def         4
// 
// %REG_CHANNEL reg_def
//     %%TITLE  group   name       reg_name    wsize     rsize     length  offset    access   init     support  callback
//     %%REG    -       SEGCONT    SEGCONT     16        8|16|32    16      0x0      W|R      0        TRUE     -
//     %%REG    -       SEGFLAG    SEGFLAG     16        8|16|32    16      0x2      W|R      0        TRUE     W|R
//     %%REG    -       SEGTYPE    SEGTYPE     16        8|16|32    16      0x4      W|R      0        TRUE     -
//     %%REG    -       SEGSIDE    SEGSIDE     16        8|16|32    16      0x6      W|R      0        TRUE     -
//     %%REG    -       SEGADDR    SEGADDR     32        8|16|32    32      0x8      W|R      0        TRUE     -
// 
// %REG_NAME SEGCONT
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    NEE         0      0      0     W|R     TRUE     -
//     %%BIT    ICCE        2      2      0     W|R     TRUE     -
//     %%BIT    VCIE        4      4      0     W|R     TRUE     -
//     %%BIT    ROME        5      5      0     W|R     TRUE     -
//     %%BIT    TCME        6      6      0     W|R     TRUE     -
//     %%BIT    VCRE        8      8      0     W|R     TRUE     -
//     %%BIT    VPGE        9      9      0     W|R     TRUE     -
//     %%BIT    VCBE        10     10     0     W|R     TRUE     -
//     %%BIT    AXCE        11     11     0     W|R     TRUE     -
//     %%BIT    SS0E        14     14     0     W|R     TRUE     -
//     %%BIT    SS1E        15     15     0     W|R     TRUE     -
// 
// %REG_NAME SEGFLAG
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    ICCF        2      2      0     W|R     TRUE     W|R
//     %%BIT    VCIF        4      4      0     W|R     TRUE     -
//     %%BIT    ROMF        5      5      0     W|R     TRUE     -
//     %%BIT    TCMF        6      6      0     W|R     TRUE     -
//     %%BIT    VCRF        8      8      0     W|R     TRUE     -
//     %%BIT    VPGF        9      9      0     W|R     TRUE     -
//     %%BIT    VCBF        10     10     0     W|R     TRUE     -
//     %%BIT    AXCF        11     11     0     W|R     TRUE     -
//     %%BIT    SS0F        14     14     0     W|R     TRUE     -
//     %%BIT    SS1F        15     15     0     W|R     TRUE     -
// 
// %REG_NAME SEGTYPE
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    CODE        3      0      0     W|R     TRUE     -
//     %%BIT    RTYP        8      4      0     W|R     TRUE     -
//     %%BIT    ETYP        15     9      0     W|R     TRUE     -
// 
// %REG_NAME SEGSIDE
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    VM          0      0      0     W|R     TRUE     -
//     %%BIT    UM          1      1      0     W|R     TRUE     -
//     %%BIT    SPID        3      2      0     W|R     TRUE     -
//     %%BIT    PEID        6      4      0     W|R     TRUE     -
//     %%BIT    VCID        9      7      0     W|R     TRUE     -
//     %%BIT    TCID        15     10     0     W|R     TRUE     -
// 
// %REG_NAME SEGADDR
//     %%TITLE  name        upper  lower  init  access  support  callback                                     
//     %%BIT    Address     31      0     0     W|R     TRUE     -
////////////////////////////////////////////////////////////////////////////////
/// @file seg_regif.h
/// @brief Register IF class of model SEG
/// $Id: seg_regif.h,v 1.6 2015/06/12 08:20:40 dungvannguyen Exp $
/// $Date: 2015/06/12 08:20:40 $
/// $Revison$
/// $Author: dungvannguyen $
////////////////////////////////////////////////////////////////////////////////
#ifndef __SEG_REGIF_H__
#define __SEG_REGIF_H__
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

/// Register IF class of SEG model
class Cseg_regif
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
    Cseg_regif(std::string name, unsigned int buswidth, std::string seg_version = "G3M_V30");
    ~Cseg_regif();

protected:
    vpcl::re_register *SEGCONT;
    vpcl::re_register *SEGFLAG;
    vpcl::re_register *SEGTYPE;
    vpcl::re_register *SEGSIDE;
    vpcl::re_register *SEGADDR;

    #ifdef CWR_SYSTEMC
    scml2::memory<uint> cwmem;
    scml2::reg<uint>      *CW_SEGCONT_SEGFLAG;
    scml2::reg<uint>      *CW_SEGTYPE_SEGSIDE;
    scml2::bitfield<uint> *CW_SEGCONT;
    scml2::bitfield<uint> *CW_SEGFLAG;
    scml2::bitfield<uint> *CW_SEGTYPE;
    scml2::bitfield<uint> *CW_SEGSIDE;
    scml2::reg<uint>      *CW_SEGADDR;
    #endif

    uint SEGCONT_NEE;
    uint SEGCONT_ICCE;
    uint SEGCONT_VCIE;
    uint SEGCONT_LSUE;//Added for G3MH
    uint SEGCONT_ROME;
    uint SEGCONT_CFBE;//Added for G3MH
    uint SEGCONT_TCME;
    uint SEGCONT_LRME;//Added for G3MH
    uint SEGCONT_GRME;//Added for G3MH
    uint SEGCONT_VCRE;
    uint SEGCONT_HBPE;//Added for G3MH
    uint SEGCONT_IPGE;//Added for G3KH
    uint SEGCONT_VPGE;
    uint SEGCONT_PBPE;//Added for G3MH
    uint SEGCONT_APIE;//Added for G3KH
    uint SEGCONT_VCBE;
    uint SEGCONT_VCSE;//Added for G3KH
    uint SEGCONT_AXCE;
    uint SEGCONT_SS0E;
    uint SEGCONT_SS1E;
    uint SEGFLAG_ICCF;
    uint SEGFLAG_VCIF;
    uint SEGFLAG_LSUF;//Added for G3MH
    uint SEGFLAG_ROMF;
    uint SEGFLAG_CFBF;//Added for G3MH
    uint SEGFLAG_TCMF;
    uint SEGFLAG_LRMF;//Added for G3MH
    uint SEGFLAG_GRMF;//Added for G3MH
    uint SEGFLAG_VCRF;
    uint SEGFLAG_HBPF;//Added for G3MH
    uint SEGFLAG_IPGF;//Added for G3KH
    uint SEGFLAG_VPGF;
    uint SEGFLAG_PBPF;//Added for G3MH
    uint SEGFLAG_APIF;//Added for G3KH
    uint SEGFLAG_VCBF;
    uint SEGFLAG_VCSF;//Added for G3KH
    uint SEGFLAG_AXCF;
    uint SEGFLAG_SS0F;
    uint SEGFLAG_SS1F;
    uint SEGTYPE_CODE;
    uint SEGTYPE_RTYP;
    uint SEGTYPE_RTYPE;//Added for G3KH
    uint SEGTYPE_ETYP;
    uint SEGTYPE_ETYPE;//Added for G3KH
    uint SEGTYPE_TYPE;//Added for G3MH
    uint SEGSIDE_VM;
    uint SEGSIDE_UM;
    uint SEGSIDE_SPID;
    uint SEGSIDE_PEID;
    uint SEGSIDE_VCID;
    uint SEGSIDE_TCID;
    uint SEGADDR_Address;

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

    virtual void cb_SEGFLAG_VCIF(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    std::string mSEGVersion;
    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Cseg_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Cseg_regif::*) (RegCBstr)> > mRdCbAPI;
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
    void InitLocalVal(const bool is_construct);
    void UpdateLocalVal(cuint addr);
    void UpdateRegVal(cuint addr);

    #ifdef CWR_SYSTEMC
    uint cw_rd_cb(tlm::tlm_generic_payload& trans, int tag);
    uint cw_wr_cb(tlm::tlm_generic_payload& trans, int tag);
    void cw_instantiate_reg(scml2::reg<uint> * reg, std::string reg_name, int offset);
    #endif
};
#endif //__SEG_REG_DEF_H__
