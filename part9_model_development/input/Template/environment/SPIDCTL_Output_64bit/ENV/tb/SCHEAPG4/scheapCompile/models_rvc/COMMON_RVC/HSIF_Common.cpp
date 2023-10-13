// ---------------------------------------------------------------------
// $Id: HSIF_Common.cpp,v 1.7 2020/04/17 04:43:03 chuonghoangle Exp $
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

#include "HSIF_Common.h"

L2Payload_t::L2Payload_t()
{//{{{
    for (int index = 0; index < emL2FrameSizeByte; index++){
        this->mPayloadUnit[index] = 0;
    }
}//}}}

L2Payload_t::L2Payload_t(uint32_t iPayload0,
                         uint32_t iPayload1,
                         uint32_t iPayload2,
                         uint32_t iPayload3,
                         uint32_t iPayload4,
                         uint32_t iPayload5,
                         uint32_t iPayload6,
                         uint32_t iPayload7)
{//{{{
    this->mPayloadUnit[0] = iPayload0;
    this->mPayloadUnit[1] = iPayload1;
    this->mPayloadUnit[2] = iPayload2;
    this->mPayloadUnit[3] = iPayload3;
    this->mPayloadUnit[4] = iPayload4;
    this->mPayloadUnit[5] = iPayload5;
    this->mPayloadUnit[6] = iPayload6;
    this->mPayloadUnit[7] = iPayload7;
}//}}}

void L2Payload_t::operator=(const L2Payload_t& rhs)
{//{{{
    for (uint8_t index = 0 ; index < emL2FrameSizeByte; index ++){
        this->mPayloadUnit[index] = rhs.mPayloadUnit[index];
    }
}//}}}

bool L2Payload_t::operator==(const L2Payload_t& rhs) const
{//{{{
    bool isEqual = true;
    for (uint8_t index = 0 ; index < emL2FrameSizeByte; index ++){
        if (this->mPayloadUnit[index] != rhs.mPayloadUnit[index]){
            isEqual = false;
            break;
        }
    }
    return isEqual;
}//}}}


std::ostream& operator<<(std::ostream& os,const L2Payload_t& iL2Payload)
{//{{{
    for (unsigned int index = 0; index < emL2FrameSizeByte; index ++){
        os << "PayLoad[" << index << "]: " << std::hex << iL2Payload.mPayloadUnit[index] <<"\n";
    }
    return os;
}//}}}

L2Frame::L2Frame()
{//{{{
    Initialize();
}//}}}

L2Frame::L2Frame(const L2Frame& iL2Frame)
{//{{{
    this->mL2CRC = iL2Frame.mL2CRC;
    this->mL2Header = iL2Frame.mL2Header;
    this->mL2Payload = iL2Frame.mL2Payload;
}//}}}

L2Frame::~L2Frame()
{//{{{
 ;
}//}}}
////Other Methods

//method for issue L1 frame from dummy
void L2Frame::SetL2Header(const uint16_t& iL2header)
{//{{{
    this->mL2Header = iL2header;
}//}}}

uint16_t L2Frame::GetL2Header(void) const
{//{{{
    return mL2Header;
}//}}}

void L2Frame::SetTransactionID(const uint8_t iTransId)
{//{{{
    mL2Header &= ~(0x7 << 13);
    mL2Header |= iTransId << 13;
}//}}}

uint8_t L2Frame::GetTransactionID(void) const
{//{{{
    return ((mL2Header >> 13) & 0x7);
}//}}}
void L2Frame::SetCommand(const uint8_t iCommand)
{//{{{
    mL2Header &= ~(0x1F << 8);
    mL2Header |= iCommand << 8;
}//}}}
uint8_t L2Frame::GetCommand(void) const
{//{{{
    return ((mL2Header >> 8) & 0x1F);
}//}}}

void L2Frame::SetChannelNumber(const uint8_t iChannelNum)
{//{{{
    mL2Header &= ~(0x7 << 1);
    mL2Header |= iChannelNum << 1;
}//}}}

uint8_t L2Frame::GetChannelNumber(void) const
{//{{{
    return ((mL2Header >> 1) & 0x7);
}//}}}

void L2Frame::SetPayloadData(const L2Payload_t& iL2Payload)
{//{{{
    mL2Payload = iL2Payload;
}//}}}

L2Payload_t L2Frame::GetPayloadData(void) const
{//{{{
    return mL2Payload;
}//}}}

void L2Frame::SetCRC(const uint16_t& iL2CRC)
{//{{{
    mL2CRC = iL2CRC;
}//}}}

uint16_t L2Frame::GetCRC(void) const
{//{{{
    return mL2CRC;
}//}}}

void L2Frame::Initialize(void)
{//{{{
    mL2Header   = 0;
    mL2CRC      = 0;
    for (int index = 0; index < emL2FrameSizeByte; index++){
        mL2Payload.mPayloadUnit[index] = 0;
    }
}//}}}

void L2Frame::operator=(const L2Frame &rhs)
{//{{{
    this->mL2CRC = rhs.mL2CRC;
    this->mL2Header = rhs.mL2Header;
    this->mL2Payload = rhs.mL2Payload;
}//}}}

bool L2Frame::operator ==(const L2Frame &rhs) const
{//{{{
    if ((this->mL2CRC == rhs.mL2CRC)
     &&( this->mL2Header == rhs.mL2Header)
     &&( this->mL2Payload == rhs.mL2Payload)){
        return true;
    } else {
        return false;
    }
}//}}}
/// add more trace function
void sc_trace(sc_core::sc_trace_file *tf, const L2Frame& v,
         const std::string& NAME) 
{//{{{
    std::ostringstream str_tmp;
    str_tmp.str("");
    str_tmp << "L2Frame.Header";
    sc_trace(tf, v.GetL2Header(), str_tmp.str().c_str());

    for (uint32_t index = 0; index < emL2FrameSizeByte; index ++){
        str_tmp.str("");
        str_tmp << "L2Frame.Payload[" << index << "]";
        sc_trace(tf, v.GetPayloadData().mPayloadUnit[index], str_tmp.str().c_str());
    }

    str_tmp.str("");
    str_tmp << "L2Frame.CRC";
    sc_trace(tf, v.GetCRC(), str_tmp.str().c_str());
}//}}}

void sc_trace(sc_core::sc_trace_file *tf, const L1Frame& v,
         const std::string& NAME) 
{//{{{
    std::ostringstream str_tmp;
    str_tmp.str("");
    str_tmp << "L1Frame.SyncCode";
    sc_trace(tf, v.GetSynCode(), str_tmp.str().c_str());

    str_tmp.str("");
    str_tmp << "L1Frame.L1Payload";
    sc_trace(tf, v.GetL1Payload(), str_tmp.str().c_str());

    L2Frame L2PayLoadTemp = v.GetL2PayLoadData();
    str_tmp.str("");
    str_tmp << "L1Frame.L2Payload";
    sc_trace(tf, L2PayLoadTemp, str_tmp.str().c_str());

    str_tmp.str("");
    str_tmp << "L1Frame.Endbit";
    sc_trace(tf, v.GetEndbit(), str_tmp.str().c_str());

    str_tmp.str("");
    str_tmp << "L1Frame.ErrorCode";
    sc_trace(tf, v.GetErrorCode(), str_tmp.str().c_str());
}//}}}

std::ostream& operator<<(std::ostream& os,const L2Frame& iL2Frame) 
{//{{{
    os << "L2 Frame Information." << "\n"
       << "TransID: " << std::hex <<(uint32_t)iL2Frame.GetTransactionID() << "\n"
       << "Command: " << std::hex <<(uint32_t)iL2Frame.GetCommand() << "\n"
       << "Channel Number: " << std::hex <<(uint32_t)iL2Frame.GetChannelNumber() << "\n"
       << iL2Frame.GetPayloadData()
       << "CRC: " << std::hex <<(uint32_t)iL2Frame.GetCRC();
    return os;
}//}}}

//L1 Frame interface//
L1Frame::L1Frame():mL2Frame()
{//{{{
    mSyncCode = 0;
    mL1Header = 0;
    mEndbit = 0;
    mL1Payload = 0;
    mErrorCode = 0;
}//}}}

L1Frame::L1Frame(const L1Frame& iL1Frame)
{//{{{
    this->mL2Frame   = iL1Frame.mL2Frame;
    this->mSyncCode  = iL1Frame.mSyncCode ;
    this->mL1Header  = iL1Frame.mL1Header  ;
    this->mEndbit    = iL1Frame.mEndbit    ;
    this->mL1Payload = iL1Frame.mL1Payload;
    this->mErrorCode = iL1Frame.mErrorCode;
}//}}}

L1Frame::~L1Frame()
{//{{{

}//}}}
void L1Frame::SetL2PayloadData(const L2Frame& iL2Frame)
{//{{{
    this->mL2Frame = iL2Frame;
}//}}}
void L1Frame::operator=(const L1Frame &dest_data)
{//{{{
    this->mSyncCode = dest_data.mSyncCode;
    this->mL1Header = dest_data.mL1Header;
    this->mL1Payload = dest_data.mL1Payload;
    this->mL2Frame = dest_data.mL2Frame;
    this->mEndbit = dest_data.mEndbit;
    this->mErrorCode = dest_data.mErrorCode;
}//}}}
bool L1Frame::operator ==(const L1Frame &indata) const
{//{{{
    if ((this->mSyncCode == indata.mSyncCode)
            && (this->mL1Header == indata.mL1Header)
                && (this->mL1Payload == indata.mL1Payload)
                    && (this->mL2Frame == indata.mL2Frame)
                            && (this->mEndbit == indata.mEndbit))
            {return true;}
    else {return false;}
}//}}}

uint8_t L1Frame::GetPayloadSize() const
{//{{{
    return ((mL1Header >> 5) & 7);
}//}}}

uint16_t L1Frame::GetL2Header()const
{//{{{
    return mL2Frame.GetL2Header();
}//}}}

L2Frame L1Frame::GetL2PayLoadData()const
{//{{{
    return mL2Frame;
}//}}}

void L1Frame::SetSynCode(const uint16_t Syncode)
{//{{{
    this->mSyncCode = Syncode;
}//}}}

uint16_t L1Frame::GetSynCode() const
{//{{{
    return mSyncCode;
}//}}}

void L1Frame::SetEndbit(bool Endbit)
{//{{{
    this->mEndbit = Endbit;
}//}}}

bool L1Frame::GetEndbit() const
{//{{{
    return mEndbit;
}//}}}

void L1Frame::SetL1header(const uint8_t L1header )
{//{{{
    this->mL1Header = L1header;
}//}}}

uint8_t L1Frame::GetL1header() const
{//{{{
    return mL1Header;
}//}}}

void L1Frame::SetL1Payload(const uint32_t L1payload)
{//{{{
    this->mL1Payload = L1payload;
}//}}}

uint32_t L1Frame::GetL1Payload() const
{//{{{
    return mL1Payload;
}//}}}

std::ostream& operator<<(std::ostream& os,const L1Frame& iL1Frame)
{//{{{
    os  << "L1 Frame infor: " << endl
            << "Syncode: " << std::hex << iL1Frame.GetSynCode() << endl
            <<"L1 header: " << std::hex << (unsigned int)iL1Frame.GetL1header() << endl
            <<"L2 header: "<<std::hex<<(unsigned int)iL1Frame.GetL2Header()<<endl
            <<"L1 Payload: " << std::hex << iL1Frame.GetL1Payload() << endl
            <<"L2 Frame: " << std:: hex << iL1Frame.GetL2PayLoadData() << endl
            <<"end bit : " <<std::hex<<iL1Frame.GetEndbit()<<endl
            <<"Error code : " <<std::hex<<iL1Frame.GetErrorCode()<<endl
            ;
    return os;
}//}}}

void L1Frame::SetErrorCode(const unsigned int ErrorCode)
{//{{{
    this->mErrorCode = ErrorCode;
}//}}}
uint32_t L1Frame::GetErrorCode() const
{//{{{
    return mErrorCode;
}//}}}

AXITransInfo::AXITransInfo(bool idebug_mode, uint8_t icmd
                         , uint8_t isize, uint32_t iaddr
                         , ChId_t iChId, uint32_t iAXITransId)
{//{{{
    mDebugMode  = idebug_mode;
    mCmd        = icmd;
    mSize       = isize;
    mAddr       = iaddr;
    mChId       = iChId;
    mAXITransId = iAXITransId;
}//}}}

AXITransInfo::~AXITransInfo()
{//{{{
}//}}}
// vim600: set foldmethod=marker :