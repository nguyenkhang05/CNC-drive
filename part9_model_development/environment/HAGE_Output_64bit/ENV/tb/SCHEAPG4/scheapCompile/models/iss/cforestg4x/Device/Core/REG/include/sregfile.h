/*
 * (c) 2011,2012,2013,2017 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once


#include "./forest_common.h"
#include "./sregfile_base.h"
#include "./sreg.h"


class NcSRegFileIF {
public:
    virtual ~NcSRegFileIF () {};

    // System register group 0
    virtual SRegHMEIPC*      SrHMEIPC() const = 0;
    virtual SRegHMEIPSW*     SrHMEIPSW() const = 0;
    virtual SRegHMFEPC*      SrHMFEPC() const = 0;
    virtual SRegHMFEPSW*     SrHMFEPSW() const = 0;
    virtual SRegHMPSW*       SrHMPSW() const = 0;
    virtual SRegFPSR*        SrFPSR() const = 0;
    virtual SRegFPEPC*       SrFPEPC() const = 0;
    virtual SRegFPST*        SrFPST() const = 0;
    virtual SRegFPCC*        SrFPCC() const = 0;
    virtual SRegFPCFG*       SrFPCFG() const = 0;
    virtual SRegFPEC*        SrFPEC() const = 0;
    virtual SRegHMEIIC*      SrHMEIIC() const = 0;
    virtual SRegHMFEIC*      SrHMFEIC() const = 0;
    virtual SRegPSWH*        SrPSWH() const = 0;
    virtual SRegCTPC*        SrCTPC() const = 0;
    virtual SRegCTPSW*       SrCTPSW() const = 0;
    virtual SRegEIPSWH*      SrEIPSWH() const = 0;
    virtual SRegFEPSWH*      SrFEPSWH() const = 0;
    virtual SRegCTBP*        SrCTBP() const = 0;
    virtual SRegSNZCFG*      SrSNZCFG() const = 0;
    virtual SRegHMEIWR*      SrHMEIWR() const = 0;
    virtual SRegHMFEWR*      SrHMFEWR() const = 0;
    // System register group 1
    virtual SRegHMSPID*      SrHMSPID() const = 0;
    virtual SRegHMSPIDLIST*  SrHMSPIDLIST() const = 0;
    virtual SRegRBASE*       SrRBASE() const = 0;
    virtual SRegHMEBASE*     SrHMEBASE() const = 0;
    virtual SRegHMINTBP*     SrHMINTBP() const = 0;
    virtual SRegMCTL*        SrMCTL() const = 0;
    virtual SRegPID*         SrPID() const = 0;
    virtual SRegFPIPR*       SrFPIPR() const = 0;
    virtual SRegHMSVLOCK*    SrHMSVLOCK() const = 0;
    virtual SRegSCCFG*       SrSCCFG() const = 0;
    virtual SRegSCBP*        SrSCBP() const = 0;
    virtual SRegHVCFG*       SrHVCFG() const = 0;
    virtual SRegGMCFG*       SrGMCFG() const = 0;
    virtual SRegHVSB*        SrHVSB() const = 0;
    // System register group 2
    virtual SRegHMPEID*      SrHMPEID() const = 0;
    virtual SRegBMID*        SrBMID() const = 0;
    virtual SRegHMMEA*       SrHMMEA() const = 0;
    virtual SRegHMMEI*       SrHMMEI() const = 0;
    virtual SRegISPR*        SrISPR() const = 0;
    virtual SRegIMSR*        SrIMSR() const = 0;
    virtual SRegICSR*        SrICSR() const = 0;
    virtual SRegHMINTCFG*    SrHMINTCFG() const = 0;
    virtual SRegHMPLMR*      SrHMPLMR() const = 0;
    virtual SRegRBCR0*       SrRBCR0() const = 0;
    virtual SRegRBCR1*       SrRBCR1() const = 0;
    virtual SRegRBNR*        SrRBNR() const = 0;
    virtual SRegRBIP*        SrRBIP() const = 0;
    // System register group 3
    virtual SRegDBGEN*       SrDBGEN() const = 0;
    virtual SRegDBPSWH*      SrDBPSWH() const = 0;
    virtual SRegDBIC*        SrDBIC() const = 0;
    virtual SRegDBPC*        SrDBPC() const = 0;
    virtual SRegDBPSW*       SrDBPSW() const = 0;
    virtual SRegDIR0*        SrDIR0() const = 0;
    virtual SRegDIR1*        SrDIR1() const = 0;
    virtual SRegBPC*         SrBPC() const = 0;
    virtual SRegBPAV*        SrBPAV() const = 0;
    virtual SRegBPAM*        SrBPAM() const = 0;
    virtual SRegDBCFG*       SrDBCFG() const = 0;
    virtual SRegDBWR*        SrDBWR() const = 0;
    // System register group 4
    virtual SRegICTAGL*      SrICTAGL() const = 0;
    virtual SRegICTAGH*      SrICTAGH() const = 0;
    virtual SRegICDATL*      SrICDATL() const = 0;
    virtual SRegICDATH*      SrICDATH() const = 0;
    virtual SRegICCTRL*      SrICCTRL() const = 0;
    virtual SRegICCFG*       SrICCFG() const = 0;
    virtual SRegICERR*       SrICERR() const = 0;
    // System register group 5
    virtual SRegHMMPM*       SrHMMPM() const = 0;
    virtual SRegMPCFG*       SrMPCFG() const = 0;
    virtual SRegMCA*         SrMCA() const = 0;
    virtual SRegMCS*         SrMCS() const = 0;
    virtual SRegMCC*         SrMCC() const = 0;
    virtual SRegMCR*         SrMCR() const = 0;
    virtual SRegMCI*         SrMCI() const = 0;
    virtual SRegMPIDX*       SrMPIDX() const = 0;
    virtual SRegMPBK*        SrMPBK() const = 0;
    virtual SRegMPLA*        SrMPLA() const = 0;
    virtual SRegMPUA*        SrMPUA() const = 0;
    virtual SRegMPAT*        SrMPAT() const = 0;
    virtual SRegMPID*        SrMPID0() const = 0;
    virtual SRegMPID*        SrMPID1() const = 0;
    virtual SRegMPID*        SrMPID2() const = 0;
    virtual SRegMPID*        SrMPID3() const = 0;
    virtual SRegMPID*        SrMPID4() const = 0;
    virtual SRegMPID*        SrMPID5() const = 0;
    virtual SRegMPID*        SrMPID6() const = 0;
    virtual SRegMPID*        SrMPID7() const = 0;
    // System register group 9
    virtual SRegGMEIPC*      SrGMEIPC() const = 0;
    virtual SRegGMEIPSW*     SrGMEIPSW() const = 0;
    virtual SRegGMFEPC*      SrGMFEPC() const = 0;
    virtual SRegGMFEPSW*     SrGMFEPSW() const = 0;
    virtual SRegGMPSW*       SrGMPSW() const = 0;
    virtual SRegGMMEA*       SrGMMEA() const = 0;
    virtual SRegGMMEI*       SrGMMEI() const = 0;
    virtual SRegGMEIIC*      SrGMEIIC() const = 0;
    virtual SRegGMFEIC*      SrGMFEIC() const = 0;
    virtual SRegGMSPID*      SrGMSPID() const = 0;
    virtual SRegGMSPIDLIST*  SrGMSPIDLIST() const = 0;
    virtual SRegGMEBASE*     SrGMEBASE() const = 0;
    virtual SRegGMINTBP*     SrGMINTBP() const = 0;
    virtual SRegGMINTCFG*    SrGMINTCFG() const = 0;
    virtual SRegGMPLMR*      SrGMPLMR() const = 0;
    virtual SRegGMSVLOCK*    SrGMSVLOCK() const = 0;
    virtual SRegGMMPM*       SrGMMPM() const = 0;
    virtual SRegGMEIWR*      SrGMEIWR() const = 0;
    virtual SRegGMFEWR*      SrGMFEWR() const = 0;
    virtual SRegGMPEID*      SrGMPEID() const = 0;
    // System register group 10
    virtual SRegFXSR*        SrFXSR() const = 0;
    virtual SRegFXST*        SrFXST() const = 0;
    virtual SRegFXINFO*      SrFXINFO() const = 0;
    virtual SRegFXCFG*       SrFXCFG() const = 0;
    virtual SRegFXXC*        SrFXXC() const = 0;
    virtual SRegFXXP*        SrFXXP() const = 0;
    // System register group 11
    virtual SRegTSCOUNTL*    SrTSCOUNTL() const = 0;
    virtual SRegTSCOUNTH*    SrTSCOUNTH() const = 0;
    virtual SRegTSCTRL*      SrTSCTRL() const = 0;
    virtual SRegPMUMCTRL*    SrPMUMCTRL() const = 0;
    virtual SRegPMGMCTRL*    SrPMGMCTRL() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT0_G4X1X() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL0_G4X1X() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT1_G4X1X() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL1_G4X1X() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT2_G4X1X() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL2_G4X1X() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT3_G4X1X() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL3_G4X1X() const = 0;
    // System register group 12
    virtual SRegLSTEST0*     SrLSTEST0() const = 0;
    virtual SRegLSTEST1*     SrLSTEST1() const = 0;
    virtual SRegLSCFG*       SrLSCFG() const = 0;
    virtual SRegICBKEY*      SrICBKEY() const = 0;
    virtual SRegIFCR*        SrIFCR() const = 0;
    virtual SRegIFCR1*       SrIFCR1() const = 0;
    virtual SRegBRPCTRL0*    SrBRPCTRL0() const = 0;
    virtual SRegBRPCTRL1*    SrBRPCTRL1() const = 0;
    virtual SRegBRPCFG*      SrBRPCFG() const = 0;
    virtual SRegBRPACTRL*    SrBRPACTRL() const = 0;
    virtual SRegBRPDATA*     SrBRPDATA() const = 0;
    // System register group 13
    virtual SRegDCBKEY*      SrDCBKEY() const = 0;
    virtual SRegLSUCR*       SrLSUCR() const = 0;
    virtual SRegLSULNK0*     SrLSULNK0() const = 0;
    virtual SRegLSULNK1*     SrLSULNK1() const = 0;
    virtual SRegL1RLCR*      SrL1RLCR() const = 0;
    virtual SRegL1RLNK0*     SrL1RLNK0() const = 0;
    virtual SRegL1RLNK1*     SrL1RLNK1() const = 0;
    virtual SRegL1RCFG*      SrL1RCFG() const = 0;
    virtual SRegDECFG*       SrDECFG() const = 0;
    virtual SRegDECTRL*      SrDECTRL() const = 0;
    virtual SRegDEVDS*       SrDEVDS() const = 0;
    virtual SRegRDBCR*       SrRDBCR() const = 0;
    virtual SRegRDBACR*      SrRDBACR() const = 0;
    virtual SRegRDBATAG*     SrRDBATAG() const = 0;
    virtual SRegRDBADAT0*    SrRDBADAT0() const = 0;
    virtual SRegRDBADAT1*    SrRDBADAT1() const = 0;
    virtual SRegRDBADAT2*    SrRDBADAT2() const = 0;
    virtual SRegRDBADAT3*    SrRDBADAT3() const = 0;
    virtual SRegRDBSTAT*     SrRDBSTAT() const = 0;
    // System register group 14
    virtual SRegPMCTRL*      SrPMCTRL0_G4MH20() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL1_G4MH20() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL2_G4MH20() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL3_G4MH20() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL4_G4MH20() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL5_G4MH20() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL6_G4MH20() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL7_G4MH20() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT0_G4MH20() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT1_G4MH20() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT2_G4MH20() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT3_G4MH20() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT4_G4MH20() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT5_G4MH20() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT6_G4MH20() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT7_G4MH20() const = 0;
    // System register group 15
    virtual SRegPMSUBCND*    SrPMSUBCND0() const = 0;
    virtual SRegPMSUBCND*    SrPMSUBCND1() const = 0;
    virtual SRegPMSUBCND*    SrPMSUBCND2() const = 0;
    virtual SRegPMSUBCND*    SrPMSUBCND3() const = 0;
    virtual SRegPMSUBCND*    SrPMSUBCND4() const = 0;
    virtual SRegPMSUBCND*    SrPMSUBCND5() const = 0;
    virtual SRegPMSUBCND*    SrPMSUBCND6() const = 0;
    virtual SRegPMSUBCND*    SrPMSUBCND7() const = 0;
    // System register group 32
    virtual SRegBPC_N*       SrBPC0() const = 0;
    virtual SRegBPC_N*       SrBPC1() const = 0;
    virtual SRegBPC_N*       SrBPC2() const = 0;
    virtual SRegBPC_N*       SrBPC3() const = 0;
    virtual SRegBPC_N*       SrBPC4() const = 0;
    virtual SRegBPC_N*       SrBPC5() const = 0;
    virtual SRegBPC_N*       SrBPC6() const = 0;
    virtual SRegBPC_N*       SrBPC7() const = 0;
    virtual SRegBPC_N*       SrBPC8() const = 0;
    virtual SRegBPC_N*       SrBPC9() const = 0;
    virtual SRegBPC_N*       SrBPC10() const = 0;
    virtual SRegBPC_N*       SrBPC11() const = 0;
    // System register group 33
    virtual SRegBPAV_N*      SrBPAV0() const = 0;
    virtual SRegBPAV_N*      SrBPAV1() const = 0;
    virtual SRegBPAV_N*      SrBPAV2() const = 0;
    virtual SRegBPAV_N*      SrBPAV3() const = 0;
    virtual SRegBPAV_N*      SrBPAV4() const = 0;
    virtual SRegBPAV_N*      SrBPAV5() const = 0;
    virtual SRegBPAV_N*      SrBPAV6() const = 0;
    virtual SRegBPAV_N*      SrBPAV7() const = 0;
    virtual SRegBPAV_N*      SrBPAV8() const = 0;
    virtual SRegBPAV_N*      SrBPAV9() const = 0;
    virtual SRegBPAV_N*      SrBPAV10() const = 0;
    virtual SRegBPAV_N*      SrBPAV11() const = 0;
    // System register group 34
    virtual SRegBPAM_N*      SrBPAM0() const = 0;
    virtual SRegBPAM_N*      SrBPAM1() const = 0;
    virtual SRegBPAM_N*      SrBPAM2() const = 0;
    virtual SRegBPAM_N*      SrBPAM3() const = 0;
    virtual SRegBPAM_N*      SrBPAM4() const = 0;
    virtual SRegBPAM_N*      SrBPAM5() const = 0;
    virtual SRegBPAM_N*      SrBPAM6() const = 0;
    virtual SRegBPAM_N*      SrBPAM7() const = 0;
    virtual SRegBPAM_N*      SrBPAM8() const = 0;
    virtual SRegBPAM_N*      SrBPAM9() const = 0;
    virtual SRegBPAM_N*      SrBPAM10() const = 0;
    virtual SRegBPAM_N*      SrBPAM11() const = 0;
    // System register group 37
    virtual SRegMPLA_N*      SrMPLA0() const = 0;
    virtual SRegMPLA_N*      SrMPLA1() const = 0;
    virtual SRegMPLA_N*      SrMPLA2() const = 0;
    virtual SRegMPLA_N*      SrMPLA3() const = 0;
    virtual SRegMPLA_N*      SrMPLA4() const = 0;
    virtual SRegMPLA_N*      SrMPLA5() const = 0;
    virtual SRegMPLA_N*      SrMPLA6() const = 0;
    virtual SRegMPLA_N*      SrMPLA7() const = 0;
    virtual SRegMPLA_N*      SrMPLA8() const = 0;
    virtual SRegMPLA_N*      SrMPLA9() const = 0;
    virtual SRegMPLA_N*      SrMPLA10() const = 0;
    virtual SRegMPLA_N*      SrMPLA11() const = 0;
    virtual SRegMPLA_N*      SrMPLA12() const = 0;
    virtual SRegMPLA_N*      SrMPLA13() const = 0;
    virtual SRegMPLA_N*      SrMPLA14() const = 0;
    virtual SRegMPLA_N*      SrMPLA15() const = 0;
    virtual SRegMPLA_N*      SrMPLA16() const = 0;
    virtual SRegMPLA_N*      SrMPLA17() const = 0;
    virtual SRegMPLA_N*      SrMPLA18() const = 0;
    virtual SRegMPLA_N*      SrMPLA19() const = 0;
    virtual SRegMPLA_N*      SrMPLA20() const = 0;
    virtual SRegMPLA_N*      SrMPLA21() const = 0;
    virtual SRegMPLA_N*      SrMPLA22() const = 0;
    virtual SRegMPLA_N*      SrMPLA23() const = 0;
    virtual SRegMPLA_N*      SrMPLA24() const = 0;
    virtual SRegMPLA_N*      SrMPLA25() const = 0;
    virtual SRegMPLA_N*      SrMPLA26() const = 0;
    virtual SRegMPLA_N*      SrMPLA27() const = 0;
    virtual SRegMPLA_N*      SrMPLA28() const = 0;
    virtual SRegMPLA_N*      SrMPLA29() const = 0;
    virtual SRegMPLA_N*      SrMPLA30() const = 0;
    virtual SRegMPLA_N*      SrMPLA31() const = 0;
    // System register group 38
    virtual SRegMPUA_N*      SrMPUA0() const = 0;
    virtual SRegMPUA_N*      SrMPUA1() const = 0;
    virtual SRegMPUA_N*      SrMPUA2() const = 0;
    virtual SRegMPUA_N*      SrMPUA3() const = 0;
    virtual SRegMPUA_N*      SrMPUA4() const = 0;
    virtual SRegMPUA_N*      SrMPUA5() const = 0;
    virtual SRegMPUA_N*      SrMPUA6() const = 0;
    virtual SRegMPUA_N*      SrMPUA7() const = 0;
    virtual SRegMPUA_N*      SrMPUA8() const = 0;
    virtual SRegMPUA_N*      SrMPUA9() const = 0;
    virtual SRegMPUA_N*      SrMPUA10() const = 0;
    virtual SRegMPUA_N*      SrMPUA11() const = 0;
    virtual SRegMPUA_N*      SrMPUA12() const = 0;
    virtual SRegMPUA_N*      SrMPUA13() const = 0;
    virtual SRegMPUA_N*      SrMPUA14() const = 0;
    virtual SRegMPUA_N*      SrMPUA15() const = 0;
    virtual SRegMPUA_N*      SrMPUA16() const = 0;
    virtual SRegMPUA_N*      SrMPUA17() const = 0;
    virtual SRegMPUA_N*      SrMPUA18() const = 0;
    virtual SRegMPUA_N*      SrMPUA19() const = 0;
    virtual SRegMPUA_N*      SrMPUA20() const = 0;
    virtual SRegMPUA_N*      SrMPUA21() const = 0;
    virtual SRegMPUA_N*      SrMPUA22() const = 0;
    virtual SRegMPUA_N*      SrMPUA23() const = 0;
    virtual SRegMPUA_N*      SrMPUA24() const = 0;
    virtual SRegMPUA_N*      SrMPUA25() const = 0;
    virtual SRegMPUA_N*      SrMPUA26() const = 0;
    virtual SRegMPUA_N*      SrMPUA27() const = 0;
    virtual SRegMPUA_N*      SrMPUA28() const = 0;
    virtual SRegMPUA_N*      SrMPUA29() const = 0;
    virtual SRegMPUA_N*      SrMPUA30() const = 0;
    virtual SRegMPUA_N*      SrMPUA31() const = 0;
    // System register group 39
    virtual SRegMPAT_N*      SrMPAT0() const = 0;
    virtual SRegMPAT_N*      SrMPAT1() const = 0;
    virtual SRegMPAT_N*      SrMPAT2() const = 0;
    virtual SRegMPAT_N*      SrMPAT3() const = 0;
    virtual SRegMPAT_N*      SrMPAT4() const = 0;
    virtual SRegMPAT_N*      SrMPAT5() const = 0;
    virtual SRegMPAT_N*      SrMPAT6() const = 0;
    virtual SRegMPAT_N*      SrMPAT7() const = 0;
    virtual SRegMPAT_N*      SrMPAT8() const = 0;
    virtual SRegMPAT_N*      SrMPAT9() const = 0;
    virtual SRegMPAT_N*      SrMPAT10() const = 0;
    virtual SRegMPAT_N*      SrMPAT11() const = 0;
    virtual SRegMPAT_N*      SrMPAT12() const = 0;
    virtual SRegMPAT_N*      SrMPAT13() const = 0;
    virtual SRegMPAT_N*      SrMPAT14() const = 0;
    virtual SRegMPAT_N*      SrMPAT15() const = 0;
    virtual SRegMPAT_N*      SrMPAT16() const = 0;
    virtual SRegMPAT_N*      SrMPAT17() const = 0;
    virtual SRegMPAT_N*      SrMPAT18() const = 0;
    virtual SRegMPAT_N*      SrMPAT19() const = 0;
    virtual SRegMPAT_N*      SrMPAT20() const = 0;
    virtual SRegMPAT_N*      SrMPAT21() const = 0;
    virtual SRegMPAT_N*      SrMPAT22() const = 0;
    virtual SRegMPAT_N*      SrMPAT23() const = 0;
    virtual SRegMPAT_N*      SrMPAT24() const = 0;
    virtual SRegMPAT_N*      SrMPAT25() const = 0;
    virtual SRegMPAT_N*      SrMPAT26() const = 0;
    virtual SRegMPAT_N*      SrMPAT27() const = 0;
    virtual SRegMPAT_N*      SrMPAT28() const = 0;
    virtual SRegMPAT_N*      SrMPAT29() const = 0;
    virtual SRegMPAT_N*      SrMPAT30() const = 0;
    virtual SRegMPAT_N*      SrMPAT31() const = 0;
    // System register group 40
    virtual SRegMPLA_N*      SrMPLA0_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA1_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA2_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA3_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA4_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA5_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA6_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA7_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA8_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA9_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA10_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA11_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA12_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA13_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA14_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA15_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA16_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA17_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA18_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA19_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA20_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA21_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA22_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA23_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA24_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA25_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA26_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA27_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA28_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA29_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA30_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA31_B1() const = 0;
    // System register group 41
    virtual SRegMPUA_N*      SrMPUA0_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA1_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA2_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA3_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA4_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA5_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA6_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA7_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA8_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA9_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA10_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA11_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA12_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA13_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA14_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA15_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA16_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA17_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA18_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA19_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA20_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA21_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA22_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA23_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA24_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA25_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA26_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA27_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA28_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA29_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA30_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA31_B1() const = 0;
    // System register group 42
    virtual SRegMPAT_N*      SrMPAT0_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT1_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT2_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT3_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT4_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT5_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT6_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT7_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT8_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT9_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT10_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT11_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT12_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT13_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT14_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT15_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT16_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT17_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT18_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT19_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT20_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT21_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT22_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT23_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT24_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT25_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT26_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT27_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT28_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT29_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT30_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT31_B1() const = 0;
};



class NcSRegFile : public SRegFileBase, NcSRegFileIF {
public:
    NcSRegFile(ContextId ctxt_id, ProcElementBase* pe, SRegBase* sreg_init);
    ~NcSRegFile();

    // System register group 0
    SRegHMEIPC*  SrHMEIPC  () const { return static_cast<SRegHMEIPC*>  (m_srf[CF_SR_HMEIPC]);}
    SRegHMEIPSW* SrHMEIPSW () const { return static_cast<SRegHMEIPSW*> (m_srf[CF_SR_HMEIPSW]);}
    SRegHMFEPC*  SrHMFEPC  () const { return static_cast<SRegHMFEPC*>  (m_srf[CF_SR_HMFEPC]);}
    SRegHMFEPSW* SrHMFEPSW () const { return static_cast<SRegHMFEPSW*> (m_srf[CF_SR_HMFEPSW]);}
    SRegHMPSW*   SrHMPSW   () const { return static_cast<SRegHMPSW*>   (m_srf[CF_SR_HMPSW]); }
    SRegFPSR*    SrFPSR    () const { return static_cast<SRegFPSR*>    (m_srf[CF_SR_FPSR]);  }
    SRegFPEPC*   SrFPEPC   () const { return static_cast<SRegFPEPC*>   (m_srf[CF_SR_FPEPC]); }
    SRegFPST*    SrFPST    () const { return static_cast<SRegFPST*>    (m_srf[CF_SR_FPST]);  }
    SRegFPCC*    SrFPCC    () const { return static_cast<SRegFPCC*>    (m_srf[CF_SR_FPCC]);  }
    SRegFPCFG*   SrFPCFG   () const { return static_cast<SRegFPCFG*>   (m_srf[CF_SR_FPCFG]); }
    SRegFPEC*    SrFPEC    () const { return static_cast<SRegFPEC*>    (m_srf[CF_SR_FPEC]);  }
    SRegHMEIIC*  SrHMEIIC  () const { return static_cast<SRegHMEIIC*>  (m_srf[CF_SR_HMEIIC]);}
    SRegHMFEIC*  SrHMFEIC  () const { return static_cast<SRegHMFEIC*>  (m_srf[CF_SR_HMFEIC]);}
    SRegPSWH*    SrPSWH    () const { return static_cast<SRegPSWH*>    (m_srf[CF_SR_PSWH]);  }
    SRegCTPC*    SrCTPC    () const { return static_cast<SRegCTPC*>    (m_srf[CF_SR_CTPC]);  }
    SRegCTPSW*   SrCTPSW   () const { return static_cast<SRegCTPSW*>   (m_srf[CF_SR_CTPSW]); }
    SRegEIPSWH*  SrEIPSWH  () const { return static_cast<SRegEIPSWH*>  (m_srf[CF_SR_EIPSWH]);}
    SRegFEPSWH*  SrFEPSWH  () const { return static_cast<SRegFEPSWH*>  (m_srf[CF_SR_FEPSWH]);}
    SRegCTBP*    SrCTBP    () const { return static_cast<SRegCTBP*>    (m_srf[CF_SR_CTBP]);  }
    SRegSNZCFG*  SrSNZCFG  () const { return static_cast<SRegSNZCFG*>  (m_srf[CF_SR_SNZCFG]);}
    SRegHMEIWR*  SrHMEIWR  () const { return static_cast<SRegHMEIWR*>  (m_srf[CF_SR_HMEIWR]);}
    SRegHMFEWR*  SrHMFEWR  () const { return static_cast<SRegHMFEWR*>  (m_srf[CF_SR_HMFEWR]);}
    // System register group 1
    SRegHMSPID*  SrHMSPID  () const { return static_cast<SRegHMSPID*>  (m_srf[CF_SR_HMSPID]);}
    SRegHMSPIDLIST* SrHMSPIDLIST() const { return static_cast<SRegHMSPIDLIST*>(m_srf[CF_SR_HMSPIDLIST]);}
    SRegRBASE*   SrRBASE   () const { return static_cast<SRegRBASE*>   (m_srf[CF_SR_RBASE]); }
    SRegHMEBASE* SrHMEBASE () const { return static_cast<SRegHMEBASE*> (m_srf[CF_SR_HMEBASE]);}
    SRegHMINTBP* SrHMINTBP () const { return static_cast<SRegHMINTBP*> (m_srf[CF_SR_HMINTBP]);}
    SRegMCTL*    SrMCTL    () const { return static_cast<SRegMCTL*>    (m_srf[CF_SR_MCTL]);  }
    SRegPID*     SrPID     () const { return static_cast<SRegPID*>     (m_srf[CF_SR_PID]);   }
    SRegFPIPR*   SrFPIPR   () const { return static_cast<SRegFPIPR*>   (m_srf[CF_SR_FPIPR]); }
    SRegHMSVLOCK* SrHMSVLOCK() const { return static_cast<SRegHMSVLOCK*>(m_srf[CF_SR_HMSVLOCK]);}
    SRegSCCFG*   SrSCCFG   () const { return static_cast<SRegSCCFG*>   (m_srf[CF_SR_SCCFG]); }
    SRegSCBP*    SrSCBP    () const { return static_cast<SRegSCBP*>    (m_srf[CF_SR_SCBP]);  }
    SRegHVCFG*   SrHVCFG   () const { return static_cast<SRegHVCFG*>   (m_srf[CF_SR_HVCFG]); }
    SRegGMCFG*   SrGMCFG   () const { return static_cast<SRegGMCFG*>   (m_srf[CF_SR_GMCFG]); }
    SRegHVSB*    SrHVSB    () const { return static_cast<SRegHVSB*>    (m_srf[CF_SR_HVSB]);  }
    // System register group 2
    SRegHMPEID*  SrHMPEID  () const { return static_cast<SRegHMPEID*>  (m_srf[CF_SR_HMPEID]);}
    SRegBMID*    SrBMID    () const { return static_cast<SRegBMID*>    (m_srf[CF_SR_BMID]);  }
    SRegHMMEA*   SrHMMEA   () const { return static_cast<SRegHMMEA*>   (m_srf[CF_SR_HMMEA]); }
    SRegHMMEI*   SrHMMEI   () const { return static_cast<SRegHMMEI*>   (m_srf[CF_SR_HMMEI]); }
    SRegISPR*    SrISPR    () const { return static_cast<SRegISPR*>    (m_srf[CF_SR_ISPR]);  }
    SRegIMSR*    SrIMSR    () const { return static_cast<SRegIMSR*>    (m_srf[CF_SR_IMSR]);  }
    SRegICSR*    SrICSR    () const { return static_cast<SRegICSR*>    (m_srf[CF_SR_ICSR]);  }
    SRegHMINTCFG* SrHMINTCFG() const { return static_cast<SRegHMINTCFG*>(m_srf[CF_SR_HMINTCFG]);}
    SRegHMPLMR*  SrHMPLMR  () const { return static_cast<SRegHMPLMR*>  (m_srf[CF_SR_HMPLMR]);}
    SRegRBCR0*   SrRBCR0   () const { return static_cast<SRegRBCR0*>   (m_srf[CF_SR_RBCR0]); }
    SRegRBCR1*   SrRBCR1   () const { return static_cast<SRegRBCR1*>   (m_srf[CF_SR_RBCR1]); }
    SRegRBNR*    SrRBNR    () const { return static_cast<SRegRBNR*>    (m_srf[CF_SR_RBNR]);  }
    SRegRBIP*    SrRBIP    () const { return static_cast<SRegRBIP*>    (m_srf[CF_SR_RBIP]);  }
    // System register group 3
    SRegDBGEN*   SrDBGEN   () const { return static_cast<SRegDBGEN*>   (m_srf[CF_SR_DBGEN]); }
    SRegDBPSWH*  SrDBPSWH  () const { return static_cast<SRegDBPSWH*>  (m_srf[CF_SR_DBPSWH]);}
    SRegDBIC*    SrDBIC    () const { return static_cast<SRegDBIC*>    (m_srf[CF_SR_DBIC]);  }
    SRegDBPC*    SrDBPC    () const { return static_cast<SRegDBPC*>    (m_srf[CF_SR_DBPC]);  }
    SRegDBPSW*   SrDBPSW   () const { return static_cast<SRegDBPSW*>   (m_srf[CF_SR_DBPSW]); }
    SRegDIR0*    SrDIR0    () const { return static_cast<SRegDIR0*>    (m_srf[CF_SR_DIR0]);  }
    SRegDIR1*    SrDIR1    () const { return static_cast<SRegDIR1*>    (m_srf[CF_SR_DIR1]);  }
    SRegBPC*     SrBPC     () const { return static_cast<SRegBPC*>     (m_srf[CF_SR_BPC]);   }
    SRegBPAV*    SrBPAV    () const { return static_cast<SRegBPAV*>    (m_srf[CF_SR_BPAV]);  }
    SRegBPAM*    SrBPAM    () const { return static_cast<SRegBPAM*>    (m_srf[CF_SR_BPAM]);  }
    SRegDBCFG*   SrDBCFG   () const { return static_cast<SRegDBCFG*>   (m_srf[CF_SR_DBCFG]); }
    SRegDBWR*    SrDBWR    () const { return static_cast<SRegDBWR*>    (m_srf[CF_SR_DBWR]);  }
    // System register group 4
    SRegICTAGL*  SrICTAGL  () const { return static_cast<SRegICTAGL*>  (m_srf[CF_SR_ICTAGL]);}
    SRegICTAGH*  SrICTAGH  () const { return static_cast<SRegICTAGH*>  (m_srf[CF_SR_ICTAGH]);}
    SRegICDATL*  SrICDATL  () const { return static_cast<SRegICDATL*>  (m_srf[CF_SR_ICDATL]);}
    SRegICDATH*  SrICDATH  () const { return static_cast<SRegICDATH*>  (m_srf[CF_SR_ICDATH]);}
    SRegICCTRL*  SrICCTRL  () const { return static_cast<SRegICCTRL*>  (m_srf[CF_SR_ICCTRL]);}
    SRegICCFG*   SrICCFG   () const { return static_cast<SRegICCFG*>   (m_srf[CF_SR_ICCFG]); }
    SRegICERR*   SrICERR   () const { return static_cast<SRegICERR*>   (m_srf[CF_SR_ICERR]); }
    // System register group 5
    SRegHMMPM*   SrHMMPM   () const { return static_cast<SRegHMMPM*>   (m_srf[CF_SR_HMMPM]); }
    SRegMPCFG*   SrMPCFG   () const { return static_cast<SRegMPCFG*>   (m_srf[CF_SR_MPCFG]); }
    SRegMCA*     SrMCA     () const { return static_cast<SRegMCA*>     (m_srf[CF_SR_MCA]);   }
    SRegMCS*     SrMCS     () const { return static_cast<SRegMCS*>     (m_srf[CF_SR_MCS]);   }
    SRegMCC*     SrMCC     () const { return static_cast<SRegMCC*>     (m_srf[CF_SR_MCC]);   }
    SRegMCR*     SrMCR     () const { return static_cast<SRegMCR*>     (m_srf[CF_SR_MCR]);   }
    SRegMCI*     SrMCI     () const { return static_cast<SRegMCI*>     (m_srf[CF_SR_MCI]);   }
    SRegMPIDX*   SrMPIDX   () const { return static_cast<SRegMPIDX*>   (m_srf[CF_SR_MPIDX]); }
    SRegMPBK*    SrMPBK    () const { return static_cast<SRegMPBK*>    (m_srf[CF_SR_MPBK]);  }
    SRegMPLA*    SrMPLA    () const { return static_cast<SRegMPLA*>    (m_srf[CF_SR_MPLA]);  }
    SRegMPUA*    SrMPUA    () const { return static_cast<SRegMPUA*>    (m_srf[CF_SR_MPUA]);  }
    SRegMPAT*    SrMPAT    () const { return static_cast<SRegMPAT*>    (m_srf[CF_SR_MPAT]);  }
    SRegMPID*    SrMPID0   () const { return static_cast<SRegMPID*>    (m_srf[CF_SR_MPID0]); }
    SRegMPID*    SrMPID1   () const { return static_cast<SRegMPID*>    (m_srf[CF_SR_MPID1]); }
    SRegMPID*    SrMPID2   () const { return static_cast<SRegMPID*>    (m_srf[CF_SR_MPID2]); }
    SRegMPID*    SrMPID3   () const { return static_cast<SRegMPID*>    (m_srf[CF_SR_MPID3]); }
    SRegMPID*    SrMPID4   () const { return static_cast<SRegMPID*>    (m_srf[CF_SR_MPID4]); }
    SRegMPID*    SrMPID5   () const { return static_cast<SRegMPID*>    (m_srf[CF_SR_MPID5]); }
    SRegMPID*    SrMPID6   () const { return static_cast<SRegMPID*>    (m_srf[CF_SR_MPID6]); }
    SRegMPID*    SrMPID7   () const { return static_cast<SRegMPID*>    (m_srf[CF_SR_MPID7]); }
    // System register group 9
    SRegGMEIPC*  SrGMEIPC  () const { return static_cast<SRegGMEIPC*>  (m_srf[CF_SR_GMEIPC]);}
    SRegGMEIPSW* SrGMEIPSW () const { return static_cast<SRegGMEIPSW*> (m_srf[CF_SR_GMEIPSW]);}
    SRegGMFEPC*  SrGMFEPC  () const { return static_cast<SRegGMFEPC*>  (m_srf[CF_SR_GMFEPC]);}
    SRegGMFEPSW* SrGMFEPSW () const { return static_cast<SRegGMFEPSW*> (m_srf[CF_SR_GMFEPSW]);}
    SRegGMPSW*   SrGMPSW   () const { return static_cast<SRegGMPSW*>   (m_srf[CF_SR_GMPSW]); }
    SRegGMMEA*   SrGMMEA   () const { return static_cast<SRegGMMEA*>   (m_srf[CF_SR_GMMEA]); }
    SRegGMMEI*   SrGMMEI   () const { return static_cast<SRegGMMEI*>   (m_srf[CF_SR_GMMEI]); }
    SRegGMEIIC*  SrGMEIIC  () const { return static_cast<SRegGMEIIC*>  (m_srf[CF_SR_GMEIIC]);}
    SRegGMFEIC*  SrGMFEIC  () const { return static_cast<SRegGMFEIC*>  (m_srf[CF_SR_GMFEIC]);}
    SRegGMSPID*  SrGMSPID  () const { return static_cast<SRegGMSPID*>  (m_srf[CF_SR_GMSPID]);}
    SRegGMSPIDLIST* SrGMSPIDLIST() const { return static_cast<SRegGMSPIDLIST*>(m_srf[CF_SR_GMSPIDLIST]);}
    SRegGMEBASE* SrGMEBASE () const { return static_cast<SRegGMEBASE*> (m_srf[CF_SR_GMEBASE]);}
    SRegGMINTBP* SrGMINTBP () const { return static_cast<SRegGMINTBP*> (m_srf[CF_SR_GMINTBP]);}
    SRegGMINTCFG* SrGMINTCFG() const { return static_cast<SRegGMINTCFG*>(m_srf[CF_SR_GMINTCFG]);}
    SRegGMPLMR*  SrGMPLMR  () const { return static_cast<SRegGMPLMR*>  (m_srf[CF_SR_GMPLMR]);}
    SRegGMSVLOCK* SrGMSVLOCK() const { return static_cast<SRegGMSVLOCK*>(m_srf[CF_SR_GMSVLOCK]);}
    SRegGMMPM*   SrGMMPM   () const { return static_cast<SRegGMMPM*>   (m_srf[CF_SR_GMMPM]); }
    SRegGMEIWR*  SrGMEIWR  () const { return static_cast<SRegGMEIWR*>  (m_srf[CF_SR_GMEIWR]);}
    SRegGMFEWR*  SrGMFEWR  () const { return static_cast<SRegGMFEWR*>  (m_srf[CF_SR_GMFEWR]);}
    SRegGMPEID*  SrGMPEID  () const { return static_cast<SRegGMPEID*>  (m_srf[CF_SR_GMPEID]);}
    // System register group 10
    SRegFXSR*    SrFXSR    () const { return static_cast<SRegFXSR*>    (m_srf[CF_SR_FXSR]);  }
    SRegFXST*    SrFXST    () const { return static_cast<SRegFXST*>    (m_srf[CF_SR_FXST]);  }
    SRegFXINFO*  SrFXINFO  () const { return static_cast<SRegFXINFO*>  (m_srf[CF_SR_FXINFO]);}
    SRegFXCFG*   SrFXCFG   () const { return static_cast<SRegFXCFG*>   (m_srf[CF_SR_FXCFG]); }
    SRegFXXC*    SrFXXC    () const { return static_cast<SRegFXXC*>    (m_srf[CF_SR_FXXC]);  }
    SRegFXXP*    SrFXXP    () const { return static_cast<SRegFXXP*>    (m_srf[CF_SR_FXXP]);  }
    // System register group 11
    SRegTSCOUNTL* SrTSCOUNTL() const { return static_cast<SRegTSCOUNTL*>(m_srf[CF_SR_TSCOUNTL]);}
    SRegTSCOUNTH* SrTSCOUNTH() const { return static_cast<SRegTSCOUNTH*>(m_srf[CF_SR_TSCOUNTH]);}
    SRegTSCTRL*  SrTSCTRL  () const { return static_cast<SRegTSCTRL*>  (m_srf[CF_SR_TSCTRL]);}
    SRegPMUMCTRL* SrPMUMCTRL() const { return static_cast<SRegPMUMCTRL*>(m_srf[CF_SR_PMUMCTRL]);}
    SRegPMGMCTRL* SrPMGMCTRL() const { return static_cast<SRegPMGMCTRL*>(m_srf[CF_SR_PMGMCTRL]);}
    SRegPMCOUNT* SrPMCOUNT0_G4X1X() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT0_G4X1X]);}
    SRegPMCTRL*  SrPMCTRL0_G4X1X() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL0_G4X1X]);}
    SRegPMCOUNT* SrPMCOUNT1_G4X1X() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT1_G4X1X]);}
    SRegPMCTRL*  SrPMCTRL1_G4X1X() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL1_G4X1X]);}
    SRegPMCOUNT* SrPMCOUNT2_G4X1X() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT2_G4X1X]);}
    SRegPMCTRL*  SrPMCTRL2_G4X1X() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL2_G4X1X]);}
    SRegPMCOUNT* SrPMCOUNT3_G4X1X() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT3_G4X1X]);}
    SRegPMCTRL*  SrPMCTRL3_G4X1X() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL3_G4X1X]);}
    // System register group 12
    SRegLSTEST0* SrLSTEST0 () const { return static_cast<SRegLSTEST0*> (m_srf[CF_SR_LSTEST0]);}
    SRegLSTEST1* SrLSTEST1 () const { return static_cast<SRegLSTEST1*> (m_srf[CF_SR_LSTEST1]);}
    SRegLSCFG*   SrLSCFG   () const { return static_cast<SRegLSCFG*>   (m_srf[CF_SR_LSCFG]); }
    SRegICBKEY*  SrICBKEY  () const { return static_cast<SRegICBKEY*>  (m_srf[CF_SR_ICBKEY]);}
    SRegIFCR*    SrIFCR    () const { return static_cast<SRegIFCR*>    (m_srf[CF_SR_IFCR]);  }
    SRegIFCR1*   SrIFCR1   () const { return static_cast<SRegIFCR1*>   (m_srf[CF_SR_IFCR1]); }
    SRegBRPCTRL0* SrBRPCTRL0() const { return static_cast<SRegBRPCTRL0*>(m_srf[CF_SR_BRPCTRL0]);}
    SRegBRPCTRL1* SrBRPCTRL1() const { return static_cast<SRegBRPCTRL1*>(m_srf[CF_SR_BRPCTRL1]);}
    SRegBRPCFG*  SrBRPCFG  () const { return static_cast<SRegBRPCFG*>  (m_srf[CF_SR_BRPCFG]);}
    SRegBRPACTRL* SrBRPACTRL() const { return static_cast<SRegBRPACTRL*>(m_srf[CF_SR_BRPACTRL]);}
    SRegBRPDATA* SrBRPDATA () const { return static_cast<SRegBRPDATA*> (m_srf[CF_SR_BRPDATA]);}
    // System register group 13
    SRegDCBKEY*  SrDCBKEY  () const { return static_cast<SRegDCBKEY*>  (m_srf[CF_SR_DCBKEY]);}
    SRegLSUCR*   SrLSUCR   () const { return static_cast<SRegLSUCR*>   (m_srf[CF_SR_LSUCR]); }
    SRegLSULNK0* SrLSULNK0 () const { return static_cast<SRegLSULNK0*> (m_srf[CF_SR_LSULNK0]);}
    SRegLSULNK1* SrLSULNK1 () const { return static_cast<SRegLSULNK1*> (m_srf[CF_SR_LSULNK1]);}
    SRegL1RLCR*  SrL1RLCR  () const { return static_cast<SRegL1RLCR*>  (m_srf[CF_SR_L1RLCR]);}
    SRegL1RLNK0* SrL1RLNK0 () const { return static_cast<SRegL1RLNK0*> (m_srf[CF_SR_L1RLNK0]);}
    SRegL1RLNK1* SrL1RLNK1 () const { return static_cast<SRegL1RLNK1*> (m_srf[CF_SR_L1RLNK1]);}
    SRegL1RCFG*  SrL1RCFG  () const { return static_cast<SRegL1RCFG*>  (m_srf[CF_SR_L1RCFG]);}
    SRegDECFG*   SrDECFG   () const { return static_cast<SRegDECFG*>   (m_srf[CF_SR_DECFG]); }
    SRegDECTRL*  SrDECTRL  () const { return static_cast<SRegDECTRL*>  (m_srf[CF_SR_DECTRL]);}
    SRegDEVDS*   SrDEVDS   () const { return static_cast<SRegDEVDS*>   (m_srf[CF_SR_DEVDS]); }
    SRegRDBCR*   SrRDBCR   () const { return static_cast<SRegRDBCR*>   (m_srf[CF_SR_RDBCR]); }
    SRegRDBACR*  SrRDBACR  () const { return static_cast<SRegRDBACR*>  (m_srf[CF_SR_RDBACR]);}
    SRegRDBATAG* SrRDBATAG () const { return static_cast<SRegRDBATAG*> (m_srf[CF_SR_RDBATAG]);}
    SRegRDBADAT0* SrRDBADAT0() const { return static_cast<SRegRDBADAT0*>(m_srf[CF_SR_RDBADAT0]);}
    SRegRDBADAT1* SrRDBADAT1() const { return static_cast<SRegRDBADAT1*>(m_srf[CF_SR_RDBADAT1]);}
    SRegRDBADAT2* SrRDBADAT2() const { return static_cast<SRegRDBADAT2*>(m_srf[CF_SR_RDBADAT2]);}
    SRegRDBADAT3* SrRDBADAT3() const { return static_cast<SRegRDBADAT3*>(m_srf[CF_SR_RDBADAT3]);}
    SRegRDBSTAT* SrRDBSTAT () const { return static_cast<SRegRDBSTAT*> (m_srf[CF_SR_RDBSTAT]);}
    // System register group 14
    SRegPMCTRL*  SrPMCTRL0_G4MH20() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL0_G4MH20]);}
    SRegPMCTRL*  SrPMCTRL1_G4MH20() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL1_G4MH20]);}
    SRegPMCTRL*  SrPMCTRL2_G4MH20() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL2_G4MH20]);}
    SRegPMCTRL*  SrPMCTRL3_G4MH20() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL3_G4MH20]);}
    SRegPMCTRL*  SrPMCTRL4_G4MH20() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL4_G4MH20]);}
    SRegPMCTRL*  SrPMCTRL5_G4MH20() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL5_G4MH20]);}
    SRegPMCTRL*  SrPMCTRL6_G4MH20() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL6_G4MH20]);}
    SRegPMCTRL*  SrPMCTRL7_G4MH20() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL7_G4MH20]);}
    SRegPMCOUNT* SrPMCOUNT0_G4MH20() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT0_G4MH20]);}
    SRegPMCOUNT* SrPMCOUNT1_G4MH20() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT1_G4MH20]);}
    SRegPMCOUNT* SrPMCOUNT2_G4MH20() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT2_G4MH20]);}
    SRegPMCOUNT* SrPMCOUNT3_G4MH20() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT3_G4MH20]);}
    SRegPMCOUNT* SrPMCOUNT4_G4MH20() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT4_G4MH20]);}
    SRegPMCOUNT* SrPMCOUNT5_G4MH20() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT5_G4MH20]);}
    SRegPMCOUNT* SrPMCOUNT6_G4MH20() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT6_G4MH20]);}
    SRegPMCOUNT* SrPMCOUNT7_G4MH20() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT7_G4MH20]);}
    // System register group 15
    SRegPMSUBCND* SrPMSUBCND0() const { return static_cast<SRegPMSUBCND*>(m_srf[CF_SR_PMSUBCND0]);}
    SRegPMSUBCND* SrPMSUBCND1() const { return static_cast<SRegPMSUBCND*>(m_srf[CF_SR_PMSUBCND1]);}
    SRegPMSUBCND* SrPMSUBCND2() const { return static_cast<SRegPMSUBCND*>(m_srf[CF_SR_PMSUBCND2]);}
    SRegPMSUBCND* SrPMSUBCND3() const { return static_cast<SRegPMSUBCND*>(m_srf[CF_SR_PMSUBCND3]);}
    SRegPMSUBCND* SrPMSUBCND4() const { return static_cast<SRegPMSUBCND*>(m_srf[CF_SR_PMSUBCND4]);}
    SRegPMSUBCND* SrPMSUBCND5() const { return static_cast<SRegPMSUBCND*>(m_srf[CF_SR_PMSUBCND5]);}
    SRegPMSUBCND* SrPMSUBCND6() const { return static_cast<SRegPMSUBCND*>(m_srf[CF_SR_PMSUBCND6]);}
    SRegPMSUBCND* SrPMSUBCND7() const { return static_cast<SRegPMSUBCND*>(m_srf[CF_SR_PMSUBCND7]);}
    // System register group 32
    SRegBPC_N*   SrBPC0    () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC0]);  }
    SRegBPC_N*   SrBPC1    () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC1]);  }
    SRegBPC_N*   SrBPC2    () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC2]);  }
    SRegBPC_N*   SrBPC3    () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC3]);  }
    SRegBPC_N*   SrBPC4    () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC4]);  }
    SRegBPC_N*   SrBPC5    () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC5]);  }
    SRegBPC_N*   SrBPC6    () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC6]);  }
    SRegBPC_N*   SrBPC7    () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC7]);  }
    SRegBPC_N*   SrBPC8    () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC8]);  }
    SRegBPC_N*   SrBPC9    () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC9]);  }
    SRegBPC_N*   SrBPC10   () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC10]); }
    SRegBPC_N*   SrBPC11   () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC11]); }
    // System register group 33
    SRegBPAV_N*  SrBPAV0   () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV0]); }
    SRegBPAV_N*  SrBPAV1   () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV1]); }
    SRegBPAV_N*  SrBPAV2   () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV2]); }
    SRegBPAV_N*  SrBPAV3   () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV3]); }
    SRegBPAV_N*  SrBPAV4   () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV4]); }
    SRegBPAV_N*  SrBPAV5   () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV5]); }
    SRegBPAV_N*  SrBPAV6   () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV6]); }
    SRegBPAV_N*  SrBPAV7   () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV7]); }
    SRegBPAV_N*  SrBPAV8   () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV8]); }
    SRegBPAV_N*  SrBPAV9   () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV9]); }
    SRegBPAV_N*  SrBPAV10  () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV10]);}
    SRegBPAV_N*  SrBPAV11  () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV11]);}
    // System register group 34
    SRegBPAM_N*  SrBPAM0   () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM0]); }
    SRegBPAM_N*  SrBPAM1   () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM1]); }
    SRegBPAM_N*  SrBPAM2   () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM2]); }
    SRegBPAM_N*  SrBPAM3   () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM3]); }
    SRegBPAM_N*  SrBPAM4   () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM4]); }
    SRegBPAM_N*  SrBPAM5   () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM5]); }
    SRegBPAM_N*  SrBPAM6   () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM6]); }
    SRegBPAM_N*  SrBPAM7   () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM7]); }
    SRegBPAM_N*  SrBPAM8   () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM8]); }
    SRegBPAM_N*  SrBPAM9   () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM9]); }
    SRegBPAM_N*  SrBPAM10  () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM10]);}
    SRegBPAM_N*  SrBPAM11  () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM11]);}
    // System register group 37
    SRegMPLA_N*  SrMPLA0   () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA0]); }
    SRegMPLA_N*  SrMPLA1   () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA1]); }
    SRegMPLA_N*  SrMPLA2   () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA2]); }
    SRegMPLA_N*  SrMPLA3   () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA3]); }
    SRegMPLA_N*  SrMPLA4   () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA4]); }
    SRegMPLA_N*  SrMPLA5   () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA5]); }
    SRegMPLA_N*  SrMPLA6   () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA6]); }
    SRegMPLA_N*  SrMPLA7   () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA7]); }
    SRegMPLA_N*  SrMPLA8   () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA8]); }
    SRegMPLA_N*  SrMPLA9   () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA9]); }
    SRegMPLA_N*  SrMPLA10  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA10]);}
    SRegMPLA_N*  SrMPLA11  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA11]);}
    SRegMPLA_N*  SrMPLA12  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA12]);}
    SRegMPLA_N*  SrMPLA13  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA13]);}
    SRegMPLA_N*  SrMPLA14  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA14]);}
    SRegMPLA_N*  SrMPLA15  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA15]);}
    SRegMPLA_N*  SrMPLA16  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA16]);}
    SRegMPLA_N*  SrMPLA17  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA17]);}
    SRegMPLA_N*  SrMPLA18  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA18]);}
    SRegMPLA_N*  SrMPLA19  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA19]);}
    SRegMPLA_N*  SrMPLA20  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA20]);}
    SRegMPLA_N*  SrMPLA21  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA21]);}
    SRegMPLA_N*  SrMPLA22  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA22]);}
    SRegMPLA_N*  SrMPLA23  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA23]);}
    SRegMPLA_N*  SrMPLA24  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA24]);}
    SRegMPLA_N*  SrMPLA25  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA25]);}
    SRegMPLA_N*  SrMPLA26  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA26]);}
    SRegMPLA_N*  SrMPLA27  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA27]);}
    SRegMPLA_N*  SrMPLA28  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA28]);}
    SRegMPLA_N*  SrMPLA29  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA29]);}
    SRegMPLA_N*  SrMPLA30  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA30]);}
    SRegMPLA_N*  SrMPLA31  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA31]);}
    // System register group 38
    SRegMPUA_N*  SrMPUA0   () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA0]); }
    SRegMPUA_N*  SrMPUA1   () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA1]); }
    SRegMPUA_N*  SrMPUA2   () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA2]); }
    SRegMPUA_N*  SrMPUA3   () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA3]); }
    SRegMPUA_N*  SrMPUA4   () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA4]); }
    SRegMPUA_N*  SrMPUA5   () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA5]); }
    SRegMPUA_N*  SrMPUA6   () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA6]); }
    SRegMPUA_N*  SrMPUA7   () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA7]); }
    SRegMPUA_N*  SrMPUA8   () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA8]); }
    SRegMPUA_N*  SrMPUA9   () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA9]); }
    SRegMPUA_N*  SrMPUA10  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA10]);}
    SRegMPUA_N*  SrMPUA11  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA11]);}
    SRegMPUA_N*  SrMPUA12  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA12]);}
    SRegMPUA_N*  SrMPUA13  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA13]);}
    SRegMPUA_N*  SrMPUA14  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA14]);}
    SRegMPUA_N*  SrMPUA15  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA15]);}
    SRegMPUA_N*  SrMPUA16  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA16]);}
    SRegMPUA_N*  SrMPUA17  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA17]);}
    SRegMPUA_N*  SrMPUA18  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA18]);}
    SRegMPUA_N*  SrMPUA19  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA19]);}
    SRegMPUA_N*  SrMPUA20  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA20]);}
    SRegMPUA_N*  SrMPUA21  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA21]);}
    SRegMPUA_N*  SrMPUA22  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA22]);}
    SRegMPUA_N*  SrMPUA23  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA23]);}
    SRegMPUA_N*  SrMPUA24  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA24]);}
    SRegMPUA_N*  SrMPUA25  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA25]);}
    SRegMPUA_N*  SrMPUA26  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA26]);}
    SRegMPUA_N*  SrMPUA27  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA27]);}
    SRegMPUA_N*  SrMPUA28  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA28]);}
    SRegMPUA_N*  SrMPUA29  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA29]);}
    SRegMPUA_N*  SrMPUA30  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA30]);}
    SRegMPUA_N*  SrMPUA31  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA31]);}
    // System register group 39
    SRegMPAT_N*  SrMPAT0   () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT0]); }
    SRegMPAT_N*  SrMPAT1   () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT1]); }
    SRegMPAT_N*  SrMPAT2   () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT2]); }
    SRegMPAT_N*  SrMPAT3   () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT3]); }
    SRegMPAT_N*  SrMPAT4   () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT4]); }
    SRegMPAT_N*  SrMPAT5   () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT5]); }
    SRegMPAT_N*  SrMPAT6   () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT6]); }
    SRegMPAT_N*  SrMPAT7   () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT7]); }
    SRegMPAT_N*  SrMPAT8   () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT8]); }
    SRegMPAT_N*  SrMPAT9   () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT9]); }
    SRegMPAT_N*  SrMPAT10  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT10]);}
    SRegMPAT_N*  SrMPAT11  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT11]);}
    SRegMPAT_N*  SrMPAT12  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT12]);}
    SRegMPAT_N*  SrMPAT13  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT13]);}
    SRegMPAT_N*  SrMPAT14  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT14]);}
    SRegMPAT_N*  SrMPAT15  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT15]);}
    SRegMPAT_N*  SrMPAT16  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT16]);}
    SRegMPAT_N*  SrMPAT17  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT17]);}
    SRegMPAT_N*  SrMPAT18  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT18]);}
    SRegMPAT_N*  SrMPAT19  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT19]);}
    SRegMPAT_N*  SrMPAT20  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT20]);}
    SRegMPAT_N*  SrMPAT21  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT21]);}
    SRegMPAT_N*  SrMPAT22  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT22]);}
    SRegMPAT_N*  SrMPAT23  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT23]);}
    SRegMPAT_N*  SrMPAT24  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT24]);}
    SRegMPAT_N*  SrMPAT25  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT25]);}
    SRegMPAT_N*  SrMPAT26  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT26]);}
    SRegMPAT_N*  SrMPAT27  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT27]);}
    SRegMPAT_N*  SrMPAT28  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT28]);}
    SRegMPAT_N*  SrMPAT29  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT29]);}
    SRegMPAT_N*  SrMPAT30  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT30]);}
    SRegMPAT_N*  SrMPAT31  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT31]);}
    // System register group 40
    SRegMPLA_N*  SrMPLA0_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA0_B1]);}
    SRegMPLA_N*  SrMPLA1_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA1_B1]);}
    SRegMPLA_N*  SrMPLA2_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA2_B1]);}
    SRegMPLA_N*  SrMPLA3_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA3_B1]);}
    SRegMPLA_N*  SrMPLA4_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA4_B1]);}
    SRegMPLA_N*  SrMPLA5_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA5_B1]);}
    SRegMPLA_N*  SrMPLA6_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA6_B1]);}
    SRegMPLA_N*  SrMPLA7_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA7_B1]);}
    SRegMPLA_N*  SrMPLA8_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA8_B1]);}
    SRegMPLA_N*  SrMPLA9_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA9_B1]);}
    SRegMPLA_N*  SrMPLA10_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA10_B1]);}
    SRegMPLA_N*  SrMPLA11_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA11_B1]);}
    SRegMPLA_N*  SrMPLA12_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA12_B1]);}
    SRegMPLA_N*  SrMPLA13_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA13_B1]);}
    SRegMPLA_N*  SrMPLA14_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA14_B1]);}
    SRegMPLA_N*  SrMPLA15_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA15_B1]);}
    SRegMPLA_N*  SrMPLA16_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA16_B1]);}
    SRegMPLA_N*  SrMPLA17_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA17_B1]);}
    SRegMPLA_N*  SrMPLA18_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA18_B1]);}
    SRegMPLA_N*  SrMPLA19_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA19_B1]);}
    SRegMPLA_N*  SrMPLA20_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA20_B1]);}
    SRegMPLA_N*  SrMPLA21_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA21_B1]);}
    SRegMPLA_N*  SrMPLA22_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA22_B1]);}
    SRegMPLA_N*  SrMPLA23_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA23_B1]);}
    SRegMPLA_N*  SrMPLA24_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA24_B1]);}
    SRegMPLA_N*  SrMPLA25_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA25_B1]);}
    SRegMPLA_N*  SrMPLA26_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA26_B1]);}
    SRegMPLA_N*  SrMPLA27_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA27_B1]);}
    SRegMPLA_N*  SrMPLA28_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA28_B1]);}
    SRegMPLA_N*  SrMPLA29_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA29_B1]);}
    SRegMPLA_N*  SrMPLA30_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA30_B1]);}
    SRegMPLA_N*  SrMPLA31_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA31_B1]);}
    // System register group 41
    SRegMPUA_N*  SrMPUA0_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA0_B1]);}
    SRegMPUA_N*  SrMPUA1_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA1_B1]);}
    SRegMPUA_N*  SrMPUA2_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA2_B1]);}
    SRegMPUA_N*  SrMPUA3_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA3_B1]);}
    SRegMPUA_N*  SrMPUA4_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA4_B1]);}
    SRegMPUA_N*  SrMPUA5_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA5_B1]);}
    SRegMPUA_N*  SrMPUA6_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA6_B1]);}
    SRegMPUA_N*  SrMPUA7_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA7_B1]);}
    SRegMPUA_N*  SrMPUA8_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA8_B1]);}
    SRegMPUA_N*  SrMPUA9_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA9_B1]);}
    SRegMPUA_N*  SrMPUA10_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA10_B1]);}
    SRegMPUA_N*  SrMPUA11_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA11_B1]);}
    SRegMPUA_N*  SrMPUA12_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA12_B1]);}
    SRegMPUA_N*  SrMPUA13_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA13_B1]);}
    SRegMPUA_N*  SrMPUA14_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA14_B1]);}
    SRegMPUA_N*  SrMPUA15_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA15_B1]);}
    SRegMPUA_N*  SrMPUA16_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA16_B1]);}
    SRegMPUA_N*  SrMPUA17_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA17_B1]);}
    SRegMPUA_N*  SrMPUA18_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA18_B1]);}
    SRegMPUA_N*  SrMPUA19_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA19_B1]);}
    SRegMPUA_N*  SrMPUA20_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA20_B1]);}
    SRegMPUA_N*  SrMPUA21_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA21_B1]);}
    SRegMPUA_N*  SrMPUA22_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA22_B1]);}
    SRegMPUA_N*  SrMPUA23_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA23_B1]);}
    SRegMPUA_N*  SrMPUA24_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA24_B1]);}
    SRegMPUA_N*  SrMPUA25_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA25_B1]);}
    SRegMPUA_N*  SrMPUA26_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA26_B1]);}
    SRegMPUA_N*  SrMPUA27_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA27_B1]);}
    SRegMPUA_N*  SrMPUA28_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA28_B1]);}
    SRegMPUA_N*  SrMPUA29_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA29_B1]);}
    SRegMPUA_N*  SrMPUA30_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA30_B1]);}
    SRegMPUA_N*  SrMPUA31_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA31_B1]);}
    // System register group 42
    SRegMPAT_N*  SrMPAT0_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT0_B1]);}
    SRegMPAT_N*  SrMPAT1_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT1_B1]);}
    SRegMPAT_N*  SrMPAT2_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT2_B1]);}
    SRegMPAT_N*  SrMPAT3_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT3_B1]);}
    SRegMPAT_N*  SrMPAT4_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT4_B1]);}
    SRegMPAT_N*  SrMPAT5_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT5_B1]);}
    SRegMPAT_N*  SrMPAT6_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT6_B1]);}
    SRegMPAT_N*  SrMPAT7_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT7_B1]);}
    SRegMPAT_N*  SrMPAT8_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT8_B1]);}
    SRegMPAT_N*  SrMPAT9_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT9_B1]);}
    SRegMPAT_N*  SrMPAT10_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT10_B1]);}
    SRegMPAT_N*  SrMPAT11_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT11_B1]);}
    SRegMPAT_N*  SrMPAT12_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT12_B1]);}
    SRegMPAT_N*  SrMPAT13_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT13_B1]);}
    SRegMPAT_N*  SrMPAT14_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT14_B1]);}
    SRegMPAT_N*  SrMPAT15_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT15_B1]);}
    SRegMPAT_N*  SrMPAT16_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT16_B1]);}
    SRegMPAT_N*  SrMPAT17_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT17_B1]);}
    SRegMPAT_N*  SrMPAT18_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT18_B1]);}
    SRegMPAT_N*  SrMPAT19_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT19_B1]);}
    SRegMPAT_N*  SrMPAT20_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT20_B1]);}
    SRegMPAT_N*  SrMPAT21_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT21_B1]);}
    SRegMPAT_N*  SrMPAT22_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT22_B1]);}
    SRegMPAT_N*  SrMPAT23_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT23_B1]);}
    SRegMPAT_N*  SrMPAT24_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT24_B1]);}
    SRegMPAT_N*  SrMPAT25_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT25_B1]);}
    SRegMPAT_N*  SrMPAT26_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT26_B1]);}
    SRegMPAT_N*  SrMPAT27_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT27_B1]);}
    SRegMPAT_N*  SrMPAT28_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT28_B1]);}
    SRegMPAT_N*  SrMPAT29_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT29_B1]);}
    SRegMPAT_N*  SrMPAT30_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT30_B1]);}
    SRegMPAT_N*  SrMPAT31_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT31_B1]);}

private:
    friend class ProcElement;
    void InitSRegs (ContextId ctxt_id, ProcElementBase* pe);
};



class SRegFileIF {
public:
    virtual ~SRegFileIF() {};

    // System register group 0
    virtual SRegBaseEIPC*    SrVEIPC() const = 0;
    virtual SRegBasePSW*     SrVEIPSW() const = 0;
    virtual SRegBaseFEPC*    SrVFEPC() const = 0;
    virtual SRegBasePSW*     SrVFEPSW() const = 0;
    virtual SRegBasePSW*     SrVPSW() const = 0;
    virtual SRegFPSR*        SrFPSR() const = 0;
    virtual SRegFPEPC*       SrFPEPC() const = 0;
    virtual SRegFPST*        SrFPST() const = 0;
    virtual SRegFPCC*        SrFPCC() const = 0;
    virtual SRegFPCFG*       SrFPCFG() const = 0;
    virtual SRegFPEC*        SrFPEC() const = 0;
    virtual SRegBaseEIIC*    SrVEIIC() const = 0;
    virtual SRegBaseFEIC*    SrVFEIC() const = 0;
    virtual SRegPSWH*        SrPSWH() const = 0;
    virtual SRegCTPC*        SrCTPC() const = 0;
    virtual SRegCTPSW*       SrCTPSW() const = 0;
    virtual SRegEIPSWH*      SrEIPSWH() const = 0;
    virtual SRegFEPSWH*      SrFEPSWH() const = 0;
    virtual SRegCTBP*        SrCTBP() const = 0;
    virtual SRegSNZCFG*      SrSNZCFG() const = 0;
    virtual SRegBaseEIWR*    SrVEIWR() const = 0;
    virtual SRegBaseFEWR*    SrVFEWR() const = 0;
    // System register group 1
    virtual SRegBaseSPID*    SrVSPID() const = 0;
    virtual SRegBaseSPIDLIST* SrVSPIDLIST() const = 0;
    virtual SRegRBASE*       SrRBASE() const = 0;
    virtual SRegBaseEBASE*   SrVEBASE() const = 0;
    virtual SRegBaseINTBP*   SrVINTBP() const = 0;
    virtual SRegMCTL*        SrMCTL() const = 0;
    virtual SRegPID*         SrPID() const = 0;
    virtual SRegFPIPR*       SrFPIPR() const = 0;
    virtual SRegBaseSVLOCK*  SrVSVLOCK() const = 0;
    virtual SRegSCCFG*       SrSCCFG() const = 0;
    virtual SRegSCBP*        SrSCBP() const = 0;
    virtual SRegHVCFG*       SrHVCFG() const = 0;
    virtual SRegGMCFG*       SrGMCFG() const = 0;
    virtual SRegHVSB*        SrHVSB() const = 0;
    // System register group 2
    virtual SRegBasePEID*    SrVPEID() const = 0;
    virtual SRegBMID*        SrBMID() const = 0;
    virtual SRegBaseMEA*     SrVMEA() const = 0;
    virtual SRegBaseMEI*     SrVMEI() const = 0;
    virtual SRegISPR*        SrISPR() const = 0;
    virtual SRegIMSR*        SrIMSR() const = 0;
    virtual SRegICSR*        SrICSR() const = 0;
    virtual SRegBaseINTCFG*  SrVINTCFG() const = 0;
    virtual SRegBasePLMR*    SrVPLMR() const = 0;
    virtual SRegRBCR0*       SrRBCR0() const = 0;
    virtual SRegRBCR1*       SrRBCR1() const = 0;
    virtual SRegRBNR*        SrRBNR() const = 0;
    virtual SRegRBIP*        SrRBIP() const = 0;
    // System register group 3
    virtual SRegDBGEN*       SrDBGEN() const = 0;
    virtual SRegDBPSWH*      SrDBPSWH() const = 0;
    virtual SRegDBIC*        SrDBIC() const = 0;
    virtual SRegDBPC*        SrDBPC() const = 0;
    virtual SRegDBPSW*       SrDBPSW() const = 0;
    virtual SRegDIR0*        SrDIR0() const = 0;
    virtual SRegDIR1*        SrDIR1() const = 0;
    virtual SRegBPC*         SrBPC() const = 0;
    virtual SRegBPAV*        SrBPAV() const = 0;
    virtual SRegBPAM*        SrBPAM() const = 0;
    virtual SRegDBCFG*       SrDBCFG() const = 0;
    virtual SRegDBWR*        SrDBWR() const = 0;
    // System register group 4
    virtual SRegICTAGL*      SrICTAGL() const = 0;
    virtual SRegICTAGH*      SrICTAGH() const = 0;
    virtual SRegICDATL*      SrICDATL() const = 0;
    virtual SRegICDATH*      SrICDATH() const = 0;
    virtual SRegICCTRL*      SrICCTRL() const = 0;
    virtual SRegICCFG*       SrICCFG() const = 0;
    virtual SRegICERR*       SrICERR() const = 0;
    // System register group 5
    virtual SRegBaseMPM*     SrVMPM() const = 0;
    virtual SRegMPCFG*       SrMPCFG() const = 0;
    virtual SRegMCA*         SrMCA() const = 0;
    virtual SRegMCS*         SrMCS() const = 0;
    virtual SRegMCC*         SrMCC() const = 0;
    virtual SRegMCR*         SrMCR() const = 0;
    virtual SRegMCI*         SrMCI() const = 0;
    virtual SRegMPIDX*       SrMPIDX() const = 0;
    virtual SRegMPBK*        SrMPBK() const = 0;
    virtual SRegMPLA*        SrMPLA() const = 0;
    virtual SRegMPUA*        SrMPUA() const = 0;
    virtual SRegMPAT*        SrMPAT() const = 0;
    virtual SRegMPID*        SrMPID0() const = 0;
    virtual SRegMPID*        SrMPID1() const = 0;
    virtual SRegMPID*        SrMPID2() const = 0;
    virtual SRegMPID*        SrMPID3() const = 0;
    virtual SRegMPID*        SrMPID4() const = 0;
    virtual SRegMPID*        SrMPID5() const = 0;
    virtual SRegMPID*        SrMPID6() const = 0;
    virtual SRegMPID*        SrMPID7() const = 0;
    // System register group 9
    virtual SRegGMEIPC*      SrGMEIPC() const = 0;
    virtual SRegGMEIPSW*     SrGMEIPSW() const = 0;
    virtual SRegGMFEPC*      SrGMFEPC() const = 0;
    virtual SRegGMFEPSW*     SrGMFEPSW() const = 0;
    virtual SRegGMPSW*       SrGMPSW() const = 0;
    virtual SRegGMMEA*       SrGMMEA() const = 0;
    virtual SRegGMMEI*       SrGMMEI() const = 0;
    virtual SRegGMEIIC*      SrGMEIIC() const = 0;
    virtual SRegGMFEIC*      SrGMFEIC() const = 0;
    virtual SRegGMSPID*      SrGMSPID() const = 0;
    virtual SRegGMSPIDLIST*  SrGMSPIDLIST() const = 0;
    virtual SRegGMEBASE*     SrGMEBASE() const = 0;
    virtual SRegGMINTBP*     SrGMINTBP() const = 0;
    virtual SRegGMINTCFG*    SrGMINTCFG() const = 0;
    virtual SRegGMPLMR*      SrGMPLMR() const = 0;
    virtual SRegGMSVLOCK*    SrGMSVLOCK() const = 0;
    virtual SRegGMMPM*       SrGMMPM() const = 0;
    virtual SRegGMEIWR*      SrGMEIWR() const = 0;
    virtual SRegGMFEWR*      SrGMFEWR() const = 0;
    virtual SRegGMPEID*      SrGMPEID() const = 0;
    // System register group 10
    virtual SRegFXSR*        SrFXSR() const = 0;
    virtual SRegFXST*        SrFXST() const = 0;
    virtual SRegFXINFO*      SrFXINFO() const = 0;
    virtual SRegFXCFG*       SrFXCFG() const = 0;
    virtual SRegFXXC*        SrFXXC() const = 0;
    virtual SRegFXXP*        SrFXXP() const = 0;
    // System register group 11
    virtual SRegTSCOUNTL*    SrTSCOUNTL() const = 0;
    virtual SRegTSCOUNTH*    SrTSCOUNTH() const = 0;
    virtual SRegTSCTRL*      SrTSCTRL() const = 0;
    virtual SRegPMUMCTRL*    SrPMUMCTRL() const = 0;
    virtual SRegPMGMCTRL*    SrPMGMCTRL() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT0_G4X1X() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL0_G4X1X() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT1_G4X1X() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL1_G4X1X() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT2_G4X1X() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL2_G4X1X() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT3_G4X1X() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL3_G4X1X() const = 0;
    // System register group 12
    virtual SRegLSTEST0*     SrLSTEST0() const = 0;
    virtual SRegLSTEST1*     SrLSTEST1() const = 0;
    virtual SRegLSCFG*       SrLSCFG() const = 0;
    virtual SRegICBKEY*      SrICBKEY() const = 0;
    virtual SRegIFCR*        SrIFCR() const = 0;
    virtual SRegIFCR1*       SrIFCR1() const = 0;
    virtual SRegBRPCTRL0*    SrBRPCTRL0() const = 0;
    virtual SRegBRPCTRL1*    SrBRPCTRL1() const = 0;
    virtual SRegBRPCFG*      SrBRPCFG() const = 0;
    virtual SRegBRPACTRL*    SrBRPACTRL() const = 0;
    virtual SRegBRPDATA*     SrBRPDATA() const = 0;
    // System register group 13
    virtual SRegDCBKEY*      SrDCBKEY() const = 0;
    virtual SRegLSUCR*       SrLSUCR() const = 0;
    virtual SRegLSULNK0*     SrLSULNK0() const = 0;
    virtual SRegLSULNK1*     SrLSULNK1() const = 0;
    virtual SRegL1RLCR*      SrL1RLCR() const = 0;
    virtual SRegL1RLNK0*     SrL1RLNK0() const = 0;
    virtual SRegL1RLNK1*     SrL1RLNK1() const = 0;
    virtual SRegL1RCFG*      SrL1RCFG() const = 0;
    virtual SRegDECFG*       SrDECFG() const = 0;
    virtual SRegDECTRL*      SrDECTRL() const = 0;
    virtual SRegDEVDS*       SrDEVDS() const = 0;
    virtual SRegRDBCR*       SrRDBCR() const = 0;
    virtual SRegRDBACR*      SrRDBACR() const = 0;
    virtual SRegRDBATAG*     SrRDBATAG() const = 0;
    virtual SRegRDBADAT0*    SrRDBADAT0() const = 0;
    virtual SRegRDBADAT1*    SrRDBADAT1() const = 0;
    virtual SRegRDBADAT2*    SrRDBADAT2() const = 0;
    virtual SRegRDBADAT3*    SrRDBADAT3() const = 0;
    virtual SRegRDBSTAT*     SrRDBSTAT() const = 0;
    // System register group 14
    virtual SRegPMCTRL*      SrPMCTRL0_G4MH20() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL1_G4MH20() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL2_G4MH20() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL3_G4MH20() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL4_G4MH20() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL5_G4MH20() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL6_G4MH20() const = 0;
    virtual SRegPMCTRL*      SrPMCTRL7_G4MH20() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT0_G4MH20() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT1_G4MH20() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT2_G4MH20() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT3_G4MH20() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT4_G4MH20() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT5_G4MH20() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT6_G4MH20() const = 0;
    virtual SRegPMCOUNT*     SrPMCOUNT7_G4MH20() const = 0;
    // System register group 15
    virtual SRegPMSUBCND*    SrPMSUBCND0() const = 0;
    virtual SRegPMSUBCND*    SrPMSUBCND1() const = 0;
    virtual SRegPMSUBCND*    SrPMSUBCND2() const = 0;
    virtual SRegPMSUBCND*    SrPMSUBCND3() const = 0;
    virtual SRegPMSUBCND*    SrPMSUBCND4() const = 0;
    virtual SRegPMSUBCND*    SrPMSUBCND5() const = 0;
    virtual SRegPMSUBCND*    SrPMSUBCND6() const = 0;
    virtual SRegPMSUBCND*    SrPMSUBCND7() const = 0;
    // System register group 32
    virtual SRegBPC_N*       SrBPC0() const = 0;
    virtual SRegBPC_N*       SrBPC1() const = 0;
    virtual SRegBPC_N*       SrBPC2() const = 0;
    virtual SRegBPC_N*       SrBPC3() const = 0;
    virtual SRegBPC_N*       SrBPC4() const = 0;
    virtual SRegBPC_N*       SrBPC5() const = 0;
    virtual SRegBPC_N*       SrBPC6() const = 0;
    virtual SRegBPC_N*       SrBPC7() const = 0;
    virtual SRegBPC_N*       SrBPC8() const = 0;
    virtual SRegBPC_N*       SrBPC9() const = 0;
    virtual SRegBPC_N*       SrBPC10() const = 0;
    virtual SRegBPC_N*       SrBPC11() const = 0;
    // System register group 33
    virtual SRegBPAV_N*      SrBPAV0() const = 0;
    virtual SRegBPAV_N*      SrBPAV1() const = 0;
    virtual SRegBPAV_N*      SrBPAV2() const = 0;
    virtual SRegBPAV_N*      SrBPAV3() const = 0;
    virtual SRegBPAV_N*      SrBPAV4() const = 0;
    virtual SRegBPAV_N*      SrBPAV5() const = 0;
    virtual SRegBPAV_N*      SrBPAV6() const = 0;
    virtual SRegBPAV_N*      SrBPAV7() const = 0;
    virtual SRegBPAV_N*      SrBPAV8() const = 0;
    virtual SRegBPAV_N*      SrBPAV9() const = 0;
    virtual SRegBPAV_N*      SrBPAV10() const = 0;
    virtual SRegBPAV_N*      SrBPAV11() const = 0;
    // System register group 34
    virtual SRegBPAM_N*      SrBPAM0() const = 0;
    virtual SRegBPAM_N*      SrBPAM1() const = 0;
    virtual SRegBPAM_N*      SrBPAM2() const = 0;
    virtual SRegBPAM_N*      SrBPAM3() const = 0;
    virtual SRegBPAM_N*      SrBPAM4() const = 0;
    virtual SRegBPAM_N*      SrBPAM5() const = 0;
    virtual SRegBPAM_N*      SrBPAM6() const = 0;
    virtual SRegBPAM_N*      SrBPAM7() const = 0;
    virtual SRegBPAM_N*      SrBPAM8() const = 0;
    virtual SRegBPAM_N*      SrBPAM9() const = 0;
    virtual SRegBPAM_N*      SrBPAM10() const = 0;
    virtual SRegBPAM_N*      SrBPAM11() const = 0;
    // System register group 37
    virtual SRegMPLA_N*      SrMPLA0() const = 0;
    virtual SRegMPLA_N*      SrMPLA1() const = 0;
    virtual SRegMPLA_N*      SrMPLA2() const = 0;
    virtual SRegMPLA_N*      SrMPLA3() const = 0;
    virtual SRegMPLA_N*      SrMPLA4() const = 0;
    virtual SRegMPLA_N*      SrMPLA5() const = 0;
    virtual SRegMPLA_N*      SrMPLA6() const = 0;
    virtual SRegMPLA_N*      SrMPLA7() const = 0;
    virtual SRegMPLA_N*      SrMPLA8() const = 0;
    virtual SRegMPLA_N*      SrMPLA9() const = 0;
    virtual SRegMPLA_N*      SrMPLA10() const = 0;
    virtual SRegMPLA_N*      SrMPLA11() const = 0;
    virtual SRegMPLA_N*      SrMPLA12() const = 0;
    virtual SRegMPLA_N*      SrMPLA13() const = 0;
    virtual SRegMPLA_N*      SrMPLA14() const = 0;
    virtual SRegMPLA_N*      SrMPLA15() const = 0;
    virtual SRegMPLA_N*      SrMPLA16() const = 0;
    virtual SRegMPLA_N*      SrMPLA17() const = 0;
    virtual SRegMPLA_N*      SrMPLA18() const = 0;
    virtual SRegMPLA_N*      SrMPLA19() const = 0;
    virtual SRegMPLA_N*      SrMPLA20() const = 0;
    virtual SRegMPLA_N*      SrMPLA21() const = 0;
    virtual SRegMPLA_N*      SrMPLA22() const = 0;
    virtual SRegMPLA_N*      SrMPLA23() const = 0;
    virtual SRegMPLA_N*      SrMPLA24() const = 0;
    virtual SRegMPLA_N*      SrMPLA25() const = 0;
    virtual SRegMPLA_N*      SrMPLA26() const = 0;
    virtual SRegMPLA_N*      SrMPLA27() const = 0;
    virtual SRegMPLA_N*      SrMPLA28() const = 0;
    virtual SRegMPLA_N*      SrMPLA29() const = 0;
    virtual SRegMPLA_N*      SrMPLA30() const = 0;
    virtual SRegMPLA_N*      SrMPLA31() const = 0;
    // System register group 38
    virtual SRegMPUA_N*      SrMPUA0() const = 0;
    virtual SRegMPUA_N*      SrMPUA1() const = 0;
    virtual SRegMPUA_N*      SrMPUA2() const = 0;
    virtual SRegMPUA_N*      SrMPUA3() const = 0;
    virtual SRegMPUA_N*      SrMPUA4() const = 0;
    virtual SRegMPUA_N*      SrMPUA5() const = 0;
    virtual SRegMPUA_N*      SrMPUA6() const = 0;
    virtual SRegMPUA_N*      SrMPUA7() const = 0;
    virtual SRegMPUA_N*      SrMPUA8() const = 0;
    virtual SRegMPUA_N*      SrMPUA9() const = 0;
    virtual SRegMPUA_N*      SrMPUA10() const = 0;
    virtual SRegMPUA_N*      SrMPUA11() const = 0;
    virtual SRegMPUA_N*      SrMPUA12() const = 0;
    virtual SRegMPUA_N*      SrMPUA13() const = 0;
    virtual SRegMPUA_N*      SrMPUA14() const = 0;
    virtual SRegMPUA_N*      SrMPUA15() const = 0;
    virtual SRegMPUA_N*      SrMPUA16() const = 0;
    virtual SRegMPUA_N*      SrMPUA17() const = 0;
    virtual SRegMPUA_N*      SrMPUA18() const = 0;
    virtual SRegMPUA_N*      SrMPUA19() const = 0;
    virtual SRegMPUA_N*      SrMPUA20() const = 0;
    virtual SRegMPUA_N*      SrMPUA21() const = 0;
    virtual SRegMPUA_N*      SrMPUA22() const = 0;
    virtual SRegMPUA_N*      SrMPUA23() const = 0;
    virtual SRegMPUA_N*      SrMPUA24() const = 0;
    virtual SRegMPUA_N*      SrMPUA25() const = 0;
    virtual SRegMPUA_N*      SrMPUA26() const = 0;
    virtual SRegMPUA_N*      SrMPUA27() const = 0;
    virtual SRegMPUA_N*      SrMPUA28() const = 0;
    virtual SRegMPUA_N*      SrMPUA29() const = 0;
    virtual SRegMPUA_N*      SrMPUA30() const = 0;
    virtual SRegMPUA_N*      SrMPUA31() const = 0;
    // System register group 39
    virtual SRegMPAT_N*      SrMPAT0() const = 0;
    virtual SRegMPAT_N*      SrMPAT1() const = 0;
    virtual SRegMPAT_N*      SrMPAT2() const = 0;
    virtual SRegMPAT_N*      SrMPAT3() const = 0;
    virtual SRegMPAT_N*      SrMPAT4() const = 0;
    virtual SRegMPAT_N*      SrMPAT5() const = 0;
    virtual SRegMPAT_N*      SrMPAT6() const = 0;
    virtual SRegMPAT_N*      SrMPAT7() const = 0;
    virtual SRegMPAT_N*      SrMPAT8() const = 0;
    virtual SRegMPAT_N*      SrMPAT9() const = 0;
    virtual SRegMPAT_N*      SrMPAT10() const = 0;
    virtual SRegMPAT_N*      SrMPAT11() const = 0;
    virtual SRegMPAT_N*      SrMPAT12() const = 0;
    virtual SRegMPAT_N*      SrMPAT13() const = 0;
    virtual SRegMPAT_N*      SrMPAT14() const = 0;
    virtual SRegMPAT_N*      SrMPAT15() const = 0;
    virtual SRegMPAT_N*      SrMPAT16() const = 0;
    virtual SRegMPAT_N*      SrMPAT17() const = 0;
    virtual SRegMPAT_N*      SrMPAT18() const = 0;
    virtual SRegMPAT_N*      SrMPAT19() const = 0;
    virtual SRegMPAT_N*      SrMPAT20() const = 0;
    virtual SRegMPAT_N*      SrMPAT21() const = 0;
    virtual SRegMPAT_N*      SrMPAT22() const = 0;
    virtual SRegMPAT_N*      SrMPAT23() const = 0;
    virtual SRegMPAT_N*      SrMPAT24() const = 0;
    virtual SRegMPAT_N*      SrMPAT25() const = 0;
    virtual SRegMPAT_N*      SrMPAT26() const = 0;
    virtual SRegMPAT_N*      SrMPAT27() const = 0;
    virtual SRegMPAT_N*      SrMPAT28() const = 0;
    virtual SRegMPAT_N*      SrMPAT29() const = 0;
    virtual SRegMPAT_N*      SrMPAT30() const = 0;
    virtual SRegMPAT_N*      SrMPAT31() const = 0;
    // System register group 40
    virtual SRegMPLA_N*      SrMPLA0_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA1_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA2_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA3_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA4_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA5_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA6_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA7_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA8_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA9_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA10_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA11_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA12_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA13_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA14_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA15_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA16_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA17_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA18_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA19_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA20_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA21_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA22_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA23_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA24_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA25_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA26_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA27_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA28_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA29_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA30_B1() const = 0;
    virtual SRegMPLA_N*      SrMPLA31_B1() const = 0;
    // System register group 41
    virtual SRegMPUA_N*      SrMPUA0_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA1_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA2_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA3_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA4_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA5_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA6_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA7_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA8_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA9_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA10_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA11_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA12_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA13_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA14_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA15_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA16_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA17_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA18_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA19_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA20_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA21_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA22_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA23_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA24_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA25_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA26_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA27_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA28_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA29_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA30_B1() const = 0;
    virtual SRegMPUA_N*      SrMPUA31_B1() const = 0;
    // System register group 42
    virtual SRegMPAT_N*      SrMPAT0_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT1_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT2_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT3_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT4_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT5_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT6_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT7_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT8_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT9_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT10_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT11_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT12_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT13_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT14_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT15_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT16_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT17_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT18_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT19_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT20_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT21_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT22_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT23_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT24_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT25_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT26_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT27_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT28_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT29_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT30_B1() const = 0;
    virtual SRegMPAT_N*      SrMPAT31_B1() const = 0;
};

class SRegFile : public SRegFileBase, SRegFileIF {
public:
    SRegFile (NcSRegFile* nc_srf);
    void LinkSRegs (SRegFileBase* srfile);

    // System register group 0
    SRegBaseEIPC* SrVEIPC   () const { return static_cast<SRegBaseEIPC*>(m_srf[CF_SR_VEIPC]); }
    SRegBasePSW* SrVEIPSW  () const { return static_cast<SRegBasePSW*> (m_srf[CF_SR_VEIPSW]);}
    SRegBaseFEPC* SrVFEPC   () const { return static_cast<SRegBaseFEPC*>(m_srf[CF_SR_VFEPC]); }
    SRegBasePSW* SrVFEPSW  () const { return static_cast<SRegBasePSW*> (m_srf[CF_SR_VFEPSW]);}
    SRegBasePSW* SrVPSW    () const { return static_cast<SRegBasePSW*> (m_srf[CF_SR_VPSW]);  }
    SRegFPSR*    SrFPSR    () const { return static_cast<SRegFPSR*>    (m_srf[CF_SR_FPSR]);  }
    SRegFPEPC*   SrFPEPC   () const { return static_cast<SRegFPEPC*>   (m_srf[CF_SR_FPEPC]); }
    SRegFPST*    SrFPST    () const { return static_cast<SRegFPST*>    (m_srf[CF_SR_FPST]);  }
    SRegFPCC*    SrFPCC    () const { return static_cast<SRegFPCC*>    (m_srf[CF_SR_FPCC]);  }
    SRegFPCFG*   SrFPCFG   () const { return static_cast<SRegFPCFG*>   (m_srf[CF_SR_FPCFG]); }
    SRegFPEC*    SrFPEC    () const { return static_cast<SRegFPEC*>    (m_srf[CF_SR_FPEC]);  }
    SRegBaseEIIC* SrVEIIC   () const { return static_cast<SRegBaseEIIC*>(m_srf[CF_SR_VEIIC]); }
    SRegBaseFEIC* SrVFEIC   () const { return static_cast<SRegBaseFEIC*>(m_srf[CF_SR_VFEIC]); }
    SRegPSWH*    SrPSWH    () const { return static_cast<SRegPSWH*>    (m_srf[CF_SR_PSWH]);  }
    SRegCTPC*    SrCTPC    () const { return static_cast<SRegCTPC*>    (m_srf[CF_SR_CTPC]);  }
    SRegCTPSW*   SrCTPSW   () const { return static_cast<SRegCTPSW*>   (m_srf[CF_SR_CTPSW]); }
    SRegEIPSWH*  SrEIPSWH  () const { return static_cast<SRegEIPSWH*>  (m_srf[CF_SR_EIPSWH]);}
    SRegFEPSWH*  SrFEPSWH  () const { return static_cast<SRegFEPSWH*>  (m_srf[CF_SR_FEPSWH]);}
    SRegCTBP*    SrCTBP    () const { return static_cast<SRegCTBP*>    (m_srf[CF_SR_CTBP]);  }
    SRegSNZCFG*  SrSNZCFG  () const { return static_cast<SRegSNZCFG*>  (m_srf[CF_SR_SNZCFG]);}
    SRegBaseEIWR* SrVEIWR   () const { return static_cast<SRegBaseEIWR*>(m_srf[CF_SR_VEIWR]); }
    SRegBaseFEWR* SrVFEWR   () const { return static_cast<SRegBaseFEWR*>(m_srf[CF_SR_VFEWR]); }
    // System register group 1
    SRegBaseSPID* SrVSPID   () const { return static_cast<SRegBaseSPID*>(m_srf[CF_SR_VSPID]); }
    SRegBaseSPIDLIST* SrVSPIDLIST() const { return static_cast<SRegBaseSPIDLIST*>(m_srf[CF_SR_VSPIDLIST]);}
    SRegRBASE*   SrRBASE   () const { return static_cast<SRegRBASE*>   (m_srf[CF_SR_RBASE]); }
    SRegBaseEBASE* SrVEBASE  () const { return static_cast<SRegBaseEBASE*>(m_srf[CF_SR_VEBASE]);}
    SRegBaseINTBP* SrVINTBP  () const { return static_cast<SRegBaseINTBP*>(m_srf[CF_SR_VINTBP]);}
    SRegMCTL*    SrMCTL    () const { return static_cast<SRegMCTL*>    (m_srf[CF_SR_MCTL]);  }
    SRegPID*     SrPID     () const { return static_cast<SRegPID*>     (m_srf[CF_SR_PID]);   }
    SRegFPIPR*   SrFPIPR   () const { return static_cast<SRegFPIPR*>   (m_srf[CF_SR_FPIPR]); }
    SRegBaseSVLOCK* SrVSVLOCK () const { return static_cast<SRegBaseSVLOCK*>(m_srf[CF_SR_VSVLOCK]);}
    SRegSCCFG*   SrSCCFG   () const { return static_cast<SRegSCCFG*>   (m_srf[CF_SR_SCCFG]); }
    SRegSCBP*    SrSCBP    () const { return static_cast<SRegSCBP*>    (m_srf[CF_SR_SCBP]);  }
    SRegHVCFG*   SrHVCFG   () const { return static_cast<SRegHVCFG*>   (m_srf[CF_SR_HVCFG]); }
    SRegGMCFG*   SrGMCFG   () const { return static_cast<SRegGMCFG*>   (m_srf[CF_SR_GMCFG]); }
    SRegHVSB*    SrHVSB    () const { return static_cast<SRegHVSB*>    (m_srf[CF_SR_HVSB]);  }
    // System register group 2
    SRegBasePEID* SrVPEID   () const { return static_cast<SRegBasePEID*>(m_srf[CF_SR_VPEID]); }
    SRegBMID*    SrBMID    () const { return static_cast<SRegBMID*>    (m_srf[CF_SR_BMID]);  }
    SRegBaseMEA* SrVMEA    () const { return static_cast<SRegBaseMEA*> (m_srf[CF_SR_VMEA]);  }
    SRegBaseMEI* SrVMEI    () const { return static_cast<SRegBaseMEI*> (m_srf[CF_SR_VMEI]);  }
    SRegISPR*    SrISPR    () const { return static_cast<SRegISPR*>    (m_srf[CF_SR_ISPR]);  }
    SRegIMSR*    SrIMSR    () const { return static_cast<SRegIMSR*>    (m_srf[CF_SR_IMSR]);  }
    SRegICSR*    SrICSR    () const { return static_cast<SRegICSR*>    (m_srf[CF_SR_ICSR]);  }
    SRegBaseINTCFG* SrVINTCFG () const { return static_cast<SRegBaseINTCFG*>(m_srf[CF_SR_VINTCFG]);}
    SRegBasePLMR* SrVPLMR   () const { return static_cast<SRegBasePLMR*>(m_srf[CF_SR_VPLMR]); }
    SRegRBCR0*   SrRBCR0   () const { return static_cast<SRegRBCR0*>   (m_srf[CF_SR_RBCR0]); }
    SRegRBCR1*   SrRBCR1   () const { return static_cast<SRegRBCR1*>   (m_srf[CF_SR_RBCR1]); }
    SRegRBNR*    SrRBNR    () const { return static_cast<SRegRBNR*>    (m_srf[CF_SR_RBNR]);  }
    SRegRBIP*    SrRBIP    () const { return static_cast<SRegRBIP*>    (m_srf[CF_SR_RBIP]);  }
    // System register group 3
    SRegDBGEN*   SrDBGEN   () const { return static_cast<SRegDBGEN*>   (m_srf[CF_SR_DBGEN]); }
    SRegDBPSWH*  SrDBPSWH  () const { return static_cast<SRegDBPSWH*>  (m_srf[CF_SR_DBPSWH]);}
    SRegDBIC*    SrDBIC    () const { return static_cast<SRegDBIC*>    (m_srf[CF_SR_DBIC]);  }
    SRegDBPC*    SrDBPC    () const { return static_cast<SRegDBPC*>    (m_srf[CF_SR_DBPC]);  }
    SRegDBPSW*   SrDBPSW   () const { return static_cast<SRegDBPSW*>   (m_srf[CF_SR_DBPSW]); }
    SRegDIR0*    SrDIR0    () const { return static_cast<SRegDIR0*>    (m_srf[CF_SR_DIR0]);  }
    SRegDIR1*    SrDIR1    () const { return static_cast<SRegDIR1*>    (m_srf[CF_SR_DIR1]);  }
    SRegBPC*     SrBPC     () const { return static_cast<SRegBPC*>     (m_srf[CF_SR_BPC]);   }
    SRegBPAV*    SrBPAV    () const { return static_cast<SRegBPAV*>    (m_srf[CF_SR_BPAV]);  }
    SRegBPAM*    SrBPAM    () const { return static_cast<SRegBPAM*>    (m_srf[CF_SR_BPAM]);  }
    SRegDBCFG*   SrDBCFG   () const { return static_cast<SRegDBCFG*>   (m_srf[CF_SR_DBCFG]); }
    SRegDBWR*    SrDBWR    () const { return static_cast<SRegDBWR*>    (m_srf[CF_SR_DBWR]);  }
    // System register group 4
    SRegICTAGL*  SrICTAGL  () const { return static_cast<SRegICTAGL*>  (m_srf[CF_SR_ICTAGL]);}
    SRegICTAGH*  SrICTAGH  () const { return static_cast<SRegICTAGH*>  (m_srf[CF_SR_ICTAGH]);}
    SRegICDATL*  SrICDATL  () const { return static_cast<SRegICDATL*>  (m_srf[CF_SR_ICDATL]);}
    SRegICDATH*  SrICDATH  () const { return static_cast<SRegICDATH*>  (m_srf[CF_SR_ICDATH]);}
    SRegICCTRL*  SrICCTRL  () const { return static_cast<SRegICCTRL*>  (m_srf[CF_SR_ICCTRL]);}
    SRegICCFG*   SrICCFG   () const { return static_cast<SRegICCFG*>   (m_srf[CF_SR_ICCFG]); }
    SRegICERR*   SrICERR   () const { return static_cast<SRegICERR*>   (m_srf[CF_SR_ICERR]); }
    // System register group 5
    SRegBaseMPM* SrVMPM    () const { return static_cast<SRegBaseMPM*> (m_srf[CF_SR_VMPM]);  }
    SRegMPCFG*   SrMPCFG   () const { return static_cast<SRegMPCFG*>   (m_srf[CF_SR_MPCFG]); }
    SRegMCA*     SrMCA     () const { return static_cast<SRegMCA*>     (m_srf[CF_SR_MCA]);   }
    SRegMCS*     SrMCS     () const { return static_cast<SRegMCS*>     (m_srf[CF_SR_MCS]);   }
    SRegMCC*     SrMCC     () const { return static_cast<SRegMCC*>     (m_srf[CF_SR_MCC]);   }
    SRegMCR*     SrMCR     () const { return static_cast<SRegMCR*>     (m_srf[CF_SR_MCR]);   }
    SRegMCI*     SrMCI     () const { return static_cast<SRegMCI*>     (m_srf[CF_SR_MCI]);   }
    SRegMPIDX*   SrMPIDX   () const { return static_cast<SRegMPIDX*>   (m_srf[CF_SR_MPIDX]); }
    SRegMPBK*    SrMPBK    () const { return static_cast<SRegMPBK*>    (m_srf[CF_SR_MPBK]);  }
    SRegMPLA*    SrMPLA    () const { return static_cast<SRegMPLA*>    (m_srf[CF_SR_MPLA]);  }
    SRegMPUA*    SrMPUA    () const { return static_cast<SRegMPUA*>    (m_srf[CF_SR_MPUA]);  }
    SRegMPAT*    SrMPAT    () const { return static_cast<SRegMPAT*>    (m_srf[CF_SR_MPAT]);  }
    SRegMPID*    SrMPID0   () const { return static_cast<SRegMPID*>    (m_srf[CF_SR_MPID0]); }
    SRegMPID*    SrMPID1   () const { return static_cast<SRegMPID*>    (m_srf[CF_SR_MPID1]); }
    SRegMPID*    SrMPID2   () const { return static_cast<SRegMPID*>    (m_srf[CF_SR_MPID2]); }
    SRegMPID*    SrMPID3   () const { return static_cast<SRegMPID*>    (m_srf[CF_SR_MPID3]); }
    SRegMPID*    SrMPID4   () const { return static_cast<SRegMPID*>    (m_srf[CF_SR_MPID4]); }
    SRegMPID*    SrMPID5   () const { return static_cast<SRegMPID*>    (m_srf[CF_SR_MPID5]); }
    SRegMPID*    SrMPID6   () const { return static_cast<SRegMPID*>    (m_srf[CF_SR_MPID6]); }
    SRegMPID*    SrMPID7   () const { return static_cast<SRegMPID*>    (m_srf[CF_SR_MPID7]); }
    // System register group 9
    SRegGMEIPC*  SrGMEIPC  () const { return static_cast<SRegGMEIPC*>  (m_srf[CF_SR_GMEIPC]);}
    SRegGMEIPSW* SrGMEIPSW () const { return static_cast<SRegGMEIPSW*> (m_srf[CF_SR_GMEIPSW]);}
    SRegGMFEPC*  SrGMFEPC  () const { return static_cast<SRegGMFEPC*>  (m_srf[CF_SR_GMFEPC]);}
    SRegGMFEPSW* SrGMFEPSW () const { return static_cast<SRegGMFEPSW*> (m_srf[CF_SR_GMFEPSW]);}
    SRegGMPSW*   SrGMPSW   () const { return static_cast<SRegGMPSW*>   (m_srf[CF_SR_GMPSW]); }
    SRegGMMEA*   SrGMMEA   () const { return static_cast<SRegGMMEA*>   (m_srf[CF_SR_GMMEA]); }
    SRegGMMEI*   SrGMMEI   () const { return static_cast<SRegGMMEI*>   (m_srf[CF_SR_GMMEI]); }
    SRegGMEIIC*  SrGMEIIC  () const { return static_cast<SRegGMEIIC*>  (m_srf[CF_SR_GMEIIC]);}
    SRegGMFEIC*  SrGMFEIC  () const { return static_cast<SRegGMFEIC*>  (m_srf[CF_SR_GMFEIC]);}
    SRegGMSPID*  SrGMSPID  () const { return static_cast<SRegGMSPID*>  (m_srf[CF_SR_GMSPID]);}
    SRegGMSPIDLIST* SrGMSPIDLIST() const { return static_cast<SRegGMSPIDLIST*>(m_srf[CF_SR_GMSPIDLIST]);}
    SRegGMEBASE* SrGMEBASE () const { return static_cast<SRegGMEBASE*> (m_srf[CF_SR_GMEBASE]);}
    SRegGMINTBP* SrGMINTBP () const { return static_cast<SRegGMINTBP*> (m_srf[CF_SR_GMINTBP]);}
    SRegGMINTCFG* SrGMINTCFG() const { return static_cast<SRegGMINTCFG*>(m_srf[CF_SR_GMINTCFG]);}
    SRegGMPLMR*  SrGMPLMR  () const { return static_cast<SRegGMPLMR*>  (m_srf[CF_SR_GMPLMR]);}
    SRegGMSVLOCK* SrGMSVLOCK() const { return static_cast<SRegGMSVLOCK*>(m_srf[CF_SR_GMSVLOCK]);}
    SRegGMMPM*   SrGMMPM   () const { return static_cast<SRegGMMPM*>   (m_srf[CF_SR_GMMPM]); }
    SRegGMEIWR*  SrGMEIWR  () const { return static_cast<SRegGMEIWR*>  (m_srf[CF_SR_GMEIWR]);}
    SRegGMFEWR*  SrGMFEWR  () const { return static_cast<SRegGMFEWR*>  (m_srf[CF_SR_GMFEWR]);}
    SRegGMPEID*  SrGMPEID  () const { return static_cast<SRegGMPEID*>  (m_srf[CF_SR_GMPEID]);}
    // System register group 10
    SRegFXSR*    SrFXSR    () const { return static_cast<SRegFXSR*>    (m_srf[CF_SR_FXSR]);  }
    SRegFXST*    SrFXST    () const { return static_cast<SRegFXST*>    (m_srf[CF_SR_FXST]);  }
    SRegFXINFO*  SrFXINFO  () const { return static_cast<SRegFXINFO*>  (m_srf[CF_SR_FXINFO]);}
    SRegFXCFG*   SrFXCFG   () const { return static_cast<SRegFXCFG*>   (m_srf[CF_SR_FXCFG]); }
    SRegFXXC*    SrFXXC    () const { return static_cast<SRegFXXC*>    (m_srf[CF_SR_FXXC]);  }
    SRegFXXP*    SrFXXP    () const { return static_cast<SRegFXXP*>    (m_srf[CF_SR_FXXP]);  }
    // System register group 11
    SRegTSCOUNTL* SrTSCOUNTL() const { return static_cast<SRegTSCOUNTL*>(m_srf[CF_SR_TSCOUNTL]);}
    SRegTSCOUNTH* SrTSCOUNTH() const { return static_cast<SRegTSCOUNTH*>(m_srf[CF_SR_TSCOUNTH]);}
    SRegTSCTRL*  SrTSCTRL  () const { return static_cast<SRegTSCTRL*>  (m_srf[CF_SR_TSCTRL]);}
    SRegPMUMCTRL* SrPMUMCTRL() const { return static_cast<SRegPMUMCTRL*>(m_srf[CF_SR_PMUMCTRL]);}
    SRegPMGMCTRL* SrPMGMCTRL() const { return static_cast<SRegPMGMCTRL*>(m_srf[CF_SR_PMGMCTRL]);}
    SRegPMCOUNT* SrPMCOUNT0_G4X1X() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT0_G4X1X]);}
    SRegPMCTRL*  SrPMCTRL0_G4X1X() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL0_G4X1X]);}
    SRegPMCOUNT* SrPMCOUNT1_G4X1X() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT1_G4X1X]);}
    SRegPMCTRL*  SrPMCTRL1_G4X1X() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL1_G4X1X]);}
    SRegPMCOUNT* SrPMCOUNT2_G4X1X() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT2_G4X1X]);}
    SRegPMCTRL*  SrPMCTRL2_G4X1X() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL2_G4X1X]);}
    SRegPMCOUNT* SrPMCOUNT3_G4X1X() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT3_G4X1X]);}
    SRegPMCTRL*  SrPMCTRL3_G4X1X() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL3_G4X1X]);}
    // System register group 12
    SRegLSTEST0* SrLSTEST0 () const { return static_cast<SRegLSTEST0*> (m_srf[CF_SR_LSTEST0]);}
    SRegLSTEST1* SrLSTEST1 () const { return static_cast<SRegLSTEST1*> (m_srf[CF_SR_LSTEST1]);}
    SRegLSCFG*   SrLSCFG   () const { return static_cast<SRegLSCFG*>   (m_srf[CF_SR_LSCFG]); }
    SRegICBKEY*  SrICBKEY  () const { return static_cast<SRegICBKEY*>  (m_srf[CF_SR_ICBKEY]);}
    SRegIFCR*    SrIFCR    () const { return static_cast<SRegIFCR*>    (m_srf[CF_SR_IFCR]);  }
    SRegIFCR1*   SrIFCR1   () const { return static_cast<SRegIFCR1*>   (m_srf[CF_SR_IFCR1]); }
    SRegBRPCTRL0* SrBRPCTRL0() const { return static_cast<SRegBRPCTRL0*>(m_srf[CF_SR_BRPCTRL0]);}
    SRegBRPCTRL1* SrBRPCTRL1() const { return static_cast<SRegBRPCTRL1*>(m_srf[CF_SR_BRPCTRL1]);}
    SRegBRPCFG*  SrBRPCFG  () const { return static_cast<SRegBRPCFG*>  (m_srf[CF_SR_BRPCFG]);}
    SRegBRPACTRL* SrBRPACTRL() const { return static_cast<SRegBRPACTRL*>(m_srf[CF_SR_BRPACTRL]);}
    SRegBRPDATA* SrBRPDATA () const { return static_cast<SRegBRPDATA*> (m_srf[CF_SR_BRPDATA]);}
    // System register group 13
    SRegDCBKEY*  SrDCBKEY  () const { return static_cast<SRegDCBKEY*>  (m_srf[CF_SR_DCBKEY]);}
    SRegLSUCR*   SrLSUCR   () const { return static_cast<SRegLSUCR*>   (m_srf[CF_SR_LSUCR]); }
    SRegLSULNK0* SrLSULNK0 () const { return static_cast<SRegLSULNK0*> (m_srf[CF_SR_LSULNK0]);}
    SRegLSULNK1* SrLSULNK1 () const { return static_cast<SRegLSULNK1*> (m_srf[CF_SR_LSULNK1]);}
    SRegL1RLCR*  SrL1RLCR  () const { return static_cast<SRegL1RLCR*>  (m_srf[CF_SR_L1RLCR]);}
    SRegL1RLNK0* SrL1RLNK0 () const { return static_cast<SRegL1RLNK0*> (m_srf[CF_SR_L1RLNK0]);}
    SRegL1RLNK1* SrL1RLNK1 () const { return static_cast<SRegL1RLNK1*> (m_srf[CF_SR_L1RLNK1]);}
    SRegL1RCFG*  SrL1RCFG  () const { return static_cast<SRegL1RCFG*>  (m_srf[CF_SR_L1RCFG]);}
    SRegDECFG*   SrDECFG   () const { return static_cast<SRegDECFG*>   (m_srf[CF_SR_DECFG]); }
    SRegDECTRL*  SrDECTRL  () const { return static_cast<SRegDECTRL*>  (m_srf[CF_SR_DECTRL]);}
    SRegDEVDS*   SrDEVDS   () const { return static_cast<SRegDEVDS*>   (m_srf[CF_SR_DEVDS]); }
    SRegRDBCR*   SrRDBCR   () const { return static_cast<SRegRDBCR*>   (m_srf[CF_SR_RDBCR]); }
    SRegRDBACR*  SrRDBACR  () const { return static_cast<SRegRDBACR*>  (m_srf[CF_SR_RDBACR]);}
    SRegRDBATAG* SrRDBATAG () const { return static_cast<SRegRDBATAG*> (m_srf[CF_SR_RDBATAG]);}
    SRegRDBADAT0* SrRDBADAT0() const { return static_cast<SRegRDBADAT0*>(m_srf[CF_SR_RDBADAT0]);}
    SRegRDBADAT1* SrRDBADAT1() const { return static_cast<SRegRDBADAT1*>(m_srf[CF_SR_RDBADAT1]);}
    SRegRDBADAT2* SrRDBADAT2() const { return static_cast<SRegRDBADAT2*>(m_srf[CF_SR_RDBADAT2]);}
    SRegRDBADAT3* SrRDBADAT3() const { return static_cast<SRegRDBADAT3*>(m_srf[CF_SR_RDBADAT3]);}
    SRegRDBSTAT* SrRDBSTAT () const { return static_cast<SRegRDBSTAT*> (m_srf[CF_SR_RDBSTAT]);}
    // System register group 14
    SRegPMCTRL*  SrPMCTRL0_G4MH20() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL0_G4MH20]);}
    SRegPMCTRL*  SrPMCTRL1_G4MH20() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL1_G4MH20]);}
    SRegPMCTRL*  SrPMCTRL2_G4MH20() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL2_G4MH20]);}
    SRegPMCTRL*  SrPMCTRL3_G4MH20() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL3_G4MH20]);}
    SRegPMCTRL*  SrPMCTRL4_G4MH20() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL4_G4MH20]);}
    SRegPMCTRL*  SrPMCTRL5_G4MH20() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL5_G4MH20]);}
    SRegPMCTRL*  SrPMCTRL6_G4MH20() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL6_G4MH20]);}
    SRegPMCTRL*  SrPMCTRL7_G4MH20() const { return static_cast<SRegPMCTRL*>  (m_srf[CF_SR_PMCTRL7_G4MH20]);}
    SRegPMCOUNT* SrPMCOUNT0_G4MH20() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT0_G4MH20]);}
    SRegPMCOUNT* SrPMCOUNT1_G4MH20() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT1_G4MH20]);}
    SRegPMCOUNT* SrPMCOUNT2_G4MH20() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT2_G4MH20]);}
    SRegPMCOUNT* SrPMCOUNT3_G4MH20() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT3_G4MH20]);}
    SRegPMCOUNT* SrPMCOUNT4_G4MH20() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT4_G4MH20]);}
    SRegPMCOUNT* SrPMCOUNT5_G4MH20() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT5_G4MH20]);}
    SRegPMCOUNT* SrPMCOUNT6_G4MH20() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT6_G4MH20]);}
    SRegPMCOUNT* SrPMCOUNT7_G4MH20() const { return static_cast<SRegPMCOUNT*> (m_srf[CF_SR_PMCOUNT7_G4MH20]);}
    // System register group 15
    SRegPMSUBCND* SrPMSUBCND0() const { return static_cast<SRegPMSUBCND*>(m_srf[CF_SR_PMSUBCND0]);}
    SRegPMSUBCND* SrPMSUBCND1() const { return static_cast<SRegPMSUBCND*>(m_srf[CF_SR_PMSUBCND1]);}
    SRegPMSUBCND* SrPMSUBCND2() const { return static_cast<SRegPMSUBCND*>(m_srf[CF_SR_PMSUBCND2]);}
    SRegPMSUBCND* SrPMSUBCND3() const { return static_cast<SRegPMSUBCND*>(m_srf[CF_SR_PMSUBCND3]);}
    SRegPMSUBCND* SrPMSUBCND4() const { return static_cast<SRegPMSUBCND*>(m_srf[CF_SR_PMSUBCND4]);}
    SRegPMSUBCND* SrPMSUBCND5() const { return static_cast<SRegPMSUBCND*>(m_srf[CF_SR_PMSUBCND5]);}
    SRegPMSUBCND* SrPMSUBCND6() const { return static_cast<SRegPMSUBCND*>(m_srf[CF_SR_PMSUBCND6]);}
    SRegPMSUBCND* SrPMSUBCND7() const { return static_cast<SRegPMSUBCND*>(m_srf[CF_SR_PMSUBCND7]);}
    // System register group 32
    SRegBPC_N*   SrBPC0    () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC0]);  }
    SRegBPC_N*   SrBPC1    () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC1]);  }
    SRegBPC_N*   SrBPC2    () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC2]);  }
    SRegBPC_N*   SrBPC3    () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC3]);  }
    SRegBPC_N*   SrBPC4    () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC4]);  }
    SRegBPC_N*   SrBPC5    () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC5]);  }
    SRegBPC_N*   SrBPC6    () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC6]);  }
    SRegBPC_N*   SrBPC7    () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC7]);  }
    SRegBPC_N*   SrBPC8    () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC8]);  }
    SRegBPC_N*   SrBPC9    () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC9]);  }
    SRegBPC_N*   SrBPC10   () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC10]); }
    SRegBPC_N*   SrBPC11   () const { return static_cast<SRegBPC_N*>   (m_srf[CF_SR_BPC11]); }
    // System register group 33
    SRegBPAV_N*  SrBPAV0   () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV0]); }
    SRegBPAV_N*  SrBPAV1   () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV1]); }
    SRegBPAV_N*  SrBPAV2   () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV2]); }
    SRegBPAV_N*  SrBPAV3   () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV3]); }
    SRegBPAV_N*  SrBPAV4   () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV4]); }
    SRegBPAV_N*  SrBPAV5   () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV5]); }
    SRegBPAV_N*  SrBPAV6   () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV6]); }
    SRegBPAV_N*  SrBPAV7   () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV7]); }
    SRegBPAV_N*  SrBPAV8   () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV8]); }
    SRegBPAV_N*  SrBPAV9   () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV9]); }
    SRegBPAV_N*  SrBPAV10  () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV10]);}
    SRegBPAV_N*  SrBPAV11  () const { return static_cast<SRegBPAV_N*>  (m_srf[CF_SR_BPAV11]);}
    // System register group 34
    SRegBPAM_N*  SrBPAM0   () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM0]); }
    SRegBPAM_N*  SrBPAM1   () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM1]); }
    SRegBPAM_N*  SrBPAM2   () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM2]); }
    SRegBPAM_N*  SrBPAM3   () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM3]); }
    SRegBPAM_N*  SrBPAM4   () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM4]); }
    SRegBPAM_N*  SrBPAM5   () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM5]); }
    SRegBPAM_N*  SrBPAM6   () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM6]); }
    SRegBPAM_N*  SrBPAM7   () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM7]); }
    SRegBPAM_N*  SrBPAM8   () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM8]); }
    SRegBPAM_N*  SrBPAM9   () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM9]); }
    SRegBPAM_N*  SrBPAM10  () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM10]);}
    SRegBPAM_N*  SrBPAM11  () const { return static_cast<SRegBPAM_N*>  (m_srf[CF_SR_BPAM11]);}
    // System register group 37
    SRegMPLA_N*  SrMPLA0   () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA0]); }
    SRegMPLA_N*  SrMPLA1   () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA1]); }
    SRegMPLA_N*  SrMPLA2   () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA2]); }
    SRegMPLA_N*  SrMPLA3   () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA3]); }
    SRegMPLA_N*  SrMPLA4   () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA4]); }
    SRegMPLA_N*  SrMPLA5   () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA5]); }
    SRegMPLA_N*  SrMPLA6   () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA6]); }
    SRegMPLA_N*  SrMPLA7   () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA7]); }
    SRegMPLA_N*  SrMPLA8   () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA8]); }
    SRegMPLA_N*  SrMPLA9   () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA9]); }
    SRegMPLA_N*  SrMPLA10  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA10]);}
    SRegMPLA_N*  SrMPLA11  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA11]);}
    SRegMPLA_N*  SrMPLA12  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA12]);}
    SRegMPLA_N*  SrMPLA13  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA13]);}
    SRegMPLA_N*  SrMPLA14  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA14]);}
    SRegMPLA_N*  SrMPLA15  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA15]);}
    SRegMPLA_N*  SrMPLA16  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA16]);}
    SRegMPLA_N*  SrMPLA17  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA17]);}
    SRegMPLA_N*  SrMPLA18  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA18]);}
    SRegMPLA_N*  SrMPLA19  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA19]);}
    SRegMPLA_N*  SrMPLA20  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA20]);}
    SRegMPLA_N*  SrMPLA21  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA21]);}
    SRegMPLA_N*  SrMPLA22  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA22]);}
    SRegMPLA_N*  SrMPLA23  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA23]);}
    SRegMPLA_N*  SrMPLA24  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA24]);}
    SRegMPLA_N*  SrMPLA25  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA25]);}
    SRegMPLA_N*  SrMPLA26  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA26]);}
    SRegMPLA_N*  SrMPLA27  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA27]);}
    SRegMPLA_N*  SrMPLA28  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA28]);}
    SRegMPLA_N*  SrMPLA29  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA29]);}
    SRegMPLA_N*  SrMPLA30  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA30]);}
    SRegMPLA_N*  SrMPLA31  () const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA31]);}
    // System register group 38
    SRegMPUA_N*  SrMPUA0   () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA0]); }
    SRegMPUA_N*  SrMPUA1   () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA1]); }
    SRegMPUA_N*  SrMPUA2   () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA2]); }
    SRegMPUA_N*  SrMPUA3   () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA3]); }
    SRegMPUA_N*  SrMPUA4   () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA4]); }
    SRegMPUA_N*  SrMPUA5   () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA5]); }
    SRegMPUA_N*  SrMPUA6   () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA6]); }
    SRegMPUA_N*  SrMPUA7   () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA7]); }
    SRegMPUA_N*  SrMPUA8   () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA8]); }
    SRegMPUA_N*  SrMPUA9   () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA9]); }
    SRegMPUA_N*  SrMPUA10  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA10]);}
    SRegMPUA_N*  SrMPUA11  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA11]);}
    SRegMPUA_N*  SrMPUA12  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA12]);}
    SRegMPUA_N*  SrMPUA13  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA13]);}
    SRegMPUA_N*  SrMPUA14  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA14]);}
    SRegMPUA_N*  SrMPUA15  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA15]);}
    SRegMPUA_N*  SrMPUA16  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA16]);}
    SRegMPUA_N*  SrMPUA17  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA17]);}
    SRegMPUA_N*  SrMPUA18  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA18]);}
    SRegMPUA_N*  SrMPUA19  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA19]);}
    SRegMPUA_N*  SrMPUA20  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA20]);}
    SRegMPUA_N*  SrMPUA21  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA21]);}
    SRegMPUA_N*  SrMPUA22  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA22]);}
    SRegMPUA_N*  SrMPUA23  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA23]);}
    SRegMPUA_N*  SrMPUA24  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA24]);}
    SRegMPUA_N*  SrMPUA25  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA25]);}
    SRegMPUA_N*  SrMPUA26  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA26]);}
    SRegMPUA_N*  SrMPUA27  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA27]);}
    SRegMPUA_N*  SrMPUA28  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA28]);}
    SRegMPUA_N*  SrMPUA29  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA29]);}
    SRegMPUA_N*  SrMPUA30  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA30]);}
    SRegMPUA_N*  SrMPUA31  () const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA31]);}
    // System register group 39
    SRegMPAT_N*  SrMPAT0   () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT0]); }
    SRegMPAT_N*  SrMPAT1   () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT1]); }
    SRegMPAT_N*  SrMPAT2   () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT2]); }
    SRegMPAT_N*  SrMPAT3   () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT3]); }
    SRegMPAT_N*  SrMPAT4   () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT4]); }
    SRegMPAT_N*  SrMPAT5   () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT5]); }
    SRegMPAT_N*  SrMPAT6   () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT6]); }
    SRegMPAT_N*  SrMPAT7   () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT7]); }
    SRegMPAT_N*  SrMPAT8   () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT8]); }
    SRegMPAT_N*  SrMPAT9   () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT9]); }
    SRegMPAT_N*  SrMPAT10  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT10]);}
    SRegMPAT_N*  SrMPAT11  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT11]);}
    SRegMPAT_N*  SrMPAT12  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT12]);}
    SRegMPAT_N*  SrMPAT13  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT13]);}
    SRegMPAT_N*  SrMPAT14  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT14]);}
    SRegMPAT_N*  SrMPAT15  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT15]);}
    SRegMPAT_N*  SrMPAT16  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT16]);}
    SRegMPAT_N*  SrMPAT17  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT17]);}
    SRegMPAT_N*  SrMPAT18  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT18]);}
    SRegMPAT_N*  SrMPAT19  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT19]);}
    SRegMPAT_N*  SrMPAT20  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT20]);}
    SRegMPAT_N*  SrMPAT21  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT21]);}
    SRegMPAT_N*  SrMPAT22  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT22]);}
    SRegMPAT_N*  SrMPAT23  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT23]);}
    SRegMPAT_N*  SrMPAT24  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT24]);}
    SRegMPAT_N*  SrMPAT25  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT25]);}
    SRegMPAT_N*  SrMPAT26  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT26]);}
    SRegMPAT_N*  SrMPAT27  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT27]);}
    SRegMPAT_N*  SrMPAT28  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT28]);}
    SRegMPAT_N*  SrMPAT29  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT29]);}
    SRegMPAT_N*  SrMPAT30  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT30]);}
    SRegMPAT_N*  SrMPAT31  () const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT31]);}
    // System register group 40
    SRegMPLA_N*  SrMPLA0_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA0_B1]);}
    SRegMPLA_N*  SrMPLA1_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA1_B1]);}
    SRegMPLA_N*  SrMPLA2_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA2_B1]);}
    SRegMPLA_N*  SrMPLA3_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA3_B1]);}
    SRegMPLA_N*  SrMPLA4_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA4_B1]);}
    SRegMPLA_N*  SrMPLA5_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA5_B1]);}
    SRegMPLA_N*  SrMPLA6_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA6_B1]);}
    SRegMPLA_N*  SrMPLA7_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA7_B1]);}
    SRegMPLA_N*  SrMPLA8_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA8_B1]);}
    SRegMPLA_N*  SrMPLA9_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA9_B1]);}
    SRegMPLA_N*  SrMPLA10_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA10_B1]);}
    SRegMPLA_N*  SrMPLA11_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA11_B1]);}
    SRegMPLA_N*  SrMPLA12_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA12_B1]);}
    SRegMPLA_N*  SrMPLA13_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA13_B1]);}
    SRegMPLA_N*  SrMPLA14_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA14_B1]);}
    SRegMPLA_N*  SrMPLA15_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA15_B1]);}
    SRegMPLA_N*  SrMPLA16_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA16_B1]);}
    SRegMPLA_N*  SrMPLA17_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA17_B1]);}
    SRegMPLA_N*  SrMPLA18_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA18_B1]);}
    SRegMPLA_N*  SrMPLA19_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA19_B1]);}
    SRegMPLA_N*  SrMPLA20_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA20_B1]);}
    SRegMPLA_N*  SrMPLA21_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA21_B1]);}
    SRegMPLA_N*  SrMPLA22_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA22_B1]);}
    SRegMPLA_N*  SrMPLA23_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA23_B1]);}
    SRegMPLA_N*  SrMPLA24_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA24_B1]);}
    SRegMPLA_N*  SrMPLA25_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA25_B1]);}
    SRegMPLA_N*  SrMPLA26_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA26_B1]);}
    SRegMPLA_N*  SrMPLA27_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA27_B1]);}
    SRegMPLA_N*  SrMPLA28_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA28_B1]);}
    SRegMPLA_N*  SrMPLA29_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA29_B1]);}
    SRegMPLA_N*  SrMPLA30_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA30_B1]);}
    SRegMPLA_N*  SrMPLA31_B1() const { return static_cast<SRegMPLA_N*>  (m_srf[CF_SR_MPLA31_B1]);}
    // System register group 41
    SRegMPUA_N*  SrMPUA0_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA0_B1]);}
    SRegMPUA_N*  SrMPUA1_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA1_B1]);}
    SRegMPUA_N*  SrMPUA2_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA2_B1]);}
    SRegMPUA_N*  SrMPUA3_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA3_B1]);}
    SRegMPUA_N*  SrMPUA4_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA4_B1]);}
    SRegMPUA_N*  SrMPUA5_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA5_B1]);}
    SRegMPUA_N*  SrMPUA6_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA6_B1]);}
    SRegMPUA_N*  SrMPUA7_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA7_B1]);}
    SRegMPUA_N*  SrMPUA8_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA8_B1]);}
    SRegMPUA_N*  SrMPUA9_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA9_B1]);}
    SRegMPUA_N*  SrMPUA10_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA10_B1]);}
    SRegMPUA_N*  SrMPUA11_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA11_B1]);}
    SRegMPUA_N*  SrMPUA12_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA12_B1]);}
    SRegMPUA_N*  SrMPUA13_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA13_B1]);}
    SRegMPUA_N*  SrMPUA14_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA14_B1]);}
    SRegMPUA_N*  SrMPUA15_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA15_B1]);}
    SRegMPUA_N*  SrMPUA16_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA16_B1]);}
    SRegMPUA_N*  SrMPUA17_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA17_B1]);}
    SRegMPUA_N*  SrMPUA18_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA18_B1]);}
    SRegMPUA_N*  SrMPUA19_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA19_B1]);}
    SRegMPUA_N*  SrMPUA20_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA20_B1]);}
    SRegMPUA_N*  SrMPUA21_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA21_B1]);}
    SRegMPUA_N*  SrMPUA22_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA22_B1]);}
    SRegMPUA_N*  SrMPUA23_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA23_B1]);}
    SRegMPUA_N*  SrMPUA24_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA24_B1]);}
    SRegMPUA_N*  SrMPUA25_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA25_B1]);}
    SRegMPUA_N*  SrMPUA26_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA26_B1]);}
    SRegMPUA_N*  SrMPUA27_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA27_B1]);}
    SRegMPUA_N*  SrMPUA28_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA28_B1]);}
    SRegMPUA_N*  SrMPUA29_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA29_B1]);}
    SRegMPUA_N*  SrMPUA30_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA30_B1]);}
    SRegMPUA_N*  SrMPUA31_B1() const { return static_cast<SRegMPUA_N*>  (m_srf[CF_SR_MPUA31_B1]);}
    // System register group 42
    SRegMPAT_N*  SrMPAT0_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT0_B1]);}
    SRegMPAT_N*  SrMPAT1_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT1_B1]);}
    SRegMPAT_N*  SrMPAT2_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT2_B1]);}
    SRegMPAT_N*  SrMPAT3_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT3_B1]);}
    SRegMPAT_N*  SrMPAT4_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT4_B1]);}
    SRegMPAT_N*  SrMPAT5_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT5_B1]);}
    SRegMPAT_N*  SrMPAT6_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT6_B1]);}
    SRegMPAT_N*  SrMPAT7_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT7_B1]);}
    SRegMPAT_N*  SrMPAT8_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT8_B1]);}
    SRegMPAT_N*  SrMPAT9_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT9_B1]);}
    SRegMPAT_N*  SrMPAT10_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT10_B1]);}
    SRegMPAT_N*  SrMPAT11_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT11_B1]);}
    SRegMPAT_N*  SrMPAT12_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT12_B1]);}
    SRegMPAT_N*  SrMPAT13_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT13_B1]);}
    SRegMPAT_N*  SrMPAT14_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT14_B1]);}
    SRegMPAT_N*  SrMPAT15_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT15_B1]);}
    SRegMPAT_N*  SrMPAT16_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT16_B1]);}
    SRegMPAT_N*  SrMPAT17_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT17_B1]);}
    SRegMPAT_N*  SrMPAT18_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT18_B1]);}
    SRegMPAT_N*  SrMPAT19_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT19_B1]);}
    SRegMPAT_N*  SrMPAT20_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT20_B1]);}
    SRegMPAT_N*  SrMPAT21_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT21_B1]);}
    SRegMPAT_N*  SrMPAT22_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT22_B1]);}
    SRegMPAT_N*  SrMPAT23_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT23_B1]);}
    SRegMPAT_N*  SrMPAT24_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT24_B1]);}
    SRegMPAT_N*  SrMPAT25_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT25_B1]);}
    SRegMPAT_N*  SrMPAT26_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT26_B1]);}
    SRegMPAT_N*  SrMPAT27_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT27_B1]);}
    SRegMPAT_N*  SrMPAT28_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT28_B1]);}
    SRegMPAT_N*  SrMPAT29_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT29_B1]);}
    SRegMPAT_N*  SrMPAT30_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT30_B1]);}
    SRegMPAT_N*  SrMPAT31_B1() const { return static_cast<SRegMPAT_N*>  (m_srf[CF_SR_MPAT31_B1]);}
};
