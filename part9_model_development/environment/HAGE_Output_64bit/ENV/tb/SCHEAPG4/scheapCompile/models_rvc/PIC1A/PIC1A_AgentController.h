// ---------------------------------------------------------------------
// $Id: PIC1A_AgentController.h,v 1.1 2014/12/15 09:31:44 binhnguyen Exp $
//
// Copyright(c) 2014 Renesas Electronics Corporation
// Copyright(c) 2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __PIC1A_AGENTCONTROLLER_H__
#define __PIC1A_AGENTCONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class PIC1A_AgentController
{
#include "PIC1A_cmdif.h"
public:
    enum ePortNum {     // Define the number of ports
        emPortNumTAUD = 16,
        emPortNumTAUJ = 4,
        emPortNumPIC2 = 5,
        emPortNumANFESO = 4,
        emPortNumHiZ = 6,
        emPortNumTSG = 3
    };
    
    enum eInputIndex {     // Define the index for input ports
        // ENCA0
        emENCAT0IOV,
        emENCAT0IEC,
        emENCAT0INT0,
        emENCAT0EQ0,
        emENCAT0ECS,
        emENCAT0EQ1,
        emENCAT0INT1,
        emENCAT0IUD,
        // ENCA1
        emENCAT1IOV,
        emENCAT1IEC,
        emENCAT1INT0,
        emENCAT1EQ0,
        emENCAT1ECS,
        emENCAT1EQ1,
        emENCAT1INT1,
        // ENCA2
        emENCAT2IOV,
        emENCAT2INT0,
        emENCAT2INT1,
        // ENCA3
        emENCAT3IOV,
        emENCAT3INT0,
        emENCAT3INT1,
        // TAUD0
        emTAUD0CDEN0,       // not affect
        emTAUD0CDEN1,       // not affect
        emTAUD0CDEN2,
        emTAUD0CDEN3,
        emTAUD0CDEN4,
        emTAUD0CDEN5,
        emTAUD0CDEN6,
        emTAUD0CDEN7,
        emTAUD0CDEN8,
        emTAUD0CDEN9,
        emTAUD0CDEN10,
        emTAUD0CDEN11,
        emTAUD0CDEN12,
        emTAUD0CDEN13,
        emTAUD0CDEN14,
        emTAUD0CDEN15,
        emTAUD0TOUT0,       // not affect
        emTAUD0TOUT1,       // not affect
        emTAUD0TOUT2,
        emTAUD0TOUT3,
        emTAUD0TOUT4,
        emTAUD0TOUT5,
        emTAUD0TOUT6,
        emTAUD0TOUT7,
        emTAUD0TOUT8,
        emTAUD0TOUT9,
        emTAUD0TOUT10,
        emTAUD0TOUT11,
        emTAUD0TOUT12,
        emTAUD0TOUT13,
        emTAUD0TOUT14,
        emTAUD0TOUT15,
        emTAUD0UDC0,
        emTAUD0UDC2,
        emTAUD0UDC8,
        emTAUD0INT0,
        emTAUD0INT1,
        emTAUD0INT2,
        emTAUD0INT3,
        emTAUD0INT4,
        emTAUD0INT5,
        emTAUD0INT6,
        emTAUD0INT7,
        emTAUD0INT8,
        emTAUD0INT9,
        emTAUD0INT10,
        emTAUD0INT11,
        emTAUD0INT12,
        emTAUD0INT13,
        emTAUD0INT14,
        emTAUD0INT15,
        emTAUD0CKEN10,
        emTAUD0CKEN12,
        emTAUD0CKEN14,
        // TAUD1
        emTAUD1CDEN0,       // not affect
        emTAUD1CDEN1,       // not affect
        emTAUD1CDEN2,
        emTAUD1CDEN3,
        emTAUD1CDEN4,
        emTAUD1CDEN5,
        emTAUD1CDEN6,
        emTAUD1CDEN7,
        emTAUD1CDEN8,
        emTAUD1CDEN9,
        emTAUD1CDEN10,
        emTAUD1CDEN11,
        emTAUD1CDEN12,
        emTAUD1CDEN13,
        emTAUD1CDEN14,
        emTAUD1CDEN15,
        emTAUD1TOUT0,       // not affect
        emTAUD1TOUT1,       // not affect
        emTAUD1TOUT2,
        emTAUD1TOUT3,
        emTAUD1TOUT4,
        emTAUD1TOUT5,
        emTAUD1TOUT6,
        emTAUD1TOUT7,
        emTAUD1TOUT8,
        emTAUD1TOUT9,
        emTAUD1TOUT10,
        emTAUD1TOUT11,
        emTAUD1TOUT12,
        emTAUD1TOUT13,
        emTAUD1TOUT14,
        emTAUD1TOUT15,
        emTAUD1UDC0,
        emTAUD1UDC2,
        emTAUD1UDC8,
        emTAUD1INT0,
        emTAUD1INT1,
        emTAUD1INT2,
        emTAUD1INT3,
        emTAUD1INT4,
        emTAUD1INT5,
        emTAUD1INT6,
        emTAUD1INT7,
        emTAUD1INT8,
        emTAUD1INT9,
        emTAUD1INT10,
        emTAUD1INT11,
        emTAUD1INT12,
        emTAUD1INT13,
        emTAUD1INT14,
        emTAUD1INT15,
        emTAUD1CKEN10,
        emTAUD1CKEN12,
        emTAUD1CKEN14,
        // TAUJ0
        emTAUJ0TOUT3,
        // TAUJ1
        emTAUJ1TOUT3,
        // TPBA0
        emTPB0INTPAT,
        // TSG0
        emTSG0TSTPTE,
        emTSG0TS2PEC,
        emTSG0TST2PUD,
        emTSG0TO1,
        emTSG0TO2,
        emTSG0TO3,
        emTSG0TO4,
        emTSG0TO5,
        emTSG0TO6,
        emTSG0INT0,
        emTSG0INT2,
        emTSG0INT6,
        emTSG0INT10,
        emTSG0TSTIER,
        // TSG1
        emTSG1TSTPTE,
        emTSG1TS2PEC,
        emTSG1TST2PUD,
        emTSG1TO1,
        emTSG1TO2,
        emTSG1TO3,
        emTSG1TO4,
        emTSG1TO5,
        emTSG1TO6,
        emTSG1INT0,
        emTSG1INT2,
        emTSG1INT6,
        emTSG1INT10,
        emTSG1TSTIER,
        // PIC2
        emADCATTOUT00,
        emADCATTOUT01,
        emADCATTOUT02,
        emADCATTOUT03,
        emADCATTOUT04,
        emADCATTOUT10,
        emADCATTOUT11,
        emADCATTOUT12,
        emADCATTOUT13,
        emADCATTOUT14,
        // DNF
        emDNFTAUD0TIN0,
        emDNFTAUD0TIN1,
        emDNFTAUD0TIN2,
        emDNFTAUD0TIN3,
        emDNFTAUD0TIN4,
        emDNFTAUD0TIN5,
        emDNFTAUD0TIN6,
        emDNFTAUD0TIN7,
        emDNFTAUD0TIN8,
        emDNFTAUD0TIN9,
        emDNFTAUD0TIN10,
        emDNFTAUD0TIN11,
        emDNFTAUD0TIN12,
        emDNFTAUD0TIN13,
        emDNFTAUD0TIN14,
        emDNFTAUD0TIN15,
        emDNFTAUD1TIN0,
        emDNFTAUD1TIN1,
        emDNFTAUD1TIN2,
        emDNFTAUD1TIN3,
        emDNFTAUD1TIN4,
        emDNFTAUD1TIN5,
        emDNFTAUD1TIN6,
        emDNFTAUD1TIN7,
        emDNFTAUD1TIN8,
        emDNFTAUD1TIN9,
        emDNFTAUD1TIN10,
        emDNFTAUD1TIN11,
        emDNFTAUD1TIN12,
        emDNFTAUD1TIN13,
        emDNFTAUD1TIN14,
        emDNFTAUD1TIN15,
        emDNFENCAT0AIN,
        emDNFENCAT0BIN,
        emDNFENCAT0TIN1,
        emDNFENCAT0ZIN,
        emDNFENCAT1AIN,
        emDNFENCAT1BIN,
        emDNFENCAT1TIN1,
        emDNFENCAT1ZIN,
        emDNFTAUJ0TIN0,
        emDNFTAUJ0TIN1,
        emDNFTAUJ0TIN2,
        emDNFTAUJ0TIN3,
        emDNFTSG0TSTCKE,
        emDNFTSG1TSTCKE,
        emDNFSIF0IREF,
        emDNFSIF1IREF,
        emDNFENCAT2TIN1,
        emDNFENCAT3TIN1,
        emANFESO0,
        emANFESO1,
        emANFESO2,
        emANFESO3,
        // ECM
        emERROROUTZ,
        // HiZ Source
        emRSVHIZIN0,
        emRSVHIZIN1,
        emRSVHIZIN2,
        emRSVHIZIN3,
        emRSVHIZIN4,
        emRSVHIZIN5,
        // PORT(TSG0 Hall Sensor)
        emPORTTSG0TSTAPT0,
        emPORTTSG0TSTAPT1,
        emPORTTSG0TSTAPT2,
        // PORT(TSG1 Hall Sensor)
        emPORTTSG1TSTAPT0,
        emPORTTSG1TSTAPT1,
        emPORTTSG1TSTAPT2,
        // RDC0
        emSAT0AOUT,
        emSAT0BOUT,
        emSAT0ZOUT,
        // RDC1
        emSAT1AOUT,
        emSAT1BOUT,
        emSAT1ZOUT,
        emInputNum
    };
    
    enum eOutputIndex {     // Define the index for normal output ports
        // ENCA0
        emENCAT0AIN,
        emENCAT0BIN,
        emENCAT0ZIN,
        emENCAT0TIN1,
        // ENCA1
        emENCAT1AIN,
        emENCAT1BIN,
        emENCAT1ZIN,
        emENCAT1TIN1,
        // ENCA2
        emENCAT2AIN,
        emENCAT2BIN,
        emENCAT2ZIN,
        emENCAT2TIN1,
        // ENCA3
        emENCAT3AIN,
        emENCAT3BIN,
        emENCAT3ZIN,
        emENCAT3TIN1,
        // TAPA0
        emTOP0TAPATHASIN,
        emTOP0TAPATSIM0,
        emTOP0TAPATUDCM0,
        // TAPA1
        emTOP1TAPATHASIN,
        emTOP1TAPATSIM0,
        emTOP1TAPATUDCM0,
        // TAPA2
        emTOP2TAPATHASIN,
        // TAPA3
        emTOP3TAPATHASIN,
        // TAUD0
        emTAUD0TIN0,
        emTAUD0TIN1,
        emTAUD0TIN2,
        emTAUD0TIN3,
        emTAUD0TIN4,
        emTAUD0TIN5,
        emTAUD0TIN6,
        emTAUD0TIN7,
        emTAUD0TIN8,
        emTAUD0TIN9,
        emTAUD0TIN10,
        emTAUD0TIN11,
        emTAUD0TIN12,
        emTAUD0TIN13,
        emTAUD0TIN14,
        emTAUD0TIN15,
        // TAUD1
        emTAUD1TIN0,
        emTAUD1TIN1,
        emTAUD1TIN2,
        emTAUD1TIN3,
        emTAUD1TIN4,
        emTAUD1TIN5,
        emTAUD1TIN6,
        emTAUD1TIN7,
        emTAUD1TIN8,
        emTAUD1TIN9,
        emTAUD1TIN10,
        emTAUD1TIN11,
        emTAUD1TIN12,
        emTAUD1TIN13,
        emTAUD1TIN14,
        emTAUD1TIN15,
        // TAUJ0
        emTAUJ0TIN0,
        emTAUJ0TIN1,
        emTAUJ0TIN2,
        emTAUJ0TIN3,
        // TSG0
        emTSG0TSTOPS,
        emTSG0TSTOPC0,
        emTSG0TSTOPC1,
        emTSG0TSTCKE,
        emTSG0TSTAPT0,
        emTSG0TSTAPT1,
        emTSG0TSTAPT2,
        emTSG0TSTIUS,
        emTSG0TSTIUC,
        emTSG0TSTIVS,
        emTSG0TSTIVC,
        emTSG0TSTIWS,
        emTSG0TSTIWC,
        // TSG1
        emTSG1TSTOPS,
        emTSG1TSTOPC0,
        emTSG1TSTOPC1,
        emTSG1TSTCKE,
        emTSG1TSTAPT0,
        emTSG1TSTAPT1,
        emTSG1TSTAPT2,
        emTSG1TSTIUS,
        emTSG1TSTIUC,
        emTSG1TSTIVS,
        emTSG1TSTIVC,
        emTSG1TSTIWS,
        emTSG1TSTIWC,
        // INTC
        emINTENC0CM0,
        emINTENC1CM0,
        emINTENC2CM0,
        emINTENC3CM0,
        // PORT(TAUD0)
        emTOP0U,
        emTOP0UB,
        emTOP0V,
        emTOP0VB,
        emTOP0W,
        emTOP0WB,
        // PORT(TAUD1)
        emTOP1U,
        emTOP1UB,
        emTOP1V,
        emTOP1VB,
        emTOP1W,
        emTOP1WB,
        // PORT(TSG0)
        emTOP2U,
        emTOP2UB,
        emTOP2V,
        emTOP2VB,
        emTOP2W,
        emTOP2WB,
        // PORT(TSG1)
        emTOP3U,
        emTOP3UB,
        emTOP3V,
        emTOP3VB,
        emTOP3W,
        emTOP3WB,
        emOutputNum,
    };
    
    enum eSyncOutputIndex {     // Define the index for Sync Start output ports
        // ENCAn
        emENCAT0TSST,
        emENCAT1TSST,
        emENCAT2TSST,
        emENCAT3TSST,
        // TAUD0
        emTAUD0SST0,
        emTAUD0SST1,
        emTAUD0SST2,
        emTAUD0SST3,
        emTAUD0SST4,
        emTAUD0SST5,
        emTAUD0SST6,
        emTAUD0SST7,
        emTAUD0SST8,
        emTAUD0SST9,
        emTAUD0SST10,
        emTAUD0SST11,
        emTAUD0SST12,
        emTAUD0SST13,
        emTAUD0SST14,
        emTAUD0SST15,
        // TAUD1
        emTAUD1SST0,
        emTAUD1SST1,
        emTAUD1SST2,
        emTAUD1SST3,
        emTAUD1SST4,
        emTAUD1SST5,
        emTAUD1SST6,
        emTAUD1SST7,
        emTAUD1SST8,
        emTAUD1SST9,
        emTAUD1SST10,
        emTAUD1SST11,
        emTAUD1SST12,
        emTAUD1SST13,
        emTAUD1SST14,
        emTAUD1SST15,
        // TAUJ0
        emTAUJ0TSST0,
        emTAUJ0TSST1,
        emTAUJ0TSST2,
        emTAUJ0TSST3,
        // TAUJ1
        emTAUJ1TSST0,
        emTAUJ1TSST1,
        emTAUJ1TSST2,
        emTAUJ1TSST3,
        // TPBAn
        emTPB0TSST,
        emTPB1TSST,
        // TSGn
        emTSG0TSST,
        emTSG1TSST,
        // OSTMn
        emOST0TSST,
        emOST1TSST,
        emOST2TSST,
        emOST3TSST,
        emSyncOutputNum
    };
    
    enum eDTIndex {         // Define the index of DT sub-module
        emDT00,
        emDT01,
        emDT02,
        emDT10,
        emDT11,
        emDT12,
        emDTnum
    };
    
    enum eRSIndex {         // Define the index of RS sub-module
        emRS00,
        emRS01,
        emRS02,
        emRS03,
        emRS04,
        emRS05,
        emRS06,
        emRS07,
        emRS10,
        emRS11,
        emRS12,
        emRS13,
        emRS14,
        emRS15,
        emRS16,
        emRS17,
        emRSnum
    };
    
    enum eRTOIndex {         // Define the index of RTOnm sub-module
        emRTO0,
        emRTO1,
        emRTO2,
        emRTO3,
        emRTO4,
        emRTO5,
        emRTOnum
    };
    
    enum eRTOChangeType {       // Define the type that trigger RTO process
        emRTOInputChange,
        emRTOTrigger,
        emRTOReset
    };
    
    enum eWNGIndex {         // Define the index of WNGnm sub-module
        emWNG0,
        emWNG1,
        emWNG2,
        emWNGnum
    };
    
    enum eWNGChangeType {       // Define the type that trigger WNG process
        emWNGabInput,
        emWNGfInput,
        emWNGhInput,
        emWNGTrigger,
        emWNGMK,
        emWNGS
    };
    
    enum eWNGOutputType {       // Define the output type of WNGnm
        emWNGdOutput,
        emWNGeOutput,
        emWNGcOutput,
        emWNGgOutput,
        emWNGiOutput
    };
    
    enum eFNIndex {         // Define the index of FNnm sub-module
        emFN0,
        emFN1,
        emFN2,
        emFN3,
        emFN4,
        emFN5,
        emFNnum
    };
    
    enum ePFNIndex {         // Define the index of PFNnm sub-module
        emPFN01,
        emPFN23,
        emPFN45,
        emPFNnum
    };
    
    enum ePFNChangeType {       // Define the type that trigger PFN process
        emPFNTOUT2Input,
        emPFNIn1Input,
        emPFNIn2Input,
        emPFNWrReg
    };
    
    enum eTSG_FFIndex {       // Define the output connection of FF in PORT(TSG)
        emFFU,
        emFFUB,
        emFFV,
        emFFVB,
        emFFW,
        emFFWB,
        emFFnum
    };
    
    enum eMUX_PIC0REG2n0_2425Output {     // Define the output connection of MUX_PIC0REG2n0[24-25]
        emTAPATSIM0,
        emTAPATUDCM0
    };
    
    enum eMUX_PIC0REG2n4Output {     // Define the output connection of MUX_PIC0REG2n4
        emT00,
        emT01,
        emT02,
        emT03,
        emT04,
        emT05,
        emT06,
        emT07,
        emT08,
        emT09,
        emT10,
        emT11,
        emT12,
        emT13,
        emT14,
        emT15,
        emMUX_PIC0REG2n4num
    };
    
    enum eMUX_PIC0REG30Output {     // Define the output connection of MUX_PIC0REG30_1617, MUX_PIC0REG30_1920
        emG1,
        emG2,
        emG3,
        emGnum
    };
    
    enum eMUX_PIC0REG5n_00Output {     // Define the output connection of MUX_PIC0REG5n_00
        emI1,
        emI2,
        emI3,
        emInum
    };
    
    enum eMUX_PIC0REG5n_03Output {     // Define the output connection of MUX_PIC0REG5n_03
        emUS,
        emUC,
        emVS,
        emVC,
        emWS,
        emWC
    };
    
    enum eMUX_PIC0REG5n_12Output {     // Define the output connection of MUX_PIC0REG5n_12
        emI4,
        emI5,
        emI6
    };
    
    PIC1A_AgentController(void) {;}
    virtual ~PIC1A_AgentController(void) {;}
    virtual bool GetResetStatus (void) = 0;
    virtual bool CheckClockPeriod (const std::string clock_name) = 0;
    
    virtual void TriggerSyncStart (const unsigned int index) = 0;
    virtual void UpdateOutputPort (const unsigned int index, const bool value) = 0;
    virtual void TriggerSettingFlag (const unsigned int module, const unsigned int index) = 0;
    virtual void WNG_FFiChange (const unsigned int module, const unsigned int index) = 0;
    virtual void WNG_FFgChange (const unsigned int module, const unsigned int index) = 0;
    virtual void REG513_FFChange (const unsigned int module, const unsigned int index) = 0;
    virtual void REG514_FFChange (const unsigned int module, const unsigned int index) = 0;
    virtual void TSGOUTCTR_FFChange (const unsigned int module, const unsigned int index) = 0;
    virtual void ResetRS (const unsigned int index) = 0;
    virtual void ResetDT (const unsigned int index) = 0;
    virtual void ResetRTO (const unsigned int module, const unsigned int index) = 0;
};
#endif
