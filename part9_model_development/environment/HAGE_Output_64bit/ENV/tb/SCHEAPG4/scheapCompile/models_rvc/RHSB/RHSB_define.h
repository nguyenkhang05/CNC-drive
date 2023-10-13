
// ---------------------------------------------------------------------
// $Id: RHSB_define.h,v 1.3 2018/08/02 08:08:50 ngathunguyen Exp $
//
// Copyright(c) 2016 - 2018 Renesas Electronics Corporation
// Copyright(c) 2016 - 2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __RHSB_DEFINE_H__
#define __RHSB_DEFINE_H__

namespace rhsb_def {
    enum eResetNumber {emPRESETn=0, emrstc_n, emResetSignalNum};//Define number of reset signal
    enum eOperationState {emRESET=0, emCONFIG, emACTIVE, emTEST};//Define operation states
    enum eInterruptId {emINTDTSF=0, emINTDTF, emINTCTF, emINTTSF, emINTL2, emINTL3, emINTUEF, emINTTOF, 
        emINTDLF, emDownDataDMA, emDownCommandDMA, emUpDMA, emInterruptNum};//Define interrupt port id
    enum eChannelNumber {emSlave0Channel=0, emSlave1Channel, emSlaveChannelNum};//Define channel number

};//rhsb_def namespace

#endif//end of RHSB_DEFINE
