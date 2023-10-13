/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include "./config.h"
#include "./forest_common.h"
#include "./fsbus.h"
#include "./module_vc.h"
#include "./forest_utility.h"
#include "./forest_message.h"
#include "./outstand_base.h"
#include "./memory_body.h"
#ifdef CLUSTER
#include "./cl_bridge.h"
#endif // CLUSTER

IsElapsedTime FsBus::RunModule (void)
{
    // Forest階層のStepExecute記述を簡易化したかったので、ここに配置。
    // system_time更新、1スレッドごとには判定不要なブレーク判定(Sigintなど）
    // 重要:ClockWaitを先にすること。割り込みー＞PEが起動（HALT解除）の場合に、
    //      PEのcpu_time=fsbusのtimeに設定され、その後でFsbus->ClockWaitで時間経過。
    //      FSBUSの時間>PEの時間になってしまう。
    ClockWait (m_clk_ratio);

    if (unlikely (m_update_targets.empty () == false) ) {
        // m_run_targetsの追加/削除
        // iterator使用中にm_run_targetsの追加/削除できないので必要。
        std::vector<TgtUpdate>::iterator it_update = m_update_targets.begin ();
        while ( it_update != m_update_targets.end() ) {
            CModuleVc* tgt = it_update->second;
            TgtVcVec::iterator it_run =
                std::find (m_run_targets.begin (), m_run_targets.end (), tgt);
            if (it_run == m_run_targets.end ()) {
                if (it_update->first == ADD_TARGET) {
                    // Add target
                    m_run_targets.push_back (tgt);
                }
            } else {
                if (it_update->first == REM_TARGET) {
                    // Remove target
                    m_run_targets.erase (it_run);
                }
            }
            ++ it_update;
        }
        m_update_targets.clear ();
    }

    TgtVcVec::iterator it = m_run_targets.begin();
    while ( it != m_run_targets.end() ) {
        (*it)->CyclicHandler ();
        ++ it;
    }

    return TIME_ELAPSED;
}


void FsBus::AddCyclicHandler (CModuleVc* tgt_vc)
{
    // 追加予約だけ。
    // iterator使用中に追加すると困るので、実際の追加はRunCyclicHandlerで実施。
    m_update_targets.push_back (TgtUpdate(ADD_TARGET, tgt_vc));
}


void FsBus::RemoveCyclicHandler (CModuleVc* tgt_vc)
{
    // 削除予約だけ。
    // iterator使用中に削除すると困るので、実際の削除はRunCyclicHandlerで実施。
    m_update_targets.push_back (TgtUpdate(REM_TARGET, tgt_vc));
}


//! 周辺モデルを指定アドレスに接続
/*!
  \param pe_location
  \param cl_location
  \param start_addr
  \param end_addr
  \param tgt_vc
  \param membody 周辺がメモリを持つなら指定。持たないならNULL。
  Smem登録前後で登録位置が異なる。
  Cforest::GenPeripherals実行後、fsbusの最終要素に全領域メモリが接続されており、
  AddTargetVcで全領域メモリ以降に周辺を登録しても到達しないので。
  \param guard
  \param latency

  \todo To be documented.
*/
bool FsBus::AddTargetVC (uint32_t pe_location, uint32_t cl_location,
                         PhysAddr start_addr, PhysAddr end_addr, CModuleVc* tgt_vc,
                         MemBody* membody, ModuleGuard* guard, MemLatencyInfoBase* latency)
{
    BusTarget* tgt = new BusTarget;

    // by default, target module is not bridge,
    // bridge is generated through AddClusterTarget
    tgt->m_is_bridge = false;

    tgt->m_btgt_peid = pe_location;
    tgt->m_btgt_clid = cl_location;
    tgt->m_btgt_start_addr = start_addr;
    tgt->m_btgt_end_addr = end_addr;
    tgt->m_btgt_vc = tgt_vc;
    tgt->m_membody = membody;
    tgt->m_btgt_guard = guard;
    tgt->m_latency = latency;

    if (is_registered_smem == false) {
        // smem登録前なので、そのまま最後に追加する
        m_bus_targets.push_back (tgt);
    } else {
        // 最後の要素は全領域smemなので、最後の1個前に登録する
        BusTargetVec::iterator it = m_bus_targets.end();
        -- it;
        m_bus_targets.insert (it, tgt);
    }
    return true;
}


/*!
  \param start_addr
  \param end_addr
  \param tgt_vc
  \param membody 周辺がメモリを持つなら指定。持たないならNULL。
  Smem登録前後で登録位置が異なる。
  Cforest::GenPeripherals実行後、fsbusの最終要素に全領域メモリが接続されており、
  AddTargetVcで全領域メモリ以降に周辺を登録しても到達しないので。
  \param guard
  \param latency

  \todo To be documented.
*/
bool FsBus::AddTargetVC (PhysAddr start_addr, PhysAddr end_addr, CModuleVc* tgt_vc,
                         MemBody* membody, ModuleGuard* guard, MemLatencyInfoBase* latency)
{
    BusTarget* tgt = new BusTarget;
    tgt->m_btgt_peid = MEM_LOCATE_GLOBAL;
    tgt->m_btgt_clid = MEM_LOCATE_GLOBAL;
    tgt->m_btgt_start_addr = start_addr;
    tgt->m_btgt_end_addr = end_addr;
    tgt->m_btgt_vc = tgt_vc;
    tgt->m_membody = membody;
    tgt->m_btgt_guard = guard;
    tgt->m_latency = latency;

    if (is_registered_smem == false) {
        // smem登録前なので、そのまま最後に追加する
        m_bus_targets.push_back (tgt);
    } else {
        // 最後の要素は全領域smemなので、最後の1個前に登録する
        BusTargetVec::iterator it = m_bus_targets.end();
        -- it;
        m_bus_targets.insert (it, tgt);
    }
    return true;
}


#ifdef CLUSTER
//! Add Cluster Latency Model
bool FsBus::AddClusterTarget (char *bridge_name,
                              uint32_t clid,
                              PhysAddr start_addr,
                              PhysAddr end_addr,
                              MemRestrictType restrict_type,
                              ModuleGuard *guard,
                              MemLatencyInfoBase* latency)
{
    BusTarget* tgt = new BusTarget;

    // Target module is Bridge,
    // bridge is generated through AddClusterTarget (in this function)
    tgt->m_is_bridge = true;

    tgt->m_bus_name        = bridge_name;
    tgt->m_btgt_cl_peid    = clid << CL_PEID_WIDTH;
    tgt->m_latency         = latency;
    tgt->m_btgt_guard      = guard;
    tgt->m_btgt_start_addr = start_addr;
    tgt->m_btgt_end_addr   = end_addr;
    tgt->m_restrict_type   = restrict_type;

    m_cluster_targets.push_back (tgt);

    return true;
}
#endif // CLUSTER


bool FsBus::RemTargetVC (PhysAddr start_addr, CModuleVc* tgt_vc, MemBody* membody)
{
    BusTargetVec::iterator it = m_bus_targets.begin();
    while ( it != m_bus_targets.end() ) {
        if (   (start_addr == (*it)->m_btgt_start_addr)
            && (tgt_vc     == (*it)->m_btgt_vc)
            && (membody    == (*it)->m_membody) ) {
            // レイテンシ情報を削除
            MemLatencyInfoBase* latency = (*it)->m_latency;
            // NULLに設定しないと、IsDeletableLatencyInfoで自分自身が引っかかる。
            (*it)->m_latency = NULL;
            if (IsDeletableLatencyInfo (latency)) {
                delete latency;
            }

            delete (*it);
            m_bus_targets.erase (it);
            return true;
        }
        ++ it;
    }
    return false;
}


bool FsBus::SetSelfArea (PhysAddr start_addr, PhysAddr end_addr, PhysOffset offset)
{
    TgtBlock* block = SearchSelfBlock (start_addr);
    if (block == NULL) {
        // 設定なし
        block = new TgtBlock;
        m_self_blocks.push_back (block);
        block->m_start_addr = start_addr;
        block->m_end_addr   = end_addr;
        block->m_offset     = offset;
        return true;
    } else {
        return false;
    }
}

bool FsBus::ClearSelfArea (PhysAddr addr)
{
    TgtBlockVec::iterator it = m_self_blocks.begin();
    while ( it != m_self_blocks.end() ) {
        // 含むエントリを消す
        if (addr >= (*it)->m_start_addr &&
            addr <= (*it)->m_end_addr) {
            delete (*it);
            it = m_self_blocks.erase (it);
            return true;
        }
        ++ it;
    }

    // 該当エントリなし
    return false;
}


bool FsBus::SetMirrorArea (PhysAddr start_addr, PhysAddr end_addr, PhysOffset offset)
{
    TgtBlock* block = SearchMirrorBlock (start_addr);
    if (block == NULL) {
        // 設定なし
        block = new TgtBlock;
        m_mirr_blocks.push_back (block);
        block->m_start_addr = start_addr;
        block->m_end_addr   = end_addr;
        block->m_offset     = offset;
        return true;
    } else {
        return false;
    }
}


bool FsBus::ClearMirrorArea (PhysAddr addr)
{
    TgtBlockVec::iterator it = m_mirr_blocks.begin();
    while ( it != m_mirr_blocks.end() ) {
        // 含むエントリを消す
        if (addr >= (*it)->m_start_addr &&
            addr <= (*it)->m_end_addr) {
            delete (*it);
            it = m_mirr_blocks.erase (it);
            return true;
        }
        ++ it;
    }

    // 該当エントリなし
    return false;
}


bool FsBus::SetAccessDeniedArea (PhysAddr start_addr, PhysAddr end_addr)
{
    TgtBlockDenied* block = SearchAccessDeniedBlock (start_addr);
    if (block == NULL) {
        // 設定なし
        block = new TgtBlockDenied;
        m_deny_blocks.push_back (block);
    }
    block->m_start_addr = start_addr;
    block->m_end_addr   = end_addr;

    return true;
}


bool FsBus::ClearAccessDeniedArea (PhysAddr addr)
{
    TgtBlockDeniedVec::iterator it = m_deny_blocks.begin();
    while ( it != m_deny_blocks.end() ) {
        // 含むエントリを消す
        if (addr >= (*it)->m_start_addr &&
            addr <= (*it)->m_end_addr) {
            delete (*it);
            it = m_deny_blocks.erase (it);
            return true;
        }
        ++ it;
    }

    // 該当エントリなし
    return false;
}


FsBus::TgtBlock* FsBus::SearchSelfBlock (PhysAddr phys_addr) const
{
    TgtBlockVec::const_iterator it = m_self_blocks.begin();
    while ( it != m_self_blocks.end() ) {
        if (phys_addr >= (*it)->m_start_addr &&
            phys_addr <= (*it)->m_end_addr ) {
            return *it;
        }
        ++ it;
    }

    return NULL;
}

FsBus::TgtBlock* FsBus::SearchMirrorBlock (PhysAddr phys_addr) const
{
    TgtBlockVec::const_iterator it = m_mirr_blocks.begin();
    while ( it != m_mirr_blocks.end() ) {
        if (phys_addr >= (*it)->m_start_addr &&
            phys_addr <= (*it)->m_end_addr ) {
            return *it;
        }
        ++ it;
    }

    return NULL;
}


FsBus::TgtBlockDenied* FsBus::SearchAccessDeniedBlock (PhysAddr phys_addr) const
{
    TgtBlockDeniedVec::const_iterator it = m_deny_blocks.begin();
    while ( it != m_deny_blocks.end() ) {
        if (phys_addr >= (*it)->m_start_addr &&
            phys_addr <= (*it)->m_end_addr ) {
            return *it;
        }
        ++ it;
    }

    return NULL;
}


bool FsBus::IsAccessDenied (PhysAddr phys_addr) const
{
    TgtBlockDeniedVec::const_iterator it = m_deny_blocks.begin();
    while ( it != m_deny_blocks.end() ) {
        if (phys_addr >= (*it)->m_start_addr &&
            phys_addr <= (*it)->m_end_addr ) {
            return true;
        }
        ++ it;
    }

    return false;
}


FsBus::BusTarget* FsBus::SearchAccessTarget (PhysAddr phys_addr)
{
    BusTargetVec::const_iterator it = m_bus_targets.begin();
    while ( it != m_bus_targets.end() ) {
        if (phys_addr >= (*it)->m_btgt_start_addr &&
            phys_addr <= (*it)->m_btgt_end_addr ) {
            return *it;
        }
        ++ it;
    }

    // アクセス対象が見つからなかった
    return m_vc_dummy;
}


#ifdef CLUSTER
FsBus::BusTarget* FsBus::SearchAccessTarget (PhysAddr phys_addr, uint32_t peid, MemAccessCommand cmd)
{
    int32_t clid_source = ForestUtil::ExtractClId (peid);

    BusTarget *target = NULL;
    BusTargetVec::const_iterator it = m_bus_targets.begin();
    int32_t clid_target = 0;
    while ( it != m_bus_targets.end() ) {
        if (phys_addr >= (*it)->m_btgt_start_addr &&
            phys_addr <= (*it)->m_btgt_end_addr ) {

            clid_target = ForestUtil::ExtractClId ((*it)->m_btgt_cl_peid);
            target = (*it);
            break;
        }
        ++ it;
    }

    if (target == NULL) {
        return m_vc_dummy;
    } else if (clid_source == clid_target) {
        // if source and destination are inside of same Cluster
        return target;
    }

    // if source and destination are in other clusters
    BusTarget *cl_target = NULL;
    BusTargetVec::const_iterator cl_it = m_cluster_targets.begin();
    while ( cl_it != m_cluster_targets.end() ) {
        int32_t clid = ForestUtil::ExtractClId ((*cl_it)->m_btgt_cl_peid);
        if (clid == clid_target &&
            (*cl_it)->m_btgt_start_addr <= phys_addr &&
            (*cl_it)->m_btgt_end_addr >= phys_addr) {
            // Copy original Target Information into Cluster Target
            if (IsValidAccess (cmd, (*cl_it)->m_restrict_type) == true) {
#ifdef CLUSTER_DEBUG
                CfMsg::DPrint (MSG_INF, "Hit[%s] (%08x) : ClID=%d, Start=%08x, End=%08x\n",
                               (*cl_it)->m_bus_name.c_str(),
                               phys_addr,
                               clid,
                               (*cl_it)->m_btgt_start_addr,
                               (*cl_it)->m_btgt_end_addr);
#endif // CLUSTER_DEBUG
                cl_target = (*cl_it);

                // copy target(via cluster)'s PEID for trace outputs
                cl_target->m_btgt_mem_peid   = target->m_btgt_cl_peid;

                cl_target->m_btgt_start_addr = target->m_btgt_start_addr;
                cl_target->m_btgt_end_addr   = target->m_btgt_end_addr;
                cl_target->m_btgt_vc         = target->m_btgt_vc;
                cl_target->m_membody         = target->m_membody;
                cl_target->m_btgt_guard      = target->m_btgt_guard;
                MemLatencyInfoExCluster *lat_info_cluster = static_cast<MemLatencyInfoExCluster *>(cl_target->m_latency);
                lat_info_cluster->SetExternalMemInfoBase (target->m_latency);
                break;
            }
        }
        ++ cl_it;
    }

    if (cl_target != NULL) {
        return cl_target;
    } else if (target != NULL) {
        return target;
    } else {
        // アクセス対象が見つからなかった
        return m_vc_dummy;
    }
}


bool FsBus::IsValidAccess (MemAccessCommand cmd, MemRestrictType restrict)
{
    switch (cmd) {
    case INST_READ  : return ((restrict & MRT_FETCH_ENABLE) != 0) ? true : false;
    case DATA_READ  : return ((restrict & MRT_READ_ENABLE ) != 0) ? true : false;
    case DATA_WRITE : return ((restrict & MRT_WRITE_ENABLE) != 0) ? true : false;
    case DATA_RMW   : return (((restrict & MRT_READ_ENABLE) != 0) && ((restrict & MRT_WRITE_ENABLE) != 0)) ? true : false;
    default         : return false;
    }
}

void FsBus::ShowAccessRoute (SideBand sideband,
                      MemAccessCommand cmd,
                      PhysAddr addr,
                      BusTarget* tgt)
{
    std::string cmd_str;
    switch (cmd) {
    case INST_READ  : cmd_str = "(IF)"; break;
    case DATA_READ  : cmd_str = "(DR)"; break;
    case DATA_WRITE : cmd_str = "(DW)"; break;
    case DATA_RMW   : cmd_str = "(RW)"; break;
    }
    std::stringstream route_str;
    route_str << "<BusAccess " << cmd_str.c_str() << " P" << sideband.GetPEID () << "-->";
    uint32_t ram_peid, ram_clid;
    CfMsg::DPrint (MSG_INF, "%s", route_str.str().c_str());
    if (tgt->m_is_bridge == true) {
        CfMsg::DPrint (MSG_INF, "cluster(%s)-->", tgt->m_bus_name.c_str());
        ram_peid = ForestUtil::ExtractPeId (tgt->m_btgt_mem_peid);
        ram_clid = ForestUtil::ExtractClId (tgt->m_btgt_mem_peid);
    } else {
        CfMsg::DPrint (MSG_INF, "cluster()-->");
        ram_peid = ForestUtil::ExtractPeId (tgt->m_btgt_cl_peid);
        ram_clid = ForestUtil::ExtractClId (tgt->m_btgt_cl_peid);
    }
    if (ram_peid == MEM_LOCATE_GLOBAL) {
        CfMsg::DPrint (MSG_INF, "C%dRAM(%08x)>\n", ram_clid ,addr);
    } else {
        CfMsg::DPrint (MSG_INF, "P%dRAM(%08x)>\n", ram_peid, addr);
    }
}

#endif // CLUSTER


template <MemAccessMode acc_mode, MemAccessCommand cmd>
SysErrCause FsBus::BusAccessInternal (SideBand sideband, PhysAddr addr, uint8_t* data,
                                      ByteSize size, BusTarget* tgt)
{
#ifdef CLUSTER
    if (m_is_bus_trace == true &&
        sideband.GetPEID () != 0 &&   // PEID=0 means debug access and srec load in initialize of CForest
        tgt->m_membody != NULL) {
        //ShowAccessRoute (sideband, cmd, addr, tgt);
    }
#endif // CLUSTER
    if (acc_mode == MACC_NORMAL) {
        SysErrCause sys_err_cause = tgt->m_btgt_guard->IsAccPermited (sideband, addr, size, cmd);
        if (sys_err_cause != SYSERR_NONE) { return sys_err_cause; }
        if (cmd == DATA_WRITE) {
            return tgt->m_btgt_vc->TargetWrite (sideband, addr, data, size);
        } else if (cmd == DATA_READ) {
            return tgt->m_btgt_vc->TargetRead  (sideband, addr, data, size);
        } else if (cmd == INST_READ) {
            return tgt->m_btgt_vc->TargetFetch (sideband, addr, data, size);
        }
    } else {
        if (cmd == DATA_WRITE) {
            tgt->m_btgt_vc->TargetWriteDebug (addr, data, size);
        } else if (cmd == DATA_READ) {
            tgt->m_btgt_vc->TargetReadDebug  (addr, data, size);
        } else if (cmd == INST_READ) {
            // CubeSuite用。非メモリのIOR領域に対するフェッチはエラーになるので区別。
            if (tgt->m_membody != NULL) {
                // 通常メモリ、非サポートのLOCALIOR領域に対するフェッチは、
                // MemRestrictTypeに従いアクセス
                tgt->m_membody->MemBodyFetchDebug (addr, data, size);
            } else {
                // 非メモリのIOR領域、サポート済み周辺領域へのフェッチはFETCH_PROTECT_ERROR
                // ただし、デバッグアクセスではエラーを起こさず、値だけエラー相当に変更
                // 実際にCPUがフェッチした際にエラーになる
                memset (data, p_forest_util->GetMemInit (), size);
            }
        }
        // デバッグアクセスではエラーを起こさない
        return SYSERR_NONE;
    }
}


template <MemAccessMode acc_mode, MemAccessCommand cmd>
SysErrCause FsBus::BusAccessPhys (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    BusTarget* tgt;

    // 最初にアクセス不許可領域を除外
    if (unlikely (IsAccessDenied (addr) == true)) {
        tgt = m_vc_dummy;
    } else {
        // 物理アドレスでもmirror領域を考慮する
        addr = TranslateMirrorAddr (sideband, addr);
#ifdef CLUSTER
        // From functional simulation side, sideband only contains small bitwidth of PEID.
        // Cluster-ID is extracted, so making special cl_peid would be created
        uint32_t cl_peid = ForestUtil::MakeClPeId (sideband.GetCLID(), sideband.GetPEID ());
        tgt = SearchAccessTarget (addr, cl_peid, cmd);
#else // CLUSTER
        tgt = SearchAccessTarget (addr);
#endif // CLUSTER
    }
    return BusAccessInternal <acc_mode, cmd> (sideband, addr, data, size, tgt);
}


//! \sa BusAccessPhys
template <MemAccessMode acc_mode, MemAccessCommand cmd>
SysErrCause FsBus::BusAccess (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    BusTarget* tgt;

    // 最初にアクセス不許可領域を除外
    if (unlikely (IsAccessDenied (addr) == true)) {
        tgt = m_vc_dummy;
    } else {
        // Self/Mirror領域のアドレスを、実体のアドレスに変換
        addr = GetBusAddress (sideband, addr);
#ifdef CLUSTER
        // From functional simulation side, sideband only contains small bitwidth of PEID.
        // Cluster-ID is extracted, so making special cl_peid would be created
        uint32_t cl_peid = ForestUtil::MakeClPeId (sideband.GetCLID(), sideband.GetPEID ());
        tgt = SearchAccessTarget (addr, cl_peid, cmd);
#else // CLUSTER
        tgt = SearchAccessTarget (addr);
#endif // CLUSTER
    }
    return BusAccessInternal <acc_mode, cmd> (sideband, addr, data, size, tgt);
}


PhysAddr FsBus::GetBusAddress (SideBand sideband, PhysAddr addr) const
{
    // @@ IsAccessDenied()未対応。SC-HEAPで必要になったら対応する。

    // Self領域のアドレスを、実体/Mirrorのアドレスに変換
    addr = TranslateSelfAddr (sideband, addr);

    // Mirror領域のアドレスを、実体のアドレスに変換
    addr = TranslateMirrorAddr (sideband, addr);

    return addr;
}


PhysAddr FsBus::TranslateSelfAddr (SideBand sideband, PhysAddr addr) const
{
    // Self->実体/Mirror変換
    TgtBlock* tgtblock = SearchSelfBlock (addr);
    if (tgtblock != NULL) {
        uint32_t peid = p_internals->GetPeidFromBmid (sideband.GetBMID ());
        addr += tgtblock->m_offset * (peid + 1U);
    }
    return addr;
}



PhysAddr FsBus::TranslateMirrorAddr (SideBand sideband, PhysAddr addr) const
{
    // Mirror->実体変換
    TgtBlock* tgtblock = SearchMirrorBlock (addr);
    if (tgtblock != NULL) {
        addr += tgtblock->m_offset;
    }
    return addr;
}


void FsBus::UpdateExoPeLatency (uint32_t exo_latency_offset)
{
    // smemの場合だけ。
    // peripheral、またはSC-HEAP/CubeSuite+接続時は直接exopeレイテンシを指定するので更新しない。
    BusTargetVec::const_iterator it = m_bus_targets.begin();
    while (it != m_bus_targets.end()) {
        if ( ((*it)->m_membody != NULL) && ((*it)->m_latency != NULL) ) {
            (*it)->m_latency->UpdateExoPeLatency (exo_latency_offset);
        }
        ++ it;
    }
}


bool FsBus::IsLatencyInfoUsedOther (MemLatencyInfoBase* mem_latency)
{
    BusTargetVec::const_iterator it = m_bus_targets.begin();
    while (it != m_bus_targets.end()) {
        if ((*it)->m_latency == mem_latency) {
            return true;
        }
        ++ it;
    }
    return false;
}


bool FsBus::IsDeletableLatencyInfo (MemLatencyInfoBase* mem_latency)
{
    if (mem_latency == NULL) {
        // 共通レイテンシなら削除しない
        return false;
    }

    if (mem_latency->IsScheap ()) {
        // SC-HEAPなら削除しない
        return false;
    }

    if (IsLatencyInfoUsedOther (mem_latency) == true) {
        // 他で使われていたら削除しない
        return false;
    }

    return true;
}


void FsBus::ResetLatencyInfo (void)
{
    BusTargetVec::const_iterator it = m_bus_targets.begin();
    while (it != m_bus_targets.end()) {
        MemLatencyInfoBase* latency = (*it)->m_latency;
        if (latency != NULL) {
            latency->InitMemCycle ();
        }
        ++ it;
    }
    m_vc_dummy->m_latency->InitMemCycle ();
}


FsBus::FsBus ()
{
    // SearchAccessTargetの戻り値のNULL判定をなくすため、ダミーターゲットを使用
    m_vc_dummy = new BusTarget;
    m_vc_dummy->m_btgt_vc = new VcDummy;
    m_vc_dummy->m_membody = NULL;
    m_vc_dummy->m_btgt_start_addr = 0;
    m_vc_dummy->m_btgt_end_addr = 0;
    m_vc_dummy->m_btgt_guard = new ModuleGuardDummy ();
    m_vc_dummy->m_latency = p_internals->CreateDefaultLatency (MEM_LOCATE_GLOBAL, MEM_LOCATE_GLOBAL);

    is_registered_smem = false;

    m_clk_cycle = 0;
    m_clk_ratio = 1;
}


FsBus::~FsBus ()
{
    BusTargetVec::iterator it_bus = m_bus_targets.begin();
    while (it_bus != m_bus_targets.end()) {
        MemLatencyInfoBase* latency = (*it_bus)->m_latency;
        // NULLに設定しないと、IsDeletableLatencyInfoで自分自身が引っかかる。
        (*it_bus)->m_latency = NULL;
        if (IsDeletableLatencyInfo (latency)) {
            delete latency;
        }
        delete *it_bus;
        it_bus = m_bus_targets.erase (it_bus);
    }

    it_bus = m_cluster_targets.begin ();
    while (it_bus != m_cluster_targets.end()) {
        MemLatencyInfoBase* latency = (*it_bus)->m_latency;
        // NULLに設定しないと、IsDeletableLatencyInfoで自分自身が引っかかる。
        (*it_bus)->m_latency = NULL;
        if (IsDeletableLatencyInfo (latency)) {
            delete latency;
        }
        delete *it_bus;
        it_bus = m_cluster_targets.erase (it_bus);
    }
    for (TgtBlockVec::iterator it = m_self_blocks.begin(); it != m_self_blocks.end(); ++it) {
        delete *it;
    }
    for (TgtBlockVec::iterator it = m_mirr_blocks.begin(); it != m_mirr_blocks.end(); ++it) {
        delete *it;
    }
    for (TgtBlockDeniedVec::iterator it = m_deny_blocks.begin(); it != m_deny_blocks.end(); ++it) {
        delete *it;
    }

    delete m_vc_dummy->m_btgt_vc;
    delete m_vc_dummy->m_btgt_guard;
    delete m_vc_dummy->m_latency;
    delete m_vc_dummy;
}


SysErrCause VcDummy::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
#ifdef SUPPORT_CUBESUITE
    p_forest_util->SetCubeErrorCode (E_MEMORY_NOT_MAPPED);
#else // SUPPORT_CUBESUITE
    CfMsg::DPrint (MSG_ERR, "<Error: illegal memory access : Addr=%08" PRIx32 ">\n", addr);
#endif // SUPPORT_CUBESUITE
    return SYSERR_NONE;
}


SysErrCause VcDummy::TargetReadDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
#ifdef SUPPORT_CUBESUITE
    p_forest_util->SetCubeErrorCode (E_MEMORY_NOT_MAPPED);
#else // SUPPORT_CUBESUITE
    CfMsg::DPrint (MSG_ERR, "<Error: illegal memory access : Addr=%08" PRIx32 ">\n", addr);
#endif // SUPPORT_CUBESUITE
    memset (data, p_forest_util->GetMemInit (), size);
    return SYSERR_NONE;
}


SysErrCause VcDummy::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
#ifdef SUPPORT_CUBESUITE
    p_internals->CauseError (sideband, E_MEMORY_NOT_MAPPED);
#endif // SUPPORT_CUBESUITE
    return TargetWriteDebug (addr, data, size);
}


SysErrCause VcDummy::TargetRead (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
#ifdef SUPPORT_CUBESUITE
    p_internals->CauseError (sideband, E_MEMORY_NOT_MAPPED);
#endif // SUPPORT_CUBESUITE
    return TargetReadDebug (addr, data, size);
}


void FsBus::SetCpuTime (uint64_t cycle)
{ m_clk_cycle = cycle; }

void FsBus::SetCpuRatio (uint32_t ratio)
{ m_clk_ratio = ratio; }

void FsBus::ClockWait (uint64_t wait_cycle)
{ m_clk_cycle += wait_cycle; }

void FsBus::PreConsumeClock (void)
{ ClockWait (1); }

void FsBus::AddHaltCycle (uint64_t halt_cycle)
{}

void FsBus::UpdateSnoozeState (void)
{}

bool FsBus::IsBreakHitPreExec (void) const
{ return false; }

bool FsBus::IsBreakHitPostExec (void) const
{ return p_internals->IsHitBreak (); }

bool FsBus::IsHtRunable (void) const
{ return true; }

bool FsBus::IsSnooze (void) const
{ return false; }

bool FsBus::IsHtActive (void) const
{ return true; }

bool FsBus::IsIntRequested (void) const
{ return false; }

bool FsBus::IsInHalt (void) const
{ return false; }

#ifdef SUPPORT_SC_HEAP
bool FsBus::IsSuspendState (void) const
{ return false; }
#endif // SUPPORT_SC_HEAP

bool FsBus::IsCpu (void) const
{ return false; }

FILE* FsBus::GetFilePointer (void) const
{ return m_filep; }

int32_t FsBus::GetScId (void) const
{ return FSBUS_SCID; }

uint64_t FsBus::GetCpuTime (void) const
{ return m_clk_cycle; }

uint32_t FsBus::GetCpuRatio (void) const
{ return m_clk_ratio; }

#ifdef CLUSTER
void FsBus::ShowPerfInfo (void) const
{
    BusTargetVec::const_iterator it = m_cluster_targets.begin();
    while (it != m_cluster_targets.end()) {
        MemLatencyInfoExCluster *cl_info = static_cast<MemLatencyInfoExCluster *>((*it)->m_latency);
        cl_info->ShowPerfInfo ();
        it++;
    }
}

void FsBus::ResetPerfInfo (void)
{
    BusTargetVec::const_iterator it = m_cluster_targets.begin();
    while (it != m_cluster_targets.end()) {
        MemLatencyInfoExCluster *cl_info = static_cast<MemLatencyInfoExCluster *>((*it)->m_latency);
        cl_info->ResetPerfInfo ();
        it++;
    }
}
#endif // CLUSTER


// template explicit instanciation
template SysErrCause FsBus::BusAccess<MACC_NORMAL, DATA_WRITE> (
        SideBand, PhysAddr, uint8_t*, ByteSize);
template SysErrCause FsBus::BusAccess<MACC_NORMAL, DATA_READ>  (
        SideBand, PhysAddr, uint8_t*, ByteSize);
template SysErrCause FsBus::BusAccess<MACC_NORMAL, INST_READ>  (
        SideBand, PhysAddr, uint8_t*, ByteSize);
template SysErrCause FsBus::BusAccess<MACC_DEBUG,  DATA_WRITE> (
        SideBand, PhysAddr, uint8_t*, ByteSize);
template SysErrCause FsBus::BusAccess<MACC_DEBUG,  DATA_READ>  (
        SideBand, PhysAddr, uint8_t*, ByteSize);
template SysErrCause FsBus::BusAccess<MACC_DEBUG,  INST_READ>  (
        SideBand, PhysAddr, uint8_t*, ByteSize);
template SysErrCause FsBus::BusAccessPhys<MACC_NORMAL, DATA_WRITE> (
        SideBand, PhysAddr, uint8_t*, ByteSize);
template SysErrCause FsBus::BusAccessPhys<MACC_NORMAL, DATA_READ>  (
        SideBand, PhysAddr, uint8_t*, ByteSize);
template SysErrCause FsBus::BusAccessPhys<MACC_NORMAL, INST_READ>  (
        SideBand, PhysAddr, uint8_t*, ByteSize);
template SysErrCause FsBus::BusAccessPhys<MACC_DEBUG, DATA_WRITE> (
        SideBand, PhysAddr, uint8_t*, ByteSize);
template SysErrCause FsBus::BusAccessPhys<MACC_DEBUG, DATA_READ>  (
        SideBand, PhysAddr, uint8_t*, ByteSize);
template SysErrCause FsBus::BusAccessPhys<MACC_DEBUG, INST_READ>  (
        SideBand, PhysAddr, uint8_t*, ByteSize);
template SysErrCause FsBus::BusAccessInternal<MACC_NORMAL, DATA_WRITE> (
        SideBand, PhysAddr, uint8_t*, ByteSize, BusTarget* tgt);
template SysErrCause FsBus::BusAccessInternal<MACC_NORMAL, DATA_READ>  (
        SideBand, PhysAddr, uint8_t*, ByteSize, BusTarget* tgt);
template SysErrCause FsBus::BusAccessInternal<MACC_NORMAL, INST_READ>  (
        SideBand, PhysAddr, uint8_t*, ByteSize, BusTarget* tgt);
template SysErrCause FsBus::BusAccessInternal<MACC_DEBUG,  DATA_WRITE> (
        SideBand, PhysAddr, uint8_t*, ByteSize, BusTarget* tgt);
template SysErrCause FsBus::BusAccessInternal<MACC_DEBUG,  DATA_READ>  (
        SideBand, PhysAddr, uint8_t*, ByteSize, BusTarget* tgt);
template SysErrCause FsBus::BusAccessInternal<MACC_DEBUG,  INST_READ>  (
        SideBand, PhysAddr, uint8_t*, ByteSize, BusTarget* tgt);
