#! python

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
import vlab
import struct

#------------------------------------------------------------------------------
# Register group mappings
#------------------------------------------------------------------------------

GR_group_index_name_map = {
  0 : 'r0',
  1 : 'r1',
  2 : 'r2',
  3 : 'r3',
  4 : 'r4',
  5 : 'r5',
  6 : 'r6',
  7 : 'r7',
  8 : 'r8',
  9 : 'r9',
  10 : 'r10',
  11 : 'r11',
  12 : 'r12',
  13 : 'r13',
  14 : 'r14',
  15 : 'r15',
  16 : 'r16',
  17 : 'r17',
  18 : 'r18',
  19 : 'r19',
  20 : 'r20',
  21 : 'r21',
  22 : 'r22',
  23 : 'r23',
  24 : 'r24',
  25 : 'r25',
  26 : 'r26',
  27 : 'r27',
  28 : 'r28',
  29 : 'r29',
  30 : 'r30',
  31 : 'r31'
}

WR_group_index_name_map = {
  0 : 'wr0',
  1 : 'wr0_u',
  2 : 'wr1',
  3 : 'wr1_u',
  4 : 'wr2',
  5 : 'wr2_u',
  6 : 'wr3',
  7 : 'wr3_u',
  8 : 'wr4',
  9 : 'wr4_u',
  10 : 'wr5',
  11 : 'wr5_u',
  12 : 'wr6',
  13 : 'wr6_u',
  14 : 'wr7',
  15 : 'wr7_u',
  16 : 'wr8',
  17 : 'wr8_u',
  18 : 'wr9',
  19 : 'wr9_u',
  20 : 'wr10',
  21 : 'wr10_u',
  22 : 'wr11',
  23 : 'wr11_u',
  24 : 'wr12',
  25 : 'wr12_u',
  26 : 'wr13',
  27 : 'wr13_u',
  28 : 'wr14',
  29 : 'wr14_u',
  30 : 'wr15',
  31 : 'wr15_u',
  32 : 'wr16',
  33 : 'wr16_u',
  34 : 'wr17',
  35 : 'wr17_u',
  36 : 'wr18',
  37 : 'wr18_u',
  38 : 'wr19',
  39 : 'wr19_u',
  40 : 'wr20',
  41 : 'wr20_u',
  42 : 'wr21',
  43 : 'wr21_u',
  44 : 'wr22',
  45 : 'wr22_u',
  46 : 'wr23',
  47 : 'wr23_u',
  48 : 'wr24',
  49 : 'wr24_u',
  50 : 'wr25',
  51 : 'wr25_u',
  52 : 'wr26',
  53 : 'wr26_u',
  54 : 'wr27',
  55 : 'wr27_u',
  56 : 'wr28',
  57 : 'wr28_u',
  58 : 'wr29',
  59 : 'wr29_u',
  60 : 'wr30',
  61 : 'wr30_u',
  62 : 'wr31',
  63 : 'wr31_u'
}

SR_SELID_0_group_index_name_map = {
  0 : 'EIPC',
  1 : 'EIPSW',
  2 : 'FEPC',
  3 : 'FEPSW',
  5 : 'PSW',
  6 : 'FPSR',
  7 : 'FPEPC',
  8 : 'FPST',
  9 : 'FPCC',
  10 : 'FPCFG',
  13 : 'EIIC',
  14 : 'FEIC',
  15 : 'PSWH',
  16 : 'CTPC',
  17 : 'CTPSW',
  18 : 'EIPSWH',
  19 : 'FEPSWH',
  20 : 'CTBP',
  21 : 'SNZCFG',
  28 : 'EIWR',
  29 : 'FEWR'
}

SR_SELID_5_group_index_name_map = {
  0 : 'MPM',
  2 : 'MPCFG',
  8 : 'MCA',
  9 : 'MCS',
  10 : 'MCC',
  11 : 'MCR',
  12 : 'MCI',
  16 : 'MPIDX',
  17 : 'MPBK',
  20 : 'MPLA',
  21 : 'MPUA',
  22 : 'MPAT',
  24 : 'MPID0',
  25 : 'MPID1',
  26 : 'MPID2',
  27 : 'MPID3',
  28 : 'MPID4',
  29 : 'MPID5',
  30 : 'MPID6',
  31 : 'MPID7'
}

#------------------------------------------------------------------------------
# Disassembler
#------------------------------------------------------------------------------

class disassembler(vlab.Disassembler):
  # Instruction form nested class
  class instruction_form(object):
    def __init__(self, operand_pattern):
      self.operand_pattern = operand_pattern
      self.instruction_info = []
 
    def add_instruction(self, mnemonic, bit_pattern, address_calculation_list=None):
      opcode_mask_pattern = bit_pattern.translate(string.maketrans("01-", "110"), " ")
      opcode_value_pattern = bit_pattern.translate(string.maketrans("01-", "010"), " ")
      opcode_mask_value = int(opcode_mask_pattern, 2)
      opcode_value_value = int(opcode_value_pattern, 2)
      opcode_len = len(opcode_value_pattern)
      self.instruction_info.append((opcode_mask_value, opcode_value_value, opcode_len / 8, mnemonic, address_calculation_list))
                  
  # Instruction set nested class
  class instruction_set(object):
    def __init__(self):
      self.form_list = []
      self.operand_map = {}
      self.instruction_info = []
      
    def add_instruction_form(self, operand_pattern):
      form = disassembler.instruction_form(operand_pattern)
      self.form_list.append(form)
      return form
      
    def initialise(self):
      for form in self.form_list:
        for (opcode_mask, opcode_pattern, opcode_len, mnemonic, address_calculation_list) in form.instruction_info:
          self.instruction_info.append((opcode_mask, opcode_pattern, opcode_len, (mnemonic + form.operand_pattern), address_calculation_list))
              
    def disassemble(self, opcode, diassembler):
      disassemble_match = False
      for (opcode_mask, opcode_pattern, opcode_len, mnemonic, address_calculation_list) in self.instruction_info:
        if (opcode & opcode_mask) == opcode_pattern:
          disassemble_match = True
          break
      if disassemble_match == False:
        return (None, 4)
      try:
        operand_name_list = re.findall("\{(\S*?)\}", mnemonic)
        operand_value_map = {}
        for operand_name in operand_name_list:
          operand_value_map[operand_name] = eval("diassembler.decode_%s(opcode)" % operand_name)
        for operand_name in operand_name_list:
          operand_value = eval("diassembler.render_%s(operand_value_map)" % operand_name)
          mnemonic = mnemonic.replace("{%s}" % operand_name, operand_value)
        if address_calculation_list != None:
          address_value_list = []
          for address_calculation in address_calculation_list:
            address_value = eval("diassembler.calculate_address_%s(operand_value_map)" % address_calculation)
            if address_value != None:
              address_value_list.append(address_value)
          if len(address_value_list) > 0:
            mnemonic = mnemonic + " ; " + " ".join(map(lambda x: str(vlab.get_symbol(x, subject=diassembler.core, space=diassembler.space)), address_value_list))
        return (mnemonic, opcode_len)
      except:
        pass
      # Only path here is if decode or render operations cause an exception,
      # typically this occurs if a wildcard bit pattern is overly permissive
      # and then the decode or render operation fails
      return (None, 4)

  # Disassembler methods
  def _extract_field(self, data, start_pos, end_pos):
    data_len = len(data)
    return data[(data_len - end_pos - 1):(data_len - start_pos)]
   
  def initialise(self):
    self.isa.initialise()

  def disassemble(self, opcode):
    return self.isa.disassemble(opcode, self)
	
  def disassemble_range(self, address, size, core, space): 
    # print "disassemble_range()"
    # print "  address=0x%08x" % address
    # print "  size=%d" % size
    data = vlab.ui.read_memory(address, size, core, space) 
    # print "  read=%d" % len(data)
    return self.disassemble_buffer(data, address, core, space) 

  def disassemble_buffer(self, data, address, core=None, space=None):
    # print "disassemble_buffer()"
    # print "  address=0x%08x" % address
    self.address = address
    self.core = core
    self.space = space
    len_data = len(data)
    offset = 0
    while (offset + 2) <= len_data:
      if len_data - offset >= 8:
        (mnemonic, opcode_len) = self.disassemble(struct.unpack_from("<Q", data, offset=offset)[0])
      elif len_data - offset >= 4:
        (mnemonic, opcode_len) = self.disassemble((struct.unpack_from("<I", data, offset=offset)[0]))
      else:
        (mnemonic, opcode_len) = self.disassemble((struct.unpack_from("<H", data, offset=offset)[0]))
      if mnemonic == None:
        yield (self.address + offset, 2, None, None)
        offset += 2
      else:
        disasm = mnemonic.split(None, 1)
        disasm_fields = len(disasm)
        if (disasm_fields == 0):
          yield (self.address + offset, opcode_len, None, None) 
        elif (disasm_fields == 1):
          yield (self.address + offset, opcode_len, disasm[0], "") 
        else:
          yield (self.address + offset, opcode_len, disasm[0], disasm[1]) 
        offset += opcode_len
    
  def __init__(self, isa_name):
    super(disassembler, self).__init__(isa_name)
    self.isa_name = isa_name
    self.isa = None
    self.address = None
    self.core = None
    self.space = None
    
  # Operand extraction operations
  def decode_bit_u8_11_13(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=5)
    operand_bits.append(self._extract_field(data_bits, 11, 13))
    return operand_bits.uint
  
  def decode_bits_ff_u8_19_20(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=6)
    operand_bits.append(self._extract_field(data_bits, 19, 20))
    return operand_bits.uint
  
  def decode_cccc_u8_0_3(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=4)
    operand_bits.append(self._extract_field(data_bits, 0, 3))
    return operand_bits.uint
  
  def decode_cccc_u8_17_20(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=4)
    operand_bits.append(self._extract_field(data_bits, 17, 20))
    return operand_bits.uint
  
  def decode_fcbit_u8_17_19(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=5)
    operand_bits.append(self._extract_field(data_bits, 17, 19))
    return operand_bits.uint
  
  def decode_fcond_u8_27_30(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=4)
    operand_bits.append(self._extract_field(data_bits, 27, 30))
    return operand_bits.uint
  
  def decode_disp4_u32_0_3(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=28)
    operand_bits.append(self._extract_field(data_bits, 0, 3))
    return operand_bits.uint
  
  def decode_disp5_u32_0_3_shift1(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=28)
    operand_bits.append(self._extract_field(data_bits, 0, 3))
    operand_bits <<= 1
    return operand_bits.uint
  
  def decode_disp7_u32_0_6(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=25)
    operand_bits.append(self._extract_field(data_bits, 0, 6))
    return operand_bits.uint
  
  def decode_disp8_u32_0_6_shift1(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=25)
    operand_bits.append(self._extract_field(data_bits, 0, 6))
    operand_bits <<= 1
    return operand_bits.uint
  
  def decode_disp8_u32_1_6_shift2(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=26)
    operand_bits.append(self._extract_field(data_bits, 1, 6))
    operand_bits <<= 2
    return operand_bits.uint
  
  def decode_disp9_s32_11_15_4_6_shift1(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=24)
    operand_bits.append(self._extract_field(data_bits, 11, 15))
    operand_bits.append(self._extract_field(data_bits, 4, 6))
    operand_bits.set(operand_bits[-8], range(0, 24))
    operand_bits <<= 1
    return operand_bits.int
  
  def decode_disp16_s32_16_31(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=16)
    operand_bits.append(self._extract_field(data_bits, 16, 31))
    operand_bits.set(operand_bits[-16], range(0, 16))
    return operand_bits.int
  
  def decode_disp16_s32_17_31_5(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=16)
    operand_bits.append(self._extract_field(data_bits, 17, 31))
    operand_bits.append(self._extract_field(data_bits, 5, 5))
    operand_bits.set(operand_bits[-16], range(0, 16))
    return operand_bits.int
  
  def decode_disp16_s32_17_31_shift1(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=17)
    operand_bits.append(self._extract_field(data_bits, 17, 31))
    operand_bits.set(operand_bits[-15], range(0, 17))
    operand_bits <<= 1
    return operand_bits.int
  
  def decode_disp22_s32_0_5_17_31_shift1(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=11)
    operand_bits.append(self._extract_field(data_bits, 0, 5))
    operand_bits.append(self._extract_field(data_bits, 17, 31))
    operand_bits.set(operand_bits[-21], range(0, 11))
    operand_bits <<= 1
    return operand_bits.int
  
  def decode_disp23_s32_32_47_20_26(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=9)
    operand_bits.append(self._extract_field(data_bits, 32, 47))
    operand_bits.append(self._extract_field(data_bits, 20, 26))
    operand_bits.set(operand_bits[-23], range(0, 9))
    return operand_bits.int
  
  def decode_disp23_s32_32_47_21_26_shift1(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=10)
    operand_bits.append(self._extract_field(data_bits, 32, 47))
    operand_bits.append(self._extract_field(data_bits, 21, 26))
    operand_bits.set(operand_bits[-22], range(0, 10))
    operand_bits <<= 1
    return operand_bits.int
  
  def decode_disp32_s32_17_47_shift1(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=1)
    operand_bits.append(self._extract_field(data_bits, 17, 47))
    operand_bits.set(operand_bits[-31], range(0, 1))
    operand_bits <<= 1
    return operand_bits.int
  
  def decode_imm4_u32_0_3(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=28)
    operand_bits.append(self._extract_field(data_bits, 0, 3))
    return operand_bits.uint
  
  def decode_imm5_s32_0_4(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=27)
    operand_bits.append(self._extract_field(data_bits, 0, 4))
    operand_bits.set(operand_bits[-5], range(0, 27))
    return operand_bits.int
  
  def decode_imm5_u32_11_15(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=27)
    operand_bits.append(self._extract_field(data_bits, 11, 15))
    return operand_bits.uint
  
  def decode_imm5_u32_0_4(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=27)
    operand_bits.append(self._extract_field(data_bits, 0, 4))
    return operand_bits.uint
  
  def decode_imm5_u32_1_5_shift2(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=27)
    operand_bits.append(self._extract_field(data_bits, 1, 5))
    operand_bits <<= 2
    return operand_bits.uint
  
  def decode_imm6_u32_0_5_shift1(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=26)
    operand_bits.append(self._extract_field(data_bits, 0, 5))
    operand_bits <<= 1
    return operand_bits.uint
  
  def decode_imm9_s32_18_21_0_4(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=23)
    operand_bits.append(self._extract_field(data_bits, 18, 21))
    operand_bits.append(self._extract_field(data_bits, 0, 4))
    operand_bits.set(operand_bits[-9], range(0, 23))
    return operand_bits.int
  
  def decode_imm9_u32_18_21_0_4(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=23)
    operand_bits.append(self._extract_field(data_bits, 18, 21))
    operand_bits.append(self._extract_field(data_bits, 0, 4))
    return operand_bits.uint
  
  def decode_imm16_s32_16_31(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=16)
    operand_bits.append(self._extract_field(data_bits, 16, 31))
    operand_bits.set(operand_bits[-16], range(0, 16))
    return operand_bits.int
  
  def decode_imm16_u32_16_31(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=16)
    operand_bits.append(self._extract_field(data_bits, 16, 31))
    return operand_bits.uint
  
  def decode_imm16_u32_16_31_shift16(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=16)
    operand_bits.append(self._extract_field(data_bits, 16, 31))
    operand_bits <<= 16
    return operand_bits.uint
  
  def decode_imm32_s32_16_47(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray()
    operand_bits.append(self._extract_field(data_bits, 16, 47))
    operand_bits.set(operand_bits[-32], range(0, 0))
    return operand_bits.int
  
  def decode_list12_u16_21_31_0(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=4)
    operand_bits.append(self._extract_field(data_bits, 21, 31))
    operand_bits.append(self._extract_field(data_bits, 0, 0))
    return operand_bits.uint
  
  def decode_reg1_u8_0_4(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=3)
    operand_bits.append(self._extract_field(data_bits, 0, 4))
    return operand_bits.uint
  
  def decode_reg1_u8_16_20(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=3)
    operand_bits.append(self._extract_field(data_bits, 16, 20))
    return operand_bits.uint
  
  def decode_reg2_u8_0_4(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=3)
    operand_bits.append(self._extract_field(data_bits, 0, 4))
    return operand_bits.uint
  
  def decode_reg2_u8_11_15(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=3)
    operand_bits.append(self._extract_field(data_bits, 11, 15))
    return operand_bits.uint
  
  def decode_reg3_u8_27_31(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=3)
    operand_bits.append(self._extract_field(data_bits, 27, 31))
    return operand_bits.uint
  
  def decode_reg4_u8_17_20_shift1(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=4)
    operand_bits.append(self._extract_field(data_bits, 17, 20))
    operand_bits <<= 1
    return operand_bits.uint
  
  def decode_reg4_u8_17_20_23(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=3)
    operand_bits.append(self._extract_field(data_bits, 17, 20))
    operand_bits.append(self._extract_field(data_bits, 23, 23))
    return operand_bits.uint
  
  def decode_reg1_u8_1_4_shift1(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=4)
    operand_bits.append(self._extract_field(data_bits, 1, 4))
    operand_bits <<= 1
    return operand_bits.uint
  
  def decode_reg2_u8_12_15_shift1(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=4)
    operand_bits.append(self._extract_field(data_bits, 12, 15))
    operand_bits <<= 1
    return operand_bits.uint
  
  def decode_reg3_u8_28_31_shift1(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=4)
    operand_bits.append(self._extract_field(data_bits, 28, 31))
    operand_bits <<= 1
    return operand_bits.uint
  
  def decode_regID_u8_0_4(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=3)
    operand_bits.append(self._extract_field(data_bits, 0, 4))
    return operand_bits.uint
  
  def decode_regID_u8_11_15(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=3)
    operand_bits.append(self._extract_field(data_bits, 11, 15))
    return operand_bits.uint
  
  def decode_vector_u8_0_4(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=3)
    operand_bits.append(self._extract_field(data_bits, 0, 4))
    return operand_bits.uint
  
  def decode_vector_u8_27_29_0_4(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray()
    operand_bits.append(self._extract_field(data_bits, 27, 29))
    operand_bits.append(self._extract_field(data_bits, 0, 4))
    return operand_bits.uint
  
  def decode_vector_u8_11_14(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=4)
    operand_bits.append(self._extract_field(data_bits, 11, 14))
    return operand_bits.uint
  
  def decode_disp17_s32_4_17_31_shift1(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=16)
    operand_bits.append(self._extract_field(data_bits, 4, 4))
    operand_bits.append(self._extract_field(data_bits, 17, 31))
    operand_bits.set(operand_bits[-16], range(0, 16))
    operand_bits <<= 1
    return operand_bits.int
  
  def decode_msb_u32_28_31(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=28)
    operand_bits.append(self._extract_field(data_bits, 28, 31))
    return operand_bits.uint
  
  def decode_lsb_u32_27_17_19(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=28)
    operand_bits.append(self._extract_field(data_bits, 27, 27))
    operand_bits.append(self._extract_field(data_bits, 17, 19))
    return operand_bits.uint
  
  def decode_selID_u8_27_31(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=3)
    operand_bits.append(self._extract_field(data_bits, 27, 31))
    return operand_bits.uint
  
  def decode_rt_u8_27_31(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=3)
    operand_bits.append(self._extract_field(data_bits, 27, 31))
    return operand_bits.uint
  
  def decode_rh_u8_0_4(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=3)
    operand_bits.append(self._extract_field(data_bits, 0, 4))
    return operand_bits.uint
  
  def decode_cacheop_u8_11_12_27_31(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=1)
    operand_bits.append(self._extract_field(data_bits, 11, 12))
    operand_bits.append(self._extract_field(data_bits, 27, 31))
    return operand_bits.uint
  
  def decode_prefop_u8_27_31(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=3)
    operand_bits.append(self._extract_field(data_bits, 27, 31))
    return operand_bits.uint
  
  def decode_imm10_u16_27_31_0_4(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=6)
    operand_bits.append(self._extract_field(data_bits, 27, 31))
    operand_bits.append(self._extract_field(data_bits, 0, 4))
    return operand_bits.uint
  
  def decode_disp16_u32_17_31_shift1(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=17)
    operand_bits.append(self._extract_field(data_bits, 17, 31))
    operand_bits <<= 1
    return operand_bits.uint
  
  def decode_wreg1_u8_0_4(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=3)
    operand_bits.append(self._extract_field(data_bits, 0, 4))
    return operand_bits.uint
  
  def decode_wreg2_u8_11_15(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=3)
    operand_bits.append(self._extract_field(data_bits, 11, 15))
    return operand_bits.uint
  
  def decode_wreg2_u8_43_47(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=3)
    operand_bits.append(self._extract_field(data_bits, 43, 47))
    return operand_bits.uint
  
  def decode_wreg3_u8_27_31(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=3)
    operand_bits.append(self._extract_field(data_bits, 27, 31))
    return operand_bits.uint
  
  def decode_wreg4_u8_32_36(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=3)
    operand_bits.append(self._extract_field(data_bits, 32, 36))
    return operand_bits.uint
  
  def decode_fcond_u8_17_20(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=4)
    operand_bits.append(self._extract_field(data_bits, 17, 20))
    return operand_bits.uint
  
  def decode_disp16_s32_34_47_shift2(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=18)
    operand_bits.append(self._extract_field(data_bits, 34, 47))
    operand_bits.set(operand_bits[-14], range(0, 18))
    operand_bits <<= 2
    return operand_bits.int
  
  def decode_disp16_s32_35_47_shift3(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=19)
    operand_bits.append(self._extract_field(data_bits, 35, 47))
    operand_bits.set(operand_bits[-13], range(0, 19))
    operand_bits <<= 3
    return operand_bits.int
  
  def decode_disp16_s32_36_47_shift4(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=20)
    operand_bits.append(self._extract_field(data_bits, 36, 47))
    operand_bits.set(operand_bits[-12], range(0, 20))
    operand_bits <<= 4
    return operand_bits.int
  
  def decode_imm1_u32_21(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=31)
    operand_bits.append(self._extract_field(data_bits, 21, 21))
    return operand_bits.uint
  
  def decode_imm2_u32_0_1(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=30)
    operand_bits.append(self._extract_field(data_bits, 0, 1))
    return operand_bits.uint
  
  def decode_imm2_u32_21_22(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=30)
    operand_bits.append(self._extract_field(data_bits, 21, 22))
    return operand_bits.uint
  
  def decode_imm3_u32_27_29(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=29)
    operand_bits.append(self._extract_field(data_bits, 27, 29))
    return operand_bits.uint
  
  def decode_imm4_u32_21_24(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=28)
    operand_bits.append(self._extract_field(data_bits, 21, 24))
    return operand_bits.uint
  
  def decode_imm12_u32_21_32_42(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=20)
    operand_bits.append(self._extract_field(data_bits, 21, 21))
    operand_bits.append(self._extract_field(data_bits, 32, 42))
    return operand_bits.uint
  
  def decode_et_u8_27_31(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=3)
    operand_bits.append(self._extract_field(data_bits, 27, 31))
    return operand_bits.uint
  
  def decode_eh_u8_11_14(self, data):
    data_bits = bitstring.Bits(uint=data, length=64)
    operand_bits = bitstring.BitArray(int=0, length=4)
    operand_bits.append(self._extract_field(data_bits, 11, 14))
    return operand_bits.uint
  
  # Value rendering operations
  def render_bit_u8_11_13(self, operand_values):
    data = operand_values['bit_u8_11_13']
    return str(data)
  
  def render_bits_ff_u8_19_20(self, operand_values):
    data = operand_values['bits_ff_u8_19_20']
    return str(data)
  
  def render_cccc_u8_0_3(self, operand_values):
    data = operand_values['cccc_u8_0_3']
    return str(data)
  
  def render_cccc_u8_17_20(self, operand_values):
    data = operand_values['cccc_u8_17_20']
    return str(data)
  
  def render_fcbit_u8_17_19(self, operand_values):
    data = operand_values['fcbit_u8_17_19']
    return str(data)
  
  def render_fcond_u8_27_30(self, operand_values):
    data = operand_values['fcond_u8_27_30']
    return str(data)
  
  def render_disp4_u32_0_3(self, operand_values):
    data = operand_values['disp4_u32_0_3']
    return str(data)
  
  def render_disp5_u32_0_3_shift1(self, operand_values):
    data = operand_values['disp5_u32_0_3_shift1']
    return str(data)
  
  def render_disp7_u32_0_6(self, operand_values):
    data = operand_values['disp7_u32_0_6']
    return str(data)
  
  def render_disp8_u32_0_6_shift1(self, operand_values):
    data = operand_values['disp8_u32_0_6_shift1']
    return str(data)
  
  def render_disp8_u32_1_6_shift2(self, operand_values):
    data = operand_values['disp8_u32_1_6_shift2']
    return str(data)
  
  def render_disp9_s32_11_15_4_6_shift1(self, operand_values):
    data = operand_values['disp9_s32_11_15_4_6_shift1']
    return str(data)
  
  def render_disp16_s32_16_31(self, operand_values):
    data = operand_values['disp16_s32_16_31']
    return str(data)
  
  def render_disp16_s32_17_31_5(self, operand_values):
    data = operand_values['disp16_s32_17_31_5']
    return str(data)
  
  def render_disp16_s32_17_31_shift1(self, operand_values):
    data = operand_values['disp16_s32_17_31_shift1']
    return str(data)
  
  def render_disp22_s32_0_5_17_31_shift1(self, operand_values):
    data = operand_values['disp22_s32_0_5_17_31_shift1']
    return str(data)
  
  def render_disp23_s32_32_47_20_26(self, operand_values):
    data = operand_values['disp23_s32_32_47_20_26']
    return str(data)
  
  def render_disp23_s32_32_47_21_26_shift1(self, operand_values):
    data = operand_values['disp23_s32_32_47_21_26_shift1']
    return str(data)
  
  def render_disp32_s32_17_47_shift1(self, operand_values):
    data = operand_values['disp32_s32_17_47_shift1']
    return str(data)
  
  def render_imm4_u32_0_3(self, operand_values):
    data = operand_values['imm4_u32_0_3']
    return str(data)
  
  def render_imm5_s32_0_4(self, operand_values):
    data = operand_values['imm5_s32_0_4']
    return str(data)
  
  def render_imm5_u32_11_15(self, operand_values):
    data = operand_values['imm5_u32_11_15']
    return str(data)
  
  def render_imm5_u32_0_4(self, operand_values):
    data = operand_values['imm5_u32_0_4']
    return str(data)
  
  def render_imm5_u32_1_5_shift2(self, operand_values):
    data = operand_values['imm5_u32_1_5_shift2']
    return str(data)
  
  def render_imm6_u32_0_5_shift1(self, operand_values):
    data = operand_values['imm6_u32_0_5_shift1']
    return str(data)
  
  def render_imm9_s32_18_21_0_4(self, operand_values):
    data = operand_values['imm9_s32_18_21_0_4']
    return str(data)
  
  def render_imm9_u32_18_21_0_4(self, operand_values):
    data = operand_values['imm9_u32_18_21_0_4']
    return str(data)
  
  def render_imm16_s32_16_31(self, operand_values):
    data = operand_values['imm16_s32_16_31']
    return str(data)
  
  def render_imm16_u32_16_31(self, operand_values):
    data = operand_values['imm16_u32_16_31']
    return str(data)
  
  def render_imm16_u32_16_31_shift16(self, operand_values):
    data = operand_values['imm16_u32_16_31_shift16']
    return str(data)
  
  def render_imm32_s32_16_47(self, operand_values):
    data = operand_values['imm32_s32_16_47']
    return str(data)
  
  def render_list12_u16_21_31_0(self, operand_values):
    data = operand_values['list12_u16_21_31_0']
    return str(data)
  
  def render_reg1_u8_0_4(self, operand_values):
    data = operand_values['reg1_u8_0_4']
    return GR_group_index_name_map[data]
  
  def render_reg1_u8_16_20(self, operand_values):
    data = operand_values['reg1_u8_16_20']
    return GR_group_index_name_map[data]
  
  def render_reg2_u8_0_4(self, operand_values):
    data = operand_values['reg2_u8_0_4']
    return GR_group_index_name_map[data]
  
  def render_reg2_u8_11_15(self, operand_values):
    data = operand_values['reg2_u8_11_15']
    return GR_group_index_name_map[data]
  
  def render_reg3_u8_27_31(self, operand_values):
    data = operand_values['reg3_u8_27_31']
    return GR_group_index_name_map[data]
  
  def render_reg4_u8_17_20_shift1(self, operand_values):
    data = operand_values['reg4_u8_17_20_shift1']
    return GR_group_index_name_map[data]
  
  def render_reg4_u8_17_20_23(self, operand_values):
    data = operand_values['reg4_u8_17_20_23']
    return GR_group_index_name_map[data]
  
  def render_reg1_u8_1_4_shift1(self, operand_values):
    data = operand_values['reg1_u8_1_4_shift1']
    return GR_group_index_name_map[data]
  
  def render_reg2_u8_12_15_shift1(self, operand_values):
    data = operand_values['reg2_u8_12_15_shift1']
    return GR_group_index_name_map[data]
  
  def render_reg3_u8_28_31_shift1(self, operand_values):
    data = operand_values['reg3_u8_28_31_shift1']
    return GR_group_index_name_map[data]
  
  def render_regID_u8_0_4(self, operand_values):
    data = operand_values['regID_u8_0_4']
    return SR_SELID_0_group_index_name_map[data]
  
  def render_regID_u8_11_15(self, operand_values):
    data = operand_values['regID_u8_11_15']
    return SR_SELID_0_group_index_name_map[data]
  
  def render_vector_u8_0_4(self, operand_values):
    data = operand_values['vector_u8_0_4']
    return str(data)
  
  def render_vector_u8_27_29_0_4(self, operand_values):
    data = operand_values['vector_u8_27_29_0_4']
    return str(data)
  
  def render_vector_u8_11_14(self, operand_values):
    data = operand_values['vector_u8_11_14']
    return str(data)
  
  def render_disp17_s32_4_17_31_shift1(self, operand_values):
    data = operand_values['disp17_s32_4_17_31_shift1']
    return str(data)
  
  def render_msb_u32_28_31(self, operand_values):
    data = operand_values['msb_u32_28_31']
    return str(data)
  
  def render_lsb_u32_27_17_19(self, operand_values):
    data = operand_values['lsb_u32_27_17_19']
    return str(data)
  
  def render_selID_u8_27_31(self, operand_values):
    data = operand_values['selID_u8_27_31']
    return str(data)
  
  def render_rt_u8_27_31(self, operand_values):
    data = operand_values['rt_u8_27_31']
    return GR_group_index_name_map[data]
  
  def render_rh_u8_0_4(self, operand_values):
    data = operand_values['rh_u8_0_4']
    return GR_group_index_name_map[data]
  
  def render_cacheop_u8_11_12_27_31(self, operand_values):
    data = operand_values['cacheop_u8_11_12_27_31']
    return str(data)
  
  def render_prefop_u8_27_31(self, operand_values):
    data = operand_values['prefop_u8_27_31']
    return str(data)
  
  def render_imm10_u16_27_31_0_4(self, operand_values):
    data = operand_values['imm10_u16_27_31_0_4']
    return str(data)
  
  def render_disp16_u32_17_31_shift1(self, operand_values):
    data = operand_values['disp16_u32_17_31_shift1']
    return str(data)
  
  def render_wreg1_u8_0_4(self, operand_values):
    data = operand_values['wreg1_u8_0_4']
    return WR_group_index_name_map[data]
  
  def render_wreg2_u8_11_15(self, operand_values):
    data = operand_values['wreg2_u8_11_15']
    return WR_group_index_name_map[data]
  
  def render_wreg2_u8_43_47(self, operand_values):
    data = operand_values['wreg2_u8_43_47']
    return WR_group_index_name_map[data]
  
  def render_wreg3_u8_27_31(self, operand_values):
    data = operand_values['wreg3_u8_27_31']
    return WR_group_index_name_map[data]
  
  def render_wreg4_u8_32_36(self, operand_values):
    data = operand_values['wreg4_u8_32_36']
    return WR_group_index_name_map[data]
  
  def render_fcond_u8_17_20(self, operand_values):
    data = operand_values['fcond_u8_17_20']
    return str(data)
  
  def render_disp16_s32_34_47_shift2(self, operand_values):
    data = operand_values['disp16_s32_34_47_shift2']
    return str(data)
  
  def render_disp16_s32_35_47_shift3(self, operand_values):
    data = operand_values['disp16_s32_35_47_shift3']
    return str(data)
  
  def render_disp16_s32_36_47_shift4(self, operand_values):
    data = operand_values['disp16_s32_36_47_shift4']
    return str(data)
  
  def render_imm1_u32_21(self, operand_values):
    data = operand_values['imm1_u32_21']
    return str(data)
  
  def render_imm2_u32_0_1(self, operand_values):
    data = operand_values['imm2_u32_0_1']
    return str(data)
  
  def render_imm2_u32_21_22(self, operand_values):
    data = operand_values['imm2_u32_21_22']
    return str(data)
  
  def render_imm3_u32_27_29(self, operand_values):
    data = operand_values['imm3_u32_27_29']
    return str(data)
  
  def render_imm4_u32_21_24(self, operand_values):
    data = operand_values['imm4_u32_21_24']
    return str(data)
  
  def render_imm12_u32_21_32_42(self, operand_values):
    data = operand_values['imm12_u32_21_32_42']
    return str(data)
  
  def render_et_u8_27_31(self, operand_values):
    data = operand_values['et_u8_27_31']
    return SR_SELID_5_group_index_name_map[data]
  
  def render_eh_u8_11_14(self, operand_values):
    data = operand_values['eh_u8_11_14']
    return SR_SELID_5_group_index_name_map[data]
  

class rh850e3_disassembler(disassembler):
  def __init__(self):
    super(rh850e3_disassembler, self).__init__("rh850e3")

    self.isa = disassembler.instruction_set()

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
    
#------------------------------------------------------------------------------
# End of file
#------------------------------------------------------------------------------
