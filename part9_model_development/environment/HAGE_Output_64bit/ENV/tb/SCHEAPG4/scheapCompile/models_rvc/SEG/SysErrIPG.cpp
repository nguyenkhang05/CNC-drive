// ---------------------------------------------------------------------
// $Id: SysErrIPG.cpp,v 1.1 2014/02/28 03:16:49 dungvannguyen Exp $
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
#include "SysErrIPG.h"

//===============Csyserripg struct=======================
//Constructor of Csyserripg struct
Csyserripg::Csyserripg()  
{//{{{
    Initialize();
}//}}}

void Csyserripg::Initialize(void)
{//{{{ 
    SysErrIPG = false;
    SysErrIPGUM = false;
    SysErrIPGVM = false;
    SysErrIPGVMId = 0x0;
}//}}}

//Destructor of Csyserripg class
Csyserripg::~Csyserripg()
{//{{{ 
}//}}} 

void Csyserripg::operator=(const Csyserripg &dest_data)
{//{{{
    this->SysErrIPG     = dest_data.SysErrIPG;
    this->SysErrIPGUM     = dest_data.SysErrIPGUM;
    this->SysErrIPGVM     = dest_data.SysErrIPGVM;
    this->SysErrIPGVMId     = dest_data.SysErrIPGVMId;
}//}}} 

bool Csyserripg::operator ==(const Csyserripg &indata) const
{//{{{
    bool is_same = false;
    if ((this->SysErrIPG == indata.SysErrIPG) && (this->SysErrIPGUM == indata.SysErrIPGUM) && 
        (this->SysErrIPGVM == indata.SysErrIPGVM) && (this->SysErrIPGVMId == indata.SysErrIPGVMId)) {
        is_same = true;
    }
    return(is_same);
}//}}}

std::ostream& operator <<(std::ostream& os,const Csyserripg& data)
{//{{{
    os << "SysErrIPG" <<": " << data.SysErrIPG << endl;
    os << "SysErrIPGUM" <<": " << data.SysErrIPGUM << endl;
    os << "SysErrIPGVM" <<": " << data.SysErrIPGVM << endl;
    os << "SysErrIPGVMId" <<": " << data.SysErrIPGVMId << endl;
    return os;
}//}}}
// vim600: set foldmethod=marker :
