# -*- coding: utf-8 -*-
# 毎回sim.run()と入力するのは面倒なので、run()を実行された場合に、
# sim.run()が呼び出されるように設定


def setreg(name, data, *contextid):
    return sim.setreg(name, data, *contextid)


def getreg(name, *contextid):
    return sim.getreg(name, *contextid)


def showreg():
    return sim.showreg()


def set_event(addr, eventname, *params):
    return sim.set_event(addr, eventname, *params)


def set_clear_event(addr, eventname, *contextid):
    return sim.set_clear_event(addr, eventname, *contextid)


def gdb_connect(port):
    return sim.gdb_connect(port)


def uart_connect(port, channel):
    return sim.uart_connect(port, channel)


def ba(start_addr, *args):
    return sim.ba(start_addr, *args)


def bad(*contextid):
    return sim.bad(*contextid)


def bac(start_addr, *args):
    return sim.bac(start_addr, *args)


def bp(addr, *contextid):
    return sim.bp(addr, *contextid)


def bpd(*contextid):
    return sim.bpd(*contextid)


def bpc(addr, *contextid):
    return sim.bpc(addr, *contextid)


def ml(name, offset=CFDEF_LOAD_OFFSET):
    return sim.ml(name, offset)


def run(count=CFDEF_DEF_STEP):
    return sim.run(count)


def max(count):
    return sim.max(count)


def getmax():
    return sim.getmax()


def max_same_pc(count):
    return sim.max_same_pc(count)


def getmax_same_pc():
    return sim.getmax_same_pc()


def bt(count):
    return sim.bt(count)


def btd():
    return sim.btd()


def mi():
    return sim.mi()


def md(start_addr, *args):
    return sim.md(start_addr, *args)


def mr(addr, *args):
    return sim.mr(addr, *args)


def me(addr, data, *args):
    return sim.me(addr, data, *args)


def ms(start_addr, end_addr, *args):
    return sim.ms(start_addr, end_addr, *args)


def ms_pe(start_addr, end_addr, access_attr, pe_fetch_latency, pe_read_latency, pe_write_latency, cl_fetch_latency, cl_read_latency, cl_write_latency, exocl_fetch_latency, exocl_read_latency, exocl_write_latency, bind_pe, outstand, repeat, *param):
    return sim.ms_pe(start_addr, end_addr, access_attr, pe_fetch_latency, pe_read_latency, pe_write_latency, cl_fetch_latency, cl_read_latency, cl_write_latency, exocl_fetch_latency, exocl_read_latency, exocl_write_latency, bind_pe, outstand, repeat, *param)


def ms_cl(start_addr, end_addr, access_attr, cl_fetch_latency, cl_read_latency, cl_write_latency, exocl_fetch_latency, exocl_read_latency, exocl_write_latency, bind_cl, outstand, repeat, *param):
    return sim.ms_cl(start_addr, end_addr, access_attr, cl_fetch_latency, cl_read_latency, cl_write_latency, exocl_fetch_latency, exocl_read_latency, exocl_write_latency, bind_cl, outstand, repeat, *param)


def mc(start_addr):
    return sim.mc(start_addr)


def self_set(start_addr, end_addr, offset):
    return sim.self_set(start_addr, end_addr, offset)


def self_clear(start_addr):
    return sim.self_clear(start_addr)


def mirror_set (start_addr, end_addr, offset):
    return sim.mirror_set (start_addr, end_addr, offset)


def mirror_clear (start_addr):
    return sim.mirror_clear (start_addr)


def error_area_set(start_addr, end_addr):
    return sim.error_area_set(start_addr, end_addr)


def error_area_clear(start_addr):
    return sim.error_area_clear(start_addr)


def pe_latency(latency):
    return sim.pe_latency(latency)


def get_pe_latency():
    return sim.get_pe_latency()


def lram_fetch_latency(latency):
    return sim.lram_fetch_latency(latency)


def get_lram_fetch_latency():
    return sim.get_lram_fetch_latency()


def rom_fetch_latency(latency):
    return sim.rom_fetch_latency(latency)


def get_rom_fetch_latency():
    return sim.get_rom_fetch_latency()


def quit():
    sim.quit()


def reset(*contextid):
    return sim.reset(*contextid)


def step(*args):
    return sim.step(*args)


def set_stepmode():
    return sim.set_stepmode()


def get_stepmode():
    return sim.get_stepmode()


def help():
    sim.help()


def pic(name=CFDEF_DEF_OUT, *contextid):
    return sim.pic(name, *contextid)


def picc(*contextid):
    return sim.picc(*contextid)


def debug(mode, *contextid):
    return sim.debug(mode, *contextid)


def exp_message(mode, start_cause, end_cause, *contextid):
    return sim.exp_message(mode, start_cause, end_cause, *contextid)


def pe(data):
    return sim.pe(data)


def vc(data):
    return sim.vc(data)


def tc(data):
    return sim.tc(data)


def peinfo():
    return sim.peinfo()


def last_tcid(select_pe):
    return sim.last_tcid(select_pe)


def last_vcid(select_pe):
    return sim.last_vcid(select_pe)


def last_peid():
    return sim.last_peid()


def setclk(ratio, *contextid):
    return sim.setclk(ratio, *contextid)


def getclk(*contextid):
    return sim.getclk(*contextid)


def setlog(fname, *multi_attr):
    return sim.setlog(fname, *multi_attr)


def set_snooze(period, *contextid):
    return sim.set_snooze(period, *contextid)


def get_snooze(*contextid):
    return sim.get_snooze(*contextid)


def set_tlbnum(data, *contextid):
    return sim.set_tlbnum(data, *contextid)


def get_tlbnum(*contextid):
    return sim.get_tlbnum(*contextid)


def set_mintlb(data, *contextid):
    return sim.set_mintlb(data, *contextid)


def get_mintlb(*contextid):
    return sim.get_mintlb(*contextid)


def set_mpm(data, *contextid):
    return sim.set_mpm(data, *contextid)


def get_mpm(*contextid):
    return sim.get_mpm(*contextid)


def set_mpunum(data, *contextid):
    return sim.set_mpunum(data, *contextid)


def get_mpunum(*contextid):
    return sim.get_mpunum(*contextid)


def set_mpubank(data, *contextid):
    return sim.set_mpubank(data, *contextid)


def get_mpubank(*contextid):
    return sim.get_mpubank(*contextid)


def set_peid(data, *contextid):
    return sim.set_peid(data, *contextid)


def get_peid(*contextid):
    return sim.get_peid(*contextid)


def set_spid(data, *contextid):
    return sim.set_spid(data, *contextid)


def get_spid(*contextid):
    return sim.get_spid(*contextid)


def set_spidlist(data, *contextid):
    return sim.set_spidlist(data, *contextid)


def get_spidlist(*contextid):
    return sim.get_spidlist(*contextid)


def set_bmid(data, *contextid):
    return sim.set_bmid(data, *contextid)


def get_bmid(*contextid):
    return sim.get_bmid(*contextid)


def set_rbase(data, *contextid):
    return sim.set_rbase(data, *contextid)


def get_rbase(*contextid):
    return sim.get_rbase(*contextid)


def set_mctl(data, *contextid):
    return sim.set_mctl(data, *contextid)


def get_mctl(*contextid):
    return sim.get_mctl(*contextid)


def set_mpudmdp(data, *contextid):
    return sim.set_mpudmdp(data, *contextid)


def get_mpudmdp(*contextid):
    return sim.get_mpudmdp(*contextid)


def set_lscfg(data, *contextid):
    return sim.set_lscfg(data, *contextid)


def set_icctrl(data, *contextid):
    return sim.set_icctrl(data, *contextid)


def get_icctrl(*contextid):
    return sim.get_icctrl(*contextid)


def set_icsize(size, way, *contextid):
    return sim.set_icsize(size, way, *contextid)


def get_icsize(*contextid):
    return sim.get_icsize(*contextid)


def set_dcctrl(data, *contextid):
    return sim.set_dcctrl(data, *contextid)


def get_dcctrl(*contextid):
    return sim.get_dcctrl(*contextid)


def set_dcsize(size, way, *contextid):
    return sim.set_dcsize(size, way, *contextid)


def get_dcsize(*contextid):
    return sim.get_dcsize(*contextid)


def set_peinfo(peid, *coretype_arg):
    return sim.set_peinfo(peid, *coretype_arg)


def set_area_rom(start_addr, end_addr):
    return sim.set_area_rom(start_addr, end_addr)


def set_area_lram(start_addr, end_addr):
    return sim.set_area_lram(start_addr, end_addr)


def set_intc1(temp_peid, start_addr):
    sim.set_intc1(temp_peid, start_addr)


def set_intc2(start_addr):
    sim.set_intc2(start_addr)

def set_req_eiint_to_intc1(temp_peid, channel):
    sim.set_req_eiint_to_intc1(temp_peid, channel)

def set_req_feint_to_intc1(temp_peid, channel):
    sim.set_req_feint_to_intc1(temp_peid, channel)

def set_ostm(bind_pe, ostmch, start_addr, intch):
    return sim.set_ostm(bind_pe, ostmch, start_addr, intch)


def set_ostm_global(start_addr, intch):
    return sim.set_ostm_global(start_addr, intch)


def set_ostm_clk(ratio):
    return sim.set_ostm_clk(ratio)


def get_ostm_clk():
    return sim.get_ostm_clk()


def set_area_debug(start_addr, end_addr, pe):
    return sim.set_area_debug(start_addr, end_addr, pe)


def clear_area_debug(start_addr):
    return sim.clear_area_debug(start_addr)


def set_peripheral_cycle(start_addr, end_addr, read_lat, write_lat, pe_latency, bind_pe, outstand, repeat_rate):
    return sim.set_peripheral_cycle(start_addr, end_addr, read_lat, write_lat, pe_latency, bind_pe, outstand, repeat_rate)


def is_valid_peid(temp_peid):
    return sim.is_valid_peid(temp_peid)


def set_debug_option(option, mode):
    return sim.set_debug_option(option, mode)


def get_debug_option():
    return sim.get_debug_option()


def get_arch_class():
    return sim.get_arch_class()


def set_msglvl(lvl):
    return sim.set_msglvl(lvl)


def get_msglvl():
    return sim.get_msglvl()


def use_stopio(enable):
    return sim.use_stopio(enable)


def set_fpu_existence(enable, *contextid):
    return sim.set_fpu_existence(enable, *contextid)


def get_fpu_existence(*contextid):
    return sim.get_fpu_existence(*contextid)


def set_simd_existence(enable, *contextid):
    return sim.set_simd_existence(enable, *contextid)


def get_simd_existence(*contextid):
    return sim.get_simd_existence(*contextid)


def set_fxu_existence(enable, *contextid):
    return sim.set_fxu_existence(enable, *contextid)


def get_fxu_existence(*contextid):
    return sim.get_fxu_existence(*contextid)


def set_mpu_existence(enable, *contextid):
    return sim.set_mpu_existence(enable, *contextid)


def get_mpu_existence(*contextid):
    return sim.get_mpu_existence(*contextid)


def set_mmu_existence(enable, *contextid):
    return sim.set_mmu_existence(enable, *contextid)


def get_mmu_existence(*contextid):
    return sim.get_mmu_existence(*contextid)


def set_has_hv(has):
    return sim.set_has_hv(has)


def set_hvcfg(data, *contextid):
    return sim.set_hvcfg(data, *contextid)


def get_hvcfg(*contextid):
    return sim.get_hvcfg(*contextid)


def set_dbgen(data, *contextid):
    return sim.set_dbgen(data, *contextid)


def get_dbgen(*contextid):
    return sim.get_dbgen(*contextid)


def set_memini(value):
    return sim.set_memini(value)


def get_memini():
    return sim.get_memini()


def set_area_dc(start_addr1, end_addr1, start_addr2, end_addr2):
    return sim.set_area_dc(start_addr1, end_addr1, start_addr2, end_addr2)


def get_area_dc():
    return sim.get_area_dc()


def dump_btb(*contextid):
    return sim.dump_btb(*contextid)


def reset_btb(*contextid):
    return sim.reset_btb(*contextid)


def show_stat(*contextid):
    return sim.show_stat(*contextid)


def reset_stat(*contextid):
    return sim.reset_stat(*contextid)


def exec_cfali(start_addr, end_addr, *contextid):
    return sim.exec_cfali(start_addr, end_addr, *contextid)


def set_has_simd(has):
    return sim.set_has_simd(has)


def set_has_fxu(has):
    return sim.set_has_fxu(has)


def set_has_fpu(has):
    return sim.set_has_fpu(has)


def set_has_mpu(has):
    return sim.set_has_mpu(has)


def set_has_mmu(has):
    return sim.set_has_mmu(has)

def set_l1rlinknum(num, *contextid):
    return sim.set_l1rlinknum(num, *contextid)

def get_l1rlinknum(*contextid):
    return sim.get_l1rlinknum(*contextid)

def set_l1rsize(num, *contextid):
    return sim.set_l1rsize(num, *contextid)

def get_l1rsize(*contextid):
    return sim.get_l1rsize(*contextid)

def set_peripherals(*peri_str):
    return sim.set_peripherals(*peri_str)

def set_ipir_eiint_ch(offset_intch):
    return sim.set_ipir_eiint_ch(offset_intch)

def set_guards(name, *param_str):
    return sim.set_guards(name, *param_str)

def set_guards_param(name, id, param_name, value):
    return sim.set_guards_param(name, id, param_name, value)

# alias定義
s = step
rs = reset
q = quit
load = ml
