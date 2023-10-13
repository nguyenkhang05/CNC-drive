// ---------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company (ASTC). 2012.
// All Rightarget_socket Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence an
// actual or intended publication of such source code.
// ---------------------------------------------------------------------------

#ifndef __TLM_SHORTCUT__
#define __TLM_SHORTCUT__

#include "systemc.h"
#include "tlm.h"
#include <boost/unordered_map.hpp>

using namespace sc_core;

class TlmShortcutExtension : public tlm::tlm_extension<TlmShortcutExtension>
{
public:
    TlmShortcutExtension(void) : 
        fw_if(NULL),
        address_offset(0),
        valid(false)
    {
    }

    ~TlmShortcutExtension(void)
    {
    }

    tlm::tlm_extension_base* clone(void) const
    {
        TlmShortcutExtension *t = new TlmShortcutExtension;
        t->fw_if = this->fw_if;
        t->address_offset = this->address_offset;
        t->valid = this->valid;
        return t;
    }

    void copy_from(const tlm::tlm_extension_base &extension)
    {
        fw_if = static_cast<TlmShortcutExtension const &>(extension).fw_if;
        address_offset = static_cast<TlmShortcutExtension const &>(extension).address_offset;
        valid = static_cast<TlmShortcutExtension const &>(extension).valid;
    }

public:
    tlm::tlm_fw_transport_if<>* fw_if;
    unsigned int address_offset;
    bool valid;
};


class TLMShortcut : public sc_module,
    tlm::tlm_fw_transport_if<>,
    tlm::tlm_bw_transport_if<>
{
public:
    TLMShortcut(sc_module_name name);
    ~TLMShortcut();

    tlm::tlm_initiator_socket<32> initiator_socket;
    tlm::tlm_target_socket<32> target_socket;
    
    
    virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& t);
    virtual void b_transport( tlm::tlm_generic_payload& trans, sc_time& delay);
    virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data);
    virtual unsigned int transport_dbg(tlm::tlm_generic_payload& trans);
    
    virtual tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& t);
    virtual void invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range);
    
    void end_of_simulation();
    
    void invalidate_shortcuts();
    
private:
    struct AccessRecord
    {
        unsigned int size;
        sc_core::sc_time delay;
        tlm::tlm_fw_transport_if<>* fw_if;
        unsigned int address_offset;
    };
    
    boost::unordered_map<unsigned int, AccessRecord> read_addresses;
    boost::unordered_map<unsigned int, AccessRecord> write_addresses;
    boost::unordered_map<unsigned int, AccessRecord> ignore_addresses;
    boost::unordered_map<unsigned int, AccessRecord>* read_write_map_array[3];
    
    
    int last_read_size;
    int last_write_size;
};



#endif // __TLM_SHORTCUT__
