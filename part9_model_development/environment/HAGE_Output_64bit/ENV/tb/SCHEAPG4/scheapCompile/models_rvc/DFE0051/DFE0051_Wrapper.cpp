// ---------------------------------------------------------------------
// $Id: DFE0051_Wrapper.cpp,v 1.2 2017/07/21 08:48:22 chuonghoangle Exp $
//
// Copyright(c) 2016 Renesas Electronics Corporation
// Copyright(c) 2016 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "DFE0051_Wrapper.h"

/// Constructor
Cdfe0051_wrapper::Cdfe0051_wrapper(sc_module_name name):
        sc_module(name)
        ,BusSlaveBase<32,3>()
        ,CLK_HSB("CLK_HSB")
        ,CLK_UHSB("CLK_UHSB")
        ,PRESETn("PRESETn")

        ,SARADC0_REQ("SARADC0_REQ")
        ,SARADC0_TAG("SARADC0_TAG")
        ,SARADC0_DATA("SARADC0_DATA")

        ,SARADC1_REQ("SARADC1_REQ")
        ,SARADC1_TAG("SARADC1_TAG")
        ,SARADC1_DATA("SARADC1_DATA")

        ,SARADC2_REQ("SARADC2_REQ")
        ,SARADC2_TAG("SARADC2_TAG")
        ,SARADC2_DATA("SARADC2_DATA")

        ,SARADC3_REQ("SARADC3_REQ")
        ,SARADC3_TAG("SARADC3_TAG")
        ,SARADC3_DATA("SARADC3_DATA")

        ,DSADC00_REQ("DSADC00_REQ")
        ,DSADC00_TAG("DSADC00_TAG")
        ,DSADC00_DATA("DSADC00_DATA")

        ,DSADC10_REQ("DSADC10_REQ")
        ,DSADC10_TAG("DSADC10_TAG")
        ,DSADC10_DATA("DSADC10_DATA")

        ,DSADC20_REQ("DSADC20_REQ")
        ,DSADC20_TAG("DSADC20_TAG")
        ,DSADC20_DATA("DSADC20_DATA")

        ,DSADC11_REQ("DSADC11_REQ")
        ,DSADC11_TAG("DSADC11_TAG")
        ,DSADC11_DATA("DSADC11_DATA")

        ,DSADC12_REQ("DSADC12_REQ")
        ,DSADC12_TAG("DSADC12_TAG")
        ,DSADC12_DATA("DSADC12_DATA")

        ,DSADC13_REQ("DSADC13_REQ")
        ,DSADC13_TAG("DSADC13_TAG")
        ,DSADC13_DATA("DSADC13_DATA")

        ,DSADC14_REQ("DSADC14_REQ")
        ,DSADC14_TAG("DSADC14_TAG")
        ,DSADC14_DATA("DSADC14_DATA")

        ,DSADC15_REQ("DSADC15_REQ")
        ,DSADC15_TAG("DSADC15_TAG")
        ,DSADC15_DATA("DSADC15_DATA")

        ,DSADC21_REQ("DSADC21_REQ")
        ,DSADC21_TAG("DSADC21_TAG")
        ,DSADC21_DATA("DSADC21_DATA")

        ,DSADC22_REQ("DSADC22_REQ")
        ,DSADC22_TAG("DSADC22_TAG")
        ,DSADC22_DATA("DSADC22_DATA")

        ,CADC0_REQ("CADC0_REQ")
        ,CADC0_TAG("CADC0_TAG")
        ,CADC0_DATA("CADC0_DATA")

        ,TMFIFOCAPTRG("TMFIFOCAPTRG")
        ,INTDFE00510ERR("INTDFE00510ERR")
        ,INTDFE00511ERR("INTDFE00511ERR")
        ,INTDFE0051FIFOOUTA("INTDFE0051FIFOOUTA")
        ,INTDFE0051FIFOOUTB("INTDFE0051FIFOOUTB")
{//{{{
    // Initialize
    CommandInit(this->name());
    CreateChannelConnectMatrix();
    mDumpInterrupt = false;
    mPreINTDFE00510ERR = false;
    mPreINTDFE00511ERR = false;
    for (unsigned int i = 0; i < DFE00510_NUM_CHANNEL; i++){
        mPreINTDFE00510DOUTCND[i] = false;
    }
    for (unsigned int i = 0; i < DFE00511_NUM_CHANNEL; i++){
        mPreINTDFE00511DOUTCND[i] = false;
    }
    for (unsigned int i = 0; i < 4; i++){
        mPreINTDFE00510CND[i] = false;
    }
    for (unsigned int i = 0; i < 4; i++){
        mPreINTDFE00511CND[i] = false;
    }
    for (unsigned int i = 0; i < 3; i++){
        mPreINTDFE00510SUBOUT[i] = false;
    }
    for (unsigned int i = 0; i < 3; i++){
        mPreINTDFE00511SUBOUT[i] = false;
    }
    mPreINTDFE0051FIFOOUTA = false;
    mPreINTDFE0051FIFOOUTB = false;

    // Declare & instantiate array of ports
    for (unsigned int index = 0; index < 20; index++){
        std::ostringstream str_TMTRG_A;
        std::ostringstream str_TMTRG_B;
        str_TMTRG_A<<"TMTRG_A"<<index;
        str_TMTRG_B<<"TMTRG_B"<<index;
        TMTRG_A[index] = new sc_in<bool>(str_TMTRG_A.str().c_str());
        TMTRG_B[index] = new sc_in<bool>(str_TMTRG_B.str().c_str());
    }
    for (unsigned int index = 0; index < 3; index++){
        std::ostringstream str_TMCAPTRG;
        str_TMCAPTRG<<"TMCAPTRG"<<index;
        TMCAPTRG[index] = new sc_in<bool>(str_TMCAPTRG.str().c_str());
    }
    for (unsigned int index = 0; index < DFE00510_NUM_CHANNEL; index++){
        std::ostringstream str_INTDFE00510DOUTCND;
        str_INTDFE00510DOUTCND<<"INTDFE00510DOUTCND"<<index;
        INTDFE00510DOUTCND[index] = new sc_out<bool>(str_INTDFE00510DOUTCND.str().c_str());
        INTDFE00510DOUTCND[index]->initialize(false);
    }
    for (unsigned int index = 0; index < DFE00511_NUM_CHANNEL; index++){
        std::ostringstream str_INTDFE00511DOUTCND;
        str_INTDFE00511DOUTCND<<"INTDFE00511DOUTCND"<<index;
        INTDFE00511DOUTCND[index] = new sc_out<bool>(str_INTDFE00511DOUTCND.str().c_str());
        INTDFE00511DOUTCND[index]->initialize(false);
    }
    for (unsigned int index = 0; index < 4; index++){
        std::ostringstream str_INTDFE00510CND;
        str_INTDFE00510CND<<"INTDFE00510CND"<<index;
        INTDFE00510CND[index] = new sc_out<bool>(str_INTDFE00510CND.str().c_str());
        INTDFE00510CND[index]->initialize(false);

        std::ostringstream str_DFE00510_PHUPD;
        str_DFE00510_PHUPD<<"DFE00510_PHUPD"<<index;
        DFE00510_PHUPD[index] = new sc_out<bool>(str_DFE00510_PHUPD.str().c_str());
        DFE00510_PHUPD[index]->initialize(false);
    }
    for (unsigned int index = 0; index < 4; index++){
        std::ostringstream str_INTDFE00511CND;
        str_INTDFE00511CND<<"INTDFE00511CND"<<index;
        INTDFE00511CND[index] = new sc_out<bool>(str_INTDFE00511CND.str().c_str());
        INTDFE00511CND[index]->initialize(false);

        std::ostringstream str_DFE00511_PHUPD;
        str_DFE00511_PHUPD<<"DFE00511_PHUPD"<<index;
        DFE00511_PHUPD[index] = new sc_out<bool>(str_DFE00511_PHUPD.str().c_str());
        DFE00511_PHUPD[index]->initialize(false);
    }
    for (unsigned int index = 0; index < 3; index++){
        std::ostringstream str_INTDFE00510SUBOUT;
        str_INTDFE00510SUBOUT<<"INTDFE00510SUBOUT"<<index;
        INTDFE00510SUBOUT[index] = new sc_out<bool>(str_INTDFE00510SUBOUT.str().c_str());
        INTDFE00510SUBOUT[index]->initialize(false);
    }
    for (unsigned int index = 0; index < 3; index++){
        std::ostringstream str_INTDFE00511SUBOUT;
        str_INTDFE00511SUBOUT<<"INTDFE00511SUBOUT"<<index;
        INTDFE00511SUBOUT[index] = new sc_out<bool>(str_INTDFE00511SUBOUT.str().c_str());
        INTDFE00511SUBOUT[index]->initialize(false);
    }
    // Instances
    mDFE00510 = new Cdfe0051<DFE00510_NUM_CHANNEL>("DFE00510", this);
    mDFE00511 = new Cdfe0051<DFE00511_NUM_CHANNEL>("DFE00511", this);
    mFIFO = new DFE0051_FIFO("DFE0051_FIFO");
    sc_assert((mDFE00510 != NULL) && (mDFE00511 != NULL) && (mFIFO != NULL));

    // Connect port DFE00510
    mDFE00510->CLK_HSB(CLK_HSB);
    mDFE00510->CLK_UHSB(CLK_UHSB);
    mDFE00510->PRESETn(PRESETn);
    // SAR-ADC input
    mDFE00510->SARADC0_REQ(SARADC0_REQ);
    mDFE00510->SARADC0_TAG(SARADC0_TAG);
    mDFE00510->SARADC0_DATA(SARADC0_DATA);
    mDFE00510->SARADC1_REQ(SARADC1_REQ);
    mDFE00510->SARADC1_TAG(SARADC1_TAG);
    mDFE00510->SARADC1_DATA(SARADC1_DATA);
    mDFE00510->SARADC2_REQ(SARADC2_REQ);
    mDFE00510->SARADC2_TAG(SARADC2_TAG);
    mDFE00510->SARADC2_DATA(SARADC2_DATA);
    mDFE00510->SARADC3_REQ(SARADC3_REQ);
    mDFE00510->SARADC3_TAG(SARADC3_TAG);
    mDFE00510->SARADC3_DATA(SARADC3_DATA);
    // DS-ADC input
    mDFE00510->DSADC00_REQ(DSADC00_REQ);
    mDFE00510->DSADC00_TAG(DSADC00_TAG);
    mDFE00510->DSADC00_DATA(DSADC00_DATA);
    mDFE00510->DSADC10_REQ(DSADC10_REQ);
    mDFE00510->DSADC10_TAG(DSADC10_TAG);
    mDFE00510->DSADC10_DATA(DSADC10_DATA);
    mDFE00510->DSADC20_REQ(DSADC20_REQ);
    mDFE00510->DSADC20_TAG(DSADC20_TAG);
    mDFE00510->DSADC20_DATA(DSADC20_DATA);
    mDFE00510->DSADC11_REQ(DSADC11_REQ);
    mDFE00510->DSADC11_TAG(DSADC11_TAG);
    mDFE00510->DSADC11_DATA(DSADC11_DATA);
    mDFE00510->DSADC12_REQ(DSADC12_REQ);
    mDFE00510->DSADC12_TAG(DSADC12_TAG);
    mDFE00510->DSADC12_DATA(DSADC12_DATA);
    mDFE00510->DSADC13_REQ(DSADC13_REQ);
    mDFE00510->DSADC13_TAG(DSADC13_TAG);
    mDFE00510->DSADC13_DATA(DSADC13_DATA);
    mDFE00510->DSADC14_REQ(DSADC14_REQ);
    mDFE00510->DSADC14_TAG(DSADC14_TAG);
    mDFE00510->DSADC14_DATA(DSADC14_DATA);
    mDFE00510->DSADC15_REQ(DSADC15_REQ);
    mDFE00510->DSADC15_TAG(DSADC15_TAG);
    mDFE00510->DSADC15_DATA(DSADC15_DATA);
    mDFE00510->DSADC21_REQ(DSADC21_REQ);
    mDFE00510->DSADC21_TAG(DSADC21_TAG);
    mDFE00510->DSADC21_DATA(DSADC21_DATA);
    mDFE00510->DSADC22_REQ(DSADC22_REQ);
    mDFE00510->DSADC22_TAG(DSADC22_TAG);
    mDFE00510->DSADC22_DATA(DSADC22_DATA);
    // Cyclic-ADC input
    mDFE00510->CADC0_REQ(CADC0_REQ);
    mDFE00510->CADC0_TAG(CADC0_TAG);
    mDFE00510->CADC0_DATA(CADC0_DATA);
    // Error
    mDFE00510->INTDFE0051ERR(INTDFE00510ERR_sig);

    // Connect ports of DFE00511
    mDFE00511->CLK_HSB(CLK_HSB);
    mDFE00511->CLK_UHSB(CLK_UHSB);
    mDFE00511->PRESETn(PRESETn);
    // SAR-ADC input
    mDFE00511->SARADC0_REQ(SARADC0_REQ);
    mDFE00511->SARADC0_TAG(SARADC0_TAG);
    mDFE00511->SARADC0_DATA(SARADC0_DATA);
    mDFE00511->SARADC1_REQ(SARADC1_REQ);
    mDFE00511->SARADC1_TAG(SARADC1_TAG);
    mDFE00511->SARADC1_DATA(SARADC1_DATA);
    mDFE00511->SARADC2_REQ(SARADC2_REQ);
    mDFE00511->SARADC2_TAG(SARADC2_TAG);
    mDFE00511->SARADC2_DATA(SARADC2_DATA);
    mDFE00511->SARADC3_REQ(SARADC3_REQ);
    mDFE00511->SARADC3_TAG(SARADC3_TAG);
    mDFE00511->SARADC3_DATA(SARADC3_DATA);
    // DS-ADC input
    mDFE00511->DSADC00_REQ(DSADC00_REQ);
    mDFE00511->DSADC00_TAG(DSADC00_TAG);
    mDFE00511->DSADC00_DATA(DSADC00_DATA);
    mDFE00511->DSADC10_REQ(DSADC10_REQ);
    mDFE00511->DSADC10_TAG(DSADC10_TAG);
    mDFE00511->DSADC10_DATA(DSADC10_DATA);
    mDFE00511->DSADC20_REQ(DSADC20_REQ);
    mDFE00511->DSADC20_TAG(DSADC20_TAG);
    mDFE00511->DSADC20_DATA(DSADC20_DATA);
    mDFE00511->DSADC11_REQ(DSADC11_REQ);
    mDFE00511->DSADC11_TAG(DSADC11_TAG);
    mDFE00511->DSADC11_DATA(DSADC11_DATA);
    mDFE00511->DSADC12_REQ(DSADC12_REQ);
    mDFE00511->DSADC12_TAG(DSADC12_TAG);
    mDFE00511->DSADC12_DATA(DSADC12_DATA);
    mDFE00511->DSADC13_REQ(DSADC13_REQ);
    mDFE00511->DSADC13_TAG(DSADC13_TAG);
    mDFE00511->DSADC13_DATA(DSADC13_DATA);
    mDFE00511->DSADC14_REQ(DSADC14_REQ);
    mDFE00511->DSADC14_TAG(DSADC14_TAG);
    mDFE00511->DSADC14_DATA(DSADC14_DATA);
    mDFE00511->DSADC15_REQ(DSADC15_REQ);
    mDFE00511->DSADC15_TAG(DSADC15_TAG);
    mDFE00511->DSADC15_DATA(DSADC15_DATA);
    mDFE00511->DSADC21_REQ(DSADC21_REQ);
    mDFE00511->DSADC21_TAG(DSADC21_TAG);
    mDFE00511->DSADC21_DATA(DSADC21_DATA);
    mDFE00511->DSADC22_REQ(DSADC22_REQ);
    mDFE00511->DSADC22_TAG(DSADC22_TAG);
    mDFE00511->DSADC22_DATA(DSADC22_DATA);
    // Cyclic-ADC input
    mDFE00511->CADC0_REQ(CADC0_REQ);
    mDFE00511->CADC0_TAG(CADC0_TAG);
    mDFE00511->CADC0_DATA(CADC0_DATA);
    // Error
    mDFE00511->INTDFE0051ERR(INTDFE00511ERR);
    for (unsigned int index = 0; index < 20; index++){
        (mDFE00510->TMTRG_A[index])->bind(*TMTRG_A[index]);
        (mDFE00510->TMTRG_B[index])->bind(*TMTRG_B[index]);
        (mDFE00511->TMTRG_A[index])->bind(*TMTRG_A[index]);
        (mDFE00511->TMTRG_B[index])->bind(*TMTRG_B[index]);
    }
    for (unsigned int index = 0; index < 3; index++){
        (mDFE00510->TMCAPTRG[index])->bind(*TMCAPTRG[index]);
        (mDFE00511->TMCAPTRG[index])->bind(*TMCAPTRG[index]);
    }
    for (unsigned int index = 0; index < DFE00510_NUM_CHANNEL; index++){
        (mDFE00510->DFE0051_FIFO_CACK[index])->bind(DFE00510_FIFO_CACK_sig[index]);
        (mDFE00510->DFE0051_DOUPD_N[index])->bind(DFE00510_DOUPD_N_sig[index]);
        (mDFE00510->DFE0051_DOUT_END[index])->bind(DFE00510_DOUT_END_sig[index]);
        (mDFE00510->DFE0051_DFDOUT[index])->bind(DFE00510_DFDOUT_sig[index]);

        (mDFE00510->INTDFE0051CND0[index])->bind(INTDFE00510CND0_sig[index]);
        (mDFE00510->INTDFE0051DOUT[index])->bind(INTDFE00510DOUT_sig[index]);
    }
    for (unsigned int index = 0; index < DFE00511_NUM_CHANNEL; index++){
        (mDFE00511->DFE0051_FIFO_CACK[index])->bind(DFE00511_FIFO_CACK_sig[index]);
        (mDFE00511->DFE0051_DOUPD_N[index])->bind(DFE00511_DOUPD_N_sig[index]);
        (mDFE00511->DFE0051_DOUT_END[index])->bind(DFE00511_DOUT_END_sig[index]);
        (mDFE00511->DFE0051_DFDOUT[index])->bind(DFE00511_DFDOUT_sig[index]);

        (mDFE00511->INTDFE0051CND0[index])->bind(INTDFE00511CND0_sig[index]);
        (mDFE00511->INTDFE0051DOUT[index])->bind(INTDFE00511DOUT_sig[index]);
    }
    for (unsigned int index = 0; index < 4; index++){
        (mDFE00510->INTDFE0051CND1[index])->bind(*(this->INTDFE00510CND[index]));
        (mDFE00511->INTDFE0051CND1[index])->bind(*(this->INTDFE00511CND[index]));
        (mDFE00510->DFE0051_PHUPD[index])->bind(*(this->DFE00510_PHUPD[index]));
        (mDFE00511->DFE0051_PHUPD[index])->bind(*(this->DFE00511_PHUPD[index]));
    }
    for (unsigned int index = 0; index < 3; index++){
        (mDFE00510->INTDFE0051SUBOUT[index])->bind(*(this->INTDFE00510SUBOUT[index]));
        (mDFE00511->INTDFE0051SUBOUT[index])->bind(*(this->INTDFE00511SUBOUT[index]));
    }

    // Bind clock, reset signal and socket to bus
    setSlaveFreqPort32(&CLK_HSB, &CLK_HSB, &CLK_HSB);
    setSlaveResetPort32(&PRESETn, &PRESETn, &PRESETn);
    setTargetSocket32("tsdfe00510", "tsdfe00511", "tsfifo");
    tsdfe00510 = BusSlaveBase<32,3>::tSocket32[0];
    tsdfe00511 = BusSlaveBase<32,3>::tSocket32[1];
    tsfifo = BusSlaveBase<32,3>::tSocket32[2];
    

    mBusSlaveIf32[0]->setFuncModulePtr(mDFE00510->mDFE0051Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setTransNmbLmt(1);
    mBusSlaveIf32[1]->setFuncModulePtr(mDFE00511->mDFE0051Func);
    mBusSlaveIf32[1]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[1]->setTransNmbLmt(1);
    mBusSlaveIf32[2]->setFuncModulePtr(mFIFO->getBusSlaveFunc());
    mBusSlaveIf32[2]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[2]->setTransNmbLmt(1);


    mFIFO->pclk   .bind(CLK_HSB);
    mFIFO->presetn.bind(PRESETn);
    mFIFO->dfe0051_tim_ctrig.bind(TMFIFOCAPTRG);
    for (unsigned int cid = 0; cid < DFE00510_NUM_CHANNEL; cid++){
         mFIFO->dfe0051_do_ctrig  [0][cid]->bind(DFE00510_DOUT_END_sig [cid]);
         mFIFO->dfe0051_do        [0][cid]->bind(DFE00510_DFDOUT_sig   [cid]);
         mFIFO->dfe0051_do_upd    [0][cid]->bind(DFE00510_DOUPD_N_sig  [cid]);
         mFIFO->dfe0051_dfifo_cack[0][cid]->bind(DFE00510_FIFO_CACK_sig[cid]);
    }
    for (unsigned int cid = 0; cid < DFE00511_NUM_CHANNEL; cid++){
         mFIFO->dfe0051_do_ctrig  [1][cid]->bind(DFE00511_DOUT_END_sig [cid]);
         mFIFO->dfe0051_do        [1][cid]->bind(DFE00511_DFDOUT_sig   [cid]);
         mFIFO->dfe0051_do_upd    [1][cid]->bind(DFE00511_DOUPD_N_sig  [cid]);
         mFIFO->dfe0051_dfifo_cack[1][cid]->bind(DFE00511_FIFO_CACK_sig[cid]);
    }
    mFIFO->dfe0051_ba_ce_irq.bind(INTDFE0051FIFOOUTA);
    mFIFO->dfe0051_bb_ce_irq.bind(INTDFE0051FIFOOUTB); 
    mFIFO->dfe0051_berr_irq .bind(INTDFE0051FIFOERR_sig); 
    
    SC_METHOD(DumpInterruptMethod);
    dont_initialize();
    for (unsigned int i = 0; i <DFE00510_NUM_CHANNEL; i++){
        sensitive << (*INTDFE00510DOUTCND[i]);
    }
    for (unsigned int i = 0; i <DFE00511_NUM_CHANNEL; i++){
        sensitive << (*INTDFE00511DOUTCND[i]);
    }
    for (unsigned int i = 0; i <4; i++){
        sensitive << (*INTDFE00510CND[i]);
        sensitive << (*INTDFE00511CND[i]);
    }
    for (unsigned int i = 0; i <3; i++){
        sensitive << (*INTDFE00510SUBOUT[i]);
        sensitive << (*INTDFE00511SUBOUT[i]);
    }
    sensitive << INTDFE00510ERR;
    sensitive << INTDFE00511ERR;
    sensitive << INTDFE0051FIFOOUTA;
    sensitive << INTDFE0051FIFOOUTB;

    SC_METHOD(MergeINTDFE0051DOUTCNDMethod);
    dont_initialize();
    for (unsigned int i = 0; i <DFE00510_NUM_CHANNEL; i++){
        sensitive << (INTDFE00510CND0_sig[i]);
        sensitive << (INTDFE00510DOUT_sig[i]);
    }
    for (unsigned int i = 0; i <DFE00511_NUM_CHANNEL; i++){
        sensitive << (INTDFE00511CND0_sig[i]);
        sensitive << (INTDFE00511DOUT_sig[i]);
    }

    SC_METHOD(MergeINTDFE00510ERRMethod);
    dont_initialize();
    sensitive << INTDFE00510ERR_sig;
    sensitive << INTDFE0051FIFOERR_sig;
}//}}}

/// Destructor
Cdfe0051_wrapper::~Cdfe0051_wrapper(void)
{//{{{
    delete mDFE00510;
    delete mDFE00511;
    delete mFIFO;
}//}}}

/// Create valid channel connection matrix (with input source)
void Cdfe0051_wrapper::CreateChannelConnectMatrix()
{//{{{
    unsigned int total_channel = DFE00511_NUM_CHANNEL + DFE00510_NUM_CHANNEL;
    // Table of valid assignment (DFE00510: CH0-15, DFE00511: CH16-19),
    // This table should be changed if the table "Data that can be Input for Each Channel" changes.
    //                       CH0    CH1    CH2    CH3    CH4    CH5    CH6    CH7    CH8    CH9    CH10   CH11   CH12   CH13   CH14   CH15   CH16   CH17   CH18   CH19
    bool matrix[20][20] =  {{true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true }, // SW
                            {true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,false ,false ,false ,false}, // SARADC0
                            {true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,false ,false ,false ,false}, // SARADC2
                            {true  ,true  ,true  ,true  ,true  ,true  ,false ,false ,false ,false ,false ,false ,false ,false ,false ,false ,false ,false ,false ,false}, // SARADC1
                            {false ,false ,false ,false ,false ,false ,true  ,true  ,true  ,true  ,true  ,true  ,false ,false ,false ,false ,false ,false ,false ,false}, // SARADC3
                            {true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true }, // DSADC00
                            {true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true }, // DSADC10
                            {true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true }, // DSADC20
                            {true  ,false ,false ,false ,false ,true  ,false ,false ,false ,false ,true  ,false ,false ,false ,false ,false ,true  ,false ,false ,false}, // DSADC11
                            {false ,true  ,false ,false ,false ,false ,true  ,false ,false ,false ,false ,true  ,false ,false ,false ,false ,false ,false ,false ,false}, // DSADC12
                            {false ,false ,true  ,false ,false ,false ,false ,true  ,false ,false ,false ,false ,true  ,false ,false ,false ,false ,false ,false ,false}, // DSADC13
                            {false ,false ,false ,true  ,false ,false ,false ,false ,true  ,false ,false ,false ,false ,true  ,false ,false ,false ,false ,false ,false}, // DSADC14
                            {false ,false ,false ,false ,true  ,false ,false ,false ,false ,true  ,false ,false ,false ,false ,true  ,false ,false ,false ,false ,false}, // DSADC15
                            {false ,false ,false ,false ,false ,false ,false ,false ,false ,false ,false ,false ,false ,true  ,false ,true  ,false ,false ,false ,false}, // DSADC21
                            {false ,false ,false ,false ,false ,false ,false ,false ,false ,false ,false ,false ,true  ,false ,true  ,false ,false ,false ,false ,false}, // DSADC22
                            {true  ,true  ,true  ,true  ,false ,false ,false ,false ,false ,false ,false ,false ,false ,false ,false ,false ,true  ,true  ,true  ,true }, // CADC0
                            {true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true }, // DeciCas
                            {true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true }, // SubCas0
                            {true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true }, // SubCas1
                            {true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true}}; // SubCas2
    for (unsigned int i = 0; i < 20; i++){// Scan all 20 input source
        for (unsigned int j = 0; j<total_channel;j++){
            mChannelConnectMatrix[i][j] = matrix[i][j];
        }
    }
}//}}}

/// Dump interrupt info
void Cdfe0051_wrapper::DumpInterruptMethod()
{//{{{
    if (mDumpInterrupt){
        bool intdfe00510err = INTDFE00510ERR.read();
        bool intdfe00511err = INTDFE00511ERR.read();
        bool intdfe00510doutcnd[DFE00510_NUM_CHANNEL];
        for (unsigned int i = 0; i < DFE00510_NUM_CHANNEL; i++){
            intdfe00510doutcnd[i] = INTDFE00510DOUTCND[i]->read();
        }
        bool intdfe00511doutcnd[DFE00511_NUM_CHANNEL];
        for (unsigned int i = 0; i < DFE00511_NUM_CHANNEL; i++){
            intdfe00511doutcnd[i] = INTDFE00511DOUTCND[i]->read();
        }
        bool intdfe00510cnd[4];
        for (unsigned int i = 0; i < 4; i++){
            intdfe00510cnd[i] = INTDFE00510CND[i]->read();
        }
        bool intdfe00511cnd[4];
        for (unsigned int i = 0; i < 4; i++){
            intdfe00511cnd[i] = INTDFE00511CND[i]->read();
        }
        bool intdfe00510subout[3];
        for (unsigned int i = 0; i < 3; i++){
            intdfe00510subout[i] = INTDFE00510SUBOUT[i]->read();
        }
        bool intdfe00511subout[3];
        for (unsigned int i = 0; i < 3; i++){
            intdfe00511subout[i] = INTDFE00511SUBOUT[i]->read();
        }
        bool intdfe0051fifoouta = INTDFE0051FIFOOUTA.read();
        bool intdfe0051fifooutb = INTDFE0051FIFOOUTB.read();

        if (intdfe00510err && !mPreINTDFE00510ERR){
            re_printf("info", "INT [INTDFE00510ERR] assert.\n");
        }
        if (!intdfe00510err && mPreINTDFE00510ERR){
            re_printf("info", "INT [INTDFE00510ERR] de-assert.\n");
        }
        if (intdfe00511err && !mPreINTDFE00511ERR){
            re_printf("info", "INT [INTDFE00511ERR] assert.\n");
        }
        if (!intdfe00511err && mPreINTDFE00511ERR){
            re_printf("info", "INT [INTDFE00511ERR] de-assert.\n");
        }
        for (unsigned int i = 0; i < DFE00510_NUM_CHANNEL; i++){
            if (intdfe00510doutcnd[i] && !mPreINTDFE00510DOUTCND[i]){
                re_printf("info", "INT [INTDFE00510DOUTCND%d] assert.\n", i);
            }
            if (!intdfe00510doutcnd[i] && mPreINTDFE00510DOUTCND[i]){
                re_printf("info", "INT [INTDFE00510DOUTCND%d] de-assert.\n", i);
            }
        }
        for (unsigned int i = 0; i < DFE00511_NUM_CHANNEL; i++){
            if (intdfe00511doutcnd[i] && !mPreINTDFE00511DOUTCND[i]){
                re_printf("info", "INT [INTDFE00511DOUTCND%d] assert.\n", i);
            }
            if (!intdfe00511doutcnd[i] && mPreINTDFE00511DOUTCND[i]){
                re_printf("info", "INT [INTDFE00511DOUTCND%d] de-assert.\n", i);
            }
        }
        for (unsigned int i = 0; i < 4; i++){
            if (intdfe00510cnd[i] && !mPreINTDFE00510CND[i]){
                re_printf("info", "INT [INTDFE00510CND%d] assert.\n", i);
            }
            if (!intdfe00510cnd[i] && mPreINTDFE00510CND[i]){
                re_printf("info", "INT [INTDFE00510CND%d] de-assert.\n", i);
            }
        }
        for (unsigned int i = 0; i < 4; i++){
            if (intdfe00511cnd[i] && !mPreINTDFE00511CND[i]){
                re_printf("info", "INT [INTDFE00511CND%d] assert.\n", i);
            }
            if (!intdfe00511cnd[i] && mPreINTDFE00511CND[i]){
                re_printf("info", "INT [INTDFE00511CND%d] de-assert.\n", i);
            }
        }
        for (unsigned int i = 0; i < 3; i++){
            if (intdfe00510subout[i] && !mPreINTDFE00510SUBOUT[i]){
                re_printf("info", "INT [INTDFE00510SUBOUT%d] assert.\n", i);
            }
            if (!intdfe00510subout[i] && mPreINTDFE00510SUBOUT[i]){
                re_printf("info", "INT [INTDFE00510SUBOUT%d] de-assert.\n", i);
            }
        }
        for (unsigned int i = 0; i < 3; i++){
            if (intdfe00511subout[i] && !mPreINTDFE00511SUBOUT[i]){
                re_printf("info", "INT [INTDFE00511SUBOUT%d] assert.\n", i);
            }
            if (!intdfe00511subout[i] && mPreINTDFE00511SUBOUT[i]){
                re_printf("info", "INT [INTDFE00511SUBOUT%d] de-assert.\n", i);
            }
        }
        if (intdfe0051fifoouta && !mPreINTDFE0051FIFOOUTA){
            re_printf("info", "INT [INTDFE0051FIFOOUTA] assert.\n");
        }
        if (!intdfe0051fifoouta && mPreINTDFE0051FIFOOUTA){
            re_printf("info", "INT [INTDFE0051FIFOOUTA] de-assert.\n");
        }
        if (intdfe0051fifooutb && !mPreINTDFE0051FIFOOUTB){
            re_printf("info", "INT [INTDFE0051FIFOOUTB] assert.\n");
        }
        if (!intdfe0051fifooutb && mPreINTDFE0051FIFOOUTB){
            re_printf("info", "INT [INTDFE0051FIFOOUTB] de-assert.\n");
        }

        // Update pre-value
        mPreINTDFE00510ERR = intdfe00510err;
        mPreINTDFE00511ERR = intdfe00511err;
        for (unsigned int i = 0; i < DFE00510_NUM_CHANNEL; i++){
            mPreINTDFE00510DOUTCND[i] = intdfe00510doutcnd[i];
        }
        for (unsigned int i = 0; i < DFE00511_NUM_CHANNEL; i++){
            mPreINTDFE00511DOUTCND[i] = intdfe00511doutcnd[i];
        }
        for (unsigned int i = 0; i < 4; i++){
            mPreINTDFE00510CND[i] = intdfe00510cnd[i];
        }
        for (unsigned int i = 0; i < 4; i++){
            mPreINTDFE00511CND[i] = intdfe00511cnd[i];
        }
        for (unsigned int i = 0; i < 3; i++){
            mPreINTDFE00510SUBOUT[i] = intdfe00510subout[i];
        }
        for (unsigned int i = 0; i < 3; i++){
            mPreINTDFE00511SUBOUT[i] = intdfe00511subout[i];
        }
        mPreINTDFE0051FIFOOUTA = intdfe0051fifoouta;
        mPreINTDFE0051FIFOOUTB = intdfe0051fifooutb;
    }
}//}}}

/// Merge INTDFE0051DOUT with INTDFE0051CND0
void Cdfe0051_wrapper::MergeINTDFE0051DOUTCNDMethod()
{//{{{
    for (unsigned int channel = 0; channel < DFE00510_NUM_CHANNEL; channel++){
        this->INTDFE00510DOUTCND[channel]->write(INTDFE00510DOUT_sig[channel].read() | INTDFE00510CND0_sig[channel].read());
    }
    for (unsigned int channel = 0; channel < DFE00511_NUM_CHANNEL; channel++){
        this->INTDFE00511DOUTCND[channel]->write(INTDFE00511DOUT_sig[channel].read() | INTDFE00511CND0_sig[channel].read());
    }
}//}}}

/// Merge INTDFE0051FIFOERR with INTDFE00510ERR
void Cdfe0051_wrapper::MergeINTDFE00510ERRMethod()
{//{{{
    INTDFE00510ERR.write(INTDFE00510ERR_sig.read() | INTDFE0051FIFOERR_sig.read());
}//}}}

/// Check valid of connection between input source and channel number
bool Cdfe0051_wrapper::CheckChannelConnectMatrix(std::string dfe0051_name, unsigned int source, unsigned int dest_channel)
{//{{{
    sc_assert((dfe0051_name == "DFE00510") || (dfe0051_name == "DFE00511"));
    unsigned int channel_index = dest_channel;
    if (dfe0051_name == "DFE00511"){
        channel_index = dest_channel + DFE00510_NUM_CHANNEL; 
    }
    return mChannelConnectMatrix[source][channel_index];
}//}}}

/// Assert reset by software
void Cdfe0051_wrapper::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    mDFE00510->AssertReset (reset_name, start_time, period);
    mDFE00511->AssertReset (reset_name, start_time, period);
    mFIFO->AssertReset (reset_name, start_time, period);
}//}}}

/// Set message level (fatal, error, warning, info)
void Cdfe0051_wrapper::SetMessageLevel (const std::string msg_lv)
{//{{{
    mFIFO->SetMessageLevel(msg_lv);

    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }

    std::string msg_ret = mDFE00510->handleCommand(cmd);
    if (msg_ret != "") {
        SC_REPORT_INFO(mDFE00510->basename(), msg_ret.c_str());
    }

    msg_ret = mDFE00511->handleCommand(cmd);
    if (msg_ret != "") {
        SC_REPORT_INFO(mDFE00511->basename(), msg_ret.c_str());
    }

    //Set message level for Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    mDFE00510->mDFE0051Func->RegisterHandler(cmd);
    mDFE00511->mDFE0051Func->RegisterHandler(cmd);


    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    mDFE00510->mDFE0051Func->RegisterHandler(cmd);
    mDFE00511->mDFE0051Func->RegisterHandler(cmd);
}//}}}

/// Set clock frequency by software
void Cdfe0051_wrapper::SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit)
{//{{{
    mDFE00510->SetCLKFreq (clock_name, freq, unit);
    mDFE00511->SetCLKFreq (clock_name, freq, unit);
    mFIFO->SetCLKFreq (clock_name, freq, unit);
}//}}}

/// Set clock frequency by software
void Cdfe0051_wrapper::GetCLKFreq (const std::string clock_name)
{//{{{
    mDFE00510->GetCLKFreq (clock_name);
    mDFE00511->GetCLKFreq (clock_name);
    mFIFO->GetCLKFreq (clock_name);
}//}}}

/// Force value to register
void Cdfe0051_wrapper::ForceRegister (const std::string unit_name, const std::string reg_name, const unsigned int reg_value)
{//{{{
    if (unit_name == "DFE00510"){
        mDFE00510->ForceRegister(reg_name, reg_value);
    }else if (unit_name == "DFE00511"){
        mDFE00511->ForceRegister(reg_name, reg_value);
    }else if (unit_name == "FIFO"){
        mFIFO->ForceRegister(reg_name, reg_value);
    }else{
        re_printf("warning","Unit name (%s) is invalid. It must be DFE00510, DFE00511, or FIFO.\n", reg_name.c_str());
    }
}//}}}

/// Release forced value after forcing registers
void Cdfe0051_wrapper::ReleaseRegister (const std::string unit_name, const std::string reg_name)
{//{{{
    if (unit_name == "DFE00510"){
        mDFE00510->ReleaseRegister(reg_name);
    }else if (unit_name == "DFE00511"){
        mDFE00511->ReleaseRegister(reg_name);
    }else if (unit_name == "FIFO"){
        mFIFO->ReleaseRegister(reg_name);
    }else{
        re_printf("warning","Unit name (%s) is invalid. It must be DFE00510, DFE00511, or FIFO.\n", reg_name.c_str());
    }
}//}}}

/// Write value to registers by software
void Cdfe0051_wrapper::WriteRegister (const std::string unit_name, const std::string reg_name, const unsigned int reg_value)
{//{{{
    if (unit_name == "DFE00510"){
        mDFE00510->WriteRegister(reg_name, reg_value);
    }else if (unit_name == "DFE00511"){
        mDFE00511->WriteRegister(reg_name, reg_value);
    }else if (unit_name == "FIFO"){
        mFIFO->WriteRegister(reg_name, reg_value);
    }else{
        re_printf("warning","Unit name (%s) is invalid. It must be DFE00510, DFE00511, or FIFO.\n", reg_name.c_str());
    }
}//}}}

/// Read value of register by software
void Cdfe0051_wrapper::ReadRegister (const std::string unit_name, const std::string reg_name)
{//{{{
    if (unit_name == "DFE00510"){
        mDFE00510->ReadRegister(reg_name);
    }else if (unit_name == "DFE00511"){
        mDFE00511->ReadRegister(reg_name);
    }else if (unit_name == "FIFO"){
        mFIFO->ReadRegister(reg_name);
    }else{
        re_printf("warning","Unit name (%s) is invalid. It must be DFE00510, DFE00511, or FIFO.\n", unit_name.c_str());
    }
}//}}}

/// List all registers name
void Cdfe0051_wrapper::ListRegister (void)
{//{{{
    mDFE00510->ListRegister();
    mDFE00511->ListRegister();
    mFIFO->ListRegister();
}//}}}

/// Write value to Coefficient memory by software
void Cdfe0051_wrapper::WriteCoefficient (const std::string unit_name, const unsigned int offset, const unsigned int value)
{//{{{ 
    if (unit_name == "DFE00510"){
        mDFE00510->WriteCoefficient(offset, value);
    }else if (unit_name == "DFE00511"){
        mDFE00511->WriteCoefficient(offset, value);
    }else{
        re_printf("warning","Unit name (%s) is invalid. It must be DFE00510 or DFE00511.\n", unit_name.c_str());
    }
}//}}}

/// Read value of Coefficient memory by software
void Cdfe0051_wrapper::ReadCoefficient (const std::string unit_name, const unsigned int offset)
{//{{{
    if (unit_name == "DFE00510"){
        mDFE00510->ReadCoefficient(offset);
    }else if (unit_name == "DFE00511"){
        mDFE00511->ReadCoefficient(offset);
    }else{
        re_printf("warning","Unit name (%s) is invalid. It must be DFE00510 or DFE00511.\n", unit_name.c_str());
    }
}//}}} 

/// Write value to DataMem memory by software
void Cdfe0051_wrapper::WriteDataMem (const std::string unit_name, const unsigned int offset, const unsigned int value)
{//{{{
    if (unit_name == "DFE00510"){
        mDFE00510->WriteDataMem(offset, value);
    }else if (unit_name == "DFE00511"){
        mDFE00511->WriteDataMem(offset, value);
    }else{
        re_printf("warning","Unit name (%s) is invalid. It must be DFE00510 or DFE00511.\n", unit_name.c_str());
    }
}//}}}

/// Read value of DataMem memory by software
void Cdfe0051_wrapper::ReadDataMem (const std::string unit_name, const unsigned int offset)
{//{{{
    if (unit_name == "DFE00510"){
        mDFE00510->ReadDataMem(offset);
    }else if (unit_name == "DFE00511"){
        mDFE00511->ReadDataMem(offset);
    }else{
        re_printf("warning","Unit name (%s) is invalid. It must be DFE00510 or DFE00511.\n", unit_name.c_str());
    }
}//}}}

/// Dump help message of all parameters or commands
void Cdfe0051_wrapper::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"DFE0051_MessageLevel (fatal|error|warning|info)         Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"DFE0051_DumpRegisterRW (true/false)                     Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"DFE0051_DumpInterrupt (true/false)                      Enable/disable dumping interrupt information (Default: false)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"DFE0051_AssertReset (reset_name, start_time, period)    Assert and de-assert reset signal to the DFE0051 model");
            SC_REPORT_INFO(this->basename(),"DFE0051_SetCLKFreq (clock_name, freq, unit)             Set clock frequency value to DFE0051 model. Valid clock_name is CLK_UHSB and CLK_HSB");
            SC_REPORT_INFO(this->basename(),"DFE0051_GetCLKFreq (clock_name)                         Get clock frequency value of DFE0051 model. Valid clock_name is CLK_UHSB and CLK_HSB");
            SC_REPORT_INFO(this->basename(),"DFE0051_ForceRegister (unit_name, reg_name, reg_value)  Force register with setting value");
            SC_REPORT_INFO(this->basename(),"DFE0051_ReleaseRegister (unit_name, reg_name)           Release register from force value");
            SC_REPORT_INFO(this->basename(),"DFE0051_WriteRegister (unit_name, reg_name, reg_value)  Write a value to a register");
            SC_REPORT_INFO(this->basename(),"DFE0051_ReadRegister (unit_name, reg_name)              Read a value from a register");
            SC_REPORT_INFO(this->basename(),"DFE0051_ListRegister ()                                 Dump name of all registers");
            SC_REPORT_INFO(this->basename(),"DFE0051_WriteCoefficient (unit_name, address, value)    Write a 4-byte value to an offset in Coefficient memory");
            SC_REPORT_INFO(this->basename(),"DFE0051_ReadCoefficient (unit_name, address)            Get a 4-byte value from an offset in Coefficient memory");
            SC_REPORT_INFO(this->basename(),"DFE0051_WriteDataMem (unit_name, address, value)        Write a 4-byte value to an offset in Data memory");
            SC_REPORT_INFO(this->basename(),"DFE0051_ReadDataMem (unit_name, address)                Get a 4-byte value from an offset in Data memory");
        } else {
            re_printf("warning","The name (%s) of DFE0051_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of DFE0051_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

/// Enable/disable dumping message when users access registers
void Cdfe0051_wrapper::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mDFE00510->mDFE0051Func->RegisterHandler(cmd);
    mDFE00511->mDFE0051Func->RegisterHandler(cmd);
    mFIFO->DumpRegisterRW(is_enable);
}//}}}

/// Enable/disable dumping interrupt information
void Cdfe0051_wrapper::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: DFE0051_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = (mDumpInterrupt) ? "true" : "false";
        re_printf("info","DFE0051_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: DFE0051_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

/// Separate some words from a string to store a vector
void Cdfe0051_wrapper::SeparateString (std::vector<std::string> &vtr, const std::string msg)
{//{{{
    std::istringstream cmd;
    cmd.str(msg);
    std::string temp = "a"; //Make temp to be not empty
    while (temp != "") {
        std::string command;
        cmd >> command;
        if (command != "") {
            vtr.push_back(command);
        }
        temp = command;
    }
}//}}}

// vim600: set foldmethod=marker :
