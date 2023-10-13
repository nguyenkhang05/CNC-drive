// ---------------------------------------------------------------------
// $Id: DFE_Wrapper.cpp,v 1.2 2018/09/06 10:17:09 huepham Exp $
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

#include "DFE_Wrapper.h"

/// Constructor
Cdfe_wrapper::Cdfe_wrapper(sc_module_name name):
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

        ,CADC0_REQ("CADC0_REQ")
        ,CADC0_TAG("CADC0_TAG")
        ,CADC0_DATA("CADC0_DATA")

        ,TMFIFOCAPTRG("TMFIFOCAPTRG")
        ,INTDFE0ERR("INTDFE0ERR")
        ,INTDFE1ERR("INTDFE1ERR")
        ,INTDFEFIFOOUTA("INTDFEFIFOOUTA")
        ,INTDFEFIFOOUTB("INTDFEFIFOOUTB")
{//{{{
    // Initialize
    CommandInit(this->name());
    CreateChannelConnectMatrix();
    mDumpInterrupt = false;
    mPreINTDFE0ERR = false;
    mPreINTDFE1ERR = false;
    for (unsigned int i = 0; i < DFE0_NUM_CHANNEL; i++){
        mPreINTDFE0DOUTCND[i] = false;
    }
    for (unsigned int i = 0; i < DFE1_NUM_CHANNEL; i++){
        mPreINTDFE1DOUTCND[i] = false;
    }
    for (unsigned int i = 0; i < 4; i++){
        mPreINTDFE0CND[i] = false;
    }
    for (unsigned int i = 0; i < 4; i++){
        mPreINTDFE1CND[i] = false;
    }
    for (unsigned int i = 0; i < 3; i++){
        mPreINTDFE0SUBOUT[i] = false;
    }
    for (unsigned int i = 0; i < 3; i++){
        mPreINTDFE1SUBOUT[i] = false;
    }
    mPreINTDFEFIFOOUTA = false;
    mPreINTDFEFIFOOUTB = false;

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
    for (unsigned int index = 0; index < DFE0_NUM_CHANNEL; index++){
        std::ostringstream str_INTDFE0DOUTCND;
        str_INTDFE0DOUTCND<<"INTDFE0DOUTCND"<<index;
        INTDFE0DOUTCND[index] = new sc_out<bool>(str_INTDFE0DOUTCND.str().c_str());
        INTDFE0DOUTCND[index]->initialize(false);
    }
    for (unsigned int index = 0; index < DFE1_NUM_CHANNEL; index++){
        std::ostringstream str_INTDFE1DOUTCND;
        str_INTDFE1DOUTCND<<"INTDFE1DOUTCND"<<index;
        INTDFE1DOUTCND[index] = new sc_out<bool>(str_INTDFE1DOUTCND.str().c_str());
        INTDFE1DOUTCND[index]->initialize(false);
    }
    for (unsigned int index = 0; index < 4; index++){
        std::ostringstream str_INTDFE0CND;
        str_INTDFE0CND<<"INTDFE0CND"<<index;
        INTDFE0CND[index] = new sc_out<bool>(str_INTDFE0CND.str().c_str());
        INTDFE0CND[index]->initialize(false);

        std::ostringstream str_DFE0_PHUPD;
        str_DFE0_PHUPD<<"DFE0_PHUPD"<<index;
        DFE0_PHUPD[index] = new sc_out<bool>(str_DFE0_PHUPD.str().c_str());
        DFE0_PHUPD[index]->initialize(false);
    }
    for (unsigned int index = 0; index < 4; index++){
        std::ostringstream str_INTDFE1CND;
        str_INTDFE1CND<<"INTDFE1CND"<<index;
        INTDFE1CND[index] = new sc_out<bool>(str_INTDFE1CND.str().c_str());
        INTDFE1CND[index]->initialize(false);

        std::ostringstream str_DFE1_PHUPD;
        str_DFE1_PHUPD<<"DFE1_PHUPD"<<index;
        DFE1_PHUPD[index] = new sc_out<bool>(str_DFE1_PHUPD.str().c_str());
        DFE1_PHUPD[index]->initialize(false);
    }
    for (unsigned int index = 0; index < 3; index++){
        std::ostringstream str_INTDFE0SUBOUT;
        str_INTDFE0SUBOUT<<"INTDFE0SUBOUT"<<index;
        INTDFE0SUBOUT[index] = new sc_out<bool>(str_INTDFE0SUBOUT.str().c_str());
        INTDFE0SUBOUT[index]->initialize(false);
    }
    for (unsigned int index = 0; index < 3; index++){
        std::ostringstream str_INTDFE1SUBOUT;
        str_INTDFE1SUBOUT<<"INTDFE1SUBOUT"<<index;
        INTDFE1SUBOUT[index] = new sc_out<bool>(str_INTDFE1SUBOUT.str().c_str());
        INTDFE1SUBOUT[index]->initialize(false);
    }
    // Instances
    mDFE0 = new Cdfe<DFE0_NUM_CHANNEL>("DFE0", this);
    mDFE1 = new Cdfe<DFE1_NUM_CHANNEL>("DFE1", this);
    mFIFO = new DFE_FIFO("DFE_FIFO");
    sc_assert((mDFE0 != NULL) && (mDFE1 != NULL) && (mFIFO != NULL));

    // Connect port DFE0
    mDFE0->CLK_HSB(CLK_HSB);
    mDFE0->CLK_UHSB(CLK_UHSB);
    mDFE0->PRESETn(PRESETn);
    // SAR-ADC input
    mDFE0->SARADC0_REQ(SARADC0_REQ);
    mDFE0->SARADC0_TAG(SARADC0_TAG);
    mDFE0->SARADC0_DATA(SARADC0_DATA);
    mDFE0->SARADC1_REQ(SARADC1_REQ);
    mDFE0->SARADC1_TAG(SARADC1_TAG);
    mDFE0->SARADC1_DATA(SARADC1_DATA);
    mDFE0->SARADC2_REQ(SARADC2_REQ);
    mDFE0->SARADC2_TAG(SARADC2_TAG);
    mDFE0->SARADC2_DATA(SARADC2_DATA);
    mDFE0->SARADC3_REQ(SARADC3_REQ);
    mDFE0->SARADC3_TAG(SARADC3_TAG);
    mDFE0->SARADC3_DATA(SARADC3_DATA);
    // DS-ADC input
    mDFE0->DSADC00_REQ(DSADC00_REQ);
    mDFE0->DSADC00_TAG(DSADC00_TAG);
    mDFE0->DSADC00_DATA(DSADC00_DATA);
    mDFE0->DSADC10_REQ(DSADC10_REQ);
    mDFE0->DSADC10_TAG(DSADC10_TAG);
    mDFE0->DSADC10_DATA(DSADC10_DATA);
    mDFE0->DSADC20_REQ(DSADC20_REQ);
    mDFE0->DSADC20_TAG(DSADC20_TAG);
    mDFE0->DSADC20_DATA(DSADC20_DATA);
    mDFE0->DSADC11_REQ(DSADC11_REQ);
    mDFE0->DSADC11_TAG(DSADC11_TAG);
    mDFE0->DSADC11_DATA(DSADC11_DATA);
    mDFE0->DSADC12_REQ(DSADC12_REQ);
    mDFE0->DSADC12_TAG(DSADC12_TAG);
    mDFE0->DSADC12_DATA(DSADC12_DATA);
    mDFE0->DSADC13_REQ(DSADC13_REQ);
    mDFE0->DSADC13_TAG(DSADC13_TAG);
    mDFE0->DSADC13_DATA(DSADC13_DATA);
    // Cyclic-ADC input
    mDFE0->CADC0_REQ(CADC0_REQ);
    mDFE0->CADC0_TAG(CADC0_TAG);
    mDFE0->CADC0_DATA(CADC0_DATA);
    // Error
    mDFE0->INTDFEERR(INTDFE0ERR_sig);

    // Connect ports of DFE1
    mDFE1->CLK_HSB(CLK_HSB);
    mDFE1->CLK_UHSB(CLK_UHSB);
    mDFE1->PRESETn(PRESETn);
    // SAR-ADC input
    mDFE1->SARADC0_REQ(SARADC0_REQ);
    mDFE1->SARADC0_TAG(SARADC0_TAG);
    mDFE1->SARADC0_DATA(SARADC0_DATA);
    mDFE1->SARADC1_REQ(SARADC1_REQ);
    mDFE1->SARADC1_TAG(SARADC1_TAG);
    mDFE1->SARADC1_DATA(SARADC1_DATA);
    mDFE1->SARADC2_REQ(SARADC2_REQ);
    mDFE1->SARADC2_TAG(SARADC2_TAG);
    mDFE1->SARADC2_DATA(SARADC2_DATA);
    mDFE1->SARADC3_REQ(SARADC3_REQ);
    mDFE1->SARADC3_TAG(SARADC3_TAG);
    mDFE1->SARADC3_DATA(SARADC3_DATA);
    // DS-ADC input
    mDFE1->DSADC00_REQ(DSADC00_REQ);
    mDFE1->DSADC00_TAG(DSADC00_TAG);
    mDFE1->DSADC00_DATA(DSADC00_DATA);
    mDFE1->DSADC10_REQ(DSADC10_REQ);
    mDFE1->DSADC10_TAG(DSADC10_TAG);
    mDFE1->DSADC10_DATA(DSADC10_DATA);
    mDFE1->DSADC20_REQ(DSADC20_REQ);
    mDFE1->DSADC20_TAG(DSADC20_TAG);
    mDFE1->DSADC20_DATA(DSADC20_DATA);
    mDFE1->DSADC11_REQ(DSADC11_REQ);
    mDFE1->DSADC11_TAG(DSADC11_TAG);
    mDFE1->DSADC11_DATA(DSADC11_DATA);
    mDFE1->DSADC12_REQ(DSADC12_REQ);
    mDFE1->DSADC12_TAG(DSADC12_TAG);
    mDFE1->DSADC12_DATA(DSADC12_DATA);
    mDFE1->DSADC13_REQ(DSADC13_REQ);
    mDFE1->DSADC13_TAG(DSADC13_TAG);
    mDFE1->DSADC13_DATA(DSADC13_DATA);
    // Cyclic-ADC input
    mDFE1->CADC0_REQ(CADC0_REQ);
    mDFE1->CADC0_TAG(CADC0_TAG);
    mDFE1->CADC0_DATA(CADC0_DATA);
    // Error
    mDFE1->INTDFEERR(INTDFE1ERR);
    for (unsigned int index = 0; index < 20; index++){
        (mDFE0->TMTRG_A[index])->bind(*TMTRG_A[index]);
        (mDFE0->TMTRG_B[index])->bind(*TMTRG_B[index]);
        (mDFE1->TMTRG_A[index])->bind(*TMTRG_A[index]);
        (mDFE1->TMTRG_B[index])->bind(*TMTRG_B[index]);
    }
    for (unsigned int index = 0; index < 3; index++){
        (mDFE0->TMCAPTRG[index])->bind(*TMCAPTRG[index]);
        (mDFE1->TMCAPTRG[index])->bind(*TMCAPTRG[index]);
    }
    for (unsigned int index = 0; index < DFE0_NUM_CHANNEL; index++){
        (mDFE0->DFE_FIFO_CACK[index])->bind(DFE0_FIFO_CACK_sig[index]);
        (mDFE0->DFE_DOUPD_N[index])->bind(DFE0_DOUPD_N_sig[index]);
        (mDFE0->DFE_DOUT_END[index])->bind(DFE0_DOUT_END_sig[index]);
        (mDFE0->DFE_DFDOUT[index])->bind(DFE0_DFDOUT_sig[index]);

        (mDFE0->INTDFECND0[index])->bind(INTDFE0CND0_sig[index]);
        (mDFE0->INTDFEDOUT[index])->bind(INTDFE0DOUT_sig[index]);
    }
    for (unsigned int index = 0; index < DFE1_NUM_CHANNEL; index++){
        (mDFE1->DFE_FIFO_CACK[index])->bind(DFE1_FIFO_CACK_sig[index]);
        (mDFE1->DFE_DOUPD_N[index])->bind(DFE1_DOUPD_N_sig[index]);
        (mDFE1->DFE_DOUT_END[index])->bind(DFE1_DOUT_END_sig[index]);
        (mDFE1->DFE_DFDOUT[index])->bind(DFE1_DFDOUT_sig[index]);

        (mDFE1->INTDFECND0[index])->bind(INTDFE1CND0_sig[index]);
        (mDFE1->INTDFEDOUT[index])->bind(INTDFE1DOUT_sig[index]);
    }
    for (unsigned int index = 0; index < 4; index++){
        (mDFE0->INTDFECND1[index])->bind(*(this->INTDFE0CND[index]));
        (mDFE1->INTDFECND1[index])->bind(*(this->INTDFE1CND[index]));
        (mDFE0->DFE_PHUPD[index])->bind(*(this->DFE0_PHUPD[index]));
        (mDFE1->DFE_PHUPD[index])->bind(*(this->DFE1_PHUPD[index]));
    }
    for (unsigned int index = 0; index < 3; index++){
        (mDFE0->INTDFESUBOUT[index])->bind(*(this->INTDFE0SUBOUT[index]));
        (mDFE1->INTDFESUBOUT[index])->bind(*(this->INTDFE1SUBOUT[index]));
    }

    // Bind clock, reset signal and socket to bus
    setSlaveFreqPort32(&CLK_HSB, &CLK_HSB, &CLK_HSB);
    setSlaveResetPort32(&PRESETn, &PRESETn, &PRESETn);
    setTargetSocket32("tsdfe0", "tsdfe1", "tsfifo");
    tsdfe0 = BusSlaveBase<32,3>::tSocket32[0];
    tsdfe1 = BusSlaveBase<32,3>::tSocket32[1];
    tsfifo = BusSlaveBase<32,3>::tSocket32[2];
    

    mBusSlaveIf32[0]->setFuncModulePtr(mDFE0->mDFEFunc);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setTransNmbLmt(1);
    mBusSlaveIf32[1]->setFuncModulePtr(mDFE1->mDFEFunc);
    mBusSlaveIf32[1]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[1]->setTransNmbLmt(1);
    mBusSlaveIf32[2]->setFuncModulePtr(mFIFO->getBusSlaveFunc());
    mBusSlaveIf32[2]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[2]->setTransNmbLmt(1);


    mFIFO->pclk   .bind(CLK_HSB);
    mFIFO->presetn.bind(PRESETn);
    mFIFO->dfe_tim_ctrig.bind(TMFIFOCAPTRG);
    for (unsigned int cid = 0; cid < DFE0_NUM_CHANNEL; cid++){
        mFIFO->dfe_do_ctrig  [0][cid]->bind(DFE0_DOUT_END_sig [cid]);
        mFIFO->dfe_do        [0][cid]->bind(DFE0_DFDOUT_sig   [cid]);
        mFIFO->dfe_do_upd    [0][cid]->bind(DFE0_DOUPD_N_sig  [cid]);
        mFIFO->dfe_dfifo_cack[0][cid]->bind(DFE0_FIFO_CACK_sig[cid]);
    }
    for (unsigned int cid = 0; cid < DFE1_NUM_CHANNEL; cid++){
        mFIFO->dfe_do_ctrig  [1][cid]->bind(DFE1_DOUT_END_sig [cid]);
        mFIFO->dfe_do        [1][cid]->bind(DFE1_DFDOUT_sig   [cid]);
        mFIFO->dfe_do_upd    [1][cid]->bind(DFE1_DOUPD_N_sig  [cid]);
        mFIFO->dfe_dfifo_cack[1][cid]->bind(DFE1_FIFO_CACK_sig[cid]);
    }
    mFIFO->dfe_ba_ce_irq.bind(INTDFEFIFOOUTA);
    mFIFO->dfe_bb_ce_irq.bind(INTDFEFIFOOUTB); 
    mFIFO->dfe_berr_irq .bind(INTDFEFIFOERR_sig); 
    
    SC_METHOD(DumpInterruptMethod);
    dont_initialize();
    for (unsigned int i = 0; i <DFE0_NUM_CHANNEL; i++){
        sensitive << (*INTDFE0DOUTCND[i]);
    }
    for (unsigned int i = 0; i <DFE1_NUM_CHANNEL; i++){
        sensitive << (*INTDFE1DOUTCND[i]);
    }
    for (unsigned int i = 0; i <4; i++){
        sensitive << (*INTDFE0CND[i]);
        sensitive << (*INTDFE1CND[i]);
    }
    for (unsigned int i = 0; i <3; i++){
        sensitive << (*INTDFE0SUBOUT[i]);
        sensitive << (*INTDFE1SUBOUT[i]);
    }
    sensitive << INTDFE0ERR;
    sensitive << INTDFE1ERR;
    sensitive << INTDFEFIFOOUTA;
    sensitive << INTDFEFIFOOUTB;

    SC_METHOD(MergeINTDFEDOUTCNDMethod);
    dont_initialize();
    for (unsigned int i = 0; i <DFE0_NUM_CHANNEL; i++){
        sensitive << (INTDFE0CND0_sig[i]);
        sensitive << (INTDFE0DOUT_sig[i]);
    }
    for (unsigned int i = 0; i <DFE1_NUM_CHANNEL; i++){
        sensitive << (INTDFE1CND0_sig[i]);
        sensitive << (INTDFE1DOUT_sig[i]);
    }

    SC_METHOD(MergeINTDFE0ERRMethod);
    dont_initialize();
    sensitive << INTDFE0ERR_sig;
    sensitive << INTDFEFIFOERR_sig;
}//}}}

/// Destructor
Cdfe_wrapper::~Cdfe_wrapper(void)
{//{{{
    delete mDFE0;
    delete mDFE1;
    delete mFIFO;
}//}}}

/// Create valid channel connection matrix (with input source)
void Cdfe_wrapper::CreateChannelConnectMatrix()
{//{{{
    unsigned int total_channel = DFE1_NUM_CHANNEL + DFE0_NUM_CHANNEL;
    // Table of valid assignment (DFE0: CH0-11, DFE1: CH12-15),
    // This table should be changed if the table "data can be input channel" changes.
    //                       CH0    CH1    CH2    CH3    CH4    CH5    CH6    CH7    CH8    CH9    CH10   CH11   CH12   CH13   CH14  CH15  
    bool matrix[16][16] =  {{true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true },
                            {true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,false ,false ,false ,false},
                            {true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,false ,false ,false ,false},
                            {true  ,true  ,true  ,true  ,true  ,true  ,false ,false ,false ,false ,false ,false ,false ,false ,false ,false},
                            {false ,false ,false ,false ,false ,false ,true  ,true  ,true  ,true  ,true  ,true  ,false ,false ,false ,false},
                            {true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true },
                            {true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true },
                            {true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true },
                            {true  ,false ,false ,false ,false ,true  ,false ,false ,false ,false ,true  ,false ,true  ,false ,false ,false},
                            {false ,true  ,false ,false ,false ,false ,true  ,false ,false ,false ,false ,true  ,false ,false ,false ,false},
                            {false ,false ,true  ,false ,false ,false ,false ,true  ,false ,false ,false ,false ,false ,false ,false ,false},
                            {false ,false ,false ,false ,false ,false ,false ,false ,false ,false ,false ,false ,true  ,true  ,true  ,true },
                            {true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true },
                            {true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true },
                            {true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true },
                            {true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true  ,true }};
    for (unsigned int i = 0; i < 16; i++){// Scan all 16 input source
        for (unsigned int j = 0; j<total_channel;j++){
            mChannelConnectMatrix[i][j] = matrix[i][j];
        }
    }
}//}}}

/// Dump interrupt info
void Cdfe_wrapper::DumpInterruptMethod()
{//{{{
    if (mDumpInterrupt){
        bool intdfe0err = INTDFE0ERR.read();
        bool intdfe1err = INTDFE1ERR.read();
        bool intdfe0doutcnd[DFE0_NUM_CHANNEL];
        for (unsigned int i = 0; i < DFE0_NUM_CHANNEL; i++){
            intdfe0doutcnd[i] = INTDFE0DOUTCND[i]->read();
        }
        bool intdfe1doutcnd[DFE1_NUM_CHANNEL];
        for (unsigned int i = 0; i < DFE1_NUM_CHANNEL; i++){
            intdfe1doutcnd[i] = INTDFE1DOUTCND[i]->read();
        }
        bool intdfe0cnd[4];
        for (unsigned int i = 0; i < 4; i++){
            intdfe0cnd[i] = INTDFE0CND[i]->read();
        }
        bool intdfe1cnd[4];
        for (unsigned int i = 0; i < 4; i++){
            intdfe1cnd[i] = INTDFE1CND[i]->read();
        }
        bool intdfe0subout[3];
        for (unsigned int i = 0; i < 3; i++){
            intdfe0subout[i] = INTDFE0SUBOUT[i]->read();
        }
        bool intdfe1subout[3];
        for (unsigned int i = 0; i < 3; i++){
            intdfe1subout[i] = INTDFE1SUBOUT[i]->read();
        }
        bool intdfefifoouta = INTDFEFIFOOUTA.read();
        bool intdfefifooutb = INTDFEFIFOOUTB.read();

        if (intdfe0err && !mPreINTDFE0ERR){
            re_printf("info", "INT [INTDFE0ERR] assert.\n");
        }
        if (!intdfe0err && mPreINTDFE0ERR){
            re_printf("info", "INT [INTDFE0ERR] de-assert.\n");
        }
        if (intdfe1err && !mPreINTDFE1ERR){
            re_printf("info", "INT [INTDFE1ERR] assert.\n");
        }
        if (!intdfe1err && mPreINTDFE1ERR){
            re_printf("info", "INT [INTDFE1ERR] de-assert.\n");
        }
        for (unsigned int i = 0; i < DFE0_NUM_CHANNEL; i++){
            if (intdfe0doutcnd[i] && !mPreINTDFE0DOUTCND[i]){
                re_printf("info", "INT [INTDFE0DOUTCND%d] assert.\n", i);
            }
            if (!intdfe0doutcnd[i] && mPreINTDFE0DOUTCND[i]){
                re_printf("info", "INT [INTDFE0DOUTCND%d] de-assert.\n", i);
            }
        }
        for (unsigned int i = 0; i < DFE1_NUM_CHANNEL; i++){
            if (intdfe1doutcnd[i] && !mPreINTDFE1DOUTCND[i]){
                re_printf("info", "INT [INTDFE1DOUTCND%d] assert.\n", i);
            }
            if (!intdfe1doutcnd[i] && mPreINTDFE1DOUTCND[i]){
                re_printf("info", "INT [INTDFE1DOUTCND%d] de-assert.\n", i);
            }
        }
        for (unsigned int i = 0; i < 4; i++){
            if (intdfe0cnd[i] && !mPreINTDFE0CND[i]){
                re_printf("info", "INT [INTDFE0CND%d] assert.\n", i);
            }
            if (!intdfe0cnd[i] && mPreINTDFE0CND[i]){
                re_printf("info", "INT [INTDFE0CND%d] de-assert.\n", i);
            }
        }
        for (unsigned int i = 0; i < 4; i++){
            if (intdfe1cnd[i] && !mPreINTDFE1CND[i]){
                re_printf("info", "INT [INTDFE1CND%d] assert.\n", i);
            }
            if (!intdfe1cnd[i] && mPreINTDFE1CND[i]){
                re_printf("info", "INT [INTDFE1CND%d] de-assert.\n", i);
            }
        }
        for (unsigned int i = 0; i < 3; i++){
            if (intdfe0subout[i] && !mPreINTDFE0SUBOUT[i]){
                re_printf("info", "INT [INTDFE0SUBOUT%d] assert.\n", i);
            }
            if (!intdfe0subout[i] && mPreINTDFE0SUBOUT[i]){
                re_printf("info", "INT [INTDFE0SUBOUT%d] de-assert.\n", i);
            }
        }
        for (unsigned int i = 0; i < 3; i++){
            if (intdfe1subout[i] && !mPreINTDFE1SUBOUT[i]){
                re_printf("info", "INT [INTDFE1SUBOUT%d] assert.\n", i);
            }
            if (!intdfe1subout[i] && mPreINTDFE1SUBOUT[i]){
                re_printf("info", "INT [INTDFE1SUBOUT%d] de-assert.\n", i);
            }
        }
        if (intdfefifoouta && !mPreINTDFEFIFOOUTA){
            re_printf("info", "INT [INTDFEFIFOOUTA] assert.\n");
        }
        if (!intdfefifoouta && mPreINTDFEFIFOOUTA){
            re_printf("info", "INT [INTDFEFIFOOUTA] de-assert.\n");
        }
        if (intdfefifooutb && !mPreINTDFEFIFOOUTB){
            re_printf("info", "INT [INTDFEFIFOOUTB] assert.\n");
        }
        if (!intdfefifooutb && mPreINTDFEFIFOOUTB){
            re_printf("info", "INT [INTDFEFIFOOUTB] de-assert.\n");
        }

        // Update pre-value
        mPreINTDFE0ERR = intdfe0err;
        mPreINTDFE1ERR = intdfe1err;
        for (unsigned int i = 0; i < DFE0_NUM_CHANNEL; i++){
            mPreINTDFE0DOUTCND[i] = intdfe0doutcnd[i];
        }
        for (unsigned int i = 0; i < DFE1_NUM_CHANNEL; i++){
            mPreINTDFE1DOUTCND[i] = intdfe1doutcnd[i];
        }
        for (unsigned int i = 0; i < 4; i++){
            mPreINTDFE0CND[i] = intdfe0cnd[i];
        }
        for (unsigned int i = 0; i < 4; i++){
            mPreINTDFE1CND[i] = intdfe1cnd[i];
        }
        for (unsigned int i = 0; i < 3; i++){
            mPreINTDFE0SUBOUT[i] = intdfe0subout[i];
        }
        for (unsigned int i = 0; i < 3; i++){
            mPreINTDFE1SUBOUT[i] = intdfe1subout[i];
        }
        mPreINTDFEFIFOOUTA = intdfefifoouta;
        mPreINTDFEFIFOOUTB = intdfefifooutb;
    }
}//}}}

/// Merge INTDFEDOUT with INTDFECND0
void Cdfe_wrapper::MergeINTDFEDOUTCNDMethod()
{//{{{
    for (unsigned int channel = 0; channel < DFE0_NUM_CHANNEL; channel++){
        this->INTDFE0DOUTCND[channel]->write(INTDFE0DOUT_sig[channel].read() | INTDFE0CND0_sig[channel].read());
    }
    for (unsigned int channel = 0; channel < DFE1_NUM_CHANNEL; channel++){
        this->INTDFE1DOUTCND[channel]->write(INTDFE1DOUT_sig[channel].read() | INTDFE1CND0_sig[channel].read());
    }
}//}}}

/// Merge INTDFEFIFOERR with INTDFE0ERR
void Cdfe_wrapper::MergeINTDFE0ERRMethod()
{//{{{
    INTDFE0ERR.write(INTDFE0ERR_sig.read() | INTDFEFIFOERR_sig.read());
}//}}}

/// Check valid of connection between input source and channel number
bool Cdfe_wrapper::CheckChannelConnectMatrix(std::string dfe_name, unsigned int source, unsigned int dest_channel)
{//{{{
    sc_assert((dfe_name == "DFE0") || (dfe_name == "DFE1"));
    unsigned int channel_index = dest_channel;
    if (dfe_name == "DFE1"){
        channel_index = dest_channel + DFE0_NUM_CHANNEL; 
    }
    return mChannelConnectMatrix[source][channel_index];
}//}}}

/// Assert reset by software
void Cdfe_wrapper::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    mDFE0->AssertReset (reset_name, start_time, period);
    mDFE1->AssertReset (reset_name, start_time, period);
    mFIFO->AssertReset (reset_name, start_time, period);
}//}}}

/// Set message level (fatal, error, warning, info)
void Cdfe_wrapper::SetMessageLevel (const std::string msg_lv)
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

    std::string msg_ret = mDFE0->handleCommand(cmd);
    if (msg_ret != "") {
        SC_REPORT_INFO(mDFE0->basename(), msg_ret.c_str());
    }

    msg_ret = mDFE1->handleCommand(cmd);
    if (msg_ret != "") {
        SC_REPORT_INFO(mDFE1->basename(), msg_ret.c_str());
    }

    //Set message level for Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    mDFE0->mDFEFunc->RegisterHandler(cmd);
    mDFE1->mDFEFunc->RegisterHandler(cmd);


    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    mDFE0->mDFEFunc->RegisterHandler(cmd);
    mDFE1->mDFEFunc->RegisterHandler(cmd);
}//}}}

/// Set clock frequency by software
void Cdfe_wrapper::SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit)
{//{{{
    mDFE0->SetCLKFreq (clock_name, freq, unit);
    mDFE1->SetCLKFreq (clock_name, freq, unit);
    mFIFO->SetCLKFreq (clock_name, freq, unit);
}//}}}

/// Set clock frequency by software
void Cdfe_wrapper::GetCLKFreq (const std::string clock_name)
{//{{{
    mDFE0->GetCLKFreq (clock_name);
    mDFE1->GetCLKFreq (clock_name);
    mFIFO->GetCLKFreq (clock_name);
}//}}}

/// Force value to register
void Cdfe_wrapper::ForceRegister (const std::string unit_name, const std::string reg_name, const unsigned int reg_value)
{//{{{
    if (unit_name == "DFE0"){
        mDFE0->ForceRegister(reg_name, reg_value);
    }else if (unit_name == "DFE1"){
        mDFE1->ForceRegister(reg_name, reg_value);
    }else if (unit_name == "FIFO"){
        mFIFO->ForceRegister(reg_name, reg_value);
    }else{
        re_printf("warning","Unit name (%s) is invalid. It must be DFE0, DFE1, or FIFO.\n", reg_name.c_str());
    }
}//}}}

/// Release forced value after forcing registers
void Cdfe_wrapper::ReleaseRegister (const std::string unit_name, const std::string reg_name)
{//{{{
    if (unit_name == "DFE0"){
        mDFE0->ReleaseRegister(reg_name);
    }else if (unit_name == "DFE1"){
        mDFE1->ReleaseRegister(reg_name);
    }else if (unit_name == "FIFO"){
        mFIFO->ReleaseRegister(reg_name);
    }else{
        re_printf("warning","Unit name (%s) is invalid. It must be DFE0, DFE1, or FIFO.\n", reg_name.c_str());
    }
}//}}}

/// Write value to registers by software
void Cdfe_wrapper::WriteRegister (const std::string unit_name, const std::string reg_name, const unsigned int reg_value)
{//{{{
    if (unit_name == "DFE0"){
        mDFE0->WriteRegister(reg_name, reg_value);
    }else if (unit_name == "DFE1"){
        mDFE1->WriteRegister(reg_name, reg_value);
    }else if (unit_name == "FIFO"){
        mFIFO->WriteRegister(reg_name, reg_value);
    }else{
        re_printf("warning","Unit name (%s) is invalid. It must be DFE0, DFE1, or FIFO.\n", reg_name.c_str());
    }
}//}}}

/// Read value of register by software
void Cdfe_wrapper::ReadRegister (const std::string unit_name, const std::string reg_name)
{//{{{
    if (unit_name == "DFE0"){
        mDFE0->ReadRegister(reg_name);
    }else if (unit_name == "DFE1"){
        mDFE1->ReadRegister(reg_name);
    }else if (unit_name == "FIFO"){
        mFIFO->ReadRegister(reg_name);
    }else{
        re_printf("warning","Unit name (%s) is invalid. It must be DFE0, DFE1, or FIFO.\n", unit_name.c_str());
    }
}//}}}

/// List all registers name
void Cdfe_wrapper::ListRegister (void)
{//{{{
    mDFE0->ListRegister();
    mDFE1->ListRegister();
    mFIFO->ListRegister();
}//}}}

/// Write value to Coefficient memory by software
void Cdfe_wrapper::WriteCoefficient (const std::string unit_name, const unsigned int offset, const unsigned int value)
{//{{{ 
    if (unit_name == "DFE0"){
        mDFE0->WriteCoefficient(offset, value);
    }else if (unit_name == "DFE1"){
        mDFE1->WriteCoefficient(offset, value);
    }else{
        re_printf("warning","Unit name (%s) is invalid. It must be DFE0 or DFE1.\n", unit_name.c_str());
    }
}//}}}

/// Read value of Coefficient memory by software
void Cdfe_wrapper::ReadCoefficient (const std::string unit_name, const unsigned int offset)
{//{{{
    if (unit_name == "DFE0"){
        mDFE0->ReadCoefficient(offset);
    }else if (unit_name == "DFE1"){
        mDFE1->ReadCoefficient(offset);
    }else{
        re_printf("warning","Unit name (%s) is invalid. It must be DFE0 or DFE1.\n", unit_name.c_str());
    }
}//}}} 

/// Write value to DataMem memory by software
void Cdfe_wrapper::WriteDataMem (const std::string unit_name, const unsigned int offset, const unsigned int value)
{//{{{
    if (unit_name == "DFE0"){
        mDFE0->WriteDataMem(offset, value);
    }else if (unit_name == "DFE1"){
        mDFE1->WriteDataMem(offset, value);
    }else{
        re_printf("warning","Unit name (%s) is invalid. It must be DFE0 or DFE1.\n", unit_name.c_str());
    }
}//}}}

/// Read value of DataMem memory by software
void Cdfe_wrapper::ReadDataMem (const std::string unit_name, const unsigned int offset)
{//{{{
    if (unit_name == "DFE0"){
        mDFE0->ReadDataMem(offset);
    }else if (unit_name == "DFE1"){
        mDFE1->ReadDataMem(offset);
    }else{
        re_printf("warning","Unit name (%s) is invalid. It must be DFE0 or DFE1.\n", unit_name.c_str());
    }
}//}}}

/// Dump help message of all parameters or commands
void Cdfe_wrapper::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"DFE_MessageLevel (fatal|error|warning|info)         Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"DFE_DumpRegisterRW (true/false)                     Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"DFE_DumpInterrupt (true/false)                      Enable/disable dumping interrupt information (Default: false)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"DFE_AssertReset (reset_name, start_time, period)    Assert and de-assert reset signal to the DFE model");
            SC_REPORT_INFO(this->basename(),"DFE_SetCLKFreq (clock_name, freq, unit)             Set clock frequency value to DFE model. Valid clock_name is CLK_UHSB and CLK_HSB");
            SC_REPORT_INFO(this->basename(),"DFE_GetCLKFreq (clock_name)                         Get clock frequency value of DFE model. Valid clock_name is CLK_UHSB and CLK_HSB");
            SC_REPORT_INFO(this->basename(),"DFE_ForceRegister (unit_name, reg_name, reg_value)  Force register with setting value");
            SC_REPORT_INFO(this->basename(),"DFE_ReleaseRegister (unit_name, reg_name)           Release register from force value");
            SC_REPORT_INFO(this->basename(),"DFE_WriteRegister (unit_name, reg_name, reg_value)  Write a value to a register");
            SC_REPORT_INFO(this->basename(),"DFE_ReadRegister (unit_name, reg_name)              Read a value from a register");
            SC_REPORT_INFO(this->basename(),"DFE_ListRegister ()                                 Dump name of all registers");
            SC_REPORT_INFO(this->basename(),"DFE_WriteCoefficient (unit_name, address, value)    Write a 4-byte value to an offset in Coefficient memory");
            SC_REPORT_INFO(this->basename(),"DFE_ReadCoefficient (unit_name, address)            Get a 4-byte value from an offset in Coefficient memory");
            SC_REPORT_INFO(this->basename(),"DFE_WriteDataMem (unit_name, address, value)        Write a 4-byte value to an offset in Data memory");
            SC_REPORT_INFO(this->basename(),"DFE_ReadDataMem (unit_name, address)                Get a 4-byte value from an offset in Data memory");
            SC_REPORT_INFO(this->basename(),"DFE_DumpStatusInfo ()                               Dump information of the status registers of the DFE model");
        } else {
            re_printf("warning","The name (%s) of DFE_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of DFE_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

/// Enable/disable dumping message when users access registers
void Cdfe_wrapper::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mDFE0->mDFEFunc->RegisterHandler(cmd);
    mDFE1->mDFEFunc->RegisterHandler(cmd);
    mFIFO->DumpRegisterRW(is_enable);
}//}}}

/// Enable/disable dumping interrupt information
void Cdfe_wrapper::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: DFE_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = (mDumpInterrupt) ? "true" : "false";
        re_printf("info","DFE_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: DFE_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

/// Separate some words from a string to store a vector
void Cdfe_wrapper::SeparateString (std::vector<std::string> &vtr, const std::string msg)
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

void Cdfe_wrapper::DumpStatusInfo(const std::string unit_name)
{//{{{
    if (unit_name == "DFE0") {
        mDFE0->DumpStatusInfo();
    }
    else if (unit_name == "DFE1") {
        mDFE1->DumpStatusInfo();
    }
    else if (unit_name == "FIFO") {
        mFIFO->DumpStatusInfo();
    }
    else {
        re_printf("warning","Incorrect unit name, it must be DFE0 or DFE1 or FIFO");
    }
}//}}}
// vim600: set foldmethod=marker :