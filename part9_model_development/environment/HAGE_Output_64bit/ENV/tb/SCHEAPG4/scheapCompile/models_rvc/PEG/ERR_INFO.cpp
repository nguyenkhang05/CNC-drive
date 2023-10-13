// ---------------------------------------------------------------------
// $Id: ERR_INFO.cpp,v 1.0 2014/02/16 05:03:30 duymai Exp $
//
// Copyright(c) 2014 Renesas Electronics Corporation
// Copyright(c) 2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include <iostream>
#include <sstream>
#include "ERR_INFO.h"

//===============Cerrinfo struct=======================
//Constructor of Cerrinfo struc
Cerrinfo::Cerrinfo()  
{//{{{
    Initialize();
}//}}}

void Cerrinfo::Initialize(void)
{//{{{
    PEGWrERR = false;
    PEGRdERR = false;
    SPID = 0;
    PEID = 0;
}//}}}

//Destructor of Cerrinfo class
Cerrinfo::~Cerrinfo()
{//{{{
}//}}} 

void Cerrinfo::operator=(const Cerrinfo &dest_data)
{//{{{
    this->PEGWrERR  = dest_data.PEGWrERR;
    this->PEGRdERR  = dest_data.PEGRdERR;
    this->SPID      = dest_data.SPID;
    this->PEID      = dest_data.PEID;
}//}}} 

bool Cerrinfo::operator ==(const Cerrinfo &indata) const
{//{{{
    bool is_same = false;
    if ((this->PEGWrERR == indata.PEGWrERR) && (this->PEGRdERR == indata.PEGRdERR) && 
        (this->SPID == indata.SPID) && (this->PEID == indata.PEID)) {
        is_same = true;
    }
    return(is_same);
}//}}}

std::ostream& operator <<(std::ostream& os,const Cerrinfo& data)
{//{{{
    os << "Write access error" <<": " << data.PEGWrERR   << endl;
    os << "Read access error" <<": " << data.PEGRdERR    << endl;
    os << "SPID"     <<": " << data.SPID                 << endl;
    os << "PEID"     <<": " << data.PEID                 << endl;
    return os;
}//}}}

// vim600: set foldmethod=marker :
