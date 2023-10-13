// ---------------------------------------------------------------------
// $Id: SysErrVCI.cpp,v 1.1 2014/02/28 03:16:50 dungvannguyen Exp $
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
#include "SysErrVCI.h"

//===============Csyserrvci struct=======================
//Constructor of Csyserrvci struct
Csyserrvci::Csyserrvci()  
{//{{{
    Initialize();
}//}}}

void Csyserrvci::Initialize(void)
{//{{{
    SysErrVCI = false;
    SysErrVCIVld = false;
    SysErrVCIInfo = 0;
    SysErrVCIAdr = 0;
}//}}}

//Destructor of Csyserrvci class
Csyserrvci::~Csyserrvci()
{//{{{
}//}}} 

void Csyserrvci::operator=(const Csyserrvci &dest_data)
{//{{{
    this->SysErrVCI     = dest_data.SysErrVCI;
    this->SysErrVCIVld   = dest_data.SysErrVCIVld;
    this->SysErrVCIInfo = dest_data.SysErrVCIInfo;
    this->SysErrVCIAdr  = dest_data.SysErrVCIAdr;
}//}}} 

bool Csyserrvci::operator ==(const Csyserrvci &indata) const
{//{{{
    bool is_same = false;
    if ((this->SysErrVCI == indata.SysErrVCI) && (this->SysErrVCIVld == indata.SysErrVCIVld) && 
        (this->SysErrVCIInfo == indata.SysErrVCIInfo) && (this->SysErrVCIAdr == indata.SysErrVCIAdr)) {
        is_same = true;
    }
    return(is_same);
}//}}}

std::ostream& operator <<(std::ostream& os,const Csyserrvci& data)
{//{{{
    os << "SysErrVCI"       <<": " << data.SysErrVCI        << endl;
    os << "SysErrVCIVld"    <<": " << data.SysErrVCIVld     << endl;
    os << "SysErrVCIInfo"   <<": " << data.SysErrVCIInfo    << endl;
    os << "SysErrVCIAdr"    <<": " << data.SysErrVCIAdr     << endl;
    return os;
}//}}}

// vim600: set foldmethod=marker :
