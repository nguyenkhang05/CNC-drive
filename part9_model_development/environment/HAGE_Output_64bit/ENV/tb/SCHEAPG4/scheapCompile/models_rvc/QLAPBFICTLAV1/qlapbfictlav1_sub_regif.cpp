// -----------------------------------------------------------------------------
// $Id: qlapbfictlav1_sub_regif.cpp,v 1.3 2020/03/24 07:16:32 duongphan Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------
// This file is generated by Register I/F generator
//    gen_regif.py 1.9 2013/12/04 09:51:36 sontran
//    gen_regif_class.py 1.60 2014/10/24 09:42:15 ducduong
//    regif_cpp.skl 1.53 2014/10/13 03:19:16 sontran
//
// Input file : /home/longnt20/girf/GenSrc/pbg_input/qlapbfictlav1_sub_regif.txt
////////////////////////////////////////////////////////////////////////////////
// %MODULE QLAPBFICTLAV1_Sub
//     #              name      offset_size
//     %%REG_INSTANCE  reg_def   2
// 
// %REG_CHANNEL reg_def
//     %%TITLE     name             reg_name        wsize     rsize     length      offset      access      init    factor_start    factor_end      factor_step     support     callback
//     %%REG       FCLACTL         FCLACTL            8        8|16|32     8          0x00        R|W        0x0           -               -               -          TRUE         -
// 
// 
// %REG_NAME FCLACTL
//     %%TITLE     name        upper   lower   init    access  support     callback
//     %%BIT       BYPS   7       7       0       R|W     TRUE        W
//     %%BIT       INTL   2       2       0       R|W     TRUE        W
//     %%BIT       INTF   1       1       0       R|W     TRUE        W
//     %%BIT       INTR   0       0       0       R|W     TRUE        W
// 
// 
// 
////////////////////////////////////////////////////////////////////////////////
/// @file qlapbfictlav1_sub_regif.cpp
/// @brief Register IF class of model QLAPBFICTLAV1_SUB
/// $Id: qlapbfictlav1_sub_regif.cpp,v 1.3 2020/03/24 07:16:32 duongphan Exp $
/// $Date: 2020/03/24 07:16:32 $
/// $Revison$
/// $Author: duongphan $
////////////////////////////////////////////////////////////////////////////////
#include "qlapbfictlav1_sub_regif.h"
#ifndef re_printf
#define re_printf get_fileline(__FILE__, __LINE__); _re_printf
#endif//re_printf

/// Constructor of Register IF class: define registers and bits
/// @return none
Cqlapbfictlav1_sub_regif::Cqlapbfictlav1_sub_regif(std::string name, uint buswidth)
    :vpcl::reg_super()
    #ifdef USR_CWR_SYSTEMC
    , cwmem("register", scml_memsize(0x1/1))
    #endif
{
    CommandInit();
    FCLACTL = new vpcl::re_register(0x0000, this, "FCLACTL", name.c_str());

    // Construct the register pointer list
    mCurReg = NULL;

    uint index = 0;
    mRegMap = new uint [1<<2];
    for (uint i = 0; i < (1<<2); i++) {
        mRegMap[i] = (1<<2);
    }

    mRegMap[0x0000]     = index++;
    mTotalRegNum = index;
    mRegArray = new SRegList* [mTotalRegNum];
    index = 0;
    #ifdef USR_CWR_SYSTEMC
    cw_instantiate_reg(CW_FCLACTL  , FCLACTL->name(), FCLACTL->addr());
    #endif
    mCurReg = mRegArray[index++] = new SRegList(FCLACTL  , mCurReg, 0,  1,       "8", "8|16|32");

    mRegList = mCurReg;

    (*FCLACTL  ) ( 7,  7, "BYPS", 0x0       , "R|W"   , vpcl::SPP_ENABLE);
    (*FCLACTL  ) ( 2,  2, "INTL", 0x0       , "R|W"   , vpcl::SPP_ENABLE);
    (*FCLACTL  ) ( 1,  1, "INTF", 0x0       , "R|W"   , vpcl::SPP_ENABLE);
    (*FCLACTL  ) ( 0,  0, "INTR", 0x0       , "R|W"   , vpcl::SPP_ENABLE);

    mFileName = "";
    mInstName = name;
    mLineNum = 0;
    mBusWidth = buswidth;
    mBusByteWidth = mBusWidth/8;   // using in BIG ENDIAN
    mIsReset = false;

    // Constructor the callback function of each register
    for (uint i = 0; i < mTotalRegNum; i++) {
        mWrCbAPI[mRegArray[i]->my_p->name()][mRegArray[i]->my_p->name()] = NULL;
        mRdCbAPI[mRegArray[i]->my_p->name()][mRegArray[i]->my_p->name()] = NULL;
    }

    //Constructor the mWrCbAPI, mRdCbAPI variable of each register
    mWrCbAPI["FCLACTL"]["BYPS"] = &Cqlapbfictlav1_sub_regif::cb_FCLACTL_BYPS;
    mWrCbAPI["FCLACTL"]["INTL"] = &Cqlapbfictlav1_sub_regif::cb_FCLACTL_INTL;
    mWrCbAPI["FCLACTL"]["INTF"] = &Cqlapbfictlav1_sub_regif::cb_FCLACTL_INTF;
    mWrCbAPI["FCLACTL"]["INTR"] = &Cqlapbfictlav1_sub_regif::cb_FCLACTL_INTR;
    InitLocalVal();
}

/// Destructor: delete pointers
/// @return none
Cqlapbfictlav1_sub_regif::~Cqlapbfictlav1_sub_regif()
{
    // Delete registers' pointer
    for (uint i = 0 ; i < mTotalRegNum ; i++) {
        delete mRegArray[i]->my_p;
        mRegArray[i]->prev = NULL;
    }
    delete [] mRegMap;
    delete [] mRegArray;
    delete mRegList;
    mCurReg = NULL;
}

/// Mask unselected bit
/// @return selected value
Cqlapbfictlav1_sub_regif::uint Cqlapbfictlav1_sub_regif::bit_select(
                                      cuint val,    ///< [in] Writting address
                                      cuint start,  ///< [in] start bit position
                                      cuint end)    ///< [in] end bit position
{
    uint ret_val = val;
    if (end - start + 1 < 32) {
        ret_val = (val>>start) & (uint)((1<<(end-start+1))-1);
    }
    return ret_val;
}

/// Write the value to requested module register
/// @return true if writing transaction is finished normally
bool Cqlapbfictlav1_sub_regif::reg_wr(cuint addr,                    ///< [in] Writting address
                                      const unsigned char *p_data,   ///< [in] Writing data
                                      cuint size)                    ///< [in] Data size (byte)
{
    if(size == 0) return false;
    #ifdef IS_MODELED_ENDIAN_BIG
    if ((addr % mBusByteWidth) + size > mBusByteWidth) return false;
    #endif

    bool ret_val = false;
    #ifdef REGIF_NOT_USE_SYSTEMC
    assert(p_data != NULL);
    #else
    sc_assert(p_data != NULL);
    #endif
    ret_val = reg_wr_process (addr, p_data, size, false);
    return ret_val;
}

/// Read the register value of requested module
/// @return true if reading transaction is finished normally
bool Cqlapbfictlav1_sub_regif::reg_rd(cuint addr,              ///< [in]  Reading address
                                      unsigned char *p_data,   ///< [out] Reading data
                                      cuint size)              ///< [in]  Data size (byte)
{
    if(size == 0) return false;
    #ifdef IS_MODELED_ENDIAN_BIG
    if ((addr % mBusByteWidth) + size > mBusByteWidth) return false;
    #endif

    bool ret_val = false;
    #ifdef REGIF_NOT_USE_SYSTEMC
    assert(p_data != NULL);
    #else
    sc_assert(p_data != NULL);
    #endif
    ret_val = reg_rd_process (addr, p_data, size, false);
    return ret_val;
}

/// Write the value to requested module register
/// @return true if writing transaction is finished normally
bool Cqlapbfictlav1_sub_regif::reg_wr_dbg(cuint addr,                    ///< [in] Writting address
                                          const unsigned char *p_data,   ///< [in] Writing data
                                          cuint size)                    ///< [in] Data size (byte)
{
    if(size == 0) return false;
    #ifdef IS_MODELED_ENDIAN_BIG
    if ((addr % mBusByteWidth) + size > mBusByteWidth) return false;
    #endif

    bool ret_val = false;
    #ifdef REGIF_NOT_USE_SYSTEMC
    assert(p_data != NULL);
    #else
    sc_assert(p_data != NULL);
    #endif
    ret_val = reg_wr_process (addr, p_data, size, true);
    return ret_val;
}

/// Read the register value of requested module
/// @return true if reading transaction is finished normally
bool Cqlapbfictlav1_sub_regif::reg_rd_dbg(cuint addr,              ///< [in]  Reading address
                                          unsigned char *p_data,   ///< [out] Reading data
                                          cuint size)              ///< [in]  Data size (byte)
{
    if(size == 0) return false;
    #ifdef IS_MODELED_ENDIAN_BIG
    if ((addr % mBusByteWidth) + size > mBusByteWidth) return false;
    #endif

    bool ret_val = false;
    #ifdef REGIF_NOT_USE_SYSTEMC
    assert(p_data != NULL);
    #else
    sc_assert(p_data != NULL);
    #endif
    ret_val = reg_rd_process (addr, p_data, size, true);
    return ret_val;
}

/// Write the value to requested module register
/// @return true if writing transaction is finished normally
bool Cqlapbfictlav1_sub_regif::reg_wr_process(cuint addr,                   ///< [in] Writting address
                                              const unsigned char *p_data,  ///< [in] Writing data
                                              cuint size,                   ///< [in] Data size (byte)
                                              bool IsDbgFunc)               ///< [in] flag indicate reg_rd or reg_rd_dbg
{
    if (mIsReset == true) {
        re_printf("warning", "Cannot write during reset period\n");
        return true;
    }
    uint tmp_addr = addr;
    #ifdef IS_MODELED_ENDIAN_BIG
    if (size < mBusByteWidth) {
        tmp_addr = addr ^ (mBusByteWidth - size);
    }
    #endif
    bool ret_val = false;
    bool valid_addr = false;
    #ifdef REGIF_NOT_USE_SYSTEMC
    assert(p_data != NULL);
    #else
    sc_assert(p_data != NULL);
    #endif
    std::list<uint> list_index;
    uint masked_addr = bit_select(tmp_addr, 0, 2-1);
    uint access_addr = masked_addr;
    while (masked_addr + size > access_addr) {
        int reg_index = get_reg_index(access_addr);
        if (reg_index != -1) {
            vpcl::re_register *reg_p = mRegArray[reg_index]->my_p; 
            if ((IsDbgFunc == true) || (valid_addr == true) || (ChkSize(mRegArray[reg_index]->wacc_size, size, tmp_addr))) {
                valid_addr = true;
                list_index.push_back(reg_index);
            }
            else {
                re_printf("error", "Writing access size to %s at address 0x%08X is wrong: %d byte(s).\n", reg_p->name().c_str(), tmp_addr, size);
                return false;
            }
            access_addr = reg_p->addr() + mRegArray[reg_index]->length;
        } else {
            access_addr += 1;
        }
    }
    if (valid_addr == true) {
        std::list<uint>::iterator it;
        for (it=list_index.begin() ; it!=list_index.end() ; it++) {
            access_addr = masked_addr;
            uint d_size = size;
            uint index = 0;
            if (masked_addr < mRegArray[(*it)]->my_p->addr()) { // adjust access address & index
                access_addr = mRegArray[(*it)]->my_p->addr();
                index = mRegArray[(*it)]->my_p->addr() - masked_addr;
            }
            if ((masked_addr + size) < (mRegArray[(*it)]->my_p->addr() + mRegArray[(*it)]->length)) { // adjust access size
                d_size = masked_addr + size - access_addr;
            } else { // adjust access size
                d_size = (mRegArray[(*it)]->my_p->addr() + mRegArray[(*it)]->length) - access_addr;
            }
            if (size > mRegArray[(*it)]->length) { // adjust index for big endianess
                #ifdef IS_MODELED_ENDIAN_BIG
                index = index ^ (size - mRegArray[(*it)]->length);
                #endif
            }
            ret_val = reg_wr_func(tmp_addr - masked_addr + access_addr, &(p_data[index]), d_size, (*it), IsDbgFunc);
        }
    } else {
        if (IsDbgFunc == false) {
            re_printf("error", "Invalid access address 0x%08X\n", tmp_addr);
        } else {
            ret_val = true;
        }
    }
    return ret_val;
}

/// Read the register value of requested module
/// @return true if reading transaction is finished normally
bool Cqlapbfictlav1_sub_regif::reg_rd_process(cuint addr,              ///< [in]  Reading address
                                              unsigned char *p_data,   ///< [out] Reading data
                                              cuint size,              ///< [in]  Data size (byte)
                                              bool IsDbgFunc)          ///< [in] flag indicate reg_rd or reg_rd_dbg
{
    uint tmp_addr = addr;
    #ifdef IS_MODELED_ENDIAN_BIG
    if (size < mBusByteWidth) {
        tmp_addr = addr ^ (mBusByteWidth - size);
    }
    #endif
    bool ret_val = false;
    bool valid_addr = false;
    #ifdef REGIF_NOT_USE_SYSTEMC
    assert(p_data != NULL);
    #else
    sc_assert(p_data != NULL);
    #endif
    std::list<uint> list_index;
    uint masked_addr = bit_select(tmp_addr, 0, 2-1);
    uint access_addr = masked_addr;
    while (masked_addr + size > access_addr) {
        int reg_index = get_reg_index(access_addr);
        if (reg_index != -1) {
            vpcl::re_register *reg_p = mRegArray[reg_index]->my_p; 
            if ((IsDbgFunc == true) || (valid_addr == true) || (ChkSize(mRegArray[reg_index]->racc_size, size, tmp_addr))) {
                valid_addr = true;
                list_index.push_back(reg_index);
            }
            else {
                re_printf("error", "Reading access size to %s at address 0x%08X is wrong: %d byte(s).\n", reg_p->name().c_str(), tmp_addr, size);
                return false;
            }
            access_addr = reg_p->addr() + mRegArray[reg_index]->length;
        } else {
            access_addr += 1;
        }
    }
    if (valid_addr == true) {
        std::list<uint>::iterator it;
        for (it=list_index.begin() ; it!=list_index.end() ; it++) {
            access_addr = masked_addr;
            uint d_size = size;
            uint index = 0;
            if (masked_addr < mRegArray[(*it)]->my_p->addr()) { // adjust access address & index
                access_addr = mRegArray[(*it)]->my_p->addr();
                index = mRegArray[(*it)]->my_p->addr() - masked_addr;
            }
            if ((masked_addr + size) < (mRegArray[(*it)]->my_p->addr() + mRegArray[(*it)]->length)) { // adjust access size
                d_size = masked_addr + size - access_addr;
            } else { // adjust access size
                d_size = (mRegArray[(*it)]->my_p->addr() + mRegArray[(*it)]->length) - access_addr;
            }
            if (size > mRegArray[(*it)]->length) { // adjust index for big endianess
                #ifdef IS_MODELED_ENDIAN_BIG
                index = index ^ (size - mRegArray[(*it)]->length);
                #endif
            }
            ret_val = reg_rd_func(tmp_addr - masked_addr + access_addr, &(p_data[index]), d_size, (*it), IsDbgFunc);
        }
    } else {
        if (IsDbgFunc == false) {
            re_printf("error", "Invalid access address 0x%08X\n", tmp_addr);
        } else {
            memset(p_data, 0, size);
            ret_val = true;
        }
    }
    return ret_val;
}

/// Process reading function
/// @return true if reading successfully
bool Cqlapbfictlav1_sub_regif::reg_rd_func(cuint addr,                     ///< [in]  Reading address
                                           unsigned char *p_data,          ///< [out] Reading data
                                           cuint size,                     ///< [in]  Data size (byte)
                                           cuint reg_index,                ///< [in] Register index
                                           bool IsDbgFunc)                 ///< [in] flag indicate reg_rd or reg_rd_dbg
{
    vpcl::re_register* Register = mRegArray[reg_index]->my_p;
    #ifdef REGIF_NOT_USE_SYSTEMC
    assert(Register != NULL);
    #else
    sc_assert(Register != NULL);
    #endif
    
    uint pst_data = 0;
    #if 1 // always 1 since any size can be accessed in debug mode 
    uint reg_length = mRegArray[reg_index]->length;
    uint start_pos = 0;
    uint mask_size = (1 << (reg_length/2)) - 1;
    if (size < reg_length) {
        #ifdef IS_MODELED_ENDIAN_BIG
        start_pos =  (reg_length - 1)*8 - (addr & mask_size)*8; // Start bit position
        #else
        start_pos =  (addr & mask_size)*8;  // Start bit position
        #endif
    }
    #endif
    if (IsDbgFunc == false) {
        if (mIsReset == true) {
            re_printf("warning", "Cannot launch call-back function during reset period\n");
        }else {
            #if 1 // always 1 since any size can be accessed in debug mode 
            if (size < reg_length) {
                re_printf("warning","Should read all bit in a register\n");
            }
            #endif
            
            pst_data = Register->read();
            
        }
    }
    pst_data = (uint)(*Register);
    #if 1 // always 1 since any size can be accessed in debug mode 
    if (size < reg_length) {
        uint read_data = 0;
        if (((reg_length%2 == 0) && (reg_length%size == 0) && ((addr&mask_size)%size == 0)) 
         || (IsDbgFunc == false)) {
            for (uint i = start_pos; i < (8*size + start_pos); i++) {
                read_data |= (pst_data & (1 << i)) >> start_pos;
            }
            pst_data = read_data;
        }
    }
    #endif

    #ifdef REGIF_NOT_USE_SYSTEMC
    assert(p_data != NULL);
    #else
    sc_assert(p_data != NULL);
    #endif
    memcpy(p_data, &pst_data, size);
    DumpRegMsg("R", Register->name(), "", size, addr, 0, 0, pst_data, mRegArray[reg_index]->length*8);
    for (vpcl::bit_info *it=Register->mBitInfo ; it!=NULL ; it=it->next) {
        uint val = (uint)(*Register);
        val = bit_select(val, it->mStartAddr, it->mEndAddr);
        DumpRegMsg("R", Register->name(), it->name(), size, addr, 0, 0, val, it->mEndAddr - it->mStartAddr + 1);
    }
    return true;
}

/// Process writing function
/// @return true if writing successfully
bool Cqlapbfictlav1_sub_regif::reg_wr_func(cuint addr,                       ///< [in] Writting address
                                           const unsigned char *p_data,      ///< [in] Writing data
                                           cuint size,                       ///< [in] Data size (byte)
                                           cuint reg_index,                  ///< [in] Register index
                                           bool IsDbgFunc)                   ///< [in] flag indicate reg_wr or reg_wr_dbg
{
    vpcl::re_register* Register = mRegArray[reg_index]->my_p;
    uint pre_data = 0;
    uint data = 0;
    #if 1 // always 1 since any size can be accessed in debug mode 
    uint reg_length = mRegArray[reg_index]->length;
    uint start_pos = 0;
    #endif
    #ifdef REGIF_NOT_USE_SYSTEMC
    assert ((p_data != NULL) && (Register != NULL));
    #else
    sc_assert ((p_data != NULL) && (Register != NULL));
    #endif
    memcpy(&data, p_data, size);
    #if 1 // always 1 since any size can be accessed in debug mode 
    if (size < reg_length) {
        uint mask_size = (1 << (reg_length/2)) - 1;
        if (((reg_length%2 == 0) && (reg_length%size == 0) && ((addr&mask_size)%size == 0)) 
         || (IsDbgFunc == false)) {
            uint new_data = (uint)(*Register);  // For writing to register
            uint written_data = 0;         // For transferring to callback function
            #ifdef IS_MODELED_ENDIAN_BIG
            start_pos =  (reg_length - 1)*8 - (addr & mask_size)*8; // Start bit position
            #else
            start_pos =  (addr & mask_size)*8;  // Start bit position
            #endif
            for (uint i = start_pos; i < (8*size + start_pos); i++) {
                new_data = (new_data & (~(1 << i))) | ((data << start_pos) & (1 << i));
                written_data = (written_data & (~(1 << i))) | ((data << start_pos) & (1 << i));
            }
            data = new_data;
        }
    }
    #endif

    if ((mRegArray[reg_index]->block == false) || (IsDbgFunc == true)){
        pre_data = (uint)(*Register);
        if (IsDbgFunc == true) {
            bool backup_warning = Register->mMessageLevel["warning"];
            bool backup_error = Register->mMessageLevel["error"];
            Register->mMessageLevel["warning"]   = false;
            Register->mMessageLevel["error"]   = false;

            Register->write(data);

            Register->mMessageLevel["warning"]   = backup_warning;
            Register->mMessageLevel["error"]   = backup_error;
        } else {
            Register->write(data);
        }
        UpdateLocalVal(Register->addr());
        DumpRegMsg("W", Register->name(), "", size, addr, data, pre_data, (uint)(*Register), mRegArray[reg_index]->length*8);
        for (vpcl::bit_info *it=Register->mBitInfo ; it!=NULL ; it=it->next) {
            uint val = (uint)(*Register);
            val = bit_select(val, it->mStartAddr, it->mEndAddr);
            uint pre_val = pre_data;
            pre_val = bit_select(pre_val, it->mStartAddr, it->mEndAddr);
            DumpRegMsg("W", Register->name(), it->name(), size, addr, 0, pre_val, val, it->mEndAddr - it->mStartAddr + 1);
            if ((mWrCbAPI[Register->name()][it->name()]) != NULL) {
                (this->*(mWrCbAPI[Register->name()][it->name()]))(RegCBstr(mRegArray[reg_index]->channel, true, size, pre_data, data));
            }
        }
    }
    else {
        re_printf("warning","%s is blocked writing from Bus I/F.\n", Register->name().c_str());
    }
    return true;
}

/// Check access size
/// @@return true if detect_size equal expect_size
bool Cqlapbfictlav1_sub_regif::ChkSize(std::string expect_size, cuint detect_size, cuint addr)
{
    std::vector<std::string> arg_vec = Str2Vec(expect_size, '|');
    std::vector<std::string>::iterator it;
    char *err_p = NULL;
    uint size = 0;
    if (detect_size > mBusByteWidth) {
        re_printf("error", "Invalid access size: %d bytes\n", detect_size);
        return false;
    }
    if(addr%detect_size != 0) {
        re_printf("error", "Invalid access address 0x%08X with access size %d bytes\n", addr, detect_size);
        return false;
    }
    for (it = arg_vec.begin(); it != arg_vec.end(); it++) {
        errno = 0; size = (uint) strtoul((*it).c_str(), &err_p, 0);
        #ifdef REGIF_NOT_USE_SYSTEMC
        assert(err_p != NULL);
        #else
        sc_assert(err_p != NULL);
        #endif
        if (errno != ERANGE && *err_p == '\0') {
            if ((size/8) == detect_size) {
                return true;
            }
        }
    }
    return false;
}

/// Initialize reg_handle_command variables
/// @return none
void Cqlapbfictlav1_sub_regif::CommandInit()
{
    mDumpRegisterRW = false;
    mMessageLevel["fatal"]   = true;
    mMessageLevel["error"]   = true;
    mMessageLevel["warning"] = false;
    mMessageLevel["info"]    = false;
}

/// Process reg_handle_command command
/// @return string
std::string Cqlapbfictlav1_sub_regif::reg_handle_command(const std::vector<std::string>& args)
{
    std::string ret = "";
    std::string err_msg = "Error ("   + mInstName + ") ";   // error message
    if (args[0] == "reg") {
        if (args[1] == "help") {
            ret += "    --- reg ---\n";
            ret += "    reg MessageLevel <fatal|error|warning|info> Select debug message level (Default: fatal,error)\n";
            ret += "    reg DumpRegisterRW <true/false>             Select dump register access information  (Default: false)\n";
            ret += "    reg <register_name> MessageLevel <fatal|error|warning|info> Select debug message level for register (Default: fatal,error)\n";
            ret += "    reg <register_name> force <value>           Force register with setting value\n";
            ret += "    reg <register_name> release                 Release register from force value\n";
            ret += "    reg <register_name> <value>                 Write a value into register\n";
            ret += "    reg <register_name>                         Read value of register\n";
            ret += "    reg help                                    Show a direction\n";
        }
        else if (args[1] == "DumpRegisterRW") {
            if (args.size() == 2) {
                ret = (mDumpRegisterRW)? "true" : "false";
            }
            else if (args.size() == 3) {
                if (args[2] == "true") {
                    mDumpRegisterRW = true;
                }
                else if (args[2] == "false") {
                    mDumpRegisterRW = false;
                } else {
                    ret = err_msg + "DumpRegisterRW command needs an argument [true/false]\n";
                }
            } else {
                ret = err_msg + "DumpRegisterRW has too much arguments\n";
            }
        }
        else if (args[1] == "MessageLevel") {
            // read mode
            if (args.size() == 2) {
                std::map<std::string, bool>::iterator it;
                for (it = mMessageLevel.begin(); it != mMessageLevel.end(); it++) {
                    if ( it->second == true ) {
                        ret += " " + it->first;
                    }
                }
            // write mode
            } else if (args.size() == 3) {
                std::vector<std::string> arg_vec = Str2Vec(args[2], '|');
                std::map<std::string, bool>::iterator msgit;
                for (msgit =mMessageLevel.begin(); msgit != mMessageLevel.end(); msgit++) {
                    msgit->second = false;
                }
                std::vector<std::string>::iterator it;
                for (it = arg_vec.begin(); it != arg_vec.end(); it++) {
                    mMessageLevel[*it] = true;
                }
            } else {
                ret = err_msg + "MessageLevel has too much arguments\n";
            }
        } else {
            bool matched = false;
            for (uint i = 0; i < mTotalRegNum; i++) {
                if (args[1] == mRegArray[i]->my_p->name()) {
                    ret = AccessRegCommand(args, mRegArray[i]->my_p, mRegArray[i]->block);
                    matched = true;
                    break;
                }
            }
            if (!matched) {
                ret = err_msg + "Register name " + (std::string)args[1] + " is invalid\n";
            }
        }
    }
    return ret;
}

/// process reg_handle_command of "reg" parameter
/// @return string
std::string Cqlapbfictlav1_sub_regif::AccessRegCommand(const std::vector<std::string>& args, vpcl::re_register *Register, bool &BlockReg)
{
    std::string ret = "";
    char *err_p = NULL;
    std::string err_msg = "Error ("   + mInstName + ") ";   // error message
    uint data = 0;
    std::ostringstream str_tmp;
    #ifdef REGIF_NOT_USE_SYSTEMC
    assert(Register != NULL);
    #else
    sc_assert(Register != NULL);
    #endif
    if (args.size() == 2) {
        data = (uint)(*Register);
        str_tmp<<std::setw(14)<<std::left<<(Register->name());
        str_tmp<<"0x"<<std::setw(8)<<std::hex<<std::uppercase<<std::left<<data<<"\n";
        ret = str_tmp.str();
    } else if ((args.size() == 3) && (args[2] == "release")) {
        BlockReg = false;
    } else if ((args.size() == 4) && (args[2] == "force")) {
        errno = 0; data = (uint) strtoul(args[3].c_str(), &err_p, 0);
        #ifdef REGIF_NOT_USE_SYSTEMC
        assert(err_p != NULL);
        #else
        sc_assert(err_p != NULL);
        #endif
        if (errno != ERANGE && *err_p == '\0') {
            *Register = data;
            BlockReg = true;
            UpdateLocalVal(Register->addr());
        } else {
            ret = err_msg + "[" + Register->name() + "] Invalid force value\n";
        }
    } else if ((args.size() >= 3) && (args[2] == "MessageLevel")) {
        if (args.size() == 3) {
            ret = Register->name() + " ";
            ret += Register->GetMessageLevel();
        } else if (args.size() == 4) {
            std::map<std::string, bool> MessageLevel;
            std::vector<std::string> arg_vec = Str2Vec(args[3], '|');
            std::vector<std::string>::iterator it;
            for (it = arg_vec.begin(); it != arg_vec.end(); it++) {
                MessageLevel[*it] = true;
            }
            Register->SetMessageLevel(MessageLevel);
        } else {
            ret = err_msg + "[" + Register->name() + "] MessageLevel has too much arguments\n";
        }
    } else if ((args.size() == 3) && (args[2] != "release")) {
        errno = 0; data = (uint) strtoul(args[2].c_str(), &err_p, 0);
        if (errno != ERANGE && *err_p == '\0') {
            *Register = data;
            UpdateLocalVal(Register->addr());
        } else {
            ret = err_msg + "[" + Register->name() + "] Invalid write value\n";
        }
    } else {
        ret = err_msg + "Wrong command : ";
        for (uint index = 0; index < args.size(); index ++) {
            ret += args[index] + " ";
        }
    }
    return ret;
}

/// Initialize local variables
/// @return none
void Cqlapbfictlav1_sub_regif::InitLocalVal()
{
    // Initialize local variables
    FCLACTL_BYPS = 0x0;
    FCLACTL_INTL = 0x0;
    FCLACTL_INTF = 0x0;
    FCLACTL_INTR = 0x0;
}

/// Update bit value to local value
/// @return None
void Cqlapbfictlav1_sub_regif::UpdateLocalVal(cuint addr)
{
    if (addr == 0x0000) {
        FCLACTL_BYPS           = (*FCLACTL)["BYPS"];
        FCLACTL_INTL           = (*FCLACTL)["INTL"];
        FCLACTL_INTF           = (*FCLACTL)["INTF"];
        FCLACTL_INTR           = (*FCLACTL)["INTR"];
        return;
    }
}

/// Update local value to bit value
/// @return None
void Cqlapbfictlav1_sub_regif::UpdateRegVal(cuint addr)
{
    if (addr == 0x0000) {
        (*FCLACTL)["BYPS"] = FCLACTL_BYPS;
        (*FCLACTL)["INTL"] = FCLACTL_INTL;
        (*FCLACTL)["INTF"] = FCLACTL_INTF;
        (*FCLACTL)["INTR"] = FCLACTL_INTR;
        return;
    }
}

/// convert string to vector
/// @return vector
std::vector<std::string> Cqlapbfictlav1_sub_regif::Str2Vec(std::string str, const char sep)
{
    std::vector<std::string> buf;
    std::string::size_type index = 0;
    for (uint i=0 ; i<str.size() ; i++) {
        if (str[i] == sep) {
            buf.push_back(str.substr(index, i-index));
            index = i+1;
        }
    }
    buf.push_back(str.substr(index));
    return buf;
}

/// Align nuber to hexadecimal format
/// @return aligned number
std::string Cqlapbfictlav1_sub_regif::Num2HexStr(cuint num, cuint num_length, bool space_strip)
{
    std::ostringstream str_tmp;
    #ifdef MSG_WO_WR_DATA
    str_tmp<<"0x"<<std::hex<<std::uppercase<<std::right<<num;
    str_tmp<<std::setfill(' ')<<std::setw(10-str_tmp.str().length())<<"";
    #else
    uint total_length = (num_length + 3)/4; // Calculate number of hexadecimal number
    str_tmp<<"0x"<<std::setfill('0')<<std::setw(total_length)<<std::hex<<std::uppercase<<std::right<<num;
    if (!space_strip) {
        str_tmp<<std::setfill(' ')<<std::setw(10-str_tmp.str().length())<<"";   
    }
    #endif
    return str_tmp.str();
}

/// Dump register access information
/// @return none
void Cqlapbfictlav1_sub_regif::DumpRegMsg(const std::string operation, const std::string RegName, const std::string BitName, cuint size, cuint addr, cuint wr_data, cuint pre_data, cuint data, cuint reg_length)
{
    if(mDumpRegisterRW == true) {
        std::string reg_name = RegName;
        std::ostringstream str_tmp;
        if (BitName != "") {
            reg_name += ":" + BitName;
        }
        str_tmp<<"REG ["<<std::setw(15)<<std::left<<reg_name<<"] ";
        str_tmp<<operation;
        str_tmp<<" Size= "<<size;
        str_tmp<<" Addr= "<<Num2HexStr(addr,2, false);
        if (operation == "R") {
            str_tmp<<" Data= "<<Num2HexStr(data    , reg_length, true);
        } else {
            #ifdef MSG_WO_WR_DATA
            str_tmp<<" Data= "<<Num2HexStr(pre_data, reg_length, false);
            #else
            str_tmp<<" Data= "<<Num2HexStr(wr_data , reg_length, false);
            str_tmp<<" : "    <<Num2HexStr(pre_data, reg_length, false);
            #endif
            str_tmp<<" => "   <<Num2HexStr(data    , reg_length, true);
        }
        re_printf("DumpRegMsg", "%s\n", str_tmp.str().c_str());
    }
}

/// find register pointer based on accessed address
/// @return accessed register pointer
int Cqlapbfictlav1_sub_regif::get_reg_index(cuint access_addr)
{
    if (mRegMap[access_addr] != (1<<2)) {
        return mRegMap[access_addr];
    } else {
        return -1;
    }
}

/// find first register pointer
/// @return first register pointer
vpcl::re_register *Cqlapbfictlav1_sub_regif::first_reg_object()
{
    mCurReg = mRegList;
    return mCurReg->my_p;
}

/// find next register pointer
/// @return next register pointer
vpcl::re_register *Cqlapbfictlav1_sub_regif::next_reg_object()
{
    if (mCurReg != NULL) {
        mCurReg = mCurReg->prev;
        if (mCurReg != NULL) {
            return mCurReg->my_p;
        }
    }
    return NULL;
}

/// write callback function of CoWare
/// @return none
void Cqlapbfictlav1_sub_regif::wr_cb(cuint addr, uint data)
{
    #ifdef USR_CWR_SYSTEMC
    cwmem.put(data, addr&0xFFFF);
    #else
    array[addr&0xFFFF] = data;
    #endif
}

/// read callback function of CoWare
/// @return read data
Cqlapbfictlav1_sub_regif::uint Cqlapbfictlav1_sub_regif::rd_cb(cuint addr)
{
    #ifdef USR_CWR_SYSTEMC
    return cwmem.get(addr&0xFFFF);
    #else
    return array[addr&0xFFFF];
    #endif
}

/// print message function
/// @return none
void Cqlapbfictlav1_sub_regif::_re_printf(std::string group, const char *message, ...)
{
    // message group check
    if (group == "DumpRegMsg") {
        group = "info";
    } else {
        #ifndef REGIF_SC_REPORT
        if (mMessageLevel[group] == false) return;
        #endif//REGIF_SC_REPORT
    }

    if (message == NULL) return;

    // print header
    #ifndef REGIF_NOT_USE_SYSTEMC
    std::stringstream t_stream;
    std::string cur_time;

    double tu_value = 0;
    std::string tu_name = "";
    if (sc_get_time_resolution() < sc_time(1, SC_PS)) {
        tu_value = 1000000000000000LL;
        tu_name = " fs";
    }
    else if (sc_get_time_resolution() < sc_time(1, SC_NS)) {
        tu_value = 1000000000000LL;
        tu_name = " ps";
    }
    else if (sc_get_time_resolution() < sc_time(1, SC_US)) {
        tu_value = 1000000000;
        tu_name = " ns";
    }
    else if (sc_get_time_resolution() < sc_time(1, SC_MS)) {
        tu_value = 1000000;
        tu_name = " us";
    }
    else if (sc_get_time_resolution() < sc_time(1, SC_SEC)) {
        tu_value = 1000;
        tu_name = " ms";
    }
    else {
        tu_value = 1;
        tu_name = " s";
    }
    t_stream << sc_time_stamp().value() * (uint64) (sc_get_time_resolution().to_seconds() * tu_value) << tu_name;

    cur_time = t_stream.str();
    #else
    std::string cur_time = "";
    #endif

    #ifndef REGIF_SC_REPORT
    #ifdef MSG_COLOR
    // color setting
    //
    // severity color       color Linux       Windows
    // fatal    RED         RED   \033[0;31m  FOREGROUND_RED
    // error    RED         BLUE  \033[0;34m  FOREGROUND_BLUE
    // warning  RED
    // info     BLUE
    #ifdef _WIN32
    HANDLE hStdout;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (group == "fatal" || group == "error" || group == "warning") {
        SetConsoleTextAttribute(hStdout, FOREGROUND_RED);
    }
    else if (group == "info" ){
        SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE);
    }
    #else
    if (group == "fatal" || group == "error" || group == "warning") {
        printf("\033[0;31m");
    }
    else if (group == "info" ){
        printf("\033[0;34m");
    }
    #endif
    #endif //MSG_COLOR
    printf("%8s [%20s] (%10s) ", group.c_str(), cur_time.c_str(), mInstName.c_str());
    #endif//REGIF_SC_REPORT

    // print body
    va_list argptr;
    va_start(argptr, message);
    if (argptr == NULL) return;
    #ifdef REGIF_SC_REPORT
    char str[1024];
    sprintf(str, "[%20s] ", cur_time.c_str());
    vsprintf(str+23, message, argptr);
    if (group == "fatal") {
        SC_REPORT_FATAL(mInstName.c_str(), str);
    }
    else if (group == "error") {
        SC_REPORT_ERROR(mInstName.c_str(), str);
    }
    else if (group == "warning") {
        SC_REPORT_WARNING(mInstName.c_str(), str);
    }
    else if (group == "info") {
        SC_REPORT_INFO(mInstName.c_str(), str);
    }
    #else //REGIF_SC_REPORT
    vprintf(message, argptr);
    #endif//REGIF_SC_REPORT

    #ifndef REGIF_SC_REPORT
    #ifdef DumpFileNameLineNum
    // print footer
    if (group == "fatal" || group == "error") {
        printf(" [%s:%d]\n", mFileName.c_str(), mLineNum);
    }
    #endif//DumpFileNameLineNum
    #endif//REGIF_SC_REPORT
    va_end(argptr);

    #ifndef REGIF_SC_REPORT
    #ifdef MSG_COLOR
    #ifdef _WIN32
    SetConsoleTextAttribute(hStdout, csbi.wAttributes);
    #else
    printf("\033[m");
    #endif
    #endif //MSG_COLOR
    #endif//REGIF_SC_REPORT

    fflush(stdout);
    fflush(stderr);

    if (group == "fatal") {
        exit(1);
    }
}

/// Get file name and line number
/// @return none
void Cqlapbfictlav1_sub_regif::get_fileline(std::string filename, int line_number)
{
    mFileName = filename;
    mLineNum  = line_number;
}

/// API for instance registration
/// @return none
void Cqlapbfictlav1_sub_regif::set_instance_name(std::string InstName)
{
    mInstName = InstName;
}

/// Method to change value of mIsReset
/// @return none
void Cqlapbfictlav1_sub_regif::EnableReset(const bool is_active)
{
    // update register IF class's reset status
    mIsReset = is_active;

    // update re_register's reset status
    for (uint i = 0; i < mTotalRegNum; i++) {
        mRegArray[i]->my_p->EnableReset(is_active);
        if (is_active) {
            re_printf("info", "Initialize %s (%08x)\n", mRegArray[i]->my_p->name().c_str(), (uint)(*mRegArray[i]->my_p));
        }
    }

    if(is_active) {
        InitLocalVal(); // Initialize local variable
    }
}

#ifdef USR_CWR_SYSTEMC
unsigned char Cqlapbfictlav1_sub_regif::cw_rd_cb(tlm::tlm_generic_payload& trans, int tag)
{
    unsigned char* ptr = (unsigned char*)trans.get_data_ptr();
    memset(ptr,0,sizeof(unsigned char));
    reg_rd_dbg((cuint)tag, ptr, sizeof(unsigned char));
    return sizeof(unsigned char);
}

unsigned char Cqlapbfictlav1_sub_regif::cw_wr_cb(tlm::tlm_generic_payload& trans, int tag)
{
    unsigned char* ptr = (unsigned char*)trans.get_data_ptr();
    reg_wr_dbg((cuint)tag, ptr, sizeof(unsigned char));
    return sizeof(unsigned char);
}
void Cqlapbfictlav1_sub_regif::cw_instantiate_reg(scml2::reg<unsigned char> * reg, std::string reg_name, int offset)
{
    reg = new scml2::reg<unsigned char>(reg_name, cwmem, offset/sizeof(unsigned char));
    scml2::set_debug_write_callback(*reg,SCML2_CALLBACK(cw_wr_cb),offset);
    scml2::set_debug_read_callback(*reg,SCML2_CALLBACK(cw_rd_cb),offset);
}
#endif
