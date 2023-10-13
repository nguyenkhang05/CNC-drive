// -----------------------------------------------------------------------------
// $Id: copyright.txt,v 1.3 2014/10/13 03:21:49 sontran Exp $
//
// Copyright(c) 2016 - 2017 Renesas Electronics Corporation
// Copyright(c) 2016 - 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------
// This file is generated by Register I/F generator
//    gen_regif.py 1.9 2013/12/04 09:51:36 sontran
//    gen_regif_class.py 1.62 2014/11/28 10:17:23 ducduong
//    regif_h.skl 1.32 2014/11/28 10:17:23 ducduong
//
// Input file : air_regif.txt
////////////////////////////////////////////////////////////////////////////////
// ## ----------------------------------------------------------------------
// ## $Id: $
// ##
// ## Copyright(c) 2016 - 2017 Renesas System Design Co., Ltd.
// ## Copyright(c) 2016 - 2017 Renesas Design Vietnam Co., Ltd.
// ## RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// ## This program must be used solely for the purpose for which
// ## it was furnished by Renesas Electronics Corporation. No part of this
// ## program may be reproduced or disclosed to others, in any
// ## form, without the prior written permission of Renesas Electronics
// ## Corporation.
// ## ----------------------------------------------------------------------
// 
// %MODULE air_core 
//     #              name     offset_size
//     %%REG_INSTANCE reg_def       4     
// 
// %REG_CHANNEL reg_def
//     %%TITLE           name          size        length  offset     access   init           support   callback
//     %%REG             AIR_ISELR0     8|16|32    32      0x00       W|R      0x00000000     TRUE      -
//     %%REG             AIR_ISELR1     8|16|32    32      0x04       W|R      0xFF800000     TRUE      -
//     %%REG             AIR_DSELR0     8|16|32    32      0x08       W|R      0x0F000000     TRUE      -
//     %%REG             AIR_DSELR1     8|16|32    32      0x0C       W|R      0xFF800000     TRUE      -
// 
// %REG_NAME AIR_ISELR0      
//     %%TITLE    name       upper  lower  access  init      support  callback    
//     %%BIT     ISEL31        31    31    R|W     0x0       TRUE      -
//     %%BIT     ISEL30        30    30    R|W     0x0       TRUE      -
//     %%BIT     ISEL29        29    29    R|W     0x0       TRUE      -
//     %%BIT     ISEL28        28    28    R|W     0x0       TRUE      -
//     %%BIT     ISEL27        27    27    R|W     0x0       TRUE      -
//     %%BIT     ISEL26        26    26    R|W     0x0       TRUE      -
//     %%BIT     ISEL25        25    25    R|W     0x0       TRUE      -
//     %%BIT     ISEL24        24    24    R|W     0x0       TRUE      -
//     %%BIT     ISEL23        23    23    R|W     0x0       TRUE      -
//     %%BIT     ISEL22        22    22    R|W     0x0       TRUE      -
//     %%BIT     ISEL21        21    21    R|W     0x0       TRUE      -
//     %%BIT     ISEL20        20    20    R|W     0x0       TRUE      -
//     %%BIT     ISEL19        19    19    R|W     0x0       TRUE      -
//     %%BIT     ISEL18        18    18    R|W     0x0       TRUE      -
//     %%BIT     ISEL17        17    17    R|W     0x0       TRUE      -
//     %%BIT     ISEL16        16    16    R|W     0x0       TRUE      -
//     %%BIT     ISEL15        15    15    R|W     0x0       TRUE      -
//     %%BIT     ISEL14        14    14    R|W     0x0       TRUE      -
//     %%BIT     ISEL13        13    13    R|W     0x0       TRUE      -
//     %%BIT     ISEL12        12    12    R|W     0x0       TRUE      -
//     %%BIT     ISEL11        11    11    R|W     0x0       TRUE      -
//     %%BIT     ISEL10        10    10    R|W     0x0       TRUE      -
//     %%BIT     ISEL9          9     9   R|W      0x0       TRUE      -
//     %%BIT     ISEL8          8     8   R|W      0x0       TRUE      -
//     %%BIT     ISEL7          7     7   R|W      0x0       TRUE      -
//     %%BIT     ISEL6          6     6   R|W      0x0       TRUE      -
//     %%BIT     ISEL5          5     5   R|W      0x0       TRUE      -
//     %%BIT     ISEL4          4     4   R|W      0x0       TRUE      -
//     %%BIT     ISEL3          3     3   R|W      0x0       TRUE      -
//     %%BIT     ISEL2          2     2   R|W      0x0       TRUE      -
//     %%BIT     ISEL1          1     1   R|W      0x0       TRUE      -
//     %%BIT     ISEL0          0     0   R|W      0x0       TRUE      -
// 
// 
// %REG_NAME AIR_ISELR1      
//     %%TITLE  name       upper  lower  access  init      support  callback    
//     %%BIT     ISEL63    31      31    R|W     0x1       TRUE  -
//     %%BIT     ISEL62    30      30    R|W     0x1       TRUE  -
//     %%BIT     ISEL61    29      29    R|W     0x1       TRUE  -
//     %%BIT     ISEL60    28      28    R|W     0x1       TRUE  -
//     %%BIT     ISEL59    27      27    R|W     0x1       TRUE  -
//     %%BIT     ISEL58    26      26    R|W     0x1       TRUE  -
//     %%BIT     ISEL57    25      25    R|W     0x1       TRUE  -
//     %%BIT     ISEL56    24      24    R|W     0x1       TRUE  -
//     %%BIT     ISEL55    23      23    R|W     0x1       TRUE  -
//     %%BIT     ISEL54    22      22    R|W     0x0       TRUE  -
//     %%BIT     ISEL53    21      21    R|W     0x0       TRUE  -
//     %%BIT     ISEL52    20      20    R|W     0x0       TRUE  -
//     %%BIT     ISEL51    19      19    R|W     0x0       TRUE  -
//     %%BIT     ISEL50    18      18    R|W     0x0       TRUE  -
//     %%BIT     ISEL49    17      17    R|W     0x0       TRUE  -
//     %%BIT     ISEL48    16      16    R|W     0x0       TRUE  -
//     %%BIT     ISEL47    15      15    R|W     0x0       TRUE  -
//     %%BIT     ISEL46    14      14    R|W     0x0       TRUE  -
//     %%BIT     ISEL45    13      13    R|W     0x0       TRUE  -
//     %%BIT     ISEL44    12      12    R|W     0x0       TRUE  -
//     %%BIT     ISEL43    11      11    R|W     0x0       TRUE  -
//     %%BIT     ISEL42    10      10    R|W     0x0       TRUE  -
//     %%BIT     ISEL41     9       9    R|W     0x0       TRUE  -
//     %%BIT     ISEL40     8       8    R|W     0x0       TRUE  -
//     %%BIT     ISEL39     7       7    R|W     0x0       TRUE  -
//     %%BIT     ISEL38     6       6    R|W     0x0       TRUE  -
//     %%BIT     ISEL37     5       5    R|W     0x0       TRUE  -
//     %%BIT     ISEL36     4       4    R|W     0x0       TRUE  -
//     %%BIT     ISEL35     3       3    R|W     0x0       TRUE  -
//     %%BIT     ISEL34     2       2    R|W     0x0       TRUE  -
//     %%BIT     ISEL33     1       1    R|W     0x0       TRUE  -
//     %%BIT     ISEL32     0       0    R|W     0x0       TRUE  -
// 
// 
// 
// %REG_NAME AIR_DSELR0      
//     %%TITLE  name       upper  lower  access  init         support  callback    
//     %%BIT     DSEL31        31    31    R|W     0x0       TRUE      -
//     %%BIT     DSEL30        30    30    R|W     0x0       TRUE      -
//     %%BIT     DSEL29        29    29    R|W     0x0       TRUE      -
//     %%BIT     DSEL28        28    28    R|W     0x0       TRUE      -
//     %%BIT     DSEL27        27    27    R|W     0x1       TRUE      -
//     %%BIT     DSEL26        26    26    R|W     0x1       TRUE      -
//     %%BIT     DSEL25        25    25    R|W     0x1       TRUE      -
//     %%BIT     DSEL24        24    24    R|W     0x1       TRUE      -
//     %%BIT     DSEL23        23    23    R|W     0x0       TRUE      -
//     %%BIT     DSEL22        22    22    R|W     0x0       TRUE      -
//     %%BIT     DSEL21        21    21    R|W     0x0       TRUE      -
//     %%BIT     DSEL20        20    20    R|W     0x0       TRUE      -
//     %%BIT     DSEL19        19    19    R|W     0x0       TRUE      -
//     %%BIT     DSEL18        18    18    R|W     0x0       TRUE      -
//     %%BIT     DSEL17        17    17    R|W     0x0       TRUE      -
//     %%BIT     DSEL16        16    16    R|W     0x0       TRUE      -
//     %%BIT     DSEL15        15    15    R|W     0x0       TRUE      -
//     %%BIT     DSEL14        14    14    R|W     0x0       TRUE      -
//     %%BIT     DSEL13        13    13    R|W     0x0       TRUE      -
//     %%BIT     DSEL12        12    12    R|W     0x0       TRUE      -
//     %%BIT     DSEL11        11    11    R|W     0x0       TRUE      -
//     %%BIT     DSEL10        10    10    R|W     0x0       TRUE      -
//     %%BIT     DSEL9          9     9   R|W      0x0       TRUE      -
//     %%BIT     DSEL8          8     8   R|W      0x0       TRUE      -
//     %%BIT     DSEL7          7     7   R|W      0x0       TRUE      -
//     %%BIT     DSEL6          6     6   R|W      0x0       TRUE      -
//     %%BIT     DSEL5          5     5   R|W      0x0       TRUE      -
//     %%BIT     DSEL4          4     4   R|W      0x0       TRUE      -
//     %%BIT     DSEL3          3     3   R|W      0x0       TRUE      -
//     %%BIT     DSEL2          2     2   R|W      0x0       TRUE      -
//     %%BIT     DSEL1          1     1   R|W      0x0       TRUE      -
//     %%BIT     DSEL0          0     0   R|W      0x0       TRUE      -
// 
// %REG_NAME AIR_DSELR1      
//     %%TITLE  name       upper  lower  access  init         support  callback    
//     %%BIT     DSEL63    31      31    R|W     0x1       TRUE  -
//     %%BIT     DSEL62    30      30    R|W     0x1       TRUE  -
//     %%BIT     DSEL61    29      29    R|W     0x1       TRUE  -
//     %%BIT     DSEL60    28      28    R|W     0x1       TRUE  -
//     %%BIT     DSEL59    27      27    R|W     0x1       TRUE  -
//     %%BIT     DSEL58    26      26    R|W     0x1       TRUE  -
//     %%BIT     DSEL57    25      25    R|W     0x1       TRUE  -
//     %%BIT     DSEL56    24      24    R|W     0x1       TRUE  -
//     %%BIT     DSEL55    23      23    R|W     0x1       TRUE  -
//     %%BIT     DSEL54    22      22    R|W     0x0       TRUE  -
//     %%BIT     DSEL53    21      21    R|W     0x0       TRUE  -
//     %%BIT     DSEL52    20      20    R|W     0x0       TRUE  -
//     %%BIT     DSEL51    19      19    R|W     0x0       TRUE  -
//     %%BIT     DSEL50    18      18    R|W     0x0       TRUE  -
//     %%BIT     DSEL49    17      17    R|W     0x0       TRUE  -
//     %%BIT     DSEL48    16      16    R|W     0x0       TRUE  -
//     %%BIT     DSEL47    15      15    R|W     0x0       TRUE  -
//     %%BIT     DSEL46    14      14    R|W     0x0       TRUE  -
//     %%BIT     DSEL45    13      13    R|W     0x0       TRUE  -
//     %%BIT     DSEL44    12      12    R|W     0x0       TRUE  -
//     %%BIT     DSEL43    11      11    R|W     0x0       TRUE  -
//     %%BIT     DSEL42    10      10    R|W     0x0       TRUE  -
//     %%BIT     DSEL41     9       9    R|W     0x0       TRUE  -
//     %%BIT     DSEL40     8       8    R|W     0x0       TRUE  -
//     %%BIT     DSEL39     7       7    R|W     0x0       TRUE  -
//     %%BIT     DSEL38     6       6    R|W     0x0       TRUE  -
//     %%BIT     DSEL37     5       5    R|W     0x0       TRUE  -
//     %%BIT     DSEL36     4       4    R|W     0x0       TRUE  -
//     %%BIT     DSEL35     3       3    R|W     0x0       TRUE  -
//     %%BIT     DSEL34     2       2    R|W     0x0       TRUE  -
//     %%BIT     DSEL33     1       1    R|W     0x0       TRUE  -
//     %%BIT     DSEL32     0       0    R|W     0x0       TRUE  -
// 
////////////////////////////////////////////////////////////////////////////////
/// @file air_core_regif.h
/// @brief Register IF class of model AIR_CORE
/// $Id$
/// $Date$
/// $Revison$
/// $Author$
////////////////////////////////////////////////////////////////////////////////
#ifndef __AIR_CORE_REGIF_H__
#define __AIR_CORE_REGIF_H__
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

/// Register IF class of AIR_CORE model
class Cair_core_regif
: public vpcl::reg_super
{
protected:
    typedef const unsigned int cuint;
    typedef unsigned int uint;
#ifdef CWR_SYSTEMC
    typedef uint REG_TYPE;
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
    Cair_core_regif(std::string name, uint buswidth);
    ~Cair_core_regif();

protected:
    vpcl::re_register *AIR_ISELR0;
    vpcl::re_register *AIR_ISELR1;
    vpcl::re_register *AIR_DSELR0;
    vpcl::re_register *AIR_DSELR1;

    #ifdef CWR_SYSTEMC
    scml2::memory<REG_TYPE> cwmem;
    #endif

    uint AIR_ISELR0_ISEL31;
    uint AIR_ISELR0_ISEL30;
    uint AIR_ISELR0_ISEL29;
    uint AIR_ISELR0_ISEL28;
    uint AIR_ISELR0_ISEL27;
    uint AIR_ISELR0_ISEL26;
    uint AIR_ISELR0_ISEL25;
    uint AIR_ISELR0_ISEL24;
    uint AIR_ISELR0_ISEL23;
    uint AIR_ISELR0_ISEL22;
    uint AIR_ISELR0_ISEL21;
    uint AIR_ISELR0_ISEL20;
    uint AIR_ISELR0_ISEL19;
    uint AIR_ISELR0_ISEL18;
    uint AIR_ISELR0_ISEL17;
    uint AIR_ISELR0_ISEL16;
    uint AIR_ISELR0_ISEL15;
    uint AIR_ISELR0_ISEL14;
    uint AIR_ISELR0_ISEL13;
    uint AIR_ISELR0_ISEL12;
    uint AIR_ISELR0_ISEL11;
    uint AIR_ISELR0_ISEL10;
    uint AIR_ISELR0_ISEL9;
    uint AIR_ISELR0_ISEL8;
    uint AIR_ISELR0_ISEL7;
    uint AIR_ISELR0_ISEL6;
    uint AIR_ISELR0_ISEL5;
    uint AIR_ISELR0_ISEL4;
    uint AIR_ISELR0_ISEL3;
    uint AIR_ISELR0_ISEL2;
    uint AIR_ISELR0_ISEL1;
    uint AIR_ISELR0_ISEL0;
    uint AIR_ISELR1_ISEL63;
    uint AIR_ISELR1_ISEL62;
    uint AIR_ISELR1_ISEL61;
    uint AIR_ISELR1_ISEL60;
    uint AIR_ISELR1_ISEL59;
    uint AIR_ISELR1_ISEL58;
    uint AIR_ISELR1_ISEL57;
    uint AIR_ISELR1_ISEL56;
    uint AIR_ISELR1_ISEL55;
    uint AIR_ISELR1_ISEL54;
    uint AIR_ISELR1_ISEL53;
    uint AIR_ISELR1_ISEL52;
    uint AIR_ISELR1_ISEL51;
    uint AIR_ISELR1_ISEL50;
    uint AIR_ISELR1_ISEL49;
    uint AIR_ISELR1_ISEL48;
    uint AIR_ISELR1_ISEL47;
    uint AIR_ISELR1_ISEL46;
    uint AIR_ISELR1_ISEL45;
    uint AIR_ISELR1_ISEL44;
    uint AIR_ISELR1_ISEL43;
    uint AIR_ISELR1_ISEL42;
    uint AIR_ISELR1_ISEL41;
    uint AIR_ISELR1_ISEL40;
    uint AIR_ISELR1_ISEL39;
    uint AIR_ISELR1_ISEL38;
    uint AIR_ISELR1_ISEL37;
    uint AIR_ISELR1_ISEL36;
    uint AIR_ISELR1_ISEL35;
    uint AIR_ISELR1_ISEL34;
    uint AIR_ISELR1_ISEL33;
    uint AIR_ISELR1_ISEL32;
    uint AIR_DSELR0_DSEL31;
    uint AIR_DSELR0_DSEL30;
    uint AIR_DSELR0_DSEL29;
    uint AIR_DSELR0_DSEL28;
    uint AIR_DSELR0_DSEL27;
    uint AIR_DSELR0_DSEL26;
    uint AIR_DSELR0_DSEL25;
    uint AIR_DSELR0_DSEL24;
    uint AIR_DSELR0_DSEL23;
    uint AIR_DSELR0_DSEL22;
    uint AIR_DSELR0_DSEL21;
    uint AIR_DSELR0_DSEL20;
    uint AIR_DSELR0_DSEL19;
    uint AIR_DSELR0_DSEL18;
    uint AIR_DSELR0_DSEL17;
    uint AIR_DSELR0_DSEL16;
    uint AIR_DSELR0_DSEL15;
    uint AIR_DSELR0_DSEL14;
    uint AIR_DSELR0_DSEL13;
    uint AIR_DSELR0_DSEL12;
    uint AIR_DSELR0_DSEL11;
    uint AIR_DSELR0_DSEL10;
    uint AIR_DSELR0_DSEL9;
    uint AIR_DSELR0_DSEL8;
    uint AIR_DSELR0_DSEL7;
    uint AIR_DSELR0_DSEL6;
    uint AIR_DSELR0_DSEL5;
    uint AIR_DSELR0_DSEL4;
    uint AIR_DSELR0_DSEL3;
    uint AIR_DSELR0_DSEL2;
    uint AIR_DSELR0_DSEL1;
    uint AIR_DSELR0_DSEL0;
    uint AIR_DSELR1_DSEL63;
    uint AIR_DSELR1_DSEL62;
    uint AIR_DSELR1_DSEL61;
    uint AIR_DSELR1_DSEL60;
    uint AIR_DSELR1_DSEL59;
    uint AIR_DSELR1_DSEL58;
    uint AIR_DSELR1_DSEL57;
    uint AIR_DSELR1_DSEL56;
    uint AIR_DSELR1_DSEL55;
    uint AIR_DSELR1_DSEL54;
    uint AIR_DSELR1_DSEL53;
    uint AIR_DSELR1_DSEL52;
    uint AIR_DSELR1_DSEL51;
    uint AIR_DSELR1_DSEL50;
    uint AIR_DSELR1_DSEL49;
    uint AIR_DSELR1_DSEL48;
    uint AIR_DSELR1_DSEL47;
    uint AIR_DSELR1_DSEL46;
    uint AIR_DSELR1_DSEL45;
    uint AIR_DSELR1_DSEL44;
    uint AIR_DSELR1_DSEL43;
    uint AIR_DSELR1_DSEL42;
    uint AIR_DSELR1_DSEL41;
    uint AIR_DSELR1_DSEL40;
    uint AIR_DSELR1_DSEL39;
    uint AIR_DSELR1_DSEL38;
    uint AIR_DSELR1_DSEL37;
    uint AIR_DSELR1_DSEL36;
    uint AIR_DSELR1_DSEL35;
    uint AIR_DSELR1_DSEL34;
    uint AIR_DSELR1_DSEL33;
    uint AIR_DSELR1_DSEL32;

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

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    bool mAPBAccessMode;
    bool mDumpBitInfo;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Cair_core_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Cair_core_regif::*) (RegCBstr)> > mRdCbAPI;
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
    uint cw_rd_cb(tlm::tlm_generic_payload& trans, int tag);
    uint cw_wr_cb(tlm::tlm_generic_payload& trans, int tag);
    virtual void cw_set_callback_reg(scml2::reg<REG_TYPE> * reg, int offset);
    void cw_set_all_callback_reg(void);
    #endif
};
#endif //__AIR_CORE_REG_DEF_H__
