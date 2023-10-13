/*
* Copyright(c) 2015 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

/* $Id:$ */

#ifndef ISEMU3IF_H
#define ISEMU3IF_H

#define APB_MAX_DATA  1024

#include <list>
#include <iostream>

#include "module_vc.h"

#include "IsIfBase.h"

#include "BusMasterIf.h"
#include "BusMasterFuncIf.h"
#include "BusTimeBase.h"

class CaIss;

class IsEmu3If : public BusMasterFuncIf, public CModuleVc, public IsIfBase
{
  public:
    IsEmu3If( CaIss *pCaIss, 
	      BusMasterIf<BUS_WIDTH_VCI> **emu3If, 
	      unsigned int num = 1);
    ~IsEmu3If( void );

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
    SysErrCause TargetFetch  (SideBand sideband, PhysAddr addr,
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
    /// <summary>set transaction information to the VCI's extention payload</summary>
    /// <param name="requestType">requset type</param>
    /// <param name="trans">Basic Payload</param>
    /// <param name="vciExt">VCI Extension</param>
    /// <returns>none</returns>
    void setTransVci( TlmVciExtension::VciRequest_t requestType,
		      TlmBasicPayload               &trans,
		      TlmVciExtension               &vciExt )
    {
      vciExt.setRequestType( requestType );
      trans.set_extension( &vciExt );
    }

    TlmVciExtension::VciRequest_t getReqType ( SideBand sideband, TlmVciExtension::VciRequest_t def_req );
    void notifyTransSend( TlmTransactionType &trans, BusTime_t &t );
    void notifyTransComp( TlmTransactionType &trans, BusTime_t &t );
    void DeleteTrans( TlmTransactionType &trans );

  private:
    CaIss                      *mpCaIss;              // for the pointer of BusTimeBase's instance
    BusMasterIf<BUS_WIDTH_VCI> *mpEmu3If[1];          // for the pointer of BusMasterIf<BUS_WIDTH_APB>'s instance
    std::list<trans_info*>      mTransInfoQueue;      // for transaction information list

    unsigned char               mDataDbg[APB_MAX_DATA];    // for data (debug access)
    TlmBasicPayload             mDbgTrans;                 // for basic payload (debug access)
    TlmG3mExtension             mDbgG3mExt;                // for extension payload of G3M (debug access)
    TlmVciExtension             mDbgVciExt;             // for extension payload of VCI (debug access)

    unsigned int                mTransId;               // for TlmVciExtension's transaction ID
}; // end of IsEmu3If definition

#endif /* ISEMU3IF_H */
