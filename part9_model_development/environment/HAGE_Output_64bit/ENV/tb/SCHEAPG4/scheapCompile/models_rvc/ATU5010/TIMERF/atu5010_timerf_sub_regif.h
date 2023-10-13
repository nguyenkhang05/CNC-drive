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
// Input file : /home/truongvv2/previous_data/ATU5010_Output/src/ATU5010/TIMERF/atu5010_timerf_sub_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE ATU5010_TIMERF_SUB
//     #              name      offset_size
//     %%REG_INSTANCE  reg_def   6
// 
// %REG_CHANNEL reg_def
//     %%TITLE      name      reg_name  wsize     rsize     length    offset    factor_start   factor_end  factor_index    factor_step     access    init         support   callback
//     %%REG        TCR1F     TCR1F     8         8|16|32   8         0x00      -              -           -               -               R/W       0x00         TRUE      W
//     %%REG        TCR2F     TCR2F     8         8|16|32   8         0x02      -              -           -               -               R/W       0x00         TRUE      -
//     %%REG        TIERF     TIERF     8         8|16|32   8         0x07      -              -           -               -               R/W       0x00         TRUE      -
//     %%REG        BKCRF     BKCRF     8         8|16|32   8         0x04      -              -           -               -               R|W       0x00         TRUE      W
//     %%REG        TSRF      TSRF      -         8|16|32   8         0x05      -              -           -               -               R         0x00         TRUE      -
//     %%REG        TSCRF     TSCRF     8         8|16|32   8         0x06      -              -           -               -               R|W       0x00         TRUE      W
//     %%REG        ECNTAF    ECNTAF    32        8|16|32   32        0x10      -              -           -               -               R|W       0x00000000   TRUE      -
//     %%REG        ECNTBF    ECNTBF    16        8|16|32   16        0x18      -              -           -               -               R|W       0x0000       TRUE      -
//     %%REG        ECNTCF    ECNTCF    32        8|16|32   32        0x20      -              -           -               -               R|W       0x00000000   TRUE      -
//     %%REG        GRAF      GRAF      32        8|16|32   32        0x14      -              -           -               -               R|W       0xFFFFFFFF   TRUE      W
//     %%REG        GRBF      GRBF      16        8|16|32   16        0x1C      -              -           -               -               R|W       0xFFFF       TRUE      -
//     %%REG        GRCF      GRCF      32        8|16|32   32        0x24      -              -           -               -               R|W       0xFFFFFFFF   TRUE      W
//     %%REG        GRDF      GRDF      32        8|16|32   32        0x28      -              -           -               -               R|W       0xFFFFFFFF   TRUE      W
//     %%REG        CDRF      CDRF      -         8|16|32   32        0x2C      -              -           -               -               R         0x00000000   TRUE      R
//     %%REG        NCNTFA    NCNTFA    16        8|16|32   16        0x08      -              -           -               -               R|W       0x0000       TRUE      -
//     %%REG        NCNTFB    NCNTFB    16        8|16|32   16        0x0C      -              -           -               -               R|W       0x0000       TRUE      -
//     %%REG        NCRFA     NCRFA     16        8|16|32   16        0x0A      -              -           -               -               R|W       0x0000       TRUE      -
//     %%REG        NCRFB     NCRFB     16        8|16|32   16        0x0E      -              -           -               -               R|W       0x0000       TRUE      -
// 
// 
// 
// %REG_NAME        TCR1F
// %%TITLE          name      upper     lower     init         access    support   callback
// %%BIT            CKSELFx   7         5         0            R|W       TRUE      -
// %%BIT            MDF       4         2         0            R|W       TRUE      W
// %%BIT            EGSELF    1         0         0            R|W       TRUE      -
// 
// 
// %REG_NAME        TCR2F
// %%TITLE          name      upper     lower     init         access    support   callback
// %%BIT            EISELEF   7         7         0            R|W       TRUE      -
// %%BIT            EISELF    2         0         0            R|W       TRUE      -
// 
// 
// %REG_NAME        TIERF
// %%TITLE          name      upper     lower     init         access    support   callback
// %%BIT            OVECF     3         3         0            R|W       TRUE      -
// %%BIT            OVEBF     2         2         0            R|W       TRUE      -
// %%BIT            OVEAF     1         1         0            R|W       TRUE      -
// %%BIT            ICIEF     0         0         0            R|W       TRUE      -
// 
// 
// %REG_NAME        BKCRF
// %%TITLE          name      upper     lower     init         access    support   callback
// %%BIT            BKENAF    6         6         0            R|W       TRUE      W
// %%BIT            BKENCF    5         5         0            R|W       TRUE      -
// %%BIT            BKENDF    4         4         0            R|W       TRUE      -
// %%BIT            ARSWAF    2         2         0            R|W       TRUE      -
// %%BIT            ARSWCF    1         1         0            R|W       TRUE      -
// %%BIT            ARSWDF    0         0         0            R|W       TRUE      -
// 
// 
// %REG_NAME        TSRF
// %%TITLE          name      upper     lower     init         access    support   callback
// %%BIT            OVFCF     3         3         0            R         TRUE      -
// %%BIT            OVFBF     2         2         0            R         TRUE      -
// %%BIT            OVFAF     1         1         0            R         TRUE      -
// %%BIT            ICFF      0         0         0            R         TRUE      -
// 
// 
// %REG_NAME        TSCRF
// %%TITLE          name      upper     lower     init         access    support   callback
// %%BIT            OVFCCF    3         3         0            W         TRUE      W
// %%BIT            OVFCBF    2         2         0            W         TRUE      W
// %%BIT            OVFCAF    1         1         0            W         TRUE      W
// %%BIT            ICFCF     0         0         0            W         TRUE      W
// 
// 
// %REG_NAME        ECNTAF
// %%TITLE          name      upper     lower     init         access    support   callback
// %%BIT            ECNTAF    31        0         0            R|W       TRUE      -
// 
// 
// %REG_NAME        ECNTBF
// %%TITLE          name      upper     lower     init         access    support   callback
// %%BIT            ECNTBF    15        0         0            R|W       TRUE      -
// 
// 
// %REG_NAME        ECNTCF
// %%TITLE          name      upper     lower     init         access    support   callback
// %%BIT            ECNTCF    31        0         0            R|W       TRUE      -
// 
// 
// %REG_NAME        GRAF
// %%TITLE          name      upper     lower     init         access    support   callback
// %%BIT            GRAF      31        0         0xFFFFFFFF   R|W       TRUE      W
// 
// 
// %REG_NAME        GRBF
// %%TITLE          name      upper     lower     init         access    support   callback
// %%BIT            GRBF      15        0         0xFFFF       R|W       TRUE      -
// 
// 
// %REG_NAME        GRCF
// %%TITLE          name      upper     lower     init         access    support   callback
// %%BIT            GRCF      31        0         0xFFFFFFFF   R|W       TRUE      W
// 
// 
// %REG_NAME        GRDF
// %%TITLE          name      upper     lower     init         access    support   callback
// %%BIT            GRDF      31        0         0xFFFFFFFF   R|W       TRUE      W
// 
// 
// %REG_NAME        CDRF
// %%TITLE          name      upper     lower     init         access    support   callback
// %%BIT            CDRF      31        0         0            R         TRUE      R
// 
// 
// %REG_NAME        NCNTFA
// %%TITLE          name      upper     lower     init         access    support   callback
// %%BIT            NCNTFA    15        0         0            R|W       TRUE      -
// 
// 
// %REG_NAME        NCNTFB
// %%TITLE          name      upper     lower     init         access    support   callback
// %%BIT            NCNTFB    15        0         0            R|W       TRUE      -
// 
// 
// %REG_NAME        NCRFA
// %%TITLE          name      upper     lower     init         access    support   callback
// %%BIT            NCTFA     15        0         0            R|W       TRUE      -
// 
// 
// %REG_NAME        NCRFB
// %%TITLE          name      upper     lower     init         access    support   callback
// %%BIT            NCTFB     15        0         0            R|W       TRUE      -
////////////////////////////////////////////////////////////////////////////////
/// @file atu5010_timerf_sub_regif.h
/// @brief Register IF class of model ATU5010_TIMERF_SUB
/// $Id$
/// $Date$
/// $Revison$
/// $Author$
////////////////////////////////////////////////////////////////////////////////
#ifndef __ATU5010_TIMERF_SUB_REGIF_H__
#define __ATU5010_TIMERF_SUB_REGIF_H__
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

/// Register IF class of ATU5010_TIMERF_SUB model
class Catu5010_timerf_sub_regif
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
    Catu5010_timerf_sub_regif(std::string name, uint buswidth);
    ~Catu5010_timerf_sub_regif();

protected:
    vpcl::re_register *TCR1F ;
    vpcl::re_register *TCR2F ;
    vpcl::re_register *TIERF ;
    vpcl::re_register *BKCRF ;
    vpcl::re_register *TSRF  ;
    vpcl::re_register *TSCRF ;
    vpcl::re_register *ECNTAF;
    vpcl::re_register *ECNTBF;
    vpcl::re_register *ECNTCF;
    vpcl::re_register *GRAF  ;
    vpcl::re_register *GRBF  ;
    vpcl::re_register *GRCF  ;
    vpcl::re_register *GRDF  ;
    vpcl::re_register *CDRF  ;
    vpcl::re_register *NCNTFA;
    vpcl::re_register *NCNTFB;
    vpcl::re_register *NCRFA ;
    vpcl::re_register *NCRFB ;

    #ifdef CWR_SYSTEMC
    scml2::memory<uint> cwmem;
    scml2::reg<uint>      *CW_TCR1F_TCR2F;
    scml2::reg<uint>      *CW_BKCRF_TSRF_TSCRF_TIERF;
    scml2::reg<uint>      *CW_NCNTFA_NCRFA;
    scml2::reg<uint>      *CW_NCNTFB_NCRFB;
    scml2::reg<uint>      *CW_ECNTBF_;
    scml2::reg<uint>      *CW_GRBF_ ;
    scml2::bitfield<uint> *CW_TCR1F ;
    scml2::bitfield<uint> *CW_TCR2F ;
    scml2::bitfield<uint> *CW_TIERF ;
    scml2::bitfield<uint> *CW_BKCRF ;
    scml2::bitfield<uint> *CW_TSRF  ;
    scml2::bitfield<uint> *CW_TSCRF ;
    scml2::reg<uint>      *CW_ECNTAF;
    scml2::bitfield<uint> *CW_ECNTBF;
    scml2::reg<uint>      *CW_ECNTCF;
    scml2::reg<uint>      *CW_GRAF  ;
    scml2::bitfield<uint> *CW_GRBF  ;
    scml2::reg<uint>      *CW_GRCF  ;
    scml2::reg<uint>      *CW_GRDF  ;
    scml2::reg<uint>      *CW_CDRF  ;
    scml2::bitfield<uint> *CW_NCNTFA;
    scml2::bitfield<uint> *CW_NCNTFB;
    scml2::bitfield<uint> *CW_NCRFA ;
    scml2::bitfield<uint> *CW_NCRFB ;
    #endif

    uint TCR1F_CKSELFx;
    uint TCR1F_MDF;
    uint TCR1F_EGSELF;
    uint TCR2F_EISELEF;
    uint TCR2F_EISELF;
    uint TIERF_OVECF;
    uint TIERF_OVEBF;
    uint TIERF_OVEAF;
    uint TIERF_ICIEF;
    uint BKCRF_BKENAF;
    uint BKCRF_BKENCF;
    uint BKCRF_BKENDF;
    uint BKCRF_ARSWAF;
    uint BKCRF_ARSWCF;
    uint BKCRF_ARSWDF;
    uint TSRF_OVFCF;
    uint TSRF_OVFBF;
    uint TSRF_OVFAF;
    uint TSRF_ICFF;
    uint TSCRF_OVFCCF;
    uint TSCRF_OVFCBF;
    uint TSCRF_OVFCAF;
    uint TSCRF_ICFCF;
    uint ECNTAF_ECNTAF;
    uint ECNTBF_ECNTBF;
    uint ECNTCF_ECNTCF;
    uint GRAF_GRAF;
    uint GRBF_GRBF;
    uint GRCF_GRCF;
    uint GRDF_GRDF;
    uint CDRF_CDRF;
    uint NCNTFA_NCNTFA;
    uint NCNTFB_NCNTFB;
    uint NCRFA_NCTFA;
    uint NCRFB_NCTFB;

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

    virtual void cb_TCR1F_MDF(RegCBstr str) = 0;
    virtual void cb_BKCRF_BKENAF(RegCBstr str) = 0;
    virtual void cb_TSCRF_OVFCCF(RegCBstr str) = 0;
    virtual void cb_TSCRF_OVFCBF(RegCBstr str) = 0;
    virtual void cb_TSCRF_OVFCAF(RegCBstr str) = 0;
    virtual void cb_TSCRF_ICFCF(RegCBstr str) = 0;
    virtual void cb_GRAF_GRAF(RegCBstr str) = 0;
    virtual void cb_GRCF_GRCF(RegCBstr str) = 0;
    virtual void cb_GRDF_GRDF(RegCBstr str) = 0;
    virtual void cb_CDRF_CDRF(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Catu5010_timerf_sub_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Catu5010_timerf_sub_regif::*) (RegCBstr)> > mRdCbAPI;
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
#endif //__ATU5010_TIMERF_SUB_REG_DEF_H__
