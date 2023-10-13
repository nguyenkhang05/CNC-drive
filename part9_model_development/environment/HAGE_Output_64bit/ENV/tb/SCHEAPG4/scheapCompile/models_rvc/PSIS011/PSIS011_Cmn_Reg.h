// ---------------------------------------------------------------------
// $Id: PSIS011_Cmn_Reg.h,v 1.4 2017/12/07 02:00:02 chanle Exp $
//
// Copyright(c) 2017 Renesas Electronics Corporation
// Copyright(c) 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __PSIS011_CMN_REG_H__
#define __PSIS011_CMN_REG_H__

#include "psis011_cmn_regif.h"

class PSIS011_Func;

class PSIS011_Cmn_Reg: public Cpsis011_cmn_regif
{
public:
    SC_HAS_PROCESS (PSIS011_Cmn_Reg);
    PSIS011_Cmn_Reg(std::string name, PSIS011_Func* PSIS011Func);
    ~PSIS011_Cmn_Reg();

    void EnableReset(const bool is_active);
    void Initialize();
    void RegisterHandler(const std::vector<std::string> &args);
    void ClearPUCLBStepDone();
    unsigned int GetPUCLBStepDone();
    unsigned int GetACSTSBit();
    unsigned int GetMSTSBit();
    void StoreUARTData(unsigned int data, bool framing_error, bool parity_error);
    void GetClockDivider(unsigned int &rx_divider, unsigned int &tx_divider);
    unsigned int GetPSI5SPUPTSReg();
    void GetConfigCPUTxCom(unsigned int &numfrm, sc_dt::uint64 &tx_cpu_com_data);
    unsigned int GetUTTDTBit();
    void SetTXSTSBit(unsigned int value);
    unsigned int GetNFSETBit();
    void UpdateUTTFINBit(unsigned int value);
    void UpdatePSI5SUCTMReg(std::string bit_name, unsigned int value);
    void DumpStatusInfo();
    void ClearAllStatus();
    void SetPSI5SPTFISReg(unsigned int value);
    void SetSWSTSBit(unsigned int value);
    bool GetLoopbackEnable();

    void read (unsigned int address, unsigned char *p_data, unsigned int size, bool debug);
    void write (unsigned int address, unsigned char *p_data, unsigned int size, bool debug);

private:
    // enum

    // Variables
    PSIS011_Func*   mpPSIS011Func;      // PSIS011_Func pointer
    std::string     mModuleName;        // Module name
    unsigned int    mPUCLBStepDone;     // Indicate done step (writing to PSI5SPUCLB register)
    bool            mLoopbackEnable;    // Indicate loopback is enabled or not
    bool            mWasReadUARTRxData; // Indicate the received UART data was read

    // Function
    void UpdatePUOSReg();
    void ConfigTimestamp(std::string timestamp_name);

    // Callback
    void cb_PSI5SPUOEB_OPEN(RegCBstr str);
    void cb_PSI5SPUOMD_OPMD(RegCBstr str);
    void cb_PSI5SPUNFST_NFSET(RegCBstr str);
    void cb_PSI5SPUSWR_SWRST(RegCBstr str);
    void cb_PSI5SPRMBC_MBCLR(RegCBstr str);
    void cb_PSI5SPUCLB_LBEN(RegCBstr str);
    void cb_PSI5SPUPTS_UTPRTY(RegCBstr str);
    void cb_PSI5SPUPTS_URPRTY(RegCBstr str);
    void cb_PSI5SPUBCE_SCKEN(RegCBstr str);
    void cb_PSI5SPUBPR_RXOSMP(RegCBstr str);
    void cb_PSI5SPUBPR_SCKPRS(RegCBstr str);
    void cb_PSI5SPTPS_TSPRSL(RegCBstr str);
    void cb_PSI5SPTCAS_TSCACLS(RegCBstr str);
    void cb_PSI5SPTCAS_TSCAEBS(RegCBstr str);
    void cb_PSI5SPTCAS_TSCACKS(RegCBstr str);
    void cb_PSI5SPTCBS_TSCBCLS(RegCBstr str);
    void cb_PSI5SPTCBS_TSCBEBS(RegCBstr str);
    void cb_PSI5SPTCBS_TSCBCKS(RegCBstr str);
    void cb_PSI5SPTCAE_TSCAEB(RegCBstr str);
    void cb_PSI5SPTCAC_TSCACLR(RegCBstr str);
    void cb_PSI5SPTCBE_TSCBEB(RegCBstr str);
    void cb_PSI5SPTCBC_TSCBCLR(RegCBstr str);
    void cb_PSI5SPATCE_ATSCEB(RegCBstr str);
    void cb_PSI5SPATCC_ATSCCLR(RegCBstr str);
    void cb_PSI5SUCRIE_IERPE(RegCBstr str);
    void cb_PSI5SUCTIE_IETOWE(RegCBstr str);
    void cb_PSI5SUCDRE_DRQEURFN(RegCBstr str);
    void cb_PSI5SUCRD_UTRDT(RegCBstr str);
    void cb_PSI5SUCRSC_UTRPECL(RegCBstr str);
    void cb_PSI5SPTFST_TXST(RegCBstr str);
    void cb_PSI5SPTFNM_TXNUM(RegCBstr str);
    void cb_PSI5SPTFD1_TDT4(RegCBstr str);
    void cb_PSI5SPTFD1_TDT3(RegCBstr str);
    void cb_PSI5SPTFD1_TDT2(RegCBstr str);
    void cb_PSI5SPTFD1_TDT1(RegCBstr str);
    void cb_PSI5SPTFD2_TDT8(RegCBstr str);
    void cb_PSI5SPTFD2_TDT7(RegCBstr str);
    void cb_PSI5SPTFD2_TDT6(RegCBstr str);
    void cb_PSI5SPTFD2_TDT5(RegCBstr str);
    void cb_PSI5SUCTD_UTTDT(RegCBstr str);
    void cb_PSI5SUCTSC_UTTOWECL(RegCBstr str);

};
#endif //__PSIS011_CMN_REG_H__
