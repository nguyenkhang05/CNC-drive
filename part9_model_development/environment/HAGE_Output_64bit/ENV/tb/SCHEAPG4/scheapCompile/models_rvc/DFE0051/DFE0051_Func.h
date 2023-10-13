// ---------------------------------------------------------------------
// $Id: DFE0051_Func.h,v 1.3 2017/11/15 08:48:22 chanle Exp $
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
#ifndef __DFE0051_FUNC_H__
#define __DFE0051_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "dfe0051_regif.h"

class Cdfe0051_agent_controller;

class Cdfe0051_func: public Cdfe0051_regif
                ,public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (Cdfe0051_func);
    Cdfe0051_func(std::string name, unsigned int num_channel, Cdfe0051_agent_controller* DFE0051AgentController);
    ~Cdfe0051_func();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

    // APIs
    unsigned int GetENBit(unsigned int channel);
    void SetTRHCHReg(unsigned int channel, unsigned int value);
    void SetFITSBit(unsigned int channel, unsigned int value);
    void SetMETSBit(unsigned int channel, unsigned int value);
    void SetMITSBit(unsigned int channel, unsigned int value);
    void SetPETSBit(unsigned int channel, unsigned int value);
    void SetPITSBit(unsigned int channel, unsigned int value);
    void SetPMETSBit(unsigned int channel, unsigned int value);
    void SetPMITSBit(unsigned int channel, unsigned int value);
    void SetVALIDBit(unsigned int channel, unsigned int value);
    unsigned int GetVALIDBit(unsigned int channel);
    void SetDIOWBit(unsigned int channel, unsigned int value);
    void SetDOOWBit(unsigned int channel, unsigned int value);
    unsigned int GetSWInputTag();
    unsigned int GetSWInputData();
    bool CheckMatchTag(unsigned int in_tag, unsigned int channel_id);
    unsigned short GetCoefficient(unsigned int channel_id, unsigned int index);
    bool SetCoeffMem(unsigned int offset, unsigned int value);
    bool GetCoeffMem(unsigned int offset, unsigned int& data);
    bool SetDataMem(unsigned int offset, unsigned int value);
    bool GetDataMem(unsigned int offset, unsigned int& data);
    unsigned short GetFIRBuffer(unsigned int channel_id, unsigned int num_taps);
    unsigned int GetIIRCirBuffer(unsigned int channel_id, unsigned int index);
    void CopyInitBufToCirBuf(unsigned int channel);
    unsigned int GetGAIN2Bit(unsigned int channel_id);
    unsigned int GetGAIN3Bit(unsigned int channel_id);
    void UpdateIIRBufferOneStage(unsigned int channel_id, unsigned int input);
    void UpdateIIRBufferTwoStage(unsigned int channel, unsigned int input1, unsigned int input2);
    void UpdateIIRBufferThreeStage(unsigned int channel, unsigned int input1, unsigned int input2, unsigned int input3);
    bool IsFIRFixedPoint(unsigned int channel_id);
    void UpdateFIRBuffer(unsigned int channel_id, unsigned short xn, unsigned int num_taps);
    bool IsIntermediateConvertFloat(unsigned int channel_id);
    bool IsFIRSelect(unsigned int channel);
    bool IsIIRSelect(unsigned int channel);
    unsigned int GetFEBit(unsigned int channel);
    unsigned int GetFITMTRGBit(unsigned int trigger);
    int GetFTBit(unsigned int channel);
    unsigned int GetCMDBit(unsigned int channel);
    unsigned int GetFMTBit(unsigned int channel);
    bool IsAbsolutePerform(unsigned int channel);
    void SetAERBit(unsigned int channel, unsigned int value);
    unsigned int GetAccumExpectedCount(unsigned int channel);
    bool IsAccumEnable(unsigned int channel);
    bool IsDecimateEnable(unsigned int channel);
    unsigned int GetDeciCATAGBit(unsigned int channel);
    void SetCERBit(unsigned int channel, unsigned int value);
    void SetGERBit(unsigned int channel, unsigned int value);
    void SetMERBit(unsigned int channel, unsigned int value);
    unsigned int GetAEBit(unsigned int channel);
    unsigned int GetAFEBit(unsigned int channel);
    unsigned int GetMITMTRGBit(unsigned int trigger);
    int GetATBit(unsigned int channel);
    unsigned int GetCAENBit(unsigned int channel);
    unsigned int GetPRCSABit(unsigned int channel);
    void SetSUBTRHCHReg(unsigned int sub_id, unsigned int value);
    unsigned int GetMinuendChannelNum(unsigned int sub_id);
    unsigned int GetSubtrahendChannelNum(unsigned int sub_id);
    void SetSUBFBit(unsigned int sub_id, unsigned int value);
    unsigned int GetSIEOBit(unsigned int sub_id);
    unsigned int GetSUBFBit(unsigned int sub_id);
    void SetMINFBit(unsigned int sub_id, unsigned int value);
    unsigned int GetMINFBit(unsigned int sub_id);
    void SetSubDataOutReg(unsigned int sub_id, unsigned int value);
    unsigned int GetSubCAEN(unsigned int sub_id);
    bool IsFPConvertSubValue(unsigned int sub_id);
    void SetSDOENBit(unsigned int sub_id, unsigned int value);
    unsigned int GetSDOENBit(unsigned int sub_id);
    void SetSDOOWBit(unsigned int sub_id, unsigned int value);
    void SetSCERBit(unsigned int sub_id, unsigned int value);
    void SetSGERBit(unsigned int sub_id, unsigned int value);
    unsigned int GetSubCATAGBit(unsigned int sub_id);
    unsigned int GetSBEBit(unsigned int channel);
    unsigned int GetSBFEBit(unsigned int channel);
    unsigned int GetSBTBit(unsigned int channel);
    unsigned int GetSUBTMTRGBit(unsigned int trigger);
    unsigned int GetSENBit(unsigned int channel);
    void SetSITSBit(unsigned int subid, unsigned int value);
    void SetSETSBit(unsigned int subid, unsigned int value);
    bool CheckSubValidSetting(unsigned int subid);
    unsigned int GetCENBit(unsigned int capid);
    unsigned int GetCAPCHBit(unsigned int capid);
    void CaptureToCAPDOCHReg(unsigned int channel, unsigned int capid);
    void StoreIntermediateValue(unsigned int channel_id, unsigned int high, unsigned int low);
    bool CheckCapValidSetting(unsigned int capid);
    bool IsFPConvertOutputValue(unsigned int channel);
    unsigned int GetIEOBit(unsigned int channel);
    void SetDOCHReg(unsigned int channel, unsigned int data);
    unsigned int GetDOCHReg(unsigned int channel);
    void SetDOENBit(unsigned int channel, unsigned int value);
    unsigned int GetDOENBit(unsigned int channel);
    void SetPHEBit(unsigned int channel, unsigned int value);
    unsigned int GetIEPBit(unsigned int channel);
    void SetDISBBit(unsigned int channel, unsigned int value);
    void SetDISABit(unsigned int channel, unsigned int value);
    unsigned int GetPRCSBBit(unsigned int channel);
    unsigned int GetPHInitVal(unsigned int phase, unsigned int channel);
    unsigned int GetPHPSBit(unsigned int ph_phase, unsigned int channel);
    unsigned int GetPHUPDCHBit(unsigned int phuid);
    unsigned int GetCNSLEBit(unsigned int channel);
    unsigned int GetCNSLBit(unsigned int channel);
    unsigned int GetOEPHUPDBit(unsigned int phuid);
    unsigned int GetPH23SLBit(unsigned int phuid);
    unsigned int GetPH23EBit(unsigned int phuid);
    void SetPHCHReg(unsigned int phase, unsigned int channel, unsigned int value);
    void SetPHCHPureVal(unsigned int phase, unsigned int channel, unsigned int value);
    void SetPrePHCH(unsigned int phase, unsigned int channel);
    unsigned int GetPrePHCHReg(unsigned int channel);
    unsigned int GetCompareOperator(unsigned int phase, unsigned int channel);
    void SetPHINDBit(unsigned int ph_phase, unsigned int channel, unsigned int value);
    void SetPHIOFBit(unsigned int ph_phase, unsigned int channel, unsigned int value);
    void SetPHINDCHReg(unsigned int channel, unsigned int value);
    void SetPH2IND0Reg(unsigned int value);
    void SetPH3IND0Reg(unsigned int value);
    unsigned int GetPHPFMTBit(unsigned int channel);
    unsigned int GetPH23PENBit();
    unsigned int GetPH23CHSBit();
    unsigned int GetPH23STBit(unsigned int channel);
    void SetPH23STBit(unsigned int channel, unsigned int value);
    void SetPH20Reg(unsigned int value);
    void SetPH30Reg(unsigned int value);
    unsigned int GetCPCSBit(unsigned int channel);
    unsigned int GetAlpha(unsigned int ph_phase, unsigned int channel);
    unsigned int GetPMFEBit(unsigned int channel);
    unsigned int GetPMEBit(unsigned int channel);
    unsigned int GetPEBit(unsigned int channel);
    unsigned int GetPFEBit(unsigned int channel);
    unsigned int GetPMTBit(unsigned int channel);
    unsigned int GetPMITMTRGBit(unsigned int trigger);
    unsigned int GetPITMTRGBit(unsigned int trigger);
    int GetPTBit(unsigned int channel);
    void SetCNDBit(unsigned int channel, unsigned int value);
    unsigned int GetCPRegWithSELB1Bit(unsigned int channel);
    unsigned int GetCHSBit();

private:
    // Enums
    enum eResetActiveLevel {
        emResetActive       = 0,
        emResetSignalNum    = 1
    };
    enum eMEMInfo{
        emCMEMType
        ,emDMEMType
        ,emCHDMEMSize       = 0x80
        ,emStartOffsetDMEM  = 0x2000
        ,emCHCMEMSize       = 0x40
        ,emStartOffsetCMEM  = 0x1000
        ,emBaseOffsetIniBuf = 0x40
        ,emIIRCHInitMemSize = 0x40
        ,emIIRCHCirMemSize  = 0x40
    };
    enum eGeneralInfo{
        emMaxPHIND = 0x3FFF
    };

    // Variables
    Cdfe0051_agent_controller   *mDFE0051AgentController;
    unsigned char           mDMEM[0x800];   // Data memory
    unsigned char           mCMEM[0x400];   // Coefficient memory
    unsigned int            mNumChannel;    // Number of channels
    bool                    mWasReadHOLCH[16];  // Was HOLCH read yet
    bool                    mWasReadHOHCH[16];  // Was HOHCH read yet
    bool                    mCurErrStatus;      // Current error status
    bool                    mPreErrStatus;      // Previous error status
    std::string             mModuleName;        // Module name
    unsigned int            mStartDataPtr[16];  // Start data pointer (used in CMEM, DMEM)
    unsigned int            mCurDataPtr[16];    // Current data pointer (used in CMEM, DMEM)
    unsigned int            mPrePHCH[16];       // Previous value of PHCH reg
    unsigned int            mPrePH20;           // Previous value of PH20 reg
    unsigned int            mPrePH30;           // Previous value of PH30 reg
    unsigned int            mPrePhase[16];      // Previous phase of PH23
    unsigned int            mPHCHPureVal[16];   // Pure value (not converted to Floating point) of PHCH reg
    unsigned int            mPH20PureVal;       // Pure value (not converted to Floating point) of PH20 reg
    unsigned int            mPH30PureVal;       // Pure value (not converted to Floating point) of PH30 reg

    // Functions
    void Initialize(void);
    bool CheckValidAccessMem(eMEMInfo mem_type, unsigned int addr, unsigned int size, bool is_wr, bool debug);
    void UpdateErrorStatus();
    bool CheckSettingDFEjCTLACHn(unsigned int channel);
    bool CheckSettingDFEjCTLBCHn(unsigned int channel);
    bool CheckSettingDFEjTRGCHn(unsigned int channel);
    bool CheckSettingDFEjPMITRG();
    bool CheckSettingDFEjPITRG();
    bool CheckSettingDFEjMITRG();
    bool CheckSettingDFEjFITRG();
    bool CheckSettingDFEjPHUPDCn();
    bool CheckSettingDFEjTMTRGCHn(unsigned int channel);
    bool CheckSettingDFEjSUBCTLCHn(unsigned int channel);
    bool CheckSettingDFEjSUBTRGCHn(unsigned int channel);
    bool CheckSettingDFEjSUBTRG();
    void CommonCallbackDFE0051HOHCHn(unsigned int channel);
    void CommonCallbackDFE0051HOLCHn(unsigned int channel);
    void UpdateDFEjST();
    void UpdateDFEjEST();

    // Callback
    void cb_DFEjCTLACHn_EN(RegCBstr str);
    void cb_DFEjCTLBCHn_PRCSA(RegCBstr str);
    void cb_DFEjDOCHn_DFEjDOCHn(RegCBstr str);
    void cb_DFEjPHINDCHn_PHIOF(RegCBstr str);
    void cb_DFEjCLRSTCHn_CLRDIOW(RegCBstr str);
    void cb_DFEjDI_DI(RegCBstr str);
    void cb_DFEjTRG_TRGA(RegCBstr str);
    void cb_DFEjHOLCH0_DFEjHOLCH0(RegCBstr str);
    void cb_DFEjHOHCH0_GURD(RegCBstr str);
    void cb_DFEjHOLCH2_DFEjHOLCH2(RegCBstr str);
    void cb_DFEjHOHCH2_GURD(RegCBstr str);
    void cb_DFEjHOLCH4_DFEjHOLCH4(RegCBstr str);
    void cb_DFEjHOHCH4_GURD(RegCBstr str);
    void cb_DFEjHOLCH6_DFEjHOLCH6(RegCBstr str);
    void cb_DFEjHOHCH6_GURD(RegCBstr str);
    void cb_DFEjHOLCH8_DFEjHOLCH8(RegCBstr str);
    void cb_DFEjHOHCH8_GURD(RegCBstr str);
    void cb_DFEjHOLCH10_DFEjHOLCH10(RegCBstr str);
    void cb_DFEjHOHCH10_GURD(RegCBstr str);
    void cb_DFEjHOLCH12_DFEjHOLCH12(RegCBstr str);
    void cb_DFEjHOHCH12_GURD(RegCBstr str);
    void cb_DFEjHOLCH14_DFEjHOLCH14(RegCBstr str);
    void cb_DFEjHOHCH14_GURD(RegCBstr str);
    void cb_DFEjPH23CCTL0_PEN(RegCBstr str);
    void cb_DFEjPH2CTL0_CN2SLB2(RegCBstr str);
    void cb_DFEjPH3CTL0_CN3SLB2(RegCBstr str);
    void cb_DFEjSUBCTLCHn_SEN(RegCBstr str);
    void cb_DFEjSUBDOCHn_DFEjSUBDOCHn(RegCBstr str);
    void cb_DFEjSUBCLRSTCHn_CLRSDOOW(RegCBstr str);
    void cb_DFEjSUBTRGCHn_SBT(RegCBstr str);
    void cb_DFEjCAPCTLCHn_CEN(RegCBstr str);
    void cb_DFEjCAPDOCHn_DFEjCAPDOCHn(RegCBstr str);
    void cb_DFEjCAPCLRSTCHn_CLRCDOOW(RegCBstr str);

};
#endif //__DFE0051_FUNC_H__
