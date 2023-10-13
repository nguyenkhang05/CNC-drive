// ---------------------------------------------------------------------
// $Id: SysErrTCM.h,v 1.1 2014/02/28 03:16:49 dungvannguyen Exp $
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

#ifndef __SYSERRTCM_H__
#define __SYSERRTCM_H__
#include <systemc.h>
#include <sstream>
struct Csyserrtcm
{
public:
    bool SysErrTCM;
    bool SysErrTCMVld;
    unsigned int SysErrTCMInfo;
    unsigned int SysErrTCMAdr;
    
    void Initialize(void);
    void operator=(const Csyserrtcm &dest_data);
    bool operator ==(const Csyserrtcm &indata) const;
    //Construct and Destruct
    Csyserrtcm();
    ~Csyserrtcm();
};

inline void sc_trace(sc_core::sc_trace_file* tf, const Csyserrtcm& data, const std::string& fn)
{//{{{
    std::ostringstream str_tmp;
    str_tmp.str("");
    str_tmp << "SysErrTCM";
    sc_trace(tf, data.SysErrTCM, str_tmp.str().c_str());

    str_tmp.str("");
    str_tmp << "SysErrTCMVld";
    sc_trace(tf, data.SysErrTCMVld, str_tmp.str().c_str());

    str_tmp.str("");
    str_tmp << "SysErrTCMInfo";
    sc_trace(tf, data.SysErrTCMInfo, str_tmp.str().c_str());

    str_tmp.str("");
    str_tmp << "SysErrTCMAdr";
    sc_trace(tf, data.SysErrTCMAdr, str_tmp.str().c_str());
}//}}}

std::ostream& operator <<(std::ostream&,const Csyserrtcm&);
#endif //__SYSERRTCM_H__
