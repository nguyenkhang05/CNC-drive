// ---------------------------------------------------------------------
// $Id: SFMA_ReadCache.h,v 1.2 2020/01/30 08:01:36 huepham Exp $
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
#ifndef __SFMA_READCACHE_H__
#define __SFMA_READCACHE_H__

#include <stdint.h>
#include <iostream>

class SFMA_ReadCache
{
public:
    SFMA_ReadCache();
    ~SFMA_ReadCache();

    void EnableReset(const bool is_active);
    void StoreData(unsigned int address, uint64_t data);
    bool GetData(unsigned int address, uint64_t *pData);
    void FlushCache(void);

private:
    // Entry number
    enum eEntryNumber {
        emEntryNumber = 16
    };

    // Valid bit position
    enum eVBitPos {
        emVBitPos = 29
    };

    // Function
    void Initialize(void);

    // Variables
    unsigned int    mAddrArray[emEntryNumber];
    uint64_t        mDataArray[emEntryNumber];
    unsigned int    mWriteIndex;
};
#endif //__SFMA_READCACHE_H__
