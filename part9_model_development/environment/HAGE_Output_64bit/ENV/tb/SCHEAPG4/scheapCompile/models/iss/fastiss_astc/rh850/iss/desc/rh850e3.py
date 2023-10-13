1#!/usr/bin/env python

#/*************************************************************************
# *
# * Copyright(c) 2019 Renesas Electronics Corporation
# * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
# * This program must be used solely for the purpose for which
# * it was furnished by Renesas Electronics Corporation. No part of this
# * program may be reproduced or disclosed to others, in any
# * form, without the prior written permission of Renesas Electronics
# * Corporation.
# *
# *************************************************************************/

#------------------------------------------------------------------------------
# Imports
#------------------------------------------------------------------------------

import sys
import types
import isa_compiler.populator.populator_impl

#------------------------------------------------------------------------------
# rh850 decription generator class
#------------------------------------------------------------------------------

class rh850_description_generator(isa_compiler.populator.populator_impl.populator_impl):

  # ---------------------------------------------------------------------------
  # Constructor
  # ---------------------------------------------------------------------------

  def __init__(self):
    # Call base class
    isa_compiler.populator.populator_impl.populator_impl.__init__(self)
    
    # Initialise internal data structures
    self.attribute_map = {}
    self.register_field_map = {}
    self.register_map = {}
    self.register_group_map = {}
    self.operand_list = []
    self.instruction_map = {}

  # ---------------------------------------------------------------------------
  # The populate() method is the top-level method that is called to populate
  # the specified an ISA description instance. It first creates the ISA
  # description instance and then makes a series of calls to populate it.
  # ---------------------------------------------------------------------------
  
  def populate(self, description):
    # Configure ISA description top-level features
    description.set_name(u"rh850")
    description.set_bit0(u"lsb")
    description.set_endianness(u"little")
    description.set_default_bus_width(32)

    # Populate ISA description
    self.generate_instruction_execution_phases(description)
    self.generate_instruction_attributes(description)
    self.generate_register_fields(description)
    self.generate_registers(description)
    self.generate_register_groups(description)
    self.generate_operands(description)
    self.generate_instructions(description)
    self.generate_instruction_sets(description)
    self.generate_bus_interfaces(description)
  
    # Reset internal data structures
    self.attribute_map = {}
    self.register_field_map = {}
    self.register_map = {}
    self.register_group_map = {}
    self.operand_list = []
    self.instruction_map = {}

  # ---------------------------------------------------------------------------
  # The generate_instruction_execution_phases() method populates the ISA
  # description with instruction execution phase description elements and
  # populates an internal map of instruction execution phases which may be used
  # to refer to a specific instruction attribute using an instruction execution
  # phase name. This operation iterates over a local list of instruction
  # execution phase names. Having parsed this list it identifies the execution
  # phases in which program counter and instruction count update occur.
  # ---------------------------------------------------------------------------
  
  def generate_instruction_execution_phases(self, description):
    execution_phase_map = {}
    execution_phase_info_list = \
    [
      ( u"decode", 2 ),
      ( u"execute", 1 )
    ]
    
    for (execution_phase_name, execution_phase_translation_pos) in execution_phase_info_list:
      execution_phase = description.create_execution_phase_description()
      execution_phase.set_name(execution_phase_name)
      execution_phase.set_translation_pos(execution_phase_translation_pos)
      execution_phase_map[execution_phase_name] = execution_phase
    description.set_fetch_phase(execution_phase_map["decode"])
    description.set_program_counter_update_phase(execution_phase_map["execute"])
    description.set_instruction_count_update_phase(execution_phase_map["execute"])

  # ---------------------------------------------------------------------------
  # The generate_instruction_attributes() method populates the ISA description
  # with instruction attribute description elements and populates an internal
  # map of instruction attributes which may be used to refer to a specific
  # instruction attribute using an instruction attribute name. This operation
  # iterates over a local list of attribute names. After parsing this list it
  # identifies the attributes that are used to mark instructions which terminate
  # block translation and the base cycle count of each instruction.
  # ---------------------------------------------------------------------------
  
  def generate_instruction_attributes(self, description):
    attribute_value_list = \
    [
      ( u"terminates_block", 1, False ),
      ( u"cycle_count", 32, 1 )
    ]
    
    for (attribute_name, attribute_size, attribute_default_value) in attribute_value_list:
      attribute = description.create_attribute_description()
      attribute.set_name(attribute_name)
      attribute.set_size(attribute_size)
      attribute.set_default_value(attribute_default_value)
      self.attribute_map[attribute_name] = attribute
    description.set_translation_termination_attribute(self.attribute_map["terminates_block"])
    description.set_cycle_count_attribute(self.attribute_map["cycle_count"])

  # ---------------------------------------------------------------------------
  # The generate_register_fields() method populates the ISA description with
  # register field description elements and populates an internal map of
  # register fields which may be used to refer to a specific register field
  # instance using a register field name. This operation iterates over a local
  # list of tuples describing the register field's name, start bit position
  # and end bit position.
  # ---------------------------------------------------------------------------
  
  def generate_register_fields(self, description):
    register_field_info_list = \
    [
      ( u"PSW_Z", 0, 0 ),
      ( u"PSW_S", 1, 1 ),
      ( u"PSW_OV", 2, 2 ),
      ( u"PSW_CY", 3, 3 ),
      ( u"PSW_SAT", 4, 4 ),
      ( u"PSW_ID", 5, 5 ),
      ( u"PSW_EP", 6, 6 ),
      ( u"PSW_NP", 7, 7 ),
      ( u"PSW_CM", 9, 9 ),
      ( u"PSW_SS", 11, 11 ),
      ( u"PSW_EBV", 15, 15 ),
      ( u"PSW_CU0", 16, 16 ),
      ( u"PSW_CU1", 17, 17 ),
      ( u"PSW_CU2", 18, 18 ),
      ( u"PSW_HVC", 19, 19 ),
      ( u"PSW_UM", 30, 30 ),
      ( u"FPSR_CC", 24, 31 ),
      ( u"FPSR_FN", 23, 23 ),
      ( u"FPSR_IF", 22, 22 ),
      ( u"FPSR_PEM", 21, 21 ),
      ( u"FPSR_RM", 18, 19 ),
      ( u"FPSR_FS", 17, 17 ),
      ( u"FPSR_XC", 10, 15 ),
      ( u"FPSR_XE", 5, 9 ),
      ( u"FPSR_XP", 0, 4 ),
      ( u"FPST_XC", 8, 13 ),
      ( u"FPST_IF", 5, 5 ),
      ( u"FPST_XP", 0, 4 ),
      ( u"FPCC_CC", 0, 7 ),
      ( u"FPCFG_RM", 8, 9 ),
      ( u"FPCFG_XE", 0, 4 ),
      ( u"MCFG0_SPID", 16, 23 ),
      ( u"MCFG0_HVP", 2, 2 ),
      ( u"RBASE_RBASE", 9, 31 ),
      ( u"RBASE_RINT", 0, 0 ),
      ( u"EBASE_EBASE", 9, 31 ),
      ( u"EBASE_RINT", 0, 0 ),
      ( u"MCTL_EN", 31, 31 ),
      ( u"MCTL_MT", 30, 30 ),
      ( u"MCTL_STID", 16, 21 ),
      ( u"MCTL_MA", 1, 1 ),
      ( u"MCTL_UIC", 0, 0 ),
      ( u"FPIPR_FPIPR", 0, 4 ),
      ( u"SCCFG_SIZE", 0, 7 ),
      ( u"HTCFG0_PEID", 16, 18 ),
      ( u"HTCFG0_NC", 15, 15 ),
      ( u"HTCFG0_TCID", 0, 7 ),
      ( u"ASID_ASID", 0, 9 ),
      ( u"MEI_REG", 16, 20 ),
      ( u"MEI_DS", 9, 10 ),
      ( u"MEI_U", 8, 8 ),
      ( u"MEI_ITYPE", 1, 5 ),
      ( u"MEI_RW", 0, 0 ),
      ( u"ISPR_ISP", 0, 7 ),
      ( u"PMR_PM", 0, 7 ),
      ( u"ICSR_PMEI", 0, 0 ),
      ( u"INTCFG_ISPC", 0, 0 ),
      ( u"MPM_DX", 10, 10 ),
      ( u"MPM_DW", 9, 9 ),
      ( u"MPM_DR", 8, 8 ),
      ( u"MPM_SVP", 1, 1 ),
      ( u"MPM_MPE", 0, 0 ),
      ( u"MPRC_E15", 15, 15 ),
      ( u"MPRC_E14", 14, 14 ),
      ( u"MPRC_E13", 13, 13 ),
      ( u"MPRC_E12", 12, 12 ),
      ( u"MPRC_E11", 11, 11 ),
      ( u"MPRC_E10", 10, 10 ),
      ( u"MPRC_E9", 9, 9 ),
      ( u"MPRC_E8", 8, 8 ),
      ( u"MPRC_E7", 7, 7 ),
      ( u"MPRC_E6", 6, 6 ),
      ( u"MPRC_E5", 5, 5 ),
      ( u"MPRC_E4", 4, 4 ),
      ( u"MPRC_E3", 3, 3 ),
      ( u"MPRC_E2", 2, 2 ),
      ( u"MPRC_E1", 1, 1 ),
      ( u"MPRC_E0", 0, 0 ),
      ( u"MCR_SXE", 5, 5 ),
      ( u"MCR_SWE", 4, 4 ),
      ( u"MCR_SRE", 3, 3 ),
      ( u"MCR_UXE", 2, 2 ),
      ( u"MCR_UWE", 1, 1 ),
      ( u"MCR_URE", 0, 0 ),
      ( u"MPBRGN_MPBRGN", 0, 4 ),
      ( u"MPTRGN_MPTRGN", 0, 4 ),
      ( u"MPAT0_ASID", 16, 25 ),
      ( u"MPAT0_E", 7, 7 ),
      ( u"MPAT0_G", 6, 6 ),
      ( u"MPAT0_SX", 5, 5 ),
      ( u"MPAT0_SW", 4, 4 ),
      ( u"MPAT0_SR", 3, 3 ),
      ( u"MPAT0_UX", 2, 2 ),
      ( u"MPAT0_UW", 1, 1 ),
      ( u"MPAT0_UR", 0, 0 ),
      ( u"MPAT1_ASID", 16, 25 ),
      ( u"MPAT1_E", 7, 7 ),
      ( u"MPAT1_G", 6, 6 ),
      ( u"MPAT1_SX", 5, 5 ),
      ( u"MPAT1_SW", 4, 4 ),
      ( u"MPAT1_SR", 3, 3 ),
      ( u"MPAT1_UX", 2, 2 ),
      ( u"MPAT1_UW", 1, 1 ),
      ( u"MPAT1_UR", 0, 0 ),
      ( u"MPAT2_ASID", 16, 25 ),
      ( u"MPAT2_E", 7, 7 ),
      ( u"MPAT2_G", 6, 6 ),
      ( u"MPAT2_SX", 5, 5 ),
      ( u"MPAT2_SW", 4, 4 ),
      ( u"MPAT2_SR", 3, 3 ),
      ( u"MPAT2_UX", 2, 2 ),
      ( u"MPAT2_UW", 1, 1 ),
      ( u"MPAT2_UR", 0, 0 ),
      ( u"MPAT3_ASID", 16, 25 ),
      ( u"MPAT3_E", 7, 7 ),
      ( u"MPAT3_G", 6, 6 ),
      ( u"MPAT3_SX", 5, 5 ),
      ( u"MPAT3_SW", 4, 4 ),
      ( u"MPAT3_SR", 3, 3 ),
      ( u"MPAT3_UX", 2, 2 ),
      ( u"MPAT3_UW", 1, 1 ),
      ( u"MPAT3_UR", 0, 0 ),
      ( u"MPAT4_ASID", 16, 25 ),
      ( u"MPAT4_E", 7, 7 ),
      ( u"MPAT4_G", 6, 6 ),
      ( u"MPAT4_SX", 5, 5 ),
      ( u"MPAT4_SW", 4, 4 ),
      ( u"MPAT4_SR", 3, 3 ),
      ( u"MPAT4_UX", 2, 2 ),
      ( u"MPAT4_UW", 1, 1 ),
      ( u"MPAT4_UR", 0, 0 ),
      ( u"MPAT5_ASID", 16, 25 ),
      ( u"MPAT5_E", 7, 7 ),
      ( u"MPAT5_G", 6, 6 ),
      ( u"MPAT5_SX", 5, 5 ),
      ( u"MPAT5_SW", 4, 4 ),
      ( u"MPAT5_SR", 3, 3 ),
      ( u"MPAT5_UX", 2, 2 ),
      ( u"MPAT5_UW", 1, 1 ),
      ( u"MPAT5_UR", 0, 0 ),
      ( u"MPAT6_ASID", 16, 25 ),
      ( u"MPAT6_E", 7, 7 ),
      ( u"MPAT6_G", 6, 6 ),
      ( u"MPAT6_SX", 5, 5 ),
      ( u"MPAT6_SW", 4, 4 ),
      ( u"MPAT6_SR", 3, 3 ),
      ( u"MPAT6_UX", 2, 2 ),
      ( u"MPAT6_UW", 1, 1 ),
      ( u"MPAT6_UR", 0, 0 ),
      ( u"MPAT7_ASID", 16, 25 ),
      ( u"MPAT7_E", 7, 7 ),
      ( u"MPAT7_G", 6, 6 ),
      ( u"MPAT7_SX", 5, 5 ),
      ( u"MPAT7_SW", 4, 4 ),
      ( u"MPAT7_SR", 3, 3 ),
      ( u"MPAT7_UX", 2, 2 ),
      ( u"MPAT7_UW", 1, 1 ),
      ( u"MPAT7_UR", 0, 0 ),
      ( u"MPAT8_ASID", 16, 25 ),
      ( u"MPAT8_E", 7, 7 ),
      ( u"MPAT8_G", 6, 6 ),
      ( u"MPAT8_SX", 5, 5 ),
      ( u"MPAT8_SW", 4, 4 ),
      ( u"MPAT8_SR", 3, 3 ),
      ( u"MPAT8_UX", 2, 2 ),
      ( u"MPAT8_UW", 1, 1 ),
      ( u"MPAT8_UR", 0, 0 ),
      ( u"MPAT9_ASID", 16, 25 ),
      ( u"MPAT9_E", 7, 7 ),
      ( u"MPAT9_G", 6, 6 ),
      ( u"MPAT9_SX", 5, 5 ),
      ( u"MPAT9_SW", 4, 4 ),
      ( u"MPAT9_SR", 3, 3 ),
      ( u"MPAT9_UX", 2, 2 ),
      ( u"MPAT9_UW", 1, 1 ),
      ( u"MPAT9_UR", 0, 0 ),
      ( u"MPAT10_ASID", 16, 25 ),
      ( u"MPAT10_E", 7, 7 ),
      ( u"MPAT10_G", 6, 6 ),
      ( u"MPAT10_SX", 5, 5 ),
      ( u"MPAT10_SW", 4, 4 ),
      ( u"MPAT10_SR", 3, 3 ),
      ( u"MPAT10_UX", 2, 2 ),
      ( u"MPAT10_UW", 1, 1 ),
      ( u"MPAT10_UR", 0, 0 ),
      ( u"MPAT11_ASID", 16, 25 ),
      ( u"MPAT11_E", 7, 7 ),
      ( u"MPAT11_G", 6, 6 ),
      ( u"MPAT11_SX", 5, 5 ),
      ( u"MPAT11_SW", 4, 4 ),
      ( u"MPAT11_SR", 3, 3 ),
      ( u"MPAT11_UX", 2, 2 ),
      ( u"MPAT11_UW", 1, 1 ),
      ( u"MPAT11_UR", 0, 0 ),
      ( u"MPAT12_ASID", 16, 25 ),
      ( u"MPAT12_E", 7, 7 ),
      ( u"MPAT12_G", 6, 6 ),
      ( u"MPAT12_SX", 5, 5 ),
      ( u"MPAT12_SW", 4, 4 ),
      ( u"MPAT12_SR", 3, 3 ),
      ( u"MPAT12_UX", 2, 2 ),
      ( u"MPAT12_UW", 1, 1 ),
      ( u"MPAT12_UR", 0, 0 ),
      ( u"MPAT13_ASID", 16, 25 ),
      ( u"MPAT13_E", 7, 7 ),
      ( u"MPAT13_G", 6, 6 ),
      ( u"MPAT13_SX", 5, 5 ),
      ( u"MPAT13_SW", 4, 4 ),
      ( u"MPAT13_SR", 3, 3 ),
      ( u"MPAT13_UX", 2, 2 ),
      ( u"MPAT13_UW", 1, 1 ),
      ( u"MPAT13_UR", 0, 0 ),
      ( u"MPAT14_ASID", 16, 25 ),
      ( u"MPAT14_E", 7, 7 ),
      ( u"MPAT14_G", 6, 6 ),
      ( u"MPAT14_SX", 5, 5 ),
      ( u"MPAT14_SW", 4, 4 ),
      ( u"MPAT14_SR", 3, 3 ),
      ( u"MPAT14_UX", 2, 2 ),
      ( u"MPAT14_UW", 1, 1 ),
      ( u"MPAT14_UR", 0, 0 ),
      ( u"MPAT15_ASID", 16, 25 ),
      ( u"MPAT15_E", 7, 7 ),
      ( u"MPAT15_G", 6, 6 ),
      ( u"MPAT15_SX", 5, 5 ),
      ( u"MPAT15_SW", 4, 4 ),
      ( u"MPAT15_SR", 3, 3 ),
      ( u"MPAT15_UX", 2, 2 ),
      ( u"MPAT15_UW", 1, 1 ),
      ( u"MPAT15_UR", 0, 0 )
    ]
    
    for (name, start_pos, end_pos) in register_field_info_list:
      register_field = description.create_register_field_description()
      register_field.set_name(name)
      register_field.set_start_pos(start_pos)
      register_field.set_end_pos(end_pos)
      self.register_field_map[name] = register_field

  # ---------------------------------------------------------------------------
  # The process_register_info() method populates the ISA description with
  # register description elements given an input list of register descriptions
  # which capture each registers name and optionally the register's sub-fields,
  # and size. Note that if the register sub-fields are not specified for any
  # given register then the register is assumed to have no sub-fields. If the
  # register size if not specified then the default_size argument is used.
  # Note that register sub-fields are referred to by name (since register field
  # names should be unique).
  # ---------------------------------------------------------------------------

  def process_register_info(self, description, register_info_list):
    for register_info in register_info_list:
      (name, register_field_name_list, size) = register_info
      register = description.create_register_description()
      register.set_name(name)
      register.set_size(size)
      if (register_field_name_list != None):
        for register_field_name in register_field_name_list:
          register.add_register_field(self.register_field_map[register_field_name])
      self.register_map[name] = register
  
  # ---------------------------------------------------------------------------
  # The generate_registers() method populates the ISA description with
  # register description elements and populates an internal map of registers
  # which may be used to refer to a specific register instance using a register
  # name. This operation passes lists of register description information
  # to the process_register_info() method which in turn performs the actual
  # population of the ISA description.
  # ---------------------------------------------------------------------------

  def generate_registers(self, description):
    register_info_list = \
    [
      ( u"r0", None, 32 ),
      ( u"r1", None, 32 ),
      ( u"r2", None, 32 ),
      ( u"r3", None, 32 ),
      ( u"r4", None, 32 ),
      ( u"r5", None, 32 ),
      ( u"r6", None, 32 ),
      ( u"r7", None, 32 ),
      ( u"r8", None, 32 ),
      ( u"r9", None, 32 ),
      ( u"r10", None, 32 ),
      ( u"r11", None, 32 ),
      ( u"r12", None, 32 ),
      ( u"r13", None, 32 ),
      ( u"r14", None, 32 ),
      ( u"r15", None, 32 ),
      ( u"r16", None, 32 ),
      ( u"r17", None, 32 ),
      ( u"r18", None, 32 ),
      ( u"r19", None, 32 ),
      ( u"r20", None, 32 ),
      ( u"r21", None, 32 ),
      ( u"r22", None, 32 ),
      ( u"r23", None, 32 ),
      ( u"r24", None, 32 ),
      ( u"r25", None, 32 ),
      ( u"r26", None, 32 ),
      ( u"r27", None, 32 ),
      ( u"r28", None, 32 ),
      ( u"r29", None, 32 ),
      ( u"r30", None, 32 ),
      ( u"r31", None, 32 ),
      ( u"PC", None, 32 ),
      ( u"EIPC", None, 32 ),
      ( u"EIPSW", None, 32 ),
      ( u"FEPC", None, 32 ),
      ( u"FEPSW", None, 32 ),
      (
        u"PSW",
        [
          u"PSW_Z",
          u"PSW_S",
          u"PSW_OV",
          u"PSW_CY",
          u"PSW_SAT",
          u"PSW_ID",
          u"PSW_EP",
          u"PSW_NP",
          u"PSW_CM",
          u"PSW_SS",
          u"PSW_EBV", 
          u"PSW_CU0", 
          u"PSW_CU1", 
          u"PSW_CU2", 
          u"PSW_HVC", 
          u"PSW_UM"
        ],
        32
      ),
      (
        u"FPSR",
        [
          u"FPSR_CC",
          u"FPSR_FN",
          u"FPSR_IF",
          u"FPSR_PEM",
          u"FPSR_RM",
          u"FPSR_FS",
          u"FPSR_XC",
          u"FPSR_XE",
          u"FPSR_XP"
        ],
        32
      ),
      ( u"FPEPC", None, 32 ),
      (
        u"FPST",
        [
          u"FPST_XC",
          u"FPST_IF",
          u"FPST_XP"
        ],
        32
      ),
      (
        u"FPCC",
        [
          u"FPCC_CC"
        ],
        32
      ),
      (
        u"FPCFG",
        [
          u"FPCFG_RM",
          u"FPCFG_XE"
        ],
        32
      ),
      ( u"FPEC", None, 32 ),
      ( u"EIIC", None, 32 ),
      ( u"FEIC", None, 32 ),
      ( u"CTPC", None, 32 ),
      ( u"CTPSW", None, 32 ),
      ( u"CTBP", None, 32 ),
      ( u"EIWR", None, 32 ),
      ( u"FEWR", None, 32 ),
      ( u"BSEL", None, 32 ),
      (
        u"MCFG0",
        [
          u"MCFG0_SPID",
          u"MCFG0_HVP"
        ],
        32
      ),
      (
        u"RBASE",
        [
          u"RBASE_RBASE",
          u"RBASE_RINT"
        ],
        32
      ),
      (
        u"EBASE",
        [
          u"EBASE_EBASE",
          u"EBASE_RINT"
        ],
        32
      ),
      ( u"INTBP", None, 32 ),
      (
        u"MCTL",
        [
          u"MCTL_EN",
          u"MCTL_MT",
          u"MCTL_STID",
          u"MCTL_MA",
          u"MCTL_UIC"
        ],
        32
      ),
      ( u"PID", None, 32 ),
      (
        u"FPIPR",
        [
          u"FPIPR_FPIPR"
        ],
        32
      ),
      (
        u"SCCFG",
        [
          u"SCCFG_SIZE"
        ],
        32
      ),
      ( u"SCBP", None, 32 ),
      (
        u"HTCFG0",
        [
          u"HTCFG0_PEID",
          u"HTCFG0_NC",
          u"HTCFG0_TCID"
        ],
        32
      ),
      ( u"HVCCFG", None, 32 ),
      ( u"HVCBP", None, 32 ),
      ( u"MEA", None, 32 ),
      (
        u"ASID",
        [
          u"ASID_ASID"
        ],
        32
      ),
      (
        u"MEI",
        [
          u"MEI_REG",
          u"MEI_DS",
          u"MEI_U",
          u"MEI_ITYPE",
          u"MEI_RW"
        ],
        32
      ),
      (
        u"ISPR",
        [
          u"ISPR_ISP"
        ],
        32
      ),
      (
        u"PMR",
        [
          u"PMR_PM"
        ],
        32
      ),
      (
        u"ICSR",
        [
          u"ICSR_PMEI"
        ],
        32
      ),
      (
        u"INTCFG",
        [
          u"INTCFG_ISPC"
        ],
        32
      ),
      ( u"DBIC", None, 32 ),
      ( u"DBCMC", None, 32 ),
      ( u"DBPC", None, 32 ),
      ( u"DBPSW", None, 32 ),
      ( u"DIR0", None, 32 ),
      ( u"DIR1", None, 32 ),
      ( u"BPC", None, 32 ),
      ( u"BPAV", None, 32 ),
      ( u"BPAM", None, 32 ),
      ( u"BPDV", None, 32 ),
      ( u"BPDM", None, 32 ),
      ( u"DBWR", None, 32 ),
      (
        u"MPM",
        [
          u"MPM_DX",
          u"MPM_DW",
          u"MPM_DR",
          u"MPM_SVP",
          u"MPM_MPE"
        ],
        32
      ),
      (
        u"MPRC",
        [
          u"MPRC_E15",
          u"MPRC_E14",
          u"MPRC_E13",
          u"MPRC_E12",
          u"MPRC_E11",
          u"MPRC_E10",
          u"MPRC_E9",
          u"MPRC_E8",
          u"MPRC_E7",
          u"MPRC_E6",
          u"MPRC_E5",
          u"MPRC_E4",
          u"MPRC_E3",
          u"MPRC_E2",
          u"MPRC_E1",
          u"MPRC_E0"
        ],
        32
      ),
      (
        u"MPBRGN",
        [
          u"MPBRGN_MPBRGN"
        ],
        32
      ),
      (
        u"MPTRGN",
        [
          u"MPTRGN_MPTRGN"
        ],
        32
      ),
      (
        u"MCR",
        [
          u"MCR_SXE",
          u"MCR_SWE",
          u"MCR_SRE",
          u"MCR_UXE",
          u"MCR_UWE",
          u"MCR_URE"
        ],
        32
      ),
      ( u"MPLA0", None, 32 ),
      ( u"MPUA0", None, 32 ),
      (
        u"MPAT0",
        [
          u"MPAT0_ASID",
          u"MPAT0_E",
          u"MPAT0_G",
          u"MPAT0_SX",
          u"MPAT0_SW",
          u"MPAT0_SR",
          u"MPAT0_UX",
          u"MPAT0_UW",
          u"MPAT0_UR"
        ],
        32
      ),
      ( u"MPLA1", None, 32 ),
      ( u"MPUA1", None, 32 ),
      (
        u"MPAT1",
        [
          u"MPAT1_ASID",
          u"MPAT1_E",
          u"MPAT1_G",
          u"MPAT1_SX",
          u"MPAT1_SW",
          u"MPAT1_SR",
          u"MPAT1_UX",
          u"MPAT1_UW",
          u"MPAT1_UR"
        ],
        32
      ),
      ( u"MPLA2", None, 32 ),
      ( u"MPUA2", None, 32 ),
      (
        u"MPAT2",
        [
          u"MPAT2_ASID",
          u"MPAT2_E",
          u"MPAT2_G",
          u"MPAT2_SX",
          u"MPAT2_SW",
          u"MPAT2_SR",
          u"MPAT2_UX",
          u"MPAT2_UW",
          u"MPAT2_UR"
        ],
        32
      ),
      ( u"MPLA3", None, 32 ),
      ( u"MPUA3", None, 32 ),
      (
        u"MPAT3",
        [
          u"MPAT3_ASID",
          u"MPAT3_E",
          u"MPAT3_G",
          u"MPAT3_SX",
          u"MPAT3_SW",
          u"MPAT3_SR",
          u"MPAT3_UX",
          u"MPAT3_UW",
          u"MPAT3_UR"
        ],
        32
      ),
      ( u"MPLA4", None, 32 ),
      ( u"MPUA4", None, 32 ),
      (
        u"MPAT4",
        [
          u"MPAT4_ASID",
          u"MPAT4_E",
          u"MPAT4_G",
          u"MPAT4_SX",
          u"MPAT4_SW",
          u"MPAT4_SR",
          u"MPAT4_UX",
          u"MPAT4_UW",
          u"MPAT4_UR"
        ],
        32
      ),
      ( u"MPLA5", None, 32 ),
      ( u"MPUA5", None, 32 ),
      (
        u"MPAT5",
        [
          u"MPAT5_ASID",
          u"MPAT5_E",
          u"MPAT5_G",
          u"MPAT5_SX",
          u"MPAT5_SW",
          u"MPAT5_SR",
          u"MPAT5_UX",
          u"MPAT5_UW",
          u"MPAT5_UR"
        ],
        32
      ),
      ( u"MPLA6", None, 32 ),
      ( u"MPUA6", None, 32 ),
      (
        u"MPAT6",
        [
          u"MPAT6_ASID",
          u"MPAT6_E",
          u"MPAT6_G",
          u"MPAT6_SX",
          u"MPAT6_SW",
          u"MPAT6_SR",
          u"MPAT6_UX",
          u"MPAT6_UW",
          u"MPAT6_UR"
        ],
        32
      ),
      ( u"MPLA7", None, 32 ),
      ( u"MPUA7", None, 32 ),
      (
        u"MPAT7",
        [
          u"MPAT7_ASID",
          u"MPAT7_E",
          u"MPAT7_G",
          u"MPAT7_SX",
          u"MPAT7_SW",
          u"MPAT7_SR",
          u"MPAT7_UX",
          u"MPAT7_UW",
          u"MPAT7_UR"
        ],
        32
      ),
      ( u"MPLA8", None, 32 ),
      ( u"MPUA8", None, 32 ),
      (
        u"MPAT8",
        [
          u"MPAT8_ASID",
          u"MPAT8_E",
          u"MPAT8_G",
          u"MPAT8_SX",
          u"MPAT8_SW",
          u"MPAT8_SR",
          u"MPAT8_UX",
          u"MPAT8_UW",
          u"MPAT8_UR"
        ],
        32
      ),
      ( u"MPLA9", None, 32 ),
      ( u"MPUA9", None, 32 ),
      (
        u"MPAT9",
        [
          u"MPAT9_ASID",
          u"MPAT9_E",
          u"MPAT9_G",
          u"MPAT9_SX",
          u"MPAT9_SW",
          u"MPAT9_SR",
          u"MPAT9_UX",
          u"MPAT9_UW",
          u"MPAT9_UR"
        ],
        32
      ),
      ( u"MPLA10", None, 32 ),
      ( u"MPUA10", None, 32 ),
      (
        u"MPAT10",
        [
          u"MPAT10_ASID",
          u"MPAT10_E",
          u"MPAT10_G",
          u"MPAT10_SX",
          u"MPAT10_SW",
          u"MPAT10_SR",
          u"MPAT10_UX",
          u"MPAT10_UW",
          u"MPAT10_UR"
        ],
        32
      ),
      ( u"MPLA11", None, 32 ),
      ( u"MPUA11", None, 32 ),
      (
        u"MPAT11",
        [
          u"MPAT11_ASID",
          u"MPAT11_E",
          u"MPAT11_G",
          u"MPAT11_SX",
          u"MPAT11_SW",
          u"MPAT11_SR",
          u"MPAT11_UX",
          u"MPAT11_UW",
          u"MPAT11_UR"
        ],
        32
      ),
      ( u"MPLA12", None, 32 ),
      ( u"MPUA12", None, 32 ),
      (
        u"MPAT12",
        [
          u"MPAT12_ASID",
          u"MPAT12_E",
          u"MPAT12_G",
          u"MPAT12_SX",
          u"MPAT12_SW",
          u"MPAT12_SR",
          u"MPAT12_UX",
          u"MPAT12_UW",
          u"MPAT12_UR"
        ],
        32
      ),
      ( u"MPLA13", None, 32 ),
      ( u"MPUA13", None, 32 ),
      (
        u"MPAT13",
        [
          u"MPAT13_ASID",
          u"MPAT13_E",
          u"MPAT13_G",
          u"MPAT13_SX",
          u"MPAT13_SW",
          u"MPAT13_SR",
          u"MPAT13_UX",
          u"MPAT13_UW",
          u"MPAT13_UR"
        ],
        32
      ),
      ( u"MPLA14", None, 32 ),
      ( u"MPUA14", None, 32 ),
      (
        u"MPAT14",
        [
          u"MPAT14_ASID",
          u"MPAT14_E",
          u"MPAT14_G",
          u"MPAT14_SX",
          u"MPAT14_SW",
          u"MPAT14_SR",
          u"MPAT14_UX",
          u"MPAT14_UW",
          u"MPAT14_UR"
        ],
        32
      ),
      ( u"MPLA15", None, 32 ),
      ( u"MPUA15", None, 32 ),
      (
        u"MPAT15",
        [
          u"MPAT15_ASID",
          u"MPAT15_E",
          u"MPAT15_G",
          u"MPAT15_SX",
          u"MPAT15_SW",
          u"MPAT15_SR",
          u"MPAT15_UX",
          u"MPAT15_UW",
          u"MPAT15_UR"
        ],
        32
      ),
      ( u"mpu_entry", None, 8 ),
      ( u"mpat_g_support", None, 8 )
    ]
    self.process_register_info(description, register_info_list)
    description.set_program_counter(self.register_map[u"PC"])
    for extra_register_name in [u"swd0pcad_exec_count", u"swd0pcad_val", u"mem_acc_type"]:
      register = description.create_register_description()
      register.set_name(extra_register_name)
      register.set_size(32)
      self.register_map[extra_register_name] = register

  # ---------------------------------------------------------------------------
  # The generate_register_groups() method populates the ISA description with
  # register group description elements and populates an internal map of
  # register groups which may be used to refer to a specific register group
  # instance using a register group name. This operation iterates over a local
  # list of tuples describing the register group's name and member registers.
  # Note that registers are referred to by name (since register names should be
  # unique).
  # ---------------------------------------------------------------------------
  
  def generate_register_groups(self, description):
    register_group_info_list = \
    [
      (
        u"GR",
        [
          ( 0, u"r0" ),
          ( 1, u"r1" ),
          ( 2, u"r2" ),
          ( 3, u"r3" ),
          ( 4, u"r4" ),
          ( 5, u"r5" ),
          ( 6, u"r6" ),
          ( 7, u"r7" ),
          ( 8, u"r8" ),
          ( 9, u"r9" ),
          ( 10, u"r10" ),
          ( 11, u"r11" ),
          ( 12, u"r12" ),
          ( 13, u"r13" ),
          ( 14, u"r14" ),
          ( 15, u"r15" ),
          ( 16, u"r16" ),
          ( 17, u"r17" ),
          ( 18, u"r18" ),
          ( 19, u"r19" ),
          ( 20, u"r20" ),
          ( 21, u"r21" ),
          ( 22, u"r22" ),
          ( 23, u"r23" ),
          ( 24, u"r24" ),
          ( 25, u"r25" ),
          ( 26, u"r26" ),
          ( 27, u"r27" ),
          ( 28, u"r28" ),
          ( 29, u"r29" ),
          ( 30, u"r30" ),
          ( 31, u"r31" )
        ]
      ),
      (
        u"SR_SELID_0",
        [
          ( 0, u"EIPC" ),
          ( 1, u"EIPSW" ),
          ( 2, u"FEPC" ),
          ( 3, u"FEPSW" ),
          ( 5, u"PSW" ),
          ( 6, u"FPSR" ),
          ( 7, u"FPEPC" ),
          ( 8, u"FPST" ),
          ( 9, u"FPCC" ),
          ( 10, u"FPCFG" ),
          ( 11, u"FPEC" ),
          ( 13, u"EIIC" ),
          ( 14, u"FEIC" ),
          ( 16, u"CTPC" ),
          ( 17, u"CTPSW" ),
          ( 20, u"CTBP" ),
          ( 28, u"EIWR" ),
          ( 29, u"FEWR" ),
          ( 31, u"BSEL" )
        ]
      ),
      (
        u"SR_SELID_1",
        [
          ( 0, u"MCFG0" ),
          ( 2, u"RBASE" ),
          ( 3, u"EBASE" ),
          ( 4, u"INTBP" ),
          ( 5, u"MCTL" ),
          ( 6, u"PID" ),
          ( 7, u"FPIPR" ),
          ( 11, u"SCCFG" ),
          ( 12, u"SCBP" ),
          ( 13, u"HVCCFG" ),
          ( 14, u"HVCBP" )
        ]
      ),
      (
        u"SR_SELID_2",
        [
          ( 0, u"HTCFG0" ),
          ( 6, u"MEA" ),
          ( 7, u"ASID" ),
          ( 8, u"MEI" ),
          ( 10, u"ISPR" ),
          ( 11, u"PMR" ),
          ( 12, u"ICSR" ),
          ( 13, u"INTCFG" )
        ]
      ),
      (
        u"SR_SELID_3",
        [
          ( 15, u"DBIC" ),
          ( 17, u"DBCMC" ),
          ( 18, u"DBPC" ),
          ( 19, u"DBPSW" ),
          ( 20, u"DIR0" ),
          ( 21, u"DIR1" ),
          ( 22, u"BPC" ),
          ( 24, u"BPAV" ),
          ( 25, u"BPAM" ),
          ( 26, u"BPDV" ),
          ( 27, u"BPDM" ),
          ( 30, u"DBWR" )
        ]
      ),
      (
        u"SR_SELID_5",
        [
          ( 0, u"MPM" ),
          ( 1, u"MPRC" ),
          ( 4, u"MPBRGN" ),
          ( 5, u"MPTRGN" ),
          ( 11, u"MCR" )
        ]
      ),
      (
        u"SR_SELID_6",
        [
          ( 0, u"MPLA0" ),
          ( 1, u"MPUA0" ),
          ( 2, u"MPAT0" ),
          ( 4, u"MPLA1" ),
          ( 5, u"MPUA1" ),
          ( 6, u"MPAT1" ),
          ( 8, u"MPLA2" ),
          ( 9, u"MPUA2" ),
          ( 10, u"MPAT2" ),
          ( 12, u"MPLA3" ),
          ( 13, u"MPUA3" ),
          ( 14, u"MPAT3" ),
          ( 16, u"MPLA4" ),
          ( 17, u"MPUA4" ),
          ( 18, u"MPAT4" ),
          ( 20, u"MPLA5" ),
          ( 21, u"MPUA5" ),
          ( 22, u"MPAT5" ),
          ( 24, u"MPLA6" ),
          ( 25, u"MPUA6" ),
          ( 26, u"MPAT6" ),
          ( 28, u"MPLA7" ),
          ( 29, u"MPUA7" ),
          ( 30, u"MPAT7" )
        ]
      ),
      (
        u"SR_SELID_7",
        [
          ( 0, u"MPLA8" ),
          ( 1, u"MPUA8" ),
          ( 2, u"MPAT8" ),
          ( 4, u"MPLA9" ),
          ( 5, u"MPUA9" ),
          ( 6, u"MPAT9" ),
          ( 8, u"MPLA10" ),
          ( 9, u"MPUA10" ),
          ( 10, u"MPAT10" ),
          ( 12, u"MPLA11" ),
          ( 13, u"MPUA11" ),
          ( 14, u"MPAT11" ),
          ( 16, u"MPLA12" ),
          ( 17, u"MPUA12" ),
          ( 18, u"MPAT12" ),
          ( 20, u"MPLA13" ),
          ( 21, u"MPUA13" ),
          ( 22, u"MPAT13" ),
          ( 24, u"MPLA14" ),
          ( 25, u"MPUA14" ),
          ( 26, u"MPAT14" ),
          ( 28, u"MPLA15" ),
          ( 29, u"MPUA15" ),
          ( 30, u"MPAT15" )
        ]
      )
    ]
    
    for (name, register_tag_name_list) in register_group_info_list:
      register_group = description.create_register_group_description()
      register_group.set_name(name)
      for (register_tag, register_name) in register_tag_name_list:
        register_group.add_register(register_tag, self.register_map[register_name])
      self.register_group_map[name] = register_group

  # ---------------------------------------------------------------------------
  # The generate_operands() method populates the ISA description with operand
  # description elements and populates an internal list of operands which may
  # be used to refer to a specific operand instance using an index value. This
  # operation iterates over a local list of tuples describing the operand's
  # name, storage size in bits, any sign extension and left shift requirements,
  # which register group if any it is used to index, and the list of sub-fields
  # of which it is comprised. Note that register groups are referred to by name
  # (since register group names should be unique), however operand sub-fields
  # are referred to by an index to the operand_field_list (since there is no
  # unique key value associated with an operand field).
  # ---------------------------------------------------------------------------
  
  def generate_operands(self, description):
    operand_info_list = \
    [
      ( u"bit", 8, 0, 0, None, [ ( 11, 13 ) ] ),
      ( u"bits_ff", 8, 0, 0, None, [ ( 19, 20 ) ] ),
      ( u"cccc", 8, 0, 0, None, [ ( 0, 3 ) ] ),
      ( u"cccc", 8, 0, 0, None, [ ( 17, 20 ) ] ),
      ( u"fcbit", 8, 0, 0, None, [ ( 17, 19 ) ] ),
      ( u"fcond", 8, 0, 0, None, [ ( 27, 30 ) ] ),
      ( u"disp4", 32, 0, 0, None, [ ( 0, 3 ) ] ),
      ( u"disp5", 32, 0, 1, None, [ ( 0, 3 ) ] ),
      ( u"disp7", 32, 0, 0, None, [ ( 0, 6 ) ] ),
      ( u"disp8", 32, 0, 1, None, [ ( 0, 6 ) ] ),
      ( u"disp8", 32, 0, 2, None, [ ( 1, 6 ) ] ),
      ( u"disp9", 32, 1, 1, None, [ ( 11, 15 ), ( 4, 6 ) ] ),
      ( u"disp16", 32, 1, 0, None, [ ( 16, 31 ) ] ),
      ( u"disp16", 32, 1, 0, None, [ ( 17, 31 ), ( 5, 5 ) ] ),
      ( u"disp16", 32, 1, 1, None, [ ( 17, 31 ) ] ),
      ( u"disp22", 32, 1, 1, None, [ ( 0, 5 ), ( 17, 31 ) ] ),
      ( u"disp23", 32, 1, 0, None, [ ( 32, 47 ), ( 20, 26 ) ] ),
      ( u"disp23", 32, 1, 1, None, [ ( 32, 47 ), ( 21, 26 ) ] ),
      ( u"disp32", 32, 1, 1, None, [ ( 17, 47 ) ] ),
      ( u"imm4", 32, 0, 0, None, [ ( 0, 3 ) ] ),
      ( u"imm5", 32, 1, 0, None, [ ( 0, 4 ) ] ),
      ( u"imm5", 32, 0, 0, None, [ ( 11, 15 ) ] ),
      ( u"imm5", 32, 0, 0, None, [ ( 0, 4 ) ] ),
      ( u"imm5", 32, 0, 2, None, [ ( 1, 5 ) ] ),
      ( u"imm6", 32, 0, 1, None, [ ( 0, 5 ) ] ),
      ( u"imm9", 32, 1, 0, None, [ ( 18, 21 ), ( 0, 4 ) ] ),
      ( u"imm9", 32, 0, 0, None, [ ( 18, 21 ), ( 0, 4 ) ] ),
      ( u"imm16", 32, 1, 0, None, [ ( 16, 31 ) ] ),
      ( u"imm16", 32, 0, 0, None, [ ( 16, 31 ) ] ),
      ( u"imm16", 32, 0, 16, None, [ ( 16, 31 ) ] ),
      ( u"imm32", 32, 1, 0, None, [ ( 16, 47 ) ] ),
      ( u"list12", 16, 0, 0, None, [ ( 21, 31 ), ( 0, 0 ) ] ),
      ( u"reg1", 8, 0, 0, u"GR", [ ( 0, 4 ) ] ),
      ( u"reg1", 8, 0, 0, u"GR", [ ( 16, 20 ) ] ),
      ( u"reg2", 8, 0, 0, u"GR", [ ( 0, 4 ) ] ),
      ( u"reg2", 8, 0, 0, u"GR", [ ( 11, 15 ) ] ),
      ( u"reg3", 8, 0, 0, u"GR", [ ( 27, 31 ) ] ),
      ( u"reg4", 8, 0, 1, u"GR", [ ( 17, 20 ) ] ),
      ( u"reg4", 8, 0, 0, u"GR", [ ( 17, 20 ), ( 23, 23 ) ] ),
      ( u"reg1", 8, 0, 1, u"GR", [ ( 1, 4 ) ] ),
      ( u"reg2", 8, 0, 1, u"GR", [ ( 12, 15 ) ] ),
      ( u"reg3", 8, 0, 1, u"GR", [ ( 28, 31 ) ] ),
      ( u"regID", 8, 0, 0, u"SR_SELID_0", [ ( 0, 4 ) ] ),
      ( u"regID", 8, 0, 0, u"SR_SELID_0", [ ( 11, 15 ) ] ),
      ( u"vector", 8, 0, 0, None, [ ( 0, 4 ) ] ),
      ( u"vector", 8, 0, 0, None, [ ( 27, 29 ), ( 0, 4 ) ] ),
      ( u"vector", 8, 0, 0, None, [ ( 11, 14 ) ] ),
      ( u"disp17", 32, 1, 1, None, [ ( 4, 4 ), ( 17, 31 ) ] ),
      ( u"msb", 32, 0, 0, None, [ ( 28, 31 ) ] ),
      ( u"lsb", 32, 0, 0, None, [ ( 27, 27 ), ( 17, 19 ) ] ),
      ( u"selID", 8, 0, 0, None, [ ( 27, 31 ) ] ),
      ( u"rt", 8, 0, 0, u"GR", [ ( 27, 31 ) ] ),
      ( u"rh", 8, 0, 0, u"GR", [ ( 0, 4 ) ] ),
      ( u"reg1", 8, 0, 0, u"GR", [ ( 11, 15 ) ] ),
      ( u"vreg1", 8, 0, 0, None, [ ( 0, 4 ) ] ),
      ( u"vreg1", 8, 0, 0, None, [ ( 11, 15 ) ] ),
      ( u"cacheop", 8, 0, 0, None, [ ( 11, 12 ), ( 27, 31 ) ] ),
      ( u"prefop", 8, 0, 0, None, [ ( 27, 31 ) ] ),
      ( u"imm10", 16, 0, 0, None, [ ( 27, 31 ), ( 0, 4 ) ] ),
      ( u"disp16", 32, 0, 1, None, [ ( 17, 31 ) ] )
    ]
    
    for (name, size, signed, left_shift, index_group, operand_field_list) in operand_info_list:
      operand = description.create_operand_description()
      operand.set_name(name)
      operand.set_size(size)
      if (signed):
        operand.set_signed()
      else:
        operand.set_unsigned()
      operand.set_left_shift(left_shift)
      if (index_group):
        operand.set_index_register_group(self.register_group_map[index_group])
      for (start_pos, end_pos) in operand_field_list:
        operand.add_operand_field(start_pos, end_pos)
      self.operand_list.append(operand)

  # ---------------------------------------------------------------------------
  # The generate_instructions() method populates the ISA description with
  # instruction description elements created by iterating over a local list of
  # tuples. Each tuple captures the instruction's mnemonic, opcode pattern,
  # operand list and optionally a list of the instruction's attributes. Note
  # that references to attributes are made by name (since attribute name should
  # be unique), however operand references are made by index values into the
  # operand_list (since operand names are generally not sufficient to uniquely
  # identify an operand).
  # ---------------------------------------------------------------------------
  
  def generate_instructions(self, description):
    instruction_info_list = \
    [
      (
        u"add {reg1}, {reg2}",
        u"-----001110-----",
        [ 32, 35 ]
      ),
      (
        u"add {imm5}, {reg2}",
        u"-----010010-----",
        [ 20, 35 ]
      ),
      (
        u"addi {imm16}, {reg1}, {reg2}",
        u"---------------------110000-----",
        [ 27, 32, 35 ]
      ),
      (
        u"adf {cccc}, {reg1}, {reg2}, {reg3}",
        u"-----011101----0-----111111-----",
        [ 3, 32, 35, 36 ]
      ),
      (
        u"and {reg1}, {reg2}",
        u"-----001010-----",
        [ 32, 35 ]
      ),
      (
        u"andi {imm16}, {reg1}, {reg2}",
        u"---------------------110110-----",
        [ 28, 32, 35 ]
      ),
      (
        u"b{cccc} {disp9}",
        u"-----1011-------",
        [ 2, 11 ]
      ),
      (
        u"bsh {reg2}, {reg3}",
        u"-----01101000010-----11111100000",
        [ 35, 36 ]
      ),
      (
        u"bsw {reg2}, {reg3}",
        u"-----01101000000-----11111100000",
        [ 35, 36 ]
      ),
      (
        u"callt {imm6}",
        u"0000001000------",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 10 )
        ],
        [ 24 ]
      ),
      (
        u"caxi [{reg1}], {reg2}, {reg3}",
        u"-----00011101110-----111111-----",
        [
          ( u"cycle_count", 4 )
        ],
        [ 32, 35, 36 ]
      ),
      (
        u"clr1 {bit}#3, {disp16} [{reg1}]",
        u"----------------10---111110-----",
        [
          ( u"cycle_count", 4 )
        ],
        [ 0, 12, 32 ]
      ),
      (
        u"clr1 {reg2}, [{reg1}]",
        u"0000000011100100-----111111-----",
        [
          ( u"cycle_count", 4 )
        ],
        [ 35, 32 ]
      ),
      (
        u"cmov {cccc}, {reg1}, {reg2}, {reg3}",
        u"-----011001----0-----111111-----",
        [ 3, 32, 35, 36 ]
      ),
      (
        u"cmov {cccc}, {imm5}, {reg2}, {reg3}",
        u"-----011000----0-----111111-----",
        [ 3, 20, 35, 36 ]
      ),
      (
        u"cmp {reg1}, {reg2}",
        u"-----001111-----",
        [ 32, 35 ]
      ),
      (
        u"cmp {imm5}, {reg2}",
        u"-----010011-----",
        [ 20, 35 ]
      ),
      (
        u"ctret",
        u"00000001010001000000011111100000",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 7 )
        ],
        None
      ),
      (
        u"di",
        u"00000001011000000000011111100000",
        [
          ( u"cycle_count", 3 )
        ],
        None
      ),
      (
        u"dispose {imm5}, {list12}",
        u"-----------000000000011001------",
        [
          ( u"cycle_count", 2 )
        ],
        [ 23, 31 ]
      ),
      (
        u"dispose {imm5}, {list12}, [{reg1}]",
        u"----------------0000011001------",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 4 )
        ],
        [ 23, 31, 33 ]
      ),
      (
        u"div {reg1}, {reg2}, {reg3}",
        u"-----01011000000-----111111-----",
        [
          ( u"cycle_count", 19 )
        ],
        [ 32, 35, 36 ]
      ),
      (
        u"switch {reg1}",
        u"00000000010-----",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 8 )
        ],
        [ 32 ]
      ),
      (
        u"divh {reg1}, {reg2}",
        u"-----000010-----",
        [
          ( u"cycle_count", 19 )
        ],
        [ 32, 35 ]
      ),
      (
        u"divh {reg1}, {reg2}, {reg3}",
        u"-----01010000000-----111111-----",
        [
          ( u"cycle_count", 19 )
        ],
        [ 32, 35, 36 ]
      ),
      (
        u"divhu {reg1}, {reg2}, {reg3}",
        u"-----01010000010-----111111-----",
        [
          ( u"cycle_count", 19 )
        ],
        [ 32, 35, 36 ]
      ),
      (
        u"divq {reg1}, {reg2}, {reg3}",
        u"-----01011111100-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"divqu {reg1}, {reg2}, {reg3}",
        u"-----01011111110-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"divu {reg1}, {reg2}, {reg3}",
        u"-----01011000010-----111111-----",
        [
          ( u"cycle_count", 19 )
        ],
        [ 32, 35, 36 ]
      ),
      (
        u"ei",
        u"00000001011000001000011111100000",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 3 )
        ],
        None
      ),
      (
        u"eiret",
        u"00000001010010000000011111100000",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 7 )
        ],
        None
      ),
      (
        u"feret",
        u"00000001010010100000011111100000",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 7 )
        ],
        None
      ),
      (
        u"fetrap {vector}",
        u"0----00001000000",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 7 )
        ],
        [ 46 ]
      ),
      (
        u"halt",
        u"00000001001000000000011111100000",
        [
          ( u"terminates_block", True )
        ],
        None
      ),
      (
        u"hsh {reg2}, {reg3}",
        u"-----01101000110-----11111100000",
        [ 35, 36 ]
      ),
      (
        u"hsw {reg2}, {reg3}",
        u"-----01101000100-----11111100000",
        [ 35, 36 ]
      ),
      (
        u"jr {disp22}",
        u"---------------00000011110------",
        [ 15 ]
      ),
      (
        u"jr {disp32}",
        u"-------------------------------00000001011100000",
        [ 18 ]
      ),
      (
        u"jarl {disp22}, {reg2}",
        u"---------------0-----11110------",
        [
          ( u"cycle_count", 4 )
        ],
        [ 35, 15 ]
      ),
      (
        u"jarl {disp32}, {reg1}",
        u"-------------------------------000000010111-----",
        [
          ( u"cycle_count", 4 )
        ],
        [ 32, 18 ]
      ),
      (
        u"jmp [{reg1}]",
        u"00000000011-----",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 4 )
        ],
        [ 32 ]
      ),
      (
        u"jmp {disp32}[{reg1}]",
        u"-------------------------------000000110111-----",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 5 )
        ],
        [ 32, 18 ]
      ),
      (
        u"prepare {list12}, {imm5}",
        u"-----------000010000011110------",
        [
          ( u"cycle_count", 2 )
        ],
        [ 31, 23 ]
      ),
      (
        u"prepare {list12}, {imm5}, {bits_ff}",
        u"-------------0110000011110------",
        [
          ( u"cycle_count", 2 )
        ],
        [ 31, 23, 1 ]
      ),
      (
        u"ld.b {disp16} [{reg1}], {reg2}",
        u"---------------------111000-----",
        [ 12, 32, 35 ]
      ),
      (
        u"ld.b {disp23} [{reg1}], {reg3}",
        u"----------------------------010100000111100-----",
        [ 16, 32, 36 ]
      ),
      (
        u"ld.bu {disp23} [{reg1}], {reg3}",
        u"----------------------------010100000111101-----",
        [ 16, 32, 36 ]
      ),
      (
        u"ld.bu {disp16} [{reg1}], {reg2}",
        u"---------------1-----11110------",
        [ 13, 32, 35 ]
      ),
      (
        u"ld.h {disp16} [{reg1}], {reg2}",
        u"---------------0-----111001-----",
        [ 14, 32, 35 ]
      ),
      (
        u"ld.h {disp23} [{reg1}], {reg3}",
        u"---------------------------0011100000111100-----",
        [ 17, 32, 36 ]
      ),
      (
        u"ld.hu {disp16} [{reg1}], {reg2}",
        u"---------------1-----111111-----",
        [ 14, 32, 35 ]
      ),
      (
        u"ld.hu {disp23} [{reg1}], {reg3}",
        u"---------------------------0011100000111101-----",
        [ 17, 32, 36 ]
      ),
      (
        u"ld.w {disp16} [{reg1}], {reg2}",
        u"---------------1-----111001-----",
        [ 14, 32, 35 ]
      ),
      (
        u"ld.w {disp23} [{reg1}], {reg3}",
        u"---------------------------0100100000111100-----",
        [ 17, 32, 36 ]
      ),
      (
        u"ldsr {reg2}, {regID}, {selID}",
        u"-----00000100000-----111111-----",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 3 )
        ],
        [ 34, 43, 50 ]
      ),
      (
        u"mac {reg1}, {reg2}, {reg3}, {reg4}",
        u"----0011110----0-----111111-----",
        [ 32, 35, 36, 37 ]
      ),
      (
        u"macu {reg1}, {reg2}, {reg3}, {reg4}",
        u"----0011111----0-----111111-----",
        [ 32, 35, 36, 37 ]
      ),
      (
        u"nop",
        u"0000000000000000",
        None
      ),
      (
        u"mov {reg1}, {reg2}",
        u"-----000000-----",
        [ 32, 35 ]
      ),
      (
        u"mov {imm5}, {reg2}",
        u"-----010000-----",
        [ 20, 35 ]
      ),
      (
        u"mov {imm32}, {reg1}",
        u"--------------------------------00000110001-----",
        [ 32, 30 ]
      ),
      (
        u"movea {imm16}, {reg1}, {reg2}",
        u"---------------------110001-----",
        [ 27, 32, 35 ]
      ),
      (
        u"movhi {imm16}, {reg1}, {reg2}",
        u"---------------------110010-----",
        [ 29, 32, 35 ]
      ),
      (
        u"mul {reg1}, {reg2}, {reg3}",
        u"-----01000100000-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"sxh {reg1}",
        u"00000000111-----",
        [ 32 ]
      ),
      (
        u"mul {imm9}, {reg2}, {reg3}",
        u"-----01001----00-----111111-----",
        [ 25, 35, 36 ]
      ),
      (
        u"mulh {reg1}, {reg2}",
        u"-----000111-----",
        [ 32, 35 ]
      ),
      (
        u"mulh {imm5}, {reg2}",
        u"-----010111-----",
        [ 20, 35 ]
      ),
      (
        u"mulhi {imm16}, {reg1}, {reg2}",
        u"---------------------110111-----",
        [ 27, 32, 35 ]
      ),
      (
        u"mulu {reg1}, {reg2}, {reg3}",
        u"-----01000100010-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"mulu {imm9}, {reg2}, {reg3}",
        u"-----01001----10-----111111-----",
        [ 26, 35, 36 ]
      ),
      (
        u"not {reg1}, {reg2}",
        u"-----000001-----",
        [ 32, 35 ]
      ),
      (
        u"not1 {bit}#3, {disp16} [{reg1}]",
        u"----------------01---111110-----",
        [
          ( u"cycle_count", 4 )
        ],
        [ 0, 12, 32 ]
      ),
      (
        u"not1 {reg2}, [{reg1}]",
        u"0000000011100010-----111111-----",
        [
          ( u"cycle_count", 4 )
        ],
        [ 35, 32 ]
      ),
      (
        u"or {reg1}, {reg2}",
        u"-----001000-----",
        [ 32, 35 ]
      ),
      (
        u"ori {imm16}, {reg1}, {reg2}",
        u"---------------------110100-----",
        [ 28, 32, 35 ]
      ),
      (
        u"rie",
        u"0000000001000000",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 7 )
        ],
        None
      ),
      (
        u"rie {imm5}, {imm4}",
        u"0000000000000000-----1111111----",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 7 )
        ],
        [ 21, 19 ]
      ),
      (
        u"sar {reg1}, {reg2}",
        u"0000000010100000-----111111-----",
        [ 32, 35 ]
      ),
      (
        u"sar {imm5}, {reg2}",
        u"-----010101-----",
        [ 22, 35 ]
      ),
      (
        u"sar {reg1}, {reg2}, {reg3}",
        u"-----00010100010-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"sasf {cccc}, {reg2}",
        u"0000001000000000-----1111110----",
        [ 2, 35 ]
      ),
      (
        u"sxb {reg1}",
        u"00000000101-----",
        [ 32 ]
      ),
      (
        u"zxb {reg1}",
        u"00000000100-----",
        [ 32 ]
      ),
      (
        u"zxh {reg1}",
        u"00000000110-----",
        [ 32 ]
      ),
      (
        u"satadd {reg1}, {reg2}",
        u"-----000110-----",
        [ 32, 35 ]
      ),
      (
        u"satadd {imm5}, {reg2}",
        u"-----010001-----",
        [ 20, 35 ]
      ),
      (
        u"satadd {reg1}, {reg2}, {reg3}",
        u"-----01110111010-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"satsub {reg1}, {reg2}",
        u"-----000101-----",
        [ 32, 35 ]
      ),
      (
        u"satsub {reg1}, {reg2}, {reg3}",
        u"-----01110011010-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"satsubi {imm16}, {reg1}, {reg2}",
        u"---------------------110011-----",
        [ 27, 32, 35 ]
      ),
      (
        u"satsubr {reg1}, {reg2}",
        u"-----000100-----",
        [ 32, 35 ]
      ),
      (
        u"sbf {cccc}, {reg1}, {reg2}, {reg3}",
        u"-----011100----0-----111111-----",
        [ 3, 32, 35, 36 ]
      ),
      (
        u"sch0l {reg2}, {reg3}",
        u"-----01101100100-----11111100000",
        [ 35, 36 ]
      ),
      (
        u"sch0r {reg2}, {reg3}",
        u"-----01101100000-----11111100000",
        [ 35, 36 ]
      ),
      (
        u"sch1l {reg2}, {reg3}",
        u"-----01101100110-----11111100000",
        [ 35, 36 ]
      ),
      (
        u"sch1r {reg2}, {reg3}",
        u"-----01101100010-----11111100000",
        [ 35, 36 ]
      ),
      (
        u"set1 {bit}#3, {disp16} [{reg1}]",
        u"----------------00---111110-----",
        [
          ( u"cycle_count", 4 )
        ],
        [ 0, 12, 32 ]
      ),
      (
        u"set1 {reg2}, [{reg1}]",
        u"0000000011100000-----111111-----",
        [
          ( u"cycle_count", 4 )
        ],
        [ 35, 32 ]
      ),
      (
        u"setf {cccc}, {reg2}",
        u"0000000000000000-----1111110----",
        [ 2, 35 ]
      ),
      (
        u"shl {reg1}, {reg2}",
        u"0000000011000000-----111111-----",
        [ 32, 35 ]
      ),
      (
        u"shl {imm5}, {reg2}",
        u"-----010110-----",
        [ 22, 35 ]
      ),
      (
        u"shl {reg1}, {reg2}, {reg3}",
        u"-----00011000010-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"shr {reg1}, {reg2}",
        u"0000000010000000-----111111-----",
        [ 32, 35 ]
      ),
      (
        u"shr {imm5}, {reg2}",
        u"-----010100-----",
        [ 22, 35 ]
      ),
      (
        u"shr {reg1}, {reg2}, {reg3}",
        u"-----00010000010-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"sld.b {disp7} [ep], {reg2}",
        u"-----0110-------",
        [ 8, 35 ]
      ),
      (
        u"sld.bu {disp4} [ep], {reg2}",
        u"-----0000110----",
        [ 6, 35 ]
      ),
      (
        u"sld.h {disp8} [ep], {reg2}",
        u"-----1000-------",
        [ 9, 35 ]
      ),
      (
        u"sld.hu {disp5} [ep], {reg2}",
        u"-----0000111----",
        [ 7, 35 ]
      ),
      (
        u"sld.w {disp8} [ep], {reg2}",
        u"-----1010------0",
        [ 10, 35 ]
      ),
      (
        u"sst.b {reg2}, {disp7} [ep]",
        u"-----0111-------",
        [ 8, 35 ]
      ),
      (
        u"sst.h {reg2}, {disp8} [ep]",
        u"-----1001-------",
        [ 9, 35 ]
      ),
      (
        u"sst.w {reg2}, {disp8} [ep]",
        u"-----1010------1",
        [ 10, 35 ]
      ),
      (
        u"st.b {reg2}, {disp16} [{reg1}]",
        u"---------------------111010-----",
        [ 35, 12, 32 ]
      ),
      (
        u"st.b {reg3}, {disp23} [{reg1}]",
        u"----------------------------110100000111100-----",
        [ 36, 16, 32 ]
      ),
      (
        u"st.h {reg2}, {disp16} [{reg1}]",
        u"---------------0-----111011-----",
        [ 35, 14, 32 ]
      ),
      (
        u"st.h {reg3}, {disp23} [{reg1}]",
        u"---------------------------0110100000111101-----",
        [ 36, 17, 32 ]
      ),
      (
        u"st.w {reg2}, {disp16} [{reg1}]",
        u"---------------1-----111011-----",
        [ 35, 14, 32 ]
      ),
      (
        u"st.w {reg3}, {disp23} [{reg1}]",
        u"---------------------------0111100000111100-----",
        [ 36, 17, 32 ]
      ),
      (
        u"stsr {regID}, {reg2}, {selID}",
        u"-----00001000000-----111111-----",
        [ 42, 35, 50 ]
      ),
      (
        u"sub {reg1}, {reg2}",
        u"-----001101-----",
        [ 32, 35 ]
      ),
      (
        u"subr {reg1}, {reg2}",
        u"-----001100-----",
        [ 32, 35 ]
      ),
      (
        u"synce",
        u"0000000000011101",
        None
      ),
      (
        u"syncm",
        u"0000000000011110",
        None
      ),
      (
        u"syncp",
        u"0000000000011111",
        None
      ),
      (
        u"syscall {vector}",
        u"00---0010110000011010111111-----",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 10 )
        ],
        [ 45 ]
      ),
      (
        u"trap {vector}",
        u"000000010000000000000111111-----",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 7 )
        ],
        [ 44 ]
      ),
      (
        u"tst {reg1}, {reg2}",
        u"-----001011-----",
        [ 32, 35 ]
      ),
      (
        u"tst1 {bit}#3, {disp16} [{reg1}]",
        u"----------------11---111110-----",
        [
          ( u"cycle_count", 4 )
        ],
        [ 0, 12, 32 ]
      ),
      (
        u"tst1 {reg2}, [{reg1}]",
        u"0000000011100110-----111111-----",
        [
          ( u"cycle_count", 4 )
        ],
        [ 35, 32 ]
        ),
      (
        u"xor {reg1}, {reg2}",
        u"-----001001-----",
        [ 32, 35 ]
      ),
      (
        u"xori {imm16}, {reg1}, {reg2}",
        u"---------------------110101-----",
        [ 28, 32, 35 ]
      ),
      (
        u"cmpf.d {fcond}, {reg2}, {reg1}, {fcbit}",
        u"0----1000011---0----0111111----0",
        [ 39, 40, 4, 5 ]
      ),
      (
        u"maddf.s {reg1}, {reg2}, {reg3}, {reg4}",
        u"-----101-00----0-----111111-----",
        [ 32, 35, 36, 38 ]
      ),
      (
        u"maxf.d {reg1}, {reg2}, {reg3}",
        u"----010001111000----0111111----0",
        [ 39, 40, 41 ]
      ),
      (
        u"b{cccc} {disp17}",
        u"---------------100000111111-----",
        [ 2, 47 ]
      ),
      (
        u"binsu {reg1}, {lsb}, {msb}, {reg2}",
        u"-----0001001---0-----111111-----",
        [ 32, 49, 48, 35 ]
      ),
      (
        u"binsm {reg1}, {lsb}, {msb}, {reg2}",
        u"-----0001011---0-----111111-----",
        [ 32, 49, 48, 35 ]
      ),
      (
        u"binsl {reg1}, {lsb}, {msb}, {reg2}",
        u"-----0001101---0-----111111-----",
        [ 32, 49, 48, 35 ]
      ),
      (
        u"jarl [{reg1}], {reg3}",
        u"-----0010110000011000111111-----",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 4 )
        ],
        [ 32, 36 ]
      ),
      (
        u"ld.dw {disp23} [{reg1}], {reg3}",
        u"---------------------------0100100000111101-----",
        [ 17, 32, 36 ]
      ),
      (
        u"ldl.w [{reg1}], {reg3}",
        u"-----0110111100000000111111-----",
        [ 32, 36 ]
      ),
      (
        u"loop {reg1}, {disp16}",
        u"---------------100000110111-----",
        [ 32, 59 ]
      ),
      (
        u"popsp {rh}-{rt}",
        u"-----0010110000001100111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 52, 51 ]
      ),
      (
        u"pushsp {rh}-{rt}",
        u"-----0010110000001000111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 52, 51 ]
      ),
      (
        u"rotl {imm5}, {reg2}, {reg3}",
        u"-----00011000100-----111111-----",
        [ 22, 35, 36 ]
      ),
      (
        u"rotl {reg1}, {reg2}, {reg3}",
        u"-----00011000110-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"snooze",
        u"00000001001000000000111111100000",
        None
      ),
      (
        u"st.dw {reg3}, {disp23} [{reg1}]",
        u"---------------------------0111100000111101-----",
        [ 36, 17, 32 ]
      ),
      (
        u"stc.w {reg3}, [{reg1}]",
        u"-----0110111101000000111111-----",
        [ 36, 32 ]
      ),
      (
        u"synci",
        u"0000000000011100",
        None
      ),
      (
        u"hvcall {vector}",
        u"01---0010110000011010111111-----",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 10 )
        ],
        [ 45 ]
      ),
      (
        u"hvtrap {vector}",
        u"000000010001000000000111111-----",
        [ 44 ]
      ),
      (
        u"ldvc.sr {reg2}, {regID}, {selID}",
        u"-----00000110100-----111111-----",
        [ 34, 43, 50 ]
      ),
      (
        u"stvc.sr {regID}, {reg2}, {selID}",
        u"-----00001010100-----111111-----",
        [ 35, 42, 50 ]
      ),
      (
        u"dst",
        u"00000001001101000000011111100000",
        None
      ),
      (
        u"est",
        u"00000001001100100000011111100000",
        None
      ),
      (
        u"ldtc.gr {reg2}, {reg1}",
        u"0000000000110010-----111111-----",
        [ 34, 53 ]
      ),
      (
        u"ldtc.vr {reg2}, {vreg1}",
        u"0000100000110010-----111111-----",
        [ 34, 55 ]
      ),
      (
        u"ldtc.pc {reg2}",
        u"1111100000110010-----111111-----",
        [ 34 ]
      ),
      (
        u"ldtc.sr {reg2}, {regID}, {selID}",
        u"-----00000110000-----111111-----",
        [ 43, 34, 50 ]
      ),
      (
        u"sttc.gr {reg1}, {reg2}",
        u"0000000001010010-----111111-----",
        [ 32, 35 ]
      ),
      (
        u"sttc.vr {vreg1}, {reg2}",
        u"0000100001010010-----111111-----",
        [ 54, 35 ]
      ),
      (
        u"sttc.pc {reg2}",
        u"1111100001010010-----111111-----",
        [ 35 ]
      ),
      (
        u"sttc.sr {regID}, {reg2}, {selID}",
        u"-----00001010000-----111111-----",
        [ 42, 35, 50 ]
      ),
      (
        u"cache {cacheop}, [{reg1}]",
        u"-----00101100000111--111111-----",
        [ 56, 32 ]
      ),
      (
        u"pref {prefop}, [{reg1}]",
        u"-----0010110000011011111111-----",
        [ 57, 32 ]
      ),
      (
        u"dbcp",
        u"1110100001000000",
        None
      ),
      (
        u"dbhvtrap",
        u"1110000001000000",
        None
      ),
      (
        u"dbpush {rh}-{rt}",
        u"-----0010110000001011111111-----",
        [ 52, 51 ]
      ),
      (
        u"dbret",
        u"00000001010001100000011111100000",
        None
      ),
      (
        u"dbtag {imm10}",
        u"-----0010110000011001111111-----",
        [ 58 ]
      ),
      (
        u"dbtrap",
        u"1111100001000000",
        None
      ),
      (
        u"rmtrap",
        u"1111000001000000",
        None
      ),
      (
        u"cll",
        u"11110001011000001111111111111111",
        None
      )
    ]
    
    for instruction_info in instruction_info_list:
      if (len(instruction_info) == 4):
        (mnemonic, opcode_pattern, attribute_value_list, operand_index_list) =  instruction_info
      else:
        (mnemonic, opcode_pattern, operand_index_list) = instruction_info
        attribute_value_list = None
      instruction = description.create_instruction_description()
      instruction.set_mnemonic(mnemonic)
      instruction.set_opcode(opcode_pattern)
      if (attribute_value_list != None):
        for (attribute_name, attribute_value) in attribute_value_list:
          instruction.add_attribute(self.attribute_map[attribute_name], attribute_value)
      if (operand_index_list != None):
        for operand_index in operand_index_list:
          instruction.add_operand(self.operand_list[operand_index])
      self.instruction_map[mnemonic] = instruction

  # ---------------------------------------------------------------------------
  # The generate_instruction_sets() method populates the ISA description with
  # instruction set description elements. This operation iterates over a local
  # list of tuples describing the instruction set's name and member instructions.
  # Note that instructions are referred to by mnemonic (since instruction names
  # should be unique).
  # ---------------------------------------------------------------------------
  
  def generate_instruction_sets(self, description):
    instruction_set_info_list = \
    [
      (
        u"rh850e3",
        [
          u"add {reg1}, {reg2}",
          u"add {imm5}, {reg2}",
          u"addi {imm16}, {reg1}, {reg2}",
          u"adf {cccc}, {reg1}, {reg2}, {reg3}",
          u"and {reg1}, {reg2}",
          u"andi {imm16}, {reg1}, {reg2}",
          u"b{cccc} {disp9}",
          u"bsh {reg2}, {reg3}",
          u"bsw {reg2}, {reg3}",
          u"callt {imm6}",
          u"caxi [{reg1}], {reg2}, {reg3}",
          u"clr1 {bit}#3, {disp16} [{reg1}]",
          u"clr1 {reg2}, [{reg1}]",
          u"cmov {cccc}, {reg1}, {reg2}, {reg3}",
          u"cmov {cccc}, {imm5}, {reg2}, {reg3}",
          u"cmp {reg1}, {reg2}",
          u"cmp {imm5}, {reg2}",
          u"ctret",
          u"di",
          u"dispose {imm5}, {list12}",
          u"dispose {imm5}, {list12}, [{reg1}]",
          u"div {reg1}, {reg2}, {reg3}",
          u"switch {reg1}",
          u"divh {reg1}, {reg2}",
          u"divh {reg1}, {reg2}, {reg3}",
          u"divhu {reg1}, {reg2}, {reg3}",
          u"divq {reg1}, {reg2}, {reg3}",
          u"divqu {reg1}, {reg2}, {reg3}",
          u"divu {reg1}, {reg2}, {reg3}",
          u"ei",
          u"eiret",
          u"feret",
          u"fetrap {vector}",
          u"halt",
          u"hsh {reg2}, {reg3}",
          u"hsw {reg2}, {reg3}",
          u"jr {disp22}",
          u"jr {disp32}",
          u"jarl {disp22}, {reg2}",
          u"jarl {disp32}, {reg1}",
          u"jmp [{reg1}]",
          u"jmp {disp32}[{reg1}]",
          u"prepare {list12}, {imm5}",
          u"prepare {list12}, {imm5}, {bits_ff}",
          u"ld.b {disp16} [{reg1}], {reg2}",
          u"ld.b {disp23} [{reg1}], {reg3}",
          u"ld.bu {disp23} [{reg1}], {reg3}",
          u"ld.bu {disp16} [{reg1}], {reg2}",
          u"ld.h {disp16} [{reg1}], {reg2}",
          u"ld.h {disp23} [{reg1}], {reg3}",
          u"ld.hu {disp16} [{reg1}], {reg2}",
          u"ld.hu {disp23} [{reg1}], {reg3}",
          u"ld.w {disp16} [{reg1}], {reg2}",
          u"ld.w {disp23} [{reg1}], {reg3}",
          u"ldsr {reg2}, {regID}, {selID}",
          u"mac {reg1}, {reg2}, {reg3}, {reg4}",
          u"macu {reg1}, {reg2}, {reg3}, {reg4}",
          u"nop",
          u"mov {reg1}, {reg2}",
          u"mov {imm5}, {reg2}",
          u"mov {imm32}, {reg1}",
          u"movea {imm16}, {reg1}, {reg2}",
          u"movhi {imm16}, {reg1}, {reg2}",
          u"mul {reg1}, {reg2}, {reg3}",
          u"sxh {reg1}",
          u"mul {imm9}, {reg2}, {reg3}",
          u"mulh {reg1}, {reg2}",
          u"mulh {imm5}, {reg2}",
          u"mulhi {imm16}, {reg1}, {reg2}",
          u"mulu {reg1}, {reg2}, {reg3}",
          u"mulu {imm9}, {reg2}, {reg3}",
          u"not {reg1}, {reg2}",
          u"not1 {bit}#3, {disp16} [{reg1}]",
          u"not1 {reg2}, [{reg1}]",
          u"or {reg1}, {reg2}",
          u"ori {imm16}, {reg1}, {reg2}",
          u"rie",
          u"rie {imm5}, {imm4}",
          u"sar {reg1}, {reg2}",
          u"sar {imm5}, {reg2}",
          u"sar {reg1}, {reg2}, {reg3}",
          u"sasf {cccc}, {reg2}",
          u"sxb {reg1}",
          u"zxb {reg1}",
          u"zxh {reg1}",
          u"satadd {reg1}, {reg2}",
          u"satadd {imm5}, {reg2}",
          u"satadd {reg1}, {reg2}, {reg3}",
          u"satsub {reg1}, {reg2}",
          u"satsub {reg1}, {reg2}, {reg3}",
          u"satsubi {imm16}, {reg1}, {reg2}",
          u"satsubr {reg1}, {reg2}",
          u"sbf {cccc}, {reg1}, {reg2}, {reg3}",
          u"sch0l {reg2}, {reg3}",
          u"sch0r {reg2}, {reg3}",
          u"sch1l {reg2}, {reg3}",
          u"sch1r {reg2}, {reg3}",
          u"set1 {bit}#3, {disp16} [{reg1}]",
          u"set1 {reg2}, [{reg1}]",
          u"setf {cccc}, {reg2}",
          u"shl {reg1}, {reg2}",
          u"shl {imm5}, {reg2}",
          u"shl {reg1}, {reg2}, {reg3}",
          u"shr {reg1}, {reg2}",
          u"shr {imm5}, {reg2}",
          u"shr {reg1}, {reg2}, {reg3}",
          u"sld.b {disp7} [ep], {reg2}",
          u"sld.bu {disp4} [ep], {reg2}",
          u"sld.h {disp8} [ep], {reg2}",
          u"sld.hu {disp5} [ep], {reg2}",
          u"sld.w {disp8} [ep], {reg2}",
          u"sst.b {reg2}, {disp7} [ep]",
          u"sst.h {reg2}, {disp8} [ep]",
          u"sst.w {reg2}, {disp8} [ep]",
          u"st.b {reg2}, {disp16} [{reg1}]",
          u"st.b {reg3}, {disp23} [{reg1}]",
          u"st.h {reg2}, {disp16} [{reg1}]",
          u"st.h {reg3}, {disp23} [{reg1}]",
          u"st.w {reg2}, {disp16} [{reg1}]",
          u"st.w {reg3}, {disp23} [{reg1}]",
          u"stsr {regID}, {reg2}, {selID}",
          u"sub {reg1}, {reg2}",
          u"subr {reg1}, {reg2}",
          u"synce",
          u"syncm",
          u"syncp",
          u"syscall {vector}",
          u"trap {vector}",
          u"tst {reg1}, {reg2}",
          u"tst1 {bit}#3, {disp16} [{reg1}]",
          u"tst1 {reg2}, [{reg1}]",
          u"xor {reg1}, {reg2}",
          u"xori {imm16}, {reg1}, {reg2}",
          u"cmpf.d {fcond}, {reg2}, {reg1}, {fcbit}",
          u"maddf.s {reg1}, {reg2}, {reg3}, {reg4}",
          u"maxf.d {reg1}, {reg2}, {reg3}",
          u"b{cccc} {disp17}",
          u"binsu {reg1}, {lsb}, {msb}, {reg2}",
          u"binsm {reg1}, {lsb}, {msb}, {reg2}",
          u"binsl {reg1}, {lsb}, {msb}, {reg2}",
          u"jarl [{reg1}], {reg3}",
          u"ld.dw {disp23} [{reg1}], {reg3}",
          u"ldl.w [{reg1}], {reg3}",
          u"loop {reg1}, {disp16}",
          u"popsp {rh}-{rt}",
          u"pushsp {rh}-{rt}",
          u"rotl {imm5}, {reg2}, {reg3}",
          u"rotl {reg1}, {reg2}, {reg3}",
          u"snooze",
          u"st.dw {reg3}, {disp23} [{reg1}]",
          u"stc.w {reg3}, [{reg1}]",
          u"synci",
          u"hvcall {vector}",
          u"hvtrap {vector}",
          u"ldvc.sr {reg2}, {regID}, {selID}",
          u"stvc.sr {regID}, {reg2}, {selID}",
          u"dst",
          u"est",
          u"ldtc.gr {reg2}, {reg1}",
          u"ldtc.vr {reg2}, {vreg1}",
          u"ldtc.pc {reg2}",
          u"ldtc.sr {reg2}, {regID}, {selID}",
          u"sttc.gr {reg1}, {reg2}",
          u"sttc.vr {vreg1}, {reg2}",
          u"sttc.pc {reg2}",
          u"sttc.sr {regID}, {reg2}, {selID}",
          u"cache {cacheop}, [{reg1}]",
          u"pref {prefop}, [{reg1}]",
          u"dbcp",
          u"dbhvtrap",
          u"dbpush {rh}-{rt}",
          u"dbret",
          u"dbtag {imm10}",
          u"dbtrap",
          u"rmtrap",
          u"cll"
        ]
      )
    ]
    
    for (name, instruction_name_list) in instruction_set_info_list:
      instruction_set = description.create_instruction_set_description()
      instruction_set.set_name(name)
      for instruction_name in instruction_name_list:
        instruction_set.add_instruction(self.instruction_map[instruction_name])

  # ---------------------------------------------------------------------------
  # The generate_bus_interfaces() method populates the ISA description with
  # bus interface description elements. This operation iterates over a local
  # list of bus_interface names.
  # ---------------------------------------------------------------------------
      
  def generate_bus_interfaces(self, description):
    pass

#------------------------------------------------------------------------------
# Module execution prevention
#------------------------------------------------------------------------------

if __name__ == '__main__':
  sys.stderr.write("Error: This is a module and cannot be executed\n")
  sys.exit(-1)

#------------------------------------------------------------------------------
# End of file
#------------------------------------------------------------------------------
