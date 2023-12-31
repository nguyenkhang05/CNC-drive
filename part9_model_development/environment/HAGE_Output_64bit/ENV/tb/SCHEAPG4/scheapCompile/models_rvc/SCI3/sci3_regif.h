// -----------------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2010-2014 Renesas Electronics Corporation
// Copyright(c) 2010-2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------
// This file is generated by Register I/F generator
//    gen_regif_class.py 1.64 2015/03/30 03:05:44 ducduong
//
// Input file : sci3_regif.txt
////////////////////////////////////////////////////////////////////////////////
//  %MODULE SCI3 
//     #              name     offset_size
//     %%REG_INSTANCE reg_def  5
// 
// %REG_CHANNEL reg_def
//     %%TITLE    name       reg_name    wsize         rsize     length  offset    access  init     support  callback
//     %%REG      SMR        SMR         8             8|16|32   8       0x00      -       0        TRUE     -
//     %%REG      BRR        BRR         8             8|16|32   8       0x04      -       0xFF     TRUE     -
//     %%REG      SCR        SCR         8             8|16|32   8       0x08      -       0        TRUE     - 
//     %%REG      TDR        TDR         8|16          8|16|32   16      0x0C      -       0xFFFF   TRUE     -  
//     %%REG      SSR        SSR         8             8|16|32   8       0x10      -       0x84     TRUE     -
//     %%REG      RDR        RDR         8|16          8|16|32   16      0x14      R       0        TRUE     -
//     %%REG      SCMR       SCMR        8             8|16|32   8       0x18      -       0xF2     TRUE     -
//     %%REG      SEMR       SEMR        8             8|16|32   8       0x1C      -       0x04     TRUE     -
//                        
// %REG_NAME RDR      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    RDR            15      0      0     R       TRUE     R   
// 
// %REG_NAME TDR      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    TDR            15      0     0xFFFF   R|W     TRUE     W    
//     
// %REG_NAME SMR      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    CM             7      7      0     R|W     TRUE     W    
//     %%BIT    CHR            6      6      0     R|W     TRUE     -    
//     %%BIT    PE             5      5      0     R|W     TRUE     -    
//     %%BIT    PM             4      4      0     R|W     TRUE     -    
//     %%BIT    STOP           3      3      0     R|W     TRUE     -    
//     %%BIT    MP             2      2      0     R|W     TRUE     -    
//     %%BIT    CKS1           1      1      0     R|W     TRUE     -    
//     %%BIT    CKS0           0      0      0     R|W     TRUE     -    
//     
// %REG_NAME SCR      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    TIE            7      7      0     R|W     TRUE     -    
//     %%BIT    RIE            6      6      0     R|W     TRUE     W    
//     %%BIT    TE             5      5      0     R|W     TRUE     W    
//     %%BIT    RE             4      4      0     R|W     TRUE     W    
//     %%BIT    MPIE           3      3      0     R|W     TRUE     -    
//     %%BIT    TEIE           2      2      0     R|W     TRUE     W    
//     %%BIT    CKE1           1      1      0     R|W     TRUE     W    
//     %%BIT    CKE0           0      0      0     R|W     TRUE     W    
//     
// %REG_NAME SSR     
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    TDRE           7      7      1     R-W0    TRUE     R|W    
//     %%BIT    RDRF           6      6      0     R-W0    TRUE     -    
//     %%BIT    ORER           5      5      0     R-W0    TRUE     -    
//     %%BIT    FER            4      4      0     R-W0    TRUE     -    
//     %%BIT    PER            3      3      0     R-W0    TRUE     -    
//     %%BIT    TEND           2      2      1     R       TRUE     -    
//     %%BIT    MPB            1      1      0     R       TRUE     -    
//     %%BIT    MPBT           0      0      0     R|W     TRUE     -    
// 
// %REG_NAME BRR      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    BRR            7      0      255   R|W     TRUE     W    
//     
// %REG_NAME SCMR    
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    BCP2           7      7      1     R|W     FALSE    -      
//     %%BIT    RESERVED_BIT1  6      5      7     R       FALSE    -      
//     %%BIT    CHR1           4      4      1     R|W     TRUE     -    
//     %%BIT    SDIR           3      3      0     R|W     TRUE     -    
//     %%BIT    SINV           2      2      0     R|W     FALSE    -    
//     %%BIT    ASTLS          1      1      1     R|W     TRUE     -      
//     %%BIT    SMIF           0      0      0     R|W0    TRUE     W    
//     
// %REG_NAME SEMR    
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    BRME           7      7      0     R|W     TRUE     W      
//     %%BIT    MDDRS          6      6      0     R|W     TRUE     -    
//     %%BIT    BGDM           4      4      0     R|W     TRUE     -    
//     %%BIT    ABCS           3      3      0     R|W     TRUE     -    
//     %%BIT    RXDMON         2      2      1     R       TRUE     -    
////////////////////////////////////////////////////////////////////////////////
/// @file sci3_regif.h
/// @brief Register IF class of model SCI3
/// $Id$
/// $Date$
/// $Revison$
/// $Author$
////////////////////////////////////////////////////////////////////////////////
#ifndef __SCI3_REGIF_H__
#define __SCI3_REGIF_H__
#include <string>
#include <map>
#include <list>
#include <cstdarg>
#include <cerrno>
#include <iomanip>
#include <sstream>
#include <cassert>
#ifndef REGIF_NOT_USE_SYSTEMC
#include "systemc.h"
#endif
#include "re_register.h"
#ifdef CWR_SYSTEMC
#include "scml2.h"
#endif

/// Register IF class of SCI3 model
class Csci3_regif
: public vpcl::reg_super
{
protected:
    typedef const unsigned int cuint;
    typedef unsigned int uint;
#ifdef CWR_SYSTEMC
    typedef unsigned short REG_TYPE;
#endif

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
            this-> channel  = channel;
            this-> is_wr    = is_wr;
            this-> size     = size;
            this-> pre_data = pre_data;
            this-> data     = data;
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
#ifdef CWR_SYSTEMC
        scml2::memory<REG_TYPE> *mcwmem;
        scml2::reg<REG_TYPE> *my_cw_p;
        SRegList( vpcl::re_register *_my_p, SRegList *_prev, uint _channel, uint _length, std::string _wacc_size, std::string _racc_size, bool _block = false, scml2::memory<REG_TYPE> *_cwmem=NULL) {
            this->my_cw_p = NULL;
            if (_cwmem != NULL) {
                mcwmem = _cwmem;
            } else {
                assert(_prev != NULL && _prev->mcwmem != NULL) ;
                mcwmem = _prev->mcwmem;
            }
            this->my_cw_p = new scml2::reg<REG_TYPE> (_my_p->name(), *mcwmem , _my_p->addr());
#else
        SRegList( vpcl::re_register *_my_p, SRegList *_prev, uint _channel, uint _length, std::string _wacc_size, std::string _racc_size, bool _block = false) {
#endif
            this->my_p = NULL;
            if (_my_p != NULL) {
                this->my_p = _my_p;
            }
            this->prev = NULL;
            if (_prev != NULL) {
                this->prev = _prev;
            }
            this->channel   = _channel;
            this->length    = _length;
            this->wacc_size = _wacc_size;
            this->racc_size = _racc_size;
            this->block     = _block;
        }
        ~SRegList() {
#ifdef CWR_SYSTEMC
            delete my_cw_p;
#endif
        }
    } *mRegList, *mCurReg, **mRegArray;

    uint *mRegMap;
    uint mTotalRegNum;

public:
    Csci3_regif(std::string name, uint buswidth);
    ~Csci3_regif();

protected:
    vpcl::re_register *SMR ;
    vpcl::re_register *BRR ;
    vpcl::re_register *SCR ;
    vpcl::re_register *TDR ;
    vpcl::re_register *SSR ;
    vpcl::re_register *RDR ;
    vpcl::re_register *SCMR;
    vpcl::re_register *SEMR;

    #ifdef CWR_SYSTEMC
    scml2::memory<REG_TYPE> cwmem;
    #endif

    uint SMR_CM;
    uint SMR_CHR;
    uint SMR_PE;
    uint SMR_PM;
    uint SMR_STOP;
    uint SMR_MP;
    uint SMR_CKS1;
    uint SMR_CKS0;
    uint BRR_BRR;
    uint SCR_TIE;
    uint SCR_RIE;
    uint SCR_TE;
    uint SCR_RE;
    uint SCR_MPIE;
    uint SCR_TEIE;
    uint SCR_CKE1;
    uint SCR_CKE0;
    uint TDR_TDR;
    uint SSR_TDRE;
    uint SSR_RDRF;
    uint SSR_ORER;
    uint SSR_FER;
    uint SSR_PER;
    uint SSR_TEND;
    uint SSR_MPB;
    uint SSR_MPBT;
    uint RDR_RDR;
    uint SCMR_BCP2;
    uint SCMR_RESERVED_BIT1;
    uint SCMR_CHR1;
    uint SCMR_SDIR;
    uint SCMR_SINV;
    uint SCMR_ASTLS;
    uint SCMR_SMIF;
    uint SEMR_BRME;
    uint SEMR_MDDRS;
    uint SEMR_BGDM;
    uint SEMR_ABCS;
    uint SEMR_RXDMON;

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

    virtual void cb_SMR_CM(RegCBstr str) = 0;
    virtual void cb_BRR_BRR(RegCBstr str) = 0;
    virtual void cb_SCR_RIE(RegCBstr str) = 0;
    virtual void cb_SCR_TE(RegCBstr str) = 0;
    virtual void cb_SCR_RE(RegCBstr str) = 0;
    virtual void cb_SCR_TEIE(RegCBstr str) = 0;
    virtual void cb_SCR_CKE1(RegCBstr str) = 0;
    virtual void cb_SCR_CKE0(RegCBstr str) = 0;
    virtual void cb_TDR_TDR(RegCBstr str) = 0;
    virtual void cb_SSR_TDRE(RegCBstr str) = 0;
    virtual void cb_RDR_RDR(RegCBstr str) = 0;
    virtual void cb_SCMR_SMIF(RegCBstr str) = 0;
    virtual void cb_SEMR_BRME(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    bool mAPBAccessMode;
    bool mDumpBitInfo;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Csci3_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Csci3_regif::*) (RegCBstr)> > mRdCbAPI;
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
    bool ChkSize(std::string expect_size, cuint detect_size, cuint addr, bool is_wr);
    std::vector<std::string> Str2Vec(std::string str, const char sep);
    std::string Num2HexStr(cuint num, cuint num_length, bool space_strip, bool is_data, uint acc_size);
    void DumpRegMsg(const std::string operation, const std::string RegName, const std::string BitName, cuint size, cuint addr, cuint wr_data, cuint pre_data, cuint data, cuint reg_length);
    std::string AccessRegCommand(const std::vector<std::string>& args, vpcl::re_register *Register, bool &BlockReg);
    void InitLocalVal();
    void UpdateLocalVal(cuint addr);
    void UpdateRegVal(cuint addr);

    #ifdef CWR_SYSTEMC
    unsigned short cw_rd_cb(tlm::tlm_generic_payload& trans, int tag);
    unsigned short cw_wr_cb(tlm::tlm_generic_payload& trans, int tag);
    virtual void cw_set_callback_reg(scml2::reg<REG_TYPE> * reg, int offset);
    void cw_set_all_callback_reg(void);
    #endif
};
#endif //__SCI3_REG_DEF_H__
