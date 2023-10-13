/*************************************************************************
 *
 * Copyright(c) 2015-2020 Renesas Electronics Corporation
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

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "rh850g4/core.h"
#include "rh850g4/custom_cpu_state_impl.h"
#include "rh850g4/custom_impl.h"
#include "rh850g4/custom_memory_impl.h"
#include "rh850g4/step_translator.h"
#include "rh850g4/continue_translator.h"
#include "rh850g4/custom_bus_interface.h"
#include "G4_intc_typ.h"
#include "G4_LinkManager.h"
#include "rh850g4/analysis_point_if.h"

#include <iostream>
#include <sstream>

using namespace G4IntcTyp;

//------------------------------------------------------------------------------
// Helper functions
//------------------------------------------------------------------------------

namespace rh850g4
{
  void terminate_quantum_helper(core *core_instance)
  {
    core_instance->terminate_quantum();
  }

  void mmu_configuration_changed_helper(core *core_instance)
  {
    core_instance->mmu_configuration_changed();
  }

  void effective_address_configuration_changed_helper(core *core_instance)
  {
    core_instance->effective_address_configuration_changed();
  }

  void enter_halt_helper(core *core_instance)
  {
    core_instance->enter_halt();
  }

  void force_wait_helper(core *core_instance)
  {
    core_instance->force_wait();
  }
  
  void force_wait_snooze_helper(core *core_instance)
  {
    core_instance->force_wait_snooze();
  }
  
  void set_link_helper(G4_LinkManager *link_man_instance, unsigned int peid, unsigned int addr, unsigned int acc_size)
  {
    link_man_instance->SetLink(peid, addr, acc_size);
  }

  unsigned int is_link_valid_helper(G4_LinkManager *link_man_instance, unsigned int peid, unsigned int addr, unsigned int acc_size)
  {
    bool status = link_man_instance->IsLinkValid(peid, addr, acc_size);
    if(status) return(1);
    else       return(0);
  }

  void clear_link_helper(G4_LinkManager *link_man_instance, unsigned int peid, unsigned int addr, unsigned int acc_size)
  {
    link_man_instance->ClearLink(peid, addr, acc_size);
  }

  void clear_llbit_helper(G4_LinkManager *link_man_instance, unsigned int peid)
  {
    link_man_instance->ClearLlbit(peid);
  }

  void clear_monitor_helper(G4_LinkManager *link_man_instance, unsigned int peid, unsigned int addr, unsigned int size)
  {
    link_man_instance->ClearMonitor(peid, addr, size);
  }

  unsigned int get_cpu_milestone_helper(core *core_instance)
  {
    return(core_instance->get_cpu_milestone());
  }

  void cpugminfo_update_helper(core *core_instance)
  {
    core_instance->cpugminfo_update();
  }

  void activate_interrupt_accepted_analysis_point_helper(core *core_instance)
  {
    core_instance->activate_interrupt_accepted_analysis_point();
  }
}

//------------------------------------------------------------------------------
// rh850g4 core definition
//------------------------------------------------------------------------------

rh850g4::core::core(sc_core::sc_module_name name, bool count_instructions) :
  // Base class
  rh850g4::core_base_t(name),
  
  // Attributes
  quantum_period("quantum_period", sc_core::sc_time(1, sc_core::SC_MS)),
  block_size("block_size", 100),
  blocking_tlm("blocking_tlm", false),
  tlm_response_error_as_error("tlm_response_error_as_error", false),
  // translation_engine("translation_engine", 3), PMS 3.3.0 has bug, if set 1 or 3 at translation_engine. until bug fix, set 2.
  translation_engine("translation_engine", 2),
  adaptive_count("adaptive_count", 5),
  translation_partition_size("translation_partition_size", 4096),
  translation_cache_max_list_entries("translation_cache_max_list_entries", 4),
  pe_id("pe_id", 1),

  // Initialize swd0pcad_val and swdt_enable(for SWDT)
  swd0pcad_val("swd0pcad_val", 0xffffffff),
  swdt_enable("swdt_enable", false),
  link_func_enable("link_func_enable", false),
  snz_time("snz_time", 32),
  cpu_milestone("cpu_milestone", FASTISS_G4MH_1_0),

  // Bus interfaces
  bus_initiator("bus_initiator"),
  
  // Signals
  clock("clock"),
  reset("reset"),
  fenmirq("fenmirq"),
  fenmirq_ack("fenmirq_ack"),
  feintrq("feintrq"),
  feintrq_ack("feintrq_ack"),
  eiintrq("eiintrq"),
  eiintrq_ack("eiintrq_ack"),
  cpugminfo("cpugminfo"),
  syserr_ack("syserr_ack"),
  syserr_req("syserr_req"),
  syserr_code("syserr_code"),
  syserr_mode("syserr_mode"),
  
  // Events
  continue_event(),
  step_event(),
  continue_completed_event(),
  step_completed_event(),
  reset_done_event(),
  swd0pcad_exec_done_event(),
  initial_event(),
  fenmirq_ack_event(),
  feintrq_ack_event(),
  eiintrq_ack_event(),
  cpugminfo_update_event(),
  syserr_ack_event(),
  
  // Generated components
  custom(NULL),

  // Link Manager
  link_manager(0),

  // Interrupt request flag
  interrupt_requested(0),

  // Time management
  frequency(0),

  // Instruction counting
  count_instructions(count_instructions),

  // Interrupt acknowledge flag
  intack_fenmi_requested(false),
  intack_feint_requested(0),
  intack_eiint_requested(0),
  intack_syserr_requested(false),
  feintrq_info(0),
  eiintrq_info(0),
  syserr_code_val(0),
  syserr_mode_val(false),
  
  // Pending floating point imprecise exception flag
  pending_fpu_imprecise_exception(0), // false

  // Halt state
  halted(false),

  // maybe not use it
  in_reset(false) 
{
  SC_HAS_PROCESS(core);

  // Initialise initial event handler
  SC_METHOD(initial_event_handler);
  sensitive << initial_event;
  dont_initialize();
  
  // Initialise reset handler
  SC_METHOD(reset_handler);
  sensitive << reset;
  dont_initialize();

  // Initialise FENMI req handler
  SC_METHOD(fenmirq_update);
  sensitive << fenmirq;
  dont_initialize();

  // Initialise FEINT req handler
  SC_METHOD(feintrq_update);
  sensitive << feintrq;
  dont_initialize();

  // Initialise EIINT req handler
  SC_METHOD(eiintrq_update);
  sensitive << eiintrq;
  dont_initialize();

  // Initialise SYSERR req handler
  SC_METHOD(syserr_req_update);
  sensitive << syserr_req;
  dont_initialize();

  // Initialise SYSERR code handler
  SC_METHOD(syserr_code_update);
  sensitive << syserr_code;
  dont_initialize();

  // Initialise SYSERR mode handler
  SC_METHOD(syserr_mode_update);
  sensitive << syserr_mode;
  dont_initialize();

  // Initialise FENMI ack handler
  SC_METHOD(fenmirq_ack_update);
  sensitive << fenmirq_ack_event;
  dont_initialize();

  // Initialise FEINT ack handler
  SC_METHOD(feintrq_ack_update);
  sensitive << feintrq_ack_event;
  dont_initialize();

  // Initialise EIINT ack handler
  SC_METHOD(eiintrq_ack_update);
  sensitive << eiintrq_ack_event;
  dont_initialize();

  // Initialise CPU mode (PSWH value) update handler
  SC_METHOD(cpugminfo_update);
  sensitive << cpugminfo_update_event;
  dont_initialize();

  // Initialise SYSERR ack handler
  SC_METHOD(syserr_ack_update);
  sensitive << syserr_ack_event;
  dont_initialize();

  // Initialise clock/frequency handler
  SC_METHOD(frequency_update);
  sensitive << clock;
  dont_initialize();

  // Completion event handlers
  SC_METHOD(continue_completed_handler);
  sensitive << continue_completed_event;
  dont_initialize();
  
  SC_METHOD(step_completed_handler);
  sensitive << step_completed_event;
  dont_initialize();

  // Create bus sub-components
  fastiss_memory = new rh850g4::custom_memory_impl_t();

  // Create and bind bus interfaces
  fastiss_bus_interface = new rh850g4::custom_bus_interface_t("fastiss_bus_interface");
  fastiss_bus_interface->bind(bus_initiator);

  // Create sub-components
  cpu_state = new rh850g4::custom_cpu_state_impl_t();
}

rh850g4::core::~core()
{
  delete reinterpret_cast<rh850g4::custom_memory_impl_t*>(fastiss_memory);
  delete step_translator;
  delete continue_translator;
  delete custom;
  delete cpu_state;
}

void rh850g4::core::before_end_of_elaboration(void)
{
  // Check translation partition size
  unsigned int trans_part_sz = translation_partition_size.value;
  if ((trans_part_sz <= 128) || (trans_part_sz & (trans_part_sz - 1)))
  {
    std::ostringstream os;
    os << "Invalid translation partition size: " << trans_part_sz << " bytes" << std::endl;
    os << "Partition size must be a power of two and greater than 128." << std::endl;
    sc_core::sc_report_handler::report(sc_core::SC_ERROR, "rh850g4", os.str().c_str(), "<UNKNOWN>", 0);
  }
  
  // Check translation cache max number of entries
  if (translation_cache_max_list_entries.value <= 1)
  {
    std::ostringstream os;
    os << "Invalid translation cache maximum number of list entries, value must be greater than one" << std::endl;
    sc_core::sc_report_handler::report(sc_core::SC_ERROR, "rh850g4", os.str().c_str(), "<UNKNOWN>", 0);
  }
  
  // Initialise components
  rh850g4::core_base_t::initialise
  (
    blocking_tlm.value,
    tlm_response_error_as_error.value,
    translation_engine.value,
    adaptive_count.value,
    ~(trans_part_sz - 1),
    translation_cache_max_list_entries.value,
    count_instructions
  );
  
  // Create sub-components - note that the block size is capped at a maximum
  // value of 100 (since optimisation can take a large amount of time for very
  // large blocks).

  if ((cpu_milestone.value==FASTISS_G4MH_1_0) || (cpu_milestone.value==FASTISS_G4MH_1_1)){
    custom = new rh850g4::custom_impl_t(fastiss_code_gen, fastiss_memory, cpu_state, m_interrupt_accepted_analysis_point, m_interrupt_completed_analysis_point, &pending_fpu_imprecise_exception, link_manager, link_func_enable.value, false);
    cpu_state->write_hv_func_support(0);
  }else{
    custom = new rh850g4::custom_impl_t(fastiss_code_gen, fastiss_memory, cpu_state, m_interrupt_accepted_analysis_point, m_interrupt_completed_analysis_point, &pending_fpu_imprecise_exception, link_manager, link_func_enable.value, true);
    cpu_state->write_hv_func_support(1);
  }

  if (block_size.value > 100)
  {
    block_size.value = 100;
  }
  step_translator = new rh850g4::step_translator_t
  (
    fastiss_code_gen,
    fastiss_memory,
    operand,
    cpu_state,
    custom,
    swdt_enable.value,
    m_pre_translate_loop_analysis_point,
    m_post_translate_loop_analysis_point,
    m_post_fetch_analysis_point,
    m_post_decode_analysis_point,
    m_post_execute_analysis_point,
    ~(trans_part_sz - 1)
  );
  continue_translator = new rh850g4::continue_translator_t
  (
    fastiss_code_gen,
    fastiss_memory,
    operand,
    cpu_state,
    custom,
    block_size.value,
    swdt_enable.value,
    m_pre_translate_loop_analysis_point,
    m_post_translate_loop_analysis_point,
    m_post_fetch_analysis_point,
    m_post_decode_analysis_point,
    m_post_execute_analysis_point,
    ~(trans_part_sz - 1)
  );

  // Initialize cpu_state::swd0pcad_val (for SWDT)
  cpu_state->write_swd0pcad_val(swd0pcad_val.value);

  // Initialise the block function cache
  initialise_cache();
  
  // Add core helper functions
  fastiss::func_args_type_t* core_helper_args = fastiss_code_gen->create_args_type();
  fastiss_code_gen->append_ptr(core_helper_args);
  fastiss_code_gen->add_helper("terminate_quantum_helper", (void*)&rh850g4::terminate_quantum_helper, fastiss_code_gen->create_void_func(core_helper_args));
  fastiss_code_gen->add_helper("mmu_configuration_changed_helper", (void*)&rh850g4::mmu_configuration_changed_helper, fastiss_code_gen->create_void_func(core_helper_args));
  fastiss_code_gen->add_helper("effective_address_configuration_changed_helper", (void*)&rh850g4::effective_address_configuration_changed_helper, fastiss_code_gen->create_void_func(core_helper_args));
  fastiss_code_gen->add_helper("enter_halt_helper", (void*)&rh850g4::enter_halt_helper, fastiss_code_gen->create_void_func(core_helper_args));
  fastiss_code_gen->add_helper("force_wait_helper", (void*)&rh850g4::force_wait_helper, fastiss_code_gen->create_void_func(core_helper_args));
  fastiss_code_gen->add_helper("force_wait_snooze_helper", (void*)&rh850g4::force_wait_snooze_helper, fastiss_code_gen->create_void_func(core_helper_args));
  fastiss_code_gen->add_helper("get_cpu_milestone_helper", (void*)&rh850g4::get_cpu_milestone_helper, fastiss_code_gen->create_void_func(core_helper_args));
  fastiss_code_gen->add_helper("cpugminfo_update_helper", (void*)&rh850g4::cpugminfo_update_helper, fastiss_code_gen->create_void_func(core_helper_args));
  fastiss_code_gen->add_helper("activate_interrupt_accepted_analysis_point_helper", (void*)&rh850g4::activate_interrupt_accepted_analysis_point_helper, fastiss_code_gen->create_void_func(core_helper_args));
  fastiss_code_gen->delete_args_type(core_helper_args);

  fastiss::func_args_type_t* core_helper_args2 = fastiss_code_gen->create_args_type();
  fastiss_code_gen->append_ptr(core_helper_args2);
  fastiss_code_gen->append_i32(core_helper_args2);
  fastiss_code_gen->add_helper("clear_llbit_helper", (void*)&rh850g4::clear_llbit_helper, fastiss_code_gen->create_void_func(core_helper_args2));
  fastiss_code_gen->delete_args_type(core_helper_args2);  

  fastiss::func_args_type_t* core_helper_args4 = fastiss_code_gen->create_args_type();
  fastiss_code_gen->append_ptr(core_helper_args4);
  fastiss_code_gen->append_i32(core_helper_args4);
  fastiss_code_gen->append_i32(core_helper_args4);
  fastiss_code_gen->append_i32(core_helper_args4);
  fastiss_code_gen->add_helper("clear_monitor_helper", (void*)&rh850g4::clear_monitor_helper, fastiss_code_gen->create_void_func(core_helper_args4));
  fastiss_code_gen->add_helper("set_link_helper", (void*)&rh850g4::set_link_helper, fastiss_code_gen->create_void_func(core_helper_args4));
  fastiss_code_gen->add_helper("is_link_valid_helper", (void*)&rh850g4::is_link_valid_helper, fastiss_code_gen->create_i32_func(core_helper_args4));
  fastiss_code_gen->add_helper("clear_link_helper", (void*)&rh850g4::clear_link_helper, fastiss_code_gen->create_void_func(core_helper_args4));
  fastiss_code_gen->delete_args_type(core_helper_args4);  

  // Spawn step and continue thread processes (method processes are not supported)
  sc_core::sc_spawn
  (
    sc_bind(&rh850g4::core::continue_thread, this),
    sc_core::sc_gen_unique_name("continue_thread")
  );
  sc_core::sc_spawn
  (
    sc_bind(&rh850g4::core::step_thread, this),
    sc_core::sc_gen_unique_name("step_thread")
  );
  
  initialise_parallel_translator();

  // Schedule initial advance event
  initial_event.notify(SC_ZERO_TIME); 
}

void rh850g4::core::initial_event_handler(void)
{
  // This check ensures that the reset done notification generated by
  // reset_core() is not issued until after the first delta cycle has
  // completed - note that this restriction is imposed by the current
  // VLAB software debug implementation.
  if (sc_core::sc_delta_count())
  {
    // Guarantee that the frequency handler has been called at least
    // once before the continue event is scheduled
    frequency_update();
    reset_handler();
    
    // Schedule advance event
    continue_event.notify(SC_ZERO_TIME);
  }
  else
  {
    // If this is this first delta - reschedule
    initial_event.notify(SC_ZERO_TIME); 
  }
}

void rh850g4::core::frequency_update(void)
{
  // Record frequency and quanta
  frequency = clock.read();
  time_limit = static_cast<unsigned long long>(quantum_period.value.value());

  // Calculate and set period of a cycle
  if (frequency == 0)
  {
    cpu_state->write_cycle_period(0);
  }
  else
  {
    cpu_state->write_cycle_period(1000000000000ull / frequency);
  }
}

void rh850g4::core::continue_handler(void)
{
  // Flush the translation cache if there is a pending request
  if (flush_required)
  {
    flush_required = false;
    partitions_to_flush.clear();
    flush_cache();
  }
  
  // Flush specific partitions if there are pending requests
  if (!partitions_to_flush.empty())
  {
    flush_partitions();
    partitions_to_flush.clear();
  }

  // Complete the operation if stop requested or hit a breakpoint
  if (stop_requested)
  {
    stop_requested = false;
    continue_completed_event.notify(SC_ZERO_TIME);
    return;
  }

  // If there are pending watchpoint hits, for example as a result of signal
  // processing then handle these now (can also catch watchpoint hits that
  // have not otherwise been serviced and cleared by the debugger.
  if (cpu_state->read_watchpoint_hit())
  {
    continue_completed_event.notify(SC_ZERO_TIME);
    return;
  }
  
  // Process pending signals
  if (process_signals())
  {
    // Activate interrupt accepted analysis point
    if (m_interrupt_accepted_analysis_point->has_attachments())
    {
      m_interrupt_accepted_analysis_point->activate();
    }
    
    continue_event.notify(SC_ZERO_TIME);
    return;
  }

  // Is the cpu halted or the clock turned off?
  if (halted || (frequency == 0))
  {
    continue_event.notify(quantum_period.value);
    return;
  }


  // Clear swd0pcad_exec_count value (for SWDT)
  cpu_state->write_swd0pcad_exec_count(0);

  // Enter translate-execute loop
  try
  {
    advance_by_continue(time_limit);
  }
  catch(std::exception& e)
  {
    sc_core::sc_report_handler::report(sc_core::SC_ERROR, "rh850g4", e.what(), "<UNKNOWN>", 0);
  }

  // Check the number of times that the executed PC is matched SWD0PCAD's value (for SWDT)
  unsigned int count_num = cpu_state->read_swd0pcad_exec_count();
  if( count_num > 0)
  {
    //notify the address match to NSMVG3MCPUV01
    swd0pcad_exec_done_event.notify();
  }

  // Schedule next quantum
  continue_event.notify(get_local_time());
}

void rh850g4::core::step_handler(void)
{
  // Flush the translation cache if there is a pending request
  if (flush_required)
  {
    flush_required = false;
    partitions_to_flush.clear();
    flush_cache();
  }
  
  // Flush specific partitions if there are pending requests
  if (!partitions_to_flush.empty())
  {
    flush_partitions();
    partitions_to_flush.clear();
  }

  // Complete the operation if stop requested
  if (stop_requested)
  {
    stop_requested = false;
    step_completed_event.notify(SC_ZERO_TIME);
    return;
  }
  
  // If there are pending watchpoint hits, for example as a result of signal
  // processing then handle these now (can also catch watchpoint hits that
  // have not otherwise been serviced and cleared by the debugger.
  if (cpu_state->read_watchpoint_hit())
  {
    step_completed_event.notify(SC_ZERO_TIME);
    return;
  }
  
  // Process pending signals
  if (process_signals())
  {
    // Activate interrupt accepted analysis point
    if (m_interrupt_accepted_analysis_point->has_attachments())
    {
      m_interrupt_accepted_analysis_point->activate();
    }
  
    step_event.notify(SC_ZERO_TIME);
    return;
  }

  // Is the cpu halted or the clock turned off?
  if (halted || (frequency == 0))
  {
    step_event.notify(quantum_period.value);
    return;
  }
  

  // Translate and execute single instruction
  // Clear swd0pcad_exec_count value (for SWDT)
  cpu_state->write_swd0pcad_exec_count(0);

  // Translate and execute single instruction
  try
  {
    advance_by_step();
  }
  catch(std::exception& e)
  {
    sc_core::sc_report_handler::report(sc_core::SC_ERROR, "rh850g4", e.what(), "<UNKNOWN>", 0);
  }

  // Check the number of times that the executed PC is matched SWD0PCAD's value (for SWDT)
  unsigned int count_num = cpu_state->read_swd0pcad_exec_count();
  if( count_num > 0)
  {
    //notify the address match to NSMVG3MCPUV01
    swd0pcad_exec_done_event.notify();
  }

  // Notify debugger that step is complete
  stop_requested = false;
  step_completed_event.notify(get_local_time());
}

void rh850g4::core::continue_thread(void)
{
  while (1)
  {
    // Wait for quantum period to start
    sc_core::wait(continue_event);

    // Call the underlying advance method
    rh850g4::core::base_continue_handler();
  }
}

void rh850g4::core::step_thread(void)
{
  while (1)
  {
    // Wait for quantum period to start
    sc_core::wait(step_event);

    // Call the underlying advance method
    rh850g4::core::base_step_handler();
  }
}

void rh850g4::core::continue_completed_handler(void)
{
  operation_complete_event.notify();
  continue_event.notify(SC_ZERO_TIME);
}

void rh850g4::core::step_completed_handler(void)
{
  operation_complete_event.notify();
  step_event.notify(SC_ZERO_TIME);
}

// ----------------------------------------------------------------------------
// Signal Handlers
// ----------------------------------------------------------------------------

void rh850g4::core::reset_handler(void)
{
  // The time limit is set to 0 in order to terminate the quanta
  local_time_limit = 0;

  if (reset.read())
  {
    // Set CPU reset requested signal
    interrupt_requested |= (INT_RESET | INT_REQ);

    // Ensure CPU is in execute state
    if (halted)
    {
      halted = false;
    }
  }
  else
  {
    // Enter halt state
    halted = true;
  }
}

// Reset the core - currently just resets the register states. This may in the
// future need to be extended to flush the translation cache.
void rh850g4::core::reset_core(void)
{
  // Clear pending interrupts
  interrupt_requested = 0;  

  // Clear pending floating point imprecise exceptions
  pending_fpu_imprecise_exception = 0; // false
  
  // Reset all registers
  cpu_state->write_PC(cpu_state->read_RBASE());
  cpu_state->write_r0(0x00000000);

  // selID=0
  cpu_state->write_EIPSW(0x00000020);
  cpu_state->write_FEPSW(0x00000020);
  cpu_state->write_PSW(0x00000020);
  cpu_state->write_FPSR(0x00220000);
  cpu_state->write_FPCFG(0x00000000);  
  cpu_state->write_EIIC(0x00000000);
  cpu_state->write_FEIC(0x00000000);
  cpu_state->write_PSWH(0x00000000);
  cpu_state->write_CTPSW(0x00000000);
  cpu_state->write_EIPSWH(0x00000000);
  cpu_state->write_FEPSWH(0x00000000);
  cpu_state->write_CTBP(0x00000000);

  // selID=1
  cpu_state->write_MCTL(0x00000000);
  cpu_state->write_SVLOCK(0x00000000);
  cpu_state->write_SCCFG(0x00000000);
  cpu_state->write_SCBP(0x00000000);
  cpu_state->write_GMCFG(0x00000000);

  // selID=2
  cpu_state->write_ISPR(0x00000000);
  cpu_state->write_IMSR(0x00000000);
  cpu_state->write_ICSR(0x00000000);
  if ((cpu_milestone.value==FASTISS_G4MH_1_0) || (cpu_milestone.value==FASTISS_G4MH_1_1)){
    cpu_state->write_INTCFG(0x00000000);
  }else{
    cpu_state->write_INTCFG(0x000F0000);
  }
  cpu_state->write_PLMR(0x00000010);
  cpu_state->write_RBCR0(0x00000000);
  cpu_state->write_RBCR1(0x0000FFFF);
  cpu_state->write_RBNR(0x00000000);

  // selID=4 (CACHE)

  // selID=5 (MPU)
  cpu_state->write_MPM(0x00000000);
  if ((cpu_milestone.value==FASTISS_G4MH_1_0) || (cpu_milestone.value==FASTISS_G4MH_1_1)){
    cpu_state->write_MPCFG(0x00010017);
  }else{
    cpu_state->write_MPCFG(0x0002001f);
  }
  cpu_state->write_MCC(0x00000000);
  cpu_state->write_MPIDX(0x00000000);
  cpu_state->write_MPBK(0x00000000);

  cpu_state->write_GMEIPSW(0x00000020);
  cpu_state->write_GMFEPSW(0x00000020);
  cpu_state->write_GMPSW(0x00000020);
  cpu_state->write_GMEIIC(0x00000000);
  cpu_state->write_GMFEIC(0x00000000);
  cpu_state->write_GMINTCFG(0x000F0002);
  cpu_state->write_GMPLMR(0x000000FF);
  cpu_state->write_GMSVLOCK(0x00000000);
  cpu_state->write_GMMPM(0x00000000);

  // selID=10 (FXU)
  cpu_state->write_FXSR(0x00220000);
  cpu_state->write_FXINFO(0x00000003);
  cpu_state->write_FXCFG(0x00000000);

  // selID=11 (TSC and PMU), selID=14(G4MH2.0 PMU), selID=15(G4MH2.0 PMU)
  cpu_state->write_TSCOUNTL(0x00000000);
  cpu_state->write_TSCOUNTH(0x00000000);
  cpu_state->write_TSCTRL(0x00000000);
  cpu_state->write_PMUMCTRL(0x00000000);
  cpu_state->write_PMGMCTRL(0x00000000);
  cpu_state->write_PMCOUNT0(0x00000000);
  cpu_state->write_PMCTRL0(0x00000000);
  cpu_state->write_PMCOUNT1(0x00000000);
  cpu_state->write_PMCTRL1(0x00000000);
  cpu_state->write_PMCOUNT2(0x00000000);
  cpu_state->write_PMCTRL2(0x00000000);
  cpu_state->write_PMCOUNT3(0x00000000);
  cpu_state->write_PMCTRL3(0x00000000);
  cpu_state->write_PMCOUNT4(0x00000000);
  cpu_state->write_PMCTRL4(0x00000000);
  cpu_state->write_PMCOUNT5(0x00000000);
  cpu_state->write_PMCTRL5(0x00000000);
  cpu_state->write_PMCOUNT6(0x00000000);
  cpu_state->write_PMCTRL6(0x00000000);
  cpu_state->write_PMCOUNT7(0x00000000);
  cpu_state->write_PMCTRL7(0x00000000);
  cpu_state->write_PMSUBCND0(0x00000000);
  cpu_state->write_PMSUBCND1(0x00000000);
  cpu_state->write_PMSUBCND2(0x00000000);
  cpu_state->write_PMSUBCND3(0x00000000);
  cpu_state->write_PMSUBCND4(0x00000000);
  cpu_state->write_PMSUBCND5(0x00000000);
  cpu_state->write_PMSUBCND6(0x00000000);
  cpu_state->write_PMSUBCND7(0x00000000);

  // selID=12
  cpu_state->write_LSTEST0(0x00000000);
  cpu_state->write_LSTEST1(0x00000000);
  if ((cpu_milestone.value==FASTISS_G4MH_1_0) || (cpu_milestone.value==FASTISS_G4MH_1_1)){
    cpu_state->write_LSCFG(0x00000000);
  }else{
    cpu_state->write_LSCFG(0x00000001);
  }
  cpu_state->write_ICBKEY(0x00000000);
  cpu_state->write_IFCR(0x00000001);
  cpu_state->write_IFCR1(0x00000000);
  cpu_state->write_BRPCTRL0(0x00000001);
  cpu_state->write_BRPCTRL1(0x00000000);
  cpu_state->write_BRPCFG(0x00001122);
  cpu_state->write_BRPACTRL(0x00000000);

  // selID=13
  cpu_state->write_DCBKEY(0x00000000);
  cpu_state->write_LSUCR(0x00000000);
  cpu_state->write_L1RLCR(0x00000000);
  if ((cpu_milestone.value==FASTISS_G4MH_1_0) || (cpu_milestone.value==FASTISS_G4MH_1_1)){
    cpu_state->write_L1RCFG(0x00000000);
  }else{
    cpu_state->write_L1RCFG(0x00004004);
  }
  cpu_state->write_DECFG(0x00000000);
  cpu_state->write_DECTRL(0x00000000);
  cpu_state->write_DEVDS(0x00000000);
  cpu_state->write_RDBCR(0x00000101);
  cpu_state->write_RDBACR(0x00000000);
  cpu_state->write_RDBSTAT(0x00000000);

  // swd0pcad_exec_count
  cpu_state->write_swd0pcad_exec_count(0);

  // Activate reset completed analysis point
  if (m_reset_completed_analysis_point->has_attachments())
  {
    m_reset_completed_analysis_point->activate();
  }    
    
  // Notify INTC immediately that the cpu mode (PSWH) is changed
  cpugminfo_update_event.notify();
    
  // Notify debugger immediately that reset request is complete
  reset_done_event.notify();
}

void rh850g4::core::enter_halt(void)
{
  // The time limit is set to 0 in order to terminate the quanta
  local_time_limit = 0;

  // Enter the halt state
  halted = true;
}

void rh850g4::core::fenmirq_update(void)
{
  bool fenmirq_sig_val = fenmirq.read();
  if (fenmirq_sig_val)
  {
    interrupt_requested |= (INT_FENMI | INT_REQ);
  }
  else
  {
    if((interrupt_requested & INT_FENMI) == INT_FENMI){
      interrupt_requested -= INT_FENMI;
    }
    if(((interrupt_requested & (INT_RESET | INT_FEINT | INT_GMFEINT | INT_BGFEINT | INT_EIINT | INT_GMEIINT | INT_BGEIINT | INT_SYSERR)) == 0) &&
       ((interrupt_requested & INT_REQ) == INT_REQ)){
      interrupt_requested -= INT_REQ;
    }
  }
}

void rh850g4::core::feintrq_update(void)
{
  union FeintReq feintrq_sig_val;
  feintrq_sig_val = feintrq.read();
  
  if (feintrq_sig_val.bit_val.req & 0x1)
  {
    if ((cpu_milestone.value==FASTISS_G4MH_1_0) || (cpu_milestone.value==FASTISS_G4MH_1_1)){
      interrupt_requested |= (INT_FEINT | INT_REQ);
    }else{
      if (feintrq_sig_val.bit_val.gm & 0x1){
	interrupt_requested |= (INT_GMFEINT | INT_REQ);
      }else if (feintrq_sig_val.bit_val.bg & 0x1){
	interrupt_requested |= (INT_BGFEINT | INT_REQ);
      }else{
	interrupt_requested |= (INT_FEINT | INT_REQ);
      }
    }
    feintrq_info = feintrq_sig_val.val;
  }
  else
  {
    if((interrupt_requested & INT_FEINT) == INT_FEINT){
      interrupt_requested -= INT_FEINT;
      cpu_state->write_IMSR_HFNP(0);
      cpu_state->write_IMSR_FNP(0);
    }
    if((interrupt_requested & INT_GMFEINT) == INT_GMFEINT){
      interrupt_requested -= INT_GMFEINT;
      cpu_state->write_IMSR_FNP(0);
    }
    if((interrupt_requested & INT_BGFEINT) == INT_BGFEINT){
      interrupt_requested -= INT_BGFEINT;
      cpu_state->write_IMSR_HENP(0);
      cpu_state->write_IMSR_HEID(0);
      cpu_state->write_IMSR_FNP(0);
    }
    if(((interrupt_requested & (INT_RESET | INT_FENMI | INT_EIINT | INT_GMEIINT | INT_BGEIINT | INT_SYSERR)) == 0) &&
       ((interrupt_requested & INT_REQ) == INT_REQ)){
      interrupt_requested -= INT_REQ;
    }
    feintrq_info = feintrq_sig_val.val;
  }
}

void rh850g4::core::eiintrq_update(void)
{
  union EiintReq eiintrq_sig_val;
  eiintrq_sig_val.val = eiintrq.read();
  
  if (eiintrq_sig_val.bit_val.req & 0x1)
  {
    if ((cpu_milestone.value==FASTISS_G4MH_1_0) || (cpu_milestone.value==FASTISS_G4MH_1_1)){
      interrupt_requested |= (INT_EIINT | INT_REQ);
    }else{
      if (eiintrq_sig_val.bit_val.gm & 0x1){
	interrupt_requested |= (INT_GMEIINT | INT_REQ);
      }else if (eiintrq_sig_val.bit_val.bg & 0x1){
	interrupt_requested |= (INT_BGEIINT | INT_REQ);
      }else{
	interrupt_requested |= (INT_EIINT | INT_REQ);
      }
    }
    eiintrq_info = eiintrq_sig_val.val;
  }
  else
  {
    if((interrupt_requested & INT_EIINT) == INT_EIINT){
      interrupt_requested -= INT_EIINT;
      cpu_state->write_IMSR_HENP(0);
      cpu_state->write_IMSR_HEID(0);
      cpu_state->write_IMSR_HEPLM(0);
      cpu_state->write_IMSR_HEEIM(0);
      cpu_state->write_IMSR_ENP(0);
      cpu_state->write_IMSR_EID(0);
      cpu_state->write_IMSR_EPLM(0);
      cpu_state->write_IMSR_EEIM(0);
    }
    if((interrupt_requested & INT_GMEIINT) == INT_GMEIINT){
      interrupt_requested -= INT_GMEIINT;
      cpu_state->write_IMSR_ENP(0);
      cpu_state->write_IMSR_EID(0);
      cpu_state->write_IMSR_EPLM(0);
      cpu_state->write_IMSR_EEIM(0);
    }
    if((interrupt_requested & INT_BGEIINT) == INT_BGEIINT){
      interrupt_requested -= INT_BGEIINT;
      cpu_state->write_IMSR_HENP(0);
      cpu_state->write_IMSR_HEID(0);
      cpu_state->write_IMSR_ENP(0);
      cpu_state->write_IMSR_EID(0);
      cpu_state->write_IMSR_EPLM(0);
      cpu_state->write_IMSR_EEIM(0);
     }
    if(((interrupt_requested & (INT_RESET | INT_FENMI | INT_FEINT | INT_GMFEINT | INT_BGFEINT | INT_SYSERR)) == 0) &&
       ((interrupt_requested & INT_REQ) == INT_REQ)){
      interrupt_requested -= INT_REQ;
    }
    if((eiintrq_sig_val.bit_val.pri) <= (unsigned int)cpu_state->read_PLMR_PLM()){
      cpu_state->write_ICSR_PMEI(0);
    }
    eiintrq_info = eiintrq_sig_val.val;
  }
}

void rh850g4::core::syserr_req_update(void)
{
  bool syserr_req_val = syserr_req.read();

  if (syserr_req_val)
  {
    interrupt_requested |= (INT_SYSERR | INT_REQ);
  }
  else
  {
    if((interrupt_requested & INT_SYSERR) == INT_SYSERR){
      interrupt_requested -= INT_SYSERR;
    }
    if(((interrupt_requested & (INT_RESET | INT_FENMI | INT_FEINT | INT_GMFEINT | INT_BGFEINT | INT_EIINT | INT_GMEIINT | INT_BGEIINT)) == 0) &&
       ((interrupt_requested & INT_REQ) == INT_REQ)){
      interrupt_requested -= INT_REQ;
    }
  }
}

void rh850g4::core::syserr_code_update(void)
{
  syserr_code_val = (unsigned int)syserr_code.read();
}

void rh850g4::core::syserr_mode_update(void)
{
  // true  : Fetch access
  // false : Operand access
  syserr_mode_val = syserr_mode.read();
}

void rh850g4::core::fenmirq_ack_update(void)
{
  fenmirq_ack.write(intack_fenmi_requested);
  if(intack_fenmi_requested){
    intack_fenmi_requested = false;
    fenmirq_ack_event.notify(cpu_state->read_cycle_period(), sc_core::SC_PS);
  }
}

void rh850g4::core::feintrq_ack_update(void)
{
  feintrq_ack.write(intack_feint_requested);
  if(intack_feint_requested){
    intack_feint_requested = 0;
    feintrq_ack_event.notify(cpu_state->read_cycle_period(), sc_core::SC_PS);
  }
}


void rh850g4::core::eiintrq_ack_update(void)
{
  eiintrq_ack.write(intack_eiint_requested);
  if(intack_eiint_requested){
    intack_eiint_requested = 0;
    eiintrq_ack_event.notify(cpu_state->read_cycle_period(), sc_core::SC_PS);
  }
}


void rh850g4::core::cpugminfo_update(void)
{
  union CpuGmInfo gm_info;
  unsigned int pswh_val = cpu_state->read_PSWH();
  gm_info.bit_val.cpu_gm   = ((pswh_val >> 31) & 0x1);
  gm_info.bit_val.cpu_gpid = ((pswh_val >>  8) & 0x7);
  cpugminfo.write(gm_info.val);
}


void rh850g4::core::syserr_ack_update(void)
{
  syserr_ack.write(intack_syserr_requested);
  if(intack_syserr_requested){
    intack_syserr_requested = false;
    syserr_ack_event.notify(cpu_state->read_cycle_period(), sc_core::SC_PS);
  }
}

// ----------------------------------------------------------------------------
// Signal management
// ----------------------------------------------------------------------------
bool rh850g4::core::process_signals(void)
{
  bool process_further_signals = false;


  // Did we hit an interrupt or reset?
  if ((interrupt_requested & INT_REQ) || pending_fpu_imprecise_exception){
    unsigned char prev_hv_func_support = cpu_state->read_hv_func_support();
    unsigned char is_receipt_cpu_mode_guest = 0;
    if((cpu_state->read_PSWH_GM() & 0x1) == 0x1) is_receipt_cpu_mode_guest = 1;

    cpu_state->write_hv_func_support(0);
    unsigned char psw_ebv = cpu_state->read_PSW_EBV();
    unsigned int  handler_addr;
    bool handler_rint = false;
    bool handler_dv = false;

    if(!psw_ebv){
      handler_addr = cpu_state->read_RBASE();
    }else{
      handler_addr = cpu_state->read_EBASE();
    }
    cpu_state->write_hv_func_support(prev_hv_func_support);

    if((handler_addr & 0x1) == 0x1) handler_rint = true; // RBASE.RINT or EBASE.RINT
    if((handler_addr & 0x2) == 0x2) handler_dv = true;   // RBASE.DV   or EBASE.DV
    handler_addr &= 0xfffffe00;

    if (cpu_milestone.value==FASTISS_G4MH_2_0){
      bool is_decfg_valid = false;
      if((interrupt_requested & INT_SYSERR) && (!syserr_mode_val)){
	if (!syserr_mode_val){
	  if((syserr_code_val == 0x18) && ((cpu_state->read_DECFG() & 0x1) == 0x1)){
	    is_decfg_valid = true;
	  } else if((syserr_code_val == 0x19) && ((cpu_state->read_DECFG() & 0x2) == 0x2)){
	    is_decfg_valid = true;
	  } else if((syserr_code_val == 0x1a) && ((cpu_state->read_DECFG() & 0x4) == 0x4)){
	    is_decfg_valid = true;
	  }
	}
      }
      if(is_decfg_valid){
	cpu_state->write_DECTRL(0x1);
	if(is_receipt_cpu_mode_guest == 0){
	  cpu_state->write_DEVDS(0x10000);
	}else{
	  cpu_state->write_DEVDS(0x1 << cpu_state->read_PSWH_GPID());
	}
      }
    }

    if(interrupt_requested & INT_RESET){ // Handle reset
      // change host mode
      cpu_state->write_PSWH_GM(0);

      // Clear pending signals
      interrupt_requested = 0;

      // Reset the core
      reset_core();      

      // Acknowledge interrupt clear
      intack_fenmi_requested = false;
      fenmirq_ack_event.notify();
      intack_feint_requested = 0;
      feintrq_ack_event.notify();
      intack_eiint_requested = 0;
      eiintrq_ack_event.notify();
      intack_syserr_requested = 0;
      syserr_ack_event.notify();

      // Indicate that we processed a signal
      process_further_signals = true;

    }else if(interrupt_requested & INT_FENMI){ // Handle FENMI
      // change host mode
      cpu_state->write_PSWH_GM(0);

      // Clear pending signals
      interrupt_requested -= INT_FENMI;

      // Update registers
      cpu_state->write_FEPC(cpu_state->read_PC());
      cpu_state->write_FEPSW(cpu_state->read_PSW());
      cpu_state->write_FEPSWH_GM(is_receipt_cpu_mode_guest);
      cpu_state->write_FEPSWH_GPID(cpu_state->read_PSWH_GPID());
      cpu_state->write_FEIC(0xe0);
      cpu_state->write_PSW_UM(0);
      cpu_state->write_PSW_NP(1);
      cpu_state->write_PSW_ID(1);
      cpu_state->write_PSW_EP(0);
      cpu_state->write_PC(handler_addr+0xe0);
      
      // Acknowledge interrupt
      intack_fenmi_requested = true;
      fenmirq_ack_event.notify();

      // Indicate that we processed a signal
      process_further_signals = true;
      
      // Clear the link(ll-bit) information
      clear_llbit();

      // Clear halt flag
      halted = false;
    }else if(interrupt_requested & INT_FEINT){ // Handle FEINT
      process_further_signals = feint_operation(is_receipt_cpu_mode_guest, handler_addr);
    }else if((interrupt_requested & INT_SYSERR) && 
	     (is_receipt_cpu_mode_guest == 0) &&
	     (!syserr_mode_val)){ // Handle SYSERR for operand access (host mode)
      process_further_signals = syserr_operation(is_receipt_cpu_mode_guest, handler_addr);
    }else if((interrupt_requested & INT_EIINT) ||
	     (pending_fpu_imprecise_exception)){ // Handle EIINT or FPI (G4MH isn't support)
      process_further_signals = eiint_operation(is_receipt_cpu_mode_guest, handler_addr, handler_rint, handler_dv);
    }else if(interrupt_requested & INT_BGFEINT){ // Handle BGFEINT
      process_further_signals = bgfeint_operation(is_receipt_cpu_mode_guest, handler_addr);
    }else if(interrupt_requested & INT_BGEIINT){ // Handle BGEIINT
      process_further_signals = bgeiint_operation(is_receipt_cpu_mode_guest, handler_addr);
    }else if(interrupt_requested & INT_GMFEINT){ // Handle GMFEINT
      process_further_signals = gmfeint_operation(is_receipt_cpu_mode_guest);
    }else if((interrupt_requested & INT_SYSERR) && 
	     (is_receipt_cpu_mode_guest == 1) &&
	     (!syserr_mode_val)){ // Handle SYSERR for operand access (guest mode)
      process_further_signals = syserr_operation(is_receipt_cpu_mode_guest, handler_addr);
    }else if(interrupt_requested & INT_GMEIINT){ // Handle GMEIINT
      process_further_signals = gmeiint_operation(is_receipt_cpu_mode_guest);
    }else if((interrupt_requested & INT_SYSERR) && (syserr_mode_val)){ // Handle SYSERR for fetch access
      process_further_signals = syserr_operation(is_receipt_cpu_mode_guest, handler_addr);
    }
    if(interrupt_requested == INT_REQ){
      // Clear pending request flag
      interrupt_requested -= INT_REQ;
    }
  }
  // Notify INTC immediately that the cpu mode (PSWH) is changed
  cpugminfo_update_event.notify();
  return process_further_signals;
}


bool rh850g4::core::syserr_operation(unsigned char is_receipt_cpu_mode_guest, unsigned int handler_addr)
{
  bool process_further_signals = false;

  if ((cpu_milestone.value==FASTISS_G4MH_1_0) || (cpu_milestone.value==FASTISS_G4MH_1_1)){
    // change host mode
    cpu_state->write_PSWH_GM(0);

    // Clear pending signals
    interrupt_requested -= INT_SYSERR;
    
    // Update registers
    cpu_state->write_FEPC(cpu_state->read_PC());
    cpu_state->write_FEPSW(cpu_state->read_PSW());
    cpu_state->write_FEPSWH_GM(is_receipt_cpu_mode_guest);
    cpu_state->write_FEPSWH_GPID(cpu_state->read_PSWH_GPID());
    cpu_state->write_FEIC(syserr_code_val & 0x1f);
    cpu_state->write_PSW_UM(0);
    cpu_state->write_PSW_NP(1);
    cpu_state->write_PSW_ID(1);
    cpu_state->write_PSW_EP(1);
    cpu_state->write_PC(handler_addr+0x10);
    
    // Acknowledge interrupt
    intack_syserr_requested = true;
    syserr_ack_event.notify();
    
    // Indicate that we processed a signal
    process_further_signals = true;
    
    // Clear the link(ll-bit) information
    clear_llbit();
  }else{
    if((is_receipt_cpu_mode_guest == 0) ||
       ((is_receipt_cpu_mode_guest == 1) && ((cpu_state->read_GMCFG_GSYSE() & 0x1) == 1))){
      // host mode or guest mode & GMCFG.GSYSE=1
      
      unsigned char prev_hv_func_support = cpu_state->read_hv_func_support();
      cpu_state->write_hv_func_support(0);
      
      if ((syserr_mode_val) || 
	  ((!syserr_mode_val) && (cpu_state->read_PSW_NP() == 0) && (cpu_state->read_DEVDS() != 0))){
	
	cpu_state->write_hv_func_support(prev_hv_func_support);
	
	// change host mode
	cpu_state->write_PSWH_GM(0);
	
	// Clear pending signals
	interrupt_requested -= INT_SYSERR;
	
	// Update registers
	cpu_state->write_FEPC(cpu_state->read_PC());
	cpu_state->write_FEPSW(cpu_state->read_PSW());
	cpu_state->write_FEPSWH_GM(is_receipt_cpu_mode_guest);
	cpu_state->write_FEPSWH_GPID(cpu_state->read_PSWH_GPID());
	cpu_state->write_FEIC(syserr_code_val & 0x1f);
	cpu_state->write_PSW_UM(0);
	cpu_state->write_PSW_NP(1);
	cpu_state->write_PSW_ID(1);
	cpu_state->write_PSW_EP(1);
	cpu_state->write_PC(handler_addr+0x10);
	if(!syserr_mode_val){
	  cpu_state->write_DECTRL(0);
	  cpu_state->write_DEVDS(0);
	}
	
	// Acknowledge interrupt
	intack_syserr_requested = true;
	syserr_ack_event.notify();
	
	// Indicate that we processed a signal
	process_further_signals = true;
	
	// Clear the link(ll-bit) information
	clear_llbit();
      }else{
	// not accept
	cpu_state->write_hv_func_support(prev_hv_func_support);
      }
    }else{
      // guest mode & GMCFG.GSYSE=0
      cpu_state->write_PSWH_GM(is_receipt_cpu_mode_guest);
      
      if ((syserr_mode_val) || 
	  ((!syserr_mode_val) && (cpu_state->read_GMPSW_NP() == 0) && (cpu_state->read_DEVDS() != 0))){
	
	// Clear pending signals
	interrupt_requested -= INT_SYSERR;
	
	handler_addr = (cpu_state->read_GMEBASE() & 0xfffffe00);
	// Update registers
	cpu_state->write_PSWH_GM(1);
	cpu_state->write_GMFEPC(cpu_state->read_PC());
	cpu_state->write_GMFEPSW(cpu_state->read_GMPSW());
	cpu_state->write_GMFEIC(syserr_code_val & 0x1f);
	cpu_state->write_GMPSW_UM(0);
	cpu_state->write_GMPSW_NP(1);
	cpu_state->write_GMPSW_ID(1);
	cpu_state->write_GMPSW_EP(1);
	cpu_state->write_PC(handler_addr+0x10);
	if(!syserr_mode_val){
	  cpu_state->write_DECTRL(0);
	  cpu_state->write_DEVDS(0);
	}
	
	// Acknowledge interrupt
	intack_syserr_requested = true;
	syserr_ack_event.notify();
	
	// Indicate that we processed a signal
	process_further_signals = true;
	
	// Clear the link(ll-bit) information
	clear_llbit();
      }else{
	// not accept
      }
    }
  }
  // Clear halt flag
  halted = false;

  return process_further_signals;
}

bool rh850g4::core::feint_operation(unsigned char is_receipt_cpu_mode_guest, unsigned int handler_addr)
{
  bool process_further_signals = false;
  unsigned char prev_hv_func_support = cpu_state->read_hv_func_support();
  cpu_state->write_hv_func_support(0);

  if (cpu_state->read_PSW_NP() == 0){
    union FeintReq feintreq_val;
    cpu_state->write_hv_func_support(prev_hv_func_support);
    feintreq_val.val = feintrq_info;
    // Clear pending signals
    interrupt_requested -= INT_FEINT;
    feintrq_info = 0;
    
    // change host mode
    cpu_state->write_PSWH_GM(0);

    // Update registers
    cpu_state->write_FEPC(cpu_state->read_PC());
    cpu_state->write_FEPSW(cpu_state->read_PSW());
    cpu_state->write_FEIC(0xf0+feintreq_val.bit_val.ch);
    cpu_state->write_FEPSWH_GM(is_receipt_cpu_mode_guest);
    cpu_state->write_FEPSWH_GPID(cpu_state->read_PSWH_GPID());
    cpu_state->write_PSW_UM(0);
    cpu_state->write_PSW_NP(1);
    cpu_state->write_PSW_ID(1);
    cpu_state->write_PSW_EP(0);
    cpu_state->write_PC(handler_addr+0xf0);
    cpu_state->write_IMSR_HFNP(0);
    cpu_state->write_IMSR_FNP(0);
    
    // Acknowledge interrupt
    union FeintAck feintack_val;
    feintack_val.bit_val.ack  = true;
    feintack_val.bit_val.ch   = feintreq_val.bit_val.ch;
    intack_feint_requested = feintack_val.val;
    feintrq_ack_event.notify();
      
    // Indicate that we processed a signal
    process_further_signals = true;
    
    // Clear the link(ll-bit) information
    clear_llbit();
  }else{
    // not accept
    cpu_state->write_hv_func_support(prev_hv_func_support);
    if(((cpu_state->read_HVCFG_HVE() & 0x1) == 0x1) && (is_receipt_cpu_mode_guest == 0x1)){
      cpu_state->write_IMSR(cpu_state->read_IMSR() & 0x00000f0f);
      cpu_state->write_IMSR_HFNP(1);
    }else {
      if ((cpu_milestone.value != FASTISS_G4MH_1_0) && (cpu_milestone.value != FASTISS_G4MH_1_1)){
	cpu_state->write_IMSR(cpu_state->read_IMSR() & 0x0000000f);
	cpu_state->write_IMSR_FNP(1);
      }
    }
  }
  
  // Clear halt flag
  halted = false;

  return process_further_signals;
}

bool rh850g4::core::eiint_operation(unsigned char is_receipt_cpu_mode_guest, unsigned int handler_addr, bool handler_rint, bool handler_dv)
{
  bool process_further_signals = false;

  union EiintReq eireq_val;
  bool is_ei_pri_masked = false;
  unsigned int ei_pri_val = 0;
  unsigned char prev_hv_func_support = cpu_state->read_hv_func_support();
  
  eireq_val.val = eiintrq_info;
  
  if(interrupt_requested & INT_EIINT){
    ei_pri_val = eireq_val.bit_val.pri;
  }
  
  cpu_state->write_hv_func_support(0);
  // check mask setting(ISPR or PSW.EIMASK)
  if ((cpu_milestone.value == FASTISS_G4MH_1_0) || 
      (cpu_milestone.value == FASTISS_G4MH_1_1) || 
      (cpu_state->read_INTCFG_EPL()==0)){
    for(int i=0; i<16; i++){
      if((cpu_state->read_ISPR_ISP() & (0x1 << i))){
	if(ei_pri_val >= i){
	  is_ei_pri_masked = true;
	  if ((cpu_milestone.value != FASTISS_G4MH_1_0) && (cpu_milestone.value != FASTISS_G4MH_1_1)){
	    if(((cpu_state->read_HVCFG_HVE() & 0x1) == 0x1) && (is_receipt_cpu_mode_guest == 1)) {
	      cpu_state->write_IMSR(cpu_state->read_IMSR() & 0x00001010);
	      cpu_state->write_IMSR_HEEIM(1);
	    }else{
	      cpu_state->write_IMSR(cpu_state->read_IMSR() & 0x00000010);
	      cpu_state->write_IMSR_EEIM(1);
	    }
	  }
	  break;
	}
      }
    }
  }else{
    unsigned int eimask_val = (unsigned int)cpu_state->read_PSW_EIMASK();
    if(ei_pri_val >= eimask_val){
      is_ei_pri_masked = true;
      if(((cpu_state->read_HVCFG_HVE() & 0x1) == 0x1) && (is_receipt_cpu_mode_guest == 1)) {
	cpu_state->write_IMSR(cpu_state->read_IMSR() & 0x00001010);
	cpu_state->write_IMSR_HEEIM(1);
      }else{
	cpu_state->write_IMSR(cpu_state->read_IMSR() & 0x00000010);
	cpu_state->write_IMSR_EEIM(1);
      }
    }
  }

  // check mask setting(PLMR)
  if(!is_ei_pri_masked){
    unsigned int plmr_plm_val = (unsigned int)cpu_state->read_PLMR_PLM();
    if(ei_pri_val >= plmr_plm_val){
      if((cpu_state->read_HVCFG_HVE() & 0x1) == 0x0) {
	cpu_state->write_ICSR_PMEI(1);
      }
      if ((cpu_milestone.value != FASTISS_G4MH_1_0) && (cpu_milestone.value != FASTISS_G4MH_1_1)){
	if(((cpu_state->read_HVCFG_HVE() & 0x1) == 0x1) && (is_receipt_cpu_mode_guest == 1)) {
	  cpu_state->write_IMSR(cpu_state->read_IMSR() & 0x00001010);
	  cpu_state->write_IMSR_HEPLM(1);
	}else{
	  cpu_state->write_IMSR(cpu_state->read_IMSR() & 0x00000010);
	  cpu_state->write_IMSR_EPLM(1);
	}
      }
      is_ei_pri_masked = true;
    }
  }

  // accept the exeption of FPI
  if (pending_fpu_imprecise_exception){ // not reached, because G4MH doesn't support FPI
    if ((cpu_state->read_PSW_NP() | cpu_state->read_PSW_ID()) == 0){
      // Clear pending floating point imprecise_exceptions
      pending_fpu_imprecise_exception = 0; // false
      
      cpu_state->write_hv_func_support(prev_hv_func_support);
      // change host mode
      cpu_state->write_PSWH_GM(0);

      // Update registers
      cpu_state->write_EIPC(cpu_state->read_PC());
      cpu_state->write_EIPSW(cpu_state->read_PSW());
      cpu_state->write_EIPSWH_GM(is_receipt_cpu_mode_guest);
      cpu_state->write_EIPSWH_GPID(cpu_state->read_PSWH_GPID());
      cpu_state->write_EIIC(0x72);
      cpu_state->write_PSW_UM(0);
      cpu_state->write_PSW_ID(1);
      cpu_state->write_PSW_EP(1);
      cpu_state->write_PC(handler_addr+0x00000070);
      
      // Indicate that we processed a signal
      process_further_signals = true;
      
      // Clear the link(ll-bit) information
      clear_llbit();
      
      // Don't process further external interrupts in the quanta
    }
    // Clear halt flag
    halted = false;
  }      

  // EIINT 
  cpu_state->write_hv_func_support(0);
  if ((interrupt_requested & INT_EIINT) && (!is_ei_pri_masked)){
    if (cpu_state->read_PSW_ID() == 0){
      if (cpu_state->read_PSW_NP() == 0){
	bool syserr_flag = false;
	bool nc0_flag = false;
	
	cpu_state->write_hv_func_support(prev_hv_func_support);
	// change host mode
	cpu_state->write_PSWH_GM(0);

	// get current PC
	unsigned int ret_pc = cpu_state->read_PC();
	if((eireq_val.bit_val.tb) && (!handler_dv) && (!is_ei_pri_masked)){
	  // G4MH 1.x and G4MH2.0 with HVCFG.HVE=0 support bank operation for EIINT -->
	  if ((cpu_milestone.value==FASTISS_G4MH_1_0) ||
	      (cpu_milestone.value==FASTISS_G4MH_1_1) ||
	      ((cpu_milestone.value==FASTISS_G4MH_2_0) && (cpu_state->read_HVCFG_HVE() == 0x0))) {
	    // determin the vector address from interrupt table
	    unsigned int pri_bit = 0x1 << ei_pri_val;
	    unsigned int be_val = cpu_state->read_RBCR0_BE();
	    unsigned int nc_val = cpu_state->read_RBCR1_NC();
	    unsigned int bn_val = cpu_state->read_RBNR() & 0x3f;
	    if((cpu_milestone.value != FASTISS_G4MH_1_0) && (cpu_milestone.value != FASTISS_G4MH_1_1)){
	      if(ei_pri_val > 15) pri_bit = 0x8000;
	    }
	    if((be_val & pri_bit) == pri_bit) {
	      // saving context
	      if((cpu_milestone.value==FASTISS_G4MH_1_0) || (cpu_milestone.value==FASTISS_G4MH_1_1)){
		 if(bn_val > 15){
		   // SYSERR
		   syserr_flag = true;
		 }
	      }else{
		if((bn_val >= 63) || (bn_val > cpu_state->read_INTCFG_ULNR())){
		  // SYSERR
		  syserr_flag = true;
		}
	      }
	      if(!syserr_flag){
		if((nc_val & pri_bit) == 0){
		  nc0_flag = true;
		}
		cpu_state->write_mem_acc_type(6);
		unsigned int addr = cpu_state->read_RBIP();
		unsigned int md_val = (cpu_state->read_RBCR0() >> 16) & 0x1;
		if(md_val == 0){
		  addr = addr - (bn_val * 0x60);
		}else{
		  addr = addr - (bn_val * 0x90);
		}
		addr = addr - 4;
		fastiss_memory->i32_store(addr, cpu_state->read_PC());
		addr = addr - 4;
		fastiss_memory->i32_store(addr, cpu_state->read_PSW());
		addr = addr - 4;
		fastiss_memory->i32_store(addr, cpu_state->read_EIIC());
		addr = addr - 4;
		fastiss_memory->i32_store(addr, cpu_state->read_FPSR());
		addr = addr - 4;
		for(int i=1; i<32; i++){
		  if((md_val == 0) &&  (i > 19)){
		    fastiss_memory->i32_store(addr, cpu_state->read_GR(GR_index_t(30)));
		    addr = addr - 4;
		    break;
		  }
		  fastiss_memory->i32_store(addr, cpu_state->read_GR(GR_index_t(i)));
		  addr = addr - 4;
		}
		if((cpu_milestone.value == FASTISS_G4MH_1_0) || (cpu_milestone.value == FASTISS_G4MH_1_1)){
		  cpu_state->write_RBNR((bn_val + 1) & 0x1f);
		}else{
		  cpu_state->write_RBNR((bn_val + 1) & 0x3f);
		}
	      }
	    }else{
	      // nothing
	    }
	  }
	  // <-- G4MH 1.x support bank operation for EIINT
	  // update PC for table reference mode
	  if(!syserr_flag){
	    cpu_state->write_mem_acc_type(2);
	    handler_addr = fastiss_memory->i32_load(cpu_state->read_INTBP()+(eireq_val.bit_val.ch*4));
	    cpu_state->write_PC(handler_addr);
	  }
	}else{
	  // determin the vector address from vector directly
	  if(handler_rint){
	    cpu_state->write_PC(handler_addr+0x100);
	  }else{
	    if(eireq_val.bit_val.pri < 16){
	      cpu_state->write_PC(handler_addr+0x100+(eireq_val.bit_val.pri*0x10));
	    }else{
	      cpu_state->write_PC(handler_addr+0x100+(15*0x10));
	    }
	  }
	}
	
	if(syserr_flag){
	  // Update registers
	  cpu_state->write_FEPC(ret_pc);
	  cpu_state->write_FEPSW(cpu_state->read_PSW());
	  cpu_state->write_FEIC(0x1C);
	  cpu_state->write_FEPSWH_GM(is_receipt_cpu_mode_guest);
	  cpu_state->write_FEPSWH_GPID(cpu_state->read_PSWH_GPID());
	  cpu_state->write_PSW_UM(0);
	  cpu_state->write_PSW_ID(1);
	  cpu_state->write_PSW_NP(1);
	  cpu_state->write_PSW_EP(1);
	  cpu_state->write_PC(handler_addr+0x10);
	}else{
	  // Update registers
	  cpu_state->write_EIPC(ret_pc);
	  cpu_state->write_EIPSW(cpu_state->read_PSW());
	  cpu_state->write_EIIC(0x1000+eireq_val.bit_val.ch);
	  cpu_state->write_EIPSWH_GM(is_receipt_cpu_mode_guest);
	  cpu_state->write_EIPSWH_GPID(cpu_state->read_PSWH_GPID());
	  cpu_state->write_PSW_UM(0);
	  if ((cpu_milestone.value==FASTISS_G4MH_1_0) || 
	      (cpu_milestone.value==FASTISS_G4MH_1_1) ||
	      ((cpu_milestone.value==FASTISS_G4MH_2_0) && (cpu_state->read_HVCFG_HVE() == 0x0))) {
	    if(nc0_flag) cpu_state->write_PSW_ID(0);
	    else         cpu_state->write_PSW_ID(1);
	  }else{
	    cpu_state->write_PSW_ID(1);
	  }
	  cpu_state->write_PSW_EP(0);
	  cpu_state->write_ICSR_PMEI(0);
	  cpu_state->write_IMSR_HENP(0);
	  cpu_state->write_IMSR_HEID(0);
	  cpu_state->write_IMSR_HEPLM(0);
	  cpu_state->write_IMSR_HEEIM(0);
	  cpu_state->write_IMSR_ENP(0);
	  cpu_state->write_IMSR_EID(0);
	  cpu_state->write_IMSR_EPLM(0);
	  cpu_state->write_IMSR_EEIM(0);
	  
	  // Clear pending signals
	  interrupt_requested -= INT_EIINT;
	  eiintrq_info = 0;
	  
	  // update ISPR or PSW.EIMASK
	  if ((cpu_milestone.value == FASTISS_G4MH_1_0) || 
	      (cpu_milestone.value == FASTISS_G4MH_1_1) ||
	      (cpu_state->read_INTCFG_EPL() == 0)){
	    if(cpu_state->read_INTCFG_ISPC() == 0){
	      if (ei_pri_val < 16){
		cpu_state->write_ISPR_ISP((cpu_state->read_ISPR_ISP()) | (0x1 << ei_pri_val));
	      }
	    }
	  }else{
	    cpu_state->write_PSW_EIMASK(ei_pri_val);
	  }
	  
	  // Acknowledge interrupt
	  union EiintAck eiack_val;
	  eiack_val.bit_val.ack  = true;
	  eiack_val.bit_val.ch   = eireq_val.bit_val.ch;
	  intack_eiint_requested = eiack_val.val;
	  eiintrq_ack_event.notify();
	}
	
	// Indicate that we processed a signal
	process_further_signals = true;
	
	// Clear the link(ll-bit) information
	clear_llbit();
      }else{
	// not accept (PSW.NP=1)
	cpu_state->write_hv_func_support(prev_hv_func_support);
	if(((cpu_state->read_HVCFG_HVE() & 0x1) == 0x1) && (is_receipt_cpu_mode_guest == 0x1)){
	  cpu_state->write_IMSR(cpu_state->read_IMSR() & 0x00001010);
	  cpu_state->write_IMSR_HENP(1);
	}else {
	  if ((cpu_milestone.value != FASTISS_G4MH_1_0) && (cpu_milestone.value != FASTISS_G4MH_1_1)){
	    cpu_state->write_IMSR(cpu_state->read_IMSR() & 0x00000010);
	    cpu_state->write_IMSR_ENP(1);
	  }
	}
      }
    }else{
      // not accept (PSW.ID=1)
      cpu_state->write_hv_func_support(prev_hv_func_support);
      if(((cpu_state->read_HVCFG_HVE() & 0x1) == 0x1) && (is_receipt_cpu_mode_guest == 0x1)){
	cpu_state->write_IMSR(cpu_state->read_IMSR() & 0x00001010);
	cpu_state->write_IMSR_HEID(1);
      }else {
	if ((cpu_milestone.value != FASTISS_G4MH_1_0) && (cpu_milestone.value != FASTISS_G4MH_1_1)){
	  cpu_state->write_IMSR(cpu_state->read_IMSR() & 0x00000010);
	  cpu_state->write_IMSR_EID(1);
	}
      }
    }
  }else{
    // mask
    cpu_state->write_hv_func_support(prev_hv_func_support);
  }
  // Clear halt flag
  halted = false;

  return process_further_signals;
}

bool rh850g4::core::bgfeint_operation(unsigned char is_receipt_cpu_mode_guest, unsigned int handler_addr)
{
  bool process_further_signals = false;
  bool mask_psw = false;
  unsigned char prev_hv_func_support = cpu_state->read_hv_func_support();

  if (is_receipt_cpu_mode_guest == 1){    
    cpu_state->write_hv_func_support(0);
    cpu_state->write_IMSR(cpu_state->read_IMSR() & 0x0000030f);
    if (cpu_state->read_PSW_ID() == 1){
      mask_psw = true;
      // not accept (PSW.ID is 1)
      if((cpu_state->read_HVCFG_HVE() & 0x1) == 0x1){
	cpu_state->write_IMSR_HEID(1);
      }
    }else if (cpu_state->read_PSW_NP() == 1){
      mask_psw = true;
      // not accept (PSW.NP is 1)	
      if((cpu_state->read_HVCFG_HVE() & 0x1) == 0x1){
	cpu_state->write_IMSR_HENP(1);
      }
    }
    if (cpu_state->read_GMPSW_NP() == 1){
      mask_psw = true; 
      // not accept (GMPSW.NP is 1)
      if((cpu_state->read_HVCFG_HVE() & 0x1) == 0x1){
	cpu_state->write_IMSR_FNP(1);
      }
    }
    cpu_state->write_hv_func_support(prev_hv_func_support);

    if(!mask_psw){
      union FeintReq feintreq_val;
      feintreq_val.val = feintrq_info;
      // Clear pending signals
      interrupt_requested -= INT_BGFEINT;
      feintrq_info = 0;
      
      // change host mode
      cpu_state->write_PSWH_GM(0);

      // Update registers
      cpu_state->write_EIPC(cpu_state->read_PC());
      cpu_state->write_EIPSW(cpu_state->read_PSW());
      cpu_state->write_EIIC((feintreq_val.bit_val.gpid << 16) | 0xd800 | feintreq_val.bit_val.ch);
      cpu_state->write_EIPSWH_GM(is_receipt_cpu_mode_guest);
      cpu_state->write_EIPSWH_GPID(cpu_state->read_PSWH_GPID());
      cpu_state->write_PSW_UM(0);
      cpu_state->write_PSW_ID(1);
      cpu_state->write_PSW_EP(1);
      cpu_state->write_PC(handler_addr+0xd0);	
      cpu_state->write_IMSR_HENP(0);
      cpu_state->write_IMSR_HEID(0);
      cpu_state->write_IMSR_FNP(0);
      
      // Indicate that we processed a signal
      process_further_signals = true;
      
      // Clear the link(ll-bit) information
      clear_llbit();
    }
    // Clear halt flag
    halted = false;
  }else{
    // not accept (not guest mode)
  }
  
  return process_further_signals;
}

bool rh850g4::core::bgeiint_operation(unsigned char is_receipt_cpu_mode_guest, unsigned int handler_addr)
{
  bool process_further_signals = false;
  bool mask_psw = false;

  union EiintReq eireq_val;
  bool is_ei_pri_masked = false;
  unsigned int ei_pri_val = 0;
  unsigned char prev_hv_func_support = cpu_state->read_hv_func_support();
  
  eireq_val.val = eiintrq_info;
  ei_pri_val = eireq_val.bit_val.pri;
 
  // check mask setting(GMPSW.EIMASK)
  unsigned int eimask_val = (unsigned int)cpu_state->read_GMPSW_EIMASK();
  if(ei_pri_val >= eimask_val){
    is_ei_pri_masked = true;
    if(((cpu_state->read_HVCFG_HVE() & 0x1) == 0x1) && (is_receipt_cpu_mode_guest == 1)) {
      cpu_state->write_IMSR(cpu_state->read_IMSR() & 0x00001010);
      cpu_state->write_IMSR_EEIM(1);
    }
  }
  
  // check mask setting(GMPLMR)
  if(!is_ei_pri_masked){
    unsigned int plmr_plm_val = (unsigned int)cpu_state->read_GMPLMR_PLM();
    if(ei_pri_val >= plmr_plm_val){
      if(((cpu_state->read_HVCFG_HVE() & 0x1) == 0x1) && (is_receipt_cpu_mode_guest == 1)){
	cpu_state->write_IMSR(cpu_state->read_IMSR() & 0x00001010);
	cpu_state->write_IMSR_EPLM(1);
      }
      is_ei_pri_masked = true;
    }
  }

  if (is_receipt_cpu_mode_guest == 1){
    if (!is_ei_pri_masked) {
      cpu_state->write_hv_func_support(0);
      cpu_state->write_IMSR(cpu_state->read_IMSR() & 0x00001010);
      if (cpu_state->read_PSW_ID() == 1){
	mask_psw = true;
 	// not accept (PSW.ID is 1)
	if((cpu_state->read_HVCFG_HVE() & 0x1) == 0x1){
	  cpu_state->write_IMSR_HEID(1);
	}
      }else if (cpu_state->read_PSW_NP() == 1){
	mask_psw = true;
	// not accept (PSW.NP is 1)
	if((cpu_state->read_HVCFG_HVE() & 0x1) == 0x1){
	  cpu_state->write_IMSR_HENP(1);
	}
      }
      if (cpu_state->read_GMPSW_ID() == 1){
	mask_psw = true;
	// not accept (GMPSW.ID is 1)
	if((cpu_state->read_HVCFG_HVE() & 0x1) == 0x1){
	  cpu_state->write_IMSR_EID(1);
	}
      }else if (cpu_state->read_GMPSW_NP() == 0x1){
	mask_psw = true;
	// not accept (GMPSW.NP is 1)
	if((cpu_state->read_HVCFG_HVE() & 0x1) == 0x1){
	  cpu_state->write_IMSR_ENP(1);
	}
      }
      cpu_state->write_hv_func_support(prev_hv_func_support);
    
      if(!mask_psw) {
	// Clear pending signals
	interrupt_requested -= INT_BGEIINT;
	eiintrq_info = 0;
	      
	// change host mode
	cpu_state->write_PSWH_GM(0);

	// Update registers
	cpu_state->write_EIPC(cpu_state->read_PC());
	cpu_state->write_EIPSW(cpu_state->read_PSW());
	cpu_state->write_EIIC((eireq_val.bit_val.gpid << 16) | 0xd000 | eireq_val.bit_val.ch);
	cpu_state->write_EIPSWH_GM(is_receipt_cpu_mode_guest);
	cpu_state->write_EIPSWH_GPID(cpu_state->read_PSWH_GPID());
	cpu_state->write_PSW_UM(0);
	cpu_state->write_PSW_ID(1);
	cpu_state->write_PSW_EP(1);
	cpu_state->write_PC(handler_addr+0xd0);
	cpu_state->write_IMSR_HENP(0);
	cpu_state->write_IMSR_HEID(0);
	cpu_state->write_IMSR_ENP(0);
	cpu_state->write_IMSR_EID(0);
	cpu_state->write_IMSR_EPLM(0);
	cpu_state->write_IMSR_EEIM(0);
	
	// Indicate that we processed a signal
	process_further_signals = true;
	
	// Clear the link(ll-bit) information
	clear_llbit();
      }
    }else{
      // not accept (masked by GMPLMR or GMPSW.EIMASK)
    }
    // Clear halt flag
    halted = false;
  }else{
    // not accept (not guest mode)
  }
  
  return process_further_signals;
}

bool rh850g4::core::gmfeint_operation(unsigned char is_receipt_cpu_mode_guest)
{
  bool process_further_signals = false;

  if (is_receipt_cpu_mode_guest == 1){
    if (cpu_state->read_GMPSW_NP() == 0){
      cpu_state->write_PSWH_GM(1);

      union FeintReq feintreq_val;
      feintreq_val.val = feintrq_info;
      // Clear pending signals
      interrupt_requested -= INT_GMFEINT;
      feintrq_info = 0;
      
      // Update registers
      cpu_state->write_GMFEPC(cpu_state->read_PC());
      cpu_state->write_GMFEPSW(cpu_state->read_GMPSW());
      cpu_state->write_GMFEIC(0xf0+feintreq_val.bit_val.ch);
      cpu_state->write_GMPSW_UM(0);
      cpu_state->write_GMPSW_NP(1);
      cpu_state->write_GMPSW_ID(1);
      cpu_state->write_GMPSW_EP(0);
      cpu_state->write_PC((cpu_state->read_GMEBASE() & 0xfffffe00) + 0xf0);
      cpu_state->write_IMSR_FNP(0);
      
      // Acknowledge interrupt
      union FeintAck feintack_val;
      feintack_val.bit_val.ack  = true;
      feintack_val.bit_val.ch   = feintreq_val.bit_val.ch;
      intack_feint_requested = feintack_val.val;
      feintrq_ack_event.notify();
      
      // Indicate that we processed a signal
      process_further_signals = true;
      
      // Clear the link(ll-bit) information
      clear_llbit();
    }else{
      // not accept (GMPSW_NP = 1)
      if(((cpu_state->read_HVCFG_HVE() & 0x1) == 0x1) && (is_receipt_cpu_mode_guest == 0x1)){
	cpu_state->write_IMSR(cpu_state->read_IMSR() & 0x0000030f);
	cpu_state->write_IMSR_FNP(1);
      }
    }
    // Clear halt flag
    halted = false;
  }else{
    // not accept (not guest mode)
    cpu_state->write_IMSR(0);
  }
  
  return process_further_signals;
}

bool rh850g4::core::gmeiint_operation(unsigned char is_receipt_cpu_mode_guest)
{
  bool process_further_signals = false;
  bool handler_rint = false;
  bool handler_dv = false;
  
  unsigned char gmpsw_ebv = cpu_state->read_GMPSW_EBV();
  
  unsigned int handler_addr = cpu_state->read_GMEBASE();
  if((handler_addr & 0x1) == 0x1) handler_rint = true; // GMEBASE.RINT
  if((handler_addr & 0x2) == 0x2) handler_dv = true;   // GMEBASE.DV
  handler_addr &= 0xfffffe00;
  
  union EiintReq eireq_val;
  bool is_ei_pri_masked = false;
  unsigned int ei_pri_val = 0;
  
  eireq_val.val = eiintrq_info;
  ei_pri_val = eireq_val.bit_val.pri;  
  
  // check mask setting(GMPSW.EIMASK)
  unsigned int eimask_val = (unsigned int)cpu_state->read_GMPSW_EIMASK();
  if(ei_pri_val >= eimask_val){
    is_ei_pri_masked = true;
    if(((cpu_state->read_HVCFG_HVE() & 0x1) == 0x1) && (is_receipt_cpu_mode_guest == 0x1)){
      cpu_state->write_IMSR(cpu_state->read_IMSR() & 0x00001010);
      cpu_state->write_IMSR_EEIM(1);
    }
  }
  
  // check mask setting(GMPLMR)
  if(!is_ei_pri_masked){
    unsigned int plmr_plm_val = (unsigned int)cpu_state->read_GMPLMR_PLM();
    if(ei_pri_val >= plmr_plm_val){
      if(((cpu_state->read_HVCFG_HVE() & 0x1) == 0x1) && (is_receipt_cpu_mode_guest == 0x1)){
	cpu_state->write_IMSR(cpu_state->read_IMSR() & 0x00001010);
	cpu_state->write_IMSR_EPLM(1);
      }
      is_ei_pri_masked = true;
    }
  }
  
  if (is_receipt_cpu_mode_guest == 1){
    if (!is_ei_pri_masked){
      if (cpu_state->read_GMPSW_ID() == 0){
	if (cpu_state->read_GMPSW_NP() == 0){
	  cpu_state->write_PSWH_GM(1);
	  bool syserr_flag = false;
	  bool nc0_flag = false;
	  
	  // get current PC
	  unsigned int ret_pc = cpu_state->read_PC();
	  
	  if((eireq_val.bit_val.tb) && (!handler_dv)){
	    // determin the vector address from interrupt table
	    unsigned int pri_bit;
	    if(ei_pri_val < 16){
	      pri_bit = 0x1 << ei_pri_val;
	    }else{
	      pri_bit = 0x1 << 15;
	    }
	    
	    unsigned int be_val = cpu_state->read_RBCR0_BE();
	    unsigned int nc_val = cpu_state->read_RBCR1_NC();
	    unsigned int bn_val = cpu_state->read_RBNR() & 0x3f;
	    if((be_val & pri_bit) == pri_bit) {
	      // saving context
	      if((bn_val >= 63) || (bn_val > cpu_state->read_INTCFG_ULNR())){
		// SYSERR
		syserr_flag = true;
	      }else{
		if((nc_val & pri_bit) == 0){
		  nc0_flag = true;
		}
		cpu_state->write_mem_acc_type(6);
		unsigned int addr = cpu_state->read_RBIP();
		unsigned int md_val = (cpu_state->read_RBCR0() >> 16) & 0x1;
		if(md_val == 0){
		  addr = addr - (bn_val * 0x60);
		}else{
		  addr = addr - (bn_val * 0x90);
		}
		addr = addr - 4;
		fastiss_memory->i32_store(addr, cpu_state->read_PC());
		addr = addr - 4;
		fastiss_memory->i32_store(addr, cpu_state->read_PSW());
		addr = addr - 4;
		fastiss_memory->i32_store(addr, cpu_state->read_EIIC());
		addr = addr - 4;
		fastiss_memory->i32_store(addr, cpu_state->read_FPSR());
		addr = addr - 4;
		for(int i=1; i<32; i++){
		  if((md_val == 0) &&  (i > 19)){
		    fastiss_memory->i32_store(addr, cpu_state->read_GR(GR_index_t(30)));
		    addr = addr - 4;
		    break;
		  }
		  fastiss_memory->i32_store(addr, cpu_state->read_GR(GR_index_t(i)));
		  addr = addr - 4;
		}
		cpu_state->write_RBNR((bn_val + 1) & 0x3f);
	      }
	    }else{
	      // nothing
	    }
	    // update PC
	    if(!syserr_flag){
	      cpu_state->write_mem_acc_type(2);
	      handler_addr = fastiss_memory->i32_load(cpu_state->read_GMINTBP()+(eireq_val.bit_val.ch*4));
	      cpu_state->write_PC(handler_addr);
	    }
	  }else{
	    // determin the vector address from vector directly
	    if(handler_rint){
	      cpu_state->write_PC(handler_addr+0x100);
	    }else{
	      if(eireq_val.bit_val.pri < 16){
		cpu_state->write_PC(handler_addr+0x100+(eireq_val.bit_val.pri*0x10));
	      }else{
		cpu_state->write_PC(handler_addr+0x100+(15*0x10));
	      }
	    }
	  }
	  
	  if(syserr_flag){
	    // Update registers
	    if(is_receipt_cpu_mode_guest){
	      // Syserr causes in guest mode. Check the GMCFG.GSYSE (0:Guest, 1:Host)
	      if(((cpu_state->read_HVCFG_HVE() && 0x1) == 0x1) && ((cpu_state->read_GMCFG_GSYSE() && 0x1) == 0x0)){
		cpu_state->write_FEPSWH_GM(is_receipt_cpu_mode_guest);
		cpu_state->write_FEPSWH_GPID(cpu_state->read_PSWH_GPID());
		cpu_state->write_GMFEPC(ret_pc);
		cpu_state->write_GMFEPSW(cpu_state->read_GMPSW());
		cpu_state->write_GMFEIC(0x1C);
		cpu_state->write_GMPSW_UM(0);
		cpu_state->write_GMPSW_ID(1);
		cpu_state->write_GMPSW_NP(1);
		cpu_state->write_GMPSW_EP(1);
		cpu_state->write_PC(handler_addr+0x10);
	      }else{
		cpu_state->write_FEPSWH_GM(is_receipt_cpu_mode_guest);
		cpu_state->write_FEPSWH_GPID(cpu_state->read_PSWH_GPID());
		cpu_state->write_PSWH_GM(0);
		cpu_state->write_FEPC(ret_pc);
		cpu_state->write_FEPSW(cpu_state->read_PSW());
		cpu_state->write_FEIC(0x1C);
		cpu_state->write_PSW_UM(0);
		cpu_state->write_PSW_ID(1);
		cpu_state->write_PSW_NP(1);
		cpu_state->write_PSW_EP(1);
		if(cpu_state->read_PSW_EBV() == 0){
		  cpu_state->write_PC((cpu_state->read_RBASE() & 0xfffffe00) + 0x10);
		}else{
		  cpu_state->write_PC((cpu_state->read_EBASE() & 0xfffffe00) + 0x10);
		}
	      }
	    }else{
	      // Syserr causes in host mode
	      cpu_state->write_FEPSWH_GM(is_receipt_cpu_mode_guest);
	      cpu_state->write_FEPSWH_GPID(cpu_state->read_PSWH_GPID());
	      cpu_state->write_PSWH_GM(0);
	      cpu_state->write_FEPC(ret_pc);
	      cpu_state->write_FEPSW(cpu_state->read_PSW());
	      cpu_state->write_FEIC(0x1C);
	      cpu_state->write_PSW_UM(0);
	      cpu_state->write_PSW_ID(1);
	      cpu_state->write_PSW_NP(1);
	      cpu_state->write_PSW_EP(1);
	      if(cpu_state->read_PSW_EBV() == 0){
		cpu_state->write_PC((cpu_state->read_RBASE() & 0xfffffe00) + 0x10);
	      }else{
		cpu_state->write_PC((cpu_state->read_EBASE() & 0xfffffe00) + 0x10);
	      }
	    }
	  }else{
	    // Update registers
	    cpu_state->write_GMEIPC(ret_pc);
	    cpu_state->write_GMEIPSW(cpu_state->read_GMPSW());
	    cpu_state->write_GMEIIC(0x1000+eireq_val.bit_val.ch);
	    cpu_state->write_GMPSW_UM(0);
	    if(nc0_flag) cpu_state->write_GMPSW_ID(0);
	    else         cpu_state->write_GMPSW_ID(1);
	    cpu_state->write_GMPSW_EP(0);
	    cpu_state->write_ICSR_PMEI(0);
	    cpu_state->write_IMSR_ENP(0);
	    cpu_state->write_IMSR_EID(0);
	    cpu_state->write_IMSR_EPLM(0);
	    cpu_state->write_IMSR_EEIM(0);
	    
	    // Clear pending signals
	    interrupt_requested -= INT_GMEIINT;
	    eiintrq_info = 0;
	    
	    cpu_state->write_GMPSW_EIMASK(ei_pri_val);
	    
	    // Acknowledge interrupt
	    union EiintAck eiack_val;
	    eiack_val.bit_val.ack  = true;
	    eiack_val.bit_val.ch   = eireq_val.bit_val.ch;
	    intack_eiint_requested = eiack_val.val;
	    eiintrq_ack_event.notify();
	  }
	  
	  // Indicate that we processed a signal
	  process_further_signals = true;
	  
	  // Clear the link(ll-bit) information
	  clear_llbit();
	}else{
	  // not accept (GMPSW.NP=1)
	  if(((cpu_state->read_HVCFG_HVE() & 0x1) == 0x1) && (is_receipt_cpu_mode_guest == 0x1)){
	    cpu_state->write_IMSR(cpu_state->read_IMSR() & 0x00001010);
	    cpu_state->write_IMSR_ENP(1);
	  }
	}
      }else{
	// not accept (GMPSW.ID=1)
	if(((cpu_state->read_HVCFG_HVE() & 0x1) == 0x1) && (is_receipt_cpu_mode_guest == 0x1)){
	  cpu_state->write_IMSR(cpu_state->read_IMSR() & 0x00001010);
	  cpu_state->write_IMSR_EID(1);
	}
      }
    }else{
      // not accept (masked by GMPLMR or GMPSW.EIMASK)
    }
    // Clear halt flag
    halted = false;
  }else{
    // not accept (not guest mode)
    cpu_state->write_IMSR(0);
  }

  return process_further_signals;
}


void rh850g4::core::fetch_exception(void)
{
  sc_core::sc_report_handler::report(sc_core::SC_ERROR, "rh850g4", "Instruction fetch failed", "<UNKNOWN>", 0);    
}

void rh850g4::core::terminate_quantum(void)
{
  local_time_limit = 0;
}

void rh850g4::core::mmu_configuration_changed(void)
{
  // Flush the Data caches and DMI handles
  fastiss_bus_interface->invalidate_all_direct_mem_ptr();

  // Flush the L1 Block function cache
  flush_l1_cache();
}

void rh850g4::core::effective_address_configuration_changed(void)
{
  // Flush the L1 Block function cache
  flush_l1_cache();
}

void rh850g4::core::activate_interrupt_accepted_analysis_point(void)
{
  m_interrupt_accepted_analysis_point->activate();
}

unsigned long long rh850g4::core::get_total_inst_num()
{
  return(get_instruction_count());
}

double rh850g4::core::get_total_time_sec()
{
  return((sc_time_stamp() + get_local_time()).to_seconds());

}

unsigned long long rh850g4::core::get_total_cycle()
{
  return((unsigned long long)(sc_time_stamp() + get_local_time()).to_double()/cpu_state->read_cycle_period());
}

void rh850g4::core::set_peid(unsigned int peid)
{
  pe_id.value = peid;
}

// Update SWD0PCAD value (for SWDT)
void rh850g4::core::set_swd0pcad_val(unsigned int val)
{
  swd0pcad_val.value = val;
  cpu_state->write_swd0pcad_val(swd0pcad_val.value);
}

// get executed time of SWD0PCAD address  (for SWDT)
unsigned int rh850g4::core::get_swd0pcad_exec_count_val(void)
{
  return(cpu_state->read_swd0pcad_exec_count());
}

// Set SWDT detection mode (for SWDT)
void rh850g4::core::set_swdt_enable(bool mode)
{
  swdt_enable.value = mode;
}

// get memory access type  (for Guard function)
unsigned int rh850g4::core::get_mem_acc_type(void)
{
  return(cpu_state->read_mem_acc_type());
}

// set G4_LinkManager module's pointer
void rh850g4::core::set_link_manager(G4_LinkManager* p_link_manager)
{
  link_manager = p_link_manager;
}

// get G3M payload information from each system register
void rh850g4::core::get_g3m_params(unsigned int& vcid, unsigned int& tcid, unsigned int& spid, bool& userMode, bool& virtualMode)
{
  if ((cpu_milestone.value==FASTISS_G4MH_1_0) || (cpu_milestone.value==FASTISS_G4MH_1_1)){
    vcid = 0;
    tcid = 0;
    spid = cpu_state->read_SPID();
    userMode    = cpu_state->read_PSW_UM() != 0;
    virtualMode = 0;
  }else{ // G4MH 2.0
    vcid = cpu_state->read_PSWH_GPID();
    tcid = 0;
    spid = cpu_state->read_SPID();
    userMode    = cpu_state->read_PSW_UM() != 0;
    virtualMode = cpu_state->read_PSWH_GM() != 0;
  }
}

void rh850g4::core::force_wait(void)
{
  // Advance GT by any non-accured local time
  sc_core::wait(get_local_time());
  cpu_state->write_accrued_time((unsigned int)get_elapsed_time().value());
}

void rh850g4::core::force_wait_snooze(void)
{
  // Advance GT by any non-accured local time
  snz_time.value = cpu_state->read_SNZCFG();
  cpu_state->increment_time(cpu_state->read_cycle_period()*snz_time.value);
  sc_core::wait(get_local_time());
  cpu_state->write_accrued_time((unsigned int)get_elapsed_time().value());
}

void rh850g4::core::clear_llbit(void)
{
  if(link_func_enable.value){
    link_manager->ClearLlbit(pe_id.value);
  }
}

unsigned int rh850g4::core::get_cpu_milestone(void)
{
  return(cpu_milestone.value);
}


//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
