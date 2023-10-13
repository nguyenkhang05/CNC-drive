#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <Python.h>
#include "./config.h"
#include "./forest_utility.h"
#include "./forest_cmdopt.h"
#include "./trace_info.h"
#include "./forest_setup.h"
#include "./forest_python.h"
#include "./inst_id_list.h"
#include "./regid.h"
#include "./frog_if.h"
#include "./frog_internals.h"
#include "./trace_operand.h"
#include "./trace_print.h"
#include "./exp_info.h"
#include "./forest_message.h"


/** \file frog_if.cpp
 * \brief FROG interface source file.
 *
 * This file contains the implementation of API common functions for FROG tool.
 * FROG tool will use these functions to connect to CForest simulator.
 * Thus, FROG tool can understand the current status in CForest and generate code step by step for its sequence.
 *
 */


class ForestUtil;
class ForestCmdOpt;
class ForestSetup;

/**
 * \var ForestUtil         * m_forest_util
 * \brief Pointer to ForestUtil class.
 *
 */
ForestUtil         * m_forest_util = NULL;
/**
 * \var ForestCmdOpt       * m_cmdopt
 * \brief Pointer to ForestCmdOpt class.
 *
 */
ForestCmdOpt       * m_cmdopt = NULL;
/**
 * \var std::ostream       &gcsMsg
 * \brief Show message to STDOUT.
 *
 */
std::ostream       &gcsMsg = std::cout;
/**
 * \var CfrogRegWriteRecord* g_reg_record
 * \brief Pointer to CfrogRegWriteRecord class.
 *
 */
CfrogRegWriteRecord* g_reg_record = nullptr;
/**
 * \var ExpCode g_last_exec_exp_code
 * \brief Exception code.
 *
 */
ExpCode g_last_exec_exp_code = EXP_NONE;

// Map of register profile
/**
 * \var std::map <std::string, std::vector <FrogRegData > > p_prev_reg_frog
 * \brief Pointer to previous register in FROG I/F.
 *
 */
std::map <std::string, std::vector <FrogRegData > > p_prev_reg_frog;
/**
 * \var std::map<std::string, FrogRegPro*> frog_reg_map
 * \brief Pointer to Reg map in FROG I/F.
 *
 */
extern std::map<std::string, FrogRegPro*> frog_reg_map;
#ifdef FROG_API_G4MH_UT
extern std::string g_get_opcode;
#endif

static const uint32_t NC_pos  = 0;
static const uint32_t VT0_pos = 1;
static const uint32_t LLBIT_DEL_SIZE = 0;

static std::vector< std::pair <PhysAddr, uint64_t> > m_pre_mem_vector;

// SR ID is 0x100 + (selID << 5) + regID
static const CReg SR_PSW = 0x105;
static const CReg SR_GMPSW = 0x225; // selID=9, regID=5
static const CReg SR_SESR = 0x10C;
static const CReg SR_PSWH = 0x10F;  // selID=0, regID=15
static const CReg SR_HVCFG = 0x130; // selID=1, regID=16

static std::string gcs_internal_build_comment(uint32_t ptid, int32_t isNative, TraceCtrl *trace);
static std::string gcs_internal_get_frog_name (CReg regno);

static GCS_CB_COMMENT       CbComment;
static GCS_CB_LLBIT         CbLLbit;
static GCS_CB_MEMORY_EX     CbMemoryEx;
static GCS_CB_NC_REG_EX     CbNcRegEx;
static GCS_CB_VC_REG_EX     CbVcRegEx;
static GCS_CB_TC_REG_EX     CbTcRegEx;
static GCS_CB_TLB_EX        CbTlbEx;

/**
 * \var enum IsCallCBType
 * \brief The enumeration for call-back usage.
 *
 */
enum IsCallCBType {
    ICT_NOT_CALL_CB = 0,
    ICT_CALL_CB = 1
};

static int gcs_internal_get_register (uint32_t peid, int32_t tcid, const std::string &reg_name, FrogRegData* data);
template <IsCallCBType is_call_cb>
static int gcs_internal_compare_and_update_register_frogif (uint32_t peid, int32_t tcid, const std::string &reg_name);
static int gcs_internal_update_referenced_register (uint32_t peid, int32_t tcid, const std::string &reg_name);


/**
 * \brief This function initializes the previous value of register.
 *
 * @param[in] pe_id     access address
 * @param[in] core_type current memory value
*/
bool InitPreValReg(uint32_t pe_id, CoreType core_type){
    if (core_type == CORE_G4MH) {
        uint64_t init_val_NC = 0x00;
        std::vector <FrogRegData> pre_val;
        bool sreg_ret_NC;

    p_prev_reg_frog.clear ();

        std::map<std::string, FrogRegPro*>::iterator it_frog_reg_map = frog_reg_map.begin();
        for (int i = 0; it_frog_reg_map != frog_reg_map.end (); ++it_frog_reg_map, i++){
            FrogRegPro *reg_profile = it_frog_reg_map->second;
            if (reg_profile->m_creg <= DATA_REG_MAX) {
                init_val_NC = 0x00;
            } else {
                if ((reg_profile->m_context).compare("V") == 0) {
                    continue;
                } else {
                    sreg_ret_NC = m_forest_util->RegReadDebug (pe_id, reg_profile->m_creg, (&init_val_NC));
                }
                if (sreg_ret_NC == false) {
                    init_val_NC = 0x00;
                }
            }
            pre_val.push_back((FrogRegData)init_val_NC);
            p_prev_reg_frog.insert(std::pair<std::string, std::vector <FrogRegData> > (reg_profile->m_reg_name, pre_val));
            pre_val.resize(0);
        }
    }
    return true;
}


bool gcs_get_register_profile (const std::string &reg_name, FrogRegPro *out) {
    if (out != NULL) {
        std::string name_copy; //(boost::to_upper_copy<std::string>(reg_name));
        ToUpperCopy (reg_name, &name_copy);
        std::map<std::string, FrogRegPro*>::iterator it_frog_reg_map = frog_reg_map.find(name_copy);
        if (it_frog_reg_map == frog_reg_map.end()) {
            return false;
        }
        *out = *(it_frog_reg_map->second);
        return true;
    } else
        return false;
}


bool gcs_get_sreg_name(uint32_t selID, uint32_t regID, std::string &name) {
    FrogRegPro *reg_profile;
    std::map<std::string, FrogRegPro*>::iterator it_frog_reg_map = frog_reg_map.begin();
    for (int i = 0; it_frog_reg_map != frog_reg_map.end (); ++it_frog_reg_map, i++){
        reg_profile = it_frog_reg_map->second;
        if (reg_profile->m_sel_id == selID && reg_profile->m_reg_id == regID) {
            name = reg_profile->m_reg_name;
            return true;
        }
    }
    return false;
}


uint32_t gcs_initialize_simulator_ex (uint32_t debug_level, GCS_CB_INFO_EX *pCbInfo,
                                 const std::string &inifilepath, GCS_HW_INFO_EX *pHwInfo)
{
//    gcsMsg << "< Info : gcs simulator is initialized by python script. >" << std::endl;
    FILE *fp;
    unsigned int debug_level_default = MSG_INF;
    g_reg_record = new CfrogRegWriteRecord();
    m_forest_util = new ForestUtil();
    m_forest_util->CreateForest();
    if (debug_level > 0) {
        debug_level_default = debug_level;
    }
    if ((fp = fopen ("cforest.log", "w")) == NULL){
        return GCSIF_ERR_ALLOCATE_MEMORY;
    }
    m_forest_util->SetFileOut (fp);
    m_forest_util->SetMessageLevel(debug_level_default);
    m_cmdopt = new ForestCmdOpt (m_forest_util);
    m_cmdopt->SetScriptName (inifilepath);
    ForestSetup* forest_setup = new ForestSetup (m_forest_util, m_cmdopt);
    PythonAPI::StartPy (m_forest_util, m_cmdopt, forest_setup, 0, NULL);
    uint32_t pe_id = m_forest_util->GetPeLastId();
    if (pHwInfo != NULL) {
        if (m_forest_util->GetCoreType(pe_id) == CORE_G4MH) {
            // Number of virtual machine
            // 1 : If G4MH2.0 has HV resource (set_has_hv("yes") in cforest config)
            // 0 : Others. If G4MH2.0 doesn't has HV resource (set_has_hv("no") in cforest config)
            uint32_t num_vm;
            if (  (m_forest_util->GetCoreVersion(pe_id) >= CORE_VER_20)
               && (m_forest_util->GetExtPin (pe_id, EXT_PIN_HAS_HV) == 1)) {
                num_vm = 1;
            } else {
                num_vm = 0;
            }
            pHwInfo->vmnum = num_vm;

            // Number of hardware thread
            // 1 : Fixed to 1 in G4MH.
            pHwInfo->threadnum = 1;

            // Number of memory protection area
            // This is the number of MPU entry (by set_mpunum (x)).
            // This is same value with MPCFG.NMPUE + 1 if PE has MPU.
            uint32_t num_mpu;
            if (m_forest_util->GetExtPin (pe_id, EXT_PIN_HAS_MPU) == 1) {
                num_mpu = m_forest_util->GetExtPin (pe_id, EXT_PIN_MPTRGN);
            } else {
                num_mpu = 0;
            }
            pHwInfo->mpnum = num_mpu;

            // Number of HW break point channel
            // The number of channel of BPCn register.
            // This will be 12 in G4MHx.
            pHwInfo->bpnum = 12;

            // Number of TLB entry
            // Becasue MMU isn't supported by G4MH ISA,
            // this is fixed to 0.
            pHwInfo->tlbnum = 0;

            // MMU Page size
            // Becasue MMU isn't supported by G4MH ISA,
            // this is fixed to 0.
            pHwInfo->smallpage = 0;
        } else {
            pHwInfo->vmnum = 0;
            pHwInfo->threadnum = 1;
            pHwInfo->mpnum = 24;
            pHwInfo->bpnum = 12;
            pHwInfo->tlbnum = 0;
            pHwInfo->smallpage = 0;
        }
    }
    if (pCbInfo != NULL) {
        CbComment       = pCbInfo->fpC;
        CbMemoryEx      = pCbInfo->fpExM;
        CbNcRegEx       = pCbInfo->fpExNR;
        CbVcRegEx       = pCbInfo->fpExVR;
        CbTcRegEx       = pCbInfo->fpExTR;
        CbLLbit         = pCbInfo->fpL;
        CbTlbEx         = pCbInfo->fpExTLB;
    }

    CoreType core_type = m_forest_util->GetCoreType(pe_id);
    CoreVersion core_version = m_forest_util->GetCoreVersion(pe_id);
    if (core_type == CORE_G4MH) {
        if (core_version >= CORE_VER_20) {
            // Initialized function for all registers in G4MH2.0, G4MH2.1.
            init_FROG_register_profile_G4MH2x();
        } else {
            // Initialized function for all registers in G4MH1.0, G4MH1.1.
            init_FROG_register_profile_G4MH1x();
        }
    }

    if (InitPreValReg(pe_id, core_type) == false ){
        return GCSIF_ERR_ALLOCATE_MEMORY;
    }

    g_last_exec_exp_code = EXP_NONE;

    return GCSIF_SUCCESS;
}


uint32_t gcs_reset_simulator(void)
{
    if (m_forest_util == NULL) {
        printf ("Error : gcs simulator is not initialized.\n");
        return GCSIF_ERR_ALLOCATE_MEMORY;
    }
    m_forest_util->HardReset();
    g_last_exec_exp_code = EXP_NONE;
    uint32_t pe_id = m_forest_util->GetPeLastId();
    if (InitPreValReg(pe_id, m_forest_util->GetCoreType(pe_id)) == false ){
        return GCSIF_ERR_ALLOCATE_MEMORY;
    }
    return GCSIF_SUCCESS;
}


/**
 * This is the refactory function for access DW when values store to memory need to access QW.
 * This is also internal function.
 *
 * @param[in] address access address
 * @param[in] cur_val current memory value
*/
uint32_t dw_access (uint32_t address, uint64_t cur_val)
{
    uint64_t pre_mem = 0;
    std::vector<std::pair <PhysAddr, uint64_t> >::iterator it_pre_mem = m_pre_mem_vector.begin ();
    for (;it_pre_mem != m_pre_mem_vector.end (); ++it_pre_mem){
        if (it_pre_mem->first == address) {
            pre_mem = it_pre_mem->second;
        break;
        }
    }
    if (it_pre_mem != m_pre_mem_vector.end ()) {
        it_pre_mem->second = cur_val;
    }
    CbMemoryEx(address, 8, cur_val, pre_mem, FROG_ACCESS_TYPE_WRITE, FROG_ACCESS_BUS_RW, FROG_LOAD_ACTION_LOAD);
    m_pre_mem_vector.push_back(std::pair <PhysAddr, uint64_t> (address, cur_val));
    return true;
}


uint32_t gcs_step_simulator(uint32_t ptid)
{
    // Convert tcid from FROG to CForest
    int tcid_cforest = ptid;
    uint32_t context = 0;
    // Simulator is not initialized.
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_NOT_INITIALIZED;
    }
    if ((context == 1) && (ptid == 0)) {
        tcid_cforest = NC;
    }
    uint32_t pe_id = m_forest_util->GetPeLastId();
    if ((!m_forest_util->IsValidPeId(pe_id)) || (tcid_cforest != NC)) {
        return GCSIF_ERR_INVALID_TCID;
    }

    NotifySimState state;
    if (m_forest_util->GetStatus (pe_id, &state) == CF_API_OK) {
        if (state == SIMULATE_HALT) {
            return GCSIF_ERR_THREAD_IS_HALT;
        }
    }

    uint32_t scid = m_forest_util->GetScId (pe_id);
    m_forest_util->StepExecute<STEP_MODE> (scid, 1);

    TraceCtrl* trace;
    trace = const_cast<TraceCtrl*> (m_forest_util->GetTraceCtrl (scid));

    // Record existence of exception/interruption in step execute
    g_last_exec_exp_code = trace->GetExpCode ();

    // Execute callback function when instructions fetch from ROM
    if (CbMemoryEx) {
        CbMemoryEx(trace->GetInstAddr(), trace->GetInstSize(), trace->GetInstCode(),
                   0x0, FROG_ACCESS_TYPE_READ, FROG_ACCESS_BUS_FETCH, FROG_LOAD_ACTION_FETCH);
    }
    // Execute callback function when values load from memory
    uint32_t rmem_count = trace->GetRmemCount();
    for (uint32_t i = 0; i < rmem_count; ++i) {
        uint32_t access_bus;
        if (m_forest_util->IsRomArea(trace->GetRmemAddr(i))) {
            access_bus = FROG_ACCESS_BUS_FETCH;
        } else {
            access_bus = FROG_ACCESS_BUS_RW;
        }

        if (trace->GetRmemSize(i) != 16) {
            CbMemoryEx(trace->GetRmemAddr(i), trace->GetRmemSize(i), trace->GetRmemValueL64(i),
                       0ULL, FROG_ACCESS_TYPE_READ, access_bus, FROG_LOAD_ACTION_LOAD);
        } else {
            // QW access
            CbMemoryEx(trace->GetRmemAddr(i), 8, trace->GetRmemValueL64(i),
                       0ULL, FROG_ACCESS_TYPE_READ, access_bus, FROG_LOAD_ACTION_LOAD);
            CbMemoryEx(trace->GetRmemAddr(i) + 8, 8, trace->GetRmemValueU64(i),
                       0ULL, FROG_ACCESS_TYPE_READ, access_bus, FROG_LOAD_ACTION_LOAD);
        }
    }
    // Execute callback function when values store to memory
    uint32_t wmem_count = trace->GetWmemCount();
    for (uint32_t i = 0; i < wmem_count; ++i) {
        if (m_forest_util->IsRomArea(trace->GetWmemAddr(i))) {
            if (trace->GetWmemSize(i) != 16) {
                CbMemoryEx(trace->GetWmemAddr(i), trace->GetWmemSize(i), trace->GetWmemValueL64(i),
                           0ULL, FROG_ACCESS_TYPE_WRITE, FROG_ACCESS_BUS_FETCH, FROG_LOAD_ACTION_LOAD);
            } else {
                // QW access
                CbMemoryEx(trace->GetWmemAddr(i), 8, trace->GetWmemValueL64(i),
                           0ULL, FROG_ACCESS_TYPE_WRITE, FROG_ACCESS_BUS_FETCH, FROG_LOAD_ACTION_LOAD);
                CbMemoryEx(trace->GetWmemAddr(i) + 8, 8, trace->GetWmemValueU64(i),
                           0ULL, FROG_ACCESS_TYPE_WRITE, FROG_ACCESS_BUS_FETCH, FROG_LOAD_ACTION_LOAD);
            }
        } else {
            if (trace->GetWmemSize(i) != 16) {
                uint64_t pre_mem = 0;
                std::vector<std::pair <PhysAddr, uint64_t> >::iterator it_pre_mem = m_pre_mem_vector.begin ();
                for (;it_pre_mem != m_pre_mem_vector.end (); ++it_pre_mem){
                    if (it_pre_mem->first == trace->GetWmemAddr(i)) {
                        pre_mem = it_pre_mem->second;
                        break;
                    }
                }
                if (it_pre_mem != m_pre_mem_vector.end ()) {
                    it_pre_mem->second = trace->GetWmemValueL64(i);
                }
                CbMemoryEx(trace->GetWmemAddr(i), trace->GetWmemSize(i), trace->GetWmemValueL64(i),
                           pre_mem, FROG_ACCESS_TYPE_WRITE, FROG_ACCESS_BUS_RW, FROG_LOAD_ACTION_LOAD);
                m_pre_mem_vector.push_back(std::pair <PhysAddr, uint64_t> (trace->GetWmemAddr(i), trace->GetWmemValueL64(i)));
            } else {
                // QW access
                // 1st DW
                dw_access(trace->GetWmemAddr(i), trace->GetWmemValueL64(i));
                // 2nd DW
                dw_access(trace->GetWmemAddr(i)+8, trace->GetWmemValueU64(i));
            }
        }
    }

    // Return callback function for register updating
    uint32_t wreg_count = trace->GetWregCount ();
    uint32_t PSW_update = 0;
    uint32_t tcid_callback = 0, vcid_callback = 0;

    for (uint32_t i = 0; i < wreg_count; ++i) {
        CReg regno = trace->GetWriteReg (i);
        // Convert name input to upper character.
        std::string name_copy; //(boost::to_upper_copy<std::string>(gcs_internal_get_frog_name(regno)));
        ToUpperCopy (gcs_internal_get_frog_name(regno), &name_copy);
        std::map <std::string, std::vector <FrogRegData > >::iterator it_p_prev_reg_frog = p_prev_reg_frog.find(name_copy);
        std::map<std::string, FrogRegPro*>::iterator it_frog_reg_map = frog_reg_map.find(name_copy);
//        FrogRegData cforest_written_value = trace->GetWregValue(i);

        if ((regno & 0x000FF000) != 0x00000000) {
            if ((regno & 0x000F0000) != 0x00000000) {
                tcid_callback = ( regno >> 16 ) & 0x0F;
            } else if ((regno & 0x0000F000) != 0x00000000) {
                vcid_callback = ( regno >> 12 ) & 0x0F;
            }
        }

        if (regno >= HMPSW_Z && regno <= GMPSW_VM) {
            PSW_update = 1;
        } else {
            if ((it_p_prev_reg_frog == p_prev_reg_frog.end()) || (it_frog_reg_map == frog_reg_map.end())) {
                return GCSIF_ERR_INVALID_REGISTER_NAME;
            } else {
                FrogRegData cforest_written_value = 0;
                uint64_t temp_cforest = 0;
                uint64_t temp_cforest_u64 = 0;
                //uint32_t temp_cforest_vc = 0;
                bool sreg_ret = false;

                if (tcid_callback == 1 || vcid_callback == 1) {
                    sreg_ret = m_forest_util->RegReadDebug (pe_id, regno & 0x00000FFF, (&temp_cforest), &temp_cforest_u64);
                    if (sreg_ret == true) {
                        cforest_written_value = (static_cast<FrogRegData>(temp_cforest_u64) << 64)
                                              | static_cast<FrogRegData>(temp_cforest);
                    } else {
                        printf ("Reg %s return error\n", name_copy.c_str());
                    }
                } else if (tcid_callback > 1) {
                    sreg_ret = m_forest_util->RegReadDebug (pe_id, regno & 0x00000FFF, (&temp_cforest), &temp_cforest_u64);
                    if (sreg_ret == true) {
                        cforest_written_value = (static_cast<FrogRegData>(temp_cforest_u64) << 64)
                                              | static_cast<FrogRegData>(temp_cforest);
                    } else {
                        printf ("Reg %s return error\n", name_copy.c_str());
                    }
                } else if (vcid_callback > 1) {
                    sreg_ret = m_forest_util->RegReadDebug (pe_id, regno & 0x00000FFF, (&temp_cforest));
                    if (sreg_ret == true) {
                        cforest_written_value = temp_cforest;
                    } else {
                        printf ("Reg %s return error\n", name_copy.c_str());
                    }
                } else if (tcid_cforest == NC) {
                    sreg_ret = m_forest_util->RegReadDebug (pe_id, regno & 0x00000FFF , (&temp_cforest), &temp_cforest_u64);
                    if (sreg_ret == true) {
                        cforest_written_value = (static_cast<FrogRegData>(temp_cforest_u64) << 64)
                                              | static_cast<FrogRegData>(temp_cforest);
                    } else {
                        printf ("Reg %s return error\n", name_copy.c_str());
                    }
                }
#if 0
                else if (tcid_cforest == TC0) {
                    FrogRegPro * reg_profile = it_frog_reg_map->second;
                    if ((reg_profile->m_context.compare("T") == 0) || (reg_profile->m_context.compare("NT") == 0)) {
                        sreg_ret = m_forest_util->RegReadDebug (pe_id, regno & 0x00000FFF, (&temp_cforest), &temp_cforest_u64);
                        if (sreg_ret == true) {
                            cforest_written_value = (static_cast<FrogRegData>(temp_cforest_u64) << 64)
                                                  | static_cast<FrogRegData>(temp_cforest);
                        } else {
                            printf ("Reg %s return error\n", name_copy.c_str());
                        }
                    } else if ((reg_profile->m_context.compare("V") == 0) || (reg_profile->m_context.compare("NV") == 0)
                            || (reg_profile->m_context.compare("VS") == 0)) {
                        sreg_ret = m_forest_util->VcRegReadDebug (pe_id, VC0, regno & 0x00000FFF, (&temp_cforest_vc));
                        if (sreg_ret == true) {
                            cforest_written_value = temp_cforest_vc;
                        } else {
                            printf ("Reg %s return error\n", name_copy.c_str());
                        }
                    }
                }
#endif // #if 0
                std::vector <FrogRegData > v_prev_reg = it_p_prev_reg_frog->second;
                FrogRegPro * reg_profile = it_frog_reg_map->second;
                if (vcid_callback > 0) {
                    if (vcid_callback == 1) {
                        FrogRegData frog_pre_val = v_prev_reg.at(NC_pos);
                        CbNcRegEx(name_copy, cforest_written_value, frog_pre_val);
                        v_prev_reg.erase(v_prev_reg.begin() + NC_pos);
                        v_prev_reg.insert (v_prev_reg.begin() + NC_pos, cforest_written_value);
                        // This register is updated by ldvc insn.
                        if (((reg_profile->m_context.compare("VS")) == 0) && v_prev_reg.size() >= 2) {
                            v_prev_reg.erase(v_prev_reg.begin() + VT0_pos);
                            v_prev_reg.insert (v_prev_reg.begin() + VT0_pos, cforest_written_value);
                        }
                        p_prev_reg_frog.erase (it_p_prev_reg_frog);
                        p_prev_reg_frog.insert (std::pair<std::string, std::vector <FrogRegData> > (reg_profile->m_reg_name, v_prev_reg));
                    } else if (vcid_callback == 2) {
                        FrogRegData frog_pre_val = v_prev_reg.at(VT0_pos);
                        CbVcRegEx(name_copy, cforest_written_value, frog_pre_val, (vcid_callback - 2));
                        v_prev_reg.erase (v_prev_reg.begin() + VT0_pos);
                        v_prev_reg.insert (v_prev_reg.begin() + VT0_pos, cforest_written_value);
                        if ((reg_profile->m_context.compare("VS")) == 0) {
                            v_prev_reg.erase (v_prev_reg.begin() + NC_pos);
                            v_prev_reg.insert (v_prev_reg.begin() + NC_pos, cforest_written_value);
                            for (uint32_t prev_pos = 2; prev_pos < v_prev_reg.size(); prev_pos++) {
                                v_prev_reg.erase(v_prev_reg.begin() + prev_pos);
                                v_prev_reg.insert (v_prev_reg.begin() + prev_pos, cforest_written_value);
                            }
                        }
                        p_prev_reg_frog.erase (it_p_prev_reg_frog);
                        p_prev_reg_frog.insert (std::pair<std::string, std::vector <FrogRegData> > (reg_profile->m_reg_name, v_prev_reg));
                    } else {
                        printf ("# Error: VCID %ud is not supported", vcid_callback);
                    }
                } else if (tcid_callback > 0){
                    if (tcid_callback == 1) {
                        FrogRegData frog_pre_val = v_prev_reg.at(NC_pos);
                        CbNcRegEx(name_copy, cforest_written_value, frog_pre_val);
                        v_prev_reg.erase(v_prev_reg.begin() + NC_pos);
                        v_prev_reg.insert (v_prev_reg.begin() + NC_pos, cforest_written_value);
                        // This register is updated by ldvc insn.
                        if (((reg_profile->m_context.compare("T")) == 0) && v_prev_reg.size() >= 2) {
                            v_prev_reg.erase(v_prev_reg.begin() + VT0_pos);
                            v_prev_reg.insert (v_prev_reg.begin() + VT0_pos, cforest_written_value);
                        }
                        p_prev_reg_frog.erase (it_p_prev_reg_frog);
                        p_prev_reg_frog.insert (std::pair<std::string, std::vector <FrogRegData> > (reg_profile->m_reg_name, v_prev_reg));

                        gcs_internal_record_reg_write (pe_id, NC, name_copy);
                    } else if (tcid_callback == 2) {
                        FrogRegData frog_pre_val = v_prev_reg.at(VT0_pos);
                        CbTcRegEx(name_copy, cforest_written_value, frog_pre_val, (tcid_callback - 2));
                        v_prev_reg.erase (v_prev_reg.begin() + VT0_pos);
                        v_prev_reg.insert (v_prev_reg.begin() + VT0_pos, cforest_written_value);
                        if ((reg_profile->m_context.compare("T")) == 0) {
                            v_prev_reg.erase (v_prev_reg.begin() + NC_pos);
                            v_prev_reg.insert (v_prev_reg.begin() + NC_pos, cforest_written_value);
                        }
                        p_prev_reg_frog.erase (it_p_prev_reg_frog);
                        p_prev_reg_frog.insert (std::pair<std::string, std::vector <FrogRegData> > (reg_profile->m_reg_name, v_prev_reg));

                        gcs_internal_record_reg_write (pe_id, (tcid_callback - 2), name_copy);
                    } else {
                        printf ("# Error: TCID %ud is not supported", tcid_callback);
                    }
                } else if (tcid_cforest == NC) {
                    FrogRegData frog_pre_val = v_prev_reg.at(NC_pos);
                    CbNcRegEx(name_copy, cforest_written_value, frog_pre_val);
                    v_prev_reg.erase(v_prev_reg.begin() + NC_pos);
                    v_prev_reg.insert (v_prev_reg.begin() + NC_pos, cforest_written_value);
                    if (((reg_profile->m_context.compare("T") == 0) || (reg_profile->m_context.compare("VS")) == 0) && v_prev_reg.size() >= 2) {
                        v_prev_reg.erase(v_prev_reg.begin() + VT0_pos);
                        v_prev_reg.insert (v_prev_reg.begin() + VT0_pos, cforest_written_value);
                    }
                    p_prev_reg_frog.erase (it_p_prev_reg_frog);
                    p_prev_reg_frog.insert (std::pair<std::string, std::vector <FrogRegData> > (reg_profile->m_reg_name, v_prev_reg));

                    gcs_internal_record_reg_write (pe_id, NC, name_copy);
                } else if (tcid_cforest == TC0) {
                    FrogRegData frog_pre_val = v_prev_reg.at(VT0_pos);
                    if ((reg_profile->m_context.compare("T") == 0)|| (reg_profile->m_context.compare("NT") == 0)) {
                        CbTcRegEx(name_copy, cforest_written_value, frog_pre_val, tcid_cforest);
                        v_prev_reg.erase(v_prev_reg.begin() + VT0_pos);
                        v_prev_reg.insert (v_prev_reg.begin() + VT0_pos, cforest_written_value);
                        if ((reg_profile->m_context.compare("T") == 0) && v_prev_reg.size() >= 2) {
                            v_prev_reg.erase(v_prev_reg.begin() + NC_pos);
                            v_prev_reg.insert (v_prev_reg.begin() + NC_pos, cforest_written_value);
                        }
                        p_prev_reg_frog.erase (it_p_prev_reg_frog);
                        p_prev_reg_frog.insert (std::pair<std::string, std::vector <FrogRegData> > (reg_profile->m_reg_name, v_prev_reg));
                    } else if ((reg_profile->m_context.compare("NV") == 0)|| (reg_profile->m_context.compare("VS") == 0)) {
                        CbVcRegEx(name_copy, cforest_written_value, frog_pre_val, VC0);
                        v_prev_reg.erase(v_prev_reg.begin() + VT0_pos);
                        v_prev_reg.insert (v_prev_reg.begin() + VT0_pos, cforest_written_value);
                        if ((reg_profile->m_context.compare("VS") == 0) && v_prev_reg.size() >= 2) {
                            v_prev_reg.erase(v_prev_reg.begin() + NC_pos);
                            v_prev_reg.insert (v_prev_reg.begin() + NC_pos, cforest_written_value);
                        }
                        p_prev_reg_frog.erase (it_p_prev_reg_frog);
                        p_prev_reg_frog.insert (std::pair<std::string, std::vector <FrogRegData> > (reg_profile->m_reg_name, v_prev_reg));
                    }
                    gcs_internal_record_reg_write (pe_id, TC0, name_copy);
                }
                if ((name_copy.compare("PSW") == 0) && (v_prev_reg.size() >= 2)) {
                    // not use (v_prev_reg.size() >= 2) for G4MH. Don't need to update for GMPSW/PSW here.
                    uint64_t value_NC, value_TC0;

                    sreg_ret = m_forest_util->RegReadDebug (pe_id, SR_PSW, (&value_NC));
                    if (sreg_ret == true) {
                        sreg_ret = m_forest_util->RegReadDebug (pe_id, SR_PSW, (&value_TC0));
                    } else {
                        printf ("Reg %s return error\n", name_copy.c_str());
                    }

                    v_prev_reg.erase(v_prev_reg.begin() + NC_pos);
                    v_prev_reg.insert (v_prev_reg.begin() + NC_pos, (FrogRegData) value_NC);
                    v_prev_reg.erase(v_prev_reg.begin() + VT0_pos);
                    v_prev_reg.insert (v_prev_reg.begin() + VT0_pos, (FrogRegData) value_TC0);
                    p_prev_reg_frog.erase (it_p_prev_reg_frog);
                    p_prev_reg_frog.insert (std::pair<std::string, std::vector <FrogRegData> > (reg_profile->m_reg_name, v_prev_reg));
                }
//                FrogRegData trace_written_value = trace->GetWregValue(i);
//                if ((cforest_written_value != trace_written_value) && !(regno >= PSW_Z && regno <= PSW_VM) && !(regno >= SESR_OV && regno <= SESR_SOV)) {
//                    printf ("-------------     * Warning %s *      -------------\n", name_copy.c_str());
//                    printf ("              CForest value | Trace info value\n");
//                    uint32_t temp_cforest = 0, temp_trace = 0;
//                    for (int i = 3; i >= 0; i--) {
//                        temp_cforest = (cforest_written_value >> 32 * i) & 0x00000000FFFFFFFF ;
//                        temp_trace = (trace_written_value >> 32 *i) & 0x00000000FFFFFFFF;
//                        if (temp_cforest != temp_trace) {
//                            printf ("Byte %02d - %02d:    %08x    |    %08x  <---\n", (i * 4), (i * 4 + 3), temp_cforest, temp_trace);
//                        } else {
//                            printf ("Byte %02d - %02d:    %08x    |    %08x \n", (i * 4), (i * 4 + 3), temp_cforest, temp_trace);
//                        }
//                    }
//                    printf ("-------------------------------------------------------\n");
//                }
            }
        }
    }
    if (PSW_update) {
        bool use_gmpsw = false;
        uint32_t err = gcs_internal_use_gmpsw (&use_gmpsw);
        if (err != GCSIF_SUCCESS) {
            return err;
        }
        std::string psw_name;
        uint32_t sr_psw_id;
        if (use_gmpsw) {
            psw_name = "GMPSW";
            sr_psw_id = SR_GMPSW;
        } else {
            psw_name = "PSW";
            sr_psw_id = SR_PSW;
        }

        std::map <std::string, std::vector <FrogRegData > >::iterator it_p_prev_reg_frog = p_prev_reg_frog.find(psw_name);
        std::vector <FrogRegData > v_prev_reg = it_p_prev_reg_frog->second;
        FrogRegData cforest_written_value = 0, frog_pre_val = 0;
        uint64_t temp_cforest = 0;

        uint32_t local_data = 0;
        err = gcs_internal_get_psw_on_current_mode (&local_data);
        if (err != GCSIF_SUCCESS) {
            return GCSIF_ERR_INTERNAL;
        }
        temp_cforest = local_data;
        cforest_written_value = temp_cforest;

        if (tcid_cforest == NC) {
            frog_pre_val = v_prev_reg.at(NC_pos);
            CbNcRegEx(gcs_internal_get_frog_name(sr_psw_id), cforest_written_value, frog_pre_val);
            v_prev_reg.erase(v_prev_reg.begin() + NC_pos);
            v_prev_reg.insert (v_prev_reg.begin() + NC_pos, cforest_written_value);
        } else if (tcid_cforest == TC0) {
            frog_pre_val = v_prev_reg.at(VT0_pos);
            CbTcRegEx(gcs_internal_get_frog_name(sr_psw_id), cforest_written_value, frog_pre_val, tcid_cforest);
            v_prev_reg.erase(v_prev_reg.begin() + VT0_pos);
            v_prev_reg.insert (v_prev_reg.begin() + VT0_pos, cforest_written_value);
        }
        p_prev_reg_frog.erase (it_p_prev_reg_frog);
        p_prev_reg_frog.insert (std::pair<std::string, std::vector <FrogRegData> > (psw_name, v_prev_reg));
    }

    // Update internal value record for reference registers
    while (gcs_internal_IsEmpty_reg_write () == false) {
        RegTupleType reg_rec = gcs_internal_get_reg_write ();
        gcs_internal_update_referenced_register (std::get<0>(reg_rec), std::get<1>(reg_rec), std::get<2>(reg_rec));
    }

    if (trace->GetLLBitAdd().first) {
        CbLLbit (pe_id, trace->GetLLBitAdd().first, trace->GetLLBitAdd().second, context, ptid, FROG_LLBIT_CREATE);
    }
    int32_t llbit_del = trace->GetDeletedLLBitCount();
    std::pair <int32_t, PhysAddr> llbit_del_val;
    for (int32_t i = 0; i < llbit_del; ++i) {
        llbit_del_val = trace->GetDeletedLLBit(i);
        pe_id = llbit_del_val.first;
        CbLLbit (pe_id, llbit_del_val.second, LLBIT_DEL_SIZE, context, ptid, FROG_LLBIT_DELETE);
    }
    std::string get_comment = gcs_internal_build_comment(ptid, context, trace);
    CbComment (get_comment);

#ifdef FROG_API_G4MH_UT
    g_get_opcode = get_comment.c_str();
#endif

    return GCSIF_SUCCESS;
}


bool gcs_get_exception (void)
{
    return (g_last_exec_exp_code != EXP_NONE);
}


/**
 * This is refactory function for display message when there are miss match value between frog & cforest
 * This is also internal function.
 *
 * @param[in] cforest_val access address
 * @param[in] frog_val    current memory value
 * @param[in] reg_name    register name
 * @param[in] func_name   function name
 */
uint32_t miss_match_message (FrogRegData cforest_val, FrogRegData frog_val, std::string reg_name, std::string func_name) {
    printf ("-------------     * Internal Error at %s in %s *      -------------\n", reg_name.c_str(), func_name.c_str());
    printf ("              CForest value | Frog I/F value\n");

    for (int i = 3; i >= 0; i--) {
        uint32_t temp_cforest = 0, temp_frog = 0;
        temp_cforest = (cforest_val >> 32 * i) & 0x00000000FFFFFFFF ;
        temp_frog = (frog_val >> 32 *i) & 0x00000000FFFFFFFF;
        if (temp_cforest != temp_frog) {
            printf ("Byte %02d - %02d:    %08x    |    %08x  <---\n", (i * 4), (i * 4 + 3), temp_cforest, temp_frog);
            } else {
                printf ("Byte %02d - %02d:    %08x    |    %08x \n", (i * 4), (i * 4 + 3), temp_cforest, temp_frog);
            }
    }
    printf ("-------------------------------------------------------\n");
    return true;
}


/**
 * \fn  uint32_t update_fpu_fxu (uint32_t pe_id, std::string reg_name)
 * \brief This is refactory function for update FXU, FPU register.
 *
 * This is refactory function for update FXU, FPU register
 * This is also internal function.
 *
 * @param[in] pe_id    PE ID
 * @param[in] reg_name Register name
 * @return fpu_fxu was updated successfully.
 */
uint32_t update_fpu_fxu (uint32_t pe_id, std::string reg_name)
{
    bool sreg_return;
    uint64_t cforest_written_value;
    std::map<std::string, FrogRegPro*>::iterator it_frog_reg_map = frog_reg_map.find(reg_name);
    FrogRegPro* reg_profile = it_frog_reg_map->second;
    sreg_return = m_forest_util->RegReadDebug (pe_id, reg_profile->m_creg, &cforest_written_value);
    std::map <std::string, std::vector <FrogRegData > >::iterator it_p_prev_reg_frog = p_prev_reg_frog.find(reg_name);
    std::vector <FrogRegData > v_prev_reg = it_p_prev_reg_frog->second;
    v_prev_reg.erase(v_prev_reg.begin() + NC_pos);
    v_prev_reg.insert (v_prev_reg.begin() + NC_pos, cforest_written_value);
    if (v_prev_reg.size() >= 2) {
        v_prev_reg.erase(v_prev_reg.begin() + VT0_pos);
        v_prev_reg.insert (v_prev_reg.begin() + VT0_pos, cforest_written_value);
    }
    p_prev_reg_frog.erase (it_p_prev_reg_frog);
    p_prev_reg_frog.insert (std::pair<std::string, std::vector <FrogRegData> > (reg_profile->m_reg_name, v_prev_reg));
    return sreg_return;
}


uint32_t gcs_set_nc_register(const std::string &name, FrogRegData value) {
    uint32_t context = 1;

    FrogRegData cforest_pre_val = 0, cforest_written_value = 0;
    uint64_t temp_pre_cforest = 0, temp_cur_cforest = 0;
    uint64_t temp_pre_u64_cforest = 0, temp_cur_u64_cforest = 0;
    // Simulator is not initialized.
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_NOT_INITIALIZED;
    }
    uint32_t pe_id = m_forest_util->GetPeLastId();

    // Convert name input to upper character.
    std::string name_copy; //(boost::to_upper_copy<std::string>(name));
    ToUpperCopy (name, &name_copy);
    std::map <std::string, std::vector <FrogRegData > >::iterator it_p_prev_reg_frog = p_prev_reg_frog.find(name_copy);
    std::map<std::string, FrogRegPro*>::iterator it_frog_reg_map = frog_reg_map.find(name_copy);

    if ((it_p_prev_reg_frog == p_prev_reg_frog.end()) || (it_frog_reg_map == frog_reg_map.end())) {
        return GCSIF_ERR_INVALID_REGISTER_NAME;
    } else {
        std::vector <FrogRegData > v_prev_reg = it_p_prev_reg_frog->second;
        FrogRegData frog_pre_val = v_prev_reg.at(NC_pos);
        FrogRegPro * reg_profile = it_frog_reg_map->second;

        if ((v_prev_reg.size() >= 1) && (reg_profile != NULL)) {
            bool sreg_return = m_forest_util->RegReadDebug (pe_id, reg_profile->m_creg, &temp_pre_cforest, &temp_pre_u64_cforest);
            cforest_pre_val = (static_cast<FrogRegData>(temp_pre_u64_cforest) << 64)
                            | static_cast<FrogRegData>(temp_pre_cforest);

            if ((sreg_return == false) || (reg_profile->m_context.compare("V") == 0)) {
                return GCSIF_ERR_INVALID_CONTEXT;
            } else {
                if ((cforest_pre_val != frog_pre_val) && (reg_profile->m_creg != PC)) {
                    miss_match_message (cforest_pre_val, frog_pre_val, name_copy, "gcs_set_nc_register");
                }

                // Write input value to NC register
                uint64_t data_l64 = static_cast <uint64_t> (value & 0xFFFFFFFFFFFFFFFFULL);
                uint64_t data_u64 = static_cast <uint64_t> ((value >> 64) & 0xFFFFFFFFFFFFFFFFULL);
                sreg_return = m_forest_util->RegWriteDebug (pe_id, reg_profile->m_creg, data_l64, data_u64);
                if (sreg_return == false) {
                    printf ("Reg %s return error\n", name_copy.c_str());
                }

                // Read writable value from NC register
                sreg_return = m_forest_util->RegReadDebug (pe_id, reg_profile->m_creg, &temp_cur_cforest, &temp_cur_u64_cforest);
                if (sreg_return == false) {
                    printf ("Reg %s return error\n", name_copy.c_str());
                }
                cforest_written_value = (static_cast<FrogRegData>(temp_cur_u64_cforest) << 64)
                                        | static_cast<FrogRegData>(temp_cur_cforest);

                CbNcRegEx(name_copy, cforest_written_value, cforest_pre_val);
                v_prev_reg.erase(v_prev_reg.begin() + NC_pos);
                v_prev_reg.insert (v_prev_reg.begin() + NC_pos, cforest_written_value);
                if (((reg_profile->m_context.compare("T") == 0) || (reg_profile->m_context.compare("VS")) == 0) && v_prev_reg.size() >= 2) {
                    v_prev_reg.erase(v_prev_reg.begin() + VT0_pos);
                    v_prev_reg.insert (v_prev_reg.begin() + VT0_pos, cforest_written_value);
                }
                p_prev_reg_frog.erase (it_p_prev_reg_frog);
                p_prev_reg_frog.insert (std::pair<std::string, std::vector <FrogRegData> > (reg_profile->m_reg_name, v_prev_reg));

                if (name_copy.compare("FPSR") == 0) {
                    // Update FPCFG depends on FPSR;
                    update_fpu_fxu (pe_id, "FPCFG");

                    // Update FPCC depends on FPSR;
                    update_fpu_fxu (pe_id, "FPCC");

                    // Update FPST depends on FPSR;
                    update_fpu_fxu (pe_id, "FPST");

                } else if (name_copy.compare("FXSR") == 0) {
                    // Update FXCFG depends on FXSR;
                    update_fpu_fxu (pe_id, "FXCFG");

                    // Update FXST depends on FXSR;
                    update_fpu_fxu (pe_id, "FXST");

                } else if (name_copy.compare("FXCFG") == 0) {
                    // Update FXSR depends on FXCFG;
                    update_fpu_fxu (pe_id, "FXSR");

                } else if (name_copy.compare("FXST") == 0) {
                    // Update FXSR depends on FXST;
                    update_fpu_fxu (pe_id, "FXSR");

                } else if ((name_copy.compare("PSW") == 0) && (v_prev_reg.size() >= 2)) {
                    // not use (v_prev_reg.size() >= 2) for G4MH. Don't need to update for GMPSW/PSW here.
                    uint64_t value_NC, value_TC0;
                    sreg_return = m_forest_util->RegReadDebug (pe_id, SR_PSW, (&value_NC));
                    if (sreg_return == true) {
                        sreg_return = m_forest_util->RegReadDebug (pe_id, SR_PSW, (&value_TC0));
                        if (sreg_return == false) {
                            printf ("Reg %s return error\n", name_copy.c_str());
                        }
                    } else {
                        printf ("Reg %s return error\n", name_copy.c_str());
                    }
                    v_prev_reg.erase(v_prev_reg.begin() + NC_pos);
                    v_prev_reg.insert (v_prev_reg.begin() + NC_pos, (FrogRegData) value_NC);
                    v_prev_reg.erase(v_prev_reg.begin() + VT0_pos);
                    v_prev_reg.insert (v_prev_reg.begin() + VT0_pos, (FrogRegData) value_TC0);
                    p_prev_reg_frog.erase (it_p_prev_reg_frog);
                    p_prev_reg_frog.insert (std::pair<std::string, std::vector <FrogRegData> > (reg_profile->m_reg_name, v_prev_reg));
                }
            }
        } else {
            return GCSIF_ERR_INTERNAL;
        }
    }
    gcs_internal_record_reg_write (pe_id, NC, name_copy);
    // Update internal value record for reference registers
    while (gcs_internal_IsEmpty_reg_write () == false) {
        RegTupleType reg_rec = gcs_internal_get_reg_write ();
        gcs_internal_update_referenced_register (std::get<0>(reg_rec), std::get<1>(reg_rec), std::get<2>(reg_rec));
    }

    return GCSIF_SUCCESS;
}


uint32_t gcs_get_nc_register(const std::string &name, FrogRegData* value)
{
    uint32_t context = 1;
    FrogRegData cforest_pre_val = 0;
    uint64_t temp_pre_cforest = 0;
    uint64_t temp_pre_u64_cforest = 0;
    // Simulator is not initialized.
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_NOT_INITIALIZED;
    }
    uint32_t pe_id = m_forest_util->GetPeLastId();

    std::string name_copy; //(boost::to_upper_copy<std::string>(name));
    ToUpperCopy (name, &name_copy);
    std::map <std::string, std::vector <FrogRegData > >::iterator it_p_prev_reg_frog = p_prev_reg_frog.find(name_copy);
    std::map<std::string, FrogRegPro*>::iterator it_frog_reg_map = frog_reg_map.find(name_copy);

    if ((it_p_prev_reg_frog == p_prev_reg_frog.end()) || (it_frog_reg_map == frog_reg_map.end())) {
        return GCSIF_ERR_INVALID_REGISTER_NAME;
    } else {
        std::vector <FrogRegData > v_prev_reg = it_p_prev_reg_frog->second;
        FrogRegData frog_pre_val = v_prev_reg.at(NC_pos);
        FrogRegPro * reg_profile = it_frog_reg_map->second;

        if ((v_prev_reg.size() >= 1) && (reg_profile != NULL)) {
            bool sreg_return = m_forest_util->RegReadDebug (pe_id, reg_profile->m_creg, &temp_pre_cforest, &temp_pre_u64_cforest);
            cforest_pre_val = (static_cast<FrogRegData>(temp_pre_u64_cforest) << 64)
                            | static_cast<FrogRegData>(temp_pre_cforest);
            if ((sreg_return == false) || (reg_profile->m_context.compare("V") == 0)) {
                return GCSIF_ERR_INVALID_CONTEXT;
            } else {
                if ((cforest_pre_val != frog_pre_val) && (reg_profile->m_creg != PC)) {
                    miss_match_message (cforest_pre_val, frog_pre_val, name_copy, "gcs_get_nc_register");
                }
                *value = cforest_pre_val;
            }
        } else {
            return GCSIF_ERR_INTERNAL;
        }
    }
    return GCSIF_SUCCESS;
}


uint32_t gcs_set_vc_register(const std::string &name, FrogRegData value, uint32_t vcid) {
    uint32_t context = 1;
    uint32_t pe_id = m_forest_util->GetPeLastId();
    FrogRegData cforest_pre_val = 0, cforest_written_value = 0;
    uint64_t temp_pre_cforest = 0, temp_cur_cforest = 0;
    // Simulator is not initialized.
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_NOT_INITIALIZED;
    }
    if ((!m_forest_util->IsValidPeId(pe_id)) || ((int32_t)vcid != NC)){
        return GCSIF_ERR_INVALID_VCID;
    }
    std::string name_copy; //(boost::to_upper_copy<std::string>(name));
    ToUpperCopy (name, &name_copy);
    std::map <std::string, std::vector <FrogRegData > >::iterator it_p_prev_reg_frog = p_prev_reg_frog.find(name_copy);
    std::map<std::string, FrogRegPro*>::iterator it_frog_reg_map = frog_reg_map.find(name_copy);

    if ((it_p_prev_reg_frog == p_prev_reg_frog.end()) || (it_frog_reg_map == frog_reg_map.end())) {
        return GCSIF_ERR_INVALID_REGISTER_NAME;
    } else {
        std::vector <FrogRegData > v_prev_reg = it_p_prev_reg_frog->second;
        FrogRegData frog_pre_val = v_prev_reg.at(VT0_pos);
        FrogRegPro * reg_profile = it_frog_reg_map->second;

        if ((v_prev_reg.size() >= 2) && (reg_profile != NULL)) {
            bool sreg_return = m_forest_util->RegReadDebug (pe_id, reg_profile->m_creg, &temp_pre_cforest);
            cforest_pre_val = temp_pre_cforest & 0xFFFFFFFFU;
            if ((sreg_return == false) || (reg_profile->m_context.compare("N") == 0)
                                       || (reg_profile->m_context.compare("T") == 0)|| (reg_profile->m_context.compare("NT") == 0)) {
                return GCSIF_ERR_INVALID_CONTEXT;
            } else {
                if (cforest_pre_val != frog_pre_val) {
                    miss_match_message (cforest_pre_val, frog_pre_val, name_copy, "gcs_set_vc_register");
                }
                // Write input value to NC register
                sreg_return = m_forest_util->RegWriteDebug (pe_id, reg_profile->m_creg, (uint64_t) value);
                if (sreg_return == true) {
                    // Read writable value from NC register
                    sreg_return = m_forest_util->RegReadDebug (pe_id, reg_profile->m_creg, &temp_cur_cforest);
                    if (sreg_return == false) {
                        printf ("Reg %s return error\n", name_copy.c_str());
                    }
                } else {
                    printf ("Reg %s return error\n", name_copy.c_str());
                }
                cforest_written_value = temp_cur_cforest;
                CbVcRegEx(name_copy, cforest_written_value, cforest_pre_val, vcid);
                v_prev_reg.erase(v_prev_reg.begin() + VT0_pos);
                v_prev_reg.insert (v_prev_reg.begin() + VT0_pos, cforest_written_value);
                if ((reg_profile->m_context.compare("VS")) == 0) {
                    v_prev_reg.erase(v_prev_reg.begin() + NC_pos);
                    v_prev_reg.insert (v_prev_reg.begin() + NC_pos, cforest_written_value);
                    for (uint32_t i = 2; i < v_prev_reg.size(); i++) {
                        v_prev_reg.erase(v_prev_reg.begin() + i);
                        v_prev_reg.insert (v_prev_reg.begin() + i, cforest_written_value);
                    }
                }
                p_prev_reg_frog.erase (it_p_prev_reg_frog);
                p_prev_reg_frog.insert (std::pair<std::string, std::vector <FrogRegData> > (reg_profile->m_reg_name, v_prev_reg));
            }
        } else {
            return GCSIF_ERR_INTERNAL;
        }
    }
    return GCSIF_SUCCESS;
}


uint32_t gcs_get_vc_register(const std::string &name, FrogRegData* value, uint32_t vcid) {
    uint32_t context = 1;
    uint32_t pe_id = m_forest_util->GetPeLastId();
    FrogRegData cforest_pre_val = 0;
    uint64_t temp_pre_cforest = 0;
    // Simulator is not initialized.
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_NOT_INITIALIZED;
    }

    if ((!m_forest_util->IsValidPeId(pe_id)) || ((int32_t)vcid != NC)){
        return GCSIF_ERR_INVALID_VCID;
    }
    std::string name_copy; //(boost::to_upper_copy<std::string>(name));
    ToUpperCopy (name, &name_copy);
    std::map <std::string, std::vector <FrogRegData > >::iterator it_p_prev_reg_frog = p_prev_reg_frog.find(name_copy);
    std::map<std::string, FrogRegPro*>::iterator it_frog_reg_map = frog_reg_map.find(name_copy);

    if ((it_p_prev_reg_frog == p_prev_reg_frog.end()) || (it_frog_reg_map == frog_reg_map.end())) {
        return GCSIF_ERR_INVALID_REGISTER_NAME;
    } else {
        std::vector <FrogRegData > v_prev_reg = it_p_prev_reg_frog->second;
        FrogRegData frog_pre_val = v_prev_reg.at(VT0_pos);
        FrogRegPro * reg_profile = it_frog_reg_map->second;

        if ((v_prev_reg.size() >= 2) && (reg_profile != NULL)) {
            bool sreg_return = m_forest_util->RegReadDebug (pe_id, reg_profile->m_creg, &temp_pre_cforest);
            cforest_pre_val = temp_pre_cforest & 0xFFFFFFFFU;
            if ((sreg_return == false) || (reg_profile->m_context.compare("N") == 0)
                                       || (reg_profile->m_context.compare("T") == 0)|| (reg_profile->m_context.compare("NT") == 0)) {
                return GCSIF_ERR_INVALID_CONTEXT;
            } else {
                if (cforest_pre_val != frog_pre_val) {
                    miss_match_message (cforest_pre_val, frog_pre_val, name_copy, "gcs_get_vc_register");
                }
                *value = (unsigned long long) cforest_pre_val;
            }
        } else {
            return GCSIF_ERR_INTERNAL;
        }
    }
    return GCSIF_SUCCESS;
}


uint32_t gcs_set_tc_register(const std::string &name, FrogRegData value, uint32_t tcid) {
    uint32_t context = 1;
    uint32_t pe_id = m_forest_util->GetPeLastId();
    FrogRegData cforest_pre_val = 0, cforest_written_value = 0;
    uint64_t temp_pre_cforest = 0, temp_cur_cforest = 0;
    uint64_t temp_pre_u64_cforest = 0, temp_cur_u64_cforest = 0;
    // Simulator is not initialized.
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_NOT_INITIALIZED;
    }

    if ((!m_forest_util->IsValidPeId(pe_id)) || ((int32_t)tcid != NC)){
        return GCSIF_ERR_INVALID_TCID;
    }
    std::string name_copy; //(boost::to_upper_copy<std::string>(name));
    ToUpperCopy (name, &name_copy);
    std::map <std::string, std::vector <FrogRegData > >::iterator it_p_prev_reg_frog = p_prev_reg_frog.find(name_copy);
    std::map<std::string, FrogRegPro*>::iterator it_frog_reg_map = frog_reg_map.find(name_copy);

    if ((it_p_prev_reg_frog == p_prev_reg_frog.end()) || (it_frog_reg_map == frog_reg_map.end())) {
        return GCSIF_ERR_INVALID_REGISTER_NAME;
    } else {
        std::vector <FrogRegData > v_prev_reg = it_p_prev_reg_frog->second;
        FrogRegData frog_pre_val = v_prev_reg.at(VT0_pos);
        FrogRegPro * reg_profile = it_frog_reg_map->second;

        if ((v_prev_reg.size() >= 2) && (reg_profile != NULL)) {
            bool sreg_return = m_forest_util->RegReadDebug (pe_id, reg_profile->m_creg, &temp_pre_cforest, &temp_pre_u64_cforest);
            cforest_pre_val = (static_cast<FrogRegData>(temp_pre_u64_cforest) << 64)
                            | static_cast<FrogRegData>(temp_pre_cforest);
            if ((sreg_return == false) || (reg_profile->m_context.compare("N") == 0) || (reg_profile->m_context.compare("NV") == 0)
                                       || (reg_profile->m_context.compare("V") == 0) || (reg_profile->m_context.compare("VS") == 0)) {
                return GCSIF_ERR_INVALID_CONTEXT;
            } else {
                if ((cforest_pre_val != frog_pre_val) && (reg_profile->m_creg != PC)) {
                    miss_match_message (cforest_pre_val, frog_pre_val, name_copy, "gcs_set_tc_register");
                }

                // Write input value to NC register
                uint64_t data_l64 = static_cast <uint64_t> (value & 0xFFFFFFFFFFFFFFFFULL);
                uint64_t data_u64 = static_cast <uint64_t> ((value >> 64) & 0xFFFFFFFFFFFFFFFFULL);
                sreg_return = m_forest_util->RegWriteDebug (pe_id, reg_profile->m_creg, data_l64, data_u64);
                if (sreg_return == false) {
                    printf ("Reg %s return error\n", name_copy.c_str());
                }
                // Read writable value from NC register
                sreg_return = m_forest_util->RegReadDebug (pe_id, reg_profile->m_creg, &temp_cur_cforest, &temp_cur_u64_cforest);
                cforest_written_value = (static_cast<FrogRegData>(temp_cur_u64_cforest) << 64)
                                      | static_cast<FrogRegData>(temp_cur_cforest);
                CbTcRegEx(name_copy, cforest_written_value, cforest_pre_val, tcid);
                v_prev_reg.erase(v_prev_reg.begin() + VT0_pos);
                v_prev_reg.insert (v_prev_reg.begin() + VT0_pos, cforest_written_value);
                if ((reg_profile->m_context.compare("T")) == 0) {
                    v_prev_reg.erase(v_prev_reg.begin() + NC_pos);
                    v_prev_reg.insert (v_prev_reg.begin() + NC_pos, cforest_written_value);
                }
                p_prev_reg_frog.erase (it_p_prev_reg_frog);
                p_prev_reg_frog.insert (std::pair<std::string, std::vector <FrogRegData> > (reg_profile->m_reg_name, v_prev_reg));

                if (name_copy.compare("FPSR") == 0) {
                    // Update FPCFG depends on FPSR;
                    update_fpu_fxu (pe_id, "FPCFG");

                    // Update FPCC depends on FPSR;
                    update_fpu_fxu (pe_id, "FPCC");

                    // Update FPST depends on FPSR;
                    update_fpu_fxu (pe_id, "FPST");

                } else if (name_copy.compare("FXSR") == 0) {
                    // Update FXCFG depends on FXSR;
                    update_fpu_fxu (pe_id, "FXCFG");

                    // Update FXST depends on FXSR;
                    update_fpu_fxu (pe_id, "FXST");

                } else if (name_copy.compare("FXCFG") == 0) {
                    // Update FXSR depends on FXCFG;
                    update_fpu_fxu (pe_id, "FXSR");

                } else if (name_copy.compare("FXST") == 0) {
                    // Update FXSR depends on FXST;
                    update_fpu_fxu (pe_id, "FXSR");

                } else if ((name_copy.compare("PSW") == 0) && (v_prev_reg.size() >= 2)) {
                    // not use (v_prev_reg.size() >= 2) for G4MH. Don't need to update for GMPSW/PSW here.
                    uint64_t value_NC, value_TC0;
                    sreg_return = m_forest_util->RegReadDebug (pe_id, SR_PSW, (&value_NC));
                    if (sreg_return == true) {
                        sreg_return = m_forest_util->RegReadDebug (pe_id, SR_PSW, (&value_TC0));
                        if (sreg_return == false) {
                            printf ("Reg %s return error\n", name_copy.c_str());
                        }
                    } else {
                        printf ("Reg %s return error\n", name_copy.c_str());
                    }
                    v_prev_reg.erase(v_prev_reg.begin() + NC_pos);
                    v_prev_reg.insert (v_prev_reg.begin() + NC_pos, (FrogRegData) value_NC);
                    v_prev_reg.erase(v_prev_reg.begin() + VT0_pos);
                    v_prev_reg.insert (v_prev_reg.begin() + VT0_pos, (FrogRegData) value_TC0);
                    p_prev_reg_frog.erase (it_p_prev_reg_frog);
                    p_prev_reg_frog.insert (std::pair<std::string, std::vector <FrogRegData> > (reg_profile->m_reg_name, v_prev_reg));
                }
            }
        } else {
            return GCSIF_ERR_INTERNAL;
        }
    }
    gcs_internal_record_reg_write (pe_id, tcid, name_copy);
    // Update internal value record for reference registers
    while (gcs_internal_IsEmpty_reg_write () == false) {
        RegTupleType reg_rec = gcs_internal_get_reg_write ();
        gcs_internal_update_referenced_register (std::get<0>(reg_rec), std::get<1>(reg_rec), std::get<2>(reg_rec));
    }

    return GCSIF_SUCCESS;
}


uint32_t gcs_get_tc_register(const std::string &name, FrogRegData* value, uint32_t tcid) {
    uint32_t context = 1;
    uint32_t pe_id = m_forest_util->GetPeLastId();
    FrogRegData cforest_pre_val = 0;
    uint64_t temp_pre_cforest = 0;
    uint64_t temp_pre_u64_cforest = 0;
    // Simulator is not initialized.
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_NOT_INITIALIZED;
    }

    if ((!m_forest_util->IsValidPeId(pe_id)) || ((int32_t)tcid != NC)){
        return GCSIF_ERR_INVALID_TCID;
    }

    std::string name_copy; //(boost::to_upper_copy<std::string>(name));
    ToUpperCopy (name, &name_copy);
    std::map <std::string, std::vector <FrogRegData > >::iterator it_p_prev_reg_frog = p_prev_reg_frog.find(name_copy);
    std::map<std::string, FrogRegPro*>::iterator it_frog_reg_map = frog_reg_map.find(name_copy);

    if ((it_p_prev_reg_frog == p_prev_reg_frog.end()) || (it_frog_reg_map == frog_reg_map.end())) {
        return GCSIF_ERR_INVALID_REGISTER_NAME;
    } else {
        std::vector <FrogRegData > v_prev_reg = it_p_prev_reg_frog->second;
        FrogRegData frog_pre_val = v_prev_reg.at(VT0_pos);
        FrogRegPro * reg_profile = it_frog_reg_map->second;

        if ((v_prev_reg.size() >= 2) && (reg_profile != NULL)) {
            bool sreg_return = m_forest_util->RegReadDebug (pe_id, reg_profile->m_creg, &temp_pre_cforest, &temp_pre_u64_cforest);
            cforest_pre_val = (static_cast<FrogRegData>(temp_pre_u64_cforest) << 64)
                            | static_cast<FrogRegData>(temp_pre_cforest);
            if ((sreg_return == false) || (reg_profile->m_context.compare("N") == 0) || (reg_profile->m_context.compare("NV") == 0)
                                       || (reg_profile->m_context.compare("V") == 0) || (reg_profile->m_context.compare("VS") == 0)) {
                return GCSIF_ERR_INVALID_CONTEXT;
            } else {
                if ((cforest_pre_val != frog_pre_val) && (reg_profile->m_creg != PC)) {
                    miss_match_message (cforest_pre_val, frog_pre_val, name_copy, "gcs_get_tc_register");
                }
                *value = cforest_pre_val;
            }
        } else {
            return GCSIF_ERR_INTERNAL;
        }
    }
    return GCSIF_SUCCESS;
}


uint32_t gcs_write_simulator_tlb(uint32_t index, uint32_t telo0, uint32_t telo1, uint32_t tehi0, uint32_t tehi1) {
    if (m_forest_util == NULL) {
        return GCSIF_ERR_NOT_INITIALIZED;
    }
    if (/*(index < 0)||*/ (index > 63)) {
        return GCSIF_ERR_INVALID_OTHER_PARAM;
    }
    return GCSIF_ERR_MMU_DISABLED;
}


uint32_t gcs_read_simulator_tlb (uint32_t index, uint32_t* telo0, uint32_t* telo1, uint32_t* tehi0, uint32_t* tehi1) {
    if (m_forest_util == NULL) {
        return GCSIF_ERR_NOT_INITIALIZED;
    }
    if (/*(index < 0)||*/  (index > 63)) {
        return GCSIF_ERR_INVALID_OTHER_PARAM;
    }
    return GCSIF_ERR_MMU_DISABLED;
}


void gcs_get_simulator_version (std::string &version)
{
    version = m_forest_util->GetVersionStr();
    return;
}


uint32_t gcs_read_simulator_memory(uint64_t address64, uint32_t size, std::string &bytes)
{
//    gcsMsg << "< Info : Read data from memory.>" << std::endl;
    if (m_forest_util == NULL) {
        return GCSIF_ERR_NOT_INITIALIZED;
    }
    if ((!bytes.empty()) || size == 0) {
        return GCSIF_ERR_INVALID_OTHER_PARAM;
    }

    uint32_t address = address64 & 0xFFFFFFFFU;
    static std::vector<char> array;
    array.clear();
    uint8_t buffer;
    char *buff;
    unsigned long long read_callback = 0;
    for (uint32_t i = 0; i < size; i++) {
        m_forest_util->BusReadDebug (address + i, &buffer, 1);
        read_callback += buffer << (i*8);
        array.push_back(buffer);
    }
    gcs_internal_make_data_string(size, &array[0], &buff);
    bytes.append(buff);
//    strcpy(bytes, buff);

    // Execute callback function when values load from memory
    if (m_forest_util->IsRomArea(address)) {
        CbMemoryEx(address, size, read_callback,
                   0ULL, FROG_ACCESS_TYPE_READ, FROG_ACCESS_BUS_FETCH, FROG_LOAD_ACTION_LOAD);
    } else {
        CbMemoryEx(address, size, read_callback,
                   0ULL, FROG_ACCESS_TYPE_READ, FROG_ACCESS_BUS_RW, FROG_LOAD_ACTION_LOAD);
    }
    return GCSIF_SUCCESS;
}


uint32_t gcs_write_simulator_memory( uint64_t address64, uint32_t size, const std::string &bytes)
{
//    gcsMsg << "< Info : Write data to memory.>" << std::endl;
    if (bytes.empty() || size == 0) {
        return GCSIF_ERR_INVALID_OTHER_PARAM;
    }

    int datalen = 0;
    char* buffer = NULL;
    if (gcs_internal_make_string_data(&datalen, bytes.c_str(), &buffer) != 0) {
        gcsMsg << "Error : Internal error : fail to decode data string [" << bytes << "]." << std::endl;
        return GCSIF_ERR_INTERNAL;
    }
    if (datalen != (int32_t)size) {
        /* Not match decode length with parameter */
        gcsMsg << "Error : Internal error : Not match decode length["
               << std::dec << datalen << "] with parameter [" << size << "]." << std::endl;
        return GCSIF_ERR_INTERNAL;
    }

    uint32_t address = address64 & 0xFFFFFFFFU;
    uint32_t context = 1;
    unsigned long long write_callback = 0;
    // Simulator is not initialized.
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_NOT_INITIALIZED;
    }
    uint8_t read_value;
    for (uint32_t i = 0; i < size; i++) {
        m_forest_util->BusWriteDebug (address + i, reinterpret_cast<MemData8*> (buffer), 1);
        m_forest_util->BusReadDebug (address + i, &read_value, 1);
        write_callback += read_value << (i*8);
        ++ buffer;
    }
    // Execute callback function when values store to memory
    if (m_forest_util->IsRomArea(address)) {
        CbMemoryEx(address, size, write_callback,
                   0ULL, FROG_ACCESS_TYPE_WRITE, FROG_ACCESS_BUS_FETCH, FROG_LOAD_ACTION_LOAD);
    } else {
        CbMemoryEx(address, size, write_callback,
                   0ULL, FROG_ACCESS_TYPE_WRITE, FROG_ACCESS_BUS_RW, FROG_LOAD_ACTION_LOAD);
    }

    return GCSIF_SUCCESS;
}


uint32_t gcs_create_memory_link( uint64_t address64, bool isNc, uint32_t ptid, uint32_t size) {
    // Convert tcid from FROG to CForest
    int tcid_cforest = ptid;
    uint32_t context = 0;
    // Simulator is not initialized.
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_NOT_INITIALIZED;
    }
    uint32_t pe_id = m_forest_util->GetPeLastId();

    if ((isNc == true) && (ptid == 0)) {
        tcid_cforest = NC;
    }

    if ((!m_forest_util->IsValidPeId(pe_id)) || (tcid_cforest != NC)){
        return GCSIF_ERR_INVALID_TCID;
    }

    uint32_t address = address64 & 0xFFFFFFFFU;
    uint32_t scid = m_forest_util->GetScId(pe_id);
    m_forest_util->CreateLLBit (scid, (PhysAddr) address, size);
    TraceCtrl* trace = const_cast<TraceCtrl*> (m_forest_util->GetTraceCtrl (scid));
    if (trace != NULL && trace->GetLLBitAdd().first) {
        CbLLbit (pe_id, trace->GetLLBitAdd().first, trace->GetLLBitAdd().second, context, ptid, FROG_LLBIT_CREATE);
    } else {
        return GCSIF_ERR_INTERNAL;
    }
    return GCSIF_SUCCESS;
}


uint32_t  gcs_remove_memory_link_addr( uint64_t address64 ) {
    uint32_t context = 0;
    const PhysAddr LINK_ADDR_MASK = ~0x1FU;
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_NOT_INITIALIZED;
    }
    uint32_t peid = m_forest_util->GetPeLastId();
    uint32_t tcid = m_forest_util->GetTcLastId(peid);
    uint32_t scid = m_forest_util->GetScId(peid);

    uint32_t address = address64 & 0xFFFFFFFFU;
    // Try to delete LLBit in current peid with specified address.
    m_forest_util->DeleteLLBit (scid, (PhysAddr) address);

    TraceCtrl* trace;
    trace = const_cast<TraceCtrl*> (m_forest_util->GetTraceCtrl (scid));
    if (trace) {
        int32_t llbit_del = trace->GetDeletedLLBitCount();
        std::pair <int32_t, PhysAddr> llbit_del_val;
        for (int32_t llbit_pos =0; llbit_pos < llbit_del; ++llbit_pos) {
            llbit_del_val = trace->GetDeletedLLBit(llbit_pos);
            int32_t pe_id = llbit_del_val.first;
            if ((llbit_del_val.second & LINK_ADDR_MASK) == (address & LINK_ADDR_MASK)) {
                // Call LLBit callback function and delete LLBit in trace info
                CbLLbit (pe_id, llbit_del_val.second, LLBIT_DEL_SIZE, NC, tcid, FROG_LLBIT_DELETE);
                trace->DelLLBit (peid, llbit_del_val.second);
                break;
            }
        }
    }
    return GCSIF_SUCCESS;
}


uint32_t gcs_disable_simulator_stop(uint32_t disable)
{
    delete m_cmdopt;
    m_cmdopt        = NULL;
    delete m_forest_util;
    m_forest_util   = NULL;
    p_prev_reg_frog.clear();
    frog_reg_map.clear();
    delete g_reg_record;

    /* clear callback entry */
    CbComment       = NULL;
    CbLLbit         = NULL;
    CbMemoryEx      = NULL;
    CbNcRegEx       = NULL;
    CbVcRegEx       = NULL;
    CbTcRegEx       = NULL;

    g_last_exec_exp_code = EXP_NONE;
    return GCSIF_SUCCESS;
}


uint32_t gcs_is_simulator_native(uint32_t* mode)
{
//    gcsMsg << "< Info : Checking gcs is in native mode or not. >" << std::endl;
    uint32_t psw_data;

    if (m_forest_util == NULL) {
        return GCSIF_ERR_NOT_INITIALIZED;
    }

    uint32_t err = gcs_internal_get_psw_on_current_mode (&psw_data);
    if (err != GCSIF_SUCCESS) {
        return err;
    }

    if ((psw_data >> 31) & 0x01) {
        *mode = 0;
    } else {
        *mode = 1;
    }

    return GCSIF_SUCCESS;
}


bool gcs_check_fxu_function (void ) {
    if (m_forest_util == NULL) {
        printf ("Error : gcs simulator is not initialized.\n");
        return false;
    }
    return m_cmdopt->HasFxu();
}


bool gcs_check_fpu_function (void ) {
    if (m_forest_util == NULL) {
        printf ("Error : gcs simulator is not initialized.\n");
        return false;
    }
    return m_cmdopt->HasFpu();
}


/**
 * \fn  void PrintFROGInfo (std::ostringstream *ss, TraceCtrl *trace, uint32_t ptid, int32_t bNative, uint64_t PSW_val)
 * \brief This function print FROG information.
 *
 * @param[out] ss      Message for printing.
 * @param[in]  trace   pointer to Trace info.
 * @param[in]  ptid    thread ID.
 * @param[in]  bNative Native context.
 * @param[in]  PSW_val Value of PSW.
 *  @return fpu_fxu was updated successfully.
 */
void PrintFROGInfo (std::ostringstream *ss, TraceCtrl *trace, uint32_t ptid, int32_t bNative, uint64_t PSW_val)
{
    /* PTID */
    *ss << "PTID=" << std::setw(2) << std::setfill('0') << std::hex << ptid;
    if (bNative){
        *ss << "[H] | ";
    } else {
        *ss << "[V] | ";
    }
    /* Program Counter */
    *ss << "PC=" << std::setw(8) << std::setfill('0') << std::hex << trace->GetInstAddr();

    /* Instruction Code */
    *ss << " | ICODE";
    *ss << std::dec << trace->GetInstSize() * 8 << " = ";
    if ( trace->GetInstSize() == 2 ){
        *ss << "        " << std::hex << std::setfill('0') << std::setw(4) << (trace->GetInstCode() & 0xFFFF) << " ";
    }else if ( trace->GetInstSize() == 4 ){
        *ss << "    " << std::hex << std::setfill('0') << std::setw(8) << (trace->GetInstCode() & 0xFFFFFFFF) << " ";
    } else {
        *ss << std::hex << std::setfill('0') << std::setw(12) << (trace->GetInstCode() & 0xFFFFFFFFFFFF) << " ";
    }

    /* PSW */
    *ss << " | PSW[7:0]=" << std::setw(2) << std::setfill('0') << std::hex << (PSW_val & 0xff);
}


static std::string gcs_internal_build_comment(uint32_t ptid, int32_t bNative, TraceCtrl *trace)
{
    TracePrintOperand m_print_operand;
    uint32_t  peid = trace->GetPeId ();
    ContextId tcid = NC;
    ContextId vcid = NC;

    std::ostringstream ss;
    uint64_t PSW_val;

    uint32_t local_data = 0;
    uint32_t err = gcs_internal_get_psw_on_current_mode (&local_data);
    if (err != GCSIF_SUCCESS) {
        printf ("Can't get value of PSW \n");
    }
    PSW_val = local_data;
    std::string inst_mnem;

    if (trace->GetExpCode() != EXP_NONE) {
        inst_mnem = ExpInfo::GetName(trace->GetExpCode()) ;
        inst_mnem.append(" [EXP]");
    } else {
        m_print_operand.PrintOperand(trace->GetInstId (), trace->GetInstCode (),
                                  trace->GetInstAddr (), &inst_mnem);
    }
    uint32_t wreg_count = trace->GetWregCount ();
    uint32_t wmem_count = trace->GetWmemCount();
    TracePrintCForest *trace_print = new TracePrintCForest (CORE_G4MH);

    if (wreg_count == 0 && wmem_count == 0) {
        PrintFROGInfo (&ss, trace, ptid, bNative, PSW_val);
        /* Register Update */
        ss << " | ";
        ss << std::setfill(' ') << std::setw (21) << ' ';

        /* StAddr and StData */
        ss << " | ";
        ss << std::setfill(' ') << std::setw (31) << ' ';

        /* Mnemonic */
        ss << " | ";
        char      vcidstr = ((vcid == NC) ? 'M' : static_cast<char>('0' + vcid));
        PrivLevel priv = trace->GetPrivLevel ();
        ss << "P" << peid << ":" << ((vcid == NC) ? 'N' : 'V') << vcidstr
             << ":T" << ((tcid == NC) ? 0 : tcid)  << ":" << CfMsg::GetPrivName (priv) << ":";
        ss << "   ";
        ss << std::left << std::setfill(' ');
        ss << std::setw(23) << inst_mnem << " | ";
        trace_print->PrintRegMemTrace ( &ss, trace);
        trace_print->PrintBranchTrace ( &ss, trace);

        ss << std::endl;
    } else {
        uint32_t PSW_update = 0;
        CReg regno;
        for (uint32_t i = 0; i < wreg_count; ++i) {
            regno = trace->GetWriteReg (i);
            if (regno >= HMPSW_Z && regno <= GMPSW_VM) {
                PSW_update = 1;
                break; // Why break instead of continue?
            }
            PrintFROGInfo (&ss, trace, ptid, bNative, PSW_val);

            /* Register Update */
            ss << " | ";
            ss << std::left  << std::setfill(' ') << std::setw(10) << RegID::GetName(trace->GetWriteReg(i)) << " = ";
            if (regno >= WR0 && regno <= WR31) {
                ss << std::right << std::setfill('0') << std::setw(8) << trace->GetWWregValue(i, 3) << std::setfill('0') << std::setw(8) << trace->GetWWregValue(i, 2);
                ss << std::right << std::setfill('0') << std::setw(8) << trace->GetWWregValue(i, 1) << std::setfill('0') << std::setw(8) << trace->GetWWregValue(i, 0);
            } else {
                ss << std::right << std::setfill('0') << std::setw(8) <<trace->GetWregValue(i);
            }

            /* StAddr and StData */
            ss << " | ";
            ss << std::setfill(' ') << std::setw (31) << ' ';
            ss << " | ";

            /* Mnemonic */
            char      vcidstr = ((vcid == NC) ? 'M' : static_cast<char>('0' + vcid));
            PrivLevel priv = trace->GetPrivLevel ();
            ss << "P" << peid << ":" << ((vcid == NC) ? 'N' : 'V') << vcidstr
                 << ":T" << ((tcid == NC) ? 0 : tcid)  << ":" << CfMsg::GetPrivName (priv) << ":";
            ss << "   ";
            ss << std::left << std::setfill(' ');
            ss << std::setw(23) << inst_mnem << " | ";
            trace_print->PrintRegMemTrace ( &ss, trace);
            trace_print->PrintBranchTrace ( &ss, trace);

            ss << std::endl;
        }
        if (PSW_update) {
            PrintFROGInfo (&ss, trace, ptid, bNative, PSW_val);
            bool use_gmpsw = false;
            err = gcs_internal_use_gmpsw (&use_gmpsw);
            if (err != GCSIF_SUCCESS) {
                printf ("Can't get current mode\n");
            }

            /* Register Update */
            ss << " | ";
            if (use_gmpsw) {
                ss << std::left  << std::setfill(' ') << std::setw(10) << RegID::GetName(SR_GMPSW) << " = ";
            } else {
                ss << std::left  << std::setfill(' ') << std::setw(10) << RegID::GetName(SR_PSW) << " = ";
            }
            ss << std::right << std::setfill('0') << std::setw(8) <<PSW_val;

            /* StAddr and StData */
            ss << " | ";
            ss << std::setfill(' ') << std::setw (31) << ' ';
            ss << " | ";

            /* Mnemonic */
            char      vcidstr = ((vcid == NC) ? 'M' : static_cast<char>('0' + vcid));
            PrivLevel priv = trace->GetPrivLevel ();
            ss << "P" << peid << ":" << ((vcid == NC) ? 'N' : 'V') << vcidstr
                 << ":T" << ((tcid == NC) ? 0 : tcid)  << ":" << CfMsg::GetPrivName (priv) << ":";
            ss << "   ";
            ss << std::left << std::setfill(' ');
            ss << std::setw(23) << inst_mnem << " | ";
            trace_print->PrintRegMemTrace ( &ss, trace);
            trace_print->PrintBranchTrace ( &ss, trace);

            ss << std::endl;
        }
        for (uint32_t i = 0; i < wmem_count; ++i) {
            {
                PrintFROGInfo (&ss, trace, ptid, bNative, PSW_val);

                /* Register Update */
                ss << " | ";
                ss << std::setfill(' ') << std::setw (21) << ' ';

                /* StAddr and StData */
                ss << " | ";
                ss << "StAddr="<< std::hex << std::setfill('0') << std::setw(8) << (trace->GetWmemAddr(i) & 0xffffffff) << " ";
                ss << "StData="<< std::hex << std::setfill('0') << std::setw(8) << (trace->GetWmemValueL64(i) & 0xffffffff);

                /* Mnemonic */
                ss << " | ";
                char      vcidstr = ((vcid == NC) ? 'M' : static_cast<char>('0' + vcid));
                PrivLevel priv = trace->GetPrivLevel ();
                ss << "P" << peid << ":" << ((vcid == NC) ? 'N' : 'V') << vcidstr
                     << ":T" << ((tcid == NC) ? 0 : tcid)  << ":" << CfMsg::GetPrivName (priv) << ":";
                ss << "   ";
                ss << std::left << std::setfill(' ');
                ss << std::setw(23) << inst_mnem << " | ";
                trace_print->PrintRegMemTrace ( &ss, trace);
                trace_print->PrintBranchTrace ( &ss, trace);
                ss << std::endl;
            }
            if (trace->GetWmemSize(i) == 16) {
                // Output upper64bit additionally in QW access
                PrintFROGInfo (&ss, trace, ptid, bNative, PSW_val);

                /* Register Update */
                ss << " | ";
                ss << std::setfill(' ') << std::setw (21) << ' ';

                /* StAddr and StData */
                ss << " | ";
                ss << "StAddr="<< std::hex << std::setfill('0') << std::setw(8) << ((trace->GetWmemAddr(i) + 8) & 0xffffffff) << " ";
                ss << "StData="<< std::hex << std::setfill('0') << std::setw(8) << (trace->GetWmemValueU64(i) & 0xffffffff);

                /* Mnemonic */
                ss << " | ";
                char      vcidstr = ((vcid == NC) ? 'M' : static_cast<char>('0' + vcid));
                PrivLevel priv = trace->GetPrivLevel ();
                ss << "P" << peid << ":" << ((vcid == NC) ? 'N' : 'V') << vcidstr
                        << ":T" << ((tcid == NC) ? 0 : tcid)  << ":" << CfMsg::GetPrivName (priv) << ":";
                ss << "   ";
                ss << std::left << std::setfill(' ');
                ss << std::setw(23) << inst_mnem << " | ";
                trace_print->PrintRegMemTrace ( &ss, trace);
                trace_print->PrintBranchTrace ( &ss, trace);
                ss << std::endl;
            }
        }
    }
    delete trace_print;

    return ss.str();
}

static std::string gcs_internal_get_frog_name (CReg regno) {
    std::string frog_name = RegID::GetName(regno);
    std::string::size_type n = frog_name.find(":");
    if (n == 2) {
        return frog_name.substr(n + 1);
    }
    std::string name_copy; //(boost::to_upper_copy<std::string>(frog_name));
    ToUpperCopy (frog_name, &name_copy);
    return name_copy;
}


/**
 *   This function translates from register name to register id for CForest.
 *   This is also internal function.
 *
 * @param[in]  reg_name register name
 * @param[out] creg_id  translated register id for CForest
 * @return Error code
 *         GCSIF_ERR_INVALID_REGISTER_NAME : Unknown register name.
 *         GCSIF_SUCCESS                   : Successful
 */
static int gcs_internal_get_creg (std::string reg_name, uint32_t* creg_id)
{
    std::map<std::string, FrogRegPro*>::iterator it_frog_reg_map = frog_reg_map.find(reg_name);
    if (it_frog_reg_map != frog_reg_map.end()) {
        FrogRegPro *reg_profile = it_frog_reg_map->second;
        *creg_id = reg_profile->m_creg;
        return GCSIF_SUCCESS;
    } else {
        *creg_id = 0;
        return GCSIF_ERR_INVALID_REGISTER_NAME;
    }
}


/**
 *   This function updates register value which recorded in FROG I/F.
 *   @note No support mirror register which includes shread register between NM and TC0.
 *   This is also internal function.
 *
 * @tparam    is_call_cb ICT_CALL_CB=call callback, others=no call.
 * @param[in] tcid       thread context which has the register.
 * @param[in] reg_name   register name
 * @param[in] data       register value
 * @return Error code
 *         GCSIF_ERR_INVALID_REGISTER_NAME : Unknown register name.
 *         GCSIF_SUCCESS                   : Successful
 */
template <IsCallCBType is_call_cb>
static int gcs_internal_update_register (int32_t tcid, std::string reg_name, FrogRegData data)
{
    std::map<std::string, FrogRegPro*>::iterator it_frog_reg_map = frog_reg_map.find(reg_name);
    if (it_frog_reg_map != frog_reg_map.end()) {
        FrogRegPro *reg_profile = it_frog_reg_map->second;

        std::map <std::string, std::vector <FrogRegData > >::iterator it_p_prev_reg_frog = p_prev_reg_frog.find(reg_name);
        if (it_p_prev_reg_frog != p_prev_reg_frog.end()) {
            std::vector <FrogRegData > v_prev_reg = it_p_prev_reg_frog->second;

            FrogRegData frog_pre_val;
            if (tcid == NC) {
                frog_pre_val = v_prev_reg.at(NC_pos);
                v_prev_reg.erase(v_prev_reg.begin() + NC_pos);
                v_prev_reg.insert (v_prev_reg.begin() + NC_pos, data);
            } else {
                frog_pre_val = v_prev_reg.at(VT0_pos);
                v_prev_reg.erase(v_prev_reg.begin() + VT0_pos);
                v_prev_reg.insert (v_prev_reg.begin() + VT0_pos, data);
            }

            p_prev_reg_frog.erase (it_p_prev_reg_frog);
            p_prev_reg_frog.insert (std::pair<std::string, std::vector <FrogRegData> > (reg_profile->m_reg_name, v_prev_reg));

            if (is_call_cb == IsCallCBType::ICT_CALL_CB) {
                if (tcid == NC) {
                    CbNcRegEx(reg_name, data, frog_pre_val);
                } else {
                    CbTcRegEx(reg_name, data, frog_pre_val, tcid);
                }
            }

            return GCSIF_SUCCESS;
        } else {
            // Don't come here. register is valid, but not target for prev value.
            return GCSIF_ERR_INVALID_REGISTER_NAME;
        }
    } else {
        return GCSIF_ERR_INVALID_REGISTER_NAME;
    }
}


/**
 *   This function reads register value from CForest.
 *   This is also internal function.
 *
 * @param[in]  peid     peid for thread context
 * @param[in]  tcid     thread context which has the register
 * @param[in]  reg_name register name
 * @param[out] data     read register value
 * @return Error code
 *         GCSIF_ERR_INVALID_REGISTER_NAME : Unknown register name or invalid register in the context.
 *         GCSIF_SUCCESS                   : Successful
 */
static int gcs_internal_get_register (uint32_t peid, int32_t tcid, const std::string &reg_name, FrogRegData* data)
{
    uint32_t creg_id = 0;
    if (gcs_internal_get_creg (reg_name, &creg_id) != GCSIF_SUCCESS) {
        *data = 0;
        return GCSIF_ERR_INVALID_REGISTER_NAME;
    } else {
        uint64_t reg_value = 0;
        uint64_t reg_value_u64 = 0;
        if (m_forest_util->RegReadDebug (peid, creg_id, &reg_value, &reg_value_u64) == true) {
            *data = (static_cast<FrogRegData>(reg_value_u64) << 64)
                  | static_cast<FrogRegData>(reg_value);
            return GCSIF_SUCCESS;
        } else {
            *data = 0;
            return GCSIF_ERR_INVALID_REGISTER_NAME;
        }
    }
}


/**
 *   This function reads register value from FROG I/F.
 *   This is also internal function.
 *
 * @param[in]  tcid     thread context which has the register
 * @param[in]  reg_name register name
 * @param[out] data     read register value
 * @return Error code
 *         GCSIF_ERR_INVALID_REGISTER_NAME : Unknown register name.
 *         GCSIF_SUCCESS                   : Successful
 */
static int gcs_internal_get_register_frogif (int32_t tcid, std::string reg_name, FrogRegData* data)
{
    std::map<std::string, FrogRegPro*>::iterator it_frog_reg_map = frog_reg_map.find(reg_name);
    if (it_frog_reg_map != frog_reg_map.end()) {
        std::map <std::string, std::vector <FrogRegData > >::iterator it_p_prev_reg_frog = p_prev_reg_frog.find(reg_name);
        if (it_p_prev_reg_frog != p_prev_reg_frog.end()) {
            std::vector <FrogRegData > v_prev_reg = it_p_prev_reg_frog->second;

            if (tcid == NC) {
                *data = v_prev_reg.at(NC_pos);
            } else {
                *data = v_prev_reg.at(VT0_pos);
            }
            return GCSIF_SUCCESS;
        } else {
            // Don't come here, register is valid, but not target for prev value.
            *data = 0;
            return GCSIF_ERR_INVALID_REGISTER_NAME;
        }
    } else {
        *data = 0;
        return GCSIF_ERR_INVALID_REGISTER_NAME;
    }
}


/**
 *   This function compare new register value of CForest and previous register value of FROG I/F,
 *   and update register value which recorded in FROG I/F.
 *   @note No support mirror register which includes shread register between NM and TC0.
 *   This is also internal function.
 *
 * @tparam    is_call_cb ICT_CALL_CB=call callback, others=no call.
 * @param[in] peid       peid for thread context
 * @param[in] tcid       thread context which has the register
 * @param[in] reg_name   register name
 * @return Error code
 *         GCSIF_ERR_INVALID_REGISTER_NAME : Unknown register name.
 *         GCSIF_SUCCESS                   : Successful
 */
template <IsCallCBType is_call_cb>
static int gcs_internal_compare_and_update_register_frogif (uint32_t peid, int32_t tcid, const std::string &reg_name)
{
    // Get previous register value from FROG I/F
    FrogRegData prev_value = 0;
    if (gcs_internal_get_register_frogif (tcid, reg_name, &prev_value) != GCSIF_SUCCESS) {
        return GCSIF_ERR_INVALID_REGISTER_NAME;
    }

    // Get latest register value from CForest
    FrogRegData post_value = 0;
    if (gcs_internal_get_register (peid, tcid, reg_name, &post_value) != GCSIF_SUCCESS) {
        return GCSIF_ERR_INVALID_REGISTER_NAME;
    }

    // Compare with previous value. If necessary, update register value of FROG I/F.
    if (post_value != prev_value) {
        // Register is updated
        gcs_internal_update_register<is_call_cb> (tcid, reg_name, post_value);
    }

    return GCSIF_SUCCESS;
}


/**
 *   This function update referenced register value when related register is updated.
 *   @note No support mirror register which includes shread register between NM and TC0.
 *   This is also internal function.
 *
 * @param[in] peid     peid for thread context
 * @param[in] tcid     thread context which has the register
 * @param[in] reg_name register name
 * @return Error code
 *         GCSIF_SUCCESS            : Successful
 */
static int gcs_internal_update_referenced_register (uint32_t peid, int32_t tcid, const std::string &reg_name)
{
    /* Not reference registers */

    // ISPR, PMR, FPIPR, FPEC -> ICSR
    if (   (reg_name.compare ("ISPR") == 0)
        || (reg_name.compare ("PMR") == 0)
        || (reg_name.compare ("FPIPR") == 0)
        || (reg_name.compare ("FPEC") == 0)) {
        // Check ICSR change (call CB)
        gcs_internal_compare_and_update_register_frogif<ICT_CALL_CB> (peid, tcid, "ICSR");
    }

    // MCC -> MCR
    if (reg_name.compare ("MCC") == 0) {
        // Check MCR change (call CB)
        gcs_internal_compare_and_update_register_frogif<ICT_CALL_CB> (peid, tcid, "MCR");
    }

    // INTCFG.EPL -> PSW.EIMASK
    if (reg_name.compare ("INTCFG") == 0) {
        // Check ICSR change (call CB)
        gcs_internal_compare_and_update_register_frogif<ICT_CALL_CB> (peid, tcid, "PSW");
    }

    /* Reference registers */

    // FPSR, FPCFG, FPST, FPCC
    if (   (reg_name.compare ("FPSR") == 0)
        || (reg_name.compare ("FPCFG") == 0)
        || (reg_name.compare ("FPCC") == 0)
        || (reg_name.compare ("FPST") == 0)) {
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "FPSR");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "FPCFG");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "FPCC");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "FPST");
    }

    // FXSR, FXCFG, FXST
    if (   (reg_name.compare ("FXSR") == 0)
        || (reg_name.compare ("FXCFG") == 0)
        || (reg_name.compare ("FXST") == 0)) {
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "FXSR");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "FXCFG");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "FXST");
    }

    // DIR1.CSL to BPC, BPAV, BPAM, BPDV, BPDM
    if (reg_name.compare ("DIR1") == 0) {
        FrogRegData dir1_value = 0;
        if (gcs_internal_get_register (peid, tcid, "DIR1", &dir1_value) == GCSIF_SUCCESS) {
            uint32_t csl = (dir1_value >> 4) & 0xF;
            std::ostringstream stream;
            stream << csl;
            std::string csl_str = stream.str();
            gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "BPC" + csl_str);
            gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "BPAV" + csl_str);
            gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "BPAM" + csl_str);
            gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "BPDV" + csl_str);
            gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "BPDM" + csl_str);
        }
    }

    // BPC to BPCn
    if (reg_name.compare ("BPC") == 0) {
        FrogRegData dir1_value = 0;
        if (gcs_internal_get_register (peid, tcid, "DIR1", &dir1_value) == GCSIF_SUCCESS) {
            uint32_t csl = (dir1_value >> 4) & 0xF;
            std::ostringstream stream;
            stream << csl;
            std::string csl_str = stream.str();
            gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "BPC" + csl_str);
        }
    }

    // BPCn to BPC
    if ((reg_name.compare (0, 3, "BPC", 0, 3) == 0) && (reg_name.compare ("BPC") != 0)) {
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "BPC");
    }

    // BPAV to BPAVn
    if (reg_name.compare ("BPAV") == 0) {
        FrogRegData dir1_value = 0;
        if (gcs_internal_get_register (peid, tcid, "DIR1", &dir1_value) == GCSIF_SUCCESS) {
            uint32_t csl = (dir1_value >> 4) & 0xF;
            std::ostringstream stream;
            stream << csl;
            std::string csl_str = stream.str();
            gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "BPAV" + csl_str);
        }
    }

    // BPAM to BPAMn
    if (reg_name.compare ("BPAM") == 0) {
        FrogRegData dir1_value = 0;
        if (gcs_internal_get_register (peid, tcid, "DIR1", &dir1_value) == GCSIF_SUCCESS) {
            uint32_t csl = (dir1_value >> 4) & 0xF;
            std::ostringstream stream;
            stream << csl;
            std::string csl_str = stream.str();
            gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "BPAM" + csl_str);
        }
    }

    // BPDV to BPDVn
    if (reg_name.compare ("BPDV") == 0) {
        FrogRegData dir1_value = 0;
        if (gcs_internal_get_register (peid, tcid, "DIR1", &dir1_value) == GCSIF_SUCCESS) {
            uint32_t csl = (dir1_value >> 4) & 0xF;
            std::ostringstream stream;
            stream << csl;
            std::string csl_str = stream.str();
            gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "BPDV" + csl_str);
        }
    }

    // BPDM to BPDMn
    if (reg_name.compare ("BPDM") == 0) {
        FrogRegData dir1_value = 0;
        if (gcs_internal_get_register (peid, tcid, "DIR1", &dir1_value) == GCSIF_SUCCESS) {
            uint32_t csl = (dir1_value >> 4) & 0xF;
            std::ostringstream stream;
            stream << csl;
            std::string csl_str = stream.str();
            gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "BPDM" + csl_str);
        }
    }

    // MPIDX,MPBK to MPLA,MPUA,MPLA,MPLAI,MPUAI,MPATI
    if (   (reg_name.compare ("MPIDX") == 0)
        || (reg_name.compare ("MPBK") == 0)) {
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPATI");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPLAI");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPUAI");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPAT");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPLA");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPUA");
    }

    // MPRC to MPATn, MPATI
    if (reg_name.compare ("MPRC") == 0) {
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPATI");
        for (uint32_t ch_value = 0; ch_value < 24; ch_value++) {
            std::ostringstream stream;
            stream << ch_value;
            std::string ch_str = stream.str();
            gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPAT" + ch_str);
        }
    }

    // MPATn, MPATI to MPRC
    if (   (reg_name.compare (0, 4, "MPAT", 0, 4) == 0)
        || (reg_name.compare ("MPATI") == 0)) {
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPRC");
    }

    // MPATI/MPAT to MPATn
    if ((reg_name.compare ("MPATI") == 0) || (reg_name.compare ("MPAT") == 0)) {
        FrogRegData mpidx = 0;
        if (gcs_internal_get_register (peid, tcid, "MPIDX", &mpidx) == GCSIF_SUCCESS) {
            uint32_t index = mpidx & 0x1F;
            std::ostringstream stream;
            stream << index;
            std::string index_str = stream.str();
            gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPAT" + index_str);
            gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPAT" + index_str + "_B1");
        }
    }

    // MPLAI/MPLA to MPLAn
    if ((reg_name.compare ("MPLAI") == 0) || (reg_name.compare ("MPLA") == 0)) {
        FrogRegData mpidx = 0;
        if (gcs_internal_get_register (peid, tcid, "MPIDX", &mpidx) == GCSIF_SUCCESS) {
            uint32_t index = mpidx & 0x1F;
            std::ostringstream stream;
            stream << index;
            std::string index_str = stream.str();
            gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPLA" + index_str);
            gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPLA" + index_str + "_B1");
        }
    }

    // MPUAI/MPUA to MPUAn
    if ((reg_name.compare ("MPUAI") == 0) || (reg_name.compare ("MPUA") == 0)) {
        FrogRegData mpidx = 0;
        if (gcs_internal_get_register (peid, tcid, "MPIDX", &mpidx) == GCSIF_SUCCESS) {
            uint32_t index = mpidx & 0x1F;
            std::ostringstream stream;
            stream << index;
            std::string index_str = stream.str();
            gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPUA" + index_str);
            gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPUA" + index_str + "_B1");
        }
    }

    // MPATn to MPATI
    if ((reg_name.compare (0, 4, "MPAT", 0, 4) == 0) && ((reg_name.compare ("MPATI") != 0) && (reg_name.compare ("MPAT") != 0))) {
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPATI");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPAT");
    }

    // MPLAn to MPLAI
    if ((reg_name.compare (0, 4, "MPLA", 0, 4) == 0) && ((reg_name.compare ("MPLAI") != 0) && (reg_name.compare ("MPLA") != 0))) {
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPLAI");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPLA");
    }

    // MPUAn to MPUAI
    if ((reg_name.compare (0, 4, "MPUA", 0, 4) == 0) && ((reg_name.compare ("MPUAI") != 0) && (reg_name.compare ("MPUA") != 0))) {
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPUAI");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPUA");
    }

    // GMCFG to GMPSW
    if (reg_name.compare ("GMCFG") == 0) {
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "GMPSW");
    }

    // MPM to GMMPM
    if (reg_name.compare ("MPM") == 0) {
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "GMMPM");
    }

    // DEVDS and DECTRL
    if (   (reg_name.compare ("DEVDS") == 0)
        || (reg_name.compare ("DECTRL") == 0)) {
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "DEVDS");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "DECTRL");
    }

    // HVCFG.HVE / PSWH.GM to all HV/GM registers ?
    // MCR.Hxxx/Gxxx, MPCFG, MPM, PEID
    // GMCFG, PSWH, FEPSWH, EIPSWH, HVSB
    // GMEIPC, GMEIPSW, GMFEPC, GMFEPSW, GMPSW, GMEIIC, GMFEIC, GMSPID, GMSPIDLIST,
    // GMEBASE, GMINTBP, GMINTCFG, GMPLMR, GMMPM, GMMPCFG, GMEIWR, GMFEWR,
    // PMGMCTRL, DBGEN, DBPSWH
    if ((reg_name.compare ("HVCFG") == 0) || (reg_name.compare ("PSWH") == 0)) {
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MCR");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPM");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "MPCFG");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "GMCFG");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "PEID");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "PSWH");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "FEPSWH");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "EIPSWH");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "HVSB");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "GMEIPC");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "GMEIPSW");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "GMFEPC");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "GMFEPSW");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "GMPSW");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "GMEIIC");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "GMFEIC");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "GMSPID");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "GMSPIDLIST");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "GMEBASE");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "GMINTBP");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "GMINTCFG");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "GMPLMR");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "GMMPM");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "GMMPCFG");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "GMEIWR");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "GMFEWR");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "PMGMCTRL");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "DBGEN");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "DBPSWH");
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "DECTRL");
    }

    // INTCFG.EPL to PSW
    if (reg_name.compare ("INTCFG") == 0) {
        gcs_internal_compare_and_update_register_frogif<ICT_NOT_CALL_CB> (peid, tcid, "PSW");
    }

    return GCSIF_SUCCESS;
}


int gcs_req_eiint_pe (uint32_t channel, uint32_t priority, bool eitb, bool is_gm, uint32_t gpid, bool is_bgint, int isNC, unsigned int ptid)
{
    uint32_t context = 1;

    // Simulator is not initialized.
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_INVALID_TCID;
    }

    uint32_t peid = m_forest_util->GetPeLastId();
    int tcid;
    if ((isNC == 1) && (ptid == 0)) {
        tcid = NC;
    } else {
        tcid = ptid;
    }

    if ((!m_forest_util->IsValidPeId(peid)) || (tcid != NC)){
        return GCSIF_ERR_INVALID_TCID;
    }

    uint32_t valid_max_priority;
    // @@ temporal checking (this should be cheked in CForest)
    if (  (m_forest_util->GetCoreType(peid) == CORE_G4MH)
       && (m_forest_util->GetCoreVersion(peid) >= CORE_VER_20) ) {
        valid_max_priority = 63;
    } else {
        valid_max_priority = 15;
    }

    if ((channel > 2047) || (priority > valid_max_priority)) {
        return GCSIF_ERR_INVALID_EVENT;
    }

    // Request EIINT
    IntErrorCode ret = m_forest_util->ReqInterruptToCpu (peid, channel, priority, eitb, is_gm, gpid, is_bgint);

    // Check ICSR change
    gcs_internal_compare_and_update_register_frogif<ICT_CALL_CB> (peid, tcid, "ICSR");

    if (ret == INT_E_CHANNEL) {
        // @@ This is checked above, 512-2047 will come here (unsupported yet.)
        return GCSIF_ERR_INTERNAL; // This should be GCSIF_ERR_INVALID_EVENT.
    } else if (ret == INT_E_NOT_RECEIVED) {
        return GCSIF_WAR_DUPLICATED_EVENT;
    } else if (ret == INT_E_OK) {
        return GCSIF_SUCCESS;
    } else {
        return GCSIF_ERR_INTERNAL;
    }
}


int gcs_req_syserr_pe (uint32_t cause, int isNC, unsigned int ptid)
{
    uint32_t context = 1;

    // Simulator is not initialized.
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_INVALID_TCID;
    }

    uint32_t peid = m_forest_util->GetPeLastId();
    int tcid;
    if ((isNC == 1) && (ptid == 0)) {
        tcid = NC;
    } else {
        tcid = ptid;
    }

    if ((!m_forest_util->IsValidPeId(peid)) || (tcid != NC)){
        return GCSIF_ERR_INVALID_TCID;
    }

    bool is_duplicated = false;
    if (m_forest_util->HaveSyserrReq (peid)) {
        is_duplicated = true;
    }

    IntErrorCode ret = m_forest_util->ReqSyserrToCpu (peid, cause);

    if (ret == INT_E_SYSERR_CODE) {
        return GCSIF_ERR_INVALID_EVENT;
    } else if (ret == INT_E_OK) {
        if (is_duplicated == true) {
            return GCSIF_WAR_DUPLICATED_EVENT;
        } else {
            return GCSIF_SUCCESS;
        }
    } else {
        return GCSIF_ERR_INTERNAL;
    }
}


// Unused.
int gcs_req_syserr_mode_pe (uint32_t cause, int isNC, unsigned int ptid, bool is_gm, uint32_t gpid)
{
    uint32_t context = 1;

    // Simulator is not initialized.
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_INVALID_TCID;
    }

    uint32_t peid = m_forest_util->GetPeLastId();
    int tcid;
    if ((isNC == 1) && (ptid == 0)) {
        tcid = NC;
    } else {
        tcid = ptid;
    }

    if ((!m_forest_util->IsValidPeId(peid)) || (tcid != NC)){
        return GCSIF_ERR_INVALID_TCID;
    }

    bool is_duplicated = false;
    if (m_forest_util->HaveSyserrReq (peid)) {
        is_duplicated = true;
    }

    IntErrorCode ret = m_forest_util->ReqSyserrToCpu (peid, cause, is_gm, gpid);

    if (ret == INT_E_SYSERR_CODE) {
        return GCSIF_ERR_INVALID_EVENT;
    } else if (ret == INT_E_OK) {
        if (is_duplicated == true) {
            return GCSIF_WAR_DUPLICATED_EVENT;
        } else {
            return GCSIF_SUCCESS;
        }
    } else {
        return GCSIF_ERR_INTERNAL;
    }
}


int gcs_req_rlb_pe (uint32_t source_peid, int isNC, unsigned int ptid)
{
    uint32_t context = 1;

    // Simulator is not initialized.
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_INVALID_TCID;
    }

    uint32_t peid = m_forest_util->GetPeLastId();
    int tcid;
    if ((isNC == 1) && (ptid == 0)) {
        tcid = NC;
    } else {
        tcid = ptid;
    }

    if ((!m_forest_util->IsValidPeId(peid)) || (tcid != NC)){
        return GCSIF_ERR_INVALID_TCID;
    }

    if (source_peid > 7) {
        return GCSIF_ERR_INVALID_TCID;
    }

    // Request RLB
    IntErrorCode ret = m_forest_util->ReqRlbToCpu (peid, source_peid);

    // Check DIR0 change
    gcs_internal_compare_and_update_register_frogif<ICT_CALL_CB> (peid, tcid, "DIR0");

    if (ret == INT_E_OK) {
        return GCSIF_SUCCESS;
    } else {
        return GCSIF_ERR_INTERNAL;
    }
}


int gcs_req_dbnmi_pe (int isNC, unsigned int ptid)
{
    uint32_t context = 1;

    // Simulator is not initialized.
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_INVALID_TCID;
    }

    uint32_t peid = m_forest_util->GetPeLastId();
    int tcid;
    if ((isNC == 1) && (ptid == 0)) {
        tcid = NC;
    } else {
        tcid = ptid;
    }

    if ((!m_forest_util->IsValidPeId(peid)) || (tcid != NC)){
        return GCSIF_ERR_INVALID_TCID;
    }

    if (m_forest_util->HaveDbnmiReq (peid)) {
        return GCSIF_ERR_NOT_RECEIVED;
    }

    IntErrorCode ret = m_forest_util->ReqDbnmiToCpu (peid);

    if (ret == INT_E_NOT_RECEIVED) {
        return GCSIF_ERR_NOT_RECEIVED;
    } else if (ret == INT_E_OK) {
        return GCSIF_SUCCESS;
    } else {
        return GCSIF_ERR_INTERNAL;
    }
}


int gcs_req_dbint_pe (uint32_t channel, int isNC, unsigned int ptid)
{
    uint32_t context = 1;

    // Simulator is not initialized.
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_INVALID_TCID;
    }

    uint32_t peid = m_forest_util->GetPeLastId();
    int tcid;
    if ((isNC == 1) && (ptid == 0)) {
        tcid = NC;
    } else {
        tcid = ptid;
    }

    if ((!m_forest_util->IsValidPeId(peid)) || (tcid != NC)){
        return GCSIF_ERR_INVALID_TCID;
    }

    // @@ Temporal check
    if (channel > 15) {
        return GCSIF_ERR_INVALID_EVENT;
    }

    if (m_forest_util->HaveDbintReq (peid)) {
        return GCSIF_ERR_NOT_RECEIVED;
    }

    IntErrorCode ret = m_forest_util->ReqDbintToCpu (peid, channel);

    if (ret == INT_E_NOT_RECEIVED) {
        return GCSIF_ERR_NOT_RECEIVED;
    } else if (ret == INT_E_OK) {
        return GCSIF_SUCCESS;
    } else {
        return GCSIF_ERR_INTERNAL;
    }
}


int gcs_req_rmint_pe (int isNC, unsigned int ptid)
{
    uint32_t context = 1;

    // Simulator is not initialized.
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_INVALID_TCID;
    }

    uint32_t peid = m_forest_util->GetPeLastId();
    int tcid;
    if ((isNC == 1) && (ptid == 0)) {
        tcid = NC;
    } else {
        tcid = ptid;
    }

    if ((!m_forest_util->IsValidPeId(peid)) || (tcid != NC)){
        return GCSIF_ERR_INVALID_TCID;
    }

    if (m_forest_util->HaveRmintReq (peid)) {
        return GCSIF_ERR_NOT_RECEIVED;
    }

    IntErrorCode ret = m_forest_util->ReqRmintToCpu (peid);

    if (ret == INT_E_NOT_RECEIVED) {
        return GCSIF_ERR_NOT_RECEIVED;
    } else if (ret == INT_E_OK) {
        return GCSIF_SUCCESS;
    } else {
        return GCSIF_ERR_INTERNAL;
    }
}


int gcs_req_fenmi_pe (int isNC, unsigned int ptid)
{
    uint32_t context = 1;

    // Simulator is not initialized.
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_INVALID_TCID;
    }

    uint32_t peid = m_forest_util->GetPeLastId();
    int tcid;
    if ((isNC == 1) && (ptid == 0)) {
        tcid = NC;
    } else {
        tcid = ptid;
    }

    if ((!m_forest_util->IsValidPeId(peid)) || (tcid != NC)){
        return GCSIF_ERR_INVALID_TCID;
    }

    if (m_forest_util->HaveFenmiReq (peid)) {
        return GCSIF_ERR_NOT_RECEIVED;
    }

    IntErrorCode ret = m_forest_util->ReqFenmiToCpu (peid);

    if (ret == INT_E_NOT_RECEIVED) {
        return GCSIF_ERR_NOT_RECEIVED;
    } else if (ret == INT_E_OK) {
        return GCSIF_SUCCESS;
    } else {
        return GCSIF_ERR_INTERNAL;
    }
}


int gcs_req_feint_pe (uint32_t channel, bool is_gm, uint32_t gpid, bool is_bgint, int isNC, unsigned int ptid)
{
    uint32_t context = 1;

    // Simulator is not initialized.
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_INVALID_TCID;
    }

    uint32_t peid = m_forest_util->GetPeLastId();
    int tcid;
    if ((isNC == 1) && (ptid == 0)) {
        tcid = NC;
    } else {
        tcid = ptid;
    }

    if ((!m_forest_util->IsValidPeId(peid)) || (tcid != NC)){
        return GCSIF_ERR_INVALID_TCID;
    }

    // @@ Temporal check
    if (channel > 15) {
        return GCSIF_ERR_INVALID_EVENT;
    }

    if (m_forest_util->HaveFeintReq (peid)) {
        return GCSIF_ERR_NOT_RECEIVED;
    }

    IntErrorCode ret = m_forest_util->ReqFeintToCpu (peid, channel, is_gm, gpid, is_bgint);

    if (ret == INT_E_NOT_RECEIVED) {
        return GCSIF_ERR_NOT_RECEIVED;
    } else if (ret == INT_E_OK) {
        return GCSIF_SUCCESS;
    } else {
        return GCSIF_ERR_INTERNAL;
    }
}


int gcs_req_cpu_stop_pe(int isNC, unsigned int ptid)
{
    uint32_t context = 1;

    // Simulator is not initialized.
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_INVALID_TCID;
    }

    uint32_t peid = m_forest_util->GetPeLastId();
    int tcid;
    if ((isNC == 1) && (ptid == 0)) {
        tcid = NC;
    } else {
        tcid = ptid;
    }

    if ((!m_forest_util->IsValidPeId(peid)) || (tcid != NC)){
        return GCSIF_ERR_INVALID_TCID;
    }

    IntErrorCode ret = m_forest_util->ReqAllInterruptDisableToCpu (peid);

    if (ret == INT_E_NOT_RECEIVED) {
        return GCSIF_ERR_NOT_RECEIVED;
    } else if (ret == INT_E_OK) {
        return GCSIF_SUCCESS;
    } else {
        // Expected: Unreachable code;
        return GCSIF_ERR_INTERNAL;
    }
}


int gcs_reset_pe(unsigned int peid, int isNC, unsigned int ptid)
{
    uint32_t context = 1;

    // Simulator is not initialized.
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_INVALID_TCID;
    }

    int tcid;
    if ((isNC == 1) && (ptid == 0)) {
        tcid = NC;
    } else {
        tcid = ptid;
    }

    if ((!m_forest_util->IsValidPeId(peid)) || (tcid != NC)){
        return GCSIF_ERR_INVALID_TCID;
    }

    if (m_forest_util->FsPeReset (peid) == false) {
        return GCSIF_ERR_NOT_RECEIVED;
    } else {
        return GCSIF_SUCCESS;
    }
}


int gcs_clear_req_pe (const std::string &event, int isNC, unsigned int ptid)
{
    uint32_t context = 1;

    // Simulator is not initialized.
    if (gcs_is_simulator_native(&context) == GCSIF_ERR_NOT_INITIALIZED) {
        return GCSIF_ERR_INVALID_TCID;
    }

    uint32_t peid = m_forest_util->GetPeLastId();
    int tcid;
    if ((isNC == 1) && (ptid == 0)) {
        tcid = NC;
    } else {
        tcid = ptid;
    }

    if ((!m_forest_util->IsValidPeId(peid)) || (tcid != NC)){
        return GCSIF_ERR_INVALID_TCID;
    }

    std::string event_name; //(boost::to_upper_copy<std::string>(event));
    ToUpperCopy (event, &event_name);
    if (event_name == "FEINT") {
        m_forest_util->CancelFeintToCpu (peid);
    } else if (event_name == "FENMI") {
        m_forest_util->CancelFenmiToCpu (peid);
    } else if ((event_name == "EIINT") || (event_name == "EITBL")) {
        m_forest_util->CancelInterruptToCpu (peid);

        // Check ICSR change
        gcs_internal_compare_and_update_register_frogif<ICT_CALL_CB> (peid, tcid, "ICSR");
    } else if (event_name == "DBINT") {
        m_forest_util->CancelDbintToCpu (peid);
    } else if (event_name == "SYSERR") {
        m_forest_util->CancelSyserrToCpu (peid);
    } else if (event_name == "RMINT") {
        m_forest_util->CancelRmintToCpu (peid);
    } else if (event_name == "DBNMI") {
        m_forest_util->CancelDbnmiToCpu (peid);
    } else if (event_name == "RLB") {
        // Because RLB req isn't hold (RLB is oneshot signal), nothing to do.
    } else if (event_name == "STOPREQ") {
        m_forest_util->CancelAllInterruptDisableToCpu (peid);
    } else {
        return GCSIF_ERR_INVALID_OTHER_PARAM; // unknown event
    }
    return GCSIF_SUCCESS;
}


bool gcs_internal_LoadSrec (std::string filename)
{
    m_forest_util->LoadSrec (filename.c_str(), 0);
    return true;
}
