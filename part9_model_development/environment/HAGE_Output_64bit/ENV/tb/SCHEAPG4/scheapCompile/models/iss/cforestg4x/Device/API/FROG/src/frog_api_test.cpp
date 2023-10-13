#include <stdlib.h>
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "./config.h"
#include "./forest_common.h"
#include "./frog_internals.h"
#include "./frog_if.h"

void cb_update_generator_memory(uint64_t address, uint32_t size, uint64_t value, uint64_t prev_value,
        const uint32_t access_type, const uint32_t access_bus, const uint32_t load_action) {
    std::string str_access_type, str_access_bus, str_load_action;
    if (access_type == FROG_ACCESS_TYPE_READ) {
        str_access_type = "read";
    } else if (access_type == FROG_ACCESS_TYPE_WRITE) {
        str_access_type = "write";
    }
    if (access_type == FROG_ACCESS_BUS_RW) {
        str_access_bus = "RW";
    } else if (access_type == FROG_ACCESS_BUS_FETCH) {
        str_access_bus = "Fetch";
    }else if (access_type == FROG_ACCESS_BUS_FETCH) {
        str_access_bus = "API";
    }
    if (access_type == FROG_LOAD_ACTION_LOAD) {
        str_access_type = "Load";
    } else if (access_type == FROG_LOAD_ACTION_FETCH) {
        str_access_type = "Fetch";
    }

    printf ("<< Callback memory: address %lx, size %d, value %lx, prev_value %lx, access_type %s, access_bus %s, load_action %s >> \n",
                 address, size, value, prev_value, str_access_type.c_str(), str_access_bus.c_str(), str_access_type.c_str());
}


void cb_update_nc_register(const std::string register_name, FrogRegData value, FrogRegData prev_value) {
    if (register_name.compare (0, 2, "WR") == 0) {
        unsigned long long value_u64 = (value >> 64) & 0xFFFFFFFFFFFFFFFFULL;
        unsigned long long value_l64 = value & 0xFFFFFFFFFFFFFFFFULL;
        unsigned long long prev_value_u64 = (prev_value >> 64) & 0xFFFFFFFFFFFFFFFFULL;
        unsigned long long prev_value_l64 = prev_value & 0xFFFFFFFFFFFFFFFFULL;
        printf ("<< Callback NC register: register_name %s, value %016llx%016llx, prev_value %016llx%016llx >>\n",
                 register_name.c_str(), value_u64, value_l64, prev_value_u64, prev_value_l64);
    } else {
        uint64_t value_temp = value;
        uint64_t prev_value_temp = prev_value;
        printf ("<< Callback NC register: register_name %s, value %lx, prev_value %lx >>\n",
                 register_name.c_str(), value_temp, prev_value_temp);
    }
}

void cb_update_vc_register(const std::string register_name, FrogRegData value, FrogRegData prev_value, uint32_t vcid) {
    uint64_t value_temp = value;
    uint64_t prev_value_temp = prev_value;
    printf ("<< Callback VC register: register_name %s, value %lx, prev_value %lx, vcid %x >>\n",
                register_name.c_str(), value_temp, prev_value_temp, vcid);
}

void cb_update_tc_register(const std::string register_name, FrogRegData value, FrogRegData prev_value, uint32_t tcid) {
    if (register_name.compare (0, 2, "WR") == 0) {
        unsigned long long value_u64 = (value >> 64) & 0xFFFFFFFFFFFFFFFFULL;
        unsigned long long value_l64 = value & 0xFFFFFFFFFFFFFFFFULL;
        unsigned long long prev_value_u64 = (prev_value >> 64) & 0xFFFFFFFFFFFFFFFFULL;
        unsigned long long prev_value_l64 = prev_value & 0xFFFFFFFFFFFFFFFFULL;
        printf ("<< Callback NC register: register_name %s, value %016llx%016llx, prev_value %016llx%016llx >>\n",
                 register_name.c_str(), value_u64, value_l64, prev_value_u64, prev_value_l64);
    } else {
        uint64_t value_temp = value;
        uint64_t prev_value_temp = prev_value;
        printf ("<< Callback TC register: register_name %s, value %lx, prev_value %lx, tcid %x >>\n",
                register_name.c_str(), value_temp, prev_value_temp, tcid);
    }
}

void cb_update_llbit (uint32_t peid, uint64_t address, uint32_t size, bool isNC, uint32_t ptid, const uint32_t status) {
    std::string str_status;
    if (status == FROG_LLBIT_CREATE) {
        str_status = "create";
    } else if (status == FROG_LLBIT_DELETE) {
        str_status = "delete";
    }
    printf ("<< Callback LLBit update: peid %x, address %lx, size=%x, isNC %x, ptid %x, status %s >>\n",
                peid, address, size, isNC, ptid, str_status.c_str());
}

void cb_update_tlb (uint32_t index, uint32_t telo0, uint32_t telo1, uint32_t tehi0, uint32_t tehi1
                                 , uint32_t pre_telo0, uint32_t pre_telo1, uint32_t pre_tehi0, uint32_t pre_tehi1, const uint32_t type) {
    std::string str_type;
    if (type == FROG_TLB_READ_ENTRY) {
        str_type = "read";
    } else if (type == FROG_TLB_WRITE_ENTRY) {
        str_type = "write";
    }
    printf ("<< Callback TLB update: index %x, telo0 %x, telo1 %x, tehi0 %x, tehi1 %x, pre_telo0 %x, pre_telo1 %x, pre_tehi0 %x, pre_tehi1 %x, access type %s >>\n",
              index, telo0, telo1, tehi0, tehi1, pre_telo0, pre_telo1, pre_tehi0, pre_tehi1, str_type.c_str());
}

void cb_comment (const std::string &comment) {
    printf ("%s\n", comment.c_str());
}

bool init_frog_if_test() {
    GCS_CB_INFO_EX      pCbInfo;
    GCS_HW_INFO_EX *    pHwInfo = new GCS_HW_INFO_EX();
    pCbInfo.fpExM    = cb_update_generator_memory;
    pCbInfo.fpExNR   = cb_update_nc_register;
    pCbInfo.fpExTR   = cb_update_tc_register;
    pCbInfo.fpExVR   = cb_update_vc_register;
    pCbInfo.fpL      = cb_update_llbit;
    pCbInfo.fpExTLB  = cb_update_tlb;
    pCbInfo.fpC      = cb_comment;
    std::string        frog_if_version;
    int err = gcs_initialize_simulator_ex(MSG_INF, &pCbInfo, "./frog_if_env/init_g3mh_1pe.py", pHwInfo);
    if (err != GCSIF_SUCCESS) {
        printf ("Initialize error \n");
        return false;
    }
    gcs_get_simulator_version(frog_if_version);
    printf ("FROG I/F version: %s\n", frog_if_version.c_str());
    return true;
}

bool NM_2_VM_internal () {
    gcs_set_nc_register("pc", 0x1056ull);
    //    2314:   e0 1f 20 08     ldsr    r0, sr3, 1
    //    2318:   e0 07 20 00     ldsr    r0, sr0, 0
    //    231c:   e0 17 20 00     ldsr    r0, sr2, 0
    //    2320:   e0 e7 20 00     ldsr    r0, sr28, 0
    //    2324:   e0 ef 20 00     ldsr    r0, sr29, 0
    //    2328:   00 a2           mov 0, r20
    //    232a:   f4 7f 20 08     ldsr    r20, sr15, 1
    //    232e:   e0 17 20 08     ldsr    r0, sr2, 1
    //    2332:   00 a2           mov 0, r20
    //    2334:   f4 57 20 08     ldsr    r20, sr10, 1
    //    2338:   e0 17 30 00     ldtc.sr r0, sr2, 0
    //    233c:   e0 07 30 00     ldtc.sr r0, sr0, 0
    //    2340:   e0 e7 30 00     ldtc.sr r0, sr28, 0
    //    2344:   e0 ef 30 00     ldtc.sr r0, sr29, 0
    //    2348:   34 06 00 01     mov 0x100, r20
        //    234c:   00 00
    gcs_write_simulator_memory(0x1056, 4, "0xe0,0x1f,0x20,0x08");
    gcs_write_simulator_memory(0x105a, 4, "0xe0,0x07,0x20,0x00");
    gcs_write_simulator_memory(0x105e, 4, "0xe0,0x17,0x20,0x00");
    gcs_write_simulator_memory(0x1062, 4, "0xe0,0xe7,0x20,0x00");
    gcs_write_simulator_memory(0x1066, 4, "0xe0,0xef,0x20,0x00");
    gcs_write_simulator_memory(0x106a, 2, "0x00,0xa2");
    gcs_write_simulator_memory(0x106c, 4, "0xf4,0x7f,0x20,0x08");
    gcs_write_simulator_memory(0x1070, 4, "0xe0,0x17,0x20,0x08");
    gcs_write_simulator_memory(0x1074, 2, "0x00,0xa2");
    gcs_write_simulator_memory(0x1076, 4, "0xf4,0x57,0x20,0x08");
    gcs_write_simulator_memory(0x107a, 4, "0xe0,0x17,0x30,0x00");
    gcs_write_simulator_memory(0x107e, 4, "0xe0,0x07,0x30,0x00");
    gcs_write_simulator_memory(0x1082, 4, "0xe0,0xe7,0x30,0x00");
    gcs_write_simulator_memory(0x1086, 6, "0x34,0x06,0x00,0x01,0x00,0x00");
    //    234e:   f4 87 20 08     ldsr    r20, sr16, 1
    //    2352:   00 a2           mov 0, r20
    //    2354:   f4 7f 20 08     ldsr    r20, sr15, 1
    //    2358:   e5 a7 54 08     stvc.sr sr5, r20, 1
    //    235c:   35 06 00 00     mov 0x80000000, r21
    //    2360:   00 80
    //    2362:   15 a1           or  r21, r20
    gcs_write_simulator_memory(0x108c, 4, "0xf4,0x87,0x20,0x08");
    gcs_write_simulator_memory(0x1090, 2, "0x00,0xa2");
    gcs_write_simulator_memory(0x1092, 4, "0xf4,0x7f,0x20,0x08");
    gcs_write_simulator_memory(0x1096, 4, "0xe5,0xa7,0x54,0x08");
    gcs_write_simulator_memory(0x109a, 6, "0x35,0x06,0x00,0x00,0x00,0x80");
    gcs_write_simulator_memory(0x10a0, 2, "0x15,0xa1");
    //    2364:   f4 2f 34 08     ldvc.sr r20, sr5
    //    2368:   00 a2           mov 0, r20
    //    236a:   f4 57 20 08     ldsr    r20, sr10, 1
    //    236e:   e5 a7 50 10     sttc.sr sr5, r20, 2
    //    2372:   35 06 00 00     mov 0x80000000, r21
    //    2376:   00 80
    //    2378:   15 a1           or  r21, r20
    gcs_write_simulator_memory(0x10a2, 4, "0xf4,0x2f,0x34,0x08");
    gcs_write_simulator_memory(0x10a6, 2, "0x00,0xa2");
    gcs_write_simulator_memory(0x10a8, 4, "0xf4,0x57,0x20,0x08");
    gcs_write_simulator_memory(0x10ac, 4, "0xe5,0xa7,0x50,0x10");
    gcs_write_simulator_memory(0x10b0, 6, "0x35,0x06,0x00,0x00,0x00,0x80");
    gcs_write_simulator_memory(0x10b6, 2, "0x15,0xa1");
    //    237a:   f4 2f 30 10     ldtc.sr r20, sr5, 2
    //    237e:   e0 a7 54 08     stvc.sr sr0, r20, 1
    //    2382:   04 aa           mov 4, r21
    //    2384:   15 a1           or  r21, r20
    //    2386:   f4 07 34 08     ldvc.sr r20, sr0
    //    238a:   e0 a7 40 10     stsr    sr0, r20, 2
    //    238e:   34 06 ac 23     mov 0x23ac, r20
    //    2392:   00 00
    gcs_write_simulator_memory(0x10b8, 4, "0xf4,0x2f,0x30,0x10");
    gcs_write_simulator_memory(0x10bc, 4, "0xe0,0xa7,0x54,0x08");
    gcs_write_simulator_memory(0x10c0, 2, "0x04,0xaa");
    gcs_write_simulator_memory(0x10c2, 2, "0x15,0xa1");
    gcs_write_simulator_memory(0x10c4, 4, "0xf4,0x07,0x34,0x08");
    gcs_write_simulator_memory(0x10c8, 4, "0xe0,0xa7,0x40,0x10");
    gcs_write_simulator_memory(0x10cc, 6, "0x34,0x06,0xac,0x23,0x00,0x00");
    //    2394:   f4 07 32 f8     ldtc.pc r20
    //    2398:   e5 a7 40 00     stsr    sr5, r20, 0
    //    239c:   35 06 00 00     mov 0x80000000, r21
    //    23a0:   00 80
    //    23a2:   14 a9           or  r20, r21
    //    23a4:   f5 1f 20 00     ldsr    r21, sr3, 0
    //    23a8:   e0 07 4a 01     feret
    //    23ac:   00 00
    gcs_write_simulator_memory(0x10d2, 4, "0xf4,0x07,0x32,0xf8");
    gcs_write_simulator_memory(0x10d6, 4, "0xe5,0xa7,0x40,0x00");
    gcs_write_simulator_memory(0x10da, 6, "0x35,0x06,0x00,0x00,0x00,0x80");
    gcs_write_simulator_memory(0x10e0, 2, "0x14,0xa9");
    gcs_write_simulator_memory(0x10e2, 4, "0xf5,0x1f,0x20,0x00");
    gcs_write_simulator_memory(0x10e6, 4, "0xe0,0x07,0x4a,0x01");
    printf("----------------------------------------\n");

    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);

    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);

    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);

    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);

    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);

    return true;
}

bool test_sr_vm (){
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }
    NM_2_VM_internal();
    //    PTID=00[V] | PC=000023ac | ICODE32 =     283057e8  | PSW[7:0]=20 | T0:MCC     = 00000000 | P1:V0:T0:H:   ldtc.sr r8,sr10,sel5    | r8=>00000000 T0:MCC<=00000000 MCR<=00000000
    //    PTID=00[V] | PC=000023ac | ICODE32 =     283057e8  | PSW[7:0]=20 | MCR        = 00000000 | P1:V0:T0:H:   ldtc.sr r8,sr10,sel5    | r8=>00000000 T0:MCC<=00000000 MCR<=00000000
    gcs_write_simulator_memory(0x23ac, 4, "0xE8,0x57,0x30,0x28");             // 106c: e8 57 30 28          ldtc.sr r8, sr10, sel5
    gcs_step_simulator(0);
    //    PTID=00[V] | PC=000023b0 | ICODE32 =     08201fe0  | PSW[7:0]=20 | EBASE      = 00000000 | P1:V0:T0:H:   ldsr r0,sr3,sel1        | r0=>00000000 EBASE<=00000000
    gcs_write_simulator_memory(0x23b0, 4, "0xe0,0x1f,0x20,0x08");
    gcs_step_simulator(0);
    //    PTID=00[V] | PC=000023b4 | ICODE32 =     00202fe2  | PSW[7:0]=00 | PSW        = 00000000 | P1:V0:T0:H:   ldsr r2,sr5,sel0        | r2=>00000000 PSW<=00000000
    gcs_write_simulator_memory(0x23b4, 4, "0xe2,0x2f,0x20,0x00");
    gcs_step_simulator(0);
    //    PTID=00[V] | PC=000023b8 | ICODE48 = 800000000634  | PSW[7:0]=00 | r20        = 80000000 | P1:V0:T0:H:   movw -2147483648,r20    | r20<=80000000
    //    PTID=00[V] | PC=000023be | ICODE32 =     082057f4  | PSW[7:0]=00 | TCSEL      = 80000000 | P1:V0:T0:H:   ldsr r20,sr10,sel1      | r20=>80000000 TCSEL<=80000000
    gcs_write_simulator_memory(0x23b8, 6, "0x34,0x06,0x00,0x00,0x00,0x80");
    gcs_write_simulator_memory(0x23be, 4, "0xf4,0x57,0x20,0x08");
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    //    PTID=00[V] | PC=000023c2 | ICODE32 =     00202fe2  | PSW[7:0]=00 | PSW        = 00000000 | P1:V0:T0:H:   ldsr r2,sr5,sel0        | r2=>00000000 PSW<=00000000
    gcs_write_simulator_memory(0x23c2, 4, "0xe2,0x2f,0x20,0x00");
    gcs_step_simulator(0);
    //    PTID=00[V] | PC=000023c6 | ICODE48 = 005555550634  | PSW[7:0]=00 | r20        = 00555555 | P1:V0:T0:H:   movw 5592405,r20        | r20<=00555555
    //    PTID=00[V] | PC=000023cc | ICODE32 =     00302ff4  | PSW[7:0]=00 | NC:PSW     = 00555555 | P1:V0:T0:H:   ldtc.sr r20,sr5,sel0    | r20=>00555555 NC:PSW<=00555555
    gcs_write_simulator_memory(0x23c6, 6, "0x34,0x06,0x55,0x55,0x55,0x00");
    gcs_write_simulator_memory(0x23cc, 4, "0xf4,0x2f,0x30,0x00");
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    //    PTID=00[V] | PC=000023d0 | ICODE48 = aaaaaaaa0634  | PSW[7:0]=00 | r20        = aaaaaaaa | P1:V0:T0:H:   movw -1431655766,r20    | r20<=aaaaaaaa
    //    PTID=00[V] | PC=000023d6 | ICODE32 =     00302ff4  | PSW[7:0]=00 | NC:PSW     = aaaaaaaa | P1:V0:T0:H:   ldtc.sr r20,sr5,sel0    | r20=>aaaaaaaa NC:PSW<=aaaaaaaa
    gcs_write_simulator_memory(0x23d0, 6, "0x34,0x06,0xAA,0xAA,0xAA,0xAA");
    gcs_write_simulator_memory(0x23d6, 4, "0xf4,0x2f,0x30,0x00");
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    //    PTID=00[V] | PC=000023da | ICODE16 =         a200  | PSW[7:0]=00 | r20        = 00000000 | P1:V0:T0:H:   mov5 0,r20              | r20<=00000000
    //    PTID=00[V] | PC=000023dc | ICODE32 =     082057f4  | PSW[7:0]=00 | TCSEL      = 00000000 | P1:V0:T0:H:   ldsr r20,sr10,sel1      | r20=>00000000 TCSEL<=00000000
    gcs_write_simulator_memory(0x23da, 2, "0x00,0xa2");
    gcs_write_simulator_memory(0x23dc, 4, "0xf4,0x57,0x20,0x08");
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    //    PTID=00[V] | PC=000023e0 | ICODE32 =     00202fe2  | PSW[7:0]=00 | PSW        = 00000000 | P1:V0:T0:H:   ldsr r2,sr5,sel0        | r2=>00000000 PSW<=00000000
    gcs_write_simulator_memory(0x23e0, 4, "0xe2,0x2f,0x20,0x00");
    gcs_step_simulator(0);
    //    PTID=00[V] | PC=000023e4 | ICODE48 = 005555550634  | PSW[7:0]=00 | r20        = 00555555 | P1:V0:T0:H:   movw 5592405,r20        | r20<=00555555
    //    PTID=00[V] | PC=000023ea | ICODE32 =     00302ff4  | PSW[7:0]=55 | T0:PSW     = 00555555 | P1:V0:T0:H:   ldtc.sr r20,sr5,sel0    | r20=>00555555 T0:PSW<=00555555
    gcs_write_simulator_memory(0x23e4, 6, "0x34,0x06,0x55,0x55,0x55,0x00");
    gcs_write_simulator_memory(0x23ea, 4, "0xf4,0x2f,0x30,0x00");
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    //    PTID=00[V] | PC=000023ee | ICODE48 = aaaaaaaa0634  | PSW[7:0]=55 | r20        = aaaaaaaa | P1:V0:T0:H:   movw -1431655766,r20    | r20<=aaaaaaaa
    //    PTID=00[V] | PC=000023f4 | ICODE32 =     00302ff4  | PSW[7:0]=aa | T0:PSW     = aaaaaaaa | P1:V0:T0:H:   ldtc.sr r20,sr5,sel0    | r20=>aaaaaaaa T0:PSW<=aaaaaaaa
    gcs_write_simulator_memory(0x23ee, 6, "0x34,0x06,0xAA,0xAA,0xAA,0xAA");
    gcs_write_simulator_memory(0x23f4, 4, "0xf4,0x2f,0x30,0x00");
    gcs_step_simulator(0);
    gcs_step_simulator(0);
//    gcs_step_simulator(0);
    printf("----------------------------------------\n");
    gcs_disable_simulator_stop(0);
    return true;
}

bool test_gcs_get_register_profile() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }
    std::vector <int32_t> vector_fail_testcase;
    int32_t ret_val = 0;
    FrogRegData get_value = 0x00;
    std::string reg_name;
    uint32_t context_TC = TC0, context_VC = VC0;
//    uint32_t context_NC = NC;
    printf("----------------------------------------\n");
    const std::string key_name = "PSW";
    FrogRegPro get_profile;
    ret_val = gcs_get_register_profile(key_name, &get_profile);
    if (ret_val != true) {
        printf ("Error %x: gcs_get_register_profile() for PSW \n", ret_val);
    } else {
        printf ("Value of PSW: selid %x regid %x \n", get_profile.m_sel_id, get_profile.m_reg_id);
    }
    printf("----------------------------------------\n");
    // Unit test for gcs_get_sreg_name().
    gcs_get_sreg_name(16, 0, reg_name);
    printf("reg_name: %s\n", reg_name.c_str());
    gcs_get_sreg_name(0, 0, reg_name);
    printf("reg_name: %s\n", reg_name.c_str());
    // Unit test for gcs_set_nc_register() and gcs_get_nc_register()
    ret_val = gcs_set_nc_register("PSW", 0x0000cafe);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_nc_register() \n", ret_val);
    }
    ret_val = gcs_get_nc_register("PSW", &get_value);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_nc_register() \n", ret_val);
    } else {
        printf ("Value of PSW: %lx \n", (uint64_t) get_value);
    }
    ret_val = gcs_set_nc_register("PSW", 0xcafecafe);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_nc_register() \n", ret_val);
    }
    ret_val = gcs_get_nc_register("PSW", &get_value);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_nc_register() \n", ret_val);
    } else {
        printf ("Value of PSW: %lx \n", (uint64_t) get_value);
    }
    printf("----------------------------------------\n");
    ret_val = gcs_set_nc_register("FPSR", 0x0000cafe);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_nc_register() \n", ret_val);
    }
    ret_val = gcs_get_nc_register("FPSR", &get_value);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_nc_register() \n", ret_val);
    } else {
        printf ("Value of FPSR: %lx \n", (uint64_t) get_value);
    }
    ret_val = gcs_set_nc_register("FPSR", 0xcafecafe);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_nc_register() \n", ret_val);
    }
    ret_val = gcs_get_nc_register("FPSR", &get_value);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_nc_register() \n", ret_val);
    } else {
        printf ("Value of FPSR: %lx \n", (uint64_t) get_value);
    }
    ret_val = gcs_set_nc_register("BPC", 0x0000cafe);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_nc_register() \n", ret_val);
    }
    ret_val = gcs_get_nc_register("BPC", &get_value);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_nc_register() \n", ret_val);
    } else {
        printf ("Value of BPC: %lx \n", (uint64_t) get_value);
    }
    ret_val = gcs_set_nc_register("BPC", 0xcafecafe);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_nc_register() \n", ret_val);
    }
    ret_val = gcs_get_nc_register("BPC", &get_value);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_nc_register() \n", ret_val);
    } else {
        printf ("Value of BPC: %lx \n", (uint64_t) get_value);
    }
    printf("----------------------------------------\n");
    // Unit test for gcs_set_vc_register() and gcs_get_vc_register()
    ret_val = gcs_set_vc_register("TCSEL", 0x0000cafe, context_VC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_vc_register() \n", ret_val);
    }
    ret_val = gcs_get_vc_register("TCSEL", &get_value, context_VC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_vc_register() \n", ret_val);
    } else {
        printf ("Value of TCSEL: %lx \n", (uint64_t) get_value);
    }
    ret_val = gcs_set_vc_register("TCSEL", 0xcafecafe, context_VC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_vc_register() \n", ret_val);
    }
    ret_val = gcs_get_vc_register("TCSEL", &get_value, context_VC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_vc_register() \n", ret_val);
    } else {
        printf ("Value of TCSEL: %lx \n", (uint64_t) get_value);
    }
    ret_val = gcs_set_vc_register("TLBSCH", 0x0000cafe, context_VC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_vc_register() \n", ret_val);
    }
    ret_val = gcs_get_vc_register("TLBSCH", &get_value, context_VC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_vc_register() \n", ret_val);
    } else {
        printf ("Value of TLBSCH: %lx \n", (uint64_t) get_value);
    }
    ret_val = gcs_set_vc_register("TLBSCH", 0xcafecafe, context_VC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_vc_register() \n", ret_val);
    }
    ret_val = gcs_get_vc_register("TLBSCH", &get_value, context_VC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_vc_register() \n", ret_val);
    } else {
        printf ("Value of TLBSCH: %lx \n", (uint64_t) get_value);
    }
    ret_val = gcs_set_vc_register("VCSEL", 0xcafecafe, context_VC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_vc_register() \n", ret_val);
    }
    ret_val = gcs_get_vc_register("VCSEL", &get_value, context_VC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_vc_register() \n", ret_val);
    } else {
        printf ("Value of VCSEL: %lx \n", (uint64_t) get_value);
    }
    ret_val = gcs_set_vc_register("TLBSCH", 0x0000cafe, context_VC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_vc_register() \n", ret_val);
    }
    ret_val = gcs_get_vc_register("TLBSCH", &get_value, context_VC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_vc_register() \n", ret_val);
    } else {
        printf ("Value of TLBSCH: %lx \n", (uint64_t) get_value);
    }
    ret_val = gcs_set_vc_register("TLBSCH", 0xcafecafe, context_VC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_vc_register() \n", ret_val);
    }
    ret_val = gcs_get_vc_register("TLBSCH", &get_value, context_VC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_vc_register() \n", ret_val);
    } else {
        printf ("Value of TLBSCH: %lx \n", (uint64_t) get_value);
    }
    ret_val = gcs_set_nc_register("TCSEL", 0xcafecafe);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_nc_register() \n", ret_val);
    }
    ret_val = gcs_get_nc_register("TCSEL", &get_value);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_nc_register() \n", ret_val);
    } else {
        printf ("Value of TCSEL: %lx \n", (uint64_t) get_value);
    }
    printf("----------------------------------------\n");
    // Unit test for gcs_set_tc_register() and gcs_get_tc_register()
    ret_val = gcs_set_tc_register("PSW", 0x0000cafe, context_TC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_tc_register() \n", ret_val);
    }
    ret_val = gcs_get_tc_register("PSW", &get_value, context_TC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_tc_register() \n", ret_val);
    } else {
        printf ("Value of TCSEL: %lx \n", (uint64_t) get_value);
    }
    ret_val = gcs_set_tc_register("PSW", 0xcafecafe, context_TC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_tc_register() \n", ret_val);
    }
    ret_val = gcs_get_tc_register("PSW", &get_value, context_TC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_tc_register() \n", ret_val);
    } else {
        printf ("Value of PSW: %lx \n", (uint64_t) get_value);
    }
    ret_val = gcs_set_tc_register("FPSR", 0x0000cafe, context_TC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_tc_register() \n", ret_val);
    }
    ret_val = gcs_get_tc_register("FPSR", &get_value, context_TC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_tc_register() \n", ret_val);
    } else {
        printf ("Value of FPSR: %lx \n", (uint64_t) get_value);
    }
    ret_val = gcs_set_tc_register("FPSR", 0xcafecafe, context_TC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_tc_register() \n", ret_val);
    }
    ret_val = gcs_get_tc_register("FPSR", &get_value, context_TC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_tc_register() \n", ret_val);
    } else {
        printf ("Value of FPSR: %lx \n", (uint64_t) get_value);
    }
    printf("----------------------------------------\n");
    // Error case;
    printf( "Error case: \n");
    ret_val = gcs_set_nc_register("HTSCCTL", 0xcafecafe);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_nc_register() \n", ret_val);
    }
    ret_val = gcs_get_nc_register("HTSCCTL", &get_value);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_nc_register() \n", ret_val);
    } else {
        printf ("Value of HTSCCTL: %lx \n", (uint64_t) get_value);
    }
    ret_val = gcs_set_tc_register("VCSEL", 0xcafecafe, context_TC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_tc_register() \n", ret_val);
    }
    ret_val = gcs_get_tc_register("VCSEL", &get_value, context_TC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_tc_register() \n", ret_val);
    } else {
        printf ("Value of VCSEL: %lx \n", (uint64_t) get_value);
    }
    ret_val = gcs_set_tc_register("HVCBP", 0xcafecafe, context_TC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_tc_register() \n", ret_val);
    }
    ret_val = gcs_get_tc_register("HVCBP", &get_value, context_TC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_tc_register() \n", ret_val);
    } else {
        printf ("Value of HVCBP: %lx \n", (uint64_t) get_value);
    }
    ret_val = gcs_set_vc_register("PSW", 0xcafecafe, context_VC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_set_vc_register() \n", ret_val);
    }
    ret_val = gcs_get_vc_register("PSW", &get_value, context_VC);
    if (ret_val != GCSIF_SUCCESS) {
        printf ("Error %x: gcs_get_vc_register() \n", ret_val);
    } else {
        printf ("Value of PSW: %lx \n", (uint64_t) get_value);
    }
    gcs_disable_simulator_stop(0);
    return true;
}

bool test_gcs_get_vc_register() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }
    int err = 0;
    FrogRegData value = 0xafafafaf;
    err = gcs_set_vc_register("VCSEL", value, 0);
    err = gcs_get_vc_register("VCSEL", &value, 0);
    gcs_disable_simulator_stop(0);
    return true;
}

bool test_gcs_get_nc_tc_register() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }
    int err = 0;
    FrogRegData value = 0xafafafaf;
    err = gcs_set_tc_register("HTCTL", value, 0);
    err = gcs_get_tc_register("HTCTL", &value, 0);
    gcs_set_tc_register("PC", 0x1056ull, TC0);
    gcs_set_tc_register("PC", 0x1058ull, TC0);
    value = 0xffffffff;
    err = gcs_set_nc_register("FPSR", value);
    err = gcs_get_nc_register("FPSR", &value);
    printf ("value FPSR: %lx \n", (uint64_t) value);
    err = gcs_get_nc_register("FPCFG", &value);
    printf ("value FPCFG: %lx \n", (uint64_t) value);
    err = gcs_get_nc_register("FPCC", &value);
    printf ("value FPCC: %lx \n", (uint64_t) value);
    err = gcs_get_nc_register("FPST", &value);
    printf ("value FPST: %lx \n", (uint64_t) value);
    gcs_disable_simulator_stop(0);
    return true;
}


bool test_FROG_sample() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }
    int err = 0;

    FrogRegData get_value = 0x0;
    gcs_write_simulator_memory(0x0, 6, "0x25,0x06,0xc4,0xb7,0x31,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x6, 2, "0x65,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31b7c4, 6, "0x25,0x06,0x08,0x00,0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31b7ca, 2, "0x65,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x8, 6, "0x23,0x06,0x64,0xab,0x31,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0xe, 4, "0xe3,0xc7,0x60,0xf9");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31ab64, 2, "0x7f,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x12, 6, "0x23,0x06,0x00,0x02,0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x18, 2, "0x63,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x200, 6, "0x23,0x06,0x20,0x80,0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x206, 4, "0xe3,0x2f,0x20,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x20a, 6, "0x23,0x06,0x20,0xe2,0xff,0xfe");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x210, 2, "0x01,0x2a");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x212, 4, "0x43,0x2f,0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x216, 6, "0x23,0x06,0x21,0xe2,0xff,0xfe");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x21c, 2, "0x01,0x2a");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x21e, 4, "0x43,0x2f,0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x222, 6, "0x23,0x06,0x00,0x00,0x11,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x228, 4, "0xe3,0x1f,0x20,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x22c, 2, "0x63,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x110000, 6, "0x23,0x06,0xaa,0xa8,0x31,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x110006, 2, "0x63,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8aa, 6, "0x25,0x06,0xde,0xb9,0x31,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8b0, 2, "0x65,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31b9de, 6, "0x25,0x06,0xb2,0xa8,0x31,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31b9e4, 2, "0x65,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8b2, 4, "0xe5,0x57,0x40,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8b6, 6, "0x2b,0x06,0xff,0xff,0xf8,0xff");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8bc, 2, "0x4b,0x51");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8be, 6, "0x2b,0x06,0x00,0x00,0x01,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8c4, 2, "0x0b,0x51");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8c6, 4, "0xea,0x2f,0x20,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8ca, 2, "0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8cc, 2, "0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8ce, 6, "0x34,0x06,0x52,0xab,0x31,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8d4, 4, "0xf4,0xc7,0x60,0xf9");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31ab52, 2, "0x7f,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8d8, 2, "0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8da, 6, "0x34,0x06,0x46,0xac,0x31,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8e0, 4, "0xf4,0xc7,0x60,0xf9");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31ac48, 2, "0xa4,0x00");
    gcs_write_simulator_memory(0x31ac4a, 2, "0xbc,0x00");
    gcs_write_simulator_memory(0x31ac4c, 2, "0xb4,0x00");
    gcs_write_simulator_memory(0x31ac4e, 2, "0x84,0x00");
    gcs_write_simulator_memory(0x31ac50, 2, "0xc0,0x00");
    gcs_write_simulator_memory(0x31ac52, 2, "0xa4,0x00");
    gcs_write_simulator_memory(0x31ac54, 2, "0xa4,0x00");
    gcs_write_simulator_memory(0x31ac56, 2, "0x9e,0x00");
    gcs_write_simulator_memory(0x31ac58, 2, "0x98,0x00");
    gcs_write_simulator_memory(0x31ac5a, 2, "0xb2,0x00");
    gcs_write_simulator_memory(0x31ac5c, 2, "0x84,0x00");
    gcs_write_simulator_memory(0x31ac5e, 2, "0xb2,0x00");
    gcs_write_simulator_memory(0x31ac60, 2, "0xc2,0x00");
    gcs_write_simulator_memory(0x31ac62, 2, "0xb0,0x00");
    gcs_write_simulator_memory(0x31ac64, 2, "0xa2,0x00");
    gcs_write_simulator_memory(0x31ac66, 2, "0x88,0x00");
    gcs_write_simulator_memory(0x31ac68, 2, "0xac,0x00");
    gcs_write_simulator_memory(0x31ac6a, 2, "0x8c,0x00");
    gcs_write_simulator_memory(0x31ac6c, 2, "0x88,0x00");
    gcs_write_simulator_memory(0x31ac6e, 2, "0x80,0x00");
    gcs_write_simulator_memory(0x31ac70, 2, "0xb0,0x00");
    gcs_write_simulator_memory(0x31ac72, 2, "0x98,0x00");
    gcs_write_simulator_memory(0x31ac74, 2, "0xc0,0x00");
    gcs_write_simulator_memory(0x31ac76, 2, "0xb2,0x00");
    gcs_write_simulator_memory(0x31ac78, 2, "0xa2,0x00");
    gcs_write_simulator_memory(0x31ac7a, 2, "0x9e,0x00");
    gcs_write_simulator_memory(0x31ac7c, 2, "0xb2,0x00");
    gcs_write_simulator_memory(0x31ac7e, 2, "0xa4,0x00");
    gcs_write_simulator_memory(0x31ac80, 2, "0x88,0x00");
    gcs_write_simulator_memory(0x31ac82, 2, "0xc6,0x00");
    gcs_write_simulator_memory(0x31ac84, 2, "0xc0,0x00");
    gcs_write_simulator_memory(0x31ac86, 2, "0xbc,0x00");
    gcs_write_simulator_memory(0x31ac88, 2, "0x88,0x00");
    gcs_write_simulator_memory(0x31ac8a, 2, "0xb2,0x00");
    gcs_write_simulator_memory(0x31ac8c, 2, "0xb0,0x00");
    gcs_write_simulator_memory(0x31ac8e, 2, "0xa4,0x00");
    gcs_write_simulator_memory(0x31ac90, 2, "0xb4,0x00");
    gcs_write_simulator_memory(0x31ac92, 2, "0xc6,0x00");
    gcs_write_simulator_memory(0x31ac94, 2, "0x8c,0x00");
    gcs_write_simulator_memory(0x31ac96, 2, "0xa4,0x00");
    gcs_write_simulator_memory(0x31ac98, 2, "0xa2,0x00");
    gcs_write_simulator_memory(0x31ac9a, 2, "0x98,0x00");
    gcs_write_simulator_memory(0x31ac9c, 2, "0x88,0x00");
    gcs_write_simulator_memory(0x31ac9e, 2, "0x98,0x00");
    gcs_write_simulator_memory(0x31aca0, 2, "0xa8,0x00");
    gcs_write_simulator_memory(0x31aca2, 2, "0x9c,0x00");
    gcs_write_simulator_memory(0x31aca4, 2, "0x9c,0x00");
    gcs_write_simulator_memory(0x31aca6, 2, "0xc6,0x00");
    gcs_write_simulator_memory(0x31aca8, 2, "0xc2,0x00");
    gcs_write_simulator_memory(0x31acaa, 2, "0xb8,0x00");
    gcs_write_simulator_memory(0x31acac, 2, "0xb8,0x00");
    gcs_write_simulator_memory(0x31acae, 2, "0xb4,0x00");
    gcs_write_simulator_memory(0x31acb0, 2, "0x9e,0x00");
    gcs_write_simulator_memory(0x31acb2, 2, "0x8c,0x00");
    gcs_write_simulator_memory(0x31acb4, 2, "0x94,0x00");
    gcs_write_simulator_memory(0x31acb6, 2, "0x80,0x00");
    gcs_write_simulator_memory(0x31acb8, 2, "0xa2,0x00");
    gcs_write_simulator_memory(0x31acba, 2, "0x98,0x00");
    gcs_write_simulator_memory(0x31acbc, 2, "0xa8,0x00");
    gcs_write_simulator_memory(0x31acbe, 2, "0xa8,0x00");
    gcs_write_simulator_memory(0x31acc0, 2, "0xa2,0x00");
    gcs_write_simulator_memory(0x31acc2, 2, "0x9e,0x00");
    gcs_write_simulator_memory(0x31acc4, 2, "0xb8,0x00");
    gcs_write_simulator_memory(0x31acc6, 2, "0xc0,0x00");
    gcs_write_simulator_memory(0x31ac46, 2, "0x7f,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8e4, 2, "0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8e6, 6, "0x34,0x06,0x34,0xad,0x31,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8ec, 4, "0xf4,0xc7,0x60,0xf9");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31ad38, 4, "0xd2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad3c, 4, "0x0a,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31ad40, 4, "0x1e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad44, 4, "0x4c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad48, 4, "0xfa,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad4c, 4, "0x2e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad50, 4, "0x4e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad54, 4, "0xe6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad58, 4, "0x4c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad5c, 4, "0x32,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad60, 4, "0x2e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad64, 4, "0xaa,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad68, 4, "0x3a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad6c, 4, "0x46,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad70, 4, "0xe2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad74, 4, "0xc6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad78, 4, "0x52,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad7c, 4, "0x1e,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31ad80, 4, "0x2e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad84, 4, "0x0a,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31ad88, 4, "0x9a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad8c, 4, "0x8e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad90, 4, "0xe2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad94, 4, "0x12,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad98, 4, "0xa2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad9c, 4, "0x82,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ada0, 4, "0x4e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ada4, 4, "0xcc,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ada8, 4, "0x02,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31adac, 4, "0x76,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31adb0, 4, "0x2e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31adb4, 4, "0xd6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31adb8, 4, "0xf2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31adbc, 4, "0xb6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31adc0, 4, "0x3e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31adc4, 4, "0x6c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31adc8, 4, "0x92,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31adcc, 4, "0x2e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31add0, 4, "0x0a,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31add4, 4, "0x7a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31add8, 4, "0x06,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31addc, 4, "0x2a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ade0, 4, "0xf6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ade4, 4, "0xec,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ade8, 4, "0xde,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31adec, 4, "0xfa,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31adf0, 4, "0xc4,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31adf4, 4, "0x08,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31adf8, 4, "0xec,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31adfc, 4, "0x5e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae00, 4, "0x6e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae04, 4, "0x10,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31ae08, 4, "0x8c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae0c, 4, "0x4c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae10, 4, "0x2a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae14, 4, "0x9a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae18, 4, "0xaa,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae1c, 4, "0xca,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae20, 4, "0x82,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae24, 4, "0x18,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31ae28, 4, "0x20,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae2c, 4, "0x14,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31ae30, 4, "0x06,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31ae34, 4, "0xd2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae38, 4, "0x9e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae3c, 4, "0xbc,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae40, 4, "0x04,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae44, 4, "0xde,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae48, 4, "0x4c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae4c, 4, "0x06,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31ae50, 4, "0xb8,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae54, 4, "0xe8,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae58, 4, "0x76,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae5c, 4, "0x68,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae60, 4, "0xde,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae64, 4, "0x14,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31ae68, 4, "0x76,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae6c, 4, "0x06,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae70, 4, "0xf4,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae74, 4, "0xca,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae78, 4, "0x82,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae7c, 4, "0xd2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae80, 4, "0x82,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae84, 4, "0xc6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae88, 4, "0x6c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae8c, 4, "0x0a,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31ae90, 4, "0x5e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae94, 4, "0x7e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ae98, 4, "0x08,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31ae9c, 4, "0xcc,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aea0, 4, "0x10,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31aea4, 4, "0x9a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aea8, 4, "0x8a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aeac, 4, "0x24,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aeb0, 4, "0x06,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31aeb4, 4, "0xc4,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aeb8, 4, "0x72,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aebc, 4, "0xae,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aec0, 4, "0x3a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aec4, 4, "0x3a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aec8, 4, "0xc0,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aecc, 4, "0x0e,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31aed0, 4, "0x40,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aed4, 4, "0x72,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aed8, 4, "0x0a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aedc, 4, "0xde,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aee0, 4, "0x0a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aee4, 4, "0xe2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aee8, 4, "0xce,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aeec, 4, "0x10,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31aef0, 4, "0x0e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aef4, 4, "0x9a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aef8, 4, "0xb6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aefc, 4, "0x40,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af00, 4, "0x8a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af04, 4, "0x12,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af08, 4, "0x68,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af0c, 4, "0x5e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af10, 4, "0xf2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af14, 4, "0x22,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31af18, 4, "0x06,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31af1c, 4, "0xd6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af20, 4, "0xb6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af24, 4, "0x1e,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31af28, 4, "0x18,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31af2c, 4, "0xf2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af30, 4, "0x96,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af34, 4, "0x46,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af38, 4, "0x14,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af3c, 4, "0xe6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af40, 4, "0xec,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af44, 4, "0xda,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af48, 4, "0xda,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af4c, 4, "0xe6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af50, 4, "0xc0,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af54, 4, "0x3e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af58, 4, "0x18,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31af5c, 4, "0x68,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af60, 4, "0x3e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af64, 4, "0xa6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af68, 4, "0xec,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af6c, 4, "0xe2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af70, 4, "0x64,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af74, 4, "0xc6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af78, 4, "0x68,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af7c, 4, "0x56,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af80, 4, "0x06,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af84, 4, "0x62,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af88, 4, "0x42,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af8c, 4, "0x06,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af90, 4, "0xce,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af94, 4, "0x1e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af98, 4, "0x86,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31af9c, 4, "0xe8,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31afa0, 4, "0x42,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31afa4, 4, "0xfa,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31afa8, 4, "0x06,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31afac, 4, "0x40,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31afb0, 4, "0x04,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31afb4, 4, "0xde,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31afb8, 4, "0x56,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31afbc, 4, "0x10,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31afc0, 4, "0x0e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31afc4, 4, "0x8c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31afc8, 4, "0x0e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31afcc, 4, "0x04,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31afd0, 4, "0x2e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31afd4, 4, "0x5c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31afd8, 4, "0x18,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31afdc, 4, "0xc4,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31afe0, 4, "0x9c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31afe4, 4, "0xf2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31afe8, 4, "0x92,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31afec, 4, "0xc4,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aff0, 4, "0x76,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31aff4, 4, "0x06,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31aff8, 4, "0x7a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31affc, 4, "0x52,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b000, 4, "0x10,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b004, 4, "0x2a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b008, 4, "0x1e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b00c, 4, "0x86,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b010, 4, "0x3a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b014, 4, "0x08,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b018, 4, "0xb2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b01c, 4, "0x1a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b020, 4, "0x0e,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b024, 4, "0xb0,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b028, 4, "0x14,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b02c, 4, "0x5a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b030, 4, "0x06,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b034, 4, "0x2a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b038, 4, "0x9c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b03c, 4, "0xcc,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b040, 4, "0x1e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b044, 4, "0xe8,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b048, 4, "0x64,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b04c, 4, "0x7a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b050, 4, "0x0e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b054, 4, "0x6e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b058, 4, "0xbc,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b05c, 4, "0x3a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b060, 4, "0xda,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b064, 4, "0x4e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b068, 4, "0xf6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b06c, 4, "0x5e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b070, 4, "0x1a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b074, 4, "0x92,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b078, 4, "0xf6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b07c, 4, "0xbc,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b080, 4, "0xec,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b084, 4, "0x76,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b088, 4, "0xa2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b08c, 4, "0x64,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b090, 4, "0x40,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b094, 4, "0xb2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b098, 4, "0x06,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b09c, 4, "0xf4,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b0a0, 4, "0xbe,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b0a4, 4, "0xb6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b0a8, 4, "0x2a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b0ac, 4, "0xe8,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b0b0, 4, "0x18,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b0b4, 4, "0xfa,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b0b8, 4, "0x90,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b0bc, 4, "0x1a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b0c0, 4, "0x9a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b0c4, 4, "0x0a,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b0c8, 4, "0x8e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b0cc, 4, "0xc0,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b0d0, 4, "0x00,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b0d4, 4, "0x12,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b0d8, 4, "0x1e,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b0dc, 4, "0x92,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b0e0, 4, "0xca,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b0e4, 4, "0xae,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b0e8, 4, "0x24,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b0ec, 4, "0x52,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b0f0, 4, "0xa6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b0f4, 4, "0x72,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b0f8, 4, "0xd2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b0fc, 4, "0x8c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b100, 4, "0x1e,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b104, 4, "0x14,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b108, 4, "0x64,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b10c, 4, "0x6e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b110, 4, "0xa6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b114, 4, "0xe2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b118, 4, "0x18,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b11c, 4, "0x8c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b120, 4, "0x4c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b124, 4, "0x46,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b128, 4, "0x18,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b12c, 4, "0x0a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b130, 4, "0xda,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b134, 4, "0x96,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31ad34, 2, "0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31ad36, 2, "0x7f,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8f0, 2, "0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8f2, 6, "0x34,0x06,0x78,0xb2,0x31,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8f8, 4, "0xf4,0xc7,0x60,0xf9");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31b27c, 4, "0xda,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b280, 4, "0xfe,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b284, 4, "0x74,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b288, 4, "0x72,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b28c, 4, "0xfa,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b290, 4, "0x6a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b294, 4, "0xfe,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b298, 4, "0xba,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b29c, 4, "0x3a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b2a0, 4, "0x9e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b2a4, 4, "0x10,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b2a8, 4, "0xc8,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b2ac, 4, "0x06,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b2b0, 4, "0x0c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b2b4, 4, "0x16,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b2b8, 4, "0x5c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b2bc, 4, "0x46,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b2c0, 4, "0x36,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b2c4, 4, "0xa8,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b2c8, 4, "0x54,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b2cc, 4, "0x7e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b2d0, 4, "0x60,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b2d4, 4, "0x16,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b2d8, 4, "0x0c,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b2dc, 4, "0x64,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b2e0, 4, "0xda,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b2e4, 4, "0x4c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b2e8, 4, "0xc8,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b2ec, 4, "0x00,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b2f0, 4, "0xf2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b2f4, 4, "0xc4,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b2f8, 4, "0x50,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b2fc, 4, "0xd0,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b300, 4, "0x46,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b304, 4, "0x04,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b308, 4, "0xac,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b30c, 4, "0x62,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b310, 4, "0xe2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b314, 4, "0x62,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b318, 4, "0x28,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b31c, 4, "0x7e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b320, 4, "0x50,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b324, 4, "0xc8,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b328, 4, "0xf2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b32c, 4, "0x46,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b330, 4, "0x6e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b334, 4, "0x1e,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b338, 4, "0x48,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b33c, 4, "0x2e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b340, 4, "0x16,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b344, 4, "0x7e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b348, 4, "0x1e,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b34c, 4, "0x1a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b350, 4, "0xc8,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b354, 4, "0xda,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b358, 4, "0x18,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b35c, 4, "0x5c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b360, 4, "0xf2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b364, 4, "0x5c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b368, 4, "0x00,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b36c, 4, "0x78,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b370, 4, "0x28,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b374, 4, "0xcc,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b378, 4, "0x86,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b37c, 4, "0x62,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b380, 4, "0x02,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b384, 4, "0x32,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b388, 4, "0x5c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b38c, 4, "0x4a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b390, 4, "0x1e,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b394, 4, "0x58,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b398, 4, "0x50,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b39c, 4, "0xfe,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b3a0, 4, "0xee,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b3a4, 4, "0x18,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b3a8, 4, "0xf6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b3ac, 4, "0xba,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b3b0, 4, "0x64,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b3b4, 4, "0x54,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b3b8, 4, "0x3a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b3bc, 4, "0x16,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b3c0, 4, "0x1a,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b3c4, 4, "0x02,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b3c8, 4, "0xb6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b3cc, 4, "0xee,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b3d0, 4, "0x02,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b3d4, 4, "0xea,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b3d8, 4, "0x04,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b3dc, 4, "0x1e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b3e0, 4, "0xae,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b3e4, 4, "0xe6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b3e8, 4, "0xda,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b3ec, 4, "0x68,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b3f0, 4, "0xc2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b3f4, 4, "0x54,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b3f8, 4, "0x28,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b3fc, 4, "0x18,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b400, 4, "0xe6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b404, 4, "0x10,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b408, 4, "0xb6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b40c, 4, "0x1e,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b410, 4, "0xa8,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b414, 4, "0x20,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b418, 4, "0x40,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b41c, 4, "0x3a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b420, 4, "0xac,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b424, 4, "0x8c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b428, 4, "0x64,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b42c, 4, "0x58,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b430, 4, "0x10,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b434, 4, "0x14,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b438, 4, "0x1a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b43c, 4, "0x1a,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b440, 4, "0x1a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b444, 4, "0xe6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b448, 4, "0x4c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b44c, 4, "0xa8,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b450, 4, "0xf2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b454, 4, "0x12,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b458, 4, "0xea,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b45c, 4, "0xc4,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b460, 4, "0x64,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b464, 4, "0x78,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b468, 4, "0x64,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b46c, 4, "0x9e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b470, 4, "0x62,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b474, 4, "0x50,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b478, 4, "0x42,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b47c, 4, "0x40,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b480, 4, "0xc8,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b484, 4, "0xee,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b488, 4, "0x12,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b48c, 4, "0x2e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b490, 4, "0xd4,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b494, 4, "0x10,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b498, 4, "0x72,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b49c, 4, "0x28,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b4a0, 4, "0xd8,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b4a4, 4, "0xde,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b4a8, 4, "0x06,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b4ac, 4, "0x1a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b4b0, 4, "0x0c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b4b4, 4, "0x12,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b4b8, 4, "0x64,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b4bc, 4, "0x24,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b4c0, 4, "0x86,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b4c4, 4, "0xd4,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b4c8, 4, "0x1a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b4cc, 4, "0xbe,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b4d0, 4, "0x00,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b4d4, 4, "0xcc,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b4d8, 4, "0x16,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b4dc, 4, "0xaa,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b4e0, 4, "0x1e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b4e4, 4, "0x82,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b4e8, 4, "0xa8,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b4ec, 4, "0x06,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b4f0, 4, "0xc4,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b4f4, 4, "0xb2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b4f8, 4, "0x8e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b4fc, 4, "0x4c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b500, 4, "0x3e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b504, 4, "0x10,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b508, 4, "0xde,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b50c, 4, "0xc2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b510, 4, "0x02,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b514, 4, "0x8c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b518, 4, "0x28,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b51c, 4, "0x64,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b520, 4, "0xe2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b524, 4, "0x0c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b528, 4, "0xea,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b52c, 4, "0x6a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b530, 4, "0x18,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b534, 4, "0xcc,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b538, 4, "0x00,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b53c, 4, "0x24,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b540, 4, "0x42,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b544, 4, "0x1a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b548, 4, "0x3a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b54c, 4, "0x20,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b550, 4, "0xba,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b554, 4, "0x1a,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b558, 4, "0xa4,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b55c, 4, "0x28,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b560, 4, "0x28,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b564, 4, "0x86,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b568, 4, "0x8c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b56c, 4, "0x6e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b570, 4, "0x06,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b574, 4, "0x46,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b578, 4, "0xba,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b57c, 4, "0x9e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b580, 4, "0x0c,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b584, 4, "0xe6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b588, 4, "0x24,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b58c, 4, "0xcc,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b590, 4, "0x60,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b594, 4, "0xc4,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b598, 4, "0xa2,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b59c, 4, "0x82,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b5a0, 4, "0x06,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b5a4, 4, "0x8e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b5a8, 4, "0x48,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b5ac, 4, "0x0c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b5b0, 4, "0x04,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b5b4, 4, "0x78,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b5b8, 4, "0x10,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b5bc, 4, "0x62,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b5c0, 4, "0x06,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b5c4, 4, "0xd0,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b5c8, 4, "0x4c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b5cc, 4, "0xba,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b5d0, 4, "0x18,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b5d4, 4, "0x32,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b5d8, 4, "0x32,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b5dc, 4, "0x6e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b5e0, 4, "0x16,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b5e4, 4, "0x0a,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b5e8, 4, "0x16,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b5ec, 4, "0xfe,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b5f0, 4, "0x58,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b5f4, 4, "0xc8,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b5f8, 4, "0xee,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b5fc, 4, "0x6e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b600, 4, "0xa8,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b604, 4, "0xd0,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b608, 4, "0xaa,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b60c, 4, "0x10,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b610, 4, "0xe6,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b614, 4, "0xaa,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b618, 4, "0x04,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b61c, 4, "0x40,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b620, 4, "0x62,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b624, 4, "0x6a,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b628, 4, "0xfe,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b62c, 4, "0x16,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b630, 4, "0x7c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b634, 4, "0xac,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b638, 4, "0x1e,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b63c, 4, "0x0c,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b640, 4, "0x54,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b644, 4, "0x04,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b648, 4, "0x0c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b64c, 4, "0xc8,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b650, 4, "0x4c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b654, 4, "0x96,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b658, 4, "0x02,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b65c, 4, "0x5c,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b660, 4, "0x12,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b664, 4, "0x66,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b668, 4, "0x10,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b66c, 4, "0x36,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b670, 4, "0x28,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b674, 4, "0x16,0x04,0x00,0x00");
    gcs_write_simulator_memory(0x31b678, 4, "0x06,0x05,0x00,0x00");
    gcs_write_simulator_memory(0x31b278, 2, "0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31b27a, 2, "0x7f,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8fc, 2, "0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a8fe, 4, "0x80,0xff,0x1e,0x02");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31ab1c, 6, "0x25,0x06,0x7c,0xe6,0xff,0xfe");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31ab22, 6, "0x26,0x06,0x7c,0xe6,0xff,0xfe");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31ab28, 4, "0x66,0x2f,0x01,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31ab2c, 6, "0x25,0x06,0x7c,0xea,0xff,0xfe");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31ab32, 6, "0x26,0x06,0x7c,0xea,0xff,0xfe");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31ab38, 4, "0x66,0x2f,0x01,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31ab3c, 2, "0x7f,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a902, 2, "0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a904, 4, "0x80,0xff,0x52,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a956, 4, "0xe0,0x07,0x20,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a95a, 4, "0xe0,0x17,0x20,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a95e, 4, "0xe0,0x5f,0x20,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a962, 4, "0xe0,0xe7,0x20,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a966, 4, "0xe0,0xef,0x20,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a96a, 4, "0xe0,0x27,0x20,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a96e, 6, "0x2a,0x06,0x01,0x00,0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a974, 4, "0xea,0x2f,0x20,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a978, 4, "0xe0,0x3f,0x20,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a97c, 4, "0xe0,0x57,0x20,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a980, 4, "0xe0,0x5f,0x20,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a984, 4, "0xe0,0x67,0x20,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a988, 4, "0xe0,0x6f,0x20,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a98c, 4, "0xe0,0x77,0x20,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a990, 4, "0xe0,0x7f,0x20,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a994, 4, "0xe0,0x37,0x20,0x10");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a998, 6, "0x2a,0x06,0xff,0x03,0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a99e, 4, "0xea,0x3f,0x20,0x10");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9a2, 4, "0xe0,0x47,0x20,0x10");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9a6, 4, "0xe0,0x07,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9aa, 4, "0xe0,0x0f,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9ae, 4, "0xe0,0x17,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9b2, 4, "0xe0,0x27,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9b6, 4, "0xe0,0x2f,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9ba, 4, "0xe0,0x37,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9be, 4, "0xe0,0x47,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9c2, 4, "0xe0,0x4f,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9c6, 4, "0xe0,0x57,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9ca, 4, "0xe0,0x67,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9ce, 4, "0xe0,0x6f,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9d2, 4, "0xe0,0x77,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9d6, 4, "0xe0,0x87,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9da, 4, "0xe0,0x8f,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9de, 4, "0xe0,0x97,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9e2, 4, "0xe0,0xa7,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9e6, 4, "0xe0,0xaf,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9ea, 4, "0xe0,0xb7,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9ee, 4, "0xe0,0xc7,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9f2, 4, "0xe0,0xcf,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9f6, 4, "0xe0,0xd7,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9fa, 4, "0xe0,0xe7,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a9fe, 4, "0xe0,0xef,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa02, 4, "0xe0,0xf7,0x20,0x30");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa06, 4, "0xe0,0x07,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa0a, 4, "0xe0,0x0f,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa0e, 4, "0xe0,0x17,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa12, 4, "0xe0,0x27,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa16, 4, "0xe0,0x2f,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa1a, 4, "0xe0,0x37,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa1e, 4, "0xe0,0x47,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa22, 4, "0xe0,0x4f,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa26, 4, "0xe0,0x57,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa2a, 4, "0xe0,0x67,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa2e, 4, "0xe0,0x6f,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa32, 4, "0xe0,0x77,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa36, 4, "0xe0,0x87,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa3a, 4, "0xe0,0x8f,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa3e, 4, "0xe0,0x97,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa42, 4, "0xe0,0xa7,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa46, 4, "0xe0,0xaf,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa4a, 4, "0xe0,0xb7,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa4e, 4, "0xe0,0xc7,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa52, 4, "0xe0,0xcf,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa56, 4, "0xe0,0xd7,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa5a, 4, "0xe0,0xe7,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa5e, 4, "0xe0,0xef,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa62, 4, "0xe0,0xf7,0x20,0x38");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa66, 2, "0x7f,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a908, 2, "0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a90a, 2, "0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a90c, 2, "0x00,0xa0");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a90e, 4, "0xf4,0x7f,0x20,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a912, 4, "0x80,0xff,0x66,0x01");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa78, 4, "0xe0,0x17,0x34,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa7c, 4, "0xe0,0x27,0x34,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa80, 4, "0xe0,0x2f,0x34,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa84, 4, "0xe0,0x57,0x34,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa88, 4, "0xe0,0x5f,0x34,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa8c, 4, "0xe0,0x67,0x34,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa90, 4, "0xe0,0x6f,0x34,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa94, 4, "0xe0,0x77,0x34,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa98, 4, "0xe0,0x7f,0x34,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aa9c, 2, "0x7f,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a916, 2, "0x41,0xa2");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a918, 2, "0x61,0xa2");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a91a, 4, "0xfa,0x07,0xf5,0xff");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a91e, 2, "0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a920, 2, "0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a922, 2, "0x00,0xa0");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a924, 6, "0x35,0x06,0x01,0x00,0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a92a, 4, "0xf4,0x57,0x20,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a92e, 4, "0x80,0xff,0x80,0x01");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aaae, 4, "0xe0,0x07,0x30,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aab2, 4, "0xe0,0x17,0x30,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aab6, 6, "0x2a,0x06,0xc0,0x01,0x22,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aabc, 4, "0xea,0x37,0x30,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aac0, 4, "0xe0,0x3f,0x30,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aac4, 4, "0xe0,0x47,0x30,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aac8, 4, "0xe0,0x4f,0x30,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aacc, 4, "0xe0,0x57,0x30,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aad0, 4, "0xe0,0x5f,0x30,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aad4, 4, "0xe0,0x67,0x30,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aad8, 4, "0xe0,0x87,0x30,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aadc, 4, "0xe0,0x8f,0x30,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aae0, 4, "0xe0,0xe7,0x30,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aae4, 4, "0xe0,0xef,0x30,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aae8, 4, "0xe0,0x3f,0x30,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aaec, 4, "0xe0,0x37,0x30,0x10");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aaf0, 6, "0x2a,0x06,0xff,0x03,0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aaf6, 4, "0xea,0x3f,0x30,0x10");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aafa, 4, "0xe0,0x47,0x30,0x10");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31aafe, 4, "0xe0,0x47,0x30,0x28");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31ab02, 4, "0xe0,0x4f,0x30,0x28");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31ab06, 4, "0xe0,0x5f,0x30,0x28");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31ab0a, 2, "0x7f,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a932, 2, "0x41,0xa2");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a934, 2, "0xf5,0xa1");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a936, 4, "0xfa,0x07,0xf5,0xff");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a93a, 2, "0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a93c, 2, "0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a93e, 6, "0x2c,0x06,0xb4,0x1b,0x11,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x31a944, 2, "0x6c,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111bb4, 2, "0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111bb6, 6, "0x28,0x06,0x48,0xac,0x31,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111bbc, 4, "0xe8,0xa7,0x20,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111bc0, 6, "0x28,0x06,0x38,0xad,0x31,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111bc6, 4, "0xe8,0x77,0x20,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111bca, 6, "0x28,0x06,0x7c,0xb2,0x31,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111bd0, 4, "0xe8,0x67,0x20,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111bd4, 6, "0x28,0x06,0x00,0x01,0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111bda, 4, "0xe8,0x87,0x20,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111bde, 6, "0x28,0x06,0x00,0x00,0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111be4, 4, "0xe8,0x7f,0x20,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111be8, 6, "0x28,0x06,0x00,0x02,0x11,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111bee, 4, "0xe8,0x17,0x34,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111bf2, 6, "0x28,0x06,0x00,0x00,0x00,0xc0");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111bf8, 4, "0xe8,0x2f,0x34,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111bfc, 6, "0x28,0x06,0x07,0x00,0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111c02, 4, "0xe8,0x07,0x34,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111c06, 6, "0x28,0x06,0x00,0x00,0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111c0c, 4, "0xe8,0x57,0x20,0x08");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111c10, 6, "0x28,0x06,0x00,0x00,0x00,0x80");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111c16, 4, "0xe8,0x2f,0x30,0x10");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111c1a, 6, "0x28,0x06,0x00,0x02,0x11,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111c20, 4, "0xe8,0x07,0x32,0xf8");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111c24, 6, "0x28,0x06,0x34,0x68,0x31,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111c2a, 4, "0xe8,0x07,0x20,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111c2e, 6, "0x28,0x06,0x20,0x80,0x00,0x80");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111c34, 4, "0xe8,0x0f,0x20,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111c38, 4, "0xe0,0x07,0x48,0x01");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x110200, 6, "0x23,0x06,0x5c,0x1c,0x11,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x110206, 2, "0x63,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111c5c, 2, "0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111c5e, 2, "0x00,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111c60, 4, "0xe5,0x47,0x40,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111c64, 6, "0x29,0x06,0xff,0xff,0xf8,0xff");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111c6a, 2, "0x49,0x41");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111c6c, 6, "0x29,0x06,0x00,0x00,0x01,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111c72, 2, "0x09,0x41");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    gcs_write_simulator_memory(0x111c74, 4, "0xe8,0x2f,0x20,0x00");
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);
    err = gcs_step_simulator(0);

    err = gcs_get_nc_register("PSW", &get_value);
    err = gcs_get_tc_register("PSW", &get_value, TC0);


    gcs_disable_simulator_stop(0);
    return true;
}

bool test_register_call_back_function (){
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }
    int err = gcs_step_simulator(0);
    int step = 0;
    while ((err == GCSIF_SUCCESS) && (step < 5000)) {
        err = gcs_step_simulator(0);
        ++ step;
    }
    gcs_disable_simulator_stop(0);

    if (err != GCSIF_ERR_THREAD_IS_HALT) {
        return false;
    } else {
        return true;
    }
}

bool test_simple_sequence_1 (){
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    uint32_t mode, err = 0;
    FrogRegData value;
    uint32_t psw_data, pc_data, r1_data, vr6_data;

    if (err != GCSIF_SUCCESS)
    {
        printf ("Error 1 <Errorcode %d>: gcs_initialize_simulator_ex \n", err);
    }
//    if (pHwInfo != NULL) {
//        printf ("vmnum: %d \n", pHwInfo->vmnum);
//        printf ("bpnum: %d \n", pHwInfo->bpnum);
//        printf ("mpnum: %d \n", pHwInfo->mpnum);
//        printf ("threadnum: %d \n", pHwInfo->threadnum);
//        printf ("smallpage: %d \n", pHwInfo->smallpage);
//        printf ("tlbnum: %d \n", pHwInfo->tlbnum);
//    }
    std::string bytes_read;
//    err = gcs_write_simulator_memory(0x00000000,0x4,"0x11, 0x39, 0xAB, 0x0C");
//    err = gcs_read_simulator_memory(0x00000000, 0x4, bytes_read);
    err = gcs_step_simulator(0);
    if (err != GCSIF_SUCCESS)
    {
        printf ("Error 2 <Errorcode %d>: gcs_step_simulator \n", err);
    }
    err = gcs_is_simulator_native(&mode);
    if (err != GCSIF_SUCCESS)
    {
        printf ("Error 3 <Errorcode %d>: gcs_is_simulator_native \n", err);
    }
    err = gcs_get_nc_register("PSW", &value);
    psw_data = static_cast<uint32_t>(value & 0xFFFFFFFFU);
    if (err != GCSIF_SUCCESS)
    {
        printf ("Error 4 <Errorcode %d>: gcs_get_nc_register \n", err);
    } else {
        printf ("value psw_64: %lx \n",(uint64_t) value);
        printf ("value psw_32: %x \n", psw_data);
    }
    err = gcs_step_simulator(0);
    if (err != GCSIF_SUCCESS)
    {
        printf ("Error 5 <Errorcode %d>: gcs_step_simulator \n", err);
    }
    err = gcs_write_simulator_memory(0x00000000,0x4,"0x11, 0x39, 0xAB, 0x0C");
    if (err != GCSIF_SUCCESS)
    {
        printf ("Error 6 <Errorcode %d>: gcs_write_simulator_memory \n", err);
    }
    err = gcs_read_simulator_memory(0x00000000, 0x4, bytes_read);
    if (err != GCSIF_SUCCESS)
    {
        printf ("Error 7 <Errorcode %d>: gcs_read_simulator_memory  \n", err);
    }
    printf ("bytes_read: %s \n", bytes_read.c_str());
    err = gcs_get_nc_register("PC", &value);
    pc_data = static_cast<uint32_t>(value & 0xFFFFFFFFU);
    if (err != GCSIF_SUCCESS)
    {
        printf ("Error 8 <Errorcode %d>: gcs_get_nc_register \n", err);
    } else {
        printf ("value PC_64: %lx \n", (uint64_t) value);
        printf ("value PC_32: %x \n", pc_data);
    }
    err = gcs_get_tc_register("R1", &value, 0);
    r1_data = static_cast<uint32_t>(value & 0xFFFFFFFFU);
    if (err != GCSIF_SUCCESS)
    {
        printf ("Error 9 <Errorcode %d>: gcs_get_tc_register \n", err);
    } else {
        printf ("value R1_64: %lx \n", (uint64_t) value);
        printf ("value R1_32: %x \n", r1_data);
    }
    err = gcs_get_vc_register("VR6", &value, 0);
    vr6_data = static_cast<uint32_t>(value & 0xFFFFFFFFU);
    if (err != GCSIF_SUCCESS)
    {
        printf ("Error 10 <Errorcode %d>: gcs_get_vc_register \n", err);
    } else {
        printf ("value VR6_64: %lx \n", (uint64_t) value);
        printf ("value VR6_32: %x \n", vr6_data);
    }

    uint32_t telo0, telo1, tehi0, tehi1;
    err = gcs_read_simulator_tlb(0, &telo0, &telo1, &tehi0, &tehi1);
    if (err != GCSIF_SUCCESS)
    {
        printf ("Error 11 <Errorcode %d>: gcs_read_simulator_tlb \n", err);
    } else {
        printf ("value telo0_32: %x \n", telo0);
        printf ("value telo1_32: %x \n", telo1);
        printf ("value tehi0_32: %x \n", tehi0);
        printf ("value tehi1_32: %x \n", tehi1);
    }
    gcs_disable_simulator_stop(0);
    return true;
}

bool test_simple_sequence_2 (){
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }
    // Set PC to address 0x1056
    gcs_set_nc_register("pc", 0x1056ull);
    FrogRegData value;

    gcs_write_simulator_memory(0x1056, 2, "0x01,0x52");                       // 1042: 01 52                mov 1, r10
    gcs_write_simulator_memory(0x1058, 4, "0xea,0x7f,0x20,0x08");             // 1058: ea 7f 20 08          ldsr r10, sr15, sel1
    gcs_write_simulator_memory(0x105c, 2, "0x0f,0x52");                       // 105c: 0f 52                mov 15, r10
    gcs_write_simulator_memory(0x105e, 4, "0xea,0x07,0x34,0x08");             // 105e: ea 07 34 08          ldvc.sr r10, sr0, sel1
    gcs_write_simulator_memory(0x1062, 4, "0x90,0x07,0x03,0x78");             // 1062: 90 07 03 78          prepare 0x78000000,0x8,sp
    gcs_write_simulator_memory(0x1066, 6, "0x22,0x06,0x03,0x78,0x00,0x00");   // 1066: 22 06 03 78 00 00    mov 0x7803, r2
    gcs_write_simulator_memory(0x106c, 4, "0xFF,0xFF,0x60,0xF1");             // 106c: FF FF 60 F1          cache 0x7e,[r31] (cll)
    gcs_write_simulator_memory(0x1070, 4, "0xE8,0x57,0x30,0x28");             // 106c: FF FF 60 F1          ldtc.sr r8, sr10, sel5
    gcs_write_simulator_memory(0x1074, 2, "0x40, 0x12");                      // 1070: 40 12                add 0x0, r2
    gcs_get_nc_register("MCFG0", &value);
    value |= 0x01;
    gcs_set_nc_register("MCFG0", value);
    gcs_write_simulator_memory(0x1076, 4, "0xF6, 0x07, 0x10, 0x01");          // 1072: F6 07 10 01          hvtrap
    printf("----------------------------------------\n");
    printf("1042: 01 52                mov 1, r10 \n");
    printf("1058: ea 7f 20 08          ldsr r10, sr15, sel1 \n");
    printf("105c: 0f 52                mov 15, r10 \n");
    printf("105e: ea 07 34 08          ldvc.sr r10, sr0, sel1 \n");
    printf("1062: 90 07 03 78          prepare 0x78000000,0x8,sp \n");
    printf("1066: 22 06 03 78 00 00    mov 0x7803, r2 \n");
    printf("106c: FF FF 60 F1          cache 0x7e,[r31] (cll) \n");
    printf("1070: 40 12                add 0x0, r2 \n");
    printf("1072: F6 07 10 01          hvtrap \n");
    printf("----------------------------------------\n");
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    gcs_step_simulator(0);
    printf("----------------------------------------\n");
    gcs_disable_simulator_stop(0);
    return true;
}


bool test_gcs_tlb() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }
    int err = 0;
    uint32_t value_telo0 = 0xffffffff;
    uint32_t value_telo1 = 0xffffffff;
    uint32_t value_tehi0 = 0xffffffff;
    uint32_t value_tehi1 = 0xffffffff;
    uint32_t index_tlb = 0x10;
    err = gcs_write_simulator_tlb(index_tlb, value_telo0, value_telo1, value_tehi0, value_tehi1);
    if (err == GCSIF_ERR_MMU_DISABLED) {
        printf ("< #Error# MMU is not enable >\n");
    }
    index_tlb = 0x10;
    err = gcs_read_simulator_tlb(index_tlb, &value_telo0, &value_telo1, &value_tehi0, &value_tehi1);
    gcs_disable_simulator_stop(0);
    return true;
}

bool test_gcs_llbit() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_create_memory_link ((unsigned long long)0x800000ull, true,  0, 4);
    gcs_create_memory_link ((unsigned long long)0x800050ull, false, 0, 4);
    gcs_create_memory_link ((unsigned long long)0x800030ull, false, 0, 4);
    gcs_create_memory_link ((unsigned long long)0x800030ull, false, 1, 4);
    gcs_create_memory_link ((unsigned long long)0x800032ull, false, 2, 4);
    gcs_create_memory_link ((unsigned long long)0x700032ull, false, 2, 4);
    gcs_create_memory_link ((unsigned long long)0x700132ull, false, 2, 4);

    gcs_remove_memory_link_addr ((unsigned long long)0x800038ull);
    gcs_remove_memory_link_addr ((unsigned long long)0x800030ull);
    gcs_disable_simulator_stop(0);
    return true;
}

bool test_gcs_reset_simulator() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }
    gcs_set_nc_register("PC", 0x1056ull);
    gcs_write_simulator_memory(0x1056, 2, "0x01,0x52");             // 1042: 01 52          mov 1, r10
    gcs_write_simulator_memory(0x1058, 4, "0xea,0x7f,0x20,0x08");   // 1058: ea 7f 20 08    ldsr r10, sr15, sel1
    gcs_write_simulator_memory(0x105c, 2, "0x0f,0x52");             // 105c: 0f 52          mov 15, r10
    gcs_write_simulator_memory(0x105e, 4, "0xea,0x07,0x34,0x08");   // 105e: ea 07 34 08    ldvc.sr r10, sr0, sel1
    gcs_reset_simulator();
    gcs_step_simulator(0);
    gcs_disable_simulator_stop(0);
    return true;
}

bool test_excp() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }
    int err = gcs_step_simulator(0);
    int step = 0;
    while ((err == GCSIF_SUCCESS) && (step < 5000)) {
        err = gcs_step_simulator(0);
        ++ step;
    }
    gcs_disable_simulator_stop(0);

    if (err != GCSIF_ERR_THREAD_IS_HALT) {
        return false;
    } else {
        return true;
    }
}

bool test_register_name () {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }
    std::string name;
    bool err = gcs_get_sreg_name(0, 5, name);
    if (err == false) {
        printf ("< #Error# Can't find register name >\n");
    } else {
        printf ("< SR: %s >\n", name.c_str());
    }

    gcs_set_nc_register(name, 0x1056ull);
    gcs_disable_simulator_stop(0);
    return true;
}

bool test_fxu_function (void) {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }
    bool ret_val = gcs_check_fxu_function();
    gcs_disable_simulator_stop(0);
    return ret_val;
}


bool test_fpu_function (void) {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }
    bool ret_val = gcs_check_fpu_function();
    gcs_disable_simulator_stop(0);
    return ret_val;
}


bool test_fpsimd_register() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    FrogRegData get_value = 0x00;

    uint32_t err_info = 0;

    if (gcs_get_nc_register("FXSR", &get_value)    != GCSIF_SUCCESS) err_info |= (1 << 0);
    if (gcs_set_nc_register("FXSR", 0xFFFFFFFFull) != GCSIF_SUCCESS) err_info |= (1 << 1);
    if (gcs_get_nc_register("FXSR", &get_value)    != GCSIF_SUCCESS) err_info |= (1 << 2);

    if (gcs_get_nc_register("FXST", &get_value)    != GCSIF_SUCCESS) err_info |= (1 << 3);
    if (gcs_set_nc_register("FXST", 0xFFFFFFFFull) != GCSIF_SUCCESS) err_info |= (1 << 4);
    if (gcs_get_nc_register("FXST", &get_value)    != GCSIF_SUCCESS) err_info |= (1 << 5);

    if (gcs_get_nc_register("FXINFO", &get_value)    != GCSIF_SUCCESS) err_info |= (1 << 6);
    if (gcs_set_nc_register("FXINFO", 0xFFFFFFFFull) != GCSIF_SUCCESS) err_info |= (1 << 7);
    if (gcs_get_nc_register("FXINFO", &get_value)    != GCSIF_SUCCESS) err_info |= (1 << 8);

    if (gcs_get_nc_register("FXCFG", &get_value)    != GCSIF_SUCCESS) err_info |= (1 << 9);
    if (gcs_set_nc_register("FXCFG", 0xFFFFFFFFull) != GCSIF_SUCCESS) err_info |= (1 << 10);
    if (gcs_get_nc_register("FXCFG", &get_value)    != GCSIF_SUCCESS) err_info |= (1 << 11);

    if (gcs_get_nc_register("FXXC", &get_value)    != GCSIF_SUCCESS) err_info |= (1 << 12);
    if (gcs_set_nc_register("FXXC", 0xFFFFFFFFull) != GCSIF_SUCCESS) err_info |= (1 << 13);
    if (gcs_get_nc_register("FXXC", &get_value)    != GCSIF_SUCCESS) err_info |= (1 << 14);

    if (gcs_get_nc_register("FXXP", &get_value)    != GCSIF_SUCCESS) err_info |= (1 << 15);
    if (gcs_set_nc_register("FXXP", 0xFFFFFFFFull) != GCSIF_SUCCESS) err_info |= (1 << 16);
    if (gcs_get_nc_register("FXXP", &get_value)    != GCSIF_SUCCESS) err_info |= (1 << 17);

    if (gcs_get_nc_register("WR0", &get_value)    != GCSIF_SUCCESS) err_info |= (1 << 18);
    if (gcs_get_nc_register("WR1", &get_value)    != GCSIF_SUCCESS) err_info |= (1 << 19);

    FrogRegData write_data0, write_data1;
    write_data0 = 0x1111222233334444ULL;
    write_data0 = (write_data0 << 64) | 0x5555666677778888ULL;
    write_data1 = 0x9999aaaabbbbccccULL;
    write_data1 = (write_data1 << 64) | 0xddddeeeeffff0000ULL;
    if (gcs_set_nc_register("WR0", write_data0) != GCSIF_SUCCESS) err_info |= (1 << 20);
    if (gcs_set_nc_register("WR1", write_data1) != GCSIF_SUCCESS) err_info |= (1 << 21);
    if (gcs_get_nc_register("WR0", &get_value)  != GCSIF_SUCCESS) err_info |= (1 << 22);
    printf ("WR0=0x%016llx%016llx\n",
           static_cast<unsigned long long>((get_value >> 64) & 0xFFFFFFFFFFFFFFFFULL),
           static_cast<unsigned long long>(get_value & 0xFFFFFFFFFFFFFFFFULL));
    if (get_value != write_data0) err_info |= (1 << 23);

    if (gcs_get_nc_register("WR1", &get_value)  != GCSIF_SUCCESS) err_info |= (1 << 24);
    printf ("WR1=0x%016llx%016llx\n",
           static_cast<unsigned long long>((get_value >> 64) & 0xFFFFFFFFFFFFFFFFULL),
           static_cast<unsigned long long>(get_value & 0xFFFFFFFFFFFFFFFFULL));
    if (get_value != write_data1) err_info |= (1 << 25);

    gcs_reset_simulator();
    gcs_disable_simulator_stop(0);
    return (err_info == 0);
}

bool test_fpsimd_simulation() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_reset_simulator();

    // Enable FPSIMD by PSW.CU1=1
    FrogRegData psw_data;
    gcs_get_nc_register("PSW", &psw_data);
    psw_data = psw_data | 0x00020000ULL;
    gcs_set_nc_register("PSW", psw_data);

    // Set value to WR0
    FrogRegData write_data;
    write_data = 0x1111222233334444ULL;
    write_data = (write_data << 64) | 0x5555666677778888ULL;
    gcs_set_nc_register("WR0", write_data);

    gcs_write_simulator_memory(0x0, 4, "0xFE,0x07,0xA0,0x0D");             // 0: FE 07 A0 0D   MOVV.W4 wr0, wr1

    gcs_step_simulator(0);
    gcs_disable_simulator_stop(0);
    return true;
}


bool test_eiint() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    uint32_t channel = 0;
    uint32_t priority = 0;
    bool eitb = false;
    int isNC = true;
    unsigned int ptid = 0;

    gcs_set_nc_register("pmr", 0x0000FFFFull);

    if (gcs_req_eiint_pe (2048, priority, eitb, isNC, ptid) != GCSIF_ERR_INVALID_EVENT) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_eiint_pe (channel, 16, eitb, isNC, ptid) != GCSIF_ERR_INVALID_EVENT) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_eiint_pe (channel, priority, eitb, isNC, 1) != GCSIF_ERR_INVALID_TCID) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_eiint_pe (0, 0, eitb, isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_eiint_pe (0, 15, eitb, isNC, ptid) != GCSIF_WAR_DUPRICATED_EVENT) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_eiint_pe (511, 15, eitb, isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    FrogRegData get_value = 0x00;
    if (gcs_get_nc_register("icsr", &get_value) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    gcs_reset_simulator();
    gcs_disable_simulator_stop(0);
    return (err_info == 0);
}


bool test_syserr() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    uint32_t cause = 0x10;
    int isNC = true;
    unsigned int ptid = 0;

    if (gcs_req_syserr_pe (0x9, isNC, ptid) != GCSIF_ERR_INVALID_EVENT) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_syserr_pe (0x20, isNC, ptid) != GCSIF_ERR_INVALID_EVENT) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_syserr_pe (cause, isNC, 1) != GCSIF_ERR_INVALID_TCID) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_syserr_pe (0x10, isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_syserr_pe (0x10, isNC, ptid) != GCSIF_WAR_DUPRICATED_EVENT) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_syserr_pe (0x1F, isNC, ptid) != GCSIF_WAR_DUPRICATED_EVENT) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    gcs_reset_simulator();
    gcs_disable_simulator_stop(0);
    return (err_info == 0);
}


bool test_rlb() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    uint32_t source_peid = 1;
    int isNC = true;
    unsigned int ptid = 0;

    FrogRegData get_value = 0x00;
    gcs_get_nc_register("dir0", &get_value);
    get_value |= 0x00010000;
    gcs_set_nc_register("dir0", get_value);

    if (gcs_req_rlb_pe (0x0, isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_rlb_pe (0x8, isNC, ptid) != GCSIF_ERR_INVALID_TCID) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_rlb_pe (source_peid, isNC, 1) != GCSIF_ERR_INVALID_TCID) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_rlb_pe (0x1, isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_rlb_pe (0x1, isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_rlb_pe (0x7, isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    if (gcs_get_nc_register("dir0", &get_value) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    gcs_reset_simulator();
    gcs_disable_simulator_stop(0);
    return (err_info == 0);
}


bool test_dbnmi() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    int isNC = true;
    unsigned int ptid = 0;

    if (gcs_req_dbnmi_pe (isNC, 1) != GCSIF_ERR_INVALID_TCID) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_dbnmi_pe (isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_dbnmi_pe (isNC, ptid) != GCSIF_ERR_NOT_RECEIVED) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    gcs_step_simulator(0);
    gcs_write_simulator_memory(0xb0, 4, "0xe0, 0x07, 0x46, 0x010");
    gcs_step_simulator(0);
    FrogRegData get_value = 0x00;
    if (gcs_get_nc_register("dir0", &get_value) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    gcs_reset_simulator();
    gcs_disable_simulator_stop(0);
    return (err_info == 0);
}


bool test_dbint() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    uint32_t channel = 0;
    int isNC = true;
    unsigned int ptid = 0;
    if (gcs_req_dbint_pe (16, isNC, ptid) != GCSIF_ERR_INVALID_EVENT) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_dbint_pe (channel, isNC, 1) != GCSIF_ERR_INVALID_TCID) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_dbint_pe (channel, isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_dbint_pe (channel, isNC, ptid) != GCSIF_ERR_NOT_RECEIVED) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    gcs_reset_simulator();
    gcs_disable_simulator_stop(0);
    return (err_info == 0);
}


bool test_rmint() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    int isNC = true;
    unsigned int ptid = 0;

    if (gcs_req_rmint_pe (isNC, 1) != GCSIF_ERR_INVALID_TCID) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_rmint_pe (isNC, ptid) != GCSIF_ERR_NOT_RECEIVED) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_rmint_pe (isNC, ptid) != GCSIF_ERR_NOT_RECEIVED) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    gcs_reset_simulator();
    gcs_disable_simulator_stop(0);
    return (err_info == 0);
}

bool test_fenmi() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    int isNC = true;
    unsigned int ptid = 0;

    if (gcs_req_fenmi_pe (isNC, 1) != GCSIF_ERR_INVALID_TCID) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_fenmi_pe (isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_fenmi_pe (isNC, ptid) != GCSIF_ERR_NOT_RECEIVED) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    gcs_reset_simulator();
    gcs_disable_simulator_stop(0);
    return (err_info == 0);
}


bool test_feint() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    uint32_t channel = 0;
    int isNC = true;
    unsigned int ptid = 0;
    if (gcs_req_feint_pe (16, isNC, ptid) != GCSIF_ERR_INVALID_EVENT) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_feint_pe (channel, isNC, 1) != GCSIF_ERR_INVALID_TCID) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_feint_pe (channel, isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_feint_pe (channel, isNC, ptid) != GCSIF_ERR_NOT_RECEIVED) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    gcs_reset_simulator();
    gcs_disable_simulator_stop(0);
    return (err_info == 0);
}


bool test_stopreq() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    int isNC = true;
    unsigned int ptid = 0;

    if (gcs_req_cpu_stop_pe(isNC, 1) != GCSIF_ERR_INVALID_TCID) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_cpu_stop_pe(isNC, 1) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_cpu_stop_pe(isNC, 1) != GCSIF_ERR_NOT_RECEIVED) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    gcs_reset_simulator();
    gcs_disable_simulator_stop(0);
    return (err_info == 0);
}


bool test_clear_eiint() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    bool eitb = false;
    int isNC = true;
    unsigned int ptid = 0;

    gcs_set_nc_register("pmr", 0x0000FFFFull);

    if (gcs_req_eiint_pe (0, 0, eitb, isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    if (gcs_clear_req_pe ("eiint", isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    FrogRegData get_value = 0x00;
    if (gcs_get_nc_register("icsr", &get_value) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    gcs_reset_simulator();
    gcs_disable_simulator_stop(0);
    return (err_info == 0);
}


bool test_clear_syserr() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    int isNC = true;
    unsigned int ptid = 0;

    if (gcs_req_syserr_pe (0x10, isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_clear_req_pe ("syserr", isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    gcs_reset_simulator();
    gcs_disable_simulator_stop(0);
    return (err_info == 0);
}


bool test_clear_rlb() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    int isNC = true;
    unsigned int ptid = 0;

    FrogRegData get_value = 0x00;
    gcs_get_nc_register("dir0", &get_value);
    get_value |= 0x00010000;
    gcs_set_nc_register("dir0", get_value);

    if (gcs_req_rlb_pe (0x1, isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_clear_req_pe ("rlb", isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_get_nc_register("dir0", &get_value) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    gcs_reset_simulator();
    gcs_disable_simulator_stop(0);
    return (err_info == 0);
}


bool test_clear_dbnmi() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    int isNC = true;
    unsigned int ptid = 0;

    if (gcs_req_dbnmi_pe (isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_clear_req_pe ("dbnmi", isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    gcs_reset_simulator();
    gcs_disable_simulator_stop(0);
    return (err_info == 0);
}


bool test_clear_dbint() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    uint32_t channel = 0;
    int isNC = true;
    unsigned int ptid = 0;
    if (gcs_req_dbint_pe (channel, isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_clear_req_pe ("dbint", isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    gcs_reset_simulator();
    gcs_disable_simulator_stop(0);
    return (err_info == 0);
}


bool test_clear_rmint() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    int isNC = true;
    unsigned int ptid = 0;

    if (gcs_req_rmint_pe (isNC, ptid) != GCSIF_ERR_NOT_RECEIVED) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_clear_req_pe ("rmint", isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    gcs_reset_simulator();
    gcs_disable_simulator_stop(0);
    return (err_info == 0);
}

bool test_clear_fenmi() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    int isNC = true;
    unsigned int ptid = 0;

    if (gcs_req_fenmi_pe (isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_clear_req_pe ("fenmi", isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    gcs_reset_simulator();
    gcs_disable_simulator_stop(0);
    return (err_info == 0);
}


bool test_clear_feint() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    uint32_t channel = 0;
    int isNC = true;
    unsigned int ptid = 0;
    if (gcs_req_feint_pe (channel, isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_clear_req_pe ("feint", isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    gcs_reset_simulator();
    gcs_disable_simulator_stop(0);
    return (err_info == 0);
}


bool test_clear_stopreq() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    uint32_t channel = 0;
    int isNC = true;
    unsigned int ptid = 0;
    if (gcs_req_cpu_stop_pe (isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_clear_req_pe ("stopreq", isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    gcs_reset_simulator();
    gcs_disable_simulator_stop(0);
    return (err_info == 0);
}


bool test_clear_event() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    int isNC = true;
    unsigned int ptid = 0;

    if (gcs_clear_req_pe ("eiint", isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_clear_req_pe ("eitbl", isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_clear_req_pe ("feint", isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_clear_req_pe ("fenmi", isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_clear_req_pe ("dbint", isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_clear_req_pe ("dbnmi", isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_clear_req_pe ("rmint", isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_clear_req_pe ("syserr", isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_clear_req_pe ("rlb", isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_clear_req_pe ("unknown", isNC, ptid) != GCSIF_ERR_INVALID_OTHER_PARAM) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_clear_req_pe ("eiint", isNC, 1) != GCSIF_ERR_INVALID_TCID) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_clear_req_pe ("stopreq", isNC, 1) != GCSIF_ERR_INVALID_TCID) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    gcs_reset_simulator();
    gcs_disable_simulator_stop(0);
    return (err_info == 0);
}


bool test_mpid_rw() {
    if (init_frog_if_test() == false ) {
        printf ("< #ERROR# Can't call init_frog_if_test() > \n");
    }

    gcs_reset_simulator();

    uint32_t err_info = 0;
    uint32_t ret;

    //  ldsr r7, sr24, sel5
    if ((ret = gcs_write_simulator_memory(0x0, 4, "0xe7,0xc7,0x20,0x28")) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed err=%x at %d\n", ret, __LINE__);
    }
    if ((ret = gcs_step_simulator(0)) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed err=%x at %d\n", ret, __LINE__);
    }

    gcs_reset_simulator();
    gcs_disable_simulator_stop(0);
    return (err_info == 0);
}


int main (int argc, char *argv[])
{
    std::vector <int32_t> vector_fail_testcase;
    int32_t func_pos = 0, count = 0;
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_gcs_get_vc_register : %d\n", func_pos);
    if (test_gcs_get_vc_register() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_gcs_get_nc_tc_register : %d\n", func_pos);
    if (test_gcs_get_nc_tc_register() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_gcs_get_register_profile : %d\n", func_pos);
    if (test_gcs_get_register_profile() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_register_call_back_function : %d\n", func_pos);
    if (test_register_call_back_function() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_simple_sequence_1 : %d\n", func_pos);
    if (test_simple_sequence_1() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_simple_sequence_2 : %d\n", func_pos);
    if (test_simple_sequence_2() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_gcs_llbit : %d\n", func_pos);
    if (test_gcs_llbit() != true) {
        test_gcs_llbit();
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_gcs_reset_simulator : %d\n", func_pos);
    if (test_gcs_reset_simulator() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_excp : %d\n", func_pos);
    if (test_excp() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_register_name : %d\n", func_pos);
    if (test_register_name() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_fxu_function : %d\n", func_pos);
    if (test_fxu_function() != true) {
        vector_fail_testcase.push_back(func_pos);
        printf ("FXU function is not supported \n");
    } else {
        printf ("FXU function is supported \n");
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_fpu_function : %d\n", func_pos);
    if (test_fpu_function() != true) {
        vector_fail_testcase.push_back(func_pos);
        printf ("FPU function is not supported \n");
    } else {
        printf ("FPU function is supported \n");
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_sr_vm : %d\n", func_pos);
    if (test_sr_vm() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_gcs_tlb : %d\n", func_pos);
    if (test_gcs_tlb() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_FROG_sample : %d\n", func_pos);
    if (test_FROG_sample() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_fpsimd_register : %d\n", func_pos);
    if (test_fpsimd_register() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_fpsimd_simulation : %d\n", func_pos);
    if (test_fpsimd_simulation() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_eiint : %d\n", func_pos);
    if (test_eiint () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_syserr : %d\n", func_pos);
    if (test_syserr () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_rlb : %d\n", func_pos);
    if (test_rlb () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_dbnmi : %d\n", func_pos);
    if (test_dbnmi () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_dbint : %d\n", func_pos);
    if (test_dbint () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_rmint : %d\n", func_pos);
    if (test_rmint () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_fenmi : %d\n", func_pos);
    if (test_fenmi () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_feint : %d\n", func_pos);
    if (test_feint () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_stopreq : %d\n", func_pos);
    if (test_stopreq () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_clear_eiint : %d\n", func_pos);
    if (test_clear_eiint () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_clear_syserr : %d\n", func_pos);
    if (test_clear_syserr () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_clear_rlb : %d\n", func_pos);
    if (test_clear_rlb () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_clear_dbnmi : %d\n", func_pos);
    if (test_clear_dbnmi () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_clear_dbint : %d\n", func_pos);
    if (test_clear_dbint () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_clear_rmint : %d\n", func_pos);
    if (test_clear_rmint () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_clear_fenmi : %d\n", func_pos);
    if (test_clear_fenmi () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_clear_feint : %d\n", func_pos);
    if (test_clear_feint () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_clear_stopreq : %d\n", func_pos);
    if (test_clear_stopreq () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_clear_event : %d\n", func_pos);
    if (test_clear_event () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    ++ func_pos;
    printf("test_mpid_rw : %d\n", func_pos);
    if (test_mpid_rw () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("----------------------------------------\n");

    printf("Total test function: %d \n", count);
    if (vector_fail_testcase.size() > 0) {
        std::vector<int32_t>::iterator it_fail = vector_fail_testcase.begin ();
        for (; it_fail != vector_fail_testcase.end (); it_fail++){
            printf("Fail function: %d,  \n", (*it_fail));
        }
    } else {
        printf("NO fail test function.\n");
    }

    printf("----------------------------------------\n");
    exit (EXIT_NORMAL);
}
