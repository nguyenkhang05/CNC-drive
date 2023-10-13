// -----------------------------------------------------------------------------
// $Id: $
//
// Copyright(c) 2015 Renesas Electronics Corporation
// Copyright(c) 2015 Renesas Design Vietnam Co., Ltd.
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
// Input file : /home/truongvv2/ATU5_E2FCC2/trunk/ATU5_Output_r24/src/ATU5010/TIMERE/atu5010_timere_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE ATU5010_TIMERE
//     #                   name            offset_size
//     %%REG_INSTANCE      reg_def         8
// 
// %REG_CHANNEL reg_def
//     %%TITLE  group  name        reg_name    wsize       rsize       length  offset  factor_start    factor_end  factor_index    factor_step     access  init        support     callback
//     %%REG    -      TSTRE       TSTRE       8|16     8|16|32        16      0x00    -               -           -               -               W|R     0x0         TRUE        -
//     %%REG    -      SBRLENE     SBRLENE     8|16     8|16|32        16      0x04    -               -           -               -               W|R     0x3FF       TRUE        -
//  
// %REG_NAME TSTRE
//     %%TITLE  name             upper  lower  init      access  support  callback
//     %%BIT    STRE0             0      0     0         W|R     TRUE     W
//     %%BIT    STRE1             1      1     0         W|R     TRUE     -
//     %%BIT    STRE2             2      2     0         W|R     TRUE     -
//     %%BIT    STRE3             3      3     0         W|R     TRUE     -
//     %%BIT    STRE4             4      4     0         W|R     TRUE     -
//     %%BIT    STRE5             5      5     0         W|R     TRUE     -
//     %%BIT    STRE6             6      6     0         W|R     TRUE     -
//     %%BIT    STRE7             7      7     0         W|R     TRUE     -
//     %%BIT    STRE8             8      8     0         W|R     TRUE     -
//     %%BIT    STRE9             9      9     0         W|R     TRUE     -
//     
// %REG_NAME SBRLENE
//     %%TITLE  name             upper  lower  init      access  support  callback
//     %%BIT    SBRLDENE0          0      0     1         W|R     TRUE     W
//     %%BIT    SBRLDENE1          1      1     1         W|R     TRUE     -
//     %%BIT    SBRLDENE2          2      2     1         W|R     TRUE     -
//     %%BIT    SBRLDENE3          3      3     1         W|R     TRUE     -
//     %%BIT    SBRLDENE4          4      4     1         W|R     TRUE     -
//     %%BIT    SBRLDENE5          5      5     1         W|R     TRUE     -
//     %%BIT    SBRLDENE6          6      6     1         W|R     TRUE     -
//     %%BIT    SBRLDENE7          7      7     1         W|R     TRUE     -
//     %%BIT    SBRLDENE8          8      8     1         W|R     TRUE     -
//     %%BIT    SBRLDENE9          9      9     1         W|R     TRUE     -
////////////////////////////////////////////////////////////////////////////////
/// @file atu5010_timere_regif.h
/// @brief Register IF class of model ATU5010_TIMERE
/// $Id$
/// $Date$
/// $Revison$
/// $Author$
////////////////////////////////////////////////////////////////////////////////
#ifndef __ATU5010_TIMERE_REGIF_H__
#define __ATU5010_TIMERE_REGIF_H__
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

/// Register IF class of ATU5010_TIMERE model
class Catu5010_timere_regif
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
    Catu5010_timere_regif(std::string name, uint buswidth);
    ~Catu5010_timere_regif();

protected:
    vpcl::re_register *TSTRE  ;
    vpcl::re_register *SBRLENE;

    #ifdef CWR_SYSTEMC
    scml2::memory<unsigned short> cwmem;
    scml2::reg<unsigned short> *CW_TSTRE  ;
    scml2::reg<unsigned short> *CW_SBRLENE;
    #endif

    uint TSTRE_STRE0;
    uint TSTRE_STRE1;
    uint TSTRE_STRE2;
    uint TSTRE_STRE3;
    uint TSTRE_STRE4;
    uint TSTRE_STRE5;
    uint TSTRE_STRE6;
    uint TSTRE_STRE7;
    uint TSTRE_STRE8;
    uint TSTRE_STRE9;
    uint SBRLENE_SBRLDENE0;
    uint SBRLENE_SBRLDENE1;
    uint SBRLENE_SBRLDENE2;
    uint SBRLENE_SBRLDENE3;
    uint SBRLENE_SBRLDENE4;
    uint SBRLENE_SBRLDENE5;
    uint SBRLENE_SBRLDENE6;
    uint SBRLENE_SBRLDENE7;
    uint SBRLENE_SBRLDENE8;
    uint SBRLENE_SBRLDENE9;

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

    virtual void cb_TSTRE_STRE0(RegCBstr str) = 0;
    virtual void cb_SBRLENE_SBRLDENE0(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Catu5010_timere_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Catu5010_timere_regif::*) (RegCBstr)> > mRdCbAPI;
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
#endif //__ATU5010_TIMERE_REG_DEF_H__
