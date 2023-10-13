/*
 * (c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once

#include <vector>
#include "./forest_common.h"
#include "./module_vc.h"
#include "./module_guard.h"
#include "./treg.h"

class GERRSTAT;
class GKCPROT;
class GuardInternal;


static const uint32_t GUARD_DATA_MASK_32        = 0xFFFFFFFFU;
static const uint32_t GUARD_DATA_MASK_16        = 0x0000FFFFU;
static const uint32_t GUARD_DATA_MASK_8         = 0x000000FFU;

static const uint32_t PEG_MODULE                = 0x00U;
static const uint32_t CRG_MODULE                = 0x01U;
static const uint32_t CSG_MODULE                = 0x02U;


/*!
 * @brief GuardInternal_Interface: Interface of GuardInternal.
 */
class GuardInternal_Interface {
public:
  virtual bool IsGuardEn () = 0;
  virtual bool IsWriteEn () = 0;
  virtual bool IsChannelEn (uint32_t chn) = 0;
  virtual bool IsCrgIvcVmEn (uint32_t chn) = 0;
  virtual bool IsCrgIvcVmLock (uint32_t chn) = 0;
  virtual bool IsCrgCsgLock (uint32_t chn) = 0;
  virtual uint32_t GetCrgSpidOwner (uint32_t chn) = 0;
  virtual uint32_t GetCrgSpidListValid (uint32_t chn) = 0;
  virtual uint32_t GetCrgMaddrw () = 0;
  virtual uint32_t GetMaxCtrlChannel () = 0;
  virtual uint32_t GetMaxCsgChannel () = 0;
  virtual uint32_t GetKeyCode () = 0;
  virtual uint32_t GetClid () = 0;
  virtual void SetGuardEn (bool guard_setting) = 0;
  virtual void SetWriteEn (bool guard_write) = 0;
  virtual void SetChannelEn (uint32_t chn, bool chn_en) = 0;
  virtual void SetCrgIvcVmEn (uint32_t chn, bool ivc_vm_set) = 0;
  virtual void SetCrgIvcVmLock (uint32_t chn, bool ivc_vm_lock) = 0;
  virtual void SetCrgCsgLock (uint32_t chn, bool crg_csg_lock) = 0;
  virtual void SetCrgSpidOwner (uint32_t chn, uint32_t spid_owner) = 0;
  virtual void SetCrgSpidListValid (uint32_t chn, uint32_t spid_valid) = 0;
  virtual void Reset () = 0;
  GuardInternal_Interface(void) {}
  virtual ~GuardInternal_Interface() {}

  // These values will be disable in CrgParam class.
  // The default value are:
  // m_addr_width (21), m_num_of_ch_crg (8), m_num_of_ch_csg (4), m_keycode (0xA5A5A500)
  // There are 8 channels in CRG control registers.
  static const uint32_t NUM_CRG_CTRL_CHANNEL            = 8;
  // There are 4 channels in CRG secure registers.
  static const uint32_t NUM_CRG_SEC_CHANNEL             = 4;
  // Default pMADDRW In G4MH2x.
  static const uint32_t CRG_PMADDRW_G4MH2X              = 19;
  // Default KeyCode Value.
  static const uint32_t GUARD_KEYCODE                   = 0xA5A5A500U;
};


/*!
 * @brief GuardInternal: This is the mediator for the communication between Guard register.
 *        It will check internal conditions between them.
 */
class GuardInternal: public GuardInternal_Interface {
private:
    /* # Variable info # */
    // 0. Use in CRG and PEG
    bool m_guard_write_en;
    bool m_guard_setting_en;
    // 1. Only Use in CRG
    // 1.a Can be changed
    bool *m_crg_ivc_vm_en;
    bool *m_crg_ivc_vm_lock;
    bool *m_crg_csg_lock;
    uint32_t *m_crg_spid_list_valid;
    uint32_t *m_crg_spid_owner;
    // 1.b Can't be changed
    uint32_t m_crg_maddrw;
    // In CRG: the following variables can be set by python settings (example: for testing).
    // In PEG: they can't be set by python settings.
    // 2.a After they are set by python, they can't be changed.
    uint32_t m_guard_max_channel;
    uint32_t m_ivc_max_channel;
    uint32_t m_csg_max_channel;
    uint32_t m_keycode;
    uint32_t m_clid;
    // 3. Channel status in CRG (not include CRG_CSG channel) and PEG.
    bool *m_channel_en;

public:
    bool IsGuardEn () {return m_guard_setting_en;}
    bool IsWriteEn () {return m_guard_write_en;}
    bool IsChannelEn (uint32_t chn) {return (m_guard_max_channel > chn) ? m_channel_en[chn] : false;}
    bool IsCrgIvcVmEn   (uint32_t chn) {return (m_ivc_max_channel > chn) ? m_crg_ivc_vm_en[chn] : false;}
    bool IsCrgIvcVmLock (uint32_t chn) {return (m_ivc_max_channel > chn) ? m_crg_ivc_vm_lock[chn] : false;}
    bool IsCrgCsgLock   (uint32_t chn) {return  (m_csg_max_channel > chn)? m_crg_csg_lock[chn] : false;}
    uint32_t GetCrgSpidOwner (uint32_t chn) {return (m_ivc_max_channel > chn) ? m_crg_spid_owner[chn] : 0x0;}
    uint32_t GetCrgSpidListValid (uint32_t chn) {return (m_ivc_max_channel > chn) ? m_crg_spid_list_valid[chn] : 0x0;}
    uint32_t GetCrgMaddrw () {return m_crg_maddrw;}
    uint32_t GetMaxCtrlChannel () {return m_guard_max_channel;}
    uint32_t GetMaxCsgChannel  () {return m_csg_max_channel;}
    uint32_t GetKeyCode () {return m_keycode;}
    uint32_t GetClid () {return m_clid;}
    void SetGuardEn (bool guard_setting) {m_guard_setting_en = guard_setting;}
    void SetWriteEn (bool guard_write) {m_guard_write_en = guard_write;}
    void SetChannelEn (uint32_t chn, bool chn_en) {if (m_guard_max_channel > chn) m_channel_en[chn] = chn_en;}
    void SetCrgIvcVmEn (uint32_t chn, bool ivc_vm_set) { if (m_ivc_max_channel > chn) m_crg_ivc_vm_en[chn] = ivc_vm_set;}
    void SetCrgIvcVmLock (uint32_t chn, bool ivc_vm_lock) { if (m_ivc_max_channel > chn) m_crg_ivc_vm_lock[chn] = ivc_vm_lock;}
    void SetCrgCsgLock (uint32_t chn, bool crg_csg_lock) {if (m_csg_max_channel > chn) m_crg_csg_lock[chn] = crg_csg_lock;}
    void SetCrgSpidOwner (uint32_t chn, uint32_t spid_owner) { if (m_ivc_max_channel > chn) m_crg_spid_owner[chn] = spid_owner;}
    void SetCrgSpidListValid (uint32_t chn, uint32_t spid_list_valid) { if (m_ivc_max_channel > chn) m_crg_spid_list_valid[chn] = spid_list_valid;}

    void Reset (void) {
        // The remaining settings will be kept after reset state:
        // m_crg_maddrw, m_guard_max_channel, m_ivc_max_channel, m_csg_max_channel, m_keycode, m_clid.

        // These following settings will be reset.
        m_guard_write_en   = false;
        m_guard_setting_en = false;
        for (uint32_t ch = 0; ch < m_guard_max_channel; ch++) {
            m_channel_en[ch] = false;
        }
        for (uint32_t chn = 0; chn < m_ivc_max_channel; chn++) {
            m_crg_ivc_vm_en[chn]       = false;
            m_crg_ivc_vm_lock[chn]     = false;
            m_crg_spid_list_valid[chn] = 0x0;
            m_crg_spid_owner[chn]      = 0x0;
        }
        for (uint32_t chn = 0; chn < m_csg_max_channel; chn++) {
            m_crg_csg_lock[chn] = false;
        }

    }
    GuardInternal (bool guard_write_en, bool guard_setting_en, uint32_t crg_maddwr,
                   uint32_t guard_max_channel, uint32_t ivc_max_channel, uint32_t csg_max_channel,
                   uint32_t keycode, uint32_t clid)
       : m_guard_write_en (guard_write_en),
         m_guard_setting_en (guard_setting_en),
         m_crg_maddrw (crg_maddwr),
         m_guard_max_channel (guard_max_channel),
         m_ivc_max_channel   (ivc_max_channel),
         m_csg_max_channel   (csg_max_channel),
         m_keycode (keycode),
         m_clid (clid)
    {
        // In G4MH2.0, there are 8 CRG (also IVC) channels and 4 CSG channels.
        m_channel_en          = new bool     [m_guard_max_channel];
        m_crg_ivc_vm_en       = new bool     [m_ivc_max_channel];
        m_crg_ivc_vm_lock     = new bool     [m_ivc_max_channel];
        m_crg_spid_list_valid = new uint32_t [m_ivc_max_channel];
        m_crg_spid_owner      = new uint32_t [m_ivc_max_channel];
        m_crg_csg_lock        = new bool     [m_csg_max_channel];
    }
    ~GuardInternal () {
        delete[] m_channel_en;
        delete[] m_crg_csg_lock;
        delete[] m_crg_ivc_vm_en;
        delete[] m_crg_ivc_vm_lock;
        delete[] m_crg_spid_list_valid;
        delete[] m_crg_spid_owner;
    }
};


/*!
 * @brief GKCPROT: This is the common class for Guard Key code protection register.
 *                It has 32bit and the accessing size is 32bit.
*/
class GKCPROT : public TRegDataBase<TRegData32> {
private:
    union {
        TRegData32 m_data;
        struct {
            RegData m_kce    :1;
            RegData          :31;    // kcprot [1-31]
        } m_bit;
    };

    static const uint32_t GKCPROT_INIT_VAL = 0x00000000U;

    std::string m_name;
    GuardInternal_Interface *m_guard_internal;

public:
#if _UNIT_TEST
    std::string m_guard_msg;
#endif

    void Reset (void) {
        m_data.m_data32 = GKCPROT_INIT_VAL;
    }

    // Override
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);

    RegData GetKCE    (void) const { return m_bit.m_kce; }
    void SetKCE    (RegData kce) { m_bit.m_kce = kce & 0x1U; }

    // Override
    const char* GetName (void) const {return m_name.c_str();}
#if _UNIT_TEST
    PhysAddr GetRegAddr () {return m_reg_addr;}
#endif
    GKCPROT (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr, std::string reg_name, GuardInternal_Interface *guard_internal)
    : TRegDataBase<TRegData32> (reg_size, mini_access_size, reg_addr, TRegMask (GUARD_DATA_MASK_32)) {
        m_name = reg_name.c_str();
        m_data.m_data32 = GKCPROT_INIT_VAL;
        m_treg = &m_data;
        m_guard_internal = guard_internal;
    }
};


/*!
 * @brief GPROT: This is the common class for Guard control register.
 *              It has 32 bit and the accessing size is 8, 16, 32bit.
 */
class GPROT : public TRegDataBase<TRegData32> {
private:
    union {
        TRegData32 m_data;
        struct {
            RegData m_rg     :1; // Bit 0: G4MH1x, 2x
            RegData m_wg     :1; // Bit 1: G4MH1x, 2x
            RegData          :2; // Bit 2, 3: Reserve
            RegData m_ume    :1; // Bit 4: G4MH1x, 2x
            RegData          :1; // Bit 5: Reserve
            RegData m_dbg    :1; // Bit 6: G4MH1x, 2x
            RegData          :1; // Bit 7: Reserve
            RegData m_gen    :1; // Bit 8: G4MH1x, 2x
            RegData          :7; // Bit 9, 15: Reserve
            RegData m_ow     :1; // Bit 16: G4MH2x (CRG)
            RegData          :14; // Bit [17, 30]: Reserve
            RegData m_lock   :1; // Bit 31: G4MH2x (CSG)
        } m_bit;
    };

    static const uint32_t GPROT_INIT_VAL  = 0x00000000U;
    static const uint32_t PEG_MASK        = 0x00000153U;
    static const uint32_t CRG_MASK_G4MH1X = 0x00000153U;
    static const uint32_t CRG_MASK_G4MH2X = 0x00010153U;
    static const uint32_t CSG_MASK_G4MH2X = 0x80000153U;

    std::string m_name;
    uint32_t m_channel;
    uint32_t m_guard_module;
    uint32_t m_core_type;
    uint32_t m_core_version;

    GuardInternal_Interface *m_guard_internal;

    TRegMask IsRegMask (uint32_t guard_module, CoreType core_type, CoreVersion core_version) {
        if (guard_module == PEG_MODULE) {
            return TRegMask (PEG_MASK);
        } else if ((guard_module == CRG_MODULE)
                && (core_type == CORE_G4MH) && (core_version < CORE_VER_20)) {
            return TRegMask (CRG_MASK_G4MH1X);
        } else if ((guard_module == CRG_MODULE)
                && (core_type == CORE_G4MH) && (core_version >= CORE_VER_20)) {
            return TRegMask (CRG_MASK_G4MH2X);
        } else if (guard_module == CSG_MODULE) {
            return TRegMask (CSG_MASK_G4MH2X);
        } else {
            // Unreachable code.
        }
        return TRegMask (PEG_MASK);
    }

public:
    void Reset (void) {
        m_data.m_data32 = GPROT_INIT_VAL;
    }

    // Override
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);

    RegData GetGEN (void) const { return m_bit.m_gen; }
    void SetGEN (RegData gen) { m_bit.m_gen = gen & 0x1U; }

    RegData GetDBG (void) const { return m_bit.m_dbg; }
    void SetDBG (RegData dbg) { m_bit.m_dbg = dbg & 0x1U; }

    RegData GetUME (void) const { return m_bit.m_ume; }
    void SetUME (RegData ume) { m_bit.m_ume = ume & 0x1U; }

    RegData GetWG  (void) const { return m_bit.m_wg; }
    void SetWG  (RegData wg)  { m_bit.m_wg  = wg & 0x1U; }

    RegData GetRG  (void) const { return m_bit.m_rg; }
    void SetRG  (RegData rg)  { m_bit.m_rg  = rg & 0x1U; }

    RegData GetOW  (void) const { return m_bit.m_ow; }
    void SetOW  (RegData ow)  {
        m_bit.m_ow  = ow & 0x1U;
        m_guard_internal->SetCrgIvcVmEn(m_channel, m_bit.m_ow);
    }

    RegData GetLOCK  (void) const { return m_bit.m_lock; }
    void SetLOCK  (RegData lock)  {
        m_bit.m_lock  = lock & 0x1U;
        m_guard_internal->SetCrgCsgLock(m_channel, m_bit.m_lock);
    }

    // Override
    const char* GetName (void) const {return m_name.c_str();}
#if _UNIT_TEST
    PhysAddr GetRegAddr () {return m_reg_addr;}
#endif
    GPROT (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr, uint32_t channel, std::string reg_name,
           uint32_t guard_module, CoreType core_type, CoreVersion core_version, GuardInternal_Interface *guard_internal)
    : TRegDataBase<TRegData32> (reg_size, mini_access_size, reg_addr, IsRegMask (guard_module, core_type, core_version)),
      m_channel (channel),
      m_guard_module (guard_module),
      m_core_type (core_type),
      m_core_version (core_version){
        m_name = reg_name.c_str();
        m_data.m_data32 = GPROT_INIT_VAL;
        m_treg = &m_data;
        m_guard_internal = guard_internal;
    }
};


/*!
 * @brief GSPID_Interface: Interface of the GPPID.
 */
class GSPID_Interface {
public:
    virtual void SetSPID (RegData spid) = 0;
    GSPID_Interface () {}
    virtual ~GSPID_Interface() {}
};


/*!
 * @brief GSPID: This is the common class for Guard SPID setting register
 *              It has 32 bit and the accessing size is 8, 16, 32bit.
 */
class GSPID : public TRegDataBase<TRegData32>, public GSPID_Interface {
private:
    union {
        TRegData32 m_data;
        struct {
            RegData m_spid :32;
        } m_bit;
    };

    static const uint32_t GSPID_INIT_VAL = 0x00000000U;

    std::string m_name;
    uint32_t m_channel;
    uint32_t m_spid_module;
    CoreType m_core_type;
    CoreVersion m_core_version;

    GuardInternal_Interface *m_guard_internal;

public:
    void Reset (void) {
        m_data.m_data32 = GSPID_INIT_VAL;
    }

    // Override
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);

    RegData GetSPID (void) const { return m_bit.m_spid; }
    void SetSPID (RegData spid) {
        m_bit.m_spid = spid & 0xFFFFFFFFU;
        if ((m_spid_module == CRG_MODULE)
                && (m_core_type == CORE_G4MH) && (m_core_version >= CORE_VER_20)) {
            m_guard_internal->SetCrgSpidOwner(m_channel, m_bit.m_spid);
        }
    }

    // Override
    const char* GetName (void) const {return m_name.c_str();}
#if _UNIT_TEST
    PhysAddr GetRegAddr () {return m_reg_addr;}
#endif
    GSPID (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr, uint32_t channel, std::string reg_name,
           uint32_t guard_module, CoreType core_type, CoreVersion core_version, GuardInternal_Interface *guard_internal)
    : TRegDataBase<TRegData32> (reg_size, mini_access_size, reg_addr, TRegMask (GUARD_DATA_MASK_32)) {
        m_name = reg_name.c_str();
        m_channel = channel;
        m_data.m_data32 = GSPID_INIT_VAL;
        m_treg = &m_data;
        m_guard_internal = guard_internal;
        m_spid_module = guard_module;
        m_core_type = core_type;
        m_core_version = core_version;
    }
};


/*!
 * @brief GBAD: This is the common class for Guard Base Address setting register
 *             It has 32 bit and the accessing size is 32bit.
 */
class GBAD : public TRegDataBase<TRegData32> {
private:
    TRegData32 m_data;

    static const uint32_t PEGBAD_DATA_MASK      = 0x03FFF000U;
    static const uint32_t CRGBAD_DATA_MASK      = 0x0007FF00U;
    static const uint32_t PEGBAD_INIT_VAL       = 0xFC000000U;
    static const uint32_t CRGBAD_INIT_VAL       = 0x00000000U;

    std::string m_name;
    uint32_t m_channel;
    uint32_t m_init;
    uint32_t m_gbad_module;
    uint32_t crg_mask_g4mh2x;
    GuardInternal_Interface *m_guard_internal;

    TRegMask IsRegMask (uint32_t guard_module, uint32_t pmaddrw, CoreType core_type, CoreVersion core_version) {
        if (guard_module == PEG_MODULE) {
            // Use default PEG mask.
            return TRegMask (PEGBAD_DATA_MASK);
        } else if ((guard_module == CRG_MODULE)
                && (core_type == CORE_G4MH) && (core_version < CORE_VER_20)) {
            // Use default CRG mask.
            return TRegMask (CRGBAD_DATA_MASK);
        } else if (((guard_module == CRG_MODULE) || (guard_module == CSG_MODULE))
                && (core_type == CORE_G4MH) && (core_version >= CORE_VER_20)) {
            // Mask is depended on the input signal.
            crg_mask_g4mh2x = 0x0003FF00U;
            for (uint32_t i = 18; i < pmaddrw; i++ ) {
                crg_mask_g4mh2x |= (1 << i);
            }
            return TRegMask (crg_mask_g4mh2x);
        } else {
            // Unreachable code.
        }
        return TRegMask (0xFFFFFFFF);
    }

public:
    void Reset (void) {
        m_data.m_data32 = m_init;
    }

    // Override
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);

    RegData GetBAD (void) const { return m_data.m_data32; }
    void SetBAD (RegData bad) { m_data.m_data32 = bad; }

    // Override
    const char* GetName (void) const {return m_name.c_str();}
#if _UNIT_TEST
    PhysAddr GetRegAddr () {return m_reg_addr;}
#endif
    GBAD (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr, uint32_t channel, std::string reg_name,
          uint32_t pmaddrw, uint32_t guard_module, CoreType core_type, CoreVersion core_version, GuardInternal_Interface *guard_internal)
    : TRegDataBase<TRegData32> (reg_size, mini_access_size, reg_addr, IsRegMask(guard_module, pmaddrw, core_type, core_version)) {
        m_name = reg_name.c_str();
        m_channel = channel;
        m_treg = &m_data;
        m_gbad_module = guard_module;
        m_init = (m_gbad_module == PEG_MODULE) ? (PEGBAD_INIT_VAL) : (CRGBAD_INIT_VAL);
        m_data.m_data32 = m_init;
        m_guard_internal = guard_internal;
    }
};


/*!
 * @brief GADV: This is the common class for Guard Address Mask register.
 *             It has 32 bit and the accessing size is 32bit.
 */
class GADV : public TRegDataBase<TRegData32> {
private:
    TRegData32 m_data;

    static const uint32_t PEGADV_DATA_MASK      = 0x03FFF000U;
    static const uint32_t CRGADV_DATA_MASK      = 0x0007FF00U;
    static const uint32_t GADV_INIT_VAL         = 0x00000000U;

    std::string m_name;
    uint32_t m_channel;
    uint32_t m_gadv_module;
    uint32_t crg_mask_g4mh2x;
    GuardInternal_Interface *m_guard_internal;

    TRegMask IsRegMask (uint32_t guard_module, uint32_t pmaddrw, CoreType core_type, CoreVersion core_version) {
        if (guard_module == PEG_MODULE) {
            // Use default PEG mask.
            return TRegMask (PEGADV_DATA_MASK);
        } else if ((guard_module == CRG_MODULE)
                && (core_type == CORE_G4MH) && (core_version < CORE_VER_20)) {
            // Use default CRG mask.
            return TRegMask (CRGADV_DATA_MASK);
        } else if (((guard_module == CRG_MODULE) || (guard_module == CSG_MODULE))
                && (core_type == CORE_G4MH) && (core_version >= CORE_VER_20)) {
            // Mask is depended on the input signal.
            crg_mask_g4mh2x = 0x0003FF00U;
            for (uint32_t i = 18; i < pmaddrw; i++ ) {
                crg_mask_g4mh2x |= (1 << i);
            }
            return TRegMask (crg_mask_g4mh2x);
        } else {
            // Unreachable code.
        }
        return TRegMask (0xFFFFFFFF);
    }

public:
    void Reset (void) {
        m_data.m_data32 = GADV_INIT_VAL;
    }

    // Override
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);

    RegData GetADV (void) const { return m_data.m_data32; }
    void SetADV (RegData adv) { m_data.m_data32 = adv; }

    // Override
    const char* GetName (void) const {return m_name.c_str();}
#if _UNIT_TEST
    PhysAddr GetRegAddr () {return m_reg_addr;}
#endif
    GADV (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr, uint32_t channel, std::string reg_name,
          uint32_t pmaddrw, uint32_t guard_module, CoreType core_type, CoreVersion core_version, GuardInternal_Interface *guard_internal)
    : TRegDataBase<TRegData32> (reg_size, mini_access_size, reg_addr, IsRegMask(guard_module, pmaddrw, core_type, core_version)) {
        m_name = reg_name.c_str();
        m_channel = channel;
        m_treg = &m_data;
        m_gadv_module = guard_module;
        m_data.m_data32 = GADV_INIT_VAL;
        m_guard_internal = guard_internal;
    }
};


/*!
 * @brief GIVCSPID: This is the common class for Guard IVCSPID setting register
 *                 It has 32 bit and the accessing size is 8, 16, 32bit.
 */
class GIVCSPID : public TRegDataBase<TRegData32> {
private:
    union {
        TRegData32 m_data;
        struct {
            RegData m_ivc_spid :32;
        } m_bit;
    };

    static const uint32_t GIVCSPID_INIT_VAL = 0x00000000U;

    std::string m_name;
    uint32_t m_channel;

    GuardInternal_Interface *m_guard_internal;

public:
    void Reset (void) {
        m_data.m_data32 = GIVCSPID_INIT_VAL;
    }

    // Override
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);

    RegData GetIvcSPID (void) const { return m_bit.m_ivc_spid; }
    void SetIvcSPID (RegData spid)
    {
        m_bit.m_ivc_spid = spid & 0xFFFFFFFFU;
        m_guard_internal->SetCrgSpidListValid(m_channel, m_bit.m_ivc_spid);
    }

    // Override
    const char* GetName (void) const {return m_name.c_str();}
#if _UNIT_TEST
    PhysAddr GetRegAddr () {return m_reg_addr;}
#endif
    GIVCSPID (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr, uint32_t channel, std::string reg_name, GuardInternal_Interface *guard_internal)
    : TRegDataBase<TRegData32> (reg_size, mini_access_size, reg_addr, TRegMask (GUARD_DATA_MASK_32)) {
        m_name = reg_name.c_str();
        m_channel = channel;
        m_data.m_data32 = GIVCSPID_INIT_VAL;
        m_treg = &m_data;
        m_guard_internal = guard_internal;
    }
};


/*!
 * @brief GIVCOWNR: This is the common class for Guard IVCOWNR setting register
 *                 It has 32 bit and the accessing size is 8, 16, 32bit.
 */
class GIVCOWNR : public TRegDataBase<TRegData32> {
private:
    union {
        TRegData32 m_data;
        struct {
            RegData :32;
        } m_bit;
    };

    static const uint32_t GIVCOWNR_INIT_VAL = 0x00000000U;

    std::string m_name;
    uint32_t m_channel;

    GuardInternal_Interface *m_guard_internal;

public:
    void Reset (void) {
        m_data.m_data32 = GIVCOWNR_INIT_VAL;
    }

    // Override
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause ReadBody (PhysAddr addr, uint8_t* data, ByteSize size) const;
    RegData GetSPID (void) const { return m_guard_internal->GetCrgSpidOwner(m_channel); }

    // Override
    const char* GetName (void) const {return m_name.c_str();}
#if _UNIT_TEST
    PhysAddr GetRegAddr () {return m_reg_addr;}
#endif
    GIVCOWNR (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr, uint32_t channel,
              std::string reg_name, GuardInternal_Interface *guard_internal)
    : TRegDataBase<TRegData32> (reg_size, mini_access_size, reg_addr, TRegMask (GUARD_DATA_MASK_32)) {
        m_name = reg_name.c_str();
        m_channel = channel;
        m_data.m_data32 = GIVCOWNR_INIT_VAL;
        m_treg = &m_data;
        m_guard_internal = guard_internal;
    }
};


/*!
 * @brief GIVCLOCK_Interface: Interface of GIVCLOCK.
 */
class GIVCLOCK_Interface {
public:
    virtual void SetLK (RegData spid) = 0;
    GIVCLOCK_Interface () {}
    virtual ~GIVCLOCK_Interface() {}
};


/*!
 * @brief GIVCLOCK: This is the common class for Guard IVCLOCK setting register
 *                 It has 32 bit and the accessing size is 32bit.
 */
class GIVCLOCK : public TRegDataBase<TRegData32>, public GIVCLOCK_Interface {
private:
    union {
        TRegData32 m_data;
        struct {
            RegData m_lk     :1; // Bit 0: Lock
            RegData          :31; // Bit [31, 1]: Reserve
        } m_bit;
    };

    static const uint32_t GIVCLOCK_INIT_VAL = 0x00000000U;

    std::string m_name;
    uint32_t m_channel;

    GuardInternal_Interface *m_guard_internal;

public:
    void Reset (void) {
        m_data.m_data32 = GIVCLOCK_INIT_VAL;
    }

    // Override
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);

    RegData GetLK (void) const { return m_bit.m_lk; }
    void SetLK (RegData lk) {
        m_bit.m_lk = lk & 0x1U;
        m_guard_internal->SetCrgIvcVmLock(m_channel, m_bit.m_lk);
    }

    // Override
    const char* GetName (void) const {return m_name.c_str();}
#if _UNIT_TEST
    PhysAddr GetRegAddr () {return m_reg_addr;}
#endif
    GIVCLOCK (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr, uint32_t channel,
              std::string reg_name, GuardInternal_Interface *guard_internal)
    : TRegDataBase<TRegData32> (reg_size, mini_access_size, reg_addr, TRegMask (0x1U)) {
        m_name = reg_name.c_str();
        m_channel = channel;
        m_data.m_data32 = GIVCLOCK_INIT_VAL;
        m_treg = &m_data;
        m_guard_internal = guard_internal;
    }
};


/*!
 * @brief GIVCREQ: This is the common class for Guard IVCREQ setting register
 *                It has 32 bit and the accessing size is 32bit.
 */
class GIVCREQ : public TRegDataBase<TRegData32> {
private:
    union {
        TRegData32 m_data;
        struct {
            RegData            :32; // Value is not used.
        } m_bit;
    };

    static const uint32_t GIVCREQ_INIT_VAL = 0x00000000U;

    std::string m_name;
    uint32_t m_channel;

    GuardInternal_Interface *m_guard_internal;
    GSPID_Interface* m_gspid;
    GIVCLOCK_Interface* m_givc_lock;

public:
    void Reset (void) {
        m_data.m_data32 = GIVCREQ_INIT_VAL;
    }

    // Override
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);

    // Override
    const char* GetName (void) const {return m_name.c_str();}
#if _UNIT_TEST
    PhysAddr GetRegAddr () {return m_reg_addr;}
#endif
    GIVCREQ (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr, uint32_t channel, std::string reg_name,
              GuardInternal_Interface *p_guard_internal, GSPID_Interface *p_gspid, GIVCLOCK_Interface *p_givc_lock)
    : TRegDataBase<TRegData32> (reg_size, mini_access_size, reg_addr, TRegMask (GUARD_DATA_MASK_32)) {
        m_name = reg_name.c_str();
        m_channel = channel;
        m_data.m_data32 = GIVCREQ_INIT_VAL;
        m_treg = &m_data;
        m_guard_internal = p_guard_internal;
        m_gspid = p_gspid;
        m_givc_lock = p_givc_lock;
    }
};


/*!
 * @brief GERRSTAT_Interface: Interface of GERRSTAT.
 */
class GERRSTAT_Interface {
public:
    virtual void SetOVF (RegData ovf) = 0;
    virtual void SetERR (RegData err) = 0;
    GERRSTAT_Interface () {}
    virtual ~GERRSTAT_Interface() {}
};


/*!
 * @brief GERRCLR: This is the common class for Guard error clear register.
 *                It has 32 bit and the accessing size is 8, 16, 32bit.
 */
class GERRCLR : public TRegDataBase<TRegData32> {
private:
    union {
        TRegData32 m_data;
        struct {
            RegData            :1;    // clre [0]
            RegData            :1;    // clro [1]
            RegData            :30;
        } m_bit;
    };

    static const uint32_t DATA_MASK = 0x03U;
    static const uint32_t GERRCLR_INIT_VAL = 0x00U;

    std::string m_name;
    GERRSTAT_Interface* m_gerrstat;
    CoreType m_core_type;
    CoreVersion m_core_version;

public:
    void Reset (void) {
        m_data.m_data32 = GERRCLR_INIT_VAL;
    }

    // Override
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);

    // Override
    const char* GetName (void) const {return m_name.c_str();}
#if _UNIT_TEST
    PhysAddr GetRegAddr () {return m_reg_addr;}
#endif
    GERRCLR (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr, GERRSTAT_Interface* gerrstat, std::string reg_name,
            CoreType core_type, CoreVersion core_version)
    : TRegDataBase<TRegData32> (reg_size, mini_access_size, reg_addr, TRegMask (DATA_MASK)) {
        m_name = reg_name.c_str();
        m_data.m_data32 = GERRCLR_INIT_VAL;
        m_treg = &m_data;
        m_gerrstat = gerrstat;
        m_core_type = core_type;
        m_core_version = core_version;
    }
};


/*!
 * @brief GERRSTAT: This is the common class for Guard error status register.
 *                 It has 32 bit and the accessing size is 32bit.
 */
class GERRSTAT : public TRegDataBase<TRegData32>, public GERRSTAT_Interface {
private:
    union {
        TRegData32 m_data;
        struct {
            RegData m_err      :1;
            RegData m_ovf      :1;
            RegData            :30;
        } m_bit;
    };

    static const uint32_t DATA_MASK = 0x03U;
    static const uint32_t GERRSTAT_INIT_VAL = 0x00U;

    std::string m_name;
    CoreType m_core_type;
    CoreVersion m_core_version;

public:
    void Reset (void) {
        m_data.m_data32 = GERRSTAT_INIT_VAL;
    }

    // Override
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
//    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
//    SysErrCause ReadBody (PhysAddr addr, uint8_t* data, ByteSize size) const;
    SysErrCause Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const;

    RegData GetOVF (void) const { return m_bit.m_ovf; }
    // Because OVF is Read only bit, so, this function is unreachable.
    // It is reserved function.
    void SetOVF (RegData ovf) { m_bit.m_ovf = ovf & 0x1U; }

    RegData GetERR (void) const { return m_bit.m_err; }
    // Because ERR is Read only bit, so, this function is unreachable.
    // It is reserved function.
    void SetERR (RegData err)
    {
        if ((m_core_type == CORE_G4MH) && (m_core_version < CORE_VER_20))
        {
            // This bit is removed from RH850v21.
            m_bit.m_err = err & 0x1U;
        }
    }

    // Override
    const char* GetName (void) const {return m_name.c_str();}
#if _UNIT_TEST
    PhysAddr GetRegAddr () {return m_reg_addr;}
#endif
    GERRSTAT (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr, std::string reg_name,
              CoreType core_type, CoreVersion core_version)
    : TRegDataBase<TRegData32> (reg_size, mini_access_size, reg_addr, TRegMask (DATA_MASK)) {
        m_name = reg_name.c_str();
        m_data.m_data32 = GERRSTAT_INIT_VAL;
        m_treg = &m_data;
        m_core_type = core_type;
        m_core_version = core_version;
    }
};


/*!
 * @brief GERRADDR: This is the common class for Guard error address register
 *                 It has 32 bit and the accessing size is 32bit.
 */
class GERRADDR : public TRegDataBase<TRegData32> {
private:
    union {
        TRegData32 m_data;
        struct {
            RegData m_addr      :32;
        } m_bit;
    };

    static const uint32_t GERRADDR_INIT_VAL = 0x00000000U;

    std::string m_name;
public:
    void Reset (void) {
        m_data.m_data32 = GERRADDR_INIT_VAL;
    }

    // Override
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    RegData GetAddr (void) const { return m_bit.m_addr; }
    void SetAddr (RegData addr) { m_bit.m_addr = addr & 0xFFFFFFFFU; }

    // Override
    const char* GetName (void) const {return m_name.c_str();}
#if _UNIT_TEST
    PhysAddr GetRegAddr () {return m_reg_addr;}
#endif
    GERRADDR (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr, std::string reg_name)
    : TRegDataBase<TRegData32> (reg_size, mini_access_size, reg_addr, TRegMask (GUARD_DATA_MASK_32)) {
        m_name = reg_name.c_str();
        m_data.m_data32 = GERRADDR_INIT_VAL;
        m_treg = &m_data;
    }
};


/*!
 * @brief GERRTYPE: This is the common class for Guard error access information register.
 *                 It has 32 bit and the accessing size is 32bit.
 */
class GERRTYPE : public TRegDataBase<TRegData32> {
private:
    union {
        TRegData32 m_data;
        struct {
            RegData m_write     :1;
            RegData             :5;
            RegData m_spid      :5;
            RegData m_um        :1;
            RegData m_dbg       :1;
            RegData m_sec       :1;
            RegData             :18;
        } m_bit;
    };

    static const uint32_t GERRTYPE_INIT_VAL = 0x00000000U;

    std::string m_name;
public:
    void Reset (void) {
        m_data.m_data32 = GERRTYPE_INIT_VAL;
    }

    // Override
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    RegData GetWrite (void) const { return m_bit.m_write; }
    // Because Write is Read only bit, so, this function is unreachable.
    // It is reserved function.
    void SetWrite (RegData is_write) { m_bit.m_write = is_write & 0x1U; }

    RegData GetSPID (void) const { return m_bit.m_spid; }
    // Because SPID is Read only bit, so, this function is unreachable.
    // It is reserved function.
    void SetSPID (RegData spid) { m_bit.m_spid = spid & SPID_MASK; }

    RegData GetUM   (void) const { return m_bit.m_um; }
    // Because UM is Read only bit, so, this function is unreachable.
    // It is reserved function.
    void SetUM   (RegData um)   { m_bit.m_um = um & 0x1U; }

    RegData GetDBG  (void) const { return m_bit.m_dbg; }
    // Because DBG is Read only bit, so, this function is unreachable.
    // It is reserved function.
    void SetDBG  (RegData dbg)  { m_bit.m_dbg = dbg & 0x1U; }

    RegData GetSEC  (void) const { return m_bit.m_sec; }
    // Because SEC is Read only bit, so, this function is unreachable.
    // It is reserved function.
    void SetSEC  (RegData sec)  { m_bit.m_sec = sec & 0x1U; }

    // Override
    const char* GetName (void) const {return m_name.c_str();}
#if _UNIT_TEST
    PhysAddr GetRegAddr () {return m_reg_addr;}
#endif
    GERRTYPE (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr, std::string reg_name)
    : TRegDataBase<TRegData32> (reg_size, mini_access_size, reg_addr, TRegMask (GUARD_DATA_MASK_32)) {
        m_name = reg_name.c_str();
        m_data.m_data32 = GERRTYPE_INIT_VAL;
        m_treg = &m_data;
    }
};


/*!
 * @brief GSPIDERRSTAT_Interface: Interface of GSPIDERRSTAT.
 */
class GSPIDERRSTAT_Interface {
public:
    virtual void ClrSPIDERR (RegData spid_list) = 0;
    virtual const char* GetName(void) const = 0;
    GSPIDERRSTAT_Interface () {}
    virtual ~GSPIDERRSTAT_Interface() {}
};


/*!
 * @brief GSPIDERRSTAT: This is the common class for containing SPID when Guard error is occurred.
 *                     It has 32 bit and the accessing size is 8, 16, 32bit.
 */
class GSPIDERRSTAT : public TRegDataBase<TRegData32>, public GSPIDERRSTAT_Interface {
private:
    union {
        TRegData32 m_data;
        struct {
            RegData m_spid :32;
        } m_bit;
    };
    // This register is only available from G4MH2x.
    static const uint32_t GERRSTAT_INIT_VAL_G4MH2x = 0x00U;

    std::string m_name;
    CoreType m_core_type;
    CoreVersion m_core_version;

public:
    void Reset (void) {
        m_data.m_data32 = GERRSTAT_INIT_VAL_G4MH2x;
    }

    // Override
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
//    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
//    SysErrCause ReadBody (PhysAddr addr, uint8_t* data, ByteSize size) const;
//    SysErrCause Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const;

    RegData GetSPIDERR (void) const { return m_bit.m_spid; }
    void SetSPIDERR (RegData spid) { m_bit.m_spid |= (1 << spid); }
    void ClrSPIDERR (RegData spid_list) { m_bit.m_spid &= spid_list; }

    // Override
    const char* GetName (void) const {return m_name.c_str();}
#if _UNIT_TEST
    PhysAddr GetRegAddr () {return m_reg_addr;}
#endif
    GSPIDERRSTAT (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr, std::string reg_name,
              CoreType core_type, CoreVersion core_version)
    : TRegDataBase<TRegData32> (reg_size, mini_access_size, reg_addr, TRegMask (GUARD_DATA_MASK_32)) {
        m_name = reg_name.c_str();
        m_data.m_data32 = GERRSTAT_INIT_VAL_G4MH2x;
        m_treg = &m_data;
        m_core_type = core_type;
        m_core_version = core_version;
    }
};


/*!
 * @brief GSPIDERRCLR: This is the common class for Guard error clear register.
 *                    It has 32 bit and the accessing size is 8, 16, 32bit.
 */
class GSPIDERRCLR : public TRegDataBase<TRegData32> {
private:
    union {
        TRegData32 m_data;
        struct {
            RegData m_spid :32;
        } m_bit;
    };

    static const uint32_t GERRCLR_INIT_VAL_G4MH2x = 0x00U;

    std::string m_name;
    GSPIDERRSTAT_Interface* m_gspiderrstat;
    CoreType m_core_type;
    CoreVersion m_core_version;

public:
    void Reset (void) {
        m_data.m_data32 = GERRCLR_INIT_VAL_G4MH2x;
    }

    // Override
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);

    // Override
    const char* GetName (void) const {return m_name.c_str();}
#if _UNIT_TEST
    PhysAddr GetRegAddr () {return m_reg_addr;}
#endif
    GSPIDERRCLR (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr, GSPIDERRSTAT_Interface* gspiderrstat, std::string reg_name,
            CoreType core_type, CoreVersion core_version)
    : TRegDataBase<TRegData32> (reg_size, mini_access_size, reg_addr, TRegMask (GUARD_DATA_MASK_32)) {
        m_name = reg_name.c_str();
        m_data.m_data32 = GERRCLR_INIT_VAL_G4MH2x;
        m_treg = &m_data;
        m_gspiderrstat = gspiderrstat;
        m_core_type = core_type;
        m_core_version = core_version;
    }
};
