/*************************************************************************
 *
 * G4_MevArbtr.cpp
 *
 * Copyright(c) 2016-2017 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

#include <cstdio>
#include "CmErrMsg.h"
#include "G4_MevArbtr.h"
#include "g4_mev_def.h"

using namespace std;
using namespace CmErrMsg;

// for debug output
#ifdef DEBUG_MESSAGE_OUTPUT
# include <fstream>
  extern std::ofstream dmvfout;
#endif // DEBUG_MESSAGE_OUTPUT


/// <summary>constructor</summary>
/// <param name="upperModName">name of upper module</param>
/// <returns>none</returns>
G4_MevArbtr::MevArbReqList::MevArbReqList( const char* upperModName ):
  mpPriorityList( 0 ),
  mPeNum( 1 )
{
  strcpy( mModName, upperModName );
  strcat( mModName, ".MevArbReqList" );
  dmsgmv( mModName << ".MevArbReqList()" );
}

/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
G4_MevArbtr::MevArbReqList::~MevArbReqList( void )
{
  dmsgmv( mModName << ".~MevArbReqList()" );
}

// override functions
/// <summary>get other request</summary>
/// <param name="pIf">interface pointer</param>
/// <returns>request pointer</returns>
OSCI2ARBTR<BUS_WIDTH_VCI32>::BusArbRequest*
G4_MevArbtr::MevArbReqList::getOtherRequest( TlmBwTransportIf* pIf )
{
  dmsgmv( mModName << ".getOtherRequest( 0x" << pIf << " )" );
  char           errMsg[128];
  bool           hit_flag = false;
  unsigned int   req_num  = 0x0;
  unsigned int   priorPe  = 0x0;
  unsigned int   priorVl  = 0xFFFFFFFF;
  BusArbRequest* priorRq  = 0x0;

  if ( mpPriorityList->size() != mPeNum ) {
    dmsgmv( "  mpPriorityList->size():0x" << mpPriorityList->size() << " mPeNum:" << mPeNum );
    sprintf( errMsg, "Priority setting has not been completed." );
    printErrorMessage( "MevArbReqList", "getOtherRequest", errMsg );
  }
  else {
    dmsgmv( "  mRequestList.size():0x" << mRequestList.size() );
    std::vector<BusArbRequest*>::iterator it = mRequestList.begin();
    while( mRequestList.end() != it ) {
      dmsgmv( "  doneFlag:" << (*it)->doneFlag << " reqTime:" << (*it)->reqTime );
      if ( !((*it)->doneFlag) && ((*it)->reqTime <= sc_time_stamp().value()) ) {
        TlmG3mExtension* pG3mEx;
        (*it)->pTransaction->get_extension( pG3mEx );
        unsigned int peid = pG3mEx->getPeId();
        if ( mPeNum <= peid ) {
          sprintf( errMsg, "Invalid PEID(%d).", peid );
          printErrorMessage( "MevArbReqList", "getOtherRequest", errMsg );
        }
        else {
          unsigned int pri = (*mpPriorityList)[peid];
          req_num++;
          if ( priorVl > pri ) {
            priorPe  = peid;
            priorVl  = pri;
            priorRq  = *it;
            hit_flag = true;
          }
          else {
            // do nothing
          }
        }
      }
      else {
        // do nothing
      }
      it++;
    }
  }

  if ( hit_flag ) {
    dmsgmv( "  pri:" << priorVl << " peid:" << priorPe << "  num:" << req_num );
    return( priorRq );
  }
  else {
    return( (BusArbRequest*)0 );
  }
}


// set function
/// <summary>set number of PE</summary>
/// <param name="peNum">number of PE</param>
/// <returns>none</returns>
void
G4_MevArbtr::MevArbReqList::setPeNum( unsigned int peNum )
{
  dmsgmv( mModName << ".setPeNum( " << peNum << " )" );
  mPeNum = peNum;
}

/// <summary>set pointer to priority-list</summary>
/// <param name="priorityList">pointer to priority-list</param>
/// <returns>none</returns>
void
G4_MevArbtr::MevArbReqList::setPriorityList(
  std::map<unsigned int, unsigned int>* priorityList )
{
  dmsgmv( mModName << ".setPriorityList()" );
  mpPriorityList = priorityList;
}



/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <returns>none</returns>
G4_MevArbtr::G4_MevArbtr( sc_module_name name ):
  OSCI2ARBTR<BUS_WIDTH_VCI32>( name ),
  mPeRequestList( name )
{
  dmsgmv( this->name() << ".G4_MevArbtr()" );
  mPeRequestList.reset();
  mPeRequestList.setPriorityList( &mPriorityList );
}

/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
G4_MevArbtr::~G4_MevArbtr( void )
{
  dmsgmv( this->name() << ".~G4_MevArbtr()" );
}


// set function
/// <summary>set number of PE</summary>
/// <param name="peNum">number of PE</param>
/// <returns>none</returns>
void
G4_MevArbtr::setPeNum( unsigned int peNum )
{
  dmsgmv( this->name() << ".setPeNum( " << peNum << " )" );
  mPeNum = peNum;
  mPeRequestList.setPeNum( peNum );
}

/// <summary>set priority of arbitration</summary>
/// <param name="peid">PEID</param>
/// <param name="priority">priority of arbitration</param>
/// <returns>none</returns>
void
G4_MevArbtr::setPriority( unsigned int peid, unsigned int priority )
{
  dmsgmv( this->name() << ".setPriority( " << peid << ", " << priority << " )" );

  if ( mPeNum <= peid ) {
    sprintf( mErrMsg, "Invalid PEID(%d).", peid );
    printErrorMessage( "MevArbReqList", "setPriority", mErrMsg );
  }
  else {
    map<unsigned int, unsigned int>::iterator it = mPriorityList.begin();
    while( it != mPriorityList.end() ) {
      if ( priority == it->second ) {
        sprintf( mErrMsg, "Priority(%d) has been set to PEID(%d) already.", priority, it->first );
        printWarningMessage( this->name(), "setPriority", mErrMsg );
        return;
      }
      else {
        it++;
      }
    }

    if ( (mPriorityList.end() != mPriorityList.find(peid)) ) { // already exist
      sprintf( mErrMsg, "PEID(%d) has been set priority already.", peid );
      printWarningMessage( this->name(), "setPriority", mErrMsg );
    }
    else { // not exist
      mPriorityList.insert( pair<unsigned int, unsigned int>(peid, priority) );
    }
  }
}

/// <summary>remove priority of arbitration</summary>
/// <param name="peid">PEID</param>
/// <returns>none</returns>
void
G4_MevArbtr::rmvPriority( unsigned int peid )
{
  dmsgmv( this->name() << ".rmvPriority( " << peid << " )" );
  if ( mPriorityList.end() != mPriorityList.find(peid) ) { // already exist
    mPriorityList.erase( peid );
  }
  else { // not exist
    sprintf( mErrMsg, "PEID(%d) has not been set priority yet.", peid );
    printWarningMessage( this->name(), "rmvPriority", mErrMsg );
  }
}

/// <summary>clear priority of arbitration</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
G4_MevArbtr::clrPriority( void )
{
  dmsgmv( this->name() << ".clrPriority()" );
  mPriorityList.clear();
}

// override functions
/// <summary>transport API for forward pass of AT</summary>
/// <param name="trans">transaction</param>
/// <param name="phase">TLM phase</param>
/// <param name="t">time</param>
/// <returns>synchronization information</returns>
TlmSyncEnum
G4_MevArbtr::nb_transport_fw(
  TlmTransactionType& trans,
  TlmPhase&           phase,
  sc_core::sc_time&   t )
{
  dmsgmv( this->name() << ".nb_transport_fw()" );
  TlmG3mExtension *pG3mEx = 0;
  trans.get_extension( pG3mEx );

  if( !pG3mEx ){
    sprintf( mErrMsg,
             "Received transaction doesn't support the TlmG3mExtension." );
    printErrorMessage( this->name(), "nb_transport_fw", mErrMsg );
    return tlm::TLM_ACCEPTED;
  }
  else {
    if( phase != tlm::BEGIN_REQ ){
      sprintf( mErrMsg,
               "Phase was wrong at nb_transport_fw() calling. It was not BEGIN_REQ." );
      printErrorMessage( this->name(), "nb_transport_fw", mErrMsg );
    }
    else {
      if ( NO_BUS_ID == mPeRequestList.getBusIdListIndex( pG3mEx->getIfPointer() ) ) {
        mPeRequestList.mBusIdList.push_back( pG3mEx->getIfPointer() );
      }
      else {
        // do nothing
      }

      mPeRequestList.registerList( &trans, &phase, &t, pG3mEx );

      if( ( mpRequest == 0 ) ||
          ( (mpRequest != 0) && !mWaitForBw ) ){
        dmsgmv( "  mpRequest:" << mpRequest << " mWaitForBw:" << mWaitForBw << " mPeriod:" << mPeriod );
        mTransportEvent.notify(sc_time((double)mPeriod,glb_resolution_unit));
      }
      else {
        // do nothing
      }

      trans.set_response_status(tlm::TLM_OK_RESPONSE);
      phase = tlm::END_REQ;
    }
  }

  return tlm::TLM_UPDATED;
}

/// <summary>transport API for backward pass of AT</summary>
/// <param name="trans">transaction</param>
/// <param name="phase">TLM phase</param>
/// <param name="t">time</param>
/// <returns>synchronization information</returns>
TlmSyncEnum
G4_MevArbtr::nb_transport_bw(
  TlmTransactionType& trans,
  TlmPhase&           phase,
  sc_core::sc_time&   t )
{
  dmsgmv( this->name() << ".nb_transport_bw()" );
  mWaitForBw = false;
  mTransportEvent.notify();
  return ts->nb_transport_bw( trans, phase, t );
}

/// <summary>arbitration</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
G4_MevArbtr::arbitrate( void )
{
  dmsgmv( this->name() << ".arbitrate()" );
  BusArbRequest* request = 0;
  TlmBwTransportIf* firstBusId = 0;

  if ( mPeRequestList.getBusIdListSize() ) {
    if( ( mOwnerId != (TlmBwTransportIf*)0 ) && ( mPrevPeId != (TlmBwTransportIf*)0 ) ){
      if( mTargetLock == true ){
        request = mPeRequestList.getRequest( mOwnerId );
      }
      else{
        request = mPeRequestList.getOtherRequest( mPrevPeId );
      }
    }
    else if ( mTargetLock == false ) {
      request = mPeRequestList.getRequest( firstBusId );
      if ( (( request == 0 ) || 
           (( request != 0 ) && request->doneFlag )) )
      request = mPeRequestList.getOtherRequest( firstBusId );
    }
    else {
      // do nothing
    }

    if ( mTargetLock == false ) {
      if ( ( request == 0 ) || 
          (( request != 0 ) && request->doneFlag ) ) {
        mPrevPeId = (TlmBwTransportIf*)0;
      }
      else {
        mPrevPeId = request->pIfPointer;
      }
    }
    else {
      // do nothing
    }
  }

  if ( mTargetLock == false ) {
    mOwnerId = ( request==0 ) ? (TlmBwTransportIf*)0 : request->pIfPointer;
  }

  if ( (request != 0) && request->doneFlag ) {
    mpRequest = 0;
  }
  else {
    mpRequest = request;
  }

  mTargetLock = ( request==0 ) ? mTargetLock : request->targetLock;
}

// override processes
/// <summary>reset process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
G4_MevArbtr::resetProcess( void )
{
  dmsgmv( this->name() << ".resetProcess()" );
  mPeRequestList.reset();
  OSCI2ARBTR<BUS_WIDTH_VCI32>::resetProcess();
}

/// <summary>arbitration</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
G4_MevArbtr::actionAtClockPos( void )
{
  dmsgmv( this->name() << ".actionAtClockPos()" );
  if ( mPeRequestList.isExistRequest() ) {
    arbitrate();
  }
  else {
    if (mTargetLock == false ) {
      mOwnerId = (TlmBwTransportIf*)0;
    }
    else {
      // do nothing
    }
  }

  if( (mpRequest != 0) && mpRequest->pTransaction &&
     !(mpRequest->doneFlag) ) {   // This "if" sentence is for reset input
    mpRequest->phase = tlm::BEGIN_REQ;
    mpRequest->pTransaction->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );
    is->nb_transport_fw( *(mpRequest->pTransaction),
                         mpRequest->phase,
                         mpRequest->t );
    mpRequest->doneFlag = true;
    mWaitForBw = true;
    dmsgmv( "  mpRequest->doneFlag: " << mpRequest->doneFlag );
  }
  else {
    // do nothing
  }
}
