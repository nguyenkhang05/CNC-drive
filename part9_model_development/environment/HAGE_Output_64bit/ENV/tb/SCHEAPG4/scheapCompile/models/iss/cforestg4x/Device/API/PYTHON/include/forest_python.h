/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once

#include <string>
#include <vector>
#include <utility>

#include "./forest_common_api.h"

class ForestUtil;
class ForestCmdOpt;
class ForestSetup;
class Uart;

class PythonAPI {
private:
    static const uint32_t MAX_PE = PEID_MASK;
    static const uint32_t MAX_CL = CLID_MASK;
    static const uint32_t MAX_VM = 0;
    static const uint32_t MAX_HT = 0;

    static bool m_is_gen_pe;

#if defined(COMP_RUNNER)
    static unsigned int m_cr_sampl_period;
    static unsigned int m_cr_rank_lines;
#endif // defined(COMP_RUNNER)

public:
    static ForestUtil* m_forest_util;
    static PyMethodDef m_cforest_api_def[];

    static bool m_print_pythonif;

    static ForestCmdOpt* m_cmdopt;
    static ForestSetup* m_forest_setup;

    typedef void (*sighandler_t)(int);

    static bool m_is_quited;
    static bool m_is_step_any;

    static uint32_t m_pe_to_pe_latency;

    static uint32_t m_lram_fetch_latency;
    static uint32_t m_rom_fetch_latency;

    static PhysAddr m_rom_start_addr;
    static PhysAddr m_rom_end_addr;

    static bool m_is_use_new_ms;

    static const int PY_SEL_ALLCPU = 0x7FFF0000;
    static const uint32_t PY_INVALID_DATA = 0x00000000;
    static const uint32_t PY_OK = 0;
    static const int32_t PY_NG = -1;
    static const uint32_t PC_BREAK_MASK = 0xFFFFFFFF;
    static const uint32_t ACC_BREAK_MASK = 0xFFFFFFFF;

    static const int PY_NOSEL_ID = -2;

    enum CacheSize {
        CACHE_SIZE_0   = 0,
        CACHE_SIZE_1K  = 1,
        CACHE_SIZE_2K  = 2,
        CACHE_SIZE_4K  = 4,
        CACHE_SIZE_8K  = 8,
        CACHE_SIZE_16K = 16,
        CACHE_SIZE_32K = 32,
        CACHE_SIZE_64K = 64
    };

    enum CacheWay {
        CACHE_WAY_0 = 0,
        CACHE_WAY_1 = 1,
        CACHE_WAY_2 = 2,
        CACHE_WAY_4 = 4,
        CACHE_WAY_8 = 8
    };

    enum CacheECC {
        ECC_NO_EXIST = 0,
        ECC_EXIST    = 1
    };

    class EventInfo {
    public:
        PhysAddr target_pc;
        int32_t  scid;
        uint32_t peid;
        uint32_t tcid;
        uint32_t channel;
        uint32_t priority;
        uint32_t gpid;
        uint32_t repeat;
        uint32_t repeated;
        std::string event;
        bool     is_set;

        bool IsHit (PhysAddr addr, int32_t id) const
        {
            return (addr == target_pc) && (id == scid) && (IsExpired () == false);
        }
        void Increment (void) { ++ repeated;}
        bool IsExpired (void) const { return (repeat != 0) && (repeat <= repeated); }
    };
    static std::vector<EventInfo*> m_event_info;

    // Reserved info by SetPeInfoPy/InstantiatePePy
    struct PEInfo {
        uint32_t    peid;
        uint32_t    clid;
        CoreType    core_type;
        CoreVersion core_version;
    };
    static std::vector<uint32_t> m_peid_vec;
    static std::vector<PEInfo> m_pe_info;
    static bool IsGenPe (void) { return m_is_gen_pe; }
    static void SetGenPe (bool is_gen) { m_is_gen_pe = is_gen; }

    typedef std::pair<int32_t, uint32_t> BreakPcInfo; // scid, addr
    static std::vector <BreakPcInfo> m_break_pc_vec;
    static BreakPcInfo m_last_break;
    static BreakEvent m_last_break_event;

    typedef bool IsBreakType;
    static const IsBreakType HIT_BREAK  = true;
    static const IsBreakType NO_BREAK   = false;
    static IsBreakType TryStepExec (void);

    static bool ClearPcBreak (int32_t scid, uint32_t addr);
    static bool SetPcBreak (int32_t scid, uint32_t addr);
    static void UpdateBreakInfo (int32_t break_scid);

#if defined(COMP_RUNNER_G3MH)
    static void StartPy (ForestUtil* forest_util_p, ForestCmdOpt* cmdopt, ForestSetup* forest_setup,
                         uint32_t l1mirr_mode, uint32_t pe1_peid, uint32_t pe2_peid,
                         int argc, char* argv[]);
    static void LoadCForestCommandDefPy (uint32_t l1mirr_mode, uint32_t pe1_peid, uint32_t pe2_peid);
#elif defined(COMP_RUNNER_G3KH)
    static void StartPy (ForestUtil* forest_util_p, ForestCmdOpt* cmdopt, ForestSetup* forest_setup,
                         uint32_t l1mirr_mode, uint32_t maaen_pin,
                         int argc, char* argv[]);
    static void LoadCForestCommandDefPy (uint32_t l1mirr_mode, uint32_t maaen_pin);
#else // defined(COMP_RUNNER)
    static void StartPy (ForestUtil* forest_util_p, ForestCmdOpt* cmdopt, ForestSetup* forest_setup,
                         int argc, char* argv[]);
    static void LoadCForestCommandDefPy (void);
#endif // defined(COMP_RUNNER)
    static void LoadReadlinePy (void);
    static bool IsScidValid (int32_t scid);
    static bool IsPeidValid (int peid);
    static bool IsCacheSizeValid (uint32_t size, uint32_t way);
    static bool IsPEInfoValid (uint32_t peid, uint32_t clid, CoreType core_type);
    static bool IsCoreTypeCombinationValid (CoreType core_type);

    static void MemDisplay (PhysAddr start_addr, PhysAddr end_addr,
                            uint32_t peid, ContextId tcid, FILE* fp, bool is_phys);

    static PyObject* SetGuardParamPy (PyObject* self, PyObject* args);
    static PyObject* IsEnabledPrintPy (PyObject* self, PyObject* args);
    static PyObject* AddBreakAccessPy (PyObject* self, PyObject* args);
    static PyObject* DisplayBreakAccessPy (PyObject* self, PyObject* args);
    static PyObject* ClearBreakAccessPy (PyObject* self, PyObject* args);
    static PyObject* AddBreakPcPy (PyObject* self, PyObject* args);
    static PyObject* DisplayBreakPcPy (PyObject* self, PyObject* args);
    static PyObject* ClearBreakPcPy (PyObject* self, PyObject* args);
    static PyObject* GetLoadNameCmdoptPy (PyObject* self, PyObject* args);
    static PyObject* LoadSrecPy (PyObject* self, PyObject* args);
    static PyObject* ContExecutePy (PyObject* self, PyObject* args);
    static PyObject* SetBreakMaxPy (PyObject* self, PyObject* args);
    static PyObject* GetBreakMaxPy (PyObject* self, PyObject* args);
    static PyObject* SetBreakSamePcMaxPy (PyObject* self, PyObject* args);
    static PyObject* GetBreakSamePcMaxPy (PyObject* self, PyObject* args);
    static PyObject* SetBreakSystimePy (PyObject* self, PyObject* args);
    static PyObject* GetBreakSystimePy (PyObject* self, PyObject* args);
    static PyObject* MemResourceDisplayPy (PyObject* self, PyObject* args);
    static PyObject* MemDisplayPy (PyObject* self, PyObject* args);
    static PyObject* MemReadPy (PyObject* self, PyObject* args);
    static PyObject* MemEditPy (PyObject* self, PyObject* args);
    static PyObject* MemSetPy (PyObject* self, PyObject* args);
    static PyObject* MemSetPePy (PyObject* self, PyObject* args);
    static PyObject* MemSetClPy (PyObject* self, PyObject* args);
    static PyObject* MemClearPy (PyObject* self, PyObject* args);
    static PyObject* SelfSetPy (PyObject* self, PyObject* args);
    static PyObject* SelfClearPy (PyObject* self, PyObject* args);
    static PyObject* MirrorSetPy (PyObject* self, PyObject* args);
    static PyObject* MirrorClearPy (PyObject* self, PyObject* args);
    static PyObject* ErrorAreaSetPy (PyObject* self, PyObject* args);
    static PyObject* ErrorAreaClearPy (PyObject* self, PyObject* args);
#ifdef CLUSTER
    static PyObject* SetClInfoPy (PyObject* self, PyObject* args);
    static PyObject* MemSetClPy (PyObject* self, PyObject* args);
#endif // CLUSTER
    static PyObject* SetPeripheralLatencyPy (PyObject* self, PyObject* args);
    static PyObject* SetPeLatency (PyObject* self, PyObject* args);
    static PyObject* GetPeLatency (PyObject* self, PyObject* args);
    static PyObject* SetLramFetchLatency (PyObject* self, PyObject* args);
    static PyObject* GetLramFetchLatency (PyObject* self, PyObject* args);
    static PyObject* SetRomFetchLatency (PyObject* self, PyObject* args);
    static PyObject* GetRomFetchLatency (PyObject* self, PyObject* args);
    static PyObject* QuitPy (PyObject* self, PyObject* args);
    static PyObject* HardResetPy (PyObject* self, PyObject* args);
    static PyObject* ResetPePy (PyObject* self, PyObject* args);
    /**
     * \fn static bool ResetPeIf (uint32_t peid);
     * \brief The Interface function for requesting individual PE reset.
     *
     * This function is used by python command reset(self, *contextid)
     *                     and set_event (address, "pereset", context)
     *
     * @param[in] peid               The source will specify which PE will go to RESET state.
     * @return boolean value
     * @retval true                : The PE was reset completely. Last PC was updated to FEPC.
     * @retval false               : The PE can't reset completely.
     */
    static bool ResetPeIf (uint32_t peid);
    static PyObject* StepExecutePy (PyObject* self, PyObject* args);
    static PyObject* SwitchStepModePy (PyObject* self, PyObject* args);
    static PyObject* GetStepModePy (PyObject* self, PyObject* args);
    static PyObject* RegReadDebugPy (PyObject* self, PyObject* args);
    static PyObject* RegWriteDebugPy (PyObject* self, PyObject* args);
    static PyObject* DisplayThreadInfoPy (PyObject* self, PyObject* args);
    static PyObject* ShowRegPy (PyObject* self, PyObject* args);
    static PyObject* PrintHelpPy (PyObject* self, PyObject* args);
    static PyObject* DisplayInstructionCountPy (PyObject* self, PyObject* args);
    static PyObject* ClearInstructionCountPy (PyObject* self, PyObject* args);
    static PyObject* SetDebugModePy (PyObject* self, PyObject* args);
    static PyObject* SetLogFilePy (PyObject* self, PyObject* args);
    static PyObject* UpdateExpMsgEnablePy (PyObject* self, PyObject* args);
    static PyObject* ConnectGDBPy (PyObject* self, PyObject* args);
    static PyObject* ConnectUARTPy (PyObject* self, PyObject* args);
    static PyObject* GetLastTcIdPy (PyObject* self, PyObject* args);
    static PyObject* GetLastPeIdPy (PyObject* self, PyObject* args);
    static PyObject* SetFrequencyPy (PyObject* self, PyObject* args);
    static PyObject* GetFrequencyPy (PyObject* self, PyObject* args);
    static PyObject* SetSnoozePeriodPy (PyObject* self, PyObject* args);
    static PyObject* GetSnoozePeriodPy (PyObject* self, PyObject* args);
    static PyObject* SetTlbEntryPy (PyObject* self, PyObject* args);
    static PyObject* GetTlbEntryPy (PyObject* self, PyObject* args);
    static PyObject* SetMinTlbSizePy (PyObject* self, PyObject* args);
    static PyObject* GetMinTlbSizePy (PyObject* self, PyObject* args);
    static PyObject* SetMpuDefaultPy (PyObject* self, PyObject* args);
    static PyObject* GetMpuDefaultPy (PyObject* self, PyObject* args);
    static PyObject* SetMpuRegionNumPy (PyObject* self, PyObject* args);
    static PyObject* GetMpuRegionNumPy (PyObject* self, PyObject* args);
    static PyObject* SetMpuBankNumPy (PyObject* self, PyObject* args);
    static PyObject* GetMpuBankNumPy (PyObject* self, PyObject* args);
    static PyObject* SetMpuDmdpPy (PyObject* self, PyObject* args);
    static PyObject* GetMpuDmdpPy (PyObject* self, PyObject* args);
    static PyObject* SetPeidPy (PyObject* self, PyObject* args);
    static PyObject* GetPeidPy (PyObject* self, PyObject* args);
    static PyObject* SetSpidPy (PyObject* self, PyObject* args);
    static PyObject* GetSpidPy (PyObject* self, PyObject* args);
    static PyObject* SetSpidlistPy (PyObject* self, PyObject* args);
    static PyObject* GetSpidlistPy (PyObject* self, PyObject* args);
    static PyObject* SetBmidPy (PyObject* self, PyObject* args);
    static PyObject* GetBmidPy (PyObject* self, PyObject* args);
    static PyObject* SetRbasePy (PyObject* self, PyObject* args);
    static PyObject* GetRbasePy (PyObject* self, PyObject* args);
    static PyObject* SetMctlPy (PyObject* self, PyObject* args);
    static PyObject* GetMctlPy (PyObject* self, PyObject* args);
    static PyObject* SetIcctrlPy (PyObject* self, PyObject* args);
    static PyObject* GetIcctrlPy (PyObject* self, PyObject* args);
    static PyObject* SetIcSizePy (PyObject* self, PyObject* args);
    static PyObject* GetIcSizePy (PyObject* self, PyObject* args);
    static PyObject* SetDcctrlPy (PyObject* self, PyObject* args);
    static PyObject* GetDcctrlPy (PyObject* self, PyObject* args);
    static PyObject* SetDcSizePy (PyObject* self, PyObject* args);
    static PyObject* GetDcSizePy (PyObject* self, PyObject* args);
    static PyObject* SetPeInfoPy (PyObject* self, PyObject* args);
    static PyObject* GeneratePePy (PyObject* self, PyObject* args);
    static PyObject* IsGenPePy (PyObject* self, PyObject* args);
    static PyObject* IsValidPeIdPy (PyObject* self, PyObject* args);
    static PyObject* SetRomAreaPy (PyObject* self, PyObject* args);
    static PyObject* SetLocalRamAreaPy (PyObject* self, PyObject* args);
    static PyObject* Intc1SetPy (PyObject* self, PyObject* args);
    static PyObject* Intc2SetPy (PyObject* self, PyObject* args);
    static PyObject* ReqEIINTfromexternalPy (PyObject* self, PyObject* args);
    static PyObject* ReqFEINTfromexternalPy (PyObject* self, PyObject* args);
    static PyObject* OstmLocalSetPy (PyObject* self, PyObject* args);
    static PyObject* OstmGlobalSetPy (PyObject* self, PyObject* args);
    static PyObject* SetOstmClkPy (PyObject* self, PyObject* args);
    static PyObject* GetOstmClkPy (PyObject* self, PyObject* args);
    static PyObject* SetDebugAreaPy (PyObject* self, PyObject* args);
    static PyObject* ClearDebugAreaPy (PyObject* self, PyObject* args);
    static PyObject* SetDebugOptionPy (PyObject* self, PyObject* args);
    static PyObject* GetDebugOptionPy (PyObject* self, PyObject* args);
    static PyObject* GetArchClassPy (PyObject* self, PyObject* args);
    static PyObject* SetMsgLvlPy (PyObject* self, PyObject* args);
    static PyObject* GetMsgLvlPy (PyObject* self, PyObject* args);
    static PyObject* UseStopIOPy (PyObject* self, PyObject* args);
    static PyObject* SetFpuExistencePy (PyObject* self, PyObject* args);
    static PyObject* GetFpuExistencePy (PyObject* self, PyObject* args);
    static PyObject* SetFxuExistencePy (PyObject* self, PyObject* args);
    static PyObject* GetFxuExistencePy (PyObject* self, PyObject* args);
    static PyObject* SetMpuExistencePy (PyObject* self, PyObject* args);
    static PyObject* GetMpuExistencePy (PyObject* self, PyObject* args);
    static PyObject* SetMmuExistencePy (PyObject* self, PyObject* args);
    static PyObject* GetMmuExistencePy (PyObject* self, PyObject* args);
    static PyObject* SetLscfgPy (PyObject* self, PyObject* args);
    static PyObject* SetMemInitPy (PyObject* self, PyObject* args);
    static PyObject* GetMemInitPy (PyObject* self, PyObject* args);
    static PyObject* SetDcAreaPy (PyObject* self, PyObject* args);
    static PyObject* GetDcAreaPy (PyObject* self, PyObject* args);
    static PyObject* DumpBtbStatusPy (PyObject* self, PyObject* args);
    static PyObject* ResetBtbStatusPy (PyObject* self, PyObject* args);
    static PyObject* ShowPerfInfoPy (PyObject* self, PyObject* args);
    static PyObject* ResetPerfInfoPy (PyObject* self, PyObject* args);
    static PyObject* ExecCfaliPy (PyObject* self, PyObject* args);
    static PyObject* SetEventPy (PyObject* self, PyObject* args);
    static PyObject* SetClearEventPy (PyObject* self, PyObject* args);
    static bool ExecEvent (uint32_t break_pc, int32_t break_scid);
    static bool ReqEvent (EventInfo* event);
    static bool ReqClearEvent (EventInfo* event);
    static bool IsValidAsyncEvent (std::string event_name);
#if defined(COMP_RUNNER)
    static PyObject* XDumpFPU (PyObject* self, PyObject* args);
    static PyObject* XDumpREG (PyObject* self, PyObject* args);
    static PyObject* XDumpFLAG (PyObject* self, PyObject* args);
    static PyObject* XDumpDATA (PyObject* self, PyObject* args);
    static PyObject* XDumpSYSREG (PyObject* self, PyObject* args);
    static PyObject* SetCmpStage (PyObject* self, PyObject* args);
    static PyObject* SetCycCmpPC (PyObject* self, PyObject* args);
    static PyObject* SetCompRunnerPy (PyObject* self, PyObject* args);
    static uint32_t GetCompSamplePeriod (void);
    static uint32_t GetCompRankLines (void);
#endif // defined(COMP_RUNNER)
#if defined(COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)
    static PyObject* SetCompMisrShowPy (PyObject* self, PyObject* args);
#endif // defined(COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)
    static PyObject* SetHasFxuPy (PyObject* self, PyObject* args);
    static PyObject* SetHasFpuPy (PyObject* self, PyObject* args);
    static PyObject* SetHasMpuPy (PyObject* self, PyObject* args);
    static PyObject* SetHasMmuPy (PyObject* self, PyObject* args);
    static PyObject* SetPeripheralPy (PyObject* self, PyObject* args);
    static PyObject* SetIpirEiintChannelPy (PyObject* self, PyObject* args);
    static PyObject* SetGuardPy (PyObject* self, PyObject* args);
    static PyObject* SetNumL1RLinkPy (PyObject* self, PyObject* args);
    static PyObject* GetNumL1RLinkPy (PyObject* self, PyObject* args);
    static PyObject* SetSizeL1RPy (PyObject* self, PyObject* args);
    static PyObject* GetSizeL1RPy (PyObject* self, PyObject* args);
    static PyObject* SetHasHvPy (PyObject* self, PyObject* args);
    static PyObject* GetHasHvPy (PyObject* self, PyObject* args);
    static PyObject* SetHvcfgPy (PyObject* self, PyObject* args);
    static PyObject* GetHvcfgPy (PyObject* self, PyObject* args);
    static PyObject* SetDbgenPy (PyObject* self, PyObject* args);
    static PyObject* GetDbgenPy (PyObject* self, PyObject* args);
    static void ExitHandler (int exit_status);
    static void InitPy (void);
};
