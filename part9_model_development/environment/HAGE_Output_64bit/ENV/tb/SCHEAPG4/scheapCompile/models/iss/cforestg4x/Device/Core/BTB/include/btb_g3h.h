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

#include <cstring>
#include <vector>
#include "./trace_info.h"
#include "./btb_base.h"

/*!
 * @brief Branch History Table
 */
class Bht {
public:
    BpResult PredictBranch (PhysAddr branch_pc, bool is_taken, bool is_forward_disp);
    void Study (PhysAddr branch_pc, bool is_taken, bool is_forward_disp);
    void UpdateGhr (bool is_taken);
    void ReserveUpdateGhr (bool is_taken);

    void ClearBht (void);
    void Reset (void);

    uint64_t GetAccessCount (void) const { return m_bp_hit_count + m_bp_miss_count; }
    uint64_t GetHitCount (void) const { return m_bp_hit_count; }
    uint64_t GetMissCount (void) const { return m_bp_miss_count; }
    void ResetBhtCounter (void);
    explicit Bht ();
    ~Bht ();

private:
    static const uint32_t BHT_WAY_NUM = 4; // WAYはPC[2:1]により選択
    static const uint32_t BHT_ENTRY_NUM = 128;  //BHTエントリ数
    static const uint32_t BHT_ENTRY_MASK = BHT_ENTRY_NUM - 1;
#ifdef USE_6BIT_GHR
    static const uint32_t GHR_BITS  = 6;    //GHRビット数
#else
    static const uint32_t GHR_BITS  = 8;    //GHRビット数
#endif
    static const uint32_t GHR_MASK  = (1<<GHR_BITS)-1; //GHRビットマスク

    typedef uint32_t BhtEntryType;
    static const BhtEntryType BHT_SNAGREE;
    static const BhtEntryType BHT_WNAGREE;
    static const BhtEntryType BHT_WAGREE;
    static const BhtEntryType BHT_SAGREE;

    std::vector<BhtEntryType> m_bht; //!< Branch History Table

    uint32_t m_ghr; //!< Global History Register
    uint32_t m_reserved_ghr;

    uint64_t m_bp_hit_count;
    uint64_t m_bp_miss_count;

    uint32_t GetBhtEntry (PhysAddr branch_pc) const;
    bool IsPredictBranchAgree (uint32_t entry) const;
    bool IsAgree (bool is_taken, bool is_forward_disp) const;
    void UpdateBht (uint32_t entry, bool is_taken);
    void CountBpResult (BpResult bp_result) {
        if (bp_result == BP_HIT) {
            ++ m_bp_hit_count;
        } else {
            ++ m_bp_miss_count;
        }
    }
};

/*!
 * @brief Return Address Stack
 */
class Ras {
public:
    void Study (PhysAddr return_pc);
    BpResult PredictBranch (PhysAddr target_pc);
    void Pop (void);

    uint64_t GetAccessCount (void) const { return m_bp_hit_count + m_bp_miss_count; }
    uint64_t GetHitCount (void) const { return m_bp_hit_count; }
    uint64_t GetMissCount (void) const { return m_bp_miss_count; }
    void ResetRasCounter (void);

    void ClearRas (void);
    void Reset (void);

    BpResult m_last_bht_bp;

    explicit Ras ();
    ~Ras ();

private:
    uint32_t m_ras_num;
    std::vector<PhysAddr> m_ras;

    uint64_t m_bp_hit_count;
    uint64_t m_bp_miss_count;

    bool IsValidEntry (void) const;
    PhysAddr GetPredictedBranchAddr (void) const;

    void CountBpResult (BpResult bp_result) {
        if (bp_result == BP_HIT) {
            ++ m_bp_hit_count;
        } else {
            ++ m_bp_miss_count;
        }
    }
};


/*!
 * @brief Branch Target Address Cache
 */
class Btac {
public:
    BpResult PredictBranch (PhysAddr branch_pc, PhysAddr target_pc,
            bool is_taken, bool* is_btac_forward, bool* is_btac_uncond);
    void Study (PhysAddr branch_pc, PhysAddr target_pc,
            bool is_taken, bool is_forward_taken, bool is_uncond);
    bool HasBtacEntry (PhysAddr branch_pc) const;

    uint64_t GetAccessCount (void) const { return m_bp_hit_count + m_bp_miss_count; }
    uint64_t GetHitCount (void) const { return m_bp_hit_count; }
    uint64_t GetMissCount (void) const { return m_bp_miss_count; }
    void ResetBtacCounter (void);

    void ClearBtac (void);
    void Reset (void);

    explicit Btac (class Ras* ras);
    ~Btac ();

    uint64_t m_bp_update_btgt_count;

private:
    static const uint32_t BTAC_ENTRY_MASK = BTAC_ENTRY_NUM - 1;
    static const PhysAddr BTAC_TAG_MASK = 0xFFFFFFFE; // TAGとoffset(PC[2:1])も含めて使用する
    static const uint32_t BTAC_MISS = 0xFFFFFFFF;

    static const PhysAddr BTAC_WAY_MASK = BTAC_WAY_NUM - 1; // WAY4=PC[2:1], WAY2=PC[1]
    static const PhysAddr BTAC_WAY_SHIFT = 1;
#if defined (USE_BTAC_LRU) || defined (USE_BTAC_ROUNDROBIN)
    static const PhysAddr BTAC_ENTRY_SHIFT = 3;
#else // defined (USE_BTAC_LRU) || defined (USE_BTAC_ROUNDROBIN)
  // ダイレクトマップについてはWAY数4,2のみサポート。WAY数1ならLRU方式でWAY数1にするのと同じ。
  #if BTAC_WAY_NUM == 4
    static const PhysAddr BTAC_ENTRY_SHIFT = 3;
  #elif BTAC_WAY_NUM == 2
    static const PhysAddr BTAC_ENTRY_SHIFT = 2;
  #else
    static const PhysAddr BTAC_ENTRY_SHIFT = 1;
  #endif // BTAC_WAY_NUM
#endif // defined (USE_BTAC_LRU) || defined (USE_BTAC_ROUNDROBIN)

    uint32_t m_round_robin_counter;

    class Ras* m_ras;

    class BtacWay {
    private:
        uint32_t m_tag;        // タグアドレスPC[2:1]も含む
        uint32_t m_branch_tgt; // 分岐先アドレス
        bool     m_always;     // 無条件分岐
        bool     m_return;     // return分岐ビット (jmp[r31]だけでなく、BackWardのbcondも含む)
        bool     m_v;          // 有効ビット
        uint32_t m_way;

    public:
        bool IsHitTag (uint32_t tag) const;
        bool IsHitBranchTarget (uint32_t branch_addr) const;
        void Init (void);

        uint32_t GetTag       (void) const { return m_tag & BTAC_TAG_MASK; }
        uint32_t GetBranchTgt (void) const { return m_branch_tgt; }
        bool     IsAlways     (void) const { return m_always; }
        bool     IsReturn     (void) const { return m_return; }
        bool     IsValid      (void) const { return m_v; }
        uint32_t GetWayId     (void) const { return m_way; }

        void SetTag (uint32_t tag) { m_tag = tag; }
        void SetBranchTgt (uint32_t branch_addr) { m_branch_tgt = branch_addr; }
        void SetAlways (bool always) { m_always = always; }
        void SetReturn (bool is_return) { m_return = is_return; }
        void SetValid  (bool v) { m_v = v; }
        explicit BtacWay (uint32_t way) {
            m_way = way;
            Init ();
        }
        ~ BtacWay () {}
    };
    typedef std::vector<BtacWay*> BtacEntry;
    std::vector<BtacEntry*> m_btac;

    bool IsHitBtac (uint32_t entry, uint32_t way, PhysAddr target_pc) const;
    uint32_t GetHitWay (uint32_t entry, PhysAddr branch_pc) const;
    void UpdateBtac (uint32_t entry, uint32_t way, PhysAddr branch_pc, PhysAddr target_pc,
            bool is_forward, bool is_uncond);
    uint32_t GetEntry (PhysAddr branch_pc) const;
    uint32_t GetReplaceWay (PhysAddr branch_pc) const;
    void UpdateRoundRobin (uint32_t entry, uint32_t way);
    void UpdateLru (uint32_t entry, uint32_t way);
    bool IsRas (uint32_t entry, uint32_t way) const;

    void CreateBtac (void);
    void DeleteBtac (void);

    uint64_t m_bp_hit_count;
    uint64_t m_bp_miss_count;
    void CountBpResult (BpResult bp_result) {
        if (bp_result == BP_HIT) {
            ++ m_bp_hit_count;
        } else {
            ++ m_bp_miss_count;
        }
    }
};


class BUncond {
public:
    BpResult PredictBranch (void);

    uint64_t GetAccessCount (void) const { return m_bp_hit_count + m_bp_miss_count; }
    uint64_t GetHitCount (void) const { return m_bp_hit_count; }
    uint64_t GetMissCount (void) const { return m_bp_miss_count; }
    void ResetCounter (void);

    void Reset (void);

    explicit BUncond ();
    ~BUncond ();

private:
    uint64_t m_bp_hit_count;
    uint64_t m_bp_miss_count;

    void CountBpResult (BpResult bp_result) {
        if (bp_result == BP_HIT) {
            ++ m_bp_hit_count;
        } else {
            ++ m_bp_miss_count;
        }
    }
};


class BtbG3MH : public Btb {
public:
    // BHT
    BpResult PredictBranchAndStudyBht (PhysAddr branch_pc, bool is_taken, bool is_forward);
    void UpdateGhr (bool is_taken);
    void ReserveUpdateGhr (bool is_taken);

    // G3M BHT unused.
    BpResult PredictBranchAndStudy (PhysAddr branch_pc, PhysAddr target_pc, bool is_taken)
    { return BP_MISS; }

    // RAS
    void StudyRas (PhysAddr return_pc);
    BpResult PredictBranchAndPopRas (PhysAddr target_pc);

    // BTAC
    BpResult PredictBranchAndStudyBtac (PhysAddr branch_pc, PhysAddr target_pc);

    // BUncond
    BpResult PredictBranch (void);

    // BTAC and BHT
    BpResult PredictBranchAndStudyBtacBht (PhysAddr branch_pc, PhysAddr target_pc,
            bool is_taken, bool is_forward, bool is_uncond);

    void ClearBtb (void);
    void Reset (void);

    void ShowPerfInfo (void) const;
    void ResetPerfInfo (void);
    BtbG3MH (uint32_t peid, ContextId tcid);
    ~BtbG3MH ();

private:
    Bht* m_bhtu;
    Ras* m_rasu;
    Btac* m_btacu;
    BUncond* m_bunc;

    const uint32_t m_peid;
    const ContextId m_tcid;

    uint64_t m_bp_hit_count;
    uint64_t m_bp_miss_count;
    void CountBpResult (BpResult bp_result) {
        if (bp_result == BP_HIT) {
            ++ m_bp_hit_count;
        } else {
            ++ m_bp_miss_count;
        }
    }

    uint64_t GetBtbAccessCount (void) const;
    uint64_t GetBtbBpHitCount (void) const;
    uint64_t GetBtbBpMissCount (void) const;

    // 詳細分けてカウントしてみる
    uint64_t m_btac_hit_bht_hit;  // BHT対象命令予測ヒット
    // BHT予測が正しかった場合に、BTAC予測はヒットしたのか？というのを見たいので細分化。
    uint64_t m_btac_hit_bht_hit_btgt_miss; // BHT対象命令予測ミス(BHTヒット、BTAC分岐先ミス)
    uint64_t m_btac_hit_bht_miss_btgt_hit; // BHT対象命令予測ミス(BHTミス、BTAC分岐先ヒット)
    uint64_t m_btac_hit_bht_miss_btgt_miss; // BHT対象命令予測ミス(BHTミス、BTAC分岐先ミス)
    uint64_t m_btac_hit_btac_hit; // BTAC対象命令予測ヒット
    uint64_t m_btac_hit_btac_miss; // BTAC対象命令予測ミス(BTAC分岐先ミス)
    uint64_t m_btac_hit_ras_hit; // RAS対象命令予測ヒット
    uint64_t m_btac_hit_ras_miss; // RAS対象命令予測ミス(RASミス)
    uint64_t m_btac_miss_ntkn_hit; // 予測ヒット(BTACミスだけど、NTKN分岐だったので静的ヒット)
    uint64_t m_btac_miss_tkn_miss; // 予測ミス(BTACミス)
};


class BtbG3M : public Btb {
public:
    BpResult PredictBranchAndStudy (PhysAddr branch_pc, PhysAddr target_pc, bool is_taken);
    void UpdateGhr (bool is_taken);
    void ReserveUpdateGhr (bool is_taken) {}
    void ClearBtb (void);
    void Reset (void);

    void ShowPerfInfo (void) const;
    void ResetPerfInfo (void);

    BtbG3M (uint32_t peid, ContextId tcid);
    ~BtbG3M ();

    // Unused.
    BpResult PredictBranchAndStudyBht (PhysAddr branch_pc, bool is_taken, bool is_forward)
    { return BP_MISS; }
    void StudyRas (PhysAddr return_pc) {}
    BpResult PredictBranchAndPopRas (PhysAddr target_pc) { return BP_MISS; }
    BpResult PredictBranchAndStudyBtac (PhysAddr branch_pc, PhysAddr target_pc) { return BP_MISS; }
    BpResult PredictBranch (void) { return BP_MISS; }
    BpResult PredictBranchAndStudyBtacBht (PhysAddr branch_pc, PhysAddr target_pc,
                                           bool is_taken, bool is_forward, bool is_uncond)
    { return BP_MISS; }

private:
    static const uint32_t BTB_ENTRY_NUM = 4;    //BTB entries number
    static const uint32_t BTB_MISS  = 0x80; //BTB miss identifier
    static const uint32_t GHR_BITS  = 4;    //GHR number of bits
    static const uint32_t GHR_MASK  = (1<<GHR_BITS)-1; //GHR bit mask

    static const uint8_t  WNTKN = 0x00; //Weakly Not-Taken
    static const uint8_t  WTKN  = 0x01; //Weakly Taken
    static const uint8_t  SNTKN = 0x10; //Strongly Not-Taken
    static const uint8_t  STKN  = 0x11; //Strongly Taken

    const uint32_t m_peid;
    const ContextId m_tcid;

    class BtbLine {
    public:
        void Study (PhysAddr branch_pc, PhysAddr target_pc) {
            Invalidate ();
            m_v = true;
            m_branch_pc = branch_pc;
            m_target_pc = target_pc;
        }
        bool IsEntryHit (PhysAddr branch_pc) const {
            return m_v == true && m_branch_pc == branch_pc;
        }
        void Invalidate (void) {
            m_v = false;
            memset (m_pht, WTKN, sizeof (m_pht));
            m_branch_pc = 0;
            m_target_pc = 0;
        }
        void SetPht (uint32_t phtid, uint8_t ph) { m_pht[phtid] = ph; }
        uint8_t GetPht (uint32_t phtid) const { return m_pht[phtid]; }
        BtbLine () { Invalidate(); }
        ~BtbLine () {}

    private:
        bool     m_v;         //V bit
        PhysAddr m_branch_pc; //Branch source address
        PhysAddr m_target_pc; //Branch target address
        uint8_t  m_pht[1<<GHR_BITS]; //Pattern history table (2-bit saturated counter array)
    };

    typedef std::vector<BtbLine*> BtbLineVec;
    BtbLineVec m_btb_lines;

    uint32_t m_ghr;
    uint32_t m_btb_lru;
    uint64_t m_bp_hit_count;
    uint64_t m_bp_miss_count;

    BpResult PredictBranch (uint8_t ph, bool is_taken) const;
    void UpdateBtbPht (uint32_t entry, bool is_taken);
    void UpdateBtbLru (uint32_t entry);
    uint32_t GetBtbEntryFromLru (void) const;
    void CountBpResult (BpResult bp_result) {
        if (bp_result == BP_HIT) {
            ++ m_bp_hit_count;
        } else {
            ++ m_bp_miss_count;
        }
    }

    uint64_t GetBtbAccessCount (void) const { return m_bp_hit_count + m_bp_miss_count; }
    uint64_t GetBtbBpHitCount (void) const { return m_bp_hit_count; }
    uint64_t GetBtbBpMissCount (void) const { return m_bp_miss_count; }

    void ResetBtbCounter (void) {
        m_bp_hit_count  = 0;
        m_bp_miss_count = 0;
    }
};
