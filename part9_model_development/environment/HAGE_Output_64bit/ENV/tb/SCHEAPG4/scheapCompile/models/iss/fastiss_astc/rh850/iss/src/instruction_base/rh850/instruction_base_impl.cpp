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

#include "rh850/instruction_base_impl.h"

//------------------------------------------------------------------------------
// Instruction base class implementation
//------------------------------------------------------------------------------

// Constructor/destructor
rh850::instruction_base_impl_t::instruction_base_impl_t
(
  fastiss::code_gen_if_t *code_gen_impl,
  rh850::memory_if_t *memory_impl,
  rh850::operand_if_t *operand_impl,
  rh850::cpu_state_if_t *cpu_state_impl,
  rh850::custom_if_t *custom_impl
) :
  immediate_base_t(memory_impl, cpu_state_impl, custom_impl),
  fastiss::code_gen_bridge_t(code_gen_impl), 
  rh850::operand_bridge_t(operand_impl)
{
}

rh850::instruction_base_impl_t::~instruction_base_impl_t()
{
}

// Instruction attributes
bool rh850::instruction_base_impl_t::terminates_block(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return false;
}

unsigned int rh850::instruction_base_impl_t::cycle_count(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 1;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
