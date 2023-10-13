/*************************************************************************
 *
 * Copyright(c) 2016-2017 Renesas Electronics Corporation
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

#ifndef _RH850G4_STV_DOT_W_IMM2_WREG3_DISP16_IND_REG1_INSTR_H_
#define _RH850G4_STV_DOT_W_IMM2_WREG3_DISP16_IND_REG1_INSTR_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850g4/instruction_base_impl.h"

//------------------------------------------------------------------------------
// stv.w imm2, wreg3, disp16 [reg1] declaration
//------------------------------------------------------------------------------

namespace rh850g4
{
  /**
   * \brief stv_dot_w_imm2_wreg3_disp16_ind_reg1 instruction
   */
  class stv_dot_w_imm2_wreg3_disp16_ind_reg1_instr_t : public instruction_base_impl_t
  {
  public:
    /**
     * \brief stv_dot_w_imm2_wreg3_disp16_ind_reg1 instruction constructor
     * \param code_gen_impl Pointer to code generator implementor
     * \param memory_impl Pointer to memory implementor
     * \param operand_impl Pointer to operand implementor
     * \param cpu_state_impl Pointer to CPU state implementor
     * \param custom_impl Pointer to custom extension interface implementor
     */
    stv_dot_w_imm2_wreg3_disp16_ind_reg1_instr_t
    (
      fastiss::code_gen_if_t *code_gen_impl,
      rh850g4::memory_if_t *memory_impl,
      operand_if_t *operand_impl,
      cpu_state_if_t *cpu_state_impl,
      custom_if_t *custom_impl
    );

    /**
     * \brief stv_dot_w_imm2_wreg3_disp16_ind_reg1 instruction destructor
     */
    virtual ~stv_dot_w_imm2_wreg3_disp16_ind_reg1_instr_t();

    /**
     * \brief decode phase handler
     * \param opcode Instruction opcode
     * \param trans_PC Translation-time PC value of the instruction
     */
    virtual void decode(opcode_t opcode, unsigned int trans_PC);
    
    /**
     * \brief stv_dot_w_imm2_wreg3_disp16_ind_reg1 instruction disassembly operation
     * \param opcode Instruction opcode
     * \param trans_PC Translation-time PC value of the instruction
     * \return Disassembly string
     */
    virtual std::string disassemble(opcode_t opcode, unsigned int trans_PC);

    /**
     * \brief stv_dot_w_imm2_wreg3_disp16_ind_reg1 instruction length operation
     * \param opcode Instruction opcode
     * \param trans_PC Translation-time PC value of the instruction
     */
    virtual unsigned int length(opcode_t opcode, unsigned int trans_PC);

    /**
     * \brief stv_dot_w_imm2_wreg3_disp16_ind_reg1 instruction execute phase handler
     * \param opcode Instruction opcode
     * \param trans_PC Translation-time PC value of the instruction
     */
    virtual void execute(opcode_t opcode, unsigned int trans_PC);

    
  };
}

#endif // _RH850G4_STV_DOT_W_IMM2_WREG3_DISP16_IND_REG1_INSTR_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
