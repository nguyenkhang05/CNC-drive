// -----------------------------------------------------------------------------
// $Id: $
//
// Copyright(c) 2012-2018 Renesas Electronics Corporation
// Copyright(c) 2012-2018 Renesas Design Vietnam Co., Ltd.
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
// Input file : /shsv/sld/ipp/project/2018/rel/ipd2_sld_12006_rlin3/02_Output/DATA/src/rlin3_uart_regif.txt
////////////////////////////////////////////////////////////////////////////////
// # ---------------------------------------------------------------------
// # $Id: $
// #
// # Copyright(c) 2012-2018 Renesas Electronics Corporation
// # Copyright(c) 2012-2018 Renesas Design Vietnam Co., Ltd.
// # RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// # This program must be used solely for the purpose for which
// # it was furnished by Renesas Electronics Corporation. No part of this
// # program may be reproduced or disclosed to others, in any
// # form, without the prior written permission of Renesas Electronics
// # Corporation.
// # ---------------------------------------------------------------------
// 
// %MODULE RLIN3_UART 
//     #              name     offset_size
//     %%REG_INSTANCE reg_def  6
// 
// %REG_CHANNEL reg_def
//     %%TITLE    name       reg_name       size       length  offset    access  init     support  callback
//     %%REG      LWBR       LWBR           8|16|32    8       0x01      -       0        TRUE     W
//     %%REG      LBRP0      LBRP0          8|16|32    8       0x02      -       0        TRUE     W  
//     %%REG      LBRP1      LBRP1          8|16|32    8       0x03      -       0        TRUE     W
//     %%REG      LSTC       LSTC           8|16|32    8       0x04      -       0        TRUE     W
//     %%REG      LMD        LMD            8|16|32    8       0x08      -       0        TRUE     W
//     %%REG      LBFC       LBFC           8|16|32    8       0x09      -       0        TRUE     W
//     %%REG      LSC        LSC            8|16|32    8       0x0A      -       0        TRUE     W
//     %%REG      LWUP       LWUP           8|16|32    8       0x0B      -       0        TRUE     W
//     %%REG      LIE        LIE            8|16|32    8       0x0C      -       0        TRUE     W
//     %%REG      LEDE       LEDE           8|16|32    8       0x0D      -       0        TRUE     W
//     %%REG      LCUC       LCUC           8|16|32    8       0x0E      -       0        TRUE     W
//     %%REG      LTRC       LTRC           8|16|32    8       0x10      -       0        TRUE     W
//     %%REG      LMST       LMST           8|16|32    8       0x11      -       0        TRUE     W
//     %%REG      LST        LST            8|16|32    8       0x12      -       0        TRUE     W
//     %%REG      LEST       LEST           8|16|32    8       0x13      -       0        TRUE     W
//     %%REG      LDFC       LDFC           8|16|32    8       0x14      -       0        TRUE     W
//     %%REG      LIDB       LIDB           8|16|32    8       0x15      -       0        TRUE     W
//     %%REG      LCBR       LCBR           8|16|32    8       0x16      -       0        TRUE     W
//     %%REG      LUDB0      LUDB0          8|16|32    8       0x17      -       0        TRUE     W
//     %%REG      LDB1       LDB1           8|16|32    8       0x18      -       0        TRUE     W
//     %%REG      LDB2       LDB2           8|16|32    8       0x19      -       0        TRUE     W
//     %%REG      LDB3       LDB3           8|16|32    8       0x1A      -       0        TRUE     W
//     %%REG      LDB4       LDB4           8|16|32    8       0x1B      -       0        TRUE     W
//     %%REG      LDB5       LDB5           8|16|32    8       0x1C      -       0        TRUE     W
//     %%REG      LDB6       LDB6           8|16|32    8       0x1D      -       0        TRUE     W
//     %%REG      LDB7       LDB7           8|16|32    8       0x1E      -       0        TRUE     W
//     %%REG      LDB8       LDB8           8|16|32    8       0x1F      -       0        TRUE     W
//     %%REG      LUOER      LUOER          8|16|32    8       0x20      -       0        TRUE     W
//     %%REG      LUOR1      LUOR1          8|16|32    8       0x21      -       0        TRUE     W
//     %%REG      LUOR2      LUOR2          8|16|32    8       0x22      -       0        FALSE    -
//     %%REG      LUTDCR     LUTDCR         8|16|32    8       0x23      -       0        FALSE    -
//     %%REG      LUTDR      LUTDR          8|16       16      0x24      -       0        TRUE     W
//     %%REG      LURDR      LURDR          8|16       16      0x26      -       0        TRUE     R
//     %%REG      LUWTDR     LUWTDR         8|16       16      0x28      -       0        TRUE     W
//     %%REG      LURDE      LURDE          8|16       16      0x2A      -       0        TRUE     -
//     %%REG      LBSS       LBSS           8|16|32    8       0x30      -       0        TRUE     -
//     %%REG      LRSS       LRSS           8|16|32    8       0x34      -       0        TRUE     -
// 
// 
// %REG_NAME LWBR      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    LWBR0          0      0      0     R|W       TRUE     W   
//     %%BIT    LPRS           3      1      0     R|W       TRUE     -   
//     %%BIT    NSPB           7      4      0     R|W       TRUE     -   
// 
// %REG_NAME LBRP0      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    BRP            7      0      0     R|W       TRUE     W  
// 
// %REG_NAME LBRP1      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    BRP            7      0      0     R|W       TRUE     W    
// 
// %REG_NAME LSTC      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    LSTM           0      0      0     R|W       TRUE     W    
//     %%BIT    LSTME          6      1      0     R|W       TRUE     -   
//     %%BIT    LSFWC          7      7      0     R|W       FALSE    -   
// 
// %REG_NAME LMD      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    LMD            1      0      0     R|W       TRUE     W    
//     %%BIT    LCKS           3      2      0     R|W       TRUE     -      
//     %%BIT    LIOS           4      4      0     R|W       TRUE     -      
//     %%BIT    LRDNSF         5      5      0     R|W       FALSE    -      
// 
// %REG_NAME LBFC      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    UBLS           0      0      0     R|W       TRUE     W  
//     %%BIT    UBOS           1      1      0     R|W       TRUE     -  
//     %%BIT    USBLS          2      2      0     R|W       TRUE     -  
//     %%BIT    UPS            4      3      0     R|W       TRUE     -  
//     %%BIT    URPS           5      5      0     R|W       TRUE     -  
//     %%BIT    UTPS           6      6      0     R|W       TRUE     -  
// 
// %REG_NAME LSC      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    IBHS           2      0      0     R|W       TRUE     W    
//     %%BIT    IBS            5      4      0     R|W       TRUE     -    
// 
// %REG_NAME LWUP      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    WUTL           7      4      0     R|W       TRUE     W   
// 
// %REG_NAME LIE      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    FTCIE          0      0      0     R|W       TRUE     W   
//     %%BIT    FRCIE          1      1      0     R|W       TRUE     -   
//     %%BIT    ERRIE          2      2      0     R|W       TRUE     -   
//     %%BIT    SHIE           3      3      0     R|W       TRUE     -   
// 
// %REG_NAME LEDE      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    BERE           0      0      0     R|W       TRUE     W    
//     %%BIT    FERE           3      3      0     R|W       TRUE     -     
//     %%BIT    OERE           2      2      0     R|W       TRUE     -  
// 
// %REG_NAME LCUC      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    OM0            0      0      0     R|W       TRUE     W     
//     %%BIT    OM1            1      1      0     R|W       TRUE     -   
// 
// %REG_NAME LTRC      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    RTS            1      1      0     R|W       TRUE     W       
// 
// %REG_NAME LMST      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    OMM0           0      0      0     R|W       TRUE     W     
//     %%BIT    OMM1           1      1      0     R|W       TRUE     -     
// 
// %REG_NAME LST      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    FTC            0      0      0     R|W       TRUE     W        
//     %%BIT    ERR            3      3      0     R|W       TRUE     -      
//     %%BIT    UTS            4      4      0     R|W       TRUE     -    
//     %%BIT    URS            5      5      0     R|W       TRUE     -    
// 
// %REG_NAME LEST      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    BER            0      0      0     R|W       TRUE     W      
//     %%BIT    FER            3      3      0     R|W       TRUE     -      
//     %%BIT    OER            2      2      0     R|W       TRUE     -    
//     %%BIT    EXBT           4      4      0     R|W       TRUE     -    
//     %%BIT    IDMT           5      5      0     R|W       TRUE     -    
//     %%BIT    UPER           6      6      0     R|W       TRUE     -    
// 
// %REG_NAME LDFC      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    MDL            3      0      0     R|W       TRUE     W    
//     %%BIT    UTSW           5      5      0     R|W       TRUE     -    
// 
// %REG_NAME LIDB      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    ID             7      0      0     R|W       TRUE     W    
// 
// %REG_NAME LCBR      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    CKSM           7      0      0     R|W       TRUE     W      
// 
// %REG_NAME LUDB0      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    UDB            7      0      0     R|W       TRUE     W    
// 
// %REG_NAME LDB1      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    LDB            7      0      0     R|W       TRUE     W     
// 
// %REG_NAME LDB2      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    LDB            7      0      0     R|W       TRUE     W     
// 
// %REG_NAME LDB3      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    LDB            7      0      0     R|W       TRUE     W     
// 
// %REG_NAME LDB4      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    LDB            7      0      0     R|W       TRUE     W     
// 
// %REG_NAME LDB5      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    LDB            7      0      0     R|W       TRUE     W     
// 
// %REG_NAME LDB6      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    LDB            7      0      0     R|W       TRUE     W     
// 
// %REG_NAME LDB7      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    LDB            7      0      0     R|W       TRUE     W     
// 
// %REG_NAME LDB8      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    LDB            7      0      0     R|W       TRUE     W     
// 
// %REG_NAME LUOER      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    UTOE           0      0      0     R|W       TRUE     W    
//     %%BIT    UROE           1      1      0     R|W       TRUE     -    
// 
// %REG_NAME LUOR1      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    UEBE           0      0      0     R|W       TRUE     W  
//     %%BIT    UEBDL          1      1      0     R|W       TRUE     -  
//     %%BIT    UEBDCE         2      2      0     R|W       TRUE     -  
//     %%BIT    UTIGTS         3      3      0     R|W       TRUE     -  
//     %%BIT    UECD           4      4      0     R|W       TRUE     -  
// 
// %REG_NAME LUTDR      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    UTD            8      0      0     R|W       TRUE     W    
// 
// %REG_NAME LURDR      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    URD            8      0      0     R         TRUE     R 
// 
// %REG_NAME LUWTDR      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    UWTD           8      0      0     R|W       TRUE     W  
// 
// %REG_NAME LURDE      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    RDE            8      0      0     R         TRUE     -    
// 
// %REG_NAME LBSS      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    BRKC           0      0      0     R       TRUE     -    
//     %%BIT    SYCC           1      1      0     R       TRUE     -    
// 
// %REG_NAME LRSS      
//     %%TITLE  name           upper  lower  init  access  support  callback    
//     %%BIT    RSDD           0      0      0     R       TRUE     -    
////////////////////////////////////////////////////////////////////////////////
/// @file rlin3_uart_regif.h
/// @brief Register IF class of model RLIN3_UART
/// $Id$
/// $Date$
/// $Revison$
/// $Author$
////////////////////////////////////////////////////////////////////////////////
#ifndef __RLIN3_UART_REGIF_H__
#define __RLIN3_UART_REGIF_H__
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

/// Register IF class of RLIN3_UART model
class Crlin3_uart_regif
: public vpcl::reg_super
{
protected:
    typedef const unsigned int cuint;
    typedef unsigned int uint;
#ifdef USE_CWR_SYSTEMC
    typedef unsigned short REG_TYPE;
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

    typedef void (Crlin3_uart_regif::* ptrRegValueIDFunc) (std::string);
    ptrRegValueIDFunc mNotifyRegValueIDAPI;
    virtual void NotifyRegValueID(const std::string reg_val_id) = 0;
#endif

public:
    Crlin3_uart_regif(std::string name, uint buswidth);
    ~Crlin3_uart_regif();

protected:
    vpcl::re_register *LWBR  ;
    vpcl::re_register *LBRP0 ;
    vpcl::re_register *LBRP1 ;
    vpcl::re_register *LSTC  ;
    vpcl::re_register *LMD   ;
    vpcl::re_register *LBFC  ;
    vpcl::re_register *LSC   ;
    vpcl::re_register *LWUP  ;
    vpcl::re_register *LIE   ;
    vpcl::re_register *LEDE  ;
    vpcl::re_register *LCUC  ;
    vpcl::re_register *LTRC  ;
    vpcl::re_register *LMST  ;
    vpcl::re_register *LST   ;
    vpcl::re_register *LEST  ;
    vpcl::re_register *LDFC  ;
    vpcl::re_register *LIDB  ;
    vpcl::re_register *LCBR  ;
    vpcl::re_register *LUDB0 ;
    vpcl::re_register *LDB1  ;
    vpcl::re_register *LDB2  ;
    vpcl::re_register *LDB3  ;
    vpcl::re_register *LDB4  ;
    vpcl::re_register *LDB5  ;
    vpcl::re_register *LDB6  ;
    vpcl::re_register *LDB7  ;
    vpcl::re_register *LDB8  ;
    vpcl::re_register *LUOER ;
    vpcl::re_register *LUOR1 ;
    vpcl::re_register *LUOR2 ;
    vpcl::re_register *LUTDCR;
    vpcl::re_register *LUTDR ;
    vpcl::re_register *LURDR ;
    vpcl::re_register *LUWTDR;
    vpcl::re_register *LURDE ;
    vpcl::re_register *LBSS  ;
    vpcl::re_register *LRSS  ;

    #ifdef USE_CWR_SYSTEMC
    scml2::memory<REG_TYPE> cwmem;
    #endif

    uint LWBR_LWBR0;
    uint LWBR_LPRS;
    uint LWBR_NSPB;
    uint LBRP0_BRP;
    uint LBRP1_BRP;
    uint LSTC_LSTM;
    uint LSTC_LSTME;
    uint LSTC_LSFWC;
    uint LMD_LMD;
    uint LMD_LCKS;
    uint LMD_LIOS;
    uint LMD_LRDNSF;
    uint LBFC_UBLS;
    uint LBFC_UBOS;
    uint LBFC_USBLS;
    uint LBFC_UPS;
    uint LBFC_URPS;
    uint LBFC_UTPS;
    uint LSC_IBHS;
    uint LSC_IBS;
    uint LWUP_WUTL;
    uint LIE_FTCIE;
    uint LIE_FRCIE;
    uint LIE_ERRIE;
    uint LIE_SHIE;
    uint LEDE_BERE;
    uint LEDE_FERE;
    uint LEDE_OERE;
    uint LCUC_OM0;
    uint LCUC_OM1;
    uint LTRC_RTS;
    uint LMST_OMM0;
    uint LMST_OMM1;
    uint LST_FTC;
    uint LST_ERR;
    uint LST_UTS;
    uint LST_URS;
    uint LEST_BER;
    uint LEST_FER;
    uint LEST_OER;
    uint LEST_EXBT;
    uint LEST_IDMT;
    uint LEST_UPER;
    uint LDFC_MDL;
    uint LDFC_UTSW;
    uint LIDB_ID;
    uint LCBR_CKSM;
    uint LUDB0_UDB;
    uint LDB1_LDB;
    uint LDB2_LDB;
    uint LDB3_LDB;
    uint LDB4_LDB;
    uint LDB5_LDB;
    uint LDB6_LDB;
    uint LDB7_LDB;
    uint LDB8_LDB;
    uint LUOER_UTOE;
    uint LUOER_UROE;
    uint LUOR1_UEBE;
    uint LUOR1_UEBDL;
    uint LUOR1_UEBDCE;
    uint LUOR1_UTIGTS;
    uint LUOR1_UECD;
    uint LUOR2_LUOR2;
    uint LUTDCR_LUTDCR;
    uint LUTDR_UTD;
    uint LURDR_URD;
    uint LUWTDR_UWTD;
    uint LURDE_RDE;
    uint LBSS_BRKC;
    uint LBSS_SYCC;
    uint LRSS_RSDD;

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

    virtual void cb_LWBR_LWBR0(RegCBstr str) = 0;
    virtual void cb_LBRP0_BRP(RegCBstr str) = 0;
    virtual void cb_LBRP1_BRP(RegCBstr str) = 0;
    virtual void cb_LSTC_LSTM(RegCBstr str) = 0;
    virtual void cb_LMD_LMD(RegCBstr str) = 0;
    virtual void cb_LBFC_UBLS(RegCBstr str) = 0;
    virtual void cb_LSC_IBHS(RegCBstr str) = 0;
    virtual void cb_LWUP_WUTL(RegCBstr str) = 0;
    virtual void cb_LIE_FTCIE(RegCBstr str) = 0;
    virtual void cb_LEDE_BERE(RegCBstr str) = 0;
    virtual void cb_LCUC_OM0(RegCBstr str) = 0;
    virtual void cb_LTRC_RTS(RegCBstr str) = 0;
    virtual void cb_LMST_OMM0(RegCBstr str) = 0;
    virtual void cb_LST_FTC(RegCBstr str) = 0;
    virtual void cb_LEST_BER(RegCBstr str) = 0;
    virtual void cb_LDFC_MDL(RegCBstr str) = 0;
    virtual void cb_LIDB_ID(RegCBstr str) = 0;
    virtual void cb_LCBR_CKSM(RegCBstr str) = 0;
    virtual void cb_LUDB0_UDB(RegCBstr str) = 0;
    virtual void cb_LDB1_LDB(RegCBstr str) = 0;
    virtual void cb_LDB2_LDB(RegCBstr str) = 0;
    virtual void cb_LDB3_LDB(RegCBstr str) = 0;
    virtual void cb_LDB4_LDB(RegCBstr str) = 0;
    virtual void cb_LDB5_LDB(RegCBstr str) = 0;
    virtual void cb_LDB6_LDB(RegCBstr str) = 0;
    virtual void cb_LDB7_LDB(RegCBstr str) = 0;
    virtual void cb_LDB8_LDB(RegCBstr str) = 0;
    virtual void cb_LUOER_UTOE(RegCBstr str) = 0;
    virtual void cb_LUOR1_UEBE(RegCBstr str) = 0;
    virtual void cb_LUTDR_UTD(RegCBstr str) = 0;
    virtual void cb_LURDR_URD(RegCBstr str) = 0;
    virtual void cb_LUWTDR_UWTD(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    bool mAPBAccessMode;
    bool mDumpBitInfo;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Crlin3_uart_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Crlin3_uart_regif::*) (RegCBstr)> > mRdCbAPI;
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
    unsigned short cw_rd_cb(tlm::tlm_generic_payload& trans, int tag);
    unsigned short cw_wr_cb(tlm::tlm_generic_payload& trans, int tag);
    virtual void cw_set_callback_reg(scml2::reg<REG_TYPE> * reg, int offset);
    void cw_set_all_callback_reg(void);
    #endif
};
#endif //__RLIN3_UART_REG_DEF_H__
