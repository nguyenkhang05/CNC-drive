// ---------------------------------------------------------------------
// $Id: HSIF_Common.h,v 1.9 2020/04/17 04:42:48 chuonghoangle Exp $
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
#ifndef __HSIF_COMMON_H__
#define __HSIF_COMMON_H__

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <cstring>
#include <stdint.h>
#include <cstdarg>
#include "systemc.h"
#include "rvc_common_model.h"

/// Define some contraint setting of HSIF model
/// RVC-Chan confirmed
#define HSIF0MASTERID               19
#define ICUMMASTERID                26
#define HSIF020_ST_DATA_BUF_SIZE    64          //512 bits
#define HSIF020_STMD_STBL           0x1         //3'b001
#define HSIF020_STMD_STTH           0x1         //3'b001
#define HSIF020_STMD_STOS           0x1         //3'b001
#define HSIF020_SRMD_SROS           0x1         //1'b1

class L1Frame;
class L2Frame;
class AXITransInfo;

typedef uint8_t                      ChId_t;
typedef uint8_t                      FrameId_t;
typedef uint32_t                     TransId_t;

typedef std::pair<ChId_t,AXITransInfo*>                      TransInfo_t;

struct LessThanCompare_TransInfo_t{
    bool operator()(const TransInfo_t& lhs, const TransInfo_t& rhs){
        bool isLessThan = false;
        if (lhs.first < rhs.first) isLessThan = true;
        return isLessThan;
    }
};

typedef std::set<TransInfo_t,LessThanCompare_TransInfo_t>   TransIdInfoSet_t;
typedef std::map<TransId_t, std::pair<ChId_t, FrameId_t> >   TransIdInfoMap_t;

typedef uint32_t    Addrress_t;
typedef bool        WriteEnable_t;
typedef bool        ReadEnable_t;

typedef std::pair<Addrress_t, Addrress_t>                   AddressRange_t;
typedef std::pair<WriteEnable_t, ReadEnable_t>              AccessInfo_t;
typedef std::pair<AddressRange_t, AccessInfo_t>             MemoryWindow_t;
typedef std::string                                         WindowLabel_t;
typedef std::set<Addrress_t>                                RegsAuthenAddrSet_t;

enum emCRC{
    emInitCRCExtraVal = 0xFFFF,
    emCRCExtraPolynomial = 0x1021           //x16 + x12 + x5 + 1
};

enum eHSIFDefine {
    emL2FrameSizeByte = 8
};

enum eAuthInfo{
    emAuthCRRegNumbs = 0x4
   ,emAuthIDbaseRegNumbs = 0x8
   ,emAuthIdTimeMax = 0x3
   ,emAuthCRTimeMax = 0x3
};

enum eMemWindowNumbs{
    emMemWindowNumbs = 0x4
};

enum eAddrMask{
    emHSIF020_AddrMask = 0x7FF
};

enum eChId {    // Define model's constant number
         emCH0
        ,emCH1
        ,emCH2
        ,emCH3
        ,emCHNum = 4
};
enum eResetActiveLevel {    // Define enum for reset control
        emResetActive = 0,
        emARESETn = 0,
        emcresetn = 1,
        emResetSignalNum = 2
    };
enum eClockIndex{    // Define enum for clock control
        emAclk= 0,
        emCclk= 1,
        emClkNum = 2
    };
enum eSTBEState{
        emIdle = 0x0,
        emFirstByte = 0x1,
        emLastByte = 0x2,
        emAborted = 0x3
};

enum eReadWriteSize{
        emRW8bits       = 0x1,
        emRW16bits      = 0x2,
        emRW32bits      = 0x4
};

enum eChannelCtrl{
        emReadCmd8   = 0x0,
        emReadCmd16  = 0x1,
        emReadCmd32  = 0x2,
        emWriteCmd8  = 0x4,
        emWriteCmd16 = 0x5,
        emWriteCmd32 = 0x6,
        emEventCmd   = 0xC,
        emIdCmd      = 0x12,
        emACKCmd         = 0x8,
        emNACKCmd        = 0x9,
        emReadAnswerCmd  = 0xA,
        emStreamCmd      = 0x17
};

enum eIntStatusReg{
        emRARS0 = 0x0,
        emAKRS0 = 0x1,
        emTERS0 = 0x2,
        emRARS1 = 0x4,
        emAKRS1 = 0x5,
        emTERS1 = 0x6,
        emRARS2 = 0x8,
        emAKRS2 = 0x9,
        emTERS2 = 0xA,
        emRARS3 = 0xC,
        emAKRS3 = 0xD,
        emTERS3 = 0xE,
        emSTCS  = 0x10,
        emSRCS0 = 0x11,
        emSRCS1 = 0x12,
        emCERS0 = 0x14,
        emCERS1 = 0x15,
        emCERS2 = 0x16,
        emCERS3 = 0x17,
        emSTES  = 0x18,
        emSRES  = 0x19,
        emMESS  = 0x1B,
        emAESS  = 0x1C
};

enum eStreamTxStatus{
        emSTC = 0x0,
        emSTE0 = 0x2,
        emSTE1 = 0x4,
        emSTE2 = 0x5,
        emSTE3 = 0x14,
        emSTE4 = 0x17
};

enum eStreamTxClearStatus{
        emSTCC = 0x0,
        emSTEC0 = 0x2,
        emSTEC1 = 0x4,
        emSTEC2 = 0x5,
        emSTEC3 = 0x14,
        emSTEC4 = 0x17
};

enum eStreamTxIntEnable{
        emSTCE  = 0x0,
        emSTEE0 = 0x2,
        emSTEE1 = 0x4,
        emSTEE2 = 0x5,
        emSTEE3 = 0x14,
        emSTEE4 = 0x17
};

enum eStreamRxStatus{
        emSRC0 = 0x0,
        emSRC1 = 0x1,
        emSRTA = 0xF,
        emSRE0 = 0x14,
        emSRE1 = 0x15,
        emSRE2 = 0x17,
        emSRA  = 0x1F
};

enum eStreamRxClearStatus{
        emSRCC0 = 0x0,
        emSRCC1 = 0x1,
        emSRTCA = 0xF,
        emSREC0 = 0x14,
        emSREC1 = 0x15,
        emSREC2 = 0x17,
        emSRAC  = 0x1F
};

enum eStreamRxIntEnable{
        emSRCE0 = 0x0,
        emSRCE1 = 0x1,
        emSRTEA = 0xF,
        emSREE0 = 0x14,
        emSREE1 = 0x15,
        emSREE2 = 0x17,
        emSRAE  = 0x1F
};

enum eAuthentErrStatus{
        emAEE0 =  0,
        emAEI0 =  1,
        emAER0 =  2,
        emAEW0 =  3,
        emAEE1 =  4,
        emAEI1 =  5,
        emAER1 =  6,
        emAEW1 =  7,
        emAEE2 =  8,
        emAEI2 =  9,
        emAER2 =  10,
        emAEW2 =  11,
        emAEE3 =  12,
        emAEI3 =  13,
        emAER3 =  14,
        emAEW3 =  15,
        emAES  =  16,
        emWER0 =  20,
        emWEW0 =  21,
        emWER1 =  22,
        emWEW1 =  23,
        emWER2 =  24,
        emWEW2 =  25,
        emWER3 =  26,
        emWEW3 =  27,
        emWES0 =  28,
        emWES1 =  29
};

enum eAuthentErrClearStatus{
        emAEEC0 =  0,
        emAEIC0 =  1,
        emAERC0 =  2,
        emAEWC0 =  3,
        emAEEC1 =  4,
        emAEIC1 =  5,
        emAERC1 =  6,
        emAEWC1 =  7,
        emAEEC2 =  8,
        emAEIC2 =  9,
        emAERC2 =  10,
        emAEWC2 =  11,
        emAEEC3 =  12,
        emAEIC3 =  13,
        emAERC3 =  14,
        emAEWC3 =  15,
        emAESC  =  16,
        emWERC0 =  20,
        emWEWC0 =  21,
        emWERC1 =  22,
        emWEWC1 =  23,
        emWERC2 =  24,
        emWEWC2 =  25,
        emWERC3 =  26,
        emWEWC3 =  27,
        emWESC0 =  28,
        emWESC1 =  29
};

enum eAuthentErrIntEnable{
        emAEEE0 =  0,
        emAEIE0 =  1,
        emAERE0 =  2,
        emAEWE0 =  3,
        emAEEE1 =  4,
        emAEIE1 =  5,
        emAERE1 =  6,
        emAEWE1 =  7,
        emAEEE2 =  8,
        emAEIE2 =  9,
        emAERE2 =  10,
        emAEWE2 =  11,
        emAEEE3 =  12,
        emAEIE3 =  13,
        emAERE3 =  14,
        emAEWE3 =  15,
        emAESE  =  16,
        emWERE0 =  20,
        emWEWE0 =  21,
        emWERE1 =  22,
        emWEWE1 =  23,
        emWERE2 =  24,
        emWEWE2 =  25,
        emWERE3 =  26,
        emWEWE3 =  27,
        emWESE0 =  28,
        emWESE1 =  29
};

enum eChStatus{
        emRAR = 0 ,
        emAKR = 1 ,
        emAKE = 2 ,
        emTOE = 4 ,
        emIDE = 5 ,
        emAOE = 7 ,
        emRDY = 15,
        emTER = 16,
        emBRE = 20,
        emCRE = 31
};

enum eChClearStatus{
        emRARC = 0 ,
        emAKRC = 1 ,
        emAKEC = 2 ,
        emTOEC = 4 ,
        emIDEC = 5 ,
        emAOEC = 7 ,
        emTERC = 16,
        emBREC = 20,
        emCREC = 31
};

enum eChIntEnable{
        emRARE = 0 ,
        emAKRE = 1 ,
        emAKEE = 2 ,
        emTOEE = 4 ,
        emIDEE = 5 ,
        emAOEE = 7 ,
        emTERE = 16,
        emBREE = 20,
        emCREE = 31
};

enum ePayloadSizeIndex{
        emPayLoadSize8   = 0x0,
        emPayLoadSize32  = 0x1,
        emPayLoadSize64  = 0x2,
        emPayLoadSize96  = 0x3,
        emPayLoadSize160 = 0x6,
        emPayLoadSize288 = 0x7,
};

enum eStreamChannel{
        emStreamChannel = 0x2
};

enum eOutStandingReqMax{
        emOutStandingReqMax = 0x2,
        emRxReceivedMaxCmd  = 0x2
};

struct L2Payload_t {
    uint32_t    mPayloadUnit[emL2FrameSizeByte];    //8 * 32 = 256

    L2Payload_t();
    L2Payload_t(uint32_t iPayload0,
                uint32_t iPayload1,
                uint32_t iPayload2,
                uint32_t iPayload3,
                uint32_t iPayload4,
                uint32_t iPayload5,
                uint32_t iPayload6,
                uint32_t iPayload7);
    void operator=(const L2Payload_t& rhs);
    bool operator==(const L2Payload_t& rhs) const;
    friend std::ostream& operator<<(std::ostream& os,const L2Payload_t& iL2Payload);
};

class L2Frame {
private:
    uint16_t            mL2Header;
    L2Payload_t         mL2Payload;
    uint16_t            mL2CRC;
public:
    //Constructor and Destructor
    L2Frame();
    L2Frame(const L2Frame& iL2Frame);
    ~L2Frame();
    //Other Methods
    uint16_t GetL2Header(void) const;
    void SetL2Header(const uint16_t& iL2header);
    void SetTransactionID(const uint8_t iTransId);
    uint8_t GetTransactionID(void) const;
    void SetCommand(const uint8_t iCommand);
    uint8_t GetCommand(void) const;
    void SetChannelNumber(const uint8_t iChannelNum);
    uint8_t GetChannelNumber(void) const;
    void SetPayloadData(const L2Payload_t& iL2Payload);
    L2Payload_t GetPayloadData(void) const;
    void SetCRC(const uint16_t& iL2CRC);
    uint16_t GetCRC(void) const;
    void Initialize(void);

    bool operator==(const L2Frame &rhs) const;
    void operator=(const L2Frame &rhs);
    inline operator const char*() const {
        return *this;
    }
    ///For debug style
    friend std::ostream& operator <<(std::ostream& os,const L2Frame& iL2Frame);
    friend void sc_trace(sc_core::sc_trace_file *tf, const L2Frame& v,
                        const std::string& NAME);
};

class L1Frame {
private:
    uint16_t        mSyncCode;
    uint8_t         mL1Header;
    uint32_t        mL1Payload;
    L2Frame         mL2Frame;
    bool            mEndbit;
    uint32_t         mErrorCode;
public:
    ///Constructor and Destructor
    L1Frame();
    L1Frame(const L1Frame& iL1Frame);
    ~L1Frame();
    ///Other Methods
    uint8_t GetPayloadSize(void) const;
    uint16_t GetL2Header() const;
    void SetL2PayloadData(const L2Frame& iL2Frame);
    L2Frame GetL2PayLoadData() const;

    //method for issue dummy l1 Frame
    void SetL1header(const uint8_t L1header);
    uint8_t    GetL1header() const;
    void SetSynCode (const uint16_t Syncode);
    uint16_t GetSynCode() const;
    void SetEndbit(bool Endbit);
    bool GetEndbit() const;
    void SetL1Payload(const uint32_t L1Payload);
    uint32_t GetL1Payload() const;
    void SetErrorCode(const unsigned int ErrorCode);
    uint32_t GetErrorCode() const;

    bool operator ==(const L1Frame &indata) const;
    void operator =(const L1Frame &dest_data);
    inline operator const char*() const {
        return *this;
    }
    ///For debug style
    friend std::ostream& operator <<(std::ostream& os,const L1Frame& iL1Frame);
    friend void sc_trace(sc_core::sc_trace_file *tf, const L1Frame& v,
                         const std::string& NAME);
};

class AXITransInfo{
public:
    bool            mDebugMode;
    uint8_t         mCmd;
    uint8_t         mSize;
    uint32_t        mAddr;
    ChId_t          mChId;
    uint32_t        mAXITransId;

    AXITransInfo(bool idebug_mode, uint8_t icmd
               , uint8_t isize, uint32_t iaddr
               , ChId_t iChId, uint32_t iAXITransId);
    ~AXITransInfo();
};

class HSIF_ultis {
private:
    std::string mModuleName;
public:
    HSIF_ultis(const std::string iname){
        mModuleName = iname;
    }
public:
    virtual ~HSIF_ultis(void) {;}
    std::string CombineStringUint(std::string name, unsigned int index)
    {//{{{
        std::ostringstream ret_string;
        ret_string.str("");
        ret_string << name << index;
        return ret_string.str();
    }//}}}
};
#endif //__HSIF_COMMON_H__
