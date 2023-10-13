/*
 * (c) 2011,2012 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once

#include "./forest_common.h"
#include "./forest_message.h"
#include <vector>


class WRegLine {
private:
    std::vector<RegData*> m_line;
    uint32_t m_element_num;

public:
    static const uint32_t FXU4_HALF = 0;
    static const uint32_t FXU4_FULL = 1;

    void SetPtr (uint32_t pos, RegData* ptr) { m_line[pos] = ptr; }
    RegData* GetPtr (uint32_t pos) const { return m_line[pos]; }

    void SetData (uint32_t pos, RegData d) {
        if (m_line[pos] != NULL) {
            *(m_line[pos]) = d;
        } else {
            CfMsg::DPrint (MSG_INF, "Fail to Write Data\n");
        }
    }
    RegData GetData (uint32_t pos) const {
        if (m_line[pos] != NULL) {
            return *(m_line[pos]);
        } else {
            return 0;
        }
    }
    void ResetData (void) {
        for (uint32_t i = 0; i < m_element_num; i++) *(m_line[i]) = 0x0;
    }

    WRegLine (uint32_t line_size) {
        m_element_num = 1 << (line_size + 1);
        if (line_size == FXU4_FULL) {
            for (uint32_t i = 0; i < m_element_num; i++) m_line.push_back (new RegData);
        } else if (line_size == FXU4_HALF) {
            for (uint32_t i = 0; i < m_element_num; i++) m_line.push_back (new RegData);
            for (uint32_t i = 0; i < m_element_num; i++) m_line.push_back (NULL);
        }
    }
    ~WRegLine (void) {
        for (uint32_t i = 0; i < m_element_num; i++) delete (m_line[i]);
        m_line.clear ();
    }
};


class WRegDataBase {
private:
    union {
        uint64_t m_d64[2];
        uint32_t m_d32[4];
        float    m_f[4];
    };
    uint32_t m_way_num;

public:
    WRegDataBase (uint32_t way_num) : m_way_num (way_num) {}
    WRegDataBase (uint32_t way_num, uint32_t d32) : m_way_num (way_num) { m_d32[0] = d32; }
    WRegDataBase (uint32_t way_num, uint64_t d64) : m_way_num (way_num) { m_d64[0] = d64; }
    WRegDataBase (uint32_t way_num, uint64_t d64_h, uint64_t d64_l) : m_way_num (way_num) {
        m_d64[1] = d64_h;
        m_d64[0] = d64_l;
    }
    ~WRegDataBase (void) {}

    uint32_t GetWayNum (void) { return m_way_num; }

    void Set64 (uint32_t pos, uint64_t d) { m_d64[pos] = d; }
    void Set32 (uint32_t pos, uint32_t d) { m_d32[pos] = d; }
    void SetFloat (uint32_t pos, float f) { m_f[pos] = f; }

    uint64_t Get64 (uint32_t pos) const { return m_d64[pos]; }
    uint32_t Get32 (uint32_t pos) const { return m_d32[pos]; }
    float GetFloat (uint32_t pos) const { return m_f[pos]; }
};


class WRegData32 : public WRegDataBase {
public:
    WRegData32 (void) : WRegDataBase (1) {}
    explicit WRegData32 (uint32_t d32) : WRegDataBase (1, d32) {}
    ~WRegData32 (void) {}
};


class WRegData64 : public WRegDataBase {
public:
    WRegData64 (void) : WRegDataBase (2) {}
    explicit WRegData64 (uint64_t d64) : WRegDataBase (2, d64) {}
    ~WRegData64 (void) {}
};


class WRegData128 : public WRegDataBase {
public:
    WRegData128 (void) : WRegDataBase (4) {}
    explicit WRegData128 (uint64_t d64_h, uint64_t d64_l) : WRegDataBase (4, d64_h, d64_l) {}
    ~WRegData128 (void) {}
};
