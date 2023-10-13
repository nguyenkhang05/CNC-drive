/*************************************************************************
 *
 * Copyright(c) 2015-2016 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2015.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file memory_wrapper.h
 * \brief Defines the memory wrapper class
 */

#ifndef _RH850G4_MEMORY_WRAPPER_H_
#define _RH850G4_MEMORY_WRAPPER_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <list>
#include <utility>
#include "systemc.h"
#include "tlm.h"

//------------------------------------------------------------------------------
// Forward declarations
//------------------------------------------------------------------------------

namespace vworks
{
  namespace genesis
  {
    class memory_block;
  }

  namespace components
  {
    template <unsigned int BUSWIDTH>
    class tlm_memory;
    template <unsigned int BUSWIDTH>
    class tlm_router;
  }
}

namespace rh850g4
{
  class tlm_memory_double_port;
}

//------------------------------------------------------------------------------
// Memory wrapper definition
//------------------------------------------------------------------------------

namespace rh850g4
{    
  class memory_wrapper : public sc_core::sc_module
  {
  public:
    /**
     * \brief Memory wrapper class constructor
     * \param name Name of the ISS in the SystemC platform
     */
    memory_wrapper(sc_core::sc_module_name name);

    /**
     * \brief Memory wrapper class destructor
     */
    ~memory_wrapper();

    /**
     * \brief Memory wrapper class configuration
     */
    void configure(unsigned int ram_size,
              unsigned int ram_start_address,
              unsigned int rom_size,
              unsigned int rom_start_address,
              unsigned int pe_local_ram_start);

    /**
     * \brief TLM bus initiator
     */
    tlm::tlm_initiator_socket< 32,tlm::tlm_base_protocol_types,0 > bus_initiator;
    /**
     * \brief TLM bus target
     */
    tlm::tlm_target_socket< 32 > fastiss_bus_target;
    /**
     * \brief TLM bus target
     */
    tlm::tlm_target_socket< 32,tlm::tlm_base_protocol_types,0 > external_bus_target;
    /**
     * \brief Set RAM latencies
     */
    void set_ram_latencies(const sc_core::sc_time &read_latency, 
                           const sc_core::sc_time &write_latency,
                           const sc_core::sc_time &fixed_latency);
    
  private:
    /**
     * \brief Memory block for the core RAM.
     */
    tlm_memory_double_port* m_ram;
    /**
     * \brief Memory block for the core ROM.
     */
    vworks::components::tlm_memory<32>* m_rom;
    /**
     * \brief Memory block for the core ROM.
     */
    vworks::components::tlm_router<32>* m_router;
    
    std::list<std::pair<unsigned int, unsigned int> > calculate_unmapped_regions(unsigned int region_start, unsigned int region_end, std::list<std::pair<unsigned int, unsigned int> > unmapped_regions);
  };
}

#endif // _RH850G4_MEMORY_WRAPPER_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------

  
