// ---------------------------------------------------------------------
// $Id: MMC44_Func.cpp,v 1.3 2020/04/06 07:28:44 anhnguyen4 Exp $
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

#include "MMC44_Func.h"

/**@brief
 *
 */
MMC44_Func::MMC44_Func(std::string iname,
                       MMC44_AgentController *iAgentController)
: Cmmc44_regif(iname, 32)
, mpAgentController(iAgentController)
, mModuleName(iname)
, mBuffer1State(emCanWrite)
, mBuffer2State(emCanWrite)
, mIsCmd12Flag(false)
, mTimeData(0)
, mReadDataPtr(0)
, mDataReadCount(0)
, mDataWriteCount(0)
, mWriteDataPtr(0)
, mIsDMAAccess(false)
, mIsCpuWritingToBuffer(0)
, mIsCpuReadingFromBuffer(0)
, mBusWidthLength(1)
, mIsSoftwareResetEnable(false)
, mIsFirstByteOfBlockWritten(true)
, mDMAValueSaved(1)
{
    sc_assert(mpAgentController != NULL);
    mpAgentController->CommandInit(iname);

    /// -# initialize the *register block* base class
    Cmmc44_regif::set_instance_name(iname);

    memset(mArrayCommand, 0, 17);
    memset(mArrayData, 0, 517);
    memset(mBuffer1, 0, 128);
    memset(mBuffer2, 0, 128);
}

/**@brief
 *
 */
MMC44_Func::~MMC44_Func()
{
}

/**@brief
 *
 */
void MMC44_Func::read(uint32_t offsetAddress, TlmBasicPayload &trans, BusTime_t *t, bool debug)
{
    /// -# get G3m extended information of the receive transaction
    TlmG3mExtension *pExtension;
    trans.get_extension(pExtension);

    /// Clear data buffer
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    /// Extract SPID information for checking data
    ///set value as default. true: DMA
    ///                      False: CPU
    mIsDMAAccess = false;
    // When clock = 0, or reset state, only debug master can read register
    if ((!mpAgentController->CheckClockPeriod("CLK_HSB")) || (mpAgentController->GetResetStatus())) {
        if (pExtension != NULL) {
            if (!(debug && (pExtension->getPeId() == DEBUG_MASTER_BMID))) {
                re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
                return;
            }
        }else{
            re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
            return;
        }
    }

    if (pExtension != NULL){
        mIsDMAAccess = pExtension->isDmaAccess();
    }
    if (!debug){
        reg_rd((uint32_t)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }else{
        reg_rd_dbg((uint32_t)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}

/**@brief
 *
 */
void MMC44_Func::write(uint32_t offsetAddress, TlmBasicPayload &trans, BusTime_t *t, bool debug)
{
    uint32_t addr = (uint32_t)trans.get_address() & 0x7F;
    /// Get extension info
    TlmG3mExtension *pExtension;
    trans.get_extension(pExtension);

    // When clock = 0, only debug master write
    if (!(mpAgentController->CheckClockPeriod("CLK_HSB"))){
        if (pExtension != NULL) {
            if (!(debug && (pExtension->getPeId() == DEBUG_MASTER_BMID))) {
                re_printf("warning", "Cannot access register when clock is 0.\n");
                return;
            }
        }else{
            re_printf("warning", "Cannot access register when clock is 0.\n");
            return;
        }
    }else if (mIsSoftwareResetEnable && (addr != 0x7C)){ /// Check Software reset register's address
        re_printf("warning", "Cannot write during reset period\n");
        return;
    }else{
        mIsDMAAccess = false;
        if (pExtension != NULL){
            mIsDMAAccess = pExtension->isDmaAccess();
        }
    }
    if (!debug){
        reg_wr((uint32_t)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }else{
        reg_wr_dbg((uint32_t)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}

/**@brief
 *
 */
void MMC44_Func::Initialize()
{
    memset(mArrayCommand, 0, 17);
    memset(mArrayData, 0, 517);
    memset(mBuffer1, 0, 128);
    memset(mBuffer2, 0, 128);

    mBusWidthLength = 0x1;
    mTimeData = 0x0;
    mBuffer1State = emCanWrite;
    mBuffer2State = emCanWrite;
    mDataReadCount = 0x0;
    mDataWriteCount = 0x0;
    mWriteDataPtr = NULL;
    mReadDataPtr = NULL;
    mDMAValueSaved = true;

    mIsDMAAccess = false;
    mIsCpuWritingToBuffer = false;
    mIsCpuReadingFromBuffer = false;
    mIsCmd12Flag = false;
    mIsFirstByteOfBlockWritten = true;
}

//-----------------------------------------------------------------------------------------------------------
//                                      Function user define
//-----------------------------------------------------------------------------------------------------------

/**@brief
 * Get buffer state
 */
eBufferState MMC44_Func::GetBufferState(eBufferNumber bufNum)
{ //{{{
    if (bufNum == emBuffer1)
    {
        return mBuffer1State;
    }
    else
    {
        return mBuffer2State;
    }
} //}}}

/**@brief
 * Save value of DMA port to variable
 */
void MMC44_Func::SaveDMAValue(bool DMAValue)
{ //{{{
    mDMAValueSaved = DMAValue;
} //}}}

/**@brief
 * Swap byte of 32 bit variable
 */
uint32_t MMC44_Func::SwapByte(uint32_t orgValue)
{ //{{{
    uint32_t swapVal = 0;

    uint32_t orgByte1 = orgValue & 0xFF;
    uint32_t orgByte2 = (orgValue >> 8) & 0xFF;
    uint32_t orgByte3 = (orgValue >> 16) & 0xFF;
    uint32_t orgByte4 = (orgValue >> 24) & 0xFF;

    swapVal = (orgByte1 << 24) | (orgByte2 << 16) | (orgByte3 << 8) | orgByte4;

    return swapVal;
} //}}}

/**@brief
 * Set data line state
 */
void MMC44_Func::SetDataLineState(eLineState lineState)
{ //{{{
    if (lineState == emActive)
    {
        if (GetCommandSetting(emDATW) == 0)
        {
            SetMMC44Status(emDATSIG, 1);
        }
        else if (GetCommandSetting(emDATW) == 1)
        {
            SetMMC44Status(emDATSIG, 0x0F);
        }
        else if (GetCommandSetting(emDATW) == 2)
        {
            SetMMC44Status(emDATSIG, 0xFF);
        }
    }
    else if (lineState == emDeActive)
    {
        SetMMC44Status(emDATSIG, 0x0);
    }
    else if (lineState == emActiveLine0)
    {
        SetMMC44Status(emDATSIG, 1);
    }
} //}}}

/**@brief
 * Check CPU/DMA is reading buffer or not
 */
bool MMC44_Func::IsReadingBuffer(void)
{ //{{{
    if ((mBuffer1State == emReading) || (mBuffer2State == emReading))
    {
        return true;
    }
    return false;
} //}}}

/**@brief
 * Check whether DMA write enable or not
 */
bool MMC44_Func::IsDMAWriteEnable(void)
{ //{{{
    return ((bool)(*MMCAnCE_BUF_ACC)["DMATYP"] && (bool)(*MMCAnCE_BUF_ACC)["DMAWEN"]);
} //}}}

/**@brief
 * Check whether DMA read enable or not
 */
bool MMC44_Func::IsDMAReadEnable(void)
{ //{{{
    return (bool)((*MMCAnCE_BUF_ACC)["DMATYP"] && (*MMCAnCE_BUF_ACC)["DMAREN"]);
} //}}}

/**@brief
 * Get Data control value in order to write to data_control port
 */
uint32_t MMC44_Func::GetDataControlValue(eSTBE stbe)
{ //{{{
    uint32_t dataControl = 0;
    dataControl |= (GetTransferBlockSetting(emBLKSZ) + emStartBitLength + emEndBitLength + emCRC16Length) << 16;
    dataControl |= (uint32_t)(emDATS) << 4;
    dataControl |= (stbe);
    return dataControl;
} //}}}

/**@brief
 * Check response size
 */
bool MMC44_Func::CheckResponseSize(uint16_t size)
{ //{{{
    if ((GetCommandSetting(emRTYP) == 0x1) && (size != 6))
    {
        return false;
    }
    else if ((GetCommandSetting(emRTYP) == 0x2) && (size != 17))
    {
        return false;
    }
    return true;
} //}}}

/**@brief
 * Get command control value in order to write to cmd_control port
 */
uint32_t MMC44_Func::GetCommandControlValue(eSTBE stbe, eCommandLineSignal signal)
{ //{{{
    uint32_t commandControl = 0;
    commandControl |= (uint32_t)emCommandSize << 16;
    commandControl |= signal << 4;
    commandControl |= stbe;
    return commandControl;
} //}}}

/**@brief
 * Set mIsCMD12 flag
 */
void MMC44_Func::SetIsCMD12Flag(bool value)
{ //{{{
    mIsCmd12Flag = value;
} //}}}

/**@brief
 * Check is CMD12 or normal command
 */
bool MMC44_Func::IsCMD12(void)
{ //{{{
    return mIsCmd12Flag;
} //}}}

/**@brief
 * CPU/DMA Write data to buffer process
 */
void MMC44_Func::WriteDataToBufferProcess(void)
{   //{{{
    //    mIsCpuWritingToBuffer = true;
    if (mIsFirstByteOfBlockWritten)
    {
        if ((GetTransferBlockSetting(emBLKSZ) % 4) != 0)
        {
            mDataWriteCount = (GetTransferBlockSetting(emBLKSZ) / 4) + 1;
        }
        else
        {
            mDataWriteCount = (GetTransferBlockSetting(emBLKSZ) / 4);
        }
    }

    if (mDataWriteCount == 1)
    {
        if (mIsFirstByteOfBlockWritten)
        {
            SetBufferState(emBuffer1, emWritting);
            mWriteDataPtr = mBuffer1;
        }
        mDataWriteCount--;
        // one block had written
        SetDataFromDataRegisterToBuffer();
        mpAgentController->AssertOneBlockHadWrittenEvent();
        mIsFirstByteOfBlockWritten = true;
    }
    else
    {
        if (mIsFirstByteOfBlockWritten)
        {
            if ((mBuffer1State == emCanWrite))
            {
                SetBufferState(emBuffer1, emWritting);
                mWriteDataPtr = mBuffer1;
            }
            else if (mBuffer2State == emCanWrite)
            {
                SetBufferState(emBuffer2, emWritting);
                mWriteDataPtr = mBuffer2;
            }
        }
        mDataWriteCount--;
        SetDataFromDataRegisterToBuffer();
        IncreaseWriteDataPointer();
        mIsFirstByteOfBlockWritten = false;
    }
} //}}}


/**@brief
 *
 */
bool MMC44_Func::IsBufferWaitting(void)
{ //{{{
    if ((mBuffer1State == emWaiting) || (mBuffer2State == emWaiting))
    {
        return true;
    }
    return false;
} //}}}

/**@brief
 * Check does have any buffer is sending or not.
 */
bool MMC44_Func::IsBufferSending(void)
{ //{{{
    if ((mBuffer1State == emSending) || (mBuffer2State == emSending))
    {
        return true;
    }
    return false;
} //}}}

/**@brief
 * Increase write data register
 */
void MMC44_Func::IncreaseWriteDataPointer(void)
{ //{{{
    if ((bool)mWriteDataPtr)
    {
        mWriteDataPtr++;
    }
} //}}}

/**@brief
 * Set Data From Data Register To Buffer
 */
void MMC44_Func::SetDataFromDataRegisterToBuffer(void)
{ //{{{
    if (!((bool)(*MMCAnCE_BUF_ACC)["ATYP"]))
    {
        if ((bool)mWriteDataPtr)
        {
            *mWriteDataPtr = GetDATARegister();
        }
    }
    else
    {
        if ((bool)mWriteDataPtr)
        {
            *mWriteDataPtr = SwapByte(GetDATARegister());
        }
    }

} //}}}

/**@brief
 * CPU/DMA Read data from buffer process
 */
void MMC44_Func::ReadDataFromBufferProcess(void)
{ //{{{
    re_printf("info", "Reading Data From Buffer.");
    if (mDataReadCount == 1)
    {
        re_printf("info", "One block had read.");
        // one block had read
        // increase block counter
        uint32_t value = GetMMC44Status(emRCVBLK) + 1;
        SetMMC44Status(emRCVBLK, value);
        mpAgentController->AssertOneBlockHadReadEvent();
        mDataReadCount--;
    }
    else
    {
        IncreaseReadDataPointer();
        SetDataFromBufferToDataRegister();
        mDataReadCount--;
    }
} //}}}

/**@brief
 * Check whether both 2 buffer is empty or not
 */
bool MMC44_Func::Is2BufferNotEmpty()
{ //{{{
    if ((mBuffer1State == emCanWrite) || (mBuffer2State == emCanWrite))
    {
        return false;
    }
    return true;
} //}}}

/**@brief
 * Increase read data pointer
 */
void MMC44_Func::IncreaseReadDataPointer()
{ //{{{
    if ((bool)mReadDataPtr)
    {
        mReadDataPtr++;
    }
} //}}}

/**@brief
 * Check whether all block had read or not
 */
bool MMC44_Func::IsAllBlockHadReaded()
{ //{{{
    if ((GetCommandSetting(emCMLTE) == 0) || (GetTransferBlockSetting(emBLKCNT) == GetMMC44Status(emRCVBLK)))
    {
        return true;
    }
    return false;
} //}}}

/**@brief
 * Set data from buffer to data register
 */
void MMC44_Func::SetDataFromBufferToDataRegister()
{ //{{{
    uint32_t data = 0;
    if ((bool)mReadDataPtr)
    {
        data = *mReadDataPtr;
    }
    if (!((bool)(*MMCAnCE_BUF_ACC)["ATYP"]))
    {
        SetDataRegister(data);
    }
    else
    {
        SetDataRegister(SwapByte(data));
    }
} //}}}

/**@brief
 * Set time to transmission data
 */
void MMC44_Func::SetTimeDataTrans(const uint32_t dataLength)
{ //{{{
    mTimeData = (dataLength * 8 / mBusWidthLength) + emCRC16Length * 8 + emEndBitLength + emStartBitLength;
} //}}}

/**@brief
 * Get time need for transmit data
 */
uint32_t MMC44_Func::GetTimeDataTrans()
{ //{{{
    return mTimeData;
} //}}}

/**@brief
 * Pre check command setting
 */
bool MMC44_Func::PreCheckCommandSettingError(void)
{ //{{{
    bool isCommandSettingError = false;
    if (((*MMCAnCE_CMD_SET)["RTYP"] == 0x0) && ((*MMCAnCE_CMD_SET)["RBSY"] == 0x1))
    {
        re_printf("warning", "Illegal setting has been made in MMCAnCE_CMD_SET or MMCAnCE_BLOCK_SET.");
        re_printf("warning", "No response + response busy.");
    }
    else if (((*MMCAnCE_CMD_SET)["RTYP"] == 0x0) && ((*MMCAnCE_CMD_SET)["WDAT"] == 0x1) && ((*MMCAnCE_CMD_SET)["BOOT"] == 0x0))
    {
        re_printf("warning", "Illegal setting has been made in MMCAnCE_CMD_SET or MMCAnCE_BLOCK_SET.");
        re_printf("warning", "No response + with data + not during boot operations.");
    }

    else if (((*MMCAnCE_CMD_SET)["WDAT"] == 0x0) && ((*MMCAnCE_CMD_SET)["CMD12EN"] == 0x1))
    {
        re_printf("warning", "Illegal setting has been made in MMCAnCE_CMD_SET or MMCAnCE_BLOCK_SET.");
        re_printf("warning", "No data + automatic CMD12 issuance.");
    }
    else if (((*MMCAnCE_CMD_SET)["WDAT"] == 0x1) && ((*MMCAnCE_CMD_SET)["CMLTE"] == 0x0) && ((*MMCAnCE_CMD_SET)["CMD12EN"] == 0x1))
    {
        re_printf("warning", "Illegal setting has been made in MMCAnCE_CMD_SET or MMCAnCE_BLOCK_SET.");
        re_printf("warning", "With data + single-block transfer + automatic CMD12 issuance.");
    }
    else if (((*MMCAnCE_CMD_SET)["WDAT"] == 0x1) && ((*MMCAnCE_CMD_SET)["RBSY"] == 0x1) && ((*MMCAnCE_CMD_SET)["CMD12EN"] == 0x1))
    {
        re_printf("warning", "Illegal setting has been made in MMCAnCE_CMD_SET or MMCAnCE_BLOCK_SET.");
        re_printf("warning", "With data + response busy + automatic CMD12 issuance.");
    }

    else if (((*MMCAnCE_CMD_SET)["WDAT"] == 0x1) && ((*MMCAnCE_BLOCK_SET)["BLKSIZ"] == 0x0))
    {
        re_printf("warning", "Illegal setting has been made in MMCAnCE_CMD_SET or MMCAnCE_BLOCK_SET.");
        re_printf("warning", "With data + transfer block size = 0.");
    }
    else if (((*MMCAnCE_CMD_SET)["WDAT"] == 0x1) && ((*MMCAnCE_BLOCK_SET)["BLKSIZ"] > 0x0200))
    {
        re_printf("warning", "Illegal setting has been made in MMCAnCE_CMD_SET or MMCAnCE_BLOCK_SET.");
        re_printf("warning", "With data + transfer block size > 512.");
    }
    else if (((*MMCAnCE_CMD_SET)["WDAT"] == 0x1) && ((*MMCAnCE_CMD_SET)["CMLTE"] == 0x1) && ((*MMCAnCE_BLOCK_SET)["BLKCNT"] == 0x0))
    {
        re_printf("warning", "Illegal setting has been made in MMCAnCE_CMD_SET or MMCAnCE_BLOCK_SET.");
        re_printf("warning", "With data + multi-block transfer + number of blocks for transfer = 0.");
    }
    else if (((*MMCAnCE_CMD_SET)["WDAT"] == 0x0) && ((*MMCAnCE_CMD_SET)["BOOT"] == 0x1))
    {
        re_printf("warning", "Illegal setting has been made in MMCAnCE_CMD_SET or MMCAnCE_BLOCK_SET.");
        re_printf("warning", "Boot operations + no data.");
    }
    else if (((*MMCAnCE_CMD_SET)["DWEN"] == 0x1) && ((*MMCAnCE_CMD_SET)["BOOT"] == 0x1))
    {
        re_printf("warning", "Illegal setting has been made in MMCAnCE_CMD_SET or MMCAnCE_BLOCK_SET.");
        re_printf("warning", "Boot operations + write.");
    }
    else if (((*MMCAnCE_CMD_SET)["RBSY"] == 0x1) && ((*MMCAnCE_CMD_SET)["BOOT"] == 0x1))
    {
        re_printf("warning", "Illegal setting has been made in MMCAnCE_CMD_SET or MMCAnCE_BLOCK_SET.");
        re_printf("warning", "Boot operations + response busy.");
    }
    else if (((*MMCAnCE_CMD_SET)["CMD12EN"] == 0x1) && ((*MMCAnCE_CMD_SET)["BOOT"] == 0x1))
    {
        re_printf("warning", "Illegal setting has been made in MMCAnCE_CMD_SET or MMCAnCE_BLOCK_SET.");
        re_printf("warning", "CMD12 setting + Boot operation.");
    }
    else if (((*MMCAnCE_CMD_SET)["BOOTACK"] == 0x1) && ((*MMCAnCE_CMD_SET)["BOOT"] == 0x0))
    {
        re_printf("warning", "Illegal setting has been made in MMCAnCE_CMD_SET or MMCAnCE_BLOCK_SET.");
        re_printf("warning", "Boot acknowledge reception + not during boot operations.");
    }
    else
    {
        isCommandSettingError = true;
    }

    return isCommandSettingError;

} //}}}

/**@brief
 * Send data to buffer
 */
void MMC44_Func::SendDataToBuffer(uint8_t *ptrData)
{ //{{{
    assert(ptrData != NULL);
    uint32_t dataLength = GetTransferBlockSetting(emBLKSZ);

    SetTimeDataTrans(dataLength);
    if (mBuffer1State == emSending)
    {
        re_printf("info", "SendDataToBuffer1\n");
        memcpy(&mBuffer1[0], ptrData, dataLength);
        mpAgentController->AssertBuffer1ReadEnableEvent();
    }
    else if (mBuffer2State == emSending)
    {
        re_printf("info", "SendDataToBuffer2");
        memcpy(&mBuffer2[0], ptrData, dataLength);
        mpAgentController->AssertBuffer2ReadEnableEvent();
    }
} //}}}

/**@brief
 * Set buffer state
 */
void MMC44_Func::SetBufferState(eBufferNumber number, eBufferState state)
{ //{{{
    if (number == emBuffer1)
    {
        mBuffer1State = state;
    }
    else if (number == emBuffer2)
    {
        mBuffer2State = state;
    }
} //}}}

/**@brief
 * Update read data pointer
 */
void MMC44_Func::UpdateReadDataPtr(eBufferState prevState)
{ //{{{
    if (prevState == emCanRead)
    {
        if ((mBuffer1State == emCanRead) && ((mBuffer2State) == emCanWrite))
        {
            mReadDataPtr = mBuffer1;
            SetDataFromBufferToDataRegister();
            SetBufferState(emBuffer1, emReading);
            if ((GetTransferBlockSetting(emBLKSZ) % 4) != 0)
            {
                mDataReadCount = (GetTransferBlockSetting(emBLKSZ) / 4) + 1;
            }
            else
            {
                mDataReadCount = (GetTransferBlockSetting(emBLKSZ) / 4);
            }
        }
        else if ((mBuffer2State == emCanRead) && ((mBuffer1State) == emCanWrite))
        {
            mReadDataPtr = mBuffer2;
            SetDataFromBufferToDataRegister();
            SetBufferState(emBuffer2, emReading);
            mDataReadCount = (GetTransferBlockSetting(emBLKSZ) / 4);
        }

    }
    else if (prevState == emReading)
    {
        if ((mBuffer1State == emReading) && ((mBuffer2State) == emCanRead))
        {
            mReadDataPtr = mBuffer2;
            SetBufferState(emBuffer1, emCanWrite);
            SetDataFromBufferToDataRegister();
            SetBufferState(emBuffer2, emReading);
            mDataReadCount = (GetTransferBlockSetting(emBLKSZ) / 4);
        }
        else if ((mBuffer2State == emReading) && ((mBuffer1State) == emCanRead))
        {
            mReadDataPtr = mBuffer1;
            SetBufferState(emBuffer2, emCanWrite);
            SetDataFromBufferToDataRegister();
            SetBufferState(emBuffer1, emReading);
            mDataReadCount = (GetTransferBlockSetting(emBLKSZ) / 4);

        }
        else if ((mBuffer1State == emReading) && ((mBuffer2State) != emCanRead))
        {
            SetBufferState(emBuffer1, emCanWrite);
        }
        else if ((mBuffer2State == emReading) && ((mBuffer1State) != emCanRead))
        {
            SetBufferState(emBuffer2, emCanWrite);
        }
    }
} //}}}

//---------------------------------------------------------------------------------
//                           functions to get/set register
//---------------------------------------------------------------------------------
/**@brief
 * Get DMA Transfer Setting
 */
uint32_t MMC44_Func::GetDMATransferSetting(void)
{ //{{{
    uint32_t value = (*MMCAnCE_BUF_ACC)["DMATYP"];

    return value;
} //}}}

/**@brief
 * Set MMC bus to busy
 */
void MMC44_Func::SetMMCBUSBSY(bool isBusy)
{ //{{{
    (*MMCAnCE_CLK_CTRL)["MMCBUSBSY"] = isBusy;
} //}}}

/**@brief
 * Get value of bits in INT_EN register
 */
uint32_t MMC44_Func::GetInterruptEnable(uint32_t bit_name)
{ //{{{
    uint32_t value = 0;
    switch (bit_name)
    {
    case emMCMD12DRE:
        value = (*MMCAnCE_INT_EN)["MCMD12DRE"];
        break;
    case emMCMD12RBE:
        value = (*MMCAnCE_INT_EN)["MCMD12RBE"];
        break;
    case emMCMD12CRE:
        value = (*MMCAnCE_INT_EN)["MCMD12CRE"];
        break;
    case emMDTRANE:
        value = (*MMCAnCE_INT_EN)["MDTRANE"];
        break;
    case emMBUFRE:
        value = (*MMCAnCE_INT_EN)["MBUFRE"];
        break;
    case emMBUFWEN:
        value = (*MMCAnCE_INT_EN)["MBUFWEN"];
        break;
    case emMBUFREN1:
        value = (*MMCAnCE_INT_EN)["MBUFREN"];
        break;
    case emMBUFREN2:
        value = (*MMCAnCE_INT_EN)["MBUFREN"];
        break;
    case emMRBSYE:
        value = (*MMCAnCE_INT_EN)["MRBSYE"];
        break;
    case emMCRSPE:
        value = (*MMCAnCE_INT_EN)["MCRSPE"];
        break;
    case emMCMDVIO:
        value = (*MMCAnCE_INT_EN)["MCMDVIO"];
        break;
    case emMBUFVIO:
        value = (*MMCAnCE_INT_EN)["MBUFVIO"];
        break;
    case emMWDATERR:
        value = (*MMCAnCE_INT_EN)["MWDATERR"];
        break;
    case emMRDATERR:
        value = (*MMCAnCE_INT_EN)["MRDATERR"];
        break;
    case emMRIDXERR:
        value = (*MMCAnCE_INT_EN)["MRIDXERR"];
        break;
    case emMRSPERR:
        value = (*MMCAnCE_INT_EN)["MRSPERR"];
        break;
    case emMCRCSTO:
        value = (*MMCAnCE_INT_EN)["MCRCSTO"];
        break;
    case emMWDATTO:
        value = (*MMCAnCE_INT_EN)["MWDATTO"];
        break;
    case emMRDATTO:
        value = (*MMCAnCE_INT_EN)["MRDATTO"];
        break;
    case emMRBSYTO:
        value = (*MMCAnCE_INT_EN)["MRBSYTO"];
        break;
    case emMRSPTO:
        value = (*MMCAnCE_INT_EN)["MRSPTO"];
        break;
    default:
        break;
    }
    return value;
} //}}}

/**@brief
 * Get value of bits in BOOT register
 */
uint32_t MMC44_Func::GetBootOpSetting(uint32_t bit_name)
{ //{{{
    uint32_t value = 0;
    switch (bit_name)
    {
    case emBTCLKDIV:
        value = (*MMCAnCE_BOOT)["BTCLKDIV"];
        break;
    case emSBTACKTO:
        value = (*MMCAnCE_BOOT)["SBTACKTO"];
        break;
    case emS1STBTDATTO:
        value = (*MMCAnCE_BOOT)["S1STBTDATTO"];
        break;
    case emSBTDATTO:
        value = (*MMCAnCE_BOOT)["SBTDATTO"];
        break;
    default:
        break;
    }
    return value;
} //}}}

/**@brief
 * Set value of bits in HOST_STS2 register
 */
void MMC44_Func::SetTimeOutAndErrSttBits(uint32_t bit_name, uint32_t value)
{ //{{{
    switch (bit_name)
    {
    case emCRCSTE_H:
        (*MMCAnCE_HOST_STS2)["CRCSTE"] = value;
        (*MMCAnCE_INT)["WDATERR"] = value;
        SetMMC44Status(emCMDSEQ, 0);
        re_printf("warning", "CRC Status error");
        if ((bool)GetInterruptEnable(emMWDATERR))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    case emCRC16E:
        (*MMCAnCE_HOST_STS2)["CRC16E"] = value;
        (*MMCAnCE_INT)["RDATERR"] = value;
        SetMMC44Status(emCMDSEQ, 0);
        re_printf("warning", "Read Data CRC16 Error");
        if ((bool)GetInterruptEnable(emMRDATERR))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    case emAC12CRCE:
        (*MMCAnCE_HOST_STS2)["AC12CRCE"] = value;
        (*MMCAnCE_INT)["RSPERR"] = value;
        SetMMC44Status(emCMDSEQ, 0);
        re_printf("warning", "Automatic CMD12 Response CRC7 Error");
        if ((bool)GetInterruptEnable(emMRSPERR))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    case emRSPCRC7E:
        (*MMCAnCE_HOST_STS2)["RSPCRC7E"] = value;
        (*MMCAnCE_INT)["RSPERR"] = value;
        SetMMC44Status(emCMDSEQ, 0);
        re_printf("warning", "Command Response CRC7 Error (other than automatically-issued CMD12)");
        if ((bool)GetInterruptEnable(emMRSPERR))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    case emCRCSTEBE:
        (*MMCAnCE_HOST_STS2)["CRCSTEBE"] = value;
        (*MMCAnCE_INT)["WDATERR"] = value;
        SetMMC44Status(emCMDSEQ, 0);
        re_printf("warning", "CRC Status End Bit Error");
        if ((bool)GetInterruptEnable(emMWDATERR))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    case emRDATEBE:
        (*MMCAnCE_HOST_STS2)["RDATEBE"] = value;
        (*MMCAnCE_INT)["RDATERR"] = value;
        SetMMC44Status(emCMDSEQ, 0);
        re_printf("warning", "Read Data End Bit Error");
        if ((bool)GetInterruptEnable(emMRDATERR))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    case emAC12REBE:
        (*MMCAnCE_HOST_STS2)["AC12REBE"] = value;
        (*MMCAnCE_INT)["RSPERR"] = value;
        SetMMC44Status(emCMDSEQ, 0);
        re_printf("warning", "Automatic CMD12 Response End Bit Error");
        if ((bool)GetInterruptEnable(emMRSPERR))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    case emRSPEBE:
        (*MMCAnCE_HOST_STS2)["RSPEBE"] = value;
        (*MMCAnCE_INT)["RSPERR"] = value;
        SetMMC44Status(emCMDSEQ, 0);
        re_printf("warning", "Command Response End Bit Error (other than automatically-issued CMD12)");
        if ((bool)GetInterruptEnable(emMRSPERR))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    case emAC12IDXE:
        (*MMCAnCE_HOST_STS2)["AC12IDXE"] = value;
        (*MMCAnCE_INT)["RIDXERR"] = value;
        SetMMC44Status(emCMDSEQ, 0);
        re_printf("warning", "Automatic CMD12 Response Index Error");
        if ((bool)GetInterruptEnable(emMRIDXERR))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    case emRSPIDXE:
        (*MMCAnCE_HOST_STS2)["RSPIDXE"] = value;
        (*MMCAnCE_INT)["RIDXERR"] = value;
        SetMMC44Status(emCMDSEQ, 0);
        re_printf("warning", "Command Response Index Error (other than automatically-issued CMD12)");
        if ((bool)GetInterruptEnable(emMRIDXERR))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    case emBTACKPATE:
        (*MMCAnCE_HOST_STS2)["BTACKPATE"] = value;
        (*MMCAnCE_INT)["RSPERR"] = value;
        SetMMC44Status(emCMDSEQ, 0);
        re_printf("warning", "Boot Acknowledge Pattern Error");
        if ((bool)GetInterruptEnable(emMRSPERR))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    case emBTACKEBE:
        (*MMCAnCE_HOST_STS2)["BTACKEBE"] = value;
        (*MMCAnCE_INT)["RSPERR"] = value;
        SetMMC44Status(emCMDSEQ, 0);
        re_printf("warning", "Boot Acknowledge End Bit Error");
        if ((bool)GetInterruptEnable(emMRSPERR))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    case emCRCST:
        (*MMCAnCE_HOST_STS2)["CRCST"] = value;
        break;
    case emSTRDATTO:
        (*MMCAnCE_HOST_STS2)["STRDATTO"] = value;
        (*MMCAnCE_INT)["RDATTO"] = value;
        re_printf("warning", "Read Data Timeout (valid other than in boot operations)");
        if ((bool)GetInterruptEnable(emMRDATTO))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    case emDATBSYTO:
        (*MMCAnCE_HOST_STS2)["DATBSYTO"] = value;
        (*MMCAnCE_INT)["WDATTO"] = value;
        re_printf("warning", "Data Busy Timeout");
        if ((bool)GetInterruptEnable(emMWDATTO))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    case emCRCSTTO:
        (*MMCAnCE_HOST_STS2)["CRCSTTO"] = value;
        (*MMCAnCE_INT)["CRCSTO"] = value;
        re_printf("warning", "CRC Status Timeout");
        if ((bool)GetInterruptEnable(emMCRCSTO))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    case emAC12BSYTO:
        (*MMCAnCE_HOST_STS2)["AC12BSYTO"] = value;
        (*MMCAnCE_INT)["RBSYTO"] = value;
        re_printf("warning", "Automatic CMD12 Response Busy Timeout");
        if ((bool)GetInterruptEnable(emMRBSYTO))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    case emRSPBSYTO:
        (*MMCAnCE_HOST_STS2)["RSPBSYTO"] = value;
        (*MMCAnCE_INT)["RBSYTO"] = value;
        re_printf("warning", "Response Busy Timeout");
        if ((bool)GetInterruptEnable(emMRBSYTO))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    case emAC12RSPTO:
        (*MMCAnCE_HOST_STS2)["AC12RSPTO"] = value;
        (*MMCAnCE_INT)["RSPTO"] = value;
        re_printf("warning", "Automatic CMD12 Response Timeout");
        if ((bool)GetInterruptEnable(emMRSPTO))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    case emSTRSPTO:
        (*MMCAnCE_HOST_STS2)["STRSPTO"] = value;
        (*MMCAnCE_INT)["RSPTO"] = value;
        re_printf("warning", "Response Timeout");
        if ((bool)GetInterruptEnable(emMRSPTO))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    case emBTACKTO:
        (*MMCAnCE_HOST_STS2)["BTACKTO"] = value;
        (*MMCAnCE_INT)["RSPTO"] = value;
        re_printf("warning", "Boot Acknowledge Timeout");
        if ((bool)GetInterruptEnable(emMRSPTO))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    case em1STBTDATTO:
        (*MMCAnCE_HOST_STS2)["1STBTDATTO"] = value;
        (*MMCAnCE_INT)["RDATTO"] = value;
        re_printf("warning", "1st Boot Data Timeout");
        if ((bool)GetInterruptEnable(emMRDATTO))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    case emBTDATTO:
        (*MMCAnCE_HOST_STS2)["BTDATTO"] = value;
        (*MMCAnCE_INT)["RDATTO"] = value;
        re_printf("warning", "Interval between Boot Data Timeout");
        if ((bool)GetInterruptEnable(emMRDATTO))
        {
            mpAgentController->AssertInterrupt();
        }
        break;
    default:
        break;
    }
} //}}}

/**@brief
 * Set value of bits in INT register
 */
void MMC44_Func::SetInterruptFlagRegister(uint32_t bit_name, uint32_t value)
{ //{{{
    switch (bit_name)
    {
        case emCMD12DRE:
        {
            (*MMCAnCE_INT)["CMD12DRE"] = value;
            break;
        }
        case emCMD12RBE:
        {
            (*MMCAnCE_INT)["CMD12RBE"] = value;
            break;
        }
        case emCMD12CRE:
        {
            (*MMCAnCE_INT)["CMD12CRE"] = value;
            break;
        }
        case emDTRANE:
        {
            (*MMCAnCE_INT)["DTRANE"] = value;
            break;
        }
        case emBUFRE:
        {
            (*MMCAnCE_INT)["BUFRE"] = value;
            break;
        }
        case emBUFWEN:
        {
            (*MMCAnCE_INT)["BUFWEN"] = value;
            break;
        }
        case emBUFREN1:
        {
            (*MMCAnCE_INT)["BUFREN"] = value;
            break;
        }
        case emBUFREN2:
        {
            (*MMCAnCE_INT)["BUFREN"] = value;
            break;
        }
        case emRBSYE:
        {
            (*MMCAnCE_INT)["RBSYE"] = value;
            break;
        }
        case emCRSPE:
        {
            (*MMCAnCE_INT)["CRSPE"] = value;
            break;
        }
        case emCMDVIO:
        {
            (*MMCAnCE_INT)["CMDVIO"] = value;
            break;
        }
        case emBUFVIO:
        {
            (*MMCAnCE_INT)["BUFVIO"] = value;
            break;
        }
        case emWDATERR:
        {
            (*MMCAnCE_INT)["WDATERR"] = value;
            break;
        }
        case emRDATERR:
        {
            (*MMCAnCE_INT)["RDATERR"] = value;
            break;
        }
        case emRIDXERR:
        {
            (*MMCAnCE_INT)["RIDXERR"] = value;
            break;
        }
        case emRSPERR:
        {
            (*MMCAnCE_INT)["RSPERR"] = value;
            break;
        }
        case emCRCSTO:
        {
            (*MMCAnCE_INT)["CRCSTO"] = value;
            break;
        }
        case emWDATTO:
        {
            (*MMCAnCE_INT)["WDATTO"] = value;
            break;
        }
        case emRDATTO:
        {
            (*MMCAnCE_INT)["RDATTO"] = value;
            break;
        }
        case emRBSYTO:
        {
            (*MMCAnCE_INT)["RBSYTO"] = value;
            break;
        }
        case emRSPTO:
        {
            (*MMCAnCE_INT)["RSPTO"] = value;
            break;
        }
        default:
            break;
    }

} //}}}

/**@brief
 * Get DATA register
 */
uint32_t MMC44_Func::GetDATARegister(void)
{ //{{{
    uint32_t value = (*MMCAnCE_DATA)["DATA"];
    return value;
} //}}}

/**@brief
 * Get value of bits in CLK_CTRL register
 */
uint32_t MMC44_Func::GetClockControl(uint32_t bit_name)
{ //{{{
    uint32_t value = 0;
    switch (bit_name)
    {
    case emMMCBUSBSY:
        value = (*MMCAnCE_CLK_CTRL)["MMCBUSBSY"];
        break;
    case emCLKEN:
        value = (*MMCAnCE_CLK_CTRL)["CLKEN"];
        break;
    case emCLKDIV:
        value = (*MMCAnCE_CLK_CTRL)["CLKDIV"];
        break;
    case emSRSPTO:
        value = (*MMCAnCE_CLK_CTRL)["SRSPTO"];
        break;
    case emSRBSYTO:
        value = (*MMCAnCE_CLK_CTRL)["SRBSYTO"];
        break;
    case emSRWDTO:
        value = (*MMCAnCE_CLK_CTRL)["SRWDTO"];
        break;
    default:
        break;
    }
    return value;

} //}}}

/**@brief
 * Get value of bits in HOST_STS1 register
 */
uint32_t MMC44_Func::GetMMC44Status(uint32_t bit_name)
{ //{{{
    uint32_t value = 0;
    switch (bit_name)
    {
        case emCMDSEQ:
        {
            value = (*MMCAnCE_HOST_STS1)["CMDSEQ"];
            break;
        }
        case emRCVBLK:
        {
            value = (*MMCAnCE_HOST_STS1)["RCVBLK"];
            break;
        }
        case emDATSIG:
        {
            value = (*MMCAnCE_HOST_STS1)["DATSIG"];
            break;
        }
        default:
            break;
    }
    return value;
} //}}}

/**@brief
 * Get value of bits in BLOCK_SET register
 */
uint32_t MMC44_Func::GetTransferBlockSetting(uint32_t bit_name)
{ //{{{
    uint32_t temp = 0;
    if (bit_name == emBLKSZ)
    {
        temp = (*MMCAnCE_BLOCK_SET)["BLKSIZ"];
    }
    else if (bit_name == emBLKCNT)
    {
        temp = (*MMCAnCE_BLOCK_SET)["BLKCNT"];
    }
    return temp;
} //}}}

/**@brief
 * Get value of bits in CMD_SET register
 */
uint32_t MMC44_Func::GetCommandSetting(uint32_t bit_name)
{ //{{{
    uint32_t value = 0;
    switch (bit_name)
    {
    case emBOOT:
        value = (*MMCAnCE_CMD_SET)["BOOT"];
        break;
    case emCMD:
        value = (*MMCAnCE_CMD_SET)["CMD"];
        break;
    case emRTYP:
        value = (*MMCAnCE_CMD_SET)["RTYP"];
        break;
    case emWDAT:
        value = (*MMCAnCE_CMD_SET)["WDAT"];
        break;
    case emDWEN:
        value = (*MMCAnCE_CMD_SET)["DWEN"];
        break;
    case emCMLTE:
        value = (*MMCAnCE_CMD_SET)["CMLTE"];
        break;
    case emCMD12EN:
        value = (*MMCAnCE_CMD_SET)["CMD12EN"];
        break;
    case emRIDXC:
        value = (*MMCAnCE_CMD_SET)["RIDXC"];
        break;
    case emRCRC7C:
        value = (*MMCAnCE_CMD_SET)["RCRC7C"];
        break;
    case emCRC16C:
        value = (*MMCAnCE_CMD_SET)["CRC16C"];
        break;
    case emBOOTACK:
        value = (*MMCAnCE_CMD_SET)["BOOTACK"];
        break;
    case emCRCSTE:
        value = (*MMCAnCE_CMD_SET)["CRCSTE"];
        break;
    case emTBIT:
        value = (*MMCAnCE_CMD_SET)["TBIT"];
        break;
    case emDATW:
        value = (*MMCAnCE_CMD_SET)["DATW"];
        break;
    default:
        break;
    }
    return value;
} //}}}

/**@brief
 * Set DATA register
 */
void MMC44_Func::SetDataRegister(uint32_t value)
{ //{{{
    (*MMCAnCE_DATA)["DATA"] = value;
} //}}}

/**@brief
 * Assert Reset
 */
void MMC44_Func::AssertReset(bool isReset)
{ //{{{
    uint32_t tmp = (uint32_t)*MMCAnCE_SWRESA;
    Cmmc44_regif::EnableReset(isReset);
    if (mIsSoftwareResetEnable)
    {
        Cmmc44_regif::EnableReset(!isReset);
        *MMCAnCE_SWRESA = tmp;
    }
    if (isReset)
    {
        Initialize();
    }
} //}}}

/**@brief
 * Set value of bits in HOST_STS1 register
 */
void MMC44_Func::SetMMC44Status(uint32_t bit_name, uint32_t value)
{ //{{{
    switch (bit_name)
    {
        case emCMDSEQ:
        {
            (*MMCAnCE_HOST_STS1)["CMDSEQ"] = value;
            break;
        }
        case emCMDSIG:
        {
            (*MMCAnCE_HOST_STS1)["CMDSIG"] = value;
            break;
        }
        case emRSPIDX:
        {
            (*MMCAnCE_HOST_STS1)["RSPIDX"] = value;
            break;
        }
        case emDATSIG:
        {
            (*MMCAnCE_HOST_STS1)["DATSIG"] = value;
            break;
        }
        case emRCVBLK:
        {
            (*MMCAnCE_HOST_STS1)["RCVBLK"] = value;
            break;
        }
        default:
            break;
    }
} //}}}

/**@brief
 * Get HOST_STS1 register
 */
uint32_t MMC44_Func::GetMMCAnCE_HOST_STS1(void)
{ //{{{
    return uint32_t(*MMCAnCE_HOST_STS1);
} //}}}

/**@brief
 * Get HOST_STS2 register
 */
uint32_t MMC44_Func::GetMMCAnCE_HOST_STS2(void)
{ //{{{
    return uint32_t(*MMCAnCE_HOST_STS2);
} //}}}

/**@brief
 * Compute CRC16
 */
uint16_t MMC44_Func::ComputeCRC16(uint8_t *dataToCalculateCRC16, uint32_t length)
{ //{{{
    assert(dataToCalculateCRC16 != NULL);
    uint16_t out = emInitCRC16Val;
    uint8_t *ptr = dataToCalculateCRC16;
    /// Mask top bit of remainder
    uint16_t maskTopBit = (1 << 15);
    for (uint32_t byte = 0; byte < length; ++byte)
    {
        /// Bring the next byte into the remainder.
        assert(ptr != NULL);
        out ^= ((uint32_t)*ptr << (15 - 8));

        for (uint8_t bit = 8; bit > 0; --bit)
        {
            if ((bool)(out & maskTopBit))
            {
                out <<= 1;
                out ^= emCRC16Polynomial;
            }
            else
            {
                out <<= 1;
            }
        }
        ptr++;
    }

    return (out);
} //}}}

/**@brief
 * Check response frame
 */
void MMC44_Func::CheckResponseFrame(uint8_t *response_ptr)
{ //{{{
    assert(response_ptr != NULL);
    // get response type
    uint8_t DataToCalculateCRC7[15];
    memset(DataToCalculateCRC7, 0x0, 15);
    uint32_t response = 0;

    uint32_t response0 = 0;
    uint32_t response1 = 0;
    uint32_t response2 = 0;
    uint32_t response3 = 0;
    uint8_t lastByte = 0;
    uint8_t firstByte = 0;
    uint8_t crc7 = 0;
    bool IsCrcErr = false;

    switch (GetCommandSetting(emRTYP))
    {
    case 1:
        /* 6 bytes response */
        firstByte = *(response_ptr);
        lastByte = *(response_ptr + 5);
        CheckFirstByteResponse(firstByte);

        // store response
        memcpy(&response, (response_ptr + 1), 4);
        if (!IsCMD12())
        {
            (*MMCAnCE_RESP0)["RSP"] = response;
        }
        else
        {
            (*MMCAnCE_RESP_CMD12)["RSP12"] = response;
        }
        if (((lastByte >> 7) & 0x1) != 0x1)
        {
            if (!IsCMD12())
            {
                mpAgentController->AssertSetTimeoutAndErrStt(emRSPEBE);
            }
            else
            {
                mpAgentController->AssertSetTimeoutAndErrStt(emAC12REBE);
            }
        }

        if (GetCommandSetting(emRCRC7C) == 0)
        {
            memcpy(&DataToCalculateCRC7[0], &firstByte, 1);
            memcpy(&DataToCalculateCRC7[1], &response, 4);
            crc7 = ComputeCRC7(DataToCalculateCRC7, 5);
            if (crc7 != (lastByte & 0x7F))
            {
                IsCrcErr = true;
            }
        }
        else if (GetCommandSetting(emRCRC7C) == 1)
        {
            crc7 = 0xFF;
            if (crc7 != (lastByte & 0x7F))
            {
                IsCrcErr = true;
            }
        }
        break;
    case 2:
        /*17 bytes response */
        firstByte = *(response_ptr);
        lastByte = *(response_ptr + 16);
        CheckFirstByteResponse(firstByte);

        // store response
        memcpy(&response0, (response_ptr + 1), 4);
        memcpy(&response1, (response_ptr + 5), 4);
        memcpy(&response2, (response_ptr + 9), 4);
        memcpy(&response3, (response_ptr + 13), 4);
        (*MMCAnCE_RESP0)["RSP"] = response0;
        (*MMCAnCE_RESP1)["RSP"] = response1;
        (*MMCAnCE_RESP2)["RSP"] = response2;
        (*MMCAnCE_RESP3)["RSP"] = response3;

        if (((lastByte >> 7) & 0x1) != 0x1)
        {
            mpAgentController->AssertSetTimeoutAndErrStt(emRSPEBE);
        }

        if (GetCommandSetting(emRCRC7C) == 2)
        {
            memcpy(&DataToCalculateCRC7[0], &response0, 4);
            memcpy(&DataToCalculateCRC7[4], &response1, 4);
            memcpy(&DataToCalculateCRC7[8], &response2, 4);
            memcpy(&DataToCalculateCRC7[12], &response3, 3);

            crc7 = ComputeCRC7(DataToCalculateCRC7, 15);
            if (crc7 != (lastByte & 0x7F))
            {
                IsCrcErr = true;
            }
        }
        break;
    default:
        break;
    }

    if (IsCrcErr)
    {
        if (!IsCMD12())
        {
            mpAgentController->AssertSetTimeoutAndErrStt(emRSPCRC7E);
        }
        else
        {
            mpAgentController->AssertSetTimeoutAndErrStt(emAC12CRCE);
        }
    }
} //}}}

/**@brief
 * Initialize data frame
 */
uint8_t *MMC44_Func::IniDataFrame(eBufferNumber bufferNum)
{ //{{{
    uint32_t blockSize = GetTransferBlockSetting(emBLKSZ);
    uint8_t *dataFramePtr = mArrayData;
    // set Start bit
    assert(dataFramePtr != NULL);
    *(dataFramePtr) = 0x0;
    // set data
    if (bufferNum == emBuffer1)
    {
        memcpy(&mArrayData[emStartBitLength], &mBuffer1[0], blockSize);
    }
    else if (bufferNum == emBuffer2)
    {
        memcpy(&mArrayData[emStartBitLength], &mBuffer2[0], blockSize);
    }
    // set crc16
    uint16_t crc16 = ComputeCRC16((dataFramePtr + emStartBitLength), blockSize);
    memcpy(&mArrayData[emStartBitLength + blockSize], &crc16, emCRC16Length);
    // end bit
    mArrayData[emStartBitLength + blockSize + emCRC16Length] = 0x1;
    return (uint8_t *)dataFramePtr;
} //}}}

/**@brief
 * Check first byte of response frame
 */
void MMC44_Func::CheckFirstByteResponse(uint8_t firstByte)
{ //{{{
    uint8_t index = (firstByte >> 2) & 0x1F;
    bool isIndexErr = false;

    // set index
    SetMMC44Status(emRSPIDX, index);

    if ((firstByte & 0x1) != 0)
    {
        re_printf("warning", "Start bit in response is not 0.");
    }
    if (((firstByte >> 1) & 0x1) != 0)
    {
        // error transmission bit
        (*MMCAnCE_INT)["RSPERR"] = 0x1;
        SetMMC44Status(emCMDSEQ, 0);
        if ((bool)GetInterruptEnable(emMRSPERR))
        {
            re_printf("warning", "Error in transmission bit.");
            mpAgentController->AssertInterrupt();
        }
    }

    // check RIDXC: Command Response Index Error (other than automatically-issued CMD12)
    if (GetCommandSetting(emRIDXC) == 0x0)
    {
        if (!IsCMD12())
        {
            if (index != GetCommandSetting(emCMD))
            {
                isIndexErr = true;
            }
        }
        else
        {
            if (index != 0xC)
            {
                isIndexErr = true;
            }
        }
    }
    else if (GetCommandSetting(emRIDXC) == 0x1)
    {
        if (index != 0x1F)
        {
            isIndexErr = true;
        }
    }


    if (isIndexErr)
    {
        if (!IsCMD12())
        {
            mpAgentController->AssertSetTimeoutAndErrStt(emRSPIDXE);
        }
        else
        {
            mpAgentController->AssertSetTimeoutAndErrStt(emAC12IDXE);
        }
    }
} //}}}

/**@brief
 * Initialize Command frame
 */
uint8_t *MMC44_Func::IniCMDFrame(bool isCMD12)
{ //{{{
    uint8_t *Cmd_ptr = mArrayCommand;
    assert(Cmd_ptr != NULL);
    if ((bool)GetCommandSetting(emBOOT))
    {
        memset(Cmd_ptr, 0x0, emCommandSize);
    }
    else
    {
        //Join data to calculate CRC7
        uint8_t DataToCalculateCRC7[5];
        memset(DataToCalculateCRC7, 0x0, 5);
        uint32_t argSetBit = 0;
        uint8_t firstByte = 0;
        if (!isCMD12)
        {
            // get argument
            argSetBit = ((*MMCAnCE_ARG)["ARG"]);
            firstByte = (0x0 << 0) | ((!(GetCommandSetting(emTBIT) & 0x1)) << 1) | ((GetCommandSetting(emCMD) & 0x1F) << 2);
        }
        else
        {
            // argument for CMD12
            argSetBit = ((*MMCAnCE_ARG_CMD12)["C12ARG"]);
            firstByte = (0x0 << 0) | ((!(GetCommandSetting(emTBIT) & 0x1)) << 1) | ((0xC & 0x1F) << 2);
        }
        memcpy(&DataToCalculateCRC7, &firstByte, 1);
        memcpy(&DataToCalculateCRC7[1], &argSetBit, 4);
        uint8_t crc7 = ComputeCRC7(DataToCalculateCRC7, 5);
        uint8_t lastByte = (crc7 | (0x1 << 7));

        // set cmd
        memset(Cmd_ptr, 0x0, emCommandSize);
        memcpy(Cmd_ptr, &firstByte, 1);
        memcpy((Cmd_ptr + 1), &argSetBit, 4);
        memcpy((Cmd_ptr + 5), &lastByte, 1);
    }
    return (uint8_t *)mArrayCommand;
} //}}}

/**@brief
 * Compute CRC7
 */
uint8_t MMC44_Func::ComputeCRC7(uint8_t *dataToCalculateCRC7, uint32_t length)
{ //{{{
    int i, temp, crc;
    crc = emInitCRC7Val;
    while (length-- > 0)
    {
        temp = *dataToCalculateCRC7++;
        for (i = 0; i < 8; i++)
        {
            crc <<= 1;
            if ((bool)((temp ^ crc) & 0x80))
                crc ^= emCRC7Polynomial;
            temp <<= 1;
        }
    }
    return crc & 0x7f;
} //}}}

/**@brief
 * Register handler
 */
void MMC44_Func::RegisterHandler(const std::vector<std::string> &args)
{ //{{{
    std::string msg_return = "";
    if (args.size() == 1)
    {
        //Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL)
        {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    }
    else
    {
        if (args[1] == "*")
        {
            //Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL)
            {
                cmd = args;
                cmd.erase(cmd.begin() + 1);
                cmd.insert(cmd.begin() + 1, reg_p->name());
                msg_return = Cmmc44_regif::reg_handle_command(cmd);
                if (msg_return != "")
                {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        }
        else
        { //Set handle command to Cmmc44_regif class
            msg_return = Cmmc44_regif::reg_handle_command(args);
            if (msg_return != "")
            {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
} //}}}


//-----------------------------------------------------------------------------------------------------------
// implement pure virtual call-back function, the means allowing base class
// register block Cmmc44_regif inform the derived class
//-----------------------------------------------------------------------------------------------------------
void MMC44_Func::cb_MMCAnCE_DATA_DATA(RegCBstr str)
{   //{{{
    // WRITE data to buffer from CPU
    bool isBufferAccessErr = false;

    if ((bool)GetMMC44Status(emCMDSEQ))
    {
        if (str.is_wr && (!mIsDMAAccess))
        {
            if ((mBuffer1State == emWritting) || (mBuffer1State == emCanWrite) || ((mBuffer2State == emWritting) && GetCommandSetting(emCMLTE)) || ((mBuffer2State == emCanWrite) && GetCommandSetting(emCMLTE)))
            {
                // write data to buffer
                if ((*MMCAnCE_INT)["BUFWEN"] == 0x1)
                {
                    re_printf("warning", "When writing data to MMCAnCE_DATA by the CPU, BUFWEN bit should be cleared first .");
                }
                mpAgentController->AssertWriteDataToBufferProcess();
            }
            else
            {
                isBufferAccessErr = true;
            }
        } // WRITE data to buffer from DMA
        else if (str.is_wr && mIsDMAAccess && IsDMAWriteEnable() && (mDMAValueSaved == 0))
        {
            if ((mBuffer1State == emWritting) || (mBuffer1State == emCanWrite) || ((mBuffer2State == emWritting) && GetCommandSetting(emCMLTE)) || ((mBuffer2State == emCanWrite) && GetCommandSetting(emCMLTE)))
            {
                // write data to buffer in case DMA access
                mpAgentController->AssertWriteDataToBufferProcess();
            }
            else
            {
                isBufferAccessErr = true;
            }

        } //READ data from buffer by CPU
        else if ((!str.is_wr) && (!mIsDMAAccess))
        {
            if (mBuffer1State == emReading || mBuffer1State == emCanRead || ((mBuffer2State == emReading) && GetCommandSetting(emCMLTE)) || ((mBuffer2State == emCanRead) && GetCommandSetting(emCMLTE)))
            {
                // read data from buffer
                if ((*MMCAnCE_INT)["BUFREN"] == 0x1)
                {
                    re_printf("warning", "When reading data from MMCAnCE_DATA by the CPU, BUFREN bit should be cleared first .");
                }
                mpAgentController->AssertReadDataFromBufferProcess();
            }
            else
            {
                isBufferAccessErr = true;
            }
        } // READ data from buffer by DMA
        else if (!str.is_wr && mIsDMAAccess && IsDMAReadEnable() && (mDMAValueSaved == 0))
        {
            if (mBuffer1State == emReading || mBuffer1State == emCanRead || ((mBuffer2State == emReading) && GetCommandSetting(emCMLTE)) || ((mBuffer2State == emCanRead) && GetCommandSetting(emCMLTE)))
            {
                // read data from buffer
                mpAgentController->AssertReadDataFromBufferProcess();
            }
            else
            {
                isBufferAccessErr = true;
            }
        }
        else
        {
            isBufferAccessErr = true;
        }

        if (isBufferAccessErr)
        {
            (*MMCAnCE_INT)["BUFVIO"] = 0x1;
            re_printf("warning", "Buffer access error.");
            if ((bool)GetInterruptEnable(emMBUFVIO))
            {
                mpAgentController->AssertInterrupt();
            }
            if ((bool)GetDMATransferSetting())
            {
                mpAgentController->ErrorOccuredDuringDMATransfer();
            }
        }
    }
    else
    {
        re_printf("warning", "Command sequence is not in process, can not use DATA register to access to buffer.");
    }
} //}}}

void MMC44_Func::cb_MMCAnCE_CMD_SET_BOOT(RegCBstr str)
{ //{{{
    //  Command sequence is being executed.
    if ((*MMCAnCE_HOST_STS1)["CMDSEQ"] == 0x1)
    {
        //Set CMDVIO bit to 1 & raise interrupt
        (*MMCAnCE_INT)["CMDVIO"] = 0x1;
        if ((bool)GetInterruptEnable(emMCMDVIO))
        {
            mpAgentController->AssertInterrupt();
        }
        re_printf("warning", "Writing to MMCAnCE_CMD_SET is disabled while a command sequence is proceeding");
    }
    else
    {
        //Check setting in this register
        if (PreCheckCommandSettingError())
        {
            // Clear prev command sequence status
            (*MMCAnCE_HOST_STS1)["RCVBLK"] = 0x0;

            // boot mode enable
            if (((*MMCAnCE_CMD_SET)["RIDXC"] == 0x3))
            {
                re_printf("warning", "RIDXC set to b'11 is prohibited. Keep old value setting.");
                uint32_t preRIDXC = (str.pre_data >> 14) & 0x3;
                (*MMCAnCE_CMD_SET)["RIDXC"] = preRIDXC;
            }
            if (((*MMCAnCE_CMD_SET)["RTYP"] == 0x3))
            {
                re_printf("warning", "RTYP set to b'11 is prohibited. Keep old value setting.");
                uint32_t preRTYP = (str.pre_data >> 22) & 0x3;
                (*MMCAnCE_CMD_SET)["RTYP"] = preRTYP;
            }
            if (((*MMCAnCE_CMD_SET)["DATW"] == 0x3))
            {
                re_printf("warning", "DATW set to b'11 is prohibited. Keep old value setting.");
                uint32_t preDATW = (str.pre_data >> 0) & 0x3;
                (*MMCAnCE_CMD_SET)["DATW"] = preDATW;
            }
            if (((*MMCAnCE_CMD_SET)["OPDM"] == 0x1))
            {
                re_printf("warning", "Open-Drain Output Mode is not support in this model because using port pointer.");
            }
            if (((*MMCAnCE_CMD_SET)["SBIT"] == 0x0))
            {
                re_printf("warning", "Read Data Start Bit Detection Setting is just detected by one line because using port pointer");
            }

            if (mpAgentController->CheckClockPeriod("CLK_HSB") && (GetCommandSetting(emBOOT)))
            {
                SetMMC44Status(emCMDSEQ, 1);
                SetMMCBUSBSY(true);
                mpAgentController->AssertSetBootCLKEvent();
                mpAgentController->AssertStartSendCommandEvent();
            }
            else
            {
                SetMMC44Status(emCMDSEQ, 1);
                SetMMCBUSBSY(true);
                mpAgentController->AssertStartSendCommandEvent();
                if (((bool)GetCommandSetting(emDWEN)) && ((bool)GetCommandSetting(emWDAT)))
                {
                    if (!IsDMAWriteEnable())
                    {
                        mpAgentController->AssertIntBUFWENSetEvent();
                    }
                    else
                    {
                        mpAgentController->AssertDMAEvent();
                    }
                }
            }
        }
        else
        {
            (*MMCAnCE_INT)["CMDVIO"] = 0x1;
            if ((bool)GetInterruptEnable(emMCMDVIO))
            {
                mpAgentController->AssertInterrupt();
            }
            re_printf("warning", "Illegal setting has been made in MMCAnCE_CMD_SET or MMCAnCE_BLOCK_SET.");
        }
    }
} //}}}

void MMC44_Func::cb_MMCAnCE_CMD_SET_DATW(RegCBstr str)
{ //{{{
    if (str.is_wr)
    {
        if (GetCommandSetting(emDATW) == 0)
        {
            mBusWidthLength = 1;
        }
        else if (GetCommandSetting(emDATW) == 1)
        {
            mBusWidthLength = 4;
        }
        else if (GetCommandSetting(emDATW) == 2)
        {
            mBusWidthLength = 8;
        }
    }
} //}}}

void MMC44_Func::cb_MMCAnCE_ARG_ARG(RegCBstr str)
{ //{{{
    if (str.is_wr)
    {
        if ((*MMCAnCE_HOST_STS1)["CMDSEQ"] == 0x1)
        {
            re_printf("warning", "Set MMCAnCE_ARG register before starting a command sequence.");
            (*MMCAnCE_ARG) = str.pre_data;
        }
    }
} //}}}

void MMC44_Func::cb_MMCAnCE_ARG_CMD12_C12ARG(RegCBstr str)
{ //{{{
    if (str.is_wr)
    {
        if ((*MMCAnCE_HOST_STS1)["CMDSEQ"] == 0x1)
        {
            re_printf("warning", "Set MMCAnCE_ARG_CMD12 register before starting a command sequence.");
            (*MMCAnCE_ARG_CMD12) = str.pre_data;
        }
    }
} //}}}

void MMC44_Func::cb_MMCAnCE_CMD_CTRL_BREAK(RegCBstr str)
{ //{{{
    if (str.is_wr)
    {
        if ((str.pre_data == 1) && (str.data == 0))
        {
            SetMMC44Status(emCMDSEQ, 0);
        }
    }
} //}}}

void MMC44_Func::cb_MMCAnCE_CLK_CTRL_CLKEN(RegCBstr str)
{ //{{{
    if (str.is_wr)
    {

        if ((*MMCAnCE_HOST_STS1)["CMDSEQ"] == 0x1)
        {
            re_printf("warning", "Set MMCAnCE_CLK_CTRL register before starting a command sequence.");
            (*MMCAnCE_CLK_CTRL) = str.pre_data;
        }
        else if (GetClockControl(emMMCBUSBSY) == 0x0)
        {
            mpAgentController->AssertSetCLKEvent();
        }
        else
        {
            re_printf("warning", " When MMC bus is busy, do not set CLKEN bit and the CLKDIV bit of the MMCAnCE_CLK_CTRL register.");
            (*MMCAnCE_CLK_CTRL) = str.pre_data;
        }
    }
} //}}}

void MMC44_Func::cb_MMCAnCE_BUF_ACC_DMATYP(RegCBstr str)
{ //{{{
    if (str.is_wr)
    {
        if ((*MMCAnCE_HOST_STS1)["CMDSEQ"] == 0x0)
        {
            if ((bool)((str.data << emDMAREN) & 0x1) && (bool)((str.data << emDMATYP) & 0x1))
            { //Enables DMA transfer request for buffer reading.
                re_printf("info", "Enables DMA transfer request for buffer reading.");
            }
            if ((bool)((str.data << emDMAWEN) & 0x1) && (bool)((str.data << emDMATYP) & 0x1))
            { //Enables DMA transfer request for buffer writing.
                re_printf("info", "Enables DMA transfer request for buffer writing.");
            }
        }
        else
        {
            re_printf("warning", "Writing to MMCAnCE_BUF_ACC is disabled while a command sequence is proceeding");
            (*MMCAnCE_BUF_ACC) = str.pre_data;
        }
    }
} //}}}

void MMC44_Func::cb_MMCAnCE_BOOT_BTCLKDIV(RegCBstr str)
{ //{{{
    if (str.is_wr)
    {
        if ((*MMCAnCE_HOST_STS1)["CMDSEQ"] == 0x1)
        {
            re_printf("warning", "Set MMCAnCE_BOOT register before starting a command sequence.");
            (*MMCAnCE_BOOT) = str.pre_data;
        }
    }
} //}}}

void MMC44_Func::cb_MMCAnCE_INT_CMD12DRE(RegCBstr str)
{ //{{{
    if (str.is_wr)
    {
        if (((bool)(*MMCAnCE_INT)["CMD12DRE"]))
        {
            re_printf("warning", "Can't write 1 to CMD12DRE.");
            SetInterruptFlagRegister(emCMD12DRE, (str.pre_data >> 26) & 0x1);
        }
        if (((bool)(*MMCAnCE_INT)["CMD12RBE"]))
        {
            re_printf("warning", "Can't write 1 to CMD12RBE.");
            SetInterruptFlagRegister(emCMD12RBE, (str.pre_data >> 25) & 0x1);
        }
        if (((bool)(*MMCAnCE_INT)["CMD12CRE"]))
        {
            re_printf("warning", "Can't write 1 to CMD12CRE.");
            SetInterruptFlagRegister(emCMD12CRE, (str.pre_data >> 24) & 0x1);
        }
        if (((bool)(*MMCAnCE_INT)["DTRANE"]))
        {
            re_printf("warning", "Can't write 1 to DTRANE.");
            SetInterruptFlagRegister(emDTRANE, (str.pre_data >> 23) & 0x1);
        }
        if (((bool)(*MMCAnCE_INT)["BUFRE"]))
        {
            re_printf("warning", "Can't write 1 to BUFRE.");
            SetInterruptFlagRegister(emBUFRE, (str.pre_data >> 22) & 0x1);
        }
        if (((bool)(*MMCAnCE_INT)["BUFWEN"]))
        {
            re_printf("warning", "Can't write 1 to BUFWEN.");
            SetInterruptFlagRegister(emBUFWEN, (str.pre_data >> 21) & 0x1);
        }
        if (((bool)(*MMCAnCE_INT)["BUFREN"]))
        {
            re_printf("warning", "Can't write 1 to BUFREN.");
            SetInterruptFlagRegister(emBUFREN1, (str.pre_data >> 20) & 0x1);
        }
        if (((bool)(*MMCAnCE_INT)["RBSYE"]))
        {
            re_printf("warning", "Can't write 1 to RBSYE.");
            SetInterruptFlagRegister(emRBSYE, (str.pre_data >> 17) & 0x1);
        }
        if (((bool)(*MMCAnCE_INT)["CRSPE"]))
        {
            re_printf("warning", "Can't write 1 to CRSPE.");
            SetInterruptFlagRegister(emCRSPE, (str.pre_data >> 16) & 0x1);
        }
        if (((bool)(*MMCAnCE_INT)["CMDVIO"]))
        {
            re_printf("warning", "Can't write 1 to CMDVIO.");
            SetInterruptFlagRegister(emCMDVIO, (str.pre_data >> 15) & 0x1);
        }
        if (((bool)(*MMCAnCE_INT)["BUFVIO"]))
        {
            re_printf("warning", "Can't write 1 to BUFVIO.");
            SetInterruptFlagRegister(emBUFVIO, (str.pre_data >> 14) & 0x1);
        }
        if (((bool)(*MMCAnCE_INT)["WDATERR"]))
        {
            re_printf("warning", "Can't write 1 to WDATERR.");
            SetInterruptFlagRegister(emWDATERR, (str.pre_data >> 11) & 0x1);
        }
        if (((bool)(*MMCAnCE_INT)["RDATERR"]))
        {
            re_printf("warning", "Can't write 1 to RDATERR.");
            SetInterruptFlagRegister(emRDATERR, (str.pre_data >> 10) & 0x1);
        }
        if (((bool)(*MMCAnCE_INT)["RIDXERR"]))
        {
            re_printf("warning", "Can't write 1 to RIDXERR.");
            SetInterruptFlagRegister(emRIDXERR, (str.pre_data >> 9) & 0x1);
        }
        if (((bool)(*MMCAnCE_INT)["RSPERR"]))
        {
            re_printf("warning", "Can't write 1 to RSPERR.");
            SetInterruptFlagRegister(emRSPERR, (str.pre_data >> 8) & 0x1);
        }
        if (((bool)(*MMCAnCE_INT)["CRCSTO"]))
        {
            re_printf("warning", "Can't write 1 to CRCSTO.");
            SetInterruptFlagRegister(emCRCSTO, (str.pre_data >> 4) & 0x1);
        }
        if (((bool)(*MMCAnCE_INT)["WDATTO"]))
        {
            re_printf("warning", "Can't write 1 to WDATTO.");
            SetInterruptFlagRegister(emWDATTO, (str.pre_data >> 3) & 0x1);
        }
        if (((bool)(*MMCAnCE_INT)["RDATTO"]))
        {
            re_printf("warning", "Can't write 1 to RDATTO.");
            SetInterruptFlagRegister(emRDATTO, (str.pre_data >> 2) & 0x1);
        }
        if (((bool)(*MMCAnCE_INT)["RBSYTO"]))
        {
            re_printf("warning", "Can't write 1 to RBSYTO.");
            SetInterruptFlagRegister(emRBSYTO, (str.pre_data >> 1) & 0x1);
        }
        if (((bool)(*MMCAnCE_INT)["RSPTO"]))
        {
            re_printf("warning", "Can't write 1 to RSPTO.");
            SetInterruptFlagRegister(emRSPTO, (str.pre_data >> 0) & 0x1);
        }
        uint32_t INT_Reg_Val = (uint32_t)(*MMCAnCE_INT);
        uint32_t INT_En_Reg_Val = (uint32_t)(*MMCAnCE_INT_EN);
        
        if((INT_Reg_Val & INT_En_Reg_Val) == 0 ){
            mpAgentController->DeAssertInterrupt();
        }
    }
} //}}}

void MMC44_Func::cb_MMCAnCE_INT_EN_MBUFWEN(RegCBstr str)
{ //{{{
    if (str.is_wr)
    {
        uint32_t INT_Reg_Val = (uint32_t)(*MMCAnCE_INT);
        uint32_t INT_En_Reg_Val = (uint32_t)(*MMCAnCE_INT_EN);
        if((INT_Reg_Val & INT_En_Reg_Val) != 0 ){
            mpAgentController->AssertInterrupt();
        }else{
            mpAgentController->DeAssertInterrupt();
        }
    }
} //}}}

void MMC44_Func::cb_MMCAnCE_SWRESA_SWRST(RegCBstr str)
{ //{{{
    if ((((str.data >> 31) & 0x1) == 0x1) && (((str.pre_data >> 31) & 0x1) == 0x0))
    { //Software reset is enable
        re_printf("info", "Software reset is enable.");
        mIsSoftwareResetEnable = true;
        mpAgentController->EnableSoftwareReset(true);
    }
    else if ((((str.data >> 31) & 0x1) == 0x0) && (((str.pre_data >> 31) & 0x1) == 0x1))
    { //Software reset is disable
        re_printf("info", "Software reset is disable.");
        mIsSoftwareResetEnable = false;
        mpAgentController->EnableSoftwareReset(false);
    }
} //}}}
