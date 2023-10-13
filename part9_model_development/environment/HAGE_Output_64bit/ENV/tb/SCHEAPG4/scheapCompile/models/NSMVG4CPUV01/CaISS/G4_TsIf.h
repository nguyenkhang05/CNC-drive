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

#ifndef G4_TSIF_H
#define G4_TSIF_H

#include "BusSlaveFuncIf.h"
#include "OSCI2.h"

class G4_CaIss;

class G4_TsIf : public BusSlaveFuncIf
{
  public:
    G4_TsIf( G4_CaIss *pCaIss );
    ~G4_TsIf( void );

    void read( unsigned int     offsetAddress, 
               TlmBasicPayload& trans, 
               sc_time*         t,
               bool             debug=false);

    void write( unsigned int     offsetAddress, 
                TlmBasicPayload& trans, 
                sc_time*         t,
                bool             debug=false);

    void setfunc( ADDRESS_TYPE size );

  private:
    G4_CaIss *mpCaIss;               // for the pointer of CaIss's instance

}; // end of TsIf definition

#endif /* G4_TSIF_H */
