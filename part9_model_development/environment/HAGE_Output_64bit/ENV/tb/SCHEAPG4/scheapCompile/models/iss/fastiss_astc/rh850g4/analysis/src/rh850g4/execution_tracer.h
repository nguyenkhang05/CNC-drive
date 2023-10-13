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

#ifndef _RH850G4_EXECUTION_TRACER_H_
#define _RH850G4_EXECUTION_TRACER_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <exception>
#include "rh850g4/core_base.h"
#include "rh850g4/analysis_types.h"
#include <sstream>
//------------------------------------------------------------------------------
// Execution Tracer
//------------------------------------------------------------------------------

namespace rh850g4
{
  /** 
   * \brief Exception class used to indicate that an issue has occurred with
   * the usage of the execution tracer.
   */
  class execution_tracer_error_t : public std::exception
  {
  public:
    /**
     * \brief Execution tracer error constructor.
     * \param message Message describing the error condition in greater detail.
     */
    execution_tracer_error_t(const char *const& message) :
#ifdef _MSC_VER
      std::exception(message)
#else
      std::exception(),
      m_message(message)
#endif
    {
    }
    
#ifndef _MSC_VER
    /**
     * \brief Overload of the std::exception::what() method to return the
     * message specified in the constructor.
     * \return Message describing the error condition in greater detail.
     */
    virtual const char* what() const throw()
    {
      return m_message;
    }
    
  private:
    /**
     * \brief Message describing the error condition in greater detail.
     */
    const char *m_message;
#endif
  };
  
  /** 
   * \brief Execution tracer class.
   */
  class execution_tracer
  {
  public:
    /**
      * \brief Class containing the information we need to capture at translation
      * time about each instruction.
      */
    class instr_info_t
    {
    public:
     /**
       * \brief Instruction disassembly string.
       */
      std::string m_disass;
      /**
       * \brief Instruction length. Note that this may be longer than the opcode
       * which only captures that part of the opcode that is used for instruction
       * decode/identification.
       */
      unsigned int m_length;
      /**
       * \brief Instruction opcode. Note that for variable length instruction set
       * architectures this may be only partially valid, the instruction length
       * can be used to determine what part of the opcode is valid.
       */
      opcode_t m_opcode;
     };
    
    /**
     * \brief Execution tracer constructor.
     * \param peid A value of PE number (PEID)
     * \param fp_trace A pointer of trace file
     */
    execution_tracer(unsigned int peid,
                     FILE *fp_trace,
		     bool is_old_format
    ) :
      m_core(NULL),
      m_instr_info(),
      m_trace_callback(NULL),
      m_last_trace_callback(NULL),
      m_post_fetch_attachment(NULL),
      m_post_decode_attachment(NULL),
      m_post_translate_loop_attachment(NULL),
      m_pe_id(peid),
      m_fp_trace(fp_trace),
      m_is_old_format(is_old_format),
      m_total_inst_count(0),
      m_trace_regs(false),
      m_psw_val(0)
    {
      for(int i=0; i<32; i++){
        m_greg_val[i] = 0;
        m_wreg_val[i*2] = 0;
        m_wreg_val[(i*2)+1] = 0;
      }
      m_psw_val = 0;
    }

    /**
     * \brief Execution tracer destructor.
     */
    ~execution_tracer()
    {
      // Clear cached instruction information
      m_instr_info.clear();
    }
    
    /**
     * \brief Attaches to callouts to the specified core instance's callout
     * hooks and stores a handle to the core instance so that the trace()
     * operations can query the program counter etc. Note that this tracer
     * may only be associated with a single core instance. It is considered
     * an error to attach the tracer to more than one core instance.
     * \param core Pointer to the core instance for which to trace execution.
     */
    void attach(rh850g4::core_base_t *core)
    {
#if !defined(__VLAB_MIT__)      
      // Check core handle is valid
      if (core == NULL)
      {
        std::ostringstream os;
        os << "Could not attach, NULL core instance specified";
        os << std::endl;
        throw execution_tracer_error_t(os.str().c_str());
      }
      
      // Check not already attached to a core
      if (m_core != NULL)
      {
        std::ostringstream os;
        os << "Could not attach to '";
        os << core->name();
        os << "', already attached to '";
        os << m_core->name();
        os << "'";
        os << std::endl;
        throw execution_tracer_error_t(os.str().c_str());
      }
      
      // Store reference to core instance
      m_core = core;

      // Register trace operation 
      m_trace_callback = m_core->register_analysis_callback((rh850g4::analysis_callback)trace_callback);
      m_last_trace_callback = m_core->register_analysis_callback((rh850g4::analysis_callback)last_trace_callback);
            
      // Attach to analysis points
      m_post_fetch_attachment = m_core->attach("post_fetch", (rh850g4::analysis_callback)fetch_callback, this);
      m_post_decode_attachment = m_core->attach("post_decode", (rh850g4::analysis_predicate)decode_predicate, this);
      m_post_translate_loop_attachment = m_core->attach("post_translate_loop", (rh850g4::analysis_predicate)post_translate_loop_predicate, this);
#endif      
      // set current value of registers
      m_greg_val[0] = m_core->read_r0();
      m_greg_val[1] = m_core->read_r1();
      m_greg_val[2] = m_core->read_r2();
      m_greg_val[3] = m_core->read_r3();
      m_greg_val[4] = m_core->read_r4();
      m_greg_val[5] = m_core->read_r5();
      m_greg_val[6] = m_core->read_r6();
      m_greg_val[7] = m_core->read_r7();
      m_greg_val[8] = m_core->read_r8();
      m_greg_val[9] = m_core->read_r9();
      m_greg_val[10] = m_core->read_r10();
      m_greg_val[11] = m_core->read_r11();
      m_greg_val[12] = m_core->read_r12();
      m_greg_val[13] = m_core->read_r13();
      m_greg_val[14] = m_core->read_r14();
      m_greg_val[15] = m_core->read_r15();
      m_greg_val[16] = m_core->read_r16();
      m_greg_val[17] = m_core->read_r17();
      m_greg_val[18] = m_core->read_r18();
      m_greg_val[19] = m_core->read_r19();
      m_greg_val[20] = m_core->read_r20();
      m_greg_val[21] = m_core->read_r21();
      m_greg_val[22] = m_core->read_r22();
      m_greg_val[23] = m_core->read_r23();
      m_greg_val[24] = m_core->read_r24();
      m_greg_val[25] = m_core->read_r25();
      m_greg_val[26] = m_core->read_r26();
      m_greg_val[27] = m_core->read_r27();
      m_greg_val[28] = m_core->read_r28();
      m_greg_val[29] = m_core->read_r29();
      m_greg_val[30] = m_core->read_r30();
      m_greg_val[31] = m_core->read_r31();
      m_psw_val      = m_core->read_PSW();
      m_wreg_val[0] = m_core->read_wr0();
      m_wreg_val[1] = m_core->read_wr0_u();
      m_wreg_val[2] = m_core->read_wr1();
      m_wreg_val[3] = m_core->read_wr1_u();
      m_wreg_val[4] = m_core->read_wr2();
      m_wreg_val[5] = m_core->read_wr2_u();
      m_wreg_val[6] = m_core->read_wr3();
      m_wreg_val[7] = m_core->read_wr3_u();
      m_wreg_val[8] = m_core->read_wr4();
      m_wreg_val[9] = m_core->read_wr4_u();
      m_wreg_val[10] = m_core->read_wr5();
      m_wreg_val[11] = m_core->read_wr5_u();
      m_wreg_val[12] = m_core->read_wr6();
      m_wreg_val[13] = m_core->read_wr6_u();
      m_wreg_val[14] = m_core->read_wr7();
      m_wreg_val[15] = m_core->read_wr7_u();
      m_wreg_val[16] = m_core->read_wr8();
      m_wreg_val[17] = m_core->read_wr8_u();
      m_wreg_val[18] = m_core->read_wr9();
      m_wreg_val[19] = m_core->read_wr9_u();
      m_wreg_val[20] = m_core->read_wr10();
      m_wreg_val[21] = m_core->read_wr10_u();
      m_wreg_val[22] = m_core->read_wr11();
      m_wreg_val[23] = m_core->read_wr11_u();
      m_wreg_val[24] = m_core->read_wr12();
      m_wreg_val[25] = m_core->read_wr12_u();
      m_wreg_val[26] = m_core->read_wr13();
      m_wreg_val[27] = m_core->read_wr13_u();
      m_wreg_val[28] = m_core->read_wr14();
      m_wreg_val[29] = m_core->read_wr14_u();
      m_wreg_val[30] = m_core->read_wr15();
      m_wreg_val[31] = m_core->read_wr15_u();
      m_wreg_val[32] = m_core->read_wr16();
      m_wreg_val[33] = m_core->read_wr16_u();
      m_wreg_val[34] = m_core->read_wr17();
      m_wreg_val[35] = m_core->read_wr17_u();
      m_wreg_val[36] = m_core->read_wr18();
      m_wreg_val[37] = m_core->read_wr18_u();
      m_wreg_val[38] = m_core->read_wr19();
      m_wreg_val[39] = m_core->read_wr19_u();
      m_wreg_val[40] = m_core->read_wr20();
      m_wreg_val[41] = m_core->read_wr20_u();
      m_wreg_val[42] = m_core->read_wr21();
      m_wreg_val[43] = m_core->read_wr21_u();
      m_wreg_val[44] = m_core->read_wr22();
      m_wreg_val[45] = m_core->read_wr22_u();
      m_wreg_val[46] = m_core->read_wr23();
      m_wreg_val[47] = m_core->read_wr23_u();
      m_wreg_val[48] = m_core->read_wr24();
      m_wreg_val[49] = m_core->read_wr24_u(); 
      m_wreg_val[50] = m_core->read_wr25();
      m_wreg_val[51] = m_core->read_wr25_u();
      m_wreg_val[52] = m_core->read_wr26();
      m_wreg_val[53] = m_core->read_wr26_u();
      m_wreg_val[54] = m_core->read_wr27();
      m_wreg_val[55] = m_core->read_wr27_u();
      m_wreg_val[56] = m_core->read_wr28();
      m_wreg_val[57] = m_core->read_wr28_u();
      m_wreg_val[58] = m_core->read_wr29();
      m_wreg_val[59] = m_core->read_wr29_u();
      m_wreg_val[60] = m_core->read_wr30();
      m_wreg_val[61] = m_core->read_wr30_u();
      m_wreg_val[62] = m_core->read_wr31();
      m_wreg_val[63] = m_core->read_wr31_u();
    }
    
    /**
     * \brief Detaches the tracer from the core with which it is associated.
     * Note that the core manages any translation caches flushes that may arise
     * from detaching from an analysis point.
     */
    void detach()
    {
#if !defined(__VLAB_MIT__)      
      // Check already attached to a core
      if (m_core == NULL)
      {
        std::ostringstream os;
        os << "Could not detach, not currently attached to core instance";
        os << std::endl;
        throw execution_tracer_error_t(os.str().c_str());
      }
      
      // Detach from analysis points
      m_core->detach(m_post_fetch_attachment);
      m_core->detach(m_post_decode_attachment);
      m_core->detach(m_post_translate_loop_attachment);
      
      // Clear reference to core instance
      m_core = NULL;
#endif      
    }

    /**
     * \brief Post decode predicate operation, this operation checks to see if
     * the current instruction is valid and if so causes the analysis callback
     * manager to emit a call to the trace callback.
     * \param tracer Pointer to self.
     */
    static rh850g4::analysis_predicate_response decode_predicate(rh850g4::execution_tracer *tracer)
    {
      analysis_predicate_response ret_val;
      
      if ((tracer->m_core != NULL) && tracer->m_core->is_decode_valid())
      {
        ret_val.set_analysis_callback(tracer->m_trace_callback, tracer);
      }
      
      return ret_val;
    }

    /**
     * \brief Post translate loop predicate operation causes
     * the analysis callback manager to emit a call to the trace
     * callback.
     * \param tracer Pointer to self.
     */
    static rh850g4::analysis_predicate_response post_translate_loop_predicate(rh850g4::execution_tracer *tracer)
    {
      analysis_predicate_response ret_val;
      ret_val.set_analysis_callback(tracer->m_last_trace_callback, tracer);
      return ret_val;
    }

    /**
     * \brief Fetch phase callback operation. This operation stores opcode
     * and mnemonic information for later use by the trace callback.  Note
     * that we are expected to overwrite any locally cached translation
     * information when re-translation takes place.
     * \param tracer Pointer to self.
     */
    static void fetch_callback(rh850g4::execution_tracer *tracer)
    {
      rh850g4::core_base_t *core = tracer->m_core;
      if ((core != NULL) && core->is_decode_valid())
      {
        unsigned int PC_val = core->get_decode_PC();
        rh850g4::execution_tracer::instr_info_t &instr_info = tracer->m_instr_info[PC_val];
        instr_info.m_disass = core->get_decode_disass();
        instr_info.m_length = core->get_decode_length();
        instr_info.m_opcode = core->get_decode_opcode();
      }
    }
    

    /**
     * \brief Trace callback operation. Calls to this operation cause the
     * current CPU state to be displayed. Opcode and mnemonic information
     * are retrieved from the information stored by the fetch phase callback
     * operation.
     * \param tracer Pointer to self.
     */
    static void trace_callback(rh850g4::execution_tracer *tracer)
    {
      rh850g4::core_base_t *core = tracer->m_core;
      if (core != NULL)
      {
	unsigned int PC_val = core->read_PC();
	rh850g4::execution_tracer::instr_info_t &instr_info = tracer->m_instr_info[PC_val];

	if (tracer->m_trace_regs)
	  {
	    tracer->trace_registers();
	  }
	tracer->m_trace_regs = true;
	char str_pe_data[128];
	char str_1st_data[128];
	char str_2nd_data[128];
	char str_3rd_data[128];

	if((FILE*)0 != tracer->m_fp_trace){
	  // Display PC and nemonic

	  if(tracer->m_is_old_format){
	    sprintf(str_pe_data, "PE%d: ", tracer->m_pe_id);
	  }else{
	    if ((core->read_PSWH() & 0x80000000) == 0){
	      sprintf(str_pe_data, "PE%d:HV : ", tracer->m_pe_id);
	    }else{
	      sprintf(str_pe_data, "PE%d:GM%d: ", tracer->m_pe_id, ((core->read_PSWH() >> 8) & 0x00000007));
	    }
	  }

#if defined(_MSC_VER)
	  sprintf(str_1st_data, "%12I64d: 0x%08x  ", 
		  core->get_instruction_count()+1, PC_val);
#else
	  sprintf(str_1st_data, "%12lld: 0x%08x  ", 
		  core->get_instruction_count()+1, PC_val);
#endif

	  if(instr_info.m_length == 2){
	    sprintf(str_2nd_data, "%04x                ",
		    (unsigned int)(instr_info.m_opcode & 0xffff));
	  }else if(instr_info.m_length == 4){
	    sprintf(str_2nd_data, "%04x %04x           ",
		    (unsigned int)(instr_info.m_opcode & 0xffff),
		    (unsigned int)((instr_info.m_opcode >> 16) & 0xffff));
	  }else if(instr_info.m_length == 6){
	    sprintf(str_2nd_data, "%04x %04x %04x      ", 
		    (unsigned int)(instr_info.m_opcode & 0xffff),
		    (unsigned int)((instr_info.m_opcode >> 16) & 0xffff),
		    (unsigned int)((instr_info.m_opcode >> 32) & 0xffff));
	  }else{
	    sprintf(str_2nd_data, "%04x %04x %04x %04x ", 
		    (unsigned int)(instr_info.m_opcode & 0xffff),
		    (unsigned int)((instr_info.m_opcode >> 16) & 0xffff),
		    (unsigned int)((instr_info.m_opcode >> 32) & 0xffff),
		    (unsigned int)((instr_info.m_opcode >> 48) & 0xffff));
	  }
	  sprintf(str_3rd_data, " %-30s", instr_info.m_disass.c_str());
	  tracer->m_trace_buf = str_pe_data;
	  tracer->m_trace_buf += str_1st_data;
	  tracer->m_trace_buf += str_2nd_data;
	  tracer->m_trace_buf += str_3rd_data;
	}
      }
    }
  
    /**
     * \brief Last trace callback operation.
     * \param tracer Pointer to self.
     */
    static void last_trace_callback(rh850g4::execution_tracer *tracer)
    {
      if (tracer->m_trace_regs)
      {
        tracer->trace_registers();
      }
      tracer->m_trace_regs = false;
    }

    /**
     * \brief Trace registers operation. Calls to this operation cause the
     * current CPU state to be displayed.
     */
    void trace_registers(void)
    {
      if(((FILE*)0 != m_fp_trace) && (m_core != NULL)){
        // Display PRs
        unsigned int cur_reg_val[32+1];
        unsigned long long cur_wreg_val[64];
        bool is_out_data = false;

        cur_reg_val[0]  = m_core->read_r0();
        cur_reg_val[1]  = m_core->read_r1();
        cur_reg_val[2]  = m_core->read_r2();
        cur_reg_val[3]  = m_core->read_r3();
        cur_reg_val[4]  = m_core->read_r4();
        cur_reg_val[5]  = m_core->read_r5();
        cur_reg_val[6]  = m_core->read_r6();
        cur_reg_val[7]  = m_core->read_r7();
        cur_reg_val[8]  = m_core->read_r8();
        cur_reg_val[9]  = m_core->read_r9();
        cur_reg_val[10] = m_core->read_r10();
        cur_reg_val[11] = m_core->read_r11();
        cur_reg_val[12] = m_core->read_r12();
        cur_reg_val[13] = m_core->read_r13();
        cur_reg_val[14] = m_core->read_r14();
        cur_reg_val[15] = m_core->read_r15();
        cur_reg_val[16] = m_core->read_r16();
        cur_reg_val[17] = m_core->read_r17();
        cur_reg_val[18] = m_core->read_r18();
        cur_reg_val[19] = m_core->read_r19();
        cur_reg_val[20] = m_core->read_r20();
        cur_reg_val[21] = m_core->read_r21();
        cur_reg_val[22] = m_core->read_r22();
        cur_reg_val[23] = m_core->read_r23();
        cur_reg_val[24] = m_core->read_r24();
        cur_reg_val[25] = m_core->read_r25();
        cur_reg_val[26] = m_core->read_r26();
        cur_reg_val[27] = m_core->read_r27();
        cur_reg_val[28] = m_core->read_r28();
        cur_reg_val[29] = m_core->read_r29();
        cur_reg_val[30] = m_core->read_r30();
        cur_reg_val[31] = m_core->read_r31();
        cur_reg_val[32] = m_core->read_PSW();
    
 	cur_wreg_val[0] = m_core->read_wr0();
	cur_wreg_val[1] = m_core->read_wr0_u();
	cur_wreg_val[2] = m_core->read_wr1();
	cur_wreg_val[3] = m_core->read_wr1_u();
	cur_wreg_val[4] = m_core->read_wr2();
	cur_wreg_val[5] = m_core->read_wr2_u();
	cur_wreg_val[6] = m_core->read_wr3();
	cur_wreg_val[7] = m_core->read_wr3_u();
	cur_wreg_val[8] = m_core->read_wr4();
	cur_wreg_val[9] = m_core->read_wr4_u();
	cur_wreg_val[10] = m_core->read_wr5();
	cur_wreg_val[11] = m_core->read_wr5_u();
	cur_wreg_val[12] = m_core->read_wr6();
	cur_wreg_val[13] = m_core->read_wr6_u();
	cur_wreg_val[14] = m_core->read_wr7();
	cur_wreg_val[15] = m_core->read_wr7_u();
	cur_wreg_val[16] = m_core->read_wr8();
	cur_wreg_val[17] = m_core->read_wr8_u();
	cur_wreg_val[18] = m_core->read_wr9();
	cur_wreg_val[19] = m_core->read_wr9_u();
	cur_wreg_val[20] = m_core->read_wr10();
	cur_wreg_val[21] = m_core->read_wr10_u();
	cur_wreg_val[22] = m_core->read_wr11();
	cur_wreg_val[23] = m_core->read_wr11_u();
	cur_wreg_val[24] = m_core->read_wr12();
	cur_wreg_val[25] = m_core->read_wr12_u();
	cur_wreg_val[26] = m_core->read_wr13();
	cur_wreg_val[27] = m_core->read_wr13_u();
	cur_wreg_val[28] = m_core->read_wr14();
	cur_wreg_val[29] = m_core->read_wr14_u();
	cur_wreg_val[30] = m_core->read_wr15();
	cur_wreg_val[31] = m_core->read_wr15_u();
	cur_wreg_val[32] = m_core->read_wr16();
	cur_wreg_val[33] = m_core->read_wr16_u();
	cur_wreg_val[34] = m_core->read_wr17();
	cur_wreg_val[35] = m_core->read_wr17_u();
	cur_wreg_val[36] = m_core->read_wr18();
	cur_wreg_val[37] = m_core->read_wr18_u();
	cur_wreg_val[38] = m_core->read_wr19();
	cur_wreg_val[39] = m_core->read_wr19_u();
	cur_wreg_val[40] = m_core->read_wr20();
	cur_wreg_val[41] = m_core->read_wr20_u();
	cur_wreg_val[42] = m_core->read_wr21();
	cur_wreg_val[43] = m_core->read_wr21_u();
	cur_wreg_val[44] = m_core->read_wr22();
	cur_wreg_val[45] = m_core->read_wr22_u();
	cur_wreg_val[46] = m_core->read_wr23();
	cur_wreg_val[47] = m_core->read_wr23_u();
	cur_wreg_val[48] = m_core->read_wr24();
	cur_wreg_val[49] = m_core->read_wr24_u(); 
	cur_wreg_val[50] = m_core->read_wr25();
	cur_wreg_val[51] = m_core->read_wr25_u();
	cur_wreg_val[52] = m_core->read_wr26();
	cur_wreg_val[53] = m_core->read_wr26_u();
	cur_wreg_val[54] = m_core->read_wr27();
	cur_wreg_val[55] = m_core->read_wr27_u();
	cur_wreg_val[56] = m_core->read_wr28();
	cur_wreg_val[57] = m_core->read_wr28_u();
	cur_wreg_val[58] = m_core->read_wr29();
	cur_wreg_val[59] = m_core->read_wr29_u();
	cur_wreg_val[60] = m_core->read_wr30();
	cur_wreg_val[61] = m_core->read_wr30_u();
	cur_wreg_val[62] = m_core->read_wr31();
	cur_wreg_val[63] = m_core->read_wr31_u();
	
	fprintf(m_fp_trace, "%s", m_trace_buf.c_str());

        // check global register's value
        for(int i=0; i<32; i++){
          if(m_greg_val[i] != cur_reg_val[i]){
	    if(is_out_data){
	      fprintf(m_fp_trace, ", ");
	    }else{
	      is_out_data = true;
	      fprintf(m_fp_trace, "\t");
	    }
	    fprintf(m_fp_trace, "r%d<-0x%08x", i, cur_reg_val[i]);
	    m_greg_val[i] = cur_reg_val[i];
          }
        }

        // check vecotr register's value
        for(int i=0; i<32; i++){
          if((m_wreg_val[i*2] != cur_wreg_val[i*2]) || 
	     (m_wreg_val[(i*2)+1] != cur_wreg_val[(i*2)+1])){
	    if(is_out_data){
	      fprintf(m_fp_trace, ", ");
	    }else{
	      is_out_data = true;
	      fprintf(m_fp_trace, "\t");
	    }
#if defined(_MSC_VER)
	    fprintf(m_fp_trace, "wr%d<-0x%016I64x_%016I64x", i, cur_wreg_val[(i*2)+1], cur_wreg_val[i*2]);
#else
	    fprintf(m_fp_trace, "wr%d<-0x%016llx_%016llx", i, cur_wreg_val[(i*2)+1], cur_wreg_val[i*2]);
#endif
	    m_wreg_val[i*2] = cur_wreg_val[i*2];
	    m_wreg_val[(i*2)+1] = cur_wreg_val[(i*2)+1];
          }
        }
        // check psw's value
        if(m_psw_val != cur_reg_val[32]){
          if(is_out_data){
	    fprintf(m_fp_trace, ", ");
          }else{
	    is_out_data = true;
	    fprintf(m_fp_trace, "\t");
          }
          fprintf(m_fp_trace, "psw<-0x%08x", cur_reg_val[32]);
          m_psw_val = cur_reg_val[32];
        }

        fprintf(m_fp_trace, "\n");
        fflush(m_fp_trace);
      }
    }
  
    /**
     * \brief Pointer to the core instance for which to trace execution, used
     * to be able to grab the program counter value. Set during attach()
     */
    rh850g4::core_base_t *m_core;

    /**
     * \brief PE's number (PEID).
     */     
    unsigned int m_pe_id;

    /**
     * \brief the trace file's pointer.
     */     
    FILE *m_fp_trace;

    /**
     * \brief the trace format.
     *     true : new format (Output CPU execution mode (HM or GMn with PEID)
     *     flase: old format (Not output CPU execution mode with PEID)
     */     
    bool m_is_old_format;

    /**
     * \brief the total count of executed instruction.
     */     
    unsigned long long m_total_inst_count;
    
    /**
     * \brief Member variable controlling whether register state should be traced.
     */
    bool m_trace_regs;

    /**
     * \brief Member variable copying the register's value.
     */
    unsigned int m_greg_val[32];
    unsigned int m_psw_val;
    unsigned long long m_wreg_val[64];

    /**
     * \brief Member variable buffering the trace data.
     */
    std::string m_trace_buf;
    
    /**
     * \brief Container for the instruction information stored by the fetch_predicate()
     * method when the core signifies that a valid instruction is being
     * translated. This information is then retrieved by the trace_callback() method
     * when invoked by executing a translated block.
     */
    std::map<unsigned int, rh850g4::execution_tracer::instr_info_t> m_instr_info;
    
    /**
     * \brief Descriptor referring to registered trace callback
     */
    rh850g4::analysis_callback_descriptor *m_trace_callback;
    
    /**
     * \brief Descriptor referring to registered last trace callback
     */
    rh850g4::analysis_callback_descriptor *m_last_trace_callback;
    
    /**
     * \brief Descriptor referring to post fetch predicate attachment
     */
    rh850g4::analysis_attachment_descriptor *m_post_fetch_attachment;
    
    /**
     * \brief Tag referring to post decode predicate attachment
     */
    rh850g4::analysis_attachment_descriptor *m_post_decode_attachment;
    
    /**
     * \brief Tag referring to post translate loop predicate attachment
     */
    rh850g4::analysis_attachment_descriptor *m_post_translate_loop_attachment;
  };
}

#endif // _RH850G4_EXECUTION_TRACER_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
