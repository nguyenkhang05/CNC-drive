// Copyright (C) Australian Semiconductor Technology Company (ASTC). 2015.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC). The copyright notice does not evidence any actual
// or intended publication of such source code.

#ifndef PFRH850_HPP
#define PFRH850_HPP

#include "PFRH850_base.hpp"

class PFRH850 : public PFRH850_base
{
public:
    PFRH850(sc_core::sc_module_name name, NSMVG4SSV01& G4SS, const char * pMapFileName);
    virtual ~PFRH850();
    
    // Interrupt signals
    sc_signal<bool, SC_UNCHECKED_WRITERS> feint_signal[PE_MAX_NUM];
    sc_signal<bool, SC_UNCHECKED_WRITERS> fenmi_signal;
    sc_signal<bool, SC_UNCHECKED_WRITERS> PE_eiint_signal[PE_MAX_NUM][EI_INTC1_CH_NUM];
    sc_signal<bool, SC_UNCHECKED_WRITERS> eiint_signal[EI_ALL_CH_NUM - EI_INTC1_CH_NUM];

private:
    SC_HAS_PROCESS(PFRH850);

    // Simulation callbacks
    virtual void start_of_simulation();

    // Handle custom bindings
    void handle_custom_bindings(NSMVG4SSV01& G4SS);
};

#endif // PFRH850_HPP
