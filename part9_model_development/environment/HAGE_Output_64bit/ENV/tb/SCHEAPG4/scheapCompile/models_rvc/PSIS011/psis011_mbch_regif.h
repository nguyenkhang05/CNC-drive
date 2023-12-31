// -----------------------------------------------------------------------------
// $Id: psis011_mbch_regif.h,v 1.6 2017/12/16 09:57:23 chanle Exp $
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
// Input file : psis011_mbch_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE psis011_mbch
//     #              name     offset_size
//     %%REG_INSTANCE reg_def  8 
// 
// %REG_CHANNEL reg_def                                                       
//     %%TITLE    name            reg_name        wsize   rsize  length offset  access   init   support  callback
//     %%REG      PSI5SPMBn1S    PSI5SPMBn1S    -       8|16|32  32     0x00   R        0x0     TRUE     -
//     %%REG      PSI5SPMBn1D    PSI5SPMBn1D    -       8|16|32  32     0x04   R        0x0     TRUE     -
//     %%REG      PSI5SPMBn1T    PSI5SPMBn1T    -       8|16|32  32     0x08   R        0x0     TRUE     -
//     %%REG      PSI5SPMBn2S    PSI5SPMBn2S    -       8|16|32  32     0x0C   R        0x0     TRUE     -
//     %%REG      PSI5SPMBn2D    PSI5SPMBn2D    -       8|16|32  32     0x10   R        0x0     TRUE     -
//     %%REG      PSI5SPMBn2T    PSI5SPMBn2T    -       8|16|32  32     0x14   R        0x0     TRUE     -
//     %%REG      PSI5SPMBn3S    PSI5SPMBn3S    -       8|16|32  32     0x18   R        0x0     TRUE     -
//     %%REG      PSI5SPMBn3D    PSI5SPMBn3D    -       8|16|32  32     0x1C   R        0x0     TRUE     -
//     %%REG      PSI5SPMBn3T    PSI5SPMBn3T    -       8|16|32  32     0x20   R        0x0     TRUE     -
//     %%REG      PSI5SPMBn4S    PSI5SPMBn4S    -       8|16|32  32     0x24   R        0x0     TRUE     -
//     %%REG      PSI5SPMBn4D    PSI5SPMBn4D    -       8|16|32  32     0x28   R        0x0     TRUE     -
//     %%REG      PSI5SPMBn4T    PSI5SPMBn4T    -       8|16|32  32     0x2C   R        0x0     TRUE     -
//     %%REG      PSI5SPMBn5S    PSI5SPMBn5S    -       8|16|32  32     0x30   R        0x0     TRUE     -
//     %%REG      PSI5SPMBn5D    PSI5SPMBn5D    -       8|16|32  32     0x34   R        0x0     TRUE     -
//     %%REG      PSI5SPMBn5T    PSI5SPMBn5T    -       8|16|32  32     0x38   R        0x0     TRUE     -
//     %%REG      PSI5SPMBn6S    PSI5SPMBn6S    -       8|16|32  32     0x3C   R        0x0     TRUE     -
//     %%REG      PSI5SPMBn6D    PSI5SPMBn6D    -       8|16|32  32     0x40   R        0x0     TRUE     -
//     %%REG      PSI5SPMBn6T    PSI5SPMBn6T    -       8|16|32  32     0x44   R        0x0     TRUE     -
// 
// %REG_NAME PSI5SPMBn1S                                                          
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    DCI                31      28      0       R       TRUE        -
//     %%BIT    CHID               24      22      0       R       TRUE        -
//     %%BIT    FID                21      19      0       R       TRUE        -
//     %%BIT    MBORERR            18      18      0       R       TRUE        -
//     %%BIT    WDTERR             17      17      0       R       TRUE        -
//     %%BIT    UTFRERR            15      15      0       R       TRUE        -
//     %%BIT    UTPTERR            14      14      0       R       TRUE        -
//     %%BIT    HEADERR            13      13      0       R       TRUE        -
//     %%BIT    HEADST             12      11      0       R       TRUE        -
//     %%BIT    CRCERR             10      10      0       R       TRUE        -
//     %%BIT    CRC                9       7       0       R       TRUE        -
//     %%BIT    XCRCERR            6       6       0       R       TRUE        -
//     %%BIT    XCRC               5       0       0       R       TRUE        -
// 
// %REG_NAME PSI5SPMBn1D                                                          
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    DCI                31      28      0       R       TRUE        -
//     %%BIT    DATA               27      0       0       R       TRUE        -
// 
// %REG_NAME PSI5SPMBn1T                                                          
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    DCI                31      28      0       R       TRUE        -
//     %%BIT    TMST               23      0       0       R       TRUE        -
// 
// %REG_NAME PSI5SPMBn2S                                                          
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    DCI                31      28      0       R       TRUE        -
//     %%BIT    CHID               24      22      0       R       TRUE        -
//     %%BIT    FID                21      19      0       R       TRUE        -
//     %%BIT    MBORERR            18      18      0       R       TRUE        -
//     %%BIT    WDTERR             17      17      0       R       TRUE        -
//     %%BIT    UTFRERR            15      15      0       R       TRUE        -
//     %%BIT    UTPTERR            14      14      0       R       TRUE        -
//     %%BIT    HEADERR            13      13      0       R       TRUE        -
//     %%BIT    HEADST             12      11      0       R       TRUE        -
//     %%BIT    CRCERR             10      10      0       R       TRUE        -
//     %%BIT    CRC                9       7       0       R       TRUE        -
//     %%BIT    XCRCERR            6       6       0       R       TRUE        -
//     %%BIT    XCRC               5       0       0       R       TRUE        -
// 
// %REG_NAME PSI5SPMBn2D                                                          
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    DCI                31      28      0       R       TRUE        -
//     %%BIT    DATA               27      0       0       R       TRUE        -
// 
// %REG_NAME PSI5SPMBn2T                                                          
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    DCI                31      28      0       R       TRUE        -
//     %%BIT    TMST               23      0       0       R       TRUE        -
// 
// %REG_NAME PSI5SPMBn3S                                                          
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    DCI                31      28      0       R       TRUE        -
//     %%BIT    CHID               24      22      0       R       TRUE        -
//     %%BIT    FID                21      19      0       R       TRUE        -
//     %%BIT    MBORERR            18      18      0       R       TRUE        -
//     %%BIT    HEADERR            13      13      0       R       TRUE        -
//     %%BIT    HEADST             12      11      0       R       TRUE        -
//     %%BIT    CRCERR             10      10      0       R       TRUE        -
//     %%BIT    CRC                9       7       0       R       TRUE        -
//     %%BIT    XCRC               5       0       0       R       TRUE        -
// 
// %REG_NAME PSI5SPMBn3D                                                          
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    DCI                31      28      0       R       TRUE        -
//     %%BIT    DATA               27      0       0       R       TRUE        -
// 
// %REG_NAME PSI5SPMBn3T                                                          
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    DCI                31      28      0       R       TRUE        -
//     %%BIT    TMST               23      0       0       R       TRUE        -
// 
// %REG_NAME PSI5SPMBn4S                                                          
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    DCI                31      28      0       R       TRUE        -
//     %%BIT    CHID               24      22      0       R       TRUE        -
//     %%BIT    FID                21      19      0       R       TRUE        -
//     %%BIT    MBORERR            18      18      0       R       TRUE        -
//     %%BIT    HEADERR            13      13      0       R       TRUE        -
//     %%BIT    HEADST             12      11      0       R       TRUE        -
//     %%BIT    CRCERR             10      10      0       R       TRUE        -
//     %%BIT    CRC                9       7       0       R       TRUE        -
//     %%BIT    XCRC               5       0       0       R       TRUE        -
// 
// %REG_NAME PSI5SPMBn4D                                                          
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    DCI                31      28      0       R       TRUE        -
//     %%BIT    DATA               27      0       0       R       TRUE        -
// 
// %REG_NAME PSI5SPMBn4T                                                          
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    DCI                31      28      0       R       TRUE        -
//     %%BIT    TMST               23      0       0       R       TRUE        -
// 
// %REG_NAME PSI5SPMBn5S                                                          
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    DCI                31      28      0       R       TRUE        -
//     %%BIT    CHID               24      22      0       R       TRUE        -
//     %%BIT    FID                21      19      0       R       TRUE        -
//     %%BIT    MBORERR            18      18      0       R       TRUE        -
//     %%BIT    HEADERR            13      13      0       R       TRUE        -
//     %%BIT    HEADST             12      11      0       R       TRUE        -
//     %%BIT    CRCERR             10      10      0       R       TRUE        -
//     %%BIT    CRC                9       7       0       R       TRUE        -
//     %%BIT    XCRC               5       0       0       R       TRUE        -
// 
// %REG_NAME PSI5SPMBn5D                                                          
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    DCI                31      28      0       R       TRUE        -
//     %%BIT    DATA               27      0       0       R       TRUE        -
// 
// %REG_NAME PSI5SPMBn5T                                                          
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    DCI                31      28      0       R       TRUE        -
//     %%BIT    TMST               23      0       0       R       TRUE        -
// 
// %REG_NAME PSI5SPMBn6S                                                          
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    DCI                31      28      0       R       TRUE        -
//     %%BIT    CHID               24      22      0       R       TRUE        -
//     %%BIT    FID                21      19      0       R       TRUE        -
//     %%BIT    MBORERR            18      18      0       R       TRUE        -
//     %%BIT    HEADERR            13      13      0       R       TRUE        -
//     %%BIT    HEADST             12      11      0       R       TRUE        -
//     %%BIT    CRCERR             10      10      0       R       TRUE        -
//     %%BIT    CRC                9       7       0       R       TRUE        -
//     %%BIT    XCRC               5       0       0       R       TRUE        -
// 
// %REG_NAME PSI5SPMBn6D                                                          
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    DCI                31      28      0       R       TRUE        -
//     %%BIT    DATA               27      0       0       R       TRUE        -
// 
// %REG_NAME PSI5SPMBn6T                                                          
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    DCI                31      28      0       R       TRUE        -
//     %%BIT    TMST               23      0       0       R       TRUE        -
////////////////////////////////////////////////////////////////////////////////
/// @file psis011_mbch_regif.h
/// @brief Register IF class of model PSIS011_MBCH
/// $Id: psis011_mbch_regif.h,v 1.6 2017/12/16 09:57:23 chanle Exp $
/// $Date: 2017/12/16 09:57:23 $
/// $Revison$
/// $Author: chanle $
////////////////////////////////////////////////////////////////////////////////
#ifndef __PSIS011_MBCH_REGIF_H__
#define __PSIS011_MBCH_REGIF_H__
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

/// Register IF class of PSIS011_MBCH model
class Cpsis011_mbch_regif
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
    Cpsis011_mbch_regif(std::string name, uint buswidth, unsigned int chid);
    ~Cpsis011_mbch_regif();

protected:
    vpcl::re_register *PSI5SPMBn1S;
    vpcl::re_register *PSI5SPMBn1D;
    vpcl::re_register *PSI5SPMBn1T;
    vpcl::re_register *PSI5SPMBn2S;
    vpcl::re_register *PSI5SPMBn2D;
    vpcl::re_register *PSI5SPMBn2T;
    vpcl::re_register *PSI5SPMBn3S;
    vpcl::re_register *PSI5SPMBn3D;
    vpcl::re_register *PSI5SPMBn3T;
    vpcl::re_register *PSI5SPMBn4S;
    vpcl::re_register *PSI5SPMBn4D;
    vpcl::re_register *PSI5SPMBn4T;
    vpcl::re_register *PSI5SPMBn5S;
    vpcl::re_register *PSI5SPMBn5D;
    vpcl::re_register *PSI5SPMBn5T;
    vpcl::re_register *PSI5SPMBn6S;
    vpcl::re_register *PSI5SPMBn6D;
    vpcl::re_register *PSI5SPMBn6T;

    #ifdef CWR_SYSTEMC
    scml2::memory<uint> cwmem;
    scml2::reg<uint> *CW_PSI5SPMBn1S;
    scml2::reg<uint> *CW_PSI5SPMBn1D;
    scml2::reg<uint> *CW_PSI5SPMBn1T;
    scml2::reg<uint> *CW_PSI5SPMBn2S;
    scml2::reg<uint> *CW_PSI5SPMBn2D;
    scml2::reg<uint> *CW_PSI5SPMBn2T;
    scml2::reg<uint> *CW_PSI5SPMBn3S;
    scml2::reg<uint> *CW_PSI5SPMBn3D;
    scml2::reg<uint> *CW_PSI5SPMBn3T;
    scml2::reg<uint> *CW_PSI5SPMBn4S;
    scml2::reg<uint> *CW_PSI5SPMBn4D;
    scml2::reg<uint> *CW_PSI5SPMBn4T;
    scml2::reg<uint> *CW_PSI5SPMBn5S;
    scml2::reg<uint> *CW_PSI5SPMBn5D;
    scml2::reg<uint> *CW_PSI5SPMBn5T;
    scml2::reg<uint> *CW_PSI5SPMBn6S;
    scml2::reg<uint> *CW_PSI5SPMBn6D;
    scml2::reg<uint> *CW_PSI5SPMBn6T;
    #endif

    uint PSI5SPMBn1S_DCI;
    uint PSI5SPMBn1S_CHID;
    uint PSI5SPMBn1S_FID;
    uint PSI5SPMBn1S_MBORERR;
    uint PSI5SPMBn1S_WDTERR;
    uint PSI5SPMBn1S_UTFRERR;
    uint PSI5SPMBn1S_UTPTERR;
    uint PSI5SPMBn1S_HEADERR;
    uint PSI5SPMBn1S_HEADST;
    uint PSI5SPMBn1S_CRCERR;
    uint PSI5SPMBn1S_CRC;
    uint PSI5SPMBn1S_XCRCERR;
    uint PSI5SPMBn1S_XCRC;
    uint PSI5SPMBn1D_DCI;
    uint PSI5SPMBn1D_DATA;
    uint PSI5SPMBn1T_DCI;
    uint PSI5SPMBn1T_TMST;
    uint PSI5SPMBn2S_DCI;
    uint PSI5SPMBn2S_CHID;
    uint PSI5SPMBn2S_FID;
    uint PSI5SPMBn2S_MBORERR;
    uint PSI5SPMBn2S_WDTERR;
    uint PSI5SPMBn2S_UTFRERR;
    uint PSI5SPMBn2S_UTPTERR;
    uint PSI5SPMBn2S_HEADERR;
    uint PSI5SPMBn2S_HEADST;
    uint PSI5SPMBn2S_CRCERR;
    uint PSI5SPMBn2S_CRC;
    uint PSI5SPMBn2S_XCRCERR;
    uint PSI5SPMBn2S_XCRC;
    uint PSI5SPMBn2D_DCI;
    uint PSI5SPMBn2D_DATA;
    uint PSI5SPMBn2T_DCI;
    uint PSI5SPMBn2T_TMST;
    uint PSI5SPMBn3S_DCI;
    uint PSI5SPMBn3S_CHID;
    uint PSI5SPMBn3S_FID;
    uint PSI5SPMBn3S_MBORERR;
    uint PSI5SPMBn3S_HEADERR;
    uint PSI5SPMBn3S_HEADST;
    uint PSI5SPMBn3S_CRCERR;
    uint PSI5SPMBn3S_CRC;
    uint PSI5SPMBn3S_XCRC;
    uint PSI5SPMBn3D_DCI;
    uint PSI5SPMBn3D_DATA;
    uint PSI5SPMBn3T_DCI;
    uint PSI5SPMBn3T_TMST;
    uint PSI5SPMBn4S_DCI;
    uint PSI5SPMBn4S_CHID;
    uint PSI5SPMBn4S_FID;
    uint PSI5SPMBn4S_MBORERR;
    uint PSI5SPMBn4S_HEADERR;
    uint PSI5SPMBn4S_HEADST;
    uint PSI5SPMBn4S_CRCERR;
    uint PSI5SPMBn4S_CRC;
    uint PSI5SPMBn4S_XCRC;
    uint PSI5SPMBn4D_DCI;
    uint PSI5SPMBn4D_DATA;
    uint PSI5SPMBn4T_DCI;
    uint PSI5SPMBn4T_TMST;
    uint PSI5SPMBn5S_DCI;
    uint PSI5SPMBn5S_CHID;
    uint PSI5SPMBn5S_FID;
    uint PSI5SPMBn5S_MBORERR;
    uint PSI5SPMBn5S_HEADERR;
    uint PSI5SPMBn5S_HEADST;
    uint PSI5SPMBn5S_CRCERR;
    uint PSI5SPMBn5S_CRC;
    uint PSI5SPMBn5S_XCRC;
    uint PSI5SPMBn5D_DCI;
    uint PSI5SPMBn5D_DATA;
    uint PSI5SPMBn5T_DCI;
    uint PSI5SPMBn5T_TMST;
    uint PSI5SPMBn6S_DCI;
    uint PSI5SPMBn6S_CHID;
    uint PSI5SPMBn6S_FID;
    uint PSI5SPMBn6S_MBORERR;
    uint PSI5SPMBn6S_HEADERR;
    uint PSI5SPMBn6S_HEADST;
    uint PSI5SPMBn6S_CRCERR;
    uint PSI5SPMBn6S_CRC;
    uint PSI5SPMBn6S_XCRC;
    uint PSI5SPMBn6D_DCI;
    uint PSI5SPMBn6D_DATA;
    uint PSI5SPMBn6T_DCI;
    uint PSI5SPMBn6T_TMST;

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


    void set_instance_name(std::string InstName);

    unsigned int mChID;
    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Cpsis011_mbch_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Cpsis011_mbch_regif::*) (RegCBstr)> > mRdCbAPI;
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
#endif //__PSIS011_MBCH_REG_DEF_H__
