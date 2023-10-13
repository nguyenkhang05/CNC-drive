// ---------------------------------------------------------------------
// $Id: HI20FSGD2_ERR_INFO.cpp,v 1.1 2019/11/26 05:07:33 hadinhtran Exp $
//
// Copyright(c) 2019 Renesas Electronics Corporation
// Copyright(c) 2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include <iostream>
#include <sstream>
#include "HI20FSGD2_ERR_INFO.h"

/*********************************
// Function     : HI20FSGD2_Cerrinfo
// Description  : Constructor of HI20FSGD2_Cerrinfo class
// Parameter    : None
// Return value : None
**********************************/
HI20FSGD2_Cerrinfo::HI20FSGD2_Cerrinfo()
{//{{{
    initialize();
}//}}}

/*********************************
// Function     : ~HI20FSGD2_Cerrinfo
// Description  : Destructor of HI20FSGD2_Cerrinfo class
// Parameter    : None
// Return value : None
**********************************/
HI20FSGD2_Cerrinfo::~HI20FSGD2_Cerrinfo()
{//{{{
}//}}}

/*********************************
// Function     : ~Initialize
// Description  : Initialize internal variables
// Parameter    : None
// Return value : None
**********************************/
void HI20FSGD2_Cerrinfo::initialize(void)
{//{{{
    PBGWrERR = false;
    PBGRdERR = false;
    UM       = false;
    DBG      = false;
    SEC      = false;
    SPID     = 0;
    ErrAddr  = 0;
}//}}}

/*********************************
// Function     : operator =
// Description  : Operator overloading function
// Parameter    : 
//    dest_data   right operand
// Return value : None
**********************************/
void HI20FSGD2_Cerrinfo::operator = (const HI20FSGD2_Cerrinfo &dest_data)
{//{{{
    this->PBGWrERR  = dest_data.PBGWrERR;
    this->PBGRdERR  = dest_data.PBGRdERR;
    this->UM        = dest_data.UM;
    this->DBG       = dest_data.DBG;
    this->SEC       = dest_data.SEC;
    this->SPID      = dest_data.SPID;
    this->ErrAddr   = dest_data.ErrAddr;
}//}}}

/*********************************
// Function     : operator ==
// Description  : Operator overloading function
// Parameter    : 
//    dest_data   right operand
// Return value : true: equal; false: not equal
**********************************/
bool HI20FSGD2_Cerrinfo::operator == (const HI20FSGD2_Cerrinfo &indata) const
{//{{{
    
    if ((this->PBGWrERR == indata.PBGWrERR) && (this->PBGRdERR == indata.PBGRdERR) &&
        (this->SPID == indata.SPID) && (this->ErrAddr == indata.ErrAddr) && 
        (this->UM == indata.UM) && (this->DBG == indata.DBG) && 
        (this->SEC == indata.SEC)) {
        return true;
    }
    return false;
}//}}}

/*********************************
// Function     : operator <<
// Description  : Operator overloading function
// Parameter    : 
//    os          output stream
//    data        data need to output
// Return value : output stream
**********************************/
std::ostream& operator << (std::ostream& os,const HI20FSGD2_Cerrinfo& data)
{//{{{
    os << "Write access error" <<": "    <<std::dec<<std::uppercase<< data.PBGWrERR             << endl;
    os << "Read access error " <<": "    <<std::dec<<std::uppercase<< data.PBGRdERR             << endl;
    os << "UM"                 <<": "    <<std::dec<<std::uppercase<< data.UM                   << endl;
    os << "DBG"                <<": "    <<std::dec<<std::uppercase<< data.DBG                  << endl;
    os << "SEC"                <<": "    <<std::dec<<std::uppercase<< data.SEC                  << endl;
    os << "SPID"               <<": 0x"  <<std::hex<<std::uppercase<< (unsigned int) data.SPID  << endl;
    os << "ErrAddr"            <<": 0x"  <<std::hex<<std::uppercase<< data.ErrAddr              << endl;
    return os;
}//}}}

// vim600: set foldmethod=marker :
