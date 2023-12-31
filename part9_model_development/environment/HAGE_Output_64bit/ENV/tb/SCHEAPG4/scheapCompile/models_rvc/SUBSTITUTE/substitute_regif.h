/// This file is generated by Register I/F generator (ver3.0)
////////////////////////////////////////////////////////////////////////////////////////////////////
/// Input file : substitute_info.txt
/// %MODULE substitute
///     #              name     offset_size  
///     %%REG_INSTANCE reg_def  15           
/// 
/// %REG_CHANNEL   reg_def
///      %%TITLE    name        reg_name     size    length   offset   factor_start   factor_end    factor_index    factor_step  init        access  support  callback
///      %%REG      CTRL         CTRL         32      32       0x7FF0      -             -               -              -          0          W|R      TRUE      -   
///      %%REG      PARAM1       PARAM1       32      32       0x7FF4      -             -               -              -          0          W|R      TRUE      - 
///      %%REG      PARAM2       PARAM2       16      16       0x7FF8      -             -               -              -          0          W|R      TRUE      -
///      %%REG      JUDGE        JUDGE        16      16       0x7FFC      -             -               -              -          0          W|R      TRUE      W    
///      %%REG      END          END          16      16       0x7FFF      -             -               -              -          0          W|R      TRUE      W    
/// 
/// 
/// 
/// %REG_NAME      CTRL 
///    %%TITLE     name       upper   lower     init    access      support     callback    
///    %%BIT       CTRL0       0        0        0        W|R        TRUE           W
///    %%BIT       CTRL1        1       1        0        W|R        TRUE           -
///    %%BIT       CTRL2        2       2        0        W|R        TRUE           -
///    %%BIT       CTRL3        3       3        0        W|R        TRUE           -
///    %%BIT       CTRL4        4       4        0        W|R        TRUE           -
///    %%BIT       CTRL5        5       5        0        W|R        TRUE           -
///    %%BIT       CTRL6        6       6        0        W|R        TRUE           -
///    %%BIT       CTRL7        7       7        0        W|R        TRUE           -
///    %%BIT       CTRL8        8       8        0        W|R        TRUE           -
///    %%BIT       CTRL9        9       9        0        W|R        TRUE           -
///    %%BIT       CTRL10       10      10       0        W|R        TRUE           -
///    %%BIT       CTRL11       11      11       0        W|R        TRUE           -
///    %%BIT       CTRL12       12      12       0        W|R        TRUE           -
///    %%BIT       CTRL13       13      13       0        W|R        TRUE           -
///    %%BIT       CTRL14       14      14       0        W|R        TRUE           -
///    %%BIT       CTRL15       15      15       0        W|R        TRUE           -
///    %%BIT       CTRL16       16      16       0        W|R        TRUE           -
///    %%BIT       CTRL17       17      17       0        W|R        TRUE           -
///    %%BIT       CTRL18       18      18       0        W|R        TRUE           -
///    %%BIT       CTRL19       19      19       0        W|R        TRUE           -
///    %%BIT       CTRL20       20      20       0        W|R        TRUE           -
///    %%BIT       CTRL21       21      21       0        W|R        TRUE           -
///    %%BIT       CTRL22       22      22       0        W|R        TRUE           -
///    %%BIT       CTRL23       23      23       0        W|R        TRUE           -
///    %%BIT       CTRL24       24      24       0        W|R        TRUE           -
///    %%BIT       CTRL25       25      25       0        W|R        TRUE           -
///    %%BIT       CTRL26       26      26       0        W|R        TRUE           -
///    %%BIT       CTRL27       27      27       0        W|R        TRUE           -
///    %%BIT       CTRL28       28      28       0        W|R        TRUE           -
///    %%BIT       CTRL29       29      29       0        W|R        TRUE           -
///    %%BIT       CTRL30       30      30       0        W|R        TRUE           -
///    %%BIT       CTRL31       31      31       0        W|R        TRUE           -
/// 
///  
/// %CMD_BASEID  tgt reg
/// %CMD_NODUMP_API CommandCB AssertReset DumpProfile ClearProfile
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file substitute_regif.h
/// @brief Header file of Register IF for SUBSTITUTE model
/// @Id $Id: substitute_regif.h,v 1.1.1.1 2012/10/15 02:47:27 chaunguyen Exp $
/// @date $Date: 2012/10/15 02:47:27 $  $Revision: 1.1.1.1 $
/// @author $Author: chaunguyen $

#ifndef __SUBSTITUTE_REGIF_H__
#define __SUBSTITUTE_REGIF_H__
#include <string>
#include <map>
#include <list>
#include <cstdarg>
#include <cerrno>
#include <iomanip>
#include <sstream>
#include "systemc.h"
#include "re_register.h"
#ifdef CWR_SYSTEMC
#include "scml.h"
#endif

/// Register IF class of SUBSTITUTE model
class Csubstitute_regif
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
        std::string acc_size;
#ifdef CWR_SYSTEMC
        scml_memory<unsigned int> *my_cw_p;
        SRegList( vpcl::re_register *_my_p, SRegList *_prev, uint _channel, uint _length, std::string _acc_size, scml_memory<unsigned int> *_cwmem) {
            this->my_cw_p = NULL;
            if (_cwmem != NULL) {
                this->my_cw_p = new scml_memory<unsigned int> (_my_p->name(), *_cwmem , _my_p->addr(), 1);
            }
#else
        SRegList( vpcl::re_register *_my_p, SRegList *_prev, uint _channel, uint _length, std::string _acc_size) {
#endif
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
            this->acc_size = _acc_size;
        }
    } *mRegList, *mCurReg;

public:
    vpcl::re_register *CTRL  ;
    vpcl::re_register *PARAM1;
    vpcl::re_register *PARAM2;
    vpcl::re_register *JUDGE ;
    vpcl::re_register *END   ;

    Csubstitute_regif(std::string name, unsigned int buswidth);
    ~Csubstitute_regif();

protected:
    void EnableReset(const bool is_active);
    bool reg_wr(cuint addr, const unsigned char *p_data, cuint size);
    bool reg_rd(cuint addr, unsigned char *p_data, cuint size);
    bool reg_wr_dbg(cuint addr, const unsigned char *p_data, cuint size);
    bool reg_rd_dbg(cuint addr, unsigned char *p_data, cuint size);
    std::string reg_handle_command (const std::vector<std::string>& args);
    vpcl::re_register *first_reg_object();
    vpcl::re_register *next_reg_object();

    void wr_cb(cuint addr, uint data);
    unsigned int rd_cb(cuint addr);

#ifdef CWR_SYSTEMC
    scml_memory<unsigned int> cwmem;
#endif

    virtual void cb_CTRL_CTRL0(RegCBstr str) = 0;
    virtual void cb_JUDGE_JUDGE(RegCBstr str) = 0;
    virtual void cb_END_END(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

private:
    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool reg_wr_process(cuint addr, const unsigned char *p_data, cuint size, bool IsDbgFunc);
    bool reg_rd_process(cuint addr, unsigned char *p_data, cuint size, bool IsDbgFunc);
    bool reg_wr_func(cuint addr, const unsigned char *p_data, cuint size, vpcl::re_register *Register, bool IsDbgFunc);
    bool reg_rd_func(cuint addr, unsigned char *p_data, cuint size, vpcl::re_register *Register, bool IsDbgFunc);
    void _re_printf(const std::string msg_level, const char *format, ...);
    void get_fileline(std::string filename, int line_number);
    bool mDumpRegisterRW;
    bool mDumpFileNameLineNum;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, bool> mBlock;
    std::map<std::string, std::string> mAccSize;
    std::map<std::string, uint> mLength;
    std::map<std::string, std::map<std::string, void (Csubstitute_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Csubstitute_regif::*) (RegCBstr)> > mRdCbAPI;
    std::map<std::string, uint> mChannel;
    std::string mFileName;
    std::string mInstName;
    int mLineNum;
    void CommandInit();
    bool ChkSize(std::string expect_size, cuint detect_size, cuint addr);
    std::vector<std::string> Str2Vec(std::string str, const char sep);
    void DumpRegMsg(const std::string operation, const std::string RegName, const std::string BitName, cuint size, cuint addr, cuint pre_data, cuint data);
    std::string AccessRegCommand (const std::vector<std::string>& args, vpcl::re_register *Register, bool &BlockReg);

};
#endif //__SUBSTITUTE_REG_DEF_H__
