// ---------------------------------------------------------------------
// $Id: LPS_def.h,v 1.2 2020/01/13 02:56:08 hadinhtran Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __LPS_DEF_H__
#define __LPS_DEF_H__
/// Enums
enum eClockInfo{                  // Define info of clock signal
     emCLK_LSBId   = 0            // ID of CLK_LSB
    ,emCLKA_LPSId  = 1            // ID of CLKA_LPS
    ,emNumClk      = 2            // Total number of clock
};                                
                                  
enum eOtherInfo{                  // Define number of other variables
     emNumDPSEL           = 3     // Number of DPSEL output port
    ,emNumDPIN            = 24    // Number of DPIN output port
    ,emNumINTTAUx         = 4     // Value of index x of mINTTAUJyIx [0:3]
    ,emNumINTTAUy         = 2     // Value of index y of mINTTAUJyIx [0:1]
    ,emStartIndexINTTAUy  = 2     // Index y of name INTTAUJyIx port (y = 2, 3)
    ,emMaxInputReadTime   = 8     // Maximum times input port DPINm can be read/ recieve
    ,emNumINT_AD          = 5     // Number of channel of ADC [4:0]
};

static const std::string mTimesString[8] = {"First", "Second", "Third", "Fourth", "Fifth", "Sixth", "Seventh", "Eighth"};
#endif //__LPS_DEF_H__
