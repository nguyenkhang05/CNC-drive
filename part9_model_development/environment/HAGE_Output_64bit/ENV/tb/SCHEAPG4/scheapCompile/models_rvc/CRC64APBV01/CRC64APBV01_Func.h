// ---------------------------------------------------------------------
// $Id: CRC64APBV01_Func.h,v 1.9 2019/12/17 01:50:15 huepham Exp $
//
// Copyright(c) 2018 - 2019 Renesas Electronics Corporation
// Copyright(c) 2018 - 2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __CRC64APBV01_FUNC_H__
#define __CRC64APBV01_FUNC_H__

#include <bitset>
#include <string>
#include <map>
#include "global.h"
#include "BusSlaveFuncIf.h"
#include "crc64apbv01_regif.h"
#include "CRC64APBV01_AgentController.h"

using namespace std;

class CRC64APBV01_Func: public Ccrc64apbv01_regif
                ,public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (CRC64APBV01_Func);
    CRC64APBV01_Func(std::string name, CRC64APBV01_AgentController* CRC64APBV01AgentController);
    ~CRC64APBV01_Func();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    void DumpStatusInfo(void);
    unsigned long long getCRCPOLYReg();

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    // enum
    enum ePolyVal { // Polynomial value
        em0x2F             = 0x000000000000002F      
        ,emSAE_J1850       = 0x000000000000001D      
        ,emCCITT16         = 0x0000000000001021      
        ,emCRC32           = 0x0000000004C11DB7      
        ,emCRC32RevPoly    = 0x00000000EDB88320
        ,emCRC32C          = 0x000000001EDC6F41      
        ,emBaicheva00      = 0x00000000000090D9                             
        ,emCRC32P4         = 0x00000000F4ACFB13      
        ,emARC             = 0x0000000000008005      
        ,emCRC15CAN        = 0x0000000000004599
        ,emCRC64ECMA1      = 0xA9EA3693
        ,emCRC64ECMA2      = 0x42F0E1EB
    };

    // Variables
    CRC64APBV01_AgentController   *mpCRC64APBV01AgentController;    // CRC64APBV01 Agent controller pointer
    std::string     mModuleName;                // Module name
    bitset<64>      mPoly;
    bitset<64>      mXorReg;
    unsigned int    mPsize;
    unsigned int    mCmd0;
    unsigned int    mCmd1;
    unsigned int    mCmd2;
    bitset<64>      mCrcResult;
    bool            misDIN1st;

    std::map<unsigned long long, std::string> mPolyMap; // Polynomial value, polynomial name
    std::map<std::string, unsigned long long> mXorMap;  // Polynomial name, XOR value
    std::map<std::string, unsigned long long> mInitMap; // Polynomial name, CRCDOUT initial value
    std::map<std::string, unsigned int> mCtrlMap;       // Polynomial name, CRCCTL value

    // Function
    void Initialize();
    bitset<64> genKCRC(bitset<64> poly, bitset<32> din, bitset<64> init_pio, unsigned int crcctl, bitset<64> xor_reg, bitset<64> &crc_result);
    void CreateMap(void);

    // Callback
    void cb_CRCDIN_DIN(RegCBstr str);
    void cb_CRCCTL_PSIZE(RegCBstr str);
    void cb_CRCCTL_DW(RegCBstr str);
    void cb_CRCDOUT0_DOUT(RegCBstr str);
    void cb_CRCDOUT1_DOUT(RegCBstr str);

};
#endif //__CRC64APBV01_FUNC_H__
