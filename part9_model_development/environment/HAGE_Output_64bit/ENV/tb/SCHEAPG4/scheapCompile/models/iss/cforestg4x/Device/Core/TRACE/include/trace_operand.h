/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once
#include <string>
#include "./inst_id_list.h"

class TracePrintOperand {
public:
    void PrintOperand (uint32_t inst_id, uint64_t inst_code,
                       uint32_t inst_pc, std::string* inst_mnem) const;

    TracePrintOperand (void);

private:
    enum ReplaceType {
        W_31_28,
        WR_4_0,
        WR_47_43,
        WR_36_32,
        WR_31_27,
        WR_15_11,
        V_4_0,
        V_29_27_4_0,
        V_14_11,
        SR_4_0,
        SR_15_11,
        SEL_31_27,
        R_4_1_S1,
        R_4_0,
        R_31_28_S1,
        R_31_27,
        R_20_17_S1,
        R_20_16,
        R_15_12_S1,
        R_15_11,
        P_27_19_17,
        PO_31_27,
        L_31_21_0,
        I_63_32,
        I_5_1,
        I_4_0,
        I_47_32,
        I_47_32_31_16,
        I_3_0,
        I_31_27_4_0,
        I_31_16,
        I_21_18_4_0,
        I_15_11,
        IU_5_1,
        IU_5_0,
        IU_4_0,
        IU_31_16,
        IU_29_27,
        IU_24_21,
        IU_22_21,
        IU_21_21,
        IU_21_21_42_32,
        IU_21_18_4_0,
        IU_1_0,
        FCO_30_27,
        FCO_20_17,
        FCB_19_17,
        E_31_27,
        E_15_11,
        D_47_36_S4,
        D_47_35_S3,
        D_47_34_S2,
        D_31_16,
        DU_6_1_S2,
        DU_6_0_S1,
        DU_6_0,
        DU_47_32_31_17_S1,
        DU_47_32_26_21_S1,
        DU_47_32_26_20,
        DU_3_0_S1,
        DU_3_0,
        DU_31_17_S1,
        DU_31_17_5,
        DU_31_16,
        C_4_0,
        C_20_17,
        CA_12_11_31_27,
        B_13_11,
        BRA_5_0_31_17_S1,
        BD_4_4_31_17_S1,
        BD_15_11_6_4_S1,

        TOTAL_REPLACE_NUM
    };

    void Conv0                (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvADD              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvADD_I5           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvADD_I16          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvADF              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvAND              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvANDI             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvBCOND_D9         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvBCOND_D17        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvBINSL            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvBINSM            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvBINSU            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvBSH              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvBSW              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCACHE            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCALLT            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCAXI             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCLR1_B3          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCLR1             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCMOV_I5          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCMOV             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCMP_I5           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCMP              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCTRET            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvDI               (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvDISPOSE          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvDISPOSE_R        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvDIV              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvDIVH_R2          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvDIVH             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvDIVHU            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvDIVQ             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvDIVU             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvDIVQU            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvEI               (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvEIRET            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvFERET            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvFETRAP           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvHALT             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvHSH              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvHSW              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvHVTRAP           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvJARL             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvJARL_D22         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvJARL_D32         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvJMP              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvJMP_D32          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvJR_D22           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvJR_D32           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDB_D16          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDB_D23          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDBU_D16         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDBU_D23         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDDW_D23         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDH_D16          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDH_D23          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDHU_D16         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDHU_D23         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDW_D16          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDW_D23          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDLW             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDSR             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLOOP             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMAC              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMACU             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMOV_I32          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMOV_I5           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMOV              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMOVEA            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMOVHI            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMUL_I9           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMUL              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMULH_I5          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMULH             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMULHI            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMULU_I9          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMULU             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvNOP              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvNOT              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvNOT1_B3          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvNOT1             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvOR               (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvORI              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvPOPSP            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvPREF             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvPREPARE          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvPREPARE_SP       (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvPREPARE_LO16     (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvPREPARE_HI16     (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvPREPARE_I32      (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvPUSHSP           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvRIE              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvRIE_I9           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvROTL_I5          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvROTL_R3          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSAR_I5           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSAR              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSAR_R3           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSASF             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSATADD_I5        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSATADD           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSATADD_R3        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSATSUB           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSATSUB_R3        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSATSUBI          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSATSUBR          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSBF              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSCH0L            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSCH0R            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSCH1L            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSCH1R            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSET1_B3          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSET1             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSETF             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSHL_I5           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSHL              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSHL_R3           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSHR_I5           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSHR              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSHR_R3           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSLDB             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSLDBU            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSLDH             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSLDHU            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSLDW             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSNOOZE           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSSTB             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSSTH             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSSTW             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTB_D16          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTB_D23          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTDW_D23         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTH_D16          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTH_D23          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTW_D16          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTW_D23          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTCW             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTSR             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSUB              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSUBR             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSWITCH           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSXB              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSXH              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSYNCE            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSYNCI            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSYNCM            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSYNCP            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSYSCALL          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvTRAP             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvTST              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvTST1_B3          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvTST1             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvXOR              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvXORI             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvZXB              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvZXH              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvDBCP             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvDBPUSH           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvDBRET            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvDBTAG            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvDBTRAP           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvABSFD            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvABSFS            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvADDFD            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvADDFS            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCEILFDL          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCEILFDUL         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCEILFDUW         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCEILFDW          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCEILFSL          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCEILFSUL         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCEILFSUW         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCEILFSW          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCMOVFD           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCMOVFS           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCMPFD            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCMPFS            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFDL           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFDS           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFDUL          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFDUW          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFDW           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFLD           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFLS           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFSD           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFSL           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFSUL          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFSUW          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFSW           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFULD          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFULS          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFUWD          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFUWS          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFWD           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFWS           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvDIVFD            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvDIVFS            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvFLOORFDL         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvFLOORFDUL        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvFLOORFDUW        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvFLOORFDW         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvFLOORFSL         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvFLOORFSUL        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvFLOORFSUW        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvFLOORFSW         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMAXFD            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMAXFS            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMINFD            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMINFS            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMULFD            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMULFS            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvNEGFD            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvNEGFS            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvRECIPFD          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvRECIPFS          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvROUNDFDL         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvROUNDFDUL        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvROUNDFDUW        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvROUNDFDW         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvROUNDFSL         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvROUNDFSUL        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvROUNDFSUW        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvROUNDFSW         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvRSQRTFD          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvRSQRTFS          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSQRTFD           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSQRTFS           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSUBFD            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSUBFS            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvTRFSR            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvTRNCFDL          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvTRNCFDUL         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvTRNCFDUW         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvTRNCFDW          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvTRNCFSL          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvTRNCFSUL         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvTRNCFSUW         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvTRNCFSW          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvFMAFS            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvFMSFS            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvFNMAFS           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvFNMSFS           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFHS           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFSH           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvBR               (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCMPFS4           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvROUNDFSW4        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvROUNDFSUW4       (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvTRNCFSW4         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvTRNCFSUW4        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCEILFSW4         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCEILFSUW4        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvFLOORFSW4        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvFLOORFSUW4       (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFSW4          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFSUW4         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFWS4          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFUWS4         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFHS4          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCVTFSH4          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvABSFS4           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvNEGFS4           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSQRTFS4          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvRECIPFS4         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvRSQRTFS4         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvFLPVS4           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMOVVW4           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvTRFSRVW4         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvADDFS4           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSUBFS4           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMULFS4           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMAXFS4           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMINFS4           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvDIVFS4           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvADDRFS4          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSUBRFS4          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMULRFS4          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMAXRFS4          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMINRFS4          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvADDXFS4          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSUBXFS4          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvMULXFS4          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvADDSUBFS4        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSUBADDFS4        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvADDSUBXFS4       (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSUBADDXFS4       (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvADDSUBNFS4       (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSUBADDNFS4       (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvADDSUBNXFS4      (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSUBADDNXFS4      (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvFMAFS4           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvFMSFS4           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvFNMAFS4          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvFNMSFS4          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDVW_D16         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTVW_D16         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDVQW_D16        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTVQW_D16        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDVDW_D16        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTVDW_D16        (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDVZH4_D16       (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTVZH4_D16       (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCMOVFW4          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSHFLVW4          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCLIP_B           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCLIP_BU          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCLIP_H           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCLIP_HU          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDLBU            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDB_INC          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDBU_INC         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDB_DEC          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDBU_DEC         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTCB             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTB_INC          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTB_DEC          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDLHU            (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDH_INC          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDHU_INC         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDH_DEC          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDHU_DEC         (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTCH             (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTH_INC          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTH_DEC          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDW_INC          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDW_DEC          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTW_INC          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTW_DEC          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvRESBANK          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvCLL              (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTM_GSR          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvSTM_MP           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDM_GSR          (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;
    void ConvLDM_MP           (std::string* inst_op, uint64_t inst_code,
                               uint32_t inst_pc) const;

    uint32_t NumFromCode (uint64_t inst_code, ReplaceType id) const;

    void ReplaceReg     (std::string* inst_op, uint64_t inst_code,
                         uint32_t inst_pc, ReplaceType id) const;
    void ReplaceVReg    (std::string* inst_op, uint64_t inst_code,
                         uint32_t inst_pc, ReplaceType id) const;
    void ReplaceWReg    (std::string* inst_op, uint64_t inst_code,
                         uint32_t inst_pc, ReplaceType id) const;
    void ReplaceHexU    (std::string* inst_op, uint64_t inst_code,
                         uint32_t inst_pc, ReplaceType id) const;
    void ReplaceDec     (std::string* inst_op, uint64_t inst_code,
                         uint32_t inst_pc, ReplaceType id) const;
    void ReplaceEntry   (std::string* inst_op, uint64_t inst_code,
                         uint32_t inst_pc, ReplaceType id) const;
    void ReplaceList    (std::string* inst_op, uint64_t inst_code,
                         uint32_t inst_pc, ReplaceType id) const;
    void ReplaceSR      (std::string* inst_op, uint64_t inst_code,
                         uint32_t inst_pc, ReplaceType id) const;
    void ReplaceSel     (std::string* inst_op, uint64_t inst_code,
                         uint32_t inst_pc, ReplaceType id) const;
    void ReplaceCnd     (std::string* inst_op, uint64_t inst_code,
                         uint32_t inst_pc, ReplaceType id) const;
    void ReplaceFCO     (std::string* inst_op, uint64_t inst_code,
                         uint32_t inst_pc, ReplaceType id) const;
    void ReplaceCacheOp (std::string* inst_op, uint64_t inst_code,
                         uint32_t inst_pc, ReplaceType id) const;
    void ReplacePrefOp  (std::string* inst_op, uint64_t inst_code,
                         uint32_t inst_pc, ReplaceType id) const;
    void ReplaceBcond   (std::string* inst_op, uint64_t inst_code,
                         uint32_t inst_pc, ReplaceType id) const;

    void ReplaceBra     (std::string* inst_op, uint64_t inst_code,
                         uint32_t inst_pc, ReplaceType id) const;

    void ReplaceWidth   (std::string* inst_op, uint64_t inst_code,
                         uint32_t inst_pc, ReplaceType id) const;

    void ReplacePos     (std::string* inst_op, uint64_t inst_code,
                         uint32_t inst_pc, ReplaceType id) const;

    inline uint32_t OperandExtract (uint64_t x, uint32_t l, uint32_t r) const {
        return static_cast<uint32_t> ((x >> r) & ~(~0x0ULL << (l - r + 1)));
    }

    void (TracePrintOperand::*m_inst_func[TOTAL_INST_NUM+1]) (
              std::string* inst_op, uint64_t inst_code, uint32_t inst_pc) const;

    class ReplaceInfo {
    public:
        std::string m_rep;
        uint32_t    m_code_ll;
        uint32_t    m_code_lr;
        uint32_t    m_code_ml;
        uint32_t    m_code_mr;
        uint32_t    m_code_hl;
        uint32_t    m_code_hr;
        bool        m_hseg;
        bool        m_mseg;
        uint32_t    m_shift;
        uint32_t    m_offset;
        uint32_t    m_sig;
    };

    ReplaceInfo replace_info[TOTAL_REPLACE_NUM];
    void SetReplaceInfoValue (ReplaceType indx, const std::string& rep,
                              int32_t code_hl, int32_t code_hr,
                              int32_t code_ml, int32_t code_mr,
                              int32_t code_ll, int32_t code_lr,
                              bool hseg, bool mseg, uint32_t shift, uint32_t offset,
                              uint32_t sig);
};

