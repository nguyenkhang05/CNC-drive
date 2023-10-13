// ---------------------------------------------------------------------
// $Id: MMC44_Func.h,v 1.2 2020/03/19 08:33:32 anhnguyen4 Exp $
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

#ifndef __MMC44_FUNC_H__
#define __MMC44_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "mmc44_regif.h"
#include "MMC44_AgentController.h"
#include "global.h"

/// utility libraries
#include <stdint.h>
#include <string>
using std::string;

class MMC44_AgentController;
class MMC44_Func : public Cmmc44_regif, public BusSlaveFuncIf
{
    friend class MMC44;

  public:
    MMC44_Func(std::string iname, MMC44_AgentController *iAgentController);
    ~MMC44_Func();
    //------------------------------------------------------------------------------
    // implement the role of a BusSlaveFuncIf the interface allows read/write
    // access into register block from the Initiator via the BusSlaveIf (the wrapper)
    //------------------------------------------------------------------------------
    void read(uint32_t offsetAddress, TlmBasicPayload &trans, BusTime_t *t, bool debug);
    void write(uint32_t offsetAddress, TlmBasicPayload &trans, BusTime_t *t, bool debug);
    void Initialize(void);
    //------------------------------------------------------------------------------
    // Function User defined
    //------------------------------------------------------------------------------
    eBufferState GetBufferState(eBufferNumber bufNum);                          ///< Get buffer state
    void SaveDMAValue(bool DMAValue);                                           ///< Save value of DMA port to variable
    void SetDataFromDataRegisterToBuffer(void);                                 ///< Set Data From Data Register To Buffer
    void WriteDataToBufferProcess(void);                                        ///< CPU/DMA Write data to buffer process
    void ReadDataFromBufferProcess(void);                                       ///< CPU/DMA Read data from buffer process
    void IncreaseWriteDataPointer(void);                                        ///< Increase write data register
    // Buffer
    bool Is2BufferNotEmpty(void);                                               ///< Check whether both 2 buffer is empty or not
    bool IsReadingBuffer(void);                                                 ///< Check CPU/DMA is reading buffer or not
    bool IsBufferWaitting(void);                                                ///< Check Buffer is waiting for send to card or not
    bool IsBufferSending(void);                                                 ///< Check does have any buffer is sending or not.
    void SetDataFromBufferToDataRegister();                                     ///< Set data from buffer to data register
    void SetBufferState(eBufferNumber number, eBufferState state);              ///< Set buffer state
    void IncreaseReadDataPointer(void);                                         ///< Increase read data pointer

    // Communication with card
    uint32_t GetDataControlValue(eSTBE stbe);                                   ///< Get Data control value in order to write to data_control port
    uint32_t GetCommandControlValue(eSTBE stbe, eCommandLineSignal signal);     ///< Get command control value in order to write to cmd_control port
    uint8_t *IniCMDFrame(bool isCMD12);                                         ///< Initialize Command frame
    void CheckResponseFrame(uint8_t *response_ptr);                             ///< Check response frame
    bool CheckResponseSize(uint16_t size);                                      ///< Check response size
    void SendDataToBuffer(uint8_t *ptr);                                        ///< Send data to buffer
    void SetTimeDataTrans(const uint32_t dataLength);                           ///< Set time to transmission data
    uint8_t *IniDataFrame(eBufferNumber bufferNum);                             ///< Initialize data frame
    uint32_t SwapByte(uint32_t orgValue);                                       ///< Swap byte of 32 bit variable
    bool IsDMAWriteEnable(void);                                                ///< Check whether DMA write enable or not
    bool IsDMAReadEnable(void);                                                 ///< Check whether DMA read enable or not
    void SetDataLineState(eLineState lineState);                                ///< Set data line state
    bool IsAllBlockHadReaded(void);                                             ///< Check whether all block had read or not
    bool PreCheckCommandSettingError(void);                                     ///< Pre check command setting
    bool IsCMD12(void);                                                         ///< Check is CMD12 or normal command
    uint8_t ComputeCRC7(uint8_t *dataToCalculateCRC7, uint32_t length);         ///< Compute CRC7
    uint16_t ComputeCRC16(uint8_t *dataToCalculateCRC16, uint32_t length);      ///< Compute CRC16
    void SetIsCMD12Flag(bool value);                                            ///< Set mIsCMD12 flag
    uint32_t GetTimeDataTrans();                                                ///< Get time need for transmit data
    void UpdateReadDataPtr(eBufferState trigger);                               ///< Update read data pointer
    void CheckFirstByteResponse(uint8_t index);                                 ///< Check first byte of response frame
    //------------------------------------------------------------------------------
    // functions to get/set register
    //------------------------------------------------------------------------------
    void RegisterHandler(const std::vector<std::string> &args);                 ///< Register handler
    void SetMMCBUSBSY(bool isBusy);                                             ///< Set MMC bus to busy
    void SetTimeOutAndErrSttBits(uint32_t bit_name, uint32_t value);            ///< Set value of bits in HOST_STS2 register
    uint32_t GetDATARegister(void);                                             ///< Get DATA register
    void SetMMC44Status(uint32_t bit_name, uint32_t value);                     ///< Set value of bits in HOST_STS1 register
    uint32_t GetMMC44Status(uint32_t bit_name);                                 ///< Get value of bits in HOST_STS1 register
    uint32_t GetCommandSetting(uint32_t bit_name);                              ///< Get value of bits in CMD_SET register
    uint32_t GetTransferBlockSetting(uint32_t bit_name);                        ///< Get value of bits in BLOCK_SET register
    uint32_t GetClockControl(uint32_t bit_name);                                ///< Get value of bits in CLK_CTRL register
    uint32_t GetDMATransferSetting(void);                                       ///< Get DMA Transfer Setting
    uint32_t GetBootOpSetting(uint32_t bit_name);                               ///< Get value of bits in BOOT register
    uint32_t GetInterruptEnable(uint32_t bit_name);                             ///< Get value of bits in INT_EN register
    void SetInterruptFlagRegister(uint32_t bit_name, uint32_t value);           ///< set value of bits in INT register
    void SetDataRegister(uint32_t value);                                       ///< Set DATA register
    void AssertReset(bool isReset);                                             ///< Assert Reset
    uint32_t GetMMCAnCE_HOST_STS1(void);                                        ///< Get HOST_STS1 register
    uint32_t GetMMCAnCE_HOST_STS2(void);                                        ///< Get HOST_STS2 register


  public:
    uint8_t mArrayData[517];                                                    ///< Store block data to prepare send to card
    uint8_t mArrayCommand[17];                                                  ///< Store command frame to prepare to send to card

  private:
    // Variables
    MMC44_AgentController *mpAgentController;                                   ///< Agent controller pointer
    std::string mModuleName;                                                    ///< Module name

    uint32_t mBuffer1[128];                                                     ///< Buffer 1
    uint32_t mBuffer2[128];                                                     ///< Buffer 2
    uint32_t mBusWidthLength;                                                   ///< Store Bus width length

    bool mIsCmd12Flag;                                                          ///< Store is command 12 or normal command
    uint32_t mTimeData;                                                         ///< Store time need to transmit data to card
    eBufferState mBuffer1State;                                                 ///< Store buffer 1 state
    eBufferState mBuffer2State;                                                 ///< Store buffer 2 state
    uint32_t mDataReadCount;                                                    ///< Store number of read that CPU/DMA need to read for 1 block
    uint32_t mDataWriteCount;                                                   ///< Store number of write that CPU/DMA need to write for 1 block
    uint32_t *mReadDataPtr;                                                     ///< Store Read data pointer (using by CPU/DMA to read data from buffer)
    uint32_t *mWriteDataPtr;                                                    ///< Store Read data pointer (using by CPU/DMA to write data to buffer)
    bool mDMAValueSaved;                                                        ///< Store value of DMA port
    bool mIsFirstByteOfBlockWritten;                                            ///< Store is first byte of block written or not
    bool mIsDMAAccess;                                                          ///< Store Is DMA Access or CPU Access
    bool mIsCpuWritingToBuffer;                                                 ///< Store Is CPU writing to buffer or not
    bool mIsCpuReadingFromBuffer;                                               ///< Store Is CPU reading from buffer or not
    bool mIsSoftwareResetEnable;                                                ///< Store is software reset enable or not

    //------------------------------------------------------------------------------
    // implement pure virtual call-back function, the means allowing base class
    // register block Cmmc44_regif inform the derived class
    //------------------------------------------------------------------------------
    void cb_MMCAnCE_CMD_SET_BOOT(RegCBstr str);
    void cb_MMCAnCE_CMD_SET_DATW(RegCBstr str);
    void cb_MMCAnCE_ARG_ARG(RegCBstr str);
    void cb_MMCAnCE_ARG_CMD12_C12ARG(RegCBstr str);
    void cb_MMCAnCE_CMD_CTRL_BREAK(RegCBstr str);
    void cb_MMCAnCE_CLK_CTRL_CLKEN(RegCBstr str);
    void cb_MMCAnCE_BUF_ACC_DMATYP(RegCBstr str);
    void cb_MMCAnCE_DATA_DATA(RegCBstr str);
    void cb_MMCAnCE_BOOT_BTCLKDIV(RegCBstr str);
    void cb_MMCAnCE_INT_CMD12DRE(RegCBstr str);
    void cb_MMCAnCE_INT_EN_MBUFWEN(RegCBstr str);
    void cb_MMCAnCE_SWRESA_SWRST(RegCBstr str);
};

#endif //__MMC44_FUNC_H__
