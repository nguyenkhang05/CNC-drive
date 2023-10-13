/*
 * (c) 2011,2012 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */


#include "./cmp_runner_g4mh.h"

/** \file cmp_runner_g4mh.cpp
 * \brief CompRunner G4MH implementation source file.
 *
 * This file contains the internal functions in CompRunner: G4MHv2, ...
 */


/**
 * \var FILE* g_cmp_fp
 * \brief Global pointer for CompRunner log file.
 *
 * This pointer is used to create the content of log file.
 * All functions will print the details info to this log file.
 *
 */
FILE* g_cmp_fp = NULL;

/**
 * \var cmprunner::CmpRunnerIF* cforestif
 * \brief Pointer to CompRunner I/F internal functions.
 *
 * This pointer is used to call interal function in CompRunner I/F.
 *
 */
cmprunner::CmpRunnerIF* cforestif = NULL;

/**
 * \var bool g_comp_misr_show
 * \brief Global pointer for misr status.
 *
 * This pointer is used to manage misr status.
 * It will be used in misr.cpp file.
 *
 */
bool g_comp_misr_show = false;

/*!
  \def RESULT_CYC_CMPARE
  Define the value for RESULT_CYC_CMPARE.
  It will be used in g_result_mode variable.
*/
#define RESULT_CYC_CMPARE       0

/*!
  \def RESULT_INST_LIST
  Define the value for RESULT_INST_LIST.
  It will be used in g_result_mode variable.
*/
#define RESULT_INST_LIST        1

/*!
  \def CYCLE_PERIOD
  Define the default value for CYCLE_PERIOD.
  User can change this value.
*/
#define CYCLE_PERIOD         3000

/*!
  \def PE0
  Define the value for PE0.
  This definition for easy reference, maintenance.
*/
#define PE0                     0

/*!
  \def PE1
  Define the value for PE1.
  This definition for easy reference, maintenance.
*/
#define PE1                     1

#ifdef ENABLE_COMP_DEBUG
#define DEBUG_NEED 1
#endif //ENABLE_COMP_DEBUG


void ComprunnerG4MHv2::DEBUG_LOG(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, const std::string *msg) {
#if DEBUG_NEED
    std::ostringstream oss;
    CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
    g_cycle_history.PrintSpace ();
    CmpPrintSpace (&oss, 27);
    oss << "[" << *msg << "]";
    fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
#endif
}


void ComprunnerG4MHv2::init_CmpG4MHv2(uint32_t rtl_pe_number) {
    for (uint32_t peid = 0; peid < rtl_pe_number; peid++) {
        rtl_FL_Wen_chk.push_back (cmprunner::UNUSED_VAL);
        rtl_FL_val_chk.push_back (cmprunner::UNUSED_VAL);

        g_cmp_error_count.push_back (0);
        g_cmp_final_status.push_back (0);
        g_cmp_mcyc_middle.push_back (0);

        g_cmp_eiitb_status.push_back (false);
    }
}


uint32_t ComprunnerG4MHv2::G4MHv2_CForestInit (uint32_t rtl_pe_number, uint32_t cmprunner_log, char *cmprunner_log_path, char *srec_file, char *python_name, uint32_t Cmprunner_Enable)
{
    if (cmprunner_log==1) {
        g_cmp_fp = fopen (cmprunner_log_path, "w");
    } else {
        g_cmp_fp = fopen ("cmpcforest.log", "w");
    }
    if (!g_cmp_fp) {
        printf("<Error: can not open file(%s) for a write>\n", cmprunner_log_path);
        exit(EXIT_FAILURE);
    }
    // Cycle comparison intialization.
    if (Cmprunner_Enable) {
        m_cmprunner_en = 1;
        g_cycle_history.SetCyclePeriod (CYCLE_PERIOD);
        g_result_mode = RESULT_CYC_CMPARE;
        rtl_pre_last_cm = true;
        m_cyc_cmp_en = false ;


        cforestif = new cmprunner::CmpRunnerIF ();

        std::ostringstream oss;
        oss << python_name;
        if (python_name != NULL) {
            //Loading python depends on PE number
            FILE *py_fp;
            if ((py_fp = fopen (oss.str().c_str(), "r")) != NULL) {
                fprintf (g_cmp_fp, "<Info: Loading %s file>\n", oss.str().c_str());
                cforestif->StartPy (srec_file, 0x0, 0x0, &oss, NULL);
                fclose (py_fp);
            } else {
                fprintf (g_cmp_fp, "<Error: Cannot find %s file>\n", oss.str().c_str());
                return COMP_DPI_SV_ERR;
            }
        }else {
            fprintf (g_cmp_fp, "<Error: Python file is empty>\n");
            return COMP_DPI_SV_ERR;
        }

        cforestif->SetFileFP (g_cmp_fp);

        fprintf (g_cmp_fp, "<CompRunner : %s>\n", cforestif->GetVersionStr ().c_str());
        g_comp_misr_show = cforestif->IsCompMisrShow ();
        fprintf (g_cmp_fp, "<g_comp_misr_show = %d>\n", g_comp_misr_show);

        if (cforestif->CmpLoadSrec (srec_file) != cmprunner::API_E_OK) {
            fprintf (g_cmp_fp, "<Error: Unexpected error at CmpLoadSrec>\n");
            return COMP_DPI_C_ERR;
        }

        if (cforestif->GetCmpStage() == STAGE_DP){
            fprintf (g_cmp_fp, "<Diff field - Compare cycle at DP stage>\n");
        } else if (cforestif->GetCmpStage() == STAGE_EA){
            fprintf (g_cmp_fp, "<Diff field - Compare cycle at EA stage>\n");
        } else if (cforestif->GetCmpStage() == STAGE_WB) { // @@ TODO Change to CM stage when commit cycle is supported
            fprintf (g_cmp_fp, "<Error: Waiting for Cforest support CM cycle>\n");
            return COMP_DPI_C_ERR;
        } else {  //Never go here
            fprintf (g_cmp_fp, "<Error: Invalid input stage to compare cycle>\n");
            return COMP_DPI_C_ERR;
        }
        CmpPrintTitle();
        init_CmpG4MHv2(rtl_pe_number);
        return COMP_DPI_OK;
    }
    else {
        m_cmprunner_en = 0;
        fprintf (g_cmp_fp, "<CMPRUNNER IS DISABLED>\n");
        return COMP_DPI_OK;
    }
}


void ComprunnerG4MHv2::G4MHv2_CForestClose (void)
{
    if (m_cmprunner_en) {
        if (m_cyc_cmp_en) {
            fprintf (g_cmp_fp, "< Cycle comparison - STOP  =================== >\n");
        }
        if (g_result_mode == RESULT_INST_LIST) {
            g_cycle_history.PrintInstFrequency(PE0);     // PEID0
        } else {
            g_cycle_history.Finalize (PE0);     // PEID0
            g_cycle_history.PrintInst(PE0);     // PEID0
            g_cycle_history.PrintSubtotal(PE0); // PEID0
            g_cycle_history.PrintRanking(PE0);  // PEID0
            g_cycle_history.PrintFinal(PE0);    // PEID0
        }
    }
    return ;
}


uint32_t ComprunnerG4MHv2::G4MHv2_GetTotalError (uint32_t rtl_peid)
{
    if (m_cmprunner_en) {
        std::ostringstream oss;
        oss << std::endl;
        CmpPrintBorder(&oss);
        // CompRunner will notify ERR
        // in case operand syserr is requested in Cforest
        // but there's no operand syserr request sent from RTL
        if (cforestif->GetOperandSyserrStatus (rtl_peid, true, 0)) {
            uint32_t g_cur_error_count = 0;
            // Update error count.
            g_cur_error_count = g_cmp_error_count.at(rtl_peid) + 1;
            g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
            g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, g_cur_error_count);

            CmpPrintSpace (&oss, 57);
            CmpPrintErrorCount(&oss, rtl_peid);
            oss << "Host - operand syserr flag isn't clear]" << std::endl;
        }
        for (uint32_t gpid = 0 ; gpid <=7 ; gpid++) {
            if (cforestif->GetOperandSyserrStatus (rtl_peid, false, gpid)) {
                uint32_t g_cur_error_count = 0;
                // Update error count.
                g_cur_error_count = g_cmp_error_count.at(rtl_peid) + 1;
                g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
                g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, g_cur_error_count);

                CmpPrintSpace (&oss, 57);
                CmpPrintErrorCount(&oss, rtl_peid);
                oss << "Guest " << gpid << " - operand syserr flag isn't clear]" << std::endl;
            }
        }
        CmpPrintSpace (&oss, 57);
        oss << "[GetTotalError] ";
        if (!cforestif->IsPeIdValid(rtl_peid)) {
            oss << "<Error: PEID" << rtl_peid << " is invalid>";
            fprintf (g_cmp_fp, "%s", oss.str().c_str());
            return COMP_DPI_SV_ERR;
        }

        if ((g_cmp_error_count.at(rtl_peid) == 0) && (g_cmp_final_status.at(rtl_peid) == 1)) {
            // Update error count.
            g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
            g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, 1);
            oss << "Final status is FAILT ---> Adjust error number to 1";
        } else {
            oss << "Number of error in PEID" << rtl_peid << " : "<< g_cmp_error_count.at(rtl_peid);
        }
        oss << std::endl;
        CmpPrintBorder(&oss);
        fprintf (g_cmp_fp, "%s", oss.str().c_str());
        return g_cmp_error_count.at(rtl_peid);
    }
    else {
        return 0;
    }
}


uint32_t ComprunnerG4MHv2::G4MHv2_CmpStep (uint32_t rtl_peid, uint64_t rtl_dp_time, uint64_t rtl_ea_time, uint64_t rtl_cm_time, uint32_t rtl_cycle, uint32_t rtl_pc, uint32_t rtl_last_commit, uint32_t rtl_c2b1_commit,
                      uint32_t rtl_ooo_commit, uint32_t rtl_mcyc_commit, uint32_t rtl_regwrite_en, uint32_t rtl_reg_num, svLogicVecVal* rtl_gr_val, svLogicVecVal* rtl_wr_val_0, svLogicVecVal* rtl_wr_val_1,
                      svLogicVecVal* rtl_wr_val_2, svLogicVecVal* rtl_wr_val_3, uint32_t rtl_flagwrite_en, uint32_t rtl_flag_val, uint32_t rtl_brk_ch, uint32_t rtl_nblk_id, uint32_t rtl_xdump_ctrl)
{
    if (m_cmprunner_en) {
        TraceCtrl* trace = NULL;
        cmprunner::CmpResult cmp_ret = cmprunner::CMP_MATCH;
        std::ostringstream oss;
        uint32_t iss_PC_c2b1_1st = cmprunner::UNUSED_VAL, iss_GR_no_c2b1 = cmprunner::UNUSED_VAL, iss_GR_val_c2b1 = cmprunner::UNUSED_VAL;
        // General_Purpose_register_write_enable values in CForest for c2b1 insn
        uint32_t iss_GR_Wen_c2b1_1st = cmprunner::UNUSED_VAL, iss_GR_Wen_c2b1_2nd = cmprunner::UNUSED_VAL;
        // Flag_write_enable values in CForest for c2b1 insn
        uint32_t iss_FL_Wen_c2b1_1st = cmprunner::UNUSED_VAL, iss_FL_Wen_c2b1_2nd = cmprunner::UNUSED_VAL;
        uint32_t iss_FL_val_c2b1_1st = cmprunner::UNUSED_VAL, iss_FL_val_c2b1_2nd = cmprunner::UNUSED_VAL;
        // Keep Flag variables in C side for comparison later.
        uint32_t iss_FL_Wen = cmprunner::UNUSED_VAL, iss_FL_val = cmprunner::UNUSED_VAL;

        uint32_t g_step_cmp_mcyc_middle = 0, g_cur_error_count = 0;
        uint64_t rtl_time;
        uint64_t start_pc, end_pc;

        //Compare cycle - stage control
        if (cforestif->GetCmpStage() == STAGE_DP) {
            rtl_time = rtl_dp_time ;
        } else if (cforestif->GetCmpStage() == STAGE_EA) {
            rtl_time = rtl_ea_time;
        } else {
            rtl_time = rtl_cm_time;
        }

        if (rtl_pre_last_cm) {
            if (!rtl_last_commit) {
                rtl_1st_cm_time = rtl_time;
            }
        } else {
            rtl_time = rtl_1st_cm_time;
        }
        rtl_pre_last_cm = rtl_last_commit ? true : false;
        const std::string msg = "CmpStep";
        DEBUG_LOG(rtl_peid, rtl_time, rtl_cycle, &msg);

        //Compare cycle - PC control
        if (!m_cyc_cmp_en) {
            start_pc = cforestif->GetCycCmpStartPC();
            if ((start_pc == 0x0) || (start_pc == rtl_pc) ) {
                m_cyc_cmp_en = true ;
                fprintf (g_cmp_fp, "< Cycle comparison - START =================== >\n");
            }
        } else {
            end_pc = cforestif->GetCycCmpEndPC();
            if (end_pc == rtl_pc) {
                m_cyc_cmp_en = false ;
                fprintf (g_cmp_fp, "< Cycle comparison - STOP  =================== >\n");
            }
        }

#if DEBUG_NEED
        // This function shows the debug info for ComprunnerG4MHv2::G4MHv2_CmpStep function.
        cforestif->DB_DPI_CmpStep(rtl_peid, rtl_time, rtl_cycle, rtl_pc, rtl_last_commit, rtl_c2b1_commit, rtl_ooo_commit, rtl_mcyc_commit,
                                  rtl_regwrite_en, rtl_reg_num, rtl_gr_val, rtl_wr_val_0, rtl_wr_val_1, rtl_wr_val_2, rtl_wr_val_3,
                                  rtl_flagwrite_en, rtl_flag_val, rtl_brk_ch, rtl_nblk_id, rtl_xdump_ctrl);
#endif //DEBUG_NEED

        if (!cforestif->IsPeIdValid(rtl_peid)) {
            std::ostringstream reg_invalid;
            reg_invalid << "PEID = " << rtl_peid;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, 0x0, 7, 0);
            return COMP_DPI_SV_ERR;
        }

        // Collect RTL values for comparison step
        if (rtl_flagwrite_en) {
            // Update flag enable.
            rtl_FL_Wen_chk.erase(rtl_FL_Wen_chk.begin() + rtl_peid);
            rtl_FL_Wen_chk.insert(rtl_FL_Wen_chk.begin() + rtl_peid, rtl_flagwrite_en);
            // Update flag value.
            rtl_FL_val_chk.erase(rtl_FL_val_chk.begin() + rtl_peid);
            rtl_FL_val_chk.insert(rtl_FL_val_chk.begin() + rtl_peid, rtl_flag_val);
        }

        // Call SetCanselTimingMcycInst function in ForestUtil in CForest.
        g_step_cmp_mcyc_middle = g_cmp_mcyc_middle.at(rtl_peid) + 1;
        // Update middle step value in case MCYC.
        g_cmp_mcyc_middle.erase(g_cmp_mcyc_middle.begin() + rtl_peid);
        g_cmp_mcyc_middle.insert(g_cmp_mcyc_middle.begin() + rtl_peid, g_step_cmp_mcyc_middle);
        cforestif->SetMcycMiddleCount(rtl_peid, g_cmp_mcyc_middle.at(rtl_peid));

        // Ignore case in G4MH RTL
        // 1. Empty commit in case normal commit
        if ((rtl_last_commit == 0) && (rtl_mcyc_commit == 0) && (rtl_regwrite_en == 0)) {
            return COMP_DPI_OK;
        }
        // 2. GR32 is updated in case complex instruction (like CALLT, SYSCALL)
        // Ignore writing R32 but need to compare PC and others.
        if ((rtl_ooo_commit == 0) && (rtl_regwrite_en == 1) && (rtl_reg_num == cmprunner::CmpR32)) {
            rtl_regwrite_en = 0; // to ignore R32.
        }

        cforestif->CmpGetTrace (rtl_peid, &trace);

        // ============================================== //
        // || Register RTL info to CompRunner buffer. ||
        // ============================================== //

        uint64_t rtl_GR_val_up_chk = 0x0;
        uint64_t rtl_GR_val_lo_chk = 0x0;
        uint32_t rtl_GR_num_chk = 0x0;

        if (rtl_regwrite_en) {
            CmpConvertRegNumStore(rtl_reg_num, rtl_GR_num_chk);
            // Check Nonblocking ID commit.
            if (rtl_ooo_commit) { // rtl_ooo_commit == 1
                if (rtl_GR_num_chk == R0) {
                    cmp_ret = cmprunner::CMP_RTL_ADD_R0_ERR;
                } else {
                    // NBLK ID will be kept as the key for comparison later.
                    cmprunner::APIErrorCode ret_add = cforestif->CmpAddRegInfo(rtl_peid, cmprunner::TYPE_EMP, rtl_nblk_id, rtl_GR_num_chk, 0x0, 0x0);
                    if (ret_add == cmprunner::API_E_OK) {
                        cmp_ret = cmprunner::CMP_RTL_ADD_EMP_OK;
                    }
                    // Return value after R32 is added to CompRunner buffer
                    if (rtl_reg_num == cmprunner::CmpR32) {
                        return COMP_DPI_OK;
                    }
                }
            } else { // rtl_ooo_commit == 0
                if (rtl_GR_num_chk == R0) {
                    cmp_ret = cmprunner::CMP_RTL_ADD_R0_ERR;
                } else {
                    if (rtl_GR_num_chk > R0 && rtl_GR_num_chk <= R31) {
                        // Validate RTL value
                        cmprunner::RtlLogicVal log_rtl_gr_val (rtl_gr_val);
                        if (!log_rtl_gr_val.IsValidSignal()) {
                            std::ostringstream reg_invalid;
                            reg_invalid << "R" << rtl_GR_num_chk;
                            uint32_t rtl_signal_control = rtl_gr_val->bval;
                            uint32_t rtl_data_control = rtl_gr_val->aval;
                            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), rtl_signal_control, rtl_data_control, 0, 0);
                            return COMP_DPI_SV_ERR;
                        }
                        rtl_GR_val_lo_chk = log_rtl_gr_val.getIntVal() & 0x0FFFFFFFF;
                    } else if (rtl_GR_num_chk >= WREG_BASE && rtl_GR_num_chk <= WREG_MAX) {
                        // Validate RTL value
                        cmprunner::RtlLogicVal log_rtl_wr_val_0 (rtl_wr_val_0);
                        cmprunner::RtlLogicVal log_rtl_wr_val_1 (rtl_wr_val_1);
                        cmprunner::RtlLogicVal log_rtl_wr_val_2 (rtl_wr_val_2);
                        cmprunner::RtlLogicVal log_rtl_wr_val_3 (rtl_wr_val_3);
                        if ((!log_rtl_wr_val_0.IsValidSignal()
                          || !log_rtl_wr_val_1.IsValidSignal()
                          || !log_rtl_wr_val_2.IsValidSignal()
                          || !log_rtl_wr_val_3.IsValidSignal())) {
                            std::ostringstream reg_invalid;
                            reg_invalid << "WR" << rtl_GR_num_chk;
                            uint32_t rtl_signal_control = 0;
                            uint32_t rtl_data_control = 0;
                            if (!log_rtl_wr_val_3.IsValidSignal()) {
                                rtl_signal_control = rtl_wr_val_3->bval;
                                rtl_data_control = rtl_wr_val_3->aval;
                                ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), rtl_signal_control, rtl_data_control, 0, 0);
                            }
                            if (!log_rtl_wr_val_2.IsValidSignal()) {
                                rtl_signal_control = rtl_wr_val_2->bval;
                                rtl_data_control = rtl_wr_val_2->aval;
                                ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), rtl_signal_control, rtl_data_control, 0, 0);
                            }
                            if (!log_rtl_wr_val_1.IsValidSignal()) {
                                rtl_signal_control = rtl_wr_val_1->bval;
                                rtl_data_control = rtl_wr_val_1->aval;
                                ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), rtl_signal_control, rtl_data_control, 0, 0);
                            }
                            if (!log_rtl_wr_val_0.IsValidSignal()) {
                                rtl_signal_control = rtl_wr_val_0->bval;
                                rtl_data_control = rtl_wr_val_0->aval;
                                ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), rtl_signal_control, rtl_data_control, 0, 0);
                            }
                            return COMP_DPI_SV_ERR;
                        }

                        rtl_GR_val_lo_chk = static_cast<uint64_t> (log_rtl_wr_val_0.getIntVal()) | (static_cast<uint64_t> (log_rtl_wr_val_1.getIntVal()) << 32);
                        rtl_GR_val_up_chk = static_cast<uint64_t> (log_rtl_wr_val_2.getIntVal()) | (static_cast<uint64_t> (log_rtl_wr_val_3.getIntVal()) << 32);
                    } else {
                        // Register index is invalid
                        cmp_ret = cmprunner::CMP_RTL_ADD_REG_ERR;
                        CmpRegBufTrace (rtl_peid, rtl_time, rtl_cycle, cmp_ret, rtl_pc, rtl_reg_num, rtl_GR_val_up_chk, rtl_GR_val_lo_chk, rtl_nblk_id);
                        return COMP_DPI_SV_ERR;
                    }
                    cmprunner::APIErrorCode ret_add = cforestif->CmpAddRegInfo(rtl_peid, cmprunner::TYPE_RTL, NBLKID_NONE,
                                                                               rtl_GR_num_chk, rtl_GR_val_up_chk, rtl_GR_val_lo_chk);
                    if (ret_add == cmprunner::API_E_OK) {
                        cmp_ret = cmprunner::CMP_RTL_ADD_REG_OK;
                    } else if (ret_add == cmprunner::API_E_MULTI_ALLOC) {
                        cmp_ret = cmprunner::CMP_RTL_ADD_REG_DUP;
                    } else {
                        cmp_ret = cmprunner::CMP_RTL_ADD_REG_ERR;
                    }
                }
            }
            if (IsRegisterBufferError(rtl_peid)) {
                // Update error count.
                g_cur_error_count = g_cmp_error_count.at(rtl_peid) + 1;
                g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
                g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, g_cur_error_count);
            }
            CmpRegBufTrace (rtl_peid, rtl_time, rtl_cycle, cmp_ret, rtl_pc, rtl_GR_num_chk, rtl_GR_val_up_chk, rtl_GR_val_lo_chk, rtl_nblk_id);
        }

        if (rtl_last_commit && (g_cmp_eiitb_status.at(rtl_peid) == false)) {
            RegData64 cforest_val_up = 0x0;
            RegData64 cforest_val_lo = 0x0;
            bool iss_fl_hmmode = false, iss_fl_gmmode = false ;
            // ============================================== //
            // || Notify to CForest side for step execution ||
            // ============================================== //

            // Check C2B1 insn and execute 1st insn.
            if (rtl_c2b1_commit) {
                const std::string msg_c2b1 = "C2B1";
                DEBUG_LOG(rtl_peid, rtl_time, rtl_cycle, &msg_c2b1);

                //Execute 1st insn in c2b1
                cforestif->CmpStepExecute (rtl_peid);
                cforestif->CmpGetTrace (rtl_peid, &trace);

                // Keep PC of 1st insn
                iss_PC_c2b1_1st =  trace->GetInstAddr ();

                // Update GR number, value in 1st insn
                // Keep Writeback GR of 1st insn in case cmp/alu + bcond9
                for (int32_t idx_count = 0; idx_count < trace->GetWregCount (); idx_count++) {
                    // In C2B1, GR writeback of 1st insn is only 1 time
                    CReg cforest_regno = trace->GetWriteReg (idx_count) & 0xfff;
                    if ((cforest_regno > R0) && cforest_regno <= R31) {
                        // Get the unique GR writeback in 1st insn
                        iss_GR_no_c2b1 = cforest_regno;
                        iss_GR_val_c2b1 = trace->GetWregValue(idx_count);
                        iss_GR_Wen_c2b1_1st = cmprunner::ENABLE_VAL;
                        continue;
                    } else if (cforest_regno >= HMPSW_Z && cforest_regno <= HMPSW_SAT) {
                        iss_FL_Wen_c2b1_1st |= (1 << (cforest_regno - HMPSW_Z));
                        iss_FL_val_c2b1_1st |= (trace->GetWregValue(idx_count) << (cforest_regno - HMPSW_Z));
                        continue;
                    } else if (cforest_regno >= GMPSW_Z && cforest_regno <= GMPSW_SAT) {
                        // @@ TODO : iss_FL_Wen_c2b1_1st and iss_FL_val_c2b1_1st should be sepalated from PSW and GMPSW?
                        iss_FL_Wen_c2b1_1st |= (1 << (cforest_regno - GMPSW_Z));
                        iss_FL_val_c2b1_1st |= (trace->GetWregValue(idx_count) << (cforest_regno - GMPSW_Z));
                        continue;
                    }
                }
                CmpPrintTrace(rtl_peid, rtl_time, rtl_cycle, cmprunner::CMP_C2B1_COMMIT, rtl_pc, rtl_GR_num_chk, rtl_GR_val_up_chk, rtl_GR_val_lo_chk,
                              cforest_val_up, cforest_val_lo, rtl_FL_Wen_chk.at(rtl_peid), rtl_FL_val_chk.at(rtl_peid), iss_FL_Wen, iss_FL_val, rtl_nblk_id, trace, false);
            }
            // Execute normal insn or 2nd insn in C2B1.
            cforestif->CmpStepExecute (rtl_peid);
            cforestif->CmpGetTrace (rtl_peid, &trace);

            // Update C2B1 info in CForest: GR update, Flag enable checking
            if (trace->GetWregCount ()) {
                // Update GR number, value in 2nd insn
                for (int32_t i = 0; i < trace->GetWregCount (); i++) {
                    CReg cforest_regno = trace->GetWriteReg (i) & 0xfff;
                    if ((cforest_regno > R0 && cforest_regno <= R31) && rtl_c2b1_commit) {
                        // If GR writeback of 2nd is available, it will be the GR writeback of c2b1 insn.
                        iss_GR_no_c2b1 = cforest_regno;
                        iss_GR_val_c2b1 = trace->GetWregValue(i);
                        iss_GR_Wen_c2b1_2nd = cmprunner::ENABLE_VAL;
                        continue;
                    } else if ((cforest_regno >= HMPSW_Z) && (cforest_regno <= HMPSW_SAT)) {
                        iss_fl_hmmode = true ;
                        iss_FL_Wen_c2b1_2nd |= (1 << (cforest_regno - HMPSW_Z));
                        iss_FL_val_c2b1_2nd |= (trace->GetWregValue(i) << (cforest_regno - HMPSW_Z));
                        continue;
                    } else if ((cforest_regno >= GMPSW_Z) && (cforest_regno <= GMPSW_SAT)) {
                        // @@ TODO : iss_FL_Wen_c2b1_1st and iss_FL_val_c2b1_1st should be sepalated from PSW and GMPSW?
                        iss_fl_gmmode = true ;
                        iss_FL_Wen_c2b1_2nd |= (1 << (cforest_regno - GMPSW_Z));
                        iss_FL_val_c2b1_2nd |= (trace->GetWregValue(i) << (cforest_regno - GMPSW_Z));
                        continue;
                    }
                }
            }

            if (rtl_c2b1_commit && (iss_GR_Wen_c2b1_2nd != cmprunner::ENABLE_VAL) && (iss_GR_Wen_c2b1_1st == cmprunner::ENABLE_VAL)) {
                trace->RecordGReg<TraceCtrl::REG_WRITE>(iss_GR_no_c2b1, iss_GR_val_c2b1);
            }

            // Update Flag Wen in CForest
            if (iss_FL_Wen_c2b1_2nd) {
                iss_FL_Wen = iss_FL_Wen_c2b1_2nd;
                iss_FL_val = iss_FL_val_c2b1_2nd;
            } else if (iss_FL_Wen_c2b1_1st) {
                iss_FL_Wen = iss_FL_Wen_c2b1_1st;
                iss_FL_val = iss_FL_val_c2b1_1st;
            }

            // XDump update RTL info:
            // 1. FPU, FXU instruction.
            // 2. Data Load.
            cforestif->UpdateXDumpInfo(rtl_peid, trace);

            // ============================================== //
            // || Compare RTL and CForest values ||
            // ============================================== //
            cmp_ret = cmprunner::CMP_MATCH;

            // Compare value in ComprunnerG4MHv2::G4MHv2_CmpStep
            // Compare PC value
            if (trace->GetExpCode() != 0x0000) {

                CmpPrintSpace (&oss, 97);
                oss << "EXP[" << ExpInfo::GetName(trace->GetExpCode());
                oss << "] is ocurred";
                if (rtl_pc != trace->GetInstAddr ()) {
                    cmp_ret = cmprunner::CMP_PC_MISS;
                } else {
                    if (trace->GetWregCount ()) {
                        for (int32_t i = 0; i < trace->GetWregCount (); i++) {
                            // In case G4MH, this operation can be ignored due to no VM
                            CReg regno = trace->GetWriteReg (i) & 0xfff;
                            if ((regno > R0 && regno <= R31) || (regno >= WREG_BASE && regno <= WREG_MAX)) {
                                rtl_GR_num_chk = regno;
                                if (regno >= WREG_BASE && regno <= WREG_MAX) {
                                    cforest_val_up = (static_cast<uint64_t>(trace->GetWWregValue(i, 3)) << 32)
                                                   | (static_cast<uint64_t>(trace->GetWWregValue(i, 2)) << 0);
                                    cforest_val_lo = (static_cast<uint64_t>(trace->GetWWregValue(i, 1)) << 32)
                                                   | (static_cast<uint64_t>(trace->GetWWregValue(i, 0)) << 0);
                                } else { // if (regno > R0 && regno <= R31) {
                                    cforest_val_up = 0;
                                    cforest_val_lo = trace->GetWregValue(i);
                                }
                                cmprunner::CmpRegInfo RTL_info;
                                // Get Register info with {GR number, TYPE_EMP} or {GR number, TYPE_RTL}
                                cforestif->CmpGetRegInfo (rtl_peid, cmprunner::CtxCompByLast, rtl_nblk_id, rtl_GR_num_chk, cforest_val_up, cforest_val_lo,
                                                                                              RTL_info, cmp_ret);


                                // Add GR update from CForest to CompRunner queue.
                                if (RTL_info.GetCommitType() == cmprunner::TYPE_EMP) {
                                    cmprunner::APIErrorCode ret_upd = cforestif->CmpUpdRegInfo (rtl_peid, cmprunner::TYPE_ISS, RTL_info.GetNblkId(),
                                                                                                rtl_GR_num_chk, cforest_val_up, cforest_val_lo, cmp_ret);
                                    if (ret_upd == cmprunner::API_E_OK){
                                        cmp_ret = cmprunner::CMP_ISS_UPD_OK;
                                    } else {
                                        cmp_ret = cmprunner::CMP_ISS_UPD_ERR;
                                    }
                                } else if (RTL_info.GetCommitType() == cmprunner::TYPE_RTL) {
                                    // Get cmp_ret in Del function.
                                    cmprunner::APIErrorCode ret_del = cforestif->CmpDelRegInfo (rtl_peid, cmprunner::TYPE_RTL, NBLKID_NONE,
                                                                                                rtl_GR_num_chk, cforest_val_up, cforest_val_lo, rtl_xdump_ctrl, cmp_ret);
                                    if (ret_del != cmprunner::API_E_OK){
                                        cmp_ret = cmprunner::CMP_ISS_DEL_ERR;
                                    }
                                } else {
                                    // Can't find CForest GR in CompRunner Buffer
                                    cmp_ret = cmprunner::CMP_ISS_GET_ERR;
                                }
                                CmpPrintTrace(rtl_peid, rtl_time, rtl_cycle, cmp_ret, rtl_pc, rtl_GR_num_chk, rtl_GR_val_up_chk, rtl_GR_val_lo_chk,
                                              cforest_val_up, cforest_val_lo, rtl_FL_Wen_chk.at(rtl_peid), rtl_FL_val_chk.at(rtl_peid), iss_FL_Wen, iss_FL_val, rtl_nblk_id, trace, false);
                            }
                        }
                    }

                    if (cmp_ret == cmprunner::CMP_MATCH || cmp_ret == cmprunner::CMP_ISS_UPD_OK) {
                        cmp_ret = cmprunner::CMP_LAST_MATCH;
                    } else {
                        cmp_ret = cmprunner::CMP_LAST_MISMATCH;
                    }

                    // Check all TYPE_RTL and TYPE_EMPTY elements to guarantee they are not exist in CompRunner buffer.
                    if (cforestif-> CheckLastCommit(rtl_peid) == false) {
                        cmp_ret = cmprunner::CMP_LAST_MISMATCH;
                        cmprunner::CmpRegInfo RTL_info;
                        cforestif->CmpGetRegInfo (rtl_peid, cmprunner::CtxCheckLast, rtl_nblk_id, R0, 0x0, 0x0, RTL_info, cmp_ret);
                        rtl_GR_num_chk = RTL_info.GetReg();
                    }
                }
                oss << std::endl;
            } else {
                if (rtl_c2b1_commit && (rtl_pc != iss_PC_c2b1_1st)) {
                    cmp_ret = cmprunner::CMP_PC_MISS;
                } else if ((!rtl_c2b1_commit) && (rtl_pc != trace->GetInstAddr ())) {
                    cmp_ret = cmprunner::CMP_PC_MISS;
                } else {

                    if (trace->GetWregCount ()) {
                        for (int32_t i = 0; i < trace->GetWregCount (); i++) {
                            // In case G4MH, this operation can be ignored due to no VM
                            CReg regno = trace->GetWriteReg (i) & 0xfff;
                            if ((regno > R0 && regno <= R31) || (regno >= WREG_BASE && regno <= WREG_MAX)) {
                                rtl_GR_num_chk = regno;
                                if (regno >= WREG_BASE && regno <= WREG_MAX) {
                                    cforest_val_up = (static_cast<uint64_t>(trace->GetWWregValue(i, 3)) << 32)
                                                   | (static_cast<uint64_t>(trace->GetWWregValue(i, 2)) << 0);
                                    cforest_val_lo = (static_cast<uint64_t>(trace->GetWWregValue(i, 1)) << 32)
                                                   | (static_cast<uint64_t>(trace->GetWWregValue(i, 0)) << 0);
                                } else { // if (regno > R0 && regno <= R31) {
                                    cforest_val_up = 0;
                                    cforest_val_lo = trace->GetWregValue(i);
                                }
                                cmprunner::CmpRegInfo RTL_info;
                                // Get Register info with {GR number, TYPE_EMP} or {GR number, TYPE_RTL}
                                cforestif->CmpGetRegInfo (rtl_peid, cmprunner::CtxCompByLast, NBLKID_NONE, rtl_GR_num_chk, cforest_val_up, cforest_val_lo,
                                                                                              RTL_info, cmp_ret);

                                // Add GR update from CForest to CompRunner queue.
                                if (RTL_info.GetCommitType() == cmprunner::TYPE_EMP) {
                                    cmprunner::APIErrorCode ret_upd = cforestif->CmpUpdRegInfo (rtl_peid, cmprunner::TYPE_ISS, RTL_info.GetNblkId(),
                                                                                                rtl_GR_num_chk, cforest_val_up, cforest_val_lo, cmp_ret);
                                    if (ret_upd == cmprunner::API_E_OK){
                                        cmp_ret = cmprunner::CMP_ISS_UPD_OK;
                                    } else {
                                        cmp_ret = cmprunner::CMP_ISS_UPD_ERR;
                                    }
                                } else if (RTL_info.GetCommitType() == cmprunner::TYPE_RTL) {
                                    // Get cmp_ret in Del function.
                                    cmprunner::APIErrorCode ret_del = cforestif->CmpDelRegInfo (rtl_peid, cmprunner::TYPE_RTL, NBLKID_NONE,
                                                                                                rtl_GR_num_chk, cforest_val_up, cforest_val_lo, rtl_xdump_ctrl, cmp_ret);
                                    if (ret_del != cmprunner::API_E_OK){
                                        cmp_ret = cmprunner::CMP_ISS_DEL_ERR;
                                    }
                                } else {
                                    // Can't find CForest GR in CompRunner Buffer
                                    cmp_ret = cmprunner::CMP_ISS_GET_ERR;
                                }
                                CmpPrintTrace(rtl_peid, rtl_time, rtl_cycle, cmp_ret, rtl_pc, rtl_GR_num_chk, rtl_GR_val_up_chk, rtl_GR_val_lo_chk,
                                              cforest_val_up, cforest_val_lo, rtl_FL_Wen_chk.at(rtl_peid), rtl_FL_val_chk.at(rtl_peid), iss_FL_Wen, iss_FL_val, rtl_nblk_id, trace, false);
                            }
                        }
                    }

                    if (cmp_ret == cmprunner::CMP_MATCH || cmp_ret == cmprunner::CMP_ISS_UPD_OK) {
                        cmp_ret = cmprunner::CMP_LAST_MATCH;
                    } else {
                        cmp_ret = cmprunner::CMP_LAST_MISMATCH;
                    }

                    // Check all TYPE_RTL and TYPE_EMPTY elements to guarantee they are not exist in CompRunner buffer.
                    if (cforestif-> CheckLastCommit(rtl_peid) == false) {
                        cmp_ret = cmprunner::CMP_LAST_MISMATCH;
                        cmprunner::CmpRegInfo RTL_info;
                        cforestif->CmpGetRegInfo (rtl_peid, cmprunner::CtxCheckLast, rtl_nblk_id, R0, 0x0, 0x0, RTL_info, cmp_ret);
                        rtl_GR_num_chk = RTL_info.GetReg();
                    } else {
                        if ((trace->GetInstId() == INST_ID_V_CTRET) || (trace->GetInstId() == INST_ID_V_EIRET)
                         || (trace->GetInstId() == INST_ID_V_FERET) || (trace->GetInstId() == INST_ID_V_DBRET)) {
                            // Don't do the comparison for Flag update
                        } else {
                            if (rtl_FL_Wen_chk.at(rtl_peid) == iss_FL_Wen) {
                                bool flagchk = cforestif->CmpCompFlag (rtl_peid, rtl_FL_Wen_chk.at(rtl_peid), rtl_FL_val_chk.at(rtl_peid), iss_FL_Wen, iss_FL_val, rtl_xdump_ctrl, iss_fl_hmmode, iss_fl_gmmode, trace);
                                if (!flagchk) {
                                    cmp_ret |= cmprunner::CMP_FLAG_MISS;
                                }
                            } else {
                                cmp_ret = cmprunner::CMP_FLAG_LACK;
                            }
                        }
                    }
                }
            }
            if (IsCompareSuccess(cmp_ret) == COMP_DPI_MISMATCH) {
                // Update error count.
                g_cur_error_count = g_cmp_error_count.at(rtl_peid) + 1;
                g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
                g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, g_cur_error_count);
            }

            CmpPrintTrace(rtl_peid, rtl_time, rtl_cycle, cmp_ret, rtl_pc, rtl_GR_num_chk, rtl_GR_val_up_chk, rtl_GR_val_lo_chk,
                          cforest_val_up, cforest_val_lo, rtl_FL_Wen_chk.at(rtl_peid), rtl_FL_val_chk.at(rtl_peid), iss_FL_Wen, iss_FL_val, rtl_nblk_id, trace, true);

            // Initialize value for Flag values.
            // Update flag enable.
            rtl_FL_Wen_chk.erase(rtl_FL_Wen_chk.begin() + rtl_peid);
            rtl_FL_Wen_chk.insert(rtl_FL_Wen_chk.begin() + rtl_peid, cmprunner::UNUSED_VAL);
            // Update flag value.
            rtl_FL_val_chk.erase(rtl_FL_val_chk.begin() + rtl_peid);
            rtl_FL_val_chk.insert(rtl_FL_val_chk.begin() + rtl_peid, cmprunner::UNUSED_VAL);

            // Update middle step value in case MCYC to init for next inst.
            g_cmp_mcyc_middle.erase(g_cmp_mcyc_middle.begin() + rtl_peid);
            g_cmp_mcyc_middle.insert(g_cmp_mcyc_middle.begin() + rtl_peid, 0);
        } else if (rtl_last_commit){
            if (g_cmp_eiitb_status.at(rtl_peid)) {
                // Update EIINT status.
                g_cmp_eiitb_status.erase(g_cmp_eiitb_status.begin() + rtl_peid);
                g_cmp_eiitb_status.insert(g_cmp_eiitb_status.begin() + rtl_peid, false);
            }
            // Update middle step value in case MCYC to init for next inst.
            g_cmp_mcyc_middle.erase(g_cmp_mcyc_middle.begin() + rtl_peid);
            g_cmp_mcyc_middle.insert(g_cmp_mcyc_middle.begin() + rtl_peid, 0);
        } else {
            if (IsCompareSuccess(cmp_ret) == COMP_DPI_MISMATCH) {
                // Update error count.
                g_cur_error_count = g_cmp_error_count.at(rtl_peid) + 1;
                g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
                g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, g_cur_error_count);
            }
        }

        fprintf (g_cmp_fp, "%s", oss.str().c_str());
        return IsCompareSuccess(cmp_ret);
    }
    else {
        return COMP_DPI_MATCH;
    }
}


    uint32_t ComprunnerG4MHv2::G4MHv2_CmpNbWb (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_nblk_id, svLogicVecVal* rtl_gr_val,
                          svLogicVecVal* rtl_wr_val_0, svLogicVecVal* rtl_wr_val_1, svLogicVecVal* rtl_wr_val_2, svLogicVecVal* rtl_wr_val_3)
{
    if (m_cmprunner_en) {
        const std::string msg = "CmpNbWb";
        DEBUG_LOG(rtl_peid, rtl_time, rtl_cycle, &msg);

#if DEBUG_NEED
        cforestif->DB_DPI_CmpNbWb(rtl_peid, rtl_time, rtl_cycle, rtl_nblk_id, rtl_gr_val, rtl_wr_val_0, rtl_wr_val_1, rtl_wr_val_2, rtl_wr_val_3);
#endif //DEBUG_NEED

        if (!cforestif->IsPeIdValid(rtl_peid)) {
            std::ostringstream reg_invalid;
            reg_invalid << "PEID = " << rtl_peid;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, 0x0, 7, 0);
            return COMP_DPI_SV_ERR;
        }

        cmprunner::CmpResult cmp_ret = cmprunner::CMP_MATCH;
        cmprunner::CmpRegInfo reg_info;
        cforestif->CmpGetRegInfo(rtl_peid, cmprunner::CtxCompByNBLK, rtl_nblk_id, R0, 0x0, 0x0, reg_info, cmp_ret);
        uint64_t rtl_GR_val_up_chk = 0x0;
        uint64_t rtl_GR_val_lo_chk = 0x0;

        if (cmp_ret != cmprunner::CMP_NB_MISS) {
            uint64_t rtl_GR_num_chk = 0x0;
            rtl_GR_num_chk = reg_info.GetReg();
            if (rtl_GR_num_chk > R0 && rtl_GR_num_chk <= R31) {
                // Validate RTL value
                cmprunner::RtlLogicVal log_rtl_gr_val (rtl_gr_val);
                if (!log_rtl_gr_val.IsValidSignal()) {
                    std::ostringstream reg_invalid;
                    reg_invalid << "R" << rtl_GR_num_chk;
                    uint32_t rtl_signal_control = rtl_gr_val->bval;
                    uint32_t rtl_data_control = rtl_gr_val->aval;
                    ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), rtl_signal_control, rtl_data_control, 0, 0);
                    return COMP_DPI_SV_ERR;
                }
                rtl_GR_val_lo_chk = log_rtl_gr_val.getIntVal() & 0x0FFFFFFFF;
            } else if (rtl_GR_num_chk >= WREG_BASE && rtl_GR_num_chk <= WREG_MAX) {
                // Validate RTL value
                cmprunner::RtlLogicVal log_rtl_wr_val_0 (rtl_wr_val_0);
                cmprunner::RtlLogicVal log_rtl_wr_val_1 (rtl_wr_val_1);
                cmprunner::RtlLogicVal log_rtl_wr_val_2 (rtl_wr_val_2);
                cmprunner::RtlLogicVal log_rtl_wr_val_3 (rtl_wr_val_3);
                if ((!log_rtl_wr_val_0.IsValidSignal()
                  || !log_rtl_wr_val_1.IsValidSignal()
                  || !log_rtl_wr_val_2.IsValidSignal()
                  || !log_rtl_wr_val_3.IsValidSignal())) {
                    std::ostringstream reg_invalid;
                    reg_invalid << "WR" << rtl_GR_num_chk;
                    uint32_t rtl_signal_control = 0;
                    uint32_t rtl_data_control = 0;
                    if (!log_rtl_wr_val_3.IsValidSignal()) {
                        rtl_signal_control = rtl_wr_val_3->bval;
                        rtl_data_control = rtl_wr_val_3->aval;
                        ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), rtl_signal_control, rtl_data_control, 0, 0);
                    }
                    if (!log_rtl_wr_val_2.IsValidSignal()) {
                        rtl_signal_control = rtl_wr_val_2->bval;
                        rtl_data_control = rtl_wr_val_2->aval;
                        ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), rtl_signal_control, rtl_data_control, 0, 0);
                    }
                    if (!log_rtl_wr_val_1.IsValidSignal()) {
                        rtl_signal_control = rtl_wr_val_1->bval;
                        rtl_data_control = rtl_wr_val_1->aval;
                        ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), rtl_signal_control, rtl_data_control, 0, 0);
                    }
                    if (!log_rtl_wr_val_0.IsValidSignal()) {
                        rtl_signal_control = rtl_wr_val_0->bval;
                        rtl_data_control = rtl_wr_val_0->aval;
                        ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), rtl_signal_control, rtl_data_control, 0, 0);
                    }
                    return COMP_DPI_SV_ERR;
                }

                rtl_GR_val_lo_chk = static_cast<uint64_t> (log_rtl_wr_val_0.getIntVal()) | (static_cast<uint64_t> (log_rtl_wr_val_1.getIntVal()) << 32);
                rtl_GR_val_up_chk = static_cast<uint64_t> (log_rtl_wr_val_2.getIntVal()) | (static_cast<uint64_t> (log_rtl_wr_val_3.getIntVal()) << 32);
            }

            if (reg_info.GetCommitType() == cmprunner::TYPE_EMP) {
                cmprunner::APIErrorCode ret_upd = cforestif->CmpUpdRegInfo (rtl_peid, cmprunner::TYPE_RTL, reg_info.GetNblkId(), reg_info.GetReg(),
                                                                            rtl_GR_val_up_chk, rtl_GR_val_lo_chk, cmp_ret);
                if (ret_upd == cmprunner::API_E_OK){
                    // Modify in case 128 bit.
                    cmp_ret = cmprunner::CMP_OMIT;
                } else {
                    cmp_ret = cmprunner::CMP_RTL_UPD_ERR;
                }
            } else if (reg_info.GetCommitType() == cmprunner::TYPE_ISS) {
                cforestif->CmpDelRegInfo (rtl_peid, cmprunner::TYPE_ISS, reg_info.GetNblkId(), reg_info.GetReg(),
                                                                            rtl_GR_val_up_chk, rtl_GR_val_lo_chk, 0x0, cmp_ret);
            }
        }

        if (IsCompareSuccess(cmp_ret) == COMP_DPI_MISMATCH) {
            uint32_t g_cur_error_count = 0;
            // Update error count.
            g_cur_error_count = g_cmp_error_count.at(rtl_peid) + 1;
            g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
            g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, g_cur_error_count);
        }

        if (reg_info.GetReg() != cmprunner::CmpR32) {
            CmpNbPrintTrace (rtl_peid, rtl_time, rtl_cycle, cmp_ret, reg_info.GetReg(), rtl_GR_val_up_chk, rtl_GR_val_lo_chk,
                             reg_info.GetRegValUp(), reg_info.GetRegValLo(), rtl_nblk_id); // CForest values
        }

        return IsCompareSuccess(cmp_ret);
    }
    else {
        return COMP_DPI_MATCH;
    }
}


cmprunner::CmpResult ComprunnerG4MHv2::CmpStepCancel (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_pc)
{
    if (m_cmprunner_en) {
        if (g_cmp_mcyc_middle.at(rtl_peid) == 0) {
            // event occured at top of instruction. No need to simulate intermidiate state of the inst.
            return cmprunner::CMP_LAST_MATCH;
        }

        RegData64 cforest_val_up = 0x0;
        RegData64 cforest_val_lo = 0x0;

        TraceCtrl* trace = NULL;

        // Execute canceled insn
        cforestif->CmpStepExecute (rtl_peid);
        cforestif->CmpGetTrace (rtl_peid, &trace);

        // XDump update RTL info:
        // 1. FPU, FXU instruction.
        // 2. Data Load.
        cforestif->UpdateXDumpInfo(rtl_peid, trace);

        // ============================================== //
        // || Compare RTL and CForest values ||
        // ============================================== //
        cmprunner::CmpResult cmp_ret = cmprunner::CMP_MATCH;
        uint64_t rtl_GR_num_chk = 0x0;
        std::ostringstream oss;

        // Compare value in ComprunnerG4MHv2::G4MHv2_CmpStep
        // Compare PC value
        if (trace->GetExpCode() != 0x0000) { // Don't come normally except unmatched case
            CmpPrintSpace (&oss, 97);
            oss << "EXP[" << ExpInfo::GetName(trace->GetExpCode());
            oss << "] is ocurred";
            if (rtl_pc != trace->GetInstAddr ()) {
                cmp_ret = cmprunner::CMP_PC_MISS;
            } else {
                if (trace->GetWregCount ()) {
                    for (int32_t i = 0; i < trace->GetWregCount (); i++) {
                        // In case G4MH, this operation can be ignored due to no VM
                        CReg regno = trace->GetWriteReg (i) & 0xfff;
                        if ((regno > R0 && regno <= R31) || (regno >= WREG_BASE && regno <= WREG_MAX)) {
                            rtl_GR_num_chk = regno;
                            if (regno >= WREG_BASE && regno <= WREG_MAX) {
                                cforest_val_up = (static_cast<uint64_t>(trace->GetWWregValue(i, 3)) << 32)
                                                | (static_cast<uint64_t>(trace->GetWWregValue(i, 2)) << 0);
                                cforest_val_lo = (static_cast<uint64_t>(trace->GetWWregValue(i, 1)) << 32)
                                                | (static_cast<uint64_t>(trace->GetWWregValue(i, 0)) << 0);
                            } else { // if (regno > R0 && regno <= R31) {
                                cforest_val_up = 0;
                                cforest_val_lo = trace->GetWregValue(i);
                            }
                            cmprunner::CmpRegInfo RTL_info;
                            // Get Register info with {GR number, TYPE_EMP} or {GR number, TYPE_RTL}
                            cforestif->CmpGetRegInfo (rtl_peid, cmprunner::CtxCompByLast, NBLKID_NONE, rtl_GR_num_chk, cforest_val_up, cforest_val_lo,
                                                                                            RTL_info, cmp_ret);

                            // Add GR update from CForest to CompRunner queue.
                            if (RTL_info.GetCommitType() == cmprunner::TYPE_EMP) {
                                cmprunner::APIErrorCode ret_upd = cforestif->CmpUpdRegInfo (rtl_peid, NBLKID_NONE, RTL_info.GetNblkId(),
                                                                                            rtl_GR_num_chk, cforest_val_up, cforest_val_lo, cmp_ret);
                                if (ret_upd == cmprunner::API_E_OK){
                                    cmp_ret = cmprunner::CMP_ISS_UPD_OK;
                                } else {
                                    cmp_ret = cmprunner::CMP_ISS_UPD_ERR;
                                }
                            } else if (RTL_info.GetCommitType() == cmprunner::TYPE_RTL) {
                                // Get cmp_ret in Del function.
                                cmprunner::APIErrorCode ret_del = cforestif->CmpDelRegInfo (rtl_peid, cmprunner::TYPE_RTL, NBLKID_NONE,
                                                                                            rtl_GR_num_chk, cforest_val_up, cforest_val_lo, 0, cmp_ret);
                                if (ret_del != cmprunner::API_E_OK){
                                    cmp_ret = cmprunner::CMP_ISS_DEL_ERR;
                                }
                            } else {
                                // Can't find CForest GR in CompRunner Buffer
                                cmp_ret = cmprunner::CMP_ISS_GET_ERR;
                            }
                            CmpPrintTrace(rtl_peid, rtl_time, rtl_cycle, cmp_ret, rtl_pc, rtl_GR_num_chk, cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL,
                                            cforest_val_up, cforest_val_lo, rtl_FL_Wen_chk.at(rtl_peid), rtl_FL_val_chk.at(rtl_peid), cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL, NBLKID_NONE, trace, false);
                        }
                    }
                }

                if (cmp_ret == cmprunner::CMP_MATCH || cmp_ret == cmprunner::CMP_ISS_UPD_OK) {
                    cmp_ret = cmprunner::CMP_LAST_MATCH;
                } else {
                    cmp_ret = cmprunner::CMP_LAST_MISMATCH;
                }

                // Check all TYPE_RTL and TYPE_EMPTY elements to guarantee they are not exist in CompRunner buffer.
                if (cforestif-> CheckLastCommit(rtl_peid) == false) {
                    cmp_ret = cmprunner::CMP_LAST_MISMATCH;
                    cmprunner::CmpRegInfo RTL_info;
                    cforestif->CmpGetRegInfo (rtl_peid, cmprunner::CtxCheckLast, NBLKID_NONE, R0, 0x0, 0x0, RTL_info, cmp_ret);
                    rtl_GR_num_chk = RTL_info.GetReg();
                }
            }
            oss << std::endl;
        } else {
            if (rtl_pc != trace->GetInstAddr ()) {
                cmp_ret = cmprunner::CMP_PC_MISS;
            } else {
                if (trace->GetWregCount ()) {
                    for (int32_t i = 0; i < trace->GetWregCount (); i++) {
                        // In case G4MH, this operation can be ignored due to no VM
                        CReg regno = trace->GetWriteReg (i) & 0xfff;
                        if ((regno > R0 && regno <= R31) || (regno >= WREG_BASE && regno <= WREG_MAX)) {
                            rtl_GR_num_chk = regno;
                            if (regno >= WREG_BASE && regno <= WREG_MAX) {
                                cforest_val_up = (static_cast<uint64_t>(trace->GetWWregValue(i, 3)) << 32)
                                                | (static_cast<uint64_t>(trace->GetWWregValue(i, 2)) << 0);
                                cforest_val_lo = (static_cast<uint64_t>(trace->GetWWregValue(i, 1)) << 32)
                                                | (static_cast<uint64_t>(trace->GetWWregValue(i, 0)) << 0);
                            } else { // if (regno > R0 && regno <= R31) {
                                cforest_val_up = 0;
                                cforest_val_lo = trace->GetWregValue(i);
                            }
                            cmprunner::CmpRegInfo RTL_info;
                            // Get Register info with {GR number, TYPE_EMP} or {GR number, TYPE_RTL}
                            cforestif->CmpGetRegInfo (rtl_peid, cmprunner::CtxCompByLast, NBLKID_NONE, rtl_GR_num_chk, cforest_val_up, cforest_val_lo,
                                                                                            RTL_info, cmp_ret);

                            // Add GR update from CForest to CompRunner queue.
                            if (RTL_info.GetCommitType() == cmprunner::TYPE_EMP) {
                                cmprunner::APIErrorCode ret_upd = cforestif->CmpUpdRegInfo (rtl_peid, cmprunner::TYPE_ISS, RTL_info.GetNblkId(),
                                                                                            rtl_GR_num_chk, cforest_val_up, cforest_val_lo, cmp_ret);
                                if (ret_upd == cmprunner::API_E_OK){
                                    cmp_ret = cmprunner::CMP_ISS_UPD_OK;
                                } else {
                                    cmp_ret = cmprunner::CMP_ISS_UPD_ERR;
                                }
                            } else if (RTL_info.GetCommitType() == cmprunner::TYPE_RTL) {
                                // Get cmp_ret in Del function.
                                cmprunner::APIErrorCode ret_del = cforestif->CmpDelRegInfo (rtl_peid, cmprunner::TYPE_RTL, NBLKID_NONE,
                                                                                            rtl_GR_num_chk, cforest_val_up, cforest_val_lo, 0, cmp_ret);
                                if (ret_del != cmprunner::API_E_OK){
                                    cmp_ret = cmprunner::CMP_ISS_DEL_ERR;
                                }
                            } else {
                                // Can't find CForest GR in CompRunner Buffer
                                cmp_ret = cmprunner::CMP_ISS_GET_ERR;
                            }
                            CmpPrintTrace(rtl_peid, rtl_time, rtl_cycle, cmp_ret, rtl_pc, rtl_GR_num_chk, cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL,
                                            cforest_val_up, cforest_val_lo, rtl_FL_Wen_chk.at(rtl_peid), rtl_FL_val_chk.at(rtl_peid), cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL, NBLKID_NONE, trace, false);
                        }
                    }
                }

                if (cmp_ret == cmprunner::CMP_MATCH || cmp_ret == cmprunner::CMP_ISS_UPD_OK) {
                    cmp_ret = cmprunner::CMP_LAST_MATCH;
                } else {
                    cmp_ret = cmprunner::CMP_LAST_MISMATCH;
                }

                // Check all TYPE_RTL and TYPE_EMPTY elements to guarantee they are not exist in CompRunner buffer.
                if (cforestif-> CheckLastCommit(rtl_peid) == false) {
                    cmp_ret = cmprunner::CMP_LAST_MISMATCH;
                    cmprunner::CmpRegInfo RTL_info;
                    cforestif->CmpGetRegInfo (rtl_peid, cmprunner::CtxCheckLast, NBLKID_NONE, R0, 0x0, 0x0, RTL_info, cmp_ret);
                    rtl_GR_num_chk = RTL_info.GetReg();
                }
            }
        }
        CmpPrintTrace(rtl_peid, rtl_time, rtl_cycle, cmp_ret, rtl_pc, rtl_GR_num_chk, cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL,
                        cforest_val_up, cforest_val_lo, rtl_FL_Wen_chk.at(rtl_peid), rtl_FL_val_chk.at(rtl_peid), cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL, NBLKID_NONE, trace, true);

        // Initialize value for Flag values.
        // Update flag enable.
        rtl_FL_Wen_chk.erase(rtl_FL_Wen_chk.begin() + rtl_peid);
        rtl_FL_Wen_chk.insert(rtl_FL_Wen_chk.begin() + rtl_peid, cmprunner::UNUSED_VAL);
        // Update flag value.
        rtl_FL_val_chk.erase(rtl_FL_val_chk.begin() + rtl_peid);
        rtl_FL_val_chk.insert(rtl_FL_val_chk.begin() + rtl_peid, cmprunner::UNUSED_VAL);
        // Update EIINT status.
        g_cmp_eiitb_status.erase(g_cmp_eiitb_status.begin() + rtl_peid);
        g_cmp_eiitb_status.insert(g_cmp_eiitb_status.begin() + rtl_peid, false);

        fprintf (g_cmp_fp, "%s", oss.str().c_str());

        if (cmp_ret == cmprunner::CMP_MATCH) {
            cmp_ret = cmprunner::CMP_LAST_MATCH;
        } else if (cmp_ret == cmprunner::CMP_PC_MISS) {
            cmp_ret = cmprunner::CMP_LAST_MISMATCH;
        }

        // Update middle step value in case MCYC to init cancel timing for next inst.
        g_cmp_mcyc_middle.erase(g_cmp_mcyc_middle.begin() + rtl_peid);
        g_cmp_mcyc_middle.insert(g_cmp_mcyc_middle.begin() + rtl_peid, 0);

        return cmp_ret;
    }
    else {
        return COMP_DPI_MATCH;
    }
}


uint32_t ComprunnerG4MHv2::G4MHv2_ExpSyserrNotify (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc,
                              uint32_t rtl_syserr_cause, uint32_t rtl_operand_syserr, uint32_t rtl_operand_syserr_mode, uint32_t rtl_operand_syserr_gpid, uint32_t rtl_bus_slave_xdmp)
{
    if (m_cmprunner_en) {
        const std::string msg = "ExpSyserrNotify";
        DEBUG_LOG(rtl_peid, rtl_time, rtl_cycle, &msg);
        cmprunner::CmpResult cmp_ret = cmprunner::CMP_MATCH;

        if (!cforestif->IsPeIdValid(rtl_peid)) {
            std::ostringstream reg_invalid;
            reg_invalid << "PEID = " << rtl_peid;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, 0x0, 7, 0);
            return COMP_DPI_SV_ERR;
        }
        if ((rtl_syserr_cause < 0x10) || (rtl_syserr_cause > 0x1F)) {
            std::ostringstream reg_invalid;
            reg_invalid << "SYSERR cause code = " << rtl_syserr_cause;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, rtl_syserr_cause, 0x1F, 0x10);
            return COMP_DPI_SV_ERR;
        }

        cmprunner::CmpResult cmp_ret_canceled = CmpStepCancel (rtl_peid, rtl_1st_cm_time, rtl_cycle, rtl_exp_pc);

        std::ostringstream oss;
        CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
        CmpPrintSpace (&oss, 65);

        if (rtl_operand_syserr) { // Operation for operand syserr
            oss << "[IDump : SYSERR] " ;
            oss <<"Cause Code = 0x" << std::hex << std::setw(4) << std::setfill('0') << std::right << rtl_syserr_cause << " ";
            bool syserr_mode;
            if (rtl_operand_syserr_mode) {
                syserr_mode = false ;
                oss << "- Guest ";
            } else {
                syserr_mode = true ;
                oss << "- Host " ;
            }
            oss <<"GPID = " << rtl_operand_syserr_gpid ;
            //Syserr which cause code is 0x1A, 0x13 is syserr cause by ECC or parity
            //This kind of syserr can only generate by force model in SV-side and C-side cannot know it
            if (cforestif->GetOperandSyserrStatus (rtl_peid, syserr_mode, rtl_operand_syserr_gpid) || (rtl_syserr_cause == 0x01A) || (rtl_syserr_cause == 0x013) || (rtl_bus_slave_xdmp && rtl_syserr_cause == 0x18) || (rtl_bus_slave_xdmp && rtl_syserr_cause == 0x19)) {
                cforestif->ReqPseudoOperandSyserr (rtl_peid, rtl_syserr_cause, syserr_mode, rtl_operand_syserr_gpid);
            } else {
                // Update error count.
                uint32_t g_cur_error_count = 0;
                g_cur_error_count = g_cmp_error_count.at(rtl_peid) + 1;
                g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
                g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, g_cur_error_count);

                oss << std::endl << std::setfill(' ') << std::setw(95) << std::left << " ";
                CmpPrintErrorCount(&oss, rtl_peid);
                oss << "Operand syserr request don't exist>";
                fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
                return COMP_DPI_MISMATCH;
            }
        } else { // Operation for fetch syserr
            oss << "[IDump : SYSERR] ";
            oss <<"Cause Code = 0x" << std::hex << std::setw(4) << std::setfill('0') << std::right << rtl_syserr_cause;
            cforestif->ReqPseudoSyserr (rtl_peid, rtl_syserr_cause);
        }

        CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
        g_cycle_history.PrintSpace ();
        fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
        oss.str("");
        oss.clear();
        // Get trace info in case Interruption type.
        TraceCtrl* trace = NULL;

        // Execute instruction to cause
        cforestif->CmpStepExecute (rtl_peid);
        cforestif->CmpGetTrace (rtl_peid, &trace);

        if (trace->GetExpCode() != 0x0000) {
            CmpPrintSpace (&oss, 97);
            oss << "EXP[" << ExpInfo::GetName(trace->GetExpCode());
            oss << "] is ocurred";
            if (rtl_exp_pc != trace->GetInstAddr ()) {
                cmp_ret = cmprunner::CMP_PC_MISS;
            } else {
                if (trace->GetWregCount ()) {
                    for (int32_t i = 0; i < trace->GetWregCount (); i++) {
                        // In case G4MH, this operation can be ignored due to no VM
                        CReg regno = trace->GetWriteReg (i) & 0xfff;
                        if ((regno > R0 && regno <= R31) || (regno >= WREG_BASE && regno <= WREG_MAX)) {
                            CReg rtl_GR_num_chk = regno;
                            RegData64 cforest_val_up;
                            RegData64 cforest_val_lo;
                            if (regno >= WREG_BASE && regno <= WREG_MAX) {
                                cforest_val_up = (static_cast<uint64_t>(trace->GetWWregValue(i, 3)) << 32)
                                               | (static_cast<uint64_t>(trace->GetWWregValue(i, 2)) << 0);
                                cforest_val_lo = (static_cast<uint64_t>(trace->GetWWregValue(i, 1)) << 32)
                                               | (static_cast<uint64_t>(trace->GetWWregValue(i, 0)) << 0);
                            } else { // if (regno > R0 && regno <= R31) {
                                cforest_val_up = 0;
                                cforest_val_lo = trace->GetWregValue(i);
                            }

                            cmprunner::CmpRegInfo RTL_info;
                            // Get Register info with {GR number, TYPE_EMP} or {GR number, TYPE_RTL}
                            cforestif->CmpGetRegInfo (rtl_peid, cmprunner::CtxCompByLast, NBLKID_NONE, rtl_GR_num_chk, cforest_val_up, cforest_val_lo,
                                                                                          RTL_info, cmp_ret);
                            RegData64 rtl_val_up = RTL_info.GetRegValUp();
                            RegData64 rtl_val_lo = RTL_info.GetRegValLo();

                            // Add GR update from CForest to CompRunner queue.
                            if (RTL_info.GetCommitType() == cmprunner::TYPE_EMP) {
                                cmprunner::APIErrorCode ret_upd = cforestif->CmpUpdRegInfo (rtl_peid, cmprunner::TYPE_ISS, RTL_info.GetNblkId(),
                                                                                            rtl_GR_num_chk, cforest_val_up, cforest_val_lo, cmp_ret);
                                if (ret_upd == cmprunner::API_E_OK){
                                    cmp_ret = cmprunner::CMP_ISS_UPD_OK;
                                } else {
                                    cmp_ret = cmprunner::CMP_ISS_UPD_ERR;
                                }
                            } else if (RTL_info.GetCommitType() == cmprunner::TYPE_RTL) {
                                // Get cmp_ret in Del function.
                                cmprunner::APIErrorCode ret_del = cforestif->CmpDelRegInfo (rtl_peid, cmprunner::TYPE_RTL, NBLKID_NONE,
                                                                                            rtl_GR_num_chk, cforest_val_up, cforest_val_lo, cmprunner::UNUSED_VAL, cmp_ret);
                                if (ret_del != cmprunner::API_E_OK){
                                    cmp_ret = cmprunner::CMP_ISS_DEL_ERR;
                                }
                            } else {
                                // Can't find CForest GR in CompRunner Buffer
                                cmp_ret = cmprunner::CMP_ISS_GET_ERR;
                            }
                            CmpPrintTrace(rtl_peid, rtl_time, rtl_cycle, cmp_ret, rtl_exp_pc, rtl_GR_num_chk, rtl_val_up, rtl_val_lo,
                                          cforest_val_up, cforest_val_lo, rtl_FL_Wen_chk.at(rtl_peid), rtl_FL_val_chk.at(rtl_peid), NBLKID_NONE, NBLKID_NONE, NBLKID_NONE, trace, false);
                        }
                    }
                }

                if (cmp_ret == cmprunner::CMP_MATCH) {
                    cmp_ret = cmprunner::CMP_LAST_MATCH;
                } else {
                    cmp_ret = cmprunner::CMP_LAST_MISMATCH;
                }

                // Check all TYPE_RTL and TYPE_EMPTY elements to guarantee they are not exist in CompRunner buffer.
                if (cforestif-> CheckLastCommit(rtl_peid) == false) {
                    cmp_ret = cmprunner::CMP_LAST_MISMATCH;
                    cmprunner::CmpRegInfo RTL_info;
                }
            }

            CmpPrintTrace(rtl_peid, rtl_time, rtl_cycle, cmp_ret, rtl_exp_pc, R0, cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL,
                          cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL,
                          cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL, trace, false);
        }

        // If canceled instruction is unmatched, count error.
        if (cmp_ret_canceled != cmprunner::CMP_LAST_MATCH) {
            cmp_ret = cmprunner::CMP_LAST_MISMATCH;
        }

        if (IsCompareSuccess(cmp_ret) == COMP_DPI_MISMATCH) {
            uint32_t g_cur_error_count = 0;
            // Update error count.
            g_cur_error_count = g_cmp_error_count.at(rtl_peid) + 1;
            g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
            g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, g_cur_error_count);
        }

        fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
        return IsCompareSuccess(cmp_ret);
    }
    else {
        return COMP_DPI_MATCH;
    }
}

// Fix #100084 - start
uint32_t ComprunnerG4MHv2::G4MHv2_HaltSnoozeReleaseDump (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_Halt_dump_en)
{
    if (m_cmprunner_en) {
        const std::string msg = "HaltSnoozeReleaseDump";
        DEBUG_LOG(rtl_peid, rtl_time, rtl_cycle, &msg);

        std::ostringstream oss;


        if (!cforestif->IsPeIdValid(rtl_peid)) {
            std::ostringstream reg_invalid;
            reg_invalid << "PEID = " << rtl_peid;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, 0x0, 7, 0);
            return COMP_DPI_SV_ERR;
        }
        CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
        CmpPrintSpace (&oss, 65);
        oss << "[IDump: Halt/Snooze Release]";
        cforestif->HaltSnoozeRel(rtl_peid);

        fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
        return COMP_DPI_OK;
    }
    else {
        return COMP_DPI_OK;
    }
}
// Fix #100084 - end

uint32_t ComprunnerG4MHv2::G4MHv2_ExpOperandSyserrPend (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc,
                              uint32_t rtl_operand_syserr_cause, uint32_t rtl_operand_syserr_mode, uint32_t rtl_operand_syserr_gpid, uint32_t rtl_commit, uint32_t rtl_bus_slave_xdmp)
{
    if (m_cmprunner_en) {
        const std::string msg = "ExpOperandSyserrPend";
        DEBUG_LOG(rtl_peid, rtl_time, rtl_cycle, &msg);

        if (!cforestif->IsPeIdValid(rtl_peid)) {
            std::ostringstream reg_invalid;
            reg_invalid << "PEID = " << rtl_peid;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, 0x0, 7, 0);
            return COMP_DPI_SV_ERR;
        }
        if ((rtl_operand_syserr_cause < 0x10) || (rtl_operand_syserr_cause > 0x1F)) {
            std::ostringstream reg_invalid;
            reg_invalid << "SYSERR cause code = " << rtl_operand_syserr_cause;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, rtl_operand_syserr_cause, 0x1F, 0x10);
            return COMP_DPI_SV_ERR;
        }

        // Fix #97492 - start
        if ( rtl_commit) {
        // Fix #97492 - end
            std::ostringstream oss;
            CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
            CmpPrintSpace (&oss, 65);

            oss << "[IDump : Pending SYSERR " ;
            oss <<"Cause Code = 0x" << std::hex << std::setw(4) << std::setfill('0') << std::right << rtl_operand_syserr_cause << " ";
            bool syserr_mode;
            if (rtl_operand_syserr_mode) {
                syserr_mode = false ;
                oss << "- Guest ";
            } else {
                syserr_mode = true ;
                oss << "- Host " ;
            }
            oss <<"GPID = " << rtl_operand_syserr_gpid ;
            //Syserr which cause code is 0x1A, 0x13 is syserr cause by ECC or parity
            //This kind of syserr can only generate by force model in SV-side and C-side cannot know it
            if (cforestif->GetOperandSyserrStatus (rtl_peid, syserr_mode, rtl_operand_syserr_gpid) || (rtl_operand_syserr_cause == 0x01A) || (rtl_operand_syserr_cause == 0x013) || (rtl_bus_slave_xdmp && rtl_operand_syserr_cause == 0x18) || (rtl_bus_slave_xdmp && rtl_operand_syserr_cause == 0x19)) {
                cforestif->ReqPseudoOperandSyserr (rtl_peid, rtl_operand_syserr_cause, syserr_mode, rtl_operand_syserr_gpid);
            } else {
                // Update error count.
                uint32_t g_cur_error_count = 0;
                g_cur_error_count = g_cmp_error_count.at(rtl_peid) + 1;
                g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
                g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, g_cur_error_count);

                oss << std::endl << std::setfill(' ') << std::setw(95) << std::left << " ";
                CmpPrintErrorCount(&oss, rtl_peid);
                oss << "Operand syserr request don't exist>";
                fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
                return COMP_DPI_MISMATCH;
            }

            fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
        }
        return COMP_DPI_OK;
    }
    else {
        return COMP_DPI_OK;
    }
}

uint32_t ComprunnerG4MHv2::G4MHv2_ExpFeIntc1ReqToCpu (uint32_t rtl_peid, uint32_t rtl_exp_mode_id, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc,
                       uint32_t rtl_fe_type, uint32_t rtl_feint_ch, uint32_t rtl_bge_val, uint32_t rtl_intc1_en, uint32_t rtl_commit)
{
    if (m_cmprunner_en) {
        std::string inttype;
        if (rtl_fe_type == cmprunner::CmpFENMI) {
            inttype = "FENMI";
        } else if (rtl_fe_type == cmprunner::CmpFEINT) {
            inttype = "FEINT";
        } else if (rtl_fe_type == cmprunner::CmpGMFEINT) {
            inttype = "GMFEINT";
        } else if (rtl_fe_type == cmprunner::CmpBGFEINT) {
            inttype = "BGFEINT";
        }
        const std::string msg = "ExpFeIntc1ReqToCpu - " + inttype;
        DEBUG_LOG(rtl_peid, rtl_time, rtl_cycle, &msg);

        if (!cforestif->IsPeIdValid(rtl_peid)) {
            std::ostringstream reg_invalid;
            reg_invalid << "PEID = " << rtl_peid;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, 0x0, 7, 0);
            return COMP_DPI_SV_ERR;
        }
        if ((rtl_fe_type == cmprunner::CmpFEINT) && (rtl_feint_ch > 15)) {
            std::ostringstream reg_invalid;
            reg_invalid << "FEINT channel = " << rtl_feint_ch;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, rtl_fe_type, 15, 0);
            return COMP_DPI_SV_ERR;
        }

        // Fix #97492 - start
        if ( rtl_commit) {
        // Fix #97492 - end
            std::ostringstream oss;
            if (rtl_fe_type == cmprunner::CmpFENMI) {
                oss << "[IDump : FENMI]" ;
            }
            else {
                oss << "[IDump : FEINT channel ";
                oss << std::dec << std::setfill ('0') << std::setw(2) << std::right << rtl_feint_ch << ", FIBG = 0x";
                oss << std::hex << std::setfill ('0') << std::setw(8) << std::right << rtl_bge_val << "]";
            }
            CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
            g_cycle_history.PrintSpace ();
            oss.clear();

            if (rtl_intc1_en) {
                //Identify FE type
                if (rtl_fe_type == cmprunner::CmpFENMI)
                    cforestif->ReqPseudoFenmi2Intc1 (rtl_peid);
                else
                    cforestif->ReqPseudoFeint2Intc1 (rtl_peid, rtl_feint_ch);
            } else {
                //Identify FE type
                if (rtl_fe_type == cmprunner::CmpFENMI)
                    cforestif->ReqPseudoFenmi (rtl_peid);
                else {
                    // Load FIBG and get BGE value from ISS
                    uint32_t fibg;
                    PhysAddr fibg_addr = INTC1_Base +  FIBG_Offset ;
                    cforestif->VMemReadDebug (rtl_peid, fibg_addr, SIZE_2BYTE, &fibg);
                    uint32_t iss_bge_val = fibg & FIBG_mask ;

                    // Compare BGE value between RTL and ISS
                    if (rtl_bge_val == iss_bge_val) {
                        // Identify FEINT type
                        bool is_gm = false, is_bgint = false ;
                        if (rtl_fe_type == cmprunner::CmpGMFEINT) {
                            is_gm = true ;
                        } else if (rtl_fe_type == cmprunner::CmpBGFEINT) {
                            is_gm = true ;
                            is_bgint = true ;
                        }

                        // Req Eiint to CPU
                        cforestif->ReqPseudoFeint (rtl_peid, rtl_feint_ch, is_gm, rtl_exp_mode_id, is_bgint);

                    } else {
                        uint32_t g_cur_error_count = 0;
                        // Update error count.
                        g_cur_error_count = g_cmp_error_count.at(rtl_peid) + 1;
                        g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
                        g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, g_cur_error_count);

                        // Inform BGPR mismatch to log file
                        oss << std::endl << std::setfill(' ') << std::setw(24) << std::left ;
                        CmpPrintErrorCount(&oss, rtl_peid);
                        oss << "BGE value is MISMATCH>" ;
                        CmpPrintPC(&oss, rtl_exp_pc);
                        CmpPrintSpace (&oss, 10);
                        oss << "BGE RTL value = 0x" << std::hex << std::setw(2) << std::setfill('0') << std::right << rtl_bge_val ;
                        CmpPrintSpace (&oss, 5);
                        oss << "BGE ISS value = 0x" << std::hex << std::setw(2) << std::setfill('0') << std::right << iss_bge_val ;
                        fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
                        return COMP_DPI_MISMATCH;
                    }
                }
            }

            fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
        }
        return COMP_DPI_MATCH;
    }
    else {
        return COMP_DPI_MATCH;
    }
};


uint32_t ComprunnerG4MHv2::G4MHv2_ExpFeCpuAckToIntc1 (uint32_t rtl_peid, uint32_t rtl_exp_mode_id, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc, uint32_t rtl_fe_type, uint32_t rtl_feint_ch, uint32_t rtl_bge_val, uint32_t rtl_intc1_en)
{
    if (m_cmprunner_en) {
        std::string inttype;
        if (rtl_fe_type == cmprunner::CmpFENMI) {
            inttype = "FENMI";
        } else if (rtl_fe_type == cmprunner::CmpFEINT) {
            inttype = "FEINT";
        } else if (rtl_fe_type == cmprunner::CmpGMFEINT) {
            inttype = "GMFEINT";
        } else if (rtl_fe_type == cmprunner::CmpBGFEINT) {
            inttype = "BGFEINT";
        }
        const std::string msg = "ExpFeCpuAckToIntc1 - " + inttype;
        DEBUG_LOG(rtl_peid, rtl_time, rtl_cycle, &msg);
        cmprunner::CmpResult cmp_ret = cmprunner::CMP_MATCH;

        if (!cforestif->IsPeIdValid(rtl_peid)) {
            std::ostringstream reg_invalid;
            reg_invalid << "PEID = " << rtl_peid;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, 0x0, 7, 0);
            return COMP_DPI_SV_ERR;
        }
        if ((rtl_fe_type == cmprunner::CmpFEINT) && (rtl_feint_ch > 15)) {
            std::ostringstream reg_invalid;
            reg_invalid << "FEINT channel = " << rtl_feint_ch;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, rtl_fe_type, 15, 0);
            return COMP_DPI_SV_ERR;
        }

        cmprunner::CmpResult cmp_ret_canceled = CmpStepCancel (rtl_peid, rtl_1st_cm_time, rtl_cycle, rtl_exp_pc);

        std::ostringstream oss;
        CmpPrintPC(&oss, rtl_exp_pc);
        CmpPrintSpace (&oss, 10);

        if (rtl_fe_type == cmprunner::CmpFENMI) {
            // Force CForest go to FENMI status
            // FNC, FNBD, FEDM address in G4MH changed to new values;
            uint32_t fnc;
            PhysAddr fnc_addr  = INTC1_Base + FNC_Offset;
            uint32_t fnbd;
            PhysAddr fnbd_addr = INTC1_Base + FNBD_Offset;
            // These info is used for debug
            cforestif->VMemReadDebug (rtl_peid, fnc_addr, SIZE_2BYTE, &fnc);
            cforestif->VMemReadDebug (rtl_peid, fnbd_addr, SIZE_4BYTE, &fnbd);

            oss << "[IDump : FENMI] ";
            oss <<"FNC = 0x" << std::hex << std::setw(4) << std::setfill('0') << std::right << fnc;
            oss <<" FNBC = 0x" << std::hex << std::setw(8) << std::setfill('0') << std::right << fnbd;
            if (rtl_intc1_en)
                // Request FENMI to INCT1
                cforestif->ReqPseudoFenmi2Intc1 (rtl_peid);
            else
                // Request FENMI to CPU
                cforestif->ReqPseudoFenmi (rtl_peid);
        } else {
            // Force CForest go to FEINT status
            // FIC, FIBD, FEDM address in G4MH changed to new values;
            uint32_t fic_offset = 0x00C0U + 0x02 * rtl_feint_ch;
            uint32_t fibd_offset = 0x01C0U + 0x04 * rtl_feint_ch;
            uint32_t fic;
            PhysAddr fic_addr = INTC1_Base + fic_offset;
            uint32_t fibd;
            PhysAddr fibd_addr = INTC1_Base + fibd_offset;
            // These info is used for debug
            cforestif->VMemReadDebug (rtl_peid, fic_addr, SIZE_2BYTE, &fic);
            cforestif->VMemReadDebug (rtl_peid, fibd_addr, SIZE_4BYTE, &fibd);

            oss << "[IDump : FEINT channel ";
            oss << std::dec << std::setfill ('0') << std::setw(2) << std::right << rtl_feint_ch << "]";
            oss <<"FIC = 0x" << std::hex << std::setw(4) << std::setfill('0') << std::right << fic;
            oss <<" FIBD = 0x" << std::hex << std::setw(8) << std::setfill('0') << std::right << fibd;
            if (rtl_intc1_en)
                // Request FENMI to INCT1
                cforestif->ReqPseudoFeint2Intc1 (rtl_peid, rtl_feint_ch);
            else {
                // Load FIBG and get BGE value from ISS
                uint32_t fibg;
                uint32_t fibg_addr = INTC1_Base +  FIBG_Offset ;
                cforestif->VMemReadDebug (rtl_peid, fibg_addr, SIZE_2BYTE, &fibg);
                uint32_t iss_bge_val = fibg & 0x0FFFFU ;

                // Compare BGE value between RTL and ISS
                if (rtl_bge_val == iss_bge_val) {
                    // Identify FEINT type
                    bool is_gm = false, is_bgint = false ;
                    if (rtl_fe_type == cmprunner::CmpGMFEINT) {
                        is_gm = true ;
                    } else if (rtl_fe_type == cmprunner::CmpBGFEINT) {
                        is_gm = true ;
                        is_bgint = true ;
                    }

                    // Req Eiint to CPU
                    cforestif->ReqPseudoFeint (rtl_peid, rtl_feint_ch, is_gm, rtl_exp_mode_id, is_bgint);

                } else {
                    uint32_t g_cur_error_count = 0;
                    // Update error count.
                    g_cur_error_count = g_cmp_error_count.at(rtl_peid) + 1;
                    g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
                    g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, g_cur_error_count);

                    // Inform BGPR mismatch to log file
                    oss << std::endl << std::setfill(' ') << std::setw(24) << std::left ;
                    CmpPrintErrorCount(&oss, rtl_peid);
                    oss << "BGE value is MISMATCH>" ;
                    CmpPrintPC(&oss, rtl_exp_pc);
                    CmpPrintSpace (&oss, 10);
                    oss << "BGE RTL value = 0x" << std::hex << std::setw(2) << std::setfill('0') << std::right << rtl_bge_val ;
                    CmpPrintSpace (&oss, 5);
                    oss << "BGE ISS value = 0x" << std::hex << std::setw(2) << std::setfill('0') << std::right << iss_bge_val ;
                    fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
                    return COMP_DPI_MISMATCH;
                }
            }
        }
        // Get DBMK values.
        uint32_t dbmk;
        PhysAddr dbmk_addr = INTC1_Base + DBMK_Offset;
        // These info is used for debug

        cforestif->VMemReadDebug (rtl_peid, dbmk_addr, SIZE_2BYTE, &dbmk);
        oss <<" DBMK = 0x" << std::hex << std::setw(8) << std::setfill('0') << std::right << dbmk << std::endl;

        CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
        g_cycle_history.PrintSpace ();
        oss.str("");
        oss.clear();
        // Get trace info in case Interruption type.
        TraceCtrl* trace = NULL;

        // Execute instruction to cause
        cforestif->CmpStepExecute (rtl_peid);
        cforestif->CmpGetTrace (rtl_peid, &trace);

        if (trace->GetExpCode() != 0x0000) {

            CmpPrintSpace (&oss, 97);
            oss << "EXP[" << ExpInfo::GetName(trace->GetExpCode());
            oss << "] is ocurred";
            if (rtl_exp_pc != trace->GetInstAddr ()) {
                cmp_ret = cmprunner::CMP_PC_MISS;
            } else {
                if (trace->GetWregCount ()) {
                    for (int32_t i = 0; i < trace->GetWregCount (); i++) {
                        // In case G4MH, this operation can be ignored due to no VM
                        CReg regno = trace->GetWriteReg (i) & 0xfff;
                        if ((regno > R0 && regno <= R31) || (regno >= WREG_BASE && regno <= WREG_MAX)) {
                            CReg rtl_GR_num_chk = regno;
                            RegData64 cforest_val_up;
                            RegData64 cforest_val_lo;
                            if (regno >= WREG_BASE && regno <= WREG_MAX) {
                                cforest_val_up = (static_cast<uint64_t>(trace->GetWWregValue(i, 3)) << 32)
                                               | (static_cast<uint64_t>(trace->GetWWregValue(i, 2)) << 0);
                                cforest_val_lo = (static_cast<uint64_t>(trace->GetWWregValue(i, 1)) << 32)
                                               | (static_cast<uint64_t>(trace->GetWWregValue(i, 0)) << 0);
                            } else { // if (regno > R0 && regno <= R31) {
                                cforest_val_up = 0;
                                cforest_val_lo = trace->GetWregValue(i);
                            }

                            cmprunner::CmpRegInfo RTL_info;
                            // Get Register info with {GR number, TYPE_EMP} or {GR number, TYPE_RTL}
                            cforestif->CmpGetRegInfo (rtl_peid, cmprunner::CtxCompByLast, NBLKID_NONE, rtl_GR_num_chk, cforest_val_up, cforest_val_lo,
                                                                                          RTL_info, cmp_ret);
                            RegData64 rtl_val_up = RTL_info.GetRegValUp();
                            RegData64 rtl_val_lo = RTL_info.GetRegValLo();

                            // Add GR update from CForest to CompRunner queue.
                            if (RTL_info.GetCommitType() == cmprunner::TYPE_EMP) {
                                cmprunner::APIErrorCode ret_upd = cforestif->CmpUpdRegInfo (rtl_peid, cmprunner::TYPE_ISS, RTL_info.GetNblkId(),
                                                                                            rtl_GR_num_chk, cforest_val_up, cforest_val_lo, cmp_ret);
                                if (ret_upd == cmprunner::API_E_OK){
                                    cmp_ret = cmprunner::CMP_ISS_UPD_OK;
                                } else {
                                    cmp_ret = cmprunner::CMP_ISS_UPD_ERR;
                                }
                            } else if (RTL_info.GetCommitType() == cmprunner::TYPE_RTL) {
                                // Get cmp_ret in Del function.
                                cmprunner::APIErrorCode ret_del = cforestif->CmpDelRegInfo (rtl_peid, cmprunner::TYPE_RTL, NBLKID_NONE,
                                                                                            rtl_GR_num_chk, cforest_val_up, cforest_val_lo, cmprunner::UNUSED_VAL, cmp_ret);
                                if (ret_del != cmprunner::API_E_OK){
                                    cmp_ret = cmprunner::CMP_ISS_DEL_ERR;
                                }
                            } else {
                                // Can't find CForest GR in CompRunner Buffer
                                cmp_ret = cmprunner::CMP_ISS_GET_ERR;
                            }
                            CmpPrintTrace(rtl_peid, rtl_time, rtl_cycle, cmp_ret, rtl_exp_pc, rtl_GR_num_chk, rtl_val_up, rtl_val_lo,
                                          cforest_val_up, cforest_val_lo, rtl_FL_Wen_chk.at(rtl_peid), rtl_FL_val_chk.at(rtl_peid), NBLKID_NONE, NBLKID_NONE, NBLKID_NONE, trace, false);
                        }
                    }
                }

                if (cmp_ret == cmprunner::CMP_MATCH) {
                    cmp_ret = cmprunner::CMP_LAST_MATCH;
                } else {
                    cmp_ret = cmprunner::CMP_LAST_MISMATCH;
                }

                // Check all TYPE_RTL and TYPE_EMPTY elements to guarantee they are not exist in CompRunner buffer.
                if (cforestif-> CheckLastCommit(rtl_peid) == false) {
                    cmp_ret = cmprunner::CMP_LAST_MISMATCH;
                    cmprunner::CmpRegInfo RTL_info;
                }
            }

            CmpPrintTrace(rtl_peid, rtl_time, rtl_cycle, cmp_ret, rtl_exp_pc, R0, cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL,
                          cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL,
                          cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL, trace, false);
        }

        // If canceled instruction is unmatched, count error.
        if (cmp_ret_canceled != cmprunner::CMP_LAST_MATCH) {
            cmp_ret = cmprunner::CMP_LAST_MISMATCH;
        }

        if (IsCompareSuccess(cmp_ret) == COMP_DPI_MISMATCH) {
            uint32_t g_cur_error_count = 0;
            // Update error count.
            g_cur_error_count = g_cmp_error_count.at(rtl_peid) + 1;
            g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
            g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, g_cur_error_count);
        }

        fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
        return IsCompareSuccess(cmp_ret);
    }
    else {
        return COMP_DPI_MATCH;
    }
}


uint32_t ComprunnerG4MHv2::G4MHv2_ExpEiIntc1ReqToCpu (uint32_t rtl_peid, uint32_t rtl_exp_mode_id, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc, uint32_t rtl_ei_type,
                          uint32_t rtl_int_ch, uint32_t rtl_int_pri, uint32_t rtl_bgpr_val, uint32_t rtl_eitb_read, uint32_t rtl_intc1_en, uint32_t rtl_commit)
{
    if (m_cmprunner_en) {
        std::string inttype;
        if (rtl_ei_type == cmprunner::CmpEIINT) {
            inttype = "EIINT";
        } else if (rtl_ei_type == cmprunner::CmpGMEIINT) {
            inttype = "GMEIINT";
        } else if (rtl_ei_type == cmprunner::CmpBGEIINT) {
            inttype = "BGEIINT";
        }
        // Inform checking label to log file
        const std::string msg = "ExpEiIntc1ReqToCpu - " + inttype;
        DEBUG_LOG(rtl_peid, rtl_time, rtl_cycle, &msg);

        // Check valid PEID
        if (!cforestif->IsPeIdValid(rtl_peid)) {
            std::ostringstream reg_invalid;
            reg_invalid << "PEID = " << rtl_peid;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, 0x0, 7, 0);
            return COMP_DPI_SV_ERR;
        }

        // Check valid EIINT channel
        if (rtl_int_ch > INTC_MAX_CHANNEL) {
            std::ostringstream ch_invalid;
            ch_invalid << "EIINT REQ channel = " << rtl_int_ch ;
            ParseRTLSignal (rtl_peid, rtl_time, rtl_cycle, ch_invalid.str(), 0, rtl_int_ch, INTC_MAX_CHANNEL, 0);
            return COMP_DPI_SV_ERR ;
        }
        // Fix #97492 - start
        if ( rtl_commit) {
        // Fix #97492 - end
            std::ostringstream oss;
            oss << "[IDump : EIINT channel ";
            oss << std::dec << std::setfill ('0') << std::setw(2) << std::right << rtl_int_ch << ", Pri ";
            oss << std::dec << std::setfill ('0') << std::setw(3) << std::right << rtl_int_pri << ", EITB = ";
            oss << std::dec << std::setfill ('0') << std::setw(1) << std::right << rtl_eitb_read << ", EIBG = 0x";
            oss << std::hex << std::setfill ('0') << std::setw(8) << std::right << rtl_bgpr_val << "] ";
            CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
            g_cycle_history.PrintSpace ();
            oss.clear();

            if (rtl_intc1_en)
                // Req Eiint to INTC1
                cforestif->ReqPseudoEiint2Intc1 (rtl_peid, rtl_int_ch);
            else {
                // Load EIBG and get BGPR value from ISS
                uint32_t eibg;
                PhysAddr eibg_addr = INTC1_Base +  EIBG_Offset ;
                cforestif->VMemReadDebug (rtl_peid, eibg_addr, SIZE_2BYTE, &eibg);
                uint32_t iss_bgpr_val = eibg & EIBG_mask ;

                // Compare BGPR value between RTL and ISS
                if (rtl_bgpr_val == iss_bgpr_val) {
                    // Identify EIINT type
                    bool is_gm = false, is_bgint = false ;
                    if (rtl_ei_type == cmprunner::CmpGMEIINT) {
                        is_gm = true ;
                    } else if (rtl_ei_type == cmprunner::CmpBGEIINT) {
                        is_gm = true ;
                        is_bgint = true ;
                    }

                    // Req Eiint to CPU
                    cforestif->ReqPseudoEiint (rtl_peid, rtl_int_ch, rtl_int_pri, rtl_eitb_read,
                                               is_gm, rtl_exp_mode_id, is_bgint);

                } else {
                    uint32_t g_cur_error_count = 0;
                    // Update error count.
                    g_cur_error_count = g_cmp_error_count.at(rtl_peid) + 1;
                    g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
                    g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, g_cur_error_count);

                    // Inform BGPR mismatch to log file
                    oss << std::endl << std::setfill(' ') << std::setw(24) << std::left ;
                    CmpPrintErrorCount(&oss, rtl_peid);
                    oss << "BGPR value is MISMATCH>" ;
                    CmpPrintPC(&oss, rtl_exp_pc);
                    CmpPrintSpace (&oss, 10);
                    oss << "BGPR RTL value = 0x" << std::hex << std::setw(2) << std::setfill('0') << std::right << rtl_bgpr_val ;
                    CmpPrintSpace (&oss, 5);
                    oss << "BGPR ISS value = 0x" << std::hex << std::setw(2) << std::setfill('0') << std::right << iss_bgpr_val ;
                    fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
                    return COMP_DPI_MISMATCH ;
                }

            }

            fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
        }
        return COMP_DPI_MATCH;
    }
    else {
        return COMP_DPI_MATCH;
    }
}


uint32_t ComprunnerG4MHv2::G4MHv2_ExpEiCpuAckToIntc1 (uint32_t rtl_peid, uint32_t rtl_exp_mode_id, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc, uint32_t rtl_ei_type,
                                                     svLogicVecVal* rtl_int_ch, svLogicVecVal* rtl_int_pri, uint32_t rtl_bgpr_val, uint32_t rtl_eitb_read, uint32_t rtl_intc1_en)
{
    if (m_cmprunner_en) {
        std::string inttype;
        if (rtl_ei_type == cmprunner::CmpEIINT) {
            inttype = "EIINT";
        } else if (rtl_ei_type == cmprunner::CmpGMEIINT) {
            inttype = "GMEIINT";
        } else if (rtl_ei_type == cmprunner::CmpBGEIINT) {
            inttype = "BGEIINT";
        }
        const std::string msg = "ExpEiCpuAckToIntc1 - " + inttype;
        DEBUG_LOG(rtl_peid, rtl_time, rtl_cycle, &msg);
        cmprunner::CmpResult cmp_ret = cmprunner::CMP_MATCH;

        if (!cforestif->IsPeIdValid(rtl_peid)) {
            std::ostringstream reg_invalid;
            reg_invalid << "PEID = " << rtl_peid;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, 0x0, 7, 0);
            return COMP_DPI_SV_ERR;
        }

        cmprunner::CmpResult cmp_ret_canceled = CmpStepCancel (rtl_peid, rtl_1st_cm_time, rtl_cycle, rtl_exp_pc);

        cmprunner::RtlLogicVal log_rtl_int_ch (rtl_int_ch);
        cmprunner::RtlLogicVal log_rtl_int_pri (rtl_int_pri);
        if (!log_rtl_int_ch.IsValidSignal() || (!log_rtl_int_pri.IsValidSignal())
        || (log_rtl_int_ch.getIntVal() > INTC_MAX_CHANNEL)) {
            std::ostringstream reg_invalid;
            if (!log_rtl_int_pri.IsValidSignal()) {
                uint32_t rtl_signal_control = rtl_int_pri->bval;
                uint32_t rtl_data_control = rtl_int_pri->aval;
                reg_invalid << "EIINT priority = ";
                ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), rtl_signal_control, rtl_data_control, 0, 0);
            } else if (!log_rtl_int_ch.IsValidSignal()) {
                uint32_t rtl_signal_control = rtl_int_ch->bval;
                uint32_t rtl_data_control = rtl_int_ch->aval;
                reg_invalid << "EIINT channel = ";
                ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), rtl_signal_control, rtl_data_control, INTC_MAX_CHANNEL, 0);
            } else {
                uint32_t rtl_signal_control = rtl_int_ch->bval;
                uint32_t rtl_data_control = rtl_int_ch->aval;
                reg_invalid << "EIINT channel = " << log_rtl_int_ch.getIntVal();
                ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), rtl_signal_control, rtl_data_control, INTC_MAX_CHANNEL, 0);
            }
            return COMP_DPI_SV_ERR;
        }

        std::ostringstream oss;

        CmpPrintPC(&oss, rtl_exp_pc);
        CmpPrintSpace (&oss, 10);

        // Force CForest go to FEINT status
        // EIC, EIBD, EIDM address in G4MH changed to new values;
        uint32_t eic, eibd;
        PhysAddr eic_addr, eibd_addr;
        if (log_rtl_int_ch.getIntVal() <= 31) {
            uint32_t intc1_eic_offset  = 0x0000U + 0x02 * log_rtl_int_ch.getIntVal();
            uint32_t intc1_eibd_offset = 0x0100 + 0x04* log_rtl_int_ch.getIntVal();

            eic_addr = INTC1_Base + intc1_eic_offset;
            eibd_addr = INTC1_Base + intc1_eibd_offset;
        } else {
            uint32_t intc2_eic_offset  = 0x0000U + 0x02 * log_rtl_int_ch.getIntVal();
            uint32_t intc2_eibd_offset = 0x2000 + 0x04* log_rtl_int_ch.getIntVal();

            eic_addr = INTC2_Base + intc2_eic_offset;
            eibd_addr = INTC2_Base + intc2_eibd_offset;
        }
        cforestif->VMemReadDebug (rtl_peid, eic_addr,  SIZE_2BYTE, &eic);
        cforestif->VMemReadDebug (rtl_peid, eibd_addr, SIZE_4BYTE, &eibd);
        // Get DBMK values.
        uint32_t dbmk;
        PhysAddr dbmk_addr = INTC1_Base + DBMK_Offset;
        // These info is used for debug

        cforestif->VMemReadDebug (rtl_peid, dbmk_addr, SIZE_2BYTE, &dbmk);

        oss << "[IDump : EIINT channel ";
        oss << std::dec << std::setfill ('0') << std::setw(2) << std::right << log_rtl_int_ch.getIntVal() << ", Pri ";
        oss << std::dec << std::setfill ('0') << std::setw(3) << std::right << log_rtl_int_pri.getIntVal() << ", EITB = ";
        oss << std::dec << std::setfill ('0') << std::setw(1) << std::right << rtl_eitb_read << "] ";
        oss <<"EIC = 0x" << std::hex << std::setw(4) << std::setfill('0') << std::right << eic;
        oss <<" EIBD = 0x" << std::hex << std::setw(8) << std::setfill('0') << std::right << eibd;
        oss <<" DBMK  = 0x" << std::hex << std::setw(4) << std::setfill('0') << std::right << dbmk;
        CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
        g_cycle_history.PrintSpace ();
        oss.str("");
        oss.clear();

        if (rtl_intc1_en)
            // Req Eiint to INTC1
            cforestif->ReqPseudoEiint2Intc1 (rtl_peid, log_rtl_int_ch.getIntVal());
        else {
            // Load EIBG and get BGPR value from ISS
            uint32_t eibg;
            PhysAddr eibg_addr = INTC1_Base +  EIBG_Offset ;
            cforestif->VMemReadDebug (rtl_peid, eibg_addr, SIZE_2BYTE, &eibg);
            uint32_t iss_bgpr_val = eibg & 0x0FFU ;

            // Compare BGPR value between RTL and ISS
            if (rtl_bgpr_val == iss_bgpr_val) {
                // Identify EIINT type
                bool is_gm = false, is_bgint = false ;
                if (rtl_ei_type == cmprunner::CmpGMEIINT) {
                    is_gm = true ;
                } else if (rtl_ei_type == cmprunner::CmpBGEIINT) {
                    is_gm = true ;
                    is_bgint = true ;
                }

                // Req Eiint to CPU
                cforestif->ReqPseudoEiint (rtl_peid, log_rtl_int_ch.getIntVal(), log_rtl_int_pri.getIntVal(), rtl_eitb_read,
                                           is_gm, rtl_exp_mode_id, is_bgint);

            } else {
                uint32_t g_cur_error_count = 0;
                // Update error count.
                g_cur_error_count = g_cmp_error_count.at(rtl_peid) + 1;
                g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
                g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, g_cur_error_count);

                // Inform BGPR mismatch to log file
                oss << std::endl << std::setfill(' ') << std::setw(24) << std::left ;
                CmpPrintErrorCount(&oss, rtl_peid);
                oss << "BGPR value is MISMATCH>" ;
                CmpPrintPC(&oss, rtl_exp_pc);
                CmpPrintSpace (&oss, 10);
                oss << "BGPR RTL value = 0x" << std::hex << std::setw(2) << std::setfill('0') << std::right << rtl_bgpr_val ;
                CmpPrintSpace (&oss, 5);
                oss << "BGPR ISS value = 0x" << std::hex << std::setw(2) << std::setfill('0') << std::right << iss_bgpr_val ;
                fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
                return COMP_DPI_MISMATCH;
            }

        }

        // Get trace info in case Interruption type.
        TraceCtrl* trace = NULL;

        // Execute instruction to cause
        cforestif->CmpStepExecute (rtl_peid);
        cforestif->CmpGetTrace (rtl_peid, &trace);
        // Prevent CForest hold EIINT request.
        cforestif->CancelPseudoEIINT (rtl_peid, log_rtl_int_ch.getIntVal());

        if (rtl_eitb_read && (trace->GetExpCode() != EXP_SYSERR_RB)) {
            // Update EIINT status.
            g_cmp_eiitb_status.erase(g_cmp_eiitb_status.begin() + rtl_peid);
            g_cmp_eiitb_status.insert(g_cmp_eiitb_status.begin() + rtl_peid, true);
        }

        cforestif->UpdateXDumpInfo(rtl_peid, trace);

        if (trace->GetExpCode() != 0x0000) {
            CmpPrintSpace (&oss, 97);
            oss << "EXP[" << ExpInfo::GetName(trace->GetExpCode());
            oss << "] is ocurred";
            if (rtl_exp_pc != trace->GetInstAddr ()) {
                cmp_ret = cmprunner::CMP_PC_MISS;
            } else {
                if (trace->GetWregCount ()) {
                    for (int32_t i = 0; i < trace->GetWregCount (); i++) {
                        // In case G4MH, this operation can be ignored due to no VM
                        CReg regno = trace->GetWriteReg (i) & 0xfff;
                        if ((regno > R0 && regno <= R31) || (regno >= WREG_BASE && regno <= WREG_MAX)) {
                            CReg rtl_GR_num_chk = regno;
                            RegData64 cforest_val_up;
                            RegData64 cforest_val_lo;
                            if (regno >= WREG_BASE && regno <= WREG_MAX) {
                                cforest_val_up = (static_cast<uint64_t>(trace->GetWWregValue(i, 3)) << 32)
                                               | (static_cast<uint64_t>(trace->GetWWregValue(i, 2)) << 0);
                                cforest_val_lo = (static_cast<uint64_t>(trace->GetWWregValue(i, 1)) << 32)
                                               | (static_cast<uint64_t>(trace->GetWWregValue(i, 0)) << 0);
                            } else { // if (regno > R0 && regno <= R31) {
                                cforest_val_up = 0;
                                cforest_val_lo = trace->GetWregValue(i);
                            }
                            cmprunner::CmpRegInfo RTL_info;
                            // Get Register info with {GR number, TYPE_EMP} or {GR number, TYPE_RTL}
                            cforestif->CmpGetRegInfo (rtl_peid, cmprunner::CtxCompByLast, NBLKID_NONE, rtl_GR_num_chk, cforest_val_up, cforest_val_lo,
                                                                                          RTL_info, cmp_ret);
                            RegData64 rtl_val_up = RTL_info.GetRegValUp();
                            RegData64 rtl_val_lo = RTL_info.GetRegValLo();

                            // Add GR update from CForest to CompRunner queue.
                            if (RTL_info.GetCommitType() == cmprunner::TYPE_EMP) {
                                cmprunner::APIErrorCode ret_upd = cforestif->CmpUpdRegInfo (rtl_peid, cmprunner::TYPE_ISS, RTL_info.GetNblkId(),
                                                                                            rtl_GR_num_chk, cforest_val_up, cforest_val_lo, cmp_ret);
                                if (ret_upd == cmprunner::API_E_OK){
                                    cmp_ret = cmprunner::CMP_ISS_UPD_OK;
                                } else {
                                    cmp_ret = cmprunner::CMP_ISS_UPD_ERR;
                                }
                            } else if (RTL_info.GetCommitType() == cmprunner::TYPE_RTL) {
                                // Get cmp_ret in Del function.
                                cmprunner::APIErrorCode ret_del = cforestif->CmpDelRegInfo (rtl_peid, cmprunner::TYPE_RTL, NBLKID_NONE,
                                                                                            rtl_GR_num_chk, cforest_val_up, cforest_val_lo, cmprunner::UNUSED_VAL, cmp_ret);
                                if (ret_del != cmprunner::API_E_OK){
                                    cmp_ret = cmprunner::CMP_ISS_DEL_ERR;
                                }
                            } else {
                                // Can't find CForest GR in CompRunner Buffer
                                cmp_ret = cmprunner::CMP_ISS_GET_ERR;
                            }
                            CmpPrintTrace(rtl_peid, rtl_time, rtl_cycle, cmp_ret, rtl_exp_pc, rtl_GR_num_chk, rtl_val_up, rtl_val_lo,
                                          cforest_val_up, cforest_val_lo, rtl_FL_Wen_chk.at(rtl_peid), rtl_FL_val_chk.at(rtl_peid), NBLKID_NONE, NBLKID_NONE, NBLKID_NONE, trace, false);
                        }
                    }
                }

                if (cmp_ret == cmprunner::CMP_MATCH) {
                    cmp_ret = cmprunner::CMP_LAST_MATCH;
                } else {
                    cmp_ret = cmprunner::CMP_LAST_MISMATCH;
                }

                // Check all TYPE_RTL and TYPE_EMPTY elements to guarantee they are not exist in CompRunner buffer.
                if (cforestif-> CheckLastCommit(rtl_peid) == false) {
                    cmp_ret = cmprunner::CMP_LAST_MISMATCH;
                }
            }

            CmpPrintTrace(rtl_peid, rtl_time, rtl_cycle, cmp_ret, rtl_exp_pc, R0, cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL,
                          cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL,
                          cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL, trace, false);
        }

        // If canceled instruction is unmatched, count error.
        if (cmp_ret_canceled != cmprunner::CMP_LAST_MATCH) {
            cmp_ret = cmprunner::CMP_LAST_MISMATCH;
        }

        if (IsCompareSuccess(cmp_ret) == COMP_DPI_MISMATCH) {
            uint32_t g_cur_error_count = 0;
            // Update error count.
            g_cur_error_count = g_cmp_error_count.at(rtl_peid) + 1;
            g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
            g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, g_cur_error_count);
        }

        fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
        return IsCompareSuccess(cmp_ret);
    }
    else {
        return COMP_DPI_MATCH;
    }
}

uint32_t ComprunnerG4MHv2::G4MHv2_ExpDbintRcuReqToCpu (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc, svLogicVecVal* rtl_brk_ch, uint32_t rtl_commit)
{
    if (m_cmprunner_en) {
        const std::string msg = "ExpDbintRcuReqToCpu";
        DEBUG_LOG(rtl_peid, rtl_time, rtl_cycle, &msg);

        cmprunner::RtlLogicVal log_rtl_brk_ch (rtl_brk_ch);
        if (!log_rtl_brk_ch.IsValidSignal()) {
            std::ostringstream reg_invalid;
            uint32_t rtl_signal_control = rtl_brk_ch->bval;
            uint32_t rtl_data_control = rtl_brk_ch->aval;
            reg_invalid << "Break channel = ";
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), rtl_signal_control, rtl_data_control, 0, 0);
            return COMP_DPI_SV_ERR;
        }

        if (!cforestif->IsPeIdValid(rtl_peid)) {
            std::ostringstream reg_invalid;
            reg_invalid << "PEID = " << rtl_peid;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, 0x0, 7, 0);
            return COMP_DPI_SV_ERR;
        }

        std::ostringstream oss;

        CmpPrintPC(&oss, rtl_exp_pc);
        CmpPrintSpace (&oss, 10);

        // Fix #97492 - start
        if ( rtl_commit) {
        // Fix #97492 - end
            oss << "[IDump : DBINT channel ";
            oss << std::dec << std::setfill ('0') << std::setw(2) << std::right << log_rtl_brk_ch.getIntVal() << "]" << std::endl;
            cforestif->ReqPseudoDbint(rtl_peid, log_rtl_brk_ch.getIntVal());

            CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
            g_cycle_history.PrintSpace ();
            fprintf (g_cmp_fp, "%s", oss.str().c_str());
        }

        return COMP_DPI_OK;
    }
    else {
        return COMP_DPI_OK;
    }
}

uint32_t ComprunnerG4MHv2::G4MHv2_ExpDbCpuAckToRcu (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc, svLogicVecVal* rtl_db_int_type, svLogicVecVal* rtl_brk_ch)
{
    if (m_cmprunner_en) {
        const std::string msg = "ExpDbCpuAckToRcu";
        DEBUG_LOG(rtl_peid, rtl_time, rtl_cycle, &msg);
        cmprunner::CmpResult cmp_ret = cmprunner::CMP_MATCH;

        cmprunner::RtlLogicVal log_rtl_db_int_type (rtl_db_int_type);
        cmprunner::RtlLogicVal log_rtl_brk_ch (rtl_brk_ch);
        if (!log_rtl_db_int_type.IsValidSignal() || !log_rtl_brk_ch.IsValidSignal()) {
            std::ostringstream reg_invalid;
            if (!log_rtl_db_int_type.IsValidSignal()) {
                uint32_t rtl_signal_control = rtl_db_int_type->bval;
                uint32_t rtl_data_control = rtl_db_int_type->aval;
                reg_invalid << "Debug Interruption type = ";
                ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), rtl_signal_control, rtl_data_control, 0, 0);
            } else {
                uint32_t rtl_signal_control = rtl_brk_ch->bval;
                uint32_t rtl_data_control = rtl_brk_ch->aval;
                reg_invalid << "Break channel = ";
                ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), rtl_signal_control, rtl_data_control, 0, 0);
            }
            return COMP_DPI_SV_ERR;
        }
        if (!cforestif->IsPeIdValid(rtl_peid)) {
            std::ostringstream reg_invalid;
            reg_invalid << "PEID = " << rtl_peid;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, 0x0, 7, 0);
            return COMP_DPI_SV_ERR;
        }
        if ((log_rtl_db_int_type.getIntVal() == cmprunner::CmpDBNMI) && (log_rtl_brk_ch.getIntVal() > 15)) {
            std::ostringstream reg_invalid;
            uint32_t rtl_signal_control = rtl_brk_ch->bval;
            uint32_t rtl_data_control = rtl_brk_ch->aval;
            reg_invalid << "DBNMI channel = " << log_rtl_brk_ch.getIntVal();
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), rtl_signal_control, rtl_data_control, 0, 0);
            return COMP_DPI_SV_ERR;
        }
        std::ostringstream oss;

        cmprunner::CmpResult cmp_ret_canceled = CmpStepCancel (rtl_peid, rtl_1st_cm_time, rtl_cycle, rtl_exp_pc);

        CmpPrintPC(&oss, rtl_exp_pc);
        CmpPrintSpace (&oss, 10);

        if (log_rtl_db_int_type.getIntVal() == cmprunner::CmpFENMI) {
            // Force CForest go to DBNMI status
            oss << "[IDump : DBNMI]" << std::endl;
            cforestif->ReqPseudoDbnmi (rtl_peid);
        } else if (log_rtl_db_int_type.getIntVal() == cmprunner::CmpFEINT) {
            // Force CForest go to DBINT status
            oss << "[IDump : DBINT channel ";
            oss << std::dec << std::setfill ('0') << std::setw(2) << std::right << log_rtl_brk_ch.getIntVal() << "]" << std::endl;
            cforestif->ReqPseudoDbint(rtl_peid, log_rtl_brk_ch.getIntVal());
        }

        CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
        g_cycle_history.PrintSpace ();
        fprintf (g_cmp_fp, "%s", oss.str().c_str());
        oss.str("");
        oss.clear();

        // Get trace info in case Interruption type.
        TraceCtrl* trace = NULL;

        // Execute instruction to cause
        cforestif->CmpStepExecute (rtl_peid);
        cforestif->CmpGetTrace (rtl_peid, &trace);

        if (trace->GetExpCode() != 0x0000) {
    //            g_cmp_exp_signal[rtl_peid] = true;

            CmpPrintSpace (&oss, 97);
            oss << "EXP[" << ExpInfo::GetName(trace->GetExpCode());
            oss << "] is ocurred";
            if (rtl_exp_pc != trace->GetInstAddr ()) {
                cmp_ret = cmprunner::CMP_PC_MISS;
            } else {
                if (trace->GetWregCount ()) {
                    for (int32_t i = 0; i < trace->GetWregCount (); i++) {
                        // In case G4MH, this operation can be ignored due to no VM
                        CReg regno = trace->GetWriteReg (i) & 0xfff;
                        if ((regno > R0 && regno <= R31) || (regno >= WREG_BASE && regno <= WREG_MAX)) {
                            CReg rtl_GR_num_chk = regno;
                            RegData64 cforest_val_up;
                            RegData64 cforest_val_lo;
                            if (regno >= WREG_BASE && regno <= WREG_MAX) {
                                cforest_val_up = (static_cast<uint64_t>(trace->GetWWregValue(i, 3)) << 32)
                                               | (static_cast<uint64_t>(trace->GetWWregValue(i, 2)) << 0);
                                cforest_val_lo = (static_cast<uint64_t>(trace->GetWWregValue(i, 1)) << 32)
                                               | (static_cast<uint64_t>(trace->GetWWregValue(i, 0)) << 0);
                            } else { // if (regno > R0 && regno <= R31) {
                                cforest_val_up = 0;
                                cforest_val_lo = trace->GetWregValue(i);
                            }
                            cmprunner::CmpRegInfo RTL_info;
                            // Get Register info with {GR number, TYPE_EMP} or {GR number, TYPE_RTL}
                            cforestif->CmpGetRegInfo (rtl_peid, cmprunner::CtxCompByLast, NBLKID_NONE, rtl_GR_num_chk, cforest_val_up, cforest_val_lo,
                                                                                          RTL_info, cmp_ret);
                            RegData64 rtl_val_up = RTL_info.GetRegValUp();
                            RegData64 rtl_val_lo = RTL_info.GetRegValLo();

                            // Add GR update from CForest to CompRunner queue.
                            if (RTL_info.GetCommitType() == cmprunner::TYPE_EMP) {
                                cmprunner::APIErrorCode ret_upd = cforestif->CmpUpdRegInfo (rtl_peid, cmprunner::TYPE_ISS, RTL_info.GetNblkId(),
                                                                                            rtl_GR_num_chk, cforest_val_up, cforest_val_lo, cmp_ret);
                                if (ret_upd == cmprunner::API_E_OK){
                                    cmp_ret = cmprunner::CMP_ISS_UPD_OK;
                                } else {
                                    cmp_ret = cmprunner::CMP_ISS_UPD_ERR;
                                }
                            } else if (RTL_info.GetCommitType() == cmprunner::TYPE_RTL) {
                                // Get cmp_ret in Del function.
                                cmprunner::APIErrorCode ret_del = cforestif->CmpDelRegInfo (rtl_peid, cmprunner::TYPE_RTL, NBLKID_NONE,
                                                                                            rtl_GR_num_chk, cforest_val_up, cforest_val_lo, cmprunner::UNUSED_VAL, cmp_ret);
                                if (ret_del != cmprunner::API_E_OK){
                                    cmp_ret = cmprunner::CMP_ISS_DEL_ERR;
                                }
                            } else {
                                // Can't find CForest GR in CompRunner Buffer
                                cmp_ret = cmprunner::CMP_ISS_GET_ERR;
                            }
                            CmpPrintTrace(rtl_peid, rtl_time, rtl_cycle, cmp_ret, rtl_exp_pc, rtl_GR_num_chk, rtl_val_up, rtl_val_lo,
                                          cforest_val_up, cforest_val_lo, rtl_FL_Wen_chk.at(rtl_peid), rtl_FL_val_chk.at(rtl_peid), NBLKID_NONE, NBLKID_NONE, NBLKID_NONE, trace, false);
                        }
                    }
                }

                if (cmp_ret == cmprunner::CMP_MATCH) {
                    cmp_ret = cmprunner::CMP_LAST_MATCH;
                } else {
                    cmp_ret = cmprunner::CMP_LAST_MISMATCH;
                }

                // Check all TYPE_RTL and TYPE_EMPTY elements to guarantee they are not exist in CompRunner buffer.
                if (cforestif-> CheckLastCommit(rtl_peid) == false) {
                    cmp_ret = cmprunner::CMP_LAST_MISMATCH;
                    cmprunner::CmpRegInfo RTL_info;
                }
            }

            CmpPrintTrace(rtl_peid, rtl_time, rtl_cycle, cmp_ret, rtl_exp_pc, R0, cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL,
                          cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL,
                          cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL, cmprunner::UNUSED_VAL, trace, false);
        }

        // If canceled instruction is unmatched, count error.
        if (cmp_ret_canceled != cmprunner::CMP_LAST_MATCH) {
            cmp_ret = cmprunner::CMP_LAST_MISMATCH;
        }

        if (IsCompareSuccess(cmp_ret) == COMP_DPI_MISMATCH) {
            uint32_t g_cur_error_count = 0;
            // Update error count.
            g_cur_error_count = g_cmp_error_count.at(rtl_peid) + 1;
            g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
            g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, g_cur_error_count);
        }

        fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
        return IsCompareSuccess(cmp_ret);
    }
    else {
        return COMP_DPI_MATCH;
    }
}

uint32_t ComprunnerG4MHv2::G4MHv2_CancelEiintReq(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle)
{
    if (m_cmprunner_en) {
        const std::string msg = "CancelEiintReq";
        DEBUG_LOG(rtl_peid, rtl_time, rtl_cycle, &msg);

        if (!cforestif->IsPeIdValid(rtl_peid)) {
            std::ostringstream reg_invalid;
            reg_invalid << "PEID = " << rtl_peid;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, 0x0, 7, 0);
            return COMP_DPI_SV_ERR;
        }

        std::ostringstream oss;
        CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
        CmpPrintSpace (&oss, 65);

        oss << "[IDUMP: Cancel Eiint request]";

        cforestif->CancelPseudoEiint(rtl_peid);

        fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
        return COMP_DPI_OK;
    }
    else {
        return COMP_DPI_OK;
    }

}

uint32_t ComprunnerG4MHv2::G4MHv2_CancelFeintReq(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle)
{
    if (m_cmprunner_en) {
        const std::string msg = "CancelFeintReq";
        DEBUG_LOG(rtl_peid, rtl_time, rtl_cycle, &msg);

        if (!cforestif->IsPeIdValid(rtl_peid)) {
            std::ostringstream reg_invalid;
            reg_invalid << "PEID = " << rtl_peid;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, 0x0, 7, 0);
            return COMP_DPI_SV_ERR;
        }

        std::ostringstream oss;
        CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
        CmpPrintSpace (&oss, 65);

        oss << "[IDUMP: Cancel Feint request]";

        cforestif->CancelPseudoFeint(rtl_peid);

        fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
        return COMP_DPI_OK;
    }
    else {
        return COMP_DPI_OK;
    }

}

uint32_t ComprunnerG4MHv2::G4MHv2_CancelFenmiReq(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle)
{
    if (m_cmprunner_en) {
        const std::string msg = "CancelFenmiReq";
        DEBUG_LOG(rtl_peid, rtl_time, rtl_cycle, &msg);

        if (!cforestif->IsPeIdValid(rtl_peid)) {
            std::ostringstream reg_invalid;
            reg_invalid << "PEID = " << rtl_peid;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, 0x0, 7, 0);
            return COMP_DPI_SV_ERR;
        }

        std::ostringstream oss;
        CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
        CmpPrintSpace (&oss, 65);

        oss << "[IDUMP: Cancel Fenmi request]";

        cforestif->CancelPseudoFenmi(rtl_peid);

        fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
        return COMP_DPI_OK;
    }
    else {
        return COMP_DPI_OK;
    }

}

uint32_t ComprunnerG4MHv2::G4MHv2_CancelDbintReq(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle)
{
    if (m_cmprunner_en) {
        const std::string msg = "CancelDbintReq";
        DEBUG_LOG(rtl_peid, rtl_time, rtl_cycle, &msg);

        if (!cforestif->IsPeIdValid(rtl_peid)) {
            std::ostringstream reg_invalid;
            reg_invalid << "PEID = " << rtl_peid;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, 0x0, 7, 0);
            return COMP_DPI_SV_ERR;
        }

        std::ostringstream oss;
        CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
        CmpPrintSpace (&oss, 65);

        oss << "[IDUMP: Cancel Dbint request]";

        cforestif->CancelPseudoDbint(rtl_peid);

        fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
        return COMP_DPI_OK;
    }
    else {
        return COMP_DPI_OK;
    }

}

uint32_t ComprunnerG4MHv2::G4MHv2_ResetPE(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc)
{
    if (m_cmprunner_en) {
        cmprunner::CmpResult cmp_ret_canceled = cmprunner::CMP_LAST_MATCH;
        const std::string msg = "ResetPE";
        DEBUG_LOG(rtl_peid, rtl_time, rtl_cycle, &msg);

        if (!cforestif->IsPeIdValid(rtl_peid)) {
            std::ostringstream reg_invalid;
            reg_invalid << "PEID = " << rtl_peid;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, 0x0, 7, 0);
            return COMP_DPI_SV_ERR;
        }

        if (g_cmp_eiitb_status.at(rtl_peid) == true) {
            // When individual reset event is occurred during EIINT table reading
            // CompRunner in C side simply clear all internal statuses during individual reset event.
            g_cmp_mcyc_middle.erase(g_cmp_mcyc_middle.begin() + rtl_peid);
            g_cmp_mcyc_middle.insert(g_cmp_mcyc_middle.begin() + rtl_peid, MCYC_MIDDLE_INITIAL);
            g_cmp_eiitb_status.erase(g_cmp_eiitb_status.begin() + rtl_peid);
            g_cmp_eiitb_status.insert(g_cmp_eiitb_status.begin() + rtl_peid, EIITB_STATUS_INITIAL);
        } else {
            // When individual reset event is occurred in others case
            // CompRunner in C side do the adjustment for the difference between RTL and CForest to compare update resource.
            cmp_ret_canceled = CmpStepCancel (rtl_peid, rtl_time, rtl_cycle, rtl_exp_pc);
        }
        std::ostringstream oss;
        CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
        CmpPrintSpace (&oss, 65);

        oss << "[IDUMP: Reset PE]";
        fprintf (g_cmp_fp, "%s\n", oss.str().c_str());

        cforestif->ResetPE(rtl_peid);

        if (cmp_ret_canceled != cmprunner::CMP_LAST_MATCH) {
            return cmprunner::CMP_LAST_MISMATCH;
        } else {
            return COMP_DPI_OK;
        }
    }
    else {
        return COMP_DPI_OK;
    }
}

uint32_t ComprunnerG4MHv2::G4MHv2_CmpSReg (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle,
                      svLogicVecVal* rtl_sreg_id, svLogicVecVal* rtl_sreg_ch,
                      svLogicVecVal* rtl_sreg_val, svLogicVecVal* rtl_sreg_mask)
{
    if (m_cmprunner_en) {
        const std::string msg = "CmpSReg";
        DEBUG_LOG(rtl_peid, rtl_time, rtl_cycle, &msg);
        if (!cforestif->IsPeIdValid(rtl_peid)) {
            std::ostringstream reg_invalid;
            reg_invalid << "PEID = " << rtl_peid;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, 0x0, 7, 0);
            return COMP_DPI_SV_ERR;
        }
        cmprunner::RtlLogicVal log_rtl_sreg_id   (rtl_sreg_id);
        cmprunner::RtlLogicVal log_rtl_sreg_ch   (rtl_sreg_ch);
        cmprunner::RtlLogicVal log_rtl_sreg_val  (rtl_sreg_val);
        cmprunner::RtlLogicVal log_rtl_sreg_mask (rtl_sreg_mask);

        if  ( !log_rtl_sreg_id.IsValidSignal()  || !log_rtl_sreg_ch.IsValidSignal()
           || !log_rtl_sreg_val.IsValidSignal() || !log_rtl_sreg_mask.IsValidSignal()) {
            uint32_t rtl_signal_control;
            uint32_t rtl_data_control;
            std::ostringstream reg_invalid;
            if (!log_rtl_sreg_id.IsValidSignal()) {
                rtl_signal_control = rtl_sreg_id->bval;
                rtl_data_control = rtl_sreg_id->aval;
                reg_invalid << "System register ID = ";
            } else if (!log_rtl_sreg_mask.IsValidSignal()) {
                rtl_signal_control = rtl_sreg_mask->bval;
                rtl_data_control = rtl_sreg_mask->aval;
                reg_invalid << "System register mask = ";
            } else if (!log_rtl_sreg_val.IsValidSignal()) {
                rtl_signal_control = rtl_sreg_val->bval;
                rtl_data_control = rtl_sreg_val->aval;
                reg_invalid << "System register value = ";
            } else {
                rtl_signal_control = rtl_sreg_ch->bval;
                rtl_data_control = rtl_sreg_ch->aval;
                reg_invalid << "System register channel = ";
            }
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), rtl_signal_control, rtl_data_control, 0, 0);
            return COMP_DPI_SV_ERR;
        }

        std::ostringstream oss;
        CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
        g_cycle_history.PrintSpace ();
        //RTL SYSREG addr {regID,selID} = [9-5], [4-0]
        uint32_t regid = ((log_rtl_sreg_id.getIntVal()) & 0x3e0) >> 5;
        uint32_t selid = (log_rtl_sreg_id.getIntVal()) & 0x01f;
        CReg sreg_id = 0x100 + (selid << 5) + regid;

        // @@ TODO : Need to use rtl_sreg_ch for banked system registers (MPLA/MPUA/MPAT/BPC/BPAV/BPAM)
        if (log_rtl_sreg_ch.getIntVal() != 0) {
            fprintf (g_cmp_fp, "System register channel != 0 isn't supported yet\n");
            return COMP_DPI_SV_ERR;
        }

        // Check if whether RTL signals are valid.
        RegData64 cforest_sreg_data = 0;

        bool is_success = cforestif->RegReadDebug (rtl_peid, sreg_id, &cforest_sreg_data);
        CmpPrintSpace(&oss, 27);
        if (is_success) {
            cmprunner::CmpResult cmp_ret = cmprunner::CMP_MATCH;
            uint32_t mask = log_rtl_sreg_mask.getIntVal();
            uint32_t rtl_val = log_rtl_sreg_val.getIntVal() & mask;
            uint32_t cf_val =  cforest_sreg_data & mask;
            if (rtl_val == cf_val) {
                oss << "<SREG Comp: " << RegID::GetName(sreg_id) << " is matched>";
            } else {
                cmp_ret = cmprunner::CMP_DATA_MISMATCH;
                uint32_t g_cur_error_count = 0;
                // Update error count.
                g_cur_error_count = g_cmp_error_count.at(rtl_peid) + 1;
                g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
                g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, g_cur_error_count);

                CmpPrintErrorCount(&oss, rtl_peid);
                oss << RegID::GetName(sreg_id);
                oss << std::setfill(' ') << std::setw(24) << std::left;
                oss << " is different>";
                oss << std::dec << " Selid: " << selid << " Regid: " << regid;
                oss << " MASK: ";
                oss << std::hex << std::setfill('0') << std::setw(8) << std::right << mask;
                oss << std::endl;
                CmpPrintErrorSpace (&oss);
                oss << "ISS : ";
                oss << std::hex << std::setfill('0') << std::setw(8) << std::right << cf_val;
                oss << std::endl;
                CmpPrintErrorSpace (&oss);
                oss << "RTL : ";
                oss << std::hex << std::setfill('0') << std::setw(8) << std::right << rtl_val;
            }
            uint32_t ret = IsCompareSuccess(cmp_ret);
#if DEBUG_NEED
            {
#else // #if DEBUG_NEED
            if (ret != COMP_DPI_MATCH) {
#endif // #if DEBUG_NEED
                fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
            }
            return ret;
        } else {
            uint32_t g_cur_error_count = 0;
            // Update error count.
            g_cur_error_count = g_cmp_error_count.at(rtl_peid) + 1;
            g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
            g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, g_cur_error_count);

            CmpPrintErrorCount(&oss, rtl_peid);
            oss << std::setfill(' ') << std::setw(24) << std::left;
            oss << "SREG id is invalid>";
            oss << std::dec << " selid: " << selid << " regid: " << regid;
            fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
            return COMP_DPI_C_ERR;
        }
    }
    else {
        return COMP_DPI_MATCH;
    }
}


uint32_t ComprunnerG4MHv2::G4MHv2_MemoryDataCheck (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, svLogicVecVal* rtl_st_addr, svLogicVecVal* rtl_st_val)
{
    if (m_cmprunner_en) {
        const std::string msg = "MemoryCheck";
        DEBUG_LOG(rtl_peid, rtl_time, rtl_cycle, &msg);
        if (!cforestif->IsPeIdValid(rtl_peid)) {
            std::ostringstream reg_invalid;
            reg_invalid << "PEID = " << rtl_peid;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, 0x0, 7, 0);
            return COMP_DPI_SV_ERR;
        }

        cmprunner::RtlLogicVal log_rtl_st_addr (rtl_st_addr);
        cmprunner::RtlLogicVal log_rtl_st_val (rtl_st_val);
        if (!log_rtl_st_addr.IsValidSignal() || !log_rtl_st_val.IsValidSignal()) {
            std::ostringstream reg_invalid;
            if (!log_rtl_st_addr.IsValidSignal()) {
                uint32_t rtl_signal_control = rtl_st_addr->bval;
                uint32_t rtl_data_control = rtl_st_addr->aval;
                reg_invalid << "Memory address";
                ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), rtl_signal_control, rtl_data_control, 0, 0);
            } else {
                uint32_t rtl_signal_control = rtl_st_val->bval;
                uint32_t rtl_data_control = rtl_st_val->aval;
                reg_invalid << "Memory data";
                ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), rtl_signal_control, rtl_data_control, 0, 0);
            }
            return COMP_DPI_SV_ERR;
        }
        std::ostringstream oss;
        CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
        g_cycle_history.PrintSpace ();
        CmpPrintSpace(&oss, 27);

        uint32_t cforest_data;
        cmprunner::APIErrorCode ret_written = cforestif->VMemReadDebug(rtl_peid,
                                                                       log_rtl_st_addr.getIntVal(), 0x4, &cforest_data);
        if (ret_written != cmprunner::API_E_OK) {
            CmpPrintErrorCount(&oss, rtl_peid);
            oss << "Can't access memory in CForest> (";
            oss << std::hex << std::right << std::setw(8) << std::setfill('0') << log_rtl_st_addr.getIntVal() << ")";
            fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
            return COMP_DPI_C_ERR;
        } else {
            cmprunner::CmpResult cmp_ret = cmprunner::CMP_MATCH;
            if (cforest_data == log_rtl_st_val.getIntVal()) {
                oss << "<Memory data is match> (";
                oss << std::hex << std::right << std::setw(8) << std::setfill('0') << log_rtl_st_addr.getIntVal() << ")=";
                oss << std::hex << std::right << std::setw(8) << std::setfill('0') << log_rtl_st_val.getIntVal() ;
            } else {
                cmp_ret = cmprunner::CMP_DATA_MISMATCH;
                uint32_t g_cur_error_count = 0;
                // Update error count.
                g_cur_error_count = g_cmp_error_count.at(rtl_peid) + 1;
                g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
                g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, g_cur_error_count);

                CmpPrintErrorCount(&oss, rtl_peid);
                oss << "Memory data is different> (";
                oss << std::hex << std::right << std::setw(8) << std::setfill('0') << log_rtl_st_addr.getIntVal() << ")=";
                oss << std::endl;
                // New line for RTL value
                CmpPrintErrorSpace (&oss);
                oss << "RTL : ";
                oss << std::hex << std::right << std::setw(8) << std::setfill('0') << log_rtl_st_val.getIntVal() ;
                oss << std::endl;
                // New line for CForest value
                CmpPrintErrorSpace (&oss);
                oss << "ISS : ";
                oss << std::hex << std::right << std::setw(8) << std::setfill('0') << cforest_data;
            }
            uint32_t ret = IsCompareSuccess(cmp_ret);
#if DEBUG_NEED
            {
#else // #if DEBUG_NEED
            if (ret != COMP_DPI_MATCH) {
#endif // #if DEBUG_NEED
                fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
            }
            return ret;
        }
    }
    else {
        return COMP_DPI_MATCH;
    }
}


uint32_t ComprunnerG4MHv2::G4MHv2_CmpPeState (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, svLogicVecVal* rtl_state)
{
    if (m_cmprunner_en) {
        const std::string msg = "CmpPeState";
        DEBUG_LOG(rtl_peid, rtl_time, rtl_cycle, &msg);
        if (!cforestif->IsPeIdValid(rtl_peid)) {
            std::ostringstream reg_invalid;
            reg_invalid << "PEID = " << rtl_peid;
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), 0x0, 0x0, 7, 0);
            return COMP_DPI_SV_ERR;
        }

        cmprunner::RtlLogicVal log_rtl_state (rtl_state);
        if (!log_rtl_state.IsValidSignal()) {
            std::ostringstream reg_invalid;
            uint32_t rtl_signal_control = rtl_state->bval;
            uint32_t rtl_data_control = rtl_state->aval;
            reg_invalid << "RTL state";
            ParseRTLSignal(rtl_peid, rtl_time, rtl_cycle, reg_invalid.str(), rtl_signal_control, rtl_data_control, 0, 0);
            return COMP_DPI_SV_ERR;
        }
        std::ostringstream oss;
        CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
        g_cycle_history.PrintSpace ();
        CmpPrintSpace(&oss, 27);

        TraceCtrl* trace = NULL;
        cforestif->CmpGetTrace (rtl_peid, &trace);
        NotifySimState cf_sim_state = trace->GetSimState ();
        uint32_t cf_is_halt = ((cf_sim_state == SIMULATE_HALT)? 1 : 0);
        uint32_t rtl_is_halt = log_rtl_state.getIntVal();

        cmprunner::CmpResult cmp_ret = cmprunner::CMP_MATCH;
        if (cf_is_halt == rtl_is_halt) {
            oss << "<Halt status is match> State=";
            oss << std::dec << std::right << std::setw(1) << log_rtl_state.getIntVal();
        } else {
            cmp_ret = cmprunner::CMP_DATA_MISMATCH;
            uint32_t g_cur_error_count = 0;
            // Update error count.
            g_cur_error_count = g_cmp_error_count.at(rtl_peid) + 1;
            g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
            g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, g_cur_error_count);

            CmpPrintErrorCount(&oss, rtl_peid);
            oss << "Halt status is different> State=";
            oss << std::endl;
            // New line for RTL value
            CmpPrintErrorSpace (&oss);
            oss << "RTL : ";
            oss << std::dec << std::right << std::setw(1) << log_rtl_state.getIntVal();
            oss << std::endl;
            // New line for CForest value
            CmpPrintErrorSpace (&oss);
            oss << "ISS : ";
            oss << std::dec << std::right << std::setw(1) << cf_is_halt;
        }
        uint32_t ret = IsCompareSuccess(cmp_ret);
    #if DEBUG_NEED
        {
    #else // #if DEBUG_NEED
        if (ret != COMP_DPI_MATCH) {
    #endif // #if DEBUG_NEED
            fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
        }
        return ret;
    }
    else {
        return COMP_DPI_MATCH;
    }
}


/**
 * Print functions in CompRunner G4MH
 *
 */

/**
 * Sub-print functions
 *
 */

void ComprunnerG4MHv2::CmpPrintBorder (std::ostringstream* oss)
{
    *oss << std::setfill('-') << std::setw(200) << "" << std::endl;
}


void ComprunnerG4MHv2::CmpPrintErrorSpace (std::ostringstream* oss)
{
    *oss << std::setfill(' ') << std::setw (47) << ' ';
}


void ComprunnerG4MHv2::CmpPrintErrorCount (std::ostringstream* oss, uint32_t rtl_peid) {
    if (g_cmp_error_count.at(rtl_peid) > 99) {
        *oss << "<ERR xx: ";
    } else {
        *oss << "<ERR ";
        *oss << std::dec << std::right << std::setfill('0') << std::setw(2) << g_cmp_error_count.at(rtl_peid);
        *oss << ": ";
    }
}


void ComprunnerG4MHv2::CmpPrintSpace (std::ostringstream* oss, int32_t space_num)
{
    *oss << std::setfill(' ') << std::setw (space_num) << ' ';
}


void ComprunnerG4MHv2::CmpPrintRTLInfo (std::ostringstream* oss_in, uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle)
{
    std::ostringstream oss;
    oss << std::dec << std::setfill(' ');
    oss << std::setw(10) << std::right << rtl_time;
    oss << std::setw(6) << std::right << "PE" << rtl_peid;
    oss << std::setw(3) << std::right << ":NC";
    oss << std::setw(10) << std::right << rtl_cycle;
    fprintf (g_cmp_fp, "%s", oss.str().c_str());
}


void ComprunnerG4MHv2::CmpPrintAction (std::ostringstream* oss, uint32_t cmp_action)
{
    switch (cmp_action) {
    case cmprunner::ActEmpty:

        *oss << std::setfill(' ') << std::setw(9) << std::left << "  Empty ";
        *oss << std::setfill(' ') << std::setw(9) << std::left << "   ---   ";
        *oss << std::setfill(' ') << std::setw(9) << std::left << "   ---   ";

        break;
    case cmprunner::ActRTLInfo:

        *oss << std::setfill(' ') << std::setw(9) << std::left << "   ---   ";
        *oss << std::setfill(' ') << std::setw(9) << std::left << "   RTL   ";
        *oss << std::setfill(' ') << std::setw(9) << std::left << "   ---   ";

        break;
    case cmprunner::ActCForestInfo:

        *oss << std::setfill(' ') << std::setw(9) << std::left << "   ---   ";
        *oss << std::setfill(' ') << std::setw(9) << std::left << " CForest ";
        *oss << std::setfill(' ') << std::setw(9) << std::left << "   ---   ";

        break;
    case cmprunner::ActMatch:

        *oss << std::setfill(' ') << std::setw(9) << std::left << "   ---   ";
        *oss << std::setfill(' ') << std::setw(9) << std::left << "   ---   ";
        *oss << std::setfill(' ') << std::setw(9) << std::left << "  MATCH  ";

        break;
    default:
        CmpPrintSpace (oss, 35);
    }
    return ;
}


void ComprunnerG4MHv2::CmpPrintNBLK (std::ostringstream* oss, uint32_t nblk_id)
{
    *oss << std::dec << std::setfill('0');
    *oss << " [NBLK = " << std::setw(3) << nblk_id << "]  ";
}


void ComprunnerG4MHv2::CmpPrintPC (std::ostringstream* oss, uint32_t pc)
{
    *oss << std::hex << std::setfill('0');
    *oss << " [PC=" << std::setw(8) << pc << "] : ";
}


void ComprunnerG4MHv2::CmpPrintGR (std::ostringstream* oss, uint32_t reg_no, uint32_t reg_val)
{
    *oss << "GR[";
    *oss << std::dec << std::setfill ('0') << std::setw(2) << std::right << reg_no;
    *oss << "]=";
    *oss << std::hex << std::setfill ('0') << std::setw(8) << std::right << reg_val;

}


void ComprunnerG4MHv2::CmpPrintWR (std::ostringstream* oss, uint32_t wreg_no,
                 uint32_t wrdata0_val, uint32_t wrdata1_val, uint32_t wrdata2_val, uint32_t wrdata3_val)
{
    // WReg number
    *oss << "WR[";
    *oss << std::dec << std::setfill ('0') << std::setw(2) << std::right << wreg_no;
    *oss << "]=";
    // WReg value
    *oss << std::hex << std::setfill ('0') << std::setw(8) << std::right << wrdata3_val << "_";
    *oss << std::hex << std::setfill ('0') << std::setw(8) << std::right << wrdata2_val << "_";
    *oss << std::hex << std::setfill ('0') << std::setw(8) << std::right << wrdata1_val << "_";
    *oss << std::hex << std::setfill ('0') << std::setw(8) << std::right << wrdata0_val;
}


void ComprunnerG4MHv2::CmpPrintFlag (std::ostringstream* oss, uint32_t aval)
{
    *oss << "FLAG=";
    *oss << std::hex << std::setfill('0') << std::setw(2) << std::right << aval;
}


void ComprunnerG4MHv2::CmpPrintInstMnem (std::ostringstream* oss, std::string* inst_mnem, TraceCtrl* trace)
{
    if (trace == NULL) {
        return ;
    }
    *oss << std::hex << " ; ";
    uint32_t  peid = trace->GetPeId ();
    ContextId tcid = NC;
    ContextId vcid = NC;
    char      vcidstr = ((vcid == NC) ? 'M' : static_cast<char>('0' + vcid));
    PrivLevel priv = trace->GetPrivLevel ();
    *oss << "P" << peid << ":" << ((vcid == NC) ? 'N' : 'V') << vcidstr
         << ":T" << ((tcid == NC) ? 0 : tcid)  << ":" << CfMsg::GetPrivName(priv) << ":";
    *oss << "   ";
    *oss << std::left << std::setfill(' ');
    *oss << std::setw(23) << *inst_mnem << ' ';

    TracePrintCForest *trace_print = new TracePrintCForest (CORE_G4MH);
    trace_print->PrintRegMemTrace (oss, trace);
    trace_print->PrintBranchTrace (oss, trace);
    delete trace_print;
}


void ComprunnerG4MHv2::CmpPrintTrace (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, cmprunner::CmpResult ret, uint32_t rtl_pc, uint32_t iss_regno_in,
                    uint64_t rtl_reg_val_up, uint64_t rtl_reg_val_lo, uint64_t iss_reg_val_up, uint64_t iss_reg_val_lo,
                    uint32_t rtl_flag_en, uint32_t rtl_flag_val, uint32_t iss_flag_en, uint32_t iss_flag_val, uint32_t rtl_nblk_id, TraceCtrl *trace, bool cyc_comp_msg)
{
    if (IsCompareSuccess(ret) == COMP_DPI_MISMATCH) {
        // Update final status.
        g_cmp_final_status.erase(g_cmp_final_status.begin() + rtl_peid);
        g_cmp_final_status.insert(g_cmp_final_status.begin() + rtl_peid, 1);
    }
    std::ostringstream oss;
    // Step 1: Print RTL basic info
    CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);

    if (cyc_comp_msg & m_cyc_cmp_en) {
        // Temporary call DPI function
        G4MHv2_CmpSetIssueCycle(rtl_peid, rtl_time, trace->GetInstId());

        // Cycle comparison message
        uint64_t rtl_issue_time = SearchIssueCycle (rtl_peid, trace->GetInstId());

        uint64_t cf_cycle = trace->GetStageCycle(cforestif->GetCmpStage());
        uint64_t cf_step  = trace->GetStepCount();
        uint32_t inst_no  = trace->GetInstId ();
        if (rtl_issue_time != static_cast<uint64_t>(-1)) {
            g_cycle_history.Set (rtl_peid, rtl_issue_time, rtl_pc, cf_cycle, cf_step,       // control here for INSTRUCTION LIST
                    inst_no, trace->GetInstCode());
            //time ,step ,cycle のログ出力
            g_cycle_history.PrintCycle (rtl_peid);
        } else {
            // empty commit, so skip
            g_cycle_history.PrintSpace ();
        }
    } else {
        g_cycle_history.PrintSpace ();
    }


    uint32_t iss_regno = 0;
    CmpConvertRegNumPrint(iss_regno_in, iss_regno);

    // Step 2: Print RTL PC
    CmpPrintPC(&oss, rtl_pc);

    std::string inst_mnem;
    // Printf insn opcode
    if (trace != NULL) {
        ByteSize inst_size = trace->GetInstSize ();
        uint64_t inst_code = trace->GetInstCode ();
        if (inst_size == SIZE_16BIT) {
            oss << "xxxx" << std::setw(4) << (inst_code & 0xFFFFU) << ' ';
        } else {
            oss << std::setw(8) << (inst_code & 0xFFFFFFFFU);
            oss << ((inst_size == SIZE_32BIT) ? ' ' : ((inst_size == SIZE_48BIT) ? 'x' : 'X'));
        }
        oss << ' ';

        // Get operand info
        cforestif->CmpGetOperand (trace->GetInstId (), trace->GetInstCode (),
                                  trace->GetInstAddr (), &inst_mnem);
    } else {
        CmpPrintSpace (&oss, 10);
    }
    // Get value of WR.
    RegData rtl_wdata_0 = 0x0, rtl_wdata_1 = 0x0, rtl_wdata_2 = 0x0, rtl_wdata_3 = 0x0;
    RegData iss_wdata_0 = 0x0, iss_wdata_1 = 0x0, iss_wdata_2 = 0x0, iss_wdata_3 = 0x0;
    if (iss_regno_in >= WREG_BASE && iss_regno_in <= WREG_MAX) {
        rtl_wdata_3 = (rtl_reg_val_up >> 32)   & 0x00FFFFFFFFUL;
        rtl_wdata_2 = rtl_reg_val_up           & 0x00FFFFFFFFUL;
        rtl_wdata_1 = (rtl_reg_val_lo >> 32)   & 0x00FFFFFFFFUL;
        rtl_wdata_0 = rtl_reg_val_lo           & 0x00FFFFFFFFUL;

        iss_wdata_3 = (iss_reg_val_up >> 32)   & 0x00FFFFFFFFUL;
        iss_wdata_2 = iss_reg_val_up           & 0x00FFFFFFFFUL;
        iss_wdata_1 = (iss_reg_val_lo >> 32)   & 0x00FFFFFFFFUL;
        iss_wdata_0 = iss_reg_val_lo           & 0x00FFFFFFFFUL;
    }
    // Print CompRunner return value
    cmprunner::CmpResult tmp_ret = ret & (~cmprunner::CMP_FLAG_MISS);
    switch (tmp_ret) {
    case cmprunner::CMP_MATCH:
        if (iss_regno_in > R0 && iss_regno_in <= R31) {
            CmpPrintGR (&oss, iss_regno, iss_reg_val_lo & 0x0FFFFFFFF);
            CmpPrintSpace (&oss, 27);
        } else if (iss_regno_in >= WREG_BASE && iss_regno_in <= WREG_MAX) {
            CmpPrintWR(&oss, iss_regno, rtl_wdata_0, rtl_wdata_1, rtl_wdata_2, rtl_wdata_3);
        } else {
            CmpPrintSpace (&oss, 42);
        }
        oss << "  ";
        CmpPrintSpace (&oss, 15);
        CmpPrintAction(&oss, cmprunner::ActMatch);
        break;
    case cmprunner::CMP_ISS_UPD_OK:
        if (iss_regno_in > R0 && iss_regno_in <= R31) {
            CmpPrintGR (&oss, iss_regno, iss_reg_val_lo & 0x0FFFFFFFF);
            CmpPrintSpace (&oss, 27);
        } else if (iss_regno_in >= WREG_BASE && iss_regno_in <= WREG_MAX) {
            CmpPrintWR(&oss, iss_regno, rtl_wdata_0, rtl_wdata_1, rtl_wdata_2, rtl_wdata_3);
        } else {
            CmpPrintSpace (&oss, 42);
        }
        oss << "  ";
        CmpPrintSpace (&oss, 15);
        CmpPrintAction(&oss, cmprunner::ActCForestInfo);
        break;
    case cmprunner::CMP_C2B1_COMMIT:
        if (iss_regno_in <= R31) {
            oss << std::setfill(' ') << std::setw(15) << std::left;
            oss << " C2B1 ";
            CmpPrintSpace (&oss, 1);
            uint32_t psw_flag_print = 0x0;
            cforestif->GetCmpIssFlags(rtl_peid, &psw_flag_print);
            CmpPrintFlag (&oss, psw_flag_print);
            CmpPrintSpace (&oss, 19);
        } else {
            oss << std::setfill(' ') << std::setw(42) << std::left;
            oss << " C2B1 ";
        }
        CmpPrintInstMnem (&oss, &inst_mnem, trace);
        break;
    case cmprunner::CMP_LAST_MATCH:
        if (iss_regno_in <= R31) {
            if (iss_regno_in == R0) {
                CmpPrintSpace (&oss, 15);
            } else {
                CmpPrintGR (&oss, iss_regno, iss_reg_val_lo & 0x0FFFFFFFF);
            }
            CmpPrintSpace (&oss, 1);
            uint32_t psw_flag_print = 0x0;
            cforestif->GetCmpIssFlags(rtl_peid, &psw_flag_print);
            CmpPrintFlag (&oss, psw_flag_print);
            CmpPrintSpace (&oss, 19);
        } else if (iss_regno_in >= WREG_BASE && iss_regno_in <= WREG_MAX) {
            CmpPrintWR(&oss, iss_regno, rtl_wdata_0, rtl_wdata_1, rtl_wdata_2, rtl_wdata_3);
        } else {
            CmpPrintSpace (&oss, 42);
        }
        CmpPrintInstMnem (&oss, &inst_mnem, trace);
        break;
    case cmprunner::CMP_LAST_MISMATCH:
        CmpPrintErrorCount(&oss, rtl_peid);
        oss << std::setfill(' ') << std::setw(24) << std::left;
        oss << "Last commit is MISMATCH>";
        if (iss_regno_in > R0 && iss_regno_in <= R31) {
            oss << "GR[" << std::dec << std::setfill ('0') << std::setw(2) << std::right << iss_regno << "]";
        } else if (iss_regno_in >= WREG_BASE && iss_regno_in <= WREG_MAX) {
            oss << "WR[" << std::dec << std::setfill ('0') << std::setw(2) << std::right << iss_regno << "]";
        } else {
            CmpPrintSpace (&oss, 6);
        }
        CmpPrintSpace (&oss, 3);
        CmpPrintInstMnem (&oss, &inst_mnem, trace);
        break;
    case cmprunner::CMP_PC_MISS:
        CmpPrintErrorCount(&oss, rtl_peid);
        oss << std::setfill(' ') << std::setw(33) << std::left;
        oss << "PC is different>";
        CmpPrintInstMnem (&oss, &inst_mnem, trace);
        oss << std::endl;
        // New line for RTL value
        CmpPrintErrorSpace (&oss);
        oss << "RTL : ";
        oss << std::hex << std::setfill('0');
        oss << " [PC=" << std::setw(8) << rtl_pc << "]";
        oss << std::endl;
        // New line for CForest value
        CmpPrintErrorSpace (&oss);
        oss << "ISS : ";
        oss << std::hex << std::setfill('0');
        oss << " [PC=" << std::setw(8) << trace->GetInstAddr() << "]";
        break;
    case cmprunner::CMP_REG_LACK:
    case cmprunner::CMP_REG_MISS:
        CmpPrintErrorCount(&oss, rtl_peid);
        oss << std::setfill(' ') << std::setw(29) << std::left;
        oss << "Lacking of register update>";
        oss << std::endl;
        // New line for CForest value
        CmpPrintErrorSpace (&oss);
        oss << "ISS : ";
        if (iss_regno_in > R0 && iss_regno_in <= R31) {
            CmpPrintGR (&oss, iss_regno, iss_reg_val_lo & 0x0FFFFFFFF);
        } else if (iss_regno_in >= WREG_BASE && iss_regno_in <= WREG_MAX) {
            CmpPrintWR(&oss, iss_regno, rtl_wdata_0, rtl_wdata_1, rtl_wdata_2, rtl_wdata_3);
        } else {
            CmpPrintSpace (&oss, 42);
        }
        break;
    case cmprunner::CMP_DATA_MISS:
        CmpPrintErrorCount(&oss, rtl_peid);
        oss << std::setfill(' ') << std::setw(29) << std::left;
        oss << "Register value is different>";
        oss << std::endl;
        // New line for RTL value
        CmpPrintErrorSpace (&oss);
        oss << "RTL : ";
        if (iss_regno_in > R0 && iss_regno_in <= R31) {
            CmpPrintGR (&oss, iss_regno, rtl_reg_val_lo & 0x0FFFFFFFF);
        } else if (iss_regno_in >= WREG_BASE && iss_regno_in <= WREG_MAX) {
            CmpPrintWR(&oss, iss_regno, rtl_wdata_0, rtl_wdata_1, rtl_wdata_2, rtl_wdata_3);
        } else {
            CmpPrintSpace (&oss, 42);
        }
        oss << std::endl;
        // New line for CForest value
        CmpPrintErrorSpace (&oss);
        oss << "ISS : ";
        if (iss_regno_in > R0 && iss_regno_in <= R31) {
            CmpPrintGR (&oss, iss_regno, iss_reg_val_lo & 0x0FFFFFFFF);
        } else if (iss_regno_in >= WREG_BASE && iss_regno_in <= WREG_MAX) {
            CmpPrintWR(&oss, iss_regno, iss_wdata_0, iss_wdata_1, iss_wdata_2, iss_wdata_3);
        } else {
            CmpPrintSpace (&oss, 42);
        }
        break;
    case cmprunner::CMP_FLAG_LACK:
        CmpPrintErrorCount(&oss, rtl_peid);
        oss << std::setfill(' ') << std::setw(33) << std::left;
        oss << "Lacking of flag enable>";
        CmpPrintInstMnem (&oss, &inst_mnem, trace);
        oss << std::endl;
        CmpPrintErrorSpace (&oss);
        oss << "RTL : ";
        CmpPrintFlag (&oss, rtl_flag_en);
        oss << std::endl;
        CmpPrintErrorSpace (&oss);
        oss << "ISS : ";
        CmpPrintFlag (&oss, iss_flag_en);
        break;
    case cmprunner::CMP_ISS_UPD_ERR:
        CmpPrintErrorCount(&oss, rtl_peid);
        oss << std::setfill(' ') << std::setw(29) << std::left;
        oss << "Can't find EMP type >";
        oss << "NBLK ID = ";
        oss << std::setfill('0') << std::setw(3) << std::right << rtl_nblk_id;
        break;
    case cmprunner::CMP_ISS_DEL_ERR:
        CmpPrintErrorCount(&oss, rtl_peid);
        oss << std::setfill(' ') << std::setw(24) << std::left;
        oss << "Can't find RTL type>";
        if (iss_regno_in > R0 && iss_regno_in <= R31) {
            oss << "GR[" << std::dec << std::setfill ('0') << std::setw(2) << std::right << iss_regno << "]";
        } else if (iss_regno_in >= WREG_BASE && iss_regno_in <= WREG_MAX) {
            oss << "WR[" << std::dec << std::setfill ('0') << std::setw(2) << std::right << iss_regno << "]";
        } else {
            CmpPrintSpace (&oss, 6);
        }
        break;
    case cmprunner::CMP_ISS_GET_ERR:
        CmpPrintErrorCount(&oss, rtl_peid);
        oss << std::setfill(' ') << std::setw(24) << std::left;
        oss << "Can't find match reg>";
        if (iss_regno_in > R0 && iss_regno_in <= R31) {
            CmpPrintGR (&oss, iss_regno, iss_reg_val_lo & 0x0FFFFFFFF);
        } else if (iss_regno_in >= WREG_BASE && iss_regno_in <= WREG_MAX) {
            CmpPrintWR(&oss, iss_regno, iss_wdata_0, iss_wdata_1, iss_wdata_2, iss_wdata_3);
        } else {
            CmpPrintSpace (&oss, 6);
        }

        break;

    default: break;
    }

    if (ret > cmprunner::CMP_FLAG_MISS) {
        oss << std::endl;
        CmpPrintSpace (&oss, 57);
        CmpPrintErrorCount(&oss, rtl_peid);
        oss << std::setfill(' ') << std::setw(29) << std::left;
        oss << "Flag is different>" << std::endl;
        CmpPrintErrorSpace (&oss);
        oss << "RTL : ";
        CmpPrintFlag (&oss, rtl_flag_val & rtl_flag_en);
        oss << std::endl;
        CmpPrintErrorSpace (&oss);
        oss << "ISS : ";
        CmpPrintFlag (&oss, iss_flag_val & iss_flag_en);
    } else if (ret == cmprunner::CMP_FLAG_MISS) {
        CmpPrintErrorCount(&oss, rtl_peid);
        oss << std::setfill(' ') << std::setw(33) << std::left;
        oss << "Flag is different>";
        CmpPrintInstMnem (&oss, &inst_mnem, trace);
        oss << std::endl;
        CmpPrintErrorSpace (&oss);
        oss << "RTL : ";
        CmpPrintFlag (&oss, rtl_flag_val & rtl_flag_en);
        oss << std::endl;
        CmpPrintErrorSpace (&oss);
        oss << "ISS : ";
        CmpPrintFlag (&oss, iss_flag_val & iss_flag_en);
    } else if (ret == cmprunner::CMP_OMIT) {
        CmpPrintSpace (&oss, 42);
        CmpPrintInstMnem (&oss, &inst_mnem, trace);
    }
    fprintf (g_cmp_fp, "%s\n", oss.str().c_str());

    return;
}


void ComprunnerG4MHv2::CmpRegBufTrace (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, cmprunner::CmpResult ret,
                    uint32_t rtl_pc, uint32_t rtl_regno_in, uint64_t rtl_reg_val_up, uint64_t rtl_reg_val_lo, uint32_t rtl_nblk_id)
{
    if (IsRegisterBufferError(ret) == true) {
        // Update final status.
        g_cmp_final_status.erase(g_cmp_final_status.begin() + rtl_peid);
        g_cmp_final_status.insert(g_cmp_final_status.begin() + rtl_peid, 1);
    }
    std::ostringstream oss;
    uint32_t rtl_reg_no = 0;
    CmpConvertRegNumPrint(rtl_regno_in, rtl_reg_no);

    // Step 1: Print RTL basic info
    CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
    g_cycle_history.PrintSpace ();
    CmpPrintPC(&oss, rtl_pc);
    CmpPrintSpace (&oss, 10);

    // Step 2: Print Non-blocking info with return value
    switch (ret) {
    case cmprunner::CMP_RTL_ADD_EMP_ERR:
        CmpPrintErrorCount(&oss, rtl_peid);
        oss << std::setfill(' ') << std::setw(29) << std::left;
        oss << "Can't register RTL info> ";
        oss << "NBLK = " << std::dec << std::setfill('0') << std::setw(3) << std::right << rtl_nblk_id;
        if (rtl_regno_in > R0 && rtl_regno_in <= R31) {
            oss << " GR[";
            oss << std::dec << std::setfill ('0') << std::setw(2) << std::right << rtl_reg_no << "]";
        } else if (rtl_regno_in >= WREG_BASE && rtl_regno_in <= WREG_MAX) {
            oss << " WR[";
            oss << std::dec << std::setfill ('0') << std::setw(2) << std::right << rtl_reg_no << "]";
        } else {
            CmpPrintSpace (&oss, 6);
        }
        break;
    case cmprunner::CMP_RTL_ADD_EMP_OK:
        if (rtl_regno_in > R0 && rtl_regno_in <= R31) {
            oss << "GR[" << std::dec << std::setfill ('0') << std::setw(2) << std::right << rtl_reg_no << "] is not available";
            CmpPrintSpace (&oss, 19);
        } else if (rtl_regno_in >= WREG_BASE && rtl_regno_in <= WREG_MAX) {
            oss << "WR[" << std::dec << std::setfill ('0') << std::setw(2) << std::right << rtl_reg_no << "] is not available";
            CmpPrintSpace (&oss, 19);
        } else {
            CmpPrintSpace (&oss, 25);
        }
        oss << "  ";
        CmpPrintNBLK(&oss, rtl_nblk_id);
        CmpPrintAction(&oss, cmprunner::ActEmpty);
        break;
    case cmprunner::CMP_RTL_ADD_REG_ERR:
        CmpPrintErrorCount(&oss, rtl_peid);
        oss << std::setfill(' ') << std::setw(29) << std::left;
        oss << "Can't register RTL info> ";
        if (rtl_regno_in > R0 && rtl_regno_in <= R31) {
            oss << "GR" << std::dec << std::setfill('0') << std::setw(2) << std::right << rtl_reg_no;
        } else if (rtl_regno_in >= WREG_BASE && rtl_regno_in <= WREG_MAX) {
            oss << "WR" << std::dec << std::setfill('0') << std::setw(2) << std::right << rtl_reg_no;
        } else {
            oss << "Index [" << std::dec << std::setfill ('0') << std::setw(2) << std::right << rtl_regno_in << "] is not available";
        }
        break;
    case cmprunner::CMP_RTL_ADD_REG_OK:
        if (rtl_regno_in > R0 && rtl_regno_in <= R31) {
            CmpPrintGR (&oss, rtl_reg_no, rtl_reg_val_lo & 0x0FFFFFFFF);
            CmpPrintSpace (&oss, 27);
        } else if (rtl_regno_in >= WREG_BASE && rtl_regno_in <= WREG_MAX) {
            CmpPrintWR(&oss, rtl_reg_no, rtl_reg_val_lo & 0x0FFFFFFFF, (rtl_reg_val_lo & 0x0FFFFFFFF00000000) >> 32,
                                         rtl_reg_val_up & 0x0FFFFFFFF, (rtl_reg_val_up & 0x0FFFFFFFF00000000) >> 32);
        } else {
            CmpPrintSpace (&oss, 6);
        }
        oss << "  ";
        CmpPrintSpace (&oss, 15);
        CmpPrintAction(&oss, cmprunner::ActRTLInfo);
        break;
    case cmprunner::CMP_RTL_ADD_EMP_DUP:
        if (rtl_regno_in > R0 && rtl_regno_in <= R31) {
            oss << "GR[" << std::dec << std::setfill ('0') << std::setw(2) << std::right << rtl_reg_no << "] is not available";
            CmpPrintSpace (&oss, 19);
        } else if (rtl_regno_in >= WREG_BASE && rtl_regno_in <= WREG_MAX) {
            CmpPrintWR(&oss, rtl_reg_no, rtl_reg_val_lo & 0x0FFFFFFFF, (rtl_reg_val_lo & 0x0FFFFFFFF00000000) >> 32,
                                         rtl_reg_val_up & 0x0FFFFFFFF, (rtl_reg_val_up & 0x0FFFFFFFF00000000) >> 32);
        } else {
            CmpPrintSpace (&oss, 42);
        }
        oss << "  ";
        CmpPrintNBLK(&oss, rtl_nblk_id);
        CmpPrintAction(&oss, cmprunner::ActEmpty);
        oss << std::setfill(' ') << std::setw(14) << std::left << " Duplication ";
        break;
    case cmprunner::CMP_RTL_ADD_REG_DUP:
        if (rtl_regno_in > R0 && rtl_regno_in <= R31) {
            CmpPrintGR (&oss, rtl_reg_no, rtl_reg_val_lo & 0x0FFFFFFFF);
            CmpPrintSpace (&oss, 27);
        } else if (rtl_regno_in >= WREG_BASE && rtl_regno_in <= WREG_MAX) {
            CmpPrintWR(&oss, rtl_reg_no, rtl_reg_val_lo & 0x0FFFFFFFF, (rtl_reg_val_lo & 0x0FFFFFFFF00000000) >> 32,
                                         rtl_reg_val_up & 0x0FFFFFFFF, (rtl_reg_val_up & 0x0FFFFFFFF00000000) >> 32);
        } else {
            CmpPrintSpace (&oss, 42);
        }
        oss << "  ";
        CmpPrintSpace (&oss, 15);
        CmpPrintAction(&oss, cmprunner::ActRTLInfo);
        oss << std::setfill(' ') << std::setw(14) << std::left << " Duplication ";
        break;
    case cmprunner::CMP_RTL_ADD_R0_ERR:
        CmpPrintErrorCount(&oss, rtl_peid);
        oss << std::setfill(' ') << std::setw(29) << std::left;
        oss << "R0 is updated> ";
        break;
    default: break;
    }

    fprintf (g_cmp_fp, "%s\n", oss.str().c_str());

    return;
}


void ComprunnerG4MHv2::CmpNbPrintTrace (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, cmprunner::CmpResult ret, uint32_t iss_regno_in,
                      uint64_t rtl_reg_val_up, uint64_t rtl_reg_val_lo, uint64_t iss_reg_val_up, uint64_t iss_reg_val_lo, uint32_t rtl_nblk_id)
{
    if (IsCompareSuccess(ret) == COMP_DPI_MISMATCH) {
        // Update final status.
        g_cmp_final_status.erase(g_cmp_final_status.begin() + rtl_peid);
        g_cmp_final_status.insert(g_cmp_final_status.begin() + rtl_peid, 1);
    }
    std::ostringstream oss;
    uint32_t iss_regno = 0;
    CmpConvertRegNumPrint(iss_regno_in, iss_regno);

    // Step 1: Print RTL basic info
    CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
    g_cycle_history.PrintSpace ();
    CmpPrintSpace (&oss, 27);

    // Step 2: Print Non-blocking info with return value
    switch (ret) {
    case cmprunner::CMP_MATCH:
        if (iss_regno_in > R0 && iss_regno_in <= R31) {
            CmpPrintGR (&oss, iss_regno, rtl_reg_val_lo & 0x0FFFFFFFF);
            CmpPrintSpace (&oss, 27);
        } else if (iss_regno_in >= WREG_BASE && iss_regno_in <= WREG_MAX) {
            CmpPrintWR(&oss, iss_regno, rtl_reg_val_lo & 0x0FFFFFFFF, (rtl_reg_val_lo & 0x0FFFFFFFF00000000) >> 32,
                                        rtl_reg_val_up & 0x0FFFFFFFF, (rtl_reg_val_up & 0x0FFFFFFFF00000000) >> 32);
        } else {
            CmpPrintSpace (&oss, 42);
        }
        oss << "  ";
        CmpPrintNBLK(&oss, rtl_nblk_id);
        CmpPrintAction(&oss, cmprunner::ActMatch);
        break;
    case cmprunner::CMP_OMIT:
        if (iss_regno_in > R0 && iss_regno_in <= R31) {
            CmpPrintGR (&oss, iss_regno, rtl_reg_val_lo & 0x0FFFFFFFF);
            CmpPrintSpace (&oss, 27);
        } else if (iss_regno_in >= WREG_BASE && iss_regno_in <= WREG_MAX) {
            CmpPrintWR(&oss, iss_regno, rtl_reg_val_lo & 0x0FFFFFFFF, (rtl_reg_val_lo & 0x0FFFFFFFF00000000) >> 32,
                                        rtl_reg_val_up & 0x0FFFFFFFF, (rtl_reg_val_up & 0x0FFFFFFFF00000000) >> 32);
        } else {
            CmpPrintSpace (&oss, 42);
        }
        oss << "  ";
        CmpPrintNBLK(&oss, rtl_nblk_id);
        CmpPrintAction(&oss, cmprunner::ActRTLInfo);
        break;
    case cmprunner::CMP_NB_MISS:
        CmpPrintErrorCount(&oss, rtl_peid);
        oss << "This NBLK is not exist>       NBLK = ";
        oss << std::dec << std::setfill('0') << std::setw(3) << std::right;
        oss << rtl_nblk_id;
        break;
    case cmprunner::CMP_DATA_MISS:
        CmpPrintErrorCount(&oss, rtl_peid);
        oss << "register value is different> NBLK = ";
        oss << std::dec << std::setfill('0') << std::setw(3) << std::right;
        oss << rtl_nblk_id;
        oss << std::endl;
        CmpPrintErrorSpace (&oss);
        oss << "RTL : ";
        // SV side can't commit GR in RTL env, so GR in CForest is displayed.
        if (iss_regno_in > R0 && iss_regno_in <= R31) {
            CmpPrintGR (&oss, iss_regno, rtl_reg_val_lo & 0x0FFFFFFFF);
        } else if (iss_regno_in >= WREG_BASE && iss_regno_in <= WREG_MAX) {
            CmpPrintWR(&oss, iss_regno, rtl_reg_val_lo & 0x0FFFFFFFF, (rtl_reg_val_lo & 0x0FFFFFFFF00000000) >> 32,
                                        rtl_reg_val_up & 0x0FFFFFFFF, (rtl_reg_val_up & 0x0FFFFFFFF00000000) >> 32);
        } else {
            CmpPrintSpace (&oss, 42);
        }
        oss << std::endl;
        CmpPrintErrorSpace (&oss);
        oss << "ISS : ";
        if (iss_regno_in > R0 && iss_regno_in <= R31) {
            CmpPrintGR (&oss, iss_regno, iss_reg_val_lo & 0x0FFFFFFFF);
        } else if (iss_regno_in >= WREG_BASE && iss_regno_in <= WREG_MAX) {
            CmpPrintWR(&oss, iss_regno, iss_reg_val_lo & 0x0FFFFFFFF, (iss_reg_val_lo & 0x0FFFFFFFF00000000) >> 32,
                                        iss_reg_val_up & 0x0FFFFFFFF, (iss_reg_val_up & 0x0FFFFFFFF00000000) >> 32);
        } else {
            CmpPrintSpace (&oss, 42);
        }
        break;
    case cmprunner::CMP_RTL_UPD_ERR:
        CmpPrintErrorCount(&oss, rtl_peid);
        oss << std::setfill(' ') << std::setw(29) << std::left;
        oss << "Can't find EMP type >         NBLK = ";
        oss << std::setfill('0') << std::setw(3) << std::left << rtl_nblk_id;
        break;
    default: break;
    }

    fprintf (g_cmp_fp, "%s\n", oss.str().c_str());

    return;
}


void ComprunnerG4MHv2::CmpPrintExp (uint32_t rtl_peid, TraceCtrl* trace,  uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_pc)
{
    std::ostringstream oss;

    // Step 1: Print RTL basic info
    CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
    g_cycle_history.PrintSpace ();

    // Step 2: Print RTL PC
    CmpPrintPC(&oss, rtl_pc);

    //Print instruction code
    ByteSize inst_size = trace->GetInstSize ();
    uint64_t inst_code = trace->GetInstCode ();
    if (inst_size == SIZE_16BIT) {
        oss << "xxxx" << std::setw(4) << (inst_code & 0xFFFFU) << ' ';
    } else {
        oss << std::setw(8) << (inst_code & 0xFFFFFFFFU);
        oss << ((inst_size == SIZE_32BIT) ? ' ' : ((inst_size == SIZE_48BIT) ? 'x' : 'X'));
    }
    oss << ' ';

    std::string exp_name = ExpInfo::GetName(trace->GetExpCode());
    oss << "<EXP [" << exp_name << "]>";
    oss << std::setw(41 - 8 - exp_name.length()) << std::setfill(' ');

    std::string inst_mnem;
    cforestif->CmpGetOperand (trace->GetInstId (), trace->GetInstCode (),
                              trace->GetInstAddr (), &inst_mnem);

    CmpPrintSpace (&oss, 4);
    CmpPrintInstMnem (&oss, &inst_mnem, trace);

    fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
    return;
}


void ComprunnerG4MHv2::CmpPrintTitle ()
{
    std::ostringstream oss;
    oss << std::endl;
    CmpPrintBorder(&oss);
    // Debug info
    oss << std::left << std::dec;
    oss << "| RTL time | Context | RTL inst | ISS inst | ISS cyc | RTL cyc | Diff |";
    oss << "      PC      | Inst code |    GR, WR writeback, Flag comparison    |         Mnemonic         " << std::endl;
    CmpPrintBorder(&oss);
    fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
}


void ComprunnerG4MHv2::ParseRTLSignal (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, std::string signal_message,
                     uint32_t rtl_signal_control, uint32_t rtl_signal_data, uint32_t max_val, uint32_t min_val){
    uint32_t g_cur_error_count = 0;
    // Update error count.
    g_cur_error_count = g_cmp_error_count.at(rtl_peid) + 1;
    g_cmp_error_count.erase(g_cmp_error_count.begin() + rtl_peid);
    g_cmp_error_count.insert(g_cmp_error_count.begin() + rtl_peid, g_cur_error_count);

    std::ostringstream oss;
    // Step 1: Print RTL basic info
    CmpPrintRTLInfo(&oss, rtl_peid, rtl_time, rtl_cycle);
    g_cycle_history.PrintSpace ();
    CmpPrintSpace (&oss, 17);
    if ((rtl_signal_control == 0xFFFFFFFF) && (rtl_signal_data == 0xFFFFFFFF)) {
        CmpPrintErrorCount(&oss, rtl_peid);
        oss << "Undefined value from SV side> " << signal_message.c_str() << " = ";
        oss << "xxxxxxxx";
    } else if ((rtl_signal_control == 0xFFFFFFFF) && (rtl_signal_data == 0x00000000)) {
        CmpPrintErrorCount(&oss, rtl_peid);
        oss << "Undefined value from SV side> " << signal_message.c_str() << " = ";
        oss << "zzzzzzzz";
    } else if (rtl_signal_control != 0x00000000) {
        CmpPrintErrorCount(&oss, rtl_peid);
        oss << "Undefined value from SV side> " << signal_message.c_str() << " = ";
        for (int32_t i = 7; i >= 0; i--) {
            uint32_t bit_signal_control = (rtl_signal_control >> i) & 0x00000001;
            uint32_t bit_signal_data    = (rtl_signal_data >> i) & 0x00000001;
            if (bit_signal_control) {
                if (bit_signal_data) {
                    oss << "X";
                } else {
                    oss << "Z";
                }
            } else {
                oss << bit_signal_data;
            }
        }
    } else {
        CmpPrintErrorCount(&oss, rtl_peid);
        oss << "Value from SV side is not in range> " << signal_message.c_str() << " ∉ [";
        oss << max_val << ", " << min_val << " ]";
    }
    fprintf (g_cmp_fp, "%s\n", oss.str().c_str());
}


bool ComprunnerG4MHv2::IsRegisterBufferError (cmprunner::CmpResult ret)
{
    if (ret == cmprunner::CMP_RTL_ADD_EMP_ERR ||
        ret == cmprunner::CMP_RTL_ADD_R0_ERR ||
        ret == cmprunner::CMP_RTL_ADD_REG_ERR ) {
        return true;
    }
    return false;
}


bool ComprunnerG4MHv2::CmpConvertRegNumStore (uint32_t rtl_reg_num_in, uint32_t &cforest_reg_num_out)
{
    bool ret_val = true;
    if ((rtl_reg_num_in > R0 && rtl_reg_num_in <= R31) || (rtl_reg_num_in == cmprunner::CmpR32)) {
        cforest_reg_num_out = rtl_reg_num_in;
    } else if (rtl_reg_num_in >= cmprunner::CmpWRegMin && rtl_reg_num_in <= cmprunner::CmpWRegMax) {
        // Validate RTL value
        cforest_reg_num_out = rtl_reg_num_in + WREG_BASE - cmprunner::CmpWRegMin;
    } else {
        ret_val = false;
    }
    return ret_val;
}


bool ComprunnerG4MHv2::CmpConvertRegNumPrint (uint32_t cforest_reg_num_in, uint32_t &cforest_reg_num_out)
{
    bool ret_val = true;
    if (cforest_reg_num_in > R0 && cforest_reg_num_in <= R31) {
        cforest_reg_num_out = cforest_reg_num_in;
    } else if (cforest_reg_num_in >= WREG_BASE && cforest_reg_num_in <= WREG_MAX) {
        // Validate RTL value
        cforest_reg_num_out = cforest_reg_num_in - WREG_BASE;
    } else {
        ret_val = false;
    }
    return ret_val;
}


// CYCLE COMPARISON FUNCTION: START //
/**
 * \typedef typedef std::vector<IssueCycle *> IssueCycleVec
 * \brief Define 2 variable for issue cycle vector: PE0, PE1.
 *
 * There are 2 vector in C side now.
 *
 * - \ref cycle_issue_vector0         : It means issue cycle in the first PE.
 * - \ref cycle_issue_vector1         : It means issue cycle in the second PE.
 *
 */
typedef std::vector<IssueCycle *> IssueCycleVec;
/**
 * \var IssueCycleVec cycle_issue_vector0
 * \brief It means issue cycle in the first PE
 *
 */
IssueCycleVec cycle_issue_vector0,
/**
 * \var IssueCycleVec cycle_issue_vector1
 * \brief It means issue cycle in the second PE
 *
 */
    cycle_issue_vector1;

//**
//** Get Issue Cycle Time and Instruction Issue
//**
uint32_t ComprunnerG4MHv2::G4MHv2_CmpSetIssueCycle (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_inst_id)
{
    IssueCycle *cycle_issue = new IssueCycle (rtl_peid, rtl_time, rtl_inst_id);
    if (rtl_peid == PE0) {
        cycle_issue_vector0.push_back (cycle_issue);
    } else if (rtl_peid == PE1) {
        cycle_issue_vector1.push_back (cycle_issue);
    }

    return 0;
}


//**
//** Search Cycle Time from Instruction ID
//**
uint64_t ComprunnerG4MHv2::SearchIssueCycle (uint32_t rtl_peid, uint32_t rtl_inst_id)
{
    uint64_t rtl_cycle;
    IssueCycleVec::iterator it;
    if (rtl_peid == PE0) {
        it = cycle_issue_vector0.begin();
        while (it != cycle_issue_vector0.end()) {
            if ((*it)->GetPEID () == rtl_peid &&
                (*it)->GetInstID () == rtl_inst_id) {
                // find corresponding ID
                rtl_cycle = (*it)->GetRtlCycle ();
                delete (*it);
                cycle_issue_vector0.erase (it);
                return rtl_cycle;
            }
            ++it;
        }
    } else if (rtl_peid == PE1) {
        it = cycle_issue_vector1.begin();
        while (it != cycle_issue_vector1.end()) {
            if ((*it)->GetPEID () == rtl_peid &&
                (*it)->GetInstID () == rtl_inst_id) {
                // find corresponding ID
                rtl_cycle = (*it)->GetRtlCycle ();
                delete (*it);
                cycle_issue_vector1.erase (it);
                return rtl_cycle;
            }
            ++it;
        }
    }
    // doesn't find corresponding cycle time with instruction ID
    return static_cast<uint64_t>(-1);
}

uint32_t ComprunnerG4MHv2::G4MHv2_CmpSetIssueFlush (uint32_t rtl_peid)
{
    if (rtl_peid == PE0) {
        cycle_issue_vector0.clear ();
    } else if (rtl_peid == PE1) {
        cycle_issue_vector1.clear ();
    }
    return 0;
}


// CYCLE COMPARISON FUNCTION: FINISH //


uint32_t ComprunnerG4MHv2::IsCompareSuccess (cmprunner::CmpResult ret)
{
    if (ret == cmprunner::CMP_PC_MISS ||
        ret == cmprunner::CMP_FLAG_LACK ||
        ret == cmprunner::CMP_FLAG_MISS ||
        ret == cmprunner::CMP_REG_LACK ||
        ret == cmprunner::CMP_REG_MISS ||
        ret == cmprunner::CMP_DATA_MISS ||
        ret == cmprunner::CMP_NB_MISS ||
        ret == cmprunner::CMP_ISS_UPD_ERR ||
        ret == cmprunner::CMP_ISS_DEL_ERR ||
        ret == cmprunner::CMP_ISS_GET_ERR ||
        ret == cmprunner::CMP_RTL_UPD_ERR ||
        ret == cmprunner::CMP_LAST_MISMATCH ||
        ret >= cmprunner::CMP_FLAG_MISS ||
        ret == cmprunner::CMP_DATA_MISMATCH) {
        return COMP_DPI_MISMATCH;
    } else {
        return COMP_DPI_MATCH;
    }
}


