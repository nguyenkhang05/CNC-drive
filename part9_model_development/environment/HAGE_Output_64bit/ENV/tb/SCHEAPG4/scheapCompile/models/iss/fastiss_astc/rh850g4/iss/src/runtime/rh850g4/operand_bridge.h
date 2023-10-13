//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2020.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file operand_bridge.h
 * \brief Defines the rh850g4 operand extraction bridge class
 */

#ifndef _RH850G4_OPERAND_BRIDGE_H_
#define _RH850G4_OPERAND_BRIDGE_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850g4/operand_if.h"

//------------------------------------------------------------------------------
// rh850g4 operand extraction interface bridge definition
//------------------------------------------------------------------------------

/**
 * \brief Contains rh850g4 specific components
 */
namespace rh850g4
{
  /**
   * \brief Class implementing the operand extraction interface that provides
   * a bridge to another implementation of operand extraction interface.
   * 
   * Essentially this class re-directs all operations called on it to
   * the corresponding operation of the specified implementation. This
   * is used to ensure that instances of the many classes derived from
   * this class are able to share a single underlying operand extraction
   * implementation.
   * 
   * Note that the generative aspects of this interface are protected in
   * order to ensure that they are not available to helper functions.
   */
  class operand_bridge_t : public operand_if_t
  {
  public:
    /**
     * \brief Operand extraction bridge constructor
     * \param operand_impl Handle to operand extraction implementation to
     * which calls on the created operand extraction interface will be
     * directed.
     */
    operand_bridge_t(operand_if_t *operand_impl);
    /**
     * \brief Operand extraction bridge destructor
     */
    virtual ~operand_bridge_t();

    // Immediate operand value extraction operations
    /**
     * \brief Calculates an integer value representing the value of the bit operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the bit operand
     */
    unsigned char get_bit_u8_11_13(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the bits_ff operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the bits_ff operand
     */
    unsigned char get_bits_ff_u8_19_20(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the cccc operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the cccc operand
     */
    unsigned char get_cccc_u8_0_3(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the cccc operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the cccc operand
     */
    unsigned char get_cccc_u8_17_20(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the fcbit operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the fcbit operand
     */
    unsigned char get_fcbit_u8_17_19(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the fcond operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the fcond operand
     */
    unsigned char get_fcond_u8_27_30(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the disp4 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp4 operand
     */
    unsigned int get_disp4_u32_0_3(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the disp5 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp5 operand
     */
    unsigned int get_disp5_u32_0_3_shift1(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the disp7 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp7 operand
     */
    unsigned int get_disp7_u32_0_6(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the disp8 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp8 operand
     */
    unsigned int get_disp8_u32_0_6_shift1(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the disp8 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp8 operand
     */
    unsigned int get_disp8_u32_1_6_shift2(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the disp9 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp9 operand
     */
    int get_disp9_s32_11_15_4_6_shift1(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp16 operand
     */
    int get_disp16_s32_16_31(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp16 operand
     */
    int get_disp16_s32_17_31_5(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp16 operand
     */
    int get_disp16_s32_17_31_shift1(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the disp22 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp22 operand
     */
    int get_disp22_s32_0_5_17_31_shift1(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the disp23 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp23 operand
     */
    int get_disp23_s32_32_47_20_26(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the disp23 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp23 operand
     */
    int get_disp23_s32_32_47_21_26_shift1(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the disp32 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp32 operand
     */
    int get_disp32_s32_17_47_shift1(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the imm4 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm4 operand
     */
    unsigned int get_imm4_u32_0_3(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the imm5 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm5 operand
     */
    int get_imm5_s32_0_4(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the imm5 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm5 operand
     */
    unsigned int get_imm5_u32_11_15(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the imm5 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm5 operand
     */
    unsigned int get_imm5_u32_0_4(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the imm5 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm5 operand
     */
    unsigned int get_imm5_u32_1_5_shift2(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the imm6 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm6 operand
     */
    unsigned int get_imm6_u32_0_5_shift1(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the imm9 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm9 operand
     */
    int get_imm9_s32_18_21_0_4(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the imm9 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm9 operand
     */
    unsigned int get_imm9_u32_18_21_0_4(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the imm16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm16 operand
     */
    int get_imm16_s32_16_31(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the imm16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm16 operand
     */
    unsigned int get_imm16_u32_16_31(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the imm16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm16 operand
     */
    unsigned int get_imm16_u32_16_31_shift16(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the imm32 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm32 operand
     */
    int get_imm32_s32_16_47(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the list12 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the list12 operand
     */
    unsigned short get_list12_u16_21_31_0(opcode_t opcode);
    /**
     * \brief Calculates a GR register group index value representing the value of the reg1 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the reg1 operand
     */
    rh850g4::GR_index_t get_reg1_u8_0_4(opcode_t opcode);
    /**
     * \brief Calculates a GR register group index value representing the value of the reg1 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the reg1 operand
     */
    rh850g4::GR_index_t get_reg1_u8_16_20(opcode_t opcode);
    /**
     * \brief Calculates a GR register group index value representing the value of the reg2 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the reg2 operand
     */
    rh850g4::GR_index_t get_reg2_u8_0_4(opcode_t opcode);
    /**
     * \brief Calculates a GR register group index value representing the value of the reg2 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the reg2 operand
     */
    rh850g4::GR_index_t get_reg2_u8_11_15(opcode_t opcode);
    /**
     * \brief Calculates a GR register group index value representing the value of the reg3 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the reg3 operand
     */
    rh850g4::GR_index_t get_reg3_u8_27_31(opcode_t opcode);
    /**
     * \brief Calculates a GR register group index value representing the value of the reg4 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the reg4 operand
     */
    rh850g4::GR_index_t get_reg4_u8_17_20_shift1(opcode_t opcode);
    /**
     * \brief Calculates a GR register group index value representing the value of the reg4 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the reg4 operand
     */
    rh850g4::GR_index_t get_reg4_u8_17_20_23(opcode_t opcode);
    /**
     * \brief Calculates a GR register group index value representing the value of the reg1 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the reg1 operand
     */
    rh850g4::GR_index_t get_reg1_u8_1_4_shift1(opcode_t opcode);
    /**
     * \brief Calculates a GR register group index value representing the value of the reg2 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the reg2 operand
     */
    rh850g4::GR_index_t get_reg2_u8_12_15_shift1(opcode_t opcode);
    /**
     * \brief Calculates a GR register group index value representing the value of the reg3 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the reg3 operand
     */
    rh850g4::GR_index_t get_reg3_u8_28_31_shift1(opcode_t opcode);
    /**
     * \brief Calculates a SR_SELID_0 register group index value representing the value of the regID operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the regID operand
     */
    rh850g4::SR_SELID_0_index_t get_regID_u8_0_4(opcode_t opcode);
    /**
     * \brief Calculates a SR_SELID_0 register group index value representing the value of the regID operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the regID operand
     */
    rh850g4::SR_SELID_0_index_t get_regID_u8_11_15(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the vector operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the vector operand
     */
    unsigned char get_vector_u8_0_4(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the vector operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the vector operand
     */
    unsigned char get_vector_u8_27_29_0_4(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the vector operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the vector operand
     */
    unsigned char get_vector_u8_11_14(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the disp17 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp17 operand
     */
    int get_disp17_s32_4_17_31_shift1(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the msb operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the msb operand
     */
    unsigned int get_msb_u32_28_31(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the lsb operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the lsb operand
     */
    unsigned int get_lsb_u32_27_17_19(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the selID operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the selID operand
     */
    unsigned char get_selID_u8_27_31(opcode_t opcode);
    /**
     * \brief Calculates a GR register group index value representing the value of the rt operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the rt operand
     */
    rh850g4::GR_index_t get_rt_u8_27_31(opcode_t opcode);
    /**
     * \brief Calculates a GR register group index value representing the value of the rh operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the rh operand
     */
    rh850g4::GR_index_t get_rh_u8_0_4(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the cacheop operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the cacheop operand
     */
    unsigned char get_cacheop_u8_11_12_27_31(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the prefop operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the prefop operand
     */
    unsigned char get_prefop_u8_27_31(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the imm10 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm10 operand
     */
    unsigned short get_imm10_u16_27_31_0_4(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp16 operand
     */
    unsigned int get_disp16_u32_17_31_shift1(opcode_t opcode);
    /**
     * \brief Calculates a WR register group index value representing the value of the wreg1 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the wreg1 operand
     */
    rh850g4::WR_index_t get_wreg1_u8_0_4(opcode_t opcode);
    /**
     * \brief Calculates a WR register group index value representing the value of the wreg2 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the wreg2 operand
     */
    rh850g4::WR_index_t get_wreg2_u8_11_15(opcode_t opcode);
    /**
     * \brief Calculates a WR register group index value representing the value of the wreg2 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the wreg2 operand
     */
    rh850g4::WR_index_t get_wreg2_u8_43_47(opcode_t opcode);
    /**
     * \brief Calculates a WR register group index value representing the value of the wreg3 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the wreg3 operand
     */
    rh850g4::WR_index_t get_wreg3_u8_27_31(opcode_t opcode);
    /**
     * \brief Calculates a WR register group index value representing the value of the wreg4 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the wreg4 operand
     */
    rh850g4::WR_index_t get_wreg4_u8_32_36(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the fcond operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the fcond operand
     */
    unsigned char get_fcond_u8_17_20(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp16 operand
     */
    int get_disp16_s32_34_47_shift2(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp16 operand
     */
    int get_disp16_s32_35_47_shift3(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the disp16 operand
     */
    int get_disp16_s32_36_47_shift4(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the imm1 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm1 operand
     */
    unsigned int get_imm1_u32_21(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the imm2 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm2 operand
     */
    unsigned int get_imm2_u32_0_1(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the imm2 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm2 operand
     */
    unsigned int get_imm2_u32_21_22(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the imm3 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm3 operand
     */
    unsigned int get_imm3_u32_27_29(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the imm4 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm4 operand
     */
    unsigned int get_imm4_u32_21_24(opcode_t opcode);
    /**
     * \brief Calculates an integer value representing the value of the imm12 operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the imm12 operand
     */
    unsigned int get_imm12_u32_21_32_42(opcode_t opcode);
    /**
     * \brief Calculates a SR_SELID_5 register group index value representing the value of the et operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the et operand
     */
    rh850g4::SR_SELID_5_index_t get_et_u8_27_31(opcode_t opcode);
    /**
     * \brief Calculates a SR_SELID_5 register group index value representing the value of the eh operand for a given opcode value
     * \param opcode Opcode value
     * \return Value of the eh operand
     */
    rh850g4::SR_SELID_5_index_t get_eh_u8_11_14(opcode_t opcode);

  protected:
    // Generative operand value extraction operations
    /**
     * \brief Generates code to calculate the value of the bit operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the bit operand
     */
    fastiss::i8_t *gen_get_bit_u8_11_13(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the bits_ff operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the bits_ff operand
     */
    fastiss::i8_t *gen_get_bits_ff_u8_19_20(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the cccc operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the cccc operand
     */
    fastiss::i8_t *gen_get_cccc_u8_0_3(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the cccc operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the cccc operand
     */
    fastiss::i8_t *gen_get_cccc_u8_17_20(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the fcbit operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the fcbit operand
     */
    fastiss::i8_t *gen_get_fcbit_u8_17_19(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the fcond operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the fcond operand
     */
    fastiss::i8_t *gen_get_fcond_u8_27_30(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the disp4 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp4 operand
     */
    fastiss::i32_t *gen_get_disp4_u32_0_3(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the disp5 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp5 operand
     */
    fastiss::i32_t *gen_get_disp5_u32_0_3_shift1(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the disp7 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp7 operand
     */
    fastiss::i32_t *gen_get_disp7_u32_0_6(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the disp8 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp8 operand
     */
    fastiss::i32_t *gen_get_disp8_u32_0_6_shift1(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the disp8 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp8 operand
     */
    fastiss::i32_t *gen_get_disp8_u32_1_6_shift2(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the disp9 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp9 operand
     */
    fastiss::i32_t *gen_get_disp9_s32_11_15_4_6_shift1(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp16 operand
     */
    fastiss::i32_t *gen_get_disp16_s32_16_31(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp16 operand
     */
    fastiss::i32_t *gen_get_disp16_s32_17_31_5(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp16 operand
     */
    fastiss::i32_t *gen_get_disp16_s32_17_31_shift1(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the disp22 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp22 operand
     */
    fastiss::i32_t *gen_get_disp22_s32_0_5_17_31_shift1(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the disp23 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp23 operand
     */
    fastiss::i32_t *gen_get_disp23_s32_32_47_20_26(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the disp23 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp23 operand
     */
    fastiss::i32_t *gen_get_disp23_s32_32_47_21_26_shift1(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the disp32 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp32 operand
     */
    fastiss::i32_t *gen_get_disp32_s32_17_47_shift1(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the imm4 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm4 operand
     */
    fastiss::i32_t *gen_get_imm4_u32_0_3(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the imm5 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm5 operand
     */
    fastiss::i32_t *gen_get_imm5_s32_0_4(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the imm5 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm5 operand
     */
    fastiss::i32_t *gen_get_imm5_u32_11_15(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the imm5 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm5 operand
     */
    fastiss::i32_t *gen_get_imm5_u32_0_4(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the imm5 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm5 operand
     */
    fastiss::i32_t *gen_get_imm5_u32_1_5_shift2(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the imm6 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm6 operand
     */
    fastiss::i32_t *gen_get_imm6_u32_0_5_shift1(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the imm9 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm9 operand
     */
    fastiss::i32_t *gen_get_imm9_s32_18_21_0_4(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the imm9 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm9 operand
     */
    fastiss::i32_t *gen_get_imm9_u32_18_21_0_4(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the imm16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm16 operand
     */
    fastiss::i32_t *gen_get_imm16_s32_16_31(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the imm16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm16 operand
     */
    fastiss::i32_t *gen_get_imm16_u32_16_31(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the imm16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm16 operand
     */
    fastiss::i32_t *gen_get_imm16_u32_16_31_shift16(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the imm32 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm32 operand
     */
    fastiss::i32_t *gen_get_imm32_s32_16_47(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the list12 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the list12 operand
     */
    fastiss::i16_t *gen_get_list12_u16_21_31_0(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the vector operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the vector operand
     */
    fastiss::i8_t *gen_get_vector_u8_0_4(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the vector operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the vector operand
     */
    fastiss::i8_t *gen_get_vector_u8_27_29_0_4(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the vector operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the vector operand
     */
    fastiss::i8_t *gen_get_vector_u8_11_14(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the disp17 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp17 operand
     */
    fastiss::i32_t *gen_get_disp17_s32_4_17_31_shift1(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the msb operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the msb operand
     */
    fastiss::i32_t *gen_get_msb_u32_28_31(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the lsb operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the lsb operand
     */
    fastiss::i32_t *gen_get_lsb_u32_27_17_19(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the selID operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the selID operand
     */
    fastiss::i8_t *gen_get_selID_u8_27_31(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the cacheop operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the cacheop operand
     */
    fastiss::i8_t *gen_get_cacheop_u8_11_12_27_31(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the prefop operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the prefop operand
     */
    fastiss::i8_t *gen_get_prefop_u8_27_31(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the imm10 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm10 operand
     */
    fastiss::i16_t *gen_get_imm10_u16_27_31_0_4(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp16 operand
     */
    fastiss::i32_t *gen_get_disp16_u32_17_31_shift1(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the fcond operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the fcond operand
     */
    fastiss::i8_t *gen_get_fcond_u8_17_20(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp16 operand
     */
    fastiss::i32_t *gen_get_disp16_s32_34_47_shift2(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp16 operand
     */
    fastiss::i32_t *gen_get_disp16_s32_35_47_shift3(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the disp16 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the disp16 operand
     */
    fastiss::i32_t *gen_get_disp16_s32_36_47_shift4(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the imm1 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm1 operand
     */
    fastiss::i32_t *gen_get_imm1_u32_21(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the imm2 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm2 operand
     */
    fastiss::i32_t *gen_get_imm2_u32_0_1(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the imm2 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm2 operand
     */
    fastiss::i32_t *gen_get_imm2_u32_21_22(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the imm3 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm3 operand
     */
    fastiss::i32_t *gen_get_imm3_u32_27_29(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the imm4 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm4 operand
     */
    fastiss::i32_t *gen_get_imm4_u32_21_24(opcode_t opcode);
    /**
     * \brief Generates code to calculate the value of the imm12 operand for a given opcode value
     * \param opcode Opcode value
     * \return Object representing the value of the imm12 operand
     */
    fastiss::i32_t *gen_get_imm12_u32_21_32_42(opcode_t opcode);

    /**
     * \brief Handle to operand interface implementation
     */
    operand_if_t *m_operand_impl;
  };
}

#endif // _RH850G4_OPERAND_BRIDGE_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
