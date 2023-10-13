// -----------------------------------------------------------------------------
// $Id: pwseld0_regif.h,v 1.4 2020/01/15 13:23:56 landang Exp $
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
// Input file : input/pwseld0_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE pwseld0
//     #              name     offset_size
//     %%REG_INSTANCE reg_def  11
// 
// %REG_CHANNEL reg_def                                                       
//     %%TITLE    name        reg_name      wsize   rsize      length offset    factor_start    factor_end    factor_index    factor_step   access   init   support  callback
//     %%REG      PWS0CTL     PWS0CTL       8       8|16|32    32     0x00      -               -             -               -             R|W      0x0     TRUE     -
//     %%REG      PWS0STR     PWS0STR       8       8|16|32    32     0x04      -               -             -               -             R        0x0     TRUE     -
//     %%REG      PWS0STC     PWS0STC       8       8|16|32    32     0x08      -               -             -               -             R|W      0x0     TRUE     -
//     %%REG      PWS0EMU     PWS0EMU       8       8|16|32    32     0x0C      -               -             -               -             R|W      0x0     TRUE     -
//     %%REG      PWS0QUE     PWS0QUE       8       8|16|32    32     0x20      0               7             -               4             R        0x7F    TRUE     -
//     %%REG      PWS0PVCR    PWS0PVCR          32  8|16|32    32     0x100     0               95            -               4             R|W      0x0     TRUE     -
//     %%REG      PWS0PWDDIR  PWS0PWDDIR        32  8|16|32    32     0x300     0               95            -               4             R        0x0     TRUE     -
// 
// %REG_NAME PWS0CTL 
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    ARSE               7       7       0       R|W     TRUE        -
//     %%BIT    ENBL               0       0       0       R|W     TRUE        W
// 
// %REG_NAME PWS0STR 
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    QFL                1       1       0       R       TRUE        -
//     %%BIT    QNE                0       0       0       R       TRUE        -
// 
// %REG_NAME PWS0STC 
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    CLFL               1       1       0       R|W     TRUE        -
//     %%BIT    CLNE               0       0       0       R|W     TRUE        W
// 
// %REG_NAME PWS0EMU 
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    SVSDIS             0       0       0       R|W     TRUE        W
// 
// %REG_NAME PWS0QUE 
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    QUE                6       0       0x7F    R       TRUE        -
// 
// %REG_NAME PWS0PVCR
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    WTTSx              19      16      0x0     R|W     TRUE        -
//     %%BIT    MPXEx              15      15      0x0     R|W     TRUE        -
//     %%BIT    MPXVx              14      12      0x0     R|W     TRUE        -
//     %%BIT    VCULLMTBSx         11      8       0x0     R|W     TRUE        -
//     %%BIT    SLADx              7       6       0x0     R|W     TRUE        -
//     %%BIT    GCTRLx             5       0       0x0     R|W     TRUE        W
// 
// %REG_NAME PWS0PWDDIR
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    MP                 31      31      0x0     R       TRUE        -
//     %%BIT    MPXV               30      28      0x0     R       TRUE        -
//     %%BIT    OWE                26      26      0x0     R       TRUE        -
//     %%BIT    WFLG               25      25      0x0     R       TRUE        -
//     %%BIT    ID                 21      16      0x0     R       TRUE        -
//     %%BIT    PWDDR              15      0       0x0     R       TRUE        R
// 
////////////////////////////////////////////////////////////////////////////////
/// @file pwseld0_regif.h
/// @brief Register IF class of model PWSELD0
/// $Id: pwseld0_regif.h,v 1.4 2020/01/15 13:23:56 landang Exp $
/// $Date: 2020/01/15 13:23:56 $
/// $Revison$
/// $Author: landang $
////////////////////////////////////////////////////////////////////////////////
#ifndef __PWSELD0_REGIF_H__
#define __PWSELD0_REGIF_H__
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

/// Register IF class of PWSELD0 model
class Cpwseld0_regif
: public vpcl::reg_super
{
protected:
    typedef const unsigned int cuint;
    typedef unsigned int uint;

    enum eRegGroup {
        emNum_of_gr
    }; ///< Enumeration for register group index
    enum eRegIndex {
        emNUM_PWS0QUE    = 8,
        emNUM_PWS0PVCR   = 96,
        emNUM_PWS0PWDDIR = 96
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
    Cpwseld0_regif(std::string name, uint buswidth);
    ~Cpwseld0_regif();

protected:
    vpcl::re_register *PWS0CTL   ;
    vpcl::re_register *PWS0STR   ;
    vpcl::re_register *PWS0STC   ;
    vpcl::re_register *PWS0EMU   ;
    vpcl::re_register *PWS0QUE   [8];
    vpcl::re_register *PWS0PVCR  [96];
    vpcl::re_register *PWS0PWDDIR[96];

    #ifdef USR_CWR_SYSTEMC
    scml2::memory<uint> cwmem;
    scml2::reg<uint> *CW_PWS0CTL   ;
    scml2::reg<uint> *CW_PWS0STR   ;
    scml2::reg<uint> *CW_PWS0STC   ;
    scml2::reg<uint> *CW_PWS0EMU   ;
    scml2::reg<uint> *CW_PWS0QUE   [8];
    scml2::reg<uint> *CW_PWS0PVCR  [96];
    scml2::reg<uint> *CW_PWS0PWDDIR[96];
    #endif

    uint PWS0CTL_ARSE  ;
    uint PWS0CTL_ENBL  ;
    uint PWS0STR_QFL   ;
    uint PWS0STR_QNE   ;
    uint PWS0STC_CLFL  ;
    uint PWS0STC_CLNE  ;
    uint PWS0EMU_SVSDIS;
    uint PWS0QUE_QUE   [8];
    uint PWS0PVCR_WTTSx[96];
    uint PWS0PVCR_MPXEx[96];
    uint PWS0PVCR_MPXVx[96];
    uint PWS0PVCR_VCULLMTBSx[96];
    uint PWS0PVCR_SLADx[96];
    uint PWS0PVCR_GCTRLx[96];
    uint PWS0PWDDIR_MP [96];
    uint PWS0PWDDIR_MPXV[96];
    uint PWS0PWDDIR_OWE[96];
    uint PWS0PWDDIR_WFLG[96];
    uint PWS0PWDDIR_ID [96];
    uint PWS0PWDDIR_PWDDR[96];

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

    virtual void cb_PWS0CTL_ENBL(RegCBstr str) = 0;
    virtual void cb_PWS0STC_CLNE(RegCBstr str) = 0;
    virtual void cb_PWS0EMU_SVSDIS(RegCBstr str) = 0;
    virtual void cb_PWS0PVCR_GCTRLx(RegCBstr str) = 0;
    virtual void cb_PWS0PWDDIR_PWDDR(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Cpwseld0_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Cpwseld0_regif::*) (RegCBstr)> > mRdCbAPI;
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
#endif //__PWSELD0_REG_DEF_H__