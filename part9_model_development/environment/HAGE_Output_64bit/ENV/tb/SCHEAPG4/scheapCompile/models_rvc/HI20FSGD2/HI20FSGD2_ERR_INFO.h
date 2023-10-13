// ---------------------------------------------------------------------
// $Id: HI20FSGD2_ERR_INFO.h,v 1.1 2019/11/26 05:07:33 hadinhtran Exp $
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

#ifndef __HI20FSGD2_ERR_INFO_H__
#define __HI20FSGD2_ERR_INFO_H__
#include <systemc.h>
#include <sstream>
struct HI20FSGD2_Cerrinfo
{
public:
    /// Constructor & Destructor
    HI20FSGD2_Cerrinfo();
    ~HI20FSGD2_Cerrinfo();
    
    /// Variables
    bool PBGRdERR;
    bool PBGWrERR;
    bool UM;
    bool DBG;
    bool SEC;
    unsigned int  SPID;
    unsigned int  ErrAddr;

    /// Functions
    void initialize(void);
    void operator = (const HI20FSGD2_Cerrinfo &dest_data);
    bool operator ==(const HI20FSGD2_Cerrinfo &indata) const;

};

inline void sc_trace(sc_core::sc_trace_file* tf, const HI20FSGD2_Cerrinfo& data, const std::string& fn)
{//{{{
    if (tf != NULL){
        std::ostringstream str_tmp;
        str_tmp.str("");
        str_tmp << "PBGWrERR";
        sc_trace(tf, data.PBGWrERR, str_tmp.str().c_str());
    
        str_tmp.str("");
        str_tmp << "PBGRdERR";
        sc_trace(tf, data.PBGRdERR, str_tmp.str().c_str());
    
        str_tmp.str("");
        str_tmp << "um";
        sc_trace(tf, data.UM, str_tmp.str().c_str());
    
        str_tmp.str("");
        str_tmp << "dbg";
        sc_trace(tf, data.DBG, str_tmp.str().c_str());
        
        str_tmp.str("");
        str_tmp << "sec";
        sc_trace(tf, data.SEC, str_tmp.str().c_str());
    
        str_tmp.str("");
        str_tmp << "spid";
        sc_trace(tf, data.SPID, str_tmp.str().c_str());
    
        str_tmp.str("");
        str_tmp << "ErrAddr";
        sc_trace(tf, data.ErrAddr, str_tmp.str().c_str());
    }

}//}}}

std::ostream& operator <<(std::ostream&,const HI20FSGD2_Cerrinfo&);

#endif //__HI20FSGD2_ERR_INFO_H__
