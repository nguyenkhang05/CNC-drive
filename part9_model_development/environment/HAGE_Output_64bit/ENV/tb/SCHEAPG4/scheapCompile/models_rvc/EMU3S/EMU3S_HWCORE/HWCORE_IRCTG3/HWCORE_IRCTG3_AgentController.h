// ---------------------------------------------------------------------
// $Id: HWCORE_IRCTG3_AgentController.h,v 1.4 2020/10/15 06:49:06 khoaanhnguyen Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of HWCORE_IRCTG3_AgentController.h
#ifndef __HWCORE_IRCTG3_AGENT_CONTROLLER_H__
#define __HWCORE_IRCTG3_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

//Registers
#define REG_IR3CCTR          "IR3CCTR"
#define REG_IR3CMODE         "IR3CMODE"
#define REG_IR3RDI           "IR3RDI"
#define REG_IR3TRSMODE       "IR3TRSMODE"
#define REG_IR3TRG           "IR3TRG"
#define REG_IR3INTCLEAR      "IR3INTCLEAR"
#define REG_IR3INTEN         "IR3INTEN"
#define REG_IR3INTST         "IR3INTST"
#define REG_IR3UINTMEM       "IR3UINTMEM"
#define REG_IR3VINTMEM       "IR3VINTMEM"
#define REG_IR3WINTMEM       "IR3WINTMEM"
#define REG_IR3INT           "IR3INT"
#define REG_IR3ADCCLEAR      "IR3ADCCLEAR"
#define REG_IR3ADCEN         "IR3ADCEN"
#define REG_IR3ADCST         "IR3ADCST"
#define REG_IR3UADCMEM       "IR3UADCMEM"
#define REG_IR3VADCMEM       "IR3VADCMEM"
#define REG_IR3WADCMEM       "IR3WADCMEM"
#define REG_IR3ADC           "IR3ADC"
#define REG_IR3COFSALL       "IR3COFSALL"
#define REG_IR3COFSU         "IR3COFSU"
#define REG_IR3COFSV         "IR3COFSV"
#define REG_IR3COFSW         "IR3COFSW"
#define REG_IR3CMPWRMD       "IR3CMPWRMD"
#define REG_IR3CMPRDMD       "IR3CMPRDMD"
#define REG_IR3CMP           "IR3CMP"
#define REG_IR3VALN          "IR3VALN"
//Bits
#define BIT_UVL           "UVL"
#define BIT_VVL           "VVL"
#define BIT_WVL           "WVL"
#define BIT_PCT           "PCT"
#define BIT_CMD           "CMD"
#define BIT_UIC           "UIC"
#define BIT_VIC           "VIC"
#define BIT_WIC           "WIC"
#define BIT_EN            "EN"
//Bit Position
#define BIT_PCT_POS       4
#define BIT_CMD_POS       7
#define BIT_PTN_POS       15
#define BIT_HOE_POS       5
#define BIT_LOE_POS       4
#define BIT_RVS_POS       2
#define BIT_EN_MSK        0x3
#define BIT_MODE_MSK      0x3
#define REG_IR3CMODE_MSK  0x97
// Description: Declaration of HWCORE_IRCTG3_AgentController class
class HWCORE_IRCTG3_AgentController
{
#include "HWCORE_IRCTG3_cmdif.h"
public:
    enum eInterrupt{
        em_ir3_int_err_r = 0,
        em_ir3_int_u_r,
        em_ir3_int_v_r,
        em_ir3_int_w_r,
        em_ir3_calc_u_r,
        em_ir3_calc_v_r,
        em_ir3_calc_w_r,
        em_ir3_adc_u_r,
        em_ir3_adc_v_r,
        em_ir3_adc_w_r,
        // Number of interrupts
        emINTERRUPT_NUM,
    };

    enum eOutput{
        em_ir3_pat_u_r = 0,
        em_ir3_pat_v_r,
        em_ir3_pat_w_r,
        // Number of output ports 
        emOUTPUT_PORTS_NUM,
    };

    enum ePhases {
        emU_PHASE = 0,
        emV_PHASE,
        emW_PHASE,
        //Number of phases
        emNumPhase
    };

    enum eCompMode {
        emCommon = 0,
        emIndependent,
        //Number of comparison mode
        emNumCompMode
    };

    // Constructor/Destructor
    HWCORE_IRCTG3_AgentController(void) { ; }
    virtual ~HWCORE_IRCTG3_AgentController(void) { ; }

    // Interface (APIs)
    virtual bool GetResetStatus(const std::string reset_name) = 0;
    virtual bool CheckClockPeriod(void) = 0;

    // API for processes
    virtual void NotifyStartupTriggerGenerated() = 0;
    virtual void NotifyUpdateOutputPort() = 0;

    // APIs for updating ports + interrupts
    virtual void UpdateInterruptValue(unsigned int InterruptIndex, bool value) = 0;
    virtual void UpdateOutputValue(unsigned int OutputIndex, unsigned int value) = 0;
};
#endif //__HWCORE_IRCTG3_AGENT_CONTROLLER_H__
