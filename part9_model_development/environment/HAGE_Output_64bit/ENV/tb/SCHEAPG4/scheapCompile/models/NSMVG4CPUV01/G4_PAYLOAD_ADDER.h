/*************************************************************************
 *
 *  G4_PAYLOAD_ADDER.h
 *
 *  Copyright(c) 2016-2017 Renesas Electronics Corporation
 *  RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 *  This program must be used solely for the purpose for which
 *  it was furnished by Renesas Electronics Corporation. No part of this
 *  program may be reproduced or disclosed to others, in any
 *  form, without the prior written permission of Renesas Electronics
 *  Corporation.
 *
 *************************************************************************/

#ifndef G4_PAYLOAD_ADDER_H
#define G4_PAYLOAD_ADDER_H

#include "OSCI2.h"
#include "g4cpu_def.h"
#ifndef FASTISS_NO_SUPPORT
#include "rh850g4/core.h"       // for rh850g4::core class
#endif // #ifndef FASTISS_NO_SUPPORT

#ifndef FASTISS_NO_SUPPORT
using namespace rh850g4;    // for ASTC's fastiss
#endif // #ifndef FASTISS_NO_SUPPORT

template<unsigned int BUSWIDTH> class G4_PAYLOAD_ADDER : 
public sc_module,
virtual public TlmFwTransportIf,
virtual public TlmBwTransportIf
{

 public:
  TlmTargetSocket<BUSWIDTH>       ts;        // initaiator socket
  TlmInitiatorSocket<BUSWIDTH>    is;        // target socket
  sc_in<bool>                     sys_reset; // for reset port
  

  SC_HAS_PROCESS(G4_PAYLOAD_ADDER);

  /// <summary>constructor</summary>
  /// <param name="moduleName">name of sc_module</param>
  /// <param name="ptr">pointer of fast ISS's instance</param>
  G4_PAYLOAD_ADDER( sc_module_name moduleName, rh850g4::core* ptr):
    ts("ts"),
    is("is"),
    sys_reset("sys_reset"),
    mPeId(0),
    mLApbUpper(0xfffc3fff),
    mLApbLower(0xfffc0000),
    mpFastIss_ASTC(ptr),
    mIsUnderReset(false)
  {
    ts(*this);
    is(*this);
    mPreTrans = (TlmTransactionType *)0;

    SC_METHOD(reset);
    sensitive << sys_reset.neg();
    dont_initialize();

    SC_METHOD(reset_done);
    sensitive << mpFastIss_ASTC->reset_done_event;
    dont_initialize();
  }
    
  /// <summary>destructor</summary>
  ~G4_PAYLOAD_ADDER( void ){}
    
  ///<summary>reset operation</summary>
  ///<returns>none</returns>
  void reset(void)
  {
    if(!sys_reset.read()){
#if defined(TEST_RESET_DBG)
      printf("%s: G4_PAYLOAD_ADDER_%d::reset() : neg edge\n", 
	     sc_time_stamp().to_string().c_str(),
	     mPeId);
      fflush(stdout);
#endif
      mIsUnderReset = true;
    }
  }  

  ///<summary>reset release operation</summary>
  ///<returns>none</returns>
  void reset_done(void)
  {
#if defined(TEST_RESET_DBG)
    printf("%s: G4_PAYLOAD_ADDER_%d::reset_done()\n", 
	   sc_time_stamp().to_string().c_str(),
	   mPeId);
    fflush(stdout);
#endif
    mIsUnderReset = false;
  }  

  /// <summary>transport API for forward pass of AT</summary>
  /// <param name="trans">transaction</param>
  /// <param name="phase">TLM phase</param>
  /// <param name="t">time</param>
  /// <returns>synchronization information</returns>
  TlmSyncEnum nb_transport_fw( TlmTransactionType& trans,
			       TlmPhase& phase,
			       sc_core::sc_time& t )
  {
    // add extension payload
    // When fastiss supports the AT mode, 
    // you must add the opration to add the extension payload at transaction

    // add block operation of transaction
    // When fastiss supports the AT mode, 
    // you must add the opration to block the sending transaction
    // during the reset period
    
    // covert address for Local APB (self area -> PE area)
    sc_dt::uint64 addr = trans.get_address(); 
    if((mLApbLower <= addr) && (addr <= mLApbUpper)){
      trans.set_address(addr + (0x4000 * (mPeId+1)));
    }

    // clear the data for read transaction
    if(trans.is_read()){
      unsigned int data_length = trans.get_data_length();
      unsigned char* data = trans.get_data_ptr();
      for (unsigned int i=0; i<data_length; i++){
	data[i]=0;
      }
    }

    is->nb_transport_fw( trans, phase, t );
    trans.set_address(addr);
    return tlm::TLM_UPDATED;
  }
  
  /// <summary>transport API for LT</summary>
  /// <param name="trans">transaction</param>
  /// <param name="t">time</param>
  /// <returns>none</returns>
  void b_transport( TlmTransactionType& trans, sc_core::sc_time& t )
  {
#if defined(TEST_RESET_DBG)
    sc_dt::uint64  dbg_addr   = trans.get_address();
    unsigned int   dbg_size   = trans.get_data_length();
    printf("%s: G4_PAYLOAD_ADDER_%d::b_transport() is called : address=0x%08x, size=%d, t=%s  :",
	   sc_time_stamp().to_string().c_str(),
	   mPeId,
	   (unsigned int)dbg_addr,
	   dbg_size,
	   t.to_string().c_str());
    fflush(stdout);
#endif
#if defined(BUSIF_TEST_LOG)
    sc_dt::uint64  bus_dbg_addr   = trans.get_address();
    printf("%s: G4_PAYLOAD_ADDER_%d::b_transport() is called : address=0x%08x\n",
	   sc_time_stamp().to_string().c_str(),
	   mPeId,
	   (unsigned int)bus_dbg_addr);
    fflush(stdout);
#endif

    if(mIsUnderReset){
      // ignore the transaction during the reset period
#if defined(TEST_RESET_DBG)
      printf(" UNDER reset period\n");
      fflush(stdout);
#endif
      return;
    }

#if defined(TEST_RESET_DBG)
    printf(" NOT reset period\n");
    fflush(stdout);
#endif

    // add extension payload
    sc_dt::uint64 addr = trans.get_address();
    bool isSpcLd = false;
    if(trans.is_read()){
      if(((LRAM_INIT_BASE - (LRAM_INIT_SIZE * PE_MAX_NUM)) <= addr) && (addr < (LRAM_INIT_BASE + LRAM_INIT_SIZE))){
	isSpcLd = true;
      }
    }

    TlmG3mExtension *g3m_ext = (TlmG3mExtension *)0;
    trans.get_extension( g3m_ext );
    if( !g3m_ext ){
      g3m_ext = new TlmG3mExtension();
      trans.set_extension( g3m_ext );
      setTransG3m( *g3m_ext, false, isSpcLd );
    } else{
      setTransG3m( *g3m_ext, false, isSpcLd );
    }      

    TlmVpiExtension *vpi_ext = (TlmVpiExtension *)0;
    trans.get_extension( vpi_ext );
    if( !vpi_ext ){
      trans.set_extension( new TlmVpiExtension() );
    }

    TlmVciExtension *vci_ext = (TlmVciExtension *)0;
    trans.get_extension( vci_ext );
    if( !vci_ext ){
      vci_ext = new TlmVciExtension();
      trans.set_extension( vci_ext );
      setTransVci( *vci_ext );
    }else{
      setTransVci( *vci_ext );
    }

    TlmApbExtension *apb_ext = (TlmApbExtension *)0;
    trans.get_extension( apb_ext);
    if( !apb_ext ){
      trans.set_extension( new TlmApbExtension() );
    }

    TlmAxiExtension *axi_ext = (TlmAxiExtension *)0;
    trans.get_extension( axi_ext);
    if( !axi_ext ){
      trans.set_extension( new TlmAxiExtension() );
    }

    // covert address for Local APB (self area -> PE area)
    if((mLApbLower <= addr) && (addr <= mLApbUpper)){
      trans.set_address(addr + (0x4000 * (mPeId+1)));
    }

    // clear the data for read transaction
    if(trans.is_read()){
      unsigned int data_length = trans.get_data_length();
      unsigned char* data = trans.get_data_ptr();
      for (unsigned int i=0; i<data_length; i++){
	data[i]=0;
      }
    }

    is->b_transport( trans, t );
    trans.set_address(addr);
    return;
  }
  
  /// <summary>transport API for debug access</summary>
  /// <param name="r">transaction</param>
  /// <returns>data number</returns>
  unsigned int transport_dbg( TlmTransactionType& r )
  {
#if defined(TEST_RESET_DBG)
    sc_dt::uint64  dbg_addr   = r.get_address();
    unsigned int   dbg_size   = r.get_data_length();
    printf("%s: G4_PAYLOAD_ADDER_%d::transport_dbg() is called : address=0x%08x, size=%d  :",
	   sc_time_stamp().to_string().c_str(),
	   mPeId,
	   (unsigned int)dbg_addr,
	   dbg_size);
    if(mIsUnderReset){
      printf(" UNDER reset period\n");
    }else{
      printf(" NOT reset period\n");
    }
    fflush(stdout);
#endif

    // add extension payload
    TlmG3mExtension *g3m_ext = (TlmG3mExtension *)0;
    r.get_extension( g3m_ext);
    if( !g3m_ext ){
      g3m_ext = new TlmG3mExtension();
      r.set_extension( g3m_ext );
      setTransG3m( *g3m_ext, true, false );
    } else{
      setTransG3m( *g3m_ext, true, false );
    }

    TlmVpiExtension *vpi_ext = (TlmVpiExtension *)0;
    r.get_extension( vpi_ext);
    if( !vpi_ext ){
      r.set_extension( new TlmVpiExtension() );
    }

    TlmVciExtension *vci_ext = (TlmVciExtension *)0;
    r.get_extension( vci_ext);
    if( !vci_ext ){
      r.set_extension( new TlmVciExtension() );
    }

    TlmApbExtension *apb_ext = (TlmApbExtension *)0;
    r.get_extension( apb_ext);
    if( !apb_ext ){
      r.set_extension( new TlmApbExtension() );
    }

    TlmAxiExtension *axi_ext = (TlmAxiExtension *)0;
    r.get_extension( axi_ext);
    if( !axi_ext ){
      r.set_extension( new TlmAxiExtension() );
    }

    // covert address for Local APB (self area -> PE area)
    sc_dt::uint64 addr = r.get_address();
    if((mLApbLower <= addr) && (addr <= mLApbUpper)){
      r.set_address(addr + (0x4000 * (mPeId+1)));
    }

    // clear the data for read transaction
    if(r.is_read()){
      unsigned int data_length = r.get_data_length();
      unsigned char* data = r.get_data_ptr();
      for (unsigned int i=0; i<data_length; i++){
	data[i]=0;
      }
    }

    unsigned int ret = is->transport_dbg( r );
    r.set_address(addr);
    return(ret);
  }
  
  /// <summary>Dummy function (DMI access is not supported)</summary>
  /// <param name="trans">transaction</param>
  /// <param name="dmi_data">permissible access(read or(and) write)</param>
  /// <returns>accept or reject</returns>
  bool get_direct_mem_ptr( TlmTransactionType& trans,
			   tlm::tlm_dmi&  dmi_data)
  {
    return false;
  }

  /// <summary>Dummy function (DMI access is not supported)</summary>
  /// <returns>none</returns>
  void invalidate_dmi_method(){}

  /// <summary>transport API for backward pass of AT</summary>
  /// <param name="trans">transaction</param>
  /// <param name="phase">TLM phase</param>
  /// <param name="t">time</param>
  /// <returns>synchronization information</returns>
  TlmSyncEnum nb_transport_bw( TlmTransactionType& trans,
			       TlmPhase& phase,
			       sc_core::sc_time& t )
  {
    TlmG3mExtension *pG3mExtension = NULL;
    trans.get_extension( pG3mExtension );
    
    if( !pG3mExtension ){
      // error (not support TlmG3mExtension)
      char errorMessageBuf[1024];
      sprintf( errorMessageBuf,
	       "Received transaction doesn't support the TlmG3mExtension." );
      CmErrMsg::printErrorMessage( this->name(), 0, errorMessageBuf );
      return tlm::TLM_ACCEPTED;
    }
    else{
      return(pG3mExtension->getIfPointer()->nb_transport_bw(trans, phase, t));
    }
  }
  
  /// <summary>Dummy function (DMI access is not supported)</summary>
  /// <param name="start_range">start range for DMI access</param>
  /// <param name="end_range">end rang for DMI access</param>
  /// <returns>none</returns>
  void invalidate_direct_mem_ptr( sc_dt::uint64 start_range,
				  sc_dt::uint64 end_range )
  {}

  /// <summary>set local APB self area</summary>
  /// <param name="upper">upper address of local APB self area</param>
  /// <param name="lower">lower address of local APB self area</param>
  /// <returns>none</returns>
  void SetLApbSelfArea(unsigned int upper, unsigned int lower)
  {
    mLApbUpper = upper;
    mLApbLower = lower;
  }

  /// <summary>set transaction information to the G3M's extention payload</summary>
  /// <param name="g3mExt">G3M's extention payload</param>
  /// <param name="isDbgAcc">access mode(true:debug access, false:normal access)</param>
  /// <param name="isSpcLd">speculation load flag(true:speculation load, false:not speculation load)</param>
  /// <returns>none</returns>
  void setTransG3m( TlmG3mExtension &g3mExt, bool isDbgAcc, bool isSpcLd )
  {
    g3mExt.setDmaAccess( false );
    unsigned int vmid, tcid, spid;
    bool userMode, virtualMode;
    mpFastIss_ASTC->get_g3m_params(vmid, tcid, spid, userMode, virtualMode);
    g3mExt.setVmId(vmid);
    g3mExt.setTcId(tcid);
    g3mExt.setSpId(spid);
    g3mExt.setUserMode(userMode);
    g3mExt.setVirtualMode(virtualMode);
    g3mExt.setSpcLd(isSpcLd);
    if(isDbgAcc){
      g3mExt.setPeId( DEBUG_MASTER_BMID );
    }else{
      g3mExt.setPeId( mPeId );
    }
#ifdef BUSIF_TEST_LOG
    printf("%s: G4_PAYLOAD_ADDER::setTransG3m: mSpcLd=%d, mVirtualMode=%d, mVmId=0x%x\n", sc_time_stamp().to_string().c_str(), g3mExt.isSpcLd(), g3mExt.isVirtualMode(), g3mExt.getVmId());
    fflush(stdout);
#endif
  }

  /// <summary>set transaction information to the VCI's extention payload</summary>
  /// <param name="vciExt">VCI's extention payload</param>
  /// <returns>none</returns>
  void setTransVci( TlmVciExtension &vciExt )
  {
    unsigned int acc_type_hi = mpFastIss_ASTC->get_mem_acc_type();
    unsigned int acc_type_lo = acc_type_hi & 0x3;
    acc_type_hi = acc_type_hi & 0x1c;

    if(acc_type_hi == 0x0){
      vciExt.setRequestType(TlmVciExtension::CMD_READ);
    }else if(acc_type_hi == 0x4){
      vciExt.setRequestType(TlmVciExtension::CMD_WRITE);
    }else if(acc_type_hi == 0x8){
      if(acc_type_lo == 0x0){
	vciExt.setRequestType(TlmVciExtension::CMD_BITOP_SET);
      }else if(acc_type_lo == 0x1){
	vciExt.setRequestType(TlmVciExtension::CMD_BITOP_AND);
      }else if(acc_type_lo == 0x2){
	vciExt.setRequestType(TlmVciExtension::CMD_BITOP_OR);
      }else{ // if(acc_type_lo == 0x3
	vciExt.setRequestType(TlmVciExtension::CMD_BITOP_XOR);
      }
    }else if(acc_type_hi == 0xc){
      if(acc_type_lo == 0x2){
	vciExt.setRequestType(TlmVciExtension::CMD_SYNC_MASTER);
      }else{
	vciExt.setRequestType(TlmVciExtension::CMD_SYNC_BUS);
      }
    }else if(acc_type_hi == 0x10){
      vciExt.setRequestType(TlmVciExtension::CMD_CAXI);
    }else if(acc_type_hi == 0x14){
      if(acc_type_lo == 0x0){
	vciExt.setRequestType(TlmVciExtension::CMD_EXCL_LDLW);
      }else if(acc_type_lo == 0x1){
	vciExt.setRequestType(TlmVciExtension::CMD_EXCL_STCW);
      }else{
	// internal error
      }
    }else{
      // internal error
    }
  }

  /// <summary>set PEID</summary>
  /// <param name="peid">PEID</param>
  void setPeId(unsigned int peid)
  {
    mPeId = peid;
  }

 private:
  rh850g4::core   *mpFastIss_ASTC; // pointer of fast ISS instance
  unsigned int    mPeId;          // for PE ID
  unsigned int    mLApbUpper;     // for Local APB address (upper)
  unsigned int    mLApbLower;     // for Local APB address (lower)
  bool            mIsUnderReset; // reset status flag(true:under reset, false:not reset)

  TlmTransactionType *mPreTrans;
};

#endif

