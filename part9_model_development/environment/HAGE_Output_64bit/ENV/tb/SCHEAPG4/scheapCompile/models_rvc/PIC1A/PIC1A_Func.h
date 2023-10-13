// ---------------------------------------------------------------------
// $Id: PIC1A_Func.h,v 1.1 2014/12/15 09:31:44 binhnguyen Exp $
//
// Copyright(c) 2014 Renesas Electronics Corporation
// Copyright(c) 2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __PIC1A_FUNC_H__
#define __PIC1A_FUNC_H__
#include "BusSlaveFuncIf.h"
#include "pic1a_regif.h"

class PIC1A_AgentController;

class PIC1A_Func: public Cpic1a_regif
                ,public BusSlaveFuncIf
{
public:
    PIC1A_Func (std::string name,
               PIC1A_AgentController *PIC1A_AgentControllerPtr);
    ~PIC1A_Func (void);

    void Reset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);
    
    void UpdateInputPort (const unsigned int index, const bool value);
    void UpdateFlag (const unsigned int module, const unsigned int index);
    void CalcDT (const unsigned int index, const bool is_reset);
    void CalcRS (unsigned int index, const bool is_set);
    void CalcRTO (const unsigned int module, const unsigned int index, const unsigned int change_type);
    void ProcessWNG (const unsigned int module, const unsigned int index, const unsigned int change_type);
    void CalcWNGOutput (const unsigned int module, const unsigned int index, const unsigned int type,
                        const bool temp_d, const bool temp_e, const bool temp_f, const bool temp_h);
    void UpdateWNG_FFiOuput (const unsigned int module, const unsigned int index);
    void UpdateWNG_FFgOuput (const unsigned int module, const unsigned int index);
    void UpdateFNInput (const unsigned int module, const unsigned int index, const bool is_AInput, const bool value);
    void CalcFNOutput (const unsigned int module, const unsigned int index);
    void UpdatePFN_In1Input (const unsigned int module, const unsigned int index, const bool value);
    void CalcPFN (const unsigned int module, const unsigned int index, const unsigned int change_type);
    void UpdateREG513_FFInput (const unsigned int module, const unsigned int index);
    void UpdateREG513_FFOuput (const unsigned int module, const unsigned int index);
    void UpdateREG514_FFInput (const unsigned int module, const unsigned int index);
    void UpdateREG514_FFOuput (const unsigned int module, const unsigned int index);
    void UpdateTSGOUTCTR_FFOuput (const unsigned int module, const unsigned int index);
    void CalcTRG (const unsigned int module);
    
    void CalcMUX_PIC0TAUD0SEL (const unsigned int index);
    void CalcMUX_PIC0TAUD1SEL (const unsigned int index);
    void CalcMUX_PIC0TSGOUTCTR_0 (const unsigned int module, const unsigned int index);
    void CalcMUX_PIC0TSGOUTCTR_Other (const unsigned int module, const unsigned int index);
    void CalcMUX_PIC0ENCSEL40_0003 (const unsigned int module);
    void CalcMUX_PIC0ENCSEL40_07 (const unsigned int module);
    void CalcMUX_PIC0HIZCEN (const unsigned int index);
    void CalcMUX_PIC0HALLSEL (const unsigned int module, const unsigned int index);
    
    void CalcMUX_PIC0REG20_00 (const unsigned int module);
    void CalcMUX_PIC0REG20_01 (const unsigned int module);
    void CalcMUX_PIC0REG20_02 (const unsigned int module);
    void CalcMUX_PIC0REG20_03 (const unsigned int module);
    void CalcMUX_PIC0REG200_04 (void);
    void CalcMUX_PIC0REG210_04 (void);
    void CalcMUX_PIC0REG20_05 (const unsigned int module);
    void CalcMUX_PIC0REG200_0809 (void);
    void CalcMUX_PIC0REG210_0809 (void);
    void CalcMUX_PIC0REG200_1011 (void);
    void CalcMUX_PIC0REG210_1011 (void);
    void CalcMUX_PIC0REG200_1213 (void);
    void CalcMUX_PIC0REG210_1213 (void);
    void CalcMUX_PIC0REG200_16 (void);
    void CalcMUX_PIC0REG210_16 (void);
    void CalcMUX_PIC0REG20_17 (const unsigned int module);
    void CalcMUX_PIC0REG200_18 (void);
    void CalcMUX_PIC0REG210_18 (void);
    void CalcMUX_PIC0REG200_2425 (const unsigned int type);
    void CalcMUX_PIC0REG210_2425 (const unsigned int type);
    void CalcMUX_PIC0REG20_28 (const unsigned int module, const unsigned int index);
    void CalcMUX_PIC0REG201_0001 (void);
    void CalcMUX_PIC0REG211_0001 (void);
    void CalcMUX_PIC0REG201_0203 (void);
    void CalcMUX_PIC0REG211_0203 (void);
    void CalcMUX_PIC0REG201_0405 (void);
    void CalcMUX_PIC0REG211_0405 (void);
    void CalcMUX_PIC0REG201_0607 (void);
    void CalcMUX_PIC0REG211_0607 (void);
    void CalcMUX_PIC0REG201_0809 (void);
    void CalcMUX_PIC0REG211_0809 (void);
    void CalcMUX_PIC0REG201_1011 (void);
    void CalcMUX_PIC0REG211_1011 (void);
    void CalcMUX_PIC0REG202_02 (void);
    void CalcMUX_PIC0REG212_02 (void);
    void CalcMUX_PIC0REG202_03 (void);
    void CalcMUX_PIC0REG212_03 (void);
    void CalcMUX_PIC0REG202_04 (void);
    void CalcMUX_PIC0REG212_04 (void);
    void CalcMUX_PIC0REG202_05 (void);
    void CalcMUX_PIC0REG212_05 (void);
    void CalcMUX_PIC0REG202_06 (void);
    void CalcMUX_PIC0REG212_06 (void);
    void CalcMUX_PIC0REG202_07 (void);
    void CalcMUX_PIC0REG212_07 (void);
    void CalcMUX_PIC0REG22_08 (const unsigned int module);
    void CalcMUX_PIC0REG22_09 (const unsigned int module);
    void CalcMUX_PIC0REG22_10 (const unsigned int module);
    void CalcMUX_PIC0REG22_11 (const unsigned int module);
    void CalcMUX_PIC0REG22_12 (const unsigned int module);
    void CalcMUX_PIC0REG22_13 (const unsigned int module);
    void CalcMUX_PIC0REG22_1617 (const unsigned int module);
    void CalcMUX_PIC0REG22_1819 (const unsigned int module);
    void CalcMUX_PIC0REG22_2021 (const unsigned int module);
    void CalcMUX_PIC0REG22_2223 (const unsigned int module);
    void CalcMUX_PIC0REG22_2425 (const unsigned int module);
    void CalcMUX_PIC0REG22_2627 (const unsigned int module);
    void CalcMUX_PIC0REG22_28 (const unsigned int module);
    void CalcMUX_PIC0REG202_29 (void);
    void CalcMUX_PIC0REG212_29 (void);
    void CalcMUX_PIC0REG22_30 (const unsigned int module);
    void CalcMUX_PIC0REG24 (const unsigned int module, const unsigned int index);
    void CalcMUX_PIC0REG30_00 (void);
    void CalcMUX_PIC0REG30_01 (void);
    void CalcMUX_PIC0REG30_0205 (void);
    void CalcMUX_PIC0REG30_0607 (void);
    void CalcMUX_PIC0REG30_0809 (void);
    void CalcMUX_PIC0REG30_1011 (void);
    void CalcMUX_PIC0REG30_1215 (void);
    void CalcMUX_PIC0REG30_1617 (const unsigned int type);
    void CalcMUX_PIC0REG30_18 (void);
    void CalcMUX_PIC0REG30_1920 (const unsigned int type);
    void CalcMUX_PIC0REG30_21 (void);
    void CalcMUX_PIC0REG30_22 (const unsigned int index);
    void CalcMUX_PIC0REG30_23 (void);
    void CalcMUX_PIC0REG30_24 (void);
    void CalcMUX_PIC0REG31_00 (void);
    void CalcMUX_PIC0REG31_0102 (void);
    void CalcMUX_PIC0REG31_03 (void);
    void CalcMUX_PIC0REG31_0405 (void);
    void CalcMUX_PIC0REG31_0608 (void);
    void CalcMUX_PIC0REG31_0910 (void);
    void CalcMUX_PIC0REG31_11 (void);
    void CalcMUX_PIC0REG31_1213 (void);
    void CalcMUX_PIC0REG31_14 (void);
    void CalcMUX_PIC0REG31_1517 (void);
    void CalcMUX_PIC0REG31_1819 (void);
    void CalcMUX_PIC0REG31_20 (void);
    void CalcMUX_PIC0REG31_2122 (void);
    void CalcMUX_PIC0REG31_23 (void);
    void CalcMUX_PIC0REG31_24 (void);
    void CalcMUX_PIC0REG31_25 (void);
    void CalcMUX_PIC0REG31_26 (void);
    void CalcMUX_PIC0REG32_00_08 (const unsigned int type);
    void CalcMUX_PIC0REG32_01_09 (const unsigned int type);
    void CalcMUX_PIC0REG32_0203 (void);
    void CalcMUX_PIC0REG32_04 (void);
    void CalcMUX_PIC0REG32_05 (void);
    void CalcMUX_PIC0REG32_1011 (void);
    void CalcMUX_PIC0REG32_12 (void);
    void CalcMUX_PIC0REG32_13 (void);
    void CalcMUX_PIC0REG33 (const unsigned int module);
    void CalcMUX_PIC0REG5_00 (const unsigned int module, const unsigned int index);
    void CalcMUX_PIC0REG50_0102 (void);
    void CalcMUX_PIC0REG51_0102 (void);
    void CalcMUX_PIC0REG5_03 (const unsigned int module, const unsigned int index);
    void CalcMUX_PIC0REG5_04 (const unsigned int module);
    void CalcMUX_PIC0REG5_0506 (const unsigned int module);
    void CalcMUX_PIC0REG5_07 (const unsigned int module);
    void CalcMUX_PIC0REG5_08 (const unsigned int module);
    void CalcMUX_PIC0REG5_10 (const unsigned int module);
    void CalcMUX_PIC0REG5_12 (const unsigned int module, const unsigned int index);
    void CalcMUX_PIC0REG5_13 (const unsigned int module, const unsigned int index);
    void CalcMUX_PIC0REG5_14 (const unsigned int module, const unsigned int index);
    
    /// Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    PIC1A_AgentController *mPIC1A_AgentController;
    std::string mModuleName;//Module name
    
    /// Internal variables
    bool mInputValue [243];                 // Store the value of input port
    bool mDT_Output [6];                    // Store the value of DTnm output
    bool mRS_Output [16];                   // Store the value of RSnm output
    bool mRTO_INInput [2][6];               // Store the value of IN input of RTOnm
    bool mRTO_TRGInput [2][6];              // Store the value of TRG input of RTOnm
    bool mRTO_Output [2][6];                // Store the value of RTOnm output
    bool mWNG_cOutput [2][3];               // Store the value of c output of WNGnm
    bool mWNG_dOutput [2][3];               // Store the value of d output of WNGnm
    bool mWNG_eOutput [2][3];               // Store the value of e output of WNGnm
    bool mWNG_gOutput [2][3];               // Store the value of g output of WNGnm
    bool mWNG_iOutput [2][3];               // Store the value of i output of WNGnm
    bool mWNG_abLatchOutput [2][3];         // Store the ouput value of Latch connected to WNGnm.(a&b) of WNGnm
    bool mWNG_dLatchOutput [2][3];          // Store the ouput value of Latch connected to WNGnm.d of WNGnm
    bool mWNG_eLatchOutput [2][3];          // Store the ouput value of Latch connected to WNGnm.e of WNGnm
    bool mWNG_FFgInput [2][3];              // Store the input value of FF connected to WNGnm.g of WNGnm
    bool mWNG_FFiInput [2][3];              // Store the input value of FF connected to WNGnm.i of WNGnm
    bool mFN_Output [2][6];                 // Store the output value of FNnm
    bool mFN_AInput [2][6];                 // Store the A input value of FNnm
    bool mFN_BInput [2][6];                 // Store the B input value of FNnm
    bool mPFN_In1Input [2][3];              // Store the U1/V1/W1 input values of FNnm
    bool mPFN_In2Input [2][3];              // Store the U2/V2/W2 input values of FNnm
    bool mREG513_FFInput [2][6];            // Store the input value of FFs in front of PIC0REG5n[13]
    bool mREG513_FFOutput [2][6];           // Store the output value of FFs in front of PIC0REG5n[13]
    bool mREG514_FFInput [2][6];            // Store the input value of FFs in front of PIC0REG5n[14]
    bool mREG514_FFOutput [2][6];           // Store the output value of FFs in front of PIC0REG5n[14]
    bool mTSGOUTCTR_FFInput [2][6];         // Store the input value of FFs in front of PIC0TSGOUTCTRn[0]
    bool mTSGOUTCTR_FFOutput [2][6];        // Store the output value of FFs in front of PIC0TSGOUTCTRn[0]
    bool mTRG_Output [2];                   // Store the output value of TRGn
    
    bool mMUX_PIC0ENCSEL40_0003Output [2];
    bool mMUX_PIC0ENCSEL40_07Output [2];
    bool mMUX_PIC0REG24Input [2][16];       // Store the value of MUX_PIC0REG2n4 Input
    bool mMUX_PIC0REG24Output [2][16];      // Store the value of MUX_PIC0REG2n4 Output
    bool mMUX_PIC0REG20_00Output [2];
    bool mMUX_PIC0REG20_01Output [2];
    bool mMUX_PIC0REG20_02Output [2];
    bool mMUX_PIC0REG20_03Output [2];
    bool mMUX_PIC0REG210_04Output;
    bool mMUX_PIC0REG20_05Output [2];
    bool mMUX_PIC0REG20_0809Output [2];
    bool mMUX_PIC0REG20_1011Output [2];
    bool mMUX_PIC0REG20_1213Output [2];
    bool mMUX_PIC0REG20_16Output [2];
    bool mMUX_PIC0REG20_18Output [2];
    bool mMUX_PIC0REG22_02Output [2];
    bool mMUX_PIC0REG22_03Output [2];
    bool mMUX_PIC0REG22_04Output [2];
    bool mMUX_PIC0REG22_08Output [2];
    bool mMUX_PIC0REG22_09Output [2];
    bool mMUX_PIC0REG22_10Output [2];
    bool mMUX_PIC0REG22_11Output [2];
    bool mMUX_PIC0REG22_12Output [2];
    bool mMUX_PIC0REG22_13Output [2];
    bool mMUX_PIC0REG22_1819Output [2];
    bool mMUX_PIC0REG212_29Output;
    bool mMUX_PIC0REG30_00Output;
    bool mMUX_PIC0REG30_01Output;
    bool mMUX_PIC0REG30_1617Output [3];
    bool mMUX_PIC0REG30_18Output;
    bool mMUX_PIC0REG30_1920Output [3];
    bool mMUX_PIC0REG30_21Output;
    bool mMUX_PIC0REG31_03Output;
    bool mMUX_PIC0REG31_0608Output;
    bool mMUX_PIC0REG31_1517Output;
    bool mMUX_PIC0REG33Output [2];
    bool mMUX_PIC0REG5_00Output [2][3];
    bool mMUX_PIC0REG5_08Output [2];
    bool mMUX_PIC0REG5_10Output [2];
    bool mMUX_PIC0REG5_12Output [2][3];
    bool mMUX_PIC0REG5_13Output [2][6];
    bool mMUX_PIC0REG5_14Output [2][6];
    
    /// Internal functions
    void Initialize (void);
    
    /// Declare virtual function of pic1a_regif class
    void cb_PIC0EN_PIC0EN0(RegCBstr str);
    void cb_PIC0SST_SYNCTRG(RegCBstr str);
    void cb_PIC0INI0_PIC0INI00(RegCBstr str);
    void cb_PIC0INI1_PIC0INI100(RegCBstr str);
    void cb_PIC0INI2_PIC0INI20(RegCBstr str);
    void cb_PIC0WNGMK_WNGMK0(RegCBstr str);
    void cb_PIC0WNGS_PIC0WNGS0(RegCBstr str);
    void cb_PIC0WNGFCLR_WNGFCLR0(RegCBstr str);
    void cb_PIC0LHSEL_PIC0LHSEL1(RegCBstr str);
    void cb_PIC0TSGOUTCTR_PIC0TSGOUTCTR0(RegCBstr str);
    void cb_PIC0HALLSEL_PIC0HALLSEL0(RegCBstr str);
    void cb_PIC0TAUDSEL_PIC0TAUDIN00(RegCBstr str);
    void cb_PIC0HIZCEN_PIC0HIZCEN0(RegCBstr str);
    void cb_PIC0ENCSEL40_PIC0ENCSEL403_0(RegCBstr str);
    void cb_PIC0REG20_PIC0REG2000(RegCBstr str);
    void cb_PIC0REG21_PIC0REG2101_00(RegCBstr str);
    void cb_PIC0REG22_PIC0REG2200(RegCBstr str);
    void cb_PIC0REG23_PIC0REG2302_00(RegCBstr str);
    void cb_PIC0REG24_PIC0REG2400(RegCBstr str);
    void cb_PIC0REG30_PIC0REG3000(RegCBstr str);
    void cb_PIC0REG31_PIC0REG3100(RegCBstr str);
    void cb_PIC0REG32_PIC0REG3200(RegCBstr str);
    void cb_PIC0REG33_PIC0REG3300(RegCBstr str);
    void cb_PIC0REG5_PIC0REG500(RegCBstr str);
};
#endif
