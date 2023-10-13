// ---------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2012 Renesas Electronics Corporation
// Copyright(c) 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __CSCIIF_H__
#define __CSCIIF_H__
#include <systemc.h>
#include <sstream>
struct CSciIF
{
private:
    bool StartBit;
    unsigned int Data;
    bool ParityBit;
    unsigned char StopBit;
    bool Te;
    bool Dir;
    unsigned int Size;
    unsigned int Bitt;
    bool Break;
    unsigned int Seq;

    void CheckBreak(void);

public:
    void SetStartBit(const bool startbit);
    bool GetStartBit(void) const;
    void SetData(const unsigned int data);
    unsigned int GetData(void) const;
    void SetParityBit(const bool paritybit);
    bool GetParityBit(void) const;
    void SetStopBit(const unsigned char stopbit);
    unsigned char GetStopBit(void) const;
    void SetTe(const bool transmitter_enable);
    bool GetTe(void) const;
    void SetDir(const bool data_direction);
    bool GetDir(void) const;
    void SetSize(const unsigned int data_size);
    unsigned int GetSize(void) const;
    void SetBitt(const unsigned int bit_time);
    unsigned int GetBitt(void) const;
    void SetBreak(const bool break_enable);
    bool GetBreak(void) const;
    void SetSeq(const unsigned int seq);
    unsigned int GetSeq(void) const;
    void IncSeq(void);
    void Initialize(void);

    void operator=(const CSciIF &dest_data);
    bool operator ==(const CSciIF &indata) const;
    //Construct and Destruct
    CSciIF();
    ~CSciIF();
   
};

inline void sc_trace(sc_core::sc_trace_file*& tf, const CSciIF& data, std::string& fn)
{//{{{
    std::ostringstream str_tmp;
    str_tmp.str("");
    str_tmp << "start_bit";
    sc_trace(tf, data.GetStartBit(), str_tmp.str().c_str());

    str_tmp.str("");
    str_tmp << "data";
    sc_trace(tf, data.GetData(), str_tmp.str().c_str());

    str_tmp.str("");
    str_tmp << "parity_bit";
    sc_trace(tf, data.GetParityBit(), str_tmp.str().c_str());

    str_tmp.str("");
    str_tmp << "stop_bit";
    sc_trace(tf, data.GetStopBit(), str_tmp.str().c_str());

    str_tmp.str("");
    str_tmp << "te";
    sc_trace(tf, data.GetTe(), str_tmp.str().c_str());

    str_tmp.str("");
    str_tmp << "dir";
    sc_trace(tf, data.GetDir(), str_tmp.str().c_str());

    str_tmp.str("");
    str_tmp << "size";
    sc_trace(tf, data.GetSize(), str_tmp.str().c_str());

    str_tmp.str("");
    str_tmp << "bitt";
    sc_trace(tf, data.GetBitt(), str_tmp.str().c_str());

    str_tmp.str("");
    str_tmp << "break";
    sc_trace(tf, data.GetBreak(), str_tmp.str().c_str());

    str_tmp.str("");
    str_tmp << "seq";
    sc_trace(tf, data.GetSeq(), str_tmp.str().c_str());

}//}}}

inline void sc_trace(sc_core::sc_trace_file*& tf, const CSciIF& data, const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& alt)
{//{{{
}//}}}

std::ostream& operator <<(std::ostream&,const CSciIF&);

#endif //__CSCIIF_H__                                                      
