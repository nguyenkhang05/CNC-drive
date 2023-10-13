// ---------------------------------------------------------------------
// $Id: SysErrIPG.h,v 1.1 2014/02/28 03:16:49 dungvannguyen Exp $
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

#ifndef __SYSERRIPG_H__
#define __SYSERRIPG_H__
#include <systemc.h>
#include <sstream>
struct Csyserripg
{
public:
    bool SysErrIPG;
    bool SysErrIPGUM;
    bool SysErrIPGVM;
    unsigned char SysErrIPGVMId;

    void Initialize(void);
    void operator=(const Csyserripg &dest_data);
    bool operator ==(const Csyserripg &indata) const;
    //Construct and Destruct
    Csyserripg();
    ~Csyserripg();
};

inline void sc_trace(sc_core::sc_trace_file* tf, const Csyserripg& data, const std::string& fn)
{//{{{
    std::ostringstream str_tmp;
    str_tmp.str("");
    str_tmp << "SysErrIPG";
    sc_trace(tf, data.SysErrIPG, str_tmp.str().c_str());

    str_tmp.str("");
    str_tmp << "SysErrIPGUM";
    sc_trace(tf, data.SysErrIPGUM, str_tmp.str().c_str());

    str_tmp.str("");
    str_tmp << "SysErrIPGVM";
    sc_trace(tf, data.SysErrIPGVM, str_tmp.str().c_str());

    str_tmp.str("");
    str_tmp << "SysErrIPGVMId";
    sc_trace(tf, data.SysErrIPGVMId, str_tmp.str().c_str());
}//}}}

std::ostream& operator <<(std::ostream&,const Csyserripg&);
#endif //__SYSERRIPG_H__
