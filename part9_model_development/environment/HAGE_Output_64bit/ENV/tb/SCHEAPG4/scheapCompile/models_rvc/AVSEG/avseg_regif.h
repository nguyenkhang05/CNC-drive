// -----------------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2010-2018 Renesas Electronics Corporation
// Copyright(c) 2010-2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------
// This file is generated by Register I/F generator
//    gen_regif.py
//    gen_regif_class.py
//    regif_h.skl
//
// Input file : avseg_info.txt
////////////////////////////////////////////////////////////////////////////////
// # -----------------------------------------------------------------------------
// # $Id$
// #
// # Copyright(c) 2010-2018 Renesas Electronics Corporation
// # Copyright(c) 2010-2018 Renesas Design Vietnam Co., Ltd.
// # RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// # This program must be used solely for the purpose for which
// # it was furnished by Renesas Electronics Corporation. No part of this
// # program may be reproduced or disclosed to others, in any
// # form, without the prior written permission of Renesas Electronics
// # Corporation.
// # -----------------------------------------------------------------------------
// %MODULE AVSEG
//     #                   name            offset_size
//     %%REG_INSTANCE      reg_def         7
// 
// %REG_CHANNEL reg_def
//     %%TITLE  group  name           reg_name       wsize       rsize       length  offset  factor_start    factor_end  factor_index    factor_step     access  init    support     callback
//     %%REG    -      VCCCHSCR       VCCCHSCR       32          8|16|32     32      0x00    -               -           -               -               W|R     0x0     TRUE        -
//     %%REG    -      VCCCNTCR       VCCCNTCR       32          8|16|32     32      0x04    -               -           -               -               W|R     0x101   TRUE        -
//     %%REG    -      EVCCCHSCR      EVCCCHSCR      32          8|16|32     32      0x10    -               -           -               -               W|R     0x0     TRUE        -
//     %%REG    -      EVCCCNTCR      EVCCCNTCR      32          8|16|32     32      0x14    -               -           -               -               W|R     0x101   TRUE        -
//     %%REG    -      AWOVDDCHSCR    AWOVDDCHSCR    32          8|16|32     32      0x20    -               -           -               -               W|R     0x0     TRUE        -
//     %%REG    -      AWOVDDCNTCR    AWOVDDCNTCR    32          8|16|32     32      0x24    -               -           -               -               W|R     0x101   TRUE        -
//     %%REG    -      ISOVDDCHSCR    ISOVDDCHSCR    32          8|16|32     32      0x30    -               -           -               -               W|R     0x0     TRUE        -
//     %%REG    -      ISOVDDCNTCR    ISOVDDCNTCR    32          8|16|32     32      0x34    -               -           -               -               W|R     0x101   TRUE        -
//     %%REG    -      SECVMONERR     SECVMONERR     32          8|16|32     32      0x40    -               -           -               -               R       0x0     TRUE        -
//     %%REG    -      SECVMONCLR     SECVMONCLR     32          8|16|32     32      0x44    -               -           -               -               D       0x0     TRUE        - 
// %REG_NAME VCCCHSCR
//     %%TITLE  name             upper  lower  init      access  support  callback
//     %%BIT    CHS               7      0     0         W|R     TRUE     W
// 
// %REG_NAME VCCCNTCR
//     %%TITLE  name             upper  lower  init      access  support  callback
//     %%BIT    ENB              28     28     0         W|R     TRUE     W
//     %%BIT    NRMCNT           11      8     1         W|R     TRUE     -
//     %%BIT    ERRCNT            3      0     1         W|R     TRUE     -
// 
// %REG_NAME EVCCCHSCR
//     %%TITLE  name             upper  lower  init      access  support  callback
//     %%BIT    CHS               7      0     0         W|R     TRUE     W
// 
// %REG_NAME EVCCCNTCR
//     %%TITLE  name             upper  lower  init      access  support  callback
//     %%BIT    ENB              28     28     0         W|R     TRUE     W
//     %%BIT    NRMCNT           11      8     1         W|R     TRUE     -
//     %%BIT    ERRCNT            3      0     1         W|R     TRUE     -
// 
// %REG_NAME AWOVDDCHSCR
//     %%TITLE  name             upper  lower  init      access  support  callback
//     %%BIT    CHS               7      0     0         W|R     TRUE     W
// 
// %REG_NAME AWOVDDCNTCR
//     %%TITLE  name             upper  lower  init      access  support  callback
//     %%BIT    ENB              28     28     0         W|R     TRUE     W
//     %%BIT    NRMCNT           11      8     1         W|R     TRUE     -
//     %%BIT    ERRCNT            3      0     1         W|R     TRUE     -
// 
// %REG_NAME ISOVDDCHSCR
//     %%TITLE  name             upper  lower  init      access  support  callback
//     %%BIT    CHS               7      0     0         W|R     TRUE     W
// 
// %REG_NAME ISOVDDCNTCR
//     %%TITLE  name             upper  lower  init      access  support  callback
//     %%BIT    ENB              28     28     0         W|R     TRUE     W
//     %%BIT    NRMCNT           11      8     1         W|R     TRUE     -
//     %%BIT    ERRCNT            3      0     1         W|R     TRUE     -
// 
// %REG_NAME SECVMONERR
//     %%TITLE  name             upper  lower  init      access  support  callback
//     %%BIT    AWOVDD_ULME      19     19     0         R       TRUE     -
//     %%BIT    ISOVDD_ULME      18     18     0         R       TRUE     -
//     %%BIT    EVCC_ULME        17     17     0         R       TRUE     -
//     %%BIT    VCC_ULME         16     16     0         R       TRUE     -
//     %%BIT    AWOVDD_LLME       3      3     0         R       TRUE     -
//     %%BIT    ISOVDD_LLME       2      2     0         R       TRUE     -
//     %%BIT    EVCC_LLME         1      1     0         R       TRUE     -
//     %%BIT    VCC_LLME          0      0     0         R       TRUE     -
// 
// %REG_NAME SECVMONCLR
//     %%TITLE  name             upper  lower  init      access  support  callback
//     %%BIT    AWOVDD_ULMEC     19     19     0         W       TRUE     -
//     %%BIT    ISOVDD_ULMEC     18     18     0         W       TRUE     -
//     %%BIT    EVCC_ULMEC       17     17     0         W       TRUE     -
//     %%BIT    VCC_ULMEC        16     16     0         W       TRUE     -
//     %%BIT    AWOVDD_LLMEC      3      3     0         W       TRUE     -
//     %%BIT    ISOVDD_LLMEC      2      2     0         W       TRUE     -
//     %%BIT    EVCC_LLMEC        1      1     0         W       TRUE     -
//     %%BIT    VCC_LLMEC         0      0     0         W       TRUE     W
// 
////////////////////////////////////////////////////////////////////////////////
/// @file avseg_regif.h
/// @brief Register IF class of model AVSEG
/// $Id$
/// $Date$
/// $Revison$
/// $Author$
////////////////////////////////////////////////////////////////////////////////
#ifndef __AVSEG_REGIF_H__
#define __AVSEG_REGIF_H__
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
#ifdef USE_CWR_SYSTEMC
#include "scml2.h"
#endif

/// Register IF class of AVSEG model
class Cavseg_regif
: public vpcl::reg_super
{
protected:
    typedef const unsigned int cuint;
    typedef unsigned int uint;
#ifdef USE_CWR_SYSTEMC
    typedef uint REG_TYPE;
#endif

    enum eRegGroup {
        emNum_of_gr
    }; ///< Enumeration for register group index

#ifdef USE_WEB_SIM
    enum eRegValIDConstant {
        emBitNum       = 0,
        emRegIDNum     = 0,
        emNumOfChannel = 1
    };
#endif

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
#ifdef USE_CWR_SYSTEMC
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
#ifdef USE_CWR_SYSTEMC
            delete my_cw_p;
#endif
        }
    } *mRegList, *mCurReg, **mRegArray;

    uint *mRegMap;
    uint mTotalRegNum;
#ifdef USE_WEB_SIM
    struct strRegValueID {
        int  BitVal[emBitNum];
        std::string RegValueID;
        strRegValueID(void) {
            for (unsigned int i = 0; i < emBitNum; i++) {
                BitVal[i] = 0;
            }
            RegValueID = "";
        }
        strRegValueID(int bitval[emBitNum], std::string reg_value_id) {
            for (unsigned int i = 0; i < emBitNum; i++) {
                this->BitVal[i] = bitval[i];
            }
            this->RegValueID = reg_value_id;
        }
    };
    
    vpcl::bit_info* mBitInfoPtr[emBitNum];
    uint mTargetRegVal[emBitNum];
    strRegValueID mRegValueIDLib[emRegIDNum];
    
    void InitializeRegValueID();
    void CalcTargetRegVal(void);
    void IssueRegValueID();

    typedef void (Cavseg_regif::* ptrRegValueIDFunc) (std::string);
    ptrRegValueIDFunc mNotifyRegValueIDAPI;
    virtual void NotifyRegValueID(const std::string reg_val_id) = 0;
#endif

public:
    Cavseg_regif(std::string name, uint buswidth);
    ~Cavseg_regif();

protected:
    vpcl::re_register *VCCCHSCR   ;
    vpcl::re_register *VCCCNTCR   ;
    vpcl::re_register *EVCCCHSCR  ;
    vpcl::re_register *EVCCCNTCR  ;
    vpcl::re_register *AWOVDDCHSCR;
    vpcl::re_register *AWOVDDCNTCR;
    vpcl::re_register *ISOVDDCHSCR;
    vpcl::re_register *ISOVDDCNTCR;
    vpcl::re_register *SECVMONERR ;
    vpcl::re_register *SECVMONCLR ;

    #ifdef USE_CWR_SYSTEMC
    scml2::memory<REG_TYPE> cwmem;
    #endif

    uint VCCCHSCR_CHS;
    uint VCCCNTCR_ENB;
    uint VCCCNTCR_NRMCNT;
    uint VCCCNTCR_ERRCNT;
    uint EVCCCHSCR_CHS;
    uint EVCCCNTCR_ENB;
    uint EVCCCNTCR_NRMCNT;
    uint EVCCCNTCR_ERRCNT;
    uint AWOVDDCHSCR_CHS;
    uint AWOVDDCNTCR_ENB;
    uint AWOVDDCNTCR_NRMCNT;
    uint AWOVDDCNTCR_ERRCNT;
    uint ISOVDDCHSCR_CHS;
    uint ISOVDDCNTCR_ENB;
    uint ISOVDDCNTCR_NRMCNT;
    uint ISOVDDCNTCR_ERRCNT;
    uint SECVMONERR_AWOVDD_ULME;
    uint SECVMONERR_ISOVDD_ULME;
    uint SECVMONERR_EVCC_ULME;
    uint SECVMONERR_VCC_ULME;
    uint SECVMONERR_AWOVDD_LLME;
    uint SECVMONERR_ISOVDD_LLME;
    uint SECVMONERR_EVCC_LLME;
    uint SECVMONERR_VCC_LLME;
    uint SECVMONCLR_AWOVDD_ULMEC;
    uint SECVMONCLR_ISOVDD_ULMEC;
    uint SECVMONCLR_EVCC_ULMEC;
    uint SECVMONCLR_VCC_ULMEC;
    uint SECVMONCLR_AWOVDD_LLMEC;
    uint SECVMONCLR_ISOVDD_LLMEC;
    uint SECVMONCLR_EVCC_LLMEC;
    uint SECVMONCLR_VCC_LLMEC;

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

    virtual void cb_VCCCHSCR_CHS(RegCBstr str) = 0;
    virtual void cb_VCCCNTCR_ENB(RegCBstr str) = 0;
    virtual void cb_EVCCCHSCR_CHS(RegCBstr str) = 0;
    virtual void cb_EVCCCNTCR_ENB(RegCBstr str) = 0;
    virtual void cb_AWOVDDCHSCR_CHS(RegCBstr str) = 0;
    virtual void cb_AWOVDDCNTCR_ENB(RegCBstr str) = 0;
    virtual void cb_ISOVDDCHSCR_CHS(RegCBstr str) = 0;
    virtual void cb_ISOVDDCNTCR_ENB(RegCBstr str) = 0;
    virtual void cb_SECVMONCLR_VCC_LLMEC(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    bool mAPBAccessMode;
    bool mDumpBitInfo;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Cavseg_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Cavseg_regif::*) (RegCBstr)> > mRdCbAPI;
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

    #ifdef USE_CWR_SYSTEMC
    uint cw_rd_cb(tlm::tlm_generic_payload& trans, int tag);
    uint cw_wr_cb(tlm::tlm_generic_payload& trans, int tag);
    virtual void cw_set_callback_reg(scml2::reg<REG_TYPE> * reg, int offset);
    void cw_set_all_callback_reg(void);
    #endif
};
#endif //__AVSEG_REG_DEF_H__
