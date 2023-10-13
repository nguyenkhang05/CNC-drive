// ---------------------------------------------------------------------
// $Id: DSADC_UNIT_Func.h 1398 2019-11-26 06:16:27Z chuonghoangle $
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
#ifndef __DSADC_UNIT_FUNC_H__
#define __DSADC_UNIT_FUNC_H__
#include "dsadc_unit_regif.h"
#include "OSCI2.h"
#include "BusSlaveFuncIf.h"
#include "global.h"

class DSADC_UNIT_AgentController;

class DSADC_UNIT_Func: public Cdsadc_unit_regif,
                       public BusSlaveFuncIf
{

public:
    DSADC_UNIT_Func (std::string name, DSADC_UNIT_AgentController *DSADC_UNIT_AgentControllerPtr, unsigned int ch_num);
    ~DSADC_UNIT_Func (void);

    void Reset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);

    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    
    /// Public functions
    bool IsSyncStartEnable (void);                                              /// Get ADTCR.ADSTARTE value
    bool GetStatusFlag (const bool is_adact);                                   /// Get ADACT/CLBACT value
    void SetStatusFlag (const bool is_adact, const bool is_active);             /// Set ADACT/CLBACT value
    bool GetStatusExterTrigger(const bool is_sttrge);                           /// Get STTRGE/ENDTRGE value
    void UpdateConversionResult (void);                                         /// Update  DIRj/UCR registers
    bool IsADInterruptEnable (const bool dsadrgt_value);                        /// Check DSADI Interrupt condition
    bool IsErrorInterrupt (void);                                               /// Check DSADE Interrupt condition
    void UpdateVCPointer (const bool is_end_trigger);                           /// Update  VCPTR registers
    unsigned int TriggerType (void);                                            /// Get ADTCR.ENDTRGE/STTRGE value
    void UpdateTimeStamp (const bool is_active, const unsigned int tx_value);   /// Update DSADnTSVAL register
    unsigned int GetPinDiagBitValue (const unsigned int an_no);                 /// DSADnTDLVR.ANmkLV value
    bool IsDFEEntry (unsigned int &req, unsigned int &tag, unsigned int &data); /// Get DFE info
    bool GetADSTTEvalue(void);                                                  /// Get ADSTTE value
    bool CheckSetting (const bool is_converting);
    void UpdateCalibResult (void);
    uint32_t GetDSADDIR();
    uint32_t GetDSADADSR();
    uint32_t GetDSADUDPTRR();
    uint32_t GetDSADUDIR();
    uint32_t GetDSADTSVAL();
    uint32_t GetDSADER();

    void Initialize (void);

private:
    enum eCNVCLSmode {
        emSingleVrefL,
        emSingleVrefL_2,
        emDifferential,
        emSelfDiag,
    };
    
    enum eConstance {
        emSignMax = 32767,          /// 0x7FFF
        emUnsignMax = 65535,        /// 0xFFFF
        emMinValue = -1,
        emSignMin = 32768,          /// 0x8000
        emSignFmtConst = 32768,     /// 2^15
        emUnsignFmtConst = 65536,   /// 2^16
        emDRbit = 16,
        emValidFosFreq = 8000000,
        emCHNUMbit = 3
    };
    
    enum eFilterValue {
        emCase1 = 0x90,
        emCase2 = 0x10,
        emCase3 = 0x20,
        emCase4 = 0xB0,
        emCase5 = 0x30,
        emCase6 = 0x40,
        emCase7 = 0x00,
        emCase8 = 0x01,
        emCase9 = 0x02
    };
public:
    enum eAdStatus{
        emdCLBACT,
        emADCT
    };

    enum eTrigeer{
        emENDTRGE,
        emSTTRGE
    };
private:
    DSADC_UNIT_AgentController *mDSADC_UNIT_AgentController;
    std::string mModuleName;                         /// Module name
    
    std::map<unsigned int, unsigned int> mFilterSettingMap;
    unsigned int mDrSignMask;
    bool mIsDSADEInterrupt;
    bool mCalibFinish;
    double mCalibValue;
    unsigned int mStampSignValue;

    /// Private functions
    void Callback_DIR (const unsigned int vc_no);   /// Called after DSADnDIRj is read
    void Callback_UDIR (void);                      /// Called after DSADnUDIR is read
    double CalculateConvertValue (const unsigned int vc_no);
    void CheckParityError (const unsigned int vc_no);

    /// Declare virtual functions of dsadc_unit_regif class
    void cb_DSADnADSTCR_ADST(RegCBstr str);
    void cb_DSADnADENDCR_ADEND(RegCBstr str);
    void cb_DSADnCLBSTCR_CLBST(RegCBstr str);
    void cb_DSADnCLBEDCR_CLBEND(RegCBstr str);
    void cb_DSADnVCPTRR_VCPTR(RegCBstr str);
    void cb_DSADnECR_PEC(RegCBstr str);
};

#endif
