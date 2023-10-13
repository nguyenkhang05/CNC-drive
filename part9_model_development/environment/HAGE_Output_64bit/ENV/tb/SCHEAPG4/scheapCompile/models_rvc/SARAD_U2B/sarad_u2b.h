// -----------------------------------------------------------------------------
// $Id$
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

#ifndef __SARAD_U2B_H__
#define __SARAD_U2B_H__

#include "systemc.h"
#include "tlm.h"
#include "tlm_tgt_if.h"
#include <iterator>

#include "sarad_u2b_regif.h"
#include "adc_u2b.h"

/// SARAD_U2B model class
class Csarad_u2b: public sc_module
             ,public vpcl::tlm_tgt_if<32,tlm::tlm_base_protocol_types,5>
             ,public Csarad_u2b_regif
{
#include "sarad_u2b_cmdif.h"
private:
    //Enum
    enum eSARAD_CONSTANCE {
        emNanoSecond = 1000000000
       ,emTargetConfigNum = 1    // U2B 
       //,emADOPDIGAddr = 0x748 //Confirm
    };
    enum eSARAD_STATE{
        emRESET = 0
       ,emIDLE = 1
       ,emOPERATING = 2
       ,emDEEPSTOP = 3
    };
    // Value for attributes
    enum eAttr_value {
        emADC_NUM_Def = 5
       ,emADC_NUM_Min = 1
       ,emADC_NUM_Max = 5
       ,emP_SADC_SARCHNUM_Def = 32
       ,emP_SADC_SARCHNUM_Min = 1
       ,emP_SADC_SARCHNUM_Max = 40
       ,emVCRnum_Def = 64
       ,emVCRnum_Min = 16
       ,emVCRnum_Max = 96
       ,emEX_MPX_PCH_Def = 5
       ,emTH_CHNUM_Def = 6
       ,emTH_CHNUM_Min = 0
       ,emTH_CHNUM_Max = 6
    };
    enum eSARAD_RESET{
        emP_RESET = 0
       ,emAD_RESET = 1
       //,emALL_RESET = 2
       ,emP_RESETA = 2
       ,emAD_RESETA = 3
    };

    enum eSARAD_SOCKET_ID {
         emADC0Id = 0
        ,emADC1Id = 1
        ,emADC2Id = 2
        ,emADC3Id = 3
        ,emADCAId = 4
        ,emMaxId = 5
    };


public:

    SC_HAS_PROCESS(Csarad_u2b);
    Cadc_u2b *mADC0;
    Cadc_u2b *mADC1;
    Cadc_u2b *mADC2;
    Cadc_u2b *mADC3;
    Cadc_u2b *mADCA;
    Csarad_u2b(sc_module_name name);
    ~Csarad_u2b();

    //Port declaration
    sc_in < sc_dt::uint64 > pclk;
    sc_in < sc_dt::uint64 > ADCLK;
    sc_in < sc_dt::uint64 > CLKA_ADC;
    sc_in < bool > preset_n;
    sc_in < bool > RESETAD_N;
    sc_in < bool > preseta_n;
    sc_in < bool > RESETADA_N;
    sc_in < double > AVREFH0;
    sc_in < double > AVREFH1;
    sc_in < double > AVREFH2;
    sc_in < double > AVREFH3;

    sc_in < bool > AD0SG0TRG;
    sc_in < bool > AD0SG1TRG;
    sc_in < bool > AD0SG2TRG;
    sc_in < bool > AD0SG3TRG;
    sc_in < bool > AD0SG4TRG;

    sc_in < bool > AD1SG0TRG;
    sc_in < bool > AD1SG1TRG;
    sc_in < bool > AD1SG2TRG;
    sc_in < bool > AD1SG3TRG;
    sc_in < bool > AD1SG4TRG;

    sc_in < bool > AD2SG0TRG;
    sc_in < bool > AD2SG1TRG;
    sc_in < bool > AD2SG2TRG;
    sc_in < bool > AD2SG3TRG;
    sc_in < bool > AD2SG4TRG;

    sc_in < bool > AD3SG0TRG;
    sc_in < bool > AD3SG1TRG;
    sc_in < bool > AD3SG2TRG;
    sc_in < bool > AD3SG3TRG;
    sc_in < bool > AD3SG4TRG;

    sc_in < bool > ADASG0TRG;
    sc_in < bool > ADASG1TRG;
    sc_in < bool > ADASG2TRG;
    sc_in < bool > ADASG3TRG;
    sc_in < bool > ADASG4TRG;

    sc_in < bool > ADxTAUD2I7; 
    sc_in < bool > ADxTAUD2I15;
    sc_in < bool > ADxTAUJ2;   
    sc_in < bool > ADxTAUJ3;   
    sc_in < bool > ADxLPS;     

    sc_in < bool > AD0SGDTRG;
    sc_in < bool > AD1SGDTRG;
    sc_in < bool > AD2SGDTRG;
    sc_in < bool > AD3SGDTRG;
    sc_in < bool > ADASGDTRG;

    sc_in < double > AN000;
    sc_in < double > AN001;
    sc_in < double > AN002;
    sc_in < double > AN003;
    sc_in < double > AN010;
    sc_in < double > AN011;
    sc_in < double > AN012;
    sc_in < double > AN013;
    sc_in < double > AN020;
    sc_in < double > AN021;
    sc_in < double > AN022;
    sc_in < double > AN023;
    sc_in < double > AN030;
    sc_in < double > AN031;
    sc_in < double > AN032;
    sc_in < double > AN033;
    sc_in < double > AN040;
    sc_in < double > AN041;
    sc_in < double > AN042;
    sc_in < double > AN043;
    sc_in < double > AN050;
    sc_in < double > AN051;
    sc_in < double > AN052;
    sc_in < double > AN053;
    sc_in < double > AN060;
    sc_in < double > AN061;
    sc_in < double > AN062;
    sc_in < double > AN063;
    sc_in < double > VMON_VCC;
    sc_in < double > VMON_E0VCC;
    sc_in < double > VMON_ISOVDD;
    sc_in < double > VMON_AWOVDD;
    sc_in < double > AN100;
    sc_in < double > AN101;
    sc_in < double > AN102;
    sc_in < double > AN103;
    sc_in < double > AN110;
    sc_in < double > AN111;
    sc_in < double > AN112;
    sc_in < double > AN113;
    sc_in < double > AN120;
    sc_in < double > AN121;
    sc_in < double > AN122;
    sc_in < double > AN123;
    sc_in < double > AN130;
    sc_in < double > AN131;
    sc_in < double > AN132;
    sc_in < double > AN133;
    sc_in < double > AN140;
    sc_in < double > AN141;
    sc_in < double > AN142;
    sc_in < double > AN143;
    sc_in < double > AN150;
    sc_in < double > AN151;
    sc_in < double > AN152;
    sc_in < double > AN153;
    sc_in < double > AN160;
    sc_in < double > AN161;
    sc_in < double > AN162;
    sc_in < double > AN163;
    sc_in < double > AN200;
    sc_in < double > AN201;
    sc_in < double > AN202;
    sc_in < double > AN203;
    sc_in < double > AN210;
    sc_in < double > AN211;
    sc_in < double > AN212;
    sc_in < double > AN213;
    sc_in < double > AN220;
    sc_in < double > AN221;
    sc_in < double > AN222;
    sc_in < double > AN223;
    sc_in < double > AN230;
    sc_in < double > AN231;
    sc_in < double > AN232;
    sc_in < double > AN233;
    sc_in < double > AN240;
    sc_in < double > AN241;
    sc_in < double > AN242;
    sc_in < double > AN243;
    sc_in < double > AN250;
    sc_in < double > AN251;
    sc_in < double > AN252;
    sc_in < double > AN253;
    sc_in < double > AN260;
    sc_in < double > AN261;
    sc_in < double > AN262;
    sc_in < double > AN263;
    sc_in < double > AN270;
    sc_in < double > AN271;
    sc_in < double > AN272;
    sc_in < double > AN273;
    sc_in < double > AN300;
    sc_in < double > AN301;
    sc_in < double > AN302;
    sc_in < double > AN303;
    sc_in < double > AN310;
    sc_in < double > AN311;
    sc_in < double > AN312;
    sc_in < double > AN313;
    sc_in < double > AN320;
    sc_in < double > AN321;
    sc_in < double > AN322;
    sc_in < double > AN323;
    sc_in < double > AN330;
    sc_in < double > AN331;
    sc_in < double > AN332;
    sc_in < double > AN333;
    sc_in < double > AN340;
    sc_in < double > AN341;
    sc_in < double > AN342;
    sc_in < double > AN343;
    sc_in < double > AN350;
    sc_in < double > AN351;
    sc_in < double > AN352;
    sc_in < double > AN353;
    sc_in < double > AN360;
    sc_in < double > AN361;
    sc_in < double > AN362;
    sc_in < double > AN363;
    sc_in < double > AN370;
    sc_in < double > AN371;
    sc_in < double > AN372;
    sc_in < double > AN373;
    sc_in < double > AN380;
    sc_in < double > AN381;
    sc_in < double > AN382;
    sc_in < double > AN383;
    sc_in < double > AN390;
    sc_in < double > AN391;
    sc_in < double > AN392;
    sc_in < double > AN393;
    sc_in < double > ANA00;
    sc_in < double > ANA01;
    sc_in < double > ANA02;
    sc_in < double > ANA03;
    sc_in < double > ANA10;
    sc_in < double > ANA11;
    sc_in < double > ANA12;
    sc_in < double > ANA13;
    sc_in < double > ANA20;
    sc_in < double > ANA21;
    sc_in < double > ANA22;
    sc_in < double > ANA23;
    sc_in < double > ANA30;
    sc_in < double > ANA31;
    sc_in < double > ANA32;
    sc_in < double > ANA33;
    sc_in < double > ANA40;
    sc_in < double > ANA41;
    sc_in < double > ANA42;
    sc_in < double > ANA43;
    sc_in < double > ANA50;
    sc_in < double > ANA51;
    sc_in < double > ANA52;
    sc_in < double > ANA53;
    sc_in < double > ANA60;
    sc_in < double > ANA61;
    sc_in < double > ANA62;
    sc_in < double > ANA63;
    sc_in < double > ANA70;
    sc_in < double > ANA71;

    sc_in < sc_uint<7> > IFVCH0;
    sc_in < sc_uint<7> > IFVCH1;
    sc_in < sc_uint<7> > IFVCH2;
    sc_in < sc_uint<7> > IFVCH3;
    sc_in < sc_uint<7> > IFVCHA;
    sc_in < bool > IFRD0;
    sc_in < bool > IFRD1;
    sc_in < bool > IFRD2;
    sc_in < bool > IFRD3;
    sc_in < bool > IFRDA;
    sc_in < bool > IFRE0;
    sc_in < bool > IFRE1;
    sc_in < bool > IFRE2;
    sc_in < bool > IFRE3;
    sc_in < bool > IFREA;
    sc_in < unsigned int > PVCR_VALUE0;
    sc_in < unsigned int > PVCR_VALUE1;
    sc_in < unsigned int > PVCR_VALUE2;
    sc_in < unsigned int > PVCR_VALUE3;
    sc_in < unsigned int > PVCR_VALUEA;
    sc_in < bool > PVCR_TRG0;
    sc_in < bool > PVCR_TRG1;
    sc_in < bool > PVCR_TRG2;
    sc_in < bool > PVCR_TRG3;
    sc_in < bool > PVCR_TRGA;
    sc_in < bool > SV_MODE;
    sc_out < bool > INT_AD00;
    sc_out < bool > INT_AD01;
    sc_out < bool > INT_AD02;
    sc_out < bool > INT_AD03;
    sc_out < bool > INT_AD04;
    sc_out < bool > INT_AD10;
    sc_out < bool > INT_AD11;
    sc_out < bool > INT_AD12;
    sc_out < bool > INT_AD13;
    sc_out < bool > INT_AD14;
    sc_out < bool > INT_AD20;
    sc_out < bool > INT_AD21;
    sc_out < bool > INT_AD22;
    sc_out < bool > INT_AD23;
    sc_out < bool > INT_AD24;
    sc_out < bool > INT_AD30;
    sc_out < bool > INT_AD31;
    sc_out < bool > INT_AD32;
    sc_out < bool > INT_AD33;
    sc_out < bool > INT_AD34;
    sc_out < bool > INT_ADA0;
    sc_out < bool > INT_ADA1;
    sc_out < bool > INT_ADA2;
    sc_out < bool > INT_ADA3;
    sc_out < bool > INT_ADA4;
    sc_out < bool > INT_MPX0;
    sc_out < bool > INT_MPX1;
    sc_out < bool > INT_MPX2;
    sc_out < bool > INT_MPX3;
    sc_out < bool > INT_MPXA;
    sc_out < bool > ADPE0;
    sc_out < bool > ADPE1;
    sc_out < bool > ADPE2;
    sc_out < bool > ADPE3;
    sc_out < bool > ADPEA;
    sc_out < bool > INT_SGD0;
    sc_out < bool > INT_SGD1;
    sc_out < bool > INT_SGD2;
    sc_out < bool > INT_SGD3;
    sc_out < bool > INT_SGDA;
    sc_out < bool > ADEND00;
    sc_out < bool > ADEND01;
    sc_out < bool > ADEND02;
    sc_out < bool > ADEND03;
    sc_out < bool > ADEND04;
    sc_out < bool > ADEND10;
    sc_out < bool > ADEND11;
    sc_out < bool > ADEND12;
    sc_out < bool > ADEND13;
    sc_out < bool > ADEND14;
    sc_out < bool > ADEND20;
    sc_out < bool > ADEND21;
    sc_out < bool > ADEND22;
    sc_out < bool > ADEND23;
    sc_out < bool > ADEND24;
    sc_out < bool > ADEND30;
    sc_out < bool > ADEND31;
    sc_out < bool > ADEND32;
    sc_out < bool > ADEND33;
    sc_out < bool > ADEND34;
    sc_out < bool > ADENDA0;
    sc_out < bool > ADENDA1;
    sc_out < bool > ADENDA2;
    sc_out < bool > ADENDA3;
    sc_out < bool > ADENDA4;
    sc_out < bool > INT_ERR0;
    sc_out < bool > INT_ERR1;
    sc_out < bool > INT_ERR2;
    sc_out < bool > INT_ERR3;
    sc_out < bool > INT_ERRA;
    // RH850_U2B for IFC
    sc_out < sc_uint<16> > ADC0_IFDATA;
    sc_out < sc_uint<16> > ADC1_IFDATA;
    sc_out < sc_uint<16> > ADC2_IFDATA;
    sc_out < sc_uint<16> > ADC3_IFDATA;
    sc_out < sc_uint<16> > ADCA_IFDATA;

    // RH850_U2B for GTM
    sc_out < bool > DFREQ0;
    sc_out < bool > DFREQ1;
    sc_out < bool > DFREQ2;
    sc_out < bool > DFREQ3;
    sc_out < bool > DFREQA;
    sc_out < sc_uint<4> > DFTAG0;
    sc_out < sc_uint<4> > DFTAG1;
    sc_out < sc_uint<4> > DFTAG2;
    sc_out < sc_uint<4> > DFTAG3;
    sc_out < sc_uint<4> > DFTAGA;
    sc_out < sc_uint<16> > DFDATA0;
    sc_out < sc_uint<16> > DFDATA1;
    sc_out < sc_uint<16> > DFDATA2;
    sc_out < sc_uint<16> > DFDATA3;
    sc_out < sc_uint<16> > DFDATAA;
    sc_out < bool > PVCR_END0;
    sc_out < bool > PVCR_END1;
    sc_out < bool > PVCR_END2;
    sc_out < bool > PVCR_END3;
    sc_out < bool > PVCR_ENDA;
    sc_out < unsigned short > PVCR_PWDDR0;
    sc_out < unsigned short > PVCR_PWDDR1;
    sc_out < unsigned short > PVCR_PWDDR2;
    sc_out < unsigned short > PVCR_PWDDR3;
    sc_out < unsigned short > PVCR_PWDDRA;

    sc_out < bool > *VCEND0[emVCRnum_Max];
    sc_out < bool > PWM_VCEND0;
    sc_out < bool > VCULMO0;
    sc_out < bool > VCLLMO0;

    sc_out < bool > *VCEND1[emVCRnum_Max];
    sc_out < bool > PWM_VCEND1;
    sc_out < bool > VCULMO1;
    sc_out < bool > VCLLMO1;

    sc_out < bool > *VCEND2[emVCRnum_Max];
    sc_out < bool > PWM_VCEND2;
    sc_out < bool > VCULMO2;
    sc_out < bool > VCLLMO2;

    sc_out < bool > *VCEND3[emVCRnum_Max];
    sc_out < bool > PWM_VCEND3;
    sc_out < bool > VCULMO3;
    sc_out < bool > VCLLMO3;

    sc_out < bool > *VCENDA[emVCRnum_Max];
    sc_out < bool > PWM_VCENDA;
    sc_out < bool > VCULMOA;
    sc_out < bool > VCLLMOA;

    sc_out < sc_uint<3> > MPXCUR0;
    sc_out < sc_uint<3> > MPXCUR1;
    sc_out < sc_uint<3> > MPXCUR2;
    sc_out < sc_uint<3> > MPXCUR3;
    sc_out < sc_uint<3> > MPXCURA;

    sc_out < unsigned char > ULE_LPS0;
    sc_out < unsigned char > ULE_LPS1;
    sc_out < unsigned char > ULE_LPS2;
    sc_out < unsigned char > ULE_LPS3;
    sc_out < unsigned char > ULE_LPSA;

    // Attributes
    sc_core::sc_attribute<std::string> TARGET_PRODUCT;          // TARGET_PRODUCT valid setting 
    sc_core::sc_attribute<std::string> ADC_NUM;                // Number of ADC inside SARAD
    // end added
    sc_core::sc_attribute<std::string> P_SADC_SARCHNUM0;       // Number of Physical channel of ADC0
    sc_core::sc_attribute<std::string> P_SADC_SARCHNUM1;       // Number of Physical channel of ADC1
    sc_core::sc_attribute<std::string> P_SADC_SARCHNUM2;       // Number of Physical channel of ADC2
    sc_core::sc_attribute<std::string> P_SADC_SARCHNUM3;       // Number of Physical channel of ADC3
    sc_core::sc_attribute<std::string> P_SADC_SARCHNUMA;       // Number of Physical channel of ADCA
    sc_core::sc_attribute<std::string> VCRnum0;                // Number of Virtual channel of ADC0
    sc_core::sc_attribute<std::string> VCRnum1;                // Number of Virtual channel of ADC1
    sc_core::sc_attribute<std::string> VCRnum2;                // Number of Virtual channel of ADC2
    sc_core::sc_attribute<std::string> VCRnum3;                // Number of Virtual channel of ADC3
    sc_core::sc_attribute<std::string> VCRnumA;                // Number of Virtual channel of ADCA
    sc_core::sc_attribute<std::string> EX_MPX_PCH0;            // Analog input for MPX of ADC0
    sc_core::sc_attribute<std::string> EX_MPX_PCH1;            // Analog input for MPX of ADC1
    sc_core::sc_attribute<std::string> EX_MPX_PCH2;            // Analog input for MPX of ADC2
    sc_core::sc_attribute<std::string> EX_MPX_PCH3;            // Analog input for MPX of ADC3
    sc_core::sc_attribute<std::string> EX_MPX_PCHA;            // Analog input for MPX of ADCA

    sc_signal < double > const_double;
    sc_signal < bool > ADCx_sig_stubs;
   
private:
    struct TargetConfig { 
       std::string target_product_name;
       unsigned int adc_num;
       unsigned int physical_ch_num0;
       unsigned int physical_ch_num1;
       unsigned int physical_ch_num2;
       unsigned int physical_ch_num3;
       unsigned int physical_ch_numa;
       unsigned int virtual_ch_num0;
       unsigned int virtual_ch_num1;
       unsigned int virtual_ch_num2;
       unsigned int virtual_ch_num3;
       unsigned int virtual_ch_numa;
       unsigned int external_mpx_pch0;
       unsigned int external_mpx_pch1;
       unsigned int external_mpx_pch2;
       unsigned int external_mpx_pch3;
       unsigned int external_mpx_pcha;
       unsigned int th_channel_num0;
       unsigned int th_channel_num1;
       unsigned int th_channel_num2;
       unsigned int th_channel_num3;
       unsigned int th_channel_numa;
    };

    //Variable 
    unsigned int mPResetPeriod;
    unsigned int mADResetPeriod;
    unsigned int mPResetPeriodA;
    unsigned int mADResetPeriodA;
    bool mIsCmdPResetFlag;
    bool mIsCmdADResetFlag;
    bool mSARPortResetFlag;
    bool mPResetFlag;
    bool mSARCmdPResetFlag;
    bool mSARCmdADResetFlag;
    bool mIsCmdPResetFlagA;
    bool mIsCmdADResetFlagA;
    bool mSARPortResetFlagA;
    bool mPResetFlagA;
    bool mSARCmdPResetFlagA;
    bool mSARCmdADResetFlagA;
    unsigned int mSARADState;
    bool mIsADInitialize;
    bool mIsPInitialize;
    bool mIsADInitializeA;
    bool mIsPInitializeA;
    bool mIsRefVol0Update;
    bool mIsRefVol1Update;
    bool mIsRefVol2Update;
    bool mIsRefVol3Update;
    double mPreAVcc0   ;
    double mPreAVcc1   ;
    double mPreAVcc2   ;
    double mPreAVcc3   ;
    double mPreAvrefh0 ;
    double mPreAvrefh1 ;
    double mPreAvrefh2 ;
    double mPreAvrefh3 ;
    double mPretTSPL   ;
    double mPretTHSD;
    double mPretTHLD;
    unsigned int mPreExSmpt;
    unsigned int mPreSmpt;
    double mPretSAR;
    double mPretD  ;
    double mPretED ;
    unsigned int mPreOffsetAddMask;
    bool mPreEnableTimeCalculation;
    double mPreAVccMax;
    double mPreAVccMin;
    double mPreAvrefhMin;
    double mPclkPeriod;
    double mADclkPeriod;
    double mPrePclkPeriod;
    double mPreADclkPeriod;

    unsigned int mADCNum;
    TargetConfig mTargetConfigTable[emTargetConfigNum];

    //Event
    sc_event mCmdPResetEvent;
    sc_event mCmdCancelPResetEvent;
    sc_event mCmdADResetEvent;
    sc_event mCmdCancelADResetEvent;
    sc_event mCmdPResetEventA;
    sc_event mCmdCancelPResetEventA;
    sc_event mCmdADResetEventA;
    sc_event mCmdCancelADResetEventA;

    //function
    //Connect port
    void ConnectPort (void);

    //tlm function
    void tgt_acc (unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t);
    unsigned int  tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans);

    bool AddressArbitrate (const unsigned int id, tlm::tlm_command command, const bool is_dbg, const unsigned int addr, unsigned char *p_data, const unsigned int size);

    //call_back function
    void cb_ADSYNSTCR_ADSTART(RegCBstr str);
    void cb_ADTSYNSTCR_ADTSTART(RegCBstr str);
    void cb_VMONVDCR1_VDE1(RegCBstr str);
    void cb_VMONVDCR2_VDE2(RegCBstr str);

    //Normal function
    void EnableReset(const bool is_active,const unsigned int port_reset);

    //handle_command function
    void DumpStatInfo(void);
    void AssertPReset(const double start_time, const double period);
    void AssertADReset(const double start_time, const double period);
    void AssertPResetA(const double start_time, const double period);
    void AssertADResetA(const double start_time, const double period);
    void AssignTHCircuit(const unsigned int adc_index, const unsigned int th_channel, const unsigned int group_channel, const unsigned int physic_channel);

    //method
    void InitialAVREFH0Method(void);
    void InitialAVREFH1Method(void);
    void InitialAVREFH2Method(void);
    void InitialAVREFH3Method(void);
    void SARADstateMethod(void);
    void AVREFH0Method(void);
    void AVREFH1Method(void);
    void AVREFH2Method(void);
    void AVREFH3Method(void);
    void PCLKMethod(void);
    void ADCLKMethod(void);
    void CLKA_ADCMethod(void);
    void PResetMethod(void);
    void ADResetMethod(void);
    void PResetMethodA(void);
    void ADResetMethodA(void);
    void AssertPResetMethod(void);
    void DeAssertPResetMethod(void);
    void AssertADResetMethod(void);
    void DeAssertADResetMethod(void);
    void AssertPResetMethodA(void);
    void DeAssertPResetMethodA(void);
    void AssertADResetMethodA(void);
    void DeAssertADResetMethodA(void);
    void SVModeMethod(void);
    void UpdateChannelNumMethod (void);
    bool GetAllSGStopCheck(void);
    double GetTimeResolution(void);
    void SetLatency_TLM(const unsigned int id, const double pclk_period, const bool is_constructor);
    void SetCLKfreq(std::string clk_name, double clk_freq);
    std::string CommandCB(const std::vector<std::string>& args);
    void FixInputMethod(void);
};

#endif //__SARAD_U2B_H__
