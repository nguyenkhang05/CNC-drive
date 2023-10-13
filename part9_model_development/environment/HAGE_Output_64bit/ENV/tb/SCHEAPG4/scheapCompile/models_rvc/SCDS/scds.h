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

#ifndef __SCDS_H__
#define __SCDS_H__
#include "tlm_tgt_if.h"
#include "scds_regif.h"
#include "re_define.h"

#include "general_reset.h"

/// SCDS model class
class Cscds: public Cgeneral_reset
            ,public vpcl::tlm_tgt_if<32, tlm::tlm_base_protocol_types, 2>
            ,public Cscds_regif
{
    
#include "scds_cmdif.h"

public:
    SC_HAS_PROCESS(Cscds);
    Cscds(sc_module_name name);
    ~Cscds();

    // clock
    sc_in<sc_dt::uint64> pclk;
    // reset signal
    sc_in<bool> preset_n;
    // Read/Write permission
    sc_in<sc_biguint<128> > tf_wr;
    sc_in<sc_biguint<128> > tf_rd;
    // ID authentication compare data
    sc_in<sc_biguint<256> > ocdidin1;
    sc_in<sc_biguint<256> > ocdid1;
    // ID authentication control signal
    sc_in<bool> onbd;
    sc_in<bool> serialiden_n;
    sc_in<bool> frefseq;
    
    // Data output (register value)
    sc_out<sc_uint<32> > *greg[128];
    // ID authentication result
    sc_out<bool> seclock_faci;
    
    /// Update in SCDS_Improvement
    // Flash ID input
    sc_in<sc_biguint<256> > cdflrdidin1;
    sc_in<sc_biguint<256> > dtflrdidin1;
    sc_in<sc_biguint<256> > cdflweidin1;
    sc_in<sc_biguint<256> > dtflweidin1;
    sc_in<sc_biguint<256> > cdflrdidin0;
    sc_in<sc_biguint<256> > dtflrdidin0;
    // Flash ID compare
    sc_in<sc_biguint<256> > cdflid0;
    sc_in<sc_biguint<256> > dtflid0;
    sc_in<sc_biguint<256> > cdflid1;
    sc_in<sc_biguint<256> > dtflid1;
    // Flash authentication flags
    sc_in<bool> cdflrdpflg0_n;
    sc_in<bool> dtflrdpflg0_n;
    sc_in<bool> cdflrdpflg1_n;
    sc_in<bool> dtflrdpflg1_n;
    
    // Flash access enable result
    sc_out<bool> cdflrden0;
    sc_out<bool> dtflrden0;
    sc_out<bool> cdflrden1;
    sc_out<bool> dtflrden1;
    sc_out<bool> cdflween;
    sc_out<bool> dtflween;
    
private:
    
    // Constance value
    enum eConstance {
        emNanoSecond = 1000000000
        // Bit mask to get relative address value
       ,emRegAddrMask = 0x1FF
       ,emFPSYSAddrMask = 0xFFF
        // Checking flash memory area
       ,emConfigMatMask = 0x7000
       ,emExtraMat3 = 0x4000
       ,emExtraMatA = 0x1000
        // Flash memory address boundary
        // Extra mat 3
       ,emMaxExtraMat3Addr = 0x120
        // Extra mat A (EEP area 0)
       ,emMinExtraMatA0Addr = 0x40
       ,emMaxExtraMatA0Addr = 0x0FC
        // Extra mat A (EEP area 1)
       ,emMinExtraMatA1Addr = 0x140
       ,emMaxExtraMatA1Addr = 0x1FC
        // Register index & size
       ,emMaxIndex = 127
       ,emMinRegSize = 1
       ,emMaxRegSize = 32
        /// Update in SCDS_Improvement
        ,emFlashIdLenghth = 256
    };
    
    // Socket ID
    enum eID {
        emCPUID = 0         // m_tgt_socket[0]
       ,emFPSYSID = 1       // m_tgt_socket[1]
    };
    
    /// Update in SCDS_Improvement
    // Flash ID Access Type
    enum eFlashIDType {
        emCodeReadAccess1
       ,emCodeReadAccess2
       ,emDataReadAccess1
       ,emDataReadAccess2
       ,emCodeWriteAccess
       ,emDataWriteAccess
    };
    
    // Register configuration
    struct stRegConfig {
        bool implement;
        unsigned int size;
    } mRegConfig[128];
    
    // FPSYS write permission
    bool mIsFPSYSWritePermit;
    // Register config permission
    bool mIsRegConfigEnable;
    // seclock_faci value
    bool mSeclockFaciValue;
    // check clock equal 0
    bool mIsZeroClock;
    
    /// Update in SCDS_Improvement
    // Flash access enable result values
    bool mCdflrden0Value;
    bool mDtflrden0Value;
    bool mCdflrden1Value;
    bool mDtflrden1Value;
    bool mCdflweenValue;
    bool mDtflweenValue;

    bool mCmdReset; 
    
    // Update output port
    sc_event mUpdateOutputPortEvent;
    // Update input port after reset
    sc_event mReadPortEvent;
    
    // Functions
    // tlm functions
    void tgt_acc(unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t);
    unsigned int tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans);
    
    // Normal functions
    void EnableReset(const bool is_active);
    void Initialize(void);
    bool FPSYSWriteAccess(const unsigned int fpsys_addr, const unsigned char *p_data, const unsigned int size);
    void SetTLMlatency(const unsigned int id);
    bool RegWrite(const unsigned int addr, const unsigned char *p_data, const unsigned int size);
    bool RegRead(const unsigned int addr, unsigned char *p_data, const unsigned int size);
    bool ExtraMat3WriteAccess(const unsigned int fpsys_addr, const unsigned char *p_data, const unsigned int size);
    bool ExtraMatAWriteAccess(const unsigned int fpsys_addr, const unsigned char *p_data, const unsigned int size);
    bool IsRegAccessAllowed(const unsigned int index, const bool is_CPU, const bool is_WriteAccess);
    void ConvertWriteData(const unsigned int index, unsigned char *p_data);
    bool IsSupportedReg(const unsigned int index);
    
    // handle_command functions
    void AssertReset(const double start_time, const double period);
    std::string SetRegEnable(const unsigned int index, const bool is_implement);
    std::string SetRegSize(const unsigned int index, const unsigned int size);
    void SetCLKfreq(const std::string clk_name, const double clk_freq);
    
    // Methods
    void PCLKMethod(void);
    void ResetMethod(void);
    void IDAuthenticationMethod(void);
    void DisableRegConfigMethod(void);
    void UpdateOutputPortMethod(void);
    
    // Callback functions
    void cb_GREG_GREG(RegCBstr str);
    
    /// Update in SCDS_Improvement
    void cb_Rsv2_GREG_Rsv2_GREG(RegCBstr str);
    void CodeFlashRead1Method(void);
    void CodeFlashRead2Method(void);
    void DataFlashRead1Method(void);
    void DataFlashRead2Method(void);
    void CodeFlashWriteEraseMethod(void);
    void DataFlashWriteEraseMethod(void);
    bool FlashIDAuthentication(const unsigned int type, const sc_biguint<256> flashId, const sc_biguint<256> flashId_in, const bool is_AuthDisable);
};

#endif //__SCDS_H__
