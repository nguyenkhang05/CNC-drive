# -*- coding: utf-8 -*-
# @file elfloader.py
# @date Created on: 2016
# @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
# @brief elf-binary loader
# @par Copyright: 2016 Renesas Electronics Corp.
from sys import path

path[0:0] = ['.', '..']

try:
    from elftools.common.exceptions import ELFError
    from elftools.elf.elffile import ELFFile
    from elftools.elf.sections import SymbolTableSection
except ImportError:
    pass
else:
    from os.path import isfile
    from struct import unpack


# elf object manager
class elfloader(object):
    elfobjects = {}
    p_cur_file = ''

    def hasFile(self, filename):
        if isfile(filename):
            return True
        else:
            print("<Error: Can not open file='%s'>" % filename)
            return False

    def checkFormat(self, filename, verbose=False):
        result = True
        with open(filename) as stream:
            stream.seek(0)
            try:
                elffile = ELFFile(stream)
            except ELFError:
                if verbose:
                    print("<Error: Invalid Elf header>")
                return False
            if elffile.header['e_ident']['EI_CLASS'] != 'ELFCLASS32':
                if verbose:
                    print("<Error: Invalid EI_CLASS %s>" %
                          repr(elffile.header['e_ident']['EI_CLASS']))
                result = False
            if elffile.header['e_ident']['EI_DATA'] != 'ELFDATA2LSB':
                if verbose:
                    print("<Error: Invalid EI_DATA %s>" %
                          repr(elffile.header['e_ident']['EI_DATA']))
                result = False
            if elffile.header['e_type'] != 'ET_EXEC':
                if verbose:
                    print("<Error: ELF isn't Executable type. %s>" %
                          repr(elffile.header['e_type']))
                result = False
            if elffile.header['e_machine'] != 'EM_V800' and elffile.header['e_machine'] != 'EM_V850':
                if verbose:
                    print("<Error: ELF isn't for v800/v850. %s>" %
                          repr(elffile.header['e_machine']))
                result = False
        return result

    def getSymbols(self, type='', filename=''):
        if filename == '':
            filename = self.p_cur_file
        if filename not in self.elfobjects:
            return []
        elf_object = self.elfobjects[filename]
        section = elf_object.get_section_by_name('.symtab')
        if not section:
            print("<Error: No symbol table found. Perhaps this ELF has been stripped?>")
            return []
        if isinstance(section, SymbolTableSection):

            cl = [(str(section.get_symbol(_).name), section.get_symbol(_).entry['st_value'], section.get_symbol(_).entry['st_size'], section.get_symbol(_).entry['st_info']['type']) for _ in range(section.num_symbols()) if section.get_symbol(_).entry['st_info']['bind'] != 'STB_LOCAL' and section.get_symbol(_).entry['st_other']['visibility'] != 'STV_HIDDEN']

            if type == 'FUNC':
                return [_ for _ in cl if 'STT_FUNC' in _]
            elif type == 'OBJECT':
                return [_ for _ in cl if 'STT_OBJECT' in _]
            else:
                return cl
        else:
            return []

    def load(self, filename):
        # if simso.IsEnabledPrintPy():
        #     print("Filename: {}".format(filename))
        if not self.hasFile(filename):
            return False
        if not self.checkFormat(filename, True):
            return False
        self.p_cur_file = filename
        stream = open(filename)
        elf_object = ELFFile(stream)
        self.elfobjects[self.p_cur_file] = elf_object
        # load binary
        if elf_object.num_segments() == 0:
            print("<Error: There are no program headers in this file.%s>" % filename)
            return False
        for segment in elf_object.iter_segments():
            if segment['p_type'] != 'PT_LOAD':
                next
            # In GHS object, p_paddr isn't supported. Then, p_paddr is 0.
            if segment['p_paddr'] != 0:
                write_addr = segment['p_paddr']
            else:
                write_addr = segment['p_vaddr']
            read_addr = segment['p_offset']
            size = segment['p_filesz']
            # print("LOAD: %08x-%08x => %08x-%08x" % (read_addr, read_addr + size - 1, write_addr, write_addr + size - 1))
            # Write data to CForest memory

            def WriteMem(index):
                segment.stream.seek(read_addr + index)
                bin_data = unpack("<B", segment.stream.read(1))[0]
                # if __name__ == '__main__':
                #     pass
                #     # print("%.8x:%.2x -> [%.8x]" % (read_addr + index, bin_data, write_addr + index))
                # else:
                #     pe, tc = CFDEF_NOSEL_CONTEXT
                #     simso.MemEditPy(write_addr + index, bin_data, 1, pe, tc)
                pe, tc = CFDEF_NOSEL_CONTEXT
                simso.MemEditPy(write_addr + index, bin_data, 1, pe, tc)
            res = [WriteMem(i) for i in range(size)]
            r = [x for x in res if x is not None]
            if len(r):
                result = False
            else:
                result = True
        return result

    def getCurFile(self):
        return self.p_cur_file

    def setCurFile(self, filename):
        if not self.hasFile(filename):
            return False
        self.p_cur_file = filename
        return filename


def getSymbolAddress(s, type=''):
    if isinstance(s, (int, long)):
        return 0, s
    elif isinstance(s, str):
        syms = el.getSymbols(type)
        if syms == []:
            return -1, s
        r = [i for i in syms if s in i]
        if r != []:
            return 0, (r[0])[1]
        else:
            return -1, s
    else:
        return -1, s

def getSymbolSize(s, type=''):
    if isinstance(s, str):
        syms = el.getSymbols(type)
        if syms == []:
            return -1, s
        r = [i for i in syms if s in i]
        if r != []:
            return 0, (r[0])[2]
        else:
            return -1, s
    else:
        return -1, s

# overwrite commands


def ba(start_addr, *args):
    result, start_addr = getSymbolAddress(start_addr)
    if (result == -1):
        return result
    return sim.ba(start_addr, *args)


def bac(start_addr, *args):
    result, start_addr = getSymbolAddress(start_addr)
    if (result == -1):
        return result
    return sim.bac(start_addr, *args)


def bp(addr, *contextid):
    result, addr = getSymbolAddress(addr)
    if (result == -1):
        return result
    return sim.bp(addr, *contextid)


def md(start_addr, *args):
    result, start_addr = getSymbolAddress(start_addr)
    if (result == -1):
        return result
    return sim.md(start_addr, *args)


def mr(addr, *args):
    result, addr = getSymbolAddress(addr)
    if (result == -1):
        return result
    return sim.mr(addr, *args)


def me(addr, data, *args):
    result, addr = getSymbolAddress(addr)
    if (result == -1):
        return result
    return sim.me(addr, data, *args)


def ms(start_addr, end_addr, *args):
    result, start_addr = getSymbolAddress(start_addr)
    if (result == -1):
        return result
    return sim.ms(start_addr, end_addr, *args)


def mc(start_addr, end_addr, *args):
    result, start_addr = getSymbolAddress(start_addr)
    if (result == -1):
        return result
    return sim.mc(start_addr, end_addr, *args)


def target_file(file=''):
    if file == '':
        return el.getCurFile()
    else:
        return el.setCurFile(file)


# if __name__ == '__main__':
#     el = elfloader()
#     for filename in sys.argv[1:]:
#         el.load(filename)
#     # for filename in sys.argv[1:]:
#     #     el.setCurFile(filename)
#     #     print(el.getSymbols())
#     #     print("---")
#     #     print(el.getSymbols(type="FUNC"))
#     #     print("---")
#     #     print(el.getSymbols(type="OBJECT"))
#     print(el.getCurFile())
#     print(el.setCurFile('/tmp/cforest_extensions/exe_loader/test/test_sample_v850'))
#     print(getSymbolAddress('_main'))  # has
#     print(getSymbolAddress('_main2')) # not
