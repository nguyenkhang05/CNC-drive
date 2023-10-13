// -----------------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2012-2015 Renesas System Design Co., Ltd.
// Copyright(c) 2012-2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------

#ifndef __SARAD_H__
#define __SARAD_H__

#include "systemc.h"
#include "tlm.h"
#include "tlm_tgt_if.h"
#include <iterator>

#include "sarad_regif.h"
#include "adc.h"
#include "Cgeneral_timer.h"

/// SAR model class
class Csarad: public sc_module
             ,public vpcl::tlm_tgt_if<32>
             ,public Csarad_regif
{
#include "sarad_cmdif.h"
private:
    //Enum
    enum eSARAD_CONSTANCE {
        emNanoSecond = 1000000000
       ,emTargetConfigNum = 8
       ,emADOPDIGAddr = 0x748
    };
    enum eSARAD_STATE{
        emRESET = 0
       ,emIDLE = 1
       ,emOPERATING = 2
    };
    /// SARAD4 add
    // Value for attributes
    enum eAttr_value {
        emADC_NUM_Def = 2
       ,emADC_NUM_Min = 1
       ,emADC_NUM_Max = 4
       ,emP_SADC_SARCHNUM_Def = 20
       ,emP_SADC_SARCHNUM_Min = 1
       ,emP_SADC_SARCHNUM_Max = 32
       ,emVCRnum_Def = 36
       ,emVCRnum_Min = 16
       ,emVCRnum_Max = 48
       ,emEX_MPX_PCH0_Def = 6
       ,emEX_MPX_PCH1_Def = 0
       ,emEX_MPX_PCH2_Def = 6
       ,emEX_MPX_PCH3_Def = 0
       ,emTH_CHNUM_Def = 6
       ,emTH_CHNUM_Min = 0
       ,emTH_CHNUM_Max = 6
    };


public:

    SC_HAS_PROCESS(Csarad);
    Cadc *mADC0;
    Cadc *mADC1;
    // RH850_E2x added
    Cadc *mADC2;
    Cadc *mADC3;
    Csarad(sc_module_name name);
    ~Csarad();

    //Port declaration
    sc_in<sc_dt::uint64> pclk;
    sc_in<sc_dt::uint64> ADCLK;
    sc_in<bool> preset_n;
    sc_in< double> AVREFH0;
    sc_in< double> AVREFH1;
    // RH850_E2x added
    sc_in< double> AVREFH2;
    sc_in< double> AVREFH3;

    sc_in<bool> AD0SG0TRG;
    sc_in<bool> AD0SG1TRG;
    sc_in<bool> AD0SG2TRG;
    sc_in<bool> AD0SG3TRG;
    sc_in<bool> AD0SG4TRG;
    sc_in<bool> AD1SG0TRG;
    sc_in<bool> AD1SG1TRG;
    sc_in<bool> AD1SG2TRG;
    sc_in<bool> AD1SG3TRG;
    sc_in<bool> AD1SG4TRG;
    // RH850_E2x added
    sc_in<bool> AD2SG0TRG;
    sc_in<bool> AD2SG1TRG;
    sc_in<bool> AD2SG2TRG;
    sc_in<bool> AD2SG3TRG;
    sc_in<bool> AD2SG4TRG;
    sc_in<bool> AD3SG0TRG;
    sc_in<bool> AD3SG1TRG;
    sc_in<bool> AD3SG2TRG;
    sc_in<bool> AD3SG3TRG;
    sc_in<bool> AD3SG4TRG;

    sc_in< double> AN000;
    sc_in< double> AN001;
    sc_in< double> AN002;
    sc_in< double> AN003;
    sc_in< double> AN010;
    sc_in< double> AN011;
    sc_in< double> AN012;
    sc_in< double> AN013;
    sc_in< double> AN020;
    sc_in< double> AN021;
    sc_in< double> AN022;
    sc_in< double> AN023;
    sc_in< double> AN030;
    sc_in< double> AN031;
    sc_in< double> AN032;
    sc_in< double> AN033;
    sc_in< double> AN040;
    sc_in< double> AN041;
    sc_in< double> AN042;
    sc_in< double> AN043;
    /// SARAD4 add
    sc_in< double> AN050;
    sc_in< double> AN051;
    sc_in< double> AN052;
    sc_in< double> AN053;
    sc_in< double> AN060;
    sc_in< double> AN061;
    sc_in< double> AN062;
    sc_in< double> AN063;
    sc_in< double> AN070;
    sc_in< double> AN071;
    sc_in< double> AN072;
    sc_in< double> AN073;

    sc_in< double> AN100;
    sc_in< double> AN101;
    sc_in< double> AN102;
    sc_in< double> AN103;
    sc_in< double> AN110;
    sc_in< double> AN111;
    sc_in< double> AN112;
    sc_in< double> AN113;
    sc_in< double> AN120;
    sc_in< double> AN121;
    sc_in< double> AN122;
    sc_in< double> AN123;
    sc_in< double> AN130;
    sc_in< double> AN131;
    sc_in< double> AN132;
    sc_in< double> AN133;
    sc_in< double> AN140;
    sc_in< double> AN141;
    sc_in< double> AN142;
    sc_in< double> AN143;
    /// SARAD4 add
    sc_in< double> AN150;
    sc_in< double> AN151;
    sc_in< double> AN152;
    sc_in< double> AN153;
    sc_in< double> AN160;
    sc_in< double> AN161;
    sc_in< double> AN162;
    sc_in< double> AN163;
    sc_in< double> AN170;
    sc_in< double> AN171;
    sc_in< double> AN172;
    sc_in< double> AN173;
    // RH850_E2x added
    sc_in< double> AN200;
    sc_in< double> AN201;
    sc_in< double> AN202;
    sc_in< double> AN203;
    sc_in< double> AN210;
    sc_in< double> AN211;
    sc_in< double> AN212;
    sc_in< double> AN213;
    sc_in< double> AN220;
    sc_in< double> AN221;
    sc_in< double> AN222;
    sc_in< double> AN223;
    sc_in< double> AN230;
    sc_in< double> AN231;
    sc_in< double> AN232;
    sc_in< double> AN233;
    sc_in< double> AN240;
    sc_in< double> AN241;
    sc_in< double> AN242;
    sc_in< double> AN243;
    sc_in< double> AN250;
    sc_in< double> AN251;
    sc_in< double> AN252;
    sc_in< double> AN253;
    sc_in< double> AN260;
    sc_in< double> AN261;
    sc_in< double> AN262;
    sc_in< double> AN263;
    sc_in< double> AN270;
    sc_in< double> AN271;
    sc_in< double> AN272;
    sc_in< double> AN273;
    sc_in< double> AN300;
    sc_in< double> AN301;
    sc_in< double> AN302;
    sc_in< double> AN303;
    sc_in< double> AN310;
    sc_in< double> AN311;
    sc_in< double> AN312;
    sc_in< double> AN313;
    sc_in< double> AN320;
    sc_in< double> AN321;
    sc_in< double> AN322;
    sc_in< double> AN323;
    sc_in< double> AN330;
    sc_in< double> AN331;
    sc_in< double> AN332;
    sc_in< double> AN333;
    sc_in< double> AN340;
    sc_in< double> AN341;
    sc_in< double> AN342;
    sc_in< double> AN343;
    sc_in< double> AN350;
    sc_in< double> AN351;
    sc_in< double> AN352;
    sc_in< double> AN353;
    sc_in< double> AN360;
    sc_in< double> AN361;
    sc_in< double> AN362;
    sc_in< double> AN363;
    sc_in< double> AN370;
    sc_in< double> AN371;
    sc_in< double> AN372;
    sc_in< double> AN373;

    // RH850_E2x added for IFC
    sc_in< sc_uint<6> > IFVCH0;
    sc_in< sc_uint<6> > IFVCH1;
    sc_in< sc_uint<6> > IFVCH2;
    sc_in< sc_uint<6> > IFVCH3;
    sc_in<bool> IFRD0;
    sc_in<bool> IFRD1;
    sc_in<bool> IFRD2;
    sc_in<bool> IFRD3;
    sc_in<bool> IFRE0;
    sc_in<bool> IFRE1;
    sc_in<bool> IFRE2;
    sc_in<bool> IFRE3;

    sc_out<bool> INT_AD00;
    sc_out<bool> INT_AD01;
    sc_out<bool> INT_AD02;
    sc_out<bool> INT_AD03;
    sc_out<bool> INT_AD04;
    sc_out<bool> INT_AD10;
    sc_out<bool> INT_AD11;
    sc_out<bool> INT_AD12;
    sc_out<bool> INT_AD13;
    sc_out<bool> INT_AD14;
    sc_out<bool> INT_MPX0;
    sc_out<bool> INT_MPX1;
    sc_out<bool> INT_ADE0;
    sc_out<bool> INT_ADE1;
    sc_out<bool> ADPE0;
    sc_out<bool> ADPE1;
    sc_out<bool> ADEND00; 
    sc_out<bool> ADEND01;
    sc_out<bool> ADEND02;
    sc_out<bool> ADEND03;
    sc_out<bool> ADEND04;
    sc_out<bool> ADEND10;
    sc_out<bool> ADEND11;
    sc_out<bool> ADEND12;
    sc_out<bool> ADEND13;
    sc_out<bool> ADEND14;

    // RH850_E2x added
    sc_out<bool> INT_AD20;
    sc_out<bool> INT_AD21;
    sc_out<bool> INT_AD22;
    sc_out<bool> INT_AD23;
    sc_out<bool> INT_AD24;
    sc_out<bool> INT_AD30;
    sc_out<bool> INT_AD31;
    sc_out<bool> INT_AD32;
    sc_out<bool> INT_AD33;
    sc_out<bool> INT_AD34;
    sc_out<bool> INT_MPX2;
    sc_out<bool> INT_MPX3;
    sc_out<bool> INT_ADE2;
    sc_out<bool> INT_ADE3;
    sc_out<bool> ADPE2;
    sc_out<bool> ADPE3;
    sc_out<bool> ADEND20; 
    sc_out<bool> ADEND21;
    sc_out<bool> ADEND22;
    sc_out<bool> ADEND23;
    sc_out<bool> ADEND24;
    sc_out<bool> ADEND30;
    sc_out<bool> ADEND31;
    sc_out<bool> ADEND32;
    sc_out<bool> ADEND33;
    sc_out<bool> ADEND34;

    // RH850_E2x added for IFC
    sc_out< sc_uint<16> > ADC0_IFDATA;
    sc_out< sc_uint<16> > ADC1_IFDATA;
    sc_out< sc_uint<16> > ADC2_IFDATA;
    sc_out< sc_uint<16> > ADC3_IFDATA;

    // RH850_E2x added for ASF
    sc_out<bool> DFREQ0;
    sc_out<bool> DFREQ1;
    sc_out<bool> DFREQ2;
    sc_out<bool> DFREQ3;
    sc_out< sc_uint<4> > DFTAG0;
    sc_out< sc_uint<4> > DFTAG1;
    sc_out< sc_uint<4> > DFTAG2;
    sc_out< sc_uint<4> > DFTAG3;
    sc_out< sc_uint<16> > DFDATA0;
    sc_out< sc_uint<16> > DFDATA1;
    sc_out< sc_uint<16> > DFDATA2;
    sc_out< sc_uint<16> > DFDATA3;

    // RH850_E2x Enhancement
    sc_out<bool> *VCEND0[emVCRnum_Max];
    sc_out<bool> VCULMO0;
    sc_out<bool> VCLLMO0;

    sc_out<bool> *VCEND1[emVCRnum_Max];
    sc_out<bool> VCULMO1;
    sc_out<bool> VCLLMO1;

    sc_out<bool> *VCEND2[emVCRnum_Max];
    sc_out<bool> VCULMO2;
    sc_out<bool> VCLLMO2;

    sc_out<bool> *VCEND3[emVCRnum_Max];
    sc_out<bool> VCULMO3;
    sc_out<bool> VCLLMO3;
   
    // RH850-E2x  FCC1/FCC2
    sc_out<bool> ADMPX00;
    sc_out<bool> ADMPX01;
    sc_out<bool> ADMPX02;
    sc_out<bool> ADMPX03;
    sc_out<bool> ADMPX04;

    sc_out<bool> ADMPX10;
    sc_out<bool> ADMPX11;
    sc_out<bool> ADMPX12;
    sc_out<bool> ADMPX13;
    sc_out<bool> ADMPX14;

    sc_out<bool> ADMPX20;
    sc_out<bool> ADMPX21;
    sc_out<bool> ADMPX22;
    sc_out<bool> ADMPX23;
    sc_out<bool> ADMPX24;

    sc_out<bool> ADMPX30;
    sc_out<bool> ADMPX31;
    sc_out<bool> ADMPX32;
    sc_out<bool> ADMPX33;
    sc_out<bool> ADMPX34;


    /// SARAD4 add
    // Attributes
    // added for CC-Cube D1D2 and RH850 P1M-E
    sc_core::sc_attribute<std::string> TARGET_PRODUCT;          // TARGET_PRODUCT valid setting {"","CCC_D<1/2>V<1/2>", "RH850_P1M-E"}
    sc_core::sc_attribute<std::string> ADC_NUM;                // Number of ADC inside SARAD
    // end added
    sc_core::sc_attribute<std::string> P_SADC_SARCHNUM0;       // Number of Physical channel of ADC0
    sc_core::sc_attribute<std::string> P_SADC_SARCHNUM1;       // Number of Physical channel of ADC1
    sc_core::sc_attribute<std::string> P_SADC_SARCHNUM2;       // Number of Physical channel of ADC2
    sc_core::sc_attribute<std::string> P_SADC_SARCHNUM3;       // Number of Physical channel of ADC3
    sc_core::sc_attribute<std::string> VCRnum0;                // Number of Virtual channel of ADC0
    sc_core::sc_attribute<std::string> VCRnum1;                // Number of Virtual channel of ADC1
    sc_core::sc_attribute<std::string> VCRnum2;                // Number of Virtual channel of ADC2
    sc_core::sc_attribute<std::string> VCRnum3;                // Number of Virtual channel of ADC3
    sc_core::sc_attribute<std::string> EX_MPX_PCH0;            // Analog input for MPX of ADC0
    sc_core::sc_attribute<std::string> EX_MPX_PCH1;            // Analog input for MPX of ADC1
    sc_core::sc_attribute<std::string> EX_MPX_PCH2;            // Analog input for MPX of ADC2
    sc_core::sc_attribute<std::string> EX_MPX_PCH3;            // Analog input for MPX of ADC3
   
private:
    struct TargetConfig { 
       std::string target_product_name;
       unsigned int adc_num;
       unsigned int physical_ch_num0;
       unsigned int physical_ch_num1;
       unsigned int physical_ch_num2;
       unsigned int physical_ch_num3;
       unsigned int virtual_ch_num0;
       unsigned int virtual_ch_num1;
       unsigned int virtual_ch_num2;
       unsigned int virtual_ch_num3;
       unsigned int external_mpx_pch0;
       unsigned int external_mpx_pch1;
       unsigned int external_mpx_pch2;
       unsigned int external_mpx_pch3;
       unsigned int th_channel_num0;
       unsigned int th_channel_num1;
       unsigned int th_channel_num2;
       unsigned int th_channel_num3;
    };

    //Variable 
    unsigned int mResetPeriod;
    bool mIsCmdResetFlag;
    bool mSARPortResetFlag;
    bool mSARCmdResetFlag;
    unsigned int mSARADState;
    bool mIsInitialize;
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
    double mPretSAR;
    double mPretD  ;
    double mPretED ;
    unsigned int mPreStartAddrADCA0;
    unsigned int mPreStartAddrADCA1;
    unsigned int mPreStartAddrADCA2;
    unsigned int mPreStartAddrADCA3;
    unsigned int mPreEndAddressADCA0;
    unsigned int mPreEndAddressADCA1;
    unsigned int mPreEndAddressADCA2;
    unsigned int mPreEndAddressADCA3;
    unsigned int mPreOffsetAddMask;
    bool mPreEnableTimeCalculation;
    /// SARAD3
    double mPreAVccMax;
    double mPreAVccMin;
    double mPreAvrefhMin;

    /// added for CC-Cube D1D2 and RH850 P1M-E
    unsigned int mADCNum;
    TargetConfig mTargetConfigTable[emTargetConfigNum];

    //Event
    sc_event mCmdResetEvent;
    sc_event mCmdCancelResetEvent;

    //function
    //Connect port
    void ConnectPort (void);

    //tlm function
    void tgt_acc (tlm::tlm_generic_payload &trans, sc_time &t);
    unsigned int  tgt_acc_dbg(tlm::tlm_generic_payload &trans);

    bool AddressArbitrate (tlm::tlm_command command, const bool is_dbg, const unsigned int addr, unsigned char *p_data, const unsigned int size);

    //call_back function
    void cb_ADSYNSTCR_ADSTART(RegCBstr str);
    void cb_ADTSYNSTCR_ADTSTART(RegCBstr str);
    void cb_VMONVDCR1_VDE1(RegCBstr str);
    void cb_VMONVDCR2_VDE2(RegCBstr str);

    //Normal function
    void EnableReset(const bool is_active);

    //handle_command function
    void DumpStatInfo(void);
    void AssertReset(const double start_time, const double period);
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
    void ResetMethod(void);
    void AssertResetMethod(void);
    void DeAssertResetMethod(void);
    void CheckBaseAddrMethod (void);
    /// SARAD4 add
    void UpdateChannelNumMethod (void);

    double GetTimeResolution(void);
    void SetLatency_TLM(const double pclk_period, const bool is_constructor);
    void SetCLKfreq(std::string clk_name, double clk_freq);
    std::string CommandCB(const std::vector<std::string>& args);

};

#endif //__SAR_H__
