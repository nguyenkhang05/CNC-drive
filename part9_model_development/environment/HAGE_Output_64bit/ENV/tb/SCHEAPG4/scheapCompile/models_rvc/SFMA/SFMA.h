// ---------------------------------------------------------------------
// $Id: SFMA.h,v 1.3 2020/01/30 08:01:36 huepham Exp $
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
#ifndef __SFMA_H__
#define __SFMA_H__

#include "re_define.h"
#include "BusSlaveBase.h"
#include "SFMA_AgentController.h"
#include "SFMA_TlmExtension.h"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "rvc_common_model.h"
#include "global.h"

using namespace tlm_utils;
using namespace tlm;

class SFMA_Func;
class SFMA_ExternalRead;
class SFMA_ReadCache;

// SFMA class
class SFMA: public sc_module
            ,public SFMA_AgentController
            ,public BusSlaveBase<64, 2>
            ,public rvc_common_model
{
public:
    // Clock/Reset
    sc_in<sc_dt::uint64>    clkb;                   // clock
    sc_in<bool>             RES_SFMA;               // reset

    // Sockets
    TlmTargetSocket<64>     *tsr;                   // target sockets to access register
    TlmTargetSocket<64>     *tsm;                   // target sockets to read flash memory
    //TlmInitiatorSocket<64>  *is;                    // initiator socket to communicate with flash memory
    simple_initiator_socket_tagged<SFMA> is;        // initiator socket to communicate with flash memory

    // Class pointer
    SFMA_Func               *mpSFMAFunc;            // Pointer of SFMA_Func
    SFMA_ExternalRead       *mpSFMAExternalRead;    // Pointer of SFMA_ExternalRead
    SFMA_ReadCache          *mpSFMAReadCache;       // Pointer of SFMA_ReadCache

    // Extension
    SFMA_TlmExtension       *mSfmaExtension;        // Extension

    SC_HAS_PROCESS (SFMA);
    // Constructor
    SFMA (sc_module_name name, const unsigned int rLatency, const unsigned int wLatency);
    ~SFMA ();

    // PythonIF
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpStatusInfo (void);
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);
    void EnableRegisterMessage(const std::string is_enable);

private:
    // Data transfer struct
    struct DataTransfer {
        unsigned int CMD;
        unsigned int OCMD;
        unsigned int ADR;
        unsigned int OPD;
        unsigned int DATA;
    };

    // Payload length
    // CMD: 1 byte, OCMD: 1 byte, ADR: 4 bytes, OPD: 4 bytes, WRITE DATA: 4 bytes, READ DATA: 128 bytes (8 * RBURST)
    enum ePayloadLen {
        emPayloadLen = 142
    };

    // Transfer data field
    enum eDataField {
        emCommand = 0
        ,emOpCommand= 1
        ,emAddress = 2
        ,emOpData = 3
        ,emReadData = 4
        ,emWriteData = 5
    };

    // Operating mode
    enum eOperatingMode {
        emExtReadMode = 0
        ,emSPIMode= 1
    };

    // Data bit size
    enum eBitSize {
        emOneBitSize = 0
        ,emTwoBitsSize= 1
        ,emFourBitSize = 2
        ,emProhibitedSetting = 3
    };

    // SPI Mode data write enable
    enum eSPIModeWriteEnable {
        emSPIModeWriteDisable = 0
        ,emSPIModeWriteEnable = 1
    };

    // SPI Mode data read enable
    enum eSPIModeReadEnable {
        emSPIModeReadDisable = 0
        ,emSPIModeReadEnable = 1
    };

    // Transfer data enable
    enum eSPIModeDataEnable {
        emSPIModeDataDisable = 0
        ,emSPIModeDataOneByte = 0x8
        ,emSPIModeDataTwoBytes = 0xC
        ,emSPIModeDataFourBytes = 0xF
    };

    // Field enable
    enum eFieldEnable {
        emDisable = 0
        ,emEnable = 1
    };

    // SPI mode Address enable
    enum eSPIModeAddressEnable {
        emSPIModeAddressDisable = 0
        ,emSPIModeAddress_23_16 = 0x4
        ,emSPIModeAddress_23_8 = 0x6
        ,emSPIModeAddress_23_0 = 0x7
        ,emSPIModeAddress_32_0 = 0xF
    };

    // Option data enable
    enum eOpDataEnable {
        emOpDataDisable = 0
        ,emOpDataOneByte = 0x8
        ,emOpDataTwoBytes = 0xC
        ,emOpDataThreeBytes = 0xE
        ,emOpDataFourBytes = 0xF
    };

    // External read mode Address enable
    enum eExtReadModeAddressEnable {
        emExtReadAddressDisable = 0
        ,emExtReadAddressThreeBytes = 0x7
        ,emExtReadAddressFourBytes = 0xF
    };

    // Pay-load index
    enum ePayloadIndex {
        emCommandIndex = 0
        ,emOpCommandIndex = 1
        ,emAddressIndex = 2
        ,emOpDataIndex = 6
        ,emWriteDataIndex = 10
        ,emReadDataIndex = 14
    };

    // Variables
    sc_dt::uint64           mclkbPeriod;            // Period of clkb
    sc_dt::uint64           mclkbOrgFreq;           // Previous frequency of clkb
    sc_dt::uint64           mclkbFreq;              // Frequency of clkb
    std::string             mclkbFreqUnit;          // Frequency unit of clkb
    sc_dt::uint64           mSPBCLKPeriod;          // Period of SPBCLK

    sc_dt::uint64           mTimeResolutionValue;   // Time resolution value of simulation
    sc_time_unit            mTimeResolutionUnit;    // Time resolution unit of simulation

    bool                    mResetCurVal;           // Store current value of reset signals
    bool                    mResetPreVal;           // Store previous value of reset signals
    bool                    mIsResetHardActive;     // Is reset by port
    bool                    mIsResetCmdReceive;     // Is receive cmd reset
    bool                    mIsResetCmdActive;      // Is reset by cmd
    double                  mResetCmdPeriod;        // Period of cmd reset
    double                  mTimeCLKChangePoint;    //The time clock is changed its value
    unsigned char           mDataArray[emPayloadLen];
    bool                    mEnableRegisterMessage; // Enable/disable info/warning/error message of register IF

    // Events
    sc_event                mResetHardEvent;        // Call HandleResetHardMethod when reset is active
    sc_event                 mResetCmdEvent;         // Call HandleResetCmdMethod when AssertReset is called
    sc_event                 mResetCmdCancelEvent;   // Event to cancel reset by command
    sc_event                mClkZeroEvent;          // Notify clock is zero
    sc_event                mTransferEndEvent;      // Notify event to set TEND = 1 to indicate transfer end

    // Threads/Methods
    void HandleclkbSignalMethod(void);
    void HandleRES_SFMASignalMethod(void);
    void TransferEndMethod(void);

    // Reset handler
    void HandleResetHardMethod(void);
    void HandleResetCmdMethod(void);
    void CancelResetCmdMethod(void);

    // Functions
    void Initialize(void);
    void EnableReset(const bool is_active);

    void ExtReadHandleData(eDataField dataField, unsigned int address, unsigned int *pBusSize, unsigned int *pDatalen, unsigned int *pData);
    void SPIModeHandleData(eDataField dataField, unsigned int *pBusSize, unsigned int *pDatalen, unsigned int *pData);
    void CalculateSPBCLKPeriod(void);
    void CancelEvents(void);
    unsigned int GetBusSize(unsigned int regValue);

    // Overwrite functions of Agent Controller class
    bool CheckClockPeriod(void);
    bool GetResetStatus(void);
    unsigned int GetBitField(const std::string regName, const std::string bitName);
    void FlushCache(void);
    void NormalReadCommunicate(unsigned int address, unsigned char *pData, unsigned int size, BusTime_t* t);
    void BurstReadCommunicate(unsigned int address, unsigned char *pData, unsigned int size, BusTime_t* t);
    void SPIModeCommunicate(void);
    bool IsExtReadSettingProhibited(void);
    bool IsSPIModeSettingProhibited(void);
    void TransferEnd(void);
    void PrintMsg(const char* severity, const char* msg);
};
#endif //__SFMA_H__
