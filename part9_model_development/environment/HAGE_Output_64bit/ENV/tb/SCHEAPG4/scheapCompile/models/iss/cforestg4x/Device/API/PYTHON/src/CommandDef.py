# -*- coding: utf-8 -*-
import re
import simso
from struct import unpack


class cforest(object):  # コマンド定義

    def __init__(self):
        self.__dict__['quit_flag'] = 0

    def __del__(self):
        if self.__dict__['quit_flag'] == 0:
            simso.QuitPy()

    def setreg(self, name, data, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        data_u64 = (data >> 64) & 0xFFFFFFFFFFFFFFFF
        data_l64 = data & 0xFFFFFFFFFFFFFFFF
        result = simso.RegWriteDebugPy(
            name.lower(), data_u64, data_l64, pe, tc)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("setreg (" + str(name) + "," + str(data) + ") = " + str(result))
        return result

    def getreg(self, name, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.RegReadDebugPy(name.lower(), pe, tc)
        if simso.IsEnabledPrintPy() == 1:
            print("getreg(" + str(name) + ") = " + str(result))
        return result

    def showreg(self):
        result = simso.ShowRegPy()
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("showreg() = " + str(result))
        return result

    def set_event(self, addr, eventname, *params):
        self.gen_pe()
        num_param = len(params)
        context_str = ''
        priority = 0
        channel = 0
        repeat = 0
        gpid = 0
        param_pos_start = 0
        # set_event (address, "feint", channel, context, repeat)
        # set_event (address, "gmfeint", channel, gpid, context, repeat)
        # set_event (address, "bgfeint", channel, gpid, context, repeat)
        # set_event (address, "fenmi", context, repeat)
        # set_event (address, "eiint", channel, priority, context, repeat)
        # set_event (address, "eitbl", channel, priority, context, repeat)
        # set_event (address, "gmeiint", channel, priority, gpid, context, repeat)
        # set_event (address, "gmeitbl", channel, priority, gpid, context, repeat)
        # set_event (address, "bgeiint", channel, priority, gpid, context, repeat)
        # set_event (address, "dbint", channel, repeat)
        # set_event (address, "syserr", cause_code, repeat)
        # set_event (address, "rmint", repeat)
        # set_event (address, "stopreq", context)
        # set_event (address, "pereset", context)
        if eventname.lower() == 'feint' or eventname.lower() == 'dbint' or eventname.lower() == 'syserr':
            param_pos_start = 1
        elif eventname.lower() == 'fenmi' or eventname.lower() == 'dbnmi' or eventname.lower() == 'rmint' or eventname.lower() == 'stopreq' or eventname.lower() == 'pereset':
            param_pos_start = 0
        elif eventname.lower() == 'eiint' or eventname.lower() == 'eitbl' or eventname.lower() == 'gmfeint' or eventname.lower() == 'bgfeint':
            param_pos_start = 2
        elif eventname.lower() == 'gmeiint' or eventname.lower() == 'gmeitbl' or eventname.lower() == 'bgeiint':
            param_pos_start = 3
        else:
            print("Invalid event name1")
            return False
        if num_param > param_pos_start:
            used_param = 0
            if sim.is_context_id(params[param_pos_start]):
                context_str = params[param_pos_start]
                used_param += 1
                if num_param > param_pos_start + 1:
                    repeat = params[param_pos_start + 1]
                    used_param += 1
            else:
                repeat = params[param_pos_start]
                used_param += 1
            num_param -= used_param
        if eventname.lower() == 'feint':
            if num_param != 1:
                print("params is invalid for FEINT")
                return False
            channel = params[0]
        elif eventname.lower() == 'gmfeint':
            if num_param != 2:
                print("params is invalid for GMFEINT")
                return False
            channel = params[0]
            gpid = params[1]
        elif eventname.lower() == 'bgfeint':
            if num_param != 2:
                print("params is invalid for BGFEINT")
                return False
            channel = params[0]
            gpid = params[1]
        elif eventname.lower() == 'fenmi':
            if num_param != 0:
                print("params is invalid for FENMI")
                return False
        elif eventname.lower() == 'eiint':
            if num_param != 2:
                print("params is invalid for EIINT")
                return False
            channel = params[0]
            priority = params[1]
        elif eventname.lower() == 'eitbl':
            if num_param != 2:
                print("params is invalid for EITBL")
                return False
            channel = params[0]
            priority = params[1]
        elif eventname.lower() == 'gmeiint':
            if num_param != 3:
                print("params is invalid for GMEIINT")
                return False
            channel = params[0]
            priority = params[1]
            gpid = params[2]
        elif eventname.lower() == 'gmeitbl':
            if num_param != 3:
                print("params is invalid for GMEITBL")
                return False
            channel = params[0]
            priority = params[1]
            gpid = params[2]
        elif eventname.lower() == 'bgeiint':
            if num_param != 3:
                print("params is invalid for BGEIINT")
                return False
            channel = params[0]
            priority = params[1]
            gpid = params[2]
        elif eventname.lower() == 'dbint':
            if num_param != 1:
                print("params is invalid for dbint")
                return False
            channel = params[0]
        elif eventname.lower() == 'dbnmi':
            if num_param != 0:
                print("params is invalid for dbnmi")
                return False
        elif eventname.lower() == 'syserr':
            if num_param != 1:
                print("params is invalid for SYSERR")
                return False
            channel = params[0]
        elif eventname.lower() == 'rmint':
            if num_param != 0:
                print("params is invalid for RMINT")
                return False
        elif eventname.lower() == 'stopreq':  #Support stopreq signal for FROG I/F operation.
            if num_param != 0:
                print("params is invalid for STOPREQ")
                return False
        elif eventname.lower() == 'pereset':  #Support individual PE reset in G4MH2.1.
            if num_param != 0:
                print("params is invalid for PE Reset")
                return False
        else:
            print("Invalid event name2")
            return False
        pe, tc = sim.GetContextArg(context_str)
        result = simso.SetEventPy(
            addr, eventname.lower(), channel, priority, gpid, repeat, pe, tc)
        if simso.IsEnabledPrintPy() == 1:
            print("set_event() = " + str(result))
        return result

    def set_clear_event(self, addr, eventname, *params):
        self.gen_pe()
        num_param = len(params)
        context_str = ''
        repeat = 0
        if num_param > 0:
            if sim.is_context_id(params[0]):
                context_str = params[0]
                if num_param > 1:
                    repeat = params[1]
            else:
                repeat = params[0]
        pe, tc = sim.GetContextArg(context_str)
        result = simso.SetClearEventPy(
            addr, eventname.lower(), repeat, pe, tc)
        if simso.IsEnabledPrintPy() == 1:
            print("set_clear_event() = " + str(result))
        return result

    def pe(self, data):
        if isinstance(data, int):
            self.m_pe = data
        else:
            pe_str = data.lower()
            if pe_str[0] == 'p':
                data = pe_str[1:]
            self.m_pe = int(data)

    def vc(self, data):
        print("Warning : Depreceted command \"vc\" is used")

    def tc(self, data):
        if isinstance(data, int):
            self.m_tc = data
        else:
            tc_str = data.lower()
            if tc_str[0] == 't':
                data = tc_str[1:]
            if tc_str == 'nc':
                self.m_tc = CFDEF_DEF_NC
            else:
                self.m_tc = int(data)

    def gdb_connect(self, port):
        self.gen_pe()
        result = simso.ConnectGDBPy(port)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("gdb_connect(" + str(port) + ") = " + str(result))
        return result

    def uart_connect(self, port, channel):
        self.gen_pe()
        result = simso.ConnectUARTPy(port, channel)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("uart_connect(" + str(port) + "," +
                  str(channel) + ") = " + str(result))
        return result

    def ba(self, start_addr, *args):
        self.gen_pe()
        end_addr = start_addr
        access_attr = 'rw'
        contextid_str = ''
        for arg in args:
            if isinstance(arg, int):
                end_addr = arg
            elif isinstance(arg, str):
                lower_str = arg.lower()
                if lower_str == 'r' or lower_str == 'w' or lower_str == 'rw':
                    access_attr = lower_str
                else:
                    contextid_str = arg
            else:
                print("Unknown argument in ba command :" + str(arg))
                return
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.AddBreakAccessPy(start_addr, end_addr, access_attr.lower(),
                                        pe, tc)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("ba(" + str(hex(start_addr)) + ") = " + str(result))
        return result

    def bad(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.DisplayBreakAccessPy(pe, tc)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("bad() = " + str(result))
        return result

    def bac(self, start_addr, *args):
        end_addr = start_addr
        access_attr = 'rw'
        contextid_str = ''
        for arg in args:
            if isinstance(arg, int):
                end_addr = arg
            elif isinstance(arg, str):
                lower_str = arg.lower()
                if lower_str == 'r' or lower_str == 'w' or lower_str == 'rw':
                    access_attr = lower_str
                else:
                    contextid_str = arg
            else:
                print("Unknown argument in ba command :" + str(arg))
                return
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.ClearBreakAccessPy(start_addr, end_addr, access_attr.lower(),
                                          pe, tc)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("bac(" + str(hex(start_addr)) + ") = " + str(result))
        return result

    def bp(self, addr, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.AddBreakPcPy(addr, pe, tc)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("bp(" + str(hex(addr)) + ") = " + str(result))
        return result

    def bpd(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.DisplayBreakPcPy(pe, tc)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("bpd() = " + str(result))
        return result

    def bpc(self, addr, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.ClearBreakPcPy(addr, pe, tc)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("bpc(" + str(hex(addr)) + ") = " + str(result))
        return result

    def run(self, count=CFDEF_DEF_STEP):
        self.gen_pe()
        result = simso.ContExecutePy(count)
        if simso.IsEnabledPrintPy() == 1:
            print("run() = " + str(result))
        return result

    def ml(self, name, offset=CFDEF_LOAD_OFFSET):
        self.gen_pe()
        result = 0
        cmdopt_filename = sim.GetLoadNameCmdopt()
        if cmdopt_filename != '':
            name = cmdopt_filename
            print("<Warning: Designated HEX file is overwritten by --hex>")

        try:
            if el.hasFile(name):
                if el.checkFormat(name):
                    e = el.load(name)
                    if not e:
                        result = 1
                    else:
                        result = 0
                else:
                    result = simso.LoadSrecPy(name, offset)
            else:
                result = 1
        except NameError:
            is_elf = sim.LoadElf(name)
            if is_elf == 'NO_FILE':
                result = 1
            else:
                if is_elf != 'NOT_ELF':
                    if is_elf == 'NOT_SUPPORTED_ELF':
                        result = 1
                    else:
                        result = 0
                else:
                    result = simso.LoadSrecPy(name, offset)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("ml(" + str(name) + ") = " + str(result))
        return result

    def max(self, count):
        self.gen_pe()
        result = simso.SetBreakMaxPy(count)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("max(" + str(count) + ") = " + str(result))
        return result

    def getmax(self):
        self.gen_pe()
        result = simso.GetBreakMaxPy()
        if simso.IsEnabledPrintPy() == 1:
            print("getmax() = " + str(result))
        return result

    def max_same_pc(self, count):
        self.gen_pe()
        result = simso.SetBreakSamePcMaxPy(count)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("max_same_pc(" + str(count) + ") = " + str(result))
        return result

    def getmax_same_pc(self):
        self.gen_pe()
        result = simso.GetBreakSamePcMaxPy()
        if simso.IsEnabledPrintPy() == 1:
            print("getmax_same_pc() = " + str(result))
        return result

    def bt(self, count):
        self.gen_pe()
        result = simso.SetBreakSystimePy(count)
        print("bt(" + str(count) + ") = " + str(result))
        return result

    def btd(self):
        self.gen_pe()
        result = simso.GetBreakSystimePy()
        print("btd() = " + str(result))
        return result

    def mi(self):
        self.gen_pe()
        result = simso.MemResourceDisplayPy()
        if simso.IsEnabledPrintPy() == 1 or result == 0:
            print("mi() = " + str(result))
        return result

    def md(self, start_addr, *args):
        self.gen_pe()
        end_addr = start_addr + 0xFF
        fname = ''
        contextid_str = ''
        for arg in args:
            if isinstance(arg, int):
                end_addr = arg
            elif isinstance(arg, str):
                if arg == args[len(args) - 1] and sim.is_context_id(arg):
                    contextid_str = arg
                else:
                    if fname == '':
                        fname = arg
                    else:
                        print("Unknown argument in md command :" + str(arg))
                        return
            else:
                print("Unknown argument in md command :" + str(arg))
                return
        if len(contextid_str) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid_str)
        result = simso.MemDisplayPy(start_addr, end_addr, fname, pe, tc)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("md(" + str(hex(start_addr)) + ") = " + str(result))
        return result

    def is_context_id(self, checked_str):
        if not isinstance(checked_str, str):
            return False
        matched_str = re.match('P[0-9]+([T][0-9]+)?(NC)?',
                               checked_str, re.IGNORECASE)
        if matched_str is None:
            return False
        elif matched_str.end() != len(checked_str):
            return False
        else:
            return True

    def mr(self, addr, *args):
        self.gen_pe()
        read_size = 1
        contextid_str = ''
        for arg in args:
            if isinstance(arg, int):
                read_size = arg
            elif isinstance(arg, str):
                contextid_str = arg
            else:
                print("Unknown argument in mr command :" + str(arg))
                return
        if len(contextid_str) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid_str)
        result = simso.MemReadPy(addr, read_size, pe, tc)
        if simso.IsEnabledPrintPy() == 1:
            print("mr(" + str(hex(addr)) + ") = " + str(result))
        return result

    def me(self, addr, data, *args):
        self.gen_pe()
        size = CFDEF_ME_SIZE
        contextid_str = ''
        for arg in args:
            if isinstance(arg, int):
                size = arg
            elif isinstance(arg, str):
                contextid_str = arg
            else:
                print("Unknown argument in me command :" + str(arg))
                return
        if len(contextid_str) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid_str)
        result = simso.MemEditPy(addr, data, size, pe, tc)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("me(" + str(hex(addr)) + "," + str(data) + ") = " + str(result))
        return result

    def ms(self, start_addr, end_addr, *args):
        self.gen_pe()
        read_lat = 0
        write_lat = 0
        bind_pe = CFDEF_GLOBAL
        outstand = 2
        repeat_rate = 1
        bank_num = 1
        bank_size = 0
        access_attr = 'RWX'
        size = len(args)
        latency_start = 0
        latency_end = size
        err_flag = False
        match_str = False
        for i in range(size):
            if isinstance(args[i], str):
                if match_str or i != 0:
                    err_flag = True
                    break
                access_attr = args[i]
                latency_start = i + 1
                latency_end = size
                match_str = True
        if latency_start < size and ~err_flag:
            latency_param = args[latency_start:latency_end]
            if len(latency_param) == 2:
                read_lat = latency_param[0]
                write_lat = latency_param[1]
            elif len(latency_param) == 3:
                read_lat = latency_param[0]
                write_lat = latency_param[1]
                bind_pe = latency_param[2]
            elif len(latency_param) == 4:
                read_lat = latency_param[0]
                write_lat = latency_param[1]
                bind_pe = latency_param[2]
                outstand = latency_param[3]
            elif len(latency_param) == 5:
                read_lat = latency_param[0]
                write_lat = latency_param[1]
                bind_pe = latency_param[2]
                outstand = latency_param[3]
                repeat_rate = latency_param[4]
            elif len(latency_param) == 6:
                read_lat = latency_param[0]
                write_lat = latency_param[1]
                bind_pe = latency_param[2]
                outstand = latency_param[3]
                repeat_rate = latency_param[4]
                bank_num = latency_param[5]
            elif len(latency_param) == 7:
                read_lat = latency_param[0]
                write_lat = latency_param[1]
                bind_pe = latency_param[2]
                outstand = latency_param[3]
                repeat_rate = latency_param[4]
                bank_num = latency_param[5]
                bank_size = latency_param[6]
            else:
                err_flag = True
        if err_flag:
            print("Unknown argument in ms command :" "ms (" +
                  str(start_addr) + ", " + str(end_addr) + ", " + str(args) + ")")
            return
        result = simso.MemSetPy(start_addr, end_addr, access_attr.lower(),
                                read_lat, write_lat, bind_pe, outstand, repeat_rate, bank_num, bank_size)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("ms(" + str(hex(start_addr)) + "," +
                  str(hex(end_addr)) + ") = " + str(result))
        return result

    def ms_pe(self, start_addr, end_addr, access_attr, pe_fetch_latency, pe_read_latency, pe_write_latency, cl_fetch_latency, cl_read_latency, cl_write_latency, exocl_fetch_latency, exocl_read_latency, exocl_write_latency, bind_pe, outstand, repeat, *param):
        self.gen_pe()
        if len(param) == 0:
            bank_num = 1
            bank_size = 0
        elif len(param) == 1:
            bank_num = param[0]
            bank_size = 0
        elif len(param) == 2:
            bank_num = param[0]
            bank_size = param[1]
        else:
            print("Unknown argument in ms_pe command")
            return False
        result = simso.MemSetPePy(start_addr, end_addr, access_attr.lower(), pe_fetch_latency, pe_read_latency, pe_write_latency, cl_fetch_latency,
                                  cl_read_latency, cl_write_latency, exocl_fetch_latency, exocl_read_latency, exocl_write_latency, bind_pe, outstand, repeat, bank_num, bank_size)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("ms_pe(" + str(hex(start_addr)) + "," +
                  str(hex(end_addr)) + ") = " + str(result))
        return result

    def ms_cl(self, start_addr, end_addr, access_attr, cl_fetch_latency, cl_read_latency, cl_write_latency, exocl_fetch_latency, exocl_read_latency, exocl_write_latency, bind_cl, outstand, repeat, *param):
        self.gen_pe()
        if len(param) == 0:
            bank_num = 1
            bank_size = 0
        elif len(param) == 1:
            bank_num = param[0]
            bank_size = 0
        elif len(param) == 2:
            bank_num = param[0]
            bank_size = param[1]
        else:
            print("Unknown argument in ms_cl command")
            return False
        result = simso.MemSetClPy(start_addr, end_addr, access_attr.lower(), cl_fetch_latency, cl_read_latency, cl_write_latency,
                                  exocl_fetch_latency, exocl_read_latency, exocl_write_latency, bind_cl, outstand, repeat, bank_num, bank_size)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("ms_cl(" + str(hex(start_addr)) + "," +
                  str(hex(end_addr)) + ") = " + str(result))
        return result

    def mc(self, start_addr):
        self.gen_pe()
        result = simso.MemClearPy(start_addr)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("mc(" + str(hex(start_addr)) + ") = " + str(result))
        return result

    def self_set(self, start_addr, end_addr, offset):
        self.gen_pe()
        result = simso.SelfSetPy(start_addr, end_addr, offset)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("self_set(" + str(hex(start_addr)) + "," +
                  str(hex(end_addr)) + "," + str(hex(offset)) + ") = " + str(result))
        return result

    def self_clear(self, start_addr):
        self.gen_pe()
        result = simso.SelfClearPy(start_addr)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("self_clear(" + str(hex(start_addr)) + ") = " + str(result))
        return result

    def mirror_set (self, start_addr, end_addr, offset):
        self.gen_pe ()
        result = simso.MirrorSetPy (start_addr, end_addr, offset)
        if simso.IsEnabledPrintPy() == 1 or result != 0 :
            print("mirror_set(" + str(hex(start_addr)) + "," + str(hex(end_addr)) + "," + str(hex(offset)) + ") = " + str(result))
        return result

    def mirror_clear (self, start_addr):
        self.gen_pe ()
        result = simso.MirrorClearPy (start_addr)
        if simso.IsEnabledPrintPy() == 1 or result != 0 :
            print("mirror_clear(" + str(hex(start_addr)) + ") = " + str(result))
        return result

    def error_area_set(self, start_addr, end_addr):
        self.gen_pe()
        result = simso.ErrorAreaSetPy(start_addr, end_addr)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("error_area_set(" + str(hex(start_addr)) +
                  "," + str(hex(end_addr)) + ") = " + str(result))
        return result

    def error_area_clear(self, start_addr):
        self.gen_pe()
        result = simso.ErrorAreaClearPy(start_addr)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("error_area_clear(" + str(hex(start_addr)) + ") = " + str(result))
        return result

    def pe_latency(self, latency):
        self.gen_pe()
        result = simso.SetPeLatency(latency)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("pe_latency(" + str(latency) + ") = " + str(result))
        return result

    def get_pe_latency(self):
        self.gen_pe()
        result = simso.GetPeLatency()
        if simso.IsEnabledPrintPy() == 1:
            print("get_pe_latency() = " + str(result))
        return result

    def lram_fetch_latency(self, latency):
        self.gen_pe()
        result = simso.SetLramFetchLatency(latency)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("lram_fetch_latency(" + str(latency) + ") = " + str(result))
        return result

    def get_lram_fetch_latency(self):
        self.gen_pe()
        result = simso.GetLramFetchLatency()
        if simso.IsEnabledPrintPy() == 1:
            print("get_lram_fetch_latency() = " + str(result))
        return result

    def rom_fetch_latency(self, latency):
        self.gen_pe()
        result = simso.SetRomFetchLatency(latency)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("rom_fetch_latency(" + str(latency) + ") = " + str(result))
        return result

    def get_rom_fetch_latency(self):
        self.gen_pe()
        result = simso.GetRomFetchLatency()
        if simso.IsEnabledPrintPy() == 1:
            print("get_rom_fetch_latency() = " + str(result))
        return result

    def quit(self):
        self.__dict__['quit_flag'] = 1
        simso.QuitPy()

    def reset(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            result = simso.HardResetPy()
        else:
            pe, tc = sim.GetContextArg(contextid[0])
            result = simso.ResetPePy(pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("reset() = " + str(result))
        return result

    def step(self, *args):
        self.gen_pe()
        step_count = CFDEF_DEF_STEP
        contextid_str = ''
        for arg in args:
            if isinstance(arg, int):
                step_count = arg
            elif isinstance(arg, str):
                contextid_str = arg
            else:
                print("Unknown argument in step command :" + str(arg))
                return
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.StepExecutePy(step_count, pe, tc)
        if simso.IsEnabledPrintPy() == 1:
            print("step() = " + str(result))
        return result

    def set_stepmode(self):
        self.gen_pe()
        result = simso.SwitchStepModePy()
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_stepmode() = " + str(result))
        return result

    def get_stepmode(self):
        self.gen_pe()
        result = simso.GetStepModePy()
        if simso.IsEnabledPrintPy() == 1:
            print("get_stepmode() = " + str(result))
        return result

    def help(self):
        simso.PrintHelpPy()

    def pic(self, name=CFDEF_DEF_OUT, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.DisplayInstructionCountPy(name, pe, tc)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("pic() = " + str(result))
        return result

    def picc(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.ClearInstructionCountPy(pe, tc)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("picc() = " + str(result))
        return result

    def debug(self, mode, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetDebugModePy(mode.lower(), pe, tc)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("debug(" + mode + ") = " + str(result))
        return result

    def exp_message(self, mode, start_cause, end_cause, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.UpdateExpMsgEnablePy(
            mode.lower(), start_cause, end_cause, pe, tc)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("exp_message(" + mode + "," + str(start_cause) +
                  "," + str(end_cause) + ") = " + str(result))
        return result

    def peinfo(self):
        self.gen_pe()
        result = simso.DisplayThreadInfoPy()
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("peinfo() = " + str(simso.DisplayThreadInfoPy()))
        return result

    def last_tcid(self, select_pe):
        self.gen_pe()
        result = simso.GetLastTcIdPy(select_pe)
        if simso.IsEnabledPrintPy() == 1:
            print("last_tcid(" + str(select_pe) + ") = " + str(result))
        return result

    def last_vcid(self, select_pe):
        print("Warning : Depreceted command \"last_vcid\" is used")
        return CFDEF_DEF_NC

    def last_peid(self):
        self.gen_pe()
        result = simso.GetLastPeIdPy()
        if simso.IsEnabledPrintPy() == 1:
            print("last_peid() = " + str(result))
        return result

    def setclk(self, ratio, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.SetFrequencyPy(ratio, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("setclk(" + str(ratio) + ") = " + str(result))
        return result

    def getclk(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetFrequencyPy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("getclk() = " + str(result))
        return result

    def setlog(self, fname, *multi_attr):
        self.gen_pe()
        is_multi = False
        if len(multi_attr) != 0:
            attr_char = multi_attr[0]
            if attr_char.lower() == 'multi':
                is_multi = True
        result = simso.SetLogFilePy(fname, is_multi)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("setlog(" + fname + ") = " + str(result))
        return result

    def set_snooze(self, period, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetSnoozePeriodPy(period, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_snooze(" + str(period) + ") = " + str(result))
        return result

    def get_snooze(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetSnoozePeriodPy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_snooze() = " + str(result))
        return result

    def set_tlbnum(self, data, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetTlbEntryPy(data, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_tlbnum(" + str(data) + ") = " + str(result))
        return result

    def get_tlbnum(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetTlbEntryPy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_tlbnum() = " + str(result))
        return result

    def set_mintlb(self, data, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetMinTlbSizePy(data, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_mintlb(" + str(data) + ") = " + str(result))
        return result

    def get_mintlb(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetMinTlbSizePy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_mintlb() = " + str(result))
        return result

    def set_mpm(self, data, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetMpuDefaultPy(data, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_mpm(" + str(data) + ") = " + str(result))
        return result

    def get_mpm(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetMpuDefaultPy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_mpm() = " + str(result))
        return result

    def set_mpunum(self, data, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetMpuRegionNumPy(data, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_mpunum(" + str(data) + ") = " + str(result))
        return result

    def get_mpunum(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetMpuRegionNumPy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_mpunum() = " + str(result))
        return result

    def set_mpubank(self, data, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetMpuBankNumPy(data, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_mpubank(" + str(data) + ") = " + str(result))
        return result

    def get_mpubank(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetMpuBankNumPy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_mpubank() = " + str(result))
        return result

    def set_l1rlinknum(self, data, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetNumL1RLinkPy(data, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_l1rlinknum(" + str(data) + ") = " + str(result))
        return result

    def get_l1rlinknum(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetNumL1RLinkPy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_l1rlinknum() = " + str(result))
        return result

    def set_l1rsize(self, data, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetSizeL1RPy(data, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_l1rsize(" + str(data) + ") = " + str(result))
        return result

    def get_l1rsize(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetSizeL1RPy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_l1rsize() = " + str(result))
        return result

    def set_peid(self, data, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetPeidPy(data, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_peid(" + str(data) + ") = " + str(result))
        return result

    def get_peid(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetPeidPy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_peid() = " + str(result))
        return result

    def set_spid(self, data, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetSpidPy(data, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_spid(" + str(data) + ") = " + str(result))
        return result

    def get_spid(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetSpidPy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_spid() = " + str(result))
        return result

    def set_spidlist(self, data, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetSpidlistPy(data, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_spidlist(" + str(data) + ") = " + str(result))
        return result

    def get_spidlist(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetSpidlistPy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_spidlist() = " + str(result))
        return result

    def set_bmid(self, data, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetBmidPy(data, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_bmid(" + str(data) + ") = " + str(result))
        return result

    def get_bmid(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetBmidPy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_bmid() = " + str(result))
        return result

    def set_rbase(self, data, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetRbasePy(data, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_rbase(" + str(data) + ") = " + str(result))
        return result

    def get_rbase(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetRbasePy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_rbase() = " + str(result))
        return result

    def set_mctl(self, data, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetMctlPy(data, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_mctl(" + str(data) + ") = " + str(result))
        return result

    def get_mctl(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetMctlPy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_mctl() = " + str(result))
        return result

    def set_mpudmdp(self, data, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetMpuDmdpPy(data, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_mpudmdp(" + str(data) + ") = " + str(result))
        return result

    def get_mpudmdp(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetMpuDmdpPy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_mpudmdp() = " + str(result))
        return result

    def set_icctrl(self, data, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetIcctrlPy(data, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_icctrl(" + str(data) + ") = " + str(result))
        return result

    def get_icctrl(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetIcctrlPy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_icctrl() = " + str(result))
        return result

    def set_icsize(self, size, way, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetIcSizePy(size, way, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_icsize(" + str(size) + "," + str(way) + ") = " + str(result))
        return result

    def get_icsize(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetIcSizePy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_icsize() = " + str(result))
        return result

    def set_dcctrl(self, data, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetDcctrlPy(data, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_dcctrl(" + str(data) + ") = " + str(result))
        return result

    def get_dcctrl(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetDcctrlPy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_dcctrl() = " + str(result))
        return result

    def set_dcsize(self, size, way, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetDcSizePy(size, way, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_dcsize(" + str(size) + "," + str(way) + ") = " + str(result))
        return result

    def get_dcsize(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetDcSizePy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_dcsize() = " + str(result))
        return result

    def set_peinfo(self, peid, *coretype_arg):
        if self.is_gen_pe():
            print("Can't exec this command after other commands")
        if len(coretype_arg) == 0:
            coretype = CFDEF_CORETYPE
            clid = 0
        elif len(coretype_arg) == 1:
            if (isinstance(coretype_arg[0], (int, long))):
                coretype = CFDEF_CORETYPE
                clid = coretype_arg[0]
            else:
                coretype = coretype_arg[0]
                clid = 0
        else:
            coretype = coretype_arg[1]
            clid = coretype_arg[0]
        result = simso.SetPeInfoPy(peid, clid, coretype.lower())
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_peinfo(" + str(peid) + "," + str(clid) +
                  "," + str(coretype) + ") = " + str(result))
        return result

    def set_area_rom(self, start_addr, end_addr):
        self.gen_pe()
        result = simso.SetRomAreaPy(start_addr, end_addr)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_area_rom(" + str(hex(start_addr)) + "," +
                  str(hex(end_addr)) + ") = " + str(result))
        return result

    def set_area_lram(self, start_addr, end_addr):
        self.gen_pe()
        result = simso.SetLocalRamAreaPy(start_addr, end_addr)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_area_lram(" + str(hex(start_addr)) +
                  "," + str(hex(end_addr)) + ") = " + str(result))
        return result

    def set_intc1(self, pe_id, start_addr):
        self.gen_pe()
        simso.Intc1SetPy(pe_id, start_addr)

    def set_intc2(self, start_addr):
        self.gen_pe()
        simso.Intc2SetPy(start_addr)

    def set_req_eiint_to_intc1(self, pe_id, channel):
        self.gen_pe()
        simso.ReqEIINTfromexternalPy(pe_id, channel)

    def set_req_feint_to_intc1(self, pe_id, channel):
        self.gen_pe()
        simso.ReqFEINTfromexternalPy(pe_id, channel)

    def set_ostm(self, bind_pe, ostmch, start_addr, intch):
        self.gen_pe()
        result = simso.OstmLocalSetPy(bind_pe, ostmch, start_addr, intch)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_ostm(" + str(bind_pe) + "," + str(ostmch) + "," +
                  str(hex(start_addr)) + "," + str(intch) + ") = " + str(result))
        return result

    def set_ostm_global(self, start_addr, intch):
        self.gen_pe()
        result = simso.OstmGlobalSetPy(start_addr, intch)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_ostm_global(" + str(hex(start_addr)) +
                  "," + str(intch) + ") = " + str(result))
        return result

    def set_ostm_clk(self, ratio):
        self.gen_pe()
        result = simso.SetOstmClkPy(ratio)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_ostm_clk(" + str(ratio) + ") = " + str(result))
        return result

    def get_ostm_clk(self):
        self.gen_pe()
        result = simso.GetOstmClkPy()
        if simso.IsEnabledPrintPy() == 1:
            print("get_ostm_clk() = " + str(result))
        return result

    def set_has_simd(self, has):
        print("set_has_simd command is deprecated. Use set_has_fxu command instead.")
        result = simso.SetHasFxuPy(has)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_has_simd(" + str(has) + ") = " + str(result))
        return result

    def set_has_fxu(self, has):
        result = simso.SetHasFxuPy(has)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_has_fxu(" + str(has) + ") = " + str(result))
        return result

    def set_has_fpu(self, has):
        result = simso.SetHasFpuPy(has)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_has_fpu(" + str(has) + ") = " + str(result))
        return result

    def set_has_mpu(self, has):
        result = simso.SetHasMpuPy(has)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_has_mpu(" + str(has) + ") = " + str(result))
        return result

    def set_has_mmu(self, has):
        print("set_has_mmu command is deprecated.")
        result = simso.SetHasMmuPy(has)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_has_mmu(" + str(has) + ") = " + str(result))
        return result

    def set_peripherals(self, *peri_str):
        if len(peri_str) < 2:
            print("Parameter is lacked")
            return False
        elif len(peri_str) > 3:
            print("Invalid parameters")
            return False
        else:
            name = peri_str[0]
            has = peri_str[1]
            if len(peri_str) == 3:
                base_addr = peri_str[2]
                result = simso.SetPeripheralPy(name.lower(), has.lower(), base_addr)
            else:
                base_addr = 0
                result = simso.SetPeripheralPy(name.lower(), has.lower(), base_addr)
            if simso.IsEnabledPrintPy() == 1 or result != 0:
                print("set_peripherals(" + str(peri_str) + ") = " + str(result))
            return result

    def set_ipir_eiint_ch(self, offset_intch):
        result = simso.SetIpirEiintChannelPy(offset_intch)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_ipir_eiint_ch(" + str(offset_intch) + ") = " + str(result))
        return result

    def set_guards(self, name, *param_str):
        # param_str : id(if has) , has(must), address (must)
        if len(param_str) <= 1:
            print("Parameter is lacked")
            return False
        elif len(param_str) == 2:
            if (name.lower() != 'cfg' and name.lower() != 'cfgcaps'):
                print("Parameter is lacked")
                return False
            id = 0
            has = param_str[0]
            address = param_str[1]
        elif len(param_str) == 3:
            id = param_str[0]
            has = param_str[1]
            address = param_str[2]
        else:
            print("Parameter is too many")
            return False
        result = simso.SetGuardPy(name.lower(), id, has.lower(), address)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_guards(" + str(name) + ") = " + str(result))
        return result

    def set_guards_param(self, name, id, param_name, value):
        result = simso.SetGuardParamPy(name.lower(), id, param_name.lower(), value)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_guards_param(" + str(name) + "," + str(hex(id)) + "," + str(param_name) + "," + str(hex(value)) + ") = " + str(result))
        return result

    def set_area_debug(self, start_addr, end_addr, pe):
        self.gen_pe()
        result = simso.SetDebugAreaPy(start_addr, end_addr, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_area_debug(" + str(hex(start_addr)) + "," +
                  str(hex(end_addr)) + "," + str(pe) + ") = " + str(result))
        return result

    def clear_area_debug(self, start_addr):
        self.gen_pe()
        result = simso.ClearDebugAreaPy(start_addr)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("clear_area_debug(" + str(hex(start_addr)) + ") = " + str(result))
        return result

    def set_peripheral_cycle(self, start_addr, end_addr, read_lat, write_lat, pe_latency, bind_pe, outstand, repeat_rate):
        self.gen_pe()
        result = simso.SetPeripheralLatencyPy(
            start_addr, end_addr, read_lat, write_lat, pe_latency, bind_pe, outstand, repeat_rate)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_peripheral_cycle(" + str(hex(start_addr)) + "," + str(hex(end_addr)) + "," + str(read_lat) + "," + str(write_lat) +
                  "," + str(pe_latency) + "," + str(bind_pe) + "," + str(outstand) + "," + str(repeat_rate) + ") = " + str(result))
        return result

    def set_area_dc(self, start_addr1, end_addr1, start_addr2, end_addr2):
        result = simso.SetDcAreaPy(
            start_addr1, end_addr1, start_addr2, end_addr2)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_area_dc(" + str(hex(start_addr1)) + "," + str(hex(end_addr1)) +
                  "," + str(hex(start_addr2)) + "," + str(hex(end_addr2)) + ") = " + str(result))
        return result

    def get_area_dc(self):
        result = simso.GetDcAreaPy()
        if simso.IsEnabledPrintPy() == 1:
            print("get_area_dc() = " + str(result))
        return result

    def dump_btb(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.DumpBtbStatusPy(pe, tc)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("dump_btb() = " + str(result))
        return result

    def reset_btb(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.ResetBtbStatusPy(pe, tc)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("reset_btb() = " + str(result))
        return result

    def show_stat(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.ShowPerfInfoPy(pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("show_stat() = " + str(result))
        return result

    def reset_stat(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.ResetPerfInfoPy(pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("reset_stat() = " + str(result))
        return result

    def exec_cfali(self, start_addr, end_addr, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.ExecCfaliPy(start_addr, end_addr, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("exec_cfali(" + str(hex(start_addr)) + "," +
                  str(hex(end_addr)) + ") = " + str(result))
        return result

    def is_valid_tcid(self, select_pe, temp_tcid):
        self.gen_pe()
        if (temp_tcid <= simso.GetLastTcIdPy(select_pe) and temp_tcid >= 0) or temp_tcid == CFDEF_DEF_NC:
            return True
        else:
            return False

    def is_valid_vcid(self, select_pe, temp_vcid):
        print("Warning : Depreceted command \"is_valid_vcid\" is used")
        return False

    def is_valid_peid(self, temp_peid):
        self.gen_pe()
        if simso.IsValidPeIdPy(temp_peid):
            return True
        else:
            return False

    def get_first_peid(self):
        curr_peid = 0
        last_peid = simso.GetLastPeIdPy()
        while curr_peid < last_peid:
            if simso.IsValidPeIdPy(curr_peid):
                return curr_peid
            curr_peid += 1
        return last_peid

    def gen_pe(self):
        if not self.is_gen_pe():
            simso.GeneratePePy()
            base_peid = self.get_first_peid()
            self.init_curr_context(base_peid)

    def is_gen_pe(self):
        if simso.IsGenPePy():
            return True
        else:
            return False

    def init_curr_context(self, base_peid):
        self.m_pe = base_peid
        self.m_tc = CFDEF_DEF_TC

    def set_debug_option(self, option, mode):
        self.gen_pe()
        result = simso.SetDebugOptionPy(option, mode)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_debug_option(" + str(option) +
                  "," + str(mode) + ") = " + str(result))
        return result

    def get_debug_option(self):
        self.gen_pe()
        result = simso.GetDebugOptionPy()
        if simso.IsEnabledPrintPy() == 1:
            print("get_debug_option() = " + str(result))
        return result

    def get_arch_class(self):
        self.gen_pe()
        result = simso.GetArchClassPy()
        if simso.IsEnabledPrintPy() == 1:
            print("get_arch_class() = " + str(result))
        return result

    def set_msglvl(self, lvl):
        self.gen_pe()
        result = simso.SetMsgLvlPy(lvl)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_msglvl(" + str(lvl) + ") = " + str(result))
        return result

    def get_msglvl(self):
        self.gen_pe()
        result = simso.GetMsgLvlPy()
        if simso.IsEnabledPrintPy() == 1:
            print("get_msglvl() = " + str(result))
        return result

    def use_stopio(self, enable):
        self.gen_pe()
        result = simso.UseStopIOPy(enable)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("use_stopio(" + str(enable) + ") = " + str(result))
        return result

    def set_fpu_existence(self, enable, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetFpuExistencePy(enable, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_fpu_existence(" + str(enable) + ") = " + str(result))
        return result

    def get_fpu_existence(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetFpuExistencePy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_fpu_existence() = " + str(result))
        return result

    def set_simd_existence(self, enable, *contextid):
        print("set_simd_existence command is deprecated. Use set_fxu_existence command instead.")
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetFxuExistencePy(enable, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_simd_existence(" + str(enable) + ") = " + str(result))
        return result

    def get_simd_existence(self, *contextid):
        print("get_simd_existence command is deprecated. Use get_fxu_existence command instead.")
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetFxuExistencePy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_simd_existence() = " + str(result))
        return result

    def set_fxu_existence(self, enable, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetFxuExistencePy(enable, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_fxu_existence(" + str(enable) + ") = " + str(result))
        return result

    def get_fxu_existence(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetFxuExistencePy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_fxu_existence() = " + str(result))
        return result

    def set_mpu_existence(self, enable, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetMpuExistencePy(enable, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_mpu_existence(" + str(enable) + ") = " + str(result))
        return result

    def get_mpu_existence(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetMpuExistencePy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_mpu_existence() = " + str(result))
        return result

    def set_mmu_existence(self, enable, *contextid):
        print("set_mmu_existence command is deprecated.")
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetMmuExistencePy(enable, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_mmu_existence(" + str(enable) + ") = " + str(result))
        return result

    def get_mmu_existence(self, *contextid):
        print("get_mmu_existence command is deprecated.")
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetMmuExistencePy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_mmu_existence() = " + str(result))
        return result

    def set_has_hv(self, has):
        result = simso.SetHasHvPy(has)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_has_hv(" + str(has) + ") = " + str(result))
        return result

    def set_hvcfg(self, data, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetHvcfgPy(data, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_hvcfg(" + str(data) + ") = " + str(result))
        return result

    def get_hvcfg(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetHvcfgPy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_hvcfg() = " + str(result))
        return result

    def set_dbgen(self, data, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetDbgenPy(data, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_dbgen(" + str(data) + ") = " + str(result))
        return result

    def get_dbgen(self, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            contextid_str = ''
        else:
            contextid_str = contextid[0]
        pe, tc = sim.GetContextArg(contextid_str)
        result = simso.GetDbgenPy(pe)
        if simso.IsEnabledPrintPy() == 1:
            print("get_dbgen() = " + str(result))
        return result

    def set_lscfg(self, data, *contextid):
        self.gen_pe()
        if len(contextid) == 0:
            pe, tc = CFDEF_NOSEL_CONTEXT
        else:
            pe, tc = sim.GetContextArg(contextid[0])
        result = simso.SetLscfgPy(data, pe)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_lscfg(" + str(data) + ") = " + str(result))
        return result

    def set_memini(self, value):
        self.gen_pe()
        result = simso.SetMemInitPy(value)
        if simso.IsEnabledPrintPy() == 1 or result != 0:
            print("set_memini(" + str(value) + ") = " + str(result))
        return result

    def get_memini(self):
        self.gen_pe()
        result = simso.GetMemInitPy()
        if simso.IsEnabledPrintPy() == 1:
            print("get_memini() = " + str(result))
        return result

    def GetContext(self):
        # pe/tcコマンドよって指定されたPE/TCを使用する
        return self.m_pe, self.m_tc

    def GetContextArg(self, context_str):
        # 引数によって指定された一時的なPE/TC指定を取得する
        context_str = context_str.lower()
        pos = 0
        end_pos = 0
        str_len = len(context_str)
        if str_len == 0:
            # 指定されていないので、カレントスレッドを使用
            temp_pe = self.m_pe
            temp_tc = self.m_tc
        elif not sim.is_context_id(context_str):
            print("Invalid ID is used :" + context_str)
            return CFDEF_INVALID_CONTEXT
        else:
            # 引数で指定されたので、カレントスレッドを無視
            temp_pe = CFDEF_NOSEL_ID
            temp_tc = CFDEF_NOSEL_ID
        while str_len > pos:
            if context_str[pos] == 'p':
                # PE取得
                pos += 1
                end_pos = sim.GetIdEndPos(context_str, pos)
                if end_pos == pos:
                    print("Invalid PEID is used :" + context_str)
                    return CFDEF_INVALID_CONTEXT
                else:
                    temp_pe = int(context_str[pos:end_pos])
                    pos = end_pos
            elif context_str[pos] == 't':
                # TC取得
                pos += 1
                end_pos = sim.GetIdEndPos(context_str, pos)
                if end_pos == pos:
                    # TC=NCの場合をチェック（TNCと指定される）
                    if context_str[pos] == 'n' and (context_str[pos + 1] == 'c' or context_str[pos + 1] == 'm'):
                        temp_tc = CFDEF_DEF_NC
                        pos += 2
                    else:
                        print("Invalid TCID is used :" + context_str)
                        return CFDEF_INVALID_CONTEXT
                else:
                    temp_tc = int(context_str[pos:end_pos])
                    pos = end_pos
            elif context_str[pos] == 'n'and context_str[pos + 1] == 'c':
                # P1NCと指定された場合はTC=NCに設定
                if temp_tc != CFDEF_DEF_NC and temp_tc != CFDEF_NOSEL_ID:
                    print("Invalid ID is used :" + context_str)
                    return CFDEF_INVALID_CONTEXT
                temp_tc = CFDEF_DEF_NC
                pos += 2
            else:
                print("Invalid ID is used :" + context_str)
                return CFDEF_INVALID_CONTEXT
        return temp_pe, temp_tc

    def GetIdEndPos(self, context_str, pos):
        end_pos = pos
        for id in context_str[pos:]:
            if not id.isdigit():
                break
            end_pos += 1
        return end_pos

    def GetLoadNameCmdopt(self):
        return simso.GetLoadNameCmdoptPy()

    def LoadElf(self, filename):
        try:
            f = open(filename, "rb")
        except IOError:
            print("<Error: Can not open file='%s'>" % filename)
            return "NO_FILE"
        elf = f.read()
        f.close()
        # Check magic numbers
        # assert len(elf) >= 52,        "not found: ELF32 header"
        # assert elf[0:4] == '\x7fELF', "not found: ELF signature"
        # assert ord(elf[4]) == 1,      "not 32bit"
        # assert ord(elf[5]) == 1,      "not little endian"
        if len(elf) < 52 or elf[0:4] != '\x7fELF' or ord(elf[4]) != 1 or ord(elf[5]) != 1:
            return "NOT_ELF"
        (e_type,
         e_machine,
         e_version,
         e_entry,
         e_phoff,
         e_shoff,
         e_flags,
         e_ehsize,
         e_phentsize,
         e_phnum,
         e_shentsize,
         e_shnum,
         e_shstrndx) = unpack("<HHLLLLLHHHHHH", elf[16:52])
        # assert e_type    == 2, "not Executable"
        # assert e_machine == 36 or e_machine == 87, "not v800 or v850"
        if e_type != 2:
            print("<Error: ELF isn't Executable type.>")
            return "NOT_SUPPORTED_ELF"
        if e_machine != 36 and e_machine != 87:
            print("<Error: ELF isn't for v800/v850.>")
            return "NOT_SUPPORTED_ELF"

        class Elf32_Phdr(object):

            def __init__(self, data, pos):
                (self.p_type,
                 self.p_offset,
                 self.p_vaddr,
                 self.p_paddr,
                 self.p_filesz,
                 self.p_memsz,
                 self.p_flags,
                 self.p_align) = unpack("<LLLLLLLL", data[pos: pos + 32])

        phs = [Elf32_Phdr(elf, e_phoff + i * e_phentsize)
               for i in range(e_phnum)]
        # load
        for ph in phs:
            if ph.p_type == 1:  # PT_LOAD
                # In GHS object, p_paddr isn't supported. Then, p_paddr is 0.
                if ph.p_paddr != 0:
                    write_addr = ph.p_paddr
                else:
                    write_addr = ph.p_vaddr
                read_addr, size = ph.p_offset, ph.p_filesz
                # print "LOAD: %08x-%08x => %08x-%08x" % (# read_addr, read_addr +
                # size - 1, write_addr, write_addr + size - 1)
                read_addr_cur = read_addr
                read_addr_end = read_addr + size
                pe, tc = CFDEF_NOSEL_CONTEXT
                while read_addr_cur < read_addr_end:
                    bin_data = unpack("<B", elf[read_addr_cur])[0]
                    # Write data to CForest memory
                    simso.MemEditPy(write_addr, bin_data, 1, pe, tc)
                    # Increment addr for next
                    write_addr = write_addr + 1
                    read_addr_cur = read_addr_cur + 1

    # alias定義
    s = step
    rs = reset
    q = quit
    load = ml
