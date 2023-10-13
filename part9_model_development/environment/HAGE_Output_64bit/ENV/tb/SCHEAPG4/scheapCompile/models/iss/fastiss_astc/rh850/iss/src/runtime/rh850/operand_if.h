//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2019.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file operand_if.h
 * \brief Defines the rh850 operand extraction interface class
 */

#ifndef _RH850_OPERAND_IF_H_
#define _RH850_OPERAND_IF_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850/opcode.h"
#include "rh850/cpu_state_if.h"

//------------------------------------------------------------------------------
// Code generator class pre-declarations
//------------------------------------------------------------------------------

namespace fastiss
{
  class i8_t;
  class i16_t;
  class i32_t;
}

//------------------------------------------------------------------------------
// rh850 operand extraction interface definition
//------------------------------------------------------------------------------

/**
 * \brief Contains rh850 specific components
 */

namespace rh850
{
  /**
   * \brief Interface providing immediate and generative operand extraction
   * operations
   */  
  class operand_if_t
  {
  public:
    /**
     * \defgroup OperandExtractionOperations Operand extraction operations
     * \brief All operand extraction operations
     * @{
     * \defgroup ImmediateOperandExtractionOperations Immediate operand extraction operations
     * \brief Immediate operand extraction operations
     * \defgroup GenerativeOperandExtractionOperations Generative operand extraction operations
     * \brief Generative operand extraction operations
     * @}
     */

    /**
     * \addtogroup ImmediateOperandExtractionOperations
     * @{
     */
    /**
     * \brief Calculates an integer value representing the value of the bit operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the bit operand
     */
    virtual unsigned char get_bit_u8_11_13(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the bits_ff operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the bits_ff operand
     */
    virtual unsigned char get_bits_ff_u8_19_20(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the cccc operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the cccc operand
     */
    virtual unsigned char get_cccc_u8_0_3(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the cccc operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the cccc operand
     */
    virtual unsigned char get_cccc_u8_17_20(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the fcbit operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the fcbit operand
     */
    virtual unsigned char get_fcbit_u8_17_19(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the fcond operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the fcond operand
     */
    virtual unsigned char get_fcond_u8_27_30(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the disp4 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp4 operand
     */
    virtual unsigned int get_disp4_u32_0_3(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the disp5 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp5 operand
     */
    virtual unsigned int get_disp5_u32_0_3_shift1(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the disp7 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp7 operand
     */
    virtual unsigned int get_disp7_u32_0_6(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the disp8 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp8 operand
     */
    virtual unsigned int get_disp8_u32_0_6_shift1(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the disp8 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp8 operand
     */
    virtual unsigned int get_disp8_u32_1_6_shift2(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the disp9 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp9 operand
     */
    virtual int get_disp9_s32_11_15_4_6_shift1(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp16 operand
     */
    virtual int get_disp16_s32_16_31(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp16 operand
     */
    virtual int get_disp16_s32_17_31_5(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp16 operand
     */
    virtual int get_disp16_s32_17_31_shift1(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the disp22 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp22 operand
     */
    virtual int get_disp22_s32_0_5_17_31_shift1(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the disp23 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp23 operand
     */
    virtual int get_disp23_s32_32_47_20_26(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the disp23 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp23 operand
     */
    virtual int get_disp23_s32_32_47_21_26_shift1(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the disp32 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp32 operand
     */
    virtual int get_disp32_s32_17_47_shift1(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the imm4 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm4 operand
     */
    virtual unsigned int get_imm4_u32_0_3(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the imm5 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm5 operand
     */
    virtual int get_imm5_s32_0_4(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the imm5 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm5 operand
     */
    virtual unsigned int get_imm5_u32_11_15(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the imm5 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm5 operand
     */
    virtual unsigned int get_imm5_u32_0_4(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the imm5 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm5 operand
     */
    virtual unsigned int get_imm5_u32_1_5_shift2(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the imm6 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm6 operand
     */
    virtual unsigned int get_imm6_u32_0_5_shift1(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the imm9 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm9 operand
     */
    virtual int get_imm9_s32_18_21_0_4(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the imm9 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm9 operand
     */
    virtual unsigned int get_imm9_u32_18_21_0_4(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the imm16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm16 operand
     */
    virtual int get_imm16_s32_16_31(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the imm16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm16 operand
     */
    virtual unsigned int get_imm16_u32_16_31(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the imm16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm16 operand
     */
    virtual unsigned int get_imm16_u32_16_31_shift16(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the imm32 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm32 operand
     */
    virtual int get_imm32_s32_16_47(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the list12 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the list12 operand
     */
    virtual unsigned short get_list12_u16_21_31_0(opcode_t opcode) = 0;
    /**
     * \brief Calculates a GR register group index value representing the value of the reg1 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the reg1 operand
     */
    virtual rh850::GR_index_t get_reg1_u8_0_4(opcode_t opcode) = 0;
    /**
     * \brief Calculates a GR register group index value representing the value of the reg1 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the reg1 operand
     */
    virtual rh850::GR_index_t get_reg1_u8_16_20(opcode_t opcode) = 0;
    /**
     * \brief Calculates a GR register group index value representing the value of the reg2 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the reg2 operand
     */
    virtual rh850::GR_index_t get_reg2_u8_0_4(opcode_t opcode) = 0;
    /**
     * \brief Calculates a GR register group index value representing the value of the reg2 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the reg2 operand
     */
    virtual rh850::GR_index_t get_reg2_u8_11_15(opcode_t opcode) = 0;
    /**
     * \brief Calculates a GR register group index value representing the value of the reg3 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the reg3 operand
     */
    virtual rh850::GR_index_t get_reg3_u8_27_31(opcode_t opcode) = 0;
    /**
     * \brief Calculates a GR register group index value representing the value of the reg4 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the reg4 operand
     */
    virtual rh850::GR_index_t get_reg4_u8_17_20_shift1(opcode_t opcode) = 0;
    /**
     * \brief Calculates a GR register group index value representing the value of the reg4 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the reg4 operand
     */
    virtual rh850::GR_index_t get_reg4_u8_17_20_23(opcode_t opcode) = 0;
    /**
     * \brief Calculates a GR register group index value representing the value of the reg1 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the reg1 operand
     */
    virtual rh850::GR_index_t get_reg1_u8_1_4_shift1(opcode_t opcode) = 0;
    /**
     * \brief Calculates a GR register group index value representing the value of the reg2 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the reg2 operand
     */
    virtual rh850::GR_index_t get_reg2_u8_12_15_shift1(opcode_t opcode) = 0;
    /**
     * \brief Calculates a GR register group index value representing the value of the reg3 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the reg3 operand
     */
    virtual rh850::GR_index_t get_reg3_u8_28_31_shift1(opcode_t opcode) = 0;
    /**
     * \brief Calculates a SR_SELID_0 register group index value representing the value of the regID operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the regID operand
     */
    virtual rh850::SR_SELID_0_index_t get_regID_u8_0_4(opcode_t opcode) = 0;
    /**
     * \brief Calculates a SR_SELID_0 register group index value representing the value of the regID operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the regID operand
     */
    virtual rh850::SR_SELID_0_index_t get_regID_u8_11_15(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the vector operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the vector operand
     */
    virtual unsigned char get_vector_u8_0_4(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the vector operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the vector operand
     */
    virtual unsigned char get_vector_u8_27_29_0_4(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the vector operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the vector operand
     */
    virtual unsigned char get_vector_u8_11_14(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the disp17 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp17 operand
     */
    virtual int get_disp17_s32_4_17_31_shift1(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the msb operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the msb operand
     */
    virtual unsigned int get_msb_u32_28_31(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the lsb operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the lsb operand
     */
    virtual unsigned int get_lsb_u32_27_17_19(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the selID operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the selID operand
     */
    virtual unsigned char get_selID_u8_27_31(opcode_t opcode) = 0;
    /**
     * \brief Calculates a GR register group index value representing the value of the rt operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the rt operand
     */
    virtual rh850::GR_index_t get_rt_u8_27_31(opcode_t opcode) = 0;
    /**
     * \brief Calculates a GR register group index value representing the value of the rh operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the rh operand
     */
    virtual rh850::GR_index_t get_rh_u8_0_4(opcode_t opcode) = 0;
    /**
     * \brief Calculates a GR register group index value representing the value of the reg1 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the reg1 operand
     */
    virtual rh850::GR_index_t get_reg1_u8_11_15(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the vreg1 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the vreg1 operand
     */
    virtual unsigned char get_vreg1_u8_0_4(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the vreg1 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the vreg1 operand
     */
    virtual unsigned char get_vreg1_u8_11_15(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the cacheop operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the cacheop operand
     */
    virtual unsigned char get_cacheop_u8_11_12_27_31(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the prefop operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the prefop operand
     */
    virtual unsigned char get_prefop_u8_27_31(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the imm10 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm10 operand
     */
    virtual unsigned short get_imm10_u16_27_31_0_4(opcode_t opcode) = 0;
    /**
     * \brief Calculates an integer value representing the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp16 operand
     */
    virtual unsigned int get_disp16_u32_17_31_shift1(opcode_t opcode) = 0;
    /**
     * @}
     */
     
    /**
     * \addtogroup GenerativeOperandExtractionOperations
     * @{
     */
    /**
     * \brief Generates code to calculate the value of the bit operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the bit operand
     */
    virtual fastiss::i8_t *gen_get_bit_u8_11_13(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the bits_ff operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the bits_ff operand
     */
    virtual fastiss::i8_t *gen_get_bits_ff_u8_19_20(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the cccc operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the cccc operand
     */
    virtual fastiss::i8_t *gen_get_cccc_u8_0_3(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the cccc operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the cccc operand
     */
    virtual fastiss::i8_t *gen_get_cccc_u8_17_20(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the fcbit operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the fcbit operand
     */
    virtual fastiss::i8_t *gen_get_fcbit_u8_17_19(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the fcond operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the fcond operand
     */
    virtual fastiss::i8_t *gen_get_fcond_u8_27_30(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the disp4 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp4 operand
     */
    virtual fastiss::i32_t *gen_get_disp4_u32_0_3(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the disp5 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp5 operand
     */
    virtual fastiss::i32_t *gen_get_disp5_u32_0_3_shift1(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the disp7 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp7 operand
     */
    virtual fastiss::i32_t *gen_get_disp7_u32_0_6(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the disp8 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp8 operand
     */
    virtual fastiss::i32_t *gen_get_disp8_u32_0_6_shift1(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the disp8 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp8 operand
     */
    virtual fastiss::i32_t *gen_get_disp8_u32_1_6_shift2(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the disp9 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp9 operand
     */
    virtual fastiss::i32_t *gen_get_disp9_s32_11_15_4_6_shift1(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp16 operand
     */
    virtual fastiss::i32_t *gen_get_disp16_s32_16_31(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp16 operand
     */
    virtual fastiss::i32_t *gen_get_disp16_s32_17_31_5(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp16 operand
     */
    virtual fastiss::i32_t *gen_get_disp16_s32_17_31_shift1(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the disp22 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp22 operand
     */
    virtual fastiss::i32_t *gen_get_disp22_s32_0_5_17_31_shift1(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the disp23 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp23 operand
     */
    virtual fastiss::i32_t *gen_get_disp23_s32_32_47_20_26(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the disp23 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp23 operand
     */
    virtual fastiss::i32_t *gen_get_disp23_s32_32_47_21_26_shift1(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the disp32 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp32 operand
     */
    virtual fastiss::i32_t *gen_get_disp32_s32_17_47_shift1(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the imm4 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm4 operand
     */
    virtual fastiss::i32_t *gen_get_imm4_u32_0_3(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the imm5 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm5 operand
     */
    virtual fastiss::i32_t *gen_get_imm5_s32_0_4(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the imm5 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm5 operand
     */
    virtual fastiss::i32_t *gen_get_imm5_u32_11_15(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the imm5 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm5 operand
     */
    virtual fastiss::i32_t *gen_get_imm5_u32_0_4(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the imm5 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm5 operand
     */
    virtual fastiss::i32_t *gen_get_imm5_u32_1_5_shift2(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the imm6 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm6 operand
     */
    virtual fastiss::i32_t *gen_get_imm6_u32_0_5_shift1(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the imm9 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm9 operand
     */
    virtual fastiss::i32_t *gen_get_imm9_s32_18_21_0_4(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the imm9 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm9 operand
     */
    virtual fastiss::i32_t *gen_get_imm9_u32_18_21_0_4(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the imm16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm16 operand
     */
    virtual fastiss::i32_t *gen_get_imm16_s32_16_31(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the imm16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm16 operand
     */
    virtual fastiss::i32_t *gen_get_imm16_u32_16_31(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the imm16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm16 operand
     */
    virtual fastiss::i32_t *gen_get_imm16_u32_16_31_shift16(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the imm32 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm32 operand
     */
    virtual fastiss::i32_t *gen_get_imm32_s32_16_47(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the list12 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the list12 operand
     */
    virtual fastiss::i16_t *gen_get_list12_u16_21_31_0(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the vector operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the vector operand
     */
    virtual fastiss::i8_t *gen_get_vector_u8_0_4(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the vector operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the vector operand
     */
    virtual fastiss::i8_t *gen_get_vector_u8_27_29_0_4(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the vector operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the vector operand
     */
    virtual fastiss::i8_t *gen_get_vector_u8_11_14(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the disp17 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp17 operand
     */
    virtual fastiss::i32_t *gen_get_disp17_s32_4_17_31_shift1(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the msb operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the msb operand
     */
    virtual fastiss::i32_t *gen_get_msb_u32_28_31(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the lsb operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the lsb operand
     */
    virtual fastiss::i32_t *gen_get_lsb_u32_27_17_19(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the selID operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the selID operand
     */
    virtual fastiss::i8_t *gen_get_selID_u8_27_31(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the vreg1 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the vreg1 operand
     */
    virtual fastiss::i8_t *gen_get_vreg1_u8_0_4(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the vreg1 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the vreg1 operand
     */
    virtual fastiss::i8_t *gen_get_vreg1_u8_11_15(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the cacheop operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the cacheop operand
     */
    virtual fastiss::i8_t *gen_get_cacheop_u8_11_12_27_31(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the prefop operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the prefop operand
     */
    virtual fastiss::i8_t *gen_get_prefop_u8_27_31(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the imm10 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm10 operand
     */
    virtual fastiss::i16_t *gen_get_imm10_u16_27_31_0_4(opcode_t opcode) = 0;
    /**
     * \brief Generates code to calculate the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp16 operand
     */
    virtual fastiss::i32_t *gen_get_disp16_u32_17_31_shift1(opcode_t opcode) = 0;
    /**
     * @}
     */
  };
}

#endif // _RH850_OPERAND_IF_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
