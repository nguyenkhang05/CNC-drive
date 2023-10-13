/**
 * @file mock_guard_base.cpp
 *
 * @date Created on: 2016
 * @author Author: Quang Lam <quang.lam.px@rvc.renesas.com>
 * @brief UNIT TEST for Guard Base. (SAMPLE)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */

#include <vector>
#include <map>
#include "./config.h"
#include "./forest_common.h"
#include "./mock_guard_base.h"

// To the definition of the mock-based class of the minimum.
// Description only the methods required for the unit test.

MockGuardBase::MockGuardBase(uint32_t pe_id)
    : m_peid(pe_id) {}

PhysAddr MockGuardBase::GetGuardAddr (GuardModuleID guard_id, uint32_t location_id)
{
    if ((guard_id == GMID_CFG) || (guard_id == GMID_CFGCAP_SX2FX)) {
        location_id = 0;
    }
    std::map<MockGuardAddrKey, MockGuardAddrVal>::iterator it
        = m_guard_addr.find (MockGuardAddrKey (guard_id, location_id));
    if (it != m_guard_addr.end ()) {
        return it->second.first;
    } else {
        return 0; // error
    }
}


void MockGuardBase::SetGuardAddr (GuardModuleID guard_id, uint32_t location_id, PhysAddr addr)
{
    if ((guard_id == GMID_CFG) || (guard_id == GMID_CFGCAP_SX2FX)) {
        location_id = 0;
    }
    std::map<MockGuardAddrKey, MockGuardAddrVal>::iterator it
        = m_guard_addr.find (MockGuardAddrKey (guard_id, location_id));
    if (it != m_guard_addr.end ()) {
        // overwrite
        it->second.first = addr;
    } else {
        // newly add
        m_guard_addr.insert (std::map<MockGuardAddrKey, MockGuardAddrVal>::value_type (
        		MockGuardAddrKey (guard_id, location_id), MockGuardAddrVal (addr, false))
        );
    }
}

uint32_t MockGuardBase::GetPEID(uint32_t sideband)
{
    return ((sideband >> 3) & 0x1f);
}
