//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2019.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

#ifndef _RH850_TRANSLATION_TRACER_H_
#define _RH850_TRANSLATION_TRACER_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <iomanip>
#include <iostream>
#include <string>
#include <map>
#include <list>
#include <exception>
#include "rh850/core_base.h"
#include "rh850/analysis_types.h"

//------------------------------------------------------------------------------
// Translation Tracer
//------------------------------------------------------------------------------

namespace rh850
{
  /** 
   * \brief Exception class used to indicate that an issue has occurred with
   * the usage of the translation tracer.
   */
  class translation_tracer_error_t : public std::exception
  {
  public:
    /**
     * \brief Translation tracer error constructor.
     * \param message Message describing the error condition in greater detail.
     */
    translation_tracer_error_t(const char *const& message) :
#ifdef _MSC_VER
      std::exception(message)
#else
      std::exception(),
      m_message(message)
#endif
    {
    }
    
#ifndef _MSC_VER
    /**
     * \brief Overload of the std::exception::what() method to return the
     * message specified in the constructor.
     * \return Message describing the error condition in greater detail.
     */
    virtual const char* what() const throw()
    {
      return m_message;
    }
    
  private:
    /**
     * \brief Message describing the error condition in greater detail.
     */
    const char *m_message;
#endif
  };
  
  /** 
   * \brief Translation tracer class.
   */
  class translation_tracer
  {
  public:    
    /**
     * \brief Translation tracer constructor.
     */
    translation_tracer() :
      m_core(NULL),
      m_block_info(),
      m_pre_translate_loop_attachment(NULL),
      m_post_fetch_attachment(NULL)
    {
    }

    /**
     * \brief Translation tracer destructor.
     */
    ~translation_tracer()
    {
      clear();
    }

    /**
     * \brief Clears all accumulated translation information
     */
    void clear()
    {
      for (std::map<unsigned int, std::list < unsigned int > * >::iterator it=m_block_info.begin(); it!=m_block_info.end(); ++it)
      {
        delete it->second;
      }
      m_block_info.clear();
    }
    
    /**
     * \brief Attaches to callouts to the specified core instance's callout
     * hooks and stores a handle to the core instance so that the trace()
     * operations can query the program counter etc. Note that this tracer
     * may only be associated with a single core instance. It is considered
     * an error to attach the tracer to more than one core instance.
     * \param core Pointer to the core instance for which to trace translation.
     */
    void attach(rh850::core_base_t *core)
    {
#if !defined(__VLAB_MIT__)      
      // Check core handle is valid
      if (core == NULL)
      {
        std::ostringstream os;
        os << "Could not attach, NULL core instance specified";
        os << std::endl;
        throw translation_tracer_error_t(os.str().c_str());
      }
      
      // Check not already attached to a core
      if (m_core != NULL)
      {
        std::ostringstream os;
        os << "Could not attach to '";
        os << core->name();
        os << "', already attached to '";
        os << m_core->name();
        os << "'";
        os << std::endl;
        throw translation_tracer_error_t(os.str().c_str());
      }
      
      // Store reference to core instance
      m_core = core;

      // Attach to analysis points
      m_pre_translate_loop_attachment = m_core->attach("pre_translate_loop", (rh850::analysis_callback)pre_translate_callback, this);
      m_post_fetch_attachment = m_core->attach("post_fetch", (rh850::analysis_callback)fetch_callback, this);
#endif      
    }
    
    /**
     * \brief Detaches the tracer from the core with which it is associated.
     * Note that the core manages any translation caches flushes that may arise
     * from detaching from an analysis point.
     */
    void detach()
    {
#if !defined(__VLAB_MIT__)      
      // Check already attached to a core
      if (m_core == NULL)
      {
        std::ostringstream os;
        os << "Could not detach, not currently attached to core instance";
        os << std::endl;
        throw translation_tracer_error_t(os.str().c_str());
      }
      
      // Detach from analysis points
      m_core->detach(m_pre_translate_loop_attachment);
      m_core->detach(m_post_fetch_attachment);
      
      // Clear reference to core instance
      m_core = NULL;
#endif      
    }

    /**
     * \brief Pre-translation loop callback operation.
     * \param tracer Pointer to self.
     */
    static void pre_translate_callback(rh850::translation_tracer *tracer)
    {
      rh850::core_base_t *core = tracer->m_core;
      if (core != NULL)
      {
        // Read the program counter
        unsigned int PC_val = core->read_PC();
        tracer->m_current_block = PC_val;
        
        tracer->m_block_info[tracer->m_current_block] = new std::list < unsigned int >();
      }
    }

    /**
     * \brief Fetch phase callback operation. This operation stores opcode
     * and mnemonic information for later use by the trace callback.  Note
     * that we are expected to overwrite any locally cached translation
     * information when re-translation takes place.
     * \param tracer Pointer to self.
     */
    static void fetch_callback(rh850::translation_tracer *tracer)
    {
      rh850::core_base_t *core = tracer->m_core;
      if ((core != NULL) && core->is_decode_valid())
      {
        unsigned int PC_val = core->get_decode_PC();
        
        tracer->m_block_info[tracer->m_current_block]->push_back(PC_val);
      }
    }
    
    /**
     * \brief Pointer to the core instance for which to trace translation, used
     * to be able to grab the program counter value. Set during attach()
     */
    rh850::core_base_t *m_core;
     
    std::map<unsigned int, std::list < unsigned int > * > m_block_info;

    unsigned int m_current_block;
    
    
    /**
     * \brief Descriptor referring to pre-translation loop predicate attachment
     */
    rh850::analysis_attachment_descriptor *m_pre_translate_loop_attachment;    
    
    /**
     * \brief Descriptor referring to post fetch predicate attachment
     */
    rh850::analysis_attachment_descriptor *m_post_fetch_attachment;    
  };
}

#endif // _RH850_TRANSLATION_TRACER_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
