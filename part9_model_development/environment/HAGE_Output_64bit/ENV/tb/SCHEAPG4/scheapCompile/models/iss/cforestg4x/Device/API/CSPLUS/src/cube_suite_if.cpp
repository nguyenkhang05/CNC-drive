/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <algorithm>
#include <vector>
#include "./config.h"
#include "./forest_common.h"
#include "./forest_utility.h"
#include "./cube_suite_if.h"
#include "./cube_callback.h"


cube::CubeSuiteIF* cube_suite_if = NULL; // for DestroyCoreModule()

/*!
 * @brief Constructor. Creates ForestUtil instance and initialize it.
 */
cube::CubeSuiteIF::CubeSuiteIF ()
{
    m_logfilep = stdout;

    m_forest_util = new ForestUtil ();
    m_forest_util->CreateForest ();

    InitLocalIOROffsetAddr ();
}


/*!
  @brief Sets the core type of designated peid.
  @param cl_id id of the Cluster
  @param pe_id id of the PE(0-7)
  @param core_type the core type on the CORE_TYPE which is included in InitPEInfoID.
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
*/
cube::APIErrorCode cube::CubeSuiteIF::AddPEInfo (uint32_t cl_id, uint32_t pe_id, CoreType core_type)
{
    // Check parameters
    if (IsAddPEInfoValid (pe_id, core_type) == false) {
        return API_E_PARAM;
    }

    // Check duplicated ID
    std::vector<uint32_t>::iterator it
        = std::find (m_pe_id_vec.begin (), m_pe_id_vec.end (), pe_id);
    if (it != m_pe_id_vec.end ()) {
        return API_E_PARAM;
    }
    m_pe_id_vec.push_back (pe_id);

    PEInfo peinfo = {cl_id, pe_id, core_type};
    m_pe_info.push_back (peinfo);

    return API_E_OK;
}


/*!
  @brief Instantiates PE and peripherals.
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM Any instance isn't created
  @retval API_F_NO_WORKAREA Less work memory (bad_alloc exception occurs)
*/
cube::APIErrorCode cube::CubeSuiteIF::InstantiatePE (void)
{
    try {
        if (m_pe_info.empty () == true) {
            return API_E_PARAM;
        }

        std::vector<PEInfo>::const_iterator it = m_pe_info.begin ();
        while (it != m_pe_info.end ()) {
            ::CoreType core_type;
            ::CoreVersion core_version;
            switch ((*it).core_type) {
            case G4MH:  core_type = CORE_G4MH;    core_version = CORE_VER_10; break;
            case G4MH11:  core_type = CORE_G4MH;  core_version = CORE_VER_11; break;
            case G4MH20:  core_type = CORE_G4MH;  core_version = CORE_VER_20; break;
            default:    return API_E_PARAM;
            }
            m_forest_util->SetPe ((*it).cl_id, (*it).pe_id, core_type, core_version);
            ++ it;
        }

        m_forest_util->GenPeripherals ();

        return API_E_OK;
    }
    catch (std::bad_alloc exception) {
        return API_F_NO_WORKAREA;
    }
}


/*!
  @brief Sets the register initial value which configured by Product spec to designated PE.
  @param pe_id PE id (>= 0)
  @param pe_info_id select resource
  @param info_data  data which is selected by pe_info_id
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
*/
cube::APIErrorCode cube::CubeSuiteIF::SetPEInfo (uint32_t pe_id,
                                                 SetPEInfoID pe_info_id,
                                                 uint32_t info_data)
{
    // Check parameters
    if (IsPeIdValid (pe_id) == false) {
        return API_E_PARAM;
    }
    if (IsSetPEInfoValid (pe_info_id, info_data) == false) {
        return API_E_PARAM;
    }

    switch (pe_info_id) {
    case SNOOZE_COUNT:
        m_forest_util->SetExtPin (pe_id, EXT_PIN_SNZ_PERIOD, info_data & 0xFFFF);
        break;
    case FPU_EXIST:
        m_forest_util->SetExtPin (pe_id, EXT_PIN_HAS_FPU, info_data & 1);
        break;
    case FXU_EXIST:
        m_forest_util->SetExtPin (pe_id, EXT_PIN_HAS_FXU, info_data & 1);
        break;
    case MPU_EXIST:
        m_forest_util->SetExtPin (pe_id, EXT_PIN_HAS_MPU, info_data & 1);
        break;
    case MPU_REGION_NUMBER:
        if (info_data <= 32) {
            m_forest_util->SetExtPin (pe_id, EXT_PIN_MPTRGN, info_data);
        } else {
            return API_E_PARAM;
        }
        break;
    case MPU_MDP_SUPPRESSED:
        m_forest_util->SetExtPin (pe_id, EXT_PIN_MDP_DMDP, info_data & 1);
        break;
    case MPU_BANK_NUMBER:
        if ((info_data >= 1) && (info_data <= 4)) { // G4MH2.0 supports 1~4.
            m_forest_util->SetExtPin (pe_id, EXT_PIN_MPBANK, info_data);
        } else {
            return API_E_PARAM;
        }
        break;
    case L1RAM_LINK_NUMBER:
        if ((info_data >= 2) && (info_data <= 8)) { // G4MH2.0 supports 2~8.
            m_forest_util->SetExtPin (pe_id, EXT_PIN_NUM_L1RLINK, info_data);
        } else {
            return API_E_PARAM;
        }
        break;
    case L1RAM_SIZE:
        if ((info_data == 0x20) || (info_data == 0x40)) { // G4MH2.0 support 0x20 or 0x40.
            m_forest_util->SetExtPin (pe_id, EXT_PIN_SIZE_L1R, info_data);
        } else {
            return API_E_PARAM;
        }
        break;
    case LSCFG_LSE:
        m_forest_util->SetExtPin (pe_id, EXT_PIN_LSCFG_LSE, info_data & 1);
        break;
    case HV_EXIST:
        m_forest_util->SetExtPin (pe_id, EXT_PIN_HAS_HV, info_data & 1);
        break;
    case HV_HVE:
        m_forest_util->SetExtPin (pe_id, EXT_PIN_HVE, info_data & 1);
        break;
    case HV_DBGEN:
        m_forest_util->SetExtPin (pe_id, EXT_PIN_DBGEN, info_data & 0x1FFU);
        break;
    case NC_RBASE:
        m_forest_util->SetExtPin (pe_id, EXT_PIN_RBASE, info_data >> 9);
        m_forest_util->SetExtPin (pe_id, EXT_PIN_DV,    info_data >> 1);
        m_forest_util->SetExtPin (pe_id, EXT_PIN_RINT,  info_data);
        break;
    case NC_ICCTRL:
        m_forest_util->SetExtPin (pe_id, EXT_PIN_ICHEN, info_data & 1);
        break;
    case NC_PEID:
        m_forest_util->SetExtPin (pe_id, EXT_PIN_PEID, info_data);
        break;
    case NC_SPID:
        m_forest_util->SetExtPin (pe_id, EXT_PIN_SPID, info_data);
        break;
    case NC_SPIDLIST:
        m_forest_util->SetExtPin (pe_id, EXT_PIN_SPIDLIST, info_data);
        break;
    case NC_BMID:
        m_forest_util->SetExtPin (pe_id, EXT_PIN_BMID, info_data);
        break;
    }

    return API_E_OK;
}


/*!
  @brief Gets processor info from designated PE.
  @param pe_id: PE id (>= 0)
  @param pe_info_id : Select resource
  @param info_data  : data which is selected by pe_info_id
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
*/
cube::APIErrorCode cube::CubeSuiteIF::GetPEInfo (uint32_t pe_id,
                                                 GetPEInfoID pe_info_id,
                                                 uint32_t* info_data)
{
    // Check parameters
    if (IsPeIdValid (pe_id) == false) {
        return API_E_PARAM;
    }
    if (IsGetPEInfoValid (pe_info_id) == false) {
        return API_E_PARAM;
    }

    switch (pe_info_id) {
    case PIPELINE_STAGE_NUMBER:
        uint32_t stage_num = 0;
        ::CoreType core_type = m_forest_util->GetCoreType (pe_id);
        switch (core_type) {
        case CORE_G4MH:    stage_num = PIPE_DEPTH_G4MH;    break;
        default:
            return API_E_PARAM;
            break;
        }
        *info_data = stage_num;
        break;
    }

    return API_E_OK;
}


/*!
  @brief Executes one instruction
  @param trace_info pointer to pointer of trace info. Set Traceinfo pointer to this pointer.
  @param ext_trace_info pointer to pointer of extended trace info. Set ExtTraceinfo pointer to this pointer.
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM No instanciated PE
  @retval API_F_NO_WORKAREA Less work memory (bad_alloc exception occurs)
*/
cube::APIErrorCode cube::CubeSuiteIF::StepExecute (TraceInfo** trace_info,
                                                   ExtTraceInfo** ext_trace_info)
{
    try {
        if (m_forest_util->GetPeNum () == 0) {
            // No instanciated PE
            return API_E_PARAM;
        }

        int32_t scid = m_forest_util->GetCurrentTcScId ();

        // Executes the PE which cycle is most least.
        m_forest_util->CubeStepExecute ();

        // Continues just before next executable PE.
        m_forest_util->SkipHaltHt ();

        // Updates next_xxx info in Trace.
        int32_t next_scid = m_forest_util->GetCurrentTcScId (); // ID of next executed PE

        m_forest_util->UpdateNextTcInfo (scid, next_scid);

        *trace_info  = const_cast<TraceInfo*> (m_forest_util->GetTraceInfo (scid));

        m_ext_trace_info.glbl_systime = m_forest_util->GetSystemTime ();
        m_ext_trace_info.ctxt_systime = m_forest_util->GetCpuTime ((*trace_info)->pe_id);
        *ext_trace_info = &m_ext_trace_info;

        return API_E_OK;
    }
    catch (std::bad_alloc exception) {
        return API_F_NO_WORKAREA;
    }
}


/*!
  @brief Asserts reset
  @param attr range of reset
  @param pe_id PEID
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
  @note Because the next info might be updated by the access,
        GetTraceInfo or etc should be called later to update next_xxx info in a trace info.
*/
cube::APIErrorCode cube::CubeSuiteIF::HardReset (SimAttr attr, uint32_t pe_id)
{
    if (IsSimAttrValid (attr, pe_id) == false) {
        return API_E_PARAM;
    }

    if (attr == ALL_PE) {
        m_forest_util->HardReset ();
    } else if (attr == S_ATTR_PE) {
        m_forest_util->ResetPE (pe_id);
    } else {
        return API_E_PARAM;
    }

    return API_E_OK;
}


/*!
  @brief Writes data to the memory or memory-mapped resources by debugger with logical address of the PE.
  @param pe_id PEID
  @param addr Logical address of the PE
  @param data data to write
  @param size size is 1,2,4,8
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
  @retval API_E_NOT_MAPPED No valid target on the address
  @retval API_F_NO_WORKAREA Less work memory (bad_alloc exception occurs)
  @note Don't update memory write cycle and trace info by this API.
        Because the next info might be updated by the access,
        GetTraceInfo or etc should be called later to update next_xxx info in a trace info.
*/
cube::APIErrorCode cube::CubeSuiteIF::VMemWriteDebug (uint32_t pe_id, PhysAddr addr,
                                                      uint32_t data, ByteSize size)
{
    try {
        // Check parameters
        if (IsPeIdValid (pe_id) == false) {
            return API_E_PARAM;
        }
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
    catch (std::bad_alloc exception) {
        return API_F_NO_WORKAREA;
    }
}


/*!
  @brief Reads data from the memory or memory-mapped resources by debugger with logical address of the PE.
  @param pe_id PEID
  @param addr Logical address of the PE
  @param size size is 1,2,4,8
  @param data pointer to allocate read data
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
  @retval API_E_NOT_MAPPED No valid target on the address
  @note Don't update memory read cycle and trace info by this API.
*/
cube::APIErrorCode cube::CubeSuiteIF::VMemReadDebug (uint32_t pe_id, PhysAddr addr,
                                         ByteSize size, uint32_t* data)
{
    // Check parameters
    if (IsPeIdValid (pe_id) == false) {
        return API_E_PARAM;
    }
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


/*!
  @brief Writes data to the memory or memory-mapped resources by debugger with Physical address.
  @param addr Physical address
  @param data data to write
  @param size size is 1,2,4,8
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
  @retval API_E_NOT_MAPPED No valid target on the address
  @retval API_F_NO_WORKAREA Less work memory (bad_alloc exception occurs)
  @note Don't update memory write cycle and trace info by this API.
        Because the next info might be updated by the access,
        GetTraceInfo or etc should be called later to update next_xxx info in a trace info.
*/
cube::APIErrorCode cube::CubeSuiteIF::PMemWriteDebug (PhysAddr addr,
                                                      uint32_t data, ByteSize size)
{
    try {
        if (IsByteSizeValid (size) == false) {
            return API_E_PARAM;
        }

        m_forest_util->SetCubeErrorCode (::E_OK);

        m_forest_util->BusWriteDebug (addr, reinterpret_cast<MemData8*> (&data), size);

        ::ErrorCode err = m_forest_util->GetCubeErrorCode ();
        if (err != ::E_OK) {
            return API_E_NOT_MAPPED;
        }

        return API_E_OK;
    }
    catch (std::bad_alloc exception) {
        return API_F_NO_WORKAREA;
    }
}


/*!
  @brief Reads data from the memory or memory-mapped resources by debugger with Physical address.
  @param addr Physical address
  @param size size is 1,2,4,8
  @param data pointer to allocate read data
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
  @retval API_E_NOT_MAPPED No valid target on the address
  @note Don't update memory read cycle and trace info by this API.
*/
cube::APIErrorCode cube::CubeSuiteIF::PMemReadDebug (PhysAddr addr,
                                                     ByteSize size, uint32_t* data)
{
    if (IsByteSizeValid (size) == false) {
        return API_E_PARAM;
    }

    m_forest_util->SetCubeErrorCode (::E_OK);

    m_forest_util->BusReadDebug (addr, reinterpret_cast<MemData8*> (data), size);

    ::ErrorCode err = m_forest_util->GetCubeErrorCode ();
    if (err != ::E_OK) {
        return API_E_NOT_MAPPED;
    }

    return API_E_OK;
}


/*!
  @brief Allocates memory block with the type.
  @param id Designate id for the block.
  @param mem_block Designate memory block info (addr, attribute, etc.)
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
  @retval API_E_NOT_ALLOCATED memory block can't be allocated.
  @retval API_E_MULTI_ALLOC Duplicated ID or address overraps with other memory block.
  @retval API_F_NO_WORKAREA Less work memory (bad_alloc exception occurs)
  @note if s_addr<e_addr : (re-)allocate memory block
        if s_addr=e_adddr : delete memory block;
        s_addr>e_addr : illigal parameter and it will not call.
*/
cube::APIErrorCode cube::CubeSuiteIF::SetMemBlock (uint32_t id, MemoryBlock mem_block)
{
    try {
        // Check parameters
        if (IsMemoryBlockValid (&mem_block) == false) {
            return API_E_PARAM;
        }

        if ((mem_block.pe_id != ALLOC_GLOBAL) && (IsPeIdValid (mem_block.pe_id) == false)) {
            return API_E_PARAM;
        }

        if (mem_block.start_address >= mem_block.end_address) {
            return API_E_PARAM;
        }

        // Check duplicated ID and overrapped address
        if (IsEntryDuplicated (id, &mem_block) == true) {
            // There is duplicated entry.
            return API_E_MULTI_ALLOC;
        }

        int32_t num_bank = mem_block.bank_size_count;
        std::vector<ByteSize> bank_size;
        if (num_bank <= 1) {
            // num_bank<=0 is treated as num_bank=1.
            // if num_bank<=1, bank_size is ignored and all area is allocated to a bank.
            bank_size.push_back ((mem_block.end_address - mem_block.start_address) + 1);
        } else {
            uint32_t total_bank_size = 0;
            for (int32_t i = 0; i < num_bank; i++) {
                bank_size.push_back (mem_block.bank_size[i]);
                total_bank_size += bank_size[i];
            }

            uint32_t total_mem_size = static_cast <uint32_t> ((mem_block.end_address - mem_block.start_address) + 1ULL);
            if (total_bank_size != total_mem_size) {
                return API_E_PARAM;
            }
        }

        bool ret;
        MemMappedArea area = mem_block.mblock_area;
        switch (area) {
        case MI_CF_SELF:
            ret = m_forest_util->SetSelfArea ((::PhysAddr)mem_block.start_address,
                                              (::PhysAddr)mem_block.end_address,
                                              (::PhysOffset)mem_block.offset);
            break;
        case MI_CF_IOR:
            ret = m_forest_util->IorResourceAlloc((::MemRestrictType)mem_block.mblock_restrict_type,
                                                  (::PhysAddr)mem_block.start_address,
                                                  (::PhysAddr)mem_block.end_address,
                                                  mem_block.mblock_read_latency,
                                                  mem_block.mblock_write_latency,
                                                  mem_block.cl_read_latency,
                                                  mem_block.cl_write_latency,
                                                  mem_block.exo_cl_read_latency,
                                                  mem_block.exo_cl_write_latency,
                                                  mem_block.pe_id,
                                                  mem_block.cl_id,
                                                  mem_block.num_outstanding,
                                                  mem_block.repeat_rate);
            break;
        case MI_CF_LOCAL_IOR:
            // Even if the address of LOCAL_IOR is overrapped with CFOrest peripherals, no error detected.
            // By the API sepc,
            // when CForest doesn't have valid peripehral on the address, RAM is allocated at the LOCAL_IOR area.
            // when CForest has valid peripehral on the addess, the peripheral is used (no RAM allocated then).
            ret = m_forest_util->LocalIorResourceAlloc(
                    (::MemRestrictType)mem_block.mblock_restrict_type,
                    (::PhysAddr)mem_block.start_address,
                    (::PhysAddr)mem_block.end_address,
                    mem_block.mblock_read_latency,
                    mem_block.mblock_write_latency,
                    mem_block.cl_read_latency,
                    mem_block.cl_write_latency,
                    mem_block.exo_cl_read_latency,
                    mem_block.exo_cl_write_latency,
                    mem_block.pe_id,
                    mem_block.cl_id,
                    mem_block.num_outstanding,
                    mem_block.repeat_rate);
            break;
        default:
            if ((mem_block.mblock_restrict_type & MRT_SELF_ONLY) == MRT_SELF_ONLY) {
                // When MRT_SELF_ONLY attrribute is designated,
                // not only allocating memory, but also setting access denied atribute.
                ret = m_forest_util->SetAccessDeniedArea ((::PhysAddr)mem_block.start_address,
                                                          (::PhysAddr)mem_block.end_address);
                if (ret == false) {
                    break;
                }
            }

            ret = m_forest_util->MemResourceAlloc(
                (::MemRestrictType)mem_block.mblock_restrict_type,
                mem_block.mblock_fetch_latency,
                mem_block.mblock_read_latency,
                mem_block.mblock_write_latency,
                mem_block.cl_fetch_latency,
                mem_block.cl_read_latency,
                mem_block.cl_write_latency,
                mem_block.exo_cl_fetch_latency,
                mem_block.exo_cl_read_latency,
                mem_block.exo_cl_write_latency,
                (::PhysAddr)mem_block.start_address,
                (::PhysAddr)mem_block.end_address,
                (::MemMappedArea)mem_block.mblock_area,
                mem_block.offset,
                mem_block.pe_id,
                mem_block.cl_id,
                mem_block.num_outstanding,
                mem_block.repeat_rate,
                &bank_size);
        }

        if (ret == true) {
            // Allocation is successed.
            // Records new eintry
            MemoryBlockInfo temp_entry;
            temp_entry.id = id;
            temp_entry.mem_attr = mem_block;

            // Copy bank_size array
            temp_entry.mem_attr.bank_size_count = static_cast<int32_t>(bank_size.size ());
            temp_entry.mem_attr.bank_size = new ByteSize [bank_size.size ()];
            int32_t i = 0;
            std::vector<ByteSize>::const_iterator it = bank_size.begin ();
            while (it != bank_size.end ()) {
               temp_entry.mem_attr.bank_size[i] = *it;
                ++ it;
                ++ i;
            }

            m_mem_attr.push_back (temp_entry);
            return API_E_OK;
        } else {
            // Allocation is failed.
            return API_E_MULTI_ALLOC;
        }
    }
    catch (std::bad_alloc exception) {
        return API_F_NO_WORKAREA;
    }
}


/*!
  @brief Gets memory block with the type
  @param id Designate id for the block.
  @param mem_block pointer to memory block info. Set info to this pointer.
  @return error code
  @retval API_E_OK no error
  @retval API_E_NOT_MAPPED No id found
  @note Keep this API, but might not be used in CubeSuite Simulator.
*/
cube::APIErrorCode cube::CubeSuiteIF::GetMemBlock (uint32_t id, MemoryBlock* mem_block)
{
    std::vector<MemoryBlockInfo>::const_iterator it = m_mem_attr.begin();
    while ( it != m_mem_attr.end() ) {
        if ((*it).id == id) {
            *mem_block = (*it).mem_attr;
            return API_E_OK;
        }
        ++ it;
    }
    return API_E_NOT_MAPPED;
}


/*!
  @brief Removes memory block with the type
  @param id Designate id for the block.
  @return error code
  @retval API_E_OK no error
  @retval API_E_NOT_MAPPED No id found
  @note Keep this API, but might not be used in CubeSuite Simulator.
*/
cube::APIErrorCode cube::CubeSuiteIF::DeleteMemBlock (uint32_t id)
{
    bool is_valid_id = false;
    std::vector<MemoryBlockInfo>::iterator it = m_mem_attr.begin();
    while ( it != m_mem_attr.end() ) {
        if ((*it).id == id) {
            is_valid_id = true;
            break;
        }
        ++ it;
    }

    if (is_valid_id == false) {
        return API_E_NOT_MAPPED;
    }

    MemoryBlock mem_block = (*it).mem_attr;
    bool ret;
    MemMappedArea area = mem_block.mblock_area;
    switch (area) {
    case MI_CF_SELF:
        ret = m_forest_util->ClearSelfArea ((::PhysAddr)mem_block.start_address);
        break;
    case MI_CF_IOR:
        ret = m_forest_util->IorResourceFree((::PhysAddr)mem_block.start_address);
        break;
    case MI_CF_LOCAL_IOR:
        ret = m_forest_util->LocalIorResourceFree((::PhysAddr)mem_block.start_address,
                                                  (::PhysAddr)mem_block.end_address);
        break;
    default:
        if ((mem_block.mblock_restrict_type & MRT_SELF_ONLY) == MRT_SELF_ONLY) {
            // When MRT_SELF_ONLY attribute is specified,
            // deletes both the memory and the access denied setting.
            ret = m_forest_util->ClearAccessDeniedArea ((::PhysAddr)mem_block.start_address);
            if (ret == false) {
                break;
            }
        }
        ret = m_forest_util->MemResourceFree((::PhysAddr)mem_block.start_address,
                                             (::PhysAddr)mem_block.end_address);
    }
    if (ret == true) {
        // Removing is successed.
        // Deletes the record for the removed area.
        delete [] (*it).mem_attr.bank_size;
        it = m_mem_attr.erase (it);
        return API_E_OK;
    } else {
        // Removing is failed.
        // This will be an internal error of CForest.
        // To keep consistency of GetMemBlock, deletes the record.
        delete [] (*it).mem_attr.bank_size;
        it = m_mem_attr.erase (it);
        return API_E_NOT_MAPPED;
    }
}


/*!
  @brief Writes to the register from Debugger for GReg and SReg, WReg
  @param pe_id PEID
  @param regno register id
  @param data pointer of data to write
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
  @note no support for VReg
        Because the next info might be updated by the access,
        GetTraceInfo or etc should be called later to update next_xxx info in a trace info.
*/
cube::APIErrorCode cube::CubeSuiteIF::RegWriteDebug (uint32_t pe_id, CReg regno,
                                         const uint32_t* data)
{
    // Check parameters
    if (IsPeIdValid (pe_id) == false) {
        return API_E_PARAM;
    }

    RegData64 data_u64;
    RegData64 data_l64;
    if (regno >= WR0 && regno <= WR31) {
        data_u64 = (static_cast<uint64_t>(data[3]) << 32) | static_cast<uint64_t>(data[2]);
        data_l64 = (static_cast<uint64_t>(data[1]) << 32) | static_cast<uint64_t>(data[0]);
    } else {
        data_u64 = 0;
        data_l64 = static_cast<uint64_t>(data[0]);
    }
    bool ret = m_forest_util->RegWriteDebug (pe_id, regno, data_l64, data_u64);

    if (ret == true){
        return API_E_OK;
    } else {
        return API_E_PARAM;
    }
}


/*!
  @brief Reads from the register from Debugger for GReg and SReg, WReg
  @param pe_id PEID
  @param regno register id
  @param data pointer to data to put read data
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
  @note no support for VReg
*/
cube::APIErrorCode cube::CubeSuiteIF::RegReadDebug (uint32_t pe_id, CReg regno,
                                        uint32_t* data)
{
    // Check parameters
    if (IsPeIdValid (pe_id) == false) {
        return API_E_PARAM;
    }

    bool ret;
    RegData64 data_u64 = 0;
    RegData64 data_l64 = 0;
    ret = m_forest_util->RegReadDebug (pe_id, regno, &data_l64, &data_u64);
    if (regno >= WR0 && regno <= WR31) {
        data[0] = static_cast<uint32_t>((data_l64 >>  0) & 0xFFFFFFFFU);
        data[1] = static_cast<uint32_t>((data_l64 >> 32) & 0xFFFFFFFFU);
        data[2] = static_cast<uint32_t>((data_u64 >>  0) & 0xFFFFFFFFU);
        data[3] = static_cast<uint32_t>((data_u64 >> 32) & 0xFFFFFFFFU);
    } else {
        *data = static_cast<uint32_t>(data_l64 & 0xFFFFFFFFU);
    }
    if (ret == true){
        return API_E_OK;
    } else {
        return API_E_PARAM;
    }
}


/*!
  @brief Sets CacheSpec of instruction cache.
  @param pe_id PEID
  @param cachespec Designate cache spec (size, way, etc).
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
  @retval API_F_NO_WORKAREA Less work memory (bad_alloc exception occurs)
*/
cube::APIErrorCode cube::CubeSuiteIF::SetICacheSpec (uint32_t pe_id, CacheSpec cachespec)
{
    try {
        // Check parameters
        if (IsCacheSpecValid (&cachespec) == false) {
            return API_E_PARAM;
        }
        if (IsPeIdValid (pe_id) == false) {
            return API_E_PARAM;
        }

        ByteSize size;
        if (cachespec.size == CACHE_SIZE_0) {
            size = 0;
        } else {
            size = 512 << static_cast <uint32_t> (cachespec.size);
        }

        m_forest_util->SetIcacheSize (pe_id, size);
        m_forest_util->SetIcacheWayNum (pe_id, static_cast<uint32_t>(cachespec.way));
        m_forest_util->SetIcacheEccState (pe_id, cachespec.ecc == ECC_EXIST);

        return API_E_OK;
    }
    catch (std::bad_alloc exception) {
        return API_F_NO_WORKAREA;
    }
}


/*!
  @brief Gets CacheSpec of instruction cache in specified PE
  @param pe_id PEID
  @param cachespec put cache spec to this pointer.
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
*/
cube::APIErrorCode cube::CubeSuiteIF::GetICacheSpec (uint32_t pe_id,  CacheSpec* cachespec)
{
    // Check parameters
    if (IsPeIdValid (pe_id) == false) {
        return API_E_PARAM;
    }

    // KB単位のキャッシュサイズをEnum型に変換
    ByteSize size = m_forest_util->GetIcacheSize (pe_id) >> 10;
    uint32_t num = 0;
    while (size != 0) {
        size >>= 1;
        ++ num;
    }
    CacheSize ic_size = static_cast <CacheSize> (num);

    cachespec->size = ic_size;
    cachespec->way  = static_cast<CacheWay> (m_forest_util->GetIcacheWayNum (pe_id));
    cachespec->ecc  = static_cast<CacheECC> (m_forest_util->GetIcacheEccState (pe_id));

    return API_E_OK;
}


/*!
  @brief Gets cache access count, both miss and hit
  @param pe_id PEID
  @param cacheinfo put cache hit/miss info to this pointer.
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
*/
cube::APIErrorCode cube::CubeSuiteIF::GetICacheInfo (uint32_t pe_id, CacheInfo* cacheinfo)
{
    // Check parameters
    if (IsPeIdValid (pe_id) == false) {
        return API_E_PARAM;
    }

    cacheinfo->Miss_count = m_forest_util->GetIcacheMissCount (pe_id);
    cacheinfo->Hit_count  = m_forest_util->GetIcacheHitCount (pe_id);
    return API_E_OK;
}


/*!
  @brief Sets debug mode
  @param mode Disignate debug mode.
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
*/
cube::APIErrorCode cube::CubeSuiteIF::SetDebugMode (DebugMode mode)
{
    if (IsDebugModeValid (mode) == false) {
        return API_E_PARAM;
    }

    // Initialize all debug mode at first.
    m_forest_util->DisableDebugOption ("rw");

    // Set new debug mode.
    if (mode == DBG_OFF) {
        m_forest_util->SetDebugMode (::DBG_OFF);
    } else if (mode == DBG_ON) {
        m_forest_util->SetDebugMode (::DBG_ON);
    } else if (mode == DBG_RW) {
        m_forest_util->SetDebugMode (::DBG_RW | ::DBG_ON);
    }

    return API_E_OK;
}


/*!
  @brief Sets file for output log
  @param file_pathname Designate output file for debug info
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
*/
cube::APIErrorCode cube::CubeSuiteIF::SetFileOut (const wchar_t* file_pathname)
{
    FILE* fp;

#ifdef _MSC_VER
    if (_wfopen_s (&fp, file_pathname, L"w") != 0) {
#else // _MSC_VER
    // Implement the API to run the test.
    // Because gcc doesn't have fopen for wchar_t, behavior is unguaranteed.
    if ((fp = fopen ((const char*)file_pathname, "w")) == NULL){
#endif // _MSC_VER
        // error
        return API_E_PARAM;
    }

    // Closes old file
    if (m_logfilep != stdout) {
        fclose (m_logfilep);
    }
    m_logfilep = fp;
    m_forest_util->SetFileOut (fp);

    return API_E_OK;
}


/*!
  @brief Gets TraceInfo and updates next_xxx info in the trace_info
  @param trace pointer to the pointer of trace info. Put trace info pointer to this pointer.
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM no PE instanciated
*/
cube::APIErrorCode cube::CubeSuiteIF::GetTraceInfo (TraceInfo** trace)
{
    if (m_forest_util->GetPeNum () == 0) {
        return API_E_PARAM;
    }

    // Continues to the PE which will be executed at next
    m_forest_util->SkipHaltHt ();

    int32_t scid = m_forest_util->GetCurrentTcScId ();

    // Updates next_xxx info in the TraceInfo
    m_forest_util->UpdateNextTcInfo (scid, scid);
    *trace = const_cast<TraceInfo*> (m_forest_util->GetTraceInfo (scid));

    return API_E_OK;
}


/*!
  @brief Gets next_xxx in designated PE
  @param pe_id PEID
  @param trace pointer to the pointer of trace info. Put trace info pointer to this pointer.
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM invalid pe_id
*/
cube::APIErrorCode cube::CubeSuiteIF::GetPeTraceInfo (uint32_t pe_id, TraceInfo** trace)
{
    if (IsPeIdValid (pe_id) == false) {
        return API_E_PARAM;
    }

    // Continues to the PE which will be executed at next
    m_forest_util->SkipHaltHt ();

    int32_t scid = m_forest_util->GetNextScidOnPe (pe_id);

    // Updates next_xxx info in the TraceInfo
    m_forest_util->UpdateNextTcInfo (scid, scid);
    *trace = const_cast<TraceInfo*> (m_forest_util->GetTraceInfo (scid));

    return API_E_OK;
}


/*!
  @brief Sets clock ratio to designated PE
  @param pe_id PEID
  @param ratio Designate clock ratio. If ratio = 0, PE is stop.
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
*/
cube::APIErrorCode cube::CubeSuiteIF::SetModeFrequency(uint32_t pe_id, uint32_t ratio)
{
    if (IsPeIdValid (pe_id) == false) {
        return API_E_PARAM;
    }

    m_forest_util->SetFrequency (pe_id, ratio);

    return API_E_OK;
}


/*!
  @brief Notices interruption request to intc1
  @param pe_id PEID (>= 0)
  @param channel interruption channel number (0~31)
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
  @retval API_E_MULTI_INTREQ interruption is not received
*/
cube::APIErrorCode cube::CubeSuiteIF::SetInterruptToIntc1 (uint32_t pe_id, uint32_t channel)
{
    if (IsPeIdValid (pe_id) == false) {
        return API_E_PARAM;
    }

    ::IntErrorCode err = m_forest_util->ReqInterruptToIntc1 (pe_id, channel);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_CHANNEL:      return API_E_PARAM;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM; // unreachable
    }
}


/*!
  @brief Notices interruption request to intc2
  @param channel interruption channel number (32~2047)
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
  @retval API_E_MULTI_INTREQ interruption is not received
*/
cube::APIErrorCode cube::CubeSuiteIF::SetInterruptToIntc2 (uint32_t channel)
{
    ::IntErrorCode err = m_forest_util->ReqInterruptToIntc2 (channel);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_CHANNEL:      return API_E_PARAM;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM; // unreachable
    }
}


/*!
  @brief Notices FEINT request to intc1
  @param pe_id PEID (>= 0)
  @param channel interruption channel number (0~15)
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
  @retval API_E_MULTI_INTREQ interruption is not received
*/
cube::APIErrorCode cube::CubeSuiteIF::SetFeintToIntc1 (uint32_t pe_id, uint32_t channel)
{
    if (IsPeIdValid (pe_id) == false) {
        return API_E_PARAM;
    }

    if (channel > MAX_INT_CHANNEL_FEINT) {
        return API_E_PARAM;
    }

    ::IntErrorCode err = m_forest_util->ReqFeintToIntc1 (pe_id, channel);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_CHANNEL:      return API_E_PARAM;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM; // unreachable
    }
}


/*!
  @brief Notices interruption request to intc1
  @param pe_id PEID (>= 0)
  @param channel interruption channel number (0~31)
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
  @retval API_E_MULTI_INTREQ interruption is not received
*/
cube::APIErrorCode cube::CubeSuiteIF::SetFenmiToIntc1 (uint32_t pe_id)
{
    if (IsPeIdValid (pe_id) == false) {
        return API_E_PARAM;
    }

    ::IntErrorCode err = m_forest_util->ReqFenmiToIntc1 (pe_id);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_CHANNEL:      return API_E_PARAM;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM; // unreachable
    }
}


/*!
  @brief Sets base address for designated peripheral
  @param peri_id   designate Peripheral ID
  @param base_addr base address by physical address
  @return error code
  @retval API_E_OK: no error
  @retval API_E_PARAM: illegal Peripheral ID
*/
cube::APIErrorCode cube::CubeSuiteIF::SetIORBaseAddr (SetIORBaseAddrID peri_id,
                                                      PhysAddr base_addr)
{
    if (IsSetIORBaseAddrIDValid (peri_id) == false) {
        return API_E_PARAM;
    }

    m_forest_util->SetPeripheralBaseAddr (static_cast< ::SetIORBaseAddrID>(peri_id),
                                          base_addr);
    m_forest_util->EnablePeripheral (static_cast< ::SetIORBaseAddrID>(peri_id),
                                          true);

    // Update offset address again when peripheral base address is updated by this API.
    UpdateLocalIOROffsetAddr ();

    return API_E_OK;
}


/*!
  @brief Sets LIOR offset for designated area (peripheral)
  @param start_address   address by physical address
  @param end_addr address by physical address
  @param offset offset address for self area
  @return error code
  @retval API_E_OK: no error
  @retval API_E_PARAM: illegal parameter
*/
cube::APIErrorCode cube::CubeSuiteIF::SetLocalIOROffsetAddr (PhysAddr start_addr,
                                                             PhysAddr end_addr,
                                                             int32_t offset)
{
    if (start_addr > end_addr) {
        return API_E_PARAM;
    }

    OffsetInfo offset_info = {start_addr, end_addr, offset};
    std::vector<OffsetInfo>::const_iterator it = m_offset_vec.begin();
    while (it != m_offset_vec.end()) {
        if (IsOffsetOverlaped (&offset_info, &(*it))) {
            // Overrapped with existed area
            return API_E_MULTI_ALLOC;
        }
        ++it;
    }

    // Keep setting.
    // - Check overrapped setting
    // - Apply setting again when peripheral base address is updated by SetIORBaseAddr() later.
    m_offset_vec.push_back (offset_info);

    // Update offset address for all peripherals within the address area.
    UpdateLocalIOROffsetAddr ();

    return API_E_OK;
}


/*!
  @brief Initializes LIOR offset record
*/
void cube::CubeSuiteIF::InitLocalIOROffsetAddr (void)
{
    // Initialize offset settings to 0 for all peripherals which supported by CS+ API spec.
    m_forest_util->SetPeripheralOffsetAddr (::BaseAddr_INTC1, 0);
    m_forest_util->SetPeripheralOffsetAddr (::BaseAddr_INTC2, 0);
    m_forest_util->SetPeripheralOffsetAddr (::BaseAddr_MECNT, 0);
    m_forest_util->SetPeripheralOffsetAddr (::BaseAddr_IPIR, 0);
#ifdef ENABLE_BARRIER_CSP
    m_forest_util->SetPeripheralOffsetAddr (::BaseAddr_BAR, 0);
#endif // #ifdef ENABLE_BARRIER_CSP
}


/*!
  @brief Updates offset address of the peripheral
  @param id id of the physical
  @param info offset info
*/
void cube::CubeSuiteIF::UpdatePeripheralOffset (SetIORBaseAddrID id, OffsetInfo* info)
{
    ::SetIORBaseAddrID cfid = static_cast < ::SetIORBaseAddrID>(id);
    PhysAddr base_addr = m_forest_util->GetPeripheralBaseAddr (cfid);
    if ((info->m_start <= base_addr) && (info->m_end >= base_addr)) {
        // Found resource. Updates offset address.
        m_forest_util->SetPeripheralOffsetAddr (cfid, info->m_offset);
    }
}


/*!
  @brief Updates LIOR offset record
  @note  The offset address is set by address area.
         When base address of peripheral is updated,
         need to update offset address for the peripheral.
         For easy implement, update for all peripherals.
*/
void cube::CubeSuiteIF::UpdateLocalIOROffsetAddr (void)
{
    // Initialize current setting.
    // Because when base address is updated, current offset address might be invalid.
    InitLocalIOROffsetAddr ();

    // Set offset address by using offset settings in m_offset_vec and base address settings.
    std::vector<OffsetInfo>::iterator it = m_offset_vec.begin ();
    while (it != m_offset_vec.end ()) {
        UpdatePeripheralOffset (BaseAddr_INTC1, &(*it));
        UpdatePeripheralOffset (BaseAddr_INTC2, &(*it));
        UpdatePeripheralOffset (BaseAddr_MECNT, &(*it));
        UpdatePeripheralOffset (BaseAddr_IPIR , &(*it));
#ifdef ENABLE_BARRIER_CSP
        UpdatePeripheralOffset (BaseAddr_BAR  , &(*it));
#endif // #ifdef ENABLE_BARRIER_CSP
        ++it;
    }
}


/*!
  @brief Notices pseudo eiint request to cpu (INTC is bypassed)
  @param pe_id PEID (>= 0)
  @param channel interruption channel number (0~2047)
  @param priority interruption priority (0~15)
  @param eitb true : table refer system, false : direct vector system
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter or called for G4MH2.0
  @retval API_E_MULTI_INTREQ interruption is not received
*/
cube::APIErrorCode cube::CubeSuiteIF::ReqPseudoEiint (uint32_t pe_id,
                                                      uint32_t channel,
                                                      uint32_t priority, bool eitb)
{
    // Check parameters
    if (IsPeIdValid (pe_id) == false) {
        return API_E_PARAM;
    }

    // G4MH2.x or later isn't supported in this API
    ::CoreType core_type = m_forest_util->GetCoreType (pe_id);
    ::CoreVersion core_ver = m_forest_util->GetCoreVersion (pe_id);
    if ((core_type == ::CORE_G4MH) && (core_ver >= ::CORE_VER_20)) {
        return API_E_PARAM;
    }

    // channel is checked by ForestUtil side.
    if (priority > MAX_INT_PRIORITY_G4MH1X) {
        // Becasue ReqPseudoEiint doesn't support G4MH2.0, no need to support 256/64level priority.
        return API_E_PARAM;
    }

    ::IntErrorCode err = m_forest_util->ReqInterruptToCpu (pe_id, channel, priority, eitb, false, 0, false);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_CHANNEL:      return API_E_PARAM;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM; // unreachable
    }
}


/*!
  @brief Notices pseudo fenmi request to cpu (intc is bypassed)
  @param pe_id PEID (>= 0)
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter or called for G4MH2.0
  @retval API_E_MULTI_INTREQ interruption is not received
*/
cube::APIErrorCode cube::CubeSuiteIF::ReqPseudoFenmi (uint32_t pe_id)
{
    // Check parameters
    if (IsPeIdValid (pe_id) == false) {
        return API_E_PARAM;
    }

    // G4MH2.x or later isn't supported in this API
    ::CoreType core_type = m_forest_util->GetCoreType (pe_id);
    ::CoreVersion core_ver = m_forest_util->GetCoreVersion (pe_id);
    if ((core_type == ::CORE_G4MH) && (core_ver >= ::CORE_VER_20)) {
        return API_E_PARAM;
    }

    ::IntErrorCode err = m_forest_util->ReqFenmiToCpu (pe_id);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM; // unreachable
    }
}


/*!
  @brief Notices pseudo feint request to cpu (intc is bypassed)
  @param pe_id PEID (>= 0)
  @param channel the channel id of feint (0-15)
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter or called for G4MH2.0
  @retval API_E_MULTI_INTREQ interruption is not received
*/
cube::APIErrorCode cube::CubeSuiteIF::ReqPseudoFeint (uint32_t pe_id, uint32_t channel)
{
    // Check parameters
    if (IsPeIdValid (pe_id) == false) {
        return API_E_PARAM;
    }

    // G4MH2.x or later isn't supported in this API
    ::CoreType core_type = m_forest_util->GetCoreType (pe_id);
    ::CoreVersion core_ver = m_forest_util->GetCoreVersion (pe_id);
    if ((core_type == ::CORE_G4MH) && (core_ver >= ::CORE_VER_20)) {
        return API_E_PARAM;
    }

    ::IntErrorCode err = m_forest_util->ReqFeintToCpu (pe_id, channel, false, 0, false);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_NOT_RECEIVED:
        if (channel > MAX_INT_CHANNEL_FEINT) {
            return API_E_PARAM;
        } else {
            return API_E_MULTI_INTREQ;
        }
    default:                   return API_E_PARAM;
    }
}


/*!
  @brief Notices pseudo system error (caused by instruction fetch) to cpu
  @param pe_id PEID (>= 0)
  @param syserr_cause System error code (0x10-1F)
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
  @retval API_E_MULTI_INTREQ system error is not received
*/
cube::APIErrorCode cube::CubeSuiteIF::ReqPseudoSyserr (uint32_t pe_id,
                                                       uint32_t syserr_cause)
{
    // Check parameters
    if (IsPeIdValid (pe_id) == false) {
        return API_E_PARAM;
    }

    ::IntErrorCode err = m_forest_util->ReqPseudoFetchSyserrToCpu (pe_id, syserr_cause);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_SYSERR_CODE:  return API_E_PARAM;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM; // unreachable
    }
}


/*!
  @brief Notices pseudo operand system error to cpu
  @param pe_id PEID (>= 0)
  @param syserr_cause System error code (0x10-1F)
  @param is_gm true=Guest mode, false=Host mode or conventional mode
  @param gpid GPID of guest mode. 0 for Host mode or conventional mode
  @param syserr_cause System error code
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM there is illigal parameter
  @retval API_E_MULTI_INTREQ system error is not received
*/
cube::APIErrorCode cube::CubeSuiteIF::ReqPseudoOperandSyserr (uint32_t pe_id, uint32_t syserr_cause, bool is_gm, uint32_t gpid)
{
    // Check arguments
    if (IsPeIdValid (pe_id) == false) {
        return API_E_PARAM;
    }

    ::IntErrorCode err = m_forest_util->NoticeOperandErrorResponse (pe_id, syserr_cause, is_gm, gpid);
    switch (err) {
    case ::INT_E_OK:           return API_E_OK;
    case ::INT_E_CHANNEL:      return API_E_PARAM;
    case ::INT_E_SYSERR_CODE:  return API_E_PARAM;
    case ::INT_E_NOT_RECEIVED: return API_E_MULTI_INTREQ;
    default:                   return API_E_PARAM; // unreachable
    }
}


/*!
  @brief Checks peri_id argument of API
  @param peri_id peripheral id to check
  @return true=NoError, false=Error
  If undefined id is used, error occurs.
*/
bool cube::CubeSuiteIF::IsSetIORBaseAddrIDValid (SetIORBaseAddrID peri_id) const
{
    switch (peri_id) {
    case BaseAddr_INTC1:
    case BaseAddr_INTC2:
    case BaseAddr_MECNT:
    case BaseAddr_IPIR:
#ifdef ENABLE_BARRIER_CSP
    case BaseAddr_BAR:
#endif // #ifdef ENABLE_BARRIER_CSP
        return true;
        break;
    default:
        return false;
    }
}


/*!
  @brief Checks pe_id argument of API
  @param pe_id PEID to check
  @return true=Exist the PE with PEID, false=None exist the PE with PEID
*/
bool cube::CubeSuiteIF::IsPeIdValid (uint32_t pe_id) const
{
    return m_forest_util->IsValidPeId (pe_id);
}


/*!
  @brief Checks InitPEInfoID and info_data content.
  @param pe_id id of the PE(0-7)
  @param core_type the core type on the CORE_TYPE which is included in InitPEInfoID.
  @return true=NoError, false=Error
  Error occur if unknown pe_info_id, or illegal info_data for the pe_info_id.
*/
bool cube::CubeSuiteIF::IsAddPEInfoValid (uint32_t pe_id,
                                          uint32_t core_type) const
{
    if (pe_id > 7) {
        return false; // error
    }

    switch (core_type) {
    case G4MH:
    case G4MH11:
    case G4MH20:
        break;
    default:
        return false;  // error
        break;
    }

    return true;
}


/*!
  @brief Checks SetPEInfoID
  @param pe_info_id resource ID
  @param info_data data of the resource
  @return true=NoError, false=Error
  Error occur if unknown pe_info_id, or data is wrong
*/
bool cube::CubeSuiteIF::IsSetPEInfoValid (SetPEInfoID pe_info_id, uint32_t info_data) const
{
    switch (pe_info_id) {
    case SNOOZE_COUNT:
    case MPU_REGION_NUMBER:
    case NC_RBASE:
    case NC_ICCTRL:
    case NC_PEID:
    case NC_SPID:
    case NC_SPIDLIST:
    case NC_BMID:
    case MPU_BANK_NUMBER:
    case L1RAM_LINK_NUMBER:
    case L1RAM_SIZE:
    case HV_DBGEN:
        return true;
        break;
    case FPU_EXIST:
    case FXU_EXIST:
    case MPU_EXIST:
    case MPU_MDP_SUPPRESSED:
    case LSCFG_LSE:
    case HV_EXIST:
    case HV_HVE:
        switch (info_data) {
        case 0:
        case 1:
            return true;
            break;
        default:
            return false;
            break;
        }
    default:
        return false;
    }
};


/*!
  @brief Checks GetPEInfoID
  @return true=NoError, false=Error
  Error occur if unknown pe_info_id,
*/
bool cube::CubeSuiteIF::IsGetPEInfoValid (GetPEInfoID pe_info_id) const
{
    switch (pe_info_id) {
    case PIPELINE_STAGE_NUMBER: return true; break;
    default: return false;
    }
};


/*!
  @brief Checks SimAttr and id content.
  @return true=NoError, false=Error
  Error occur if unknown attr or illegal pe_id.
*/
bool cube::CubeSuiteIF::IsSimAttrValid (SimAttr attr, uint32_t pe_id) const
{
    if (attr != ALL_PE) {
        if (IsPeIdValid (pe_id) == false) {
            return false;
        }
    }

    switch (attr) {
    case ALL_PE:    return true; break;
    case S_ATTR_PE: return true; break;
    default:
        return true;
        break;
    }
};


/*!
  @brief Checks CacheSpec content.
  @return true=NoError, false=Error
  Error occur if unknown val is used in cachespec.
*/
bool cube::CubeSuiteIF::IsCacheSpecValid (CacheSpec* cachespec) const
{
    switch (cachespec->size) {
    case CACHE_SIZE_0:
    //case CACHE_SIZE_1K:
    //case CACHE_SIZE_2K:
    //case CACHE_SIZE_4K:
    case CACHE_SIZE_8K:
    case CACHE_SIZE_16K:
    case CACHE_SIZE_32K:
    //case CACHE_SIZE_64K:
        break;
    default:
        // error
        return false;
    }

    switch (cachespec->way) {
    case CACHE_WAY_0:
    //case CACHE_WAY_1:
    //case CACHE_WAY_2:
    case CACHE_WAY_4:
        break;
    default:
        // error
        return false;
    }

    switch (cachespec->ecc) {
    case ECC_NO_EXIST:
    case ECC_EXIST:
        break;
    default:
        // error
        return false;
    }

    // No error
    return true;
}


/*!
  @brief Checks MemoryBlock content.
  @param mem_block memory block parameter to be checked
  @return true=NoError, false=Error
  Error occur if unknown val is used in mem_block
*/
bool cube::CubeSuiteIF::IsMemoryBlockValid (MemoryBlock* mem_block) const
{
    uint32_t mask = ~(MRT_READ_ENABLE | MRT_WRITE_ENABLE | MRT_FETCH_ENABLE |
                      MRT_SELF_ONLY | MRT_OWN_PE_ONLY);

    if ( (mem_block->mblock_restrict_type & mask) != 0x0 ) {
        return false;
    }

    switch (mem_block->mblock_area) {
    case MI_CF_SELF:      /* Self area */
        if ((mem_block->mblock_restrict_type & MRT_SELF_ONLY) != 0x0) {
            // MRT_SELF_ONLY is valid only for memory area
            return false;
        }

    case MI_CF_IOR:       /* IOR area */
    case MI_CF_LOCAL_IOR: /* Local APB IOR area */
        if ((mem_block->mblock_restrict_type & MRT_SELF_ONLY) != 0x0) {
            // MRT_SELF_ONLY is valid only for memory area
            return false;
        }
        if (mem_block->num_outstanding == 0) {
            // Outstand must be 1 or more
            return false;
        }
        break;
    case MI_CF_FROM:      /* FlashROM area */
    case MI_CF_LRAM:      /* LocalRAM area */
    case MI_CF_CRAM:      /* CRAM area */
    case MI_CF_OTHER:     /* External (exo-cl) area */
        if (mem_block->num_outstanding == 0) {
            // Outstand must be 1 or more
            return false;
        }
        break;
    default:
        return false;
    }

    // No error
    return true;
}


/*!
  @brief Checks duplicated MemoryBlock content.
  @param id id of the memory block to be checked
  @param mem_block memory block parameter to be checked
  @return true=NoError, false=Error
  Error occur if MemoryBlock is already allocated by SetMemBlockAlloc
*/
bool cube::CubeSuiteIF::IsEntryDuplicated (uint32_t id, MemoryBlock* mem_block) const
{
    // Checks duplicated entry
    std::vector<MemoryBlockInfo>::const_iterator it = m_mem_attr.begin();
    while ( it != m_mem_attr.end() ) {
        // duplicated id
        if ((*it).id == id) {
            return true;
        }

        // address overrapped
        if (IsAddrOverlaped (mem_block, &((*it).mem_attr)) == true) {
            return true;
        }
        ++ it;
    }
    return false;
}


/*!
  @brief checks address overrapping between 2 areas
  @param mem_block1 1st memory block parameter
  @param mem_block2 2nd memory block parameter
  @return true=NoOverrapped, false=Overrapped
*/
bool cube::CubeSuiteIF::IsAddrOverlaped (const MemoryBlock* mem_block1,
                                         const MemoryBlock* mem_block2) const
{
    PhysAddr start_addr1 = mem_block1->start_address;
    PhysAddr end_addr1   = mem_block1->end_address;
    PhysAddr start_addr2 = mem_block2->start_address;
    PhysAddr end_addr2   = mem_block2->end_address;

    if (  ((start_addr1 <= start_addr2) && (start_addr2 <= end_addr1)) // a part overrapped (area1)
       || ((start_addr1 >= start_addr2) && (start_addr1 <= end_addr2)) // a part overrapped (area2)
       || ((start_addr1 <= start_addr2) && (end_addr1 >= end_addr2)) // area1 includes area 2
       || ((start_addr1 >= start_addr2) && (end_addr1 <= end_addr2)) // area2 includes area 1
    ) {
        // Overrapped
        return true;
    } else {
        return false;
    }
}


/*!
  @brief checks address overrapping between 2 areas
  @param offset_info1 1st offset parameter
  @param offset_info2 2nd offset parameter
  @return true=NoOverrapped, false=Overrapped
*/
bool cube::CubeSuiteIF::IsOffsetOverlaped (const OffsetInfo* offset_info1,
                                           const OffsetInfo* offset_info2) const
{
    PhysAddr start_addr1 = offset_info1->m_start;
    PhysAddr end_addr1   = offset_info1->m_end;
    PhysAddr start_addr2 = offset_info2->m_start;
    PhysAddr end_addr2   = offset_info2->m_end;

    if (  ((start_addr1 <= start_addr2) && (start_addr2 <= end_addr1)) // a part overrapped (area1)
       || ((start_addr1 >= start_addr2) && (start_addr1 <= end_addr2)) // a part overrapped (area2)
       || ((start_addr1 <= start_addr2) && (end_addr1 >= end_addr2)) // area1 includes area 2
       || ((start_addr1 >= start_addr2) && (end_addr1 <= end_addr2)) // area2 includes area 1
    ) {
        // Overrapped
        return true;
    } else {
        return false;
    }
}


/*!
  @brief Checks DebugMode content.
  @param mode debug mode
  @return true=NoError, false=Error
  Error occur if unknown val is used in mode
*/
bool cube::CubeSuiteIF::IsDebugModeValid (DebugMode mode) const
{
    return (mode == DBG_ON) || (mode == DBG_OFF) || (mode == DBG_RW);
}


/*!
  @brief Checks ByteSize content for VMemRead/Write, PMemRead/Write.
  @param size memory access size
  @return true=NoError, false=Error
  Error occur if size isn't 1/2/4/8.
*/
bool cube::CubeSuiteIF::IsByteSizeValid (ByteSize size) const
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
}


/*!
  @brief Destructor. Deletes forest/forest_util instance and close debug log file.
*/
cube::CubeSuiteIF::~CubeSuiteIF ()
{
    delete m_forest_util;

    std::vector<MemoryBlockInfo>::iterator it = m_mem_attr.begin();
    while ( it != m_mem_attr.end() ) {
        delete [] (*it).mem_attr.bank_size;
        ++ it;
    }
    m_mem_attr.clear ();

    // close opened file
    if (m_logfilep != stdout) {
        fclose (m_logfilep);
    }
}


/*!
  @brief creates Cforest objects
  @param dllname Designate dll name to get call back function.
  @return pointer to CubeSuiteIF instance. (casted to CForestIF interface class)
  @retval not NULL pointer to CubeSuiteIF instance (No error)
  @retval NULL Failed to open dll
*/
cube::CForestIF* cube::MakeCoreModule (const wchar_t* dllname)
{
    try {
        if (CubeCallBack::OpenDllFunction (dllname) != ::API_E_OK) {
            return NULL;
        }

        if (cube_suite_if == NULL){
            cube_suite_if = new CubeSuiteIF;
        }
        return static_cast<CForestIF*> (cube_suite_if);
    }
    catch (std::bad_alloc exception) {
        return NULL;
    }
}


/*!
  @brief Deletes Cforest objects and close call back function.
  @return error code
  @retval API_E_OK no error
  @retval API_E_PARAM error in close dll. (no need now)
*/
cube::APIErrorCode cube::DestroyCoreModule(void)
{
    if (cube_suite_if != NULL){
        delete cube_suite_if;
        cube_suite_if = NULL;
    }

    if (CubeCallBack::CloseDllFunction () != ::API_E_OK) {
        return API_E_PARAM;
    }
    return API_E_OK;
}
