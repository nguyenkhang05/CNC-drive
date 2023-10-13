/*
 * (c) 2011,2012 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <iostream>
#include <sstream>
#include <iomanip>
#include "./config.h"
#include "./forest_common.h"
#include "./forest_utility.h"
#include "./cmp_runner_if_g4mh.h"
#include "./micro_arch_db.h"
#include "./trace_operand.h"


/** \file cmp_runner_if_g4mh.cpp
 * \brief CompRunner G4MH sub-routine source file.
 *
 * This file contains the sub-routine in CompRunner G4MHv2.
 */


extern FILE *g_cmp_fp;


bool
cmprunner::CmpRegInfo::IsGReg()
{
    return ((this->GetReg() > R0) && (this->GetReg() <= R31));
}

bool
cmprunner::CmpRegInfo::IsWReg()
{
    return ((this->GetReg() >= WREG_BASE) && (this->GetReg() <= WREG_MAX));
}

bool
cmprunner::CmpRegInfo::IsMatchType(const RegInfoType& type_in) {
    return GetCommitType() == type_in;
}

bool
cmprunner::CmpRegInfo::IsMatchType(const RegInfoType& type_1, const RegInfoType& type_2) {
    return (    GetCommitType() == type_1)
           ||   (GetCommitType() == type_2);
}

bool
cmprunner::CmpRunnerIF::IsGReg(CReg regno)
{
    return ((regno > R0) && (regno <= R31));
}

bool
cmprunner::CmpRunnerIF::IsWReg(CReg regno)
{
    return ((regno >= WREG_BASE) && (regno <= WREG_MAX));
}
bool
cmprunner::CmpRunnerIF::RegReadDebug (uint32_t pe_id, CReg regno,
                                      RegData64* data, RegData64* data_u64)
{
    bool ret = m_forest_util->RegReadDebug (pe_id, regno, data, data_u64);
    return ret;
}


bool
cmprunner::CmpRunnerIF::RegWriteDebug (uint32_t pe_id, CReg regno,
                                      RegData64 data, RegData64 data_u64)
{
    bool ret = m_forest_util->RegWriteDebug (pe_id, regno, data, data_u64);
    return ret;
}


cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::VMemWriteDebug (uint32_t pe_id, PhysAddr addr,
        uint32_t data, ByteSize size)
{
    if (IsByteSizeValid (size) == false) {
        return API_E_PARAM;
    }
    m_forest_util->SetCubeErrorCode (::E_OK);
    m_forest_util->VMemWriteDebug (pe_id, addr, data, size);

    ::ErrorCode err = m_forest_util->GetCubeErrorCode ();
    if (err != ::E_OK) {
        return API_E_NOT_MAPPED;
    }

    return API_E_OK;
}


cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::VMemReadDebug (uint32_t pe_id,
        PhysAddr addr,
        ByteSize size, uint32_t* data)
{
    if (IsByteSizeValid (size) == false) {
        return API_E_PARAM;
    }
    m_forest_util->SetCubeErrorCode (::E_OK);
    *data = m_forest_util->VMemReadDebug (pe_id, addr, size);

    ::ErrorCode err = m_forest_util->GetCubeErrorCode ();
    if (err != ::E_OK) {
        return API_E_NOT_MAPPED;
    }

    return API_E_OK;
}

cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::ReqPseudoEiint2Intc1 (uint32_t pe_id, uint32_t channel)
{
    // TCID is not used in CompRunner G4MH
    ::IntErrorCode err = m_forest_util->ReqPseudoInterruptToIntc1(pe_id, channel);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_CHANNEL:      return API_E_PARAM;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM; // unreachable
    }
}

cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::ReqPseudoFeint2Intc1 (uint32_t pe_id, uint32_t channel)
{
    // TCID is not used in CompRunner G4MH
    ::IntErrorCode err = m_forest_util->ReqPseudoFeintToIntc1 (pe_id, channel);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_CHANNEL:      return API_E_PARAM;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM; // unreachable
    }
}

cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::ReqPseudoFenmi2Intc1 (uint32_t pe_id)
{
    // TCID is not used in CompRunner G4MH
    ::IntErrorCode err = m_forest_util->ReqPseudoFenmiToIntc1 (pe_id);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM; // unreachable
    }
}

cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::ReqPseudoEiint (uint32_t pe_id, uint32_t channel, uint32_t priority, bool eitb,
                                        bool is_gm, uint32_t gpid, bool is_bgint)
{
    // TCID is not used in CompRunner G4MH
    ::IntErrorCode err = m_forest_util->ReqInterruptToCpu(pe_id, channel, priority, eitb,
                                                          is_gm, gpid, is_bgint);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_CHANNEL:      return API_E_PARAM;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM; // unreachable
    }
}


cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::ReqPseudoFenmi (uint32_t pe_id)
{
    // TCID is not used in CompRunner G4MH
    ::IntErrorCode err = m_forest_util->ReqFenmiToCpu (pe_id);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM; // unreachable
    }
}


cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::ReqPseudoFeint (uint32_t pe_id, uint32_t channel, bool is_gm, uint32_t gpid, bool is_bgint)
{
    // TCID is not used in CompRunner G4MH
    ::IntErrorCode err = m_forest_util->ReqFeintToCpu (pe_id, channel, is_gm, gpid, is_bgint);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_CHANNEL:      return API_E_PARAM;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM; // unreachable
    }
}


cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::ReqPseudoDbnmi(uint32_t pe_id)
{
    ::IntErrorCode err = m_forest_util->ReqDbnmiToCpu(pe_id);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM; // unreachable
    }
}


cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::ReqPseudoDbint (uint32_t pe_id, uint32_t channel)
{
    ::IntErrorCode err = m_forest_util->ReqDbintToCpu (pe_id, channel);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_CHANNEL:      return API_E_PARAM;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM; // unreachable
    }
}


cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::ReqPseudoSyserr (uint32_t pe_id, uint32_t syserr_cause)
{
    ::IntErrorCode err = m_forest_util->ReqPseudoFetchSyserrToCpu (pe_id, syserr_cause);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_CHANNEL:      return API_E_PARAM;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM;
    }
}

cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::HaltSnoozeRel (uint32_t pe_id)
{
    ::IntErrorCode err = m_forest_util->ReqHaltSnoozeReleaseToCpu(pe_id);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_CHANNEL:      return API_E_PARAM;
    default:                   return API_E_PARAM; // unreachable
    }
}

cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::ReqPseudoOperandSyserr (uint32_t pe_id, uint32_t syserr_cause, bool syserr_mode, uint32_t syserr_gpid)
{
    ::IntErrorCode err = m_forest_util->ReqOperandSyserrToCpu (pe_id, syserr_cause, syserr_mode, syserr_gpid);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_CHANNEL:      return API_E_PARAM;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM;
    }
}

bool
cmprunner::CmpRunnerIF::GetOperandSyserrStatus (uint32_t pe_id, bool syserr_mode, uint32_t syserr_gpid)
{
    return m_forest_util->GetOperandSyserrStatus (pe_id, syserr_mode, syserr_gpid);
}

cmprunner::APIErrorCode
cmprunner::CmpRunnerIF:: CancelPseudoEIINT (uint32_t pe_id, uint32_t channel)
{
    ::IntErrorCode err = m_forest_util->CancelInterruptToCpu (pe_id, channel);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_CHANNEL:      return API_E_PARAM;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM; // unreachable
    }
}

cmprunner::APIErrorCode
cmprunner::CmpRunnerIF:: CancelPseudoEiint (uint32_t pe_id)
{
    ::IntErrorCode err = m_forest_util->CancelInterruptToCpu (pe_id);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_CHANNEL:      return API_E_PARAM;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM; // unreachable
    }
}

cmprunner::APIErrorCode
cmprunner::CmpRunnerIF:: CancelPseudoFeint (uint32_t pe_id)
{
    ::IntErrorCode err = m_forest_util->CancelFeintToCpu (pe_id);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_CHANNEL:      return API_E_PARAM;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM; // unreachable
    }
}

cmprunner::APIErrorCode
cmprunner::CmpRunnerIF:: CancelPseudoFenmi (uint32_t pe_id)
{
    ::IntErrorCode err = m_forest_util->CancelFenmiToCpu (pe_id);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_CHANNEL:      return API_E_PARAM;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM; // unreachable
    }
}

cmprunner::APIErrorCode
cmprunner::CmpRunnerIF:: CancelPseudoDbint (uint32_t pe_id)
{
    ::IntErrorCode err = m_forest_util->CancelDbintToCpu (pe_id);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_CHANNEL:      return API_E_PARAM;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM; // unreachable
    }
}

cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::GetCmpIssFlags (uint32_t rtl_peid, uint32_t *iss_flags)
{
    if (iss_flags == NULL) {
        return API_E_PARAM;
    }
    APIErrorCode ret_val;
    CReg SR_PSW = 0x105;

    ret_val = RegReadDebug (rtl_peid, SR_PSW, iss_flags);
    // Get SAT, CY, OY, S, Z in PSW
    *iss_flags &= 0x1F;
    return ret_val;
}

cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::GetOperandFormat (uint32_t inst_id, std::string* inst_str_ptr)
{
    *inst_str_ptr = MicroArchDB::GetOperandFormat (inst_id);
    return API_E_OK;
}


cmprunner::APIErrorCode cmprunner::CmpRunnerIF::CmpLoadSrec (const char* filename)
{

    if (m_forest_util->LoadSrec (filename, 0) == true) {
        return cmprunner::API_E_OK;
    } else {
        return cmprunner::API_E_PARAM;
    }
}


cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::CmpStepExecute (uint32_t pe_id)
{
    // G4MH has only 1 context NC.
    uint32_t scid = m_forest_util->GetScId (pe_id);

    int32_t ret_val = m_forest_util->StepExecute<STEP_MODE> (scid, 1);

    if (ret_val == -1) {
        return API_E_PARAM;
    }
    return API_E_OK;
}


cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::CmpGetTrace (uint32_t pe_id, TraceCtrl** trace)
{
    // G4MH has only 1 context NC.
    uint32_t scid = m_forest_util->GetScId (pe_id);
    *trace = const_cast<TraceCtrl*> (m_forest_util->GetTraceCtrl (scid));

    return API_E_OK;
}


cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::CmpGetOperand (uint32_t inst_id, uint64_t inst_code,
                                       uint32_t inst_pc, std::string* inst_mnem)
{
    m_print_operand->PrintOperand (inst_id, inst_code, inst_pc, inst_mnem);
    return API_E_OK;
}


cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::CmpAddRegInfo (uint32_t rtl_peid, uint32_t commit_type, uint32_t rtl_nblk_id, CReg regno, RegData64 reg_val_up, RegData64 reg_val_lo)
{
    cmprunner::APIErrorCode ret_val = API_E_OK;
    CmpRegInfoVec::iterator it_reg_info = m_reg_info.begin ();
    CmpRegInfo * p_reg_info = NULL;
    int reg_info_count = 0;
    // Update WAW status for the existent register.
    while (it_reg_info != m_reg_info.end()) {
        if (((*it_reg_info)-> GetReg() == regno) && ((*it_reg_info)-> GetPeid() == rtl_peid)) {
            (*it_reg_info)->SetWAWstatus(true);
            if ((*it_reg_info)->GetWAWstatus() == false) {
                std::ostringstream oss;
                oss << std::setfill(' ') << std::setw (57) << ' ';
                oss << "<Info: Don't apply XDump due to WAW>";
                CmpRegInfo *new_reg_info = new CmpRegInfo((*it_reg_info)-> GetPeid(), (*it_reg_info)-> GetCommitType(), (*it_reg_info)->GetNblkId(), (*it_reg_info)->GetReg(),
                                                          (*it_reg_info)-> GetRegValUp(), (*it_reg_info)-> GetRegValLo(), (*it_reg_info)->GetHasCondition(),
                                                          (*it_reg_info)->GetXDumpArith(), (*it_reg_info)->GetXDumpDiffVal(), (*it_reg_info)->GetXDumpMask(), true);
                m_reg_info.erase(it_reg_info);
                it_reg_info = m_reg_info.begin () + reg_info_count;
                m_reg_info.insert(it_reg_info, new_reg_info);
                fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
            } else {
                std::ostringstream oss;
                oss << std::setfill(' ') << std::setw (57) << ' ';
                oss << "<Warning: Don't apply XDump due to WAW>";
                fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
            }
        }
        ++it_reg_info;
        ++reg_info_count;
    }

    // Add new register to CompRunner buffer.
    it_reg_info = m_reg_info.begin ();
    switch (commit_type) {
    case TYPE_EMP:
        while (it_reg_info != m_reg_info.end()) {
            if (((*it_reg_info)-> GetNblkId() == rtl_nblk_id) && ((*it_reg_info)-> GetPeid() == rtl_peid)) {
                // Registration is duplicated.
                ret_val = API_E_MULTI_ALLOC;
                break;
            }
            ++it_reg_info;
        }
        if (it_reg_info == m_reg_info.end()) {
            it_reg_info = m_reg_info.begin ();
            while (it_reg_info != m_reg_info.end()) {
                if (((*it_reg_info)-> IsMatchType(TYPE_EMP,TYPE_RTL))
                  && ((*it_reg_info)-> GetReg() == regno) && ((*it_reg_info)-> GetPeid() == rtl_peid)) {
                    // Registration is duplicated.
                    ret_val = API_E_MULTI_ALLOC;
                }
                else if (((*it_reg_info)->IsMatchType(TYPE_ISS))
                  && ((*it_reg_info)-> GetReg() == regno) && ((*it_reg_info)-> GetPeid() == rtl_peid)) {
                    // There is 1 NBLK with same regno already in buffer.
                    (*it_reg_info)->SetDataVld(DATA_REG_INVALID);
                }
                ++it_reg_info;
            }
        }
        if (it_reg_info == m_reg_info.end()) {
            p_reg_info = new CmpRegInfo(rtl_peid, commit_type, rtl_nblk_id, regno, reg_val_up, reg_val_lo, false, false, 0x0, WORD_MASK, false);
            m_reg_info.push_back(p_reg_info);
        }
        break;
    case TYPE_RTL:
        // Search register number with TYPE_EMPTY or TYPE_RTL in CompRunner buffer. If it is existent, this function return error message.
        while (it_reg_info != m_reg_info.end()) {
            if (((*it_reg_info)-> IsMatchType(TYPE_EMP,TYPE_RTL))
              && ((*it_reg_info)-> GetReg() == regno) && ((*it_reg_info)-> GetPeid() == rtl_peid)) {
                // Registration is duplicated.
                ret_val = API_E_MULTI_ALLOC;
            }
            else if (((*it_reg_info)-> GetCommitType() == TYPE_ISS)
              && ((*it_reg_info)-> GetReg() == regno) && ((*it_reg_info)-> GetPeid() == rtl_peid)) {
                // There is 1 NBLK with same regno already in buffer.
                (*it_reg_info)->SetDataVld(DATA_REG_INVALID);
            }
            ++it_reg_info;
        }
        if (it_reg_info == m_reg_info.end()) {
            p_reg_info = new CmpRegInfo(rtl_peid, commit_type, rtl_nblk_id, regno, reg_val_up, reg_val_lo, false, false, 0x0, WORD_MASK, false);
            m_reg_info.push_back(p_reg_info);
        }
        break;

    case TYPE_ISS:        // Can't add Register Info with TYPE_ISS.
    default:
        // Commit type is incorrect.
        ret_val = API_E_NOT_MAPPED;
        break;
    }
    return ret_val;
}


cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::CmpDelRegInfo (uint32_t rtl_peid, uint32_t commit_type, uint32_t rtl_nblk_id, CReg rtl_regno,
                                       RegData64 rtl_reg_val_up, RegData64 rtl_reg_val_lo, uint32_t rtl_xdump_ctrl, CmpResult &cmp_ret)
{
    cmprunner::APIErrorCode ret_val = API_E_OK;
    CmpRegInfoVec::iterator it_reg_info = m_reg_info.begin ();
    bool is_reg_found = false;
    bool is_reg_del = false;
    uint32_t mem_data;
    switch (commit_type) {
    case TYPE_ISS:
        // Delete element with TYPE_ISS. Search in CompRunner buffer with NBLK ID key.
        while (it_reg_info != m_reg_info.end()) {
            if (((*it_reg_info)-> GetNblkId() == rtl_nblk_id)  && ((*it_reg_info)-> GetPeid() == rtl_peid)) {
                is_reg_found = true;
                if (((*it_reg_info)-> GetRegValUp() == rtl_reg_val_up) && ((*it_reg_info)-> GetRegValLo() == rtl_reg_val_lo)) {
                    is_reg_del = true;
                    m_reg_info.erase(it_reg_info);
                } else if ((*it_reg_info)-> GetHasCondition() || (rtl_xdump_ctrl == 1)) {
                    CReg reg_no = (*it_reg_info)-> GetReg();
                    uint32_t mask_val = (*it_reg_info)->GetXDumpMask();
                    bool xdump_arith = (*it_reg_info)->GetXDumpArith();
                    uint32_t xdump_diffval = (*it_reg_info)->GetXDumpDiffVal();

                    // Separate comparison values to 4 32-bit data.
                    RegData iss_wdata_3 = ((*it_reg_info)-> GetRegValUp() >> 32)   & WORD_MASK_UL;
                    RegData iss_wdata_2 = (*it_reg_info)-> GetRegValUp()           & WORD_MASK_UL;
                    RegData iss_wdata_1 = ((*it_reg_info)-> GetRegValLo() >> 32)   & WORD_MASK_UL;
                    RegData iss_wdata_0 = (*it_reg_info)-> GetRegValLo()           & WORD_MASK_UL;

                    RegData rtl_wdata_3 = (rtl_reg_val_up >> 32)   & WORD_MASK_UL;
                    RegData rtl_wdata_2 = rtl_reg_val_up           & WORD_MASK_UL;
                    RegData rtl_wdata_1 = (rtl_reg_val_lo >> 32)   & WORD_MASK_UL;
                    RegData rtl_wdata_0 = rtl_reg_val_lo           & WORD_MASK_UL;

                    // Call XDump function for comparison
                    is_reg_del = CheckXDumpInfo(rtl_peid, reg_no, iss_wdata_0, iss_wdata_1, iss_wdata_2, iss_wdata_3,
                                                     rtl_wdata_0, rtl_wdata_1, rtl_wdata_2, rtl_wdata_3,
                                                     mask_val, xdump_arith, xdump_diffval, rtl_xdump_ctrl, (*it_reg_info)-> GetWAWstatus());
                    // Write back to memory in case Xdump address type
                    bool IsUpdateMem = m_xdump->IsUpdateXDumpAddr(reg_no, (*it_reg_info)->GetNblkId());
                    while (IsUpdateMem) {
                        uint32_t mem_start = m_xdump->XDumpAddrEle.m_start_addr;
                        uint32_t mem_mask  = m_xdump->XDumpAddrEle.m_xdump_mask;
                        uint32_t store_type = m_xdump->XDumpAddrEle.m_store_type;
                        uint32_t shift_offset = 0;
                        uint32_t rtl_data = 0;
                        switch(store_type){
                            case SIZE_8BIT:
                                shift_offset = log2(mem_mask/BYTE_MASK);
                                rtl_data = rtl_wdata_0 & BYTE_MASK;
                                rtl_data = (rtl_data << shift_offset) & mem_mask ;
                            break;
                            case SIZE_16BIT:
                                shift_offset = log2(mem_mask/HWORD_MASK) > HWORD_HIGH ? HWORD_HIGH_SHIFT
                                                                                      : HWORD_LOW_SHIFT;
                                rtl_data = rtl_wdata_0 & HWORD_MASK;
                                rtl_data = (rtl_data << shift_offset) & mem_mask;
                            break;
                            case SIZE_32BIT:
                                shift_offset = log2(mem_mask/WORD_MASK);
                                rtl_data = rtl_wdata_0 & WORD_MASK;
                                rtl_data = rtl_data & mem_mask;
                            break;
                        }
                        VMemReadDebug(rtl_peid, mem_start, SIZE_32BIT, &mem_data);
                        mem_data = (mem_data & ~mem_mask) | rtl_data ;
                        VMemWriteDebug(rtl_peid, mem_start, mem_data, SIZE_32BIT);
                        m_xdump->DelXDumpAddr(reg_no, (*it_reg_info)->GetNblkId());
                        IsUpdateMem = m_xdump->IsUpdateXDumpAddr(reg_no, (*it_reg_info)->GetNblkId());
                    }
                    m_reg_info.erase(it_reg_info);
                } else {
                    m_reg_info.erase(it_reg_info);
                }
                break;
            }
            ++it_reg_info;
        }
        break;

    case TYPE_RTL:
        // Delete element with TYPE_RTL. Search in CompRunner buffer with TYPE_RTL and register number.
        while (it_reg_info != m_reg_info.end()) {
            if ((   (*it_reg_info)-> IsMatchType(TYPE_RTL))
                && ((*it_reg_info)-> GetReg() == rtl_regno)
                && ((*it_reg_info)-> GetPeid() == rtl_peid)) {
                is_reg_found = true;
                if (((*it_reg_info)-> GetRegValUp() == rtl_reg_val_up) && ((*it_reg_info)-> GetRegValLo() == rtl_reg_val_lo)) {
                    is_reg_del = true;
                    m_reg_info.erase(it_reg_info);
                } else if ((*it_reg_info)-> GetHasCondition() || (rtl_xdump_ctrl == 1)) {
                    CReg reg_no = (*it_reg_info)-> GetReg();
                    uint32_t mask_val = (*it_reg_info)->GetXDumpMask();
                    bool xdump_arith = (*it_reg_info)->GetXDumpArith();
                    uint32_t xdump_diffval = (*it_reg_info)->GetXDumpDiffVal();

                    // Separate comparison values to 4 32-bit data.
                    RegData rtl_wdata_3 = ((*it_reg_info)-> GetRegValUp() >> 32)   & WORD_MASK_UL;
                    RegData rtl_wdata_2 = (*it_reg_info)-> GetRegValUp()           & WORD_MASK_UL;
                    RegData rtl_wdata_1 = ((*it_reg_info)-> GetRegValLo() >> 32)   & WORD_MASK_UL;
                    RegData rtl_wdata_0 = (*it_reg_info)-> GetRegValLo()           & WORD_MASK_UL;

                    RegData iss_wdata_3 = (rtl_reg_val_up >> 32)   & WORD_MASK_UL;
                    RegData iss_wdata_2 = rtl_reg_val_up           & WORD_MASK_UL;
                    RegData iss_wdata_1 = (rtl_reg_val_lo >> 32)   & WORD_MASK_UL;
                    RegData iss_wdata_0 = rtl_reg_val_lo           & WORD_MASK_UL;

                    // Call XDump function for comparison
                    is_reg_del = CheckXDumpInfo(rtl_peid, reg_no, iss_wdata_0, iss_wdata_1, iss_wdata_2, iss_wdata_3,
                                                     rtl_wdata_0, rtl_wdata_1, rtl_wdata_2, rtl_wdata_3,
                                                     mask_val, xdump_arith, xdump_diffval, rtl_xdump_ctrl, (*it_reg_info)-> GetWAWstatus());

                    // Write back to memory in case Xdump address type
                    bool IsUpdateMem = m_xdump->IsUpdateXDumpAddr(reg_no, (*it_reg_info)->GetNblkId());
                    while (IsUpdateMem) {
                        uint32_t mem_start  = m_xdump->XDumpAddrEle.m_start_addr;
                        uint32_t mem_mask   = m_xdump->XDumpAddrEle.m_xdump_mask;
                        uint32_t store_type = m_xdump->XDumpAddrEle.m_store_type;
                        uint32_t shift_offset = 0;
                        uint32_t rtl_data = 0;
                        switch(store_type){
                            case SIZE_8BIT:
                                shift_offset = log2(mem_mask/BYTE_MASK);
                                rtl_data = rtl_wdata_0 & BYTE_MASK;
                                rtl_data = (rtl_data << shift_offset) & mem_mask;
                            break;
                            case SIZE_16BIT:
                                shift_offset = log2(mem_mask/HWORD_MASK) > HWORD_HIGH ? HWORD_HIGH_SHIFT
                                                                                      : HWORD_LOW_SHIFT;
                                rtl_data = rtl_wdata_0 & HWORD_MASK;
                                rtl_data = (rtl_data << shift_offset) & mem_mask;
                            break;
                            case SIZE_32BIT:
                                shift_offset = log2(mem_mask/WORD_MASK);
                                rtl_data = rtl_wdata_0 & WORD_MASK;
                                rtl_data = rtl_data & mem_mask;
                            break;
                        }
                        VMemReadDebug(rtl_peid, mem_start, SIZE_32BIT, &mem_data);
                        mem_data = (mem_data & ~mem_mask) | rtl_data ;
                        VMemWriteDebug(rtl_peid, mem_start, mem_data, SIZE_32BIT);
                        m_xdump->DelXDumpAddr(reg_no, (*it_reg_info)->GetNblkId());
                        IsUpdateMem = m_xdump->IsUpdateXDumpAddr(reg_no, (*it_reg_info)->GetNblkId());
                    }
                    m_reg_info.erase(it_reg_info);
                } else {
                    m_reg_info.erase(it_reg_info);
                }
                break;
            }
            ++it_reg_info;
        }

        break;

    case TYPE_EMP:
    default:
        // Commit type is incorrect.
        ret_val = API_E_NOT_MAPPED;
        break;
    }

    if (is_reg_found == false) {
        cmp_ret = CMP_REG_MISS;
    } else if (is_reg_del == false) {
        cmp_ret = CMP_DATA_MISS;
    } else {
        cmp_ret = CMP_MATCH;
    }
    return ret_val;

}


cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::CmpGetRegInfo (uint32_t rtl_peid, uint32_t context_info, uint32_t rtl_nblk_id, CReg rtl_regno, RegData64 reg_val_upd_up, RegData64 reg_val_upd_lo,
                                      CmpRegInfo &cmp_reg_info, CmpResult &cmp_ret)
{
    cmprunner::APIErrorCode ret_val = API_E_OK;
    CmpRegInfoVec::iterator it_reg_info = m_reg_info.begin ();
    switch (context_info) {
    case CtxCompByLast:
        // Get Register Info in DPI_CmpStep function in Comparison step.
        // Search Register Info with the key {Register Number, TYPE_RTL} or {Register Number, TYPE_EMP}
        while (it_reg_info != m_reg_info.end()) {
            if (((*it_reg_info)-> IsMatchType(TYPE_RTL)) && ((*it_reg_info)-> GetPeid() == rtl_peid)) {
                if (((*it_reg_info)->IsGReg())) {
                    if (((*it_reg_info)-> GetReg() == rtl_regno)) {
                        if ((((*it_reg_info)-> GetRegValUp() == reg_val_upd_up)
                          && ((abs ((*it_reg_info)-> GetRegValLo() - reg_val_upd_lo)) <= (*it_reg_info)->GetXDumpDiffVal()))
                           || (*it_reg_info)-> GetHasCondition()) {
                            break;
                        }
                    }
                } else if ((*it_reg_info)->IsWReg() ) {
                    if (((*it_reg_info)-> GetReg() == rtl_regno)) {
                        break;
                    }
                }
            // Search the Register index at 2nd time to eliminate the mismatch from difference of register index order between CForest and RTL
            } else if ((    (*it_reg_info)-> IsMatchType(TYPE_EMP))
                        &&  ((*it_reg_info)-> GetPeid() == rtl_peid)
                        &&  ((*it_reg_info)-> GetReg() == rtl_regno)) {
                break;
            }
            ++it_reg_info;
        }

        if (it_reg_info == m_reg_info.end()) {
            // Can't find suitable register info.
            cmp_ret = CMP_REG_LACK;
        }
        break;

    case CtxCompByNBLK:
        // Get Register Info in DPI_CmpNbWb function.
        while (it_reg_info != m_reg_info.end()) {
            // No need to check commit type: TYPE_EMP or TYPE_ISS because NBLK ID is the unique key.
            // Search Register Info with NBLK ID.
            if (((*it_reg_info)-> GetNblkId() == rtl_nblk_id) && ((*it_reg_info)-> GetPeid() == rtl_peid)) {
                break;
            }
            ++it_reg_info;
        }
        if (it_reg_info == m_reg_info.end()) {
            // Can't find suitable register info.
            cmp_ret = CMP_NB_MISS;
        }
        break;

    case CtxRegEmpInfo:
        // Get Register Info in DPI_CmpStep function in Register Info step.
        // Search register info with the key {TYPE_EMP, NBLK ID}
        while (it_reg_info != m_reg_info.end()) {
            if ((   (*it_reg_info)-> IsMatchType(TYPE_EMP))
                && ((*it_reg_info)-> GetNblkId() == rtl_nblk_id)
                && ((*it_reg_info)-> GetPeid() == rtl_peid)) {
                break;
            }
            ++it_reg_info;
        }
        if (it_reg_info == m_reg_info.end()) {
            // Can't find suitable register info.
            cmp_ret = CMP_NB_MISS;
        }
        break;
    case CtxCheckLast:
        // Get Register Info in DPI_CmpStep function in Register Info step.
        // Search register info with the key {TYPE_EMP, NBLK ID}
        while (it_reg_info != m_reg_info.end()) {
            if ((   (*it_reg_info)-> IsMatchType(TYPE_EMP,TYPE_RTL))
                && ((*it_reg_info)-> GetPeid() == rtl_peid)) {
                break;
            }
            ++it_reg_info;
        }
        break;
    case CtxRegRTLInfo:
    default:
        // Commit type is incorrect.
        ret_val = API_E_NOT_MAPPED;
        break;
    }

    if (it_reg_info != m_reg_info.end()) {
        cmp_reg_info.SetPeid         ((*it_reg_info)-> GetPeid());
        cmp_reg_info.SetCommitType   ((*it_reg_info)-> GetCommitType());
        cmp_reg_info.SetNblkId       ((*it_reg_info)-> GetNblkId());
        cmp_reg_info.SetReg          ((*it_reg_info)-> GetReg());
        cmp_reg_info.SetRegValUp     ((*it_reg_info)-> GetRegValUp());
        cmp_reg_info.SetRegValLo     ((*it_reg_info)-> GetRegValLo());
        cmp_reg_info.SetHasCondition ((*it_reg_info)-> GetHasCondition());
        cmp_reg_info.SetXDumpArith   ((*it_reg_info)-> GetXDumpArith());
        cmp_reg_info.SetXDumpDiffVal ((*it_reg_info)-> GetXDumpDiffVal());
        cmp_reg_info.SetXDumpMask    ((*it_reg_info)-> GetXDumpMask());
    }
    return ret_val;
}


cmprunner::APIErrorCode
cmprunner::CmpRunnerIF::CmpUpdRegInfo (uint32_t rtl_peid, uint32_t commit_type, uint32_t rtl_nblk_id, CReg rtl_regno,
                                       RegData64 reg_val_upd_up, RegData64 reg_val_upd_lo, CmpResult &cmp_ret)
{
    cmprunner::APIErrorCode ret_val = API_E_OK;
    CmpRegInfoVec::iterator it_reg_info = m_reg_info.begin ();
    int reg_info_count = 0;

    switch (commit_type) {
    case TYPE_ISS:
        // Update Register Info from TYPE_EMP to TYPE_ISS.
        while (it_reg_info != m_reg_info.end()) {
            if ((   (*it_reg_info)-> IsMatchType(TYPE_EMP))
                && ((*it_reg_info)-> GetReg() == rtl_regno)
                && ((*it_reg_info)-> GetPeid() == rtl_peid)) {
                CmpRegInfo *new_reg_info = new CmpRegInfo((*it_reg_info)-> GetPeid(), TYPE_ISS, (*it_reg_info)->GetNblkId(), (*it_reg_info)->GetReg(),
                                                          reg_val_upd_up, reg_val_upd_lo, (*it_reg_info)->GetHasCondition(), (*it_reg_info)->GetXDumpArith(),
                                                          (*it_reg_info)->GetXDumpDiffVal(), (*it_reg_info)->GetXDumpMask(), (*it_reg_info)->GetWAWstatus());
                m_reg_info.erase(it_reg_info);
                it_reg_info = m_reg_info.begin () + reg_info_count;
                m_reg_info.insert(it_reg_info, new_reg_info);
                break;
            }
            ++it_reg_info;
            ++reg_info_count;
        }
        if (it_reg_info == m_reg_info.end()) {
            // Can't find the suitable register info.
        }
        break;

    case TYPE_RTL:
        // Update Register Info from TYPE_EMP to TYPE_RTL in case Out-of-order commit.
        // NBLKID will be removed because it can be re-used in another out-of-order commit.
        while (it_reg_info != m_reg_info.end()) {
            if (    (*it_reg_info)->IsMatchType(TYPE_EMP)
                && ((*it_reg_info)-> GetNblkId() == rtl_nblk_id)
                && ((*it_reg_info)-> GetPeid() == rtl_peid)) {
                if ((*it_reg_info)->GetReg() == CmpR32) {
                    m_reg_info.erase(it_reg_info);
                } else {
                    uint32_t NBLKID_DEL = 0x0000FFFF;
                    CmpRegInfo *new_reg_info = new CmpRegInfo((*it_reg_info)-> GetPeid(), TYPE_RTL, NBLKID_DEL, (*it_reg_info)->GetReg(),
                                                               reg_val_upd_up, reg_val_upd_lo, (*it_reg_info)->GetHasCondition(), (*it_reg_info)->GetXDumpArith(),
                                                               (*it_reg_info)->GetXDumpDiffVal(), (*it_reg_info)->GetXDumpMask(), (*it_reg_info)->GetWAWstatus());
                    m_reg_info.erase(it_reg_info);
                    it_reg_info = m_reg_info.begin () + reg_info_count;
                    m_reg_info.insert(it_reg_info, new_reg_info);
                }
                break;
            }
            ++it_reg_info;
            ++reg_info_count;
        }
        if (it_reg_info == m_reg_info.end()) {
            // Can't find the suitable register info.
        }
        break;

    case TYPE_EMP:
    default:
        // Commit type is incorrect.
        ret_val = API_E_NOT_MAPPED;
        break;
    }

    return ret_val;
}


bool cmprunner::CmpRunnerIF::CheckLastCommit (uint32_t pe_id)
{
    bool ret_val = true;
    CmpRegInfoVec::iterator it_reg_info = m_reg_info.begin ();
    while (it_reg_info != m_reg_info.end()) {
        if (((*it_reg_info)-> IsMatchType(TYPE_RTL)) && ((*it_reg_info)-> GetPeid() == pe_id)) {
            // Ignore comparison in case R0 and R32
            if ((*it_reg_info)-> GetReg() != R0 && (*it_reg_info)-> GetReg() != CmpR32) {
                fprintf (g_cmp_fp, "%96s <ERR: R%d is not clear>\n", "", (*it_reg_info)-> GetReg());
                ret_val = false;
            }
            it_reg_info = m_reg_info.erase(it_reg_info);
        } else {
            ++it_reg_info;
        }

    }
    return ret_val;
}


bool cmprunner::CmpRunnerIF::UpdateXDumpInfo(uint32_t rtl_peid, TraceCtrl *trace) {
    if (trace == NULL) {
        return false;
    }
    bool ret_val = true;
    static const int32_t REC_SREG_OFFSET = 0x00000100;

    // Update XDump for FPU, FXU instructions
    bool is_xdump_arith = m_xdump->IsXDumpArith (trace->GetInstId ());
    if (is_xdump_arith) {
        uint32_t arith_diff = m_xdump->XDumpArithEle.m_diff;
        for (int32_t i_reg = 0; i_reg < trace->GetWregCount(); i_reg++) {
            CReg cforest_regno = trace->GetWriteReg(i_reg) & REG_ID_MASK;
            if (IsGReg(cforest_regno) || IsWReg(cforest_regno)) {
                // Update XDump function for all register info.
                CmpRegInfoVec::iterator it_reg_info = m_reg_info.begin ();
                while (it_reg_info != m_reg_info.end()) {
                    // Only update XDump info for Empty and RTL type.
                    if (    ((*it_reg_info)->IsMatchType(TYPE_EMP,TYPE_RTL))
                       &&   ((*it_reg_info)-> GetPeid() == rtl_peid)) {
                        if ((*it_reg_info)-> GetReg() == cforest_regno) {
                            // XDump for arithmetic operation
                            (*it_reg_info)->SetHasCondition(true);
                            (*it_reg_info)->SetXDumpArith(true);
                            (*it_reg_info)->SetXDumpDiffVal(arith_diff);
                        }
                    }
                    ++it_reg_info;
                }
            }
        }
    }
    // Check store register is Xdump register or not
    if ( trace->GetWmemCount() > NO_WRITE_MEM ) {
        if (trace->GetRregCount() > NO_READ_REG) {
            bool NotXDumpRegister = NOT_XDUMP_REG;
            RegData64 hvcfg_val;
            CReg SR_HVCFG = 0x130 ;
            ret_val = RegReadDebug(rtl_peid, SR_HVCFG, &hvcfg_val);
            int32_t i_max = trace->GetWmemCount() > trace->GetRregCount() ? trace->GetRregCount()
                                                                          : trace->GetWmemCount() ;
            for (int32_t i_mem = 0; i_mem < i_max ; i_mem++) {
                PhysAddr WAddr = trace->GetWmemAddr(i_mem);
                ByteSize WSize = (trace->GetWmemSize(i_mem) == SIZE_64BIT) ? SIZE_32BIT
                                                                           : trace->GetWmemSize(i_mem);
                CmpRegInfoVec::iterator it_reg_info = m_reg_info.begin ();
                if ( (   (static_cast<ExpCode>(trace->GetExpCode()) == EXP_EIINT) && (hvcfg_val == NO_VM_SUPPORT))
                      || (static_cast<ExpCode>(trace->GetExpCode()) == EXP_GMEIINT)) {
                    CReg cforest_regno = (i_mem - 1 ) > 0 ? trace->GetReadReg(i_mem - 1) & REG_ID_MASK
                                                                                         : DUMMY_REG_ID;
                    it_reg_info = m_reg_info.begin ();
                    NotXDumpRegister = NOT_XDUMP_REG;
                    while ( it_reg_info != m_reg_info.end() ) {
                        if ( (*it_reg_info)->GetPeid() == rtl_peid ) {
                            if (    ((*it_reg_info)->GetReg() == cforest_regno)
                                 && ((*it_reg_info)->GetDataVld() == DATA_REG_VALID) ) {
                                if ( (*it_reg_info)->GetHasCondition()) {
                                    m_xdump->AddXDumpAddr(WAddr, WSize, cforest_regno,
                                                                   (*it_reg_info)->GetNblkId());
                                    NotXDumpRegister = IS_XDUMP_REG;
                                }
                            }
                        }
                        ++it_reg_info;
                    }
                    if ( NotXDumpRegister == NOT_XDUMP_REG ) {
                        if ( !m_xdump->IsXDumpAddrEmpty()) {
                            if ( m_xdump->IsXDumpAddr(WAddr) ) {
                                m_xdump->DelXDumpAddr(WAddr, WSize);
                            }
                        }
                    }
                }
                else {
                    CReg cforest_regno = (i_mem+1)<trace->GetRregCount()?trace->GetReadReg(i_mem+1) & REG_ID_MASK
                                                                              : DUMMY_REG_ID ;
                    it_reg_info = m_reg_info.begin ();
                    NotXDumpRegister = NOT_XDUMP_REG;
                    while ( it_reg_info != m_reg_info.end() ) {
                        if ( (*it_reg_info)->GetPeid() == rtl_peid ) {
                            if (   ((*it_reg_info)->GetReg() == cforest_regno)
                                && ((*it_reg_info)->GetDataVld() == DATA_REG_VALID)) {
                                if ( (*it_reg_info)->GetHasCondition()) {
                                    m_xdump->AddXDumpAddr(WAddr, WSize, cforest_regno,
                                                                   (*it_reg_info)->GetNblkId());
                                    NotXDumpRegister = IS_XDUMP_REG;
                                }
                            }
                        }
                        ++it_reg_info;
                    }
                    if ( NotXDumpRegister == NOT_XDUMP_REG ) {
                        if ( !m_xdump->IsXDumpAddrEmpty()) {
                            if ( m_xdump->IsXDumpAddr(WAddr) ) {
                                m_xdump->DelXDumpAddr(WAddr, WSize);
                            }
                        }
                    }
                    // For ST.DW -- checking the 2nd word of ST.DW is sasitsfy dumping condition or not
                    if (    IsGReg(cforest_regno)
                        && (trace->GetWmemSize(i_mem) == SIZE_64BIT)) {
                        WAddr = WAddr + 0x04U ;
                        cforest_regno = cforest_regno + 1;
                        it_reg_info = m_reg_info.begin ();
                        NotXDumpRegister = NOT_XDUMP_REG;
                        while ( it_reg_info != m_reg_info.end() ) {
                            if ( (*it_reg_info)->GetPeid() == rtl_peid ) {
                                if (   ((*it_reg_info)->GetReg() == cforest_regno)
                                    && ((*it_reg_info)->GetDataVld() == DATA_REG_VALID)) {
                                    if ( (*it_reg_info)->GetHasCondition()) {
                                        m_xdump->AddXDumpAddr(WAddr, SIZE_32BIT, cforest_regno,
                                                                       (*it_reg_info)->GetNblkId());
                                        NotXDumpRegister = IS_XDUMP_REG;
                                    }
                                }
                            }
                            ++it_reg_info;
                        }
                        if ( NotXDumpRegister == NOT_XDUMP_REG ) {
                            if ( !m_xdump->IsXDumpAddrEmpty() ) {
                                if ( m_xdump->IsXDumpAddr(WAddr) ) {
                                    m_xdump->DelXDumpAddr(WAddr, SIZE_32BIT);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //Update XDump for GR/WR register
    if (trace->GetWregCount() > 0) {
        if ((trace->GetRmemCount() > 0)) { // Begin of checking read memory
            int32_t i_max = trace->GetRmemCount() > trace->GetWregCount() ? trace->GetWregCount() : trace->GetRmemCount() ;
            for (int32_t i_mem = 0; i_mem < i_max ; i_mem++) {
                PhysAddr RAddr = trace->GetRmemAddr(i_mem);
                if (m_xdump->IsXDumpReg(RAddr)) {
                    CReg cforest_regno = trace->GetWriteReg(i_mem) & REG_ID_MASK;
                    if (IsGReg(cforest_regno) || IsWReg(cforest_regno)) {
                        CmpRegInfoVec::iterator it_reg_info = m_reg_info.begin ();
                        while (it_reg_info != m_reg_info.end()) {
                            // Only update XDump info for Empty and RTL type.
                            if (((*it_reg_info)-> IsMatchType(TYPE_EMP,TYPE_RTL))
                               && ((*it_reg_info)-> GetPeid() == rtl_peid)) {
                                if ((*it_reg_info)-> GetReg() == cforest_regno) {
                                    (*it_reg_info)->SetHasCondition(true);
                                    (*it_reg_info)->SetXDumpMask(WORD_MASK);
                                }
                            }
                            ++it_reg_info;
                        }
                        //For LD.DW -- checking the 2nd word of LD.DW is satisfy dumping condition or not
                        if (IsGReg(cforest_regno)
                            && (trace->GetRmemSize(i_mem) == SIZE_64BIT)) {
                            RAddr = RAddr + 0x04U ;
                            if (m_xdump->IsXDumpReg(RAddr)) {
                                cforest_regno = trace->GetWriteReg(i_mem+1) & REG_ID_MASK;
                                it_reg_info = m_reg_info.begin ();
                                while (it_reg_info != m_reg_info.end()) {
                                    // Only update XDump info for Empty and RTL type.
                                    if (((*it_reg_info)-> IsMatchType(TYPE_EMP,TYPE_RTL))
                                       && ((*it_reg_info)-> GetPeid() == rtl_peid)) {
                                        if ((*it_reg_info)-> GetReg() == cforest_regno) {
                                            (*it_reg_info)->SetHasCondition(true);
                                            (*it_reg_info)->SetXDumpMask(WORD_MASK);
                                        }
                                    }
                                    ++it_reg_info;
                                }
                            }
                        }
                    }
    // Update Xdunmp for GR/WR register in case Xdump address
                } else if (m_xdump->IsXDumpAddr(RAddr)){
                    CReg cforest_regno = trace->GetWriteReg(i_mem) & REG_ID_MASK;
                    if (    IsGReg(cforest_regno)
                        ||  IsWReg(cforest_regno)) {
                        CmpRegInfoVec::iterator it_reg_info = m_reg_info.begin ();
                        while (it_reg_info != m_reg_info.end()) {
                            // Only update XDump info for Empty and RTL type.
                            if (    ((*it_reg_info)-> IsMatchType(TYPE_EMP,TYPE_RTL))
                                &&  ((*it_reg_info)-> GetPeid() == rtl_peid) ) {
                                if ((*it_reg_info)-> GetReg() == cforest_regno) {
                                    (*it_reg_info)->SetHasCondition(XDUMP_CONDITION);
                                    (*it_reg_info)->SetXDumpMask(m_xdump->XDumpAddrEle.m_xdump_mask);
                                }
                            }
                            ++it_reg_info;
                        }
                        //For LD.DW -- checking the 2nd word of LD.DW is satisfy dumping condition or not
                        if (   IsGReg(cforest_regno)
                            && (trace->GetRmemSize(i_mem) == SIZE_64BIT)) {
                            RAddr = RAddr + 0x04U ;
                            if (m_xdump->IsXDumpAddr(RAddr)) {
                                cforest_regno = trace->GetWriteReg(i_mem+1) & REG_ID_MASK;
                                it_reg_info = m_reg_info.begin ();
                                while (it_reg_info != m_reg_info.end()) {
                                    // Only update XDump info for Empty and RTL type.
                                    if (    ((*it_reg_info)-> IsMatchType(TYPE_EMP,TYPE_RTL))
                                       &&   ((*it_reg_info)-> GetPeid() == rtl_peid)) {
                                        if ((*it_reg_info)-> GetReg() == cforest_regno) {
                                            (*it_reg_info)->SetHasCondition(XDUMP_CONDITION);
                                            (*it_reg_info)->SetXDumpMask(m_xdump->XDumpAddrEle.m_xdump_mask);
                                        }
                                    }
                                    ++it_reg_info;
                                }
                            }
                        }
                    }
                } else if (m_xdump->IsXDumpData(RAddr)) {
                    CReg cforest_regno = trace->GetWriteReg(i_mem) & REG_ID_MASK;
                    if (IsGReg(cforest_regno) || IsWReg(cforest_regno)) {
                        CmpRegInfoVec::iterator it_reg_info = m_reg_info.begin ();
                        while (it_reg_info != m_reg_info.end()) {
                            // Only update XDump info for Empty and RTL type.
                            if (    ((*it_reg_info)-> IsMatchType(TYPE_EMP,TYPE_RTL))
                               &&   ((*it_reg_info)-> GetPeid() == rtl_peid)) {
                                if ((*it_reg_info)-> GetReg() == cforest_regno) {
                                    (*it_reg_info)->SetHasCondition(true);
                                    uint32_t mask ;
                                    if (trace->GetRmemSize(i_mem) == SIZE_8BIT) {
                                        uint32_t shift_offset = RAddr & 0x03U ;
                                        mask = (m_xdump->XDumpDataEle.m_mask >> (shift_offset*8)) & BYTE_MASK ;
                                    } else if (trace->GetRmemSize(i_mem) == SIZE_16BIT) {
                                        uint32_t shift_offset = (RAddr >> 1) & 0x01U ;
                                        mask = (m_xdump->XDumpDataEle.m_mask >> (shift_offset*16)) & HWORD_MASK ;
                                    } else {
                                        mask = m_xdump->XDumpDataEle.m_mask & WORD_MASK ;
                                    }
                                    (*it_reg_info)->SetXDumpMask(mask);
                                }
                            }
                            ++it_reg_info;
                        }
                        //For LD.DW -- checking the 2nd word of LD.DW is satisfy dumping condition or not
                        if (    IsGReg(cforest_regno)
                            && (trace->GetRmemSize(i_mem) == SIZE_64BIT)) {
                            RAddr = RAddr + 0x04U ;
                            if (m_xdump->IsXDumpReg(RAddr)) {
                                cforest_regno = trace->GetWriteReg(i_mem+1) & REG_ID_MASK;
                                it_reg_info = m_reg_info.begin ();
                                while (it_reg_info != m_reg_info.end()) {
                                    // Only update XDump info for Empty and RTL type.
                                    if (    ((*it_reg_info)-> IsMatchType(TYPE_EMP,TYPE_RTL))
                                       &&   ((*it_reg_info)-> GetPeid() == rtl_peid)) {
                                        if ((*it_reg_info)-> GetReg() == cforest_regno) {
                                            uint32_t mask = m_xdump->XDumpDataEle.m_mask & WORD_MASK ;
                                            (*it_reg_info)->SetHasCondition(true);
                                            (*it_reg_info)->SetXDumpMask(mask);
                                        }
                                    }
                                    ++it_reg_info;
                                }
                            }
                        }
                    }
                }
            }
        } //End of checking read memory
        else if ((trace->GetInstId() == INST_ID_V_STCW) || (trace->GetInstId() == INST_ID_V_STCH) || ((trace->GetInstId() == INST_ID_V_STCB))) {
            PhysAddr WAddr = trace->GetWmemAddr(0) ;
            if (m_xdump->IsXDumpReg(WAddr)) {
                CReg cforest_regno = trace->GetWriteReg(0) & REG_ID_MASK;
                CmpRegInfoVec::iterator it_reg_info = m_reg_info.begin ();
                while (it_reg_info != m_reg_info.end()) {
                    // Only update XDump info for Empty and RTL type.
                    if (    ((*it_reg_info)-> IsMatchType(TYPE_EMP,TYPE_RTL))
                       && ((*it_reg_info)-> GetPeid() == rtl_peid)) {
                        if ((*it_reg_info)-> GetReg() == cforest_regno) {
                            (*it_reg_info)->SetHasCondition(true);
                            (*it_reg_info)->SetXDumpMask(WORD_MASK);
                        }
                    }
                    ++it_reg_info;
                }
            }
        }
        // Checking read from System register
        else if ((trace->GetRregCount() > 0)) {
            int32_t i_max = trace->GetRregCount() > trace->GetWregCount() ? trace->GetWregCount()
		                                                          : trace->GetRregCount();
            for (int32_t i_mem = 0; i_mem < i_max ; i_mem++) {
                if ( trace->GetReadReg(i_mem) >= REC_SREG_OFFSET ) {
                    if ( m_xdump->IsXDumpSysreg(trace->GetReadReg(i_mem)) ) {
                        CReg cforest_regno = trace->GetWriteReg(i_mem) & REG_ID_MASK;
                        CmpRegInfoVec::iterator it_reg_info = m_reg_info.begin ();
                        while ( it_reg_info != m_reg_info.end() ) {
                            if ( (*it_reg_info)-> GetPeid() == rtl_peid ) {
                                if ( (*it_reg_info)->GetReg() == cforest_regno ) {
                                    uint32_t mask = m_xdump->XDumpSysregEle.m_mask & WORD_MASK;
                                    (*it_reg_info)->SetHasCondition(XDUMP_CONDITION);
                                    (*it_reg_info)->SetXDumpMask(mask);
                                }
                            }
                            ++it_reg_info;
                        }
                    }
                }
            }
        }
    }
    return ret_val;
}


bool cmprunner::CmpRunnerIF::CheckXDumpInfo (uint32_t rtl_peid, CReg reg_no, RegData iss_wdata_0, RegData iss_wdata_1, RegData iss_wdata_2, RegData iss_wdata_3,
                                                                             RegData rtl_wdata_0, RegData rtl_wdata_1, RegData rtl_wdata_2, RegData rtl_wdata_3,
                                             uint32_t mask_val, bool xdump_arith, uint32_t xdump_diffval, uint32_t rtl_xdump_ctrl, bool is_waw_status)
{
    bool ret_val = true;
    std::ostringstream oss;
    // Apply XDump for Data Load.
    uint32_t rtl_replaced_val_3 = rtl_wdata_3 & ~mask_val;
    uint32_t rtl_replaced_val_2 = rtl_wdata_2 & ~mask_val;
    uint32_t rtl_replaced_val_1 = rtl_wdata_1 & ~mask_val;
    uint32_t rtl_replaced_val_0 = rtl_wdata_0 & ~mask_val;

    uint32_t iss_replaced_val_3 = iss_wdata_3 & ~mask_val;
    uint32_t iss_replaced_val_2 = iss_wdata_2 & ~mask_val;
    uint32_t iss_replaced_val_1 = iss_wdata_1 & ~mask_val;
    uint32_t iss_replaced_val_0 = iss_wdata_0 & ~mask_val;

    // Get the replacement data.
    RegData64 replaced_written = static_cast<uint64_t> (rtl_wdata_0) | (static_cast<uint64_t> (rtl_wdata_1) << 32);
    RegData64 replaced_written_u64 = static_cast<uint64_t> (rtl_wdata_2) | (static_cast<uint64_t> (rtl_wdata_3) << 32);

    std::string XDump_message = "";
    if (rtl_xdump_ctrl) {
        // Apply XDump for control signal
        XDump_message = "XDump for control signal";

    } else if (xdump_arith) {
        if ((abs(rtl_replaced_val_3 - iss_replaced_val_3) <= xdump_diffval)
         && (abs(rtl_replaced_val_2 - iss_replaced_val_2) <= xdump_diffval)
         && (abs(rtl_replaced_val_1 - iss_replaced_val_1) <= xdump_diffval)
         && (abs(rtl_replaced_val_0 - iss_replaced_val_0) <= xdump_diffval)) {
            // Apply XDump for arithmetic operation
            XDump_message = "XDump for arithmetic operation";
        } else {
            XDump_message = "<Error: XDump for arithmetic operation>";
            ret_val = false;
        }
    } else {
        if ((rtl_replaced_val_3 == iss_replaced_val_3)
         && (rtl_replaced_val_2 == iss_replaced_val_2)
         && (rtl_replaced_val_1 == iss_replaced_val_1)
         && (rtl_replaced_val_0 == iss_replaced_val_0)) {
            // Apply XDump for data load
            XDump_message = "XDump data from RTL to Cforest";
        } else {
            XDump_message = "<Error: XDump data from RTL to Cforest>";
            ret_val = false;
        }
    }

    // Print XDump info to CompRunner log
    oss << std::setfill(' ') << std::setw (57) << ' ';
    if (ret_val == true) {
        if (IsGReg(reg_no)) {
            // GReg number
            oss << "GR[";
            oss << std::dec << std::setfill ('0') << std::setw(2) << std::right << reg_no;
            oss << "]=";
            // GReg value
            // Aplly GR value from RTL to CForest.
            oss << std::hex << std::setfill ('0') << std::setw(8) << std::right << iss_wdata_0;
            oss << std::setfill(' ') << std::setw (28) << ' ';
            oss << "  " << XDump_message;
            oss << std::endl;
            // GReg number
            oss << std::setfill(' ') << std::setw (50) << ' ' << " --->  " ;
            oss << std::setfill(' ') << std::setw (7) << ' ';
            oss << std::hex << std::setfill ('0') << std::setw(8) << std::right << rtl_wdata_0;
        } else if ((reg_no >= WR0) && (reg_no <= WR31)) {
            // WReg number
            oss << "WR[";
            oss << std::dec << std::setfill ('0') << std::setw(2) << std::right << reg_no - WREG_BASE;
            oss << "]=";
            // WReg value
            oss << std::hex << std::setfill ('0') << std::setw(8) << std::right << iss_wdata_3 << "_";
            oss << std::hex << std::setfill ('0') << std::setw(8) << std::right << iss_wdata_2 << "_";
            oss << std::hex << std::setfill ('0') << std::setw(8) << std::right << iss_wdata_1 << "_";
            oss << std::hex << std::setfill ('0') << std::setw(8) << std::right << iss_wdata_0;
            oss << " ; " << XDump_message;
            oss << std::endl;
            // WReg number
            oss << std::setfill(' ') << std::setw (50) << ' ' << " --->  " ;
            oss << std::setfill(' ') << std::setw (7) << ' ';
            oss << std::hex << std::setfill ('0') << std::setw(8) << std::right << rtl_wdata_3 << "_";
            oss << std::hex << std::setfill ('0') << std::setw(8) << std::right << rtl_wdata_2 << "_";
            oss << std::hex << std::setfill ('0') << std::setw(8) << std::right << rtl_wdata_1 << "_";
            oss << std::hex << std::setfill ('0') << std::setw(8) << std::right << rtl_wdata_0;
        }


    } else {
        oss << XDump_message;
    }

    // Apply data from RTL to CForest by XDump functions.
    if (is_waw_status) {
        oss << std::endl;
        oss << std::setfill(' ') << std::setw (57) << ' ';
        oss << "<Info: Don't apply XDump due to WAW>";
    } else {
        bool ret_written = RegWriteDebug (rtl_peid, reg_no, replaced_written, replaced_written_u64);
        if (ret_written == false) {
            ret_val = false;
            oss << std::endl;
            oss << std::setfill(' ') << std::setw (57) << ' ';
            oss << "<Error: Can't apply XDump data from RTL to CForest>";
        }
    }

    fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
    return ret_val;
}

bool
cmprunner::CmpRunnerIF::CmpCompFlag (uint32_t rtl_peid, uint32_t rtl_FL_Wen_chk, uint32_t rtl_FL_val_chk, uint32_t iss_FL_Wen, uint32_t iss_FL_val, uint32_t rtl_xdump_ctrl,
                        bool iss_fl_hmode, bool iss_fl_gmode, TraceCtrl *trace) {
    std::ostringstream oss;
    // Only check the flag values which are enable.
    if ((rtl_FL_Wen_chk & rtl_FL_val_chk) != (iss_FL_Wen & iss_FL_val)) {
        // Flag write enable of RTL and Cforest (rtl_FL_Wen_chk and iss_FL_Wen) should be compared before using this function
        bool xdumpflag = false ;
        if (trace->GetRmemCount() > 0) {        //Targets are CAXI and BITOP
            for (int i_mem = 0; i_mem < trace->GetRmemCount(); i_mem++) {
                if (m_xdump->IsXDumpFlag(trace->GetRmemAddr(i_mem))) {
                    xdumpflag = true ;
                }
            }
        }
        if (xdumpflag) {
            oss << std::setfill (' ') << std::setw(70) << ' ' ;
            if (iss_fl_hmode) {
                oss << "Xdump HM flag value from RTL to Cforest" << std::endl ;
                DumpFlag (rtl_peid, rtl_FL_Wen_chk, rtl_FL_val_chk, HMPSW_ID);
            } else if (iss_fl_gmode) {
                oss << "Xdump GM flag value from RTL to Cforest" << std::endl;
                DumpFlag (rtl_peid, rtl_FL_Wen_chk, rtl_FL_val_chk, GMPSW_ID);
            }
            oss << std::setfill (' ') << std::setw(72) << ' ' ;
            oss << "FLAG=" << std::hex << std::setfill ('0') << std::setw(2) << std::right << iss_FL_val << std::endl;
            oss << std::setfill (' ') << std::setw(72) << ' ' << "---> ";
            oss << std::hex << std::setfill ('0') << std::setw(2) << std::right << rtl_FL_val_chk;
            fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
            return true ;
        } else {
            return false ;
        }
    }
    return true ;
}

void cmprunner::CmpRunnerIF::DumpFlag(uint32_t rtl_peid, uint32_t rtl_FL_Wen_chk, uint32_t rtl_FL_val_chk, uint32_t psw_id) {
    RegData64 cforest_psw_data = 0 ;
    RegReadDebug (rtl_peid, psw_id, &cforest_psw_data, NULL) ;
    uint32_t mask_psw_data = 0x0FFFFFFE0UL | ((~rtl_FL_Wen_chk) & 0x01FU) ;
    uint32_t dump_psw_flag = rtl_FL_val_chk & rtl_FL_Wen_chk ;
    cforest_psw_data = (cforest_psw_data & mask_psw_data) | dump_psw_flag ;
    RegWriteDebug (rtl_peid, psw_id, cforest_psw_data, 0);
}

bool cmprunner::CmpRunnerIF::SetMcycMiddleCount(uint32_t rtl_peid, uint32_t cmp_mcyc_middle_cnt)
{
    uint32_t scid = m_forest_util->GetScId (rtl_peid);
    m_forest_util->SetCanselTimingMcycInst(scid, cmp_mcyc_middle_cnt);
    return true;
}


void cmprunner::CmpRunnerIF::StartPy (char *srec_file, uint32_t l1mirr_mode, uint32_t maaen,
                                      std::ostringstream *oss_python_name, char* argv[])
{
    ForestCmdOpt* cmdopt = new ForestCmdOpt (m_forest_util);
    cmdopt->SetScriptName (oss_python_name->str());
    cmdopt->SetMemInit(0x00);
    cmdopt->SetHexName (srec_file);
    // CompRunner function
    cmdopt->SetXDump(m_xdump);
    ForestSetup* forest_setup = new ForestSetup (m_forest_util, cmdopt);
    // CompRunner function
    PythonAPI::StartPy (m_forest_util, cmdopt, forest_setup, 0, NULL);
    // CompRunner function
}


void cmprunner::CmpRunnerIF::SetFileFP (FILE *fp)
{
    // 以前にファイルをopenしていたらcloseする。
    if (m_logfilep != stdout) {
        fclose (m_logfilep);
    }
    m_logfilep = fp;
    m_forest_util->SetFileOut (fp);

    return;
}

void cmprunner::CmpRunnerIF::ResetPE (uint32_t rtl_peid)
{
   m_forest_util->ResetPE(rtl_peid);
}

std::string cmprunner::CmpRunnerIF::GetVersionStr (void)
{
    return  m_forest_util->GetVersionStr ();
}


bool cmprunner::CmpRunnerIF::IsPeIdValid (uint32_t pe_id) const
{
    // Return PEID checking in forest utility function.
    return m_forest_util->IsValidPeId (pe_id);
}


bool cmprunner::CmpRunnerIF::IsByteSizeValid (ByteSize size) const
{
    switch (size) {
    case 1:
    case 2:
    case 4:
    case 8:
        return true;
        break;
    default:
        return false;
        break;
    }
    return true;
}


// Debug function
void cmprunner::CmpRunnerIF::DB_DPI_CmpStep (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_pc, uint32_t rtl_last_commit, uint32_t rtl_c2b1_commit, uint32_t rtl_ooo_commit,
                            uint32_t rtl_mcyc_commit, uint32_t rtl_regwrite_en, uint32_t rtl_reg_num, svLogicVecVal* rtl_gr_val, svLogicVecVal* rtl_wr_val_0, svLogicVecVal* rtl_wr_val_1,
                            svLogicVecVal* rtl_wr_val_2, svLogicVecVal* rtl_wr_val_3, uint32_t rtl_flagwrite_en, uint32_t rtl_flag_val, uint32_t rtl_brk_ch, uint32_t rtl_nblk_id,
                            uint32_t rtl_xdump_ctrl)
{
    std::ostringstream oss;
    oss << std::setfill('-') << std::setw(172) << "" << std::endl;
    // Debug info
    oss << "|| PE" << rtl_peid;
    oss << std::hex << std::setfill('0');
    oss << " [PC=" << std::setw(8) << rtl_pc << "] ||";
    oss << " LAST : ";
    oss << std::dec << std::setfill(' ') << std::setw(4);
    if (rtl_last_commit) {
        oss << "Y ||";
    } else {
        oss << "N ||";
    }
    oss << " C2B1 : ";
    oss << std::dec << std::setfill(' ') << std::setw(4);
    if (rtl_c2b1_commit) {
        oss << "Y ||";
    } else {
        oss << "N ||";
    }
    oss << " NBLK : ";
    oss << std::dec << std::setfill(' ') << std::setw(2);
    if (rtl_ooo_commit) {
        oss << rtl_nblk_id;
    } else {
        oss << "XX";
    }
    oss << " ||";
    // MCYC info
    oss << " MCYC : ";
    oss << std::dec << std::setfill(' ') << std::setw(4);
    if (rtl_mcyc_commit) {
        oss << "Y";
    } else {
        oss << "N";
    }

    // Register info
    oss << " || Reg R";
    if (rtl_regwrite_en) {
        if (rtl_reg_num <= 32) {
            oss << std::dec << std::setfill('0') << std::setw(2) << std::right << rtl_reg_num;
            oss << " = ";
            if (rtl_gr_val->bval) {
                oss << "0x" << std::hex << std::setfill('X') << std::setw(8) << std::right << "";
            } else {
                oss << "0x" << std::hex << std::setfill('0') << std::setw(8) << std::right << rtl_gr_val->aval;
            }
        } else if ((rtl_reg_num >= CmpWRegMin) && (rtl_reg_num <= CmpWRegMax)) {
            oss << std::dec << std::setfill('0') << std::setw(2) << std::right << rtl_reg_num;
            oss << " = 0x";
            if (rtl_wr_val_3->bval) {
                oss << std::hex << std::setfill('X') << std::setw(8) << std::right << "";
            } else {
                oss << std::hex << std::setfill('0') << std::setw(8) << std::right << rtl_wr_val_3->aval;
            }
            oss << "_";
            if (rtl_wr_val_2->bval) {
                oss << std::hex << std::setfill('X') << std::setw(8) << std::right << "";
            } else {
                oss << std::hex << std::setfill('0') << std::setw(8) << std::right << rtl_wr_val_2->aval;
            }
            oss << "_";
            if (rtl_wr_val_1->bval) {
                oss << std::hex << std::setfill('X') << std::setw(8) << std::right << "";
            } else {
                oss << std::hex << std::setfill('0') << std::setw(8) << std::right << rtl_wr_val_1->aval;
            }
            oss << "_";
            if (rtl_wr_val_0->bval) {
                oss << "0x" << std::hex << std::setfill('X') << std::setw(8) << std::right << "";
            } else {
                oss << "0x" << std::hex << std::setfill('0') << std::setw(8) << std::right << rtl_wr_val_0->aval;
            }
        }

    } else {
        oss << "XX";
    }
    oss << " || Flag = ";
    if (rtl_flagwrite_en) {
        oss << std::hex << std::setfill('0') << std::setw(2) << std::right << rtl_flag_val;
    } else {
        oss << "XX";
    }
    oss << " || Brk ch" << std::dec<<rtl_brk_ch;
    oss << " || XDump ctrl : ";
    if (rtl_xdump_ctrl) {
        oss << "Y";
    } else {
        oss << "N";
    }
    oss << " ||" << std::endl;

    oss << std::setfill('-') << std::setw(172) << "" << std::endl;
    fprintf (g_cmp_fp, "%s", oss.str().c_str());
}


// Debug function
void cmprunner::CmpRunnerIF::DB_DPI_CmpNbWb (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_nblk_id, svLogicVecVal* rtl_gr_val,
                                             svLogicVecVal* rtl_wr_val_0, svLogicVecVal* rtl_wr_val_1, svLogicVecVal* rtl_wr_val_2, svLogicVecVal* rtl_wr_val_3)
{
    std::ostringstream oss;
    oss << std::setfill('-') << std::setw(172) << "" << std::endl;
    // Debug info
    oss << "|| PE" << rtl_peid;
    oss << std::hex << std::setfill('0');
    oss << " || NBLK ID = " << std::setw(3) << std::dec << std::setfill('0') << rtl_nblk_id;
    if (rtl_nblk_id <= 0x100) {
        oss << " || GR = 0x";
        if (rtl_gr_val->bval) {
            oss << std::hex << std::setfill('X') << std::setw(8) << std::right << "";
        } else {
            oss << std::hex << std::setfill('0') << std::setw(8) << std::right << rtl_gr_val->aval;
        }
    } else {
        oss << " || WR = 0x";
        if (rtl_wr_val_3->bval) {
            oss << std::hex << std::setfill('X') << std::setw(8) << std::right << "";
        } else {
            oss << std::hex << std::setfill('0') << std::setw(8) << std::right << rtl_wr_val_3->aval;
        }
        oss << "_";
        if (rtl_wr_val_2->bval) {
            oss << std::hex << std::setfill('X') << std::setw(8) << std::right << "";
        } else {
            oss << std::hex << std::setfill('0') << std::setw(8) << std::right << rtl_wr_val_2->aval;
        }
        oss << "_";
        if (rtl_wr_val_1->bval) {
            oss << std::hex << std::setfill('X') << std::setw(8) << std::right << "";
        } else {
            oss << std::hex << std::setfill('0') << std::setw(8) << std::right << rtl_wr_val_1->aval;
        }
        oss << "_";
        if (rtl_wr_val_0->bval) {
            oss << "0x" << std::hex << std::setfill('X') << std::setw(8) << std::right << "";
        } else {
            oss << "0x" << std::hex << std::setfill('0') << std::setw(8) << std::right << rtl_wr_val_0->aval;
        }
    }

    oss << " ||" << std::endl;
    oss << std::setfill('-') << std::setw(172) << "" << std::endl;
    fprintf (g_cmp_fp, "%s", oss.str().c_str());
}

PipeStage
cmprunner::CmpRunnerIF::GetCmpStage (void)
{
    return m_forest_util->GetCmpStage();
}

uint64_t
cmprunner::CmpRunnerIF::GetCycCmpStartPC (void)
{
    return m_forest_util->GetCycCmpStartPC();
}

uint64_t
cmprunner::CmpRunnerIF::GetCycCmpEndPC (void)
{
    return m_forest_util->GetCycCmpEndPC();
}

//! Constructor. Create ForestUtil instance and initialize it.
cmprunner::CmpRunnerIF::CmpRunnerIF ()
{
    m_logfilep = stdout;
    m_exope_mem_outstand = NUM_OUTSTAND;

    m_forest_util = new ForestUtil ();
    m_print_operand = new TracePrintOperand ();
    m_forest_util->CreateForest ();
    m_xdump = new XDUMP(m_forest_util);
    m_idump = new IDUMP(m_forest_util);

    m_forest_util->SetFileOut (g_cmp_fp);
    m_forest_util->SetMessageLevel (MSG_INF);
    m_forest_util->SetCmpStage (STAGE_EA);
    m_forest_util->SetCycCmpStartPC (0x00000000U);
    m_forest_util->SetCycCmpEndPC (WORD_MASK);

    m_comp_misr_show = false;
}


//! Destructor
/*!
  Delete forest/forest_util instance and close debug log file.
*/
cmprunner::CmpRunnerIF::~CmpRunnerIF ()
{
    delete m_forest_util;
    delete m_print_operand;

    // 以前にファイルをopenしていたらcloseする。
    if (m_logfilep != stdout) {
        fclose (m_logfilep);
    }
}
