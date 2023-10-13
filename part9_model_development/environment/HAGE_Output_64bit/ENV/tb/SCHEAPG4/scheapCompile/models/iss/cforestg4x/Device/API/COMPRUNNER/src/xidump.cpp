#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

#include "./config.h"
#include "./xidump.h"
#include "./forest_utility.h"
#include "./fs_controler.h"

bool XDUMP::IsXDumpArith (uint32_t insn_id) {
    XDumpArithVec::const_iterator it = m_xdump_arith.begin();
    while (it != m_xdump_arith.end()) {
        if ((*it)->m_insn_id == insn_id) {
            XDumpArithEle.m_insn_id = (*it)->m_insn_id;
            XDumpArithEle.m_diff = (*it)->m_diff;
            return true;
        }
        ++ it;
    }
    return false;
}

bool XDUMP::IsXDumpAddrEmpty () {
    return m_xdump_addr.empty() ;
}

bool XDUMP::IsXDumpAddr (PhysAddr load_addr) {
    XDumpAddrVec::const_iterator it = m_xdump_addr.begin();
    XDumpAddrEle.m_xdump_mask = DEFAULT_MASK;
    while (it != m_xdump_addr.end()) {
        if ( ((*it)->m_start_addr <= load_addr) && ( ((*it)->m_end_addr) >= load_addr)) {
            XDumpAddrEle.m_xdump_mask = XDumpAddrEle.m_xdump_mask | (*it)->m_xdump_mask;
            if (XDumpAddrEle.m_xdump_mask == WORD_MASK) {
                return IS_XDUMP_ADDR;
            }
        }
        ++ it;
    }
    if (XDumpAddrEle.m_xdump_mask == DEFAULT_MASK){
            return NOT_XDUMP_ADDR;
    }
    else {
            return IS_XDUMP_ADDR;
    }
}

bool XDUMP::IsUpdateXDumpAddr (CReg regnum, uint32_t nblk_id) {
    XDumpAddrVec::const_iterator it = m_xdump_addr.begin();
    while (it != m_xdump_addr.end()) {
        if ( ((*it)->m_xdump_regno == regnum) && ((*it)->m_nblk_id == nblk_id) ) {
            XDumpAddrEle.m_xdump_mask = (*it)->m_xdump_mask;
            XDumpAddrEle.m_start_addr = (*it)->m_start_addr;
            XDumpAddrEle.m_end_addr   = (*it)->m_end_addr;
            XDumpAddrEle.m_store_type = (*it)->m_store_type;
            return IS_XDUMP_ADDR;
        }
        ++ it;
    }
    return NOT_XDUMP_ADDR;
}

bool XDUMP::AddXDumpAddr (PhysAddr store_addr, ByteSize store_size, CReg regnum, uint32_t nblk_id) {
    XDumpAddrVec::const_iterator it = m_xdump_addr.begin();
    XDUMP_ADDR * p_xdump_addr = NULL;
    uint32_t mask = DEFAULT_MASK;
    uint32_t shift_offset = UNUSED_VAL;
    uint32_t start_addr = UNUSED_VAL;
    uint32_t end_addr = UNUSED_VAL;
    switch (store_size) {
        case SIZE_8BIT:
            shift_offset = store_addr & BYTE_SHIFT_MASK;
            start_addr = store_addr & WORD_ALIGN_MASK;
            end_addr = start_addr + END_ADDR_OFFSET;
            mask = BYTE_MASK << shift_offset*BYTE_SIZE;
        break;
        case SIZE_16BIT:
            shift_offset = (store_addr >> 1) & HWORD_SHIFT_MASK;
            start_addr = store_addr & WORD_ALIGN_MASK;
            end_addr = start_addr + END_ADDR_OFFSET;
            mask = HWORD_MASK << shift_offset*HWORD_SIZE;
        break;
        case SIZE_32BIT:
            start_addr = store_addr & WORD_ALIGN_MASK;
            end_addr = start_addr + END_ADDR_OFFSET;
            mask = WORD_MASK;
        break;
    }
    while (it != m_xdump_addr.end()) {
        if ( (*it)->m_start_addr == start_addr) {
            (*it)->m_xdump_mask = (*it)->m_xdump_mask & (~mask);
            if ( (*it)->m_xdump_mask == DEFAULT_MASK ) {
                m_xdump_addr.erase(it);
            }
            else {
                ++it;
            }
        }
        else {
            ++it;
        }
    }
    if (it == m_xdump_addr.end() ){
        p_xdump_addr = new XDUMP_ADDR(start_addr, end_addr, store_size, regnum, nblk_id, mask);
        m_xdump_addr.push_back(p_xdump_addr);
    }
    return true;
}
void XDUMP::DelXDumpAddr (CReg regnum, uint32_t nblk_id) {
    XDumpAddrVec::const_iterator it = m_xdump_addr.begin();
    while (it != m_xdump_addr.end()) {
        if ( ((*it)->m_xdump_regno == regnum) && ((*it)->m_nblk_id == nblk_id) ) {
            m_xdump_addr.erase(it);
            break;
        }
        ++it;
    }
}

// delete by store mem by stable register
void XDUMP::DelXDumpAddr (PhysAddr store_addr, ByteSize store_size) {
    XDumpAddrVec::const_iterator it = m_xdump_addr.begin();
    uint32_t mask = DEFAULT_MASK;
    uint32_t shift_offset = UNUSED_VAL;
    uint32_t start_addr = UNUSED_VAL;
    switch (store_size) {
        case SIZE_8BIT:
            shift_offset = store_addr & BYTE_SHIFT_MASK;
            start_addr = store_addr & WORD_ALIGN_MASK;
            mask = BYTE_MASK << shift_offset*BYTE_SIZE;
        break;
        case SIZE_16BIT:
            shift_offset = (store_addr >> 1) & HWORD_SHIFT_MASK;
            start_addr = store_addr & WORD_ALIGN_MASK;
            mask = HWORD_MASK << shift_offset*HWORD_SIZE;
        break;
        case SIZE_32BIT:
            start_addr = store_addr & WORD_ALIGN_MASK;
            mask = WORD_MASK;
        break;
    }
    while (it != m_xdump_addr.end()) {
        if ( (*it)->m_start_addr == start_addr ) {
            (*it)->m_xdump_mask = (*it)->m_xdump_mask & (~mask);
            if ( (*it)->m_xdump_mask == DEFAULT_MASK ) {
                it = m_xdump_addr.erase(it);
            }
            else {
                ++it;
            }
        }
        else {
            ++it;
        }
    }
}

bool XDUMP::IsXDumpReg (uint32_t load_addr) {
    XDumpRegVec::const_iterator it = m_xdump_reg.begin();
    while (it != m_xdump_reg.end()) {
        if ((*it)->m_start_addr <= load_addr && (*it)->m_end_addr >= load_addr) {
            return true;
        }
        ++ it;
    }
    return false;
}

bool XDUMP::IsXDumpFlag (uint32_t load_addr) {
    XDumpFlagVec::const_iterator it = m_xdump_flag.begin();
    while (it != m_xdump_flag.end()) {
        if ((*it)->m_start_addr <= load_addr && (*it)->m_end_addr >= load_addr) {
            return true;
        }
        ++ it;
    }
    return false;
}

bool XDUMP::IsXDumpData (uint32_t load_addr) {
    XDumpDataVec::const_iterator it = m_xdump_data.begin();
    while (it != m_xdump_data.end()) {
        if ((*it)->m_start_addr <= load_addr && (*it)->m_end_addr >= load_addr) {
            XDumpDataEle.m_mask = (*it)->m_mask;
            return true;
        }
        ++ it;
    }
    return false;
}

bool XDUMP::IsXDumpSysreg (CReg regid) {
    static const uint32_t REC_SREG_OFFSET = 0x00000100;
    static const bool XDUMP_SYSREG = true;
    static const bool NOT_XDUMP_SYSREG = false;
    CReg cforest_regno;
    XDumpSysregVec::const_iterator it = m_xdump_sysreg.begin();
    while ( it != m_xdump_sysreg.end()) {
        cforest_regno = REC_SREG_OFFSET + ( (*it)->m_sel_id << 5 ) + (*it)->m_reg_id;
        if ( regid == cforest_regno ) {
            XDumpSysregEle.m_mask = (*it)->m_mask;
            return XDUMP_SYSREG;
        }
        ++it;
    }
    return NOT_XDUMP_SYSREG;
}

#if defined(COMP_RUNNER_G3KH)
bool XDUMP::IsXDumpFpuMin (uint32_t insn_id) {
    XDumpFpuMinVec::const_iterator it = m_xdump_fpumin.begin();
    while (it != m_xdump_fpumin.end()) {
        if ((*it)->m_insn_id == insn_id) {
            XDumpFpuMinEle.m_insn_id = (*it)->m_insn_id;
            return true;
        }
        ++ it;
    }
    return false;
}

void IDUMP::SetFPIState(uint32_t pe_id, ContextId tc_id, bool fpi_nc) {
    uint32_t scid = m_forest_util->GetScId (pe_id, tc_id);

    ForestUtil::HtVec::iterator it_s = m_forest_util->m_cedar.begin();
    ForestUtil::HtVec::iterator it_d = m_forest_util->m_cedar.end();

    while (it_s != it_d) {
        if ((*it_s)->GetScId() == scid) {
            //(*it_s)->SetFPIState(fpi_nc);
        }
        ++it_s;
    }
}


void IDUMP::SetINTState(uint32_t pe_id, ContextId tc_id, bool int_nc) {
    uint32_t scid = m_forest_util->GetScId (pe_id, tc_id);

    ForestUtil::HtVec::iterator it_s = m_forest_util->m_cedar.begin();
    ForestUtil::HtVec::iterator it_d = m_forest_util->m_cedar.end();

    while (it_s != it_d) {
        if ((*it_s)->GetScId() == scid) {
            (*it_s)->SetINTState(int_nc);
        }
        ++it_s;
    }
}


void IDUMP::SetINTState(uint32_t pe_id, ContextId tc_id, bool int_nc,
        uint32_t pri, uint32_t ch) {
    uint32_t scid = m_forest_util->GetScId (pe_id, tc_id);

    ForestUtil::HtVec::iterator it_s = m_forest_util->m_cedar.begin();
    ForestUtil::HtVec::iterator it_d = m_forest_util->m_cedar.end();

    while (it_s != it_d) {
        if ((*it_s)->GetScId() == scid) {
            (*it_s)->SetINTState(int_nc, pri, ch);
        }
        ++it_s;
    }
}


void IDUMP::SetFEINTState(uint32_t pe_id, ContextId tc_id, bool int_nc) {
    uint32_t scid = m_forest_util->GetScId (pe_id, tc_id);

    ForestUtil::HtVec::iterator it_s = m_forest_util->m_cedar.begin();
    ForestUtil::HtVec::iterator it_d = m_forest_util->m_cedar.end();

    while (it_s != it_d) {
        if ((*it_s)->GetScId() == scid) {
            (*it_s)->SetFEINTState(int_nc);
        }
        ++it_s;
    }
}


void IDUMP::SetSYSERRState(uint32_t pe_id, ContextId tc_id, bool int_nc) {
    uint32_t scid = m_forest_util->GetScId (pe_id, tc_id);

    ForestUtil::HtVec::iterator it_s = m_forest_util->m_cedar.begin();
    ForestUtil::HtVec::iterator it_d = m_forest_util->m_cedar.end();

    while (it_s != it_d) {
        if ((*it_s)->GetScId() == scid) {
            (*it_s)->SetSYSERRState(int_nc);
        }
        ++it_s;
    }
}


void IDUMP::SetSYSERRIFState(uint32_t pe_id, ContextId tc_id, bool int_nc) {
    uint32_t scid = m_forest_util->GetScId (pe_id, tc_id);

    ForestUtil::HtVec::iterator it_s = m_forest_util->m_cedar.begin();
    ForestUtil::HtVec::iterator it_d = m_forest_util->m_cedar.end();

    while (it_s != it_d) {
        if ((*it_s)->GetScId() == scid) {
            (*it_s)->SetSYSERRIFState(int_nc);
        }
        ++it_s;
    }
}


bool IDUMP::IDumpValidate(uint32_t type, std::ostringstream *oss, uint32_t pri, uint32_t ch) {

    bool result = false;

    switch (type) {
    case brk_async_ld_stdw_nm_ack:
        (*oss) << "Unknown[type=" << type << "][brk_async_ld_stdw_nm_ack]>";
        break;
    case brk_async_rmw_nm_ack:
        (*oss) << "Unknown[type=" << type << "][brk_async_rmw_nm_ack]>";
        break;
    case eh_relay_brk_nm_ack:
        (*oss) << "Unknown[type=" << type << "][eh_relay_brk_nm_ack]>";
        break;
    case dbnmi_nt_ack:
        (*oss) << "Unknown[type=" << type << "][dbnmi_nt_ack]>";
        break;
    case dbint_nt_ack:
        (*oss) << "Unknown[type=" << type << "][dbint_nt_ack]>";
        break;
    case rmint_nt_ack:
        (*oss) << "Unknown[type=" << type << "][rmint_nt_ack]>";
        break;
    case cpu_fenmiack_1l_nt:
        (*oss) << "FENMI[NC]>";
        result = true;
        break;
    case syserr_pin_ack_nt:
        (*oss) << "SYSERR[NC]>";
        result = true;
        break;
    case cpu_feintack_1l_nt:
        (*oss) << "FEINT[NC]>";
        result = true;
        break;
    case cpu_intack_1l_nt:
        (*oss) << "EIINT[NC] PRI[" << pri << "] CH[" << ch << "]>";
        result = true;
        break;
    case ehu_ack_fpi_fpu_nt:
        (*oss) << "FPI[NC]>";
        result = true;
        break;
    case syserr_re_exe_dtec:
        (*oss) << "SYSERR_IF[NC]>";
        result = true;
        break;

    case brk_async_ld_stdw_vm_ack:
        (*oss) << "Unknown[type=" << type << "][brk_async_ld_stdw_vm_ack]>";
        break;
    case brk_async_rmw_vm_ack:
        (*oss) << "Unknown[type=" << type << "][brk_async_rmw_vm_ack]>";
        break;
    case eh_relay_brk_vm_ack:
        (*oss) << "Unknown[type=" << type << "][eh_relay_brk_vm_ack]>";
        break;
    case dbnmi_t0_ack:
        (*oss) << "Unknown[type=" << type << "][dbnmi_t0_ack]>";
        break;
    case dbint_t0_ack:
        (*oss) << "Unknown[type=" << type << "][dbint_t0_ack]>";
        break;
    case rmint_t0_ack:
        (*oss) << "Unknown[type=" << type << "][rmint_t0_ack]>";
        break;
    case cpu_feintack_1l_t0:
        (*oss) << "FEINT[TC0]>";
        result = true;
        break;
    case cpu_intack_1l_t0:
        (*oss) << "EIINT[TC0] PRI[" << pri << "] CH[" << ch << "]>";
        result = true;
        break;
    case ehu_ack_fpi_fpu_t0:
        (*oss) << "FPI[TC0]>";
        result = true;
        break;

    default:
        (*oss) << "Unknown[type=" << type << "][default]>";
        break;
    }

    return result;
}


void IDUMP::IDumpProcessing(uint32_t type, uint32_t pe_id, ContextId tc_id, uint32_t pri, uint32_t ch) {
    switch (type) {
    case brk_async_ld_stdw_nm_ack:
        break;
    case brk_async_rmw_nm_ack:
        break;
    case eh_relay_brk_nm_ack:
        break;
    case dbnmi_nt_ack:
        break;
    case dbint_nt_ack:
        break;
    case rmint_nt_ack:
        break;
    case cpu_fenmiack_1l_nt:
        break;
    case syserr_pin_ack_nt:
        SetSYSERRState(pe_id, tc_id, true);
        break;
    case cpu_feintack_1l_nt:
        SetFEINTState(pe_id, tc_id, true);
        break;
    case cpu_intack_1l_nt:
        SetINTState(pe_id, tc_id, true, pri, ch);
        break;
    case ehu_ack_fpi_fpu_nt:
        SetFPIState(pe_id, tc_id, true);
        break;
    case syserr_re_exe_dtec:
        SetSYSERRIFState(pe_id, tc_id, true);
        break;

    case brk_async_ld_stdw_vm_ack:
        break;
    case brk_async_rmw_vm_ack:
        break;
    case eh_relay_brk_vm_ack:
        break;
    case dbnmi_t0_ack:
        break;
    case dbint_t0_ack:
        break;
    case rmint_t0_ack:
        break;
    case cpu_feintack_1l_t0:
        SetFEINTState(pe_id, tc_id, false);
        break;
    case cpu_intack_1l_t0:
        SetINTState(pe_id, tc_id, false, pri, ch);
        break;
    case ehu_ack_fpi_fpu_t0:
        SetFPIState(pe_id, tc_id, false);
        break;

    default:
        break;
    }
}
#elif defined(COMP_RUNNER_G3MH)
bool IDUMP::IDumpValidate(uint32_t type, std::ostringstream *oss) {
    bool result = false;

    if (type >= 0x0000U && type <= 0x0FFFU) {
        (*oss) << "EIINT[TYPE=" << std::hex << type << "]>";
        return true;
    }

    switch (type) {
    case brk_async_ld_stdw_nm_ack:
    case brk_async_rmw_nm_ack:
    case eh_relay_brk_nm_ack:
    case dbnmi_nt_ack:
    case dbint_nt_ack:
    case rmint_nt_ack:
    case cpu_fenmiack_1l_nt:
    case syserr_pin_ack_nt:
    case cpu_feintack_1l_nt:
    case cpu_intack_1l_nt:
        break;
    case ehu_ack_fpi_fpu_nt:
        (*oss) << "FPI[NC]>";
        result = true;
        break;

    case brk_async_ld_stdw_vm_ack:
    case brk_async_rmw_vm_ack:
    case eh_relay_brk_vm_ack:
    case dbnmi_t0_ack:
    case dbint_t0_ack:
    case rmint_t0_ack:
    case cpu_feintack_1l_t0:
    case cpu_intack_1l_t0:
        break;
    case ehu_ack_fpi_fpu_t0:
        (*oss) << "FPI[TC0]>";
        result = true;
        break;
    default:
        break;
    }

    return result;
}
#else  // defined(COMP_RUNNER_G3MH) || defined(COMP_RUNNER_G3KH)
#endif // defined(COMP_RUNNER_G3MH) || defined(COMP_RUNNER_G3KH)


