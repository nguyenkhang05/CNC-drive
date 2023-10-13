#------------------------------------------------------------------------------
# Copyright (C) Australian Semiconductor Technology Company. 2019.
# All Rights Reserved.
#
# This is unpublished proprietary source code of the Australian Semiconductor
# Technology Company (ASTC).  The copyright notice does not evidence any actual
# or intended publication of such source code.
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Imports
#------------------------------------------------------------------------------

import re
import string
import bitstring

#------------------------------------------------------------------------------
# Register group mappings
#------------------------------------------------------------------------------

GR_group_name_index_map = {
  'r0' : 0,
  'r1' : 1,
  'r2' : 2,
  'r3' : 3,
  'r4' : 4,
  'r5' : 5,
  'r6' : 6,
  'r7' : 7,
  'r8' : 8,
  'r9' : 9,
  'r10' : 10,
  'r11' : 11,
  'r12' : 12,
  'r13' : 13,
  'r14' : 14,
  'r15' : 15,
  'r16' : 16,
  'r17' : 17,
  'r18' : 18,
  'r19' : 19,
  'r20' : 20,
  'r21' : 21,
  'r22' : 22,
  'r23' : 23,
  'r24' : 24,
  'r25' : 25,
  'r26' : 26,
  'r27' : 27,
  'r28' : 28,
  'r29' : 29,
  'r30' : 30,
  'r31' : 31
}

SR_SELID_0_group_name_index_map = {
  'EIPC' : 0,
  'EIPSW' : 1,
  'FEPC' : 2,
  'FEPSW' : 3,
  'PSW' : 5,
  'FPSR' : 6,
  'FPEPC' : 7,
  'FPST' : 8,
  'FPCC' : 9,
  'FPCFG' : 10,
  'FPEC' : 11,
  'EIIC' : 13,
  'FEIC' : 14,
  'CTPC' : 16,
  'CTPSW' : 17,
  'CTBP' : 20,
  'EIWR' : 28,
  'FEWR' : 29,
  'BSEL' : 31
}

#------------------------------------------------------------------------------
# Assembler
#------------------------------------------------------------------------------

class assembler(object):

  # Instruction form nested class
  class instruction_form(object):
    def __init__(self, operand_pattern):
      self.operand_pattern = self._group_ungrouped_text(operand_pattern)
      self.instruction_info = {}

    def _bits_to_opcode_value(self, bit_pattern):
      opcode_value_pattern = bit_pattern.translate(string.maketrans("01-", "010"), " ")
      return (int(opcode_value_pattern, 2), len(opcode_value_pattern))
 
    def _group_ungrouped_text(self, operand_pattern):
      operand_pattern = re.sub("\+", "\\+", operand_pattern)
      operand_pattern = re.sub("\.", "\\.", operand_pattern)
      operand_pattern = re.sub("\(", "\\(", operand_pattern)
      operand_pattern = re.sub("\)", "\\)", operand_pattern)
      operand_pattern = re.sub("\[", "{\\[}", operand_pattern)
      operand_pattern = re.sub("\]", "{\\]}", operand_pattern)
      operand_pattern = re.sub(",\s*", "{,\\s*}", operand_pattern)
      operand_pattern = re.sub("\s+", "{\\s+}", operand_pattern)
      operand_pattern = re.sub("{+", "{", operand_pattern)
      operand_pattern = re.sub("}+", "}", operand_pattern)
      grouped_pattern = ""
      li = 0
      for mo in re.finditer("({[^{]*?})", operand_pattern):
        mo_start = mo.start(1)
        mo_end = mo.end(1)
        if mo_start > li:
          grouped_pattern += "{%s}" % operand_pattern[li:mo_start]
        grouped_pattern = grouped_pattern + mo.group(1)
        li = mo_end
      if len(operand_pattern) > li:
        grouped_pattern += "{%s}" % operand_pattern[li:]
      return grouped_pattern
 
    def add_instruction(self, mnemonic, bit_pattern):
      self.instruction_info[mnemonic] = self._bits_to_opcode_value(bit_pattern)

    def initialise(self, operand_map):
      self.mnemonic_pattern = "(" + "|".join(sorted(self.instruction_info.keys())) + ")"
      for substring in re.findall("{.*?}", self.operand_pattern):
        substring = substring[1:-1]
        if substring in operand_map.keys():
          (operand_pattern, operand_encode_fn) = operand_map[substring]
          self.mnemonic_pattern += "(?P<%s>%s)" % (substring, operand_pattern)
        elif substring in ['\\s+', ',\\s*']:
          self.mnemonic_pattern += substring          
        else:  
          self.mnemonic_pattern += "(%s)" % substring
      self.mnemonic_pattern = "^%s$" % self.mnemonic_pattern
      self.mnemonic_re = re.compile(self.mnemonic_pattern)
 	  
  # Instruction set nested class
  class instruction_set(object):
    def __init__(self):
      self.form_list = []
      self.operand_map = {}
      
    def add_instruction_form(self, operand_pattern):
      form = assembler.instruction_form(operand_pattern)
      self.form_list.append(form)
      return form
      
    def initialise(self):
      for form in self.form_list:
        form.initialise(self.operand_map)
              
    def add_operand(self, name, pattern, encode_fn):
      self.operand_map[name] = (pattern, encode_fn)
      
    def assemble(self, mnemonic):
      opcode = 0
      # print "mnemonic=%s" % mnemonic
      for form in self.form_list:
        mnemonic_re_match = form.mnemonic_re.match(mnemonic)
        if mnemonic_re_match:
          # print form.mnemonic_pattern
          instr = mnemonic_re_match.group(1)
          # print "  instr='%s'" % instr
          (opcode, bits) = form.instruction_info[instr]
          # print ("  base=0x%%0%dx" % (bits / 4)) % opcode
          operand_group_map = mnemonic_re_match.groupdict()
          for operand_name in operand_group_map.keys():
            (operand_pattern, operand_encode_fn) = self.operand_map[operand_name]
            operand_group_value = operand_group_map[operand_name]
            operand_value = operand_encode_fn(operand_group_value)
            # print ("  %%s:\t'%%s'\t-> 0x%%0%dx" % (bits / 4)) % (operand_name, operand_group_value, operand_value)
            opcode += operand_value
          # print ("  opcode=0x%%0%dx" % (bits / 4)) % opcode
          # return bytearray.fromhex(("%%0%dx" % (bits / 4)) % opcode)
          return bitstring.BitArray(uintle=opcode, length=bits).bytes
      return None

  # Assembler operand encoding operations
  def encode_bit_u8_11_13_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0x7) << 11
    return opcode_value
  
  def encode_bits_ff_u8_19_20_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0x3) << 19
    return opcode_value
  
  def encode_cacheop_u8_11_12_27_31_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0x1f) << 27
    operand_value = (operand_value >> 5)
    opcode_value |= (operand_value & 0x3) << 11
    return opcode_value
  
  def encode_cccc_u8_0_3_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= operand_value & 0xf
    return opcode_value
  
  def encode_cccc_u8_17_20_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0xf) << 17
    return opcode_value
  
  def encode_disp16_s32_16_31_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0xffff) << 16
    return opcode_value
  
  def encode_disp16_s32_17_31_5_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0x1) << 5
    operand_value = (operand_value >> 1)
    opcode_value |= (operand_value & 0x7fff) << 17
    return opcode_value
  
  def encode_disp16_s32_17_31_shift1_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    operand_value = (operand_value >> 1)
    opcode_value |= (operand_value & 0x7fff) << 17
    return opcode_value
  
  def encode_disp16_u32_17_31_shift1_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    operand_value = (operand_value >> 1)
    opcode_value |= (operand_value & 0x7fff) << 17
    return opcode_value
  
  def encode_disp17_s32_4_17_31_shift1_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    operand_value = (operand_value >> 1)
    opcode_value |= (operand_value & 0x7fff) << 17
    operand_value = (operand_value >> 15)
    opcode_value |= (operand_value & 0x1) << 4
    return opcode_value
  
  def encode_disp22_s32_0_5_17_31_shift1_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    operand_value = (operand_value >> 1)
    opcode_value |= (operand_value & 0x7fff) << 17
    operand_value = (operand_value >> 15)
    opcode_value |= operand_value & 0x3f
    return opcode_value
  
  def encode_disp23_s32_32_47_20_26_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0x7f) << 20
    operand_value = (operand_value >> 7)
    opcode_value |= (operand_value & 0xffff) << 32
    return opcode_value
  
  def encode_disp23_s32_32_47_21_26_shift1_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    operand_value = (operand_value >> 1)
    opcode_value |= (operand_value & 0x3f) << 21
    operand_value = (operand_value >> 6)
    opcode_value |= (operand_value & 0xffff) << 32
    return opcode_value
  
  def encode_disp32_s32_17_47_shift1_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    operand_value = (operand_value >> 1)
    opcode_value |= (operand_value & 0x7fffffff) << 17
    return opcode_value
  
  def encode_disp4_u32_0_3_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= operand_value & 0xf
    return opcode_value
  
  def encode_disp5_u32_0_3_shift1_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    operand_value = (operand_value >> 1)
    opcode_value |= operand_value & 0xf
    return opcode_value
  
  def encode_disp7_u32_0_6_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= operand_value & 0x7f
    return opcode_value
  
  def encode_disp8_u32_0_6_shift1_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    operand_value = (operand_value >> 1)
    opcode_value |= operand_value & 0x7f
    return opcode_value
  
  def encode_disp8_u32_1_6_shift2_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    operand_value = (operand_value >> 2)
    opcode_value |= (operand_value & 0x3f) << 1
    return opcode_value
  
  def encode_disp9_s32_11_15_4_6_shift1_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    operand_value = (operand_value >> 1)
    opcode_value |= (operand_value & 0x7) << 4
    operand_value = (operand_value >> 3)
    opcode_value |= (operand_value & 0x1f) << 11
    return opcode_value
  
  def encode_fcbit_u8_17_19_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0x7) << 17
    return opcode_value
  
  def encode_fcond_u8_27_30_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0xf) << 27
    return opcode_value
  
  def encode_imm10_u16_27_31_0_4_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= operand_value & 0x1f
    operand_value = (operand_value >> 5)
    opcode_value |= (operand_value & 0x1f) << 27
    return opcode_value
  
  def encode_imm16_s32_16_31_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0xffff) << 16
    return opcode_value
  
  def encode_imm16_u32_16_31_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0xffff) << 16
    return opcode_value
  
  def encode_imm16_u32_16_31_shift16_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    operand_value = (operand_value >> 16)
    opcode_value |= (operand_value & 0xffff) << 16
    return opcode_value
  
  def encode_imm32_s32_16_47_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0xffffffff) << 16
    return opcode_value
  
  def encode_imm4_u32_0_3_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= operand_value & 0xf
    return opcode_value
  
  def encode_imm5_s32_0_4_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= operand_value & 0x1f
    return opcode_value
  
  def encode_imm5_u32_0_4_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= operand_value & 0x1f
    return opcode_value
  
  def encode_imm5_u32_11_15_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0x1f) << 11
    return opcode_value
  
  def encode_imm5_u32_1_5_shift2_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    operand_value = (operand_value >> 2)
    opcode_value |= (operand_value & 0x1f) << 1
    return opcode_value
  
  def encode_imm6_u32_0_5_shift1_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    operand_value = (operand_value >> 1)
    opcode_value |= operand_value & 0x3f
    return opcode_value
  
  def encode_imm9_s32_18_21_0_4_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= operand_value & 0x1f
    operand_value = (operand_value >> 5)
    opcode_value |= (operand_value & 0xf) << 18
    return opcode_value
  
  def encode_imm9_u32_18_21_0_4_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= operand_value & 0x1f
    operand_value = (operand_value >> 5)
    opcode_value |= (operand_value & 0xf) << 18
    return opcode_value
  
  def encode_list12_u16_21_31_0_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= operand_value & 0x1
    operand_value = (operand_value >> 1)
    opcode_value |= (operand_value & 0x7ff) << 21
    return opcode_value
  
  def encode_lsb_u32_27_17_19_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0x7) << 17
    operand_value = (operand_value >> 3)
    opcode_value |= (operand_value & 0x1) << 27
    return opcode_value
  
  def encode_msb_u32_28_31_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0xf) << 28
    return opcode_value
  
  def encode_prefop_u8_27_31_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0x1f) << 27
    return opcode_value
  
  def encode_reg1_u8_0_4_value(self, operand):
    opcode_value = 0
    operand_value = GR_group_name_index_map[operand]
    opcode_value |= operand_value & 0x1f
    return opcode_value
  
  def encode_reg1_u8_11_15_value(self, operand):
    opcode_value = 0
    operand_value = GR_group_name_index_map[operand]
    opcode_value |= (operand_value & 0x1f) << 11
    return opcode_value
  
  def encode_reg1_u8_16_20_value(self, operand):
    opcode_value = 0
    operand_value = GR_group_name_index_map[operand]
    opcode_value |= (operand_value & 0x1f) << 16
    return opcode_value
  
  def encode_reg1_u8_1_4_shift1_value(self, operand):
    opcode_value = 0
    operand_value = GR_group_name_index_map[operand]
    operand_value = (operand_value >> 1)
    opcode_value |= (operand_value & 0xf) << 1
    return opcode_value
  
  def encode_reg2_u8_0_4_value(self, operand):
    opcode_value = 0
    operand_value = GR_group_name_index_map[operand]
    opcode_value |= operand_value & 0x1f
    return opcode_value
  
  def encode_reg2_u8_11_15_value(self, operand):
    opcode_value = 0
    operand_value = GR_group_name_index_map[operand]
    opcode_value |= (operand_value & 0x1f) << 11
    return opcode_value
  
  def encode_reg2_u8_12_15_shift1_value(self, operand):
    opcode_value = 0
    operand_value = GR_group_name_index_map[operand]
    operand_value = (operand_value >> 1)
    opcode_value |= (operand_value & 0xf) << 12
    return opcode_value
  
  def encode_reg3_u8_27_31_value(self, operand):
    opcode_value = 0
    operand_value = GR_group_name_index_map[operand]
    opcode_value |= (operand_value & 0x1f) << 27
    return opcode_value
  
  def encode_reg3_u8_28_31_shift1_value(self, operand):
    opcode_value = 0
    operand_value = GR_group_name_index_map[operand]
    operand_value = (operand_value >> 1)
    opcode_value |= (operand_value & 0xf) << 28
    return opcode_value
  
  def encode_reg4_u8_17_20_23_value(self, operand):
    opcode_value = 0
    operand_value = GR_group_name_index_map[operand]
    opcode_value |= (operand_value & 0x1) << 23
    operand_value = (operand_value >> 1)
    opcode_value |= (operand_value & 0xf) << 17
    return opcode_value
  
  def encode_reg4_u8_17_20_shift1_value(self, operand):
    opcode_value = 0
    operand_value = GR_group_name_index_map[operand]
    operand_value = (operand_value >> 1)
    opcode_value |= (operand_value & 0xf) << 17
    return opcode_value
  
  def encode_regID_u8_0_4_value(self, operand):
    opcode_value = 0
    operand_value = SR_SELID_0_group_name_index_map[operand]
    opcode_value |= operand_value & 0x1f
    return opcode_value
  
  def encode_regID_u8_11_15_value(self, operand):
    opcode_value = 0
    operand_value = SR_SELID_0_group_name_index_map[operand]
    opcode_value |= (operand_value & 0x1f) << 11
    return opcode_value
  
  def encode_rh_u8_0_4_value(self, operand):
    opcode_value = 0
    operand_value = GR_group_name_index_map[operand]
    opcode_value |= operand_value & 0x1f
    return opcode_value
  
  def encode_rt_u8_27_31_value(self, operand):
    opcode_value = 0
    operand_value = GR_group_name_index_map[operand]
    opcode_value |= (operand_value & 0x1f) << 27
    return opcode_value
  
  def encode_selID_u8_27_31_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0x1f) << 27
    return opcode_value
  
  def encode_vector_u8_0_4_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= operand_value & 0x1f
    return opcode_value
  
  def encode_vector_u8_11_14_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0xf) << 11
    return opcode_value
  
  def encode_vector_u8_27_29_0_4_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= operand_value & 0x1f
    operand_value = (operand_value >> 5)
    opcode_value |= (operand_value & 0x7) << 27
    return opcode_value
  
  def encode_vreg1_u8_0_4_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= operand_value & 0x1f
    return opcode_value
  
  def encode_vreg1_u8_11_15_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0x1f) << 11
    return opcode_value
  
  # Assembler methods    
  def initialise(self):
    self.isa.initialise()
    
  def assemble(self, mnemonic):
    return self.isa.assemble(mnemonic)
  
  def __init__(self, isa_name):
    self.isa_name = isa_name
    self.isa = None
	
class rh850e3_assembler(assembler):
  def __init__(self):
    super(rh850e3_assembler, self).__init__("rh850e3")

    self.isa = assembler.instruction_set()

    form = self.isa.add_instruction_form("")
    form.add_instruction("cll", "11110001011000001111111111111111")
    form.add_instruction("ctret", "00000001010001000000011111100000")
    form.add_instruction("dbcp", "1110100001000000")
    form.add_instruction("dbhvtrap", "1110000001000000")
    form.add_instruction("dbret", "00000001010001100000011111100000")
    form.add_instruction("dbtrap", "1111100001000000")
    form.add_instruction("di", "00000001011000000000011111100000")
    form.add_instruction("dst", "00000001001101000000011111100000")
    form.add_instruction("ei", "00000001011000001000011111100000")
    form.add_instruction("eiret", "00000001010010000000011111100000")
    form.add_instruction("est", "00000001001100100000011111100000")
    form.add_instruction("feret", "00000001010010100000011111100000")
    form.add_instruction("halt", "00000001001000000000011111100000")
    form.add_instruction("nop", "0000000000000000")
    form.add_instruction("rie", "0000000001000000")
    form.add_instruction("rmtrap", "1111000001000000")
    form.add_instruction("snooze", "00000001001000000000111111100000")
    form.add_instruction("synce", "0000000000011101")
    form.add_instruction("synci", "0000000000011100")
    form.add_instruction("syncm", "0000000000011110")
    form.add_instruction("syncp", "0000000000011111")
    
    form = self.isa.add_instruction_form(" [{reg1_u8_0_4}]")
    form.add_instruction("jmp", "00000000011-----")
    
    form = self.isa.add_instruction_form(" [{reg1_u8_0_4}], {reg2_u8_11_15}, {reg3_u8_27_31}")
    form.add_instruction("caxi", "-----00011101110-----111111-----")
    
    form = self.isa.add_instruction_form(" [{reg1_u8_0_4}], {reg3_u8_27_31}")
    form.add_instruction("jarl", "-----0010110000011000111111-----")
    form.add_instruction("ldl.w", "-----0110111100000000111111-----")
    
    form = self.isa.add_instruction_form(" {bit_u8_11_13}#3, {disp16_s32_16_31} [{reg1_u8_0_4}]")
    form.add_instruction("clr1", "----------------10---111110-----")
    form.add_instruction("not1", "----------------01---111110-----")
    form.add_instruction("set1", "----------------00---111110-----")
    form.add_instruction("tst1", "----------------11---111110-----")
    
    form = self.isa.add_instruction_form(" {cacheop_u8_11_12_27_31}, [{reg1_u8_0_4}]")
    form.add_instruction("cache", "-----00101100000111--111111-----")
    
    form = self.isa.add_instruction_form(" {cccc_u8_0_3}, {reg2_u8_11_15}")
    form.add_instruction("sasf", "0000001000000000-----1111110----")
    form.add_instruction("setf", "0000000000000000-----1111110----")
    
    form = self.isa.add_instruction_form(" {cccc_u8_17_20}, {imm5_s32_0_4}, {reg2_u8_11_15}, {reg3_u8_27_31}")
    form.add_instruction("cmov", "-----011000----0-----111111-----")
    
    form = self.isa.add_instruction_form(" {cccc_u8_17_20}, {reg1_u8_0_4}, {reg2_u8_11_15}, {reg3_u8_27_31}")
    form.add_instruction("adf", "-----011101----0-----111111-----")
    form.add_instruction("cmov", "-----011001----0-----111111-----")
    form.add_instruction("sbf", "-----011100----0-----111111-----")
    
    form = self.isa.add_instruction_form(" {disp16_s32_16_31} [{reg1_u8_0_4}], {reg2_u8_11_15}")
    form.add_instruction("ld.b", "---------------------111000-----")
    
    form = self.isa.add_instruction_form(" {disp16_s32_17_31_5} [{reg1_u8_0_4}], {reg2_u8_11_15}")
    form.add_instruction("ld.bu", "---------------1-----11110------")
    
    form = self.isa.add_instruction_form(" {disp16_s32_17_31_shift1} [{reg1_u8_0_4}], {reg2_u8_11_15}")
    form.add_instruction("ld.h", "---------------0-----111001-----")
    form.add_instruction("ld.hu", "---------------1-----111111-----")
    form.add_instruction("ld.w", "---------------1-----111001-----")
    
    form = self.isa.add_instruction_form(" {disp22_s32_0_5_17_31_shift1}")
    form.add_instruction("jr", "---------------00000011110------")
    
    form = self.isa.add_instruction_form(" {disp22_s32_0_5_17_31_shift1}, {reg2_u8_11_15}")
    form.add_instruction("jarl", "---------------0-----11110------")
    
    form = self.isa.add_instruction_form(" {disp23_s32_32_47_20_26} [{reg1_u8_0_4}], {reg3_u8_27_31}")
    form.add_instruction("ld.b", "----------------------------010100000111100-----")
    form.add_instruction("ld.bu", "----------------------------010100000111101-----")
    
    form = self.isa.add_instruction_form(" {disp23_s32_32_47_21_26_shift1} [{reg1_u8_0_4}], {reg3_u8_27_31}")
    form.add_instruction("ld.dw", "---------------------------0100100000111101-----")
    form.add_instruction("ld.h", "---------------------------0011100000111100-----")
    form.add_instruction("ld.hu", "---------------------------0011100000111101-----")
    form.add_instruction("ld.w", "---------------------------0100100000111100-----")
    
    form = self.isa.add_instruction_form(" {disp32_s32_17_47_shift1}")
    form.add_instruction("jr", "-------------------------------00000001011100000")
    
    form = self.isa.add_instruction_form(" {disp32_s32_17_47_shift1}, {reg1_u8_0_4}")
    form.add_instruction("jarl", "-------------------------------000000010111-----")
    
    form = self.isa.add_instruction_form(" {disp32_s32_17_47_shift1}[{reg1_u8_0_4}]")
    form.add_instruction("jmp", "-------------------------------000000110111-----")
    
    form = self.isa.add_instruction_form(" {disp4_u32_0_3} [ep], {reg2_u8_11_15}")
    form.add_instruction("sld.bu", "-----0000110----")
    
    form = self.isa.add_instruction_form(" {disp5_u32_0_3_shift1} [ep], {reg2_u8_11_15}")
    form.add_instruction("sld.hu", "-----0000111----")
    
    form = self.isa.add_instruction_form(" {disp7_u32_0_6} [ep], {reg2_u8_11_15}")
    form.add_instruction("sld.b", "-----0110-------")
    
    form = self.isa.add_instruction_form(" {disp8_u32_0_6_shift1} [ep], {reg2_u8_11_15}")
    form.add_instruction("sld.h", "-----1000-------")
    
    form = self.isa.add_instruction_form(" {disp8_u32_1_6_shift2} [ep], {reg2_u8_11_15}")
    form.add_instruction("sld.w", "-----1010------0")
    
    form = self.isa.add_instruction_form(" {fcond_u8_27_30}, {reg2_u8_12_15_shift1}, {reg1_u8_1_4_shift1}, {fcbit_u8_17_19}")
    form.add_instruction("cmpf.d", "0----1000011---0----0111111----0")
    
    form = self.isa.add_instruction_form(" {imm10_u16_27_31_0_4}")
    form.add_instruction("dbtag", "-----0010110000011001111111-----")
    
    form = self.isa.add_instruction_form(" {imm16_s32_16_31}, {reg1_u8_0_4}, {reg2_u8_11_15}")
    form.add_instruction("addi", "---------------------110000-----")
    form.add_instruction("movea", "---------------------110001-----")
    form.add_instruction("mulhi", "---------------------110111-----")
    form.add_instruction("satsubi", "---------------------110011-----")
    
    form = self.isa.add_instruction_form(" {imm16_u32_16_31_shift16}, {reg1_u8_0_4}, {reg2_u8_11_15}")
    form.add_instruction("movhi", "---------------------110010-----")
    
    form = self.isa.add_instruction_form(" {imm16_u32_16_31}, {reg1_u8_0_4}, {reg2_u8_11_15}")
    form.add_instruction("andi", "---------------------110110-----")
    form.add_instruction("ori", "---------------------110100-----")
    form.add_instruction("xori", "---------------------110101-----")
    
    form = self.isa.add_instruction_form(" {imm32_s32_16_47}, {reg1_u8_0_4}")
    form.add_instruction("mov", "--------------------------------00000110001-----")
    
    form = self.isa.add_instruction_form(" {imm5_s32_0_4}, {reg2_u8_11_15}")
    form.add_instruction("add", "-----010010-----")
    form.add_instruction("cmp", "-----010011-----")
    form.add_instruction("mov", "-----010000-----")
    form.add_instruction("mulh", "-----010111-----")
    form.add_instruction("satadd", "-----010001-----")
    
    form = self.isa.add_instruction_form(" {imm5_u32_0_4}, {reg2_u8_11_15}")
    form.add_instruction("sar", "-----010101-----")
    form.add_instruction("shl", "-----010110-----")
    form.add_instruction("shr", "-----010100-----")
    
    form = self.isa.add_instruction_form(" {imm5_u32_0_4}, {reg2_u8_11_15}, {reg3_u8_27_31}")
    form.add_instruction("rotl", "-----00011000100-----111111-----")
    
    form = self.isa.add_instruction_form(" {imm5_u32_11_15}, {imm4_u32_0_3}")
    form.add_instruction("rie", "0000000000000000-----1111111----")
    
    form = self.isa.add_instruction_form(" {imm5_u32_1_5_shift2}, {list12_u16_21_31_0}")
    form.add_instruction("dispose", "-----------000000000011001------")
    
    form = self.isa.add_instruction_form(" {imm5_u32_1_5_shift2}, {list12_u16_21_31_0}, [{reg1_u8_16_20}]")
    form.add_instruction("dispose", "----------------0000011001------")
    
    form = self.isa.add_instruction_form(" {imm6_u32_0_5_shift1}")
    form.add_instruction("callt", "0000001000------")
    
    form = self.isa.add_instruction_form(" {imm9_s32_18_21_0_4}, {reg2_u8_11_15}, {reg3_u8_27_31}")
    form.add_instruction("mul", "-----01001----00-----111111-----")
    
    form = self.isa.add_instruction_form(" {imm9_u32_18_21_0_4}, {reg2_u8_11_15}, {reg3_u8_27_31}")
    form.add_instruction("mulu", "-----01001----10-----111111-----")
    
    form = self.isa.add_instruction_form(" {list12_u16_21_31_0}, {imm5_u32_1_5_shift2}")
    form.add_instruction("prepare", "-----------000010000011110------")
    
    form = self.isa.add_instruction_form(" {list12_u16_21_31_0}, {imm5_u32_1_5_shift2}, {bits_ff_u8_19_20}")
    form.add_instruction("prepare", "-------------0110000011110------")
    
    form = self.isa.add_instruction_form(" {prefop_u8_27_31}, [{reg1_u8_0_4}]")
    form.add_instruction("pref", "-----0010110000011011111111-----")
    
    form = self.isa.add_instruction_form(" {reg1_u8_0_4}")
    form.add_instruction("switch", "00000000010-----")
    form.add_instruction("sxb", "00000000101-----")
    form.add_instruction("sxh", "00000000111-----")
    form.add_instruction("zxb", "00000000100-----")
    form.add_instruction("zxh", "00000000110-----")
    
    form = self.isa.add_instruction_form(" {reg1_u8_0_4}, {disp16_u32_17_31_shift1}")
    form.add_instruction("loop", "---------------100000110111-----")
    
    form = self.isa.add_instruction_form(" {reg1_u8_0_4}, {lsb_u32_27_17_19}, {msb_u32_28_31}, {reg2_u8_11_15}")
    form.add_instruction("binsl", "-----0001101---0-----111111-----")
    form.add_instruction("binsm", "-----0001011---0-----111111-----")
    form.add_instruction("binsu", "-----0001001---0-----111111-----")
    
    form = self.isa.add_instruction_form(" {reg1_u8_0_4}, {reg2_u8_11_15}")
    form.add_instruction("add", "-----001110-----")
    form.add_instruction("and", "-----001010-----")
    form.add_instruction("cmp", "-----001111-----")
    form.add_instruction("divh", "-----000010-----")
    form.add_instruction("mov", "-----000000-----")
    form.add_instruction("mulh", "-----000111-----")
    form.add_instruction("not", "-----000001-----")
    form.add_instruction("or", "-----001000-----")
    form.add_instruction("sar", "0000000010100000-----111111-----")
    form.add_instruction("satadd", "-----000110-----")
    form.add_instruction("satsub", "-----000101-----")
    form.add_instruction("satsubr", "-----000100-----")
    form.add_instruction("shl", "0000000011000000-----111111-----")
    form.add_instruction("shr", "0000000010000000-----111111-----")
    form.add_instruction("sttc.gr", "0000000001010010-----111111-----")
    form.add_instruction("sub", "-----001101-----")
    form.add_instruction("subr", "-----001100-----")
    form.add_instruction("tst", "-----001011-----")
    form.add_instruction("xor", "-----001001-----")
    
    form = self.isa.add_instruction_form(" {reg1_u8_0_4}, {reg2_u8_11_15}, {reg3_u8_27_31}")
    form.add_instruction("div", "-----01011000000-----111111-----")
    form.add_instruction("divh", "-----01010000000-----111111-----")
    form.add_instruction("divhu", "-----01010000010-----111111-----")
    form.add_instruction("divq", "-----01011111100-----111111-----")
    form.add_instruction("divqu", "-----01011111110-----111111-----")
    form.add_instruction("divu", "-----01011000010-----111111-----")
    form.add_instruction("mul", "-----01000100000-----111111-----")
    form.add_instruction("mulu", "-----01000100010-----111111-----")
    form.add_instruction("rotl", "-----00011000110-----111111-----")
    form.add_instruction("sar", "-----00010100010-----111111-----")
    form.add_instruction("satadd", "-----01110111010-----111111-----")
    form.add_instruction("satsub", "-----01110011010-----111111-----")
    form.add_instruction("shl", "-----00011000010-----111111-----")
    form.add_instruction("shr", "-----00010000010-----111111-----")
    
    form = self.isa.add_instruction_form(" {reg1_u8_0_4}, {reg2_u8_11_15}, {reg3_u8_27_31}, {reg4_u8_17_20_23}")
    form.add_instruction("maddf.s", "-----101-00----0-----111111-----")
    
    form = self.isa.add_instruction_form(" {reg1_u8_0_4}, {reg2_u8_11_15}, {reg3_u8_27_31}, {reg4_u8_17_20_shift1}")
    form.add_instruction("mac", "----0011110----0-----111111-----")
    form.add_instruction("macu", "----0011111----0-----111111-----")
    
    form = self.isa.add_instruction_form(" {reg1_u8_1_4_shift1}, {reg2_u8_12_15_shift1}, {reg3_u8_28_31_shift1}")
    form.add_instruction("maxf.d", "----010001111000----0111111----0")
    
    form = self.isa.add_instruction_form(" {reg2_u8_0_4}")
    form.add_instruction("ldtc.pc", "1111100000110010-----111111-----")
    
    form = self.isa.add_instruction_form(" {reg2_u8_0_4}, {reg1_u8_11_15}")
    form.add_instruction("ldtc.gr", "0000000000110010-----111111-----")
    
    form = self.isa.add_instruction_form(" {reg2_u8_0_4}, {regID_u8_11_15}, {selID_u8_27_31}")
    form.add_instruction("ldsr", "-----00000100000-----111111-----")
    form.add_instruction("ldtc.sr", "-----00000110000-----111111-----")
    form.add_instruction("ldvc.sr", "-----00000110100-----111111-----")
    
    form = self.isa.add_instruction_form(" {reg2_u8_0_4}, {vreg1_u8_11_15}")
    form.add_instruction("ldtc.vr", "0000100000110010-----111111-----")
    
    form = self.isa.add_instruction_form(" {reg2_u8_11_15}")
    form.add_instruction("sttc.pc", "1111100001010010-----111111-----")
    
    form = self.isa.add_instruction_form(" {reg2_u8_11_15}, [{reg1_u8_0_4}]")
    form.add_instruction("clr1", "0000000011100100-----111111-----")
    form.add_instruction("not1", "0000000011100010-----111111-----")
    form.add_instruction("set1", "0000000011100000-----111111-----")
    form.add_instruction("tst1", "0000000011100110-----111111-----")
    
    form = self.isa.add_instruction_form(" {reg2_u8_11_15}, {disp16_s32_16_31} [{reg1_u8_0_4}]")
    form.add_instruction("st.b", "---------------------111010-----")
    
    form = self.isa.add_instruction_form(" {reg2_u8_11_15}, {disp16_s32_17_31_shift1} [{reg1_u8_0_4}]")
    form.add_instruction("st.h", "---------------0-----111011-----")
    form.add_instruction("st.w", "---------------1-----111011-----")
    
    form = self.isa.add_instruction_form(" {reg2_u8_11_15}, {disp7_u32_0_6} [ep]")
    form.add_instruction("sst.b", "-----0111-------")
    
    form = self.isa.add_instruction_form(" {reg2_u8_11_15}, {disp8_u32_0_6_shift1} [ep]")
    form.add_instruction("sst.h", "-----1001-------")
    
    form = self.isa.add_instruction_form(" {reg2_u8_11_15}, {disp8_u32_1_6_shift2} [ep]")
    form.add_instruction("sst.w", "-----1010------1")
    
    form = self.isa.add_instruction_form(" {reg2_u8_11_15}, {reg3_u8_27_31}")
    form.add_instruction("bsh", "-----01101000010-----11111100000")
    form.add_instruction("bsw", "-----01101000000-----11111100000")
    form.add_instruction("hsh", "-----01101000110-----11111100000")
    form.add_instruction("hsw", "-----01101000100-----11111100000")
    form.add_instruction("sch0l", "-----01101100100-----11111100000")
    form.add_instruction("sch0r", "-----01101100000-----11111100000")
    form.add_instruction("sch1l", "-----01101100110-----11111100000")
    form.add_instruction("sch1r", "-----01101100010-----11111100000")
    
    form = self.isa.add_instruction_form(" {reg3_u8_27_31}, [{reg1_u8_0_4}]")
    form.add_instruction("stc.w", "-----0110111101000000111111-----")
    
    form = self.isa.add_instruction_form(" {reg3_u8_27_31}, {disp23_s32_32_47_20_26} [{reg1_u8_0_4}]")
    form.add_instruction("st.b", "----------------------------110100000111100-----")
    
    form = self.isa.add_instruction_form(" {reg3_u8_27_31}, {disp23_s32_32_47_21_26_shift1} [{reg1_u8_0_4}]")
    form.add_instruction("st.dw", "---------------------------0111100000111101-----")
    form.add_instruction("st.h", "---------------------------0110100000111101-----")
    form.add_instruction("st.w", "---------------------------0111100000111100-----")
    
    form = self.isa.add_instruction_form(" {regID_u8_0_4}, {reg2_u8_11_15}, {selID_u8_27_31}")
    form.add_instruction("stsr", "-----00001000000-----111111-----")
    form.add_instruction("sttc.sr", "-----00001010000-----111111-----")
    form.add_instruction("stvc.sr", "-----00001010100-----111111-----")
    
    form = self.isa.add_instruction_form(" {rh_u8_0_4}-{rt_u8_27_31}")
    form.add_instruction("dbpush", "-----0010110000001011111111-----")
    form.add_instruction("popsp", "-----0010110000001100111111-----")
    form.add_instruction("pushsp", "-----0010110000001000111111-----")
    
    form = self.isa.add_instruction_form(" {vector_u8_0_4}")
    form.add_instruction("hvtrap", "000000010001000000000111111-----")
    form.add_instruction("trap", "000000010000000000000111111-----")
    
    form = self.isa.add_instruction_form(" {vector_u8_11_14}")
    form.add_instruction("fetrap", "0----00001000000")
    
    form = self.isa.add_instruction_form(" {vector_u8_27_29_0_4}")
    form.add_instruction("hvcall", "01---0010110000011010111111-----")
    form.add_instruction("syscall", "00---0010110000011010111111-----")
    
    form = self.isa.add_instruction_form(" {vreg1_u8_0_4}, {reg2_u8_11_15}")
    form.add_instruction("sttc.vr", "0000100001010010-----111111-----")
    
    form = self.isa.add_instruction_form("{cccc_u8_0_3} {disp17_s32_4_17_31_shift1}")
    form.add_instruction("b", "---------------100000111111-----")
    
    form = self.isa.add_instruction_form("{cccc_u8_0_3} {disp9_s32_11_15_4_6_shift1}")
    form.add_instruction("b", "-----1011-------")
    
    self.isa.add_operand('bit_u8_11_13', '\w+', self.encode_bit_u8_11_13_value)
    self.isa.add_operand('bits_ff_u8_19_20', '\w+', self.encode_bits_ff_u8_19_20_value)
    self.isa.add_operand('cacheop_u8_11_12_27_31', '\w+', self.encode_cacheop_u8_11_12_27_31_value)
    self.isa.add_operand('cccc_u8_0_3', '\w+', self.encode_cccc_u8_0_3_value)
    self.isa.add_operand('cccc_u8_17_20', '\w+', self.encode_cccc_u8_17_20_value)
    self.isa.add_operand('disp16_s32_16_31', '\w+', self.encode_disp16_s32_16_31_value)
    self.isa.add_operand('disp16_s32_17_31_5', '\w+', self.encode_disp16_s32_17_31_5_value)
    self.isa.add_operand('disp16_s32_17_31_shift1', '\w+', self.encode_disp16_s32_17_31_shift1_value)
    self.isa.add_operand('disp16_u32_17_31_shift1', '\w+', self.encode_disp16_u32_17_31_shift1_value)
    self.isa.add_operand('disp17_s32_4_17_31_shift1', '\w+', self.encode_disp17_s32_4_17_31_shift1_value)
    self.isa.add_operand('disp22_s32_0_5_17_31_shift1', '\w+', self.encode_disp22_s32_0_5_17_31_shift1_value)
    self.isa.add_operand('disp23_s32_32_47_20_26', '\w+', self.encode_disp23_s32_32_47_20_26_value)
    self.isa.add_operand('disp23_s32_32_47_21_26_shift1', '\w+', self.encode_disp23_s32_32_47_21_26_shift1_value)
    self.isa.add_operand('disp32_s32_17_47_shift1', '\w+', self.encode_disp32_s32_17_47_shift1_value)
    self.isa.add_operand('disp4_u32_0_3', '\w+', self.encode_disp4_u32_0_3_value)
    self.isa.add_operand('disp5_u32_0_3_shift1', '\w+', self.encode_disp5_u32_0_3_shift1_value)
    self.isa.add_operand('disp7_u32_0_6', '\w+', self.encode_disp7_u32_0_6_value)
    self.isa.add_operand('disp8_u32_0_6_shift1', '\w+', self.encode_disp8_u32_0_6_shift1_value)
    self.isa.add_operand('disp8_u32_1_6_shift2', '\w+', self.encode_disp8_u32_1_6_shift2_value)
    self.isa.add_operand('disp9_s32_11_15_4_6_shift1', '\w+', self.encode_disp9_s32_11_15_4_6_shift1_value)
    self.isa.add_operand('fcbit_u8_17_19', '\w+', self.encode_fcbit_u8_17_19_value)
    self.isa.add_operand('fcond_u8_27_30', '\w+', self.encode_fcond_u8_27_30_value)
    self.isa.add_operand('imm10_u16_27_31_0_4', '\w+', self.encode_imm10_u16_27_31_0_4_value)
    self.isa.add_operand('imm16_s32_16_31', '\w+', self.encode_imm16_s32_16_31_value)
    self.isa.add_operand('imm16_u32_16_31', '\w+', self.encode_imm16_u32_16_31_value)
    self.isa.add_operand('imm16_u32_16_31_shift16', '\w+', self.encode_imm16_u32_16_31_shift16_value)
    self.isa.add_operand('imm32_s32_16_47', '\w+', self.encode_imm32_s32_16_47_value)
    self.isa.add_operand('imm4_u32_0_3', '\w+', self.encode_imm4_u32_0_3_value)
    self.isa.add_operand('imm5_s32_0_4', '\w+', self.encode_imm5_s32_0_4_value)
    self.isa.add_operand('imm5_u32_0_4', '\w+', self.encode_imm5_u32_0_4_value)
    self.isa.add_operand('imm5_u32_11_15', '\w+', self.encode_imm5_u32_11_15_value)
    self.isa.add_operand('imm5_u32_1_5_shift2', '\w+', self.encode_imm5_u32_1_5_shift2_value)
    self.isa.add_operand('imm6_u32_0_5_shift1', '\w+', self.encode_imm6_u32_0_5_shift1_value)
    self.isa.add_operand('imm9_s32_18_21_0_4', '\w+', self.encode_imm9_s32_18_21_0_4_value)
    self.isa.add_operand('imm9_u32_18_21_0_4', '\w+', self.encode_imm9_u32_18_21_0_4_value)
    self.isa.add_operand('list12_u16_21_31_0', '\w+', self.encode_list12_u16_21_31_0_value)
    self.isa.add_operand('lsb_u32_27_17_19', '\w+', self.encode_lsb_u32_27_17_19_value)
    self.isa.add_operand('msb_u32_28_31', '\w+', self.encode_msb_u32_28_31_value)
    self.isa.add_operand('prefop_u8_27_31', '\w+', self.encode_prefop_u8_27_31_value)
    self.isa.add_operand('reg1_u8_0_4', '|'.join(GR_group_name_index_map.keys()), self.encode_reg1_u8_0_4_value)
    self.isa.add_operand('reg1_u8_11_15', '|'.join(GR_group_name_index_map.keys()), self.encode_reg1_u8_11_15_value)
    self.isa.add_operand('reg1_u8_16_20', '|'.join(GR_group_name_index_map.keys()), self.encode_reg1_u8_16_20_value)
    self.isa.add_operand('reg1_u8_1_4_shift1', '|'.join(GR_group_name_index_map.keys()), self.encode_reg1_u8_1_4_shift1_value)
    self.isa.add_operand('reg2_u8_0_4', '|'.join(GR_group_name_index_map.keys()), self.encode_reg2_u8_0_4_value)
    self.isa.add_operand('reg2_u8_11_15', '|'.join(GR_group_name_index_map.keys()), self.encode_reg2_u8_11_15_value)
    self.isa.add_operand('reg2_u8_12_15_shift1', '|'.join(GR_group_name_index_map.keys()), self.encode_reg2_u8_12_15_shift1_value)
    self.isa.add_operand('reg3_u8_27_31', '|'.join(GR_group_name_index_map.keys()), self.encode_reg3_u8_27_31_value)
    self.isa.add_operand('reg3_u8_28_31_shift1', '|'.join(GR_group_name_index_map.keys()), self.encode_reg3_u8_28_31_shift1_value)
    self.isa.add_operand('reg4_u8_17_20_23', '|'.join(GR_group_name_index_map.keys()), self.encode_reg4_u8_17_20_23_value)
    self.isa.add_operand('reg4_u8_17_20_shift1', '|'.join(GR_group_name_index_map.keys()), self.encode_reg4_u8_17_20_shift1_value)
    self.isa.add_operand('regID_u8_0_4', '|'.join(SR_SELID_0_group_name_index_map.keys()), self.encode_regID_u8_0_4_value)
    self.isa.add_operand('regID_u8_11_15', '|'.join(SR_SELID_0_group_name_index_map.keys()), self.encode_regID_u8_11_15_value)
    self.isa.add_operand('rh_u8_0_4', '|'.join(GR_group_name_index_map.keys()), self.encode_rh_u8_0_4_value)
    self.isa.add_operand('rt_u8_27_31', '|'.join(GR_group_name_index_map.keys()), self.encode_rt_u8_27_31_value)
    self.isa.add_operand('selID_u8_27_31', '\w+', self.encode_selID_u8_27_31_value)
    self.isa.add_operand('vector_u8_0_4', '\w+', self.encode_vector_u8_0_4_value)
    self.isa.add_operand('vector_u8_11_14', '\w+', self.encode_vector_u8_11_14_value)
    self.isa.add_operand('vector_u8_27_29_0_4', '\w+', self.encode_vector_u8_27_29_0_4_value)
    self.isa.add_operand('vreg1_u8_0_4', '\w+', self.encode_vreg1_u8_0_4_value)
    self.isa.add_operand('vreg1_u8_11_15', '\w+', self.encode_vreg1_u8_11_15_value)
    
#------------------------------------------------------------------------------
# End of file
#------------------------------------------------------------------------------
