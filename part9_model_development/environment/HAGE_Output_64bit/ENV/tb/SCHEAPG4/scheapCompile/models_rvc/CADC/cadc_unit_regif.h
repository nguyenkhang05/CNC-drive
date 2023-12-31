// -----------------------------------------------------------------------------
// $Id: cadc_unit_regif.h,v 1.1 2018/11/19 02:50:58 chanle Exp $
//
// Copyright(c) 2016-2018 Renesas Electronics Corporation
// Copyright(c) 2016-2018 Renesas Design Vietnam Co., Ltd.
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
// Input file : CADC/cadc_unit_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE CADC_UNIT
//     #                   name            offset_size
//     %%REG_INSTANCE      reg_def         8
// 
// %REG_CHANNEL reg_def
//     %%TITLE  name          reg_name      wsize    rsize    length  offset  factor_start  factor_end  factor_index  factor_step  access init        support callback
//     %%REG    CADCnVCR      CADCnVCR      8|16|32  8|16|32  32      0x00    0             7           -             -            W|R    0           TRUE    -
//     %%REG    CADCnDIR      CADCnDIR      8|16|32  8|16|32  32      0x20    0             7           -             -            R      0           TRUE    -
//     %%REG    CADCnADSTCR   CADCnADSTCR   8        8|16|32  8       0x40    -             -           -             -            W      0           TRUE    W
//     %%REG    CADCnADENDCR  CADCnADENDCR  8        8|16|32  8       0x44    -             -           -             -            W      0           TRUE    W
//     %%REG    CADCnCLBSTCR  CADCnCLBSTCR  8        8|16|32  8       0x48    -             -           -             -            W      0           TRUE    W
//     %%REG    CADCnCLBEDCR  CADCnCLBEDCR  8        8|16|32  8       0x4C    -             -           -             -            W      0           TRUE    W
//     %%REG    CADCnADTCR    CADCnADTCR    8        8|16|32  8       0x50    -             -           -             -            W|R    0           TRUE    -
//     %%REG    CADCnUCR      CADCnUCR      8|16|32  8|16|32  32      0x54    -             -           -             -            W|R    0           TRUE    -
//     %%REG    CADCnVCPTRR   CADCnVCPTRR   8        8|16|32  8       0x58    -             -           -             -            W|R    0           TRUE    W
//     %%REG    CADCnADSR     CADCnADSR     8|16|32  8|16|32  8       0x60    -             -           -             -            R      0           TRUE    -
//     %%REG    CADCnUDPTRR   CADCnUDPTRR   8|16|32  8|16|32  8       0x64    -             -           -             -            R      0           TRUE    -
//     %%REG    CADCnUDIR     CADCnUDIR     8|16|32  8|16|32  32      0x68    -             -           -             -            R      0           TRUE    -
//     %%REG    CADCnSMPCR    CADCnSMPCR    8|16|32  8|16|32  32      0x6C    -             -           -             -            R|W    0x00010707  TRUE    -
//     %%REG    CADCnSFTCR    CADCnSFTCR    8        8|16|32  8       0x70    -             -           -             -            W|R    0           TRUE    -
//     %%REG    CADCnECR      CADCnECR      8        8|16|32  8       0x74    -             -           -             -            W      0           TRUE    W
//     %%REG    CADCnER       CADCnER       8|16|32  8|16|32  32      0x78    -             -           -             -            R      0           TRUE    -
//     %%REG    CADCnTDLVR    CADCnTDLVR    8        8|16|32  8       0x7C    -             -           -             -            W|R    0           TRUE    -
//     %%REG    CADCnULTBR    CADCnULTBR    16|32    8|16|32  32      0x80    0             3           -             -            W|R    0x7FFF8000  TRUE    -
// 
// 
// %REG_NAME CADCnVCR
//     %%TITLE  name        upper  lower  init  access  support  callback
//     %%BIT    CADC00VCULME      27     27     0     W|R     TRUE     -
//     %%BIT    CADC00VCLLME      26     26     0     W|R     TRUE     -
//     %%BIT    CADC00VCULLMTBS   25     24     0     W|R     TRUE     -
//     %%BIT    CADC00ADIE        15     15     0     W|R     TRUE     -
//     %%BIT    CADC00ULEIE       14     14     0     W|R     TRUE     -
//     %%BIT    CADC00DFENT       12     12     0     W|R     TRUE     -
//     %%BIT    CADC00DFTAG       11     8      0     W|R     TRUE     -
//     %%BIT    CADC00CNVCLS      5      4      0     W|R     TRUE     -
//     %%BIT    CADC00GCTRL       3      0      0     W|R     TRUE     -
// 
// %REG_NAME CADCnDIR
//     %%TITLE  name        upper  lower  init  access  support  callback
//     %%BIT    CADC00IDEF          26     26     0     R       TRUE     -
//     %%BIT    CADC00WFLG        25     25     0     R       TRUE     -
//     %%BIT    CADC00PRTY        24     24     0     R       TRUE     -
//     %%BIT    CADC00DR          15     0      0     R       TRUE     -
// 
// %REG_NAME CADCnADSTCR
//     %%TITLE  name        upper  lower  init  access  support  callback
//     %%BIT    CADC00ADST        0      0      0     W       TRUE     W
// 
// %REG_NAME CADCnADENDCR
//     %%TITLE  name        upper  lower  init  access  support  callback
//     %%BIT    CADC00ADEND       0      0      0     W       TRUE     W
// 
// %REG_NAME CADCnCLBSTCR
//     %%TITLE  name        upper  lower  init  access  support  callback
//     %%BIT    CADC00CLBST       0      0      0     W       TRUE     W
// 
// %REG_NAME CADCnCLBEDCR
//     %%TITLE  name        upper  lower  init  access  support  callback
//     %%BIT    CADC00CLBEND      0      0      0     W       TRUE     W
// 
// %REG_NAME CADCnADTCR
//     %%TITLE  name        upper  lower  init  access  support  callback
//     %%BIT    CADC00ENDTRGE     1      1      0     W|R     TRUE     -
//     %%BIT    CADC00STTRGE      0      0      0     W|R     TRUE     -
// 
// %REG_NAME CADCnUCR
//     %%TITLE  name        upper  lower  init  access  support  callback
//     %%BIT    CADC00VPRSTE      29     29     0     W|R     TRUE     -
//     %%BIT    CADC00RDMA        28     28     0     W|R     TRUE     -
//     %%BIT    CADC00DFMT        11     8      0     W|R     TRUE     -
//     %%BIT    CADC00VCEP        2      0      0     W|R     TRUE     -
// 
// %REG_NAME CADCnVCPTRR
//     %%TITLE  name        upper  lower  init  access  support  callback
//     %%BIT    CADC00VCPTR       2      0      0     W|R     TRUE     W
// 
// %REG_NAME CADCnADSR
//     %%TITLE  name        upper  lower  init  access  support  callback
//     %%BIT    CADC00CLBACT      1      1      0     R       TRUE     -
//     %%BIT    CADC00ADACT       0      0      0     R       TRUE     -
// 
// %REG_NAME CADCnUDPTRR
//     %%TITLE  name        upper  lower  init  access  support  callback
//     %%BIT    CADC00UDPTR       2      0      0     R       TRUE     -
// 
// %REG_NAME CADCnUDIR
//     %%TITLE  name        upper  lower  init  access  support  callback
//     %%BIT    CADC00CHNUM       31     29     0     R       TRUE     -
//     %%BIT    CADC00UIDEF       26     26     0     R       TRUE     -
//     %%BIT    CADC00UWFLG       25     25     0     R       TRUE     -
//     %%BIT    CADC00UPRTY       24     24     0     R       TRUE     -
//     %%BIT    CADC00UDR         15     0      0     R       TRUE     -
// 
// %REG_NAME CADCnSMPCR
//     %%TITLE  name        upper  lower  init  access  support  callback
//     %%BIT    CADC00ODAV       16     16     0x1   R|W       TRUE     -
// 
// %REG_NAME CADCnSFTCR
//     %%TITLE  name        upper  lower  init  access  support  callback
//     %%BIT    CADC00RDCLRE      7      7      0     W|R     TRUE     -
//     %%BIT    CADC00OWEIE       2      2      0     W|R     TRUE     -
//     %%BIT    CADC00PEIE        1      1      0     W|R     TRUE     -
//     %%BIT    CADC00IDEIE       0      0      0     W|R     TRUE     -
// 
// %REG_NAME CADCnECR
//     %%TITLE  name        upper  lower  init  access  support  callback
//     %%BIT    CADC00ULEC        4      4      0     W       TRUE     W
//     %%BIT    CADC00LLEC        3      3      0     W       TRUE     -
//     %%BIT    CADC00OWEC        2      2      0     W       TRUE     -
//     %%BIT    CADC00PEC         1      1      0     W       TRUE     -
// 
// %REG_NAME CADCnER
//     %%TITLE  name        upper  lower  init  access  support  callback
//     %%BIT    CADC00ULE         31     31     0     R       TRUE     -
//     %%BIT    CADC00LLE         30     30     0     R       TRUE     -
//     %%BIT    CADC00ULECAP      26     24     0     R       TRUE     -
//     %%BIT    CADC00OWE         23     23     0     R       TRUE     -
//     %%BIT    CADC00OWECAP      18     16     0     R       TRUE     -
//     %%BIT    CADC00PE          15     15     0     R       TRUE     -
//     %%BIT    CADC00PECAP       10     8      0     R       TRUE     -
// 
// %REG_NAME CADCnTDLVR
//     %%TITLE  name        upper  lower  init  access  support  callback
//     %%BIT    CADC00AN3NLV      7      7      0     W|R     TRUE     -
//     %%BIT    CADC00AN3PLV      6      6      0     W|R     TRUE     -
//     %%BIT    CADC00AN2NLV      5      5      0     W|R     TRUE     -
//     %%BIT    CADC00AN2PLV      4      4      0     W|R     TRUE     -
//     %%BIT    CADC00AN1NLV      3      3      0     W|R     TRUE     -
//     %%BIT    CADC00AN1PLV      2      2      0     W|R     TRUE     -
//     %%BIT    CADC00AN0NLV      1      1      0     W|R     TRUE     -
//     %%BIT    CADC00AN0PLV      0      0      0     W|R     TRUE     -
// 
// %REG_NAME CADCnULTBR
//     %%TITLE  name        upper  lower  init    access  support  callback
//     %%BIT    CADC00ULMTB       31     16     0x7FFF  W|R     TRUE     -
//     %%BIT    CADC00LLMTB       15     0      0x8000  W|R     TRUE     -
////////////////////////////////////////////////////////////////////////////////
/// @file cadc_unit_regif.h
/// @brief Register IF class of model ADC_UNIT
/// $Id: cadc_unit_regif.h,v 1.1 2018/11/19 02:50:58 chanle Exp $
/// $Date: 2018/11/19 02:50:58 $
/// $Revison$
/// $Author: chanle $
////////////////////////////////////////////////////////////////////////////////
#ifndef __CADC_UNIT_REGIF_H__
#define __CADC_UNIT_REGIF_H__
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

/// Register IF class of ADC_UNIT model
class Ccadc_unit_regif
: public vpcl::reg_super
{
protected:
    typedef const unsigned int cuint;
    typedef unsigned int uint;

    enum eRegGroup {
        emNum_of_gr
    }; ///< Enumeration for register group index
    enum eRegIndex {
        emNUM_CADCnVCR     = 8,
        emNUM_CADCnDIR     = 8,
        emNUM_CADCnULTBR   = 4
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
    Ccadc_unit_regif(std::string name, uint buswidth);
    ~Ccadc_unit_regif();

protected:
    vpcl::re_register *CADCnVCR    [8];
    vpcl::re_register *CADCnDIR    [8];
    vpcl::re_register *CADCnADSTCR ;
    vpcl::re_register *CADCnADENDCR;
    vpcl::re_register *CADCnCLBSTCR;
    vpcl::re_register *CADCnCLBEDCR;
    vpcl::re_register *CADCnADTCR  ;
    vpcl::re_register *CADCnUCR    ;
    vpcl::re_register *CADCnVCPTRR ;
    vpcl::re_register *CADCnADSR   ;
    vpcl::re_register *CADCnUDPTRR ;
    vpcl::re_register *CADCnUDIR   ;
    vpcl::re_register *CADCnSMPCR  ;
    vpcl::re_register *CADCnSFTCR  ;
    vpcl::re_register *CADCnECR    ;
    vpcl::re_register *CADCnER     ;
    vpcl::re_register *CADCnTDLVR  ;
    vpcl::re_register *CADCnULTBR  [4];

    #ifdef CWR_SYSTEMC
    scml2::memory<uint> cwmem;
    scml2::reg<uint>      *CW_CADCnADSTCR_;
    scml2::reg<uint>      *CW_CADCnADENDCR_;
    scml2::reg<uint>      *CW_CADCnCLBSTCR_;
    scml2::reg<uint>      *CW_CADCnCLBEDCR_;
    scml2::reg<uint>      *CW_CADCnADTCR_ ;
    scml2::reg<uint>      *CW_CADCnVCPTRR_;
    scml2::reg<uint>      *CW_CADCnADSR_  ;
    scml2::reg<uint>      *CW_CADCnUDPTRR_;
    scml2::reg<uint>      *CW_CADCnSFTCR_ ;
    scml2::reg<uint>      *CW_CADCnECR_   ;
    scml2::reg<uint>      *CW_CADCnTDLVR_ ;
    scml2::reg<uint>      *CW_CADCnVCR    [8];
    scml2::reg<uint>      *CW_CADCnDIR    [8];
    scml2::bitfield<uint> *CW_CADCnADSTCR ;
    scml2::bitfield<uint> *CW_CADCnADENDCR;
    scml2::bitfield<uint> *CW_CADCnCLBSTCR;
    scml2::bitfield<uint> *CW_CADCnCLBEDCR;
    scml2::bitfield<uint> *CW_CADCnADTCR  ;
    scml2::reg<uint>      *CW_CADCnUCR    ;
    scml2::bitfield<uint> *CW_CADCnVCPTRR ;
    scml2::bitfield<uint> *CW_CADCnADSR   ;
    scml2::bitfield<uint> *CW_CADCnUDPTRR ;
    scml2::reg<uint>      *CW_CADCnUDIR   ;
    scml2::reg<uint>      *CW_CADCnSMPCR  ;
    scml2::bitfield<uint> *CW_CADCnSFTCR  ;
    scml2::bitfield<uint> *CW_CADCnECR    ;
    scml2::reg<uint>      *CW_CADCnER     ;
    scml2::bitfield<uint> *CW_CADCnTDLVR  ;
    scml2::reg<uint>      *CW_CADCnULTBR  [4];
    #endif

    uint CADCnVCR_CADC00VCULME[8];
    uint CADCnVCR_CADC00VCLLME[8];
    uint CADCnVCR_CADC00VCULLMTBS[8];
    uint CADCnVCR_CADC00ADIE[8];
    uint CADCnVCR_CADC00ULEIE[8];
    uint CADCnVCR_CADC00DFENT[8];
    uint CADCnVCR_CADC00DFTAG[8];
    uint CADCnVCR_CADC00CNVCLS[8];
    uint CADCnVCR_CADC00GCTRL[8];
    uint CADCnDIR_CADC00IDEF[8];
    uint CADCnDIR_CADC00WFLG[8];
    uint CADCnDIR_CADC00PRTY[8];
    uint CADCnDIR_CADC00DR[8];
    uint CADCnADSTCR_CADC00ADST;
    uint CADCnADENDCR_CADC00ADEND;
    uint CADCnCLBSTCR_CADC00CLBST;
    uint CADCnCLBEDCR_CADC00CLBEND;
    uint CADCnADTCR_CADC00ENDTRGE;
    uint CADCnADTCR_CADC00STTRGE;
    uint CADCnUCR_CADC00VPRSTE;
    uint CADCnUCR_CADC00RDMA;
    uint CADCnUCR_CADC00DFMT;
    uint CADCnUCR_CADC00VCEP;
    uint CADCnVCPTRR_CADC00VCPTR;
    uint CADCnADSR_CADC00CLBACT;
    uint CADCnADSR_CADC00ADACT;
    uint CADCnUDPTRR_CADC00UDPTR;
    uint CADCnUDIR_CADC00CHNUM;
    uint CADCnUDIR_CADC00UIDEF;
    uint CADCnUDIR_CADC00UWFLG;
    uint CADCnUDIR_CADC00UPRTY;
    uint CADCnUDIR_CADC00UDR;
    uint CADCnSMPCR_CADC00ODAV;
    uint CADCnSFTCR_CADC00RDCLRE;
    uint CADCnSFTCR_CADC00OWEIE;
    uint CADCnSFTCR_CADC00PEIE;
    uint CADCnSFTCR_CADC00IDEIE;
    uint CADCnECR_CADC00ULEC;
    uint CADCnECR_CADC00LLEC;
    uint CADCnECR_CADC00OWEC;
    uint CADCnECR_CADC00PEC;
    uint CADCnER_CADC00ULE;
    uint CADCnER_CADC00LLE;
    uint CADCnER_CADC00ULECAP;
    uint CADCnER_CADC00OWE;
    uint CADCnER_CADC00OWECAP;
    uint CADCnER_CADC00PE;
    uint CADCnER_CADC00PECAP;
    uint CADCnTDLVR_CADC00AN3NLV;
    uint CADCnTDLVR_CADC00AN3PLV;
    uint CADCnTDLVR_CADC00AN2NLV;
    uint CADCnTDLVR_CADC00AN2PLV;
    uint CADCnTDLVR_CADC00AN1NLV;
    uint CADCnTDLVR_CADC00AN1PLV;
    uint CADCnTDLVR_CADC00AN0NLV;
    uint CADCnTDLVR_CADC00AN0PLV;
    uint CADCnULTBR_CADC00ULMTB[4];
    uint CADCnULTBR_CADC00LLMTB[4];

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

    virtual void cb_CADCnADSTCR_CADC00ADST(RegCBstr str) = 0;
    virtual void cb_CADCnADENDCR_CADC00ADEND(RegCBstr str) = 0;
    virtual void cb_CADCnCLBSTCR_CADC00CLBST(RegCBstr str) = 0;
    virtual void cb_CADCnCLBEDCR_CADC00CLBEND(RegCBstr str) = 0;
    virtual void cb_CADCnVCPTRR_CADC00VCPTR(RegCBstr str) = 0;
    virtual void cb_CADCnECR_CADC00ULEC(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Ccadc_unit_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Ccadc_unit_regif::*) (RegCBstr)> > mRdCbAPI;
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
#endif //__CADC_UNIT_REG_DEF_H__
