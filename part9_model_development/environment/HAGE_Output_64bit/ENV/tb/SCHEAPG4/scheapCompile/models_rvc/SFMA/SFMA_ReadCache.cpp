// ---------------------------------------------------------------------
// $Id: SFMA_ReadCache.cpp,v 1.3 2020/01/30 08:01:36 huepham Exp $
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
#include "SFMA_ReadCache.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
SFMA_ReadCache::SFMA_ReadCache()
{//{{{
    Initialize();

    // Set all zero
    for (int i = 0; i < emEntryNumber; i++) {
        mAddrArray[i] = 0;
        mDataArray[i] = 0;
    }
}//}}}

/// Destructor
SFMA_ReadCache::~SFMA_ReadCache()
{//{{{
}//}}}

/// Reset ReadCache
void SFMA_ReadCache::EnableReset(const bool is_active)
{//{{{
    if (is_active) {
        Initialize();
    }
}//}}}

/// Initialize internal variables
void SFMA_ReadCache::Initialize(void)
{//{{{
    FlushCache();
}//}}}

/// Flush Cache
void SFMA_ReadCache::FlushCache(void)
{//{{{
    for (int i = 0; i < emEntryNumber; i++) {
        mAddrArray[i] &= ~(1u << (unsigned int)emVBitPos);
    }

    mWriteIndex = 0;
}//}}}

/// Get Data
bool SFMA_ReadCache::GetData(unsigned int address, uint64_t *pData)
{//{{{
    bool isAvailable = false;

    for (int i = 0; i < emEntryNumber; i++) {
        // Address matched and data is valid
        if ((address | (1u << (unsigned int)emVBitPos)) == mAddrArray[i]) {
            isAvailable = true;
            if (pData != NULL){
                *pData = mDataArray[i];
                break;
            }
        }
    }

    return isAvailable;
}//}}}

/// Store Data
void SFMA_ReadCache::StoreData(unsigned int address, uint64_t data)
{//{{{
    if(mWriteIndex >= emEntryNumber) {
        mWriteIndex = 0;
    }

    mAddrArray[mWriteIndex] = address | (1u << (unsigned int)emVBitPos);
    mDataArray[mWriteIndex] = data;

    mWriteIndex++;
}//}}}

// vim600: set foldmethod=marker :
