#include <stdlib.h>
#include <iostream>
#include <vector>
#include "./forest_common.h"
#include "./frog_internals.h"
#include "./frog_if.h"
#include "./forest_utility.h"
#include "./frog_internals.h"

uint64_t cb_mem_value = 1, global_cnt_pass = 0, global_cnt_fail = 0;
std::string cb_access_type, cb_access_bus, cb_load_action;
std::string g_get_opcode;
std::string g_target_python_name;

void cb_update_generator_memory(uint64_t address, uint32_t size, uint64_t value, uint64_t prev_value,
        const uint32_t access_type, const uint32_t access_bus, const uint32_t load_action) {
    std::string str_access_type, str_access_bus, str_load_action;
    cb_mem_value = value;

    if (access_type == FROG_ACCESS_TYPE_READ) {
        str_access_type = "read";
    } else if (access_type == FROG_ACCESS_TYPE_WRITE) {
        str_access_type = "write";
    }
    cb_access_type = str_access_type;
    if (access_bus == FROG_ACCESS_BUS_RW) {
        str_access_bus = "RW";
    } else if (access_bus == FROG_ACCESS_BUS_FETCH) {
        str_access_bus = "Fetch";
    }else if (access_bus == FROG_ACCESS_BUS_API) {
        str_access_bus = "API";
    }
    cb_access_bus = str_access_bus;
    if (load_action == FROG_LOAD_ACTION_LOAD) {
        str_load_action = "Load";
    } else if (load_action == FROG_LOAD_ACTION_FETCH) {
        str_load_action = "Fetch";
    }
    cb_load_action = str_load_action;

    printf ("<< Callback memory: address %lx, size %d, value %lx, prev_value %lx, access_type %s, access_bus %s, load_action %s >> \n",
                 address, size, value, prev_value, str_access_type.c_str(), str_access_bus.c_str(), str_load_action.c_str());
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

void display_local(int32_t num_pass, int32_t num_fail)
{
    printf("******************************************\n");
    printf("Pattern FAIL = %d.\n", num_fail);
    printf("Pattern PASS = %d.\n", num_pass);
    printf("******************************************\n\n");
}

void display_global(int32_t total_pass, int32_t total_fail)
{
    printf("-----------------SUMMARY------------------\n");
    printf("Total FAIL = %d.\n", total_fail);
    printf("Total PASS = %d.\n", total_pass);
    printf("------------------------------------------\n\n");
}

int init_seting()
{
    GCS_CB_INFO_EX      pCbInfo;
    GCS_HW_INFO_EX *    pHwInfo = new GCS_HW_INFO_EX();
    pCbInfo.fpExM    = cb_update_generator_memory;
    pCbInfo.fpExNR   = cb_update_nc_register;
    pCbInfo.fpExTR   = cb_update_tc_register;
    pCbInfo.fpExVR   = cb_update_vc_register;
    pCbInfo.fpL      = cb_update_llbit;
    pCbInfo.fpExTLB  = cb_update_tlb;
    pCbInfo.fpC      = cb_comment;

    int err = gcs_initialize_simulator_ex(MSG_INF, &pCbInfo, g_target_python_name, pHwInfo);
    return err;
}

bool set_up_simulator_item_1_1()
{
    int fail_local = 0, pass_local = 0;
    int err=init_seting();
    if (err != GCSIF_SUCCESS) {
        printf ("Initialize error \n");
        printf ("===> gcs_initialize_simulator_ex_item_1_1: FAIL\n");
        global_cnt_fail++;
        fail_local++;
        display_local(pass_local,fail_local);
        return false;
    }

    printf ("===> gcs_initialize_simulator_ex_item_1_1: PASS\n");
    pass_local++;
    display_local(pass_local,fail_local);
    global_cnt_pass++;
    return true;
}

bool disable_simulator_item_4_1()
{
    int fail_local = 0, pass_local = 0;
    int err=gcs_disable_simulator_stop(0);
    if (err != GCSIF_SUCCESS)
    {
        printf ("===> gcs_disable_simulator_stop_item_4_1: FAIL\n");
        global_cnt_fail++;
        fail_local++;
        display_local(pass_local,fail_local);
        return false;
    }
    printf ("===> gcs_disable_simulator_stop_item_4_1: PASS\n");
    pass_local++;
    display_local(pass_local,fail_local);
    global_cnt_pass++;
    return true;
}

bool reset_simulator_item_3_1()
{
    int fail_local = 0, pass_local = 0;
    init_seting();
    int err=gcs_reset_simulator();
    if (err == GCSIF_SUCCESS)
    {
        printf ("===> gcs_reset_simulator_item_3_1: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_reset_simulator_item_3_1: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;
}

bool reset_simulator_item_3_2()
{
    int fail_local = 0, pass_local = 0;
    int err=gcs_reset_simulator();
    if (err == GCSIF_ERR_ALLOCATE_MEMORY)
    {
        printf ("===> gcs_reset_simulator_item_3_2: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_reset_simulator_item_3_2: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;
}

bool write_memory_item_5_1 ()
{
    int fail_local = 0, pass_local = 0;
    extern ForestUtil* p_forest_util;
    int err = 0;
    //uint64_t cforest_read_value;
    std::string write_value[4] = {"0xCA",
                                  "0xFE,0xCA",
                                  "0xCA,0xFE,0xCA",
                                  "0xFE,0xCA,0xFE,0xCA"};
    uint32_t read_value[4]={0,
                            0,
                            0,
                            0};

    uint32_t write_address[4]={0x00000000,
                               0xfea0ffff,
                               0xfeab0000,
                               0xfebffffb};

/* // Convert string to hex
    uint32_t hexNumber;
    sscanf(write_value[0].c_str(), "%x", &hexNumber);
    printf("%x\n",hexNumber);
*/

    for (int i = 0; i <=3 ; i ++)
    {
        for (int j = 0; j<=3; j++)
        {
            err = gcs_write_simulator_memory(write_address[i], 0x1+j , write_value[j] );
            if (err != GCSIF_SUCCESS)
            {
                printf ("Error while writing to MEM.\n");
                printf ("===> gcs_write_simulator_memory_item_5_1: FAIL\n");
                global_cnt_fail++;
                fail_local++;
                display_local(pass_local,fail_local);
                return false;
            }
            else
            {
                /*
                 * Check by use function in CForest to Read the value that written
                 */
                read_value[j] = p_forest_util->VMemReadDebug(0, write_address[i], 1+j);
                //cforest_read_value = read_value[j];
                //if (cforest_read_value == static_cast<uint32_t>(cb_mem_value))
                if (read_value[j] == static_cast<uint32_t>(cb_mem_value))
                {
                    printf ("Write %d byte success ", j+1);
                    printf ("===> gcs_write_simulator_memory_item_5_1: PASS\n");
                    global_cnt_pass++;
                    pass_local++;
                    display_local(pass_local,fail_local);
                }
                else
                {
                      printf("Compare mismatch.\n");
                    //printf("Read  value = %lx\n", cforest_read_value);
                      printf("Read  value = %x\n", read_value[j]);
                    printf("Write value = %x\n", static_cast<uint32_t>(cb_mem_value));
                    printf ("===> gcs_write_simulator_memory_item_5_1: FAIL\n");
                    global_cnt_fail++;
                    fail_local++;
                    display_local(pass_local,fail_local);
                    return false;
                }
            }
        }
    }
    return true;
}

bool write_memory_item_5_2 ()
{
    int fail_local = 0, pass_local = 0;
    int err = 0;
    err = gcs_write_simulator_memory(0xfea00000, 0x1, "0xCA");
    if (err == GCSIF_ERR_NOT_INITIALIZED)
    {
        printf ("Initialize error \n");
        printf ("===> gcs_write_simulator_memory_item_5_2: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_write_simulator_memory_item_5_2: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;
}

bool write_memory_item_5_3_1 ()
{
    int fail_local = 0, pass_local = 0;
    int err = 0;
    err = gcs_write_simulator_memory (0xfea00000, 0x4, "");  // Invalid argument is assigned
    if (err == GCSIF_ERR_INVALID_OTHER_PARAM)
    {
        printf ("Bytes is empty.\n");
        printf ("===> gcs_write_simulator_memory_item_5_3_1: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_write_simulator_memory_item_5_3_1: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;
}

bool write_memory_item_5_3_2 ()
{
    int fail_local = 0, pass_local = 0;
    int err = 0;
    err = gcs_write_simulator_memory (0xfea00000, 0, "0xCA, 0xFE, 0xCA, 0xFE"); // Invalid argument is assigned
    if (err == GCSIF_ERR_INVALID_OTHER_PARAM)
    {
        printf ("Size is zero.\n");
        printf ("===> gcs_write_simulator_memory_item_5_3_2: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_write_simulator_memory_item_5_3_2: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;
}

bool read_memory_item_6_1 ()
{
    int fail_local = 0, pass_local = 0;
    extern ForestUtil* p_forest_util;
    int err = 0;
    uint32_t cforest_write_value;
    uint32_t write_value[4] = {0xCA,
                               0xCAFE,
                               0xCAFECA,
                               0xCAFECAFE};

    std::string read_value;

    uint32_t write_address[4]={0x00000000,
                               0xfea0ffff,
                               0xfeab0000,
                               0xfebffffb};

    for (int i = 0; i <=3 ; i ++)
    {
        for (int j = 0; j<=3 ; j++)
        {
            /*
             * Check by use gcs function to Read the value that written by CForest function
             */
            p_forest_util->VMemWriteDebug(0, write_address[i], write_value[j], 1+j);
            err = gcs_read_simulator_memory(write_address[i], 0x1+j, read_value);
            cforest_write_value = write_value[j];

            if (err != GCSIF_SUCCESS)
            {
                printf ("Error while reading from MEM.%x\n", err);
                printf ("===> gcs_read_simulator_memory_item_6_1: FAIL\n");
                global_cnt_fail++;
                fail_local++;
                display_local(pass_local,fail_local);
                return false;
            }

            if (cforest_write_value == static_cast<uint32_t>(cb_mem_value))
            {
                printf ("===> gcs_read_simulator_memory_item_6_1: PASS\n\n");
                global_cnt_pass++;
                pass_local++;
                display_local(pass_local,fail_local);
            }
            else
            {
                printf("Compare mismatch.\n");
                printf("Write value = %x\n", cforest_write_value);
                printf("Read  value = %x\n", static_cast<uint32_t>(cb_mem_value));
                printf ("===> gcs_read_simulator_memory_item_6_1: FAIL\n");
                global_cnt_fail++;
                fail_local++;
                display_local(pass_local,fail_local);
                return false;
            }
            read_value.clear();
        }
    }
    return true;
}

bool read_memory_item_6_2 ()
{
    int fail_local = 0, pass_local = 0;
    int err = 0;
    std::string bytes_read;

    err = gcs_read_simulator_memory(0xfea00000, 0x1, bytes_read);
    if (err == GCSIF_ERR_NOT_INITIALIZED)
    {
        printf ("Initialize error \n");
        printf ("===> gcs_read_simulator_memory_item_6_2: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_read_simulator_memory_item_6_2: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;
}

bool read_memory_item_6_3_1 ()
{
    int fail_local = 0, pass_local = 0;
    int err = 0;
    std::string bytes_read = "0xCA";
    extern ForestUtil* p_forest_util;

    p_forest_util->VMemWriteDebug(0, 0xfea00000, 0xCA, 1);
    err = gcs_read_simulator_memory (0xfea00000, 0x4, bytes_read);
    if (err == GCSIF_ERR_INVALID_OTHER_PARAM)
    {
        printf ("Bytes is NOT empty \n");
        printf ("===> gcs_read_simulator_memory_item_6_3_1: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_read_simulator_memory_item_6_3_1: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;
}

bool read_memory_item_6_3_2 ()
{
    int fail_local = 0, pass_local = 0;
    int err = 0;
    std::string bytes_read;
    extern ForestUtil* p_forest_util;

    p_forest_util->VMemWriteDebug(0, 0xfea00000, 0xCA, 1);
    err = gcs_read_simulator_memory (0xfea00000, 0, bytes_read);
    if (err == GCSIF_ERR_INVALID_OTHER_PARAM)
    {
        printf ("Size is zero.\n");
        printf ("===> gcs_read_simulator_memory_item_6_3_2: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_read_simulator_memory_item_6_3_2: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;
}

bool write_register_NC_item_7_1 ()
{
    int fail_local = 0, pass_local = 0;
    extern ForestUtil* p_forest_util;
    extern std::map<std::string, FrogRegPro*> frog_reg_map;
    uint64_t cforest_read_value = 1, temp_u64_cforest = 0, temp_cforest = 0;

    std::string g_reg[32] = {"r0", "r1", "r2", "r3",
                           "r4", "r5", "r6", "r7",
                           "r8", "r9", "r10", "r11",
                           "r12", "r13", "r14", "r15",
                           "r16", "r17", "r18", "r19",
                           "r20", "r21", "r22", "r23",
                           "r24", "r25", "r26", "r27",
                           "r28", "r29", "r30", "r31"}; //General register

    std::string w_reg[32] = {"wr0", "wr1", "wr2", "wr3",
                             "wr4", "wr5", "wr6", "wr7",
                             "wr8", "wr9", "wr10", "wr11",
                             "wr12", "wr13", "wr14", "wr15",
                             "wr16", "wr17", "wr18", "wr19",
                             "wr20", "wr21", "wr22", "wr23",
                             "wr24", "wr25", "wr26", "wr27",
                             "wr28", "wr29", "wr30", "wr31"}; // Vector register G4MH

    std::string sys_reg[] =     {"EIPC",     "EIPSW",    "FEPC",       "FEPSW",    "PSW",
                               "FPSR",     "FPEPC",    "FPST",     "FPCC",       "FPCFG",
                               "EIIC",     "FEIC",     "CTPC",     "CTPSW",    "CTBP",
                               "SNZCFG",   "EIWR",     "FEWR",     "SPID",     "SPIDLIST",
                               "RBASE",    "EBASE",    "INTBP",    "MCTL",     "PID",
                               "SVLOCK",   "SCCFG",    "SCBP",     "PEID",     "BMID",
                               "MEA",      "MEI",      "RBCR0",    "RBCR1",    "RBNR",
                               "RBIP",     "ISPR",     "ICSR",     "INTCFG",   "PLMR",
                               "FXSR",     "FXST",     "FXINFO",   "FXCFG",    "FXXC",
                               "FXXP",     "MPM",      "MPCFG",    "MCA",      "MCS",
                               "MCC",      "MCR",      "MCI",      "MPIDX",    "MPLA",
                               "MPUA",     "MPAT",     "MPID0",    "MPID1",    "MPID2",
                               "MPID3",    "MPID4",    "MPID5",    "MPID6",    "MPID7",
                               "ICTAGL",   "ICTAGH",   "ICDATL",   "ICDATH",   "ICCTRL",
                               "ICCFG",    "ICERR",    "TSCOUNTL", "TSCOUNTH", "TSCTRL",
                               "PMUMCTRL", "PMCOUNT0", "PMCTRL0",  "PMCOUNT1", "PMCTRL1",
                               "PMCOUNT2", "PMCTRL2",  "PMCOUNT3", "PMCTRL3",  /*"PMCOUNT4",
                               "PMCTRL4",  "PMCOUNT5", "PMCTRL5",  "PMCOUNT6", "PMCTRL6",
                               "PMCOUNT7", "PMCTRL7",*/  "DBIC",
                               "DBPC",     "DBPSW",    "DIR0",     "DIR1",     "BPC",
                               "BPAV",     "BPAM",     "DBWR",     "LSTEST0",  "LSTEST1",
                               "IFCR",     "BRPCTRL0", "BRPCTRL1", "BRPCFG",   "BRPACTRL",
                               "BRPDATA",  "LSUCR",    "RDBCR",    "RDBACR",   "RDBATAG",
                               "RDBADAT0", "RDBADAT1", "RDBADAT2", "RDBADAT3",
                               "GMEIPC",   "GMEIPSW",  "GMFEPC",   "GMEIIC",   "GMFEIC",
                               "GMEIWR",   "GMFEWR",   "GMSPID",   "GMSPIDLIST", "GMEBASE",
                               "GMINTBP",  "GMINTCFG", "GMPLMR",   "GMMPM",    /*"GMMPCFG",*/
                               "GMEIPSW",  "GMFEPSW",  "GMPSW",    "PMGMCTRL", "DBPSWH",
                               "HVCFG",    "GMCFG",    "PSWH",     "FEPSWH",   "EIPSWH",
                               "HVSB",     "MPBK",     "DBGEN",    "MPAT23",   "MPLA23", "MPUA23",
                               "MPAT31",   "MPLA31",   "MPUA31",   "MPAT0",   "MPLA0", "MPUA0",
                               "MPAT23_B1",   "MPLA23_B1", "MPUA23_B1",
                               "MPAT31_B1",   "MPLA31_B1",   "MPUA31_B1",   "MPAT0_B1",   "MPLA0_B1", "MPUA0_B1",};

    uint32_t write_value[7] =    {0x00000000,
                                  0x11112222,
                                  0xffff0000,
                                  0xffffffff,
                                  0xcafecafe,
                                  0x0000cafe,
                                  0x1111ffff};

    uint64_t write_128_l[7] = {0x3333444455556666,
                               0x1111222233334444,
                               0x5555666677778888,
                               0xffffffff00000000,
                               0xcafecafecafecafe,
                               0x00ca00fe00ca00fe,
                               0x1111ffff1111ffff};

    uint64_t write_128_u[7] = {0x3333444455556666,
                                  0x1111222233334444,
                                  0x5555666677778888,
                               0xffffffff00000000,
                               0xcafecafecafecafe,
                               0x00ca00fe00ca00fe,
                               0x1111ffff1111ffff};

    int err = 0;
    for (int i = 0; i<=31; i++)
    {
        std::string upper; // (boost::to_upper_copy<std::string>(g_reg[i]));
        ToUpperCopy (g_reg[i], &upper);
        std::map<std::string, FrogRegPro*>::iterator it_frog_reg_map = frog_reg_map.find(upper);
        FrogRegPro * reg_profile = it_frog_reg_map->second;

        if (g_reg[i]=="r0") //Check if R0 value is equal with 0
        {
            for (int k = 0; k<=6; k++)
            {
                err = gcs_set_nc_register(g_reg[i],write_value[k]);
                if (err != GCSIF_SUCCESS)
                {
                    printf ("Error while writing to g_reg. \n");
                    printf("===> gcs_set_nc_register_item_7_1a: FAIL\n");
                    global_cnt_fail++;
                    fail_local++;
                    display_local(pass_local,fail_local);
                    return false;
                }
                p_forest_util->RegReadDebug(0, reg_profile->m_creg, &temp_cforest, &temp_u64_cforest );
                cforest_read_value = temp_cforest;
                if (cforest_read_value == 0)
                {
                    global_cnt_pass++;
                    pass_local++;
                    printf("Zero register R0 = 0 ===> PASS\n");
                    display_local(pass_local,fail_local);
                }
            }
        }
        else
        {
            for (int j = 0; j<=6; j++)
            {
                err = gcs_set_nc_register(g_reg[i], write_value[j]);
                if (err != GCSIF_SUCCESS)
                {
                    printf ("Error while writing to g_reg. \n");
                    printf("===> gcs_set_nc_register_item_7_1b: FAIL\n");
                    global_cnt_fail++;
                    fail_local++;
                    display_local(pass_local,fail_local);
                    return false;
                }
                p_forest_util->RegReadDebug(0, reg_profile->m_creg, &temp_cforest, &temp_u64_cforest );
                cforest_read_value = temp_cforest;

                if (write_value[j] == static_cast<uint32_t>(cforest_read_value)) //Check Read-Write value
                {
                    global_cnt_pass++;
                    pass_local++;
                    printf("===> PASS\n");
                    display_local(pass_local,fail_local);
                }
                else
                {
                    printf("Compare mismatch.\n");
                    printf("Read  value = %x\n", static_cast<uint32_t>(cforest_read_value));
                    printf("Write value = %x\n", write_value[j]);
                    printf("===> gcs_set_nc_register_item_7_1c: FAIL\n");
                    global_cnt_fail++;
                    fail_local++;
                    display_local(pass_local,fail_local);
                    return false;
                }
               }
        }
    }

    for (int i = 0; i<=31; i++)
    {
        std::string upper_wr; // (boost::to_upper_copy<std::string>(w_reg[i]));
        ToUpperCopy (w_reg[i], &upper_wr);
        std::map<std::string, FrogRegPro*>::iterator it_frog_reg_map_wr = frog_reg_map.find(upper_wr);
        FrogRegPro * reg_profile_wr = it_frog_reg_map_wr->second;

        for (int j = 0; j<=6; j++)
        {
            FrogRegData write_128_value = (static_cast<FrogRegData>(write_128_u[j]) << 64) | static_cast<FrogRegData>(write_128_l[j]);
            err = gcs_set_nc_register(w_reg[i], write_128_value);
            if (err != GCSIF_SUCCESS)
            {
                printf ("Error while writing to REG. \n");
                printf("===> gcs_set_nc_register_item_7_1d: FAIL\n");
                global_cnt_fail++;
                fail_local++;
                display_local(pass_local,fail_local);
                return false;
            }
            p_forest_util->RegReadDebug(0, reg_profile_wr->m_creg, &temp_cforest, &temp_u64_cforest );

            if (write_128_l[j] == temp_cforest)
             {
                if (write_128_u[j] == temp_u64_cforest)
                {
                    global_cnt_pass++;
                    pass_local++;
                    printf("===> PASS\n");
                    display_local(pass_local,fail_local);
                }
                else
                {
                      printf("Compare mismatch.\n");
                    printf("Read  value up = %lx\n", write_128_u[j]);
                       printf("Write value up = %lx\n", temp_u64_cforest);
                      printf("===> gcs_set_nc_register_item_7_1e: FAIL\n");
                    global_cnt_fail++;
                    fail_local++;
                    display_local(pass_local,fail_local);
                       return false;
                }
            }
            else
            {
                  printf("Compare mismatch.\n");
                printf("Read  value low = %lx\n", write_128_l[j]);
                   printf("Write value low = %lx\n", temp_cforest);
                  printf("===> gcs_set_nc_register_item_7_1f: FAIL\n");
                global_cnt_fail++;
                fail_local++;
                display_local(pass_local,fail_local);
                   return false;
            }
        }
    }

    for (unsigned int i = 0; i<sizeof(sys_reg)/sizeof(*sys_reg); i++) //To find the number of element in array: num = sizeof(array)/sizeof(*array)
    {
        for (unsigned int j = 0; j<=6; j++)
        {
            err = gcs_set_nc_register(sys_reg[i], write_value[j]);
            if (err != GCSIF_SUCCESS)
            {
                printf ("Error while writing to REG. \n");
                printf("===> gcs_set_nc_register_item_7_1g: FAIL\n");
                global_cnt_fail++;
                fail_local++;
                display_local(pass_local,fail_local);
                return false;
            }
            for (unsigned int k = 0; k<sizeof(sys_reg)/sizeof(*sys_reg); k++) //To find the number of element in array: num = sizeof(array)/sizeof(*array)
            {
                FrogRegData read_value;
                err = gcs_get_nc_register(sys_reg[k], &read_value);
                if (err != GCSIF_SUCCESS)
                {
                    printf ("Error while read to REG: %s \n", sys_reg[k].c_str());
                    printf("===> gcs_get_nc_register_item_7_1h: FAIL\n");
                    global_cnt_fail++;
                    fail_local++;
                    display_local(pass_local,fail_local);
                    return false;
                }
            }
            global_cnt_pass++;
            pass_local++;
            printf ("===> PASS\n");
            display_local(pass_local,fail_local);
        }
    }
    printf ("===> gcs_set_nc_register_item_7_1: PASS\n");
    return true;
}

bool write_register_NC_item_7_2 ()
{
    int fail_local = 0, pass_local = 0;
    uint32_t err = 0;
    FrogRegData value = 0xcafecafe;
    err = gcs_set_nc_register("r16", value);
    if (err == GCSIF_ERR_NOT_INITIALIZED)
    {
        printf ("Initialize error \n");
        printf("===> gcs_set_nc_register_item_7_2: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_set_nc_register_item_7_2: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;

}

bool write_register_NC_item_7_3()
{
    int fail_local = 0, pass_local = 0;
    int err = 0;
    FrogRegData value = 0xcafecafe;
    err = gcs_set_nc_register ("r32", value);
    if (err == GCSIF_ERR_INVALID_REGISTER_NAME)
    {
        printf ("Can't find reg name. \n");
        printf("===> gcs_set_nc_register_item_7_3: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_set_nc_register_item_7_3: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;
}

bool read_register_NC_item_8_1 ()
{
    int fail_local = 0, pass_local = 0;
    FrogRegData cforest_written_value = 0x1111ffff, read_value;
    uint32_t err = 0;
    std::string g_reg[32] = {"r0", "r1", "r2", "r3",
                           "r4", "r5", "r6", "r7",
                           "r8", "r9", "r10", "r11",
                           "r12", "r13", "r14", "r15",
                           "r16", "r17", "r18", "r19",
                           "r20", "r21", "r22", "r23",
                           "r24", "r25", "r26", "r27",
                           "r28", "r29", "r30", "r31"};

    std::string w_reg[32] = {"wr0", "wr1", "wr2", "wr3",
                             "wr4", "wr5", "wr6", "wr7",
                             "wr8", "wr9", "wr10", "wr11",
                             "wr12", "wr13", "wr14", "wr15",
                             "wr16", "wr17", "wr18", "wr19",
                             "wr20", "wr21", "wr22", "wr23",
                             "wr24", "wr25", "wr26", "wr27",
                             "wr28", "wr29", "wr30", "wr31"};

    std::string sys_reg[] =     {"EIPC",     "EIPSW",    "FEPC",       "FEPSW",    "PSW",
                               "FPSR",     "FPEPC",    "FPST",     "FPCC",       "FPCFG",
                               "EIIC",     "FEIC",     "CTPC",     "CTPSW",    "CTBP",
                               "SNZCFG",   "EIWR",     "FEWR",     "SPID",     "SPIDLIST",
                               "RBASE",    "EBASE",    "INTBP",    "MCTL",     "PID",
                               "SVLOCK",   "SCCFG",    "SCBP",     "PEID",     "BMID",
                               "MEA",      "MEI",      "RBCR0",    "RBCR1",    "RBNR",
                               "RBIP",     "ISPR",     "ICSR",     "INTCFG",   "PLMR",
                               "FXSR",     "FXST",     "FXINFO",   "FXCFG",    "FXXC",
                               "FXXP",     "MPM",      "MPCFG",    "MCA",      "MCS",
                               "MCC",      "MCR",      "MCI",      "MPIDX",    "MPLA",
                               "MPUA",     "MPAT",     "MPID0",    "MPID1",    "MPID2",
                               "MPID3",    "MPID4",    "MPID5",    "MPID6",    "MPID7",
                               "ICTAGL",   "ICTAGH",   "ICDATL",   "ICDATH",   "ICCTRL",
                               "ICCFG",    "ICERR",    "TSCOUNTL", "TSCOUNTH", "TSCTRL",
                               "PMUMCTRL", "PMCOUNT0", "PMCTRL0",  "PMCOUNT1", "PMCTRL1",
                               "PMCOUNT2", "PMCTRL2",  "PMCOUNT3", "PMCTRL3",  /*"PMCOUNT4",
                               "PMCTRL4",  "PMCOUNT5", "PMCTRL5",  "PMCOUNT6", "PMCTRL6",
                               "PMCOUNT7", "PMCTRL7",*/  "DBIC",
                               "DBPC",     "DBPSW",    "DIR0",     "DIR1",     "BPC",
                               "BPAV",     "BPAM",     "DBWR",     "LSTEST0",  "LSTEST1",
                               "IFCR",     "BRPCTRL0", "BRPCTRL1", "BRPCFG",   "BRPACTRL",
                               "BRPDATA",  "LSUCR",    "RDBCR",    "RDBACR",   "RDBATAG",
                               "RDBADAT0", "RDBADAT1", "RDBADAT2", "RDBADAT3",
                               "GMEIPC",   "GMEIPSW",  "GMFEPC",   "GMEIIC",   "GMFEIC",
                               "GMEIWR",   "GMFEWR",   "GMSPID",   "GMSPIDLIST", "GMEBASE",
                               "GMINTBP",  "GMINTCFG", "GMPLMR",   "GMMPM",    /*"GMMPCFG",*/
                               "GMEIPSW",  "GMFEPSW",  "GMPSW",    "PMGMCTRL", "DBPSWH",
                               "HVCFG",    "GMCFG",    "PSWH",     "FEPSWH",   "EIPSWH",
                               "HVSB",     "MPBK",     "DBGEN",    "MPAT23",   "MPLA23", "MPUA23",
                               "MPAT31",   "MPLA31",   "MPUA31"};

    for (int i = 0; i<=31; i++)
    {
        err = gcs_get_nc_register(g_reg[i],&read_value);
        if (err != GCSIF_SUCCESS)
        {
            printf ("Error while reading from REG: %s \n", g_reg[i].c_str());
            printf("===> gcs_get_nc_register_item_8_1: FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
        if (g_reg[i]=="r0")
        {
            if (read_value ==0)
            {
                printf ("Zero register: %s\n", g_reg[i].c_str());
                printf ("===> PASS\n");
                global_cnt_pass++;
                pass_local++;
                display_local(pass_local,fail_local);
            }
        }
        else
        {
            if (read_value == cforest_written_value)
            {
                printf ("Read register: %s\n", g_reg[i].c_str());
                printf ("===> PASS\n");
                global_cnt_pass++;
                pass_local++;
                display_local(pass_local,fail_local);
            }
            else
            {
                printf("===> gcs_get_nc_register_item_8_1: FAIL\n");
                global_cnt_fail++;
                fail_local++;
                display_local(pass_local,fail_local);
                return false;
            }
        }
    }

    for (int i = 0; i<=31; i++)
    {
        err = gcs_get_nc_register(w_reg[i],&read_value);
        uint64_t data_l64 = static_cast <uint64_t> (read_value & 0xFFFFFFFFFFFFFFFFULL);  //To use 128bit --> 2*64bit
        uint64_t data_u64 = static_cast <uint64_t> ((read_value >> 64) & 0xFFFFFFFFFFFFFFFFULL);

        if (err != GCSIF_SUCCESS)
        {
            printf ("Error while reading from REG. \n");
            printf("===> gcs_get_nc_register_item_8_1: FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
        else
        {
            if (data_l64 == 0x1111ffff1111ffff)
            {
                if (data_u64 == 0x1111ffff1111ffff)
                {
                    printf ("Read register: %s\n", w_reg[i].c_str());
                    printf ("===> PASS\n");
                    global_cnt_pass++;
                    pass_local++;
                    display_local(pass_local,fail_local);
                }
                else
                {
                    printf ("Error while reading 64bits-up value from REG. \n");
                    printf("===> gcs_get_nc_register_item_8_1: FAIL\n");
                    global_cnt_fail++;
                    fail_local++;
                    display_local(pass_local,fail_local);
                    return false;
                }
            }
            else
            {
                printf ("Error while reading 64bits-low value from REG. \n");
                printf("===> gcs_get_nc_register_item_8_1: FAIL\n");
                global_cnt_fail++;
                fail_local++;
                display_local(pass_local,fail_local);
                return false;
            }
        }

    }

    for (unsigned int i = 0; i<sizeof(sys_reg)/sizeof(*sys_reg); i++)
    {
        err = gcs_get_nc_register(sys_reg[i],&read_value);
        if (err != GCSIF_SUCCESS)
        {
            printf ("Error while reading from REG. \n");
            printf("===> gcs_get_nc_register_item_8_1: FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
        printf ("Read register: %s\n", sys_reg[i].c_str());
        printf ("===> PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
    }
    printf ("==> gcs_get_nc_register_item_8_1: PASS\n\n");
    return true;
}

bool read_register_NC_item_8_2 ()
{
    int fail_local = 0, pass_local = 0;
    uint32_t err = 0;
    FrogRegData value;
    err = gcs_get_nc_register ("r16", &value);
    if (err == GCSIF_ERR_NOT_INITIALIZED)
    {
        printf ("Initialize error \n");
        printf("===> gcs_get_nc_register_item_8_2: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_get_nc_register_item_8_2: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;
}

bool read_register_NC_item_8_3()
{
    int fail_local = 0, pass_local = 0;
    int err = 0;
    FrogRegData value;
    err = gcs_get_nc_register ("r32", &value);
    if (err == GCSIF_ERR_INVALID_REGISTER_NAME)
    {
        printf ("Can't find reg name. \n");
        printf("===> gcs_get_nc_register_item_8_3: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_get_nc_register_item_8_3: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;
}

bool create_llbit_item_9_1 ()
{
    int fail_local = 0, pass_local = 0;
    int err = 0;
    uint64_t address[2] = {0xfea00000, 0xfebfffff};
    bool isNC = true;
    for (int i=0; i<=1; i++)
    {
        err = gcs_create_memory_link (address[i], isNC, 0, 4);
        if (err != GCSIF_SUCCESS)
        {
            printf ("Error. %x\n", err);
            printf("===> gcs_create_memory_link_item_9_1: FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
        //printf ("Address: %lx", address[i]);
        printf ("===> gcs_create_memory_link_item_9_1: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
    }
    return true;
}

bool create_llbit_item_9_2 ()
{
    int fail_local = 0, pass_local = 0;
    int err = 0;
    bool isNC = true;
    err = gcs_create_memory_link (0xfea00000, isNC, 0, 4);
    if (err == GCSIF_ERR_NOT_INITIALIZED)
    {
        printf ("Initialize error. \n");
        printf("===> gcs_create_memory_link_item_9_2: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_create_memory_link_item_9_2: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;
}

bool create_llbit_item_9_3 ()
{
    int fail_local = 0, pass_local = 0;
    int err = 0;
    uint64_t address[2] = {0xfea00000, 0xfebfffff};
    bool isNC = true;
    for (int i=0; i<=1; i++)
    {
        err = gcs_create_memory_link (address[i], isNC, 64, 4);
        if (err == GCSIF_ERR_INVALID_TCID)
        {
            printf ("TCID Error. \n");
            printf("===> gcs_create_memory_link_item_9_3: PASS\n");
            global_cnt_pass++;
            pass_local++;
            display_local(pass_local,fail_local);
        }
        else
        {
            printf ("===> gcs_create_memory_link_item_9_3: FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
    }
    return true;
}

bool remove_llbit_item_10_1 ()
{
    int fail_local = 0, pass_local = 0;
    int err = 0;
    uint64_t address[2] = {0xfea00000, 0xfebfffff};
    for (int i=0; i<=1; i++)
    {
        err = gcs_remove_memory_link_addr (address[i]);
        if (err != GCSIF_SUCCESS)
        {
            printf ("Error. %x\n", err);
            printf("===> gcs_remove_memory_link_addr_item_10_1: FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
        printf ("===> gcs_remove_memory_link_addr_item_10_1: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
    }
    return true;
}

bool remove_llbit_item_10_2 ()
{
    int fail_local = 0, pass_local = 0;
    int err = 0;
    err = gcs_remove_memory_link_addr (0xfea00000);
    if (err == GCSIF_ERR_NOT_INITIALIZED)
    {
        printf ("Initialize error. \n");
        printf("===> gcs_remove_memory_link_addr_item_10_2: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_remove_memory_link_addr_item_10_2: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;
}

bool get_simulator_mode_item_11_1 ()
{
    int fail_local = 0, pass_local = 0;
    int err = 0;
    uint32_t mode;
    err = gcs_is_simulator_native(&mode);
    if (err != GCSIF_SUCCESS)
    {
        printf ("Error. %x\n", err);
        printf("===> gcs_is_simulator_native_item_11_1: FAIL\n");
        global_cnt_fail++;
        fail_local++;
        display_local(pass_local,fail_local);
        return false;
    }
    printf ("Mode is %x\n",mode);
    printf ("===> gcs_is_simulator_native_item_11_1: PASS\n");
    global_cnt_pass++;
    pass_local++;
    display_local(pass_local,fail_local);
    return true;
}

bool get_simulator_mode_item_11_2 ()
{
    int fail_local = 0, pass_local = 0;
    int err = 0;
    uint32_t mode;
    err = gcs_is_simulator_native(&mode);
    if (err == GCSIF_ERR_NOT_INITIALIZED)
    {
        printf ("Initial error. \n");
        printf("===> gcs_is_simulator_native_item_11_2: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_is_simulator_native_item_11_2: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;
}

bool check_fxu_item_12_1 ()
{
    int fail_local = 0, pass_local = 0;
    bool err;
    err = gcs_check_fxu_function();
    if (err != true)
    {
        printf ("Error. %x\n", err);
        printf("===> gcs_check_fxu_function_item_12_1: FAIL\n");
        global_cnt_fail++;
        fail_local++;
        display_local(pass_local,fail_local);
        return false;
    }
    printf ("Support FXU functions.\n");
    printf ("===> gcs_check_fxu_function_item_12_1: PASS\n");
    global_cnt_pass++;
    pass_local++;
    display_local(pass_local,fail_local);
    return true;
}

bool check_fxu_item_12_2 ()
{
    int fail_local = 0, pass_local = 0;
    bool err;
    err = gcs_check_fxu_function();
    if (err == false)
    {
        printf ("Initial error. \n");
        printf("===> gcs_check_fxu_function_item_12_2: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_check_fxu_function_item_12_2: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;
}

bool check_fpu_item_12_3 ()
{
    int fail_local = 0, pass_local = 0;
    bool err;
    err = gcs_check_fpu_function();
    if (err != true)
    {
        printf ("Error. %x\n", err);
        printf("===> gcs_check_fpu_function_item_12_3: FAIL\n");
        global_cnt_fail++;
        fail_local++;
        display_local(pass_local,fail_local);
        return false;
    }
    printf ("Support FPU functions.\n");
    printf ("===> gcs_check_fxu_function_item_12_3: PASS\n");
    global_cnt_pass++;
    pass_local++;
    display_local(pass_local,fail_local);
    return true;
}

bool check_fpu_item_12_4 ()
{
    int fail_local = 0, pass_local = 0;
    bool err;
    err = gcs_check_fpu_function();
    if (err == false)
    {
        printf ("Initial error. \n");
        printf("===> gcs_check_fpu_function_item_12_4: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_check_fpu_function_item_12_4: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;
}

bool get_reg_inf_item_13_1 ()
{
    int fail_local = 0, pass_local = 0;
    bool err;
    FrogRegPro out;
    std::string g_reg[32] =     {"r0", "r1", "r2", "r3",
                               "r4", "r5", "r6", "r7",
                               "r8", "r9", "r10", "r11",
                               "r12", "r13", "r14", "r15",
                               "r16", "r17", "r18", "r19",
                               "r20", "r21", "r22", "r23",
                               "r24", "r25", "r26", "r27",
                               "r28", "r29", "r30", "r31"};

    std::string wr_reg[32] =  {"wr0", "wr1", "wr2", "wr3",
                               "wr4", "wr5", "wr6", "wr7",
                               "wr8", "wr9", "wr10", "wr11",
                               "wr12", "wr13", "wr14", "wr15",
                               "wr16", "wr17", "wr18", "wr19",
                               "wr20", "wr21", "wr22", "wr23",
                               "wr24", "wr25", "wr26", "wr27",
                               "wr28", "wr29", "wr30", "wr31"};

    std::string sreg_sel0[18] = {"EIPC", "EIPSW", "FEPC",  "FEPSW",  "PSW",
                               "FPSR", "FPEPC", "FPST",  "FPCC",   "FPCFG",
                               "EIIC", "FEIC",  "CTPC",  "CTPSW",  "CTBP",
                               "SNZCFG", "EIWR", "FEWR"};

    std::string sreg_sel1[10] = {"SPID",  "SPIDLIST", "RBASE", "EBASE", "INTBP",
                               "MCTL", "PID", "SVLOCK", "SCCFG",  "SCBP"};

    std::string sreg_sel2[12] = {"PEID", "BMID", "MEA", "MEI",
                               "RBCR0", "RBCR1", "RBNR", "RBIP",
                               "ISPR",  "ICSR", "INTCFG", "PLMR"};

    std::string sreg_sel10[6] = {"FXSR", "FXST", "FXINFO",
                               "FXCFG", "FXXC","FXXP"};

    std::string sreg_sel5[19] = {"MPM", "MPCFG", "MCA", "MCS",
                               "MCC", "MCR",   "MCI", "MPIDX",
                               "MPLA", "MPUA", "MPAT", "MPID0",
                               "MPID1", "MPID2", "MPID3", "MPID4",
                               "MPID5", "MPID6", "MPID7"};

    std::string sreg_sel4[7] = {"ICTAGL", "ICTAGH", "ICDATL", "ICDATH",
                               "ICCTRL", "ICCFG", "ICERR"};

    std::string sreg_sel11[12] = {"TSCOUNTL", "TSCOUNTH", "TSCTRL", "PMUMCTRL",
                                "PMCOUNT0", "PMCTRL0", "PMCOUNT1", "PMCTRL1",
                                "PMCOUNT2", "PMCTRL2", "PMCOUNT3", "PMCTRL3"};

    std::string sreg_sel3[9] = {"DBIC", "DBPC", "DBPSW", "DIR0",
                               "DIR1", "BPC",  "BPAV", "BPAM", "DBWR"};

    std::string sreg_sel12[16] = {"LSTEST0", "LSTEST1", "IFCR",
                                "BRPCTRL0", "BRPCTRL1", "BRPCFG",
                                "BRPACTRL", "BRPDATA", "LSUCR",
                                "RDBCR",   "RDBACR", "RDBATAG",
                                "RDBADAT0", "RDBADAT1", "RDBADAT2", "RDBADAT3"};

    for (int i = 0; i<=31; i++)
    {
        err = gcs_get_register_profile(g_reg[i], &out);
        if (err != true)
        {
            printf ("Error in get register inf. %x\n", err);
            printf("===> gcs_get_register_profile_item_13_1: FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
        printf ("Get reg profile: %s", g_reg[i].c_str());
        printf ("===> PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);

        err = gcs_get_register_profile(wr_reg[i], &out);
        if (err != true)
        {
            printf ("Error in wr_reg. %x\n", err);
            printf("===> gcs_get_register_profile_item_13_1: FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
        printf ("Get reg profile: %s", wr_reg[i].c_str());
        printf ("===> PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
    }

    for (int i = 0; i<18; i++)
    {
        err = gcs_get_register_profile(sreg_sel0[i], &out);
        if (err != true)
        {
            printf ("Error in sreg_sel0. %x\n", err);
            printf("===> gcs_get_register_profile_item_13_1: FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
        printf ("Get reg profile: %s", sreg_sel0[i].c_str());
        printf ("===> PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
    }

    for (int i = 0; i<10; i++)
    {
        err = gcs_get_register_profile(sreg_sel1[i], &out);
        if (err != true)
        {
            printf ("Error in sreg_sel1. %s\n", sreg_sel1[i].c_str());
            printf("===> gcs_get_register_profile_item_13_1: FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
        printf ("Get reg profile: %s", sreg_sel1[i].c_str());
        printf ("===> PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
    }

    for (int i = 0; i<12; i++)
    {
        err = gcs_get_register_profile(sreg_sel2[i], &out);
        if (err != true)
        {
            printf ("Error in sreg_sel2. %s\n", sreg_sel2[i].c_str());
            printf("===> gcs_get_register_profile_item_13_1: FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
        printf ("Get reg profile: %s", sreg_sel2[i].c_str());
        printf ("===> PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
    }

    for (int i = 0; i<6; i++)
    {
        err = gcs_get_register_profile(sreg_sel10[i], &out);
        if (err != true)
        {
            printf ("Error in sreg_sel10. %x\n", err);
            printf("===> gcs_get_register_profile_item_13_1: FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
        printf ("Get reg profile: %s", sreg_sel10[i].c_str());
        printf ("===> PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
    }

    for (int i = 0; i<19; i++)
    {
        err = gcs_get_register_profile(sreg_sel5[i], &out);
        if (err != true)
        {
            printf ("Error in sreg_sel5. %x\n", err);
            printf("===> gcs_get_register_profile_item_13_1: FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
        printf ("Get reg profile: %s", sreg_sel5[i].c_str());
        printf ("===> PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
    }

    for (int i = 0; i<7; i++)
    {
        err = gcs_get_register_profile(sreg_sel4[i], &out);
        if (err != true)
        {
            printf ("Error in sreg_sel4. %x\n", err);
            printf("===> gcs_get_register_profile_item_13_1: FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
        printf ("Get reg profile: %s", sreg_sel4[i].c_str());
        printf ("===> PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
    }

    for (int i = 0; i<12; i++)
    {
        err = gcs_get_register_profile(sreg_sel11[i], &out);
        if (err != true)
        {
            printf ("Error in sreg_sel11. %x\n", err);
            printf("===> gcs_get_register_profile_item_13_1: FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
        printf ("Get reg profile: %s", sreg_sel11[i].c_str());
        printf ("===> PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
    }

    for (int i = 0; i<9; i++)
    {
        err = gcs_get_register_profile(sreg_sel3[i], &out);
        if (err != true)
        {
            printf ("Error in sreg_sel13. %x\n", err);
            printf("===> gcs_get_register_profile_item_13_1: FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
        printf ("Get reg profile: %s", sreg_sel3[i].c_str());
        printf ("===> PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
    }

    for (int i = 0; i<16; i++)
    {
        err = gcs_get_register_profile(sreg_sel12[i], &out);
        if (err != true)
        {
            printf ("Error in sreg_sel12. %x\n", err);
            printf("===> gcs_get_register_profile_item_13_1: FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
        printf ("Get reg profile: %s", sreg_sel12[i].c_str());
        printf ("===> PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
    }
    printf ("Get profile of register successfully.\n");
    printf ("===> gcs_get_register_profile_item_13_1: PASS\n");
    return true;
}

bool get_reg_inf_item_13_2 ()
{
    int fail_local = 0, pass_local = 0;
    bool err;
    FrogRegPro out;
    err = gcs_get_register_profile("PSW", &out);
    if (err == false)
    {
        printf ("Initial error. \n");
        printf("===> gcs_get_register_profile_item_13_2: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_get_register_profile_item_13_2: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;
}

bool get_reg_inf_item_13_3 ()
{
    int fail_local = 0, pass_local = 0;
    bool err;
    FrogRegPro* out = NULL;
    err = gcs_get_register_profile("PSW", out);
    if (err == false)
    {
        printf ("Invalid argument. \n");
        printf("===> gcs_get_register_profile_item_13_3: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_get_register_profile_item_13_3: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;
}

bool get_reg_name_item_14_1 ()
{
    int fail_local = 0, pass_local = 0;
    bool err;
    std::string register_name;
    uint32_t selID[] = {0, 1, 2, 10, 5,
                        4, 11, 3, 12, 13};

    uint32_t regID_sreg0[] = {0, 1, 2, 3,
                             5, 6, 7, 8, 9, 10,
                             13, 14, 16, 17,
                             20, 21, 28, 29};

    uint32_t regID_sreg1[] = {0, 1, 2, 3, 4,
                                5, 6, 8, 11, 12};

    uint32_t regID_sreg2[] = {0, 1, 6, 8,
                                 15, 16, 17, 18,
                                 10, 12, 13, 14};

    uint32_t regID_sreg10[] = {6, 8, 9, 10, 12, 13};

    uint32_t regID_sreg5[] = {0, 2, 8, 9, 10,
                              11, 12, 16, 20,
                              21, 22, 24, 25,
                              26, 27, 28, 29, 30, 31};

    uint32_t regID_sreg4[] = {16, 17, 18, 19,
                                 24, 26, 28};

    uint32_t regID_sreg11[] = {0, 1, 2, 8,
                                 /*16, 17, 18, 19,
                                 20, 21, 22, 23*/};

    uint32_t regID_sreg3[] = {15, 18, 19, 20,
                                 21, 22, 24, 25, 30};

    uint32_t regID_sreg12[] = {0, 1, 4, 5, 6, 8, 9, 10,
                              11, 12};

    uint32_t regID_sreg13[] = {0,  1,  2,  3,  8,  9, 10,
                              24, 25,
                              26, 27, 28, 29, 30, 31};

    for (unsigned int i = 0; i<(sizeof(selID)/sizeof(*selID)); i++)
    {
        if (selID[i] == 0)
        {
            for (unsigned int j = 0; j<sizeof(regID_sreg0)/sizeof(*regID_sreg0); j++)
            {
                err = gcs_get_sreg_name(selID[i], regID_sreg0[j], register_name);
                if (err != true)
                {
                    printf ("gcs_get_sreg_name error selID0. \n");
                    printf("===> gcs_get_sreg_name_item_14_1: FAIL\n");
                    global_cnt_fail++;
                    fail_local++;
                    display_local(pass_local,fail_local);
                    return false;
                }
                printf("%s\n", register_name.c_str());
                printf("===> PASS\n");
                global_cnt_pass++;
                pass_local++;
                display_local(pass_local,fail_local);
            }
        }
        else if (selID[i] == 1)
        {
            for (unsigned int j = 0; j<sizeof(regID_sreg1)/sizeof(*regID_sreg1); j++)
            {
                err = gcs_get_sreg_name(selID[i], regID_sreg1[j], register_name);
                if (err != true)
                {
                    printf ("gcs_get_sreg_name error selID1, regID_sreg%d \n", regID_sreg1[j]);
                    printf("===> gcs_get_sreg_name_item_14_1: FAIL\n");
                    global_cnt_fail++;
                    fail_local++;
                    display_local(pass_local,fail_local);
                    return false;
                }
                printf("%s\n", register_name.c_str());
                printf("===> PASS\n");
                global_cnt_pass++;
                pass_local++;
                display_local(pass_local,fail_local);
            }
        }
        else if (selID[i] == 2)
        {
            for (unsigned int j = 0; j<sizeof(regID_sreg2)/sizeof(*regID_sreg2); j++)
            {
                err = gcs_get_sreg_name(selID[i], regID_sreg2[j], register_name);
                if (err != true)
                {
                    printf ("gcs_get_sreg_name error selID2. \n");
                    printf("===> gcs_get_sreg_name_item_14_1: FAIL\n");
                    global_cnt_fail++;
                    fail_local++;
                    display_local(pass_local,fail_local);
                    return false;
                }
                printf("%s\n", register_name.c_str());
                printf("===> PASS\n");
                global_cnt_pass++;
                pass_local++;
                display_local(pass_local,fail_local);
            }
        }
        else if (selID[i] == 10)
        {
            for (unsigned int j = 0; j<sizeof(regID_sreg10)/sizeof(*regID_sreg10); j++)
            {
                err = gcs_get_sreg_name(selID[i], regID_sreg10[j], register_name);
                if (err != true)
                {
                    printf ("gcs_get_sreg_name error selID10. \n");
                    printf("===> gcs_get_sreg_name_item_14_1: FAIL\n");
                    global_cnt_fail++;
                    fail_local++;
                    display_local(pass_local,fail_local);
                    return false;
                }
                printf("%s\n", register_name.c_str());
                printf("===> PASS\n");
                global_cnt_pass++;
                pass_local++;
                display_local(pass_local,fail_local);
            }
        }
        else if (selID[i] == 5)
        {
            for (unsigned int j = 0; j<sizeof(regID_sreg5)/sizeof(*regID_sreg5); j++)
            {
                err = gcs_get_sreg_name(selID[i], regID_sreg5[j], register_name);
                if (err != true)
                {
                    printf ("gcs_get_sreg_name error selID5. \n");
                    printf("===> gcs_get_sreg_name_item_14_1: FAIL\n");
                    global_cnt_fail++;
                    fail_local++;
                    display_local(pass_local,fail_local);
                    return false;
                }
                printf("%s\n", register_name.c_str());
                printf("===> PASS\n");
                global_cnt_pass++;
                pass_local++;
                display_local(pass_local,fail_local);
            }
        }
        else if (selID[i] == 4)
        {
            for (unsigned int j = 0; j<sizeof(regID_sreg4)/sizeof(*regID_sreg4); j++)
            {
                err = gcs_get_sreg_name(selID[i], regID_sreg4[j], register_name);
                if (err != true)
                {
                    printf ("gcs_get_sreg_name error selID4. \n");
                    printf("===> gcs_get_sreg_name_item_14_1: FAIL\n");
                    global_cnt_fail++;
                    fail_local++;
                    display_local(pass_local,fail_local);
                    return false;
                }
                printf("%s\n", register_name.c_str());
                printf("===> PASS\n");
                global_cnt_pass++;
                pass_local++;
                display_local(pass_local,fail_local);
            }
        }
        else if (selID[i] == 11)
        {
            for (unsigned int j = 0; j<sizeof(regID_sreg11)/sizeof(*regID_sreg11); j++)
            {
                err = gcs_get_sreg_name(selID[i], regID_sreg11[j], register_name);
                if (err != true)
                {
                    printf ("gcs_get_sreg_name error selID11, regID%d \n", regID_sreg11[j]);
                    printf("===> gcs_get_sreg_name_item_14_1: FAIL\n");
                    global_cnt_fail++;
                    fail_local++;
                    display_local(pass_local,fail_local);
                    return false;
                }
                printf("%s\n", register_name.c_str());
                printf("===> PASS\n");
                global_cnt_pass++;
                pass_local++;
                display_local(pass_local,fail_local);
            }
        }
        else if (selID[i] == 2)
        {
            for (unsigned int j = 0; j<sizeof(regID_sreg2)/sizeof(*regID_sreg2); j++)
            {
                err = gcs_get_sreg_name(selID[i], regID_sreg2[j], register_name);
                if (err != true)
                {
                    printf ("gcs_get_sreg_name error selID2. \n");
                    printf("===> gcs_get_sreg_name_item_14_1: FAIL\n");
                    global_cnt_fail++;
                    fail_local++;
                    display_local(pass_local,fail_local);
                    return false;
                }
                printf("%s\n", register_name.c_str());
                printf("===> PASS\n");
                global_cnt_pass++;
                pass_local++;
                display_local(pass_local,fail_local);
            }
        }
        else if (selID[i] == 3)
        {
            for (unsigned int j = 0; j<sizeof(regID_sreg3)/sizeof(*regID_sreg3); j++)
            {
                err = gcs_get_sreg_name(selID[i], regID_sreg3[j], register_name);
                if (err != true)
                {
                    printf ("gcs_get_sreg_name error selID3. \n");
                    printf("===> gcs_get_sreg_name_item_14_1: FAIL\n");
                    global_cnt_fail++;
                    fail_local++;
                    display_local(pass_local,fail_local);
                    return false;
                }
                printf("%s\n", register_name.c_str());
                printf("===> PASS\n");
                global_cnt_pass++;
                pass_local++;
                display_local(pass_local,fail_local);
            }
        }
        else if (selID[i] == 12)
        {
            for (unsigned int j = 0; j<sizeof(regID_sreg12)/sizeof(*regID_sreg12); j++)
            {
                err = gcs_get_sreg_name(selID[i], regID_sreg12[j], register_name);
                if (err != true)
                {
                    printf ("gcs_get_sreg_name error selID12. \n");
                    printf ("selID[%d]=%u, regID_sreg12[%d]=%u\n", i, selID[i], j, regID_sreg12[j]);
                    printf("===> gcs_get_sreg_name_item_14_1: FAIL\n");
                    global_cnt_fail++;
                    fail_local++;
                    display_local(pass_local,fail_local);
                    return false;
                }
                printf("%s\n", register_name.c_str());
                printf("===> PASS\n");
                global_cnt_pass++;
                pass_local++;
                display_local(pass_local,fail_local);
            }
        }
        else if (selID[i] == 13)
        {
            for (unsigned int j = 0; j<sizeof(regID_sreg13)/sizeof(*regID_sreg13); j++)
            {
                err = gcs_get_sreg_name(selID[i], regID_sreg13[j], register_name);
                if (err != true)
                {
                    printf ("gcs_get_sreg_name error selID13. \n");
                    printf ("selID[%d]=%u, regID_sreg13[%d]=%u\n", i, selID[i], j, regID_sreg13[j]);
                    printf("===> gcs_get_sreg_name_item_14_1: FAIL\n");
                    global_cnt_fail++;
                    fail_local++;
                    display_local(pass_local,fail_local);
                    return false;
                }
                printf("%s\n", register_name.c_str());
                printf("===> PASS\n");
                global_cnt_pass++;
                pass_local++;
                display_local(pass_local,fail_local);
            }
        }
    }
    printf ("Get register name successfully.\n");
    printf("===> gcs_get_sreg_name_item_14_1: PASS\n\n");
    return true;
}

bool get_reg_name_item_14_2 ()
{
    int fail_local = 0, pass_local = 0;
    bool err;
    std::string register_name;
    uint32_t selID = 1, regID = 7;
    err = gcs_get_sreg_name(selID, regID, register_name);
    if (err != true)
    {
        printf ("Can't find reg name. \n");
           printf("===> gcs_get_sreg_name_item_14_2: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_get_sreg_name_item_14_2: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;
}

bool step_simulator_item_2_1 ()
{
    int fail_local = 0, pass_local = 0;
    int err;
    err = gcs_step_simulator (0);
    if (err == GCSIF_ERR_NOT_INITIALIZED)
    {
        printf ("Initial error. \n");
        printf("===> gcs_step_simulator_item_2_1: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_step_simulator_item_2_1: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;
}

bool step_simulator_item_2_2 ()
{
    int fail_local = 0, pass_local = 0;
    int err;
    err = gcs_step_simulator (1);
    if (err == GCSIF_ERR_INVALID_TCID)
    {
        printf ("Invalid TCID. \n");
        printf("===> gcs_step_simulator_item_2_2: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    printf ("===> gcs_step_simulator_item_2_2: FAIL\n");
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    return false;
}

bool step_simulator_item_2_3 ()
{
    int fail_local = 0, pass_local = 0;
    int err;
    std::string opcode_32[8] = {"0xea, 0x7f, 0x08, 0x00", "0xea, 0x7f, 0x0a, 0x00",
                                "0xea, 0x7f, 0x0c, 0x00", "0xea, 0x7f, 0x0e, 0x00",
                                "0xe5, 0x0f, 0x70, 0x2b", "0xea, 0x0f, 0x70, 0x7b",
                                "0xea, 0x0f, 0x74, 0x7b", "0xef, 0x07, 0x72, 0x53"};
    std::string halt_op = "0xe0, 0x07, 0x20, 0x01";

    uint64_t address = 0xfea00000;
    FrogRegData pc_val;

    gcs_disable_simulator_stop(0);
    init_seting();
    gcs_reset_simulator();

    gcs_set_nc_register("pc", 0xfea00000);
    gcs_set_nc_register("psw", 0x00030020);
    for (int i = 0; i<8; i++)
    {
        gcs_write_simulator_memory (address, 4, opcode_32[i]);
        printf ("\n");
        err = gcs_step_simulator (0);
        gcs_get_nc_register("pc", &pc_val);
        address = pc_val;
    }
    gcs_write_simulator_memory (address, 4, halt_op);
    printf ("\n");
    err = gcs_step_simulator (0);
    gcs_get_nc_register("pc", &pc_val);
    address = pc_val;
    gcs_write_simulator_memory (address, 4, opcode_32[1]);
    printf ("\n");
    err = gcs_step_simulator (0);
    if (err == GCSIF_ERR_THREAD_IS_HALT)
    {
        printf ("HALT STATE. \n");
        printf("===> gcs_step_simulator_item_2_3: PASS\n");
        global_cnt_pass++;
        pass_local++;
        display_local(pass_local,fail_local);
        return true;
    }
    else
    {
        global_cnt_fail++;
        fail_local++;
        display_local(pass_local,fail_local);
        printf ("===> NO HALT\n");
        printf ("===> gcs_step_simulator_item_2_3: FAIL\n");
        return false;
    }
}


bool step_simulator_item_host_guest ()
{
    int fail_local = 0, pass_local = 0;

    gcs_disable_simulator_stop(0);

    init_seting ();

    gcs_internal_LoadSrec ("test_host_guest.hex");

    gcs_reset_simulator();
    for (int i = 0; i <= 200; i++) {
        int err = gcs_step_simulator (0);
        if (err == GCSIF_ERR_THREAD_IS_HALT) {
            printf ("HALT STATE. \n");
            printf("===> step_simulator_item_host_guest: PASS\n");
            global_cnt_pass++;
            pass_local++;
            display_local(pass_local,fail_local);
            return true;
        } else {
            // @@ TODO : Check the return value of gcs_get_exception().
            //           It will return 1 when exception occurs.
            printf("gcs_get_exception()=%d\n", gcs_get_exception());
        }
    }

    // Not goto HALT (doen't finish?)
    global_cnt_fail++;
    fail_local++;
    display_local(pass_local,fail_local);
    printf ("===> NO HALT\n");
    printf ("===> step_simulator_item_host_guest: FAIL\n");
    return false;
}


int run_ins (uint64_t address)
{
    int fail_local = 0, pass_local = 0;
    int err, pos=0;
    FrogRegData pc_val;
    uint64_t temp_add = 0;
    std::string opcode_32[] = {"0xea, 0x7f, 0x08, 0x00", "0xea, 0x7f, 0x0a, 0x00", "0xea, 0x7f, 0x0c, 0x00", "0xea, 0x7f, 0x0e, 0x00",
                            "0xea, 0x0f, 0x70, 0x1b", "0xea, 0x0f, 0x70, 0x7b", "0xea, 0x0f, 0x74, 0x7b", "0xef, 0x07, 0x72, 0x53",
                            "0xea, 0x17, 0x70, 0x7b", "0xea, 0x27, 0x70, 0x7b", "0xea, 0x1f, 0x70, 0x7b", "0xea, 0x2f, 0x70, 0x7b",
                            "0xea, 0x17, 0x74, 0x7b", "0xea, 0x27, 0x74, 0x7b", "0xea, 0x1f, 0x74, 0x7b", "0xea, 0x2f, 0x74, 0x7b",
                            "0xea, 0x17, 0x78, 0x7b", "0xea, 0x27, 0x78, 0x7b", "0xea, 0x27, 0x72, 0x7b", "0xea, 0x17, 0x72, 0x7b",
                            "0xea, 0x17, 0x7a, 0x7b", "0xea, 0x27, 0x7a, 0x7b", "0xea, 0x17, 0x76, 0x7b", "0xea, 0x27, 0x76, 0x7b"};
    std::string ins_opcode_32[] = {"00087fea", "000a7fea", "000c7fea", "000e7fea", "1b700fea", "7b700fea",
                                   "7b740fea", "537207ef", "7b7017ea", "7b7027ea", "7b701fea", "7b702fea",
                                   "7b7417ea", "7b7427ea", "7b741fea", "7b742fea", "7b7817ea", "7b7827ea",
                                   "7b7227ea", "7b7217ea", "7b7a17ea", "7b7a27ea", "7b7617ea", "7b7627ea"};

    std::string opcode_ldst[] = {"0xe5, 0x57, 0x00, 0x7d", "0xe5, 0x57, 0x0e, 0x7d", "0xe5, 0x57, 0x1e, 0x7d",
                                 "0xe5, 0x57, 0x20, 0x7d", "0xe5, 0x57, 0x22, 0x7d", "0xe5, 0x57, 0x26, 0x7d",
                                 "0xe5, 0x57, 0x28, 0x7d", "0xe5, 0x57, 0x2a, 0x7d", "0xe5, 0x57, 0x30, 0x7d",
                                 "0xe5, 0x57, 0x32, 0x7d", "0xe5, 0x57, 0x36, 0x7d", "0xe5, 0x57, 0x38, 0x7d",
                                 "0xe5, 0x57, 0x3a, 0x7d", "0xe5, 0x57, 0x3c, 0x7d", "0xe5, 0x57, 0x3e, 0x7d"};
    std::string ins_opcode_ldst[] ={"7d0057e5", "7d0e57e5", "7d1e57e5", "7d2057e5", "7d2257e5",
                                    "7d2657e5", "7d2857e5", "7d2a57e5", "7d3057e5", "7d3257e5",
                                    "7d3657e5", "7d3857e5", "7d3a57e5", "7d3c57e5", "7d3e57e5"};


    std::string opcode_mac[] = {"0xef, 0xa7, 0xc0, 0xcc", "0xef, 0xa7, 0xc2, 0xcc",
                                "0xef, 0xa7, 0xc4, 0xcc", "0xef, 0xa7, 0xc6, 0xcc"};
    std::string ins_opcode_mac[] ={"ccc0a7ef","ccc2a7ef", "ccc4a7ef", "ccc6a7ef"};

    std::string opcode_4way[] = {"0xe0, 0x7f, 0xa0, 0xcd", "0xe1, 0x7f, 0xa0, 0xcd", "0xe2, 0x7f, 0xa0, 0xcd", "0xe3, 0x7f, 0xa0, 0xcd",
                                 "0xe4, 0x7f, 0xa0, 0xcd", "0xe5, 0x7f, 0xa0, 0xcd", "0xe6, 0x7f, 0xa0, 0xcd", "0xe7, 0x7f, 0xa0, 0xcd",
                                 "0xe8, 0x7f, 0xa0, 0xcd", "0xe9, 0x7f, 0xa0, 0xcd", "0xea, 0x7f, 0xa0, 0xcd", "0xeb, 0x7f, 0xa0, 0xcd",
                                 "0xec, 0x7f, 0xa0, 0xcd", "0xed, 0x7f, 0xa0, 0xcd", "0xf0, 0x7f, 0xa0, 0xcd", "0xf1, 0x7f, 0xa0, 0xcd",
                                 "0xf2, 0x7f, 0xa0, 0xcd", "0xf3, 0x7f, 0xa0, 0xcd", "0xf4, 0x7f, 0xa0, 0xcd", "0xfe, 0x7f, 0xa0, 0xcd",
                                 "0xea, 0x7f, 0xa4, 0xcd", "0xea, 0x7f, 0xa6, 0xcd", "0xea, 0x7f, 0xa8, 0xcd", "0xea, 0x7f, 0xaa, 0xcd",
                                 "0xea, 0x7f, 0xac, 0xcd", "0xea, 0x7f, 0xae, 0xcd", "0xea, 0x7f, 0xb4, 0xcd", "0xea, 0x7f, 0xb6, 0xcd",
                                 "0xea, 0x7f, 0xb8, 0xcd", "0xea, 0x7f, 0xb4, 0xcd", "0xea, 0x7f, 0xba, 0xcd", "0xea, 0x7f, 0xbc, 0xcd",
                                 "0xea, 0x7f, 0xc4, 0xcd", "0xea, 0x7f, 0xc6, 0xcd", "0xea, 0x7f, 0xc8, 0xcd", "0xea, 0x7f, 0xd0, 0xcd",
                                 "0xea, 0x7f, 0xd2, 0xcd", "0xea, 0x7f, 0xd4, 0xcd", "0xea, 0x7f, 0xd6, 0xcd", "0xea, 0x7f, 0xd8, 0xcd",
                                 "0xea, 0x7f, 0xda, 0xcd", "0xea, 0x7f, 0xdc, 0xcd", "0xea, 0x7f, 0xd0, 0xcd", "0xea, 0x7f, 0xde, 0xcd",
                                 "0xe5, 0x57, 0x80, 0x7d", "0xe5, 0x57, 0x82, 0x7d", "0xe5, 0x57, 0x84, 0x7d", "0xe5, 0x57, 0x86, 0x7d",
                                 "0xe5, 0x57, 0x88, 0x7d", "0xe5, 0x57, 0x8a, 0x7d", "0xe5, 0x57, 0x8c, 0x7d", "0xe5, 0x57, 0x8e, 0x7d",
                                 "0xe5, 0x57, 0x90, 0x7d", "0xe5, 0x57, 0x92, 0x7d", "0xe5, 0x57, 0x94, 0x7d", "0xe5, 0x57, 0x96, 0x7d",
                                 "0xe5, 0x57, 0x98, 0x7d", "0xe5, 0x57, 0x9a, 0x7d", "0xe5, 0x57, 0x9c, 0x7d", "0xe5, 0x57, 0x9e, 0x7d",
                                 "0xf8, 0x7f, 0xa0, 0xcd", "0xf8, 0x7f, 0xa0, 0xcd", "0xf9, 0x7f, 0xa0, 0xcd", "0xfb, 0x7f, 0xa0, 0xcd",
                                 "0xff, 0x7f, 0xa0, 0x05", "0xff, 0x7f, 0xa0, 0x25", "0xff, 0x7f, 0xa0, 0x3d"};
    std::string ins_opcode_4way[]={"cda07fe0",  "cda07fe1",  "cda07fe2",  "cda07fe3",  "cda07fe4",  "cda07fe5",  "cda07fe6",
                                   "cda07fe7",  "cda07fe8",  "cda07fe9",  "cda07fea",  "cda07feb",  "cda07fec",  "cda07fed",
                                   "cda07ff0",  "cda07ff1",  "cda07ff2",  "cda07ff3",  "cda07ff4",  "cda07ffe",  "cda47fea",
                                   "cda67fea",  "cda87fea",  "cdaa7fea",  "cdac7fea",  "cdae7fea",  "cdb47fea",  "cdb67fea",
                                   "cdb87fea",  "cdb47fea",  "cdba7fea",  "cdbc7fea",  "cdc47fea",  "cdc67fea",  "cdc87fea",
                                   "cdd07fea",  "cdd27fea",  "cdd47fea",  "cdd67fea",  "cdd87fea",  "cdda7fea",  "cddc7fea",
                                   "cdd07fea",  "cdde7fea",  "7d8057e5",  "7d8257e5",  "7d8457e5",  "7d8657e5",  "7d8857e5",
                                   "7d8a57e5",  "7d8c57e5",  "7d8e57e5",  "7d9057e5",  "7d9257e5",  "7d9457e5",  "7d9657e5",
                                   "7d9857e5",  "7d9a57e5",  "7d9c57e5",  "7d9e57e5",  "cda07ff8",  "cda07ff8",  "cda07ff9",
                                   "cda07ffb",  "05a07fff",  "25a07fff",  "3da07fff"};

    std::string opcode_48[] = {"0xaa, 0x07, 0x1d, 0xce", "0x14, 0x78", "0xaa, 0x07, 0x9d, 0xa6", "0x00, 0x78",
                                "0xaa, 0x07, 0xbd, 0xa6", "0x00, 0x78", "0xaa, 0x07, 0xbd, 0xa6", "0xff, 0x7f",
                                "0xa0, 0x07, 0xdd, 0x03", "0x00, 0x00", "0xa0, 0x07, 0xdd, 0x03", "0xff, 0x7f",
                                "0xa0, 0x07, 0xfd, 0x03", "0x34, 0xf3", "0xa0, 0x07, 0xfd, 0x03", "0x00, 0x00",
                                "0xa0, 0x07, 0x1d, 0x03", "0x34, 0xf3", "0xa0, 0x07, 0x1d, 0x03", "0x00, 0x00",
                                "0xa0, 0x07, 0x1d, 0x03", "0xff, 0x7f", "0xa0, 0x07, 0x3d, 0x03", "0x34, 0xf3",
                                "0xa0, 0x07, 0x3d, 0x03", "0x00, 0x00", "0xa0, 0x07, 0x3d, 0x03", "0xff, 0x7f",
                                "0xa0, 0x07, 0x7d, 0x03", "0x34, 0xf3", "0xa0, 0x07, 0x7d, 0x03", "0x00, 0x00",
                                "0xa0, 0x07, 0x7d, 0x03", "0xff, 0x7f", "0xa0, 0x07, 0x9d, 0x03", "0x34, 0xf3",
                                "0xa0, 0x07, 0x9d, 0x03", "0x00, 0x00", "0xa0, 0x07, 0x9d, 0x03", "0xff, 0x7f",
                                "0xa0, 0x07, 0xbd, 0x03", "0x34, 0xf3", "0xa0, 0x07, 0xbd, 0x03", "0x00, 0x00",
                                "0xa0, 0x07, 0xbd, 0x03", "0xff, 0x7f", "0xa0, 0x07, 0x9d, 0x02", "0x34, 0xf3",
                                "0xa0, 0x07, 0x9d, 0x02", "0x00, 0x00", "0xa0, 0x07, 0x9d, 0x02", "0xff, 0x7f",
                                "0xa0, 0x07, 0xbd, 0x02", "0x34, 0xf3", "0xa0, 0x07, 0xbd, 0x02", "0x00, 0x00",
                                "0xa0, 0x07, 0xbd, 0x02", "0xff, 0x7f", "0xa0, 0x07, 0x1d, 0x00", "0x34, 0xf3",
                                "0xa0, 0x07, 0x1d, 0x00", "0x00, 0x00", "0xa0, 0x07, 0x1d, 0x00", "0xff, 0x7f",
                                "0xa0, 0x07, 0xfd, 0x00", "0x34, 0xf3", "0xa0, 0x07, 0xfd, 0x00", "0x00, 0x00",
                                "0xa0, 0x07, 0xfd, 0x00", "0xff, 0x7f", "0xa0, 0x07, 0xfd, 0x01", "0x34, 0xf3",
                                "0xa0, 0x07, 0xfd, 0x01", "0x00, 0x00", "0xa0, 0x07, 0xfd, 0x01", "0xff, 0x7f",
                                "0xa0, 0x07, 0x1d, 0x02", "0x34, 0xf3", "0xa0, 0x07, 0x1d, 0x02", "0x00, 0x00",
                                "0xa0, 0x07, 0x7d, 0x02", "0xff, 0x7f", "0xa0, 0x07, 0x3d, 0x02", "0x34, 0xf3",
                                "0xa0, 0x07, 0x3d, 0x02", "0x00, 0x00", "0xa0, 0x07, 0x3d, 0x02", "0xff, 0x7f",
                                "0xa0, 0x07, 0x7d, 0x02", "0x34, 0xf3", "0xa0, 0x07, 0x7d, 0x02", "0x00, 0x00",
                                "0xa0, 0x07, 0x7d, 0x02", "0xff, 0x7f"};
    std::string ins_opcode_48[]= {"7814ce1d07aa",  "7800a69d07aa",  "7800a6bd07aa",  "7fffa6bd07aa",  "000003dd07a0",
                                  "7fff03dd07a0",  "f33403fd07a0",  "000003fd07a0",  "f334031d07a0",  "0000031d07a0",
                                  "7fff031d07a0",  "f334033d07a0",  "0000033d07a0",  "7fff033d07a0",  "f334037d07a0",
                                  "0000037d07a0",  "7fff037d07a0",  "f334039d07a0",  "0000039d07a0",  "7fff039d07a0",
                                  "f33403bd07a0",  "000003bd07a0",  "7fff03bd07a0",  "f334029d07a0",  "0000029d07a0",
                                  "7fff029d07a0",  "f33402bd07a0",  "000002bd07a0",  "7fff02bd07a0",  "f334001d07a0",
                                  "0000001d07a0",  "7fff001d07a0",  "f33400fd07a0",  "000000fd07a0",  "7fff00fd07a0",
                                  "f33401fd07a0",  "000001fd07a0",  "7fff01fd07a0",  "f334021d07a0",  "0000021d07a0",
                                  "7fff027d07a0",  "f334023d07a0",  "0000023d07a0",  "7fff023d07a0",  "f334027d07a0",
                                  "0000027d07a0", "7fff027d07a0"};

    for (unsigned int i = 0; i<sizeof(opcode_32)/sizeof(*opcode_32); i++)
    {
        gcs_write_simulator_memory (address, 0x4, opcode_32[i]);
        printf ("\n");
        err = gcs_step_simulator(0);
        gcs_get_nc_register("pc", &pc_val);
        address = pc_val;
        if (err != GCSIF_SUCCESS)
        {
            printf ("Step simulator error. %x\n", err);
            printf("===> FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }

        pos = i;
        if (g_get_opcode.find(ins_opcode_32[pos], 0) != std::string::npos)
        {
            printf("===> PASS\n");
            global_cnt_pass++;
            pass_local++;
            display_local(pass_local,fail_local);
        }
        else
        {
            printf ("Compare instruction name miss match. \n");
            printf("===> FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
    }

    for (unsigned int i = 0; i<sizeof(opcode_ldst)/sizeof(*opcode_ldst); i++)
    {
        gcs_write_simulator_memory (address, 0x4, opcode_ldst[i]);
           printf ("\n");
        err = gcs_step_simulator(0);
        gcs_get_nc_register("pc", &pc_val);
        address = pc_val;
        if (err != GCSIF_SUCCESS)
        {
            printf ("Step simulator error. %x\n", err);
            printf("===> FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }

        pos=i;
        if (g_get_opcode.find(ins_opcode_ldst[pos], 0) != std::string::npos)
        {
            printf("===> PASS\n");
            global_cnt_pass++;
            pass_local++;
            display_local(pass_local,fail_local);
        }
        else
        {
            printf ("Compare instruction name miss match.\n");
            printf("===> FAIL\n");
               global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
    }

    for (unsigned int i = 0; i<sizeof(opcode_mac)/sizeof(*opcode_mac); i++)
    {
        gcs_write_simulator_memory (address, 0x4, opcode_mac[i]);
        printf ("\n");
        err = gcs_step_simulator(0);
        gcs_get_nc_register("pc", &pc_val);
        address = pc_val;
        if (err != GCSIF_SUCCESS)
        {
            printf ("Step simulator error. %x\n", err);
            printf("===> FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }

        pos=i;
        if (g_get_opcode.find(ins_opcode_mac[pos], 0) != std::string::npos)
        {
            printf("===> PASS\n");
            global_cnt_pass++;
            pass_local++;
            display_local(pass_local,fail_local);
        }
        else
        {
            printf ("Compare instruction name miss match. \n");
            printf("===> FAIL\n");
               global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
    }

    for (unsigned int i = 0; i<sizeof(opcode_4way)/sizeof(*opcode_4way); i++)
    {
        gcs_write_simulator_memory (address, 0x4, opcode_4way[i]);
        printf ("\n");
        err = gcs_step_simulator(0);
        gcs_get_nc_register("pc", &pc_val);
        address = pc_val;
        if (err != GCSIF_SUCCESS)
        {
            printf ("Step simulator error. %x\n", err);
            printf("===> FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }

        pos=i;
        if (g_get_opcode.find(ins_opcode_4way[pos], 0) != std::string::npos)
        {
            printf("===> PASS\n");
            global_cnt_pass++;
            pass_local++;
            display_local(pass_local,fail_local);
        }
        else
        {
            printf ("Compare instruction name miss match. \n");
            printf("===> FAIL\n");
               global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
    }

    for (unsigned int i = 0; i<sizeof(opcode_48)/sizeof(*opcode_48); i=i+2)
    {
        gcs_write_simulator_memory (address, 0x4, opcode_48[i]);
        temp_add = address + 4;
        gcs_write_simulator_memory (temp_add, 0x2, opcode_48[i+1]);
           printf ("\n");
        err = gcs_step_simulator(0);
        gcs_get_nc_register("pc", &pc_val);
        address = pc_val;
        if (err != GCSIF_SUCCESS)
        {
            printf ("Step simulator error. %x\n", err);
            printf("===> FAIL\n");
            global_cnt_fail++;
            fail_local++;
            display_local(pass_local,fail_local);
            return false;
        }
        else
        {
            pos = i/2;
            if (g_get_opcode.find(ins_opcode_48[pos], 0) != std::string::npos)
            {
                printf("===> PASS\n");
                global_cnt_pass++;
                pass_local++;
                display_local(pass_local,fail_local);
            }
            else
            {
                printf ("Compare instruction name miss match.\n");
                   printf("===> FAIL\n");
                   global_cnt_fail++;
                   fail_local++;
                display_local(pass_local,fail_local);
                return false;
            }
        }
    }
    return true;
}

bool step_simulator_item_2_4 ()
{
    uint64_t address;
    FrogRegData pc_val;
    gcs_disable_simulator_stop(0);
    init_seting();
    gcs_reset_simulator();
    //Set instruction address (It will be the same with the address in gcs_write_simulator_memory)
    gcs_set_nc_register("pc", 0x0);
    gcs_get_nc_register("pc", &pc_val);
    address = pc_val;
    //Config PSW reg
    gcs_set_nc_register("psw", 0x40030020);
    run_ins(address);

    printf ("Start fetch from other address\n");
    gcs_disable_simulator_stop(0);
    init_seting();
    //gcs_initialize_simulator_ex(MSG_INF, &pCbInfo, "./python_script/FROG/ut_CForest_g4mh_1pe.py", pHwInfo);
    gcs_reset_simulator();
    gcs_set_nc_register("pc", 0xfea00000);
    gcs_get_nc_register("pc", &pc_val);
    address = pc_val;
    gcs_set_nc_register("psw", 0x40030020);
    run_ins(address);
    printf("===> gcs_step_simulator_item_2_4: PASS\n");
    return true;
}

bool test_eiint() {
    gcs_disable_simulator_stop (0);
    init_seting();

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    uint32_t channel = 0;
    uint32_t priority = 0;
    bool eitb = false;
    int isNC = true;
    unsigned int ptid = 0;

    gcs_set_nc_register("pmr", 0x0000FFFFull);

    if (gcs_req_eiint_pe (2048, priority, eitb, false, 0, false, isNC, ptid) != GCSIF_ERR_INVALID_EVENT) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_eiint_pe (channel, 256, eitb, false, 0, false, isNC, ptid) != GCSIF_ERR_INVALID_EVENT) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_eiint_pe (channel, priority, eitb, false, 0, false, isNC, 1) != GCSIF_ERR_INVALID_TCID) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_eiint_pe (0, 0, eitb, false, 0, false, isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_eiint_pe (0, 15, eitb, false, 0, false, isNC, ptid) != GCSIF_WAR_DUPLICATED_EVENT) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    uint32_t max_pri;
    if (g_target_python_name == "ut_CForest_g4mh20_1pe.py") {
        max_pri = 255;
    } else {
        max_pri = 15;
    }
    if (gcs_req_eiint_pe (511, max_pri, eitb, false, 0, false, isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    FrogRegData get_value = 0x00;
    if (gcs_get_nc_register("icsr", &get_value) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    return (err_info == 0);
}


bool test_syserr() {
    gcs_disable_simulator_stop (0);
    init_seting();

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
    if (gcs_req_syserr_pe (0x10, isNC, ptid) != GCSIF_WAR_DUPLICATED_EVENT) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_syserr_pe (0x1F, isNC, ptid) != GCSIF_WAR_DUPLICATED_EVENT) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    return (err_info == 0);
}


bool test_rlb() {
    gcs_disable_simulator_stop (0);
    init_seting();

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

    return (err_info == 0);
}


bool test_dbnmi() {
    gcs_disable_simulator_stop (0);
    init_seting();

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

    return (err_info == 0);
}


bool test_dbint() {
    gcs_disable_simulator_stop (0);
    init_seting();

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

    return (err_info == 0);
}


bool test_rmint() {
    gcs_disable_simulator_stop (0);
    init_seting();

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

    return (err_info == 0);
}

bool test_fenmi() {
    gcs_disable_simulator_stop (0);
    init_seting();

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

    return (err_info == 0);
}


bool test_feint() {
    gcs_disable_simulator_stop (0);
    init_seting();

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    uint32_t channel = 0;
    int isNC = true;
    unsigned int ptid = 0;
    if (gcs_req_feint_pe (16, false, 0, false, isNC, ptid) != GCSIF_ERR_INVALID_EVENT) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_feint_pe (channel, false, 0, false, isNC, 1) != GCSIF_ERR_INVALID_TCID) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_feint_pe (channel, false, 0, false, isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_feint_pe (channel, false, 0, false, isNC, ptid) != GCSIF_ERR_NOT_RECEIVED) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    return (err_info == 0);
}


bool test_stopreq() {
    gcs_disable_simulator_stop (0);
    init_seting();

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    int isNC = true;
    unsigned int ptid = 0;

    if (gcs_req_cpu_stop_pe(isNC, 1) != GCSIF_ERR_INVALID_TCID) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_cpu_stop_pe(isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_req_cpu_stop_pe(isNC, ptid) != GCSIF_ERR_NOT_RECEIVED) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    return (err_info == 0);
}

bool test_resetpe() {
    gcs_disable_simulator_stop (0);
    init_seting();

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    int isNC = true;
    unsigned int ptid = 0, peid = 0;

    if (gcs_reset_pe(peid, isNC, 1) != GCSIF_ERR_INVALID_TCID) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_reset_pe(peid, isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    return (err_info == 0);
}

bool test_clear_eiint() {
    gcs_disable_simulator_stop (0);
    init_seting();

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    bool eitb = false;
    int isNC = true;
    unsigned int ptid = 0;

    gcs_set_nc_register("pmr", 0x0000FFFFull);

    if (gcs_req_eiint_pe (0, 0, eitb, false, 0, false, isNC, ptid) != GCSIF_SUCCESS) {
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

    return (err_info == 0);
}


bool test_clear_syserr() {
    gcs_disable_simulator_stop (0);
    init_seting();

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

    return (err_info == 0);
}


bool test_clear_rlb() {
    gcs_disable_simulator_stop (0);
    init_seting();

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

    return (err_info == 0);
}


bool test_clear_dbnmi() {
    gcs_disable_simulator_stop (0);
    init_seting();

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

    return (err_info == 0);
}


bool test_clear_dbint() {
    gcs_disable_simulator_stop (0);
    init_seting();

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

    return (err_info == 0);
}


bool test_clear_rmint() {
    gcs_disable_simulator_stop (0);
    init_seting();

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

    return (err_info == 0);
}


bool test_clear_fenmi() {
    gcs_disable_simulator_stop (0);
    init_seting();

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

    return (err_info == 0);
}


bool test_clear_feint() {
    gcs_disable_simulator_stop (0);
    init_seting();

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
    uint32_t channel = 0;
    int isNC = true;
    unsigned int ptid = 0;
    if (gcs_req_feint_pe (channel, false, 0, false, isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }
    if (gcs_clear_req_pe ("feint", isNC, ptid) != GCSIF_SUCCESS) {
        ++ err_info;
        printf ("Failed at %d\n", __LINE__);
    }

    return (err_info == 0);
}


bool test_clear_stopreq() {
    gcs_disable_simulator_stop (0);
    init_seting();

    gcs_reset_simulator();

    uint32_t err_info = 0;

    // initial
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

    return (err_info == 0);
}


bool test_clear_event() {
    gcs_disable_simulator_stop (0);
    init_seting();

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

    return (err_info == 0);
}


bool test_main_g4mhx (void)
{
    std::vector <int32_t> vector_fail_testcase;
    std::string        frog_if_version;
    int32_t func_pos = 0, count = 0;

    gcs_get_simulator_version(frog_if_version);
    printf ("FROG I/F version: %s\n\n", frog_if_version.c_str());
    printf ("Test configuration: %s\n", g_target_python_name.c_str());
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_initialize_simulator_ex_item_1_1: START \n");
    if (set_up_simulator_item_1_1() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_disable_simulator_stop_item_4_1: START \n");
    if (disable_simulator_item_4_1() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_reset_simulator_item_3_2: START \n");
    if (reset_simulator_item_3_2() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_write_simulator_memory_item_5_2: START \n");
    if (write_memory_item_5_2() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_write_simulator_memory_item_5_3_1: START \n");
    if (write_memory_item_5_3_1() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_write_simulator_memory_item_5_3_2: START \n");
    if (write_memory_item_5_3_2() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_read_simulator_memory_item_6_2: START \n");
    if (read_memory_item_6_2() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_set_nc_register_item_7_2: START \n");
    if (write_register_NC_item_7_2() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_get_nc_register_item_8_2: START \n");
    if (read_register_NC_item_8_2() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_create_memory_link_item_9_2: START \n");
    if (create_llbit_item_9_2() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_remove_memory_link_addr_item_10_2: START \n");
    if (remove_llbit_item_10_2() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_is_simulator_native_item_11_2: START \n");
    if (get_simulator_mode_item_11_2() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_check_fxu_function_item_12_2: START \n");
    if (check_fxu_item_12_2() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_check_fpu_function_item_12_4: START \n");
    if (check_fpu_item_12_4() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    ++ func_pos;
    printf("gcs_get_register_profile_item_13_2: START \n");
    if (get_reg_inf_item_13_2() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_get_sreg_name_item_14_2: START \n");
    if (get_reg_name_item_14_2() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_step_simulator_item_2_1: START \n");
    if (step_simulator_item_2_1() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_reset_simulator_item_3_1: START \n");
    if (reset_simulator_item_3_1() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_write_simulator_memory_item_5_1: START \n");
    if (write_memory_item_5_1() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_read_simulator_memory_item_6_1: START \n");
    if (read_memory_item_6_1() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_read_simulator_memory_item_6_3_1: START \n");
    if (read_memory_item_6_3_1() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_read_simulator_memory_item_6_3_2: START \n");
    if (read_memory_item_6_3_2() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_set_nc_register_item_7_1: START \n");
    if (write_register_NC_item_7_1() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_set_nc_register_item_7_3: START \n");
    if (write_register_NC_item_7_3() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_get_nc_register_item_8_1: START \n");
    if (read_register_NC_item_8_1() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_get_nc_register_item_8_3: START \n");
    if (read_register_NC_item_8_3() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_create_memory_link_item_9_1: START \n");
    if (create_llbit_item_9_1() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_create_memory_link_item_9_3: START \n");
    if (create_llbit_item_9_3() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_remove_memory_link_addr_item_10_1: START \n");
    if (remove_llbit_item_10_1() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_is_simulator_native_item_11_1: START \n");
    if (get_simulator_mode_item_11_1() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_check_fxu_function_item_12_1: START \n");
    if (check_fxu_item_12_1() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_check_fpu_function_item_12_3: START \n");
    if (check_fpu_item_12_3() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("test_eiint : %d\n", func_pos);
    if (test_eiint () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("test_syserr : %d\n", func_pos);
    if (test_syserr () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("test_rlb : %d\n", func_pos);
    if (test_rlb () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("test_dbnmi : %d\n", func_pos);
    if (test_dbnmi () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("test_dbint : %d\n", func_pos);
    if (test_dbint () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("test_rmint : %d\n", func_pos);
    if (test_rmint () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("test_fenmi : %d\n", func_pos);
    if (test_fenmi () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("test_feint : %d\n", func_pos);
    if (test_feint () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("test_stopreq : %d\n", func_pos);
    if (test_stopreq () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("test_resetpe : %d\n", func_pos);
    if (test_resetpe () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("test_clear_eiint : %d\n", func_pos);
    if (test_clear_eiint () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("test_clear_syserr : %d\n", func_pos);
    if (test_clear_syserr () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("test_clear_rlb : %d\n", func_pos);
    if (test_clear_rlb () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("test_clear_dbnmi : %d\n", func_pos);
    if (test_clear_dbnmi () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("test_clear_dbint : %d\n", func_pos);
    if (test_clear_dbint () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("test_clear_rmint : %d\n", func_pos);
    if (test_clear_rmint () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("test_clear_fenmi : %d\n", func_pos);
    if (test_clear_fenmi () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("test_clear_feint : %d\n", func_pos);
    if (test_clear_feint () != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

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
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_get_register_profile_item_13_1: START \n");
    if (get_reg_inf_item_13_1() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_get_register_profile_item_13_3: START \n");
    if (get_reg_inf_item_13_3() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_get_sreg_name_item_14_1: START \n");
    if (get_reg_name_item_14_1() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_step_simulator_item_2_2: START \n");
    if (step_simulator_item_2_2() != true) {
        vector_fail_testcase.push_back(func_pos);
    } else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_step_simulator_item_2_4: START \n");
    if (step_simulator_item_2_4 () != true)
    {
        vector_fail_testcase.push_back(func_pos);
        display_global(global_cnt_pass,global_cnt_fail);
    }
    else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_step_simulator_item_2_3: START \n");
    if (step_simulator_item_2_3 () != true)
    {
        vector_fail_testcase.push_back(func_pos);
        display_global(global_cnt_pass,global_cnt_fail);
    }
    else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    ++ func_pos;
    printf("gcs_step_simulator_item_host_guest: START \n");
    if (step_simulator_item_host_guest () != true)
    {
        vector_fail_testcase.push_back(func_pos);
        display_global(global_cnt_pass,global_cnt_fail);
    }
    else {
        count++;
        display_global(global_cnt_pass,global_cnt_fail);
    }
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    //printf("Total functions: %d \n", count);
    if (vector_fail_testcase.size() > 0) {
        std::vector<int32_t>::iterator it_fail = vector_fail_testcase.begin ();
        for (; it_fail != vector_fail_testcase.end (); it_fail++){
            printf("Fail function: %d,  \n", (*it_fail));
        }
        printf("UT functions FAIL for %s.\n", g_target_python_name.c_str());
        printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
        return false;
    } else {
        printf("UT functions PASS for %s.\n", g_target_python_name.c_str());
        printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
        return true;
    }
}


int main (int argc, char *argv[])
{
    // For G4MH1.0
    g_target_python_name = "ut_CForest_g4mh_1pe.py";
    if (! test_main_g4mhx ()) {
        exit (1);
    }

    // For G4MH2.0
    g_target_python_name = "ut_CForest_g4mh20_1pe.py";
    if (! test_main_g4mhx ()) {
        exit (1);
    }

    printf("All UT functions PASS.\n");
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    return 0;
}
