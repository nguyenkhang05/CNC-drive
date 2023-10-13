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

/**
 * \file step_translator.h
 * \brief Defines the continue translator class
 */

#ifndef _RH850G4_STEP_TRANSLATOR_H_
#define _RH850G4_STEP_TRANSLATOR_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850g4/translator_base.h"
#include "rh850g4/instr_set_impl.h"
#include <string>

//------------------------------------------------------------------------------
// Class pre-declarations
//------------------------------------------------------------------------------

namespace rh850g4
{
  class operand_if_t;
  class cpu_state_if_t;
  class custom_if_t;
  class memory_if_t;
}

//------------------------------------------------------------------------------
// rh850g4 step translator definition
//------------------------------------------------------------------------------

namespace rh850g4
{
  /**
   * \brief Class providing step code translation operations for the
   * Fast-ISS. The step translator will translate a single instruction
   * into the block. Translated blocks are not inserted into a cache.
   */
  class step_translator_t : public rh850g4::translator_base_t
  {
  public:
    /**
     * \defgroup StepTranslatorOperations Step translator operations
     * @{
     * \defgroup StepTranslatorCalloutOperations Step translator callout operations
     * @}
     */

    /**
     * \brief Step translator constructor
     * \param code_gen A pointer to the code generator implementor
     * \param memory A pointer to the code memory access implementor
     * \param operand A pointer to the operand extraction implementor
     * \param cpu_state A pointer to the cpu state implementor
     * \param custom A pointer to the custom generator implementor
     * \param swdt_enable The mode if generate code for SWDT's PC detection
     * \param pre_translate_loop_analysis_point A pointer to the pre-translate loop analysis point
     * \param post_translate_loop_analysis_point A pointer to the post-translate loop analysis point
     * \param post_fetch_analysis_point A pointer to the post-fetch analysis point
     * \param post_decode_analysis_point A pointer to the post-decode phase analysis point
     * \param post_execute_analysis_point A pointer to the post-execute phase analysis point
     * \param translation_partition_mask Address mask used to enable the core to
     * determine the translation* partition base address when evaluating which
     * blocks should be flushed when a breakpoint is set/cleared or a write to
     * memory occurs. 
     */
    step_translator_t
    (
      fastiss::code_gen_if_t *code_gen,
      rh850g4::memory_if_t *memory,
      rh850g4::operand_if_t *operand,
      rh850g4::cpu_state_if_t *cpu_state,
      rh850g4::custom_if_t *custom,
      bool swdt_enable,
      rh850g4::analysis_point_if* pre_translate_loop_analysis_point,
      rh850g4::analysis_point_if* post_translate_loop_analysis_point,
      rh850g4::analysis_point_if* post_fetch_analysis_point,
      rh850g4::analysis_point_if* post_decode_analysis_point,
      rh850g4::analysis_point_if* post_execute_analysis_point,
      unsigned int translation_partition_mask
    );
    /**
     * \brief Step translator destructor
     */          
    virtual ~step_translator_t();
  protected:
    /**
     * \addtogroup StepTranslatorCalloutOperations
     * @{
     */

    /**
     * \brief Callout function for the step translator that is called prior to
     * starting the translation of a block. This callout is used to setup the block
     * as a multiple exit point block.
     * \param block_fn The block into which the translated code is inserted
     */
    void pre_translate_loop_callout(fastiss::block_function_t *block_fn);
    /**
     * \brief Callout function for the step translator that is called after
     * completing the translation of a block. This callout is used to complete the block
     * as a multiple exit point block. This callout also performs the watchpoint checks
     * for the translated block.
     * \param block_fn The block into which the translated code is inserted
     */
    void post_translate_loop_callout(fastiss::block_function_t *block_fn);
    /**
     * \brief Callout function called after each instruction has completed
     * translation of the fetch phase.
     * This callout is responsible for:
       - Determining how to handle opcodes that do not decode to a known
         instruction.
     * \param block_fn The block into which the translated code is inserted
     */
    virtual void post_fetch_callout(fastiss::block_function_t *block_fn);
    /**
     * \brief Callout function called after each instruction has completed
     * translation of the decode phase.
     * \param block_fn The block into which the translated code is inserted
     */
    virtual void post_decode_callout(fastiss::block_function_t *block_fn);
    /**
     * \brief Callout function called after each instruction has completed
     * translation of the execute phase.
     * This callout is responsible for checking if a watchpoint has been hit.
     * \param block_fn The block into which the translated code is inserted
     */
    virtual void post_execute_callout(fastiss::block_function_t *block_fn);
    /**
     * @}
     */

  private:
    /**
     * \brief A set of instruction implementors for decoder0.
     * Note that there is a separate decoder for each translation phase
     */
    rh850g4::instr_set_impl_t m_decoder0_instructions;
    /**
     * \brief A set of instruction implementors for decoder1.
     * Note that there is a separate decoder for each translation phase
     */
    rh850g4::instr_set_impl_t m_decoder1_instructions;
    /**
     * \brief A pointer to the custom generator implementor
     */          
    rh850g4::custom_if_t *m_custom;
    /**
     * \brief A pointer to the operand extraction implementor
     */
    rh850g4::operand_if_t *m_operand;
    /**
     * \brief Boolean flag to indicate whether an instruction in the block
     * accessed memory and a watchpoint check is required in the exit block.
     */
     bool m_check_watchpoint_hit_at_exit;
    /**
     * \brief Pointer to the undefined instruction. This instruction is passed through
     * the translation pipeline to indicate that an undefined instruction has been hit.
     */          
    rh850g4::instruction_base_impl_t *m_undef_instr;
    /**
     * \brief Address mask used to enable the core to determine the translation
     * partition base address when evaluating which blocks should be flushed when
     * a breakpoint is set/cleared or a write to memory occurs.
     */
    unsigned int m_translation_partition_mask;
    /**
     * \brief The mode if generate code for SWDT's PC detection
     */          
    bool m_swdt_enable;
  };
}

#endif // _RH850G4_STEP_TRANSLATOR_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
