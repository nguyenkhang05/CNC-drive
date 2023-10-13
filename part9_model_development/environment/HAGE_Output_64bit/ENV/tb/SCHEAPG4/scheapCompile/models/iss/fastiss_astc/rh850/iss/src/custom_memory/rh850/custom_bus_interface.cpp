//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2019.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "fastiss/code_gen_if.h"
#include "rh850/custom_bus_interface.h"

//------------------------------------------------------------------------------
// Bus interface implementation
//------------------------------------------------------------------------------

rh850::custom_bus_interface_t::custom_bus_interface_t(const std::string& name) :
  rh850::bus_interface_t(name),
  m_debug_payload(),
  m_fetch_payload(),
  m_dmi_payload(),
  m_payload()
{
  m_fetch_payload.set_command(tlm::TLM_READ_COMMAND);
  m_debug_payload.set_mm(this);
  m_fetch_payload.set_mm(this);
  m_dmi_payload.set_mm(this);
  m_payload.set_mm(this);
}

rh850::custom_bus_interface_t::~custom_bus_interface_t()
{
}

tlm::tlm_generic_payload& rh850::custom_bus_interface_t::get_debug_payload(tlm::tlm_command command, sc_dt::uint64 address, unsigned int size, unsigned char *data)
{
  m_debug_payload.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
  m_debug_payload.set_command(command);
  m_debug_payload.set_address(address);
  m_debug_payload.set_streaming_width(size);
  m_debug_payload.set_data_length(size);
  m_debug_payload.set_data_ptr(data);
  return m_debug_payload;
}

tlm::tlm_generic_payload& rh850::custom_bus_interface_t::get_fetch_payload(sc_dt::uint64 address, unsigned int size, unsigned char *data)
{
  m_fetch_payload.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
  m_fetch_payload.set_address(address);
  m_fetch_payload.set_streaming_width(size);
  m_fetch_payload.set_data_length(size);
  m_fetch_payload.set_data_ptr(data);
  return m_fetch_payload;
}

tlm::tlm_generic_payload& rh850::custom_bus_interface_t::get_dmi_payload(tlm::tlm_command command, sc_dt::uint64 address, unsigned int size, unsigned char *data)
{
  m_dmi_payload.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
  m_dmi_payload.set_command(command);
  m_dmi_payload.set_address(address);
  m_dmi_payload.set_streaming_width(size);
  m_dmi_payload.set_data_length(size);
  m_dmi_payload.set_data_ptr(data);
  return m_dmi_payload;
}

tlm::tlm_generic_payload& rh850::custom_bus_interface_t::get_payload(tlm::tlm_command command, sc_dt::uint64 address, unsigned int size, unsigned char *data)
{
  m_payload.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
  m_payload.set_command(command);
  m_payload.set_address(address);
  m_payload.set_streaming_width(size);
  m_payload.set_data_length(size);
  m_payload.set_data_ptr(data);
  return m_payload;
}

void rh850::custom_bus_interface_t::debug_completed(unsigned int bytes_transferred)
{
}

void rh850::custom_bus_interface_t::fetch_completed(unsigned int bytes_transferred)
{
}

void rh850::custom_bus_interface_t::dmi_completed(void)
{
}

void rh850::custom_bus_interface_t::transaction_completed(void)
{
}

void rh850::custom_bus_interface_t::dmi_pointer_post_callout(rh850::access_type_t type, dmi_region *val)
{
  val->eff_start_address -= val->physical_address_requested - val->dmi.get_start_address();
  val->eff_end_address = val->eff_start_address + (val->dmi.get_end_address() - val->dmi.get_start_address());
  _dmi_read_regions.insert(*val);
  _dmi_write_regions.insert(*val);
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
