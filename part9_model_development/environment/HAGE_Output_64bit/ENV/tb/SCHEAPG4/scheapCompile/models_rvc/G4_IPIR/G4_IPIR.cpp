// ---------------------------------------------------------------------
// $Id: G4_IPIR.cpp,v 1.14 2019/07/14 04:44:29 nhuvo Exp $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "Common.h"
#include "G4_IPIR.h"
#include "G4_IPIR_Func.h"

/// Constructor and Destructor
G4_IPIR::G4_IPIR(sc_module_name name, const char* pCfgFileName, const unsigned int rLatency, const unsigned int wLatency)
        :sc_module(name)
        ,G4_IPIR_AgentController()
        ,clk("clk")
        ,resetz("resetz")
        ,ext_err("ext_err")
        ,rvc_common_model()
{//{{{ 
    mpG4_IPIRFunc = new G4_IPIR_Func((std::string)name, this);  
    // Read configuration file
       readCfgFile(pCfgFileName);  
    // Configure Slave socket
    setSlaveResetPort32(&resetz);
    setSlaveFreqPort32(&clk);
    setTargetSocket32("ts");    
    ts = tSocket32[0];  
    mBusSlaveIf32[0]->setFuncModulePtr(mpG4_IPIRFunc);
    mBusSlaveIf32[0]->setBusProtocol(BUS_VCI);
    mBusSlaveIf32[0]->setReadLatency(rLatency);
    mBusSlaveIf32[0]->setWriteLatency(wLatency);
    // Create instances of ports
    char inst_name[128];
    for (int i = 0; i < PE_MAX_NUM; i++){
        for (int j = 0; j < INTC_IPIR_CH_NUM; j++){
            sprintf(inst_name, "ipir_int_pe%d_ch%d", i, j);
            ipir_int_pe_ch[i][j] = new sc_out<bool>(inst_name);
            ipir_int_pe_ch[i][j]->initialize(false);
        }
    }
    for (int i = 0; i < INTC_IPIR_CH_NUM; i++){
        sprintf(inst_name, "ipir_init_ch%d", i);
        ipir_init_ch[i] = new sc_in<bool>(inst_name);
    }   
    // Initialize variables
    mIsresetzInit = true;
    mclkPeriod = 0;
    mclkOrgFreq = 0;
    mclkFreq = 0;
    mclkFreqUnit = "Hz";
    mIsResetHardActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive = false;
    mResetCmdPeriod = 0;
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    mExtErr = false;
    
    Initialize();
    
    // SC_THREAD/METHOD
    SC_METHOD(clkMethod);
    dont_initialize();
    sensitive << clk;
    
    SC_METHOD(resetzMethod);
    dont_initialize();
    sensitive << resetz;
    
    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << meResetCmd;
    
    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << meResetCmdCancel;
    
    SC_METHOD(InitChannelMethod);
    dont_initialize();
    for (int i = 0; i < INTC_IPIR_CH_NUM; i++){
        sensitive << (*ipir_init_ch[i]);
    }

    SC_METHOD(ExtErrMethod);
    dont_initialize();
    sensitive << ext_err;

    SC_METHOD(WriteInterruptMethod);
    dont_initialize();
    sensitive << meWriteInterrupt;
}//}}}

/// Destructor
G4_IPIR::~G4_IPIR()
{//{{{
    delete mpG4_IPIRFunc;
    for (int j = 0; j < PE_MAX_NUM; j++){
        for (int i = 0; i < INTC_IPIR_CH_NUM; i++){
            delete ipir_int_pe_ch[j][i];
        }
    }
}//}}}

/// Initialize when reset activated
void G4_IPIR::Initialize(void)
{//{{{
    for (int i = 0; i < PE_MAX_NUM; i++){
        for (int j = 0; j < INTC_IPIR_CH_NUM; j++){
            maInterrupt[i][j] = false;
        }
    }
    mDumpInterrupt = false;
    mEnableRegisterMessage = true;
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
/// Drive Interrupt ports
void G4_IPIR::DriveInterrupt(bool int_array[PE_MAX_NUM][INTC_IPIR_CH_NUM])
{//{{{
    sc_assert(int_array!=NULL); 
    for (int i = 0; i < PE_MAX_NUM; i++){
        for (int j = 0; j < INTC_IPIR_CH_NUM; j++){
            maInterrupt[i][j] = int_array[i][j];
        }
    }
    meWriteInterrupt.notify();
}//}}}

/// Get external error value
bool G4_IPIR::GetExtErr()
{//{{{
    return mExtErr;
}//}}}

/// Get Pe Num from BMID info
bool G4_IPIR::GetPeNum(unsigned int bmid, unsigned int& pe_num)
{//{{{
    bool ret = false;
    if (mPEMap.find(bmid) != mPEMap.end() ){
        pe_num = mPEMap[bmid];
        ret = true;
    }else{
        re_printf("warning", "Cannot find pe_num value from bmid_val (%d).\n", bmid);
        ret = false;
    }
    return ret;
}//}}}

/// Get clk period
sc_dt::uint64 G4_IPIR::GetClockPeriod()
{//{{{
    return mclkPeriod;
}//}}}

/// Get reset status
bool G4_IPIR::GetResetStatus()
{//{{{
    return (mIsResetCmdActive || mIsResetHardActive);
}//}}}
//========================================================
//============ Reset and clock  ==========================
//========================================================
/// Monitor resetz port
void G4_IPIR::resetzMethod()
{//{{{
    bool reset = resetz.read();
    if (reset == vpcl::emResetActive){
        mIsresetzInit = false;
        mIsResetHardActive = true;
        re_printf("info", "The reset port resetz is asserted.\n");
        EnableReset(true);
    }else{
        if (mIsresetzInit){
            mIsresetzInit = false;
        }else{
            mIsResetHardActive = false;
            re_printf("info", "The reset port resetz is de-asserted.\n");
            EnableReset(false);
        }
    }
    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive && mIsResetHardActive) {
        meResetCmd.cancel();
        meResetCmdCancel.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
    }
}//}}}

/// Reset model and its sub-instance if reset is actived
void G4_IPIR::EnableReset(const bool is_active)
{//{{{
    if (is_active){
        // Cancel events
        meWriteInterrupt.cancel();
        // Initialize variables
        Initialize();
        // Initialize output ports
        meWriteInterrupt.notify();
    }
    mpG4_IPIRFunc->EnableReset(is_active);
}//}}}

/// Monitor clk clock
void G4_IPIR::clkMethod()
{//{{{
    sc_dt::uint64 freq_value = clk.read();
    SetCLKFreq("clk", freq_value, "Hz");
}//}}}

/// Process reset function when reset command is active
void G4_IPIR::HandleResetCmdMethod ()
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of resetz.\n");
    this->EnableReset(mIsResetCmdActive);
    meResetCmdCancel.notify(mResetCmdPeriod, SC_NS);
}//}}}

/// Cancel reset function when reset command is active
void G4_IPIR::CancelResetCmdMethod ()
{//{{{
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of resetz is over.\n");
    this->EnableReset(mIsResetCmdActive);
}//}}}

//========================================================
//============ Operation               ===================
//========================================================
/// Monitor ipir_init_ch ports
void G4_IPIR::InitChannelMethod()
{//{{{
    for (int i = 0; i < INTC_IPIR_CH_NUM; i++){
        if (ipir_init_ch[i]->read()){
            mpG4_IPIRFunc->ClearRegsChannel(i);
        } 
    }
}//}}}

/// Monitor ext_err port
void G4_IPIR::ExtErrMethod()
{//{{{
    mExtErr = ext_err.read();
}//}}}

/// Write out the interrupt ports
void G4_IPIR::WriteInterruptMethod()
{//{{{
    for (int pe = 0; pe < PE_MAX_NUM; pe++){
        for (int ch = 0; ch < INTC_IPIR_CH_NUM; ch++){
            if (get_active_attribute(pe)){
                // Write interrupt ports
                ipir_int_pe_ch[pe][ch]->write(maInterrupt[pe][ch]);
                // Dump message
                if (mDumpInterrupt){
                    if (maInterrupt[pe][ch]){
                        re_printf("info","INT [ipir_int_pe%d_ch%d] Assert.\n", pe, ch);
                    }else{
                        re_printf("info","INT [ipir_int_pe%d_ch%d] De-assert.\n", pe, ch);
                    }
                }
            }
        }
    }
}//}}}

/// Get the active attribute according PE num
bool G4_IPIR::get_active_attribute(unsigned int pe_num)
{//{{{
    bool ret = false;
    if (mBMIDMap.find(pe_num) != mBMIDMap.end() ){
        unsigned int bmid = mBMIDMap[pe_num];
        if (mActiveMap.find(bmid) != mActiveMap.end() ){
            ret = mActiveMap[bmid];
        } 
    }
    return ret;
}//}}}

/// Read config file to get info
void G4_IPIR::readCfgFile( const char *filename)
{//{{{
    get_G4CPU_PE_INFO(filename);
    get_G4IPIR_MODE_BMID_PE(filename);
}//}}}

/// Read config file to get G4CPU_PE_INFO
void G4_IPIR::get_G4CPU_PE_INFO(const char *filename)
{//{{{
    if(filename != NULL) {
        char seps[] = " ,=();\t\n\r";
        unsigned int line = 0;
        char word[512];
        char* token;
        ifstream  config_file(filename);
        while(1){
            config_file.getline(word, 512, '\n');
            line++;
            token = strtok(word, seps);
            // For EOF
            if (config_file.eof()){
                break;
            }
            // For comment
            if ((token == NULL) || (strncmp(token, "//", 2) == 0) ){
                continue;
            }
            // Maker [END] - NOTICE:You have to check whether token is NULL first.
            if (strcmp(token, "[END]") == 0){
                break;
            }
            // Get token G4CPU_PE_INFO
            if (strcmp(token, "[G4CPU_PE_INFO]") == 0){
                bool is_info_valid = true;
                unsigned int pe_num = 0;
                unsigned int bmid_val = 0;

                // pe_num
                token = strtok(0, seps);
                if (token != NULL){
                    pe_num = token_to_uint(token, this->basename(), "[G4CPU_PE_INFO]");
                    if (pe_num >= PE_MAX_NUM){
                        re_printf("warning", "[file: %s][line: %d] pe_num must be under %d, but specified (%d).\n", filename, line, PE_MAX_NUM-1, pe_num);
                        is_info_valid = false;
                    }
                    std::map<unsigned int, unsigned int>::iterator it;
                    for (it = mBMIDMap.begin(); it != mBMIDMap.end(); it++){
                        if (pe_num == it->first){
                            mBMIDMap.erase(it);
                            re_printf("warning", "[file: %s][line: %d] The pe_num [%d] was declared before.\n \
                                Previous declaration will be ignored.\n", filename, line, pe_num);
                            std::map<unsigned int, unsigned int>::iterator ref;
                            for (ref = mPEMap.begin(); ref != mPEMap.end(); ref++){
                                if (pe_num == ref->second){
                                    mPEMap.erase(ref);
                                    break;
                                } 
                            }
                            break;
                        }
                    }
                }else{
                    re_printf("warning", "[file: %s][line: %d] Lack pe_num info.\n", filename, line);
                    is_info_valid = false;
                    continue;
                }

                // ignore peid_val
                token = strtok(0, seps);
                // bmid_val
                token = strtok(0, seps);
                if (token != NULL){
                    bmid_val = token_to_uint(token, this->basename(), "[G4CPU_PE_INFO]");
                    std::map<unsigned int, unsigned int>::iterator it;  
                    for (it = mPEMap.begin(); it != mPEMap.end(); it++){
                        if (bmid_val == it->first){
                            mPEMap.erase(it);
                            re_printf("warning", "[file: %s][line: %d] The bmid_val [%d] was declared before.\n \
                                Previous declaration will be ignored.\n", filename, line, bmid_val);
                            std::map<unsigned int, unsigned int>::iterator ref;
                            for (ref = mBMIDMap.begin(); ref != mBMIDMap.end(); ref++){
                                if (bmid_val == ref->second){
                                    mBMIDMap.erase(ref);
                                    break;
                                }
                            }
                            break;
                        } 
                    }
                }else{
                    re_printf("warning", "[file: %s][line: %d] Lack bmid_val info.\n", filename, line);
                    is_info_valid = false;
                    continue;
                }
                // Do not care other params

                // Insert to map if valid
                if (is_info_valid){
                    mPEMap.insert(std::pair<unsigned int, unsigned int>(bmid_val, pe_num));
                    mBMIDMap.insert(std::pair<unsigned int, unsigned int>(pe_num, bmid_val));
                } 
            }
        }
    } 
    // Set default value to mPEMap/mBMIDMap if no info in configuration file
    if (mPEMap.size() == 0){
        for (unsigned int i = 0; i < PE_MAX_NUM; i++){
            mPEMap.insert(std::pair<unsigned int, unsigned int>(i, i));
            mBMIDMap.insert(std::pair<unsigned int, unsigned int>(i, i));
        }
    }
}//}}}

/// Read config file to get G4IPIR_MODE_BMID_PE
void G4_IPIR::get_G4IPIR_MODE_BMID_PE(const char *filename)
{//{{{
    if(filename != NULL) {
        char seps[] = " ,=();\t\n\r";
        unsigned int line = 0;
        char word[512];
        char* token;
        ifstream  config_file(filename);
        while(1){
            config_file.getline(word, 512, '\n');
            line++;

            token = strtok(word, seps);
            // For EOF
            if (config_file.eof()){
                break;
            }
            // For comment
            if ((token == NULL) || (strncmp(token, "//", 2) == 0) ){
                continue;
            }
            // Maker [END] - NOTICE:You have to check whether token is NULL first.
            if (strcmp(token, "[END]") == 0){
                break;
            }
            // Get token G4CPU_PE_INFO
            if (strcmp(token, "[G4IPIR_MODE_BMID_PE]") == 0){
                bool is_info_valid = true;
                unsigned int bmid_val = 0;
                bool active = false;

                // bmid_val
                token = strtok(0, seps);
                if (token != NULL){
                    bmid_val = token_to_uint(token, this->basename(), "[G4IPIR_MODE_BMID_PE]");
                    std::map<unsigned int, bool>::iterator it;
                    for (it = mActiveMap.begin(); it != mActiveMap.end(); it++){
                        if (bmid_val == it->first){
                            mActiveMap.erase(it);
                            re_printf("warning", "[file: %s][line: %d] The Bus master ID [%d] was declared before.\n \
                                Previous declaration will be ignored.\n", filename, line, bmid_val);
                            break;
                        } 
                    }
                    bool is_bmid_found = false;
                    std::map<unsigned int, unsigned int>::iterator ref;
                    for (ref = mPEMap.begin(); ref != mPEMap.end(); ref++){
                        if ( (ref->first) == bmid_val ){
                            is_bmid_found = true;
                            break;
                        } 
                    }
                    if (!is_bmid_found){
                        re_printf("warning", "[file: %s][line: %d] The Bus master ID [%d] was not defined before in [G4CPU_PE_INFO].\n \
                                This declaration will be ignored.\n", filename, line, bmid_val);
                        is_info_valid = false;
                    } 
                }else{
                    re_printf("warning", "[file: %s][line: %d] Lack bmid_val info.\n", filename, line);
                    is_info_valid = false;
                    continue;
                }

                // Active
                token = strtok(0, seps);
                if (token != NULL){
                    active = (strcmp(token, "TRUE") == 0);
                }else{
                    re_printf("warning", "[file: %s][line: %d] Lack bmid active info.\n", filename, line);
                    is_info_valid = false;
                    continue;
                }

                // Insert to map if valid
                if (is_info_valid){
                    mActiveMap.insert(std::pair<unsigned int, bool>(bmid_val, active));
                }
            }
        }
    }  
    // Set default value to mActiveMap if no info in configuration file
    if (mActiveMap.size() == 0){
        std::map<unsigned int, unsigned int>::iterator it;
        for (it = mPEMap.begin(); it != mPEMap.end(); it++){
            mActiveMap.insert(std::pair<unsigned int, bool>(it->first, true));
        }
    }
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
/// Set message level (fatal, error, warning, info)
void G4_IPIR::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }

    if (true == mEnableRegisterMessage) {
        //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpG4_IPIRFunc->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpG4_IPIRFunc->RegisterHandler(cmd);
    } else {
        //Set message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mpG4_IPIRFunc->RegisterHandler(cmd);

        //Set message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mpG4_IPIRFunc->RegisterHandler(cmd);
    }
}//}}}

/// Enable/disable dumping message when users access registers
void G4_IPIR::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpG4_IPIRFunc->RegisterHandler(cmd);
}//}}}

/// Enable/disable dumping interrupt information
void G4_IPIR::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: IPIR_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "false";
        if(mDumpInterrupt) {
            temp = "true";
        } 
        re_printf("info","IPIR_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: IPIR_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

/// Command to assert reset
void G4_IPIR::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "resetz") {
        if ((!mIsResetHardActive)&&(!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (resetz) for %f ns after %f ns.\n", period, start_time);
            meResetCmd.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of resetz is called in the reset operation of the model. So it is ignored.\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be resetz.\n",reset_name.c_str());
    }
}//}}}

/// Set clock value and clock unit
void G4_IPIR::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "clk") {
            mclkOrgFreq = freq;
            ConvertClockFreq (mclkFreq, mclkFreqUnit, freq, unit);
            if (mclkFreq > 0) {
                mclkPeriod = (sc_dt::uint64)(((1/(double)mclkFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mclkPeriod = 0;
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Get clock value
void G4_IPIR::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "clk") {
            re_printf("info","clk frequency is %0.0f %s\n", (double)mclkOrgFreq, mclkFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Force value to register
void G4_IPIR::ForceRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("force");
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            mpG4_IPIRFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Release forced value after forcing registers
void G4_IPIR::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mpG4_IPIRFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Write value to registers by software
void G4_IPIR::WriteRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            mpG4_IPIRFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Read value of register by software
void G4_IPIR::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mpG4_IPIRFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// List all registers name
void G4_IPIR::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpG4_IPIRFunc->RegisterHandler(cmd);
}//}}}

/// Dump help message of all parameters or commands
void G4_IPIR::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"IPIR_MessageLevel (\"IPIR instance\", \"fatal|error|warning|info\")        Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"IPIR_DumpRegisterRW (\"IPIR instance\", \"true/false\")                    Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"IPIR_DumpInterrupt (\"IPIR instance\", \"true/false\")                     Enable/disable dumping interrupt information (Default: false)");
            SC_REPORT_INFO(this->basename(),"IPIR_EnableRegisterMessage (\"G4_IPIR instance\", \"true/false\")               Enable/disable info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"IPIR_AssertReset (\"IPIR instance\", \"reset_name\", start_time, period)   Assert and de-assert reset signal to the IPIR model");
            SC_REPORT_INFO(this->basename(),"IPIR_SetCLKFreq (\"IPIR instance\", \"clock_name\", freq, \"unit\")        Set clock frequency value to IPIR model. Valid clock_name is clk");
            SC_REPORT_INFO(this->basename(),"IPIR_GetCLKFreq (\"IPIR instance\", \"clock_name\")                        Get clock frequency value of IPIR model. Valid clock_name is clk");
            SC_REPORT_INFO(this->basename(),"IPIR_ForceRegister (\"IPIR instance\", \"reg_name\", reg_value)            Force register with setting value");
            SC_REPORT_INFO(this->basename(),"IPIR_ReleaseRegister (\"IPIR instance\", \"reg_name\")                     Release register from force value");
            SC_REPORT_INFO(this->basename(),"IPIR_WriteRegister (\"IPIR instance\", \"reg_name\", reg_value)            Write a value to a register");
            SC_REPORT_INFO(this->basename(),"IPIR_ReadRegister (\"IPIR instance\", \"reg_name\")                        Read a value from a register");
            SC_REPORT_INFO(this->basename(),"IPIR_ListRegister (\"IPIR instance\")                                      Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of IPIR_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of IPIR_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

void G4_IPIR::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: IPIR_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
            std::string intr_name = "false";
            if (mEnableRegisterMessage) {
                intr_name = "true";
            } 
            re_printf("info","IPIR_EnableRegisterMessage %s\n", intr_name.c_str());
    } else {
        re_printf("warning","Invalid argument: IPIR_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :
