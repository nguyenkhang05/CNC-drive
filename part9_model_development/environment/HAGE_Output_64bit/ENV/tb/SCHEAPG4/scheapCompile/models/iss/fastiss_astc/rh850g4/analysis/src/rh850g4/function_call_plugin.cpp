//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2014.
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
#include <vector>

//------------------------------------------------------------------------------
// Data structures
//------------------------------------------------------------------------------

class FunctionCallPluginCallbackInfo;

class FunctionCallSymbol
{
public: 
  caif_symbol_t *m_interface_sym; 
  FunctionCallPluginCallbackInfo *m_callback_info; 
  
  FunctionCallSymbol(caif_symbol_t *sym = NULL, FunctionCallPluginCallbackInfo *callback_info = NULL) :
    m_interface_sym(sym),
    m_callback_info(callback_info)
  {
  }
  
  // Compare two stack entries based on their addresses
  inline bool iseq(FunctionCallSymbol *rsym)
  { 
    return (rsym && rsym->m_interface_sym && m_interface_sym && (rsym->m_interface_sym->address == m_interface_sym->address)); 
  } 
};

class FunctionCallStackEntry : public FunctionCallSymbol
{
public:
  unsigned long long m_return_addr; 
  
  FunctionCallStackEntry() :
    FunctionCallSymbol(),
    m_return_addr(0)
  {
  }
  
  FunctionCallStackEntry(caif_symbol_t *sym, unsigned long long return_addr, FunctionCallPluginCallbackInfo *callback_info = NULL) :
    FunctionCallSymbol(sym, callback_info),
    m_return_addr(return_addr) 
  {
  }
}; 

class FunctionCallPluginCallbackInfo
{
public:
  func_callback_func_t m_call_callback;
  func_callback_func_t m_return_callback;
  func_query_callback_func_t m_query_callback;
  void *m_analysis_context;
  AnalysisInterfaceInfo *m_analysis_interface_info;
  rh850g4::core_base_t *m_core;
  
  // Constructor
  FunctionCallPluginCallbackInfo();
  
  // Predicate handlers
  rh850g4::analysis_predicate_response post_decode_predicate_handler(void); 

  // Callback handlers
  void start_symbol_callback_handler(FunctionCallSymbol *fcs); 
  void known_symbol_callback_handler(FunctionCallSymbol *fcs); 
  void unknown_symbol_callback_handler(void); 
  
  // Utility routines
  FunctionCallSymbol* trans_lookup_symbol(unsigned long long addr);
  unsigned long long get_function_return_address(void); 

  // Map of symbol objects - used to ensure we can pass pointers to symbols
  // around without worrying about new'ing and/or deleting them. 
  std::map<unsigned long long, FunctionCallSymbol> m_symbol_info; 
  
  // Call stack
  std::vector<FunctionCallStackEntry> m_call_stack;

  // Handle to last translated symbol (note may be NULL). 
  FunctionCallSymbol *m_last_trans_symbol; 

  // Handle to last symbol to be executed (note may be NULL). 
  FunctionCallSymbol *m_last_symbol; 

  // Flag indicating first pc in the translated block. 
  bool m_first_trans_symbol; 
  
  // Flag indicating that an IRQ has been accepted
  bool m_irq_accepted;
};

class FunctionCallPluginInfo
{
public:
  rh850g4::core_base_t *m_core;
  std::list<FunctionCallPluginCallbackInfo *> m_callback_info;
  std::list<rh850g4::analysis_attachment_descriptor *> m_attachments;    
};

//------------------------------------------------------------------------------
// Function call plug-in instance map - maps analysis_context instances to
// plug-in instances.
//------------------------------------------------------------------------------

std::map<void *, FunctionCallPluginInfo> function_call_plugin_info_map;

//------------------------------------------------------------------------------
// Function call plug-in internal functions
//------------------------------------------------------------------------------

FunctionCallPluginCallbackInfo::FunctionCallPluginCallbackInfo() :
  m_call_callback(NULL),
  m_return_callback(NULL),
  m_query_callback(NULL),
  m_analysis_context(NULL),
  m_analysis_interface_info(NULL),
  m_core(NULL),
  m_symbol_info(), 
  m_call_stack(),
  m_last_trans_symbol(NULL),
  m_last_symbol(NULL),
  m_first_trans_symbol(true),
  m_irq_accepted(false)
{
}

// Start symbol callback
void start_symbol_callback(FunctionCallSymbol *fcs)
{
  fcs->m_callback_info->start_symbol_callback_handler(fcs); 
}

void FunctionCallPluginCallbackInfo::start_symbol_callback_handler(FunctionCallSymbol *fcs)
{
    unsigned long long ret_addr = get_function_return_address(); 
    FunctionCallStackEntry fcse(fcs->m_interface_sym, ret_addr); 
    m_call_stack.push_back(fcse); 
    if (m_last_symbol)
    { 
      // Now activate the external callback 
      (*m_call_callback)(m_analysis_context, fcs->m_interface_sym, m_last_symbol->m_interface_sym); 
    }
    else
    { 
      // Now activate the external callback 
      (*m_call_callback)(m_analysis_context, fcs->m_interface_sym, NULL); 
    } 
    m_last_symbol = fcs; 
    
    // Clear any notifications of interrupt acceptance
    m_irq_accepted = false;
}

// Known symbol callback
void known_symbol_callback(FunctionCallSymbol *fcs)
{
  fcs->m_callback_info->known_symbol_callback_handler(fcs);   
}

void FunctionCallPluginCallbackInfo::known_symbol_callback_handler(FunctionCallSymbol *fcs)
{
  if (!m_call_stack.empty())
  { 
    unsigned long long PC_val = (unsigned long long) m_core->read_PC();
    FunctionCallStackEntry cslast = m_call_stack.back(); 
    if (PC_val == cslast.m_return_addr)
    { 
      // PC matches the return address from the last stack entry, 
      // so this is a function return. 
      (*m_return_callback)(m_analysis_context, fcs->m_interface_sym, cslast.m_interface_sym); 
      m_call_stack.pop_back(); 
      // In some circumstances, we may need to exit multiple functions for the
      // one PC value - e.g. on multiple ISR exits. As long as the return
      // address of functions on the stack match we continue to call the exit
      // function callback, until the call stack matches the current symbol. 
      while (!m_call_stack.empty() && (PC_val == m_call_stack.back().m_return_addr))
      { 
        cslast = m_call_stack.back(); 
        if (fcs->iseq(&cslast))
        { 
          break; 
        } 
        // Keep going on the stack until the return address or symbol doesn't match 
        (*m_return_callback)(m_analysis_context, fcs->m_interface_sym, cslast.m_interface_sym); 
        m_call_stack.pop_back(); 
      } 
    } 
  } 
  m_last_symbol = fcs;
  
  // Clear any notifications of interrupt acceptance
  m_irq_accepted = false;
}

// Unknown symbol callback
void unknown_symbol_callback(FunctionCallPluginCallbackInfo *fcpcbi)
{
  fcpcbi->unknown_symbol_callback_handler(); 
}

void FunctionCallPluginCallbackInfo::unknown_symbol_callback_handler(void)
{
  if (!m_call_stack.empty())
  { 
    unsigned long long PC_val = (unsigned long long) m_core->read_PC();
    FunctionCallStackEntry cslast = m_call_stack.back(); 
    if (PC_val == cslast.m_return_addr)
    { 
      // PC matches the return address from the last stack entry, 
      // so this is a function return. 
      (*m_return_callback)(m_analysis_context, NULL, cslast.m_interface_sym); 
      m_call_stack.pop_back(); 
      // In some circumstances, we may need to exit multiple functions 
      // for the one PC value - e.g. on multiple ISR exits 
      // As long as the return address of functions on the stack match 
      // we continue to call the exit function callback 
      while (!m_call_stack.empty() && (PC_val == m_call_stack.back().m_return_addr))
      { 
        cslast = m_call_stack.back(); 
        // Keep going on the stack until the return address 
        // doesn't match 
        (*m_return_callback)(m_analysis_context, NULL, cslast.m_interface_sym); 
        m_call_stack.pop_back(); 
      } 
    } 
  } 
  m_last_symbol = NULL;
  
  // Clear any notifications of interrupt acceptance
  m_irq_accepted = false;
}

// Interrupt accepted callback
void interrupt_accepted_callback(FunctionCallPluginCallbackInfo *fcpcbi)
{
  fcpcbi->m_irq_accepted = true;
}

// Pre-translate loop predicate
rh850g4::analysis_predicate_response pre_translate_loop_predicate(FunctionCallPluginCallbackInfo *fcpcbi)
{
  // Create null callback structure 
  rh850g4::analysis_predicate_response ret_val; 

  // Reset transaction symbol pointers
  fcpcbi->m_last_trans_symbol = NULL; 
  fcpcbi->m_first_trans_symbol = true; 

  return ret_val; 
}

// Post-decode predicate
rh850g4::analysis_predicate_response post_decode_predicate(FunctionCallPluginCallbackInfo *fcpcbi)
{
  return fcpcbi->post_decode_predicate_handler();
}

rh850g4::analysis_predicate_response FunctionCallPluginCallbackInfo::post_decode_predicate_handler(void)
{
  // Create null callback structure 
  rh850g4::analysis_predicate_response ret_val; 

  if (m_core->is_decode_valid()) 
  { 
    unsigned long long PC_val = (unsigned long long) m_core->get_decode_PC();    
    FunctionCallSymbol *current_symbol = trans_lookup_symbol(PC_val); 
    if (current_symbol) 
    { 
      // If we have hit the start of a function and tracking entries 
      if (PC_val == current_symbol->m_interface_sym->address) 
      { 
        ret_val.set_analysis_callback(m_analysis_interface_info->m_start_symbol_callback, current_symbol); 
      } 
      // See if this is a function return location 
      else if (// Last translated sym == NULL can mean a number of things: 
            //     - the last symbol was an 'unknown' 
            //     - this is the first pc in a translated block 
            (m_last_trans_symbol == NULL) || 
            // Current symbol is different to last translated symbol 
            //     - this means that we have exited the last func 
            (current_symbol->m_interface_sym->address != m_last_trans_symbol->m_interface_sym->address)) 
      { 
        ret_val.set_analysis_callback(m_analysis_interface_info->m_known_symbol_callback, current_symbol); 
      } 
      // else is continuation of previous known symbol 
    } 
    // PC is a return point 
    else if (// First symbol post translation 
        (m_first_trans_symbol) || 
        // known -> unknown - could be a call or a return 
        (m_last_trans_symbol)) 
    { 
      ret_val.set_analysis_callback(m_analysis_interface_info->m_unknown_symbol_callback, this); 
    } 
    // else is unknown -> unknown 
    m_last_trans_symbol = current_symbol; 
    m_first_trans_symbol = false; 
  } 

  return ret_val; 
}

// Utility operations
FunctionCallSymbol* FunctionCallPluginCallbackInfo::trans_lookup_symbol(unsigned long long addr) 
{ 
  // Check if we know what the last translated symbol was 
  if (m_last_trans_symbol) 
  { 
    // If the address falls in the range of the last symbol then all good 
    unsigned long long offset = addr - m_last_trans_symbol->m_interface_sym->address; 
    if (offset < m_last_trans_symbol->m_interface_sym->size) 
    { 
      return m_last_trans_symbol; 
    } 
  } 
  
  // Must be a different symbol so will call back to VLAB to see if this 
  // address falls into a symbol of interest 
  caif_symbol_t *sym = m_query_callback(m_analysis_context, addr); 
  if (sym) { 
    // Symbol found so create a local symbol struct 
    FunctionCallSymbol &loc_sym = m_symbol_info[sym->address + sym->size - 1]; 
    // NOTE: If this symbol was already in the table, we're overwriting it 
    // with the same information. If it wasn't, we've created a new one. 
    // However, we do this in m_symbol_info to avoid having to new/delete 
    // pointers to FunctionCallSymbol's whilst still being able to pass  
    // pointers to them around. 
    loc_sym.m_interface_sym = sym; 
    loc_sym.m_callback_info = this; 
    return &loc_sym; 
  } 
  return NULL; 
} 

unsigned long long FunctionCallPluginCallbackInfo::get_function_return_address(void)
{
  unsigned int return_address;
  
  if (m_irq_accepted)
  {
    // TODO: Remove when implemented
    printf("Warning: function_call_plugin get_function_return_address() operation incomplete\n");
    return_address =  0;
  }
  else
  {
    // TODO: Remove when implemented
    printf("Warning: function_call_plugin get_function_return_address() operation incomplete\n");
    return_address =  0;
  }
  
  return (unsigned long long) return_address;
}


//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------

extern "C" bool add_func_call_cb(
    void *core_context,
    void *analysis_context,
    func_callback_func_t call_func,
    func_callback_func_t return_func,
    func_query_callback_func_t query_func)
{
  if (analysis_context && core_context && call_func && return_func && query_func)
  {
    // Get analysis interface
    AnalysisInterfaceInfo *aii =(AnalysisInterfaceInfo *) core_context;
    rh850g4::core_base_t *core = aii->m_core;
    
    // Create and initialise analysis context
    FunctionCallPluginInfo *fcpi = &function_call_plugin_info_map[analysis_context];
    fcpi->m_core = core;

    // Create callback information
    FunctionCallPluginCallbackInfo *fcpcbi = new FunctionCallPluginCallbackInfo();
    fcpcbi->m_call_callback = call_func;
    fcpcbi->m_return_callback = return_func;
    fcpcbi->m_query_callback = query_func;
    fcpcbi->m_analysis_context = analysis_context;
    fcpcbi->m_analysis_interface_info = aii;
    fcpcbi->m_core = core;
    fcpi->m_callback_info.push_back(fcpcbi);    

    // Attach predicates to the post-decode and pre-translate-loop analysis points
    fcpi->m_attachments.push_back(core->attach("post_decode", (rh850g4::analysis_predicate)post_decode_predicate, fcpcbi)); 
    fcpi->m_attachments.push_back(core->attach("pre_translate_loop",  (rh850g4::analysis_predicate)pre_translate_loop_predicate, fcpcbi)); 
    fcpi->m_attachments.push_back(core->attach("interrupt_accepted", (rh850g4::analysis_callback)interrupt_accepted_callback, fcpcbi)); 
  }
  
  return true;
}

extern "C" bool remove_func_call_cb(void *analysis_context)
{
  if (analysis_context)
  {
    // Get analysis context information
    FunctionCallPluginInfo *fcpi = &function_call_plugin_info_map[analysis_context];
    
    // Detach attachments
    for (std::list<rh850g4::analysis_attachment_descriptor *>::iterator att_it = fcpi->m_attachments.begin(); att_it != fcpi->m_attachments.end(); ++att_it )
    {
      fcpi->m_core->detach(*att_it);
    }
    fcpi->m_attachments.clear();
    
    // Delete callback information 
    for (std::list<FunctionCallPluginCallbackInfo *>::iterator fcpcbi_it = fcpi->m_callback_info.begin(); fcpcbi_it != fcpi->m_callback_info.end(); ++fcpcbi_it )
    {
      delete *fcpcbi_it;
    }
    fcpi->m_callback_info.clear();
  }
  
  return true;
}

//------------------------------------------------------------------------------
// Analysis interface internal operations
//------------------------------------------------------------------------------

void initialise_function_call_plugin(AnalysisInterfaceInfo *aii)
{
  // Register callbacks 
  aii->m_start_symbol_callback = aii->m_core->register_analysis_callback((rh850g4::analysis_callback)start_symbol_callback);
  aii->m_known_symbol_callback = aii->m_core->register_analysis_callback((rh850g4::analysis_callback)known_symbol_callback);
  aii->m_unknown_symbol_callback = aii->m_core->register_analysis_callback((rh850g4::analysis_callback)unknown_symbol_callback); 
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
