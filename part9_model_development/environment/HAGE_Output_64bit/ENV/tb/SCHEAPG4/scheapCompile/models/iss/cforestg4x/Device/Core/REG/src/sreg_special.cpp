/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for whichEIPSW
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <algorithm>
#include <vector>
#include "./config.h"
#include "./forest_common.h"
#include "./gregfile.h"
#include "./sregfile.h"
#include "./mpu.h"
#include "./lsu.h"
#include "./proc_element.h"
#include "./fs_controler.h"
#include "./icache.h"
#include "./dbg_break.h"
#include "./forest_message.h"
#include "./inst_id_list.h"
#include "./proc_element.h"
#include "./sreg.h"
#include "./forest_utility.h"
#include "./counter_base.h"
#ifdef SUPPORT_CUBESUITE
#include "./cube_callback.h"
#endif // SUPPORT_CUBESUITE


void SRegHMPSW::SetUM (bool um)
{
    if (um == m_um) return;  //! Do nothing when no updating for simulation speed.

    m_um = um;
    m_pe->UpdatePrivLevel ();

    if ((m_pe->IsHvEnabled () == false) || (m_pe->IsHostMode () == true)) {
        m_pe->ClearBtb ();
        m_pe->UpdateSideband ();
    }
}


void SRegHMPSW::SetEIMASK (RegData eimask)
{
    //All of bit fields related to the interruption priority are changed to 6bit from 8bit.
    // Upper 2bits are fixed to 0.
    eimask &= 0x3FU;

    if (m_eimask == eimask) return; //! Do nothing when no updating for simulation speed.

    m_eimask = eimask;
    m_pe->GetHtPtr()->UpdateIntStatus ();
    m_pe->GetHtPtr()->SetPmcUpdateFlag();
}


RegData SRegHMPSW::GetEIMASK (void) const
{
    if ((m_pe->GetCoreType() == CORE_G4MH) && (m_pe->GetCoreVersion() >= CORE_VER_20)) {
        if (m_pe->GetNcSrfPtr()->SrHMINTCFG()->GetEPL () == 1) {
            return m_eimask;
        }
    }
    return 0U;
}


void SRegHMPSW::SetCU (RegData cu)
{
    // CU2：実装依存 (0固定)
    cu &= ~0x4;
    if (m_pe->GetCoreType() == CORE_G4KH) {
        // CU1：Not support FXU in G4KH
        cu &= ~0x2;
    } else {
        if (! m_pe->HasFxu()) { cu &= ~0x2; }
    }
    if (! m_pe->HasFpu())  { cu &= ~0x1; }
    m_cu = cu;   // 非搭載のコプロは0固定
}


void SRegHMPSW::SetNP (bool np)
{
    if (np == m_np) return;  //! Do nothing when no updating for simulation speed.

    m_np = np;

    // 割り込み受付許可状態を通知
    m_pe->GetHtPtr()->UpdateIntAcceptable ();
    m_pe->GetHtPtr()->UpdateBgIntAcceptable ();
    m_pe->GetHtPtr()->UpdateFeintAcceptable ();
    m_pe->GetHtPtr()->UpdateBgFeintAcceptable ();
    m_pe->GetHtPtr()->UpdateOperandSyserrAcceptable ();
}


void SRegHMPSW::SetID (bool id)
{
    if (id == m_id) return;  //! Do nothing when no updating for simulation speed.

    m_id = id;

    // 割り込み受付許可状態を通知
    m_pe->GetHtPtr()->UpdateIntAcceptable ();
    m_pe->GetHtPtr()->UpdateBgIntAcceptable ();
    m_pe->GetHtPtr()->UpdateBgFeintAcceptable ();

    m_pe->GetHtPtr()->SetPmcUpdateFlag();
}


void SRegHMPSW::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged (priv) == false) return;

    //! ユーザモードで書込可能
    //! PSW.SAT,CY,OV,S,Z
    SetSAT ((data >>  4) & RegData (0x00000001U));
    SetCY  ((data >>  3) & RegData (0x00000001U));
    SetOV  ((data >>  2) & RegData (0x00000001U));
    SetS   ((data >>  1) & RegData (0x00000001U));
    SetZ   ((data >>  0) & RegData (0x00000001U));

    if (HasPrivileged (PRIV_PERMIT_SV, priv)) {
        //! SV権限時のみ書込可能
        //! PSW.UM,EIMASK,CU,EBV,NP,EP,ID

        if ((m_pe->GetCoreType() == CORE_G4MH) && (m_pe->GetCoreVersion() >= CORE_VER_20)) {
            // On G4MH2.0, can't update PSW.UM when DIR0.DM=1.
            if (m_pe->GetHtPtr()->m_srf->SrDIR0()->GetDM () == 0) {
                SetUM  ((data >> 30) & RegData (0x00000001U));
            }
            if (m_pe->GetNcSrfPtr()->SrHMINTCFG()->GetEPL () == 1) {
                SetEIMASK ((data >>  20) & RegData (0x000000FFU));
            }
        } else {
            // On G4MH1.x, can update PSW.UM even if DIR0.DM=1.
            SetUM  ((data >> 30) & RegData (0x00000001U));
        }
        SetCU  ((data >> 16) & RegData (0x00000007U));
        SetEBV ((data >> 15) & RegData (0x00000001U));
        SetNP  ((data >>  7) & RegData (0x00000001U));
        SetEP  ((data >>  6) & RegData (0x00000001U));
        SetID  ((data >>  5) & RegData (0x00000001U));

        //! PSW.VMの更新は以下のケースで個別に実施
        //! 4. NC && PSW.VM=0時、復帰命令によって退避PSWの値を複写
        //! 5. ネイティブ遷移を伴う例外が発生した場合に 0クリア
    }
}


RegData SRegHMPSW::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;

    RegData ret = 0x0
        | (GetUM  () << 30)
        | (GetCU  () << 16)
        | (GetEBV () << 15)
        | (GetNP  () <<  7)
        | (GetEP  () <<  6)
        | (GetID  () <<  5)
        | (GetSAT () <<  4)
        | (GetCY  () <<  3)
        | (GetOV  () <<  2)
        | (GetS   () <<  1)
        | (GetZ   () <<  0);

        if ((m_pe->GetCoreType() == CORE_G4MH) && (m_pe->GetCoreVersion() >= CORE_VER_20)) {
            if (m_pe->GetNcSrfPtr()->SrHMINTCFG()->GetEPL () == 1) {
                ret |= (GetEIMASK () << 20);
            }
        }


    return ret;
}


void SRegPSWH::SetGM (bool gm)
{
    if (m_pe->IsHvEnabled () == false) {
        gm = 0;
    }
    m_gm = gm;
    if (m_pe->IsHvEnabled ()) {
        //m_pe->GetHtPtr()->SetPmcUpdateFlag(); // called in ChangeToXXXMode
        if (gm == 0) {
            m_pe->ChangeToHostMode ();
        } else {
            m_pe->ChangeToGuestMode ();
        }
    } else {
        m_pe->UpdateSideband ();
    }
    m_pe->NoticeCpuGuestModeToIntc (gm);
}


void SRegPSWH::SetGPID (RegData gpid)
{
    if (m_pe->IsHvEnabled () == false) {
        gpid = 0;
    }
    m_gpid = gpid;
    m_pe->GetHtPtr()->SetPmcUpdateFlag();
    m_pe->UpdateSideband ();
    m_pe->NoticeCpuGpidToIntc (gpid);
}


void SRegPSWH::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Write needs HV priv.
            return;
        }
    }
}


void SRegPSWH::Reset (void)
{
    SetGPID (0x0U);
    SetGM (0x0U);
}


void SRegGMPSW::SetUM (bool um)
{
    if (um == m_um) return;  //! Do nothing when no updating for simulation speed.

    m_um = um;
    if ((m_pe->IsHvEnabled () == true) && (m_pe->IsHostMode () == false)) {
        m_pe->UpdatePrivLevel ();
        m_pe->ClearBtb ();
        m_pe->UpdateSideband ();
    }
}


void SRegGMPSW::SetEIMASK (RegData eimask)
{
    //All of bit fields related to the interruption priority are changed to 6bit from 8bit.
    // Upper 2bits are fixed to 0.
    eimask &= 0x3FU;

    if (m_eimask == eimask) return; //! Do nothing when no updating for simulation speed.

    m_eimask = eimask;
    m_pe->GetHtPtr()->UpdateIntStatus ();
}


void SRegGMPSW::SetEBV (bool ebv)
{
    // Fixed to 1 in Guest mode
}


void SRegGMPSW::SetCU (RegData cu)
{
    bool enable_cu2 = false; // CU2 is fixed to 0
    bool enable_cu1 = m_pe->HasFxu();
    bool enable_cu0 = m_pe->HasFpu();

    if ((m_pe->GetCoreType() == CORE_G4MH) && (m_pe->GetCoreVersion() >= CORE_VER_20)) {
        if (m_pe->GetNcSrfPtr()->SrGMCFG()->GetGCU2() == 0) {
            enable_cu2 = false;
        }
        if (m_pe->GetNcSrfPtr()->SrGMCFG()->GetGCU1() == 0) {
            enable_cu1 = false;
        }
        if (m_pe->GetNcSrfPtr()->SrGMCFG()->GetGCU0() == 0) {
            enable_cu0 = false;
        }
    }

    if (! enable_cu2) { cu &= ~0x4; }
    if (! enable_cu1) { cu &= ~0x2; }
    if (! enable_cu0) { cu &= ~0x1; }
    m_cu = cu;
}


void SRegGMPSW::SetNP (bool np)
{
    if (np == m_np) return;  //! Do nothing when no updating for simulation speed.
    m_np = np;

    // 割り込み受付許可状態を通知
    m_pe->GetHtPtr()->UpdateIntAcceptable ();
    m_pe->GetHtPtr()->UpdateGmIntAcceptable ();
    m_pe->GetHtPtr()->UpdateBgIntAcceptable ();
    m_pe->GetHtPtr()->UpdateGmFeintAcceptable ();
    m_pe->GetHtPtr()->UpdateBgFeintAcceptable ();
    m_pe->GetHtPtr()->UpdateOperandSyserrAcceptable ();
}


void SRegGMPSW::SetID (bool id)
{
    if (id == m_id) return;  //! Do nothing when no updating for simulation speed.
    m_id = id;

    // 割り込み受付許可状態を通知
    m_pe->GetHtPtr()->UpdateIntAcceptable ();
    m_pe->GetHtPtr()->UpdateGmIntAcceptable ();
    m_pe->GetHtPtr()->UpdateBgIntAcceptable ();
    m_pe->GetHtPtr()->SetPmcUpdateFlag();
}


void SRegGMPSW::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled () == false) return;
    if (m_pe->IsHostMode () == true) {
        // In case of Host mode, only HV mode can access GMPSW register.
        // UM on Host mode can't acces (PIE occur).
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            // Host UM mode
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE);
            return;
        }
    } else {
        // In guest mode, GMPSW can access via PSW register.
        // Because GMPSW direct access can't be, all access is via PSW.
        // If GMPSW is accessed directly, SRegDUMMY(HV) is used instead of SRegGMPSW to cause PIE.
        ; // No exception occur on Guest mode here
    }

    //! ユーザモードで書込可能
    //! PSW.SAT,CY,OV,S,Z
    SetSAT ((data >>  4) & RegData (0x00000001U));
    SetCY  ((data >>  3) & RegData (0x00000001U));
    SetOV  ((data >>  2) & RegData (0x00000001U));
    SetS   ((data >>  1) & RegData (0x00000001U));
    SetZ   ((data >>  0) & RegData (0x00000001U));

    if (HasPrivileged (PRIV_PERMIT_SV, priv)) {
        //! SV権限時のみ書込可能
        //! PSW.UM,EIMASK,CU,EBV,NP,EP,ID
        SetUM  ((data >> 30) & RegData (0x00000001U));
        SetEIMASK ((data >>  20) & RegData (0x000000FFU));
        SetCU  ((data >> 16) & RegData (0x00000007U));
        SetNP  ((data >>  7) & RegData (0x00000001U));
        SetEP  ((data >>  6) & RegData (0x00000001U));
        SetID  ((data >>  5) & RegData (0x00000001U));
    }
}


RegData SRegGMPSW::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled () == false) return 0;
    if (m_pe->IsHostMode () == true) {
        // In case of Host mode, only HV mode can access GMPSW register.
        // UM on Host mode can't acces (PIE occur).
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            // Host UM mode
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE);
            return 0;
        }
    } else {
        // In guest mode, GMPSW can access via PSW register.
        // Because GMPSW direct access can't be, all access is via PSW.
        // If GMPSW is accessed directly, SRegDUMMY(HV) is used instead of SRegGMPSW to cause PIE.
        ; // No exception occur on Guest mode here
    }

    RegData ret = 0x0
        | (GetUM  () << 30)
        | (GetEIMASK () << 20)
        | (GetCU  () << 16)
        | (GetEBV () << 15)
        | (GetNP  () <<  7)
        | (GetEP  () <<  6)
        | (GetID  () <<  5)
        | (GetSAT () <<  4)
        | (GetCY  () <<  3)
        | (GetOV  () <<  2)
        | (GetS   () <<  1)
        | (GetZ   () <<  0);
    return ret;
}


void SRegHMEIPSW::SetEIMASK (RegData eimask)
{
    //All of bit fields related to the interruption priority are changed to 6bit from 8bit.
    // Upper 2bits are fixed to 0.
    m_eimask = eimask & 0x3F;
}


void SRegHMEIPSW::SetCU (RegData cu)
{
    // CU0-1 can be written always. (different from PSW CUn).
    cu &= ~0x4; // CU2 is fixed to 0 in G4MH
    if (m_pe->GetCoreType() == CORE_G4KH) {
        // CU1：Not support FXU in G4KH
        cu &= ~0x2;
    }
    m_cu = cu;
}


void SRegHMEIPSW::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged (priv) == false) return;

    SetUM  ((data >> 30) & RegData (0x00000001U));
    if ((m_pe->GetCoreType() == CORE_G4MH) && (m_pe->GetCoreVersion() >= CORE_VER_20)) {
        if (m_pe->GetNcSrfPtr()->SrHMINTCFG()->GetEPL () == 1) {
            SetEIMASK ((data >>  20) & RegData (0x000000FFU));
        }
    }
    SetCU  ((data >> 16) & RegData (0x00000007U));
    SetEBV ((data >> 15) & RegData (0x00000001U));
    SetNP  ((data >>  7) & RegData (0x00000001U));
    SetEP  ((data >>  6) & RegData (0x00000001U));
    SetID  ((data >>  5) & RegData (0x00000001U));
    SetSAT ((data >>  4) & RegData (0x00000001U));
    SetCY  ((data >>  3) & RegData (0x00000001U));
    SetOV  ((data >>  2) & RegData (0x00000001U));
    SetS   ((data >>  1) & RegData (0x00000001U));
    SetZ   ((data >>  0) & RegData (0x00000001U));
}


RegData SRegHMEIPSW::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;

    RegData ret = 0x0
        | (GetUM  () << 30)
        | (GetEIMASK () << 20)
        | (GetCU  () << 16)
        | (GetEBV () << 15)
        | (GetNP  () <<  7)
        | (GetEP  () <<  6)
        | (GetID  () <<  5)
        | (GetSAT () <<  4)
        | (GetCY  () <<  3)
        | (GetOV  () <<  2)
        | (GetS   () <<  1)
        | (GetZ   () <<  0);
    return ret;
}


void SRegGMEIPSW::SetEIMASK (RegData eimask)
{
    //All of bit fields related to the interruption priority are changed to 6bit from 8bit.
    // Upper 2bits are fixed to 0.
    m_eimask = eimask & 0x3F;
}


void SRegGMEIPSW::SetCU (RegData cu)
{
    // CU0-1 can be written always. (different from PSW CUn).
    cu &= ~0x4; // CU2 is fixed to 0 in G4MH
    m_cu = cu;
}


void SRegGMEIPSW::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled () == false) return;

    SetUM  ((data >> 30) & RegData (0x00000001U));
    if ((m_pe->GetCoreType() == CORE_G4MH) && (m_pe->GetCoreVersion() >= CORE_VER_20)) {
        SetEIMASK ((data >>  20) & RegData (0x000000FFU));
    }
    SetCU  ((data >> 16) & RegData (0x00000007U));
    SetEBV ((data >> 15) & RegData (0x00000001U));
    SetNP  ((data >>  7) & RegData (0x00000001U));
    SetEP  ((data >>  6) & RegData (0x00000001U));
    SetID  ((data >>  5) & RegData (0x00000001U));
    SetSAT ((data >>  4) & RegData (0x00000001U));
    SetCY  ((data >>  3) & RegData (0x00000001U));
    SetOV  ((data >>  2) & RegData (0x00000001U));
    SetS   ((data >>  1) & RegData (0x00000001U));
    SetZ   ((data >>  0) & RegData (0x00000001U));
}


RegData SRegGMEIPSW::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled () == false) return 0;

    RegData ret = 0x0
        | (GetUM  () << 30)
        | (GetEIMASK () << 20)
        | (GetCU  () << 16)
        | (GetEBV () << 15)
        | (GetNP  () <<  7)
        | (GetEP  () <<  6)
        | (GetID  () <<  5)
        | (GetSAT () <<  4)
        | (GetCY  () <<  3)
        | (GetOV  () <<  2)
        | (GetS   () <<  1)
        | (GetZ   () <<  0);
    return ret;
}


void SRegHMFEPSW::SetEIMASK (RegData eimask)
{
    //All of bit fields related to the interruption priority are changed to 6bit from 8bit.
    // Upper 2bits are fixed to 0.
    m_eimask = eimask & 0x3F;
}


void SRegHMFEPSW::SetCU (RegData cu)
{
    // CU0-1 can be written always. (different from PSW CUn).
    cu &= ~0x4;   // CU2 is fixed to 0 in G4MH
    if (m_pe->GetCoreType() == CORE_G4KH) {
        // CU1：Not support FXU in G4KH
        cu &= ~0x2;
    }
    m_cu = cu;
}


void SRegHMFEPSW::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged (priv) == false) return;

    SetUM  ((data >> 30) & RegData (0x00000001U));
    if ((m_pe->GetCoreType() == CORE_G4MH) && (m_pe->GetCoreVersion() >= CORE_VER_20)) {
        if (m_pe->GetNcSrfPtr()->SrHMINTCFG()->GetEPL () == 1) {
            SetEIMASK ((data >>  20) & RegData (0x000000FFU));
        }
    }
    SetCU  ((data >> 16) & RegData (0x00000007U));
    SetEBV ((data >> 15) & RegData (0x00000001U));
    SetNP  ((data >>  7) & RegData (0x00000001U));
    SetEP  ((data >>  6) & RegData (0x00000001U));
    SetID  ((data >>  5) & RegData (0x00000001U));
    SetSAT ((data >>  4) & RegData (0x00000001U));
    SetCY  ((data >>  3) & RegData (0x00000001U));
    SetOV  ((data >>  2) & RegData (0x00000001U));
    SetS   ((data >>  1) & RegData (0x00000001U));
    SetZ   ((data >>  0) & RegData (0x00000001U));
}


RegData SRegHMFEPSW::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;

    RegData ret = 0x0
        | (GetUM  () << 30)
        | (GetEIMASK () << 20)
        | (GetCU  () << 16)
        | (GetEBV () << 15)
        | (GetNP  () <<  7)
        | (GetEP  () <<  6)
        | (GetID  () <<  5)
        | (GetSAT () <<  4)
        | (GetCY  () <<  3)
        | (GetOV  () <<  2)
        | (GetS   () <<  1)
        | (GetZ   () <<  0);
    return ret;
}


void SRegGMFEPSW::SetEIMASK (RegData eimask)
{
    //All of bit fields related to the interruption priority are changed to 6bit from 8bit.
    // Upper 2bits are fixed to 0.
    m_eimask = eimask & 0x3F;
}


void SRegGMFEPSW::SetCU (RegData cu)
{
    // CU0-1 can be written always. (different from PSW CUn).
    cu &= ~0x4;   // CU2 is fixed to 0 in G4MH
    m_cu = cu;
}


void SRegGMFEPSW::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled () == false) return;

    SetUM  ((data >> 30) & RegData (0x00000001U));
    if ((m_pe->GetCoreType() == CORE_G4MH) && (m_pe->GetCoreVersion() >= CORE_VER_20)) {
        SetEIMASK ((data >>  20) & RegData (0x000000FFU));
    }
    SetCU  ((data >> 16) & RegData (0x00000007U));
    SetEBV ((data >> 15) & RegData (0x00000001U));
    SetNP  ((data >>  7) & RegData (0x00000001U));
    SetEP  ((data >>  6) & RegData (0x00000001U));
    SetID  ((data >>  5) & RegData (0x00000001U));
    SetSAT ((data >>  4) & RegData (0x00000001U));
    SetCY  ((data >>  3) & RegData (0x00000001U));
    SetOV  ((data >>  2) & RegData (0x00000001U));
    SetS   ((data >>  1) & RegData (0x00000001U));
    SetZ   ((data >>  0) & RegData (0x00000001U));
}


RegData SRegGMFEPSW::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled () == false) return 0;

    RegData ret = 0x0
        | (GetUM  () << 30)
        | (GetEIMASK () << 20)
        | (GetCU  () << 16)
        | (GetEBV () << 15)
        | (GetNP  () <<  7)
        | (GetEP  () <<  6)
        | (GetID  () <<  5)
        | (GetSAT () <<  4)
        | (GetCY  () <<  3)
        | (GetOV  () <<  2)
        | (GetS   () <<  1)
        | (GetZ   () <<  0);
    return ret;
}


void SRegHMFEPC::Reset (void)
{
    // For G4MH2.x
    // Keep PC value which is for the last completed instruction in HMFEPC when reset occur.
    if ((m_pe->GetCoreType() == CORE_G4MH) && (m_pe->GetCoreVersion() >= CORE_VER_20)) {
        SetFEPC (m_pe->GetHtPtr()->GetLastCompletedPc ());
    }
    // Otherwise, FEPC value is kept.
}


void SRegGMINTCFG::SetULNR (RegData ulnr)
{
    //All of bit fields related to the interruption priority are changed to 6bit from 8bit.
    // Upper 2bits are fixed to 0.
    m_ulnr = ulnr & 0x3F;
}


void SRegDBPSW::SetEIMASK (RegData eimask)
{
    //All of bit fields related to the interruption priority are changed to 6bit from 8bit.
    // Upper 2bits are fixed to 0.
    m_eimask = eimask & 0x3F;
}


void SRegDBPSW::SetCU (RegData cu)
{
    // CU0-1 can be written always. (different from PSW CUn).
    cu &= ~0x4;   // CU2 is fixed to 0 in G4MH
    if (m_pe->GetCoreType() == CORE_G4KH) {
        // CU1：Not support FXU in G4KH
        cu &= ~0x2;
    }
    m_cu = cu;
}


void SRegDBPSW::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    // Privilege mode checking
    // HVCFG.HVE=1 : DBHV priv is necessary
    //   DBHV: OK
    //   DBSV: OK
    //   DBUM: PIE (Never come. DB mode can't change to SV in Draft1.07)
    //   HV : NOP
    //   SV : NOP
    //   UM : PIE  (Never come. Cause PIE on InstV850::IsPrivUserException)
    // HVCFG.HVE=0 : DBSV priv is necessary
    //   DBHV: NOP (Never come.)
    //   DBSV: OK
    //   DBUM: PIE (Never come. Cause PIE on InstV850::IsPrivUserException)
    //   HV : NOP (Never come.)
    //   SV : NOP
    //   UM : PIE  (Never come. Cause PIE on InstV850::IsPrivUserException)

    if (IsPrivileged(priv) == false) return;
    SetUM ((data >> 30) & RegData (0x00000001U));
    if ((m_pe->GetCoreType() == CORE_G4MH) && (m_pe->GetCoreVersion() >= CORE_VER_20)) {
        if (m_pe->GetNcSrfPtr()->SrHMINTCFG()->GetEPL () == 1) {
            SetEIMASK ((data >>  20) & RegData (0x000000FFU));
        }
    }
    SetCU ((data >> 16) & RegData (0x00000007U));
    SetEBV ((data >> 15) & RegData (0x00000001U));
    SetNP ((data >> 7) & RegData (0x00000001U));
    SetEP ((data >> 6) & RegData (0x00000001U));
    SetID ((data >> 5) & RegData (0x00000001U));
    SetSAT ((data >> 4) & RegData (0x00000001U));
    SetCY ((data >> 3) & RegData (0x00000001U));
    SetOV ((data >> 2) & RegData (0x00000001U));
    SetS ((data >> 1) & RegData (0x00000001U));
    SetZ ((data >> 0) & RegData (0x00000001U));
}


RegData SRegDBPSW::Read (PrivLevel priv, ContextId ctxt) const
{
    // Privilege mode checking
    // HVCFG.HVE=1 : DBHV priv is necessary
    //   DBHV: OK
    //   DBSV: OK
    //   DBUM: PIE (Never come. Cause PIE on InstV850::IsPrivUserException)
    //   HV : NOP
    //   SV : NOP
    //   UM : PIE  (Never come. Cause PIE on InstV850::IsPrivUserException)
    // HVCFG.HVE=0 : DBSV priv is necessary
    //   DBHV: NOP (Never come.)
    //   DBSV: OK
    //   DBUM: PIE (Never come. Cause PIE on InstV850::IsPrivUserException)
    //   HV : NOP (Never come.)
    //   SV : NOP
    //   UM : PIE  (Never come. Cause PIE on InstV850::IsPrivUserException)
    if (IsPrivileged (priv) == false) return 0;
    RegData ret = 0x0
        | (GetUM () << 30)
        | (GetEIMASK () << 20)
        | (GetCU () << 16)
        | (GetEBV () << 15)
        | (GetNP () << 7)
        | (GetEP () << 6)
        | (GetID () << 5)
        | (GetSAT () << 4)
        | (GetCY () << 3)
        | (GetOV () << 2)
        | (GetS () << 1)
        | (GetZ () << 0);
    return ret;
}


void SRegDBPSWH::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    // Privilege mode checking
    // HVCFG.HVE=1 : DBHV priv is necessary
    //   DBHV: OK
    //   DBSV: OK
    //   DBUM: PIE (Never come. Cause PIE on InstV850::IsPrivUserException)
    //   HV : NOP
    //   SV : NOP
    //   UM : PIE  (Never come. Cause PIE on InstV850::IsPrivUserException)
    // HVCFG.HVE=0 : DBSV priv is necessary
    //   DBHV: NOP (Never come.)
    //   DBSV: OK. Treat as undefined register.
    //   DBUM: PIE (Never come. Cause PIE on InstV850::IsPrivUserException)
    //   HV : NOP (Never come.)
    //   SV : NOP. Treat as undefined register.
    //   UM : PIE  (Never come. Cause PIE on InstV850::IsPrivUserException)

	if (IsPrivileged(priv) == false) return;
    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){
        // there is no the register in G4MH1.x
        return;
    }
    SetGM ((data >> 31) & RegData (0x00000001U));
    SetGPID ((data >> 8) & RegData (0x00000007U));
}


RegData SRegDBPSWH::Read (PrivLevel priv, ContextId ctxt) const
{
    // Privilege mode checking
    // HVCFG.HVE=1 : DBHV priv is necessary
    //   DBHV: OK
    //   DBSV: OK
    //   DBUM: PIE (Never come. Cause PIE on InstV850::IsPrivUserException)
    //   HV : NOP
    //   SV : NOP
    //   UM : PIE  (Never come. Cause PIE on InstV850::IsPrivUserException)
    // HVCFG.HVE=0 : DBSV priv is necessary
    //   DBHV: NOP (Never come.)
    //   DBSV: OK. Treat as undefined register.
    //   DBUM: PIE (Never come. Cause PIE on InstV850::IsPrivUserException)
    //   HV : NOP (Never come.)
    //   SV : NOP. Treat as undefined register.
    //   UM : PIE  (Never come. Cause PIE on InstV850::IsPrivUserException)

    if (IsPrivileged (priv) == false) return 0;
    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){
        // there is no the register in G4MH1.x
        return 0;
    }
    RegData ret = 0x0
        | (GetGM () << 31)
        | (GetGPID () << 8);
    return ret;
}


void SRegRBASE::Reset (void)
{
    //! 外部ピン扱い
    m_rbase = m_pe->GetExtPin (EXT_PIN_RBASE) & 0x007fffffU;
    m_dv    = m_pe->GetExtPin (EXT_PIN_DV)    & 0x00000001U;
    m_rint  = m_pe->GetExtPin (EXT_PIN_RINT)  & 0x00000001U;
}


void SRegRBNR::SetBN (RegData bn)
{
    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){
        // there is valid bit only in [4:0] in G4MH1.x
        bn &= 0x1FU;

        // round to 0x10 when over 0x10 is written.
        m_bn = std::min (static_cast<uint32_t> (bn), 0x10U);
    } else {
        // G4MH2.0 : all value (0~FF) can be acceptable. No need to use std::min.
        //All of bit fields related to the interruption priority are changed to 6bit from 8bit.
        // Upper 2bits are fixed to 0.
        m_bn = bn & 0x3F;
    }
}


void SRegTSCOUNTL::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) {
        // cause PIE on write with UM mode.
        m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE);
        return;
    }

    uint64_t count = m_pe->GetHtPtr()->GetTsCounter()->GetCount ();
    count = (count & 0xFFFFFFFF00000000ULL) | (data & 0xFFFFFFFFU);
    m_pe->GetHtPtr()->GetTsCounter()->SetCount (count);
}


RegData SRegTSCOUNTL::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;

    uint64_t count = m_pe->GetHtPtr()->GetTsCounter()->GetCount ();
    RegData ret = 0x0
        | ((count & 0xFFFFFFFFU) << 0);
    return ret;
}


void SRegTSCOUNTH::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) {
        // cause PIE on write with UM mode.
        m_pe->GetHtPtr()->m_trace->SetExpCode (EXP_PIE);
        return;
    }

    uint64_t count = m_pe->GetHtPtr()->GetTsCounter()->GetCount ();
    count = (count & 0xFFFFFFFFULL)
            | (static_cast <uint64_t> (data) << 32);
    m_pe->GetHtPtr()->GetTsCounter()->SetCount (count);
}


RegData SRegTSCOUNTH::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;

    uint64_t count = m_pe->GetHtPtr()->GetTsCounter()->GetCount ();
    RegData ret = 0x0
        | static_cast<uint32_t> ((count >> 32) & 0xFFFFFFFFU);
    return ret;
}


void SRegTSCTRL::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    bool cen = (data >> 0) & 0x1;
    bool ovf = (data >> 1) & 0x1;

    // Reserve count (set value at the last of FsExecute.)
    // When ForestUtil::RegWriteDebug() call this, set value in RegWriteDebug();
    m_pe->GetHtPtr()->GetTsCounter()->ReserveSetting (cen, ovf);
    m_pe->GetHtPtr()->SetPmcReservedFlag ();
    m_pe->GetHtPtr()->SetPmcUpdateFlag ();
}


RegData SRegTSCTRL::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    bool cen = m_pe->GetHtPtr()->GetTsCounter()->GetStart ();
    bool ovf = m_pe->GetHtPtr()->GetTsCounter()->GetOvf ();
    RegData ret = 0x0
        | ((ovf & 0x1) << 1)
        | ((cen & 0x1) << 0);
    return ret;
}


void SRegPMCOUNT::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){
        if (m_id >= 4) {
            // G4MH1.x only has 4ch. Then, treated as undefined register.
            if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) {
                m_pe->GetHtPtr()->m_trace->SetExpCode (EXP_PIE);
            }
            return;
        }
    }

    // Check access permission by PMUMCTRL and PMGMCTRL
    uint32_t permit_um = ((m_pe->GetHtPtr()->m_srf->SrPMUMCTRL ()->GetPMUE ()) >> m_id) & 0x1;
    uint32_t permit_gm;
    if (m_pe->IsHvEnabled ()) {
        permit_gm = ((m_pe->GetHtPtr()->m_srf->SrPMGMCTRL ()->GetPMGE ()) >> m_id) & 0x1;
    } else {
        permit_gm = 1; // If virtualization is disabled, guest mode checking is ignored.
    }
    if (   ((permit_um == 0) && (HasPrivileged (PRIV_PERMIT_SV, priv) == false))
        || ((permit_gm == 0) && (m_pe->IsHostMode () == false)) ){
        m_pe->GetHtPtr()->m_trace->SetExpCode (EXP_PIE);
        return;
    }

    m_pe->GetHtPtr()->GetPmCounter(m_id)->SetCount (data);
}


RegData SRegPMCOUNT::Read (PrivLevel priv, ContextId ctxt) const
{
    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){
        if (m_id >= 4) {
            // G4MH1.x only has 4ch. Then, treated as undefined register.
            if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) {
                m_pe->GetHtPtr()->m_trace->SetExpCode (EXP_PIE);
            }
            return 0;
        }
    }

    // Check access permission by PMUMCTRL and PMGMCTRL
    uint32_t permit_um = ((m_pe->GetHtPtr()->m_srf->SrPMUMCTRL ()->GetPMUE ()) >> m_id) & 0x1;
    uint32_t permit_gm;
    if (m_pe->IsHvEnabled ()) {
        permit_gm = ((m_pe->GetHtPtr()->m_srf->SrPMGMCTRL ()->GetPMGE ()) >> m_id) & 0x1;
    } else {
        permit_gm = 1; // If virtualization is disabled, guest mode checking is ignored.
    }
    if (   ((permit_um == 0) && (HasPrivileged (PRIV_PERMIT_SV, priv) == false))
        || ((permit_gm == 0) && (m_pe->IsHostMode () == false)) ){
        m_pe->GetHtPtr()->m_trace->SetExpCode (EXP_PIE);
        return 0;
    }

    RegData ret = 0x0
        | (m_pe->GetHtPtr()->GetPmCounter(m_id)->GetCount () << 0);
    return ret;
}


void SRegPMCTRL::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){
        if (m_id >= 4) {
            // G4MH1.x only has 4ch. Then, treated as undefined register.
            if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) {
                m_pe->GetHtPtr()->m_trace->SetExpCode (EXP_PIE);
            }
            return;
        }
    }

    // Check access permission by PMUMCTRL and PMGMCTRL
    uint32_t permit_um = ((m_pe->GetHtPtr()->m_srf->SrPMUMCTRL ()->GetPMUE ()) >> m_id) & 0x1;
    uint32_t permit_gm;
    if (m_pe->IsHvEnabled ()) {
        permit_gm = ((m_pe->GetHtPtr()->m_srf->SrPMGMCTRL ()->GetPMGE ()) >> m_id) & 0x1;
    } else {
        permit_gm = 1; // If virtualization is disabled, guest mode checking is ignored.
    }
    if (   ((permit_um == 0) && (HasPrivileged (PRIV_PERMIT_SV, priv) == false))
        || ((permit_gm == 0) && (m_pe->IsHostMode () == false)) ){
        m_pe->GetHtPtr()->m_trace->SetExpCode (EXP_PIE);
        return;
    }

    bool he;
    uint32_t ge;
    if ((m_pe->GetCoreType() == CORE_G4MH) && (m_pe->GetCoreVersion() >= CORE_VER_20)) {
        // About PMCTRL.HE/GE when HVE=0
        //   G4MH2.1 Impl: read 0 and write ignore
        //   G4MH2.0 Impl: readable and writable
        if ((m_pe->GetCoreVersion() == CORE_VER_20) || (m_pe->IsHvEnabled ())) {
            he = (data >> 24) & 0x1U;
            ge = (data >> 16) & RegData (0x000000FFU);
        } else {
            // Keep current val (undefined behavior)
            he = m_he;
            ge = m_ge;
        }
    } else {
        he = false;
        ge = 0;
    }

    uint32_t cnd = (data >> 8) & RegData (0x000000ffU);
    bool ovf = (data >> 1) & 0x1U;
    bool cen = (data >> 0) & 0x1U;

    // Reserve count (set value at the last of FsExecute.)
    // When ForestUtil::RegWriteDebug() call this, set value in RegWriteDebug();
    m_pe->GetHtPtr()->GetPmCounter(m_id)->ReserveSetting (cnd, cen, ovf, he, ge);
    m_pe->GetHtPtr()->SetPmcReservedFlag ();
    m_pe->GetHtPtr()->SetPmcUpdateFlag ();
}


RegData SRegPMCTRL::Read (PrivLevel priv, ContextId ctxt) const
{
    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){
        if (m_id >= 4) {
            // G4MH1.x only has 4ch. Then, treated as undefined register.
            if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) {
                m_pe->GetHtPtr()->m_trace->SetExpCode (EXP_PIE);
            }
            return 0;
        }
    }

    // Check access permission by PMUMCTRL and PMGMCTRL
    uint32_t permit_um = ((m_pe->GetHtPtr()->m_srf->SrPMUMCTRL ()->GetPMUE ()) >> m_id) & 0x1;
    uint32_t permit_gm;
    if (m_pe->IsHvEnabled ()) {
        permit_gm = ((m_pe->GetHtPtr()->m_srf->SrPMGMCTRL ()->GetPMGE ()) >> m_id) & 0x1;
    } else {
        permit_gm = 1; // If virtualization is disabled, guest mode checking is ignored.
    }
    if (   ((permit_um == 0) && (HasPrivileged (PRIV_PERMIT_SV, priv) == false))
        || ((permit_gm == 0) && (m_pe->IsHostMode () == false)) ){
        m_pe->GetHtPtr()->m_trace->SetExpCode (EXP_PIE);
        return 0;
    }
    uint32_t cnd = m_pe->GetHtPtr()->GetPmCounter(m_id)->GetCondition () & 0xFFU;
    bool ovf = m_pe->GetHtPtr()->GetPmCounter(m_id)->GetOvf () & 0x1U;
    bool cen = m_pe->GetHtPtr()->GetPmCounter(m_id)->GetStart () & 0x1U;
    RegData ret = 0x0
        | (cnd << 8)
        | (ovf << 1)
        | (cen << 0);

    if ((m_pe->GetCoreType() == CORE_G4MH) && (m_pe->GetCoreVersion() >= CORE_VER_20)) {
        // About PMCTRL.HE/GE when HVE=0
        //   G4MH2.1 Impl: read 0 and write ignore
        //   G4MH2.0 Impl: readable and writable
        if ((m_pe->GetCoreVersion() == CORE_VER_20) || (m_pe->IsHvEnabled ())) {
            bool he = m_pe->GetHtPtr()->GetPmCounter(m_id)->GetHe () & 0x1U;
            uint32_t ge = m_pe->GetHtPtr()->GetPmCounter(m_id)->GetGe () & 0xFFU;
            ret |=
                  (he << 24)
                | (ge << 16);
        }
    }

    return ret;
}


void SRegPMSUBCND::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){
        if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode (EXP_PIE);
        }
        return;
    }

    // Check access permission by PMUMCTRL and PMGMCTRL
    uint32_t permit_um = ((m_pe->GetHtPtr()->m_srf->SrPMUMCTRL ()->GetPMUE ()) >> m_id) & 0x1;
    uint32_t permit_gm;
    if (m_pe->IsHvEnabled ()) {
        permit_gm = ((m_pe->GetHtPtr()->m_srf->SrPMGMCTRL ()->GetPMGE ()) >> m_id) & 0x1;
    } else {
        permit_gm = 1; // If virtualization is disabled, guest mode checking is ignored.
    }
    if (   ((permit_um == 0) && (HasPrivileged (PRIV_PERMIT_SV, priv) == false))
        || ((permit_gm == 0) && (m_pe->IsHostMode () == false)) ){
        m_pe->GetHtPtr()->m_trace->SetExpCode (EXP_PIE);
        return;
    }
    // Reserve count (set value at the last of FsExecute.)
    // When ForestUtil::RegWriteDebug() call this, set value in RegWriteDebug();
    RegData subcnd = (data >> 0) & (0x00001FFFU);
    SetSUBCND (subcnd);
    m_pe->GetHtPtr()->GetPmCounter(m_id)->ReserveSettingSubcond (subcnd);
    m_pe->GetHtPtr()->SetPmcReservedFlag ();
    m_pe->GetHtPtr()->SetPmcUpdateFlag ();
}


RegData SRegPMSUBCND::Read (PrivLevel priv, ContextId ctxt) const
{
    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){
        if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode (EXP_PIE);
        }
        return 0;
    }

    // Check access permission by PMUMCTRL and PMGMCTRL
    uint32_t permit_um = ((m_pe->GetHtPtr()->m_srf->SrPMUMCTRL ()->GetPMUE ()) >> m_id) & 0x1;
    uint32_t permit_gm;
    if (m_pe->IsHvEnabled ()) {
        permit_gm = ((m_pe->GetHtPtr()->m_srf->SrPMGMCTRL ()->GetPMGE ()) >> m_id) & 0x1;
    } else {
        permit_gm = 1; // If virtualization is disabled, guest mode checking is ignored.
    }
    if (   ((permit_um == 0) && (HasPrivileged (PRIV_PERMIT_SV, priv) == false))
        || ((permit_gm == 0) && (m_pe->IsHostMode () == false)) ){
        m_pe->GetHtPtr()->m_trace->SetExpCode (EXP_PIE);
        return 0;
    }

    RegData ret = 0x0
          | (GetSUBCND() << 0);
    return ret;
}


void SRegPMUMCTRL::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged (priv) == false) return;
    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){
        // there is no the bit [7:4] in G4MH1.x
        data &= 0xFFFFFF0FU;
    }

    SetPMUE ((data >> 0) & RegData (0x000000ffU));
}


void SRegPID::Reset (void)
{
    // FID : LSB is PID[8]
    // bit23  Virtualization (HV)
    // bit22  reserved
    // bit21, 20 reserved
    // bit19  reserved
    // bit18  register bank
    // bit17-12  reserved
    // bit11  floating point SIMD
    // bit10  Double precision FPU
    // bit9   Single precision FPU
    // bit8   MPU
    m_fid = 0x0;

    // PID,VID
    uint32_t pid;
    uint32_t vid;
    switch (m_pe->GetCoreType()) {
    case CORE_G4MH:
        if (m_pe->HasFxu()) { m_fid |= 0x0008; }
        if (m_pe->HasFpu())  { m_fid |= 0x0006; }
        if (m_pe->HasMpu())  { m_fid |= 0x0001; }

        m_fid |= 0x0400; // reg bank
        switch (m_pe->GetCoreVersion()) {
        case CORE_VER_10:
            pid = 0x06U; // G4MH1.0 is RH850 version2.0
            vid = 0x00U;
            break;
        case CORE_VER_11:
            pid = 0x06U; // G4MH1.1 is RH850 version2.0
            vid = 0x01U;
            break;
        case CORE_VER_20:
            pid = 0x07U; // G4MH2.0 is RH850 version2.1
            vid = 0x00U;
            if (m_pe->GetExtPin (EXT_PIN_HAS_HV) == 1) {
                m_fid |= 0x8000; // Virtualization
            }
            break;
        case CORE_VER_21:
            pid = 0x07U; // G4MH2.1 is RH850 version2.1
            vid = 0x01U;
            if (m_pe->GetExtPin (EXT_PIN_HAS_HV) == 1) {
                m_fid |= 0x8000; // Virtualization
            }
            break;

        default:
            // Not reached here
            pid = 0x00U;
            vid = 0x00U;
        }
        break;
    case CORE_G4KH:
        if (m_pe->HasFpu())  { m_fid |= 0x0002; }
        if (m_pe->HasMpu())  { m_fid |= 0x0001; }

        m_fid |= 0x0400; // reg bank function
        switch (m_pe->GetCoreVersion()) {
        case CORE_VER_10:
            pid = 0x06U; // G4KH1.0 is RH850 version2.0
            vid = 0xA8U;
            break;
        default:
            // Not reached here
            pid = 0x00U;
            vid = 0x00U;
        }
        break;
    default:
        pid = 0x00U;
        vid = 0x00U;
    }
    m_pid = pid;
    m_vid = vid;
}


void SRegHMSPID::Write (RegData data, PrivLevel priv, ContextId ctxt) {
    if (IsPrivileged (priv) == false) return;

    // SVLOCK : SV/HV mode can't update the resource
    if (m_pe->GetHtPtr()->m_srf->SrVSVLOCK ()->GetSVL () == 1) {
        return;
    }

    SetSPID ((data >> 0) & RegData (0x0000001fU));
}


void SRegHMSPID::SetSPID (RegData spid)
{
    // To prevent the issue by the order of reset, use extpin directly for spidlist
    uint32_t list = m_pe->GetExtPin (EXT_PIN_SPIDLIST) & 0xFFFFFFFF;
    if (((list >> spid) & 0x1) == 1) {
        m_spid = spid;
        if ((m_pe->IsHvEnabled () == false) || (m_pe->IsHostMode () == true)) {
            m_pe->UpdateSideband ();
#ifdef SUPPORT_CUBESUITE
            (*CubeCallBack::__NotifySPID) (m_pe->GetHtPtr()->GetPeId (), spid);
#endif // SUPPORT_CUBESUITE
        }
    } else {
        CfMsg::DPrint (MSG_WAR, "<Warning: SPID=%u isn't permitted by SPIDLIST\n", spid);
    }
}


void SRegHMSPID::Reset (void)
{
    uint32_t extpin_spid = m_pe->GetExtPin (EXT_PIN_SPID) & SPID_MASK;
    SetSPID (extpin_spid);
}


void SRegGMSPID::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled () == false) return;

    // Host mode can update "GMSPID" even if SVL=1.
    // Guest mode can't update GMSPID via SPID register if SVL=1.
    // SVLOCK : SV mode can't update the resource
    if   ((m_pe->GetHtPtr()->m_srf->SrVSVLOCK ()->GetSVL () == 1)
       && (HasPrivileged (PRIV_PERMIT_HV, priv) == false)) {
        return;
    }

    SetSPID ((data >> 0) & RegData (0x0000001fU));
}


void SRegGMSPID::SetSPID (RegData spid)
{
    uint32_t list = m_pe->GetHtPtr()->m_srf->SrVSPIDLIST ()->GetSPIDLIST ();
    if (((list >> spid) & 0x1) == 1) {
        m_spid = spid;
        if ((m_pe->IsHvEnabled () == true) && (m_pe->IsHostMode () == false)) {
            m_pe->UpdateSideband ();
#ifdef SUPPORT_CUBESUITE
            (*CubeCallBack::__NotifySPID) (m_pe->GetHtPtr()->GetPeId (), spid);
#endif // SUPPORT_CUBESUITE
        }
    } else {
        CfMsg::DPrint (MSG_WAR, "<Warning: SPID=%u isn't permitted by current SPIDLIST\n", spid);
    }
}


void SRegGMSPID::Reset (void)
{
    // To guarantee to initialize SPIDLIST reg before SPID reg.
    m_pe->GetHtPtr()->m_srf->SrVSPIDLIST ()->Reset ();

    uint32_t extpin_spid = m_pe->GetExtPin (EXT_PIN_SPID) & SPID_MASK;
    SetSPID (extpin_spid);
}


void SRegHMSPIDLIST::Reset (void)
{
    m_spidlist = m_pe->GetExtPin (EXT_PIN_SPIDLIST) & 0xFFFFFFFF; //! External pin
}


void SRegGMSPIDLIST::SetSPIDLIST (RegData spidlist)
{
    uint32_t list = m_pe->GetExtPin (EXT_PIN_SPIDLIST) & 0xFFFFFFFF; //! External pin
    m_spidlist = spidlist & list;
}


void SRegGMSPIDLIST::Write(RegData data, PrivLevel priv, ContextId ctxt)
{
    //Only host mode can write GMSPIDLIST. Otherwise, write is ignored.
    // @@ TODO : create other virtual register to write via debugger API always.
    if ((m_pe->IsHvEnabled() == true) && (m_pe->IsHostMode () == true)) {
        SetSPIDLIST ((data >> 0) & RegData (0xffffffffU));
    }
}


void SRegGMSPIDLIST::Reset (void)
{
    m_spidlist = m_pe->GetExtPin (EXT_PIN_SPIDLIST) & 0xFFFFFFFF; //! External pin
}


void SRegHMPEID::Reset (void)
{
    m_peid = m_pe->GetExtPin(EXT_PIN_PEID) & PEID_MASK; //! External pin
}


void SRegHMPEID::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){
        // Because PEID is SV mode on G4MH1.x, cause PIE on UM mode.
        if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE);
            return;
        }
    } else {
        if (IsPrivileged (priv) == false) return;
    }
}


RegData SRegHMPEID::Read (PrivLevel priv, ContextId ctxt) const
{
    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){
        // Because PEID is SV mode on G4MH1.x, cause PIE on UM mode.
        if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE);
            return 0;
        }
    } else {
        if (IsPrivileged (priv) == false) return 0;
    }

    uint32_t peid_pos = 0; // PEID is [2:0] in G4x spec
#ifdef ENABLE_PEID_POS_CTRL
    if (m_pe->IsPeidPosG4x () == false) {
        // Use [18:16] for PEID bit pos as same as G3x HTCFG0 register instead of [2:0].
        peid_pos = 16;
    }
#endif // #ifdef ENABLE_PEID_POS_CTRL
    RegData ret = 0x0
        | (GetPEID () << peid_pos);
    return ret;
}


void SRegGMPEID::Reset (void)
{
    m_peid = m_pe->GetExtPin(EXT_PIN_PEID) & PEID_MASK; //! External pin
}


void SRegGMPEID::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled () == false) return;
    if (m_pe->IsHostMode () == true) {
        // In case of Host mode, only HV mode can access GMPEID register.
        // UM on Host mode can't acces (PIE occur).
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            // Host UM mode
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE);
            return;
        } else {
            // PEID field is only updated in HV privilege.
            SetPEID ((data >> 0) & RegData (0x0000001fU));
        }
    } else {
        // In guest mode, GMPEID can access via PEID register.
        // Because GMPEID direct access can't be, all access is via PEID.
        // If GMPEID is accessed directly, SRegDUMMY(HV) is used instead of GMPEID to cause PIE.
        // No exception occur on Guest mode here
    }
}


RegData SRegGMPEID::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled () == false) return 0;
    if (m_pe->IsHostMode () == true) {
        // In case of Host mode, only HV mode can access GMPEID register.
        // UM on Host mode can't acces (PIE occur).
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            // Host UM mode
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE);
            return 0;
        }
    } else {
        // In guest mode, GMPEID can access via PEID register.
        // Because GMPEID direct access can't be, all access is via PSW.
        // If GMPEID is accessed directly, SRegDUMMY(HV) is used instead of SRegGMPEID to cause PIE.
        ; // No exception occur on Guest mode here
    }

    // PEID field is read in HV privilege (SV in Host mode) and User, SV privilege (via PEID register) in Guest Mode.
    RegData ret = 0x0
        | (GetPEID () << 0);
    return ret;
}


void SRegBMID::Reset (void)
{
    m_bmid = m_pe->GetExtPin(EXT_PIN_BMID) & BMID_MASK; //! External pin
    m_pe->UpdateSideband ();
}


void SRegBMID::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){
        // Because BMID is SV mode on G4MH1.x, cause PIE on UM mode.
        if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE);
            return;
        }
    } else {
        if (IsPrivileged (priv) == false) return;
    }
}


RegData SRegBMID::Read (PrivLevel priv, ContextId ctxt) const
{
    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){
        // Because BMID is SV mode on G4MH1.x, cause PIE on UM mode.
        if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE);
            return 0;
        }
    } else {
        if (IsPrivileged (priv) == false) return 0;
    }

    RegData ret = 0x0
        | (GetBMID () << 0);
    return ret;
}


void SRegFPSR::Reset(void)
{
    m_fn = 0x0U;
    m_if_ = 0x0U;
    if (m_pe->GetCoreType() == CORE_G4KH) {
        m_pem = 0x0U;
    } else {
        m_pem = 0x1U;
    }
    m_rm = 0x0U;
    m_fs = 0x1U;
    m_xe = 0x0U;
 }


void SRegFPSR::SetPEM (bool pem)
{
    if (m_pe->GetCoreType() == CORE_G4KH) {
        m_pem = pem;
    } else {
        return;
    }
 }


void SRegFPST::SetXC (RegData xc)
{ m_pe->GetHtPtr()->m_srf->SrFPSR()->SetXC (xc); }

void SRegFPST::SetIF_ (bool if_)
{ m_pe->GetHtPtr()->m_srf->SrFPSR()->SetIF_ (if_); }

void SRegFPST::SetXP (RegData xp)
{ m_pe->GetHtPtr()->m_srf->SrFPSR()->SetXP (xp); }

void SRegFPCC::SetCC (RegData cc)
{ m_pe->GetHtPtr()->m_srf->SrFPSR()->SetCC (cc); }

void SRegFPCFG::SetRM (RegData rm)
{ m_pe->GetHtPtr()->m_srf->SrFPSR()->SetRM (rm); }

void SRegFPCFG::SetXE (RegData xe)
{ m_pe->GetHtPtr()->m_srf->SrFPSR()->SetXE (xe); }


RegData SRegFPST::GetXC (void) const
{ return m_pe->GetHtPtr()->m_srf->SrFPSR()->GetXC (); }

bool SRegFPST::GetIF_ (void) const
{ return m_pe->GetHtPtr()->m_srf->SrFPSR()->GetIF_ (); }

RegData SRegFPST::GetXP (void) const
{ return m_pe->GetHtPtr()->m_srf->SrFPSR()->GetXP (); }

RegData SRegFPCC::GetCC (void) const
{ return m_pe->GetHtPtr()->m_srf->SrFPSR()->GetCC (); }

RegData SRegFPCFG::GetRM (void) const
{ return m_pe->GetHtPtr()->m_srf->SrFPSR()->GetRM (); }

RegData SRegFPCFG::GetXE (void) const
{ return m_pe->GetHtPtr()->m_srf->SrFPSR()->GetXE (); }


void SRegFPEC::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged (priv) == false) return;

    if (priv <= PRIV_PERMIT_DBUM || (data & 0x01) == 0x0) {
        //! Writable only when debugging or when FPIVD is 0
        SetFPIVD (0);
    }
}

void SRegFPEC::SetFPIVD (bool fpivd)
{
    m_fpivd = fpivd;

    // Exception occurrence by FPIVD reflected in interrupt flag.
    m_pe->GetHtPtr()->UpdateIntRequested ();
}


void SRegHMMPM::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged (priv) == false) return;
    // SVLOCK : SV/HV mode can't update the resource
    if (m_pe->GetHtPtr()->m_srf->SrVSVLOCK ()->GetSVL () == 1) {
        return;
    }

    SetSVP ((data >> 1) & RegData (0x00000001U));
    SetMPE ((data >> 0) & RegData (0x00000001U));
}


void SRegHMMPM::SetMPE (bool mpe)
{
    if (m_pe->HasMpu ()) {
        m_mpe = mpe;
    }
}


bool SRegHMMPM::GetSVP (void) const
{
    return m_svp;
}


bool SRegHMMPM::GetMPE (void) const
{
    return m_mpe;
}


void SRegGMMPM::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled () == false) return;

    // Host mode can update "GMMPM" even if SVL=1.
    // Guest mode can't update GMMPM via MPM register if SVL=1.
    // SVLOCK : SV mode can't update the resource
    if   ((m_pe->GetHtPtr()->m_srf->SrVSVLOCK ()->GetSVL () == 1)
       && (HasPrivileged (PRIV_PERMIT_HV, priv) == false)) {
        return;
    }

    // GMPE can be written by HOST mode via GMMPM.
    if (HasPrivileged (PRIV_PERMIT_HV, priv) == true) {
        SetGMPE ((data >> 2) & RegData (0x00000001U));
    }

    SetSVP  ((data >> 1) & RegData (0x00000001U));
    SetMPE  ((data >> 0) & RegData (0x00000001U));
}


RegData SRegGMMPM::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled () == false) return 0; // Virtual resource can't be read on HVE=1.

    RegData ret = 0x0
        | (GetGMPE () << 2)
        | (GetSVP () << 1)
        | (GetMPE () << 0);
    return ret;
}


void SRegGMMPM::SetMPE (bool mpe)
{
    if (m_pe->HasMpu ()) {
        m_mpe = mpe;
    }
}


void SRegGMCFG::SetGCU1 (bool gcu1)
{
    m_gcu1 = gcu1;
    if (gcu1 == 0) {
        // If GCU1 is 0, clear GMPSW.CU1
        SRegGMPSW* gmpsw = m_pe->GetNcSrfPtr ()->SrGMPSW ();
        uint32_t new_cu = gmpsw->GetCU() & 0x1; // clear CU1 (Keep CU0)
        gmpsw->SetCU (new_cu);
    }
}


void SRegGMCFG::SetGCU0 (bool gcu0)
{
    m_gcu0 = gcu0;
    if (gcu0 == 0) {
        // If GCU0 is 0, clear GMPSW.CU0
        SRegGMPSW* gmpsw = m_pe->GetNcSrfPtr ()->SrGMPSW ();
        uint32_t new_cu = gmpsw->GetCU() & 0x2; // clear CU0 (Keep CU1)
        gmpsw->SetCU (new_cu);
    }
}


void SRegMCR::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged (priv) == false) return;

    if ((m_pe->GetCoreType() == CORE_G4MH) && (m_pe->GetCoreVersion() >= CORE_VER_20)) {
        SetHSXE ((data >> 29) & RegData (0x00000001U));
        SetHSWE ((data >> 28) & RegData (0x00000001U));
        SetHSRE ((data >> 27) & RegData (0x00000001U));
        SetHUXE ((data >> 26) & RegData (0x00000001U));
        SetHUWE ((data >> 25) & RegData (0x00000001U));
        SetHURE ((data >> 24) & RegData (0x00000001U));
        SetGSXE ((data >> 21) & RegData (0x00000001U));
        SetGSWE ((data >> 20) & RegData (0x00000001U));
        SetGSRE ((data >> 19) & RegData (0x00000001U));
        SetGUXE ((data >> 18) & RegData (0x00000001U));
        SetGUWE ((data >> 17) & RegData (0x00000001U));
        SetGURE ((data >> 16) & RegData (0x00000001U));
    }
    SetOV ((data >> 8) & RegData (0x00000001U));
    SetSXE ((data >> 5) & RegData (0x00000001U));
    SetSWE ((data >> 4) & RegData (0x00000001U));
    SetSRE ((data >> 3) & RegData (0x00000001U));
    SetUXE ((data >> 2) & RegData (0x00000001U));
    SetUWE ((data >> 1) & RegData (0x00000001U));
    SetURE ((data >> 0) & RegData (0x00000001U));
}


RegData SRegMCR::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;
    RegData ret = 0x0
        | (GetOV () << 8)
        | (GetSXE () << 5)
        | (GetSWE () << 4)
        | (GetSRE () << 3)
        | (GetUXE () << 2)
        | (GetUWE () << 1)
        | (GetURE () << 0);

    if ((m_pe->GetCoreType() == CORE_G4MH) && (m_pe->GetCoreVersion() >= CORE_VER_20)) {
        if (m_pe->IsHvEnabled ()) {
        ret |=
              (GetHSXE () << 29)
            | (GetHSWE () << 28)
            | (GetHSRE () << 27)
            | (GetHUXE () << 26)
            | (GetHUWE () << 25)
            | (GetHURE () << 24)
            | (GetGSXE () << 21)
            | (GetGSWE () << 20)
            | (GetGSRE () << 19)
            | (GetGUXE () << 18)
            | (GetGUWE () << 17)
            | (GetGURE () << 16);
        }
    }

    return ret;
}


void SRegMCC::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged (priv) == false) return;

    // 任意の値書込みにより、メモリ保護設定チェックを開始
    m_pe->GetHtPtr()->m_lsu->GetMpuPtr()->CheckMpuSetting();
}


void SRegMPCFG::SetDMDP (bool dmdp)
{
    m_dmdp = dmdp;

    // SetDMDP is called only in Reset().
    // Then, CPU is on Host mode or conventional mode always (not in guest mode).
    // Therefore, set MPCFG.DMDP to LSU.
    m_pe->GetHtPtr()->m_lsu->SetMdpSuppress (GetDMDP());
}


void SRegMPCFG::Reset (void)
{
    if ((m_pe->GetCoreType() == CORE_G4MH) && (m_pe->GetCoreVersion() >= CORE_VER_20)) {
        m_arch = 0x2U;
        // In ISA spec 2.6, MPCFG.NBK is changed from 1 to 0.
        // CForest users can change this setting to ISA spec 2.5/2.6 by python command.
        uint32_t num_bank = m_pe->GetExtPin (EXT_PIN_MPBANK);
        m_nbk = num_bank - 1;
        m_hbe = 0x0U;
    } else {
        m_arch = 0x1U;
    }

    m_nmpue = m_pe->GetNumMpuRegion () - 1;
    SetDMDP (m_pe->GetExtPin (EXT_PIN_MDP_DMDP) & 0x1);
}


void SRegMPCFG::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Write needs HV priv.
            return;
        }
        SetHBE ((data >> 8) & RegData (0x0000003fU));
    }
}


bool SRegMPCFG::GetDMDP (void) const
{
    if ((m_pe->GetCoreType() == CORE_G4MH) && (m_pe->IsHvEnabled () == false)) {
        return m_dmdp;
    } else {
        return 0;
    }
}


RegData SRegMPCFG::Read (PrivLevel priv, ContextId ctxt) const
{
    if ((m_pe->GetCoreType() == CORE_G4MH) && (m_pe->GetCoreVersion() >= CORE_VER_20)) {
        RegData ret = 0x0
            | (GetDMDP () << 24)
            | (GetNBK () << 20)
            | (GetARCH () << 16)
            | (GetHBE () << 8)
            | (GetNMPUE () << 0);
        return ret;
    }

    // In case G4MH1.1.
    RegData ret = 0x0
        | (GetDMDP () << 24)
        | (GetARCH () << 16)
        | (GetNMPUE () << 0);
    return ret;
}


void SRegMPBK::SetBK (bool bk)
{
    // Bank number is changed when there are more than 2 MPU banks.
    if (m_pe->GetExtPin (EXT_PIN_MPBANK) == 2) {
        m_bk = bk;
        m_pe->ChangeMpuBank (bk);
    }
}


void SRegMPBK::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){ return; }

    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    // SVLOCK : SV/HV mode can't update the resource
    if (m_pe->GetHtPtr()->m_srf->SrVSVLOCK ()->GetSVL () == 1) {
        return;
    }
    SetBK ((data >> 0) & RegData (0x00000001U)); // bit 1 is fixed to 0 in G4MH2.0
}


RegData SRegMPBK::Read (PrivLevel priv, ContextId ctxt) const
{
    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){ return 0; }

    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret = 0x0
        | (GetBK () << 0);
    return ret;
}


void SRegMPBK::Reset (void)
{
    SetBK (0x0U);
}


void SRegMPLA::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged (priv) == false) return;
    // SVLOCK : SV/HV mode can't update the resource
    if (m_pe->GetHtPtr()->m_srf->SrVSVLOCK ()->GetSVL () == 1) {
        return;
    }

    uint32_t idx = m_pe->GetHtPtr()->m_srf->SrMPIDX ()->GetMPIDX ();
    if (m_pe->GetNumMpuRegion () <= idx) return;

    m_pe->GetHtPtr()->m_srf->GetSrPtr (idx + CF_SR_MPLA0)->Write (data, priv, ctxt);
}


RegData SRegMPLA::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;

    uint32_t idx = m_pe->GetHtPtr()->m_srf->SrMPIDX ()->GetMPIDX ();
    if (m_pe->GetNumMpuRegion () <= idx) return 0;

    return m_pe->GetHtPtr()->m_srf->GetSrPtr (idx + CF_SR_MPLA0)->Read (priv, ctxt);
}


void SRegMPUA::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged (priv) == false) return;
    // SVLOCK : SV/HV mode can't update the resource
    if (m_pe->GetHtPtr()->m_srf->SrVSVLOCK ()->GetSVL () == 1) {
        return;
    }

    uint32_t idx = m_pe->GetHtPtr()->m_srf->SrMPIDX ()->GetMPIDX ();
    if (m_pe->GetNumMpuRegion () <= idx) return;

    m_pe->GetHtPtr()->m_srf->GetSrPtr (idx + CF_SR_MPUA0)->Write (data, priv, ctxt);
}


RegData SRegMPUA::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;

    uint32_t idx = m_pe->GetHtPtr()->m_srf->SrMPIDX ()->GetMPIDX ();
    if (m_pe->GetNumMpuRegion () <= idx) return 0;

    return m_pe->GetHtPtr()->m_srf->GetSrPtr (idx + CF_SR_MPUA0)->Read (priv, ctxt);
}


void SRegMPAT::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged (priv) == false) return;
    // SVLOCK : SV/HV mode can't update the resource
    if (m_pe->GetHtPtr()->m_srf->SrVSVLOCK ()->GetSVL () == 1) {
        return;
    }

    uint32_t idx = m_pe->GetHtPtr()->m_srf->SrMPIDX ()->GetMPIDX ();
    if (m_pe->GetNumMpuRegion () <= idx) return;

    m_pe->GetHtPtr()->m_srf->GetSrPtr (idx + CF_SR_MPAT0)->Write (data, priv, ctxt);
}


RegData SRegMPAT::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;

    uint32_t idx = m_pe->GetHtPtr()->m_srf->SrMPIDX ()->GetMPIDX ();
    if (m_pe->GetNumMpuRegion () <= idx) return 0;

    return m_pe->GetHtPtr()->m_srf->GetSrPtr (idx + CF_SR_MPAT0)->Read (priv, ctxt);
}


void SRegMPLA_N::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged (priv) == false) return;
    if (m_pe->GetNumMpuRegion () <= m_id) return;
    // SVLOCK : SV/HV mode can't update the resource
    if (m_pe->GetHtPtr()->m_srf->SrVSVLOCK ()->GetSVL () == 1) {
        return;
    }

    // Guest mode can't write to host managed MPAT/MPUA/MPLA registers
    if (m_pe->IsHvEnabled ()) {
        uint32_t host_entry_start_id = m_pe->GetSrfPtr(NC)->SrMPCFG ()->GetHBE ();
        if (m_id >= host_entry_start_id) {
            // This is the Host managed MPU entry
            if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
                // No PIE
                return;
            }
        }
    }
    SetMPLA ((data >> 2) & RegData (0x3fffffffU));
}


RegData SRegMPLA_N::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;
    if (m_pe->GetNumMpuRegion () <= m_id) return 0;
    return (GetMPLA () << 2);
}


void SRegMPUA_N::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged (priv) == false) return;
    if (m_pe->GetNumMpuRegion () <= m_id) return;
    // SVLOCK : SV/HV mode can't update the resource
    if (m_pe->GetHtPtr()->m_srf->SrVSVLOCK ()->GetSVL () == 1) {
        return;
    }
    // Guest mode can't write to host managed MPAT/MPUA/MPLA registers
    if (m_pe->IsHvEnabled ()) {
        uint32_t host_entry_start_id = m_pe->GetSrfPtr(NC)->SrMPCFG ()->GetHBE ();
        if (m_id >= host_entry_start_id) {
            // This is the Host managed MPU entry
            if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
                // No PIE
                return;
            }
        }
    }
    SetMPUA ((data >> 2) & RegData (0x3fffffffU));
}


RegData SRegMPUA_N::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;
    if (m_pe->GetNumMpuRegion () <= m_id) return 0;
    return (GetMPUA () << 2);
}


void SRegMPAT_N::SetE (bool e)
{
    m_e = e;

    //! １MPU領域だけ更新すればよいが、同様のルーチンが3個あり、
    //! 不具合恐いので全MPU領域再割り当ての共通ルーチンを使うように変更。
    //! 頻繁に実施されないので、速度影響は無視。

    //! MPUクラスの情報を再構成
    m_pe->ReallocMpuRegion ();
}


void SRegMPAT_N::Reset(void)
{
    SetE (0);
}


void SRegMPAT_N::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged (priv) == false) return;
    if (m_pe->GetNumMpuRegion () <= m_id) return;
    // SVLOCK : SV/HV mode can't update the resource
    if (m_pe->GetHtPtr()->m_srf->SrVSVLOCK ()->GetSVL () == 1) {
        return;
    }
    // Guest mode can't write to host managed MPAT/MPUA/MPLA registers
    if (m_pe->IsHvEnabled ()) {
        uint32_t host_entry_start_id = m_pe->GetSrfPtr(NC)->SrMPCFG ()->GetHBE ();
        if (m_id >= host_entry_start_id) {
            // This is the Host managed MPU entry
            if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
                // No PIE
                return;
            }
        }
    }
    SetWMPID ((data >> 24) & RegData (0x000000ffU));
    SetRMPID ((data >> 16) & RegData (0x000000ffU));
    SetWG ((data >> 15) & RegData (0x00000001U));
    SetRG ((data >> 14) & RegData (0x00000001U));
    SetSX ((data >> 5) & RegData (0x00000001U));
    SetSW ((data >> 4) & RegData (0x00000001U));
    SetSR ((data >> 3) & RegData (0x00000001U));
    SetUX ((data >> 2) & RegData (0x00000001U));
    SetUW ((data >> 1) & RegData (0x00000001U));
    SetUR ((data >> 0) & RegData (0x00000001U));

    // Because update permitted SPID list in SetE(), call SetE() lastly.
    SetE ((data >> 7) & RegData (0x00000001U));
}


RegData SRegMPAT_N::Read (PrivLevel priv, ContextId ctxt) const {
    if (IsPrivileged (priv) == false) return 0;
    if (m_pe->GetNumMpuRegion () <= m_id) return 0;

    RegData ret = 0x0
        | (GetWMPID () << 24)
        | (GetRMPID () << 16)
        | (GetWG () << 15)
        | (GetRG () << 14)
        | (GetE () << 7)
        | (GetSX () << 5)
        | (GetSW () << 4)
        | (GetSR () << 3)
        | (GetUX () << 2)
        | (GetUW () << 1)
        | (GetUR () << 0);
    return ret;
}


void SRegMPID::SetSPID (RegData spid)
{
    m_spid = spid;

    //! Update MPU entry (permitted spid list)
    m_pe->ReallocMpuRegion ();
}


void SRegMPID::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    // SVLOCK : SV/HV mode can't update the resource
    if (m_pe->GetHtPtr()->m_srf->SrVSVLOCK ()->GetSVL () == 1) {
        return;
    }

    SetSPID ((data >> 0) & RegData (0x0000001fU));
}


RegData SRegICCFG::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;
    if (m_pe->GetIcPtr()->IsIcacheSizeZero ()) return 0;

    return m_pe->GetIcPtr()->ReadIccfgData ();
}


void SRegICCTRL::SetICHCLR (bool ichclr)
{
    if (ichclr == 1) {
        m_pe->GetIcPtr()->Invalidate();
    }
}


void SRegICCTRL::SetICHEN (bool ichen)
{
    if (m_pe->GetIcPtr()->IsIcacheSizeZero ()) {
        ichen = false;
    }
    m_ichen = ichen;
}


void SRegICCTRL::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    if (IsPrivileged (priv) == false) return;
    // In G3KH, there is no Icache.
    if (m_pe->GetIcPtr()->IsIcacheSizeZero ()) return;
    SetICHCLR ((data >> 8) & RegData (0x00000001U));
    SetICHEIV ((data >> 2) & RegData (0x00000001U));
    SetICHEMK ((data >> 1) & RegData (0x00000001U));
    SetICHEN ((data >> 0) & RegData (0x00000001U));
}


RegData SRegICCTRL::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;
    // In G3KH, there is no Icache.
    if (m_pe->GetIcPtr()->IsIcacheSizeZero ()) return 0;
    RegData ret = 0x0
        | (GetPBS () << 16)
        | (GetICHCLR () << 8)
        | (GetICHEIV () << 2)
        | (GetICHEMK () << 1)
        | (GetICHEN () << 0);
    return ret;
}


void SRegICCTRL::Reset (void)
{
    // D1EIV,PBSは G3M HW仕様で追加定義
    m_ichclr = 0x0U;
    m_icheiv = 0x0U;

    switch (m_pe->GetCoreType()) {
    case CORE_G4MH:
        m_pbs = 0x1U;
        m_ichemk = 0x1U;
        SetICHEN (m_pe->GetExtPin (EXT_PIN_ICHEN) & 0x1);
        break;
    case CORE_G4KH:
         m_pbs = 0x0U;
         m_ichemk = 0x1U;
         SetICHEN (m_pe->GetExtPin (EXT_PIN_ICHEN) & 0x1);
         break;
    default:
        m_pbs = 0x0U;
        m_ichemk = 0x0U;
        SetICHEN (m_pe->GetExtPin (EXT_PIN_ICHEN) & 0x1);
        break;
    }
}


void SRegICTAGL::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            return;
        }
    } else if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) return; // Needs SV prin in HVE=0
    if (m_pe->GetIcPtr()->IsIcacheSizeZero ()) return;

    uint32_t ppl_shift = m_pe->GetIcPtr()->m_tag_shift;
    uint32_t ppl_mask = (1 << ((Icache::TAG_UPPER_POS + 1) - ppl_shift)) - 1;
    SetLPN ((data >> ppl_shift) & RegData (ppl_mask));
    SetLRU  ((data >> 4) & RegData (0x00000003U));
    SetL    ((data >> 2) & RegData (0x00000001U));
    SetV    ((data >> 0) & RegData (0x00000001U));
}


RegData SRegICTAGL::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            return 0;
        }
    } else if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) return 0; // Needs SV prin in HVE=0
    if (m_pe->GetIcPtr()->IsIcacheSizeZero ()) return 0;

    uint32_t ppl_shift = m_pe->GetIcPtr()->m_tag_shift;
    RegData ret = (GetLPN () << ppl_shift)
                | (GetLRU  () << 4)
                | (GetL    () << 2)
                | (GetV    () << 0);
    return ret;
}


void SRegICTAGH::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            return;
        }
    } else if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) return; // Needs SV prin in HVE=0
    if (m_pe->GetIcPtr()->IsIcacheSizeZero ()) return;

    SetWD ((data >> 31) & RegData (0x00000001U));
    SetPD ((data >> 30) & RegData (0x00000001U));
    SetWT ((data >> 29) & RegData (0x00000001U));
    SetPT ((data >> 28) & RegData (0x00000001U));
    SetDATAECC ((data >> 16) & RegData (0x000000ffU));
    SetTAGECC ((data >> 8) & RegData (0x0000007fU)); // bit15 is fixed to 0
}


RegData SRegICTAGH::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            return 0;
        }
    } else if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) return 0; // Needs SV prin in HVE=0
    if (m_pe->GetIcPtr()->IsIcacheSizeZero ()) return 0;

    RegData ret = 0x0
        | (GetWD () << 31)
        | (GetPD () << 30)
        | (GetWT () << 29)
        | (GetPT () << 28)
        | (GetDATAECC () << 16)
        | (GetTAGECC () << 8);
    return ret;
}


void SRegICDATH::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            return;
        }
    } else if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) return; // Needs SV prin in HVE=0

    if (m_pe->GetIcPtr()->IsIcacheSizeZero ()) return;

    SetDATAH ((data >> 0) & RegData (0xffffffffU));
}


RegData SRegICDATH::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            return 0;
        }
    } else if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) return 0; // Needs SV prin in HVE=0
    if (m_pe->GetIcPtr()->IsIcacheSizeZero ()) return 0;

    RegData ret = 0x0
        | (GetDATAH () << 0);
    return ret;
}


void SRegICDATL::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            return;
        }
    } else if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) return; // Needs SV prin in HVE=0
    if (m_pe->GetIcPtr()->IsIcacheSizeZero ()) return;

    SetDATAL ((data >> 0) & RegData (0xffffffffU));
}


RegData SRegICDATL::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            return 0;
        }
    } else if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) return 0; // Needs SV prin in HVE=0
    if (m_pe->GetIcPtr()->IsIcacheSizeZero ()) return 0;

    RegData ret = 0x0
        | (GetDATAL () << 0);
    return ret;
}


void SRegICERR::SetICHEIX (RegData icheix)
{
    // G4MH ISA spec has only 8KB and 16KB.
    //  8KB : Bit 10-5 is valid. Bit 12-11 are fixed to 0. (6bit=64entry)
    // 16KB : Bit 11-5 is valid. Bit 12 is fixed to 0. (7bit=128entry)
    m_icheix = icheix & m_pe->GetIcPtr()->m_entry_mask;
}


void SRegICERR::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged (priv) == false) return;
    if (m_pe->GetIcPtr()->IsIcacheSizeZero ()) return;

    if (m_pe->GetCoreType() != CORE_G4KH) {
        SetCISTW ((data >> 31) & RegData (0x00000001U));
        SetESPBSE ((data >> 28) & RegData (0x00000001U));
        SetERMPBSE ((data >> 20) & RegData (0x00000001U));

    }
    SetESAFE ((data >> 30) & RegData (0x00000001U));
    SetESMH ((data >> 29) & RegData (0x00000001U));
    SetESTE1 ((data >> 27) & RegData (0x00000001U));
    SetESTE2 ((data >> 26) & RegData (0x00000001U));
    SetESDC ((data >> 25) & RegData (0x00000001U));
    SetESDE ((data >> 24) & RegData (0x00000001U));
    SetERMAFE ((data >> 22) & RegData (0x00000001U));
    SetERMMH ((data >> 21) & RegData (0x00000001U));
    SetERMTE1 ((data >> 19) & RegData (0x00000001U));
    SetERMTE2 ((data >> 18) & RegData (0x00000001U));
    SetERMDC ((data >> 17) & RegData (0x00000001U));
    SetERMDE ((data >> 16) & RegData (0x00000001U));
    if (m_pe->GetCoreType() == CORE_G4KH) {
        SetICHEWY ((data >> 13) & RegData (0x00000001U));
    } else {
        SetICHEWY ((data >> 13) & RegData (0x00000003U));
    }
    SetICHEIX ((data >> 5) & RegData (0x000000ffU));
    SetICHERQ ((data >> 4) & RegData (0x00000001U));
    SetICHED ((data >> 3) & RegData (0x00000001U));
    SetICHET ((data >> 2) & RegData (0x00000001U));
    SetICHERR ((data >> 0) & RegData (0x00000001U));
}


RegData SRegICERR::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;
    if (m_pe->GetIcPtr()->IsIcacheSizeZero ()) return 0;

    RegData ret = 0x0
        | (GetCISTW () << 31)
        | (GetESAFE () << 30)
        | (GetESMH () << 29)
        | (GetESPBSE () << 28)
        | (GetESTE1 () << 27)
        | (GetESTE2 () << 26)
        | (GetESDC () << 25)
        | (GetESDE () << 24)
        | (GetERMAFE () << 22)
        | (GetERMMH () << 21)
        | (GetERMPBSE () << 20)
        | (GetERMTE1 () << 19)
        | (GetERMTE2 () << 18)
        | (GetERMDC () << 17)
        | (GetERMDE () << 16)
        | (GetICHEWY () << 13)
        | (GetICHEIX () << 5)
        | (GetICHERQ () << 4)
        | (GetICHED () << 3)
        | (GetICHET () << 2)
        | (GetICHERR () << 0);
    return ret;
}


void SRegGMPLMR::Reset (void)
{
    // Initial value of PLM is 0xFF value in ISA spec.
    SetPLM(0xFF);
}


void SRegHMPLMR::SetPLM (RegData plm)
{
    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){
        // Valid only PLM[4:0] in G4MH1.x
        plm &= 0x1FU;
    }

    //All of bit fields related to the interruption priority are changed to 6bit from 8bit.
    // Upper 2bits are fixed to 0.
    m_plm = plm & 0x3F;
    m_pe->GetHtPtr()->UpdateIntStatus ();
}


void SRegGMPLMR::SetPLM (RegData plm)
{
    //All of bit fields related to the interruption priority are changed to 6bit from 8bit.
    // Upper 2bits are fixed to 0.
    m_plm = plm & 0x3F;
    m_pe->GetHtPtr()->UpdateIntStatus ();
}


void SRegFPIPR::SetFPIPR (RegData fpipr)
{
    if (m_pe->GetCoreType() == CORE_G4MH) {
        // In G4MH, FPIPR bit is disabled
        return;
    }

    //Update in G4KH
    if (fpipr > 16) { CfMsg::DPrint (MSG_ERR, "<Error: illegal FPIPR.FPIPR(%d) value. FPI priority must be in [0-16] >\n", fpipr); }
    m_fpipr = fpipr;
    m_pe->GetHtPtr()->UpdateIntStatus ();
}


void SRegISPR::SetISP (RegData isp)
{
    m_isp = isp;
    m_pe->GetHtPtr()->UpdateIntStatus ();
    m_pe->GetHtPtr()->SetPmcUpdateFlag();
}


void SRegISPR::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged (priv) == false) return;

    if (m_pe->GetSrfPtr(m_sr_ctid)->SrVINTCFG()->GetISPC() == 1) {
        SetISP ((data >> 0) & RegData (0x0000ffffU));
    }
}


bool SRegIMSR::GetHFNP (void) const
{
    if((m_pe->IsHvEnabled () == false) || (m_pe->GetHtPtr()->m_nc_srf->SrPSWH ()->GetGM() == 0)) {
        return 0;
    }
    return m_hfnp;
}


bool SRegIMSR::GetHENP (void) const
{
    if((m_pe->IsHvEnabled () == false) || (m_pe->GetHtPtr()->m_nc_srf->SrPSWH ()->GetGM() == 0)) {
        return 0;
    }
    return m_henp;
}


bool SRegIMSR::GetHEID (void) const
{
    if((m_pe->IsHvEnabled () == false) || (m_pe->GetHtPtr()->m_nc_srf->SrPSWH ()->GetGM() == 0)) {
        return 0;
    }
    return m_heid;
}


bool SRegIMSR::GetHEPLM (void) const
{
    if((m_pe->IsHvEnabled () == false) || (m_pe->GetHtPtr()->m_nc_srf->SrPSWH ()->GetGM() == 0)) {
        return 0;
    }
    return m_heplm;
}


bool SRegIMSR::GetHEEIM (void) const
{
    if((m_pe->IsHvEnabled () == false) || (m_pe->GetHtPtr()->m_nc_srf->SrPSWH ()->GetGM() == 0)) {
        return 0;
    }
    return m_heeim;
}


void SRegIMSR::SetHENP (bool henp)
{
    if(henp == 1) {
        SetHEID(0);
        SetHEPLM(0);
        SetHEEIM(0);
    }
    m_henp = henp;
}


void SRegIMSR::SetHEID (bool heid)
{
    if(heid == 1) {
        SetHEPLM(0);
        SetHEEIM(0);
    }
    m_heid = heid;
}


void SRegIMSR::SetHEPLM (bool heplm)
{
    if(heplm == 1) {
        SetHEEIM(0);
    }
    m_heplm = heplm;
}


void SRegIMSR::SetENP (bool enp)
{
    if(enp == 1) {
        SetEID(0);
        SetEPLM(0);
        SetEEIM(0);
    }
    m_enp = enp;
}


void SRegIMSR::SetEID (bool eid)
{
    if(eid == 1) {
        SetEPLM(0);
        SetEEIM(0);
    }
    m_eid = eid;
}


void SRegIMSR::SetEPLM (bool eplm)
{
    if(eplm == 1) {
        SetEEIM(0);
    }
    m_eplm = eplm;
}


void SRegHMINTCFG::SetULNR (RegData ulnr)
{
    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){
        // there is no the bit in G4MH1.x
        return;
    }
    //All of bit fields related to the interruption priority are changed to 6bit from 8bit.
    // Upper 2bits are fixed to 0.
    m_ulnr = ulnr & 0x3F;
}


void SRegHMINTCFG::SetEPL (bool epl)
{
    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){
        // there is no the bit in G4MH1.x
        return;
    }
    m_epl = epl;

    if (epl == 0) {
        // When EPL=0, xxPSW.EIMASK is set to 0.
        m_pe->GetNcSrfPtr()->SrHMPSW()->SetEIMASK(0);
        m_pe->GetNcSrfPtr()->SrHMEIPSW()->SetEIMASK(0);
        m_pe->GetNcSrfPtr()->SrHMFEPSW()->SetEIMASK(0);
        m_pe->GetNcSrfPtr()->SrDBPSW()->SetEIMASK(0);
    } else {
        // When EPL=1, INTCFG.ISPC is set to 0.
        m_ispc = 0;
    }
    m_pe->GetHtPtr()->UpdateIntStatus ();
    m_pe->GetHtPtr()->SetPmcUpdateFlag();
}


void SRegHMINTCFG::SetISPC (bool ispc)
{
    if (m_epl == 0) {
        m_ispc = ispc;
    }
}


void SRegHMINTCFG::Reset (void)
{
    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){
        // there is no the bit in G4MH1.x
        m_ulnr = 0x0U;
        m_epl = 0x0U;
    } else {
        m_ulnr = 0x0FU;
        m_epl = 0x0U;
    }
    m_ispc = 0x0U;
}


void SRegHMMEI::SetDS (RegData ds) {
    if (m_pe->GetCoreType() == CORE_G4KH) {
        ds &= 0x3; // There is no Quad-Word access in G4KH
    }
    m_ds = ds;
}


void SRegDIR0::SetDN  (bool dn) {
    m_dn = dn;
    m_pe->UpdatePrivLevel ();
    m_pe->GetHtPtr()->UpdateIntAcceptable ();
    m_pe->GetHtPtr()->UpdateFeintAcceptable ();
    m_pe->GetHtPtr()->UpdateIntStatus ();
}

void SRegDIR0::SetDM (bool dm)
{
    m_dm = dm;
    m_pe->UpdateSideband ();
    m_pe->UpdatePrivLevel ();
    m_pe->GetHtPtr()->UpdateDir0DmStatus ();
    m_pe->GetHtPtr()->UpdateHaltState ();
    m_pe->GetHtPtr()->UpdateIntAcceptable ();
    m_pe->GetHtPtr()->UpdateFeintAcceptable ();
    m_pe->GetHtPtr()->UpdateGmFeintAcceptable ();
    m_pe->GetHtPtr()->UpdateBgFeintAcceptable ();
    m_pe->GetHtPtr()->UpdateOperandSyserrAcceptable ();
    m_pe->GetHtPtr()->UpdateIntStatus ();
    m_pe->GetHtPtr()->SetPmcUpdateFlag();
}


void SRegDIR0::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged(priv) == false) return;
    SetAEE ((data >> 9) & RegData (0x00000001U));
    SetAT ((data >> 8) & RegData (0x00000001U));
}


RegData SRegDIR0::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;
    RegData ret = 0x0
        | (GetAEE () << 9)
        | (GetAT () << 8)
        | (GetDN () << 2)
        | (GetDM () << 0);
    return ret;
}


void SRegDIR1::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged(priv) == false) return;
    SetBT ((data >> 8) & RegData (0x00000fffU));
    SetCSL ((data >> 4) & RegData (0x0000000fU));
    // Sequential break is supported in G4MH1.x only
    if ((m_pe->GetCoreType() == CORE_G4MH) && (m_pe->GetCoreVersion() < CORE_VER_20)){
        SetSQ1 ((data >> 2) & RegData (0x00000001U));
        SetSQ0 ((data >> 1) & RegData (0x00000001U));
    }
    SetBEN ((data >> 0) & RegData (0x00000001U));
}


RegData SRegDIR1::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;
    RegData ret = 0x0
        | (GetBT () << 8)
        | (GetCSL () << 4)
        | (GetBEN () << 0);

    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){
        ret |= (GetSQ1 () << 2)
            |  (GetSQ0 () << 1);
    }
    return ret;
}


void SRegSNZCFG::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) { // Needs HV priv.
            return;
        }
    } else if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) return; // Needs SV prin in HVE=0

    SetSNZCNT ((data >> 0) & RegData (0x000000ffU));
    m_pe->GetHtPtr()->SetSnoozePeriod (GetSNZCNT () & 0xFFFFU);
}


RegData SRegSNZCFG::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) { // Needs HV priv.
            return 0;
        }
    } else if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) return 0; // Needs SV prin in HVE=0

    RegData ret = 0x0
                | (GetSNZCNT () << 0);
    return ret;
}


void SRegSNZCFG::Reset (void)
{
    // Read EXTPIN value
    SetSNZCNT (m_pe->GetSnoozePeriod () & 0xFF);
    if (m_pe->GetHtsPtr ()->size () > static_cast<uint32_t>(m_sr_ctid + 1)) {
        m_pe->GetHtPtr ()->SetSnoozePeriod (GetSNZCNT () & 0xFFFFU);
    }
}


void SRegDCBKEY::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    uint32_t cur_cbakey = GetCBAKEY ();
    uint32_t new_cbakey = (data >> 28) & 0xFU;

    if ((cur_cbakey == 0x0) && (data == 0xC0000000U) && (GetKEYCODE () == 0x00000000U)) {
        // Now init state (CLOSE)
        // Goto KEYOPEN1
        SetCBAKEY (0xC);
    } else if ((cur_cbakey == 0xC) && (data == 0xD0000000U)) {
        // Now KEYOPEN1
        // Goto KEYOPEN2
        SetCBAKEY (0xD);
    } else if ((cur_cbakey == 0xD) && (new_cbakey == 0x0)) {
        // Now KEYOPEN2
        uint32_t new_keycode = data & 0x00FFFFFFU;
        switch (new_keycode) {
        case 0x004076A7:
        case 0x0073A43E:
        case 0x00434954:
        case 0x00ED2922:
        case 0x0003B635:
        case 0x0063CEBE:
        case 0x009C9ABA:
            // Goto KEYOPEN
            SetCBAKEY (0x0);
            SetKEYCODE (new_keycode);
            break;
        default:
            // Wrong KEYCODE
            // Goto CLOSE
            SetCBAKEY (0x0);
            SetKEYCODE (0x00000000U); // close
        }
    } else {
        // Goto CLOSE
        SetCBAKEY (0x0);
        SetKEYCODE (0x00000000U); // close
    }
}


RegData SRegDCBKEY::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;
    return 0; // read value is 0 always
}


void SRegLSUCR::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x004076A7U)) {
        SetBBOPEN ((data >> 0) & RegData (0x00000001U));
    }
}


RegData SRegLSUCR::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret;
    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x004076A7U)) {
        ret = 0x0
            | (GetBBOPEN () << 0);
    } else {
        ret = 0;
    }
    return ret;
}


void SRegLSULNK0::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;
}


RegData SRegLSULNK0::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret;
    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x0073A43EU)) {
        ret = 0x0
            | (GetSIZE () << 4)
            | (GetV ()    << 0);
    } else {
        ret = 0;
    }
    return ret;
}


void SRegLSULNK1::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;
}


RegData SRegLSULNK1::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret;
    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x0073A43EU)) {
        ret = 0x0
            | (GetADDR ()    << 0);
    } else {
        ret = 0;
    }
    return ret;
}


void SRegL1RLCR::SetCLR (bool clr)
{
    // Read value is 0 always
    // @@ TODO : LNK clear isn't supported.
}


void SRegL1RLCR::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x00434954U)) {
        if (m_pe->GetCoreVersion() == CORE_VER_10){
            // G4MH1.0 and G4KH1.0 are depended on ISA spec 1.40
            // ASEL bit is 1bit on ISA spec 1.40.
            SetASEL ((data >> 16) & RegData (0x00000001U));
        } else {
            // G4MH1.1 and later is depends on ISA spec 1.50 or later.
            // ASEL bit is expanded to 3bit from 1bit.
            uint32_t asel = (data >> 16) & RegData (0x00000007U);
            uint32_t num_l1rlink = m_pe->GetExtPin (EXT_PIN_NUM_L1RLINK);
            if (asel >= num_l1rlink) {
                CfMsg::DPrint (MSG_WAR, "<Warning: L1RLCR.ASEL is larger than num of L1RAM link resource: %u>\n", asel);
            }
            SetASEL (asel);
        }
    }
    SetCLR    ((data >> 0) & RegData (0x00000001U));
}


RegData SRegL1RLCR::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret;
    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x00434954U)) {
        ret = 0x0
            | (GetASEL () << 16)
            | (GetCLR ()  << 0);
    } else {
        ret = 0x0
            | (GetCLR ()  << 0);
    }
    return ret;
}


void SRegL1RLNK0::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;
}


RegData SRegL1RLNK0::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret;
    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x00434954U)) {
        ret = 0x0
            | (GetTID ()  << 16)
            | (GetBMID () << 8)
            | (GetSIZE () << 4)
            | (GetV ()    << 0);
    } else {
        ret = 0;
    }
    return ret;
}


void SRegL1RLNK1::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;
}


RegData SRegL1RLNK1::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret;
    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x00434954U)) {
        ret = 0x0
            | (GetADDR () << 0);
    } else {
        ret = 0;
    }
    return ret;
}


void SRegL1RCFG::Write(RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged(priv) == false) return;
}


RegData SRegL1RCFG::GetL1RSIZE(void) const
{
    // This pin value will returns 0x20 or 0x40. We don't need to shift left 10 for it.
    return (m_pe->GetExtPin (EXT_PIN_SIZE_L1R) & 0x3FFFFF);
}


RegData SRegL1RCFG::GetL1RLNKE(void) const
{
    // L1RLNKE is in [2,8] entries.
    return ((m_pe->GetExtPin (EXT_PIN_NUM_L1RLINK)) & 0x0F);
}


RegData SRegL1RCFG::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged(priv) == false) return 0;
    RegData ret = 0x0
          | (GetL1RSIZE () << 8)
          | (GetL1RLNKE () << 0);
    return ret;
}


void SRegLSCFG::Reset (void)
{
    m_lse = m_pe->GetExtPin (EXT_PIN_LSCFG_LSE) & 0x1; //! External pin
}


void SRegDECFG::Write(RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) { // Needs HV priv.
            return;
        }
    } else if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) return; // Needs SV priv in HVE=0.

    SetEHE ((data >> 2) & RegData (0x00000001U));
    SetEGE ((data >> 1) & RegData (0x00000001U));
    SetESE ((data >> 0) & RegData (0x00000001U));
}


RegData SRegDECFG::Read(PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) { // Needs HV priv.
            return 0;
        }
    } else if (HasPrivileged (PRIV_PERMIT_SV, priv) == false) return 0; // Needs SV priv in HVE=0.

    RegData ret = 0x0
          | (GetEHE () << 2)
          | (GetEGE () << 1)
          | (GetESE () << 0);
   return ret;
}


void SRegDECTRL::SetBEVD(bool bevd)
{
    if (bevd == 0) {
        if (m_pe->IsHvEnabled ()) {
            if (m_pe->IsHostMode ()) {
                m_pe->GetHtPtr()->m_lsu->SetSyserrCause (true, 0, SYSERR_NONE);
            } else {
                m_pe->GetHtPtr()->m_lsu->SetSyserrCause (false, m_pe->GetHtPtr()->m_nc_srf->SrPSWH ()->GetGPID(), SYSERR_NONE);
            }
        } else {
            // In case conventional mode.
            m_pe->GetHtPtr()->m_lsu->SetSyserrCause (true, 0, SYSERR_NONE);
        }
    }
}


void SRegDECTRL::Write(RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged(priv) == false) return;
    SetBEVD ((data >> 0) & RegData (0x00000001U));
    m_pe->GetHtPtr()->UpdateOperandSyserrEvent();
}


bool SRegDECTRL::GetBEVD(void) const
{
    if (m_pe->IsHvEnabled ()) {
        if (m_pe->IsHostMode ()) {
            return (m_pe->GetHtPtr()->m_lsu->GetSyserrCause (true, 0) != SYSERR_NONE);
        } else {
            return (m_pe->GetHtPtr()->m_lsu->GetSyserrCause (false, m_pe->GetHtPtr()->m_nc_srf->SrPSWH ()->GetGPID()) != SYSERR_NONE);
        }
    } else {
        // In case conventional mode.
        return (m_pe->GetHtPtr()->m_lsu->GetSyserrCause (true, 0) != SYSERR_NONE);
    }
}


RegData SRegDECTRL::Read(PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged(priv) == false) return 0;
    RegData ret = 0x0
          | (GetBEVD () << 0);
   return ret;
}


void SRegDEVDS::SetBEVDHM(bool bevdhm)
{
    if (bevdhm == 0) {
        m_pe->GetHtPtr()->m_lsu->SetSyserrCause (true, 0, SYSERR_NONE);
    }
}


void SRegDEVDS::SetBEVDGM7(bool bevdgm7)
{
    if (bevdgm7 == 0) {
        m_pe->GetHtPtr()->m_lsu->SetSyserrCause (false, 7, SYSERR_NONE);
    }
}


void SRegDEVDS::SetBEVDGM6(bool bevdgm6)
{
    if (bevdgm6 == 0) {
        m_pe->GetHtPtr()->m_lsu->SetSyserrCause (false, 6, SYSERR_NONE);
    }
}


void SRegDEVDS::SetBEVDGM5(bool bevdgm5)
{
    if (bevdgm5 == 0) {
        m_pe->GetHtPtr()->m_lsu->SetSyserrCause (false, 5, SYSERR_NONE);
    }
}


void SRegDEVDS::SetBEVDGM4(bool bevdgm4)
{
    if (bevdgm4 == 0) {
        m_pe->GetHtPtr()->m_lsu->SetSyserrCause (false, 4, SYSERR_NONE);
    }
}


void SRegDEVDS::SetBEVDGM3(bool bevdgm3)
{
    if (bevdgm3 == 0) {
        m_pe->GetHtPtr()->m_lsu->SetSyserrCause (false, 3, SYSERR_NONE);
    }
}


void SRegDEVDS::SetBEVDGM2(bool bevdgm2)
{
    if (bevdgm2 == 0) {
        m_pe->GetHtPtr()->m_lsu->SetSyserrCause (false, 2, SYSERR_NONE);
    }
}


void SRegDEVDS::SetBEVDGM1(bool bevdgm1)
{
    if (bevdgm1 == 0) {
        m_pe->GetHtPtr()->m_lsu->SetSyserrCause (false, 1, SYSERR_NONE);
    }
}


void SRegDEVDS::SetBEVDGM0(bool bevdgm0)
{
    if (bevdgm0 == 0) {
        m_pe->GetHtPtr()->m_lsu->SetSyserrCause (false, 0, SYSERR_NONE);
    }
}


void SRegDEVDS::Write(RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged(priv) == false) return;
    SetBEVDHM ((data >> 16) & RegData (0x00000001U));
    SetBEVDGM7 ((data >> 7) & RegData (0x00000001U));
    SetBEVDGM6 ((data >> 6) & RegData (0x00000001U));
    SetBEVDGM5 ((data >> 5) & RegData (0x00000001U));
    SetBEVDGM4 ((data >> 4) & RegData (0x00000001U));
    SetBEVDGM3 ((data >> 3) & RegData (0x00000001U));
    SetBEVDGM2 ((data >> 2) & RegData (0x00000001U));
    SetBEVDGM1 ((data >> 1) & RegData (0x00000001U));
    SetBEVDGM0 ((data >> 0) & RegData (0x00000001U));
}


bool SRegDEVDS::GetBEVDHM(void) const
{
    return (m_pe->GetHtPtr()->m_lsu->GetSyserrCause (true, 0) != SYSERR_NONE);
}


bool SRegDEVDS::GetBEVDGM7(void) const
{
    return (m_pe->GetHtPtr()->m_lsu->GetSyserrCause (false, 7) != SYSERR_NONE);
}


bool SRegDEVDS::GetBEVDGM6(void) const
{
    return (m_pe->GetHtPtr()->m_lsu->GetSyserrCause (false, 6) != SYSERR_NONE);
}


bool SRegDEVDS::GetBEVDGM5(void) const
{
    return (m_pe->GetHtPtr()->m_lsu->GetSyserrCause (false, 5) != SYSERR_NONE);
}


bool SRegDEVDS::GetBEVDGM4(void) const
{
    return (m_pe->GetHtPtr()->m_lsu->GetSyserrCause (false, 4) != SYSERR_NONE);
}


bool SRegDEVDS::GetBEVDGM3(void) const
{
    return (m_pe->GetHtPtr()->m_lsu->GetSyserrCause (false, 3) != SYSERR_NONE);
}


bool SRegDEVDS::GetBEVDGM2(void) const
{
    return (m_pe->GetHtPtr()->m_lsu->GetSyserrCause (false, 2) != SYSERR_NONE);
}


bool SRegDEVDS::GetBEVDGM1(void) const
{
    return (m_pe->GetHtPtr()->m_lsu->GetSyserrCause (false, 1) != SYSERR_NONE);
}


bool SRegDEVDS::GetBEVDGM0(void) const
{
    return (m_pe->GetHtPtr()->m_lsu->GetSyserrCause (false, 0) != SYSERR_NONE);
}


RegData SRegDEVDS::Read(PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged(priv) == false) return 0;
    RegData ret = 0x0
          | (GetBEVDHM () << 16)
          | (GetBEVDGM7 () << 7)
          | (GetBEVDGM6 () << 6)
          | (GetBEVDGM5 () << 5)
          | (GetBEVDGM4 () << 4)
          | (GetBEVDGM3 () << 3)
          | (GetBEVDGM2 () << 2)
          | (GetBEVDGM1 () << 1)
          | (GetBEVDGM0 () << 0);
   return ret;
}


void SRegICBKEY::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    uint32_t cur_cbakey = GetCBAKEY ();
    uint32_t new_cbakey = (data >> 28) & 0xFU;

    if ((cur_cbakey == 0x0) && (data == 0xC0000000U) && (GetKEYCODE () == 0x00000000U)) {
        // Now init state (CLOSE)
        // Goto KEYOPEN1
        SetCBAKEY (0xC);
    } else if ((cur_cbakey == 0xC) && (data == 0xD0000000U)) {
        // Now KEYOPEN1
        // Goto KEYOPEN2
        SetCBAKEY (0xD);
    } else if ((cur_cbakey == 0xD) && (new_cbakey == 0x0)) {
        // Now KEYOPEN2
        uint32_t new_keycode = data & 0x00FFFFFFU;
        switch (new_keycode) {
        case 0x00263F2D:
        case 0x00E9D641:
        case 0x00B6B422:
        case 0x007F6EAA:
            // Goto KEYOPEN
            SetCBAKEY (0x0);
            SetKEYCODE (new_keycode);
            break;
        default:
            // Wrong KEYCODE
            // Goto CLOSE
            SetCBAKEY (0x0);
            SetKEYCODE (0x00000000U); // close
        }
    } else {
        // Goto CLOSE
        SetCBAKEY (0x0);
        SetKEYCODE (0x00000000U); // close
    }
}


RegData SRegICBKEY::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;
    return 0; // read alue is 0 always
}


void SRegIFCR::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    SetPLEN ((data >> 0) & RegData (0x00000001U));
}


RegData SRegIFCR::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret = 0x0
        | (GetPLEN () << 0);
    return ret;
}


void SRegIFCR1::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrICBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrICBKEY ()->GetKEYCODE ();
    if (cbakey == 0x0) {
        if (keycode == 0x00263F2DU) {
            SetPLINTD ((data >> 6) & RegData (0x00000001U));
            SetPLNLND ((data >> 5) & RegData (0x00000001U));
        } else if (keycode == 0x00E9D641U) {
            SetFD2    ((data >> 8) & RegData (0x00000001U));
        }
    }
}


RegData SRegIFCR1::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret;
    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrICBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrICBKEY ()->GetKEYCODE ();
    if (cbakey == 0x0) {
        if (keycode == 0x00263F2DU) {
            ret = 0x0
                | (GetPLINTD () << 6)
                | (GetPLNLND () << 5);
        } else if (keycode == 0x00E9D641U) {
            ret = 0x0
                | (GetFD2    () << 8);
        } else {
            ret = 0x0;
        }
    } else {
        ret = 0x0;
    }
    return ret;
}


void SRegBRPCTRL0::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    SetBRPEN ((data >> 0) & RegData (0x00000001U));
}


RegData SRegBRPCTRL0::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret = 0x0
        | (GetBRPCLR () << 1)
        | (GetBRPEN () << 0);
    return ret;
}


void SRegBRPCTRL1::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrICBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrICBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x00B6B422U)) {
        SetBHTST ((data >> 4) & RegData (0x00000001U));
        SetRASD ((data >> 3) & RegData (0x00000001U));
        SetBTACD ((data >> 2) & RegData (0x00000001U));
        SetGBHTD ((data >> 1) & RegData (0x00000001U));
        SetBHTD ((data >> 0) & RegData (0x00000001U));
    }
}


RegData SRegBRPCTRL1::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret;
    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrICBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrICBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x00B6B422U)) {
        ret = 0x0
            | (GetBHTST () << 4)
            | (GetRASD () << 3)
            | (GetBTACD () << 2)
            | (GetGBHTD () << 1)
            | (GetBHTD () << 0);
    } else {
        ret = 0;
    }
    return ret;
}


void SRegBRPCFG::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;
}


RegData SRegBRPCFG::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret;
    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrICBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrICBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x007F6EAAU)) {
        ret = 0x0
            | (GetRASSIZE () << 12)
            | (GetBTACSIZE () << 8)
            | (GetGBHTSIZE () << 4)
            | (GetBHTSIZE () << 0);
    } else {
        ret = 0;
    }
    return ret;
}


void SRegBRPACTRL::SetSR (bool sr)
{
    // Write is available. But read value is always 0.
}


void SRegBRPACTRL::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrICBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrICBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x007F6EAAU)) {
        SetSR ((data >> 17) & RegData (0x00000001U));
        SetRW ((data >> 16) & RegData (0x00000001U));
        SetSEL ((data >> 12) & RegData (0x00000003U));
        SetENTRY ((data >> 3) & RegData (0x000001ffU));
    }
}


RegData SRegBRPACTRL::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret;
    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrICBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrICBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x007F6EAAU)) {
        ret = 0x0
            | (GetSR () << 17)
            | (GetRW () << 16)
            | (GetSEL () << 12)
            | (GetENTRY () << 3);
    } else {
        ret = 0;
    }
    return ret;
}


void SRegBRPDATA::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrICBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrICBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x007F6EAAU)) {
        SetDATA ((data >> 0) & RegData (0x0fffffffU)); // bit31-28 is fixed to 0
    }
}


RegData SRegBRPDATA::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret;
    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrICBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrICBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x007F6EAAU)) {
        ret = 0x0
            | (GetDATA () << 0);
    } else {
        ret = 0;
    }
    return ret;
}


void SRegRDBCR::SetRDBCLR (bool rdbclr)
{
    // 常に0(初期値)読み出しなので更新しない
}


void SRegRDBCR::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if (cbakey == 0x0) {
        if (keycode == 0x009C9ABAU) {
            SetRDBDBE ((data >> 17) & RegData (0x00000001U));
        } else if (keycode == 0x0063CEBEU) {
            SetTPEN ((data >> 9) & RegData (0x00000001U));
        }
    }
    SetAPEN ((data >> 8) & RegData (0x00000001U));
    SetRDBCLR ((data >> 1) & RegData (0x00000001U));
    SetRDBEN ((data >> 0) & RegData (0x00000001U));
}


RegData SRegRDBCR::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret;
    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if (cbakey == 0x0) {
        if (keycode == 0x009C9ABAU) {
            ret = 0x0
                | (GetRDBDBE () << 17)
                | (GetAPEN () << 8)
                | (GetRDBCLR () << 1)
                | (GetRDBEN () << 0);
        } else if (keycode == 0x0063CEBEU) {
            ret = 0x0
                | (GetTPEN () << 9)
                | (GetAPEN () << 8)
                | (GetRDBCLR () << 1)
                | (GetRDBEN () << 0);
        } else {
            ret = 0x0
                | (GetAPEN () << 8)
                | (GetRDBCLR () << 1)
                | (GetRDBEN () << 0);
        }
    } else {
        ret = 0x0
            | (GetAPEN () << 8)
            | (GetRDBCLR () << 1)
            | (GetRDBEN () << 0);
    }
    return ret;
}


void SRegRDBACR::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x00ED2922U)) {
        SetENTRY ((data >> 1) & RegData (0x00000003U));
        SetULSEL ((data >> 0) & RegData (0x00000001U));
    }
}


RegData SRegRDBACR::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;
    RegData ret;
    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x00ED2922U)) {
        ret = 0x0
            | (GetENTRY () << 1)
            | (GetULSEL () << 0);
    } else {
        ret = 0;
    }
    return ret;
}


void SRegRDBADAT0::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x00ED2922U)) {
        SetDATA ((data >> 0) & RegData (0xffffffffU));
    }
}


RegData SRegRDBADAT0::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret;
    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x00ED2922U)) {
        ret = 0x0
            | (GetDATA () << 0);
    } else {
        ret = 0;
    }
    return ret;
}


void SRegRDBADAT1::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x00ED2922U)) {
        SetDATA ((data >> 0) & RegData (0xffffffffU));
    }
}


RegData SRegRDBADAT1::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret;
    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x00ED2922U)) {
        ret = 0x0
            | (GetDATA () << 0);
    } else {
        ret = 0;
    }
    return ret;
}

void SRegRDBADAT2::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x00ED2922U)) {
        SetDATA ((data >> 0) & RegData (0xffffffffU));
    }
}


RegData SRegRDBADAT2::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret;
    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x00ED2922U)) {
        ret = 0x0
            | (GetDATA () << 0);
    } else {
        ret = 0;
    }
    return ret;
}


void SRegRDBADAT3::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x00ED2922U)) {
        SetDATA ((data >> 0) & RegData (0xffffffffU));
    }
}


RegData SRegRDBADAT3::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret;
    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x00ED2922U)) {
        ret = 0x0
            | (GetDATA () << 0);
    } else {
        ret = 0;
    }
    return ret;
}


void SRegRDBATAG::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x00ED2922U)) {
        SetPPN ((data >> 5) & RegData (0x007fffffU));
        SetV ((data >> 0) & RegData (0x00000001U));
    }
}


RegData SRegRDBATAG::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret;
    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x00ED2922U)) {
        ret = 0x0
            | (GetPPN () << 5)
            | (GetV () << 0);
    } else {
        ret = 0;
    }
    return ret;
}


void SRegRDBSTAT::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x0003B635U)) {
        SetLRU ((data >> 0) & RegData (0x0000003FU));
    }
}


RegData SRegRDBSTAT::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret;
    uint32_t cbakey = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetCBAKEY ();
    uint32_t keycode = m_pe->GetSrfPtr(NC)->SrDCBKEY ()->GetKEYCODE ();
    if ((cbakey == 0x0) && (keycode == 0x0003B635U)) {
        ret = 0x0
            | (GetLRU () << 0);
    } else {
        ret = 0;
    }
    return ret;
}


void SRegBRPCFG::Reset (void)
{
    SetRASSIZE (1);
    SetBTACSIZE (1);
    SetGBHTSIZE (2);
    SetBHTSIZE (2);
}


void SRegBPC::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged(priv) == false) return;
    SRegFile* srf = m_pe->GetHtPtr()->m_srf;
    uint32_t  csl = srf->SrDIR1()->GetCSL ();
    if (csl < DbgBreak::N_BPD) {
        srf->GetSrPtr(CF_SR_BPC0 + csl)->Write (data, priv, ctxt);
    } else if (csl < DbgBreak::N_BPA) {
        // Only PCB is supported in ch4-11
        // - TY[12-10]/WE[1]/RE[0]=0 (no VD[9]/MD[7] in G4MH)
        data &= 0xffffe3fc;
        srf->GetSrPtr(CF_SR_BPC0 + csl)->Write (data, priv, ctxt);
    } else {
        CfMsg::DPrint (MSG_WAR, "<Warning: invalid DIR1.CSL to access BPC: %d>\n", csl);
    }
}


RegData SRegBPC::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;
    SRegFile* srf = m_pe->GetHtPtr()->m_srf;
    uint32_t  csl = srf->SrDIR1()->GetCSL ();
    if (csl < DbgBreak::N_BPA) {
        return srf->GetSrPtr(CF_SR_BPC0 + csl)->Read (priv, ctxt);
    } else {
        CfMsg::DPrint (MSG_WAR, "<Warning: invalid DIR1.CSL to access BPC: %d>\n", csl);
        return 0;
    }
}


RegData SRegBPC_N::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;
    RegData ret = 0x0
        | (GetTY () << 10)
        | (GetVA () << 8)
        | (GetBE () << 3)
        | (GetFE () << 2)
        | (GetWE () << 1)
        | (GetRE () << 0);

    if (m_pe->IsHvEnabled ()) {
        ret |=
              (GetHE () << 24)
            | (GetGE () << 16);
    }

    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){
        ret |=
          (GetEO () << 5)
        | (GetTE () << 4);
    }
    return ret;
}


void SRegBPC_N::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged (priv) == false) return;
    if (m_pe->IsHvEnabled ()) {
        SetHE ((data >> 24) & RegData (0x00000001U));
        SetGE ((data >> 16) & RegData (0x000000ffU));
    }
    SetTY ((data >> 10) & RegData (0x00000007U));
    SetVA ((data >> 8) & RegData (0x00000001U));
    if ((m_pe->GetCoreType() == CORE_G4MH) && (m_pe->GetCoreVersion() < CORE_VER_20)){ // G4MH1.x
        SetEO ((data >> 5) & RegData (0x00000001U));
        SetTE ((data >> 4) & RegData (0x00000001U));
    } else if ((m_pe->GetCoreType() == CORE_G4MH) && (m_pe->GetCoreVersion() > CORE_VER_20)) { // G4MH2.x
        // Do nothing.
    } else { // G4KH
        // TE bit treated as read only bit
        SetEO ((data >> 5) & RegData (0x00000001U));
    }
    SetBE ((data >> 3) & RegData (0x00000001U));
    SetFE ((data >> 2) & RegData (0x00000001U));
    SetWE ((data >> 1) & RegData (0x00000001U));
    SetRE ((data >> 0) & RegData (0x00000001U));
}


void SRegBPC_N::SetTY (RegData ty)
{
    if (m_id >= DbgBreak::N_BPD) return;
    m_ty = ty;
}


void SRegBPC_N::SetWE (bool we)
{
    if (m_id >= DbgBreak::N_BPD) return;
    m_we = we;
}


void SRegBPC_N::SetRE (bool re)
{
    if (m_id >= DbgBreak::N_BPD) return;
    m_re = re;
}


void SRegBPAV::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged(priv) == false) return;
    SRegFile* srf = m_pe->GetHtPtr()->m_srf;
    uint32_t  csl = srf->SrDIR1()->GetCSL ();
    if (csl < DbgBreak::N_BPA) {
        srf->GetSrPtr(CF_SR_BPAV0 + csl)->Write (data, priv, ctxt);
    } else {
        CfMsg::DPrint (MSG_WAR, "<Warning: invalid DIR1.CSL to access BPAV: %d>\n", csl);
    }
}


RegData SRegBPAV::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;
    SRegFile* srf = m_pe->GetHtPtr()->m_srf;
    uint32_t  csl = srf->SrDIR1()->GetCSL ();
    if (csl < DbgBreak::N_BPA) {
        return srf->GetSrPtr(CF_SR_BPAV0 + csl)->Read (priv, ctxt);
    } else {
        CfMsg::DPrint (MSG_WAR, "<Warning: invalid DIR1.CSL to access BPAV: %d>\n", csl);
        return 0;
    }
}


RegData SRegBPAV_N::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;
    RegData ret = 0x0
        | (GetBPAV () << 0);
    return ret;
}


void SRegBPAM::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged(priv) == false) return;
    SRegFile* srf = m_pe->GetHtPtr()->m_srf;
    uint32_t  csl = srf->SrDIR1()->GetCSL ();
    if (csl < DbgBreak::N_BPA) {
        srf->GetSrPtr(CF_SR_BPAM0 + csl)->Write (data, priv, ctxt);
    } else {
        CfMsg::DPrint (MSG_WAR, "<Warning: invalid DIR1.CSL to access BPAM: %d>\n", csl);
    }
}


RegData SRegBPAM::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;
    SRegFile* srf = m_pe->GetHtPtr()->m_srf;
    uint32_t  csl = srf->SrDIR1()->GetCSL ();
    if (csl < DbgBreak::N_BPA) {
        return srf->GetSrPtr(CF_SR_BPAM0 + csl)->Read (priv, ctxt);
    } else {
        CfMsg::DPrint (MSG_WAR, "<Warning: invalid DIR1.CSL to access BPAM: %d>\n", csl);
        return 0;
    }
}


RegData SRegBPAM_N::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;
    RegData ret = 0x0
        | (GetBPAM () << 0);
    return ret;
}


void SRegDBIC::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged(priv) == false) return;
    SetDBIC ((data >> 0) & RegData (0xffffffffU));
}


RegData SRegDBIC::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;
    RegData ret = 0x0
        | (GetDBIC () << 0);
    return ret;
}


void SRegDBPC::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged(priv) == false) return;
    SetDBPC ((data >> 0) & RegData (0xffffffffU));
}


RegData SRegDBPC::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;
    RegData ret = 0x0
        | (GetDBPC () << 0);
    return ret;
}


RegData SRegDBCFG::GetNBP(void) const
{
    // The largest breakpoint channel can connect to CPU.
    return DbgBreak::N_BPA - 1;
}


RegData SRegDBCFG::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;
    RegData ret = 0x0
          | (GetNBP() << 0);
    return ret;
}


void SRegDBGEN::Reset (void)
{
    m_he = (m_pe->GetExtPin (EXT_PIN_DBGEN) >> 8) & 0x00000001U;
    m_ge = (m_pe->GetExtPin (EXT_PIN_DBGEN) >> 0) & 0x000000ffU;
}


void SRegDBWR::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPrivileged(priv) == false) return;
    SetDBWR ((data >> 0) & RegData (0xffffffffU));
}


RegData SRegDBWR::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPrivileged (priv) == false) return 0;
    RegData ret = 0x0
        | (GetDBWR () << 0);
    return ret;
}


void SRegLSTEST0::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    SetLSTEST0 ((data >> 0) & RegData (0xffffffffU));
}


RegData SRegLSTEST0::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret = 0x0
        | (GetLSTEST0 () << 0);
    return ret;
}


void SRegLSTEST1::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return;
        }
    } else if (IsPrivileged (priv) == false) return;

    SetLSTEST1 ((data >> 0) & RegData (0xffffffffU));
}


RegData SRegLSTEST1::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Needs HV priv.
            return 0;
        }
    } else if (IsPrivileged (priv) == false) return 0;

    RegData ret = 0x0
        | (GetLSTEST1 () << 0);
    return ret;
}


void SRegFXST::SetXC (RegData xc)
{ m_pe->GetHtPtr()->m_srf->SrFXSR()->SetXC (xc); }

void SRegFXST::SetIF_ (bool if_)
{ m_pe->GetHtPtr()->m_srf->SrFXSR()->SetIF_ (if_); }

void SRegFXST::SetXP (RegData xp)
{ m_pe->GetHtPtr()->m_srf->SrFXSR()->SetXP (xp); }

RegData SRegFXST::GetXC (void) const
{ return m_pe->GetHtPtr()->m_srf->SrFXSR()->GetXC (); }

bool SRegFXST::GetIF_ (void) const
{ return m_pe->GetHtPtr()->m_srf->SrFXSR()->GetIF_ (); }

RegData SRegFXST::GetXP (void) const
{ return m_pe->GetHtPtr()->m_srf->SrFXSR()->GetXP (); }

void SRegFXCFG::SetRM (RegData rm)
{ m_pe->GetHtPtr()->m_srf->SrFXSR()->SetRM (rm); }

void SRegFXCFG::SetXE (RegData xe)
{ m_pe->GetHtPtr()->m_srf->SrFXSR()->SetXE (xe); }

RegData SRegFXCFG::GetRM (void) const
{ return m_pe->GetHtPtr()->m_srf->SrFXSR()->GetRM (); }

RegData SRegFXCFG::GetXE (void) const
{ return m_pe->GetHtPtr()->m_srf->SrFXSR()->GetXE (); }

void SRegHVCFG::SetHVE (bool hve)
{
    m_hve = hve;
    m_pe->UpdateHvStatus ();
    if (m_pe->IsHvEnabled ()) {
        // Because before calling ChangeToHostMode/ChangeToGuestMode,
        // Need to read nc_srf for HV registers.
        if (m_pe->GetHtPtr()->m_nc_srf->SrPSWH ()->GetGM() == 0) {
            m_pe->ChangeToHostMode ();
        } else {
            m_pe->ChangeToGuestMode ();
        }
    } else {
        // Update HV registers when HVE is changed to 0
        // PSWH.GM, GPID
        // MCR.Gxx, Hxx
        m_pe->GetNcSrfPtr()->SrPSWH ()->SetGPID (0);
        m_pe->GetNcSrfPtr()->SrPSWH ()->SetGM (0);
        m_pe->ChangeToHostMode ();
    }
}


void SRegHVCFG::Reset (void)
{
    // HV spec is added G4MH2.0 and later
    if (  (m_pe->GetCoreType() == CORE_G4MH) && (m_pe->GetCoreVersion() >= CORE_VER_20)
       && (m_pe->GetExtPin (EXT_PIN_HAS_HV) == 1) ) {
        SetHVE (m_pe->GetExtPin (EXT_PIN_HVE) & 0x00000001U);
    } else {
        SetHVE (0);
    }
}


void SRegHVCFG::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    // HV spec is added G4MH2.0 and later
      if (  (m_pe->GetCoreType() == CORE_G4MH) && (m_pe->GetCoreVersion() >= CORE_VER_20)
       && (m_pe->GetExtPin (EXT_PIN_HAS_HV) == 1) ) {
        SetHVE (data & 0x1);
    }
}


RegData SRegHVCFG::Read (PrivLevel priv, ContextId ctxt) const
{
    if ((m_pe->GetCoreType() != CORE_G4MH) || (m_pe->GetCoreVersion() < CORE_VER_20)){ return 0; }

    RegData ret = 0x0
                | (GetHVE () << 0);
    return ret;
}


void SRegHVSB::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (m_pe->IsHvEnabled ()) {
        if (HasPrivileged (PRIV_PERMIT_HV, priv) == false) {
            m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE); // Write needs HV priv.
            return;
        }
        SetHVSB (data & 0xFFFFFFFFU);
    }
}


RegData SRegHVSB::Read (PrivLevel priv, ContextId ctxt) const
{
    if (m_pe->IsHvEnabled () == false) return 0;

    RegData ret = 0x0
        | (GetHVSB () << 0);
    return ret;
}


void SRegDUMMYHV::Write (RegData data, PrivLevel priv, ContextId ctxt)
{
    if (IsPie (priv)) {
        m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE);
    }
}


RegData SRegDUMMYHV::Read (PrivLevel priv, ContextId ctxt) const
{
    if (IsPie (priv)) {
        m_pe->GetHtPtr()->m_trace->SetExpCode(EXP_PIE);
    }
    return 0x0;
}
