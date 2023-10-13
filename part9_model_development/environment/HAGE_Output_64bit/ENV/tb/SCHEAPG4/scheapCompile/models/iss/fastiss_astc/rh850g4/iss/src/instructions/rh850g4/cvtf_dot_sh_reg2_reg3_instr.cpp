/*************************************************************************
 *
 * Copyright(c) 2015-2017 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

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

#include "rh850g4/cvtf_dot_sh_reg2_reg3_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// cvtf.sh reg2, reg3 implementation
//------------------------------------------------------------------------------

rh850g4::cvtf_dot_sh_reg2_reg3_instr_t::cvtf_dot_sh_reg2_reg3_instr_t
(
  fastiss::code_gen_if_t *code_gen_impl,
  rh850g4::memory_if_t *memory_impl,
  rh850g4::operand_if_t *operand_impl,
  rh850g4::cpu_state_if_t *cpu_state_impl,
  rh850g4::custom_if_t *custom_impl
) :
  rh850g4::instruction_base_impl_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl)
{
}

rh850g4::cvtf_dot_sh_reg2_reg3_instr_t::~cvtf_dot_sh_reg2_reg3_instr_t()
{
}

void rh850g4::cvtf_dot_sh_reg2_reg3_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::cvtf_dot_sh_reg2_reg3_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  std::string reg2_name = register_name_GR(reg2);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);
  std::string reg3_name = register_name_GR(reg3);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "cvtf.sh " << reg2_name << ", " << reg3_name;
  return disass_string.str();
}

unsigned int rh850g4::cvtf_dot_sh_reg2_reg3_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::cvtf_dot_sh_reg2_reg3_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));

  // Create sub-blocks
  fastiss::block_t *unimplemented_exp_op_branch = create_basic_block();
  fastiss::block_t *unimplemented_exp_res_branch = create_basic_block();
  fastiss::block_t *implemented_res_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *invalid_qnan_branch = create_basic_block();
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *continue_block = create_basic_block();

  // Load operand registers
  fastiss::i32_t *reg2_load = gen_read_GR(reg2);

  // Cast the operands to 64-bit floating point values
  fastiss::f32_t *reg2_float32 = gen_f32_cast(reg2_load);

  // Check if the operand is a denormaled value and FPSR.FS=0
  gen_cond_branch(is_unimplemented_exp(reg2_float32), unimplemented_exp_op_branch, check_qnan_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_op_branch);
  gen_update_dest_and_fpsr(false, reg3, gen_i32_constant(0), gen_i8_constant(0x20), continue_block);

  // Check if qnan
  set_gen_point(check_qnan_branch);

  // Check if either operand is a signalling NaN
  fastiss::i1_t* is_signalling_nan = gen_is_signalling_nan(reg2_float32);

  // Branch to the invalid quiet Nan case if the operand is a signalling NaN
  gen_cond_branch(is_signalling_nan, invalid_qnan_branch, perform_calc_branch);

  // Handle invalid quiet NaN case
  set_gen_point(invalid_qnan_branch);
  gen_update_dest_and_fpsr(false, reg3, gen_i32_constant(0x7fff), gen_i8_constant(0x10), continue_block);

  // Handle general operand case
  set_gen_point(perform_calc_branch);   

  // Flush denorm operands to zero (keeping the sign of the denorm)
  fastiss::f32_t *reg2_float32_flushed = gen_flush_denorm(reg2_float32);

  // Truncate the operand to 16-bits - always round to nearest
  fastiss::f16_with_exception_flags_t trunc_result = gen_f16_trunc(reg2_float32_flushed, gen_read_round_mode());
  
  // Check for denormal (underflow) results
  gen_underflow_denorm_update(trunc_result);

  // Check if the result is a denormaled or underflow value and FPSR.FS=0
  gen_cond_branch(gen_and(trunc_result.underflow, gen_icmp_eq(gen_read_FPSR_FS(), gen_i1_constant(0))), 
		  unimplemented_exp_res_branch,
		  implemented_res_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_res_branch);
  gen_update_dest_and_fpsr(false, reg3, gen_i32_constant(0), gen_i8_constant(0x20), continue_block);

  // not unimplementeded exception case
  set_gen_point(implemented_res_branch);

  trunc_result.result = gen_select
  (
    trunc_result.underflow,
    gen_f16_cast(gen_and(gen_i16_cast(trunc_result.result), gen_i16_constant(0x8000))),
    trunc_result.result
  );

  // Check if the inexact flag must be explicitly cleared due to an enabled overflow/underflow
  trunc_result.inexact = gen_inexact_enabled_overflow_underflow_update(trunc_result.overflow, trunc_result.underflow, trunc_result.inexact);
   
  // Check if the inexact flag must be explicitly set due to a denorm result
  trunc_result.inexact = gen_inexact_denorm_update(trunc_result.invalid, trunc_result.divide_by_zero, trunc_result.inexact, reg2_float32);

  // Update the result, FPSR.XP, and handle exceptions
  fastiss::i8_t* trunc_result_xp = gen_i8_compose(gen_false(), gen_false(), gen_false(), trunc_result.invalid, trunc_result.divide_by_zero, trunc_result.overflow, trunc_result.underflow, trunc_result.inexact);
  gen_update_dest_and_fpsr(false, reg3, gen_select(gen_is_quiet_nan(reg2_float32), gen_select(gen_is_positive(reg2_float32), gen_i32_constant(0x7fff), gen_i32_constant(0xffff)), gen_i32_zext(gen_i16_cast(trunc_result.result))), trunc_result_xp, continue_block);

  // Continue block
  set_gen_point(continue_block);
    
  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
