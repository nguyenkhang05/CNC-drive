// -----------------------------------------------------------------------------
// $Id: mecccap_dts_regif.h,v 1.2 2019/03/05 04:17:12 synguyen Exp $
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
// Input file : mecccap_dts_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE mecccap_dts
//     #              name     offset_size
//     %%REG_INSTANCE reg_def  8 
// 
// %REG_CHANNEL reg_def                                                       
//     %%TITLE    name                           reg_name                     wsize   rsize      length offset  factor_start    factor_end  factor_index    factor_step access   init   support  callback
//     %%REG      DR_ERRINT                     DR_ERRINT                   8|16|32 8|16|32      32     0x00    -               -           -               -           R/W      0x0     TRUE     -
//     %%REG      DR_SSTCLR                     DR_SSTCLR                   8|16|32 8|16|32      32     0x10    -               -           -               -           W        0x0     TRUE     -
//     %%REG      DR_DSTCLR                     DR_DSTCLR                   8|16|32 8|16|32      32     0x14    -               -           -               -           W        0x0     TRUE     -
//     %%REG      DR_OVFCLR                     DR_OVFCLR                   8|16|32 8|16|32      32     0x18    -               -           -               -           W        0x0     TRUE     -
//     %%REG      DR_SERSTR                     DR_SERSTR                   -       8|16|32      32     0x20    -               -           -               -           R        0x0     TRUE     -
//     %%REG      DR_DERSTR                     DR_DERSTR                   8|16|32 8|16|32      32     0x24    -               -           -               -           R        0x0     TRUE     -
//     %%REG      DR_OVFSTR                     DR_OVFSTR                   8|16|32 8|16|32      32     0x28    -               -           -               -           R        0x0     TRUE     -
//     %%REG      DR_SERINF                     DR_SERINF                   8|16|32 8|16|32      32     0x30    -               -           -               -           R        0x0     TRUE     -
//     %%REG      DR_nSEADR                     DR_nSEADR                   -       8|16|32      32     0x70    0               31          -               4           R        0x0     TRUE     -
//     %%REG      DR_00DEADR                    DR_00DEADR                  -       8|16|32      32     0xF0    -               -           -               -           R        0x0     TRUE     -
//     
// %REG_NAME DR_ERRINT                                                           
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    SEOVFIE            7        7      1       R|W     TRUE        -
//     %%BIT    AFEIE              3        3      1       R|W     TRUE        -
//     %%BIT    DEDIE              1        1      1       R|W     TRUE        -
//     %%BIT    SEDIE              0        0      1       R|W     TRUE        -
// 
// %REG_NAME DR_SSTCLR                                                           
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    SSTCLR00           0        0      0       W       TRUE        W
// 
// %REG_NAME DR_DSTCLR                                                           
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    DSTCLR00           0        0      0       W       TRUE        W
// 
// %REG_NAME DR_OVFCLR                                                           
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    SERROVFCLR1        1        1      0       W       TRUE        -
//     %%BIT    SERROVFCLR0        0        0      0       W       TRUE        W
// 
// %REG_NAME DR_SERSTR                                                           
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    SEDF00             0        0      0       R       TRUE        -
// 
// %REG_NAME DR_DERSTR                                                           
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    AFEF00             3        3      0       R       TRUE        -
//     %%BIT    DEDF00             1        1      0       R       TRUE        -
// 
// %REG_NAME DR_OVFSTR                                                           
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    SERROVF1           1        1      0       R       TRUE        -
//     %%BIT    SERROVF0           0        0      0       R       TRUE        -
// 
// %REG_NAME DR_SERINF                                                           
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    SEDLINF00          0        0      0       R       TRUE        -
// 
// %REG_NAME DR_nSEADR                                                           
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    SEDL               31       27     0       R       TRUE        -
//     %%BIT    SEADR0             26       2      0       R       TRUE        -
// 
// %REG_NAME DR_00DEADR                                                          
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    DEDL               31       27     0       R       TRUE        -
//     %%BIT    DEADR0             26       2      0       R       TRUE        -
////////////////////////////////////////////////////////////////////////////////
/// @file mecccap_dts_regif.h
/// @brief Register IF class of model MECCCAP_DTS
/// $Id: mecccap_dts_regif.h,v 1.2 2019/03/05 04:17:12 synguyen Exp $
/// $Date: 2019/03/05 04:17:12 $
/// $Revison$
/// $Author: synguyen $
////////////////////////////////////////////////////////////////////////////////
#ifndef __MECCCAP_DTS_REGIF_H__
#define __MECCCAP_DTS_REGIF_H__
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

/// Register IF class of MECCCAP_DTS model
class Cmecccap_dts_regif
: public vpcl::reg_super
{
protected:
    typedef const unsigned int cuint;
    typedef unsigned int uint;

    enum eRegGroup {
        emNum_of_gr
    }; ///< Enumeration for register group index
    enum eRegIndex {
        emNUM_DR_nSEADR  = 32
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
    Cmecccap_dts_regif(std::string name, uint buswidth);
    ~Cmecccap_dts_regif();

protected:
    vpcl::re_register *DR_ERRINT ;
    vpcl::re_register *DR_SSTCLR ;
    vpcl::re_register *DR_DSTCLR ;
    vpcl::re_register *DR_OVFCLR ;
    vpcl::re_register *DR_SERSTR ;
    vpcl::re_register *DR_DERSTR ;
    vpcl::re_register *DR_OVFSTR ;
    vpcl::re_register *DR_SERINF ;
    vpcl::re_register *DR_nSEADR [32];
    vpcl::re_register *DR_00DEADR;

    #ifdef CWR_SYSTEMC
    scml2::memory<uint> cwmem;
    scml2::reg<uint> *CW_DR_ERRINT ;
    scml2::reg<uint> *CW_DR_SSTCLR ;
    scml2::reg<uint> *CW_DR_DSTCLR ;
    scml2::reg<uint> *CW_DR_OVFCLR ;
    scml2::reg<uint> *CW_DR_SERSTR ;
    scml2::reg<uint> *CW_DR_DERSTR ;
    scml2::reg<uint> *CW_DR_OVFSTR ;
    scml2::reg<uint> *CW_DR_SERINF ;
    scml2::reg<uint> *CW_DR_nSEADR [32];
    scml2::reg<uint> *CW_DR_00DEADR;
    #endif

    uint DR_ERRINT_SEOVFIE;
    uint DR_ERRINT_AFEIE;
    uint DR_ERRINT_DEDIE;
    uint DR_ERRINT_SEDIE;
    uint DR_SSTCLR_SSTCLR00;
    uint DR_DSTCLR_DSTCLR00;
    uint DR_OVFCLR_SERROVFCLR1;
    uint DR_OVFCLR_SERROVFCLR0;
    uint DR_SERSTR_SEDF00;
    uint DR_DERSTR_AFEF00;
    uint DR_DERSTR_DEDF00;
    uint DR_OVFSTR_SERROVF1;
    uint DR_OVFSTR_SERROVF0;
    uint DR_SERINF_SEDLINF00;
    uint DR_nSEADR_SEDL[32];
    uint DR_nSEADR_SEADR0[32];
    uint DR_00DEADR_DEDL;
    uint DR_00DEADR_DEADR0;

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

    virtual void cb_DR_SSTCLR_SSTCLR00(RegCBstr str) = 0;
    virtual void cb_DR_DSTCLR_DSTCLR00(RegCBstr str) = 0;
    virtual void cb_DR_OVFCLR_SERROVFCLR0(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Cmecccap_dts_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Cmecccap_dts_regif::*) (RegCBstr)> > mRdCbAPI;
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
#endif //__MECCCAP_DTS_REG_DEF_H__
