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

#include "assert.h"
#include "rh850g4/continue_translator.h"
#include "rh850g4/undefined_instr.h"
#include "rh850g4/rie_instr.h"
#include "fastiss/block_function.h"

//------------------------------------------------------------------------------
// rh850g4 continue translator implementation
//------------------------------------------------------------------------------

rh850g4::continue_translator_t::continue_translator_t
(
  fastiss::code_gen_if_t *code_gen,
  rh850g4::memory_if_t *memory,
  rh850g4::operand_if_t *operand,
  rh850g4::cpu_state_if_t *cpu_state,
  rh850g4::custom_if_t *custom,
  unsigned int block_size,
  bool swdt_enable,
  rh850g4::analysis_point_if* pre_translate_loop_analysis_point,
  rh850g4::analysis_point_if* post_translate_loop_analysis_point,
  rh850g4::analysis_point_if* post_fetch_analysis_point,
  rh850g4::analysis_point_if* post_decode_analysis_point,
  rh850g4::analysis_point_if* post_execute_analysis_point,
  unsigned int translation_partition_mask
) :
  m_decoder0_instructions(code_gen, memory, operand, cpu_state, custom),
  m_decoder1_instructions(code_gen, memory, operand, cpu_state, custom),
  continue_translator_base_t
  (
    code_gen,
    memory,
    cpu_state,
    pre_translate_loop_analysis_point,
    post_translate_loop_analysis_point,
    post_fetch_analysis_point,
    post_decode_analysis_point,
    post_execute_analysis_point,
    &m_decoder0_instructions, &m_decoder1_instructions
  ),
  m_operand(operand),
  m_custom(custom),
  m_check_watchpoint_hit_at_exit(false),
  m_instruction_count(0),
  m_block_size(block_size),
  m_translated_PC_set(),
  m_translation_partition_mask(translation_partition_mask),
  m_swdt_enable(swdt_enable)
{
  m_undef_instr = new rie_instr_t(m_code_gen, m_memory, m_operand, m_cpu_state, m_custom);
}
    
rh850g4::continue_translator_t::~continue_translator_t()
{
  delete m_undef_instr;
}

void rh850g4::continue_translator_t::pre_translate_loop_callout(fastiss::block_function_t *block_fn)
{
  // Reset translated PC set
  m_translated_PC_set.clear();
  
  // Reset instruction count
  m_instruction_count = 0;
  
  // Reset the watchpoint flags
  m_cpu_state->write_check_watchpoint_hit(false);
  m_check_watchpoint_hit_at_exit = false;
  
  // Setup the translation region information
  m_block_start_address = m_cpu_state->read_trans_PC();
  m_current_translation_partition = m_block_start_address & m_translation_partition_mask;
  if (m_translation_partitions.find(m_current_translation_partition) == m_translation_partitions.end())
  {
    m_translation_partitions[m_current_translation_partition] = new std::set<unsigned int>;
  }
  m_translation_partitions[m_current_translation_partition]->insert(m_block_start_address);
}

void rh850g4::continue_translator_t::post_translate_loop_callout(fastiss::block_function_t *block_fn)
{
  // Create exit block
  m_custom->create_exit_block();

  // Check if watchpoints were hit
  if (m_check_watchpoint_hit_at_exit || m_cpu_state->read_check_watchpoint_hit())
  {
    m_check_watchpoint_hit_at_exit = false;
    fastiss::block_t *watchpoint_hit_block = m_code_gen->create_basic_block();
    fastiss::block_t *continue_block = m_code_gen->create_basic_block();
    fastiss::i1_t *is_watchpoint_hit = m_code_gen->gen_icmp_ne(m_cpu_state->gen_read_watchpoint_hit(), m_code_gen->gen_i8_constant(0));
    m_code_gen->gen_cond_branch(is_watchpoint_hit, watchpoint_hit_block, continue_block);
    m_code_gen->set_gen_point(watchpoint_hit_block);
    m_code_gen->call_helper("hit_breakpoint_helper", m_cpu_state->gen_read_core_ptr(), NULL);
    m_code_gen->gen_branch(continue_block);
    m_code_gen->set_gen_point(continue_block);
  }
}

void rh850g4::continue_translator_t::post_fetch_callout(fastiss::block_function_t *block_fn)
{
  // Grab the decode phase instruction
  instruction_base_if_t *instruction = m_instruction[m_decode_phase_index];

  // Ensure that instruction is not NULL
  if (instruction == NULL)
  {
    instruction = m_undef_instr;
    m_instruction[m_decode_phase_index] = m_undef_instr;
  }

  // Determine if a breakpoint has been hit
  if (is_bkpt_at_addr(m_PC_value[m_decode_phase_index]))
  {
    m_instruction[m_decode_phase_index] = m_bkpt_instr;
    m_fetches_enabled = false;
  }

  // Determine if the PC has previously been translated
  if (m_translated_PC_set.count(m_PC_value[m_decode_phase_index]))
  {
    m_instruction[m_decode_phase_index] = m_terminate_instruction;
    m_fetches_enabled = false;
  }
  else
  {
    m_translated_PC_set.insert(m_PC_value[m_decode_phase_index]);
  }

  if ((m_PC_value[m_decode_phase_index] & m_translation_partition_mask) != m_current_translation_partition)
  {
    // Update translation partition information
    m_current_translation_partition = (m_PC_value[m_decode_phase_index] & m_translation_partition_mask);
    if (m_translation_partitions.find(m_current_translation_partition) == m_translation_partitions.end())
    {
        m_translation_partitions[m_current_translation_partition] = new std::set<unsigned int>;
    }
    m_translation_partitions[m_current_translation_partition]->insert(m_block_start_address);
  }
}

void rh850g4::continue_translator_t::post_decode_callout(fastiss::block_function_t *block_fn)
{
  // Grab the decode phase instruction
  instruction_base_if_t *instruction = m_instruction[m_decode_phase_index];
}

void rh850g4::continue_translator_t::post_execute_callout(fastiss::block_function_t *block_fn)
{
  // Grab the execute phase instruction
  instruction_base_if_t *instruction = m_instruction[m_execute_phase_index];

  // Check if executed PC is matched SWD0PCAD value
  if(m_swdt_enable){
    fastiss::i1_t *is_at_swd0pcad_value = m_code_gen->gen_icmp_eq(m_cpu_state->gen_read_swd0pcad_val(), m_code_gen->gen_i32_constant(m_PC_value[m_execute_phase_index]));
    fastiss::i32_t *increment_amount = m_code_gen->gen_select(is_at_swd0pcad_value, m_code_gen->gen_i32_constant(1), m_code_gen->gen_i32_constant(0));
    m_cpu_state->gen_increment_swd0pcad_exec_count(increment_amount);
  }

  // Check if watchpoints need to be checked
  if (m_cpu_state->read_check_watchpoint_hit())
  {
    m_cpu_state->write_check_watchpoint_hit(false);
    m_check_watchpoint_hit_at_exit = true;
    fastiss::block_t *continue_block = m_code_gen->create_basic_block();
    fastiss::i1_t *is_watchpoint_hit = m_code_gen->gen_icmp_ne(m_cpu_state->gen_read_watchpoint_hit(), m_code_gen->gen_i8_constant(0));
    m_code_gen->gen_cond_branch(is_watchpoint_hit, m_custom->get_exit_block(), continue_block);
    m_code_gen->set_gen_point(continue_block);
  }

  // Perform translation termination checks
  if ((instruction != m_null_instruction) && (instruction != m_bkpt_instr) && (instruction != m_terminate_instruction))
  {
    // Increment the translated instruction count
    m_instruction_count++;

    // Check block size limits
    m_fetches_enabled &= (m_instruction_count != m_block_size);

    // Check for translation termination attribute
    m_fetches_enabled &= !instruction->terminates_block(m_opcode[m_execute_phase_index], m_PC_value[m_execute_phase_index]);
  }

  // Check for translation termination instruction
  if (instruction == m_terminate_instruction)
  {
    m_translation_complete = true;
  }
}

bool rh850g4::continue_translator_t::is_decode_valid()
{
  return ((m_instruction[m_decode_phase_index] != m_bkpt_instr) && (m_instruction[m_decode_phase_index] != m_null_instruction) && (m_instruction[m_decode_phase_index] != m_terminate_instruction));
}

bool rh850g4::continue_translator_t::is_execute_valid()
{
  return ((m_instruction[m_execute_phase_index] != m_bkpt_instr) && (m_instruction[m_execute_phase_index] != m_null_instruction) && (m_instruction[m_execute_phase_index] != m_terminate_instruction));
}


//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
