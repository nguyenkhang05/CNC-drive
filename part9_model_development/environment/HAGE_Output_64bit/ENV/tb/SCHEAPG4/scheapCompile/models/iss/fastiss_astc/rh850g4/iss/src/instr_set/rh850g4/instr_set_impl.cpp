//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2017.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850g4/instr_set_impl.h"
#include "rh850g4/add_reg1_reg2_instr.h"
#include "rh850g4/add_imm5_reg2_instr.h"
#include "rh850g4/addi_imm16_reg1_reg2_instr.h"
#include "rh850g4/adf_cccc_reg1_reg2_reg3_instr.h"
#include "rh850g4/and_reg1_reg2_instr.h"
#include "rh850g4/andi_imm16_reg1_reg2_instr.h"
#include "rh850g4/b_cccc_disp9_instr.h"
#include "rh850g4/bsh_reg2_reg3_instr.h"
#include "rh850g4/bsw_reg2_reg3_instr.h"
#include "rh850g4/callt_imm6_instr.h"
#include "rh850g4/caxi_ind_reg1_reg2_reg3_instr.h"
#include "rh850g4/clr1_bit_3_disp16_ind_reg1_instr.h"
#include "rh850g4/clr1_reg2_ind_reg1_instr.h"
#include "rh850g4/cmov_cccc_reg1_reg2_reg3_instr.h"
#include "rh850g4/cmov_cccc_imm5_reg2_reg3_instr.h"
#include "rh850g4/cmp_reg1_reg2_instr.h"
#include "rh850g4/cmp_imm5_reg2_instr.h"
#include "rh850g4/ctret_instr.h"
#include "rh850g4/di_instr.h"
#include "rh850g4/dispose_imm5_list12_instr.h"
#include "rh850g4/dispose_imm5_list12_ind_reg1_instr.h"
#include "rh850g4/div_reg1_reg2_reg3_instr.h"
#include "rh850g4/switch_reg1_instr.h"
#include "rh850g4/divh_reg1_reg2_instr.h"
#include "rh850g4/divh_reg1_reg2_reg3_instr.h"
#include "rh850g4/divhu_reg1_reg2_reg3_instr.h"
#include "rh850g4/divq_reg1_reg2_reg3_instr.h"
#include "rh850g4/divqu_reg1_reg2_reg3_instr.h"
#include "rh850g4/divu_reg1_reg2_reg3_instr.h"
#include "rh850g4/ei_instr.h"
#include "rh850g4/eiret_instr.h"
#include "rh850g4/feret_instr.h"
#include "rh850g4/fetrap_vector_instr.h"
#include "rh850g4/halt_instr.h"
#include "rh850g4/hsh_reg2_reg3_instr.h"
#include "rh850g4/hsw_reg2_reg3_instr.h"
#include "rh850g4/jr_disp22_instr.h"
#include "rh850g4/jr_disp32_instr.h"
#include "rh850g4/jarl_disp22_reg2_instr.h"
#include "rh850g4/jarl_disp32_reg1_instr.h"
#include "rh850g4/jmp_ind_reg1_instr.h"
#include "rh850g4/jmp_disp32_ind_reg1_instr.h"
#include "rh850g4/prepare_list12_imm5_instr.h"
#include "rh850g4/prepare_list12_imm5_bits_ff_instr.h"
#include "rh850g4/ld_dot_b_disp16_ind_reg1_reg2_instr.h"
#include "rh850g4/ld_dot_b_disp23_ind_reg1_reg3_instr.h"
#include "rh850g4/ld_dot_b_ind_reg1_inc_reg3_instr.h"
#include "rh850g4/ld_dot_b_ind_reg1_dec_reg3_instr.h"
#include "rh850g4/ld_dot_bu_disp23_ind_reg1_reg3_instr.h"
#include "rh850g4/ld_dot_bu_disp16_ind_reg1_reg2_instr.h"
#include "rh850g4/ld_dot_bu_ind_reg1_inc_reg3_instr.h"
#include "rh850g4/ld_dot_bu_ind_reg1_dec_reg3_instr.h"
#include "rh850g4/ld_dot_h_disp16_ind_reg1_reg2_instr.h"
#include "rh850g4/ld_dot_h_disp23_ind_reg1_reg3_instr.h"
#include "rh850g4/ld_dot_h_ind_reg1_inc_reg3_instr.h"
#include "rh850g4/ld_dot_h_ind_reg1_dec_reg3_instr.h"
#include "rh850g4/ld_dot_hu_disp16_ind_reg1_reg2_instr.h"
#include "rh850g4/ld_dot_hu_disp23_ind_reg1_reg3_instr.h"
#include "rh850g4/ld_dot_hu_ind_reg1_inc_reg3_instr.h"
#include "rh850g4/ld_dot_hu_ind_reg1_dec_reg3_instr.h"
#include "rh850g4/ld_dot_w_disp16_ind_reg1_reg2_instr.h"
#include "rh850g4/ld_dot_w_disp23_ind_reg1_reg3_instr.h"
#include "rh850g4/ld_dot_w_ind_reg1_inc_reg3_instr.h"
#include "rh850g4/ld_dot_w_ind_reg1_dec_reg3_instr.h"
#include "rh850g4/ldsr_reg2_regid_selid_instr.h"
#include "rh850g4/mac_reg1_reg2_reg3_reg4_instr.h"
#include "rh850g4/macu_reg1_reg2_reg3_reg4_instr.h"
#include "rh850g4/nop_instr.h"
#include "rh850g4/mov_reg1_reg2_instr.h"
#include "rh850g4/mov_imm5_reg2_instr.h"
#include "rh850g4/mov_imm32_reg1_instr.h"
#include "rh850g4/movea_imm16_reg1_reg2_instr.h"
#include "rh850g4/movhi_imm16_reg1_reg2_instr.h"
#include "rh850g4/mul_reg1_reg2_reg3_instr.h"
#include "rh850g4/sxh_reg1_instr.h"
#include "rh850g4/mul_imm9_reg2_reg3_instr.h"
#include "rh850g4/mulh_reg1_reg2_instr.h"
#include "rh850g4/mulh_imm5_reg2_instr.h"
#include "rh850g4/mulhi_imm16_reg1_reg2_instr.h"
#include "rh850g4/mulu_reg1_reg2_reg3_instr.h"
#include "rh850g4/mulu_imm9_reg2_reg3_instr.h"
#include "rh850g4/not_reg1_reg2_instr.h"
#include "rh850g4/not1_bit_3_disp16_ind_reg1_instr.h"
#include "rh850g4/not1_reg2_ind_reg1_instr.h"
#include "rh850g4/or_reg1_reg2_instr.h"
#include "rh850g4/ori_imm16_reg1_reg2_instr.h"
#include "rh850g4/rie_instr.h"
#include "rh850g4/rie_imm5_imm4_instr.h"
#include "rh850g4/sar_reg1_reg2_instr.h"
#include "rh850g4/sar_imm5_reg2_instr.h"
#include "rh850g4/sar_reg1_reg2_reg3_instr.h"
#include "rh850g4/sasf_cccc_reg2_instr.h"
#include "rh850g4/sxb_reg1_instr.h"
#include "rh850g4/zxb_reg1_instr.h"
#include "rh850g4/zxh_reg1_instr.h"
#include "rh850g4/satadd_reg1_reg2_instr.h"
#include "rh850g4/satadd_imm5_reg2_instr.h"
#include "rh850g4/satadd_reg1_reg2_reg3_instr.h"
#include "rh850g4/satsub_reg1_reg2_instr.h"
#include "rh850g4/satsub_reg1_reg2_reg3_instr.h"
#include "rh850g4/satsubi_imm16_reg1_reg2_instr.h"
#include "rh850g4/satsubr_reg1_reg2_instr.h"
#include "rh850g4/sbf_cccc_reg1_reg2_reg3_instr.h"
#include "rh850g4/sch0l_reg2_reg3_instr.h"
#include "rh850g4/sch0r_reg2_reg3_instr.h"
#include "rh850g4/sch1l_reg2_reg3_instr.h"
#include "rh850g4/sch1r_reg2_reg3_instr.h"
#include "rh850g4/set1_bit_3_disp16_ind_reg1_instr.h"
#include "rh850g4/set1_reg2_ind_reg1_instr.h"
#include "rh850g4/setf_cccc_reg2_instr.h"
#include "rh850g4/shl_reg1_reg2_instr.h"
#include "rh850g4/shl_imm5_reg2_instr.h"
#include "rh850g4/shl_reg1_reg2_reg3_instr.h"
#include "rh850g4/shr_reg1_reg2_instr.h"
#include "rh850g4/shr_imm5_reg2_instr.h"
#include "rh850g4/shr_reg1_reg2_reg3_instr.h"
#include "rh850g4/sld_dot_b_disp7_ind_ep_reg2_instr.h"
#include "rh850g4/sld_dot_bu_disp4_ind_ep_reg2_instr.h"
#include "rh850g4/sld_dot_h_disp8_ind_ep_reg2_instr.h"
#include "rh850g4/sld_dot_hu_disp5_ind_ep_reg2_instr.h"
#include "rh850g4/sld_dot_w_disp8_ind_ep_reg2_instr.h"
#include "rh850g4/sst_dot_b_reg2_disp7_ind_ep_instr.h"
#include "rh850g4/sst_dot_h_reg2_disp8_ind_ep_instr.h"
#include "rh850g4/sst_dot_w_reg2_disp8_ind_ep_instr.h"
#include "rh850g4/st_dot_b_reg2_disp16_ind_reg1_instr.h"
#include "rh850g4/st_dot_b_reg3_disp23_ind_reg1_instr.h"
#include "rh850g4/st_dot_b_reg3_ind_reg1_inc_instr.h"
#include "rh850g4/st_dot_b_reg3_ind_reg1_dec_instr.h"
#include "rh850g4/st_dot_h_reg2_disp16_ind_reg1_instr.h"
#include "rh850g4/st_dot_h_reg3_disp23_ind_reg1_instr.h"
#include "rh850g4/st_dot_h_reg3_ind_reg1_inc_instr.h"
#include "rh850g4/st_dot_h_reg3_ind_reg1_dec_instr.h"
#include "rh850g4/st_dot_w_reg2_disp16_ind_reg1_instr.h"
#include "rh850g4/st_dot_w_reg3_disp23_ind_reg1_instr.h"
#include "rh850g4/st_dot_w_reg3_ind_reg1_inc_instr.h"
#include "rh850g4/st_dot_w_reg3_ind_reg1_dec_instr.h"
#include "rh850g4/stsr_regid_reg2_selid_instr.h"
#include "rh850g4/sub_reg1_reg2_instr.h"
#include "rh850g4/subr_reg1_reg2_instr.h"
#include "rh850g4/synce_instr.h"
#include "rh850g4/syncm_instr.h"
#include "rh850g4/syncp_instr.h"
#include "rh850g4/syscall_vector_instr.h"
#include "rh850g4/trap_vector_instr.h"
#include "rh850g4/tst_reg1_reg2_instr.h"
#include "rh850g4/tst1_bit_3_disp16_ind_reg1_instr.h"
#include "rh850g4/tst1_reg2_ind_reg1_instr.h"
#include "rh850g4/xor_reg1_reg2_instr.h"
#include "rh850g4/xori_imm16_reg1_reg2_instr.h"
#include "rh850g4/absf_dot_d_reg2_reg3_instr.h"
#include "rh850g4/absf_dot_s_reg2_reg3_instr.h"
#include "rh850g4/addf_dot_d_reg1_reg2_reg3_instr.h"
#include "rh850g4/addf_dot_s_reg1_reg2_reg3_instr.h"
#include "rh850g4/ceilf_dot_dl_reg2_reg3_instr.h"
#include "rh850g4/ceilf_dot_dul_reg2_reg3_instr.h"
#include "rh850g4/ceilf_dot_duw_reg2_reg3_instr.h"
#include "rh850g4/ceilf_dot_dw_reg2_reg3_instr.h"
#include "rh850g4/ceilf_dot_sl_reg2_reg3_instr.h"
#include "rh850g4/ceilf_dot_sul_reg2_reg3_instr.h"
#include "rh850g4/ceilf_dot_suw_reg2_reg3_instr.h"
#include "rh850g4/ceilf_dot_sw_reg2_reg3_instr.h"
#include "rh850g4/cmovf_dot_d_fcbit_reg1_reg2_reg3_instr.h"
#include "rh850g4/cmovf_dot_s_fcbit_reg1_reg2_reg3_instr.h"
#include "rh850g4/cmpf_dot_d_fcond_reg2_reg1_fcbit_instr.h"
#include "rh850g4/cmpf_dot_s_fcond_reg2_reg1_fcbit_instr.h"
#include "rh850g4/cvtf_dot_dl_reg2_reg3_instr.h"
#include "rh850g4/cvtf_dot_ds_reg2_reg3_instr.h"
#include "rh850g4/cvtf_dot_dul_reg2_reg3_instr.h"
#include "rh850g4/cvtf_dot_duw_reg2_reg3_instr.h"
#include "rh850g4/cvtf_dot_dw_reg2_reg3_instr.h"
#include "rh850g4/cvtf_dot_ld_reg2_reg3_instr.h"
#include "rh850g4/cvtf_dot_ls_reg2_reg3_instr.h"
#include "rh850g4/cvtf_dot_sd_reg2_reg3_instr.h"
#include "rh850g4/cvtf_dot_sl_reg2_reg3_instr.h"
#include "rh850g4/cvtf_dot_sul_reg2_reg3_instr.h"
#include "rh850g4/cvtf_dot_suw_reg2_reg3_instr.h"
#include "rh850g4/cvtf_dot_sw_reg2_reg3_instr.h"
#include "rh850g4/cvtf_dot_uld_reg2_reg3_instr.h"
#include "rh850g4/cvtf_dot_uls_reg2_reg3_instr.h"
#include "rh850g4/cvtf_dot_uwd_reg2_reg3_instr.h"
#include "rh850g4/cvtf_dot_uws_reg2_reg3_instr.h"
#include "rh850g4/cvtf_dot_wd_reg2_reg3_instr.h"
#include "rh850g4/cvtf_dot_ws_reg2_reg3_instr.h"
#include "rh850g4/divf_dot_d_reg1_reg2_reg3_instr.h"
#include "rh850g4/divf_dot_s_reg1_reg2_reg3_instr.h"
#include "rh850g4/floorf_dot_dl_reg2_reg3_instr.h"
#include "rh850g4/floorf_dot_dul_reg2_reg3_instr.h"
#include "rh850g4/floorf_dot_duw_reg2_reg3_instr.h"
#include "rh850g4/floorf_dot_dw_reg2_reg3_instr.h"
#include "rh850g4/floorf_dot_sl_reg2_reg3_instr.h"
#include "rh850g4/floorf_dot_sul_reg2_reg3_instr.h"
#include "rh850g4/floorf_dot_suw_reg2_reg3_instr.h"
#include "rh850g4/floorf_dot_sw_reg2_reg3_instr.h"
#include "rh850g4/maddf_dot_s_reg1_reg2_reg3_reg4_instr.h"
#include "rh850g4/maxf_dot_d_reg1_reg2_reg3_instr.h"
#include "rh850g4/maxf_dot_s_reg1_reg2_reg3_instr.h"
#include "rh850g4/minf_dot_d_reg1_reg2_reg3_instr.h"
#include "rh850g4/minf_dot_s_reg1_reg2_reg3_instr.h"
#include "rh850g4/msubf_dot_s_reg1_reg2_reg3_reg4_instr.h"
#include "rh850g4/mulf_dot_d_reg1_reg2_reg3_instr.h"
#include "rh850g4/mulf_dot_s_reg1_reg2_reg3_instr.h"
#include "rh850g4/negf_dot_d_reg2_reg3_instr.h"
#include "rh850g4/negf_dot_s_reg2_reg3_instr.h"
#include "rh850g4/nmaddf_dot_s_reg1_reg2_reg3_reg4_instr.h"
#include "rh850g4/nmsubf_dot_s_reg1_reg2_reg3_reg4_instr.h"
#include "rh850g4/recipf_dot_d_reg2_reg3_instr.h"
#include "rh850g4/recipf_dot_s_reg2_reg3_instr.h"
#include "rh850g4/rsqrtf_dot_d_reg2_reg3_instr.h"
#include "rh850g4/rsqrtf_dot_s_reg2_reg3_instr.h"
#include "rh850g4/sqrtf_dot_d_reg2_reg3_instr.h"
#include "rh850g4/sqrtf_dot_s_reg2_reg3_instr.h"
#include "rh850g4/subf_dot_d_reg1_reg2_reg3_instr.h"
#include "rh850g4/subf_dot_s_reg1_reg2_reg3_instr.h"
#include "rh850g4/trfsr_fcbit_instr.h"
#include "rh850g4/trncf_dot_dl_reg2_reg3_instr.h"
#include "rh850g4/trncf_dot_dul_reg2_reg3_instr.h"
#include "rh850g4/trncf_dot_duw_reg2_reg3_instr.h"
#include "rh850g4/trncf_dot_dw_reg2_reg3_instr.h"
#include "rh850g4/trncf_dot_sl_reg2_reg3_instr.h"
#include "rh850g4/trncf_dot_sul_reg2_reg3_instr.h"
#include "rh850g4/trncf_dot_suw_reg2_reg3_instr.h"
#include "rh850g4/trncf_dot_sw_reg2_reg3_instr.h"
#include "rh850g4/b_cccc_disp17_instr.h"
#include "rh850g4/binsu_reg1_lsb_msb_reg2_instr.h"
#include "rh850g4/binsm_reg1_lsb_msb_reg2_instr.h"
#include "rh850g4/binsl_reg1_lsb_msb_reg2_instr.h"
#include "rh850g4/jarl_ind_reg1_reg3_instr.h"
#include "rh850g4/ld_dot_dw_disp23_ind_reg1_reg3_instr.h"
#include "rh850g4/ldl_dot_bu_ind_reg1_reg3_instr.h"
#include "rh850g4/ldl_dot_hu_ind_reg1_reg3_instr.h"
#include "rh850g4/ldl_dot_w_ind_reg1_reg3_instr.h"
#include "rh850g4/loop_reg1_disp16_instr.h"
#include "rh850g4/popsp_rh_dec_rt_instr.h"
#include "rh850g4/pushsp_rh_dec_rt_instr.h"
#include "rh850g4/rotl_imm5_reg2_reg3_instr.h"
#include "rh850g4/rotl_reg1_reg2_reg3_instr.h"
#include "rh850g4/snooze_instr.h"
#include "rh850g4/st_dot_dw_reg3_disp23_ind_reg1_instr.h"
#include "rh850g4/stc_dot_b_reg3_ind_reg1_instr.h"
#include "rh850g4/stc_dot_h_reg3_ind_reg1_instr.h"
#include "rh850g4/stc_dot_w_reg3_ind_reg1_instr.h"
#include "rh850g4/synci_instr.h"
#include "rh850g4/roundf_dot_dl_reg2_reg3_instr.h"
#include "rh850g4/roundf_dot_dul_reg2_reg3_instr.h"
#include "rh850g4/roundf_dot_duw_reg2_reg3_instr.h"
#include "rh850g4/roundf_dot_dw_reg2_reg3_instr.h"
#include "rh850g4/roundf_dot_sl_reg2_reg3_instr.h"
#include "rh850g4/roundf_dot_sul_reg2_reg3_instr.h"
#include "rh850g4/roundf_dot_suw_reg2_reg3_instr.h"
#include "rh850g4/roundf_dot_sw_reg2_reg3_instr.h"
#include "rh850g4/cache_cacheop_ind_reg1_instr.h"
#include "rh850g4/pref_prefop_ind_reg1_instr.h"
#include "rh850g4/dbcp_instr.h"
#include "rh850g4/dbpush_rh_dec_rt_instr.h"
#include "rh850g4/dbret_instr.h"
#include "rh850g4/dbtag_imm10_instr.h"
#include "rh850g4/dbtrap_instr.h"
#include "rh850g4/cvtf_dot_hs_reg2_reg3_instr.h"
#include "rh850g4/cvtf_dot_sh_reg2_reg3_instr.h"
#include "rh850g4/fmaf_dot_s_reg1_reg2_reg3_instr.h"
#include "rh850g4/fmsf_dot_s_reg1_reg2_reg3_instr.h"
#include "rh850g4/fnmaf_dot_s_reg1_reg2_reg3_instr.h"
#include "rh850g4/fnmsf_dot_s_reg1_reg2_reg3_instr.h"
#include "rh850g4/cll_instr.h"
#include "rh850g4/resbank_instr.h"
#include "rh850g4/clip_dot_b_reg1_reg2_instr.h"
#include "rh850g4/clip_dot_bu_reg1_reg2_instr.h"
#include "rh850g4/clip_dot_h_reg1_reg2_instr.h"
#include "rh850g4/clip_dot_hu_reg1_reg2_instr.h"
#include "rh850g4/ldv_dot_w_imm4_disp16_ind_reg1_wreg3_instr.h"
#include "rh850g4/stv_dot_w_imm2_wreg3_disp16_ind_reg1_instr.h"
#include "rh850g4/ldv_dot_qw_disp16_ind_reg1_wreg3_instr.h"
#include "rh850g4/stv_dot_qw_wreg3_disp16_ind_reg1_instr.h"
#include "rh850g4/ldv_dot_dw_imm2_disp16_ind_reg1_wreg3_instr.h"
#include "rh850g4/stv_dot_dw_imm1_wreg3_disp16_ind_reg1_instr.h"
#include "rh850g4/ldvz_dot_h4_disp16_ind_reg1_wreg3_instr.h"
#include "rh850g4/stvz_dot_h4_wreg3_disp16_ind_reg1_instr.h"
#include "rh850g4/cmovf_dot_w4_wreg4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/shflv_dot_w4_imm12_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/fmaf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/fmsf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/fnmaf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/fnmsf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/cmpf_dot_s4_fcond_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/roundf_dot_sw4_wreg2_wreg3_instr.h"
#include "rh850g4/roundf_dot_suw4_wreg2_wreg3_instr.h"
#include "rh850g4/trncf_dot_sw4_wreg2_wreg3_instr.h"
#include "rh850g4/trncf_dot_suw4_wreg2_wreg3_instr.h"
#include "rh850g4/ceilf_dot_sw4_wreg2_wreg3_instr.h"
#include "rh850g4/ceilf_dot_suw4_wreg2_wreg3_instr.h"
#include "rh850g4/floorf_dot_sw4_wreg2_wreg3_instr.h"
#include "rh850g4/floorf_dot_suw4_wreg2_wreg3_instr.h"
#include "rh850g4/cvtf_dot_sw4_wreg2_wreg3_instr.h"
#include "rh850g4/cvtf_dot_suw4_wreg2_wreg3_instr.h"
#include "rh850g4/cvtf_dot_ws4_wreg2_wreg3_instr.h"
#include "rh850g4/cvtf_dot_uws4_wreg2_wreg3_instr.h"
#include "rh850g4/cvtf_dot_hs4_wreg2_wreg3_instr.h"
#include "rh850g4/cvtf_dot_sh4_wreg2_wreg3_instr.h"
#include "rh850g4/absf_dot_s4_wreg2_wreg3_instr.h"
#include "rh850g4/negf_dot_s4_wreg2_wreg3_instr.h"
#include "rh850g4/sqrtf_dot_s4_wreg2_wreg3_instr.h"
#include "rh850g4/recipf_dot_s4_wreg2_wreg3_instr.h"
#include "rh850g4/rsqrtf_dot_s4_wreg2_wreg3_instr.h"
#include "rh850g4/flpv_dot_s4_imm2_wreg2_wreg3_instr.h"
#include "rh850g4/movv_dot_w4_wreg2_wreg3_instr.h"
#include "rh850g4/trfsrv_dot_w4_imm3_wreg2_instr.h"
#include "rh850g4/addf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/subf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/mulf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/maxf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/minf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/divf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/addrf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/subrf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/mulrf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/maxrf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/minrf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/addxf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/subxf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/mulxf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/addsubf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/subaddf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/addsubxf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/subaddxf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/addsubnf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/subaddnf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/addsubnxf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/subaddnxf_dot_s4_wreg1_wreg2_wreg3_instr.h"
#include "rh850g4/hvtrap_vector_instr.h"
#include "rh850g4/stm_dot_gsr_ind_reg1_instr.h"
#include "rh850g4/stm_dot_mp_eh_dec_et_ind_reg1_instr.h"
#include "rh850g4/ldm_dot_gsr_ind_reg1_instr.h"
#include "rh850g4/ldm_dot_mp_ind_reg1_eh_dec_et_instr.h"

//------------------------------------------------------------------------------
// rh850g4 instructions definition
//------------------------------------------------------------------------------

rh850g4::instr_set_impl_t::instr_set_impl_t
(
  fastiss::code_gen_if_t *code_gen_impl,
  rh850g4::memory_if_t *memory_impl,
  rh850g4::operand_if_t *operand_impl,
  rh850g4::cpu_state_if_t *cpu_state_impl,
  rh850g4::custom_if_t *custom_impl
)
{
  m_add_reg1_reg2 = new rh850g4::add_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_add_imm5_reg2 = new rh850g4::add_imm5_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_addi_imm16_reg1_reg2 = new rh850g4::addi_imm16_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_adf_cccc_reg1_reg2_reg3 = new rh850g4::adf_cccc_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_and_reg1_reg2 = new rh850g4::and_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_andi_imm16_reg1_reg2 = new rh850g4::andi_imm16_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_b_cccc_disp9 = new rh850g4::b_cccc_disp9_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_bsh_reg2_reg3 = new rh850g4::bsh_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_bsw_reg2_reg3 = new rh850g4::bsw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_callt_imm6 = new rh850g4::callt_imm6_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_caxi_ind_reg1_reg2_reg3 = new rh850g4::caxi_ind_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_clr1_bit_3_disp16_ind_reg1 = new rh850g4::clr1_bit_3_disp16_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_clr1_reg2_ind_reg1 = new rh850g4::clr1_reg2_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cmov_cccc_reg1_reg2_reg3 = new rh850g4::cmov_cccc_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cmov_cccc_imm5_reg2_reg3 = new rh850g4::cmov_cccc_imm5_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cmp_reg1_reg2 = new rh850g4::cmp_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cmp_imm5_reg2 = new rh850g4::cmp_imm5_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ctret = new rh850g4::ctret_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_di = new rh850g4::di_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_dispose_imm5_list12 = new rh850g4::dispose_imm5_list12_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_dispose_imm5_list12_ind_reg1 = new rh850g4::dispose_imm5_list12_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_div_reg1_reg2_reg3 = new rh850g4::div_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_switch_reg1 = new rh850g4::switch_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_divh_reg1_reg2 = new rh850g4::divh_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_divh_reg1_reg2_reg3 = new rh850g4::divh_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_divhu_reg1_reg2_reg3 = new rh850g4::divhu_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_divq_reg1_reg2_reg3 = new rh850g4::divq_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_divqu_reg1_reg2_reg3 = new rh850g4::divqu_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_divu_reg1_reg2_reg3 = new rh850g4::divu_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ei = new rh850g4::ei_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_eiret = new rh850g4::eiret_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_feret = new rh850g4::feret_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_fetrap_vector = new rh850g4::fetrap_vector_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_halt = new rh850g4::halt_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_hsh_reg2_reg3 = new rh850g4::hsh_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_hsw_reg2_reg3 = new rh850g4::hsw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_jr_disp22 = new rh850g4::jr_disp22_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_jr_disp32 = new rh850g4::jr_disp32_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_jarl_disp22_reg2 = new rh850g4::jarl_disp22_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_jarl_disp32_reg1 = new rh850g4::jarl_disp32_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_jmp_ind_reg1 = new rh850g4::jmp_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_jmp_disp32_ind_reg1 = new rh850g4::jmp_disp32_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_prepare_list12_imm5 = new rh850g4::prepare_list12_imm5_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_prepare_list12_imm5_bits_ff = new rh850g4::prepare_list12_imm5_bits_ff_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_b_disp16_ind_reg1_reg2 = new rh850g4::ld_dot_b_disp16_ind_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_b_disp23_ind_reg1_reg3 = new rh850g4::ld_dot_b_disp23_ind_reg1_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_b_ind_reg1_inc_reg3 = new rh850g4::ld_dot_b_ind_reg1_inc_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_b_ind_reg1_dec_reg3 = new rh850g4::ld_dot_b_ind_reg1_dec_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_bu_disp23_ind_reg1_reg3 = new rh850g4::ld_dot_bu_disp23_ind_reg1_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_bu_disp16_ind_reg1_reg2 = new rh850g4::ld_dot_bu_disp16_ind_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_bu_ind_reg1_inc_reg3 = new rh850g4::ld_dot_bu_ind_reg1_inc_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_bu_ind_reg1_dec_reg3 = new rh850g4::ld_dot_bu_ind_reg1_dec_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_h_disp16_ind_reg1_reg2 = new rh850g4::ld_dot_h_disp16_ind_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_h_disp23_ind_reg1_reg3 = new rh850g4::ld_dot_h_disp23_ind_reg1_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_h_ind_reg1_inc_reg3 = new rh850g4::ld_dot_h_ind_reg1_inc_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_h_ind_reg1_dec_reg3 = new rh850g4::ld_dot_h_ind_reg1_dec_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_hu_disp16_ind_reg1_reg2 = new rh850g4::ld_dot_hu_disp16_ind_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_hu_disp23_ind_reg1_reg3 = new rh850g4::ld_dot_hu_disp23_ind_reg1_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_hu_ind_reg1_inc_reg3 = new rh850g4::ld_dot_hu_ind_reg1_inc_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_hu_ind_reg1_dec_reg3 = new rh850g4::ld_dot_hu_ind_reg1_dec_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_w_disp16_ind_reg1_reg2 = new rh850g4::ld_dot_w_disp16_ind_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_w_disp23_ind_reg1_reg3 = new rh850g4::ld_dot_w_disp23_ind_reg1_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_w_ind_reg1_inc_reg3 = new rh850g4::ld_dot_w_ind_reg1_inc_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_w_ind_reg1_dec_reg3 = new rh850g4::ld_dot_w_ind_reg1_dec_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ldsr_reg2_regid_selid = new rh850g4::ldsr_reg2_regid_selid_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mac_reg1_reg2_reg3_reg4 = new rh850g4::mac_reg1_reg2_reg3_reg4_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_macu_reg1_reg2_reg3_reg4 = new rh850g4::macu_reg1_reg2_reg3_reg4_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_nop = new rh850g4::nop_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mov_reg1_reg2 = new rh850g4::mov_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mov_imm5_reg2 = new rh850g4::mov_imm5_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mov_imm32_reg1 = new rh850g4::mov_imm32_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_movea_imm16_reg1_reg2 = new rh850g4::movea_imm16_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_movhi_imm16_reg1_reg2 = new rh850g4::movhi_imm16_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mul_reg1_reg2_reg3 = new rh850g4::mul_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sxh_reg1 = new rh850g4::sxh_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mul_imm9_reg2_reg3 = new rh850g4::mul_imm9_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mulh_reg1_reg2 = new rh850g4::mulh_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mulh_imm5_reg2 = new rh850g4::mulh_imm5_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mulhi_imm16_reg1_reg2 = new rh850g4::mulhi_imm16_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mulu_reg1_reg2_reg3 = new rh850g4::mulu_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mulu_imm9_reg2_reg3 = new rh850g4::mulu_imm9_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_not_reg1_reg2 = new rh850g4::not_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_not1_bit_3_disp16_ind_reg1 = new rh850g4::not1_bit_3_disp16_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_not1_reg2_ind_reg1 = new rh850g4::not1_reg2_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_or_reg1_reg2 = new rh850g4::or_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ori_imm16_reg1_reg2 = new rh850g4::ori_imm16_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_rie = new rh850g4::rie_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_rie_imm5_imm4 = new rh850g4::rie_imm5_imm4_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sar_reg1_reg2 = new rh850g4::sar_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sar_imm5_reg2 = new rh850g4::sar_imm5_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sar_reg1_reg2_reg3 = new rh850g4::sar_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sasf_cccc_reg2 = new rh850g4::sasf_cccc_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sxb_reg1 = new rh850g4::sxb_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_zxb_reg1 = new rh850g4::zxb_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_zxh_reg1 = new rh850g4::zxh_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_satadd_reg1_reg2 = new rh850g4::satadd_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_satadd_imm5_reg2 = new rh850g4::satadd_imm5_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_satadd_reg1_reg2_reg3 = new rh850g4::satadd_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_satsub_reg1_reg2 = new rh850g4::satsub_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_satsub_reg1_reg2_reg3 = new rh850g4::satsub_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_satsubi_imm16_reg1_reg2 = new rh850g4::satsubi_imm16_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_satsubr_reg1_reg2 = new rh850g4::satsubr_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sbf_cccc_reg1_reg2_reg3 = new rh850g4::sbf_cccc_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sch0l_reg2_reg3 = new rh850g4::sch0l_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sch0r_reg2_reg3 = new rh850g4::sch0r_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sch1l_reg2_reg3 = new rh850g4::sch1l_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sch1r_reg2_reg3 = new rh850g4::sch1r_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_set1_bit_3_disp16_ind_reg1 = new rh850g4::set1_bit_3_disp16_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_set1_reg2_ind_reg1 = new rh850g4::set1_reg2_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_setf_cccc_reg2 = new rh850g4::setf_cccc_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_shl_reg1_reg2 = new rh850g4::shl_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_shl_imm5_reg2 = new rh850g4::shl_imm5_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_shl_reg1_reg2_reg3 = new rh850g4::shl_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_shr_reg1_reg2 = new rh850g4::shr_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_shr_imm5_reg2 = new rh850g4::shr_imm5_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_shr_reg1_reg2_reg3 = new rh850g4::shr_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sld_dot_b_disp7_ind_ep_reg2 = new rh850g4::sld_dot_b_disp7_ind_ep_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sld_dot_bu_disp4_ind_ep_reg2 = new rh850g4::sld_dot_bu_disp4_ind_ep_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sld_dot_h_disp8_ind_ep_reg2 = new rh850g4::sld_dot_h_disp8_ind_ep_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sld_dot_hu_disp5_ind_ep_reg2 = new rh850g4::sld_dot_hu_disp5_ind_ep_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sld_dot_w_disp8_ind_ep_reg2 = new rh850g4::sld_dot_w_disp8_ind_ep_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sst_dot_b_reg2_disp7_ind_ep = new rh850g4::sst_dot_b_reg2_disp7_ind_ep_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sst_dot_h_reg2_disp8_ind_ep = new rh850g4::sst_dot_h_reg2_disp8_ind_ep_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sst_dot_w_reg2_disp8_ind_ep = new rh850g4::sst_dot_w_reg2_disp8_ind_ep_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_st_dot_b_reg2_disp16_ind_reg1 = new rh850g4::st_dot_b_reg2_disp16_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_st_dot_b_reg3_disp23_ind_reg1 = new rh850g4::st_dot_b_reg3_disp23_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_st_dot_b_reg3_ind_reg1_inc = new rh850g4::st_dot_b_reg3_ind_reg1_inc_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_st_dot_b_reg3_ind_reg1_dec = new rh850g4::st_dot_b_reg3_ind_reg1_dec_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_st_dot_h_reg2_disp16_ind_reg1 = new rh850g4::st_dot_h_reg2_disp16_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_st_dot_h_reg3_disp23_ind_reg1 = new rh850g4::st_dot_h_reg3_disp23_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_st_dot_h_reg3_ind_reg1_inc = new rh850g4::st_dot_h_reg3_ind_reg1_inc_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_st_dot_h_reg3_ind_reg1_dec = new rh850g4::st_dot_h_reg3_ind_reg1_dec_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_st_dot_w_reg2_disp16_ind_reg1 = new rh850g4::st_dot_w_reg2_disp16_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_st_dot_w_reg3_disp23_ind_reg1 = new rh850g4::st_dot_w_reg3_disp23_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_st_dot_w_reg3_ind_reg1_inc = new rh850g4::st_dot_w_reg3_ind_reg1_inc_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_st_dot_w_reg3_ind_reg1_dec = new rh850g4::st_dot_w_reg3_ind_reg1_dec_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_stsr_regid_reg2_selid = new rh850g4::stsr_regid_reg2_selid_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sub_reg1_reg2 = new rh850g4::sub_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_subr_reg1_reg2 = new rh850g4::subr_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_synce = new rh850g4::synce_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_syncm = new rh850g4::syncm_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_syncp = new rh850g4::syncp_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_syscall_vector = new rh850g4::syscall_vector_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_trap_vector = new rh850g4::trap_vector_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_tst_reg1_reg2 = new rh850g4::tst_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_tst1_bit_3_disp16_ind_reg1 = new rh850g4::tst1_bit_3_disp16_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_tst1_reg2_ind_reg1 = new rh850g4::tst1_reg2_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_xor_reg1_reg2 = new rh850g4::xor_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_xori_imm16_reg1_reg2 = new rh850g4::xori_imm16_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_absf_dot_d_reg2_reg3 = new rh850g4::absf_dot_d_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_absf_dot_s_reg2_reg3 = new rh850g4::absf_dot_s_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_addf_dot_d_reg1_reg2_reg3 = new rh850g4::addf_dot_d_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_addf_dot_s_reg1_reg2_reg3 = new rh850g4::addf_dot_s_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ceilf_dot_dl_reg2_reg3 = new rh850g4::ceilf_dot_dl_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ceilf_dot_dul_reg2_reg3 = new rh850g4::ceilf_dot_dul_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ceilf_dot_duw_reg2_reg3 = new rh850g4::ceilf_dot_duw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ceilf_dot_dw_reg2_reg3 = new rh850g4::ceilf_dot_dw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ceilf_dot_sl_reg2_reg3 = new rh850g4::ceilf_dot_sl_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ceilf_dot_sul_reg2_reg3 = new rh850g4::ceilf_dot_sul_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ceilf_dot_suw_reg2_reg3 = new rh850g4::ceilf_dot_suw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ceilf_dot_sw_reg2_reg3 = new rh850g4::ceilf_dot_sw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cmovf_dot_d_fcbit_reg1_reg2_reg3 = new rh850g4::cmovf_dot_d_fcbit_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cmovf_dot_s_fcbit_reg1_reg2_reg3 = new rh850g4::cmovf_dot_s_fcbit_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cmpf_dot_d_fcond_reg2_reg1_fcbit = new rh850g4::cmpf_dot_d_fcond_reg2_reg1_fcbit_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cmpf_dot_s_fcond_reg2_reg1_fcbit = new rh850g4::cmpf_dot_s_fcond_reg2_reg1_fcbit_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_dl_reg2_reg3 = new rh850g4::cvtf_dot_dl_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_ds_reg2_reg3 = new rh850g4::cvtf_dot_ds_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_dul_reg2_reg3 = new rh850g4::cvtf_dot_dul_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_duw_reg2_reg3 = new rh850g4::cvtf_dot_duw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_dw_reg2_reg3 = new rh850g4::cvtf_dot_dw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_ld_reg2_reg3 = new rh850g4::cvtf_dot_ld_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_ls_reg2_reg3 = new rh850g4::cvtf_dot_ls_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_sd_reg2_reg3 = new rh850g4::cvtf_dot_sd_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_sl_reg2_reg3 = new rh850g4::cvtf_dot_sl_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_sul_reg2_reg3 = new rh850g4::cvtf_dot_sul_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_suw_reg2_reg3 = new rh850g4::cvtf_dot_suw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_sw_reg2_reg3 = new rh850g4::cvtf_dot_sw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_uld_reg2_reg3 = new rh850g4::cvtf_dot_uld_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_uls_reg2_reg3 = new rh850g4::cvtf_dot_uls_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_uwd_reg2_reg3 = new rh850g4::cvtf_dot_uwd_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_uws_reg2_reg3 = new rh850g4::cvtf_dot_uws_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_wd_reg2_reg3 = new rh850g4::cvtf_dot_wd_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_ws_reg2_reg3 = new rh850g4::cvtf_dot_ws_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_divf_dot_d_reg1_reg2_reg3 = new rh850g4::divf_dot_d_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_divf_dot_s_reg1_reg2_reg3 = new rh850g4::divf_dot_s_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_floorf_dot_dl_reg2_reg3 = new rh850g4::floorf_dot_dl_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_floorf_dot_dul_reg2_reg3 = new rh850g4::floorf_dot_dul_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_floorf_dot_duw_reg2_reg3 = new rh850g4::floorf_dot_duw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_floorf_dot_dw_reg2_reg3 = new rh850g4::floorf_dot_dw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_floorf_dot_sl_reg2_reg3 = new rh850g4::floorf_dot_sl_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_floorf_dot_sul_reg2_reg3 = new rh850g4::floorf_dot_sul_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_floorf_dot_suw_reg2_reg3 = new rh850g4::floorf_dot_suw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_floorf_dot_sw_reg2_reg3 = new rh850g4::floorf_dot_sw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_maddf_dot_s_reg1_reg2_reg3_reg4 = new rh850g4::maddf_dot_s_reg1_reg2_reg3_reg4_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_maxf_dot_d_reg1_reg2_reg3 = new rh850g4::maxf_dot_d_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_maxf_dot_s_reg1_reg2_reg3 = new rh850g4::maxf_dot_s_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_minf_dot_d_reg1_reg2_reg3 = new rh850g4::minf_dot_d_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_minf_dot_s_reg1_reg2_reg3 = new rh850g4::minf_dot_s_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_msubf_dot_s_reg1_reg2_reg3_reg4 = new rh850g4::msubf_dot_s_reg1_reg2_reg3_reg4_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mulf_dot_d_reg1_reg2_reg3 = new rh850g4::mulf_dot_d_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mulf_dot_s_reg1_reg2_reg3 = new rh850g4::mulf_dot_s_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_negf_dot_d_reg2_reg3 = new rh850g4::negf_dot_d_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_negf_dot_s_reg2_reg3 = new rh850g4::negf_dot_s_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_nmaddf_dot_s_reg1_reg2_reg3_reg4 = new rh850g4::nmaddf_dot_s_reg1_reg2_reg3_reg4_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_nmsubf_dot_s_reg1_reg2_reg3_reg4 = new rh850g4::nmsubf_dot_s_reg1_reg2_reg3_reg4_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_recipf_dot_d_reg2_reg3 = new rh850g4::recipf_dot_d_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_recipf_dot_s_reg2_reg3 = new rh850g4::recipf_dot_s_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_rsqrtf_dot_d_reg2_reg3 = new rh850g4::rsqrtf_dot_d_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_rsqrtf_dot_s_reg2_reg3 = new rh850g4::rsqrtf_dot_s_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sqrtf_dot_d_reg2_reg3 = new rh850g4::sqrtf_dot_d_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sqrtf_dot_s_reg2_reg3 = new rh850g4::sqrtf_dot_s_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_subf_dot_d_reg1_reg2_reg3 = new rh850g4::subf_dot_d_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_subf_dot_s_reg1_reg2_reg3 = new rh850g4::subf_dot_s_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_trfsr_fcbit = new rh850g4::trfsr_fcbit_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_trncf_dot_dl_reg2_reg3 = new rh850g4::trncf_dot_dl_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_trncf_dot_dul_reg2_reg3 = new rh850g4::trncf_dot_dul_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_trncf_dot_duw_reg2_reg3 = new rh850g4::trncf_dot_duw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_trncf_dot_dw_reg2_reg3 = new rh850g4::trncf_dot_dw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_trncf_dot_sl_reg2_reg3 = new rh850g4::trncf_dot_sl_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_trncf_dot_sul_reg2_reg3 = new rh850g4::trncf_dot_sul_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_trncf_dot_suw_reg2_reg3 = new rh850g4::trncf_dot_suw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_trncf_dot_sw_reg2_reg3 = new rh850g4::trncf_dot_sw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_b_cccc_disp17 = new rh850g4::b_cccc_disp17_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_binsu_reg1_lsb_msb_reg2 = new rh850g4::binsu_reg1_lsb_msb_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_binsm_reg1_lsb_msb_reg2 = new rh850g4::binsm_reg1_lsb_msb_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_binsl_reg1_lsb_msb_reg2 = new rh850g4::binsl_reg1_lsb_msb_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_jarl_ind_reg1_reg3 = new rh850g4::jarl_ind_reg1_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_dw_disp23_ind_reg1_reg3 = new rh850g4::ld_dot_dw_disp23_ind_reg1_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ldl_dot_bu_ind_reg1_reg3 = new rh850g4::ldl_dot_bu_ind_reg1_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ldl_dot_hu_ind_reg1_reg3 = new rh850g4::ldl_dot_hu_ind_reg1_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ldl_dot_w_ind_reg1_reg3 = new rh850g4::ldl_dot_w_ind_reg1_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_loop_reg1_disp16 = new rh850g4::loop_reg1_disp16_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_popsp_rh_dec_rt = new rh850g4::popsp_rh_dec_rt_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_pushsp_rh_dec_rt = new rh850g4::pushsp_rh_dec_rt_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_rotl_imm5_reg2_reg3 = new rh850g4::rotl_imm5_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_rotl_reg1_reg2_reg3 = new rh850g4::rotl_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_snooze = new rh850g4::snooze_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_st_dot_dw_reg3_disp23_ind_reg1 = new rh850g4::st_dot_dw_reg3_disp23_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_stc_dot_b_reg3_ind_reg1 = new rh850g4::stc_dot_b_reg3_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_stc_dot_h_reg3_ind_reg1 = new rh850g4::stc_dot_h_reg3_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_stc_dot_w_reg3_ind_reg1 = new rh850g4::stc_dot_w_reg3_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_synci = new rh850g4::synci_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_roundf_dot_dl_reg2_reg3 = new rh850g4::roundf_dot_dl_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_roundf_dot_dul_reg2_reg3 = new rh850g4::roundf_dot_dul_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_roundf_dot_duw_reg2_reg3 = new rh850g4::roundf_dot_duw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_roundf_dot_dw_reg2_reg3 = new rh850g4::roundf_dot_dw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_roundf_dot_sl_reg2_reg3 = new rh850g4::roundf_dot_sl_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_roundf_dot_sul_reg2_reg3 = new rh850g4::roundf_dot_sul_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_roundf_dot_suw_reg2_reg3 = new rh850g4::roundf_dot_suw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_roundf_dot_sw_reg2_reg3 = new rh850g4::roundf_dot_sw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cache_cacheop_ind_reg1 = new rh850g4::cache_cacheop_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_pref_prefop_ind_reg1 = new rh850g4::pref_prefop_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_dbcp = new rh850g4::dbcp_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_dbpush_rh_dec_rt = new rh850g4::dbpush_rh_dec_rt_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_dbret = new rh850g4::dbret_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_dbtag_imm10 = new rh850g4::dbtag_imm10_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_dbtrap = new rh850g4::dbtrap_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_hs_reg2_reg3 = new rh850g4::cvtf_dot_hs_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_sh_reg2_reg3 = new rh850g4::cvtf_dot_sh_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_fmaf_dot_s_reg1_reg2_reg3 = new rh850g4::fmaf_dot_s_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_fmsf_dot_s_reg1_reg2_reg3 = new rh850g4::fmsf_dot_s_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_fnmaf_dot_s_reg1_reg2_reg3 = new rh850g4::fnmaf_dot_s_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_fnmsf_dot_s_reg1_reg2_reg3 = new rh850g4::fnmsf_dot_s_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cll = new rh850g4::cll_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_resbank = new rh850g4::resbank_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_clip_dot_b_reg1_reg2 = new rh850g4::clip_dot_b_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_clip_dot_bu_reg1_reg2 = new rh850g4::clip_dot_bu_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_clip_dot_h_reg1_reg2 = new rh850g4::clip_dot_h_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_clip_dot_hu_reg1_reg2 = new rh850g4::clip_dot_hu_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ldv_dot_w_imm4_disp16_ind_reg1_wreg3 = new rh850g4::ldv_dot_w_imm4_disp16_ind_reg1_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_stv_dot_w_imm2_wreg3_disp16_ind_reg1 = new rh850g4::stv_dot_w_imm2_wreg3_disp16_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ldv_dot_qw_disp16_ind_reg1_wreg3 = new rh850g4::ldv_dot_qw_disp16_ind_reg1_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_stv_dot_qw_wreg3_disp16_ind_reg1 = new rh850g4::stv_dot_qw_wreg3_disp16_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ldv_dot_dw_imm2_disp16_ind_reg1_wreg3 = new rh850g4::ldv_dot_dw_imm2_disp16_ind_reg1_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_stv_dot_dw_imm1_wreg3_disp16_ind_reg1 = new rh850g4::stv_dot_dw_imm1_wreg3_disp16_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ldvz_dot_h4_disp16_ind_reg1_wreg3 = new rh850g4::ldvz_dot_h4_disp16_ind_reg1_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_stvz_dot_h4_wreg3_disp16_ind_reg1 = new rh850g4::stvz_dot_h4_wreg3_disp16_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cmovf_dot_w4_wreg4_wreg1_wreg2_wreg3 = new rh850g4::cmovf_dot_w4_wreg4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_shflv_dot_w4_imm12_wreg1_wreg2_wreg3 = new rh850g4::shflv_dot_w4_imm12_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_fmaf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::fmaf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_fmsf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::fmsf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_fnmaf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::fnmaf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_fnmsf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::fnmsf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cmpf_dot_s4_fcond_wreg1_wreg2_wreg3 = new rh850g4::cmpf_dot_s4_fcond_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_roundf_dot_sw4_wreg2_wreg3 = new rh850g4::roundf_dot_sw4_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_roundf_dot_suw4_wreg2_wreg3 = new rh850g4::roundf_dot_suw4_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_trncf_dot_sw4_wreg2_wreg3 = new rh850g4::trncf_dot_sw4_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_trncf_dot_suw4_wreg2_wreg3 = new rh850g4::trncf_dot_suw4_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ceilf_dot_sw4_wreg2_wreg3 = new rh850g4::ceilf_dot_sw4_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ceilf_dot_suw4_wreg2_wreg3 = new rh850g4::ceilf_dot_suw4_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_floorf_dot_sw4_wreg2_wreg3 = new rh850g4::floorf_dot_sw4_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_floorf_dot_suw4_wreg2_wreg3 = new rh850g4::floorf_dot_suw4_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_sw4_wreg2_wreg3 = new rh850g4::cvtf_dot_sw4_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_suw4_wreg2_wreg3 = new rh850g4::cvtf_dot_suw4_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_ws4_wreg2_wreg3 = new rh850g4::cvtf_dot_ws4_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_uws4_wreg2_wreg3 = new rh850g4::cvtf_dot_uws4_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_hs4_wreg2_wreg3 = new rh850g4::cvtf_dot_hs4_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cvtf_dot_sh4_wreg2_wreg3 = new rh850g4::cvtf_dot_sh4_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_absf_dot_s4_wreg2_wreg3 = new rh850g4::absf_dot_s4_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_negf_dot_s4_wreg2_wreg3 = new rh850g4::negf_dot_s4_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sqrtf_dot_s4_wreg2_wreg3 = new rh850g4::sqrtf_dot_s4_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_recipf_dot_s4_wreg2_wreg3 = new rh850g4::recipf_dot_s4_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_rsqrtf_dot_s4_wreg2_wreg3 = new rh850g4::rsqrtf_dot_s4_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_flpv_dot_s4_imm2_wreg2_wreg3 = new rh850g4::flpv_dot_s4_imm2_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_movv_dot_w4_wreg2_wreg3 = new rh850g4::movv_dot_w4_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_trfsrv_dot_w4_imm3_wreg2 = new rh850g4::trfsrv_dot_w4_imm3_wreg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_addf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::addf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_subf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::subf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mulf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::mulf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_maxf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::maxf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_minf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::minf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_divf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::divf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_addrf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::addrf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_subrf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::subrf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mulrf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::mulrf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_maxrf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::maxrf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_minrf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::minrf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_addxf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::addxf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_subxf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::subxf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mulxf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::mulxf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_addsubf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::addsubf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_subaddf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::subaddf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_addsubxf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::addsubxf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_subaddxf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::subaddxf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_addsubnf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::addsubnf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_subaddnf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::subaddnf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_addsubnxf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::addsubnxf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_subaddnxf_dot_s4_wreg1_wreg2_wreg3 = new rh850g4::subaddnxf_dot_s4_wreg1_wreg2_wreg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_hvtrap_vector = new rh850g4::hvtrap_vector_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_stm_dot_gsr_ind_reg1 = new rh850g4::stm_dot_gsr_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_stm_dot_mp_eh_dec_et_ind_reg1 = new rh850g4::stm_dot_mp_eh_dec_et_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ldm_dot_gsr_ind_reg1 = new rh850g4::ldm_dot_gsr_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ldm_dot_mp_ind_reg1_eh_dec_et = new rh850g4::ldm_dot_mp_ind_reg1_eh_dec_et_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
}

rh850g4::instr_set_impl_t::~instr_set_impl_t()
{
  delete m_add_reg1_reg2;
  delete m_add_imm5_reg2;
  delete m_addi_imm16_reg1_reg2;
  delete m_adf_cccc_reg1_reg2_reg3;
  delete m_and_reg1_reg2;
  delete m_andi_imm16_reg1_reg2;
  delete m_b_cccc_disp9;
  delete m_bsh_reg2_reg3;
  delete m_bsw_reg2_reg3;
  delete m_callt_imm6;
  delete m_caxi_ind_reg1_reg2_reg3;
  delete m_clr1_bit_3_disp16_ind_reg1;
  delete m_clr1_reg2_ind_reg1;
  delete m_cmov_cccc_reg1_reg2_reg3;
  delete m_cmov_cccc_imm5_reg2_reg3;
  delete m_cmp_reg1_reg2;
  delete m_cmp_imm5_reg2;
  delete m_ctret;
  delete m_di;
  delete m_dispose_imm5_list12;
  delete m_dispose_imm5_list12_ind_reg1;
  delete m_div_reg1_reg2_reg3;
  delete m_switch_reg1;
  delete m_divh_reg1_reg2;
  delete m_divh_reg1_reg2_reg3;
  delete m_divhu_reg1_reg2_reg3;
  delete m_divq_reg1_reg2_reg3;
  delete m_divqu_reg1_reg2_reg3;
  delete m_divu_reg1_reg2_reg3;
  delete m_ei;
  delete m_eiret;
  delete m_feret;
  delete m_fetrap_vector;
  delete m_halt;
  delete m_hsh_reg2_reg3;
  delete m_hsw_reg2_reg3;
  delete m_jr_disp22;
  delete m_jr_disp32;
  delete m_jarl_disp22_reg2;
  delete m_jarl_disp32_reg1;
  delete m_jmp_ind_reg1;
  delete m_jmp_disp32_ind_reg1;
  delete m_prepare_list12_imm5;
  delete m_prepare_list12_imm5_bits_ff;
  delete m_ld_dot_b_disp16_ind_reg1_reg2;
  delete m_ld_dot_b_disp23_ind_reg1_reg3;
  delete m_ld_dot_b_ind_reg1_inc_reg3;
  delete m_ld_dot_b_ind_reg1_dec_reg3;
  delete m_ld_dot_bu_disp23_ind_reg1_reg3;
  delete m_ld_dot_bu_disp16_ind_reg1_reg2;
  delete m_ld_dot_bu_ind_reg1_inc_reg3;
  delete m_ld_dot_bu_ind_reg1_dec_reg3;
  delete m_ld_dot_h_disp16_ind_reg1_reg2;
  delete m_ld_dot_h_disp23_ind_reg1_reg3;
  delete m_ld_dot_h_ind_reg1_inc_reg3;
  delete m_ld_dot_h_ind_reg1_dec_reg3;
  delete m_ld_dot_hu_disp16_ind_reg1_reg2;
  delete m_ld_dot_hu_disp23_ind_reg1_reg3;
  delete m_ld_dot_hu_ind_reg1_inc_reg3;
  delete m_ld_dot_hu_ind_reg1_dec_reg3;
  delete m_ld_dot_w_disp16_ind_reg1_reg2;
  delete m_ld_dot_w_disp23_ind_reg1_reg3;
  delete m_ld_dot_w_ind_reg1_inc_reg3;
  delete m_ld_dot_w_ind_reg1_dec_reg3;
  delete m_ldsr_reg2_regid_selid;
  delete m_mac_reg1_reg2_reg3_reg4;
  delete m_macu_reg1_reg2_reg3_reg4;
  delete m_nop;
  delete m_mov_reg1_reg2;
  delete m_mov_imm5_reg2;
  delete m_mov_imm32_reg1;
  delete m_movea_imm16_reg1_reg2;
  delete m_movhi_imm16_reg1_reg2;
  delete m_mul_reg1_reg2_reg3;
  delete m_sxh_reg1;
  delete m_mul_imm9_reg2_reg3;
  delete m_mulh_reg1_reg2;
  delete m_mulh_imm5_reg2;
  delete m_mulhi_imm16_reg1_reg2;
  delete m_mulu_reg1_reg2_reg3;
  delete m_mulu_imm9_reg2_reg3;
  delete m_not_reg1_reg2;
  delete m_not1_bit_3_disp16_ind_reg1;
  delete m_not1_reg2_ind_reg1;
  delete m_or_reg1_reg2;
  delete m_ori_imm16_reg1_reg2;
  delete m_rie;
  delete m_rie_imm5_imm4;
  delete m_sar_reg1_reg2;
  delete m_sar_imm5_reg2;
  delete m_sar_reg1_reg2_reg3;
  delete m_sasf_cccc_reg2;
  delete m_sxb_reg1;
  delete m_zxb_reg1;
  delete m_zxh_reg1;
  delete m_satadd_reg1_reg2;
  delete m_satadd_imm5_reg2;
  delete m_satadd_reg1_reg2_reg3;
  delete m_satsub_reg1_reg2;
  delete m_satsub_reg1_reg2_reg3;
  delete m_satsubi_imm16_reg1_reg2;
  delete m_satsubr_reg1_reg2;
  delete m_sbf_cccc_reg1_reg2_reg3;
  delete m_sch0l_reg2_reg3;
  delete m_sch0r_reg2_reg3;
  delete m_sch1l_reg2_reg3;
  delete m_sch1r_reg2_reg3;
  delete m_set1_bit_3_disp16_ind_reg1;
  delete m_set1_reg2_ind_reg1;
  delete m_setf_cccc_reg2;
  delete m_shl_reg1_reg2;
  delete m_shl_imm5_reg2;
  delete m_shl_reg1_reg2_reg3;
  delete m_shr_reg1_reg2;
  delete m_shr_imm5_reg2;
  delete m_shr_reg1_reg2_reg3;
  delete m_sld_dot_b_disp7_ind_ep_reg2;
  delete m_sld_dot_bu_disp4_ind_ep_reg2;
  delete m_sld_dot_h_disp8_ind_ep_reg2;
  delete m_sld_dot_hu_disp5_ind_ep_reg2;
  delete m_sld_dot_w_disp8_ind_ep_reg2;
  delete m_sst_dot_b_reg2_disp7_ind_ep;
  delete m_sst_dot_h_reg2_disp8_ind_ep;
  delete m_sst_dot_w_reg2_disp8_ind_ep;
  delete m_st_dot_b_reg2_disp16_ind_reg1;
  delete m_st_dot_b_reg3_disp23_ind_reg1;
  delete m_st_dot_b_reg3_ind_reg1_inc;
  delete m_st_dot_b_reg3_ind_reg1_dec;
  delete m_st_dot_h_reg2_disp16_ind_reg1;
  delete m_st_dot_h_reg3_disp23_ind_reg1;
  delete m_st_dot_h_reg3_ind_reg1_inc;
  delete m_st_dot_h_reg3_ind_reg1_dec;
  delete m_st_dot_w_reg2_disp16_ind_reg1;
  delete m_st_dot_w_reg3_disp23_ind_reg1;
  delete m_st_dot_w_reg3_ind_reg1_inc;
  delete m_st_dot_w_reg3_ind_reg1_dec;
  delete m_stsr_regid_reg2_selid;
  delete m_sub_reg1_reg2;
  delete m_subr_reg1_reg2;
  delete m_synce;
  delete m_syncm;
  delete m_syncp;
  delete m_syscall_vector;
  delete m_trap_vector;
  delete m_tst_reg1_reg2;
  delete m_tst1_bit_3_disp16_ind_reg1;
  delete m_tst1_reg2_ind_reg1;
  delete m_xor_reg1_reg2;
  delete m_xori_imm16_reg1_reg2;
  delete m_absf_dot_d_reg2_reg3;
  delete m_absf_dot_s_reg2_reg3;
  delete m_addf_dot_d_reg1_reg2_reg3;
  delete m_addf_dot_s_reg1_reg2_reg3;
  delete m_ceilf_dot_dl_reg2_reg3;
  delete m_ceilf_dot_dul_reg2_reg3;
  delete m_ceilf_dot_duw_reg2_reg3;
  delete m_ceilf_dot_dw_reg2_reg3;
  delete m_ceilf_dot_sl_reg2_reg3;
  delete m_ceilf_dot_sul_reg2_reg3;
  delete m_ceilf_dot_suw_reg2_reg3;
  delete m_ceilf_dot_sw_reg2_reg3;
  delete m_cmovf_dot_d_fcbit_reg1_reg2_reg3;
  delete m_cmovf_dot_s_fcbit_reg1_reg2_reg3;
  delete m_cmpf_dot_d_fcond_reg2_reg1_fcbit;
  delete m_cmpf_dot_s_fcond_reg2_reg1_fcbit;
  delete m_cvtf_dot_dl_reg2_reg3;
  delete m_cvtf_dot_ds_reg2_reg3;
  delete m_cvtf_dot_dul_reg2_reg3;
  delete m_cvtf_dot_duw_reg2_reg3;
  delete m_cvtf_dot_dw_reg2_reg3;
  delete m_cvtf_dot_ld_reg2_reg3;
  delete m_cvtf_dot_ls_reg2_reg3;
  delete m_cvtf_dot_sd_reg2_reg3;
  delete m_cvtf_dot_sl_reg2_reg3;
  delete m_cvtf_dot_sul_reg2_reg3;
  delete m_cvtf_dot_suw_reg2_reg3;
  delete m_cvtf_dot_sw_reg2_reg3;
  delete m_cvtf_dot_uld_reg2_reg3;
  delete m_cvtf_dot_uls_reg2_reg3;
  delete m_cvtf_dot_uwd_reg2_reg3;
  delete m_cvtf_dot_uws_reg2_reg3;
  delete m_cvtf_dot_wd_reg2_reg3;
  delete m_cvtf_dot_ws_reg2_reg3;
  delete m_divf_dot_d_reg1_reg2_reg3;
  delete m_divf_dot_s_reg1_reg2_reg3;
  delete m_floorf_dot_dl_reg2_reg3;
  delete m_floorf_dot_dul_reg2_reg3;
  delete m_floorf_dot_duw_reg2_reg3;
  delete m_floorf_dot_dw_reg2_reg3;
  delete m_floorf_dot_sl_reg2_reg3;
  delete m_floorf_dot_sul_reg2_reg3;
  delete m_floorf_dot_suw_reg2_reg3;
  delete m_floorf_dot_sw_reg2_reg3;
  delete m_maddf_dot_s_reg1_reg2_reg3_reg4;
  delete m_maxf_dot_d_reg1_reg2_reg3;
  delete m_maxf_dot_s_reg1_reg2_reg3;
  delete m_minf_dot_d_reg1_reg2_reg3;
  delete m_minf_dot_s_reg1_reg2_reg3;
  delete m_msubf_dot_s_reg1_reg2_reg3_reg4;
  delete m_mulf_dot_d_reg1_reg2_reg3;
  delete m_mulf_dot_s_reg1_reg2_reg3;
  delete m_negf_dot_d_reg2_reg3;
  delete m_negf_dot_s_reg2_reg3;
  delete m_nmaddf_dot_s_reg1_reg2_reg3_reg4;
  delete m_nmsubf_dot_s_reg1_reg2_reg3_reg4;
  delete m_recipf_dot_d_reg2_reg3;
  delete m_recipf_dot_s_reg2_reg3;
  delete m_rsqrtf_dot_d_reg2_reg3;
  delete m_rsqrtf_dot_s_reg2_reg3;
  delete m_sqrtf_dot_d_reg2_reg3;
  delete m_sqrtf_dot_s_reg2_reg3;
  delete m_subf_dot_d_reg1_reg2_reg3;
  delete m_subf_dot_s_reg1_reg2_reg3;
  delete m_trfsr_fcbit;
  delete m_trncf_dot_dl_reg2_reg3;
  delete m_trncf_dot_dul_reg2_reg3;
  delete m_trncf_dot_duw_reg2_reg3;
  delete m_trncf_dot_dw_reg2_reg3;
  delete m_trncf_dot_sl_reg2_reg3;
  delete m_trncf_dot_sul_reg2_reg3;
  delete m_trncf_dot_suw_reg2_reg3;
  delete m_trncf_dot_sw_reg2_reg3;
  delete m_b_cccc_disp17;
  delete m_binsu_reg1_lsb_msb_reg2;
  delete m_binsm_reg1_lsb_msb_reg2;
  delete m_binsl_reg1_lsb_msb_reg2;
  delete m_jarl_ind_reg1_reg3;
  delete m_ld_dot_dw_disp23_ind_reg1_reg3;
  delete m_ldl_dot_bu_ind_reg1_reg3;
  delete m_ldl_dot_hu_ind_reg1_reg3;
  delete m_ldl_dot_w_ind_reg1_reg3;
  delete m_loop_reg1_disp16;
  delete m_popsp_rh_dec_rt;
  delete m_pushsp_rh_dec_rt;
  delete m_rotl_imm5_reg2_reg3;
  delete m_rotl_reg1_reg2_reg3;
  delete m_snooze;
  delete m_st_dot_dw_reg3_disp23_ind_reg1;
  delete m_stc_dot_b_reg3_ind_reg1;
  delete m_stc_dot_h_reg3_ind_reg1;
  delete m_stc_dot_w_reg3_ind_reg1;
  delete m_synci;
  delete m_roundf_dot_dl_reg2_reg3;
  delete m_roundf_dot_dul_reg2_reg3;
  delete m_roundf_dot_duw_reg2_reg3;
  delete m_roundf_dot_dw_reg2_reg3;
  delete m_roundf_dot_sl_reg2_reg3;
  delete m_roundf_dot_sul_reg2_reg3;
  delete m_roundf_dot_suw_reg2_reg3;
  delete m_roundf_dot_sw_reg2_reg3;
  delete m_cache_cacheop_ind_reg1;
  delete m_pref_prefop_ind_reg1;
  delete m_dbcp;
  delete m_dbpush_rh_dec_rt;
  delete m_dbret;
  delete m_dbtag_imm10;
  delete m_dbtrap;
  delete m_cvtf_dot_hs_reg2_reg3;
  delete m_cvtf_dot_sh_reg2_reg3;
  delete m_fmaf_dot_s_reg1_reg2_reg3;
  delete m_fmsf_dot_s_reg1_reg2_reg3;
  delete m_fnmaf_dot_s_reg1_reg2_reg3;
  delete m_fnmsf_dot_s_reg1_reg2_reg3;
  delete m_cll;
  delete m_resbank;
  delete m_clip_dot_b_reg1_reg2;
  delete m_clip_dot_bu_reg1_reg2;
  delete m_clip_dot_h_reg1_reg2;
  delete m_clip_dot_hu_reg1_reg2;
  delete m_ldv_dot_w_imm4_disp16_ind_reg1_wreg3;
  delete m_stv_dot_w_imm2_wreg3_disp16_ind_reg1;
  delete m_ldv_dot_qw_disp16_ind_reg1_wreg3;
  delete m_stv_dot_qw_wreg3_disp16_ind_reg1;
  delete m_ldv_dot_dw_imm2_disp16_ind_reg1_wreg3;
  delete m_stv_dot_dw_imm1_wreg3_disp16_ind_reg1;
  delete m_ldvz_dot_h4_disp16_ind_reg1_wreg3;
  delete m_stvz_dot_h4_wreg3_disp16_ind_reg1;
  delete m_cmovf_dot_w4_wreg4_wreg1_wreg2_wreg3;
  delete m_shflv_dot_w4_imm12_wreg1_wreg2_wreg3;
  delete m_fmaf_dot_s4_wreg1_wreg2_wreg3;
  delete m_fmsf_dot_s4_wreg1_wreg2_wreg3;
  delete m_fnmaf_dot_s4_wreg1_wreg2_wreg3;
  delete m_fnmsf_dot_s4_wreg1_wreg2_wreg3;
  delete m_cmpf_dot_s4_fcond_wreg1_wreg2_wreg3;
  delete m_roundf_dot_sw4_wreg2_wreg3;
  delete m_roundf_dot_suw4_wreg2_wreg3;
  delete m_trncf_dot_sw4_wreg2_wreg3;
  delete m_trncf_dot_suw4_wreg2_wreg3;
  delete m_ceilf_dot_sw4_wreg2_wreg3;
  delete m_ceilf_dot_suw4_wreg2_wreg3;
  delete m_floorf_dot_sw4_wreg2_wreg3;
  delete m_floorf_dot_suw4_wreg2_wreg3;
  delete m_cvtf_dot_sw4_wreg2_wreg3;
  delete m_cvtf_dot_suw4_wreg2_wreg3;
  delete m_cvtf_dot_ws4_wreg2_wreg3;
  delete m_cvtf_dot_uws4_wreg2_wreg3;
  delete m_cvtf_dot_hs4_wreg2_wreg3;
  delete m_cvtf_dot_sh4_wreg2_wreg3;
  delete m_absf_dot_s4_wreg2_wreg3;
  delete m_negf_dot_s4_wreg2_wreg3;
  delete m_sqrtf_dot_s4_wreg2_wreg3;
  delete m_recipf_dot_s4_wreg2_wreg3;
  delete m_rsqrtf_dot_s4_wreg2_wreg3;
  delete m_flpv_dot_s4_imm2_wreg2_wreg3;
  delete m_movv_dot_w4_wreg2_wreg3;
  delete m_trfsrv_dot_w4_imm3_wreg2;
  delete m_addf_dot_s4_wreg1_wreg2_wreg3;
  delete m_subf_dot_s4_wreg1_wreg2_wreg3;
  delete m_mulf_dot_s4_wreg1_wreg2_wreg3;
  delete m_maxf_dot_s4_wreg1_wreg2_wreg3;
  delete m_minf_dot_s4_wreg1_wreg2_wreg3;
  delete m_divf_dot_s4_wreg1_wreg2_wreg3;
  delete m_addrf_dot_s4_wreg1_wreg2_wreg3;
  delete m_subrf_dot_s4_wreg1_wreg2_wreg3;
  delete m_mulrf_dot_s4_wreg1_wreg2_wreg3;
  delete m_maxrf_dot_s4_wreg1_wreg2_wreg3;
  delete m_minrf_dot_s4_wreg1_wreg2_wreg3;
  delete m_addxf_dot_s4_wreg1_wreg2_wreg3;
  delete m_subxf_dot_s4_wreg1_wreg2_wreg3;
  delete m_mulxf_dot_s4_wreg1_wreg2_wreg3;
  delete m_addsubf_dot_s4_wreg1_wreg2_wreg3;
  delete m_subaddf_dot_s4_wreg1_wreg2_wreg3;
  delete m_addsubxf_dot_s4_wreg1_wreg2_wreg3;
  delete m_subaddxf_dot_s4_wreg1_wreg2_wreg3;
  delete m_addsubnf_dot_s4_wreg1_wreg2_wreg3;
  delete m_subaddnf_dot_s4_wreg1_wreg2_wreg3;
  delete m_addsubnxf_dot_s4_wreg1_wreg2_wreg3;
  delete m_subaddnxf_dot_s4_wreg1_wreg2_wreg3;
  delete m_hvtrap_vector;
  delete m_stm_dot_gsr_ind_reg1;
  delete m_stm_dot_mp_eh_dec_et_ind_reg1;
  delete m_ldm_dot_gsr_ind_reg1;
  delete m_ldm_dot_mp_ind_reg1_eh_dec_et;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_add_reg1_reg2()
{
  return m_add_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_add_imm5_reg2()
{
  return m_add_imm5_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_addi_imm16_reg1_reg2()
{
  return m_addi_imm16_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_adf_cccc_reg1_reg2_reg3()
{
  return m_adf_cccc_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_and_reg1_reg2()
{
  return m_and_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_andi_imm16_reg1_reg2()
{
  return m_andi_imm16_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_b_cccc_disp9()
{
  return m_b_cccc_disp9;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_bsh_reg2_reg3()
{
  return m_bsh_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_bsw_reg2_reg3()
{
  return m_bsw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_callt_imm6()
{
  return m_callt_imm6;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_caxi_ind_reg1_reg2_reg3()
{
  return m_caxi_ind_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_clr1_bit_3_disp16_ind_reg1()
{
  return m_clr1_bit_3_disp16_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_clr1_reg2_ind_reg1()
{
  return m_clr1_reg2_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cmov_cccc_reg1_reg2_reg3()
{
  return m_cmov_cccc_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cmov_cccc_imm5_reg2_reg3()
{
  return m_cmov_cccc_imm5_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cmp_reg1_reg2()
{
  return m_cmp_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cmp_imm5_reg2()
{
  return m_cmp_imm5_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ctret()
{
  return m_ctret;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_di()
{
  return m_di;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_dispose_imm5_list12()
{
  return m_dispose_imm5_list12;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_dispose_imm5_list12_ind_reg1()
{
  return m_dispose_imm5_list12_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_div_reg1_reg2_reg3()
{
  return m_div_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_switch_reg1()
{
  return m_switch_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_divh_reg1_reg2()
{
  return m_divh_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_divh_reg1_reg2_reg3()
{
  return m_divh_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_divhu_reg1_reg2_reg3()
{
  return m_divhu_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_divq_reg1_reg2_reg3()
{
  return m_divq_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_divqu_reg1_reg2_reg3()
{
  return m_divqu_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_divu_reg1_reg2_reg3()
{
  return m_divu_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ei()
{
  return m_ei;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_eiret()
{
  return m_eiret;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_feret()
{
  return m_feret;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_fetrap_vector()
{
  return m_fetrap_vector;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_halt()
{
  return m_halt;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_hsh_reg2_reg3()
{
  return m_hsh_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_hsw_reg2_reg3()
{
  return m_hsw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_jr_disp22()
{
  return m_jr_disp22;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_jr_disp32()
{
  return m_jr_disp32;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_jarl_disp22_reg2()
{
  return m_jarl_disp22_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_jarl_disp32_reg1()
{
  return m_jarl_disp32_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_jmp_ind_reg1()
{
  return m_jmp_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_jmp_disp32_ind_reg1()
{
  return m_jmp_disp32_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_prepare_list12_imm5()
{
  return m_prepare_list12_imm5;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_prepare_list12_imm5_bits_ff()
{
  return m_prepare_list12_imm5_bits_ff;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ld_dot_b_disp16_ind_reg1_reg2()
{
  return m_ld_dot_b_disp16_ind_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ld_dot_b_disp23_ind_reg1_reg3()
{
  return m_ld_dot_b_disp23_ind_reg1_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ld_dot_b_ind_reg1_inc_reg3()
{
  return m_ld_dot_b_ind_reg1_inc_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ld_dot_b_ind_reg1_dec_reg3()
{
  return m_ld_dot_b_ind_reg1_dec_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ld_dot_bu_disp23_ind_reg1_reg3()
{
  return m_ld_dot_bu_disp23_ind_reg1_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ld_dot_bu_disp16_ind_reg1_reg2()
{
  return m_ld_dot_bu_disp16_ind_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ld_dot_bu_ind_reg1_inc_reg3()
{
  return m_ld_dot_bu_ind_reg1_inc_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ld_dot_bu_ind_reg1_dec_reg3()
{
  return m_ld_dot_bu_ind_reg1_dec_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ld_dot_h_disp16_ind_reg1_reg2()
{
  return m_ld_dot_h_disp16_ind_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ld_dot_h_disp23_ind_reg1_reg3()
{
  return m_ld_dot_h_disp23_ind_reg1_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ld_dot_h_ind_reg1_inc_reg3()
{
  return m_ld_dot_h_ind_reg1_inc_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ld_dot_h_ind_reg1_dec_reg3()
{
  return m_ld_dot_h_ind_reg1_dec_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ld_dot_hu_disp16_ind_reg1_reg2()
{
  return m_ld_dot_hu_disp16_ind_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ld_dot_hu_disp23_ind_reg1_reg3()
{
  return m_ld_dot_hu_disp23_ind_reg1_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ld_dot_hu_ind_reg1_inc_reg3()
{
  return m_ld_dot_hu_ind_reg1_inc_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ld_dot_hu_ind_reg1_dec_reg3()
{
  return m_ld_dot_hu_ind_reg1_dec_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ld_dot_w_disp16_ind_reg1_reg2()
{
  return m_ld_dot_w_disp16_ind_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ld_dot_w_disp23_ind_reg1_reg3()
{
  return m_ld_dot_w_disp23_ind_reg1_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ld_dot_w_ind_reg1_inc_reg3()
{
  return m_ld_dot_w_ind_reg1_inc_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ld_dot_w_ind_reg1_dec_reg3()
{
  return m_ld_dot_w_ind_reg1_dec_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ldsr_reg2_regid_selid()
{
  return m_ldsr_reg2_regid_selid;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_mac_reg1_reg2_reg3_reg4()
{
  return m_mac_reg1_reg2_reg3_reg4;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_macu_reg1_reg2_reg3_reg4()
{
  return m_macu_reg1_reg2_reg3_reg4;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_nop()
{
  return m_nop;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_mov_reg1_reg2()
{
  return m_mov_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_mov_imm5_reg2()
{
  return m_mov_imm5_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_mov_imm32_reg1()
{
  return m_mov_imm32_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_movea_imm16_reg1_reg2()
{
  return m_movea_imm16_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_movhi_imm16_reg1_reg2()
{
  return m_movhi_imm16_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_mul_reg1_reg2_reg3()
{
  return m_mul_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sxh_reg1()
{
  return m_sxh_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_mul_imm9_reg2_reg3()
{
  return m_mul_imm9_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_mulh_reg1_reg2()
{
  return m_mulh_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_mulh_imm5_reg2()
{
  return m_mulh_imm5_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_mulhi_imm16_reg1_reg2()
{
  return m_mulhi_imm16_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_mulu_reg1_reg2_reg3()
{
  return m_mulu_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_mulu_imm9_reg2_reg3()
{
  return m_mulu_imm9_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_not_reg1_reg2()
{
  return m_not_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_not1_bit_3_disp16_ind_reg1()
{
  return m_not1_bit_3_disp16_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_not1_reg2_ind_reg1()
{
  return m_not1_reg2_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_or_reg1_reg2()
{
  return m_or_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ori_imm16_reg1_reg2()
{
  return m_ori_imm16_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_rie()
{
  return m_rie;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_rie_imm5_imm4()
{
  return m_rie_imm5_imm4;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sar_reg1_reg2()
{
  return m_sar_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sar_imm5_reg2()
{
  return m_sar_imm5_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sar_reg1_reg2_reg3()
{
  return m_sar_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sasf_cccc_reg2()
{
  return m_sasf_cccc_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sxb_reg1()
{
  return m_sxb_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_zxb_reg1()
{
  return m_zxb_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_zxh_reg1()
{
  return m_zxh_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_satadd_reg1_reg2()
{
  return m_satadd_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_satadd_imm5_reg2()
{
  return m_satadd_imm5_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_satadd_reg1_reg2_reg3()
{
  return m_satadd_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_satsub_reg1_reg2()
{
  return m_satsub_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_satsub_reg1_reg2_reg3()
{
  return m_satsub_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_satsubi_imm16_reg1_reg2()
{
  return m_satsubi_imm16_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_satsubr_reg1_reg2()
{
  return m_satsubr_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sbf_cccc_reg1_reg2_reg3()
{
  return m_sbf_cccc_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sch0l_reg2_reg3()
{
  return m_sch0l_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sch0r_reg2_reg3()
{
  return m_sch0r_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sch1l_reg2_reg3()
{
  return m_sch1l_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sch1r_reg2_reg3()
{
  return m_sch1r_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_set1_bit_3_disp16_ind_reg1()
{
  return m_set1_bit_3_disp16_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_set1_reg2_ind_reg1()
{
  return m_set1_reg2_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_setf_cccc_reg2()
{
  return m_setf_cccc_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_shl_reg1_reg2()
{
  return m_shl_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_shl_imm5_reg2()
{
  return m_shl_imm5_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_shl_reg1_reg2_reg3()
{
  return m_shl_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_shr_reg1_reg2()
{
  return m_shr_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_shr_imm5_reg2()
{
  return m_shr_imm5_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_shr_reg1_reg2_reg3()
{
  return m_shr_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sld_dot_b_disp7_ind_ep_reg2()
{
  return m_sld_dot_b_disp7_ind_ep_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sld_dot_bu_disp4_ind_ep_reg2()
{
  return m_sld_dot_bu_disp4_ind_ep_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sld_dot_h_disp8_ind_ep_reg2()
{
  return m_sld_dot_h_disp8_ind_ep_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sld_dot_hu_disp5_ind_ep_reg2()
{
  return m_sld_dot_hu_disp5_ind_ep_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sld_dot_w_disp8_ind_ep_reg2()
{
  return m_sld_dot_w_disp8_ind_ep_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sst_dot_b_reg2_disp7_ind_ep()
{
  return m_sst_dot_b_reg2_disp7_ind_ep;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sst_dot_h_reg2_disp8_ind_ep()
{
  return m_sst_dot_h_reg2_disp8_ind_ep;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sst_dot_w_reg2_disp8_ind_ep()
{
  return m_sst_dot_w_reg2_disp8_ind_ep;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_st_dot_b_reg2_disp16_ind_reg1()
{
  return m_st_dot_b_reg2_disp16_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_st_dot_b_reg3_disp23_ind_reg1()
{
  return m_st_dot_b_reg3_disp23_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_st_dot_b_reg3_ind_reg1_inc()
{
  return m_st_dot_b_reg3_ind_reg1_inc;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_st_dot_b_reg3_ind_reg1_dec()
{
  return m_st_dot_b_reg3_ind_reg1_dec;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_st_dot_h_reg2_disp16_ind_reg1()
{
  return m_st_dot_h_reg2_disp16_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_st_dot_h_reg3_disp23_ind_reg1()
{
  return m_st_dot_h_reg3_disp23_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_st_dot_h_reg3_ind_reg1_inc()
{
  return m_st_dot_h_reg3_ind_reg1_inc;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_st_dot_h_reg3_ind_reg1_dec()
{
  return m_st_dot_h_reg3_ind_reg1_dec;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_st_dot_w_reg2_disp16_ind_reg1()
{
  return m_st_dot_w_reg2_disp16_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_st_dot_w_reg3_disp23_ind_reg1()
{
  return m_st_dot_w_reg3_disp23_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_st_dot_w_reg3_ind_reg1_inc()
{
  return m_st_dot_w_reg3_ind_reg1_inc;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_st_dot_w_reg3_ind_reg1_dec()
{
  return m_st_dot_w_reg3_ind_reg1_dec;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_stsr_regid_reg2_selid()
{
  return m_stsr_regid_reg2_selid;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sub_reg1_reg2()
{
  return m_sub_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_subr_reg1_reg2()
{
  return m_subr_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_synce()
{
  return m_synce;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_syncm()
{
  return m_syncm;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_syncp()
{
  return m_syncp;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_syscall_vector()
{
  return m_syscall_vector;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_trap_vector()
{
  return m_trap_vector;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_tst_reg1_reg2()
{
  return m_tst_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_tst1_bit_3_disp16_ind_reg1()
{
  return m_tst1_bit_3_disp16_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_tst1_reg2_ind_reg1()
{
  return m_tst1_reg2_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_xor_reg1_reg2()
{
  return m_xor_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_xori_imm16_reg1_reg2()
{
  return m_xori_imm16_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_absf_dot_d_reg2_reg3()
{
  return m_absf_dot_d_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_absf_dot_s_reg2_reg3()
{
  return m_absf_dot_s_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_addf_dot_d_reg1_reg2_reg3()
{
  return m_addf_dot_d_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_addf_dot_s_reg1_reg2_reg3()
{
  return m_addf_dot_s_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ceilf_dot_dl_reg2_reg3()
{
  return m_ceilf_dot_dl_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ceilf_dot_dul_reg2_reg3()
{
  return m_ceilf_dot_dul_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ceilf_dot_duw_reg2_reg3()
{
  return m_ceilf_dot_duw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ceilf_dot_dw_reg2_reg3()
{
  return m_ceilf_dot_dw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ceilf_dot_sl_reg2_reg3()
{
  return m_ceilf_dot_sl_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ceilf_dot_sul_reg2_reg3()
{
  return m_ceilf_dot_sul_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ceilf_dot_suw_reg2_reg3()
{
  return m_ceilf_dot_suw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ceilf_dot_sw_reg2_reg3()
{
  return m_ceilf_dot_sw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cmovf_dot_d_fcbit_reg1_reg2_reg3()
{
  return m_cmovf_dot_d_fcbit_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cmovf_dot_s_fcbit_reg1_reg2_reg3()
{
  return m_cmovf_dot_s_fcbit_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cmpf_dot_d_fcond_reg2_reg1_fcbit()
{
  return m_cmpf_dot_d_fcond_reg2_reg1_fcbit;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cmpf_dot_s_fcond_reg2_reg1_fcbit()
{
  return m_cmpf_dot_s_fcond_reg2_reg1_fcbit;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_dl_reg2_reg3()
{
  return m_cvtf_dot_dl_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_ds_reg2_reg3()
{
  return m_cvtf_dot_ds_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_dul_reg2_reg3()
{
  return m_cvtf_dot_dul_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_duw_reg2_reg3()
{
  return m_cvtf_dot_duw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_dw_reg2_reg3()
{
  return m_cvtf_dot_dw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_ld_reg2_reg3()
{
  return m_cvtf_dot_ld_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_ls_reg2_reg3()
{
  return m_cvtf_dot_ls_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_sd_reg2_reg3()
{
  return m_cvtf_dot_sd_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_sl_reg2_reg3()
{
  return m_cvtf_dot_sl_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_sul_reg2_reg3()
{
  return m_cvtf_dot_sul_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_suw_reg2_reg3()
{
  return m_cvtf_dot_suw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_sw_reg2_reg3()
{
  return m_cvtf_dot_sw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_uld_reg2_reg3()
{
  return m_cvtf_dot_uld_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_uls_reg2_reg3()
{
  return m_cvtf_dot_uls_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_uwd_reg2_reg3()
{
  return m_cvtf_dot_uwd_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_uws_reg2_reg3()
{
  return m_cvtf_dot_uws_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_wd_reg2_reg3()
{
  return m_cvtf_dot_wd_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_ws_reg2_reg3()
{
  return m_cvtf_dot_ws_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_divf_dot_d_reg1_reg2_reg3()
{
  return m_divf_dot_d_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_divf_dot_s_reg1_reg2_reg3()
{
  return m_divf_dot_s_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_floorf_dot_dl_reg2_reg3()
{
  return m_floorf_dot_dl_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_floorf_dot_dul_reg2_reg3()
{
  return m_floorf_dot_dul_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_floorf_dot_duw_reg2_reg3()
{
  return m_floorf_dot_duw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_floorf_dot_dw_reg2_reg3()
{
  return m_floorf_dot_dw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_floorf_dot_sl_reg2_reg3()
{
  return m_floorf_dot_sl_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_floorf_dot_sul_reg2_reg3()
{
  return m_floorf_dot_sul_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_floorf_dot_suw_reg2_reg3()
{
  return m_floorf_dot_suw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_floorf_dot_sw_reg2_reg3()
{
  return m_floorf_dot_sw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_maddf_dot_s_reg1_reg2_reg3_reg4()
{
  return m_maddf_dot_s_reg1_reg2_reg3_reg4;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_maxf_dot_d_reg1_reg2_reg3()
{
  return m_maxf_dot_d_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_maxf_dot_s_reg1_reg2_reg3()
{
  return m_maxf_dot_s_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_minf_dot_d_reg1_reg2_reg3()
{
  return m_minf_dot_d_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_minf_dot_s_reg1_reg2_reg3()
{
  return m_minf_dot_s_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_msubf_dot_s_reg1_reg2_reg3_reg4()
{
  return m_msubf_dot_s_reg1_reg2_reg3_reg4;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_mulf_dot_d_reg1_reg2_reg3()
{
  return m_mulf_dot_d_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_mulf_dot_s_reg1_reg2_reg3()
{
  return m_mulf_dot_s_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_negf_dot_d_reg2_reg3()
{
  return m_negf_dot_d_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_negf_dot_s_reg2_reg3()
{
  return m_negf_dot_s_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_nmaddf_dot_s_reg1_reg2_reg3_reg4()
{
  return m_nmaddf_dot_s_reg1_reg2_reg3_reg4;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_nmsubf_dot_s_reg1_reg2_reg3_reg4()
{
  return m_nmsubf_dot_s_reg1_reg2_reg3_reg4;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_recipf_dot_d_reg2_reg3()
{
  return m_recipf_dot_d_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_recipf_dot_s_reg2_reg3()
{
  return m_recipf_dot_s_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_rsqrtf_dot_d_reg2_reg3()
{
  return m_rsqrtf_dot_d_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_rsqrtf_dot_s_reg2_reg3()
{
  return m_rsqrtf_dot_s_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sqrtf_dot_d_reg2_reg3()
{
  return m_sqrtf_dot_d_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sqrtf_dot_s_reg2_reg3()
{
  return m_sqrtf_dot_s_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_subf_dot_d_reg1_reg2_reg3()
{
  return m_subf_dot_d_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_subf_dot_s_reg1_reg2_reg3()
{
  return m_subf_dot_s_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_trfsr_fcbit()
{
  return m_trfsr_fcbit;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_trncf_dot_dl_reg2_reg3()
{
  return m_trncf_dot_dl_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_trncf_dot_dul_reg2_reg3()
{
  return m_trncf_dot_dul_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_trncf_dot_duw_reg2_reg3()
{
  return m_trncf_dot_duw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_trncf_dot_dw_reg2_reg3()
{
  return m_trncf_dot_dw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_trncf_dot_sl_reg2_reg3()
{
  return m_trncf_dot_sl_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_trncf_dot_sul_reg2_reg3()
{
  return m_trncf_dot_sul_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_trncf_dot_suw_reg2_reg3()
{
  return m_trncf_dot_suw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_trncf_dot_sw_reg2_reg3()
{
  return m_trncf_dot_sw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_b_cccc_disp17()
{
  return m_b_cccc_disp17;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_binsu_reg1_lsb_msb_reg2()
{
  return m_binsu_reg1_lsb_msb_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_binsm_reg1_lsb_msb_reg2()
{
  return m_binsm_reg1_lsb_msb_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_binsl_reg1_lsb_msb_reg2()
{
  return m_binsl_reg1_lsb_msb_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_jarl_ind_reg1_reg3()
{
  return m_jarl_ind_reg1_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ld_dot_dw_disp23_ind_reg1_reg3()
{
  return m_ld_dot_dw_disp23_ind_reg1_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ldl_dot_bu_ind_reg1_reg3()
{
  return m_ldl_dot_bu_ind_reg1_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ldl_dot_hu_ind_reg1_reg3()
{
  return m_ldl_dot_hu_ind_reg1_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ldl_dot_w_ind_reg1_reg3()
{
  return m_ldl_dot_w_ind_reg1_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_loop_reg1_disp16()
{
  return m_loop_reg1_disp16;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_popsp_rh_dec_rt()
{
  return m_popsp_rh_dec_rt;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_pushsp_rh_dec_rt()
{
  return m_pushsp_rh_dec_rt;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_rotl_imm5_reg2_reg3()
{
  return m_rotl_imm5_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_rotl_reg1_reg2_reg3()
{
  return m_rotl_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_snooze()
{
  return m_snooze;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_st_dot_dw_reg3_disp23_ind_reg1()
{
  return m_st_dot_dw_reg3_disp23_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_stc_dot_b_reg3_ind_reg1()
{
  return m_stc_dot_b_reg3_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_stc_dot_h_reg3_ind_reg1()
{
  return m_stc_dot_h_reg3_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_stc_dot_w_reg3_ind_reg1()
{
  return m_stc_dot_w_reg3_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_synci()
{
  return m_synci;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_roundf_dot_dl_reg2_reg3()
{
  return m_roundf_dot_dl_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_roundf_dot_dul_reg2_reg3()
{
  return m_roundf_dot_dul_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_roundf_dot_duw_reg2_reg3()
{
  return m_roundf_dot_duw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_roundf_dot_dw_reg2_reg3()
{
  return m_roundf_dot_dw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_roundf_dot_sl_reg2_reg3()
{
  return m_roundf_dot_sl_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_roundf_dot_sul_reg2_reg3()
{
  return m_roundf_dot_sul_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_roundf_dot_suw_reg2_reg3()
{
  return m_roundf_dot_suw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_roundf_dot_sw_reg2_reg3()
{
  return m_roundf_dot_sw_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cache_cacheop_ind_reg1()
{
  return m_cache_cacheop_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_pref_prefop_ind_reg1()
{
  return m_pref_prefop_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_dbcp()
{
  return m_dbcp;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_dbpush_rh_dec_rt()
{
  return m_dbpush_rh_dec_rt;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_dbret()
{
  return m_dbret;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_dbtag_imm10()
{
  return m_dbtag_imm10;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_dbtrap()
{
  return m_dbtrap;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_hs_reg2_reg3()
{
  return m_cvtf_dot_hs_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_sh_reg2_reg3()
{
  return m_cvtf_dot_sh_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_fmaf_dot_s_reg1_reg2_reg3()
{
  return m_fmaf_dot_s_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_fmsf_dot_s_reg1_reg2_reg3()
{
  return m_fmsf_dot_s_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_fnmaf_dot_s_reg1_reg2_reg3()
{
  return m_fnmaf_dot_s_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_fnmsf_dot_s_reg1_reg2_reg3()
{
  return m_fnmsf_dot_s_reg1_reg2_reg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cll()
{
  return m_cll;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_resbank()
{
  return m_resbank;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_clip_dot_b_reg1_reg2()
{
  return m_clip_dot_b_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_clip_dot_bu_reg1_reg2()
{
  return m_clip_dot_bu_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_clip_dot_h_reg1_reg2()
{
  return m_clip_dot_h_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_clip_dot_hu_reg1_reg2()
{
  return m_clip_dot_hu_reg1_reg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ldv_dot_w_imm4_disp16_ind_reg1_wreg3()
{
  return m_ldv_dot_w_imm4_disp16_ind_reg1_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_stv_dot_w_imm2_wreg3_disp16_ind_reg1()
{
  return m_stv_dot_w_imm2_wreg3_disp16_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ldv_dot_qw_disp16_ind_reg1_wreg3()
{
  return m_ldv_dot_qw_disp16_ind_reg1_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_stv_dot_qw_wreg3_disp16_ind_reg1()
{
  return m_stv_dot_qw_wreg3_disp16_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ldv_dot_dw_imm2_disp16_ind_reg1_wreg3()
{
  return m_ldv_dot_dw_imm2_disp16_ind_reg1_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_stv_dot_dw_imm1_wreg3_disp16_ind_reg1()
{
  return m_stv_dot_dw_imm1_wreg3_disp16_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ldvz_dot_h4_disp16_ind_reg1_wreg3()
{
  return m_ldvz_dot_h4_disp16_ind_reg1_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_stvz_dot_h4_wreg3_disp16_ind_reg1()
{
  return m_stvz_dot_h4_wreg3_disp16_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cmovf_dot_w4_wreg4_wreg1_wreg2_wreg3()
{
  return m_cmovf_dot_w4_wreg4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_shflv_dot_w4_imm12_wreg1_wreg2_wreg3()
{
  return m_shflv_dot_w4_imm12_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_fmaf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_fmaf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_fmsf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_fmsf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_fnmaf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_fnmaf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_fnmsf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_fnmsf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cmpf_dot_s4_fcond_wreg1_wreg2_wreg3()
{
  return m_cmpf_dot_s4_fcond_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_roundf_dot_sw4_wreg2_wreg3()
{
  return m_roundf_dot_sw4_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_roundf_dot_suw4_wreg2_wreg3()
{
  return m_roundf_dot_suw4_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_trncf_dot_sw4_wreg2_wreg3()
{
  return m_trncf_dot_sw4_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_trncf_dot_suw4_wreg2_wreg3()
{
  return m_trncf_dot_suw4_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ceilf_dot_sw4_wreg2_wreg3()
{
  return m_ceilf_dot_sw4_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ceilf_dot_suw4_wreg2_wreg3()
{
  return m_ceilf_dot_suw4_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_floorf_dot_sw4_wreg2_wreg3()
{
  return m_floorf_dot_sw4_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_floorf_dot_suw4_wreg2_wreg3()
{
  return m_floorf_dot_suw4_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_sw4_wreg2_wreg3()
{
  return m_cvtf_dot_sw4_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_suw4_wreg2_wreg3()
{
  return m_cvtf_dot_suw4_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_ws4_wreg2_wreg3()
{
  return m_cvtf_dot_ws4_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_uws4_wreg2_wreg3()
{
  return m_cvtf_dot_uws4_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_hs4_wreg2_wreg3()
{
  return m_cvtf_dot_hs4_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_cvtf_dot_sh4_wreg2_wreg3()
{
  return m_cvtf_dot_sh4_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_absf_dot_s4_wreg2_wreg3()
{
  return m_absf_dot_s4_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_negf_dot_s4_wreg2_wreg3()
{
  return m_negf_dot_s4_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_sqrtf_dot_s4_wreg2_wreg3()
{
  return m_sqrtf_dot_s4_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_recipf_dot_s4_wreg2_wreg3()
{
  return m_recipf_dot_s4_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_rsqrtf_dot_s4_wreg2_wreg3()
{
  return m_rsqrtf_dot_s4_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_flpv_dot_s4_imm2_wreg2_wreg3()
{
  return m_flpv_dot_s4_imm2_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_movv_dot_w4_wreg2_wreg3()
{
  return m_movv_dot_w4_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_trfsrv_dot_w4_imm3_wreg2()
{
  return m_trfsrv_dot_w4_imm3_wreg2;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_addf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_addf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_subf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_subf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_mulf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_mulf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_maxf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_maxf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_minf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_minf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_divf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_divf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_addrf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_addrf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_subrf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_subrf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_mulrf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_mulrf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_maxrf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_maxrf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_minrf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_minrf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_addxf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_addxf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_subxf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_subxf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_mulxf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_mulxf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_addsubf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_addsubf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_subaddf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_subaddf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_addsubxf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_addsubxf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_subaddxf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_subaddxf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_addsubnf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_addsubnf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_subaddnf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_subaddnf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_addsubnxf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_addsubnxf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_subaddnxf_dot_s4_wreg1_wreg2_wreg3()
{
  return m_subaddnxf_dot_s4_wreg1_wreg2_wreg3;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_hvtrap_vector()
{
  return m_hvtrap_vector;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_stm_dot_gsr_ind_reg1()
{
  return m_stm_dot_gsr_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_stm_dot_mp_eh_dec_et_ind_reg1()
{
  return m_stm_dot_mp_eh_dec_et_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ldm_dot_gsr_ind_reg1()
{
  return m_ldm_dot_gsr_ind_reg1;
}

rh850g4::instruction_base_if_t *rh850g4::instr_set_impl_t::get_ldm_dot_mp_ind_reg1_eh_dec_et()
{
  return m_ldm_dot_mp_ind_reg1_eh_dec_et;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
