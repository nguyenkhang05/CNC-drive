//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2020.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file instruction_base_if.h
 * \brief Defines common base interface class for all rh850g4 instructions
 */

#ifndef _RH850G4_INSTRUCTION_BASE_IF_H_
#define _RH850G4_INSTRUCTION_BASE_IF_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850g4/opcode.h"
#include <string>

//------------------------------------------------------------------------------
// rh850g4 instruction base interface definition
//------------------------------------------------------------------------------

/**
 * \brief Contains rh850g4 specific components
 */  
namespace rh850g4
{
  /**
   * \brief Common base class for all rh850g4 instructions. Provides the
   * disassembly, phase handler and attribute query interface
   * (overloaded as required by individual instructions) used by the translators
   * to perform code generation.
   */
  class instruction_base_if_t
  {
  public:
    /**
     * \defgroup InstructionInterfaceOperations Instruction operations
     * @{
     * \defgroup InstructionExecutionPhaseOperations Instruction execution phase operations
     * \defgroup InstructionAttributeOperations Instruction attribute operations
     * @}
     */

    /**
     * \addtogroup InstructionInterfaceOperations
     * @{
     */

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
     * rh850g4::instruction_base_if_t::terminates_block attribute is used for
     * this purpose).
     *
     * Note that if the derived instruction terminates the current block then the
     * derived instruction does not need to update the translation time PC.
     *
     * \param opcode Opcode of instruction to be translated
     * \param trans_PC Translation-time PC value of the instruction
     */
    virtual void decode(rh850g4::opcode_t opcode, unsigned int trans_PC) = 0;
    /**
     * \brief Instruction disassemble operation
     * \param opcode Opcode of instruction to be disassembled
     * \param trans_PC Translation-time PC value of the instruction
     * \return String representing disassembly of the specified opcode
     */
    virtual std::string disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC) = 0;
    /**
     * \brief Instruction length operation
     * \param opcode Opcode of the instruction
     * \param trans_PC Translation-time PC value of the instruction
     * \return The length (in bytes) of the opcode representation of the instruction
     */
    virtual unsigned int length(rh850g4::opcode_t opcode, unsigned int trans_PC) = 0;
    /**
     * @}
     */
  
    /**
     * \addtogroup InstructionExecutionPhaseOperations
     * @{
     */

    /**
     * \brief Generates execute phase code
     * \param opcode Opcode of instruction to be translated
     * \param trans_PC Translation-time PC value of the instruction
     */
    virtual void execute(rh850g4::opcode_t opcode, unsigned int trans_PC) = 0;
    /**
     * @}
     */

    /**
     * \addtogroup InstructionAttributeOperations
     * @{
     */

    /**
     * \brief terminates_block attribute query operation
     * \param opcode Opcode of instruction to be translated
     * \param trans_PC Translation-time PC value of the instruction
     * \return Result of terminates_block attribute query
     */
    virtual bool terminates_block(rh850g4::opcode_t opcode, unsigned int trans_PC) = 0;
    /**
     * \brief cycle_count attribute query operation
     * \param opcode Opcode of instruction to be translated
     * \param trans_PC Translation-time PC value of the instruction
     * \return Result of cycle_count attribute query
     */
    virtual unsigned int cycle_count(rh850g4::opcode_t opcode, unsigned int trans_PC) = 0;
    /**
     * @}
     */
  };
}

#endif // _RH850G4_INSTRUCTION_BASE_IF_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
