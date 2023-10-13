//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2020.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file analysis_types.h
 * \brief Definitions of fundamental analysis types
 */

#ifndef _RH850G4_ANALYSIS_TYPES_H_
#define _RH850G4_ANALYSIS_TYPES_H_

//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------

namespace rh850g4
{
  /**
   * \class rh850g4::analysis_attachment_descriptor
   * \brief Used identify a unique attachment of an analysis callback or predicate to an analysis attachment point.
   * Required so that it may be subsequently detached
   */
  class analysis_attachment_descriptor;
  
  /**
   * \class rh850g4::analysis_callback_descriptor
   * \brief Used to identify an analysis callback that has been registered with the analysis callback manager.
   * Required so that an analysis predicate operation may identify the analysis callback that should be inserted into the translated code
   */
  class analysis_callback_descriptor;
  
  /**
   * \brief Type specification for the analysis callback operation
   * \param callback_data Handle to the data upon which the analysis callback operates
   */
  typedef void (*analysis_callback)(void* callback_data);

  /**
   * \class rh850g4::analysis_predicate_response
   * \brief Data structure returned by an analysis predicate operation to indicate what operation (if any) should be inserted into the generated code
   */
  class analysis_predicate_response
  {
  public:
    /**
     * \brief Analysis predicate response constructor. Creates an analysis predicate response which indicates that no operation should be inserted into the generated code
     */
    analysis_predicate_response();

    /**
     * \brief Sets the analysis predicate response to indicate that invocation of a registered analysis callback should be inserted into the generated code
     * \param callback Analysis callback register descriptor obtained by registering the analysis callback with the analysis callback manager
     * \param callback_data Data to be passed to the analysis callback operation
     */
    void set_analysis_callback(analysis_callback_descriptor *callback, void *callback_data);
    
    /**
     * \brief Sets the analysis predicate response to indicate that increment of the specified analysis counter should be inserted into the generated code
     * \param counter Analysis counter
     */
    void set_analysis_counter_increment(unsigned long long *counter);

    /**
     * \brief Queries whether this response is intended to cause the insertion of a call to an analysis callback into the generated code
     * Note that this operation is intended for use by the analysis callback manager only
     * \return Boolean value indicating whether this response is intended to cause the insertion of a call to an analysis callback into the generated code
     */
    bool is_analysis_callback();

    /**
     * \brief Queries whether this response is intended to cause the insertion of an increment of an analysis counter into the generated code
     * Note that this operation is intended for use by the analysis callback manager only
     * \return Boolean value indicating whether this response is intended to cause the insertion of an increment of an analysis counter into the generated code
     */
    bool is_analysis_counter_increment();

    /**
     * \brief Queries whether this response is intended to cause the insertion of any operation into the generated code
     * Note that this operation is intended for use by the analysis callback manager only
     * \return Boolean value indicating whether this response is intended to cause the insertion of any operation into the generated code
     */
    bool is_valid();
    
    /**
     * \brief Query the analysis callback descriptor value.
     * Note that this operation is intended for use by the analysis callback manager only and is presumed to be guarded by is_analysis_callback()
     * \return Handle to an analysis callback descriptor
     */
    analysis_callback_descriptor *get_analysis_callback_descriptor();

    /**
     * \brief Query the analysis callback data value.
     * Note that this operation is intended for use by the analysis callback manager only and is presumed to be guarded by is_analysis_callback()
     * \return Handle to data to be passed to analysis callback
     */
    void *get_analysis_callback_data();
    
    /**
     * \brief Query the analysis counter value.
     * Note that this operation is intended for use by the analysis callback manager only and is presumed to be guarded by is_analysis_counter_increment()
     * \return Handle to counter to be incremented by generated code
     */
    unsigned long long *get_analysis_counter();
    
  private:
    /**
     * \brief Analysis callback descriptor identifying the analysis callback operation to be invoked.
     * This should be NULL if no callback is to be invoked or if the response denotes an analysis counter increment operation is to be inserted into the generated code.
     */
    analysis_callback_descriptor *m_callback;
    
    /**
     * \brief Pointer to the data upon which the analysis callback should operate.
     * This should be NULL if no callback is to be invoked and if no analysis counter increment operation is to be inserted into the generated code.
     */
    void *m_callback_data;
  };
  
  /**
   * \brief Type specification for the analysis predicate operation.
   * The predicate operation is responsible for determining which registered analysis callback or analysis counter increment operation (if any) should be inserted into the translated code.
   * \param predicate_data Handle to the data upon which the analysis predicate operates
   * \return Response value indicating which registered analysis callback or analysis counter increment operation (if any) should be inserted into the translated code.
   * If an analysis callback is to be inserted the response also indicates what data that analysis callback should operate upon.
   */
  typedef analysis_predicate_response (*analysis_predicate) (void *predicate_data);  
}

#endif // _RH850G4_ANALYSIS_TYPES_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
