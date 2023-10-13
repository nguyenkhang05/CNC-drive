// ---------------------------------------------------------------------
// $Id: SysErrTCM.cpp,v 1.1 2014/02/28 03:16:49 dungvannguyen Exp $
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
#include "SysErrTCM.h"

//===============Csyserrtcm struct=======================
//Constructor of Csyserrtcm struct
Csyserrtcm::Csyserrtcm()  
{//{{{
    Initialize();
}//}}}

void Csyserrtcm::Initialize(void)
{//{{{ 
    SysErrTCM = false;
    SysErrTCMVld = false;
    SysErrTCMInfo = 0;
    SysErrTCMAdr = 0;
}//}}}

//Destructor of Csyserrtcm class
Csyserrtcm::~Csyserrtcm()
{//{{{ 
}//}}} 

void Csyserrtcm::operator=(const Csyserrtcm &dest_data)
{//{{{
    this->SysErrTCM     = dest_data.SysErrTCM;
    this->SysErrTCMVld   = dest_data.SysErrTCMVld;
    this->SysErrTCMInfo = dest_data.SysErrTCMInfo;
    this->SysErrTCMAdr  = dest_data.SysErrTCMAdr;
}//}}} 

bool Csyserrtcm::operator ==(const Csyserrtcm &indata) const
{//{{{
    bool is_same = false;
    if ((this->SysErrTCM == indata.SysErrTCM) && (this->SysErrTCMVld == indata.SysErrTCMVld) && 
        (this->SysErrTCMInfo == indata.SysErrTCMInfo) && (this->SysErrTCMAdr == indata.SysErrTCMAdr)) {
        is_same = true;
    }
    return(is_same);
}//}}}

std::ostream& operator <<(std::ostream& os,const Csyserrtcm& data)
{//{{{
    os << "SysErrTCM" <<": " << data.SysErrTCM << endl;
    os << "SysErrTCMVld" <<": " << data.SysErrTCMVld << endl;
    os << "SysErrTCMInfo"     <<": " << data.SysErrTCMInfo << endl;
    os << "SysErrTCMAdr"     <<": " << data.SysErrTCMAdr << endl;
    return os;
}//}}}
// vim600: set foldmethod=marker :
