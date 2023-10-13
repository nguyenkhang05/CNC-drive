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

#include "re_define.h"
#include "sarad.h"
#ifndef FRACTION_TO_INT
#define FRACTION_TO_INT(d) ((unsigned int)((d - (unsigned long long)(d))*1000000))
#endif

/// SARAD4 add - fix sc_assert() issue on SCHEAP
#define re_sc_assert(expr, msg)     ((void) ((expr) ? 0 : (SC_REPORT_FATAL( sc_core::SC_ID_ASSERTION_FAILED_ , msg), 0)))

//Constructor of SAR AD class
Csarad::Csarad(sc_module_name name):
         vpcl::tlm_tgt_if<32>(name)
        ,Csarad_regif((std::string)name, 32)

        ,pclk("pclk")
        ,ADCLK("ADCLK")
        ,preset_n("preset_n")
        ,AVREFH0("AVREFH0")
        ,AVREFH1("AVREFH1")
        // RH850_E2x added
        ,AVREFH2("AVREFH2")
        ,AVREFH3("AVREFH3")
        //trigger input port
        ,AD0SG0TRG("AD0SG0TRG")
        ,AD0SG1TRG("AD0SG1TRG")
        ,AD0SG2TRG("AD0SG2TRG")
        ,AD0SG3TRG("AD0SG3TRG")
        ,AD0SG4TRG("AD0SG4TRG")
        ,AD1SG0TRG("AD1SG0TRG")
        ,AD1SG1TRG("AD1SG1TRG")
        ,AD1SG2TRG("AD1SG2TRG")
        ,AD1SG3TRG("AD1SG3TRG")
        ,AD1SG4TRG("AD1SG4TRG")
        // RH850_E2x added
        ,AD2SG0TRG("AD2SG0TRG")
        ,AD2SG1TRG("AD2SG1TRG")
        ,AD2SG2TRG("AD2SG2TRG")
        ,AD2SG3TRG("AD2SG3TRG")
        ,AD2SG4TRG("AD2SG4TRG")
        ,AD3SG0TRG("AD3SG0TRG")
        ,AD3SG1TRG("AD3SG1TRG")
        ,AD3SG2TRG("AD3SG2TRG")
        ,AD3SG3TRG("AD3SG3TRG")
        ,AD3SG4TRG("AD3SG4TRG")
        //Analog terminal input port
        ,AN000("AN000")
        ,AN001("AN001")
        ,AN002("AN002")
        ,AN003("AN003")
        ,AN010("AN010")
        ,AN011("AN011")
        ,AN012("AN012")
        ,AN013("AN013")
        ,AN020("AN020")
        ,AN021("AN021")
        ,AN022("AN022")
        ,AN023("AN023")
        ,AN030("AN030")
        ,AN031("AN031")
        ,AN032("AN032")
        ,AN033("AN033")
        ,AN040("AN040")
        ,AN041("AN041")
        ,AN042("AN042")
        ,AN043("AN043")
        /// SARAD4 add
        ,AN050("AN050")
        ,AN051("AN051")
        ,AN052("AN052")
        ,AN053("AN053")
        ,AN060("AN060")
        ,AN061("AN061")
        ,AN062("AN062")
        ,AN063("AN063")
        ,AN070("AN070")
        ,AN071("AN071")
        ,AN072("AN072")
        ,AN073("AN073")

        ,AN100("AN100")
        ,AN101("AN101")
        ,AN102("AN102")
        ,AN103("AN103")
        ,AN110("AN110")
        ,AN111("AN111")
        ,AN112("AN112")
        ,AN113("AN113")
        ,AN120("AN120")
        ,AN121("AN121")
        ,AN122("AN122")
        ,AN123("AN123")
        ,AN130("AN130")
        ,AN131("AN131")
        ,AN132("AN132")
        ,AN133("AN133")
        ,AN140("AN140")
        ,AN141("AN141")
        ,AN142("AN142")
        ,AN143("AN143")
        /// SARAD4 add
        ,AN150("AN150")
        ,AN151("AN151")
        ,AN152("AN152")
        ,AN153("AN153")
        ,AN160("AN160")
        ,AN161("AN161")
        ,AN162("AN162")
        ,AN163("AN163")
        ,AN170("AN170")
        ,AN171("AN171")
        ,AN172("AN172")
        ,AN173("AN173")
        // RH850_E2x added
        ,AN200("AN200")
        ,AN201("AN201")
        ,AN202("AN202")
        ,AN203("AN203")
        ,AN210("AN210")
        ,AN211("AN211")
        ,AN212("AN212")
        ,AN213("AN213")
        ,AN220("AN220")
        ,AN221("AN221")
        ,AN222("AN222")
        ,AN223("AN223")
        ,AN230("AN230")
        ,AN231("AN231")
        ,AN232("AN232")
        ,AN233("AN233")
        ,AN240("AN240")
        ,AN241("AN241")
        ,AN242("AN242")
        ,AN243("AN243")
        ,AN250("AN250")
        ,AN251("AN251")
        ,AN252("AN252")
        ,AN253("AN253")
        ,AN260("AN260")
        ,AN261("AN261")
        ,AN262("AN262")
        ,AN263("AN263")
        ,AN270("AN270")
        ,AN271("AN271")
        ,AN272("AN272")
        ,AN273("AN273")
        ,AN300("AN300")
        ,AN301("AN301")
        ,AN302("AN302")
        ,AN303("AN303")
        ,AN310("AN310")
        ,AN311("AN311")
        ,AN312("AN312")
        ,AN313("AN313")
        ,AN320("AN320")
        ,AN321("AN321")
        ,AN322("AN322")
        ,AN323("AN323")
        ,AN330("AN330")
        ,AN331("AN331")
        ,AN332("AN332")
        ,AN333("AN333")
        ,AN340("AN340")
        ,AN341("AN341")
        ,AN342("AN342")
        ,AN343("AN343")
        ,AN350("AN350")
        ,AN351("AN351")
        ,AN352("AN352")
        ,AN353("AN353")
        ,AN360("AN360")
        ,AN361("AN361")
        ,AN362("AN362")
        ,AN363("AN363")
        ,AN370("AN370")
        ,AN371("AN371")
        ,AN372("AN372")
        ,AN373("AN373")
        // RH850_E2x added for IFC
        ,IFVCH0("IFVCH0")
        ,IFVCH1("IFVCH1")
        ,IFVCH2("IFVCH2")
        ,IFVCH3("IFVCH3")
        ,IFRD0("IFRD0")
        ,IFRD1("IFRD1")
        ,IFRD2("IFRD2")
        ,IFRD3("IFRD3")
        ,IFRE0("IFRE0")
        ,IFRE1("IFRE1")
        ,IFRE2("IFRE2")
        ,IFRE3("IFRE3")

        //interrupt ouptut port
        ,INT_AD00("INT_AD00")
        ,INT_AD01("INT_AD01")
        ,INT_AD02("INT_AD02")
        ,INT_AD03("INT_AD03")
        ,INT_AD04("INT_AD04")
        ,INT_AD10("INT_AD10")
        ,INT_AD11("INT_AD11")
        ,INT_AD12("INT_AD12")
        ,INT_AD13("INT_AD13")
        ,INT_AD14("INT_AD14")
        ,INT_MPX0("INT_MPX0")
        ,INT_MPX1("INT_MPX1")
        ,INT_ADE0("INT_ADE0")
        ,INT_ADE1("INT_ADE1")
        ,ADPE0("ADPE0")
        ,ADPE1("ADPE1")
        //Data output port
        ,ADEND00("ADEND00")
        ,ADEND01("ADEND01")
        ,ADEND02("ADEND02")
        ,ADEND03("ADEND03")
        ,ADEND04("ADEND04")
        ,ADEND10("ADEND10")
        ,ADEND11("ADEND11")
        ,ADEND12("ADEND12")
        ,ADEND13("ADEND13")
        ,ADEND14("ADEND14")

        // RH850_E2x added
        ,INT_AD20("INT_AD20")
        ,INT_AD21("INT_AD21")
        ,INT_AD22("INT_AD22")
        ,INT_AD23("INT_AD23")
        ,INT_AD24("INT_AD24")
        ,INT_AD30("INT_AD30")
        ,INT_AD31("INT_AD31")
        ,INT_AD32("INT_AD32")
        ,INT_AD33("INT_AD33")
        ,INT_AD34("INT_AD34")
        ,INT_MPX2("INT_MPX2")
        ,INT_MPX3("INT_MPX3")
        ,INT_ADE2("INT_ADE2")
        ,INT_ADE3("INT_ADE3")
        ,ADPE2("ADPE2")
        ,ADPE3("ADPE3")

        //Data output port
        ,ADEND20("ADEND20")
        ,ADEND21("ADEND21")
        ,ADEND22("ADEND22")
        ,ADEND23("ADEND23")
        ,ADEND24("ADEND24")
        ,ADEND30("ADEND30")
        ,ADEND31("ADEND31")
        ,ADEND32("ADEND32")
        ,ADEND33("ADEND33")
        ,ADEND34("ADEND34")

        // RH850_E2x added for IFC
        ,ADC0_IFDATA("ADC0_IFDATA")
        ,ADC1_IFDATA("ADC1_IFDATA")
        ,ADC2_IFDATA("ADC2_IFDATA")
        ,ADC3_IFDATA("ADC3_IFDATA")

        // RH850_E2x added for ASF
        ,DFREQ0("DFREQ0")
        ,DFREQ1("DFREQ1")
        ,DFREQ2("DFREQ2")
        ,DFREQ3("DFREQ3")
        ,DFTAG0("DFTAG0")
        ,DFTAG1("DFTAG1")
        ,DFTAG2("DFTAG2")
        ,DFTAG3("DFTAG3")
        ,DFDATA0("DFDATA0")
        ,DFDATA1("DFDATA1")
        ,DFDATA2("DFDATA2")
        ,DFDATA3("DFDATA3")
        ,VCULMO0("VCULMO0")
        ,VCLLMO0("VCLLMO0")
        ,VCULMO1("VCULMO1")
        ,VCLLMO1("VCLLMO1")
        ,VCULMO2("VCULMO2")
        ,VCLLMO2("VCLLMO2")
        ,VCULMO3("VCULMO3")
        ,ADMPX00("ADMPX00")
        ,ADMPX01("ADMPX01")
        ,ADMPX02("ADMPX02")
        ,ADMPX03("ADMPX03")
        ,ADMPX04("ADMPX04")
        ,ADMPX10("ADMPX10")
        ,ADMPX11("ADMPX11")
        ,ADMPX12("ADMPX12")
        ,ADMPX13("ADMPX13")
        ,ADMPX14("ADMPX14")
        ,ADMPX20("ADMPX20")
        ,ADMPX21("ADMPX21")
        ,ADMPX22("ADMPX22")
        ,ADMPX23("ADMPX23")
        ,ADMPX24("ADMPX24")
        ,ADMPX30("ADMPX30")
        ,ADMPX31("ADMPX31")
        ,ADMPX32("ADMPX32")
        ,ADMPX33("ADMPX33")
        ,ADMPX34("ADMPX34")

        /// SARAD4 add
        // Attributes
        ,TARGET_PRODUCT("TARGET_PRODUCT","")
        ,ADC_NUM("ADC_NUM","")
        ,P_SADC_SARCHNUM0("P_SADC_SARCHNUM0", "")
        ,P_SADC_SARCHNUM1("P_SADC_SARCHNUM1", "")
        ,P_SADC_SARCHNUM2("P_SADC_SARCHNUM2", "")
        ,P_SADC_SARCHNUM3("P_SADC_SARCHNUM3", "")
        ,VCRnum0("VCRnum0", "")
        ,VCRnum1("VCRnum1", "")
        ,VCRnum2("VCRnum2", "")
        ,VCRnum3("VCRnum3", "")
        ,EX_MPX_PCH0("EX_MPX_PCH0", "")
        ,EX_MPX_PCH1("EX_MPX_PCH1", "")
        ,EX_MPX_PCH2("EX_MPX_PCH2", "")
        ,EX_MPX_PCH3("EX_MPX_PCH3", "")
{//{{{

    //Initalze handleCommand
    CommandInit(this->name());
    Csarad_regif::set_instance_name(this->name());

    for (unsigned int index = 0; index < emVCRnum_Max; index++){
        std::ostringstream str_vcend[emADC_NUM_Max];
        for (unsigned int adc_num = 0; adc_num < emADC_NUM_Max; adc_num++){
            str_vcend[adc_num]<<"vcend"<<adc_num<<"_"<<index;
        }
        VCEND0[index] = new sc_out<bool>(str_vcend[0].str().c_str());
        VCEND1[index] = new sc_out<bool>(str_vcend[1].str().c_str());
        VCEND2[index] = new sc_out<bool>(str_vcend[2].str().c_str());
        VCEND3[index] = new sc_out<bool>(str_vcend[3].str().c_str());
        VCEND0[index]->initialize(0);
        VCEND1[index]->initialize(0);
        VCEND2[index]->initialize(0);
        VCEND3[index]->initialize(0);
    }

    //Instance ADC0 and ADC1 modules and connect ports
    mADC0 = new Cadc ("mADC0", 0);
    sc_assert(mADC0 != NULL);
    mADC1 = new Cadc ("mADC1", 1);
    sc_assert(mADC1 != NULL);
    mADC2 = new Cadc ("mADC2", 2);
    sc_assert(mADC2 != NULL);
    mADC3 = new Cadc ("mADC3", 3);
    sc_assert(mADC3 != NULL);

    ConnectPort();

    //Initialze variable
    mIsRefVol0Update  = false;
    mIsRefVol1Update  = false;
    mIsRefVol2Update  = false;
    mIsRefVol3Update  = false;
    mIsInitialize     = true;
    mResetPeriod      = 0;
    mIsCmdResetFlag   = false;
    mSARPortResetFlag = false;
    mSARCmdResetFlag  = false;
    mSARADState = emIDLE;
    mPreAVcc0       = 3.3;
    mPreAVcc1       = 3.3;
    mPreAVcc2       = 3.3;
    mPreAVcc3       = 3.3;
    mPreAvrefh0     = 3.3;
    mPreAvrefh1     = 3.3;
    mPreAvrefh2     = 3.3;
    mPreAvrefh3     = 3.3;
    mPretTSPL       = 27;
    mPretTHSD        = 5;
    mPretTHLD        = 10;
    mPreExSmpt        = 18;
    mPretSAR        = 0;
    mPretD          = 0;
    mPretED         = 0;
    mPreStartAddrADCA0 = 0x1000;
    mPreStartAddrADCA1 = 0x2000;
    mPreStartAddrADCA2 = 0x3000;
    mPreStartAddrADCA3 = 0x4000;
    mPreEndAddressADCA0 = 0x1FFF;
    mPreEndAddressADCA1 = 0x2FFF;
    mPreEndAddressADCA2 = 0x3FFF;
    mPreEndAddressADCA3 = 0x4FFF;
    mPreOffsetAddMask = 0x7FFF;
    mPreEnableTimeCalculation = true;
    /// SARAD3
    mPreAVccMax = 3.6;
    mPreAVccMin = 3;
    mPreAvrefhMin = 3;
    mADCNum = emADC_NUM_Def;

    /// added for CC-Cube D1D2 and RH850 P1M-E
    TargetConfig TempTargetConfigTable[emTargetConfigNum] =  { {"CCC_D1V1"              ,1,15, 0, 0, 0,24, 0, 0, 0,10,0, 0,0,2,2,0,0}
                                                              ,{"CCC_D1V2"              ,1,16, 0, 0, 0,24, 0, 0, 0,10,0, 0,0,2,2,0,0}
                                                              ,{"CCC_D2V1"              ,1,16, 0, 0, 0,32, 0, 0, 0,10,0, 0,0,4,4,0,0}
                                                              ,{"CCC_D2V2"              ,1,20, 0, 0, 0,32, 0, 0, 0,10,0, 0,0,4,4,0,0}
                                                              ,{"RH850_P1M-E"           ,2,12,13, 0, 0,24,24, 0, 0, 6,0, 0,0,6,4,0,0}
                                                              ,{"RH850_E2x"             ,4,31,20,28,20,40,40,40,40,13,0,19,0,0,0,0,0}
                                                              ,{"RH850_E2x-468Pins"     ,4,31,20,32,32,40,40,40,40,13,0,19,0,0,0,0,0}
                                                              ,{"RH850_E2x_fcc1"        ,4,31,20,28,20,40,40,40,40,13,0,19,0,0,0,0,0} // Add to separate FCC1 - DucDuong 2018.12.26
                                                             };
    // copy TempTargetConfig
    for (unsigned int i=0;i<emTargetConfigNum;i++) {
        mTargetConfigTable[i] = TempTargetConfigTable[i];
    }

    //Initialize output port
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
    INT_MPX0.initialize(0);
    INT_MPX1.initialize(0);
    INT_ADE0.initialize(0);
    INT_ADE1.initialize(0);
    ADPE0.initialize(0);
    ADPE1.initialize(0);
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
    // RH850_E2x added
    INT_AD20.initialize(0);
    INT_AD21.initialize(0);
    INT_AD22.initialize(0);
    INT_AD23.initialize(0);
    INT_AD24.initialize(0);
    INT_AD30.initialize(0);
    INT_AD31.initialize(0);
    INT_AD32.initialize(0);
    INT_AD33.initialize(0);
    INT_AD34.initialize(0);
    INT_MPX2.initialize(0);
    INT_MPX3.initialize(0);
    INT_ADE2.initialize(0);
    INT_ADE3.initialize(0);
    ADPE2.initialize(0);
    ADPE3.initialize(0);
    ADEND20.initialize(0);
    ADEND21.initialize(0);
    ADEND22.initialize(0);
    ADEND23.initialize(0);
    ADEND24.initialize(0);
    ADEND30.initialize(0);
    ADEND31.initialize(0);
    ADEND32.initialize(0);
    ADEND33.initialize(0);
    ADEND34.initialize(0);

    // RH850_E2x added for IFC
    ADC0_IFDATA.initialize(0);
    ADC1_IFDATA.initialize(0);
    ADC2_IFDATA.initialize(0);
    ADC3_IFDATA.initialize(0);

    // RH850_E2x added for ASF
    DFREQ0.initialize(0);
    DFREQ1.initialize(0);
    DFREQ2.initialize(0);
    DFREQ3.initialize(0);
    DFTAG0.initialize(0);
    DFTAG1.initialize(0);
    DFTAG2.initialize(0);
    DFTAG3.initialize(0);
    DFDATA0.initialize(0);
    DFDATA1.initialize(0);
    DFDATA2.initialize(0);
    DFDATA3.initialize(0);

    VCULMO0.initialize(0);
    VCULMO1.initialize(0);
    VCULMO2.initialize(0);
    VCULMO3.initialize(0);

    VCLLMO0.initialize(0);
    VCLLMO1.initialize(0);
    VCLLMO2.initialize(0);
    VCLLMO3.initialize(0);

    // RH850-E2x FCC1/FCC2
    ADMPX00.initialize(0);
    ADMPX01.initialize(0);
    ADMPX02.initialize(0);
    ADMPX03.initialize(0);
    ADMPX04.initialize(0);

    ADMPX10.initialize(0);
    ADMPX11.initialize(0);
    ADMPX12.initialize(0);
    ADMPX13.initialize(0);
    ADMPX14.initialize(0);

    ADMPX20.initialize(0);
    ADMPX21.initialize(0);
    ADMPX22.initialize(0);
    ADMPX23.initialize(0);
    ADMPX24.initialize(0);

    ADMPX30.initialize(0);
    ADMPX31.initialize(0);
    ADMPX32.initialize(0);
    ADMPX33.initialize(0);
    ADMPX34.initialize(0);

    //Reset Method
    SC_METHOD(ResetMethod);
    dont_initialize();
    sensitive << preset_n;
    SC_METHOD(AssertResetMethod);
    dont_initialize();
    sensitive << mCmdResetEvent;
    SC_METHOD(DeAssertResetMethod);
    dont_initialize();
    sensitive << mCmdCancelResetEvent;
    //Clock Method
    SC_METHOD(PCLKMethod);
    dont_initialize();
    sensitive << pclk;
    SC_METHOD(ADCLKMethod);
    dont_initialize();
    sensitive << ADCLK;
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
    SC_METHOD(AVREFH3Method);
    dont_initialize();
    sensitive << AVREFH3;
    SC_METHOD(InitialAVREFH0Method);
    SC_METHOD(InitialAVREFH1Method);
    SC_METHOD(InitialAVREFH2Method);
    SC_METHOD(InitialAVREFH3Method);

    SC_METHOD(CheckBaseAddrMethod);

    //State Method
    SC_METHOD(SARADstateMethod);
    dont_initialize();
    sensitive << mADC0->mADCChangeStateEvent;
    sensitive << mADC1->mADCChangeStateEvent;
    sensitive << mADC2->mADCChangeStateEvent;
    sensitive << mADC3->mADCChangeStateEvent;
    SetLatency_TLM (0, true);

    /// SARAD4 add
    SC_METHOD(UpdateChannelNumMethod);

}//}}}

//destructor of Csarad class
Csarad::~Csarad()
{//{{{
    delete mADC0;
    delete mADC1;
    delete mADC2;
    delete mADC3;
}//}}}

//Connect port
void Csarad::ConnectPort(void)
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
    /// SARAD4 add
    mADC0->ANm50(AN050);
    mADC0->ANm51(AN051);
    mADC0->ANm52(AN052);
    mADC0->ANm53(AN053);
    mADC0->ANm60(AN060);
    mADC0->ANm61(AN061);
    mADC0->ANm62(AN062);
    mADC0->ANm63(AN063);
    mADC0->ANm70(AN070);
    mADC0->ANm71(AN071);
    mADC0->ANm72(AN072);
    mADC0->ANm73(AN073);

    mADC0->ADmSG0TRG(AD0SG0TRG);
    mADC0->ADmSG1TRG(AD0SG1TRG);
    mADC0->ADmSG2TRG(AD0SG2TRG);
    mADC0->ADmSG3TRG(AD0SG3TRG);
    mADC0->ADmSG4TRG(AD0SG4TRG);

    // RH850_E2x added
    mADC0->IFVCHm(IFVCH0);
    mADC0->IFRDm(IFRD0);
    mADC0->IFREm(IFRE0);

    //output port
    mADC0->INT_ADm0(INT_AD00);
    mADC0->INT_ADm1(INT_AD01);
    mADC0->INT_ADm2(INT_AD02);
    mADC0->INT_ADm3(INT_AD03);
    mADC0->INT_ADm4(INT_AD04);
    mADC0->INT_MPXm(INT_MPX0);
    mADC0->INT_ADEm(INT_ADE0);
    mADC0->ADPEm(ADPE0);

    mADC0->ADENDm0(ADEND00);
    mADC0->ADENDm1(ADEND01);
    mADC0->ADENDm2(ADEND02);
    mADC0->ADENDm3(ADEND03);
    mADC0->ADENDm4(ADEND04);

    // RH850_E2x added
    mADC0->ADCm_IFDATA(ADC0_IFDATA);
    mADC0->DFREQm(DFREQ0);
    mADC0->DFTAGm(DFTAG0);
    mADC0->DFDATAm(DFDATA0);

    for (unsigned int i = 0; i < emVCRnum_Max; i++) {
        mADC0->VCEND[i]->bind(*VCEND0[i]);
    }
    mADC0->VCULMO(VCULMO0);
    mADC0->VCLLMO(VCLLMO0);

    mADC0->ADMPX0(ADMPX00);
    mADC0->ADMPX1(ADMPX01);
    mADC0->ADMPX2(ADMPX02);
    mADC0->ADMPX3(ADMPX03);
    mADC0->ADMPX4(ADMPX04);

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
    /// SARAD4 add
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

    mADC1->ADmSG0TRG(AD1SG0TRG);
    mADC1->ADmSG1TRG(AD1SG1TRG);
    mADC1->ADmSG2TRG(AD1SG2TRG);
    mADC1->ADmSG3TRG(AD1SG3TRG);
    mADC1->ADmSG4TRG(AD1SG4TRG);

    // RH850_E2x added
    mADC1->IFVCHm(IFVCH1);
    mADC1->IFRDm(IFRD1);
    mADC1->IFREm(IFRE1);

    //output port
    mADC1->INT_ADm0(INT_AD10);
    mADC1->INT_ADm1(INT_AD11);
    mADC1->INT_ADm2(INT_AD12);
    mADC1->INT_ADm3(INT_AD13);
    mADC1->INT_ADm4(INT_AD14);
    mADC1->INT_MPXm(INT_MPX1);
    mADC1->INT_ADEm(INT_ADE1);
    mADC1->ADPEm(ADPE1);

    mADC1->ADENDm0(ADEND10);
    mADC1->ADENDm1(ADEND11);
    mADC1->ADENDm2(ADEND12);
    mADC1->ADENDm3(ADEND13);
    mADC1->ADENDm4(ADEND14);

    // RH850_E2x added
    mADC1->ADCm_IFDATA(ADC1_IFDATA);
    mADC1->DFREQm(DFREQ1);
    mADC1->DFTAGm(DFTAG1);
    mADC1->DFDATAm(DFDATA1);

    for (unsigned int i = 0; i < emVCRnum_Max; i++) {
        mADC1->VCEND[i]->bind(*VCEND1[i]);
    }
    mADC1->VCULMO(VCULMO1);
    mADC1->VCLLMO(VCLLMO1);

    mADC1->ADMPX0(ADMPX10);
    mADC1->ADMPX1(ADMPX11);
    mADC1->ADMPX2(ADMPX12);
    mADC1->ADMPX3(ADMPX13);
    mADC1->ADMPX4(ADMPX14);

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
    /// SARAD4 add
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
    mADC2->ANm72(AN272);
    mADC2->ANm73(AN273);

    mADC2->ADmSG0TRG(AD2SG0TRG);
    mADC2->ADmSG1TRG(AD2SG1TRG);
    mADC2->ADmSG2TRG(AD2SG2TRG);
    mADC2->ADmSG3TRG(AD2SG3TRG);
    mADC2->ADmSG4TRG(AD2SG4TRG);

    // RH850_E2x added
    mADC2->IFVCHm(IFVCH2);
    mADC2->IFRDm(IFRD2);
    mADC2->IFREm(IFRE2);

    //output port
    mADC2->INT_ADm0(INT_AD20);
    mADC2->INT_ADm1(INT_AD21);
    mADC2->INT_ADm2(INT_AD22);
    mADC2->INT_ADm3(INT_AD23);
    mADC2->INT_ADm4(INT_AD24);
    mADC2->INT_MPXm(INT_MPX2);
    mADC2->INT_ADEm(INT_ADE2);
    mADC2->ADPEm(ADPE2);

    mADC2->ADENDm0(ADEND20);
    mADC2->ADENDm1(ADEND21);
    mADC2->ADENDm2(ADEND22);
    mADC2->ADENDm3(ADEND23);
    mADC2->ADENDm4(ADEND24);

    // RH850_E2x added
    mADC2->ADCm_IFDATA(ADC2_IFDATA);
    mADC2->DFREQm(DFREQ2);
    mADC2->DFTAGm(DFTAG2);
    mADC2->DFDATAm(DFDATA2);

    for (unsigned int i = 0; i < emVCRnum_Max; i++) {
        mADC2->VCEND[i]->bind(*VCEND2[i]);
    }
    mADC2->VCULMO(VCULMO2);
    mADC2->VCLLMO(VCLLMO2);

    mADC2->ADMPX0(ADMPX20);
    mADC2->ADMPX1(ADMPX21);
    mADC2->ADMPX2(ADMPX22);
    mADC2->ADMPX3(ADMPX23);
    mADC2->ADMPX4(ADMPX24);
    // ADC3
    //input port
    mADC3->ANm00(AN300);
    mADC3->ANm01(AN301);
    mADC3->ANm02(AN302);
    mADC3->ANm03(AN303);
    mADC3->ANm10(AN310);
    mADC3->ANm11(AN311);
    mADC3->ANm12(AN312);
    mADC3->ANm13(AN313);
    mADC3->ANm20(AN320);
    mADC3->ANm21(AN321);
    mADC3->ANm22(AN322);
    mADC3->ANm23(AN323);
    mADC3->ANm30(AN330);
    mADC3->ANm31(AN331);
    mADC3->ANm32(AN332);
    mADC3->ANm33(AN333);
    mADC3->ANm40(AN340);
    mADC3->ANm41(AN341);
    mADC3->ANm42(AN342);
    mADC3->ANm43(AN343);
    /// SARAD4 add
    mADC3->ANm50(AN350);
    mADC3->ANm51(AN351);
    mADC3->ANm52(AN352);
    mADC3->ANm53(AN353);
    mADC3->ANm60(AN360);
    mADC3->ANm61(AN361);
    mADC3->ANm62(AN362);
    mADC3->ANm63(AN363);
    mADC3->ANm70(AN370);
    mADC3->ANm71(AN371);
    mADC3->ANm72(AN372);
    mADC3->ANm73(AN373);

    mADC3->ADmSG0TRG(AD3SG0TRG);
    mADC3->ADmSG1TRG(AD3SG1TRG);
    mADC3->ADmSG2TRG(AD3SG2TRG);
    mADC3->ADmSG3TRG(AD3SG3TRG);
    mADC3->ADmSG4TRG(AD3SG4TRG);

    // RH850_E2x added
    mADC3->IFVCHm(IFVCH3);
    mADC3->IFRDm(IFRD3);
    mADC3->IFREm(IFRE3);

    //output port
    mADC3->INT_ADm0(INT_AD30);
    mADC3->INT_ADm1(INT_AD31);
    mADC3->INT_ADm2(INT_AD32);
    mADC3->INT_ADm3(INT_AD33);
    mADC3->INT_ADm4(INT_AD34);
    mADC3->INT_MPXm(INT_MPX3);
    mADC3->INT_ADEm(INT_ADE3);
    mADC3->ADPEm(ADPE3);

    mADC3->ADENDm0(ADEND30);
    mADC3->ADENDm1(ADEND31);
    mADC3->ADENDm2(ADEND32);
    mADC3->ADENDm3(ADEND33);
    mADC3->ADENDm4(ADEND34);

    // RH850_E2x added
    mADC3->ADCm_IFDATA(ADC3_IFDATA);
    mADC3->DFREQm(DFREQ3);
    mADC3->DFTAGm(DFTAG3);
    mADC3->DFDATAm(DFDATA3);

    for (unsigned int i = 0; i < emVCRnum_Max; i++) {
        mADC3->VCEND[i]->bind(*VCEND3[i]);
    }
    mADC3->VCULMO(VCULMO3);
    mADC3->VCLLMO(VCLLMO3);

    mADC3->ADMPX0(ADMPX30);
    mADC3->ADMPX1(ADMPX31);
    mADC3->ADMPX2(ADMPX32);
    mADC3->ADMPX3(ADMPX33);
    mADC3->ADMPX4(ADMPX34);

}//}}}

// SAR AD METHOD

void Csarad::InitialAVREFH0Method(void)
{//{{{
    if(!mIsRefVol0Update){
        mIsRefVol0Update = true;
        Avrefh0 = (double)(AVREFH0.read());
        this->mADC0->mAvrefh = Avrefh0;
    }
}//}}}

void Csarad::InitialAVREFH1Method(void)
{//{{{
    if(!mIsRefVol1Update){
        mIsRefVol1Update = true;
        Avrefh1 = (double)(AVREFH1.read());
        this->mADC1->mAvrefh = Avrefh1;
    }
}//}}}

void Csarad::InitialAVREFH2Method(void)
{//{{{
    if(!mIsRefVol2Update){
        mIsRefVol2Update = true;
        Avrefh2 = (double)(AVREFH2.read());
        this->mADC2->mAvrefh = Avrefh2;
    }
}//}}}

void Csarad::InitialAVREFH3Method(void)
{//{{{
    if(!mIsRefVol3Update){
        mIsRefVol3Update = true;
        Avrefh3 = (double)(AVREFH3.read());
        this->mADC3->mAvrefh = Avrefh3;
    }
}//}}}

void Csarad::AVREFH0Method(void)
{//{{{
    if(!(mSARPortResetFlag || mSARCmdResetFlag)){
        double current_time = sc_time_stamp().to_double();
        if(current_time > 0){
            re_printf("warning","Changing reference voltage Avrefh0 is invalid when simulation time is greater than zero.\n");
        }
    }
}//}}}

void Csarad::AVREFH1Method(void)
{//{{{
    if(!(mSARPortResetFlag || mSARCmdResetFlag)){
        double current_time = sc_time_stamp().to_double();
        if(current_time > 0){
            re_printf("warning","Changing reference voltage Avrefh1 is invalid when simulation time is greater than zero.\n");
        }
    }
}//}}}

void Csarad::AVREFH2Method(void)
{//{{{
    if(!(mSARPortResetFlag || mSARCmdResetFlag)){
        double current_time = sc_time_stamp().to_double();
        if(current_time > 0){
            re_printf("warning","Changing reference voltage Avrefh2 is invalid when simulation time is greater than zero.\n");
        }
    }
}//}}}

void Csarad::AVREFH3Method(void)
{//{{{
    if(!(mSARPortResetFlag || mSARCmdResetFlag)){
        double current_time = sc_time_stamp().to_double();
        if(current_time > 0){
            re_printf("warning","Changing reference voltage Avrefh3 is invalid when simulation time is greater than zero.\n");
        }
    }
}//}}}

void Csarad::SARADstateMethod(void)
{//{{{
    if(!(mSARPortResetFlag || mSARCmdResetFlag)){
        if((mADC0->mADCChangeState || mADC1->mADCChangeState || mADC2->mADCChangeState || mADC3->mADCChangeState)&&(mSARADState == emIDLE)) {
            re_printf("info","SAR AD moves from IDLE state to OPERATING state.\n");
            mSARADState = emOPERATING;
        }
        else if((!(mADC0->mADCChangeState || mADC1->mADCChangeState || mADC2->mADCChangeState || mADC3->mADCChangeState))&&(mSARADState == emOPERATING)) {
            re_printf("info","SAR AD moves from OPERATING state to IDLE state.\n");
            mSARADState = emIDLE;
        }
    }
}//}}}

void Csarad::PCLKMethod(void)
{//{{{
    SetCLKfreq("pclk", sc_dt::uint64_to_double(pclk.read()));
}//}}}

void Csarad::ADCLKMethod(void)
{//{{{
    SetCLKfreq("ADCLK", sc_dt::uint64_to_double(ADCLK.read()));
}//}}}

void Csarad::ResetMethod(void)
{//{{{
    //1. Reset port sensitive
    bool is_cancel_assertreset = false;
    if(preset_n.read() == vpcl::emResetActive){
        mSARPortResetFlag = true;
    }else{
        mSARPortResetFlag = false;
    }
    if(mSARPortResetFlag){
        mIsInitialize = false;
        re_printf("info","Reset signal is asserted.\n");
        this->EnableReset(mSARPortResetFlag);
        is_cancel_assertreset = true;
    }else{
        if(mIsInitialize){
            mIsInitialize = false;
            is_cancel_assertreset = false;
        }else{
            re_printf("info","Reset signal is negated.\n");
            this->EnableReset(mSARPortResetFlag);
            is_cancel_assertreset = true;
        }
    }

    //Cancel AssertReset
    if(is_cancel_assertreset){
        mCmdResetEvent.cancel();
        mCmdCancelResetEvent.cancel();
        mIsCmdResetFlag = false;
        mSARCmdResetFlag = false;
    }

}//}}}

void Csarad::AssertResetMethod(void)
{//{{{
    mSARCmdResetFlag = true;
    this->EnableReset(mSARCmdResetFlag);
    mCmdCancelResetEvent.notify((unsigned int)mResetPeriod, SC_NS);
}//}}}

void Csarad::DeAssertResetMethod(void)
{//{{{
    mSARCmdResetFlag = false;
    mIsCmdResetFlag = false;
    this->EnableReset(mSARCmdResetFlag);
}//}}}

// Check the address ranges of ADC0 and ADC1
void Csarad::CheckBaseAddrMethod (void)
{//{{{
    if (StartAddrADCA0 > EndAddressADCA0) {
        re_printf("warning","The start address of ADC0 0x%X is greater than the end address of ADC0 0x%X.\n", StartAddrADCA0, EndAddressADCA0);
    }
    if (StartAddrADCA1 > EndAddressADCA1) {
        re_printf("warning","The start address of ADC1 0x%X is greater than the end address of ADC1 0x%X.\n", StartAddrADCA1, EndAddressADCA1);
    }
    if (StartAddrADCA2 > EndAddressADCA2) {
        re_printf("warning","The start address of ADC2 0x%X is greater than the end address of ADC0 0x%X.\n", StartAddrADCA2, EndAddressADCA2);
    }
    if (StartAddrADCA3 > EndAddressADCA3) {
        re_printf("warning","The start address of ADC3 0x%X is greater than the end address of ADC1 0x%X.\n", StartAddrADCA3, EndAddressADCA3);
    }
    if (((StartAddrADCA0 <= StartAddrADCA1) && (StartAddrADCA1 < EndAddressADCA0)) ||
        ((StartAddrADCA1 <= StartAddrADCA0) && (StartAddrADCA0 < EndAddressADCA1))) {
        re_printf("warning","The address range of ADC0 and ADC1 are overlapped.\n");
    }
    if (((StartAddrADCA0 <= StartAddrADCA2) && (StartAddrADCA2 < EndAddressADCA0)) ||
        ((StartAddrADCA2 <= StartAddrADCA0) && (StartAddrADCA0 < EndAddressADCA2))) {
        re_printf("warning","The address range of ADC0 and ADC2 are overlapped.\n");
    }
    if (((StartAddrADCA0 <= StartAddrADCA3) && (StartAddrADCA3 < EndAddressADCA0)) ||
        ((StartAddrADCA3 <= StartAddrADCA0) && (StartAddrADCA0 < EndAddressADCA3))) {
        re_printf("warning","The address range of ADC0 and ADC3 are overlapped.\n");
    }
    if (((StartAddrADCA1 <= StartAddrADCA2) && (StartAddrADCA2 < EndAddressADCA1)) ||
        ((StartAddrADCA2 <= StartAddrADCA1) && (StartAddrADCA1 < EndAddressADCA2))) {
        re_printf("warning","The address range of ADC1 and ADC2 are overlapped.\n");
    }
    if (((StartAddrADCA1 <= StartAddrADCA3) && (StartAddrADCA3 < EndAddressADCA1)) ||
        ((StartAddrADCA3 <= StartAddrADCA1) && (StartAddrADCA1 < EndAddressADCA3))) {
        re_printf("warning","The address range of ADC1 and ADC3 are overlapped.\n");
    }
    if (((StartAddrADCA2 <= StartAddrADCA3) && (StartAddrADCA3 < EndAddressADCA2)) ||
        ((StartAddrADCA3 <= StartAddrADCA2) && (StartAddrADCA2 < EndAddressADCA3))) {
        re_printf("warning","The address range of ADC2 and ADC3 are overlapped.\n");
    }
}//}}}

//Function
//tlm_tgt_if function

void Csarad::tgt_acc (tlm::tlm_generic_payload &trans, sc_time &t)
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

    status = AddressArbitrate(command, false, (unsigned int)addr, p_data, size);
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
} //}}}

unsigned int Csarad::tgt_acc_dbg(tlm::tlm_generic_payload &trans)
{//{{{
    //Get information
    tlm::tlm_command command;
    sc_dt::uint64 addr = 0;
    unsigned char *p_data = NULL;
    unsigned int size = 0;
    bool status = this->tgt_get_gp_attribute(command, addr, p_data, size, trans, true);
    sc_assert(p_data != NULL);

    status = AddressArbitrate(command, true, (unsigned int)addr, p_data, size);
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    if (status){
        return size;
    } else {
        return 0;
    }
}//}}}

bool Csarad::AddressArbitrate (tlm::tlm_command command, const bool is_dbg, const unsigned int addr, unsigned char *p_data, const unsigned int size)
{//{{{
    //1. Check data
    sc_assert(p_data != NULL);

    //2. Read & Write process
    unsigned int tmp_addr = addr & OffsetAddMask;
    //2.0 Check address for SMPCR and ODCR in case TARGET PRODUCT is "CCC_D1V1/CCC_D1V2/CCC_D2V1/CCC_D2V2"
    //2.1 Check address for ADSYNSTCR, ADTSYNSTRC, RDRVR
    if ((((OffsetAddMask & StartAddrADCA0) + 0x300) <= tmp_addr) && (tmp_addr <= ((OffsetAddMask & StartAddrADCA0) + 0x310))) {
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

    //2.2 Check address for ADC0
    } else if (((OffsetAddMask & StartAddrADCA0) <= tmp_addr) && (tmp_addr <= (OffsetAddMask & EndAddressADCA0))) {
        if(command == tlm::TLM_WRITE_COMMAND){
            return mADC0->common_tgt_wr(is_dbg, addr, p_data, size);
        }else if(command == tlm::TLM_READ_COMMAND){
            return mADC0->common_tgt_rd(is_dbg, addr, p_data, size);
        }else{
            return true;
        }
    //2.3 Check address for ADC1
    } else if ((((OffsetAddMask & StartAddrADCA1) <= tmp_addr) && (tmp_addr <= (OffsetAddMask & EndAddressADCA1))) && (mADCNum > 1)) {
        if(command == tlm::TLM_WRITE_COMMAND){
            return mADC1->common_tgt_wr(is_dbg, addr, p_data, size);
        }else if(command == tlm::TLM_READ_COMMAND){
            return mADC1->common_tgt_rd(is_dbg, addr, p_data, size);
        }else{
            return true;
        }
    //2.3a Check address for ADC2
    } else if ((((OffsetAddMask & StartAddrADCA2) <= tmp_addr) && (tmp_addr <= (OffsetAddMask & EndAddressADCA2))) && (mADCNum > 2)) {
        if(command == tlm::TLM_WRITE_COMMAND){
            return mADC2->common_tgt_wr(is_dbg, addr, p_data, size);
        }else if(command == tlm::TLM_READ_COMMAND){
            return mADC2->common_tgt_rd(is_dbg, addr, p_data, size);
        }else{
            return true;
        }
    //2.3b Check address for ADC3
    } else if ((((OffsetAddMask & StartAddrADCA3) <= tmp_addr) && (tmp_addr <= (OffsetAddMask & EndAddressADCA3))) && (mADCNum > 3)) {
        if(command == tlm::TLM_WRITE_COMMAND){
            return mADC3->common_tgt_wr(is_dbg, addr, p_data, size);
        }else if(command == tlm::TLM_READ_COMMAND){
            return mADC3->common_tgt_rd(is_dbg, addr, p_data, size);
        }else{
            return true;
        }
    //2.4 Check address for ADOPDIG0
    } else if ((addr == ADOPDIG0Addr) && (TARGET_PRODUCT.value == "RH850_P1M-E")) {
        if(command == tlm::TLM_WRITE_COMMAND){
            return mADC0->common_tgt_wr(is_dbg, emADOPDIGAddr, p_data, size);
        }else if(command == tlm::TLM_READ_COMMAND){
            return mADC0->common_tgt_rd(is_dbg, emADOPDIGAddr, p_data, size);
        }else{
            return true;
        }
    //2.5 Check address for AD1PDIG0
    } else if ((addr == ADOPDIG1Addr) && (TARGET_PRODUCT.value == "RH850_P1M-E")) {
        if(command == tlm::TLM_WRITE_COMMAND){
            return mADC1->common_tgt_wr(is_dbg, emADOPDIGAddr, p_data, size);
        }else if(command == tlm::TLM_READ_COMMAND){
            return mADC1->common_tgt_rd(is_dbg, emADOPDIGAddr, p_data, size);
        }else{
            return true;
        }
    //2.6 Out of SAR AD address
    } else{
        re_printf("error","The access address %08X is out of base address.\n",addr);
        return false;
    }
}//}}}

//Call_back function
void Csarad::cb_ADSYNSTCR_ADSTART(RegCBstr str)
{//{{{
    if((*ADSYNSTCR)["ADSTART"] == 1){
        (*ADSYNSTCR) = 0;
        mADC0->WritingADSYNSTCR();
        mADC1->WritingADSYNSTCR();
        mADC2->WritingADSYNSTCR();
        mADC3->WritingADSYNSTCR();
    }
}//}}}

void Csarad::cb_ADTSYNSTCR_ADTSTART(RegCBstr str)
{//{{{
    if((*ADTSYNSTCR)["ADTSTART"] == 1){
        (*ADTSYNSTCR) = 0;
        mADC0->WritingADTSYNSTCR();
        mADC1->WritingADTSYNSTCR();
        mADC2->WritingADTSYNSTCR();
        mADC3->WritingADTSYNSTCR();
    }
}//}}}

void Csarad::cb_VMONVDCR1_VDE1(RegCBstr str)
{//{{{
    bool CheckCondition = mADC0->WritingVMONVDCR("VMONVDCR1", "VDE1", (*VMONVDCR1)["VDE1"]);

    if ((!CheckCondition) && ((TARGET_PRODUCT.value == "RH850_E2x_fcc1") || (TARGET_PRODUCT.value == "RH850_E2x"))) {  // KhangDinh 2018.12.20
        re_printf("warning","Setting value to register VMONVDCR1 is not affected.\n");
        (*VMONVDCR1) = (unsigned int)(str.pre_data) & 0x01;
    }
}//}}}

void Csarad::cb_VMONVDCR2_VDE2(RegCBstr str)
{//{{{
    bool CheckCondition = mADC0->WritingVMONVDCR("VMONVDCR2", "VDE2", (*VMONVDCR2)["VDE2"]);

    if ((!CheckCondition) && ((TARGET_PRODUCT.value == "RH850_E2x_fcc1") || (TARGET_PRODUCT.value == "RH850_E2x"))) {  // KhangDinh 2018.12.20
        re_printf("warning","Setting value to register VMONVDCR2 is not affected.\n");
        (*VMONVDCR2) = (unsigned int)(str.pre_data) & 0x01;
    }
}//}}}

//Reset function
void Csarad::EnableReset(const bool is_active)
{//{{{
    if(is_active){
        //1.Dump report
        if(mSARADState == emIDLE){
            re_printf("info","SAR AD moves from IDLE state to RESET state.\n");
        }else if(mSARADState == emOPERATING){
            re_printf("info","SAR AD moves from OPERATING state to RESET state.\n");
        }

        //2.Global variable init
        mSARADState = emRESET;
        re_printf("info","The ADC0 and ADC1 are reset.\n");

        //3. Cancel all event
        mADC0->mADCChangeStateEvent.cancel();
        mADC1->mADCChangeStateEvent.cancel();
        mADC2->mADCChangeStateEvent.cancel();
        mADC3->mADCChangeStateEvent.cancel();
    }else{
        re_printf("info","SAR AD moves from RESET state to IDLE state.\n");
        mSARADState =  emIDLE;
    }

    //4.Reset registers
    Csarad_regif::EnableReset(is_active);

    //5.Reset ADC0 and ADC1
    mADC0->EnableReset(is_active);
    mADC1->EnableReset(is_active);
    mADC2->EnableReset(is_active);
    mADC3->EnableReset(is_active);
}//}}}

//handleCommand function
void Csarad::DumpStatInfo(void)
{//{{{
    mADC0->DumpStatInfo();
    mADC1->DumpStatInfo();
    mADC2->DumpStatInfo();
    mADC3->DumpStatInfo();
}//}}}

void Csarad::AssertReset(const double start_time, const double period)
{//{{{
    if((!mSARPortResetFlag)&&(!mIsCmdResetFlag)) {
        mIsCmdResetFlag = true;
        re_printf("info","SAR AD will reset for %lld.%06d ns after %lld.%06d ns.\n", (unsigned long long) period, FRACTION_TO_INT(period), (unsigned long long) start_time, FRACTION_TO_INT(start_time));
        mCmdResetEvent.notify(start_time, SC_NS);
        mResetPeriod = (unsigned int)period;
    }else{
        re_printf("warning","Reset is in progress.\n");
    }
}//}}}

void Csarad::AssignTHCircuit(const unsigned int adc_index, const unsigned int th_channel, const unsigned int group_channel, const unsigned int physic_channel)
{//{{{
    if (adc_index == 0) {
        mADC0->AssignTHCircuit(th_channel, group_channel, physic_channel);
    } else if (adc_index == 1) {
        mADC1->AssignTHCircuit(th_channel, group_channel, physic_channel);
    } else if (adc_index == 2) {
        mADC2->AssignTHCircuit(th_channel, group_channel, physic_channel);
    } else if (adc_index == 3) {
        mADC3->AssignTHCircuit(th_channel, group_channel, physic_channel);
    }
}//}}}

double Csarad::GetTimeResolution(void)
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

void Csarad::SetCLKfreq(std::string clk_name, double clk_freq)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    double time_unit = 0;
    double pclk_period = 0;
    double adclk_period = 0;
    bool is_period_valid = true;

    // Setting clock values
    if (clk_name == "pclk") {
        if (this->mADC0->mADCChangeState || this->mADC1->mADCChangeState || this->mADC2->mADCChangeState || this->mADC3->mADCChangeState) {
            re_printf("warning","The frequency of pclk clock is changed while SAR AD model is operating.\n");
        }

        if (clk_freq == 0) {
            pclk_period = 0;
        } else {
            pclk_period  = (double)(emNanoSecond / clk_freq);
            time_unit    = (double)(emNanoSecond / GetTimeResolution());
            if (time_unit > pclk_period) {
                is_period_valid = false;
                re_printf("warning","The pclk period is less than 1 unit time of system.\n");
            }
        }

        if (is_period_valid) {
            this->mADC0->mPCLKPeriod = pclk_period;
            this->mADC1->mPCLKPeriod = pclk_period;
            this->mADC2->mPCLKPeriod = pclk_period;
            this->mADC3->mPCLKPeriod = pclk_period;
            SetLatency_TLM(pclk_period, false);
            re_printf("info","The pclk is set with a frequency as %lld.%06d\n", (unsigned long long) clk_freq, FRACTION_TO_INT(clk_freq));
        }
    } else if (clk_name == "ADCLK") {
        if (this->mADC0->mADCChangeState || this->mADC1->mADCChangeState || this->mADC2->mADCChangeState || this->mADC3->mADCChangeState) {
            re_printf("warning","The frequency of ADCLK clock is changed while SAR AD model is operating.\n");
        }

        if (clk_freq == 0) {
            adclk_period = 0;
        } else {
            adclk_period = (double)(emNanoSecond / clk_freq);
            time_unit    = (double)(emNanoSecond / GetTimeResolution());
            if (time_unit > adclk_period) {
                is_period_valid = false;
                re_printf("warning","The ADCLK period is less than 1 unit time of system.\n");
            }
        }

        if (is_period_valid) {
            this->mADC0->mADCLKPeriod = adclk_period;
            this->mADC1->mADCLKPeriod = adclk_period;
            this->mADC2->mADCLKPeriod = adclk_period;
            this->mADC3->mADCLKPeriod = adclk_period;
            re_printf("info","The ADCLK is set with a frequency as %lld.%06d\n", (unsigned long long) clk_freq, FRACTION_TO_INT(clk_freq));
        }
    } else {
        re_printf("error","Clock name is invalid.\n");
    }

    // Stop operation when clock frequency = 0
    if (((clk_name == "pclk") || (clk_name == "ADCLK")) && (clk_freq == 0)) {
        mADC0->SettingZeroClock();
        mADC1->SettingZeroClock();
        mADC2->SettingZeroClock();
        mADC3->SettingZeroClock();
    }
}//}}}

void Csarad::SetLatency_TLM(const double pclk_period, const bool is_constructor)
{//{{{
    vpcl::tlm_if_tgt_parameter tgt_param = vpcl::tlm_tgt_if<32>::tgt_get_param();
    if(is_constructor){
        tgt_param.fw_req_phase = tlm::END_REQ;
    }
    sc_time new_clock(pclk_period, SC_NS);
    tgt_param.bus_clk = new_clock;
    tgt_param.rd_latency = tgt_param.rd_req_latency + 2 * tgt_param.bus_clk;
    tgt_param.wr_latency = tgt_param.wr_req_latency + 2 * tgt_param.bus_clk;
    vpcl::tlm_tgt_if<32>::tgt_set_param(&tgt_param);
}//}}}

std::string Csarad::CommandCB(const std::vector<std::string>& args)
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
            this->mADC3->mDumpInterrupt = true;
        }else if (_args[1] == "false") {
            this->mADC0->mDumpInterrupt = false;
            this->mADC1->mDumpInterrupt = false;
            this->mADC2->mDumpInterrupt = false;
            this->mADC3->mDumpInterrupt = false;
        }
    }else if((_args[0] == "EnableConvertInfo")&&((int)_args.size() == 2)){
        if (_args[1] == "true"){
            this->mADC0->mEnableConvertInfo = true;
            this->mADC1->mEnableConvertInfo = true;
            this->mADC2->mEnableConvertInfo = true;
            this->mADC3->mEnableConvertInfo = true;
        }else if (_args[1] == "false"){
            this->mADC0->mEnableConvertInfo = false;
            this->mADC1->mEnableConvertInfo = false;
            this->mADC2->mEnableConvertInfo = false;
            this->mADC3->mEnableConvertInfo = false;
        }
    }else if((_args[0] == "Avrefh0")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing reference voltage Avrefh0 is invalid when simulation time is greater than zero.\n");
            Avrefh0 = mPreAvrefh0;
        }else{
            mPreAvrefh0 = Avrefh0;
            this->mADC0->mAvrefh = Avrefh0;
            mIsRefVol0Update = true;
        }
    }else if((_args[0] == "Avrefh1")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing reference voltage Avrefh1 is invalid when simulation time is greater than zero.\n");
            Avrefh1 = mPreAvrefh1;
        }else{
            mPreAvrefh1 = Avrefh1;
            this->mADC1->mAvrefh = Avrefh1;
            mIsRefVol1Update = true;
        }
    }else if((_args[0] == "Avrefh2")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing reference voltage Avrefh2 is invalid when simulation time is greater than zero.\n");
            Avrefh2 = mPreAvrefh2;
        }else{
            mPreAvrefh2 = Avrefh2;
            this->mADC2->mAvrefh = Avrefh2;
            mIsRefVol2Update = true;
        }
    }else if((_args[0] == "Avrefh3")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing reference voltage Avrefh3 is invalid when simulation time is greater than zero.\n");
            Avrefh3 = mPreAvrefh3;
        }else{
            mPreAvrefh3 = Avrefh3;
            this->mADC3->mAvrefh = Avrefh3;
            mIsRefVol3Update = true;
        }
    }else if((_args[0] == "AVcc0")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing reference voltage AVcc0 is invalid when simulation time is greater than zero.\n");
            AVcc0 = mPreAVcc0;
        }else{
            mPreAVcc0 = AVcc0;
            this->mADC0->mAVcc = AVcc0;
        }
    }else if((_args[0] == "AVcc1")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing reference voltage AVcc1 is invalid when simulation time is greater than zero.\n");
            AVcc1 = mPreAVcc1;
        }else{
            mPreAVcc1 = AVcc1;
            this->mADC1->mAVcc = AVcc1;
        }
    }else if((_args[0] == "AVcc2")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing reference voltage AVcc2 is invalid when simulation time is greater than zero.\n");
            AVcc2 = mPreAVcc2;
        }else{
            mPreAVcc2 = AVcc2;
            this->mADC2->mAVcc = AVcc2;
        }
    }else if((_args[0] == "AVcc3")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing reference voltage AVcc3 is invalid when simulation time is greater than zero.\n");
            AVcc3 = mPreAVcc3;
        }else{
            mPreAVcc3 = AVcc3;
            this->mADC3->mAVcc = AVcc3;
        }
    }else if((_args[0] == "tTSPL")&&((int)_args.size() == 2)){
        if (this->mADC0->mADCChangeState || this->mADC1->mADCChangeState || this->mADC2->mADCChangeState || this->mADC3->mADCChangeState) {
            re_printf("warning","Changing the tTSPL parameter is not affected when SAR AD model is operating.\n");
            tTSPL = mPretTSPL;
        }else{
            mPretTSPL = tTSPL;
            this->mADC0->mtTSPL = tTSPL;
            this->mADC1->mtTSPL = tTSPL;
            this->mADC2->mtTSPL = tTSPL;
            this->mADC3->mtTSPL = tTSPL;
        }
    }else if((_args[0] == "tTHLD")&&((int)_args.size() == 2)){
        if (this->mADC0->mADCChangeState || this->mADC1->mADCChangeState || this->mADC2->mADCChangeState || this->mADC3->mADCChangeState) {
            re_printf("warning","Changing the tTHLD parameter is not affected when SAR AD model is operating.\n");
            tTHLD = mPretTHLD;
        }else{
            mPretTHLD = tTHLD;
            this->mADC0->mtTHLD = tTHLD;
            this->mADC1->mtTHLD = tTHLD;
            this->mADC2->mtTHLD = tTHLD;
            this->mADC3->mtTHLD = tTHLD;
        }
    }else if((_args[0] == "tTHSD")&&((int)_args.size() == 2)){
        if (this->mADC0->mADCChangeState || this->mADC1->mADCChangeState || this->mADC2->mADCChangeState || this->mADC3->mADCChangeState) {
            re_printf("warning","Changing the tTHSD parameter is not affected when SAR AD model is operating.\n");
            tTHSD = mPretTHSD;
        }else{
            mPretTHSD = tTHSD;
            this->mADC0->mtTHSD = tTHSD;
            this->mADC1->mtTHSD = tTHSD;
            this->mADC2->mtTHSD = tTHSD;
            this->mADC3->mtTHSD = tTHSD;
        }
    }else if((_args[0] == "ExSmpt")&&((int)_args.size() == 2)){
        if (this->mADC0->mADCChangeState || this->mADC1->mADCChangeState || this->mADC2->mADCChangeState || this->mADC3->mADCChangeState) {
            re_printf("warning","Changing the ExSmpt parameter is not affected when SAR AD model is operating.\n");
            ExSmpt = mPreExSmpt;
        }else{
            mPreExSmpt = ExSmpt;
            this->mADC0->mExSmpt = ExSmpt;
            this->mADC1->mExSmpt = ExSmpt;
            this->mADC2->mExSmpt = ExSmpt;
            this->mADC3->mExSmpt = ExSmpt;
        }
    }else if((_args[0] == "tSAR")&&((int)_args.size() == 2)){
        if (this->mADC0->mADCChangeState || this->mADC1->mADCChangeState || this->mADC2->mADCChangeState || this->mADC3->mADCChangeState) {
            re_printf("warning","Changing the tSAR parameter is not affected when SAR AD model is operating.\n");
            tSAR = mPretSAR;
        }else{
            mPretSAR = tSAR;
            this->mADC0->mtSAR = tSAR;
            this->mADC1->mtSAR = tSAR;
            this->mADC2->mtSAR = tSAR;
            this->mADC3->mtSAR = tSAR;
        }
    }else if((_args[0] == "tD")&&((int)_args.size() == 2)){
        if (this->mADC0->mADCChangeState || this->mADC1->mADCChangeState || this->mADC2->mADCChangeState || this->mADC3->mADCChangeState) {
            re_printf("warning","Changing the tD parameter is not affected when SAR AD model is operating.\n");
            tD = mPretD;
        }else{
            mPretD = tD;
            this->mADC0->mtD = tD;
            this->mADC1->mtD = tD;
            this->mADC2->mtD = tD;
            this->mADC3->mtD = tD;
        }
    }else if((_args[0] == "tED")&&((int)_args.size() == 2)){
        if (this->mADC0->mADCChangeState || this->mADC1->mADCChangeState || this->mADC2->mADCChangeState || this->mADC3->mADCChangeState) {
            re_printf("warning","Changing the tED parameter is not affected when SAR AD model is operating.\n");
            tED = mPretED;
        }else{
            mPretED = tED;
            this->mADC0->mtED = tED;
            this->mADC1->mtED = tED;
            this->mADC2->mtED = tED;
            this->mADC3->mtED = tED;
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
    }else if((_args[0] == "ForcedIdError3")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            this->mADC3->mForcedIdError = true;
        }else if (_args[1] == "false") {
            this->mADC3->mForcedIdError = false;
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
    }else if((_args[0] == "ForcedParityError3")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            this->mADC3->mForcedParityError = true;
        }else if (_args[1] == "false") {
            this->mADC3->mForcedParityError = false;
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
    }else if((_args[0] == "ForcedOpenError3")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            this->mADC3->mForcedOpenError = true;
        }else if (_args[1] == "false") {
            this->mADC3->mForcedOpenError = false;
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
    }else if((_args[0] == "ForcedBreak3")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            this->mADC3->mForcedBreak = true;
        }else if (_args[1] == "false") {
            this->mADC3->mForcedBreak = false;
        }
    }else if((_args[0] == "StartAddrADCA0")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing start address of ADC0 is invalid when simulation time is greater than zero.\n");
            StartAddrADCA0 = mPreStartAddrADCA0;
        }else{
            mPreStartAddrADCA0 = StartAddrADCA0;
        }
    }else if((_args[0] == "StartAddrADCA1")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing start address of ADC1 is invalid when simulation time is greater than zero.\n");
            StartAddrADCA1 = mPreStartAddrADCA1;
        }else{
            mPreStartAddrADCA1 = StartAddrADCA1;
        }
    }else if((_args[0] == "StartAddrADCA2")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing start address of ADC2 is invalid when simulation time is greater than zero.\n");
            StartAddrADCA2 = mPreStartAddrADCA2;
        }else{
            mPreStartAddrADCA2 = StartAddrADCA2;
        }
    }else if((_args[0] == "StartAddrADCA3")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing start address of ADC3 is invalid when simulation time is greater than zero.\n");
            StartAddrADCA3 = mPreStartAddrADCA3;
        }else{
            mPreStartAddrADCA3 = StartAddrADCA3;
        }
    }else if((_args[0] == "EndAddressADCA0")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing end address of ADC0 is invalid when simulation time is greater than zero.\n");
            EndAddressADCA0 = mPreEndAddressADCA0;
        }else{
            mPreEndAddressADCA0 = EndAddressADCA0;
        }
    }else if((_args[0] == "EndAddressADCA1")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing end address of ADC1 is invalid when simulation time is greater than zero.\n");
            EndAddressADCA1 = mPreEndAddressADCA1;
        }else{
            mPreEndAddressADCA1 = EndAddressADCA1;
        }
    }else if((_args[0] == "EndAddressADCA2")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing end address of ADC2 is invalid when simulation time is greater than zero.\n");
            EndAddressADCA2 = mPreEndAddressADCA2;
        }else{
            mPreEndAddressADCA2 = EndAddressADCA2;
        }
    }else if((_args[0] == "EndAddressADCA3")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing end address of ADC3 is invalid when simulation time is greater than zero.\n");
            EndAddressADCA3 = mPreEndAddressADCA3;
        }else{
            mPreEndAddressADCA3 = EndAddressADCA3;
        }
    }else if((_args[0] == "OffsetAddMask")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing offset address mask is invalid when simulation time is greater than zero.\n");
            OffsetAddMask = mPreOffsetAddMask;
        }else{
            mPreOffsetAddMask = OffsetAddMask;
        }
    }else if((_args[0] == "EnableTimeCalculation")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing delay times calculation is invalid when simulation time is greater than zero.\n");
            EnableTimeCalculation = mPreEnableTimeCalculation;
        }else{
            mPreEnableTimeCalculation = EnableTimeCalculation;
            this->mADC0->mEnableTimeCalculation = EnableTimeCalculation;
            this->mADC1->mEnableTimeCalculation = EnableTimeCalculation;
            this->mADC2->mEnableTimeCalculation = EnableTimeCalculation;
            this->mADC3->mEnableTimeCalculation = EnableTimeCalculation;
        }
    /// SARAD3
    }else if((_args[0] == "AVccMax")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing reference voltage AVccMax is invalid when simulation time is greater than zero.\n");
            AVccMax = mPreAVccMax;
        }else{
            mPreAVccMax = AVccMax;
            this->mADC0->mAVccMax = AVccMax;
            this->mADC1->mAVccMax = AVccMax;
            this->mADC2->mAVccMax = AVccMax;
            this->mADC3->mAVccMax = AVccMax;
        }
    }else if((_args[0] == "AVccMin")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing reference voltage AVccMin is invalid when simulation time is greater than zero.\n");
            AVccMin = mPreAVccMin;
        }else{
            mPreAVccMin = AVccMin;
            this->mADC0->mAVccMin = AVccMin;
            this->mADC1->mAVccMin = AVccMin;
            this->mADC2->mAVccMin = AVccMin;
            this->mADC3->mAVccMin = AVccMin;
        }
    }else if((_args[0] == "AvrefhMin")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing reference voltage AvrefhMin is invalid when simulation time is greater than zero.\n");
            AvrefhMin = mPreAvrefhMin;
        }else{
            mPreAvrefhMin = AvrefhMin;
            this->mADC0->mAvrefhMin = AvrefhMin;
            this->mADC1->mAvrefhMin = AvrefhMin;
            this->mADC2->mAvrefhMin = AvrefhMin;
            this->mADC3->mAvrefhMin = AvrefhMin;
        }
    }
    return ret;
}//}}}


/// SARAD4 add
// Check & set attribute values to ADC modules
void Csarad::UpdateChannelNumMethod(void)
{//{{{
    unsigned int found_index = 0;
    unsigned int ADC0_phy_num = emP_SADC_SARCHNUM_Def;
    unsigned int ADC1_phy_num = emP_SADC_SARCHNUM_Def;
    unsigned int ADC2_phy_num = emP_SADC_SARCHNUM_Def;
    unsigned int ADC3_phy_num = emP_SADC_SARCHNUM_Def;
    unsigned int ADC0_vir_num = emVCRnum_Def;
    unsigned int ADC1_vir_num = emVCRnum_Def;
    unsigned int ADC2_vir_num = emVCRnum_Def;
    unsigned int ADC3_vir_num = emVCRnum_Def;
    unsigned int ADC0_MPX_ch = emEX_MPX_PCH0_Def;
    unsigned int ADC1_MPX_ch = emEX_MPX_PCH1_Def;
    unsigned int ADC2_MPX_ch = emEX_MPX_PCH2_Def;
    unsigned int ADC3_MPX_ch = emEX_MPX_PCH3_Def;
    unsigned int ADC0_TH_ch = emTH_CHNUM_Def;
    unsigned int ADC1_TH_ch = emTH_CHNUM_Def;
    unsigned int ADC2_TH_ch = emTH_CHNUM_Def;
    unsigned int ADC3_TH_ch = emTH_CHNUM_Def;
    mADCNum = emADC_NUM_Def;

    re_printf("info","TARGET_PRODUCT is %s .\n", TARGET_PRODUCT.value.c_str());

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
        ADC3_phy_num = mTargetConfigTable[found_index].physical_ch_num3;
        ADC0_vir_num = mTargetConfigTable[found_index].virtual_ch_num0;
        ADC1_vir_num = mTargetConfigTable[found_index].virtual_ch_num1;
        ADC2_vir_num = mTargetConfigTable[found_index].virtual_ch_num2;
        ADC3_vir_num = mTargetConfigTable[found_index].virtual_ch_num3;
        ADC0_MPX_ch = mTargetConfigTable[found_index].external_mpx_pch0;
        ADC1_MPX_ch = mTargetConfigTable[found_index].external_mpx_pch1;
        ADC2_MPX_ch = mTargetConfigTable[found_index].external_mpx_pch2;
        ADC3_MPX_ch = mTargetConfigTable[found_index].external_mpx_pch3;
        ADC0_TH_ch = mTargetConfigTable[found_index].th_channel_num0;
        ADC1_TH_ch = mTargetConfigTable[found_index].th_channel_num1;
        ADC2_TH_ch = mTargetConfigTable[found_index].th_channel_num2;
        ADC3_TH_ch = mTargetConfigTable[found_index].th_channel_num3;
    } else { // for custom configuration
        // already set in initial variables
    }

    // Use defined value if it configured
    if (ADC_NUM.value != "") {
        mADCNum = (unsigned int) strtoul(ADC_NUM.value.c_str(),NULL,0);
    }
    if (P_SADC_SARCHNUM0.value != "") {
        ADC0_phy_num = (unsigned int) strtoul(P_SADC_SARCHNUM0.value.c_str(),NULL,0);
    }
    if (P_SADC_SARCHNUM1.value != "") {
        ADC1_phy_num = (unsigned int) strtoul(P_SADC_SARCHNUM1.value.c_str(),NULL,0);
    }
    if (P_SADC_SARCHNUM2.value != "") {
        ADC2_phy_num = (unsigned int) strtoul(P_SADC_SARCHNUM2.value.c_str(),NULL,0);
    }
    if (P_SADC_SARCHNUM3.value != "") {
        ADC3_phy_num = (unsigned int) strtoul(P_SADC_SARCHNUM3.value.c_str(),NULL,0);
    }
    if (VCRnum0.value != "") {
        ADC0_vir_num = (unsigned int) strtoul(VCRnum0.value.c_str(),NULL,0);
    }
    if (VCRnum1.value != "") {
        ADC1_vir_num = (unsigned int) strtoul(VCRnum1.value.c_str(),NULL,0);
    }
    if (VCRnum2.value != "") {
        ADC2_vir_num = (unsigned int) strtoul(VCRnum2.value.c_str(),NULL,0);
    }
    if (VCRnum3.value != "") {
        ADC3_vir_num = (unsigned int) strtoul(VCRnum3.value.c_str(),NULL,0);
    }
    if (EX_MPX_PCH0.value != "") {
        ADC0_MPX_ch = (unsigned int) strtoul(EX_MPX_PCH0.value.c_str(),NULL,0);
    }
    if (EX_MPX_PCH1.value != "") {
        ADC1_MPX_ch = (unsigned int) strtoul(EX_MPX_PCH1.value.c_str(),NULL,0);
    }
    if (EX_MPX_PCH2.value != "") {
        ADC2_MPX_ch = (unsigned int) strtoul(EX_MPX_PCH2.value.c_str(),NULL,0);
    }
    if (EX_MPX_PCH3.value != "") {
        ADC3_MPX_ch = (unsigned int) strtoul(EX_MPX_PCH3.value.c_str(),NULL,0);
    }

    // Stop simulation if attributes is set with wrong value
    /// SARAD4 modify - fix sc_assert() issue on SCHEAP
    re_sc_assert(emADC_NUM_Min <= mADCNum, "The number of ADC module is less than 1");
    re_sc_assert(emADC_NUM_Max >= mADCNum, "The number of ADC module is greater than 4");
    re_sc_assert(emP_SADC_SARCHNUM_Min <= ADC0_phy_num , "The number of physical channels of ADC0 is less than 1");
    re_sc_assert(ADC0_phy_num <= emP_SADC_SARCHNUM_Max , "The number of physical channels of ADC0 is greater than 32");
    re_sc_assert(emVCRnum_Min <= ADC0_vir_num , "The number of virtual channels of ADC0 is less than 16");
    re_sc_assert(ADC0_vir_num <= emVCRnum_Max , "The number of virtual channels of ADC0 is greater than 48");
    re_sc_assert((ADC0_vir_num & 0x3) == 0 , "The number of virtual channels of ADC0 is not a multiple of 4");
    re_sc_assert(ADC0_MPX_ch < ADC0_phy_num , "The analog channel for MPX of ADC0 is greater than the maximum index of physical channel");
    ///// re_sc_assert(ADC0_TH_ch <= emTH_CHNUM_Max , "The TH channel of ADC0 is greater than 6");

    // E2x added
    this->mADC0->mADCNum = mADCNum;
    this->mADC1->mADCNum = mADCNum;
    this->mADC2->mADCNum = mADCNum;
    this->mADC3->mADCNum = mADCNum;

    // Update value to ADC0
    this->mADC0->mTargetProduct = TARGET_PRODUCT.value;
    this->mADC0->mPhyChannelNum = ADC0_phy_num;
    this->mADC0->mVirChannelNum = ADC0_vir_num;
    this->mADC0->mMPXInput = ADC0_MPX_ch;
    this->mADC0->mTHChannelNum = ADC0_TH_ch;
    re_printf("info","ADC0 is configured with P_SADC_SARCHNUM0 = %d, VCRnum0 = %d, EX_MPX_PCH0 = %d, TH channel = %d.\n", ADC0_phy_num, ADC0_vir_num, ADC0_MPX_ch,ADC0_TH_ch);

    if (mADCNum > 1) { // if there is ADC1
        re_sc_assert(emP_SADC_SARCHNUM_Min <= ADC1_phy_num , "The number of physical channels of ADC1 is less than 1");
        re_sc_assert(ADC1_phy_num <= emP_SADC_SARCHNUM_Max , "The number of physical channels of ADC1 is greater than 32");
        re_sc_assert(emVCRnum_Min <= ADC1_vir_num , "The number of virtual channels of ADC1 is less than 16");
        re_sc_assert(ADC1_vir_num <= emVCRnum_Max , "The number of virtual channels of ADC1 is greater than 48");
        re_sc_assert((ADC1_vir_num & 0x3) == 0 , "The number of virtual channels of ADC1 is not a multiple of 4");
        re_sc_assert(ADC1_MPX_ch < ADC1_phy_num , "The analog channel for MPX of ADC1 is greater than the maximum index of physical channel");
        ///// re_sc_assert(ADC1_TH_ch <= emTH_CHNUM_Max , "The TH channel of ADC1 is greater than 6");
        // Update value to ADC1
        this->mADC1->mTargetProduct = TARGET_PRODUCT.value;
        this->mADC1->mPhyChannelNum = ADC1_phy_num;
        this->mADC1->mVirChannelNum = ADC1_vir_num;
        this->mADC1->mMPXInput = ADC1_MPX_ch;
        this->mADC1->mTHChannelNum = ADC1_TH_ch;
        re_printf("info","ADC1 is configured with P_SADC_SARCHNUM1 = %d, VCRnum1 = %d, EX_MPX_PCH1 = %d, TH channel = %d.\n", ADC1_phy_num, ADC1_vir_num, ADC1_MPX_ch, ADC1_TH_ch);
    }
    if (mADCNum > 2) { // if there is ADC2
        re_sc_assert(emP_SADC_SARCHNUM_Min <= ADC2_phy_num , "The number of physical channels of ADC2 is less than 1");
        re_sc_assert(ADC2_phy_num <= emP_SADC_SARCHNUM_Max , "The number of physical channels of ADC2 is greater than 32");
        re_sc_assert(emVCRnum_Min <= ADC2_vir_num , "The number of virtual channels of ADC2 is less than 16");
        re_sc_assert(ADC2_vir_num <= emVCRnum_Max , "The number of virtual channels of ADC2 is greater than 48");
        re_sc_assert((ADC2_vir_num & 0x3) == 0 , "The number of virtual channels of ADC2 is not a multiple of 4");
        re_sc_assert(ADC2_MPX_ch < ADC2_phy_num , "The analog channel for MPX of ADC2 is greater than the maximum index of physical channel");
        ///// re_sc_assert(ADC2_TH_ch <= emTH_CHNUM_Max , "The TH channel of ADC2 is greater than 6");
        // Update value to ADC2
        this->mADC2->mTargetProduct = TARGET_PRODUCT.value;
        this->mADC2->mPhyChannelNum = ADC2_phy_num;
        this->mADC2->mVirChannelNum = ADC2_vir_num;
        this->mADC2->mMPXInput = ADC2_MPX_ch;
        this->mADC2->mTHChannelNum = ADC2_TH_ch;
        re_printf("info","ADC2 is configured with P_SADC_SARCHNUM2 = %d, VCRnum2 = %d, EX_MPX_PCH2 = %d, TH channel = %d.\n", ADC2_phy_num, ADC2_vir_num, ADC2_MPX_ch, ADC2_TH_ch);
    }
    if (mADCNum > 3) { // if there is ADC3
        re_sc_assert(emP_SADC_SARCHNUM_Min <= ADC3_phy_num , "The number of physical channels of ADC3 is less than 1");
        re_sc_assert(ADC3_phy_num <= emP_SADC_SARCHNUM_Max , "The number of physical channels of ADC3 is greater than 32");
        re_sc_assert(emVCRnum_Min <= ADC3_vir_num , "The number of virtual channels of ADC3 is less than 16");
        re_sc_assert(ADC3_vir_num <= emVCRnum_Max , "The number of virtual channels of ADC3 is greater than 48");
        re_sc_assert((ADC3_vir_num & 0x3) == 0 , "The number of virtual channels of ADC3 is not a multiple of 4");
        re_sc_assert(ADC3_MPX_ch < ADC3_phy_num , "The analog channel for MPX of ADC3 is greater than the maximum index of physical channel");
        ///// re_sc_assert(ADC3_TH_ch <= emTH_CHNUM_Max , "The TH channel of ADC3 is greater than 6");
        // Update value to ADC3
        this->mADC3->mTargetProduct = TARGET_PRODUCT.value;
        this->mADC3->mPhyChannelNum = ADC3_phy_num;
        this->mADC3->mVirChannelNum = ADC3_vir_num;
        this->mADC3->mMPXInput = ADC3_MPX_ch;
        this->mADC3->mTHChannelNum = ADC3_TH_ch;
        re_printf("info","ADC3 is configured with P_SADC_SARCHNUM3 = %d, VCRnum3 = %d, EX_MPX_PCH3 = %d, TH channel = %d.\n", ADC3_phy_num, ADC3_vir_num, ADC3_MPX_ch, ADC3_TH_ch);
    }
}//}}}

// vim600: set foldmethod=marker :
