#------------------------------------------------------------------------------
# Copyright (C) Australian Semiconductor Technology Company. 2020.
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

WR_group_name_index_map = {
  'wr0' : 0,
  'wr0_u' : 1,
  'wr1' : 2,
  'wr1_u' : 3,
  'wr2' : 4,
  'wr2_u' : 5,
  'wr3' : 6,
  'wr3_u' : 7,
  'wr4' : 8,
  'wr4_u' : 9,
  'wr5' : 10,
  'wr5_u' : 11,
  'wr6' : 12,
  'wr6_u' : 13,
  'wr7' : 14,
  'wr7_u' : 15,
  'wr8' : 16,
  'wr8_u' : 17,
  'wr9' : 18,
  'wr9_u' : 19,
  'wr10' : 20,
  'wr10_u' : 21,
  'wr11' : 22,
  'wr11_u' : 23,
  'wr12' : 24,
  'wr12_u' : 25,
  'wr13' : 26,
  'wr13_u' : 27,
  'wr14' : 28,
  'wr14_u' : 29,
  'wr15' : 30,
  'wr15_u' : 31,
  'wr16' : 32,
  'wr16_u' : 33,
  'wr17' : 34,
  'wr17_u' : 35,
  'wr18' : 36,
  'wr18_u' : 37,
  'wr19' : 38,
  'wr19_u' : 39,
  'wr20' : 40,
  'wr20_u' : 41,
  'wr21' : 42,
  'wr21_u' : 43,
  'wr22' : 44,
  'wr22_u' : 45,
  'wr23' : 46,
  'wr23_u' : 47,
  'wr24' : 48,
  'wr24_u' : 49,
  'wr25' : 50,
  'wr25_u' : 51,
  'wr26' : 52,
  'wr26_u' : 53,
  'wr27' : 54,
  'wr27_u' : 55,
  'wr28' : 56,
  'wr28_u' : 57,
  'wr29' : 58,
  'wr29_u' : 59,
  'wr30' : 60,
  'wr30_u' : 61,
  'wr31' : 62,
  'wr31_u' : 63
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
  'EIIC' : 13,
  'FEIC' : 14,
  'PSWH' : 15,
  'CTPC' : 16,
  'CTPSW' : 17,
  'EIPSWH' : 18,
  'FEPSWH' : 19,
  'CTBP' : 20,
  'SNZCFG' : 21,
  'EIWR' : 28,
  'FEWR' : 29
}

SR_SELID_5_group_name_index_map = {
  'MPM' : 0,
  'MPCFG' : 2,
  'MCA' : 8,
  'MCS' : 9,
  'MCC' : 10,
  'MCR' : 11,
  'MCI' : 12,
  'MPIDX' : 16,
  'MPBK' : 17,
  'MPLA' : 20,
  'MPUA' : 21,
  'MPAT' : 22,
  'MPID0' : 24,
  'MPID1' : 25,
  'MPID2' : 26,
  'MPID3' : 27,
  'MPID4' : 28,
  'MPID5' : 29,
  'MPID6' : 30,
  'MPID7' : 31
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
  
  def encode_disp16_s32_34_47_shift2_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    operand_value = (operand_value >> 2)
    opcode_value |= (operand_value & 0x3fff) << 34
    return opcode_value
  
  def encode_disp16_s32_35_47_shift3_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    operand_value = (operand_value >> 3)
    opcode_value |= (operand_value & 0x1fff) << 35
    return opcode_value
  
  def encode_disp16_s32_36_47_shift4_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    operand_value = (operand_value >> 4)
    opcode_value |= (operand_value & 0xfff) << 36
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
  
  def encode_eh_u8_11_14_value(self, operand):
    opcode_value = 0
    operand_value = SR_SELID_5_group_name_index_map[operand]
    opcode_value |= (operand_value & 0xf) << 11
    return opcode_value
  
  def encode_et_u8_27_31_value(self, operand):
    opcode_value = 0
    operand_value = SR_SELID_5_group_name_index_map[operand]
    opcode_value |= (operand_value & 0x1f) << 27
    return opcode_value
  
  def encode_fcbit_u8_17_19_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0x7) << 17
    return opcode_value
  
  def encode_fcond_u8_17_20_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0xf) << 17
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
  
  def encode_imm12_u32_21_32_42_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0x7ff) << 32
    operand_value = (operand_value >> 11)
    opcode_value |= (operand_value & 0x1) << 21
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
  
  def encode_imm1_u32_21_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0x1) << 21
    return opcode_value
  
  def encode_imm2_u32_0_1_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= operand_value & 0x3
    return opcode_value
  
  def encode_imm2_u32_21_22_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0x3) << 21
    return opcode_value
  
  def encode_imm32_s32_16_47_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0xffffffff) << 16
    return opcode_value
  
  def encode_imm3_u32_27_29_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0x7) << 27
    return opcode_value
  
  def encode_imm4_u32_0_3_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= operand_value & 0xf
    return opcode_value
  
  def encode_imm4_u32_21_24_value(self, operand):
    opcode_value = 0
    operand_value = int(operand)
    opcode_value |= (operand_value & 0xf) << 21
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
  
  def encode_wreg1_u8_0_4_value(self, operand):
    opcode_value = 0
    operand_value = WR_group_name_index_map[operand]
    opcode_value |= operand_value & 0x1f
    return opcode_value
  
  def encode_wreg2_u8_11_15_value(self, operand):
    opcode_value = 0
    operand_value = WR_group_name_index_map[operand]
    opcode_value |= (operand_value & 0x1f) << 11
    return opcode_value
  
  def encode_wreg2_u8_43_47_value(self, operand):
    opcode_value = 0
    operand_value = WR_group_name_index_map[operand]
    opcode_value |= (operand_value & 0x1f) << 43
    return opcode_value
  
  def encode_wreg3_u8_27_31_value(self, operand):
    opcode_value = 0
    operand_value = WR_group_name_index_map[operand]
    opcode_value |= (operand_value & 0x1f) << 27
    return opcode_value
  
  def encode_wreg4_u8_32_36_value(self, operand):
    opcode_value = 0
    operand_value = WR_group_name_index_map[operand]
    opcode_value |= (operand_value & 0x1f) << 32
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
    form.add_instruction("dbret", "00000001010001100000011111100000")
    form.add_instruction("dbtrap", "1111100001000000")
    form.add_instruction("di", "00000001011000000000011111100000")
    form.add_instruction("ei", "00000001011000001000011111100000")
    form.add_instruction("eiret", "00000001010010000000011111100000")
    form.add_instruction("feret", "00000001010010100000011111100000")
    form.add_instruction("halt", "00000001001000000000011111100000")
    form.add_instruction("nop", "0000000000000000")
    form.add_instruction("resbank", "10000001011000000000011111100000")
    form.add_instruction("rie", "0000000001000000")
    form.add_instruction("snooze", "00000001001000000000111111100000")
    form.add_instruction("synce", "0000000000011101")
    form.add_instruction("synci", "0000000000011100")
    form.add_instruction("syncm", "0000000000011110")
    form.add_instruction("syncp", "0000000000011111")
    
    form = self.isa.add_instruction_form(" [{reg1_u8_0_4}]")
    form.add_instruction("jmp", "00000000011-----")
    form.add_instruction("ldm.gsr", "100110010110000000000111111-----")
    form.add_instruction("stm.gsr", "100100010110000000000111111-----")
    
    form = self.isa.add_instruction_form(" [{reg1_u8_0_4}]+, {reg3_u8_27_31}")
    form.add_instruction("ld.b", "-----0110111000000010111111-----")
    form.add_instruction("ld.bu", "-----0110111000000011111111-----")
    form.add_instruction("ld.h", "-----0110111010000010111111-----")
    form.add_instruction("ld.hu", "-----0110111010000011111111-----")
    form.add_instruction("ld.w", "-----0110111100000010111111-----")
    
    form = self.isa.add_instruction_form(" [{reg1_u8_0_4}], {eh_u8_11_14}-{et_u8_27_31}")
    form.add_instruction("ldm.mp", "-----00101100110-----111111-----")
    
    form = self.isa.add_instruction_form(" [{reg1_u8_0_4}], {reg2_u8_11_15}, {reg3_u8_27_31}")
    form.add_instruction("caxi", "-----00011101110-----111111-----")
    
    form = self.isa.add_instruction_form(" [{reg1_u8_0_4}], {reg3_u8_27_31}")
    form.add_instruction("jarl", "-----0010110000011000111111-----")
    form.add_instruction("ldl.bu", "-----0110111000000001111111-----")
    form.add_instruction("ldl.hu", "-----0110111010000001111111-----")
    form.add_instruction("ldl.w", "-----0110111100000000111111-----")
    
    form = self.isa.add_instruction_form(" [{reg1_u8_0_4}]-, {reg3_u8_27_31}")
    form.add_instruction("ld.b", "-----0110111000000100111111-----")
    form.add_instruction("ld.bu", "-----0110111000000101111111-----")
    form.add_instruction("ld.h", "-----0110111010000100111111-----")
    form.add_instruction("ld.hu", "-----0110111010000101111111-----")
    form.add_instruction("ld.w", "-----0110111100000100111111-----")
    
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
    
    form = self.isa.add_instruction_form(" {disp16_s32_35_47_shift3} [{reg1_u8_0_4}], {wreg3_u8_27_31}")
    form.add_instruction("ldvz.h4", "---------------------0111101110100000111101-----")
    
    form = self.isa.add_instruction_form(" {disp16_s32_36_47_shift4} [{reg1_u8_0_4}], {wreg3_u8_27_31}")
    form.add_instruction("ldv.qw", "---------------------0101001110100000111101-----")
    
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
    
    form = self.isa.add_instruction_form(" {eh_u8_11_14}-{et_u8_27_31}, [{reg1_u8_0_4}]")
    form.add_instruction("stm.mp", "-----00101100100-----111111-----")
    
    form = self.isa.add_instruction_form(" {fcbit_u8_17_19}")
    form.add_instruction("trfsr", "000001000000---00000011111100000")
    
    form = self.isa.add_instruction_form(" {fcbit_u8_17_19}, {reg1_u8_0_4}, {reg2_u8_11_15}, {reg3_u8_27_31}")
    form.add_instruction("cmovf.s", "-----1000000---0-----111111-----")
    
    form = self.isa.add_instruction_form(" {fcbit_u8_17_19}, {reg1_u8_1_4_shift1}, {reg2_u8_12_15_shift1}, {reg3_u8_28_31_shift1}")
    form.add_instruction("cmovf.d", "----01000001---0----0111111----0")
    
    form = self.isa.add_instruction_form(" {fcond_u8_17_20}, {wreg1_u8_0_4}, {wreg2_u8_11_15}, {wreg3_u8_27_31}")
    form.add_instruction("cmpf.s4", "-----101100----0-----111111-----")
    
    form = self.isa.add_instruction_form(" {fcond_u8_27_30}, {reg2_u8_11_15}, {reg1_u8_0_4}, {fcbit_u8_17_19}")
    form.add_instruction("cmpf.s", "0----1000010---0-----111111-----")
    
    form = self.isa.add_instruction_form(" {fcond_u8_27_30}, {reg2_u8_12_15_shift1}, {reg1_u8_1_4_shift1}, {fcbit_u8_17_19}")
    form.add_instruction("cmpf.d", "0----1000011---0----0111111----0")
    
    form = self.isa.add_instruction_form(" {imm10_u16_27_31_0_4}")
    form.add_instruction("dbtag", "-----0010110000011001111111-----")
    
    form = self.isa.add_instruction_form(" {imm12_u32_21_32_42}, {wreg1_u8_0_4}, {wreg2_u8_43_47}, {wreg3_u8_27_31}")
    form.add_instruction("shflv.w4", "---------------------11010-1110100000111101-----")
    
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
    
    form = self.isa.add_instruction_form(" {imm1_u32_21}, {wreg3_u8_27_31}, {disp16_s32_35_47_shift3} [{reg1_u8_0_4}]")
    form.add_instruction("stv.dw", "---------------------01110-1110100000111101-----")
    
    form = self.isa.add_instruction_form(" {imm2_u32_0_1}, {wreg2_u8_11_15}, {wreg3_u8_27_31}")
    form.add_instruction("flpv.s4", "-----10110100000-----111111110--")
    
    form = self.isa.add_instruction_form(" {imm2_u32_21_22}, {disp16_s32_35_47_shift3} [{reg1_u8_0_4}], {wreg3_u8_27_31}")
    form.add_instruction("ldv.dw", "---------------------0110--1110100000111101-----")
    
    form = self.isa.add_instruction_form(" {imm2_u32_21_22}, {wreg3_u8_27_31}, {disp16_s32_34_47_shift2} [{reg1_u8_0_4}]")
    form.add_instruction("stv.w", "---------------------0100--1110100000111101-----")
    
    form = self.isa.add_instruction_form(" {imm32_s32_16_47}, {reg1_u8_0_4}")
    form.add_instruction("mov", "--------------------------------00000110001-----")
    
    form = self.isa.add_instruction_form(" {imm3_u32_27_29}, {wreg2_u8_11_15}")
    form.add_instruction("trfsrv.w4", "00---10110100000-----11111111111")
    
    form = self.isa.add_instruction_form(" {imm4_u32_21_24}, {disp16_s32_34_47_shift2} [{reg1_u8_0_4}], {wreg3_u8_27_31}")
    form.add_instruction("ldv.w", "---------------------00----1110100000111101-----")
    
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
    form.add_instruction("clip.b", "0000000000001000-----111111-----")
    form.add_instruction("clip.bu", "0000000000001010-----111111-----")
    form.add_instruction("clip.h", "0000000000001100-----111111-----")
    form.add_instruction("clip.hu", "0000000000001110-----111111-----")
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
    form.add_instruction("sub", "-----001101-----")
    form.add_instruction("subr", "-----001100-----")
    form.add_instruction("tst", "-----001011-----")
    form.add_instruction("xor", "-----001001-----")
    
    form = self.isa.add_instruction_form(" {reg1_u8_0_4}, {reg2_u8_11_15}, {reg3_u8_27_31}")
    form.add_instruction("addf.s", "-----10001100000-----111111-----")
    form.add_instruction("div", "-----01011000000-----111111-----")
    form.add_instruction("divf.s", "-----10001101110-----111111-----")
    form.add_instruction("divh", "-----01010000000-----111111-----")
    form.add_instruction("divhu", "-----01010000010-----111111-----")
    form.add_instruction("divq", "-----01011111100-----111111-----")
    form.add_instruction("divqu", "-----01011111110-----111111-----")
    form.add_instruction("divu", "-----01011000010-----111111-----")
    form.add_instruction("fmaf.s", "-----10011100000-----111111-----")
    form.add_instruction("fmsf.s", "-----10011100010-----111111-----")
    form.add_instruction("fnmaf.s", "-----10011100100-----111111-----")
    form.add_instruction("fnmsf.s", "-----10011100110-----111111-----")
    form.add_instruction("maxf.s", "-----10001101000-----111111-----")
    form.add_instruction("minf.s", "-----10001101010-----111111-----")
    form.add_instruction("mul", "-----01000100000-----111111-----")
    form.add_instruction("mulf.s", "-----10001100100-----111111-----")
    form.add_instruction("mulu", "-----01000100010-----111111-----")
    form.add_instruction("rotl", "-----00011000110-----111111-----")
    form.add_instruction("sar", "-----00010100010-----111111-----")
    form.add_instruction("satadd", "-----01110111010-----111111-----")
    form.add_instruction("satsub", "-----01110011010-----111111-----")
    form.add_instruction("shl", "-----00011000010-----111111-----")
    form.add_instruction("shr", "-----00010000010-----111111-----")
    form.add_instruction("subf.s", "-----10001100010-----111111-----")
    
    form = self.isa.add_instruction_form(" {reg1_u8_0_4}, {reg2_u8_11_15}, {reg3_u8_27_31}, {reg4_u8_17_20_23}")
    form.add_instruction("maddf.s", "-----101-00----0-----111111-----")
    form.add_instruction("msubf.s", "-----101-01----0-----111111-----")
    form.add_instruction("nmaddf.s", "-----101-10----0-----111111-----")
    form.add_instruction("nmsubf.s", "-----101-11----0-----111111-----")
    
    form = self.isa.add_instruction_form(" {reg1_u8_0_4}, {reg2_u8_11_15}, {reg3_u8_27_31}, {reg4_u8_17_20_shift1}")
    form.add_instruction("mac", "----0011110----0-----111111-----")
    form.add_instruction("macu", "----0011111----0-----111111-----")
    
    form = self.isa.add_instruction_form(" {reg1_u8_1_4_shift1}, {reg2_u8_12_15_shift1}, {reg3_u8_28_31_shift1}")
    form.add_instruction("addf.d", "----010001110000----0111111----0")
    form.add_instruction("divf.d", "----010001111110----0111111----0")
    form.add_instruction("maxf.d", "----010001111000----0111111----0")
    form.add_instruction("minf.d", "----010001111010----0111111----0")
    form.add_instruction("mulf.d", "----010001110100----0111111----0")
    form.add_instruction("subf.d", "----010001110010----0111111----0")
    
    form = self.isa.add_instruction_form(" {reg2_u8_0_4}, {regID_u8_11_15}, {selID_u8_27_31}")
    form.add_instruction("ldsr", "-----00000100000-----111111-----")
    
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
    form.add_instruction("absf.s", "-----10001001000-----11111100000")
    form.add_instruction("bsh", "-----01101000010-----11111100000")
    form.add_instruction("bsw", "-----01101000000-----11111100000")
    form.add_instruction("ceilf.suw", "-----10001000000-----11111110010")
    form.add_instruction("ceilf.sw", "-----10001000000-----11111100010")
    form.add_instruction("cvtf.hs", "-----10001000010-----111111---10")
    form.add_instruction("cvtf.sh", "-----10001000010-----111111---11")
    form.add_instruction("cvtf.suw", "-----10001000000-----11111110100")
    form.add_instruction("cvtf.sw", "-----10001000000-----11111100100")
    form.add_instruction("cvtf.uws", "-----10001000010-----11111110000")
    form.add_instruction("cvtf.ws", "-----10001000010-----11111100000")
    form.add_instruction("floorf.suw", "-----10001000000-----11111110011")
    form.add_instruction("floorf.sw", "-----10001000000-----11111100011")
    form.add_instruction("hsh", "-----01101000110-----11111100000")
    form.add_instruction("hsw", "-----01101000100-----11111100000")
    form.add_instruction("negf.s", "-----10001001000-----11111100001")
    form.add_instruction("recipf.s", "-----10001001110-----11111100001")
    form.add_instruction("roundf.suw", "-----10001000000-----11111110000")
    form.add_instruction("roundf.sw", "-----10001000000-----11111100000")
    form.add_instruction("rsqrtf.s", "-----10001001110-----11111100010")
    form.add_instruction("sch0l", "-----01101100100-----11111100000")
    form.add_instruction("sch0r", "-----01101100000-----11111100000")
    form.add_instruction("sch1l", "-----01101100110-----11111100000")
    form.add_instruction("sch1r", "-----01101100010-----11111100000")
    form.add_instruction("sqrtf.s", "-----10001001110-----11111100000")
    form.add_instruction("trncf.suw", "-----10001000000-----11111110001")
    form.add_instruction("trncf.sw", "-----10001000000-----11111100001")
    
    form = self.isa.add_instruction_form(" {reg2_u8_11_15}, {reg3_u8_28_31_shift1}")
    form.add_instruction("ceilf.sl", "----010001000100-----11111100010")
    form.add_instruction("ceilf.sul", "----010001000100-----11111110010")
    form.add_instruction("cvtf.sd", "----010001010010-----11111100010")
    form.add_instruction("cvtf.sl", "----010001000100-----11111100100")
    form.add_instruction("cvtf.sul", "----010001000100-----11111110100")
    form.add_instruction("cvtf.uwd", "----010001010010-----11111110000")
    form.add_instruction("cvtf.wd", "----010001010010-----11111100000")
    form.add_instruction("floorf.sl", "----010001000100-----11111100011")
    form.add_instruction("floorf.sul", "----010001000100-----11111110011")
    form.add_instruction("roundf.sl", "----010001000100-----11111100000")
    form.add_instruction("roundf.sul", "----010001000100-----11111110000")
    form.add_instruction("trncf.sl", "----010001000100-----11111100001")
    form.add_instruction("trncf.sul", "----010001000100-----11111110001")
    
    form = self.isa.add_instruction_form(" {reg2_u8_12_15_shift1}, {reg3_u8_27_31}")
    form.add_instruction("ceilf.duw", "-----10001010000----011111110010")
    form.add_instruction("ceilf.dw", "-----10001010000----011111100010")
    form.add_instruction("cvtf.ds", "-----10001010010----011111100011")
    form.add_instruction("cvtf.duw", "-----10001010000----011111110100")
    form.add_instruction("cvtf.dw", "-----10001010000----011111100100")
    form.add_instruction("cvtf.ls", "-----10001000010----011111100001")
    form.add_instruction("cvtf.uls", "-----10001000010----011111110001")
    form.add_instruction("floorf.duw", "-----10001010000----011111110011")
    form.add_instruction("floorf.dw", "-----10001010000----011111100011")
    form.add_instruction("roundf.duw", "-----10001010000----011111110000")
    form.add_instruction("roundf.dw", "-----10001010000----011111100000")
    form.add_instruction("trncf.duw", "-----10001010000----011111110001")
    form.add_instruction("trncf.dw", "-----10001010000----011111100001")
    
    form = self.isa.add_instruction_form(" {reg2_u8_12_15_shift1}, {reg3_u8_28_31_shift1}")
    form.add_instruction("absf.d", "----010001011000----011111100000")
    form.add_instruction("ceilf.dl", "----010001010100----011111100010")
    form.add_instruction("ceilf.dul", "----010001010100----011111110010")
    form.add_instruction("cvtf.dl", "----010001010100----011111100100")
    form.add_instruction("cvtf.dul", "----010001010100----011111110100")
    form.add_instruction("cvtf.ld", "----010001010010----011111100001")
    form.add_instruction("cvtf.uld", "----010001010010----011111110001")
    form.add_instruction("floorf.dl", "----010001010100----011111100011")
    form.add_instruction("floorf.dul", "----010001010100----011111110011")
    form.add_instruction("negf.d", "----010001011000----011111100001")
    form.add_instruction("recipf.d", "----010001011110----011111100001")
    form.add_instruction("roundf.dl", "----010001010100----011111100000")
    form.add_instruction("roundf.dul", "----010001010100----011111110000")
    form.add_instruction("rsqrtf.d", "----010001011110----011111100010")
    form.add_instruction("sqrtf.d", "----010001011110----011111100000")
    form.add_instruction("trncf.dl", "----010001010100----011111100001")
    form.add_instruction("trncf.dul", "----010001010100----011111110001")
    
    form = self.isa.add_instruction_form(" {reg3_u8_27_31}, [{reg1_u8_0_4}]")
    form.add_instruction("stc.b", "-----0110111001000000111111-----")
    form.add_instruction("stc.h", "-----0110111011000000111111-----")
    form.add_instruction("stc.w", "-----0110111101000000111111-----")
    
    form = self.isa.add_instruction_form(" {reg3_u8_27_31}, [{reg1_u8_0_4}]+")
    form.add_instruction("st.b", "-----0110111001000010111111-----")
    form.add_instruction("st.h", "-----0110111011000010111111-----")
    form.add_instruction("st.w", "-----0110111101000010111111-----")
    
    form = self.isa.add_instruction_form(" {reg3_u8_27_31}, [{reg1_u8_0_4}]-")
    form.add_instruction("st.b", "-----0110111001000100111111-----")
    form.add_instruction("st.h", "-----0110111011000100111111-----")
    form.add_instruction("st.w", "-----0110111101000100111111-----")
    
    form = self.isa.add_instruction_form(" {reg3_u8_27_31}, {disp23_s32_32_47_20_26} [{reg1_u8_0_4}]")
    form.add_instruction("st.b", "----------------------------110100000111100-----")
    
    form = self.isa.add_instruction_form(" {reg3_u8_27_31}, {disp23_s32_32_47_21_26_shift1} [{reg1_u8_0_4}]")
    form.add_instruction("st.dw", "---------------------------0111100000111101-----")
    form.add_instruction("st.h", "---------------------------0110100000111101-----")
    form.add_instruction("st.w", "---------------------------0111100000111100-----")
    
    form = self.isa.add_instruction_form(" {regID_u8_0_4}, {reg2_u8_11_15}, {selID_u8_27_31}")
    form.add_instruction("stsr", "-----00001000000-----111111-----")
    
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
    form.add_instruction("syscall", "00---0010110000011010111111-----")
    
    form = self.isa.add_instruction_form(" {wreg1_u8_0_4}, {wreg2_u8_11_15}, {wreg3_u8_27_31}")
    form.add_instruction("addf.s4", "-----10110100100-----111111-----")
    form.add_instruction("addrf.s4", "-----10110110100-----111111-----")
    form.add_instruction("addsubf.s4", "-----10111010000-----111111-----")
    form.add_instruction("addsubnf.s4", "-----10111011000-----111111-----")
    form.add_instruction("addsubnxf.s4", "-----10111011100-----111111-----")
    form.add_instruction("addsubxf.s4", "-----10111010100-----111111-----")
    form.add_instruction("addxf.s4", "-----10111000100-----111111-----")
    form.add_instruction("divf.s4", "-----10110101110-----111111-----")
    form.add_instruction("fmaf.s4", "-----10011000000-----111111-----")
    form.add_instruction("fmsf.s4", "-----10011000010-----111111-----")
    form.add_instruction("fnmaf.s4", "-----10011000100-----111111-----")
    form.add_instruction("fnmsf.s4", "-----10011000110-----111111-----")
    form.add_instruction("maxf.s4", "-----10110101010-----111111-----")
    form.add_instruction("maxrf.s4", "-----10110111010-----111111-----")
    form.add_instruction("minf.s4", "-----10110101100-----111111-----")
    form.add_instruction("minrf.s4", "-----10110111100-----111111-----")
    form.add_instruction("mulf.s4", "-----10110101000-----111111-----")
    form.add_instruction("mulrf.s4", "-----10110111000-----111111-----")
    form.add_instruction("mulxf.s4", "-----10111001000-----111111-----")
    form.add_instruction("subaddf.s4", "-----10111010010-----111111-----")
    form.add_instruction("subaddnf.s4", "-----10111011010-----111111-----")
    form.add_instruction("subaddnxf.s4", "-----10111011110-----111111-----")
    form.add_instruction("subaddxf.s4", "-----10111010110-----111111-----")
    form.add_instruction("subf.s4", "-----10110100110-----111111-----")
    form.add_instruction("subrf.s4", "-----10110110110-----111111-----")
    form.add_instruction("subxf.s4", "-----10111000110-----111111-----")
    
    form = self.isa.add_instruction_form(" {wreg2_u8_11_15}, {wreg3_u8_27_31}")
    form.add_instruction("absf.s4", "-----10110100000-----11111110000")
    form.add_instruction("ceilf.suw4", "-----10110100000-----11111100101")
    form.add_instruction("ceilf.sw4", "-----10110100000-----11111100100")
    form.add_instruction("cvtf.hs4", "-----10110100000-----11111101100")
    form.add_instruction("cvtf.sh4", "-----10110100000-----11111101101")
    form.add_instruction("cvtf.suw4", "-----10110100000-----11111101001")
    form.add_instruction("cvtf.sw4", "-----10110100000-----11111101000")
    form.add_instruction("cvtf.uws4", "-----10110100000-----11111101011")
    form.add_instruction("cvtf.ws4", "-----10110100000-----11111101010")
    form.add_instruction("floorf.suw4", "-----10110100000-----11111100111")
    form.add_instruction("floorf.sw4", "-----10110100000-----11111100110")
    form.add_instruction("movv.w4", "-----10110100000-----11111111110")
    form.add_instruction("negf.s4", "-----10110100000-----11111110001")
    form.add_instruction("recipf.s4", "-----10110100000-----11111110011")
    form.add_instruction("roundf.suw4", "-----10110100000-----11111100001")
    form.add_instruction("roundf.sw4", "-----10110100000-----11111100000")
    form.add_instruction("rsqrtf.s4", "-----10110100000-----11111110100")
    form.add_instruction("sqrtf.s4", "-----10110100000-----11111110010")
    form.add_instruction("trncf.suw4", "-----10110100000-----11111100011")
    form.add_instruction("trncf.sw4", "-----10110100000-----11111100010")
    
    form = self.isa.add_instruction_form(" {wreg3_u8_27_31}, {disp16_s32_35_47_shift3} [{reg1_u8_0_4}]")
    form.add_instruction("stvz.h4", "---------------------0111111110100000111101-----")
    
    form = self.isa.add_instruction_form(" {wreg3_u8_27_31}, {disp16_s32_36_47_shift4} [{reg1_u8_0_4}]")
    form.add_instruction("stv.qw", "---------------------0101011110100000111101-----")
    
    form = self.isa.add_instruction_form(" {wreg4_u8_32_36}, {wreg1_u8_0_4}, {wreg2_u8_43_47}, {wreg3_u8_27_31}")
    form.add_instruction("cmovf.w4", "---------------------1100001110100000111101-----")
    
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
    self.isa.add_operand('disp16_s32_34_47_shift2', '\w+', self.encode_disp16_s32_34_47_shift2_value)
    self.isa.add_operand('disp16_s32_35_47_shift3', '\w+', self.encode_disp16_s32_35_47_shift3_value)
    self.isa.add_operand('disp16_s32_36_47_shift4', '\w+', self.encode_disp16_s32_36_47_shift4_value)
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
    self.isa.add_operand('eh_u8_11_14', '|'.join(SR_SELID_5_group_name_index_map.keys()), self.encode_eh_u8_11_14_value)
    self.isa.add_operand('et_u8_27_31', '|'.join(SR_SELID_5_group_name_index_map.keys()), self.encode_et_u8_27_31_value)
    self.isa.add_operand('fcbit_u8_17_19', '\w+', self.encode_fcbit_u8_17_19_value)
    self.isa.add_operand('fcond_u8_17_20', '\w+', self.encode_fcond_u8_17_20_value)
    self.isa.add_operand('fcond_u8_27_30', '\w+', self.encode_fcond_u8_27_30_value)
    self.isa.add_operand('imm10_u16_27_31_0_4', '\w+', self.encode_imm10_u16_27_31_0_4_value)
    self.isa.add_operand('imm12_u32_21_32_42', '\w+', self.encode_imm12_u32_21_32_42_value)
    self.isa.add_operand('imm16_s32_16_31', '\w+', self.encode_imm16_s32_16_31_value)
    self.isa.add_operand('imm16_u32_16_31', '\w+', self.encode_imm16_u32_16_31_value)
    self.isa.add_operand('imm16_u32_16_31_shift16', '\w+', self.encode_imm16_u32_16_31_shift16_value)
    self.isa.add_operand('imm1_u32_21', '\w+', self.encode_imm1_u32_21_value)
    self.isa.add_operand('imm2_u32_0_1', '\w+', self.encode_imm2_u32_0_1_value)
    self.isa.add_operand('imm2_u32_21_22', '\w+', self.encode_imm2_u32_21_22_value)
    self.isa.add_operand('imm32_s32_16_47', '\w+', self.encode_imm32_s32_16_47_value)
    self.isa.add_operand('imm3_u32_27_29', '\w+', self.encode_imm3_u32_27_29_value)
    self.isa.add_operand('imm4_u32_0_3', '\w+', self.encode_imm4_u32_0_3_value)
    self.isa.add_operand('imm4_u32_21_24', '\w+', self.encode_imm4_u32_21_24_value)
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
    self.isa.add_operand('wreg1_u8_0_4', '|'.join(WR_group_name_index_map.keys()), self.encode_wreg1_u8_0_4_value)
    self.isa.add_operand('wreg2_u8_11_15', '|'.join(WR_group_name_index_map.keys()), self.encode_wreg2_u8_11_15_value)
    self.isa.add_operand('wreg2_u8_43_47', '|'.join(WR_group_name_index_map.keys()), self.encode_wreg2_u8_43_47_value)
    self.isa.add_operand('wreg3_u8_27_31', '|'.join(WR_group_name_index_map.keys()), self.encode_wreg3_u8_27_31_value)
    self.isa.add_operand('wreg4_u8_32_36', '|'.join(WR_group_name_index_map.keys()), self.encode_wreg4_u8_32_36_value)
    
#------------------------------------------------------------------------------
# End of file
#------------------------------------------------------------------------------
