// ---------------------------------------------------------------------
// $Id: CRC64APBV01_Func.cpp,v 1.13 2019/12/17 01:50:15 huepham Exp $
//
// Copyright(c) 2018 - 2019 Renesas Electronics Corporation
// Copyright(c) 2018 - 2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "CRC64APBV01_Func.h"
#ifndef func_re_printf
#define func_re_printf mpCRC64APBV01AgentController->get_fileline(__FILE__, __LINE__); mpCRC64APBV01AgentController->_re_printf
#endif//func_re_printf

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
CRC64APBV01_Func::CRC64APBV01_Func (std::string name, CRC64APBV01_AgentController* CRC64APBV01AgentController):
                Ccrc64apbv01_regif(name, 32)
                ,BusSlaveFuncIf()
{//{{{
    assert(CRC64APBV01AgentController != NULL);
    mpCRC64APBV01AgentController = CRC64APBV01AgentController;
    mpCRC64APBV01AgentController->CommandInit(name);
    mModuleName = name;

    Ccrc64apbv01_regif::set_instance_name(name);
    Initialize();
    CreateMap();
}//}}}

/// Destructor
CRC64APBV01_Func::~CRC64APBV01_Func()
{//{{{
}//}}}

/// Reset Func and registers
void CRC64APBV01_Func::EnableReset(const bool is_active)
{//{{{
    Initialize();
    Ccrc64apbv01_regif::EnableReset(is_active);
}//}}}

/// Initialize internal variables
void CRC64APBV01_Func::Initialize()
{//{{{
    mPsize    = 0x1F;
    mCmd0     = 0;
    mCmd1     = 0;
    mCmd2     = 0;
    misDIN1st = true;
}//}}}

/// Initialize when reset activated
void CRC64APBV01_Func::CreateMap(void)
{//{{{
    // Create mPolyMap
    mPolyMap.insert(std::pair<unsigned long long, std::string>(em0x2F         , "0x2F"));
    mPolyMap.insert(std::pair<unsigned long long, std::string>(emSAE_J1850    , "SAE_J1850"));
    mPolyMap.insert(std::pair<unsigned long long, std::string>(emCCITT16      , "CCITT16"));
    mPolyMap.insert(std::pair<unsigned long long, std::string>(emCRC32        , "CRC32"));
    mPolyMap.insert(std::pair<unsigned long long, std::string>(emCRC32RevPoly , "CRC32RevPoly"));
    mPolyMap.insert(std::pair<unsigned long long, std::string>(emCRC32C       , "CRC32C"));
    mPolyMap.insert(std::pair<unsigned long long, std::string>(emBaicheva00   , "Baicheva00"));
    mPolyMap.insert(std::pair<unsigned long long, std::string>(emCRC32P4      , "CRC32P4"));
    mPolyMap.insert(std::pair<unsigned long long, std::string>(emARC          , "ARC"));
    mPolyMap.insert(std::pair<unsigned long long, std::string>(emCRC15CAN     , "CRC15CAN"));
    unsigned long long tmp = ((unsigned long long)emCRC64ECMA2 << 32) | (unsigned long long)emCRC64ECMA1;
    mPolyMap.insert(std::pair<unsigned long long, std::string>(tmp    , "CRC64ECMA"));

    // Create mXorMap
    mXorMap.insert(std::pair<std::string, unsigned long long>("0x2F"            , 0x00000000000000FF)); 
    mXorMap.insert(std::pair<std::string, unsigned long long>("SAE_J1850"       , 0x00000000000000FF));
    mXorMap.insert(std::pair<std::string, unsigned long long>("CCITT16"         , 0x0000000000000000));
    mXorMap.insert(std::pair<std::string, unsigned long long>("CRC32"           , 0x00000000FFFFFFFF));
    mXorMap.insert(std::pair<std::string, unsigned long long>("CRC32RevPoly"    , 0x00000000FFFFFFFF));
    mXorMap.insert(std::pair<std::string, unsigned long long>("CRC32C"          , 0x00000000FFFFFFFF));
    mXorMap.insert(std::pair<std::string, unsigned long long>("Baicheva00"      , 0x0000000000000000));
    mXorMap.insert(std::pair<std::string, unsigned long long>("CRC32P4"         , 0x00000000FFFFFFFF));
    mXorMap.insert(std::pair<std::string, unsigned long long>("ARC"             , 0x0000000000000000));
    mXorMap.insert(std::pair<std::string, unsigned long long>("CRC15CAN"        , 0x0000000000000000));
    mXorMap.insert(std::pair<std::string, unsigned long long>("CRC64ECMA"       , 0xFFFFFFFFFFFFFFFF));

    // Create mInitMap
    mInitMap.insert(std::pair<std::string, unsigned long long>("0x2F"            , 0x00000000000000FF)); 
    mInitMap.insert(std::pair<std::string, unsigned long long>("SAE_J1850"       , 0x00000000000000FF));
    mInitMap.insert(std::pair<std::string, unsigned long long>("CCITT16"         , 0x000000000000FFFF));
    mInitMap.insert(std::pair<std::string, unsigned long long>("CRC32"           , 0x00000000FFFFFFFF));
    mInitMap.insert(std::pair<std::string, unsigned long long>("CRC32RevPoly"    , 0x00000000FFFFFFFF));
    mInitMap.insert(std::pair<std::string, unsigned long long>("CRC32C"          , 0x00000000FFFFFFFF));
    mInitMap.insert(std::pair<std::string, unsigned long long>("Baicheva00"      , 0x0000000000000000));
    mInitMap.insert(std::pair<std::string, unsigned long long>("CRC32P4"         , 0x00000000FFFFFFFF));
    mInitMap.insert(std::pair<std::string, unsigned long long>("ARC"             , 0x0000000000000000));
    mInitMap.insert(std::pair<std::string, unsigned long long>("CRC15CAN"        , 0x0000000000000000));
    mInitMap.insert(std::pair<std::string, unsigned long long>("CRC64ECMA"       , 0xFFFFFFFFFFFFFFFF));
    
    // Create mCtrlMap
    mCtrlMap.insert(std::pair<std::string, unsigned int>("0x2F"            , 0x00070003)); 
    mCtrlMap.insert(std::pair<std::string, unsigned int>("SAE_J1850"       , 0x00070003));
    mCtrlMap.insert(std::pair<std::string, unsigned int>("CCITT16"         , 0x000F0003));
    mCtrlMap.insert(std::pair<std::string, unsigned int>("CRC32"           , 0x001F0123));
    mCtrlMap.insert(std::pair<std::string, unsigned int>("CRC32RevPoly"    , 0x001F0013));
    mCtrlMap.insert(std::pair<std::string, unsigned int>("CRC32C"          , 0x001F0123));
    mCtrlMap.insert(std::pair<std::string, unsigned int>("Baicheva00"      , 0x000F0003));
    mCtrlMap.insert(std::pair<std::string, unsigned int>("CRC32P4"         , 0x001F0123));
    mCtrlMap.insert(std::pair<std::string, unsigned int>("ARC"             , 0x000F0123));
    mCtrlMap.insert(std::pair<std::string, unsigned int>("CRC15CAN"        , 0x000E0003));
    mCtrlMap.insert(std::pair<std::string, unsigned int>("CRC64ECMA"       , 0x003F0123));
}//}}}

/// Read API (target socket)
void CRC64APBV01_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    // Clear data buffer
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    
    // Zero clock or reset active
    if ((false == mpCRC64APBV01AgentController->CheckClockPeriod())
            || (true == mpCRC64APBV01AgentController->GetResetStatus())) {
        if (pExtInfo != NULL){
            if (! (debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID)) ){
                re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
            return;
            }
        }else{
            re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
            return;
        }
    }
    if (!debug) {
        reg_rd((unsigned int) trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int) trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }

}//}}}

/// Write API (target socket)
void CRC64APBV01_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    // Zero clock
    if (false == mpCRC64APBV01AgentController->CheckClockPeriod()) {
        if (pExtInfo != NULL){
            if (! (debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID)) ){
                re_printf("warning", "Cannot write register when clock is 0.\n");
                return;
            }
        } else{
            re_printf("warning", "Cannot write register when clock is 0.\n");
            return;
        }
    }
    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
    
}//}}}

/// Handler for register IF
void CRC64APBV01_Func::RegisterHandler (const std::vector<std::string> &args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {//Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    } else {
        if (args[1] == "*") {//Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase(cmd.begin()+1);
                cmd.insert(cmd.begin()+1, reg_p->name());
                msg_return = Ccrc64apbv01_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Catu5_timera_regif class
            msg_return = Ccrc64apbv01_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// CRC generation function
bitset<64> CRC64APBV01_Func::genKCRC(bitset<64> poly, bitset<32> din, bitset<64> init_pio, unsigned int crcctl, bitset<64> xor_reg, bitset<64> &crc_result)
{//{{{
    // Analyzing CRC control register
    unsigned int psize = (crcctl >> 16) & 0x3F;
    unsigned int cmd0  = (crcctl >> 8)  & 0x1;
    unsigned int cmd1  = (crcctl >> 5)  & 0x1;
    unsigned int cmd2  = (crcctl >> 4)  & 0x1;
    unsigned int dw    = (crcctl >> 0)  & 0x7;

    // Generate poly_in
    bitset<64> poly_in = poly;
    string stmp = poly_in.to_string<char,std::string::traits_type,std::string::allocator_type>();
    reverse(stmp.begin(), stmp.end());
    stmp = stmp.substr(0, psize + 1);
    if (cmd2 == 0) poly_in = bitset<64>(stmp);

    // Generate target_in
    bitset<32> msb_target;

    string stmp3 = din.to_string<char,std::string::traits_type,std::string::allocator_type>().substr(0, 8);
    string stmp2 = din.to_string<char,std::string::traits_type,std::string::allocator_type>().substr(8, 8);
    string stmp1 = din.to_string<char,std::string::traits_type,std::string::allocator_type>().substr(16, 8);
    string stmp0 = din.to_string<char,std::string::traits_type,std::string::allocator_type>().substr(24, 8);

    // Reverse CRCDIN
    reverse(stmp3.begin(), stmp3.end());
    reverse(stmp2.begin(), stmp2.end());
    reverse(stmp1.begin(), stmp1.end());
    reverse(stmp0.begin(), stmp0.end());
    msb_target = bitset<32>(stmp3 + stmp2 + stmp1 + stmp0);
    bitset<32> target_in(msb_target);
    if (cmd2 != cmd1) target_in = bitset<32>(din.to_string<char,std::string::traits_type,std::string::allocator_type>());

    // Engine begin
    bitset<64> d0[9], d1[9], d2[9], d3[9];

    stmp.clear();
    stmp = target_in.to_string<char,std::string::traits_type,std::string::allocator_type>().substr(24, 8);
    bitset<64> btmp(stmp);
    d0[0] = init_pio ^ btmp;
    for (unsigned int i = 0; i <= 7; i++) {
        if(d0[i][0]){
            d0[i+1] = ((d0[i] >> 1) ^ poly_in);
        }
        else{
            d0[i+1] = (d0[i] >> 1);
        }
    }
    
    stmp.clear();
    stmp = target_in.to_string<char,std::string::traits_type,std::string::allocator_type>().substr(16, 8);
    btmp = bitset<64>(stmp);
    d1[0] = d0[8] ^ btmp;
    for (unsigned int i = 0; i <= 7; i++) {
        if(d1[i][0]){
            d1[i+1] = ((d1[i] >> 1) ^ poly_in);
        }
        else{
            d1[i+1] = (d1[i] >> 1);
        }
    }

    stmp.clear();
    stmp = target_in.to_string<char,std::string::traits_type,std::string::allocator_type>().substr(8, 8);
    btmp = bitset<64>(stmp);
    d2[0] = d1[8] ^ btmp;
    for (unsigned int i = 0; i <= 7; i++) {
        if(d2[i][0]){
            d2[i+1] = ((d2[i] >> 1) ^ poly_in);
        }
        else{
            d2[i+1] = (d2[i] >> 1);
        }
    }

    stmp.clear();
    stmp = target_in.to_string<char,std::string::traits_type,std::string::allocator_type>().substr(0, 8);
    btmp = bitset<64>(stmp);
    d3[0] = d2[8] ^ btmp;
    for (unsigned int i = 0; i <= 7; i++) {
        if(d3[i][0]){
            d3[i+1] = ((d3[i] >> 1) ^ poly_in);
        }
        else{
            d3[i+1] = (d3[i] >> 1);
        }
    }
    // Engine end 

    // crc_result
    switch (dw) {
        case 0: { 
            crc_result = d3[8]; // 32bit
            break;
        }
        case 1:
        case 2: {
            crc_result = d1[8]; // 16bit
            break;
        }
        default: {
            crc_result = d0[8]; // 8bit
            break;
        }
    }

    // DOUT
    bitset<64> dout;
    if (cmd0 != cmd2) {
        dout = crc_result ^ xor_reg;
    } else {
        stmp.clear();
        stmp = crc_result.to_string<char,std::string::traits_type,std::string::allocator_type>();
        reverse(stmp.begin(), stmp.end());
        btmp = bitset<64>(stmp.substr(0, psize + 1));
        dout = btmp ^ xor_reg;
    }

    return dout;
}//}}}

/// Dump value of status registers
void CRC64APBV01_Func::DumpStatusInfo(void)
{//{{{
    printf("Status information of registers:\n");
    printf("  CRCPOLY: 0x%016llX\n",getCRCPOLYReg());
 }//}}}

/// Get CRCPOLY regsiters
unsigned long long CRC64APBV01_Func::getCRCPOLYReg()
{//{{{
    unsigned int poly0 = (unsigned int)(*CRCPOLY0)["POLY"];
    unsigned int poly1 = (unsigned int)(*CRCPOLY1)["POLY"];
    unsigned long long poly_conca = poly1;
    poly_conca <<= 32;
    poly_conca |= poly0;
    return (poly_conca);
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
void CRC64APBV01_Func::cb_CRCDIN_DIN(RegCBstr str)
{//{{{ 
    bool warning = false;

    // If the poly is changed from the 2nd calculation time onward, a warning message is thrown 
    unsigned int poly0 = (unsigned int)(*CRCPOLY0)["POLY"];
    unsigned int poly1 = (unsigned int)(*CRCPOLY1)["POLY"];
    unsigned long long poly_conca = poly1;
    poly_conca <<= 32;
    poly_conca |= poly0;
    bitset<64> poly_new(poly_conca);
    if (poly_new != mPoly) {
        if (misDIN1st == false) {
            warning = true;
        }
        mPoly = bitset<64>(poly_new);
    }

    bitset<32> din((unsigned int)(*CRCDIN)["DIN"]);

    // If the xor value is changed from the 2nd calculation time onward, a warning message is thrown
    unsigned int xor_reg0 = (unsigned int)(*CRCXOR0)["XOR"];
    unsigned int xor_reg1 = (unsigned int)(*CRCXOR1)["XOR"];
    unsigned long long xor_reg_conca = xor_reg1;
    xor_reg_conca <<= 32;
    xor_reg_conca |= xor_reg0;
    bitset<64> xor_reg_new(xor_reg_conca);
    if (xor_reg_new != mXorReg) {
        if (misDIN1st == false) {
            warning = true;
        }
        mXorReg = bitset<64>(xor_reg_new);
    }

    // If the PSIZE | CMD0 | CMD1 | CMD2 are changed from the 2nd calculation time onward, a awarning message is thrown
    unsigned int psize_new = (unsigned int)(*CRCCTL)["PSIZE"];
    unsigned int cmd0_new  = (unsigned int)(*CRCCTL)["CMD0"];
    unsigned int cmd1_new  = (unsigned int)(*CRCCTL)["CMD1"];
    unsigned int cmd2_new  = (unsigned int)(*CRCCTL)["CMD2"];
    if ((psize_new != mPsize) || (cmd0_new != mCmd0) || (cmd1_new != mCmd1) || (cmd2_new != mCmd2)) {
        if (misDIN1st == false) {
            warning = true;
        }
        mPsize = psize_new;
        mCmd0 = cmd0_new;
        mCmd1 = cmd1_new;
        mCmd2 = cmd2_new;
    }

    // From the 2nd calculation time onward, only CRCCTL.DW is able to change
    if (warning == true) {
        re_printf("warning", "The message sequence written into CRCDIN is reset with 0x%08X.\n", (unsigned int)din.to_ulong());
        misDIN1st = true;
    }

    // At the 1st calculation time, init_pio is copied from CRCDOUT
    // From the 2nd calculation time onward, mCrcResult is fed back into init_pio
    unsigned int dout0 = (unsigned int)(*CRCDOUT0)["DOUT"];
    unsigned int dout1 = (unsigned int)(*CRCDOUT1)["DOUT"];
    unsigned long long dout_conca = dout1;
    dout_conca <<= 32;
    dout_conca |= dout0;
    bitset<64> init_pio(dout_conca); // default is 1st calculation time
    if (misDIN1st == false) {
        init_pio = bitset<64>(mCrcResult);
    }

    // Register setting validation as table 4-1
    // CRCPOLY validation
    std::map<unsigned long long, std::string>::iterator itp;
    itp = mPolyMap.find(poly_conca);

    std::string pname;
    // If found
    if (itp != mPolyMap.end()) {
        pname = itp->second; // get the polynomial name

        // CRCXOR validation
        std::map<std::string, unsigned long long>::iterator itx;
        itx = mXorMap.find(pname);

        // If found
        if (itx->second != xor_reg_conca) {
            re_printf("warning", "Register CRCXOR value 0x%016llX does not obey setting table.\n", xor_reg_conca);
        }

        // Initial value of CRCDOUT validation
        if (misDIN1st) {
            std::map<std::string, unsigned long long>::iterator iti;
            iti = mInitMap.find(pname);

            // If found
            if (iti->second != dout_conca) {
                re_printf("warning", "Register CRCDOUT value 0x%016llX does not obey setting table.\n", dout_conca);
            }
        }

        // CRCCTL validation (excluding DW)
        std::map<std::string, unsigned int>::iterator itc;
        itc = mCtrlMap.find(pname);

        // If found
        unsigned int ctl_exp_msk = (itc->second) & 0xFFFFFFF8;
        unsigned int ctl_act_msk = (unsigned int)(*CRCCTL) & 0xFFFFFFF8;
        if (ctl_act_msk != ctl_exp_msk) {
            re_printf("warning", "Register CRCCTL value 0x%08X does not obey setting table.\n", (unsigned int)(*CRCCTL));
        }
        
    // If not found
    } else {
        re_printf("warning", "Register CRCPOLY value 0x%016llX does not obey setting table.\n", poly_conca);
    }

    // Generate CRC value & update mCrcResult
    bitset<64> dout(genKCRC(mPoly, din, init_pio, (unsigned int)(*CRCCTL), mXorReg, mCrcResult));

    // Write CRC value into CRCDOUT registers
    string stmp;
    stmp = dout.to_string<char,std::string::traits_type,std::string::allocator_type>().substr(0, 32);
    bitset<32> btmp(stmp);
    (*CRCDOUT1)["DOUT"] = (unsigned int)btmp.to_ulong(); // MSB
    stmp = dout.to_string<char,std::string::traits_type,std::string::allocator_type>().substr(32, 32);
    btmp = bitset<32>(stmp);
    (*CRCDOUT0)["DOUT"] = (unsigned int)btmp.to_ulong(); // LSB

    misDIN1st = false;

}//}}} 

void CRC64APBV01_Func::cb_CRCCTL_PSIZE(RegCBstr str)
{//{{{
    unsigned int psize = (unsigned int)(*CRCCTL)["PSIZE"];
    if ((psize != 0x7) && (psize != 0xE) && (psize != 0xF) && (psize != 0x1F) && (psize != 0x3F)) {
        re_printf("info", "The setting value 0x%02X to CRCCTL.PSIZE is prohibited.\n", psize);
        (*CRCCTL) = str.pre_data;
    }
}//}}}

void CRC64APBV01_Func::cb_CRCCTL_DW(RegCBstr str)
{//{{{ 
    unsigned int dw = (unsigned int)(*CRCCTL)["DW"];
    if ((dw != 0x0) && (dw != 0x1) && (dw != 0x3)) {
        re_printf("info", "The setting value 0x%02X to CRCCTL.DW is prohibited.\n", dw);
        (*CRCCTL) = str.pre_data;
    }
}//}}}

void CRC64APBV01_Func::cb_CRCDOUT0_DOUT(RegCBstr str)
{
    misDIN1st = true; 
}//}}}

void CRC64APBV01_Func::cb_CRCDOUT1_DOUT(RegCBstr str)
{//{{{ 
    misDIN1st = true; 
}//}}}
// vim600: set foldmethod=marker :
