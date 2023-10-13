// ---------------------------------------------------------------------
// $Id: CADC_UNIT_Func.h,v 1.1 2018/11/19 02:50:58 chanle Exp $
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
#ifndef __CADC_UNIT_FUNC_H__
#define __CADC_UNIT_FUNC_H__
#include "cadc_unit_regif.h"
#include "OSCI2.h"

class CADC_UNIT_AgentController;

class CADC_UNIT_Func: public Ccadc_unit_regif
{
public:
    CADC_UNIT_Func (std::string name, CADC_UNIT_AgentController *CADC_UNIT_AgentControllerPtr);
    ~CADC_UNIT_Func (void);

    void Reset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);

    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    
    /// Public functions
    bool GetStatusFlag (const bool is_adact);         // Get ADACT/CLBACT value
    void SetStatusFlag (const bool is_adact, const bool is_active);         // Set ADACT/CLBACT value
    void UpdateConversionResult (void);             // Update  DIRj/UCR registers
    bool IsADInterruptEnable (const bool dsadrgt_value);        // Check DSADI Interrupt condition
    bool IsErrorInterrupt (void);                   // Check DSADE Interrupt condition
    void UpdateVCPointer (const bool is_end_trigger);       // Update  VCPTR registers
    unsigned int TriggerType (void);                // Get ADTCR.ENDTRGE/STTRGE value
    unsigned int GetVCEndPointer (void);            // Get UCR.VCEP value
    bool CheckSetting (const bool is_converting);
    unsigned int GetPinDiagBitValue (const unsigned int an_no);      // DSADnTDLVR.ANmkLV value
    bool IsDFEEntry (unsigned int &tag, unsigned int &data);      // Get DFE info
    unsigned int GetAver ();      // Get output data averaging
    void UpdateCalibResult (void);
    void Initialize (void);
    void UpdateDIR(unsigned int vc_no);
private:
    enum eCNVCLSmode {
        emSingleVrefL,
        emSingleVrefL_2,
        emDifferential,
        emSelfDiag,
    };
    
    enum eConstance {
        emSignMax = 32767,          // 0x7FFF
        emUnsignMax = 65535,        // 0xFFFF
        emMinValue = -1,
        emSignMin = 32768,          // 0x8000
        emSignFmtConst = 32768,     // 2^15
        emUnsignFmtConst = 65536,   // 2^16
        emDRbit = 16,
        emValidFosFreq = 8000000
    };
    
    CADC_UNIT_AgentController *mCADC_UNIT_AgentController;
    std::string mModuleName;        //Module name
    
    unsigned int mDrSignMask;
    bool mIsCADCEInterrupt;
    bool mCalibFinish;
    double mCalibValue;
    bool mIsDIRSetting;
    bool mIsUDIRSetting;
    bool mIsDIRClear;
    bool mIsUDIRClear;

    /// Private functions
    void Callback_DIR (const unsigned int vc_no);   // Called after CADCnDIRj is read
    void Callback_UDIR (void);                      // Called after CADCnUDIR is read
    void Callback_SMPCR (void);                     // Called after CADCnSMPCR is read
    double CalculateConvertValue (const unsigned int vc_no);
    void CheckParityError (const unsigned int vc_no);
    
    /// Declare virtual functions of cadc_unit_regif class
    void cb_CADCnADSTCR_CADC00ADST(RegCBstr str);
    void cb_CADCnADENDCR_CADC00ADEND(RegCBstr str);
    void cb_CADCnCLBSTCR_CADC00CLBST(RegCBstr str);
    void cb_CADCnCLBEDCR_CADC00CLBEND(RegCBstr str);
    void cb_CADCnVCPTRR_CADC00VCPTR(RegCBstr str);
    void cb_CADCnECR_CADC00ULEC(RegCBstr str);
};

#endif
