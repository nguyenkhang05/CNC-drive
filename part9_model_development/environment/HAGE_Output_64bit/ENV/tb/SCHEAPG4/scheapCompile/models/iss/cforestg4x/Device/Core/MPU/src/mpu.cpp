/*
 * (c) 2011,2012,2013,2017 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <algorithm>
#include "./config.h"
#include "./forest_common.h"
#include "./trace_info.h"
#include "./sregfile.h"
#include "./mpu.h"
#include "./micro_arch_db.h"
#include "./forest_message.h"


/*!
 * @brief Search MPU Region
 * @param vrtl_addr Address
 * @param size mem access size
 * @param spid Spid number
 * @retval true Hit
 * @retval false Not hit
 */
template <MemAccessCommand cmd>
bool Mpu::MpuRegion::IsMpuRegionHit(PhysAddr vrtl_addr, ByteSize size, uint32_t spid) const {
  PhysAddr start_addr = vrtl_addr;
  PhysAddr end_addr = (vrtl_addr + (size - 1)) & 0xFFFFFFFFU;
  if ((m_mpla->ReadBody() <= start_addr && end_addr <= m_mpua->ReadBody() + 3) &&
      (start_addr <= end_addr)) {
    if (cmd == DATA_WRITE) {
      if (m_write_permit_spid.test(spid) == true) {
        return true;  // UT: Exit (a)
      } else {
        return false; // UT: Exit (b)
      }
    } else if ((cmd == INST_READ) || (cmd == DATA_READ)) {
      if (m_read_permit_spid.test(spid) == true) {
        return true;  // UT: Exit (c)
      } else {
        return false; // UT: Exit (d)
      }
    } else if (cmd == DATA_RMW) {
      if ((m_read_permit_spid.test(spid) == true) && (m_write_permit_spid.test(spid) == true)) {
        return true;
      } else {
        return false;
      }
    }
  }
  return false;       // UT: Exit (e)
}

void Mpu::MpuRegion::UpdatePermitSpid(void) {
  m_write_permit_spid.reset();
  m_read_permit_spid.reset();

  // Update permitted spid list for write access
  if (m_mpat->GetWG() == 1) {
    m_write_permit_spid.set(); // permit all spid
  } else {
    uint32_t wmpid = m_mpat->GetWMPID();
    for (uint32_t i = 0; i < N_MPID; ++i) {
      if (((wmpid >> i) & 0x1) == 1) {
        uint32_t spid = static_cast<SRegMPID*>(m_srf->GetSrPtr(CF_SR_MPID0 + i))->GetSPID();
        m_write_permit_spid.set(spid, 1);
      }
    }
  }

  // Update permitted spid list for fetch/read access
  if (m_mpat->GetRG() == 1) {
    m_read_permit_spid.set(); // permit all spid
  } else {
    uint32_t rmpid = m_mpat->GetRMPID();
    for (uint32_t i = 0; i < N_MPID; ++i) {
      if (((rmpid >> i) & 0x1) == 1) {
        uint32_t spid = static_cast<SRegMPID*>(m_srf->GetSrPtr(CF_SR_MPID0 + i))->GetSPID();
        m_read_permit_spid.set(spid, 1);
      }
    }
  }
}

/*!
 * @brief Check MPU region boundary alignment
 * @param vrtl_addr Address
 * @param size mem access size
 * @retval true MPU region misalignment
 * @retval false MPU region alignment
 */
bool Mpu::MpuRegion::IsMpuRegionMissAlign(PhysAddr vrtl_addr, ByteSize size) const {
  PhysAddr mpla = m_mpla->ReadBody();
  PhysAddr mpua = m_mpua->ReadBody() + 3;
  PhysAddr vrtl_addr_ua = vrtl_addr + size - 1;

  if (mpla <= vrtl_addr && vrtl_addr_ua <= mpua) {
    return false;
  } else if (mpla <= vrtl_addr && vrtl_addr <= mpua) {
    return true;
  } else if (mpla <= vrtl_addr_ua && vrtl_addr_ua <= mpua) {
    return true;
  } else {
    return false;
  }
}

/*!
 * @brief check MPU region protection attribute
 * @param um user mode or sv mode
 * @retval true Protection permitted
 * @retval false Protection not permitted
 */
template <MemAccessCommand cmd>
bool Mpu::MpuRegion::IsMpuRegionPermitted(bool um) const {
  if (cmd == INST_READ) {
    if (um == 1) return m_mpat->GetUX(); // User mode
    else         return m_mpat->GetSX(); // Super visor mode
  }
  else if (cmd == DATA_READ) {
    if (um == 1) return m_mpat->GetUR(); // User mode
    else         return m_mpat->GetSR(); // Super visor mode
  }
  else if (cmd == DATA_WRITE) {
    if (um == 1) return m_mpat->GetUW(); // User mode
    else         return m_mpat->GetSW(); // Super visor mode
  }
  return false;
}

bool Mpu::IsEnableDebugMonitor(void) const {
  // - DIR0.DM=1
  return m_srf->SrDIR0()->GetDM() == 1;
}

/*!
 * @brief Check mem access protection violation
 * @param vrtl_addr address
 * @param size mem access size
 * @retval true access granted
 * @retval false access not granted
 */
template <MemAccessCommand cmd>
bool Mpu::IsMpuPermitted(PhysAddr vrtl_addr, ByteSize size) {
#ifdef ENABLE_DBG_BREAK
  if (IsEnableDebugMonitor()) {
    return true;
  }
#endif // ENABLE_DBG_BREAK

  // access granted
  if (IsMpuPermittedDefNG<cmd>(vrtl_addr, size) == true) {
    return true; // UT: Exit (a)
  }
  // access not granted
  else {
    m_exp_cause = GetMpuExpCause(vrtl_addr, cmd, size, MDP_AT_NORMAL);
    return false; // UT: Exit (b)
  }
}

/*!
 * @brief Check mem access protection violation for table/regbank access in EIINT
 * @param vrtl_addr Address
 * @param size mem access size
 * @retval true access granted
 * @retval false access not granted
 */
template <MemAccessCommand cmd> bool Mpu::IsMpuPermittedEI(PhysAddr vrtl_addr, ByteSize size) {
#ifdef ENABLE_DBG_BREAK
  if (IsEnableDebugMonitor()) {
    return true;
  }
#endif //#ifdef ENABLE_DBG_BREAK

  // access granted
  if (IsMpuPermittedDefNG<cmd>(vrtl_addr, size) == true) {
    return true;  // UT: Exit (a)
  }
  // access not granted
  else {
    m_exp_cause = GetMpuExpCause(vrtl_addr, cmd, size, MDP_AT_EIINT);
    return false; // UT: Exit (b)
  }
}

/*!
 * @brief Check mem access protection violation when default NG
 * @param vrtl_addr address
 * @param size mem access size
 * @retval true access granted
 * @retval false access not granted
 */
template <MemAccessCommand cmd> bool Mpu::IsMpuPermittedDefNG(PhysAddr vrtl_addr, ByteSize size) {
  uint32_t spid = m_srf->SrVSPID()->ReadBody();
  bool hve = m_vm_supported ? static_cast<bool>(m_srf->SrHVCFG()->GetHVE()) : false;
  bool is_conventional = !hve;
  bool um = static_cast<bool>(m_srf->SrVPSW()->GetUM()); // true: UM, false: SV
  bool gm = is_conventional ? true : static_cast<bool>(m_srf->SrPSWH()->GetGM()); // true: guest mode, false: host mode
  bool svp =
      static_cast<bool>(m_srf->SrVMPM()->GetSVP()); // true: mem protect valid, false: invalid
  bool mpe = m_srf->SrVMPM()->GetMPE();
  bool gmpe = m_srf->SrVMPM()->GetGMPE();
  int hbe;
  if (cmd == INST_READ) {
    std::bitset<MpuRegion::N_SPID> is_guest_prmt_low(!mpe);
    std::bitset<MpuRegion::N_SPID> is_guest_prmt_mdl(!mpe);
    std::bitset<MpuRegion::N_SPID> is_guest_prmt_high(!mpe);
    std::bitset<MpuRegion::N_SPID> is_host_prmt_low(gm ? !gmpe : !mpe);
    std::bitset<MpuRegion::N_SPID> is_host_prmt_mdl(gm ? !gmpe : !mpe);
    std::bitset<MpuRegion::N_SPID> is_host_prmt_high(gm ? !gmpe : !mpe);
    // if before 2.0 or HVE==0, HOST ENTRY is nothing, set a large value for HBE.
    if (is_conventional) {
      hbe = 0xffU;
      is_host_prmt_low = is_host_prmt_mdl = is_host_prmt_high = 1;
    }
    // HVE is true
    else {
      hbe = m_srf->SrMPCFG()->GetHBE();
    }
    bool prmt_low, prmt_mdl, prmt_high;
    auto checkpermit = [&](MpuRegion* ent) {
      prmt_low = prmt_mdl = prmt_high = false;
      if (ent->IsMpuRegionHit<cmd>(vrtl_addr, SIZE_8BIT, spid)) {
        prmt_low = ent->IsMpuRegionPermitted<cmd>(um);
      }
      if ((size == SIZE_64BIT) && ((vrtl_addr & 0x2) != 0)) {
        if (ent->IsMpuRegionHit<cmd>(vrtl_addr + 2, SIZE_8BIT, spid)) {
          prmt_mdl = ent->IsMpuRegionPermitted<cmd>(um);
        }
      } else {
        prmt_mdl = true;
      }
      if (ent->IsMpuRegionHit<cmd>(vrtl_addr + size - 1, SIZE_8BIT, spid)) {
        prmt_high = ent->IsMpuRegionPermitted<cmd>(um);
      }
    };
    // guest-mode
    if (gm) {
      for (auto&& ite : m_mprgn_vec) {
        const int rgnid = ite.GetMpuRgnID();
        // guest entry
        if (rgnid < hbe) {
          // (mpe AND NOT(NOT um AND NOT svp) -> (mpe AND (svp OR um))
          if (mpe && (svp || um)) {
            checkpermit(&ite);
            is_guest_prmt_low[rgnid] = prmt_low;
            is_guest_prmt_mdl[rgnid] = prmt_mdl;
            is_guest_prmt_high[rgnid] = prmt_high;
          } else {
            is_guest_prmt_low[rgnid] = is_guest_prmt_mdl[rgnid] = is_guest_prmt_high[rgnid] = true;
          }
        }
        // host entry
        else {
          if (gmpe) {
            // check host entry
            checkpermit(&ite);
            is_host_prmt_low[rgnid] = prmt_low;
            is_host_prmt_mdl[rgnid] = prmt_mdl;
            is_host_prmt_high[rgnid] = prmt_high;
          } else {
            is_host_prmt_low[rgnid] = is_host_prmt_mdl[rgnid] = is_host_prmt_high[rgnid] = true;
          }
        }
      }
      // result
      bool is_host = (is_host_prmt_low.any() & is_host_prmt_mdl.any() & is_host_prmt_high.any());
      bool is_guest =
          (is_guest_prmt_low.any() & is_guest_prmt_mdl.any() & is_guest_prmt_high.any());
      if (is_guest) {
        if (is_host) {
          SetMpuExpPOS(M_NONE);
          return true;
        } else {
          SetMpuExpPOS(M_HOSTENT);
        }
      } else {
        SetMpuExpPOS(M_GUESTENT);
      }
    }
    // host-mode
    else {
      for (auto&& ite : m_mprgn_vec) {
        const int rgnid = ite.GetMpuRgnID();
        if (rgnid >= hbe) {
          // host entry
          if (mpe && (svp || um)) {
            checkpermit(&ite);
            is_host_prmt_low[rgnid] = prmt_low;
            is_host_prmt_mdl[rgnid] = prmt_mdl;
            is_host_prmt_high[rgnid] = prmt_high;
          } else {
            is_host_prmt_low[rgnid] = is_host_prmt_mdl[rgnid] = is_host_prmt_high[rgnid] = true;
          }
        }
      }
      // result
      bool is_host = (is_host_prmt_low.any() & is_host_prmt_mdl.any() & is_host_prmt_high.any());
      if (is_host) {
        SetMpuExpPOS(M_NONE);
        return true;
      } else {
        SetMpuExpPOS(M_HOSTENT);
      }
    }
    return false;
  } else {
    std::bitset<MpuRegion::N_SPID> is_guest_prmt(!mpe);
    std::bitset<MpuRegion::N_SPID> is_host_prmt(gm ? !gmpe : !mpe);
    // if before 2.0 or HVE==0, HOST ENTRY is nothing, set a large value for HBE.
    if (is_conventional) {
      hbe = 0xffU;
      is_host_prmt = 1;
    }
    // HVE is true
    else {
      hbe = m_srf->SrMPCFG()->GetHBE();
    }

    // guest-mode
    if (gm) {
      for (auto&& ite : m_mprgn_vec) {
        const int rgnid = ite.GetMpuRgnID();
        // guest entry
        if (rgnid < hbe) {
          if (mpe && (svp || um)) {
            if (ite.IsMpuRegionHit<cmd>(vrtl_addr, size, spid)) {
              is_guest_prmt[rgnid] = ite.IsMpuRegionPermitted<cmd>(um);
            }
          } else {
            is_guest_prmt[rgnid] = true;
          }
        }
        // host entry
        else {
          if (gmpe) {
            if (ite.IsMpuRegionHit<cmd>(vrtl_addr, size, spid)) {
              is_host_prmt[rgnid] = ite.IsMpuRegionPermitted<cmd>(um);
            }
          } else {
            is_host_prmt[rgnid] = true;
          }
        }
      }
      // result
      if (is_guest_prmt.any()) {
        if (is_host_prmt.any()) {
          SetMpuExpPOS(M_NONE);
          return true;
        } else {
          SetMpuExpPOS(M_HOSTENT);
        }
      } else {
        SetMpuExpPOS(M_GUESTENT);
      }
    }
    // host-mode
    else {
      for (auto&& ite : m_mprgn_vec) {
        const int rgnid = ite.GetMpuRgnID();
        // host entry
        if (rgnid >= hbe) {
          if (mpe && (svp || um)) {
            if (ite.IsMpuRegionHit<cmd>(vrtl_addr, size, spid)) {
              is_host_prmt[rgnid] = ite.IsMpuRegionPermitted<cmd>(um);
            }
          } else {
            is_host_prmt[rgnid] = true;
          }
        }
      }
      // result
      if (is_host_prmt.any()) {
        SetMpuExpPOS(M_NONE);
        return true;
      } else {
        SetMpuExpPOS(M_HOSTENT);
      }
    }
    return false;
  }
}

bool Mpu::IsMpuPermittedRMW(PhysAddr vrtl_addr, ByteSize size) {
  bool is_read_ok = IsMpuPermitted<DATA_READ>(vrtl_addr, size);
  faultPos exp_cause_read = GetMpuExpPOS();
  uint32_t e_read = GetMpuExpCause();
  bool is_write_ok = IsMpuPermitted<DATA_WRITE>(vrtl_addr, size);
  faultPos exp_cause_write = GetMpuExpPOS();
  uint32_t e_write = GetMpuExpCause();

  if (is_read_ok && is_write_ok) {
    return true;
  } else {
    uint32_t exp_cause = 0x0;
    uint32_t exp_code_read;
    uint32_t exp_code_write;

    if (exp_cause_read < exp_cause_write) {
      SetMpuExpPOS(exp_cause_read);
    } else {
      SetMpuExpPOS(exp_cause_write);
    }

    if (is_read_ok == false) {
      exp_code_read = e_read & 0x0000ffffU;
      exp_cause |= e_read & 0xffff0000U;
    } else {
      exp_code_read = 0x0000ffffU;
    }
    if (is_write_ok == false) {
      exp_code_write = e_write & 0x0000ffffU;
      exp_cause |= e_write & 0xffff0000U;
    } else {
      exp_code_write = 0x0000ffffU;
    }

    if (exp_code_read == exp_code_write) {
      exp_cause |= exp_code_read;
    } else if (exp_code_read < exp_code_write) {
      exp_cause |= exp_code_read;
    } else {
      exp_cause |= exp_code_write;
    }
    m_exp_cause = exp_cause;
    return false;
  }
}

/*!
 * @brief Get MPU exception cause code
 * @param vrtl_addr address
 * @param cmd Access command
 * @param size mem access size
 * @param mdp_type Type of MDP exception
 * @return value of Exception cause code
 */

uint32_t Mpu::GetMpuExpCause(PhysAddr vrtl_addr, MemAccessCommand cmd, ByteSize size,
                             MdpType mdp_type) const {
  uint32_t exp_cause = 0;

  if (cmd != INST_READ) {
    // SET1,NOT1,CLR1,CAXI命令時に bit 22 をセット
    // PREPARE,DISPOSE,PUSHSP,POPSP命令時に bit 23 をセット
    uint32_t exp_cause_bitpos = MicroArchDB::GetMdpExpCauseBitPos(m_trace->GetInstId());
    if (exp_cause_bitpos != 0) {
      exp_cause |= (1 << exp_cause_bitpos);
    }

    // ミスアライン・アクセス時に bit 24 をセット
    if ((vrtl_addr & (size - 1)) != 0x0) {
      exp_cause |= 0x01000000;
    }
  }

  // 保護違反内容に応じた bit をセット
  if (mdp_type == MDP_AT_NORMAL) {
    if (m_srf->SrVPSW()->GetUM() == 1) {
      switch (cmd) {
      case INST_READ:
        exp_cause |= 0x00040090;
        break;
      case DATA_READ:
        exp_cause |= 0x00010091;
        break;
      case DATA_WRITE:
        exp_cause |= 0x00020091;
        break;
      default:
        break;
      }
    } else {
      switch (cmd) {
      case INST_READ:
        exp_cause |= 0x00200090;
        break;
      case DATA_READ:
        exp_cause |= 0x00080091;
        break;
      case DATA_WRITE:
        exp_cause |= 0x00100091;
        break;
      default:
        break;
      }
    }
  } else { // MDP_AT_EIINT
    // PSW.UM=0 is used for MDP checking in EIINT.
    switch (cmd) {
    case DATA_READ:
      exp_cause |= 0x00080095;
      break;
    case DATA_WRITE:
      exp_cause |= 0x00100095;
      break;
    default:
      break;
    }
  }
  // Table2-21: ゲストモードでのMPU例外要因コード
  if (GetMpuExpPOS() == M_HOSTENT) {
    exp_cause |= 0x00000008;
  }

  return exp_cause;
}

/*!
 * @brief Check memory protection setting
 * @param -
 * @return -
 * @warning The TRUE return is the MCR register
 * @note Memory check result when overflow input
 *   | input                                                            | output (MCR)                               |
 *   | PSWH.GM | MPM.MPE | MPM.SVP | GMMPM.GMPE | GMMPM.MPE | GMMPM.SVP | HS*E | HU*E | GS*E | GU*E | S*E | U*E | OV |
 *   |       1 |       x |       x |          0 |         0 |         x |    1 |    1 |    1 |    1 |   1 |   1 |  0 |
 *   |       1 |       x |       x |          0 |         1 |         0 |    1 |    1 |    1 |    0 |   1 |   0 |  1 |
 *   |       1 |       x |       x |          0 |         1 |         1 |    1 |    1 |    0 |    0 |   0 |   0 |  1 |
 *   |       1 |       x |       x |          1 |         0 |         x |    0 |    0 |    1 |    1 |   0 |   0 |  1 |
 *   |       1 |       x |       x |          1 |         1 |         0 |    0 |    0 |    1 |    0 |   0 |   0 |  1 |
 *   |       1 |       x |       x |          1 |         1 |         1 |    0 |    0 |    0 |    0 |   0 |   0 |  1 |
 *   |       0 |       0 |       x |          x |         x |         x |    1 |    1 |    1 |    1 |   1 |   1 |  0 |
 *   |       0 |       1 |       0 |          x |         x |         x |    1 |    0 |    1 |    1 |   1 |   0 |  1 |
 *   |       0 |       1 |       1 |          x |         x |         x |    0 |    0 |    1 |    1 |   0 |   0 |  1 |
 */
void Mpu::CheckMpuSetting(void) {
  bool hve = m_vm_supported ? static_cast<bool>(m_srf->SrHVCFG()->GetHVE()) : false;
  bool is_conventional = !hve;
  bool gm = is_conventional ? true : static_cast<bool>(m_srf->SrPSWH()->GetGM()); // true: guest mode, false: host mode
  SRegBaseMPM* vmpm = m_srf->SrVMPM();
  bool mpe = vmpm->GetMPE();
  bool svp = vmpm->GetSVP();
  bool gmpe = vmpm->GetGMPE();

  SRegMCR* mcr = m_srf->SrMCR();
  mcr->WriteBody(0x0);
  RegData mca = m_srf->SrMCA()->ReadBody();
  RegData mcs = m_srf->SrMCS()->ReadBody();
  if (mca < 0x080000000U) {
    if (0x7FFFFFFFU - mca < mcs - 1) {
      //指定された領域が7FFFFFFHをまたがる場合
      mcr->SetOV(1); // UT: (a1)
    }
  } else {
    if (0xFFFFFFFFU - mca < mcs - 1) {
      //指定された領域が00000000Hをまたがる場合
      mcr->SetOV(1); // UT: (a2)
    }
  }

#define MCR(reg, val)       \
  {                         \
    mcr->Set##reg##XE(val); \
    mcr->Set##reg##WE(val); \
    mcr->Set##reg##RE(val); \
  }
  if (mcr->GetOV() == 1) {
    // guest mode
    if (gm) {
      if (gmpe) {
        if (mpe) {
          if (!svp) {
            if (!is_conventional) { MCR(GS, 1); }
          }
        } else {
          if (!is_conventional) { MCR(GS, 1); MCR(GU, 1); }
        }
      } else {
        if (mpe) {
          if (svp) {
            if (!is_conventional) { MCR(HS, 1); MCR(HU, 1); }
          } else {
            if (is_conventional) { MCR(S, 1); }
            else                 { MCR(HS, 1); MCR(HU, 1); MCR(GS, 1);  MCR(S, 1); }
          }
        } else {
          mcr->SetOV(0);
          if (is_conventional) { MCR(S, 1); MCR(U, 1); }
          else                 { MCR(HS, 1); MCR(HU, 1); MCR(GS, 1); MCR(GU, 1); MCR(S, 1); MCR(U, 1); }
        }
      }
    }
    // host mode
    else {
      if (mpe) {
        if (svp) {
          if (!is_conventional) { MCR(GS, 1); MCR(GU, 1); }
        } else {
          if (is_conventional) { MCR(S, 1); }
          else                 { MCR(HS, 1);  MCR(GS, 1); MCR(GU, 1); MCR(S, 1); }
        }
      } else {
        mcr->SetOV(0);
        if (is_conventional) { MCR(S, 1); MCR(U, 1); }
        else                 { MCR(HS, 1); MCR(HU, 1); MCR(GS, 1); MCR(GU, 1); MCR(S, 1); MCR(U, 1); }
      }
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE>(CF_SR_MCR, mcr->ReadBody());
    return; // Exit (a)
  }

  uint32_t spid = m_srf->SrMCI()->GetSPID();
#define XE 0
#define WE 1
#define RE 2
  std::vector<bool> s  = {false, false, false}; // sxe, swe, sre
  std::vector<bool> u  = {false, false, false}; // uxe, uwe, ure
  std::vector<bool> gs = {false, false, false}; // gsxe, gswe, gsre
  std::vector<bool> gu = {false, false, false}; // guxe, guwe, gure
  std::vector<bool> hs = {false, false, false}; // hsxe, hswe, hsre
  std::vector<bool> hu = {false, false, false}; // huxe, huwe, hure
  int hbe;

  gs[XE] = gs[WE] = gs[RE] = gu[XE] = gu[WE] = gu[RE] = !mpe;
  hs[XE] = hs[WE] = hs[RE] = hu[XE] = hu[WE] = hu[RE] = (gm ? !gmpe : !mpe);

  // if before 2.0 or HVE==0, HOST ENTRY is nothing, set a large value for HBE.
  if (is_conventional) {
    hs = hu = { true, true, true };
    hbe = 0xffU;
  }
  // HVE is true
  else {
    hbe = m_srf->SrMPCFG()->GetHBE();
  }

  // guest mode
  if (gm) {
    for (auto&& ite : m_mprgn_vec) {
      const int rgnid = ite.GetMpuRgnID();
      // guest entry
      if (rgnid < hbe) {
        for (int um = 0; um < 2; um++) {
          if (mpe && (svp || um)) {
            if (ite.IsMpuRegionHit<INST_READ>(mca, mcs, spid)) {
              if (ite.IsMpuRegionPermitted<INST_READ>(um)) {
                if (um) { gu[XE] = true; } // user mode
                else    { gs[XE] = true; } // sv mode
              }
            }
          #if defined(_UNIT_TEST)
                if (um) { gu[WE] = true; } // user mode
                else    { gs[WE] = true; } // sv mode
          #else
            if (ite.IsMpuRegionHit<DATA_WRITE>(mca, mcs, spid)) {
              if (ite.IsMpuRegionPermitted<DATA_WRITE>(um)) {
                if (um) { gu[WE] = true; } // user mode
                else    { gs[WE] = true; } // sv mode
              }
            }
          #endif
          #if defined(_UNIT_TEST)
              if (um) { gu[RE] = true; } // user mode
              else    { gs[RE] = true; } // sv mode
          #else
            if (ite.IsMpuRegionHit<DATA_READ>(mca, mcs, spid)) {
              if (ite.IsMpuRegionPermitted<DATA_READ>(um)) {
                if (um) { gu[RE] = true; } // user mode
                else    { gs[RE] = true; } // sv mode
              }
            }
          #endif
          } else {
            if (um) { gu = { true, true, true }; } // user mode
            else    { gs = { true, true, true }; } // sv mode
          }
        }
      }
      // host entry
      else {
        for (int um = 0; um < 2; um++) {
          if (gmpe) {
            if (ite.IsMpuRegionHit<INST_READ>(mca, mcs, spid)) {
              if (ite.IsMpuRegionPermitted<INST_READ>(um)) {
                if (um) { hu[XE] = true; } // user mode
                else    { hs[XE] = true; } // sv mode
              }
            }
          #if defined(_UNIT_TEST)
                if (um) { hu[WE] = true; } // user mode
                else    { hs[WE] = true; } // sv mode
          #else
            if (ite.IsMpuRegionHit<DATA_WRITE>(mca, mcs, spid)) {
              if (ite.IsMpuRegionPermitted<DATA_WRITE>(um)) {
                if (um) { hu[WE] = true; } // user mode
                else    { hs[WE] = true; } // sv mode
              }
            }
          #endif
          #if defined(_UNIT_TEST)
                if (um) { hu[RE] = true; } // user mode
                else    { hs[RE] = true; } // sv mode
          #else
            if (ite.IsMpuRegionHit<DATA_READ>(mca, mcs, spid)) {
              if (ite.IsMpuRegionPermitted<DATA_READ>(um)) {
                if (um) { hu[RE] = true; } // user mode
                else    { hs[RE] = true; } // sv mode
              }
            }
          #endif
          } else {
            if (um) { hu = { true, true, true }; } // user mode
            else    { hs = { true, true, true }; } // sv mode
          }
        }
      }
    }
  }
  // host mode
  else {
    for (auto&& ite : m_mprgn_vec) {
      const int rgnid = ite.GetMpuRgnID();
      // host entry
      if (rgnid >= hbe) {
        for (int um = 0; um < 2; um++) {
          if (mpe && (svp || um)) {
            if (ite.IsMpuRegionHit<INST_READ>(mca, mcs, spid)) {
              if (ite.IsMpuRegionPermitted<INST_READ>(um)) {
                if (um) { hu[XE] = true; } // user mode
                else    { hs[XE] = true; } // sv mode
              }
            }
          #if defined(_UNIT_TEST)
                if (um) { hu[WE] = true; } // user mode
                else    { hs[WE] = true; } // sv mode
          #else
            if (ite.IsMpuRegionHit<DATA_WRITE>(mca, mcs, spid)) {
              if (ite.IsMpuRegionPermitted<DATA_WRITE>(um)) {
                if (um) { hu[WE] = true; } // user mode
                else    { hs[WE] = true; } // sv mode
              }
            }
          #endif
          #if defined(_UNIT_TEST)
                if (um) { hu[RE] = true; } // user mode
                else    { hs[RE] = true; } // sv mode
          #else
            if (ite.IsMpuRegionHit<DATA_READ>(mca, mcs, spid)) {
              if (ite.IsMpuRegionPermitted<DATA_READ>(um)) {
                if (um) { hu[RE] = true; } // user mode
                else    { hs[RE] = true; } // sv mode
              }
            }
          #endif
          } else {
            if (um) { hu = { true, true, true }; } // user mode
            else    { hs = { true, true, true }; } // sv mode
          }
        }
      }
    }
    gs = { true, true, true }; // user mode
    gu = { true, true, true }; // sv mode
  }

  if (is_conventional) {
    s[XE] = gs[XE]; s[WE] = gs[WE]; s[RE] = gs[RE];
    u[XE] = gu[XE]; u[WE] = gu[WE]; u[RE] = gu[RE];
    hs = hu = gs = gu = { false, false, false };
  }
  else {
    s[XE] = hs[XE] & gs[XE]; s[WE] = hs[WE] & gs[WE]; s[RE] = hs[RE] & gs[RE];
    u[XE] = hu[XE] & gu[XE]; u[WE] = hu[WE] & gu[WE]; u[RE] = hu[RE] & gu[RE];
  }

  mcr->SetSXE(s[XE]);   mcr->SetUXE(u[XE]);   mcr->SetSWE(s[WE]);
  mcr->SetUWE(u[WE]);   mcr->SetSRE(s[RE]);   mcr->SetURE(u[RE]);
  mcr->SetGSXE(gs[XE]); mcr->SetGUXE(gu[XE]); mcr->SetGSWE(gs[WE]);
  mcr->SetGUWE(gu[WE]); mcr->SetGSRE(gs[RE]); mcr->SetGURE(gu[RE]);
  mcr->SetHSXE(hs[XE]); mcr->SetHUXE(hu[XE]); mcr->SetHSWE(hs[WE]);
  mcr->SetHUWE(hu[WE]); mcr->SetHSRE(hs[RE]); mcr->SetHURE(hu[RE]);

  m_trace->RecordSReg<TraceCtrl::REG_WRITE>(CF_SR_MCR, mcr->ReadBody());
  // UT: Exit (b)
}

/*!
 * @brief Add MPU region object
 * @param rgnid MPU region ID
 * @return -
 */
void Mpu::AddMpuRegion(uint32_t rgnid) {
  MpuRegion mprgn(rgnid,

                  static_cast<SRegMPLA_N*>(m_srf->GetSrPtr(rgnid + CF_SR_MPLA0)),
                  static_cast<SRegMPUA_N*>(m_srf->GetSrPtr(rgnid + CF_SR_MPUA0)),
                  static_cast<SRegMPAT_N*>(m_srf->GetSrPtr(rgnid + CF_SR_MPAT0)), m_srf);
  m_mprgn_vec.push_back(mprgn);
  m_mprgn_vec.back().UpdatePermitSpid(); // @@ TODO : Set this in constructor.
}

/*!
 * @brief delete MPU region object
 * @param rgnid MPU regin ID
 * @return -
 */
void Mpu::DelMpuRegion(uint32_t rgnid) {
  MpuRegionVec::iterator ite = std::find(m_mprgn_vec.begin(), m_mprgn_vec.end(), rgnid);
  if (ite != m_mprgn_vec.end()) {
    m_mprgn_vec.erase(ite);
  }
}

/*!
 * @brief delete all MPU region object
 * @param -
 * @return -
 */
void Mpu::AllDelMpuRegion(void) { m_mprgn_vec.erase(m_mprgn_vec.begin(), m_mprgn_vec.end()); }

template bool Mpu::IsMpuPermitted<INST_READ>(PhysAddr vrtl_addr, ByteSize size);
template bool Mpu::IsMpuPermitted<DATA_READ>(PhysAddr vrtl_addr, ByteSize size);
template bool Mpu::IsMpuPermitted<DATA_WRITE>(PhysAddr vrtl_addr, ByteSize size);
template bool Mpu::IsMpuPermittedEI<DATA_READ>(PhysAddr vrtl_addr, ByteSize size);
template bool Mpu::IsMpuPermittedEI<DATA_WRITE>(PhysAddr vrtl_addr, ByteSize size);
template bool Mpu::MpuRegion::IsMpuRegionPermitted<INST_READ>(bool um) const;
template bool Mpu::MpuRegion::IsMpuRegionPermitted<DATA_READ>(bool um) const;
template bool Mpu::MpuRegion::IsMpuRegionPermitted<DATA_WRITE>(bool um) const;
