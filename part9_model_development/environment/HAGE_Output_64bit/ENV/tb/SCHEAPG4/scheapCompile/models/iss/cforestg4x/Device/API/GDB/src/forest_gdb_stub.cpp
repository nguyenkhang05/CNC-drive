/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <vector>

#ifdef MSVC
#else // MSVC
#include <unistd.h>
#endif // MSVC

#include "./config.h"
#include "./regid.h"
#include "./forest_utility.h"
#include "./forest_gdb_stub.h"
#include "./forest_cmdopt.h"
#include "./forest_message.h"


//#define DEBUG_GDB
#define FORCE_VCONT_SUPPORT_SC

const CReg GdbStub::reglist[N_GDB_REG] = {
    R0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15,
    R16,
    R17,
    R18,
    R19,
    R20,
    R21,
    R22,
    R23,
    R24,
    R25,
    R26,
    R27,
    R28,
    R29,
    R30,
    R31,
    CF_SR_VEIPC,
    CF_SR_VEIPSW,
    CF_SR_VFEPC,
    CF_SR_VFEPSW,
    GR_NONE,
    CF_SR_VPSW,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_VEIIC,
    CF_SR_VFEIC,
    CF_SR_PSWH,
    CF_SR_CTPC,
    CF_SR_CTPSW,
    CF_SR_EIPSWH,
    CF_SR_FEPSWH,
    CF_SR_CTBP,
    CF_SR_SNZCFG,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_VEIWR,
    CF_SR_VFEWR,
    GR_NONE,
    GR_NONE,
    PC,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_VMPM,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_FPSR,
    CF_SR_FPEPC,
    CF_SR_FPST,
    CF_SR_FPCC,
    CF_SR_FPCFG,
    GR_NONE, // CF_SR_FPEC,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_VSPID,
    CF_SR_VSPIDLIST,
    CF_SR_RBASE,
    CF_SR_VEBASE,
    CF_SR_VINTBP,
    CF_SR_MCTL,
    CF_SR_PID,
    GR_NONE,
    CF_SR_VSVLOCK,
    GR_NONE,
    GR_NONE,
    CF_SR_SCCFG,
    CF_SR_SCBP,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_HVCFG,
    CF_SR_GMCFG,
    GR_NONE,
    GR_NONE,
    CF_SR_HVSB,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_VPEID,
    CF_SR_BMID,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_VMEA,
    GR_NONE,
    CF_SR_VMEI,
    GR_NONE,
    CF_SR_ISPR,
    CF_SR_IMSR,
    CF_SR_ICSR,
    CF_SR_VINTCFG,
    CF_SR_VPLMR,
    CF_SR_RBCR0,
    CF_SR_RBCR1,
    CF_SR_RBNR,
    CF_SR_RBIP,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE, // CF_SR_DBGEN,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE, // CF_SR_DBPSWH,
    GR_NONE,
    GR_NONE, // CF_SR_DBIC,
    GR_NONE,
    GR_NONE,
    GR_NONE, // CF_SR_DBPC,
    GR_NONE, // CF_SR_DBPSW,
    GR_NONE, // CF_SR_DIR0,
    GR_NONE, // CF_SR_DIR1,
    GR_NONE, // CF_SR_BPC,
    GR_NONE,
    GR_NONE, // CF_SR_BPAV,
    GR_NONE, // CF_SR_BPAM,
    GR_NONE,
    GR_NONE,
    GR_NONE, // CF_SR_DBCFG
    GR_NONE,
    GR_NONE, // CF_SR_DBWR,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_ICTAGL,
    CF_SR_ICTAGH,
    CF_SR_ICDATL,
    CF_SR_ICDATH,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_ICCTRL,
    GR_NONE,
    CF_SR_ICCFG,
    GR_NONE,
    CF_SR_ICERR,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_VMPM,
    GR_NONE,
    CF_SR_MPCFG,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_MCA,
    CF_SR_MCS,
    CF_SR_MCC,
    CF_SR_MCR,
    CF_SR_MCI,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_MPIDX,
    CF_SR_MPBK,
    GR_NONE,
    GR_NONE,
    CF_SR_MPLA,
    CF_SR_MPUA,
    CF_SR_MPAT,
    GR_NONE,
    CF_SR_MPID0,
    CF_SR_MPID1,
    CF_SR_MPID2,
    CF_SR_MPID3,
    CF_SR_MPID4,
    CF_SR_MPID5,
    CF_SR_MPID6,
    CF_SR_MPID7,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE, // VR0,
    GR_NONE, // VR1,
    GR_NONE, // VR2,
    GR_NONE, // VR3,
    GR_NONE, // VR4,
    GR_NONE, // VR5,
    GR_NONE, // VR6,
    GR_NONE, // VR7,
    GR_NONE, // VR8,
    GR_NONE, // VR9,
    GR_NONE, // VR10,
    GR_NONE, // VR11,
    GR_NONE, // VR12,
    GR_NONE, // VR13,
    GR_NONE, // VR14,
    GR_NONE, // VR15,
    GR_NONE, // VR16,
    GR_NONE, // VR17,
    GR_NONE, // VR18,
    GR_NONE, // VR19,
    GR_NONE, // VR20,
    GR_NONE, // VR21,
    GR_NONE, // VR22,
    GR_NONE, // VR23,
    GR_NONE, // VR24,
    GR_NONE, // VR25,
    GR_NONE, // VR26,
    GR_NONE, // VR27,
    GR_NONE, // VR28,
    GR_NONE, // VR29,
    GR_NONE, // VR30,
    GR_NONE, // VR31,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_GMEIPC,
    CF_SR_GMEIPSW,
    CF_SR_GMFEPC,
    CF_SR_GMFEPSW,
    GR_NONE,
    CF_SR_GMPSW,
    CF_SR_GMMEA,
    GR_NONE,
    CF_SR_GMMEI,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_GMEIIC,
    CF_SR_GMFEIC,
    GR_NONE,
    CF_SR_GMSPID,
    CF_SR_GMSPIDLIST,
    GR_NONE,
    CF_SR_GMEBASE,
    CF_SR_GMINTBP,
    CF_SR_GMINTCFG,
    CF_SR_GMPLMR,
    GR_NONE,
    CF_SR_GMSVLOCK,
    CF_SR_GMMPM,
    GR_NONE,
    GR_NONE,
    CF_SR_GMEIWR,
    CF_SR_GMFEWR,
    CF_SR_GMPEID,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_FXSR,
    GR_NONE,
    CF_SR_FXST,
    CF_SR_FXINFO,
    CF_SR_FXCFG,
    GR_NONE,
    CF_SR_FXXC,
    CF_SR_FXXP,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_TSCOUNTL,
    CF_SR_TSCOUNTH,
    CF_SR_TSCTRL,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_PMUMCTRL,
    CF_SR_PMGMCTRL,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_PMCOUNT0_G4X1X,
    CF_SR_PMCTRL0_G4X1X,
    CF_SR_PMCOUNT1_G4X1X,
    CF_SR_PMCTRL1_G4X1X,
    CF_SR_PMCOUNT2_G4X1X,
    CF_SR_PMCTRL2_G4X1X,
    CF_SR_PMCOUNT3_G4X1X,
    CF_SR_PMCTRL3_G4X1X,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_LSTEST0,
    CF_SR_LSTEST1,
    CF_SR_LSCFG,
    GR_NONE,
    GR_NONE, // CF_SR_ICBKEY,
    CF_SR_IFCR,
    GR_NONE, // CF_SR_IFCR1,
    GR_NONE,
    CF_SR_BRPCTRL0,
    GR_NONE, // CF_SR_BRPCTRL1,
    GR_NONE, // CF_SR_BRPCFG,
    GR_NONE, // CF_SR_BRPACTRL,
    GR_NONE, // CF_SR_BRPDATA,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE, // CF_SR_DCBKEY,
    GR_NONE, // CF_SR_LSUCR,
    GR_NONE, // CF_SR_LSULNK0,
    GR_NONE, // CF_SR_LSULNK1,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE, // CF_SR_L1RLCR,
    GR_NONE, // CF_SR_L1RLNK0,
    GR_NONE, // CF_SR_L1RLNK1,
    GR_NONE,
    GR_NONE, // CF_SR_L1RCFG
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_DECFG,
    CF_SR_DECTRL,
    CF_SR_DEVDS,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_RDBCR,
    GR_NONE, // CF_SR_RDBACR,
    GR_NONE, // CF_SR_RDBATAG,
    GR_NONE, // CF_SR_RDBADAT0,
    GR_NONE, // CF_SR_RDBADAT1,
    GR_NONE, // CF_SR_RDBADAT2,
    GR_NONE, // CF_SR_RDBADAT3,
    GR_NONE, // CF_SR_RDBSTAT,
    WR0,
    WR1,
    WR2,
    WR3,
    WR4,
    WR5,
    WR6,
    WR7,
    WR8,
    WR9,
    WR10,
    WR11,
    WR12,
    WR13,
    WR14,
    WR15,
    WR16,
    WR17,
    WR18,
    WR19,
    WR20,
    WR21,
    WR22,
    WR23,
    WR24,
    WR25,
    WR26,
    WR27,
    WR28,
    WR29,
    WR30,
    WR31,
    CF_SR_PMCTRL0_G4MH20,
    CF_SR_PMCTRL1_G4MH20,
    CF_SR_PMCTRL2_G4MH20,
    CF_SR_PMCTRL3_G4MH20,
    CF_SR_PMCTRL4_G4MH20,
    CF_SR_PMCTRL5_G4MH20,
    CF_SR_PMCTRL6_G4MH20,
    CF_SR_PMCTRL7_G4MH20,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_PMCOUNT0_G4MH20,
    CF_SR_PMCOUNT1_G4MH20,
    CF_SR_PMCOUNT2_G4MH20,
    CF_SR_PMCOUNT3_G4MH20,
    CF_SR_PMCOUNT4_G4MH20,
    CF_SR_PMCOUNT5_G4MH20,
    CF_SR_PMCOUNT6_G4MH20,
    CF_SR_PMCOUNT7_G4MH20,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    CF_SR_PMSUBCND0,
    CF_SR_PMSUBCND1,
    CF_SR_PMSUBCND2,
    CF_SR_PMSUBCND3,
    CF_SR_PMSUBCND4,
    CF_SR_PMSUBCND5,
    CF_SR_PMSUBCND6,
    CF_SR_PMSUBCND7,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE,
    GR_NONE, // NUM_REGS,
  };


const char GdbStub::regname[N_GDB_REG][LEN_REGNAME] = {
    "R0",
    "R1",
    "R2",
    "R3",
    "R4",
    "R5",
    "R6",
    "R7",
    "R8",
    "R9",
    "R10",
    "R11",
    "R12",
    "R13",
    "R14",
    "R15",
    "R16",
    "R17",
    "R18",
    "R19",
    "R20",
    "R21",
    "R22",
    "R23",
    "R24",
    "R25",
    "R26",
    "R27",
    "R28",
    "R29",
    "R30",
    "R31",
    "CF_SR_VEIPC",
    "CF_SR_VEIPSW",
    "CF_SR_VFEPC",
    "CF_SR_VFEPSW",
    "GR_NONE",
    "CF_SR_VPSW",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_VEIIC",
    "CF_SR_VFEIC",
    "CF_SR_PSWH",
    "CF_SR_CTPC",
    "CF_SR_CTPSW",
    "CF_SR_EIPSWH",
    "CF_SR_FEPSWH",
    "CF_SR_CTBP",
    "CF_SR_SNZCFG",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_VEIWR",
    "CF_SR_VFEWR",
    "GR_NONE",
    "GR_NONE",
    "PC",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_VMPM",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_FPSR",
    "CF_SR_FPEPC",
    "CF_SR_FPST",
    "CF_SR_FPCC",
    "CF_SR_FPCFG",
    "GR_NONE", // "CF_SR_FPEC",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_VSPID",
    "CF_SR_VSPIDLIST",
    "CF_SR_RBASE",
    "CF_SR_VEBASE",
    "CF_SR_VINTBP",
    "CF_SR_MCTL",
    "CF_SR_PID",
    "GR_NONE",
    "CF_SR_SVLOCK",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_SCCFG",
    "CF_SR_SCBP",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_HVCFG",
    "CF_SR_GMCFG",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_HVSB",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_VPEID",
    "CF_SR_BMID",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_MEA",
    "GR_NONE",
    "CF_SR_MEI",
    "GR_NONE",
    "CF_SR_ISPR",
    "CF_SR_IMSR",
    "CF_SR_ICSR",
    "CF_SR_VINTCFG",
    "CF_SR_VPLMR",
    "CF_SR_RBCR0",
    "CF_SR_RBCR1",
    "CF_SR_RBNR",
    "CF_SR_RBIP",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE", // "CF_SR_DBGEN",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE", // "CF_SR_DBPSWH",
    "GR_NONE",
    "GR_NONE", // "CF_SR_DBIC",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE", // "CF_SR_DBPC",
    "GR_NONE", // "CF_SR_DBPSW",
    "GR_NONE", // "CF_SR_DIR0",
    "GR_NONE", // "CF_SR_DIR1",
    "GR_NONE", // "CF_SR_BPC",
    "GR_NONE",
    "GR_NONE", // "CF_SR_BPAV",
    "GR_NONE", // "CF_SR_BPAM",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE", // "CF_SR_DBCFG",
    "GR_NONE",
    "GR_NONE", // "CF_SR_DBWR",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_ICTAGL",
    "CF_SR_ICTAGH",
    "CF_SR_ICDATL",
    "CF_SR_ICDATH",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_ICCTRL",
    "GR_NONE",
    "CF_SR_ICCFG",
    "GR_NONE",
    "CF_SR_ICERR",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_VMPM",
    "GR_NONE",
    "CF_SR_MPCFG",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_MCA",
    "CF_SR_MCS",
    "CF_SR_MCC",
    "CF_SR_MCR",
    "CF_SR_MCI",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_MPIDX",
    "CF_SR_MPBK",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_MPLA",
    "CF_SR_MPUA",
    "CF_SR_MPAT",
    "GR_NONE",
    "CF_SR_MPID0",
    "CF_SR_MPID1",
    "CF_SR_MPID2",
    "CF_SR_MPID3",
    "CF_SR_MPID4",
    "CF_SR_MPID5",
    "CF_SR_MPID6",
    "CF_SR_MPID7",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE", // "VR0",
    "GR_NONE", // "VR1",
    "GR_NONE", // "VR2",
    "GR_NONE", // "VR3",
    "GR_NONE", // "VR4",
    "GR_NONE", // "VR5",
    "GR_NONE", // "VR6",
    "GR_NONE", // "VR7",
    "GR_NONE", // "VR8",
    "GR_NONE", // "VR9",
    "GR_NONE", // "VR10",
    "GR_NONE", // "VR11",
    "GR_NONE", // "VR12",
    "GR_NONE", // "VR13",
    "GR_NONE", // "VR14",
    "GR_NONE", // "VR15",
    "GR_NONE", // "VR16",
    "GR_NONE", // "VR17",
    "GR_NONE", // "VR18",
    "GR_NONE", // "VR19",
    "GR_NONE", // "VR20",
    "GR_NONE", // "VR21",
    "GR_NONE", // "VR22",
    "GR_NONE", // "VR23",
    "GR_NONE", // "VR24",
    "GR_NONE", // "VR25",
    "GR_NONE", // "VR26",
    "GR_NONE", // "VR27",
    "GR_NONE", // "VR28",
    "GR_NONE", // "VR29",
    "GR_NONE", // "VR30",
    "GR_NONE", // "VR31",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_GMEIPC",
    "CF_SR_GMEIPSW",
    "CF_SR_GMFEPC",
    "CF_SR_GMFEPSW",
    "GR_NONE",
    "CF_SR_GMPSW",
    "CF_SR_GMMEA",
    "GR_NONE",
    "CF_SR_GMMEI",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_GMEIIC",
    "CF_SR_GMFEIC",
    "GR_NONE",
    "CF_SR_GMSPID",
    "CF_SR_GMSPIDLIST",
    "GR_NONE",
    "CF_SR_GMEBASE",
    "CF_SR_GMINTBP",
    "CF_SR_GMINTCFG",
    "CF_SR_GMPLMR",
    "GR_NONE",
    "CF_SR_GMSVLOCK",
    "CF_SR_GMMPM",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_GMEIWR",
    "CF_SR_GMFEWR",
    "CF_SR_GMPEID",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_FXSR",
    "GR_NONE",
    "CF_SR_FXST",
    "CF_SR_FXINFO",
    "CF_SR_FXCFG",
    "GR_NONE",
    "CF_SR_FXXC",
    "CF_SR_FXXP",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_TSCOUNTL",
    "CF_SR_TSCOUNTH",
    "CF_SR_TSCTRL",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_PMUMCTRL",
    "CF_SR_PMGMCTRL",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_PMCOUNT0",
    "CF_SR_PMCTRL0",
    "CF_SR_PMCOUNT1",
    "CF_SR_PMCTRL1",
    "CF_SR_PMCOUNT2",
    "CF_SR_PMCTRL2",
    "CF_SR_PMCOUNT3",
    "CF_SR_PMCTRL3",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_LSTEST0",
    "CF_SR_LSTEST1",
    "CF_SR_LSCFG",
    "GR_NONE",
    "GR_NONE", // "CF_SR_ICBKEY",
    "CF_SR_IFCR",
    "GR_NONE", // "CF_SR_IFCR1",
    "GR_NONE",
    "CF_SR_BRPCTRL0",
    "GR_NONE", // "CF_SR_BRPCTRL1",
    "GR_NONE", // "CF_SR_BRPCFG",
    "GR_NONE", // "CF_SR_BRPACTRL",
    "GR_NONE", // "CF_SR_BRPDATA",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE", // "CF_SR_DCBKEY",
    "GR_NONE", // "CF_SR_LSUCR",
    "GR_NONE", // "CF_SR_LSULNK0",
    "GR_NONE", // "CF_SR_LSULNK1",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE", // "CF_SR_L1RLCR",
    "GR_NONE", // "CF_SR_L1RLNK0",
    "GR_NONE", // "CF_SR_L1RLNK1",
    "GR_NONE",
    "GR_NONE", // "CF_SR_L1RCFG",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_DECFG",
    "CF_SR_DECTRL",
    "CF_SR_DEVDS",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_RDBCR",
    "GR_NONE", // "CF_SR_RDBACR",
    "GR_NONE", // "CF_SR_RDBATAG",
    "GR_NONE", // "CF_SR_RDBADAT0",
    "GR_NONE", // "CF_SR_RDBADAT1",
    "GR_NONE", // "CF_SR_RDBADAT2",
    "GR_NONE", // "CF_SR_RDBADAT3",
    "GR_NONE", // "CF_SR_RDBSTAT",
    "WR0",
    "WR1",
    "WR2",
    "WR3",
    "WR4",
    "WR5",
    "WR6",
    "WR7",
    "WR8",
    "WR9",
    "WR10",
    "WR11",
    "WR12",
    "WR13",
    "WR14",
    "WR15",
    "WR16",
    "WR17",
    "WR18",
    "WR19",
    "WR20",
    "WR21",
    "WR22",
    "WR23",
    "WR24",
    "WR25",
    "WR26",
    "WR27",
    "WR28",
    "WR29",
    "WR30",
    "WR31",
    "CF_SR_PMCTRL0",
    "CF_SR_PMCTRL1",
    "CF_SR_PMCTRL2",
    "CF_SR_PMCTRL3",
    "CF_SR_PMCTRL4",
    "CF_SR_PMCTRL5",
    "CF_SR_PMCTRL6",
    "CF_SR_PMCTRL7",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_PMCOUNT0",
    "CF_SR_PMCOUNT1",
    "CF_SR_PMCOUNT2",
    "CF_SR_PMCOUNT3",
    "CF_SR_PMCOUNT4",
    "CF_SR_PMCOUNT5",
    "CF_SR_PMCOUNT6",
    "CF_SR_PMCOUNT7",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "CF_SR_PMSUBCND0",
    "CF_SR_PMSUBCND1",
    "CF_SR_PMSUBCND2",
    "CF_SR_PMSUBCND3",
    "CF_SR_PMSUBCND4",
    "CF_SR_PMSUBCND5",
    "CF_SR_PMSUBCND6",
    "CF_SR_PMSUBCND7",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
    "GR_NONE",
  };

/* 現在のV850はLittleエンディアン固定 */
GdbStub::EndianType GdbStub::GetEndian (void) const
{
    return ENDIAN_LITTLE;
}


/* Htidの範囲チェック */
/* 有効なHTを示すHTIDならそのままのHTIDを返す */
/* 無効なHTID（GDB_HTID_ALL、GDB_HTID_ANYを含む）の場合は、 */
/*   入力 GDB_HTID_ANY : Anyなのでhtid=0を返す */
/*   入力 GDB_HTID_ANY以外 : ALLかINVALIDなのでエラー。GDB_HTID_INVALIDを返す */
GdbStub::HtIdType GdbStub::GdbCheckValidHtid(HtIdType htid) const
{
    if (htid == GDB_HTID_ANY) {
        return (GDB_HTID_INIT);
    } else if ( (htid == GDB_HTID_ALL)
             || (htid == GDB_HTID_INVALID)
             || (htid < 0)
             || (htid >= m_forest_util->GetScNum ()) ) {
        return (GDB_HTID_INVALID);
    } else {
        return(htid);
    }
}


GdbStub::ThreadIdType GdbStub::GdbGetThreadidFromHtid(HtIdType htid) const
{
    if (htid == GDB_HTID_ALL) {
        return (GDB_THREAD_ALL);
    } else if (htid == GDB_HTID_ANY) {
        return (GDB_THREAD_ANY);
    } else if ( (htid < 0)
             || (htid >= m_forest_util->GetScNum ()) ) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid HTID(%d) is designated>\n", htid);
        return(GDB_THREAD_INVALID); /* Error */
    } else {
        /* threadid0 は特殊なので、 htid+1 = threadid とする */
        return(htid + 1);
    }
}


GdbStub::HtIdType GdbStub::GdbGetHtidFromThreadid(ThreadIdType threadid) const
{
    if (threadid == GDB_THREAD_ALL) {
        return (GDB_HTID_ALL);
    } else if (threadid == GDB_THREAD_ANY ) {
        return (GDB_HTID_ANY);
    } else if ( (threadid < 0)
             || (threadid > m_forest_util->GetScNum ()) ){
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid ThreadID(%d) is designated>\n", threadid);
        return (GDB_THREAD_INVALID); /* Error */
    } else {
        /* threadid0 は特殊なので、 htid+1 = threadid とする */
        return (threadid - 1);
    }
}


int32_t GdbStub::CharToHex (char ch) const
{
    if (ch >= '0' && ch <= '9') {
        return (ch - '0');
    } else if (ch >= 'a' && ch <= 'f') {
        return (ch - 'a' + 10);
    } else if (ch >= 'A' && ch <= 'F') {
        return (ch - 'A' + 10);
    } else {
        return (-1);
    }
}


uint32_t GdbStub::ChangeEndianForGDB (int32_t byte_size, uint32_t data) const
{
    uint32_t ret;

    /* GDBは常にビッグエンディアンなので、CPUがリトルエンディアンなら変換 */
    if (GetEndian() == ENDIAN_BIG){
        ret = data;
    } else {
        // レジスタアクセス用に4byte/2byteだけ特別対応。
        // それ以外のデータサイズについては1byteずつread/writeするので変換不要。
        switch (byte_size) {
        case 4:
            ret = (((data >>  0) & 0xFF) << 24)
                | (((data >>  8) & 0xFF) << 16)
                | (((data >> 16) & 0xFF) <<  8)
                | (((data >> 24) & 0xFF) <<  0);
            break;
        case 2:
            ret = (((data >>  0) & 0xFF) << 8)
                | (((data >>  8) & 0xFF) << 0);
            break;
        case 1:
            ret = data;
            break;
        default:
            ret = data;
            printf ("Unsupported size\n"); // ここにはこないはず
        }
    }

    return(ret);
}


char GdbStub::GetGdbChar (int client_sockfd)
{
    size_t size;
    unsigned char ch;

    /* コマンドを受信 */
    size = recv (client_sockfd, &ch, sizeof (ch), 0);
    if (size == 0) {
        //perror ("Connection refused!");
        return ('\0');
    } else {
        return (ch);
    }
}


size_t GdbStub::GetGdbStr (int client_sockfd, size_t length, char* buff)
{
    ssize_t size;

    /* コマンドを受信 */
    size = recv (client_sockfd, buff, length, 0);
    if ((size == 0) || (size == -1)){
        perror ("Connection refused!");
        return (0);
    } else {
        return (size);
    }
}

void GdbStub::PutGdbChar (int client_sockfd, char ch)
{
    /* コマンドを送信 */
    send (client_sockfd, &ch, sizeof (ch), 0);
}


char* GdbStub::GetGdbPacket (int client_sockfd, char* cmd_buffer)
{
    int32_t count;
    unsigned int checksum;
    unsigned int xmitcsum;
    char ch;

    while ((ch = GetGdbChar (client_sockfd)) != '$') {
        /* '$'が出現するまで無視 */
        /* '\0' が来たらエラー */
        if (ch == '\0') {
            return NULL;
        }
    }

    /* 1byteずつrecvすると遅いので、一度にBUFMAXまで読み込んでから処理する */
    size_t size = GetGdbStr(client_sockfd, BUFMAX, cmd_buffer);
    if (size == 0){
        return NULL;
    }
    uint32_t inpos = 0;
_retry:
    count = 0;
    checksum = 0;
    while (inpos < BUFMAX) {
        ch = cmd_buffer[inpos++];
        if (ch == '$') { /* $ = 0x24 */
            /* リトライ */
            goto _retry;
        } else if (ch == '#') { /* # = 0x23 */
            /* パケット終了 */
            break;
        } else if (ch == 0x7d) { /* } = 0x7d */
            // 0x7dはエスケープ文字。
            // ただし、 GDB のエスケープは特殊で 0x20 と XOR する必要あり。
            // 0x7d の後の文字は 0x20 と XOR してからバッファに格納
            checksum += ch;
            ch = cmd_buffer[inpos++];
            checksum += ch;
            ch ^= 0x20;
        } else {
            checksum += ch;
        }
        cmd_buffer[count++] = ch;
    }

    if ((inpos + 2) < BUFMAX){
        xmitcsum = CharToHex(cmd_buffer[inpos++]) << 4;
        xmitcsum += CharToHex(cmd_buffer[inpos++]);
    } else {
        xmitcsum = checksum + 1;
    }
    cmd_buffer[count] = '\0';

    if ((checksum & 0xFF) != (xmitcsum & 0xFF)) {
        /* チェックサムミス */
        PutGdbChar (client_sockfd, '-');
        return (cmd_buffer);
    } else {
        /* 転送成功 */
        PutGdbChar (client_sockfd, '+');
        if (cmd_buffer[2] == ':') {
            /* シーケンスコマンドの場合、シーケンスIDを返信する */
            PutGdbChar (client_sockfd, cmd_buffer[0]);
            PutGdbChar (client_sockfd, cmd_buffer[1]);
            return (&cmd_buffer[3]);
        } else {
            return (cmd_buffer);
        }
    }
}


void GdbStub::PutGdbPacket (int client_sockfd, char* cmd_buffer)
{
    const char hexchars[] = "0123456789abcdef";
    int32_t count;
    unsigned int checksum;
    char ch;

    /* $<packet_info>#<checksum>. */
    do {
        count = 0;
        checksum = 0;

        PutGdbChar (client_sockfd, '$');
        while ((ch = cmd_buffer[count]) != '\0') {
            PutGdbChar (client_sockfd, ch);
            checksum += ch;
            count ++;
        }
        PutGdbChar (client_sockfd, '#');
        PutGdbChar (client_sockfd, hexchars[(checksum >> 4) & 0x0f]);
        PutGdbChar (client_sockfd, hexchars[checksum & 0x0f]);
    } while (GetGdbChar (client_sockfd) != '+');
}


int32_t GdbStub::HexToUintN (char** ptr, uint32_t* intValue, int32_t length) const
{
    int32_t hexValue;
    int32_t num_char = 0;

    *intValue = 0;

    while ( (**ptr) && (num_char < length) ) {
        hexValue = CharToHex (**ptr);
        if (hexValue >= 0) {
            *intValue = (*intValue << 4) | hexValue;
            num_char ++;
        } else {
            break;
        }
        (*ptr)++;
    }

  return (num_char);
}


int32_t GdbStub::HexToUint (char** ptr, uint32_t* intValue) const
{
    int32_t hexValue;
    int32_t num_char = 0;

    *intValue = 0;

    while(**ptr){
        hexValue = CharToHex (**ptr);
        if (hexValue >= 0) {
            *intValue = (*intValue << 4) | hexValue;
            num_char ++;
        } else {
            break;
        }
        (*ptr)++;
    }

  return (num_char);
}


int32_t GdbStub::HexToUint128 (char** ptr, uint64_t* data_l, uint64_t* data_u) const
{
    int32_t hexValue;
    int32_t num_char = 0;

    *data_l = 0;
    *data_u = 0;

    while(**ptr){
        hexValue = CharToHex (**ptr);
        if (hexValue >= 0) {
            // 4 left shift of 128bit value
            *data_u <<= 4;
            *data_u |= *data_l >> (64 - 4);
            *data_l <<= 4;
            *data_l |= static_cast<uint8_t> (hexValue & 0xF);
            num_char ++;
        } else {
            // Invalid ascii code is detected.
            break;
        }
        (*ptr)++;
    }

  return (num_char);
}


int32_t GdbStub::HexToInt (char** ptr, int32_t* intValue) const
{
    int32_t hexValue;
    int32_t num_char = 0;

    *intValue = 0;

    while(**ptr){
        hexValue = CharToHex (**ptr);
        if (hexValue >= 0) {
            *intValue = (*intValue << 4) | hexValue;
            num_char ++;
        } else {
            break;
        }
        (*ptr)++;
    }

  return (num_char);
}


void GdbStub::GdbQuerry (char* inbuf, char* outbuf)
{
    if (strncmp(inbuf, "C", 1) == 0){
        /* qC : Return current thread id */
        ThreadIdType threadid = GdbGetDebugThread();
        if (GDB_THREAD_ANY == 0) threadid = GDB_THREAD_0; /* AnyならHT0を選択 */

        if (threadid < 0){
            sprintf (outbuf, "QC-%x", -threadid);
        } else {
            sprintf (outbuf, "QC%x", threadid);
        }
    } else if (strncmp(inbuf, "Supported", 9) == 0){
        // stubが受信可能な最大サイズ（終端\0を除く）をGDBに通知する。
        // ファイル転送時の1パケットあたりのサイズ(=転送速度)に影響。
        outbuf += sprintf(outbuf, "PacketSize=%x", BUFMAX - 1);

    } else if (strncmp(inbuf, "fThreadInfo", 11) == 0){
        /* qfThreadInfo command */
        /* スレッド情報の出力：最初のスレッド（htid=0）のスレッド情報を返す */
        sprintf(outbuf, "m%08x", GdbGetThreadidFromHtid(GDB_HTID_INIT));
        m_qthreadinfo_next_htid = GDB_HTID_INIT + 1;

    } else if (strncmp(inbuf, "sThreadInfo", 11) == 0){
        /* qsThreadInfo command */
        // スレッド情報の出力(継続)：前回（qfThreadInfo または qsThreadInfo）の
        //                           次のスレッドのスレッド情報を返す
        HtIdType htid = m_qthreadinfo_next_htid;
        if (htid < m_forest_util->GetScNum ()){
            sprintf(outbuf, "m%08x", GdbGetThreadidFromHtid(htid));
            ++ m_qthreadinfo_next_htid;
        } else {
            sprintf(outbuf, "l");
        }
    } else if (strncmp(inbuf, "Rcmd", 4) == 0){
        /* qRcmd command */
        /* User defined packet */
        inbuf += 5;

        /* REGNAME=XX... : Write XX... to REGNAME register. */
        /* REGNAME       : Read data from REGNAME register. */
        #define QRCMD_MAX_LEN (96)
        #define QRCMD_RES_MAX_LEN (QRCMD_MAX_LEN + 48)
        char qrcmd_str[QRCMD_MAX_LEN];
        char qrcmd_response_str[QRCMD_RES_MAX_LEN] = "";
        char* reg_str; /* register name. */
        char* data_str; /* XX... accepts "0x" etc. */
        int pos = 0;

        /* Get qrCmd command */
        while((inbuf[0] != '\0') && (inbuf[1] != '\0')){
            int32_t c10 = CharToHex (inbuf[0]);
            int32_t c01 = CharToHex (inbuf[1]);
            if ((c10 == -1) || (c01 == -1)){
                /* 16進hexでない文字 */
                sprintf(outbuf, "ERR");
                return;
            } else {
                /* 16進 */
                qrcmd_str[pos] = static_cast<char>(((c10 & 0xf) << 4) | (c01 & 0xf));
                pos++;
                if (pos >= QRCMD_MAX_LEN) {
                    /* Too long */
                    sprintf(outbuf, "ERR");
                    return;
                }
            }
            inbuf += 2;
        }
        qrcmd_str[pos] = '\0';

        HtIdType htid = GdbGetDebugHtid();
        /* エラー判定 ： 無効なhtidならreturn */
        htid = GdbCheckValidHtid(htid);
        if (htid == GDB_HTID_INVALID){
            sprintf(outbuf, "ERR");
            return;
        }
        if (htid >= m_forest_util->GetScNum ()) {
            sprintf(outbuf, "ERR");
            return;
        }

        uint32_t peid = m_forest_util->GetPeId (htid);

        // debugモード設定コマンドかどうか確認
        if (strncmp(qrcmd_str, "debug", 5) == 0){
            int qrcmd_pos = 5;
            // debugモード設定
            // debug {on|off} または debug={on|off}
            if ((qrcmd_str[qrcmd_pos] == '=') || (qrcmd_str[qrcmd_pos] == ' ')) {
                // 区切りが＝かスペースなら次を読む
                qrcmd_pos++;
                if (strncmp(&qrcmd_str[qrcmd_pos], "on", 2) == 0) {
                    m_forest_util->SetTcDebugMode (peid, m_cmdopt->GetDebugMode ());
                } else if (strncmp(&qrcmd_str[qrcmd_pos], "off", 3) == 0) {
                    m_forest_util->SetTcDebugMode (peid, DBG_OFF);
                } else {
                    // 未対応
                    CfMsg::DPrint (MSG_ERR,
                                   "<Error: Unsupported debug mode (%s)>\n", &qrcmd_str[qrcmd_pos]);
                }
                sprintf(outbuf, "OK");
                return;
            }

            // error
            sprintf(outbuf, "ERR");
            return;
        }

        const char hexchars[] = "0123456789abcdef";

        // レジスタ名の終端位置を探し、そこで文字列を区切る。
        // もし区切りが '=' なら Write 操作。 '=' が無ければ Read 操作
        char* last_pos = strchr (qrcmd_str, '=');
        if (last_pos == NULL){
            /* Read */
            reg_str = qrcmd_str;

            CReg regno;

            /* レジスタ名検索 */
            regno = m_forest_util->RegGetRegno (peid, reg_str);
            if (regno != GR_NONE) {
                int written_size;
                if ((regno >= WR0) && (regno <= WR31)) {
                    // WR registers 128bit
                    RegData64 data_l;
                    RegData64 data_u;
                    m_forest_util->RegReadDebug (peid, regno, &data_l, &data_u);
                    written_size = snprintf(qrcmd_response_str, QRCMD_RES_MAX_LEN,
                                            "%s = 0x%016" PRIx64 "%016" PRIx64 "\n",
                                             reg_str, data_u, data_l);
                } else {
                    RegData64 data;
                    m_forest_util->RegReadDebug (peid, regno, &data);
                    written_size = snprintf(qrcmd_response_str, QRCMD_RES_MAX_LEN, "%s = 0x%08x\n",
                                            reg_str, static_cast<uint32_t>(data & 0xFFFFFFFFU));
                }
                if ((written_size < 0) || (written_size >= QRCMD_RES_MAX_LEN)) {
                    // Failed to snprintf to qrcmd_response_str
                    // Because buffer size is enough, this code isn't executed.
                    sprintf(outbuf, "ERR");
                    return;
                }
                char* p = qrcmd_response_str;
                while (*p != '\0'){
                    char c = *p++;
                    *outbuf++ = hexchars[(c >> 4) & 0xf];
                    *outbuf++ = hexchars[c & 0xf];
                }
                *outbuf = '\0';
            } else {
                /* 無効なレジスタ */
                sprintf(outbuf, "ERR");
                return;
            }
        } else {
            /* Write */
            *last_pos = '\0';
            reg_str = qrcmd_str;
            data_str = last_pos + 1;

            CReg regno = m_forest_util->RegGetRegno (peid, reg_str);
            if (regno != GR_NONE) {
                /* 文字列から書き込み値を生成 */
                if ((data_str[0] != '\0') && (data_str[1] != '\0')){
                    if (strncmp(data_str, "0x", 2) == 0){
                        data_str += 2;
                    }
                } else {
                    /* = のあとに有効な数値がない */
                    sprintf(outbuf, "ERR");
                    return;
                }
                if ((regno >= WR0) && (regno <= WR31)) {
                    uint64_t data_l = 0;
                    uint64_t data_u = 0;
                    HexToUint128(&data_str, &data_l, &data_u);
                    m_forest_util->RegWriteDebug (peid, regno, data_l, data_u);
                } else {
                    uint32_t data;
                    HexToUint(&data_str, &data);
                    m_forest_util->RegWriteDebug (peid, regno, data);
                }
                sprintf(outbuf, "OK");
            } else {
                /* 無効なレジスタ */
                sprintf(outbuf, "ERR");
                return;
            }
        }
    }
}


// GDBレジスタ番号をCForestのレジスタ番号に変換
CReg GdbStub::ChangeRegId (int32_t regno)
{
    if (regno >= static_cast<int32_t>(N_GDB_REG)) {
        return GR_NONE;
    }

    CReg regid = reglist[regno];
    if (regid != GR_NONE) {
        if ( (regno >= GREG_POS)
          && ((regno < WREG_POS_START) || (regno > WREG_POS_END))
          && (regno != PC_MIRROR_POS)
          && (regno != R29_MIRROR_POS) ) {
            regid = RegID::GetSRegId (static_cast<CfSReg>(regid));
        }
    }

    return regid;
}


void GdbStub::GdbRegRead (HtIdType htid, char* inbuf, char* outbuf)
{
    uint32_t regno;
    regno = 0;
    while (*inbuf != '\0') {
        regno = (regno << 4) + CharToHex (*inbuf++);
    }

    /* エラー判定 ： 無効なhtidならreturn */
    htid = GdbCheckValidHtid(htid);
    if (htid == GDB_HTID_INVALID) {
        return;
    }

    uint32_t peid = m_forest_util->GetPeId (htid);

    CReg regid = ChangeRegId (regno);
    if (regid != GR_NONE) {
        if ((regid >= WR0) && (regid <= WR31)) {
            // 128bit WR register
            uint64_t data_l = 0;
            uint64_t data_u = 0;
            m_forest_util->RegReadDebug (peid, regid, &data_l, &data_u);
            if (GetEndian() == ENDIAN_BIG){
                uint32_t data_w0 = ChangeEndianForGDB (4, static_cast<uint32_t>(data_l & 0xFFFFFFFFU));
                uint32_t data_w1 = ChangeEndianForGDB (4, static_cast<uint32_t>((data_l >> 32) & 0xFFFFFFFFU));
                uint32_t data_w2 = ChangeEndianForGDB (4, static_cast<uint32_t>(data_u & 0xFFFFFFFFU));
                uint32_t data_w3 = ChangeEndianForGDB (4, static_cast<uint32_t>((data_u >> 32) & 0xFFFFFFFFU));
                sprintf (outbuf, "%08x%08x%08x%08x", data_w3, data_w2, data_w1, data_w0);
            } else {
                uint32_t data_w0 = ChangeEndianForGDB (4, static_cast<uint32_t>(data_l & 0xFFFFFFFFU));
                uint32_t data_w1 = ChangeEndianForGDB (4, static_cast<uint32_t>((data_l >> 32) & 0xFFFFFFFFU));
                uint32_t data_w2 = ChangeEndianForGDB (4, static_cast<uint32_t>(data_u & 0xFFFFFFFFU));
                uint32_t data_w3 = ChangeEndianForGDB (4, static_cast<uint32_t>((data_u >> 32) & 0xFFFFFFFFU));
                sprintf (outbuf, "%08x%08x%08x%08x", data_w0, data_w1, data_w2, data_w3);
            }
        } else {
            // 32bit registers
            uint64_t data = 0;
            m_forest_util->RegReadDebug (peid, regid, &data);
            data = ChangeEndianForGDB (4, static_cast<uint32_t>(data & 0xFFFFFFFFU));
            sprintf (outbuf, "%08x", static_cast<uint32_t>(data & 0xFFFFFFFFU));
        }
    } else {
        /* 未サポートレジスタ番号 */
        uint64_t data = 0;
        sprintf (outbuf, "%08x", static_cast<uint32_t>(data & 0xFFFFFFFFU));
    }
}


void GdbStub::GdbRegReadAll (HtIdType htid, char* inbuf, char* outbuf)
{
    // VRegに対応するため64bitでデータ取得
    // とりあえずは 32bit に切り落として表示。要改変
    char tmpbuf[64];
    int32_t i;

    /* エラー判定 ： 無効なhtidならreturn */
    htid = GdbCheckValidHtid(htid);
    if (htid == GDB_HTID_INVALID) {
        return;
    }

    uint32_t peid = m_forest_util->GetPeId (htid);

    outbuf[0] = '\0';
    for (i = 0; i < static_cast<int32_t>(N_G_CMD_LIMIT); i++) {
        uint64_t data;
        CReg regid = ChangeRegId (i);
        if (regid != GR_NONE) {
            // Read register is only 32bit.
            // Because of N_G_CMD_LIMIT, VR/WR isn't read by this action.
            // If VR/WR is included, need to update as GdbRegRead to support 64bit/128bit registers.
            data = 0;
            m_forest_util->RegReadDebug (peid, regid, &data);
        } else {
            data = 0; // Invalid register
        }
        data = ChangeEndianForGDB (4, static_cast<uint32_t>(data & 0xFFFFFFFFU));
        sprintf (tmpbuf, "%08x", static_cast<uint32_t>(data & 0xFFFFFFFFU));
        strcat (outbuf, tmpbuf);
    }
}


void GdbStub::GdbRegWrite (HtIdType htid, char* inbuf, char* outbuf)
{
    /* エラー判定 ： 無効なhtidならreturn */
    htid = GdbCheckValidHtid(htid);
    if (htid == GDB_HTID_INVALID) {
        return;
    }

    uint32_t regno = 0;
    while ((*inbuf != '\0') && (*inbuf != '=')) {
        regno = (regno << 4) + CharToHex (*inbuf++);
    }
    inbuf++;

    CReg regid = ChangeRegId (regno);
    if (regid == GR_NONE) {
        // unsupported or invalid registers
        sprintf (outbuf, "OK");
    }

    if ((regid >= WR0) && (regid <= WR31)) {
        // 128bit WR register
        uint32_t count = 0;
        uint32_t data_w0 = 0;
        while ((*inbuf != '\0') && (count < 8)) {
            data_w0 = (data_w0 << 4) + CharToHex (*inbuf++);
            count++;
        }
        uint32_t data_w1 = 0;
        while ((*inbuf != '\0') && (count < 16)) {
            data_w1 = (data_w1 << 4) + CharToHex (*inbuf++);
            count++;
        }
        uint32_t data_w2 = 0;
        while ((*inbuf != '\0') && (count < 24)) {
            data_w2 = (data_w2 << 4) + CharToHex (*inbuf++);
            count++;
        }
        uint32_t data_w3 = 0;
        while ((*inbuf != '\0') && (count < 32)) {
            data_w3 = (data_w3 << 4) + CharToHex (*inbuf++);
            count++;
        }

        uint64_t data_l = static_cast<uint64_t> (ChangeEndianForGDB (4, data_w0));
        data_l |= static_cast<uint64_t> (ChangeEndianForGDB (4, data_w1)) << 32;
        uint64_t data_u = static_cast<uint64_t> (ChangeEndianForGDB (4, data_w2));
        data_u |= static_cast<uint64_t> (ChangeEndianForGDB (4, data_w3)) << 32;
        uint32_t peid = m_forest_util->GetPeId (htid);
        m_forest_util->RegWriteDebug (peid, regid, data_l, data_u);
    } else {
        // 32bit registers
        uint32_t data = 0;
        uint32_t count = 0;
        while ((*inbuf != '\0') && (count++ < 8)) {
            data = (data << 4) + CharToHex (*inbuf++);
        }
        data = ChangeEndianForGDB (4, data);
        uint32_t peid = m_forest_util->GetPeId (htid);
        m_forest_util->RegWriteDebug (peid, regid, data);
    }
    sprintf (outbuf, "OK");
}


void GdbStub::GdbAllTcPcWrite (RegData data)
{
    // 全スレッドのレジスタを設定する
    int32_t scnum = m_forest_util->GetScNum ();
    for (int32_t scid = 0; scid < scnum; scid++) {
        uint32_t peid = m_forest_util->GetPeId (scid);
        m_forest_util->RegWriteDebug (peid, PC, data);
    }
}


void GdbStub::GdbSetBreakPoint (char* inbuf, char* outbuf)
{
    uint32_t addr;
    uint32_t length;

    enum BreakType {
        SOFT_BREAKPOINT,
        HARD_BREAKPOINT,
        WRITE_WATCHPOINT,
        READ_WATCHPOINT,
        ACCESS_WATCHPOINT
    };
    BreakType break_type;

    /* <Z>t,addr,length */
    /* t : ブレーク種別 (Z0/Z1 のみサポート) */
    /* 0 : Software breakpoint */
    /* 1 : Hardware breakpoint */
    /* 2 : Write watchpoint */
    /* 3 : Read watchpoint */
    /* 4 : Access watchpoint */
    char in_type = *inbuf;
    if (in_type == '0') {
        break_type = SOFT_BREAKPOINT;
    } else if (in_type == '1') {
        break_type = HARD_BREAKPOINT;
    } else if (in_type == '2') {
        break_type = WRITE_WATCHPOINT;
    } else if (in_type == '3') {
        break_type = READ_WATCHPOINT;
    } else if (in_type == '4') {
        break_type = ACCESS_WATCHPOINT;
    } else {
        // サポートしていないブレーク種別
        return;
    }
    inbuf++;

    /* ','を空送り */
    if (strncmp(inbuf, ",", 1) != 0) {
        /* ztのあとに「,」が存在しない不正パケット */
        return;
    }
    inbuf++;

    /* addr : ブレークアドレス */
    addr = 0;
    /* NULLでなく、かつ、「,」でなければアドレスとみなして変換 */
    while ((*inbuf != '\0') && (*inbuf != ',')) {
        addr = (addr << 4) + CharToHex (*inbuf++);
    }

    /* ','を空送り */
    if (strncmp(inbuf, ",", 1) != 0) {
        /* zt,addrのあとに「,」が存在しない不正パケット */
        return;
    }
    inbuf++;

    // length : 監視アドレスの長さ (Z0/Z1では使わない)
    length = 0;
    while (*inbuf != '\0') {
        length = (length << 4) + CharToHex (*inbuf++);
    }
    inbuf++;

    // 終了アドレスを計算
    uint32_t end_addr = addr + (length - 1);
    if (addr > end_addr){
        /* invalid length */
        sprintf (outbuf, "EFF");  /* @@ エラーコード未定 */
        return;
    }

    /* 全スレッドにブレークを設定 */
    uint32_t sc_num = m_forest_util->GetScNum ();
    for (uint32_t scid = 0; scid < sc_num; scid ++) {
        uint32_t peid = m_forest_util->GetPeId (scid);

        switch (break_type) {
        case SOFT_BREAKPOINT:
        case HARD_BREAKPOINT:
            m_forest_util->AddBreakNextPc (peid, addr, BREAK_ADDR_MASK);
            break;
        case WRITE_WATCHPOINT:
            m_forest_util->AddBreakAccess (peid, addr, end_addr, ACC_WRITE, BREAK_ADDR_MASK);
            break;
        case READ_WATCHPOINT:
            m_forest_util->AddBreakAccess (peid, addr, end_addr, ACC_READ, BREAK_ADDR_MASK);
            break;
        case ACCESS_WATCHPOINT:
            m_forest_util->AddBreakAccess (peid,
                    addr, end_addr, ACC_READWRITE, BREAK_ADDR_MASK);
            break;
        }
    }

    sprintf (outbuf, "OK");
}


void GdbStub::GdbClearBreakPoint (char* inbuf, char* outbuf)
{
    uint32_t addr;
    uint32_t length;

    enum BreakType {
        SOFT_BREAKPOINT,
        HARD_BREAKPOINT,
        WRITE_WATCHPOINT,
        READ_WATCHPOINT,
        ACCESS_WATCHPOINT
    };
    BreakType break_type;

    /* <z>t,addr,length */
    /* t : ブレーク種別 (Z0/Z1 のみサポート) */
    /* 0 : Software breakpoint */
    /* 1 : Hardware breakpoint */
    /* 2 : Write watchpoint */
    /* 3 : Read watchpoint */
    /* 4 : Access watchpoint */
    char in_type = *inbuf;
    if (in_type == '0') {
        break_type = SOFT_BREAKPOINT;
    } else if (in_type == '1') {
        break_type = HARD_BREAKPOINT;
    } else if (in_type == '2') {
        break_type = WRITE_WATCHPOINT;
    } else if (in_type == '3') {
        break_type = READ_WATCHPOINT;
    } else if (in_type == '4') {
        break_type = ACCESS_WATCHPOINT;
    } else {
        // サポートしていないブレーク種別
        return;
    }
    inbuf++;

    /* ','を空送り */
    if (strncmp(inbuf, ",", 1) != 0) {
        /* ztのあとに「,」が存在しない不正パケット */
        return;
    }
    inbuf++;

    /* addr : ブレークアドレス */
    addr = 0;
    /* NULLでなく、かつ、「,」でなければアドレスとみなして変換 */
    while ((*inbuf != '\0') && (*inbuf != ',')) {
        addr = (addr << 4) + CharToHex (*inbuf++);
    }

    /* ','を空送り */
    if (strncmp(inbuf, ",", 1) != 0) {
        /* zt,addrのあとに「,」が存在しない不正パケット */
        return;
    }
    inbuf++;

    /* length : 監視アドレスの長さ (Z0/Z1 では使わない) */
    length = 0;
    while (*inbuf != '\0') {
        length = (length << 4) + CharToHex (*inbuf++);
    }
    inbuf++;

    // 終了アドレスを計算
    uint32_t end_addr = addr + (length - 1);
    if (addr > end_addr){
        /* invalid length */
        sprintf (outbuf, "EFF");  /* @@ エラーコード未定 */
        return;
    }

    /* 全スレッドのブレーク設定 */
    uint32_t sc_num = m_forest_util->GetScNum ();
    for (uint32_t scid = 0; scid < sc_num; scid ++) {
        uint32_t peid = m_forest_util->GetPeId (scid);

        switch (break_type) {
        case SOFT_BREAKPOINT:
        case HARD_BREAKPOINT:
            m_forest_util->ClearBreakNextPc (peid, addr, BREAK_ADDR_MASK);
            break;
        case WRITE_WATCHPOINT:
            m_forest_util->ClearBreakAccess (peid,
                    addr, end_addr, ACC_WRITE, BREAK_ADDR_MASK);
            break;
        case READ_WATCHPOINT:
            m_forest_util->ClearBreakAccess (peid,
                    addr, end_addr, ACC_READ, BREAK_ADDR_MASK);
            break;
        case ACCESS_WATCHPOINT:
            m_forest_util->ClearBreakAccess (peid,
                    addr, end_addr, ACC_READWRITE, BREAK_ADDR_MASK);
            break;
        }
    }
    sprintf (outbuf, "OK");
}


void GdbStub::GdbMemWrite (HtIdType htid, char* inbuf, char* outbuf)
{
    uint32_t addr;
    uint32_t length;
    uint32_t i;

    /* <M>addr,length:data... */
    /* addr : アドレス */
    addr = 0;
    /* NULLでなく、かつ、「,」でなければアドレスとみなして変換 */
    while ((*inbuf != '\0') && (*inbuf != ',')) {
        addr = (addr << 4) + CharToHex (*inbuf++);
    }

    /* ','を空送り */
    if (strncmp(inbuf, ",", 1) != 0) {
        /* addrのあとに「,」が存在しない不正パケット */
        return;
    }
    inbuf++;

    /* length : サイズ */
    length = 0;
    while ((*inbuf != '\0') && (*inbuf != ':')) {
        length = (length << 4) + CharToHex (*inbuf++);
    }

    /* ':'を空送り */
    if (strncmp(inbuf, ":", 1) != 0) {
        /* lengthのあとに「:」が存在しない不正パケット */
        return;
    }
    inbuf++;

    /* エラー判定 ： 無効なhtidならreturn */
    htid = GdbCheckValidHtid(htid);
    if (htid == GDB_HTID_INVALID) {
        return;
    }
    uint32_t peid = m_forest_util->GetPeId (htid);

    /* メモリライト */
    if ((length == 4) && ((addr & 0x3) == 0)) {
        /* レジスタアクセスのため、4byte単位アクセスする */
        uint32_t data;
        if (HexToUintN (&inbuf, &data, 8) == 8) {
            m_forest_util->VMemWriteDebug (peid, addr, data, SIZE_32BIT);
        } else {
            return;
        }
    } else if ((length == 2) && ((addr & 0x1) == 0)) {
        /* レジスタアクセスのため、2byte単位アクセスする */
        uint32_t data;
        if (HexToUintN (&inbuf, &data, 4) == 4) {
            m_forest_util->VMemWriteDebug (peid, addr, data & 0xFFFF, SIZE_16BIT);
        } else {
            return;
        }
    } else {
        for (i = 0; i < length; i++) {
            uint32_t data;
            if (HexToUintN (&inbuf, &data, 2) == 2) {
                m_forest_util->VMemWriteDebug (peid, addr + i, data & 0xFF, SIZE_8BIT);
            } else {
                return;
            }
        }
    }

    sprintf (outbuf, "OK");
}


void GdbStub::GdbMemWriteBinary (HtIdType htid, char* inbuf, char* outbuf)
{
    uint32_t addr;
    uint32_t length;

    /* <X>addr,length:data... */
    /* addr : アドレス */
    addr = 0;
    /* NULLでなく、かつ、「,」でなければアドレスとみなして変換 */
    while ((*inbuf != '\0') && (*inbuf != ',')) {
        addr = (addr << 4) + CharToHex (*inbuf++);
    }

    /* ','を空送り */
    if (strncmp(inbuf, ",", 1) != 0) {
        /* addrのあとに「,」が存在しない不正パケット */
        return;
    }
    inbuf++;

    /* length : サイズ */
    length = 0;
    while ((*inbuf != '\0') && (*inbuf != ':')) {
        length = (length << 4) + CharToHex (*inbuf++);
    }
    inbuf++;

    /* エラー判定 ： 無効なhtidならreturn */
    htid = GdbCheckValidHtid(htid);
    if (htid == GDB_HTID_INVALID) {
        return;
    }
    uint32_t peid = m_forest_util->GetPeId (htid);

    uint8_t* bin_data = reinterpret_cast <uint8_t*> (inbuf);

    uint32_t written_len;
    /* メモリライト */
    if ((length == 4) && ((addr & 0x3) == 0)){
        /* レジスタアクセスのため、4byte単位でアクセスする */
        uint32_t data = ((*bin_data & 0xFF) << 0)
             |   ((*(bin_data+1) & 0xFF) << 8)
             |   ((*(bin_data+2) & 0xFF) << 16)
             |   ((*(bin_data+3) & 0xFF) << 24);
        bin_data += 4;

        m_forest_util->VMemWriteDebug (peid, addr, data, SIZE_32BIT);
        written_len = 4;
    } else if ((length == 2) && ((addr & 0x1) == 0)){
        /* レジスタアクセスのため、2byte単位でアクセスする */
        uint32_t data = ((*bin_data & 0xFF) << 0)
                      | ((*(bin_data+1) & 0xFF) << 8);
        bin_data += 2;

        m_forest_util->VMemWriteDebug (peid, addr, data, SIZE_16BIT);
        written_len = 2;
    } else {
        for (PhysAddr i = 0; i < length; i++) {
            m_forest_util->VMemWriteDebug (peid, addr + i, *bin_data, SIZE_8BIT);
            ++ bin_data;
        }
        written_len = length;
    }

    if (written_len == length) {
        sprintf (outbuf, "OK");
    } else {
        sprintf (outbuf, "EFF");  /* @@ エラーコード未定 */
    }
}


void GdbStub::GdbMemRead (HtIdType htid, char* inbuf, char* outbuf)
{
    uint32_t addr;
    uint32_t data = 0;
    int32_t length;
    char databuf[9];
    int32_t i;

    /* <m>addr,length */
    /* addr : アドレス */
    addr = 0;
    while ((*inbuf != '\0') && (*inbuf != ',')) {
        addr = (addr << 4) + CharToHex (*inbuf++);
    }

    /* ','を空送り */
    if (strncmp(inbuf, ",", 1) != 0) {
        /* addrのあとに「,」が存在しない不正パケット */
        return;
    }
    inbuf++;

    /* length : サイズ */
    length = 0;
    while (*inbuf != '\0') {
        length = (length << 4) + CharToHex (*inbuf++);
    }

    /* エラー判定 ： 無効なhtidならreturn */
    htid = GdbCheckValidHtid(htid);
    if (htid == GDB_HTID_INVALID) {
        return;
    }
    uint32_t peid = m_forest_util->GetPeId (htid);

    outbuf[0] = '\0';
    if ((length == 4) && ((addr & 0x3) == 0)) {
        /* レジスタアクセスのため、4byteアクセスする */
        data = m_forest_util->VMemReadDebug (peid, addr, SIZE_32BIT);
        data = ChangeEndianForGDB (4, data & 0xFFFFFFFFU);
        sprintf (databuf, "%08x", data);
        strcat (outbuf, databuf);
    } else if ((length == 2) && ((addr & 0x1) == 0)) {
        /* レジスタアクセスのため、2byteアクセスする */
        data = m_forest_util->VMemReadDebug (peid, addr, SIZE_16BIT);
        data = ChangeEndianForGDB (2, data & 0xFFFFU);
        sprintf (databuf, "%04x", data);
        strcat (outbuf, databuf);
    } else {
        for (i = 0; i < length; i++) {
            data = m_forest_util->VMemReadDebug (peid, addr + i, SIZE_8BIT);
            sprintf (databuf, "%02x", data & 0xFFU);
            strcat (outbuf, databuf);
        }
    }
}


void GdbStub::GdbStepExec(int32_t htid, char* outbuf)
{
    // Ctrl-C をフック
    sighandler_t sigint_bak = signal (SIGINT, ForestUtil::ReqSimStop);

    // 全HTが動く
    if ( (htid < 0) || (htid >= m_forest_util->GetScNum ()) ) {
        // 無効なHT指定時はいずれかのCPUでstep実行
        m_break_htid = m_forest_util->StepExecute<STEP_ANY_MODE> (0, 1);
    } else {
        m_break_htid = m_forest_util->StepExecute<STEP_MODE> (htid, 1);
    }

    // Ctrl-Cのをフックを解除
    signal (SIGINT, sigint_bak);

    if (m_break_htid == -1){
        // CPUでブレークせず、step完了した場合 −１になる（SystemTimeブレーク、Ctrl-C)
        if (htid == GDB_HTID_ALL){
            m_break_htid = 0;
        } else {
            // vCont;s:TID;c 実行時にブレークせずステップで終わる場合、指定HIDを返す
            m_break_htid = htid;
        }
    }

    // ブレークシグナルを生成 PCとスレッドIDを渡す
    BreakEvent event  = m_forest_util->GetBreakEvent ();
    // VRegに対応するため64bitでデータ取得
    // とりあえずは 32bit に切り落として表示。要改変
    uint64_t break_pc = m_forest_util->GetBreakPc ();
    PhysAddr break_acc_addr = m_forest_util->GetAccBreakAddr ();
    ThreadIdType gdb_tid = GdbGetThreadidFromHtid (m_break_htid);
    uint32_t peid = m_forest_util->GetPeId (m_break_htid);

    // ChangeEndianForGDB の第2引数
    // むりやり32bitに切り落としている。要改変
    switch (event) {
    case BREAK_NONE:
#ifdef SUPPORT_SC_HEAP
    case BREAK_TIMEOUT:
    case BREAK_ACCEPT_INT:
#endif // SUPPORT_SC_HEAP
        m_forest_util->RegReadDebug (peid, reglist[0x40], &break_pc);
        sprintf(outbuf, "T05%02x:%08x;thread:%08x;",
                        0x40,
                        ChangeEndianForGDB (4, static_cast<uint32_t>(break_pc & 0xFFFFFFFFU)),
                        gdb_tid);
        break;
    case BREAK_PC:
    case BREAK_MAX:
        sprintf(outbuf, "T05%02x:%08x;thread:%08x;",
                        0x40,
                        ChangeEndianForGDB (4, static_cast<uint32_t>(break_pc & 0xFFFFFFFFU)),
                        gdb_tid);
        break;
    case BREAK_ACC_READ:
        sprintf(outbuf, "T05%02x:%08x;rwatch:%08x;thread:%08x;",
                        0x40,
                        ChangeEndianForGDB (4, static_cast<uint32_t>(break_pc & 0xFFFFFFFFU)),
                        break_acc_addr,
                        gdb_tid);
        break;
    case BREAK_ACC_WRITE:
        sprintf(outbuf, "T05%02x:%08x;watch:%08x;thread:%08x;",
                        0x40,
                        ChangeEndianForGDB (4, static_cast<uint32_t>(break_pc & 0xFFFFFFFFU)),
                        break_acc_addr,
                        gdb_tid);
        break;
    }

    // ブレーク発生元のスレッドにカレントを切り替える
    GdbSetDebugThread(gdb_tid);

    // 次回のためにブレーク情報をクリア
    m_forest_util->ClearBreakEvent ();
}


void GdbStub::GdbContinueExec(int32_t htid, char* outbuf)
{
    // Ctrl-C をフック
    sighandler_t sigint_bak = signal (SIGINT, ForestUtil::ReqSimStop);

    // 全HTが動く
    m_break_htid = m_forest_util->StepExecute<RUN_MODE> (0, 0);

    // Ctrl-Cのをフックを解除
    signal (SIGINT, sigint_bak);

    if (m_break_htid == -1){
        // CPUでブレークせず SystemTimeブレーク、Ctrl-Cで−１
        if (htid == GDB_HTID_ALL){
            m_break_htid = 0;
        } else {
            m_break_htid = htid;
        }
    }

    // ブレークシグナルを生成 PCとスレッドIDを渡す
    BreakEvent event  = m_forest_util->GetBreakEvent ();
    // VRegに対応するため64bitでデータ取得
    // とりあえずは 32bit に切り落として表示。要改変
    uint64_t break_pc = m_forest_util->GetBreakPc ();
    PhysAddr break_acc_addr = m_forest_util->GetAccBreakAddr ();
    ThreadIdType gdb_tid = GdbGetThreadidFromHtid (m_break_htid);
    uint32_t peid = m_forest_util->GetPeId (m_break_htid);

    // ChangeEndianForGDB の第2引数
    // むりやり32bitに切り落としている。要改変
    switch (event) {
    case BREAK_NONE:
#ifdef SUPPORT_SC_HEAP
    case BREAK_TIMEOUT:
    case BREAK_ACCEPT_INT:
#endif // SUPPORT_SC_HEAP
        m_forest_util->RegReadDebug (peid, reglist[0x40], &break_pc);
        sprintf(outbuf, "T05%02x:%08x;thread:%08x;",
                        0x40,
                        ChangeEndianForGDB (4, static_cast<uint32_t>(break_pc & 0xFFFFFFFFU)),
                        gdb_tid);
        break;
    case BREAK_PC:
    case BREAK_MAX:
        sprintf(outbuf, "T05%02x:%08x;thread:%08x;",
                        0x40,
                        ChangeEndianForGDB (4, static_cast<uint32_t>(break_pc & 0xFFFFFFFFU)),
                        gdb_tid);
        break;
    case BREAK_ACC_READ:
        sprintf(outbuf, "T05%02x:%08x;rwatch:%08x;thread:%08x;",
                        0x40,
                        ChangeEndianForGDB (4, static_cast<uint32_t>(break_pc & 0xFFFFFFFFU)),
                        break_acc_addr,
                        gdb_tid);
        break;
    case BREAK_ACC_WRITE:
        sprintf(outbuf, "T05%02x:%08x;watch:%08x;thread:%08x;",
                        0x40,
                        ChangeEndianForGDB (4, static_cast<uint32_t>(break_pc & 0xFFFFFFFFU)),
                        break_acc_addr,
                        gdb_tid);
        break;
    }

    // ブレーク発生元のスレッドにカレントを切り替える
    GdbSetDebugThread(gdb_tid);

    // 次回のためにブレーク情報をクリア
    m_forest_util->ClearBreakEvent ();
}


void GdbStub::GdbContinue (HtIdType htid, char* inbuf, char* outbuf)
{
    /* <c>[addr] */
    /* addr : アドレス */
    /* カレントスレッドが特定HTなら、 特定HTのみをcontinue実行 */
    /* カレントスレッドがANY/ALLなら全HTをcontinue実行 */

    uint32_t addr;

    /* エラー判定 ： 無効なhtidならreturn */
    if ( (htid == GDB_HTID_INVALID)
      || (htid >= m_forest_util->GetScNum ()) ) {
        sprintf (outbuf, "EFF");  /* @@ エラーコード未定 */
        return;
    } else {
        /* アドレス指定があれば、アドレスをPCに設定する */
        if (*inbuf != '\0') {
            addr = 0;
            while (*inbuf) {
                addr = (addr << 4) + CharToHex (*inbuf++);
            }
            addr = ChangeEndianForGDB (4, addr);

            if ((htid == GDB_HTID_ALL) || (htid == GDB_HTID_ANY)){
                GdbAllTcPcWrite (addr);
            } else {
                uint32_t peid = m_forest_util->GetPeId (htid);
                m_forest_util->RegWriteDebug (peid, PC, addr);
            }
        }

        /* ステップ実行 */
        GdbContinueExec(GDB_HTID_ALL, outbuf);
    }
}


void GdbStub::GdbStep (HtIdType htid, char* inbuf, char* outbuf)
{
    /* <s>[addr] */
    /* addr : アドレス */
    /* カレントスレッドが特定HTなら、 特定HTのみをステップ実行 */
    /* カレントスレッドがANY/ALLなら全HTをステップ実行 */

    uint32_t addr;

    /* エラー判定 ： 無効なhtidならreturn */
    if ( (htid == GDB_HTID_INVALID)
      || (htid >= m_forest_util->GetScNum ()) ) {
        sprintf (outbuf, "EFF");  /* @@ エラーコード未定 */
        return;
    } else {
        /* アドレス指定があれば、アドレスをPCに設定する */
        if (*inbuf != '\0') {
            addr = 0;
            while (*inbuf) {
                addr = (addr << 4) + CharToHex (*inbuf++);
            }
            addr = ChangeEndianForGDB (4, addr);

            if ((htid == GDB_HTID_ALL) || (htid == GDB_HTID_ANY)){
                GdbAllTcPcWrite (addr);
            } else {
                uint32_t peid = m_forest_util->GetPeId (htid);
                m_forest_util->RegWriteDebug (peid, PC, addr);
            }
        }

        /* ステップ実行 */
        GdbStepExec(GDB_HTID_ALL, outbuf);
    }
}


void GdbStub::GdbCheckThreadAlive(char* p_inbuf, char* outbuf)
{
    /* <T> thread-id */
    /* thread-id : -1=All thread, 0=any thread, other=スレッド番号 */
    ThreadIdType thread_id;
    HtIdType htid;

    HexToInt(&p_inbuf, &thread_id);
    htid = GdbGetHtidFromThreadid(thread_id);
    htid = GdbCheckValidHtid(htid);
    /* thread_id のスレッド生存確認　*/
    /* [FIXME] CPUOFFの場合、エラーを返すべき？ */
    if (htid != GDB_HTID_INVALID) {
      /* スレッドが存在 */
      sprintf (outbuf, "OK");
    } else {
      /* スレッドが存在しない */
      sprintf (outbuf, "EFF");  /* @@ エラーコード未定 */
    }
}


void GdbStub::GdbThreadSubsequentOperation(char* p_inbuf, char* outbuf)
{
    /* <H> subcommand thread-id */
    /* subcommand : c(Set thread for continue/step)，g(Set thread for other commands) */
    /* thread-id  : スレッド番号 (-1=All thread, 0=any thread, other=スレッド番号 */

    if ((p_inbuf[0] == '\0') || (p_inbuf[1] == '\0')){
        /* No Subsequent Operation or No thread-id */
        return;
    }

    /* Get thread-id */
    ThreadIdType threadid;
    if (p_inbuf[1] != '-') {
        char* p_buf = &p_inbuf[1];
        threadid = 0;
        while (*p_buf) {
            threadid = (threadid << 4) + CharToHex (*p_buf++);
        }
    } else if ((p_inbuf[2] == '1') && (p_inbuf[3] == '\0')){
        /* id = -1, All thread */
        threadid = GDB_THREAD_ALL;
    } else {
        /* Error */
        return;
    }

    /* Get subcommand */
    if (p_inbuf[0] == 'c'){
        /* Set current thread for c/s command */
        GdbSetRunThread(threadid);
    } else if (p_inbuf[0] == 'g'){
        /* Set current thread for other commands */
        GdbSetDebugThread(threadid);
    } else {
        /* unknown */
        return;
    }

    sprintf (outbuf, "OK");
}

//Memo
//http://www.cygwin.com/ml/archer/2010-q4/msg00006.html
//vCont was introduced because with only 'Hc', 's' and 'c', there's
//no way to distinguish "step a thread and resume all others" vs "step
//a thread and leave others stopped" (scheduler-locking, in gdb lingo).
//This was added way before non-stop was added.
//
//"vCont;s" or "vCont;s:-1" : Step all threads(ignore Hc thread)
//"vCont;c:TID;s:-1" isn't appeared. "vCont;c:TID;s" should be used.
//All GDB currently sends is in gdb/remote.c:remote_vcont_resume.
//All vCont packets GDB sends today have the actions ordered
//from more specific to less specific --- the most complicated
//is something like "vCont;s:pPID.TID;c" (step PID.TID, continue
//all others).

/* vCont;c/s;TID パケットを解析し、情報をvcont_argに格納する。 */
/* return 0=有効な情報が無い、1=有効な情報がある */
int GdbStub::ParseVcontPacket(char** p_inbuf, VcontArg* vcont_arg)
{
    char c = **p_inbuf;

    vcont_arg->signal = 0;

    /* command文字(C/c/S/sを取得) */
    switch(c){
    case 'C': /* Csig[:TID] */
        vcont_arg->signal = 1;
        vcont_arg->step = false;
        break;
    case 'c': /* c[:TID] */
        vcont_arg->step = false;
        break;
    case 'S': /* Ssig[:TID] */
        vcont_arg->signal = 1;
        vcont_arg->step = true;
#if (defined(_MSVC_LANG) && (_MSVC_LANG >= 201703L)) || (defined(__GNUC__) && (__GNUC__ >= 7))
        [[fallthrough]]; // valid attribute on c++17 and later
#endif

    case 's': /* s[:TID] */
        vcont_arg->step = true;
        break;
    default:
        return(0); /* No Packet */
    }
    /* C/c/S/sの次の文字にポインタを進める */
    (*p_inbuf)++;

    /* シグナル引数の取得 */
    // シグナル指定動作には非対応だが,シグナル指定ありの場合、
    // シグナル指定分だけポインタを進める必要がある。
    if (vcont_arg->signal == 1){
        if ((**p_inbuf == '\0') || (*(*p_inbuf+1) == '\0')){
            /* 異常な終端 */
            return(0);
        }
        int signal = CharToHex(**p_inbuf) << 4;
        (*p_inbuf)++;
        signal += CharToHex(**p_inbuf);
        (*p_inbuf)++;
        vcont_arg->signal = signal;
    }

    /* スレッド引数の取得 */
    if (**p_inbuf == ':'){
        /* スレッド引数あり */
        (*p_inbuf)++;

        if ((**p_inbuf == '-') && (*(*p_inbuf+1) == '1')){
            // -1指定とTID省略時の動作は同じなので、
            // GDBは-1を使わずTID省略パケットを使用するはずだが念のため
            vcont_arg->htid = GDB_HTID_ALL;
            (*p_inbuf)+=2;
        } else {
            ThreadIdType tid;
            HexToInt(p_inbuf, &tid);
            HtIdType htid = GdbGetHtidFromThreadid(tid);
            if (htid == GDB_HTID_ANY) {
                vcont_arg->htid = GDB_HTID_ALL; /* ANY指定時はALL実行 */
            } else if (htid == GDB_HTID_INVALID){
                return(0); /* 認識失敗 */
            } else {
                vcont_arg->htid = htid;
            }
        }
    } else {
        /* スレッド引数なし=All other threads */
        vcont_arg->htid = GDB_HTID_ALL;
    }

    return(1);
}


void GdbStub::GdbVCont(char* p_inbuf, char* outbuf)
{
    /* vCont packets */
    if (p_inbuf[0] == '?'){
        /* vCont? : Reply supported vCont command. */
        /* GDBはs/c/S/CをサポートしていないとvContコマンドを発行しないので注意 */
        /* Non-stop modeではさらにtコマンドのサポートも必須 */
#ifdef FORCE_VCONT_SUPPORT_SC
        // S/Cはシグナル付きs/cコマンド。CForestはシグナル非対応なので、C=c、S=sとして動作。
        sprintf(outbuf, "vCont;s;c;S;C");
#else // FORCE_VCONT_SUPPORT_SC
        // 真にCForestがサポートするvCont情報。
        // 現状、C/S非サポートだとGDBがvContコマンドを使わなくなるのでFORCE_VCONT_SUPPORT_SCで詐称。
        sprintf(outbuf, "vCont;s;c;");
#endif // FORCE_VCONT_SUPPORT_SC
    } else if (p_inbuf[0] == ';'){
        /* vCont : スレッド単位の実行 or ステップ実行 or Stop */
        // HT1個の場合でも、GDBは最大で2個までコマンドを送ってくるので+1する
        std::vector<VcontArg> vcont_arg (m_forest_util->GetScNum () + 1);
        int num_args = 0;
        /* とりあえず全部の指定を読み込む */
        while (*p_inbuf++ == ';') {
            if (ParseVcontPacket(&p_inbuf, &(vcont_arg[num_args])) == 1) {
                num_args++;
            } else {
                /* 有効な引数はなし => 次の；（区切り文字）までポインタを進める */
                char* next=strchr(p_inbuf, ';');
                if (next == NULL){
                    /* 区切り文字がないので、終端まで処理したこととなる。 */
                    break;
                } else {
                    p_inbuf = next;
                }
            }
        }

        /* 個別に処理 */
        /* NonStopMode */
        /* vCont;s:1;s:2;c => Step execute thread 1 and 2, and continue other threads. */
        /* vCont;s:1;c     => Step execute thread 1, and continue other threads. */
        /* vCont;s         => Step execute all thread. */
        /* vCont;s:1       => Step execute thread 1. */
        /* 本GDB stubでの動作(AllStopMode) */
        /* もしもstepとcontinueが同時に指定されたら,continueせずにstepする。 */
        /* vCont;s:1;s:2;c => Step execute all thread. */
        /* vCont;s:1;c     => Step execute all thread. */
        /* vCont;s         => Step execute all thread. GDBでは発行されない */
        /* vCont;s:1       => Step execute all thread. GDBでは発行されない */
        int use_s_thread = 0;
        int use_c_thread = 0;
        int use_s_no_thread = 0;
        int use_c_no_thread = 0;
        HtIdType target_c_htid = GDB_HTID_ALL;
        HtIdType target_s_htid = GDB_HTID_ALL;
        for (int i = 0; i < num_args; i++){
            if (vcont_arg[i].step == true){
                if ( (vcont_arg[i].htid == GDB_HTID_ALL)
                  || (vcont_arg[i].htid == GDB_HTID_ANY) ){
                    use_s_no_thread++;
                } else {
                    use_s_thread++;
                    target_s_htid = vcont_arg[i].htid;
                }
            } else {
                if ( (vcont_arg[i].htid == GDB_HTID_ALL)
                  || (vcont_arg[i].htid == GDB_HTID_ANY) ){
                    use_c_no_thread++;
                } else {
                    use_c_thread++;
                    target_c_htid = vcont_arg[i].htid;
                }
            }
        }

        /* AllStopMode 用ルーチン */
        if ((use_s_thread == 1) && (num_args == 1)){
            /* vCont;s:TID : 指定スレッドのみstep実行 */
            GdbStepExec(target_s_htid, outbuf);
        } else if ((use_c_thread == 1) && (num_args == 1)){
            /* vCont;c:TID : 指定スレッドのみcontinue実行 */
            GdbContinueExec(target_c_htid, outbuf);
        } else if ((use_s_thread != 0) || (use_s_no_thread != 0)){
            /* vCont;s , vCont;c:1;s:2, vCont;s:1;s:2;c 全スレッドstep実行 */
            GdbStepExec(target_s_htid, outbuf);
        } else if ((use_c_thread != 0) || (use_c_no_thread != 0)){
            /* vCont;c , vCont;c:1;c:2 全スレッドcontinue実行 */
            GdbContinueExec(target_c_htid, outbuf);
        } else {
            /* Error (num_args == 0の場合はここに来る) */
            return;
        }
    }
}


void GdbStub::GdbGetSignalNumber(char* p_inbuf, char* outbuf)
{
    sprintf(outbuf, "S05");
}


void GdbStub::GdbCmdExec (int client_sockfd)
{
    static char in_buffer[BUFMAX];
    static char out_buffer[BUFMAX];
    char* p_inbuf;
    out_buffer[0] = '\0';
    p_inbuf = GetGdbPacket (client_sockfd, in_buffer);

    if (p_inbuf == NULL) {
        /* 通信が切れた */
#ifdef DEBUG_GDB
        //printf ("\np_inbuf is NULL\n");
#endif // DEBUG_GDB
        return;
    }

#ifdef DEBUG_GDB
    printf ("\nStart command\n<- %s\n", in_buffer);
#endif // DEBUG_GDB

    switch (p_inbuf[0]) {
    case 'c': GdbContinue (GdbGetRunHtid(), &p_inbuf[1], out_buffer);     break;
    case 's': GdbStep (GdbGetRunHtid(), &p_inbuf[1], out_buffer);         break;
    case 'P': GdbRegWrite (GdbGetDebugHtid(), &p_inbuf[1], out_buffer);   break;
    case 'p': GdbRegRead (GdbGetDebugHtid(), &p_inbuf[1], out_buffer);    break;
    case 'g': GdbRegReadAll (GdbGetDebugHtid(), &p_inbuf[1], out_buffer); break;
    case 'H': GdbThreadSubsequentOperation(&p_inbuf[1], out_buffer); break;
    case 'M': GdbMemWrite (GdbGetDebugHtid(), &p_inbuf[1], out_buffer);   break;
    case 'X': GdbMemWriteBinary (GdbGetDebugHtid(), &p_inbuf[1], out_buffer); break;
    case 'm': GdbMemRead (GdbGetDebugHtid(), &p_inbuf[1], out_buffer);    break;
    case 'T': GdbCheckThreadAlive (&p_inbuf[1], out_buffer);         break;
    case 'Z': GdbSetBreakPoint (&p_inbuf[1], out_buffer);   break;
    case 'z': GdbClearBreakPoint (&p_inbuf[1], out_buffer); break;
    case 'q': GdbQuerry (&p_inbuf[1], out_buffer);          break;
    case 'k': m_quit = true; return;                         break;
    case 'v':
        if (strncmp("Cont", &p_inbuf[1], 4) == 0){
            GdbVCont(&p_inbuf[5], out_buffer);
        }
        break;
    case '?': GdbGetSignalNumber (&p_inbuf[1], out_buffer);        break;
    default: CfMsg::DPrint (MSG_WAR, "Unknown GDB command: %s\n", in_buffer); break;
    }

    /* コマンドに応答を返す */
    PutGdbPacket (client_sockfd, out_buffer);

#ifdef DEBUG_GDB
    printf ("\nAfter Command executed\n");
    printf ("<- %s\n", in_buffer);
    printf ("-> %s\n", out_buffer);
#endif // DEBUG_GDB
}


void GdbStub::ConnectGDB (uint16_t port)
{
    int server_sockfd;
    int client_sockfd;
    socklen_t server_len;
    socklen_t client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    /* ソケットの作成 */
    server_sockfd = socket (AF_INET, SOCK_STREAM, 0);
    if (server_sockfd < 0) {
        perror ("Socket for server failed!");
        exit (EXIT_NORMAL);
    }

    /* ソケットに名前をつける */
    bzero (reinterpret_cast<char*>(&server_address), sizeof (server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port); // Ignore conversion warning because it comes from htons code.
    server_len = sizeof (server_address);

    /* ソケットにアドレスをバインド */
    void* server_addr_tmp = &server_address; // To erace compiler warning
    if (bind (server_sockfd,
              static_cast<struct sockaddr*>(server_addr_tmp),
              server_len) < 0) {
        perror ("Bind failed!");
        exit (EXIT_NORMAL);
    }

    /* 接続キューを作成 */
    if (listen (server_sockfd, 1)) {
        perror ("Listen failed!");
        exit (EXIT_NORMAL);
    }
    printf("Plaese connect GDB to localhost:%d\n", port);

    /* 接続確立 */
    client_len = sizeof (client_address);
    void* client_addr_tmp = &client_address; // To erace compiler warning
    client_sockfd = accept (server_sockfd,
                            static_cast<struct sockaddr*>(client_addr_tmp),
                            &client_len);
    if (client_sockfd < 0) {
        perror ("Client accept failed!");
        exit (EXIT_NORMAL);
    }
    printf("GDB has been connected.\n");

    while (1) {
        fd_set rfds;
        struct timeval tv;
        int retval;

        /* コマンド受信待ち */
        FD_ZERO (&rfds);
        FD_SET (client_sockfd, &rfds);
        tv.tv_sec = 0;
        tv.tv_usec = 1;
        retval = select (client_sockfd+1, &rfds, NULL, NULL, &tv);

        if (retval == -1) {
            perror ("Select failed!");
        } else {
            if (FD_ISSET (client_sockfd, &rfds) > 0) {
                /* コマンドを受信 */
                /* GDBコマンド開始 */
                GdbCmdExec (client_sockfd);
                if (m_quit == true){
                    break;
                }
            }
        }
    }

    /* 後始末 */
    close(server_sockfd);
}


void GdbStub::GdbSetRunThread(ThreadIdType threadid)
{
    m_run_thread = threadid;
}

GdbStub::ThreadIdType GdbStub::GdbGetRunThread(void) const
{
    return(m_run_thread);
}

void GdbStub::GdbSetDebugThread(ThreadIdType threadid)
{
    m_debug_thread = threadid;
}

GdbStub::ThreadIdType GdbStub::GdbGetDebugThread(void) const
{
    return(m_debug_thread);
}


GdbStub::HtIdType GdbStub::GdbGetRunHtid(void) const
{
    return(GdbGetHtidFromThreadid(m_run_thread));
}


GdbStub::HtIdType GdbStub::GdbGetDebugHtid(void) const
{
    return(GdbGetHtidFromThreadid(m_debug_thread));
}

GdbStub::GdbStub (ForestUtil* forest_util, ForestCmdOpt* cmdopt)
    : m_forest_util (forest_util), m_cmdopt (cmdopt)
{
    m_quit = false;
    m_run_thread = GDB_THREAD_ANY;
    m_debug_thread = GDB_THREAD_ANY;
    m_qthreadinfo_next_htid = GDB_HTID_INIT;
    m_break_htid = GDB_HTID_INIT;
}
