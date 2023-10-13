// -----------------------------------------------------------------------------
// $Id: emu3s_hwcore_regif.h,v 1.3 2020/10/27 04:26:15 huyquocpham Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
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
// Input file : emu3s_hwcore_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE EMU3S_HWCORE
//     #              name     offset_size
//     %%REG_INSTANCE reg_def  8
// 
// %REG_CHANNEL reg_def
//   %%TITLE    name               reg_name             size         wsize         rsize       length      offset    factor_start   factor_end   factor_index   factor_step     init        access    support    callback
//   %%REG      PRT                 PRT                   8                8       8|16|32         8       0x000        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      CTR                 CTR                   8                8       8|16|32         8       0x004        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      REFCTR              REFCTR                8                8       8|16|32         8       0x008        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      AHBPRT              AHBPRT                8                8       8|16|32         8       0x00C        -             -            -             -             0x0000000D       R|W       TRUE       -
//   %%REG      AHBPRI              AHBPRI                8                8       8|16|32         8       0x00D        -             -            -             -             0x00000001       R|W       TRUE       -
//   %%REG      APBPRI              APBPRI                8                8       8|16|32         8       0x00E        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      IPTRG               IPTRG                 8                8       8|16|32         8       0x010        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      IPSFT               IPSFT                 8                8       8|16|32         8       0x014        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      TBLADR              TBLADR               32               32       8|16|32        32       0x018        -             -            -             -             0xFFFFFFFC       R|W       TRUE       -
//   %%REG      IPTRGCTR            IPTRGCTR              8                8       8|16|32         8       0x020        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      IPTRGNUM            IPTRGNUM             32          8|16|32       8|16|32        32       0x024        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      IPTRGMON            IPTRGMON             32                -       8|16|32        32       0x028        -             -            -             -             0x00000000       R         TRUE       -
//   %%REG      FUNCIDLEGRPA0       FUNCIDLEGRPA0        16               16       8|16|32        16       0x040        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      FUNCIDLEGRPA1       FUNCIDLEGRPA1        16               16       8|16|32        16       0x042        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      FUNCIDLEGRPA2       FUNCIDLEGRPA2        16               16       8|16|32        16       0x044        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      FUNCFINGRPA         FUNCFINGRPA          16                -       8|16|32        16       0x046        -             -            -             -             0x00000000       R         TRUE       -
//   %%REG      FUNCIDLEGRPB        FUNCIDLEGRPB         16               16       8|16|32        16       0x048        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      FUNCFINGRPB         FUNCFINGRPB          16                -       8|16|32        16       0x04A        -             -            -             -             0x00000000       R         TRUE       -
//   %%REG      FUNCWAITGRPA        FUNCWAITGRPA         16               16       8|16|32        16       0x050        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      FUNCWAITGRPB        FUNCWAITGRPB         16               16       8|16|32        16       0x054        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      FUNCSTGRPA          FUNCSTGRPA           32               32       8|16|32        32       0x058        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      FUNCSTGRPB          FUNCSTGRPB           16               16       8|16|32        16       0x05C        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      FSMSTGRPA           FSMSTGRPA            32                -       8|16|32        32       0x068        -             -            -             -             0x00000000       R         TRUE       -
//   %%REG      FSMSTGRPB           FSMSTGRPB            32                -       8|16|32        32       0x06C        -             -            -             -             0x00000000       R         TRUE       -
//   %%REG      FUNCFLGRPA0         FUNCFLGRPA0          16               16       8|16|32        16       0x070        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      FUNCFLGRPA1         FUNCFLGRPA1           8                8       8|16|32         8       0x074        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      FUNCFLGRPA2         FUNCFLGRPA2          32               32       8|16|32        32       0x078        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      FUNCFLGRPB          FUNCFLGRPB           32               32       8|16|32        32       0x07C        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      RECMD               RECMD                 8                8       8|16|32         8       0x0A0        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      ADRDCSL             ADRDCSL               8                8       8|16|32         8       0x0A4        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      EDCCTR              EDCCTR                8                8       8|16|32         8       0x0B0        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      SFERC               SFERC                 8                8       8|16|32         8       0x0B1        -             -            -             -             0x00000033       R|W       TRUE       -
//   %%REG      EDCSERR             EDCSERR               8                -       8|16|32         8       0x0B4        -             -            -             -             0x00000000       R         TRUE       -
//   %%REG      EDCDERR             EDCDERR               8                -       8|16|32         8       0x0B5        -             -            -             -             0x00000000       R         TRUE       -
//   %%REG      EDCSECLR            EDCSECLR              8                8       8|16|32         8       0x0B8        -             -            -             -             0x00000000       R|W       TRUE       -
//   %%REG      EDCDECLR            EDCDECLR              8                8       8|16|32         8       0x0B9        -             -            -             -             0x00000000       R|W       TRUE       -
// 
// %REG_NAME PRT
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   MEM0PRT           4       4     0x0       R|W      TRUE     W
//     %%BIT   PRTCT             0       0     0x0       R|W      TRUE     -
// 
// %REG_NAME CTR
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   EMUST             0       0     0x0       R|W      TRUE     W
// 
// %REG_NAME REFCTR
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   FPWMREFPER        0       0     0x0       R|W      TRUE     W
// 
// %REG_NAME AHBPRT
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   HPRT              3       0     0xD       R|W      TRUE     W
// 
// %REG_NAME AHBPRI
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   HPRI              1       0     0x1       R|W      TRUE     -
// 
// %REG_NAME APBPRI
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   PPRI              1       0     0x0       R|W      TRUE     -
// 
// %REG_NAME IPTRG
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   RECIPTRG          4       4     0x0       R|W      TRUE     -
//     %%BIT   PWMIPTRG          3       3     0x0       R|W      TRUE     -
//     %%BIT   PIIPTRG           2       2     0x0       R|W      TRUE     -
//     %%BIT   INIPTRG           1       0     0x0       R|W      TRUE     -
// 
// %REG_NAME IPSFT
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   RECIPSFT          3       3     0x0       R|W      TRUE     -
//     %%BIT   PWMIPSFT          2       2     0x0       R|W      TRUE     -
//     %%BIT   PIIPSFT           1       1     0x0       R|W      TRUE     -
//     %%BIT   INIPSFT           0       0     0x0       R|W      TRUE     W
// 
// %REG_NAME TBLADR
//     %%TITLE  name           upper   lower   init            access   support   callback
//     %%BIT   DATA             31       0     0xFFFFFFFC      R|W      TRUE      W
// 
// %REG_NAME IPTRGCTR
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   TNEN              0       0     0x0       R|W      TRUE     W
// 
// %REG_NAME IPTRGNUM 
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   PWMTRGNUM        31      24     0x0       R|W      TRUE     -
//     %%BIT   PITRGNUM         23      16     0x0       R|W      TRUE     -
//     %%BIT   INTRGNUM         15       8     0x0       R|W      TRUE     -
//     %%BIT   ADTRGNUM          7       0     0x0       R|W      TRUE     W
// 
// %REG_NAME IPTRGMON 
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   PWMTRGNUM        31      24     0x0       R        TRUE     -
//     %%BIT   PITRGNUM         23      16     0x0       R        TRUE     -
//     %%BIT   INTRGNUM         15       8     0x0       R        TRUE     -
//     %%BIT   ADTRGNUM          7       0     0x0       R        TRUE     R
//     
// %REG_NAME FUNCIDLEGRPA0
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   SFTEN            12      12     0x0       R|W      TRUE     W
//     %%BIT   IP                8       8     0x0       R|W      TRUE     -
//     %%BIT   FUNC              2       0     0x0       R|W      TRUE     -
// 
// %REG_NAME FUNCIDLEGRPA1
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   SFTEN            12      12     0x0       R|W      TRUE     W
//     %%BIT   IP                8       8     0x0       R|W      TRUE     -
//     %%BIT   FUNC              0       0     0x0       R|W      TRUE     -
// 
// %REG_NAME FUNCIDLEGRPA2
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   SFTEN            12      12     0x0       R|W      TRUE     W
//     %%BIT   IP                8       8     0x0       R|W      TRUE     -
//     %%BIT   FUNC              3       0     0x0       R|W      TRUE     -
// 
// %REG_NAME FUNCFINGRPA
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   IP                9       8     0x0       R        TRUE     -
//     %%BIT   FUNC              3       0     0x0       R        TRUE     -
// 
// %REG_NAME FUNCIDLEGRPB
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   IP                8       8     0x0       R|W      TRUE     W
//     %%BIT   FUNC              3       0     0x0       R|W      TRUE     -
// 
// %REG_NAME FUNCFINGRPB
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   IP                8       8     0x0       R        TRUE     -
//     %%BIT   FUNC              3       0     0x0       R        TRUE     -
// 
// %REG_NAME FUNCWAITGRPA
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   IP                9       8     0x0       R|W      TRUE     W
//     %%BIT   FUNC              3       0     0x0       R|W      TRUE     -
// 
// %REG_NAME FUNCWAITGRPB
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   IP                8       8     0x0       R|W      TRUE     W
//     %%BIT   FUNC              3       0     0x0       R|W      TRUE     -
// 
// %REG_NAME FUNCSTGRPA
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   PWM10            19      19     0x0       R|W      TRUE     -
//     %%BIT   PWM9             18      18     0x0       R|W      TRUE     -
//     %%BIT   PWM8             17      17     0x0       R|W      TRUE     -
//     %%BIT   PWM7             16      16     0x0       R|W      TRUE     -
//     %%BIT   PWM6             15      15     0x0       R|W      TRUE     -
//     %%BIT   PWM5             14      14     0x0       R|W      TRUE     -
//     %%BIT   PWM4             13      13     0x0       R|W      TRUE     -
//     %%BIT   PWM3             12      12     0x0       R|W      TRUE     -
//     %%BIT   PWM2             11      11     0x0       R|W      TRUE     -
//     %%BIT   PWM1             10      10     0x0       R|W      TRUE     -
//     %%BIT   PWM0              9       9     0x0       R|W      TRUE     -
//     %%BIT   PI1               8       8     0x0       R|W      TRUE     -
//     %%BIT   PI0               7       7     0x0       R|W      TRUE     -
//     %%BIT   INPUT3            3       3     0x0       R|W      TRUE     -
//     %%BIT   INPUT2            2       2     0x0       R|W      TRUE     -
//     %%BIT   INPUT1            1       1     0x0       R|W      TRUE     -
//     %%BIT   INPUT0            0       0     0x0       R|W      TRUE     W
// 
// %REG_NAME FUNCSTGRPB
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   ANG3              3       3     0x0       R|W      TRUE     -
//     %%BIT   ANG2              2       2     0x0       R|W      TRUE     -
//     %%BIT   ANG1              1       1     0x0       R|W      TRUE     -
//     %%BIT   ANG0              0       0     0x0       R|W      TRUE     W
// 
// %REG_NAME FSMSTGRPA
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   IP               17      16     0x0       R        TRUE     -
//     %%BIT   WAIT              8       8     0x0       R        TRUE     -
//     %%BIT   BUSY              0       0     0x0       R        TRUE     -
// 
// %REG_NAME FSMSTGRPB
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   IP               16      16     0x0       R        TRUE     -
//     %%BIT   WAIT              8       8     0x0       R        TRUE     -
//     %%BIT   BUSY              0       0     0x0       R        TRUE     -
// 
// %REG_NAME FUNCFLGRPA0
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   INPUT3            7       6     0x0       R|W      TRUE     -
//     %%BIT   INPUT2            5       4     0x0       R|W      TRUE     -
//     %%BIT   INPUT1            3       2     0x0       R|W      TRUE     -
//     %%BIT   INPUT0            1       0     0x0       R|W      TRUE     W
// 
// %REG_NAME FUNCFLGRPA1
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   PI1               3       2     0x0       R|W      TRUE     -
//     %%BIT   PI0               1       0     0x0       R|W      TRUE     W
// 
// %REG_NAME FUNCFLGRPA2
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   PWM10            21      20     0x0       R|W      TRUE     -
//     %%BIT   PWM9             19      18     0x0       R|W      TRUE     -
//     %%BIT   PWM8             17      16     0x0       R|W      TRUE     -
//     %%BIT   PWM7             15      14     0x0       R|W      TRUE     -
//     %%BIT   PWM6             13      12     0x0       R|W      TRUE     -
//     %%BIT   PWM5             11      10     0x0       R|W      TRUE     -
//     %%BIT   PWM4              9       8     0x0       R|W      TRUE     -
//     %%BIT   PWM3              7       6     0x0       R|W      TRUE     -
//     %%BIT   PWM2              5       4     0x0       R|W      TRUE     -
//     %%BIT   PWM1              3       2     0x0       R|W      TRUE     -
//     %%BIT   PWM0              1       0     0x0       R|W      TRUE     W
// 
// %REG_NAME FUNCFLGRPB
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   ANG3              7       6     0x0       R|W      TRUE     -
//     %%BIT   ANG2              5       4     0x0       R|W      TRUE     -
//     %%BIT   ANG1              3       2     0x0       R|W      TRUE     -
//     %%BIT   ANG0              1       0     0x0       R|W      TRUE     W
// 
// %REG_NAME RECMD
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   RECMD             0       0     0x0       R|W      TRUE     -
// 
// %REG_NAME ADRDCSL
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   ADSEL             1       1     0x0       R|W      TRUE     W
//     %%BIT   RDCSEL            0       0     0x0       R|W      TRUE     -
// 
// %REG_NAME EDCCTR
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   EDCDIS            0       0     0x0       R|W      TRUE     W
// 
// %REG_NAME SFERC
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   CDNE              5       5     0x1       R|W      TRUE     -
//     %%BIT   PDNE              4       4     0x1       R|W      TRUE     -
//     %%BIT   DENE              1       1     0x1       R|W      TRUE     -
//     %%BIT   SENE              0       0     0x1       R|W      TRUE     W
// 
// %REG_NAME EDCSERR
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   SES               0       0     0x0       R        TRUE     -
// 
// %REG_NAME EDCDERR
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   DES               0       0     0x0       R        TRUE     -
// 
// %REG_NAME EDCSECLR
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   SESCLR            0       0     0x0       R|W      TRUE     W
// 
// %REG_NAME EDCDECLR
//     %%TITLE  name           upper   lower   init    access   support   callback
//     %%BIT   DESCLR            0       0     0x0       R|W      TRUE     W
//     ////////////////////////////////////////////////////////////////////////////////
/// @file emu3s_hwcore_regif.h
/// @brief Register IF class of model EMU3S_HWCORE
/// $Id: emu3s_hwcore_regif.h,v 1.3 2020/10/27 04:26:15 huyquocpham Exp $
/// $Date: 2020/10/27 04:26:15 $
/// $Revison$
/// $Author: huyquocpham $
////////////////////////////////////////////////////////////////////////////////
#ifndef __EMU3S_HWCORE_REGIF_H__
#define __EMU3S_HWCORE_REGIF_H__
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

/// Register IF class of EMU3S_HWCORE model
class Cemu3s_hwcore_regif
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

    typedef void (Cemu3s_hwcore_regif::* ptrRegValueIDFunc) (std::string);
    ptrRegValueIDFunc mNotifyRegValueIDAPI;
    virtual void NotifyRegValueID(const std::string reg_val_id) = 0;
#endif

public:
    Cemu3s_hwcore_regif(std::string name, uint buswidth);
    ~Cemu3s_hwcore_regif();

protected:
    vpcl::re_register *PRT          ;
    vpcl::re_register *CTR          ;
    vpcl::re_register *REFCTR       ;
    vpcl::re_register *AHBPRT       ;
    vpcl::re_register *AHBPRI       ;
    vpcl::re_register *APBPRI       ;
    vpcl::re_register *IPTRG        ;
    vpcl::re_register *IPSFT        ;
    vpcl::re_register *TBLADR       ;
    vpcl::re_register *IPTRGCTR     ;
    vpcl::re_register *IPTRGNUM     ;
    vpcl::re_register *IPTRGMON     ;
    vpcl::re_register *FUNCIDLEGRPA0;
    vpcl::re_register *FUNCIDLEGRPA1;
    vpcl::re_register *FUNCIDLEGRPA2;
    vpcl::re_register *FUNCFINGRPA  ;
    vpcl::re_register *FUNCIDLEGRPB ;
    vpcl::re_register *FUNCFINGRPB  ;
    vpcl::re_register *FUNCWAITGRPA ;
    vpcl::re_register *FUNCWAITGRPB ;
    vpcl::re_register *FUNCSTGRPA   ;
    vpcl::re_register *FUNCSTGRPB   ;
    vpcl::re_register *FSMSTGRPA    ;
    vpcl::re_register *FSMSTGRPB    ;
    vpcl::re_register *FUNCFLGRPA0  ;
    vpcl::re_register *FUNCFLGRPA1  ;
    vpcl::re_register *FUNCFLGRPA2  ;
    vpcl::re_register *FUNCFLGRPB   ;
    vpcl::re_register *RECMD        ;
    vpcl::re_register *ADRDCSL      ;
    vpcl::re_register *EDCCTR       ;
    vpcl::re_register *SFERC        ;
    vpcl::re_register *EDCSERR      ;
    vpcl::re_register *EDCDERR      ;
    vpcl::re_register *EDCSECLR     ;
    vpcl::re_register *EDCDECLR     ;

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

    virtual void cb_PRT_MEM0PRT(RegCBstr str) = 0;
    virtual void cb_CTR_EMUST(RegCBstr str) = 0;
    virtual void cb_REFCTR_FPWMREFPER(RegCBstr str) = 0;
    virtual void cb_AHBPRT_HPRT(RegCBstr str) = 0;
    virtual void cb_IPSFT_INIPSFT(RegCBstr str) = 0;
    virtual void cb_TBLADR_DATA(RegCBstr str) = 0;
    virtual void cb_IPTRGCTR_TNEN(RegCBstr str) = 0;
    virtual void cb_IPTRGNUM_ADTRGNUM(RegCBstr str) = 0;
    virtual void cb_IPTRGMON_ADTRGNUM(RegCBstr str) = 0;
    virtual void cb_FUNCIDLEGRPA0_SFTEN(RegCBstr str) = 0;
    virtual void cb_FUNCIDLEGRPA1_SFTEN(RegCBstr str) = 0;
    virtual void cb_FUNCIDLEGRPA2_SFTEN(RegCBstr str) = 0;
    virtual void cb_FUNCIDLEGRPB_IP(RegCBstr str) = 0;
    virtual void cb_FUNCWAITGRPA_IP(RegCBstr str) = 0;
    virtual void cb_FUNCWAITGRPB_IP(RegCBstr str) = 0;
    virtual void cb_FUNCSTGRPA_INPUT0(RegCBstr str) = 0;
    virtual void cb_FUNCSTGRPB_ANG0(RegCBstr str) = 0;
    virtual void cb_FUNCFLGRPA0_INPUT0(RegCBstr str) = 0;
    virtual void cb_FUNCFLGRPA1_PI0(RegCBstr str) = 0;
    virtual void cb_FUNCFLGRPA2_PWM0(RegCBstr str) = 0;
    virtual void cb_FUNCFLGRPB_ANG0(RegCBstr str) = 0;
    virtual void cb_ADRDCSL_ADSEL(RegCBstr str) = 0;
    virtual void cb_EDCCTR_EDCDIS(RegCBstr str) = 0;
    virtual void cb_SFERC_SENE(RegCBstr str) = 0;
    virtual void cb_EDCSECLR_SESCLR(RegCBstr str) = 0;
    virtual void cb_EDCDECLR_DESCLR(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    bool mAPBAccessMode;
    bool mDumpBitInfo;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Cemu3s_hwcore_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Cemu3s_hwcore_regif::*) (RegCBstr)> > mRdCbAPI;
    std::map<std::string, std::map<std::string, void (Cemu3s_hwcore_regif::*) (RegCBstr)> > mWrDbgCbAPI;
    std::map<std::string, std::map<std::string, void (Cemu3s_hwcore_regif::*) (RegCBstr)> > mRdDbgCbAPI;
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
#endif //__EMU3S_HWCORE_REGIF_H__