//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2019.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file instruction_base_impl.h
 * \brief Defines common base class for all rh850 instructions
 */

#ifndef _RH850_INSTRUCTION_BASE_IMPL_H_
#define _RH850_INSTRUCTION_BASE_IMPL_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850/instruction_base_if.h"
#include "rh850/immediate_base.h"
#include "fastiss/code_gen_bridge.h"
#include "rh850/operand_bridge.h"
#include <string>
#include <cstdio>

//------------------------------------------------------------------------------
// rh850 instruction base class definition
//------------------------------------------------------------------------------

/**
 * \brief Contains rh850 specific components
 */  
namespace rh850
{
  /**
   * \brief Common base class for all rh850 instructions
   *
   * Provides bridge implementations for the code generator, memory, operand extraction,
   * CPU state and custom operation components. This allows instructions to access the
   * operations provided by these interfaces without the need to write a significant
   * amount of "boiler-plate" code.
   * 
   * Also establishes the disassembly, phase handler and attribute query interface
   * (overloaded as required by individual instructions) used by the translators to perform
   * code generation.
   */
  class instruction_base_impl_t :
    public rh850::instruction_base_if_t,
    public rh850::immediate_base_t,
    public fastiss::code_gen_bridge_t, 
    public rh850::operand_bridge_t
  {
  public:
    /**
     * \brief Instruction base class constructor
     * \param code_gen_impl Handle to code generator to be used by instruction implementations
     * \param memory_impl Handle to memory access sub-system to be used by instruction implementations
     * \param operand_impl Handle to operand extraction operations to be used by instruction implementations
     * \param cpu_state_impl Handle to CPU state to be manipulated by instruction implementations
     * \param custom_impl Handle to custom operations to be used by instruction implementations
     */
    instruction_base_impl_t
    (
      fastiss::code_gen_if_t *code_gen_impl,
      rh850::memory_if_t *memory_impl,
      rh850::operand_if_t *operand_impl,
      rh850::cpu_state_if_t *cpu_state_impl,
      rh850::custom_if_t *custom_impl
    );
    
    /**
     * \brief Instruction base class destructor
     */
    virtual ~instruction_base_impl_t();

    /**
     * \brief Instruction decode operation
     *
     * It is expected that derived instructions should override this operations
     * to increment the translation-time PC to point at the next instruction to
     * be translated and to calculate the return values of the attribute query
     * operations (if these cannot be established statically).
     *
     * If the address of the next PC cannot be established at translation time,
     * then the derived instruction should terminate the current block (the
     * rh850::instruction_base_if_t::terminates_block
     * attribute is used for this purpose).
     *
     * Note that if the derived instruction terminates the current block then the
     * derived instruction does not need to update the translation time PC.
     *
     * \param opcode Opcode of instruction to be translated
     * \param trans_PC Translation-time PC value of the instruction
     */
    virtual void decode(rh850::opcode_t opcode, unsigned int trans_PC) = 0;
    /**
     * \brief Instruction disassemble operation
     * \param opcode Opcode of instruction to be disassembled
     * \param trans_PC Translation-time PC value of the instruction
     * \return String representing disassembly of the specified opcode
     */
    virtual std::string disassemble(rh850::opcode_t opcode, unsigned int trans_PC) = 0;
    /**
     * \brief Instruction length operation
     * \param opcode Opcode of the instruction
     * \param trans_PC Translation-time PC value of the instruction
     * \return The length (in bytes) of the opcode representation of the instruction
     */
    virtual unsigned int length(rh850::opcode_t opcode, unsigned int trans_PC) = 0;
  
    /**
     * \brief Generates execute phase code
     * \param opcode Opcode of instruction to be translated
     * \param trans_PC Translation-time PC value of the instruction
     */
    virtual void execute(rh850::opcode_t opcode, unsigned int trans_PC) = 0;

    /**
     * \brief terminates_block attribute query operation
     * \param opcode Opcode of instruction to be translated
     * \param trans_PC Translation-time PC value of the instruction
     * \return Result of terminates_block attribute query
     */
    virtual bool terminates_block(rh850::opcode_t opcode, unsigned int trans_PC);
    /**
     * \brief cycle_count attribute query operation
     * \param opcode Opcode of instruction to be translated
     * \param trans_PC Translation-time PC value of the instruction
     * \return Result of cycle_count attribute query
     */
    virtual unsigned int cycle_count(rh850::opcode_t opcode, unsigned int trans_PC);
  };
}

#endif // _RH850_INSTRUCTION_BASE_IMPL_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
