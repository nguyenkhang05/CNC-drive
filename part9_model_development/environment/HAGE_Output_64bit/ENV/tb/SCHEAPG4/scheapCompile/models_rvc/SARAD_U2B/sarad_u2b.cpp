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

#include "re_define.h"
#include "sarad_u2b.h"

/// SARAD4 add - fix sc_assert() issue on SCHEAP
#define re_sc_assert(expr, msg)     ((void) ((expr) ? 0 : (SC_REPORT_FATAL( sc_core::SC_ID_ASSERTION_FAILED_ , msg), 0)))

//Constructor of SARAD class
Csarad_u2b::Csarad_u2b(sc_module_name name):
         vpcl::tlm_tgt_if<32,tlm::tlm_base_protocol_types,5>(name)
        , Csarad_u2b_regif((std::string)name, 32)
        , pclk("pclk")
        , ADCLK("ADCLK")
        , CLKA_ADC("CLKA_ADC")
        , preset_n("preset_n")
        , RESETAD_N("RESETAD_N")
        , preseta_n("preseta_n")
        , RESETADA_N("RESETADA_N")
        , AVREFH0("AVREFH0")
        , AVREFH1("AVREFH1")
        , AVREFH2("AVREFH2")
        , AVREFH3("AVREFH3")
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
        , AD3SG0TRG("AD3SG0TRG")
        , AD3SG1TRG("AD3SG1TRG")
        , AD3SG2TRG("AD3SG2TRG")
        , AD3SG3TRG("AD3SG3TRG")
        , AD3SG4TRG("AD3SG4TRG")
        , ADASG0TRG("ADASG0TRG")
        , ADASG1TRG("ADASG1TRG")
        , ADASG2TRG("ADASG2TRG")
        , ADASG3TRG("ADASG3TRG")
        , ADASG4TRG("ADASG4TRG")
        , ADxTAUD2I7("ADxTAUD2I7")
        , ADxTAUD2I15("ADxTAUD2I15")
        , ADxTAUJ2("ADxTAUJ2")
        , ADxTAUJ3("ADxTAUJ3")
        , ADxLPS("ADxLPS")
        , AD0SGDTRG("AD0SGDTRG")
        , AD1SGDTRG("AD1SGDTRG")
        , AD2SGDTRG("AD2SGDTRG")
        , AD3SGDTRG("AD3SGDTRG")
        , ADASGDTRG("ADASGDTRG")
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
        , AN050("AN050")
        , AN051("AN051")
        , AN052("AN052")
        , AN053("AN053")
        , AN060("AN060")
        , AN061("AN061")
        , AN062("AN062")
        , AN063("AN063")
        , VMON_VCC("VMON_VCC")
        , VMON_E0VCC("VMON_E0VCC")
        , VMON_ISOVDD("VMON_ISOVDD")
        , VMON_AWOVDD("VMON_AWOVDD")
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
        , AN272("AN272")
        , AN273("AN273")
        , AN300("AN300")
        , AN301("AN301")
        , AN302("AN302")
        , AN303("AN303")
        , AN310("AN310")
        , AN311("AN311")
        , AN312("AN312")
        , AN313("AN313")
        , AN320("AN320")
        , AN321("AN321")
        , AN322("AN322")
        , AN323("AN323")
        , AN330("AN330")
        , AN331("AN331")
        , AN332("AN332")
        , AN333("AN333")
        , AN340("AN340")
        , AN341("AN341")
        , AN342("AN342")
        , AN343("AN343")
        , AN350("AN350")
        , AN351("AN351")
        , AN352("AN352")
        , AN353("AN353")
        , AN360("AN360")
        , AN361("AN361")
        , AN362("AN362")
        , AN363("AN363")
        , AN370("AN370")
        , AN371("AN371")
        , AN372("AN372")
        , AN373("AN373")
        , AN380("AN380")
        , AN381("AN381")
        , AN382("AN382")
        , AN383("AN383")
        , AN390("AN390")
        , AN391("AN391")
        , AN392("AN392")
        , AN393("AN393")
        , ANA00("ANA00")
        , ANA01("ANA01")
        , ANA02("ANA02")
        , ANA03("ANA03")
        , ANA10("ANA10")
        , ANA11("ANA11")
        , ANA12("ANA12")
        , ANA13("ANA13")
        , ANA20("ANA20")
        , ANA21("ANA21")
        , ANA22("ANA22")
        , ANA23("ANA23")
        , ANA30("ANA30")
        , ANA31("ANA31")
        , ANA32("ANA32")
        , ANA33("ANA33")
        , ANA40("ANA40")
        , ANA41("ANA41")
        , ANA42("ANA42")
        , ANA43("ANA43")
        , ANA50("ANA50")
        , ANA51("ANA51")
        , ANA52("ANA52")
        , ANA53("ANA53")
        , ANA60("ANA60")
        , ANA61("ANA61")
        , ANA62("ANA62")
        , ANA63("ANA63")
        , ANA70("ANA70")
        , ANA71("ANA71")
        , IFVCH0("IFVCH0")
        , IFVCH1("IFVCH1")
        , IFVCH2("IFVCH2")
        , IFVCH3("IFVCH3")
        , IFVCHA("IFVCHA")
        , IFRD0("IFRD0")
        , IFRD1("IFRD1")
        , IFRD2("IFRD2")
        , IFRD3("IFRD3")
        , IFRDA("IFRDA")
        , IFRE0("IFRE0")
        , IFRE1("IFRE1")
        , IFRE2("IFRE2")
        , IFRE3("IFRE3")
        , IFREA("IFREA")
        , PVCR_VALUE0("PVCR_VALUE0")
        , PVCR_VALUE1("PVCR_VALUE1")
        , PVCR_VALUE2("PVCR_VALUE2")
        , PVCR_VALUE3("PVCR_VALUE3")
        , PVCR_VALUEA("PVCR_VALUEA")
        , PVCR_TRG0("PVCR_TRG0")
        , PVCR_TRG1("PVCR_TRG1")
        , PVCR_TRG2("PVCR_TRG2")
        , PVCR_TRG3("PVCR_TRG3")
        , PVCR_TRGA("PVCR_TRGA")
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
        , INT_AD30("INT_AD30")
        , INT_AD31("INT_AD31")
        , INT_AD32("INT_AD32")
        , INT_AD33("INT_AD33")
        , INT_AD34("INT_AD34")
        , INT_ADA0("INT_ADA0")
        , INT_ADA1("INT_ADA1")
        , INT_ADA2("INT_ADA2")
        , INT_ADA3("INT_ADA3")
        , INT_ADA4("INT_ADA4")
        , INT_MPX0("INT_MPX0")
        , INT_MPX1("INT_MPX1")
        , INT_MPX2("INT_MPX2")
        , INT_MPX3("INT_MPX3")
        , INT_MPXA("INT_MPXA")
        , ADPE0("ADPE0")
        , ADPE1("ADPE1")
        , ADPE2("ADPE2")
        , ADPE3("ADPE3")
        , ADPEA("ADPEA")
        , INT_SGD0("INT_SGD0")
        , INT_SGD1("INT_SGD1")
        , INT_SGD2("INT_SGD2")
        , INT_SGD3("INT_SGD3")
        , INT_SGDA("INT_SGDA")
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
        , ADEND30("ADEND30")
        , ADEND31("ADEND31")
        , ADEND32("ADEND32")
        , ADEND33("ADEND33")
        , ADEND34("ADEND34")
        , ADENDA0("ADENDA0")
        , ADENDA1("ADENDA1")
        , ADENDA2("ADENDA2")
        , ADENDA3("ADENDA3")
        , ADENDA4("ADENDA4")
        , INT_ERR0("INT_ERR0")
        , INT_ERR1("INT_ERR1")
        , INT_ERR2("INT_ERR2")
        , INT_ERR3("INT_ERR3")
        , INT_ERRA("INT_ERRA")
        , ADC0_IFDATA("ADC0_IFDATA")
        , ADC1_IFDATA("ADC1_IFDATA")
        , ADC2_IFDATA("ADC2_IFDATA")
        , ADC3_IFDATA("ADC3_IFDATA")
        , ADCA_IFDATA("ADCA_IFDATA")
        , DFREQ0("DFREQ0")
        , DFREQ1("DFREQ1")
        , DFREQ2("DFREQ2")
        , DFREQ3("DFREQ3")
        , DFREQA("DFREQA")
        , DFTAG0("DFTAG0")
        , DFTAG1("DFTAG1")
        , DFTAG2("DFTAG2")
        , DFTAG3("DFTAG3")
        , DFTAGA("DFTAGA")
        , DFDATA0("DFDATA0")
        , DFDATA1("DFDATA1")
        , DFDATA2("DFDATA2")
        , DFDATA3("DFDATA3")
        , DFDATAA("DFDATAA")
        , PVCR_END0("PVCR_END0")
        , PVCR_END1("PVCR_END1")
        , PVCR_END2("PVCR_END2")
        , PVCR_END3("PVCR_END3")
        , PVCR_ENDA("PVCR_ENDA")
        , PVCR_PWDDR0("PVCR_PWDDR0")
        , PVCR_PWDDR1("PVCR_PWDDR1")
        , PVCR_PWDDR2("PVCR_PWDDR2")
        , PVCR_PWDDR3("PVCR_PWDDR3")
        , PVCR_PWDDRA("PVCR_PWDDRA")
        , PWM_VCEND0("PWM_VCEND0")
        , VCULMO0("VCULMO0")
        , VCLLMO0("VCLLMO0")
        , PWM_VCEND1("PWM_VCEND1")
        , VCULMO1("VCULMO1")
        , VCLLMO1("VCLLMO1")
        , PWM_VCEND2("PWM_VCEND2")
        , VCULMO2("VCULMO2")
        , VCLLMO2("VCLLMO2")
        , PWM_VCEND3("PWM_VCEND3")
        , VCULMO3("VCULMO3")
        , VCLLMO3("VCLLMO3")
        , PWM_VCENDA("PWM_VCENDA")
        , VCULMOA("VCULMOA")
        , VCLLMOA("VCLLMOA")
        , MPXCUR0("MPXCUR0")
        , MPXCUR1("MPXCUR1")
        , MPXCUR2("MPXCUR2")
        , MPXCUR3("MPXCUR3")
        , MPXCURA("MPXCURA")
        , ULE_LPS0("ULE_LPS0")
        , ULE_LPS1("ULE_LPS1")
        , ULE_LPS2("ULE_LPS2")
        , ULE_LPS3("ULE_LPS3")
        , ULE_LPSA("ULE_LPSA")
        // Attributes
        , TARGET_PRODUCT("TARGET_PRODUCT", "RH850_U2B")
        , ADC_NUM("ADC_NUM", "")
        , P_SADC_SARCHNUM0("P_SADC_SARCHNUM0", "")
        , P_SADC_SARCHNUM1("P_SADC_SARCHNUM1", "")
        , P_SADC_SARCHNUM2("P_SADC_SARCHNUM2", "")
        , P_SADC_SARCHNUM3("P_SADC_SARCHNUM3", "")
        , P_SADC_SARCHNUMA("P_SADC_SARCHNUMA", "")
        , VCRnum0("VCRnum0", "")
        , VCRnum1("VCRnum1", "")
        , VCRnum2("VCRnum2", "")
        , VCRnum3("VCRnum3", "")
        , VCRnumA("VCRnumA", "")
        , EX_MPX_PCH0("EX_MPX_PCH0", "")
        , EX_MPX_PCH1("EX_MPX_PCH1", "")
        , EX_MPX_PCH2("EX_MPX_PCH2", "")
        , EX_MPX_PCH3("EX_MPX_PCH3", "")
        , EX_MPX_PCHA("EX_MPX_PCHA", "")
        , const_double("const_double")
        , ADCx_sig_stubs("ADCx_sig_stubs")
{//{{{

    //Initalze handleCommand
    CommandInit(this->name());
    Csarad_u2b_regif::set_instance_name(this->name());

    for (unsigned int index = 0; index < emVCRnum_Max; index++){
        std::ostringstream str_vcend[emADC_NUM_Max];
        for (unsigned int adc_num = 0; adc_num < emADC_NUM_Max; adc_num++){
            str_vcend[adc_num]<<"vcend"<<adc_num<<"_"<<index;
        }
        VCEND0[index] = new sc_out<bool>(str_vcend[0].str().c_str());
        VCEND1[index] = new sc_out<bool>(str_vcend[1].str().c_str());
        VCEND2[index] = new sc_out<bool>(str_vcend[2].str().c_str());
        VCEND3[index] = new sc_out<bool>(str_vcend[3].str().c_str());
        VCENDA[index] = new sc_out<bool>(str_vcend[4].str().c_str());
        VCEND0[index]->initialize(0);
        VCEND1[index]->initialize(0);
        VCEND2[index]->initialize(0);
        VCEND3[index]->initialize(0);
        VCENDA[index]->initialize(0);
    }

    //Instance ADC0, ADC1 and ADC2 modules and connect ports
    mADC0 = new Cadc_u2b ("mADC0", 0);
    sc_assert(mADC0 != NULL);
    mADC1 = new Cadc_u2b ("mADC1", 1);
    sc_assert(mADC1 != NULL);
    mADC2 = new Cadc_u2b ("mADC2", 2);
    sc_assert(mADC2 != NULL);
    mADC3 = new Cadc_u2b ("mADC3", 3);
    sc_assert(mADC3 != NULL);
    mADCA = new Cadc_u2b ("mADCA", 4);
    sc_assert(mADCA != NULL);

    ConnectPort();

    //Initialze variable
    mIsRefVol0Update    = false;
    mIsRefVol1Update    = false;
    mIsRefVol2Update    = false;
    mIsRefVol3Update    = false;
    mIsADInitialize     = true;
    mIsPInitialize      = true;
    mIsADInitializeA    = true;
    mIsPInitializeA     = true;
    mPResetPeriod       = 0;
    mADResetPeriod      = 0;
    mPResetPeriodA      = 0;
    mADResetPeriodA     = 0;
    mIsCmdPResetFlag    = false;
    mIsCmdADResetFlag   = false;
    mSARPortResetFlag   = false;
    mPResetFlag         = false;
    mSARCmdPResetFlag   = false;
    mSARCmdADResetFlag  = false;
    mIsCmdPResetFlagA   = false;
    mIsCmdADResetFlagA  = false;
    mSARPortResetFlagA  = false;
    mPResetFlagA        = false;
    mSARCmdPResetFlagA  = false;
    mSARCmdADResetFlagA = false;
    mSARADState         = emIDLE;
    mPreAVcc0       = 3.3;
    mPreAVcc1       = 3.3;
    mPreAVcc2       = 3.3;
    mPreAVcc3       = 3.3;
    mPreAvrefh0     = 3.3;
    mPreAvrefh1     = 3.3;
    mPreAvrefh2     = 3.3;
    mPreAvrefh3     = 3.3;
    mPretTSPL       = 27;
    mPretTHSD       = 5;
    mPretTHLD       = 10;
    mPreExSmpt      = 60;
    mPreSmpt        = 18;
    mPretSAR        = 0;
    mPretD          = 0;
    mPretED         = 0;
    mPreEnableTimeCalculation = true;
    mPreAVccMax = 3.6;
    mPreAVccMin = 3;
    mPreAvrefhMin   = 3;
    mPclkPeriod     = 0;
    mADclkPeriod    = 0;
    mPrePclkPeriod  = 0;
    mPreADclkPeriod = 0;
    mADCNum         = emADC_NUM_Def;

    TargetConfig TempTargetConfigTable[emTargetConfigNum] =  {
                                                              {"RH850_U2B", 5, 28, 28, 32, 40, 30,  64, 64, 64, 64, 64, 5, 5, 5, 5, 5, 6, 6, 6, 6, 0}
                                                             };
    // copy TempTargetConfig
    for (unsigned int i=0;i<emTargetConfigNum;i++) {
        mTargetConfigTable[i] = TempTargetConfigTable[i];
    }

    //Initialize output port
    INT_ERR0.initialize(0);
    INT_ERR1.initialize(0);
    INT_ERR2.initialize(0);
    INT_ERR3.initialize(0);
    INT_ERRA.initialize(0);
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
    INT_AD24.initialize(0);
    INT_AD30.initialize(0);
    INT_AD31.initialize(0);
    INT_AD32.initialize(0);
    INT_AD33.initialize(0);
    INT_AD34.initialize(0);
    INT_ADA0.initialize(0);
    INT_ADA1.initialize(0);
    INT_ADA2.initialize(0);
    INT_ADA3.initialize(0);
    INT_ADA4.initialize(0);
    INT_MPX0.initialize(0);
    INT_MPX1.initialize(0);
    INT_MPX2.initialize(0);
    INT_MPX3.initialize(0);
    INT_MPXA.initialize(0);
    INT_SGD0.initialize(0);
    INT_SGD1.initialize(0);
    INT_SGD2.initialize(0);
    INT_SGD3.initialize(0);
    INT_SGDA.initialize(0);
    ADPE0.initialize(0);
    ADPE1.initialize(0);
    ADPE2.initialize(0);
    ADPE3.initialize(0);
    ADPEA.initialize(0);
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
    ADEND30.initialize(0);
    ADEND31.initialize(0);
    ADEND32.initialize(0);
    ADEND33.initialize(0);
    ADEND34.initialize(0);
    ADENDA0.initialize(0);
    ADENDA1.initialize(0);
    ADENDA2.initialize(0);
    ADENDA3.initialize(0);
    ADENDA4.initialize(0);
    ADC0_IFDATA.initialize(0);
    ADC1_IFDATA.initialize(0);
    ADC2_IFDATA.initialize(0);
    ADC3_IFDATA.initialize(0);
    ADCA_IFDATA.initialize(0);
    DFREQ0.initialize(0);
    DFREQ1.initialize(0);
    DFREQ2.initialize(0);
    DFREQ3.initialize(0);
    DFREQA.initialize(0);
    DFTAG0.initialize(0);
    DFTAG1.initialize(0);
    DFTAG2.initialize(0);
    DFTAG3.initialize(0);
    DFTAGA.initialize(0);
    DFDATA0.initialize(0);
    DFDATA1.initialize(0);
    DFDATA2.initialize(0);
    DFDATA3.initialize(0);
    DFDATAA.initialize(0);
    VCULMO0.initialize(0);
    VCLLMO0.initialize(0);
    VCULMO1.initialize(0);
    VCLLMO1.initialize(0);
    VCULMO2.initialize(0);
    VCLLMO2.initialize(0);
    VCULMO3.initialize(0);
    VCLLMO3.initialize(0);
    VCULMOA.initialize(0);
    VCLLMOA.initialize(0);
//    MPXCUR0.initialize(0); // Initialized in ADC
//    MPXCUR1.initialize(0); // Initialized in ADC

    //Reset Method
    SC_METHOD(PResetMethod);
    dont_initialize();
    sensitive << preset_n;
    SC_METHOD(PResetMethodA);
    dont_initialize();
    sensitive << preseta_n;
    SC_METHOD(ADResetMethod);
    dont_initialize();
    sensitive << RESETAD_N;
    SC_METHOD(ADResetMethodA);
    dont_initialize();
    sensitive << RESETADA_N;
    SC_METHOD(AssertPResetMethod);
    dont_initialize();
    sensitive << mCmdPResetEvent;
    SC_METHOD(AssertPResetMethodA);
    dont_initialize();
    sensitive << mCmdPResetEventA;
    SC_METHOD(DeAssertPResetMethod);
    dont_initialize();
    sensitive << mCmdCancelPResetEvent;
    SC_METHOD(DeAssertPResetMethodA);
    dont_initialize();
    sensitive << mCmdCancelPResetEventA;
    SC_METHOD(AssertADResetMethod);
    dont_initialize();
    sensitive << mCmdADResetEvent;
    SC_METHOD(AssertADResetMethodA);
    dont_initialize();
    sensitive << mCmdADResetEventA;
    SC_METHOD(DeAssertADResetMethod);
    dont_initialize();
    sensitive << mCmdCancelADResetEvent;
    SC_METHOD(DeAssertADResetMethodA);
    dont_initialize();
    sensitive << mCmdCancelADResetEventA;
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
    SC_METHOD(AVREFH3Method);
    dont_initialize();
    sensitive << AVREFH3;
    SC_METHOD(InitialAVREFH0Method);
    SC_METHOD(InitialAVREFH1Method);
    SC_METHOD(InitialAVREFH2Method);
    SC_METHOD(InitialAVREFH3Method);

    //State Method
    SC_METHOD(SARADstateMethod);
    dont_initialize();
    sensitive << mADC0->mADCChangeStateEvent;
    sensitive << mADC1->mADCChangeStateEvent;
    sensitive << mADC2->mADCChangeStateEvent;
    sensitive << mADC3->mADCChangeStateEvent;
    sensitive << mADCA->mADCChangeStateEvent;

    for (unsigned int id = 0; id < emMaxId; id++) {
        SetLatency_TLM (id, 0, true);
    }

    SC_METHOD(SVModeMethod);
    dont_initialize();
    sensitive << mADC0->mSVModeEvent;
    sensitive << mADC1->mSVModeEvent;
    sensitive << mADC2->mSVModeEvent;
    sensitive << mADC3->mSVModeEvent;
    sensitive << mADCA->mSVModeEvent;

    SC_METHOD(UpdateChannelNumMethod);

    // Fix input ports 
    SC_METHOD(FixInputMethod);


}//}}}

//destructor of Csarad_u2b class
Csarad_u2b::~Csarad_u2b()
{//{{{
    delete mADC0;
    delete mADC1;
    delete mADC2;
    delete mADC3;
    delete mADCA;
}//}}}

//Connect port
void Csarad_u2b::ConnectPort(void)
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
    mADC0->ANm50(AN050);
    mADC0->ANm51(AN051);
    mADC0->ANm52(AN052);
    mADC0->ANm53(AN053);
    mADC0->ANm60(AN060);
    mADC0->ANm61(AN061);
    mADC0->ANm62(AN062);
    mADC0->ANm63(AN063);
    mADC0->ANm70(VMON_VCC);
    mADC0->ANm71(VMON_E0VCC);
    mADC0->ANm72(VMON_ISOVDD);
    mADC0->ANm73(VMON_AWOVDD);
    mADC0->ANm80(const_double);
    mADC0->ANm81(const_double);
    mADC0->ANm82(const_double);
    mADC0->ANm83(const_double);
    mADC0->ANm90(const_double);
    mADC0->ANm91(const_double);
    mADC0->ANm92(const_double);
    mADC0->ANm93(const_double);

    mADC0->ADmSG0TRG(AD0SG0TRG);
    mADC0->ADmSG1TRG(AD0SG1TRG);
    mADC0->ADmSG2TRG(AD0SG2TRG);
    mADC0->ADmSG3TRG(AD0SG3TRG);
    mADC0->ADmSG4TRG(AD0SG4TRG);
    mADC0->ADmSGDTRG(AD0SGDTRG);
    mADC0->ADmTAUD2I7(ADCx_sig_stubs);
    mADC0->ADmTAUD2I15(ADCx_sig_stubs);
    mADC0->ADmTAUJ2(ADCx_sig_stubs);
    mADC0->ADmTAUJ3(ADCx_sig_stubs);
    mADC0->ADmLPS(ADCx_sig_stubs);
    mADC0->SV_MODEm(SV_MODE);
    //output port
    mADC0->INT_ADm0(INT_AD00);
    mADC0->INT_ADm1(INT_AD01);
    mADC0->INT_ADm2(INT_AD02);
    mADC0->INT_ADm3(INT_AD03);
    mADC0->INT_ADm4(INT_AD04);
    mADC0->INT_MPXm(INT_MPX0);
    mADC0->INT_ERRm(INT_ERR0);
    mADC0->ADPEm(ADPE0);
    mADC0->INT_SGDm(INT_SGD0);

    mADC0->ADENDm0(ADEND00);
    mADC0->ADENDm1(ADEND01);
    mADC0->ADENDm2(ADEND02);
    mADC0->ADENDm3(ADEND03);
    mADC0->ADENDm4(ADEND04);

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
    mADC1->ANm70(const_double);
    mADC1->ANm71(const_double);
    mADC1->ANm72(const_double);
    mADC1->ANm73(const_double);
    mADC1->ANm80(const_double);
    mADC1->ANm81(const_double);
    mADC1->ANm82(const_double);
    mADC1->ANm83(const_double);
    mADC1->ANm90(const_double);
    mADC1->ANm91(const_double);
    mADC1->ANm92(const_double);
    mADC1->ANm93(const_double);

    mADC1->ADmSG0TRG(AD1SG0TRG);
    mADC1->ADmSG1TRG(AD1SG1TRG);
    mADC1->ADmSG2TRG(AD1SG2TRG);
    mADC1->ADmSG3TRG(AD1SG3TRG);
    mADC1->ADmSG4TRG(AD1SG4TRG);
    mADC1->ADmSGDTRG(AD1SGDTRG);
    mADC1->ADmTAUD2I7(ADCx_sig_stubs);
    mADC1->ADmTAUD2I15(ADCx_sig_stubs);
    mADC1->ADmTAUJ2(ADCx_sig_stubs);
    mADC1->ADmTAUJ3(ADCx_sig_stubs);
    mADC1->ADmLPS(ADCx_sig_stubs);
    mADC1->SV_MODEm(SV_MODE);
    //output port
    mADC1->INT_ADm0(INT_AD10);
    mADC1->INT_ADm1(INT_AD11);
    mADC1->INT_ADm2(INT_AD12);
    mADC1->INT_ADm3(INT_AD13);
    mADC1->INT_ADm4(INT_AD14);
    mADC1->INT_MPXm(INT_MPX1);
    mADC1->INT_ERRm(INT_ERR1);
    mADC1->ADPEm(ADPE1);
    mADC1->INT_SGDm(INT_SGD1);

    mADC1->ADENDm0(ADEND10);
    mADC1->ADENDm1(ADEND11);
    mADC1->ADENDm2(ADEND12);
    mADC1->ADENDm3(ADEND13);
    mADC1->ADENDm4(ADEND14);

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
    mADC2->ANm72(AN272);
    mADC2->ANm73(AN273);
    mADC2->ANm80(const_double);
    mADC2->ANm81(const_double);
    mADC2->ANm82(const_double);
    mADC2->ANm83(const_double);
    mADC2->ANm90(const_double);
    mADC2->ANm91(const_double);
    mADC2->ANm92(const_double);
    mADC2->ANm93(const_double);

    mADC2->ADmSG0TRG(AD2SG0TRG);
    mADC2->ADmSG1TRG(AD2SG1TRG);
    mADC2->ADmSG2TRG(AD2SG2TRG);
    mADC2->ADmSG3TRG(AD2SG3TRG);
    mADC2->ADmSG4TRG(AD2SG4TRG);
    mADC2->ADmSGDTRG(AD2SGDTRG);
    mADC2->ADmTAUD2I7(ADCx_sig_stubs);
    mADC2->ADmTAUD2I15(ADCx_sig_stubs);
    mADC2->ADmTAUJ2(ADCx_sig_stubs);
    mADC2->ADmTAUJ3(ADCx_sig_stubs);
    mADC2->ADmLPS(ADCx_sig_stubs);

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
    mADC2->INT_SGDm(INT_SGD2);

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
    mADC3->ANm80(AN380);
    mADC3->ANm81(AN381);
    mADC3->ANm82(AN382);
    mADC3->ANm83(AN383);
    mADC3->ANm90(AN390);
    mADC3->ANm91(AN391);
    mADC3->ANm92(AN392);
    mADC3->ANm93(AN393);

    mADC3->ADmSG0TRG(AD3SG0TRG);
    mADC3->ADmSG1TRG(AD3SG1TRG);
    mADC3->ADmSG2TRG(AD3SG2TRG);
    mADC3->ADmSG3TRG(AD3SG3TRG);
    mADC3->ADmSG4TRG(AD3SG4TRG);
    mADC3->ADmSGDTRG(AD3SGDTRG);
    mADC3->ADmTAUD2I7(ADCx_sig_stubs);
    mADC3->ADmTAUD2I15(ADCx_sig_stubs);
    mADC3->ADmTAUJ2(ADCx_sig_stubs);
    mADC3->ADmTAUJ3(ADCx_sig_stubs);
    mADC3->ADmLPS(ADCx_sig_stubs);

    mADC3->ADCm_IFDATA(ADC3_IFDATA);
    mADC3->IFVCHm(IFVCH3);
    mADC3->IFRDm(IFRD3);
    mADC3->IFREm(IFRE3);
    mADC3->SV_MODEm(SV_MODE);
    //output port
    mADC3->INT_ERRm(INT_ERR3);
    mADC3->INT_ADm0(INT_AD30);
    mADC3->INT_ADm1(INT_AD31);
    mADC3->INT_ADm2(INT_AD32);
    mADC3->INT_ADm3(INT_AD33);
    mADC3->INT_ADm4(INT_AD34);
    mADC3->INT_MPXm(INT_MPX3);
    mADC3->ADPEm(ADPE3);
    mADC3->INT_SGDm(INT_SGD3);

    mADC3->ADENDm0(ADEND30);
    mADC3->ADENDm1(ADEND31);
    mADC3->ADENDm2(ADEND32);
    mADC3->ADENDm3(ADEND33);
    mADC3->ADENDm4(ADEND34);

    mADC3->DFREQm(DFREQ3);
    mADC3->DFTAGm(DFTAG3);
    mADC3->DFDATAm(DFDATA3);

    for (unsigned int i = 0; i < emVCRnum_Max; i++) {
        mADC3->VCEND[i]->bind(*VCEND3[i]);
    }
    mADC3->VCULMO(VCULMO3);
    mADC3->VCLLMO(VCLLMO3);
    mADC3->PWM_VCEND(PWM_VCEND3);
    mADC3->PVCR_PWDDRm(PVCR_PWDDR3);
    mADC3->PVCR_ENDm(PVCR_END3);
    mADC3->PVCR_TRGm(PVCR_TRG3);
    mADC3->PVCR_VALUEm(PVCR_VALUE3);
    mADC3->MPXCURm(MPXCUR3);
    mADC3->ULE_LPSm(ULE_LPS3);

    // ADCA
    //input port
    mADCA->ANm00(ANA00);
    mADCA->ANm01(ANA01);
    mADCA->ANm02(ANA02);
    mADCA->ANm03(ANA03);
    mADCA->ANm10(ANA10);
    mADCA->ANm11(ANA11);
    mADCA->ANm12(ANA12);
    mADCA->ANm13(ANA13);
    mADCA->ANm20(ANA20);
    mADCA->ANm21(ANA21);
    mADCA->ANm22(ANA22);
    mADCA->ANm23(ANA23);
    mADCA->ANm30(ANA30);
    mADCA->ANm31(ANA31);
    mADCA->ANm32(ANA32);
    mADCA->ANm33(ANA33);
    mADCA->ANm40(ANA40);
    mADCA->ANm41(ANA41);
    mADCA->ANm42(ANA42);
    mADCA->ANm43(ANA43);
    mADCA->ANm50(ANA50);
    mADCA->ANm51(ANA51);
    mADCA->ANm52(ANA52);
    mADCA->ANm53(ANA53);
    mADCA->ANm60(ANA60);
    mADCA->ANm61(ANA61);
    mADCA->ANm62(ANA62);
    mADCA->ANm63(ANA63);
    mADCA->ANm70(ANA70);
    mADCA->ANm71(ANA71);
    mADCA->ANm72(const_double);
    mADCA->ANm73(const_double);
    mADCA->ANm80(const_double);
    mADCA->ANm81(const_double);
    mADCA->ANm82(const_double);
    mADCA->ANm83(const_double);
    mADCA->ANm90(const_double);
    mADCA->ANm91(const_double);
    mADCA->ANm92(const_double);
    mADCA->ANm93(const_double);

    mADCA->ADmSG0TRG(ADASG0TRG);
    mADCA->ADmSG1TRG(ADASG1TRG);
    mADCA->ADmSG2TRG(ADASG2TRG);
    mADCA->ADmSG3TRG(ADASG3TRG);
    mADCA->ADmSG4TRG(ADASG4TRG);
    mADCA->ADmSGDTRG(ADASGDTRG);
    mADCA->ADmTAUD2I7(ADxTAUD2I7);
    mADCA->ADmTAUD2I15(ADxTAUD2I15);
    mADCA->ADmTAUJ2(ADxTAUJ2);
    mADCA->ADmTAUJ3(ADxTAUJ3);
    mADCA->ADmLPS(ADxLPS);

    mADCA->ADCm_IFDATA(ADCA_IFDATA);
    mADCA->IFVCHm(IFVCHA);
    mADCA->IFRDm(IFRDA);
    mADCA->IFREm(IFREA);
    mADCA->SV_MODEm(SV_MODE);
    //output port
    mADCA->INT_ERRm(INT_ERRA);
    mADCA->INT_ADm0(INT_ADA0);
    mADCA->INT_ADm1(INT_ADA1);
    mADCA->INT_ADm2(INT_ADA2);
    mADCA->INT_ADm3(INT_ADA3);
    mADCA->INT_ADm4(INT_ADA4);
    mADCA->INT_MPXm(INT_MPXA);
    mADCA->ADPEm(ADPEA);
    mADCA->INT_SGDm(INT_SGDA);

    mADCA->ADENDm0(ADENDA0);
    mADCA->ADENDm1(ADENDA1);
    mADCA->ADENDm2(ADENDA2);
    mADCA->ADENDm3(ADENDA3);
    mADCA->ADENDm4(ADENDA4);

    mADCA->DFREQm(DFREQA);
    mADCA->DFTAGm(DFTAGA);
    mADCA->DFDATAm(DFDATAA);

    for (unsigned int i = 0; i < emVCRnum_Max; i++) {
        mADCA->VCEND[i]->bind(*VCENDA[i]);
    }
    mADCA->VCULMO(VCULMOA);
    mADCA->VCLLMO(VCLLMOA);
    mADCA->PWM_VCEND(PWM_VCENDA);
    mADCA->PVCR_PWDDRm(PVCR_PWDDRA);
    mADCA->PVCR_ENDm(PVCR_ENDA);
    mADCA->PVCR_TRGm(PVCR_TRGA);
    mADCA->PVCR_VALUEm(PVCR_VALUEA);
    mADCA->MPXCURm(MPXCURA);
    mADCA->ULE_LPSm(ULE_LPSA);
}//}}}

// SARAD METHOD

void Csarad_u2b::InitialAVREFH0Method(void)
{//{{{
    if(!mIsRefVol0Update){
        mIsRefVol0Update = true;
        Avrefh0_Val = (double)(AVREFH0.read());
        mADC0->mAvrefh = Avrefh0_Val;
    }
}//}}}

void Csarad_u2b::InitialAVREFH1Method(void)
{//{{{
    if(!mIsRefVol1Update){
        mIsRefVol1Update = true;
        Avrefh1_Val = (double)(AVREFH1.read());
        mADC1->mAvrefh = Avrefh1_Val;
    }
}//}}}

void Csarad_u2b::InitialAVREFH2Method(void)
{//{{{
    if(!mIsRefVol2Update){
        mIsRefVol2Update = true;
        Avrefh2_Val = (double)(AVREFH2.read());
        mADC2->mAvrefh = Avrefh2_Val;
    }
}//}}}


void Csarad_u2b::InitialAVREFH3Method(void)
{//{{{
    if(!mIsRefVol3Update){
        mIsRefVol3Update = true;
        Avrefh3_Val = (double)(AVREFH3.read());
        mADC3->mAvrefh = Avrefh3_Val;
        mADCA->mAvrefh = Avrefh3_Val;
    }
}//}}}

void Csarad_u2b::AVREFH0Method(void)
{//{{{
    if(!(mPResetFlag || mSARPortResetFlag || mSARCmdPResetFlag || mSARCmdADResetFlag)){
        double current_time = sc_time_stamp().to_double();
        if(current_time > 0){
            re_printf("warning", "Changing reference voltage Avrefh0 is invalid when simulation time is greater than zero.\n");
        }
    }
}//}}}

void Csarad_u2b::AVREFH1Method(void)
{//{{{
    if(!(mPResetFlag || mSARPortResetFlag || mSARCmdPResetFlag || mSARCmdADResetFlag)){
        double current_time = sc_time_stamp().to_double();
        if(current_time > 0){
            re_printf("warning", "Changing reference voltage Avrefh1 is invalid when simulation time is greater than zero.\n");
        }
    }
}//}}}

void Csarad_u2b::AVREFH2Method(void)
{//{{{
    if(!(mPResetFlag || mSARPortResetFlag || mSARCmdPResetFlag || mSARCmdADResetFlag)){
        double current_time = sc_time_stamp().to_double();
        if(current_time > 0){
            re_printf("warning", "Changing reference voltage Avrefh2 is invalid when simulation time is greater than zero.\n");
        }
    }
}//}}}

void Csarad_u2b::AVREFH3Method(void)
{//{{{
    if(!(mPResetFlag || mSARPortResetFlag || mSARCmdPResetFlag || mSARCmdADResetFlag || 
         mPResetFlagA || mSARPortResetFlagA || mSARCmdPResetFlagA || mSARCmdADResetFlagA)){
        double current_time = sc_time_stamp().to_double();
        if(current_time > 0){
            re_printf("warning", "Changing reference voltage Avrefh3 is invalid when simulation time is greater than zero.\n");
        }
    }
}//}}}

void Csarad_u2b::SARADstateMethod(void)
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

void Csarad_u2b::PCLKMethod(void)
{//{{{
    SetCLKfreq("pclk", sc_dt::uint64_to_double(pclk.read()));
}//}}}

void Csarad_u2b::ADCLKMethod(void)
{//{{{
    SetCLKfreq("ADCLK", sc_dt::uint64_to_double(ADCLK.read()));
}//}}}

void Csarad_u2b::CLKA_ADCMethod(void)
{//{{{
    SetCLKfreq("CLKA_ADC", sc_dt::uint64_to_double(CLKA_ADC.read()));
}//}}}

void Csarad_u2b::ADResetMethod(void)
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

void Csarad_u2b::ADResetMethodA(void)
{//{{{
    //1. Reset port sensitive
    bool is_cancel_assertreset = false;
    if(RESETADA_N.read() == vpcl::emResetActive){
        mSARPortResetFlagA = true;
    }else{
        mSARPortResetFlagA = false;
    }
    if(mSARPortResetFlagA){
        mIsADInitializeA = false;
        re_printf("info", "RESETADA_N signal is asserted.\n");
        this->EnableReset(mSARPortResetFlagA, emAD_RESETA);
        is_cancel_assertreset = true;
    }else{
        if(mIsADInitializeA){
            mIsADInitializeA = false;
            is_cancel_assertreset = false;
        }else{
            re_printf("info", "RESETADA_N signal is de-asserted.\n");
            this->EnableReset(mSARPortResetFlag, emAD_RESETA);
            is_cancel_assertreset = true;
        }
    }

    //Cancel AssertReset
    if(is_cancel_assertreset){
        mCmdADResetEventA.cancel();
        mCmdCancelADResetEventA.cancel();
        mIsCmdADResetFlagA = false;
        mSARCmdADResetFlagA = false;
    }

}//}}}

void Csarad_u2b::PResetMethod(void)
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

void Csarad_u2b::PResetMethodA(void)
{//{{{
    //1. Reset port sensitive
    bool is_cancel_assertreset = false;
    if(preseta_n.read() == vpcl::emResetActive){
        mPResetFlagA = true;
    }else{
        mPResetFlagA = false;
    }
    if(mPResetFlagA){
        mIsPInitializeA = false;
        re_printf("info", "preseta_n signal is asserted.\n");
        this->EnableReset(mPResetFlagA, emP_RESETA);
        is_cancel_assertreset = true;
    }else{
        if(mIsPInitializeA){
            mIsPInitializeA = false;
            is_cancel_assertreset = false;
        }else{
            re_printf("info", "preseta_n signal is de-asserted.\n");
            this->EnableReset(mPResetFlagA, emP_RESETA);
            is_cancel_assertreset = true;
        }
    }

    //Cancel AssertReset
    if(is_cancel_assertreset){
        mCmdPResetEventA.cancel();
        mCmdCancelPResetEventA.cancel();
        mIsCmdPResetFlagA = false;
        mSARCmdPResetFlagA = false;
    }
}//}}}

void Csarad_u2b::AssertPResetMethod(void)
{//{{{
    mSARCmdPResetFlag = true;
    this->EnableReset(mSARCmdPResetFlag, emP_RESET);
    mCmdCancelPResetEvent.notify((unsigned int)mPResetPeriod, SC_NS);
}//}}}

void Csarad_u2b::AssertPResetMethodA(void)
{//{{{
    mSARCmdPResetFlagA = true;
    this->EnableReset(mSARCmdPResetFlagA, emP_RESETA);
    mCmdCancelPResetEventA.notify((unsigned int)mPResetPeriodA, SC_NS);
}//}}}

void Csarad_u2b::DeAssertPResetMethod(void)
{//{{{
    mSARCmdPResetFlag = false;
    mIsCmdPResetFlag = false;
    this->EnableReset(mSARCmdPResetFlag, emP_RESET);
}//}}}

void Csarad_u2b::DeAssertPResetMethodA(void)
{//{{{
    mSARCmdPResetFlagA = false;
    mIsCmdPResetFlagA = false;
    this->EnableReset(mSARCmdPResetFlagA, emP_RESETA);
}//}}}

void Csarad_u2b::AssertADResetMethod(void)
{//{{{
    mSARCmdADResetFlag = true;
    this->EnableReset(mSARCmdADResetFlag, emAD_RESET);
    mCmdCancelADResetEvent.notify((unsigned int)mADResetPeriod, SC_NS);
}//}}}

void Csarad_u2b::AssertADResetMethodA(void)
{//{{{
    mSARCmdADResetFlagA = true;
    this->EnableReset(mSARCmdADResetFlagA, emAD_RESETA);
    mCmdCancelADResetEventA.notify((unsigned int)mADResetPeriodA, SC_NS);
}//}}}

void Csarad_u2b::DeAssertADResetMethod(void)
{//{{{
    mSARCmdADResetFlag = false;
    mIsCmdADResetFlag = false;
    this->EnableReset(mSARCmdADResetFlag, emAD_RESET);
}//}}}

void Csarad_u2b::DeAssertADResetMethodA(void)
{//{{{
    mSARCmdADResetFlagA = false;
    mIsCmdADResetFlagA = false;
    this->EnableReset(mSARCmdADResetFlagA, emAD_RESETA);
}//}}}

void Csarad_u2b::SVModeMethod(void)
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
        // RH850_U2B added
        if(mADC0->mEnterSVModeFlag) {
            re_printf("info", "ADC0 entered Supervisor Mode.\n");
        }
        if(mADC1->mEnterSVModeFlag) {
            re_printf("info", "ADC1 entered Supervisor Mode.\n");
        }
        if(mADC2->mEnterSVModeFlag) {
            re_printf("info", "ADC2 entered Supervisor Mode.\n");
        }
        if(mADC3->mEnterSVModeFlag) {
            re_printf("info", "ADC3 entered Supervisor Mode.\n");
        }
        if(mADC0->mExitSVModeFlag) {
            re_printf("info", "ADC0 exited from Supervisor Mode.\n");
        }
        if(mADC1->mExitSVModeFlag) {
            re_printf("info", "ADC1 exited from Supervisor Mode.\n");
        }
        if(mADC2->mExitSVModeFlag) {
            re_printf("info", "ADC2 exited from Supervisor Mode.\n");
        }
        if(mADC3->mExitSVModeFlag) {
            re_printf("info", "ADC3 exited from Supervisor Mode.\n");
        }
#endif
    }
    if(!(mPResetFlagA || mSARPortResetFlagA || mSARCmdPResetFlagA || mSARCmdADResetFlagA)){
        if(mADCA->mEnterSVModeFlag) {
            re_printf("info", "ADCA entered Supervisor Mode.\n");
        }
        if(mADCA->mExitSVModeFlag) {
            re_printf("info", "ADCA exited from Supervisor Mode.\n");
        }
    }
}//}}}
//Function
//tlm_tgt_if function

void Csarad_u2b::tgt_acc (unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t)
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

unsigned int Csarad_u2b::tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans)
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

bool Csarad_u2b::AddressArbitrate (const unsigned int id, tlm::tlm_command command, const bool is_dbg, const unsigned int addr, unsigned char *p_data, const unsigned int size)
{//{{{
    //1. Check data
    sc_assert(p_data != NULL);

    //2. Read & Write process
    unsigned int tmp_addr = addr & 0x1FFF;
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
        //2.2 Check address for ADC1
        if(command == tlm::TLM_WRITE_COMMAND){
            return mADC1->common_tgt_wr(is_dbg, addr, p_data, size);
        }else if(command == tlm::TLM_READ_COMMAND){
            return mADC1->common_tgt_rd(is_dbg, addr, p_data, size);
        }else{
            return true;
        }
    } else if (id == emADC2Id) {
        //2.3 Check address for ADC2
        if(command == tlm::TLM_WRITE_COMMAND){
            return mADC2->common_tgt_wr(is_dbg, addr, p_data, size);
        }else if(command == tlm::TLM_READ_COMMAND){
            return mADC2->common_tgt_rd(is_dbg, addr, p_data, size);
        }else{
            return true;
        }
    } else if (id == emADC3Id) {
        //2.4 Check address for ADC3
        if(command == tlm::TLM_WRITE_COMMAND){
            return mADC3->common_tgt_wr(is_dbg, addr, p_data, size);
        }else if(command == tlm::TLM_READ_COMMAND){
            return mADC3->common_tgt_rd(is_dbg, addr, p_data, size);
        }else{
            return true;
        }
    } else {
        //2.5 Check address for ADCA
        if(command == tlm::TLM_WRITE_COMMAND){
            return mADCA->common_tgt_wr(is_dbg, addr, p_data, size);
        }else if(command == tlm::TLM_READ_COMMAND){
            return mADCA->common_tgt_rd(is_dbg, addr, p_data, size);
        }else{
            return true;
        }

    }
}//}}}

//Call_back function
void Csarad_u2b::cb_ADSYNSTCR_ADSTART(RegCBstr str)
{//{{{
    if((*ADSYNSTCR)["ADSTART"] == 1){
        (*ADSYNSTCR) = 0;
        mADC0->WritingADSYNSTCR();
        mADC1->WritingADSYNSTCR();
        mADC2->WritingADSYNSTCR();
        mADC3->WritingADSYNSTCR();
    }
}//}}}

void Csarad_u2b::cb_ADTSYNSTCR_ADTSTART(RegCBstr str)
{//{{{
    if((*ADTSYNSTCR)["ADTSTART"] == 1){
        (*ADTSYNSTCR) = 0;
        mADC0->WritingADTSYNSTCR();
        mADC1->WritingADTSYNSTCR();
        mADC2->WritingADTSYNSTCR();
        mADC3->WritingADTSYNSTCR();
    }
}//}}}

void Csarad_u2b::cb_VMONVDCR1_VDE1(RegCBstr str)
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

void Csarad_u2b::cb_VMONVDCR2_VDE2(RegCBstr str)
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

bool Csarad_u2b::GetAllSGStopCheck(void) 
{//{{{
    return((mADC0->AllSGStopCheck() && mADC1->AllSGStopCheck() && mADC2->AllSGStopCheck() && mADC3->AllSGStopCheck() && mADCA->AllSGStopCheck())); 
}//}}}

//Reset function
void Csarad_u2b::EnableReset(const bool is_active, const unsigned int port_reset)
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
    // RH850_U2B added
    if (is_active) {
        if (port_reset == emAD_RESET) {
            re_printf("info", "The ADC0, ADC1, ADC2 and ADC3 are reset.\n");
            //3. Cancel all event
            mADC0->mADCChangeStateEvent.cancel();
            mADC1->mADCChangeStateEvent.cancel();
            mADC2->mADCChangeStateEvent.cancel();
            mADC3->mADCChangeStateEvent.cancel();
        }
        if (port_reset == emAD_RESETA) {
            re_printf("info", "The ADCA is reset.\n");
            mADCA->mADCChangeStateEvent.cancel();
        }
    }
#endif
    if(port_reset == emP_RESET){
        //4.Reset registers
        Csarad_u2b_regif::EnableReset(is_active);
    }

    if (port_reset == emP_RESET || port_reset == emAD_RESET) {
        //5.Reset ADC0, ADC1, ADC2 and ADC3 are reset
        mADC0->EnableReset(is_active, port_reset);
        mADC1->EnableReset(is_active, port_reset);
        mADC2->EnableReset(is_active, port_reset);
        mADC3->EnableReset(is_active, port_reset);
    }
    if (port_reset == emP_RESETA || port_reset == emAD_RESETA) {
        //6.Reset ADCA
        mADCA->EnableReset(is_active, port_reset);
    }
}//}}}

//handleCommand function
void Csarad_u2b::DumpStatInfo(void)
{//{{{
    mADC0->DumpStatInfo();
    mADC1->DumpStatInfo();
    mADC2->DumpStatInfo();
    mADC3->DumpStatInfo();
    mADCA->DumpStatInfo();
}//}}}

void Csarad_u2b::AssertPReset(const double start_time, const double period)
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

void Csarad_u2b::AssertPResetA(const double start_time, const double period)
{//{{{
    if((!mPResetFlagA)&&(!mIsCmdPResetFlagA)) {
        mIsCmdPResetFlagA = true;
        re_printf("info", "PReset for ADCA will reset for %lld.%06d ns after %lld.%06d ns.\n", (unsigned long long)period, FRACTION_TO_INT(period), (unsigned long long)start_time, FRACTION_TO_INT(start_time));
        mCmdPResetEventA.notify(start_time, SC_NS);
        mPResetPeriodA = (unsigned int)period;
    }else{
        re_printf("warning", "PReset for ADCA is in progress.\n");
    }
}//}}}

void Csarad_u2b::AssertADReset(const double start_time, const double period)
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

void Csarad_u2b::AssertADResetA(const double start_time, const double period)
{//{{{
    if((!mSARPortResetFlagA)&&(!mIsCmdADResetFlagA)) {
        mIsCmdADResetFlagA = true;
        re_printf("info", "ADReset for ADCA will reset for %lld.%06d ns after %lld.%06d ns.\n", (unsigned long long)period, FRACTION_TO_INT(period), (unsigned long long)start_time, FRACTION_TO_INT(start_time));
        mCmdADResetEventA.notify(start_time, SC_NS);
        mADResetPeriodA = (unsigned int)period;
    }else{
        re_printf("warning", "ADReset for ADCA is in progress.\n");
    }
}//}}}

void Csarad_u2b::AssignTHCircuit(const unsigned int adc_index, const unsigned int th_channel, const unsigned int group_channel, const unsigned int physic_channel)
{//{{{
    if (adc_index == 0) {
        mADC0->AssignTHCircuit(th_channel, group_channel, physic_channel);
    } else if (adc_index == 1) {
        mADC1->AssignTHCircuit(th_channel, group_channel, physic_channel);
    } else if (adc_index == 2) {
        mADC2->AssignTHCircuit(th_channel, group_channel, physic_channel);
    } else if (adc_index == 3) {
        mADC3->AssignTHCircuit(th_channel, group_channel, physic_channel);
    } else {
        re_printf("warning", "adc_index %d is invalid for AssignTHCircuit.\n", adc_index);
        // TBD adc_index = 4 and other
    }
}//}}}

double Csarad_u2b::GetTimeResolution(void)
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

void Csarad_u2b::SetCLKfreq(std::string clk_name, double clk_freq)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    double time_unit = 0;
    double pclk_period = 0;
    double adclk_period = 0;
    double clka_adc_period = 0;
    bool is_period_valid = true;

    // Setting clock values
    if (clk_name == "pclk") {
        if (mADC0->mADCChangeState || mADC1->mADCChangeState || mADC2->mADCChangeState || mADC3->mADCChangeState) {
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
            mADC0->mPCLKPeriod = pclk_period;
            mADC1->mPCLKPeriod = pclk_period;
            mADC2->mPCLKPeriod = pclk_period;
            mADC3->mPCLKPeriod = pclk_period;
            mADC0->mADCLKPeriod = mPreADclkPeriod;
            mADC1->mADCLKPeriod = mPreADclkPeriod;
            mADC2->mADCLKPeriod = mPreADclkPeriod;
            mADC3->mADCLKPeriod = mPreADclkPeriod;
            SetLatency_TLM(emADC0Id, pclk_period, false);
            SetLatency_TLM(emADC1Id, pclk_period, false);
            SetLatency_TLM(emADC2Id, pclk_period, false);
            SetLatency_TLM(emADC3Id, pclk_period, false);
            re_printf("info", "The pclk is set with a frequency as %lld.%06d\n", (unsigned long long)(clk_freq), FRACTION_TO_INT(clk_freq));
        }
        mPrePclkPeriod = pclk_period;
    } else if (clk_name == "ADCLK") {
        if (mADC0->mADCChangeState || mADC1->mADCChangeState || mADC2->mADCChangeState || mADC3->mADCChangeState) {
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
            mADC0->mADCLKPeriod = adclk_period;
            mADC1->mADCLKPeriod = adclk_period;
            mADC2->mADCLKPeriod = adclk_period;
            mADC3->mADCLKPeriod = adclk_period;
            mADC0->mPCLKPeriod = mPrePclkPeriod;
            mADC1->mPCLKPeriod = mPrePclkPeriod;
            mADC2->mPCLKPeriod = mPrePclkPeriod;
            mADC3->mPCLKPeriod = mPrePclkPeriod;
            SetLatency_TLM(emADC0Id, mPrePclkPeriod, false);
            SetLatency_TLM(emADC1Id, mPrePclkPeriod, false);
            SetLatency_TLM(emADC2Id, mPrePclkPeriod, false);
            SetLatency_TLM(emADC3Id, mPrePclkPeriod, false);
            re_printf("info", "The ADCLK is set with a frequency as %lld.%06d\n", (unsigned long long)(clk_freq), FRACTION_TO_INT(clk_freq));
        }
        mPreADclkPeriod = adclk_period;
    // RH850_U2B added
    } else if (clk_name == "CLKA_ADC") {
        if (mADCA->mADCChangeState) {
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
            mADCA->mPCLKPeriod = clka_adc_period;
            mADCA->mADCLKPeriod = clka_adc_period;
            SetLatency_TLM(emADCAId, clka_adc_period, false);
            re_printf("info", "The CLKA_ADC is set with a frequency as %lld.%06d\n", (unsigned long long)(clk_freq), FRACTION_TO_INT(clk_freq));
        }
    } else {
        re_printf("error", "Clock name is invalid.\n");
    }

    // Stop operation when clock frequency = 0
    if (((clk_name == "pclk") || (clk_name == "ADCLK")) && (clk_freq == 0)) {
        mADC0->SettingZeroClock();
        mADC1->SettingZeroClock();
        mADC2->SettingZeroClock();
        mADC3->SettingZeroClock();
    }
    // RH850_U2B added
    if ((clk_name == "CLKA_ADC") && (clk_freq == 0)) {
        mADCA->SettingZeroClock();
    }
}//}}}

void Csarad_u2b::SetLatency_TLM(const unsigned int id, const double pclk_period, const bool is_constructor)
{//{{{
    vpcl::tlm_if_tgt_parameter tgt_param = vpcl::tlm_tgt_if<32,tlm::tlm_base_protocol_types,5>::tgt_get_param(id);
    if(is_constructor){
        tgt_param.fw_req_phase = tlm::END_REQ;
    }
    sc_time new_clock(pclk_period, SC_NS);
    tgt_param.bus_clk = new_clock;
    tgt_param.rd_latency = tgt_param.rd_req_latency + 2 * tgt_param.bus_clk;
    tgt_param.wr_latency = tgt_param.wr_req_latency + 2 * tgt_param.bus_clk;
    vpcl::tlm_tgt_if<32,tlm::tlm_base_protocol_types,5>::tgt_set_param(id, tgt_param);
}//}}}

std::string Csarad_u2b::CommandCB(const std::vector<std::string>& args)
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
            mADC0->mDumpInterrupt = true;
            mADC1->mDumpInterrupt = true;
            mADC2->mDumpInterrupt = true;
            mADC3->mDumpInterrupt = true;
            mADCA->mDumpInterrupt = true;
        }else if (_args[1] == "false") {
            mADC0->mDumpInterrupt = false;
            mADC1->mDumpInterrupt = false;
            mADC2->mDumpInterrupt = false;
            mADC3->mDumpInterrupt = false;
            mADCA->mDumpInterrupt = false;
        }
    }else if((_args[0] == "EnableConvertInfo")&&((int)_args.size() == 2)){
        if (_args[1] == "true"){
            mADC0->mEnableConvertInfo = true;
            mADC1->mEnableConvertInfo = true;
            mADC2->mEnableConvertInfo = true;
            mADC3->mEnableConvertInfo = true;
            mADCA->mEnableConvertInfo = true;
        }else if (_args[1] == "false"){
            mADC0->mEnableConvertInfo = false;
            mADC1->mEnableConvertInfo = false;
            mADC2->mEnableConvertInfo = false;
            mADC3->mEnableConvertInfo = false;
            mADCA->mEnableConvertInfo = false;
        }
    }else if((_args[0] == "Avrefh0_Val")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage Avrefh0_Val is invalid when simulation time is greater than zero.\n");
            Avrefh0_Val = mPreAvrefh0;
        }else{
            mPreAvrefh0 = Avrefh0_Val;
            mADC0->mAvrefh = Avrefh0_Val;
            mIsRefVol0Update = true;
        }
    }else if((_args[0] == "Avrefh1_Val")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage Avrefh1_Val is invalid when simulation time is greater than zero.\n");
            Avrefh1_Val = mPreAvrefh1;
        }else{
            mPreAvrefh1 = Avrefh1_Val;
            mADC1->mAvrefh = Avrefh1_Val;
            mIsRefVol1Update = true;
        }
    }else if((_args[0] == "Avrefh2_Val")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage Avrefh2_Val is invalid when simulation time is greater than zero.\n");
            Avrefh2_Val = mPreAvrefh2;
        }else{
            mPreAvrefh2 = Avrefh2_Val;
            mADC2->mAvrefh = Avrefh2_Val;
            mIsRefVol2Update = true;
        }
    }else if((_args[0] == "Avrefh3_Val")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage Avrefh3_Val is invalid when simulation time is greater than zero.\n");
            Avrefh3_Val = mPreAvrefh3;
        }else{
            mPreAvrefh3 = Avrefh3_Val;
            mADC3->mAvrefh = Avrefh3_Val;
            mADCA->mAvrefh = Avrefh3_Val;
            mIsRefVol3Update = true;
        }
    }else if((_args[0] == "AVcc0")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage AVcc0 is invalid when simulation time is greater than zero.\n");
            AVcc0 = mPreAVcc0;
        }else{
            mPreAVcc0 = AVcc0;
            mADC0->mAVcc = AVcc0;
        }
    }else if((_args[0] == "AVcc1")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage AVcc1 is invalid when simulation time is greater than zero.\n");
            AVcc1 = mPreAVcc1;
        }else{
            mPreAVcc1 = AVcc1;
            mADC1->mAVcc = AVcc1;
        }
    }else if((_args[0] == "AVcc2")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage AVcc2 is invalid when simulation time is greater than zero.\n");
            AVcc2 = mPreAVcc2;
        }else{
            mPreAVcc2 = AVcc2;
            mADC2->mAVcc = AVcc2;
        }
    }else if((_args[0] == "AVcc3")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage AVcc3 is invalid when simulation time is greater than zero.\n");
            AVcc3 = mPreAVcc3;
        }else{
            mPreAVcc3 = AVcc3;
            mADC3->mAVcc = AVcc3;
            mADCA->mAVcc = AVcc3;
        }
    }else if((_args[0] == "tTSPL")&&((int)_args.size() == 2)){
        if (mADC0->mADCChangeState || mADC1->mADCChangeState || mADC2->mADCChangeState || mADC3->mADCChangeState || mADCA->mADCChangeState) {
            re_printf("warning", "Changing the tTSPL parameter is not affected when SARAD model is operating.\n");
            tTSPL = mPretTSPL;
        }else{
            mPretTSPL = tTSPL;
            mADC0->mtTSPL = tTSPL;
            mADC1->mtTSPL = tTSPL;
            mADC2->mtTSPL = tTSPL;
            mADC3->mtTSPL = tTSPL;
            mADCA->mtTSPL = tTSPL;
        }
    }else if((_args[0] == "tTHLD")&&((int)_args.size() == 2)){
        if (mADC0->mADCChangeState || mADC1->mADCChangeState || mADC2->mADCChangeState || mADC3->mADCChangeState || mADCA->mADCChangeState) {
            re_printf("warning", "Changing the tTHLD parameter is not affected when SARAD model is operating.\n");
            tTHLD = mPretTHLD;
        }else{
            mPretTHLD = tTHLD;
            mADC0->mtTHLD = tTHLD;
            mADC1->mtTHLD = tTHLD;
            mADC2->mtTHLD = tTHLD;
            mADC3->mtTHLD = tTHLD;
            mADCA->mtTHLD = tTHLD;
        }
    }else if((_args[0] == "tTHSD")&&((int)_args.size() == 2)){
        if (mADC0->mADCChangeState || mADC1->mADCChangeState || mADC2->mADCChangeState || mADC3->mADCChangeState || mADCA->mADCChangeState) {
            re_printf("warning", "Changing the tTHSD parameter is not affected when SARAD model is operating.\n");
            tTHSD = mPretTHSD;
        }else{
            mPretTHSD = tTHSD;
            mADC0->mtTHSD = tTHSD;
            mADC1->mtTHSD = tTHSD;
            mADC2->mtTHSD = tTHSD;
            mADC3->mtTHSD = tTHSD;
            mADCA->mtTHSD = tTHSD;
        }
    }else if((_args[0] == "ExSmpt")&&((int)_args.size() == 2)){
        if (mADC0->mADCChangeState || mADC1->mADCChangeState || mADC2->mADCChangeState || mADC3->mADCChangeState || mADCA->mADCChangeState) {
            re_printf("warning", "Changing the ExSmpt parameter is not affected when SARAD model is operating.\n");
            ExSmpt = mPreExSmpt;
        }else{
            mPreExSmpt = ExSmpt;
            mADC0->mExSmpt = ExSmpt;
            mADC1->mExSmpt = ExSmpt;
            mADC2->mExSmpt = ExSmpt;
            mADC3->mExSmpt = ExSmpt;
            mADCA->mExSmpt = ExSmpt;
        }
    }else if((_args[0] == "Smpt")&&((int)_args.size() == 2)){
        if (mADC0->mADCChangeState || mADC1->mADCChangeState || mADC2->mADCChangeState || mADC3->mADCChangeState || mADCA->mADCChangeState) {
            re_printf("warning", "Changing the Smpt parameter is not affected when SARAD model is operating.\n");
            Smpt = mPreSmpt;
        }else{
            mPreSmpt = Smpt;
            mADC0->mSmpt = Smpt;
            mADC1->mSmpt = Smpt;
            mADC2->mSmpt = Smpt;
            mADC3->mSmpt = Smpt;
            mADCA->mSmpt = Smpt;
        }
    }else if((_args[0] == "tSAR")&&((int)_args.size() == 2)){
        if (mADC0->mADCChangeState || mADC1->mADCChangeState || mADC2->mADCChangeState || mADC3->mADCChangeState || mADCA->mADCChangeState) {
            re_printf("warning", "Changing the tSAR parameter is not affected when SARAD model is operating.\n");
            tSAR = mPretSAR;
        }else{
            mPretSAR = tSAR;
            mADC0->mtSAR = tSAR;
            mADC1->mtSAR = tSAR;
            mADC2->mtSAR = tSAR;
            mADC3->mtSAR = tSAR;
            mADCA->mtSAR = tSAR;
        }
    }else if((_args[0] == "tD")&&((int)_args.size() == 2)){
        if (mADC0->mADCChangeState || mADC1->mADCChangeState || mADC2->mADCChangeState || mADC3->mADCChangeState || mADCA->mADCChangeState) {
            re_printf("warning", "Changing the tD parameter is not affected when SARAD model is operating.\n");
            tD = mPretD;
        }else{
            mPretD = tD;
            mADC0->mtD = tD;
            mADC1->mtD = tD;
            mADC2->mtD = tD;
            mADC3->mtD = tD;
            mADCA->mtD = tD;
        }
    }else if((_args[0] == "tED")&&((int)_args.size() == 2)){
        if (mADC0->mADCChangeState || mADC1->mADCChangeState || mADC2->mADCChangeState || mADC3->mADCChangeState || mADCA->mADCChangeState) {
            re_printf("warning", "Changing the tED parameter is not affected when SARAD model is operating.\n");
            tED = mPretED;
        }else{
            mPretED = tED;
            mADC0->mtED = tED;
            mADC1->mtED = tED;
            mADC2->mtED = tED;
            mADC3->mtED = tED;
            mADCA->mtED = tED;
        }
    }else if((_args[0] == "ForcedIdError0")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADC0->mForcedIdError = true;
        }else if (_args[1] == "false") {
            mADC0->mForcedIdError = false;
        }
    }else if((_args[0] == "ForcedIdError1")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADC1->mForcedIdError = true;
        }else if (_args[1] == "false") {
            mADC1->mForcedIdError = false;
        }
    }else if((_args[0] == "ForcedIdError2")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADC2->mForcedIdError = true;
        }else if (_args[1] == "false") {
            mADC2->mForcedIdError = false;
        }
    }else if((_args[0] == "ForcedIdError3")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADC3->mForcedIdError = true;
        }else if (_args[1] == "false") {
            mADC3->mForcedIdError = false;
        }
    }else if((_args[0] == "ForcedIdErrorA")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADCA->mForcedIdError = true;
        }else if (_args[1] == "false") {
            mADCA->mForcedIdError = false;
        }
    }else if((_args[0] == "ForcedParityError0")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADC0->mForcedParityError = true;
        }else if (_args[1] == "false") {
            mADC0->mForcedParityError = false;
        }
    }else if((_args[0] == "ForcedParityError1")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADC1->mForcedParityError = true;
        }else if (_args[1] == "false") {
            mADC1->mForcedParityError = false;
        }
    }else if((_args[0] == "ForcedParityError2")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADC2->mForcedParityError = true;
        }else if (_args[1] == "false") {
            mADC2->mForcedParityError = false;
        }
    }else if((_args[0] == "ForcedParityError3")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADC3->mForcedParityError = true;
        }else if (_args[1] == "false") {
            mADC3->mForcedParityError = false;
        }
    }else if((_args[0] == "ForcedParityErrorA")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADCA->mForcedParityError = true;
        }else if (_args[1] == "false") {
            mADCA->mForcedParityError = false;
        }
    }else if((_args[0] == "ForcedOpenError0")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADC0->mForcedOpenError = true;
        }else if (_args[1] == "false") {
            mADC0->mForcedOpenError = false;
        }
    }else if((_args[0] == "ForcedOpenError1")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADC1->mForcedOpenError = true;
        }else if (_args[1] == "false") {
            mADC1->mForcedOpenError = false;
        }
    }else if((_args[0] == "ForcedOpenError2")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADC2->mForcedOpenError = true;
        }else if (_args[1] == "false") {
            mADC2->mForcedOpenError = false;
        }
    }else if((_args[0] == "ForcedOpenError3")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADC3->mForcedOpenError = true;
        }else if (_args[1] == "false") {
            mADC3->mForcedOpenError = false;
        }
    }else if((_args[0] == "ForcedOpenErrorA")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADCA->mForcedOpenError = true;
        }else if (_args[1] == "false") {
            mADCA->mForcedOpenError = false;
        }
    }else if((_args[0] == "ForcedBreak0")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADC0->mForcedBreak = true;
        }else if (_args[1] == "false") {
            mADC0->mForcedBreak = false;
        }
    }else if((_args[0] == "ForcedBreak1")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADC1->mForcedBreak = true;
        }else if (_args[1] == "false") {
            mADC1->mForcedBreak = false;
        }
    }else if((_args[0] == "ForcedBreak2")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADC2->mForcedBreak = true;
        }else if (_args[1] == "false") {
            mADC2->mForcedBreak = false;
        }
    }else if((_args[0] == "ForcedBreak3")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADC3->mForcedBreak = true;
        }else if (_args[1] == "false") {
            mADC3->mForcedBreak = false;
        }
    }else if((_args[0] == "ForcedBreakA")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADCA->mForcedBreak = true;
        }else if (_args[1] == "false") {
            mADCA->mForcedBreak = false;
        }
    }else if((_args[0] == "ForcedDataPathError0")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADC0->mForcedDataPathError = true;
        }else if (_args[1] == "false") {
            mADC0->mForcedDataPathError = false;
        }
    }else if((_args[0] == "ForcedDataPathError1")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADC1->mForcedDataPathError = true;
        }else if (_args[1] == "false") {
            mADC1->mForcedDataPathError = false;
        }
    }else if((_args[0] == "ForcedDataPathError2")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADC2->mForcedDataPathError = true;
        }else if (_args[1] == "false") {
            mADC2->mForcedDataPathError = false;
        }
    }else if((_args[0] == "ForcedDataPathError3")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADC3->mForcedDataPathError = true;
        }else if (_args[1] == "false") {
            mADC3->mForcedDataPathError = false;
        }
    }else if((_args[0] == "ForcedDataPathErrorA")&&((int)_args.size() == 2)){
        if (_args[1] == "true") {
            mADCA->mForcedDataPathError = true;
        }else if (_args[1] == "false") {
            mADCA->mForcedDataPathError = false;
        }
    }else if((_args[0] == "EnableTimeCalculation")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing delay times calculation is invalid when simulation time is greater than zero.\n");
            EnableTimeCalculation = mPreEnableTimeCalculation;
        }else{
            mPreEnableTimeCalculation = EnableTimeCalculation;
            mADC0->mEnableTimeCalculation = EnableTimeCalculation;
            mADC1->mEnableTimeCalculation = EnableTimeCalculation;
            mADC2->mEnableTimeCalculation = EnableTimeCalculation;
            mADC3->mEnableTimeCalculation = EnableTimeCalculation;
            mADCA->mEnableTimeCalculation = EnableTimeCalculation;
        }
    /// SARAD3
    }else if((_args[0] == "AVccMax")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage AVccMax is invalid when simulation time is greater than zero.\n");
            AVccMax = mPreAVccMax;
        }else{
            mPreAVccMax = AVccMax;
            mADC0->mAVccMax = AVccMax;
            mADC1->mAVccMax = AVccMax;
            mADC2->mAVccMax = AVccMax;
            mADC3->mAVccMax = AVccMax;
            mADCA->mAVccMax = AVccMax;
        }
    }else if((_args[0] == "AVccMin")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage AVccMin is invalid when simulation time is greater than zero.\n");
            AVccMin = mPreAVccMin;
        }else{
            mPreAVccMin = AVccMin;
            mADC0->mAVccMin = AVccMin;
            mADC1->mAVccMin = AVccMin;
            mADC2->mAVccMin = AVccMin;
            mADC3->mAVccMin = AVccMin;
            mADCA->mAVccMin = AVccMin;
        }
    }else if((_args[0] == "AvrefhMin")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning", "Changing reference voltage AvrefhMin is invalid when simulation time is greater than zero.\n");
            AvrefhMin = mPreAvrefhMin;
        }else{
            mPreAvrefhMin = AvrefhMin;
            mADC0->mAvrefhMin = AvrefhMin;
            mADC1->mAvrefhMin = AvrefhMin;
            mADC2->mAvrefhMin = AvrefhMin;
            mADC3->mAvrefhMin = AvrefhMin;
            mADCA->mAvrefhMin = AvrefhMin;
        }
    }
    return ret;
}//}}}


// Check & set attribute values to ADC modules
void Csarad_u2b::UpdateChannelNumMethod(void)
{//{{{
    unsigned int found_index = 0;
    unsigned int ADC0_phy_num = emP_SADC_SARCHNUM_Def;
    unsigned int ADC1_phy_num = emP_SADC_SARCHNUM_Def;
    unsigned int ADC2_phy_num = emP_SADC_SARCHNUM_Def;
    unsigned int ADC3_phy_num = emP_SADC_SARCHNUM_Def;
    unsigned int ADCA_phy_num = emP_SADC_SARCHNUM_Def;
    unsigned int ADC0_vir_num = emVCRnum_Def;
    unsigned int ADC1_vir_num = emVCRnum_Def;
    unsigned int ADC2_vir_num = emVCRnum_Def;
    unsigned int ADC3_vir_num = emVCRnum_Def;
    unsigned int ADCA_vir_num = emVCRnum_Def;
    unsigned int ADC0_MPX_ch = emEX_MPX_PCH_Def;
    unsigned int ADC1_MPX_ch = emEX_MPX_PCH_Def;
    unsigned int ADC2_MPX_ch = emEX_MPX_PCH_Def;
    unsigned int ADC3_MPX_ch = emEX_MPX_PCH_Def;
    unsigned int ADCA_MPX_ch = emEX_MPX_PCH_Def;
    unsigned int ADC0_TH_ch = emTH_CHNUM_Def;
    unsigned int ADC1_TH_ch = emTH_CHNUM_Def;
    unsigned int ADC2_TH_ch = emTH_CHNUM_Def;
    unsigned int ADC3_TH_ch = emTH_CHNUM_Def;
    unsigned int ADCA_TH_ch = emTH_CHNUM_Def;
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
        ADC3_phy_num = mTargetConfigTable[found_index].physical_ch_num3;
        ADCA_phy_num = mTargetConfigTable[found_index].physical_ch_numa;
        ADC0_vir_num = mTargetConfigTable[found_index].virtual_ch_num0;
        ADC1_vir_num = mTargetConfigTable[found_index].virtual_ch_num1;
        ADC2_vir_num = mTargetConfigTable[found_index].virtual_ch_num2;
        ADC3_vir_num = mTargetConfigTable[found_index].virtual_ch_num3;
        ADCA_vir_num = mTargetConfigTable[found_index].virtual_ch_numa;
        ADC0_MPX_ch = mTargetConfigTable[found_index].external_mpx_pch0;
        ADC1_MPX_ch = mTargetConfigTable[found_index].external_mpx_pch1;
        ADC2_MPX_ch = mTargetConfigTable[found_index].external_mpx_pch2;
        ADC3_MPX_ch = mTargetConfigTable[found_index].external_mpx_pch3;
        ADCA_MPX_ch = mTargetConfigTable[found_index].external_mpx_pcha;
        ADC0_TH_ch = mTargetConfigTable[found_index].th_channel_num0;
        ADC1_TH_ch = mTargetConfigTable[found_index].th_channel_num1;
        ADC2_TH_ch = mTargetConfigTable[found_index].th_channel_num2;
        ADC3_TH_ch = mTargetConfigTable[found_index].th_channel_num3;
        ADCA_TH_ch = mTargetConfigTable[found_index].th_channel_numa;
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
    if (P_SADC_SARCHNUM3.value != "") {
        ADC3_phy_num = (unsigned int) strtoul(P_SADC_SARCHNUM3.value.c_str(), NULL, 0);
    }
    if (P_SADC_SARCHNUMA.value != "") {
        ADCA_phy_num = (unsigned int) strtoul(P_SADC_SARCHNUMA.value.c_str(), NULL, 0);
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
    if (VCRnum3.value != "") {
        ADC3_vir_num = (unsigned int) strtoul(VCRnum3.value.c_str(), NULL, 0);
    }
    if (VCRnumA.value != "") {
        ADCA_vir_num = (unsigned int) strtoul(VCRnumA.value.c_str(), NULL, 0);
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
    if (EX_MPX_PCH3.value != "") {
        ADC3_MPX_ch = (unsigned int) strtoul(EX_MPX_PCH3.value.c_str(), NULL, 0);
    }
    if (EX_MPX_PCHA.value != "") {
        ADCA_MPX_ch = (unsigned int) strtoul(EX_MPX_PCHA.value.c_str(), NULL, 0);
    }

    // Stop simulation if attributes is set with wrong value
    /// SARAD4 modify - fix sc_assert() issue on SCHEAP
    re_sc_assert(emADC_NUM_Min <= mADCNum, "The number of ADC module is less than 1");
    re_sc_assert(emADC_NUM_Max >= mADCNum, "The number of ADC module is greater than 5");
    re_sc_assert(emP_SADC_SARCHNUM_Min <= ADC0_phy_num , "The number of physical channels of ADC0 is less than 1");
    re_sc_assert(ADC0_phy_num <= emP_SADC_SARCHNUM_Max , "The number of physical channels of ADC0 is greater than 40");
    re_sc_assert(emVCRnum_Min <= ADC0_vir_num , "The number of virtual channels of ADC0 is less than 16");
    re_sc_assert(ADC0_vir_num <= emVCRnum_Max , "The number of virtual channels of ADC0 is greater than 96");
    re_sc_assert((ADC0_vir_num & 0x3) == 0 , "The number of virtual channels of ADC0 is not a multiple of 4");
    re_sc_assert(ADC0_MPX_ch < ADC0_phy_num , "The analog channel for MPX of ADC0 is greater than the maximum index of physical channel");
    ///// re_sc_assert(ADC0_TH_ch <= emTH_CHNUM_Max , "The TH channel of ADC0 is greater than 6");

    // E2x added
    mADC0->mADCNum = mADCNum;

    // Update value to ADC0
    mADC0->mTargetProduct = TARGET_PRODUCT.value;
    mADC0->mPhyChannelNum = ADC0_phy_num;
    mADC0->mVirChannelNum = ADC0_vir_num;
    mADC0->mMPXInput = ADC0_MPX_ch;
    mADC0->mTHChannelNum = ADC0_TH_ch;
    re_printf("info", "ADC0 is configured with P_SADC_SARCHNUM0 = %d, VCRnum0 = %d, EX_MPX_PCH0 = %d, TH channel = %d.\n", ADC0_phy_num, ADC0_vir_num, ADC0_MPX_ch, ADC0_TH_ch);

    if (mADCNum > 1) { // if there is ADC1
        re_sc_assert(emP_SADC_SARCHNUM_Min <= ADC1_phy_num , "The number of physical channels of ADC1 is less than 1");
        re_sc_assert(ADC1_phy_num <= emP_SADC_SARCHNUM_Max , "The number of physical channels of ADC1 is greater than 40");
        re_sc_assert(emVCRnum_Min <= ADC1_vir_num , "The number of virtual channels of ADC1 is less than 16");
        re_sc_assert(ADC1_vir_num <= emVCRnum_Max , "The number of virtual channels of ADC1 is greater than 96");
        re_sc_assert((ADC1_vir_num & 0x3) == 0 , "The number of virtual channels of ADC1 is not a multiple of 4");
        re_sc_assert(ADC1_MPX_ch < ADC1_phy_num , "The analog channel for MPX of ADC1 is greater than the maximum index of physical channel");
        // Update value to ADC1
        mADC1->mTargetProduct = TARGET_PRODUCT.value;
        mADC1->mPhyChannelNum = ADC1_phy_num;
        mADC1->mVirChannelNum = ADC1_vir_num;
        mADC1->mMPXInput = ADC1_MPX_ch;
        mADC1->mTHChannelNum = ADC1_TH_ch;
        re_printf("info", "ADC1 is configured with P_SADC_SARCHNUM1 = %d, VCRnum1 = %d, EX_MPX_PCH1 = %d, TH channel = %d.\n", ADC1_phy_num, ADC1_vir_num, ADC1_MPX_ch, ADC1_TH_ch);

        mADC1->mADCNum = mADCNum;
    }

    if (mADCNum > 2) { // if there is ADC2
        re_sc_assert(emP_SADC_SARCHNUM_Min <= ADC2_phy_num , "The number of physical channels of ADC2 is less than 1");
        re_sc_assert(ADC2_phy_num <= emP_SADC_SARCHNUM_Max , "The number of physical channels of ADC2 is greater than 40");
        re_sc_assert(emVCRnum_Min <= ADC2_vir_num , "The number of virtual channels of ADC2 is less than 16");
        re_sc_assert(ADC2_vir_num <= emVCRnum_Max , "The number of virtual channels of ADC2 is greater than 96");
        re_sc_assert((ADC2_vir_num & 0x3) == 0 , "The number of virtual channels of ADC2 is not a multiple of 4");
        re_sc_assert(ADC2_MPX_ch < ADC2_phy_num , "The analog channel for MPX of ADC2 is greater than the maximum index of physical channel");
        // Update value to ADC2
        mADC2->mTargetProduct = TARGET_PRODUCT.value;
        mADC2->mPhyChannelNum = ADC2_phy_num;
        mADC2->mVirChannelNum = ADC2_vir_num;
        mADC2->mMPXInput = ADC2_MPX_ch;
        mADC2->mTHChannelNum = ADC2_TH_ch;
        re_printf("info", "ADC2 is configured with P_SADC_SARCHNUM2 = %d, VCRnum2 = %d, EX_MPX_PCH2 = %d, TH channel = %d.\n", ADC2_phy_num, ADC2_vir_num, ADC2_MPX_ch, ADC2_TH_ch);

        mADC2->mADCNum = mADCNum;
    }

    if (mADCNum > 3) { // if there is ADC3
        re_sc_assert(emP_SADC_SARCHNUM_Min <= ADC3_phy_num , "The number of physical channels of ADC3 is less than 1");
        re_sc_assert(ADC3_phy_num <= emP_SADC_SARCHNUM_Max , "The number of physical channels of ADC3 is greater than 40");
        re_sc_assert(emVCRnum_Min <= ADC3_vir_num , "The number of virtual channels of ADC3 is less than 16");
        re_sc_assert(ADC3_vir_num <= emVCRnum_Max , "The number of virtual channels of ADC3 is greater than 96");
        re_sc_assert((ADC3_vir_num & 0x3) == 0 , "The number of virtual channels of ADC3 is not a multiple of 4");
        re_sc_assert(ADC3_MPX_ch < ADC3_phy_num , "The analog channel for MPX of ADC3 is greater than the maximum index of physical channel");
        // Update value to ADC3
        mADC3->mTargetProduct = TARGET_PRODUCT.value;
        mADC3->mPhyChannelNum = ADC3_phy_num;
        mADC3->mVirChannelNum = ADC3_vir_num;
        mADC3->mMPXInput = ADC3_MPX_ch;
        mADC3->mTHChannelNum = ADC3_TH_ch;
        re_printf("info", "ADC3 is configured with P_SADC_SARCHNUM3 = %d, VCRnum3 = %d, EX_MPX_PCH3 = %d, TH channel = %d.\n", ADC3_phy_num, ADC3_vir_num, ADC3_MPX_ch, ADC3_TH_ch);

        mADC3->mADCNum = mADCNum;
    }

    if (mADCNum > 4) { // if there is ADCA
        re_sc_assert(emP_SADC_SARCHNUM_Min <= ADCA_phy_num , "The number of physical channels of ADCA is less than 1");
        re_sc_assert(ADCA_phy_num <= emP_SADC_SARCHNUM_Max , "The number of physical channels of ADCA is greater than 40");
        re_sc_assert(emVCRnum_Min <= ADCA_vir_num , "The number of virtual channels of ADCA is less than 16");
        re_sc_assert(ADCA_vir_num <= emVCRnum_Max , "The number of virtual channels of ADCA is greater than 96");
        re_sc_assert((ADCA_vir_num & 0x3) == 0 , "The number of virtual channels of ADCA is not a multiple of 4");
        re_sc_assert(ADCA_MPX_ch < ADCA_phy_num , "The analog channel for MPX of ADCA is greater than the maximum index of physical channel");
        // Update value to ADCA
        mADCA->mTargetProduct = TARGET_PRODUCT.value;
        mADCA->mPhyChannelNum = ADCA_phy_num;
        mADCA->mVirChannelNum = ADCA_vir_num;
        mADCA->mMPXInput = ADCA_MPX_ch;
        mADCA->mTHChannelNum = ADCA_TH_ch;
        re_printf("info", "ADCA is configured with P_SADC_SARCHNUMA = %d, VCRnumA = %d, EX_MPX_PCHA = %d, TH channel = %d.\n", ADCA_phy_num, ADCA_vir_num, ADCA_MPX_ch, ADCA_TH_ch);

        mADCA->mADCNum = mADCNum;
    }
}//}}}

void Csarad_u2b::FixInputMethod(void)
{//{{{
    const_double.write(0);
}//}}}

// vim600: set foldmethod=marker :
