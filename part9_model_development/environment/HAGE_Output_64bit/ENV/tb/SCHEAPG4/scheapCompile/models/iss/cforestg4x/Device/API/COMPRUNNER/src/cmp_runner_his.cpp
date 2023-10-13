/*
 * (c) 2011,2012 Renesas Electronics Corporation
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
#include <utility>
#include <map>
#include <vector>
#include "./config.h"
#include "./inst_v850.h"
#include "./inst_id_list.h"
#include "./forest_common.h"
#include "./cmp_runner_his.h"
#if defined(COMP_RUNNER_G3MH)
#include "./cmp_runner_if_g3mh.h"
#elif defined(COMP_RUNNER_G4MH)
#include "./cmp_runner_if_g4mh.h"
#else  // defined (COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)
#include "./cmp_runner_if_g3kh.h"
#endif // defined (COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)
#include "./cmp_runner_common.h"
#include "./forest_python.h"


extern cmprunner::CmpRunnerIF* cforestif;
extern FILE* g_cmp_fp;



//! @brief  サイクル周期の設定
void CycleHistory::SetCyclePeriod (uint64_t cycle_period)
{
    if (cycle_period != 0) {
        m_cycle_period = cycle_period;
    }
}



/*!
  @brief  小計データの蓄積
*/
void CycleHistory::SetSubtotal (uint32_t rtl_peid, uint64_t cf, uint64_t rtl)
{
    CycleItem item (cf - m_cf_last_sample_cyc[rtl_peid],
            rtl - m_rtl_last_sample_cyc[rtl_peid], cf, rtl);
    m_history[rtl_peid].push_back(item);
    m_cf_last_sample_cyc[rtl_peid] = cf;
    m_rtl_last_sample_cyc[rtl_peid] = rtl;
}



/*!
  @brief  未採取の小計データ（端数）があればここで採取
*/
void CycleHistory::Finalize (uint32_t rtl_peid)
{
    uint32_t sampl_period = PythonAPI::GetCompSamplePeriod ();
    if (m_cf_last_step[rtl_peid] % sampl_period != 0) {
        SetSubtotal (rtl_peid, m_cf_last_step_cyc[rtl_peid], m_rtl_last_step_cyc[rtl_peid]);
    }
}



/*!
  @brief  命令別データの蓄積
*/
void CycleHistory::SetInst (uint32_t rtl_peid, uint32_t inst_no, uint64_t abs_cycle)
{
    InstTbl::iterator itr = m_inst_table[rtl_peid].find(inst_no);
    if (itr == m_inst_table[rtl_peid].end()) {
        m_inst_table[rtl_peid].insert(InstTbl_p2(inst_no, InstTbl_p1 (1, abs_cycle)));
    } else {
        itr->second.first++;
        itr->second.second += abs_cycle;
    }
}


/*!
  @brief  命令別データの蓄積
*/
void CycleHistory::SetInstFreq (uint32_t rtl_peid, uint32_t inst_no, uint64_t inst_code)
{
    if (unlikely(inst_no == INST_ID_V_CACHE)) {
        uint32_t opl  = OperandExtract (inst_code, 31, 27);
        uint32_t oph  = OperandExtract (inst_code, 12, 11) << 5;
        uint32_t op   = oph | opl;
        switch (op) {
        case Icache::OP_CHBII : inst_no = INST_ID_V_CHBII; break;
        case Icache::OP_CFALI : inst_no = INST_ID_V_CFALI; break;
        case Icache::OP_CIBII : inst_no = INST_ID_V_CIBII; break;
        case Icache::OP_CISTI : inst_no = INST_ID_V_CISTI; break;
        case Icache::OP_CILDI :
            switch (op) {
            case Icache::OP_CIBII : inst_no = INST_ID_V_CIBII; break;
            case Icache::OP_CISTI : inst_no = INST_ID_V_CISTI; break;
            case Icache::OP_CILDI : inst_no = INST_ID_V_CILDI; break;
            }
            break;
#if defined (COMP_RUNNER_G4MH)
#else  // defined (COMP_RUNNER_G4MH)
        case Icache::OP_CHBID :   inst_no = INST_ID_V_CHBID;   break;
        case Icache::OP_CHBIWBD : inst_no = INST_ID_V_CHBIWBD; break;
        case Icache::OP_CHBWBD :  inst_no = INST_ID_V_CHBWBD;  break;
        case Icache::OP_CIBID :   inst_no = INST_ID_V_CIBID;   break;
        case Icache::OP_CIBIWBD : inst_no = INST_ID_V_CIBIWBD; break;
        case Icache::OP_CIBWBD :  inst_no = INST_ID_V_CIBWBD;  break;
        case Icache::OP_CFALD :   inst_no = INST_ID_V_CFALD;   break;
        case Icache::OP_CISTD :   inst_no = INST_ID_V_CISTD;   break;
        case Icache::OP_CILDD :   inst_no = INST_ID_V_CILDD;   break;
        case Icache::OP_CLL :     inst_no = INST_ID_V_CLL;     break;
#endif // defined (COMP_RUNNER_G4MH)
        default : break;
        }
    }

    InstTblFreq::iterator itr = m_inst_table_freq[rtl_peid].find(inst_no);
    if (itr == m_inst_table_freq[rtl_peid].end()) {
        m_inst_table_freq[rtl_peid].insert(std::pair<uint32_t, uint32_t>(inst_no, 1));
    } else {
        itr->second++;
    }
}


/*!
  @brief   ステップ毎のCycle値の観測
*/
void CycleHistory::Set (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_pc,
                        uint64_t cf_cycle, uint64_t cf_step,
                        uint32_t inst_no, uint64_t inst_code)
{
    // RTLサイクル数のベース値を確定
    if (cf_step <= 1) {
        m_base_step_rtl[rtl_peid] = cf_cycle;
        m_base_time_rtl[rtl_peid] = rtl_time;
    }

    // PrintCycle()で表示させるデータを保存
    m_rtl_time[rtl_peid]   = rtl_time;
    m_cf_cycle[rtl_peid]   = cf_cycle;
    m_cf_step[rtl_peid]    = cf_step;
    m_rtl_cycle[rtl_peid]  = (rtl_time - m_base_time_rtl[rtl_peid]) /
            m_cycle_period + m_base_step_rtl[rtl_peid];
    if (rtl_pc == m_rtl_pc[rtl_peid]) {
        m_diff_cycle[rtl_peid] = 0;
        return;
    }

    // 命令別データ蓄積チェック
    m_diff_cycle[rtl_peid] = (cf_cycle - m_cf_last_step_cyc[rtl_peid]) -
            (m_rtl_cycle[rtl_peid] - m_rtl_last_step_cyc[rtl_peid]);
    int64_t abs_cycle  = (m_diff_cycle[rtl_peid] < 0) ?
            -m_diff_cycle[rtl_peid] : m_diff_cycle[rtl_peid];
    if ((abs_cycle != 0)&&(m_inst_no[rtl_peid] != 0)) {
        SetInst (rtl_peid, m_inst_no[rtl_peid], abs_cycle);
    }
    SetInstFreq (rtl_peid, inst_no, inst_code);

    // Absolute Diff蓄積
    m_acc_abs_cycle[rtl_peid] += abs_cycle;

    // 小計データ蓄積チェック
    uint32_t sampl_period = PythonAPI::GetCompSamplePeriod ();
    if ((cf_step % sampl_period == 0)&&(cf_step != m_cf_last_sample_step[rtl_peid])) {
        SetSubtotal (rtl_peid, cf_cycle, m_rtl_cycle[rtl_peid]);
        m_cf_last_sample_step[rtl_peid] = cf_step;
    }

    // 今回値を前回値として保存
    m_cf_last_step[rtl_peid] = cf_step;
    m_cf_last_step_cyc[rtl_peid] = cf_cycle;
    m_rtl_last_step_cyc[rtl_peid] = m_rtl_cycle[rtl_peid];
    m_rtl_pc[rtl_peid] = rtl_pc;
    m_inst_no[rtl_peid] = inst_no;
}



/*!
  @brief  最終結果データをログに出力。
*/
void CycleHistory::PrintFinal (uint32_t rtl_peid)
{
    //ヘッダー出力
    fprintf (g_cmp_fp, "\n");
    fprintf (g_cmp_fp,
        "< FINAL RESULT ================================================ >\n");
    fprintf (g_cmp_fp,
        " Cycle      Cycle     Relative   Relative  Absolute   Absolute\n");
    fprintf (g_cmp_fp,
        " (CForest)      (RTL)       Diff   Diff(%%)       Diff   Diff(%%)\n");

    // 本体出力
    uint64_t diff_abs;
    int64_t sign;
    if (m_cf_last_step_cyc[rtl_peid] > m_rtl_last_step_cyc[rtl_peid]) {
        diff_abs = m_cf_last_step_cyc[rtl_peid] - m_rtl_last_step_cyc[rtl_peid];
        sign = 1;
    } else {
        diff_abs = m_rtl_last_step_cyc[rtl_peid] - m_cf_last_step_cyc[rtl_peid];
        sign = -1;
    }
    if (m_rtl_last_step_cyc[rtl_peid] == 0) {
        fprintf (g_cmp_fp, "%10ld %10ld ",
                m_cf_last_step_cyc[rtl_peid], m_rtl_last_step_cyc[rtl_peid]);
        fprintf (g_cmp_fp, "%10ld --------- ", sign * static_cast<int64_t>(diff_abs));
        fprintf (g_cmp_fp, "%10ld --------- ", m_acc_abs_cycle[rtl_peid]);
        fprintf (g_cmp_fp, "\n");
        return;
    }
    long double diff = roundl(static_cast<long double>(diff_abs) * 10000 * sign /
            m_rtl_last_step_cyc[rtl_peid]);
    fprintf (g_cmp_fp, "%10ld %10ld ", m_cf_last_step_cyc[rtl_peid], m_rtl_last_step_cyc[rtl_peid]);
    fprintf (g_cmp_fp, "%10ld ", sign * static_cast<int64_t>(diff_abs));
    fprintf (g_cmp_fp, "%9.2Lf ", diff / 100);

    int32_t abs_100 = static_cast<int32_t>((m_acc_abs_cycle[rtl_peid] * 20000 /
            m_rtl_last_step_cyc[rtl_peid] + 1) / 2);
    fprintf (g_cmp_fp, "%10ld ", m_acc_abs_cycle[rtl_peid]);
    fprintf (g_cmp_fp, "%6d.%02d ", (abs_100 / 100), abs_100 % 100);
    fprintf (g_cmp_fp, "\n");
}


void CycleHistory::PrintInstFrequency (uint32_t rtl_peid)
{
    //ヘッダー出力
    fprintf (g_cmp_fp, "\n");
    fprintf (g_cmp_fp, "< INSTRUCTION LIST (FREQUENCY LIST) ================ >\n");
    fprintf (g_cmp_fp, " insts        Num. of    Total diff  Average    \n");
    fprintf (g_cmp_fp, "              difference      cycle  diff cycle \n");

    if (m_inst_table_freq[rtl_peid].empty()) {
        fprintf (g_cmp_fp, "no data !!\n");
        return;
    }

    for (uint32_t id = INST_ID_V_0 + 1; id <= TOTAL_INST_NUM; id++) {
        InstTblFreq::iterator itr = m_inst_table_freq[rtl_peid].find(id);
        std::string inst_str;
        uint64_t term;
        if (id == INST_ID_V_CACHE) {
            for (uint32_t id_cache = INST_ID_V_CHBII; id_cache <= INST_ID_V_CLL; id_cache++) {
                itr = m_inst_table_freq[rtl_peid].find(id_cache);
                cforestif->GetOperandFormat (id, &inst_str);
                std::stringstream ss;
                switch (id_cache) {
                case INST_ID_V_CHBII   : ss << "CHBII";   break;
                case INST_ID_V_CIBII   : ss << "CIBII";   break;
                case INST_ID_V_CFALI   : ss << "CFALI";   break;
                case INST_ID_V_CISTI   : ss << "CISTI";   break;
                case INST_ID_V_CILDI   : ss << "CILDI";   break;
                case INST_ID_V_CHBID   : ss << "CHBID";   break;
                case INST_ID_V_CHBIWBD : ss << "CHBIWBD"; break;
                case INST_ID_V_CHBWBD  : ss << "CHBWBD";  break;
                case INST_ID_V_CIBID   : ss << "CIBID";   break;
                case INST_ID_V_CIBIWBD : ss << "CIBIWBD"; break;
                case INST_ID_V_CIBWBD  : ss << "CIBWBD";  break;
                case INST_ID_V_CFALD   : ss << "CFALD";   break;
                case INST_ID_V_CISTD   : ss << "CISTD";   break;
                case INST_ID_V_CILDD   : ss << "CILDD";   break;
                case INST_ID_V_CLL     : ss << "CLL"  ;   break;
                }
                // 対象文字列の置換
                term = inst_str.find(" ", 0);
                if (term != std::string::npos) {
                    inst_str = inst_str.substr(0, term);
                }

                inst_str.insert(inst_str.length(), " ");
                inst_str.insert(inst_str.length(), ss.str());

                if (itr == m_inst_table_freq[rtl_peid].end()) {
                    fprintf (g_cmp_fp, "%-15s : %4d\n", inst_str.c_str(), 0);
                } else {
                    fprintf (g_cmp_fp, "%-15s : %4d\n", inst_str.c_str(), itr->second);
                }
            }
        } else {
            cforestif->GetOperandFormat (id, &inst_str);
            term = inst_str.find(" ", 0);
            if (term != std::string::npos) {
                inst_str = inst_str.substr(0, term);
            }
            if (itr == m_inst_table_freq[rtl_peid].end()) {
                fprintf (g_cmp_fp, "%-15s : %4d\n", inst_str.c_str(), 0);
            } else {
                fprintf (g_cmp_fp, "%-15s : %4d\n", inst_str.c_str(), itr->second);
            }
        }
    }
}


/*!
  @brief  全ての小計データをログに出力。
*/
void CycleHistory::PrintSubtotal(uint32_t rtl_peid)
{
    uint32_t sampl_period = PythonAPI::GetCompSamplePeriod ();
    //ヘッダー出力
    fprintf (g_cmp_fp, "\n");
    fprintf (g_cmp_fp,
        "< BLOCK LIST (%ud Step / Block) =========================================== >\n",
        sampl_period);
    fprintf (g_cmp_fp,
        " No. insts  Cycle      Cycle       Relative  Relative TotalCycle TotalCycle \n");
    fprintf (g_cmp_fp,
        "            (CForest)      (RTL)       Diff   Diff(%%)  (CForest)      (RTL) \n");
    //本体出力

    if (m_history[rtl_peid].empty()) {
        fprintf (g_cmp_fp, "no data !!\n");
        return;
    }

    uint32_t step = 0;
    std::vector<CycleItem>::iterator itr = m_history[rtl_peid].begin ();
    while (itr != m_history[rtl_peid].end ()) {
        if ((*itr).IsValid()) {
            (*itr).Print (step);
        }
        ++ itr;
        step += sampl_period;
    }
}



/*!
  @brief  Ranking出力
*/
void CycleHistory::PrintRanking(uint32_t rtl_peid)
{
    uint32_t sampl_period = PythonAPI::GetCompSamplePeriod ();      //!< 小計を採る塊のサイズ
    uint32_t rank_lines   = PythonAPI::GetCompRankLines ();         //!< ランキング項目数

    //ヘッダー出力
    fprintf (g_cmp_fp, "\n");
    fprintf (g_cmp_fp,
        "< TOP %ud BLOCK LIST (%ud Step / Block) ==================================== >\n",
        rank_lines, sampl_period);
    fprintf (g_cmp_fp,
        " No. insts  Cycle      Cycle       Relative  Relative TotalCycle TotalCycle \n");
    fprintf (g_cmp_fp,
        "            (CForest)      (RTL)       Diff   Diff(%%)  (CForest)      (RTL) \n");

    if (m_history[rtl_peid].empty()) {
        fprintf (g_cmp_fp, "no data !!\n");
        return;
    }

    std::vector<CycleItem>::iterator head_itr;  //!< 最大相対差異値を持つ最初の項目を指す
    std::vector<CycleItem>::iterator current_itr;

    uint32_t rank = 1;                          //!< 順位
    while (rank <= rank_lines) {
        // 小計データから最大相対差異値(%)を検索
        uint32_t step = 0;                  //!< （その小計を採り始めた命令の）stepカウント
        uint64_t top_value = 0;             //!< 最大相対差異値
        uint32_t matchs = 0;                //!< 最大相対差異値を持つ項目数
        uint32_t head_step = 0;             //!< 最大相対差異値を持つ最初の項目のstepカウント
        current_itr = m_history[rtl_peid].begin ();
        while (current_itr != m_history[rtl_peid].end ()) {
            if ((*current_itr).IsValid()) {
                //相対差異値(%)を取り出す
                uint64_t abs_100;          //!< 検索中の項目の相対差異値
                (*current_itr).GetAbsDiff (&abs_100);
                if (top_value < abs_100) {
                    //最大値の更新処理
                    top_value = abs_100;
                    head_itr = current_itr;
                    head_step = step;
                    matchs = 1;
                } else if (top_value == abs_100) {
                    //最大値と等しかった場合の処理
                    ++ matchs;
                }
            }
            ++ current_itr;
            step += sampl_period;
        }
        if (top_value == 0) {
            if (rank == 1) {
                fprintf (g_cmp_fp, "All Match !!\n");
            }
            break;
        }
        //最大相対差異値(%)を持つ小計データをログ出力。出力した項目は無効化。
        current_itr = head_itr;
        step = head_step;
        while (current_itr != m_history[rtl_peid].end ()) {
            if ((*current_itr).IsValid()) {
                //相対差異値(%)を取得
                uint64_t abs_100;
                (*current_itr).GetAbsDiff (&abs_100);
                if (top_value == abs_100) {
                    //小計データをログ出力。
                    (*current_itr).Print (step);
                    //無効化
                    (*current_itr).SetValid(false);
                    -- matchs;
                    ++ rank;
                    if (matchs == 0) {
                        //最大相対差異値(%)を持つ項目を出し切った。
                        break;
                    }
                    if (rank > rank_lines) {
                        //最大相対差異値(%)を持つ項目が残っているが指定されたRanking項目数に
                        //達した。
                        fprintf (g_cmp_fp, "         :          :          :          :\n");
                        break;
                    }
                }
            }
            ++ current_itr;
            step += sampl_period;
        }
    }
}



/*!
  @brief  命令別データをログに出力。
*/
void CycleHistory::PrintInst (uint32_t rtl_peid)
{
    //ヘッダー出力
    fprintf (g_cmp_fp, "\n");
    fprintf (g_cmp_fp, "< INSTRUCTION LIST ============================ >\n");
    fprintf (g_cmp_fp, " insts        Num. of    Total diff  Average    \n");
    fprintf (g_cmp_fp, "              difference      cycle  diff cycle \n");

    if (m_inst_table[rtl_peid].empty()) {
        fprintf (g_cmp_fp, "no data !!\n");
        return;
    }

    typedef std::multimap <std::string, std::pair<uint32_t, uint64_t> > InstTblWk1;
    typedef std::pair<std::string, std::pair<uint32_t, uint64_t> > InstTblWk1_p2;
    InstTblWk1   m_inst_table_work1;

    typedef std::multimap <uint64_t, std::pair<std::string, uint32_t> > InstTblWk2;
    typedef std::pair<uint64_t, std::pair<std::string, uint32_t> > InstTblWk2_p2;
    typedef                     std::pair<std::string, uint32_t>   InstTblWk2_p1;
    InstTblWk2   m_inst_table_work2;

    //命令ＩＤをキーとするコンテナから、命令文字列をキーとするコンテナに積み替える
    InstTbl::iterator itr = m_inst_table[rtl_peid].begin();
    while (itr != m_inst_table[rtl_peid].end()) {
        std::string inst_str;
        cforestif->GetOperandFormat (itr->first, &inst_str);
        uint64_t term = inst_str.find(" ", 0);
        if (term != std::string::npos) {
            inst_str = inst_str.substr(0, term);
        }
        m_inst_table_work1.insert(InstTblWk1_p2(inst_str, itr->second));
        ++ itr;
    }

    //命令文字列をキーとするコンテナから、相対差異値をキーとするコンテナに積み替える
    InstTblWk1::iterator it_wk1 = m_inst_table_work1.end();
    do {
        //行ごとの処理
        -- it_wk1;
        std::string inst_str = it_wk1->first;
        uint32_t inst_count = it_wk1->second.first;
        uint64_t abs_cycle = it_wk1->second.second;
        m_inst_table_work2.insert(
            InstTblWk2_p2(abs_cycle, InstTblWk2_p1(inst_str, inst_count)));
        ++ itr;
    } while (it_wk1 != m_inst_table_work1.begin ());

    //相対差異値が降順になるようにログ出力
    InstTblWk2::iterator it_wk2 = m_inst_table_work2.end ();
    do {
        //行ごとの処理
        -- it_wk2;
        uint64_t abs_cycle = it_wk2->first;
        std::string inst_str = it_wk2->second.first;
        uint32_t inst_count = it_wk2->second.second;
        long double diff = roundl(static_cast<long double>(abs_cycle) * 100 / inst_count);
        //ログ出力
        fprintf (g_cmp_fp, "%-13s ", inst_str.c_str());
        fprintf (g_cmp_fp, "%10ud %10lu   ", inst_count, static_cast<uint64_t>(abs_cycle));
        fprintf (g_cmp_fp, "%9.2Lf\n", diff / 100);
    } while (it_wk2 != m_inst_table_work2.begin ());
}



/*!
  @brief  シミュレーション時間,サイクル数をログ出力
*/
void CycleHistory::PrintCycle (uint32_t rtl_peid) const
{
    std::ostringstream oss;
#if defined(COMP_RUNNER_G4MH)
#else // defined(COMP_RUNNER_G4MH)
    oss << std::setw(10) << std::right << m_rtl_time[rtl_peid];
#endif // defined(COMP_RUNNER_G4MH)
    oss << std::setw(10) << std::right << m_cf_step[rtl_peid];
    oss << std::setw(10) << std::right << m_cf_cycle[rtl_peid];
    oss << " : ";
    oss << std::setw(10) << std::right << m_rtl_cycle[rtl_peid];
    if (m_diff_cycle[rtl_peid] != 0ULL) {
        oss << std::showpos;
    }
    oss << std::setw(7) << std::right << m_diff_cycle[rtl_peid];

    fprintf (g_cmp_fp, "%s", oss.str().c_str());
}


/*!
  @brief  シミュレーション時間,サイクル数をログ出力
*/
void CycleHistory::PrintSpace (void) const
{
    std::ostringstream oss;
#if defined(COMP_RUNNER_G4MH)
#else // defined(COMP_RUNNER_G4MH)
    oss << "          ";
#endif // defined(COMP_RUNNER_G4MH)
    oss << "          ";
    oss << "          ";
    oss << "   ";
    oss << "          ";
    oss << "       ";

    fprintf (g_cmp_fp, "%s", oss.str().c_str());
}


/*!
  @brief  相対差異値(%)を取得する。
*/
void CycleItem::GetAbsDiff (uint64_t *abs_100) const
{
    uint64_t diff_abs;
    if (m_rtl < m_cf) {
        diff_abs = m_cf - m_rtl;
    } else {
        diff_abs = m_rtl - m_cf;
    }
    if (m_rtl == 0) {
        *abs_100 = 0;
    } else {
        *abs_100 = (diff_abs * 20000 / m_rtl + 1) / 2;
    }
}



/*!
  @brief  小計データ１項目をログに出力。
*/
void CycleItem::Print (uint32_t step)
{
    uint64_t diff_abs;
    int64_t sign;
    if (m_cf < m_rtl) {
        diff_abs = m_rtl - m_cf;
        sign = -1;
    } else {
        diff_abs = m_cf - m_rtl;
        sign = 1;
    }
    int64_t diff = sign * static_cast<int64_t>(diff_abs);
    long double diff_100;
    if (m_rtl == 0) {
        diff_100 = 0;
    } else {
        diff_100 = roundl(static_cast<long double>(diff_abs) * 10000 * sign / m_rtl);
    }
    fprintf (g_cmp_fp, "%10d %10ld %10ld %10ld ", step, m_cf, m_rtl, diff);
    fprintf (g_cmp_fp, "%9.2Lf ", diff_100 / 100);
    fprintf (g_cmp_fp, "%10ld %10ld ", m_cf_total, m_rtl_total);
    fprintf (g_cmp_fp, "\n");
}
