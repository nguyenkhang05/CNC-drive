//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2019.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

#ifndef _RH850_BUS_INTERFACE_H_
#define _RH850_BUS_INTERFACE_H_

/**
 * \file bus_interface.h
 * \brief Defines the base bus interface class
 */

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <vector>
#include <systemc>
#include <tlm.h>
#include "rh850/memory_if.h"

//------------------------------------------------------------------------------
// Class declarations
//------------------------------------------------------------------------------

namespace fastiss
{
  class i1_t;
  class i8_t;
  class i16_t;
  class i32_t;
  class i64_t;
  class ptr_t;
  class i1_ptr_t;
  class i8_ptr_t;
  class code_gen_if_t;
}

namespace rh850
{
  class cpu_state_if_t;
}

//------------------------------------------------------------------------------
// Bus interface
//------------------------------------------------------------------------------

namespace rh850
{
  /**
   * \brief Models a interface to bus via a tlm::tlm_initiator_socket.
   *
   * Having been granted DMI access of a given type to a given region, an
   * initiator may perform access of the given type anywhere in that region until
   * it is invalidated. In other words, access is not restricted to the address
   * given in the DMI request.
   *
   * Each DMI region shall remain valid until it is explicitly invalidated by a
   * target using a call to invalidate_direct_mem_ptr. Each initiator may maintain
   * a table of valid DMI regions and may continue to use each region until it is
   * invalidated.
   *
   * An initiator receiving an incoming call to invalidate_direct_mem_ptr shall
   * immediately invalidate and discard any DMI region (previously received from
   * a call to get_direct_mem_ptr) that overlaps with the given address range.
   * In the case of a partial overlap, that is, only part of an existing DMI
   * region is invalidated, an initiator may adjust the boundaries of the existing
   * region or may invalidate the entire region.
   */
  class bus_interface_t : public sc_core::sc_object,
                          public tlm::tlm_bw_transport_if<>,
                          public tlm::tlm_mm_interface
  {
  public:
    /**
     * \brief Bus interface constructor.
     * \param name Name of the bus interface.
     */
    bus_interface_t(const std::string& name);

    /**
     * \brief Bus interface destructor.
     */
    virtual ~bus_interface_t();

    /**
     * \brief Bus interface initialisation.
     * \param code_gen_impl A pointer to the code generator implementor
     * \param cpu_state A pointer to the CPU state implementor
     * \param memory_impl A pointer to the memory generator implementor
     * \brief blocking_tlm Flag controlling whether the core issues
     * non-blocking or blocking transactions. A value of true will cause the
     * core to issue blocking transactions.
     */
    virtual void initialise(fastiss::code_gen_if_t *code_gen_impl, rh850::cpu_state_if_t *cpu_state, rh850::memory_if_t *memory_impl, bool blocking_tlm);
    
    /**
     * \brief Bind a TLM-2.0 initiator socket to the bus interface.
     *
     * \param BUSWIDTH Width of the TLM-2.0 socket.
     * \param socket TLM-2.0 initiator socket to bind to.
     */
    template <unsigned int BUSWIDTH>
    void bind(tlm::tlm_initiator_socket<BUSWIDTH>& socket);
    
    /**
     * \brief Initiate a blocking transport request on the bound interface, using DMI
     * if possible.
     * \param trans The TLM-2.0 generic payload for the transaction
     * \param delay The timing annotation for the blocking transport
     */
    void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay);

    /**
     * \brief Perform a debug transport request on the bound interface, using DMI if
     * possible.
     * \param type The type of transaction, either a debug load or store
     * \param eff_address The effective address on the transaction
     * \param size The size of the transaction
     * \param data Buffer holding the data to be read or written.       
     * \return the number of bytes actually read or written
     */
    virtual unsigned int transport_dbg(rh850::access_type_t type, sc_dt::uint64 eff_address, unsigned int size, unsigned char *data);

    /**
     * \brief Perform a debug transport request to a physical address on the 
     * bound interface, using DMI if possible.
     * \param type The type of transaction, either a debug load or store
     * \param physical_address The physical address on the transaction
     * \param size The size of the transaction
     * \param data Buffer holding the data to be read or written.       
     * \return the number of bytes actually read or written
     */
    virtual unsigned int transport_dbg_physical(rh850::access_type_t type, unsigned int physical_address, unsigned int size, unsigned char *data);

    /**
     * \brief Perform a fetch transport request on the bound interface, using DMI if
     * possible.
     * \param eff_address The effective address on the transaction
     * \param size The size of the transaction
     * \param data Buffer holding the data to be read or written.       
     * \return the number of bytes actually read or written
     */
    unsigned int transport_fetch(sc_dt::uint64 eff_address, unsigned int size, unsigned char *data);

    /**
     * \brief Mechanism for registering method to implement non-blocking transport
     * backward interface
     * \param OWNER Class that declares \p callback.
     * \param owner Reference to the object to call \p callback on.
     * \param callback Pointer to callback method.
     */
    template <typename OWNER>
    void register_nb_transport_bw(OWNER& owner, tlm::tlm_sync_enum (OWNER::*callback)(tlm::tlm_generic_payload&, tlm::tlm_phase&, sc_core::sc_time&));

    // tlm::tlm_bw_transport_if over-rides
    tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& delay);

    /**
     * \brief Invalidates all direct memory pointers with regions overlapping
     * the specified address range, excluding those used for physical address
     * accesses.
     * \param start_range The removal region physical start address
     * \param end_range The removal region physical end address
     */
    void invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range);
    /**
     * \brief Invalidates all direct memory pointers, except for those used
     * for physical address accesses.
     */
    void invalidate_all_direct_mem_ptr(void);

    /**
     * \brief Invalidates all direct memory pointers used for physical
     * address accesses.
     */
    void invalidate_all_physical_access_direct_mem_ptr(void);

    /**
     * \brief Memory manager free operation. This operation does nothing as
     * the TLM payloads are deleted when the bus interface is destroyed.
     */
    void free(tlm::tlm_generic_payload* trans);

    /**
     * \brief Generates the issue of a TLM_IGNORE transaction.
     * \param addr The virtual address at which to issue the transactions
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
    */
    void gen_issue_ignore(fastiss::i32_t* addr, fastiss::i1_ptr_t* is_error);
    
    /**
     * \brief Generates a load of an 8-bit value from memory
     * \param addr The virtual address to load a value from (as a 32-bit value)
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \param success_block If a non-NULL block is provided then execution
     * will branch to the supplied block in the case of an L1 DMI table hit.
     * \return The loaded value, which is undefined if an error occurred
     */
    virtual fastiss::i8_t *gen_i8_load(fastiss::i32_t* addr, fastiss::i1_ptr_t* is_error, fastiss::block_t* success_block);

    /**
     * \brief Generates a load of a 16-bit value from memory
     * \param addr The virtual address to load a value from (as a 32-bit value)
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \param success_block If a non-NULL block is provided then execution
     * will branch to the supplied block in the case of an L1 DMI table hit.
     * \return The loaded value, which is undefined if an error occurred
     */
    virtual fastiss::i16_t *gen_i16_load(fastiss::i32_t* addr, fastiss::i1_ptr_t* is_error, fastiss::block_t* success_block);
    
    /**
     * \brief Generates a load of a 32-bit value from memory
     * \param addr The virtual address to load a value from (as a 32-bit value)
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \param success_block If a non-NULL block is provided then execution
     * will branch to the supplied block in the case of an L1 DMI table hit.
     * \return The loaded value, which is undefined if an error occurred
     */
    virtual fastiss::i32_t *gen_i32_load(fastiss::i32_t* addr, fastiss::i1_ptr_t* is_error, fastiss::block_t* success_block);

    /**
     * \brief Generates a load of a 64-bit value from memory
     * \param addr The virtual address to load a value from (as a 32-bit value)
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \param success_block If a non-NULL block is provided then execution
     * will branch to the supplied block in the case of an L1 DMI table hit.
     * \return The loaded value, which is undefined if an error occurred
     */
    virtual fastiss::i64_t *gen_i64_load(fastiss::i32_t* addr, fastiss::i1_ptr_t* is_error, fastiss::block_t* success_block);

    /**
     * \brief Generates a an arbitrary size load from memory
     * \param addr The virtual address to load a value from (as a 32-bit value)
     * \param size The size of access to perform in byte. Note this must be a multiple of 8.
     * \param data Pointer to the loaded value, which is undefined if an error occurred
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \param success_block If a non-NULL block is provided then execution
     * will branch to the supplied block in the case of an L1 DMI table hit.
     */
    virtual void gen_iN_load(fastiss::i32_t* addr, unsigned int size, fastiss::i8_ptr_t* data, fastiss::i1_ptr_t* is_error, fastiss::block_t* success_block);
    
    /**
     * \brief Generates a store of an 8-bit value to memory
     * \param addr The virtual address to store a value to (as a 32-bit value)
     * \param data The value to be stored
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \param success_block If a non-NULL block is provided then execution
     * will branch to the supplied block in the case of an L1 DMI table hit.
     */
    virtual void gen_i8_store(fastiss::i32_t* addr, fastiss::i8_t* data, fastiss::i1_ptr_t* is_error, fastiss::block_t* success_block);
  
    /**
     * \brief Generates a store of a 16-bit value to memory
     * \param addr The virtual address to store a value to (as a 32-bit value)
     * \param data The value to be stored
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \param success_block If a non-NULL block is provided then execution
     * will branch to the supplied block in the case of an L1 DMI table hit.
     */
    virtual void gen_i16_store(fastiss::i32_t* addr, fastiss::i16_t* data, fastiss::i1_ptr_t* is_error, fastiss::block_t* success_block);
    
    /**
     * \brief Generates a store of a 32-bit value to memory
     * \param addr The virtual address to store a value to (as a 32-bit value)
     * \param data The value to be stored
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \param success_block If a non-NULL block is provided then execution
     * will branch to the supplied block in the case of an L1 DMI table hit.
     */
    virtual void gen_i32_store(fastiss::i32_t* addr, fastiss::i32_t* data, fastiss::i1_ptr_t* is_error, fastiss::block_t* success_block);

    /**
     * \brief Generates a store of a 64-bit value to memory
     * \param addr The virtual address to store a value to (as a 32-bit value)
     * \param data The value to be stored
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \param success_block If a non-NULL block is provided then execution
     * will branch to the supplied block in the case of an L1 DMI table hit.
     */
    virtual void gen_i64_store(fastiss::i32_t* addr, fastiss::i64_t* data, fastiss::i1_ptr_t* is_error, fastiss::block_t* success_block);

    /**
     * \brief Generates a store of a arbitrary size value to memory
     * \param addr The virtual address to store a value to (as a 32-bit value)
     * \param size The size of the value to be written to memory. Note this must
     * be a multiple of 8.
     * \param data Pointer to the value to be stored
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \param success_block If a non-NULL block is provided then execution
     * will branch to the supplied block in the case of an L1 DMI table hit.
     */
    virtual void gen_iN_store(fastiss::i32_t* addr, unsigned int size, fastiss::i8_ptr_t* data, fastiss::i1_ptr_t* is_error, fastiss::block_t* success_block);
    
    /**
     * \brief Perform a blocking read access at the specified address, using DMI if
     * possible.
     * \param eff_address Effective address of the access.
     * \param size Number of data bytes to read.
     * \param data Pointer to the buffer to place the read data.
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     */
    virtual void read(sc_dt::uint64 eff_address, unsigned int size, unsigned char *data, bool *is_error);

    /**
     * \brief Perform a blocking write access at the specified address, using DMI if
     * possible.
     * \param eff_address Effective address of the access.
     * \param size Number of data bytes to write.
     * \param data Pointer to the buffer containing the data to write.
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     */
    virtual void write(sc_dt::uint64 eff_address, unsigned int size, unsigned char *data, bool *is_error);

    /**
     * \brief Issues a TLM ignore transaction at the specified address, note that DMI is always bypassed.
     * \param eff_address Effective address of the access.
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     */
    void issue_ignore(sc_dt::uint64 eff_address, bool *is_error);

  protected:
    /**
     * \brief Performs a blocking TLM transaction access using the provided payload
     * \param payload The TLM payload for the transaction
     * \param local_time The current local time offset from the start of the quanta
     */
    virtual void issue_blocking_transaction(tlm::tlm_generic_payload& payload, sc_core::sc_time& local_time);

    /**
     * \brief Performs a non-blocking TLM transaction access using the provided payload
     * \param payload The TLM payload for the transaction
     * \param local_time The current local time offset from the start of the quanta
     */
    virtual void issue_non_blocking_transaction(tlm::tlm_generic_payload& payload, sc_core::sc_time& local_time);
    
  public:
    /**
     * \brief Perform a DMI read access at the specified address. The access
     * will either find an existing DMI region, or request a DMI pointer.
     * \param type The type of transaction, either a load, debug load or a fetch
     * \param eff_address Effective address of the access.
     * \param size Number of data bytes to read.
     * \param data Pointer to the buffer to place the read data.
     * \param delay The timing annotation for the DMI read
     * \return True if the DMI read completed successfully, false otherwise
     */
    bool dmi_read(rh850::access_type_t type, sc_dt::uint64 eff_address, unsigned int size, unsigned char *data, sc_core::sc_time& delay);
    /**
     * \brief Perform a DMI write access at the specified address. The access
     * will either find an existing DMI region, or request a DMI pointer.
     * \param type The type of transaction, either a store or debug store
     * \param eff_address Effective address of the access.
     * \param size Number of data bytes to read.
     * \param data Pointer to the buffer to place the read data.
     * \param delay The timing annotation for the DMI read
     * \return True if the DMI write completed successfully, false otherwise
     */
    bool dmi_write(rh850::access_type_t type, sc_dt::uint64 eff_address, unsigned int size, unsigned char *data, sc_core::sc_time& delay);      

    /**
     * \brief Displays the regions currently held in the region cache
     */
    void show_regions(void);

    /**
     * \brief Struct for code generator accessible DMI region information
     */
    typedef struct cg_region_info
    {
      /**
       * \brief Effective start address of the 4096 byte code generator walk-able
       * region, see the cache description below.
       */
      unsigned long long eff_address;
      /**
       * \brief Pointer to the data for the region: this is the difference between
       * the host and guest page addresses. i.e. host_addr - guest_addr
       */
      unsigned char *data;
#if !defined(_MSC_VER) && !defined(__x86_64__)
      /**
       * \brief Padding to ensure that the region info data structure is the same
       * size on both Windows and Linux.
       */
      unsigned int padding;
#endif
    } cg_region_info;
    
    /**
     * \brief Storage class for a single DMI region with granted/denied access
     */
    typedef struct
    {
        /**
         * \brief The TLM DMI region information containing the DMI region
         * address range and DMI pointer
         */
        tlm::tlm_dmi dmi;
        /**
         * \brief The start address of the associated effective region
         */
        unsigned long long eff_start_address;
        /**
         * \brief The end address of the associated effective region
         */
        unsigned long long eff_end_address;
        /**
         * \brief The physical address used in the DMI request
         */
        unsigned int physical_address_requested;
        /**
         * \brief The start address of the associated physical region
         */
        unsigned int physical_region_start_address;
        /**
         * \brief The end address of the associated physical region
         */
        unsigned int physical_region_end_address;
        /**
         * \brief A boolean indicating if DMI access was granted or denied
         * for the region
         */
        bool granted;
    } dmi_region;

    /**
     * \brief Storage class for all known DMI region information - including address
     * range (effective and physical), DMI pointer and granted/denied access.
     */
    class dmi_region_cache
    {
    public:
      /**
       * \brief Region cache constructor
      */
      dmi_region_cache();
      /**
       * \brief Region cache destructor
       */
      ~dmi_region_cache();
      /**
       * \brief Region insertion operation
       * \param val DMI region information
       */
      void insert(dmi_region& val);
      /**
       * \brief Region removal operation - removes all regions overlapping
       * the specified address range.
       * \param start_range Removal region physical start address
       * \param end_range Removal region physical end address
       */
      void remove(sc_dt::uint64 start_range, sc_dt::uint64 end_range);
      /**
       * \brief Region removal operation - removes all regions
       */
      void remove_all(void);
      /**
       * \brief Region lookup operation
       * \param addr Region lookup effective address
       * \param val DMI region information
       * \return Boolean indicating whether a matching region was found
       */
      const bool find(sc_dt::uint64 addr, dmi_region& val);
      /**
       * \brief Displays the regions currently held in the region cache
       */
      void show_regions(void);
      /**
       * \brief MRU (most recently used) vector of all known regions,
       * including those that are too small to satisfy the cache
       * requirements and regions for which DMI access is denied.
       */
      std::vector< dmi_region > _vec;
      /**
       * \brief Code generator walkable cache of regions, note that each
       * entry in this list corresponds to a 4096 byte region of memory,
       * if the DMI region corresponding to this address does not fully
       * occupy a given 4096 byte region then no entry will be created.
       * The cache lookup algorithm is based on the multiplication
       * technique described here:
       * https://www.cs.auckland.ac.nz/~jmor159/PLDS210/niemann/s_has.htm
       */
      cg_region_info *_regions;
    };

    /**
     * \brief Operation to get a populated payload to be used for a debug transaction
     * \param command Type of transaction to be issued
     * \param address Physical address at which the transaction is to be issued
     * \param size Size of the transaction to be issued
     * \param data Buffer used to hold data read or written by the transaction
     * \return Reference to the populated payload structure to be used for the transaction
     */
    virtual tlm::tlm_generic_payload& get_debug_payload(tlm::tlm_command command, sc_dt::uint64 address, unsigned int size, unsigned char *data) = 0;
    
    /**
     * \brief Operation to get a populated payload to be used for a fetch transaction
     * \param address Physical address at which the transaction is to be issued
     * \param size Size of the transaction to be issued
     * \param data Buffer used to hold data read or written by the transaction
     * \return Reference to the populated payload structure to be used for the transaction
     */
    virtual tlm::tlm_generic_payload& get_fetch_payload(sc_dt::uint64 address, unsigned int size, unsigned char *data) = 0;
    
    /**
     * \brief Operation to get a populated payload to be used for a DMI request
     * \param command Type of transaction to be issued
     * \param address Physical address at which the transaction is to be issued
     * \param size Size of the transaction to be issued
     * \param data Buffer used to hold data read or written by the transaction
     * \return Reference to the populated payload structure to be used for the transaction
     */
    virtual tlm::tlm_generic_payload& get_dmi_payload(tlm::tlm_command command, sc_dt::uint64 address, unsigned int size, unsigned char *data) = 0;
    
    /**
     * \brief Operation to get a populated payload to be used for other (ie. normal blocking or non-blocking) transactions
     * \param command Type of transaction to be issued
     * \param address Physical address at which the transaction is to be issued
     * \param size Size of the transaction to be issued
     * \param data Buffer used to hold data read or written by the transaction
     * \return Reference to the populated payload structure to be used for the transaction
     */
    virtual tlm::tlm_generic_payload& get_payload(tlm::tlm_command command, sc_dt::uint64 address, unsigned int size, unsigned char *data) = 0;

    /**
     * \brief Operation used to complete any necessary post-transaction processing of
     * a debug transaction. Typically this is used to check the payload response
     * status.
     * \param bytes_transferred Number of bytes successfully transferred.
     */
    virtual void debug_completed(unsigned int bytes_transferred) = 0;

    /**
     * \brief Operation used to complete any necessary post-transaction processing of
     * a fetch transaction. Typically this is used to check the payload response
     * status.
     * \param bytes_transferred Number of bytes successfully transferred.
     */
    virtual void fetch_completed(unsigned int bytes_transferred) = 0;

    /**
     * \brief Operation used to complete any necessary post-transaction processing of
     * a DMI request. Typically this is used to check the payload response status.
     */
    virtual void dmi_completed(void) = 0;

    /**
     * \brief Operation used to complete any necessary post-transaction processing of
     * other (ie. normal blocking or non-blocking) transactions. Typically this
     * is used to check the payload response status.
     */
    virtual void transaction_completed(void) = 0;

    /**
     * \brief Callout used after initiating a DMI request. Allows the DMI,
     * physical and effective address information to be customised for an MPU
     * or MMU (for example to restrict the region to an MMU page or MPU region).
     * \param type The transaction type (either a load, store, debug load, debug
     * store, fetch, or ignore (zero sized) access)
     * \param val The DMI region information after the DMI request
     */
    virtual void dmi_pointer_post_callout(rh850::access_type_t type, dmi_region *val) = 0;

  private:
    virtual fastiss::i1_t* gen_is_dmi_hit(void* dmi_table, fastiss::i64_t* eff_addr, unsigned int size, fastiss::ptr_t*& host_region_ptr);
    virtual fastiss::i1_t* gen_is_read_dmi_hit(fastiss::i64_t* eff_addr, unsigned int size, fastiss::ptr_t*& host_region_ptr);
    virtual fastiss::i1_t* gen_is_write_dmi_hit(fastiss::i64_t* eff_addr, unsigned int size, fastiss::ptr_t*& host_region_ptr);

  protected:
    std::auto_ptr<tlm::tlm_fw_transport_if<> > _fw_if;
    bool m_blocking_tlm;
    bool m_dummy_is_error_flag;
    fastiss::code_gen_if_t* m_code_gen_impl;
    rh850::cpu_state_if_t *m_cpu_state;
    rh850::memory_if_t* m_memory;
    /**
     * \brief SystemC event used by non-blocking transport operations to yield
     * SystemC control to allow simulation time to advance
     */
    sc_core::sc_event nb_yield_event; 
    
  public:
    dmi_region_cache _dmi_read_regions;
    dmi_region_cache _dmi_write_regions;
    dmi_region* _physical_access_dmi_read_region;
    dmi_region* _physical_access_dmi_write_region;
  };
}

#endif //  _RH850_BUS_INTERFACE_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
