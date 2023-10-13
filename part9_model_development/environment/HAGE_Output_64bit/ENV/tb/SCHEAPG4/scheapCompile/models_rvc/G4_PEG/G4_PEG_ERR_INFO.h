// ---------------------------------------------------------------------
// $Id: G4_PEG_ERR_INFO.h,v 1.6 2020/01/14 02:39:30 nhutminhdang Exp $
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

#ifndef __G4_PEG_ERR_INFO_H__
#define __G4_PEG_ERR_INFO_H__
#include <systemc.h>
#include <sstream>
struct G4_PEG_Cerrinfo
{
public:
    /// Constructor & Destructor
    G4_PEG_Cerrinfo();
    ~G4_PEG_Cerrinfo();
    
    /// Variables
    bool PEGRdERR;
    bool PEGWrERR;
    bool UM;
    bool DBG;
    bool SEC;
    unsigned int  SPID;
    unsigned int  BMID;
    unsigned int  ErrAddr;

    /// Functions
    void initialize(void);
    void operator = (const G4_PEG_Cerrinfo &dest_data);
    bool operator ==(const G4_PEG_Cerrinfo &indata) const;

};

inline void sc_trace(sc_core::sc_trace_file* tf, const G4_PEG_Cerrinfo& data, const std::string& fn)
{//{{{
    if (tf != NULL) {
        std::ostringstream str_tmp;
        str_tmp.str("");
        str_tmp << "PEGWrERR";
        sc_trace(tf, data.PEGWrERR, str_tmp.str().c_str());

        str_tmp.str("");
        str_tmp << "PEGRdERR";
        sc_trace(tf, data.PEGRdERR, str_tmp.str().c_str());

        str_tmp.str("");
        str_tmp << "spid";
        sc_trace(tf, data.SPID, str_tmp.str().c_str());

        str_tmp.str("");
        str_tmp << "bmid";
        sc_trace(tf, data.BMID, str_tmp.str().c_str());

        str_tmp.str("");
        str_tmp << "ErrAddr";
        sc_trace(tf, data.ErrAddr, str_tmp.str().c_str());

        str_tmp.str("");
        str_tmp << "um";
        sc_trace(tf, data.UM, str_tmp.str().c_str());

        str_tmp.str("");
        str_tmp << "dbg";
        sc_trace(tf, data.DBG, str_tmp.str().c_str());
        
        str_tmp.str("");
        str_tmp << "sec";
        sc_trace(tf, data.SEC, str_tmp.str().c_str());
    }
}//}}}

std::ostream& operator <<(std::ostream&,const G4_PEG_Cerrinfo&);

#endif //__G4_PEG_ERR_INFO_H__
