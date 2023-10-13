/*
 * (c) 2011,2012 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once

#include <Python.h>

#include "./svdpi.h"
#include "./forest_python.h"
#include "./forest_cmdopt.h"
#include "./forest_setup.h"
#include "./xidump.h"


/** \file cmp_runner_if_g4mh.h
 * \brief CompRunner G4MH sub-routine header file.
 *
 * This file contains the sub-routine in CompRunner G4MHv2.
 */


class ForestUtil;
class TracePrintOperand;

/** \namespace cmprunner
 * \brief This namespace is used for internal communication in CompRunner.
 *
 * This namespace contains the function, variable to control the behavior in internal CompRunner.
 */
namespace cmprunner {
#include "./forest_if.h"
#include "./trace_info.h"


/**
 * \var typedef uint32_t CmpResult.
 * \brief CompRunner return status.
 *
 * CompRunner details.
 *
 * \param CMP_OMIT                 : No register value in SV side. There is no comparison, only register index is described info in log file.
 * \param CMP_MATCH                : The comparison for each step is match. The result is described info in log file.
 * \param CMP_PC_MISS              : PC is mismatch. The result is described info in log file.
 * \param CMP_REG_LACK             : Lacking of register index by C side. The error status is described in log file.
 * \param CMP_REG_MISS             : Lacking of register index by SV side. The error status is described in log file.
 * \param CMP_FLAG_LACK            : Having different flag enable between C side and SV side. The error status is described in log file.
 * \param CMP_FLAG_MISS            : Having different flag value between C side and SV side. The error status is described in log file.
 * \param CMP_DATA_MISS            : Having different data write-back between C side and SV side. The error status is described in log file.
 * \param CMP_NB_MISS              : Lacking of Non-blocking ID between C side and SV side. The error status is described in log file.
 * \param CMP_OOO                  : Commit is in case out-of-order, used in G3KH. The info status is described in log file.
 * \param CMP_FXU_MATCH            : The comparison of FPSIMD instruction is match. The result is described info in log file.
 * \param CMP_FXU_REG_MISS         : Lacking of wide register index by SV side. The error status is described in log file.
 * \param CMP_FXU_DATA_MISS        : Having different data write-back in FPSIMD instruction between C side and SV side. The error status is described in log file.
 * \param CMP_ISS_UPD_OK           : Update CForest value to CompRunner buffer successfully. The result is described info in log file.
 * \param CMP_C2B1_COMMIT          : Commit is in case C2B1. The info status is described in log file.
 * \param CMP_LAST_MATCH           : The comparison for last commit in each instruction is match. The result is described info in log file.
 * \param CMP_RTL_ADD_EMP_OK       : Add register index without value from RTL info to CompRunner buffer successfully. The result is described info in log file.
 * \param CMP_RTL_ADD_EMP_ERR      : Can't add RTL info to CompRunner buffer. The error status is described info in log file.
 * \param CMP_RTL_ADD_R0_ERR       : Error due to adding R0 in SV side. The error status is described info in log file.
 * \param CMP_RTL_ADD_REG_OK       : Add register index with value from RTL side to CompRunner buffer successfully. The result is described info in log file.
 * \param CMP_RTL_ADD_REG_ERR      : Can't add register index with value from RTL side to CompRunner buffer. The error status is described info in log file.
 * \param CMP_RTL_ADD_EMP_DUP      : Duplication when adding register index without value from RTL info to CompRunner buffer. The error status is described info in log file.
 * \param CMP_RTL_ADD_REG_DUP      : Duplication when adding register index with value from RTL info to CompRunner buffer. The error status is described info in log file.
 * \param CMP_RTL_ADD_NBLK_DUP     : Duplication when adding non-blocking ID from RTL info to CompRunner buffer. The error status is described info in log file.
 * \param CMP_ISS_UPD_ERR          : Can't update CForest info to CompRunner buffer. The error status is described info in log file.
 * \param CMP_ISS_DEL_ERR          : Can't delete CForest info to CompRunner buffer. The error status is described info in log file.
 * \param CMP_ISS_GET_ERR          : Can't find CForest info to CompRunner buffer. The error status is described info in log file.
 * \param CMP_RTL_UPD_ERR          : Can't update RTL info to CompRunner buffer. The error status is described info in log file.
 * \param CMP_LAST_MISMATCH        : The comparison for last commit in each instruction is mismatch. The error status is described info in log file.
 * \param CMP_DATA_MISMATCH        : Different data between RTL and CForest. The error status is described info in log file.
 *
 */
typedef uint32_t CmpResult;
static const CmpResult CMP_OMIT           = 0;
static const CmpResult CMP_MATCH          = 1;
static const CmpResult CMP_PC_MISS        = 2;
static const CmpResult CMP_REG_LACK       = 3;
static const CmpResult CMP_REG_MISS       = 4;
static const CmpResult CMP_FLAG_LACK      = 5;
static const CmpResult CMP_DATA_MISS      = 6;
static const CmpResult CMP_NB_MISS        = 7;
static const CmpResult CMP_FXU_MATCH      = 9;
static const CmpResult CMP_FXU_REG_MISS   = 10;
static const CmpResult CMP_FXU_DATA_MISS  = 11;
static const CmpResult CMP_ISS_UPD_OK     = 12;
static const CmpResult CMP_C2B1_COMMIT    = 14;
static const CmpResult CMP_LAST_MATCH     = 15;

// result of RTL info registration step
static const CmpResult CMP_RTL_ADD_EMP_OK        = 16;
static const CmpResult CMP_RTL_ADD_EMP_ERR       = 17;
static const CmpResult CMP_RTL_ADD_R0_ERR        = 18;
static const CmpResult CMP_RTL_ADD_REG_OK        = 19;
static const CmpResult CMP_RTL_ADD_REG_ERR       = 20;
static const CmpResult CMP_RTL_ADD_EMP_DUP       = 21;
static const CmpResult CMP_RTL_ADD_REG_DUP       = 22;
static const CmpResult CMP_RTL_ADD_NBLK_DUP      = 23;

// Error when comparison
static const CmpResult CMP_ISS_UPD_ERR           = 25;
static const CmpResult CMP_ISS_DEL_ERR           = 26;
static const CmpResult CMP_ISS_GET_ERR           = 27;
static const CmpResult CMP_RTL_UPD_ERR           = 28;

static const CmpResult CMP_LAST_MISMATCH         = 30;

static const CmpResult CMP_FLAG_MISS = 32;

// Missmatch in mem/sreg cehcking function (DPI_MemoryDataCheck and DPI_SRegCheck)
static const CmpResult CMP_DATA_MISMATCH     = 33;



/**
 * \var typedef uint32_t RegInfoType.
 * \brief Register info type.
 *
 * This type describes the status of register index.
 *
 * \param TYPE_ISS                 : Register info is updated with CForest value.
 * \param TYPE_RTL                 : Register info is updated with RTL value.
 * \param TYPE_EMP                 : Register info is updated with register index, not any value.
 *
 */
typedef uint32_t RegInfoType;
static const RegInfoType TYPE_ISS       = 0;
static const RegInfoType TYPE_RTL       = 1;
static const RegInfoType TYPE_EMP       = 2;


/**
 * \var typedef uint32_t CmpRegNo.
 * \brief Register index value.
 *
 * This type describes the value of register index.
 *
 * \param CmpGRegMin               : Minimum index of general register
 * \param CmpGRegMax               : Maximum index of general register
 * \param CmpR32                   : Index of dummy register in register renaming technique.
 * \param CmpWRegMin               : Minimum index of wide register
 * \param CmpWRegMax               : Maximum index of wide register
 *
 */
typedef uint32_t CmpRegNo;
static const CmpRegNo CmpGRegMin  =  0;
static const CmpRegNo CmpGRegMax  = 31;
static const CmpRegNo CmpR32      = 32;
static const CmpRegNo CmpWRegMin  = 64;
static const CmpRegNo CmpWRegMax  = 95;


/**
 * \var typedef uint32_t CmpContextInfo.
 * \brief Define register context to check info.
 *
 * This type describes the value of register context when C side get info.
 *
 * \param CtxRegEmpInfo            : Register context in case Data read, not any values.
 * \param CtxRegRTLInfo            : Register context in case RTL value.
 * \param CtxCompByLast            : Context in case last commit.
 * \param CtxCompByNBLK            : Context in case Non-blocking ID.
 * \param CtxCheckLast             : Checking redundant register in CompRunner buffer.
 *
 */
typedef uint32_t CmpContextInfo;
static const CmpContextInfo CtxRegEmpInfo      = 0;
static const CmpContextInfo CtxRegRTLInfo      = 1;
static const CmpContextInfo CtxCompByLast      = 2;
static const CmpContextInfo CtxCompByNBLK      = 3;
static const CmpContextInfo CtxCheckLast       = 6;


/**
 * \var typedef uint32_t CmpActivity.
 * \brief Define print info in different case.
 *
 * This type describes the print message in different case.
 *
 * \param ActEmpty                 : Print message in case data read, not any values, not comparison.
 * \param ActRTLInfo               : Print message in case RTL value, not comparison.
 * \param ActCForestInfo           : Print message in case CForest value, not comparison.
 * \param ActMatch                 : Print message in case match result.
 *
 */
typedef uint32_t CmpActivity;
static const CmpActivity ActEmpty           = 0;
static const CmpActivity ActRTLInfo         = 1;
static const CmpActivity ActCForestInfo     = 2;
static const CmpActivity ActMatch           = 3;


/**
 * \var typedef uint32_t CmpIntLevel.
 * \brief Define the interruption type in each level.
 *
 * This type describes the different type in each interruption level.
 *
 * \param CmpFENMI                 : FENMI type.
 * \param CmpFEINT                 : FEINT type.
 * \param CmpGMFEINT               : GMFEINT type.
 * \param CmpBGFEINT               : BGFEINT type.
 * \param CmpEIINT                 : EIINT type.
 * \param CmpGMEIINT               : GMEIINT type.
 * \param CmpBGEIINT               : BGEIINT type.
 * \param CmpDBNMI                 : DBNMI type.
 * \param CmpDBINT                 : DBINT type.
 *
 */
typedef uint32_t CmpIntLevel;
static const CmpIntLevel CmpFENMI           = 0;
static const CmpIntLevel CmpFEINT           = 1;
static const CmpIntLevel CmpGMFEINT         = 2;
static const CmpIntLevel CmpBGFEINT         = 3;

static const CmpIntLevel CmpEIINT           = 0;
static const CmpIntLevel CmpGMEIINT         = 1;
static const CmpIntLevel CmpBGEIINT         = 2;

static const CmpIntLevel CmpDBNMI           = 0;
static const CmpIntLevel CmpDBINT           = 1;

//The ID of PSW, GMPSW are caculated basing selid, regid and cforest formula
//ID = 0x100 + (selid << 5) + regid
static const uint32_t HMPSW_ID    = 0x0105U ;
static const uint32_t GMPSW_NO    = 0x0225U ;

static const uint32_t UNUSED_VAL  = 0x00000000;
static const uint32_t ENABLE_VAL  = 0x00000001;
static const uint32_t DISABLE_VAL = 0x00000000;

// For Support new XDUMP Address updated by Xdump register
static const bool DATA_REG_INVALID = false;
static const bool DATA_REG_VALID = true;

static const uint32_t BYTE_MASK  = 0xFFU;
static const uint32_t HWORD_MASK = 0xFFFFU;
static const uint32_t WORD_MASK  = 0xFFFFFFFFU;
static const uint64_t WORD_MASK_UL = 0x00FFFFFFFFUL;

static const uint32_t HWORD_HIGH_SHIFT  = 16;
static const uint32_t HWORD_LOW_SHIFT   = 0;
static const uint32_t HWORD_HIGH  = 1;
static const uint32_t HWORD_LOW   = 0;

static const int32_t NO_WRITE_REG   = 0;
static const int32_t NO_WRITE_MEM   = 0;
static const int32_t NO_READ_REG   = 0;
static const int32_t NO_READ_MEM   = 0;

static const uint32_t NO_VM_SUPPORT   = 0;
static const uint32_t REG_ID_MASK   = 0xFFFU;
static const uint32_t DUMMY_REG_ID   = 0xFFFFFFFFU;

static const bool NOT_XDUMP_REG = true;
static const bool IS_XDUMP_REG = false;

static const bool XDUMP_CONDITION = true;
/**
 * \class RtlLogicVal
 *
 * \brief Define the method to convert from Logic type to C/C++ type.
 *
 * Logic value have 4 values, but C/C++ type only use 2 values.
 * This class is used to convert the input Logic values to C/C++ values.
 *
 */
class RtlLogicVal {
public:
    /**
     * \fn RtlLogicVal(svLogicVecVal *val)
     * \brief Create an RtlLogicVal object with input value.
     *
     * @param[in]  val                      input for RtlLogicVal object.
     * @return None.
     */
    RtlLogicVal(svLogicVecVal *val) {
        m_val = new svLogicVecVal();
        memcpy (m_val, val, sizeof (svLogicVecVal));
    }
    /**
     * \fn bool IsValidSignal (void)
     * \brief Check signal is valid or not.
     *
     * @param  -
     * @return bool value.
     */
    bool IsValidSignal (void) { return m_val->bval == 0; }
    /**
     * \fn uint32_t getIntVal (void)
     * \brief Get integer value of signal.
     *
     * @param  -
     * @return integer value.
     */
    uint32_t getIntVal (void) { return m_val->aval; }
    /**
     * \fn bool operator== (const RtlLogicVal *input_val)
     * \brief Overwrite operation == in RtlLogicVal objects.
     *
     * @param[in]  input_val                input for comparison.
     * @return bool value.
     */
    bool operator== (const RtlLogicVal *input_val) {
        svLogicVecVal *p_input_val = new svLogicVecVal();
        memcpy (p_input_val, input_val, sizeof (svLogicVecVal));
        return (p_input_val->aval == m_val->aval) && (p_input_val->bval == m_val->bval);
    }
    /**
     * \fn ~RtlLogicVal()
     * \brief Destructor function
     *
     * @param  -
     * @return None.
     */
    ~RtlLogicVal(){
        if (m_val) delete m_val;
    }
protected:
    /**
     * \var svLogicVecVal *m_val
     * \brief Pointer to svLogicVecVal.
     *
     */
    svLogicVecVal *m_val;
};


/**
 * \class CmpRegInfo
 *
 * \brief Define register info in CompRunner vector
 *
 * It is used to define the information of each register write-back.
 *
 */
class CmpRegInfo {
public:
    /**
     * \fn CmpRegInfo (uint32_t peid, uint32_t commit_type, uint32_t nblk_id, CReg reg, RegData64 reg_val_up, RegData64 reg_val_lo,
     *                 bool has_condition, bool xdump_arith, uint32_t xdump_diff_val, uint32_t xdump_mask, bool waw_status)
     * \brief Create an CmpRegInfo object with input value.
     *
     * @param[in]  peid                      peid.
     * @param[in]  commit_type               commit type.
     * @param[in]  nblk_id                   non-blocking id.
     * @param[in]  reg                       register index.
     * @param[in]  reg_val_up                64bit high value.
     * @param[in]  reg_val_lo                64bit low value.
     * @param[in]  has_condition             XDump condition.
     * @param[in]  xdump_arith               XDump for arithmetic.
     * @param[in]  xdump_diff_val            Different value in FPU/FXU.
     * @param[in]  xdump_mask                Value mask in comparison.
     * @param[in]  waw_status                Write-after-write status.
     *
     * @return None.
     */
    CmpRegInfo (uint32_t peid, uint32_t commit_type, uint32_t nblk_id, CReg reg, RegData64 reg_val_up,
                RegData64 reg_val_lo, bool has_condition, bool xdump_arith, uint32_t xdump_diff_val,
                uint32_t xdump_mask, bool waw_status)
        : m_peid(peid), m_commit_type (commit_type), m_nblk_id (nblk_id), m_reg (reg),
          m_reg_val_upper (reg_val_up), m_reg_val_lower (reg_val_lo), m_has_condition (has_condition),
          m_xdump_arith (xdump_arith), m_xdump_diff_val (xdump_diff_val), m_xdump_mask (xdump_mask),
          m_waw_status (waw_status), m_data_vld (true) {}
    /**
     * \fn CmpRegInfo ()
     * \brief Create an CmpRegInfo object with empty value.
     *
     * @param  -
     *
     * @return None.
     */
    CmpRegInfo () : m_peid(0x0), m_commit_type (0x0), m_nblk_id (0x0), m_reg (0x0), m_reg_val_upper (0x0),
               m_reg_val_lower (0x0), m_has_condition(false), m_xdump_arith (false), m_xdump_diff_val (0x0),
               m_xdump_mask (0x0), m_waw_status (false), m_data_vld (true) {}
    /**
     * \fn ~CmpRegInfo (void)
     * \brief Destructor function.
     *
     * @param  -
     *
     * @return None.
     */
    ~CmpRegInfo (void) {}

    /**
     * \fn uint32_t  GetPeid (void) const
     * \brief Return peid value.
     *
     * @param  -
     *
     * @return peid value.
     */
    uint32_t  GetPeid (void) const { return m_peid; }
    /**
     * \fn void SetPeid (uint32_t peid)
     * \brief Update peid value
     *
     * @param[in]  peid                      New peid.
     *
     * @return None.
     */
    void      SetPeid (uint32_t peid) { m_peid = peid; }
    /**
     * \fn uint32_t GetCommitType (void) const
     * \brief Get commit type
     *
     * @param  -
     *
     * @return commit type.
     */
    uint32_t  GetCommitType (void) const { return m_commit_type; }
    /**
     * \fn void SetCommitType (uint32_t commit_type)
     * \brief Update commit type
     *
     * @param[in]  commit_type                New commit type.
     *
     * @return None.
     */
    void      SetCommitType (uint32_t commit_type) { m_commit_type = commit_type; }
    /**
     * \fn uint32_t GetNblkId (void) const
     * \brief Get non-blocking ID.
     *
     * @param  -
     *
     * @return Non-blocking ID.
     */
    uint32_t  GetNblkId (void) const { return m_nblk_id; }
    /**
     * \fn void SetNblkId (uint32_t nblk_id)
     * \brief Update non-blocking id
     *
     * @param[in]  nblk_id                   New non-blocking ID.
     *
     * @return None.
     */
    void      SetNblkId (uint32_t nblk_id) { m_nblk_id = nblk_id; }
    /**
     * \fn CReg GetReg (void) const
     * \brief Get register index.
     *
     * @param  -
     *
     * @return register index.
     */
    CReg      GetReg (void) const { return m_reg; }
    /**
     * \fn void SetReg (uint32_t regno)
     * \brief Update register index.
     *
     * @param[in]  regno                     New register index.
     *
     * @return None.
     */
    void      SetReg (uint32_t regno) { m_reg = regno; }
    /**
     * \fn RegData64 GetRegValUp (void) const
     * \brief Get 64bit high value.
     *
     * @param  -
     *
     * @return 64bit value.
     */
    RegData64 GetRegValUp (void) const { return m_reg_val_upper; }
    /**
     * \fn RegData64 GetRegValLo (void) const
     * \brief Get 64bit low value.
     *
     * @param  -
     *
     * @return 64bit value.
     */
    RegData64 GetRegValLo (void) const { return m_reg_val_lower; }
    /**
     * \fn void SetRegValUp (RegData64 reg_val)
     * \brief Update 64bit high value
     *
     * @param[in]  reg_val                   New 64bit high value.
     *
     * @return None.
     */
    void      SetRegValUp (RegData64 reg_val) { m_reg_val_upper = reg_val; }
    /**
     * \fn void SetRegValLo (RegData64 reg_val)
     * \brief Update 64bit low value
     *
     * @param[in]  reg_val                   New 64bit low value.
     *
     * @return None.
     */
    void      SetRegValLo (RegData64 reg_val) { m_reg_val_lower = reg_val; }
    /**
     * \fn bool GetHasCondition (void) const
     * \brief Get XDump condition
     *
     * @param  -
     *
     * @return bool value.
     */
    bool      GetHasCondition (void) const {return m_has_condition; }
    /**
     * \fn void SetHasCondition (bool has_condition)
     * \brief Update XDump condition.
     *
     * @param[in]  has_condition                  New XDump condition.
     *
     * @return None.
     */
    void      SetHasCondition (bool has_condition) { m_has_condition = has_condition; }
    /**
     * \fn bool GetXDumpArith (void) const
     * \brief Get XDump arithmetic.
     *
     * @param  -
     *
     * @return bool value.
     */
    bool      GetXDumpArith (void) const {return m_xdump_arith; }
    /**
     * \fn void SetXDumpArith (bool xdump_arith)
     * \brief Update XDump arithmetic status
     *
     * @param[in]  xdump_arith                   New XDump arithmetic status.
     *
     * @return None.
     */
    void      SetXDumpArith (bool xdump_arith) { m_xdump_arith = xdump_arith; }
    /**
     * \fn uint32_t GetXDumpDiffVal (void) const
     * \brief Get XDump different value
     *
     * @param  -
     *
     * @return XDump different value.
     */
    uint32_t  GetXDumpDiffVal (void) const {return m_xdump_diff_val; }
    /**
     * \fn void SetXDumpDiffVal (uint32_t xdump_diff_val)
     * \brief Update the differnt value
     *
     * @param[in]  xdump_diff_val             New XDump different value.
     *
     * @return None.
     */
    void      SetXDumpDiffVal (uint32_t xdump_diff_val) { m_xdump_diff_val = xdump_diff_val; }
    /**
     * \fn uint32_t  GetXDumpMask (void) const
     * \brief Get the XDump mask value.
     *
     * @param  -
     *
     * @return XDump mask value.
     */
    uint32_t  GetXDumpMask (void) const {return m_xdump_mask; }
    /**
     * \fn void SetXDumpMask (uint32_t xdump_mask)
     * \brief Update XDump mask value
     *
     * @param[in]  xdump_mask                 New XDump mask value.
     *
     * @return None.
     */
    void      SetXDumpMask (uint32_t xdump_mask) { m_xdump_mask = xdump_mask; }
    /**
     * \fn bool GetWAWstatus (void) const
     * \brief Get Write-after-write status.
     *
     * @param  -
     *
     * @return bool value.
     */
    bool      GetWAWstatus (void) const {return m_waw_status; }
    /**
     * \fn void SetWAWstatus (bool waw_status)
     * \brief Update Write-after-write status.
     *
     * @param[in]  waw_status                 New write-after-write status.
     *
     * @return None.
     */
    void      SetWAWstatus (bool waw_status) { m_waw_status = waw_status; }
    /**
     * \fn void SetDataVld (bool data_vld)
     * \brief Set data valid flag by given input
     *
     * @param[in]  data_vld                   data valid flag
     *
     * @return None.
     */
    void      SetDataVld (bool data_vld) {m_data_vld = data_vld;}
    /**
     * \fn bool GetDataVld (void) const {return m_data_vld;}
     * \brief Get data valid flag of value
     *
     * @param  -
     *
     * @return bool value.
     */
    bool      GetDataVld (void) const {return m_data_vld;}

    // Check this register is General-purpose register or not.
    bool    IsGReg();
    // Check this register is Vector register or not.
    bool    IsWReg();
    // Check type of this register is type_in or not
    bool    IsMatchType(const RegInfoType& type_in);
    // Check type of this register is type_1/type_2 or not
    bool    IsMatchType(const RegInfoType& type_1, const RegInfoType& type_2);
private:
    uint32_t  m_peid;
    // Commit type
    // TYPE_ISS       = 0;
    // TYPE_RTL       = 1;
    // TYPE_EMP       = 2;
    uint32_t  m_commit_type;
    uint32_t  m_nblk_id;
    CReg      m_reg;
    // Register value have 2 elements: 64 bit upper and 64 bit lower.
    RegData64 m_reg_val_upper;
    RegData64 m_reg_val_lower;
    // this FPU, FXU insn will be applied XDump
    bool      m_has_condition;
    bool      m_xdump_arith;
    uint32_t  m_xdump_diff_val;
    uint32_t  m_xdump_mask;
    bool      m_waw_status;
    bool      m_data_vld;
};


/**
 * \class CmpRunnerIF
 *
 * \brief Define internal function CompRunner interface.
 *
 * This class is inherited from class CForestIF.
 * It also add some new internal functions for CompRunner behavior.
 *
 */
class CmpRunnerIF : public CForestIF {
public:
    /**
     * \fn virtual APIErrorCode VMemWriteDebug (uint32_t pe_id, PhysAddr addr,
     *                                          uint32_t data, ByteSize size)
     * \brief Memory write by debugger. Don't update memory write cycle and trace info
     *
     * @param[in]  pe_id                   PEID.
     * @param[in]  addr                    Virtual address.
     * @param[in]  size                    Size is 1,2,3,4.
     * @param[in]  data                    write data.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    virtual APIErrorCode VMemWriteDebug (uint32_t pe_id, PhysAddr addr,
                                         uint32_t data, ByteSize size);
    /**
     * \fn virtual APIErrorCode VMemReadDebug (uint32_t pe_id, PhysAddr addr,
     *                                         ByteSize size, uint32_t* data)
     * \brief Memory read by debugger. Don't update memory read cycle and trace info
     *
     * @param[in]  pe_id                   PEID.
     * @param[in]  addr                    Virtual address.
     * @param[in]  size                    Size is 1,2,3,4.
     * @param[out] data                    pointer to allocate read data.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    virtual APIErrorCode VMemReadDebug (uint32_t pe_id, PhysAddr addr,
                                        ByteSize size, uint32_t* data);
    /**
     * \fn virtual APIErrorCode ReqPseudoEiint (uint32_t pe_id, uint32_t channel,
     *                                          uint32_t priority, bool eitb)
     * \brief Request EIINT type to CForest. This function is used in CompRunner G4MHv1.
     *
     * @param[in]  pe_id                   PEID.
     * @param[in]  channel                 channel.
     * @param[in]  priority                priority.
     * @param[in]  eitb                    eitb.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    virtual APIErrorCode ReqPseudoEiint (uint32_t pe_id, uint32_t channel,
                                         uint32_t priority, bool eitb)
    {
        return ReqPseudoEiint (pe_id, channel, priority, eitb, false, 0, false);
    }
    /**
     * \fn virtual APIErrorCode ReqPseudoFeint (uint32_t pe_id, uint32_t channel)
     * \brief Request FEINT type to CPU. This function is used in CompRunner G4MHv1.
     *
     * @param[in]  pe_id                   PEID.
     * @param[in]  channel                 channel.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    virtual APIErrorCode ReqPseudoFeint (uint32_t pe_id, uint32_t channel)
    {
        return ReqPseudoFeint (pe_id, channel, false, 0, false);
    }
    /**
     * \fn virtual APIErrorCode ReqPseudoEiint (uint32_t pe_id, uint32_t channel, uint32_t priority, bool eitb,
     *                                          bool is_gm, uint32_t gpid, bool is_bgint)
     * \brief Request EIINT type to CPU. This function is used in CompRunner G4MHv2.
     *
     * @param[in]  pe_id                   PEID.
     * @param[in]  channel                 channel.
     * @param[in]  priority                priority.
     * @param[in]  eitb                    eitb.
     * @param[in]  is_gm                   Host mode or Guess mode.
     * @param[in]  gpid                    Guess ID.
     * @param[in]  is_bgint                background interruption status.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    virtual APIErrorCode ReqPseudoEiint (uint32_t pe_id, uint32_t channel, uint32_t priority, bool eitb,
                                        bool is_gm, uint32_t gpid, bool is_bgint);
    /**
     * \fn virtual APIErrorCode ReqPseudoFenmi (uint32_t pe_id).
     * \brief Request FENMI type to CPU. This function is used in CompRunner G4MHv2.
     *
     * @param[in]  pe_id                   PEID.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    virtual APIErrorCode ReqPseudoFenmi (uint32_t pe_id);
    /**
     * \fn virtual APIErrorCode ReqPseudoFeint (uint32_t pe_id, uint32_t channel, bool is_gm, uint32_t gpid, bool is_bgint)
     * \brief Request FENMI type to CPU. This function is used in CompRunner G4MHv2.
     *
     * @param[in]  pe_id                   PEID.
     * @param[in]  channel                 channel.
     * @param[in]  is_gm                   Host mode or Guess mode.
     * @param[in]  gpid                    Guess ID.
     * @param[in]  is_bgint                background interruption status.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    virtual APIErrorCode ReqPseudoFeint (uint32_t pe_id, uint32_t channel, bool is_gm, uint32_t gpid, bool is_bgint);
    /**
     * \fn virtual APIErrorCode ReqPseudoSyserr (uint32_t pe_id, uint32_t syserr_cause)
     * \brief Request SYSERR type to CPU.
     *
     * @param[in]  pe_id                   PEID.
     * @param[in]  syserr_cause            SysErr cause.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    virtual APIErrorCode ReqPseudoSyserr (uint32_t pe_id, uint32_t syserr_cause);
    /**
     * \fn virtual APIErrorCode HaltSnoozeRel (uint32_t pe_id)
     * \brief Request Release Halt/Snoooze state to CPU.
     *
     * @param[in]  pe_id                   PEID.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    virtual APIErrorCode HaltSnoozeRel (uint32_t pe_id);
    /**
     * \fn virtual APIErrorCode ReqPseudoOperandSyserr (uint32_t pe_id, uint32_t syserr_cause, bool syserr_mode, uint32_t syserr_gpid)
     * \brief Request Operand SYSERR type to CPU.
     *
     * @param[in]  pe_id                   PEID.
     * @param[in]  syserr_cause            Operand SysErr cause.
     * @param[in]  syserr_mode             Operand SysErr mode.
     * @param[in]  syserr_gpid             Operand SysErr gpid.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    virtual APIErrorCode ReqPseudoOperandSyserr (uint32_t pe_id, uint32_t syserr_cause, bool syserr_mode, uint32_t syserr_gpid);
    /**
     * \fn virtual APIErrorCode ReqPseudoEiint2Intc1 (uint32_t pe_id, uint32_t channel)
     * \brief Request EIINT type to INTC1. This function is used in CompRunner G4MHv2.
     *
     * @param[in]  pe_id                   PEID.
     * @param[in]  channel                 channel.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    virtual APIErrorCode ReqPseudoEiint2Intc1 (uint32_t pe_id, uint32_t channel);
    /**
     * \fn virtual APIErrorCode ReqPseudoFeint2Intc1 (uint32_t pe_id, uint32_t channel)
     * \brief Request FEINT type to INTC1. This function is used in CompRunner G4MHv2.
     *
     * @param[in]  pe_id                   PEID.
     * @param[in]  channel                 channel.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    virtual APIErrorCode ReqPseudoFeint2Intc1 (uint32_t pe_id, uint32_t channel);
    /**
     * \fn virtual APIErrorCode ReqPseudoFenmi2Intc1 (uint32_t pe_id)
     * \brief Request FENMI type to INTC1. This function is used in CompRunner G4MHv2.
     *
     * @param[in]  pe_id                   PEID.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    virtual APIErrorCode ReqPseudoFenmi2Intc1 (uint32_t pe_id);
    /**
     * \fn APIErrorCode ReqPseudoDbnmi (uint32_t pe_id)
     * \brief Request DBNMI type to CPU. This function is used in CompRunner G4MHv2.
     *
     * @param[in]  pe_id                   PEID.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    APIErrorCode ReqPseudoDbnmi (uint32_t pe_id);
    /**
     * \fn APIErrorCode ReqPseudoDbint (uint32_t pe_id, uint32_t channel)
     * \brief Request DBINT type to CPU. This function is used in CompRunner G4MHv2.
     *
     * @param[in]  pe_id                   PEID.
     * @param[in]  channel                 channel.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    APIErrorCode ReqPseudoDbint (uint32_t pe_id, uint32_t channel);
    /**
     * \fn APIErrorCode CancelPseudoEIINT (uint32_t pe_id, uint32_t channel)
     * \brief Cancel Eiint request to CPU.
     *
     * @param[in]  pe_id                   PEID.
     * @param[in]  channel                 channel.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    APIErrorCode CancelPseudoEIINT (uint32_t pe_id, uint32_t channel);
    /**
     * \fn APIErrorCode CancelPseudoEiint (uint32_t pe_id)
     * \brief Cancel Eiint request to CPU.
     *
     * @param[in]  pe_id                   PEID.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    APIErrorCode CancelPseudoEiint (uint32_t pe_id);
    /**
     * \fn APIErrorCode CancelPseudoFeint (uint32_t pe_id)
     * \brief Cancel Feint request to CPU.
     *
     * @param[in]  pe_id                   PEID.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    APIErrorCode CancelPseudoFeint (uint32_t pe_id);
    /**
     * \fn APIErrorCode CancelPseudoFenmi (uint32_t pe_id)
     * \brief Cancel Fenmi request to CPU.
     *
     * @param[in]  pe_id                   PEID.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    APIErrorCode CancelPseudoFenmi (uint32_t pe_id);
    /**
     * \fn APIErrorCode CancelPseudoDbint (uint32_t pe_id)
     * \brief Cancel Dbint request to CPU.
     *
     * @param[in]  pe_id                   PEID.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    APIErrorCode CancelPseudoDbint (uint32_t pe_id);
    /**
     * \fn APIErrorCode GetCmpIssFlags (uint32_t pe_id, uint32_t *iss_flags)
     * \brief Get PSW flag in CForest.
     *
     * @param[in]  pe_id                   PEID.
     * @param[out] iss_flags               Pointer to PSW flag in CForest.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    APIErrorCode GetCmpIssFlags (uint32_t pe_id, uint32_t *iss_flags);
    /**
     * \fn APIErrorCode GetOperandFormat (uint32_t inst_id, std::string* inst_str_ptr)
     * \brief Get operand format by CForest instruction id.
     *
     * @param[in]  inst_id                 CForest instruction id.
     * @param[out] inst_str_ptr            pointer to instruction operand.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    APIErrorCode GetOperandFormat (uint32_t inst_id, std::string* inst_str_ptr);
    /**
     * \fn APIErrorCode GetOperandSyserrStatus (uint32_t pe_id, bool syserr_mode, uint32_t syserr_gpid)
     * \brief Get operand syserr status in Cforest base on PEID, syserr mode and syserr gpid
     *
     * @param[in]  pe_id                   PEID.
     * @param[in]  syserr_mode             syserr_mode.
     * @param[in]  syserr_gpid             syserr_gpid.
     *
     * @return corresponding operand system error status.
     */
    bool GetOperandSyserrStatus (uint32_t pe_id, bool syserr_mode, uint32_t syserr_gpid);
    /**
     * \fn APIErrorCode CmpLoadSrec (const char* filename)
     * \brief Load srec file to CForest.
     *
     * @param[in]  filename                name of srec file.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    APIErrorCode CmpLoadSrec (const char* filename);
    /**
     * \fn APIErrorCode CmpStepExecute (uint32_t pe_id)
     * \brief Call CForest step execution.
     *
     * @param[in]  pe_id                   PEID.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    APIErrorCode CmpStepExecute (uint32_t pe_id);
    /**
     * \fn APIErrorCode CmpGetTrace (uint32_t pe_id, TraceCtrl** trace)
     * \brief Get Trace information by PEID.
     *
     * @param[in]  pe_id                   PEID.
     * @param[out] trace                   pointer to trace info.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    APIErrorCode CmpGetTrace (uint32_t pe_id, TraceCtrl** trace);
    /**
     * \fn APIErrorCode CmpGetOperand (uint32_t inst_id, uint64_t inst_code, uint32_t inst_pc, std::string* inst_mnem)
     * \brief Get mnemonic info by instruction ID.
     *
     * @param[in]  inst_id                 instruction ID.
     * @param[in]  inst_code               instruction opcode.
     * @param[in]  inst_pc                 instruction PC.
     * @param[out] inst_mnem               mnemonic of instruction.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    APIErrorCode CmpGetOperand (uint32_t inst_id, uint64_t inst_code, uint32_t inst_pc, std::string* inst_mnem);


    // Functions access Regiser info class
    /**
     * \fn APIErrorCode CmpAddRegInfo (uint32_t rtl_peid, uint32_t commit_type, uint32_t rtl_nblk_id, CReg regno,
     *                                 RegData64 reg_val_up, RegData64 reg_val_lo)
     * \brief Add register info to CompRunner buffer.
     *
     * @param[in]  rtl_peid                PEID.
     * @param[in]  commit_type             commit type.
     * @param[in]  rtl_nblk_id             RTL non-blocking ID.
     * @param[in]  regno                   register index.
     * @param[in]  reg_val_up              64bit high data.
     * @param[in]  reg_val_lo              64bit low data.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    APIErrorCode CmpAddRegInfo (uint32_t rtl_peid, uint32_t commit_type, uint32_t rtl_nblk_id, CReg regno, RegData64 reg_val_up, RegData64 reg_val_lo);
    /**
     * \fn APIErrorCode CmpDelRegInfo (uint32_t rtl_peid, uint32_t commit_type, uint32_t rtl_nblk_id, CReg regno,
     *                                 RegData64 reg_val_up, RegData64 reg_val_lo, uint32_t rtl_xdump_ctrl, CmpResult &cmp_ret)
     * \brief Delete register info in CompRunner buffer.
     *
     * @param[in]  rtl_peid                PEID.
     * @param[in]  commit_type             commit type.
     * @param[in]  rtl_nblk_id             RTL non-blocking ID.
     * @param[in]  regno                   register index.
     * @param[in]  reg_val_up              64bit high data.
     * @param[in]  reg_val_lo              64bit low data.
     * @param[in]  rtl_xdump_ctrl          RTL XDump control status.
     * @param[out] cmp_ret                 deletion result.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    APIErrorCode CmpDelRegInfo (uint32_t rtl_peid, uint32_t commit_type, uint32_t rtl_nblk_id, CReg regno,
                                RegData64 reg_val_up, RegData64 reg_val_lo, uint32_t rtl_xdump_ctrl, CmpResult &cmp_ret);
    /**
     * \fn APIErrorCode CmpUpdRegInfo (uint32_t rtl_peid, uint32_t commit_type, uint32_t rtl_nblk_id, CReg regno,
     *                                 RegData64 reg_val_up, RegData64 reg_val_lo, CmpResult &cmp_ret)
     * \brief Update register info in CompRunner buffer.
     *
     * @param[in]  rtl_peid                PEID.
     * @param[in]  commit_type             commit type.
     * @param[in]  rtl_nblk_id             RTL non-blocking ID.
     * @param[in]  regno                   register index.
     * @param[in]  reg_val_up              64bit high data.
     * @param[in]  reg_val_lo              64bit low data.
     * @param[out] cmp_ret                 update result.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    APIErrorCode CmpUpdRegInfo (uint32_t rtl_peid, uint32_t commit_type, uint32_t rtl_nblk_id, CReg regno,
                                RegData64 reg_val_up, RegData64 reg_val_lo, CmpResult &cmp_ret);
    /**
     * \fn APIErrorCode CmpGetRegInfo (uint32_t rtl_peid, uint32_t context_info, uint32_t rtl_nblk_id, CReg regno,
     *                                 RegData64 reg_val_up, RegData64 reg_val_lo, CmpRegInfo &cmp_reg_info, CmpResult &cmp_ret)
     * \brief Update register info in CompRunner buffer.
     *
     * @param[in]  rtl_peid                PEID.
     * @param[in]  context_info            Context information.
     * @param[in]  rtl_nblk_id             RTL non-blocking ID.
     * @param[in]  regno                   register index.
     * @param[in]  reg_val_up              64bit high data.
     * @param[in]  reg_val_lo              64bit low data.
     * @param[out] cmp_reg_info            Pointer to CmpRegInfo object.
     * @param[out] cmp_ret                 getting result.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    APIErrorCode CmpGetRegInfo (uint32_t rtl_peid, uint32_t context_info, uint32_t rtl_nblk_id, CReg regno,
                                RegData64 reg_val_up, RegData64 reg_val_lo, CmpRegInfo &cmp_reg_info, CmpResult &cmp_ret);
    /**
     * \fn bool CheckLastCommit (uint32_t pe_id)
     * \brief Check last commit or not.
     *
     * @param[in]  pe_id                   PEID.
     *
     * @return checking result.
     */
    bool CheckLastCommit (uint32_t pe_id);
    /**
     * \fn bool UpdateXDumpInfo(uint32_t pe_id, TraceCtrl *trace)
     * \brief Update XDump information.
     *
     * @param[in]  pe_id                   PEID.
     * @param[in]  trace                   pointer to TraceCtrl object.
     *
     * @return update result.
     */
    bool UpdateXDumpInfo(uint32_t pe_id, TraceCtrl *trace);
    /**
     * \fn bool CheckXDumpInfo (uint32_t pe_id, CReg reg_no, RegData iss_wdata_0, RegData iss_wdata_1, RegData iss_wdata_2, RegData iss_wdata_3,
     *                          RegData rtl_wdata_0, RegData rtl_wdata_1, RegData rtl_wdata_2, RegData rtl_wdata_3,
     *                          uint32_t mask_val, bool xdump_arith, uint32_t xdump_diffval, uint32_t rtl_xdump_ctrl, bool is_waw_status)
     * \brief Check XDump method is applied or not with the input information.
     *
     * @param[in]  pe_id                   PEID.
     * @param[in]  reg_no                  register index.
     * @param[in]  iss_wdata_0             the 1st 32bit in CForest.
     * @param[in]  iss_wdata_1             the 2nd 32bit in CForest.
     * @param[in]  iss_wdata_2             the 3rd 32bit in CForest.
     * @param[in]  iss_wdata_3             the 4th 32bit in CForest.
     * @param[in]  rtl_wdata_0             the 1st 32bit in RTL.
     * @param[in]  rtl_wdata_1             the 2nd 32bit in RTL.
     * @param[in]  rtl_wdata_2             the 3rd 32bit in RTL.
     * @param[in]  rtl_wdata_3             the 4th 32bit in RTL.
     * @param[in]  mask_val                Mask value.
     * @param[in]  xdump_arith             XDump arithmetic status.
     * @param[in]  xdump_diffval           XDump different limitation value.
     * @param[in]  rtl_xdump_ctrl          RTL XDump control status.
     * @param[in]  is_waw_status           Write-after-write status.
     *
     * @return checking result.
     */
    bool CheckXDumpInfo (uint32_t pe_id, CReg reg_no, RegData iss_wdata_0, RegData iss_wdata_1, RegData iss_wdata_2, RegData iss_wdata_3,
                         RegData rtl_wdata_0, RegData rtl_wdata_1, RegData rtl_wdata_2, RegData rtl_wdata_3,
                         uint32_t mask_val, bool xdump_arith, uint32_t xdump_diffval, uint32_t rtl_xdump_ctrl, bool is_waw_status);
    /**
     * \fn void DumpFlag (uint32_t rtl_peid, uint32_t rtl_FL_Wen_chk, uint32_t rtl_FL_val_chk, uint32_t psw_type)
     * \brief Force RTL value to CForest with specified PSW type.
     *
     * @param[in]  rtl_peid                PEID.
     * @param[in]  rtl_FL_Wen_chk          RTL flag write enable.
     * @param[in]  rtl_FL_val_chk          RTL flag write value.
     * @param[in]  psw_type                PSW type.
     *
     * @return None.
     */
    void DumpFlag (uint32_t rtl_peid, uint32_t rtl_FL_Wen_chk, uint32_t rtl_FL_val_chk, uint32_t psw_type);
    /**
     * \fn bool CmpCompFlag (uint32_t rtl_peid, uint32_t rtl_FL_Wen_chk, uint32_t rtl_FL_val_chk, uint32_t iss_FL_Wen_chk, uint32_t iss_FL_val_chk,
     *                       uint32_t rtl_xdump_ctrl, bool iss_fl_hmode, bool iss_fl_gmode, TraceCtrl *trace)
     * \brief Compare status flag between RTL and CForest. IF XDump method is specified, RTL flag will be forced to CForest flag.
     *
     * @param[in]  rtl_peid                PEID.
     * @param[in]  rtl_FL_Wen_chk          RTL flag write enable.
     * @param[in]  rtl_FL_val_chk          RTL flag write value.
     * @param[in]  iss_FL_Wen_chk          CForest flag write enable.
     * @param[in]  iss_FL_val_chk          CForest flag write value.
     * @param[in]  rtl_xdump_ctrl          RTL XDump control status.
     * @param[in]  iss_fl_hmode            Host mode .
     * @param[in]  iss_fl_gmode            Guess mode.
     * @param[in]  trace                   Pointer to TraceCtrl object.
     *
     * @return comparison status.
     */
    bool CmpCompFlag (uint32_t rtl_peid, uint32_t rtl_FL_Wen_chk, uint32_t rtl_FL_val_chk, uint32_t iss_FL_Wen_chk, uint32_t iss_FL_val_chk,
                      uint32_t rtl_xdump_ctrl, bool iss_fl_hmode, bool iss_fl_gmode, TraceCtrl *trace);
    /**
     * \fn bool SetMcycMiddleCount(uint32_t rtl_peid, uint32_t cmp_mcyc_middle_cnt)
     * \brief Update the counter to middle step in multi-cycle instruction.
     *
     * @param[in]  rtl_peid                PEID.
     * @param[in]  cmp_mcyc_middle_cnt     counter of middle step.
     *
     * @return update result.
     */
    bool SetMcycMiddleCount(uint32_t rtl_peid, uint32_t cmp_mcyc_middle_cnt);
    /**
     * \fn bool RegWriteDebug (uint32_t peid, CReg regid, RegData64 data_l64, RegData64 data_u64 = 0)
     * \brief Register write from Debugger, GReg and Sreg, Vreg
     *
     * @param[in]  peid                   PEID.
     * @param[in]  regid                  register id.
     * @param[in]  data_l64               64bit low data to write.
     * @param[in]  data_u64               64bit high data to write.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    bool RegWriteDebug (uint32_t peid, CReg regid,
                          RegData64 data_l64, RegData64 data_u64 = 0);
    /**
     * \fn bool RegReadDebug  (uint32_t peid, CReg regid, RegData64* data_l64, RegData64* data_u64 = NULL).
     * \brief Register read from Debugger, GReg and Sreg, Vreg
     *
     * @param[in]  peid                   PEID.
     * @param[in]  regid                  register id.
     * @param[out] data_l64               pointer to 64bit low data to put read data.
     * @param[out] data_u64               pointer to 64bit high data to put read data.
     *
     * @return APIErrorCode.
     * @retval API_E_OK     : There is no error.
     * @retval API_E_PARAM  : There is illegal parameter.
     */
    bool RegReadDebug  (uint32_t peid, CReg regid,
                          RegData64* data_l64, RegData64* data_u64 = NULL);
    /**
     * \fn void StartPy (char *srec_file, uint32_t l1mirr_mode, uint32_t maaen,
     *                   std::ostringstream *oss_python_name, char* argv[])
     * \brief Configure python setting with full info.
     *
     * @param[in]  srec_file               Pointer to srec file.
     * @param[in]  l1mirr_mode             L1MIRR mode.
     * @param[in]  maaen                   MAAEN value.
     * @param[in]  oss_python_name         Python name.
     * @param[in]  argv                    Pointer to other arguments.
     *
     * @return None.
     */
    void StartPy (char *srec_file, uint32_t l1mirr_mode, uint32_t maaen,
                  std::ostringstream *oss_python_name, char* argv[]);
    /**
     * \fn void SetFileFP (FILE *fp)
     * \brief Set log file
     *
     * @param[in]  fp                      Pointer to log file.
     *
     * @return None.
     */
    void SetFileFP (FILE *fp);
    /**
     * \fn void ResetPE (uint32_t rtl_peid)
     * \brief reset PE
     *
     * @param[in]  rtl_peid               PEID.
     *
     * @return None.
     */
    void ResetPE (uint32_t rtl_peid);
    /**
     * \fn std::string GetVersionStr (void)
     * \brief Get CompRunner version.
     *
     * @param  -
     *
     * @return CompRunner version.
     */
    std::string GetVersionStr (void);

    // Error check for argument of API
    /**
     * \fn bool IsPeIdValid (uint32_t pe_id) const.
     * \brief Check pe_id argument of API
     *
     * @param[in]  pe_id                   PEID to check.
     *
     * @return Checking result: true=NoError, false=Error. Error case is pe_id=0 or pe_id > num of pe.
     */
    bool IsPeIdValid (uint32_t pe_id) const;

    /**
     * \fn bool IsByteSizeValid (ByteSize size) const
     * \brief Check ByteSize content for VMemRead/Write, PMemRead/Write.
     *
     * @param[in]  size                    memory access size.
     *
     * @return Checking result: true=NoError, false=Error. Error occur if size isn't 1~4.
     */
    //! Check ByteSize content for VMemRead/Write, PMemRead/Write.
    bool IsByteSizeValid (ByteSize size) const;

    // Debug function
    /**
     * \fn void DB_DPI_CmpStep (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_pc, uint32_t rtl_last_commit,
     *                          uint32_t rtl_c2b1_commit, uint32_t rtl_ooo_commit, uint32_t rtl_mcyc_commit,
     *                          uint32_t rtl_regwrite_en, uint32_t rtl_reg_num, svLogicVecVal* rtl_gr_val,
     *                          svLogicVecVal* rtl_wr_val_0, svLogicVecVal* rtl_wr_val_1, svLogicVecVal* rtl_wr_val_2, svLogicVecVal* rtl_wr_val_3,
     *                          uint32_t rtl_flagwrite_en, uint32_t rtl_flag_val, uint32_t rtl_brk_ch, uint32_t rtl_nblk_id, uint32_t rtl_xdump_ctrl)
     * \brief Print RTL info of DPI_CmpStep for debugging. There is no CForest info here.
     *
     * @param[in]  rtl_peid                PEID.
     * @param[in]  rtl_time                Time.
     * @param[in]  rtl_cycle               Cycle.
     * @param[in]  rtl_pc                  PC.
     * @param[in]  rtl_last_commit         Last commit status.
     * @param[in]  rtl_c2b1_commit         C2B1 commit status.
     * @param[in]  rtl_ooo_commit          Out-of-order commit status.
     * @param[in]  rtl_mcyc_commit         Multi-cycle instruction commit status.
     * @param[in]  rtl_regwrite_en         Register write enable.
     * @param[in]  rtl_reg_num             Register index.
     * @param[in]  rtl_gr_val              Register value.
     * @param[in]  rtl_wr_val_0            the 1st 32bit.
     * @param[in]  rtl_wr_val_1            the 2nd 32bit.
     * @param[in]  rtl_wr_val_2            the 3rd 32bit.
     * @param[in]  rtl_wr_val_3            the 4th 32bit.
     * @param[in]  rtl_flagwrite_en        Flag write enable.
     * @param[in]  rtl_flag_val            Flag value.
     * @param[in]  rtl_brk_ch              Breack channel.
     * @param[in]  rtl_nblk_id             Non-blocking ID.
     * @param[in]  rtl_xdump_ctrl          XDump control.
     *
     * @return None.
     */
    void DB_DPI_CmpStep (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_pc, uint32_t rtl_last_commit,
                         uint32_t rtl_c2b1_commit, uint32_t rtl_ooo_commit, uint32_t rtl_mcyc_commit,
                         uint32_t rtl_regwrite_en, uint32_t rtl_reg_num, svLogicVecVal* rtl_gr_val,
                         svLogicVecVal* rtl_wr_val_0, svLogicVecVal* rtl_wr_val_1, svLogicVecVal* rtl_wr_val_2, svLogicVecVal* rtl_wr_val_3,
                         uint32_t rtl_flagwrite_en, uint32_t rtl_flag_val, uint32_t rtl_brk_ch, uint32_t rtl_nblk_id, uint32_t rtl_xdump_ctrl);
    /**
     * \fn void DB_DPI_CmpNbWb (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_nblk_id, svLogicVecVal* rtl_gr_val,
     *                          svLogicVecVal* rtl_wr_val_0, svLogicVecVal* rtl_wr_val_1, svLogicVecVal* rtl_wr_val_2, svLogicVecVal* rtl_wr_val_3)
     * \brief Print RTL info of DPI_CmpNbWb for debugging. There is no CForest info here.
     *
     * @param[in]  rtl_peid                PEID.
     * @param[in]  rtl_time                Time.
     * @param[in]  rtl_cycle               Cycle.
     * @param[in]  rtl_nblk_id             Non-blocking ID.
     * @param[in]  rtl_gr_val              Register value.
     * @param[in]  rtl_wr_val_0            the 1st 32bit.
     * @param[in]  rtl_wr_val_1            the 2nd 32bit.
     * @param[in]  rtl_wr_val_2            the 3rd 32bit.
     * @param[in]  rtl_wr_val_3            the 4th 32bit.
     *
     * @return None.
     */
    void DB_DPI_CmpNbWb (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_nblk_id, svLogicVecVal* rtl_gr_val,
                         svLogicVecVal* rtl_wr_val_0, svLogicVecVal* rtl_wr_val_1, svLogicVecVal* rtl_wr_val_2, svLogicVecVal* rtl_wr_val_3);
    /**
     * \fn GetCmpStage(void)
     * \brief Get stage in which cycle is compared.
     *
     * @param  -
     *
     * @return Stage name.
     */
    PipeStage GetCmpStage (void);
    /**
     * \fn GetCycCmpStartPC(void)
     * \brief Get PC which starts cycle comparison.
     *
     * @param  -
     *
     * @return Started PC.
     */
    uint64_t GetCycCmpStartPC (void);
    /**
     * \fn GetCycCmpEndPC(void)
     * \brief Get PC which ends cycle comparison.
     *
     * @param  -
     *
     * @return Ended PC.
     */
    uint64_t GetCycCmpEndPC (void);
    /**
     * \fn CmpRunnerIF()
     * \brief Constructor of CmpRunnerIF.
     *
     * @param  -
     *
     * @return None.
     */
    CmpRunnerIF();
    /**
     * \fn virtual ~CmpRunnerIF()
     * \brief Destructor of CmpRunnerIF.
     *
     * @param  -
     *
     * @return None.
     */
    virtual ~CmpRunnerIF();

private:
    ForestUtil* m_forest_util;
    TracePrintOperand* m_print_operand;
    XDUMP* m_xdump;
    IDUMP* m_idump;

    FILE* m_logfilep;

    uint32_t m_exope_mem_outstand;
    static const uint32_t NUM_OUTSTAND = 2;

    typedef std::vector<CmpRegInfo*> CmpRegInfoVec;
    CmpRegInfoVec m_reg_info;


public:
    /**
     * \fn IDUMP* GetIDump (void)
     * \brief Get IDUMP object.
     *
     * @param  -
     *
     * @return Pointer to IDUMP object.
     */
    IDUMP* GetIDump (void) { return m_idump; }
    /**
     * \fn XDUMP* GetXDump (void)
     * \brief Get XDUMP object.
     *
     * @param  -
     *
     * @return Pointer to XDUMP object.
     */
    XDUMP* GetXDump (void) { return m_xdump; }

    /**
     * \var bool m_comp_misr_show
     * \brief MISR status variable.
     *
     */
    bool m_comp_misr_show;
    /**
     * \fn bool IsCompMisrShow (void)const
     * \brief Check MISR status.
     *
     * @param  -
     *
     * @return MISR status.
     */
    bool IsCompMisrShow (void)const { return m_comp_misr_show; }

    // Check reg_no is General-purpose register or not.
    bool IsGReg (CReg reg_no);
    // Check reg_no is Vector register or not.
    bool IsWReg (CReg reg_no);
private:
    virtual APIErrorCode AddPEInfo (uint32_t clid, uint32_t pe_id, CoreType core_type) { return API_E_OK; }
    virtual APIErrorCode InstantiatePE (void) { return API_E_OK; }
    virtual APIErrorCode SetPEInfo (uint32_t pe_id, SetPEInfoID pe_info_id,
                                    uint32_t info_data) { return API_E_OK; }
    virtual APIErrorCode GetPEInfo (uint32_t pe_id, GetPEInfoID pe_info_id,
                                    uint32_t* info_data) { return API_E_OK; }
    virtual APIErrorCode StepExecute (TraceInfo** trace_info, ExtTraceInfo** ext_trace_info) { return API_E_OK; }
    virtual APIErrorCode HardReset (SimAttr attr, uint32_t pe_id) { return API_E_OK; }
    virtual APIErrorCode PMemWriteDebug (PhysAddr addr, uint32_t data,
                                         ByteSize size) { return API_E_OK; }
    virtual APIErrorCode PMemReadDebug (PhysAddr addr, ByteSize size,
                                        uint32_t* data) { return API_E_OK; }
    virtual APIErrorCode SetMemBlock (uint32_t id, MemoryBlock mem_block) { return API_E_OK; }
    virtual APIErrorCode GetMemBlock (uint32_t id, MemoryBlock* mem_block) { return API_E_OK; }
    virtual APIErrorCode DeleteMemBlock (uint32_t id) { return API_E_OK; }
    virtual APIErrorCode RegWriteDebug (uint32_t pe_id, CReg regno,
                                        const uint32_t* data) { return API_E_OK; }
    virtual APIErrorCode RegReadDebug (uint32_t pe_id, CReg regno,
                                       uint32_t* data) { return API_E_OK; }
    virtual APIErrorCode SetICacheSpec (uint32_t pe_id, CacheSpec cachespec) { return API_E_OK; }
    virtual APIErrorCode GetICacheSpec (uint32_t pe_id,  CacheSpec* cachespec) { return API_E_OK; }
    virtual APIErrorCode GetICacheInfo (uint32_t pe_id, CacheInfo* cacheinfo) { return API_E_OK; }
    virtual APIErrorCode SetDebugMode  (DebugMode mode) { return API_E_OK; }
    virtual APIErrorCode SetFileOut (const wchar_t* file_pathname) { return API_E_OK; }
    virtual APIErrorCode GetTraceInfo  (TraceInfo** trace) { return API_E_OK; }
    virtual APIErrorCode GetPeTraceInfo (uint32_t pe_id, TraceInfo** trace) { return API_E_OK; }
    virtual APIErrorCode SetModeFrequency(uint32_t pe_id, uint32_t ratio) { return API_E_OK; }
    virtual APIErrorCode SetInterruptToIntc1 (uint32_t pe_id, uint32_t channel) { return API_E_OK; }
    virtual APIErrorCode SetInterruptToIntc2 (uint32_t channel) { return API_E_OK; }
    virtual APIErrorCode SetFeintToIntc1 (uint32_t pe_id, uint32_t channel) { return API_E_OK; }
    virtual APIErrorCode SetFenmiToIntc1 (uint32_t pe_id) { return API_E_OK; }
    virtual APIErrorCode SetIORBaseAddr (SetIORBaseAddrID peripheral_id,
                                         PhysAddr base_addr) { return API_E_OK; }
    virtual APIErrorCode SetLocalIOROffsetAddr (PhysAddr start_address,
                                                PhysAddr end_addr,
                                                int32_t offset) { return API_E_OK; }
    virtual APIErrorCode SetGuard (GuardModuleID guard_id, uint32_t allocation_id,
                                   PhysAddr base_addr, bool is_valid) { return API_E_OK; }
    virtual APIErrorCode SetGuardParam (GuardModuleID guard_id, uint32_t allocation_id,
                                        GuardParamID param_id, uint32_t value) { return API_E_OK; }
};
};
