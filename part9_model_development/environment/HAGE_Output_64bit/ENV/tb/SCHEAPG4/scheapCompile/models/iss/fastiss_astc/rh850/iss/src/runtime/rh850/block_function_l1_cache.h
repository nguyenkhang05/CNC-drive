//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2019.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file block_function_l1_cache.h
 * \brief Defines the block function level 1 cache class
 */

#ifndef _RH850_BLOCK_FUNCTION_L1_CACHE_H_
#define _RH850_BLOCK_FUNCTION_L1_CACHE_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "fastiss/block_function_l1_cache_base.h"

#include "rh850/block_function_l2_cache.h"
#include "rh850/memory_if.h"

//------------------------------------------------------------------------------
// Class pre-declarations
//------------------------------------------------------------------------------

namespace fastiss
{
  class block_function_t;
}

//------------------------------------------------------------------------------
// Direct mapped block l1 cache implementation
//------------------------------------------------------------------------------

namespace rh850
{
  /**
   * \brief The L1 block function cache class is a cache of code blocks based on
   * the 32-bit virtual address of the first instruction in the block. A lookup
   * in this cache is guaranteed to succeed. If a block does not exist in the
   * cache then a lookup is performed in the L2 block function cache and
   * returned. This cache is non-evicting. The mapping from virtual address to
   * translated block is changed whenever the MMU/MPU configuration changes, so
   * this cache is flushed when the MMU/MPU configuration changes. This cache
   * does not own or create any translated blocks so flushing does not cause any
   * translated blocks to be deleted. The L1 block function cache provides
   * operations to flush the cache, and look-up a block function based on a
   * virtual address.
   */
  class block_function_l1_cache_t : public fastiss::block_function_l1_cache_base_t
  {
  public:
    /**
     * \brief L1 block function cache class constructor
     * \param memory_impl A pointer to the memory generator implementor
     * \param fn_l2_cache A pointer to the L2 block function cache
     */
    block_function_l1_cache_t(rh850::memory_if_t *memory_impl, rh850::block_function_l2_cache_t *fn_l2_cache);
    /**
     * \brief L1 Block function cache class destructor
     */
    ~block_function_l1_cache_t(void);

    /**
     * \brief Set the fetch exception function.
     * \param fetch_exception_fn A pointer to the block function that is executed
     * when a fetch fails. The cache takes ownership of the fetch exception
     * function and deletes it when the cache is destroyed.
     */
    void set_fetch_exception_fn(fastiss::block_function_t *fetch_exception_fn);

    /**
     * \brief Flushes the entire cache, no translated blocks are deleted. This
     * cache should be flushed whenever the MMU/MPU configuration changes.
     */
    void flush(void);
    
    /**
     * \brief Block function look-up operation - checks the 32-bit virtual
     * address key against a masked tag and returns the corresponding block
     * function, or looks up the block in the L2 block function cache if there
     * was no match. This method is used when looking up a function to be
     * executed.
     * \param key The 32-bit virtual address of the first instruction in
     * the block
     * \return The corresponding block function
     */
    fastiss::block_function_t *lookup_function(unsigned int key);

    /**
     * \brief Flushes one or more blocks from the cache. If the block is not in the
     * cache then the cache is not modified.
     * \param key The 32-bit virtual address of the first instruction in
     * the block.
     */
    void flush_function(unsigned int key);
    
  private:

    typedef block_function_l1_cache_base_t::entry_t entry_t;

    /**
     * \brief A pointer to the memory generator implementor
     */
    rh850::memory_if_t *m_memory_impl;
    
    /**
     * \brief A pointer to the L2 block function cache
     */
    rh850::block_function_l2_cache_t* m_fn_l2_cache;
    
    /**
     * \brief A pointer to the fetch exception block function.
     */
    fastiss::block_function_t *m_fetch_exception_fn;
  };
}

inline fastiss::block_function_t *rh850::block_function_l1_cache_t::lookup_function(unsigned int key)
{
  rh850::block_function_l1_cache_t::entry_t *cache_entry = find_entry(key);

  // If the entry is not already in the cache then replace the existing entry
  if (cache_entry->m_key != key)
  {
    cache_entry->m_key = key;
    
    unsigned int physical_address = 0;
    if (m_memory_impl->calculate_physical_address(FETCH_ACCESS, m_memory_impl->calculate_effective_address(FETCH_ACCESS, key), &physical_address, NULL, NULL))
    {
      // Physical address could not be calculated, return the exception block
      // as the block function
      cache_entry->m_block_fn = m_fetch_exception_fn;
    }
    else
    {
      // Look up the block in the L2 blocks function cache
      cache_entry->m_block_fn = m_fn_l2_cache->lookup_function(physical_address);
    }
  }
  // Return the entry
  return cache_entry->m_block_fn;
}

inline void rh850::block_function_l1_cache_t::flush_function(unsigned int key)
{
  flush_entry(key);
}

#endif // _RH850_BLOCK_FUNCTION_L1_CACHE_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
