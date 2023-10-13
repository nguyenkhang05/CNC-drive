/*************************************************************************
 *
 * Copyright(c) 2015-2018 Renesas Electronics Corporation
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
 * \file core.h
 * \brief Defines the Core class
 */

#ifndef _RH850G4_CORE_H_
#define _RH850G4_CORE_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "systemc.h"
#include "tlm.h"
#include "rh850g4/core_base.h"

//------------------------------------------------------------------------------
// Class pre-declarations
//------------------------------------------------------------------------------

class G4_LinkManager;

namespace rh850g4
{
  class custom_impl_t;
  class step_translator_t;
  class continue_translator_t;
}

//------------------------------------------------------------------------------
// rh850g4 core definition
//------------------------------------------------------------------------------

namespace rh850g4
{
  /**
   * \brief The class core is the external interface of the ISS for
   * integration into a platform. The core base provides a SystemC interface for
   * integration and customisation, and an interface to allow debugger operations.
   */
  class core : public rh850g4::core_base_t
  {
  public:
    /**
     * \brief SystemC attribute to specify the duration of one quanta of ISS
     * execution
     */     
    sc_core::sc_attribute<sc_core::sc_time> quantum_period;
    /**
     * \brief SystemC attribute to specify the maximum number of instructions to
     * be translated into a single block. The execution of a quanta can over-run
     * by up to this number of instructions.
     */     
    sc_core::sc_attribute<unsigned int> block_size;
    /**
     * \brief SystemC attribute to control whether the core issues non-blocking or
     * blocking transactions. A value of true will cause the core to issue blocking
     * transactions.
     */     
    sc_core::sc_attribute<bool> blocking_tlm;
    /**
     * \brief SystemC attribute to control whether the core reports TLM response
     * errors as either errors or warnings. A value of true will cause TLM
     * response errors to be reported as errors otherwise they will be reported
     * as warnings.
     */     
    sc_core::sc_attribute<bool> tlm_response_error_as_error;
    /** 
     * \param SystemC attribute which encodes the translation engine to use.
     * A value of zero disables translation, i.e. blocks are only interpreted.
     * The value 1 selects the lightweight translator exclusively, while 2
     * means that only the "heavyweight" translator will be used. A value of 3
     * indicates that both light and heavy translators will be used. A valid
     * license is required to use a value other than 0. If a valid license is
     * not available, then a warning will be produced and all blocks will
     * be interpreted.
     */
    sc_core::sc_attribute<unsigned int> translation_engine;
    /**
     * \brief SystemC attribute to control the number of times a block is
     * interpreted before it is binary translated. A value of zero means that
     * all blocks are always binary translated. A value of 0xffffffff means
     * that all blocks are always interpreted. A valid license is required to
     * use a value other than 0xffffffff, if a valid license is not available
     * then a warning will be produced and all blocks will be interpreted.
     */     
    sc_core::sc_attribute<unsigned int> adaptive_count;
    /**
     * \brief SystemC attribute to control the size of the regions into which
     * the address space is partitioned to enable the core to determine which
     * blocks should be flushed when a breakpoint is set/cleared or a write to
     * memory occurs. Note that the default partition size is 4096 bytes. The
     * partition size must be a power of two. 
     */
    sc_core::sc_attribute<unsigned int> translation_partition_size;
    /**
     * \brief SystemC attribute to control the maximum number of entries in a
     * single linked list of block functions in the translation cache. Once the
     * size reaches this number the earliest translated block is evicted and
     * deleted. This attrubute must have a value greater than 1 and the default
     * value is 4.
     */
    sc_core::sc_attribute<unsigned int> translation_cache_max_list_entries;

    /**
     * \brief SystemC attribute to specify the PE's number (PEID).
     */     
    sc_core::sc_attribute<unsigned int> pe_id;
    /**
     * \brief SystemC attribute to specify the SWD0PCAD's value for SWDT.
     */     
    sc_core::sc_attribute<unsigned int> swd0pcad_val;
    /**
     * \brief SystemC attribute to specify the PC detection mode for SWDT.
     */     
    sc_core::sc_attribute<bool> swdt_enable;
    /**
     * \brief SystemC attribute to specify the Link-function detection mode for the ldl.w/stc.w.
     */     
    sc_core::sc_attribute<bool> link_func_enable;
    /**
     * \brief SystemC attribute to specify the snooze cycle
     */     
    sc_core::sc_attribute<unsigned int> snz_time;
    /**
     * \brief SystemC attribute to specify the cpu milestone
     */     
    sc_core::sc_attribute<unsigned int> cpu_milestone;
    
    /**
     * \brief TLM bus initiator
     */
    tlm::tlm_initiator_socket< 32 > bus_initiator;
    /**
     * \brief Clock input to the ISS. The value of the clock specifies the
     * clock frequency in Hz.
     */     
    sc_core::sc_in<sc_dt::uint64> clock;
    /**
     * \brief Reset request signal, true when a reset is requested and false
     * otherwise
     */
    sc_core::sc_in<bool> reset;
    /**
     * \brief Interrupt request signal, the value of the signal is the interrupt
     * identifier being requested
     */
    sc_core::sc_in<bool> fenmirq;
    /**
     * \brief Interrupt acknowledge signal, set to the interrupt identifier when the
     * ISS begins servicing an interrupt
     */
    sc_core::sc_out<bool> fenmirq_ack;
    /**
     * \brief Interrupt request signal, the value of the signal is the interrupt
     * identifier being requested
     */
    sc_core::sc_in<unsigned int> feintrq;
    /**
     * \brief Interrupt acknowledge signal, set to the interrupt identifier when the
     * ISS begins servicing an interrupt
     */
    sc_core::sc_out<unsigned int> feintrq_ack;
    /**
     * \brief Interrupt request signal, the value of the signal is the interrupt
     * identifier being requested
     */
    sc_core::sc_in<sc_dt::uint64> eiintrq;
    /**
     * \brief Interrupt acknowledge signal, set to the interrupt identifier when the
     * ISS begins servicing an interrupt
     */
    sc_core::sc_out<unsigned int> eiintrq_ack;
    /**
     * \brief CPU GM information notification signal, set to the CPU GM information 
     * when the ISS changes the CPU mode
     */
    sc_core::sc_out<unsigned int> cpugminfo;
    /**
     * \brief Syserr acknowledge signal
     */
    sc_core::sc_out<bool> syserr_ack;
    /**
     * \brief Syserr request signal
     */
    sc_core::sc_in<bool> syserr_req;
    /**
     * \brief Syserr code signal
     */
    sc_core::sc_in<sc_uint<8> > syserr_code;
    /**
     * \brief Syserr mode signal (true:instruction fetch access, false:operand access)
     */
    sc_core::sc_in<bool> syserr_mode;

    /**
     * \brief SystemC event used by the ISS to commence a continue operation
     * (a full quanta of instructions)
     */
    sc_core::sc_event continue_event; 
    /**
     * \brief SystemC event used by the ISS to commence a step operation
     * (a single instruction is executed)
     */
    sc_core::sc_event step_event;
    /**
     * \brief SystemC event used by the ISS to indicate to schedule the
     * operation complete event and subsequent continue event
     */
    sc_core::sc_event continue_completed_event; 
    /**
     * \brief SystemC event used by the ISS to indicate to schedule the
     * operation complete event and subsequent step event
     */
    sc_core::sc_event step_completed_event;
    /**
     * \brief SystemC event used by the ISS to notify the external platform
     * that a reset has been completed
     */
    sc_core::sc_event reset_done_event;

    /**
     * \brief SystemC event used by the ISS to notify the executed PC is
     * matched SWD0PCAD
     */
    sc_core::sc_event swd0pcad_exec_done_event;

  private:
    /**
     * \brief Initial event, used to ensure that the frequency and reset
     * conditions are set before the first continue or step event
     */
    sc_core::sc_event initial_event;
    /**
     * \brief FENMI ackonowledge event
     */
    sc_core::sc_event fenmirq_ack_event;
    /**
     * \brief FEINT ackonowledge event
     */
    sc_core::sc_event feintrq_ack_event;
    /**
     * \brief EIINT ackonowledge event
     */
    sc_core::sc_event eiintrq_ack_event;
    /**
     * \brief CPU mode notify event
     */
    sc_core::sc_event cpugminfo_update_event;
    /**
     * \brief SYSERR ackonowledge event
     */
    sc_core::sc_event syserr_ack_event;
    /**
     * \brief A pointer to the custom generator implementor
     */          
    rh850g4::custom_impl_t *custom;

    /**
     * \brief The module pointer of G4_LinkManager.
     */
    G4_LinkManager *link_manager;

  private:
    /**
     * \addtogroup SystemCInterface
     * @{
     */

    /**
     * \brief SystemC life-cycle handle of code to be executed before the end
     * of the SystemC elaboration phase
     */
    void before_end_of_elaboration(void);
    /**
     * \brief Continue event handler function, executed when the continue event
     * occurs in the simulation
     */
    void continue_handler(void);
    /**
     * \brief Step event handler function, executed when the step event
     * occurs in the simulation
     */
    void step_handler(void);
    /**
     * \brief Handler function which is executed when the value of the clock
     * signal is changed in the simulation
     */
    void frequency_update(void);
    /**
     * \brief Continue thread, used when waits need to be issued by the target
     * of blocking transactions.
     */
    void continue_thread(void);
    /**
     * \brief Step thread, used when waits need to be issued by the target
     * of blocking transactions.
     */
    void step_thread(void);
    /**
     * \brief Continue completed event handler function
     */
    void continue_completed_handler(void);
    /**
     * \brief Step completed event handler function
     */
    void step_completed_handler(void);
    /**
     * \brief Processes any signal changes and indicates whether the continue
     * or step operation should be re-scheduled to allow further signal changes
     * to propagate.
     * \return Boolean value indicating whether the current advance operation
     * should be delayed by one delta cycle.
     */
    bool process_signals(void);
    /**
     * \brief Function called when a fetch exception occurs due to MMU/MPU
     * protections
     */
    void fetch_exception(void);

    /**
     * @}
     */

  public: 
    /**
     * \brief Core class constructor
     * \param name Name of the ISS in the SystemC platform
     * \param count_instructions Boolean flag that indicates if instructions
     * should be counted (if true) or not (if false)     
     */
    core(sc_core::sc_module_name name, bool count_instructions = false);
    /**
     * \brief Core class destructor
     */
    ~core();

    /**
     * \brief get total number of executed instructions
     */
    unsigned long long get_total_inst_num();

    /**
     * \brief get total simulation time
     */
    double get_total_time_sec();

    /**
     * \brief get total simulation time
     */
    unsigned long long get_total_cycle();

    /**
     * \brief set PE ID
     */
    void set_peid(unsigned int peid);

    /**
     * \brief set SWD0OCAD value
     */
    void set_swd0pcad_val(unsigned int val);

    /**
     * \brief set SWDT detection mode
     */
    void set_swdt_enable(bool mode);

    /**
     * \brief get swd0pcad_exec_count value
     */
    unsigned int get_swd0pcad_exec_count_val(void);

    /**
     * \brief get mem_acc_type value
     */
    unsigned int get_mem_acc_type(void);

    /**
     * \brief set G4_LinkManager module's pointer
     */
    void set_link_manager(G4_LinkManager* p_link_manager);

    /**
     * \brief get G3M payload information from each system register
     */
    void get_g3m_params(unsigned int& vcid, unsigned int& tcid, unsigned int& spid, bool& userMode, bool& virtualMode);

    /**
     * \addtogroup SystemCInterface
     * @{
     */
    
    /**
     * \brief CPU milestone type
     */
    typedef enum {
      FASTISS_G4MH_1_0,
      FASTISS_G4MH_1_1,
      FASTISS_G4MH_2_0
    } CORE_CPU_MS_KIND;
    /**
     * @}
     */

  private:    
    /**
     * \brief Handler function that is called when the initial event occurs
     * in the simulation
     */
    void initial_event_handler(void);
    /**
     * \brief Resets the core, resetting register values and clearing all
     * pending interrupts
     */
    void reset_core(void);
    /**
     * \brief The clock frequency in Hz
     */
    unsigned long long frequency;
    /**
     * Boolean flag that indicates if instructions should be counted (if true)
     * or not (if false)
     */
    bool count_instructions;
    
    /**
     * \brief Method invoked by the terminate quantum helper function when a
     * quantum termination request is issued.
     */
    void terminate_quantum(void);

    /**
     * \brief Handler function that is called when the reset signal changes
     * value in the simulation
     */
    void reset_handler(void);
    /**
     * \brief Helper function called when a halt instruction is executed. The
     * current quanta is terminated and the external platform is notified that
     * the core is halted.
     */
    void enter_halt(void);
    /**
     * \brief Allow rh850g4::enter_halt_helper access to the
     * rh850g4::core::enter_halt function
     */
    friend void enter_halt_helper(rh850g4::core *core_instance);

    /**
     * \brief Helper function called when the interrupt request signal changes
     * value in the simulation
     */
    void fenmirq_update(void);
    /**
     * \brief Helper function called when the interrupt request signal changes
     * value in the simulation
     */
    void feintrq_update(void);
    /**
     * \brief Helper function called when the interrupt request signal changes
     * value in the simulation
     */
    void eiintrq_update(void);
    /**
     * \brief Helper function called when the syserr request signal changes
     * value in the simulation
     */
    void syserr_req_update(void);
    /**
     * \brief Helper function called when the syserr code signal changes
     * value in the simulation
     */
    void syserr_code_update(void);
    /**
     * \brief Helper function called when the syserr mode signal changes
     * value in the simulation
     */
    void syserr_mode_update(void);

    /**
     * \brief Helper function called when the FENMI interrupt acknowledge 
     * request signal changes value in the simulation
     */
    void fenmirq_ack_update(void);
    /**
     * \brief Helper function called when the FEINT interrupt acknowledge 
     * request signal changes value in the simulation
     */
    void feintrq_ack_update(void);
    /**
     * \brief Helper function called when the EIINT interrupt acknowledge 
     * request signal changes value in the simulation
     */
    void eiintrq_ack_update(void);
    /**
     * \brief Helper function called when the PSWH value is changed 
     * in the simulation
     */
    void cpugminfo_update(void);
    /**
     * \brief Helper function called when the SYSERR request acknowledge 
     * request signal changes value in the simulation
     */
    void syserr_ack_update(void);
    /**
     * \brief Helper function called when an instruction need to wait before it is invoked.
     */
    void force_wait(void);
    /**
     * \brief Helper function called when snooze is executed.
     */
    void force_wait_snooze(void);
    /**
     * \brief Helper function called when cll and exceptrion are called.
     */
    void clear_llbit(void);
    /**
     * \brief Helper function called when cpu milestone is checked
     */
    unsigned int get_cpu_milestone(void);

    /**
     * \brief SYSERR operation called when the request is INT_SYSERR in process_signal
     */
    bool syserr_operation(unsigned char is_receipt_cpu_mode_guest, unsigned int handler_addr);

    /**
     * \brief FEINT operation called when the request is INT_FEINT in process_signal
     */
    bool feint_operation(unsigned char is_receipt_cpu_mode_guest, unsigned int handler_addr);

    /**
     * \brief EIINT operation called when the request is INT_EIINT in process_signal
     */
    bool eiint_operation(unsigned char is_receipt_cpu_mode_guest, unsigned int handler_addr, bool handler_rint, bool handler_dv);

    /**
     * \brief BGFEINT operation called when the request is INT_BGFEINT in process_signal
     */
    bool bgfeint_operation(unsigned char is_receipt_cpu_mode_guest, unsigned int handler_addr);

    /**
     * \brief BGEIINT operation called when the request is INT_BGEIINT in process_signal
     */
    bool bgeiint_operation(unsigned char is_receipt_cpu_mode_guest, unsigned int handler_addr);

    /**
     * \brief GMFEINT operation called when the request is INT_GMFEINT in process_signal
     */
    bool gmfeint_operation(unsigned char is_receipt_cpu_mode_guest);

    /**
     * \brief GMEIINT operation called when the request is INT_GMEIINT in process_signal
     */
    bool gmeiint_operation(unsigned char is_receipt_cpu_mode_guest);

    /**
     * \brief Allow rh850g4::force_wait_helper access to the
     * rh850g4::core::force_wait function
     */
    friend void force_wait_helper(rh850g4::core *core_instance);

    /**
     * \brief Allow rh850g4::force_wait_snooze_helper access to the
     * rh850g4::core::force_wait_snooze function
     */
    friend void force_wait_snooze_helper(rh850g4::core *core_instance);

    /**
     * \brief Allow rh850g4::set_link_helper access to the
     * rh850g4::core::set_link function
     */
    friend void set_link_helper(G4_LinkManager *link_man_instance, unsigned int peid, unsigned int addr, unsigned int acc_size);

    /**
     * \brief Allow rh850g4::is_link_enable_helper access to the
     * rh850g4::core::is_link_valid function
     */
    friend unsigned int is_link_valid_helper(G4_LinkManager *link_man_instance, unsigned int peid, unsigned int addr, unsigned int acc_size);

    /**
     * \brief Allow rh850g4::clear_link_helper access to the
     * rh850g4::core::clear_link function
     */
    friend void clear_link_helper(G4_LinkManager *link_man_instance, unsigned int peid, unsigned int addr, unsigned int acc_size);

    /**
     * \brief Allow rh850g4::clear_llbit_helper access to the
     * rh850g4::core::clear_llbit function
     */
    friend void clear_llbit_helper(G4_LinkManager *link_man_instance, unsigned int peid);
    /**
     * \brief Allow rh850g4::clear_monitor_helper access to the
     * rh850g4::core::clear_link function
     */
    friend void clear_monitor_helper(G4_LinkManager *link_man_instance, unsigned int peid, unsigned int addr, unsigned int size);

    /**
     * \brief Allow rh850g4::get_cpu_milestone_helper access to the
     * rh850g4::core::cpu_mulestone function
     */
    friend unsigned int get_cpu_milestone_helper(rh850g4::core *core_instance);

    /**
     * \brief Allow rh850g4::cpugminfo_update_helper access to the
     * rh850g4::core::cpugminfo_update_event event
     */
    friend void cpugminfo_update_helper(rh850g4::core *core_instance);

    /**
     * \brief Method invoked by translated code to indicate that an exception/interrupt
     * has been accepted
     */
    void activate_interrupt_accepted_analysis_point(void);

    /**
     * \brief Make the interrupt accepted analysis point activation helper
     * function friend to core to allow it to invoke the
     * rh850g4::core::activate_interrupt_accepted_analysis_point method.
     */
    friend void activate_interrupt_accepted_analysis_point_helper(rh850g4::core *core_instance);

    /**
     * \brief Make the quantum termination helper function friend to core to
     * allow it to invoke the rh850g4::core::terminate_quantum
     * method.
     */
    friend void terminate_quantum_helper(rh850g4::core *core_instance);

    /**
     * \brief Helper function called when the MMU configuration is changed
     * and the L1 Block function cache and data caches needs to be flushed
     */
    void mmu_configuration_changed(void);

    /**
     * \brief Make the MMU configuration changed helper function friend to core to
     * allow it to invoke the rh850g4::core::mmu_configuration_changed
     * method.
     */
    friend void mmu_configuration_changed_helper(rh850g4::core *core_instance);

    /**
     * \brief Helper function called when a setting in the effective address is
     * changed and the L1 Block function cache needs to be flushed
     */
    void effective_address_configuration_changed(void);

    /**
     * \brief Make the effective address configuration changed helper function friend to core to
     * allow it to invoke the rh850g4::core::effective_address_configuration_changed
     * method.
     */
    friend void effective_address_configuration_changed_helper(rh850g4::core *core_instance);

    /**
     * \brief The interrupt identifier of a pending interrupt request
     * bit0 : pending interrupt request exist or not
     * bit1 : interrupt cause is reset
     * bit2 : interrupt cause is fenmi
     * bit3 : interrupt cause is feint
     * bit4 : interrupt cause is eiint
     * bit5 : interrupt cause is syserr
     */
    unsigned int interrupt_requested;
    /**
     * \brief The FENMI interrupt acknowledge value
     */
    bool intack_fenmi_requested;
    /**
     * \brief The FEINT interrupt acknowledge value
     */
    unsigned int intack_feint_requested;
    /**
     * \brief The EIINT interrupt acknowledge value
     */
    unsigned int intack_eiint_requested;
    /**
     * \brief The SYSERR acknowledge value
     */
    unsigned int intack_syserr_requested;
    /**
     * \brief The EI level's interrupt identifier of a pending interrupt request
     */
    unsigned int feintrq_info;
    /**
     * \brief The EI level's interrupt identifier of a pending interrupt request
     */
    sc_dt::uint64 eiintrq_info;
    /**
     * \brief The syserr code
     */
    unsigned int syserr_code_val;
    /**
     * \brief The syserr mode (true:Fetch access, false:Operand access)
     */
    bool syserr_mode_val;

    /**
     * \brief A flag to indicate a pending floating-point imprecise exception
     */
    unsigned int pending_fpu_imprecise_exception;
   
    /**
     * \brief Flag indicating the halt status of the ISS. The value is true when
     * the ISS is halted by the host mode or G4MH 1.x.
     */
    bool halted;

    /**
     * \brief Flag indicating whether the ISS is currently held in a reset state
     */
    bool in_reset;

    /**
     * \brief The interrupt type
     */
    typedef enum {
      INT_REQ   = 0x1,
      INT_RESET = 0x2,
      INT_FENMI = 0x4,
      INT_FEINT = 0x8,
      INT_EIINT = 0x10,
      INT_SYSERR = 0x20,
      INT_GMFEINT = 0x1000,
      INT_GMEIINT = 0x2000,
      INT_BGFEINT = 0x4000,
      INT_BGEIINT = 0x8000
   } CORE_INT_KIND;
  };
}

#endif // _RH850G4_CORE_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
