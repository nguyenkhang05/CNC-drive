// ---------------------------------------------------------------------
// $Id: $
//
// Copyright(c) 2012-2020 Renesas Electronics Corporation
// Copyright(c) 2012-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __RLIN3_H__
#define __RLIN3_H__
#include "systemc.h"
#include "rlin3_regif.h"
#include "rlin3_master.h"
#include "rlin3_slave.h"
#include "rlin3_uart.h"
#include "rlin3_selfTest.h"
#include "rlin3_common.h"
#include "tlm.h"
#include "tlm_tgt_if.h"
#include "iterator"
#include "re_define.h"
#include <sstream>

class Crlin3_master;
class Crlin3_slave;
class Crlin3_uart;
class Crlin3_selftest;
class Crlin3_common;

//===============Crlin3 class=======================
class Crlin3: public sc_module 
           ,public Crlin3_regif
           ,public vpcl::tlm_tgt_if<32>
{
friend class Crlin3_master;
friend class Crlin3_slave;
friend class Crlin3_uart;
friend class Crlin3_selftest;
#include "rlin3_cmdif.h"
public:
    //Declare input ports
    sc_in<bool>  rstc_n;
    sc_in<bool>  preset_n;
    sc_in<sc_dt::uint64 > pclk;
    sc_in<sc_dt::uint64 > clkc;
    sc_in<sc_uint<32> > RX_DATA;
    sc_in<sc_uint<32> > RX_CONTROL;

    //Declare output ports
    sc_out<sc_uint<32> > TX_DATA;
    sc_out<sc_uint<32> > TX_CONTROL;
    sc_out<bool>  lin3_int_t;
    sc_out<bool>  lin3_int_r;
    sc_out<bool>  lin3_int_s;

    //Delare signal use for connect master, slave, uart classes with selftest class 
    sc_signal<sc_uint<32> > tx_data_m_sig;
    sc_signal<sc_uint<32> > tx_control_m_sig;
    sc_signal<sc_uint<32> > tx_data_s_sig;
    sc_signal<sc_uint<32> > tx_control_s_sig;
    sc_signal<sc_uint<32> > tx_data_u_sig;
    sc_signal<sc_uint<32> > tx_control_u_sig;
    sc_signal<sc_uint<32> > rx_data_m_sig;
    sc_signal<sc_uint<32> > rx_control_m_sig;
    sc_signal<sc_uint<32> > rx_data_s_sig;
    sc_signal<sc_uint<32> > rx_control_s_sig;
    sc_signal<sc_uint<32> > rx_data_u_sig;
    sc_signal<sc_uint<32> > rx_control_u_sig;

    //Construct and Destruct
    SC_HAS_PROCESS(Crlin3);
    Crlin3(sc_module_name name);
    ~Crlin3();

private:
    //Declare event
    sc_event mDeAssertIntrEvent;         //Event use for notify DeAssertIntrMethod to de-assert interrupt after 1 cycle of LIN clock
    sc_event mCmdResetEvent;             //Event use for notify CmdResetMethod when reset by handleCommand occurs
    sc_event mCancelCmdResetEvent;       //Event use for notify CancelCmdResetMethod when reset by handleCommand is cancelled
    sc_event mWriteLin3IntEvent;         //Event use for notify WriteLin3IntMethod when interrupts asserts or de-assert
                                         
    //Declare internal variable          
    Crlin3_master   *mMaster;            //Instance of master class
    Crlin3_slave    *mSlave;             //Instance of slave class
    Crlin3_uart     *mUart;              //Instance of uart class
    Crlin3_selftest *mSelfTest;          //Instance of selftest class 
    bool mIsCmdResetStatus;              //Indicate handleCommand reset status
    bool mRlin3Reset;                    //Reset by port
    bool mCmdReset;                      //Reset by handleCommand
    double mResetPeriod;                 //HandleCommand reset period time
    unsigned int mTransmitDataAmount;    //Store number of bytes which transmitted to dump static info
    unsigned int mReceiveDataAmount;     //Store number of bytes which received to dump static info
    double mPCLK_freq;                   //Store PCLK frequency value
    double mCLKC_freq;                   //Store CLKC frequency value
    Crlin3_common::eOPERATION_MODE mMode; //Store current mode 
    Crlin3_common::eLIN_STATUS  mStatus; //Store current status
    bool mLin3IntT;                      //store lin3_int_t value
    bool mLin3IntR;                      //store lin3_int_r value
    bool mLin3IntS;                      //store lin3_int_s value
    bool mIsSelfTest;


    enum eRLIN3_MODE {
        emRLIN3_MASTER = 0x0
       ,emRLIN3_UART   = 0x1
       ,emRLIN3_SLAVE  = 0x2
    };


    //methods
    void DeAssertIntrMethod(void);          //Method use to de-assert interrupt after 1 cycle of LIN clock
    void ResetMethod (void);                //Method use to handle hw reset
    void CmdResetMethod(void);              //Method use to handle hw reset from command if 
    void CancelCmdResetMethod(void);        //Method use to handle cancelling hw reset from command if
    void CLKCPeriodMethod(void);            //Method use to handle CLKC clock when CLKC clock change 
    void PCLKPeriodMethod(void);            //Method use to handle PCLK clock when PCLK clock change
    void WriteLin3IntMethod(void);          //write output lin3_int_t interrupt function



    //Internal function
    void Initialize(void);                  //Function use for initialize global data member in contructor and when hw reset occurs
    void EnableReset(const bool is_active); //Function use for handle when hw rwset assert or de-assert
    void AssertReset(const double delay,const double period); //Function use for handle hw reset when reset is called by handleCommand
    void DumpInfo(const char *type, const char *message, ...);//Function use for re_printf function
    void DumpStatInfo(void);                                  //Function use for dump static info when user call DumpStatInfo command
    void SetCLKfreq(std::string clk_name, double clk_freq);   //Function use to set clocks when user call SetCLKfreq command
    void DumpInterruptMsg(Crlin3_common::eINTERRUPT_KIND interrupt_id, bool int_assert); //Function use for dump interrupt info when interrupts asserts
    unsigned int GetRegBitsVal(unsigned int reg, unsigned int lower_index,unsigned int upper_index); //Function use for getting current value of bits
    unsigned int SetRegBitsVal(unsigned int reg, unsigned int pre_reg, unsigned int lower_index, unsigned int upper_index); ////Function use for setting current value of bits base on each mode
    double GetTimeResolution(void);           //Function use for change time unit 
    void SetLatency_TLM(void);                //Function use for setting latency when LT mode
    void DumpOperationInfo(const char *frame_name, const char *operation, unsigned int id_val, unsigned int data_val, const char *no_cksum, unsigned int cksum_val);            //This function use to dump operation infor when rlin3 transmit/receive data
    void UpdateRlin3Reg(Crlin3_common::RlinRegs rlin_reg);//update value of registers to other modes 
    void UpdateRegReset(Crlin3_common::RlinRegs rlin_reg);//update value of registers to other modes
    void SetSelfTestFunc(bool is_selftest);   //this function use to enable/disable selftest mode
    void UpdateAllRegs();                   //this function is called to update RLIN3 value of register to Master, Slave and Uart class when SW reset assert
    void UpdateAllRegsReset();                   //this function is called to update RLIN3 value of register to Master, Slave and Uart class when SW reset assert
    bool SelftestAccessCheck (vpcl::re_register *reg, bool is_wr, uint pre_data, uint status, bool is_selftest); // This function is used to check access register in Selftest mode // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode.
    void RegisterAccessCheck (vpcl::re_register *reg, bool is_wr, uint pre_data, std::string bit_name, Crlin3_common::eACCESS_MODE expected_access, unsigned int lower_index,unsigned int upper_index);   //this function use to update bits of registers of RLIN3 base on each mode 
    void WriteLin3IntT(bool value);           //write output lin3_int_t interrupt function
    void WriteLin3IntR(bool value);           //write output lin3_int_r interrupt function
    void WriteLin3IntS(bool value);           //write output lin3_int_s interrupt function
    void SoftwareReset(unsigned int om0_val, unsigned int mode_change); //software reset

    //reg handleCommand API
    std::string RegIfCommand(const std::vector<std::string>& args);

    //tlm_tgt_if api functions
    void tgt_acc(tlm::tlm_generic_payload &trans, sc_time &t); 
    unsigned int tgt_acc_dbg(tlm::tlm_generic_payload &trans); 
};
#endif //__RLIN3_H__
