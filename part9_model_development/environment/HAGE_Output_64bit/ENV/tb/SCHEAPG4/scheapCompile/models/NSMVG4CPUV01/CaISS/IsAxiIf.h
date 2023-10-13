/*
* Copyright(c) 2011-2013 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

/* $Id:$ */

#ifndef ISAXIIF_H
#define ISAXIIF_H

#define AXI_MAX_DATA  1024

#include <list>
#include <iostream>

#include "module_vc.h"

#include "IsIfBase.h"

#include "BusMasterIf.h"
#include "BusMasterFuncIf.h"
#include "BusTimeBase.h"

class CaIss;

class IsAxiIf : public BusMasterFuncIf, public CModuleVc, public IsIfBase
{
  public:
    IsAxiIf( CaIss *pCaIss, BusMasterIf<BUS_WIDTH_AXI> **axiIf, unsigned int num=PE_MAX_NUM );
    ~IsAxiIf( void );

    void CyclicHandler (void);
    SysErrCause TargetWrite (SideBand sideband, PhysAddr addr,
		      uint8_t* p_data, ByteSize size);
    SysErrCause TargetRead  (SideBand sideband, PhysAddr addr,
		      uint8_t* p_data, ByteSize size);
    /// <summary>request the fetch transaction</summary>
    /// <param name="sideband">sideband</param>
    /// <param name="addr">address</param>
    /// <param name="p_data">data pointer</param>
    /// <param name="size">data length</param>
    /// <returns>SYSERR cause</returns>
    SysErrCause TargetFetch (SideBand sideband, PhysAddr addr,
		      uint8_t* p_data, ByteSize size) {return(SYSERR_NONE);};
    SysErrCause TargetWriteDebug (PhysAddr addr, uint8_t* p_data, ByteSize size);
    SysErrCause TargetReadDebug  (PhysAddr addr, uint8_t* p_data, ByteSize size);
    /// <summary>Check the access permition of the memory</summary>
    /// <param name="sideband">sideband</param>
    /// <param name="addr">address</param>
    /// <param name="size">data length</param>
    /// <param name="cmd">access command</param>
    /// <returns>SYSERR cause</returns>
    SysErrCause IsAccPermited (SideBand sideband, PhysAddr addr, ByteSize size, MemAccessCommand cmd) { return SYSERR_NONE; }
    void HardReset (void);

  private:
    /// <summary>set transaction information to the AXI's extention payload</summary>
    /// <param name="burstType">burst type</param>
    /// <param name="lock">lock</param>
    /// <param name="cachable">cachable</param>
    /// <param name="bufferable">bufferable</param>
    /// <param name="secure">secure</param>
    /// <param name="transId">transaction ID</param>
    /// <param name="bitopType">bit operation type</param>
    /// <param name="bitopPos">bit operation pos</param>
    /// <param name="trans">Basic Payload</param>
    /// <param name="axiExt">AXI Extension</param>
    /// <returns>none</returns>
    void setTransAxi( TlmAxiExtension::AxiBurst_t burstType,
		      bool                        lock,
		      bool                        cachable,
		      bool                        bufferable,
		      unsigned char               secure,
		      unsigned int                transId,
		      TlmAxiExtension::AxiBitop_t bitopType,
		      unsigned char               bitopPos,
		      TlmBasicPayload             &trans,
		      TlmAxiExtension             &axiExt )
    {
      axiExt.setBurstType( burstType );
      axiExt.setLock( lock );
      axiExt.setCachable( cachable );
      axiExt.setBufferable( bufferable );
      axiExt.setSecure( secure );
      axiExt.setTransId( transId );
      axiExt.setBitOpType( bitopType );
      axiExt.setBitOpPos( bitopPos );
      trans.set_extension( &axiExt );
    }


    void notifyTransSend( TlmTransactionType &trans, BusTime_t &t );
    void notifyTransComp( TlmTransactionType &trans, BusTime_t &t );
    void DeleteTrans( TlmTransactionType &trans );

    TlmAxiExtension::AxiBitop_t getBitOpType (SideBand sideband);
    unsigned char getBitOpPos (SideBand sideband);
    bool getLockMode (SideBand sideband);

  private:
    CaIss                      *mpCaIss;         // for the pointer of CaIss's instance
    BusMasterIf<BUS_WIDTH_AXI> *mpAxiIf[PE_MAX_NUM];         // for the pointer of BusMasterIf<BUS_WIDTH_AXI>'s instance
    std::list<trans_info*>      mTransInfoQueue; // for transaction information list

    unsigned char               mDataDbg[AXI_MAX_DATA];     // for data (debug access)
    TlmBasicPayload             mDbgTrans;                  // for basic payload (debug access)
    TlmG3mExtension             mDbgG3mExt;                 // for extension payload of G3M (debug access)
    TlmAxiExtension             mDbgAxiExt;                 // for extension payload of AXI (debug access)

    unsigned int                mTransId;                   // for TlmAxiExtension's transaction ID
}; // end of IsAxiIf definition

#endif /* ISAXIIF_H */
