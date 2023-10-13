// ---------------------------------------------------------------------
// $Id: MSPI000_Func.h,v 1.7 2020/03/11 00:52:43 ducla Exp $
//
// Copyright(c) 2017-2020 Renesas Electronics Corporation
// Copyright(c) 2017-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __MSPI000_FUNC_H__
#define __MSPI000_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "mspi000_regif.h"
#include "global.h"

#define MAX_CH_NUM 16

class MSPI000_AgentController;
class MSPI000_Func: public Cmspi000_regif
                ,public BusSlaveFuncIf
{
public:
    MSPI000_Func(std::string name, unsigned int ch_num, unsigned int cs_num,  MSPI000_AgentController* AgentController);
    ~MSPI000_Func();

    //user function here
    void RegisterHandler (const std::vector<std::string>& args);
    void EnableReset (const bool is_active);
    //

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

    //Mem
    bool SetDataMem(unsigned int offset, unsigned int value, unsigned int size);
    bool GetDataMem(unsigned int offset, unsigned int& data, unsigned int size);
    void SetMemSize (unsigned int id);

    unsigned int Get_SPI_Control_Setting(unsigned int chid);

    //Get/Set Registers
    unsigned int GetMSPInCSP();
    unsigned int GetMSPInCKR();
    unsigned int GetMSPInSOLS();
    unsigned int GetMSPInMSSEL();
    unsigned int GetMSPInSAMP();
    unsigned int GetMSPInCSIE();

    unsigned int GetMSPInCPOLm(unsigned int chid);
    unsigned int GetMSPInCPHAm(unsigned int chid);
    unsigned int GetMSPInDIRm(unsigned int chid);
    unsigned int GetMSPInFLENm(unsigned int chid);
    unsigned int GetMSPInIEREm(unsigned int chid);
    unsigned int GetMSPInIFEEm(unsigned int chid);
    unsigned int GetMSPInIRXEm(unsigned int chid);
    unsigned int GetMSPInITXEm(unsigned int chid);
    unsigned int GetMSPInCSRm(unsigned int chid);
    unsigned int GetMSPInCFSETm(unsigned int chid);
    unsigned int GetMSPInLOCKm(unsigned int chid);
    unsigned int GetMSPInJOBENm(unsigned int chid);
    unsigned int GetMSPInPSm(unsigned int chid);
    unsigned int GetMSPInDECHKm(unsigned int chid);
    unsigned int GetMSPInSAFSm(unsigned int chid);
    unsigned int GetMSPInSAFCMm(unsigned int chid);
    unsigned int GetMSPInCFCNTm(unsigned int chid);
    unsigned int GetMSPInFCCEm(unsigned int chid);
    unsigned int GetMSPInCSRIm(unsigned int chid);
    unsigned int GetMSPInRXRQFm(unsigned int chid);

    void SetMSPInTXRQFm(unsigned int chid, unsigned int value);
    void SetMSPInRXRQFm(unsigned int chid, unsigned int value);

    void SetMSPInCFCNTm(unsigned int chid, unsigned int value);
    void SetMSPInRXDAm0(unsigned int chid, unsigned int value);
    void SetMSPInCESTm(unsigned int chid, bool ovree, bool ovwre, bool ovrue, bool dce, bool ce, bool pe);
    void SetMSPInCHENm(unsigned int chid, unsigned int value);
    void SetMSPInCSF(unsigned int value);
    void SetMSPInCNUMF(unsigned int value);
    void SetMSPInFIRXNm(unsigned int chid);
    void SetMSPInDCEFSTm(unsigned int chid, unsigned int value);
    void SetMSPInCEFSTm(unsigned int chid, unsigned int value);
    void SetMSPInPEFSTm(unsigned int chid, unsigned int value);

    double GetmMSPInSCKbaudarate (unsigned int chid);
    unsigned int GetMSPInTXDAm0  (unsigned int chid);
    unsigned int GetRAMValue (unsigned int chid, unsigned int size);
    void SetRAMValue (unsigned int chid, unsigned int value, unsigned int size);
    unsigned int GetMSPInPRIOm  (unsigned int chid);
    unsigned int GetMSPInFIDLm  (unsigned int chid);
    unsigned int GetMSPInICLSm  (unsigned int chid);

    void GetDelayValue (unsigned int chid, unsigned int delay_value[4]);
    bool GetIsLoopBack(void);
    bool GetIsDataConsistency(void);
    bool GetmHalfFIFOFilled(unsigned int chid);
    bool GetmIsSafeSPI(unsigned int chid);

    void CheckSameValue(unsigned int i, unsigned int chid);

private:
    // enum indicate commmunication mode
    enum eCommunicationMode {
           emNo_Comunication         = 0                  // No mode
           ,emTrans_Rep              = 1                  //Transmission/Reception
           ,emTrans_Only             = 2                  //Transmission only
           ,emRep_Only               = 3                  //Reception only
    };
    // enum indicate memory mode
    enum eMemoryMode {
           emDirectMem               = 0                  //Direct memory mode
           ,emFixedBufferMem         = 1                  //Fixed buffer memory mode
           ,emFixedFifoMem           = 2                  //Fixed FIFO memory mode
    };
    // memmory info
     enum eMEMInfo{
        emDMEMType                                        // Mem type
        ,emStartOffsetDMEM  = 0x1000                      // Start offset of memory
        ,emDMEMSize         = 0x800                       // Max memory size
    };
     eCommunicationMode mpCom[MAX_CH_NUM];                // Indicate communication mode
     eMemoryMode        mpMem[MAX_CH_NUM];                // Indicate memmory mode
    // Variables
    MSPI000_AgentController     *mpAgentController;       // Agent controller pointer
    std::string                 mModuleName;              // Module name
    unsigned int                mNumChannel;              // Number of channels

    bool mIPEN;                                           // Indicate IP Enable
    bool mIsMaster;                                       // Indicate Master mode
    bool mIsLoopBack;                                     // Indicate Loop-back mode
    bool mIsDataConsistency;                              // Indicate Data consistency check enable.
    bool mIsCHEN[MAX_CH_NUM];                             // Indicate channel is enable or not
    bool mIsSafeSPI[MAX_CH_NUM];                          // Indicate channel is safe spi or not
    unsigned int mSizes[MAX_CH_NUM];                       // Indicate size step MEM of each channel

    double mMSPInSCKbaudarate[MAX_CH_NUM];
    // Mem
    unsigned char           *mDMEM;                        // Data memory
    unsigned int            mStartTX[MAX_CH_NUM];          // Start data pointer for transmission (used in DMEM)
    unsigned int            mCurTX[MAX_CH_NUM];            // Current data pointer for transmission(used in DMEM)
    unsigned int            mCurTX1[MAX_CH_NUM];           // Current data pointer for transmission(used in DMEM)
    unsigned int            mStartRX[MAX_CH_NUM];          // Start data pointer for reception(used in DMEM)
    unsigned int            mCurRX[MAX_CH_NUM];            // Current data pointer for reception(used in DMEM)
    unsigned int            mCurRX1[MAX_CH_NUM];           // Current data pointer for reception(used in DMEM)
    unsigned int            mMemSize;                      // Store memory size
    bool mAccessRAM;                                       // access ram or no
    bool mHalfFIFOFilled[MAX_CH_NUM];                      // Indicate half of FIFO is filled
    unsigned int mCount_read_rx[MAX_CH_NUM];                // count read FIFO

    // Function user define
    void Initialize(void);
    bool CheckValidAccessMem(eMEMInfo mem_type, unsigned int addr, unsigned int size, bool is_wr, bool debug);

    //Callback function
    void cb_MSPInCTL0_MSPInEN(RegCBstr str);
    void cb_MSPInCTL1_MSPInMSSEL(RegCBstr str);
    void cb_MSPInCTL2_MSPInLBM(RegCBstr str);
    void cb_MSPInCSTSm_MSPInACTFSm(RegCBstr str);
    void cb_MSPInCSTCm_MSPInCHENCm(RegCBstr str);
    void cb_MSPInCFGm0_MSPInTXEm(RegCBstr str);
    void cb_MSPInCFGm1_MSPInCPOLm(RegCBstr str);
    void cb_MSPInCFGm2_MSPInFLENm(RegCBstr str);
    void cb_MSPInCFGm3_MSPInPRCSm(RegCBstr str);
    void cb_MSPInCFGm4_MSPInHWTSm(RegCBstr str);
    void cb_MSPInRASTADm_MSPInRASTAD(RegCBstr str);
    void cb_MSPInSEUPm_MSPInSEUPm(RegCBstr str);
    void cb_MSPInHOLDm_MSPInHOLDm(RegCBstr str);
    void cb_MSPInIDLEm_MSPInIDLEm(RegCBstr str);
    void cb_MSPInINDAm_MSPInINDAm(RegCBstr str);
    void cb_MSPInCFSETm_MSPInCFSETm(RegCBstr str);
    void cb_MSPInSSELm_MSPInJOBENm(RegCBstr str);
    void cb_MSPInTXDAm0_MSPInTXDAm0(RegCBstr str);
    void cb_MSPInFRERSTCm_MSPInDCEFSCm(RegCBstr str);
    void cb_MSPInRXDAm0_MSPInRXDAm0(RegCBstr str);
    void cb_MSPInSSTXm_MSPInTXDAm0(RegCBstr str);
    void cb_MSPInFSRXm_MSPInRXDAm0(RegCBstr str);
    void cb_MSPInCESTCm_MSPInOVREECm(RegCBstr str);
};
#endif //__MSPI000_FUNC_H__
