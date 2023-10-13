// ---------------------------------------------------------------------
// $Id: SFMA.cpp,v 1.8 2020/02/10 09:39:57 huepham Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "SFMA_Func.h"
#include "SFMA.h"
#include "SFMA_ExternalRead.h"
#include "SFMA_ReadCache.h"

/// Constructor and Destructor
SFMA::SFMA(sc_module_name name, const unsigned int rLatency, const unsigned int wLatency)
        :sc_module(name)
        ,SFMA_AgentController()
        ,BusSlaveBase<64,2>()
        ,rvc_common_model()
        ,clkb("clkb")
        ,RES_SFMA("RES_SFMA")
{//{{{ 
    // Configure Slave socket
    setSlaveResetPort64(&RES_SFMA, &RES_SFMA);
    setSlaveFreqPort64(&clkb, &clkb);
    setTargetSocket64((char*)"tsr", (char*)"tsm");

    tsr = BusSlaveBase<64, 2>::tSocket64[0];
    tsm = BusSlaveBase<64, 2>::tSocket64[1];


    mpSFMAFunc = new SFMA_Func((std::string)name, this);
    sc_assert(mpSFMAFunc != NULL);
    mBusSlaveIf64[0]->setFuncModulePtr(mpSFMAFunc);
    mBusSlaveIf64[0]->setBusProtocol(BUS_AHB);
    mBusSlaveIf64[0]->setReadInitLatency(rLatency);
    mBusSlaveIf64[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf64[0]->setTransNmbLmt(0x1);

    mpSFMAExternalRead = new SFMA_ExternalRead((std::string)name, this);
    sc_assert(mpSFMAExternalRead != NULL);
    mBusSlaveIf64[1]->setFuncModulePtr(mpSFMAExternalRead);
    mBusSlaveIf64[1]->setBusProtocol(BUS_AHB);
    mBusSlaveIf64[1]->setReadInitLatency(rLatency);
    mBusSlaveIf64[1]->setWriteInitLatency(wLatency);
    mBusSlaveIf64[1]->setTransNmbLmt(0x1);

    // Read cache
    mpSFMAReadCache = new SFMA_ReadCache();

    // Extension
    mSfmaExtension = new SFMA_TlmExtension();

    // Initialize variables
    mclkbPeriod = 0;
    mclkbOrgFreq = 0;
    mclkbFreq = 0;
    mclkbFreqUnit = "Hz";
    mSPBCLKPeriod = 0;
    mResetCurVal = true;
    mResetPreVal = true;
    mIsResetHardActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive = false;
    mResetCmdPeriod = 0;
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    mTimeCLKChangePoint = 0;
    mEnableRegisterMessage = true;
    memset(mDataArray, 0, sizeof(mDataArray));

    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    Initialize();

    // SC_THREAD/METHOD
    SC_METHOD(HandleclkbSignalMethod);
    dont_initialize();
    sensitive << clkb;

    SC_METHOD(HandleRES_SFMASignalMethod);
    sensitive << RES_SFMA;

    SC_METHOD(HandleResetHardMethod);
    dont_initialize();
    sensitive << mResetHardEvent;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;

    SC_METHOD(TransferEndMethod);
    dont_initialize();
    sensitive << mTransferEndEvent;
}//}}}

/// Destructor
SFMA::~SFMA()
{//{{{
    delete mpSFMAFunc;
    delete mpSFMAExternalRead;
    delete mpSFMAReadCache;
    delete mSfmaExtension;
}//}}}

/// Initialize when reset activated
void SFMA::Initialize(void)
{//{{{
    memset(mDataArray, 0, sizeof(mDataArray));
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
/// Check clock period 
bool SFMA::CheckClockPeriod (void)
{//{{{ 
    bool clock_status = false;
    if (mclkbFreq > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","clkb frequency is zero\n");
    }
    return clock_status;
}//}}} 

/// Get Reset Status
bool SFMA::GetResetStatus (void)
{//{{{
    return (mIsResetCmdActive || mIsResetHardActive);
}//}}}

unsigned int SFMA::GetBitField(const std::string regName, const std::string bitName)
{
    return mpSFMAFunc->GetBitField(regName, bitName);
}

/// Transfer end
void SFMA::TransferEnd(void)
{//{{{
    double t3 = 0;

    // Calculate SPI clock
    CalculateSPBCLKPeriod();
    // Delay t3
    t3 = (double)mSPBCLKPeriod * (double)(GetBitField("SFMASSLDR", "SPNDL") + 1);

    // Notify to set TEND = 1 after t3 delay
    mTransferEndEvent.notify(t3, mTimeResolutionUnit);
}//}}}

/// SPI Mode setting checking
bool SFMA::IsSPIModeSettingProhibited(void)
{//{{{
    bool isProhibited = false;

    // Checking prohibited cases at first
    if (emExtReadMode == GetBitField("SFMACMNCR", "MD")) {
        isProhibited = true;
        re_printf("warning","[SPI Mode] Operation mode is not correct: External address space read mode\n");

    } else if ((0x1 == GetBitField("SFMACMNCR", "CPHAT"))
            && (0x0 == GetBitField("SFMACMNCR", "CPHAR"))) {
        isProhibited = true;
        re_printf("warning","[SPI Mode] Output shift = 1, input latch = 0 setting is prohibited\n");

    } else if (0x0 == GetBitField("SFMASPBCR", "SPBR")) {
        isProhibited = true;
        re_printf("warning","[SPI Mode] Bit rate needs to be changed to non-zero value before communication starts\n");

    } else if ((emSPIModeReadEnable == GetBitField("SFMASMCR", "SPIRE"))
            && (emSPIModeWriteEnable == GetBitField("SFMASMCR", "SPIWE"))
            && ((emTwoBitsSize == GetBitField("SFMASMENR", "SPIDB"))
                    || (emFourBitSize == GetBitField("SFMASMENR", "SPIDB")))) {
        isProhibited = true;
        re_printf("warning","[SPI Mode] Transfer data bit size is set to 2 bits or 4 bits but both SPIRE and SPIWE bits set to 1\n");

    } else if (emProhibitedSetting == GetBitField("SFMASMENR", "CDB")) {
        isProhibited = true;
        re_printf("warning","[SPI Mode] Setting is prohibited: Command Bit Size\n");

    } else if (emProhibitedSetting == GetBitField("SFMASMENR", "OCDB")) {
        isProhibited = true;
        re_printf("warning","[SPI Mode] Setting is prohibited: Optional Command Bit Size\n");

    } else if (emProhibitedSetting == GetBitField("SFMASMENR", "ADB")) {
        isProhibited = true;
        re_printf("warning","[SPI Mode] Setting is prohibited: Address Bit Size\n");

    } else if (emProhibitedSetting == GetBitField("SFMASMENR", "OPDB")) {
        isProhibited = true;
        re_printf("warning","[SPI Mode] Setting is prohibited: Option Data Bit Size\n");

    } else if (emProhibitedSetting == GetBitField("SFMASMENR", "SPIDB")) {
        isProhibited = true;
        re_printf("warning","[SPI Mode] Setting is prohibited: Transfer Data Bit Size\n");

    } else if (((emSPIModeReadDisable == GetBitField("SFMASMCR", "SPIRE"))
                    || (emSPIModeDataDisable == GetBitField("SFMASMENR", "SPIDE")))
            && (emEnable == GetBitField("SFMASMENR", "DME"))) {
        isProhibited = true;
        re_printf("warning","[SPI Mode] Dummy cycle insertion is prohibited for write in SPI operating mode or a transfer ends with a dummy cycle\n");

    } else if (emProhibitedSetting == GetBitField("SFMASMDMCR", "DMDB")) {
        isProhibited = true;
        re_printf("warning","[SPI Mode] Setting is prohibited: Dummy Cycle Bit Size\n");

    } else if ((emDisable == GetBitField("SFMASMENR", "CDE"))
            && (emDisable == GetBitField("SFMASMENR", "OCDE"))
            && (emDisable == GetBitField("SFMASMENR", "ADE"))
            && (emDisable == GetBitField("SFMASMENR", "OPDE"))
            && (emEnable == GetBitField("SFMASMENR", "DME"))) {
        isProhibited = true;
        re_printf("warning","[SPI Mode] Setting is prohibited: a transfer starting with a dummy cycle\n");

    } else if ((emSPIModeAddressDisable != GetBitField("SFMASMENR", "ADE"))
            && (emSPIModeAddress_23_16 != GetBitField("SFMASMENR", "ADE"))
            && (emSPIModeAddress_23_8 != GetBitField("SFMASMENR", "ADE"))
            && (emSPIModeAddress_23_0 != GetBitField("SFMASMENR", "ADE"))
            && (emSPIModeAddress_32_0 != GetBitField("SFMASMENR", "ADE"))) {
        isProhibited = true;
        re_printf("warning","[SPI Mode] Setting is prohibited: Address Enable\n");

    } else if ((emOpDataDisable != GetBitField("SFMASMENR", "OPDE"))
            && (emOpDataOneByte != GetBitField("SFMASMENR", "OPDE"))
            && (emOpDataTwoBytes != GetBitField("SFMASMENR", "OPDE"))
            && (emOpDataThreeBytes != GetBitField("SFMASMENR", "OPDE"))
            && (emOpDataFourBytes != GetBitField("SFMASMENR", "OPDE"))) {
        isProhibited = true;
        re_printf("warning","[SPI Mode] Setting is prohibited: Option Data Enable\n");

    } else if ((emSPIModeDataDisable != GetBitField("SFMASMENR", "SPIDE"))
            && (emSPIModeDataOneByte != GetBitField("SFMASMENR", "SPIDE"))
            && (emSPIModeDataTwoBytes != GetBitField("SFMASMENR", "SPIDE"))
            && (emSPIModeDataFourBytes != GetBitField("SFMASMENR", "SPIDE"))) {
        isProhibited = true;
        re_printf("warning","[SPI Mode] Setting is prohibited: Transfer Data Enable\n");

    } else if ((emDisable == GetBitField("SFMASMENR", "CDE"))
            && (emDisable == GetBitField("SFMASMENR", "OCDE"))
            && (emDisable == GetBitField("SFMASMENR", "ADE"))
            && (emDisable == GetBitField("SFMASMENR", "OPDE"))
            && (emDisable == GetBitField("SFMASMENR", "SPIDE"))) {
        isProhibited = true;
        re_printf("warning","[SPI Mode] Setting is prohibited: Disabling all of data fields\n");

    }

    return isProhibited;
}//}}}

/// Flush Cache
void SFMA::FlushCache(void)
{//{{{
    mpSFMAReadCache->FlushCache();
}//}}}

/// Is External Read setting prohibited
bool SFMA::IsExtReadSettingProhibited(void)
{//{{{
    bool isProhibited = false;

    // Checking prohibited cases at first
    if (emSPIMode == GetBitField("SFMACMNCR", "MD")) {
        isProhibited = true;
        re_printf("warning","[ExRead Mode] Operation mode is not correct: SPI operating mode\n");

    } else if ((0x1 == GetBitField("SFMACMNCR", "CPHAT"))
            && (0x0 == GetBitField("SFMACMNCR", "CPHAR"))) {
        isProhibited = true;
        re_printf("warning","[ExRead Mode] Output shift = 1, input latch = 0 setting is prohibited\n");

    } else if ((0x0 == GetBitField("SFMADRENR", "ADE"))
            && (0x1 == GetBitField("SFMADRCR", "RBE"))) {
        isProhibited = true;
        re_printf("warning","[ExRead Mode] In Burst Read Operation, disable address output is prohibited\n");

    } else if (0x0 == GetBitField("SFMASPBCR", "SPBR")) {
        isProhibited = true;
        re_printf("warning","[ExRead Mode] Bit rate needs to be changed to non-zero value before communication starts\n");

    } else if (emProhibitedSetting == GetBitField("SFMADRENR", "CDB")) {
        isProhibited = true;
        re_printf("warning","[ExRead Mode] Setting is prohibited: Command Bit Size\n");

    } else if (emProhibitedSetting == GetBitField("SFMADRENR", "OCDB")) {
        isProhibited = true;
        re_printf("warning","[ExRead Mode] Setting is prohibited: Optional Command Bit Size\n");

    } else if (emProhibitedSetting == GetBitField("SFMADRENR", "ADB")) {
        isProhibited = true;
        re_printf("warning","[ExRead Mode] Setting is prohibited: Address Bit Size\n");

    } else if (emProhibitedSetting == GetBitField("SFMADRENR", "OPDB")) {
        isProhibited = true;
        re_printf("warning","[ExRead Mode] Setting is prohibited: Option Data Bit Size\n");

    } else if (emProhibitedSetting == GetBitField("SFMADRENR", "DRDB")) {
        isProhibited = true;
        re_printf("warning","[ExRead Mode] Setting is prohibited: Data Read Bit Size\n");

    } else if (emProhibitedSetting == GetBitField("SFMADRDMCR", "DMDB")) {
        isProhibited = true;
        re_printf("warning","[ExRead Mode] Setting is prohibited: Dummy Cycle Bit Size\n");

    } else if ((emDisable == GetBitField("SFMADRENR", "CDE"))
            && (emDisable == GetBitField("SFMADRENR", "OCDE"))
            && (emDisable == GetBitField("SFMADRENR", "ADE"))
            && (emDisable == GetBitField("SFMADRENR", "OPDE"))
            && (emEnable == GetBitField("SFMADRENR", "DME"))) {
        isProhibited = true;
        re_printf("warning","[ExRead Mode]Setting is prohibited: a transfer starting with a dummy cycle\n");

    } else if ((emExtReadAddressDisable != GetBitField("SFMADRENR", "ADE"))
            && (emExtReadAddressThreeBytes != GetBitField("SFMADRENR", "ADE"))
            && (emExtReadAddressFourBytes != GetBitField("SFMADRENR", "ADE"))) {
        isProhibited = true;
        re_printf("warning","[ExRead Mode]Setting is prohibited: Address Enable\n");

    } else if ((emOpDataDisable != GetBitField("SFMADRENR", "OPDE"))
            && (emOpDataOneByte != GetBitField("SFMADRENR", "OPDE"))
            && (emOpDataTwoBytes != GetBitField("SFMADRENR", "OPDE"))
            && (emOpDataThreeBytes != GetBitField("SFMADRENR", "OPDE"))
            && (emOpDataFourBytes != GetBitField("SFMADRENR", "OPDE"))) {
        isProhibited = true;
        re_printf("warning","[ExRead Mode] Setting is prohibited: Option Data Enable\n");

    }

    return isProhibited;
}//}}}

/// Normal Read Communicate
void SFMA::NormalReadCommunicate(unsigned int address, unsigned char *pData, unsigned int size, BusTime_t* t)
{//{{{
    DataTransfer dataTrans;
    TlmTransactionType tlmTrans;
    double spiTime = 0;
    unsigned int busSize = 1;
    unsigned int dataLen = 0;
    unsigned int readData = 0;
    unsigned char dataEnable = 0;
    BusTime_t localTime = SC_ZERO_TIME;

    // Transfer start
    mpSFMAFunc->SetBitField("SFMACMNSR", "TEND", 0x0);

    // Calculate SPI clock
    CalculateSPBCLKPeriod();

    // Command
    ExtReadHandleData(emCommand, address, &busSize, &dataLen, &dataTrans.CMD);
    spiTime += (double) (dataLen / busSize) * mSPBCLKPeriod;
    if (dataLen != 0) {
        dataEnable |= SFMA_TlmExtension::emCommand_Enable;
    }

    // Option command
    ExtReadHandleData(emOpCommand, address, &busSize, &dataLen, &dataTrans.OCMD);
    spiTime += (double) (dataLen / busSize) * mSPBCLKPeriod;
    if (dataLen != 0) {
        dataEnable |= SFMA_TlmExtension::emOpCommand_Enable;
    }

    // Address
    ExtReadHandleData(emAddress, address, &busSize, &dataLen, &dataTrans.ADR);
    spiTime += (double) (dataLen / busSize) * mSPBCLKPeriod;
    if (dataLen != 0) {
        dataEnable |= SFMA_TlmExtension::emAddress_Enable;
    }

    // Option data
    ExtReadHandleData(emOpData, address, &busSize, &dataLen, &dataTrans.OPD);
    spiTime += (double) (dataLen / busSize) * mSPBCLKPeriod;
    if (dataLen != 0) {
        dataEnable |= SFMA_TlmExtension::emOpData_Enable;
    }

    // Read data
    ExtReadHandleData(emReadData, address, &busSize, &dataLen, &dataTrans.DATA);
    spiTime += (double) (8 * size / busSize) * mSPBCLKPeriod;
    dataEnable |= SFMA_TlmExtension::emReadData_Enable;

    // Add delay time and dummy cycle
    // Delay t1
    spiTime += (double)mSPBCLKPeriod * (double)(GetBitField("SFMASSLDR", "SCKDL") + 1);
    // Delay t2
    spiTime += (double)mSPBCLKPeriod * ((double)GetBitField("SFMASSLDR", "SLNDL") + 1.5);
    // Delay t3
    spiTime += (double)mSPBCLKPeriod * (double)(GetBitField("SFMASSLDR", "SPNDL") + 1);
    // Dummy cycle
    if(0 != GetBitField("SFMADRENR", "DME")) {
        spiTime += (double)mSPBCLKPeriod * (double)(GetBitField("SFMADRDMCR", "DMCYC") + 1);
    }

    // Clear data array
    memset(&mDataArray[0], 0, sizeof(mDataArray));
    // Copy data to array
    memcpy(&mDataArray[emCommandIndex], &dataTrans.CMD, 1);
    memcpy(&mDataArray[emOpCommandIndex], &dataTrans.OCMD, 1);
    memcpy(&mDataArray[emAddressIndex], &dataTrans.ADR, 4);
    memcpy(&mDataArray[emOpDataIndex], &dataTrans.OPD, 4);

    // Extension
    mSfmaExtension->setReadWriteDataSize(size);
    mSfmaExtension->setTransactionCommand(SFMA_TlmExtension::emTrans_Read_Command);
    mSfmaExtension->setDataFieldEnable(dataEnable);

    // Build tlm transaction
    tlmTrans.set_data_length(emPayloadLen);
    tlmTrans.set_data_ptr(mDataArray);
    tlmTrans.set_extension(mSfmaExtension);

    // Send communication time to memory
    localTime = sc_time(spiTime, mTimeResolutionUnit);

    // Transport
    is->b_transport(tlmTrans, localTime);

    tlmTrans.clear_extension(mSfmaExtension);

    re_printf("info", "[Normal Read] Issue read request: Command = 0x%02X, Option Command = 0x%02X, Address = 0x%08X, Option data = 0x%08X\n",
            dataTrans.CMD, dataTrans.OCMD, dataTrans.ADR, dataTrans.OPD);

    // Get data
    if((pData != NULL) && (tlmTrans.get_data_ptr() != NULL)) {
        memcpy(pData, tlmTrans.get_data_ptr() + emReadDataIndex, size);

        // Print read data
        memcpy(&readData, tlmTrans.get_data_ptr() + emReadDataIndex, size);
        re_printf("info", "[Normal Read] Read data = 0x%X\n", readData);

        sc_assert(t != NULL);
        *t += localTime;
    }

    // Event to set TEND bit after SPI communication time
    mTransferEndEvent.notify(spiTime, mTimeResolutionUnit);
}//}}}

/// Burst Read Communicate
void SFMA::BurstReadCommunicate(unsigned int address, unsigned char *pData, unsigned int size, BusTime_t* t)
{//{{{
    DataTransfer dataTrans;
    TlmTransactionType tlmTrans;
    double spiTime = 0;
    double t1 = 0;
    double t2 = 0;
    double t3 = 0;
    double readTime = 0;
    double infoTime = 0;
    static unsigned int previousAddr = 0;
    unsigned int busSize = 1;
    unsigned int dataLen = 0;
    unsigned int offset = address & 0x7;
    uint64_t data = 0;
    uint64_t readData = 0;
    unsigned int currentTend = 0;
    unsigned char dataEnable = 0;
    BusTime_t localTime = SC_ZERO_TIME;

    // Modify the address
    ExtReadHandleData(emAddress, address, &busSize, &dataLen, &dataTrans.ADR);

    // Get data from cache
    if(true == mpSFMAReadCache->GetData(dataTrans.ADR >> 3, &data)) {
        data >>= offset * 8;
        sc_assert(pData != NULL);
        memcpy(pData, &data, size);

        // Print read data
        memcpy(&readData, &data, size);
        re_printf("info", "[Burst Read] Read cache data = 0x%lX\n", readData);
    } else {
        currentTend = GetBitField("SFMACMNSR", "TEND");
        if (1 == currentTend) {
            // Transfer start
            mpSFMAFunc->SetBitField("SFMACMNSR", "TEND", 0x0);
        }

        // Calculate SPI clock
        CalculateSPBCLKPeriod();

        // Command
        ExtReadHandleData(emCommand, address, &busSize, &dataLen, &dataTrans.CMD);
        infoTime += (double) (dataLen / busSize) * mSPBCLKPeriod;
        if (dataLen != 0) {
            dataEnable |= SFMA_TlmExtension::emCommand_Enable;
        }

        // Option command
        ExtReadHandleData(emOpCommand, address, &busSize, &dataLen, &dataTrans.OCMD);
        infoTime += (double) (dataLen / busSize) * mSPBCLKPeriod;
        if (dataLen != 0) {
            dataEnable |= SFMA_TlmExtension::emOpCommand_Enable;
        }

        // Address
        ExtReadHandleData(emAddress, address, &busSize, &dataLen, &dataTrans.ADR);
        // Read with boundary 64-bit
        dataTrans.ADR &= 0xFFFFFFF8;
        infoTime += (double) (dataLen / busSize) * mSPBCLKPeriod;
        if (dataLen != 0) {
            dataEnable |= SFMA_TlmExtension::emAddress_Enable;
        }

        // Option data
        ExtReadHandleData(emOpData, address, &busSize, &dataLen, &dataTrans.OPD);
        infoTime += (double) (dataLen / busSize) * mSPBCLKPeriod;
        if (dataLen != 0) {
            dataEnable |= SFMA_TlmExtension::emOpData_Enable;
        }

        // Read data
        ExtReadHandleData(emReadData, address, &busSize, &dataLen, &dataTrans.DATA);
        readTime += (double) ((GetBitField("SFMADRCR", "RBURST") + 1) * 64 / busSize) * mSPBCLKPeriod;
        dataEnable |= SFMA_TlmExtension::emReadData_Enable;

        // Add delay time and dummy cycle
        // Delay t1
        t1 += (double)mSPBCLKPeriod * (double)(GetBitField("SFMASSLDR", "SCKDL") + 1);
        // Delay t2
        t2 += (double)mSPBCLKPeriod * ((double)GetBitField("SFMASSLDR", "SLNDL") + 1.5);
        // Delay t3
        t3 += (double)mSPBCLKPeriod * (double)(GetBitField("SFMASSLDR", "SPNDL") + 1);
        // Dummy cycle
        if(0 != GetBitField("SFMADRENR", "DME")) {
            infoTime += (double)mSPBCLKPeriod * (double)(GetBitField("SFMADRDMCR", "DMCYC") + 1);
        }

        // Clear data array
        memset(&mDataArray[0], 0, sizeof(mDataArray));
        // Copy data to array
        memcpy(&mDataArray[emCommandIndex], &dataTrans.CMD, 1);
        memcpy(&mDataArray[emOpCommandIndex], &dataTrans.OCMD, 1);
        memcpy(&mDataArray[emAddressIndex], &dataTrans.ADR, 4);
        memcpy(&mDataArray[emOpDataIndex], &dataTrans.OPD, 4);

        // Extension
        mSfmaExtension->setReadWriteDataSize(8 * (GetBitField("SFMADRCR", "RBURST") + 1));
        mSfmaExtension->setTransactionCommand(SFMA_TlmExtension::emTrans_Read_Command);
        mSfmaExtension->setDataFieldEnable(dataEnable);

        // Build tlm transaction
        tlmTrans.set_data_length(emPayloadLen);
        tlmTrans.set_data_ptr(mDataArray);
        tlmTrans.set_extension(mSfmaExtension);

        // SPBSSL is negated after transfer of data set in burst length
        if (0x0 == GetBitField("SFMADRCR", "SSLE")) {
            spiTime = t1 + t2 + t3 + infoTime + readTime;
            // Event to set TEND bit after SPI communication time
            mTransferEndEvent.notify(spiTime, mTimeResolutionUnit);
        } else if ((0x1 == GetBitField("SFMADRCR", "SSLE"))
                && (0x1 == currentTend)) { // First time of automatic SSL negation
            spiTime = t1 + t2 + infoTime + readTime;
            // Save address
            previousAddr = dataTrans.ADR;
        } else if ((0x1 == GetBitField("SFMADRCR", "SSLE"))
                && (0x0 == currentTend)) { // Not first time of automatic SSL negation
            // Address is continuous
            if (dataTrans.ADR == (previousAddr + mSfmaExtension->getReadWriteDataSize())) {
                spiTime = t1 + t2 + readTime;
            } else {
                spiTime = t1 + t2 + t3 + infoTime + readTime;
            }
            // Save address
            previousAddr = dataTrans.ADR;
        }

        // Send communication time to memory
        localTime = sc_time(spiTime, mTimeResolutionUnit);

        // Transport
        is->b_transport(tlmTrans, localTime);

        tlmTrans.clear_extension(mSfmaExtension);

        re_printf("info", "[Burst Read] Issue read request: Command = 0x%02X, Option Command = 0x%02X, Address = 0x%08X, Option data = 0x%08X\n",
                dataTrans.CMD, dataTrans.OCMD, dataTrans.ADR, dataTrans.OPD);

        // Push data
        if((pData != NULL) && (tlmTrans.get_data_ptr() != NULL)) {
            // Store data to cache
            for(unsigned int i = 0; i < mSfmaExtension->getReadWriteDataSize() / 8; i ++) {
                memcpy(&data, tlmTrans.get_data_ptr() + emReadDataIndex + i*8, 8);
                mpSFMAReadCache->StoreData((dataTrans.ADR >> 3) + i, data);
            }

            // Read data from cache
            if (true == mpSFMAReadCache->GetData(dataTrans.ADR >> 3, &data)) {
                data >>= offset * 8;
                memcpy(pData, &data, size);

                // Print read data
                memcpy(&readData, &data, size);
                re_printf("info", "[Burst Read] Read data = 0x%lX\n", readData);
            }
            // Update time
            sc_assert(t != NULL);
            *t += localTime;
        }
    }
}//}}}

/// SPI Mode Communicate
void SFMA::SPIModeCommunicate(void)
{//{{{
    DataTransfer dataTrans;
    TlmTransactionType tlmTrans;
    BusTime_t localTime = SC_ZERO_TIME;
    unsigned int busSize = 1;
    unsigned int dataLen = 0;
    unsigned int transferDataLen = 0;
    unsigned int data = 0;
    double spiTime = 0;
    unsigned char dataEnable = 0;

    // Transfer start
    mpSFMAFunc->SetBitField("SFMACMNSR", "TEND", 0x0);

    // Calculate SPI clock
    CalculateSPBCLKPeriod();

    // Command
    SPIModeHandleData(emCommand, &busSize, &dataLen, &dataTrans.CMD);
    spiTime += (double) (dataLen / busSize) * mSPBCLKPeriod;
    if (dataLen != 0) {
        dataEnable |= SFMA_TlmExtension::emCommand_Enable;
    }

    // Option command
    SPIModeHandleData(emOpCommand, &busSize, &dataLen, &dataTrans.OCMD);
    spiTime += (double) (dataLen / busSize) * mSPBCLKPeriod;
    if (dataLen != 0) {
        dataEnable |= SFMA_TlmExtension::emOpCommand_Enable;
    }

    // Address
    SPIModeHandleData(emAddress, &busSize, &dataLen, &dataTrans.ADR);
    spiTime += (double) (dataLen / busSize) * mSPBCLKPeriod;
    if (dataLen != 0) {
        dataEnable |= SFMA_TlmExtension::emAddress_Enable;
    }

    // Option data
    SPIModeHandleData(emOpData, &busSize, &dataLen, &dataTrans.OPD);
    spiTime += (double) (dataLen / busSize) * mSPBCLKPeriod;
    if (dataLen != 0) {
        dataEnable |= SFMA_TlmExtension::emOpData_Enable;
    }

    // Transfer data
    SPIModeHandleData(emWriteData, &busSize, &transferDataLen, &dataTrans.DATA);
    SPIModeHandleData(emReadData, &busSize, &transferDataLen, &dataTrans.DATA);
    spiTime += (double) (transferDataLen / busSize) * mSPBCLKPeriod;

    // Add delay time and dummy cycle
    // Delay t1
    spiTime += (double)mSPBCLKPeriod * (double)(GetBitField("SFMASSLDR", "SCKDL") + 1);
    // Delay t2
    spiTime += (double)mSPBCLKPeriod * ((double)GetBitField("SFMASSLDR", "SLNDL") + 1.5);
    // Delay t3
    spiTime += (double)mSPBCLKPeriod * (double)(GetBitField("SFMASSLDR", "SPNDL") + 1);
    // Dummy cycle
    if(0 != GetBitField("SFMASMENR", "DME")) {
        spiTime += (double)mSPBCLKPeriod * (double)(GetBitField("SFMASMDMCR", "DMCYC") + 1);
    }

    // Clear data array
    memset(&mDataArray[0], 0, sizeof(mDataArray));
    // Copy data to array
    memcpy(&mDataArray[emCommandIndex], &dataTrans.CMD, 1);
    memcpy(&mDataArray[emOpCommandIndex], &dataTrans.OCMD, 1);
    memcpy(&mDataArray[emAddressIndex], &dataTrans.ADR, 4);
    memcpy(&mDataArray[emOpDataIndex], &dataTrans.OPD, 4);

    if ((0 == GetBitField("SFMASMENR", "SPIDE"))
            || ((0 == GetBitField("SFMASMCR", "SPIWE"))
                    && (0 == GetBitField("SFMASMCR", "SPIRE")))) {
        // No read or write data
        // Extension
        mSfmaExtension->setReadWriteDataSize(0);
        mSfmaExtension->setTransactionCommand(SFMA_TlmExtension::emTrans_Write_Command);
        mSfmaExtension->setDataFieldEnable(dataEnable);

        // Build tlm transaction
        tlmTrans.set_data_length(emPayloadLen);
        tlmTrans.set_data_ptr(mDataArray);
        tlmTrans.set_extension(mSfmaExtension);

        // Send communication time to memory
        localTime = sc_time(spiTime, mTimeResolutionUnit);

        // Transport
        is->b_transport(tlmTrans, localTime);
        re_printf("info", "[SPI Mode] Issue write request: Command = 0x%02X, Option Command = 0x%02X, Address = 0x%08X, Option data = 0x%08X\n",
                dataTrans.CMD, dataTrans.OCMD, dataTrans.ADR, dataTrans.OPD);
    } else if ((0 != GetBitField("SFMASMCR", "SPIWE"))
            && (0 != GetBitField("SFMASMCR", "SPIRE"))) {
        // Read and write data
        dataEnable |= SFMA_TlmExtension::emReadData_Enable;
        dataEnable |= SFMA_TlmExtension::emWriteData_Enable;
        // Copy data to array
        memcpy(&mDataArray[emWriteDataIndex], &dataTrans.DATA, 4);

        // Extension
        mSfmaExtension->setReadWriteDataSize(transferDataLen / 8);
        mSfmaExtension->setTransactionCommand(SFMA_TlmExtension::emTrans_ReadWrite_Command);
        mSfmaExtension->setDataFieldEnable(dataEnable);

        // Build tlm transaction
        tlmTrans.set_data_length(emPayloadLen);
        tlmTrans.set_data_ptr(mDataArray);
        tlmTrans.set_extension(mSfmaExtension);

        // Send communication time to memory
        localTime = sc_time(spiTime, mTimeResolutionUnit);

        // Transport
        is->b_transport(tlmTrans, localTime);
        re_printf("info", "[SPI Mode] Issue read-write request: Command = 0x%02X, Option Command = 0x%02X, Address = 0x%08X, Option data = 0x%08X, Write data = 0x%08X\n",
                dataTrans.CMD, dataTrans.OCMD, dataTrans.ADR, dataTrans.OPD, dataTrans.DATA);

        // Get read data
        memcpy(&data, tlmTrans.get_data_ptr() + emReadDataIndex, transferDataLen / 8);
        mpSFMAFunc->SetBitField("SFMASMRDR", "RDATA", data);
        re_printf("info", "[SPI Mode] Read data = 0x%X\n", data);
    } else if ((0 != GetBitField("SFMASMCR", "SPIWE"))
            && (0 == GetBitField("SFMASMCR", "SPIRE"))) {
        // Write data
        dataEnable |= SFMA_TlmExtension::emWriteData_Enable;
        // Copy data to array
        memcpy(&mDataArray[emWriteDataIndex], &dataTrans.DATA, 4);

        // Extension
        mSfmaExtension->setReadWriteDataSize(transferDataLen / 8);
        mSfmaExtension->setTransactionCommand(SFMA_TlmExtension::emTrans_Write_Command);
        mSfmaExtension->setDataFieldEnable(dataEnable);

        // Build tlm transaction
        tlmTrans.set_data_length(emPayloadLen);
        tlmTrans.set_data_ptr(mDataArray);
        tlmTrans.set_extension(mSfmaExtension);

        // Send communication time to memory
        localTime = sc_time(spiTime, mTimeResolutionUnit);

        // Transport
        is->b_transport(tlmTrans, localTime);
        re_printf("info", "[SPI Mode] Issue write request: Command = 0x%02X, Option Command = 0x%02X, Address = 0x%08X, Option data = 0x%08X, Write data = 0x%08X\n",
                dataTrans.CMD, dataTrans.OCMD, dataTrans.ADR, dataTrans.OPD, dataTrans.DATA);
    } else if ((0 == GetBitField("SFMASMCR", "SPIWE"))
            && (0 != GetBitField("SFMASMCR", "SPIRE"))) {
        // Read data
        dataEnable |= SFMA_TlmExtension::emReadData_Enable;
        // Extension
        mSfmaExtension->setReadWriteDataSize(transferDataLen / 8);
        mSfmaExtension->setTransactionCommand(SFMA_TlmExtension::emTrans_Read_Command);
        mSfmaExtension->setDataFieldEnable(dataEnable);

        // Build tlm transaction
        tlmTrans.set_data_length(emPayloadLen);
        tlmTrans.set_data_ptr(mDataArray);
        tlmTrans.set_extension(mSfmaExtension);

        // Send communication time to memory
        localTime = sc_time(spiTime, mTimeResolutionUnit);

        // Transport
        is->b_transport(tlmTrans, localTime);
        re_printf("info", "[SPI Mode] Issue read request: Command = 0x%02X, Option Command = 0x%02X, Address = 0x%08X, Option data = 0x%08X\n",
                dataTrans.CMD, dataTrans.OCMD, dataTrans.ADR, dataTrans.OPD);

        // Get read data
        memcpy(&data, tlmTrans.get_data_ptr() + emReadDataIndex, transferDataLen / 8);
        mpSFMAFunc->SetBitField("SFMASMRDR", "RDATA", data);
        re_printf("info", "[SPI Mode] Read data = 0x%X\n", data);
    }

    tlmTrans.clear_extension(mSfmaExtension);

    // Event to set TEND bit after SPI communication time
    mTransferEndEvent.notify(spiTime, mTimeResolutionUnit);
}//}}}

/// Print message
void SFMA::PrintMsg(const char* severity, const char* msg)
{//{{{
    _re_printf(severity, msg);
}//}}}
//========================================================
//============ Reset and clock  ==========================
//========================================================
/// Monitor RES_SFMA port
void SFMA::HandleRES_SFMASignalMethod(void)
{//{{{
    mResetCurVal = RES_SFMA.read();
    if (mResetCurVal != mResetPreVal) { //mResetPreVal is "true" at initial state
        mResetPreVal = mResetCurVal;
        // Reset signals are asynchronous
        mResetHardEvent.notify(SC_ZERO_TIME);
    }
}//}}}

/// Reset model and its sub-instance if reset is actived
void SFMA::EnableReset(const bool is_active)
{//{{{
    if (is_active){
        // Cancel events
        CancelEvents();
        // Initialize variables
        Initialize();
    }

    mpSFMAFunc->EnableReset(is_active);
    mpSFMAExternalRead->EnableReset(is_active);
    mpSFMAReadCache->EnableReset(is_active);
}//}}}


/// Monitor clkb clock
void SFMA::HandleclkbSignalMethod(void)
{//{{{
    sc_dt::uint64 freq_value = clkb.read();
    SetCLKFreq("clkb", freq_value, "Hz");
}//}}}

/// Process reset function when reset port is active
void SFMA::HandleResetHardMethod(void)
{//{{{
    if (mResetCurVal == vpcl::emResetActive) {
        mIsResetHardActive = true;
        re_printf("info","The reset port RES_SFMA is asserted.\n");
        this->EnableReset(mIsResetHardActive);
    } else {
        if (mIsResetHardActive) {
            mIsResetHardActive = false;
            re_printf("info","The reset port RES_SFMA is de-asserted.\n");
            this->EnableReset(mIsResetHardActive);
        }
    }

    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive && mIsResetHardActive) {
        mResetCmdEvent.cancel();
        mResetCmdCancelEvent.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
    }
} //}}}
 
/// Process reset function when reset command is active
void SFMA::HandleResetCmdMethod(void)
{//{{{
    std::string reset_name = "RES_SFMA";
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

/// Cancel reset function when reset command is active
void SFMA::CancelResetCmdMethod(void)
{//{{{
    std::string reset_name = "RES_SFMA";
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive);
}//}}}

//========================================================
//============ Operation               ===================
//========================================================
// Calculate SPBCLK Period
void SFMA::CalculateSPBCLKPeriod(void)
{//{{{
    mSPBCLKPeriod = mclkbPeriod * 2
            * GetBitField("SFMASPBCR", "SPBR")
            *(sc_dt::uint64)(pow(2, GetBitField("SFMASPBCR", "BRDV")));
}//}}}

/// Get Transfer Data
void SFMA::ExtReadHandleData(eDataField dataField, unsigned int address, unsigned int *pBusSize, unsigned int *pDatalen, unsigned int *pData)
{//{{{
    sc_assert(pData != NULL);
    sc_assert(pBusSize != NULL);
    sc_assert(pDatalen != NULL);
    *pData = 0;
    *pBusSize = 1;
    *pDatalen = 0;

    switch (dataField) {

    case emCommand:
        if (emEnable == GetBitField("SFMADRENR", "CDE")) {
            *pData = GetBitField("SFMADRCMR", "CMD");
            *pDatalen = 8;
        }

        *pBusSize = GetBusSize(GetBitField("SFMADRENR", "CDB"));
        break;

    case emOpCommand:
        if (emEnable == GetBitField("SFMADRENR", "OCDE")) {
            *pData = GetBitField("SFMADRCMR", "OCMD");
            *pDatalen = 8;
        }

        *pBusSize = GetBusSize(GetBitField("SFMADRENR", "OCDB"));
        break;

    case emAddress:
        if (emExtReadAddressFourBytes == GetBitField("SFMADRENR", "ADE")) {
            *pData = (address & 0x3FFFFFF) | (GetBitField("SFMADREAR", "EAV") << 26);
            *pDatalen = 32;
        } else if (emExtReadAddressThreeBytes == GetBitField("SFMADRENR", "ADE")) {
            *pData = address & 0xFFFFFF;
            *pDatalen = 24;
        }

        *pBusSize = GetBusSize(GetBitField("SFMADRENR", "ADB"));
        break;

    case emOpData:
        if (emOpDataOneByte == GetBitField("SFMADRENR", "OPDE")) {
            *pData = GetBitField("SFMADROPR", "OPD3");
            *pDatalen = 8;
        } else if (emOpDataTwoBytes == GetBitField("SFMADRENR", "OPDE")) {
            *pData = (GetBitField("SFMADROPR", "OPD3") << 8)
                    | GetBitField("SFMADROPR", "OPD2");
            *pDatalen = 16;
        } else if (emOpDataThreeBytes == GetBitField("SFMADRENR", "OPDE")) {
            *pData = (GetBitField("SFMADROPR", "OPD3") << 16)
                    | (GetBitField("SFMADROPR", "OPD2") << 8)
                    | GetBitField("SFMADROPR", "OPD1");
            *pDatalen = 24;
        } else if (emOpDataFourBytes == GetBitField("SFMADRENR", "OPDE")) {
            *pData = (GetBitField("SFMADROPR", "OPD3") << 24)
                    | (GetBitField("SFMADROPR", "OPD2") << 16)
                    | (GetBitField("SFMADROPR", "OPD1") << 8)
                    | GetBitField("SFMADROPR", "OPD0");
            *pDatalen = 32;
        }

        *pBusSize = GetBusSize(GetBitField("SFMADRENR", "OPDB"));
        break;

    default: // emReadData
        *pBusSize = GetBusSize(GetBitField("SFMADRENR", "DRDB"));
        break;
    }
}//}}}

/// Get Transfer Data
void SFMA::SPIModeHandleData(eDataField dataField, unsigned int *pBusSize, unsigned int *pDatalen, unsigned int *pData)
{//{{{
    sc_assert(pData != NULL);
    sc_assert(pBusSize != NULL);
    sc_assert(pDatalen != NULL);
    switch (dataField) {

    case emCommand:
        if (emEnable == GetBitField("SFMASMENR", "CDE")) {
            *pData = GetBitField("SFMASMCMR", "CMD");
            *pDatalen = 8;
        } else {
            *pData = 0;
            *pDatalen = 0;
        }

        *pBusSize = GetBusSize(GetBitField("SFMASMENR", "CDB"));
        break;

    case emOpCommand:
        if (emEnable == GetBitField("SFMASMENR", "OCDE")) {
            *pData = GetBitField("SFMASMCMR", "OCMD");
            *pDatalen = 8;
        } else {
            *pData = 0;
            *pDatalen = 0;
        }

        *pBusSize = GetBusSize(GetBitField("SFMASMENR", "OCDB"));
        break;

    case emAddress:
        if (emSPIModeAddress_23_16 == GetBitField("SFMASMENR", "ADE")) {
            *pData = GetBitField("SFMASMADR", "ADR") & 0xFF0000;
            *pDatalen = 24;
        } else if (emSPIModeAddress_23_8 == GetBitField("SFMASMENR", "ADE")) {
            *pData = GetBitField("SFMASMADR", "ADR") & 0xFFFF00;
            *pDatalen = 24;
        } else if (emSPIModeAddress_23_0 == GetBitField("SFMASMENR", "ADE")) {
            *pData = GetBitField("SFMASMADR", "ADR") & 0xFFFFFF;
            *pDatalen = 24;
        } else if (emSPIModeAddress_32_0 == GetBitField("SFMASMENR", "ADE")){
            *pData = GetBitField("SFMASMADR", "ADR");
            *pDatalen = 32;
        } else {
            *pData = 0;
            *pDatalen = 0;
        }

        *pBusSize = GetBusSize(GetBitField("SFMASMENR", "ADB"));
        break;

    case emOpData:
        if (emOpDataOneByte == GetBitField("SFMASMENR", "OPDE")) {
            *pData = GetBitField("SFMASMOPR", "OPD3");
            *pDatalen = 8;
        } else if (emOpDataTwoBytes == GetBitField("SFMASMENR", "OPDE")) {
            *pData = (GetBitField("SFMASMOPR", "OPD3") << 8)
                    | GetBitField("SFMASMOPR", "OPD2");
            *pDatalen = 16;
        } else if (emOpDataThreeBytes == GetBitField("SFMASMENR", "OPDE")) {
            *pData = (GetBitField("SFMASMOPR", "OPD3") << 16)
                    | (GetBitField("SFMASMOPR", "OPD2") << 8)
                    | GetBitField("SFMASMOPR", "OPD1");
            *pDatalen = 24;
        } else if (emOpDataFourBytes == GetBitField("SFMASMENR", "OPDE")) {
            *pData = (GetBitField("SFMASMOPR", "OPD3") << 24)
                    | (GetBitField("SFMASMOPR", "OPD2") << 16)
                    | (GetBitField("SFMASMOPR", "OPD1") << 8)
                    | GetBitField("SFMASMOPR", "OPD0");
            *pDatalen = 32;
        } else {
            *pData = 0;
            *pDatalen = 0;
        }

        *pBusSize = GetBusSize(GetBitField("SFMASMENR", "OPDB"));
        break;

    case emReadData:
        if ((emSPIModeWriteDisable != GetBitField("SFMASMCR", "SPIWE"))
                || (emSPIModeReadDisable != GetBitField("SFMASMCR", "SPIRE"))) {
            if (emSPIModeDataOneByte == GetBitField("SFMASMENR", "SPIDE")) {
                *pDatalen = 8;
            } else if (emSPIModeDataTwoBytes == GetBitField("SFMASMENR", "SPIDE")) {
                *pDatalen = 16;
            } else if (emSPIModeDataFourBytes == GetBitField("SFMASMENR", "SPIDE")) {
                *pDatalen = 32;
            } else {
                *pDatalen = 0;
            }
        }

        *pBusSize = GetBusSize(GetBitField("SFMASMENR", "SPIDB"));
        break;

    default:// emWriteData
        if (((emSPIModeWriteDisable != GetBitField("SFMASMCR", "SPIWE")))
                && (emSPIModeDataDisable != GetBitField("SFMASMENR", "SPIDE"))) {
            if (emSPIModeDataOneByte == GetBitField("SFMASMENR", "SPIDE")) {
                *pData = GetBitField("SFMASMWDR", "WDATA") & 0xFF;
            } else if (emSPIModeDataTwoBytes == GetBitField("SFMASMENR", "SPIDE")) {
                *pData = GetBitField("SFMASMWDR", "WDATA") & 0xFFFF;
            } else if (emSPIModeDataFourBytes == GetBitField("SFMASMENR", "SPIDE")) {
                *pData = GetBitField("SFMASMWDR", "WDATA");
            }
        }
        break;
    }
}//}}}

/// Transfer End Method
void SFMA::TransferEndMethod(void)
{//{{{
    mpSFMAFunc->SetBitField("SFMACMNSR", "TEND", 0x1);
}//}}}

/// Cancel operation events
void SFMA::CancelEvents(void)
{//{{{
    mTransferEndEvent.cancel();
}//}}}

// Get data bus size
unsigned int SFMA::GetBusSize(unsigned int regValue)
{
    unsigned int busSize = 1;

    if (emOneBitSize == regValue) {
        busSize = 1;
    } else if (emTwoBitsSize == regValue) {
        busSize = 2;
    } else if (emFourBitSize == regValue) {
        busSize = 4;
    }

    return busSize;
}
//========================================================
//============ PYTHON IF               ===================
//========================================================
/// Set message level (fatal, error, warning, info)
void SFMA::SetMessageLevel(const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }

    if (true == mEnableRegisterMessage) {
        //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpSFMAFunc->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpSFMAFunc->RegisterHandler(cmd);
    } else {
        //Set message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        //SeparateString(cmd, msg_lv);
        cmd.push_back("fatal");

        mpSFMAFunc->RegisterHandler(cmd);

        //Set message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        //SeparateString(cmd, msg_lv);
        cmd.push_back("fatal");

        mpSFMAFunc->RegisterHandler(cmd);
    }
}//}}}

/// Enable/disable dumping message when users access registers
void SFMA::DumpRegisterRW(const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpSFMAFunc->RegisterHandler(cmd);
}//}}}

/// Dump status register's value
void SFMA::DumpStatusInfo(void)
{//{{{
    mpSFMAFunc->DumpStatusInfo();
}//}}}

/// Command to assert reset
void SFMA::AssertReset(const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "RES_SFMA") {
        if ((!mIsResetHardActive)&&(!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (RES_SFMA) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of RES_SFMA is called in the reset operation of the model. So it is ignored.\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be RES_SFMA.\n",reset_name.c_str());
    }
}//}}}

/// Set clock value and clock unit
void SFMA::SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "clkb") {
            mclkbOrgFreq = freq;
            ConvertClockFreq (mclkbFreq, mclkbFreqUnit, freq, unit);
            if (mclkbFreq > 0) {
                mclkbPeriod = (sc_dt::uint64)(((1/(double)mclkbFreq) * (double)mTimeResolutionValue) + 0.5);
            } else {
                mclkbPeriod = 0;
                mClkZeroEvent.notify();
                // Prepare for next run
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Get clock value
void SFMA::GetCLKFreq(const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "clkb") {
            re_printf("info","clkb frequency is %0.0f %s\n", (double)mclkbOrgFreq, mclkbFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Force value to register
void SFMA::ForceRegister(const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("force");
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            mpSFMAFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Release forced value after forcing registers
void SFMA::ReleaseRegister(const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mpSFMAFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Write value to registers by software
void SFMA::WriteRegister(const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            mpSFMAFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Read value of register by software
void SFMA::ReadRegister(const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mpSFMAFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Enable/disable info/warning/error message of register IF
void SFMA::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: SFMA_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        std::string temp = (mEnableRegisterMessage) ? "true" : "false";
        re_printf("info","SFMA_EnableRegisterMessage %s\n", temp.c_str());
    } else {
        re_printf("warning","Invalid argument: SFMA_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

/// List all registers name
void SFMA::ListRegister(void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpSFMAFunc->RegisterHandler(cmd);
}//}}}

/// Dump help message of all parameters or commands
void SFMA::Help(const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"SFMA_MessageLevel (\"SFMA instance\", \"fatal|error|warning|info\")        Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"SFMA_DumpRegisterRW (\"SFMA instance\", \"true/false\")                    Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"SFMA_EnableRegisterMessage (\"SFMA instance\", \"true/false\")             Enable/disable info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"SFMA_DumpStatusInfo (\"SFMA instance\")                                    Dump the status info of model");
            SC_REPORT_INFO(this->basename(),"SFMA_AssertReset (\"SFMA instance\", \"reset_name\", start_time, period)   Assert and de-assert reset signal to the SFMA model");
            SC_REPORT_INFO(this->basename(),"SFMA_SetCLKFreq (\"SFMA instance\", \"clock_name\", freq, \"unit\")        Set clock frequency value to SFMA model. Valid clock_name is clkb");
            SC_REPORT_INFO(this->basename(),"SFMA_GetCLKFreq (\"SFMA instance\", \"clock_name\")                        Get clock frequency value of SFMA model. Valid clock_name is clkb");
            SC_REPORT_INFO(this->basename(),"SFMA_ForceRegister (\"SFMA instance\", \"reg_name\", reg_value)            Force register with setting value");
            SC_REPORT_INFO(this->basename(),"SFMA_ReleaseRegister (\"SFMA instance\", \"reg_name\")                     Release register from force value");
            SC_REPORT_INFO(this->basename(),"SFMA_WriteRegister (\"SFMA instance\", \"reg_name\", reg_value)            Write a value to a register");
            SC_REPORT_INFO(this->basename(),"SFMA_ReadRegister (\"SFMA instance\", \"reg_name\")                        Read a value from a register");
            SC_REPORT_INFO(this->basename(),"SFMA_ListRegister (\"SFMA instance\")                                      Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of SFMA_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of SFMA_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :
