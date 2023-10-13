// -----------------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2012 Renesas Electronics Corporation
// Copyright(c) 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------


#include "scds.h"
#ifndef FRACTION_TO_INT
#define FRACTION_TO_INT(d) ((unsigned int)((d - (unsigned long long)(d))*1000000))
#endif

/// Constructor of SCDS class
Cscds::Cscds(sc_module_name name): Cgeneral_reset(name)
                                  ,vpcl::tlm_tgt_if<32, tlm::tlm_base_protocol_types, 2>(name)
                                  ,Cscds_regif((std::string)name, 32)
                                  ,pclk("pclk")
                                  ,preset_n("preset_n")
                                  // Input ports
                                  ,tf_wr("tf_wr")
                                  ,tf_rd("tf_rd")
                                  ,ocdidin1("ocdidin1")
                                  ,ocdid1("ocdid1")
                                  ,onbd("onbd")
                                  ,serialiden_n("serialiden_n")
                                  ,frefseq("frefseq")
                                  // Output ports
                                  ,seclock_faci("seclock_faci")
                                  /// Update in SCDS_Improvement
                                  ,cdflrdidin1("cdflrdidin1")
                                  ,dtflrdidin1("dtflrdidin1")
                                  ,cdflweidin1("cdflweidin1")
                                  ,dtflweidin1("dtflweidin1")
                                  ,cdflrdidin0("cdflrdidin0")
                                  ,dtflrdidin0("dtflrdidin0")
                                  ,cdflid0("cdflid0")
                                  ,dtflid0("dtflid0")
                                  ,cdflid1("cdflid1")
                                  ,dtflid1("dtflid1")
                                  ,cdflrdpflg0_n("cdflrdpflg0_n")
                                  ,dtflrdpflg0_n("dtflrdpflg0_n")
                                  ,cdflrdpflg1_n("cdflrdpflg1_n")
                                  ,dtflrdpflg1_n("dtflrdpflg1_n")
                                  ,cdflrden0("cdflrden0")
                                  ,dtflrden0("dtflrden0")
                                  ,cdflrden1("cdflrden1")
                                  ,dtflrden1("dtflrden1")
                                  ,cdflween("cdflween")
                                  ,dtflween("dtflween")
{//{{{
    // Initialize handleCommand  
    CommandInit(this->name());
    Cscds_regif::set_instance_name(this->name());
    
    // Initialize variables
    Initialize();
    mIsRegConfigEnable = true;
    mIsZeroClock = true;
    
    for (unsigned int i = 0; i <= emMaxIndex; i++) {
        // Initialize mRegConfig
        mRegConfig[i].implement = true;
        mRegConfig[i].size = emMaxRegSize;
        
        // Initialize greg output ports
        std::ostringstream str;
        str << "greg" << i;
        greg[i] = new sc_out<sc_uint<32> > (str.str().c_str());
        sc_assert(greg[i] != NULL);
        greg[i]->initialize(0);
    }
    
    // Initialize seclock_faci output port
    seclock_faci.initialize(false);
    /// Update in SCDS_Improvement
    cdflrden0.initialize(true);
    dtflrden0.initialize(true);
    cdflrden1.initialize(true);
    dtflrden1.initialize(true);
    cdflween.initialize(true);
    dtflween.initialize(true);
    
    mCmdReset = false;

    // Reset Method
    SC_METHOD(ResetMethod);
    dont_initialize();
    sensitive << preset_n;
    
    // Clock Method
    SC_METHOD(PCLKMethod);
    dont_initialize();
    sensitive << pclk;
    sensitive << mReadPortEvent;
    
    // ID Authentication Method
    SC_METHOD(IDAuthenticationMethod);
    dont_initialize();
    sensitive << onbd;
    sensitive << serialiden_n;
    sensitive << ocdid1;
    sensitive << ocdidin1;
    sensitive << mReadPortEvent;
    
    // Update Output ports Method
    SC_METHOD(UpdateOutputPortMethod);
    dont_initialize();
    sensitive << mUpdateOutputPortEvent;
    
    // Disable config registers Method
    SC_METHOD(DisableRegConfigMethod);
    
    /// Update in SCDS_Improvement
    // Code Flash Read Access Protection (1)
    SC_METHOD(CodeFlashRead1Method);
    dont_initialize();
    sensitive << cdflid0;
    sensitive << cdflrdidin0;
    sensitive << cdflrdpflg0_n;
    sensitive << mReadPortEvent;
    // Code Flash Read Access Protection (2)
    SC_METHOD(CodeFlashRead2Method);
    dont_initialize();
    sensitive << cdflid1;
    sensitive << cdflrdidin1;
    sensitive << cdflrdpflg1_n;
    sensitive << mReadPortEvent;
    // Data Flash Read Access Protection (1)
    SC_METHOD(DataFlashRead1Method);
    dont_initialize();
    sensitive << dtflid0;
    sensitive << dtflrdidin0;
    sensitive << dtflrdpflg0_n;
    sensitive << mReadPortEvent;
    // Data Flash Read Access Protection (2)
    SC_METHOD(DataFlashRead2Method);
    dont_initialize();
    sensitive << dtflid1;
    sensitive << dtflrdidin1;
    sensitive << dtflrdpflg1_n;
    sensitive << mReadPortEvent;
    // Code Flash Write/Erase Access Protection
    SC_METHOD(CodeFlashWriteEraseMethod);
    dont_initialize();
    sensitive << cdflid1;
    sensitive << cdflweidin1;
    sensitive << mReadPortEvent;
    // Data Flash Write/Erase Access Protection
    SC_METHOD(DataFlashWriteEraseMethod);
    dont_initialize();
    sensitive << dtflid1;
    sensitive << dtflweidin1;
    sensitive << mReadPortEvent;

    // TLM AT, LT
    vpcl::tlm_if_tgt_parameter tgt_param = vpcl::tlm_tgt_if<32, tlm::tlm_base_protocol_types, 2>::tgt_get_param(emCPUID);
    tgt_param.fw_req_phase = tlm::END_REQ;
    vpcl::tlm_tgt_if<32, tlm::tlm_base_protocol_types, 2>::tgt_set_param(emCPUID, tgt_param);
    SetTLMlatency(emCPUID);
}//}}}


// Destructor of Cscds class
Cscds::~Cscds()
{//{{{
    for (unsigned int i = 0; i <= emMaxIndex; i++) {
        delete greg[i];
        greg[i] = NULL;
    }
}//}}}


// Process debug transport request
unsigned int Cscds::tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans)
{//{{{
    // Get Generic Payload Attributes
    tlm::tlm_command command = tlm::TLM_IGNORE_COMMAND;
    sc_dt::uint64 address = 0;
    unsigned char *data_ptr = NULL;
    unsigned int data_length = 0;
    bool status = tgt_get_gp_attribute(command, address, data_ptr, data_length, trans, true);
    sc_assert(data_ptr != NULL);
    
    // Call debug functions of RegisterIF or FPSYSWriteAccess() depending on socket ID
    if (command == tlm::TLM_READ_COMMAND) {
        if (id == emCPUID) {
            memset(data_ptr, 0, data_length);
            status = reg_rd_dbg((unsigned int)address, data_ptr, data_length);
        }
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        if (id == emFPSYSID) {
            status = FPSYSWriteAccess((unsigned int)address, data_ptr, data_length);
        } else if (id == emCPUID) {
            status = reg_wr_dbg((unsigned int)address, data_ptr, data_length);
        }
    } else {
        // TLM_IGNORE_COMMAND
        status = true;
        data_length = 0;
    }

    // Set response status & return value
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    if (status) {
        return data_length;
    } else {
        return 0;
    }
}//}}}


// Process transport request
void Cscds::tgt_acc(unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t)
{//{{{
    if(Cgeneral_reset::IsResetPeriod()) {
        re_printf("error","Read/Write access is forbidden while resetting.\n");
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
        return;
    }
    
    // Get Generic Payload Attributes
    tlm::tlm_command command = tlm::TLM_IGNORE_COMMAND;
    sc_dt::uint64 address = 0;
    unsigned char *data_ptr = NULL;
    unsigned int data_length = 0;
    bool status = tgt_get_gp_attribute(command, address, data_ptr, data_length, trans, false);
    
    if (!status) {
        trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
        return;
    }
    sc_assert(data_ptr != NULL);

    // Output warning message if FPSYS access using transport interface
    if (id == emFPSYSID) {
        re_printf("error","FPSYS access using transport interface.\n");
        trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    } else if (id == emCPUID) {
        // Clear FPSYS write permission
        mIsFPSYSWritePermit = false;
        
        // Call to RegWrite() or RegRead()
        if (command == tlm::TLM_READ_COMMAND) {
            memset(data_ptr, 0, data_length);
            status = RegRead((unsigned int)address, data_ptr, data_length);
        } else if (command == tlm::TLM_WRITE_COMMAND) {
            status = RegWrite((unsigned int)address, data_ptr, data_length);
        } else {
            // TLM_IGNORE_COMMAND
            status = true;
        }
    
        // Set response status
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    }
}//}}}


// Write access issued by CPU
bool Cscds::RegWrite(const unsigned int addr, const unsigned char *p_data, const unsigned int size)
{//{{{
    sc_assert(p_data != NULL);
    unsigned char temp_data[4] = {0};
    // Get the relative address of register
    unsigned int reg_addr = addr & emRegAddrMask;
    
    // Relative address of register must be a multiple of 4
    if ((reg_addr & 0x3) == 0) {
        // Copy written data to temporary variable
        for (unsigned int i = 0; i < 4; i++) {
            temp_data[i] = p_data[i];
        }
    
        // Calculate the register index
        unsigned int reg_index = reg_addr >> 2;
        // Only apply register checking & data conversion to supported register
        if (IsSupportedReg(reg_index)) {
            // Check register configuration
            if (!IsRegAccessAllowed(reg_index, true, true)) {
                // Return if CPU does not have permission to write to register
                return true;
            }
        
            // Convert the temp data
            ConvertWriteData(reg_index, temp_data);
        }
    }
    
    return reg_wr(addr, temp_data, size);
}//}}}


// Read access issued by CPU
bool Cscds::RegRead(const unsigned int addr, unsigned char *p_data, const unsigned int size)
{//{{{
    sc_assert(p_data != NULL);
    // Get the relative address of register
    unsigned int reg_addr = addr & emRegAddrMask;
    
    // Relative address of register must be a multiple of 4
    if ((reg_addr & 0x3) == 0) {
        // Calculate the register index
        unsigned int reg_index = reg_addr >> 2;
        
        // Only apply register checking to supported register
        if (IsSupportedReg(reg_index)) {
            // Check register configuration
            if (!IsRegAccessAllowed(reg_index, true, false)) {
                // CPU does not have permission to read from register
                re_printf("info","Read from register GREG %d is not permitted, return 0x00000000.\n", reg_index);
                // Set return data to 0x00000000
                for (unsigned int i = 0; i < 4; i++) {
                    p_data[i] = 0;
                }
                return true;
            }
        }
    }

    return reg_rd(addr, p_data, size);
}//}}}


// Write access issued by FPSYS
bool Cscds::FPSYSWriteAccess(const unsigned int fpsys_addr, const unsigned char *p_data, const unsigned int size)
{//{{{
    sc_assert(p_data != NULL);
    // Check FPSYS write permission
    if ((!mIsFPSYSWritePermit) && (!frefseq.read())) {
        re_printf("error","FPSYS does not have permission to write to SCDS register.\n");
        return false;
    }
    
    // Access size must be 16 bytes
    if (size != 16) {
        re_printf("error", "Writing access size for Extra mat 3/Extra mat A at address 0x%08X is wrong: %d byte(s).\n", fpsys_addr, size);
        return false;
    }
    
    // Get the value of lataddr[14:12]
    unsigned int temp_addr = fpsys_addr & emConfigMatMask;
    
    if (temp_addr == emExtraMat3) {
        // Sent data belong to Extra mat 3 area
        return ExtraMat3WriteAccess(fpsys_addr, p_data, size);
    } else if (temp_addr == emExtraMatA) {
        // Sent data belong to Extra mat A area
        return ExtraMatAWriteAccess(fpsys_addr, p_data, size);
    } else {
        re_printf("error","Sent data does not belong to Extra mat A or Extra mat 3.\n");
        return false;
    }
}//}}}


// Process write access from Extra mat 3
bool Cscds::ExtraMat3WriteAccess(const unsigned int fpsys_addr, const unsigned char *p_data, const unsigned int size)
{//{{{
    sc_assert(p_data != NULL);
    // Get the value of lataddr[11:0]
    unsigned int data_addr = fpsys_addr & emFPSYSAddrMask;
    unsigned int first_index = 0;
    unsigned char temp_data[4] = {0};
    bool result = false;
    
    // lataddr[11:0] value must be a multiple of 16
    if ((data_addr & 0xF) != 0) {
        re_printf("error","Invalid latch address 0x%04X.\n", fpsys_addr);
        return false;
    }
    
    // Check the boundary of lataddr[11:0]
    if (data_addr > emMaxExtraMat3Addr) {
        // data address is greater than maximum address of Extra mat 3
        re_printf("error","The address in lataddr[11:0] is out of range.\n");
        return false;
    }
        
    re_printf("info","Data is sent from Extra mat 3.\n");

    // Calculate the index of the first register in 4 consecutive registers
    first_index = 48 + (data_addr >> 2);
        
    // Write data to registers each in turn
    for (unsigned int i = 0; i < 4; i++) {
        // Copy written data to temporary variable
        for (unsigned int j = 0; j < 4; j++) {
            temp_data[j] = p_data[i*4 + j];
        }
        
        // Only apply register checking & data conversion to supported register
        if (IsSupportedReg(first_index + i)) {
            // Check register configuration
            if (!IsRegAccessAllowed(first_index + i, false, true)) {
                // Check the next register index
                continue;
            }
            
            // Convert the temp data
            ConvertWriteData(first_index + i, temp_data);
        }
        
        // Get the register address based on the index
        unsigned int reg_addr = (first_index +i) << 2;
        
        // Write data to register using debug function
        result = reg_wr_dbg(reg_addr, temp_data, 4);
            
        // Check the result of each writing
        if (!result) {
            return false;
        }
    }

    // all 4 registers is written successfully
    return true;
}//}}}


// Process write access from Extra mat A
bool Cscds::ExtraMatAWriteAccess(const unsigned int fpsys_addr, const unsigned char *p_data, const unsigned int size)
{//{{{
    sc_assert(p_data != NULL);
    // Get the value of lataddr[11:0]
    unsigned int data_addr = fpsys_addr & emFPSYSAddrMask;
    unsigned int reg_index = 0;
    unsigned char temp_data[4] = {0};
    
    // lataddr[11:0] value must be a multiple of 4
    if ((data_addr & 0x3) != 0) {
        re_printf("error","Invalid latch address 0x%04X.\n", fpsys_addr);
        return false;
    }
    
    // Check the boundary of lataddr[11:0]
    if ((data_addr < (unsigned int)emMinExtraMatA0Addr) ||
    (((unsigned int)emMaxExtraMatA0Addr < data_addr) && (data_addr < (unsigned int)emMinExtraMatA1Addr)) ||
    (data_addr > (unsigned int)emMaxExtraMatA1Addr)) {
        re_printf("error","The address in lataddr[11:0] is out of range.\n");
        return false;
    }
        
    // Get the last byte of the address
    unsigned int matA_addr = data_addr & 0xFF;
    re_printf("info","Data is sent from Extra mat A.\n");
    
    // Copy written data to temporary variable
    for (unsigned int i = 0; i < 4; i++) {
        temp_data[i] = p_data[i];
    }
    
    // Calculate register index
    reg_index = (matA_addr - 0x40) >> 2;

    // Only apply register checking & data conversion to supported register
    if (IsSupportedReg(reg_index)) {
        // Check register configuration
        if (!IsRegAccessAllowed(reg_index, false, true)) {
            return true;
        }
        
        // Convert the temp data
        ConvertWriteData(reg_index, temp_data);
    }
    
    // Get the register address based on the index
    unsigned int reg_addr = reg_index << 2;

    // Write the first 4-bytes of data to register using debug function
    return reg_wr_dbg(reg_addr, temp_data, 4);
}//}}}


// Return TRUE if accessed register is supported by model
bool Cscds::IsSupportedReg(const unsigned int index)
{//{{{
    // Check whether index belong to supported register index
    // mFactorIndexGREG[] array & emNUM_GREG enum are declared protected in scds_regif.h
    for (unsigned int i = 0; i < emNUM_GREG; i++) {
        if (index == mFactorIndexGREG[i]) {
            return true;
        }
    }
    
    return false;
}//}}}


// Check the registers enable/disable
bool Cscds::IsRegAccessAllowed(const unsigned int index, const bool is_CPU, const bool is_WriteAccess)
{//{{{
    // Check register implemented/unimplemented attribute
    if (!mRegConfig[index].implement) {
        re_printf("warning","Unable to access to unimplemented register GREG %d.\n", index);
        return false;
    }
    
    // If read/write access is issued by CPU, check enable read/write access signal
    if (is_CPU) {
        if (is_WriteAccess) {
            // Check the value of the pin correspond with register index in tf_wr port
            sc_biguint<128> is_write_enable = tf_wr.read();
            if (is_write_enable[index] == 0) {
                re_printf("warning","Unable to write to disable register GREG %d.\n", index);
                return false;
            }
        } else {
            // Check the value of the pin correspond with register index in tf_rd port
            sc_biguint<128> is_read_enable = tf_rd.read();
            if (is_read_enable[index] == 0) {
                re_printf("warning","Read from disable register GREG %d.\n", index);
                return false;
            }
        }
    }
    
    return true;
}//}}}


// Convert the data based on the bit size of register
void Cscds::ConvertWriteData(const unsigned int index, unsigned char *p_data)
{//{{{
    sc_assert(p_data != NULL);
    // Check register bit size
    if (mRegConfig[index].size < emMaxRegSize) {
        // Bit size < 32
        // Calculate the byte position of bit size (Ex: bit size = 20 -> pos = 2)
        // Byte position take value from 0 to 3; equivalent to p_data[0] to p_data[3]
        unsigned int pos = mRegConfig[index].size >> 3;
        // Write 0 to bit which order number is higher than (bit size - 1)
        p_data[pos] = p_data[pos] & (unsigned int)((1 << (mRegConfig[index].size & 0x7)) - 1);
        
        // Store 0x00 to bytes which position is greater than pos
        for (unsigned int i = pos+1; i < 4; i++) {
            p_data[i] = 0x00;
        }
    }
}//}}}


// Callback which corresponds with register write access
void Cscds::cb_GREG_GREG(RegCBstr str)
{//{{{
    if (mIsZeroClock) {
        return;
    }
    // Notify update output port method
    mUpdateOutputPortEvent.notify();
}//}}}


// Output new value to outuput ports
void Cscds::UpdateOutputPortMethod(void)
{//{{{
    if (Cgeneral_reset::IsResetPeriod() ) {
        // Initialize output ports in reset period
        for (unsigned int i = 0; i <= emMaxIndex; i++) {
            greg[i]->write(0);
        }
    } else {
        for (unsigned int i = 0; i <= emMaxIndex; i++) {
            if (IsSupportedReg(i)) {
                greg[i]->write((sc_uint<32>)(*GREG[i]));
            }
        }
    }

    /// Update in SCDS_Improvement
    seclock_faci.write(mSeclockFaciValue);
    cdflrden0.write(mCdflrden0Value);
    dtflrden0.write(mDtflrden0Value);
    cdflrden1.write(mCdflrden1Value);
    dtflrden1.write(mDtflrden1Value);
    cdflween.write(mCdflweenValue);
    dtflween.write(mDtflweenValue);
}//}}}


// ID authentication process
void Cscds::IDAuthenticationMethod(void)
{//{{{
    if(Cgeneral_reset::IsResetPeriod() || mIsZeroClock) {
        if (mIsZeroClock) {
            re_printf("warning","The pclk period is equal 0.\n");
        }
        return;
    }
    
    // Calculate the value to be output to seclock_faci port
    mSeclockFaciValue = (onbd.read() & serialiden_n.read())     // serial programmer ID is invalid in serial programming mode
                        || (ocdidin1.read() != ocdid1.read());  // Execute ID authentication
    
    // Notify update output port method
    mUpdateOutputPortEvent.notify();
}//}}}


// Disable config registers (called at the start of the simulation)
void Cscds::DisableRegConfigMethod(void)
{//{{{
    // Clear mIsRegConfigEnable flag
    mIsRegConfigEnable = false;
}//}}}


// SetRegEnable handleCommand
std::string Cscds::SetRegEnable(const unsigned int index, const bool is_implement)
{//{{{
    // The register index must be equal or less than 127
    if (index <= emMaxIndex) {
        if (mIsRegConfigEnable) {
            // Config register is permitted
            mRegConfig[index].implement = is_implement;
            return "SetRegEnable command executes SUCCESS.";
        } else {
            return "Setting register implement/not implement is only effective at the start time of simulation.";
        }
    } else {
        return "SetRegEnable command is called with wrong register index.";
    }
}//}}}


// SetRegSize handleCommand
std::string Cscds::SetRegSize(const unsigned int index, const unsigned int size)
{//{{{
    // The register index must be equal or less than 127
    if (index <= emMaxIndex) {
        // The setting size must be between 1 and 32
        if ((emMinRegSize <= size) && (size <= emMaxRegSize)) {
            if (mIsRegConfigEnable) {
                mRegConfig[index].size = size;
                return "SetRegSize command executes SUCCESS.";
            } else {
                return "Setting register bit size is only effective at the start time of simulation.";
            }
       } else {
           return "SetRegSize handleCommand is called with wrong size.";
       }
    } else {
        return "SetRegSize command is called with wrong register index.";
    }
}//}}}


// Called by PCLKMethod() or SetCLKfreq handleCommand
void Cscds::SetCLKfreq(const std::string clk_name, const double clk_freq)
{//{{{
    // Clock name must be "pclk"
    if (clk_name == "pclk") {
        double pclk_period = 0;
        mIsZeroClock = true;
        if (clk_freq > 0) {
            // Calculate the clock period to check the resolution
            mIsZeroClock = false;
            pclk_period = (double)(emNanoSecond / clk_freq);
            double time_unit = (double)(emNanoSecond / Cgeneral_reset::GetTimeResolution());
            if(time_unit > pclk_period){
                re_printf("warning","The pclk period is less than 1 unit time of system.\n");
                return;
            }
        }
        // address decode time = 1 PCLK period
        sc_time clock_value(pclk_period, SC_NS);
        
        // Set clock value to target parameters of CPU socket
        vpcl::tlm_if_tgt_parameter tgt_param = vpcl::tlm_tgt_if<32, tlm::tlm_base_protocol_types, 2>::tgt_get_param(emCPUID);
        tgt_param.bus_clk = clock_value;
        vpcl::tlm_tgt_if<32, tlm::tlm_base_protocol_types, 2>::tgt_set_param(emCPUID, tgt_param);
        // Calculate again the latency
        SetTLMlatency(emCPUID);
        re_printf("info","pclk is set with a frequency as %lld.%06d.\n", (unsigned long long) clk_freq, FRACTION_TO_INT(clk_freq));
    } else {
        re_printf("warning","Clock name = %s is invalid.\n", clk_name.c_str());
    }
}//}}}


// Setting latency for target parameters
void Cscds::SetTLMlatency(const unsigned int id)
{//{{{
    vpcl::tlm_if_tgt_parameter tgt_param = vpcl::tlm_tgt_if<32, tlm::tlm_base_protocol_types, 2>::tgt_get_param(id);
    tgt_param.rd_latency = tgt_param.rd_req_latency + 2 * tgt_param.bus_clk;
    tgt_param.wr_latency = tgt_param.wr_req_latency + 2 * tgt_param.bus_clk;
    vpcl::tlm_tgt_if<32, tlm::tlm_base_protocol_types, 2>::tgt_set_param(id, tgt_param);
}//}}}


void Cscds::PCLKMethod(void)
{//{{{ 
    if(Cgeneral_reset::IsResetPeriod()) {
        return;
    }
    
    double pclk_freq = sc_dt::uint64_to_double(pclk.read());
    // Check & update the new clock
    SetCLKfreq("pclk", pclk_freq);
}//}}}


void Cscds::ResetMethod(void)
{//{{{
    //Check value of preset_n signal
    bool is_PortReset = (preset_n.read() == vpcl::emResetActive) ? true : false;
    
    // Call reset method of common class
    Cgeneral_reset::Com_ResetMethod(is_PortReset);
}//}}}


void Cscds::AssertReset(const double delay, const double period)
{//{{{
    mCmdReset = true;
    // Call assert reset function of Cgeneral_reset class
    Cgeneral_reset::Com_AssertReset(delay, period);
}//}}}


void Cscds::EnableReset(const bool is_active)
{//{{{
    if (is_active) {
        // Iinitialize variables
        Initialize();
        
        // Notify to initialize output ports
        mUpdateOutputPortEvent.notify();
    } else {
        // Reset is negated, update input ports
        if (!Cgeneral_reset::IsSimStarting()) {
            mReadPortEvent.notify();
        }
    }

    // Initialize registers
    Cscds_regif::EnableReset(is_active);
    if(mCmdReset == false) {
        this->tgt_enable_reset(is_active);
    } else {
        mCmdReset = false;
    }

}//}}}


void Cscds::Initialize(void)
{//{{{
    // Initialize variables
    mIsFPSYSWritePermit = true;
    mSeclockFaciValue = false;
    
    /// Update in SCDS_Improvement
    mCdflrden0Value = true;
    mDtflrden0Value = true;
    mCdflrden1Value = true;
    mDtflrden1Value = true;
    mCdflweenValue = true;
    mDtflweenValue = true;
}//}}}


/// Update in SCDS_Improvement
void Cscds::cb_Rsv2_GREG_Rsv2_GREG(RegCBstr str)
{//{{{
    if (mIsZeroClock) {
        return;
    }
    // Output warning message when user access to these registers
    re_printf("warning","GREG%d register is prohibited to use.\n", str.channel);
}//}}}


// Code Flash Read Access Protection (1)
void Cscds::CodeFlashRead1Method(void)
{//{{{
    if (mIsZeroClock) {
        re_printf("warning","The pclk period is equal 0.\n");
        return;
    }
    mCdflrden0Value = FlashIDAuthentication(emCodeReadAccess1, cdflid0.read(), cdflrdidin0.read(), cdflrdpflg0_n.read() );
    
    mUpdateOutputPortEvent.notify();
}//}}}


// Code Flash Read Access Protection (2)
void Cscds::CodeFlashRead2Method(void)
{//{{{
    if (mIsZeroClock) {
        re_printf("warning","The pclk period is equal 0.\n");
        return;
    }
    mCdflrden1Value = FlashIDAuthentication(emCodeReadAccess2, cdflid1.read(), cdflrdidin1.read(), cdflrdpflg1_n.read() );
    
    mUpdateOutputPortEvent.notify();
}//}}}


// Data Flash Read Access Protection (1)
void Cscds::DataFlashRead1Method(void)
{//{{{
    if (mIsZeroClock) {
        re_printf("warning","The pclk period is equal 0.\n");
        return;
    }
    mDtflrden0Value = FlashIDAuthentication(emDataReadAccess1, dtflid0.read(), dtflrdidin0.read(), dtflrdpflg0_n.read() );
    
    mUpdateOutputPortEvent.notify();
}//}}}


// Data Flash Read Access Protection (2)
void Cscds::DataFlashRead2Method(void)
{//{{{
    if (mIsZeroClock) {
        re_printf("warning","The pclk period is equal 0.\n");
        return;
    }
    mDtflrden1Value = FlashIDAuthentication(emDataReadAccess2, dtflid1.read(), dtflrdidin1.read(), dtflrdpflg1_n.read() );
    
    mUpdateOutputPortEvent.notify();
}//}}}


// Code Flash Write/Erase Access Protection
void Cscds::CodeFlashWriteEraseMethod(void)
{//{{{
    if (mIsZeroClock) {
        re_printf("warning","The pclk period is equal 0.\n");
        return;
    }
    mCdflweenValue = FlashIDAuthentication(emCodeWriteAccess, cdflid1.read(), cdflweidin1.read(), false );
    
    mUpdateOutputPortEvent.notify();
}//}}}


// Data Flash Write/Erase Access Protection
void Cscds::DataFlashWriteEraseMethod(void)
{//{{{
    if (mIsZeroClock) {
        re_printf("warning","The pclk period is equal 0.\n");
        return;
    }
    mDtflweenValue = FlashIDAuthentication(emDataWriteAccess, dtflid1.read(), dtflweidin1.read(), false );
    
    mUpdateOutputPortEvent.notify();
}//}}}


// Flash ID Authentication process
bool Cscds::FlashIDAuthentication(const unsigned int type, const sc_biguint<256> flashId, const sc_biguint<256> flashId_in, const bool is_AuthDisable)
{//{{{
    bool is_IdAllOne = true;
    bool result = true;
    
    switch (type) {
        case emCodeReadAccess1 :
        case emCodeReadAccess2 :
        case emDataReadAccess1 :
        case emDataReadAccess2 :
            for (unsigned int i = 0; i < emFlashIdLenghth; i++) {
                if (flashId[i] == 0) {
                    is_IdAllOne = false;
                    break;
                }
            }
            if ((!is_IdAllOne) && (!is_AuthDisable) && (flashId != flashId_in)) {
                // Authentication FAIL
                result = false;
            }
            break;
        default:
            // Code/Data Write/Erase Access Protection
            if (flashId != flashId_in) {
                // Authentication FAIL
                result = false;
            }
            break;
    }
    
    return result;
}//}}}

// vim600: set foldmethod=marker :
