// ---------------------------------------------------------------------
// $Id: PSIS011_Ch_Reg.h,v 1.4 2017/12/07 02:00:02 chanle Exp $
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
#ifndef __PSIS011_CH_REG_H__
#define __PSIS011_CH_REG_H__

#include "psis011_ch_regif.h"
#include "psis011_mbch_regif.h"

class PSIS011_Func;

class PSIS011_Ch_Reg: public Cpsis011_ch_regif
                    , public Cpsis011_mbch_regif
{
public:
    SC_HAS_PROCESS (PSIS011_Ch_Reg);
    PSIS011_Ch_Reg(std::string name, unsigned int chid, PSIS011_Func* PSIS011Func);
    ~PSIS011_Ch_Reg();

    void EnableReset (const bool is_active);
    void Initialize();
    void RegisterHandler (const std::string reg_area, const std::vector<std::string> &args);
    void GetConfigInnerSyncPulse (unsigned int &select, unsigned int &prescaler, unsigned int &expired);
    void GetConfigWDT (unsigned int &syncmode, unsigned int &prescaler, unsigned int &expired, bool &wdt_enable);
    void GetConfigPSI5RxFrame (unsigned int fid, unsigned int& fpkt, unsigned int& fpayload, unsigned int& rfcps);
    unsigned int GetRFCPSBit();
    unsigned int GetMaxIdle();
    void GetConfigTimestamp(unsigned int &ch_en, unsigned int &ts_sel, unsigned int &ts_en, unsigned int &ts_trg_sel);
    void CaptureTimestamp(unsigned int timestamp);
    unsigned int GetCapturedTimestamp();
    bool CheckMBDataWasRead(unsigned int frmid);
    void StoreMBData(unsigned int frame, unsigned int status_reg_val, unsigned int data_reg_val, unsigned int timestamp_reg_val);
    unsigned int GetCommandData();
    void GetDDSRInfo(unsigned int &frame_type, unsigned int &address, unsigned int &data);
    void ClearDDSRSTSBit();
    void ClearAllMailBoxData();
    void SetPSI5SPCISReg(std::string status);
    void DumpStatusInfo();
    void ClearAllStatus();
    void SetPSI5SPRESReg(unsigned int frmid);

    void read (std::string area, unsigned int address, unsigned char *p_data, unsigned int size, bool debug);
    void write (std::string area, unsigned int address, unsigned char *p_data, unsigned int size, bool debug);

private:
    // enum
    enum eRegInfo{
        emBaseOffsetMBCh0           = 0x500
        ,emSizeEachMBChannel        = 0x48
        ,emSizeEachMBFrame          = 0xC
        ,emNumMBFrameInChannel      = 6
    };


    // Variables
    PSIS011_Func*   mpPSIS011Func;  // PSIS011_Func pointer
    std::string     mModuleName;    // Module name
    unsigned int    mChID;          // Channel index
    bool            mWasReadMBFrm[emNumMBFrameInChannel];   // Indicate a MB frame was read or not

    // Function

    // Callback
    void cb_PSI5SPRCF1n_CHEN(Cpsis011_ch_regif::RegCBstr str);
    void cb_PSI5SPRCF2n_F1PAYLD(Cpsis011_ch_regif::RegCBstr str);
    void cb_PSI5SPWDEn_WDTEB(Cpsis011_ch_regif::RegCBstr str);
    void cb_PSI5SPWDPn_WDTPRS(Cpsis011_ch_regif::RegCBstr str);
    void cb_PSI5SPWDEVn_WDTEX(Cpsis011_ch_regif::RegCBstr str);
    void cb_PSI5SPTCDn_CHID(Cpsis011_ch_regif::RegCBstr str);
    void cb_PSI5SPCIEn_IEBCRC(Cpsis011_ch_regif::RegCBstr str);
    void cb_PSI5SPDREn_DRQERFN(Cpsis011_ch_regif::RegCBstr str);
    void cb_PSI5SPSTPn_STPRS(Cpsis011_ch_regif::RegCBstr str);
    void cb_PSI5SPSTEVn_STEX(Cpsis011_ch_regif::RegCBstr str);
    void cb_PSI5SPSTSn_STSEL(Cpsis011_ch_regif::RegCBstr str);
    void cb_PSI5SPRESCn_RERRCLF1(Cpsis011_ch_regif::RegCBstr str);
    void cb_PSI5SPTCDCn_TSCCLR(Cpsis011_ch_regif::RegCBstr str);
    void cb_PSI5SPDDTPn_DDSRTYPE(Cpsis011_ch_regif::RegCBstr str);
    void cb_PSI5SPDDDn_DDSRADR(Cpsis011_ch_regif::RegCBstr str);
    void cb_PSI5SPDDSPn_DDSRSTP(Cpsis011_ch_regif::RegCBstr str);
    void cb_PSI5SPCISCn_ISTCCRC(Cpsis011_ch_regif::RegCBstr str);

};
#endif //__PSIS011_CH_REG_H__
