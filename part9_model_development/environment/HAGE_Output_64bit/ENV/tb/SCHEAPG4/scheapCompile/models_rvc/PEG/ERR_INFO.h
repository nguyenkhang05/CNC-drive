// ---------------------------------------------------------------------
// $Id: ERR_INFO.h,v 1.1 2014/03/03 11:25:59 binhnguyen Exp $
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

#ifndef __ERR_INFO_H__
#define __ERR_INFO_H__
#include <systemc.h>
#include <sstream>
struct Cerrinfo
{
public:
    bool PEGRdERR;
    bool PEGWrERR;
    unsigned char SPID;
    unsigned char PEID;

    void Initialize(void);
    void operator=(const Cerrinfo &dest_data);
    bool operator ==(const Cerrinfo &indata) const;
    //Construct and Destruct
    Cerrinfo();
    ~Cerrinfo();
   
};

inline void sc_trace(sc_core::sc_trace_file* tf, const Cerrinfo& data, const std::string& fn)
{//{{{
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
    str_tmp << "peid";
    sc_trace(tf, data.PEID, str_tmp.str().c_str());
}//}}}

std::ostream& operator <<(std::ostream&,const Cerrinfo&);

#endif //__ERR_INFO_H__                                                      
