//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2020.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file continue_translator_base.h
 * \brief Defines the continue translator base class
 */

#ifndef _RH850G4_CONTINUE_TRANSLATOR_BASE_H_
#define _RH850G4_CONTINUE_TRANSLATOR_BASE_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850g4/translator_base.h"
#include <set>

//------------------------------------------------------------------------------
// Continue translator base class
//------------------------------------------------------------------------------

namespace rh850g4
{
  /**
   * \brief Base class providing continue code translation operations for the
   * Fast-ISS. The continue translator will continue to translate instructions
   * into the block until a termination condition is met. The base class is
   * responsible for the breakpoint management.
   */
  class continue_translator_base_t : public rh850g4::translator_base_t
  {
  public:
    /**
     * \defgroup ContinueTranslatorOperations Continue translator operations
     * @{
     * \defgroup BreakpointOperations Breakpoint operations
     * \defgroup ContinueTranslatorCalloutOperations Continue translator callout operations
     * @}
     */

    /**
     * \brief Continue translator base constructor. The translator has a
     * decoder for each translation phase.
     * \param code_gen A pointer to the code generator implementor
     * \param memory A pointer to the code memory access implementor
     * \param cpu_state A pointer to the cpu state implementor
     * \param pre_translate_loop_analysis_point A pointer to the pre-translate loop analysis point
     * \param post_translate_loop_analysis_point A pointer to the post-translate loop analysis point
     * \param post_fetch_analysis_point A pointer to the post-fetch analysis point
     * \param post_decode_analysis_point A pointer to the post-decode phase analysis point
       * \param post_execute_analysis_point A pointer to the post-execute phase analysis point
     * \param decoder0_instructions A pointer to a set of instructions for use by decoder0
     * \param decoder1_instructions A pointer to a set of instructions for use by decoder1
     */          
    continue_translator_base_t
    (
      fastiss::code_gen_if_t *code_gen,
      rh850g4::memory_if_t *memory,
      rh850g4::cpu_state_if_t *cpu_state,
      rh850g4::analysis_point_if* pre_translate_loop_analysis_point,
      rh850g4::analysis_point_if* post_translate_loop_analysis_point,
      rh850g4::analysis_point_if* post_fetch_analysis_point,
      rh850g4::analysis_point_if* post_decode_analysis_point,
      rh850g4::analysis_point_if* post_execute_analysis_point,
      rh850g4::instr_set_if_t *decoder0_instructions,
      rh850g4::instr_set_if_t *decoder1_instructions
    );
    /**
     * \brief Continue translator base destructor
     */          
    virtual ~continue_translator_base_t();

    /**
     * \addtogroup BreakpointOperations
     * @{
     */

    /**
     * \brief Inserts an address into the set of breakpoints.
     * \param bkpt_addr The program counter address to add as a breakpoint
     * \return True if the address was not already in the set of breakpoints,
     * false otherwise.
     */          
    bool insert_bkpt(unsigned int bkpt_addr);
    /**
     * \brief Clears an address from the set of breakpoints.
     * \param bkpt_addr The program counter address to remove as a breakpoint
     * \return True if the address was in the set of breakpoints, false
     * otherwise.
     */          
    bool clear_bkpt(unsigned int bkpt_addr);

  protected:
    /**
     * \brief Pointer to the breakpoint instruction. This instruction is passed through
     * the translation pipeline to indicate that a breakpoint address has been hit.
     */          
    rh850g4::instruction_base_if_t *m_bkpt_instr;
    
    /**
     * \brief Determines whether a breakpoint is at a specified address
     * \param bkpt_addr The program counter address to check for a breakpoint
     * \return True if a breakpoint exists at the specified address, false
     * otherwise
     */          
    bool is_bkpt_at_addr(unsigned int bkpt_addr);
    /**
     * @}
     */
     
  private:

    /**
     * \brief Set of the breakpoint addresses
     */          
    std::set<unsigned int> m_bkpt_set;
  };
}

#endif // _RH850G4_CONTINUE_TRANSLATOR_BASE_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
