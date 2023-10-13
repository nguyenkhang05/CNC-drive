// ---------------------------------------------------------------------
// $Id: CADC_UNIT.h,v 1.2 2018/11/19 02:50:58 chanle Exp $
//
// Copyright(c) 2016-2018 Renesas Electronics Corporation
// Copyright(c) 2016-2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __CADC_UNIT_H__
#define __CADC_UNIT_H__

#include "CADC_UNIT_AgentController.h"
#include "CADC_AgentController.h"
#include "re_define.h"

class CADC_UNIT_Func;

class CADC_UNIT: public sc_module,
            public CADC_UNIT_AgentController
{
friend class CADC;
friend class CADC_Func;

private:

    CADC_UNIT_Func *mCADC_UNIT_Func;      // CADC_UNIT_Func class instance

    /// Declare variables
    std::string mModuleName;
    bool mCADEND;
    bool mCADI;
    bool mCADE;
    bool mCADPE;
    bool mCADDFREQ;
    bool mCADDFREQ1;
    unsigned int mCADDFTAG;
    unsigned int mCADDFDATA;
    bool mCADUE;
    bool mCADLE;
    bool mCADCVCI[CADC_AgentController::emVCNum];
    
    bool mIsACtopEvent;
    bool mIsTriggerProcessing;
    bool mIsContinueConvert;
    bool mIsEndTrigger;

    /// Declare events
    sc_event mWriteCADENDEvent;            // Call WriteCADENDMethod() to control CADEND port
    sc_event mWriteCADDFTAGEvent;            // Call WriteCADDFTAGMethod() to control CADDFTAG port
    sc_event mWriteCADDFDATAEvent;            // Call WriteCADDFDATAMethod() to control CADDFDATA port
    
    sc_event mAssertCADIEvent;             // Call AssertCADIMethod() to assert CADI interrupt
    sc_event mDeassertCADIEvent;           // Call DeassertCADIMethod() to de-assert CADI interrupt
    sc_event mAssertCADEEvent;             // Call AssertCADEMethod() to assert CADE interrupt
    sc_event mDeassertCADEEvent;           // Call DeassertCADEMethod() to de-assert CADE interrupt
    sc_event mAssertCADPEEvent;             // Call AssertCADPEMethod() to assert CADPE interrupt
    sc_event mDeassertCADPEEvent;           // Call DeassertCADPEMethod() to de-assert CADPE interrupt
    sc_event mAssertCADDFREQEvent;             // Call AssertCADDFREQMethod() to assert CADDFREQ interrupt
    sc_event mDeassertCADDFREQEvent;           // Call DeassertCADDFREQMethod() to de-assert CADDFREQ interrupt
    sc_event mAssertCADUEEvent;             // Call AssertCADUEMethod() to assert CADUE interrupt
    sc_event mDeassertCADUEEvent;           // Call DeassertCADUEMethod() to de-assert CADUE interrupt
    sc_event mAssertCADLEEvent;             // Call AssertCADLEMethod() to assert CADLE interrupt
    sc_event mDeassertCADLEEvent;           // Call DeassertCADLEMethod() to de-assert CADLE interrupt
    sc_event mAssertCADVCIEvent[CADC_AgentController::emVCNum];             // Call AssertCADVCIMethod() to assert CADVCI interrupt
    sc_event mDeassertCADCVCIEvent[CADC_AgentController::emVCNum];           // Call DeassertCADVCIMethod() to de-assert CADVCI interrupt
    
    sc_event mStartTriggerEvent;            // Call StartTriggerMethod() to process Start trigger
    sc_event mEndTriggerEvent;              // Call EndTriggerMethod() to process End trigger
    
    sc_event mCalibStartEvent;          // Start Calibration processing
    sc_event mCalibStopEvent;           // Stop Calibration processing
    sc_event mCalibFinishEvent;         // Calibration processing is finished
    sc_event mACtartEvent;             // Start A/D conversion processing
    sc_event mACtopEvent;              // Stop A/D conversion processing
    sc_event mConvertFinishEvent;       // Call ConvertFinishMethod()
    sc_event mConvertStopEvent;         // Call ConvertStopMethod()
    
    
    sc_event mAssertADACTEvent;             // Call AssertADACTMethod() to assert CADnACR.ADACT bit
    sc_event mDeassertADACTEvent;           // Call DeassertADACTMethod() to de-assert CADnACR.ADACT bit

    sc_event mWriteCADIEvent;
    sc_event mWriteCADEEvent;
    sc_event mWriteCADPEEvent;
    sc_event mWriteCADDFREQEvent;
    sc_event mWriteCADUEEvent;
    sc_event mWriteCADLEEvent;
    sc_event mWriteCADVCIEvent[CADC_AgentController::emVCNum];
    sc_event mUpdateDIREvent[CADC_AgentController::emVCNum];
    sc_event mUpdateConversionFinishEvent;

public:
    /// Declare input signals
    sc_in<double> ADSVREFH;
    sc_in<double> ADSVREFL;
    sc_in<double> *AN[CADC_AgentController::emVCNum];
    sc_in<bool> CADTRG;
    sc_in<bool> CADRGT;
    
    /// Declare output signals
    sc_out<bool> CADEND;
    sc_out<bool> CADI;
    sc_out<bool> CADE;
    sc_out<bool> CADPE;
    sc_out<bool> CADDFREQ;
    sc_out<sc_uint<4> > CADDFTAG;
    sc_out<sc_uint<16> > CADDFDATA;
    sc_out<bool> CADUE;
    sc_out<bool> CADLE;
    sc_out<bool> *CADCVCI[CADC_AgentController::emVCNum];

    SC_HAS_PROCESS (CADC_UNIT);
    CADC_UNIT (sc_module_name name,
                CADC *CADC_Ptr,
                unsigned int ad_id,
                unsigned int ch_num);
    ~CADC_UNIT (void);

private:
    /// Declare methods
    void WriteCADENDMethod (void);
    void WriteCADDFTAGMethod (void);
    void WriteCADDFDATAMethod (void);
    void WriteCADIMethod (void);
    void AssertCADIMethod (void);
    void DeassertCADIMethod (void);
    void WriteCADEMethod (void);
    void AssertCADEMethod (void);
    void DeassertCADEMethod (void);
    void WriteCADPEMethod (void);  
    void AssertCADPEMethod (void);
    void DeassertCADPEMethod (void);
    void WriteCADDFREQMethod (void);  
    void AssertCADDFREQMethod (void);
    void DeassertCADDFREQMethod (void);
    void WriteCADUEMethod (void);  
    void AssertCADUEMethod (void);
    void DeassertCADUEMethod (void);
    void WriteCADLEMethod (void);  
    void AssertCADLEMethod (void);
    void DeassertCADLEMethod (void);
    void WriteCADVCIMethod (const unsigned int ch_id);  
    void AssertCADVCIMethod (const unsigned int ch_id);
    void DeassertCADVCIMethod (const unsigned int ch_id);
    
    void HandleCADTRGInputMethod (void);

    void StartTriggerMethod (void);
    void EndTriggerMethod (void);
    
    void CalibStartMethod (void);
    void CalibStopMethod (void);
    void CalibFinishMethod (void);
    void ADProcessingThread (void);
    void ConvertFinishMethod (void);
    void ConvertStopMethod (void);
    
    void WriteADACTMethod (void); 
    void AssertADACTMethod (void);
    void DeassertADACTMethod (void);
    
    /// Internal functions
    void CancelEvents (void);
    void Initialize (void);

    /// Virtual functions of CADC_UNIT_AgentController class
    double GetAnalogValue (const bool is_refh, const unsigned int an_no);
    void NotifyEvent (const unsigned int event, const unsigned int vc_no);
    double CaltS (void);
    double CaltDF (void);

    void UpdateDIRMethod(unsigned int ch_id); /// Update DIR, UDR function
    void CheckFinishMethod(void);
};
#endif
