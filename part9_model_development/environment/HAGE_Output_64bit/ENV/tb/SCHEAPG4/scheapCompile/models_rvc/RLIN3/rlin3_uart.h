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

#ifndef __RLIN3_UART_H__
#define __RLIN3_UART_H__
#include "systemc.h"
#include "rlin3_uart_regif.h"
#include "rlin3_common.h"
#include "rlin3.h"

class Crlin3;

/// RLIN3 model class
class Crlin3_uart: public Crlin3_uart_regif, public Crlin3_common
{
public:
    //----------------------pins----------------------//
    sc_in<sc_uint<32> > rx_data;    //connect to RX_DATA of Rlin3 
    sc_in<sc_uint<32> > rx_control; //connect to RX_CONTROL of Rlin3
    sc_out<sc_uint<32> > tx_data;   //connect to TX_DATA of Rlin3
    sc_out<sc_uint<32> > tx_control;//connect to TX_CONTROL of Rlin3
  
    //Constructor
    SC_HAS_PROCESS(Crlin3_uart);
    Crlin3_uart(sc_module_name name, Crlin3 *parent);
    // Destructor
    ~Crlin3_uart();

    //Function
    void ResetUart(bool is_active); //Reset uart when hw reset occurs
    void SetUartClock(double clkc, double pclk);    //Called by rlin3 class to set LIN clock to uart class
    void UpdateUartRegs(RlinRegs rlin_reg); //Called by rlin3 class to update all register when change mode
    std::string GetCurrentStatus();         //Called by rlin3 class to get current status of master class when in uart mode
    std::string uart_reg_command(const std::vector<std::string>& args);
    bool uart_reg_rd(unsigned int addr,unsigned char *p_data,unsigned int size);     //Called by rlin3 class to call reg_rd of uart class
    bool uart_reg_rd_dbg(unsigned int addr,unsigned char *p_data,unsigned int size); //Called by rlin3 class to call reg_rd_dbg of uart class
    bool uart_reg_wr(unsigned int addr,unsigned char *p_data,unsigned int size);     //Called by rlin3 class to call reg_wr of uart class
    bool uart_reg_wr_dbg(unsigned int addr,unsigned char *p_data,unsigned int size); //Called by rlin3 class to call reg_wr_dbg of uart class
    void SoftwareReset(unsigned int om0_val, RlinRegs rlin_reg, bool status);        //Software reset

private:
    enum eWAIT_TRANS {            //Enum show status of waiting stop bit when transmit in uart mode
        emSingWaitStopBit = 0     //Single transmission , wait stop bit
       ,emSingNoWaitStopBit = 2   //Single transmission , no wait stop bit
       ,emMultiWaitStopBit = 3    //Multi transmission , wait stop bit
       ,emMultiNoWaitStopBit = 4  //Multi transmission , no wait stop bit
       ,emNoTrans = 5             //No Transmission status
    };


    //Variable
    eWAIT_TRANS mWait_Trans;      //Store status of waiting transmission
    bool mIsDataRead;             //Indicate data is read from data register or not to check overrun error
    bool mIsLUTDRWrite;// Indicate data written to LUTDR // Add by SonTran: No.52 In case LUOR1.UTIGTS = 1, if users write to LUTDR before the transmission interrupt occurs, the transmission interrupt does not occur (RLIN3v4)
    bool mIsLUWTDRWrite;             //Indicate whether LUWTDR is written or Not
    Crlin3 *parent_pointer;       //Pointer of Crlin3 class
    unsigned int mTransStatus;           //Indicate transmission status is reset, idle or operation to check RW resgiter permission
    unsigned int mRecvStatus;         //Indicate reception status is reset, idle or operation to check RW resgiter permission
    unsigned int mDataValue;      //Store RX data value
    unsigned int mSingleTxDataValue;// Indicate current data of single transmission // Add by SonTran: No.52 In case LUOR1.UTIGTS = 1, if users write to LUTDR before the transmission interrupt occurs, the transmission interrupt does not occur (RLIN3v4)
    bool mNoMultiTransWaitStopBit;
    bool mCheckAccessLUTDR;
    bool mCheckAccessLUWTDR;
    bool mIsReceiveStopBit; // indicate is receiving stop bit in Uart mode // Add by SonTran: No.55, 57 timing conflict at receiving stop bit

    //Events
    sc_event mSingleTransEvent;
    sc_event mMultiTransEvent;
    sc_event mMultiTransWaitStopBitEvent;

    //Method
    void ReceptionMethod(void);  //Method progress receive operation
    void SingleTransMethod(void); //Method progress single byte transmission operation
    void MultiTransMethod(void); //Method progress multi byte transmission with wait stop bit operation
    void MultiTransWaitStopBitMethod(void); //Method progress multi byte transmission operation

    //Function
    unsigned int AddParity(unsigned int data); //Function use to add parity bit into data to transfer
    
    //overwritten virtual functions
    void OutputData(unsigned int tx_control, unsigned int tx_data); //virtual function to output data to Master
    void UpdateRegisters(eREG_KIND reg_kind, unsigned int value); //virtual function for updating baud rate or data register
    void UpdateStatus(eSTATUS_FLAG flag); //virtual function to update status register and output interrupt if any  
    void UpdateErrorStatus(eERROR_FLAG flag); //virtual function to update error status register  
    void _re_printf(const std::string group, const char *message, ...) {};//virtual function for outputing debug message // Not used in Uart mode
    void UpdateRegsOfLin3();
    void CheckWriteLDBN(RegCBstr str, vpcl::re_register *reg, unsigned int index);

    //call-back functions
    void cb_LWBR_LWBR0(RegCBstr str);
    void cb_LBRP0_BRP(RegCBstr str);
    void cb_LBRP1_BRP(RegCBstr str);
    void cb_LSTC_LSTM(RegCBstr str);
    void cb_LMD_LMD(RegCBstr str);
    void cb_LBFC_UBLS(RegCBstr str);
    void cb_LSC_IBHS(RegCBstr str);
    void cb_LWUP_WUTL(RegCBstr str);
    void cb_LIE_FTCIE(RegCBstr str);
    void cb_LEDE_BERE(RegCBstr str);
    void cb_LCUC_OM0(RegCBstr str);
    void cb_LTRC_RTS(RegCBstr str);
    void cb_LMST_OMM0(RegCBstr str);
    void cb_LST_FTC(RegCBstr str);
    void cb_LEST_BER(RegCBstr str);
    void cb_LDFC_MDL(RegCBstr str);
    void cb_LIDB_ID(RegCBstr str);
    void cb_LCBR_CKSM(RegCBstr str);
    void cb_LUDB0_UDB(RegCBstr str);
    void cb_LDB1_LDB(RegCBstr str);
    void cb_LDB2_LDB(RegCBstr str);
    void cb_LDB3_LDB(RegCBstr str);
    void cb_LDB4_LDB(RegCBstr str);
    void cb_LDB5_LDB(RegCBstr str);
    void cb_LDB6_LDB(RegCBstr str);
    void cb_LDB7_LDB(RegCBstr str);
    void cb_LDB8_LDB(RegCBstr str);
    void cb_LUOER_UTOE(RegCBstr str);
    void cb_LUOR1_UEBE(RegCBstr str);
    void cb_LUTDR_UTD(RegCBstr str);
    void cb_LURDR_URD(RegCBstr str);
    void cb_LUWTDR_UWTD(RegCBstr str);

};
#endif //__RLIN3_H__




