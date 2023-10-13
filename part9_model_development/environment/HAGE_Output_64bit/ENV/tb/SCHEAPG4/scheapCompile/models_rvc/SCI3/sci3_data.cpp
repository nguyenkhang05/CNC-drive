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

#include <iostream>
#include <sstream>
#include "sci3_data.h"

//===============CSciIF class=======================
//Constructor of CSciIF class
CSciIF::CSciIF()  
{//{{{
    Initialize();
}//}}}

void CSciIF::Initialize(void)
{//{{{
    StartBit  = true;
    Data      = 0xFFFF;
    ParityBit = true;
    StopBit   = 0x3;
    Te        = false;
    Dir       = false;
    Size      = 0x8;
    Bitt      = 0x0;
    Break     = false;
    Seq       = 0x0;
}//}}}

//Destructor of CSciIF class
CSciIF::~CSciIF()
{//{{{
}//}}} 

void CSciIF::SetStartBit (const bool startbit)
{//{{{
    this->StartBit = startbit;  
    CheckBreak();
}//}}}

bool CSciIF::GetStartBit(void) const
{//{{{
    return this->StartBit;
}//}}}

void CSciIF::SetData(const unsigned int data)
{//{{{
    this->Data = data;
    CheckBreak();
}//}}} 

unsigned int CSciIF::GetData(void) const
{//{{{
    return this->Data;
}//}}} 

void CSciIF::SetParityBit (const bool paritybit)
{//{{{
    this->ParityBit = paritybit;  
    CheckBreak();
}//}}}

bool CSciIF::GetParityBit(void) const
{//{{{
    return this->ParityBit;
}//}}}

void CSciIF::SetStopBit (const unsigned char stopbit)
{//{{{
    this->StopBit = stopbit&0x3;  
    CheckBreak();
}//}}}

unsigned char CSciIF::GetStopBit(void) const
{//{{{
    return this->StopBit;
}//}}}

void CSciIF::SetTe (const bool transmitter_enable)
{//{{{
    this->Te = transmitter_enable;  
}//}}}

bool CSciIF::GetTe(void) const
{//{{{
    return this->Te;
}//}}}

void CSciIF::SetDir (const bool data_direction)
{//{{{
    this->Dir = data_direction;   
}//}}}

bool CSciIF::GetDir(void) const
{//{{{
    return this->Dir;
}//}}}

void CSciIF::SetSize (const unsigned int data_size)
{//{{{
    this->Size = data_size;   
}//}}}

unsigned int CSciIF::GetSize(void) const
{//{{{
    return this->Size;
}//}}}

void CSciIF::SetBitt (const unsigned int bit_time)
{//{{{
    this->Bitt = bit_time&0xFFFF;   
}//}}}

unsigned int CSciIF::GetBitt(void) const
{//{{{
    return this->Bitt;
}//}}}

void CSciIF::SetBreak(const bool break_enable)
{//{{{
    if (break_enable) {
        this->StartBit  = false;
        this->Data      = 0;
        this->ParityBit = false;
        this->StopBit   = 0;
    }
    this->Break = break_enable;
}//}}} 

bool CSciIF::GetBreak(void) const
{//{{{
    return this->Break;
}//}}} 

void CSciIF::CheckBreak(void)
{//{{{
    if ((!(this->StartBit))&&(this->Data == 0)&&(!(this->ParityBit))&&(this->StopBit == 0)) {
        this->Break = true;
    } else {
        this->Break = false;
    }
}//}}} 

void CSciIF::SetSeq(const unsigned int seq)
{//{{{
    this->Seq = seq;
}//}}} 

unsigned int CSciIF::GetSeq(void) const
{//{{{
    return this->Seq;
}//}}} 

void CSciIF::IncSeq(void)
{//{{{
    this->Seq++ ;
}//}}} 

void CSciIF::operator=(const CSciIF &dest_data)
{//{{{
    this->StartBit  = dest_data.GetStartBit();
    this->Data      = dest_data.GetData();
    this->ParityBit = dest_data.GetParityBit();
    this->StopBit   = dest_data.GetStopBit();
    this->Te        = dest_data.GetTe();     
    this->Dir       = dest_data.GetDir();
    this->Size      = dest_data.GetSize();
    this->Bitt      = dest_data.GetBitt();
    this->Break     = dest_data.GetBreak();
    this->Seq       = dest_data.GetSeq();
}//}}} 

bool CSciIF::operator ==(const CSciIF &indata) const
{//{{{
    bool is_same = true;
    if(this->StartBit != indata.StartBit) {
        is_same = false;
    }else if(this->Data != indata.Data) {
        is_same = false;
    }else if(this->ParityBit != indata.ParityBit) {
        is_same = false;
    }else if(this->StopBit != indata.StopBit) {
        is_same = false;
    }else if(this->Te   != indata.Te  ) {
        is_same = false;
    }else if(this->Dir  != indata.Dir  ) {
        is_same = false;
    }else if(this->Size != indata.Size ) {
        is_same = false;
    }else if(this->Bitt != indata.Bitt ) {
        is_same = false;
    }else if(this->Break!= indata.Break) {
        is_same = false;
    }else if(this->Seq  != indata.Seq  ) {
        is_same = false;
    }
    return(is_same);
}//}}}

std::ostream& operator <<(std::ostream& os,const CSciIF& data)
{//{{{
    os << "StartBit" <<": " << data.GetStartBit() << endl;
    os << "Data"     <<": " << data.GetData()     << endl;
    os << "ParityBit"<<": " << data.GetParityBit()<< endl;
    os << "StopBit"  <<": " << data.GetStopBit()  << endl;
    os << "Te"       <<": " << data.GetTe()       << endl;
    os << "Dir"      <<": " << data.GetDir()      << endl;
    os << "Size"     <<": " << data.GetSize()     << endl;
    os << "Bitt"     <<": " << data.GetBitt()     << endl;
    os << "Break"    <<": " << data.GetBreak()    << endl;
    os << "Seq"      <<": " << data.GetSeq()      << endl;
    return os;
}//}}}
// vim600: set foldmethod=marker :
