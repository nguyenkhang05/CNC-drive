// ---------------------------------------------------------------------
// $Id: DFE_FIFO_Utils.cpp,v 1.2 2018/09/06 10:17:09 huepham Exp $
//
// Copyright(c) 2018 Renesas Electronics Corporation
// Copyright(c) 2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "DFE_FIFO_Utils.h"
#include <string>
using std::string;
#include <sstream>
using std::ostringstream;
//------------------------------------------------------------------------------
/**@brief constructor of reset control class
 * @details this function allocates and initializes internal states
 *          of reset sources
 * @param iRST_SRC_NUM number of reset source
 */
DFE_FIFO_Utils::CrstCtrl::CrstCtrl(const uint8_t& iRST_SRC_NUM)
: RST_SRC_NUM    (iRST_SRC_NUM)
, rstSrcActStates(NULL)
{//{{{
    sc_assert(RST_SRC_NUM > 0);
    rstSrcActStates = new bool[RST_SRC_NUM];
    sc_assert(rstSrcActStates != NULL);
    for (uint8_t rstIdx = 0; rstIdx < RST_SRC_NUM; ++rstIdx) {
        rstSrcActStates[rstIdx] = false;
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief destructor of reset control class
 * @details this function releases allocated memories
 */
DFE_FIFO_Utils::CrstCtrl::~CrstCtrl() 
{//{{{
    if (rstSrcActStates != NULL)
        delete [] rstSrcActStates;
}//}}}
//------------------------------------------------------------------------------
/**@brief constructor of clock control class
 * @details this allocates memories for arrays of frequencies, periods and
 *          start synchronization points of clock sources
 * @param iCLK_SRC_NUM number of clock sources
 */
DFE_FIFO_Utils::CclkCtrl::CclkCtrl(const uint8_t& iCLK_SRC_NUM)
: CLK_SRC_NUM (iCLK_SRC_NUM)
, pFrequencies(NULL)
, pPeriods    (NULL)
, pSyncPoints (NULL)
{//{{{
    sc_assert(CLK_SRC_NUM > 0);
    pFrequencies = new uint64_t[CLK_SRC_NUM];
    pPeriods     = new sc_time [CLK_SRC_NUM];
    pSyncPoints  = new sc_time [CLK_SRC_NUM];
    sc_assert(pFrequencies != NULL);
    sc_assert(pPeriods     != NULL);
    sc_assert(pSyncPoints  != NULL);
    for (uint8_t clkIdx = 0; clkIdx < CLK_SRC_NUM; ++clkIdx)
        setClkFreq(clkIdx, 0);
}//}}}
//------------------------------------------------------------------------------
DFE_FIFO_Utils::CclkCtrl::~CclkCtrl() 
{//{{{
    if (pFrequencies != NULL)
        delete[] pFrequencies;
    if (pPeriods != NULL)
        delete[] pPeriods;
    if (pSyncPoints != NULL)
        delete[] pSyncPoints;
}//}}}
// vim600: set foldmethod=marker :