//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2019.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file block_function_l2_cache.h
 * \brief Defines the block function level 2 cache class
 */

#ifndef _RH850_BLOCK_FUNCTION_L2_CACHE_H_
#define _RH850_BLOCK_FUNCTION_L2_CACHE_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "fastiss/block_function.h"
#include "fastiss/parallel_translator.h"

//------------------------------------------------------------------------------
// Class pre-declarations
//------------------------------------------------------------------------------

namespace fastiss
{
  class code_gen_if_t;
}

//------------------------------------------------------------------------------
// Direct mapped block l2 cache implementation
//------------------------------------------------------------------------------

namespace rh850
{
  /**
   * \brief The L2 block function cache class is a cache of code blocks based on
   * the 32-bit physical address of the first instruction in the block.
   * Translated blocks are inserted into this cache to ensure each code block
   * is translated once only. A lookup in this cache is guaranteed to succeed.
   * If a block does not exist in the cache then a new block will be created
   * and returned. If this cache grows above a threshold then the earliest
   * translated blocks are evicted and deleted as new blocks are added. The
   * contents of memory at physical addresses does not change when the MMU/MPU
   * configuration changes, so this cache is not flushed when the MMU/MPU
   * configuration changes. Flushes should only occur if the memory contents
   * changes, breakpoints are added or removed, a first or last watchpoint is
   * added or removed, or analyses listeners are modified. The L2 block function
   * cache provides operations to flush the cache and re-initialise, and look-up
   * a block function based on a physical address.
   */
  class block_function_l2_cache_t
  {
  public:
    /**
     * \brief L2 block function cache class constructor
     * \param interpreter_impl A pointer to the interpreter implementor
     * \param parallel_translator A pointer to the parallel translator
     * \param max_list_entries The maximum number of entries in a single linked
     * list of block functions. Once the size reaches this number the earliest
     * translated block is evicted and deleted. This must have a value greater
     * than 1.
     */
    block_function_l2_cache_t(fastiss::code_gen_if_t *interpreter_impl, fastiss::parallel_translator_t *parallel_translator, unsigned int max_list_entries);
    /**
     * \brief L2 Block function cache class destructor
     */
    ~block_function_l2_cache_t(void);

    /**
     * \brief Set the cache initialisation function.
     * \param initialise_fn A pointer to the block function to initialise new
     * cache entries with. The cache takes ownership of the initialisation
     * function and deletes it when the cache is destroyed.
     */
    void set_initialise_fn(fastiss::block_function_t *initialise_fn);
    
    /**
     * \brief Flush the cache, which returns it to the initialised state
     */
    void flush(void);
    
    /**
     * \brief Block function look-up operation - checks the 32-bit physical
     * address key against a masked tag and returns the corresponding block
     * function, or creates a new initialised cache entry if there was no match.
     * This method is used when looking up a function to be executed.
     * \param key The 32-bit physical address of the first instruction in
     * the block
     * \return The corresponding block function, or a new initialised block
     * function if there was no match
     */
    fastiss::block_function_t *lookup_function(unsigned int key);

    /**
     * \brief Flushes a block from the cache. If the block is not in the
     * cache then the cache is not modified.
     * \param key The 32-bit physical address of the first instruction in
     * the block.
     */
    void flush_function(unsigned int key);
    
  private:
    /**
     * \brief Type definition for a cache entry with a 32-bit physical
     * address key and a block function
     */
    struct entry_t
    {
      unsigned int m_key;
      fastiss::block_function_t *m_block_fn;
      entry_t *m_next_entry;
    };

    /**
     * \brief A pointer to the interpreter implementor
     */
    fastiss::code_gen_if_t *m_interpreter_impl;
    /**
     * \brief A pointer to the parallel translator
     */
    fastiss::parallel_translator_t *m_parallel_translator;
    /**
     * \brief The maximum number of entries in a single linked
     * list of block functions. Once the size reaches this number the earliest
     * translated block is evicted and deleted. This must have a value greater
     * than 1.
     */
    unsigned int m_max_list_entries;
    /**
     * \brief A pointer to the cache initialisation block function.
     */
    fastiss::block_function_t *m_initialise_fn;
    /**
     * \brief An array of block function cache entry pointers
     */
    rh850::block_function_l2_cache_t::entry_t* m_entries[4096];
  };
}

inline fastiss::block_function_t *rh850::block_function_l2_cache_t::lookup_function(unsigned int key)
{
  unsigned short index = (unsigned short)(40503 * key) >> 4;
  unsigned char number_of_entries = 0;
  rh850::block_function_l2_cache_t::entry_t *cache_entry = m_entries[index];  
  rh850::block_function_l2_cache_t::entry_t *first_entry = cache_entry;
  rh850::block_function_l2_cache_t::entry_t *previous_entry = NULL;
  
  if (cache_entry == NULL)
  {
    // The list is empty, create a new entry, insert it in the cache and return
    // the initialised block function
    entry_t* new_entry = new entry_t;
    m_entries[index] = new_entry;
    new_entry->m_key = key;
    new_entry->m_block_fn = new fastiss::block_function_t(m_interpreter_impl);
    new_entry->m_block_fn->m_block_fn = m_initialise_fn->m_block_fn;
    new_entry->m_next_entry = NULL;
    return new_entry->m_block_fn;
  }
  else
  {
    // Search if the entry exists in the list
    while (cache_entry->m_key != key)
    {
      number_of_entries++;
          
      if (cache_entry->m_next_entry == NULL)
      {
        if (number_of_entries >= m_max_list_entries)
        {
          // Max number of entries reached for the list, evict the last entry
          previous_entry->m_next_entry = NULL;
          m_parallel_translator->delete_block(cache_entry->m_block_fn);
          delete cache_entry;
        }
          
        // Need to create a new entry which will be inserted at the head of the list
        entry_t* new_entry = new entry_t;
        m_entries[index] = new_entry;          
        new_entry->m_key = key;
        new_entry->m_block_fn = new fastiss::block_function_t(m_interpreter_impl);
        new_entry->m_block_fn->m_block_fn = m_initialise_fn->m_block_fn;
        new_entry->m_next_entry = first_entry;
        return new_entry->m_block_fn;
      }
      // Check the next cache entry for the key
      previous_entry = cache_entry;
      cache_entry = cache_entry->m_next_entry;
    }
  }
  // If this point is reached the block was found
  return cache_entry->m_block_fn;
}

inline void rh850::block_function_l2_cache_t::flush_function(unsigned int key)
{
  unsigned short index = (unsigned short)(40503 * key) >> 4;
  rh850::block_function_l2_cache_t::entry_t *cache_entry = m_entries[index];  
  rh850::block_function_l2_cache_t::entry_t *previous_entry = NULL;

  while (cache_entry != NULL)
  {
    // Search if the entry exists in the list
    if (cache_entry->m_key == key)
    {
      // Found a matching block, pass it to the parallel translator to delete.
      m_parallel_translator->delete_block(cache_entry->m_block_fn);
        
      if (previous_entry == NULL)
      {
        // This is the first entry for the key
        m_entries[index] = cache_entry->m_next_entry;
        delete cache_entry;
      }
      else
      {
        // This is a subsequent cache entry for the key. Update the previous
        // entries link and delete this entry
        previous_entry->m_next_entry = cache_entry->m_next_entry;
        delete cache_entry;
      }
      return;
    }
    else
    {
      // Check the next cache entry for the key
      previous_entry = cache_entry;
      cache_entry = cache_entry->m_next_entry;
    }
  }
}

#endif // _RH850_BLOCK_FUNCTION_L2_CACHE_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
