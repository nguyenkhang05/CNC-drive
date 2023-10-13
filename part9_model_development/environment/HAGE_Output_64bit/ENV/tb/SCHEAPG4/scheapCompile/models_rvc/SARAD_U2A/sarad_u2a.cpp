// -----------------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2017-2020 Renesas Electronics Corporation
// Copyright(c) 2017-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------

#include "re_define.h"
#include "sarad_u2a.h"

/// SARAD4 add - fix sc_assert() issue on SCHEAP
#define re_sc_assert(expr, msg)     ((void) ((expr) ? 0 : (SC_REPORT_FATAL( sc_core::SC_ID_ASSERTION_FAILED_ , msg), 0)))

//Constructor of SARAD class
Csarad_u2a::Csarad_u2a(sc_module_name name):
         vpcl::tlm_tgt_if<32,tlm::tlm_base_protocol_types,3>(name)
        , Csarad_u2a_regif((std::string)name, 32)
        , pclk("pclk")
        , ADCLK("ADCLK")
        , CLKA_ADC("CLKA_ADC")
        , preset_n("preset_n")
        , RESETAD_N("RESETAD_N")
        , preset2_n("preset2_n")
        , RESETAD2_N("RESETAD2_N")
        , AVREFH0("AVREFH0")
        , AVREFH1("AVREFH1")
        , AVREFH2("AVREFH2")
        , AD0SG0TRG("AD0SG0TRG")
        , AD0SG1TRG("AD0SG1TRG")
        , AD0SG2TRG("AD0SG2TRG")
        , AD0SG3TRG("AD0SG3TRG")
        , AD0SG4TRG("AD0SG4TRG")
        , AD1SG0TRG("AD1SG0TRG")
        , AD1SG1TRG("AD1SG1TRG")
        , AD1SG2TRG("AD1SG2TRG")
        , AD1SG3TRG("AD1SG3TRG")
        , AD1SG4TRG("AD1SG4TRG")
        , AD2SG0TRG("AD2SG0TRG")
        , AD2SG1TRG("AD2SG1TRG")
        , AD2SG2TRG("AD2SG2TRG")
        , AD2SG3TRG("AD2SG3TRG")
        , AD2SG4TRG("AD2SG4TRG")
        , AD2TAUD2I7("AD2TAUD2I7")
        , AD2TAUD2I15("AD2TAUD2I15")
        , AD2TAUJ2("AD2TAUJ2")
        , AD2TAUJ3("AD2TAUJ3")
        , AD2LPS("AD2LPS")
        , AN000("AN000")
        , AN001("AN001")
        , AN002("AN002")
        , AN003("AN003")
        , AN010("AN010")
        , AN011("AN011")
        , AN012("AN012")
        , AN013("AN013")
        , AN020("AN020")
        , AN021("AN021")
        , AN022("AN022")
        , AN023("AN023")
        , AN030("AN030")
        , AN031("AN031")
        , AN032("AN032")
        , AN033("AN033")
        , AN040("AN040")
        , AN041("AN041")
        , AN042("AN042")
        , AN043("AN043")
        , VMON_VCC("VMON_VCC")
        , VMON_E0VCC("VMON_E0VCC")
        , VMON_ISOVDD("VMON_ISOVDD")
        , VMON_AWOVDD("VMON_AWOVDD")
        , AN050("AN050")
        , AN051("AN051")
        , AN052("AN052")
        , AN053("AN053")
        , AN060("AN060")
        , AN061("AN061")
        , AN062("AN062")
        , AN063("AN063")
        , AN070("AN070")
        , AN071("AN071")
        , AN100("AN100")
        , AN101("AN101")
        , AN102("AN102")
        , AN103("AN103")
        , AN110("AN110")
        , AN111("AN111")
        , AN112("AN112")
        , AN113("AN113")
        , AN120("AN120")
        , AN121("AN121")
        , AN122("AN122")
        , AN123("AN123")
        , AN130("AN130")
        , AN131("AN131")
        , AN132("AN132")
        , AN133("AN133")
        , AN140("AN140")
        , AN141("AN141")
        , AN142("AN142")
        , AN143("AN143")
        , AN150("AN150")
        , AN151("AN151")
        , AN152("AN152")
        , AN153("AN153")
        , AN160("AN160")
        , AN161("AN161")
        , AN162("AN162")
        , AN163("AN163")
        , AN170("AN170")
        , AN171("AN171")
        , AN172("AN172")
        , AN173("AN173")
        , AN180("AN180")
        , AN181("AN181")
        , AN200("AN200")
        , AN201("AN201")
        , AN202("AN202")
        , AN203("AN203")
        , AN210("AN210")
        , AN211("AN211")
        , AN212("AN212")
        , AN213("AN213")
        , AN220("AN220")
        , AN221("AN221")
        , AN222("AN222")
        , AN223("AN223")
        , AN230("AN230")
        , AN231("AN231")
        , AN232("AN232")
        , AN233("AN233")
        , AN240("AN240")
        , AN241("AN241")
        , AN242("AN242")
        , AN243("AN243")
        , AN250("AN250")
        , AN251("AN251")
        , AN252("AN252")
        , AN253("AN253")
        , AN260("AN260")
        , AN261("AN261")
        , AN262("AN262")
        , AN263("AN263")
        , AN270("AN270")
        , AN271("AN271")
        , IFVCH0("IFVCH0")
        , IFVCH1("IFVCH1")
        , IFVCH2("IFVCH2")
        , IFRD0("IFRD0")
        , IFRD1("IFRD1")
        , IFRD2("IFRD2")
        , IFRE0("IFRE0")
        , IFRE1("IFRE1")
        , IFRE2("IFRE2")
        , PVCR_VALUE0("PVCR_VALUE0")
        , PVCR_VALUE1("PVCR_VALUE1")
        , PVCR_VALUE2("PVCR_VALUE2")
        , PVCR_TRG0("PVCR_TRG0")
        , PVCR_TRG1("PVCR_TRG1")
        , PVCR_TRG2("PVCR_TRG2")
        , SV_MODE("SV_MODE")
        , INT_AD00("INT_AD00")
        , INT_AD01("INT_AD01")
        , INT_AD02("INT_AD02")
        , INT_AD03("INT_AD03")
        , INT_AD04("INT_AD04")
        , INT_AD10("INT_AD10")
        , INT_AD11("INT_AD11")
        , INT_AD12("INT_AD12")
        , INT_AD13("INT_AD13")
        , INT_AD14("INT_AD14")
        , INT_AD20("INT_AD20")
        , INT_AD21("INT_AD21")
        , INT_AD22("INT_AD22")
        , INT_AD23("INT_AD23")
        , INT_AD24("INT_AD24")
        , INT_MPX0("INT_MPX0")
        , INT_MPX1("INT_MPX1")
        , INT_MPX2("INT_MPX2")
        , ADPE0("ADPE0")
        , ADPE1("ADPE1")
        , ADPE2("ADPE2")
        , ADEND00("ADEND00")
        , ADEND01("ADEND01")
        , ADEND02("ADEND02")
        , ADEND03("ADEND03")
        , ADEND04("ADEND04")
        , ADEND10("ADEND10")
        , ADEND11("ADEND11")
        , ADEND12("ADEND12")
        , ADEND13("ADEND13")
        , ADEND14("ADEND14")
        , ADEND20("ADEND20")
        , ADEND21("ADEND21")
        , ADEND22("ADEND22")
        , ADEND23("ADEND23")
        , ADEND24("ADEND24")
#if 0
        , INT_ADE0("INT_ADE0")
        , INT_ADE1("INT_ADE1")
        , INT_UL0("INT_UL0")
        , INT_UL1("INT_UL1")
#else
        // RH850_U2A added
        , INT_ERR0("INT_ERR0")
        , INT_ERR1("INT_ERR1")
        , INT_ERR2("INT_ERR2")
#endif
        , ADC0_IFDATA("ADC0_IFDATA")
        , ADC1_IFDATA("ADC1_IFDATA")
        , ADC2_IFDATA("ADC2_IFDATA")
        , DFREQ0("DFREQ0")
        , DFREQ1("DFREQ1")
        , DFREQ2("DFREQ2")
        , DFTAG0("DFTAG0")
        , DFTAG1("DFTAG1")
        , DFTAG2("DFTAG2")
        , DFDATA0("DFDATA0")
        , DFDATA1("DFDATA1")
        , DFDATA2("DFDATA2")
        , PVCR_END0("PVCR_END0")
        , PVCR_END1("PVCR_END1")
        , PVCR_END2("PVCR_END2")
        , PVCR_PWDDR0("PVCR_PWDDR0")
        , PVCR_PWDDR1("PVCR_PWDDR1")
        , PVCR_PWDDR2("PVCR_PWDDR2")
        , PWM_VCEND0("PWM_VCEND0")
        , VCULMO0("VCULMO0")
        , VCLLMO0("VCLLMO0")
        , PWM_VCEND1("PWM_VCEND1")
        , VCULMO1("VCULMO1")
        , VCLLMO1("VCLLMO1")
        , PWM_VCEND2("PWM_VCEND2")
        , VCULMO2("VCULMO2")
        , VCLLMO2("VCLLMO2")
        , MPXCUR0("MPXCUR0")
        , MPXCUR1("MPXCUR1")
        , MPXCUR2("MPXCUR2")
        , ULE_LPS0("ULE_LPS0")
        , ULE_LPS1("ULE_LPS1")
        , ULE_LPS2("ULE_LPS2")
        /// SARAD4 add
        // Attributes
        , TARGET_PRODUCT("TARGET_PRODUCT", "RH850_U2A")
        , ADC_NUM("ADC_NUM", "")
        , P_SADC_SARCHNUM0("P_SADC_SARCHNUM0", "")
        , P_SADC_SARCHNUM1("P_SADC_SARCHNUM1", "")
        , P_SADC_SARCHNUM2("P_SADC_SARCHNUM2", "")
        , VCRnum0("VCRnum0", "")
        , VCRnum1("VCRnum1", "")
        , VCRnum2("VCRnum2", "")
        , EX_MPX_PCH0("EX_MPX_PCH0", "")
        , EX_MPX_PCH1("EX_MPX_PCH1", "")
        , EX_MPX_PCH2("EX_MPX_PCH2", "")
        , const_double("const_double")
        , ADC0_sig_stubs("ADC0_sig_stubs")
        , ADC1_sig_stubs("ADC1_sig_stubs")
{//{{{

    //Initalze handleCommand
    CommandInit(this->name());
    Csarad_u2a_regif::set_instance_name(this->name());

    for (unsigned int index = 0; index < emVCRnum_Max; index++){
        std::ostringstream str_vcend[emADC_NUM_Max];
        for (unsigned int adc_num = 0; adc_num < emADC_NUM_Max; adc_num++){
            str_vcend[adc_num]<<"vcend"<<adc_num<<"_"<<index;
        }
        VCEND0[index] = new sc_out<bool>(str_vcend[0].str().c_str());
        VCEND1[index] = new sc_out<bool>(str_vcend[1].str().c_str());
        VCEND2[index] = new sc_out<bool>(str_vcend[2].str().c_str());
        VCEND0[index]->initialize(0);
        VCEND1[index]->initialize(0);
        VCEND2[index]->initialize(0);
    }

    //Instance ADC0, ADC1 and ADC2 modules and connect ports
    mADC0 = new Cadc_u2a ("mADC0", 0);
    sc_assert(mADC0 != NULL);
    mADC1 = new Cadc_u2a ("mADC1", 1);
    sc_assert(mADC1 != NULL);
    mADC2 = new Cadc_u2a ("mADC2", 2);
    sc_assert(mADC2 != NULL);

    ConnectPort();

    //Initialze variable
    mIsRefVol0Update    = false;
    mIsRefVol1Update    = false;
    mIsRefVol2Update    = false;
    mIsADInitialize     = true;
    mIsPInitialize      = true;
    mIsADInitialize2    = true;
    mIsPInitialize2     = true;
    mPResetPeriod       = 0;
    mADResetPeriod      = 0;
    mPResetPeriod2      = 0;
    mADResetPeriod2     = 0;
    mIsCmdPResetFlag    = false;
    mIsCmdADResetFlag   = false;
    mSARPortResetFlag   = false;
    mPResetFlag         = false;
    mSARCmdPResetFlag   = false;
    mSARCmdADResetFlag  = false;
    mIsCmdPResetFlag2   = false;
    mIsCmdADResetFlag2  = false;
    mSARPortResetFlag2  = false;
    mPResetFlag2        = false;
    mSARCmdPResetFlag2  = false;
    mSARCmdADResetFlag2 = false;
    mSARADState         = emIDLE;
    mPreAVcc0       = 3.3;
    mPreAVcc1       = 3.3;
    mPreAVcc2       = 3.3;
    mPreEVcc0       = 3.3;
    mPreEVcc1       = 3.3;
    mPreEVcc2       = 3.3;
    mPreAvrefh0     = 3.3;
    mPreAvrefh1     = 3.3;
    mPreAvrefh2     = 3.3;
    mPretTSPL       = 27;
    mPretTHSD       = 5;
    mPretTHLD       = 10;
    mPreExSmpt      = 60;
    mPreSmpt        = 18;
    mPretSAR        = 0;
    mPretD          = 0;
    mPretED         = 0;
    mPreStartAddrADCA0  = 0x1000;
    mPreStartAddrADCA1  = 0x2000;
    mPreStartAddrADCA2  = 0x3000;
    mPreEndAddressADCA0 = 0x1FFF;
    mPreEndAddressADCA1 = 0x2FFF;
    mPreEndAddressADCA2 = 0x3FFF;
    mPreOffsetAddMask   = 0x7FFF;
    mPreEnableTimeCalculation = true;
    mPreAVccMax = 3.6;
    mPreAVccMin = 3;
    mPreEVccMax = 3.6;
    mPreEVccMin = 3;
    mPreAvrefhMin   = 3;
    mPclkPeriod     = 0;
    mADclkPeriod    = 0;
    mPrePclkPeriod  = 0;
    mPreADclkPeriod = 0;
//    mHWTriggerSelectNum = 5;
    mADCNum         = emADC_NUM_Def;

    TargetConfig TempTargetConfigTable[emTargetConfigNum] =  {
                                                              {"RH850_U2A", 3, 34, 34, 30, 64, 64, 64, 5, 5, 5, 4, 4, 0}
                                                             };
    // copy TempTargetConfig
    for (unsigned int i=0;i<emTargetConfigNum;i++) {
        mTargetConfigTable[i] = TempTargetConfigTable[i];
    }

    //Initialize output port
    INT_ERR0.initialize(0);
    INT_ERR1.initialize(0);
    INT_ERR2.initialize(0);
    INT_AD00.initialize(0);
    INT_AD01.initialize(0);
    INT_AD02.initialize(0);
    INT_AD03.initialize(0);
    INT_AD04.initialize(0);
    INT_AD10.initialize(0);
    INT_AD11.initialize(0);
    INT_AD12.initialize(0);
    INT_AD13.initialize(0);
    INT_AD14.initialize(0);
    INT_AD20.initialize(0);
    INT_AD21.initialize(0);
    INT_AD22.initialize(0);
    INT_AD23.initialize(0);
    INT_AD14.initialize(0);
    INT_MPX0.initialize(0);
    INT_MPX1.initialize(0);
    INT_MPX2.initialize(0);
    //INT_ADE0.initialize(0);
    //INT_ADE1.initialize(0);
    ADPE0.initialize(0);
    ADPE1.initialize(0);
    ADPE2.initialize(0);
    ADEND00.initialize(0);
    ADEND01.initialize(0);
    ADEND02.initialize(0);
    ADEND03.initialize(0);
    ADEND04.initialize(0);
    ADEND10.initialize(0);
    ADEND11.initialize(0);
    ADEND12.initialize(0);
    ADEND13.initialize(0);
    ADEND14.initialize(0);
    ADEND20.initialize(0);
    ADEND21.initialize(0);
    ADEND22.initialize(0);
    ADEND23.initialize(0);
    ADEND24.initialize(0);
    //INT_UL0.initialize(0);
    //INT_UL1.initialize(0);
    //INT_UL2.initialize(0);
    ADC0_IFDATA.initialize(0);
    ADC1_IFDATA.initialize(0);
    ADC2_IFDATA.initialize(0);
    DFREQ0.initialize(0);
    DFREQ1.initialize(0);
    DFREQ2.initialize(0);
    DFTAG0.initialize(0);
    DFTAG1.initialize(0);
    DFTAG2.initialize(0);
    DFDATA0.initialize(0);
    DFDATA1.initialize(0);
    DFDATA2.initialize(0);
    VCULMO0.initialize(0);
    VCLLMO0.initialize(0);
    VCULMO1.initialize(0);
    VCLLMO1.initialize(0);
    VCULMO2.initialize(0);
    VCLLMO2.initialize(0);
//    MPXCUR0.initialize(0); // Initialized in ADC
//    MPXCUR1.initialize(0); // Initialized in ADC

    //Reset Method
    SC_METHOD(PResetMethod);
    dont_initialize();
    sensitive << preset_n;
    SC_METHOD(PResetMethod2);
    dont_initialize();
    sensitive << preset2_n;
    SC_METHOD(ADResetMethod);
    dont_initialize();
    sensitive << RESETAD_N;
    SC_METHOD(ADResetMethod2);
    dont_initialize();
    sensitive << RESETAD2_N;
    SC_METHOD(AssertPResetMethod);
    dont_initialize();
    sensitive << mCmdPResetEvent;
    SC_METHOD(AssertPResetMethod2);
    dont_initialize();
    sensitive << mCmdPResetEvent2;
    SC_METHOD(DeAssertPResetMethod);
    dont_initialize();
    sensitive << mCmdCancelPResetEvent;
    SC_METHOD(DeAssertPResetMethod2);
    dont_initialize();
    sensitive << mCmdCancelPResetEvent2;
    SC_METHOD(AssertADResetMethod);
    dont_initialize();
    sensitive << mCmdADResetEvent;
    SC_METHOD(AssertADResetMethod2);
    dont_initialize();
    sensitive << mCmdADResetEvent2;
    SC_METHOD(DeAssertADResetMethod);
    dont_initialize();
    sensitive << mCmdCancelADResetEvent;
    SC_METHOD(DeAssertADResetMethod2);
    dont_initialize();
    sensitive << mCmdCancelADResetEvent2;
    //Clock Method
    SC_METHOD(PCLKMethod);
    dont_initialize();
    sensitive << pclk;
    SC_METHOD(ADCLKMethod);
    dont_initialize();
    sensitive << ADCLK;
    SC_METHOD(CLKA_ADCMethod);
    dont_initialize();
    sensitive << CLKA_ADC;
    //VoltageMethod
    SC_METHOD(AVREFH0Method);
    dont_initialize();
    sensitive << AVREFH0;
    SC_METHOD(AVREFH1Method);
    dont_initialize();
    sensitive << AVREFH1;
    SC_METHOD(AVREFH2Method);
    dont_initialize();
    sensitive << AVREFH2;
    SC_METHOD(InitialAVREFH0Method);
    SC_METHOD(InitialAVREFH1Method);
    SC_METHOD(InitialAVREFH2Method);

    //State Method
    SC_METHOD(SARADstateMethod);
    dont_initialize();
    sensitive << mADC0->mADCChangeStateEvent;
    sensitive << mADC1->mADCChangeStateEvent;
    sensitive << mADC2->mADCChangeStateEvent;

    for (unsigned int id = 0; id < emMaxId; id++) {
        SetLatency_TLM (id, 0, true);
    }

    SC_METHOD(SVModeMethod);
    dont_initialize();
    sensitive << mADC0->mSVModeEvent;
    sensitive << mADC1->mSVModeEvent;
    sensitive << mADC2->mSVModeEvent;

    /// SARAD4 add
    SC_METHOD(UpdateChannelNumMethod);

    // Fix input ports for ADC2
    SC_METHOD(FixInputMethod);


}//}}}

//destructor of Csarad_u2a class
Csarad_u2a::~Csarad_u2a()
{//{{{
    delete mADC0;
    delete mADC1;
    delete mADC2;
}//}}}

//Connect port
void Csarad_u2a::ConnectPort(void)
{//{{{
    // ADC0
    //input port
    mADC0->ANm00(AN000);
    mADC0->ANm01(AN001);
    mADC0->ANm02(AN002);
    mADC0->ANm03(AN003);
    mADC0->ANm10(AN010);
    mADC0->ANm11(AN011);
    mADC0->ANm12(AN012);
    mADC0->ANm13(AN013);
    mADC0->ANm20(AN020);
    mADC0->ANm21(AN021);
    mADC0->ANm22(AN022);
    mADC0->ANm23(AN023);
    mADC0->ANm30(AN030);
    mADC0->ANm31(AN031);
    mADC0->ANm32(AN032);
    mADC0->ANm33(AN033);
    mADC0->ANm40(AN040);
    mADC0->ANm41(AN041);
    mADC0->ANm42(AN042);
    mADC0->ANm43(AN043);
#if 0
    /// SARAD4 add
    //mADC0->ANm50(AN050);
    //mADC0->ANm51(AN051);
    //mADC0->ANm52(AN052);
    //mADC0->ANm53(AN053);
    //mADC0->ANm60(AN060);
    //mADC0->ANm61(AN061);
    //mADC0->ANm62(AN062);
    //mADC0->ANm63(AN063);
    //mADC0->ANm70(AN070);
    //mADC0->ANm71(AN071);
    //mADC0->ANm72(AN072);
    //mADC0->ANm73(AN073);
#else
    //// For RH850/U2A
    mADC0->ANm50(VMON_VCC);
    mADC0->ANm51(VMON_E0VCC);
    mADC0->ANm52(VMON_ISOVDD);
    mADC0->ANm53(VMON_AWOVDD);
    mADC0->ANm60(AN050);
    mADC0->ANm61(AN051);
    mADC0->ANm62(AN052);
    mADC0->ANm63(AN053);
    mADC0->ANm70(AN060);
    mADC0->ANm71(AN061);
    mADC0->ANm72(AN062);
    mADC0->ANm73(AN063);
    mADC0->ANm80(AN070);
    mADC0->ANm81(AN071);
#endif
    mADC0->ADmSG0TRG(AD0SG0TRG);
    mADC0->ADmSG1TRG(AD0SG1TRG);
    mADC0->ADmSG2TRG(AD0SG2TRG);
    mADC0->ADmSG3TRG(AD0SG3TRG);
    mADC0->ADmSG4TRG(AD0SG4TRG);
    mADC0->ADmTAUD2I7(ADC0_sig_stubs);
    mADC0->ADmTAUD2I15(ADC0_sig_stubs);
    mADC0->ADmTAUJ2(ADC0_sig_stubs);
    mADC0->ADmTAUJ3(ADC0_sig_stubs);
    mADC0->ADmLPS(ADC0_sig_stubs);

    mADC0->SV_MODEm(SV_MODE);

    //output port
    mADC0->INT_ADm0(INT_AD00);
    mADC0->INT_ADm1(INT_AD01);
    mADC0->INT_ADm2(INT_AD02);
    mADC0->INT_ADm3(INT_AD03);
    mADC0->INT_ADm4(INT_AD04);
    mADC0->INT_MPXm(INT_MPX0);
#if 0
    mADC0->INT_ADEm(INT_ADE0);
    mADC0->INT_ULm(INT_UL0);
#else
    mADC0->INT_ERRm(INT_ERR0);
#endif
    mADC0->ADPEm(ADPE0);

    mADC0->ADENDm0(ADEND00);
    mADC0->ADENDm1(ADEND01);
    mADC0->ADENDm2(ADEND02);
    mADC0->ADENDm3(ADEND03);
    mADC0->ADENDm4(ADEND04);

    // RH850_E2x added
    mADC0->ADCm_IFDATA(ADC0_IFDATA);
    mADC0->IFVCHm(IFVCH0);
    mADC0->IFRDm(IFRD0);
    mADC0->IFREm(IFRE0);
    mADC0->DFREQm(DFREQ0);
    mADC0->DFTAGm(DFTAG0);
    mADC0->DFDATAm(DFDATA0);

    for (unsigned int i = 0; i < emVCRnum_Max; i++) {
        mADC0->VCEND[i]->bind(*VCEND0[i]);
    }
    mADC0->VCULMO(VCULMO0);
    mADC0->VCLLMO(VCLLMO0);
    mADC0->PWM_VCEND(PWM_VCEND0);
    mADC0->PVCR_PWDDRm(PVCR_PWDDR0);
    mADC0->PVCR_ENDm(PVCR_END0);
    mADC0->PVCR_TRGm(PVCR_TRG0);
    mADC0->PVCR_VALUEm(PVCR_VALUE0);
    mADC0->MPXCURm(MPXCUR0);
    mADC0->ULE_LPSm(ULE_LPS0);

    // ADC1
    //input port
    mADC1->ANm00(AN100);
    mADC1->ANm01(AN101);
    mADC1->ANm02(AN102);
    mADC1->ANm03(AN103);
    mADC1->ANm10(AN110);
    mADC1->ANm11(AN111);
    mADC1->ANm12(AN112);
    mADC1->ANm13(AN113);
    mADC1->ANm20(AN120);
    mADC1->ANm21(AN121);
    mADC1->ANm22(AN122);
    mADC1->ANm23(AN123);
    mADC1->ANm30(AN130);
    mADC1->ANm31(AN131);
    mADC1->ANm32(AN132);
    mADC1->ANm33(AN133);
    mADC1->ANm40(AN140);
    mADC1->ANm41(AN141);
    mADC1->ANm42(AN142);
    mADC1->ANm43(AN143);
    mADC1->ANm50(AN150);
    mADC1->ANm51(AN151);
    mADC1->ANm52(AN152);
    mADC1->ANm53(AN153);
    mADC1->ANm60(AN160);
    mADC1->ANm61(AN161);
    mADC1->ANm62(AN162);
    mADC1->ANm63(AN163);
    mADC1->ANm70(AN170);
    mADC1->ANm71(AN171);
    mADC1->ANm72(AN172);
    mADC1->ANm73(AN173);
    mADC1->ANm80(AN180);
    mADC1->ANm81(AN181);

    mADC1->ADmSG0TRG(AD1SG0TRG);
    mADC1->ADmSG1TRG(AD1SG1TRG);
    mADC1->ADmSG2TRG(AD1SG2TRG);
    mADC1->ADmSG3TRG(AD1SG3TRG);
    mADC1->ADmSG4TRG(AD1SG4TRG);
    mADC1->ADmTAUD2I7(ADC1_sig_stubs);
    mADC1->ADmTAUD2I15(ADC1_sig_stubs);
    mADC1->ADmTAUJ2(ADC1_sig_stubs);
    mADC1->ADmTAUJ3(ADC1_sig_stubs);
    mADC1->ADmLPS(ADC1_sig_stubs);

    mADC1->SV_MODEm(SV_MODE);

    //output port
    mADC1->INT_ADm0(INT_AD10);
    mADC1->INT_ADm1(INT_AD11);
    mADC1->INT_ADm2(INT_AD12);
    mADC1->INT_ADm3(INT_AD13);
    mADC1->INT_ADm4(INT_AD14);
    mADC1->INT_MPXm(INT_MPX1);
#if 0
    mADC1->INT_ADEm(INT_ADE1);
    mADC1->INT_ULm(INT_UL1);
#else
    mADC1->INT_ERRm(INT_ERR1);
#endif
    mADC1->ADPEm(ADPE1);

    mADC1->ADENDm0(ADEND10);
    mADC1->ADENDm1(ADEND11);
    mADC1->ADENDm2(ADEND12);
    mADC1->ADENDm3(ADEND13);
    mADC1->ADENDm4(ADEND14);

    // RH850_E2x added
    mADC1->ADCm_IFDATA(ADC1_IFDATA);
    mADC1->IFVCHm(IFVCH1);
    mADC1->IFRDm(IFRD1);
    mADC1->IFREm(IFRE1);
    mADC1->DFREQm(DFREQ1);
    mADC1->DFTAGm(DFTAG1);
    mADC1->DFDATAm(DFDATA1);

    for (unsigned int i = 0; i < emVCRnum_Max; i++) {
        mADC1->VCEND[i]->bind(*VCEND1[i]);
    }
    mADC1->VCULMO(VCULMO1);
    mADC1->VCLLMO(VCLLMO1);
    mADC1->PWM_VCEND(PWM_VCEND1);
    mADC1->PVCR_PWDDRm(PVCR_PWDDR1);
    mADC1->PVCR_ENDm(PVCR_END1);
    mADC1->PVCR_TRGm(PVCR_TRG1);
    mADC1->PVCR_VALUEm(PVCR_VALUE1);
    mADC1->MPXCURm(MPXCUR1);
    mADC1->ULE_LPSm(ULE_LPS1);

    // ADC2
    //input port
    mADC2->ANm00(AN200);
    mADC2->ANm01(AN201);
    mADC2->ANm02(AN202);
    mADC2->ANm03(AN203);
    mADC2->ANm10(AN210);
    mADC2->ANm11(AN211);
    mADC2->ANm12(AN212);
    mADC2->ANm13(AN213);
    mADC2->ANm20(AN220);
    mADC2->ANm21(AN221);
    mADC2->ANm22(AN222);
    mADC2->ANm23(AN223);
    mADC2->ANm30(AN230);
    mADC2->ANm31(AN231);
    mADC2->ANm32(AN232);
    mADC2->ANm33(AN233);
    mADC2->ANm40(AN240);
    mADC2->ANm41(AN241);
    mADC2->ANm42(AN242);
    mADC2->ANm43(AN243);
    mADC2->ANm50(AN250);
    mADC2->ANm51(AN251);
    mADC2->ANm52(AN252);
    mADC2->ANm53(AN253);
    mADC2->ANm60(AN260);
    mADC2->ANm61(AN261);
    mADC2->ANm62(AN262);
    mADC2->ANm63(AN263);
    mADC2->ANm70(AN270);
    mADC2->ANm71(AN271);
    mADC2->ANm72(const_double);
    mADC2->ANm73(const_double);
    mADC2->ANm80(const_double);
    mADC2->ANm81(const_double);

    mADC2->ADmSG0TRG(AD2SG0TRG);
    mADC2->ADmSG1TRG(AD2SG1TRG);
    mADC2->ADmSG2TRG(AD2SG2TRG);
    mADC2->ADmSG3TRG(AD2SG3TRG);
    mADC2->ADmSG4TRG(AD2SG4TRG);
    mADC2->ADmTAUD2I7(AD2TAUD2I7);
    mADC2->ADmTAUD2I15(AD2TAUD2I15);
    mADC2->ADmTAUJ2(AD2TAUJ2);
    mADC2->ADmTAUJ3(AD2TAUJ3);
    mADC2->ADmLPS(AD2LPS);

    mADC2->ADCm_IFDATA(ADC2_IFDATA);
    mADC2->IFVCHm(IFVCH2);
    mADC2->IFRDm(IFRD2);
    mADC2->IFREm(IFRE2);

    mADC2->SV_MODEm(SV_MODE);

    //output port
    mADC2->INT_ERRm(INT_ERR2);
    mADC2->INT_ADm0(INT_AD20);
    mADC2->INT_ADm1(INT_AD21);
    mADC2->INT_ADm2(INT_AD22);
    mADC2->INT_ADm3(INT_AD23);
    mADC2->INT_ADm4(INT_AD24);
    mADC2->INT_MPXm(INT_MPX2);
    mADC2->ADPEm(ADPE2);

    mADC2->ADENDm0(ADEND20);
    mADC2->ADENDm1(ADEND21);
    mADC2->ADENDm2(ADEND22);
    mADC2->ADENDm3(ADEND23);
    mADC2->ADENDm4(ADEND24);

    mADC2->DFREQm(DFREQ2);
    mADC2->DFTAGm(DFTAG2);
    mADC2->DFDATAm(DFDATA2);

    for (unsigned int i = 0; i < emVCRnum_Max; i++) {
        mADC2->VCEND[i]->bind(*VCEND2[i]);
    }
    mADC2->VCULMO(VCULMO2);
    mADC2->VCLLMO(VCLLMO2);
    mADC2->PWM_VCEND(PWM_VCEND2);
    mADC2->PVCR_PWDDRm(PVCR_PWDDR2);
    mADC2->PVCR_ENDm(PVCR_END2);
    mADC2->PVCR_TRGm(PVCR_TRG2);
    mADC2->PVCR_VALUEm(PVCR_VALUE2);
    mADC2->MPXCURm(MPXCUR2);
    mADC2->ULE_LPSm(ULE_LPS2);
}//}}}

// SARAD METHOD

void Csarad_u2a::InitialAVREFH0Method(void)
{//{{{
    if(!mIsRefVol0Update){
        mIsRefVol0Update = true;
        Avrefh0_Val = (double)(AVREFH0.read());
        this->mADC0->mAvrefh = Avrefh0_Val;
    }
}//}}}

void Csarad_u2a::InitialAVREFH1Method(void)
{//{{{
    if(!mIsRefVol1Update){
        mIsRefVol1Update = true;
        Avrefh1_Val = (double)(AVREFH1.read());
        this->mADC1->mAvrefh = Avrefh1_Val;
    }
}//}}}

void Csarad_u2a::InitialAVREFH2Method(void)
{//{{{
    if(!mIsRefVol2Update){
        mIsRefVol2Update = true;
        Avrefh2_Val = (double)(AVREFH2.read());
        this->mADC2->mAvrefh = Avrefh2_Val;
    }
}//}}}

void Csarad_u2a::AVREFH0Method(void)
{//{{{
    if(!(mPResetFlag || mSARPortResetFlag || mSARCmdPResetFlag || mSARCmdADResetFlag)){
        double current_time = sc_time_stamp().to_double();
        if(current_time > 0){
            re_printf("warning", "Changing reference voltage Avrefh0 is invalid when simulation time is greater than zero.\n");
        }
    }
}//}}}

void Csarad_u2a::AVREFH1Method(void)
{//{{{
    if(!(mPResetFlag || mSARPortResetFlag || mSARCmdPResetFlag || mSARCmdADResetFlag)){
        double current_time = sc_time_stamp().to_double();
        if(current_time > 0){
            re_printf("warning", "Changing reference voltage Avrefh1 is invalid when simulation time is greater than zero.\n");
        }
    }
}//}}}

void Csarad_u2a::AVREFH2Method(void)
{//{{{
    if(!(mPResetFlag2 || mSARPortResetFlag2 || mSARCmdPResetFlag2 || mSARCmdADResetFlag2)){
        double current_time = sc_time_stamp().to_double();
        if(current_time > 0){
            re_printf("warning", "Changing reference voltage Avrefh2 is invalid when simulation time is greater than zero.\n");
        }
    }
}//}}}

void Csarad_u2a::SARADstateMethod(void)
{//{{{
#if 0
    if(!(mPResetFlag || mSARPortResetFlag || mSARCmdPResetFlag || mSARCmdADResetFlag)){
        if((mADC0->mADCChangeState || mADC1->mADCChangeState) && (mSARADState == emIDLE)) {
            re_printf("info", "SARAD moves from IDLE state to OPERATING state.\n");
            mSARADState = emOPERATING;
        }
        else if((!(mADC0->mADCChangeState || mADC1->mADCChangeState))&&(mSARADState == emOPERATING)) {
            re_printf("info", "SARAD moves from OPERATING state to IDLE state.\n");
            mSARADState = emIDLE;
        }
    }
#endif
}//}}}

void Csarad_u2a::PCLKMethod(void)
{//{{{
    SetCLKfreq("pclk", sc_dt::uint64_to_double(pclk.read()));
}//}}}

void Csarad_u2a::ADCLKMethod(void)
{//{{{
    SetCLKfreq("ADCLK", sc_dt::uint64_to_double(ADCLK.read()));
}//}}}

void Csarad_u2a::CLKA_ADCMethod(void)
{//{{{
    SetCLKfreq("CLKA_ADC", sc_dt::uint64_to_double(CLKA_ADC.read()));
}//}}}

void Csarad_u2a::ADResetMethod(void)
{//{{{
    //1. Reset port sensitive
    bool is_cancel_assertreset = false;
    if(RESETAD_N.read() == vpcl::emResetActive){
        mSARPortResetFlag = true;
    }else{
        mSARPortResetFlag = false;
    }
    if(mSARPortResetFlag){
        mIsADInitialize = false;
        re_printf("info", "RESETAD_N signal is asserted.\n");
        this->EnableReset(mSARPortResetFlag, emAD_RESET);
        is_cancel_assertreset = true;
    }else{
        if(mIsADInitialize){
            mIsADInitialize = false;
            is_cancel_assertreset = false;
        }else{
            re_printf("info", "RESETAD_N signal is de-asserted.\n");
            this->EnableReset(mSARPortResetFlag, emAD_RESET);
            is_cancel_assertreset = true;
        }
    }

    //Cancel AssertReset
    if(is_cancel_assertreset){
        mCmdADResetEvent.cancel();
        mCmdCancelADResetEvent.cancel();
        mIsCmdADResetFlag = false;
        mSARCmdADResetFlag = false;
    }

}//}}}

void Csarad_u2a::ADResetMethod2(void)
{//{{{
    //1. Reset port sensitive
    bool is_cancel_assertreset = false;
    if(RESETAD2_N.read() == vpcl::emResetActive){
        mSARPortResetFlag2 = true;
    }else{
        mSARPortResetFlag2 = false;
    }
    if(mSARPortResetFlag2){
        mIsADInitialize2 = false;
        re_printf("info", "RESETAD2_N signal is asserted.\n");
        this->EnableReset(mSARPortResetFlag2, emAD_RESET2);
        is_cancel_assertreset = true;
    }else{
        if(mIsADInitialize2){
            mIsADInitialize2 = false;
            is_cancel_assertreset = false;
        }else{
            re_printf("info", "RESETAD2_N signal is de-asserted.\n");
            this->EnableReset(mSARPortResetFlag, emAD_RESET2);
            is_cancel_assertreset = true;
        }
    }

    //Cancel AssertReset
    if(is_cancel_assertreset){
        mCmdADResetEvent2.cancel();
        mCmdCancelADResetEvent2.cancel();
        mIsCmdADResetFlag2 = false;
        mSARCmdADResetFlag2 = false;
    }

}//}}}

void Csarad_u2a::PResetMethod(void)
{//{{{
    //1. Reset port sensitive
    bool is_cancel_assertreset = false;
    if(preset_n.read() == vpcl::emResetActive){
        mPResetFlag = true;
    }else{
        mPResetFlag = false;
    }
    if(mPResetFlag){
        mIsPInitialize = false;
        re_printf("info", "preset_n signal is asserted.\n");
        this->EnableReset(mPResetFlag, emP_RESET);
        is_cancel_assertreset = true;
    }else{
        if(mIsPInitialize){
            mIsPInitialize = false;
            is_cancel_assertreset = false;
        }else{
            re_printf("info", "preset_n signal is de-asserted.\n");
            this->EnableReset(mPResetFlag, emP_RESET);
            is_cancel_assertreset = true;
        }
    }

    //Cancel AssertReset
    if(is_cancel_assertreset){
        mCmdPResetEvent.cancel();
        mCmdCancelPResetEvent.cancel();
        mIsCmdPResetFlag = false;
        mSARCmdPResetFlag = false;
    }
}//}}}

void Csarad_u2a::PResetMethod2(void)
{//{{{
    //1. Reset port sensitive
    bool is_cancel_assertreset = false;
    if(preset2_n.read() == vpcl::emResetActive){
        mPResetFlag2 = true;
    }else{
        mPResetFlag2 = false;
    }
    if(mPResetFlag2){
        mIsPInitialize2 = false;
        re_printf("info", "preset2_n signal is asserted.\n");
        this->EnableReset(mPResetFlag2, emP_RESET2);
        is_cancel_assertreset = true;
    }else{
        if(mIsPInitialize2){
            mIsPInitialize2 = false;
            is_cancel_assertreset = false;
        }else{
            re_printf("info", "preset2_n signal is de-asserted.\n");
            this->EnableReset(mPResetFlag2, emP_RESET2);
            is_cancel_assertreset = true;
        }
    }

    //Cancel AssertReset
    if(is_cancel_assertreset){
        mCmdPResetEvent2.cancel();
        mCmdCancelPResetEvent2.cancel();
        mIsCmdPResetFlag2 = false;
        mSARCmdPResetFlag2 = false;
    }
}//}}}

void Csarad_u2a::AssertPResetMethod(void)
{//{{{
    mSARCmdPResetFlag = true;
    this->EnableReset(mSARCmdPResetFlag, emP_RESET);
    mCmdCancelPResetEvent.notify((unsigned int)mPResetPeriod, SC_NS);
}//}}}

void Csarad_u2a::AssertPResetMethod2(void)
{//{{{
    mSARCmdPResetFlag2 = true;
    this->EnableReset(mSARCmdPResetFlag2, emP_RESET2);
    mCmdCancelPResetEvent2.notify((unsigned int)mPResetPeriod2, SC_NS);
}//}}}

void Csarad_u2a::DeAssertPResetMethod(void)
{//{{{
    mSARCmdPResetFlag = false;
    mIsCmdPResetFlag = false;
    this->EnableReset(mSARCmdPResetFlag, emP_RESET);
}//}}}

void Csarad_u2a::DeAssertPResetMethod2(void)
{//{{{
    mSARCmdPResetFlag2 = false;
    mIsCmdPResetFlag2 = false;
    this->EnableReset(mSARCmdPResetFlag2, emP_RESET2);
}//}}}

void Csarad_u2a::AssertADResetMethod(void)
{//{{{
    mSARCmdADResetFlag = true;
    this->EnableReset(mSARCmdADResetFlag, emAD_RESET);
    mCmdCancelADResetEvent.notify((unsigned int)mADResetPeriod, SC_NS);
}//}}}

void Csarad_u2a::AssertADResetMethod2(void)
{//{{{
    mSARCmdADResetFlag2 = true;
    this->EnableReset(mSARCmdADResetFlag2, emAD_RESET2);
    mCmdCancelADResetEvent2.notify((unsigned int)mADResetPeriod2, SC_NS);
}//}}}

void Csarad_u2a::DeAssertADResetMethod(void)
{//{{{
    mSARCmdADResetFlag = false;
    mIsCmdADResetFlag = false;
    this->EnableReset(mSARCmdADResetFlag, emAD_RESET);
}//}}}

void Csarad_u2a::DeAssertADResetMethod2(void)
{//{{{
    mSARCmdADResetFlag2 = false;
    mIsCmdADResetFlag2 = false;
    this->EnableReset(mSARCmdADResetFlag2, emAD_RESET2);
}//}}}

void Csarad_u2a::SVModeMethod(void)
{//{{{
    if(!(mPResetFlag || mSARPortResetFlag || mSARCmdPResetFlag || mSARCmdADResetFlag)){
#if 0
        if(mADC0->mEnterSVModeFlag || mADC1->mEnterSVModeFlag) {
            re_printf("info", "SARAD entered Supervisor Mode.\n");
        }
        if(mADC0->mExitSVModeFlag || mADC1->mExitSVModeFlag) {
            re_printf("info", "SARAD exited from Supervisor Mode.\n");
        }
#else
        // RH850_U2A added
        if(mADC0->mEnterSVModeFlag) {
            re_printf("info", "ADC0 entered Supervisor Mode.\n");
        }
        if(mADC1->mEnterSVModeFlag) {
            re_printf("info", "ADC1 entered Supervisor Mode.\n");
        }
        if(mADC0->mExitSVModeFlag) {
            re_printf("info", "ADC0 exited from Supervisor Mode.\n");
        }
        if(mADC1->mExitSVModeFlag) {
            re_printf("info", "ADC1 exited from Supervisor Mode.\n");
        }
#endif
    }
    if(!(mPResetFlag2 || mSARPortResetFlag2 || mSARCmdPResetFlag2 || mSARCmdADResetFlag2)){
        if(mADC2->mEnterSVModeFlag) {
            re_printf("info", "ADC2 entered Supervisor Mode.\n");
        }
        if(mADC2->mExitSVModeFlag) {
            re_printf("info", "ADC2 exited from Supervisor Mode.\n");
        }
    }
}//}}}

//Function
//tlm_tgt_if function

void Csarad_u2a::tgt_acc (unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t)
{ //{{{
    //Get information
    tlm::tlm_command command;
    sc_dt::uint64 addr    = 0;
    unsigned char *p_data  = NULL;
    unsigned int size = 0;
    bool status = this->tgt_get_gp_attribute(command, addr, p_data, size, trans, false);
    if(!status){
        trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
        return;
    }
    sc_assert(p_data != NULL);

    status = AddressArbitrate(id, command, false, (unsigned int)addr, p_data, size);
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
} //}}}

unsigned int Csarad_u2a::tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans)
{//{{{
    //Get information
    tlm::tlm_command command;
    sc_dt::uint64 addr = 0;
    unsigned char *p_data = NULL;
    unsigned int size = 0;
    bool status = this->tgt_get_gp_attribute(command, addr, p_data, size, trans, true);
    sc_assert(p_data != NULL);

    status = AddressArbitrate(id, command, true, (unsigned int)addr, p_data, size);
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    if (status){
        return size;
    } else {
        return 0;
    }
}//}}}

bool Csarad_u2a::AddressArbitrate (const unsigned int id, tlm::tlm_command command, const bool is_dbg, const unsigned int addr, unsigned char *p_data, const unsigned int size)
{//{{{
    //1. Check data
    sc_assert(p_data != NULL);

    //2. Read & Write process
    unsigned int tmp_addr = addr & 0xFFF;
    if (id == emADC0Id) {
        //2.1 Check address for ADSYNSTCR, ADTSYNSTRC, RDRVR
        if (((0x610 <= tmp_addr) && (tmp_addr <= 0x614)) || ((0x740 <= tmp_addr) && (tmp_addr <= 0x744))) {
            if(command == tlm::TLM_WRITE_COMMAND){  //write process
                if (!is_dbg) {
                    return reg_wr(addr, p_data, size);
                } else {
                    return reg_wr_dbg(addr, p_data, size);
                }
            }else if (command == tlm::TLM_READ_COMMAND){  //read process
                if (!is_dbg) {
                    return reg_rd(addr, p_data, size);
                } else {
                    return reg_rd_dbg(addr, p_data, size);
                }
            }else{
                return true;
            }
        } else {
            if(command == tlm::TLM_WRITE_COMMAND){
                return mADC0->common_tgt_wr(is_dbg, addr, p_data, size);
            }else if(command == tlm::TLM_READ_COMMAND){
                return mADC0->common_tgt_rd(is_dbg, addr, p_data, size);
            }else{
                return true;
            }
        }
    } else if (id == emADC1Id) {
        //2.3 Check address for ADC1
        if(command == tlm::TLM_WRITE_COMMAND){
            return mADC1->common_tgt_wr(is_dbg, addr, p_data, size);
        }else if(command == tlm::TLM_READ_COMMAND){
            return mADC1->common_tgt_rd(is_dbg, addr, p_data, size);
        }else{
            return true;
        }
    } else {
        //2.6 Check address for ADC2
        if(command == tlm::TLM_WRITE_COMMAND){
            return mADC2->common_tgt_wr(is_dbg, addr, p_data, size);
        }else if(command == tlm::TLM_READ_COMMAND){
            return mADC2->common_tgt_rd(is_dbg, addr, p_data, size);
        }else{
            return true;
        }
    }
}//}}}

//Call_back function
void Csarad_u2a::cb_ADSYNSTCR_ADSTART(RegCBstr str)
{//{{{
    if((*ADSYNSTCR)["ADSTART"] == 1){
        (*ADSYNSTCR) = 0;
        mADC0->WritingADSYNSTCR();
        mADC1->WritingADSYNSTCR();
    }
}//}}}

void Csarad_u2a::cb_ADTSYNSTCR_ADTSTART(RegCBstr str)
{//{{{
    if((*ADTSYNSTCR)["ADTSTART"] == 1){
        (*ADTSYNSTCR) = 0;
        mADC0->WritingADTSYNSTCR();
        mADC1->WritingADTSYNSTCR();
    }
}//}}}

void Csarad_u2a::cb_VMONVDCR1_VDE1(RegCBstr str)
{//{{{
    if (str.data != str.pre_data) {
        if (!GetAllSGStopCheck()) {
            re_printf("warning", "Writing into VMONVDCR1 register is not affected.\n");
            (*VMONVDCR1) = str.pre_data;
        } else {
            mADC0->WritingVMONVDCR("VMONVDCR1", "VDE1", (*VMONVDCR1)["VDE1"]);
        }
    }
}//}}}

void Csarad_u2a::cb_VMONVDCR2_VDE2(RegCBstr str)
{//{{{
    if (str.data != str.pre_data) {
        if (!GetAllSGStopCheck()) {
            re_printf("warning", "Writing into VMONVDCR2 register is not affected.\n");
            (*VMONVDCR2) = str.pre_data;
        } else {
            mADC0->WritingVMONVDCR("VMONVDCR2", "VDE2", (*VMONVDCR2)["VDE2"]);
        }
    }
}//}}}

bool Csarad_u2a::GetAllSGStopCheck(void) 
{//{{{
    if ((mADC0->AllSGStopCheck()) && (mADC1->AllSGStopCheck()) && (mADC2->AllSGStopCheck())) {
        return true;
    } else {
        return false;
    }
}//}}}

//Reset function
void Csarad_u2a::EnableReset(const bool is_active, const unsigned int port_reset)
{//{{{
    // TBD
#if 0
    if(port_reset == emAD_RESET || port_reset == emALL_RESET){
        if(is_active){
            //1.Dump report
            if(mSARADState == emIDLE){
                re_printf("info", "SARAD moves from IDLE state to RESET state.\n");
            }else if(mSARADState == emOPERATING){
                re_printf("info", "SARAD moves from OPERATING state to RESET state.\n");
            }

            //2.Global variable init
            mSARADState = emRESET;
            re_printf("info", "The ADC0 and ADC1 are reset.\n");

            //3. Cancel all event
            mADC0->mADCChangeStateEvent.cancel();
            mADC1->mADCChangeStateEvent.cancel();
        }else{
            re_printf("info", "SARAD moves from RESET state to IDLE state.\n");
            mSARADState =  emIDLE;
        }
    }
#else
    // RH850_U2A added
    if (is_active) {
        if (port_reset == emAD_RESET) {
            re_printf("info", "The ADC0 and ADC1 are reset.\n");
            //3. Cancel all event
            mADC0->mADCChangeStateEvent.cancel();
            mADC1->mADCChangeStateEvent.cancel();
        }
        if (port_reset == emAD_RESET2) {
            re_printf("info", "The ADC2 is reset.\n");
            mADC2->mADCChangeStateEvent.cancel();
        }
    }
#endif
    if(port_reset == emP_RESET){
        //4.Reset registers
        Csarad_u2a_regif::EnableReset(is_active);
    }

    if (port_reset == emP_RESET || port_reset == emAD_RESET) {
        //5.Reset ADC0 and ADC1
        mADC0->EnableReset(is_active, port_reset);
        mADC1->EnableReset(is_active, port_reset);
    }
    if (port_reset == emP_RESET2 || port_reset == emAD_RESET2) {
        //6.Reset ADC2
        mADC2->EnableReset(is_active, port_reset);
    }
}//}}}

//handleCommand function
void Csarad_u2a::DumpStatInfo(void)
{//{{{
    mADC0->DumpStatInfo();
    mADC1->DumpStatInfo();
    // RH850_U2A added
    mADC2->DumpStatInfo();
}//}}}

void Csarad_u2a::AssertPReset(const double start_time, const double period)
{//{{{
    if((!mPResetFlag)&&(!mIsCmdPResetFlag)) {
        mIsCmdPResetFlag = true;
        re_printf("info", "PReset will reset for %lld.%06d ns after %lld.%06d ns.\n", (unsigned long long)period, FRACTION_TO_INT(period), (unsigned long long)start_time, FRACTION_TO_INT(start_time));
        mCmdPResetEvent.notify(start_time, SC_NS);
        mPResetPeriod = (unsigned int)period;
    }else{
        re_printf("warning", "PReset is in progress.\n");
    }
}//}}}

// RH850_U2A added
void Csarad_u2a::AssertPReset2(const double start_time, const double period)
{//{{{
    if((!mPResetFlag2)&&(!mIsCmdPResetFlag2)) {
        mIsCmdPResetFlag2 = true;
        re_printf("info", "PReset for ADC2 will reset for %lld.%06d ns after %lld.%06d ns.\n", (unsigned long long)period, FRACTION_TO_INT(period), (unsigned long long)start_time, FRACTION_TO_INT(start_time));
        mCmdPResetEvent2.notify(start_time, SC_NS);
        mPResetPeriod2 = (unsigned int)period;
    }else{
        re_printf("warning", "PReset for ADC2 is in progress.\n");
    }
}//}}}

void Csarad_u2a::AssertADReset(const double start_time, const double period)
{//{{{
    if((!mSARPortResetFlag)&&(!mIsCmdADResetFlag)) {
        mIsCmdADResetFlag = true;
        re_printf("info", "ADReset will reset for %lld.%06d ns after %lld.%06d ns.\n", (unsigned long long)period, FRACTION_TO_INT(period), (unsigned long long)start_time, FRACTION_TO_INT(start_time));
        mCmdADResetEvent.notify(start_time, SC_NS);
        mADResetPeriod = (unsigned int)period;
    }else{
        re_printf("warning", "ADReset is in progress.\n");
    }
}//}}}

// RH850_U2A added
void Csarad_u2a::AssertADReset2(const double start_time, const double period)
{//{{{
    if((!mSARPortResetFlag2)&&(!mIsCmdADResetFlag2)) {
        mIsCmdADResetFlag2 = true;
        re_printf("info", "ADReset for ADC2 will reset for %lld.%06d ns after %lld.%06d ns.\n", (unsigned long long)period, FRACTION_TO_INT(period), (unsigned long long)start_time, FRACTION_TO_INT(start_time));
        mCmdADResetEvent2.notify(start_time, SC_NS);
        mADResetPeriod2 = (unsigned int)period;
    }else{
        re_printf("warning", "ADReset for ADC2 is in progress.\n");
    }
}//}}}

void Csarad_u2a::AssignTHCircuit(const unsigned int adc_index, const unsigned int th_channel, const unsigned int group_channel, const unsigned int physic_channel)
{//{{{
    if (adc_index == 0) {
        mADC0->AssignTHCircuit(th_channel, group_channel, physic_channel);
    } else if (adc_index == 1) {
        mADC1->AssignTHCircuit(th_channel, group_channel, physic_channel);
    } else {
        re_printf("warning", "adc_index %d is invalid for AssignTHCircuit.\n", adc_index);
        // TBD adc_index = 2 and other
    }
}//}}}

double Csarad_u2a::GetTimeResolution(void)
{//{{{
    double tu_value = 0;
    if (sc_get_time_resolution() < sc_time(1, SC_PS)){
        tu_value = 1000000000000000LL;
    } else if (sc_get_time_resolution() < sc_time(1, SC_NS)){
        tu_value = 1000000000000LL;
    } else if (sc_get_time_resolution() < sc_time(1, SC_US)){
        tu_value = 1000000000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_MS)){
        tu_value = 1000000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)){
        tu_value = 1000;
    } else {
        tu_value = 1;
    }
    return tu_value;
}//}}}

void Csarad_u2a::SetCLKfreq(std::string clk_name, double clk_freq)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    double time_unit = 0;
    double pclk_period = 0;
    double adclk_period = 0;
    double clka_adc_period = 0;
    bool is_period_valid = true;

    // Setting clock values
    if (clk_name == "pclk") {
        if (this->mADC0->mADCChangeState || this->mADC1->mADCChangeState) {
            re_printf("warning", "The frequency of pclk clock is changed while SARAD model is operating.\n");
        }

        if (clk_freq == 0) {
            pclk_period = 0;
        } else {
            pclk_period  = (double)(emNanoSecond / clk_freq);
            time_unit    = (double)(emNanoSecond / GetTimeResolution());
            if (time_unit > pclk_period) {
                is_period_valid = false;
                re_printf("warning", "The pclk period is less than 1 unit time of system.\n");
            } else if (mADclkPeriod != 0) {
                if(pclk_period < 10
                || (mPreADclkPeriod != pclk_period &&
                    mPreADclkPeriod != 2 * pclk_period)) {
                    is_period_valid = false;
                    re_printf("warning", "The pclk period is less than 10 ns (> 100MHz) or not ratio with clkad (1:1 or 2:1).\n");
                }
            }

        }

        if (is_period_valid) {
            mPclkPeriod = pclk_period;
            this->mADC0->mPCLKPeriod = pclk_period;
            this->mADC1->mPCLKPeriod = pclk_period;
            this->mADC0->mADCLKPeriod = mPreADclkPeriod;
            this->mADC1->mADCLKPeriod = mPreADclkPeriod;
            SetLatency_TLM(emADC0Id, pclk_period, false);
            SetLatency_TLM(emADC1Id, pclk_period, false);
            re_printf("info", "The pclk is set with a frequency as %lld.%06d\n", (unsigned long long)(clk_freq), FRACTION_TO_INT(clk_freq));
        }
        mPrePclkPeriod = pclk_period;
    } else if (clk_name == "ADCLK") {
        if (this->mADC0->mADCChangeState || this->mADC1->mADCChangeState) {
            re_printf("warning", "The frequency of ADCLK clock is changed while SARAD model is operating.\n");
        }

        if (clk_freq == 0) {
            adclk_period = 0;
        } else {
            adclk_period = (double)(emNanoSecond / clk_freq);
            time_unit    = (double)(emNanoSecond / GetTimeResolution());
            if (time_unit > adclk_period) {
                is_period_valid = false;
                re_printf("warning", "The ADCLK period is less than 1 unit time of system.\n");
            }else if(mPclkPeriod != 0) {
                if(adclk_period < 10
                || (adclk_period != mPrePclkPeriod &&
                    adclk_period != 2 * mPrePclkPeriod )) {
                    is_period_valid = false;
                    re_printf("warning", "The ADCLK period is less than 10 ns (> 100MHz) or not ratio with pclk (1:1 or 1:2).\n");
                }
            }
        }

        if (is_period_valid) {
            mADclkPeriod = adclk_period;
            this->mADC0->mADCLKPeriod = adclk_period;
            this->mADC1->mADCLKPeriod = adclk_period;
            this->mADC0->mPCLKPeriod = mPrePclkPeriod;
            this->mADC1->mPCLKPeriod = mPrePclkPeriod;
            SetLatency_TLM(emADC0Id, mPrePclkPeriod, false);
            SetLatency_TLM(emADC1Id, mPrePclkPeriod, false);
            re_printf("info", "The ADCLK is set with a frequency as %lld.%06d\n", (unsigned long long)(clk_freq), FRACTION_TO_INT(clk_freq));
        }
        mPreADclkPeriod = adclk_period;
    // RH850_U2A added
    } else if (clk_name == "CLKA_ADC") {
        if (this->mADC2->mADCChangeState) {
            re_printf("warning", "The frequency of CLKA_ADC clock is changed while SARAD model is operating.\n");
        }
        if (clk_freq == 0) {
            clka_adc_period = 0;
        } else {
            clka_adc_period  = (double)(emNanoSecond / clk_freq);
            time_unit        = (double)(emNanoSecond / GetTimeResolution());
            if (time_unit > clka_adc_period) {
                is_period_valid = false;
                re_printf("warning", "The CLKA_ADC period is less than 1 unit time of system.\n");
            }
        }

        if (is_period_valid) {
            this->mADC2->mPCLKPeriod = clka_adc_period;
            this->mADC2->mADCLKPeriod = clka_adc_period;
            SetLatency_TLM(emADC2Id, clka_adc_period, false);
            re_printf("info", "The CLKA_ADC is set with a frequency as %lld.%06d\n", (unsigned long long)(clk_freq), FRACTION_TO_INT(clk_freq));
        }
    } else {
        re_printf("error", "Clock name is invalid.\n");
    }

    // Stop operation when clock frequency = 0
    if (((clk_name == "pclk") || (clk_name == "ADCLK")) && (clk_freq == 0)) {
        mADC0->SettingZeroClock();
        mADC1->SettingZeroClock();
    }
    // RH850_U2A added
    if ((clk_name == "CLKA_ADC") && (clk_freq == 0)) {
        mADC2->SettingZeroClock();
    }
}//}}}

void Csarad_u2a::SetLatency_TLM(const unsigned int id, const double pclk_period, const bool is_constructor)
{//{{{
    vpcl::tlm_if_tgt_parameter tgt_param = vpcl::tlm_tgt_if<32,tlm::tlm_base_protocol_types,3>::tgt_get_param(id);
    if(is_constructor){
        tgt_param.fw_req_phase = tlm::END_REQ;
    }
    sc_time new_clock(pclk_period, SC_NS);
    tgt_param.bus_clk = new_clock;
    tgt_param.rd_latency = tgt_param.rd_req_latency + 2 * tgt_param.bus_clk;
    tgt_param.wr_latency = tgt_param.wr_req_latency + 2 * tgt_param.bus_clk;
    vpcl::tlm_tgt_if<32,tlm::tlm_base_protocol_types,3>::tgt_set_param(id, tgt_param);
}//}}}

std::string Csarad_u2a::CommandCB(const std::vector<std::string>& args)
{//{{{
    double present_time = sc_time_stamp().to_double();
    std::vector<std::string> _args = args;
    std::string ret = "";

    // Remove the "command" argument. This process allows user to use
    // a handleCommand with/without "command" argument

    if (_args[0] == "command") {
        _args.erase(_args.begin());
    }

    if((_args[0] == "DumpInterrupt")&&((int)(_args.size()) == 2)){
        if (_args[1] == "true") {
            this->mADC0->mDumpInterrupt = true;
            this->mADC1->mDumpInterrupt = true;
            this->mADC2->mDumpInterrupt = true;
        }else if (_args[1] == "false") {
            this->mADC0->mDumpInterrupt = false;
            this->mADC1->mDumpInterrupt = false;
            this->mADC2->mDumpInterrupt = false;
        }
    }else if((_args[0] == "EnableConvertInfo")&&((int)_args.size() == 2)){
        if (_args[1] == "true"){
            this->mADC0->mEnableConvertInfo = true;
            this->mADC1->mEnableConvertInfo = true;
            this->mADC2->mEnableConvertInfo = true;
        }else if (_args[1] == "false"){
            this->mADC0->mEnableConvertInfo = false;
            this->mADC1->mEnableConvertInfo = false;
            this->mADC2->mEnableConvertInfo = false;
        }
    }else if((_args[0] == "Avrefh0_Val")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage Avrefh0_Val is invalid when simulation time is greater than zero.\n");
            Avrefh0_Val = mPreAvrefh0;
        }else{
            mPreAvrefh0 = Avrefh0_Val;
            this->mADC0->mAvrefh = Avrefh0_Val;
            mIsRefVol0Update = true;
        }
    }else if((_args[0] == "Avrefh1_Val")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage Avrefh1_Val is invalid when simulation time is greater than zero.\n");
            Avrefh1_Val = mPreAvrefh1;
        }else{
            mPreAvrefh1 = Avrefh1_Val;
            this->mADC1->mAvrefh = Avrefh1_Val;
            mIsRefVol1Update = true;
        }
    }else if((_args[0] == "Avrefh2_Val")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage Avrefh2_Val is invalid when simulation time is greater than zero.\n");
            Avrefh2_Val = mPreAvrefh2;
        }else{
            mPreAvrefh2 = Avrefh2_Val;
            this->mADC2->mAvrefh = Avrefh2_Val;
            mIsRefVol2Update = true;
        }
    }else if((_args[0] == "AVcc0")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage AVcc0 is invalid when simulation time is greater than zero.\n");
            AVcc0 = mPreAVcc0;
        }else{
            mPreAVcc0 = AVcc0;
            this->mADC0->mAVcc = AVcc0;
        }
    }else if((_args[0] == "AVcc1")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage AVcc1 is invalid when simulation time is greater than zero.\n");
            AVcc1 = mPreAVcc1;
        }else{
            mPreAVcc1 = AVcc1;
            this->mADC1->mAVcc = AVcc1;
        }
    }else if((_args[0] == "AVcc2")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage AVcc2 is invalid when simulation time is greater than zero.\n");
            AVcc2 = mPreAVcc2;
        }else{
            mPreAVcc2 = AVcc2;
            this->mADC2->mAVcc = AVcc2;
        }
    }else if((_args[0] == "EVcc0")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage EVcc0 is invalid when simulation time is greater than zero.\n");
            EVcc0 = mPreEVcc0;
        }else{
            mPreEVcc0 = EVcc0;
            this->mADC0->mEVcc = EVcc0;
        }
    }else if((_args[0] == "EVcc1")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage EVcc1 is invalid when simulation time is greater than zero.\n");
            EVcc1 = mPreEVcc1;
        }else{
            mPreEVcc1 = EVcc1;
            this->mADC1->mEVcc = EVcc1;
        }
    }else if((_args[0] == "EVcc2")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage EVcc2 is invalid when simulation time is greater than zero.\n");
            EVcc2 = mPreEVcc2;
        }else{
            mPreEVcc2 = EVcc2;
            this->mADC2->mEVcc = EVcc2;
        }
    }else if((_args[0] == "tTSPL")&&((int)_args.size() == 2)){
        if (this->mADC0->mADCChangeState || this->mADC1->mADCChangeState || this->mADC2->mADCChangeState ) {
            re_printf("warning", "Changing the tTSPL parameter is not affected when SARAD model is operating.\n");
            tTSPL = mPretTSPL;
        }else{
            mPretTSPL = tTSPL;
            this->mADC0->mtTSPL = tTSPL;
            this->mADC1->mtTSPL = tTSPL;
            this->mADC2->mtTSPL = tTSPL;
        }
    }else if((_args[0] == "tTHLD")&&((int)_args.size() == 2)){
        if (this->mADC0->mADCChangeState || this->mADC1->mADCChangeState || this->mADC2->mADCChangeState ) {
            re_printf("warning", "Changing the tTHLD parameter is not affected when SARAD model is operating.\n");
            tTHLD = mPretTHLD;
        }else{
            mPretTHLD = tTHLD;
            this->mADC0->mtTHLD = tTHLD;
            this->mADC1->mtTHLD = tTHLD;
            this->mADC2->mtTHLD = tTHLD;
        }
    }else if((_args[0] == "tTHSD")&&((int)_args.size() == 2)){
        if (this->mADC0->mADCChangeState || this->mADC1->mADCChangeState || this->mADC2->mADCChangeState ) {
            re_printf("warning", "Changing the tTHSD parameter is not affected when SARAD model is operating.\n");
            tTHSD = mPretTHSD;
        }else{
            mPretTHSD = tTHSD;
            this->mADC0->mtTHSD = tTHSD;
            this->mADC1->mtTHSD = tTHSD;
            this->mADC2->mtTHSD = tTHSD;
        }
    }else if((_args[0] == "ExSmpt")&&((int)_args.size() == 2)){
        if (this->mADC0->mADCChangeState || this->mADC1->mADCChangeState || this->mADC2->mADCChangeState ) {
            re_printf("warning", "Changing the ExSmpt parameter is not affected when SARAD model is operating.\n");
            ExSmpt = mPreExSmpt;
        }else{
            mPreExSmpt = ExSmpt;
            this->mADC0->mExSmpt = ExSmpt;
            this->mADC1->mExSmpt = ExSmpt;
            this->mADC2->mExSmpt = ExSmpt;
        }
    }else if((_args[0] == "Smpt")&&((int)_args.size() == 2)){
        if (this->mADC0->mADCChangeState || this->mADC1->mADCChangeState || this->mADC2->mADCChangeState ) {
            re_printf("warning", "Changing the Smpt parameter is not affected when SARAD model is operating.\n");
            Smpt = mPreSmpt;
        }else{
            mPreSmpt = Smpt;
            this->mADC0->mSmpt = Smpt;
            this->mADC1->mSmpt = Smpt;
            this->mADC2->mSmpt = Smpt;
        }
    }else if((_args[0] == "tSAR")&&((int)_args.size() == 2)){
        if (this->mADC0->mADCChangeState || this->mADC1->mADCChangeState || this->mADC2->mADCChangeState ) {
            re_printf("warning", "Changing the tSAR parameter is not affected when SARAD model is operating.\n");
            tSAR = mPretSAR;
        }else{
            mPretSAR = tSAR;
            this->mADC0->mtSAR = tSAR;
            this->mADC1->mtSAR = tSAR;
            this->mADC2->mtSAR = tSAR;
        }
    }else if((_args[0] == "tD")&&((int)_args.size() == 2)){
        if (this->mADC0->mADCChangeState || this->mADC1->mADCChangeState || this->mADC2->mADCChangeState ) {
            re_printf("warning", "Changing the tD parameter is not affected when SARAD model is operating.\n");
            tD = mPretD;
        }else{
            mPretD = tD;
            this->mADC0->mtD = tD;
            this->mADC1->mtD = tD;
            this->mADC2->mtD = tD;
        }
    }else if((_args[0] == "tED")&&((int)_args.size() == 2)){
        if (this->mADC0->mADCChangeState || this->mADC1->mADCChangeState || this->mADC2->mADCChangeState ) {
            re_printf("warning", "Changing the tED parameter is not affected when SARAD model is operating.\n");
            tED = mPretED;
        }else{
            mPretED = tED;
            this->mADC0->mtED = tED;
            this->mADC1->mtED = tED;
            this->mADC2->mtED = tED;
        }
    }else if((_args[0] == "ForcedIdError0")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            this->mADC0->mForcedIdError = true;
        }else if (_args[1] == "false") {
            this->mADC0->mForcedIdError = false;
        }
    }else if((_args[0] == "ForcedIdError1")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            this->mADC1->mForcedIdError = true;
        }else if (_args[1] == "false") {
            this->mADC1->mForcedIdError = false;
        }
    }else if((_args[0] == "ForcedIdError2")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            this->mADC2->mForcedIdError = true;
        }else if (_args[1] == "false") {
            this->mADC2->mForcedIdError = false;
        }
    }else if((_args[0] == "ForcedParityError0")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            this->mADC0->mForcedParityError = true;
        }else if (_args[1] == "false") {
            this->mADC0->mForcedParityError = false;
        }
    }else if((_args[0] == "ForcedParityError1")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            this->mADC1->mForcedParityError = true;
        }else if (_args[1] == "false") {
            this->mADC1->mForcedParityError = false;
        }
    }else if((_args[0] == "ForcedParityError2")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            this->mADC2->mForcedParityError = true;
        }else if (_args[1] == "false") {
            this->mADC2->mForcedParityError = false;
        }
    }else if((_args[0] == "ForcedOpenError0")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            this->mADC0->mForcedOpenError = true;
        }else if (_args[1] == "false") {
            this->mADC0->mForcedOpenError = false;
        }
    }else if((_args[0] == "ForcedOpenError1")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            this->mADC1->mForcedOpenError = true;
        }else if (_args[1] == "false") {
            this->mADC1->mForcedOpenError = false;
        }
    }else if((_args[0] == "ForcedOpenError2")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            this->mADC2->mForcedOpenError = true;
        }else if (_args[1] == "false") {
            this->mADC2->mForcedOpenError = false;
        }
    }else if((_args[0] == "ForcedBreak0")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            this->mADC0->mForcedBreak = true;
        }else if (_args[1] == "false") {
            this->mADC0->mForcedBreak = false;
        }
    }else if((_args[0] == "ForcedBreak1")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            this->mADC1->mForcedBreak = true;
        }else if (_args[1] == "false") {
            this->mADC1->mForcedBreak = false;
        }
    }else if((_args[0] == "ForcedBreak2")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            this->mADC2->mForcedBreak = true;
        }else if (_args[1] == "false") {
            this->mADC2->mForcedBreak = false;
        }
    }else if((_args[0] == "ForcedDataPathError0")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            this->mADC0->mForcedDataPathError = true;
        }else if (_args[1] == "false") {
            this->mADC0->mForcedDataPathError = false;
        }
    }else if((_args[0] == "ForcedDataPathError1")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            this->mADC1->mForcedDataPathError = true;
        }else if (_args[1] == "false") {
            this->mADC1->mForcedDataPathError = false;
        }
    }else if((_args[0] == "ForcedDataPathError2")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            this->mADC2->mForcedDataPathError = true;
        }else if (_args[1] == "false") {
            this->mADC2->mForcedDataPathError = false;
        }
    }else if((_args[0] == "EnableTimeCalculation")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing delay times calculation is invalid when simulation time is greater than zero.\n");
            EnableTimeCalculation = mPreEnableTimeCalculation;
        }else{
            mPreEnableTimeCalculation = EnableTimeCalculation;
            this->mADC0->mEnableTimeCalculation = EnableTimeCalculation;
            this->mADC1->mEnableTimeCalculation = EnableTimeCalculation;
            this->mADC2->mEnableTimeCalculation = EnableTimeCalculation;
        }
    /// SARAD3
    }else if((_args[0] == "AVccMax")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage AVccMax is invalid when simulation time is greater than zero.\n");
            AVccMax = mPreAVccMax;
        }else{
            mPreAVccMax = AVccMax;
            this->mADC0->mAVccMax = AVccMax;
            this->mADC1->mAVccMax = AVccMax;
            this->mADC2->mAVccMax = AVccMax;
        }
    }else if((_args[0] == "AVccMin")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage AVccMin is invalid when simulation time is greater than zero.\n");
            AVccMin = mPreAVccMin;
        }else{
            mPreAVccMin = AVccMin;
            this->mADC0->mAVccMin = AVccMin;
            this->mADC1->mAVccMin = AVccMin;
            this->mADC2->mAVccMin = AVccMin;
        }
    }else if((_args[0] == "EVccMax")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage EVccMax is invalid when simulation time is greater than zero.\n");
            EVccMax = mPreEVccMax;
        }else{
            mPreEVccMax = EVccMax;
            this->mADC0->mEVccMax = EVccMax;
            this->mADC1->mEVccMax = EVccMax;
            this->mADC2->mEVccMax = EVccMax;
        }
    }else if((_args[0] == "EVccMin")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage EVccMin is invalid when simulation time is greater than zero.\n");
            EVccMin = mPreEVccMin;
        }else{
            mPreEVccMin = EVccMin;
            this->mADC0->mEVccMin = EVccMin;
            this->mADC1->mEVccMin = EVccMin;
            this->mADC2->mEVccMin = EVccMin;
        }
    }else if((_args[0] == "AvrefhMin")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage AvrefhMin is invalid when simulation time is greater than zero.\n");
            AvrefhMin = mPreAvrefhMin;
        }else{
            mPreAvrefhMin = AvrefhMin;
            this->mADC0->mAvrefhMin = AvrefhMin;
            this->mADC1->mAvrefhMin = AvrefhMin;
            this->mADC2->mAvrefhMin = AvrefhMin;
        }
    }
    return ret;
}//}}}


/// SARAD4 add
// Check & set attribute values to ADC modules
void Csarad_u2a::UpdateChannelNumMethod(void)
{//{{{
    unsigned int found_index = 0;
    unsigned int ADC0_phy_num = emP_SADC_SARCHNUM_Def;
    unsigned int ADC1_phy_num = emP_SADC_SARCHNUM_Def;
    unsigned int ADC2_phy_num = emP_SADC_SARCHNUM_Def;
    unsigned int ADC0_vir_num = emVCRnum_Def;
    unsigned int ADC1_vir_num = emVCRnum_Def;
    unsigned int ADC2_vir_num = emVCRnum_Def;
    unsigned int ADC0_MPX_ch = emEX_MPX_PCH_Def;
    unsigned int ADC1_MPX_ch = emEX_MPX_PCH_Def;
    unsigned int ADC2_MPX_ch = emEX_MPX_PCH_Def;
    unsigned int ADC0_TH_ch = emTH_CHNUM_Def;
    unsigned int ADC1_TH_ch = emTH_CHNUM_Def;
    unsigned int ADC2_TH_ch = emTH_CHNUM_Def;
    mADCNum = emADC_NUM_Def;

    re_printf("info", "TARGET_PRODUCT is %s .\n", TARGET_PRODUCT.value.c_str());

    for(found_index = 0; found_index < emTargetConfigNum; found_index++) {
        if (TARGET_PRODUCT.value == mTargetConfigTable[found_index].target_product_name) {
            break;
        }
    }
    if (found_index < emTargetConfigNum) { // target product name found
        mADCNum = mTargetConfigTable[found_index].adc_num;
        ADC0_phy_num = mTargetConfigTable[found_index].physical_ch_num0;
        ADC1_phy_num = mTargetConfigTable[found_index].physical_ch_num1;
        ADC2_phy_num = mTargetConfigTable[found_index].physical_ch_num2;
        ADC0_vir_num = mTargetConfigTable[found_index].virtual_ch_num0;
        ADC1_vir_num = mTargetConfigTable[found_index].virtual_ch_num1;
        ADC2_vir_num = mTargetConfigTable[found_index].virtual_ch_num2;
        ADC0_MPX_ch = mTargetConfigTable[found_index].external_mpx_pch0;
        ADC1_MPX_ch = mTargetConfigTable[found_index].external_mpx_pch1;
        ADC2_MPX_ch = mTargetConfigTable[found_index].external_mpx_pch2;
        ADC0_TH_ch = mTargetConfigTable[found_index].th_channel_num0;
        ADC1_TH_ch = mTargetConfigTable[found_index].th_channel_num1;
        ADC2_TH_ch = mTargetConfigTable[found_index].th_channel_num2;
    } else { // for custom configuration
        // already set in initial variables
    }

    // Use defined value if it configured
    if (ADC_NUM.value != "") {
        mADCNum = (unsigned int) strtoul(ADC_NUM.value.c_str(), NULL, 0);
    }
    if (P_SADC_SARCHNUM0.value != "") {
        ADC0_phy_num = (unsigned int) strtoul(P_SADC_SARCHNUM0.value.c_str(), NULL, 0);
    }
    if (P_SADC_SARCHNUM1.value != "") {
        ADC1_phy_num = (unsigned int) strtoul(P_SADC_SARCHNUM1.value.c_str(), NULL, 0);
    }
    if (P_SADC_SARCHNUM2.value != "") {
        ADC2_phy_num = (unsigned int) strtoul(P_SADC_SARCHNUM2.value.c_str(), NULL, 0);
    }
    if (VCRnum0.value != "") {
        ADC0_vir_num = (unsigned int) strtoul(VCRnum0.value.c_str(), NULL, 0);
    }
    if (VCRnum1.value != "") {
        ADC1_vir_num = (unsigned int) strtoul(VCRnum1.value.c_str(), NULL, 0);
    }
    if (VCRnum2.value != "") {
        ADC2_vir_num = (unsigned int) strtoul(VCRnum2.value.c_str(), NULL, 0);
    }
    if (EX_MPX_PCH0.value != "") {
        ADC0_MPX_ch = (unsigned int) strtoul(EX_MPX_PCH0.value.c_str(), NULL, 0);
    }
    if (EX_MPX_PCH1.value != "") {
        ADC1_MPX_ch = (unsigned int) strtoul(EX_MPX_PCH1.value.c_str(), NULL, 0);
    }
    if (EX_MPX_PCH2.value != "") {
        ADC2_MPX_ch = (unsigned int) strtoul(EX_MPX_PCH2.value.c_str(), NULL, 0);
    }

    // Stop simulation if attributes is set with wrong value
    /// SARAD4 modify - fix sc_assert() issue on SCHEAP
    re_sc_assert(emADC_NUM_Min <= mADCNum, "The number of ADC module is less than 1");
    re_sc_assert(emADC_NUM_Max >= mADCNum, "The number of ADC module is greater than 3");
    re_sc_assert(emP_SADC_SARCHNUM_Min <= ADC0_phy_num , "The number of physical channels of ADC0 is less than 1");
    re_sc_assert(ADC0_phy_num <= emP_SADC_SARCHNUM_Max , "The number of physical channels of ADC0 is greater than 34");
    re_sc_assert(emVCRnum_Min <= ADC0_vir_num , "The number of virtual channels of ADC0 is less than 16");
    re_sc_assert(ADC0_vir_num <= emVCRnum_Max , "The number of virtual channels of ADC0 is greater than 96");
    re_sc_assert((ADC0_vir_num & 0x3) == 0 , "The number of virtual channels of ADC0 is not a multiple of 4");
    re_sc_assert(ADC0_MPX_ch < ADC0_phy_num , "The analog channel for MPX of ADC0 is greater than the maximum index of physical channel");
    ///// re_sc_assert(ADC0_TH_ch <= emTH_CHNUM_Max , "The TH channel of ADC0 is greater than 6");

    // E2x added
    this->mADC0->mADCNum = mADCNum;

    // Update value to ADC0
    this->mADC0->mTargetProduct = TARGET_PRODUCT.value;
    this->mADC0->mPhyChannelNum = ADC0_phy_num;
    this->mADC0->mVirChannelNum = ADC0_vir_num;
    this->mADC0->mMPXInput = ADC0_MPX_ch;
    this->mADC0->mTHChannelNum = ADC0_TH_ch;
    re_printf("info", "ADC0 is configured with P_SADC_SARCHNUM0 = %d, VCRnum0 = %d, EX_MPX_PCH0 = %d, TH channel = %d.\n", ADC0_phy_num, ADC0_vir_num, ADC0_MPX_ch, ADC0_TH_ch);

    if (mADCNum > 1) { // if there is ADC1
        re_sc_assert(emP_SADC_SARCHNUM_Min <= ADC1_phy_num , "The number of physical channels of ADC1 is less than 1");
        re_sc_assert(ADC1_phy_num <= emP_SADC_SARCHNUM_Max , "The number of physical channels of ADC1 is greater than 34");
        re_sc_assert(emVCRnum_Min <= ADC1_vir_num , "The number of virtual channels of ADC1 is less than 16");
        re_sc_assert(ADC1_vir_num <= emVCRnum_Max , "The number of virtual channels of ADC1 is greater than 96");
        re_sc_assert((ADC1_vir_num & 0x3) == 0 , "The number of virtual channels of ADC1 is not a multiple of 4");
        re_sc_assert(ADC1_MPX_ch < ADC1_phy_num , "The analog channel for MPX of ADC1 is greater than the maximum index of physical channel");
        // Update value to ADC1
        this->mADC1->mTargetProduct = TARGET_PRODUCT.value;
        this->mADC1->mPhyChannelNum = ADC1_phy_num;
        this->mADC1->mVirChannelNum = ADC1_vir_num;
        this->mADC1->mMPXInput = ADC1_MPX_ch;
        this->mADC1->mTHChannelNum = ADC1_TH_ch;
        re_printf("info", "ADC1 is configured with P_SADC_SARCHNUM1 = %d, VCRnum1 = %d, EX_MPX_PCH1 = %d, TH channel = %d.\n", ADC1_phy_num, ADC1_vir_num, ADC1_MPX_ch, ADC1_TH_ch);

        this->mADC1->mADCNum = mADCNum;
    }

    if (mADCNum > 2) { // if there is ADC2
        re_sc_assert(emP_SADC_SARCHNUM_Min <= ADC2_phy_num , "The number of physical channels of ADC2 is less than 1");
        re_sc_assert(ADC2_phy_num <= emP_SADC_SARCHNUM_Max , "The number of physical channels of ADC2 is greater than 34");
        re_sc_assert(emVCRnum_Min <= ADC2_vir_num , "The number of virtual channels of ADC2 is less than 16");
        re_sc_assert(ADC2_vir_num <= emVCRnum_Max , "The number of virtual channels of ADC2 is greater than 96");
        re_sc_assert((ADC2_vir_num & 0x3) == 0 , "The number of virtual channels of ADC2 is not a multiple of 4");
        re_sc_assert(ADC2_MPX_ch < ADC2_phy_num , "The analog channel for MPX of ADC2 is greater than the maximum index of physical channel");
        // Update value to ADC2
        this->mADC2->mTargetProduct = TARGET_PRODUCT.value;
        this->mADC2->mPhyChannelNum = ADC2_phy_num;
        this->mADC2->mVirChannelNum = ADC2_vir_num;
        this->mADC2->mMPXInput = ADC2_MPX_ch;
        this->mADC2->mTHChannelNum = ADC2_TH_ch;
        re_printf("info", "ADC2 is configured with P_SADC_SARCHNUM1 = %d, VCRnum1 = %d, EX_MPX_PCH1 = %d, TH channel = %d.\n", ADC2_phy_num, ADC2_vir_num, ADC2_MPX_ch, ADC2_TH_ch);

        this->mADC2->mADCNum = mADCNum;
    }
}//}}}

void Csarad_u2a::FixInputMethod(void)
{//{{{
    const_double.write(0);
}//}}}


// vim600: set foldmethod=marker :
