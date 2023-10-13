/*************************************************************************
 *
 * Copyright(c) 2019 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2019.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850/prepare_list12_imm5_bits_ff_instr.h"
#include <iomanip>
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// prepare list12, imm5, bits_ff implementation
//------------------------------------------------------------------------------

rh850::prepare_list12_imm5_bits_ff_instr_t::prepare_list12_imm5_bits_ff_instr_t
(
  fastiss::code_gen_if_t *code_gen_impl,
  rh850::memory_if_t *memory_impl,
  rh850::operand_if_t *operand_impl,
  rh850::cpu_state_if_t *cpu_state_impl,
  rh850::custom_if_t *custom_impl
) :
  rh850::instruction_base_impl_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl)
{
}

rh850::prepare_list12_imm5_bits_ff_instr_t::~prepare_list12_imm5_bits_ff_instr_t()
{
}

void rh850::prepare_list12_imm5_bits_ff_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned char bits_ff = get_bits_ff_u8_19_20(opcode);
  
  unsigned int pc_increment = 0;
  switch (bits_ff)
  {
    case 0x0:
      pc_increment = 4;
      break;
    case 0x1:
      pc_increment = 6;
      break;
    case 0x2:
      pc_increment = 6;
      break;
    default:
    case 0x3:
      pc_increment = 8;
      break;
  }
  
  // Increment PC
  increment_trans_PC(pc_increment);  
}

std::string rh850::prepare_list12_imm5_bits_ff_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned short list12 = get_list12_u16_21_31_0(opcode);
  unsigned int imm5 = get_imm5_u32_1_5_shift2(opcode);
  unsigned char bits_ff = get_bits_ff_u8_19_20(opcode);
  
  // Render and return result
  std::ostringstream disass_string;
  bool list12_empty = true;
  disass_string << std::string("prepare ");
  for (int list_id = 0; list_id < 12; list_id++)
  {
    if (list12 & prepare_dispose_rx_mask[list_id])
    {
      disass_string << register_name_GR(GR_index_t(list_id + 20)) << ", ";
      list12_empty = false;
    }
  }
  if (list12_empty)
  {
    // If there are no registers in the list12, then write 0x0
    disass_string << "0x0, ";
  }
  disass_string << "0x" << std::hex << imm5;
  
  unsigned int addr = read_trans_PC() + 4; 
  switch (bits_ff)
  {
    case 0x0:
      disass_string << ", sp";
      break;
    case 0x1:
      {
        unsigned short imm16;
        iN_fetch(addr, 2, (unsigned char *)&imm16);
        disass_string << ", 0x" << std::setfill('0') << std::setw(4) << std::hex << imm16;
      }
      break;
    case 0x2:
      {
        unsigned int imm16;
        iN_fetch(addr, 2, (unsigned char *)&imm16);
        // Shift immediate value
        imm16 <<= 16;
        disass_string << ", 0x" << std::setfill('0') << std::setw(8) << std::hex << imm16; 
      }
      break;
    default:
    case 0x3:
      {
        unsigned int imm32;
        iN_fetch(addr, 4, (unsigned char *)&imm32);
        disass_string << ", 0x" << std::hex << imm32; 
      }
      break;
  }
  return disass_string.str();  
}

unsigned int rh850::prepare_list12_imm5_bits_ff_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned char bits_ff = get_bits_ff_u8_19_20(opcode);
  
  unsigned int instr_length = 0;
  switch (bits_ff)
  {
    case 0x0:
      instr_length = 4;
      break;
    case 0x1:
    case 0x2:
      instr_length = 6;
      break;
    default:
    case 0x3:
      instr_length = 8;
      break;
  }
  return instr_length;
}

void rh850::prepare_list12_imm5_bits_ff_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned short list12 = get_list12_u16_21_31_0(opcode);
  fastiss::i32_t *imm5 = gen_get_imm5_u32_1_5_shift2(opcode);
  unsigned char bits_ff = get_bits_ff_u8_19_20(opcode);
  
  // Load SP registers
  fastiss::i32_t *sp_val = gen_read_r3();

  // Update mem_acc_type register (write)
  gen_write_mem_acc_type(gen_i32_constant(0x06));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));

  // Iterate over all registers in the range r20-r31
  for (int list_id = 0; list_id < 12; list_id++)
  {
    if (list12 & prepare_dispose_rx_mask[list_id])
    {
      // Write the register to the stack if the register is selected
      // The save address has the lower 2-bits masked to 0.
      sp_val = gen_sub(sp_val, gen_i32_constant(4));
      fastiss::i32_t *masked_sp_val = gen_and(sp_val, gen_i32_constant(0xfffffffc));
      // Check if MPU function enable
      if (is_mdp_enable()){
        fastiss::block_t *continue_block = create_basic_block();
        fastiss::i32_t *mpu_func_result = call_helper(
          "mpu_func_helper", 
          gen_read_core_ptr(), 
          gen_i32_constant(rh850::MPU_KIND_BL_W), 
          gen_i32_constant(rh850::WORD_SIZE),
          masked_sp_val,
          gen_i32_constant(0x519),
          NULL
        );
        gen_cond_branch(
          gen_icmp_eq(mpu_func_result, gen_i32_constant(1)),
          continue_block,
          get_exit_block()
        );
        set_gen_point(continue_block);
      }
      fastiss::i32_t *reg_val = gen_read_GR(GR_index_t(list_id + 20));
      gen_i32_store(masked_sp_val, reg_val);

      if(is_link_func_enable()){
	// clear the link(monitor) information
	call_helper("clear_monitor_helper", gen_read_link_manager_ptr(), gen_read_HTCFG0_PEID(), masked_sp_val, gen_i32_constant(4), NULL);
      }
    }
  }

  // Update the stack pointer value
  sp_val = gen_sub(sp_val, imm5);
  gen_write_r3(sp_val);
  
  // Update the EP value
  fastiss::i16_t *hword;
  fastiss::i32_t *new_ep_val;
  fastiss::i32_t *pc_increment;
  fastiss::i32_t *addr = gen_add(gen_read_PC(), gen_i32_constant(4));  
  switch (bits_ff)
  {
    case 0x0:
      new_ep_val = sp_val;
      pc_increment = gen_i32_constant(4);
      break;
    case 0x1:
      // Update mem_acc_type register (read)
      gen_write_mem_acc_type(gen_i32_constant(0x01));
      hword = gen_i16_load(addr);
      new_ep_val = gen_i32_sext(hword);
      pc_increment = gen_i32_constant(6);
      break;
    case 0x2:
      // Update mem_acc_type register (read)
      gen_write_mem_acc_type(gen_i32_constant(0x01));
      hword = gen_i16_load(addr);
      new_ep_val = gen_i32_zext(hword);
      new_ep_val = gen_shl(new_ep_val, gen_i32_constant(16));
      pc_increment = gen_i32_constant(6);
      break;
    default:
    case 0x3:
      // Update mem_acc_type register (read)
      gen_write_mem_acc_type(gen_i32_constant(0x02));
      new_ep_val = gen_i32_load(addr);
      pc_increment = gen_i32_constant(8);
      break;
  }
  gen_write_r30(new_ep_val);

  // Update PC
  gen_increment_PC(pc_increment);
}

unsigned int rh850::prepare_list12_imm5_bits_ff_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  unsigned short list12 = get_list12_u16_21_31_0(opcode);

  // Determine the number of set bits in the register list
  unsigned int ones_count = 0;
  for (int list_id = 0; list_id < 12; list_id++)
  {
    if (list12 & prepare_dispose_rx_mask[list_id])
    {
      ones_count++;
    }
  }
  // Two registers can be written per cycle, with a minimum of 1 cycle
  unsigned int return_value = (ones_count + 3) / 4;
  return (return_value ? return_value : 1);
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
