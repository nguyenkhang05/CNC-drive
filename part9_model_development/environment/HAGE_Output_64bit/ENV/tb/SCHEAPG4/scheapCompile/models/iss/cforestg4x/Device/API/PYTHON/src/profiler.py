# -*- coding: utf-8 -*-
#######################################
# Profiler
#######################################
# from logging import getLogger, basicConfig, CRITICAL, ERROR, WARNING, INFO, DEBUG, NOTSET
from logging import basicConfig
from logging import DEBUG
from logging import getLogger
from logging import NOTSET
logger = getLogger('prof')
logging_level = NOTSET
logging_level = DEBUG
basicConfig(level=logging_level, filename='log.txt',
            format='%(name)s: %(levelname)s: %(funcName)s:%(lineno)s: %(message)s', datefmt='%H:%M:%S')


class profiler(object):
    address_symbol_map = {}
    symbol_size_map = {}
    prof_func_top_list = []
    prof_func_out_list = []
    cur_status = {}
    cur_function = {}
    pre_function = {}
    master_pe = 0 # G3MH is 1, G4MH is 0
    profile_function = {}
    is_profiling_map = {}
    plugins = {}

    def setup(self):
        self.symtbl = el.getSymbols(type='FUNC')
        if self.symtbl != []:
            pass
        else:
            print("Please load the ELF file first because there is no symbol information.")
            return None
        logger.debug('self.symtbl:')
        logger.debug(self.symtbl)
        # Set break for all func top
        self.prof_func_top_list = self._undup_list([_[1] for _ in self.symtbl])
        logger.debug(["0x%s" % format(_, 'x')
                      for _ in self.prof_func_top_list])
        [bp(_) for _ in self.prof_func_top_list]
        # Set the data to ADDRESS_MAP in order to search function name.
        self.address_symbol_map = {int(_[1]): _[0]
                                   for _ in self.symtbl if _[2] > 0}
        self.symbol_size_map = {str(_[0]): _[2]
                                for _ in self.symtbl if _[2] > 0}
        logger.debug("address_symbol_map: %s" %
                     {"0x%s" % format(_a, 'x'): _b
                      for _a, _b in self.address_symbol_map.iteritems()})
        logger.debug('symbol_size_map: %s' % self.symbol_size_map)
        # Use performance counters
        # set_peripherals ("perf", "on"); # perfcnt
        # self_set (0xfffe0000, 0xffffffff, -0x00002000) #self area for perfcnt and result_register
        # Initialize plugins
        for plugin in self.plugins.keys():
            logger.debug(self.plugins[plugin])
            self.plugins[plugin].init()

        logger.debug("symbol_table       %s" % self.symtbl)
        logger.debug("address_symbol_map %s" % self.address_symbol_map)
        bpd()

    def set_profile_function(self, func, contextid=''):
        pe, tc = sim.GetContextArg(contextid)
        # print("set_profile_function(): pe = %d, tc = %d" % (pe, tc))
        self.profile_function[(pe, tc)] = func
        logger.debug("profile_function: %s" % self.profile_function)

    def start(self):
        class BreakEvent(object):
            BREAK_NONE = 0  # Initial value
            BREAK_PC = 1  # Break point
            BREAK_ACC_READ = 2  # Access Break (read)
            BREAK_ACC_WRITE = 3  # Access Break (write)
            BREAK_MAX = 4  # Max Break
            BREAK_CTRL_C = 7  # Ctrl-C
            BREAK_HALT = 8  # HALT

        debug("OFF")
        # print("SYMBOL_TABLE       ", self.symtbl)
        # print("address_symbol_map ", self.address_symbol_map)
        # print("profile_function   ", self.profile_function)

        if len(self.symtbl) <= 0:
            print(
                "Please load the ELF file first because there is no symbol information.")
            return None
        elif len(self.address_symbol_map) <= 0:
            print("Please setup() first because there is no setup for profile target..")
            return None
        elif len(self.profile_function) <= 0:
            print("Please set_profile_function() first because there is no profile target..")
            return None

        # first run (skip prologue)
        result = run()
        logger.debug("run result: %d" % result[0])
        while isinstance(result, tuple) and result[0] == BreakEvent.BREAK_PC:
            pe = result[2]
            tc = result[3]
            pc = getreg("pc")

            logger.debug("BreakEvent: BREAK_PC, PC=%08x" % (pc))
            if pc in self.address_symbol_map:
                logger.debug(self.address_symbol_map[pc])
            else:
                logger.debug("Can't find address_symbol: %08x" % pc)

            # Check whether the PC is in a function top.
            if pc in self.prof_func_top_list:
                self._set_now_function(self._get_function_name(pc), pe, tc)
                self._set_now_status(True, pe, tc)
                logger.debug("{} IN".format(self.cur_function[(pe, tc)]))
                print("Func: {}, PE = {}, TC = {}, PC = {}, In: {}".format(
                    self.cur_function[(pe, tc)], pe, tc, hex(pc), self.pre_function[(pe, tc)]))
                self._set_break_on_funcend()

                # Check profiling start
                if not self._is_profiling(pe, tc):
                    logger.debug("{} == {}? then set profiling true".format(self.profile_function[(pe, tc)], self.cur_function[(pe, tc)]))
                    if self.profile_function[(pe, tc)] == self.cur_function[(pe, tc)]:
                        self._set_now_profiling(True, pe, tc)
                        debug("ON")

                logger.debug("{} is profiling?: {}".format(self.cur_function[(pe, tc)], self._is_profiling(pe, tc)))

                # Profiling data
                if self._is_profiling(pe, tc):
                    # IN-IN, OUT-IN
                    [self.plugins[plugin].checkpoint(
                        self.cur_status[(pe, tc)],
                        self.cur_function[(pe, tc)],
                        self.pre_function[(pe, tc)],
                        pe, tc) for plugin in self.plugins.keys()]

            # Check whether the PC is in a function out.
            elif pc in self.prof_func_out_list:
                self._set_now_function(self._get_function_name(pc), pe, tc)
                self._set_now_status(False, pe, tc)
                logger.debug("{} OUT".format(self.cur_function[(pe, tc)]))
                print("Func: {}, PE = {}, TC = {}, PC = {}, Out: {}".format(
                    self.cur_function[(pe, tc)], pe, tc, hex(pc), self.pre_function[(pe, tc)]))

                # Profiling data
                if self._is_profiling(pe, tc):
                    # IN_OUT, OUT-OUT
                    [self.plugins[plugin].checkpoint(
                        self.cur_status[(pe, tc)],
                        self.cur_function[(pe, tc)],
                        self.pre_function[(pe, tc)],
                        pe, tc) for plugin in self.plugins.keys()]

                # Check profiling finish
                if self._is_profiling(pe, tc):
                    logger.debug("Check profiling finish: s:%s == pre:%s and cur:%s != pre:%s" % (self.profile_function[(pe, tc)], self.pre_function[(pe, tc)], self.profile_function[(pe, tc)], self.pre_function[(pe, tc)]))

                    if self.profile_function[(pe, tc)] == self.pre_function[(pe, tc)] and self.cur_function[(pe, tc)] != self.pre_function[(pe, tc)] :
                        self._set_now_profiling(False, pe, tc)
                        debug("OFF")
                    # Delete break pointer if not master PE
                    logger.debug("PE=%d, master_PE=%d" % (pe, self.master_pe))
                    if pe != self.master_pe:
                        logger.debug('prof_func_top_list: %s' % [
                                     "0x%s" % format(_, 'x') for _ in self.prof_func_top_list])
                        logger.debug('prof_func_out_list: %s' % [
                                     "0x%s" % format(_, 'x') for _ in self.prof_func_out_list])
                        sorted_list = sorted(self.prof_func_top_list + self.prof_func_out_list,
                                             cmp=lambda x, y: cmp(int(x), int(y)))
                        sorted_list = self._undup_list(sorted_list)
                        for addr in sorted_list:
                            result = simso.ClearBreakPcPy(addr, pe, tc)
                        logger.debug('Clear Break: %s' % [
                                     "0x%s" % format(_, 'x') for _ in sorted_list])
                        # bpd()

            # user break
            elif (pc not in self.prof_func_top_list and pc not in self.prof_func_out_list):
                print("User break: PC = ", hex(pc))
                break

            if isinstance(result, tuple):
                # accesss break
                if (result[0] == BreakEvent.BREAK_ACC_READ or result[0] == BreakEvent.BREAK_ACC_WRITE):
                    print("Access break: PC = ", hex(pc))
                    break
                # max break
                if result[0] == BreakEvent.BREAK_MAX:
                    print("Reached max break: PC = ", hex(pc))
                    break

            # HALT
            if isinstance(result, int):
                if result == BreakEvent.BREAK_HALT:
                    print("Reached HALT: PC = ", hex(pc))
                    # Profiling data
                    if self._is_profiling(pe, tc):
                        # HALT OUT
                        [self.plugins[plugin].checkpoint(
                            self.cur_status[(pe, tc)],
                            self.cur_function[(pe, tc)],
                            self.pre_function[(pe, tc)],
                            pe, tc) for plugin in self.plugins.keys()]
                    break

            # GHS exit loop
            if "__Exit" == self.cur_function[(pe, tc)] or "__exit" == self.cur_function[(pe, tc)]:
                print("Reached __Exit: PC = ", hex(pc))
                # Profiling data
                if self._is_profiling(pe, tc):
                    # __EXIT OUT
                    [self.plugins[plugin].checkpoint(
                        self.cur_status[(pe, tc)],
                        self.cur_function[(pe, tc)],
                        self.pre_function[(pe, tc)],
                        pe, tc) for plugin in self.plugins.keys()]
                break

            result = run()
            logger.debug("run result: %d" % result[0])
            # bpd()
        else:
            logger.debug("BreakEvent: %d, PC=%08x" % (result[0], pc))

        # calc profiled data
        for plugin in self.plugins.keys():
            print(plugin)
            self.plugins[plugin].disp_info()

    def set_plugin(self, plugin_name, plugin):
        self.plugins[plugin_name] = plugin

    def _set_now_function(self, func, pe, tc):
        self.pre_function[(pe, tc)] = self.cur_function.setdefault(
            (pe, tc), 'unknown')
        self.cur_function[(pe, tc)] = func
        logger.debug("Pre func: %s" % self.pre_function[(pe, tc)])
        logger.debug("Cur func: %s" % self.cur_function[(pe, tc)])

    def _set_now_status(self, stat, pe, tc):
        ''' status: CheckIn: True, CheckOut: False '''
        self.cur_status[(pe, tc)] = stat

    def _set_now_profiling(self, is_profiling, pe, tc):
        ''' profiling status: Profiling: True, Not Profiling: False '''
        self.is_profiling_map[(pe, tc)] = is_profiling

    def _is_profiling(self, pe, tc):
        ''' profiling status: Profiling: True, Not Profiling: False '''
        return self.is_profiling_map.setdefault((pe, tc), False)

    def _get_function_name(self, pc):
        func_addresses = sorted(
            self.address_symbol_map.keys(), cmp=lambda x, y: cmp(int(x), int(y)))
        for addr in func_addresses:
            func_name = self.address_symbol_map[addr]
            if (pc >= addr):
                if (pc < (addr + self.symbol_size_map[func_name])):
                    return func_name
        return "unknown"

    def _set_break_on_funcend(self):
        ''' Set a breakpoint on the end point of the function '''
        lp = getreg("r31")
        logger.debug("return address: lp = %s" % hex(lp))
        if lp == 0:
            print("Invalid return address: lp = ", hex(lp))
        if lp not in self.prof_func_out_list:
            bp(lp)
            self.prof_func_out_list.append(int(lp))
            self.prof_func_out_list = self._undup_list(self.prof_func_out_list)

    def _undup_list(self, seq):
        ''' Remove duplicated data '''
        seen = set()
        seen_add = seen.add
        return [x for x in seq if x not in seen and not seen_add(x)]


def setup_profile():
    return prof.setup()


def set_profile_function(func, contextid=''):
    return prof.set_profile_function(func, contextid)


def start_profile():
    return prof.start()

#######################################
# Plugins
#######################################
import pickle
# callgraph plugin


class plugin_callgraph(object):
    _callgraph = {}

    def init(self):
        print("call-graph plugin loaded.")
        pass

    def checkpoint(self, cur_status, cur_func, pre_func, pe, tc):
        logger.debug(cur_status)
        if cur_status:
            self._callgraph[(cur_func, pre_func, pe, tc)] = self._callgraph.setdefault(
                (cur_func, pre_func, pe, tc), 0) + 1
            logger.debug(self._callgraph[(cur_func, pre_func, pe, tc)])

    def disp_info(self):
        print(self._callgraph)

    def dump(self, file):
        with open("%s_callgraph.pickle" % file, 'wb') as f:
            pickle.dump(self._callgraph, f)

    def load(self, file):
        with open("%s_callgraph.pickle" % file, 'rb') as f:
            self._callgraph = pickle.load(f)

    def get_keys(self):
        return self._callgraph.keys()

    def get_value(self, key):
        return self._callgraph[key]

    def version(self):
        return self.__class__.__name__, "version", "1.0"

# cycle count plugin


class plugin_cycle(object):
    _cycle_count = {}
    _cycle_cache = 0

    def init(self):
        print("cycle counter plugin loaded.")
        pass

    def _get_cycle(self, pe):
        cycle_u = mr(0xffff7ff4, 4, "P%d" % pe)
        cycle_l = mr(0xffff7ff0, 4, "P%d" % pe)
        cycle = (cycle_u << 32) + cycle_l
        return cycle

    def checkpoint(self, cur_status, cur_func, pre_func, pe, tc):
        cycle = self._get_cycle(pe)
        self._cycle_count[(pre_func, pe, tc)] = self._cycle_count.setdefault(
            (pre_func, pe, tc), 0) + cycle - self._cycle_cache
        self._cycle_cache = cycle
        logger.debug(self._cycle_cache)

    def disp_info(self):
        print(self._cycle_count)

    def dump(self, file):
        with open("%s_cycle.pickle" % file, 'wb') as f:
            pickle.dump(self._cycle_count, f)

    def load(self, file):
        with open("%s_cycle.pickle" % file, 'rb') as f:
            self._cycle_count = pickle.load(f)

    def get_keys(self):
        return self._cycle_count.keys()

    def get_value(self, key):
        return self._cycle_count[key]

    def version(self):
        return self.__class__.__name__, "version", "1.0"

# Plugins command


def save_plugin_data(bench_name):
    ''' Save plugin data '''
    for plugin in prof.plugins.keys():
        prof.plugins[plugin].dump(bench_name)


def load_plugin_data(bench_name):
    ''' Load plugin data '''
    for plugin in prof.plugins.keys():
        prof.plugins[plugin].load(bench_name)

# Special Command


def save_csv(benchmark_name):
    '''
    Save the data in CSV format
    Need plugin
    1. callgraph
    2. cycle
    '''
    import csv
    with open('%s.csv' % benchmark_name, 'w') as f:
        writer = csv.writer(f, lineterminator='\n',
                            quoting=csv.QUOTE_NONNUMERIC)
        # Header
        list = ['Func', 'PE', 'TC', 'Call Count', 'Cycle Count']
        writer.writerow(list)
        # Body
        node_list = prof.plugins['callgraph'].get_keys()
        callgraph_normalize = {}
        for node in node_list:
            key_callgraph = (node[0], node[1], node[2], node[3])
            key_cycle = (node[0], node[2], node[3])
            callgraph_normalize[key_cycle] = callgraph_normalize.setdefault(
                key_cycle, 0) + prof.plugins['callgraph'].get_value(key_callgraph)
        for node in callgraph_normalize.keys():
            list = [node[0], node[1], node[2], callgraph_normalize[
                node], prof.plugins['cycle'].get_value(node)]
            writer.writerow(list)


def save_callgraph(benchmark_name):
    '''
    Save the call-graph data in SVG format
    Need plugin
    1. callgraph
    2. cycle
    '''
    try:
        import pygraphviz as pgv
    except ImportError:
        print("pygraphviz module is not installed")
        return

    G = pgv.AGraph(strict=False, directed=True)

    cycle_node_list = prof.plugins['cycle'].get_keys()
    for cycle_node in cycle_node_list:
        G.add_node(cycle_node[0])
        n = G.get_node(cycle_node[0])
        n.attr['label'] = '{<f0> %s\nPE: %d,TC: %d|<f1> Cycle: %d\n\n}' % (
            cycle_node[0], cycle_node[1], cycle_node[2], prof.plugins['cycle'].get_value(cycle_node))
        n.attr['shape'] = 'record'

    callgraph_list = prof.plugins['callgraph'].get_keys()
    for callgraph_edge in callgraph_list:
        G.add_edge(callgraph_edge[1], callgraph_edge[0], color='blue')
        # print('Callgraph:', callgraph_edge[1], '->', callgraph_edge[0])
        e = G.get_edge(callgraph_edge[1], callgraph_edge[0])
        e.attr['label'] = 'count: %d' % prof.plugins[
            'callgraph'].get_value(callgraph_edge)

    G.write('%s.dot' % benchmark_name)
    G.layout()
    G.draw('%s.svg' % benchmark_name, prog='dot')
