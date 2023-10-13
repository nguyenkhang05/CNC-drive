/*************************************************************************
 *
 *  G4_PEGERR_OR.h
 *
 *  Copyright(c) 2017 Renesas Electronics Corporation
 *  RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 *  This program must be used solely for the purpose for which
 *  it was furnished by Renesas Electronics Corporation. No part of this
 *  program may be reproduced or disclosed to others, in any
 *  form, without the prior written permission of Renesas Electronics
 *  Corporation.
 *
 *************************************************************************/

#ifndef G4_PEGERR_OR_H
#define G4_PEGERR_OR_H

#include "systemc.h"
#include "global.h"
#include "G4_PEG_ERR_INFO.h"

extern sc_time_unit glb_resolution_unit;

class G4_PEGERR_OR : public sc_module
{
 private: 
  class ErrInAdapter : public sc_module{
   public:
    G4_PEGERR_OR              *mpG4PegerrOr; // pointer of G4_PEGERR_OR's instance
    sc_in<G4_PEG_Cerrinfo>     err_info_in;  // for error info of PEG

    SC_HAS_PROCESS(ErrInAdapter);
    ErrInAdapter( sc_module_name module_name );
    ~ErrInAdapter();
    void ErrProcess(void);
    void SetPegerrOrPtr( G4_PEGERR_OR* pPegerrOrPtr );
  };

  class ErrOutAdapter : public sc_module{
   public:
    G4_PEGERR_OR               *mpG4PegerrOr;   // pointer of G4_PEGERR_OR's instance
    bool                        mIsErrSet;      // error flag (true:finish to output the error info, false:not finish to output the error info)
    unsigned int                mBmId;          // Bus Master ID
    sc_out<G4_PEG_Cerrinfo>     err_info_out;   // for error info of PEG
    sc_event                    err_out_event;  // for event to output the error info to PEGCAP_M
    
    SC_HAS_PROCESS(ErrOutAdapter);
    ErrOutAdapter( sc_module_name module_name );
    ~ErrOutAdapter();
    void ErrProcess(void);
    void SetPegerrOrPtr(G4_PEGERR_OR* pPegerrOrPtr);
    void SetBmId(unsigned int bmid);
  };

 public:
  sc_in<sc_dt::uint64>     *PE_sys_clk[PE_MAX_NUM];    // for PE's clock port
  sc_in<G4_PEG_Cerrinfo>   *PE_PegInfoIn[PE_MAX_NUM];  // for input the error information to Master CPU
  sc_out<G4_PEG_Cerrinfo>  *PE_PegInfoOut[PE_MAX_NUM]; // for output the error information to Master CPU

  G4_PEG_Cerrinfo  *mpPegInfo[PE_MAX_NUM];         // for store the PEG error info
  double mPeriodPE[PE_MAX_NUM];                    // for PE's cycle period

  ErrInAdapter*  PE_PegInfoInAdapter[PE_MAX_NUM];  // for the pointer of ErrInAdapter's instance
  ErrOutAdapter* PE_PegInfoOutAdapter[PE_MAX_NUM]; // for the pointer of ErrOutAdapter's instance

 private:

 public:
  SC_HAS_PROCESS(G4_PEGERR_OR);
  G4_PEGERR_OR( sc_module_name module_name );
  ~G4_PEGERR_OR();

  void ChangeFreqPE0(void);
  void ChangeFreqPE1(void);
  void ChangeFreqPE2(void);
  void ChangeFreqPE3(void);
  void ChangeFreqPE4(void);
  void ChangeFreqPE5(void);
  void ChangeFreqPE6(void);
  void ChangeFreqPE7(void);

 private:
  double periodCal( sc_dt::uint64 new_freq);

};

#endif

