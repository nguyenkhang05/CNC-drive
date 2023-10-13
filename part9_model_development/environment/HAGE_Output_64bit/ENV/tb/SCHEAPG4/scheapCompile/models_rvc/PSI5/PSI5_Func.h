// ---------------------------------------------------------------------
// $Id: PSI5_Func.h,v 1.5 2020/04/23 07:12:11 hadinhtran Exp $
//
// Copyright(c) 2018 - 2020 Renesas Electronics Corporation
// Copyright(c) 2018 - 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __PSI5_FUNC_H__
#define __PSI5_FUNC_H__
#include <queue>
#include "BusSlaveFuncIf.h"
#include "psi5_regif.h"
#include "PSI5_AgentController.h"

class PSI5_Func: public Cpsi5_regif
                ,public BusSlaveFuncIf
{
private:
    enum eFifoInfo {//Define RXDFIFO and RXMFIFO info
        emFifoDepth = 2,
        emDATA = 0,
        emStatus = 1,
        emTime = 2,
        emFifoRegisterNum = 3
    };
    enum eInterruptId {//Define interrupt port id
        emINT_SYNCED = 0,
        emINT_SYNCST = 1,
        emINT_TXDEMPTY = 2,
        emINT_RXDSCNFERR = 3,
        emINT_RXDERR = 4,
        emINT_RXDFOVF = 5,
        emINT_RXDEXIST = 6,
        emINT_RXMFOVF = 7,
        emINT_RXMERR = 8,
        emINT_RXMEXIST = 9
    };
    enum eTimerIndex {//Define timer index
        emSlot1 = 0,
        emSlot2 = 1,
        emSlot3 = 2,
        emSlot4 = 3,
        emSlot5 = 4,
        emSlot6 = 5,
        emSlot7 = 6,
        emSlot8 = 7,
        emSyncPulseTimer = 8,
        emStampTimeCounter = 9
    };
    enum eSlotNumber {//Define the number of time slot
        emSlotNum = 8
    };
    enum eSerialMessageFrameNum { //Define the number of data frame of a serial message
        emMessageFrameNum = 18
    };
public:
    PSI5_Func (std::string name, 
               PSI5_AgentController *PSI5_AgentPtr);
    ~PSI5_Func (void);
    void SetChannelNumber (const unsigned short channel_num);
    void RegisterHandler (const std::vector<std::string>& args);
    void EnableReset (const bool is_active, const std::string clock_domain);
    void ProcessSyncPulseStart (void);
    void ProcessSyncPulseEnd (void);
    double CalculateSyncPulseWidth (const bool is_one_bit);
    double CalculateReceptionBitsTotalTime (const unsigned int slot_index);
    bool GetSyncPulseMode (void);
    bool GetSyncPulseDefault (void);
    unsigned int GetCommunicationMode (void);
    void UpdateIPTIMERValue (const unsigned int value);
    unsigned int GetIPTIMERValue (void);
    bool CheckTimerEnable (void);
    bool IsMasterModeTimer (void);
    bool CheckSlotEnable (const unsigned int slot_index);
    bool CheckChannelEnable (void);
    void StartSlotTimer (const unsigned int slot_index);
    void ProcessReceptionData (const unsigned int i_control_value
                              ,const unsigned int i_data_value
                              ,const unsigned int slot_index);
    unsigned int CalculateCRC (const unsigned int data, const unsigned int data_length
                               ,const unsigned int crc_length);
    void SendSyncPulseCollision (void);
    void UpdateCompletedTransmissionStatus (void);

    //Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    PSI5_AgentController *mPSI5_Agent;
    std::string mModuleName;
    std::queue<unsigned int> mRXDFIFO[emFifoRegisterNum];//Store the information of reception data
    std::queue<unsigned int> mRXMFIFO[emFifoRegisterNum];//Store the information of reception message
    bool mIsChannelEnable;              //Status of model enable
    bool mIsSoftwareResetEnable;        //Status of software enable
    bool mIsDataTransferring;           //Status of data transfer
    bool mIsRXDATAEmpty;                //Reading status of RXDATA register
    bool mIsRXDSTEmpty;                 //Reading status of RXDST register
    bool mIsRXDTIMEmpty;                //Reading status of RXDTIM register
    bool mIsRXMRXMSGEmpty;              //Reading status of RXMRXMSG register
    bool mIsRXMRXSTEmpty;               //Reading status of RXMRXST register
    bool mIsRXMRXTIMEmpty;              //Reading status of RXMRXTIM register
    bool mIsRXDFIFOOverflow;            //RXDFIFO status
    bool mIsRXMFIFOOverflow;            //RXMFIFO status
    bool mIsDataUpdated;                //Status of accessing to RXDFIFO
    unsigned int mRXDATA;               //The data of RXDATA
    unsigned int mRXDST;                //The data of RXDST
    unsigned int mRXDTIM;               //The data of RXDTIM
    bool mIsMsgUpdated;                 //Status of accessing to RXMFIFO
    unsigned int mRXMRXMSG;             //The data of RXMRXMSG
    unsigned int mRXMRXST;              //The data of RXMRXST
    unsigned int mRXMRXTIM;             //The data of RXMRXTIM
    unsigned char mRxdFifoReadTimes;    //Times of reading RXDFIFO register 
    unsigned char mEmuRxdFifoReadTimes; //Times of reading EMRXDFIFO register 
    unsigned char mRxmFifoReadTimes;    //Times of reading RXMFIFO register 
    unsigned char mEmuRxmFifoReadTimes; //Times of reading EMRXMFIFO register 
    unsigned char mReceivedDataFrameNum[emSlotNum]; //The number of the received data frame
    unsigned int mM0Value[emSlotNum][emMessageFrameNum]; //The value of M0 bit
    unsigned int mM1Value[emSlotNum][emMessageFrameNum]; //The value of M1 bit

    void Initialize (void);
    unsigned int GetCRCLength (const unsigned int slot_index);
    void UpdateReceptionDataStatus (const unsigned int slot_index
                                   ,const unsigned int received_data
                                   ,const unsigned int received_crc
                                   ,const bool is_start_bit_err
                                   ,const bool is_data_len_err
                                   ,const bool is_crc_err);
    void ProcessSerialMessage (const unsigned int slot_index
                              ,const unsigned int received_data
                              ,const bool is_frame_crc_err);
    void StoreSerialMessage (const unsigned int slot_index);
    unsigned int CollectMessageContent (const unsigned int slot_index);
    unsigned int CollectMessageStatus (const unsigned int slot_index);
    void StartMasterTimer (void);
    //Declare virtual function of psi5_regif class
    void cb_CHCTRL_CHEN(RegCBstr str);
    void cb_IPTIMERCTRL_TSCLR(RegCBstr str);
    void cb_IPTIMER_COUNTVAL(RegCBstr str);
    void cb_OPMCOMM_COMMODE(RegCBstr str);
    void cb_OPMBITRATE_BITRATECNT(RegCBstr str);
    void cb_OPMCYCT_TTTTCNT(RegCBstr str);
    void cb_EMRXDFIFO_RXDST(RegCBstr str);
    void cb_EMRXMFIFO_RXMFIFO(RegCBstr str);
    void cb_TXSETTING_SHORTCNT(RegCBstr str);
    void cb_SYNCCTRL_VALTIMSYNC(RegCBstr str);
    void cb_TXSTCLR_SYNCSTCLR(RegCBstr str);
    void cb_TXSTINTEN_TXDEMPTYINTEN(RegCBstr str);
    void cb_TXDATA_TXDATA(RegCBstr str);
    void cb_RXSPLSET_SMPLPROD(RegCBstr str);
    void cb_RXWINSET_DWINSTPOS(RegCBstr str);
    void cb_RXSSET_OFFSETCNT(RegCBstr str);
    void cb_RXDATA_RXDATA(RegCBstr str);
    void cb_RXDST_RXSTATUS(RegCBstr str);
    void cb_RXDTIM_RXDTIM(RegCBstr str);
    void cb_RXDFIFO_RXDST(RegCBstr str);
    void cb_RXMODSTCLR_RXDFOVFCLR(RegCBstr str);
    void cb_RXMODSTINTEN_RXDEXISTINTEN(RegCBstr str);
    void cb_RXMSET_RXM1EN(RegCBstr str);
    void cb_RXMRXMSG_DATAFIELD(RegCBstr str);
    void cb_RXMRXST_RXSTATUS(RegCBstr str);
    void cb_RXMRXTIM_RXMTIM(RegCBstr str);
    void cb_RXMFIFO_RXMFIFO(RegCBstr str);
    void cb_RXMMSTCLR_RXMFOVFCLR(RegCBstr str);
    void cb_RXMMSTINTEN_RXMEXISTINTEN(RegCBstr str);
    void UpdateMirrorRegister(unsigned int OffsetAdress);
};
#endif
