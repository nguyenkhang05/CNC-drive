/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include "ipir.h"
const uint32_t TRegIpir::N_G4MH_CH      =     4;
const uint32_t TRegIpir::IPIEN_OFFSET   = 0x800;
const uint32_t TRegIpir::IPIFLG_OFFSET  = 0x804;
const uint32_t TRegIpir::IPIFCLR_OFFSET = 0x808;

const uint32_t TRegIpir::IPIREQ_OFFSET  = 0x810;
const uint32_t TRegIpir::IPIRCLR_OFFSET = 0x814;
