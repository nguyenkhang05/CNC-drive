/*
 * (c) 2011-2015 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <string>
#include <sstream>
#include <iomanip>
#include "./config.h"
#include "./forest_common.h"
#include "./trace_info.h"
#include "./trace_print.h"
#include "./regid.h"
#include "./cmp_runner_if_g4mh.h"
#include "./cmp_runner_his.h"
#include "./forest_python.h"
#include "./forest_message.h"
#include "./exp_info.h"


/** \file cmp_runner_g4mh.h
 * \brief CompRunner G4MH interface header file.
 *
 * This file contains the DPI common functions in CompRunner: G4MHv2, ...
 */


/**
 * \class ComprunnerG4MHv2
 *
 * \brief G4MH CompRunner class.
 *
 * It is used to contain the DPI functions for ComprunnerG4MHv2.
 * Please enable macro COMP_RUNNER_G4MH for using this class.
 *
 */
class ComprunnerG4MHv2 {

public:
    /**
     * \typedef typedef uint32_t CmpRunnerStatus
     * \brief CompRunner status: There are 5 status in G4MHv2 C side.
     *
     * There are 5 return values in C side.
     *    - 2 values notify the successful execution.
     *    - value notify the mismatch in the comparison result.
     *    - 2 values notify the errors in C side or SV side.
     *
     * \param COMP_DPI_OK                 : It means DPI function in CompRunner process correctly without error, there is no comparison.
     *                                      This return value is usually used in initialization/termination DPI functions.
     *                                      CompRunner SV side can notify next notification to continue comparison.
     * \param COMP_DPI_MATCH              : It means DPI function in CompRunner process correctly without error, the comparison result is also MATCH.
     *                                      CompRunner SV side can notify next notification to continue comparison.
     * \param COMP_DPI_MISMATCH           : It means DPI function in CompRunner process correctly without error, the comparison result is also MISMATCH.
     *                                      The MISMATCH reason can come from RTL or CForest side values, not in CompRunner side.
     *                                      The result of next execution can be wrong. CompRunner log should start with the error keyword "<ERR: ...>".
     * \param COMP_DPI_C_ERR              : It means CompRunner can't proceed the comparison, there is error in CompRunner C side processing.
     *                                      The result of next execution can be wrong. CompRunner log should start with the error keyword "<ERR: ...>".
     * \param COMP_DPI_SV_ERR             : It means CompRunner can't proceed the comparison, there is error in input of CompRunner SV side.
     *                                      The result of next execution can be wrong. CompRunner log should start with the error keyword "<ERR: ...>".
     *
     */
    typedef uint32_t CmpRunnerStatus;
    /**
     * \var ::COMP_DPI_OK
     * \brief It means DPI function in CompRunner process correctly without error, there is no comparison.
     *
     * This return value is usually used in initialization/termination DPI functions.
     * CompRunner SV side can notify next notification to continue comparison.
     *
     */
    static const CmpRunnerStatus COMP_DPI_OK          = 0;
    /**
     * \var ::COMP_DPI_MATCH
     * \brief It means DPI function in CompRunner process correctly without error, the comparison result is also MATCH.
     *
     * CompRunner SV side can notify next notification to continue comparison.
     *
     */
    static const CmpRunnerStatus COMP_DPI_MATCH       = 1;
    /**
     * \var COMP_DPI_MISMATCH
     * \brief It means DPI function in CompRunner process correctly without error, the comparison result is also MISMATCH.
     *
     * This return value is usually used in initialization/termination DPI functions.
     * CompRunner SV side can notify next notification to continue comparison.
     *
     */
    static const CmpRunnerStatus COMP_DPI_MISMATCH    = 2;
    /**
     * \var COMP_DPI_C_ERR
     * \brief It means CompRunner can't proceed the comparison, there is error in CompRunner C side processing.
     *
     * The result of next execution can be wrong. CompRunner log should start with the error keyword "<ERR: ...>".
     *
     */
    static const CmpRunnerStatus COMP_DPI_C_ERR       = 3;
    /**
     * \var COMP_DPI_SV_ERR
     * \brief It means CompRunner can't proceed the comparison, there is error in input of CompRunner SV side.
     *
     * The result of next execution can be wrong. CompRunner log should start with the error keyword "<ERR: ...>".
     *
     */
    static const CmpRunnerStatus COMP_DPI_SV_ERR      = 4;

    /**
     * \fn uint32_t G4MHv2_CForestInit (uint32_t rtl_pe_number, uint32_t cmprunner_log, char *cmprunner_log_path, char *srec_file, char *python_name, uint32_t Cmprunner_Enable);
     * \brief Initialization CompRunner G4MHv2 function, call once at the beginning.
     *
     * This function is called 1 time at the beginning of CompRunner execution.
     * It will initialize values in CompRunner G4MHv2.0. CForest values will be also initialized.
     *
     * @param[in]  rtl_pe_number            SV side will notify the number of pe is initialized in CForest for the comparison.
     *                                      rtl_pe_number should be accepted in [1-8] (In G4MHv2.0, "peid = 0" is still OK).
     *                                      Ex: rtl_pe_number = 2, it means PE0 and PE1 will be initialized.
     * @param[in]  cmprunner_log            SV side will specify the name of CompRunner log.
     *                                       - 1: use cmprunner_log_path as name of log.
     *                                       - 0: default use "cmpcforest.log" name.
     * @param[in]  cmprunner_log_path       SV side will specify name of CompRunner log. This name is specified by RTL run command.
     *                                      It is available when cmprunner_log = 1.
     * @param[in]  srec_file                SV side will specify the path of hex file (srec file). This path is specified by RTL run command.
     * @param[in]  python_name              SV side will specify python file. This file will be configure CForest settings.
     * @param[in]  Cmprunner_Enable         SV side will send signal to enable Cmprunner.
     *                                       - 1: Use Cmprunner
     *                                       - 0: Dont use cmprunner
     * @return Error code
     * @retval #COMP_DPI_OK        : This return value means DPI function in CompRunner process correctly, no issue was occurred.
     *                               CompRunner C side can initialize CForest successfully.
     *                               CompRunner SV side can notify next DPI function to start comparison.
     * @retval #COMP_DPI_C_ERR     : This return value means DPI function in C side got error in processing, some issues were occurred.
     *                               CForest was not initialized successfully.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     * @retval #COMP_DPI_SV_ERR    : This return value means DPI function in SV side got error in processing, some issues were occurred.
     *                               CForest was not initialized successfully.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     */
    uint32_t G4MHv2_CForestInit (uint32_t rtl_pe_number, uint32_t cmprunner_log, char *cmprunner_log_path, char *srec_file, char *python_name, uint32_t Cmprunner_Enable);


    /**
     * \fn void G4MHv2_CForestClose (void);
     * \brief Termination CompRunner G4MHv2 function, call once at the end.
     *
     * This function is called 1 time at the end of CompRunner execution,
     * usually after DPI_GetTotalError is called to show the number of error in final result.
     * It will finalize CompRunner in C side. It is used for releasing memory allocation and closing Cforest.
     *
     * @param  -
     * @return None
     */
    void G4MHv2_CForestClose (void);


    /**
     * \fn uint32_t G4MHv2_GetTotalError (uint32_t rtl_peid)
     * \brief Get total error CompRunner G4MHv2 function, call once at the end.
     *
     * This function is called 1 time for each PE at the end of CompRunner execution.
     * It will show the total error of specified pe in CompRunner G4MHv2.0.
     * CompRunner log must start with the following message "[GetTotalError] ".
     *
     * @param[in]  rtl_peid                 SV side will notify the PE would like to know number of errors to C side.
     *                                      Otherwise, the result is mismatched and the number of errors will be show.
     * @return Error code
     * @retval 0                    : If C side returns value is 0, the result of CompRunner G4MH in this PE is matched.
     * @retval 1-n                  : Otherwise, the result is mismatched and the number of errors will be show.
     */
    uint32_t G4MHv2_GetTotalError (uint32_t rtl_peid);


    /**
     * \fn uint32_t G4MHv2_CmpStep (uint32_t rtl_peid, uint64_t rtl_dp_time, uint64_t rtl_ea_time, uint64_t rtl_cm_time, uint32_t rtl_cycle, uint32_t rtl_pc, uint32_t rtl_last_commit,
     *                              uint32_t rtl_c2b1_commit, uint32_t rtl_ooo_commit, uint32_t rtl_mcyc_commit, uint32_t rtl_regwrite_en,
     *                              uint32_t rtl_reg_num, svLogicVecVal* rtl_gr_val, svLogicVecVal* rtl_wr_val_0, svLogicVecVal* rtl_wr_val_1,
     *                              svLogicVecVal* rtl_wr_val_2, svLogicVecVal* rtl_wr_val_3, uint32_t rtl_flagwrite_en, uint32_t rtl_flag_val,
     *                              uint32_t rtl_brk_ch, uint32_t rtl_nblk_id, uint32_t rtl_xdump_ctrl);
     * \brief Step exeuction CompRunner G4MHv2 function, call when 1 instruction was executed in RTL side.
     *
     * This function is called after 1 instruction was executed normally in RTL side.
     * It will notify to Cforest to execute next instruction.
     *
     * @param[in]  rtl_peid                 SV side will notify the current PE is executed in RTL side.
     *                                      C side will notify to CForest to execute the suitable PE.
     *                                      This info is also showed in CompRunner log.
     * @param[in]  rtl_dp_time              SV side will notify the the DP stage timing in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_ea_time              SV side will notify the the EA stage timing in RTL side.
     *                                      C side will do nothing for this signal.
     * @param[in]  rtl_cm_time              SV side will notify the the CM stage timing in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is used for comparing cycle at CM stage.
     * @param[in]  rtl_cycle                SV side will notify the current cycle in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_pc                   SV side will notify the current PC in RTL side.
     *                                      C side will do the comparison for this value.
     *                                      The comparison result will be showed in CompRunner log.
     * @param[in]  rtl_last_commit          SV side will notify to C side when RTL executed 1 insn completely.
     *                                      - 0: This is an intermediate commit of multi-commit instruction.
     *                                           C side doesn't need to notify to CForest for the execution.
     *                                      - 1: One instruction in RTL side is executed completely.
     *                                           It is can be normal instruction or last commit of multi-commit instruction.
     *                                           C side will notify to CForest for the execution.
     *                                           After that, C side will do the comparison for flag value, register write-back values.
     *                                           The comparison result will be showed in details in CompRunner log.
     * @param[in]  rtl_c2b1_commit          SV side will notify when C2B1 instruction is executed in RTL side.
     *                                      C side will notify to CForest to execute 2 sequential instructions.
     *                                      After that, C side will do the comparison for flag value, register write-back values.
     * @param[in]  rtl_ooo_commit           SV side will notify when out-of-order instruction is executed in RTL side (LD, POPSP, DIPOSE, LDL, SYSCALL, CALLT, LDV, etc..).
     *                                      C side will notify to CForest for the execution.
     *                                      In this case, C side will understand that CForest side will have write-back value before RTL side.
     *                                      After CForest execution is finished, C side will keep write-back value of CForest in CompRunner buffer.
     *                                      So, C side can compare later when write-back value of RTL side is available by DPI_CmpNbWb commit.
     *                                      "rtl_nblk_id" argument will be used to recognize the exactly register index.
     * @param[in]  rtl_mcyc_commit          SV side will notify this commit is a part of multi-cycle insn.
     *                                      C side will keep write-back value of CForest in CompRunner buffer.
     *                                      When the next commit with rtl_last_commit = 1 is sent, C side will do the comparison for all register write-back values.
     * @param[in]  rtl_regwrite_en          SV side will notify to C side there is an register update in RTL side.
     *                                     - 0: There is no register write-back in SV side.
     *                                          C side will do nothing for rtl_reg_num argument.
     *                                     - 1: There is register write-back in SV side.
     *                                          C side will keep value of rtl_reg_num and rtl_gr_val/rtl_wr_val in CompRunner buffer.
     * @param[in]  rtl_reg_num              SV side will notify register number when rtl_regwrite_en = 1.
     *                                      If rtl_reg_num > 32, rtl_reg_num is WR. rtl_reg_num with [64-93] will be WR0-WR31.
     *                                      Otherwise, if rtl_reg_num <= 31, rtl_reg_num is GR. rtl_reg_num with [0-31] will be R0-R31.
     * @param[in]  rtl_gr_val               SV side will notify register write-back value in RTL side.
     *                                      This value is valid if rtl_regwrite_en = 1.
     * @param[in]  rtl_wr_val_0             Value of WR is separated to 4 arguments. SV side will notify each argument one by one.
     *                                      After C side received 4 parts of WR, it will concatenate them to 128bit value and compare with CForest value.
     *                                      This argument is LSB value of WR.
     * @param[in]  rtl_wr_val_1             This argument is one part of WR value.
     * @param[in]  rtl_wr_val_2             This argument is one part of WR value.
     * @param[in]  rtl_wr_val_3             This argument is MSB value of WR.
     * @param[in]  rtl_flagwrite_en         SV side will notify to C side there is an flag (bit[0-4]: SAT bit 4, CY bit 3, OV bit 2, S bit 1, Z bit 0 in PSW) update in RTL side.
     *                                      - 0: this flag is not update.
     *                                      - 1: this flag is update.
     * @param[in]  rtl_flag_val             SV side will notify the flag value in RTL side. This value is only available if the suitable flag in rtl_flagwrite_en is set.
     *                                      ((rtl_flag_val & rtl_flagwrite_en) == (iss_flag_val & iss_flagwrite_en)) ? MATCH : MISMATCH.
     * @param[in]  rtl_brk_ch               SV side will notify the channel value of debug synchronous event.
     *                                      In G4MHv2.0, it must be from 0 to 11.
     * @param[in]  rtl_nblk_id              SV side will notify the non-blocking id for out-of-order insn.
     *                                      C side will use this value to recognize the suitable GR for the comparison in DPI_CmpNbWb later.
     *                                      [0x0 - 0x28] is using for normal insn, [0x100-0x140] is using for FXU insn.
     *                                      nonblk ID is the unique ID at that time, it can be re-used if the old one is deleted.
     * @param[in]  rtl_xdump_ctrl           SV side will notify the xdump control signal for register update in SV side. This signal is just available when last_commit = 1.
     *                                      C side will ignore the comparison for the all GR updates in this instruction. C side will apply value from SV side to CForest.
     *                                      - 0: XDump control signal is disable.
     *                                      - 1: XDump control signal is enable.
     * @return Error code
     * @retval #COMP_DPI_OK        : This return value means DPI function in CompRunner process correctly without error, there is no comparision.
     *                               It returns in case there's a empty commit from RTL or R32 is added to CompRunner buffer.
     *                               CompRunner SV side can notify next notification to continue comparision.
     * @retval #COMP_DPI_MATCH     : This return value means DPI function in CompRunner process correctly without error, the comparision result is aslo MATCH.
     *                               CompRunner SV side can notify next notification to continue comparision.
     * @retval #COMP_DPI_MISMATCH  : This return value means DPI function in CompRunner process correctly, no issue was occurred.
     *                               The comparison between RTL and CForest is MISMATCH. The result of next execution can be wrong.
     *                               CompRunner log should start with the error keyword "<ERR: ...>"
     * @retval #COMP_DPI_SV_ERR    : This return value means DPI function in SV side got error in processing, some issues were occurred.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     */
    uint32_t G4MHv2_CmpStep (uint32_t rtl_peid, uint64_t rtl_dp_time, uint64_t rtl_ea_time, uint64_t rtl_cm_time, uint32_t rtl_cycle, uint32_t rtl_pc, uint32_t rtl_last_commit,
                             uint32_t rtl_c2b1_commit, uint32_t rtl_ooo_commit, uint32_t rtl_mcyc_commit,
                             uint32_t rtl_regwrite_en, uint32_t rtl_reg_num, svLogicVecVal* rtl_gr_val, svLogicVecVal* rtl_wr_val_0, svLogicVecVal* rtl_wr_val_1,
                             svLogicVecVal* rtl_wr_val_2, svLogicVecVal* rtl_wr_val_3, uint32_t rtl_flagwrite_en, uint32_t rtl_flag_val,
                             uint32_t rtl_brk_ch, uint32_t rtl_nblk_id, uint32_t rtl_xdump_ctrl);


    /**
     * \fn uint32_t G4MHv2_CmpNbWb (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_nonblk_id, svLogicVecVal* rtl_gr_val,
     *                              svLogicVecVal* rtl_wr_val_0, svLogicVecVal* rtl_wr_val_1, svLogicVecVal* rtl_wr_val_2, svLogicVecVal* rtl_wr_val_3)
     * \brief Reading memory execution CompRunner G4MHv2 function, call when there is a response for out-of-order reading in RTL side.
     *
     * This function is called when there was a response for out-of-order reading in RTL side.
     * The value of rtl_nonblk_id is matched with rtl_nblk_id in previous DPI_CmpStep commit.
     * C side will depend on rtl_nonblk_id argument to recognize which register index will be updated in CompRunner buffer, it can be GR or WR.
     * C side will do the comparison or update CompRunner buffer depends on the register status.
     * After that, the result will be showed in CompRunner log.
     *
     * @param[in]  rtl_peid                 SV side will notify the current PE is executed in RTL side.
     *                                      C side will depend on this info to understand which out-of-order info will be updated.
     * @param[in]  rtl_time                 SV side will notify the the current timing in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_cycle                SV side will notify the current cycle in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_nonblk_id            SV side will notify nonblocking id of out-of-order commit to C.
     *                                      This is the unique ID for out-of-order commit and is created by G4MHv2 RTL env:
     *                                      [0x0 - 0x28] for normal insn, [0x100-0x140] for FXU insn.
     *                                      C side will depend on this info to proceed the update for CompRunner status.
     *                                      NonBlk ID is unique ID here because SV side can't know the register index at this time.
     *                                      It is also the unique ID at each time, it can be re-used if the old one is deleted.
     * @param[in]  rtl_gr_val               SV side will notify the suitable value of nonblk ID.
     * @param[in]  rtl_wr_val_0             Value of WR is separated to 4 arguments. SV side will notify each argument one by one.
     *                                      After C side received 4 parts of WR, it will concatenate them to 128bit value and compare with CForest value.
     *                                      This argument is LSB value of WR.
     * @param[in]  rtl_wr_val_1             This argument is one part of WR value.
     * @param[in]  rtl_wr_val_2             This argument is one part of WR value.
     * @param[in]  rtl_wr_val_3             This argument is MSB value of WR.
     * @return Error code
     * @retval #COMP_DPI_MATCH     : This return value means DPI function in CompRunner process correctly without error, the comparision result is aslo MATCH.
     *                               CompRunner SV side can notify next notification to continue comparision.
     * @retval #COMP_DPI_MISMATCH  : This return value means DPI function in CompRunner process correctly, no issue was occurred.
     *                               The comparison between RTL and CForest is MISMATCH or C side can't update CompRunner buffer successfully.
     *                               The result of next execution can be wrong.
     *                               CompRunner log should start with the error keyword "<ERR: ...>"
     * @retval #COMP_DPI_SV_ERR    : This return value means DPI function in SV side got error in processing, some issues were occurred.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     */
    uint32_t G4MHv2_CmpNbWb (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_nonblk_id, svLogicVecVal* rtl_gr_val,
                             svLogicVecVal* rtl_wr_val_0, svLogicVecVal* rtl_wr_val_1, svLogicVecVal* rtl_wr_val_2, svLogicVecVal* rtl_wr_val_3);


    /**
     * \fn uint32_t G4MHv2_ExpSyserrNotify (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc, uint32_t rtl_syserr_cause,
     *                                      uint32_t rtl_operand_syserr, uint32_t rtl_operand_syserr_mode, uint32_t rtl_operand_syserr_gpid);
     * \brief SysErr asynchronous exception notification CompRunner G4MHv2 function, call when this event was occurred in RTL side.
     *
     * This function is called when there is an SYSERR exception was occurred in RTL side.
     * C side will notify to CForest go to the same situation.
     * C side will compare PC value and any values if neccesary.
     * After that, the result will be showed in CompRunner log.
     *
     *
     * @param[in]  rtl_peid                 SV side will notify the current PE is executed in RTL side.
     *                                      C side will depend on this info to understand which out-of-order info will be updated.
     * @param[in]  rtl_time                 SV side will notify the the current timing in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_cycle                SV side will notify the current cycle in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_exp_pc               SV side will notify the exception PC in RTL side.
     *                                      C side will compare this PC value with CForest side.
     * @param[in]  rtl_syserr_cause         SV side will notify the cause code of Syserr, from 0x10 to 0x1F.
     *                                      C side will check the condition for this PC value.
     *                                      If it is OK, C side will show PC info to CompRunner log.
     * @param[in]  rtl_operand_syserr       SV side will notify whether syserr is operand systemerr or not
     *                                       0: is not operand syserr
     *                                       1: is operand syserr
     * @param[in]  rtl_operand_syserr_mode  SV will notify mode of operand syserr
     *                                       0: is host mode or conventional mode (modes which syserr have same behavior)
     *                                       1: is guest mode
     * @param[in]  rtl_operand_syserr_gpid  SV will notify gpid of operand syserr. The value wil run from 0 to 7
     *
     * @return Error code
     * @retval #COMP_DPI_MATCH     : This return value means DPI function in CompRunner process correctly without error, the comparision result is aslo MATCH.
     *                               CompRunner SV side can notify next notification to continue comparision.
     * @retval #COMP_DPI_MISMATCH  : This return value means DPI function in CompRunner process correctly, no issue was occurred.
     *                               The comparison between RTL and CForest is MISMATCH. The result of next execution can be wrong.
     *                               CompRunner log should start with the error keyword "<ERR: ...>"
     * @retval #COMP_DPI_SV_ERR    : This return value means DPI function in SV side got error in processing, some issues were occurred.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     */
    uint32_t G4MHv2_ExpSyserrNotify (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc, uint32_t rtl_syserr_cause,
                                     uint32_t rtl_operand_syserr, uint32_t rtl_operand_syserr_mode, uint32_t rtl_operand_syserr_gpid, uint32_t rtl_bus_slave_xdmp);

    /**
     * \fn uint32_t uint32_t G4MHv2_ExpOperandSyserrPend (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc,
     *                                            uint32_t rtl_operand_syserr_cause, uint32_t rtl_operand_syserr_mode, uint32_t rtl_operand_syserr_gpid);
     * \brief SysErr asynchronous exception pending CompRunner G4MHv2 function, call when this event was occurred in RTL side.
     *
     * This function is called when there is an SYSERR exception is pended in RTL side.
     * C side will notify to CForest go to the same situation.
     * C side will compare PC value and any values if neccesary.
     * After that, the result will be showed in CompRunner log.
     *
     *
     * @param[in]  rtl_peid                 SV side will notify the current PE is executed in RTL side.
     *                                      C side will depend on this info to understand which out-of-order info will be updated.
     * @param[in]  rtl_time                 SV side will notify the the current timing in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_cycle                SV side will notify the current cycle in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_exp_pc               SV side will notify the exception PC in RTL side.
     *                                      C side will compare this PC value with CForest side.
     * @param[in]  rtl_operand_syserr_cause  SV side will notify the cause code of Syserr, from 0x10 to 0x1F.
     *                                      C side will check the condition for this PC value.
     *                                      If it is OK, C side will show PC info to CompRunner log.
     * @param[in]  rtl_operand_syserr_mode  SV will notify mode of operand syserr
     *                                       0: is host mode or conventional mode (modes which syserr have same behavior)
     *                                       1: is guest mode
     * @param[in]  rtl_operand_syserr_gpid  SV will notify gpid of operand syserr. The value wil run from 0 to 7
     *
     * @return Error code
     * @retval COMP_DPI_OK         : This return value means DPI function in CompRunner process correctly, no issue was occurred.
     *                               The comparison between RTL and CForest is also MATCH. CompRunner SV side can notify next execution to continue comparison.
     * @retval #COMP_DPI_MISMATCH  : This return value means DPI function in CompRunner process correctly, no issue was occurred.
     *                               The comparison between RTL and CForest is MISMATCH. The result of next execution can be wrong.
     *                               CompRunner log should start with the error keyword "<ERR: ...>"
     * @retval #COMP_DPI_SV_ERR    : This return value means DPI function in SV side got error in processing, some issues were occurred.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     */
    uint32_t G4MHv2_ExpOperandSyserrPend (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc,
                                     uint32_t rtl_operand_syserr_cause, uint32_t rtl_operand_syserr_mode, uint32_t rtl_operand_syserr_gpid, uint32_t rtl_commit, uint32_t rtl_bus_slave_xdmp);

    /**
     * \fn uint32_t uint32_t G4MHv2_HaltSnoozeReleaseDump (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_Halt_dump_en)
     * \brief Halt/Snooze Release Idump CompRunner G4MHv2 function, call when RTL side release Halt/Snooze state
     *
     * This function is called when there is a Release Halt/Snooze in RTL side.
     * C side will notify to CForest go to the same situation - Release Halt/Snooze state.
     *
     * @param[in]  rtl_peid                 SV side will notify the current PE is executed in RTL side.
     *                                      C side will depend on this info to understand which out-of-order info will be updated.
     * @param[in]  rtl_time                 SV side will notify the the current timing in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_cycle                SV side will notify the current cycle in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_Halt_dump_en         Halt/Snooze Release Idump enable
     *                                      If no enable signal, this function can not work
     * @return Error code
     * @retval COMP_DPI_OK         : This return value means DPI function in CompRunner process correctly, no issue was occurred.
     *                               The comparison between RTL and CForest is also MATCH. CompRunner SV side can notify next execution to continue comparison.
     * @retval #COMP_DPI_SV_ERR    : This return value means DPI function in SV side got error in processing, some issues were occurred.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     */

    uint32_t G4MHv2_HaltSnoozeReleaseDump (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_Halt_dump_en);

    /**
     * \fn uint32_t G4MHv2_ExpFeIntc1ReqToCpu (uint32_t rtl_peid, uint32_t rtl_exp_mode_id, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc,
     *                               uint32_t rtl_fe_type, uint32_t rtl_feint_ch, uint32_t rtl_bge_val, uint32_t rtl_intc1_en)
     * \brief FENMI/FEINT/GMFEINT/BGFEINT request notification CompRunner G4MHv2 function, call when this event was occurred in RTL side.
     *
     * This function is used to notify when there is an FEINT/GMFEINT/BGFEINT request from RTL side (FENMI doesn't have this kind of commit).
     * SV side can call it many times if there is the request from INTC module and CPU didn't still accept it.
     * This function will be stopped when DPI_ExpFeCpuAckToIntc1 is called with this FE interruption.
     *
     * @param[in]  rtl_peid                 SV side will notify the current PE is executed in RTL side.
     *                                      C side will depend on this info to understand which out-of-order info will be updated.
     * @param[in]  rtl_exp_mode_id          SV side will notify the host id/guess id of this exception.
     *                                      GuessID = [0,7], HostID = 256.
     *                                      This is a new argument to support virtualization function.
     * @param[in]  rtl_time                 SV side will notify the the current timing in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_cycle                SV side will notify the current cycle in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_exp_pc               SV side will notify the exception PC in RTL side.
     *                                      C side will compare this PC value with CForest side.
     * @param[in]  rtl_fe_type              SV side will notify the interruption type in FE level.
     *                                      1: FEINT, 2: GMFEINT, 3: BGFEINT, 0: FENMI.
     *                                      C side will depend on the committed FE type to control CForest to the right FE level type..
     * @param[in]  rtl_feint_ch             SV side will notify the channel of FEINT type, from channel 0 to channel 15 in G4MHv2.
     *                                      C side will send the suitable channel with specified interruption type.
     * @param[in]  rtl_bge_val              SV side will notify the value of 16 bit BGE in RTL side.
     *                                      C side will compare BGE value between RTL and CForest if rtl_intc1_en = 1, otherwise, this bge value is unused.
     *                                      If the BGE value is mismatch, C side will stop proceeding FE interruption and return error code COMP_DPI_MISMATCH.
     * @param[in]  rtl_intc1_en             SV side will notify if there is an INTC1 module in RTL side or not.
     *                                    - 0: there is no INTC1 module in RTL side.
     *                                         C side will notify to INTC1 module in CForest to execute FE request.
     *                                         There is no checking for BGE value between RTL and CForest.
     *                                    - 1: INTC1 module is existent in RTL side.
     *                                         C side will notify to CPU in CForest to execute FE request.
     *                                         BGE value will be checked at first between RTL and CForest.
     * @return Error code
     * @retval #COMP_DPI_MATCH     : This return value means DPI function in CompRunner process correctly without error, the comparision result is aslo MATCH.
     *                               CompRunner SV side can notify next notification to continue comparision.
     * @retval #COMP_DPI_MISMATCH  : This return value means DPI function in CompRunner process correctly, no issue was occurred.
     *                               The comparison between RTL and CForest is MISMATCH. The result of next execution can be wrong.
     *                               CompRunner log should start with the error keyword "<ERR: ...>"
     * @retval #COMP_DPI_SV_ERR    : This return value means DPI function in SV side got error in processing, some issues were occurred.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     */
    uint32_t G4MHv2_ExpFeIntc1ReqToCpu (uint32_t rtl_peid, uint32_t rtl_exp_mode_id, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc,
                                        uint32_t rtl_fe_type, uint32_t rtl_feint_ch, uint32_t rtl_bge_val, uint32_t rtl_intc1_en, uint32_t rtl_commit);


    /**
     * \fn uint32_t G4MHv2_ExpFeCpuAckToIntc1 (uint32_t rtl_peid, uint32_t rtl_exp_mode_id, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc,
     *                               uint32_t rtl_fe_type, uint32_t rtl_feint_ch, uint32_t rtl_bge_val, uint32_t rtl_intc1_en)
     * \brief FENMI/FEINT/GMFEINT/BGFEINT acknowledgment notification CompRunner G4MHv2 function, call when this event was occurred in RTL side.
     *
     * This function is used to notify when there is an FEINT/GMFEINT/BGFEINT/FENMI acknowledgment from RTL side.
     * SV side will call this function one time for acknowledgment from CPU to request from INTC module.
     *
     * @param[in]  rtl_peid                 SV side will notify the current PE is executed in RTL side.
     *                                      C side will depend on this info to understand which out-of-order info will be updated.
     * @param[in]  rtl_exp_mode_id          SV side will notify the host id/guess id of this exception.
     *                                      GuessID = [0,7], HostID = 256.
     *                                      This is a new argument to support virtualization function.
     * @param[in]  rtl_time                 SV side will notify the the current timing in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_cycle                SV side will notify the current cycle in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_exp_pc               SV side will notify the exception PC in RTL side.
     *                                      C side will compare this PC value with CForest side.
     * @param[in]  rtl_fe_type              SV side will notify the interruption type in FE level.
     *                                      1: FEINT, 2: GMFEINT, 3: BGFEINT, 0: FENMI.
     *                                      C side will depend on the committed FE type to control CForest to the right FE level type.
     * @param[in]  rtl_feint_ch             SV side will notify the channel of FEINT type, from channel 0 to channel 15 in G4MHv2.
     *                                      C side will send the suitable channel with specified interruption type.
     * @param[in]  rtl_bge_val              SV side will notify the value of 16 bit BGE in RTL side.
     *                                      C side will compare BGE value between RTL and CForest if rtl_intc1_en = 1, otherwise, this bge value is unused.
     *                                      If the BGE value is mismatch, C side will stop proceeding FE interruption and return error code COMP_DPI_MISMATCH.
     * @param[in]  rtl_intc1_en             SV side will notify if there is an INTC1 module in RTL side or not.
     *                                    - 0: there is no INTC1 module in RTL side.
     *                                         C side will notify to INTC1 module in CForest to execute FE request.
     *                                         There is no checking for BGE value between RTL and CForest.
     *                                    - 1: INTC1 module is existent in RTL side.
     *                                         C side will notify to CPU in CForest to execute FE request.
     *                                         BGE value will be checked at first between RTL and CForest.
     * @return Error code
     * @retval #COMP_DPI_MATCH     : This return value means DPI function in CompRunner process correctly without error, the comparision result is aslo MATCH.
     *                               CompRunner SV side can notify next notification to continue comparision.
     * @retval #COMP_DPI_MISMATCH  : This return value means DPI function in CompRunner process correctly, no issue was occurred.
     *                               The comparison between RTL and CForest is MISMATCH. The result of next execution can be wrong.
     *                               CompRunner log should start with the error keyword "<ERR: ...>"
     * @retval #COMP_DPI_SV_ERR    : This return value means DPI function in SV side got error in processing, some issues were occurred.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     */
    uint32_t G4MHv2_ExpFeCpuAckToIntc1 (uint32_t rtl_peid, uint32_t rtl_exp_mode_id, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc, uint32_t rtl_fe_type,
                                        uint32_t rtl_feint_ch, uint32_t rtl_bge_val, uint32_t rtl_intc1_en);


    /**
     * \fn uint32_t G4MHv2_ExpEiIntc1ReqToCpu (uint32_t rtl_peid, uint32_t rtl_exp_mode_id, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc,
     *                                  uint32_t rtl_ei_type, uint32_t rtl_int_ch, uint32_t rtl_int_pri, uint32_t rtl_bgpr_val,
     *                                  uint32_t rtl_eitb_read, uint32_t rtl_intc1_en);
     * \brief EIINT/GMEIINT/BGEIINT request notification CompRunner G4MHv2 function, call when this event was occurred in RTL side.
     *
     * This function is used to notify when there is an EIINT/GMEIINT/BGEIINT request from RTL side.
     * SV side can call it many times if there is the request from INTC module and CPU didn't still accept it.
     * This function will be stopped when DPI_ExpEiCpuAckToIntc1 is called.
     *
     *
     * @param[in]  rtl_peid                 SV side will notify the current PE is executed in RTL side.
     *                                      C side will depend on this info to understand which out-of-order info will be updated.
     * @param[in]  rtl_exp_mode_id          SV side will notify the host id/guess id of this exception.
     *                                      GuessID = [0,7], HostID = 256.
     *                                      This is a new argument to support virtualization function.
     * @param[in]  rtl_time                 SV side will notify the the current timing in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_cycle                SV side will notify the current cycle in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_exp_pc               SV side will notify the exception PC in RTL side.
     *                                      C side will compare this PC value with CForest side.
     * @param[in]  rtl_ei_type              SV side will notify the interruption type in EI level.
     *                                      0: EIINT, 1: GMEIINT, 2: BGEIINT.
     *                                      C side will depend on the committed EI type to control CForest to the right EI level type.
     * @param[in]  rtl_int_ch               SV side will notify the channel of EIINT type, from channel 0 to channel 511 (or 2047) in G4MHv2.
     *                                      C side will send the suitable channel with specified interruption type.
     * @param[in]  rtl_int_pri              SV side will notify the priority of EIINT type, from priority 0 to priority l5.
     *                                      C side will send the suitable priority with specified interruption type.
     * @param[in]  rtl_bgpr_val             SV side will notify the value of BGPR in RTL side.
     *                                      C side will compare bgpr value between RTL and CForest if rtl_intc1_en = 1, otherwise, this bgpr value is unused.
     *                                      If the bgpr value is mismatch, C side will stop proceeding EI interruption and return error code COMP_DPI_MISMATCH.
     * @param[in]  rtl_eitb_read            SV side will notify EIINT table is read when EIINT is occurred or not.
     *                                      0: using EIINT table.
     *                                      1: not using EIINT table.
     * @param[in]  rtl_intc1_en             SV side will notify if there is an INTC1 module in RTL side or not.
     *                                    - 0: there is no INTC1 module in RTL side.
     *                                         C side will notify to INTC1 module in CForest to execute EI request.
     *                                         There is no checking for BGBR value between RTL and CForest.
     *                                    - 1: INTC1 module is existent in RTL side.
     *                                         C side will notify to CPU in CForest to execute EI request.
     *                                         BGBR value will be checked at first between RTL and CForest.
     * @return Error code
     * @retval #COMP_DPI_MATCH     : This return value means DPI function in CompRunner process correctly without error, the comparision result is aslo MATCH.
     *                               CompRunner SV side can notify next notification to continue comparision.
     * @retval #COMP_DPI_MISMATCH  : This return value means DPI function in CompRunner process correctly, no issue was occurred.
     *                               The comparison between RTL and CForest is MISMATCH. The result of next execution can be wrong.
     *                               CompRunner log should start with the error keyword "<ERR: ...>"
     * @retval #COMP_DPI_SV_ERR    : This return value means DPI function in SV side got error in processing, some issues were occurred.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     */
    uint32_t G4MHv2_ExpEiIntc1ReqToCpu (uint32_t rtl_peid, uint32_t rtl_exp_mode_id, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc, uint32_t rtl_ei_type,
                                       uint32_t rtl_int_ch, uint32_t rtl_int_pri, uint32_t rtl_bgpr_val, uint32_t rtl_eitb_read, uint32_t rtl_intc1_en, uint32_t rtl_commit);


    /**
     * \n uint32_t G4MHv2_ExpEiCpuAckToIntc1 (uint32_t rtl_peid, uint32_t rtl_exp_mode_id, uint64_t rtl_time, uint32_t rtl_cycle,
     *                                 uint32_t rtl_exp_pc, uint32_t rtl_ei_type, svLogicVecVal* rtl_int_ch, svLogicVecVal* rtl_int_pri,
     *                                 uint32_t rtl_bgpr_val, uint32_t rtl_eitb_read, uint32_t rtl_intc1_en);
     * \brief EIINT/GMEIINT/BGEIINT request notification CompRunner G4MHv2 function, call when this event was occurred in RTL side.
     *
     * This function is used to notify when there is an EIINT/GMEIINT/BGEIINT acknowledgment from RTL side.
     * SV side will call this function one time for acknowledgment from CPU to request from INTC module.
     *
     * @param[in]  rtl_peid                 SV side will notify the current PE is executed in RTL side.
     *                                      C side will depend on this info to understand which out-of-order info will be updated.
     * @param[in]  rtl_exp_mode_id          SV side will notify the host id/guess id of this exception.
     *                                      GuessID = [0,7], HostID = 256.
     *                                      This is a new argument to support virtualization function.
     * @param[in]  rtl_time                 SV side will notify the the current timing in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_cycle                SV side will notify the current cycle in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_exp_pc               SV side will notify the exception PC in RTL side.
     *                                      C side will compare this PC value with CForest side.
     * @param[in]  rtl_ei_type              SV side will notify the interruption type in EI level.
     *                                      0: EIINT, 1: GMEIINT, 2: BGEIINT.
     *                                      C side will depend on the committed EI type to control CForest to the right EI level type.
     * @param[in]  rtl_int_ch               SV side will notify the channel of EIINT type, from channel 0 to channel 511 (or 2047) in G4MHv2.
     *                                      C side will send the suitable channel with specified interruption type.
     * @param[in]  rtl_int_pri              SV side will notify the priority of EIINT type, from priority 0 to priority l5.
     *                                      C side will send the suitable priority with specified interruption type.
     * @param[in]  rtl_bgpr_val             SV side will notify the value of BGPR in RTL side.
     *                                      C side will compare bgpr value between RTL and CForest if rtl_intc1_en = 1, otherwise, this bgpr value is unused.
     *                                      If the bgpr value is mismatch, C side will stop proceeding EI interruption and return error code COMP_DPI_MISMATCH.
     * @param[in]  rtl_eitb_read            SV side will notify EIINT table is read when EIINT is occurred or not.
     *                                      0: using EIINT table.
     *                                      1: not using EIINT table.
     * @param[in]  rtl_intc1_en             SV side will notify if there is an INTC1 module in RTL side or not.
     *                                    - 0: there is no INTC1 module in RTL side.
     *                                         C side will notify to INTC1 module in CForest to execute EI request.
     *                                         There is no checking for BGBR value between RTL and CForest.
     *                                    - 1: INTC1 module is existent in RTL side.
     *                                         C side will notify to CPU in CForest to execute EI request.
     *                                         BGBR value will be checked at first between RTL and CForest.
      * @return Error code
     * @retval #COMP_DPI_MATCH     : This return value means DPI function in CompRunner process correctly without error, the comparision result is aslo MATCH.
     *                               CompRunner SV side can notify next notification to continue comparision.
     * @retval #COMP_DPI_MISMATCH  : This return value means DPI function in CompRunner process correctly, no issue was occurred.
     *                               The comparison between RTL and CForest is MISMATCH. The result of next execution can be wrong.
     *                               CompRunner log should start with the error keyword "<ERR: ...>"
     * @retval #COMP_DPI_SV_ERR    : This return value means DPI function in SV side got error in processing, some issues were occurred.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     */
    uint32_t G4MHv2_ExpEiCpuAckToIntc1 (uint32_t rtl_peid, uint32_t rtl_exp_mode_id, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc, uint32_t rtl_ei_type,
                                       svLogicVecVal* rtl_int_ch, svLogicVecVal* rtl_int_pri, uint32_t rtl_bgpr_val, uint32_t rtl_eitb_read, uint32_t rtl_intc1_en);


    /**
     * \fn uint32_t G4MHv2_ExpDbintRcuReqToCpu (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle,
     *                                     uint32_t rtl_exp_pc, svLogicVecVal* rtl_brk_ch)
     * \brief Debug asynchronous exception notification CompRunner G4MHv2 function, call when this event was occurred in RTL side.
     *
     * This function is used to notify when ther is a DBINT reques from RTL side.
     * SV side can call it many times if there is a request from RCU and CPU didn't still accept it.
     * This function will be stopped when DPI_ExpDbCpuAckToRcu is called.
     *
     * @param[in]  rtl_peid                 SV side will notify the current PE is executed in RTL side.
     *                                      C side will depend on this info to understand which out-of-order info will be updated.
     * @param[in]  rtl_time                 SV side will notify the the current timing in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_cycle                SV side will notify the current cycle in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_exp_pc               SV side will notify the exception PC in RTL side.
     *                                      C side will compare this PC value with CForest side.
     * @param[in]  rtl_brk_ch               SV side will specify break channel of DBINT in RTL, it is from 0 to 15.
     *                                      C side will use pass this argument to Debug notification in CForest.
     * @return Error code
     * @retval #COMP_DPI_OK        : This return value means DPI function in CompRunner process correctly without error, there is no comparision.
     *                               CompRunner SV side can notify next notification to continue comparision.
     * @retval #COMP_DPI_SV_ERR    : This return value means DPI function in SV side got error in processing, some issues were occurred.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     */
    uint32_t G4MHv2_ExpDbintRcuReqToCpu (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc,
                                        svLogicVecVal* rtl_brk_ch, uint32_t rtl_commit);


    /**
     * \fn uint32_t G4MHv2_ExpDbCpuAckToRcu (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle,
     *                                     uint32_t rtl_exp_pc, svLogicVecVal* rtl_db_int_type, svLogicVecVal* rtl_brk_ch)
     * \brief Debug asynchronous exception notification CompRunner G4MHv2 function, call when this event was occurred in RTL side.
     *
     * This function is called when there is a DBINT/DBNMI acknowledgement from RTL side.
     * SV side will call this fuction on time for acknowledgement from CPU to request from RCU.
     *
     * @param[in]  rtl_peid                 SV side will notify the current PE is executed in RTL side.
     *                                      C side will depend on this info to understand which out-of-order info will be updated.
     * @param[in]  rtl_time                 SV side will notify the the current timing in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_cycle                SV side will notify the current cycle in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_exp_pc               SV side will notify the exception PC in RTL side.
     *                                      C side will compare this PC value with CForest side.
     * @param[in]  rtl_db_int_type          SV side will notify the debug interruption type: 0 - DBNMI, 1 - DBINT.
     *                                      C side will use the suitable function to notify to CForest side.
     * @param[in]  rtl_brk_ch               SV side will specify break channel of DBINT in RTL, it is from 0 to 15.
     *                                      C side will use pass this argument to Debug notification in CForest.
     * @return Error code
     * @retval #COMP_DPI_MATCH     : This return value means DPI function in CompRunner process correctly without error, the comparision result is aslo MATCH.
     *                               CompRunner SV side can notify next notification to continue comparision.
     * @retval #COMP_DPI_MISMATCH  : This return value means DPI function in CompRunner process correctly, no issue was occurred.
     *                               The comparison between RTL and CForest is MISMATCH. The result of next execution can be wrong.
     *                               CompRunner log should start with the error keyword "<ERR: ...>"
     * @retval #COMP_DPI_SV_ERR    : This return value means DPI function in SV side got error in processing, some issues were occurred.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     */
    uint32_t G4MHv2_ExpDbCpuAckToRcu (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc,
                                     svLogicVecVal* rtl_db_int_type, svLogicVecVal* rtl_brk_ch);

    /**
     * \fn uint32_t G4MHv2_CancelEiintReq(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle)
     * \brief Eiint request falls without ack
     *
     * This function is called when Eiint request signal falls without ack (by force model)
     * SV side will call this fuction on time for cancel Eiint request in CPU
     *
     * @param[in]  rtl_peid                 SV side will notify the current PE is executed in RTL side.
     *                                      C side will depend on this info to understand which out-of-order info will be updated.
     * @param[in]  rtl_time                 SV side will notify the the current timing in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_cycle                SV side will notify the current cycle in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @return Error code
     * @retval #COMP_DPI_OK        : This return value means DPI function in CompRunner process correctly without error, there is no comparision.
     *                               CompRunner SV side can notify next notification to continue comparision.
     * @retval #COMP_DPI_SV_ERR    : This return value means DPI function in SV side got error in processing, some issues were occurred.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     */
    uint32_t G4MHv2_CancelEiintReq(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle);

    /**
     * \fn uint32_t G4MHv2_CancelFeintReq(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle)
     * \brief Feint request falls without ack
     *
     * This function is called when Feint request signal falls without ack (by force model)
     * SV side will call this fuction on time for cancel Feint request in CPU
     *
     * @param[in]  rtl_peid                 SV side will notify the current PE is executed in RTL side.
     *                                      C side will depend on this info to understand which out-of-order info will be updated.
     * @param[in]  rtl_time                 SV side will notify the the current timing in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_cycle                SV side will notify the current cycle in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @return Error code
     * @retval #COMP_DPI_OK        : This return value means DPI function in CompRunner process correctly without error, there is no comparision.
     *                               CompRunner SV side can notify next notification to continue comparision.
     * @retval #COMP_DPI_SV_ERR    : This return value means DPI function in SV side got error in processing, some issues were occurred.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     */
    uint32_t G4MHv2_CancelFeintReq(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle);

    /**
     * \fn uint32_t G4MHv2_CancelFenmiReq(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle)
     * \brief Fenmi request falls without ack
     *
     * This function is called when Fenmi request signal falls without ack (by force model)
     * SV side will call this fuction on time for cancel Fenmi request in CPU
     *
     * @param[in]  rtl_peid                 SV side will notify the current PE is executed in RTL side.
     *                                      C side will depend on this info to understand which out-of-order info will be updated.
     * @param[in]  rtl_time                 SV side will notify the the current timing in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_cycle                SV side will notify the current cycle in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @return Error code
     * @retval #COMP_DPI_OK        : This return value means DPI function in CompRunner process correctly without error, there is no comparision.
     *                               CompRunner SV side can notify next notification to continue comparision.
     * @retval #COMP_DPI_SV_ERR    : This return value means DPI function in SV side got error in processing, some issues were occurred.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     */
    uint32_t G4MHv2_CancelFenmiReq(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle);

    /**
     * \fn uint32_t G4MHv2_CancelDbintReq(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle)
     * \brief Dbint request falls without ack
     *
     * This function is called when Dbint request signal falls without ack (by force model)
     * SV side will call this fuction on time for cancel Dbint request in CPU
     *
     * @param[in]  rtl_peid                 SV side will notify the current PE is executed in RTL side.
     *                                      C side will depend on this info to understand which out-of-order info will be updated.
     * @param[in]  rtl_time                 SV side will notify the the current timing in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_cycle                SV side will notify the current cycle in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @return Error code
     * @retval #COMP_DPI_OK        : This return value means DPI function in CompRunner process correctly without error, there is no comparision.
     *                               CompRunner SV side can notify next notification to continue comparision.
     * @retval #COMP_DPI_SV_ERR    : This return value means DPI function in SV side got error in processing, some issues were occurred.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     */
    uint32_t G4MHv2_CancelDbintReq(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle);

    /**
     * \fn uint32_t G4MHv2_ResetPE(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc)
     * \brief Reset PE
     *
     * This function is used to proceed the individual PE reset for specified PE.
     * It also reset EIINT table and MCYC status to initial values after individual PE reset execution.
     *
     * @param[in]  rtl_peid                 SV side will notify the current PE is executed in RTL side.
     *                                      C side will depend on this info to reset the specified individual PE.
     * @param[in]  rtl_time                 SV side will notify the the current timing in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_cycle                SV side will notify the current cycle in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_exp_pc               SV side will notify the exception PC in RTL side.
     *                                      C side will compare this PC value with CForest side.
     * @return Error code
     * @retval #COMP_DPI_OK        : This return value means DPI function in CompRunner process correctly without error, there is no comparision.
     *                               CompRunner SV side can notify next notification to continue comparision.
     * @retval #COMP_DPI_SV_ERR    : This return value means DPI function in SV side got error in processing, some issues were occurred.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     */
    uint32_t G4MHv2_ResetPE(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc);

    /**
     * \n uint32_t G4MHv2_CmpSReg (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle,
     *                             svLogicVecVal* rtl_sreg_id, svLogicVecVal* rtl_sreg_ch,
     *                             svLogicVecVal* rtl_sreg_val, svLogicVecVal* rtl_sreg_mask);
     * \brief System register comparison CompRunner G4MHv2 function.
     *
     * This function is used to compare system register value between RTL and CForest.
     * SV side can call it any time (after initial settings and before stop simulation) for checking values between RTL and CForest.
     * If miss-match is detected, C side will notice error in CompRunner log.
     *
     * @param[in]  rtl_peid                 SV side will notify the current PE is executed in RTL side.
     *                                      C side will depend on this info to understand which out-of-order info will be updated.
     * @param[in]  rtl_time                 SV side will notify the the current timing in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_cycle                SV side will notify the current cycle in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_sreg_id              SV side will notify selID and regID of system register for checking.
     *                                      Bit field of this info: selID = bit [0, 4], regID = bit [5, 9]
     * @param[in]  rtl_sreg_ch              SV side will notify the channel number in case MPLA/MPUA/MPAT/BPC/BPAV/BPAM. (For others, specify 0.)
     *                                      Because in G4MH, there are many system registers have the same selID and regID, but different channel.
     * @param[in]  rtl_sreg_val             SV side will notify value of specified system register in RTL side.
     *                                      C side will compare value of RTL side with CForest side.
     *                                      The comparison result will be showed in CompRunner log.
     * @param[in]  rtl_sreg_mask            SV side will notify mask for comparison.
     *                                      C side will compare only the bits which have 1 in the mask and will ignore the bits which have 0 in the mask.
     * @return Error code
     * @retval #COMP_DPI_MATCH     : This return value means DPI function in CompRunner process correctly without error, the comparision result is aslo MATCH.
     *                               CompRunner SV side can notify next notification to continue comparision.
     * @retval #COMP_DPI_MISMATCH  : This return value means DPI function in CompRunner process correctly, no issue was occurred.
     *                               The comparison between RTL and CForest is MISMATCH. The result of next execution can be wrong.
     *                               CompRunner log should start with the error keyword "<ERR: ...>"
     * @retval #COMP_DPI_SV_ERR    : This return value means DPI function in SV side got error in processing, some issues were occurred.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     * @retval #COMP_DPI_C_ERR     : This return value mean DPI function in C side got error in processing, some issues were occured by reading system register.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     */
    uint32_t G4MHv2_CmpSReg (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle,
                          svLogicVecVal* rtl_sreg_id, svLogicVecVal* rtl_sreg_ch,
                          svLogicVecVal* rtl_sreg_val, svLogicVecVal* rtl_sreg_mask);


    /**
     * \fn uint32_t G4MHv2_MemoryDataCheck (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, svLogicVecVal* rtl_mem_addr, svLogicVecVal* rtl_mem_val)
     * \brief Memory checking CompRunner G4MHv2 function.
     *
     * This function is used to check 4 byte memory data with address and RTL data.
     * SV side can call it any time (after initial settings and before stop simulation) for checking data between RTL and CForest.
     * If miss-match is detected, C side will notice error in CompRunner log.
     *
     * @param[in]  rtl_peid                 SV side will notify the current PE is executed in RTL side.
     *                                      C side will depend on this info to understand which out-of-order info will be updated.
     * @param[in]  rtl_time                 SV side will notify the the current timing in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_cycle                SV side will notify the current cycle in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_mem_addr             SV side will notify the memory address for comparison.
     *                                      C side will get the suitable data from CForest with this address.
     * @param[in]  rtl_mem_val              SV side will notify the memory data in RTL side.
     *                                      C side will compare memory data between RTL and CForest.
     *                                      The comparison result will be show in CompRunner log.
     * @return Error code
     * @retval #COMP_DPI_MATCH     : This return value means DPI function in CompRunner process correctly without error, the comparision result is aslo MATCH.
     *                               CompRunner SV side can notify next notification to continue comparision.
     * @retval #COMP_DPI_MISMATCH  : This return value means DPI function in CompRunner process correctly, no issue was occurred.
     *                               The comparison between RTL and CForest is MISMATCH. The result of next execution can be wrong.
     *                               CompRunner log should start with the error keyword "<ERR: ...>"
     * @retval #COMP_DPI_SV_ERR    : This return value means DPI function in SV side got error in processing, some issues were occurred.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     * @retval #COMP_DPI_C_ERR     : This return value mean DPI function in C side got error in processing, some issues were occured by reading memory.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     */
    uint32_t G4MHv2_MemoryDataCheck (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, svLogicVecVal* rtl_mem_addr, svLogicVecVal* rtl_mem_val);


    /**
     * \fn uint32_t G4MHv2_CmpPeState (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, svLogicVecVal* rtl_state)
     * \brief PE status comparison CompRunner G4MHv2 function.
     *
     * This function is used to check PE status between RTL and CForest.
     * SV side can call it any time (after initial settings and before stop simulation) for checking status between RTL and CForest.
     * If miss-match is detected, C side will notice error in CompRunner log.
     *
     * @param[in]  rtl_peid                 SV side will notify the current PE is executed in RTL side.
     *                                      C side will depend on this info to understand which out-of-order info will be updated.
     * @param[in]  rtl_time                 SV side will notify the the current timing in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_cycle                SV side will notify the current cycle in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_state                SV side will notify status of the PE in RTL side.
     *                                    - 0 = running (not in halt).
     *                                    - 1 = Halt.
     *                                    - C side will compare status between CForest and RTL side.
     *                                      The comparison result will be show in CompRunner log.
     * @return Error code
     * @retval #COMP_DPI_MATCH     : This return value means DPI function in CompRunner process correctly without error, the comparision result is aslo MATCH.
     *                               CompRunner SV side can notify next notification to continue comparision.
     * @retval #COMP_DPI_MISMATCH  : This return value means DPI function in CompRunner process correctly, no issue was occurred.
     *                               The comparison between RTL and CForest is MISMATCH. The result of next execution can be wrong.
     *                               CompRunner log should start with the error keyword "<ERR: ...>"
     * @retval #COMP_DPI_SV_ERR    : This return value means DPI function in SV side got error in processing, some issues were occurred.
     *                               CompRunner SV side should cancel the next comparisons while CompRunner C side must fix these issues.
     */
    uint32_t G4MHv2_CmpPeState (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, svLogicVecVal* rtl_state);


    /**
     * \fn uint32_t G4MHv2_CmpSetIssueCycle (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_inst_id)
     * \brief Issue cycle time and instruction issue getting CompRunner G4MHv2 function.
     *
     * This function is used to get issue cycle time for specified in struction in RTL side..
     * This info will be used in CompRunner G4MHv2.0 in Cycle comparison function.
     *
     * @param[in]  rtl_peid                 SV side will notify the current PE is executed in RTL side.
     *                                      C side will depend on this info to understand which out-of-order info will be updated.
     * @param[in]  rtl_time                 SV side will notify the the current timing in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @param[in]  rtl_inst_id              SV side will notify the id of instruction in RTL side.
     *                                      C side will do nothing for this signal.
     *                                      This info is only showed in CompRunner log.
     * @return Error code
     * @retval 0                   : This function always return value 0.
     */
    uint32_t G4MHv2_CmpSetIssueCycle (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_inst_id);


    /**
     * \fn uint32_t G4MHv2_CmpSetIssueFlush (uint32_t rtl_peid)
     * \brief Flush cycle info CompRunner G4MHv2 function.
     *
     * This function is used to flush issue cycle time for specified PE in RTL side..
     * This info will be used in CompRunner G4MHv2.0 in Cycle comparison function.
     *
     * @param[in]  rtl_peid                 SV side will notify the current PE is executed in RTL side.
     *                                      C side will depend on this info to understand which out-of-order info will be updated.
     * @return Error code
     * @retval 0                   : This function always return value 0.
     */
    uint32_t G4MHv2_CmpSetIssueFlush (uint32_t rtl_peid);


    /**
     * \fn ComprunnerG4MHv2()
     * \brief Constructor function.
     */
    ComprunnerG4MHv2(){};


    /**
     * \fn ~ComprunnerG4MHv2()
     * \brief Destructor function.
     */
    ~ComprunnerG4MHv2(){};

private:


    /**
     * \var CycleHistory g_cycle_history
     * \brief Cycle comparison information
     *
     * This varibale is used to manage instruction or cycle information will be printed in log file.
     *
     */
    CycleHistory g_cycle_history;

    uint32_t g_result_mode = 0;
    uint64_t rtl_1st_cm_time;
    bool    rtl_pre_last_cm;
    bool    m_cyc_cmp_en;
    bool    m_cmprunner_en=1;

    // Non-blocking ID of mcyc load insn
    const uint32_t NBLKID_NONE        = 0x0000FFFF;
    const uint32_t INTC1_Base         = 0xFFFC0000U;
    const uint32_t INTC2_Base         = 0xFFF80000U;
    const uint32_t INTC_MAX_CHANNEL   = 2047;
    const PhysAddr EIBG_Offset        = 0x0280U;
    const PhysAddr FIBG_Offset        = 0x02C0U;
    const PhysAddr DBMK_Offset        = 0x080U ;
    const PhysAddr FNC_Offset         = 0x0A0U ;
    const PhysAddr FNBD_Offset        = 0x01A0U ;
    const uint32_t EIBG_mask          = 0x0FFU ;
    const uint32_t FIBG_mask          = 0x0FFFFU ;

    // Number PEs in G4MH is [1-8]. In G4MH, PEID = 0 is OK.
    // Number of errors in each PE.
    std::vector <uint32_t> g_cmp_error_count, g_cmp_final_status;

    const uint32_t EIITB_STATUS_INITIAL = false;
    std::vector <bool> g_cmp_eiitb_status;

    // Temporary setting
    const uint32_t MCYC_MIDDLE_INITIAL = 0;
    std::vector <uint32_t> g_cmp_mcyc_middle;

    // RTL flag write enable value, flag value.
    std::vector <uint32_t> rtl_FL_Wen_chk, rtl_FL_val_chk;

    // Declaration of print functions in CompRunner G4MH.
    // Sub-print function

    /**
     * \fn void CmpPrintBorder (std::ostringstream* oss)
     * \brief Sub-print function: Print border for message.
     *
     * This function will print 200 character "-" for message.
     *
     * @param[out] oss                      Append the border for log message.
     * @return None.
     */
    void CmpPrintBorder (std::ostringstream* oss);


    /**
     * \fn void CmpPrintErrorSpace (std::ostringstream* oss)
     * \brief Sub-print function: Print space for error message.
     *
     * This function will print 47 character " " for message.
     *
     * @param[out] oss                      Append the border for log message.
     * @return None.
     */
    void CmpPrintErrorSpace (std::ostringstream* oss);


    /**
     * \fn void CmpPrintErrorSpace (std::ostringstream* oss)
     * \brief Sub-print function: Print space for error message.
     *
     * This function will print 47 character " " for message.
     *
     * @param[out] oss                      Append the border for log message.
     * @return None.
     */
    void CmpPrintErrorCount (std::ostringstream* oss, uint32_t rtl_peid);
    void CmpPrintSpace (std::ostringstream* oss, int32_t space_num);
    void CmpPrintRTLInfo (std::ostringstream* oss, uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle);
    void CmpPrintPC (std::ostringstream* oss, uint32_t pc);
    void CmpPrintGR (std::ostringstream* oss, uint32_t reg_no, uint32_t reg_val);
    void CmpPrintWR (std::ostringstream* oss, uint32_t wreg_no,
                     uint32_t wrdata0_val, uint32_t wrdata1_val, uint32_t wrdata2_val, uint32_t wrdata3_val);
    void CmpPrintFlag (std::ostringstream* oss, uint32_t aval);
    void CmpPrintInstMnem (std::ostringstream* oss, std::string* inst_mnem, TraceCtrl* trace);
    void CmpPrintTrace (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, cmprunner::CmpResult ret, uint32_t rtl_pc, uint32_t iss_regno,
                          uint64_t rtl_reg_val_up, uint64_t rtl_reg_val_lo, uint64_t iss_reg_val_up, uint64_t iss_reg_val_lo,
                          uint32_t rtl_flag_en, uint32_t rtl_flag_val, uint32_t iss_flag_en, uint32_t iss_flag_val, uint32_t rtl_nblk_id, TraceCtrl *trace, bool cyc_comp_msg);
    void CmpNbPrintTrace (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, cmprunner::CmpResult ret, uint32_t iss_regno,
                          uint64_t rtl_reg_val_up, uint64_t rtl_reg_val_lo, uint64_t iss_reg_val_up, uint64_t iss_reg_val_lo, uint32_t rtl_nblk_id);
    void CmpPrintExp (uint32_t rtl_peid, TraceCtrl* trace,  uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_pc);
    void CmpRegBufTrace (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, cmprunner::CmpResult ret,
                        uint32_t rtl_pc, uint32_t rtl_reg_no, uint64_t rtl_reg_val_up, uint64_t rtl_reg_val_lo, uint32_t rtl_nblk_id);
    void CmpPrintTitle ();
    void CmpPrintAction (std::ostringstream* oss, uint32_t cmp_action);
    void CmpPrintNBLK (std::ostringstream* oss, uint32_t nblk_id);

    void ParseRTLSignal (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, std::string signal_message,
                         uint32_t rtl_signal_control, uint32_t rtl_signal_data, uint32_t max_val, uint32_t min_val);
    bool IsRegisterBufferError (cmprunner::CmpResult ret);

    uint64_t SearchIssueCycle (uint32_t rtl_peid, uint32_t rtl_inst_id);
    bool CmpConvertRegNumStore (uint32_t rtl_reg_num_in, uint32_t &cforest_reg_num_out);
    bool CmpConvertRegNumPrint (uint32_t cforest_reg_num_in, uint32_t &cforest_reg_num_out);
    uint32_t IsCompareSuccess (cmprunner::CmpResult ret);
    cmprunner::CmpResult CmpStepCancel (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_pc);

    /**
     * Add Debug function to print SV side value without processing in C++ side.
     *
     * @param[in]  rtl_peid                 show the current PE is executed in comprunner log.
     * @param[in]  rtl_time                 show the RTL timing in comprunner log.
     * @param[in]  rtl_cycle                show the RTL cycle in comprunner log.
     * @param[in]  msg                      show the message for DPI functions in comprunner log.
     */
    void DEBUG_LOG(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, const std::string *msg);


    /**
     * Initial function for internal value in CompRunner G4MHv2.
     *
     * @param[in]  rtl_pe_number            the number of running PE.
     */
    void init_CmpG4MHv2(uint32_t rtl_pe_number);
};


