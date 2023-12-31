// -----------------------------------------------------------------------------
// $Id: atu5011_glue_regif.h 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2015-2019 Renesas Electronics Corporation
// Copyright(c) 2015-2019 Renesas Design Vietnam Co., Ltd.
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
// Input file : atu5011_glue_regif.txt
////////////////////////////////////////////////////////////////////////////////
//  %MODULE ATU5011_GLUE
//      #                   name            offset_size
//      %%REG_INSTANCE      reg_def         8
//  
//  %REG_CHANNEL reg_def
//      %%TITLE  name           reg_name       wsize    rsize    length  offset  factor_start  factor_end  factor_index  factor_step  access init  support callback
//      %%REG    ATUINTSELA     ATUINTSELA     8|16|32  8|16|32  32      0x00    -             -           -             -            W|R    0     TRUE    -
//      %%REG    ATUINTSELD     ATUINTSELD     8|16|32  8|16|32  32      0x0C    0             6           -             0x4          W|R    0     TRUE    -
//      %%REG    ATUINTSELD7    ATUINTSELD7    8|16|32  8|16|32  32      0x28    -             -           -             -            W|R    0     TRUE    -
//      %%REG    ATUDMASELB     ATUDMASELB     8|16|32  8|16|32  32      0x2C    -             -           -             -            W|R    0     TRUE    -
//      %%REG    ATUDMASELCD    ATUDMASELCD    8|16|32  8|16|32  32      0x30    0             7           -             0x4          W|R    0     TRUE    -
//      %%REG    ATUDMASELE     ATUDMASELE     8|16|32  8|16|32  32      0x50    -             -           -             -            W|R    0     TRUE    -
//      %%REG    ATUDFEENTQ     ATUDFEENTQ     8|16|32  8|16|32  32      0x80    0             2           -             0x8          W|R    0     TRUE    -
//      %%REG    ATUDFESEL      ATUDFESEL      8|16|32  8|16|32  32      0x84    0             2           -             0x8          W|R    0     TRUE    -
//      %%REG    ATUDFESELD1T   ATUDFESELD1T   8|16|32  8|16|32  32      0x98    -             -           -             -            W|R    0     TRUE    -
//      %%REG    ATUDSSELDSTS   ATUDSSELDSTS   8|16|32  8|16|32  32      0x9C    -             -           -             -            W|R    0     TRUE    -
//      %%REG    ATUCASELCATS   ATUCASELCATS   8|16|32  8|16|32  32      0xA0    -             -           -             -            W|R    0     TRUE    -
//      %%REG    ATUP5SSEL      ATUP5SSEL      8|16|32  8|16|32  32      0xA4    -             -           -             -            W|R    0     TRUE    -
//  
//  
//  %REG_NAME ATUINTSELA
//      %%TITLE  name           upper  lower  init  access  support  callback
//      %%BIT    ATU_INTSEL_A3  14     12     0     W|R     TRUE     -
//      %%BIT    ATU_INTSEL_A2  10     8      0     W|R     TRUE     -
//      %%BIT    ATU_INTSEL_A1  6      4      0     W|R     TRUE     -
//      %%BIT    ATU_INTSEL_A0  2      0      0     W|R     TRUE     W
//  
//  %REG_NAME ATUINTSELD
//      %%TITLE  name            upper  lower  init  access  support  callback
//      %%BIT    ATU_INTSEL_D13  30     28     0     W|R     TRUE     -
//      %%BIT    ATU_INTSEL_D12  26     24     0     W|R     TRUE     -
//      %%BIT    ATU_INTSEL_D11  22     20     0     W|R     TRUE     -
//      %%BIT    ATU_INTSEL_D10  18     16     0     W|R     TRUE     -
//      %%BIT    ATU_INTSEL_D03  14     12     0     W|R     TRUE     -
//      %%BIT    ATU_INTSEL_D02  10     8      0     W|R     TRUE     -
//      %%BIT    ATU_INTSEL_D01  6      4      0     W|R     TRUE     -
//      %%BIT    ATU_INTSEL_D00  2      0      0     W|R     TRUE     W
//  
//  %REG_NAME ATUINTSELD7
//      %%TITLE  name            upper  lower  init  access  support  callback
//      %%BIT    ATU_INTSEL_D03  14     12     0     W|R     TRUE     -
//      %%BIT    ATU_INTSEL_D02  10     8      0     W|R     TRUE     -
//      %%BIT    ATU_INTSEL_D01  6      4      0     W|R     TRUE     -
//      %%BIT    ATU_INTSEL_D00  2      0      0     W|R     TRUE     W
//  
//  %REG_NAME ATUDMASELB
//      %%TITLE  name            upper  lower  init  access  support  callback
//      %%BIT    ATU_DMASEL_B3   15     12     0     W|R     TRUE     -
//      %%BIT    ATU_DMASEL_B2   11     8      0     W|R     TRUE     -
//      %%BIT    ATU_DMASEL_B1   7      4      0     W|R     TRUE     -
//      %%BIT    ATU_DMASEL_B0   3      0      0     W|R     TRUE     W
//  
//  %REG_NAME ATUDMASELCD
//      %%TITLE  name            upper  lower  init  access  support  callback
//      %%BIT    ATU_DMASEL_CD7  30     28     0     W|R     TRUE     -
//      %%BIT    ATU_DMASEL_CD6  26     24     0     W|R     TRUE     -
//      %%BIT    ATU_DMASEL_CD5  22     20     0     W|R     TRUE     -
//      %%BIT    ATU_DMASEL_CD4  18     16     0     W|R     TRUE     -
//      %%BIT    ATU_DMASEL_CD3  14     12     0     W|R     TRUE     -
//      %%BIT    ATU_DMASEL_CD2  10     8      0     W|R     TRUE     -
//      %%BIT    ATU_DMASEL_CD1  6      4      0     W|R     TRUE     -
//      %%BIT    ATU_DMASEL_CD0  2      0      0     W|R     TRUE     W
//  
//  %REG_NAME ATUDMASELE
//      %%TITLE  name           upper  lower  init  access  support  callback
//      %%BIT    ATU_DMASEL_E   19     0      0     W|R     TRUE     W
//  
//  %REG_NAME ATUDFEENTQ
//      %%TITLE  name           upper  lower  init  access  support  callback
//      %%BIT    ATU_DFEEN_TQ   11     0      0     W|R     TRUE     W
//  
//  %REG_NAME ATUDFESEL
//      %%TITLE  name            upper  lower  init  access  support  callback
//      %%BIT    ATU_DFESEL_C91  29     29     0     W|R     TRUE     -
//      %%BIT    ATU_DFESEL_C90  28     28     0     W|R     TRUE     -
//      %%BIT    ATU_DFESEL_C83  27     27     0     W|R     TRUE     -
//      %%BIT    ATU_DFESEL_C82  26     26     0     W|R     TRUE     -
//      %%BIT    ATU_DFESEL_C81  25     25     0     W|R     TRUE     -
//      %%BIT    ATU_DFESEL_C80  24     24     0     W|R     TRUE     -
//      %%BIT    ATU_DFESEL_D11  22     20     0     W|R     TRUE     -
//      %%BIT    ATU_DFESEL_D10  18     16     0     W|R     TRUE     -
//      %%BIT    ATU_DFESEL_D03  14     12     0     W|R     TRUE     -
//      %%BIT    ATU_DFESEL_D02  10     8      0     W|R     TRUE     -
//      %%BIT    ATU_DFESEL_D01  6      4      0     W|R     TRUE     -
//      %%BIT    ATU_DFESEL_D00  2      0      0     W|R     TRUE     W
//  
//  %REG_NAME ATUDFESELD1T
//      %%TITLE  name            upper  lower  init  access  support  callback
//      %%BIT    ATU_DFESEL_D1T  2      0      0     W|R     TRUE     W
//  
//  %REG_NAME ATUDSSELDSTS
//      %%TITLE  name             upper  lower  init  access  support  callback
//      %%BIT    ATU_DSSEL_DSTS9  19     18     0     W|R     TRUE     -
//      %%BIT    ATU_DSSEL_DSTS8  17     16     0     W|R     TRUE     -
//      %%BIT    ATU_DSSEL_DSTS7  15     14     0     W|R     TRUE     -
//      %%BIT    ATU_DSSEL_DSTS6  13     12     0     W|R     TRUE     -
//      %%BIT    ATU_DSSEL_DSTS5  11     10     0     W|R     TRUE     -
//      %%BIT    ATU_DSSEL_DSTS4  9      8      0     W|R     TRUE     -
//      %%BIT    ATU_DSSEL_DSTS3  7      6      0     W|R     TRUE     -
//      %%BIT    ATU_DSSEL_DSTS2  5      4      0     W|R     TRUE     -
//      %%BIT    ATU_DSSEL_DSTS1  3      2      0     W|R     TRUE     -
//      %%BIT    ATU_DSSEL_DSTS0  1      0      0     W|R     TRUE     W
//  
//  %REG_NAME ATUCASELCATS
//      %%TITLE  name             upper  lower  init  access  support  callback
//      %%BIT    ATU_CASEL_CATS0  1      0      0     W|R     TRUE     W
//  
//  %REG_NAME ATUP5SSEL
//      %%TITLE  name              upper  lower  init  access  support  callback
//      %%BIT    ATU_P5SSEL_SSTSB  27     26     0     W|R     TRUE     -
//      %%BIT    ATU_P5SSEL_SSTSA  25     24     0     W|R     TRUE     -
//      %%BIT    ATU_P5SSEL_CRTSB  23     22     0     W|R     TRUE     -
//      %%BIT    ATU_P5SSEL_CRTSA  21     20     0     W|R     TRUE     -
//      %%BIT    ATU_P5SSEL_CKTSB  19     18     0     W|R     TRUE     -
//      %%BIT    ATU_P5SSEL_CKTSA  17     16     0     W|R     TRUE     -
//      %%BIT    ATU_P5SSEL_TGC7   15     14     0     W|R     TRUE     -
//      %%BIT    ATU_P5SSEL_TGC6   13     12     0     W|R     TRUE     -
//      %%BIT    ATU_P5SSEL_TGC5   11     10     0     W|R     TRUE     -
//      %%BIT    ATU_P5SSEL_TGC4   9      8      0     W|R     TRUE     -
//      %%BIT    ATU_P5SSEL_TGC3   7      6      0     W|R     TRUE     -
//      %%BIT    ATU_P5SSEL_TGC2   5      4      0     W|R     TRUE     -
//      %%BIT    ATU_P5SSEL_TGC1   3      2      0     W|R     TRUE     W
//      ////////////////////////////////////////////////////////////////////////////////
/// @file atu5011_glue_regif.h
/// @brief Register IF class of model ATU5011_GLUE
/// $Id: atu5011_glue_regif.h 1196 2019-10-17 05:33:04Z chuonghoangle $
/// $Date: 2019-10-17 14:33:04 +0900 (2019/10/17 (木)) $
/// $Revison$
/// $Author: chuonghoangle $
////////////////////////////////////////////////////////////////////////////////
#ifndef __ATU5011_GLUE_REGIF_H__
#define __ATU5011_GLUE_REGIF_H__
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

/// Register IF class of ATU5011_GLUE model
class Catu5011_glue_regif
: public vpcl::reg_super
{
protected:
    typedef const unsigned int cuint;
    typedef unsigned int uint;

    enum eRegGroup {
        emNum_of_gr
    }; ///< Enumeration for register group index
    enum eRegIndex {
        emNUM_ATUINTSELD   = 7,
        emNUM_ATUDMASELCD  = 8,
        emNUM_ATUDFEENTQ   = 3,
        emNUM_ATUDFESEL    = 3
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
    Catu5011_glue_regif(std::string name, uint buswidth);
    ~Catu5011_glue_regif();

protected:
    vpcl::re_register *ATUINTSELA  ;
    vpcl::re_register *ATUINTSELD  [7];
    vpcl::re_register *ATUINTSELD7 ;
    vpcl::re_register *ATUDMASELB  ;
    vpcl::re_register *ATUDMASELCD [8];
    vpcl::re_register *ATUDMASELE  ;
    vpcl::re_register *ATUDFEENTQ  [3];
    vpcl::re_register *ATUDFESEL   [3];
    vpcl::re_register *ATUDFESELD1T;
    vpcl::re_register *ATUDSSELDSTS;
    vpcl::re_register *ATUCASELCATS;
    vpcl::re_register *ATUP5SSEL   ;

    #ifdef USR_CWR_SYSTEMC
    scml2::memory<uint> cwmem;
    scml2::reg<uint> *CW_ATUINTSELA  ;
    scml2::reg<uint> *CW_ATUINTSELD  [7];
    scml2::reg<uint> *CW_ATUINTSELD7 ;
    scml2::reg<uint> *CW_ATUDMASELB  ;
    scml2::reg<uint> *CW_ATUDMASELCD [8];
    scml2::reg<uint> *CW_ATUDMASELE  ;
    scml2::reg<uint> *CW_ATUDFEENTQ  [3];
    scml2::reg<uint> *CW_ATUDFESEL   [3];
    scml2::reg<uint> *CW_ATUDFESELD1T;
    scml2::reg<uint> *CW_ATUDSSELDSTS;
    scml2::reg<uint> *CW_ATUCASELCATS;
    scml2::reg<uint> *CW_ATUP5SSEL   ;
    #endif

    uint ATUINTSELA_ATU_INTSEL_A3;
    uint ATUINTSELA_ATU_INTSEL_A2;
    uint ATUINTSELA_ATU_INTSEL_A1;
    uint ATUINTSELA_ATU_INTSEL_A0;
    uint ATUINTSELD_ATU_INTSEL_D13[7];
    uint ATUINTSELD_ATU_INTSEL_D12[7];
    uint ATUINTSELD_ATU_INTSEL_D11[7];
    uint ATUINTSELD_ATU_INTSEL_D10[7];
    uint ATUINTSELD_ATU_INTSEL_D03[7];
    uint ATUINTSELD_ATU_INTSEL_D02[7];
    uint ATUINTSELD_ATU_INTSEL_D01[7];
    uint ATUINTSELD_ATU_INTSEL_D00[7];
    uint ATUINTSELD7_ATU_INTSEL_D03;
    uint ATUINTSELD7_ATU_INTSEL_D02;
    uint ATUINTSELD7_ATU_INTSEL_D01;
    uint ATUINTSELD7_ATU_INTSEL_D00;
    uint ATUDMASELB_ATU_DMASEL_B3;
    uint ATUDMASELB_ATU_DMASEL_B2;
    uint ATUDMASELB_ATU_DMASEL_B1;
    uint ATUDMASELB_ATU_DMASEL_B0;
    uint ATUDMASELCD_ATU_DMASEL_CD7[8];
    uint ATUDMASELCD_ATU_DMASEL_CD6[8];
    uint ATUDMASELCD_ATU_DMASEL_CD5[8];
    uint ATUDMASELCD_ATU_DMASEL_CD4[8];
    uint ATUDMASELCD_ATU_DMASEL_CD3[8];
    uint ATUDMASELCD_ATU_DMASEL_CD2[8];
    uint ATUDMASELCD_ATU_DMASEL_CD1[8];
    uint ATUDMASELCD_ATU_DMASEL_CD0[8];
    uint ATUDMASELE_ATU_DMASEL_E;
    uint ATUDFEENTQ_ATU_DFEEN_TQ[3];
    uint ATUDFESEL_ATU_DFESEL_C91[3];
    uint ATUDFESEL_ATU_DFESEL_C90[3];
    uint ATUDFESEL_ATU_DFESEL_C83[3];
    uint ATUDFESEL_ATU_DFESEL_C82[3];
    uint ATUDFESEL_ATU_DFESEL_C81[3];
    uint ATUDFESEL_ATU_DFESEL_C80[3];
    uint ATUDFESEL_ATU_DFESEL_D11[3];
    uint ATUDFESEL_ATU_DFESEL_D10[3];
    uint ATUDFESEL_ATU_DFESEL_D03[3];
    uint ATUDFESEL_ATU_DFESEL_D02[3];
    uint ATUDFESEL_ATU_DFESEL_D01[3];
    uint ATUDFESEL_ATU_DFESEL_D00[3];
    uint ATUDFESELD1T_ATU_DFESEL_D1T;
    uint ATUDSSELDSTS_ATU_DSSEL_DSTS9;
    uint ATUDSSELDSTS_ATU_DSSEL_DSTS8;
    uint ATUDSSELDSTS_ATU_DSSEL_DSTS7;
    uint ATUDSSELDSTS_ATU_DSSEL_DSTS6;
    uint ATUDSSELDSTS_ATU_DSSEL_DSTS5;
    uint ATUDSSELDSTS_ATU_DSSEL_DSTS4;
    uint ATUDSSELDSTS_ATU_DSSEL_DSTS3;
    uint ATUDSSELDSTS_ATU_DSSEL_DSTS2;
    uint ATUDSSELDSTS_ATU_DSSEL_DSTS1;
    uint ATUDSSELDSTS_ATU_DSSEL_DSTS0;
    uint ATUCASELCATS_ATU_CASEL_CATS0;
    uint ATUP5SSEL_ATU_P5SSEL_SSTSB;
    uint ATUP5SSEL_ATU_P5SSEL_SSTSA;
    uint ATUP5SSEL_ATU_P5SSEL_CRTSB;
    uint ATUP5SSEL_ATU_P5SSEL_CRTSA;
    uint ATUP5SSEL_ATU_P5SSEL_CKTSB;
    uint ATUP5SSEL_ATU_P5SSEL_CKTSA;
    uint ATUP5SSEL_ATU_P5SSEL_TGC7;
    uint ATUP5SSEL_ATU_P5SSEL_TGC6;
    uint ATUP5SSEL_ATU_P5SSEL_TGC5;
    uint ATUP5SSEL_ATU_P5SSEL_TGC4;
    uint ATUP5SSEL_ATU_P5SSEL_TGC3;
    uint ATUP5SSEL_ATU_P5SSEL_TGC2;
    uint ATUP5SSEL_ATU_P5SSEL_TGC1;

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

    virtual void cb_ATUINTSELA_ATU_INTSEL_A0(RegCBstr str) = 0;
    virtual void cb_ATUINTSELD_ATU_INTSEL_D00(RegCBstr str) = 0;
    virtual void cb_ATUINTSELD7_ATU_INTSEL_D00(RegCBstr str) = 0;
    virtual void cb_ATUDMASELB_ATU_DMASEL_B0(RegCBstr str) = 0;
    virtual void cb_ATUDMASELCD_ATU_DMASEL_CD0(RegCBstr str) = 0;
    virtual void cb_ATUDMASELE_ATU_DMASEL_E(RegCBstr str) = 0;
    virtual void cb_ATUDFEENTQ_ATU_DFEEN_TQ(RegCBstr str) = 0;
    virtual void cb_ATUDFESEL_ATU_DFESEL_D00(RegCBstr str) = 0;
    virtual void cb_ATUDFESELD1T_ATU_DFESEL_D1T(RegCBstr str) = 0;
    virtual void cb_ATUDSSELDSTS_ATU_DSSEL_DSTS0(RegCBstr str) = 0;
    virtual void cb_ATUCASELCATS_ATU_CASEL_CATS0(RegCBstr str) = 0;
    virtual void cb_ATUP5SSEL_ATU_P5SSEL_TGC1(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Catu5011_glue_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Catu5011_glue_regif::*) (RegCBstr)> > mRdCbAPI;
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
#endif //__ATU5011_GLUE_REG_DEF_H__
