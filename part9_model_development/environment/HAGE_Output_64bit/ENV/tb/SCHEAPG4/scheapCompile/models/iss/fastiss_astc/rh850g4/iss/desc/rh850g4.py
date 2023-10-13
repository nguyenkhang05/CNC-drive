#!/usr/bin/env python

#/*************************************************************************
# *
# * Copyright(c) 2015-2017, 2020 Renesas Electronics Corporation
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
    description.set_name(u"rh850g4")
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
      ( u"PSW_EBV", 15, 15 ),
      ( u"PSW_CU0", 16, 16 ),
      ( u"PSW_CU1", 17, 17 ),
      ( u"PSW_CU2", 18, 18 ),
      ( u"PSW_EIMASK", 20, 27 ),
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
      ( u"PSWH_GM", 31, 31 ),
      ( u"PSWH_GPID", 8, 10 ),
      ( u"EIPSWH_GM", 31, 31 ),
      ( u"EIPSWH_GPID", 8, 10 ),
      ( u"FEPSWH_GM", 31, 31 ),
      ( u"FEPSWH_GPID", 8, 10 ),
      ( u"RBASE_RBASE", 9, 31 ),
      ( u"RBASE_DV", 1, 1 ),
      ( u"RBASE_RINT", 0, 0 ),
      ( u"EBASE_EBASE", 9, 31 ),
      ( u"EBASE_DV", 1, 1 ),
      ( u"EBASE_RINT", 0, 0 ),
      ( u"MCTL_UIC", 0, 0 ),
      ( u"SCCFG_SIZE", 0, 7 ),
      ( u"HVCFG_HVE", 0, 0 ),
      ( u"GMCFG_GCU2", 18, 18 ),
      ( u"GMCFG_GCU1", 17, 17 ),
      ( u"GMCFG_GCU0", 16, 16 ),
      ( u"GMCFG_GSYSE", 4, 4 ),
      ( u"GMCFG_HMP", 1, 1 ),
      ( u"GMCFG_GMP", 0, 0 ),
      ( u"BMID_BMID", 0, 4 ),
      ( u"RBCR0_MD", 16, 16 ),
      ( u"RBCR0_BE", 0, 15 ),
      ( u"RBCR1_NC", 0, 15 ),
      ( u"RBNR_BN", 0, 7 ),
      ( u"RBIP_RBIP", 4, 31 ),
      ( u"ISPR_ISP", 0, 15 ),
      ( u"IMSR_HFNP", 12, 12 ),
      ( u"IMSR_HENP", 11, 11 ),
      ( u"IMSR_HEID", 10, 10 ),
      ( u"IMSR_HEPLM", 9, 9 ),
      ( u"IMSR_HEEIM", 8, 8 ),
      ( u"IMSR_FNP", 4, 4 ),
      ( u"IMSR_ENP", 3, 3 ),
      ( u"IMSR_EID", 2, 2 ),
      ( u"IMSR_EPLM", 1, 1 ),
      ( u"IMSR_EEIM", 0, 0 ),
      ( u"ICSR_PMEI", 0, 0 ),
      ( u"INTCFG_ISPC", 0, 0 ),
      ( u"INTCFG_EPL", 1, 1 ),
      ( u"INTCFG_ULNR", 16, 23 ),
      ( u"PLMR_PLM", 0, 7 ),
      ( u"FXSR_FN", 23, 23 ),
      ( u"FXSR_IF", 22, 22 ),
      ( u"FXSR_PEM", 21, 21 ),
      ( u"FXSR_RM", 18, 19 ),
      ( u"FXSR_FS", 17, 17 ),
      ( u"FXSR_XC", 10, 15 ),
      ( u"FXSR_XE", 5, 9 ),
      ( u"FXSR_XP", 0, 4 ),
      ( u"FXST_XC", 8, 13 ),
      ( u"FXST_IF", 5, 5 ),
      ( u"FXST_XP", 0, 4 ),
      ( u"FXINFO_NFPU", 1, 1 ),
      ( u"FXINFO_RSIZE", 0, 0 ),
      ( u"FXCFG_RM", 8, 9 ),
      ( u"FXCFG_XE", 0, 4 ),      
      ( u"FXXC_XC3", 24, 29 ),
      ( u"FXXC_XC2", 16, 21 ),
      ( u"FXXC_XC1", 8, 13 ),
      ( u"FXXC_XC0", 0, 5 ),
      ( u"FXXP_XP3", 24, 28 ),
      ( u"FXXP_XP2", 16, 20 ),
      ( u"FXXP_XP1", 8, 12 ),
      ( u"FXXP_XP0", 0, 4 ),
      ( u"GMPSW_Z", 0, 0 ),
      ( u"GMPSW_S", 1, 1 ),
      ( u"GMPSW_OV", 2, 2 ),
      ( u"GMPSW_CY", 3, 3 ),
      ( u"GMPSW_SAT", 4, 4 ),
      ( u"GMPSW_ID", 5, 5 ),
      ( u"GMPSW_EP", 6, 6 ),
      ( u"GMPSW_NP", 7, 7 ),
      ( u"GMPSW_EBV", 15, 15 ),
      ( u"GMPSW_CU0", 16, 16 ),
      ( u"GMPSW_CU1", 17, 17 ),
      ( u"GMPSW_CU2", 18, 18 ),
      ( u"GMPSW_EIMASK", 20, 27 ),
      ( u"GMPSW_UM", 30, 30 ),     
      ( u"GMEBASE_EBASE", 9, 31 ),
      ( u"GMEBASE_DV", 1, 1 ),
      ( u"GMEBASE_RINT", 0, 0 ),
      ( u"GMINTCFG_ISPC", 0, 0 ),
      ( u"GMINTCFG_EPL", 1, 1 ),
      ( u"GMINTCFG_ULNR", 16, 23 ),
      ( u"GMPLMR_PLM", 0, 7 ),
      ( u"MPCFG_DMDP", 24, 24 ),
      ( u"MPCFG_NBK", 20, 21 ),
      ( u"MPCFG_ARCH", 16, 19 ),
      ( u"MPCFG_HBE", 8, 13 ),
      ( u"MPCFG_NMPUE", 0, 4 ),
      ( u"MCR_HSXE", 29, 29 ),
      ( u"MCR_HSWE", 28, 28 ),
      ( u"MCR_HSRE", 27, 27 ),
      ( u"MCR_HUXE", 26, 26 ),
      ( u"MCR_HUWE", 25, 25 ),
      ( u"MCR_HURE", 24, 24 ),
      ( u"MCR_GSXE", 21, 21 ),
      ( u"MCR_GSWE", 20, 20 ),
      ( u"MCR_GSRE", 19, 19 ),
      ( u"MCR_GUXE", 18, 18 ),
      ( u"MCR_GUWE", 17, 17 ),
      ( u"MCR_GURE", 16, 16 ),
      ( u"MCR_OV", 8, 8 ),
      ( u"MCR_SXE", 5, 5 ),
      ( u"MCR_SWE", 4, 4 ),
      ( u"MCR_SRE", 3, 3 ),
      ( u"MCR_UXE", 2, 2 ),
      ( u"MCR_UWE", 1, 1 ),
      ( u"MCR_URE", 0, 0 ),
      ( u"MCI_SPID", 0, 4 ),
      ( u"MPIDX_IDX", 0, 4 ),
      ( u"MPBK_BK", 0, 1 ),
      ( u"MPAT_WMPID7", 31, 31 ),
      ( u"MPAT_WMPID6", 30, 30 ),
      ( u"MPAT_WMPID5", 29, 29 ),
      ( u"MPAT_WMPID4", 28, 28 ),
      ( u"MPAT_WMPID3", 27, 27 ),
      ( u"MPAT_WMPID2", 26, 26 ),
      ( u"MPAT_WMPID1", 25, 25 ),
      ( u"MPAT_WMPID0", 24, 24 ),
      ( u"MPAT_RMPID7", 23, 23 ),
      ( u"MPAT_RMPID6", 22, 22 ),
      ( u"MPAT_RMPID5", 21, 21 ),
      ( u"MPAT_RMPID4", 20, 20 ),
      ( u"MPAT_RMPID3", 19, 19 ),
      ( u"MPAT_RMPID2", 18, 18 ),
      ( u"MPAT_RMPID1", 17, 17 ),
      ( u"MPAT_RMPID0", 16, 16 ),
      ( u"MPAT_WG", 15, 15 ),
      ( u"MPAT_RG", 14, 14 ),
      ( u"MPAT_E", 7, 7 ),
      ( u"MPAT_SX", 5, 5 ),
      ( u"MPAT_SW", 4, 4 ),
      ( u"MPAT_SR", 3, 3 ),
      ( u"MPAT_UX", 2, 2 ),
      ( u"MPAT_UW", 1, 1 ),
      ( u"MPAT_UR", 0, 0 ),
      ( u"MPID0_SPID", 0, 4 ),
      ( u"MPID1_SPID", 0, 4 ),
      ( u"MPID2_SPID", 0, 4 ),
      ( u"MPID3_SPID", 0, 4 ),
      ( u"MPID4_SPID", 0, 4 ),
      ( u"MPID5_SPID", 0, 4 ),
      ( u"MPID6_SPID", 0, 4 ),
      ( u"MPID7_SPID", 0, 4 ),
      ( u"ICTAGL_LPN", 11, 31 ),
      ( u"ICTAGL_LRU", 4, 5 ),
      ( u"ICTAGL_L", 2, 2 ),
      ( u"ICTAGL_V", 0, 0 ),
      ( u"ICTAGH_WD", 31, 31 ),
      ( u"ICTAGH_PD", 30, 30 ),
      ( u"ICTAGH_WT", 29, 29 ),
      ( u"ICTAGH_PT", 28, 28 ),
      ( u"ICTAGH_DATAECC", 16, 23 ),
      ( u"ICTAGH_TAGECC", 8, 15 ),
      ( u"ICCTRL_PBS", 16, 16 ),
      ( u"ICCTRL_ICHCLR", 8, 8 ),
      ( u"ICCTRL_ICHEIV", 2, 2 ),
      ( u"ICCTRL_ICHEMK", 1, 1 ),
      ( u"ICCTRL_ICHEN", 0, 0 ),
      ( u"ICCFG_ICHSIZE", 8, 14 ),
      ( u"ICCFG_ICHLINE", 4, 7 ),
      ( u"ICCFG_ICHWAY", 0, 3 ),
      ( u"ICERR_CISTW", 31, 31 ),
      ( u"ICERR_ESAFE", 30, 30 ),
      ( u"ICERR_ESMH", 29, 29 ),
      ( u"ICERR_ESPBSE", 28, 28 ),
      ( u"ICERR_ESTE1", 27, 27 ),
      ( u"ICERR_ESTE2", 26, 26 ),
      ( u"ICERR_ESDC", 25, 25 ),
      ( u"ICERR_ESDE", 24, 24 ),
      ( u"ICERR_ERMAFE", 22, 22 ),
      ( u"ICERR_ERMMH", 21, 21 ),
      ( u"ICERR_ERMPBSE", 20, 20 ),
      ( u"ICERR_ERMTE1", 19, 19 ),
      ( u"ICERR_ERMTE2", 18, 18 ),
      ( u"ICERR_ERMDC", 17, 17 ),
      ( u"ICERR_ERMDE", 16, 16 ),
      ( u"ICERR_ICHEWY", 13, 15 ),
      ( u"ICERR_ICHEIX", 5, 12 ),
      ( u"ICERR_ICHERQ", 4, 4 ),
      ( u"ICERR_ICHED", 3, 3 ),
      ( u"ICERR_ICHET", 2, 2 ),
      ( u"ICERR_ICHERR", 0, 0 )
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
      ( u"wr0", None, 64 ),
      ( u"wr0_u", None, 64 ),
      ( u"wr1", None, 64 ),
      ( u"wr1_u", None, 64 ),
      ( u"wr2", None, 64 ),
      ( u"wr2_u", None, 64 ),
      ( u"wr3", None, 64 ),
      ( u"wr3_u", None, 64 ),
      ( u"wr4", None, 64 ),
      ( u"wr4_u", None, 64 ),
      ( u"wr5", None, 64 ),
      ( u"wr5_u", None, 64 ),
      ( u"wr6", None, 64 ),
      ( u"wr6_u", None, 64 ),
      ( u"wr7", None, 64 ),
      ( u"wr7_u", None, 64 ),
      ( u"wr8", None, 64 ),
      ( u"wr8_u", None, 64 ),
      ( u"wr9", None, 64 ),
      ( u"wr9_u", None, 64 ),
      ( u"wr10", None, 64 ),
      ( u"wr10_u", None, 64 ),
      ( u"wr11", None, 64 ),
      ( u"wr11_u", None, 64 ),
      ( u"wr12", None, 64 ),
      ( u"wr12_u", None, 64 ),
      ( u"wr13", None, 64 ),
      ( u"wr13_u", None, 64 ),
      ( u"wr14", None, 64 ),
      ( u"wr14_u", None, 64 ),
      ( u"wr15", None, 64 ),
      ( u"wr15_u", None, 64 ),
      ( u"wr16", None, 64 ),
      ( u"wr16_u", None, 64 ),
      ( u"wr17", None, 64 ),
      ( u"wr17_u", None, 64 ),
      ( u"wr18", None, 64 ),
      ( u"wr18_u", None, 64 ),
      ( u"wr19", None, 64 ),
      ( u"wr19_u", None, 64 ),
      ( u"wr20", None, 64 ),
      ( u"wr20_u", None, 64 ),
      ( u"wr21", None, 64 ),
      ( u"wr21_u", None, 64 ),
      ( u"wr22", None, 64 ),
      ( u"wr22_u", None, 64 ),
      ( u"wr23", None, 64 ),
      ( u"wr23_u", None, 64 ),
      ( u"wr24", None, 64 ),
      ( u"wr24_u", None, 64 ),
      ( u"wr25", None, 64 ),
      ( u"wr25_u", None, 64 ),
      ( u"wr26", None, 64 ),
      ( u"wr26_u", None, 64 ),
      ( u"wr27", None, 64 ),
      ( u"wr27_u", None, 64 ),
      ( u"wr28", None, 64 ),
      ( u"wr28_u", None, 64 ),
      ( u"wr29", None, 64 ),
      ( u"wr29_u", None, 64 ),
      ( u"wr30", None, 64 ),
      ( u"wr30_u", None, 64 ),
      ( u"wr31", None, 64 ),
      ( u"wr31_u", None, 64 ),
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
          u"PSW_EBV", 
          u"PSW_CU0", 
          u"PSW_CU1", 
          u"PSW_CU2", 
          u"PSW_EIMASK", 
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
      ( u"EIIC", None, 32 ),
      ( u"FEIC", None, 32 ),
      (
        u"PSWH",
        [
          u"PSWH_GPID",
          u"PSWH_GM"
        ],
        32
      ),
      ( u"CTPC", None, 32 ),
      ( u"CTPSW", None, 32 ),
      (
        u"EIPSWH",
        [
          u"EIPSWH_GPID",
          u"EIPSWH_GM"
        ],
        32
      ),
      (
        u"FEPSWH",
        [
          u"FEPSWH_GPID",
          u"FEPSWH_GM"
        ],
        32
      ),
      ( u"CTBP", None, 32 ),
      ( u"SNZCFG", None, 32 ),
      ( u"EIWR", None, 32 ),
      ( u"FEWR", None, 32 ),
      ( u"SPID", None, 32 ),
      ( u"SPIDLIST", None, 32 ),
      (
        u"RBASE",
        [
          u"RBASE_RBASE",
          u"RBASE_DV",
          u"RBASE_RINT"
        ],
        32
      ),
      (
        u"EBASE",
        [
          u"EBASE_EBASE",
          u"EBASE_DV",
          u"EBASE_RINT"
        ],
        32
      ),
      ( u"INTBP", None, 32 ),
      (
        u"MCTL",
        [
          u"MCTL_UIC"
        ],
        32
      ),
      ( u"PID", None, 32 ),
      ( u"SVLOCK", None, 32),
      (
        u"SCCFG",
        [
          u"SCCFG_SIZE"
        ],
        32
      ),
      ( u"SCBP", None, 32 ),
      (
        u"HVCFG",
        [
          u"HVCFG_HVE"
        ],
        32
      ),
      (
        u"GMCFG",
        [
          u"GMCFG_GMP",
          u"GMCFG_HMP",
          u"GMCFG_GSYSE",
          u"GMCFG_GCU0",
          u"GMCFG_GCU1",
          u"GMCFG_GCU2"
        ],
        32
      ),
      ( u"HVSB", None, 32 ),
      ( u"PEID", None, 32 ),
      (
        u"BMID",
        [
          u"BMID_BMID"
        ],
        32
      ),
      ( u"MEA", None, 32 ),
      ( u"MEI", None, 32 ),
      (
        u"RBCR0",
        [
          u"RBCR0_MD",
          u"RBCR0_BE"
        ],
        32
      ),
      (
        u"RBCR1",
        [
          u"RBCR1_NC"
        ],
        32
      ),
      (
        u"RBNR",
        [
          u"RBNR_BN"
        ],
        32
      ),
      (
        u"RBIP",
        [
          u"RBIP_RBIP"
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
        u"IMSR",
        [
          u"IMSR_EEIM",
          u"IMSR_EPLM",
          u"IMSR_EID",
          u"IMSR_ENP",
          u"IMSR_FNP",
          u"IMSR_HEEIM",
          u"IMSR_HEPLM",
          u"IMSR_HEID",
          u"IMSR_HENP",
          u"IMSR_HFNP"
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
          u"INTCFG_ISPC",
          u"INTCFG_EPL",
          u"INTCFG_ULNR"
        ],
        32
      ),
      (
        u"PLMR",
        [
          u"PLMR_PLM"
        ],
        32
      ),
      (
        u"FXSR",
        [
          u"FXSR_FN",
          u"FXSR_IF",
          u"FXSR_PEM",
          u"FXSR_RM",
          u"FXSR_FS",
          u"FXSR_XC",
          u"FXSR_XE",
          u"FXSR_XP"
        ],
        32
      ),
      (
        u"FXST",
        [
          u"FXST_XC",
          u"FXST_IF",
          u"FXST_XP"
        ],
        32
      ),
      (
        u"FXINFO",
        [
          u"FXINFO_NFPU",
          u"FXINFO_RSIZE"
        ],
        32
      ),
      (
        u"FXCFG",
        [
          u"FXCFG_RM",
          u"FXCFG_XE"
        ],
        32
      ),
      (
        u"FXXC",
        [
          u"FXXC_XC3",
          u"FXXC_XC2",
          u"FXXC_XC1",
          u"FXXC_XC0"
        ],
        32
      ),

      (
        u"FXXP",
        [
          u"FXXP_XP3",
          u"FXXP_XP2",
          u"FXXP_XP1",
          u"FXXP_XP0"
        ],
        32
      ),      
      ( u"GMEIPC", None, 32 ),
      ( u"GMEIPSW", None, 32 ),
      ( u"GMFEPC", None, 32 ),
      ( u"GMFEPSW", None, 32 ),
      (
        u"GMPSW",
        [
          u"GMPSW_Z",
          u"GMPSW_S",
          u"GMPSW_OV",
          u"GMPSW_CY",
          u"GMPSW_SAT",
          u"GMPSW_ID",
          u"GMPSW_EP",
          u"GMPSW_NP",
          u"GMPSW_EBV", 
          u"GMPSW_CU0", 
          u"GMPSW_CU1", 
          u"GMPSW_CU2", 
          u"GMPSW_EIMASK", 
          u"GMPSW_UM"
        ],
        32
      ),
      ( u"GMMEA", None, 32 ),
      ( u"GMMEI", None, 32 ),
      ( u"GMEIIC", None, 32 ),
      ( u"GMFEIC", None, 32 ),
      ( u"GMSPID", None, 32 ),
      ( u"GMSPIDLIST", None, 32 ),
      (
        u"GMEBASE",
        [
          u"GMEBASE_EBASE",
          u"GMEBASE_DV",
          u"GMEBASE_RINT"
        ],
        32
      ),
      ( u"GMINTBP", None, 32 ),
      (
        u"GMINTCFG",
        [
          u"GMINTCFG_ISPC",
          u"GMINTCFG_EPL",
          u"GMINTCFG_ULNR"
        ],
        32
      ),
      (
        u"GMPLMR",
        [
          u"GMPLMR_PLM"
        ],
        32
      ),
      ( u"GMSVLOCK", None, 32 ),
      ( u"GMMPM", None, 32), 
      ( u"GMEIWR", None, 32 ),
      ( u"GMFEWR", None, 32 ),
      ( u"GMPEID", None, 32 ),
      ( u"MPM", None, 32 ),
      (
        u"MPCFG",
        [
          u"MPCFG_NMPUE",
          u"MPCFG_HBE",
          u"MPCFG_ARCH",
          u"MPCFG_NBK",
          u"MPCFG_DMDP"
        ],
        32
      ),
      ( u"MCA", None, 32 ),
      ( u"MCS", None, 32 ),
      ( u"MCC", None, 32 ),
      (
        u"MCR",
        [
          u"MCR_HSXE",
          u"MCR_HSWE",
          u"MCR_HSRE",
          u"MCR_HUXE",
          u"MCR_HUWE",
          u"MCR_HURE",
          u"MCR_GSXE",
          u"MCR_GSWE",
          u"MCR_GSRE",
          u"MCR_GUXE",
          u"MCR_GUWE",
          u"MCR_GURE",
          u"MCR_OV",
          u"MCR_SXE",
          u"MCR_SWE",
          u"MCR_SRE",
          u"MCR_UXE",
          u"MCR_UWE",
          u"MCR_URE"
        ],
        32
      ),
      (
        u"MCI",
        [
          u"MCI_SPID"
        ],
        32
      ),
      (
        u"MPIDX",
        [
          u"MPIDX_IDX"
        ],
        32
      ),
      (
        u"MPBK",
        [
          u"MPBK_BK"
        ],
        32
      ),
      ( u"MPLA", None, 32 ),
      ( u"MPUA", None, 32 ),
      (
        u"MPAT",
        [
          u"MPAT_WMPID7",
          u"MPAT_WMPID6",
          u"MPAT_WMPID5",
          u"MPAT_WMPID4",
          u"MPAT_WMPID3",
          u"MPAT_WMPID2",
          u"MPAT_WMPID1",
          u"MPAT_WMPID0",
          u"MPAT_RMPID7",
          u"MPAT_RMPID6",
          u"MPAT_RMPID5",
          u"MPAT_RMPID4",
          u"MPAT_RMPID3",
          u"MPAT_RMPID2",
          u"MPAT_RMPID1",
          u"MPAT_RMPID0",
          u"MPAT_WG",
          u"MPAT_RG",
          u"MPAT_E",
          u"MPAT_SX",
          u"MPAT_SW",
          u"MPAT_SR",
          u"MPAT_UX",
          u"MPAT_UW",
          u"MPAT_UR"
        ],
        32
      ),
      (
        u"MPID0",
        [
          u"MPID0_SPID"
        ],
        32
      ),
      (
        u"MPID1",
        [
          u"MPID1_SPID"
        ],
        32
      ),
      (
        u"MPID2",
        [
          u"MPID2_SPID"
        ],
        32
      ),
      (
        u"MPID3",
        [
          u"MPID3_SPID"
        ],
        32
      ),
      (
        u"MPID4",
        [
          u"MPID4_SPID"
        ],
        32
      ),
      (
        u"MPID5",
        [
          u"MPID5_SPID"
        ],
        32
      ),
      (
        u"MPID6",
        [
          u"MPID6_SPID"
        ],
        32
      ),
      (
        u"MPID7",
        [
          u"MPID7_SPID"
        ],
        32
      ),
      (
        u"ICTAGL",
        [
          u"ICTAGL_LPN",
          u"ICTAGL_LRU",
          u"ICTAGL_L",
          u"ICTAGL_V"
        ],
        32
      ),
      (
        u"ICTAGH",
        [
          u"ICTAGH_WD",
          u"ICTAGH_PD",
          u"ICTAGH_WT",
          u"ICTAGH_PT",
          u"ICTAGH_DATAECC",
          u"ICTAGH_TAGECC"
        ],
        32
      ),
      ( u"ICDATL", None, 32 ),
      ( u"ICDATH", None, 32 ),
      (
        u"ICCTRL",
        [
          u"ICCTRL_PBS",
          u"ICCTRL_ICHCLR",
          u"ICCTRL_ICHEIV",
          u"ICCTRL_ICHEMK",
          u"ICCTRL_ICHEN"
        ],
        32
      ),
      (
        u"ICCFG",
        [
          u"ICCFG_ICHSIZE",
          u"ICCFG_ICHLINE",
          u"ICCFG_ICHWAY"
        ],
        32
      ),
      (
        u"ICERR",
        [
          u"ICERR_CISTW",
          u"ICERR_ESAFE",
          u"ICERR_ESMH",
          u"ICERR_ESPBSE",
          u"ICERR_ESTE1",
          u"ICERR_ESTE2",
          u"ICERR_ESDC",
          u"ICERR_ESDE",
          u"ICERR_ERMAFE",
          u"ICERR_ERMMH",
          u"ICERR_ERMPBSE",
          u"ICERR_ERMTE1",
          u"ICERR_ERMTE2",
          u"ICERR_ERMDC",
          u"ICERR_ERMDE",
          u"ICERR_ICHEWY",
          u"ICERR_ICHEIX",
          u"ICERR_ICHERQ",
          u"ICERR_ICHED",
          u"ICERR_ICHET",
          u"ICERR_ICHERR"
        ],
        32
      ),
      ( u"TSCOUNTL", None, 32 ),
      ( u"TSCOUNTH", None, 32 ),
      ( u"TSCTRL", None, 32 ),
      ( u"PMUMCTRL", None, 32 ),
      ( u"PMGMCTRL", None, 32 ),
      ( u"PMCOUNT0", None, 32 ),
      ( u"PMCTRL0", None, 32 ),
      ( u"PMCOUNT1", None, 32 ),
      ( u"PMCTRL1", None, 32 ),
      ( u"PMCOUNT2", None, 32 ),
      ( u"PMCTRL2", None, 32 ),
      ( u"PMCOUNT3", None, 32 ),
      ( u"PMCTRL3", None, 32 ),
      ( u"PMCTRL4", None, 32 ),
      ( u"PMCTRL5", None, 32 ),
      ( u"PMCTRL6", None, 32 ),
      ( u"PMCTRL7", None, 32 ),
      ( u"PMCOUNT4", None, 32 ),
      ( u"PMCOUNT5", None, 32 ),
      ( u"PMCOUNT6", None, 32 ),
      ( u"PMCOUNT7", None, 32 ),
      ( u"PMSUBCND0", None, 32 ),
      ( u"PMSUBCND1", None, 32 ),
      ( u"PMSUBCND2", None, 32 ),
      ( u"PMSUBCND3", None, 32 ),
      ( u"PMSUBCND4", None, 32 ),
      ( u"PMSUBCND5", None, 32 ),
      ( u"PMSUBCND6", None, 32 ),
      ( u"PMSUBCND7", None, 32 ),
      ( u"DBGEN", None, 32 ),
      ( u"DBPSWH", None, 32 ),
      ( u"DBIC", None, 32 ),
      ( u"DBPC", None, 32 ),
      ( u"DBPSW", None, 32 ),
      ( u"DIR0", None, 32 ),
      ( u"DIR1", None, 32 ),
      ( u"BPC", None, 32 ),
      ( u"BPAV", None, 32 ),
      ( u"BPAM", None, 32 ),
      ( u"DBCFG", None, 32 ),
      ( u"DBWR", None, 32 ),
      ( u"LSTEST0", None, 32 ),
      ( u"LSTEST1", None, 32 ),
      ( u"LSCFG", None, 32 ),
      ( u"ICBKEY", None, 32 ),
      ( u"IFCR", None, 32 ),
      ( u"IFCR1", None, 32 ),
      ( u"BRPCTRL0", None, 32 ),
      ( u"BRPCTRL1", None, 32 ),
      ( u"BRPCFG", None, 32 ),
      ( u"BRPACTRL", None, 32 ),
      ( u"BRPDATA", None, 32 ),
      ( u"DCBKEY", None, 32 ),
      ( u"LSUCR", None, 32 ),
      ( u"LSULNK0", None, 32 ),
      ( u"LSULNK1", None, 32 ),
      ( u"L1RLCR", None, 32 ),
      ( u"L1RLNK0", None, 32 ),
      ( u"L1RLNK1", None, 32 ),
      ( u"L1RCFG", None, 32 ),
      ( u"DECFG", None, 32 ),
      ( u"DECTRL", None, 32 ),
      ( u"DEVDS", None, 32 ),
      ( u"RDBCR", None, 32 ),
      ( u"RDBACR", None, 32 ),
      ( u"RDBATAG", None, 32 ),
      ( u"RDBADAT0", None, 32 ),
      ( u"RDBADAT1", None, 32 ),
      ( u"RDBADAT2", None, 32 ),
      ( u"RDBADAT3", None, 32 ),
      ( u"RDBSTAT", None, 32 ),
      ( u"hv_func_support", None, 8 )
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
        u"WR",
        [
          ( 0, u"wr0" ),
          ( 1, u"wr0_u" ),
          ( 2, u"wr1" ),
          ( 3, u"wr1_u" ),
          ( 4, u"wr2" ),
          ( 5, u"wr2_u" ),
          ( 6, u"wr3" ),
          ( 7, u"wr3_u" ),
          ( 8, u"wr4" ),
          ( 9, u"wr4_u" ),
          ( 10, u"wr5" ),
          ( 11, u"wr5_u" ),
          ( 12, u"wr6" ),
          ( 13, u"wr6_u" ),
          ( 14, u"wr7" ),
          ( 15, u"wr7_u" ),
          ( 16, u"wr8" ),
          ( 17, u"wr8_u" ),
          ( 18, u"wr9" ),
          ( 19, u"wr9_u" ),
          ( 20, u"wr10" ),
          ( 21, u"wr10_u" ),
          ( 22, u"wr11" ),
          ( 23, u"wr11_u" ),
          ( 24, u"wr12" ),
          ( 25, u"wr12_u" ),
          ( 26, u"wr13" ),
          ( 27, u"wr13_u" ),
          ( 28, u"wr14" ),
          ( 29, u"wr14_u" ),
          ( 30, u"wr15" ),
          ( 31, u"wr15_u" ),
          ( 32, u"wr16" ),
          ( 33, u"wr16_u" ),
          ( 34, u"wr17" ),
          ( 35, u"wr17_u" ),
          ( 36, u"wr18" ),
          ( 37, u"wr18_u" ),
          ( 38, u"wr19" ),
          ( 39, u"wr19_u" ),
          ( 40, u"wr20" ),
          ( 41, u"wr20_u" ),
          ( 42, u"wr21" ),
          ( 43, u"wr21_u" ),
          ( 44, u"wr22" ),
          ( 45, u"wr22_u" ),
          ( 46, u"wr23" ),
          ( 47, u"wr23_u" ),
          ( 48, u"wr24" ),
          ( 49, u"wr24_u" ),
          ( 50, u"wr25" ),
          ( 51, u"wr25_u" ),
          ( 52, u"wr26" ),
          ( 53, u"wr26_u" ),
          ( 54, u"wr27" ),
          ( 55, u"wr27_u" ),
          ( 56, u"wr28" ),
          ( 57, u"wr28_u" ),
          ( 58, u"wr29" ),
          ( 59, u"wr29_u" ),
          ( 60, u"wr30" ),
          ( 61, u"wr30_u" ),
          ( 62, u"wr31" ),
          ( 63, u"wr31_u" )
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
          ( 13, u"EIIC" ),
          ( 14, u"FEIC" ),
          ( 15, u"PSWH" ),
          ( 16, u"CTPC" ),
          ( 17, u"CTPSW" ),
          ( 18, u"EIPSWH" ),
          ( 19, u"FEPSWH" ),
          ( 20, u"CTBP" ),
          ( 21, u"SNZCFG" ),
          ( 28, u"EIWR" ),
          ( 29, u"FEWR" )
        ]
      ),
      (
        u"SR_SELID_1",
        [
          ( 0, u"SPID" ),
          ( 1, u"SPIDLIST" ),
          ( 2, u"RBASE" ),
          ( 3, u"EBASE" ),
          ( 4, u"INTBP" ),
          ( 5, u"MCTL" ),
          ( 6, u"PID" ),
          ( 8, u"SVLOCK" ),
          ( 11, u"SCCFG" ),
          ( 12, u"SCBP" ),
          ( 16, u"HVCFG" ),
          ( 17, u"GMCFG" ),
          ( 20, u"HVSB" )
       ]
      ),
      (
        u"SR_SELID_2",
        [
          ( 0, u"PEID" ),
          ( 1, u"BMID" ),
          ( 6, u"MEA" ),
          ( 8, u"MEI" ),
          ( 10, u"ISPR" ),
          ( 11, u"IMSR" ),
          ( 12, u"ICSR" ),
          ( 13, u"INTCFG" ),
          ( 14, u"PLMR" ),
          ( 15, u"RBCR0" ),
          ( 16, u"RBCR1" ),
          ( 17, u"RBNR" ),
          ( 18, u"RBIP" )
        ]
      ),
      (
        u"SR_SELID_3",
        [
          ( 0, u"DBGEN" ),
          ( 13, u"DBPSWH" ),
          ( 15, u"DBIC" ),
          ( 18, u"DBPC" ),
          ( 19, u"DBPSW" ),
          ( 20, u"DIR0" ),
          ( 21, u"DIR1" ),
          ( 22, u"BPC" ),
          ( 24, u"BPAV" ),
          ( 25, u"BPAM" ),
          ( 28, u"DBCFG" ),
          ( 30, u"DBWR" )
        ]
      ),
      (
        u"SR_SELID_4",
        [
          ( 16, u"ICTAGL" ),
          ( 17, u"ICTAGH" ),
          ( 18, u"ICDATL" ),
          ( 19, u"ICDATH" ),
          ( 24, u"ICCTRL" ),
          ( 26, u"ICCFG" ),
          ( 28, u"ICERR" )
        ]
      ),
      (
        u"SR_SELID_5",
        [
          ( 0, u"MPM" ),
          ( 2, u"MPCFG" ),
          ( 8, u"MCA" ),
          ( 9, u"MCS" ),
          ( 10, u"MCC" ),
          ( 11, u"MCR" ),
          ( 12, u"MCI" ),
          ( 16, u"MPIDX" ),
          ( 17, u"MPBK" ),
          ( 20, u"MPLA" ),
          ( 21, u"MPUA" ),
          ( 22, u"MPAT" ),
          ( 24, u"MPID0" ),
          ( 25, u"MPID1" ),
          ( 26, u"MPID2" ),
          ( 27, u"MPID3" ),
          ( 28, u"MPID4" ),
          ( 29, u"MPID5" ),
          ( 30, u"MPID6" ),
          ( 31, u"MPID7" )
        ]
      ),
      (
        u"SR_SELID_9",
        [
          ( 0, u"GMEIPC" ),
          ( 1, u"GMEIPSW" ),
          ( 2, u"GMFEPC" ),
          ( 3, u"GMFEPSW" ),
          ( 5, u"GMPSW" ),
          ( 6, u"GMMEA" ),
          ( 8, u"GMMEI" ),
          ( 13, u"GMEIIC" ),
          ( 14, u"GMFEIC" ),
          ( 16, u"GMSPID" ),
          ( 17, u"GMSPIDLIST" ),
          ( 19, u"GMEBASE" ),
          ( 20, u"GMINTBP" ),
          ( 21, u"GMINTCFG" ),
          ( 22, u"GMPLMR" ),
          ( 24, u"GMSVLOCK" ),
          ( 25, u"GMMPM" ),
          ( 28, u"GMEIWR" ),
          ( 29, u"GMFEWR" ),
          ( 30, u"GMPEID" )
        ]
      ),
      (
        u"SR_SELID_10",
        [
          ( 6, u"FXSR" ),
          ( 8, u"FXST" ),
          ( 9, u"FXINFO" ),
          ( 10, u"FXCFG" ),
          ( 12, u"FXXC" ),
          ( 13, u"FXXP" )
        ]
      ),
      (
        u"SR_SELID_11",
        [
          ( 0, u"TSCOUNTL" ),
          ( 1, u"TSCOUNTH" ),
          ( 2, u"TSCTRL" ),
          ( 8, u"PMUMCTRL" ),
          ( 9, u"PMGMCTRL" ),
          ( 16, u"PMCOUNT0" ),
          ( 17, u"PMCTRL0" ),
          ( 18, u"PMCOUNT1" ),
          ( 19, u"PMCTRL1" ),
          ( 20, u"PMCOUNT2" ),
          ( 21, u"PMCTRL2" ),
          ( 22, u"PMCOUNT3" ),
          ( 23, u"PMCTRL3" )
        ]
      ),
      (
        u"SR_SELID_12",
        [
          ( 0, u"LSTEST0" ),
          ( 1, u"LSTEST1" ),
          ( 2, u"LSCFG" ),
          ( 4, u"ICBKEY" ),
          ( 5, u"IFCR" ),
          ( 6, u"IFCR1" ),
          ( 8, u"BRPCTRL0" ),
          ( 9, u"BRPCTRL1" ),
          ( 10, u"BRPCFG" ),
          ( 11, u"BRPACTRL" ),
          ( 12, u"BRPDATA" )
        ]
      ),
      (
        u"SR_SELID_13",
        [
          ( 0, u"DCBKEY" ),
          ( 1, u"LSUCR" ),
          ( 2, u"LSULNK0" ),
          ( 3, u"LSULNK1" ),
          ( 8, u"L1RLCR" ),
          ( 9, u"L1RLNK0" ),
          ( 10, u"L1RLNK1" ),
          ( 12, u"L1RCFG" ),
          ( 16, u"DECFG" ),
          ( 17, u"DECTRL" ),
          ( 18, u"DEVDS" ),
          ( 24, u"RDBCR" ),
          ( 25, u"RDBACR" ),
          ( 26, u"RDBATAG" ),
          ( 27, u"RDBADAT0" ),
          ( 28, u"RDBADAT1" ),
          ( 29, u"RDBADAT2" ),
          ( 30, u"RDBADAT3" ),
          ( 31, u"RDBSTAT" )
        ]
      ),
      (
        u"SR_SELID_14",
        [
          ( 4, u"PMCTRL4" ),
          ( 5, u"PMCTRL5" ),
          ( 6, u"PMCTRL6" ),
          ( 7, u"PMCTRL7" ),
          ( 20, u"PMCOUNT4" ),
          ( 21, u"PMCOUNT5" ),
          ( 22, u"PMCOUNT6" ),
          ( 23, u"PMCOUNT7" )
        ]
      ),
      (
        u"SR_SELID_15",
        [
          ( 0, u"PMSUBCND0" ),
          ( 1, u"PMSUBCND1" ),
          ( 2, u"PMSUBCND2" ),
          ( 3, u"PMSUBCND3" ),
          ( 4, u"PMSUBCND4" ),
          ( 5, u"PMSUBCND5" ),
          ( 6, u"PMSUBCND6" ),
          ( 7, u"PMSUBCND7" )
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
      ( u"cacheop", 8, 0, 0, None, [ ( 11, 12 ), ( 27, 31 ) ] ),
      ( u"prefop", 8, 0, 0, None, [ ( 27, 31 ) ] ),
      ( u"imm10", 16, 0, 0, None, [ ( 27, 31 ), ( 0, 4 ) ] ),
      ( u"disp16", 32, 0, 1, None, [ ( 17, 31 ) ] ),
      ( u"wreg1", 8, 0, 0, u"WR", [ ( 0, 4 ) ] ),
      ( u"wreg2", 8, 0, 0, u"WR", [ ( 11, 15 ) ] ),
      ( u"wreg2", 8, 0, 0, u"WR", [ ( 43, 47 ) ] ),
      ( u"wreg3", 8, 0, 0, u"WR", [ ( 27, 31 ) ] ),
      ( u"wreg4", 8, 0, 0, u"WR", [ ( 32, 36 ) ] ),
      ( u"fcond", 8, 0, 0, None, [ ( 17, 20 ) ] ),
      ( u"disp16", 32, 1, 2, None, [ ( 34, 47 ) ] ),
      ( u"disp16", 32, 1, 3, None, [ ( 35, 47 ) ] ),
      ( u"disp16", 32, 1, 4, None, [ ( 36, 47 ) ] ),
      ( u"imm1", 32, 0, 0, None, [ ( 21, 21 ) ] ),
      ( u"imm2", 32, 0, 0, None, [ ( 0, 1 ) ] ),
      ( u"imm2", 32, 0, 0, None, [ ( 21, 22 ) ] ),
      ( u"imm3", 32, 0, 0, None, [ ( 27, 29 ) ] ),
      ( u"imm4", 32, 0, 0, None, [ ( 21, 24 ) ] ),
      ( u"imm12", 32, 0, 0, None, [ ( 21, 21 ), ( 32, 42 ) ] ),
      ( u"et", 8, 0, 0, u"SR_SELID_5", [ ( 27, 31 ) ] ),
      ( u"eh", 8, 0, 0, u"SR_SELID_5", [ ( 11, 14 ) ] )
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
        [
          ( u"cycle_count", 2 )
        ],
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
          ( u"cycle_count", 17 )
        ],
        [ 24 ]
      ),
      (
        u"caxi [{reg1}], {reg2}, {reg3}",
        u"-----00011101110-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"clr1 {bit}#3, {disp16} [{reg1}]",
        u"----------------10---111110-----",
        [ 0, 12, 32 ]
      ),
      (
        u"clr1 {reg2}, [{reg1}]",
        u"0000000011100100-----111111-----",
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
          ( u"cycle_count", 8 )
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
          ( u"cycle_count", 1 )
        ],
        [ 23, 31 ]
      ),
      (
        u"dispose {imm5}, {list12}, [{reg1}]",
        u"----------------0000011001------",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 8 )
        ],
        [ 23, 31, 33 ]
      ),
      (
        u"div {reg1}, {reg2}, {reg3}",
        u"-----01011000000-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"switch {reg1}",
        u"00000000010-----",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 11 )
        ],
        [ 32 ]
      ),
      (
        u"divh {reg1}, {reg2}",
        u"-----000010-----",
        [ 32, 35 ]
      ),
      (
        u"divh {reg1}, {reg2}, {reg3}",
        u"-----01010000000-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"divhu {reg1}, {reg2}, {reg3}",
        u"-----01010000010-----111111-----",
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
          ( u"cycle_count", 8 )
        ],
        None
      ),
      (
        u"feret",
        u"00000001010010100000011111100000",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 8 )
        ],
        None
      ),
      (
        u"fetrap {vector}",
        u"0----00001000000",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 8 )
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
        [
          ( u"cycle_count", 2 )
        ],
        [ 15 ]
      ),
      (
        u"jr {disp32}",
        u"-------------------------------00000001011100000",
        [
          ( u"cycle_count", 2 )
        ],
        [ 18 ]
      ),
      (
        u"jarl {disp22}, {reg2}",
        u"---------------0-----11110------",
        [
          ( u"cycle_count", 2 )
        ],
        [ 35, 15 ]
      ),
      (
        u"jarl {disp32}, {reg1}",
        u"-------------------------------000000010111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 32, 18 ]
      ),
      (
        u"jmp [{reg1}]",
        u"00000000011-----",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 2 )
        ],
        [ 32 ]
      ),
      (
        u"jmp {disp32}[{reg1}]",
        u"-------------------------------000000110111-----",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 2 )
        ],
        [ 32, 18 ]
      ),
      (
        u"prepare {list12}, {imm5}",
        u"-----------000010000011110------",
        [
          ( u"cycle_count", 1 )
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
        u"ld.b [{reg1}]+, {reg3}",
        u"-----0110111000000010111111-----",
        [ 32, 36 ]
      ),
      (
        u"ld.b [{reg1}]-, {reg3}",
        u"-----0110111000000100111111-----",
        [ 32, 36 ]
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
        u"ld.bu [{reg1}]+, {reg3}",
        u"-----0110111000000011111111-----",
        [ 32, 36 ]
      ),
      (
        u"ld.bu [{reg1}]-, {reg3}",
        u"-----0110111000000101111111-----",
        [ 32, 36 ]
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
        u"ld.h [{reg1}]+, {reg3}",
        u"-----0110111010000010111111-----",
        [ 32, 36 ]
      ),
      (
        u"ld.h [{reg1}]-, {reg3}",
        u"-----0110111010000100111111-----",
        [ 32, 36 ]
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
        u"ld.hu [{reg1}]+, {reg3}",
        u"-----0110111010000011111111-----",
        [ 32, 36 ]
      ),
      (
        u"ld.hu [{reg1}]-, {reg3}",
        u"-----0110111010000101111111-----",
        [ 32, 36 ]
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
        u"ld.w [{reg1}]+, {reg3}",
        u"-----0110111100000010111111-----",
        [ 32, 36 ]
      ),
      (
        u"ld.w [{reg1}]-, {reg3}",
        u"-----0110111100000100111111-----",
        [ 32, 36 ]
      ),
      (
        u"ldsr {reg2}, {regID}, {selID}",
        u"-----00000100000-----111111-----",
        [
          ( u"terminates_block", True )
        ],
        [ 34, 43, 50 ]
      ),
      (
        u"mac {reg1}, {reg2}, {reg3}, {reg4}",
        u"----0011110----0-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 32, 35, 36, 37 ]
      ),
      (
        u"macu {reg1}, {reg2}, {reg3}, {reg4}",
        u"----0011111----0-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
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
        [ 0, 12, 32 ]
      ),
      (
        u"not1 {reg2}, [{reg1}]",
        u"0000000011100010-----111111-----",
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
          ( u"cycle_count", 8 )
        ],
        None
      ),
      (
        u"rie {imm5}, {imm4}",
        u"0000000000000000-----1111111----",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 8 )
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
        [ 0, 12, 32 ]
      ),
      (
        u"set1 {reg2}, [{reg1}]",
        u"0000000011100000-----111111-----",
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
        u"st.b {reg3}, [{reg1}]+",
        u"-----0110111001000010111111-----",
        [ 36, 32 ]
      ),
      (
        u"st.b {reg3}, [{reg1}]-",
        u"-----0110111001000100111111-----",
        [ 36, 32 ]
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
        u"st.h {reg3}, [{reg1}]+",
        u"-----0110111011000010111111-----",
        [ 36, 32 ]
      ),
      (
        u"st.h {reg3}, [{reg1}]-",
        u"-----0110111011000100111111-----",
        [ 36, 32 ]
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
        u"st.w {reg3}, [{reg1}]+",
        u"-----0110111101000010111111-----",
        [ 36, 32 ]
      ),
      (
        u"st.w {reg3}, [{reg1}]-",
        u"-----0110111101000100111111-----",
        [ 36, 32 ]
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
          ( u"cycle_count", 17 )
        ],
        [ 45 ]
      ),
      (
        u"trap {vector}",
        u"000000010000000000000111111-----",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 8 )
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
        [ 0, 12, 32 ]
      ),
      (
        u"tst1 {reg2}, [{reg1}]",
        u"0000000011100110-----111111-----",
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
        u"absf.d {reg2}, {reg3}",
        u"----010001011000----011111100000",
        [ 40, 41 ]
      ),
      (
        u"absf.s {reg2}, {reg3}",
        u"-----10001001000-----11111100000",
        [ 35, 36 ]
      ),
      (
        u"addf.d {reg1}, {reg2}, {reg3}",
        u"----010001110000----0111111----0",
        [ 39, 40, 41 ]
      ),
      (
        u"addf.s {reg1}, {reg2}, {reg3}",
        u"-----10001100000-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"ceilf.dl {reg2}, {reg3}",
        u"----010001010100----011111100010",
        [ 40, 41 ]
      ),
      (
        u"ceilf.dul {reg2}, {reg3}",
        u"----010001010100----011111110010",
        [ 40, 41 ]
      ),
      (
        u"ceilf.duw {reg2}, {reg3}",
        u"-----10001010000----011111110010",
        [ 40, 36 ]
      ),
      (
        u"ceilf.dw {reg2}, {reg3}",
        u"-----10001010000----011111100010",
        [ 40, 36 ]
      ),
      (
        u"ceilf.sl {reg2}, {reg3}",
        u"----010001000100-----11111100010",
        [ 35, 41 ]
      ),
      (
        u"ceilf.sul {reg2}, {reg3}",
        u"----010001000100-----11111110010",
        [ 35, 41 ]
      ),
      (
        u"ceilf.suw {reg2}, {reg3}",
        u"-----10001000000-----11111110010",
        [ 35, 36 ]
      ),
      (
        u"ceilf.sw {reg2}, {reg3}",
        u"-----10001000000-----11111100010",
        [ 35, 36 ]
      ),
      (
        u"cmovf.d {fcbit}, {reg1}, {reg2}, {reg3}",
        u"----01000001---0----0111111----0",
        [ 39, 40, 4, 41 ]
      ),
      (
        u"cmovf.s {fcbit}, {reg1}, {reg2}, {reg3}",
        u"-----1000000---0-----111111-----",
        [ 32, 35, 4, 36 ]
      ),
      (
        u"cmpf.d {fcond}, {reg2}, {reg1}, {fcbit}",
        u"0----1000011---0----0111111----0",
        [ 39, 40, 4, 5 ]
      ),
      (
        u"cmpf.s {fcond}, {reg2}, {reg1}, {fcbit}",
        u"0----1000010---0-----111111-----",
        [ 32, 35, 4, 5 ]
      ),
      (
        u"cvtf.dl {reg2}, {reg3}",
        u"----010001010100----011111100100",
        [ 40, 41 ]
      ),
      (
        u"cvtf.ds {reg2}, {reg3}",
        u"-----10001010010----011111100011",
        [ 40, 36 ]
      ),
      (
        u"cvtf.dul {reg2}, {reg3}",
        u"----010001010100----011111110100",
        [ 40, 41 ]
      ),
      (
        u"cvtf.duw {reg2}, {reg3}",
        u"-----10001010000----011111110100",
        [ 40, 36 ]
      ),
      (
        u"cvtf.dw {reg2}, {reg3}",
        u"-----10001010000----011111100100",
        [ 40, 36 ]
      ),
      (
        u"cvtf.ld {reg2}, {reg3}",
        u"----010001010010----011111100001",
        [ 40, 41 ]
      ),
      (
        u"cvtf.ls {reg2}, {reg3}",
        u"-----10001000010----011111100001",
        [ 40, 36 ]
      ),
      (
        u"cvtf.sd {reg2}, {reg3}",
        u"----010001010010-----11111100010",
        [ 35, 41 ]
      ),
      (
        u"cvtf.sl {reg2}, {reg3}",
        u"----010001000100-----11111100100",
        [ 35, 41 ]
      ),
      (
        u"cvtf.sul {reg2}, {reg3}",
        u"----010001000100-----11111110100",
        [ 35, 41 ]
      ),
      (
        u"cvtf.suw {reg2}, {reg3}",
        u"-----10001000000-----11111110100",
        [ 35, 36 ]
      ),
      (
        u"cvtf.sw {reg2}, {reg3}",
        u"-----10001000000-----11111100100",
        [ 35, 36 ]
      ),
      (
        u"cvtf.uld {reg2}, {reg3}",
        u"----010001010010----011111110001",
        [ 40, 41 ]
      ),
      (
        u"cvtf.uls {reg2}, {reg3}",
        u"-----10001000010----011111110001",
        [ 40, 36 ]
      ),
      (
        u"cvtf.uwd {reg2}, {reg3}",
        u"----010001010010-----11111110000",
        [ 35, 41 ]
      ),
      (
        u"cvtf.uws {reg2}, {reg3}",
        u"-----10001000010-----11111110000",
        [ 35, 36 ]
      ),
      (
        u"cvtf.wd {reg2}, {reg3}",
        u"----010001010010-----11111100000",
        [ 35, 41 ]
      ),
      (
        u"cvtf.ws {reg2}, {reg3}",
        u"-----10001000010-----11111100000",
        [ 35, 36 ]
      ),
      (
        u"divf.d {reg1}, {reg2}, {reg3}",
        u"----010001111110----0111111----0",
        [
          ( u"cycle_count", 16 )
        ],
        [ 39, 40, 41 ]
      ),
      (
        u"divf.s {reg1}, {reg2}, {reg3}",
        u"-----10001101110-----111111-----",
        [
          ( u"cycle_count", 8 )
        ],
        [ 32, 35, 36 ]
      ),
      (
        u"floorf.dl {reg2}, {reg3}",
        u"----010001010100----011111100011",
        [ 40, 41 ]
      ),
      (
        u"floorf.dul {reg2}, {reg3}",
        u"----010001010100----011111110011",
        [ 40, 41 ]
      ),
      (
        u"floorf.duw {reg2}, {reg3}",
        u"-----10001010000----011111110011",
        [ 40, 36 ]
      ),
      (
        u"floorf.dw {reg2}, {reg3}",
        u"-----10001010000----011111100011",
        [ 40, 36 ]
      ),
      (
        u"floorf.sl {reg2}, {reg3}",
        u"----010001000100-----11111100011",
        [ 35, 41 ]
      ),
      (
        u"floorf.sul {reg2}, {reg3}",
        u"----010001000100-----11111110011",
        [ 35, 41 ]
      ),
      (
        u"floorf.suw {reg2}, {reg3}",
        u"-----10001000000-----11111110011",
        [ 35, 36 ]
      ),
      (
        u"floorf.sw {reg2}, {reg3}",
        u"-----10001000000-----11111100011",
        [ 35, 36 ]
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
        u"maxf.s {reg1}, {reg2}, {reg3}",
        u"-----10001101000-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"minf.d {reg1}, {reg2}, {reg3}",
        u"----010001111010----0111111----0",
        [ 39, 40, 41 ]
      ),
      (
        u"minf.s {reg1}, {reg2}, {reg3}",
        u"-----10001101010-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"msubf.s {reg1}, {reg2}, {reg3}, {reg4}",
        u"-----101-01----0-----111111-----",
        [ 32, 35, 36, 38 ]
      ),
      (
        u"mulf.d {reg1}, {reg2}, {reg3}",
        u"----010001110100----0111111----0",
        [
          ( u"cycle_count", 4 )
        ],
        [ 39, 40, 41 ]
      ),
      (
        u"mulf.s {reg1}, {reg2}, {reg3}",
        u"-----10001100100-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"negf.d {reg2}, {reg3}",
        u"----010001011000----011111100001",
        [ 40, 41 ]
      ),
      (
        u"negf.s {reg2}, {reg3}",
        u"-----10001001000-----11111100001",
        [ 35, 36 ]
      ),
      (
        u"nmaddf.s {reg1}, {reg2}, {reg3}, {reg4}",
        u"-----101-10----0-----111111-----",
        [ 32, 35, 36, 38 ]
      ),
      (
        u"nmsubf.s {reg1}, {reg2}, {reg3}, {reg4}",
        u"-----101-11----0-----111111-----",
        [ 32, 35, 36, 38 ]
      ),
      (
        u"recipf.d {reg2}, {reg3}",
        u"----010001011110----011111100001",
        [
          ( u"cycle_count", 16 )
        ],
        [ 40, 41 ]
      ),
      (
        u"recipf.s {reg2}, {reg3}",
        u"-----10001001110-----11111100001",
        [
          ( u"cycle_count", 8 )
        ],
        [ 35, 36 ]
      ),
      (
        u"rsqrtf.d {reg2}, {reg3}",
        u"----010001011110----011111100010",
        [
          ( u"cycle_count", 45 )
        ],
        [ 40, 41 ]
      ),
      (
        u"rsqrtf.s {reg2}, {reg3}",
        u"-----10001001110-----11111100010",
        [
          ( u"cycle_count", 21 )
        ],
        [ 35, 36 ]
      ),
      (
        u"sqrtf.d {reg2}, {reg3}",
        u"----010001011110----011111100000",
        [
          ( u"cycle_count", 30 )
        ],
        [ 40, 41 ]
      ),
      (
        u"sqrtf.s {reg2}, {reg3}",
        u"-----10001001110-----11111100000",
        [
          ( u"cycle_count", 14 )
        ],
        [ 35, 36 ]
      ),
      (
        u"subf.d {reg1}, {reg2}, {reg3}",
        u"----010001110010----0111111----0",
        [ 39, 40, 41 ]
      ),
      (
        u"subf.s {reg1}, {reg2}, {reg3}",
        u"-----10001100010-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"trfsr {fcbit}",
        u"000001000000---00000011111100000",
        [ 4 ]
      ),
      (
        u"trncf.dl {reg2}, {reg3}",
        u"----010001010100----011111100001",
        [ 40, 41 ]
      ),
      (
        u"trncf.dul {reg2}, {reg3}",
        u"----010001010100----011111110001",
        [ 40, 41 ]
      ),
      (
        u"trncf.duw {reg2}, {reg3}",
        u"-----10001010000----011111110001",
        [ 40, 36 ]
      ),
      (
        u"trncf.dw {reg2}, {reg3}",
        u"-----10001010000----011111100001",
        [ 40, 36 ]
      ),
      (
        u"trncf.sl {reg2}, {reg3}",
        u"----010001000100-----11111100001",
        [ 35, 41 ]
      ),
      (
        u"trncf.sul {reg2}, {reg3}",
        u"----010001000100-----11111110001",
        [ 35, 41 ]
      ),
      (
        u"trncf.suw {reg2}, {reg3}",
        u"-----10001000000-----11111110001",
        [ 35, 36 ]
      ),
      (
        u"trncf.sw {reg2}, {reg3}",
        u"-----10001000000-----11111100001",
        [ 35, 36 ]
      ),
      (
        u"b{cccc} {disp17}",
        u"---------------100000111111-----",
        [
          ( u"cycle_count", 2 )
        ],
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
          ( u"cycle_count", 2 )
        ],
        [ 32, 36 ]
      ),
      (
        u"ld.dw {disp23} [{reg1}], {reg3}",
        u"---------------------------0100100000111101-----",
        [ 17, 32, 36 ]
      ),
      (
        u"ldl.bu [{reg1}], {reg3}",
        u"-----0110111000000001111111-----",
        [
          ( u"cycle_count", 11 )
        ],
        [ 32, 36 ]
      ),
      (
        u"ldl.hu [{reg1}], {reg3}",
        u"-----0110111010000001111111-----",
        [
          ( u"cycle_count", 11 )
        ],
        [ 32, 36 ]
      ),
      (
        u"ldl.w [{reg1}], {reg3}",
        u"-----0110111100000000111111-----",
        [
          ( u"cycle_count", 11 )
        ],
        [ 32, 36 ]
      ),
      (
        u"loop {reg1}, {disp16}",
        u"---------------100000110111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 32, 56 ]
      ),
      (
        u"popsp {rh}-{rt}",
        u"-----0010110000001100111111-----",
        [
          ( u"cycle_count", 1 )
        ],
        [ 52, 51 ]
      ),
      (
        u"pushsp {rh}-{rt}",
        u"-----0010110000001000111111-----",
        [
          ( u"cycle_count", 1 )
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
        u"stc.b {reg3}, [{reg1}]",
        u"-----0110111001000000111111-----",
        [
          ( u"cycle_count", 8 )
        ],
        [ 36, 32 ]
      ),
      (
        u"stc.h {reg3}, [{reg1}]",
        u"-----0110111011000000111111-----",
        [
          ( u"cycle_count", 8 )
        ],
        [ 36, 32 ]
      ),
      (
        u"stc.w {reg3}, [{reg1}]",
        u"-----0110111101000000111111-----",
        [
          ( u"cycle_count", 8 )
        ],
        [ 36, 32 ]
      ),
      (
        u"synci",
        u"0000000000011100",
        None
      ),
      (
        u"roundf.dl {reg2}, {reg3}",
        u"----010001010100----011111100000",
        [ 40, 41 ]
      ),
      (
        u"roundf.dul {reg2}, {reg3}",
        u"----010001010100----011111110000",
        [ 40, 41 ]
      ),
      (
        u"roundf.duw {reg2}, {reg3}",
        u"-----10001010000----011111110000",
        [ 40, 36 ]
      ),
      (
        u"roundf.dw {reg2}, {reg3}",
        u"-----10001010000----011111100000",
        [ 40, 36 ]
      ),
      (
        u"roundf.sl {reg2}, {reg3}",
        u"----010001000100-----11111100000",
        [ 35, 41 ]
      ),
      (
        u"roundf.sul {reg2}, {reg3}",
        u"----010001000100-----11111110000",
        [ 35, 41 ]
      ),
      (
        u"roundf.suw {reg2}, {reg3}",
        u"-----10001000000-----11111110000",
        [ 35, 36 ]
      ),
      (
        u"roundf.sw {reg2}, {reg3}",
        u"-----10001000000-----11111100000",
        [ 35, 36 ]
      ),
      (
        u"cache {cacheop}, [{reg1}]",
        u"-----00101100000111--111111-----",
        [ 53, 32 ]
      ),
      (
        u"pref {prefop}, [{reg1}]",
        u"-----0010110000011011111111-----",
        [ 54, 32 ]
      ),
      (
        u"dbcp",
        u"1110100001000000",
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
        [
          ( u"terminates_block", True )
        ],
        None
      ),
      (
        u"dbtag {imm10}",
        u"-----0010110000011001111111-----",
        [ 55 ]
      ),
      (
        u"dbtrap",
        u"1111100001000000",
        [
          ( u"terminates_block", True )
        ],
        None
      ),
      (
        u"cvtf.hs {reg2}, {reg3}",
        u"-----10001000010-----111111---10",
        [ 35, 36 ]
      ),
      (
        u"cvtf.sh {reg2}, {reg3}",
        u"-----10001000010-----111111---11",
        [ 35, 36 ]
      ),
      (
        u"fmaf.s {reg1}, {reg2}, {reg3}",
        u"-----10011100000-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"fmsf.s {reg1}, {reg2}, {reg3}",
        u"-----10011100010-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"fnmaf.s {reg1}, {reg2}, {reg3}",
        u"-----10011100100-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"fnmsf.s {reg1}, {reg2}, {reg3}",
        u"-----10011100110-----111111-----",
        [ 32, 35, 36 ]
      ),
      (
        u"cll",
        u"11110001011000001111111111111111",
        None
      ),
      (
        u"resbank",
        u"10000001011000000000011111100000",
        None
      ),
      (
        u"clip.b {reg1}, {reg2}",
        u"0000000000001000-----111111-----",
        [ 32, 35 ]
      ),
      (
        u"clip.bu {reg1}, {reg2}",
        u"0000000000001010-----111111-----",
        [ 32, 35 ]
      ),
      (
        u"clip.h {reg1}, {reg2}",
        u"0000000000001100-----111111-----",
        [ 32, 35 ]
      ),
      (
        u"clip.hu {reg1}, {reg2}",
        u"0000000000001110-----111111-----",
        [ 32, 35 ]
      ),
      (
        u"ldv.w {imm4}, {disp16} [{reg1}], {wreg3}",
        u"---------------------00----1110100000111101-----",
        [ 70, 63, 32, 60 ]
      ),
      (
        u"stv.w {imm2}, {wreg3}, {disp16} [{reg1}]",
        u"---------------------0100--1110100000111101-----",
        [ 68, 60, 63, 32 ]
      ),
      (
        u"ldv.qw {disp16} [{reg1}], {wreg3}",
        u"---------------------0101001110100000111101-----",
        [ 65, 32, 60 ]
      ),
      (
        u"stv.qw {wreg3}, {disp16} [{reg1}]",
        u"---------------------0101011110100000111101-----",
        [ 60, 65, 32 ]
      ),
      (
        u"ldv.dw {imm2}, {disp16} [{reg1}], {wreg3}",
        u"---------------------0110--1110100000111101-----",
        [ 68, 64, 32, 60 ]
      ),
      (
        u"stv.dw {imm1}, {wreg3}, {disp16} [{reg1}]",
        u"---------------------01110-1110100000111101-----",
        [ 66, 60, 64, 32 ]
      ),
      (
        u"ldvz.h4 {disp16} [{reg1}], {wreg3}",
        u"---------------------0111101110100000111101-----",
        [ 64, 32, 60 ]
      ),
      (
        u"stvz.h4 {wreg3}, {disp16} [{reg1}]",
        u"---------------------0111111110100000111101-----",
        [ 60, 64, 32 ]
      ),
      (
        u"cmovf.w4 {wreg4}, {wreg1}, {wreg2}, {wreg3}",
        u"---------------------1100001110100000111101-----",
        [ 61, 57, 59, 60 ]
      ),
      (
        u"shflv.w4 {imm12}, {wreg1}, {wreg2}, {wreg3}",
        u"---------------------11010-1110100000111101-----",
        [ 71, 57, 59, 60 ]
      ),
      (
        u"fmaf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10011000000-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"fmsf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10011000010-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"fnmaf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10011000100-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"fnmsf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10011000110-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"cmpf.s4 {fcond}, {wreg1}, {wreg2}, {wreg3}",
        u"-----101100----0-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 62, 57, 58, 60 ]
      ),
      (
        u"roundf.sw4 {wreg2}, {wreg3}",
        u"-----10110100000-----11111100000",
        [
          ( u"cycle_count", 2 )
        ],
        [ 58, 60 ]
      ),
      (
        u"roundf.suw4 {wreg2}, {wreg3}",
        u"-----10110100000-----11111100001",
        [
          ( u"cycle_count", 2 )
        ],
        [ 58, 60 ]
      ),
      (
        u"trncf.sw4 {wreg2}, {wreg3}",
        u"-----10110100000-----11111100010",
        [
          ( u"cycle_count", 2 )
        ],
        [ 58, 60 ]
      ),
      (
        u"trncf.suw4 {wreg2}, {wreg3}",
        u"-----10110100000-----11111100011",
        [
          ( u"cycle_count", 2 )
        ],
        [ 58, 60 ]
      ),
      (
        u"ceilf.sw4 {wreg2}, {wreg3}",
        u"-----10110100000-----11111100100",
        [
          ( u"cycle_count", 2 )
        ],
        [ 58, 60 ]
      ),
      (
        u"ceilf.suw4 {wreg2}, {wreg3}",
        u"-----10110100000-----11111100101",
        [
          ( u"cycle_count", 2 )
        ],
        [ 58, 60 ]
      ),
      (
        u"floorf.sw4 {wreg2}, {wreg3}",
        u"-----10110100000-----11111100110",
        [
          ( u"cycle_count", 2 )
        ],
        [ 58, 60 ]
      ),
      (
        u"floorf.suw4 {wreg2}, {wreg3}",
        u"-----10110100000-----11111100111",
        [
          ( u"cycle_count", 2 )
        ],
        [ 58, 60 ]
      ),
      (
        u"cvtf.sw4 {wreg2}, {wreg3}",
        u"-----10110100000-----11111101000",
        [
          ( u"cycle_count", 2 )
        ],
        [ 58, 60 ]
      ),
      (
        u"cvtf.suw4 {wreg2}, {wreg3}",
        u"-----10110100000-----11111101001",
        [
          ( u"cycle_count", 2 )
        ],
        [ 58, 60 ]
      ),
      (
        u"cvtf.ws4 {wreg2}, {wreg3}",
        u"-----10110100000-----11111101010",
        [
          ( u"cycle_count", 2 )
        ],
        [ 58, 60 ]
      ),
      (
        u"cvtf.uws4 {wreg2}, {wreg3}",
        u"-----10110100000-----11111101011",
        [
          ( u"cycle_count", 2 )
        ],
        [ 58, 60 ]
      ),
      (
        u"cvtf.hs4 {wreg2}, {wreg3}",
        u"-----10110100000-----11111101100",
        [
          ( u"cycle_count", 2 )
        ],
        [ 58, 60 ]
      ),
      (
        u"cvtf.sh4 {wreg2}, {wreg3}",
        u"-----10110100000-----11111101101",
        [
          ( u"cycle_count", 2 )
        ],
        [ 58, 60 ]
      ),
      (
        u"absf.s4 {wreg2}, {wreg3}",
        u"-----10110100000-----11111110000",
        [ 58, 60 ]
      ),
      (
        u"negf.s4 {wreg2}, {wreg3}",
        u"-----10110100000-----11111110001",
        [ 58, 60 ]
      ),
      (
        u"sqrtf.s4 {wreg2}, {wreg3}",
        u"-----10110100000-----11111110010",
        [
          ( u"cycle_count", 28 )
        ],
        [ 58, 60 ]
      ),
      (
        u"recipf.s4 {wreg2}, {wreg3}",
        u"-----10110100000-----11111110011",
        [
          ( u"cycle_count", 16 )
        ],
        [ 58, 60 ]
      ),
      (
        u"rsqrtf.s4 {wreg2}, {wreg3}",
        u"-----10110100000-----11111110100",
        [
          ( u"cycle_count", 42 )
        ],
        [ 58, 60 ]
      ),
      (
        u"flpv.s4 {imm2}, {wreg2}, {wreg3}",
        u"-----10110100000-----111111110--",
        [ 67, 58, 60 ]
      ),
      (
        u"movv.w4 {wreg2}, {wreg3}",
        u"-----10110100000-----11111111110",
        [ 58, 60 ]
      ),
      (
        u"trfsrv.w4 {imm3}, {wreg2}",
        u"00---10110100000-----11111111111",
        [ 69, 58 ]
      ),
      (
        u"addf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10110100100-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"subf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10110100110-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"mulf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10110101000-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"maxf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10110101010-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"minf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10110101100-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"divf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10110101110-----111111-----",
        [
          ( u"cycle_count", 16 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"addrf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10110110100-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"subrf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10110110110-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"mulrf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10110111000-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"maxrf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10110111010-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"minrf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10110111100-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"addxf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10111000100-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"subxf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10111000110-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"mulxf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10111001000-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"addsubf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10111010000-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"subaddf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10111010010-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"addsubxf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10111010100-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"subaddxf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10111010110-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"addsubnf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10111011000-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"subaddnf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10111011010-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"addsubnxf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10111011100-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"subaddnxf.s4 {wreg1}, {wreg2}, {wreg3}",
        u"-----10111011110-----111111-----",
        [
          ( u"cycle_count", 2 )
        ],
        [ 57, 58, 60 ]
      ),
      (
        u"hvtrap {vector}",
        u"000000010001000000000111111-----",
        [
          ( u"terminates_block", True ),
          ( u"cycle_count", 8 )
        ],
        [ 44 ]
      ),
      (
        u"stm.gsr [{reg1}]",
        u"100100010110000000000111111-----",
        [
          ( u"cycle_count", 1 )
        ],
        [ 32 ]
      ),
      (
        u"stm.mp {eh}-{et}, [{reg1}]",
        u"-----00101100100-----111111-----",
        [
          ( u"cycle_count", 1 )
        ],
        [ 73, 72, 32 ]
      ),
      (
        u"ldm.gsr [{reg1}]",
        u"100110010110000000000111111-----",
        [
          ( u"cycle_count", 1 )
        ],
        [ 32 ]
      ),
      (
        u"ldm.mp [{reg1}], {eh}-{et}",
        u"-----00101100110-----111111-----",
        [
          ( u"cycle_count", 1 )
        ],
        [ 32, 73, 72 ]
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
          u"ld.b [{reg1}]+, {reg3}",
          u"ld.b [{reg1}]-, {reg3}",
          u"ld.bu {disp23} [{reg1}], {reg3}",
          u"ld.bu {disp16} [{reg1}], {reg2}",
          u"ld.bu [{reg1}]+, {reg3}",
          u"ld.bu [{reg1}]-, {reg3}",
          u"ld.h {disp16} [{reg1}], {reg2}",
          u"ld.h {disp23} [{reg1}], {reg3}",
          u"ld.h [{reg1}]+, {reg3}",
          u"ld.h [{reg1}]-, {reg3}",
          u"ld.hu {disp16} [{reg1}], {reg2}",
          u"ld.hu {disp23} [{reg1}], {reg3}",
          u"ld.hu [{reg1}]+, {reg3}",
          u"ld.hu [{reg1}]-, {reg3}",
          u"ld.w {disp16} [{reg1}], {reg2}",
          u"ld.w {disp23} [{reg1}], {reg3}",
          u"ld.w [{reg1}]+, {reg3}",
          u"ld.w [{reg1}]-, {reg3}",
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
          u"st.b {reg3}, [{reg1}]+",
          u"st.b {reg3}, [{reg1}]-",
          u"st.h {reg2}, {disp16} [{reg1}]",
          u"st.h {reg3}, {disp23} [{reg1}]",
          u"st.h {reg3}, [{reg1}]+",
          u"st.h {reg3}, [{reg1}]-",
          u"st.w {reg2}, {disp16} [{reg1}]",
          u"st.w {reg3}, {disp23} [{reg1}]",
          u"st.w {reg3}, [{reg1}]+",
          u"st.w {reg3}, [{reg1}]-",
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
          u"absf.d {reg2}, {reg3}",
          u"absf.s {reg2}, {reg3}",
          u"addf.d {reg1}, {reg2}, {reg3}",
          u"addf.s {reg1}, {reg2}, {reg3}",
          u"ceilf.dl {reg2}, {reg3}",
          u"ceilf.dul {reg2}, {reg3}",
          u"ceilf.duw {reg2}, {reg3}",
          u"ceilf.dw {reg2}, {reg3}",
          u"ceilf.sl {reg2}, {reg3}",
          u"ceilf.sul {reg2}, {reg3}",
          u"ceilf.suw {reg2}, {reg3}",
          u"ceilf.sw {reg2}, {reg3}",
          u"cmovf.d {fcbit}, {reg1}, {reg2}, {reg3}",
          u"cmovf.s {fcbit}, {reg1}, {reg2}, {reg3}",
          u"cmpf.d {fcond}, {reg2}, {reg1}, {fcbit}",
          u"cmpf.s {fcond}, {reg2}, {reg1}, {fcbit}",
          u"cvtf.dl {reg2}, {reg3}",
          u"cvtf.ds {reg2}, {reg3}",
          u"cvtf.dul {reg2}, {reg3}",
          u"cvtf.duw {reg2}, {reg3}",
          u"cvtf.dw {reg2}, {reg3}",
          u"cvtf.ld {reg2}, {reg3}",
          u"cvtf.ls {reg2}, {reg3}",
          u"cvtf.sd {reg2}, {reg3}",
          u"cvtf.sl {reg2}, {reg3}",
          u"cvtf.sul {reg2}, {reg3}",
          u"cvtf.suw {reg2}, {reg3}",
          u"cvtf.sw {reg2}, {reg3}",
          u"cvtf.uld {reg2}, {reg3}",
          u"cvtf.uls {reg2}, {reg3}",
          u"cvtf.uwd {reg2}, {reg3}",
          u"cvtf.uws {reg2}, {reg3}",
          u"cvtf.wd {reg2}, {reg3}",
          u"cvtf.ws {reg2}, {reg3}",
          u"divf.d {reg1}, {reg2}, {reg3}",
          u"divf.s {reg1}, {reg2}, {reg3}",
          u"floorf.dl {reg2}, {reg3}",
          u"floorf.dul {reg2}, {reg3}",
          u"floorf.duw {reg2}, {reg3}",
          u"floorf.dw {reg2}, {reg3}",
          u"floorf.sl {reg2}, {reg3}",
          u"floorf.sul {reg2}, {reg3}",
          u"floorf.suw {reg2}, {reg3}",
          u"floorf.sw {reg2}, {reg3}",
          u"maddf.s {reg1}, {reg2}, {reg3}, {reg4}",
          u"maxf.d {reg1}, {reg2}, {reg3}",
          u"maxf.s {reg1}, {reg2}, {reg3}",
          u"minf.d {reg1}, {reg2}, {reg3}",
          u"minf.s {reg1}, {reg2}, {reg3}",
          u"msubf.s {reg1}, {reg2}, {reg3}, {reg4}",
          u"mulf.d {reg1}, {reg2}, {reg3}",
          u"mulf.s {reg1}, {reg2}, {reg3}",
          u"negf.d {reg2}, {reg3}",
          u"negf.s {reg2}, {reg3}",
          u"nmaddf.s {reg1}, {reg2}, {reg3}, {reg4}",
          u"nmsubf.s {reg1}, {reg2}, {reg3}, {reg4}",
          u"recipf.d {reg2}, {reg3}",
          u"recipf.s {reg2}, {reg3}",
          u"rsqrtf.d {reg2}, {reg3}",
          u"rsqrtf.s {reg2}, {reg3}",
          u"sqrtf.d {reg2}, {reg3}",
          u"sqrtf.s {reg2}, {reg3}",
          u"subf.d {reg1}, {reg2}, {reg3}",
          u"subf.s {reg1}, {reg2}, {reg3}",
          u"trfsr {fcbit}",
          u"trncf.dl {reg2}, {reg3}",
          u"trncf.dul {reg2}, {reg3}",
          u"trncf.duw {reg2}, {reg3}",
          u"trncf.dw {reg2}, {reg3}",
          u"trncf.sl {reg2}, {reg3}",
          u"trncf.sul {reg2}, {reg3}",
          u"trncf.suw {reg2}, {reg3}",
          u"trncf.sw {reg2}, {reg3}",
          u"b{cccc} {disp17}",
          u"binsu {reg1}, {lsb}, {msb}, {reg2}",
          u"binsm {reg1}, {lsb}, {msb}, {reg2}",
          u"binsl {reg1}, {lsb}, {msb}, {reg2}",
          u"jarl [{reg1}], {reg3}",
          u"ld.dw {disp23} [{reg1}], {reg3}",
          u"ldl.bu [{reg1}], {reg3}",
          u"ldl.hu [{reg1}], {reg3}",
          u"ldl.w [{reg1}], {reg3}",
          u"loop {reg1}, {disp16}",
          u"popsp {rh}-{rt}",
          u"pushsp {rh}-{rt}",
          u"rotl {imm5}, {reg2}, {reg3}",
          u"rotl {reg1}, {reg2}, {reg3}",
          u"snooze",
          u"st.dw {reg3}, {disp23} [{reg1}]",
          u"stc.b {reg3}, [{reg1}]",
          u"stc.h {reg3}, [{reg1}]",
          u"stc.w {reg3}, [{reg1}]",
          u"synci",
          u"roundf.dl {reg2}, {reg3}",
          u"roundf.dul {reg2}, {reg3}",
          u"roundf.duw {reg2}, {reg3}",
          u"roundf.dw {reg2}, {reg3}",
          u"roundf.sl {reg2}, {reg3}",
          u"roundf.sul {reg2}, {reg3}",
          u"roundf.suw {reg2}, {reg3}",
          u"roundf.sw {reg2}, {reg3}",
          u"cache {cacheop}, [{reg1}]",
          u"pref {prefop}, [{reg1}]",
          u"dbcp",
          u"dbpush {rh}-{rt}",
          u"dbret",
          u"dbtag {imm10}",
          u"dbtrap",
          u"cvtf.hs {reg2}, {reg3}",
          u"cvtf.sh {reg2}, {reg3}",
          u"fmaf.s {reg1}, {reg2}, {reg3}",
          u"fmsf.s {reg1}, {reg2}, {reg3}",
          u"fnmaf.s {reg1}, {reg2}, {reg3}",
          u"fnmsf.s {reg1}, {reg2}, {reg3}",
          u"cll",
          u"resbank",
          u"clip.b {reg1}, {reg2}",
          u"clip.bu {reg1}, {reg2}",
          u"clip.h {reg1}, {reg2}",
          u"clip.hu {reg1}, {reg2}",
          u"ldv.w {imm4}, {disp16} [{reg1}], {wreg3}",
          u"stv.w {imm2}, {wreg3}, {disp16} [{reg1}]",
          u"ldv.qw {disp16} [{reg1}], {wreg3}",
          u"stv.qw {wreg3}, {disp16} [{reg1}]",
          u"ldv.dw {imm2}, {disp16} [{reg1}], {wreg3}",
          u"stv.dw {imm1}, {wreg3}, {disp16} [{reg1}]",
          u"ldvz.h4 {disp16} [{reg1}], {wreg3}",
          u"stvz.h4 {wreg3}, {disp16} [{reg1}]",
          u"cmovf.w4 {wreg4}, {wreg1}, {wreg2}, {wreg3}",
          u"shflv.w4 {imm12}, {wreg1}, {wreg2}, {wreg3}",
          u"fmaf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"fmsf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"fnmaf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"fnmsf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"cmpf.s4 {fcond}, {wreg1}, {wreg2}, {wreg3}",
          u"roundf.sw4 {wreg2}, {wreg3}",
          u"roundf.suw4 {wreg2}, {wreg3}",
          u"trncf.sw4 {wreg2}, {wreg3}",
          u"trncf.suw4 {wreg2}, {wreg3}",
          u"ceilf.sw4 {wreg2}, {wreg3}",
          u"ceilf.suw4 {wreg2}, {wreg3}",
          u"floorf.sw4 {wreg2}, {wreg3}",
          u"floorf.suw4 {wreg2}, {wreg3}",
          u"cvtf.sw4 {wreg2}, {wreg3}",
          u"cvtf.suw4 {wreg2}, {wreg3}",
          u"cvtf.ws4 {wreg2}, {wreg3}",
          u"cvtf.uws4 {wreg2}, {wreg3}",
          u"cvtf.hs4 {wreg2}, {wreg3}",
          u"cvtf.sh4 {wreg2}, {wreg3}",
          u"absf.s4 {wreg2}, {wreg3}",
          u"negf.s4 {wreg2}, {wreg3}",
          u"sqrtf.s4 {wreg2}, {wreg3}",
          u"recipf.s4 {wreg2}, {wreg3}",
          u"rsqrtf.s4 {wreg2}, {wreg3}",
          u"flpv.s4 {imm2}, {wreg2}, {wreg3}",
          u"movv.w4 {wreg2}, {wreg3}",
          u"trfsrv.w4 {imm3}, {wreg2}",
          u"addf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"subf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"mulf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"maxf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"minf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"divf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"addrf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"subrf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"mulrf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"maxrf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"minrf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"addxf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"subxf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"mulxf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"addsubf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"subaddf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"addsubxf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"subaddxf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"addsubnf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"subaddnf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"addsubnxf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"subaddnxf.s4 {wreg1}, {wreg2}, {wreg3}",
          u"hvtrap {vector}",
          u"stm.gsr [{reg1}]",
          u"stm.mp {eh}-{et}, [{reg1}]",
          u"ldm.gsr [{reg1}]",
          u"ldm.mp [{reg1}], {eh}-{et}"
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
