/*************************************************************************
 *
 *  G4_PEGERR_OR.cpp
 *
 * Copyright(c) 2017-2018, 2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

/* $Id:$ */

#include <stdio.h>   // for sprintf
#include "G4_PEGERR_OR.h"

/**************************************/
/*                                    */
/*  G4_PEGERR_OR::ErrInAdapter class  */
/*                                    */
/**************************************/

///<summary>Constructor</summary>
///<param name="module_name">module name</param>
///<returns>none</returns>
G4_PEGERR_OR::
ErrInAdapter::ErrInAdapter(sc_module_name module_name ) :
  mpG4PegerrOr(NULL),
  err_info_in("err_info_in")
{
  SC_METHOD( ErrProcess );
  sensitive << err_info_in;
  dont_initialize();
}

///<summary>Destructor</summary>
///<returns>none</returns>
G4_PEGERR_OR::
ErrInAdapter::~ErrInAdapter( void )
{
}


///<summary>Check PEG error info</summary>
///<returns>none</returns>
void G4_PEGERR_OR::
ErrInAdapter::ErrProcess( void )
{
  G4_PEG_Cerrinfo peg_err = err_info_in.read();

  if(peg_err.BMID >= PE_MAX_NUM) return;

  if((peg_err.PEGRdERR != false) ||
     (peg_err.PEGWrERR != false) ||
     (peg_err.SPID != 0) ||
     (peg_err.BMID != 0) ||
     (peg_err.ErrAddr != 0) ||
     (peg_err.UM  != false) ||
     (peg_err.DBG != false)){
    mpG4PegerrOr->mpPegInfo[peg_err.BMID]->PEGRdERR = peg_err.PEGRdERR;
    mpG4PegerrOr->mpPegInfo[peg_err.BMID]->PEGWrERR = peg_err.PEGWrERR;
    mpG4PegerrOr->mpPegInfo[peg_err.BMID]->SPID     = peg_err.SPID;
    mpG4PegerrOr->mpPegInfo[peg_err.BMID]->BMID     = peg_err.BMID;
    mpG4PegerrOr->mpPegInfo[peg_err.BMID]->ErrAddr  = peg_err.ErrAddr;
    mpG4PegerrOr->mpPegInfo[peg_err.BMID]->UM       = peg_err.UM;
    mpG4PegerrOr->mpPegInfo[peg_err.BMID]->DBG      = peg_err.DBG;
    // event
    mpG4PegerrOr->PE_PegInfoOutAdapter[peg_err.BMID]->err_out_event.notify();
#ifdef TEST_CORE_DBG
    printf("%s %s:G4_PEGERR_OR::ErrInAdapter::ErrProcess : BMID=%d, read error=%d, address=0x%08x\n", sc_time_stamp().to_string().c_str(), this->name(), peg_err.BMID, peg_err.PEGRdERR, peg_err.ErrAddr);
    fflush(stdout);
#endif
  }
}


///<summary>set pointer of G4_PEGERR_OR's instance</summary>
///<param name="pPegerrOrPtr">pointer of G4_PEGERR_OR's instance</param>
///<returns>none</returns>
void G4_PEGERR_OR::
ErrInAdapter::SetPegerrOrPtr( G4_PEGERR_OR* pPegerrOrPtr )
{
  mpG4PegerrOr = pPegerrOrPtr;
}



/***************************************/
/*                                     */
/*  G4_PEGERR_OR::ErrOutAdapter class  */
/*                                     */
/***************************************/

///<summary>Constructor</summary>
///<param name="module_name">module name</param>
///<returns>none</returns>
G4_PEGERR_OR::
ErrOutAdapter::ErrOutAdapter(sc_module_name module_name ) :
  mpG4PegerrOr(NULL),
  mIsErrSet(false),
  mBmId(0),
  err_info_out("err_info_out"),
  err_out_event()
{
  SC_METHOD( ErrProcess );
  sensitive << err_out_event;
  dont_initialize();
}

///<summary>Destructor</summary>
///<returns>none</returns>
G4_PEGERR_OR::
ErrOutAdapter::~ErrOutAdapter( void )
{
}


///<summary>Output PEG error info</summary>
///<returns>none</returns>
void G4_PEGERR_OR::
ErrOutAdapter::ErrProcess( void )
{
  if (!mIsErrSet){
#ifdef TEST_CORE_DBG
    printf("%s %s:G4_PEGERR_OR::ErrOutAdapter::ErrProcess(output the error) : BMID=%d\n", sc_time_stamp().to_string().c_str(), this->name(), mBmId);
    fflush(stdout);
#endif
    mIsErrSet = true;
    err_info_out.write(*(mpG4PegerrOr->mpPegInfo[mBmId]));
    err_out_event.notify(mpG4PegerrOr->mPeriodPE[mBmId], glb_resolution_unit);
  }else{
#ifdef TEST_CORE_DBG
    printf("%s %s:G4_PEGERR_OR::ErrOutAdapter::ErrProcess(clear the error) : BMID=%d\n", sc_time_stamp().to_string().c_str(), this->name(), mBmId);
    fflush(stdout);
#endif
    mpG4PegerrOr->mpPegInfo[mBmId]->initialize();
    mpG4PegerrOr->mpPegInfo[mBmId]->BMID = mBmId;
    err_info_out.write(*(mpG4PegerrOr->mpPegInfo[mBmId]));
    mIsErrSet = false;
  }
}


///<summary>set pointer of G4_PEGERR_OR's instance</summary>
///<param name="pPegerrOrPtr">pointer of G4_PEGERR_OR's instance</param>
///<returns>none</returns>
void G4_PEGERR_OR::
ErrOutAdapter::SetPegerrOrPtr( G4_PEGERR_OR* pPegerrOrPtr )
{
  mpG4PegerrOr = pPegerrOrPtr;
}


///<summary>set Bus Master ID</summary>
///<param name="bmid">Bus Master ID</param>
///<returns>none</returns>
void G4_PEGERR_OR::
ErrOutAdapter::SetBmId(unsigned int bmid)
{
  mBmId = bmid;
}



/***************************************/
/*                                     */
/*         G4_PEGERR_OR class          */
/*                                     */
/***************************************/

///<summary>Constructor</summary>
///<param name="module_name">module name</param>
///<param name="config_file">file name of configulation file</param>
///<returns>none</returns>
G4_PEGERR_OR::G4_PEGERR_OR(sc_module_name module_name )
{
  char port_name[64];
  char mod_name[64];

  for(int i=0; i<PE_MAX_NUM; i++){
    sprintf(port_name, "PE%d_sys_clk", i);
    PE_sys_clk[i] = new sc_in<sc_dt::uint64>(port_name);
    sprintf(port_name, "PE%d_PegInfoIn", i);
    PE_PegInfoIn[i] = new sc_in<G4_PEG_Cerrinfo>(port_name);
    sprintf(port_name, "PE%d_PegInfoOut", i);
    PE_PegInfoOut[i] = new sc_out<G4_PEG_Cerrinfo>(port_name);
    mpPegInfo[i] = new G4_PEG_Cerrinfo();

    sprintf(mod_name, "PE%d_PegInfoInAdapter", i);
    PE_PegInfoInAdapter[i] = new ErrInAdapter(mod_name);
    PE_PegInfoInAdapter[i]->SetPegerrOrPtr(this);
    sprintf(mod_name, "PE%d_PegInfoOutAdapter", i);
    PE_PegInfoOutAdapter[i] = new ErrOutAdapter(mod_name);
    PE_PegInfoOutAdapter[i]->SetPegerrOrPtr(this);
    PE_PegInfoOutAdapter[i]->SetBmId(i);

    // connection
    (PE_PegInfoInAdapter[i]->err_info_in)(*((PE_PegInfoIn)[i]));
    (PE_PegInfoOutAdapter[i]->err_info_out)(*((PE_PegInfoOut)[i]));

    mPeriodPE[i] = +HUGE_VAL;
  }

  SC_METHOD(ChangeFreqPE0);
  sensitive << *(PE_sys_clk)[0];

  SC_METHOD(ChangeFreqPE1);
  sensitive << *(PE_sys_clk)[1];

  SC_METHOD(ChangeFreqPE2);
  sensitive << *(PE_sys_clk)[2];

  SC_METHOD(ChangeFreqPE3);
  sensitive << *(PE_sys_clk)[3];

  SC_METHOD(ChangeFreqPE4);
  sensitive << *(PE_sys_clk)[4];

  SC_METHOD(ChangeFreqPE5);
  sensitive << *(PE_sys_clk)[5];

  SC_METHOD(ChangeFreqPE6);
  sensitive << *(PE_sys_clk)[6];

  SC_METHOD(ChangeFreqPE7);
  sensitive << *(PE_sys_clk)[7];


}


///<summary>Destructor</summary>
///<returns>none</returns>
G4_PEGERR_OR::~G4_PEGERR_OR( void )
{
  for(int i=0; i<PE_MAX_NUM; i++){
    delete PE_sys_clk[i];
    delete PE_PegInfoIn[i];
    delete PE_PegInfoOut[i];
    delete mpPegInfo[i];
    delete PE_PegInfoInAdapter[i];
    delete PE_PegInfoOutAdapter[i];
  }
}

///<summary>change the PE0's cycle period and set it to the debugger I/F</summary>
///<returns>none</returns>
void G4_PEGERR_OR::ChangeFreqPE0(void)
{
  mPeriodPE[0] = periodCal((sc_dt::uint64)(*PE_sys_clk[0]).read());
}

///<summary>change the PE1's cycle period and set it to the debugger I/F</summary>
///<returns>none</returns>
void G4_PEGERR_OR::ChangeFreqPE1(void)
{
  mPeriodPE[1] = periodCal((sc_dt::uint64)(*PE_sys_clk[1]).read());
}

///<summary>change the PE2's cycle period and set it to the debugger I/F</summary>
///<returns>none</returns>
void G4_PEGERR_OR::ChangeFreqPE2(void)
{
  mPeriodPE[2] = periodCal((sc_dt::uint64)(*PE_sys_clk[2]).read());
}

///<summary>change the PE3's cycle period and set it to the debugger I/F</summary>
///<returns>none</returns>
void G4_PEGERR_OR::ChangeFreqPE3(void)
{
  mPeriodPE[3] = periodCal((sc_dt::uint64)(*PE_sys_clk[3]).read());
}

///<summary>change the PE4's cycle period and set it to the debugger I/F</summary>
///<returns>none</returns>
void G4_PEGERR_OR::ChangeFreqPE4(void)
{
  mPeriodPE[4] = periodCal((sc_dt::uint64)(*PE_sys_clk[4]).read());
}

///<summary>change the PE5's cycle period and set it to the debugger I/F</summary>
///<returns>none</returns>
void G4_PEGERR_OR::ChangeFreqPE5(void)
{
  mPeriodPE[5] = periodCal((sc_dt::uint64)(*PE_sys_clk[5]).read());
}

///<summary>change the PE6's cycle period and set it to the debugger I/F</summary>
///<returns>none</returns>
void G4_PEGERR_OR::ChangeFreqPE6(void)
{
  mPeriodPE[6] = periodCal((sc_dt::uint64)(*PE_sys_clk[6]).read());
}

///<summary>change the PE7's cycle period and set it to the debugger I/F</summary>
///<returns>none</returns>
void G4_PEGERR_OR::ChangeFreqPE7(void)
{
  mPeriodPE[7] = periodCal((sc_dt::uint64)(*PE_sys_clk[7]).read());
}

///<summary>calculate the cycle period from input value (clock frequency)</summary>
///<param name="new_freq">clock frequency</param>
///<returns>cycle period</returns>
double G4_PEGERR_OR::periodCal( sc_dt::uint64 new_freq)
{
  double period = +HUGE_VAL;
  if (new_freq == 0){
    return (+HUGE_VAL);
  }
  switch(glb_resolution_unit){
  case SC_SEC:
    period = (double)((sc_dt::uint64)(1.0)/new_freq);
    break;
  case SC_MS:
    period = (double)((sc_dt::uint64)(1.0e+3)/new_freq);
    break;
  case SC_US:
    period = (double)((sc_dt::uint64)(1.0e+6)/new_freq);
    break;
  case SC_NS:
    period = (double)((sc_dt::uint64)(1.0e+9)/new_freq);
    break;
  case SC_PS:
    period = (double)((sc_dt::uint64)(1.0e+12)/new_freq);
    break;
  case SC_FS:
    period = (double)((sc_dt::uint64)(1.0e+15)/new_freq);
    break;
  }
  return(period);
}

