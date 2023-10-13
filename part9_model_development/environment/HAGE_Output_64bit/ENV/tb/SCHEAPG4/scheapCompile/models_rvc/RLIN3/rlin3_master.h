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

#ifndef __RLIN3_MASTER_H__
#define __RLIN3_MASTER_H__
#include "systemc.h"
#include "rlin3_master_regif.h"
#include "rlin3.h"
#include "rlin3_common.h"
#include "tlm.h"
#include "tlm_tgt_if.h"
#include "iterator"
#include "re_define.h"
#include <sstream>

class Crlin3;

/// RLIN3 model class
class Crlin3_master: public Crlin3_master_regif, public Crlin3_common
{
public:
    //----------------------pins----------------------//
    sc_in<sc_uint<32> > rx_data;    //connect to RX_DATA of Rlin3 
    sc_in<sc_uint<32> > rx_control; //connect to RX_CONTROL of Rlin3
    sc_out<sc_uint<32> > tx_data;   //connect to TX_DATA of Rlin3 
    sc_out<sc_uint<32> > tx_control;//connect to TX_CONTROL of Rlin3
  
    //Constructor
    SC_HAS_PROCESS(Crlin3_master);
    Crlin3_master(sc_module_name name, Crlin3 *parent);
    // Destructor
    virtual ~Crlin3_master();

    void ResetMaster(bool is_active);        //Reset master when hw reset occurs 
    void SetMasterClock(double clkc, double pclk);        //Called by rlin3 class to set LIN clock to master class
    void UpdateMasterRegs(RlinRegs rlin_reg);//Called by rlin3 class to update all register when change mode
    std::string GetCurrentStatus();          //Called by rlin3 class to get current status of master class when in master mode
    void Lin3EnterSelfTest();                //Called by rlin3 class when LIN enter is enterred selftest mode
    std::string master_reg_command(const std::vector<std::string>& args);              //Called by rlin3 class to pass reg command 
    bool master_reg_rd(unsigned int addr,unsigned char *p_data,unsigned int size);     //Called by rlin3 class to call reg_rd of master class
    bool master_reg_rd_dbg(unsigned int addr,unsigned char *p_data,unsigned int size); //Called by rlin3 class to call reg_rd_dbg of master class
    bool master_reg_wr(unsigned int addr,unsigned char *p_data,unsigned int size);     //Called by rlin3 class to call reg_wr of master class
    bool master_reg_wr_dbg(unsigned int addr,unsigned char *p_data,unsigned int size); //Called by rlin3 class to call reg_wr_dbg of master class
    void SoftwareReset(unsigned int om0_val, RlinRegs rlin_reg, bool status);          //Software reset

private:
    //----------------------Event---------------------//
    sc_event mTimeoutEvent;            //Event to notify TimeoutHandlingMethod 
    sc_event mStartRespondEvent;       // Add by SonTran: No.2 Remove transmitting Respond space at the end of header 
 
    //----------------------Variable---------------------//
    bool mIsEnterSelfTest;             //Indicate LIN is in normal mode or enter self test mode
    Crlin3 *parent_pointer;            //Pointer of Crlin3 class
    eLIN_STATUS  mStatus;              //Indicate status is reset, idle or operation to check RW resgiter permission
    unsigned int mWriteLSTCTimes;      //Count number of write sequence key to enter self test mode
    unsigned int mDataValue;           //Stored data value to compare when input data change to process Wakup mode
    bool mIsFinishHeader;              //Indicate that Frame Header is transmited 

    //Function
    void TimeoutChecking();            //Function is called when start operation of LIN mode when enable time out

    //method
    void TimeoutHandlingMethod(void);  //Method progress timeout operations
    void ReceiveMethod(void);          //Method progress receive operation
    void TransRespondMethod(void);     // Add by SonTran: No.2 Remove transmitting Respond space at the end of header 

    // overwritten virtual functions
    void OutputData(unsigned int tx_control, unsigned int tx_data); //virtual function to output data to Master
    void UpdateStatus(eSTATUS_FLAG flag); //virtual function to update status register and output interrupt if any
    void UpdateErrorStatus (eERROR_FLAG error_kind); // virtual function for handling error occur
    void UpdateRegisters(eREG_KIND reg_kind, unsigned int value); //virtual function for updating baud rate or data register
    void _re_printf(const std::string group, const char *message, ...) {};//virtual function for outputing debug message // Not used in Master mode
    void UpdateRegsOfLin3();
    void CheckWriteLDBN(RegCBstr str, vpcl::re_register *reg, unsigned int index);
    
    //Declaration callback functions
    void cb_LWBR_LWBR0(RegCBstr str);
    void cb_LBRP0_LBRP0(RegCBstr str);
    void cb_LBRP1_LBRP1(RegCBstr str);
    void cb_LSTC_LSTM(RegCBstr str);
    void cb_LMD_LMD(RegCBstr str);
    void cb_LBFC_BLT(RegCBstr str);
    void cb_LSC_IBHS(RegCBstr str);
    void cb_LWUP_WUTL(RegCBstr str);
    void cb_LIE_FTCIE(RegCBstr str);
    void cb_LEDE_BERE(RegCBstr str);
    void cb_LCUC_OM0(RegCBstr str);
    void cb_LTRC_FTS(RegCBstr str);
    void cb_LMST_OMM0(RegCBstr str);
    void cb_LST_FTC(RegCBstr str);
    void cb_LEST_BER(RegCBstr str);
    void cb_LDFC_RFDL(RegCBstr str);
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
    void cb_LUTDR_UTD(RegCBstr str); // Add by SonTran: No.7-1 LUTDR and LUWTDR are the same register but differ address
    void cb_LURDR_URD(RegCBstr str);
    void cb_LUWTDR_UWTD(RegCBstr str); // Add by SonTran: No.7-1 LUTDR and LUWTDR are the same register but differ address

};

#endif //__RLIN3_H__
