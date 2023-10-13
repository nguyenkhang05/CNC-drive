/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

// Python.hは内部でinttypes.hをincludeする。
// PRIu64表示のために"__STDC_FORMAT_MACROS"をinttypes.hの前にdefineする必要がある。
#define __STDC_FORMAT_MACROS
// to prevent to use pythonxx_d.lib in Python.h(pyconfig.h)
#if defined(_DEBUG)
/* Use debug wrappers with the Python release dll */
# undef _DEBUG
# include <Python.h>
# define _DEBUG
#else // #if defined(_DEBUG)
#include <Python.h>
#endif // #if defined(_DEBUG)
#include <signal.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>
#include "./config.h"
#include "./forest_common.h"
#include "./forest_utility.h"
#include "./forest_cmdopt.h"
#include "./forest_setup.h"
#include "./forest_message.h"
#if defined(COMP_RUNNER)
#include "./xidump.h"
#endif // defined(COMP_RUNNER)
#ifdef ENABLE_PYTHON
#include "./forest_python.h"
#endif // ENABLE_PYTHON
#ifdef ENABLE_GDB
#include "./forest_gdb_stub.h"
#endif // ENABLE_GDB

#ifdef ENABLE_UART
#include "./intc2.h"
#include "./uart.h"
#include "./fsbus.h"
#endif // ENABLE_UART
#include "./sideband.h"
#include "./lsu.h"
#include "./inst_id_list.h"

// Py_から始まる関数はPythonライブラリの関数

ForestUtil* PythonAPI::m_forest_util;
ForestCmdOpt* PythonAPI::m_cmdopt;
ForestSetup* PythonAPI::m_forest_setup;

std::vector <PythonAPI::BreakPcInfo> PythonAPI::m_break_pc_vec;
PythonAPI::BreakPcInfo PythonAPI::m_last_break;
BreakEvent PythonAPI::m_last_break_event = BREAK_NONE;

std::vector<PythonAPI::EventInfo*> PythonAPI::m_event_info;

std::vector<uint32_t> PythonAPI::m_peid_vec;
std::vector<PythonAPI::PEInfo> PythonAPI::m_pe_info;
bool PythonAPI::m_is_gen_pe = false;

uint32_t PythonAPI::m_pe_to_pe_latency = 0;

bool PythonAPI::m_is_quited = false;
bool PythonAPI::m_is_step_any = false;
bool PythonAPI::m_print_pythonif;

bool PythonAPI::m_is_use_new_ms = false;

#if defined(COMP_RUNNER)
unsigned int PythonAPI::m_cr_sampl_period = 100;
unsigned int PythonAPI::m_cr_rank_lines = 10;
#endif // defined(COMP_RUNNER)

#if defined(FROG_API_IF)
static bool g_is_init_py = false;
#endif // #if defined(FROG_API_IF)

#include "./forest.h" // FLASH_ROM_BASE,FLASH_ROM_SIZEの初期値取得用
PhysAddr PythonAPI::m_rom_start_addr = Lsu::ROM_BGN;
PhysAddr PythonAPI::m_rom_end_addr = Lsu::ROM_END;

uint32_t PythonAPI::m_lram_fetch_latency = 8;
uint32_t PythonAPI::m_rom_fetch_latency = 1;


// PythonからC関数を呼び出すためのAPI定義テーブル
PyMethodDef PythonAPI::m_cforest_api_def[] = {
    {"AddBreakAccessPy", AddBreakAccessPy, METH_VARARGS, "Set Watchpoint"},
    {"DisplayBreakAccessPy", DisplayBreakAccessPy, METH_VARARGS, "Display Watchpoint"},
    {"ClearBreakAccessPy", ClearBreakAccessPy, METH_VARARGS, "Delete Watchpoint"},
    {"AddBreakPcPy", AddBreakPcPy, METH_VARARGS, "Set Breakpoint"},
    {"DisplayBreakPcPy", DisplayBreakPcPy, METH_VARARGS, "Display Breakpoint"},
    {"ClearBreakPcPy", ClearBreakPcPy, METH_VARARGS, "Delete Breakpoint"},
    {"LoadSrecPy", LoadSrecPy, METH_VARARGS, "Load Srecord file"},
    {"ContExecutePy", ContExecutePy, METH_VARARGS, "Run simulation"},
    {"SetBreakMaxPy", SetBreakMaxPy, METH_VARARGS, "Set max instruction break"},
    {"GetBreakMaxPy", GetBreakMaxPy, METH_VARARGS, "Get max instruction break"},
    {"SetBreakSamePcMaxPy", SetBreakSamePcMaxPy, METH_VARARGS, "Set same pc max break"},
    {"GetBreakSamePcMaxPy", GetBreakSamePcMaxPy, METH_VARARGS, "Get same pc max break"},
    {"SetBreakSystimePy", SetBreakSystimePy, METH_VARARGS, "Set cycle count break"},
    {"GetBreakSystimePy", GetBreakSystimePy, METH_VARARGS, "Get cycle count break"},
    {"MemResourceDisplayPy", MemResourceDisplayPy, METH_VARARGS, "Display memory resource"},
    {"MemDisplayPy", MemDisplayPy, METH_VARARGS, "Display memory content"},
    {"MemReadPy", MemReadPy, METH_VARARGS, "Read memory content"},
    {"MemEditPy", MemEditPy, METH_VARARGS, "Edit memory content"},
    {"MemSetPy", MemSetPy, METH_VARARGS, "Set memory"},
    {"MemSetPePy", MemSetPePy, METH_VARARGS, "Set pe local or exo-cluster memory"},
    {"MemSetClPy", MemSetClPy, METH_VARARGS, "Set cluster or exo-cluster memory"},
    {"MemClearPy", MemClearPy, METH_VARARGS, "Delete memory"},
    {"SelfSetPy", SelfSetPy, METH_VARARGS, "Set self area"},
    {"SelfClearPy", SelfClearPy, METH_VARARGS, "Delete self area"},
    {"MirrorSetPy", MirrorSetPy, METH_VARARGS, "Set mirror area"},
    {"MirrorClearPy", MirrorClearPy, METH_VARARGS, "Delete mirror area"},
    {"ErrorAreaSetPy", ErrorAreaSetPy, METH_VARARGS, "Set error area"},
    {"ErrorAreaClearPy", ErrorAreaClearPy, METH_VARARGS, "Delete error area"},
    {"SetPeLatency", SetPeLatency, METH_VARARGS, "Set pe to pe latency"},
    {"GetPeLatency", GetPeLatency, METH_VARARGS, "Get pe latency"},
    {"SetLramFetchLatency", SetLramFetchLatency, METH_VARARGS, "Set fetch latency for lram"},
    {"GetLramFetchLatency", GetLramFetchLatency, METH_VARARGS, "Get fetch latency of lram"},
    {"SetRomFetchLatency", SetRomFetchLatency, METH_VARARGS, "Reduction val for rom latency on fetch"},
    {"GetRomFetchLatency", GetRomFetchLatency, METH_VARARGS, "Get val of rom latency on fetch"},
    {"SetPeripheralLatencyPy", SetPeripheralLatencyPy, METH_VARARGS, "Set latency for peripherals"},
    {"QuitPy", QuitPy, METH_VARARGS, "Quit"},
    {"HardResetPy", HardResetPy, METH_VARARGS, "Reset simurator"},
    {"ResetPePy", ResetPePy, METH_VARARGS, "Reset individual PE. Be available from G4MH2.1"},
    {"StepExecutePy", StepExecutePy, METH_VARARGS, "Step execute simulation"},
    {"SwitchStepModePy", SwitchStepModePy, METH_VARARGS, "Switch step mode"},
    {"GetStepModePy", GetStepModePy, METH_VARARGS, "Get step mode"},
    {"RegReadDebugPy", RegReadDebugPy, METH_VARARGS, "Display register content"},
    {"RegWriteDebugPy", RegWriteDebugPy, METH_VARARGS, "Edit register content"},
    {"DisplayThreadInfoPy", DisplayThreadInfoPy, METH_VARARGS, "Display thread information"},
    {"ShowRegPy", ShowRegPy, METH_VARARGS, "Display list of register name"},
    {"PrintHelpPy", PrintHelpPy, METH_VARARGS, "Print help"},
    {"DisplayInstructionCountPy", DisplayInstructionCountPy, METH_VARARGS, "Display inst count"},
    {"ClearInstructionCountPy", ClearInstructionCountPy, METH_VARARGS, "Clear inst count"},
    {"SetDebugModePy", SetDebugModePy, METH_VARARGS, "Set debug mode"},
    {"SetLogFilePy", SetLogFilePy, METH_VARARGS, "Set log file"},
    {"UpdateExpMsgEnablePy", UpdateExpMsgEnablePy, METH_VARARGS, "Update exception message region"},
    {"ConnectGDBPy", ConnectGDBPy, METH_VARARGS, "Connect to GDB"},
    {"ConnectUARTPy", ConnectUARTPy, METH_VARARGS, "Connect to UART"},
    {"GetLastTcIdPy", GetLastTcIdPy, METH_VARARGS, "Get biggest tcid of PE"},
    {"GetLastPeIdPy", GetLastPeIdPy, METH_VARARGS, "Get biggest peid"},
    {"IsValidPeIdPy", IsValidPeIdPy, METH_VARARGS, "Check whether the peid is valid"},
    {"SetFrequencyPy", SetFrequencyPy, METH_VARARGS, "Set clock ratio of PE"},
    {"GetFrequencyPy", GetFrequencyPy, METH_VARARGS, "Get clock ratio of PE"},
    {"SetOstmClkPy", SetOstmClkPy, METH_VARARGS, "Set clock ratio of OSTM"},
    {"GetOstmClkPy", GetOstmClkPy, METH_VARARGS, "Get clock ratio of OSTM"},
    {"SetSnoozePeriodPy", SetSnoozePeriodPy, METH_VARARGS, "Set Snooze period"},
    {"GetSnoozePeriodPy", GetSnoozePeriodPy, METH_VARARGS, "Get Snooze period"},
    {"SetTlbEntryPy", SetTlbEntryPy, METH_VARARGS, "Set num of TLB entry"},
    {"GetTlbEntryPy", GetTlbEntryPy, METH_VARARGS, "Get num of TLB entry"},
    {"SetMinTlbSizePy", SetMinTlbSizePy, METH_VARARGS, "Set the min size of TLB entry"},
    {"GetMinTlbSizePy", GetMinTlbSizePy, METH_VARARGS, "Get the min size of TLB entry"},
    {"SetMpuDefaultPy", SetMpuDefaultPy, METH_VARARGS, "Set MPU default operation"},
    {"GetMpuDefaultPy", GetMpuDefaultPy, METH_VARARGS, "Get MPU default operation"},
    {"SetMpuRegionNumPy", SetMpuRegionNumPy, METH_VARARGS, "Set the num of MPU region"},
    {"GetMpuRegionNumPy", GetMpuRegionNumPy, METH_VARARGS, "Get the num of MPU region"},
    {"SetMpuBankNumPy", SetMpuBankNumPy, METH_VARARGS, "Set the num of MPU bank"},
    {"GetMpuBankNumPy", GetMpuBankNumPy, METH_VARARGS, "Get the num of MPU bank"},
    {"SetPeidPy", SetPeidPy, METH_VARARGS, "Set PEID"},
    {"GetPeidPy", GetPeidPy, METH_VARARGS, "Get PEID"},
    {"SetSpidPy", SetSpidPy, METH_VARARGS, "Set SPID"},
    {"GetSpidPy", GetSpidPy, METH_VARARGS, "Get SPID"},
    {"SetSpidlistPy", SetSpidlistPy, METH_VARARGS, "Set SPIDLIST"},
    {"GetSpidlistPy", GetSpidlistPy, METH_VARARGS, "Get SPIDLIST"},
    {"SetBmidPy", SetBmidPy, METH_VARARGS, "Set BMID"},
    {"GetBmidPy", GetBmidPy, METH_VARARGS, "Get BMID"},
    {"SetRbasePy", SetRbasePy, METH_VARARGS, "Set NM.RBASE for RBASE/DV/RINT"},
    {"GetRbasePy", GetRbasePy, METH_VARARGS, "Get NM.RBASE for RBASE/DV/RINT"},
    {"SetMctlPy", SetMctlPy, METH_VARARGS, "Set NM.MCTL for MA"},
    {"GetMctlPy", GetMctlPy, METH_VARARGS, "Get NM.MCTL for MA"},
    {"SetMpuDmdpPy", SetMpuDmdpPy, METH_VARARGS, "Set DMDP for MPU"},
    {"GetMpuDmdpPy", GetMpuDmdpPy, METH_VARARGS, "Get DMDP for MPU"},
    {"SetIcctrlPy", SetIcctrlPy, METH_VARARGS, "Set NM.ICCTRL for ICHEN"},
    {"GetIcctrlPy", GetIcctrlPy, METH_VARARGS, "Get NM.ICCTRL for ICHEN"},
    {"SetDcctrlPy", SetDcctrlPy, METH_VARARGS, "Set NM.DCCTRL for DCHEN"},
    {"GetDcctrlPy", GetDcctrlPy, METH_VARARGS, "Get NM.DCCTRL for DCHEN"},
    {"SetIcSizePy", SetIcSizePy, METH_VARARGS, "Set IC size"},
    {"GetIcSizePy", GetIcSizePy, METH_VARARGS, "Get IC size"},
    {"SetDcSizePy", SetDcSizePy, METH_VARARGS, "Set DC size"},
    {"GetDcSizePy", GetDcSizePy, METH_VARARGS, "Set DC size"},
    {"SetPeInfoPy", SetPeInfoPy, METH_VARARGS, "Set PE info"},
    {"GeneratePePy", GeneratePePy, METH_VARARGS, "Instantiate PE instance"},
    {"IsGenPePy", IsGenPePy, METH_VARARGS, "Check created PE instance"},
    {"SetRomAreaPy", SetRomAreaPy, METH_VARARGS, "Set ROM area"},
    {"SetLocalRamAreaPy", SetLocalRamAreaPy, METH_VARARGS, "Set LocalRAM area"},
    {"Intc1SetPy", Intc1SetPy, METH_VARARGS, "Set Intc1 in PE"},
    {"Intc2SetPy", Intc2SetPy, METH_VARARGS, "Set Intc2 in Cluster"},
    {"ReqEIINTfromexternalPy", ReqEIINTfromexternalPy, METH_VARARGS, "Req int EIINT from python"},
    {"ReqFEINTfromexternalPy", ReqFEINTfromexternalPy, METH_VARARGS, "Req int FEINT from python"},
    {"OstmGlobalSetPy", OstmGlobalSetPy, METH_VARARGS, "Set ostm"},
    {"OstmLocalSetPy", OstmLocalSetPy, METH_VARARGS, "Set ostm"},
    {"SetDebugAreaPy", SetDebugAreaPy, METH_VARARGS, "Set DEBUG area"},
    {"ClearDebugAreaPy", ClearDebugAreaPy, METH_VARARGS, "Clear DEBUG area"},
    {"SetDebugOptionPy", SetDebugOptionPy, METH_VARARGS, "Set debug option"},
    {"GetDebugOptionPy", GetDebugOptionPy, METH_VARARGS, "Get debug option"},
    {"GetArchClassPy", GetArchClassPy, METH_VARARGS, "Get architecture class"},
    {"SetMsgLvlPy", SetMsgLvlPy, METH_VARARGS, "Set message level"},
    {"GetMsgLvlPy", GetMsgLvlPy, METH_VARARGS, "Get message level"},
    {"UseStopIOPy", UseStopIOPy, METH_VARARGS, "Enable break access at STOPIO"},
    {"SetFxuExistencePy", SetFxuExistencePy, METH_VARARGS, "Set FXU existence"},
    {"GetFxuExistencePy", GetFxuExistencePy, METH_VARARGS, "Get FXU existence"},
    {"SetFpuExistencePy", SetFpuExistencePy, METH_VARARGS, "Set FPU existence"},
    {"GetFpuExistencePy", GetFpuExistencePy, METH_VARARGS, "Get FPU existence"},
    {"SetMpuExistencePy", SetMpuExistencePy, METH_VARARGS, "Set MPU existence"},
    {"GetMpuExistencePy", GetMpuExistencePy, METH_VARARGS, "Get MPU existence"},
    {"SetMmuExistencePy", SetMmuExistencePy, METH_VARARGS, "Set MMU existence"},
    {"GetMmuExistencePy", GetMmuExistencePy, METH_VARARGS, "Get MMU existence"},
    {"SetHasHvPy", SetHasHvPy, METH_VARARGS, "Set HV existence"},
    {"SetHvcfgPy", SetHvcfgPy, METH_VARARGS, "Set HVCFG reset value"},
    {"GetHvcfgPy", GetHvcfgPy, METH_VARARGS, "Get HVCFG reset value"},
    {"SetLscfgPy", SetLscfgPy, METH_VARARGS, "Set Lock-step hardware existence"},
    {"SetDbgenPy", SetDbgenPy, METH_VARARGS, "Set DGBEN reset value"},
    {"GetDbgenPy", GetDbgenPy, METH_VARARGS, "Get DBGEN reset value"},
    {"SetMemInitPy", SetMemInitPy, METH_VARARGS, "Set initial value for memory"},
    {"GetMemInitPy", GetMemInitPy, METH_VARARGS, "Get initial value for memory"},
    {"SetDcAreaPy", SetDcAreaPy, METH_VARARGS, "Set cacheable area for DCache"},
    {"GetDcAreaPy", GetDcAreaPy, METH_VARARGS, "Get cacheable area of DCache"},
    {"DumpBtbStatusPy", DumpBtbStatusPy, METH_VARARGS, "Dump btb statistics"},
    {"ResetBtbStatusPy", ResetBtbStatusPy, METH_VARARGS, "Reset btb statistics"},
    {"ShowPerfInfoPy", ShowPerfInfoPy, METH_VARARGS, "Show performance statistics"},
    {"ResetPerfInfoPy", ResetPerfInfoPy, METH_VARARGS, "Reset performance statistics"},
    {"ExecCfaliPy", ExecCfaliPy, METH_VARARGS, "Execute CFALI inst for the address range"},
    {"SetEventPy", SetEventPy, METH_VARARGS, "Request events"},
    {"SetClearEventPy", SetClearEventPy, METH_VARARGS, "Request clear events"},
#if defined(COMP_RUNNER)
    {"XDumpFPU", XDumpFPU, METH_VARARGS, "Set FPU condition for XDump"},
    {"XDumpREG", XDumpREG, METH_VARARGS, "Set address region for dumping GR/WR write-back"},
    {"XDumpFLAG", XDumpFLAG, METH_VARARGS, "Set address region for dumping flag"},
    {"XDumpDATA", XDumpDATA, METH_VARARGS, "Set address region for XDump"},
    {"XDumpSYSREG", XDumpSYSREG, METH_VARARGS, "Set system register for XDump"},
    {"SetCmpStage", SetCmpStage, METH_VARARGS, "Set stage for dumping cycle"},
    {"SetIpirEiintChannelPy", SetIpirEiintChannelPy, METH_VARARGS, "Set EIINT channel offset "},
    {"SetCycCmpPC", SetCycCmpPC, METH_VARARGS, "Set PC range for comparing cycle"},
    {"SetCompRunnerPy", SetCompRunnerPy, METH_VARARGS, "Set Comp Runner parameter"},
#endif // defined(COMP_RUNNER)
#if defined(COMP_RUNNER_G3MH) || defined(COMP_RUNNER_G4MH)
    {"SetCompMisrShowPy", SetCompMisrShowPy, METH_VARARGS, "Set Comp Runner MISR Show parametor"},
#endif // defined(COMP_RUNNER_G3MH) || defined(COMP_RUNNER_G4MH)
    {"SetHasFxuPy", SetHasFxuPy, METH_VARARGS, "Set has FXU"},
    {"SetHasFpuPy", SetHasFpuPy, METH_VARARGS, "Set has FPU"},
    {"SetHasMpuPy", SetHasMpuPy, METH_VARARGS, "Set has MPU"},
    {"SetHasMmuPy", SetHasMmuPy, METH_VARARGS, "Set has MMU"},
    {"SetNumL1RLinkPy", SetNumL1RLinkPy, METH_VARARGS, "Set Num of L1RAM Link resource"},
    {"GetNumL1RLinkPy", GetNumL1RLinkPy, METH_VARARGS, "Get Num of L1RAM Link resource"},
    {"SetSizeL1RPy", SetSizeL1RPy, METH_VARARGS, "Set Size of L1RAM for L1RCFG.L1RSIZE"},
    {"GetSizeL1RPy", GetSizeL1RPy, METH_VARARGS, "Get Size of L1RAM for L1RCFG.L1RSIZE"},
    {"SetPeripheralPy", SetPeripheralPy, METH_VARARGS, "Set connecting Peripheral IPs"},
    {"SetIpirEiintChannelPy", SetIpirEiintChannelPy, METH_VARARGS, "Set EIINT channel offset "},
    {"SetGuardPy", SetGuardPy, METH_VARARGS, "Set connecting Guard IPs"},
    {"SetGuardParamPy", SetGuardParamPy, METH_VARARGS, "Set parameters for Guard IPs"},
    {"IsEnabledPrintPy", IsEnabledPrintPy, METH_VARARGS, "Enable printing the result value of PythonIF"},
    {"GetLoadNameCmdoptPy", GetLoadNameCmdoptPy, METH_VARARGS, "Get filename which is designated by --hex cmdopt"},
#ifdef CLUSTER
    {"SetClInfoPy", SetClInfoPy, METH_VARARGS, "Set memory access information of Cluster"},
    {"MemSetClPy", MemSetClPy, METH_VARARGS, "Set memory for Cluster Environment"},
#endif // CLUSTER
    {NULL, NULL, 0, NULL}
};


// readlineモジュールの設定
void PythonAPI::LoadReadlinePy (void) {
    // 最終的にコマンドが固まったら、補間リストを実装した方が綺麗
    PyRun_SimpleString(
        "try:\n"
        "    import readline\n"
        "except ImportError:\n"
        // readlineが無い場合は読み込まずにreturn
        "    pass\n"
        "else:\n"
        // readlineがある場合は有効にする
        "    import rlcompleter\n"
        "    readline.parse_and_bind(\"tab: complete\")\n"
    );
}

#include "CommandDef.h"
#include "CommandAlias.h"
#include "elfloader.h"
#include "profiler.h"

// Pythonの初期設定と開始
#if defined (COMP_RUNNER_G3MH)
void PythonAPI::StartPy (ForestUtil* forest_util_p, ForestCmdOpt* cmdopt, ForestSetup* forest_setup,
                         uint32_t l1mirr_mode, uint32_t pe1_peid, uint32_t pe2_peid,
                         int argc, char* argv[])
#elif  defined(COMP_RUNNER_G3KH)
void PythonAPI::StartPy (ForestUtil* forest_util_p, ForestCmdOpt* cmdopt, ForestSetup* forest_setup,
                         uint32_t l1mirr_mode, uint32_t maaen_pin,
                         int argc, char* argv[])
#else  // defined(COMP_RUNNER)
void PythonAPI::StartPy (ForestUtil* forest_util_p, ForestCmdOpt* cmdopt, ForestSetup* forest_setup,
                         int argc, char* argv[])
#endif // defined(COMP_RUNNER)
{
#if defined(FROG_API_IF)
    if (g_is_init_py) {
        InitPy ();
    }
#endif // #if defined(FROG_API_IF)

    m_forest_util = forest_util_p;
    m_cmdopt = cmdopt;
    m_forest_setup = forest_setup;
    m_print_pythonif = m_cmdopt->m_print_pythonif_flag;
    m_rom_fetch_latency += m_cmdopt->GetRomWait ();

#if defined(FROG_API_IF)
    if (g_is_init_py == false) {
        g_is_init_py = true;
#else // #if defined(FROG_API_IF)
    {
#endif // #if defined(FROG_API_IF)

        // Pythonインタプリタ初期化
        Py_Initialize ();

        // CForest用インタフェース定義のロード
        Py_InitModule ("simso", m_cforest_api_def);

#if defined(COMP_RUNNER)
        PyRun_SimpleString("CFDEF_DEF_CMP_RANKS = 10\n");
#endif // defined(COMP_RUNNER)

        // Load common function
        if (PyRun_SimpleString(R"xyz(
# 定数
CFDEF_GLOBAL = 0xFFFF
CFDEF_DEF_NC = -1
CFDEF_DEF_TC = CFDEF_DEF_NC
CFDEF_DEF_STEP = 1
CFDEF_DEF_OUT = ''
CFDEF_LOAD_OFFSET = 0
CFDEF_ME_SIZE = 1
CFDEF_INVALIDID = -3
CFDEF_INVALID_CONTEXT = CFDEF_INVALIDID, CFDEF_INVALIDID
CFDEF_ALL_PE = 0x7FFF0000
CFDEF_NOSEL_ID = -2
CFDEF_NOSEL_CONTEXT = CFDEF_NOSEL_ID, CFDEF_NOSEL_ID
CFDEF_CORETYPE = "g4mh"
)xyz") == -1) {printf("Error: internal error: %d\n", __LINE__);}
        if (PyRun_SimpleString(CommandDef_data) == -1) {printf("Error: internal error: %d\n", __LINE__);}

#if defined(COMP_RUNNER)
        if (PyRun_SimpleString(R"xyz(
def iset_comprunner(self, block_size, limit_lines = CFDEF_DEF_CMP_RANKS):
    simso.SetCompRunnerPy(block_size, limit_lines)
def iset_xdump_fpu(self, insn_id, ulf_diff):
    simso.XDumpFPU(insn_id, ulf_diff)
def iset_xdump_reg(self, start_addr, end_start):
    simso.XDumpREG(start_addr, end_start)
def iset_xdump_flag(self, start_addr, end_start):
    simso.XDumpFLAG(start_addr, end_start)
def iset_xdump_data(self, start_addr, end_start, mask):
    simso.XDumpDATA(start_addr, end_start, mask)
def iset_xdump_sysreg(self, reg_id, sel_id, mask):
    simso.XDumpSYSREG(reg_id, sel_id, mask)
def iset_cmp_stage(self, stage):
    simso.SetCmpStage(stage)
def iset_cyc_cmp_pc(self, start_pc, end_pc):
    simso.SetCycCmpPC(start_pc, end_pc)
#def iset_ipir_eiint_ch(self, offset_intch):
#    simso.SetIpirEiintChannelPy(offset_intch)
setattr (cforest, "set_comprunner", iset_comprunner)
setattr (cforest, "set_xdump_fpu",  iset_xdump_fpu)
setattr (cforest, "set_xdump_reg", iset_xdump_reg)
setattr (cforest, "set_xdump_flag", iset_xdump_flag)
setattr (cforest, "set_xdump_data", iset_xdump_data)
setattr (cforest, "set_xdump_sysreg", iset_xdump_sysreg)
setattr (cforest, "set_cmp_stage", iset_cmp_stage)
setattr (cforest, "set_cyc_cmp_pc", iset_cyc_cmp_pc)
#setattr (cforest, "set_ipir_eiint_ch", iset_ipir_eiint_ch)
)xyz") == -1) {printf("Error: internal error: %d\n", __LINE__);}
#endif // defined(COMP_RUNNER)
#if defined(COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)
        if (PyRun_SimpleString(R"xyz(
def iset_misr_log(self, has):
    simso.SetCompMisrShowPy(has)
setattr (cforest, "set_misr_log", iset_misr_log)
)xyz") == -1) {printf("Error: internal error: %d\n", __LINE__);}
#endif // defined(COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)
        if (PyRun_SimpleString(CommandAlias_data) == -1) {printf("Error: internal error: %d\n", __LINE__);}
#if defined(COMP_RUNNER)
        if (PyRun_SimpleString(R"xyz(
def set_comprunner(block_size, limit_lines = CFDEF_DEF_CMP_RANKS):
    sim.set_comprunner(block_size, limit_lines)
def xdump_fpu(insn_id, ulp_diff):
    sim.set_xdump_fpu(insn_id, ulp_diff)
def xdump_reg(start_addr, end_start):
    sim.set_xdump_reg(start_addr, end_start)
def xdump_flag(start_addr, end_start):
    sim.set_xdump_flag(start_addr, end_start)
def xdump_data(start_addr, end_start, mask):
    sim.set_xdump_data(start_addr, end_start, mask)
def xdump_sysreg(reg_id, sel_id, mask):
    sim.set_xdump_sysreg(reg_id, sel_id, mask)
def set_cmp_stage(stage):
    sim.set_cmp_stage(stage)
def set_cmp_pc(start_pc, end_pc):
    sim.set_cyc_cmp_pc(start_pc, end_pc)
#def set_ipir_eiint_ch(offset_intch):
#    sim.set_eiint_channel_offset(offset_intch)
)xyz") == -1) {printf("Error: internal error: %d\n", __LINE__);}
#endif // defined(COMP_RUNNER)
#if defined(COMP_RUNNER_G3MH)
        if (PyRun_SimpleString(R"xyz(
def set_misr_log (has):
    sim.set_misr_log (has)
)xyz") == -1) {printf("Error: internal error: %d\n", __LINE__);}
#endif // defined(COMP_RUNNER_G3MH)
        if (PyRun_SimpleString(elfloader_data) == -1) {printf("Error: internal error: %d\n", __LINE__);}
        if (m_cmdopt->IsEnableProfiler()) {
            if (PyRun_SimpleString(profiler_data) == -1) {printf("Error: internal error: %d\n", __LINE__);}
        }
        if (PyRun_SimpleString(R"xyz(
if __name__ == '__main__':
    sim = cforest()
)xyz") == -1) {printf("Error: internal error: %d\n", __LINE__);}

#if defined(COMP_RUNNER_G3KH)
        if (maaen_pin != 0) {
          if (PyRun_SimpleString("force_maaen = 1\n") == -1) {printf("Error: internal error: %d\n", __LINE__);}
        } else {
          if (PyRun_SimpleString("force_maaen = 0\n") == -1) {printf("Error: internal error: %d\n", __LINE__);}
        }
#endif
#if defined (COMP_RUNNER_G3MH)
        if (pe1_peid == 0) {
          pe1_peid = 1;   // PEID is not set from RTL side, default value is applied
        }
        if (pe2_peid == 0) {
          pe2_peid = 2;   // PEID is not set from RTL side, default value is applied
        }
        std::stringstream str_pe1_peid, str_pe2_peid;
        str_pe1_peid << "pe1_peid = " << pe1_peid << "\n";
        str_pe2_peid << "pe2_peid = " << pe2_peid << "\n";
        if (PyRun_SimpleString(str_pe1_peid.str().c_str()) == -1) {printf("Error: internal error: %d\n", __LINE__);}
        if (PyRun_SimpleString(str_pe2_peid.str().c_str()) == -1) {printf("Error: internal error: %d\n", __LINE__);}
#endif // defined (COMP_RUNNER_G3MH)

        // 追加ライブラリのロード
        if (PyRun_SimpleString(R"xyz(
if __name__ == '__main__':
    # elftoolsが無い場合はelfloaderを組み込まない
    try:
        import elftools
    except ImportError:
        pass
    else:
        global el
        el=elfloader()
        if el:
            print('Enable elf loader')
)xyz") == -1) {printf("Error: internal error: %d\n", __LINE__);}

        LoadReadlinePy();
    }

    // もしもscrが指定されていたら実行
    if (m_cmdopt->GetScriptName () != "") {
        PyObject* py_file = PyFile_FromString (
                                 const_cast<char*>(m_cmdopt->GetScriptName ().c_str()),
                                 const_cast<char*>("r"));
        if (py_file == NULL) {
            std::cout << "file open error: " << m_cmdopt->GetScriptName () << std::endl;
            ExitHandler (EXIT_NORMAL);
        }

        // Pythonでスクリプトを実行
        PyRun_SimpleFile (PyFile_AsFile(py_file), m_cmdopt->GetScriptName ().c_str());

#if defined(COMP_RUNNER) || defined(FROG_API_IF)
#else // defined(COMP_RUNNER) || defined(FROG_API_IF)
        // quitせずに終わっていたら、Pythonインタプリタを呼び出す
        if (m_is_quited == false) {
            Py_Main (1, argv);
        }
#endif // defined(COMP_RUNNER) || defined(FROG_API_IF)
    } else {
        // Pythonインタプリタ開始
        //   現在(Python2.4〜2.7)では終了時にPy_Mainから戻ってこないので、
        //   後始末はPy_Mainの中で行う
        Py_Main (1, argv);
        // 現在は戻ってこないが念の為
        ExitHandler (m_forest_util->GetExitStatus());
    }

    // Pythonインタプリタ終了
#if defined(COMP_RUNNER) || defined(FROG_API_IF)
#else // defined(COMP_RUNNER) || defined(FROG_API_IF)
    Py_Finalize ();
#endif // defined(COMP_RUNNER) || defined(FROG_API_IF)
}


PyObject* PythonAPI::IsEnabledPrintPy(PyObject* self, PyObject* args)
{
    return Py_BuildValue ("i", m_print_pythonif);
}

PyObject* PythonAPI::AddBreakAccessPy (PyObject* self, PyObject* args)
{
    int peid, tcid;
    unsigned int start_addr;
    unsigned int end_addr;
    char* access_attr;

    PyArg_ParseTuple (args, "IIsii",
                      &start_addr, &end_addr, &access_attr, &peid, &tcid);

    if (IsPeidValid (peid) == false) {
      CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    std::string attr = access_attr;
    if (attr == "r") {
        m_forest_util->AddBreakAccess (peid, start_addr, end_addr,
                                       ACC_READ, ACC_BREAK_MASK);
    } else if (attr == "w") {
        m_forest_util->AddBreakAccess (peid, start_addr, end_addr,
                                       ACC_WRITE, ACC_BREAK_MASK);
    } else if (attr == "rw") {
        m_forest_util->AddBreakAccess (peid, start_addr, end_addr,
                                       ACC_READWRITE, ACC_BREAK_MASK);
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid access type>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::DisplayBreakAccessPy (PyObject* self, PyObject* args)
{
    int peid, tcid;

    PyArg_ParseTuple (args, "ii", &peid, &tcid);
    if (IsPeidValid (peid) == false) {
      CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
        return Py_BuildValue ("i", PY_NG);
    }

    m_forest_util->DisplayBreakAccess (peid);

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::ClearBreakAccessPy (PyObject* self, PyObject* args)
{
    int peid, tcid;
    unsigned int start_addr;
    unsigned int end_addr;
    char* access_attr;

    PyArg_ParseTuple (args, "IIsii",
                      &start_addr, &end_addr, &access_attr, &peid, &tcid);

    if (IsPeidValid (peid) == false) {
      CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    std::string attr = access_attr;
    if (attr == "r") {
        m_forest_util->ClearBreakAccess (peid, start_addr, end_addr,
                                         ACC_READ, ACC_BREAK_MASK);
    } else if (attr == "w") {
        m_forest_util->ClearBreakAccess (peid, start_addr, end_addr,
                                         ACC_WRITE, ACC_BREAK_MASK);
    } else if (attr == "rw") {
        m_forest_util->ClearBreakAccess (peid, start_addr, end_addr,
                                         ACC_READWRITE, ACC_BREAK_MASK);
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid access type>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::AddBreakPcPy (PyObject* self, PyObject* args)
{
    int peid, tcid;
    unsigned int addr;

    PyArg_ParseTuple (args, "Iii", &addr, &peid, &tcid);

    if (IsPeidValid (peid) == false) {
      CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    int32_t scid = m_forest_util->GetScId (peid);
    if (SetPcBreak (scid, addr) == false) {
        // 既に同じエントリがある
        // メッセージ自体はWarningだが、必ず表示したいのでERRレベル
        CfMsg::DPrint (MSG_ERR, "<Warning: Same breakpoint is already set>\n");
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::DisplayBreakPcPy (PyObject* self, PyObject* args)
{
    int peid, tcid;

    PyArg_ParseTuple (args, "ii", &peid, &tcid);
    if (IsPeidValid (peid) == false) {
      CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
        return Py_BuildValue ("i", PY_NG);
    }

    m_forest_util->DisplayBreakPc (peid);
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::ClearBreakPcPy (PyObject* self, PyObject* args)
{
    int peid, tcid;
    unsigned int addr;

    PyArg_ParseTuple (args, "Iii", &addr, &peid, &tcid);

    if (IsPeidValid (peid) == false) {
      CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    int32_t scid = m_forest_util->GetScId (peid);
    if (ClearPcBreak (scid, addr) == false) {
        // 指定されたエントリが存在しない
        // メッセージ自体はWarningだが、必ず表示したいのでERRレベル
        CfMsg::DPrint (MSG_ERR, "<Warning: No matched entry>\n");
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetLoadNameCmdoptPy (PyObject* self, PyObject* args)
{
    std::string filename;
    if (m_cmdopt->GetHexName () != "") {
        // コマンドラインでHEX指定済みなら、指定されたHEXファイルを使用
        filename = m_cmdopt->GetHexName ();
    } else {
        filename = "";
    }
    return Py_BuildValue("s#", filename.c_str(), filename.length());
    //return Py_BuildValue ("s", filename.c_str());
}


PyObject* PythonAPI::LoadSrecPy (PyObject* self, PyObject* args)
{
    char* filename;
    unsigned int offset;
    bool result;

    PyArg_ParseTuple (args, "sI", &filename, &offset);

    result = m_forest_util->LoadSrec (filename, offset);

    if (result == true) {
        return Py_BuildValue ("i", PY_OK);
    } else {
        return Py_BuildValue ("i", PY_NG);
    }
}


PyObject* PythonAPI::ContExecutePy (PyObject* self, PyObject* args)
{
    int count;
    int break_reason = BREAK_NONE;
    PhysAddr break_acc = 0;
    PhysAddr break_pc = 0;
    unsigned long long break_max = 0;
    ContextId break_tcid = NC;
    unsigned int break_peid = 0;
    int32_t break_scid = 0;

    PyArg_ParseTuple (args, "i", &count);

    for (int i = 0; i < count; i++) {
        if (TryStepExec () == NO_BREAK) {
            // Ctrl-C をフック
            sighandler_t sigint_bak = signal (SIGINT, ForestUtil::ReqSimStop);

            break_scid = m_forest_util->StepExecute<RUN_MODE> (0, 0);

            // Ctrl-Cのをフックを解除
            signal (SIGINT, sigint_bak);

            break_reason = m_forest_util->GetBreakEvent ();
            break_pc = m_forest_util->GetBreakPc ();
            UpdateBreakInfo (break_scid);
        } else {
            // There is no update for SCID routine because it was already updated in the previous condition.
            // In this case, only the variable status for the last Breakpoint event with caused scid, and PC address are updated.
            break_scid = m_last_break.first;
            break_reason = m_last_break_event;
            break_pc = m_last_break.second;
        }

        // Update the common information for Breakpoint event.
        // NOTE: Because some unnecessary informations will have invalid value,
        //       use only valid information in later sequence.
        //       For example, break_acc in BREAK_PC is invalid.
        break_acc = m_forest_util->GetAccBreakAddr ();
        break_max = m_forest_util->GetBreakMax();
        break_peid = m_forest_util->GetPeId (break_scid);

        // Check event in pc break
        if (break_reason == BREAK_PC) {
            if (ExecEvent (break_pc, break_scid) == true) {
                // retry
                -- i;
                continue;
            }
        }
    }

    if (break_reason == BREAK_PC) {
        return Py_BuildValue ("iKIi", 1, break_pc, break_peid, break_tcid);
    } else if (break_reason == BREAK_ACC_READ) {
        return Py_BuildValue ("iKKIi", 2, break_pc, break_acc, break_peid, break_tcid);
    } else if (break_reason == BREAK_ACC_WRITE) {
        return Py_BuildValue ("iKKIi", 3, break_pc, break_acc, break_peid, break_tcid);
    } else if (break_reason == BREAK_MAX) {
        return Py_BuildValue ("iKK", 4, break_max, break_pc);
    } else {
        if (unlikely (ForestUtil::m_stop_req == true) ) {
            PyErr_SetInterrupt();
            return Py_BuildValue ("i", 7);
        }
        if (unlikely (m_forest_util->m_internals->IsAllInHalt () == true) ) {
            return Py_BuildValue ("i", 8);
        }
        return Py_BuildValue ("i", break_reason);
    }
}


PythonAPI::IsBreakType PythonAPI::TryStepExec (void)
{
    if (m_last_break_event == BREAK_PC) {
        int32_t scid = m_last_break.first;
        unsigned int addr = m_last_break.second;
        bool is_removed = ClearPcBreak (scid, addr);

        // Ctrl-C をフック
        sighandler_t sigint_bak = signal (SIGINT, ForestUtil::ReqSimStop);

        int32_t break_scid = m_forest_util->StepExecute<STEP_MODE> (scid, 1);

        // Ctrl-Cのをフックを解除
        signal (SIGINT, sigint_bak);

        UpdateBreakInfo (break_scid); // step実行でブレークしたかもしれないので更新

        if (is_removed) {
            SetPcBreak (scid, addr);
        }

        if (m_last_break_event != BREAK_NONE) {
            // step実行でブレーク発生
            return HIT_BREAK;
        }
    }

    return NO_BREAK;
}


bool PythonAPI::ClearPcBreak (int32_t scid, uint32_t addr)
{
    if (IsScidValid (scid) == false) {
        return false;
    }

    std::vector <BreakPcInfo>::iterator it =
    std::find (m_break_pc_vec.begin (), m_break_pc_vec.end (), BreakPcInfo (scid, addr));
    if (it != m_break_pc_vec.end ()) {
        m_break_pc_vec.erase (it);
        uint32_t peid = m_forest_util->GetPeId (scid);
        m_forest_util->ClearBreakNextPc (peid, addr, PC_BREAK_MASK);
        return true;
    }

    return false;
}


bool PythonAPI::SetPcBreak (int32_t scid, uint32_t addr)
{
    if (IsScidValid (scid) == false) {
        return false;
    }

    BreakPcInfo new_bp (scid, addr);
    std::vector <BreakPcInfo>::const_iterator it =
    std::find (m_break_pc_vec.begin (), m_break_pc_vec.end (), new_bp);
    if (it != m_break_pc_vec.end ()) {
        // 既に同じエントリがある
        return false;
    }

    m_break_pc_vec.push_back (new_bp);
    uint32_t peid = m_forest_util->GetPeId (scid);
    m_forest_util->AddBreakNextPc (peid, addr, PC_BREAK_MASK);
    return true;
}


void PythonAPI::UpdateBreakInfo (int32_t break_scid)
{
    // Ctrl+Cブレークの場合など、break_scidが無効な値(-1)になりうる。
    // m_last_break.first(=break_scid)を使う箇所で判定するので、
    // ここではscidの範囲チェックはしない。

    m_last_break_event = m_forest_util->GetBreakEvent ();
    if (m_last_break_event == BREAK_PC) {
        // 実行前ブレークの一時解除のために、ブレーク情報を保存
        m_last_break.first = break_scid;
        m_last_break.second = m_forest_util->GetBreakPc ();
    }
    m_forest_util->ClearBreakEvent ();
}


PyObject* PythonAPI::SetBreakMaxPy (PyObject* self, PyObject* args)
{
    unsigned long long count;

    PyArg_ParseTuple (args, "K", &count);

    if (m_cmdopt->IsSetMaxCmdOpt () == true) {
        // コマンドラインで--max指定された場合は、Python指定値を上書き
        count = m_cmdopt->GetMaxStep ();
        CfMsg::DPrint (MSG_WAR, "<Warning: Designated max step is overwritten by --max>\n");
    }
    m_forest_util->SetBreakMax (count);

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetBreakMaxPy (PyObject* self, PyObject* args)
{
    unsigned long long count;

    count = m_forest_util->GetBreakMax ();

    return Py_BuildValue ("K", count);
}


PyObject* PythonAPI::SetBreakSamePcMaxPy (PyObject* self, PyObject* args)
{
    unsigned long long count;

    PyArg_ParseTuple (args, "K", &count);

    m_forest_util->SetBreakSamePcMax (count);

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetBreakSamePcMaxPy (PyObject* self, PyObject* args)
{
    unsigned long long count;

    count = m_forest_util->GetBreakSamePcMax ();

    return Py_BuildValue ("K", count);
}


PyObject* PythonAPI::SetBreakSystimePy (PyObject* self, PyObject* args)
{
    unsigned long long count;

    PyArg_ParseTuple (args, "K", &count);

    m_forest_util->SetBreakSystime (count);

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetBreakSystimePy (PyObject* self, PyObject* args)
{
    unsigned long long count;

    count = m_forest_util->GetBreakSystime ();

    return Py_BuildValue ("K", count);
}


PyObject* PythonAPI::MemResourceDisplayPy (PyObject* self, PyObject* args)
{
    std::string peri;
    std::string mem;

    mem = m_forest_util->MemResourceDisplay ();
    peri = m_forest_util->PeripheralMapDisplay ();

    return Py_BuildValue ("s", (mem + peri).c_str());
}


PyObject* PythonAPI::MemDisplayPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;
    unsigned int end_addr;
    char* filename;
    int peid, tcid;

    PyArg_ParseTuple (args, "IIsii",
                      &start_addr, &end_addr, &filename, &peid, &tcid);

    std::string fname = filename;
    FILE* local_file = stdout;
    if (fname.empty () == false) {
        local_file = fopen (fname.c_str (), "a");
        if (local_file == NULL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Can't open %s>\n", filename);
            return Py_BuildValue ("i", PY_NG);
        }
    }

    // 全部PY_NOSEL_IDならContextID指定なしなので、Physicalアドレス扱い
    bool is_phys = ((peid == PY_NOSEL_ID) && (tcid == PY_NOSEL_ID));

    // TCID/PEIDの有効判定はMemDisplay内で実施
    MemDisplay (start_addr, end_addr, peid, tcid, local_file, is_phys);

    if (local_file != stdout) {
        fclose (local_file);
    }

    return Py_BuildValue ("i", PY_OK);
}


void PythonAPI::MemDisplay (PhysAddr start, PhysAddr end, uint32_t peid,
                            ContextId tcid, FILE* fp, bool is_phys)
{
    // 環境変数 MD_MAX による表示バイト数制限をかける
    char* p_mdmax_env = getenv("MD_MAX");
    if (p_mdmax_env != NULL) {
        uint32_t mdmax = strtol (p_mdmax_env, NULL, 16);
        if (mdmax == 0) return;  // MD_MAX==0なら何も表示しない
        if (start + mdmax - 1 < end) {
            CfMsg::DPrint (MSG_WAR, "<Warning: $MD_MAX truncated memory display length>\n");
            end = start + mdmax - 1;
        }
    }

    if (is_phys == false) {
        // 仮想アドレス
        if (IsPeidValid (peid) == false) {
          CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
            return; // エラー
        }

        start &= 0xFFFFFFFF;
        end &= 0xFFFFFFFF;
    }

    if (end <= start) {
        end = start;
    }

    if (start % 16 != 0) {
        fprintf (fp, "%08" PRIx32, start);
    }
    for (PhysAddr addr = start; addr <= end; ++addr) {
        if (addr % 16 == 0) {
            fprintf (fp, "%08" PRIx32, addr);
        }
        uint8_t data = 0;
        if (is_phys) {
            m_forest_util->BusReadDebug (addr, &data, SIZE_8BIT);
        } else {
            data = static_cast <uint8_t> (m_forest_util->VMemReadDebug (peid,
                                                 static_cast <PhysAddr>(addr),
                                                 SIZE_8BIT) & 0xFF);
        }
        fprintf (fp, " %02x", data);
        if (addr % 16 == 15 || addr == end) {
            fprintf (fp, "\n");
        }
    }
}


PyObject* PythonAPI::MemReadPy (PyObject* self, PyObject* args)
{
    unsigned int addr;
    unsigned int size;
    int peid, tcid;

    PyArg_ParseTuple (args, "IIii",
                      &addr, &size, &peid, &tcid);

    if ( (size == 0) || (size > 4) ) {
        CfMsg::DPrint (MSG_ERR, "<Error: Unsupported access size %u>\n", size);
        return Py_BuildValue ("i", PY_NG);
    }

    uint32_t data = 0;
    // 全部PY_NOSEL_IDならContextID指定なしなので、Physicalアドレス扱い
    if ( (peid == PY_NOSEL_ID) && (tcid == PY_NOSEL_ID) ) {
        m_forest_util->BusReadDebug (addr, reinterpret_cast<uint8_t*>(&data), size);
    } else {
        // 仮想アドレス
        if (IsPeidValid (peid) == false) {
          CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n",__func__,peid);
            return Py_BuildValue ("i", PY_NG);
        }

        data = m_forest_util->VMemReadDebug (peid,
                                             static_cast<PhysAddr>(addr & 0xFFFFFFFF),
                                             size);
    }
    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::MemEditPy (PyObject* self, PyObject* args)
{
    unsigned int addr;
    unsigned int data;
    unsigned int size;
    int peid, tcid;

    PyArg_ParseTuple (args, "IIIii", &addr, &data, &size, &peid, &tcid);

    if ( (size == 0) || (size > 4) ) {
        CfMsg::DPrint (MSG_ERR, "<Error: Unsupported access size %u>\n", size);
        return Py_BuildValue ("i", PY_NG);
    }

    if ( (peid == PY_NOSEL_ID) && (tcid == PY_NOSEL_ID) ) {
        // 全部PY_NOSEL_IDならContextID指定なしなので、Physicalアドレス扱い
        m_forest_util->BusWriteDebug (addr, reinterpret_cast<MemData8*>(&data), size);
    } else {
        // 仮想アドレス
        if (IsPeidValid (peid) == false) {
          CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
            return Py_BuildValue ("i", PY_NG); // エラー
        }

        PhysAddr vaddr = static_cast<PhysAddr> (addr & 0xFFFFFFFF);
        m_forest_util->VMemWriteDebug (peid, vaddr, data, static_cast<ByteSize> (size));
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::MemSetPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;
    unsigned int end_addr;
    unsigned int read_latency;
    unsigned int write_latency;
    unsigned int bind_pe;
    unsigned int num_outstand;
    unsigned int repeat_rate;
    unsigned int bank_num;
    unsigned int bank_size;
    char* access_attr;

    CfMsg::DPrint (MSG_WAR, "<Warning: ms command is deprecated. Consider using ms_pe and ms_cl command instead>\n");

    PyArg_ParseTuple (args, "IIsIIIIIII", &start_addr, &end_addr, &access_attr,
                                        &read_latency, &write_latency, &bind_pe,
                                        &num_outstand, &repeat_rate, &bank_num, &bank_size);

    if ((bind_pe != MEM_LOCATE_GLOBAL) && (IsPeidValid (bind_pe) == false)) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    MemRestrictType restrict_type = MRT_ACCESS_DISABLE;
    std::string attr = access_attr;
    std::string::iterator it = attr.begin();
    while (it != attr.end()) {
        switch (*it) {
        case 'r' : restrict_type |= MRT_READ_ENABLE;  break;
        case 'w' : restrict_type |= MRT_WRITE_ENABLE; break;
        case 'x' : restrict_type |= MRT_FETCH_ENABLE; break;
        case 'd' : restrict_type |= MRT_SELF_ONLY;    break;
        case 'n' :
            restrict_type = MRT_ACCESS_DISABLE;
            it = attr.end() - 1;
            break;
        default:
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid memory restrict type>\n");
            return Py_BuildValue ("i", PY_NG);
        }
        ++ it;
    }

    // PE間レイテンシはPEローカルメモリにだけ反映する
    uint32_t pe_to_pe_latency;
    if (bind_pe == MEM_LOCATE_GLOBAL) {
        pe_to_pe_latency = 0;
    } else {
        pe_to_pe_latency = m_pe_to_pe_latency;
    }

    uint32_t fetch_latency = read_latency;
    uint32_t exo_pe_fetch_latency = read_latency + pe_to_pe_latency;
    MemMappedArea area;
    if ( (start_addr >= m_rom_start_addr)
      && (start_addr < m_rom_end_addr) ) {
        area = MI_CF_FROM;
        if (bind_pe == MEM_LOCATE_GLOBAL) {
            fetch_latency = m_rom_fetch_latency;
            exo_pe_fetch_latency = m_rom_fetch_latency;
        } else {
            // PEローカルなら、自PEレイテンシだけ特殊。
            fetch_latency = m_rom_fetch_latency;
        }
    } else if (bind_pe != MEM_LOCATE_GLOBAL) {
        area = MI_CF_LRAM;
        fetch_latency = m_lram_fetch_latency;
    } else {
        area = MI_CF_CRAM;
    }

    uint32_t total_bank_size = static_cast <uint32_t> ((end_addr - start_addr) + 1ULL);
    if (total_bank_size < bank_size) {
        bank_num = 1;
    }

    std::vector <ByteSize> bank_size_vec;
    if (bank_num <= 1) {
        bank_size_vec.push_back (total_bank_size);
    } else {
        if (bank_size != 0) {
            // First bank is designated bank_size.
            // Remain banks are divided into equal parts.
            // Last bank has remain size additionally when (size%bank_num!=0).
            total_bank_size -= bank_size;
            bank_size_vec.assign (bank_num, total_bank_size / (bank_num - 1));
            uint32_t bank_size_remain = total_bank_size % (bank_num - 1);
            bank_size_vec.back() += bank_size_remain;
            bank_size_vec.front() = bank_size;
        } else {
            // Banks are divided into equal parts by designated bank_num.
            // Last bank has remain size additionally when (size%bank_num!=0).
            bank_size = total_bank_size / bank_num;
            uint32_t bank_size_remain = total_bank_size % bank_num;
            bank_size_vec.assign (bank_num, bank_size);
            bank_size_vec.back() += bank_size_remain;
        }
    }

    bool ret = m_forest_util->MemResourceAlloc (restrict_type,
                                                fetch_latency,
                                                read_latency,
                                                write_latency,
                                                exo_pe_fetch_latency,
                                                read_latency + pe_to_pe_latency,
                                                write_latency + pe_to_pe_latency,
                                                // @@ TODO : Temporally, cl_latency = exo_cl_latency
                                                exo_pe_fetch_latency,
                                                read_latency + pe_to_pe_latency,
                                                write_latency + pe_to_pe_latency,
                                                start_addr,
                                                end_addr,
                                                area,
                                                0,
                                                static_cast<MemLocationType> (bind_pe),
                                                0, // @@ TODO: Temporally, locate in CL0.
                                                num_outstand,
                                                repeat_rate,
                                                &bank_size_vec);
    if (ret == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Failed to allocate memory>\n");
        return Py_BuildValue ("i", PY_NG);
    }
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::MemSetPePy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;
    unsigned int end_addr;
    unsigned int pe_fetch_latency;
    unsigned int pe_read_latency;
    unsigned int pe_write_latency;
    unsigned int cl_fetch_latency;
    unsigned int cl_read_latency;
    unsigned int cl_write_latency;
    unsigned int exocl_fetch_latency;
    unsigned int exocl_read_latency;
    unsigned int exocl_write_latency;
    unsigned int bind_pe;
    unsigned int num_outstand;
    unsigned int repeat_rate;
    unsigned int bank_num;
    unsigned int bank_size;
    char* access_attr;

    PyArg_ParseTuple (args, "IIsIIIIIIIIIIIIII", &start_addr, &end_addr, &access_attr,
                                        &pe_fetch_latency, &pe_read_latency, &pe_write_latency,
                                        &cl_fetch_latency, &cl_read_latency, &cl_write_latency,
                                        &exocl_fetch_latency, &exocl_read_latency, &exocl_write_latency,
                                        &bind_pe,
                                        &num_outstand, &repeat_rate, &bank_num, &bank_size);

    if ((bind_pe != MEM_LOCATE_GLOBAL) && (IsPeidValid (bind_pe) == false)) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    uint32_t bind_cl = 0;
    if (bind_pe != MEM_LOCATE_GLOBAL) {
        bind_cl = m_forest_util->GetClidFromPeid (bind_pe);
    }

    MemRestrictType restrict_type = MRT_ACCESS_DISABLE;
    std::string attr = access_attr;
    std::string::iterator it = attr.begin();
    while (it != attr.end()) {
        switch (*it) {
        case 'r' : restrict_type |= MRT_READ_ENABLE;  break;
        case 'w' : restrict_type |= MRT_WRITE_ENABLE; break;
        case 'x' : restrict_type |= MRT_FETCH_ENABLE; break;
        case 'd' : restrict_type |= MRT_SELF_ONLY;    break;
        case 'n' :
            restrict_type = MRT_ACCESS_DISABLE;
            it = attr.end() - 1;
            break;
        default:
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid memory restrict type>\n");
            return Py_BuildValue ("i", PY_NG);
        }
        ++ it;
    }

    MemMappedArea area;
    if ( (start_addr >= m_rom_start_addr)
      && (start_addr < m_rom_end_addr) ) {
        area = MI_CF_FROM;
    } else if (bind_pe != MEM_LOCATE_GLOBAL) {
        area = MI_CF_LRAM;
    } else {
        area = MI_CF_OTHER;
    }

    uint32_t total_bank_size = static_cast <uint32_t> ((end_addr - start_addr) + 1ULL);
    if (total_bank_size < bank_size) {
        bank_num = 1;
    }

    std::vector <ByteSize> bank_size_vec;
    if (bank_num <= 1) {
        bank_size_vec.push_back (total_bank_size);
    } else {
        if (bank_size != 0) {
            // First bank is designated bank_size.
            // Remain banks are divided into equal parts.
            // Last bank has remain size additionally when (size%bank_num!=0).
            total_bank_size -= bank_size;
            bank_size_vec.assign (bank_num, total_bank_size / (bank_num - 1));
            uint32_t bank_size_remain = total_bank_size % (bank_num - 1);
            bank_size_vec.back() += bank_size_remain;
            bank_size_vec.front() = bank_size;
        } else {
            // Banks are divided into equal parts by designated bank_num.
            // Last bank has remain size additionally when (size%bank_num!=0).
            bank_size = total_bank_size / bank_num;
            uint32_t bank_size_remain = total_bank_size % bank_num;
            bank_size_vec.assign (bank_num, bank_size);
            bank_size_vec.back() += bank_size_remain;
        }
    }

    bool ret = m_forest_util->MemResourceAlloc (restrict_type,
                                                pe_fetch_latency, // Unused
                                                pe_read_latency,  // Unused
                                                pe_write_latency, // Unused
                                                cl_fetch_latency,
                                                cl_read_latency,
                                                cl_write_latency,
                                                exocl_fetch_latency,
                                                exocl_read_latency,
                                                exocl_write_latency,
                                                start_addr,
                                                end_addr,
                                                area,
                                                0,
                                                static_cast<MemLocationType> (bind_pe),
                                                static_cast<MemLocationType> (bind_cl),
                                                num_outstand,
                                                repeat_rate,
                                                &bank_size_vec);
    if (ret == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Failed to allocate memory>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    m_is_use_new_ms = true;

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::MemSetClPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;
    unsigned int end_addr;
    unsigned int cl_fetch_latency;
    unsigned int cl_read_latency;
    unsigned int cl_write_latency;
    unsigned int exocl_fetch_latency;
    unsigned int exocl_read_latency;
    unsigned int exocl_write_latency;
    unsigned int bind_cl;
    unsigned int num_outstand;
    unsigned int repeat_rate;
    unsigned int bank_num;
    unsigned int bank_size;
    char* access_attr;

    PyArg_ParseTuple (args, "IIsIIIIIIIIIII", &start_addr, &end_addr, &access_attr,
                                        &cl_fetch_latency, &cl_read_latency, &cl_write_latency,
                                        &exocl_fetch_latency, &exocl_read_latency, &exocl_write_latency,
                                        &bind_cl,
                                        &num_outstand, &repeat_rate, &bank_num, &bank_size);

    if ((bind_cl != MEM_LOCATE_GLOBAL) && (bind_cl > MAX_CL)) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid clid>\n");
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    MemRestrictType restrict_type = MRT_ACCESS_DISABLE;
    std::string attr = access_attr;
    std::string::iterator it = attr.begin();
    while (it != attr.end()) {
        switch (*it) {
        case 'r' : restrict_type |= MRT_READ_ENABLE;  break;
        case 'w' : restrict_type |= MRT_WRITE_ENABLE; break;
        case 'x' : restrict_type |= MRT_FETCH_ENABLE; break;
        case 'd' : restrict_type |= MRT_SELF_ONLY;    break;
        case 'n' :
            restrict_type = MRT_ACCESS_DISABLE;
            it = attr.end() - 1;
            break;
        default:
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid memory restrict type>\n");
            return Py_BuildValue ("i", PY_NG);
        }
        ++ it;
    }

    MemMappedArea area;
    if ( (start_addr >= m_rom_start_addr)
      && (start_addr < m_rom_end_addr) ) {
        area = MI_CF_FROM;
    } else if (bind_cl != MEM_LOCATE_GLOBAL) {
        area = MI_CF_CRAM;
    } else {
        // Exo-cl GlobalRAM / Peripherals
        area = MI_CF_OTHER;
    }

    uint32_t total_bank_size = static_cast <uint32_t> ((end_addr - start_addr) + 1ULL);
    if (total_bank_size < bank_size) {
        bank_num = 1;
    }

    std::vector <ByteSize> bank_size_vec;
    if (bank_num <= 1) {
        bank_size_vec.push_back (total_bank_size);
    } else {
        if (bank_size != 0) {
            // First bank is designated bank_size.
            // Remain banks are divided into equal parts.
            // Last bank has remain size additionally when (size%bank_num!=0).
            total_bank_size -= bank_size;
            bank_size_vec.assign (bank_num, total_bank_size / (bank_num - 1));
            uint32_t bank_size_remain = total_bank_size % (bank_num - 1);
            bank_size_vec.back() += bank_size_remain;
            bank_size_vec.front() = bank_size;
        } else {
            // Banks are divided into equal parts by designated bank_num.
            // Last bank has remain size additionally when (size%bank_num!=0).
            bank_size = total_bank_size / bank_num;
            uint32_t bank_size_remain = total_bank_size % bank_num;
            bank_size_vec.assign (bank_num, bank_size);
            bank_size_vec.back() += bank_size_remain;
        }
    }

    bool ret = m_forest_util->MemResourceAlloc (restrict_type,
                                                cl_fetch_latency, // Unused
                                                cl_read_latency,  // Unused
                                                cl_write_latency, // Unused
                                                cl_fetch_latency,
                                                cl_read_latency,
                                                cl_write_latency,
                                                exocl_fetch_latency,
                                                exocl_read_latency,
                                                exocl_write_latency,
                                                start_addr,
                                                end_addr,
                                                area,
                                                0,
                                                static_cast<MemLocationType> (MEM_LOCATE_GLOBAL),
                                                static_cast<MemLocationType> (bind_cl),
                                                num_outstand,
                                                repeat_rate,
                                                &bank_size_vec);
    if (ret == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Failed to allocate memory>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    m_is_use_new_ms = true;

    return Py_BuildValue ("i", PY_OK);
}


#ifdef CLUSTER
PyObject* PythonAPI::SetClInfoPy (PyObject* self, PyObject* args)
{
    unsigned int clid;
    unsigned int start_addr;
    unsigned int end_addr;
    unsigned int read_latency, write_latency, fetch_latency;
    unsigned int outstand;
    unsigned int repeat_rate;
    char* bridge_name;
    char* access_attr;

    PyArg_ParseTuple (args, "sIIsIIIIII",
                      &bridge_name,
                      &start_addr, &end_addr,
                      &access_attr, &clid,
                      &read_latency, &write_latency, &fetch_latency, &outstand, &repeat_rate);

    /* make bridge restrict type */
    MemRestrictType restrict_type = MRT_ACCESS_DISABLE;
    std::string attr = access_attr;
    std::string::iterator it = attr.begin();
    while (it != attr.end()) {
        switch (*it) {
        case 'r' : restrict_type |= MRT_READ_ENABLE;  break;
        case 'w' : restrict_type |= MRT_WRITE_ENABLE; break;
        case 'x' : restrict_type |= MRT_FETCH_ENABLE; break;
        case 'd' : restrict_type |= MRT_SELF_ONLY;    break;
        case 'n' :
            restrict_type = MRT_ACCESS_DISABLE;
            it = attr.end() - 1;
            break;
        default:
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid memory restrict type %c>\n", (*it));
            return Py_BuildValue ("i", PY_NG);
        }
        ++ it;
    }

    bool ret = m_forest_util->UpdateClusterCycleInfo (bridge_name,    // Bridge name for recognition
                                                      clid,           // Cluster-ID that the region allocated
                                                      start_addr,     // region start address
                                                      end_addr,       // region end address
                                                      restrict_type,  // access restrict type
                                                      read_latency,   // read_latency
                                                      write_latency,  // write_latency
                                                      fetch_latency,  // fetch_latency
                                                      outstand,       // outstandings
                                                      repeat_rate);   // Repeat rate

    if (ret == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Failed to set cluster information>\n");
        return Py_BuildValue ("i", PY_NG);
    }
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::MemSetClPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;
    unsigned int end_addr;
    unsigned int read_latency;
    unsigned int write_latency;
    unsigned int bind_cl;
    unsigned int num_outstand;
    unsigned int repeat_rate;
    unsigned int bank_num;
    unsigned int bank_size;
    char* access_attr;

    PyArg_ParseTuple (args, "IIsIIIIIII", &start_addr, &end_addr, &access_attr,
                                        &read_latency, &write_latency, &bind_cl,
                                        &num_outstand, &repeat_rate, &bank_num, &bank_size);

    unsigned int bind_pe = ForestUtil::MakeClPeId (bind_cl, MEM_LOCATE_GLOBAL);

    MemRestrictType restrict_type = MRT_ACCESS_DISABLE;
    std::string attr = access_attr;
    std::string::iterator it = attr.begin();
    while (it != attr.end()) {
        switch (*it) {
        case 'r' : restrict_type |= MRT_READ_ENABLE;  break;
        case 'w' : restrict_type |= MRT_WRITE_ENABLE; break;
        case 'x' : restrict_type |= MRT_FETCH_ENABLE; break;
        case 'd' : restrict_type |= MRT_SELF_ONLY;    break;
        case 'n' :
            restrict_type = MRT_ACCESS_DISABLE;
            it = attr.end() - 1;
            break;
        default:
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid memory restrict type>\n");
            return Py_BuildValue ("i", PY_NG);
        }
        ++ it;
    }

    uint32_t fetch_latency = read_latency;
    uint32_t exo_pe_fetch_latency = read_latency;
    MemMappedArea area;
    if ( (start_addr >= m_rom_start_addr)
      && (start_addr < m_rom_end_addr) ) {
        area = MI_CF_FROM;
        fetch_latency = m_rom_fetch_latency;
        exo_pe_fetch_latency = m_rom_fetch_latency;
    } else if (read_latency == 0) {
        area = MI_CF_LRAM;
        fetch_latency = m_lram_fetch_latency;
    } else {
        area = MI_CF_EXOPEMEM;
    }

    uint32_t total_bank_size = static_cast <uint32_t> ((end_addr - start_addr) + 1ULL);
    if (total_bank_size < bank_size) {
        bank_num = 1;
    }

    std::vector <ByteSize> bank_size_vec;
    if (bank_num <= 1) {
        bank_size_vec.push_back (total_bank_size);
    } else {
        if (bank_size != 0) {
            // First bank is designated bank_size.
            // Remain banks are divided into equal parts.
            // Last bank has remain size additionally when (size%bank_num!=0).
            total_bank_size -= bank_size;
            bank_size_vec.assign (bank_num, total_bank_size / (bank_num - 1));
            uint32_t bank_size_remain = total_bank_size % (bank_num - 1);
            bank_size_vec.back() += bank_size_remain;
            bank_size_vec.front() = bank_size;
        } else {
            // Banks are divided into equal parts by designated bank_num.
            // Last bank has remain size additionally when (size%bank_num!=0).
            bank_size = total_bank_size / bank_num;
            uint32_t bank_size_remain = total_bank_size % bank_num;
            bank_size_vec.assign (bank_num, bank_size);
            bank_size_vec.back() += bank_size_remain;
        }
    }

    bool ret = m_forest_util->MemResourceAlloc (restrict_type,
                                                fetch_latency,
                                                read_latency,
                                                write_latency,
                                                exo_pe_fetch_latency,
                                                read_latency,
                                                write_latency,
                                                start_addr,
                                                end_addr,
                                                area,
                                                0,
                                                static_cast<MemLocationType> (bind_pe),
                                                num_outstand,
                                                repeat_rate,
                                                &bank_size_vec);
    if (ret == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Failed to allocate memory(%08x-%08x)>\n", start_addr, end_addr);
        return Py_BuildValue ("i", PY_NG);
    }
    return Py_BuildValue ("i", PY_OK);
}
#endif // CLUSTER


PyObject* PythonAPI::SetPeripheralLatencyPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;
    unsigned int end_addr;
    unsigned int read_latency;
    unsigned int write_latency;
    unsigned int exo_pe_latency;
    unsigned int bind_pe;
    unsigned int num_outstand;
    unsigned int repeat_rate;

    PyArg_ParseTuple (args, "IIIIIIII", &start_addr, &end_addr,
                                        &read_latency, &write_latency, &exo_pe_latency,
                                        &bind_pe, &num_outstand, &repeat_rate);

    if (start_addr > end_addr) {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid range>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    if ((bind_pe != MEM_LOCATE_GLOBAL) && (IsPeidValid (bind_pe) == false)) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    bool ret = m_forest_util->SetPeripheralLatency (start_addr, end_addr,
                                                    read_latency,
                                                    write_latency,
                                                    read_latency + exo_pe_latency,
                                                    write_latency + exo_pe_latency,
                                                    read_latency + exo_pe_latency,
                                                    write_latency + exo_pe_latency,
                                                    static_cast<MemLocationType> (bind_pe),
                                                    0,
                                                    num_outstand,
                                                    repeat_rate);
    if (ret == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Failed to find valid peripheral>\n");
        return Py_BuildValue ("i", PY_NG);
    }
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::MemClearPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;

    PyArg_ParseTuple (args, "I", &start_addr);

    if (m_forest_util->MemResourceFree (start_addr, start_addr) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Failed to remove memory>\n");
        return Py_BuildValue ("i", PY_NG);
    } else {
        m_forest_util->ClearAccessDeniedArea (start_addr);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::SelfSetPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;
    unsigned int end_addr;
    int offset;

    PyArg_ParseTuple (args, "IIi", &start_addr, &end_addr, &offset);
    if (start_addr > end_addr) {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid range>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    if (m_forest_util->SetSelfArea (start_addr, end_addr, offset) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Failed to allocate self area>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::SelfClearPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;

    PyArg_ParseTuple (args, "I", &start_addr);
    if (m_forest_util->ClearSelfArea (start_addr) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Failed to remove self area>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::MirrorSetPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;
    unsigned int end_addr;
    int offset;

    PyArg_ParseTuple (args, "IIi", &start_addr, &end_addr, &offset);
    if (start_addr > end_addr) {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid range>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    if (m_forest_util->SetMirrorArea (start_addr, end_addr, offset) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Failed to allocate mirror area>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::MirrorClearPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;

    PyArg_ParseTuple (args, "I", &start_addr);
    if (m_forest_util->ClearMirrorArea (start_addr) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Failed to remove mirror area>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::ErrorAreaSetPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;
    unsigned int end_addr;

    PyArg_ParseTuple (args, "II", &start_addr, &end_addr);
    if (start_addr > end_addr) {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid range>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    if (m_forest_util->SetErrorArea (start_addr, end_addr) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Failed to allocate error area>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::ErrorAreaClearPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;

    PyArg_ParseTuple (args, "I", &start_addr);
    if (m_forest_util->ClearErrorArea (start_addr) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Failed to remove error area>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::SetPeLatency (PyObject* self, PyObject* args)
{
    unsigned int pe_latency;

    PyArg_ParseTuple (args, "I", &pe_latency);

    m_pe_to_pe_latency = static_cast<uint32_t> (pe_latency);

    if (m_is_use_new_ms) {
        // Don't update exist memory latency because new ms command can specify pe_latency directly.
        return Py_BuildValue ("i", PY_OK);
    }

    // 設定済みメモリのexo_pe_latencyを更新
    m_forest_util->UpdateExoPeLatency (m_pe_to_pe_latency);
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetPeLatency (PyObject* self, PyObject* args)
{
    return Py_BuildValue ("I", m_pe_to_pe_latency);
}


PyObject* PythonAPI::SetLramFetchLatency (PyObject* self, PyObject* args)
{
    unsigned int lram_latency;

    PyArg_ParseTuple (args, "I", &lram_latency);
    m_lram_fetch_latency = lram_latency;

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetLramFetchLatency (PyObject* self, PyObject* args)
{
    return Py_BuildValue ("I", m_lram_fetch_latency);
}


PyObject* PythonAPI::SetRomFetchLatency (PyObject* self, PyObject* args)
{
    unsigned int rom_latency;

    PyArg_ParseTuple (args, "I", &rom_latency);
    m_rom_fetch_latency = rom_latency;

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetRomFetchLatency (PyObject* self, PyObject* args)
{
    return Py_BuildValue ("I", m_rom_fetch_latency);
}


PyObject* PythonAPI::QuitPy (PyObject* self, PyObject* args)
{
    m_is_quited = true;

    ExitHandler (m_forest_util->GetExitStatus());
    return Py_BuildValue (""); // 非到達コード
}


PyObject* PythonAPI::HardResetPy (PyObject* self, PyObject* args)
{
    m_forest_util->HardReset ();
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::ResetPePy (PyObject* self, PyObject* args)
{
    unsigned int peid;

    PyArg_ParseTuple (args, "I", &peid);
    RegData64 last_pc = 0;
    m_forest_util->RegReadDebug (peid, PC, &last_pc);
    if (ResetPeIf(peid) == false) {
        CfMsg::DPrint (MSG_WAR, "<Warning: Can't reset PE%d at this core. Error at PC=0x%08x>\n", peid, last_pc);
    } else {
        CfMsg::DPrint (MSG_INF, "<Info: Reset PE%d completely at PC=0x%08x>\n", peid, last_pc);
    }

    return Py_BuildValue ("i", PY_OK);
}


bool PythonAPI::ResetPeIf (uint32_t peid)
{
    if ((m_forest_util->GetCoreType(peid) == CORE_G4MH)
     && (m_forest_util->GetCoreVersion(peid) >= CORE_VER_21 )) {
        if (m_forest_util->FsPeReset (peid) == false){
            return false;
        }
    } else {
        return false;
    }
    return true;
}


PyObject* PythonAPI::StepExecutePy (PyObject* self, PyObject* args)
{
    unsigned int step_goal;
    int peid, tcid;
    int break_scid = 0, break_reason = BREAK_NONE;
    PhysAddr break_acc = 0;
    PhysAddr break_pc = 0;
    unsigned long long break_max = 0;
    ContextId break_tcid = NC;
    unsigned int break_peid = 0;

    PyArg_ParseTuple (args, "Iii", &step_goal, &peid, &tcid);

    if (IsPeidValid (peid) == false) {
      CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
        return Py_BuildValue ("i", PY_NG);
    }

    uint32_t scid = m_forest_util->GetScId (peid);

    // PCブレーク直後ではTryStepExec内で1命令実行されるので、その分を引く
    if (m_last_break_event == BREAK_PC) {
        -- step_goal;
    }

    if (TryStepExec () == NO_BREAK) {
        if (step_goal > 0) {
            // Ctrl-C をフック
            sighandler_t sigint_bak = signal (SIGINT, ForestUtil::ReqSimStop);

            if (m_is_step_any) {
                break_scid = m_forest_util->StepExecute<STEP_ANY_MODE> (scid, step_goal);
            } else {
                break_scid = m_forest_util->StepExecute<STEP_MODE> (scid, step_goal);
            }

            // Ctrl-Cのをフックを解除
            signal (SIGINT, sigint_bak);

            break_reason = m_forest_util->GetBreakEvent ();
            break_pc = m_forest_util->GetBreakPc ();
            UpdateBreakInfo (break_scid);
        }
    } else {
        // There is no update for SCID routine because it was already updated in the previous condition.
        // In this case, only the variable status for the last Breakpoint event with caused scid, and PC address are updated.
        break_scid = m_last_break.first;
        break_reason = m_last_break_event;
        break_pc = m_last_break.second;
    }

    // Update the common information for Breakpoint event.
    // NOTE: Because some unnecessary informations will have invalid value,
    //       use only valid information in later sequence.
    //       For example, break_acc in BREAK_PC is invalid.
    break_acc = m_forest_util->GetAccBreakAddr ();
    break_max = m_forest_util->GetBreakMax();
    break_peid = m_forest_util->GetPeId (break_scid);

    if (break_reason == BREAK_PC) {
        // Check event in PC break
        if (ExecEvent (break_pc, break_scid) == true) {
            return Py_BuildValue ("iKIi", 1, break_pc, break_peid, break_tcid);
        }
        return Py_BuildValue ("i", PY_NG);
    } else if (break_reason == BREAK_ACC_READ) {
        return Py_BuildValue ("iKKIi", 2, break_pc, break_acc, break_peid, break_tcid);
    } else if (break_reason == BREAK_ACC_WRITE) {
        return Py_BuildValue ("iKKIi", 3, break_pc, break_acc, break_peid, break_tcid);
    } else if (break_reason == BREAK_MAX) {
        return Py_BuildValue ("iKK", 4, break_max, break_pc);
    } else {
        if (unlikely (ForestUtil::m_stop_req == true) ) {
            PyErr_SetInterrupt();
            return Py_BuildValue ("i", 7);
        }
        if (unlikely (m_forest_util->m_internals->IsAllInHalt () == true) ) {
            return Py_BuildValue ("i", 8);
        }
        if (step_goal == 1) {
            return Py_BuildValue ("i", 9);
        }
        return Py_BuildValue ("i", break_reason);
    }
}


PyObject* PythonAPI::SwitchStepModePy (PyObject* self, PyObject* args)
{
    if (m_is_step_any == true) {
        m_is_step_any = false;
    } else {
        m_is_step_any = true;
    }
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetStepModePy (PyObject* self, PyObject* args)
{
    return Py_BuildValue ("i", m_is_step_any);
}


PyObject* PythonAPI::RegReadDebugPy (PyObject* self, PyObject* args)
{
    char* regname;
    int peid, tcid;

    PyArg_ParseTuple (args, "sii", &regname, &peid, &tcid);

    if (IsPeidValid (peid) == false) {
      CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
        return Py_BuildValue ("i", PY_NG);
    }

    // レジスタ番号検索
    // VCレジスタアクセス時はTCID無効
    // - レジスタ番号の取得なので、有効なTCであれば、R/W対象スレッドと異なってもOK
    //   ただし、ArchType違いがあり得るので、PEIDは対象スレッドにしないとだめ
    CReg regno = m_forest_util->RegGetRegno (peid, regname);
    if (regno == GR_NONE) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid register name:%s>\n", regname);
        return Py_BuildValue ("i", PY_NG);
    }

    // TCにレジスタアクセス
    uint64_t regdata64 = PY_INVALID_DATA;
    uint64_t regdata64_up = PY_INVALID_DATA;
    bool get_data = m_forest_util->RegReadDebug (peid, regno, &regdata64, &regdata64_up);
    if ((regno >= WR0 && regno <= WR31) && (get_data == true)) {
        // return 128bit (2 * 64bit data) for WR register
        return Py_BuildValue ("KK", regdata64_up, regdata64);
    }

    // TC/VCでレジスタにアクセスできなかったらエラーデータを格納
    if (get_data == false) {
        return Py_BuildValue ("i", PY_NG);
    }

    return Py_BuildValue ("K", regdata64);
}


PyObject* PythonAPI::RegWriteDebugPy (PyObject* self, PyObject* args)
{
    char* regname;
    uint64_t data_u64, data_l64;
    int peid, tcid;

    PyArg_ParseTuple (args, "sKKii", &regname, &data_u64, &data_l64, &peid, &tcid);
    RegData64 regdata = static_cast<RegData64> (data_l64);
    RegData64 regdata_u64 = static_cast<RegData64> (data_u64);

    if (IsPeidValid (peid) == false) {
      CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
        return Py_BuildValue ("i", PY_NG);
    }

    // レジスタ番号検索
    // VCレジスタアクセス時はTCID無効
    // - レジスタ番号の取得なので、有効なTCであれば、R/W対象スレッドと異なってもOK
    //   ただし、ArchType違いがあり得るので、PEIDは対象スレッドにしないとだめ
    CReg regno = m_forest_util->RegGetRegno (peid, regname);
    if (regno == GR_NONE) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid register name:%s>\n", regname);
        return Py_BuildValue ("i", PY_NG);
    }

    // TCにレジスタアクセス
    bool set_data = m_forest_util->RegWriteDebug (peid, regno, regdata, regdata_u64);

    // TC/VCでレジスタにアクセスできなかったらエラーデータを格納
    if (set_data == false) {
        return Py_BuildValue ("i", PY_NG);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::ShowRegPy (PyObject* self, PyObject* args)
{
    m_forest_util->ShowReg();

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::SetEventPy (PyObject* self, PyObject* args)
{
    char* eventchar;
    int peid, tcid;
    unsigned int addr;
    unsigned int channel;
    unsigned int priority;
    unsigned int repeat;
    unsigned int gpid;

    PyArg_ParseTuple (args, "IsIIIIii", &addr, &eventchar, &channel, &priority, &gpid, &repeat, &peid, &tcid);

    if (IsPeidValid (peid) == false) {
      CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    int32_t scid = m_forest_util->GetScId (peid);
    std::string eventstr = eventchar;

    if (IsValidAsyncEvent (eventstr) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid event name:%s>\n", eventstr.c_str());
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    EventInfo* event = new EventInfo;
    event->target_pc = addr;
    event->scid = scid;
    event->peid = peid;
    event->tcid = tcid;
    event->channel = channel;
    event->priority = priority;
    event->event = eventstr;
    event->repeat = repeat;
    event->repeated = 0;
    event->gpid = gpid;
    event->is_set = true;

    SetPcBreak (scid, addr); // @@ set_event doesn't functional with bp command.

    m_event_info.push_back (event);

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::SetClearEventPy (PyObject* self, PyObject* args)
{
    char* eventchar;
    int peid, tcid;
    unsigned int addr;
    unsigned int repeat;

    PyArg_ParseTuple (args, "IsIii", &addr, &eventchar, &repeat, &peid, &tcid);

    if (IsPeidValid (peid) == false) {
      CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    int32_t scid = m_forest_util->GetScId (peid);
    std::string eventstr = eventchar;

    if (IsValidAsyncEvent (eventstr) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid event name:%s>\n", eventstr.c_str());
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    EventInfo* event = new EventInfo;
    event->target_pc = addr;
    event->scid = scid;
    event->peid = peid;
    event->tcid = tcid;
    event->channel = 0;
    event->priority = 0;
    event->event = eventstr;
    event->repeat = repeat;
    event->repeated = 0;
    event->gpid = 0;
    event->is_set = false;

    SetPcBreak (scid, addr); // @@ set_event doesn't functional with bp command.

    m_event_info.push_back (event);

    return Py_BuildValue ("i", PY_OK);
}


bool PythonAPI::ExecEvent (uint32_t break_pc, int32_t break_scid)
{
    bool hit = false;
    std::vector<EventInfo*>::iterator it = m_event_info.begin ();
    while (it != m_event_info.end ()) {
        if ((*it)->IsHit(break_pc, break_scid) == true) {
            if ((*it)->is_set == true) {
                ReqEvent (*it);
            } else {
                ReqClearEvent (*it);
            }
            hit = true;
            (*it)->Increment ();
            if ((*it)->IsExpired ()) {
                // Check other envent at same PC and same context
                bool is_find_same = false;
                std::vector<EventInfo*>::iterator it_check = m_event_info.begin ();
                while (it_check != m_event_info.end ()) {
                    if (  (it != it_check)
                       && ((*it)->scid == (*it_check)->scid)
                       && ((*it)->target_pc == (*it_check)->target_pc)) {
                        is_find_same = true;
                        break;
                    }
                    ++ it_check;
                }
                if (is_find_same == false) {
                    // if there is no event at same PC and same context, delete break point.
                    ClearPcBreak ((*it)->scid, (*it)->target_pc);
                }
                // Delete expired event.
                it = m_event_info.erase (it);
                continue;
            }
        }
        ++ it;
    }
    return hit;
}


bool PythonAPI::ReqEvent (EventInfo* event)
{
    uint32_t peid = event->peid;
    uint32_t channel = event->channel;
    uint32_t priority = event->priority;
    uint32_t gpid = event->gpid;
    std::string eventstr = event->event;
    if (eventstr == "feint") {
        if (m_forest_util->ReqFeintToCpu (peid, channel, false, 0, false) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "gmfeint") {
        if (m_forest_util->ReqFeintToCpu (peid, channel, true, gpid, false) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "bgfeint") {
        if (m_forest_util->ReqFeintToCpu (peid, channel, true, gpid, true) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "fenmi") {
        if (m_forest_util->ReqFenmiToCpu (peid) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "eiint") {
        if (m_forest_util->ReqInterruptToCpu (peid, channel, priority, false, false, 0, false) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "eitbl") {
        if (m_forest_util->ReqInterruptToCpu (peid, channel, priority, true, false, 0, false) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "gmeiint") {
        if (m_forest_util->ReqInterruptToCpu (peid, channel, priority, false, true, gpid, false) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "gmeitbl") {
        if (m_forest_util->ReqInterruptToCpu (peid, channel, priority, true, true, gpid, false) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "bgeiint") {
        if (m_forest_util->ReqInterruptToCpu (peid, channel, priority, false, true, gpid, true) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "dbint") {
        if (m_forest_util->ReqDbintToCpu (peid, channel) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "dbnmi") {
        if (m_forest_util->ReqDbnmiToCpu (peid) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "syserr") { // Operand syserr
        if (m_forest_util->ReqSyserrToCpu (peid, channel) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
#if 0
    } else if (eventstr == "syserr_hm") {
        if (m_forest_util->ReqOperandSyserrToCpu (peid, channel, true) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "syserr_gm") {
        if (m_forest_util->ReqOperandSyserrToCpu (peid, channel, false) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
#endif //0
    } else if (eventstr == "rmint") {
        if (m_forest_util->ReqRmintToCpu (peid) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "stopreq") {
        if (m_forest_util->ReqAllInterruptDisableToCpu (peid) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_WAR, "<Warning: Can't set stopreq at PC=%x>\n", event->target_pc);
            return false;
        }
    } else if (eventstr == "pereset") {
        if (ResetPeIf(peid) == false) {
            CfMsg::DPrint (MSG_WAR, "<Warning: Can't reset PE%d at this core by set_event(). Error at PC=0x%08x>\n", peid, event->target_pc);
            return false;
        } else {
            CfMsg::DPrint (MSG_INF, "<Info: Reset PE%d completely at PC=0x%08x>\n", peid, event->target_pc);
        }
    } else {
        // Doesn't come here, because IsValidAsyncEvent checked this condition.
        return false;
    }

    return true;
}


bool PythonAPI::ReqClearEvent (EventInfo* event)
{
    uint32_t peid = event->peid;
    std::string eventstr = event->event;
    if (eventstr == "feint") {
        if (m_forest_util->CancelFeintToCpu (peid) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "gmfeint") {
        if (m_forest_util->CancelFeintToCpu (peid) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "bgfeint") {
        if (m_forest_util->CancelFeintToCpu (peid) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "fenmi") {
        if (m_forest_util->CancelFenmiToCpu (peid) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "eiint") {
        if (m_forest_util->CancelInterruptToCpu (peid) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "eitbl") {
        if (m_forest_util->CancelInterruptToCpu (peid) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "gmeiint") {
        if (m_forest_util->CancelInterruptToCpu (peid) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "gmeitbl") {
        if (m_forest_util->CancelInterruptToCpu (peid) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "bgeiint") {
        if (m_forest_util->CancelInterruptToCpu (peid) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "dbint") {
        if (m_forest_util->CancelDbintToCpu (peid) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "dbnmi") {
        if (m_forest_util->CancelDbnmiToCpu (peid) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "syserr") {
        if (m_forest_util->CancelSyserrToCpu (peid) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
#if 0
    } else if (eventstr == "syserr_gm") {
        if (m_forest_util->CancelSyserrToCpu (peid) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "syserr_hm") {
        if (m_forest_util->CancelSyserrToCpu (peid) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
#endif //0
    } else if (eventstr == "rmint") {
        if (m_forest_util->CancelRmintToCpu (peid) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid param>\n");
            return false;
        }
    } else if (eventstr == "stopreq") {
        if (m_forest_util->CancelAllInterruptDisableToCpu (peid) == INT_E_CHANNEL) {
            CfMsg::DPrint (MSG_WAR, "<Warning: Can't clear stopreq at PC=%x>\n", event->target_pc);
            return false;
        }
    } else {
        // Doesn't come here, because IsValidAsyncEvent checked this condition.
        return false;
    }

    return true;
}


bool PythonAPI::IsValidAsyncEvent (std::string eventstr)
{
    if (   (eventstr == "feint")
        || (eventstr == "gmfeint")
        || (eventstr == "bgfeint")
        || (eventstr == "fenmi")
        || (eventstr == "eiint")
        || (eventstr == "eitbl")
        || (eventstr == "gmeiint")
        || (eventstr == "gmeitbl")
        || (eventstr == "bgeiint")
        || (eventstr == "dbint")
        || (eventstr == "dbnmi")
        || (eventstr == "syserr")
#if 0
        || (eventstr == "syserr_gm")
        || (eventstr == "syserr_hm")
#endif //0
        || (eventstr == "rmint")
        || (eventstr == "stopreq")
        || (eventstr == "pereset")
       ) {
        return true;
    } else {
        return false;
    }
}


PyObject* PythonAPI::PrintHelpPy (PyObject* self, PyObject* args)
{
    CfMsg::DPrint (MSG_ERR, "<CForest Commands>\n");
    CfMsg::DPrint (MSG_ERR, "- Setup\n");
    CfMsg::DPrint (MSG_ERR, "    debug (\"{ON|OFF}\" [,contextid])\n");
    CfMsg::DPrint (MSG_ERR, "                              : Enable/Disable to display debug information.\n");
    CfMsg::DPrint (MSG_ERR, "    set_debug_option (\"{rw|pipe|dep|bra|systime|cache|timer|intc|perfcnt|fput}, {on|off}\")\n");
    CfMsg::DPrint (MSG_ERR, "                              : Enable/Disable each option of debug mode.\n");
    CfMsg::DPrint (MSG_ERR, "    get_debug_option ()       : Get debug mode.\n");
    CfMsg::DPrint (MSG_ERR, "    exp_message (\"{ON|OFF}\", start_cause, end_cause [,contextid])\n");
    CfMsg::DPrint (MSG_ERR, "                              : Enable/Disable to display exception information.\n");
    CfMsg::DPrint (MSG_ERR, "    setlog (\"filename\" [,\"MULTI\"])\n");
    CfMsg::DPrint (MSG_ERR, "                              : Designate file name for outputting debug information.\n");
    CfMsg::DPrint (MSG_ERR, "    gdb_connect (port)        : Connect to GDB.\n");
    CfMsg::DPrint (MSG_ERR, "    uart_connect (port, channel) : Connect to UART.\n");
    CfMsg::DPrint (MSG_ERR, "    pe (peid)                 : Set default PEID for some command.\n");
    CfMsg::DPrint (MSG_ERR, "- Execution\n");
    CfMsg::DPrint (MSG_ERR, "    {reset|rs} ()               : Issue power-on reset to entire system.\n");
    CfMsg::DPrint (MSG_ERR, "    run ([num])                 : Start simulation from address of current PC.\n");
    CfMsg::DPrint (MSG_ERR, "    step ([num] [[,]contextid]) : Execute designated number of instructions.\n");
    CfMsg::DPrint (MSG_ERR, "    set_stepmode ()             : Switch condition of counting step count in step command.\n");
    CfMsg::DPrint (MSG_ERR, "    get_stepmode ()             : Get condition of counting step count in step command.\n");
    CfMsg::DPrint (MSG_ERR, "- Break\n");
    CfMsg::DPrint (MSG_ERR, "    ba (start_addr [,end_addr] [,\"{R|W|RW}\"] [,contextid])\n");
    CfMsg::DPrint (MSG_ERR, "                              : Set access break to designated data address.\n");
    CfMsg::DPrint (MSG_ERR, "    bad ([contextid])         : Display break accesses of designated instruction address.\n");
    CfMsg::DPrint (MSG_ERR, "    bac (start_addr [,end_addr] [,\"{R|W|RW}\"] [,contextid])\n");
    CfMsg::DPrint (MSG_ERR, "                              : Delete access break of designated address.\n");
    CfMsg::DPrint (MSG_ERR, "    bp (addr [,contextid])    : Set break points to designated instruction address.\n");
    CfMsg::DPrint (MSG_ERR, "    bpd ([contextid])         : Display break points of designated instruction address.\n");
    CfMsg::DPrint (MSG_ERR, "    bpc (addr [,contextid])   : Delete break points of designated address.\n");
    CfMsg::DPrint (MSG_ERR, "    max (num)                 : Stop simulation when it has passed num steps.\n");
    CfMsg::DPrint (MSG_ERR, "    getmax ()                 : Get step count condition of max break.\n");
    CfMsg::DPrint (MSG_ERR, "    max_same_pc (num)         : Stop simulation when same pc is executed continuously for num times.\n");
    CfMsg::DPrint (MSG_ERR, "    get_max_same_pc ()        : Get the num for setting of max_same_pc.\n");
    CfMsg::DPrint (MSG_ERR, "    bt (clock)                : Stop simulation when it has passed num clock in simulation time.\n");
    CfMsg::DPrint (MSG_ERR, "    btd ()                    : Get clock count condition of simulation time break.\n");
    CfMsg::DPrint (MSG_ERR, "- Register\n");
    CfMsg::DPrint (MSG_ERR, "    getreg (\"register\" [,contextid])\n");
    CfMsg::DPrint (MSG_ERR, "                              : Get register value.\n");
    CfMsg::DPrint (MSG_ERR, "    setreg (\"register\", data [,contextid])\n");
    CfMsg::DPrint (MSG_ERR, "                              : Set register value.\n");
    CfMsg::DPrint (MSG_ERR, "    showreg ()                : Display list of register name.\n");
    CfMsg::DPrint (MSG_ERR, "- Memory\n");
    CfMsg::DPrint (MSG_ERR, "    mi ()                     : Display memory map information.\n");
    CfMsg::DPrint (MSG_ERR, "    {ml|load} (\"filename\" [,offset])\n");
    CfMsg::DPrint (MSG_ERR, "                              : Load s-record file.\n");
    CfMsg::DPrint (MSG_ERR, "    mc (start_addr)           : Remove allocated memory.\n");
    CfMsg::DPrint (MSG_ERR, "    md (start_addr [,end_addr] [,\"file_name\"] [,contextid])\n");
    CfMsg::DPrint (MSG_ERR, "                              : Dump designated address of memory.\n");
    CfMsg::DPrint (MSG_ERR, "    me (address, data [,size] [,contextid])\n");
    CfMsg::DPrint (MSG_ERR, "                              : Write to designated address of memory.\n");
    CfMsg::DPrint (MSG_ERR, "    mr (address [,size] [,contextid])\n");
    CfMsg::DPrint (MSG_ERR, "                              : Get from designated address of memory \n");
    //CfMsg::DPrint (MSG_ERR, "    ms (start_addr, end_addr [,\"permission\"] [,read_latency ,write_latency [,bind_pe [,outstand [,repeat]]]])\n");
    //CfMsg::DPrint (MSG_ERR, "                              : Allocate memory. permission is combination of [R|W|X].\n");
    CfMsg::DPrint (MSG_ERR, "    ms_pe (start_addr, end_addr ,\"permission\", pe_fetch_latency, pe_read_latency ,pe_write_latency,\n");
    CfMsg::DPrint (MSG_ERR, "           cl_fetch_latency, cl_read_latency ,cl_write_latency,\n");
    CfMsg::DPrint (MSG_ERR, "           exocl_fetch_latency, exocl_read_latency ,exocl_write_latency,\n");
    CfMsg::DPrint (MSG_ERR, "           bind_peid, outstand ,repeat [, bank_num[, bank_size]])\n");
    CfMsg::DPrint (MSG_ERR, "                              : Allocate PE local memory. permission is combination of [R|W|X].\n");
    CfMsg::DPrint (MSG_ERR, "    ms_cl (start_addr, end_addr ,\"permission\", cl_fetch_latency, cl_read_latency ,cl_write_latency,\n");
    CfMsg::DPrint (MSG_ERR, "           exocl_fetch_latency, exocl_read_latency ,exocl_write_latency,\n");
    CfMsg::DPrint (MSG_ERR, "           bind_clid, outstand ,repeat [, bank_num[, bank_size]])\n");
    CfMsg::DPrint (MSG_ERR, "                              : Allocate cluster local memory. permission is combination of [R|W|X].\n");
    CfMsg::DPrint (MSG_ERR, "    self_clear (start_addr)   : Delete self area.\n");
    CfMsg::DPrint (MSG_ERR, "    self_set (start_addr, end_addr, offset)\n");
    CfMsg::DPrint (MSG_ERR, "                              : Allocate self area.\n");
    CfMsg::DPrint (MSG_ERR, "    mirror_clear (start_addr) : Delete mirror area.\n");
    CfMsg::DPrint (MSG_ERR, "    mirror_set (start_addr, end_addr, offset)\n");
    CfMsg::DPrint (MSG_ERR, "                              : Allocate mirror area.\n");
    CfMsg::DPrint (MSG_ERR, "    error_area_clear (start_addr)   : Delete self area.\n");
    CfMsg::DPrint (MSG_ERR, "    error_area_set (start_addr, end_addr) : Allocate error area.\n");
    CfMsg::DPrint (MSG_ERR, "    pe_latency (latency)      : Set additional latency to access other PE's local memory.\n");
    CfMsg::DPrint (MSG_ERR, "    lram_fetch_latency (latency)     : Set latency to fetch from own local memory.\n");
    CfMsg::DPrint (MSG_ERR, "    rom_fetch_latency (latency)      : Set latency to fetch from FlashROM.\n");
    CfMsg::DPrint (MSG_ERR, "    set_area_rom (start_addr, end_addr)   : Set FlashROM area.\n");
    CfMsg::DPrint (MSG_ERR, "    set_area_lram (start_addr, end_addr)  : Set local ram area.\n");
    CfMsg::DPrint (MSG_ERR, "    set_ostm (peid, ostmch, start_addr, intch) : Sets OSTimer(PE local).\n");
    CfMsg::DPrint (MSG_ERR, "    set_ostm_global (start_addr, intch) : Sets OSTimer(Global).\n");
    CfMsg::DPrint (MSG_ERR, "    set_ostm_clk (ratio)                  : Sets clock divisor for OSTimer.\n");
    CfMsg::DPrint (MSG_ERR, "    get_ostm_clk ()                       : Gets clock divisor for OSTimer.\n");
    CfMsg::DPrint (MSG_ERR, "    set_area_debug (start_addr, end_addr) : Set debug area.\n");
    CfMsg::DPrint (MSG_ERR, "    set_peripheral_cycle (start_addr, end_addr, read_lat, write_lat, pe_latency, bind_pe, outstand, repeat_rate)\n");
    CfMsg::DPrint (MSG_ERR, "                                          : Set latency of peripherals at address area.\n");
    CfMsg::DPrint (MSG_ERR, "    set_memini (value)                      : Set initial value of memory data\n");
    CfMsg::DPrint (MSG_ERR, "    get_memini ()                         : Get initial value of memory data\n");
    CfMsg::DPrint (MSG_ERR, "- HW config\n");
    CfMsg::DPrint (MSG_ERR, "    set_peinfo (peid [, clid], \"{g4mh|g4mh1.0|g4mh1.1|g4mh2.0|g4mh2.1}\")\n");
    CfMsg::DPrint (MSG_ERR, "                                          : Set PE parameter. This command must be used before other commands.\n");
    CfMsg::DPrint (MSG_ERR, "    set_peripherals (\"name\", \"{on|off}\"[,addr])  : Set existance of peripherals for each - edited base address.\n");
    CfMsg::DPrint (MSG_ERR, "        valid name for set_peripherals are\n");
    CfMsg::DPrint (MSG_ERR, "            all, intc1, intc2, mecnt, ipir, barrier, uart, perf, tmu, simio\n");
    CfMsg::DPrint (MSG_ERR, "    set_ipir_eiint_ch (offset_intch)  : eiint channel = ipir channel + offset_intch\n");
    CfMsg::DPrint (MSG_ERR, "    set_guards (\"name\", id, \"{on|off}\", addr)  : Set guard for each.\n");
    CfMsg::DPrint (MSG_ERR, "        valid name for set_guards is\n");
    CfMsg::DPrint (MSG_ERR, "          - id is peid\n");
    CfMsg::DPrint (MSG_ERR, "            peg(PEG_PEn), pegcapm(PEGCAP_M_PEn), pegcaps(PEGCAP_S_PEn),\n");
    CfMsg::DPrint (MSG_ERR, "            crgcapp(CRGCAP_PEn)\n");
    CfMsg::DPrint (MSG_ERR, "          - id is clid\n");
    CfMsg::DPrint (MSG_ERR, "            crg(CRG_CLm), crgcapch(CRGCAP_CRAMHCLm), crgcapcl(CRGCAP_CRAMLCLm),\n");
    CfMsg::DPrint (MSG_ERR, "            crgcapsh(CRGCAP_SX2MBHCLm), crgcapsl(CRGCAP_SX2MBLCLm)\n");
    CfMsg::DPrint (MSG_ERR, "            crgcsg(CRG_CSG_CLm), crgivc(CRG_IVC_CLm)\n");
    CfMsg::DPrint (MSG_ERR, "    set_guards_param (\"guard_name\", id, \"param_name\", value)  : Set parameters for guard.\n");
    CfMsg::DPrint (MSG_ERR, "        valid guard_name is\n");
    CfMsg::DPrint (MSG_ERR, "            crg : CRAM guard. id is clid then.\n");
    CfMsg::DPrint (MSG_ERR, "        valid param_name is\n");
    CfMsg::DPrint (MSG_ERR, "            crgaddrw   : Address width for crg\n");
    CfMsg::DPrint (MSG_ERR, "            crgcrgch   : The number of ch for crg in crg\n");
    CfMsg::DPrint (MSG_ERR, "            crgcsgch   : The number of ch for csg in crg\n");
    CfMsg::DPrint (MSG_ERR, "            crgkeycode : Keycode for crg\n");
    CfMsg::DPrint (MSG_ERR, "    set_snooze (period [,contextid])      : Set snooze period.\n");
    CfMsg::DPrint (MSG_ERR, "    get_snooze ([contextid])              : Get snooze period.\n");
    CfMsg::DPrint (MSG_ERR, "    set_mpunum (num_region [,contextid])  : Set number of MPU region.\n");
    CfMsg::DPrint (MSG_ERR, "    get_mpunum ([contextid])              : Get number of MPU region.\n");
    CfMsg::DPrint (MSG_ERR, "    set_mpubank ({1, 2} [,contextid])     : Set number of MPU bank (1 or 2). \n");
    CfMsg::DPrint (MSG_ERR, "    get_mpubank ([contextid])             : Get number of MPU bank.\n");
    CfMsg::DPrint (MSG_ERR, "    set_mpudmdp (data [,contextid])       : Set initial value of MPCFG DMDP\n");
    CfMsg::DPrint (MSG_ERR, "    set_peid (data ,contextid)            : Set initial value of PEID register\n");
    CfMsg::DPrint (MSG_ERR, "    get_peid (contextid)                  : Get initial value of PEID register\n");
    CfMsg::DPrint (MSG_ERR, "    set_bmid (data ,contextid)            : Set initial value of BMID register\n");
    CfMsg::DPrint (MSG_ERR, "    get_bmid (contextid)                  : Get initial value of BMID register\n");
    CfMsg::DPrint (MSG_ERR, "    set_spid (data ,contextid)            : Set initial value of SPID register\n");
    CfMsg::DPrint (MSG_ERR, "    get_spid (contextid)                  : Get initial value of SPID register\n");
    CfMsg::DPrint (MSG_ERR, "    set_spidlist (data [,contextid])      : Set initial value of SPIDLIST register\n");
    CfMsg::DPrint (MSG_ERR, "    get_spidlist (contextid)              : Get initial value of SPIDLIST register\n");
    CfMsg::DPrint (MSG_ERR, "    set_rbase (data [,contextid])         : Set initial value of RBASE.\n");
    CfMsg::DPrint (MSG_ERR, "    get_rbase (contextid)                 : Get initial value of RBASE.\n");
    CfMsg::DPrint (MSG_ERR, "    set_lscfg (data [,contextid])         : Set initial value of LSCFG.\n");
    CfMsg::DPrint (MSG_ERR, "    set_hvcfg (data [,contextid])         : Set initial value of HVCFG.\n");
    CfMsg::DPrint (MSG_ERR, "    set_dbgen (data [,contextid])         : Set initial value of DBGEN.\n");
    CfMsg::DPrint (MSG_ERR, "    get_dbgen ([contextid])               : Get initial value of DBGEN.\n");
    CfMsg::DPrint (MSG_ERR, "    set_icctrl (data [,contextid])        : Set initial value of ICCTRL for ICHEN bit.\n");
    CfMsg::DPrint (MSG_ERR, "    get_icctrl ([contextid])              : Get initial value of ICCTRL for ICHEN bit.\n");
    CfMsg::DPrint (MSG_ERR, "    set_icsize (size, way [,contextid])   : Set ICache size. size(KB)=[0|1|2|4|8|16|32|64], way=[0|1|2|4]\n");
    CfMsg::DPrint (MSG_ERR, "    get_icsize ([contextid])              : Get ICache size. size(KB)=[0|1|2|4|8|16|32|64], way=[0|1|2|4]\n");
    CfMsg::DPrint (MSG_ERR, "    set_intc1 (peid, address)             : Sets intc1 address.\n");
    CfMsg::DPrint (MSG_ERR, "    set_intc2 (address)                   : Sets intc2 address.\n");
    CfMsg::DPrint (MSG_ERR, "    set_req_eiint_to_intc1 (peid, address): Sets int req eiint to intc1\n");
    CfMsg::DPrint (MSG_ERR, "    set_req_feint_to_intc1 (peid, address): Sets int req feint to intc1\n");
    CfMsg::DPrint (MSG_ERR, "    setclk (ratio, [contextid])           : Set clock ratio. ratio=0:stop PE. others=active PE. 1 is the fastest clock\n");
    CfMsg::DPrint (MSG_ERR, "    getclk ([contextid])                  : Get clock ratio. ratio=0:stop PE. others=active PE. 1 is the fastest clock\n");
    CfMsg::DPrint (MSG_ERR, "    set_has_fpu (\"{on|off}\")              : Set default setting for FPU existance\n");
    CfMsg::DPrint (MSG_ERR, "    set_has_fxu (\"{on|off}\")              : Set default setting for FXU existance\n");
    CfMsg::DPrint (MSG_ERR, "    set_has_mpu (\"{on|off}\")              : Set default setting for MPU existance\n");
    CfMsg::DPrint (MSG_ERR, "    set_fpu_existence (data, [contextid]) : Set FPU existance for designated PE\n");
    CfMsg::DPrint (MSG_ERR, "    set_fxu_existence (data, [contextid]) : Set FXU existance for designated PE\n");
    CfMsg::DPrint (MSG_ERR, "    set_mpu_existence (data, [contextid]) : Set MPU existance for designated PE\n");
    CfMsg::DPrint (MSG_ERR, "    set_has_hv (\"{on|off}\")               : Set Virtualization existance\n");
    CfMsg::DPrint (MSG_ERR, "    set_l1rlinknum (num [,contextid])     : Set Num of L1RAM Link resource.\n");
    CfMsg::DPrint (MSG_ERR, "    get_l1rlinknum ([contextid])          : Get Num of L1RAM Link resource.\n");
    CfMsg::DPrint (MSG_ERR, "    set_l1rsize (num [,contextid])        : Set L1R size [KB] for L1RCFG.L1RSIZE. No relation with actual allocated memory size by ms* commands.\n");
    CfMsg::DPrint (MSG_ERR, "    get_l1rsize ([contextid])             : Get L1R size [KB]. No relation with actual allocated memory size by ms* commands.\n");
    CfMsg::DPrint (MSG_ERR, "- Etc\n");
    CfMsg::DPrint (MSG_ERR, "    help ()                   : Show this message.\n");
    CfMsg::DPrint (MSG_ERR, "    last_peid ()              : Get last PEID.\n");
    CfMsg::DPrint (MSG_ERR, "    is_valid_peid (peid)      : Check peid valid. If valid, return True\n");
    CfMsg::DPrint (MSG_ERR, "    {quit|q} ()               : Quit CForest.\n");
    CfMsg::DPrint (MSG_ERR, "    peinfo ()                 : Dump information for each PE.\n");
    CfMsg::DPrint (MSG_ERR, "    set_msglvl (\"level\")      : Set message level {ERR|WAR|EXP|INF}\n");
    CfMsg::DPrint (MSG_ERR, "    get_msglvl ()             : Get message level\n");
    CfMsg::DPrint (MSG_ERR, "    use_stopio ()             : Enable access break for STOPIO\n");

    return Py_BuildValue ("");
}


PyObject* PythonAPI::DisplayInstructionCountPy (PyObject* self, PyObject* args)
{
#ifdef ENABLE_PIC
    char* filename;
    int peid, tcid;

    PyArg_ParseTuple (args, "sii", &filename, &peid, &tcid);

    if (IsPeidValid (peid) == false) {
      CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
        return Py_BuildValue ("i", PY_NG);
    }

    std::string fname = filename;
    FILE* local_file = stdout;
    if (fname.empty () == false) {
        local_file = fopen (fname.c_str (), "w");
        if (local_file == NULL) {
            CfMsg::DPrint (MSG_ERR, "<Error: Can't open %s\n", filename);
            return Py_BuildValue ("i", PY_NG);
        }
    }

    m_forest_util->DisplayInstructionCount (peid, local_file);
    if (local_file != stdout) {
        fclose (local_file);
    }
#else // ENABLE_PIC
    CfMsg::DPrint (MSG_ERR, "<Warning: pic command isn't supported>\n");
#endif // ENABLE_PIC
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::ClearInstructionCountPy (PyObject* self, PyObject* args)
{
#ifdef ENABLE_PIC
    int peid, tcid;

    PyArg_ParseTuple (args, "ii", &peid, &tcid);

    if (IsPeidValid (peid) == false) {
      CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
        return Py_BuildValue ("i", PY_NG);
    }

    m_forest_util->ClearInstructionCount (peid);
#else // ENABLE_PIC
    CfMsg::DPrint (MSG_ERR, "<Warning: picc command isn't supported>\n");
#endif // ENABLE_PIC
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::SetDebugModePy (PyObject* self, PyObject* args)
{
    char* mode;
    int peid, tcid;

    PyArg_ParseTuple (args, "sii", &mode, &peid, &tcid);

    std::string mode_str = mode;
    DebugMode dbg_mode;
    if (mode_str == "on") {
        if (m_cmdopt->GetDebugMode () == DBG_OFF) {
            m_cmdopt->SetDebugMode (DBG_ON);
        }
        dbg_mode = m_cmdopt->GetDebugMode ();
    } else if (mode_str == "off") {
        dbg_mode = DBG_OFF;
    }
#if defined(COMP_RUNNER)
    else if (mode_str == "pipe") {
        dbg_mode = DBG_PIPE;
    }
#endif // defined(COMP_RUNNER)
    else {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid mode (%s)>\n", mode);
        return Py_BuildValue ("i", PY_NG);
    }

    if ( (peid == PY_NOSEL_ID) && (tcid == PY_NOSEL_ID) ) {
        // 全部PY_NOSEL_IDならContextID指定なしなので、全PE/TCが対象
        m_forest_util->SetDebugMode (dbg_mode);
    } else {
        // 指定された単一のTCのデバッグモードを設定
        if (IsPeidValid (peid) == false) {
          CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
            return Py_BuildValue ("i", PY_NG);
        }
        m_forest_util->SetTcDebugMode (peid, dbg_mode);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::SetLogFilePy (PyObject* self, PyObject* args)
{
    char* filename;
    int is_multi_log;
    PyArg_ParseTuple (args, "si", &filename, &is_multi_log);

    std::string fname = filename;

    // CfMsg::DPrint(共用）ログファイル
    FILE* fp = m_forest_util->OpenFileOut (fname);
    if (fp == NULL) {
        return Py_BuildValue ("i", PY_NG);
    }

    // CfMsg::TPrint(スレッド別）ログファイル
    if ((is_multi_log == 1) && (fname.empty () == false)) {
        if (m_forest_util->OpenSeparatedFileOut (fname) == false) {
            return Py_BuildValue ("i", PY_NG);
        }
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::UpdateExpMsgEnablePy (PyObject* self, PyObject* args)
{
    char* mode;
    unsigned int start_cause;
    unsigned int end_cause;
    int peid, tcid;

    PyArg_ParseTuple (args, "sIIii", &mode, &start_cause, &end_cause, &peid, &tcid);

    std::string mode_str = mode;
    bool is_enable;
    if (mode_str == "on") {
        is_enable = true;
    } else if (mode_str == "off") {
        is_enable = false;
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid mode (%s)>\n", mode);
        return Py_BuildValue ("i", PY_NG);
    }

    if ( (peid == PY_NOSEL_ID) && (tcid == PY_NOSEL_ID) ) {
        // 全部-1ならContextID指定なしなので、全PE/TCが対象
        uint32_t sc_num = m_forest_util->GetScNum ();
        for (uint32_t scid = 0; scid < sc_num; scid ++) {
            uint32_t  lpeid = m_forest_util->GetPeId (scid);
            m_forest_util->UpdateExpMsgRange (lpeid, start_cause, end_cause, is_enable);
        }

    } else {
        // 指定された単一のTCの例外メッセージ出力を設定
        if (IsPeidValid (peid) == false) {
          CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
            return Py_BuildValue ("i", PY_NG);
        }
        m_forest_util->UpdateExpMsgRange (peid, start_cause, end_cause, is_enable);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::ConnectGDBPy (PyObject* self, PyObject* args)
{
    unsigned short port;

    PyArg_ParseTuple (args, "H", &port);

    if (m_cmdopt->IsSetGdbCmdopt () == true) {
        // GDBを指定済みなら指定済みGDBポート番号で接続
        port = m_cmdopt->GetGdbPort ();
        CfMsg::DPrint (MSG_WAR, "<Warning: Designated GDB port is overwritten by --gdb>\n");
    }

#ifdef ENABLE_GDB
    GdbStub* gdb_stub = new GdbStub (m_forest_util, m_cmdopt);
    gdb_stub->ConnectGDB (port);
    delete gdb_stub;
#endif // ENABLE_GDB

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::ConnectUARTPy (PyObject* self, PyObject* args)
{
    unsigned short port;
    int channel;

    PyArg_ParseTuple (args, "Hi", &port, &channel);

    if (m_cmdopt->IsSetUartCmdopt () == true) {
        CfMsg::DPrint (MSG_WAR, "<Warning: UART is set by --uart already>\n");
        return Py_BuildValue ("i", PY_NG);
    }

#ifdef ENABLE_UART
    m_forest_util->SetUartSocketPort (port);
    m_forest_util->SetUartChannelNum (channel);
    if (m_forest_util->CreateUart () == true) {
        m_forest_util->SetIsUartInstance (true);
        return Py_BuildValue ("i", PY_OK);
    }
#endif // ENABLE_UART
    return Py_BuildValue ("i", PY_NG);
}


// HTの情報を表示する
PyObject* PythonAPI::DisplayThreadInfoPy (PyObject* self, PyObject* args)
{
    //simso.DisplayThreadInfoPy ()\n"

    CfMsg::DPrint (MSG_ERR, "<Current status>\n");
    CfMsg::DPrint (MSG_ERR, " PE  PC          STATUS        INST     CYCLE\n");

    uint32_t sc_num = m_forest_util->GetScNum ();
    for (uint32_t scid = 0; scid < sc_num; scid ++) {
        uint32_t peid = m_forest_util->GetPeId (scid);

        // TCのPCの値
        RegData64 pc_data = 0;
        m_forest_util->RegReadDebug (peid, PC, &pc_data);

        // PEID,VCID,TCID,PC表示
        std::ostringstream output_stream;
        output_stream << std::setw(3) << peid << "  ";
        output_stream << "0x" << std::hex << std::setfill('0') << std::setw(8) << pc_data;
        output_stream << std::dec << std::setfill(' ');

        // TCの状態を表示
        const TraceInfo* trace = m_forest_util->GetTraceInfo (scid);

        std::string status_str = "ERR"; // 未対応の値が来たらエラー
        switch (trace->sim_state) {
        case SIMULATE_HALT:     status_str = "HALT";    break;
        case SIMULATE_STOP:     status_str = "STOP";    break;
        case SIMULATE_SNOOZE:   status_str = "SNOOZE";  break;
        case SIMULATE_DISABLE:  status_str = "DISABLE"; break;
        case SIMULATE_ON:       status_str = "RUN";     break;
        case SIMULATE_RESET:    status_str = "RESET";   break; // 来ないはず
        }
        output_stream << std::left << "  " << std::setw(8) << status_str;
        output_stream << std::right;

        // TCの命令数を表示
        output_stream << "  " << std::setw(8) << trace->step_count;

        // CSモードならTCのサイクル数を表示
        output_stream << "  " << std::setw(8) << trace->stage_cycle[STAGE_DP];

        CfMsg::DPrint (MSG_ERR, "%s\n", output_stream.str().c_str());
    }

    return Py_BuildValue ("i", PY_OK);
}


// PE内の最大TCIDを取得
// 全スレッドにコマンドを発行する際、あった方が楽なので用意
PyObject* PythonAPI::GetLastTcIdPy (PyObject* self, PyObject* args)
{
    int peid;

    //return simso.GetLastTcIdPy (select_pe)\n"
    PyArg_ParseTuple (args, "i", &peid);

    int32_t last_tc_id;
    if (IsPeidValid (peid) == true) {
        // 有効なPEが指定されていたらPE内の最大TCIDを返す
        last_tc_id = m_forest_util->GetTcLastId (peid);
    } else {
        last_tc_id = -1; // Invalid PEID
        CfMsg::DPrint (MSG_ERR, "Invalid PE\n");
    }

    return Py_BuildValue ("i", last_tc_id);
}


// 最大PEIDを取得
PyObject* PythonAPI::GetLastPeIdPy (PyObject* self, PyObject* args)
{
    unsigned int last_pe_id = m_forest_util->GetPeLastId ();
    return Py_BuildValue ("I", last_pe_id);
}


PyObject* PythonAPI::IsValidPeIdPy (PyObject* self, PyObject* args)
{
    int peid;

    PyArg_ParseTuple (args, "i", &peid);

    if (IsPeidValid (peid)) {
        return Py_BuildValue ("i", 1);
    } else {
        return Py_BuildValue ("i", 0);
    }
}


// スレッド数を取得
// 全スレッドにコマンドを発行する際、あった方が楽なので用意
PyObject* PythonAPI::SetFrequencyPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int ratio;

    // simso.SetFrequencyPy (ratio, pe)
    PyArg_ParseTuple (args, "Ii", &ratio, &peid);

    if (IsPeidValid (peid) == false) {
      CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    m_forest_util->SetFrequency (peid, ratio);
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetFrequencyPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int ratio;

    PyArg_ParseTuple (args, "i", &peid);

    if (IsPeidValid (peid) == false) {
      CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    ratio = m_forest_util->GetFrequency (peid);
    return Py_BuildValue ("I", ratio);
}


PyObject* PythonAPI::SetSnoozePeriodPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "Ii", &data, &peid);

    if (peid == PY_NOSEL_ID) {
        // PEID指定無しなら全PEが対象
        for (uint32_t lpeid = 0; lpeid <= m_forest_util->GetPeLastId (); lpeid++) {
            if (IsPeidValid (lpeid)) {
                m_forest_util->SetExtPin (lpeid, EXT_PIN_SNZ_PERIOD, data & 0xFFFF);
            }
        }
    } else {
        if (IsPeidValid (peid) == false) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
            return Py_BuildValue ("i", PY_NG); // エラー
        } else {
            m_forest_util->SetExtPin (peid, EXT_PIN_SNZ_PERIOD, data & 0xFFFF);
        }
    }

    return Py_BuildValue("i", PY_OK);
}


PyObject* PythonAPI::GetSnoozePeriodPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "i", &peid);

    if (IsPeidValid (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    data = m_forest_util->GetExtPin (peid, EXT_PIN_SNZ_PERIOD);

    return Py_BuildValue("I", data);
}


PyObject* PythonAPI::SetTlbEntryPy (PyObject* self, PyObject* args)
{
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetTlbEntryPy (PyObject* self, PyObject* args)
{
    unsigned int data = 0;
    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetMinTlbSizePy (PyObject* self, PyObject* args)
{
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetMinTlbSizePy (PyObject* self, PyObject* args)
{
    unsigned int data = 0;
    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetMpuDefaultPy (PyObject* self, PyObject* args)
{
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetMpuDefaultPy (PyObject* self, PyObject* args)
{
    unsigned int data = 0;
    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetMpuRegionNumPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "Ii", &data, &peid);

    if (data == 0 || data > 32) {
        // MPU領域数は1-32 (0ならHasMpu=Falseを期待)
        CfMsg::DPrint (MSG_ERR,
                       "<Error: the num of MPU region is invalid (%u). Valid range is 1-32>\n", data);
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    if (peid == PY_NOSEL_ID) {
        // PEID指定無しなら全PEが対象
        for (uint32_t lpeid = 0; lpeid <= m_forest_util->GetPeLastId (); lpeid++) {
            if (IsPeidValid (lpeid)) {
                m_forest_util->SetExtPin (lpeid, EXT_PIN_MPTRGN, data);
            }
        }
    } else {
        if (IsPeidValid (peid) == false) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
            return Py_BuildValue ("i", PY_NG); // エラー
        } else {
            m_forest_util->SetExtPin (peid, EXT_PIN_MPTRGN, data);
        }
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetMpuRegionNumPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "i", &peid);

    if (IsPeidValid (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    data = m_forest_util->GetExtPin (peid, EXT_PIN_MPTRGN);

    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetMpuBankNumPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "Ii", &data, &peid);

    if (data < 1 || data > 2) {
        // Supported num of bank is 1 or 2 (1 is no bank)
        CfMsg::DPrint (MSG_ERR,
                       "<Error: the num of MPU bank is invalid (%u). Valid range is 1-2>\n", data);
        return Py_BuildValue ("i", PY_NG); // Error
    }

    if (peid == PY_NOSEL_ID) {
        // When PEID isn't designated, target is all PE
        for (uint32_t lpeid = 0; lpeid <= m_forest_util->GetPeLastId (); lpeid++) {
            if (IsPeidValid (lpeid)) {
                m_forest_util->SetExtPin (lpeid, EXT_PIN_MPBANK, data);
            }
        }
    } else {
        if (IsPeidValid (peid) == false) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
            return Py_BuildValue ("i", PY_NG); // error
        } else {
            m_forest_util->SetExtPin (peid, EXT_PIN_MPBANK, data);
        }
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetMpuBankNumPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "i", &peid);

    if (IsPeidValid (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    data = m_forest_util->GetExtPin (peid, EXT_PIN_MPBANK);

    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetPeidPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "Ii", &data, &peid);

    if (IsPeidValid (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG); // エラー
    } else {
        m_forest_util->SetExtPin (peid, EXT_PIN_PEID, data);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetPeidPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "i", &peid);

    if (IsPeidValid (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    data = m_forest_util->GetExtPin (peid, EXT_PIN_PEID);

    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetSpidPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "Ii", &data, &peid);

    if (IsPeidValid (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG); // エラー
    } else {
        m_forest_util->SetExtPin (peid, EXT_PIN_SPID, data);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetSpidPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "i", &peid);

    if (IsPeidValid (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    data = m_forest_util->GetExtPin (peid, EXT_PIN_SPID);

    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetSpidlistPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "Ii", &data, &peid);

    if (peid == PY_NOSEL_ID) {
        // PEID指定無しなら全PEが対象
        for (uint32_t lpeid = 0; lpeid <= m_forest_util->GetPeLastId (); lpeid++) {
            if (IsPeidValid (lpeid)) {
                m_forest_util->SetExtPin (lpeid, EXT_PIN_SPIDLIST, data);
            }
        }
    } else {
        if (IsPeidValid (peid) == false) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
            return Py_BuildValue ("i", PY_NG); // エラー
        } else {
            m_forest_util->SetExtPin (peid, EXT_PIN_SPIDLIST, data);
        }
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetSpidlistPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "i", &peid);

    if (IsPeidValid (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    data = m_forest_util->GetExtPin (peid, EXT_PIN_SPIDLIST);

    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetBmidPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "Ii", &data, &peid);

    if (IsPeidValid (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG); // エラー
    } else {
        m_forest_util->SetExtPin (peid, EXT_PIN_BMID, data);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetBmidPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "i", &peid);

    if (IsPeidValid (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    data = m_forest_util->GetExtPin (peid, EXT_PIN_BMID);

    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetRbasePy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "Ii", &data, &peid);

    if (peid == PY_NOSEL_ID) {
        // PEID指定無しなら全PEが対象
        for (uint32_t lpeid = 0; lpeid <= m_forest_util->GetPeLastId (); lpeid++) {
            if (IsPeidValid (lpeid)) {
                m_forest_util->SetExtPin (lpeid, EXT_PIN_RBASE, data >> 9);
                m_forest_util->SetExtPin (lpeid, EXT_PIN_DV,    (data >> 1) & 0x1);
                m_forest_util->SetExtPin (lpeid, EXT_PIN_RINT,  data & 0x1);
            }
        }
    } else {
        if (IsPeidValid (peid) == false) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
            return Py_BuildValue ("i", PY_NG); // エラー
        } else {
            m_forest_util->SetExtPin (peid, EXT_PIN_RBASE, data >> 9);
            m_forest_util->SetExtPin (peid, EXT_PIN_DV,    (data >> 1) & 0x1);
            m_forest_util->SetExtPin (peid, EXT_PIN_RINT,  data & 0x1);
        }
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetRbasePy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "i", &peid);

    if (IsPeidValid (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    uint32_t rbase = m_forest_util->GetExtPin (peid, EXT_PIN_RBASE) & 0x7FFFFF;
    uint32_t dv    = m_forest_util->GetExtPin (peid, EXT_PIN_DV)    & 0x1;
    uint32_t rint  = m_forest_util->GetExtPin (peid, EXT_PIN_RINT)  & 0x1;

    data = (rbase << 9)
         | (dv << 1)
         | rint;

    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetMctlPy (PyObject* self, PyObject* args)
{
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetMctlPy (PyObject* self, PyObject* args)
{
    unsigned int data = 0;
    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetIcctrlPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "Ii", &data, &peid);

    if (peid == PY_NOSEL_ID) {
        // PEID指定無しなら全PEが対象
        for (uint32_t lpeid = 0; lpeid <= m_forest_util->GetPeLastId (); lpeid++) {
            if (IsPeidValid (lpeid)) {
                m_forest_util->SetExtPin (lpeid, EXT_PIN_ICHEN, data & 1);
            }
        }
    } else {
        if (IsPeidValid (peid) == false) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
            return Py_BuildValue ("i", PY_NG); // エラー
        } else {
            m_forest_util->SetExtPin (peid, EXT_PIN_ICHEN, data & 1);
        }
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetIcctrlPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "i", &peid);

    if (IsPeidValid (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    data = m_forest_util->GetExtPin (peid, EXT_PIN_ICHEN);

    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetIcSizePy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int size;
    unsigned int way;
    PyArg_ParseTuple (args, "IIi", &size, &way, &peid);

    if (IsCacheSizeValid (size, way) == false) {
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    ByteSize ic_size = size * 1024;
    if (peid == PY_NOSEL_ID) {
        // PEID指定無しなら全PEが対象
        for (uint32_t lpeid = 0; lpeid <= m_forest_util->GetPeLastId (); lpeid++) {
            if (IsPeidValid (lpeid)) {
                m_forest_util->SetIcacheSize (lpeid, ic_size);
                m_forest_util->SetIcacheWayNum (lpeid, way);
                m_forest_util->SetIcacheEccState (lpeid, true);
            }
        }
    } else {
        if (IsPeidValid (peid) == false) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
            return Py_BuildValue ("i", PY_NG); // エラー
        } else {
            m_forest_util->SetIcacheSize (peid, ic_size);
            m_forest_util->SetIcacheWayNum (peid, way);
            m_forest_util->SetIcacheEccState (peid, true);
        }
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetIcSizePy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int size;
    unsigned int way;
    PyArg_ParseTuple (args, "i", &peid);

    if (IsPeidValid (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    size = m_forest_util->GetIcacheSize (peid) / 1024;
    way  = m_forest_util->GetIcacheWayNum (peid);

    return Py_BuildValue ("II", size, way);
}


PyObject* PythonAPI::SetDcctrlPy (PyObject* self, PyObject* args)
{
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetDcctrlPy (PyObject* self, PyObject* args)
{
    unsigned int data = 0;
    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetDcSizePy (PyObject* self, PyObject* args)
{
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetDcSizePy (PyObject* self, PyObject* args)
{
    unsigned int data = 0;
    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetMpuDmdpPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "Ii", &data, &peid);

    if (peid == PY_NOSEL_ID) {
        // PEID指定無しなら全PEが対象
        for (uint32_t lpeid = 0; lpeid <= m_forest_util->GetPeLastId (); lpeid++) {
            if (IsPeidValid (lpeid)) {
                m_forest_util->SetExtPin (lpeid, EXT_PIN_MDP_DMDP, data & 0x1);
            }
        }
    } else {
        if (IsPeidValid (peid) == false) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
            return Py_BuildValue ("i", PY_NG); // エラー
        } else {
            m_forest_util->SetExtPin (peid, EXT_PIN_MDP_DMDP, data & 0x1);
        }
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetMpuDmdpPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "i", &peid);

    if (IsPeidValid (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    data = m_forest_util->GetExtPin (peid, EXT_PIN_MDP_DMDP) & 0x1;

    return Py_BuildValue ("I", data);
}


bool PythonAPI::IsCoreTypeCombinationValid (CoreType core_type)
{
    // Restrict the combination of different SPID size
    // Temporally support 5bit and 2bit mixed.
    // 5bit : CCG3M30, G3MH
    // 2bit : G3M10, G3M20, G3KH
    // Special : G4P(has clid)
    std::vector<PEInfo>::const_iterator it_pe = m_pe_info.begin ();
    while (it_pe != m_pe_info.end ()) {
        switch (core_type) {
        case CORE_G4MH:
        case CORE_G4KH:
        default:
            break; // ここにはこない
        }
        ++ it_pe;
    }

    return true;
}


PyObject* PythonAPI::SetPeInfoPy (PyObject* self, PyObject* args)
{
    unsigned int peid;
    unsigned int clid;
    char* core_type_char;

    PyArg_ParseTuple (args, "IIs", &peid, &clid, &core_type_char);

    std::string core_type_str = core_type_char;
    CoreType    core_type;
    CoreVersion core_version;
    if (core_type_str == "g4mh" || core_type_str == "g4mh10" || core_type_str == "g4mh1.0") {
        core_type    = CORE_G4MH;
        core_version = CORE_VER_10;
    } else if (core_type_str == "g4mh11" || core_type_str == "g4mh1.1") {
        core_type    = CORE_G4MH;
        core_version = CORE_VER_11;
    } else if (core_type_str == "g4mh20" || core_type_str == "g4mh2.0") {
        core_type    = CORE_G4MH;
        core_version = CORE_VER_20;
    } else if (core_type_str == "g4mh21" || core_type_str == "g4mh2.1") {
        core_type    = CORE_G4MH;
        core_version = CORE_VER_21;
    } else if (core_type_str == "g4kh" || core_type_str == "g4kh10" || core_type_str == "g4kh1.0") {
        core_type    = CORE_G4KH;
        core_version = CORE_VER_10;
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid core type : %s>\n", core_type_str.c_str());
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    if (IsPEInfoValid (peid, clid, core_type) == false) {
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    // SPID幅の異なるCoreTypeの混在は禁止
    if (IsCoreTypeCombinationValid (core_type) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Unsupported combination with different SPID width>\n");
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    // PEID多重定義チェック
    std::vector<uint32_t>::iterator it
        = std::find (m_peid_vec.begin (), m_peid_vec.end (), peid);
    if (it != m_peid_vec.end ()) {
        CfMsg::DPrint (MSG_ERR, "<Error: Already registered PEID:%u>\n", peid);
        return Py_BuildValue ("i", PY_NG); // エラー
    }
    m_peid_vec.push_back (peid);

    // GeneratePePy用にPE情報を記録
    PEInfo peinfo = {peid, clid, core_type, core_version};
    m_pe_info.push_back (peinfo);

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::IsGenPePy (PyObject* self, PyObject* args)
{
    if (IsGenPe ()) {
        return Py_BuildValue ("i", 1);
    } else {
        return Py_BuildValue ("i", 0);
    }
}


PyObject* PythonAPI::GeneratePePy (PyObject* self, PyObject* args)
{
    if (IsGenPe () == true) {
        // 生成済み
        return Py_BuildValue("");
    }

    if (m_pe_info.empty () == true) {
        // SetPeInfoで情報が登録されていないので、デフォルト値でPEを生成
        uint32_t pe_num = m_cmdopt->GetPeNumFromDbg ();
        CoreType core_type = m_cmdopt->GetCoreType ();
        CoreVersion core_version = m_cmdopt->GetCoreVersion ();
        for (uint32_t peid = 0; peid <= pe_num; peid++) {
            // TODO : CL0 is supported for E2xFCC1
            m_forest_util->SetPe (0, peid, core_type, core_version);
        }
    } else {
        // SetPeInfo で登録された情報を元にPEIDを生成
        std::vector<PEInfo>::const_iterator it = m_pe_info.begin ();
        while (it != m_pe_info.end ()) {
            m_forest_util->SetPe ((*it).clid, (*it).peid, (*it).core_type, (*it).core_version);
            ++ it;
        }
    }

    // PE作成以降のデフォルト設定は共通関数に任せる
    m_forest_setup->DefaultSetup ();

    SetGenPe (true);

    return Py_BuildValue("");
}


// 現状必要性がないので、他PEのROM領域をキャッシング不可にする構成はCForest非サポート
PyObject* PythonAPI::SetRomAreaPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;
    unsigned int end_addr;
    PyArg_ParseTuple (args, "II", &start_addr, &end_addr);

    if (start_addr > end_addr) {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid range>\n");
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    m_rom_start_addr = start_addr;
    m_rom_end_addr = end_addr;

    m_forest_util->SetRomArea (start_addr, end_addr);
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::SetLocalRamAreaPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;
    unsigned int end_addr;
    PyArg_ParseTuple (args, "II", &start_addr, &end_addr);

    if (start_addr > end_addr) {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid range>\n");
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    m_forest_util->SetLocalRamArea (start_addr, end_addr);
    return Py_BuildValue ("i", PY_OK);
}

PyObject* PythonAPI::Intc1SetPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;
    unsigned int peid;

    PyArg_ParseTuple (args, "II", &peid, &start_addr);

    if (IsPeidValid (peid) == false) {
      CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
        return Py_BuildValue ("");
    }
    if ((start_addr & 3) != 0) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid address (unaligned).>\n");
        return Py_BuildValue ("");
    }
    m_forest_util->SetIntc1(peid, start_addr);
    return Py_BuildValue("");
}

PyObject* PythonAPI::Intc2SetPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;

    PyArg_ParseTuple (args, "I", &start_addr);

    if ((start_addr & 3) != 0) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid address (unaligned).>\n");
        return Py_BuildValue ("");
    }
    m_forest_util->SetIntc2(start_addr);
    return Py_BuildValue("");
}

PyObject* PythonAPI::ReqEIINTfromexternalPy (PyObject* self, PyObject* args)
{
    unsigned int channel;
    unsigned int peid;

    PyArg_ParseTuple (args, "II", &peid, &channel);

    if (IsPeidValid (peid) == false) {
      CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
        return Py_BuildValue ("");
    }
    if (channel < 32){
        m_forest_util->ReqInterruptToIntc1(peid, channel);
    }else{
        m_forest_util->ReqInterruptToIntc2(channel);
    }
    return Py_BuildValue("");
}

PyObject* PythonAPI::ReqFEINTfromexternalPy (PyObject* self, PyObject* args)
{
    unsigned int channel;
    unsigned int peid;

    PyArg_ParseTuple (args, "II", &peid, &channel);

    if (IsPeidValid (peid) == false) {
      CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
        return Py_BuildValue ("");
    }
    if (channel > 32) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid channel.>\n");
        return Py_BuildValue ("");
    }
    m_forest_util->ReqFeintToIntc1(peid, channel);
    return Py_BuildValue("");
}


PyObject* PythonAPI::OstmLocalSetPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;
    unsigned int bind_pe, ostmch, intch;

    PyArg_ParseTuple (args, "IIII", &bind_pe, &ostmch, &start_addr, &intch);

    if (IsPeidValid (bind_pe) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid.\n");
        return Py_BuildValue ("i", PY_NG); // エラー
    }
    if ((start_addr & 3) != 0) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid address (unaligned).>\n");
        return Py_BuildValue ("i", PY_NG); // エラー
    }
    if (intch > INTC_CH_MAX) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid intch.>\n");
        return Py_BuildValue ("i", PY_NG); // エラー
    }
    if (m_forest_util->SetOstmLocal(bind_pe, ostmch, start_addr, intch) == false) {
        return Py_BuildValue ("i", PY_NG); // エラー
    }
    return Py_BuildValue("i", PY_OK);
}


PyObject* PythonAPI::OstmGlobalSetPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;
    unsigned int intch;

    PyArg_ParseTuple (args, "II", &start_addr, &intch);

    if ((start_addr & 3) != 0) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid address (unaligned).>\n");
        return Py_BuildValue ("i", PY_NG); // エラー
    }
    if (intch > INTC_CH_MAX) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid intch.>\n");
        return Py_BuildValue ("i", PY_NG); // エラー
    }
    if (m_forest_util->SetOstmGlobal(start_addr, intch) == false) {
        return Py_BuildValue ("i", PY_NG); // エラー
    }
    return Py_BuildValue("i", PY_OK);
}


PyObject* PythonAPI::SetOstmClkPy (PyObject* self, PyObject* args)
{
    unsigned int ratio;

    PyArg_ParseTuple (args, "I", &ratio);

    if (ratio == 0) {
        CfMsg::DPrint (MSG_ERR, "<Error: clock ratio must be 1 and more>\n", ratio);
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    m_forest_util->SetClkRatioOstm (ratio);
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetOstmClkPy (PyObject* self, PyObject* args)
{
    unsigned int ratio;

    ratio = m_forest_util->GetClkRatioOstm ();
    return Py_BuildValue ("I", ratio);
}


PyObject* PythonAPI::SetDcAreaPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr1;
    unsigned int end_addr1;
    unsigned int start_addr2;
    unsigned int end_addr2;
    PyArg_ParseTuple (args, "IIII", &start_addr1, &end_addr1, &start_addr2, &end_addr2);

    if ((start_addr1 > end_addr1) || (start_addr2 > end_addr2)) {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid range>\n");
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    m_forest_util->SetDCacheArea (start_addr1, end_addr1, start_addr2, end_addr2);
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetDcAreaPy (PyObject* self, PyObject* args)
{
    std::ostringstream dc_area;
    PhysAddr dc_area_addr [4];

    m_forest_util->GetDCacheArea (dc_area_addr);
    dc_area << " Start addr 1: ";
    dc_area << dc_area_addr [0];
    dc_area << " End addr 1: ";
    dc_area << dc_area_addr [1];
    dc_area << " Start addr 2: ";
    dc_area << dc_area_addr [2];
    dc_area << " End addr 2: ";
    dc_area << dc_area_addr [3];

    return Py_BuildValue ("s", dc_area.str().c_str());
}


PyObject* PythonAPI::SetDebugAreaPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;
    unsigned int end_addr;
    int peid;
    PyArg_ParseTuple (args, "IIi", &start_addr, &end_addr, &peid);

    if (start_addr > end_addr) {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid range>\n");
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    if (IsPeidValid (peid) == false) {
      CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
        return Py_BuildValue ("i", PY_NG); // エラー
    }

    m_forest_util->SetDebugArea (start_addr, end_addr, peid);
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::ClearDebugAreaPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;

    PyArg_ParseTuple (args, "I", &start_addr);

    if (m_forest_util->ClearDebugArea (start_addr) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Failed to remove DebugArea>\n");
        return Py_BuildValue ("i", PY_NG);
    } else {
        m_forest_util->ClearAccessDeniedArea (start_addr);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::DumpBtbStatusPy (PyObject* self, PyObject* args)
{
    int peid, tcid;

    PyArg_ParseTuple (args, "ii", &peid, &tcid);

    if ( (peid == PY_NOSEL_ID) && (tcid == PY_NOSEL_ID) ) {
        // 全部PY_NOSEL_IDならContextID指定なしなので、全PE/TCが対象
        uint32_t sc_num = m_forest_util->GetScNum ();
        for (uint32_t scid = 0; scid < sc_num; scid ++) {
            uint32_t  lpeid = m_forest_util->GetPeId (scid);
            m_forest_util->DumpBtbStatistics (lpeid);
        }
    } else {
        // 指定された単一のTCのデバッグモードを設定
        if (IsPeidValid (peid) == false) {
          CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
            return Py_BuildValue ("i", PY_NG);
        }
        m_forest_util->DumpBtbStatistics (peid);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::ResetBtbStatusPy (PyObject* self, PyObject* args)
{
    int peid, tcid;

    PyArg_ParseTuple (args, "ii", &peid, &tcid);

    if ( (peid == PY_NOSEL_ID) && (tcid == PY_NOSEL_ID) ) {
        // 全部PY_NOSEL_IDならContextID指定なしなので、全PE/TCが対象
        uint32_t sc_num = m_forest_util->GetScNum ();
        for (uint32_t scid = 0; scid < sc_num; scid ++) {
            uint32_t  lpeid = m_forest_util->GetPeId (scid);
            m_forest_util->ResetBtbStatistics (lpeid);
        }
    } else {
        // 指定された単一のTCのデバッグモードを設定
        if (IsPeidValid (peid) == false) {
          CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n", __func__,peid);
            return Py_BuildValue ("i", PY_NG);
        }
        m_forest_util->ResetBtbStatistics (peid);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::ShowPerfInfoPy (PyObject* self, PyObject* args)
{
    int peid;

    PyArg_ParseTuple (args, "i", &peid);

    if (peid == PY_NOSEL_ID) {
        // PEID指定無しなら全PEが対象
        for (uint32_t lpeid = 0; lpeid <= m_forest_util->GetPeLastId (); lpeid++) {
            if (IsPeidValid (lpeid)) {
                m_forest_util->ShowPerfInfo (lpeid);
            }
        }
    } else {
        // 指定された単一のTCのデバッグモードを設定
        if (IsPeidValid (peid) == false) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n",__func__,peid);
            return Py_BuildValue ("i", PY_NG);
        }
        m_forest_util->ShowPerfInfo (peid);
    }
#ifdef CLUSTER
    m_forest_util->ShowClPerfInfo ();
#endif // CLUSTER
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::ResetPerfInfoPy (PyObject* self, PyObject* args)
{
    int peid;

    PyArg_ParseTuple (args, "i", &peid);

    if (peid == PY_NOSEL_ID) {
        // PEID指定無しなら全PEが対象
        for (uint32_t lpeid = 0; lpeid <= m_forest_util->GetPeLastId (); lpeid++) {
            if (IsPeidValid (lpeid)) {
                m_forest_util->ResetPerfInfo (lpeid);
            }
        }
    } else {
        // 指定された単一のTCのデバッグモードを設定
        if (IsPeidValid (peid) == false) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid id: %s:%u>\n",__func__,peid);
            return Py_BuildValue ("i", PY_NG);
        }
        m_forest_util->ResetPerfInfo (peid);
    }
#ifdef CLUSTER
    m_forest_util->ResetClPerfInfo ();
#endif // CLUSTER

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::ExecCfaliPy (PyObject* self, PyObject* args)
{
    unsigned int start_addr;
    unsigned int end_addr;
    int peid;
    PyArg_ParseTuple (args, "IIi", &start_addr, &end_addr, &peid);

    if (peid == PY_NOSEL_ID) {
        // ID指定無しなら全PEのNCが対象
        for (uint32_t lpeid = 0; lpeid <= m_forest_util->GetPeLastId (); lpeid++) {
            if (IsPeidValid (lpeid)) {
                m_forest_util->ExecCfali (lpeid, static_cast<PhysAddr>(start_addr & 0xFFFFFFFF),
                                          static_cast<PhysAddr>(end_addr & 0xFFFFFFFF));
            }
        }
    } else {
        // 指定された単一のTCのデバッグモードを設定
        if (IsPeidValid (peid) == false) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid id:%u>\n", peid);
            return Py_BuildValue ("i", PY_NG);
        }
        m_forest_util->ExecCfali (peid, static_cast<PhysAddr>(start_addr & 0xFFFFFFFF),
                                  static_cast<PhysAddr>(end_addr & 0xFFFFFFFF));
    }

    return Py_BuildValue ("i", PY_OK);
}


#if defined( COMP_RUNNER)
PyObject* PythonAPI::SetCompRunnerPy (PyObject* self, PyObject* args)
{
    PyArg_ParseTuple (args, "II", &m_cr_sampl_period, &m_cr_rank_lines);
    if (m_cr_sampl_period == 0) {
        m_cr_sampl_period = 1;
    }

    return Py_BuildValue("i", PY_OK);
}


uint32_t PythonAPI::GetCompSamplePeriod (void)
{
    return m_cr_sampl_period;
}


uint32_t PythonAPI::GetCompRankLines (void)
{
    return m_cr_rank_lines;
}

#endif // defined(COMP_RUNNER)


#if defined(COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)
PyObject* PythonAPI::SetCompMisrShowPy (PyObject* self, PyObject* args)
{

    char* has;
    PyArg_ParseTuple (args, "s", &has);

    std::string has_str = has;
    if (has_str == "on") {
        m_cmdopt->SetMisrShow(true);
    } else if (has_str == "off") {
        m_cmdopt->SetMisrShow(false);
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid setting.>\n");
    }

    return Py_BuildValue("");
}
#endif // defined(COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)


PyObject* PythonAPI::SetHasFxuPy (PyObject* self, PyObject* args)
{
    if (IsGenPe () == true) {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid setting. pe generate has already been.>\n");
        return Py_BuildValue ("i", PY_NG);
    }
    char* has;
    PyArg_ParseTuple (args, "s", &has);

    std::string has_str = has;
    if (has_str == "on") {
        m_cmdopt->SetHasFxu(true);
    } else if (has_str == "off") {
        m_cmdopt->SetHasFxu(false);
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid setting.>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    return Py_BuildValue ("i", PY_OK);
}

PyObject* PythonAPI::SetHasFpuPy (PyObject* self, PyObject* args)
{
    if (IsGenPe () == true) {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid setting. pe generate has already been.>\n");
        return Py_BuildValue ("i", PY_NG);
    }
    char* has;
    PyArg_ParseTuple (args, "s", &has);

    std::string has_str = has;
    if (has_str == "on") {
        m_cmdopt->SetHasFpu(true);
    } else if (has_str == "off") {
        m_cmdopt->SetHasFpu(false);
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid setting.>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::SetHasMpuPy (PyObject* self, PyObject* args)
{
    if (IsGenPe () == true) {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid setting. pe generate has already been.>\n");
        return Py_BuildValue ("i", PY_NG);
    }
    char* has;
    PyArg_ParseTuple (args, "s", &has);

    std::string has_str = has;
    if (has_str == "on") {
        m_cmdopt->SetHasMpu(true);
    } else if (has_str == "off") {
        m_cmdopt->SetHasMpu(false);
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid setting.>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::SetHasMmuPy (PyObject* self, PyObject* args)
{
    if (IsGenPe () == true) {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid setting. pe generate has already been.>\n");
        return Py_BuildValue ("i", PY_NG);
    }
    char* has;
    PyArg_ParseTuple (args, "s", &has);

    std::string has_str = has;
    if (has_str == "on") {
        m_cmdopt->SetHasMmu(true);
    } else if (has_str == "off") {
        m_cmdopt->SetHasMmu(false);
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid setting.>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::SetNumL1RLinkPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "Ii", &data, &peid);

    if ((data < 2) || (data > 8)) {
        // Number of the link resource is 2-8
        CfMsg::DPrint (MSG_ERR,
                       "<Error: the num of L1RAM link resource is invalid (%u). Valid range is 2-8>\n", data);
        return Py_BuildValue ("i", PY_NG); // Error
    }

    if (peid == PY_NOSEL_ID) {
        // PEID指定無しなら全PEが対象
        for (uint32_t lpeid = 0; lpeid <= m_forest_util->GetPeLastId (); lpeid++) {
            if (IsPeidValid (lpeid)) {
                m_forest_util->SetExtPin (lpeid, EXT_PIN_NUM_L1RLINK, data);
            }
        }
    } else {
        if (IsPeidValid (peid) == false) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
            return Py_BuildValue ("i", PY_NG); // エラー
        } else {
            m_forest_util->SetExtPin (peid, EXT_PIN_NUM_L1RLINK, data);
        }
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetNumL1RLinkPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "i", &peid);

    if (IsPeidValid (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    data = m_forest_util->GetExtPin (peid, EXT_PIN_NUM_L1RLINK);

    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetSizeL1RPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "Ii", &data, &peid);

    if ((data != 0x20) && (data != 0x40)) {
        // Valid size is 0x20 (32KB) or 0x40 (64KB)
        CfMsg::DPrint (MSG_ERR,
                       "<Error: the size of L1RAM is invalid (%u). Valid size is 0x20 and 0x40>\n", data);
        return Py_BuildValue ("i", PY_NG); // Error
    }

    if (peid == PY_NOSEL_ID) {
        // Target is all PE when PEID isn't specified.
        for (uint32_t lpeid = 0; lpeid <= m_forest_util->GetPeLastId (); lpeid++) {
            if (IsPeidValid (lpeid)) {
                m_forest_util->SetExtPin (lpeid, EXT_PIN_SIZE_L1R, data);
            }
        }
    } else {
        if (IsPeidValid (peid) == false) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
            return Py_BuildValue ("i", PY_NG); // Error
        } else {
            m_forest_util->SetExtPin (peid, EXT_PIN_SIZE_L1R, data);
        }
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetSizeL1RPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "i", &peid);

    if (IsPeidValid (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    data = m_forest_util->GetExtPin (peid, EXT_PIN_SIZE_L1R);

    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetPeripheralPy (PyObject* self, PyObject* args)
{
    if (IsGenPe () == true) {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid setting. pe generate has already been.>\n");
        return Py_BuildValue ("i", PY_NG);
    }
    char* name;
    char* has;
    unsigned int base_addr;

    PyArg_ParseTuple (args, "ssI", &name, &has, &base_addr);

    std::string has_str = has;
    bool is_enabled;
    if (has_str == "on") {
        is_enabled = true;
    } else if (has_str == "off") {
        is_enabled = false;
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid setting : %s>\n", has);
        return Py_BuildValue ("i", PY_NG);
    }
    std::string name_str = name;
    SetIORBaseAddrID peri_id;
    if (name_str == "all") {
        peri_id = BaseAddr_NUM;
    } else if (name_str == "intc1") {
        peri_id = BaseAddr_INTC1;
    } else if (name_str == "intc2") {
        peri_id = BaseAddr_INTC2;
    } else if (name_str == "mecnt") {
        peri_id = BaseAddr_MECNT;
    } else if (name_str == "ipir") {
        peri_id = BaseAddr_IPIR;
    } else if (name_str == "barrier") {
        peri_id = BaseAddr_BAR;
    } else if (name_str == "uart") {
        peri_id = BaseAddr_UART;
    } else if (name_str == "misr") {
        peri_id = BaseAddr_MISR;
    } else if (name_str == "perf") {
        peri_id = BaseAddr_PERF;
    } else if (name_str == "tmu") {
        peri_id = BaseAddr_TMU;
    } else if (name_str == "simio") {
        peri_id = BaseAddr_SIMIO;
    } else if (name_str == "simiog3k") {
        peri_id = BaseAddr_SIMIOG3K;
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid setting.>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    if (peri_id != BaseAddr_NUM) {
        //m_forest_util->EnablePeripheral (peri_id, is_enabled);
        if (base_addr == 0) {
            m_forest_util->EnablePeripheral (static_cast<SetIORBaseAddrID>(peri_id), is_enabled);
        } else {
            m_forest_util->SetPeripheral (static_cast<SetIORBaseAddrID> (peri_id),base_addr,is_enabled);
        }
    } else {
        // set all peripherals.
        // But, don't update for depricated peripherals (uart, misr, tmu, simiog3k)
        // If user wants to use depricated peripherals, set by for each.
        for (uint32_t id = BaseAddr_INTC1; id < BaseAddr_NUM; id++) {
            switch (static_cast<SetIORBaseAddrID>(id)) {
            case BaseAddr_UART:
            case BaseAddr_MISR:
            case BaseAddr_TMU:
            case BaseAddr_SIMIOG3K:
                continue;
            default:
                m_forest_util->EnablePeripheral (static_cast<SetIORBaseAddrID>(id), is_enabled);
                break;
            }
        }
    }

    return Py_BuildValue ("i", PY_OK);
}

PyObject* PythonAPI::SetIpirEiintChannelPy (PyObject* self, PyObject* args)
{
    uint32_t offset;
    PyArg_ParseTuple (args, "I", &offset);


    m_forest_util->SetEiintChannelOffset (offset);
    return Py_BuildValue ("i", PY_OK);
}

PyObject* PythonAPI::SetGuardPy (PyObject* self, PyObject* args)
{
    if (IsGenPe () == true) {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid setting. pe generate has already been.>\n");
        return Py_BuildValue ("i", PY_NG);
    }
    char* name;
    char* has;
    unsigned int location_id;
    unsigned int addr;
    PyArg_ParseTuple (args, "sIsI", &name, &location_id, &has, &addr);

    std::string has_str = has;
    bool is_enabled;
    if (has_str == "on") {
        is_enabled = true;
    } else if (has_str == "off") {
        is_enabled = false;
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid setting : %s>\n", has);
        return Py_BuildValue ("i", PY_NG);
    }
    std::string name_str = name;
    GuardModuleID gm_id;
    if (name_str == "peg") {
        gm_id = GMID_PEG_PEnCLm;       // PEID
    } else if (name_str == "pegcapm") {
        gm_id = GMID_PEGCAP_M_PEnCLm;  // PEID
    } else if (name_str == "pegcaps") {
        gm_id = GMID_PEGCAP_S_PEnCLm;  // PEID
    } else if (name_str == "crg") {
        gm_id = GMID_CRG_CLm;          // CLID
    } else if (name_str == "crgcapp") {
        gm_id = GMID_CRGCAP_PEnCLm;    // PEID
    } else if (name_str == "crgcapch") {
        gm_id = GMID_CRGCAP_CRAMHCLm;  // CLID
    } else if (name_str == "crgcapcl") {
        gm_id = GMID_CRGCAP_CRAMLCLm;  // CLID
    } else if (name_str == "crgcapsh") {
        gm_id = GMID_CRGCAP_SX2MBHCLm; // CLID
    } else if (name_str == "crgcapsl") {
        gm_id = GMID_CRGCAP_SX2MBLCLm; // CLID
    // } else if (name_str == "mdpcap") { // Unsupported
    //     gm_id = GMID_MDPCAP_PEnCLm;    // PEID
    } else if (name_str == "cfg") {
        gm_id = GMID_CFG;              // ignored
    } else if (name_str == "cfgcapi") {
        gm_id = GMID_CFGCAP_IPEnCLm;   // PEID
    } else if (name_str == "cfgcapd") {
        gm_id = GMID_CFGCAP_DPEnCLm;   // PEID
    } else if (name_str == "cfgcaps") {
        gm_id = GMID_CFGCAP_SX2FX;     // ignored
    } else if (name_str == "crgivc") {
        gm_id = GMID_CRG_IVC_CLm;      // CLID
    } else if (name_str == "crgcsg") {
        gm_id = GMID_CRG_CSG_CLm;      // CLID
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid guard module name:%s>\n", name_str.c_str());
        return Py_BuildValue ("i", PY_NG);
    }

    m_forest_util->SetGuard (gm_id, location_id, addr, is_enabled);
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::SetGuardParamPy (PyObject* self, PyObject* args)
{
    if (IsGenPe () == true) {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid setting. pe generate has already been.>\n");
        return Py_BuildValue ("i", PY_NG);
    }
    char* name;
    char* param_name;
    unsigned int location_id;
    unsigned int value;
    PyArg_ParseTuple (args, "sIsI", &name, &location_id, &param_name, &value);

    std::string name_str = name;
    GuardModuleID gm_id;
    if (name_str == "peg") {
        gm_id = GMID_PEG_PEnCLm;       // PEID
    } else if (name_str == "pegcapm") {
        gm_id = GMID_PEGCAP_M_PEnCLm;  // PEID
    } else if (name_str == "pegcaps") {
        gm_id = GMID_PEGCAP_S_PEnCLm;  // PEID
    } else if (name_str == "crg") {
        gm_id = GMID_CRG_CLm;          // CLID
    } else if (name_str == "crgivc") {
        gm_id = GMID_CRG_IVC_CLm;      // CLID
    } else if (name_str == "crgcsg") {
        gm_id = GMID_CRG_CSG_CLm;      // CLID
    } else if (name_str == "crgcapp") {
        gm_id = GMID_CRGCAP_PEnCLm;    // PEID
    } else if (name_str == "crgcapch") {
        gm_id = GMID_CRGCAP_CRAMHCLm;  // CLID
    } else if (name_str == "crgcapcl") {
        gm_id = GMID_CRGCAP_CRAMLCLm;  // CLID
    } else if (name_str == "crgcapsh") {
        gm_id = GMID_CRGCAP_SX2MBHCLm; // CLID
    } else if (name_str == "crgcapsl") {
        gm_id = GMID_CRGCAP_SX2MBLCLm; // CLID
    // } else if (name_str == "mdpcap") { // Unsupported
    //     gm_id = GMID_MDPCAP_PEnCLm;    // PEID
    } else if (name_str == "cfg") {
        gm_id = GMID_CFG;              // ignored
    } else if (name_str == "cfgcapi") {
        gm_id = GMID_CFGCAP_IPEnCLm;   // PEID
    } else if (name_str == "cfgcapd") {
        gm_id = GMID_CFGCAP_DPEnCLm;   // PEID
    } else if (name_str == "cfgcaps") {
        gm_id = GMID_CFGCAP_SX2FX;     // ignored
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid guard module name:%s>\n", name_str.c_str());
        return Py_BuildValue ("i", PY_NG);
    }

    std::string param_name_str = param_name;
    GuardParamID param_id = GPID_NUM;
    if (gm_id == GMID_CRG_CLm) {
        if (param_name_str == "crgaddrw") {
            param_id = GPID_CRG_ADDR_WIDTH;
        } else if (param_name_str == "crgcrgch") {
            param_id = GPID_CRG_CRG_CH_NUM;
        } else if (param_name_str == "crgcsgch") {
            param_id = GPID_CRG_CSG_CH_NUM;
        } else if (param_name_str == "crgkeycode") {
            param_id = GPID_CRG_KEYCODE;
        } else {
            CfMsg::DPrint (MSG_ERR, "<Error: invalid guard parameter name:%s>\n", param_name_str.c_str());
            return Py_BuildValue ("i", PY_NG);
        }
    }
    if (m_forest_util->SetGuardParam (gm_id, location_id, param_id, value)) {
        return Py_BuildValue ("i", PY_OK);
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid parameter in set_guards_param>\n");
        return Py_BuildValue ("i", PY_NG);
    }
}


bool PythonAPI::IsPEInfoValid (uint32_t peid, uint32_t clid, CoreType core_type)
{
    if (peid > MAX_PE) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid :%u>\n", peid);
        return false; // エラー
    }

    if (peid > MAX_CL) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid clid :%u>\n", peid);
        return false; // エラー
    }

    switch (core_type) {
    case CORE_G4MH:
        break;
    case CORE_G4KH:
        break;
    default:
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid CoreType>\n");
        return false;  // エラー
        break;
    }

    return true;
}


bool PythonAPI::IsCacheSizeValid (uint32_t size, uint32_t way)
{
    switch (size) {
    case CACHE_SIZE_0:
    case CACHE_SIZE_1K:
    case CACHE_SIZE_2K:
    case CACHE_SIZE_4K:
    case CACHE_SIZE_8K:
    case CACHE_SIZE_16K:
    case CACHE_SIZE_32K:
    case CACHE_SIZE_64K:
        break;
    default:
        // error
        CfMsg::DPrint (MSG_ERR,
                       "<Error: Invalid cache size(%u). Valid range are 0,1,2,4,8,16,32,64>\n",
                       size);
        return false;
    }

    switch (way) {
    case CACHE_WAY_0:
    case CACHE_WAY_1:
    case CACHE_WAY_2:
    case CACHE_WAY_4:
        break;
    default:
        // error
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid cache way(%u). Valid range are 0,1,2,4>\n", way);
        return false;
    }

    // No error
    return true;
}


// 無効なscidのチェック用
bool PythonAPI::IsScidValid (int32_t scid)
{
    return m_forest_util->IsValidScId (scid);
}


bool PythonAPI::IsPeidValid (int peid)
{
    return m_forest_util->IsValidPeId (peid);
}


#if defined(COMP_RUNNER)
PyObject* PythonAPI::XDumpFPU (PyObject* self, PyObject* args)
{
    int32_t ulp_diff, insn_id;
    char* insn_mn;

    PyArg_ParseTuple (args, "sI", &insn_mn, &ulp_diff);
    std::string insnstr = insn_mn ;

    //There are 6 FPU instruction are still different 1 ulp in RH850v2.
    //They are rsqrtf.s, rsqrtf.d, rsqrtf.s4, recipf.s, recipf.d, recipf.s4
    if (insnstr == "rsqrtfs") {
        insn_id = INST_ID_V_RSQRTFS ;
    } else if (insnstr == "rsqrtfd") {
        insn_id = INST_ID_V_RSQRTFD ;
    } else if (insnstr == "rsqrtfs4") {
        insn_id = INST_ID_V_RSQRTFS4 ;
    } else if (insnstr == "recipfs") {
        insn_id = INST_ID_V_RECIPFS ;
    } else if (insnstr == "recipfd") {
        insn_id = INST_ID_V_RECIPFD ;
    } else if (insnstr == "recipfs4") {
        insn_id = INST_ID_V_RECIPFS4 ;
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid fpu instruction name:%s>\n", insnstr.c_str());
        return Py_BuildValue ("i", PY_NG);
    }

    XDUMP_ARITH *new_xda = new XDUMP_ARITH(insn_id, ulp_diff);
    m_cmdopt->m_xdump->m_xdump_arith.push_back(new_xda);

    return Py_BuildValue ("i", PY_OK);
}

PyObject* PythonAPI::XDumpREG (PyObject* self, PyObject* args)
{
    uint64_t start_addr, end_addr;

    PyArg_ParseTuple (args, "KK", &start_addr, &end_addr);

    XDUMP_REG *new_xdreg = new XDUMP_REG(start_addr, end_addr);
    m_cmdopt->m_xdump->m_xdump_reg.push_back(new_xdreg);

    return Py_BuildValue ("i", PY_OK);
}

PyObject* PythonAPI::XDumpFLAG (PyObject* self, PyObject* args)
{
    uint64_t start_addr, end_addr;

    PyArg_ParseTuple (args, "KK", &start_addr, &end_addr);

    XDUMP_FLAG *new_xdflag = new XDUMP_FLAG(start_addr, end_addr);
    m_cmdopt->m_xdump->m_xdump_flag.push_back(new_xdflag);

    return Py_BuildValue ("i", PY_OK);
}

PyObject* PythonAPI::XDumpDATA (PyObject* self, PyObject* args)
{
    uint64_t start_addr, end_addr, mask;

    PyArg_ParseTuple (args, "KKK", &start_addr, &end_addr, &mask);

    XDUMP_DATA *new_xdd = new XDUMP_DATA(start_addr, end_addr, mask);
    m_cmdopt->m_xdump->m_xdump_data.push_back(new_xdd);

    return Py_BuildValue ("i", PY_OK);
}

PyObject* PythonAPI::XDumpSYSREG (PyObject* self, PyObject* args)
{
    uint32_t reg_id, sel_id;
    uint64_t mask;

    PyArg_ParseTuple (args, "IIK", &reg_id, &sel_id, &mask);

    XDUMP_SYSREG *new_xdsr = new XDUMP_SYSREG(reg_id, sel_id, mask);
    m_cmdopt->m_xdump->m_xdump_sysreg.push_back(new_xdsr);

    return Py_BuildValue ("i",PY_OK);
}

PyObject* PythonAPI::SetCmpStage (PyObject* self, PyObject* args)
{
    char* cmp_stage;
    PyArg_ParseTuple (args, "s", &cmp_stage);
    std::string cmpstage = cmp_stage ;
    
    if(cmpstage == "dispatch"){
        m_forest_util->SetCmpStage(STAGE_DP);
    } else if(cmpstage == "issue"){
        m_forest_util->SetCmpStage(STAGE_EA);
    } else if (cmpstage == "commit") {
        m_forest_util->SetCmpStage(STAGE_WB); // @@ TODO change CM stage when commit cycle is supported in Cforest
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid stage name for set_cmp_stage : %s>\n", cmpstage.c_str());
        return Py_BuildValue ("i", PY_NG);
    }
    return Py_BuildValue ("i", PY_OK);
}

PyObject* PythonAPI::SetCycCmpPC (PyObject* self, PyObject* args)
{
    uint64_t start_pc, end_pc;

    PyArg_ParseTuple (args, "KK", &start_pc, &end_pc);
    m_forest_util->SetCycCmpStartPC(start_pc);
    m_forest_util->SetCycCmpEndPC(end_pc);

    return Py_BuildValue ("i", PY_OK);
}
#endif // defined(COMP_RUNNER)


void PythonAPI::ExitHandler (int exit_status)
{
#ifdef DISABLE_DESTRUCTOR_CYCLE
#else // DISABLE_DESTRUCTOR_CYCLE
    if (m_forest_util->GetShowStatistics () == true) {
        for (uint32_t lpeid = 0; lpeid <= m_forest_util->GetPeLastId (); lpeid++) {
            if (m_forest_util->IsValidPeId (lpeid)) {
                m_forest_util->ShowPerfInfo (lpeid);
            }
        }
    }
#endif // DISABLE_DESTRUCTOR_CYCLE

    std::vector<EventInfo*>::iterator it = m_event_info.begin ();
    while (it != m_event_info.end ()) {
        delete (*it);
        ++ it;
    }
    m_event_info.clear ();

    delete m_forest_setup;
    delete m_cmdopt;
    delete m_forest_util;
    exit (exit_status);
}


PyObject* PythonAPI::SetDebugOptionPy (PyObject* self, PyObject* args)
{
    char* opt;
    char* mod;

    DebugMode dbg_mode = m_forest_util->GetDebugMode ();

    PyArg_ParseTuple (args, "ss", &opt, &mod);

    std::string option = opt;
    std::string mode = mod;

    if (mode == "on") {
        if (option == "rw") {
            dbg_mode |= DBG_RW;
        } else if (option == "pipe") {
            dbg_mode |= DBG_PIPE;
        } else if (option == "dep") {
            dbg_mode |= DBG_DEP;
        } else if (option == "bra") {
            dbg_mode |= DBG_BRA;
        } else if (option == "systime") {
            dbg_mode |= DBG_TIME;
        } else if (option == "cache") {
            dbg_mode |= DBG_CACHE;
        } else if (option == "stall") {
            dbg_mode |= DBG_STALL;
        } else if (option == "timer") {
            dbg_mode |= DBG_TIMER;
        } else if (option == "intc") {
            dbg_mode |= DBG_INTC;
        } else if (option == "perfcnt") {
            dbg_mode |= DBG_PERF;
        } else if (option == "fput") {
            dbg_mode |= DBG_FPUT;
        } else {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid debug option (%s)>\n", opt);
            return Py_BuildValue ("i", PY_NG);
        }
    } else if (mode == "off") {
        if (option == "rw") {
            dbg_mode = dbg_mode & (DBG_MASK ^ DBG_RW);
        } else if (option == "pipe") {
            dbg_mode = dbg_mode & (DBG_MASK ^ DBG_PIPE);
        } else if (option == "dep") {
            dbg_mode = dbg_mode & (DBG_MASK ^ DBG_DEP);
        } else if (option == "bra") {
            dbg_mode = dbg_mode & (DBG_MASK ^ DBG_BRA);
        } else if (option == "systime") {
            dbg_mode = dbg_mode & (DBG_MASK ^ DBG_TIME);
        } else if (option == "cache") {
            dbg_mode = dbg_mode & (DBG_MASK ^ DBG_CACHE);
        } else if (option == "stall") {
            dbg_mode = dbg_mode & (DBG_MASK ^ DBG_STALL);
        } else if (option == "timer") {
            dbg_mode = dbg_mode & (DBG_MASK ^ DBG_TIMER);
        } else if (option == "intc") {
            dbg_mode = dbg_mode & (DBG_MASK ^ DBG_INTC);
        } else if (option == "perfcnt") {
            dbg_mode = dbg_mode & (DBG_MASK ^ DBG_PERF);
        } else if (option == "fput") {
            dbg_mode = dbg_mode & (DBG_MASK ^ DBG_FPUT);
        } else {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid debug option (%x)>\n", dbg_mode);
            return Py_BuildValue ("i", PY_NG);
        }
        m_forest_util->DisableDebugOption (option);
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid debug mode (%s)>\n", mod);
        return Py_BuildValue ("i", PY_NG);
    }

    m_forest_util->SetDebugMode (dbg_mode);

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetDebugOptionPy (PyObject* self, PyObject* args)
{
    std::ostringstream dbg_str;
    dbg_str << "";
    DebugMode dbg_mode = m_forest_util->GetDebugMode ();

    if ((dbg_mode & DBG_RW) != DBG_OFF) {
        dbg_str << "rw ";
    }
    if ((dbg_mode & DBG_PIPE) != DBG_OFF) {
        dbg_str << "pipe ";
    }
    if ((dbg_mode & DBG_DEP) != DBG_OFF) {
        dbg_str << "dep ";
    }
    if ((dbg_mode & DBG_BRA) != DBG_OFF) {
        dbg_str << "bra ";
    }
    if ((dbg_mode & DBG_TIME) != DBG_OFF) {
        dbg_str << "systime ";
    }
    if ((dbg_mode & DBG_CACHE) != DBG_OFF) {
        dbg_str << "cache";
    }
    if ((dbg_mode & DBG_STALL) != DBG_OFF) {
        dbg_str << "stall ";
    }
    if ((dbg_mode & DBG_TIMER) != DBG_OFF) {
        dbg_str << "timer ";
    }
    if ((dbg_mode & DBG_INTC) != DBG_OFF) {
        dbg_str << "intc ";
    }
    if ((dbg_mode & DBG_PERF) != DBG_OFF) {
        dbg_str << "perfcnt ";
    }
    return Py_BuildValue ("s", dbg_str.str().c_str());
}


PyObject* PythonAPI::GetArchClassPy (PyObject* self, PyObject* args)
{
    ArchClass arch_class;
    std::string arch_str;

    arch_class = m_cmdopt->GetArchClass ();

    if (arch_class == E3V5_F) {
        arch_str = "F";
    } else if (arch_class == E3V5_V) {
        arch_str = "V";
    } else if (arch_class == E3V5_S) {
        arch_str = "S";
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid arch class (%u)>\n", arch_class);
        return Py_BuildValue ("i", PY_NG);
    }

    return Py_BuildValue ("s", arch_str.c_str());
}


PyObject* PythonAPI::SetMsgLvlPy (PyObject* self, PyObject* args)
{
    char* lvl;
    MessageLevel msg_lvl;

    PyArg_ParseTuple (args, "s", &lvl);

    std::string lvl_str = lvl;
    if (lvl_str == "ERR") {
        msg_lvl = MSG_ERR;
    } else if (lvl_str == "WAR") {
        msg_lvl = MSG_WAR;
    } else if (lvl_str == "EXP") {
        msg_lvl = MSG_EXP;
    } else if (lvl_str == "INF") {
        msg_lvl = MSG_INF;
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid message level (%s)>\n", lvl);
        return Py_BuildValue ("i", PY_NG);
    }

    m_forest_util->SetMessageLevel (msg_lvl);

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetMsgLvlPy (PyObject* self, PyObject* args)
{
    MessageLevel msg_lvl;
    std::string lvl_str;

    msg_lvl = m_forest_util->GetMessageLevel ();

    if (msg_lvl == MSG_ERR) {
        lvl_str = "ERR";
    } else if (msg_lvl == MSG_WAR) {
        lvl_str = "WAR";
    } else if (msg_lvl == MSG_EXP) {
        lvl_str = "EXP";
    } else if (msg_lvl == MSG_INF) {
        lvl_str = "INF";
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid message level (%u)>\n", msg_lvl);
        return Py_BuildValue ("i", PY_NG);
    }

    return Py_BuildValue ("s", lvl_str.c_str());
}


PyObject* PythonAPI::UseStopIOPy (PyObject* self, PyObject* args)
{
    int enable;

    PyArg_ParseTuple (args, "i", &enable);
    if (enable != 0) {
        m_forest_util->EnableBreakAccStopio();
    }
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::SetFpuExistencePy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "Ii", &data, &peid);

    if (data > 1) {
        CfMsg::DPrint (MSG_ERR,
                       "<Error: Invalid default operation (%u). Valid range is 0-1>\n", data);
        CfMsg::DPrint (MSG_ERR, "<0: FPU function is disable>\n");
        CfMsg::DPrint (MSG_ERR, "<1: FPU function is enable>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    if (peid == PY_NOSEL_ID) {
        for (uint32_t lpeid = 0; lpeid <= m_forest_util->GetPeLastId (); lpeid++) {
            if (IsPeidValid (lpeid)) {
                m_forest_util->SetExtPin (lpeid, EXT_PIN_HAS_FPU, data & 0x01);
            }
        }
    } else {
        if (IsPeidValid (peid) == false) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
            return Py_BuildValue ("i", PY_NG);
        } else {
            m_forest_util->SetExtPin (peid, EXT_PIN_HAS_FPU, data & 0x01);
        }
    }

    return Py_BuildValue("i", PY_OK);
}


PyObject* PythonAPI::GetFpuExistencePy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "i", &peid);

    if (IsPeidValid (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    data = m_forest_util->GetExtPin (peid, EXT_PIN_HAS_FPU);

    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetFxuExistencePy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "Ii", &data, &peid);

    if (data > 1) {
        CfMsg::DPrint (MSG_ERR,
                       "<Error: Invalid default operation (%u). Valid range is 0-1>\n", data);
        CfMsg::DPrint (MSG_ERR, "<0: FXU function is disable>\n");
        CfMsg::DPrint (MSG_ERR, "<1: FXU function is enable>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    if (peid == PY_NOSEL_ID) {
        for (uint32_t lpeid = 0; lpeid <= m_forest_util->GetPeLastId (); lpeid++) {
            if (IsPeidValid (lpeid)) {
                m_forest_util->SetExtPin (lpeid, EXT_PIN_HAS_FXU, data & 0x01);
            }
        }
    } else {
        if (IsPeidValid (peid) == false) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
            return Py_BuildValue ("i", PY_NG);
        } else {
            m_forest_util->SetExtPin (peid, EXT_PIN_HAS_FXU, data & 0x01);
        }
    }

    return Py_BuildValue("i", PY_OK);
}


PyObject* PythonAPI::GetFxuExistencePy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "i", &peid);

    if (IsPeidValid (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    data = m_forest_util->GetExtPin (peid, EXT_PIN_HAS_FXU);

    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetMpuExistencePy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "Ii", &data, &peid);

    if (data > 1) {
        CfMsg::DPrint (MSG_ERR,
                       "<Error: Invalid default operation (%u). Valid range is 0-1>\n", data);
        CfMsg::DPrint (MSG_ERR, "<0: MPU function is disable>\n");
        CfMsg::DPrint (MSG_ERR, "<1: MPU function is enable>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    if (peid == PY_NOSEL_ID) {
        for (uint32_t lpeid = 0; lpeid <= m_forest_util->GetPeLastId (); lpeid++) {
            if (IsPeidValid (lpeid)) {
                m_forest_util->SetExtPin (lpeid, EXT_PIN_HAS_MPU, data & 0x01);
            }
        }
    } else {
        if (IsPeidValid (peid) == false) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
            return Py_BuildValue ("i", PY_NG);
        } else {
            m_forest_util->SetExtPin (peid, EXT_PIN_HAS_MPU, data & 0x01);
        }
    }

    return Py_BuildValue("i", PY_OK);
}


PyObject* PythonAPI::GetMpuExistencePy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "i", &peid);

    if (IsPeidValid (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    data = m_forest_util->GetExtPin (peid, EXT_PIN_HAS_MPU);

    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetMmuExistencePy (PyObject* self, PyObject* args)
{
    return Py_BuildValue("i", PY_OK);
}


PyObject* PythonAPI::GetMmuExistencePy (PyObject* self, PyObject* args)
{
    unsigned int data = 0;
    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetHvcfgPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "Ii", &data, &peid);

    if (peid == PY_NOSEL_ID) {
        for (uint32_t lpeid = 0; lpeid <= m_forest_util->GetPeLastId (); lpeid++) {
            if (IsPeidValid (lpeid)) {
                m_forest_util->SetExtPin (lpeid, EXT_PIN_HVE, data & 0x01);
            }
        }
    } else {
        if (IsPeidValid (peid) == false) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
            return Py_BuildValue ("i", PY_NG);
        } else {
            m_forest_util->SetExtPin (peid, EXT_PIN_HVE, data & 0x01);
        }
    }

    return Py_BuildValue("i", PY_OK);
}


PyObject* PythonAPI::GetHvcfgPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "i", &peid);

    if (IsPeidValid (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    data = m_forest_util->GetExtPin (peid, EXT_PIN_HVE);

    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetHasHvPy (PyObject* self, PyObject* args)
{
    if (IsGenPe () == true) {
        CfMsg::DPrint (MSG_ERR, "<Error: invalid setting. pe generate has already been.>\n");
        return Py_BuildValue ("i", PY_NG);
    }
    char* has;
    PyArg_ParseTuple (args, "s", &has);

    std::string has_str = has;
    if (has_str == "on") {
        m_cmdopt->SetHasHv(true);
    } else if (has_str == "off") {
        m_cmdopt->SetHasHv(false);
    } else {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid default operation (%s). Specify \"on\" or \"off\">\n", has_str.c_str());
        return Py_BuildValue ("i", PY_NG);
    }

    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::SetLscfgPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "Ii", &data, &peid);

    if (peid == PY_NOSEL_ID) {
        for (uint32_t lpeid = 0; lpeid <= m_forest_util->GetPeLastId (); lpeid++) {
            if (IsPeidValid (lpeid)) {
                m_forest_util->SetExtPin (lpeid, EXT_PIN_LSCFG_LSE, data & 0x01);
            }
        }
    } else {
        if (IsPeidValid (peid) == false) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
            return Py_BuildValue ("i", PY_NG);
        } else {
            m_forest_util->SetExtPin (peid, EXT_PIN_LSCFG_LSE, data & 0x01);
        }
    }

    return Py_BuildValue("i", PY_OK);
}


PyObject* PythonAPI::SetDbgenPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "Ii", &data, &peid);

    if (peid == PY_NOSEL_ID) {
        for (uint32_t lpeid = 0; lpeid <= m_forest_util->GetPeLastId (); lpeid++) {
            if (IsPeidValid (lpeid)) {
                m_forest_util->SetExtPin (lpeid, EXT_PIN_DBGEN, data);
            }
        }
    } else {
        if (IsPeidValid (peid) == false) {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
            return Py_BuildValue ("i", PY_NG);
        } else {
            m_forest_util->SetExtPin (peid, EXT_PIN_DBGEN, data);
        }
    }

    return Py_BuildValue("i", PY_OK);
}


PyObject* PythonAPI::GetDbgenPy (PyObject* self, PyObject* args)
{
    int peid;
    unsigned int data;
    PyArg_ParseTuple (args, "i", &peid);

    if (IsPeidValid (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid peid>\n");
        return Py_BuildValue ("i", PY_NG);
    }

    data = m_forest_util->GetExtPin (peid, EXT_PIN_DBGEN);

    return Py_BuildValue ("I", data);
}


PyObject* PythonAPI::SetMemInitPy (PyObject* self, PyObject* args)
{
    unsigned int data;

    PyArg_ParseTuple (args, "I", &data);
    if (data <= 255) {
        m_forest_util->SetMemInit (data & 0xFFU);
    } else {
        CfMsg::DPrint (MSG_ERR,
                       "<Error: Invalid Memory Initialize (%u). Valid range are 0->255>\n", data);
        return Py_BuildValue ("i", PY_NG);
    }
    return Py_BuildValue ("i", PY_OK);
}


PyObject* PythonAPI::GetMemInitPy (PyObject* self, PyObject* args)
{
    unsigned int mem_ini;

    mem_ini = m_forest_util->GetMemInit ();

    return Py_BuildValue ("I", mem_ini);
}


void PythonAPI::InitPy (void)
{
    m_last_break_event = BREAK_NONE;
    m_is_gen_pe = false;

    m_pe_to_pe_latency = 0;
    m_is_quited = false;
    m_is_step_any = false;

    m_rom_start_addr = Lsu::ROM_BGN;
    m_rom_end_addr = Lsu::ROM_END;
    m_lram_fetch_latency = 8;
    m_rom_fetch_latency = 1;

    m_peid_vec.clear();
    m_pe_info.clear();

    m_event_info.clear ();
}
