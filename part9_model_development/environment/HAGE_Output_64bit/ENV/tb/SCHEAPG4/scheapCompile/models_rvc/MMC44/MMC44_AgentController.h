// ---------------------------------------------------------------------
// $Id: MMC44_AgentController.h,v 1.2 2020/03/19 08:33:32 anhnguyen4 Exp $
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

#ifndef __MMC44_AGENT_CONTROLLER_H__
#define __MMC44_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>
/// utility libraries
#include <stdint.h>
#include <string>
using std::string;
#define CLOG(fstream1, fstream2) {cout << "[DEBUG][" << sc_time_stamp().to_string() << "]   " << __FILE__ \
                           << ": " << __FUNCTION__ << ": " << __LINE__ << ": "                \
                           << (fstream1) << (fstream2) << endl;}

enum eTimeSwitchCLKBoot{
    emTimeSwitchToCLKBoot = 74,
    emTimeSwitchToCLKInCMDReg = 48
};

enum eCRC{
    emInitCRC16Val = 0x0000,
    emCRC16Polynomial = 0x1021, //x16 + x12 + x5 + 1
    emInitCRC7Val = 0x00,
    emCRC7Polynomial = 0x09 //x7 + x3 + 1
};

enum eIntEnableBitName{
    emMCMD12DRE, // Automatic CMD12 Issuance & Buffer Read Complete
    emMCMD12RBE, // Automatic CMD12 Issuance Response Busy Complete
    emMCMD12CRE, // Automatic CMD12 Response Complete
    emMDTRANE,   // Data Transmission Complete
    emMBUFRE,    // Buffer Read Complete
    emMBUFWEN,   // Buffer Write Ready
    emMBUFREN1,  // Buffer1 Read Ready
    emMBUFREN2,  // Buffer2 Read Ready
    emMRBSYE,    // Response Busy Complete
    emMCRSPE,    // command/ response completed
    emMCMDVIO,   // Command Issuance Error
    emMBUFVIO,   // Buffer Access Error
    emMWDATERR,  // Write Data Error
    emMRDATERR,  // Read Data Error
    emMRIDXERR,  // Response Index Error
    emMRSPERR,   // Response Error
    emMCRCSTO,   // CRC Status Timeout
    emMWDATTO,   // Write Data Timeout
    emMRDATTO,   // Read Data Timeout
    emMRBSYTO,   // Response Busy Timeout
    emMRSPTO     // Response Timeout
};

enum eLineState{
    emActive,
    emActiveLine0,
    emDeActive
};

enum eSTBE{
    emIdle = 0x0,
    emStart = 0x1,
    emStop = 0x2
};

enum eCommandLineSignal{
    emDefaultCommandInfo = 0,
    emCommandInfo = 0x1,
    emResponseInfo = 0x2,
    emKeepLowInfo = 0x3
};

enum eTimeOutAndErrSttBits{
    emCRCSTE_H,
    emCRC16E,
    emAC12CRCE,
    emRSPCRC7E,
    emCRCSTEBE,
    emRDATEBE,
    emAC12REBE,
    emRSPEBE,
    emAC12IDXE,
    emRSPIDXE,
    emBTACKPATE,
    emBTACKEBE,
    emCRCST,
    emSTRDATTO,
    emDATBSYTO,
    emCRCSTTO,
    emAC12BSYTO,
    emRSPBSYTO,
    emAC12RSPTO,
    emSTRSPTO,
    emBTACKTO,
    em1STBTDATTO,
    emBTDATTO
};

enum eBootOpBits{
    emBTCLKDIV,
    emSBTACKTO,
    emS1STBTDATTO,
    emSBTDATTO
};

enum eClockControlBit{
    emMMCBUSBSY,
    emCLKEN,
    emCLKDIV,
    emSRSPTO,
    emSRBSYTO,
    emSRWDTO
};

enum eTimeoutTypes{
    emWriteDataTimeout,
    emReadDataTimeout,
    emResponseBusyTimeout,
    emResponseTimeout
};

enum eDataLineSignal{
    emDefaultDataSignal,
    emDATS = 0x4,
    emDATR = 0x5,
    emCRCR = 0x6,
    emBUSYR = 0x7,
    emBAR = 0x8,
};

enum eCommandSetting{
    emBOOT,
    emCMD,
    emRTYP,
    emRBSY,
    emWDAT,
    emDWEN,
    emCMLTE, // Single/Multi Block Transfer Select
    emCMD12EN,
    emRIDXC, // command index
    emRCRC7C,
    emCRC16C,
    emBOOTACK,
    emCRCSTE,
    emTBIT,
    emDATW
};

enum eLength{
    emBARLength = 1,
    emCRCRLength = 1,
    emCommandSize = 6,
    emStartBitLength = 1,
    emStatusBitLength = 1,
    emCRC16Length = 2,
    emEndBitLength = 1,
    emDataRegLength = 4,
    emSizeOfTimeOutAndErrorSttRegister = 23
};

enum eBufferNumber{
    emBuffer1 = 1,
    emBuffer2 = 2
};

enum eBufferState{
    emWaiting,
    emSending,
    emWritting,
    emReading,
    emCanRead,
    emCanWrite
};

enum eMMC44Status{
    emCMDSEQ,
    emCMDSIG,
    emRSPIDX,
    emDATSIG,
    emRCVBLK
};

enum eBlockSet{
    emBLKCNT,
    emBLKSZ
};

enum eINTBitName{
    emCMD12DRE, // Automatic CMD12 Issuance & Buffer Read Complete
    emCMD12RBE, // Automatic CMD12 Issuance Response Busy Complete
    emCMD12CRE, // Automatic CMD12 Response Complete
    emDTRANE,   // Data Transmission Complete
    emBUFRE,    // Buffer Read Complete
    emBUFWEN,   // Buffer Write Ready
    emBUFREN1,  // Buffer1 Read Ready
    emBUFREN2,  // Buffer2 Read Ready
    emRBSYE,    // Response Busy Complete
    emCRSPE,    // command/ response completed
    emCMDVIO,   // Command Issuance Error
    emBUFVIO,   // Buffer Access Error
    emWDATERR,  // Write Data Error
    emRDATERR,  // Read Data Error
    emRIDXERR,  // Response Index Error
    emRSPERR,   // Response Error
    emCRCSTO,   // CRC Status Timeout
    emWDATTO,   // Write Data Timeout
    emRDATTO,   // Read Data Timeout
    emRBSYTO,   // Response Busy Timeout
    emRSPTO     // Response Timeout
};

enum eREG_BUF_ACC{
    emDMATYP = 26,
    emDMAWEN = 25,
    emDMAREN = 24,
    emATYP = 16
};

class MMC44_AgentController {
#include "MMC44_cmdif.h"
public:
    // Constructor/Destructor
    MMC44_AgentController(void) {;}
    virtual ~MMC44_AgentController(void) {;}
    virtual bool CheckClockPeriod(const std::string clock_name) = 0;
    virtual bool GetResetStatus(void) = 0;
    //--------------------------------------------------------------------------
    // Interface (APIs)
    // Defined API to comunicate from C++ to SystemC
    //--------------------------------------------------------------------------
    virtual void ErrorOccuredDuringDMATransfer() = 0;
    virtual void AssertDMAEvent() = 0;
    virtual void AssertOneBlockHadWrittenEvent()= 0;
    virtual void AssertOneBlockHadReadEvent() = 0;
    virtual void AssertStartSendCommandEvent() = 0;
    virtual void AssertSetCLKEvent() = 0;
    virtual void AssertSetBootCLKEvent() = 0;
    virtual void AssertSetTimeoutAndErrStt(unsigned int index) = 0;
    virtual void AssertIntBUFWENSetEvent() = 0;
    virtual void AssertBuffer1ReadEnableEvent() = 0;
    virtual void AssertBuffer2ReadEnableEvent() = 0;
    virtual void AssertReadDataFromBufferProcess() = 0;
    virtual void AssertWriteDataToBufferProcess() = 0;
    virtual void AssertInterrupt() = 0;
    virtual void EnableSoftwareReset(const bool isReset) = 0;
    virtual void DeAssertInterrupt() = 0;

};
#endif //__MMC44_AGENT_CONTROLLER_H__
