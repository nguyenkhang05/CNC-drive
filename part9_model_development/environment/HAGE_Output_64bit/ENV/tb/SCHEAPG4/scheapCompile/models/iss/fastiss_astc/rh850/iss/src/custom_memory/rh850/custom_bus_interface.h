//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2019.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

#ifndef _CUSTOM_BUS_INTERFACE_H_
#define _CUSTOM_BUS_INTERFACE_H_

/**
 * \file custom_bus_interface.h
 * \brief Defines the custom bus interface class
 */

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850/bus_interface.h"
#include <systemc>
#include <tlm.h>

//------------------------------------------------------------------------------
// Bus interface
//------------------------------------------------------------------------------

namespace rh850
{
  /**
   * \brief Models the custom bus interface.
   */
  class custom_bus_interface_t : public bus_interface_t
  {
  public:
    /**
     * \brief custom bus interface constructor.
     * \param name Name of the bus interface.
     */
    custom_bus_interface_t(const std::string& name);

    /**
     * \brief custom bus interface destructor.
     */
    ~custom_bus_interface_t();

    /**
     * \brief Operation to get a populated payload to be used for a debug transaction
     * \param command Type of transaction to be issued
     * \param address Physical address at which the transaction is to be issued
     * \param size Size of the transaction to be issued
     * \param data Buffer used to hold data read or written by the transaction
     * \return Reference to the populated payload structure to be used for the transaction
     */
    tlm::tlm_generic_payload& get_debug_payload(tlm::tlm_command command, sc_dt::uint64 address, unsigned int size, unsigned char *data);
    
    /**
     * \brief Operation to get a populated payload to be used for a fetch transaction
     * \param address Physical address at which the transaction is to be issued
     * \param size Size of the transaction to be issued
     * \param data Buffer used to hold data read or written by the transaction
     * \return Reference to the populated payload structure to be used for the transaction
     */
    tlm::tlm_generic_payload& get_fetch_payload(sc_dt::uint64 address, unsigned int size, unsigned char *data);
    
    /**
     * \brief Operation to get a populated payload to be used for a DMI request
     * \param command Type of transaction to be issued
     * \param address Physical address at which the transaction is to be issued
     * \param size Size of the transaction to be issued
     * \param data Buffer used to hold data read or written by the transaction
     * \return Reference to the populated payload structure to be used for the transaction
     */
    tlm::tlm_generic_payload& get_dmi_payload(tlm::tlm_command command, sc_dt::uint64 address, unsigned int size, unsigned char *data);
    
    /**
     * \brief Operation to get a populated payload to be used for other (ie. normal blocking or non-blocking) transactions
     * \param command Type of transaction to be issued
     * \param address Physical address at which the transaction is to be issued
     * \param size Size of the transaction to be issued
     * \param data Buffer used to hold data read or written by the transaction
     * \return Reference to the populated payload structure to be used for the transaction
     */
    tlm::tlm_generic_payload& get_payload(tlm::tlm_command command, sc_dt::uint64 address, unsigned int size, unsigned char *data);

    /**
     * \brief Operation used to complete any necessary post-transaction processing of
     * a debug transaction. Typically this is used to check the payload response
     * status.
     * \param bytes_transferred Number of bytes successfully transferred.
     */
    void debug_completed(unsigned int bytes_transferred);

    /**
     * \brief Operation used to complete any necessary post-transaction processing of
     * a fetch transaction. Typically this is used to check the payload response
     * status.
     * \param bytes_transferred Number of bytes successfully transferred.
     */
    void fetch_completed(unsigned int bytes_transferred);

    /**
     * \brief Operation used to complete any necessary post-transaction processing of
     * a DMI request. Typically this is used to check the payload response status.
     */
    void dmi_completed(void);

    /**
     * \brief Operation used to complete any necessary post-transaction processing of
     * other (ie. normal blocking or non-blocking) transactions. Typically this
     * is used to check the payload response status.
     */
    void transaction_completed(void);

    /**
     * \brief Callout used after initiating a DMI request. Allows the DMI,
     * physical and effective address information to be customised for an MPU
     * or MMU (for example to restrict the region to an MMU page or MPU region).
     * \param type The transaction type (either a load, store, debug load, debug
     * store, fetch, or ignore (zero sized) access)
     * \param val The DMI region information after the DMI request
     */
    void dmi_pointer_post_callout(rh850::access_type_t type, dmi_region *val);

  private:
    /**
     * \brief Payload used for debug transactions.
     */
    tlm::tlm_generic_payload m_debug_payload;
    /**
     * \brief Payload used for fetch transactions.
     */
    tlm::tlm_generic_payload m_fetch_payload;
    /**
     * \brief Payload used for DMI requests
     */
    tlm::tlm_generic_payload m_dmi_payload;
    /**
     * \brief Payload used for other (ie. normal blocking or non-blocking) transactions
     */
    tlm::tlm_generic_payload m_payload;
  };
}

#endif //  _CUSTOM_BUS_INTERFACE_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
