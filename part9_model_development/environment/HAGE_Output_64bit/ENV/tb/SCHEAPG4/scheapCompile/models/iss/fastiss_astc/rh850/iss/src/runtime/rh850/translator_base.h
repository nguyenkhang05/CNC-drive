//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2019.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file translator_base.h
 * \brief Defines the translator base class
 */

#ifndef _RH850_TRANSLATOR_BASE_H_
#define _RH850_TRANSLATOR_BASE_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <string>
#include "rh850/opcode.h"
#include <map>
#include <set>

//------------------------------------------------------------------------------
// Class pre-declarations
//------------------------------------------------------------------------------

namespace fastiss
{
  class block_function_t;
  class code_gen_if_t;
}

namespace rh850
{
  class memory_if_t;
  class cpu_state_if_t;
  class decoder_if_t;
  class rh850e3_decoder_t;
  class instr_set_if_t;
  class instruction_base_if_t;
  class analysis_point_if;
}

//------------------------------------------------------------------------------
// Translator base class
//------------------------------------------------------------------------------

namespace rh850
{
  /**
   * \brief Indices for individual instruction sets in the rh850
   */
  typedef enum
  {
    INSTRUCTION_SET_INDEX_RH850E3
  } instruction_set_index_t;

  /**
   * \brief Base class providing code translation operations for the Fast-ISS.
   * The operations provide a means to translate to blocks of generated code.
   */
  class translator_base_t
  {
  public:
    /**
     * \defgroup TranslatorOperations Translator operations
     * @{
     * \defgroup TranslationOperations Translation operations
     * \defgroup DerivedTranslatorCalloutOperations Derived translator callout operations
     * @}
     */

    /**
     * \brief Translator base constructor
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
    translator_base_t
    (
      fastiss::code_gen_if_t *code_gen,
      rh850::memory_if_t *memory,
      rh850::cpu_state_if_t *cpu_state,
      rh850::analysis_point_if* pre_translate_loop_analysis_point,
      rh850::analysis_point_if* post_translate_loop_analysis_point,
      rh850::analysis_point_if* post_fetch_analysis_point,
      rh850::analysis_point_if* post_decode_analysis_point,
      rh850::analysis_point_if* post_execute_analysis_point,
      rh850::instr_set_if_t *decoder0_instructions,
      rh850::instr_set_if_t *decoder1_instructions
    );
    /**
     * \brief Translator base destructor
     */          
    virtual ~translator_base_t();

    /**
     * \brief Returns a pointer to a set containing all block start addresses
     * that have been translated into the specified translation partition. The
     * translation partition is reset as a result of the call. It is the callers
     * responsibility to delete the list.
     * \param partition The start address of the translation partition.
     * \return A pointer to the set containing all block start addresses
     * that have been translated into the translation partition. A NULL pointer is
     * returned if no blocks have been translated in the specified partition.
     */    
    std::set<unsigned int>* get_blocks_in_translation_partition(unsigned int partition);
    
    /**
     * \addtogroup TranslationOperations
     * @{
     */

    /**
     * \brief Translates instructions starting from the current program counter
     * address into a block of code. On completing the translation, the block of
     * code is optimised and generated into machine code.
     * \param block_fn The block into which the translated code is inserted
     */          
    virtual void* translate_block(fastiss::block_function_t *block_fn);

    /**
     * \brief Sets the currently active instruction set. The decoders for this
     * instruction set will be used for the translation of future blocks.
     * \param index Index value of the required instruction set
     */          
    virtual void set_active_instruction_set(rh850::instruction_set_index_t index);
    
    /**
     * @}
     */

    /**
     * \addtogroup InstrumentationQueryAPI
     * @{
     */
    /**
     * \brief Reads the opcode of the instruction currently in the
     * decode translation phase.
     * \return Opcode value
     */
    virtual rh850::opcode_t get_decode_opcode();
    /**
     * \brief Reads the opcode of the instruction currently in the
     * execute translation phase.
     * \return Opcode value
     */
    virtual rh850::opcode_t get_execute_opcode();
    /**
     * \brief Reads the value of PC for the instruction currently in the
     * decode translation phase.
     * \return PC value
     */
    virtual unsigned int get_decode_PC();
    /**
     * \brief Reads the value of PC for the instruction currently in the
     * execute translation phase.
     * \return PC value
     */
    virtual unsigned int get_execute_PC();
    /**
     * \brief Gets a disassembly string for the instruction currently in the
     * decode translation phase.
     * \return Disassembly string
     */
    virtual std::string get_decode_disass();
    /**
     * \brief Gets a disassembly string for the instruction currently in the
     * execute translation phase.
     * \return Disassembly string
     */
    virtual std::string get_execute_disass();
    /**
     * \brief Reads the value of the 'terminates_block' attribute of the instruction currently in the
     * decode translation phase.
     * \return Boolean representing value of instruction 'terminates_block' attribute
     */
    virtual bool get_decode_terminates_block();
    /**
     * \brief Reads the value of the 'cycle_count' attribute of the instruction currently in the
     * decode translation phase.
     * \return Unsigned 32-bit integer representing value of instruction 'cycle_count' attribute
     */
    virtual unsigned int get_decode_cycle_count();
    /**
     * \brief Reads the value of the 'terminates_block' attribute of the instruction currently in the
     * execute translation phase.
     * \return Boolean representing value of instruction 'terminates_block' attribute
     */
    virtual bool get_execute_terminates_block();
    /**
     * \brief Reads the value of the 'cycle_count' attribute of the instruction currently in the
     * execute translation phase.
     * \return Unsigned 32-bit integer representing value of instruction 'cycle_count' attribute
     */
    virtual unsigned int get_execute_cycle_count();
    /**
     * \brief Reads the length in bytes of the instruction currently in the
     * decode translation phase.
     * \return Integer value representing length of the instruction
     */
    virtual unsigned int get_decode_length();
    /**
     * \brief Reads the length in bytes of the instruction currently in the
     * execute translation phase.
     * \return Integer value representing length of the instruction
     */
    virtual unsigned int get_execute_length();
    /**
     * \brief Indicates whether the instruction currently in the
     * decode translation phase is valid.
     * \return Boolean indicating whether the instruction is valid
     */
    virtual bool is_decode_valid();
    /**
     * \brief Indicates whether the instruction currently in the
     * execute translation phase is valid.
     * \return Boolean indicating whether the instruction is valid
     */
    virtual bool is_execute_valid();
    /**
     * @}
     */

  protected:
    /**
     * \brief A pointer to the pre-translate loop analysis point.
     */
    rh850::analysis_point_if* m_pre_translate_loop_analysis_point;
    /**
     * \brief A pointer to the post-translate loop analysis point.
     */
    rh850::analysis_point_if* m_post_translate_loop_analysis_point;
    /**
     * \brief A pointer to the post-fetch analysis point.
     */
    rh850::analysis_point_if* m_post_fetch_analysis_point;
    /**
     * \brief A pointer to the post-decode analysis point.
     */
    rh850::analysis_point_if* m_post_decode_analysis_point;
    /**
     * \brief A pointer to the post-execute analysis point.
     */
    rh850::analysis_point_if* m_post_execute_analysis_point;

    /**
     * \addtogroup DerivedTranslatorCalloutOperations
     * @{
     */

    /**
     * \brief Callout function for derived translators that is called prior to
     * starting the translation of a block
     * \param block_fn The block into which the translated code is inserted
     */          
    virtual void pre_translate_loop_callout(fastiss::block_function_t *block_fn) = 0;
    /**
     * \brief Callout function for derived translators that is called after
     * completing the translation of a block
     * \param block_fn The block into which the translated code is inserted
     */          
    virtual void post_translate_loop_callout(fastiss::block_function_t *block_fn) = 0;
    /**
     * \brief Callout function for derived translators that is called after
     * completion of each instructions fetch phase
     * \param block_fn The block into which the translated code is inserted
     */
    virtual void post_fetch_callout(fastiss::block_function_t *block_fn) = 0;
    /**
     * \brief Callout function for derived translators that is called after
     * completion of each instructions decode phase
     * \param block_fn The block into which the translated code is inserted
     */
    virtual void post_decode_callout(fastiss::block_function_t *block_fn) = 0;
    /**
     * \brief Callout function for derived translators that is called after
     * completion of each instructions execute phase
     * \param block_fn The block into which the translated code is inserted
     */
    virtual void post_execute_callout(fastiss::block_function_t *block_fn) = 0;
    /**
     * \brief Operation of fetch the instruction opcode at the specified address
     * \param addr Address at which to fetch opcode
     * \return Instruction opcode found at specified address
     */
    virtual rh850::opcode_t fetch_opcode_callout(unsigned int addr);
    
    /**
     * @}
     */

    /**
     * \brief A pointer to the code generator implementor
     */          
    fastiss::code_gen_if_t *m_code_gen;

    /**
     * \brief A pointer to the memory access implementor
     */          
    rh850::memory_if_t *m_memory;

    /**
     * \brief A pointer to the cpu state implementor
     */          
    rh850::cpu_state_if_t *m_cpu_state;

    /**
     * \brief Translation control flag to indicate whether translation is
     * complete for the block
     */          
    bool m_translation_complete;
    /**
     * \brief Translation control flag to indicate that no further instructions
     * should be fetched for translation and only instructions remaining in the
     * translation pipeline should be completed for the block
     */          
    bool m_fetches_enabled;

    /**
     * \brief The address of the first instruction in the block being translated.
     */              
    unsigned int m_block_start_address;
    /**
     * \brief The translation partition start address that the block being
     * translated falls within.
     */              
    unsigned int m_current_translation_partition;
    /**
     * \brief A mapping from translation partition start addresses to a pointer to
     * the set of block start addresses that fall within the partition.
     */              
    std::map<unsigned int,std::set<unsigned int>*> m_translation_partitions;
    
      /**
     * \brief Array of instruction decoders, one for each translation phase.
     * This array contains the instruction decoders for the currently active
     * instruction set. When the currently active instruction set changes the
     * pointers in this array are changed.
     */
    rh850::decoder_if_t *m_decoder[2];
  
  private:
    /**
     * \brief Array of instruction decoders for the rh850e3 instruction set,
     * one for each translation phase.
     */
    rh850::rh850e3_decoder_t *m_rh850e3_decoder[2];
  
  protected:
    /**
     * \brief Pointer to the NULL instruction. This instruction is passed through
     * the translation pipeline to indicate that nothing is required to be done
     * in the relevant translation phase
     */          
    rh850::instruction_base_if_t *m_null_instruction;
    /**
     * \brief Pointer to the termination instruction. This instruction is passed
     * through the translation pipeline to indicate that translation is to be
     * terminated once the instruction reaches the final execution phase
     */          
    rh850::instruction_base_if_t *m_terminate_instruction;

    /**
     * \brief Array containing the PC register value of the instruction in
     * each translation phase
     */
    unsigned int m_PC_value[2];
    /**
     * \brief Array containing the opcode of the instruction in each translation
     * phase
     */
    rh850::opcode_t m_opcode[2];
    /**
     * \brief Array containing the instruction in each translation phase
     */
    rh850::instruction_base_if_t *m_instruction[2];

    /**
     * \brief Index of the decode translation phase
     */
    unsigned int m_decode_phase_index;
    /**
     * \brief Index of the execute translation phase
     */
    unsigned int m_execute_phase_index;
  };
}

#endif // _RH850_TRANSLATOR_BASE_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
