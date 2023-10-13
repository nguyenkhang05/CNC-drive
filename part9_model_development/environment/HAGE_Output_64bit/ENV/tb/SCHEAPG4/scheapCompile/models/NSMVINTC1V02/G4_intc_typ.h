/*
 * $Id: G4_intc_typ.h,v 1.5 2020/02/24 06:19:36 hadinhtran Exp $
 * Copyright(c) 2016-2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#ifndef G4_INTC_TYP_H
#define G4_INTC_TYP_H

#include "systemc.h"

namespace G4IntcTyp {

/*****************************************************
  MSB                               LSB
  31 15      12    11    10     1     0
  [ 0 ][ gpid ][ bg ][ gm ][ ch ][ rq ]
  17bit  3bit   1bit  1bit  9bit  1bit
******************************************************/
union FeintReq
{
  unsigned int val;
  struct {
    unsigned int  req  :  1;
    unsigned int  ch   :  9;
    unsigned int  gm   :  1; // added by yoshinaga for G4MH 2.0
    unsigned int  bg   :  1; // added by yoshinaga for G4MH 2.0
    unsigned int  gpid :  3; // added by yoshinaga for G4MH 2.0
    unsigned int       : 17; // changed by yoshinaga 22->17 for G4MH 2.0
  } bit_val;

  FeintReq( void ): val( 0 ) {}
  FeintReq( unsigned int value ): val( value ) {}
};

/*****************************************************
  MSB          LSB
  31  10    1      0
  [ 0 ][ ch ][ ack ]
  22bit 9bit  1bit
******************************************************/
union FeintAck
{
  unsigned int val;
  struct {
    unsigned int  ack  :  1;
    unsigned int  ch   :  9; 
    unsigned int       : 22;
  } bit_val;

  FeintAck( void ): val( 0 ) {}
  FeintAck( unsigned int value ): val( value ) {}
};

/*****************************************************
  MSB                                                    LSB
  63 50    42      39    38    37      21    20     12     1     0
  [ 0 ][bgpr][ gpid ][ bg ][ gm ][ bind ][ tb ][ pri ][ ch ][ rq ]
  22bit 8bit   3bit   1bit  1bit  16bit   1bit  8bit  11bit  1bit
******************************************************/
union EiintReq
{
  uint64 val;
  struct {
    uint64  req  :  1;
    uint64  ch   : 11;
    uint64  pri  :  8; // changed by yoshinaga 4->8 for G4MH 2.0
    uint64  tb   :  1;
    uint64  bind : 16;
    uint64  gm   :  1; // added by yoshinaga for G4MH 2.0
    uint64  bg   :  1; // added by yoshinaga for G4MH 2.0
    uint64  gpid :  3; // added by yoshinaga for G4MH 2.0
    uint64  bgpr :  8; // added by chanle for G4MH2.0 to convey bgpr bit from INTC2 to INTC1. This bgpr is updated into mpIntc2Dummy register.
    uint64       : 14; 
  } bit_val;

  EiintReq( void ): val( 0 ) {}
  EiintReq( uint64 value ): val( value ) {}
};

/*****************************************************
  MSB                          LSB
  15  15   14       8    3       0
  [ et ][ 0 ][ tcid ][ 0 ][ peid ]
   1bit  1bit  6bit   5bit  3bit
******************************************************/
union EiintReqBind
{
  unsigned int val;
  struct {
    unsigned int  peid :  3;
    unsigned int       :  5;
    unsigned int  tcid :  6;
    unsigned int       :  1;
    unsigned int  et   :  1;
    unsigned int       : 16;
  } bit_val;

  EiintReqBind( void ): val( 0 ) {}
  EiintReqBind( unsigned int value ): val( value ) {}
};


/*****************************************************
  MSB            LSB
  31 10     1      0
  [ 0 ][ ch ][ ack ]
  20bit 11it  1bit
******************************************************/
union EiintAck
{
  unsigned int val;
  struct {
    unsigned int  ack :  1;
    unsigned int  ch  : 11;
    unsigned int      : 20;
  } bit_val;
  
  EiintAck( void ): val( 0 ) {}
  EiintAck( unsigned int value ): val( value ) {}
};


// added by Yoshinaga for U2A -->
/*****************************************************
  MSB                  LSB
  31  4           1         0
  [ 0 ][ cpu_gpid ][ cpu_gm ]
  28bit     3it       1bit
******************************************************/
union CpuGmInfo
{
  unsigned int val;
  struct {
    unsigned int  cpu_gm   :  1;
    unsigned int  cpu_gpid :  3;
    unsigned int           : 28;
  } bit_val;
  
  CpuGmInfo( void ): val( 0 ) {}
  CpuGmInfo( unsigned int value ): val( value ) {}
};
// <-- added by Yoshinaga for U2A 


enum REG_OFFSET_ADDR_T {
  INTC1_EIC_OFFSET    = 0x000,
  INTC2_EIC_OFFSET    = 0x040,
  INTC1_IMR_OFFSET    = 0x0F0,
  INTC2_IMR_OFFSET    = 0x1004,
  INTC1_EIBD_OFFSET   = 0x100,
  INTC2_EIBD_OFFSET   = 0x2080,
  INTC1_FNC_OFFSET    = 0x0A0,
  INTC1_FIC_OFFSET    = 0x0C0,
  INTC1_FNBD_OFFSET   = 0x1A0,
  INTC1_FIBD_OFFSET   = 0x1C0,
  INTC1_DBMK_OFFSET   = 0x080,
  INTC1_EEIC_OFFSET   = 0x200,
  INTC2_EEIC_OFFSET   = 0x4080,
  INTC1_EIBG_OFFSET   = 0x280,
  INTC2_I2EIBG_OFFSET = 0x1FE0,
  INTC1_FIBG_OFFSET   = 0x2C0,
  INTC1_IHVCFG_OFFSET = 0x2F0
};


// priority of processes.
// (the smaller value, the higher priority.)
enum PRI_PROC_T {
  PRI_RESET_PROC = 0,     // ResetProc() process      :INTC1, INTC2
  PRI_DBG_PROC,           // Debug operation process
  PRI_CANCEL_PROC,        // Cancel request process
  PRI_FNACK_PROC,         // ExFenmiAckProc() process :INTC1
  PRI_FNREQ_PROC,         // ExFenmiReqProc() process :INTC1
  PRI_FIACK_PROC,         // ExFeintAckProc() process :INTC1
  PRI_FIREQ_PROC,         // ExFeintReqProc() process :INTC1
  PRI_EIACK_PROC,         // EiintAckProc() process   :INTC2
  PRI_EIREQ_PROC,         // EiintReqProc() process   :INTC2
  PRI_EXEC_PROC,          // ExecProc() process       :INTC1, INTC2
  PRI_DEFAULT    = 0xFFFF // default(priority=lowest)
};


// to avoid the error which occurs when plural process write a same port.
// (provided port-write process, and managed by the priorities of drive processes.)
template <class T>
class  PortAccInfo {
public:
  unsigned int tcid;
  T            data;
  PRI_PROC_T   drvProcPri;
  PortAccInfo( void ):
    tcid( 0 ),
    data( 0 ),
    drvProcPri( PRI_DEFAULT )
  {}
  void clear( void ) {
    tcid = 0;
    data = static_cast<T>( 0 );
    drvProcPri = PRI_DEFAULT;
  }
};

} // namespace G4IntcTyp

#endif // G4_INTC_TYP_H
