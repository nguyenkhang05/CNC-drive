// -----------------------------------------------------------------------------
// $Id: mmc44_regif.h,v 1.3 2020/04/15 04:37:36 anhnguyen4 Exp $
//
// Copyright(c) 2018 - 2020 Renesas Electronics Corporation
// Copyright(c) 2018 - 2020 Renesas Design Vietnam Co., Ltd.
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
// Input file : /home/longnt20/gen/DummyperipheralRvc_input/mmc44_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE MMC44
//     #                   name            offset_size
//     %%REG_INSTANCE      reg_def         7
// 
// %REG_CHANNEL reg_def
//     %%TITLE  group  name                    reg_name                  wsize     rsize       length  offset  factor_start  factor_end  factor_index factor_step     access  init              support     callback
//     %%REG    -     MMCAnCE_CMD_SET          MMCAnCE_CMD_SET           32        8|16|32     32      0x0     -             -           -            -               R|W     0x00000000        TRUE            W
//     %%REG    -     MMCAnCE_ARG              MMCAnCE_ARG               32        8|16|32     32      0x8     -             -           -            -               R|W     0x00000000        TRUE            W
//     %%REG    -     MMCAnCE_ARG_CMD12        MMCAnCE_ARG_CMD12         32        8|16|32     32      0xC     -             -           -            -               R|W     0x00000000        TRUE            W
//     %%REG    -     MMCAnCE_CMD_CTRL         MMCAnCE_CMD_CTRL          32        8|16|32     32      0x10    -             -           -            -               R|W     0x00000000        TRUE            W
//     %%REG    -     MMCAnCE_BLOCK_SET        MMCAnCE_BLOCK_SET         32        8|16|32     32      0x14    -             -           -            -               R|W     0x00000000        TRUE            W
//     %%REG    -     MMCAnCE_CLK_CTRL         MMCAnCE_CLK_CTRL          32        8|16|32     32      0x18    -             -           -            -               R|W     0x00000000        TRUE            W
//     %%REG    -     MMCAnCE_BUF_ACC          MMCAnCE_BUF_ACC           32        8|16|32     32      0x1C    -             -           -            -               R|W     0x00000000        TRUE            W
//     %%REG    -     MMCAnCE_RESP3            MMCAnCE_RESP3             -         8|16|32     32      0x20    -             -           -            -               R       0x00000000        TRUE            -
//     %%REG    -     MMCAnCE_RESP2            MMCAnCE_RESP2             -         8|16|32     32      0x24    -             -           -            -               R       0x00000000        TRUE            -
//     %%REG    -     MMCAnCE_RESP1            MMCAnCE_RESP1             -         8|16|32     32      0x28    -             -           -            -               R       0x00000000        TRUE            -
//     %%REG    -     MMCAnCE_RESP0            MMCAnCE_RESP0             -         8|16|32     32      0x2C    -             -           -            -               R       0x00000000        TRUE            -
//     %%REG    -     MMCAnCE_RESP_CMD12       MMCAnCE_RESP_CMD12        -         8|16|32     32      0x30    -             -           -            -               R       0x00000000        TRUE            -
//     %%REG    -     MMCAnCE_DATA             MMCAnCE_DATA              32        8|16|32     32      0x34    -             -           -            -               R|W     0x00000000        TRUE            W|R
//     %%REG    -     MMCAnCE_BOOT             MMCAnCE_BOOT              32        8|16|32     32      0x3C    -             -           -            -               R|W     0x00000000        TRUE            W
//     %%REG    -     MMCAnCE_INT              MMCAnCE_INT               32        8|16|32     32      0x40    -             -           -            -               R|W     0x00000000        TRUE            W
//     %%REG    -     MMCAnCE_INT_EN           MMCAnCE_INT_EN            32        8|16|32     32      0x44    -             -           -            -               R|W     0x00000000        TRUE            W
//     %%REG    -     MMCAnCE_HOST_STS1        MMCAnCE_HOST_STS1         -         8|16|32     32      0x48    -             -           -            -               R       0x00000000        TRUE            -
//     %%REG    -     MMCAnCE_HOST_STS2        MMCAnCE_HOST_STS2         -         8|16|32     32      0x4C    -             -           -            -               R       0x00000000        TRUE            -
//     %%REG    -     MMCAnCE_SWRESA           MMCAnCE_SWRESA            32        8|16|32     32      0x7C    -             -           -            -               R|W     0x00000000        TRUE            W
//                                                                                                                                                                                                                                  
// %REG_NAME   MMCAnCE_CMD_SET                                                                                              
//     %%TITLE        name                upper    lower        init        access        support        callback             
//     %%BIT          BOOT                30       30           0           R|W           TRUE           W
//     %%BIT          CMD                 29       24           0           R|W           TRUE           -
//     %%BIT          RTYP                23       22           0           R|W           TRUE           -
//     %%BIT          RBSY                21       21           0           R|W           TRUE           -
//     %%BIT          WDAT                19       19           0           R|W           TRUE           -
//     %%BIT          DWEN                18       18           0           R|W           TRUE           -
//     %%BIT          CMLTE               17       17           0           R|W           TRUE           -
//     %%BIT          CMD12EN             16       16           0           R|W           TRUE           -
//     %%BIT          RIDXC               15       14           0           R|W           TRUE           -
//     %%BIT          RCRC7C              13       12           0           R|W           TRUE           -
//     %%BIT          CRC16C              10       10           0           R|W           TRUE           -
//     %%BIT          BOOTACK             9        9            0           R|W           TRUE           -
//     %%BIT          CRCSTE              8        8            0           R|W           TRUE           -
//     %%BIT          TBIT                7        7            0           R|W           TRUE           -
//     %%BIT          OPDM                6        6            0           R|W           TRUE           -
//     %%BIT          SBIT                3        3            0           R|W           TRUE           -
//     %%BIT          DATW                1        0            0           R|W           TRUE           W
//     
// %REG_NAME   MMCAnCE_ARG                                                                                              
//     %%TITLE        name                upper    lower        init        access        support        callback             
//     %%BIT          ARG                 31       0            0           R|W           TRUE           W
//     
// %REG_NAME   MMCAnCE_ARG_CMD12                                                                                              
//     %%TITLE        name                upper    lower        init        access        support        callback             
//     %%BIT          C12ARG              31       0            0           R|W           TRUE           W
//     
// %REG_NAME   MMCAnCE_CMD_CTRL                                                                                              
//     %%TITLE        name                upper    lower        init        access        support        callback             
//     %%BIT          BREAK               0        0            0           R|W           TRUE           W
//     
// %REG_NAME   MMCAnCE_BLOCK_SET                                                                                              
//     %%TITLE        name                upper    lower        init        access        support        callback             
//     %%BIT          BLKCNT              31       16           0           R|W           TRUE           -
//     %%BIT          BLKSIZ              15       0            0           R|W           TRUE           -
//     
// %REG_NAME   MMCAnCE_CLK_CTRL                                                                                              
//     %%TITLE        name                upper    lower        init        access        support        callback             
//     %%BIT          MMCBUSBSY           31       31           0           R             TRUE           -
//     %%BIT          CLKEN               24       24           0           R|W           TRUE           W
//     %%BIT          CLKDIV              19       16           0           R|W           TRUE           -
//     %%BIT          SRSPTO              13       12           0           R|W           TRUE           -
//     %%BIT          SRBSYTO             11       8            0           R|W           TRUE           -
//     %%BIT          SRWDTO              7        4            0           R|W           TRUE           -
//     
// %REG_NAME   MMCAnCE_BUF_ACC                                                                                              
//     %%TITLE        name                upper    lower        init        access        support        callback             
//     %%BIT          DMATYP              26       26           0           R|W           TRUE           W
//     %%BIT          DMAWEN              25       25           0           R|W           TRUE           -
//     %%BIT          DMAREN              24       24           0           R|W           TRUE           -
//     %%BIT          ATYP                16       16           0           R|W           TRUE           -
//                                                 
// %REG_NAME   MMCAnCE_RESP3                                                                                              
//     %%TITLE        name                upper    lower        init        access        support        callback             
//     %%BIT          RSP                 31       0            0           R             TRUE           -    
//     
// %REG_NAME   MMCAnCE_RESP2                                                                                              
//     %%TITLE        name                upper    lower        init        access        support        callback             
//     %%BIT          RSP                 31       0            0           R             TRUE           -        
//     
// %REG_NAME   MMCAnCE_RESP1                                                                                              
//     %%TITLE        name                upper    lower        init        access        support        callback             
//     %%BIT          RSP                 31       0            0           R             TRUE           -        
//     
// %REG_NAME   MMCAnCE_RESP0                                                                                              
//     %%TITLE        name                upper    lower        init        access        support        callback             
//     %%BIT          RSP                 31       0            0           R             TRUE           -        
//     
// %REG_NAME   MMCAnCE_RESP_CMD12                                                                                              
//     %%TITLE        name                upper    lower        init        access        support        callback             
//     %%BIT          RSP12               31       0            0           R             TRUE           -    
//     
// %REG_NAME   MMCAnCE_DATA                                                                                              
//     %%TITLE        name                upper    lower        init        access        support        callback             
//     %%BIT          DATA                31       0            0           R|W           TRUE           W|R    
//     
// %REG_NAME   MMCAnCE_BOOT                                                                                              
//     %%TITLE        name                upper    lower        init        access        support        callback             
//     %%BIT          BTCLKDIV            31       28            0           R|W           TRUE           W        
//     %%BIT          SBTACKTO            27       24            0           R|W           TRUE           -
//     %%BIT          S1STBTDATTO         23       20            0           R|W           TRUE           -
//     %%BIT          SBTDATTO            19       16            0           R|W           TRUE           -
//     
// %REG_NAME   MMCAnCE_INT     
//     %%TITLE        name                upper    lower        init        access        support        callback                                                                                             
//     %%BIT          CMD12DRE            26       26           0           R|W           TRUE           W
//     %%BIT          CMD12RBE            25       25           0           R|W           TRUE           -
//     %%BIT          CMD12CRE            24       24           0           R|W           TRUE           -
//     %%BIT          DTRANE              23       23           0           R|W           TRUE           -
//     %%BIT          BUFRE               22       22           0           R|W           TRUE           -
//     %%BIT          BUFWEN              21       21           0           R|W           TRUE           -
//     %%BIT          BUFREN              20       20           0           R|W           TRUE           -
//     %%BIT          RBSYE               17       17           0           R|W           TRUE           -
//     %%BIT          CRSPE               16       16           0           R|W           TRUE           -
//     %%BIT          CMDVIO              15       15           0           R|W           TRUE           -
//     %%BIT          BUFVIO              14       14           0           R|W           TRUE           -
//     %%BIT          WDATERR             11       11           0           R|W           TRUE           -
//     %%BIT          RDATERR             10       10           0           R|W           TRUE           -
//     %%BIT          RIDXERR             9        9            0           R|W           TRUE           -
//     %%BIT          RSPERR              8        8            0           R|W           TRUE           -
//     %%BIT          CRCSTO              4        4            0           R|W           TRUE           -
//     %%BIT          WDATTO              3        3            0           R|W           TRUE           -
//     %%BIT          RDATTO              2        2            0           R|W           TRUE           -
//     %%BIT          RBSYTO              1        1            0           R|W           TRUE           -
//     %%BIT          RSPTO               0        0            0           R|W           TRUE           -
//                                                                                                       
// %REG_NAME   MMCAnCE_INT_EN     
//     %%TITLE        name                upper    lower        init        access        support        callback                                                                                             
//     %%BIT          MCMD12DRE           26       26           0           R|W           TRUE           -
//     %%BIT          MCMD12RBE           25       25           0           R|W           TRUE           -
//     %%BIT          MCMD12CRE           24       24           0           R|W           TRUE           -
//     %%BIT          MDTRANE             23       23           0           R|W           TRUE           -
//     %%BIT          MBUFRE              22       22           0           R|W           TRUE           -
//     %%BIT          MBUFWEN             21       21           0           R|W           TRUE           W
//     %%BIT          MBUFREN             20       20           0           R|W           TRUE           -
//     %%BIT          MRBSYE              17       17           0           R|W           TRUE           -
//     %%BIT          MCRSPE              16       16           0           R|W           TRUE           -
//     %%BIT          MCMDVIO             15       15           0           R|W           TRUE           -
//     %%BIT          MBUFVIO             14       14           0           R|W           TRUE           -
//     %%BIT          MWDATERR            11       11           0           R|W           TRUE           -
//     %%BIT          MRDATERR            10       10           0           R|W           TRUE           -
//     %%BIT          MRIDXERR            9        9            0           R|W           TRUE           -
//     %%BIT          MRSPERR             8        8            0           R|W           TRUE           -
//     %%BIT          MCRCSTO             4        4            0           R|W           TRUE           -
//     %%BIT          MWDATTO             3        3            0           R|W           TRUE           -
//     %%BIT          MRDATTO             2        2            0           R|W           TRUE           -
//     %%BIT          MRBSYTO             1        1            0           R|W           TRUE           -
//     %%BIT          MRSPTO              0        0            0           R|W           TRUE           -
//     
// %REG_NAME   MMCAnCE_HOST_STS1
//     %%TITLE        name                upper    lower        init        access        support        callback
//     %%BIT          CMDSEQ              31       31           0           R             TRUE           -
//     %%BIT          CMDSIG              30       30           0           R             TRUE           -
//     %%BIT          RSPIDX              29       24           0           R             TRUE           -
//     %%BIT          DATSIG              23       16           0           R             TRUE           -
//     %%BIT          RCVBLK              15       0            0           R             TRUE           -              
//                
// %REG_NAME   MMCAnCE_HOST_STS2     
//     %%TITLE        name                upper    lower        init        access        support        callback                                                                                             
//     %%BIT          CRCSTE              31       31           0           R             TRUE           -
//     %%BIT          CRC16E              30       30           0           R             TRUE           -
//     %%BIT          AC12CRCE            29       29           0           R             TRUE           -
//     %%BIT          RSPCRC7E            28       28           0           R             TRUE           -
//     %%BIT          CRCSTEBE            27       27           0           R             TRUE           -
//     %%BIT          RDATEBE             26       26           0           R             TRUE           -
//     %%BIT          AC12REBE            25       25           0           R             TRUE           -
//     %%BIT          RSPEBE              24       24           0           R             TRUE           -
//     %%BIT          AC12IDXE            23       23           0           R             TRUE           -
//     %%BIT          RSPIDXE             22       22           0           R             TRUE           -
//     %%BIT          BTACKPATE           21       21           0           R             TRUE           -
//     %%BIT          BTACKEBE            20       20           0           R             TRUE           -
//     %%BIT          CRCST               18       16           0           R             TRUE           -
//     %%BIT          STRDATTO            14       14           0           R             TRUE           -
//     %%BIT          DATBSYTO            13       13           0           R             TRUE           -
//     %%BIT          CRCSTTO             12       12           0           R             TRUE           -
//     %%BIT          AC12BSYTO           11       11           0           R             TRUE           -
//     %%BIT          RSPBSYTO            10       10           0           R             TRUE           -
//     %%BIT          AC12RSPTO           9        9            0           R             TRUE           -
//     %%BIT          STRSPTO             8        8            0           R             TRUE           -
//     %%BIT          BTACKTO             7        7            0           R             TRUE           -
//     %%BIT          1STBTDATTO          6        6            0           R             TRUE           -
//     %%BIT          BTDATTO             5        5            0           R             TRUE           -
//                                                                                                    
// %REG_NAME   MMCAnCE_SWRESA                                   
//     %%TITLE        name                upper    lower        init        access        support        callback                                                                                             
//     %%BIT          SWRST               31       31           0           R|W           TRUE           W
//     ////////////////////////////////////////////////////////////////////////////////
/// @file mmc44_regif.h
/// @brief Register IF class of model MMC44
/// $Id: mmc44_regif.h,v 1.3 2020/04/15 04:37:36 anhnguyen4 Exp $
/// $Date: 2020/04/15 04:37:36 $
/// $Revison$
/// $Author: anhnguyen4 $
////////////////////////////////////////////////////////////////////////////////
#ifndef __MMC44_REGIF_H__
#define __MMC44_REGIF_H__
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

/// Register IF class of MMC44 model
class Cmmc44_regif
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
    Cmmc44_regif(std::string name, uint buswidth);
    ~Cmmc44_regif();

protected:
    vpcl::re_register *MMCAnCE_CMD_SET   ;
    vpcl::re_register *MMCAnCE_ARG       ;
    vpcl::re_register *MMCAnCE_ARG_CMD12 ;
    vpcl::re_register *MMCAnCE_CMD_CTRL  ;
    vpcl::re_register *MMCAnCE_BLOCK_SET ;
    vpcl::re_register *MMCAnCE_CLK_CTRL  ;
    vpcl::re_register *MMCAnCE_BUF_ACC   ;
    vpcl::re_register *MMCAnCE_RESP3     ;
    vpcl::re_register *MMCAnCE_RESP2     ;
    vpcl::re_register *MMCAnCE_RESP1     ;
    vpcl::re_register *MMCAnCE_RESP0     ;
    vpcl::re_register *MMCAnCE_RESP_CMD12;
    vpcl::re_register *MMCAnCE_DATA      ;
    vpcl::re_register *MMCAnCE_BOOT      ;
    vpcl::re_register *MMCAnCE_INT       ;
    vpcl::re_register *MMCAnCE_INT_EN    ;
    vpcl::re_register *MMCAnCE_HOST_STS1 ;
    vpcl::re_register *MMCAnCE_HOST_STS2 ;
    vpcl::re_register *MMCAnCE_SWRESA    ;

    #ifdef USR_CWR_SYSTEMC
    scml2::memory<uint> cwmem;
    scml2::reg<uint> *CW_MMCAnCE_CMD_SET   ;
    scml2::reg<uint> *CW_MMCAnCE_ARG       ;
    scml2::reg<uint> *CW_MMCAnCE_ARG_CMD12 ;
    scml2::reg<uint> *CW_MMCAnCE_CMD_CTRL  ;
    scml2::reg<uint> *CW_MMCAnCE_BLOCK_SET ;
    scml2::reg<uint> *CW_MMCAnCE_CLK_CTRL  ;
    scml2::reg<uint> *CW_MMCAnCE_BUF_ACC   ;
    scml2::reg<uint> *CW_MMCAnCE_RESP3     ;
    scml2::reg<uint> *CW_MMCAnCE_RESP2     ;
    scml2::reg<uint> *CW_MMCAnCE_RESP1     ;
    scml2::reg<uint> *CW_MMCAnCE_RESP0     ;
    scml2::reg<uint> *CW_MMCAnCE_RESP_CMD12;
    scml2::reg<uint> *CW_MMCAnCE_DATA      ;
    scml2::reg<uint> *CW_MMCAnCE_BOOT      ;
    scml2::reg<uint> *CW_MMCAnCE_INT       ;
    scml2::reg<uint> *CW_MMCAnCE_INT_EN    ;
    scml2::reg<uint> *CW_MMCAnCE_HOST_STS1 ;
    scml2::reg<uint> *CW_MMCAnCE_HOST_STS2 ;
    scml2::reg<uint> *CW_MMCAnCE_SWRESA    ;
    #endif

    uint MMCAnCE_CMD_SET_BOOT;
    uint MMCAnCE_CMD_SET_CMD;
    uint MMCAnCE_CMD_SET_RTYP;
    uint MMCAnCE_CMD_SET_RBSY;
    uint MMCAnCE_CMD_SET_WDAT;
    uint MMCAnCE_CMD_SET_DWEN;
    uint MMCAnCE_CMD_SET_CMLTE;
    uint MMCAnCE_CMD_SET_CMD12EN;
    uint MMCAnCE_CMD_SET_RIDXC;
    uint MMCAnCE_CMD_SET_RCRC7C;
    uint MMCAnCE_CMD_SET_CRC16C;
    uint MMCAnCE_CMD_SET_BOOTACK;
    uint MMCAnCE_CMD_SET_CRCSTE;
    uint MMCAnCE_CMD_SET_TBIT;
    uint MMCAnCE_CMD_SET_OPDM;
    uint MMCAnCE_CMD_SET_SBIT;
    uint MMCAnCE_CMD_SET_DATW;
    uint MMCAnCE_ARG_ARG   ;
    uint MMCAnCE_ARG_CMD12_C12ARG;
    uint MMCAnCE_CMD_CTRL_BREAK;
    uint MMCAnCE_BLOCK_SET_BLKCNT;
    uint MMCAnCE_BLOCK_SET_BLKSIZ;
    uint MMCAnCE_CLK_CTRL_MMCBUSBSY;
    uint MMCAnCE_CLK_CTRL_CLKEN;
    uint MMCAnCE_CLK_CTRL_CLKDIV;
    uint MMCAnCE_CLK_CTRL_SRSPTO;
    uint MMCAnCE_CLK_CTRL_SRBSYTO;
    uint MMCAnCE_CLK_CTRL_SRWDTO;
    uint MMCAnCE_BUF_ACC_DMATYP;
    uint MMCAnCE_BUF_ACC_DMAWEN;
    uint MMCAnCE_BUF_ACC_DMAREN;
    uint MMCAnCE_BUF_ACC_ATYP;
    uint MMCAnCE_RESP3_RSP ;
    uint MMCAnCE_RESP2_RSP ;
    uint MMCAnCE_RESP1_RSP ;
    uint MMCAnCE_RESP0_RSP ;
    uint MMCAnCE_RESP_CMD12_RSP12;
    uint MMCAnCE_DATA_DATA ;
    uint MMCAnCE_BOOT_BTCLKDIV;
    uint MMCAnCE_BOOT_SBTACKTO;
    uint MMCAnCE_BOOT_S1STBTDATTO;
    uint MMCAnCE_BOOT_SBTDATTO;
    uint MMCAnCE_INT_CMD12DRE;
    uint MMCAnCE_INT_CMD12RBE;
    uint MMCAnCE_INT_CMD12CRE;
    uint MMCAnCE_INT_DTRANE;
    uint MMCAnCE_INT_BUFRE ;
    uint MMCAnCE_INT_BUFWEN;
    uint MMCAnCE_INT_BUFREN;
    uint MMCAnCE_INT_RBSYE ;
    uint MMCAnCE_INT_CRSPE ;
    uint MMCAnCE_INT_CMDVIO;
    uint MMCAnCE_INT_BUFVIO;
    uint MMCAnCE_INT_WDATERR;
    uint MMCAnCE_INT_RDATERR;
    uint MMCAnCE_INT_RIDXERR;
    uint MMCAnCE_INT_RSPERR;
    uint MMCAnCE_INT_CRCSTO;
    uint MMCAnCE_INT_WDATTO;
    uint MMCAnCE_INT_RDATTO;
    uint MMCAnCE_INT_RBSYTO;
    uint MMCAnCE_INT_RSPTO ;
    uint MMCAnCE_INT_EN_MCMD12DRE;
    uint MMCAnCE_INT_EN_MCMD12RBE;
    uint MMCAnCE_INT_EN_MCMD12CRE;
    uint MMCAnCE_INT_EN_MDTRANE;
    uint MMCAnCE_INT_EN_MBUFRE;
    uint MMCAnCE_INT_EN_MBUFWEN;
    uint MMCAnCE_INT_EN_MBUFREN;
    uint MMCAnCE_INT_EN_MRBSYE;
    uint MMCAnCE_INT_EN_MCRSPE;
    uint MMCAnCE_INT_EN_MCMDVIO;
    uint MMCAnCE_INT_EN_MBUFVIO;
    uint MMCAnCE_INT_EN_MWDATERR;
    uint MMCAnCE_INT_EN_MRDATERR;
    uint MMCAnCE_INT_EN_MRIDXERR;
    uint MMCAnCE_INT_EN_MRSPERR;
    uint MMCAnCE_INT_EN_MCRCSTO;
    uint MMCAnCE_INT_EN_MWDATTO;
    uint MMCAnCE_INT_EN_MRDATTO;
    uint MMCAnCE_INT_EN_MRBSYTO;
    uint MMCAnCE_INT_EN_MRSPTO;
    uint MMCAnCE_HOST_STS1_CMDSEQ;
    uint MMCAnCE_HOST_STS1_CMDSIG;
    uint MMCAnCE_HOST_STS1_RSPIDX;
    uint MMCAnCE_HOST_STS1_DATSIG;
    uint MMCAnCE_HOST_STS1_RCVBLK;
    uint MMCAnCE_HOST_STS2_CRCSTE;
    uint MMCAnCE_HOST_STS2_CRC16E;
    uint MMCAnCE_HOST_STS2_AC12CRCE;
    uint MMCAnCE_HOST_STS2_RSPCRC7E;
    uint MMCAnCE_HOST_STS2_CRCSTEBE;
    uint MMCAnCE_HOST_STS2_RDATEBE;
    uint MMCAnCE_HOST_STS2_AC12REBE;
    uint MMCAnCE_HOST_STS2_RSPEBE;
    uint MMCAnCE_HOST_STS2_AC12IDXE;
    uint MMCAnCE_HOST_STS2_RSPIDXE;
    uint MMCAnCE_HOST_STS2_BTACKPATE;
    uint MMCAnCE_HOST_STS2_BTACKEBE;
    uint MMCAnCE_HOST_STS2_CRCST;
    uint MMCAnCE_HOST_STS2_STRDATTO;
    uint MMCAnCE_HOST_STS2_DATBSYTO;
    uint MMCAnCE_HOST_STS2_CRCSTTO;
    uint MMCAnCE_HOST_STS2_AC12BSYTO;
    uint MMCAnCE_HOST_STS2_RSPBSYTO;
    uint MMCAnCE_HOST_STS2_AC12RSPTO;
    uint MMCAnCE_HOST_STS2_STRSPTO;
    uint MMCAnCE_HOST_STS2_BTACKTO;
    uint MMCAnCE_HOST_STS2_1STBTDATTO;
    uint MMCAnCE_HOST_STS2_BTDATTO;
    uint MMCAnCE_SWRESA_SWRST;

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

    virtual void cb_MMCAnCE_CMD_SET_BOOT(RegCBstr str) = 0;
    virtual void cb_MMCAnCE_CMD_SET_DATW(RegCBstr str) = 0;
    virtual void cb_MMCAnCE_ARG_ARG(RegCBstr str) = 0;
    virtual void cb_MMCAnCE_ARG_CMD12_C12ARG(RegCBstr str) = 0;
    virtual void cb_MMCAnCE_CMD_CTRL_BREAK(RegCBstr str) = 0;
    virtual void cb_MMCAnCE_CLK_CTRL_CLKEN(RegCBstr str) = 0;
    virtual void cb_MMCAnCE_BUF_ACC_DMATYP(RegCBstr str) = 0;
    virtual void cb_MMCAnCE_DATA_DATA(RegCBstr str) = 0;
    virtual void cb_MMCAnCE_BOOT_BTCLKDIV(RegCBstr str) = 0;
    virtual void cb_MMCAnCE_INT_CMD12DRE(RegCBstr str) = 0;
    virtual void cb_MMCAnCE_INT_EN_MBUFWEN(RegCBstr str) = 0;
    virtual void cb_MMCAnCE_SWRESA_SWRST(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Cmmc44_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Cmmc44_regif::*) (RegCBstr)> > mRdCbAPI;
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
#endif //__MMC44_REG_DEF_H__
