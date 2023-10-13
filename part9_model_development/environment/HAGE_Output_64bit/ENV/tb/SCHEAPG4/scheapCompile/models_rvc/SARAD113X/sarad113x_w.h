// ---------------------------------------------------------------------
// $Id: sarad113x_w.h,v 1.4 2014/03/31 08:26:21 honglam Exp $
//
// Copyright(c) 2013 Renesas Electronics Corporation
// Copyright(c) 2013 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __SARAD113x_W_H__
#define __SARAD113x_W_H__
#include "systemc.h"
#include "tlm.h"
#include "sarad113x.h"

class Csarad113x_w:  public sc_module
{
public:
    sc_out < sc_uint<32> > PWDATA;
    // Declare SARAD113x I/O interface
    // Input ports
    sc_in <sc_dt::uint64> pclk;
    sc_in <sc_dt::uint64> ADCLK;
    sc_in <bool> preset_n;
    sc_in <double> AVREFH0;

    sc_in <bool> TSN_TRG;
    sc_in <bool> SG1_TRG;
    sc_in <bool> SG2_TRG;
    sc_in <bool> SG3_TRG;
    sc_in <bool> PVCR_TRG;

    sc_in <double> ANI00;
    sc_in <double> ANI01;
    sc_in <double> ANI02;
    sc_in <double> ANI03;
    sc_in <double> ANI04;
    sc_in <double> ANI05;
    sc_in <double> ANI06;
    sc_in <double> ANI07;
    sc_in <double> ANI08;
    sc_in <double> ANI09;
    sc_in <double> ANI10;
    sc_in <double> ANI11;
    sc_in <double> ANI12;
    sc_in <double> ANI13;
    sc_in <double> ANI14;
    sc_in <double> ANI15;
    sc_in <double> ANI16;
    sc_in <double> ANI17;
    sc_in <double> ANI18;
    sc_in <double> ANI19;
    sc_in <double> ANI20;
    sc_in <double> ANI21;
    sc_in <double> ANI22;
    sc_in <double> ANI23;
    sc_in <double> ANI24;
    sc_in <double> ANI25;
    sc_in <double> ANI26;
    sc_in <double> ANI27;
    sc_in <double> ANI28;
    sc_in <double> ANI29;
    sc_in <double> ANI30;
    sc_in <double> ANI31;
    sc_in <double> ANI32;
    sc_in <double> ANI33;
    sc_in <double> ANI34;
    sc_in <double> ANI35;
    sc_in <double> TSN_ANI; // Dedicated port for TSN

    sc_in <sc_uint<12> > PVCR_VALUE;
    sc_in <sc_uint<16> > ADOP_OPA1_DATA;
    sc_in <sc_uint<16> > ADOP_OPA2_DATA;
    sc_in <sc_uint<16> > ADOP_OPA3_DATA;

    // Output ports
    sc_out <bool> INT_TSN;
    sc_out <bool> INT_SG1;
    sc_out <bool> INT_SG2;
    sc_out <bool> INT_SG3;
    sc_out <bool> INT_ADE;
    sc_out <bool> ADCATCNV0;
    sc_out <bool> ADCATCNV1;
    sc_out <bool> ADCATCNV2;
    sc_out <bool> ADCATCNV3;
    sc_out <bool> ADCATCNV4;
    sc_out <bool> ULE;
    sc_out <sc_uint<3> > PVCR_MUXCUR;
    sc_out <bool> PVCR_END;
    sc_out <bool> TSN_TS_EN;
    sc_out <bool> TSN_TSSW;
    sc_out <bool> TSN_TSSW_DISCH;
    sc_out <bool> TSN_TSMASK;
    sc_out <bool> TSN_SELF_DIAG;
    sc_out <sc_uint<2> > TSN_TRIM;
    sc_out <bool> ADOP_OPA1_PSEL;
    sc_out <bool> ADOP_OPA1_WEN;
    sc_out <bool> ADOP_OPA2_PSEL;
    sc_out <bool> ADOP_OPA2_WEN;
    sc_out <bool> ADOP_OPA3_PSEL;
    sc_out <bool> ADOP_OPA3_WEN;
    tlm::tlm_target_socket<32u, tlm::tlm_base_protocol_types, 1, SC_ONE_OR_MORE_BOUND> *m_tgt_sockets[1];

    SC_HAS_PROCESS(Csarad113x_w);
    Csarad113x_w(sc_module_name name) : sc_module(name)
                                      ,PWDATA("PWDATA")
                                      // SARAD113x I/O interface
                                      ,pclk ("pclk")
                                      ,ADCLK ("ADCLK")
                                      ,preset_n ("preset_n")
                                      ,AVREFH0 ("AVREFH0")
                                      //trigger input port
                                      ,TSN_TRG ("TSN_TRG")
                                      ,SG1_TRG ("SG1_TRG")
                                      ,SG2_TRG ("SG2_TRG")
                                      ,SG3_TRG ("SG3_TRG")
                                      ,PVCR_TRG ("PVCR_TRG")
                                      //Analog terminal input port
                                      ,ANI00 ("ANI00")
                                      ,ANI01 ("ANI01")
                                      ,ANI02 ("ANI02")
                                      ,ANI03 ("ANI03")
                                      ,ANI04 ("ANI04")
                                      ,ANI05 ("ANI05")
                                      ,ANI06 ("ANI06")
                                      ,ANI07 ("ANI07")
                                      ,ANI08 ("ANI08")
                                      ,ANI09 ("ANI09")
                                      ,ANI10 ("ANI10")
                                      ,ANI11 ("ANI11")
                                      ,ANI12 ("ANI12")
                                      ,ANI13 ("ANI13")
                                      ,ANI14 ("ANI14")
                                      ,ANI15 ("ANI15")
                                      ,ANI16 ("ANI16")
                                      ,ANI17 ("ANI17")
                                      ,ANI18 ("ANI18")
                                      ,ANI19 ("ANI19")
                                      ,ANI20 ("ANI20")
                                      ,ANI21 ("ANI21")
                                      ,ANI22 ("ANI22")
                                      ,ANI23 ("ANI23")
                                      ,ANI24 ("ANI24")
                                      ,ANI25 ("ANI25")
                                      ,ANI26 ("ANI26")
                                      ,ANI27 ("ANI27")
                                      ,ANI28 ("ANI28")
                                      ,ANI29 ("ANI29")
                                      ,ANI30 ("ANI30")
                                      ,ANI31 ("ANI31")
                                      ,ANI32 ("ANI32")
                                      ,ANI33 ("ANI33")
                                      ,ANI34 ("ANI34")
                                      ,ANI35 ("ANI35")
                                      ,TSN_ANI ("TSN_ANI")

                                      ,PVCR_VALUE ("PVCR_VALUE")
                                      ,ADOP_OPA1_DATA("ADOP_OPA1_DATA")
                                      ,ADOP_OPA2_DATA("ADOP_OPA2_DATA")
                                      ,ADOP_OPA3_DATA("ADOP_OPA3_DATA")

                                      //interrupt ouptut port
                                      ,INT_TSN ("INT_TSN")
                                      ,INT_SG1 ("INT_SG1")
                                      ,INT_SG2 ("INT_SG2")
                                      ,INT_SG3 ("INT_SG3")
                                      ,INT_ADE ("INT_ADE")

                                      ,ADCATCNV0 ("ADCATCNV0")
                                      ,ADCATCNV1 ("ADCATCNV1")
                                      ,ADCATCNV2 ("ADCATCNV2")
                                      ,ADCATCNV3 ("ADCATCNV3")
                                      ,ADCATCNV4 ("ADCATCNV4")
                                      ,ULE ("ULE")
                                      ,PVCR_MUXCUR ("PVCR_MUXCUR")
                                      ,PVCR_END ("PVCR_END")
                                      ,TSN_TS_EN ("TSN_TS_EN")
                                      ,TSN_TSSW ("TSN_TSSW")
                                      ,TSN_TSSW_DISCH ("TSN_TSSW_DISCH")
                                      ,TSN_TSMASK ("TSN_TSMASK")
                                      ,TSN_SELF_DIAG ("TSN_SELF_DIAG")
                                      ,TSN_TRIM ("TSN_TRIM")
                                      ,ADOP_OPA1_PSEL("ADOP_OPA1_PSEL")
                                      ,ADOP_OPA1_WEN("ADOP_OPA1_WEN")
                                      ,ADOP_OPA2_PSEL("ADOP_OPA2_PSEL")
                                      ,ADOP_OPA2_WEN("ADOP_OPA2_WEN")
                                      ,ADOP_OPA3_PSEL("ADOP_OPA3_PSEL")
                                      ,ADOP_OPA3_WEN("ADOP_OPA3_WEN")
    {//{{{
        sarad113x = new Csarad113x("sarad113x");
        sc_assert(sarad113x != NULL);
        // SARAD113x connection
        this->m_tgt_sockets[0] = sarad113x->m_tgt_sockets[0];
        PWDATA.initialize(0);
        sarad113x->pclk(pclk);
        sarad113x->ADCLK(ADCLK);
        sarad113x->preset_n(preset_n);
        sarad113x->INT_TSN(INT_TSN);
        sarad113x->INT_SG1(INT_SG1);
        sarad113x->INT_SG2(INT_SG2);
        sarad113x->INT_SG3(INT_SG3);
        sarad113x->INT_ADE(INT_ADE);
        sarad113x->TSN_TRG(TSN_TRG);
        sarad113x->SG1_TRG(SG1_TRG);
        sarad113x->SG2_TRG(SG2_TRG);
        sarad113x->SG3_TRG(SG3_TRG);
        sarad113x->PVCR_TRG(PVCR_TRG);
        sarad113x->AVREFH0(AVREFH0);
        sarad113x->ADCATCNV0(ADCATCNV0);
        sarad113x->ADCATCNV1(ADCATCNV1);
        sarad113x->ADCATCNV2(ADCATCNV2);
        sarad113x->ADCATCNV3(ADCATCNV3);
        sarad113x->ADCATCNV4(ADCATCNV4);
        sarad113x->ANI00(ANI00);
        sarad113x->ANI01(ANI01);
        sarad113x->ANI02(ANI02);
        sarad113x->ANI03(ANI03);
        sarad113x->ANI04(ANI04);
        sarad113x->ANI05(ANI05);
        sarad113x->ANI06(ANI06);
        sarad113x->ANI07(ANI07);
        sarad113x->ANI08(ANI08);
        sarad113x->ANI09(ANI09);
        sarad113x->ANI10(ANI10);
        sarad113x->ANI11(ANI11);
        sarad113x->ANI12(ANI12);
        sarad113x->ANI13(ANI13);
        sarad113x->ANI14(ANI14);
        sarad113x->ANI15(ANI15);
        sarad113x->ANI16(ANI16);
        sarad113x->ANI17(ANI17);
        sarad113x->ANI18(ANI18);
        sarad113x->ANI19(ANI19);
        sarad113x->ANI20(ANI20);
        sarad113x->ANI21(ANI21);
        sarad113x->ANI22(ANI22);
        sarad113x->ANI23(ANI23);
        sarad113x->ANI24(ANI24);
        sarad113x->ANI25(ANI25);
        sarad113x->ANI26(ANI26);
        sarad113x->ANI27(ANI27);
        sarad113x->ANI28(ANI28);
        sarad113x->ANI29(ANI29);
        sarad113x->ANI30(ANI30);
        sarad113x->ANI31(ANI31);
        sarad113x->ANI32(ANI32);
        sarad113x->ANI33(ANI33);
        sarad113x->ANI34(ANI34);
        sarad113x->ANI35(ANI35);
        sarad113x->TSN_ANI(TSN_ANI);
        sarad113x->ULE(ULE);
        sarad113x->PVCR_VALUE(PVCR_VALUE);
        sarad113x->PVCR_MUXCUR(PVCR_MUXCUR);
        sarad113x->PVCR_END(PVCR_END);
        sarad113x->TSN_TS_EN(TSN_TS_EN);
        sarad113x->TSN_TSSW(TSN_TSSW);
        sarad113x->TSN_TSSW_DISCH(TSN_TSSW_DISCH);
        sarad113x->TSN_TSMASK(TSN_TSMASK);
        sarad113x->TSN_SELF_DIAG(TSN_SELF_DIAG);
        sarad113x->TSN_TRIM(TSN_TRIM);
        sarad113x->ADOP_OPA1_DATA(ADOP_OPA1_DATA);
        sarad113x->ADOP_OPA2_DATA(ADOP_OPA2_DATA);
        sarad113x->ADOP_OPA3_DATA(ADOP_OPA3_DATA);
        sarad113x->ADOP_OPA1_PSEL(ADOP_OPA1_PSEL);
        sarad113x->ADOP_OPA1_WEN(ADOP_OPA1_WEN);
        sarad113x->ADOP_OPA2_PSEL(ADOP_OPA2_PSEL);
        sarad113x->ADOP_OPA2_WEN(ADOP_OPA2_WEN);
        sarad113x->ADOP_OPA3_PSEL(ADOP_OPA3_PSEL);
        sarad113x->ADOP_OPA3_WEN(ADOP_OPA3_WEN);

        SC_METHOD(UpdatePWDATAMethod);
        dont_initialize();
        sensitive << sarad113x->mWrittenPWDATAEvent;
    }//}}}

    ~Csarad113x_w()
    {//{{{
    }//}}}

    std::string handleCommand (const std::vector<std::string>& args)
    {//{{{
        return sarad113x->handleCommand(args);
    }//}}}

    Csarad113x* GetSarad113xPointer()
    {
        return sarad113x;
    }
private:
    Csarad113x* sarad113x;
    
    void UpdatePWDATAMethod(void)
    {//{{{
        PWDATA.write((sc_uint<32>)(sarad113x->GetWrittenData()));
    }//}}}

};

#endif //__SARAD113x_W_H__
// vim600: set foldmethod=marker :
