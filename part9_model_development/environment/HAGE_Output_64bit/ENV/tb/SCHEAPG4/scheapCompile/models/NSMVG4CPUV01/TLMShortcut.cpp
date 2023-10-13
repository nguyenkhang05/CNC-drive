#include "TLMShortcut.h"

TLMShortcut::TLMShortcut(sc_module_name name):
    sc_module(name),
    initiator_socket("initiator_socket"),
    target_socket("target_socket"),
    last_read_size(0),
    last_write_size(0)
{
    initiator_socket.bind(*this);
    target_socket.bind(*this);
    
    read_write_map_array[0] = &read_addresses;
    read_write_map_array[1] = &write_addresses;
    read_write_map_array[2] = &ignore_addresses;
}

TLMShortcut::~TLMShortcut()
{
}

void TLMShortcut::end_of_simulation()
{
    //printf("read_addresses.size() == %d, write_addresses.size() == %d\n",
    //            read_addresses.size(), write_addresses.size());
}

void TLMShortcut::invalidate_shortcuts()
{
    read_addresses.clear();
    write_addresses.clear();
}

tlm::tlm_sync_enum TLMShortcut::nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& t)
{
    return initiator_socket->nb_transport_fw(trans, phase, t);
}

void TLMShortcut::b_transport(tlm::tlm_generic_payload& trans, sc_time& delay)
{
    unsigned int addr = (unsigned int)trans.get_address();
    boost::unordered_map<unsigned int, AccessRecord>::iterator iter;
    tlm::tlm_command command = trans.get_command();
    boost::unordered_map<unsigned int, AccessRecord>* address_map = read_write_map_array[command];
    
    iter = address_map->find(addr);
    if (iter != address_map->end())
    {
        // Get the existing access record
        AccessRecord& access_record = iter->second;
        if (access_record.size == trans.get_data_length())
        {
            // Shortcut the access
            trans.set_address(addr - access_record.address_offset);
            trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
            (access_record.fw_if)->b_transport(trans, delay);
            delay = access_record.delay;
            return;
        }
    }
    
    TlmShortcutExtension* extension = NULL;
    trans.get_extension(extension);
    
    if (extension == NULL)
    {
        extension = new TlmShortcutExtension;
        trans.set_extension(extension);
    }
    extension->valid = false;
    
    initiator_socket->b_transport(trans, delay);
    
    if(extension->valid)
    {
        // Create an access record for next time
        AccessRecord access_record;
        access_record.size = trans.get_data_length();
        access_record.delay = delay;
        access_record.fw_if = extension->fw_if;
        access_record.address_offset = extension->address_offset;
        
       (*address_map)[addr] = access_record;
    }
}

bool TLMShortcut::get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data)
{
    return initiator_socket->get_direct_mem_ptr(trans, dmi_data);
}

unsigned int TLMShortcut::transport_dbg(tlm::tlm_generic_payload& trans)
{
    return initiator_socket->transport_dbg(trans);
}

tlm::tlm_sync_enum TLMShortcut::nb_transport_bw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& t)
{
    return target_socket->nb_transport_bw(trans, phase, t);
}

void TLMShortcut::invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range)
{
    target_socket->invalidate_direct_mem_ptr(start_range, end_range);
}