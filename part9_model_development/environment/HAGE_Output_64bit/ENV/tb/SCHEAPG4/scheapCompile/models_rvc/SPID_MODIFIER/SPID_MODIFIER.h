// ---------------------------------------------------------------------
// $Id: SPID_MODIFIER.h,v 1.3 2019/12/27 02:16:58 ngathunguyen Exp $
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

#ifndef __SPID_MODIFIER_H__
#define __SPID_MODIFIER_H__

#include "OSCI2.h"
#include <map>
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>
#include "global.h"
#include "re_define.h"
//Ref: {SPID_MODIFIER_DD_ATT1_001, SPID_MODIFIER_DD_Direction_020}
template<unsigned int BUS_WIDTH> 
class SPID_MODIFIER :
    public sc_module,
    virtual public TlmFwTransportIf,
    virtual public TlmBwTransportIf
{
#include "SPID_MODIFIER_cmdif.h"
//----------------------------
// constructing the model
//----------------------------
public:
    TlmInitiatorSocket<BUS_WIDTH>   is;                 // initiator socket
    TlmTargetSocket<BUS_WIDTH>      ts;                 // target socket
    sc_in<bool>                     resetPort;          // reset port
    sc_in<unsigned char>            SPID;               // reset port
    SC_HAS_PROCESS(SPID_MODIFIER);
    SPID_MODIFIER(sc_module_name name);
    ~SPID_MODIFIER(void);
    
    void AssertReset     (const std::string reset_name
                         ,const double      start_time
                         ,const double      period);
    void SetMessageLevel (const std::string msg_lv);
    void Help            (const std::string type);
    
private:
    unsigned char       mSPIDPort;
    // <TlmTransactionType*, <SPID, IfPointer>>
    std::map<TlmTransactionType*, std::pair<unsigned char, TlmBwTransportIf*> > mTransMap; // Bus request map

    bool   mIsResetPortActive;  ///< Is reset by port
    bool   mIsResetCmdReceive;  ///< Is receive cmd reset
    bool   mIsResetCmdActive;   ///< Is reset by cmd
    double mResetCmdPeriod;     ///< Period of cmd reset
    sc_event mResetCmdEvent;
    sc_event mCancelResetCmdEvent;

    void HandleSPIDPortMethod();
    void HandleresetPortMethod();
    void HandleresetPortCmdMethod();
    void CancelResetCmdMethod();
    void EnableReset(bool isActive);
    void Initialize();
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);

    TlmSyncEnum nb_transport_fw( TlmTransactionType& trans, TlmPhase& phase, sc_core::sc_time& t );
    void b_transport( TlmTransactionType& trans, sc_core::sc_time& t );
    unsigned int transport_dbg( TlmTransactionType& r );
    TlmSyncEnum nb_transport_bw( TlmTransactionType& trans, TlmPhase& phase, sc_core::sc_time& t );
    void BackupTransInfo ( TlmTransactionType &trans);
    void RestoreTrans ( TlmTransactionType &trans);
    void UpdateTransaction( TlmTransactionType &trans, unsigned char spid, TlmBwTransportIf* IfPointer );
    bool get_direct_mem_ptr( TlmTransactionType& trans, tlm::tlm_dmi&  dmiData );
    void invalidate_direct_mem_ptr( uint64 startRange, uint64 endRange );
};

#endif /*__SPID_MODIFIER_H__*/
