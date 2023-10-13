/*************************************************************************
*
*  HAGE.h
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

#ifndef __HAGE_H__
#define __HAGE_H__
#include "HAGE_AgentController.h"
class HAGE_Func;

class HAGE : public sc_module
    , public BusSlaveBase<BUS_WIDTH_APB, 1>
    , public BusMasterBase<BUS_WIDTH_FLSH, 1>
    , public HAGE_AgentController
{
public:
    SC_HAS_PROCESS(HAGE);

    sc_in<sc_dt::uint64> PCLK;
    sc_in<bool> PRESET;
    sc_out<bool> EINT;

    TlmTargetSocket<BUS_WIDTH_APB>* ts;
    TlmInitiatorSocket<BUS_WIDTH_FLSH>* is;

    HAGE(sc_module_name name
        , const char* configFile
        , const unsigned int rLatency
        , const unsigned int wLatency
        , const SIM_MODE_T simMode);
    ~HAGE(void);

    /* Python API */
    void SetMessageLevel(std::string msg_lv);

    /* Clock */
    void SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit);
    void GetCLKFreq(const std::string clock_name);
    bool CheckClockPeriod(const std::string clock_name);

    /* Common API */
    void ExchangePLL(void);

private:
    uint32_t   mSeedValue;
    SIM_MODE_T mSimMode;
    HAGE_Func *mpHAGE_Func;


    /*--------------Clock----------- */
    /* PCLK clock */
    sc_dt::uint64   mPCLKClock;
    /* Period of PCLK */
    sc_dt::uint64   mPCLKPeriod;
    /* Period of HASH -> after callulate with PLL */
    sc_dt::uint64 mHASHPeriod;

    /*-------------Time--------------*/
    /* Time resolution value of simulation */
    sc_dt::uint64   mTimeResolutionVal;

    //Declare methods and threads
    void HandleClkMethod();
    void HandleResetMethod();
    void StartDMAThread();
    void HandleInterruptThread();

    void StartDMA();                        // Notify mStartDMAEvent
    void StopDMA();                         // Notify mStopDMAEvent
    void TriggerInterrupt();                // Notify mInterruptEvent

    void DMA_Read_Input(uint32_t* lSeedValue, uint32_t* lDataLength, uint32_t* lSrcAddress, uint32_t* lDstAddress);
    void DMA_Write_Payload(const unsigned int addr, const unsigned int write_size, const unsigned int data, bool is_debug);
    void DMA_Read_Payload (const unsigned int addr, const unsigned int read_size, bool is_debug);

    // Declare Internal Variables
    unsigned          mDMAWriteBuff[4];
    unsigned char     mDMAReadBuff[16];
    bool              mForceStopDMA;
    TlmBasicPayload*  mDMAPayLoad;
    
    // Declare Events
    sc_event mStartDMAEvent;                // Call StartDMAThread when DMASTR is set
    sc_event mStopDMAEvent;                 // Force stop DMA when DMASTR is reset
    sc_event mInterruptEvent;                // Call InterruptThread when finish computation



};

#endif /* __HAGE_H__ */
