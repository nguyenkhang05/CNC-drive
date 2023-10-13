//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2019.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

#ifndef _RH850_MEMORY_IMPL_H_
#define _RH850_MEMORY_IMPL_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850/memory_if.h"
#include <list>
#include <map>

//------------------------------------------------------------------------------
// Class pre-declarations
//------------------------------------------------------------------------------

namespace fastiss
{
  class i1_t;
  class i8_t;
  class i16_t;
  class i32_t;
  class i64_t;
  class i8_ptr_t;
  class i16_ptr_t;
  class i32_ptr_t;
  class i64_ptr_t;
  class block_t;
  class code_gen_if_t;
}

namespace rh850
{
  class watchpoint_info_t;
  class analysis_callback_manager;
  class analysis_point_if;
}

//------------------------------------------------------------------------------
// Memory implementation class
//------------------------------------------------------------------------------

namespace rh850
{
  class memory_impl_t : public rh850::memory_if_t
  {
  protected:
    rh850::cpu_state_if_t *m_cpu_state;
    fastiss::code_gen_if_t* m_code_gen_impl;
    rh850::bus_interface_t *m_bus_interface;
  public:
    memory_impl_t();
    ~memory_impl_t();
    
    virtual void initialise
    (
      rh850::cpu_state_if_t *cpu_state,
      fastiss::code_gen_if_t* code_generator,
      rh850::bus_interface_t* bus_interface,
      bool tlm_response_error_as_error
    );
    void initialise_analysis_points(rh850::analysis_callback_manager *callback_manager);
    
    unsigned int add_watchpoint(unsigned int address, unsigned int bus_interface_id, unsigned int size, rh850::watchpoint_kind_t kind);
    void add_read_watchpoint( rh850::watchpoint_info_t *wp );
    void add_write_watchpoint( rh850::watchpoint_info_t *wp );

    void erase_watchpoint( unsigned int wp );
    void erase_read_watchpoint( rh850::watchpoint_info_t *wp );
    void erase_write_watchpoint( rh850::watchpoint_info_t *wp );
    
    void clear_all_watchpoints(void);
    unsigned int num_watchpoints(void);
        
    std::list< unsigned int > pending_watchpoints(void);
    
    void set_check_watchpoint_hit(void);
    void read_watchpoint_check(unsigned int addr, unsigned int bus_interface_id, unsigned int size);
    void write_watchpoint_check(unsigned int addr, unsigned int bus_interface_id, unsigned int size);
   
    void gen_read_watchpoint_check(fastiss::i32_t *addr, fastiss::i32_t *bus_interface_id, unsigned int size);
    void gen_write_watchpoint_check(fastiss::i32_t *addr, fastiss::i32_t *bus_interface_id, unsigned int size);

    unsigned int debugger_load(unsigned int addr, unsigned int size, unsigned char *data);
    unsigned int debugger_load_physical(unsigned int addr, unsigned int size, unsigned char *data);
    unsigned int debugger_store(unsigned int addr, unsigned int size, unsigned char *data);
    unsigned int debugger_store_physical(unsigned int addr, unsigned int size, unsigned char *data);
    unsigned char i8_fetch(unsigned int addr, bool *is_error = NULL);
    unsigned short i16_fetch(unsigned int addr, bool *is_error = NULL);
    unsigned int i32_fetch(unsigned int addr, bool *is_error = NULL);
    unsigned long long i64_fetch(unsigned int addr, bool *is_error = NULL);
    rh850::uint128_t i128_fetch(unsigned int addr, bool *is_error = NULL);
    unsigned int iN_fetch(unsigned int addr, unsigned int size, unsigned char *data);
    void issue_ignore(unsigned int addr, bool *is_error = NULL);
    void gen_issue_ignore(fastiss::i32_t* addr, fastiss::i1_ptr_t* is_error = NULL);
    fastiss::i8_t* gen_i8_load(fastiss::i32_t* addr, fastiss::i1_ptr_t* is_error = NULL, fastiss::block_t* success_block = NULL);
    fastiss::i16_t* gen_i16_load(fastiss::i32_t* addr, fastiss::i1_ptr_t* is_error = NULL, fastiss::block_t* success_block = NULL);
    fastiss::i32_t* gen_i32_load(fastiss::i32_t* addr, fastiss::i1_ptr_t* is_error = NULL, fastiss::block_t* success_block = NULL);
    fastiss::i64_t* gen_i64_load(fastiss::i32_t* addr, fastiss::i1_ptr_t* is_error = NULL, fastiss::block_t* success_block = NULL);
    void gen_i8_store(fastiss::i32_t* addr, fastiss::i8_t* data, fastiss::i1_ptr_t* is_error = NULL, fastiss::block_t* success_block = NULL);
    void gen_i16_store(fastiss::i32_t* addr, fastiss::i16_t* data, fastiss::i1_ptr_t* is_error = NULL, fastiss::block_t* success_block = NULL);
    void gen_i32_store(fastiss::i32_t* addr, fastiss::i32_t* data, fastiss::i1_ptr_t* is_error = NULL, fastiss::block_t* success_block = NULL);
    void gen_i64_store(fastiss::i32_t* addr, fastiss::i64_t* data, fastiss::i1_ptr_t* is_error = NULL, fastiss::block_t* success_block = NULL);
    unsigned char i8_load(unsigned int addr, bool *is_error = NULL);
    unsigned short i16_load(unsigned int addr, bool *is_error = NULL);
    unsigned int i32_load(unsigned int addr, bool *is_error = NULL);
    unsigned long long i64_load(unsigned int addr, bool *is_error = NULL);
    rh850::uint128_t i128_load(unsigned int addr, bool *is_error = NULL);
    void i8_store(unsigned int addr, unsigned char data, bool *is_error = NULL);
    void i16_store(unsigned int addr, unsigned short data, bool *is_error = NULL);
    void i32_store(unsigned int addr, unsigned int data, bool *is_error = NULL);
    void i64_store(unsigned int addr, unsigned long long data, bool *is_error = NULL);
    void i128_store(unsigned int addr, rh850::uint128_t data, bool *is_error = NULL);
    virtual unsigned long long calculate_effective_address(rh850::access_type_t type, unsigned int virtual_address) = 0;
    virtual bool calculate_physical_address(rh850::access_type_t type,
        unsigned long long eff_address,
        unsigned int *physical_address,
        unsigned int *physical_region_start_address,
        unsigned int *physical_region_end_address) = 0;
    virtual fastiss::i64_t* gen_calculate_ignore_effective_address(fastiss::i32_t* virtual_address) = 0;
    virtual fastiss::i64_t* gen_calculate_load_effective_address(fastiss::i32_t* virtual_address) = 0;
    virtual fastiss::i64_t* gen_calculate_store_effective_address(fastiss::i32_t* virtual_address) = 0;
    
  protected:
    
    /**
     * \brief Counter for generating watchpoint ids.
     */
    unsigned int m_next_watchpoint_id;

    /**
     * \brief Memory subsystem - watchpoints indexed by id.
     */
    std::map< unsigned int, watchpoint_info_t * > m_watchpoints;

    /**
     * \brief Memory subsystem read watchpoint map - used for smaller watchpoint
     * regions.
     */
    std::map< unsigned int, std::list< watchpoint_info_t * > * > m_read_watchpoint_map;

    /**
     * \brief Memory subsystem write watchpoint map - used for smaller watchpoint
     * regions.
     */
    std::map< unsigned int, std::list< watchpoint_info_t * > * > m_write_watchpoint_map;

    /**
     * \brief Memory subsystem read watchpoint list - used for larger watchpoint
     * regions.
     */
    std::list< watchpoint_info_t * > m_read_watchpoint_list;

    /**
     * \brief Memory subsystem write watchpoint list - used for larger watchpoint
     * regions.
     */
    std::list< watchpoint_info_t * > m_write_watchpoint_list;

    /**
     * \brief Memory subsystem list of currently pending "hit" watchpoints.
     */
    std::list< watchpoint_info_t * > m_hit_watchpoints;
    
    /**
     * \brief Flag indicating whether watchpoints are present
     */
    bool m_watchpoints_present;
      
    // Analysis points
    rh850::analysis_point_if *m_imm_ld_analysis_point;
    rh850::analysis_point_if *m_imm_st_analysis_point;
    rh850::analysis_point_if *m_gen_ld_analysis_point;
    rh850::analysis_point_if *m_gen_st_analysis_point;
  };
}

#endif // _RH850_MEMORY_IMPL_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
