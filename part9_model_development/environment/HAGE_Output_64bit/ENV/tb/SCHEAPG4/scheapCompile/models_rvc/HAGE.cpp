/*************************************************************************
*
*  HAGE.cpp
*
* Copyright(c) 2018-2021 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*
*************************************************************************/

#include "HAGE.h"
#include "HAGE_Func.h"

HAGE::HAGE(sc_module_name name
    , const char* configFile
    , const unsigned int rLatency
    , const unsigned int wLatency
    , const SIM_MODE_T simMode):
    sc_module(name)
    , BusSlaveBase<BUS_WIDTH_APB, 1>()
    , BusMasterBase<BUS_WIDTH_FLSH,1>()
    , HAGE_AgentController((std::string) name)
    , mSimMode(simMode)
    , PCLK("PCLK")
    , PRESET("PRESET")
    , EINT("EINT")
{
    sc_assert(NULL != configFile);

    // Initialize Internal Variables
    mSeedValue = 0;
    mSimMode = simMode;
    EINT.initialize(false);
    mForceStopDMA = false;
    mDMAPayLoad = new TlmBasicPayload;

    mpHAGE_Func = new HAGE_Func((std::string) name, this);
    sc_assert(NULL != mpHAGE_Func);

    // Initialize Bus
    setSlaveResetPort32(&PRESET);
    setSlaveFreqPort32(&PCLK);
    setTargetSocket32((char*)"ts");
    ts = BusSlaveBase<BUS_WIDTH_APB, 1>::tSocket32[0];
    mBusSlaveIf32[0]->setFuncModulePtr((BusSlaveFuncIf *) mpHAGE_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    
    setMasterResetPort128(&PRESET);
    setMasterFreqPort128(&PCLK);
    setInitiatorSocket128((char*)"is");
    is = BusMasterBase<BUS_WIDTH_FLSH, 1>::iSocket128[0];
    mBusMasterIf128[0]->setFuncModulePtr((BusMasterFuncIf*)this);
    mBusMasterIf128[0]->setBusProtocol(BUS_AXI);
    mBusMasterIf128[0]->setTransNmbLmt(0x1);


    SC_METHOD(HandleClkMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(HandleResetMethod);
    dont_initialize();
    sensitive << PRESET;

    SC_THREAD(StartDMAThread);
    dont_initialize();
    sensitive << mStartDMAEvent;

    SC_THREAD(HandleInterruptThread);
    dont_initialize();
    sensitive << mInterruptEvent;
}
    
HAGE::~HAGE(void)
{
    delete mpHAGE_Func;
}

void HAGE::HandleClkMethod()
{
    common_re_printf("info"
        , "PLCK change to %llu (Hz)\n"
        , (unsigned long long) PCLK.read());
}

void HAGE::HandleResetMethod()
{
    common_re_printf("info"
        , "PRESET change to %d\n"
        , (unsigned int) PRESET.read());
}

void HAGE::StartDMAThread()
{
    while (true) {
        uint32_t lDataLength, lSeedValue, lSrcAddress, lDstAddress;
        uint32_t temp_input[4];
        memset(temp_input, 0, 4);
        uint32_t temp_output = 0;

        if (this->mpHAGE_Func->GetHAGECR("MODSEL") == 1) {

            // Read Seed, Data Length, Source Address and Destination Address
            DMA_Read_Input(&lSeedValue, &lDataLength, &lSrcAddress, &lDstAddress);

            // Loop whole message to calculate 
            for (int i = 0; i < lDataLength; ) {
                i += 4;                                                  // Each time prefetch number of bit is 4 * 32 = 128
                // Pre-fetch 16 bytes = 128 bits to temp_input
                DMA_Read_Payload(lSrcAddress + 32 * (i - 4), 16, false); // i=4->read at lSrcAddress; i=8->read at lSrcAddress+128
                memcpy(temp_input, mDMAPayLoad->get_data_ptr(), sizeof(uint32_t) * 4);

                for (int j = 0; j < 4; j++) {
                    if (i == 4 && j == 0) {
                        // First calculation unit
                        temp_output = lSeedValue ^ temp_input[j];
                    }
                    else
                    {
                        temp_output ^= temp_input[j];
                    }
                }
                // DMA stop immediately
                if (mForceStopDMA == true)
                    break;
            }

            //uint32_t temp_lDataLength = 0;
            //uint32_t temp_input       = 0;
            //uint32_t temp_output      = 0;

            //while (temp_lDataLength <= lDataLength) {
            //    // Read from Memory Peripheral at lSrcAddress --> save to temp_input
            //    std::cout << "Read from Memory Peripheral at lSrcAddress --> save to temp_input" << std::endl;

            //    // Calculate HASH
            //    if (temp_lDataLength == 0)
            //        temp_output = lSeedValue ^ temp_input;
            //    else
            //        temp_output ^= temp_input;
            //    temp_lDataLength++;
            //}

            if (this->mpHAGE_Func->GetHAGECR("HASHOUT") == 1) {
                //Write back to Memory Peripheral at lDstAddress
                std::cout << "Write back to Memory Peripheral at lDstAddress" << std::endl;
                DMA_Write_Payload(lDstAddress, 4, temp_output, false);
            }


            if (this->mpHAGE_Func->GetHAGECR("EINT_MSK") == 0)
                // Trigger Interrupt
                this->TriggerInterrupt();
        }
        wait();
    }
}

void HAGE::HandleInterruptThread()
{
    //EINT.write(true);
}

// Notify mStartDMAEvent
void HAGE::StartDMA()
{
    mForceStopDMA = false;
    this->mStartDMAEvent.notify();
}

void HAGE::StopDMA()
{
    mForceStopDMA = true;
    this->mStopDMAEvent.notify();
}

void HAGE::TriggerInterrupt()
{
    this->mInterruptEvent.notify();
}

void HAGE::DMA_Read_Input(uint32_t *lSeedValue, uint32_t *lDataLength, uint32_t *lSrcAddress, uint32_t *lDstAddress)
{
    // Read Seed
    *lSeedValue = (this->mpHAGE_Func->GetHAGECR("NEW_SEED") == 0) ? mSeedValue : this->mpHAGE_Func->GetSEEDDATA();
    mSeedValue  = *lSeedValue;
    this->mpHAGE_Func->SetValue("HAGECR", "NEW_SEED", 0);

    // Read DL, SRCADDR, DSTADDR
    *lDataLength = this->mpHAGE_Func->GetDATALEN();
    *lSrcAddress = this->mpHAGE_Func->GetSRCADD();
    *lDstAddress = this->mpHAGE_Func->GetDSTADD();
}

void HAGE::DMA_Write_Payload(const unsigned int addr, const unsigned int write_size, const unsigned int data, bool is_debug)
{
    /* Declare buffer for storage temp data */

    BusTime_t t_time;
    /* Init for payload */
    this->mDMAPayLoad->set_address((sc_dt::uint64)addr);
    this->mDMAPayLoad->set_data_length(write_size);

    static unsigned char* mData;
    mData = new unsigned char[1024];
    memset(mData, 0, 1024);

    memcpy(mData, &data, write_size);
    this->mDMAPayLoad->set_write();
    this->mDMAPayLoad->set_data_ptr(mData);

    TlmAxiExtension* mAxiExt = new TlmAxiExtension;
    this->mDMAPayLoad->set_extension(mAxiExt);
    TlmG3mExtension* mG3mExt = new TlmG3mExtension;
    this->mDMAPayLoad->set_extension(mG3mExt);


    /* Write DMA payload */
    mBusMasterIf128[0]->write(*this->mDMAPayLoad, is_debug, t_time, false);

    /* wait to switch state*/
    wait(t_time);
}

void HAGE::DMA_Read_Payload(const unsigned int addr, const unsigned int read_size, bool is_debug)
{
    BusTime_t t_time;
    /* Clear buffer for read data 16 bytes = 128 bits */
    memset(this->mDMAReadBuff, 0, 16);

    /* Init for payload */
    mDMAPayLoad->set_address((sc_dt::uint64)addr);
    mDMAPayLoad->set_data_length(read_size);

    /* read command data */
    mDMAPayLoad->set_read();
    mDMAPayLoad->set_data_ptr(this->mDMAReadBuff);

    TlmAxiExtension* mAxiExt = new TlmAxiExtension;
    mDMAPayLoad->set_extension(mAxiExt);
    TlmG3mExtension* mG3mExt = new TlmG3mExtension;
    mDMAPayLoad->set_extension(mG3mExt);


    /* Read transaction */
    mBusMasterIf128[0]->read(*mDMAPayLoad, is_debug, t_time, false);

    /* wait to switch state*/
    wait(t_time);
}

void HAGE::SetMessageLevel(std::string msg_lv)
{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    HandleCommand(cmd);
}
