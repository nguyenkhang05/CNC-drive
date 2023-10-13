// ---------------------------------------------------------------------
// $Id: HWCORE_IRCTG3_Func.h,v 1.5 2020/10/15 06:49:06 khoaanhnguyen Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation:
// ---------------------------------------------------------------------
// Description: Copyright of HWCORE_IRCTG3_Func.h
//Ref: {HWCORE_IRCTG3_UD_ATT1_005}
#ifndef __HWCORE_IRCTG3_FUNC_H__
#define __HWCORE_IRCTG3_FUNC_H__
#include "hwcore_irctg3_regif.h"

#define SIGNBIT_IDX             11      // Index of signed bit
#define MAX_SHORT               0xFFFF  // Max hexadecimal number of unsigned short
#define MAX_CHAR                0xFF
#define MAX_ZONE_PER_PHASE      32      // Max hexadecimal number of unsigned short
#define MAX_ZONE                96
#define COMPARE_REG_MASK        0x8FFF  // Comparison register mask
#define BOUNDARY_MASK           0xFFF  // Boundary mask
#define MIN_VLD_ZONE            2
#define MAX_VLD_U               96
#define MAX_VLD_V               32
#define MAX_VLD_W               32
#define DEGREES_180             0x800
#define U_PHASE_NAME            "U Phase"
#define V_PHASE_NAME            "V Phase"
#define W_PHASE_NAME            "W Phase"

struct ZoneList {
    unsigned char No;
    unsigned short High;
    unsigned short Low;
    bool bInt;
    bool bADC;
    bool bPtn;
    ZoneList* pNxtZone;
    ZoneList* pPreZone;

    ZoneList(unsigned char cZoneNo, ZoneList* pPreZone, unsigned short upper, unsigned short lower, bool bInt, bool bADTrig, bool bPtn) {
        this->No = cZoneNo;
        this->High = upper;
        this->Low = lower;
        this->bInt = bInt;
        this->bADC = bADTrig;
        this->bPtn = bPtn;
        if (pPreZone != NULL){
            this->pPreZone = pPreZone;
        }
        this->pNxtZone = NULL;
    }
};

// Description: Declaration of HWCORE_IRCTG3_Func class
class HWCORE_IRCTG3_Func : public Chwcore_irctg3_regif
{
public:
    SC_HAS_PROCESS(HWCORE_IRCTG3_Func);
    HWCORE_IRCTG3_Func(std::string name, HWCORE_IRCTG3_AgentController* HWCORE_IRCTG3_AgentControllerPtr);
    ~HWCORE_IRCTG3_Func();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);

    // Read/write registers API
    void read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
    void write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);

    // Set/Get registers APIs
    void SetRegVal(std::string RegName, unsigned int RegIdx, unsigned int val);
    void SetRegVal(std::string RegName, unsigned int RegIdx, std::string BitName, unsigned int val);
    unsigned int GetRegVal(std::string RegName, unsigned int RegIdx, std::string BitName);
    unsigned int GetRegVal(std::string RegName, unsigned int RegIdx);
    void SetElectricalAngle(unsigned short val);
    void RegisterBlockOperation();
    void ZoneSeachAndInterruptOperation(unsigned char cPhase);
    void ResetFlagAfterComplete(void);
    void DumpStatInfo();

private:
    enum eState {
        emStopped = 0,
        emRunning,
        //Number of states
        emNumState
    };

    enum eTransModeBit {
        emCMP = 0,
        emOFS,
        emINT,
        emADC,
        //Number of transfer mode bits
        emNumTransModeBit
    };

    enum eTransType {
        emBatch = 0,
        emImmediate,
        //Number of transfer type
        emNumTransType
    };

    enum eWaveMode {
        emSawtooth = 0,
        emTriangle,
        emSawtoothW,
        //Number of wave modes
        emNumWaveMode
    };

    enum eRotateDirection {
        emPositive = 0,
        emNegative,
        //Number of comparison mode
        emNumRotateDirection
    };

    enum eAngleBuff {
        emBuff_Old = 0,
        emBuff_Cur,
        //Buffer size
        emBuffSize
    };

    // Variables
    HWCORE_IRCTG3_AgentController * mpIRCTG3_AgentController;    // HWCORE_IRCTG3 Agent controller pointer
    std::string     mModuleName;                           // Module name
    std::map<std::string, vpcl::re_register**> mRegisterMap;        // Map value of registers

    //Operation mode/state variables
    bool m_IsTrigger;
    unsigned char m_Phase;
    unsigned char m_State;
    unsigned char m_WaveMode;
    unsigned char m_CompMode;
    unsigned char m_SetRotDir;
    unsigned char m_ActualRotDir;
    unsigned char m_TransMode[emNumTransModeBit];
    bool m_bIsAngChange;
    bool m_bIsOfsAngChange;
    bool m_bIntEn[MAX_ZONE];
    bool m_bADCEn[MAX_ZONE];
    //Angle related variables
    unsigned short m_InpAng[emBuffSize][HWCORE_IRCTG3_AgentController::emNumPhase];
    unsigned short m_OffAng[emBuffSize][HWCORE_IRCTG3_AgentController::emNumPhase];
    //Zone related variables
    unsigned short m_RetBuff[MAX_ZONE];
    unsigned char m_VldZoneMask[HWCORE_IRCTG3_AgentController::emNumPhase];
    unsigned char m_VldZoneNum;
    unsigned char m_MaxVldZone[HWCORE_IRCTG3_AgentController::emNumCompMode][emNumWaveMode];
    unsigned char m_DetectZoneNo[HWCORE_IRCTG3_AgentController::emNumPhase];
    ZoneList* m_pCurZone;
    ZoneList* m_pPreZone;
    //Reference buffer
    unsigned char m_RefBuff_CMODE;
    unsigned char m_RefBuff_INTEN;
    unsigned char m_RefBuff_ADCEN;
    unsigned short m_RefBuff_OFSAL;
    unsigned short m_RefBuff_OFSU;
    unsigned short m_RefBuff_OFSV;
    unsigned short m_RefBuff_OFSW;
    unsigned int m_RefBuff_VALN;
    unsigned int m_RefBuff_INT[HWCORE_IRCTG3_AgentController::emNumPhase];
    unsigned int m_RefBuff_ADC[HWCORE_IRCTG3_AgentController::emNumPhase];
    unsigned short m_RefBuff_CMP[MAX_ZONE];

    // Operation
    void OffsetAddition();
    ZoneList* ZoneSegmentation();
    bool ZoneDetermination(ZoneList* pZoneList);
    bool ZoneTransDetect();
    void OutputGeneration();
    //Support function
    void ReleaseZoneListMemory(ZoneList*);
    bool ValidateValidZoneNum(unsigned char);
    ZoneList* ZoneSegmentation(unsigned char cPhase);
    bool ZoneSegmentationCheckBoundary(ZoneList*);
    void ZoneSegmentationTriangle_Auto(ZoneList*);
    void ZoneSegmentationSawtoothW_Auto(ZoneList*);
    bool InterruptGeneration(unsigned char&);
    bool ADTriggerGeneration(unsigned char&);
    void SetINTStatus(bool bInt, unsigned char cZoneNo);
    void SetADCStatus(bool bAdc, unsigned char cZoneNo);
    void UpdateSettingZoneRegister(ZoneList*);
    void UpdateDetectZoneInfo(ZoneList*);
    std::string GetPhaseNameString();
    void Initialize(void);

    // Callback
    void cb_IR3CCTR_EN(RegCBstr str);
    void cb_IR3CMODE_MODE(RegCBstr str);
    void cb_IR3TRSMODE_CMP(RegCBstr str);
    void cb_IR3TRG_TRG(RegCBstr str);
    void cb_IR3INTCLEAR_UCLR(RegCBstr str);
    void cb_IR3INTEN_EN(RegCBstr str);
    void cb_IR3INT_INT(RegCBstr str);
    void cb_IR3ADCCLEAR_UCLR(RegCBstr str);
    void cb_IR3ADCEN_EN(RegCBstr str);
    void cb_IR3ADC_ADI(RegCBstr str);
    void cb_IR3COFSALL_DATA(RegCBstr str);
    void cb_IR3COFSU_DATA(RegCBstr str);
    void cb_IR3COFSV_DATA(RegCBstr str);
    void cb_IR3COFSW_DATA(RegCBstr str);
    void cb_IR3CMP_CMP(RegCBstr str);
    void cb_IR3VALN_UVL(RegCBstr str);
};

#endif // __HWCORE_IRCTG3_FUNC_H__
