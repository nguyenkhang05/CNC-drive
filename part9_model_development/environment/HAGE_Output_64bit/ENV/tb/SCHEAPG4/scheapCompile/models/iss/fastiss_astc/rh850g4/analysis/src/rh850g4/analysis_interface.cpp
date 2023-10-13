//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2017.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

// Core runtime header files
#include "rh850g4/analysis_types.h"

// Analysis interface header files
#include "rh850g4/analysis_interface.h"
#include "rh850g4/analysis_interface_info.h"
#include "rh850g4/execution_plugin.h"
#include "rh850g4/memory_access_plugin.h"
#include "rh850g4/function_call_plugin.h"

// Standard header files
#include <map>

//------------------------------------------------------------------------------
// Analysis interface instance map - maps core instances to
// AnalysisInterfaceInfo data structures
//------------------------------------------------------------------------------

std::map<rh850g4::core_base_t *, AnalysisInterfaceInfo> analysis_interface_info_map;

//------------------------------------------------------------------------------
// Public analysis interface operations
//------------------------------------------------------------------------------

extern "C" bool read_memory(
    void* core_context,
    unsigned long long address,
    unsigned int size,
    unsigned char* buf)
{
  if (core_context)
  {
    // Get analysis interface
    AnalysisInterfaceInfo *aii =(AnalysisInterfaceInfo *) core_context;
    aii->m_core->iN_load((unsigned int)address, size, buf);
  }
  return true;
}

extern "C" bool invalidate(void *core_context)
{
  if (core_context)
  {
    // Get analysis interface
    AnalysisInterfaceInfo *aii =(AnalysisInterfaceInfo *) core_context;
    aii->m_core->flush_cache();
  }
  return false;
}

extern "C" bool get_local_time_offset(
  void* core_context,
  unsigned long long* time_offset)
{
  if (core_context)
  {
    // Get analysis interface
    AnalysisInterfaceInfo *aii =(AnalysisInterfaceInfo *) core_context;
    sc_core::sc_time lt = aii->m_core->get_local_time();
    *time_offset = lt.value();
  }
  return true;
}

CoreAnalysisInterface *create_analysis_interface(rh850g4::core_base_t *core)
{
  if (core)
  {
    if (analysis_interface_info_map.count(core) == 0)
    {
      // Create analysis interface
      AnalysisInterfaceInfo *aii = &(analysis_interface_info_map[core]);
          
      // Initialise core pointer
      aii->m_core = core;
      
      // Initialise analysis interface
      aii->m_itf.core_context = aii;
      
      aii->m_itf.add_execution_cb = add_execution_cb;
      aii->m_itf.remove_execution_cb = remove_execution_cb;
      aii->m_itf.add_func_call_cb = add_func_call_cb;
      aii->m_itf.remove_func_call_cb = remove_func_call_cb;
      aii->m_itf.invalidate = invalidate;
      aii->m_itf.add_memory_access_cb = add_memory_access_cb;
      aii->m_itf.remove_memory_access_cb = remove_memory_access_cb;
      aii->m_itf.read_memory = read_memory;      
      aii->m_itf.get_local_time_offset = get_local_time_offset;
      
      // Register callbacks with the core
      initialise_execution_plugin(aii);
      initialise_memory_access_plugin(aii);
      initialise_function_call_plugin(aii);
      
      return &(aii->m_itf);
    }
    return &(analysis_interface_info_map[core].m_itf);
  }
  return NULL;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
