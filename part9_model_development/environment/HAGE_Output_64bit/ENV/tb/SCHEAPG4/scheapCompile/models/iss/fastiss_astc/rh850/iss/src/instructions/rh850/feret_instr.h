//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2019.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

#ifndef _RH850_FERET_INSTR_H_
#define _RH850_FERET_INSTR_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850/instruction_base_impl.h"

//------------------------------------------------------------------------------
// feret declaration
//------------------------------------------------------------------------------

namespace rh850
{
  /**
   * \brief feret instruction
   */
  class feret_instr_t : public instruction_base_impl_t
  {
  public:
    /**
     * \brief feret instruction constructor
     * \param code_gen_impl Pointer to code generator implementor
     * \param memory_impl Pointer to memory implementor
     * \param operand_impl Pointer to operand implementor
     * \param cpu_state_impl Pointer to CPU state implementor
     * \param custom_impl Pointer to custom extension interface implementor
     */
    feret_instr_t
    (
      fastiss::code_gen_if_t *code_gen_impl,
      rh850::memory_if_t *memory_impl,
      operand_if_t *operand_impl,
      cpu_state_if_t *cpu_state_impl,
      custom_if_t *custom_impl
    );

    /**
     * \brief feret instruction destructor
     */
    virtual ~feret_instr_t();

    /**
     * \brief decode phase handler
     * \param opcode Instruction opcode
     * \param trans_PC Translation-time PC value of the instruction
     */
    virtual void decode(opcode_t opcode, unsigned int trans_PC);
    
    /**
     * \brief feret instruction disassembly operation
     * \param opcode Instruction opcode
     * \param trans_PC Translation-time PC value of the instruction
     * \return Disassembly string
     */
    virtual std::string disassemble(opcode_t opcode, unsigned int trans_PC);

    /**
     * \brief feret instruction length operation
     * \param opcode Instruction opcode
     * \param trans_PC Translation-time PC value of the instruction
     */
    virtual unsigned int length(opcode_t opcode, unsigned int trans_PC);

    /**
     * \brief feret instruction execute phase handler
     * \param opcode Instruction opcode
     * \param trans_PC Translation-time PC value of the instruction
     */
    virtual void execute(opcode_t opcode, unsigned int trans_PC);

    /**
     * \brief feret instruction terminates_block attribute query handler
     * \param opcode Instruction opcode
     * \param trans_PC Translation-time PC value of the instruction
     * \return terminates_block attribute value
     */
    virtual bool terminates_block(opcode_t opcode, unsigned int trans_PC);

    /**
     * \brief feret instruction cycle_count attribute query handler
     * \param opcode Instruction opcode
     * \param trans_PC Translation-time PC value of the instruction
     * \return cycle_count attribute value
     */
    virtual unsigned int cycle_count(opcode_t opcode, unsigned int trans_PC);
  };
}

#endif // _RH850_FERET_INSTR_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
