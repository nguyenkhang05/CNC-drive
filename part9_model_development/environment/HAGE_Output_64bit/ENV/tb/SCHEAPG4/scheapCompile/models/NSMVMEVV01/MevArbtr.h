/*************************************************************************
 * Copyright(c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  MEV_ARBITOR_H
#define  MEV_ARBITOR_H

#include <map>
#include "systemc.h"
#include "global.h"
#include "OSCI2ARBTR.h"


class MevArbtr: public OSCI2ARBTR<BUS_WIDTH_APB>
{
  class MevArbReqList: public OSCI2ARBTR<BUS_WIDTH_APB>::BusArbRequestList
  {
  public:
    /* constructor */
    MevArbReqList( const char* upperModName );

    /* destructor */
    virtual ~MevArbReqList( void );

    /* set functions */
    void    setName( char* name );
    void    setPeNum( unsigned int peNum );
    void    setPriorityList( std::map<unsigned int, unsigned int>* priorityList );

    /* override functions */
    virtual OSCI2ARBTR<BUS_WIDTH_APB>::BusArbRequest* getOtherRequest( TlmBwTransportIf *pIf );

  private:
    /* private variables */
    char         mModName[128]; // module name
    unsigned int mPeNum;        // Number of PE
    std::map<unsigned int, unsigned int>* mpPriorityList;  // pointer to priority list
  };

public:
  /* constructor */
  MevArbtr( sc_module_name name );

  /* destructor */
  ~MevArbtr( void );

  /* set functions */
  void setPeNum( unsigned int peNum );
  void setPriority( unsigned int peid, unsigned int priority );
  void rmvPriority( unsigned int peid );
  void clrPriority( void );

  /* override functions */
  virtual TlmSyncEnum nb_transport_fw( TlmTransactionType& trans,
                                       TlmPhase&           phase,
                                       sc_core::sc_time&   t );
  virtual TlmSyncEnum nb_transport_bw( TlmTransactionType& trans,
                                       TlmPhase&           phase,
                                       sc_core::sc_time&   t );

  /* public variables */
  std::map<unsigned int, unsigned int> mPriorityList; // priority list

private:
  /* private variables */
  char          mErrMsg[128];   // for error message
  MevArbReqList mPeRequestList; // PE request list
  unsigned int  mPeNum;         // Number of PE

  /* override function */
  virtual void arbitrate( void );

  /* override process */
  virtual void resetProcess( void );
  virtual void actionAtClockPos( void );
};

#endif // MEV_ARBITOR_H
