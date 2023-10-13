// ---------------------------------------------------------------------
// $Id: DSADC_UNIT.h 1398 2019-11-26 06:16:27Z chuonghoangle $
//
// Copyright(c) 2018-2019 Renesas Electronics Corporation
// Copyright(c) 2018-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __DSADC_UNIT_H__
#define __DSADC_UNIT_H__

#include "DSADC_UNIT_AgentController.h"
#include "DSADC_Base.h"
#include "re_define.h"
#include "BusSlaveBase.h"
#include "BusSlaveFuncIf.h"

class DSADC_UNIT_Func;

class DSADC_UNIT: public sc_module,
            public BusSlaveBase<32,1>,
            public DSADC_UNIT_AgentController
{

public:   

    DSADC_UNIT_Func * mDSADC_UNIT_Func;      /// DSADC_UNIT_Func class instance
    /// Internal functions
    bool IsOddDsadc(void);
    void CancelEvents(void);
    void Initialize(void);
    sc_event mStartTriggerEvent;             /// Call StartTriggerMethod() to process Start trigger
    std::string mModuleName;

    sc_event mWriteDSADDFTAGEvent;           /// Call WriteDSADDFTAGMethod() to control DSADDFTAG port
    sc_event mWriteDSADENDEvent;             /// Call WriteDSADENDMethod() to control DSADEND port
    sc_event mWriteDSADDFDATAEvent;          /// Call WriteDSADDFDATAMethod() to control DSADDFDATA port
    sc_event mWriteOutDSADIevent;
    sc_event mWriteOutDSADVCIjEvent[DSADC_Base::emVCNum];
    sc_event mWriteOutDSADEevent;
    sc_event mWriteOutDSADPEevent;
    sc_event mWriteOutDFREQ0event;
    sc_event mWriteOutDFREQ1event;
    sc_event mWriteOutDSADUEevent;
    sc_event mWriteOutDSADLEevent;

private:
    /// Declare variables
    bool mDSADEND;
    bool mDSADI;
    bool mDSADE;
    bool mDSADPE;
    bool mDSADDFREQ0;
    bool mDSADDFREQ1;
    unsigned int mDSADDFTAG;
    unsigned int mDSADDFDATA;
    bool mDSADUE;
    bool mDSADLE;
    bool mDSADVCI[DSADC_Base::emVCNum];   
    bool mIsADStopEvent;
    bool mIsTriggerProcessing;
    bool mIsContinueConvert;
    double mADConvFinishTime;
    bool mIsEndTrigger;    
    double mCalibTimeStop;

    /// Declare events
    sc_event mAssertDSADIEvent;                              /// Call AssertDSADIMethod() to assert DSADI interrupt
    sc_event mDeassertDSADIEvent;                            /// Call DeassertDSADIMethod() to de-assert DSADI interrupt
    sc_event mAssertDSADEEvent;                              /// Call AssertDSADEMethod() to assert DSADE interrupt
    sc_event mDeassertDSADEEvent;                            /// Call DeassertDSADEMethod() to de-assert DSADE interrupt
    sc_event mAssertDSADPEEvent;                             /// Call AssertDSADPEMethod() to assert DSADPE interrupt
    sc_event mDeassertDSADPEEvent;                           /// Call DeassertDSADPEMethod() to de-assert DSADPE interrupt
    sc_event mAssertDSADDFREQ0Event;                         /// Call AssertDSADDFREQ0Method() to assert DSADDFREQ0 interrupt
    sc_event mDeassertDSADDFREQ0Event;                       /// Call DeassertDSADDFREQ0Method() to de-assert DSADDFREQ0 interrupt
    sc_event mAssertDSADDFREQ1Event;                         /// Call AssertDSADDFREQ1Method() to assert DSADDFREQ1 interrupt
    sc_event mDeassertDSADDFREQ1Event;                       /// Call DeassertDSADDFREQ1Method() to de-assert DSADDFREQ1 interrupt
    sc_event mAssertDSADUEEvent;                             /// Call AssertDSADUEMethod() to assert DSADUE interrupt
    sc_event mDeassertDSADUEEvent;                           /// Call DeassertDSADUEMethod() to de-assert DSADUE interrupt
    sc_event mAssertDSADLEEvent;                             /// Call AssertDSADLEMethod() to assert DSADLE interrupt
    sc_event mDeassertDSADLEEvent;                           /// Call DeassertDSADLEMethod() to de-assert DSADLE interrupt
    sc_event mAssertDSADVCIEvent[DSADC_Base::emVCNum];       /// Call AssertDSADVCIMethod() to assert DSADVCI interrupt
    sc_event mDeassertDSADVCIEvent[DSADC_Base::emVCNum];     /// Call DeassertDSADVCIMethod() to de-assert DSADVCI interrupt   
    sc_event mEndTriggerEvent;                                /// Call EndTriggerMethod() to process End trigger    
    sc_event mCalibStartEvent;                                /// Start Calibration processing
    sc_event mCalibStopEvent;                                 /// Stop Calibration processing
    sc_event mCalibFinishEvent;                               /// Calibration processing is finished
    sc_event mADStartEvent;                                   /// Start A/D conversion processing
    sc_event mADStopEvent;                                    /// Stop A/D conversion processing
    sc_event mConvertFinishEvent;                             /// Call ConvertFinishMethod()
    sc_event mConvertStopEvent;                               /// Call ConvertStopMethod()    
    sc_event mAssertADACTEvent;                               /// Call AssertADACTMethod() to assert DSADnADSR.ADACT bit
    sc_event mDeassertADACTEvent;                             /// Call DeassertADACTMethod() to de-assert DSADnADSR.ADACT bit
    sc_event mTriggerEvent;

public:
    /// Declare input signals
    sc_in<double> ADSVREFH;
    sc_in<double> ADSVREFL;
    sc_in<double> *AN[DSADC_Base::emVCNum];
    sc_in<bool> DSADTRG;
    sc_in<bool> DSADRGT;
    
    /// Declare target socket
    TlmTargetSocket<32> *tsp;

    /// Declare output signals
    sc_out<bool> DSADEND;
    sc_out<bool> DSADI;
    sc_out<bool> DSADE;
    sc_out<bool> DSADPE;
    sc_out<bool> DSADDFREQ0;
    sc_out<bool> DSADDFREQ1;
    sc_out<sc_uint<4> > DSADDFTAG;
    sc_out<sc_uint<16> > DSADDFDATA;
    sc_out<bool> DSADUE;
    sc_out<bool> DSADLE;
    sc_out<bool> *DSADVCI[DSADC_Base::emVCNum];
    SC_HAS_PROCESS (DSADC_UNIT);
    DSADC_UNIT (sc_module_name name,
            DSADC_Base *DSADC_Ptr,
            unsigned int ad_id,
            unsigned int ch_num,
            unsigned int rLaytency,
            unsigned int wLaytency);
    ~DSADC_UNIT (void);

private:
    /// Declare variable
    bool IsRGTpos;
    double mTimeRGTpos;
    double mTimeRGTneg;

private:
    /// Declare methods
    void WriteDSADENDMethod (void);
    void WriteDSADDFTAGMethod (void);
    void WriteDSADDFDATAMethod (void);
    void AssertDSADIMethod (void);
    void DeassertDSADIMethod (void);
    void AssertDSADEMethod (void);
    void DeassertDSADEMethod (void);
    void AssertDSADPEMethod (void);
    void DeassertDSADPEMethod (void);
    void AssertDSADDFREQ0Method (void);
    void DeassertDSADDFREQ0Method (void);
    void AssertDSADDFREQ1Method (void);
    void DeassertDSADDFREQ1Method (void);
    void AssertDSADUEMethod (void);
    void DeassertDSADUEMethod (void);
    void AssertDSADLEMethod (void);
    void DeassertDSADLEMethod (void);
    void AssertDSADVCIMethod (const unsigned int ch_id);
    void DeassertDSADVCIMethod (const unsigned int ch_id);    
    void WriteOutDSADIportMethod(void);
    void WriteOutVCIjPortMethod(const unsigned int ch_id);
    void WriteOutDSADEportMethod(void);
    void WriteOutDSADPEportMethod(void);
    void WriteOutFREQ0portMethod(void);
    void WriteOutFREQ1portMethod(void);
    void WriteOutDSADUEportMethod(void);
    void WriteOutDSADLEportMethod(void);
    void HandleDSADTRGInputMethod (void);
    void HandleDSADRGTInputMethod (void);
    void StartTriggerMethod (void);
    void EndTriggerMethod (void);
    void CalibStartMethod (void);
    void CalibStopMethod (void);
    void CalibFinishMethod (void);
    void ADProcessingThread (void);
    void ConvertFinishMethod (void);
    void ConvertStopMethod (void);   
    void AssertADACTMethod (void);
    void DeassertADACTMethod (void);
    void CheckPeriodTriggerMethod(void);

    /// Virtual functions of DSADC_UNIT_AgentController class
    double GetAnalogValue (const bool is_refh, const unsigned int an_no);
    void NotifyEvent (const unsigned int event, const unsigned int vc_no);
    double GetCalibTimeStop(void);
    void ClearCalibTimeStop(void);
};
#endif
