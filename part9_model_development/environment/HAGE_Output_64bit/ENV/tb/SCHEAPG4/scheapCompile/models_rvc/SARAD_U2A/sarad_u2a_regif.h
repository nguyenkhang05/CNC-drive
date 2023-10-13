// -----------------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2017-2019 Renesas Electronics Corporation
// Copyright(c) 2017-2019 Renesas Design Vietnam Co., Ltd.
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
// Input file : sarad_u2a_info.txt
////////////////////////////////////////////////////////////////////////////////
// # -----------------------------------------------------------------------------
// # $Id$
// #
// # Copyright(c) 2012-2019 Renesas System Design Co., Ltd.
// # Copyright(c) 2012-2019 Renesas Design Vietnam Co., Ltd.
// # RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// # This program must be used solely for the purpose for which
// # it was furnished by Renesas Electronics Corporation. No part of this
// # program may be reproduced or disclosed to others, in any
// # form, without the prior written permission of Renesas Electronics
// # Corporation.
// # -----------------------------------------------------------------------------
// 
// ####################################################
// ## For PORTs
// ####################################################
// %MODULE sarad_u2a
// %HEADER_FILES sarad_u2a.h
// %IMPLEMENTATION_FILES sarad_u2a.cpp sarad_u2a_regif.cpp adc_u2a.cpp adc_u2a_regif.cpp
// %PORT  pclk                          64        in        sc_dt::uint64
// %PORT  ADCLK                         64        in        sc_dt::uint64
// %PORT  CLKA_ADC                      64        in        sc_dt::uint64
// %PORT  preset_n                      1         in        bool
// %PORT  RESETAD_N                     1         in        bool
// %PORT  preset2_n                     1         in        bool
// %PORT  RESETAD2_N                    1         in        bool
// %PORT  AVREFH0                       64        in        double
// %PORT  AVREFH1                       64        in        double
// %PORT  AVREFH2                       64        in        double
// %PORT  AD0SG0TRG                     1         in        bool
// %PORT  AD0SG1TRG                     1         in        bool
// %PORT  AD0SG2TRG                     1         in        bool
// %PORT  AD0SG3TRG                     1         in        bool
// %PORT  AD0SG4TRG                     1         in        bool
// %PORT  AD1SG0TRG                     1         in        bool
// %PORT  AD1SG1TRG                     1         in        bool
// %PORT  AD1SG2TRG                     1         in        bool
// %PORT  AD1SG3TRG                     1         in        bool
// %PORT  AD1SG4TRG                     1         in        bool
// %PORT  AD2SG0TRG                     1         in        bool
// %PORT  AD2SG1TRG                     1         in        bool
// %PORT  AD2SG2TRG                     1         in        bool
// %PORT  AD2SG3TRG                     1         in        bool
// %PORT  AD2SG4TRG                     1         in        bool
// %PORT  AD2TAUD2I7                    1         in        bool
// %PORT  AD2TAUD2I15                   1         in        bool
// %PORT  AD2TAUJ2                      1         in        bool
// %PORT  AD2TAUJ3                      1         in        bool
// %PORT  AD2LPS                        1         in        bool
// #%PORT  ADSTART_I                     1         in        bool
// #%PORT  ADSTART_O                     1         out       bool
// #%PORT  ADTSTART_I                    1         in        bool
// #%PORT  ADTSTART_O                    1         out       bool
// %PORT  AN000                         64        in        double
// %PORT  AN001                         64        in        double
// %PORT  AN002                         64        in        double
// %PORT  AN003                         64        in        double
// %PORT  AN010                         64        in        double
// %PORT  AN011                         64        in        double
// %PORT  AN012                         64        in        double
// %PORT  AN013                         64        in        double
// %PORT  AN020                         64        in        double
// %PORT  AN021                         64        in        double
// %PORT  AN022                         64        in        double
// %PORT  AN023                         64        in        double
// %PORT  AN030                         64        in        double
// %PORT  AN031                         64        in        double
// %PORT  AN032                         64        in        double
// %PORT  AN033                         64        in        double
// %PORT  AN040                         64        in        double
// %PORT  AN041                         64        in        double
// %PORT  AN042                         64        in        double
// %PORT  AN043                         64        in        double
// %PORT  VMON_VCC                      64        in        double
// %PORT  VMON_E0VCC                    64        in        double
// %PORT  VMON_ISOVDD                   64        in        double
// %PORT  VMON_AWOVDD                   64        in        double
// %PORT  AN050                         64        in        double
// %PORT  AN051                         64        in        double
// %PORT  AN052                         64        in        double
// %PORT  AN053                         64        in        double
// %PORT  AN060                         64        in        double
// %PORT  AN061                         64        in        double
// %PORT  AN062                         64        in        double
// %PORT  AN063                         64        in        double
// %PORT  AN070                         64        in        double
// %PORT  AN071                         64        in        double
// %PORT  AN100                         64        in        double
// %PORT  AN101                         64        in        double
// %PORT  AN102                         64        in        double
// %PORT  AN103                         64        in        double
// %PORT  AN110                         64        in        double
// %PORT  AN111                         64        in        double
// %PORT  AN112                         64        in        double
// %PORT  AN113                         64        in        double
// %PORT  AN120                         64        in        double
// %PORT  AN121                         64        in        double
// %PORT  AN122                         64        in        double
// %PORT  AN123                         64        in        double
// %PORT  AN130                         64        in        double
// %PORT  AN131                         64        in        double
// %PORT  AN132                         64        in        double
// %PORT  AN133                         64        in        double
// %PORT  AN140                         64        in        double
// %PORT  AN141                         64        in        double
// %PORT  AN142                         64        in        double
// %PORT  AN143                         64        in        double
// %PORT  AN150                         64        in        double
// %PORT  AN151                         64        in        double
// %PORT  AN152                         64        in        double
// %PORT  AN153                         64        in        double
// %PORT  AN160                         64        in        double
// %PORT  AN161                         64        in        double
// %PORT  AN162                         64        in        double
// %PORT  AN163                         64        in        double
// %PORT  AN170                         64        in        double
// %PORT  AN171                         64        in        double
// %PORT  AN172                         64        in        double
// %PORT  AN173                         64        in        double
// %PORT  AN180                         64        in        double
// %PORT  AN181                         64        in        double
// %PORT  AN200                         64        in        double
// %PORT  AN201                         64        in        double
// %PORT  AN202                         64        in        double
// %PORT  AN203                         64        in        double
// %PORT  AN210                         64        in        double
// %PORT  AN211                         64        in        double
// %PORT  AN212                         64        in        double
// %PORT  AN213                         64        in        double
// %PORT  AN220                         64        in        double
// %PORT  AN221                         64        in        double
// %PORT  AN222                         64        in        double
// %PORT  AN223                         64        in        double
// %PORT  AN230                         64        in        double
// %PORT  AN231                         64        in        double
// %PORT  AN232                         64        in        double
// %PORT  AN233                         64        in        double
// %PORT  AN240                         64        in        double
// %PORT  AN241                         64        in        double
// %PORT  AN242                         64        in        double
// %PORT  AN243                         64        in        double
// %PORT  AN250                         64        in        double
// %PORT  AN251                         64        in        double
// %PORT  AN252                         64        in        double
// %PORT  AN253                         64        in        double
// %PORT  AN260                         64        in        double
// %PORT  AN261                         64        in        double
// %PORT  AN262                         64        in        double
// %PORT  AN263                         64        in        double
// %PORT  AN270                         64        in        double
// %PORT  AN271                         64        in        double
// %PORT  IFVCH0                        7         in        sc_uint
// %PORT  IFVCH1                        7         in        sc_uint
// %PORT  IFVCH2                        7         in        sc_uint
// %PORT  IFRD0                         1         in        bool
// %PORT  IFRD1                         1         in        bool
// %PORT  IFRD2                         1         in        bool
// %PORT  IFRE0                         1         in        bool
// %PORT  IFRE1                         1         in        bool
// %PORT  IFRE2                         1         in        bool
// %PORT  INT_AD00                      1         out       bool
// %PORT  INT_AD01                      1         out       bool
// %PORT  INT_AD02                      1         out       bool
// %PORT  INT_AD03                      1         out       bool
// %PORT  INT_AD04                      1         out       bool
// %PORT  INT_AD10                      1         out       bool
// %PORT  INT_AD11                      1         out       bool
// %PORT  INT_AD12                      1         out       bool
// %PORT  INT_AD13                      1         out       bool
// %PORT  INT_AD14                      1         out       bool
// %PORT  INT_AD20                      1         out       bool
// %PORT  INT_AD21                      1         out       bool
// %PORT  INT_AD22                      1         out       bool
// %PORT  INT_AD23                      1         out       bool
// %PORT  INT_AD24                      1         out       bool
// %PORT  INT_MPX0                      1         out       bool
// %PORT  INT_MPX1                      1         out       bool
// %PORT  INT_MPX2                      1         out       bool
// ##%PORT  INT_ADE0                      1         out       bool
// ##%PORT  INT_ADE1                      1         out       bool
// %PORT  ADPE0                         1         out       bool
// %PORT  ADPE1                         1         out       bool
// %PORT  ADPE2                         1         out       bool
// %PORT  ADEND00                       1         out       bool
// %PORT  ADEND01                       1         out       bool
// %PORT  ADEND02                       1         out       bool
// %PORT  ADEND03                       1         out       bool
// %PORT  ADEND04                       1         out       bool
// %PORT  ADEND10                       1         out       bool
// %PORT  ADEND11                       1         out       bool
// %PORT  ADEND12                       1         out       bool
// %PORT  ADEND13                       1         out       bool
// %PORT  ADEND14                       1         out       bool
// %PORT  ADEND20                       1         out       bool
// %PORT  ADEND21                       1         out       bool
// %PORT  ADEND22                       1         out       bool
// %PORT  ADEND23                       1         out       bool
// %PORT  ADEND24                       1         out       bool
// %PORT  ULE_LPS0                      8         out       uint8_t
// %PORT  ULE_LPS1                      8         out       uint8_t
// %PORT  ULE_LPS2                      8         out       uint8_t
// %PORT  INT_ERR0                      1         out       bool
// %PORT  INT_ERR1                      1         out       bool
// %PORT  INT_ERR2                      1         out       bool
// #%PORT  INT_UL0                       1         out       bool
// #%PORT  INT_UL1                       1         out       bool
// #%PORT  INT_SD0                       1         out       bool
// #%PORT  INT_SD1                       1         out       bool
// %PORT  ADC0_IFDATA                   16        out       sc_uint
// %PORT  ADC1_IFDATA                   16        out       sc_uint
// %PORT  ADC2_IFDATA                   16        out       sc_uint
// %PORT  DFREQ0                        1         out       bool
// %PORT  DFREQ1                        1         out       bool
// %PORT  DFREQ2                        1         out       bool
// #%PORT  ASREQ0                        1         out       bool
// #%PORT  ASREQ1                        1         out       bool
// %PORT  DFTAG0                        4         out       sc_uint
// %PORT  DFTAG1                        4         out       sc_uint
// %PORT  DFTAG2                        4         out       sc_uint
// %PORT  DFDATA0                       16        out       sc_uint
// %PORT  DFDATA1                       16        out       sc_uint
// %PORT  DFDATA2                       16        out       sc_uint
// %PORT  PVCR_VALUE0                   32        in        uint32_t
// %PORT  PVCR_VALUE1                   32        in        uint32_t
// %PORT  PVCR_VALUE2                   32        in        uint32_t
// %PORT  PVCR_TRG0                     1         in        bool
// %PORT  PVCR_TRG1                     1         in        bool
// %PORT  PVCR_TRG2                     1         in        bool
// %PORT  PVCR_END0                     1         in        bool
// %PORT  PVCR_END1                     1         in        bool
// %PORT  PVCR_END2                     1         in        bool
// %PORT  PVCR_PWDDR0                   16        in        uint16_t
// %PORT  PVCR_PWDDR1                   16        in        uint16_t
// %PORT  PVCR_PWDDR2                   16        in        uint16_t
// %PORT  *VCEND0[96]                   1         out       bool
// %PORT  PWM_VCEND0                    1         out       bool
// %PORT  VCULMO0                       1         out       bool
// %PORT  VCLLMO0                       1         out       bool
// %PORT  *VCEND1[96]                   1         out       bool
// %PORT  PWM_VCEND1                    1         out       bool
// %PORT  VCULMO1                       1         out       bool
// %PORT  VCLLMO1                       1         out       bool
// %PORT  *VCEND2[96]                   1         out       bool
// %PORT  PWM_VCEND2                    1         out       bool
// %PORT  VCULMO2                       1         out       bool
// %PORT  VCLLMO2                       1         out       bool
// %PORT  MPXCUR0                       3         out       sc_uint
// %PORT  MPXCUR1                       3         out       sc_uint
// %PORT  MPXCUR2                       3         out       sc_uint
// %PORT  SV_MODE                       1         in        bool
// 
// ####################################################
// ## For Registers
// ####################################################
// %MODULE sarad_u2a
//     #              name     offset_size  
//     %%REG_INSTANCE reg_def  11           
// 
// %REG_CHANNEL   reg_def
//      %%TITLE    name        reg_name     wsize     rsize     length   offset   factor_start   factor_end    factor_index    factor_step  init        access  support  callback
//      %%REG      ADSYNSTCR   ADSYNSTCR    8|16|32   8|16|32   8        0x610    -              -             -               -            0           W|R     TRUE     -
//      %%REG      ADTSYNSTCR  ADTSYNSTCR   8|16|32   8|16|32   8        0x614    -              -             -               -            0           W|R     TRUE     - 
//      %%REG      VMONVDCR1   VMONVDCR1    8|16|32   8|16|32   8        0x740    -              -             -               -            0           W|R     TRUE     -
//      %%REG      VMONVDCR2   VMONVDCR2    8|16|32   8|16|32   8        0x744    -              -             -               -            0           W|R     TRUE     - 
// 
// %REG_NAME  ADSYNSTCR 
//    %%TITLE     name       upper   lower     init    access      support     callback    
//    %%BIT       ADSTART    0        0        0       W|R         TRUE        W
// 
// %REG_NAME  ADTSYNSTCR
//    %%TITLE     name       upper   lower     init    access      support     callback    
//    %%BIT       ADTSTART   0        0        0       W|R         TRUE        W
// 
// %REG_NAME  VMONVDCR1
//    %%TITLE     name       upper   lower     init    access      support     callback  
//    %%BIT       VDE1       0       0         0       W|R         TRUE        W
// 
// %REG_NAME  VMONVDCR2
//    %%TITLE     name       upper   lower     init    access      support     callback 
//    %%BIT       VDE2       0       0         0       W|R         TRUE        W
// 
// ####################################################
// ## For command I/F
// ####################################################
// %CMD_BASEID  tgt reg
// %CMD_NODUMP_API EnableInsertInput EnableDumpResult DumpProfile ClearProfile AssertReset
// %CMD_RW DumpInterrupt      <enable: bool=false>             @ Display interrupt information when interrupt get set[ns]
// %CMD_RW EnableConvertInfo  <enable: bool=false>             @ Display AD convert activity
// 
// %CMD_RW Avrefh0_Val        <Avrefh0: double=3.3>            @ Reference voltage of AVREFH0
// %CMD_RW Avrefh1_Val        <Avrefh1: double=3.3>            @ Reference voltage of AVREFH1
// %CMD_RW Avrefh2_Val        <Avrefh2: double=3.3>            @ Reference voltage of AVREFH2
// %CMD_RW AVcc0              <AVcc0: double=3.3>              @ Reference voltage of AVCC0
// %CMD_RW AVcc1              <AVcc1: double=3.3>              @ Reference voltage of AVCC1
// %CMD_RW AVcc2              <AVcc2: double=3.3>              @ Reference voltage of AVCC2
// %CMD_RW EVcc0              <EVcc0: double=3.3>              @ Reference voltage of EVCC0
// %CMD_RW EVcc1              <EVcc1: double=3.3>              @ Reference voltage of EVCC1
// %CMD_RW EVcc2              <EVcc2: double=3.3>              @ Reference voltage of EVCC2
// 
// %CMD_RW tTSPL              <tTSPL: double=27>               @ Number of sampling cycles in T&H circuit (AD clock)
// %CMD_RW tTHSD              <tTHSD: double=5>                @ Delay time from detecting a hold trigger until starting to hold an analog input (AD clock)
// %CMD_RW tTHLD              <tTHLD: double=10>               @ Delay time from starting to hold an analog input until starting AD conversion (AD clock)
// %CMD_RW ExSmpt             <ExSmpt: unsigned int=60>        @ Default number of extended sampling cycles in AD conversion (AD clock)
// %CMD_RW Smpt               <Smpt: unsigned int=18>          @ Default number of sampling cycles in AD conversion (AD clock)
// %CMD_RW tSAR               <tSAR: double=0>                 @ Successive approximation AD convert cycle (AD clock)
// %CMD_RW tD                 <tD: double=0>                   @ Delay time until starting scan group (APB clock)
// %CMD_RW tED                <tED: double=0>                  @ Delay time until complete scan group (APB clock)
// 
// %CMD_RW ForcedIdError0     <enable: bool=false>             @ The ID error occurs forcibly into ADCA0 when writting DRn
// %CMD_RW ForcedIdError1     <enable: bool=false>             @ The ID error occurs forcibly into ADCA1 when writting DRn
// %CMD_RW ForcedIdError2     <enable: bool=false>             @ The ID error occurs forcibly into ADCA2 when writting DRn
// %CMD_RW ForcedParityError0 <enable: bool=false>             @ The parity error occurs forcibly into ADCA0 when reading DRn or DIRn
// %CMD_RW ForcedParityError1 <enable: bool=false>             @ The parity error occurs forcibly into ADCA1 when reading DRn or DIRn
// %CMD_RW ForcedParityError2 <enable: bool=false>             @ The parity error occurs forcibly into ADCA2 when reading DRn or DIRn
// %CMD_RW ForcedOpenError0   <enable: bool=false>             @ The open circuit error occurs forcibly into ADCA0 when VCRn.PUE or PDE is 1
// %CMD_RW ForcedOpenError1   <enable: bool=false>             @ The open circuit error occurs forcibly into ADCA1 when VCRn.PUE or PDE is 1
// %CMD_RW ForcedOpenError2   <enable: bool=false>             @ The open circuit error occurs forcibly into ADCA2 when VCRn.PUE or PDE is 1
// %CMD_RW ForcedBreak0       <enable: bool=false>             @ The break circuit error occurs forcibly into ADCA0 when ODCR.ODDE = 1
// %CMD_RW ForcedBreak1       <enable: bool=false>             @ The break circuit error occurs forcibly into ADCA1 when ODCR.ODDE = 1
// %CMD_RW ForcedBreak2       <enable: bool=false>             @ The break circuit error occurs forcibly into ADCA2 when ODCR.ODDE = 1
// %CMD_RW ForcedDataPathError0       <enable: bool=false>             @ The data path error occurs forcibly into ADCA0
// %CMD_RW ForcedDataPathError1       <enable: bool=false>             @ The data path error occurs forcibly into ADCA1
// %CMD_RW ForcedDataPathError2       <enable: bool=false>             @ The data path error occurs forcibly into ADCA2
// 
// #%CMD_RW StartAddrADCA0     <addr: unsigned int=0x1000>      @ The start address of ADCA0 module
// #%CMD_RW StartAddrADCA1     <addr: unsigned int=0x2000>      @ The start address of ADCA1 module
// #%CMD_RW StartAddrADCA2     <addr: unsigned int=0x3000>      @ The start address of ADCA2 module
// #%CMD_RW EndAddressADCA0    <addr: unsigned int=0x1FFF>      @ The end address of ADCA0 module
// #%CMD_RW EndAddressADCA1    <addr: unsigned int=0x2FFF>      @ The end address of ADCA1 module
// #%CMD_RW EndAddressADCA2    <addr: unsigned int=0x3FFF>      @ The end address of ADCA2 module
// #%CMD_RW OffsetAddMask      <addr: unsigned int=0x7FFF>      @ The offset address mask
// 
// %CMD_RW EnableTimeCalculation   <enable: bool=true>         @ Enable using formula to calculate the delay times
// 
// %CMD_RW AVccMax            <AVccMax: double=3.6>            @ The maximum value of AVCCm
// %CMD_RW AVccMin            <AVccMin: double=3>              @ The minimum value of AVCCm
// %CMD_RW EVccMax            <EVccMax: double=3.6>            @ The maximum value of EVCCm
// %CMD_RW EVccMin            <EVccMin: double=3>              @ The minimum value of EVCCm
// %CMD_RW AvrefhMin          <AvrefhMin: double=3>            @ The minimum value of Avrefhm
// 
// %CMD_ACTION DumpStatInfo : void @ Dump the statistical information about SAR model activity
// %CMD_ACTION SetCLKfreq <clk_name: std::string> <clk_freq: double>: void @ Setup the new clock frequency
// %CMD_ACTION AssertPReset <start_time: double> <period: double>: void @ Assert preset_n for ADC0 and ADC2
// %CMD_ACTION AssertADReset <start_time: double> <period: double>: void @ Assert RESETAD_N for ADC0 and ADC2
// %CMD_ACTION AssertPReset2 <start_time: double> <period: double>: void @ Assert preset_n for ADC2
// %CMD_ACTION AssertADReset2 <start_time: double> <period: double>: void @ Assert RESETAD_N for ADC2
// %CMD_ACTION AssignTHCircuit <adc_index: unsigned int> <th_channel: unsigned int> <group_channel: unsigned int> <physic_channel: unsigned int>: void @ Assign the T&H channel to corresponding analog channel
// 
////////////////////////////////////////////////////////////////////////////////
/// @file sarad_u2a_regif.h
/// @brief Register IF class of model SARAD_U2A
/// $Id$
/// $Date$
/// $Revison$
/// $Author$
////////////////////////////////////////////////////////////////////////////////
#ifndef __SARAD_U2A_REGIF_H__
#define __SARAD_U2A_REGIF_H__
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

/// Register IF class of SARAD_U2A model
class Csarad_u2a_regif
: public vpcl::reg_super
{
protected:
    typedef const unsigned int cuint;
    typedef unsigned int uint;
#ifdef USE_CWR_SYSTEMC
    typedef unsigned char REG_TYPE;
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

    typedef void (Csarad_u2a_regif::* ptrRegValueIDFunc) (std::string);
    ptrRegValueIDFunc mNotifyRegValueIDAPI;
    virtual void NotifyRegValueID(const std::string reg_val_id) = 0;
#endif

public:
    Csarad_u2a_regif(std::string name, uint buswidth);
    ~Csarad_u2a_regif();

protected:
    vpcl::re_register *ADSYNSTCR ;
    vpcl::re_register *ADTSYNSTCR;
    vpcl::re_register *VMONVDCR1 ;
    vpcl::re_register *VMONVDCR2 ;

    #ifdef USE_CWR_SYSTEMC
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

    virtual void cb_ADSYNSTCR_ADSTART(RegCBstr str) = 0;
    virtual void cb_ADTSYNSTCR_ADTSTART(RegCBstr str) = 0;
    virtual void cb_VMONVDCR1_VDE1(RegCBstr str) = 0;
    virtual void cb_VMONVDCR2_VDE2(RegCBstr str) = 0;

    void set_instance_name(std::string InstName);

    uint mBusByteWidth;
    uint mBusWidth;
    bool mIsReset;
    bool mDumpRegisterRW;
    bool mAPBAccessMode;
    bool mDumpBitInfo;
    std::map<std::string, bool> mMessageLevel;
    std::map<std::string, std::map<std::string, void (Csarad_u2a_regif::*) (RegCBstr)> > mWrCbAPI;
    std::map<std::string, std::map<std::string, void (Csarad_u2a_regif::*) (RegCBstr)> > mRdCbAPI;
    std::map<std::string, std::map<std::string, void (Csarad_u2a_regif::*) (RegCBstr)> > mWrDbgCbAPI;
    std::map<std::string, std::map<std::string, void (Csarad_u2a_regif::*) (RegCBstr)> > mRdDbgCbAPI;
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

    #ifdef USE_CWR_SYSTEMC
    unsigned char cw_rd_cb(tlm::tlm_generic_payload& trans, int tag);
    unsigned char cw_wr_cb(tlm::tlm_generic_payload& trans, int tag);
    virtual void cw_set_callback_reg(scml2::reg<REG_TYPE> * reg, int offset);
    void cw_set_all_callback_reg(void);
    #endif

private:
    void _re_printf(const std::string msg_level, const char *format, ...);
};
#endif //__SARAD_U2A_REG_DEF_H__
