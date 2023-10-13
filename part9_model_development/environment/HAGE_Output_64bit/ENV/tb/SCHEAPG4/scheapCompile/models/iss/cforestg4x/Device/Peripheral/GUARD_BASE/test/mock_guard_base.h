/**
 * @file mock_guard_base.h
 *
 * @date Created on: 2016
 * @author Author: Quang Lam <quang.lam.px@rvc.renesas.com>
 * @brief UNIT TEST for Guard Base. (SAMPLE)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#pragma once
#include <vector>
#include <map>
#include "gmock/gmock.h"
#include "forest_common.h"


class MockGuardBase {
public:
    class MockGuardAddrKey {
    public:
        GuardModuleID m_gmid;
        uint32_t      m_id;
        bool operator < (const MockGuardAddrKey& rhs) const {
            if (m_gmid < rhs.m_gmid) {
                return true;
            }
            if (m_gmid > rhs.m_gmid) {
                return false;
            }
            if (m_id < rhs.m_id) {
                return true;
            }
            if (m_id > rhs.m_id) {
                return false;
            }
            return false;
        }
        MockGuardAddrKey (GuardModuleID gmid, uint32_t id)
          : m_gmid (gmid), m_id (id) {}
        ~MockGuardAddrKey () {}
    };
    typedef std::pair<PhysAddr, bool> MockGuardAddrVal;
    std::map<MockGuardAddrKey, MockGuardAddrVal> m_guard_addr;

public:
    MockGuardBase(uint32_t pe_id);
    uint32_t GetPEID(uint32_t sideband);

    PhysAddr GetGuardAddr (GuardModuleID guard_id, uint32_t location_id);
    void SetGuardAddr (GuardModuleID guard_id, uint32_t location_id, PhysAddr addr);

    uint32_t m_peid = 0;
};
