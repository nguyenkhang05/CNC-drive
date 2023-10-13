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
#include "./forest_common.h"

class ForestUtil;
#if defined(COMP_RUNNER)
class XDUMP;
#endif // defined(COMP_RUNNER)

class ForestCmdOpt {
private:
    static const uint32_t MAX_PE = PEID_MASK;
    static const uint32_t MAX_VM = 0;
    static const uint32_t MAX_HT = 0;

    static const uint32_t N_PE = 1;
    static const uint32_t N_VM_PER_PE = 0;
    static const uint32_t N_HT_PER_PE = 0;

    ForestUtil* m_forest_util;

public:
    static const uint32_t UART_NO_OPT   = 0;
    static const uint32_t UART_PORT_OPT = 1;
    static const uint32_t UART_CH_OPT   = 2;
    static const uint32_t UART_BOTH_OPT = 3;

    explicit ForestCmdOpt (ForestUtil* forest_util);
    ~ForestCmdOpt () {}

    void AnalyzeOption (int argc, char* argv[]);

#if defined(COMP_RUNNER)
    XDUMP* m_xdump;
#endif // defined(COMP_RUNNER)

#if defined(COMP_RUNNER_G3MH) || defined(COMP_RUNNER_G4MH)
    bool m_comp_misr_show;
    void SetMisrShow (bool misr_show) { m_comp_misr_show = misr_show; }
    bool IsMisrShow (void)const { return m_comp_misr_show; }
#endif // defined(COMP_RUNNER_G3MH) || defined(COMP_RUNNER_G4MH)

    // コマンドラインパラメータ類
    uint32_t m_pe_num;
    uint32_t m_vc_num;
    uint32_t m_tc_num;
    void SetPeNumFromDbg (uint32_t num) { m_pe_num = num; }
    void SetVcNumFromDbg (uint32_t num) { m_vc_num = num; }
    void SetTcNumFromDbg (uint32_t num) { m_tc_num = num; }
    uint32_t GetPeNumFromDbg (void) const { return m_pe_num; }
    uint32_t GetVcNumFromDbg (void) const { return m_vc_num; }
    uint32_t GetTcNumFromDbg (void) const { return m_tc_num; }

    ArchClass m_arch_class;
    void SetArchClass (ArchClass arch_class) { m_arch_class = arch_class; }
    ArchClass GetArchClass (void) const { return m_arch_class; }

    CoreType m_core_type;
    void SetCoreType (CoreType core_type) { m_core_type = core_type; }
    CoreType GetCoreType (void) const { return m_core_type; }

    CoreVersion m_core_version;
    void SetCoreVersion (CoreVersion core_version) { m_core_version = core_version; }
    CoreVersion GetCoreVersion (void) const { return m_core_version; }

    bool m_print_pythonif_flag;
    void EnablePrintPythonIF (bool is_print) { m_print_pythonif_flag = is_print; }

    static const uint64_t INIT_MAX_STEP = 0xFFFFFFFFFFFFFFFFULL;
    uint64_t m_max_step;
    void SetMaxStep (uint64_t step) { m_max_step = step; }
    uint64_t GetMaxStep (void) const { return m_max_step; }

    bool m_is_set_max;
    void SetMaxCmdOpt (bool is_set) { m_is_set_max = is_set; }
    bool IsSetMaxCmdOpt (void) const { return m_is_set_max; }

    uint8_t m_memini_value;
    void SetMemInit (uint8_t memini) { m_memini_value = memini; }
    uint8_t GetMemInit (void) const { return m_memini_value; }

    DebugMode m_dbg_mode;
    void SetDebugMode (DebugMode mode) { m_dbg_mode = mode; }
    DebugMode GetDebugMode (void) const { return m_dbg_mode; }

    bool m_isstrace_flag;
    void EnableIssTrace (bool enable) { m_isstrace_flag = enable; }
    bool IsEnabledIssTrace (void) const { return m_isstrace_flag; }

    bool m_is_break_halt_enabled;
    void EnableHaltBreak (bool enable) { m_is_break_halt_enabled = enable; }
    bool IsEnabledHaltBreak (void) const { return m_is_break_halt_enabled; }

    bool m_is_stopio_enabled;
    void EnableStopio (bool enable) { m_is_stopio_enabled = enable; }
    bool IsEnabedStopio (void) const { return m_is_stopio_enabled; }

    uint16_t m_gdb_port;
    void SetGdbPort (uint16_t port) { m_gdb_port = port; }
    uint16_t GetGdbPort (void) const { return m_gdb_port; }

    bool m_is_set_gdb_cmdopt;
    void SetGdbCmdopt (bool is_set) { m_is_set_gdb_cmdopt = is_set; }
    bool IsSetGdbCmdopt (void) const { return m_is_set_gdb_cmdopt; }

    uint32_t m_uart_option;
    void SetUartOption (uint32_t option) { m_uart_option = option; }
    uint32_t GetUartOption (void) const { return m_uart_option; }

    uint16_t m_uart_socket_port;
    void SetUartSocketPort (uint16_t port) { m_uart_socket_port = port; }
    uint16_t GetUartSocketPort (void) const { return m_uart_socket_port; }

    uint32_t m_uart_channel_num;
    void SetUartChannelNum (uint32_t num) { m_uart_channel_num = num; }
    uint32_t GetUartChannelNum (void) const { return m_uart_channel_num; }

    bool m_is_set_uart_cmdopt;
    void SetUartCmdopt (bool is_set) { m_is_set_uart_cmdopt = is_set; }
    bool IsSetUartCmdopt (void) const { return m_is_set_uart_cmdopt; }

    enum CmdOptSet {
        CMD_OPT_ENABLE,
        CMD_OPT_DISABLE,
        CMD_OPT_NOSEL
    };
    CmdOptSet m_enable_ic;
    void EnableIC (CmdOptSet enable) { m_enable_ic = enable; }
    CmdOptSet GetEnableIC (void) const { return m_enable_ic; }

    bool m_enable_dc;
    void EnableDC (bool enable) { m_enable_dc = enable; }
    bool IsEnabedDC (void) const { return m_enable_dc; }

    bool m_has_fpu;
    void SetHasFpu (bool has_fpu) { m_has_fpu = has_fpu; }
    bool HasFpu (void) const { return m_has_fpu; }

    bool m_has_fxu;
    void SetHasFxu (bool has_fxu) { m_has_fxu = has_fxu; }
    bool HasFxu (void) const { return m_has_fxu; }

    bool m_has_mpu;
    void SetHasMpu (bool has_mpu) { m_has_mpu = has_mpu; }
    bool HasMpu (void) const { return m_has_mpu; }

    bool m_has_mmu;
    void SetHasMmu (bool has_mmu) { m_has_mmu = has_mmu; }
    bool HasMmu (void) const { return m_has_mmu; }

    bool m_has_hv;
    void SetHasHv (bool has_hv) { m_has_hv = has_hv; }
    bool HasHv (void) const { return m_has_hv; }

    uint32_t m_dc_fill_latency;
    void SetDcacheFillLatency (uint32_t latency) { m_dc_fill_latency = latency; }
    uint32_t GetDcacheFillLatency (void) const { return m_dc_fill_latency; }

    uint32_t m_dc_hit_latency;
    void SetDcacheHitLatency (uint32_t latency) { m_dc_hit_latency = latency; }
    uint32_t GetDcacheHitLatency (void) const { return m_dc_hit_latency; }

    uint32_t m_additonal_fetch_stage;
    void SetAdditonalFetchStage (uint32_t num) { m_additonal_fetch_stage = num; }
    uint32_t GetAdditonalFetchStage (void) const { return m_additonal_fetch_stage; }

    static const uint16_t INIT_SNOOZE_PERIOD = 32;
    uint16_t m_snooze_period;
    void SetSnoozePeriod (uint16_t period) { m_snooze_period = period; }
    uint16_t GetSnoozePeriod (void) const { return m_snooze_period; }

    static const uint32_t INIT_ROM_WAIT = 4;
    uint32_t m_rom_wait;
    void SetRomWait (uint32_t rom_wait) { m_rom_wait = rom_wait; }
    uint32_t GetRomWait (void) const { return m_rom_wait; }

    std::string m_script_fname;
    void SetScriptName (std::string fname) { m_script_fname = fname; }
    std::string GetScriptName (void) const { return m_script_fname; }

    std::string m_out_fname;
    void SetOutFname (std::string fname) { m_out_fname = fname; }
    std::string GetOutFname (void) const { return m_out_fname; }

    bool m_enable_out_multi;
    void EnableOutMulti (bool is_enabled) { m_enable_out_multi = is_enabled; }
    bool IsOutMulti (void) const { return m_enable_out_multi; }

    std::string m_hex_fname;
    void SetHexName (std::string fname) { m_hex_fname = fname; }
    std::string GetHexName (void) const { return m_hex_fname; }

    uint32_t m_pe_to_pe_latency;
    static const uint32_t INIT_PE_LATENCY = 0;

    MessageLevel m_msglvl;
    void SetMessageLevel (MessageLevel lvl) { m_msglvl = lvl; }
    MessageLevel GetMessageLevel (void) { return m_msglvl; }

    bool m_show_staticstics;
    void SetShowStatistics (bool is_show) { m_show_staticstics = is_show; }
    bool GetShowStatistics (void) const { return m_show_staticstics; }

    int32_t m_fpu_lat_offset;
    void SetFpuLatencyOffset (int32_t offset) { m_fpu_lat_offset = offset; }
    int32_t GetFpuLatencyOffset (void) const { return m_fpu_lat_offset; }

    int32_t m_divf_lat_offset;
    void SetDivfLatencyOffset (int32_t offset) { m_divf_lat_offset = offset; }
    int32_t GetDivfLatencyOffset (void) const { return m_divf_lat_offset; }

    uint32_t m_dbuf_depth;
    void SetDbufDepth (uint32_t depth) { m_dbuf_depth = depth; }
    uint32_t GetDbufDepth (void) const { return m_dbuf_depth; }

    uint32_t m_dbuf_line_shift;
    void SetDbufLineShift (uint32_t shift) { m_dbuf_line_shift = shift; }
    uint32_t GetDbufLineShift (void) const { return m_dbuf_line_shift; }

    uint32_t m_dbuf_fill_num;
    void SetDbufFillBurstNum (uint32_t num) { m_dbuf_fill_num = num; }
    uint32_t GetDbufFillBurstNum (void) const { return m_dbuf_fill_num; }

    uint32_t m_ic_line_byte;
    void SetIcLineByte (uint32_t size) { m_ic_line_byte = size; }
    uint32_t GetIcLineByte (void) const { return m_ic_line_byte; }

    DbufPreloadType m_dbuf_preload_type;
    void SetDbufPreloadType (DbufPreloadType type) { m_dbuf_preload_type = type; }
    DbufPreloadType GetDbufPreloadType (void) const { return m_dbuf_preload_type; }

    uint32_t m_flash_width_byte;
    void SetFlashWidthByte (uint32_t size) { m_flash_width_byte = size; }
    uint32_t GetFlashWidthByte (void) const { return m_flash_width_byte; }

    uint32_t m_ras_num;
    void SetRasNum (uint32_t num) { m_ras_num = num; }
    uint32_t GetRasNum (void) const { return m_ras_num; }

    bool m_is_tso;
    void SetTso (bool is_tso) { m_is_tso = is_tso; }
    bool IsTso (void) const { return m_is_tso; }

#ifdef ENABLE_EXE_ORDER_CTRL
    bool m_is_ooo_g4mh;
    void SetExecOrderOoOG4MH (bool is_ooo) { m_is_ooo_g4mh = is_ooo; }
    bool IsExecOrderOoOG4MH (void) const { return m_is_ooo_g4mh; }
#endif // #ifdef ENABLE_EXE_ORDER_CTRL

    bool m_work_perfinfo_with_perfcnt;
    void SetWorkPerfInfoWithPerfcnt (bool is_enabled)
    { m_work_perfinfo_with_perfcnt = is_enabled; }
    bool IsPerfInfoWorkWithPerfcnt (void) const { return m_work_perfinfo_with_perfcnt; }

#ifdef ENABLE_PEID_POS_CTRL
    bool m_is_peid_g4x;
    void SetPeidPosG4x (bool is_g4x) { m_is_peid_g4x = is_g4x; }
    bool IsPeidPosG4x (void) const { return m_is_peid_g4x; }
#endif // #ifdef ENABLE_PEID_POS_CTRL

    bool m_is_bus_trace;
    void SetBusTrace (bool is_bus_trace) { m_is_bus_trace = is_bus_trace; }
    bool GetBusTrace (void) const { return m_is_bus_trace; }

#if defined(COMP_RUNNER)
    void SetXDump(XDUMP* xdump) { m_xdump = xdump; }
    XDUMP* GetXDump(void) { return m_xdump; }
#endif // defined(COMP_RUNNER)

    bool m_is_enable_profiler;
    void SetEnableProfiler (bool is_enable) { m_is_enable_profiler = is_enable; }
    bool IsEnableProfiler (void) const { return m_is_enable_profiler; }
};
