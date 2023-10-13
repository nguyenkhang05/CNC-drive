// -----------------------------------------------------------------------------
// $Id: copyright.txt 5316 2020-08-17 04:15:48Z chuonghoangle $
//
// Copyright(c) 2021 Renesas Electronics Corporation
// Copyright(c) 2021 Renesas Design Vietnam Co., Ltd.
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
// Input file : dummyperipheralrvc_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE DummyPeripheralRVC
//     #              name     offset_size
//     %%REG_INSTANCE reg_def  9
// 
// %REG_CHANNEL reg_def                                                       
//     %%TITLE    name                           reg_name                     wsize   rsize        length  offset  factor_start    factor_end  factor_index    factor_step access   init   support  callback
//     %%REG      JUDGE_REG                      JUDGE_REG                    32      8|16|32      32      0x00    -               -           -               -           R|W      0x0     TRUE     -
//     %%REG      RESET_REG                      RESET_REG                    32      8|16|32      32      0x04    -               -           -               -           R|W      0x1     TRUE     -
//     %%REG      PCLK_LOW_REG                   PCLK_LOW_REG                 32      8|16|32      32      0x08    -               -           -               -           R|W      0x0     TRUE     -
//     %%REG      PCLK_HIGH_REG                  PCLK_HIGH_REG                32      8|16|32      32      0x0C    -               -           -               -           R|W      0x0     TRUE     -
//     %%REG      BMSPIDOUT_REG                  BMSPIDOUT_REG                32      8|16|32      32      0x10    0               63          -               -           R|W      0x0     TRUE     -
//     
// %REG_NAME JUDGE_REG                                                            
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    JUDGE               0       0       0      R|W     TRUE        W
// 
// %REG_NAME RESET_REG
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    RESETPORT           0       0       1      R|W     TRUE        W
// 
// %REG_NAME PCLK_LOW_REG
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    CLK_LOW            31       0       0      R|W     TRUE        W
// 
// %REG_NAME PCLK_HIGH_REG
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    CLK_HIGH           31       0       0      R|W     TRUE        -
// 
// %REG_NAME BMSPIDOUT_REG
//     %%TITLE  name               upper   lower   init    access  support     callback
//     %%BIT    BMSPIDOUT          4        0       0      R       TRUE        -
// 	////////////////////////////////////////////////////////////////////////////////
/// @file dummyperipheralrvc_regif.h
/// @brief Register IF class of model DUMMYPERIPHERALRVC
/// $Id$
/// $Date$
/// $Revison$
/// $Author$
////////////////////////////////////////////////////////////////////////////////
#ifndef __DUMMYPERIPHERALRVC_REGIF_H__
#define __DUMMYPERIPHERALRVC_REGIF_H__
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
#ifdef USR_CWR_SYSTEMC
#include "scml2.h"
#endif

/// Register IF class of DUMMYPERIPHERALRVC model
class Cdummyperipheralrvc_regif
: public vpcl::reg_super
{
protected:
    typedef const unsigned int cuint;
    typedef unsigned int uint;
#ifdef USR_CWR_SYSTEMC
    typedef uint REG_TYPE;
#endif

    enum eRegGroup {
        emNum_of_gr
    }; ///< Enumeration for register group index
    enum eRegIndex {
        emNUM_BMSPIDOUT_REG = 64
    }; ///< Enumeration for register factor index
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
#ifdef USR_CWR_SYSTEMC
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
#ifdef USR_CWR_SYSTEMC
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

    typedef void (Cdummyperipheralrvc_regif::* ptrRegValueIDFunc) (std::string);
    ptrRegValueIDFunc mNotifyRegValueIDAPI;
    virtual void NotifyRegValueID(const std::string reg_val_id) = 0;
#endif

public:
    Cdummyperipheralrvc_regif(std::string name, uint buswidth);
    ~Cdummyperipheralrvc_regif();

protected:
    vpcl::re_register *JUDGE_REG    ;
    vpcl::re_register *RESET_REG    ;
    vpcl::re_register *PCLK_LOW_REG ;
    vpcl::re_register *PCLK_HIGH_REG;
    vpcl::re_register *BMSPIDOUT_REG[64];

    #ifdef USR_CWR_SYSTEMC
    scml2::memory<REG_TYPE> cwmem;
    #endif


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

    virtual void cb_JUDGE_REG_JUDGE(RegCBstr str) = 0;
    virtual void cb_RESET_REG_RESETPORT(RegCBstr str) = 0;
    virtual void cb_PCLK_LOW_REG_CLK_LOW(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    bool mAPBAccessMode;
    bool mDumpBitInfo;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Cdummyperipheralrvc_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Cdummyperipheralrvc_regif::*) (RegCBstr)> > mRdCbAPI;
    std::map<std::string, std::map<std::string, void (Cdummyperipheralrvc_regif::*) (RegCBstr)> > mWrDbgCbAPI;
    std::map<std::string, std::map<std::string, void (Cdummyperipheralrvc_regif::*) (RegCBstr)> > mRdDbgCbAPI;
    std::string mFileName;
    std::string mInstName;
    int mLineNum;

    bool reg_wr_process(cuint addr, const unsigned char *p_data, cuint size, bool IsDbgFunc);
    bool reg_rd_process(cuint addr, unsigned char *p_data, cuint size, bool IsDbgFunc);
    bool reg_wr_func(cuint addr, const unsigned char *p_data, cuint size, cuint reg_index, bool IsDbgFunc);
    bool reg_rd_func(cuint addr, unsigned char *p_data, cuint size, cuint reg_index, bool IsDbgFunc);
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

    #ifdef USR_CWR_SYSTEMC
    uint cw_rd_cb(tlm::tlm_generic_payload& trans, int tag);
    uint cw_wr_cb(tlm::tlm_generic_payload& trans, int tag);
    virtual void cw_set_callback_reg(scml2::reg<REG_TYPE> * reg, int offset);
    void cw_set_all_callback_reg(void);
    #endif

private:
    void _re_printf(const std::string msg_level, const char *format, ...);
};
#endif //__DUMMYPERIPHERALRVC_REGIF_H__
