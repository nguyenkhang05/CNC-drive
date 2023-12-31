// -----------------------------------------------------------------------------
// $Id: atu5_timere_sub_regif.h 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
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
// Input file : atu5_timere_sub_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE ATU5_TIMERE_SUB
//     #                   name            offset_size
//     %%REG_INSTANCE      reg_def         8
// 
// %REG_CHANNEL reg_def
//     %%TITLE  group  name        reg_name    wsize       rsize       length  offset  factor_start    factor_end  factor_index    factor_step     access      init        support     callback
//     %%REG    -      SSTRE       SSTRE       8           8|16|32     8       0x00    -               -           -               -                W|R        0x0         TRUE        -
//     %%REG    -      PSCRE       PSCRE       8           8|16|32     8       0x04    -               -           -               -                W|R        0x0         TRUE        -
//     %%REG    -      TCRE        TCRE        8           8|16|32     8       0x08   -               -           -               -                W|R        0x0         TRUE        -
//     %%REG    -      RLDCRE      RLDCRE      8           8|16|32     8       0x0A   -               -           -               -                W|R        0x0         TRUE        -
//     %%REG    -      POECRE      POECRE      16          8|16|32     16      0x0C   -               -           -               -                W|R        0x0         TRUE        -
//     %%REG    -      SOLVLE      SOLVLE      8           8|16|32     8       0x0E   -               -           -               -                W|R        0x0         TRUE        -
//     %%REG    -      TSRE        TSRE        -           8|16|32     16      0x10   -               -           -               -                R          0x0         TRUE        -
//     %%REG    -      TSCRE       TSCRE       8|16        8|16|32     16      0x12   -               -           -               -                R|W1:0     0x0         TRUE        -
//     %%REG    -      TOCRE       TOCRE       8           8|16|32     8       0x14   -               -           -               -                W|R        0x0         TRUE        -
//     %%REG    -      TIERE       TIERE       8|16        8|16|32     16      0x16   -               -           -               -                W|R        0x0         TRUE        -
//     %%REG    -      PSCCRE     PSCCRE     8           8|16|32     8       0x18   0               3           -              0x01              W|R        0x0         TRUE        -
//     %%REG    -      FCTRGE      FCTRGE      8           8|16|32     8       0x1C   -               -           -              -               R|W1:0        0x0         TRUE        -
//     %%REG    -      TCNTE      TCNTE      32          8|16|32     32      0x24   0               3           -              0x20              W|R        0x00000100  TRUE        -
//     %%REG    -      CYLRE      CYLRE      32          8|16|32     32      0x28   0               3           -              0x20              W|R        0xFFFFFF00  TRUE        -
//     %%REG    -      DTRE       DTRE       32          8|16|32     32      0x2C   0               3           -              0x20              W|R        0xFFFFFF00  TRUE        -
//     %%REG    -      CRLDE      CRLDE      32          8|16|32     32      0x30   0               3           -              0x20              W|R        0xFFFFFF00  TRUE        -
//     %%REG    -      DRLDE      DRLDE      32          8|16|32     32      0x34   0               3           -              0x20              W|R        0xFFFFFF00  TRUE        -
// 
// %REG_NAME SSTRE
//     %%TITLE  name             upper  lower  init            access  support  callback
//     %%BIT    SSTRE0             0      0     0               W|R     TRUE     W
//     %%BIT    SSTRE1             1      1     0               W|R     TRUE     -
//     %%BIT    SSTRE2             2      2     0               W|R     TRUE     -
//     %%BIT    SSTRE3             3      3     0               W|R     TRUE     -
//                                                           
// %REG_NAME PSCRE                                           
//     %%TITLE  name             upper  lower  init            access  support  callback
//     %%BIT    PSCE               2      0     0               W|R     TRUE     W
//                                                           
// %REG_NAME PSCCRE                                         
//     %%TITLE  name             upper  lower  init            access  support  callback
//     %%BIT    PSCCE              7      0     0               W|R     TRUE     W
//                                                           
// %REG_NAME TCRE                                            
//     %%TITLE  name             upper  lower  init            access  support  callback
//     %%BIT    CKSELE             2      0     0               W|R     TRUE     W
//     %%BIT    PSCSEL             7      7     0               W|R     TRUE     -
//                                                           
// %REG_NAME RLDCRE                                          
//     %%TITLE  name             upper  lower  init            access  support  callback
//     %%BIT    RLDENE0            0      0     0               W|R     TRUE     W
//     %%BIT    RLDENE1            1      1     0               W|R     TRUE     -
//     %%BIT    RLDENE2            2      2     0               W|R     TRUE     -
//     %%BIT    RLDENE3            3      3     0               W|R     TRUE     -
//                                                           
// %REG_NAME POECRE                                          
//     %%TITLE  name             upper  lower  init            access  support  callback
//     %%BIT    POEEN0             0      0     0               W|R     TRUE     -
//     %%BIT    POEEN1             1      1     0               W|R     TRUE     -
//     %%BIT    POEEN2             2      2     0               W|R     TRUE     -
//     %%BIT    POEEN3             3      3     0               W|R     TRUE     -
//     %%BIT    POEPOL             4      4     0               W|R     TRUE     -
//     %%BIT    POECRKEY           15     8     0               W|R     TRUE     W
//                                                           
// %REG_NAME SOLVLE                                          
//     %%TITLE  name             upper  lower  init            access  support  callback
//     %%BIT    PWMSLV0            0      0     0               W|R     TRUE     -
//     %%BIT    PWMSLV1            1      1     0               W|R     TRUE     -
//     %%BIT    PWMSLV2            2      2     0               W|R     TRUE     -
//     %%BIT    PWMSLV3            3      3     0               W|R     TRUE     -
//                                                           
// %REG_NAME TSRE                                            
//     %%TITLE  name             upper  lower  init            access  support  callback
//     %%BIT    CMFE0              0      0     0               R       TRUE     -
//     %%BIT    CMFE1              1      1     0               R       TRUE     -
//     %%BIT    CMFE2              2      2     0               R       TRUE     -
//     %%BIT    CMFE3              3      3     0               R       TRUE     -
//     %%BIT    OVFE0              4      4     0               R       TRUE     -
//     %%BIT    OVFE1              5      5     0               R       TRUE     -
//     %%BIT    OVFE2              6      6     0               R       TRUE     -
//     %%BIT    OVFE3              7      7     0               R       TRUE     -
//     %%BIT    DMFE0              8      8     0               R       TRUE     -
//     %%BIT    DMFE1              9      9     0               R       TRUE     -
//     %%BIT    DMFE2              10     10    0               R       TRUE     -
//     %%BIT    DMFE3              11     11    0               R       TRUE     -
//                                                           
// %REG_NAME TSCRE                                           
//     %%TITLE  name             upper  lower  init            access  support  callback
//     %%BIT    CMFCE0             0      0     0               R|W1:0     TRUE     W
//     %%BIT    CMFCE1             1      1     0               R|W1:0     TRUE     -
//     %%BIT    CMFCE2             2      2     0               R|W1:0     TRUE     -
//     %%BIT    CMFCE3             3      3     0               R|W1:0     TRUE     -
//     %%BIT    OVFCE0             4      4     0               R|W1:0     TRUE     -
//     %%BIT    OVFCE1             5      5     0               R|W1:0     TRUE     -
//     %%BIT    OVFCE2             6      6     0               R|W1:0     TRUE     -
//     %%BIT    OVFCE3             7      7     0               R|W1:0     TRUE     -
//     %%BIT    DMFCE0             8      8     0               R|W1:0     TRUE     -
//     %%BIT    DMFCE1             9      9     0               R|W1:0     TRUE     -
//     %%BIT    DMFCE2             10     10    0               R|W1:0     TRUE     -
//     %%BIT    DMFCE3             11     11    0               R|W1:0     TRUE     -
//                                                           
// %REG_NAME TIERE                                           
//     %%TITLE  name             upper  lower  init            access  support  callback
//     %%BIT    CMEE0              0      0     0               W|R     TRUE     -
//     %%BIT    CMEE1              1      1     0               W|R     TRUE     -
//     %%BIT    CMEE2              2      2     0               W|R     TRUE     -
//     %%BIT    CMEE3              3      3     0               W|R     TRUE     -
//     %%BIT    DMEE0              8      8     0               W|R     TRUE     -
//     %%BIT    DMEE1              9      9     0               W|R     TRUE     -
//     %%BIT    DMEE2              10     10    0               W|R     TRUE     -
//     %%BIT    DMEE3              11     11    0               W|R     TRUE     -
//                                                           
// %REG_NAME TOCRE                                           
//     %%TITLE  name             upper  lower  init            access  support  callback
//     %%BIT    TONEE0             0      0     0               W|R     TRUE     W
//     %%BIT    TONEE1             1      1     0               W|R     TRUE     -
//     %%BIT    TONEE2             2      2     0               W|R     TRUE     -
//     %%BIT    TONEE3             3      3     0               W|R     TRUE     -
//                                                           
// %REG_NAME FCTRGE                                          
//     %%TITLE  name             upper  lower  init            access  support  callback
//     %%BIT    FCMT0              0      0     0               R|W1:0     TRUE     W
//     %%BIT    FCMT1              1      1     0               R|W1:0     TRUE     -
//     %%BIT    FCMT2              2      2     0               R|W1:0     TRUE     -
//     %%BIT    FCMT3              3      3     0               R|W1:0     TRUE     -
//                                                           
// %REG_NAME TCNTE                                          
//     %%TITLE  name             upper  lower  init            access  support  callback
//     %%BIT    TCNTE              31      8    1               W|R     TRUE     W
//      
// %REG_NAME CYLRE
//     %%TITLE  name             upper  lower  init            access  support  callback
//     %%BIT    CYLRE              31      8   0xFFFFFF         W|R     TRUE     W
//      
// %REG_NAME DTRE
//     %%TITLE  name             upper  lower  init            access  support  callback
//     %%BIT    DTRE               31      8   0xFFFFFF         W|R     TRUE     W
//      
// %REG_NAME CRLDE
//     %%TITLE  name             upper  lower  init            access  support  callback
//     %%BIT    CRLDE              31      8   0xFFFFFF         W|R     TRUE     -
//      
// %REG_NAME DRLDE
//     %%TITLE  name             upper  lower  init            access  support  callback
//     %%BIT    DRLDE              31      8   0xFFFFFF         W|R     TRUE     -
//  
////////////////////////////////////////////////////////////////////////////////
/// @file atu5_timere_sub_regif.h
/// @brief Register IF class of model ATU5_TIMERE_SUB
/// $Id: atu5_timere_sub_regif.h 1196 2019-10-17 05:33:04Z chuonghoangle $
/// $Date: 2019-10-17 14:33:04 +0900 (2019/10/17 (木)) $
/// $Revison$
/// $Author: chuonghoangle $
////////////////////////////////////////////////////////////////////////////////
#ifndef __ATU5_TIMERE_SUB_REGIF_H__
#define __ATU5_TIMERE_SUB_REGIF_H__
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

/// Register IF class of ATU5_TIMERE_SUB model
class Catu5_timere_sub_regif
: public vpcl::reg_super
{
protected:
    typedef const unsigned int cuint;
    typedef unsigned int uint;

    enum eRegGroup {
        emNum_of_gr
    }; ///< Enumeration for register group index
    enum eRegIndex {
        emNUM_PSCCRE = 4,
        emNUM_TCNTE  = 4,
        emNUM_CYLRE  = 4,
        emNUM_DTRE   = 4,
        emNUM_CRLDE  = 4,
        emNUM_DRLDE  = 4
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
    Catu5_timere_sub_regif(std::string name, uint buswidth);
    ~Catu5_timere_sub_regif();

protected:
    vpcl::re_register *SSTRE ;
    vpcl::re_register *PSCRE ;
    vpcl::re_register *TCRE  ;
    vpcl::re_register *RLDCRE;
    vpcl::re_register *POECRE;
    vpcl::re_register *SOLVLE;
    vpcl::re_register *TSRE  ;
    vpcl::re_register *TSCRE ;
    vpcl::re_register *TOCRE ;
    vpcl::re_register *TIERE ;
    vpcl::re_register *PSCCRE[4];
    vpcl::re_register *FCTRGE;
    vpcl::re_register *TCNTE [4];
    vpcl::re_register *CYLRE [4];
    vpcl::re_register *DTRE  [4];
    vpcl::re_register *CRLDE [4];
    vpcl::re_register *DRLDE [4];

    #ifdef USR_CWR_SYSTEMC
    scml2::memory<uint> cwmem;
    scml2::reg<uint>      *CW_SSTRE_;
    scml2::reg<uint>      *CW_PSCRE_;
    scml2::reg<uint>      *CW_TCRE_RLDCRE;
    scml2::reg<uint>      *CW_POECRE_SOLVLE;
    scml2::reg<uint>      *CW_TSRE_TSCRE;
    scml2::reg<uint>      *CW_TOCRE_TIERE;
    scml2::reg<uint>      *CW_PSCCRE_0_PSCCRE_1_PSCCRE_2_PSCCRE_3;
    scml2::reg<uint>      *CW_FCTRGE_;
    scml2::bitfield<uint> *CW_SSTRE ;
    scml2::bitfield<uint> *CW_PSCRE ;
    scml2::bitfield<uint> *CW_TCRE  ;
    scml2::bitfield<uint> *CW_RLDCRE;
    scml2::bitfield<uint> *CW_POECRE;
    scml2::bitfield<uint> *CW_SOLVLE;
    scml2::bitfield<uint> *CW_TSRE  ;
    scml2::bitfield<uint> *CW_TSCRE ;
    scml2::bitfield<uint> *CW_TOCRE ;
    scml2::bitfield<uint> *CW_TIERE ;
    scml2::bitfield<uint> *CW_PSCCRE[4];
    scml2::bitfield<uint> *CW_FCTRGE;
    scml2::reg<uint>      *CW_TCNTE [4];
    scml2::reg<uint>      *CW_CYLRE [4];
    scml2::reg<uint>      *CW_DTRE  [4];
    scml2::reg<uint>      *CW_CRLDE [4];
    scml2::reg<uint>      *CW_DRLDE [4];
    #endif

    uint SSTRE_SSTRE0;
    uint SSTRE_SSTRE1;
    uint SSTRE_SSTRE2;
    uint SSTRE_SSTRE3;
    uint PSCRE_PSCE;
    uint TCRE_CKSELE;
    uint TCRE_PSCSEL;
    uint RLDCRE_RLDENE0;
    uint RLDCRE_RLDENE1;
    uint RLDCRE_RLDENE2;
    uint RLDCRE_RLDENE3;
    uint POECRE_POEEN0;
    uint POECRE_POEEN1;
    uint POECRE_POEEN2;
    uint POECRE_POEEN3;
    uint POECRE_POEPOL;
    uint POECRE_POECRKEY;
    uint SOLVLE_PWMSLV0;
    uint SOLVLE_PWMSLV1;
    uint SOLVLE_PWMSLV2;
    uint SOLVLE_PWMSLV3;
    uint TSRE_CMFE0;
    uint TSRE_CMFE1;
    uint TSRE_CMFE2;
    uint TSRE_CMFE3;
    uint TSRE_OVFE0;
    uint TSRE_OVFE1;
    uint TSRE_OVFE2;
    uint TSRE_OVFE3;
    uint TSRE_DMFE0;
    uint TSRE_DMFE1;
    uint TSRE_DMFE2;
    uint TSRE_DMFE3;
    uint TSCRE_CMFCE0;
    uint TSCRE_CMFCE1;
    uint TSCRE_CMFCE2;
    uint TSCRE_CMFCE3;
    uint TSCRE_OVFCE0;
    uint TSCRE_OVFCE1;
    uint TSCRE_OVFCE2;
    uint TSCRE_OVFCE3;
    uint TSCRE_DMFCE0;
    uint TSCRE_DMFCE1;
    uint TSCRE_DMFCE2;
    uint TSCRE_DMFCE3;
    uint TOCRE_TONEE0;
    uint TOCRE_TONEE1;
    uint TOCRE_TONEE2;
    uint TOCRE_TONEE3;
    uint TIERE_CMEE0;
    uint TIERE_CMEE1;
    uint TIERE_CMEE2;
    uint TIERE_CMEE3;
    uint TIERE_DMEE0;
    uint TIERE_DMEE1;
    uint TIERE_DMEE2;
    uint TIERE_DMEE3;
    uint PSCCRE_PSCCE[4];
    uint FCTRGE_FCMT0;
    uint FCTRGE_FCMT1;
    uint FCTRGE_FCMT2;
    uint FCTRGE_FCMT3;
    uint TCNTE_TCNTE[4];
    uint CYLRE_CYLRE[4];
    uint DTRE_DTRE[4];
    uint CRLDE_CRLDE[4];
    uint DRLDE_DRLDE[4];

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

    virtual void cb_SSTRE_SSTRE0(RegCBstr str) = 0;
    virtual void cb_PSCRE_PSCE(RegCBstr str) = 0;
    virtual void cb_TCRE_CKSELE(RegCBstr str) = 0;
    virtual void cb_RLDCRE_RLDENE0(RegCBstr str) = 0;
    virtual void cb_POECRE_POECRKEY(RegCBstr str) = 0;
    virtual void cb_TSCRE_CMFCE0(RegCBstr str) = 0;
    virtual void cb_TOCRE_TONEE0(RegCBstr str) = 0;
    virtual void cb_PSCCRE_PSCCE(RegCBstr str) = 0;
    virtual void cb_FCTRGE_FCMT0(RegCBstr str) = 0;
    virtual void cb_TCNTE_TCNTE(RegCBstr str) = 0;
    virtual void cb_CYLRE_CYLRE(RegCBstr str) = 0;
    virtual void cb_DTRE_DTRE(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Catu5_timere_sub_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Catu5_timere_sub_regif::*) (RegCBstr)> > mRdCbAPI;
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
#endif //__ATU5_TIMERE_SUB_REG_DEF_H__
