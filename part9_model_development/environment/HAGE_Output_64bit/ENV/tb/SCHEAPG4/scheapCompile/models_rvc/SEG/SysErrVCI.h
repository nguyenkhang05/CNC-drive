// ---------------------------------------------------------------------
// $Id: SysErrVCI.h,v 1.1 2014/02/28 03:16:50 dungvannguyen Exp $
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

#ifndef __SYSERRVCI_H__
#define __SYSERRVCI_H__
#include <systemc.h>
#include <sstream>
struct Csyserrvci
{
public:
    bool SysErrVCI;
    bool SysErrVCIVld;
    unsigned int SysErrVCIInfo;
    unsigned int SysErrVCIAdr;

    void Initialize(void);
    void operator=(const Csyserrvci &dest_data);
    bool operator ==(const Csyserrvci &indata) const;
    //Construct and Destruct
    Csyserrvci();
    ~Csyserrvci();
   
};

inline void sc_trace(sc_core::sc_trace_file* tf, const Csyserrvci& data, const std::string& fn)
{//{{{
    std::ostringstream str_tmp;
    str_tmp.str("");
    str_tmp << "SysErrVCI";
    sc_trace(tf, data.SysErrVCI, str_tmp.str().c_str());

    str_tmp.str("");
    str_tmp << "SysErrVCIVld";
    sc_trace(tf, data.SysErrVCIVld, str_tmp.str().c_str());

    str_tmp.str("");
    str_tmp << "SysErrVCIInfo";
    sc_trace(tf, data.SysErrVCIInfo, str_tmp.str().c_str());

    str_tmp.str("");
    str_tmp << "SysErrVCIAdr";
    sc_trace(tf, data.SysErrVCIAdr, str_tmp.str().c_str());
}//}}}

std::ostream& operator <<(std::ostream&,const Csyserrvci&);

#endif //__SYSERRVCI_H__                                                      
