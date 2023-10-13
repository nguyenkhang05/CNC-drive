/*
 * (c) 2011,2012 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <utility>
#include <map>
#include <vector>
#include "./cmp_runner_common.h"
#include "./icache.h"


/** \file cmp_runner_his.h
 * \brief CompRunner G4MH cycle comparison header file.
 *
 * This file contains the class, variable in cycle comparison function.
 */


/*!
  \def INST_ID_V_CHBII
  Define INST_ID_V_CHBII index.
*/
/*!
  \def INST_ID_V_CIBII
  Define INST_ID_V_CIBII index.
*/
/*!
  \def INST_ID_V_CFALI
  Define INST_ID_V_CFALI index.
*/
/*!
  \def INST_ID_V_CISTI
  Define INST_ID_V_CISTI index.
*/
/*!
  \def INST_ID_V_CILDI
  Define INST_ID_V_CILDI index.
*/
/*!
  \def INST_ID_V_CHBID
  Define INST_ID_V_CHBID index.
*/
/*!
  \def INST_ID_V_CHBIWBD
  Define INST_ID_V_CHBIWBD index.
*/
/*!
  \def INST_ID_V_CHBWBD
  Define INST_ID_V_CHBWBD index.
*/
/*!
  \def INST_ID_V_CIBID
  Define INST_ID_V_CIBID index.
*/
/*!
  \def INST_ID_V_CIBIWBD
  Define INST_ID_V_CIBIWBD index.
*/
/*!
  \def INST_ID_V_CIBWBD
  Define INST_ID_V_CIBWBD index.
*/
/*!
  \def INST_ID_V_CFALD
  Define INST_ID_V_CFALD index.
*/
/*!
  \def INST_ID_V_CISTD
  Define INST_ID_V_CISTD index.
*/
/*!
  \def INST_ID_V_CILDD
  Define INST_ID_V_CILDD index.
*/
/*!
  \def INST_ID_V_CLL
  Define INST_ID_V_CLL index.
*/
#define INST_ID_V_CHBII   TOTAL_INST_NUM + 1
#define INST_ID_V_CIBII   TOTAL_INST_NUM + 2
#define INST_ID_V_CFALI   TOTAL_INST_NUM + 3
#define INST_ID_V_CISTI   TOTAL_INST_NUM + 4
#define INST_ID_V_CILDI   TOTAL_INST_NUM + 5
#define INST_ID_V_CHBID   TOTAL_INST_NUM + 6
#define INST_ID_V_CHBIWBD TOTAL_INST_NUM + 7
#define INST_ID_V_CHBWBD  TOTAL_INST_NUM + 8
#define INST_ID_V_CIBID   TOTAL_INST_NUM + 9
#define INST_ID_V_CIBIWBD TOTAL_INST_NUM + 10
#define INST_ID_V_CIBWBD  TOTAL_INST_NUM + 11
#define INST_ID_V_CFALD   TOTAL_INST_NUM + 12
#define INST_ID_V_CISTD   TOTAL_INST_NUM + 13
#define INST_ID_V_CILDD   TOTAL_INST_NUM + 14
#if defined(COMP_RUNNER_G4MH)
#else //defined(COMP_RUNNER_G4MH)
#define INST_ID_V_CLL     TOTAL_INST_NUM + 15
#endif //defined(COMP_RUNNER_G4MH)
/*!
  \def INST_ID_V_TOTAL_CACHE
  Define INST_ID_V_TOTAL_CACHE index.
*/
#define INST_ID_V_TOTAL_CACHE TOTAL_INST_NUM + 15


/**
 * \class CycleItem
 *
 * \brief Define the cycle items in CForest and RTL..
 *
 */
class CycleItem {
public:
    CycleItem () : m_valid(false)  {}
    //! @brief Create CycleItem object with full arguments.
    CycleItem (uint64_t cf, uint64_t rtl, uint64_t cf_total, uint64_t rtl_total)
        : m_cf(cf), m_rtl(rtl), m_cf_total(cf_total), m_rtl_total(rtl_total), m_valid(true) {}
    ~CycleItem () {}
    //! @brief  m_valid値を設定する。
    void SetValid (bool valid) { m_valid = valid; }
    //! @brief  m_valid値を取得する。
    bool IsValid () const { return m_valid; }
    //! @brief  相対差異値(%)を取得する。
    void GetAbsDiff (uint64_t *abs_100) const;
    //! @brief  小計データ１項目をログに出力。
    void Print (uint32_t step);

private:
    uint64_t    m_cf;           //!< 小計区間のCforestサイクル数
    uint64_t    m_rtl;          //!< 小計区間のRTLサイクル数
    uint64_t    m_cf_total;     //!< Cforest通算サイクル数
    uint64_t    m_rtl_total;    //!< RTL通算サイクル数
    bool        m_valid;        //!< ランキング抽出用フラグ。
};


 /**
  * \class CycleHistory
  *
  * \brief This class is used to print the cycle comparison result in log file.
  *
  */
class CycleHistory {
public:
    CycleHistory()  {
        m_cycle_period = 1ULL;
        for (uint32_t i = 0; i <= PENUM; i++) {
            m_base_step_rtl[i]       = 0ULL;
            m_base_time_rtl[i]       = 0ULL;
            m_cf_last_sample_cyc[i]  = 0ULL;
            m_rtl_last_sample_cyc[i] = 0ULL;
            m_cf_last_sample_step[i] = 0ULL;
            m_cf_last_step_cyc[i]    = 0ULL;
            m_rtl_last_step_cyc[i]   = 0ULL;
            m_cf_last_step[i]        = 0ULL;
        }
    }
    ~CycleHistory() {}
    //! @brief The internal calculation.
    inline uint32_t OperandExtract (uint64_t x, uint32_t l, uint32_t r) const {
        return static_cast<uint32_t> ((x >> r) & ~(~0x0UL << (l - r + 1)));
    }

    //! @brief  サイクル周期の設定
    void SetCyclePeriod (uint64_t cycle_period);
    //! @brief  未採取の小計データ（端数）があればここで採取
    void Finalize (uint32_t rtl_peid);
    //! @brief  命令別データの蓄積
    void SetInst (uint32_t rtl_peid, uint32_t inst_no, uint64_t abs_cycle);
    //! @brief  命令別データの蓄積
    void SetInstFreq (uint32_t rtl_peid, uint32_t inst_no, uint64_t inst_code);
    //! @brief  ステップ毎のCycle値の観測
    void Set (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_pc,
            uint64_t cf_cycle, uint64_t cf_step, uint32_t inst_no, uint64_t inst_code);
    //! @brief  最終結果データをログに出力。
    void PrintFinal (uint32_t rtl_peid);
    //! @brief  全ての小計データをログに出力。
    void PrintSubtotal(uint32_t rtl_peid);
    //! @brief  Ranking出力
    void PrintRanking(uint32_t rtl_peid);
    //! @brief  命令別データをログに出力。
    void PrintInst (uint32_t rtl_peid);
    //! @brief  シミュレーション時間,サイクル数をログ出力
    void PrintCycle (uint32_t rtl_peid) const;
    //! @brief  シミュレーション時間,サイクル数をログ出力
    void PrintSpace (void) const;
    //! @brief Print the instruction which are used frequency.
    void PrintInstFrequency (uint32_t rtl_peid);


private:
    //! @brief  小計データの蓄積
    void SetSubtotal (uint32_t rtl_peid, uint64_t cf, uint64_t rtl);

    std::vector<CycleItem> m_history[PENUM+1]; //!< 小計データ蓄積用
    uint64_t m_cycle_period;                   //!< RTLクロック周期あたりの時間値
    uint64_t m_base_step_rtl[PENUM + 1];       //!< １ステップ目実行後のCForestのcycleカウント値
    uint64_t m_base_time_rtl[PENUM + 1];       //!< DPI_CmpStep()が最初に呼ばれたときのrtl時間
    uint64_t m_cf_last_sample_cyc[PENUM + 1];  //!< 前回小計データ採取時のCForestのcycleカウント値
    uint64_t m_rtl_last_sample_cyc[PENUM + 1]; //!< 前回小計データ採取時のRTLのcycleカウント値
    uint64_t m_cf_last_sample_step[PENUM + 1]; //!< 前回小計データ採取時のCForestステップカウント値
    uint64_t m_cf_last_step_cyc[PENUM + 1];    //!< 前回ステップ実行時点のCForestのcycleカウント値
    uint64_t m_rtl_last_step_cyc[PENUM + 1];   //!< 前回ステップ呼出し時点のRTLのcycleカウント値
    uint64_t m_cf_last_step[PENUM + 1];        //!< 前回ステップ実行後のCForestのステップカウント値
    uint64_t m_rtl_time[PENUM + 1];            //!< 今回ステップ呼出し時点のRTLシミュレーション時間
    uint64_t m_cf_cycle[PENUM + 1];            //!< 今回ステップ実行後のCForestのcycleカウント値
    uint64_t m_cf_step[PENUM + 1];             //!< 今回ステップ実行後のCForestのステップカウント値
    uint64_t m_rtl_cycle[PENUM + 1];           //!< 今回ステップ呼出し時点のRTLのcycleカウント値
    int64_t  m_diff_cycle[PENUM + 1];          //!< 今回ステップ実行による消費時間のCforest-RTL差異
    int64_t  m_acc_abs_cycle[PENUM + 1];       //!< Absolute Diff Cycle
    uint32_t m_rtl_pc[PENUM + 1];              //!< 前回実行命令PC
    uint32_t m_inst_no[PENUM + 1];             //!< 前回実行命令ID

    typedef std::map <uint32_t, std::pair<uint32_t, uint64_t> > InstTbl;
    typedef std::pair<uint32_t, std::pair<uint32_t, uint64_t> > InstTbl_p2;
    typedef                     std::pair<uint32_t, uint64_t>   InstTbl_p1;
    typedef std::map <uint32_t, uint32_t>                       InstTblFreq;
    InstTbl     m_inst_table[PENUM + 1];               //!< 命令別データ蓄積用コンテナ
    InstTblFreq m_inst_table_freq[PENUM + 1];               //!< 命令別データ蓄積用コンテナ
};


/**
 * \class IssueCycle
 *
 * \brief Issue cycle class.
 *
 */
class IssueCycle {
 public:
    /**
     * \fn IssueCycle (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_inst_id)
     * \brief Create IssueCycle with RTL initial values.
     *
     * @param[in]  rtl_peid                 PEID.
     * @param[in]  rtl_time                 Time.
     * @param[in]  rtl_inst_id              Instruction ID.
     *
     * @return IssueCycle object.
     */
    IssueCycle (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_inst_id) {
        m_rtl_peid = rtl_peid;
        m_rtl_time = rtl_time;
        m_rtl_inst_id = rtl_inst_id;
    }
    /**
     * \fn uint32_t GetPEID ()
     * \brief Get PEID.
     *
     * @param  -
     * @return PEID.
     */
    uint32_t GetPEID () { return m_rtl_peid; }
    /**
     * \fn uint32_t GetInstID ()
     * \brief Get Instruction ID.
     *
     * @param  -
     * @return Instruction ID.
     */
    uint32_t GetInstID () { return m_rtl_inst_id; }
    /**
     * \fn uint64_t GetRtlCycle ()
     * \brief Get RTL cycle.
     *
     * @param  -
     * @return RTL cycle.
     */
    uint64_t GetRtlCycle () { return m_rtl_time; }

 private:
    uint32_t m_rtl_peid;
    uint64_t m_rtl_time;
    uint32_t m_rtl_inst_id;
};
