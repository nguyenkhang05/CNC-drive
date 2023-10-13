/*
 * (c) 2011-2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once

#include <stdint.h>
#include "./config.h"

//! @par SideBand class
//!   BitPos/BitOp are special member for CForest (There is no them in RTL.)\n
//!   BitPos and BitOP are for SC-HEAP module.\n
//!   Spe is for slave guard.\n
//!   [24:22] BitPos  0:bit0 ... 7:bit7 if BitOp == 001 || 010 || 011\n
//!   [21:19] BitOp   000:normal 001:SET 010:CLR 011:NOT, 100:CAXI\n
//!   [18]    SPCLD   0: instruction can issue normal load, 1: instruction can issue speculative load\n
//!   [16]    GM\n
//!   [15:13] GPID\n
//!   [12:8]  SPID\n
//!   [7:3]   BMID\n
//!   [2]     Secure  (Fixed to 0 in G4MH PE)\n
//!   [1]     DM      0: Normal mode, 1: Debug mode\n
//!   [0]     UM      User Mode\n


class SideBand {
private:
    union {
        uint32_t m_sideband;
        struct {
            uint32_t m_um     : 1;
            uint32_t m_dm     : 1;
            uint32_t m_secure : 1;
            uint32_t m_bmid   : 5;
            uint32_t m_spid   : 5;
            uint32_t m_gpid   : 3;
            uint32_t m_gm     : 1;
            uint32_t          : 1;
            uint32_t m_spcld  : 1;
            uint32_t m_bitop  : 3;
            uint32_t m_bitpos : 3;
            uint32_t          : 7;
        };
    };
    static const uint32_t SB_DEBUG = 0x00000002;

    explicit SideBand (uint32_t sideband) : m_sideband (sideband) {}

public:
    enum BitOpType {
        BITOP_NONE = 0x0,
        BITOP_SET  = 0x1,
        BITOP_CLR  = 0x2,
        BITOP_NOT  = 0x3,
        BITOP_CAXI = 0x4
    };

    void SetBitPos (uint32_t bitpos) { m_bitpos = bitpos & 0x7; }
    void SetBitOp (uint32_t bitop) { m_bitop = bitop & 0x7; }
    void SetSPCLD (uint32_t is_spcld) { m_spcld = is_spcld & 0x1; }
    void SetSecure (uint32_t is_sec) { m_secure = is_sec & 0x1; }
    void SetDM  (uint32_t dm)  { m_dm = dm & 0x1; }
    void SetUM  (uint32_t um)  { m_um = um & 0x1; }
    void SetBMID (uint32_t bmid) { m_bmid = bmid & 0x1F; }
    void SetSPID (uint32_t spid) { m_spid = spid & 0x1F; }
    void SetGPID (uint32_t gpid) { m_gpid = gpid & 0x7; }
    void SetGM (uint32_t gm) { m_gm = gm & 0x1; }

    uint32_t GetBitPos (void) const { return m_bitpos; }
    uint32_t GetBitOp  (void) const { return m_bitop; }
    uint32_t GetSPCLD  (void) const { return m_spcld; }
    uint32_t GetSecure (void) const { return m_secure; }
    uint32_t GetDM   (void) const { return m_dm; }
    uint32_t GetUM   (void) const { return m_um; }
    uint32_t GetBMID (void) const { return m_bmid; }
    uint32_t GetSPID (void) const { return m_spid; }
    uint32_t GetGPID (void) const { return m_gpid; }
    uint32_t GetGM (void) const { return m_gm; }
    uint32_t GetValue (void) const { return m_sideband; }

    bool IsDebugAccess (void) const { return GetDM() == 0x1; }
    static SideBand DebugAccess (void) { return SideBand(SB_DEBUG); }

    SideBand () : m_sideband (0x0) {}
    ~SideBand () {}
};
