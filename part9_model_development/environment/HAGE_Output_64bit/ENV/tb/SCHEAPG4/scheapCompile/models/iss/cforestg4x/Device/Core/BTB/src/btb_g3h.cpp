/*
 * (c) 2011,2012 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <algorithm>
#include <sstream>
#include <iomanip>
#include <vector>
#include "./config.h"
#include "./forest_utility.h"
#include "./forest_common.h"
#include "./btb_g3h.h"
#include "./forest_message.h"
#include "./forest_utility.h"
const Bht::BhtEntryType Bht::BHT_SNAGREE = 0x0;
const Bht::BhtEntryType Bht::BHT_WNAGREE = 0x1;
const Bht::BhtEntryType Bht::BHT_WAGREE  = 0x2;
const Bht::BhtEntryType Bht::BHT_SAGREE  = 0x3;

//#define CLAER_RAS_BY_BPMISS

/*!
 * @brief 分岐予測判定
 * @param branch_pc 分岐元アドレス
 * @param is_taken
 * @param is_forward_disp
 * @return BpResult
 *
 * @todo To be documented.
 */
BpResult Bht::PredictBranch (PhysAddr branch_pc, bool is_taken, bool is_forward_disp)
{
    bool is_agree = IsAgree (is_taken, is_forward_disp);

    // BHTで分岐agreeを予測
    uint32_t bht_entry = GetBhtEntry (branch_pc);
    BpResult bp_result;
    if (IsPredictBranchAgree (bht_entry) == is_agree) {
        // ヒット
        bp_result = BP_HIT;
    } else {
        // ミス
        bp_result = BP_MISS;
    }

    // 無条件分岐はBHTに学習されないが、
    // 初回予測ではBHTが使用される。HIT/MISSカウントに反映
    CountBpResult (bp_result);
    return bp_result;
}


/*!
 * @brief 分岐予測判定及び分岐履歴学習
 * @param branch_pc 分岐元アドレス
 * @param is_taken 分岐情報
 * @param is_forward_disp
 *
 * @todo To be documented.
 */
void Bht::Study (PhysAddr branch_pc, bool is_taken, bool is_forward_disp)
{
    // 静的分岐方向
    bool is_agree = IsAgree (is_taken, is_forward_disp);

    // BHTで分岐agreeを予測
    uint32_t bht_entry = GetBhtEntry (branch_pc);

    // BHTの更新
    UpdateBht (bht_entry, is_agree);

    // GHRの更新
#ifdef USE_TAKEN_GHT
#ifdef STUDY_GHR_64BIT
    ReserveUpdateGhr (is_taken);
#else // STUDY_GHR_64BIT
    UpdateGhr (is_taken);
#endif // STUDY_GHR_64BIT
#else // USE_TAKEN_GHT
    UpdateGhr (is_agree); // g4hはこちら
#endif // USE_TAKEN_GHT
}



bool Bht::IsAgree (bool is_taken, bool is_forward_disp) const
{
    bool is_static_tkn;
    if (is_forward_disp){
        is_static_tkn = false;
    } else {
        is_static_tkn = true;
    }

    // agree方向の決定
    bool is_agree;
    if (is_static_tkn == is_taken){
        is_agree = true;
    } else {
        is_agree = false;
    }

    return is_agree;
}


/*!
 * @brief BHTのインデックス計算
 * @param branch_pc 分岐元アドレス
 * @return BHTのindex
 */
uint32_t Bht::GetBhtEntry (PhysAddr branch_pc) const
{
#ifdef USE_GSHARE_BHT
#ifdef USE_6BIT_GHR
    // index = ( PC[9:4] xor GHR ) + PC[3]
    // way = PC[2:1]
    return ((((branch_pc >> 4) ^ m_ghr) & GHR_MASK) << 3) | ((branch_pc >> 1) & 0x7);
#else // USE_6BIT_GHR
    // index = ( PC[10:3] xor GHR )
    // way = PC[2:1]
    return ((((branch_pc >> 3) ^ m_ghr) & GHR_MASK) << 2) | ((branch_pc >> 1) & 0x3);
#endif // USE_6BIT_GHR
#else // USE_GSHARE_BHT
    // GshareなしBHT : GHRを使用しない
    // index = PC[9:3]
    // way = PC[2:1]
    return (branch_pc >> 1) & BHT_ENTRY_MASK;
#endif // USE_GSHARE_BHT
}


/*!
 * @brief GHR更新
 * @param is_taken 分岐情報
 */
void Bht::UpdateGhr (bool is_taken)
{
#ifdef STUDY_GHR_64BIT
    m_ghr = m_reserved_ghr;
    m_reserved_ghr = (m_ghr << 1) & GHR_MASK;
#else // STUDY_GHR_64BIT
    //学習対象以外の分岐命令でもGHRを更新
    m_ghr = (m_ghr << 1) & GHR_MASK;
    if (is_taken) {
        // 分岐時は1をシフトイン
        m_ghr |= 1;
    } else {
        // 非分岐時は0をシフトイン
    }
#endif // STUDY_GHR_64BIT
}


/*!
 * @brief GHR更新
 * @param is_taken 分岐情報
 */
void Bht::ReserveUpdateGhr (bool is_taken)
{
    //学習対象以外の分岐命令でもGHRを更新
    m_reserved_ghr = (m_ghr << 1) & GHR_MASK;
    if (is_taken) {
        // 分岐時は1をシフトイン
        m_reserved_ghr |= 1;
    } else {
        // 非分岐時は0をシフトイン
    }
}


/*!
 * @brief 分岐予測結果の取得
 * @param entry BHTindex
 * @return bool true=Agree, false=Agree
 */
bool Bht::IsPredictBranchAgree (uint32_t entry) const
{
    return ((m_bht[entry] & 0x2) == 0x2);
}


/*!
 * @brief パタン履歴(2bit飽和型カウンタ)更新
 * @param entry BHTindex
 * @param is_agree
 *
 * @todo To be documented.
 */
void Bht::UpdateBht (uint32_t entry, bool is_agree)
{
    BhtEntryType history = m_bht[entry];
    if (is_agree) {
        if (history != BHT_SAGREE) {
#ifdef USE_2BIT_COUNTER_BHT
            m_bht[entry] = history + 1;
#else // USE_2BIT_COUNTER_BHT
            if (history == BHT_WNAGREE) {
                // Weak状態だと反対側のStrongに一気に飛ぶ、早いカウンタ
                m_bht[entry] = BHT_SAGREE;
            } else {
                m_bht[entry] = history + 1;
            }
#endif // USE_2BIT_COUNTER_BHT
        }
    } else {
        if (history != BHT_SNAGREE) {
#ifdef USE_2BIT_COUNTER_BHT
            m_bht[entry] = history - 1;
#else // USE_2BIT_COUNTER_BHT
            if (history == BHT_WAGREE) {
                // Weak状態だと反対側のStrongに一気に飛ぶ、早いカウンタ
                m_bht[entry] = BHT_SNAGREE;
            } else {
                m_bht[entry] = history - 1;
            }
#endif // USE_2BIT_COUNTER_BHT
        }
    }
}


/*!
 * @brief BHT/GHRクリア
 */
void Bht::ClearBht (void)
{
    m_bht.clear ();
    m_bht.assign (BHT_ENTRY_NUM * BHT_WAY_NUM, BHT_WAGREE);
    m_ghr = 0;
    m_reserved_ghr = 0;
}


/*!
 * @brief BHT予測HIT/MISS統計情報のクリア
 */
void Bht::ResetBhtCounter (void)
{
    m_bp_hit_count  = 0;
    m_bp_miss_count = 0;
}


/*!
 * @brief BHTリセット
 */
void Bht::Reset (void)
{
    ClearBht ();
    ResetBhtCounter ();
}


/*!
 * @brief BHTコンストラクタ
 */
Bht::Bht ()
{
    Reset ();
}


/*!
 * @brief BHTデストラクタ
 */
Bht::~Bht (void)
{
}


/*!
 * @brief 分岐の学習
 * @param return_pc 関数コールの戻り先アドレス
 */
void Ras::Study (PhysAddr return_pc)
{
    m_ras.push_back (return_pc);
    if (m_ras.size () > m_ras_num) {
        // エントリ数が最大値を超えたら、一番古いエントリを削除
        m_ras.erase (m_ras.begin ());
    }
}


/*!
 * @brief 分岐予測
 * @param target_pc 分岐先アドレス
 * @return BpResult 分岐予測結果
 */
BpResult Ras::PredictBranch (PhysAddr target_pc)
{
    if (IsValidEntry () != true) {
        CountBpResult (BP_MISS);
        return BP_MISS;
    }

    BpResult bp_result;
    if (GetPredictedBranchAddr () == target_pc) {
        bp_result = BP_HIT;
    } else {
        bp_result = BP_MISS;
    }

    CountBpResult (bp_result);
    return bp_result;
}


/*!
 * @brief RASエントリのpop更新
 */
void Ras::Pop (void)
{
    if (IsValidEntry ()) {
        // POP: delete latest entry
        m_ras.pop_back ();
    }
}


/*!
 * @brief RASに有効なエントリがあるか確認
 * @return bool true=あり, false=なし
 */
bool Ras::IsValidEntry (void) const
{
    return ! (m_ras.empty ());
}


/*!
 * @brief 分岐予測された戻り先アドレスの取得
 * @return PhysAddr 分岐先アドレス
 */
PhysAddr Ras::GetPredictedBranchAddr (void) const
{
    return m_ras.back ();
}


/*!
 * @brief RASエントリの学習履歴のクリア
 */
void Ras::ClearRas (void)
{
    m_ras.clear ();
    m_last_bht_bp = BP_HIT;
}


/*!
 * @brief RAS分岐予測統計情報の初期化
 */
void Ras::ResetRasCounter (void)
{
    m_bp_hit_count  = 0;
    m_bp_miss_count = 0;
}


/*!
 * @brief RASリセット
 */
void Ras::Reset (void)
{
    ClearRas ();
    ResetRasCounter ();
}


/*!
 * @brief RASコンストラクタ
 */
Ras::Ras ()
{
    m_ras_num = p_internals->GetRasNum();
    Reset ();
}


/*!
 * @brief RASデストラクタ
 */
Ras::~Ras ()
{
}


BpResult Btac::PredictBranch (PhysAddr branch_pc, PhysAddr target_pc, bool is_taken,
        bool* is_btac_forward, bool* is_btac_uncond)
{
    // index = PC[n:3] : 64bit範囲内に1個のjarlを想定
    uint32_t entry = GetEntry (branch_pc);
    uint32_t way = GetHitWay (entry, branch_pc);
    if (way != BTAC_MISS) {
        // BTACに学習履歴あり。
        // 分岐先が正しいか判定（ただし、NotTaken分岐なら分岐先間違ってもヒット扱い)
        // - 実際:NotTaken, BHT:Takenの場合、BHT側でミス扱いになるのでここでは気にしなくてOK
        BtacEntry* btac_entry = m_btac[entry];
        *is_btac_forward = ! ((*btac_entry)[way]->IsReturn ());
        *is_btac_uncond  =    (*btac_entry)[way]->IsAlways ();
        if (IsRas (entry, way) == true) {
            // ヒット: RAS対象
            CountBpResult (BP_HIT);
            //CfMsg::DPrint (MSG_ERR, "<BTAC_RAS:e%03xw%x>\n",
            //        entry, (*btac_entry)[way]->GetWayId ());
            return m_ras->PredictBranch (target_pc); // ここだけ更新するのか。。。
        } else {
            if (IsHitBtac (entry, way, target_pc) || (is_taken == false)) {
                // ヒット
                // 条件付き分岐命令のNotTaken分岐では、BTACの分岐先を使用しないのでヒット扱い
                CountBpResult (BP_HIT);
                //CfMsg::DPrint (MSG_ERR, "<BTAC_HIT:e%03xw%x>\n",
                //        entry, (*btac_entry)[way]->GetWayId ());
                return BP_HIT;
            } else {
                // ミス
                // ただし、BTACの分岐先違いなので、同wayの分岐先記録を上書き
                CountBpResult (BP_MISS);
                ++ m_bp_update_btgt_count;
                //CfMsg::DPrint (MSG_ERR, "<BTAC_MISS_BTGT:e%03xw%x>\n",
                //        entry, (*btac_entry)[way]->GetWayId ());
                return BP_MISS;
            }
        }
    } else {
        // ミス
        // BTACの更新
        CountBpResult (BP_MISS);
        //uint32_t replace_way = GetReplaceWay ();
        //BtacEntry* btac_entry = m_btac[entry];
        //CfMsg::DPrint (MSG_ERR, "<BTAC_MISS:e%03xw%x>\n",
        //        entry, (*btac_entry)[replace_way]->GetWayId ());
        *is_btac_forward = false;
        *is_btac_uncond  = false;
        return BP_MISS;
    }
}


void Btac::Study (PhysAddr branch_pc, PhysAddr target_pc,
        bool is_taken, bool is_forward, bool is_uncond)
{
    // index = PC[n:3] : 64bit範囲内に1個のjarlを想定
    uint32_t entry = GetEntry (branch_pc);
    uint32_t way = GetHitWay (entry, branch_pc);
    if (way != BTAC_MISS) {
        // BTACに学習履歴あり。
        // 分岐先が正しいか判定（ただし、NotTaken分岐なら分岐先間違ってもヒット扱い)
        // - 実際:NotTaken, BHT:Takenの場合、BHT側でミス扱いになるのでここでは気にしなくてOK
        if (IsRas (entry, way) == true) {
            // ヒット: RAS対象
            UpdateLru (entry, way);
        } else {
            if (IsHitBtac (entry, way, target_pc) || (is_taken == false)) {
                // ヒット
                UpdateLru (entry, way);
            } else {
                // ミス (BTACの分岐先間違い)
#ifdef ALLOW_MULTI_HIT
                // LRUに従ってリプレース先を選択
                // - BTACに同TAGのエントリが複数存在することを許容する。
                //   RTLで同じエントリを判定するのが間に合わないらしい。
                uint32_t replace_way = GetReplaceWay (branch_pc);
                UpdateBtac (entry, replace_way, branch_pc, target_pc, is_forward, is_uncond);
                UpdateLru (entry, replace_way);
                UpdateRoundRobin (entry, replace_way);
#else // ALLOW_MULTI_HIT
                // BTACの分岐先違いなので、同wayの分岐先記録を上書き
                UpdateBtac (entry, way, branch_pc, target_pc, is_forward, is_uncond);
                UpdateLru (entry, way);
                // リプレースしないので、UpdateRoundRobin (entry, replace_way)はしない
#endif // ALLOW_MULTI_HIT
            }
        }
    } else {
        // ミス
        // BTACの更新
        uint32_t replace_way = GetReplaceWay (branch_pc);
        UpdateBtac (entry, replace_way, branch_pc, target_pc, is_forward, is_uncond);
        UpdateLru (entry, replace_way);
        UpdateRoundRobin (entry, replace_way);
    }
}


bool Btac::HasBtacEntry (PhysAddr branch_pc) const
{
    // index = PC[n:3] : 64bit範囲内に1個のjarlを想定
    uint32_t entry = GetEntry (branch_pc);
    uint32_t way = GetHitWay (entry, branch_pc);
    if (way != BTAC_MISS) {
        return true;
    } else {
        return false;
    }
}


// LRUとの違いはヒット時に更新しないこと。リプレース時にだけ更新。
void Btac::UpdateRoundRobin (uint32_t entry, uint32_t way)
{
#ifdef USE_BTAC_ROUNDROBIN
#ifdef BTAC_RR_EACH_ENTRY
    // ラウンドロビンのカウンタはエントリごとに存在
    if (BTAC_WAY_NUM > 1) {
        BtacEntry* btac_entry = m_btac[entry];
        BtacEntry::iterator it = btac_entry->begin ();
        rotate (it, it + 1, btac_entry->end ());
    }
#else // BTAC_RR_EACH_ENTRY
    // ラウンドロビンのカウンタは全エントリ共通で1個。
    ++ m_round_robin_counter;
    if (m_round_robin_counter >= BTAC_WAY_NUM) {
        // 上限に達したら0に戻す
        m_round_robin_counter = 0;
    }
#endif // BTAC_RR_EACH_ENTRY
#endif // USE_BTAC_ROUNDROBIN
}


void Btac::UpdateLru (uint32_t entry, uint32_t way)
{
#ifdef USE_BTAC_LRU
    if (BTAC_WAY_NUM > 1) {
        BtacEntry* btac_entry = m_btac[entry];
        BtacEntry::iterator it = btac_entry->begin () + way;
        if (it != btac_entry->end ()) {
            rotate (it, it + 1, btac_entry->end ());
        }
    }
#endif //USE_BTAC_LRU
}


/*!
 * @brief 分岐の学習
 * @param entry BTACindex
 * @param way
 * @param branch_pc 分岐元アドレス
 * @param target_pc 分岐先アドレス
 * @param is_forward
 * @param is_uncond
 *
 * @todo To be documented.
 */
void Btac::UpdateBtac (uint32_t entry, uint32_t way, PhysAddr branch_pc, PhysAddr target_pc,
        bool is_forward, bool is_uncond)
{
    BtacEntry* btac_entry = m_btac[entry];
    (*btac_entry)[way]->SetTag (branch_pc);
    (*btac_entry)[way]->SetBranchTgt (target_pc);
    (*btac_entry)[way]->SetAlways (is_uncond);
    (*btac_entry)[way]->SetReturn (! is_forward);
    (*btac_entry)[way]->SetValid (true);
}


bool Btac::IsHitBtac (uint32_t entry, uint32_t way, PhysAddr target_pc) const
{
    BtacEntry* btac_entry = m_btac[entry];
    return (*btac_entry)[way]->IsHitBranchTarget (target_pc);
}


bool Btac::IsRas (uint32_t entry, uint32_t way) const
{
    BtacEntry* btac_entry = m_btac[entry];
    return (*btac_entry)[way]->IsReturn () && (*btac_entry)[way]->IsAlways ();
}


uint32_t Btac::GetHitWay (uint32_t entry, PhysAddr branch_pc) const
{
#if defined (USE_BTAC_LRU) || defined (USE_BTAC_ROUNDROBIN)
    // マルチヒット時は最近使用したエントリを使用してみる
    // - LRU順に並んでいるので、後ろから検索すればOK
    BtacEntry::const_iterator it_start = m_btac[entry]->begin();
    BtacEntry::const_iterator it = m_btac[entry]->end ();
    while (it != it_start) {
        -- it;
        if ((*it)->IsHitTag (branch_pc)) {
            return static_cast <uint32_t> (distance (it_start, it));
        }
    }
    return BTAC_MISS;
#else // defined (USE_BTAC_LRU) || defined (USE_BTAC_ROUNDROBIN)
    BtacEntry* btac_entry = m_btac[entry];
    uint32_t way = (branch_pc >> BTAC_WAY_SHIFT) & BTAC_WAY_MASK;
    if ((*btac_entry)[way]->IsHitTag (branch_pc) == true) {
        return way;
    } else {
        return BTAC_MISS;
    }
#endif // defined (USE_BTAC_LRU) || defined (USE_BTAC_ROUNDROBIN)
}


uint32_t Btac::GetEntry (PhysAddr branch_pc) const
{
    return (branch_pc >> BTAC_ENTRY_SHIFT) & BTAC_ENTRY_MASK;
}


uint32_t Btac::GetReplaceWay (PhysAddr branch_pc) const
{
#if defined (USE_BTAC_LRU) || defined (USE_BTAC_ROUNDROBIN)
#if defined (USE_BTAC_LRU) || defined (BTAC_RR_EACH_ENTRY)
    // 常にvectorの先頭がreplace対象
    return 0;
#else // defined (USE_BTAC_LRU) || defined (BTAC_RR_EACH_ENTRY)
    // round_robinカウンタにより、replace先のwayを選択
    return m_round_robin_counter;
#endif // defined (USE_BTAC_LRU) || defined (BTAC_RR_EACH_ENTRY)
#else // defined (USE_BTAC_LRU) || defined (USE_BTAC_ROUNDROBIN)
    return (branch_pc >> BTAC_WAY_SHIFT) & BTAC_WAY_MASK;
#endif // defined (USE_BTAC_LRU) || defined (USE_BTAC_ROUNDROBIN)
}

/*!
 * @brief 分岐予測統計情報のクリア
 */
void Btac::ResetBtacCounter (void)
{
    m_bp_hit_count  = 0;
    m_bp_miss_count = 0;
    m_bp_update_btgt_count = 0;
}


/*!
 * @brief 分岐予測学習内容のクリア
 */
void Btac::ClearBtac (void)
{
    std::vector<BtacEntry*>::iterator it = m_btac.begin();
    while (it != m_btac.end()) {
        BtacEntry* btac_entry = *it;
        std::vector<BtacWay*>::iterator it_way = btac_entry->begin();
        while (it_way != btac_entry->end()) {
            (*it_way)->Init ();
            ++it_way;
        }
        ++it;
    }

    m_round_robin_counter = 0;
}


void Btac::CreateBtac (void)
{
    uint32_t entry;
    for (entry = 0; entry < BTAC_ENTRY_NUM; entry++) {
        BtacEntry* btac_entry = new BtacEntry ();
        uint32_t way;
        for (way = 0; way < BTAC_WAY_NUM; way++) {
            btac_entry->push_back (new BtacWay (way));
        }
        m_btac.push_back (btac_entry);
    }
}


void Btac::DeleteBtac (void)
{
    std::vector<BtacEntry*>::iterator it = m_btac.begin();
    while (it != m_btac.end()) {
        BtacEntry* btac_entry = *it;
        std::vector<BtacWay*>::iterator it_way = btac_entry->begin();
        while (it_way != btac_entry->end()) {
            delete *it_way;
            ++it_way;
        }
        delete btac_entry;
        ++it;
    }
}


/*!
 * @brief BTACリセット
 */
void Btac::Reset (void)
{
    ClearBtac ();
    ResetBtacCounter ();
}


/*!
 * @brief BTACコンストラクタ
 * @param ras
 *
 * @todo To be documented.
 */
Btac::Btac (class Ras* ras)
{
    m_ras = ras;
    CreateBtac ();
    Reset ();
}


/*!
 * @brief BTACデストラクタ
 */
Btac::~Btac ()
{
    DeleteBtac ();
}


bool Btac::BtacWay::IsHitTag (uint32_t branch_pc) const
{
    return (m_tag == (branch_pc & BTAC_TAG_MASK)) && m_v;
}


bool Btac::BtacWay::IsHitBranchTarget (uint32_t branch_addr) const
{
    return (m_branch_tgt == branch_addr) && m_v;
}


void Btac::BtacWay::Init (void)
{
    m_tag        = 0;
    m_branch_tgt = 0;
    m_always     = false;
    m_return     = false;
    m_v          = false;
}


/*!
 * @brief 分岐予測結果の取得
 * @return BpResult 分岐予測HIT/MISS
 */
BpResult BUncond::PredictBranch (void)
{
    CountBpResult (BP_HIT);
    return BP_HIT;
}


/*!
 * @brief 分岐予測統計情報のクリア
 */
void BUncond::ResetCounter (void)
{
    m_bp_hit_count  = 0;
    m_bp_miss_count = 0;
}


/*!
 * @brief BTACリセット
 */
void BUncond::Reset (void)
{
    ResetCounter ();
}


/*!
 * @brief BUncondコンストラクタ
 */
BUncond::BUncond ()
{
    Reset ();
}


/*!
 * @brief BUncondデストラクタ
 */
BUncond::~BUncond ()
{
}


/*!
 * @brief BHT予測器の予測結果の取得と学習
 */
BpResult BtbG3MH::PredictBranchAndStudyBht (PhysAddr branch_pc, bool is_taken, bool is_forward_disp)
{
    BpResult bp_result = m_bhtu->PredictBranch (branch_pc, is_taken, is_forward_disp);
    m_bhtu->Study (branch_pc, is_taken, is_forward_disp);

#ifdef CLAER_RAS_BY_BPMISS
    m_rasu->m_last_bht_bp = bp_result;
#endif // #ifdef CLAER_RAS_BY_BPMISS

    return bp_result;
}


/*!
 * @brief BHT予測器のGHR更新
 */
void BtbG3MH::UpdateGhr (bool is_taken)
{
    m_bhtu->UpdateGhr (is_taken);
}


void BtbG3MH::ReserveUpdateGhr (bool is_taken)
{
    m_bhtu->ReserveUpdateGhr (is_taken);
}

/*!
 * @brief RAS予測器の学習
 */
void BtbG3MH::StudyRas (PhysAddr return_pc)
{
#ifdef CLAER_RAS_BY_BPMISS
    m_rasu->m_last_bht_bp = BP_HIT;
#endif // #ifdef CLAER_RAS_BY_BPMISS
    m_rasu->Study (return_pc);
}


/*!
 * @brief RAS予測結果の取得とRAS予測エントリのpop更新
 */
BpResult BtbG3MH::PredictBranchAndPopRas (PhysAddr target_pc)
{
#ifdef CLAER_RAS_BY_BPMISS
    if (m_rasu->m_last_bht_bp == BP_MISS) {
        m_rasu->ClearRas ();
    }
#endif // #ifdef CLAER_RAS_BY_BPMISS

    BpResult bp_result = m_rasu->PredictBranch (target_pc);
    m_rasu->Pop ();
    return bp_result;
}


/*!
 * @brief BTAC予測結果の取得と学習
 */
BpResult BtbG3MH::PredictBranchAndStudyBtac (PhysAddr branch_pc, PhysAddr target_pc)
{
    bool is_btac_forward;
    bool is_btac_uncond;
    BpResult bp_result = m_btacu->PredictBranch (branch_pc, target_pc, true,
            &is_btac_forward, &is_btac_uncond);
    m_btacu->Study (branch_pc, target_pc, true, true, true);
    return bp_result;
}


BpResult BtbG3MH::PredictBranch (void)
{
    return m_bunc->PredictBranch ();
}


BpResult BtbG3MH::PredictBranchAndStudyBtacBht (PhysAddr branch_pc, PhysAddr target_pc,
        bool is_taken, bool is_forward_taken, bool is_uncond)
{
    // 予測
    BpResult bp_result;
    bool has_btac = m_btacu->HasBtacEntry (branch_pc);
    if (has_btac == true) {
        bool is_btac_forward = false;
        bool is_btac_uncond = false;
        BpResult btac_result = m_btacu->PredictBranch (branch_pc, target_pc, is_taken,
                &is_btac_forward, &is_btac_uncond);

        BpResult bht_result;
        if (is_btac_uncond == true) {
            // BHTを使用しない
            bht_result = BP_HIT;
        } else {
            // BTACエントリ内容に従い、BHTを引く
            bht_result = m_bhtu->PredictBranch (branch_pc, is_taken, is_btac_forward);
        }

        if ((btac_result == BP_HIT) && (bht_result == BP_HIT)) {
            bp_result = BP_HIT;
        } else {
            bp_result = BP_MISS;
        }

        if ((is_btac_uncond == true) && (is_btac_forward == true)) {
            // BTAC予測 (無条件分岐)
            if (bp_result == BP_HIT) {
                ++ m_btac_hit_btac_hit;
            } else {
                ++ m_btac_hit_btac_miss;
            }
        } else if ((is_btac_uncond == true) && (is_btac_forward == false)) {
            // RAS予測 (return分岐)
            if (bp_result == BP_HIT) {
                ++ m_btac_hit_ras_hit;
            } else {
                ++ m_btac_hit_ras_miss;
            }
        } else {
            // BHT予測
            if (bp_result == BP_HIT) {
                ++ m_btac_hit_bht_hit;
            } else {
                if ((bht_result == BP_HIT) && (btac_result == BP_MISS)) {
                    ++ m_btac_hit_bht_hit_btgt_miss;
                } else if ((bht_result == BP_MISS) && (btac_result == BP_HIT)) {
                    ++ m_btac_hit_bht_miss_btgt_hit;
                } else {
                    ++ m_btac_hit_bht_miss_btgt_miss;
                }
            }
        }
    } else {
        // BTACにエントリがないので、NotTaken予測
        if (is_taken == false) {
            bp_result = BP_HIT;
            ++ m_btac_miss_ntkn_hit;
        } else {
            bool temp;
            // BPHIT/MISSのカウント＆表示用
            m_btacu->PredictBranch (branch_pc, target_pc, is_taken, &temp, &temp);
            bp_result = BP_MISS;
            ++ m_btac_miss_tkn_miss;
        }
    }
    CountBpResult (bp_result);

    // 学習
    if ((has_btac == false) && (is_taken == false)) {
        // BTACにないNotTaken分岐は学習されない
    } else {
        m_btacu->Study (branch_pc, target_pc, is_taken, is_forward_taken, is_uncond);
        if (! is_uncond) {
            m_bhtu->Study (branch_pc, is_taken, is_forward_taken);
        }
    }
    // RAS更新 (BTACの内容に関係なく、jmp[r31]で更新)
    if ((is_uncond == true) && (is_forward_taken == false)) {
        m_rasu->Pop ();
    }
    return bp_result;
}


/*!
 * @brief BRUアクセス（予測）回数の取得
 */
uint64_t BtbG3MH::GetBtbAccessCount (void) const
{
#ifdef USE_HYBRID_BP
    return m_bhtu->GetAccessCount () + m_rasu->GetAccessCount () + m_btacu->GetAccessCount ();
#else // USE_HYBRID_BP
    return m_bp_hit_count + m_bp_miss_count;
#endif //USE_HYBRID_BP
}


/*!
 * @brief BRUヒット回数の取得
 */
uint64_t BtbG3MH::GetBtbBpHitCount (void) const
{
#ifdef USE_HYBRID_BP
    return m_bhtu->GetHitCount () + m_rasu->GetHitCount () + m_btacu->GetHitCount ();
#else // USE_HYBRID_BP
    return m_bp_hit_count;
#endif //USE_HYBRID_BP
}


/*!
 * @brief BRUミス回数取得
 */
uint64_t BtbG3MH::GetBtbBpMissCount (void) const
{
#ifdef USE_HYBRID_BP
    return m_bhtu->GetMissCount () + m_rasu->GetMissCount () + m_btacu->GetMissCount ();
#else // USE_HYBRID_BP
    return m_bp_miss_count;
#endif //USE_HYBRID_BP
}


/*!
 * @brief BRU予測統計情報のクリア
 */
void BtbG3MH::ResetPerfInfo (void)
{
    m_bhtu->ResetBhtCounter ();
    m_rasu->ResetRasCounter ();
    m_btacu->ResetBtacCounter ();
    m_bunc->ResetCounter ();

    // for BTAC with BHT
    m_bp_hit_count = 0;
    m_bp_miss_count = 0;

    // 詳細カウント
    m_btac_hit_bht_hit = 0;
    m_btac_hit_bht_hit_btgt_miss = 0;
    m_btac_hit_bht_miss_btgt_hit = 0;
    m_btac_hit_bht_miss_btgt_miss = 0;
    m_btac_hit_btac_hit = 0;
    m_btac_hit_btac_miss = 0;
    m_btac_hit_ras_hit = 0;
    m_btac_hit_ras_miss = 0;
    m_btac_miss_ntkn_hit = 0;
    m_btac_miss_tkn_miss = 0;
}


/*!
 * @brief BRU学習内容のクリア
 */
void BtbG3MH::ClearBtb (void)
{
    m_bhtu->ClearBht ();
    m_rasu->ClearRas ();
    m_btacu->ClearBtac ();
}


/*!
 * @brief BRUリセット
 */
void BtbG3MH::Reset (void)
{
    ClearBtb ();
    ResetPerfInfo ();
}


/*!
 * @brief BRU分岐予測統計情報の表示
 */
void BtbG3MH::ShowPerfInfo (void) const
{
    uint64_t hit = GetBtbBpHitCount ();
    uint64_t miss = GetBtbBpMissCount ();
    uint64_t access = GetBtbAccessCount ();

    if (access != 0) { // 寝ていたPE/TCでは表示しない
        std::ostringstream oss;
        oss << "P" << std::setw(1) << m_peid;

        double hit_rate = (static_cast<double>(hit) / static_cast<double>(access)) * 100.0;
        CfMsg::DPrint (MSG_ERR,
                "  Total Branch Prediction (%s): Hit Rate=%3.3f%% (Hit=%8" PRIu64
                ", Miss=%8" PRIu64 ")\n",
                oss.str().c_str(), hit_rate, hit, miss);

        // BHT
        uint64_t bht_hit = m_bhtu->GetHitCount ();
        uint64_t bht_miss = m_bhtu->GetMissCount ();
        uint64_t bht_access = m_bhtu->GetAccessCount ();

        double bht_hit_rate = (static_cast<double>(bht_hit) /
                                                   static_cast<double>(bht_access)) * 100.0;
        CfMsg::DPrint (MSG_ERR,
                "    BHT Prediction : Hit Rate=%3.3f%% (Hit=%8" PRIu64 ", Miss=%8" PRIu64 ")\n",
                bht_hit_rate, bht_hit, bht_miss);

        // RAS
        uint64_t ras_hit = m_rasu->GetHitCount ();
        uint64_t ras_miss = m_rasu->GetMissCount ();
        uint64_t ras_access = m_rasu->GetAccessCount ();

        double ras_hit_rate = (static_cast<double>(ras_hit) /
                                                   static_cast<double>(ras_access)) * 100.0;
        CfMsg::DPrint (MSG_ERR,
                "    RAS Prediction : Hit Rate=%3.3f%% (Hit=%8" PRIu64 ", Miss=%8" PRIu64 ")\n",
                ras_hit_rate, ras_hit, ras_miss);

        // BTAC
        uint64_t btac_hit = m_btacu->GetHitCount ();
        uint64_t btac_miss = m_btacu->GetMissCount ();
        uint64_t btac_access = m_btacu->GetAccessCount ();

        double btac_hit_rate = (static_cast<double>(btac_hit) /
                                                    static_cast<double>(btac_access)) * 100.0;
#ifdef USE_HYBRID_BP
        CfMsg::DPrint (MSG_ERR,
                "    BTAC Prediction : Hit Rate=%3.3f%% (Hit=%8" PRIu64 ", Miss=%8" PRIu64 ")\n",
                btac_hit_rate, btac_hit, btac_miss);
#else // USE_HYBRID_BP
        CfMsg::DPrint (MSG_ERR,
                "    BTAC Prediction : Hit Rate=%3.3f%% (Hit=%8" PRIu64 ", Miss=%8" PRIu64
                " (BranchTargetMiss=%8" PRIu64 "))\n",
                btac_hit_rate, btac_hit, btac_miss, m_btacu->m_bp_update_btgt_count);
#endif // USE_HYBRID_BP

#ifdef USE_HYBRID_BP
        // BUncond
        uint64_t bunc_access = m_bunc->GetAccessCount ();
        CfMsg::DPrint (MSG_ERR, "    Unconditional branch count=%8" PRIu64 "\n", bunc_access);

        //double hit_rate_total_unc =
        //        ((double)(hit + bunc_access) / (double)(access + bunc_access)) * 100.0;
        //CfMsg::DPrint (MSG_ERR,
        //        "  Total Branch Prediction (inc BUnc) : Hit Rate=%3.3f%% (Hit=%8" PRIu64
        //        ", Miss=%8" PRIu64 ")\n",
        //        hit_rate_total_unc, hit + bunc_access, miss);
#endif // USE_HYBRID_BP

#ifdef USE_HYBRID_BP
#else // USE_HYBRID_BP
    // BTACの詳細表示
    CfMsg::DPrint (MSG_ERR, "BHT-HIT, BHT-HIT-BTGT-MISS, BHT-MISS-BTGT-HIT, BHT-MISS-BTGT-MISS, "
            "BTAC-BTGT-HIT, BTAC-BTGT-MISS, RAS-HIT, RAS-MISS, BTAC-MISS-NTKN, BTC-MISS-TKN\n");
    CfMsg::DPrint (MSG_ERR,
            "Detail BP,%" PRIu64 ",%" PRIu64 ",%" PRIu64 ",%" PRIu64 ",%" PRIu64
            ",%" PRIu64",%" PRIu64",%" PRIu64 ",%" PRIu64 ",%" PRIu64 "\n",
            m_btac_hit_bht_hit, m_btac_hit_bht_hit_btgt_miss,
            m_btac_hit_bht_miss_btgt_hit, m_btac_hit_bht_miss_btgt_miss,
            m_btac_hit_btac_hit, m_btac_hit_btac_miss,
            m_btac_hit_ras_hit, m_btac_hit_ras_miss,
            m_btac_miss_ntkn_hit, m_btac_miss_tkn_miss
                   );
#endif //#ifdef USE_HYBRID_BP
    }
}


/*!
 * @brief BRUコンストラクタ
 * @param peid
 * @param tcid
 *
 * @todo To be documented.
 */
BtbG3MH::BtbG3MH (uint32_t peid, ContextId tcid)
    : m_peid (peid), m_tcid (tcid)
{
    m_bhtu = new Bht ();
    m_rasu = new Ras ();
    m_btacu = new Btac (m_rasu);
    m_bunc = new BUncond ();
    Reset ();
}


/*!
 * @brief BRUデストラクタ
 */
BtbG3MH::~BtbG3MH ()
{
    delete m_bhtu;
    delete m_rasu;
    delete m_btacu;
    delete m_bunc;
}


/*!
 * @brief Branch prediction judgment and branch history learning
 * @param branch_pc Branch source address
 * @param target_pc Branch target address
 * @param is_taken Previous result of branch instruction
 * @return BpResult
 */
BpResult BtbG3M::PredictBranchAndStudy (PhysAddr branch_pc, PhysAddr target_pc, bool is_taken)
{
    for (uint32_t entry = 0; entry < BTB_ENTRY_NUM; entry++) {
        BtbLine* btb_line = m_btb_lines[entry];
        if (btb_line->IsEntryHit (branch_pc)) {
            //When a BTB entry is hit,
            //Get 2 bits saturation counter with reference to GHR and perform branch prediction hit miss judgment.
            BpResult bp_result = PredictBranch (btb_line->GetPht (m_ghr), is_taken);
            UpdateBtbPht (entry, is_taken); //2 bits saturation counter update
            UpdateBtbLru (entry); //LRU update
            CountBpResult (bp_result);
            return bp_result;
        }
    }
    //If the BTB entry not hit,
    //Predict as Not-Taken when fetching
    if (is_taken == true) {
        //Because the branch instruction was Taken, the branch prediction error
        //Need to learn branch history
        uint32_t replace_entry = GetBtbEntryFromLru (); //Replacement entry identification
        m_btb_lines[replace_entry]->Study (branch_pc, target_pc);
        UpdateBtbPht (replace_entry, is_taken); //2 bits saturation counter update
        UpdateBtbLru (replace_entry); //LRU update
        CountBpResult (BP_MISS);
        return BP_MISS;
    } else {
        //Because the branch instruction was Not-Taken, branch prediction hit
        //No need to learn branch history
        CountBpResult (BP_HIT);
        return BP_HIT;
    }
}


/*!
 * @brief GHR update
 * @param is_taken Previous result of branch instruction
 * @return -
 */
void BtbG3M::UpdateGhr (bool is_taken)
{
    //GHR is updated even for branch instructions other than learning targets
    m_ghr = (m_ghr << 1) & GHR_MASK;
    if (is_taken) {
        // 分岐時は1をシフトイン
        m_ghr |= 1;
    } else {
        // 非分岐時は0をシフトイン
    }
}



/*!
 * @brief Branch prediction hit / miss judgment
 * @param ph 2bit saturation counter
 * @param is_taken Branch prediction result
 * @return BpResult
 */
BpResult BtbG3M::PredictBranch (uint8_t ph, bool is_taken) const
{
    // Pattern history table (2-bit saturated counter array)
    // 0x01:Weakly Taken,       0x11:Strongly Taken
    // 0x10:Strongly Not-Taken, 0x00:Weakly Not-Taken
    if (is_taken) {
        if ((ph & 0x01) != 0) {
            return BP_HIT;
        } else {
            return BP_MISS;
        }
    } else {
        if ((ph & 0x01) != 0) {
            return BP_MISS;
        } else {
            return BP_HIT;
        }
    }
}


/*!
 * @brief Update pattern history table (2-bit saturated counter array)
 * @param entry BTB Entry number
 * @param is_taken Branch prediction result
 * @return -
 */
void BtbG3M::UpdateBtbPht (uint32_t entry, bool is_taken)
{
    BtbLine* btb_line = m_btb_lines[entry];
    uint8_t ph = btb_line->GetPht (m_ghr);

    if (is_taken) {
        switch (ph) {
        case WNTKN: btb_line->SetPht (m_ghr, WTKN);  break;
        case WTKN:  btb_line->SetPht (m_ghr, STKN);  break;
        case SNTKN: btb_line->SetPht (m_ghr, WNTKN); break;
        case STKN:  btb_line->SetPht (m_ghr, STKN);  break;
        default: break;
        }
    } else {
        switch (ph) {
        case WNTKN: btb_line->SetPht (m_ghr, SNTKN); break;
        case WTKN:  btb_line->SetPht (m_ghr, WNTKN); break;
        case SNTKN: btb_line->SetPht (m_ghr, SNTKN); break;
        case STKN:  btb_line->SetPht (m_ghr, WTKN);  break;
        default: break;
        }
    }
}


/*!
 * @brief LRU update
 * @param entry BTB entry number
 * @return -
 */
void BtbG3M::UpdateBtbLru (uint32_t entry)
{
    static const uint32_t lru_on_mask[]  = { 0x38, 0x06, 0x01, 0x00 };
    static const uint32_t lru_off_mask[] = { 0x3F, 0x1F, 0x2B, 0x34 };

    m_btb_lru = (m_btb_lru | lru_on_mask[entry]) & lru_off_mask[entry];
}


/*!
 * @brief Specify replacement BTB entry
 * @param -
 * @return uint32_t BTB entry number
 */
uint32_t BtbG3M::GetBtbEntryFromLru (void) const
{
    static const uint32_t lru_sel_mask[] = { 0x38, 0x26, 0x15, 0x0B };
    static const uint32_t lru_chk_val[]  = { 0x00, 0x20, 0x14, 0x0B };

    uint32_t lru_data = m_btb_lru;
    if ((lru_data & lru_sel_mask[0]) == lru_chk_val[0]) { return 0; }
    if ((lru_data & lru_sel_mask[1]) == lru_chk_val[1]) { return 1; }
    if ((lru_data & lru_sel_mask[2]) == lru_chk_val[2]) { return 2; }
    if ((lru_data & lru_sel_mask[3]) == lru_chk_val[3]) { return 3; }
    return BTB_MISS;
}


/*!
 * @brief BTB clear
 * @param void
 * @return void
 */
void BtbG3M::ClearBtb (void)
{
    BtbLineVec::iterator ite = m_btb_lines.begin();
    while (ite != m_btb_lines.end()) {
        (*ite)->Invalidate();
        ++ite;
    }
    m_ghr = 0;
    m_btb_lru = 0;
}


/*!
 * @brief BTB reset
 * @param void
 * @return void
 */
void BtbG3M::Reset (void)
{
    ClearBtb ();
    ResetBtbCounter ();
}


/*!
 * @brief Clear forecast statistics
 */
void BtbG3M::ResetPerfInfo (void)
{
    ResetBtbCounter ();
}


/*!
 * @brief Display branch prediction statistics
 * @param void
 * @return void
 */
void BtbG3M::ShowPerfInfo (void) const
{
    uint64_t access = GetBtbAccessCount ();

    if (access != 0) { // Not displayed on sleeping PE/TC
        std::ostringstream oss;
        oss << "P" << std::setw(1) << m_peid;

        uint64_t hit = GetBtbBpHitCount ();
        uint64_t miss = GetBtbBpMissCount ();

        double hit_rate = (static_cast<double>(hit) / static_cast<double>(access)) * 100.0;
        CfMsg::DPrint (MSG_ERR,
                "  Total Branch Prediction (%s): Hit Rate=%3.3f%% (Hit=%8" PRIu64
                ", Miss=%8" PRIu64 ")\n",
                oss.str().c_str(), hit_rate, hit, miss);
    }
}


/*!
 * @brief Constructor
 * @param peid PE index number
 * @param tcid Thresh context ID
 * @return -
 */
BtbG3M::BtbG3M (uint32_t peid, ContextId tcid)
    : m_peid (peid), m_tcid (tcid)
{
    for (uint32_t i = 0; i < BTB_ENTRY_NUM; i++) {
        m_btb_lines.push_back (new BtbLine ());
    }
    m_ghr = 0;
    m_btb_lru = 0;
    m_bp_hit_count = 0;
    m_bp_miss_count = 0;
}


/*!
 * @brief Destructor
 * @param -
 * @return -
 */
BtbG3M::~BtbG3M (void)
{
    BtbLineVec::iterator ite = m_btb_lines.begin();
    while (ite != m_btb_lines.end()) {
        delete *ite;
        ++ite;
    }
}
