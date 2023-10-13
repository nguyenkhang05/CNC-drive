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
#include "rh850g4/analysis_interface_info.h"
#include "rh850g4/execution_plugin.h"

// Standard header files
#include <list>
#include <map>

//------------------------------------------------------------------------------
// Data structures
//------------------------------------------------------------------------------

class ExecutionPluginCallbackInfo
{
public:
  pc_exec_callback_func_t m_exec_callback;
  pc_query_callback_func_t m_query_callback;
  void *m_analysis_context;
  AnalysisInterfaceInfo *m_analysis_interface_info;
  rh850g4::core_base_t *m_core;
};

class ExecutionPluginInfo
{
public:
  rh850g4::core_base_t *m_core;
  std::list<ExecutionPluginCallbackInfo *> m_callback_info;
  std::list<rh850g4::analysis_attachment_descriptor *> m_attachments;
};

//------------------------------------------------------------------------------
// Execution plug-in instance map - maps analysis_context instances to plug-in
// instances.
//------------------------------------------------------------------------------

std::map<void *, ExecutionPluginInfo> execution_plugin_info_map;

//------------------------------------------------------------------------------
// Execution plug-in internal functions
//------------------------------------------------------------------------------

void execution_callback(ExecutionPluginCallbackInfo *epcbi)
{
  unsigned long long PC_val = (unsigned long long) epcbi->m_core->read_PC();
  epcbi->m_exec_callback(epcbi->m_analysis_context, PC_val);
}

rh850g4::analysis_predicate_response execution_predicate(ExecutionPluginCallbackInfo *epcbi)
{
  AnalysisInterfaceInfo *aii =(AnalysisInterfaceInfo *)epcbi->m_analysis_interface_info;
  rh850g4::core_base_t *core = epcbi->m_core;
  rh850g4::analysis_predicate_response ret_val;
  
  if (core->is_decode_valid())
  {
    unsigned int PC_val = core->get_decode_PC();
 
    if (epcbi->m_query_callback(epcbi->m_analysis_context, PC_val))
    {
      ret_val.set_analysis_callback(aii->m_execute_callback, epcbi);
    }
  }
  return ret_val;
}

//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------

extern "C" bool add_execution_cb(
    void *core_context,
    void *analysis_context,
    pc_exec_callback_func_t pc_exec_func,
    pc_query_callback_func_t pc_query_func)
{
  if (analysis_context && core_context && pc_exec_func && pc_query_func)
  {
    // Get analysis interface
    AnalysisInterfaceInfo *aii =(AnalysisInterfaceInfo *) core_context;
    rh850g4::core_base_t *core = aii->m_core;
    
    // Create and initialise analysis context
    ExecutionPluginInfo *epi = &execution_plugin_info_map[analysis_context];
    epi->m_core = core;

    // Create callback information
    ExecutionPluginCallbackInfo *epcbi = new ExecutionPluginCallbackInfo();
    epcbi->m_exec_callback = pc_exec_func;
    epcbi->m_query_callback = pc_query_func;
    epcbi->m_analysis_context = analysis_context;
    epcbi->m_analysis_interface_info = aii;
    epcbi->m_core = core;
    epi->m_callback_info.push_back(epcbi);    
    
    // Attach to post-decode analysis point
    epi->m_attachments.push_back(core->attach("post_decode", (rh850g4::analysis_predicate)execution_predicate, epcbi));
  }
  
  return true;
}

extern "C" bool remove_execution_cb(void *analysis_context)
{
  if (analysis_context)
  {
    // Get analysis context information
    ExecutionPluginInfo *epi = &execution_plugin_info_map[analysis_context];
    
    // Detach attachments
    for (std::list<rh850g4::analysis_attachment_descriptor *>::iterator att_it = epi->m_attachments.begin(); att_it != epi->m_attachments.end(); ++att_it )
    {
      epi->m_core->detach(*att_it);
    }
    epi->m_attachments.clear();
    
    // Delete callback information 
    for (std::list<ExecutionPluginCallbackInfo *>::iterator epcbi_it = epi->m_callback_info.begin(); epcbi_it != epi->m_callback_info.end(); ++epcbi_it )
    {
      delete *epcbi_it;
    }
    epi->m_callback_info.clear();
  }
  
  return true;
}

//------------------------------------------------------------------------------
// Analysis interface internal operations
//------------------------------------------------------------------------------

void initialise_execution_plugin(AnalysisInterfaceInfo *aii)
{
  aii->m_execute_callback = aii->m_core->register_analysis_callback((rh850g4::analysis_callback) execution_callback);
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
