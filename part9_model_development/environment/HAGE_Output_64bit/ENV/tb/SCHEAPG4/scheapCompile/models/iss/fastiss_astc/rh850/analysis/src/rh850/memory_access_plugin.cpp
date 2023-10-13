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

// Core runtime header files
#include "rh850/analysis_types.h"

// Analysis interface header files
#include "rh850/analysis_interface_info.h"
#include "rh850/memory_access_plugin.h"

// Standard header files
#include <list>
#include <map>

//------------------------------------------------------------------------------
// Data structures
//------------------------------------------------------------------------------

class MemoryAccessPluginCallbackInfo
{
public:
  memory_access_callback_func_t m_callback;
  void *m_analysis_context;
  AnalysisInterfaceInfo *m_analysis_interface_info;
  rh850::core_base_t *m_core;
};

class MemoryAccessPluginInfo
{
public:
  rh850::core_base_t *m_core;
  std::list<MemoryAccessPluginCallbackInfo *> m_callback_info;
  std::list<rh850::analysis_attachment_descriptor *> m_attachments;
};

//------------------------------------------------------------------------------
// Memory access plug-in instance map - maps analysis_context instances to
// plug-in instances.
//------------------------------------------------------------------------------

std::map<void *, MemoryAccessPluginInfo> memory_access_plugin_info_map;

//------------------------------------------------------------------------------
// Memory access plug-in internal functions
//------------------------------------------------------------------------------

void memory_access_callback(MemoryAccessPluginCallbackInfo *mapcbi)
{
  unsigned long long address = mapcbi->m_core->read_last_access_addr();
  size_t size = mapcbi->m_core->read_last_access_size();
  mapcbi->m_callback(mapcbi->m_analysis_context, address, size);
}

rh850::analysis_predicate_response memory_access_predicate(MemoryAccessPluginCallbackInfo *mapcbi)
{
  AnalysisInterfaceInfo *aii =(AnalysisInterfaceInfo *)mapcbi->m_analysis_interface_info;
  rh850::analysis_predicate_response ret_val;
  ret_val.set_analysis_callback(aii->m_memory_access_callback, mapcbi);
  return ret_val;
}

//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------

extern "C" bool add_memory_access_cb(
  void* core_context,
  void* analysis_context,
  memory_access_callback_func_t write_func,
  memory_access_callback_func_t read_func)
{
  if (analysis_context && core_context)
  {
    // Get analysis interface
    AnalysisInterfaceInfo *aii =(AnalysisInterfaceInfo *) core_context;
    rh850::core_base_t *core = aii->m_core;
    
    // Create and initialise analysis context
    MemoryAccessPluginInfo *mapi = &memory_access_plugin_info_map[analysis_context];
    mapi->m_core = core;
    
    // Attach to load analysis points
    if (read_func)
    {
      // Create callback information
      MemoryAccessPluginCallbackInfo *load_info = new MemoryAccessPluginCallbackInfo();
      load_info->m_callback = read_func;
      load_info->m_analysis_context = analysis_context;
      load_info->m_analysis_interface_info = aii;
      load_info->m_core = core;
      mapi->m_callback_info.push_back(load_info);
      
      // Attach to load analysis points
      mapi->m_attachments.push_back(core->attach("immediate_load", (rh850::analysis_callback)memory_access_callback, load_info));
      mapi->m_attachments.push_back(core->attach("generative_load", (rh850::analysis_predicate)memory_access_predicate, load_info));
    }

    // Attach to store analysis points
    if (write_func)
    {
      // Create callback information
      MemoryAccessPluginCallbackInfo *store_info = new MemoryAccessPluginCallbackInfo();
      store_info->m_callback = write_func;
      store_info->m_analysis_context = analysis_context;
      store_info->m_analysis_interface_info = aii;
      store_info->m_core = core;
      mapi->m_callback_info.push_back(store_info);
      
      // Attach to store analysis points
      mapi->m_attachments.push_back(core->attach("immediate_store", (rh850::analysis_callback)memory_access_callback, store_info));
      mapi->m_attachments.push_back(core->attach("generative_store", (rh850::analysis_predicate)memory_access_predicate, store_info));
    }
  }
  
  return true;
}

extern "C" bool remove_memory_access_cb(
  void* analysis_context)
{
  if (analysis_context)
  {
    // Get analysis context information
    MemoryAccessPluginInfo *mapi = &memory_access_plugin_info_map[analysis_context];

    // Detach attachments
    for (std::list<rh850::analysis_attachment_descriptor *>::iterator att_it = mapi->m_attachments.begin(); att_it != mapi->m_attachments.end(); ++att_it )
    {
      mapi->m_core->detach(*att_it);
    }
    mapi->m_attachments.clear();
    
    // Delete callback information 
    for (std::list<MemoryAccessPluginCallbackInfo *>::iterator mapcbi_it = mapi->m_callback_info.begin(); mapcbi_it != mapi->m_callback_info.end(); ++mapcbi_it )
    {
      delete *mapcbi_it;
    }
    mapi->m_callback_info.clear();
  }
  
  return true;
}

//------------------------------------------------------------------------------
// Analysis interface internal operations
//------------------------------------------------------------------------------

void initialise_memory_access_plugin(AnalysisInterfaceInfo *aii)
{
  aii->m_memory_access_callback = aii->m_core->register_analysis_callback((rh850::analysis_callback) memory_access_callback);
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
