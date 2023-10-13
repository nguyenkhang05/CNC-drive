// ---------------------------------------------------------------------
// $Id: DFE0051.cpp,v 1.3 2017/11/15 08:48:22 chanle Exp $
//
// Copyright(c) 2016-2017 Renesas Electronics Corporation
// Copyright(c) 2016-2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "DFE0051.h"
#include "DFE0051_Func.h"
#include "DFE0051_Wrapper.h"

/// Constructor and Destructor
template <unsigned int NUM_CHANNEL>
Cdfe0051<NUM_CHANNEL>::Cdfe0051(sc_module_name name, Cdfe0051_wrapper *parent)
        :sc_module(name)
        ,Cdfe0051_agent_controller()
        // Clock & reset
        ,CLK_HSB("CLK_HSB")
        ,CLK_UHSB("CLK_UHSB")
        ,PRESETn("PRESETn")
        // SAR-ADC input
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
        // DS-ADC input
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
        // Cyclic-ADC input
        ,CADC0_REQ("CADC0_REQ")
        ,CADC0_TAG("CADC0_TAG")
        ,CADC0_DATA("CADC0_DATA")
        // Error
        ,INTDFE0051ERR("INTDFE0051ERR")
{//{{{
    // Get parent pointer (dfe0051 wrapper)
    sc_assert(parent != NULL);
    mWrapper = parent;

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
    for (unsigned int index = 0; index < NUM_CHANNEL; index++){
        // INTDFE0051DOUT
        std::ostringstream str_INTDFE0051DOUT;
        str_INTDFE0051DOUT<<"INTDFE0051DOUT"<<index;
        INTDFE0051DOUT[index] = new sc_out<bool>(str_INTDFE0051DOUT.str().c_str());
        INTDFE0051DOUT[index]->initialize(false);
        // INTDFE0051CND0
        std::ostringstream str_INTDFE0051CND0;
        str_INTDFE0051CND0<<"INTDFE0051CND0"<<index;
        INTDFE0051CND0[index] = new sc_out<bool>(str_INTDFE0051CND0.str().c_str());
        INTDFE0051CND0[index]->initialize(false);
        // DFE0051_FIFO_CACK
        std::ostringstream str_DFE0051_FIFO_CACK;
        str_DFE0051_FIFO_CACK<<"DFE0051_FIFO_CACK"<<index;
        DFE0051_FIFO_CACK[index] = new sc_in<bool>(str_DFE0051_FIFO_CACK.str().c_str());
        // DFE0051_DFDOUT
        std::ostringstream str_DFE0051_DFDOUT;
        str_DFE0051_DFDOUT<<"DFE0051_DFDOUT"<<index;
        DFE0051_DFDOUT[index] = new sc_out<unsigned int>(str_DFE0051_DFDOUT.str().c_str());
        DFE0051_DFDOUT[index]->initialize(0);
        // DFE0051_DOUPD_N
        std::ostringstream str_DFE0051_DOUPD_N;
        str_DFE0051_DOUPD_N<<"DFE0051_DOUPD_N"<<index;
        DFE0051_DOUPD_N[index] = new sc_out<bool>(str_DFE0051_DOUPD_N.str().c_str());
        DFE0051_DOUPD_N[index]->initialize(false);
        // DFE0051_DOUT_END
        std::ostringstream str_DFE0051_DOUT_END;
        str_DFE0051_DOUT_END<<"DFE0051_DOUT_END"<<index;
        DFE0051_DOUT_END[index] = new sc_out<bool>(str_DFE0051_DOUT_END.str().c_str());
        DFE0051_DOUT_END[index]->initialize(false);
    }
    for (unsigned int index = 0; index < 4; index++){
        std::ostringstream str_INTDFE0051CND1;
        str_INTDFE0051CND1<<"INTDFE0051CND1"<<index;
        INTDFE0051CND1[index] = new sc_out<bool>(str_INTDFE0051CND1.str().c_str());
        INTDFE0051CND1[index]->initialize(false);

        std::ostringstream str_DFE0051_PHUPD;
        str_DFE0051_PHUPD<<"DFE0051_PHUPD"<<index;
        DFE0051_PHUPD[index] = new sc_out<bool>(str_DFE0051_PHUPD.str().c_str());
        DFE0051_PHUPD[index]->initialize(false);
    }
    for (unsigned int index = 0; index < 3; index++){
        std::ostringstream str_INTDFE0051SUBOUT;
        str_INTDFE0051SUBOUT<<"INTDFE0051SUBOUT"<<index;
        INTDFE0051SUBOUT[index] = new sc_out<bool>(str_INTDFE0051SUBOUT.str().c_str());
        INTDFE0051SUBOUT[index]->initialize(false);
    }

    mDFE0051Func = new Cdfe0051_func((std::string)name, NUM_CHANNEL, this);

    // Initialize output ports
    INTDFE0051ERR.initialize(false);

    // Initialize variables
    ConstructADInputPortTable();
    mIsPRESETInit = true;
    mHSBPeriod = 0;
    mHSBOrgFreq = 0;
    mHSBFreq = 0;
    mHSBFreqUnit = "Hz";
    mUHSBPeriod = 0;
    mUHSBOrgFreq = 0;
    mUHSBFreq = 0;
    mUHSBFreqUnit = "Hz";
    mIsResetHardActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive = false;
    mResetCmdPeriod = 0;
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    //mNormalFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emFIR8Tap, 23));
    //mNormalFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emFIR16Tap, 27));
    //mNormalFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emFIR24Tap, 31));
    //mNormalFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emFIR32Tap, 35));
    //mNormalFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emFIR64Tap, 51));
    //mNormalFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emIIR1Stage, 28));
    //mNormalFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emIIR2Stage, 35));
    //mNormalFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emIIR3Stage, 42));
    //mNormalFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emIIRGain2Stage, 37));
    //mNormalFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emIIRGain3Stage, 46));

    //mInitFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emFIR8Tap, 28));
    //mInitFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emFIR16Tap, 36));
    //mInitFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emFIR24Tap, 44));
    //mInitFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emFIR32Tap, 52));
    //mInitFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emFIR64Tap, 84));
    //mInitFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emIIR1Stage, 35));
    //mInitFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emIIR2Stage, 48));
    //mInitFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emIIR3Stage, 61));
    //mInitFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emIIRGain2Stage, 50));
    //mInitFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emIIRGain3Stage, 65));

    mNormalFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emFIR8Tap, 22));
    mNormalFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emFIR16Tap, 26));
    mNormalFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emFIR24Tap, 30));
    mNormalFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emFIR32Tap, 34));
    mNormalFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emFIR64Tap, 50));
    mNormalFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emIIR1Stage, 28));
    mNormalFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emIIR2Stage, 36));
    mNormalFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emIIR3Stage, 44));
    mNormalFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emIIRGain2Stage, 42));
    mNormalFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emIIRGain3Stage, 56));

    mInitFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emFIR8Tap, 28));
    mInitFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emFIR16Tap, 36));
    mInitFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emFIR24Tap, 44));
    mInitFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emFIR32Tap, 52));
    mInitFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emFIR64Tap, 84));
    mInitFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emIIR1Stage, 38));
    mInitFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emIIR2Stage, 54));
    mInitFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emIIR3Stage, 70));
    mInitFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emIIRGain2Stage, 60));
    mInitFilterProcCycle.insert(std::pair<eFilterKind, unsigned int>(emIIRGain3Stage, 82));

    mInputSourceNameMap.insert(std::pair<eInputSource, std::string>(emSoftware, "Software"));
    mInputSourceNameMap.insert(std::pair<eInputSource, std::string>(emSARADC0, "SAR-AD0"));
    mInputSourceNameMap.insert(std::pair<eInputSource, std::string>(emSARADC2, "SAR-AD2"));
    mInputSourceNameMap.insert(std::pair<eInputSource, std::string>(emSARADC1, "SAR-AD1"));
    mInputSourceNameMap.insert(std::pair<eInputSource, std::string>(emSARADC3, "SAR-AD3"));
    mInputSourceNameMap.insert(std::pair<eInputSource, std::string>(emDSADC00, "DS-ADC00"));
    mInputSourceNameMap.insert(std::pair<eInputSource, std::string>(emDSADC10, "DS-ADC10"));
    mInputSourceNameMap.insert(std::pair<eInputSource, std::string>(emDSADC20, "DS-ADC20"));
    mInputSourceNameMap.insert(std::pair<eInputSource, std::string>(emDSADC11, "DS-ADC11"));
    mInputSourceNameMap.insert(std::pair<eInputSource, std::string>(emDSADC12, "DS-ADC12"));
    mInputSourceNameMap.insert(std::pair<eInputSource, std::string>(emDSADC13, "DS-ADC13"));
    mInputSourceNameMap.insert(std::pair<eInputSource, std::string>(emDSADC14, "DS-ADC14"));
    mInputSourceNameMap.insert(std::pair<eInputSource, std::string>(emDSADC15, "DS-ADC15"));
    mInputSourceNameMap.insert(std::pair<eInputSource, std::string>(emDSADC21, "DS-ADC21"));
    mInputSourceNameMap.insert(std::pair<eInputSource, std::string>(emDSADC22, "DS-ADC22"));
    mInputSourceNameMap.insert(std::pair<eInputSource, std::string>(emCADC0, "C-ADC0"));
    mInputSourceNameMap.insert(std::pair<eInputSource, std::string>(emDeciCas, "Cascade of Decimation"));
    mInputSourceNameMap.insert(std::pair<eInputSource, std::string>(emSubCas0, "Cascade of Subtraction-0"));
    mInputSourceNameMap.insert(std::pair<eInputSource, std::string>(emSubCas1, "Cascade of Subtraction-1"));
    mInputSourceNameMap.insert(std::pair<eInputSource, std::string>(emSubCas2, "Cascade of Subtraction-2"));
    Initialize();

    // SC_THREAD/METHOD
    SC_METHOD(ADRequestMethod);
    dont_initialize();
    sensitive << SARADC0_REQ;
    sensitive << SARADC1_REQ;
    sensitive << SARADC2_REQ;
    sensitive << SARADC3_REQ;
    sensitive << DSADC00_REQ;
    sensitive << DSADC10_REQ;
    sensitive << DSADC20_REQ;
    sensitive << DSADC11_REQ;
    sensitive << DSADC12_REQ;
    sensitive << DSADC13_REQ;
    sensitive << DSADC14_REQ;
    sensitive << DSADC15_REQ;
    sensitive << DSADC21_REQ;
    sensitive << DSADC22_REQ;
    sensitive << CADC0_REQ;

    SC_METHOD(MonitorTimerTriggerMethod);
    dont_initialize();
    for (unsigned int index = 0; index < 20; index++){
        sensitive << (*TMTRG_A[index]);
        sensitive << (*TMTRG_B[index]);
    }

    SC_METHOD(DispatchMethod);
    dont_initialize();
    sensitive << mDispatchEvent;

    SC_METHOD(ActivateErrorInterruptMethod);
    dont_initialize();
    sensitive << mActivateErrorInterruptEvent;

    SC_METHOD(NegateErrorInterruptMethod);
    dont_initialize();
    sensitive << mNegateErrorInterruptEvent;

    SC_METHOD(OutputValueMethod);
    dont_initialize();
    sensitive << mOutputValueEvent;

    SC_METHOD(CaptureMethod);
    dont_initialize();
    for (unsigned int index = 0; index < 3; index++){
        sensitive << (*TMCAPTRG[index]);
    }

    SC_METHOD(RaiseINTDFE0051DOUTMethod);
    dont_initialize();
    sensitive << mRaiseINTDFE0051DOUTEvent;

    SC_METHOD(NegateINTDFE0051DOUTMethod);
    dont_initialize();
    sensitive << mNegateINTDFE0051DOUTEvent;

    SC_METHOD(RaiseINTDFE0051SUBOUTMethod);
    dont_initialize();
    sensitive << mRaiseINTDFE0051SUBOUTEvent;

    SC_METHOD(NegateINTDFE0051SUBOUTMethod);
    dont_initialize();
    sensitive << mNegateINTDFE0051SUBOUTEvent;

    SC_METHOD(RaiseINTDFE0051CND1Method);
    dont_initialize();
    sensitive << mRaiseINTDFE0051CND1Event;

    SC_METHOD(NegateINTDFE0051CND1Method);
    dont_initialize();
    sensitive << mNegateINTDFE0051CND1Event;

    SC_METHOD(UpdateTriggerStatusMethod);
    dont_initialize();
    sensitive << mUpdateTriggerStatusEvent;

    SC_METHOD(IssueOutputDataMethod);
    dont_initialize();
    sensitive << mIssueOutputDataEvent;

    SC_METHOD(NegateDOUTENDMethod);
    dont_initialize();
    sensitive << mNegateDOUTENDEvent;

    SC_METHOD(ClearDFDOUTMethod);
    dont_initialize();
    sensitive << mClearDFDOUTEvent;

    SC_METHOD(RaiseDFE0051PHUPDMethod);
    dont_initialize();
    sensitive << mRaiseDFE0051PHUPDEvent;

    SC_METHOD(NegateDFE0051PHUPDMethod);
    dont_initialize();
    sensitive << mNegateDFE0051PHUPDEvent;

    SC_METHOD(MonitorFIFOACKMethod);
    dont_initialize();
    for (unsigned int i = 0; i < NUM_CHANNEL; i++){
        sensitive << (*DFE0051_FIFO_CACK[i]);
    }

    SC_METHOD(CLKHSBMethod);
    dont_initialize();
    sensitive << CLK_HSB;

    SC_METHOD(CLKUHSBMethod);
    dont_initialize();
    sensitive << CLK_UHSB;

    SC_METHOD(PRESETnMethod);
    dont_initialize();
    sensitive << PRESETn;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;

    SC_METHOD(RaiseINTDFE0051CND0Method);
    dont_initialize();
    sensitive << mRaiseINTDFE0051CND0Event;

    SC_METHOD(NegateINTDFE0051CND0Method);
    dont_initialize();
    sensitive << mNegateINTDFE0051CND0Event;

    SC_METHOD(DeassertDOUTNMethod);
    dont_initialize();
    sensitive << mDeassertDOUTNEvent;

    SC_METHOD(WriteDFE0051PHUPDMethod);
    dont_initialize();
    sensitive << mWriteDFE0051PHUPDEvent;

    SC_METHOD(WriteINTDFE0051DOUTMethod);
    dont_initialize();
    sensitive << mWriteINTDFE0051DOUTEvent;

    SC_METHOD(WriteINTDFE0051CND0Method);
    dont_initialize();
    sensitive << mWriteINTDFE0051CND0Event;

    SC_METHOD(WriteINTDFE0051CND1Method);
    dont_initialize();
    sensitive << mWriteINTDFE0051CND1Event;

    SC_METHOD(WriteINTDFE0051ERRMethod);
    dont_initialize();
    sensitive << mWriteINTDFE0051ERREvent;

    SC_METHOD(WriteINTDFE0051SUBOUTMethod);
    dont_initialize();
    sensitive << mWriteINTDFE0051SUBOUTEvent;

    SC_METHOD(WriteDFDOUTMethod);
    dont_initialize();
    sensitive << mWriteDFE0051_DFDOUTEvent;

    SC_METHOD(WriteDFE0051_DOUPD_NMethod);
    dont_initialize();
    sensitive << mWriteDFE0051_DOUPD_NEvent;

    SC_METHOD(WriteDFE0051_DOUT_ENDMethod);
    dont_initialize();
    sensitive << mWriteDFE0051_DOUT_ENDEvent;

    SC_THREAD(DFE0051ProcessThread);
    dont_initialize();
    sensitive << mProcessEvent;
    sensitive << mResetEvent;
}//}}}

/// Destructor
template <unsigned int NUM_CHANNEL>
Cdfe0051<NUM_CHANNEL>::~Cdfe0051()
{//{{{
    delete mDFE0051Func;
}//}}}

/// Table store info of AD ports
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::ConstructADInputPortTable(void)
{//{{{
    // Table of AD input port
    mADPortTable.push_back(ADPortElement(emSARADC0, &SARADC0_REQ, &SARADC0_TAG, &SARADC0_DATA, false));
    mADPortTable.push_back(ADPortElement(emSARADC1, &SARADC1_REQ, &SARADC1_TAG, &SARADC1_DATA, false));
    mADPortTable.push_back(ADPortElement(emSARADC2, &SARADC2_REQ, &SARADC2_TAG, &SARADC2_DATA, false));
    mADPortTable.push_back(ADPortElement(emSARADC3, &SARADC3_REQ, &SARADC3_TAG, &SARADC3_DATA, false));
    mADPortTable.push_back(ADPortElement(emDSADC00, &DSADC00_REQ, &DSADC00_TAG, &DSADC00_DATA, false));
    mADPortTable.push_back(ADPortElement(emDSADC10, &DSADC10_REQ, &DSADC10_TAG, &DSADC10_DATA, false));
    mADPortTable.push_back(ADPortElement(emDSADC20, &DSADC20_REQ, &DSADC20_TAG, &DSADC20_DATA, false));
    mADPortTable.push_back(ADPortElement(emDSADC11, &DSADC11_REQ, &DSADC11_TAG, &DSADC11_DATA, false));
    mADPortTable.push_back(ADPortElement(emDSADC12, &DSADC12_REQ, &DSADC12_TAG, &DSADC12_DATA, false));
    mADPortTable.push_back(ADPortElement(emDSADC13, &DSADC13_REQ, &DSADC13_TAG, &DSADC13_DATA, false));
    mADPortTable.push_back(ADPortElement(emDSADC14, &DSADC14_REQ, &DSADC14_TAG, &DSADC14_DATA, false));
    mADPortTable.push_back(ADPortElement(emDSADC15, &DSADC15_REQ, &DSADC15_TAG, &DSADC15_DATA, false));
    mADPortTable.push_back(ADPortElement(emDSADC21, &DSADC21_REQ, &DSADC21_TAG, &DSADC21_DATA, false));
    mADPortTable.push_back(ADPortElement(emDSADC22, &DSADC22_REQ, &DSADC22_TAG, &DSADC22_DATA, false));
    mADPortTable.push_back(ADPortElement(emCADC0,   &CADC0_REQ,   &CADC0_TAG,   &CADC0_DATA,   false));
}//}}}

/// Initialize when reset activated
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::Initialize(void)
{//{{{
    for (unsigned int channel = 0; channel < NUM_CHANNEL; channel++){
        mChannelInfoTable.push_back(ChannelInfo(false, 0));
        mGetPHMaskFlag[channel] = false;
        mGetPHEnableFlag[channel] = false;
        mGetAccumDeciEnableFlag[channel] = true;
        mGetFilterEnableFlag[channel] = true;
        mStatusPHEnable[channel] = false;
        mStatusPHMask[channel] = false;
        mStatusAccumDeciEnable[channel] = true;
        mStatusFilterEnable[channel] = true;
        mChannelEnable[channel] = false;
        mIsFilterInit[channel] = true;
        mIsPHInit[channel] = true;
        mCount[channel] = 0;
        mExpectedCount[channel] = 0;
        mAccumVal[channel] = 0;
        mINTDFE0051CND0[channel] = 0;
        mPHCounter[channel] = 0;
        mIsStartPHPhase[channel] = false;
        mIsInitAccumDeci[channel] = false;
        mIsDisablePHTrg[channel] = false;
        mIsOutFromAccum[channel] = false;
        mDOUTN[channel] = false;
        mINTDFE0051DOUT[channel] = false;
        mDFE0051_DFDOUT[channel] = 0;
        mDFE0051_DOUPD_N[channel] = false;
        mDFE0051_DOUT_END[channel] = false;
    }
    for (unsigned int i = 0; i < emNumInputSource; i++){
        mInputSourceTable[i].StoreInfo(0,0,0);
    }
    for (unsigned int i = 0; i < 3; i++){
        mGetSubEnableFlag[i] = true;
        mStatusSubEnable[i] = true;
        mCaptureEnable[i] = false;
        mSubtractionEnable[i] = false;
        mSubtrahend[i] = 0;
        mMinuend[i] = 0;
        mSubValue[i] = 0;
        mINTDFE0051SUBOUT[i] = false;
        mIsSubClear[i] = false;
    }
    for (unsigned int i = 0; i < 4; i++){
        mINTDFE0051CND1[i] = 0;
        mDFE0051PHUPD[i] = 0;
    }
    mOutChannelNum = 0;
    mOutValue = 0;
    mFilterOutData = 0;
    mIsFirstInput = true;
    mINTDFE0051ERR = false;
}//}}}

/// Assert INTDFE0051ERR 
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::ActivateErrorInterruptMethod()
{//{{{
    mINTDFE0051ERR = true;
    mWriteINTDFE0051ERREvent.notify();
    mNegateErrorInterruptEvent.notify((double)mUHSBPeriod, this->mTimeResolutionUnit);
}//}}}

/// Negate INTDFE0051ERR
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::NegateErrorInterruptMethod()
{//{{{
    mINTDFE0051ERR = false;
    mWriteINTDFE0051ERREvent.notify();
}//}}}

/// Write INTDFE0051ERR
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::WriteINTDFE0051ERRMethod()
{//{{{
    INTDFE0051ERR.write(mINTDFE0051ERR);
}//}}}

/// Monitor Timer trigger ports
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::MonitorTimerTriggerMethod()
{//{{{
    for (unsigned int channel = 0; channel < NUM_CHANNEL; channel++){
        // Peak-hold Mask End/Start
        unsigned int pmfe = mDFE0051Func->GetPMFEBit(channel);
        unsigned int pme = mDFE0051Func->GetPMEBit(channel);
        unsigned int pmt = mDFE0051Func->GetPMTBit(channel); // Get trigger is 0-5
        unsigned int pmitmtrg = mDFE0051Func->GetPMITMTRGBit(pmt); // Get comparison is from 0-19
        if (pme == 1){
            if (TMTRG_A[pmitmtrg]->read()){
                mGetPHMaskFlag[channel] = true;
                mDFE0051Func->SetPMITSBit(channel, 1);
                re_printf("info", "Mask start PH on channel %d.\n", channel);
            }
            if ((pmfe == 1) && (TMTRG_B[pmitmtrg]->read())){
                mGetPHMaskFlag[channel] = false;
                mDFE0051Func->SetPMETSBit(channel, 1);
                re_printf("info", "Mask end PH on channel %d.\n", channel);
            }
        }
        // Peak-hold End/Initialize
        unsigned int pe = mDFE0051Func->GetPEBit(channel);
        unsigned int pfe = mDFE0051Func->GetPFEBit(channel);
        unsigned int pitmtrg = 0;
        int pt = mDFE0051Func->GetPTBit(channel);
        if ((pe == 1) || (pe == 3)){
            pitmtrg = mDFE0051Func->GetPITMTRGBit((unsigned int)pt);
            if (TMTRG_A[pitmtrg]->read()){
                mGetPHEnableFlag[channel] = true;
                mDFE0051Func->SetDISBBit(channel, 0);
                mDFE0051Func->SetPITSBit(channel, 1);
                mIsPHInit[channel] = true;
                mPHCounter[channel] = 0;
                re_printf("info", "Initialize PH on channel %d.\n", channel);
            }
            if ((pfe == 1) && (TMTRG_B[pitmtrg]->read())){
                mGetPHEnableFlag[channel] = false;
                mDFE0051Func->SetDISBBit(channel, 1);
                mDFE0051Func->SetPETSBit(channel, 1);
                mIsDisablePHTrg[channel] = true;
                re_printf("info", "Disable PH on channel %d.\n", channel);
            }
        }
        // Accumulation/Decimation Initialize/Disable
        unsigned int ae = mDFE0051Func->GetAEBit(channel);
        unsigned int afe = mDFE0051Func->GetAFEBit(channel);
        unsigned int mitmtrg =0;
        int at = mDFE0051Func->GetATBit(channel);
        if ((ae == 1) || (ae == 3)){
            unsigned int mitmtrg = mDFE0051Func->GetMITMTRGBit((unsigned int)at);
            if (TMTRG_A[mitmtrg]->read()){
                mGetAccumDeciEnableFlag[channel] = true;
                mDFE0051Func->SetDISABit(channel, 0);
                mDFE0051Func->SetMITSBit(channel, 1);
                mIsInitAccumDeci[channel] = true;
                re_printf("info", "Initialize Accumulation/Decimation on channel %d.\n", channel);
            }
            if ((afe == 1) && (TMTRG_B[mitmtrg]->read())){
                mGetAccumDeciEnableFlag[channel] = false;
                mDFE0051Func->SetDISABit(channel, 1);
                mDFE0051Func->SetMETSBit(channel, 1);
                re_printf("info", "Disable Accumulation/Decimation on channel %d.\n", channel);
            }
        }
        // Filter Initialize
        unsigned int fe = mDFE0051Func->GetFEBit(channel);
        unsigned int ft = mDFE0051Func->GetFTBit(channel);
        if ((fe == 1) || (fe == 3)){
            unsigned int fitmtrg = mDFE0051Func->GetFITMTRGBit((unsigned int)ft);
            if (TMTRG_A[fitmtrg]->read()){
                mGetFilterEnableFlag[channel] = true;
                mDFE0051Func->SetFITSBit(channel, 1);
                mIsFilterInit[channel] = true;
                re_printf("info", "Initialize Filter on channel %d.\n", channel);
            }
        }
    }
    for (unsigned int subid = 0; subid < 3; subid++){
        // Subtraction Start/End
        unsigned int sbe = mDFE0051Func->GetSBEBit(subid);
        unsigned int sbfe = mDFE0051Func->GetSBFEBit(subid);
        unsigned int sbt = mDFE0051Func->GetSBTBit(subid); // Choose 0-2
        unsigned int subtmtrg = mDFE0051Func->GetSUBTMTRGBit(sbt);
        if ((sbe == 1) && (TMTRG_A[subtmtrg]->read())){
            mGetSubEnableFlag[subid] = true;
            mDFE0051Func->SetSITSBit(subid, 1);
            re_printf("info", "Subtraction start trigger on ID[%d].\n", subid);
        }
        if ((sbfe == 1) && (TMTRG_B[subtmtrg]->read())){
            mGetSubEnableFlag[subid] = false;
            mDFE0051Func->SetSETSBit(subid, 1);
            re_printf("info", "Subtraction end trigger on ID[%d].\n", subid);
        }
    }
    mUpdateTriggerStatusEvent.cancel(); // Cancel first
    mUpdateTriggerStatusEvent.notify((double)mUHSBPeriod, this->mTimeResolutionUnit); // Then, notify after 1 UHSB period
}//}}}

/// Update status according timer trigger port
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::UpdateTriggerStatusMethod()
{//{{{
    for (unsigned int channel = 0; channel < NUM_CHANNEL; channel++){
        // PH Disable trigger
        if (mStatusPHEnable[channel] && !mGetPHEnableFlag[channel]){
            mDFE0051Func->SetPETSBit(channel, 0);// Clear when finish disable PH
            // PH end interrupt for channel 0-3 only when PRCSB =3, CNSLE=0
            unsigned int prcsb = mDFE0051Func->GetPRCSBBit(channel);
            unsigned int cnsle = mDFE0051Func->GetCNSLEBit(channel);
            unsigned int cnsl = mDFE0051Func->GetCNSLBit(channel);
            unsigned int iep = mDFE0051Func->GetIEPBit(channel);
            if (mStatusPHEnable[channel] && !mGetPHEnableFlag[channel] && (channel < 4) && (prcsb == 3) && (iep ==1)){
                if (cnsle == 0){
                    mINTDFE0051CND1[channel] = true;
                }else{//cnsle == 1
                    if (cnsl == 0){
                        mINTDFE0051CND1[channel] = true;
                    }else{//cnsl == 1
                        mINTDFE0051CND0[channel] = true;
                    }
                }
            }
        }
        //// Note: PH Init trigger finish when there is input to PH

        // PH Mask End trigger
        if (mStatusPHMask[channel] && !mGetPHMaskFlag[channel]){
            mDFE0051Func->SetPMETSBit(channel, 0);// Clear when finish PH Mask Start
        }
        // PH Mask Start trigger
        if (!mStatusPHMask[channel] && mGetPHMaskFlag[channel]){
            mDFE0051Func->SetPMITSBit(channel, 0);// Clear when finish PH Mask Start
        }
        // Accum Init trigger
        if (!mStatusAccumDeciEnable[channel] && mGetAccumDeciEnableFlag[channel]){
            mCount[channel] = 0;// Initialize counter value
            mAccumVal[channel] = 0;
            mDFE0051Func->SetMITSBit(channel, 0);// Clear when finish initialization accumulation/decimation
        }
        // Accum Disable trigger
        if (mStatusAccumDeciEnable[channel] && !mGetAccumDeciEnableFlag[channel]){
            mDFE0051Func->SetMETSBit(channel, 0);// Clear when finish disable accumulation/decimation
        }
        //// Note: Filter Init trigger finish initialization when there is input to filter
    }
    // Out CND1 if any
    for (unsigned int channel = 0; channel <4; channel++){
        if (mINTDFE0051CND1[channel]){
            mRaiseINTDFE0051CND1Event.notify();
            break;
        }
        if (mINTDFE0051CND0[channel]){
            mRaiseINTDFE0051CND0Event.notify();
            break;
        }
    }

    for (unsigned int subid = 0; subid < 3; subid++){
        if (!mStatusSubEnable[subid] && mGetSubEnableFlag[subid]){
            mDFE0051Func->SetSITSBit(subid, 0);// Clear when finish start subtraction
        }else if (mStatusSubEnable[subid] && !mGetSubEnableFlag[subid]){
            mDFE0051Func->SetSETSBit(subid, 0);// Clear when finish end subtraction
        }
    }

}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
/// Start SW input when users write data to DFE0051jDI reg
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::StartSWInput()
{//{{{
    unsigned int tag = mDFE0051Func->GetSWInputTag();
    unsigned int data = mDFE0051Func->GetSWInputData();
    mInputSourceTable[emSoftware].StoreInfo(tag, data, true);
    
    mDispatchEvent.notify();
}//}}}

/// Issue SW trigger when users write value SW trigger reg
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::IssueSWTrigger()
{//{{{
    // Check which sensitive with sw trigger
    for (unsigned int channel = 0; channel < NUM_CHANNEL; channel++){
        // Peak-hold Mask End/Start
        unsigned int pmfe = mDFE0051Func->GetPMFEBit(channel);
        unsigned int pme = mDFE0051Func->GetPMEBit(channel);
        if (pme == 2){
            mGetPHMaskFlag[channel] = true;
            mDFE0051Func->SetPMITSBit(channel, 1);
            re_printf("info", "Mask start PH on channel %d.\n", channel);
        }
        if (pmfe == 2){
            mGetPHMaskFlag[channel] = false;
            mDFE0051Func->SetPMETSBit(channel, 1);
            re_printf("info", "Mask end PH on channel %d.\n", channel);
        }
        // Peak-hold End/Initialize
        unsigned int pe = mDFE0051Func->GetPEBit(channel);
        unsigned int pfe = mDFE0051Func->GetPFEBit(channel);
        if (pe == 2){
            mGetPHEnableFlag[channel] = true;
            mDFE0051Func->SetDISBBit(channel, 0);
            mDFE0051Func->SetPITSBit(channel, 1);
            mIsPHInit[channel] = true;
            mPHCounter[channel] = 0;
            re_printf("info", "Initialize PH on channel %d.\n", channel);
        }
        if (pfe == 2){
            mGetPHEnableFlag[channel] = false;
            mDFE0051Func->SetDISBBit(channel, 1);
            mDFE0051Func->SetPETSBit(channel, 1);
            mIsDisablePHTrg[channel] = true;
            re_printf("info", "Disable PH on channel %d.\n", channel);
        }
        // Accumulation/Decimation Initialize/Disable
        unsigned int ae = mDFE0051Func->GetAEBit(channel);
        unsigned int afe = mDFE0051Func->GetAFEBit(channel);
        if (ae == 2){
            mGetAccumDeciEnableFlag[channel] = true;
            mDFE0051Func->SetDISABit(channel, 0);
            mDFE0051Func->SetMITSBit(channel, 1);
            mIsInitAccumDeci[channel] = true;
            re_printf("info", "Initialize Accumulation/Decimation on channel %d.\n", channel);
        }
        if (afe == 2){
            mGetAccumDeciEnableFlag[channel] = false;
            mDFE0051Func->SetDISABit(channel, 1);
            mDFE0051Func->SetMETSBit(channel, 1);
            re_printf("info", "Disable Accumulation/Decimation on channel %d.\n", channel);
        }
        // Filter Initialize
        unsigned int fe = mDFE0051Func->GetFEBit(channel);
        unsigned int ft = mDFE0051Func->GetFTBit(channel);
        if (fe == 2){
            mGetFilterEnableFlag[channel] = true;
            mDFE0051Func->SetFITSBit(channel, 1);
            mIsFilterInit[channel] = true;
            re_printf("info", "Initialize Filter on channel %d.\n", channel);
        }
    }
    for (unsigned int subid = 0; subid < 3; subid++){
        // Subtraction Start/End
        unsigned int sbe = mDFE0051Func->GetSBEBit(subid);
        unsigned int sbfe = mDFE0051Func->GetSBFEBit(subid);
        if (sbe == 2){
            mGetSubEnableFlag[subid] = true;
            mDFE0051Func->SetSITSBit(subid, 1);
            re_printf("info", "Subtraction start trigger on ID[%d].\n", subid);
        }
        if (sbfe == 2){
            mGetSubEnableFlag[subid] = false;
            mDFE0051Func->SetSETSBit(subid, 1);
            mIsSubClear[subid] = true;
            re_printf("info", "Subtraction end trigger on ID[%d].\n", subid);
        }
    }
    mUpdateTriggerStatusEvent.cancel(); // Cancel first
    mUpdateTriggerStatusEvent.notify(); // Then, notify
}//}}}

/// Update enabled status of channel once EN bit is changed
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::UpdateChannelEnable(unsigned int channel, bool enable)
{//{{{
    mChannelEnable[channel] = enable;
    // Update status of Sub enable
    for(unsigned int subid = 0; subid < 3; subid++){
        if (mDFE0051Func->GetSBEBit(subid) == 0){
            mStatusSubEnable[subid] = true;
        }else{
            mStatusSubEnable[subid] = false;
            mIsSubClear[subid] = true;
        }
        
    }
    // Update status of PH incase PH and comparison run simultaneously
    unsigned int prcsb = mDFE0051Func->GetPRCSBBit(channel);
    if (prcsb == 3){
        mStatusPHEnable[channel] = false;// must wait initialize trigger to start
    }
}//}}}

/// Update enabled status of capture unit once CEN is changed
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::UpdateCaptureEnable(unsigned int capid, bool enable)
{//{{{
    mCaptureEnable[capid] = enable;
}//}}}

/// Auto initialize when EN = 1, and AIME = 1
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::AutoInitialize(unsigned int channel)
{//{{{
    re_printf("info", "Auto-initialize on channel %d.\n", channel);
    unsigned int ph23enable = mDFE0051Func->GetPH23PENBit();
    unsigned int chs = mDFE0051Func->GetCHSBit();

    mIsFilterInit[channel] = true;
    mIsPHInit[channel] = true;
    mPHCounter[channel] = 0;
    mCount[channel] = 0;// Initialize counter value
    mAccumVal[channel] = 0;
    // Clear some registers as initialization
    mDFE0051Func->SetPHINDBit(1,channel, 0);
    if ((ph23enable == 1) && (channel == chs)){
        mDFE0051Func->SetPHINDBit(2,channel, 0);//Clear PH2IND
        mDFE0051Func->SetPHINDBit(3,channel, 0);//Clear PH3IND
    }
    mDFE0051Func->SetDOCHReg(channel, 0);
    if ((channel %2) == 0){
        mDFE0051Func->StoreIntermediateValue(channel, 0, 0);
    }
    // Clear Data output port and notification
    mClearDFDOUTEvent.notify();
    mNegateDOUTENDEvent.notify();
}//}}}

/// Trigger other method to assert INTDFE0051ERR
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::RaiseINTDFE0051ERR()
{//{{{
    mActivateErrorInterruptEvent.notify();
}//}}}

/// Disable status of subtraction
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::DisableSubtractionFlag(unsigned int subid)
{//{{{
    mGetSubEnableFlag[subid] = false;
}//}}}
//========================================================
//============ Reset and clock  ==========================
//========================================================
/// Monitor PRESETn port
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::PRESETnMethod()
{//{{{
    bool reset = PRESETn.read();
    if (reset == vpcl::emResetActive){
        mIsPRESETInit = false;
        mIsResetHardActive = true;
        re_printf("info", "The reset port PRESETn is asserted.\n");
        EnableReset(true);
    }else{
        if (mIsPRESETInit){
            mIsPRESETInit = false;
        }else{
            mIsResetHardActive = false;
            re_printf("info", "The reset port PRESETn is de-asserted.\n");
            EnableReset(false);
        }
    }
    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive && mIsResetHardActive) {
        mResetCmdEvent.cancel();
        mResetCmdCancelEvent.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
    }
}//}}}

/// Reset model and its sub-instance if reset is actived
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::EnableReset(const bool is_active)
{//{{{
    if (is_active){
        // Cancel events
        mDispatchEvent.cancel();
        mActivateErrorInterruptEvent.cancel();
        mNegateErrorInterruptEvent.cancel();
        mOutputValueEvent.cancel();
        mRaiseINTDFE0051DOUTEvent.cancel();
        mNegateINTDFE0051DOUTEvent.cancel();
        mRaiseINTDFE0051SUBOUTEvent.cancel();
        mNegateINTDFE0051SUBOUTEvent.cancel();
        mRaiseINTDFE0051CND1Event.cancel();
        mNegateINTDFE0051CND1Event.cancel();
        mRaiseINTDFE0051CND0Event.cancel();
        mNegateINTDFE0051CND0Event.cancel();
        mProcessEvent.cancel();
        mUpdateTriggerStatusEvent.cancel();
        mIssueOutputDataEvent.cancel();
        mNegateDOUTENDEvent.cancel();
        mRaiseDFE0051PHUPDEvent.cancel();
        mNegateDFE0051PHUPDEvent.cancel();
        mDeassertDOUTNEvent.cancel();
        mWriteDFE0051PHUPDEvent.cancel();
        mWriteINTDFE0051DOUTEvent.cancel();
        mWriteINTDFE0051CND0Event.cancel();
        mWriteINTDFE0051CND1Event.cancel();
        mWriteINTDFE0051ERREvent.cancel();
        mWriteINTDFE0051SUBOUTEvent.cancel();
        mWriteDFE0051_DFDOUTEvent.cancel();
        mWriteDFE0051_DOUPD_NEvent.cancel();
        mWriteDFE0051_DOUT_ENDEvent.cancel();
        // Notify reset event
        mResetEvent.notify();
        // Initialize variables
        Initialize();
        // Initialize output ports
        mNegateErrorInterruptEvent.notify();
        mNegateINTDFE0051SUBOUTEvent.notify();
        mNegateINTDFE0051CND1Event.notify();
        mNegateINTDFE0051CND0Event.notify();
        mNegateINTDFE0051DOUTEvent.notify();
        mNegateDOUTENDEvent.notify();
        mClearDFDOUTEvent.notify();
        mNegateDFE0051PHUPDEvent.notify();
        mDeassertDOUTNEvent.notify();
    }
    mDFE0051Func->EnableReset(is_active);
}//}}}

/// Monitor HSB clock
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::CLKHSBMethod()
{//{{{
    sc_dt::uint64 freq_value = CLK_HSB.read();
    SetCLKFreq("CLK_HSB", freq_value, "Hz");
}//}}}

/// Monitor UHSB clock
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::CLKUHSBMethod()
{//{{{
    sc_dt::uint64 freq_value = CLK_UHSB.read();
    SetCLKFreq("CLK_UHSB", freq_value, "Hz");
}//}}}

/// Check frequency value and frequency unit
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out, sc_dt::uint64 freq_in, std::string unit_in)
{//{{{
    if(unit_in == "GHz"){
        freq_out = freq_in * (sc_dt::uint64)(1.0e+9);
        unit_out = "GHz";
    } else if (unit_in == "MHz") {
        freq_out = freq_in * (sc_dt::uint64)(1.0e+6);
        unit_out = "MHz";
    } else if (unit_in == "KHz") {
        freq_out = freq_in * (sc_dt::uint64)(1.0e+3);
        unit_out = "KHz";
    } else if (unit_in == "Hz") {
        freq_out = freq_in;
        unit_out = "Hz";
    } else {// Frequency unit is wrong, frequency unit is set as Hz default.
        freq_out = freq_in;
        unit_out = "Hz";
        re_printf("warning","Frequency unit (%s) is wrong, frequency unit is set as unit Hz default.\n", unit_in.c_str());
    }
}//}}}

/// Process reset function when reset command is active
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::HandleResetCmdMethod ()
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of PRESETn.\n");
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

/// Cancel reset function when reset command is active
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::CancelResetCmdMethod ()
{//{{{
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of PRESETn is over.\n");
    this->EnableReset(mIsResetCmdActive);
}//}}}

/// Check reset status
template <unsigned int NUM_CHANNEL>
bool Cdfe0051<NUM_CHANNEL>::GetResetStatus ()
{//{{{
    return (mIsResetHardActive || mIsResetCmdActive);
}//}}}

/// Get time resolution
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
{//{{{
    if (sc_get_time_resolution() < sc_time(1, SC_PS)) {
        resolution_value = 1000000000000000LL;
        resolution_unit = SC_FS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_NS)) {
        resolution_value = 1000000000000LL;
        resolution_unit = SC_PS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_US)) {
        resolution_value = 1000000000;
        resolution_unit = SC_NS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_MS)) {
        resolution_value = 1000000;
        resolution_unit = SC_US;
    } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)) {
        resolution_value = 1000;
        resolution_unit = SC_MS;
    } else {
        resolution_value = 1;
        resolution_unit = SC_SEC;
    }
}//}}}

//========================================================
//============ Dispatch circuit ==========================
//========================================================
/// Monitor the change of REQ port of AD input
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::ADRequestMethod(void)
{//{{{
    bool has_input = false;
    for (unsigned int port_id = 0; port_id < mADPortTable.size(); port_id++){
        if ((mADPortTable[port_id].request->read()) && (!mADPortTable[port_id].pre_request)){// request change from 0 to 1
            sc_uint<4> tag = mADPortTable[port_id].tag->read();
            sc_uint<16> data = mADPortTable[port_id].data->read();
            eInputSource source = mADPortTable[port_id].source;
            mInputSourceTable[source].StoreInfo((unsigned int)tag, (unsigned int)data, true);
            has_input = true;
        }
        mADPortTable[port_id].pre_request = mADPortTable[port_id].request->read();
    }
    if (has_input){
        mDispatchEvent.notify();
    }
}//}}}

/// Update Enable of all circuit once receiving input
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::UpdateEnableStatus()
{//{{{ 
    // Update status trigger
    for (unsigned int channel = 0; channel < NUM_CHANNEL; channel++){
        unsigned int pme = mDFE0051Func->GetPMEBit(channel);
        unsigned int pe = mDFE0051Func->GetPEBit(channel);
        unsigned int ae = mDFE0051Func->GetAEBit(channel);
        unsigned int fe = mDFE0051Func->GetFEBit(channel);
        if (pme != 0){
            mStatusPHMask[channel]           = mGetPHMaskFlag[channel];
        }
        if (pe != 0){
            mStatusPHEnable[channel]         = mGetPHEnableFlag[channel];
        }
        if (ae != 0){
            mStatusAccumDeciEnable[channel]  = mGetAccumDeciEnableFlag[channel];
        }
        if (fe != 0){
            mStatusFilterEnable[channel]     = mGetFilterEnableFlag[channel];
        }
    }
    for (unsigned int subid = 0; subid < 3; subid++){
        unsigned int sbe = mDFE0051Func->GetSBEBit(subid);
        unsigned int sbfe = mDFE0051Func->GetSBFEBit(subid);
        if ((sbe!=0) ||(sbfe!=0)){
            mStatusSubEnable[subid]        = mGetSubEnableFlag[subid];
        }
    }

    // Update according EN, Accumulation enable, 
    for (unsigned int channel = 0; channel < NUM_CHANNEL; channel++){
        if (mDFE0051Func->GetENBit(channel) == 1){
            mChannelEnable[channel] = true;
        }else{
            mChannelEnable[channel] = false;
            mChannelInfoTable[channel].active = false;
            mDFE0051Func->SetTRHCHReg(channel, 0);
        }
        // Init variable for AccumDeci
        if (mIsInitAccumDeci[channel]){
            mCount[channel] = 0;
            mAccumVal[channel] = 0;
            mIsInitAccumDeci[channel] = false;
        }
    }
    // Update according SEN
    for (unsigned int subid = 0; subid < 3; subid++){
        if (mDFE0051Func->GetSENBit(subid) == 1){
            mSubtractionEnable[subid] = true;
        }else{
            mSubtractionEnable[subid] = false;
            mDFE0051Func->SetSUBTRHCHReg(subid, 0);
            mIsSubClear[subid] = true;
        }
    }
    // Update according CEN
    for (unsigned int capid = 0; capid < 3; capid++){
        mCaptureEnable[capid] = (mDFE0051Func->GetCENBit(capid) == 1);
    }
}//}}}

/// Dispatch input data, select data to filter
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::DispatchMethod(void)
{//{{{
    unsigned int tag = 0;
    sc_uint<16> data = 0;
    eInputSource source = emNumInputSource;
    bool has_valid_input = false;

    // Priority SW > AD input > Cascade
    for (unsigned int src_id = 0; src_id < emNumInputSource; src_id++){
        if (mInputSourceTable[src_id].active){
            tag = mInputSourceTable[src_id].tag;
            data = mInputSourceTable[src_id].data;
            source = (eInputSource)src_id;
            re_printf("info","Receive input: [Source]: %s, [Tag]: 0x%X, [Data]: 0x%X.\n", mInputSourceNameMap[source].c_str(), tag, (unsigned int)data);

            // Process for all channel which matched tag with above
            for (unsigned int ch_id = 0; ch_id < NUM_CHANNEL; ch_id++){
                bool is_matched = mDFE0051Func->CheckMatchTag(tag, ch_id);
                if (is_matched){
                    bool is_connect_valid = CheckAssignChannelValid(source, ch_id);
                    if (is_connect_valid){
                        re_printf("info", "Channel %d is matched.\n", ch_id);
                        if (mDFE0051Func->GetVALIDBit(ch_id) == 1){
                            mDFE0051Func->SetDIOWBit(ch_id, 1);
                        }else{
                            mChannelInfoTable[ch_id].StoreInfo(true, data);
                            mDFE0051Func->SetVALIDBit(ch_id, 1);
                            has_valid_input = true;
                        }
                    }
                }
            }
        }
    }

    // Clear mInputSourceTable to not active
    for (unsigned int src_id = 0; src_id < emNumInputSource; src_id++){
        mInputSourceTable[src_id].active = false;
    }

    if (has_valid_input && mIsFirstInput){
        mIsFirstInput = false;
        mProcessEvent.notify(4*(double)mUHSBPeriod, this->mTimeResolutionUnit);//4 cycles as IP spec. chapter 8.3.1
        UpdateEnableStatus();
    }
}//}}}

/// Main thread to process DFE0051
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::DFE0051ProcessThread(void)
{//{{{
    while (1){
            fflush(stdout);
        if (GetResetStatus() || (mUHSBPeriod == 0) || (mHSBPeriod == 0)){
            fflush(stdout);
            wait();
            continue;
        }
        try {
            while (1){
                mIsFirstInput = true;
                // =========== PROCESS === //
                for (unsigned int ch_id = 0; ch_id<NUM_CHANNEL; ch_id++){
                    // Not operate if channel is not enable or no input active
                    if (!((mChannelInfoTable[ch_id].active) && (mChannelEnable[ch_id]))){
                        mDFE0051Func->SetVALIDBit(ch_id, 0);//Clear VALID when start Accumulation unit
                        continue;
                    }

                    // Ignored if odd channel and previous even channel is FIR64.
                    if (ch_id%2 == 1){
                        unsigned int filter_kind = mDFE0051Func->GetCMDBit(ch_id-1);
                        if ((eFilterKind)filter_kind == emFIR64Tap){
                            re_printf("info", "Channel %d is ignored because FIR64 is processed in channel %d.\n", ch_id, ch_id-1);
                            continue;
                        }
                    }

                    //=======Wait "filter processing time" as HWM // 
                    bool is_filter_init = mIsFilterInit[ch_id];// Get before this variable be changed in Filter()
                    if (mStatusFilterEnable[ch_id]){
                        re_printf("info", "Start filter on channel %d.\n", ch_id);
                        bool is_filter_success = Filter(ch_id);
                        if (!is_filter_success){
                            continue;
                        }else{
                            // Wait a processing time
                            unsigned int filter_kind = mDFE0051Func->GetCMDBit(ch_id);
                            unsigned int proc_cycle = GetFilterProcCycle(filter_kind, is_filter_init);
                            wait(proc_cycle * (double)mUHSBPeriod, this->mTimeResolutionUnit, mResetEvent);
                            if (GetResetStatus() || (mUHSBPeriod == 0)){
                                throw vpcl::emRstException;
                            }else{
                                re_printf("info", "Finish filter on channel %d.\n", ch_id);
                            }
                        }
                        mDFE0051Func->SetVALIDBit(ch_id, 0);//Clear VALID when start Accumulation unit
                        
                        //=======Wait 2 cycles for "Accumulation and output data" as IP spec. chapter 8.3.1 // 
                        // Accumumalation/Decimation if any
                        bool is_accum_unit_finished = false;
                        if (mStatusAccumDeciEnable[ch_id]){
                            is_accum_unit_finished = AccumDeciProcess(ch_id);
                        }
                        // Subtraction
                        bool is_sub_unit_finished = SubtractProcess(ch_id);
                        
                        // Peak-hold operate when enable and not masked.
                        bool is_peak_update = false;
                        bool is_compare_matched = false;
                        unsigned int prcsb = mDFE0051Func->GetPRCSBBit(ch_id);
                        unsigned int ph23enable = mDFE0051Func->GetPH23PENBit();
                        unsigned int chs = mDFE0051Func->GetCHSBit();
                        unsigned int ph23st = mDFE0051Func->GetPH23STBit(ch_id);
                        unsigned int phase = 0;
                        if ((ph23enable == 1) && (ch_id == chs)){
                            if (ph23st == 0){
                                phase = 1;
                            }else{
                                phase = ph23st;
                            }
                        }else {
                            phase = 1; // Normal case, get PHPS from DFE0051jCTLBCH
                        }
                        mDFE0051Func->SetPrePHCH(phase, ch_id);//Back up pre PHCH

                        if (is_accum_unit_finished){
                            if ((ph23enable == 1) && (ch_id == chs)){
                                if (mStatusPHEnable[ch_id]){
                                    is_peak_update = PeakProcess(ch_id);
                                    is_compare_matched = CompareProcess(ch_id);
                                }
                            }else{
                                if (prcsb == 1){
                                    if (mStatusPHEnable[ch_id]){
                                        is_peak_update = PeakProcess(ch_id);
                                    }
                                }else if (prcsb == 2){
                                    is_compare_matched = CompareProcess(ch_id);
                                }else if (prcsb == 3){
                                    if (mStatusPHEnable[ch_id]){
                                        is_peak_update = PeakProcess(ch_id);
                                    }
                                    is_compare_matched = CompareProcess(ch_id);
                                }// prcsb == 0 --> no PH, no Compare
                            }
                        }
                        // Update status if disable ph
                        if (mIsDisablePHTrg[ch_id] && !mStatusPHEnable[ch_id]){
                            if (((ph23enable == 1) && (ch_id == chs)) || (prcsb == 3)){
                                mDFE0051Func->SetPHEBit(ch_id, 1);
                            }
                            mIsDisablePHTrg[ch_id] = false;
                        }

                        wait(2 * (double)mUHSBPeriod, this->mTimeResolutionUnit, mResetEvent);
                        if (GetResetStatus() || (mUHSBPeriod == 0)){
                            throw vpcl::emRstException;
                        }else{
                            re_printf("info", "Finish process output data on channel %d.\n", ch_id);
                        }

                        // Output data of Accumulation unit (raise interrupt inside)
                        if ((is_accum_unit_finished) && !(mDFE0051Func->IsDecimateEnable(ch_id) && (mDFE0051Func->GetCAENBit(ch_id) == 3))){
                            mOutputValueEvent.notify();
                        }
                        // Raise interrupt of Subtraction unit
                        for (unsigned int sub_id = 0; sub_id < 3; sub_id++){
                            if (mINTDFE0051SUBOUT[sub_id]){
                                mRaiseINTDFE0051SUBOUTEvent.notify();
                                break;
                            }
                        }
                        // Output Peak update notification
                        if(is_peak_update){
                            mRaiseDFE0051PHUPDEvent.notify();
                        }
                        // Raise compare matched interrupt
                        if (is_compare_matched){
                            unsigned int prcsb = mDFE0051Func->GetPRCSBBit(ch_id);
                            unsigned int cnsle = mDFE0051Func->GetCNSLEBit(ch_id);
                            unsigned int cnsl = mDFE0051Func->GetCNSLBit(ch_id);
                            if (prcsb == 3){
                                if (cnsle == 0){
                                    mINTDFE0051CND0[ch_id] = true;
                                }else{//cnsle == 1
                                    if (cnsl == 0){
                                        mINTDFE0051CND0[ch_id] = true;
                                    }else if (ch_id <4){//cnsl == 1 and ch_id < 4
                                        mINTDFE0051CND1[ch_id] = true;
                                    }// no INTDFE0051CND1 for case cnsl = 1 and ch_id >=4 due to range of CND1[4]
                                }
                            }else{
                                mINTDFE0051CND0[ch_id] = true;
                            }
                        }
                        if (mINTDFE0051CND0[ch_id]){
                            mRaiseINTDFE0051CND0Event.notify();
                        }
                        if (ch_id < 4){
                            if (mINTDFE0051CND1[ch_id]){
                                mRaiseINTDFE0051CND1Event.notify();
                            }
                        }

                        // Cascade (decimation is higher priority than subtraction)
                        if (is_accum_unit_finished && (!(mDFE0051Func->IsAccumEnable(ch_id))) && (mDFE0051Func->GetCAENBit(ch_id) >=2)){
                            DeciCascadeProcess(ch_id);
                        }
                        if (is_sub_unit_finished){
                            SubCascadeProcess(ch_id);
                        }
                    }

                    // Off channel after done
                    mChannelInfoTable[ch_id].active = false;
                }

                wait();
                if (GetResetStatus() || (mUHSBPeriod == 0) || (mHSBPeriod == 0)){
                    throw vpcl::emRstException;
                }
            }
        }
        catch (vpcl::eResetException){
        }
    }
}//}}}

/// Check valid between input source and channel number
template <unsigned int NUM_CHANNEL>
bool Cdfe0051<NUM_CHANNEL>::CheckAssignChannelValid(eInputSource source, unsigned int dest_channel)
{//{{{
    bool ret = mWrapper->CheckChannelConnectMatrix(this->basename(), (unsigned int)source, dest_channel);
    if (!ret){
        re_printf("warning","The input from %s is invalid to channel %d.\n", mInputSourceNameMap[source].c_str(), dest_channel);
    }
    return ret;
}//}}}

//========================================================
//============ Filter circuit ============================
//========================================================
/// Filter process, call FIR/IIR according
template <unsigned int NUM_CHANNEL>
bool Cdfe0051<NUM_CHANNEL>::Filter(unsigned int channel_id)
{//{{{
    unsigned int filter_kind = mDFE0051Func->GetCMDBit(channel_id);
    sc_uint<16> in_data      = mChannelInfoTable[channel_id].in_data;

    // Copy initial buffer to circular buffer for first time calculation (p3654)
    if (mIsFilterInit[channel_id]){
        if (((unsigned int)emIIR1Stage <= filter_kind ) && (filter_kind <= (unsigned int)emIIRGain3Stage)){
            mDFE0051Func->CopyInitBufToCirBuf(channel_id);
        }
        mDFE0051Func->StoreIntermediateValue(channel_id, 0, 0);
        mIsFilterInit[channel_id] = false;
        mDFE0051Func->SetFITSBit(channel_id, 0);// Clear trigger history when finish init filter circuit
    }

    // Check setting filter valid
    if ((filter_kind == emFIR64Tap) && (((channel_id) % 2) != 0)){
        re_printf("error", "Cannot proceed FIR 64-tap with odd channel %d.\n", channel_id);
        return false;
    }

    // FIR
    if (((unsigned int)emFIR8Tap <= filter_kind) && (filter_kind <= (unsigned int)emFIR64Tap)){
        unsigned int num_tap = 8 * (filter_kind + 1);
        mFilterOutData = FilterFIR(channel_id, (short)in_data, num_tap);
    }

    // IIR no GAIN
    if (((unsigned int)emIIR1Stage <= filter_kind) && (filter_kind <= (unsigned int)emIIR3Stage)){
        unsigned int num_stage = filter_kind - (unsigned int)emIIR1Stage + 1;
        mFilterOutData = FilterIIR(channel_id, (unsigned short)in_data, num_stage, false); // false means no GAIN
    }

    // IIR with GAIN
    if (((unsigned int)emIIRGain2Stage <= filter_kind) && (filter_kind <= (unsigned int)emIIRGain3Stage)){
        unsigned int num_stage = filter_kind - (unsigned int)emIIRGain2Stage + 2;
        mFilterOutData = FilterIIR(channel_id, (unsigned short)in_data, num_stage, true); // true means GAIN
    }

    return true;
}//}}}

/// Get processed cycles need to finish a filter
template <unsigned int NUM_CHANNEL>
unsigned int Cdfe0051<NUM_CHANNEL>::GetFilterProcCycle(unsigned int filter_kind, bool with_initialize)
{//{{{
    unsigned int ret = 0;
    if (with_initialize){
        ret = mInitFilterProcCycle[(eFilterKind)filter_kind];
    }else{
        ret = mNormalFilterProcCycle[(eFilterKind)filter_kind];
    }
    return ret;
}//}}}

/// FIR filter
template <unsigned int NUM_CHANNEL>
int32_t Cdfe0051<NUM_CHANNEL>::FilterFIR(unsigned int channel_id, unsigned short xn, unsigned int num_taps)
{//{{{
    int64_t y = 0; // use 64bit instead of 54bit
    int64_t ypre = 0;

    // Convert data input x(n) and calculate y(n)
    unsigned short c0 = mDFE0051Func->GetCoefficient(channel_id, 0);
    // Calculate x(n) * C0
    if (mDFE0051Func->IsFIRFixedPoint(channel_id)){
        y = (int64_t)MulQ15Q15(channel_id, xn, c0);
    }else{
        y = (int64_t)MulInt(channel_id, xn, c0);
    }
    // Sum all
    for (unsigned int i = 1; i < num_taps; i++){ // x(n-1)*c1 + ....
        unsigned short xpre = mDFE0051Func->GetFIRBuffer(channel_id, num_taps);
        unsigned short cpre = mDFE0051Func->GetCoefficient(channel_id, i);
        if (mDFE0051Func->IsFIRFixedPoint(channel_id)){
            ypre = (int64_t)MulQ15Q15(channel_id, xpre, cpre);
        }else{
            ypre = (int64_t)MulInt(channel_id, xpre, cpre);
        }
        y += ypre;
    }

    // Shift pointer in circular buffer
    mDFE0051Func->UpdateFIRBuffer(channel_id, xn, num_taps);

    // Store Intermediate value output for even channel
    if ((channel_id % 2) == 0){
        StoreIntermediateValue(channel_id, y);
    }

    int32_t ret = SaturateTo32bit(y);
    if (y != (int64_t)ret){
        mDFE0051Func->SetGERBit(channel_id, 1);
    }
    return ret;
}//}}} 

/// IIR filter
template <unsigned int NUM_CHANNEL>
int32_t Cdfe0051<NUM_CHANNEL>::FilterIIR(unsigned int channel_id, unsigned short xn, unsigned int num_stage, bool is_gain)
{//{{{
    int64_t y = 0;
    // Update multiply error if any
    unsigned short b00 = mDFE0051Func->GetCoefficient(channel_id, 0);
    if ((b00 == 0x8000) && (xn == 0x8000)){
        mDFE0051Func->SetMERBit(channel_id, 1);
    }
    // Calculate case 1 stage
    if (num_stage == 1){
        int16_t a11 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 1));
        int16_t a12 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 2));
        int16_t b11 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 3));
        int16_t b12 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 4));
        int16_t b10 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 5));
        int32_t w1 = (int32_t)(mDFE0051Func->GetIIRCirBuffer(channel_id, 0));// w(n-1)
        int32_t w2 = (int32_t)(mDFE0051Func->GetIIRCirBuffer(channel_id, 1));// w(n-2)

        int32_t input_acc = 0;
        int16_t xn16 = (int16_t)xn;
        int32_t xn32 = (int32_t)xn16 <<16;
        y = CalBasicIIR(input_acc, xn32, w1, w2, (int16_t)b00, b10, a11, b11, a12, b12, true,channel_id);
        mDFE0051Func->UpdateIIRBufferOneStage(channel_id, (unsigned int)input_acc);
    // Calculate case 2 stages
    }else if (num_stage == 2){
        int16_t a11 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 1));
        int16_t a12 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 2));
        int16_t b11 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 3));
        int16_t b12 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 4));
        int16_t a21 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 5));
        int16_t a22 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 6));
        int16_t b10 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 7));
        int16_t b21 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 8));
        int16_t b22 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 9));
        int16_t b20 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 10));
        int32_t w11 = (int32_t)(mDFE0051Func->GetIIRCirBuffer(channel_id, 0));
        int32_t w12 = (int32_t)(mDFE0051Func->GetIIRCirBuffer(channel_id, 1));
        int32_t w21 = (int32_t)(mDFE0051Func->GetIIRCirBuffer(channel_id, 2));
        int32_t w22 = (int32_t)(mDFE0051Func->GetIIRCirBuffer(channel_id, 3));
        int16_t gain2 = 1;
        int32_t input_acc1 = 0;
        int32_t input_acc2 = 0;
        if (is_gain){
            gain2 = (int16_t)(mDFE0051Func->GetGAIN2Bit(channel_id));
        }
        int16_t xn16 = (int16_t)xn;
        int32_t xn32 = (int32_t)xn16 <<16;
        int64_t yphase1 = CalBasicIIR(input_acc1, xn32, w11, w12, (int16_t)b00, b10, a11, b11, a12, b12, true, channel_id);
        y = CalBasicIIR(input_acc2, (int32_t)(yphase1), w21, w22, gain2, b20, a21, b21, a22, b22, is_gain, channel_id);
        mDFE0051Func->UpdateIIRBufferTwoStage(channel_id, input_acc1, input_acc2);

    // Calculate case 3 stages
    }else{
        int16_t a11 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 1));
        int16_t a12 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 2));
        int16_t b11 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 3));
        int16_t b12 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 4));
        int16_t a21 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 5));
        int16_t a22 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 6));
        int16_t b10 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 7));
        int16_t b21 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 8));
        int16_t b22 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id, 9));
        int16_t a31 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id,10));
        int16_t a32 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id,11));
        int16_t b20 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id,12));
        int16_t b31 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id,13));
        int16_t b32 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id,14));
        int16_t b30 = (int16_t)(mDFE0051Func->GetCoefficient(channel_id,15));
        int32_t w11 = (int32_t)(mDFE0051Func->GetIIRCirBuffer(channel_id, 0));
        int32_t w12 = (int32_t)(mDFE0051Func->GetIIRCirBuffer(channel_id, 1));
        int32_t w21 = (int32_t)(mDFE0051Func->GetIIRCirBuffer(channel_id, 2));
        int32_t w22 = (int32_t)(mDFE0051Func->GetIIRCirBuffer(channel_id, 3));
        int32_t w31 = (int32_t)(mDFE0051Func->GetIIRCirBuffer(channel_id, 4));
        int32_t w32 = (int32_t)(mDFE0051Func->GetIIRCirBuffer(channel_id, 5));
        int16_t gain2 = 1;
        int16_t gain3 = 1;
        int32_t input_acc1 = 0;
        int32_t input_acc2 = 0;
        int32_t input_acc3 = 0;
        if (is_gain){
            gain2 = (int16_t)(mDFE0051Func->GetGAIN2Bit(channel_id));
            gain3 = (int16_t)(mDFE0051Func->GetGAIN3Bit(channel_id));
        }
        int16_t xn16 = (int16_t)xn;
        int32_t xn32 = (int32_t)xn16 <<16;
        int64_t yphase1 = CalBasicIIR(input_acc1, xn32, w11, w12, (int16_t)b00, b10, a11, b11, a12, b12, true, channel_id);
        int64_t yphase2 = CalBasicIIR(input_acc2, (int32_t)yphase1, w21, w22, gain2, b20, a21, b21, a22, b22, is_gain, channel_id);
        y = CalBasicIIR(input_acc3, (int32_t)yphase2, w31, w32, gain3, b30, a31, b31, a32, b32, is_gain, channel_id);
        mDFE0051Func->UpdateIIRBufferThreeStage(channel_id, input_acc1, input_acc2, input_acc3);
    }

    // Store 0 to Intermediate value output
    StoreIntermediateValue(channel_id, y);
    
    if ((y > 0xFFFFFFFFLL) || (y < -0x100000000LL)){
        mDFE0051Func->SetGERBit(channel_id, 1);
    }
    return SaturateTo32bit(y);
}//}}}

/// Calculate a basic (1 stage) of IIR
template <unsigned int NUM_CHANNEL>
int64_t Cdfe0051<NUM_CHANNEL>::CalBasicIIR(int32_t& input_acc, int32_t x, int32_t w1, int32_t w2, 
    int16_t b00, int16_t b10, int16_t a11, int16_t b11, int16_t a12, int16_t b12, bool is_gain,  unsigned int channel)
{//{{{
    int64_t input = 0;
    int64_t output = 0;

    // Calculate input path
    int64_t in1 = 0;
    if (is_gain){
        in1 = MulQ114Q31RtnQ31(b00, x, channel);
    }else{
        in1 = (int64_t)x;
    }

    int64_t in2 = MulQ114Q31RtnQ31(a11, w1, channel);
    int64_t in3 = MulQ114Q31RtnQ31(a12, w2, channel);
    input = in1 + in2 + in3;

    // Calculate output path
    int64_t out1 = MulQ114Q31RtnQ31(b10, (int32_t)input, channel);
    int64_t out2 = MulQ114Q31RtnQ31(b11, w1, channel);
    int64_t out3 = MulQ114Q31RtnQ31(b12, w2, channel);
    output = out1 + out2 + out3;
    
    input_acc = (int32_t)input;// For update circular buffer
    return output;
}//}}}

/// Multiply S1.14 with S.31 and return S.31 in IIR
template <unsigned int NUM_CHANNEL>
int64_t Cdfe0051<NUM_CHANNEL>::MulQ114Q31RtnQ31(int16_t b, int32_t x, unsigned int channel)
{//{{{
    // Update error bit if any
    unsigned short bcvt = (unsigned short)b;
    unsigned int xcvt = (unsigned int)x;
    if ((bcvt == 0x8000) && (((xcvt & 0xFFFF) == 0x8000) || ((xcvt >> 16) == 0x8000))){
        mDFE0051Func->SetMERBit(channel, 1);
    }
    int64_t result = (int64_t)b * (int64_t)x;
    result = (result << 2) >> (14+2);// Q29 << 2 = Q31
    return result;
}//}}}

/// Multiply S.15 and S.15 (input is unsigned short to check error case)
template <unsigned int NUM_CHANNEL>
int32_t Cdfe0051<NUM_CHANNEL>::MulQ15Q15(unsigned int channel, unsigned short a, unsigned short b)
{//{{{
    // Update error bit if any
    if ((a == 0x8000) && (b == 0x8000)){
        mDFE0051Func->SetMERBit(channel, 1);
    }
    int16_t a_cvt = (int16_t)a;
    int16_t b_cvt = (int16_t)b;
    int32_t result  = (int32_t)a_cvt * (int32_t)b_cvt;
    return (result << 1);// Q2.30 left shift 1 bit to Q31
}//}}} 

/// Multiply integer
template <unsigned int NUM_CHANNEL>
int32_t Cdfe0051<NUM_CHANNEL>::MulInt(unsigned int channel, unsigned short a, unsigned short b)
{//{{{
    // Update error bit if any
    if ((a == 0x8000) && (b == 0x8000)){
        mDFE0051Func->SetMERBit(channel, 1);
    }
    int16_t a_cvt = (int16_t)a;
    int16_t b_cvt = (int16_t)b;
    int32_t result  = (int32_t)a_cvt * (int32_t)b_cvt;
    return result;
}//}}}

/// Saturate output of filter to 32 bit 
template <unsigned int NUM_CHANNEL>
int32_t Cdfe0051<NUM_CHANNEL>::SaturateTo32bit(int64_t input)
{//{{{
    int32_t ret = 0;
    if (input > 0x7FFFFFFFLL){
        ret = 0x7FFFFFFF;
    }else if (input < -0x80000000LL){
        ret = -0x80000000;
    }else{
        ret = (int32_t)input;
    }
    return ret;
}//}}}

//========================================================
//============ Intermediate value block===================
//========================================================
/// Store intermediate value
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::StoreIntermediateValue(unsigned int channel_id, int64_t y)
{//{{{
    // Store 0 to intermediate registers if IIR
    if (mDFE0051Func->IsIIRSelect(channel_id)){
        mDFE0051Func->StoreIntermediateValue(channel_id, 0, 0);
    }else{
        // Store value if FIR
        if (mDFE0051Func->IsIntermediateConvertFloat(channel_id)){// Convert to floating point if any
            unsigned int converted_value = Convert2Float((unsigned long long)y);
            mDFE0051Func->StoreIntermediateValue(channel_id, 0, converted_value);// high value is 0, just store the lower value
        }else{// Store as fixed point if HOFT = 0
            if (mDFE0051Func->IsFIRFixedPoint(channel_id)) {
                unsigned int sign = 0;
                if ((y & 0x8000000000000000LL) == 0x8000000000000000LL){
                    sign = 0x80000000;
                }
                unsigned int high_bits = (unsigned int)((y >> 31) & 0x3F);// Get 6 integer bits
                unsigned int low_bits = (unsigned int)(y & emData32Mask) | sign; // Get S.31
                mDFE0051Func->StoreIntermediateValue(channel_id, high_bits, low_bits);
            }else{
                unsigned int high_bits = (unsigned int)((y >> 32) & 0x3F);
                unsigned int low_bits = (unsigned int)(y & 0xFFFFFFFF);
                mDFE0051Func->StoreIntermediateValue(channel_id, high_bits, low_bits);
            }
        }
    }
}//}}}

/// Convert from fix point to Floating point (input format S9.22)
template <unsigned int NUM_CHANNEL>
unsigned int Cdfe0051<NUM_CHANNEL>::Convert2Float(unsigned long long data)
{//{{{
    unsigned int converted_val = 0;
    unsigned int tmp_exp = 0;
    unsigned int exp_val = 0;
    unsigned int fraction_val = 0;
    unsigned long long value = data & 0x7FFFFFFFFFFFFFFFLL;
    unsigned long long sign = data & 0x8000000000000000LL;
    // Process signed bit
    if (sign == 0x8000000000000000LL) {
        converted_val = emSign32Mask;
        value = 0x8000000000000000LL - value;
    }

    if (value != 0) {
        unsigned int num_data_bit = 64 - 1;
        for (unsigned int i=0; i<=num_data_bit; i++) { // "<=" compare is needed for next comparasion
            if (((unsigned long long)(1LL<<i)) < value) {
                tmp_exp = i;
            } else {
                break;
            }
        }
        exp_val = tmp_exp+127-54;// Process Exponent // tmp_exp = (EXP - 127) + 54 ==> EXP = tmp_exp + 127 - 54
        converted_val = converted_val | ((exp_val<<23)&emExpMask);

        // Process Fraction // value = 2^exp + ((FRACTION * 2^-23) * 2^exp) ==> FRACTION = (value - 2^exp)*(2^(23-exp))
        if (tmp_exp >= 23){
            fraction_val = (unsigned int)((value-(unsigned long long)(1LL<<tmp_exp))>>(tmp_exp - 23));
        }else{
            fraction_val = (unsigned int)((value-(unsigned long long)(1LL<<tmp_exp))<<(23 - tmp_exp));
        }
        converted_val = converted_val | (fraction_val & emFractionMask);
    }
    return converted_val;
}//}}}

//========================================================
//============ Accumulation/Decimation ===================
//========================================================
/// Accumulation/Decimation process
template <unsigned int NUM_CHANNEL>
bool Cdfe0051<NUM_CHANNEL>::AccumDeciProcess(unsigned int channel)
{//{{{
    bool ret = false;
    bool is_accumulation_unit_run = false;
    int32_t pro_data = mFilterOutData;

    // Absolute data if any
    if (mDFE0051Func->IsAbsolutePerform(channel)){
        pro_data = Absolute(channel, mFilterOutData);
    }

    // Accumulate/Decimate if any (Accumulate/Decimate output data by themselves)
    if (mDFE0051Func->IsAccumEnable(channel)){
        is_accumulation_unit_run = true;
        ret = Accumulate(channel, pro_data);
        if (ret){
            mIsOutFromAccum[channel] = true;
        }
    }else if (mDFE0051Func->IsDecimateEnable(channel)){
        is_accumulation_unit_run = true;
        ret = Decimate(channel, pro_data);
        if (ret){
            mIsOutFromAccum[channel] = false;
        }
    }

    if (!is_accumulation_unit_run && (mDFE0051Func->GetPRCSABit(channel) == 0)){// Issue output if Accumulate/Decimate not run
        mOutChannelNum = channel;
        mOutValue = pro_data;
        mIsOutFromAccum[channel] = false;
        ret = true;
    }
    return ret;
}//}}}

/// Accumulate input value
template <unsigned int NUM_CHANNEL>
bool Cdfe0051<NUM_CHANNEL>::Accumulate(unsigned int channel, int32_t pro_data)
{//{{{
    re_printf("info", "Start Accumulation on channel %d.\n", channel);
    bool ret = false;
    unsigned int fmt = mDFE0051Func->GetFMTBit(channel);
    // Expected count
    mExpectedCount[channel] = mDFE0051Func->GetAccumExpectedCount(channel);
    // Accum
    mAccumVal[channel] = Accum3232(channel, mAccumVal[channel], pro_data);// Right shift 9 bit is done in Accum3232() ==> S31->S31, S9.22->S.31
    mCount[channel]++;
    if (mCount[channel] > mExpectedCount[channel]){
        // Output accum value - NO cascade for accum
        mOutChannelNum = channel;
        mOutValue = mAccumVal[channel];
        mCount[channel] = 0;
        mAccumVal[channel] = 0;
        ret = true;
    }
    return ret;
}//}}}

/// Plus 2 number in Accumumalation circuit
template <unsigned int NUM_CHANNEL>
int32_t Cdfe0051<NUM_CHANNEL>::Accum3232(unsigned int channel, int32_t accum, int32_t addition)
{//{{{
    int64_t plus64 = (int64_t)accum + ((int64_t)addition >> 9);
    int32_t sat_val = SaturateTo32bit(plus64);
    return sat_val;
}//}}}

/// Absolute input value
template <unsigned int NUM_CHANNEL>
unsigned int Cdfe0051<NUM_CHANNEL>::Absolute(unsigned int channel_id, unsigned int input)
{//{{{
    unsigned int ret = input;
    if (input == 0x80000000){
        ret = 0x7FFFFFFF;
        mDFE0051Func->SetAERBit(channel_id, 1);
        re_printf("info", "Absolution error occurs in channel %d.\n", channel_id);
    }else if ((input & emSign32Mask) == emSign32Mask){
        ret = ~input + 1;
    }
    return ret;
}//}}}

/// Decimate input value
template <unsigned int NUM_CHANNEL>
bool Cdfe0051<NUM_CHANNEL>::Decimate(unsigned int channel, int32_t pro_data)
{//{{{
    re_printf("info", "Start Decimation on channel %d.\n", channel);
    bool ret = false;

    // Expected count
    mExpectedCount[channel] = mDFE0051Func->GetAccumExpectedCount(channel);
    mCount[channel]++;
    if (mCount[channel] > mExpectedCount[channel]){
        // Output accum value - NO cascade for accum
        mOutChannelNum = channel;
        mOutValue = pro_data;
        mCount[channel] = 0;
        ret = true;
    }
    return ret;
}//}}}

/// Cascade Decimation output value
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::DeciCascadeProcess(unsigned int channel)
{//{{{
    unsigned int cascade_tag = mDFE0051Func->GetDeciCATAGBit(channel);
    unsigned int fmt = mDFE0051Func->GetFMTBit(channel);
    int16_t cascade_data = 0;
    bool cascade_error = false;

    if (fmt == 1){// Integer
        cascade_error = ZipInt32To16(mOutValue, cascade_data);
    }else{ // Fixed point
        cascade_error = ZipFix32To16(mOutValue, cascade_data);
    }
    if (cascade_error){
        mDFE0051Func->SetCERBit(channel, 1);
    }
    // Cascade
    mInputSourceTable[emDeciCas].StoreInfo(cascade_tag, cascade_data, true);
    mDispatchEvent.notify();
}//}}}

//========================================================
//============ Output value unit       ===================
//========================================================
/// Output value and trigger method to issue data outside
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::OutputValueMethod()
{//{{{
    unsigned int pro_data = mOutValue;
    if (mDFE0051Func->IsFPConvertOutputValue(mOutChannelNum)){
        unsigned long long outputs954 = (unsigned long long)mOutValue << (32-9);
        if (mIsOutFromAccum[mOutChannelNum]){
            outputs954 = (unsigned long long)mOutValue << 32;
        }
        pro_data = Convert2Float(outputs954);
    }
    // Store output data reg
    mDFE0051Func->SetDOCHReg(mOutChannelNum, pro_data);
    // Set status bit and issue interrupt, output data
    if ((mDFE0051Func->IsAccumEnable(mOutChannelNum)) || (mDFE0051Func->IsDecimateEnable(mOutChannelNum))){
        if (mDFE0051Func->GetDOENBit(mOutChannelNum) == 1){
            mDFE0051Func->SetDOOWBit(mOutChannelNum, 1);
        }else{
            mDFE0051Func->SetDOENBit(mOutChannelNum, 1);
            if (mDFE0051Func->GetIEOBit(mOutChannelNum) == 1){
                mRaiseINTDFE0051DOUTEvent.notify();
            }
        }
        mIssueOutputDataEvent.notify();
    }
}//}}}

/// Issue output data
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::IssueOutputDataMethod()
{//{{{
    unsigned int outdata = mDFE0051Func->GetDOCHReg(mOutChannelNum);
    mDFE0051_DFDOUT[mOutChannelNum] = outdata;
    mWriteDFE0051_DFDOUTEvent.notify();
    re_printf("info", "Channel[%d] outputs data 0x%08X to FIFO.\n", mOutChannelNum, outdata);
    mDFE0051_DOUPD_N[mOutChannelNum] = false;
    mWriteDFE0051_DOUPD_NEvent.notify();
    mDFE0051_DOUT_END[mOutChannelNum] = true;
    mWriteDFE0051_DOUT_ENDEvent.notify();
    mNegateDOUTENDEvent.notify((double)mHSBPeriod, this->mTimeResolutionUnit);
}//}}}

/// Negate DFE0051_DOUT_END
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::NegateDOUTENDMethod()
{//{{{
    mDFE0051_DOUT_END[mOutChannelNum] = false;
    mWriteDFE0051_DOUT_ENDEvent.notify();
}//}}}

/// Write DFE0051_DOUT_END
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::WriteDFE0051_DOUT_ENDMethod()
{//{{{
    DFE0051_DOUT_END[mOutChannelNum]->write(mDFE0051_DOUT_END[mOutChannelNum]);
}//}}}

/// Clear DFE0051_DFDOUT
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::ClearDFDOUTMethod()
{//{{{
    mDFE0051_DFDOUT[mOutChannelNum] = 0;
    mWriteDFE0051_DFDOUTEvent.notify();
}//}}}

/// Write DFE0051_DFDOUT
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::WriteDFDOUTMethod()
{//{{{
    DFE0051_DFDOUT[mOutChannelNum]->write(mDFE0051_DFDOUT[mOutChannelNum]);
}//}}}

/// Assert DFE0051_PHUPD
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::RaiseDFE0051PHUPDMethod()
{//{{{
    mWriteDFE0051PHUPDEvent.notify();
    mNegateDFE0051PHUPDEvent.notify((double)mHSBPeriod, this->mTimeResolutionUnit);
}//}}}

/// Negate DFE0051_PHUPD
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::NegateDFE0051PHUPDMethod()
{//{{{
    for (unsigned int i = 0; i < 4; i++){
        mDFE0051PHUPD[i] = false;
    }
    mWriteDFE0051PHUPDEvent.notify();
}//}}}

/// Write DFE0051_PHUPD
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::WriteDFE0051PHUPDMethod()
{//{{{
    for (unsigned int i = 0; i < 4; i++){
        DFE0051_PHUPD[i]->write(mDFE0051PHUPD[i]);
    }
}//}}}

/// Monitor DFE0051_FIFO_CACK
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::MonitorFIFOACKMethod()
{//{{{
    for (unsigned int channel = 0; channel < NUM_CHANNEL; channel++){
        if ((DFE0051_FIFO_CACK[channel]->read()) && (mDFE0051Func->GetDOENBit(channel) == 1)){
            mDOUTN[channel] = true;
            mDeassertDOUTNEvent.notify((double)mHSBPeriod, this->mTimeResolutionUnit);
        }
    }
}//}}}

/// Negate DFE0051_DOUPD_N
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::DeassertDOUTNMethod()
{//{{{
    for (unsigned int channel = 0; channel < NUM_CHANNEL; channel++){
        if (mDOUTN[channel]){
            mDFE0051_DOUPD_N[channel] = true;
            mDFE0051Func->SetDOENBit(channel, 0);// Clear DOEN
        }
    }
    mWriteDFE0051_DOUPD_NEvent.notify();
}//}}}

/// Write DFE0051_DOUPD_N
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::WriteDFE0051_DOUPD_NMethod()
{//{{{
    for (unsigned int channel = 0; channel < NUM_CHANNEL; channel++){
        DFE0051_DOUPD_N[channel]->write(mDFE0051_DOUPD_N[channel]);
    }
}//}}}

/// Assert INTDFE0051DOUT
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::RaiseINTDFE0051DOUTMethod()
{//{{{
    mINTDFE0051DOUT[mOutChannelNum] = 1;
    mWriteINTDFE0051DOUTEvent.notify();
    mNegateINTDFE0051DOUTEvent.notify((double)mUHSBPeriod *2, this->mTimeResolutionUnit);// Interrupt DOUT last 2 cycles
}//}}}

/// Negate INTDFE0051DOUT
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::NegateINTDFE0051DOUTMethod()
{//{{{
    mINTDFE0051DOUT[mOutChannelNum] = 0;
    mWriteINTDFE0051DOUTEvent.notify();
}//}}}

/// Write INTDFE0051DOUT
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::WriteINTDFE0051DOUTMethod()
{//{{{
    INTDFE0051DOUT[mOutChannelNum]->write(mINTDFE0051DOUT[mOutChannelNum]);
}//}}}


//========================================================
//============ Subtraction unit        ===================
//========================================================
/// Subtraction process
template <unsigned int NUM_CHANNEL>
bool Cdfe0051<NUM_CHANNEL>::SubtractProcess(unsigned int channel)
{//{{{
    bool ret = false;
    for (unsigned int sub_id = 0; sub_id < 3; sub_id++){
        // Not run if setting invalid curiously
        if (!(mDFE0051Func->CheckSubValidSetting(sub_id))){
            continue;
        }
        // ==> Continue next sub_id is not trigger or not enable
        if (!mStatusSubEnable[sub_id] || (mSubtractionEnable[sub_id] == 0)){
            continue;
        }

        // ==> Process if OK
        re_printf("info", "Start Subtraction for channel %d.\n", channel);
        unsigned int minuend_num = mDFE0051Func->GetMinuendChannelNum(sub_id);
        unsigned int subtrahend_num = mDFE0051Func->GetSubtrahendChannelNum(sub_id);
        bool is_sub_done = false;
        // Check format of subtrahend and minuend same
        unsigned int fmt_minuend = mDFE0051Func->GetFMTBit(minuend_num);
        unsigned int fmt_subtrahend = mDFE0051Func->GetFMTBit(subtrahend_num);
        if (fmt_minuend != fmt_subtrahend){
            re_printf("warning", "Format of subtrahend channel %d differs format of minuend channel %d.\n", fmt_subtrahend, fmt_minuend);
        }

        if (channel == minuend_num){// Minuend
            if ((mDFE0051Func->GetSUBFBit(sub_id) == 1) && (!mIsSubClear[sub_id])){
                mSubValue[sub_id] = Sub3232(sub_id, mFilterOutData, mSubtrahend[sub_id]);
                is_sub_done = true;
            }else{
                mMinuend[sub_id] = mFilterOutData;
                mDFE0051Func->SetMINFBit(sub_id, 1);
                mDFE0051Func->SetSUBFBit(sub_id, 0);
                mIsSubClear[sub_id] = false;
            }
        }else if (channel == subtrahend_num){// Subtrahend
            if ((mDFE0051Func->GetMINFBit(sub_id) == 1) && (!mIsSubClear[sub_id])){
                mSubValue[sub_id] = Sub3232(sub_id, mMinuend[sub_id], mFilterOutData);
                is_sub_done = true;
            }else{
                mSubtrahend[sub_id] = mFilterOutData;
                mDFE0051Func->SetSUBFBit(sub_id, 1);
                mDFE0051Func->SetMINFBit(sub_id, 0);
                mIsSubClear[sub_id] = false;
            }
        }

        // When sub done
        if (is_sub_done){
            // Output subtraction value
            if ((mDFE0051Func->GetSubCAEN(sub_id) == 0) || (mDFE0051Func->GetSubCAEN(sub_id) == 2)){
                unsigned int pro_data = mSubValue[sub_id];
                if (mDFE0051Func->IsFPConvertSubValue(sub_id)){
                    unsigned long long sub_val_s954 = (unsigned long long)mSubValue[sub_id] << (32-9);
                    pro_data = Convert2Float(sub_val_s954);
                }
                mDFE0051Func->SetSubDataOutReg(sub_id, pro_data);// Store sub output register
                if (mDFE0051Func->GetSDOENBit(sub_id) == 1){
                    mDFE0051Func->SetSDOOWBit(sub_id, 1);
                }else{
                    mDFE0051Func->SetSDOENBit(sub_id, 1);
                    mINTDFE0051SUBOUT[sub_id] = true;
                }
                mDFE0051Func->SetMINFBit(sub_id, 0);
                mDFE0051Func->SetSUBFBit(sub_id, 0);
            }
            ret = true;
        }
    }
    return ret;
}//}}}

/// Cascade Subtraction value
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::SubCascadeProcess(unsigned int channel)
{//{{{
    for (unsigned int sub_id = 0; sub_id < 3; sub_id++){
        if (mDFE0051Func->GetSubCAEN(sub_id) >= 2){
            if (mDFE0051Func->IsFPConvertSubValue(sub_id)){
                re_printf("warning", "Not proceed cascade for subtraction floating point value.\n");
            }else{
                unsigned int cascade_tag = mDFE0051Func->GetSubCATAGBit(sub_id);
                unsigned int fmt = mDFE0051Func->GetFMTBit(channel);
                int16_t cascade_data = 0;
                bool cascade_error = false;

                if (fmt == 1){// Integer
                    cascade_error = ZipInt32To16(mSubValue[sub_id], cascade_data);
                }else{ // Fixed point
                    cascade_error = ZipFix32To16(mSubValue[sub_id], cascade_data);
                }
                if (cascade_error){
                    mDFE0051Func->SetSCERBit(sub_id, 1);
                }
                // Cascade
                mInputSourceTable[(eInputSource)(sub_id + emSubCas0)].StoreInfo(cascade_tag, cascade_data, true);
                mDispatchEvent.notify();
            }
        }
    }
}//}}}

/// Subtract minuend and subtrahend
template <unsigned int NUM_CHANNEL>
int32_t Cdfe0051<NUM_CHANNEL>::Sub3232(unsigned int sub_id, int32_t minuend, int32_t subtrahend)
{//{{{
    // Integer and fixed point are processed as same way.
    int64_t sub64 = (int64_t)minuend - (int64_t)subtrahend;
    // Overflow
    int32_t sat_val = SaturateTo32bit(sub64);
    if ((int64_t)sat_val != sub64){// Guard bit has value
        mDFE0051Func->SetSGERBit(sub_id, 1);
    }
    return sat_val;
}//}}}

/// Zip a integer from 32 to 16
template <unsigned int NUM_CHANNEL>
bool Cdfe0051<NUM_CHANNEL>::ZipInt32To16(int32_t input, int16_t& output)
{//{{{
    bool err = false;
    if (input > (int32_t)0x7FFF){
        output = (int16_t)0x7FFF;
    }else if (input < (int32_t)(-0x8000)){
        output = (int16_t)(-0x8000);
    }else{
        output = (int16_t) input;
    }
    if (input != (int32_t)output){
        err = true;
    }
    return err;
}//}}}

/// Zip a fixed point from 32 to 16
template <unsigned int NUM_CHANNEL>
bool Cdfe0051<NUM_CHANNEL>::ZipFix32To16(int32_t input, int16_t& output)
{//{{{
    bool err = false;
    int64_t added_input = (int64_t)input + (int64_t)0x8000;
    if (added_input > 0x7FFFFFFFLL){
        output = 0x7FFF;
        err = true;
    }else{
        output = (int16_t)(added_input >> 16);
    }
    return err;
}//}}}

/// Assert INTDFE0051SUBOUT
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::RaiseINTDFE0051SUBOUTMethod()
{//{{{
    for (unsigned int sub_id = 0; sub_id < 3; sub_id++){
        if ((mDFE0051Func->GetSIEOBit(sub_id) == 1) && mINTDFE0051SUBOUT[sub_id]){
            mINTDFE0051SUBOUT[sub_id] = true;
        }else{
            mINTDFE0051SUBOUT[sub_id] = false;
        }
    }
    mWriteINTDFE0051SUBOUTEvent.notify();
    mNegateINTDFE0051SUBOUTEvent.notify((double)mUHSBPeriod, this->mTimeResolutionUnit);
}//}}}

/// Negate INTDFE0051SUBOUT
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::NegateINTDFE0051SUBOUTMethod()
{//{{{
    for (unsigned int sub_id = 0; sub_id < 3; sub_id++){
        mINTDFE0051SUBOUT[sub_id] = false;
    }
    mWriteINTDFE0051SUBOUTEvent.notify();
}//}}}

/// Write INTDFE0051SUBOUT
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::WriteINTDFE0051SUBOUTMethod()
{//{{{
    for (unsigned int sub_id = 0; sub_id < 3; sub_id++){
        INTDFE0051SUBOUT[sub_id]->write(mINTDFE0051SUBOUT[sub_id]);
    }
}//}}}

//========================================================
//============ Peak-hold unit          ===================
//========================================================
/// Peak-hold for both PH normal and PH23
template <unsigned int NUM_CHANNEL>
bool Cdfe0051<NUM_CHANNEL>::PeakProcess(unsigned int channel)
{//{{{
    re_printf("info", "Start Peak-hold on channel %d.\n", channel);
    bool ret = false;
    unsigned int ph23enable = mDFE0051Func->GetPH23PENBit();
    unsigned int chs = mDFE0051Func->GetCHSBit();
    unsigned int ph23st = mDFE0051Func->GetPH23STBit(channel);
    unsigned int phase = 0;
    unsigned int compare_reg = 0;
    
    // Phase
    if ((ph23enable == 1) && (channel == chs)){
        if (ph23st == 0){
            phase = 1; // PH23, get PHPS from DFE0051jCTLBCH or DFE0051jPH2CTL0 or DFE0051jPH3CTL0
        }else{
            phase = ph23st; // PH23, get PHPS from DFE0051jCTLBCH or DFE0051jPH2CTL0 or DFE0051jPH3CTL0
        }
    }else {
        phase = 1; // Normal case, get PHPS from DFE0051jCTLBCH
    }

    // Increase counter
    if (mPHCounter[channel] == emMaxPHIND){
        mPHCounter[channel] = 0;
        mDFE0051Func->SetPHIOFBit(phase, channel, 1);
    }else{
        mPHCounter[channel]++;
    }

    // Not process if mask
    if (mStatusPHMask[channel]){
        return false;
    }

    unsigned int phps = mDFE0051Func->GetPHPSBit(phase, channel);
    // Determine compare register
    if (mIsPHInit[channel]){
        // Clear some register as initialization
        if ((ph23enable == 1) && (channel == chs)){
            mDFE0051Func->SetPH20Reg(0x80000000);
            mDFE0051Func->SetPH30Reg(0x80000000);
            mDFE0051Func->SetPH2IND0Reg(0);
            mDFE0051Func->SetPH3IND0Reg(0);
            mDFE0051Func->SetPH23STBit(channel, 0);
        }else {
            mDFE0051Func->SetPHCHReg(phase, channel, 0x80000000);
            mDFE0051Func->SetPHCHPureVal(phase, channel, 0x80000000);
            mDFE0051Func->SetPHINDCHReg(channel, 0);
            mDFE0051Func->SetPH23STBit(channel, 0);
        }
        mDFE0051Func->SetPITSBit(channel, 0);// Clear when finish initialization PH
        mIsStartPHPhase[channel] = true;
    }
    if (mIsStartPHPhase[channel]){
        compare_reg = mDFE0051Func->GetPHInitVal(phase, channel);
    }else{
        compare_reg = mDFE0051Func->GetPrePHCHReg(channel);
    }

    // Compare PH matched (both upper and lower peak)
    if (((phps == 0) && (mOutValue > (int32_t)compare_reg)) || ((phps == 1) && (mOutValue < (int32_t)compare_reg))){
        // Check corresponding PHUPD => raise PH Update port
        for (unsigned int phuid = 0; phuid < 4; phuid++){
            unsigned int phuphch = mDFE0051Func->GetPHUPDCHBit(phuid);
            unsigned int oephupd = mDFE0051Func->GetOEPHUPDBit(phuid);
            unsigned int ph23sl = mDFE0051Func->GetPH23SLBit(phuid);
            unsigned int ph23e = mDFE0051Func->GetPH23EBit(phuid);
            unsigned int ph23 = (ph23e << 1) | ph23sl;
            if ((channel == phuphch) && (oephupd == 1)){
                if (((phase == 1) && (ph23 < 2)) || ((phase == 2) && (ph23 == 2)) || ((phase == 3) && (ph23 == 3))){
                    mDFE0051PHUPD[phuid] = true;
                }
                ret = true;
            }
        }

        // Convert to floating point if any
        unsigned int pro_data = mOutValue;
        if (mDFE0051Func->GetPHPFMTBit(channel) == 1){
            unsigned long long outputs954 = (unsigned long long)mOutValue << (32-9);
            if (mIsOutFromAccum[channel]){
                outputs954 = (unsigned long long)mOutValue << 32;
            }
            pro_data = Convert2Float(outputs954);
        }
        // Update PH result register
        mDFE0051Func->SetPHCHReg(phase, channel, pro_data);
        mDFE0051Func->SetPHCHPureVal(phase, channel, mOutValue);
        // Increase PHIND
        mDFE0051Func->SetPHINDBit(phase, channel, mPHCounter[channel]);
    }else if (mIsStartPHPhase[channel]){
        // Convert to floating point if any
        unsigned int pro_data = compare_reg;
        if (mDFE0051Func->GetPHPFMTBit(channel) == 1){
            unsigned long long outputs954 = (unsigned long long)compare_reg << (32-9);
            if (mIsOutFromAccum[channel]){
                outputs954 = (unsigned long long)compare_reg << 32;
            }
            pro_data = Convert2Float(outputs954);
        }
        // Update PH result register for next compare- NO UPDATE PHCH, NO PHUPD NOTIFICATION
        mDFE0051Func->SetPHCHReg(phase, channel, pro_data);
        mDFE0051Func->SetPHCHPureVal(phase, channel, compare_reg);
    }

    mIsStartPHPhase[channel] = false;
    mIsPHInit[channel] = false;
    return ret;
}//}}}

/// Compare initial value with output from Filter (format S31 or S9.22)
template <unsigned int NUM_CHANNEL>
bool Cdfe0051<NUM_CHANNEL>::CompareProcess(unsigned int channel)
{//{{{
    re_printf("info", "Start Comparison on channel %d.\n", channel);
    bool ret = false;
    unsigned int ph23enable = mDFE0051Func->GetPH23PENBit();
    unsigned int ph23chs = mDFE0051Func->GetPH23CHSBit();
    unsigned int ph23st = mDFE0051Func->GetPH23STBit(channel);
    unsigned int prcsb = mDFE0051Func->GetPRCSBBit(channel);
    unsigned int cpcs = mDFE0051Func->GetCPCSBit(channel);
    unsigned int phase = 0;
    unsigned int compare_operator = 0;
    if (ph23enable == 0){
        phase = 1; // Normal case, get from ofsl from DFE0051jCTLBCH
    }else {
        if (ph23st == 0){
            phase = 1;
            mDFE0051Func->SetPH23STBit(channel, 1);
        }else{
            phase = ph23st; // PH23, get ofsl from DFE0051jCTLBCH or DFE0051jPH2CTL0 or DFE0051jPH3CTL0
        }
    }
    
    if ((ph23enable == 0) || ((channel == ph23chs) && (ph23st < 4))){
        bool is_compare_process = false;
        int32_t compare_value = 0;
        // PH and compare simultaneously
        if ((prcsb == 3) || (ph23enable == 1)){
            is_compare_process = true;
            if (cpcs == 1){
                // Get alpha (from according ofsl)
                unsigned int alpha = mDFE0051Func->GetAlpha(phase, channel);
                // Calculate compare value
                int32_t phregval = (int32_t)mDFE0051Func->GetPrePHCHReg(channel);
                compare_value = SumCompare(channel, phregval, alpha);
            }else{
                compare_value = mDFE0051Func->GetCPRegWithSELB1Bit(channel);
            }
            compare_operator = mDFE0051Func->GetCompareOperator(phase, channel);
        // Compare only
        }else if (prcsb == 2){
            is_compare_process = true;
            compare_operator = mDFE0051Func->GetCompareOperator(phase, channel);
            compare_value = mDFE0051Func->GetCPRegWithSELB1Bit(channel);
        }

        if ((is_compare_process) && (CompareValues(compare_value, mOutValue, compare_operator))){
            if (ph23enable){
                mDFE0051Func->SetPH23STBit(channel, phase + 1);
                mIsStartPHPhase[channel] = true;
                if (mDFE0051Func->GetPH23STBit(channel) == 4){
                    mDFE0051Func->SetCNDBit(channel, 1);
                    ret = true;
                }
            }else{
                mDFE0051Func->SetCNDBit(channel, 1);
                ret = true;
            }
        }
    }
    return ret;
}//}}}

/// Compare 2 value with a specified operator
template <unsigned int NUM_CHANNEL>
bool Cdfe0051<NUM_CHANNEL>::CompareValues(int32_t compare_value, int32_t output_value, unsigned int operator_value)
{//{{{ 
    if ((( operator_value == 0) && (output_value == compare_value))
       ||((operator_value == 1) && (output_value <= compare_value))
       ||((operator_value == 2) && (output_value >= compare_value))
       ||((operator_value == 3) && (output_value <  compare_value))
       ||((operator_value == 4) && (output_value >  compare_value))){
       return true;
    }
    return false;
}//}}}

/// Sum used in Compare process
template <unsigned int NUM_CHANNEL>
int32_t Cdfe0051<NUM_CHANNEL>::SumCompare(unsigned int channel, int32_t phregval, int32_t alpha)
{//{{{
    unsigned int fmt = mDFE0051Func->GetFMTBit(channel);
    int32_t sat_val = 0;
    int64_t plus64 = (int64_t)phregval + (int64_t)alpha;
    sat_val = SaturateTo32bit(plus64);
    if ((int64_t)sat_val != plus64){// Guard bit has value
        mDFE0051Func->SetGERBit(channel, 1);
    }
    return sat_val;
}//}}}

/// Assert INTDFE0051CND1
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::RaiseINTDFE0051CND1Method()
{//{{{
    mWriteINTDFE0051CND1Event.notify();
    mNegateINTDFE0051CND1Event.notify((double)mUHSBPeriod, this->mTimeResolutionUnit);
}//}}}

/// Negate INTDFE0051CND1
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::NegateINTDFE0051CND1Method()
{//{{{
    for (unsigned int channel = 0; channel < 4; channel++){
        mINTDFE0051CND1[channel] = false;
    }
    mWriteINTDFE0051CND1Event.notify();
}//}}}

/// Write INTDFE0051CND1
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::WriteINTDFE0051CND1Method()
{//{{{
    for (unsigned int channel = 0; channel < 4; channel++){
        INTDFE0051CND1[channel]->write(mINTDFE0051CND1[channel]);
    }
}//}}}

/// Assert INTDFE0051CND0
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::RaiseINTDFE0051CND0Method()
{//{{{
    mWriteINTDFE0051CND0Event.notify();
    mNegateINTDFE0051CND0Event.notify((double)mUHSBPeriod, this->mTimeResolutionUnit);
}//}}}

/// Negate INTDFE0051CND0
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::NegateINTDFE0051CND0Method()
{//{{{
    for (unsigned int channel = 0; channel < NUM_CHANNEL; channel++){
        mINTDFE0051CND0[channel] = false;
    }
    mWriteINTDFE0051CND0Event.notify();
}//}}}

/// Write INTDFE0051CND0
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::WriteINTDFE0051CND0Method()
{//{{{
    for (unsigned int channel = 0; channel < NUM_CHANNEL; channel++){
        INTDFE0051CND0[channel]->write(mINTDFE0051CND0[channel]);
    }
}//}}}


//========================================================
//============ Capture unit            ===================
//========================================================
/// Capture output value
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::CaptureMethod()
{//{{{
    for (unsigned int capid = 0; capid < 3; capid++){
        if (!(mDFE0051Func->CheckCapValidSetting(capid))){
            continue;
        }
        unsigned int capch = mDFE0051Func->GetCAPCHBit(capid);
        if ((capch == mOutChannelNum) && mCaptureEnable[capid] && (TMCAPTRG[capid]->read())){
            mDFE0051Func->CaptureToCAPDOCHReg(mOutChannelNum, capid);
        }
    }
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
/// Command to assert reset
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsResetHardActive)&&(!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (PRESETn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of PRESETn is called in the reset operation of the model. So it is ignored.\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be PRESETn.\n",reset_name.c_str());
    }
}//}}}

/// Set clock value and clock unit
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "CLK_HSB") {
            mHSBOrgFreq = freq;
            ConvertClockFreq (mHSBFreq, mHSBFreqUnit, freq, unit);
            if (mHSBFreq > 0) {
                mHSBPeriod = (sc_dt::uint64)(((1/(double)mHSBFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mHSBPeriod = 0;
            }
        } else if (word_vector[0] == "CLK_UHSB") {
            mUHSBOrgFreq = freq;
            ConvertClockFreq (mUHSBFreq, mUHSBFreqUnit, freq, unit);
            if (mUHSBFreq > 0) {
                mUHSBPeriod = (sc_dt::uint64)(((1/(double)mUHSBFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mUHSBPeriod = 0;
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Get clock value
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "CLK_HSB") {
            re_printf("info","CLK_HSB frequency is %0.0f %s\n", (double)mHSBOrgFreq, mHSBFreqUnit.c_str());
        } else if (word_vector[0] == "CLK_UHSB") {
            re_printf("info","CLK_UHSB frequency is %0.0f %s\n", (double)mUHSBOrgFreq, mUHSBFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Force value to register
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::ForceRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("force");
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            mDFE0051Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Release forced value after forcing registers
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mDFE0051Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Write value to registers by software
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::WriteRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            mDFE0051Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Read value of register by software
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mDFE0051Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// List all registers name
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mDFE0051Func->RegisterHandler(cmd);
}//}}}

/// Write value to Coefficient memory by software
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::WriteCoefficient (const unsigned int offset, const unsigned int value)
{//{{{
    if (!(mDFE0051Func->SetCoeffMem(offset, value))){
        re_printf("warning", "Should not write to the reservation area at Cofficient memory offset: 0x%X.\n", offset);
    }
}//}}}

/// Read value of Coefficient memory by software
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::ReadCoefficient (const unsigned int offset)
{//{{{
    unsigned int value = 0;
    if (mDFE0051Func->GetCoeffMem(offset, value)){
        re_printf("info", "4 bytes Coefficient memory at offset 0x%X is 0x%08X.\n", offset, value);
    }else{
        re_printf("warning", "Should not read from the reservation area at Cofficient memory offset: 0x%X.\n", offset);
    }
}//}}}

/// Write value to DataMem memory by software
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::WriteDataMem (const unsigned int offset, const unsigned int value)
{//{{{
    if (!(mDFE0051Func->SetDataMem(offset, value))){
        re_printf("warning", "Should not write to the reservation area at Data memory offset: 0x%X.\n", offset);
    }
}//}}}

/// Read value of DataMem memory by software
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::ReadDataMem (const unsigned int offset)
{//{{{
    unsigned int value = 0;
    if (mDFE0051Func->GetDataMem(offset, value)){
        re_printf("info", "4 bytes Data Memory at offset 0x%X is 0x%08X.\n", offset, value);
    }else{
        re_printf("warning", "Should not read from the reservation area at Data memory offset: 0x%X.\n", offset);
    }
}//}}}

/// Separate some words from a string to store a vector
template <unsigned int NUM_CHANNEL>
void Cdfe0051<NUM_CHANNEL>::SeparateString (std::vector<std::string> &vtr, const std::string msg)
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

template class Cdfe0051<2>;
template class Cdfe0051<4>;
template class Cdfe0051<6>;
template class Cdfe0051<8>;
template class Cdfe0051<10>;
template class Cdfe0051<12>;
template class Cdfe0051<14>;
template class Cdfe0051<16>;
template class Cdfe0051<18>;
template class Cdfe0051<20>;
// vim600: set foldmethod=marker :
