/*************************************************************************
 *
 * Copyright(c) 2019 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2019.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850/core.h"
#include "rh850/custom_cpu_state_impl.h"
#include "rh850/custom_impl.h"
#include "rh850/custom_memory_impl.h"
#include "rh850/step_translator.h"
#include "rh850/continue_translator.h"
#include "rh850/custom_bus_interface.h"
#include "rh850/analysis_point_if.h"
#include "rh850/custom_if.h"
#include "icum_intc_type.h"
#include "LinkManager.h"

#include <iostream>
#include <sstream>

using namespace ICUM_INTCType;

//------------------------------------------------------------------------------
// Helper functions
//------------------------------------------------------------------------------

namespace rh850
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
  
  void set_link_helper(LinkManager *link_man_instance, unsigned int peid, unsigned int addr)
  {
    link_man_instance->SetLink(peid, addr);
  }


  unsigned int is_link_valid_helper(LinkManager *link_man_instance, unsigned int peid, unsigned int addr)
  {
    bool status = link_man_instance->IsLinkValid(peid, addr);
    if(status) return(1);
    else       return(0);
  }

  void clear_link_helper(LinkManager *link_man_instance, unsigned int peid, unsigned int addr)
  {
    link_man_instance->ClearLink(peid, addr);
  }

  void clear_llbit_helper(LinkManager *link_man_instance, unsigned int peid)
  {
    link_man_instance->ClearLlbit(peid);
  }

  void clear_monitor_helper(LinkManager *link_man_instance, unsigned int peid, unsigned int addr, unsigned int size)
  {
    link_man_instance->ClearMonitor(peid, addr, size);
  }

  unsigned int get_cpu_milestone_helper(core *core_instance)
  {
    return(core_instance->get_cpu_milestone());
  }

  void activate_interrupt_accepted_analysis_point_helper(core *core_instance)
  {
    core_instance->activate_interrupt_accepted_analysis_point();
  }

  unsigned int mpu_func_helper(core *core_instance, unsigned int mpu_kind, unsigned int size, unsigned int address, unsigned int mei_val)
  {
    return core_instance->mpu_func(mpu_kind, size, address, mei_val);
  }
}

//------------------------------------------------------------------------------
// rh850 core definition
//------------------------------------------------------------------------------

rh850::core::core(sc_core::sc_module_name name, bool count_instructions) :
  // Base class
  rh850::core_base_t(name),
  
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
  //cpu_milestone("cpu_milestone", FASTISS_G3M_V30),
  cpu_milestone("cpu_milestone", FASTISS_G3K_PCU),

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
  syserr_ack("syserr_ack"),
  syserr_req("syserr_req"),
  syserr_code("syserr_code"),
  syserr_ebv("syserr_ebv"),
  
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
  
  // Generated components
  custom(NULL),
  
  // Interrupt request flag
  interrupt_requested(0),

  // Interrupt acknowledge flag
  intack_fenmi_requested(false),
  intack_feint_requested(false),
  intack_eiint_requested(0),
  intack_syserr_requested(false),
  eiintrq_info(0),
  syserr_code_val(0),
  
  // Pending floating point imprecise exception flag
  pending_fpu_imprecise_exception(0), // false

  // Halt state
  halted(false),
  
  // maybe not use it
  in_reset(false) ,

  // Link management
  link_manager(0),

  // Time management
  frequency(0),

  // Instruction counting
  count_instructions(count_instructions),

  // mip enable
  m_mip_enable(false),

  // mdp enable
  m_mdp_enable(false),

  // mpu entry
  m_mpu_entry(ICUM_MPU_INIT_NUM)
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

  // Initialise EIINT req hanvoidr
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
  fastiss_memory = new rh850::custom_memory_impl_t();
  
  // Create and bind bus interfaces
  fastiss_bus_interface = new rh850::custom_bus_interface_t("fastiss_bus_interface");
  fastiss_bus_interface->bind(bus_initiator);

  // Create sub-components
  cpu_state = new rh850::custom_cpu_state_impl_t();
}

rh850::core::~core()
{
  delete reinterpret_cast<rh850::custom_memory_impl_t*>(fastiss_memory);
  delete step_translator;
  delete continue_translator;
  delete custom;
  delete cpu_state;
}

void rh850::core::before_end_of_elaboration(void)
{
  // Check translation partition size
  unsigned int trans_part_sz = translation_partition_size.value;
  if ((trans_part_sz <= 128) || (trans_part_sz & (trans_part_sz - 1)))
  {
    std::ostringstream os;
    os << "Invalid translation partition size: " << trans_part_sz << " bytes" << std::endl;
    os << "Partition size must be a power of two and greater than 128." << std::endl;
    sc_core::sc_report_handler::report(sc_core::SC_ERROR, "rh850", os.str().c_str(), "<UNKNOWN>", 0);
  }
  
  // Check translation cache max number of entries
  if (translation_cache_max_list_entries.value <= 1)
  {
    std::ostringstream os;
    os << "Invalid translation cache maximum number of list entries, value must be greater than one" << std::endl;
    sc_core::sc_report_handler::report(sc_core::SC_ERROR, "rh850", os.str().c_str(), "<UNKNOWN>", 0);
  }
  
  // Initialise components
  rh850::core_base_t::initialise
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
  custom = new rh850::custom_impl_t(fastiss_code_gen, fastiss_memory, cpu_state, m_interrupt_accepted_analysis_point, m_interrupt_completed_analysis_point, &pending_fpu_imprecise_exception, link_manager, link_func_enable.value, cpu_milestone.value);

  if (block_size.value > 100)
  {
    block_size.value = 100;
  }
  step_translator = new rh850::step_translator_t
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
    ~(trans_part_sz - 1),
    &m_mip_enable
  );
  continue_translator = new rh850::continue_translator_t
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
    ~(trans_part_sz - 1),
    &m_mip_enable
  );

  // Initialize cpu_state::swd0pcad_val (for SWDT)
  cpu_state->write_swd0pcad_val(swd0pcad_val.value);
  
  // Initialise the block function cache
  initialise_cache();
  
  // Add core helper functions
  fastiss::func_args_type_t* core_helper_args = fastiss_code_gen->create_args_type();
  fastiss_code_gen->append_ptr(core_helper_args);
  fastiss_code_gen->add_helper("terminate_quantum_helper", (void*)&rh850::terminate_quantum_helper, fastiss_code_gen->create_void_func(core_helper_args));
  fastiss_code_gen->add_helper("mmu_configuration_changed_helper", (void*)&rh850::mmu_configuration_changed_helper, fastiss_code_gen->create_void_func(core_helper_args));
  fastiss_code_gen->add_helper("effective_address_configuration_changed_helper", (void*)&rh850::effective_address_configuration_changed_helper, fastiss_code_gen->create_void_func(core_helper_args));
  fastiss_code_gen->add_helper("enter_halt_helper", (void*)&rh850::enter_halt_helper, fastiss_code_gen->create_void_func(core_helper_args));
  fastiss_code_gen->add_helper("force_wait_helper", (void*)&rh850::force_wait_helper, fastiss_code_gen->create_void_func(core_helper_args));
  fastiss_code_gen->add_helper("force_wait_snooze_helper", (void*)&rh850::force_wait_snooze_helper, fastiss_code_gen->create_void_func(core_helper_args));
  fastiss_code_gen->add_helper("get_cpu_milestone_helper", (void*)&rh850::get_cpu_milestone_helper, fastiss_code_gen->create_void_func(core_helper_args));
  fastiss_code_gen->add_helper("activate_interrupt_accepted_analysis_point_helper", (void*)&rh850::activate_interrupt_accepted_analysis_point_helper, fastiss_code_gen->create_void_func(core_helper_args));
  fastiss_code_gen->delete_args_type(core_helper_args);

  fastiss::func_args_type_t* core_helper_args2 = fastiss_code_gen->create_args_type();
  fastiss_code_gen->append_i32(core_helper_args2);
  fastiss_code_gen->append_i32(core_helper_args2);
  fastiss_code_gen->add_helper("clear_llbit_helper", (void*)&rh850::clear_llbit_helper, fastiss_code_gen->create_void_func(core_helper_args2));
  fastiss_code_gen->delete_args_type(core_helper_args2);  

  fastiss::func_args_type_t* core_helper_args3 = fastiss_code_gen->create_args_type();
  fastiss_code_gen->append_i32(core_helper_args3);
  fastiss_code_gen->append_i32(core_helper_args3);
  fastiss_code_gen->append_i32(core_helper_args3);
  fastiss_code_gen->add_helper("set_link_helper", (void*)&rh850::set_link_helper, fastiss_code_gen->create_void_func(core_helper_args3));
  fastiss_code_gen->add_helper("is_link_valid_helper", (void*)&rh850::is_link_valid_helper, fastiss_code_gen->create_i32_func(core_helper_args3));
  fastiss_code_gen->add_helper("clear_link_helper", (void*)&rh850::clear_link_helper, fastiss_code_gen->create_void_func(core_helper_args3));
  fastiss_code_gen->delete_args_type(core_helper_args3);  

  fastiss::func_args_type_t* core_helper_args4 = fastiss_code_gen->create_args_type();
  fastiss_code_gen->append_i32(core_helper_args4);
  fastiss_code_gen->append_i32(core_helper_args4);
  fastiss_code_gen->append_i32(core_helper_args4);
  fastiss_code_gen->append_i32(core_helper_args4);
  fastiss_code_gen->add_helper("clear_monitor_helper", (void*)&rh850::clear_monitor_helper, fastiss_code_gen->create_void_func(core_helper_args4));
  fastiss_code_gen->delete_args_type(core_helper_args4);  


  fastiss::func_args_type_t* core_helper_args5 = fastiss_code_gen->create_args_type();
  fastiss_code_gen->append_i32(core_helper_args5);
  fastiss_code_gen->append_i32(core_helper_args5);
  fastiss_code_gen->append_i32(core_helper_args5);
  fastiss_code_gen->append_i32(core_helper_args5);
  fastiss_code_gen->append_i32(core_helper_args5);
  fastiss_code_gen->add_helper("mpu_func_helper", (void*)&rh850::mpu_func_helper, fastiss_code_gen->create_i32_func(core_helper_args5));
  fastiss_code_gen->delete_args_type(core_helper_args5);  


  // Spawn step and continue thread processes (method processes are not supported)
  sc_core::sc_spawn
  (
    sc_bind(&rh850::core::continue_thread, this),
    sc_core::sc_gen_unique_name("continue_thread")
  );
  sc_core::sc_spawn
  (
    sc_bind(&rh850::core::step_thread, this),
    sc_core::sc_gen_unique_name("step_thread")
  );
  
  initialise_parallel_translator();

  // Schedule initial advance event
  initial_event.notify(SC_ZERO_TIME); 
}

void rh850::core::initial_event_handler(void)
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

    // Reset core
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

void rh850::core::frequency_update(void)
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

void rh850::core::continue_handler(void)
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
  
  // Skip this quanta if the clock is turned off or CPU halted?
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
    sc_core::sc_report_handler::report(sc_core::SC_ERROR, "rh850", e.what(), "<UNKNOWN>", 0);
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

void rh850::core::step_handler(void)
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
  
  // Skip this quanta if the clock is turned off or CPU halted?
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
    sc_core::sc_report_handler::report(sc_core::SC_ERROR, "rh850", e.what(), "<UNKNOWN>", 0);
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

void rh850::core::continue_thread(void)
{
  while (1)
  {
    // Wait for quantum period to start
    sc_core::wait(continue_event);

    // Call the underlying advance method
    rh850::core::base_continue_handler();
  }
}

void rh850::core::step_thread(void)
{
  while (1)
  {
    // Wait for quantum period to start
    sc_core::wait(step_event);

    // Call the underlying advance method
    rh850::core::base_step_handler();
  }
}

void rh850::core::continue_completed_handler(void)
{
  operation_complete_event.notify();
  continue_event.notify(SC_ZERO_TIME);
}

void rh850::core::step_completed_handler(void)
{
  operation_complete_event.notify();
  step_event.notify(SC_ZERO_TIME);
}

void rh850::core::reset_core(void)
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
  //cpu_state->write_FPSR(0x00020000); set by FastIssIF
  cpu_state->write_FPCFG(0x00000000);  
  cpu_state->write_FPEC(0x00000000);  
  cpu_state->write_EIIC(0x00000000);
  cpu_state->write_FEIC(0x00000000);
  cpu_state->write_CTPSW(0x00000000);

  // selID=1
  //cpu_state->write_MCFG0(0x00020004); // set by FastIssIF
  //cpu_state->write_MCTL(0x80000000); // set by FastIssIF
  //cpu_state->write_PID(0x05800701); // set by FastIssIF
  cpu_state->write_FPIPR(0x00000000);

  // selID=2
  //cpu_state->write_HTCFG0(0xxxxxxx); // set by FastIssIF
  cpu_state->write_ISPR(0x00000000);
  cpu_state->write_PMR(0x00000000);
  cpu_state->write_ICSR(0x00000000);
  cpu_state->write_INTCFG(0x00000000);

  // selID=3 (Debug)
  cpu_state->write_DBIC(0x00000000);
  cpu_state->write_DBCMC(0x00000000);
  cpu_state->write_DBPSW(0x00000020);
  cpu_state->write_DIR0(0x00000000);
  cpu_state->write_DIR1(0x00000000);

  // selID=5 (MPU)
  cpu_state->write_MPM(0x00000000);
  cpu_state->write_MPRC(0x00000000);
  cpu_state->write_MPBRGN(0x00000000);
  cpu_state->write_MPTRGN(m_mpu_entry - 1);
  cpu_state->write_MCR(0x0000003f);

  unsigned int mpat_val[16];
  for (unsigned int i = 0; i < 16; i++)
  {
    if(i < m_mpu_entry)
    {
      if(cpu_state->read_mpat_g_support())
      {
        mpat_val[i] = 0x40;
      }
      else
      {
        mpat_val[i] = 0x0;
      }
    }
    else
    {
      mpat_val[i] = 0x0;
    }
  }

  // selID=6 (MPU)
  cpu_state->write_MPLA0(0);
  cpu_state->write_MPUA0(0);
  cpu_state->write_MPAT0(mpat_val[0]);
  cpu_state->write_MPLA1(0);
  cpu_state->write_MPUA1(0);
  cpu_state->write_MPAT1(mpat_val[1]);
  cpu_state->write_MPLA2(0);
  cpu_state->write_MPUA2(0);
  cpu_state->write_MPAT2(mpat_val[2]);
  cpu_state->write_MPLA3(0);
  cpu_state->write_MPUA3(0);
  cpu_state->write_MPAT3(mpat_val[3]);
  cpu_state->write_MPLA4(0);
  cpu_state->write_MPUA4(0);
  cpu_state->write_MPAT4(mpat_val[4]);
  cpu_state->write_MPLA5(0);
  cpu_state->write_MPUA5(0);
  cpu_state->write_MPAT5(mpat_val[5]);
  cpu_state->write_MPLA6(0);
  cpu_state->write_MPUA6(0);
  cpu_state->write_MPAT6(mpat_val[6]);
  cpu_state->write_MPLA7(0);
  cpu_state->write_MPUA7(0);
  cpu_state->write_MPAT7(mpat_val[7]);

  // selID=7 (MPU)
  cpu_state->write_MPLA8(0);
  cpu_state->write_MPUA8(0);
  cpu_state->write_MPAT8(mpat_val[8]);
  cpu_state->write_MPLA9(0);
  cpu_state->write_MPUA9(0);
  cpu_state->write_MPAT9(mpat_val[9]);
  cpu_state->write_MPLA10(0);
  cpu_state->write_MPUA10(0);
  cpu_state->write_MPAT10(mpat_val[10]);
  cpu_state->write_MPLA11(0);
  cpu_state->write_MPUA11(0);
  cpu_state->write_MPAT11(mpat_val[11]);
  cpu_state->write_MPLA12(0);
  cpu_state->write_MPUA12(0);
  cpu_state->write_MPAT12(mpat_val[12]);
  cpu_state->write_MPLA13(0);
  cpu_state->write_MPUA13(0);
  cpu_state->write_MPAT13(mpat_val[13]);
  cpu_state->write_MPLA14(0);
  cpu_state->write_MPUA14(0);
  cpu_state->write_MPAT14(mpat_val[14]);
  cpu_state->write_MPLA15(0);
  cpu_state->write_MPUA15(0);
  cpu_state->write_MPAT15(mpat_val[15]);

  // swd0pcad_exec_count
  cpu_state->write_swd0pcad_exec_count(0);

  // Activate reset completed analysis point
  if (m_reset_completed_analysis_point->has_attachments())
  {
    m_reset_completed_analysis_point->activate();
  }    
    
  reset_done_event.notify();
}

// Signal processing
bool rh850::core::process_signals(void)
{
  bool process_further_signals = false;

  // Did we hit an interrupt or reset?
  if ((interrupt_requested & INT_REQ) || pending_fpu_imprecise_exception)
  {
    unsigned char psw_ebv = cpu_state->read_PSW_EBV();
    unsigned int  handler_addr;
    bool handler_rint = false;

    if(!psw_ebv){
      handler_addr = cpu_state->read_RBASE();
    }else{
      handler_addr = cpu_state->read_EBASE();
    }
    if((handler_addr & 0x1) == 0x1) handler_rint = true;
    handler_addr &= 0xfffffe00;
    
    if(interrupt_requested & INT_RESET){ // Handle reset

      // Clear pending signals
      interrupt_requested = 0;

      // Reset the core
      reset_core();      

      // Acknowledge interrupt clear
      intack_fenmi_requested = false;
      fenmirq_ack_event.notify();
      intack_feint_requested = false;
      feintrq_ack_event.notify();
      intack_eiint_requested = 0;
      eiintrq_ack_event.notify();
      intack_syserr_requested = false;
      syserr_ack_event.notify();
      
      // Indicate that we processed a signal
      process_further_signals = true;

    }else if(interrupt_requested & INT_FENMI){ // Handle FENMI

      // Clear pending signals
      interrupt_requested -= INT_FENMI;
      
      // Update registers
      cpu_state->write_FEPC(cpu_state->read_PC());
      cpu_state->write_FEPSW(cpu_state->read_PSW());
      cpu_state->write_FEIC(0xe0);
      //cpu_state->write_MCTL_MT(0); G3K doesn't supprt this bit
      //cpu_state->write_PSW_VM(0); G3K doesn't supprt this bit
      cpu_state->write_PSW_UM(0);
      //cpu_state->write_PSW_HVC(0); G3K doesn't supprt this bit
      //cpu_state->write_PSW_EBV(?);
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

    }else if(interrupt_requested & INT_SYSERR){ // Handle SYSERR

      // Clear pending signals
      interrupt_requested -= INT_SYSERR;
      
      // Update registers
      cpu_state->write_FEPC(cpu_state->read_PC());
      cpu_state->write_FEPSW(cpu_state->read_PSW());
      cpu_state->write_FEIC(0x10 | (syserr_code_val & 0xf));
      //cpu_state->write_MCTL_MT(0); G3K doesn't supprt this bit
      //cpu_state->write_PSW_VM(0); G3K doesn't supprt this bit
      cpu_state->write_PSW_UM(0);
      //cpu_state->write_PSW_HVC(0); G3K doesn't supprt this bit
      //cpu_state->write_PSW_EBV(?);
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

      // Clear halt flag
      halted = false;

    }else if(interrupt_requested & INT_FEINT){ // Handle FEINT

      if (cpu_state->read_PSW_NP() == 0){
	// Clear pending signals
	interrupt_requested -= INT_FEINT;
	
	// Update registers
	cpu_state->write_FEPC(cpu_state->read_PC());
	cpu_state->write_FEPSW(cpu_state->read_PSW());
	cpu_state->write_FEIC(0xf0);
	//cpu_state->write_PSW_VM(0); G3K doesn't supprt this bit
	cpu_state->write_PSW_UM(0);
	//cpu_state->write_PSW_HVC(0); G3K doesn't supprt this bit
	//cpu_state->write_PSW_EBV(?);
	cpu_state->write_PSW_NP(1);
	cpu_state->write_PSW_ID(1);
	cpu_state->write_PSW_EP(0);
	cpu_state->write_PC(handler_addr+0xf0);
	
	// Acknowledge interrupt
	intack_feint_requested = true;
	feintrq_ack_event.notify();
      
	// Indicate that we processed a signal
	process_further_signals = true;

	// Clear the link(ll-bit) information
	clear_llbit();
      }

      // Clear halt flag
      halted = false;

    }else{ // Handle maskable interrupts

      union EiintReq eireq_val;
      union EiintReq pre_eireq_val;
      bool is_fpi_win = false;
      bool is_ei_pri_masked = false;
      unsigned int ei_pri_val = 0;

      pre_eireq_val.val = eireq_val.val;
      eireq_val.val = eiintrq_info;

      // check priority between FPI and EIINT
      if(pending_fpu_imprecise_exception){
	is_fpi_win = true;
	ei_pri_val = (unsigned int)cpu_state->read_FPIPR_FPIPR();
	if(ei_pri_val > 16) ei_pri_val = 16;
      }
      if(interrupt_requested & INT_EIINT){
	if((!is_fpi_win) || ((is_fpi_win) && (ei_pri_val > eireq_val.bit_val.pri))){
	  is_fpi_win = false;
	  ei_pri_val = eireq_val.bit_val.pri;
	}
      }

      // check mask setting(ISPR)
      for(int i=0; i<16; i++){
	if((cpu_state->read_ISPR_ISP() & (0x1 << i))){
	  if(ei_pri_val >= i){
	    is_ei_pri_masked = true;
	    break;
	  }
	}
      }

      // check mask setting(PMR)
      if(!is_ei_pri_masked){
	unsigned int pmr_pm_val = (unsigned int)cpu_state->read_PMR_PM();
	if ((pmr_pm_val & 0x8000) == 0x8000) pmr_pm_val = pmr_pm_val | 0x10000;
	if((0x1 << ei_pri_val) & pmr_pm_val){
	  if(is_fpi_win){
	    //cpu_state->write_ICSR_PMFP(1); G3K doesn't support this bit.
	  }else if(interrupt_requested & INT_EIINT){
	    cpu_state->write_ICSR_PMEI(1);
	  }
	  is_ei_pri_masked = true;
	}
      }

      // accept the exeption
      //if (pending_fpu_imprecise_exception){
      if ((is_fpi_win) && (!is_ei_pri_masked)){
	if ((cpu_state->read_PSW_NP() | cpu_state->read_PSW_ID()) == 0){
          // Clear pending floating point imprecise_exceptions
          pending_fpu_imprecise_exception = 0; // false
          
          // Update registers
          cpu_state->write_EIPC(cpu_state->read_PC());
          cpu_state->write_EIPSW(cpu_state->read_PSW());
          cpu_state->write_EIIC(0x72);
	  cpu_state->write_PSW_UM(0);
	  // cpu_state->write_PSW_HVC(0); G3K doesn't support this bit
          cpu_state->write_PSW_ID(1);
          cpu_state->write_PSW_EP(1);
          cpu_state->write_FPEC(0);
	  //cpu_state->write_ICSR_PMFP(0); G3K doesn't support this bit.
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
      // if (interrupt_requested & INT_EIINT){ // Handle EIINT
      if ((interrupt_requested & INT_EIINT) && (!is_fpi_win) && (!is_ei_pri_masked)){
	if ((cpu_state->read_PSW_NP() | cpu_state->read_PSW_ID()) == 0){
	  if(handler_rint){
	    handler_addr += 0x100;
	  }else if(eireq_val.bit_val.tb){
	    unsigned int load_addr = cpu_state->read_INTBP()+(eireq_val.bit_val.ch*4);
            if(m_mdp_enable){
              //mpu_func == 0 means MDP or MIP occured
              //0x52a means MEI value when interrupt(table refernce method) occured
              
              if(mpu_func(rh850::MPU_KIND_R, rh850::WORD_SIZE, load_addr, 0x52a) == 0){
                eireq_val.val = pre_eireq_val.val;
                return true;
              }
            }
	    handler_addr = fastiss_memory->i32_load(cpu_state->read_INTBP()+(eireq_val.bit_val.ch*4));
	  }else{
	    handler_addr += 0x100+(eireq_val.bit_val.pri*0x10);
	  }
	  
          // Clear pending signals
	  interrupt_requested -= INT_EIINT;
	  eiintrq_info = 0;
	  
          // Update registers
          cpu_state->write_EIPC(cpu_state->read_PC());
          cpu_state->write_EIPSW(cpu_state->read_PSW());
          cpu_state->write_EIIC(0x1000+eireq_val.bit_val.ch);
	  //cpu_state->write_PSW_VM(0); G3K doesn't supprt this bit
	  cpu_state->write_PSW_UM(0);
	  //cpu_state->write_PSW_HVC(0); G3K doesn't supprt this bit
          cpu_state->write_PSW_ID(1);
          cpu_state->write_PSW_EP(0);
	  cpu_state->write_ICSR_PMEI(0);

          cpu_state->write_PC(handler_addr);

	  // update ISPR
	  if(cpu_state->read_INTCFG_ISPC() == 0){
	    cpu_state->write_ISPR_ISP((cpu_state->read_ISPR_ISP()) | (0x1 << ei_pri_val));
	  }
        
          // Acknowledge interrupt
	  union EiintAck eiack_val;
	  eiack_val.bit_val.ack  = true;
	  eiack_val.bit_val.ch   = eireq_val.bit_val.ch;
	  intack_eiint_requested = eiack_val.val;
	  eiintrq_ack_event.notify();
	  
          // Indicate that we processed a signal
          process_further_signals = true;

	  // Clear the link(ll-bit) information
	  clear_llbit();
        }
	// Clear halt flag
	halted = false;
      }else{
	// not reached
      }
    }
    if(interrupt_requested == INT_REQ){
      // Clear pending request flag
      interrupt_requested -= INT_REQ;
    }
  }
  return process_further_signals;
}

void rh850::core::fetch_exception(void)
{
  sc_core::sc_report_handler::report(sc_core::SC_ERROR, "rh850", "Instruction fetch failed", "<UNKNOWN>", 0);    
}

void rh850::core::terminate_quantum(void)
{
  local_time_limit = 0;
}

void rh850::core::mmu_configuration_changed(void)
{
  // Flush the Data caches and DMI handles
  fastiss_bus_interface->invalidate_all_direct_mem_ptr();

  // Flush the L1 Block function cache
  flush_l1_cache();
}

void rh850::core::effective_address_configuration_changed(void)
{
  // Flush the L1 Block function cache
  flush_l1_cache();
}


// ----------------------------------------------------------------------------
// Signal Handlers
// ----------------------------------------------------------------------------

void rh850::core::reset_handler(void)
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

void rh850::core::fenmirq_update(void)
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
    if(((interrupt_requested & (INT_RESET | INT_FEINT | INT_EIINT | INT_SYSERR)) == 0) &&
       ((interrupt_requested & INT_REQ) == INT_REQ)){
      interrupt_requested -= INT_REQ;
    }
  }
}

void rh850::core::feintrq_update(void)
{
  bool feintrq_sig_val = feintrq.read();
  
  if (feintrq_sig_val)
  {
    interrupt_requested |= (INT_FEINT | INT_REQ);
  }
  else
  {
    if((interrupt_requested & INT_FEINT) == INT_FEINT){
      interrupt_requested -= INT_FEINT;
    }
    if(((interrupt_requested & (INT_RESET | INT_FENMI | INT_EIINT | INT_SYSERR)) == 0) &&
       ((interrupt_requested & INT_REQ) == INT_REQ)){
      interrupt_requested -= INT_REQ;
    }
  }
}

void rh850::core::eiintrq_update(void)
{
  union EiintReq eiintrq_sig_val;
  eiintrq_sig_val.val = eiintrq.read();
  
  if (eiintrq_sig_val.bit_val.req & 0x1)
  {
    interrupt_requested |= (INT_EIINT | INT_REQ);
    eiintrq_info = eiintrq_sig_val.val;
  }
  else
  {
    if((interrupt_requested & INT_EIINT) == INT_EIINT){
      interrupt_requested -= INT_EIINT;
    }
    if(((interrupt_requested & (INT_RESET | INT_FENMI | INT_FEINT | INT_SYSERR)) == 0) &&
       ((interrupt_requested & INT_REQ) == INT_REQ)){
      interrupt_requested -= INT_REQ;
    }
    if((0x1 << eiintrq_sig_val.bit_val.pri) & cpu_state->read_PMR_PM()){
      cpu_state->write_ICSR_PMEI(0);
    }
    eiintrq_info = eiintrq_sig_val.val;
  }
}

void rh850::core::syserr_req_update(void)
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
    if(((interrupt_requested & (INT_RESET | INT_FENMI | INT_FEINT | INT_EIINT)) == 0) &&
       ((interrupt_requested & INT_REQ) == INT_REQ)){
      interrupt_requested -= INT_REQ;
    }
  }
}

void rh850::core::syserr_code_update(void)
{
  syserr_code_val = (unsigned int)syserr_code.read();
}

void rh850::core::fenmirq_ack_update(void)
{
  fenmirq_ack.write(intack_fenmi_requested);
  if(intack_fenmi_requested){
    intack_fenmi_requested = false;
    fenmirq_ack_event.notify(cpu_state->read_cycle_period(), sc_core::SC_PS);
  }
}

void rh850::core::feintrq_ack_update(void)
{
  feintrq_ack.write(intack_feint_requested);
  if(intack_feint_requested){
    intack_feint_requested = false;
    feintrq_ack_event.notify(cpu_state->read_cycle_period(), sc_core::SC_PS);
  }
}

void rh850::core::eiintrq_ack_update(void)
{
  eiintrq_ack.write(intack_eiint_requested);
  if(intack_eiint_requested){
    intack_eiint_requested = 0;
    eiintrq_ack_event.notify(cpu_state->read_cycle_period(), sc_core::SC_PS);
  }
}

void rh850::core::syserr_ack_update(void)
{
  syserr_ack.write(intack_syserr_requested);
  if(intack_syserr_requested){
    intack_syserr_requested = false;
    syserr_ack_event.notify(cpu_state->read_cycle_period(), sc_core::SC_PS);
  }
}


// ----------------------------------------------------------------------------
// Additional functions for RH850
// ----------------------------------------------------------------------------

void rh850::core::enter_halt(void)
{
  // The time limit is set to 0 in order to terminate the quanta
  local_time_limit = 0;
  
  // Enter the halt state
  halted = true;
}

void rh850::core::activate_interrupt_accepted_analysis_point(void)
{
  m_interrupt_accepted_analysis_point->activate();
}

unsigned long long rh850::core::get_total_inst_num()
{
  return(get_instruction_count());
}

double rh850::core::get_total_time_sec()
{
  return((sc_time_stamp() + get_local_time()).to_seconds());

}

unsigned long long rh850::core::get_total_cycle()
{
  return((unsigned long long)(sc_time_stamp() + get_local_time()).to_double()/cpu_state->read_cycle_period());
}

void rh850::core::set_peid(unsigned int peid)
{
  pe_id.value = peid;
}

// Update SWD0PCAD value (for SWDT)
void rh850::core::set_swd0pcad_val(unsigned int val)
{
  swd0pcad_val.value = val;
  cpu_state->write_swd0pcad_val(swd0pcad_val.value);
}

// get executed time of SWD0PCAD address  (for SWDT)
unsigned int rh850::core::get_swd0pcad_exec_count_val(void)
{
  return(cpu_state->read_swd0pcad_exec_count());
}

// Set SWDT detection mode (for SWDT)
void rh850::core::set_swdt_enable(bool mode)
{
  swdt_enable.value = mode;
}

// get memory access type  (for Guard function)
unsigned int rh850::core::get_mem_acc_type(void)
{
  return(cpu_state->read_mem_acc_type());
}

// set LinkManager module's pointer
void rh850::core::set_link_manager(LinkManager* p_link_manager)
{
  link_manager = p_link_manager;
}

// get G3M payload information from each system register
void rh850::core::get_g3m_params(unsigned int& vcid, unsigned int& tcid, unsigned int& spid, bool& userMode, bool& virtualMode)
{
  if (cpu_milestone.value == FASTISS_G3K_PCU){
    vcid = 0;
    tcid = 0;
    spid = cpu_state->read_MCFG0_SPID();
    userMode    = cpu_state->read_PSW_UM() != 0;
    virtualMode = 0;
  }else{
    //vcid = cpu_state->read_HTCFG0_VCID(); G3K doesn't supprt this bit
    //tcid = cpu_state->read_HTCFG0_TCID(); G3K doesn't supprt this bit
    spid = cpu_state->read_MCFG0_SPID();
    userMode    = cpu_state->read_PSW_UM() != 0;
    // virtualMode = cpu_state->read_PSW_VM() != 0; G3K doesn't supprt this bit
  }
}

void rh850::core::force_wait(void)
{
  // Advance GT by any non-accured local time
  sc_core::wait(get_local_time());
  cpu_state->write_accrued_time((unsigned int)get_elapsed_time().value());
}

void rh850::core::force_wait_snooze(void)
{
  // Advance GT by any non-accured local time
  cpu_state->increment_time(cpu_state->read_cycle_period()*snz_time.value);
  sc_core::wait(get_local_time());
  cpu_state->write_accrued_time((unsigned int)get_elapsed_time().value());
}

void rh850::core::clear_llbit(void)
{
  if(link_func_enable.value){
    link_manager->ClearLlbit(pe_id.value);
  }
}

unsigned int rh850::core::get_cpu_milestone(void)
{
  return(cpu_milestone.value);
}
  
void rh850::core::set_mpu_enable(bool is_mip, bool is_mdp)
{
  m_mip_enable = is_mip;
  m_mdp_enable = is_mdp;
  if(custom){
    custom->set_mdp_enable(is_mdp);
  }
}

void rh850::core::set_mpu_entry(unsigned int mpu_entry)
{
  m_mpu_entry = mpu_entry;
  cpu_state->write_mpu_entry((unsigned char)mpu_entry);
  cpu_state->write_MPTRGN(mpu_entry - 1);
}

unsigned int rh850::core::mpu_func(unsigned int mpu_kind, unsigned int size, unsigned int address, unsigned int mei_val)
{
  if(cpu_state->read_MPM_MPE() == 1 && (! (cpu_state->read_PSW_UM() == 0 && cpu_state->read_MPM_SVP() == 0)))
  {
    //printf("core::mpu_func\n");
    //printf("  mpu_kind=%d\n size=%d address=0x%x mei_val=0x%x mpm=0x%x\n", mpu_kind, size, address, mei_val, read_MPM());
    //fflush(stdout);
    unsigned int mpla[ICUM_MPU_MAX_NUM];
    unsigned int mpua[ICUM_MPU_MAX_NUM];
    unsigned int mpat[ICUM_MPU_MAX_NUM];
    unsigned int lower_address = address;
    unsigned int middle_address = address + (size / 2);
    unsigned int upper_address = address + size  - 1;
    bool is_lower_address_in_region = false;
    bool is_middle_address_in_region = false;
    bool is_upper_address_in_region = false;
    bool is_lower_address_allow = false;
    bool is_middle_address_allow = false;
    bool is_upper_address_allow = false;
    bool is_size_other_than_word = false;
    if (WORD_SIZE < size) {
      is_size_other_than_word = true;
    }

    //These codes are for debug and will be remaining for the future.
    //if (is_size_other_than_word){
    //  printf("  lower_address=0x%x middle_address=0x%x upper_address=0x%x\n", lower_address, middle_address, upper_address);
    //}
    //else{
    //  printf("  lower_address=0x%x upper_address=0x%x\n", lower_address, upper_address);
    //}
    for(unsigned int i = 0; i < m_mpu_entry; i++)
    {
      if (i < 8){
        mpla[i] = cpu_state->read_SR_SELID_6((rh850::SR_SELID_6_index_t)(4 * i));
        mpua[i] = cpu_state->read_SR_SELID_6((rh850::SR_SELID_6_index_t)(4 * i + 1)) + 3;
        mpat[i] = cpu_state->read_SR_SELID_6((rh850::SR_SELID_6_index_t)(4 * i + 2));
      }
      else{
        mpla[i] = cpu_state->read_SR_SELID_7((rh850::SR_SELID_7_index_t)(4 * (i - 8)));
        mpua[i] = cpu_state->read_SR_SELID_7((rh850::SR_SELID_7_index_t)(4 * (i - 8) + 1)) + 3;
        mpat[i] = cpu_state->read_SR_SELID_7((rh850::SR_SELID_7_index_t)(4 * (i - 8) + 2));
      }
      //These codes are for debug and will be remaining for the future.
      //printf("mpla%d=0x%x mpua%d=0x%x mpat%d=0x%x\n", i, mpla[i], i, mpua[i], i, mpat[i]);
      if ((mpat[i] & 0x80)== 0x80 && 
         (!((mpat[i] & 0x40) == 0 &&
         (((mpat[i] >> 16) & 0x3ff) != cpu_state->read_ASID_ASID())))){
        bool mpu_allow_func_result = is_mpu_allow_func(mpu_kind, mpat[i]);
        if(mpla[i] <= lower_address && lower_address <= mpua[i]){
          is_lower_address_in_region = true;
          is_lower_address_allow |= mpu_allow_func_result;
        }
        if(is_size_other_than_word && mpla[i] <= middle_address && middle_address <= mpua[i]){
          is_middle_address_in_region = true;
          is_middle_address_allow |= mpu_allow_func_result;
        }
        if(mpla[i] <= upper_address && upper_address <= mpua[i]){
          is_upper_address_in_region = true;
          is_upper_address_allow |= mpu_allow_func_result;
        }

        if(is_size_other_than_word){
          if(is_lower_address_allow && is_middle_address_allow && is_upper_address_allow){
            break;
          }
        }
        else{
          if(is_lower_address_allow  && is_upper_address_allow){
            break;
          }
        }
      }
      //These codes are for debug and will be remaining for the future.
      //if (is_size_other_than_word){
      //  printf("  is_lower_address_in_region=%d is_middle_address_in_region=%d is_upper_address_in_region=%d\n", is_lower_address_in_region, is_middle_address_in_region, is_upper_address_in_region);
      //  printf("  is_lower_address_allow=%d is_middle_address_allow=%d is_upper_address_allow=%d\n", is_lower_address_allow, is_middle_address_allow, is_upper_address_allow);
      //}
      //else{
      //  printf("  is_lower_address_in_region=%d is_upper_address_in_region=%d\n", is_lower_address_in_region, is_upper_address_in_region);
      //  printf("  is_lower_address_allow=%d is_upper_address_allow=%d\n", is_lower_address_allow,  is_upper_address_allow);
      //}
    }
    bool default_func_result = is_mpu_default_func(mpu_kind);
    if(! is_lower_address_in_region){
      is_lower_address_allow = default_func_result;
    }
    if(is_size_other_than_word && (! is_middle_address_in_region)){
      is_middle_address_allow = default_func_result;
    }
    if(! is_upper_address_in_region){
      is_upper_address_allow = default_func_result;
    }
      //These codes are for debug and will be remaining for the future.
    //if (is_size_other_than_word){
    //  printf("  is_lower_address_allow=%d is_middle_address_allow=%d is_upper_address_allow=%d\n", is_lower_address_allow, is_middle_address_allow, is_upper_address_allow);
    //}
    //else{
    //  printf("  is_lower_address_allow=%d is_upper_address_allow=%d\n", is_lower_address_allow,  is_upper_address_allow);
    //}
    //fflush(stdout);

    if (is_size_other_than_word){
      if (! (is_upper_address_allow && is_lower_address_allow && is_middle_address_allow)){
        update_mpu_exception(mpu_kind, size, address, mei_val);
        //These codes are for debug and will be remaining for the future.
        //printf("  finish core function&return 0\n"); fflush(stdout);
        return 0;
      }
    }
    else{
      if(!( is_upper_address_allow && is_lower_address_allow)){
        update_mpu_exception(mpu_kind, size, address, mei_val);
        //These codes are for debug and will be remaining for the future.
        //printf("  finish core function&return 0\n"); fflush(stdout);
        return 0;
      }
    }
  }
  //These codes are for debug and will be remaining for the future.
  //printf("  finish core function&return 1\n"); fflush(stdout);
  return 1;
}

bool rh850::core::is_mpu_allow_func(unsigned int mpu_kind, unsigned int mpatn)
{
    switch(mpu_kind) {
      case rh850::MPU_KIND_R:
      case rh850::MPU_KIND_RMW:
      case rh850::MPU_KIND_BL_R:
        if(cpu_state->read_PSW_UM()){
          if ((mpatn & 0x1) == 0x1){
            return true;
          }
          else {
            return false;
          }
        }
        else{
          if ((mpatn & 0x8) == 0x8){
            return true;
          }
          else {
            return false;
          }
        }
      case rh850::MPU_KIND_W:
      case rh850::MPU_KIND_BL_W:
        if(cpu_state->read_PSW_UM()){
          if ((mpatn & 0x2) == 0x2){
            return true;
          }
          else {
            return false;
          }
        }
        else{
          if ((mpatn & 0x10) == 0x10){
            return true;
          }
          else {
            return false;
          }
        }
      case rh850::MPU_KIND_X:
        if(cpu_state->read_PSW_UM()){
          if ((mpatn & 0x4) == 0x4){
            return true;
          }
          else {
            return false;
          }
        }
        else{
          if ((mpatn & 0x20) == 0x20){
            return true;
          }
          else {
            return false;
          }
        }
   }
}

bool rh850::core::is_mpu_default_func(unsigned int mpu_kind)
{
  switch (mpu_kind) {
    case rh850::MPU_KIND_R:
    case rh850::MPU_KIND_RMW:
    case rh850::MPU_KIND_BL_R:
      return cpu_state->read_MPM_DR();
    case rh850::MPU_KIND_W:
    case rh850::MPU_KIND_BL_W:
      return cpu_state->read_MPM_DW();
    case rh850::MPU_KIND_X:
      return cpu_state->read_MPM_DX();
  }
}

void rh850::core::update_mpu_exception(unsigned int mpu_kind, unsigned int size, unsigned int address, unsigned int mei_val)
{      
  //These codes are for debug and will be remaining for the future.
  //printf("core::update_mpu_exception\n");
  //printf("  mpu_kind=%d, size=%d, address=0x%x, mei_val=0x%x\n", mpu_kind, size, address, mei_val);
  //fflush(stdout);
  unsigned int base_addr = 0;
  if(cpu_state->read_PSW_EBV()){
    base_addr = cpu_state->read_EBASE();
  }
  else{
    base_addr = cpu_state->read_RBASE();
  }
  unsigned int pc_load = cpu_state->read_PC();
  unsigned int feic_value;
  if (mpu_kind == rh850::MPU_KIND_X){
    //FEIC value
    feic_value = 0x00000090;
    //Write PC to MEA
    cpu_state->write_MEA(pc_load);
  }
  else{
    //FEIC value
    unsigned int feic_up16b_value;
    switch (mpu_kind) {
     case rh850::MPU_KIND_R:
       if (cpu_state->read_PSW_UM()){
         feic_up16b_value = (((unsigned int)is_miss_align_access_func(size, address) << 24) | 0x00010000);
       }
       else{
         feic_up16b_value = (((unsigned int)is_miss_align_access_func(size, address) << 24) | 0x00080000);
       }
       break;
     case rh850::MPU_KIND_W:
       if (cpu_state->read_PSW_UM()){
         feic_up16b_value = (((unsigned int)is_miss_align_access_func(size, address) << 24) | 0x00020000);
       }
       else{
         feic_up16b_value = (((unsigned int)is_miss_align_access_func(size, address) << 24) | 0x00100000);
       }
       break;
     case rh850::MPU_KIND_RMW:
       feic_up16b_value = (((unsigned int)is_miss_align_access_func(size, address) << 24) | 0x00400000);
       break;
     case rh850::MPU_KIND_BL_R:
     case rh850::MPU_KIND_BL_W:
       feic_up16b_value = 0x00800000;
       break;
    }
    feic_value = (feic_up16b_value | 0x00000091);
    //Write address to MEA and instruction's info to MEI
    cpu_state->write_MEA(address); 
    cpu_state->write_MEI(mei_val);
  }

  cpu_state->write_FEPC(pc_load);
  cpu_state->write_FEPSW(cpu_state->read_PSW());

  cpu_state->write_FEIC(feic_value);

  cpu_state->write_PSW_HVC(false);
  cpu_state->write_PSW_UM(false);
  cpu_state->write_PSW_ID(true);
  cpu_state->write_PSW_NP(true);
  cpu_state->write_PSW_EP(true);

  cpu_state->write_PC((0xfffffe00 & base_addr) + 0x90);

  if(link_func_enable.value){
    link_manager->ClearLlbit(pe_id.value);
  }

  // Activate interrupt accepted analysis point
  if (m_interrupt_accepted_analysis_point->has_attachments())
  {
    m_interrupt_accepted_analysis_point->activate();
  }
  //custom->get_exit_block();
}

bool rh850::core::is_miss_align_access_func(unsigned int size, unsigned int address)
{
  switch(size) {
    case rh850::HALF_WORD_SIZE:
      if ((address & 0x1) == 0){
        return false;
      }
      else {
        return true;
      }
    case rh850::WORD_SIZE:
      if ((address & 0x3) == 0){
        return false;
      }
      else {
        return true;
      }
    case rh850::DOUBLE_WORD_SIZE:
      if ((address & 0x7) == 0){
        return false;
      }
      else {
        return true;
      }
    default:
      return false;
  }
}

void rh850::core::output_member_val(void)
{
  printf("rh850::core::output_member_val() is start.\n");
  printf("  public member\n");
  printf("    quantum_period=%s\n", quantum_period.value.to_string().c_str());
  printf("    block_size=%d\n", block_size.value);
  printf("    blocking_tlm=%d\n", blocking_tlm.value);
  printf("    tlm_response_error_as_error=%d\n", tlm_response_error_as_error.value);
  printf("    adaptive_count=%d\n", adaptive_count.value);
  printf("    translation_partition_size=%d\n", translation_partition_size.value);
  printf("    pe_id=%d\n", pe_id.value);
  printf("    link_func_enable=%d\n", link_func_enable.value);
  printf("    snz_time=%d\n", snz_time.value);
  printf("    cpu_milestone=%d\n", cpu_milestone.value);
  //printf("    bus_initiator=%d\n", bus_initiator); // initiator spcket
  //printf("    clock=%d\n", clock); // port
  //printf("    continue_event=%d\n", continue_event); // event
  //printf("    step_event=%d\n", step_event); // event
  //printf("    step_completed_event=%d\n", step_completed_event); // event
  //printf("    reset_done_event=%d\n", reset_done_event); //event
  printf("    custom=0x%08x\n", &custom);
  printf("    link_manager=0x%08x\n", &link_manager);
  //printf("    reset=%d\n", reset); // port
  //printf("    fenmirq=%d\n", fenmirq); // port
  //printf("    fenmirq_ack=%d\n", fenmirq_ack); // port
  //printf("    feintrq=%d\n", feintrq); // port
  //printf("    feintrq_ack=%d\n", feintrq_ack); // port
  //printf("    eiintrq=%d\n", eiintrq); // port
  //printf("    eiintrq_ack=%d\n", eiintrq_ack); // port
  //printf("    syserr_ack=%d\n", syserr_ack); // port
  //printf("    syserr_req=%d\n", syserr_req); // port
  //printf("    syserr_code=%d\n", syserr_code); // port
  //printf("    syserr_ebv=%d\n", syserr_ebv); // port
  printf("  private member\n");
  //printf("    fenmirq_ack_event=%d\n", fenmirq_ack_event); // event
  //printf("    feintrq_ack_event=%d\n", feintrq_ack_event); // event
  //printf("    eiintrq_ack_event=%d\n", eiintrq_ack_event); // event
  //printf("    syserr_ack_event=%d\n", syserr_ack_event);
  printf("    interrupt_requested=0x%08x\n", interrupt_requested);
  printf("    intack_fenmi_requested=%d\n", intack_fenmi_requested);
  printf("    intack_feint_requested=%d\n", intack_feint_requested);
  printf("    intack_eiint_requested=%d\n", intack_eiint_requested);
  printf("    intack_syserr_requested=%d\n", intack_syserr_requested);
#if defined(_MSC_VER)
  printf("    eiintrq_info=0x%I64x\n", eiintrq_info);
#else
  printf("    eiintrq_info=0x%llx\n", eiintrq_info);
#endif
  printf("    syserr_code_val=%d\n", syserr_code_val);
  printf("    pending_fpu_imprecise_exception=%d\n", pending_fpu_imprecise_exception);
  printf("    halted=%d\n", halted);
#if defined(_MSC_VER)
  printf("    frequency=%I64d\n", frequency);
#else
  printf("    frequency=%lld\n", frequency);
#endif
  printf("    in_reset=%d\n", in_reset);
  printf("    m_mip_enable=%d\n", m_mip_enable);
  printf("    m_mdp_enable=%d\n", m_mdp_enable);
  printf("    m_mpu_entry=%d\n", m_mpu_entry);
  printf("rh850::core::output_member_val() is end.\n");
}


//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
