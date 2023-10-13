/*
* Copyright(c) 2016 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

/* $Id:$ */

#ifndef G4_ISIFBASE_H
#define G4_ISIFBASE_H

#include "forest_common.h"
#include "sideband.h"
#include "OSCI2.h"
#include "global.h"

class G4_IsIfBase
{
  public:
    typedef enum {
      ACC_WRITE,
      ACC_READ,
      ACC_FETCH
    } ACC_MODE_T;

  protected:
    typedef struct{
      SideBand             sideband;   // for the sideband value of transaction
      PhysAddr             addr;       // for the address of transaction
      ByteSize             size;       // for the access size of transaction
      G4_IsIfBase::ACC_MODE_T acc_mode;   // for the access mode
      TlmBasicPayload     *trans;      // for the pointer of transaction data
      BusTime_t            req_time;   // for the time of receiving request 
    }trans_info;

  public:
    G4_IsIfBase( void );
    ~G4_IsIfBase( void );

  protected:
    unsigned long long getTransDataVal(TlmTransactionType &trans);

    /// <summary>get PE id from sideband data</summary>
    /// <param name="sideband">sideband</param>
    /// <returns>Bus Master ID</returns>
    unsigned char getBmId (SideBand sideband)
    {
      return((unsigned char)(sideband.GetBMID ()));
    }


    /// <summary>get SP id from sideband data</summary>
    /// <param name="sideband">sideband</param>
    /// <returns>SPID</returns>
    unsigned char getSpId (SideBand sideband)
    {
      return((unsigned char)(sideband.GetSPID ()));
    }


    /// <summary>get user mode from sideband data</summary>
    /// <param name="sideband">sideband</param>
    /// <returns>user mode</returns>
    bool getUserMode (SideBand sideband)
    {
      if(sideband.GetUM ()){
	return(true);
      }else{
	return(false);
      }
    }


    /// <summary>get bit opeation mode from sideband data</summary>
    /// <param name="sideband">sideband</param>
    /// <returns>bit operation mode</returns>
    unsigned int getBitOp (SideBand sideband)
    {
      return(sideband.GetBitOp ());
    }


    /// <summary>get bit position from sideband data</summary>
    /// <param name="sideband">sideband</param>
    /// <returns>bit position</returns>
    unsigned int getBitPos (SideBand sideband)
    {
      return(sideband.GetBitPos ());
    }


    /// <summary>get speculation load flag from sideband data</summary>
    /// <param name="sideband">sideband</param>
    /// <returns>user mode</returns>
    bool getSpcLd (SideBand sideband)
    {
      if(sideband.GetSPCLD () == 1){
	return(true);
      }else{
	return(false);
      }
    }


    /// <summary>get CPU mode (PSWH.GM) from sideband data</summary>
    /// <param name="sideband">sideband</param>
    /// <returns>CPU mode (PSWH.GM)</returns>
    bool getGm (SideBand sideband)
    {
      if(sideband.GetGM () == 1){
	return(true);
      }else{
	return(false);
      }
    }

    /// <summary>get GPID (PSWH.GPID) from sideband data</summary>
    /// <param name="sideband">sideband</param>
    /// <returns>GPID (PSWH.GPID)</returns>
    unsigned char getGpid (SideBand sideband)
    {
      return((unsigned char)(sideband.GetGPID ()));
    }


    /// <summary>set transaction information to the basic payload</summary>
    /// <param name="address">address</param>
    /// <param name="length">data length</param>
    /// <param name="data">data pointer</param>
    /// <param name="trans">transaction</param>
    /// <returns>none</returns>
    void setTransBasic( uint64          address,
			unsigned int    length,
			unsigned char   *data,
			TlmBasicPayload &trans )
    {
      trans.set_address( address );
      trans.set_data_ptr( data );
      trans.set_data_length( length );
    }

    /// <summary>set transaction information to the G3M's extention payload</summary>
    /// <param name="vmId">VM ID</param>
    /// <param name="tcId">TC ID</param>
    /// <param name="peId">PE ID</param>
    /// <param name="spId">SPID</param>
    /// <param name="userMode">user mode</param>
    /// <param name="virtualMode">virtual mode</param>
    /// <param name="speculativeMode">speculation load flag</param>
    /// <param name="trans">transaction</param>
    /// <param name="g3mExt">G3M's extention payload</param>
    /// <param name="dmaAccess"></param>
    /// <returns>none</returns>
    void setTransG3m( unsigned char   vmId,
		      unsigned char   tcId,
		      unsigned char   peId,
		      unsigned char   spId,
		      bool            userMode,
		      bool            virtualMode,
		      bool            speculativeMode,
		      TlmBasicPayload &trans,
		      TlmG3mExtension &g3mExt,
		      bool            dmaAccess=false )
    {
      g3mExt.setDmaAccess( dmaAccess );
      g3mExt.setVmId( vmId );
      g3mExt.setTcId( tcId );
      g3mExt.setPeId( peId );
      g3mExt.setSpId( spId );
      g3mExt.setUserMode( userMode );
      g3mExt.setVirtualMode( virtualMode );
      g3mExt.setSpcLd( speculativeMode );
      trans.set_extension( &g3mExt );
#ifdef BUSIF_TEST_LOG
      printf("G4_IsIfBase::setTransG3m: mSpcLd=%d, mVirtualMode=%d, mVmId=0x%x\n", g3mExt.isSpcLd(), g3mExt.isVirtualMode(), g3mExt.getVmId());
      fflush(stdout);
#endif
    }

}; // end of G4_IsIfBase definition

#endif /* G4_ISIFBASE_H */
