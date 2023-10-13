// ---------------------------------------------------------------------
// $Id: G4_PEG_ERR_INFO.cpp,v 1.8 2020/01/14 02:39:30 nhutminhdang Exp $
//
// Copyright(c) 2016-2020 Renesas Electronics Corporation
// Copyright(c) 2016-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include <iostream>
#include <sstream>
#include "G4_PEG_ERR_INFO.h"

/*********************************
// Function     : G4_PEG_Cerrinfo
// Description  : Constructor of G4_PEG_Cerrinfo class
// Parameter    : None
// Return value : None
**********************************/
G4_PEG_Cerrinfo::G4_PEG_Cerrinfo()
{//{{{
    initialize();
}//}}}

/*********************************
// Function     : ~G4_PEG_Cerrinfo
// Description  : Destructor of G4_PEG_Cerrinfo class
// Parameter    : None
// Return value : None
**********************************/
G4_PEG_Cerrinfo::~G4_PEG_Cerrinfo()
{//{{{
}//}}}

/*********************************
// Function     : ~Initialize
// Description  : Initialize internal variables
// Parameter    : None
// Return value : None
**********************************/
void G4_PEG_Cerrinfo::initialize(void)
{//{{{
    PEGWrERR = false;
    PEGRdERR = false;
    UM = false;
    DBG = false;
    SEC = false;
    SPID = 0;
    BMID = 0;
    ErrAddr = 0;
}//}}}

/*********************************
// Function     : operator =
// Description  : Operator overloading function
// Parameter    : 
//    dest_data   right operand
// Return value : None
**********************************/
void G4_PEG_Cerrinfo::operator = (const G4_PEG_Cerrinfo &dest_data)
{//{{{
    this->PEGWrERR  = dest_data.PEGWrERR;
    this->PEGRdERR  = dest_data.PEGRdERR;
    this->UM        = dest_data.UM;
    this->DBG       = dest_data.DBG;
    this-> SEC      = dest_data.SEC;
    this->SPID      = dest_data.SPID;
    this->BMID      = dest_data.BMID;
    this->ErrAddr   = dest_data.ErrAddr;
}//}}}

/*********************************
// Function     : operator ==
// Description  : Operator overloading function
// Parameter    : 
//    dest_data   right operand
// Return value : true: equal; false: not equal
**********************************/
bool G4_PEG_Cerrinfo::operator == (const G4_PEG_Cerrinfo &indata) const
{//{{{
    
    if ((this->PEGWrERR == indata.PEGWrERR) && (this->PEGRdERR == indata.PEGRdERR) &&
        (this->SPID == indata.SPID) && (this->BMID == indata.BMID) &&
        (this->ErrAddr == indata.ErrAddr) && (this->UM == indata.UM) && (this->DBG == indata.DBG) && 
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
std::ostream& operator << (std::ostream& os,const G4_PEG_Cerrinfo& data)
{//{{{
    os << "Write access error" <<": "    <<std::dec<<std::uppercase<< data.PEGWrERR             << endl;
    os << "Read access error " <<": "    <<std::dec<<std::uppercase<< data.PEGRdERR             << endl;
    os << "SPID"               <<": 0x"  <<std::hex<<std::uppercase<< (unsigned int) data.SPID  << endl;
    os << "BMID"               <<": 0x"  <<std::hex<<std::uppercase<< data.BMID                 << endl;
    os << "ErrAddr"            <<": 0x"  <<std::hex<<std::uppercase<< data.ErrAddr              << endl;
    os << "UM"                 <<": "    <<std::dec<<std::uppercase<< data.UM                   << endl;
    os << "DBG"                <<": "    <<std::dec<<std::uppercase<< data.DBG                  << endl;
    os << "SEC"                <<": "    <<std::dec<<std::uppercase<< data.SEC                  << endl;
    return os;
}//}}}

// vim600: set foldmethod=marker :
