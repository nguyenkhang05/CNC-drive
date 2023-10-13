/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <string>
#include "./config.h"
#include "./forest_utility.h"
#include "./forest.h"
#include "./forest_cmdopt.h"

#ifdef SUPPORT_CUBESUITE
#else // SUPPORT_CUBESUITE
#include "./cmdline.h"
#endif // SUPPORT_CUBESUITE


void ForestCmdOpt::AnalyzeOption (int argc, char *argv[])
{
#ifdef SUPPORT_CUBESUITE
#else // SUPPORT_CUBESUITE
    using ::std::string;
    using ::std::cout;
    using ::std::cerr;
    using ::std::endl;

    // Hide options
    // hide=0 : Show all options
    // hide=1 : depricated options (for compatibility, options are remained.)
    // hide=2 : secret option to external users (hide options for evaluation, internal name)
    // hide=3 : secret option to customers/students
#ifdef HIDE_OPT
    const uint32_t hide_level = 3;
#else // HIDE_OPT
#if defined(SUPPORT_SC_HEAP) || defined(SUPPORT_CUBESUITE)
    const uint32_t hide_level = 2;
#else // defined(SUPPORT_SC_HEAP) || defined(SUPPORT_CUBESUITE)
    const uint32_t hide_level = 1;
#endif // defined(SUPPORT_SC_HEAP) || defined(SUPPORT_CUBESUITE)
#endif // HIDE_OPT

    const bool not_hide = false;
    const bool is_hide_1 = (hide_level >= 1);
    //const bool is_hide_2 = (hide_level >= 2);
    const bool is_hide_3 = (hide_level >= 3);

    // オプションの設定
    cmdline::parser ps;
    ps.add<string>   ("hex", 'x', "file", "hex file to run without script file", false, "", not_hide);
    ps.add<uint64_t> ("max", 'm', "num",  "max cycles to stop simulation", false, INIT_MAX_STEP, not_hide);
    ps.add           ("cycle", 'c', "", "cycle simulation mode", is_hide_1);
    ps.add<string>   ("scr", 's', "file", "script file", false, "", not_hide);
    ps.add<string>   ("msglvl", 'l', "str", "message level {ERR|WAR|EXP|INF}", false, "ERR", not_hide);
    ps.add<string>   ("out", 'o', "file", "log file name (one file for all HT)", false, "", not_hide);
    ps.add<string>   ("out-multi", 'O', "file",
            "log file name (separated files for each HT)", false, "", not_hide);
    ps.add           ("debug", 'd', "",
            "print trace (sub-options: rw pipe dep bra systime cache timer intc perfcnt fput)", not_hide);
#if defined(BUILD_G3MH_ONLY)
#else //defined(BUILD_G3MH_ONLY)
    ps.add<string>   ("arch-class", 0, "str",
            "select E3v5 architecture class {F|V|S}", false, "S", is_hide_1);
    ps.add<string>   ("core-type", 0, "str",
            "select CPU core type {G4MH|G4MH1.0|G4MH1.1|G4MH2.0|G4MH2.1}. G4MH is G4MH1.0", false, "G4MH", not_hide);
#endif //defined(BUILD_G3MH_ONLY) || defined(HIDE_OPT)
    ps.add<uint32_t> ("penum", 0, "num", "number of PE", false, N_PE, not_hide);
    ps.add<uint32_t> ("vmnum", 0, "num", "number of VM per PE", false, N_VM_PER_PE, is_hide_1);
    ps.add<uint32_t> ("htnum", 0, "num", "number of HT per PE", false, N_HT_PER_PE, is_hide_1);
    ps.add<string>   ("memini", 0, "hex", "initial value of memory data (byte)", false, "FF", not_hide);
    ps.add<uint16_t> ("snooze-period", 0, "num", "stopped period of SNOOZE instruction",
                      false, 32, not_hide);
    ps.add<uint32_t> ("rom-wait", 0, "num", "change rom latency", false, INIT_ROM_WAIT, is_hide_1);
    ps.add<string>   ("halt-break", 0, "[on|off]",
            "stop simulation when all HTs halt", false, "on", not_hide);
    ps.add           ("use-stopio", 0, "", "enable STOPIO to stop simulation", not_hide);
    ps.add<uint16_t> ("gdb", 0, "port", "connect gdb", false, 0, not_hide);
    ps.add<uint32_t> ("uart", 0, "port", "connect uart", false, 0, not_hide);
    ps.add<uint32_t> ("uart-num", 0, "num", "number of uart channel", false, 0, not_hide);
#ifdef HIDE_OPT
#else //HIDE_OPT
    ps.add           ("isstrace", 0, "", "dump trace log same as RTL trace format", not_hide);
#endif //HIDE_OPT
    ps.add           ("enable-ic", 0, "", "enable instruction cache at default", not_hide);
    ps.add           ("disable-ic", 0, "", "disable instruction cache at default", not_hide);
#ifdef ENABLE_DCACHE
    ps.add           ("enable-dc", 0, "", "enable data cache at default", is_hide_3);
#endif // ENABLE_DCACHE
    ps.add<string>   ("fpu-exist", 0, "[yes|no]", "select FPU HW existence", false, "yes", not_hide);
    ps.add<string>   ("simd-exist", 0, "[yes|no]", "select SIMD(FXU) HW existence", false, "no", is_hide_1);
    ps.add<string>   ("fxu-exist", 0, "[yes|no]", "select FXU HW existence", false, "no", not_hide);
    ps.add<string>   ("mpu-exist", 0, "[yes|no]", "select MPU HW existence", false, "yes", not_hide);
    ps.add<string>   ("mmu-exist", 0, "[yes|no]", "select MMU HW existence", false, "no", is_hide_1);
    ps.add<string>   ("hv-exist", 0, "[yes|no]", "select Virtualization HW existence", false, "no", not_hide);

#ifdef ENABLE_DCACHE
    ps.add<uint32_t> ("dc-fill", 0, "num",
            "change the additional fill latency for data cache {0, 2}",
            false, ForestInternals::INIT_DC_FILL_LATENCY, is_hide_3);
    ps.add<uint32_t> ("dc-hit", 0, "num", "change the hit latency for data cache",
            false, ForestInternals::INIT_DC_HIT_LATENCY, is_hide_3);
#endif //ENABLE_DCACHE
    ps.add<uint32_t> ("fetch-stage", 0, "num", "Set additional the num of fetch stage",
            false, ForestInternals::INIT_ADD_FETCH_STAGE, is_hide_3);
#ifdef USE_DBUF_CMDOPT
    ps.add<uint32_t> ("dbuf-entry", 0, "num", "change num of DBUF entry",
            false, ForestInternals::INIT_DBUF_DEPTH, is_hide_3);
    ps.add<uint32_t> ("dbuf-line-size", 0, "num",
            "change line size(byte) of DBUF entry by 1 << num",
            false, ForestInternals::INIT_DBUF_ADDR_SHIFT, is_hide_3);
    ps.add<uint32_t> ("dbuf-burst-num", 0, "num", "change burst num to fill DBUF",
            false, ForestInternals::INIT_DBUF_FILL_BURST_SIZE, is_hide_3);
#endif // USE_DBUF_CMDOPT
    ps.add<string>   ("dbuf-preload", 0, "[no|plus|minus]",
            "change DBUF preload on flash-256bit", false, "no", is_hide_3);
#ifdef USE_IC_LINE_SIZE_CMDOPT
    ps.add<uint32_t> ("ic-line-size", 0, "num", "change line size(byte) of IC",
            false, ForestInternals::INIT_IC_LINE_BYTES, is_hide_3);
#endif // USE_IC_LINE_SIZE_CMDOPT
    ps.add<uint32_t> ("flash-width", 0, "num", "change flash width to 16 or 32byte",
            false, ForestInternals::INIT_FLASH_WIDTH, is_hide_3);
    ps.add           ("stat", 0, "", "Show CPU statistics", not_hide);
    ps.add           ("help", 0, "", "help", not_hide);
    ps.add           ("version", 'v', "", "version", not_hide);
#ifdef ENABLE_PEID_POS_CTRL
    ps.add<string>   ("peid-pos", 0, "[g3|g4]", "Select PEID bit position in PEID register from g3=[18:16], g4=[2:0] (GMPEID isn't changed)",
            false, "g4");
#endif // #ifdef ENABLE_PEID_POS_CTRL
    ps.add<int32_t>  ("fpu-lat", 0, "num", "Set offset to change FPU latency",
            false, ForestUtil::INIT_FPU_LATENCY, is_hide_3);
    ps.add<int32_t>  ("divf-lat", 0, "num", "Set offset to change divf.s latency",
            false, ForestUtil::INIT_DIVF_LATENCY, is_hide_3);
    ps.add<uint32_t> ("ras-num", 0, "num", "Set entry size for RAS",
            false, ForestInternals::INIT_RAS_NUM, is_hide_3);
    ps.add<string>   ("mem-order", 0, "[tso|so]", "select memory ordering type from TSO and SO",
            false, "tso", is_hide_3);
#ifdef ENABLE_EXE_ORDER_CTRL
    ps.add<string>   ("exe-order", 0, "[ino|ooo]", "Select execution type from InO and OoO for G4MH",
            false, "ooo");
#endif // #ifdef ENABLE_EXE_ORDER_CTRL
    ps.add           ("print-pythonif", 0, "", "enable printing result value in pythonif", is_hide_3);
    ps.add           ("auto-perf", 0, "", "Work performance info with Perfcnt function", is_hide_3);
#ifdef CLUSTER
    ps.add           ("bus-trace", 0, "", "Show Bus access trace", is_hide_3);
#endif // CLUSTER
    ps.add           ("profiler", 0, "", "Load profiler module of Python", is_hide_3);

    // 各オプションの処理
    if (ps.parse (argc, argv) == false) {
        m_forest_util->PrintVersion (stderr);
        cerr << ps.error_full() << ps.usage();
        exit (EXIT_NORMAL);
    }

    if (ps.exist ("help") == true) {
        m_forest_util->PrintVersion (stderr);
        cerr << ps.error_full() << ps.usage();
        exit (EXIT_NORMAL);
    }

    if (ps.exist ("version") == true) {
        m_forest_util->PrintVersion (stderr);
        exit (EXIT_NORMAL);
    }

    if (ps.exist ("print-pythonif") == true) {
            EnablePrintPythonIF (true);
    }else{
            EnablePrintPythonIF (false);
    }

    if (ps.exist ("auto-perf") == true) {
            SetWorkPerfInfoWithPerfcnt (true);
    }else{
            SetWorkPerfInfoWithPerfcnt (false);
    }

#ifdef CLUSTER
    if (ps.exist ("bus-trace") == true) {
        SetBusTrace (true);
    }else{
        SetBusTrace (false);
    }
#endif // CLUSTER

    if (ps.exist ("profiler") == true) {
        SetEnableProfiler (true);
    }

#if defined(BUILD_G3MH_ONLY)
#else //defined(BUILD_G3MH_ONLY)
    if (ps.exist ("arch-class") == true) {
        string arch_class = ps.get<string> ("arch-class");
        if      (arch_class == "F") { SetArchClass (E3V5_F); }
        else if (arch_class == "V") { SetArchClass (E3V5_V); }
        else if (arch_class == "S") { SetArchClass (E3V5_S); }
        else {
            cout << "unknown arch-class option: " << arch_class << endl;
            exit (EXIT_NORMAL);
        }
    }

    if (ps.exist ("core-type") == true) {
        string core_type = ps.get<string> ("core-type");
        if   (core_type == "G4MH")   { SetCoreType (CORE_G4MH);    SetCoreVersion (CORE_VER_10);}
        else if (core_type == "G4MH1.0")   { SetCoreType (CORE_G4MH);    SetCoreVersion (CORE_VER_10);}
        else if (core_type == "G4MH1.1")   { SetCoreType (CORE_G4MH);    SetCoreVersion (CORE_VER_11);}
        else if (core_type == "G4MH2.0")   { SetCoreType (CORE_G4MH);    SetCoreVersion (CORE_VER_20);}
        else if (core_type == "G4MH2.1")   { SetCoreType (CORE_G4MH);    SetCoreVersion (CORE_VER_21);}
        else if (core_type == "G4KH")   { SetCoreType (CORE_G4KH);    SetCoreVersion (CORE_VER_10);}
        else {
            cout << "unknown core-type option: " << core_type << endl;
            exit (EXIT_NORMAL);
        }
    }
#endif //defined(BUILD_G3MH_ONLY)

    if (ps.exist ("penum") == true) {
        uint32_t pe_num = ps.get<uint32_t> ("penum");
        if (pe_num < 1 || pe_num > MAX_PE) {
            cout << "penum is out of 1-" << MAX_PE << ": " << pe_num << endl;
            exit (EXIT_NORMAL);
        }
        SetPeNumFromDbg (pe_num);
    }

    if (ps.exist ("vmnum") == true) {
        cout << "vmnum option is deprecated." << endl;
        uint32_t vc_num = ps.get<uint32_t> ("vmnum");
        if (vc_num > MAX_VM) {
            cout << "vmnum is out of 0-" << MAX_VM << ": " << vc_num << endl;
            exit (EXIT_NORMAL);
        }
        SetVcNumFromDbg (vc_num);
    }

    if (ps.exist ("htnum") == true) {
        cout << "htnum option is deprecated." << endl;
        uint32_t tc_num = ps.get<uint32_t> ("htnum");
        if (tc_num > MAX_HT) {
            cout << "htnum is out of 0-" << MAX_HT << ": " << tc_num << endl;
            exit (EXIT_NORMAL);
        }
        SetTcNumFromDbg (tc_num);
    }

    if (ps.exist ("hex") == true) {
        SetHexName (ps.get<string> ("hex"));
    }

    if (ps.exist ("cycle") == true) {
        // Removed.
    }

    if (ps.exist ("max") == true) {
        SetMaxStep (ps.get<uint64_t> ("max"));
        SetMaxCmdOpt (true);
    }

#ifdef USE_DBUF_CMDOPT
    if (ps.exist ("dbuf-entry") == true) {
        SetDbufDepth (ps.get<uint32_t> ("dbuf-entry"));
    }
    if (ps.exist ("dbuf-line-size") == true) {
        SetDbufLineShift (ps.get<uint32_t> ("dbuf-line-size"));
    }
    if (ps.exist ("dbuf-burst-num") == true) {
        SetDbufFillBurstNum (ps.get<uint32_t> ("dbuf-burst-num"));
    }
#endif // USE_DBUF_CMDOPT
#ifdef USE_IC_LINE_SIZE_CMDOPT
    if (ps.exist ("ic-line-size") == true) {
        SetIcLineByte (ps.get<uint32_t> ("ic-line-size"));
    }
#endif // USE_IC_LINE_SIZE_CMDOPT
    if (ps.exist ("flash-width") == true) {
        uint32_t flash_width = ps.get<uint32_t> ("flash-width");
        if ((flash_width != 16) && (flash_width != 32)) {
            cout << "invalid flash-width size: " << flash_width << endl;
            exit (EXIT_NORMAL);
        }
        SetFlashWidthByte (flash_width);
    }

    if (ps.exist ("dbuf-preload") == true) {
        string preload_str = ps.get<string> ("dbuf-preload");
        DbufPreloadType type;
        if      (preload_str == "no")    { type = DBUF_PRLD_NO; }
        else if (preload_str == "plus")  { type = DBUF_PRLD_PLUS; }
        else if (preload_str == "minus") { type = DBUF_PRLD_MINUS; }
        else {
            cout << "unknown dbuf-preload option: " << preload_str << endl;
            exit (EXIT_NORMAL);
        }

        if ((type != DBUF_PRLD_NO) && (GetFlashWidthByte () != 32) &&
                (GetDbufFillBurstNum() != 1)) {
            cout << "dbuf-preload must be enabled with --flash-width 32 and --dbuf-burst-num 1"
                 << endl;
            exit (EXIT_NORMAL);
        }

        SetDbufPreloadType (type);
    }

    if (ps.exist ("out") == true) {
        SetOutFname (ps.get<string> ("out"));
    }
    if (ps.exist ("out-multi") == true) {
        EnableOutMulti (true);
        SetOutFname (ps.get<string> ("out-multi"));
    }
    if (ps.exist ("scr") == true) {
        SetScriptName (ps.get<string> ("scr"));
    }
    if (ps.exist ("use-stopio") == true) {
        EnableStopio (true);
    }
    if (ps.exist ("isstrace") == true) {
        EnableIssTrace (true);
    }
    if (ps.exist ("enable-ic") == true) {
        EnableIC (CMD_OPT_ENABLE);
    }
    if (ps.exist ("disable-ic") == true) {
        EnableIC (CMD_OPT_DISABLE);
    }
#ifdef ENABLE_DCACHE
    if (ps.exist ("enable-dc") == true) {
        EnableDC (true);
    }
#endif //ENABLE_DCACHE
    if (ps.exist ("fpu-exist") == true) {
        string fpu_flag = ps.get<string> ("fpu-exist");
        bool has_fpu;
        if      (fpu_flag == "yes")  { has_fpu = true; }
        else if (fpu_flag == "no")   { has_fpu = false; }
        else {
            cout << "unknown fpu-exist option: " << fpu_flag << endl;
            exit (EXIT_NORMAL);
        }
        SetHasFpu (has_fpu);
    }

    if (ps.exist ("simd-exist") == true) {
        string simd_flag = ps.get<string> ("simd-exist");
        bool has_simd;
        if      (simd_flag == "yes")  { has_simd = true; }
        else if (simd_flag == "no")   { has_simd = false; }
        else {
            cout << "unknown simd-exist option: " << simd_flag << endl;
            exit (EXIT_NORMAL);
        }
        cout << "simd-exist option is deprecated. Use --fxu-exist instead." << endl;
        SetHasFxu (has_simd);
    }

    if (ps.exist ("fxu-exist") == true) {
        string fxu_flag = ps.get<string> ("fxu-exist");
        bool has_fxu;
        if      (fxu_flag == "yes")  { has_fxu = true; }
        else if (fxu_flag == "no")   { has_fxu = false; }
        else {
            cout << "unknown fxu-exist option: " << fxu_flag << endl;
            exit (EXIT_NORMAL);
        }
        SetHasFxu (has_fxu);
    }


    if (ps.exist ("mpu-exist") == true) {
        string mpu_flag = ps.get<string> ("mpu-exist");
        bool has_mpu;
        if      (mpu_flag == "yes")  { has_mpu = true; }
        else if (mpu_flag == "no")   { has_mpu = false; }
        else {
            cout << "unknown mpu-exist option: " << mpu_flag << endl;
            exit (EXIT_NORMAL);
        }
        SetHasMpu (has_mpu);
    }

    if (ps.exist ("mmu-exist") == true) {
        string mmu_flag = ps.get<string> ("mmu-exist");
        bool has_mmu;
        if      (mmu_flag == "yes")  { has_mmu = true; }
        else if (mmu_flag == "no")   { has_mmu = false; }
        else {
            cout << "unknown mmu-exist option: " << mmu_flag << endl;
            exit (EXIT_NORMAL);
        }
        cout << "mmu-exist option is deprecated." << endl;
        SetHasMmu (has_mmu);
    }

    if (ps.exist ("hv-exist") == true) {
        string hv_flag = ps.get<string> ("hv-exist");
        bool has_hv;
        if      (hv_flag == "yes")  { has_hv = true; }
        else if (hv_flag == "no")   { has_hv = false; }
        else {
            cout << "unknown hv-exist option: " << hv_flag << endl;
            exit (EXIT_NORMAL);
        }
        SetHasHv (has_hv);
    }

    if (ps.exist ("memini") == true) {
        uint32_t memini_value;
        std::istringstream memini_ss (ps.get<string> ("memini"));
        memini_ss >> std::hex >> memini_value;
        SetMemInit (static_cast<uint8_t> (memini_value));
    }

    if (ps.exist ("snooze-period") == true) {
        SetSnoozePeriod (ps.get<uint16_t> ("snooze-period"));
    }

    if (ps.exist ("rom-wait") == true) {
        SetRomWait (ps.get<uint32_t> ("rom-wait"));
    }

    if (ps.exist ("msglvl") == true) {
        string msglv = ps.get<string> ("msglvl");
        if      (msglv == "ERR") { SetMessageLevel (MSG_ERR); }
        else if (msglv == "WAR") { SetMessageLevel (MSG_WAR); }
        else if (msglv == "EXP") { SetMessageLevel (MSG_EXP); }
        else if (msglv == "INF") { SetMessageLevel (MSG_INF); }
        else {
            cout << "unknown msglvl option: " << msglv << endl;
            exit (EXIT_NORMAL);
        }
    }

    if (ps.exist ("halt-break") == true) {
        string break_flag = ps.get<string> ("halt-break");
        bool is_break_halt_enabled;
        if      (break_flag == "on")  { is_break_halt_enabled = true; }
        else if (break_flag == "off") { is_break_halt_enabled = false; }
        else {
            cout << "unknown halt-break option: " << break_flag << endl;
            exit (EXIT_NORMAL);
        }
        EnableHaltBreak (is_break_halt_enabled);
    }

    if (ps.exist ("gdb") == true) {
        SetGdbPort (ps.get<uint16_t> ("gdb"));
        SetGdbCmdopt (true);
    }

    if (ps.exist ("uart") == true) {
        SetUartSocketPort (ps.get<uint32_t> ("uart") & 0xFFFFU);
        SetUartOption (GetUartOption () | UART_PORT_OPT);
        SetUartCmdopt (true);
    }

    if (ps.exist ("uart-num") == true) {
        SetUartChannelNum (ps.get<uint32_t> ("uart-num"));
        SetUartOption (GetUartOption () | UART_CH_OPT);
    }

    if (ps.exist ("stat") == true) {
        SetShowStatistics (true);
    }

#ifdef ENABLE_PEID_POS_CTRL
    if (ps.exist ("peid-pos") == true) {
        string peid_flag = ps.get<string> ("peid-pos");
        bool is_g4x_peid;
        if      (peid_flag == "g4") { is_g4x_peid = true; }
        else if (peid_flag == "g3") { is_g4x_peid = false; }
        else {
            cout << "unknown --peid_pos option: " << peid_flag << endl;
            exit (EXIT_NORMAL);
        }
        SetPeidPosG4x (is_g4x_peid);
    }
#endif // #ifdef ENABLE_PEID_POS_CTRL

#ifdef ENABLE_DCACHE
    if (ps.exist ("dc-fill") == true) {
        SetDcacheFillLatency (ps.get<uint32_t> ("dc-fill"));
    }

    if (ps.exist ("dc-hit") == true) {
        //SetDcacheHitLatency (ps.get<uint32_t> ("dc-hit"));
    }
#endif //ENABLE_DCACHE
    if (ps.exist ("fetch-stage") == true) {
        SetAdditonalFetchStage (ps.get<uint32_t> ("fetch-stage"));
    }

    if (ps.exist ("fpu-lat") == true) {
        SetFpuLatencyOffset (ps.get<int32_t> ("fpu-lat"));
    }
    if (ps.exist ("divf-lat") == true) {
        SetDivfLatencyOffset (ps.get<int32_t> ("divf-lat"));
    }
    if (ps.exist ("ras-num") == true) {
        uint32_t ras_num = ps.get<uint32_t> ("ras-num");
        if (ras_num == 0) {
            cout << "invalid ras_num size: " << ras_num << endl;
            exit (EXIT_NORMAL);
        }
        SetRasNum (ras_num);
    }
    if (ps.exist ("mem-order") == true) {
        string mem_order_flag = ps.get<string> ("mem-order");
        bool is_tso_enabled;
        if      (mem_order_flag == "tso")  { is_tso_enabled = true; }
        else if (mem_order_flag == "so")   { is_tso_enabled = false; }
        else {
            cout << "unknown mem-order option: " << mem_order_flag << endl;
            exit (EXIT_NORMAL);
        }
        SetTso (is_tso_enabled);
    }

#ifdef ENABLE_EXE_ORDER_CTRL
    if (ps.exist ("exe-order") == true) {
        string exe_order_flag = ps.get<string> ("exe-order");
        bool is_ooo_enabled;
        if      (exe_order_flag == "ooo")  { is_ooo_enabled = true; }
        else if (exe_order_flag == "ino")   { is_ooo_enabled = false; }
        else {
            cout << "unknown exe-order option: " << exe_order_flag << endl;
            exit (EXIT_NORMAL);
        }
        SetExecOrderOoOG4MH (is_ooo_enabled);
    }
#endif // #ifdef ENABLE_EXE_ORDER_CTRL

    // --debug処理 (--debug の後でなくても debugオプションとみなす)
    DebugMode dbg_mode = DBG_OFF;
    if (ps.exist ("debug") == true) {
        dbg_mode |= DBG_ON;
    }
    for (uint32_t i = 0; i < ps.rest().size(); i++) {
        if      (ps.rest()[i] == "rw")   { dbg_mode |= DBG_RW; }
        else if (ps.rest()[i] == "pipe") { dbg_mode |= DBG_PIPE; }
        else if (ps.rest()[i] == "dep")  { dbg_mode |= DBG_DEP; }
        else if (ps.rest()[i] == "bra")  { dbg_mode |= DBG_BRA; }
        else if (ps.rest()[i] == "systime") { dbg_mode |= DBG_TIME; }
        else if (ps.rest()[i] == "cache")   { dbg_mode |= DBG_CACHE; }
        else if (ps.rest()[i] == "stall")   { dbg_mode |= DBG_STALL; }
        else if (ps.rest()[i] == "timer")   { dbg_mode |= DBG_TIMER; }
        else if (ps.rest()[i] == "intc")    { dbg_mode |= DBG_INTC; }
        else if (ps.rest()[i] == "perfcnt") { dbg_mode |= DBG_PERF; }
        else if (ps.rest()[i] == "fput")    { dbg_mode |= DBG_FPUT; }
        else {
            cout << "unknown option: " << ps.rest()[i] << endl;
            cerr << ps.error_full() << ps.usage();
            exit (EXIT_NORMAL);
        }
    }
    // --debugのみは --debug rw 相当
    if (dbg_mode == DBG_ON) { dbg_mode |= DBG_RW | DBG_PERF; }

    // When --debug is omitted, --debug perf is enabled.
    if (dbg_mode == DBG_OFF) { dbg_mode |= DBG_PERF;}
    SetDebugMode (dbg_mode);

    // 違反処理
    if ((ps.exist ("out") == true) && (ps.exist ("out-multi") == true)) {
        cout << "--out and --out-multi can't be used simultaneously" << endl;
        exit (EXIT_NORMAL);
    }
    if ((ps.exist ("enable-ic") == true) && (ps.exist ("disable-ic") == true)) {
        cout << "--enable-ic and disable-ic can't be used simultaneously" << endl;
        exit (EXIT_NORMAL);
    }

    if ((m_uart_option == UART_PORT_OPT) ||
        (m_uart_option == UART_CH_OPT) ||
        ((m_uart_option == UART_BOTH_OPT) && (m_uart_channel_num == 0))) {
        // --uartと--uart-numのいずれか一方しか設定されていない
        // または、両方設定されているがUARTチャネル数が0(--uart-num 0)の場合、エラー終了
        cout << "either --uart or --uart-num is set. or --uart-num is 0." << endl;
        exit (EXIT_NORMAL);
    }
#endif // SUPPORT_CUBESUITE
}


ForestCmdOpt::ForestCmdOpt (ForestUtil* forest_util)
    : m_forest_util (forest_util)
{
    m_dbg_mode = DBG_OFF;
    m_max_step = INIT_MAX_STEP;
    m_is_set_max = false;
    m_memini_value = 0xFF;
    m_isstrace_flag = false;
#if defined (COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)
    m_enable_ic = CMD_OPT_ENABLE;
#else  //defined (COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)
    // in case default and COMP_RUNNER_G3KH
    m_enable_ic = CMD_OPT_NOSEL;
#endif //defined (COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)
    m_enable_dc = false;
    m_script_fname = "";
    m_out_fname = "";
    m_enable_out_multi = false;
    m_hex_fname = "";
    m_is_set_gdb_cmdopt = false;
    m_uart_option = UART_NO_OPT;
    m_uart_channel_num = 0;
    m_uart_socket_port = ForestUtil::UART_PORT_ID_SOCKET;
    m_is_stopio_enabled = false;
    m_is_break_halt_enabled = true;
    m_rom_wait = INIT_ROM_WAIT;
    m_msglvl = MSG_ERR;
    m_is_set_uart_cmdopt = false;
    m_has_fpu  = true;
    m_has_fxu  = false;
    m_has_mpu  = true;
    m_has_mmu  = false;
    m_has_hv   = false;

    m_pe_num = N_PE;
    m_vc_num = N_VM_PER_PE;
    m_tc_num = N_HT_PER_PE;

    m_arch_class = E3V5_F;
    m_core_type = CORE_G4MH;
    m_core_version = CORE_VER_10;

    m_snooze_period = INIT_SNOOZE_PERIOD;
    m_pe_to_pe_latency = INIT_PE_LATENCY;

    m_dc_fill_latency = ForestInternals::INIT_DC_FILL_LATENCY;
    m_dc_hit_latency = ForestInternals::INIT_DC_HIT_LATENCY;
    m_additonal_fetch_stage = ForestInternals::INIT_ADD_FETCH_STAGE;

    m_show_staticstics = ForestUtil::INIT_SHOW_STATISTICS;

    m_fpu_lat_offset = ForestUtil::INIT_FPU_LATENCY;
    m_divf_lat_offset = ForestUtil::INIT_FPU_LATENCY;
    m_ras_num = ForestInternals::INIT_RAS_NUM;
    m_is_tso = ForestInternals::INIT_TSO_MODE;
#ifdef ENABLE_EXE_ORDER_CTRL
    m_is_ooo_g4mh = ForestInternals::INIT_OOO_MODE;
#endif // #ifdef ENABLE_EXE_ORDER_CTRL

    m_dbuf_depth = ForestInternals::INIT_DBUF_DEPTH;
    m_dbuf_line_shift = ForestInternals::INIT_DBUF_ADDR_SHIFT;
    m_dbuf_fill_num = ForestInternals::INIT_DBUF_FILL_BURST_SIZE;
    m_dbuf_preload_type = ForestInternals::INIT_DBUF_PRELOAD_TYPE;
#ifdef USE_IC_LINE_SIZE_CMDOPT
    m_ic_line_byte = ForestInternals::INIT_IC_LINE_BYTES;
#endif //#ifdef USE_IC_LINE_SIZE_CMDOPT
    m_flash_width_byte = ForestInternals::INIT_FLASH_WIDTH;
    m_work_perfinfo_with_perfcnt = false;
#ifdef ENABLE_PEID_POS_CTRL
    m_is_peid_g4x = true;
#endif // #ifdef ENABLE_PEID_POS_CTRL
    m_is_enable_profiler = false;
}
