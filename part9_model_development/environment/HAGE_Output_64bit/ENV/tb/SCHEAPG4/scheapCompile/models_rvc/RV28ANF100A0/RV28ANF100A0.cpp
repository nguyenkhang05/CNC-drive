// ---------------------------------------------------------------------
// $Id: RV28ANF100A0.cpp,v 1.9 2018/05/18 08:02:02 chuonghoangle Exp $
//
// Copyright(c) 2018 Renesas Electronics Corporation
// Copyright(c) 2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "RV28ANF100A0.h"

/// Constructor and Destructor
RV28ANF100A0::RV28ANF100A0(sc_module_name name, const unsigned int rLatency, const unsigned int wLatency)
        :sc_module(name)
	,ANFIN("ANFIN")
	,ANFOUT("ANFOUT")
	,AEDOUT("AEDOUT")
	,INTR("INTR")
	,INTF("INTF")
{//{{{ 
    // Initialize output port
    ANFOUT.initialize(false);
    AEDOUT.initialize(false);

    // Initialize variables
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    mNoiseWidth = 100;
    mNoiseWidthUnit = SC_NS;
    mEdgeWidth = 50;
    mEdgeWidthUnit = SC_NS;

    mDelay1 = false;
    mDelay2 = false;
    mANFOUT = false;
    mIsDelay1UpdateEventPending = false;
    mIsDelay2UpdateEventPending = false;
    mDelay3 = false;

    // SC_THREAD/METHOD
    SC_METHOD(InbarMethod);
    dont_initialize();
    sensitive << ANFIN;

    SC_METHOD(Delay1Method);
    dont_initialize();
    sensitive << mInbarSignal;
    sensitive << mInbarEvent;

    SC_METHOD(Delay1UpdateMethod);
    dont_initialize();
    sensitive << mDelay1UpdateEvent;

    SC_METHOD(Delay2Method);
    dont_initialize();
    sensitive << mDelay1Signal;
    sensitive << mDelay1Event;

    SC_METHOD(Delay2UpdateMethod);
    dont_initialize();
    sensitive << mDelay2UpdateEvent;

    SC_METHOD(ResetMethod);
    dont_initialize();
    sensitive << mInbarSignal;
    sensitive << mDelay1Signal;
    sensitive << mDelay2Signal;
    sensitive << mDelay2Event; 

    SC_METHOD(SetMethod);
    dont_initialize();
    sensitive << mInbarSignal;
    sensitive << mDelay1Signal;
    sensitive << mDelay2Signal;
    sensitive << mDelay2Event;
    
    SC_METHOD(SrlatchMethod);
    dont_initialize();
    sensitive << mResetSignal;
    sensitive << mSetSignal;

    SC_METHOD(ANFOUTUpdateMethod);
    dont_initialize();
    sensitive << mANFOUTUpdateEvent;

    SC_THREAD(ANFOUTInitThread);

    SC_METHOD(Delay3Method);
    dont_initialize();
    sensitive << ANFOUT;

    SC_METHOD(Delay3UpdateMethod);
    dont_initialize();
    sensitive << mDelay3UpdateEvent;

    SC_METHOD(In1Method);
    dont_initialize();
    sensitive << ANFOUT;
    sensitive << mDelay3Signal;

    SC_METHOD(In2Method);
    dont_initialize();
    sensitive << ANFOUT;
    sensitive << mDelay3Signal;

    SC_METHOD(In3Method);
    dont_initialize();
    sensitive << mIn1Signal;
    sensitive << mIn2Signal;

    SC_METHOD(Mux4to1Method);
    dont_initialize();
    sensitive << mIn1Signal;
    sensitive << mIn2Signal;
    sensitive << mIn3Signal;
    sensitive << INTR;
    sensitive << INTF;
}//}}}

/// Destructor
RV28ANF100A0::~RV28ANF100A0()
{//{{{ 
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
/// Get time resolution
void RV28ANF100A0::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
{//{{{ 
    if (sc_get_time_resolution() < sc_time(1, SC_PS)) {
        resolution_value = 1000000000000000LL;
        resolution_unit = SC_FS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_NS)) {
        resolution_value = 1000000000000LL;
        resolution_unit = SC_PS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_US)) {
        resolution_value = 1000000000;
        resolution_unit = SC_NS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_MS)) {
        resolution_value = 1000000;
        resolution_unit = SC_US;
    } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)) {
        resolution_value = 1000;
        resolution_unit = SC_MS;
    } else {
        resolution_value = 1;
        resolution_unit = SC_SEC;
    }
}//}}}

//========================================================
//============ Operation               ===================
//========================================================
/// Method to generate INBAR signal
void RV28ANF100A0::InbarMethod()
{//{{{
    mInbarSignal = !(ANFIN.read());
    mInbarEvent.notify();
}//}}}

/// Method to generate DELAY1 signal
void RV28ANF100A0::Delay1Method()
{//{{{
    mDelay1 = mInbarSignal;
    mDelay1UpdateEvent.notify(mNoiseWidth / 2.0, mNoiseWidthUnit);
    mIsDelay1UpdateEventPending = true;
}//}}}

/// Method to generate DELAY1 signal
void RV28ANF100A0::Delay1UpdateMethod()
{//{{{
    mDelay1Signal.write(mDelay1);
    mDelay1Event.notify();
    mIsDelay1UpdateEventPending = false;
}//}}}

/// Method to generate DELAY2 signal
void RV28ANF100A0::Delay2Method()
{//{{{
    mDelay2 = mDelay1Signal;
    mDelay2UpdateEvent.notify(mNoiseWidth / 2.0, mNoiseWidthUnit);
    mIsDelay2UpdateEventPending = true;
}//}}}

/// Method to generate DELAY2 signal
void RV28ANF100A0::Delay2UpdateMethod()
{//{{{
    mDelay2Signal.write(mDelay2);
    mDelay2Event.notify();
    mIsDelay2UpdateEventPending = false;
}//}}}

/// Method to generate RESET signal
void RV28ANF100A0::ResetMethod()
{//{{{
    if ((!mIsDelay1UpdateEventPending) && (!mIsDelay2UpdateEventPending)) { // no pending event
        mResetSignal.write(!(mInbarSignal.read() & mDelay1Signal.read() & mDelay2Signal.read())); // NAND 3 inputs
    } else {
        /* do nothing */
    }
}//}}}

/// Method to generate SET signal
void RV28ANF100A0::SetMethod()
{//{{{
    if ((!mIsDelay1UpdateEventPending) && (!mIsDelay2UpdateEventPending)) { // no pending event
        mSetSignal.write(mInbarSignal.read() | mDelay1Signal.read() | mDelay2Signal.read()); // OR 3 inputs
    } else {
        /* do nothing */
    }
}//}}}

/// Method to generate ANFOUT output
void RV28ANF100A0::SrlatchMethod()
{//{{{ 
    if (mResetSignal.read() == 0) {
        mANFOUT = false;
        mANFOUTUpdateEvent.notify();
    } else if (mSetSignal.read() == 0) {
        mANFOUT = true;
        mANFOUTUpdateEvent.notify();
    }  else {
        /* do nothing */
    }
}//}}}

/// Method to generate ANFOUT output
void RV28ANF100A0::ANFOUTUpdateMethod()
{//{{{ 
    ANFOUT.write(mANFOUT);
}//}}}

/// Thread to initialize ANFOUT output
void RV28ANF100A0::ANFOUTInitThread()
{//{{{
    mANFOUT = ANFIN.read();
    mANFOUTUpdateEvent.notify();
}//}}}

/// Method to generate DELAY3 signal
void RV28ANF100A0::Delay3Method()
{//{{{ 
    mDelay3 = ANFOUT.read();
    mDelay3UpdateEvent.notify(mEdgeWidth, mEdgeWidthUnit);
}//}}}

/// Method to generate DELAY3 signal
void RV28ANF100A0::Delay3UpdateMethod()
{//{{{
    mDelay3Signal.write(mDelay3);
}//}}}

/// Method to generate IN1 signal
void RV28ANF100A0::In1Method()
{//{{{
    mIn1Signal.write(ANFOUT.read() & !(mDelay3Signal.read()));
}//}}}

/// Method to generate IN2 signal
void RV28ANF100A0::In2Method()
{//{{{
    mIn2Signal.write(!(ANFOUT.read() | !(mDelay3Signal.read())));
}//}}}

/// Method to generate IN3 signal
void RV28ANF100A0::In3Method()
{//{{{
    mIn3Signal.write(mIn1Signal.read() | mIn2Signal.read());
}//}}}

/// Method to generate AEDOUT output
void RV28ANF100A0::Mux4to1Method()
{//{{{
    AEDOUT.write((!INTF.read() & !INTR.read() & false) | (!INTF.read() & INTR.read() & mIn1Signal.read()) 
            | (INTF.read() & !INTR.read() & mIn2Signal.read()) | (INTF.read() & INTR.read() & mIn3Signal.read()));
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
/// Set message level (fatal, error, warning, info)
void RV28ANF100A0::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }
}//}}}

/// Set the max width of the input noise
void RV28ANF100A0::SetNoiseMaxWidth (const unsigned int noisewidth, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, unit);
    if (word_vector.size() == 1) {
        if ((word_vector[0] == "s") || (word_vector[0] == "ms") || (word_vector[0] == "us") || (word_vector[0] == "ns") || (word_vector[0] == "ps")) {
            
            // Get noise width
            mNoiseWidth = noisewidth;
            if (unit == "s") {
                mNoiseWidthUnit = SC_SEC;
            } else if (unit == "ms") {
                mNoiseWidthUnit = SC_MS;
            } else if (unit == "us") {
                mNoiseWidthUnit = SC_US;
            } else if (unit == "ns") {
                mNoiseWidthUnit = SC_NS;
            } else if (unit == "ps") {
                mNoiseWidthUnit = SC_PS;
            } else {
                mNoiseWidthUnit = SC_NS;
            }

            // Check if noise width is set to 0
            sc_time current_width(mNoiseWidth, mNoiseWidthUnit);
            sc_time min_width(0, SC_NS);
            if (current_width == min_width) {
                re_printf("warning","noisewidth is set to zero. noisewidth is reset to its default value (100ns).\n", noisewidth);
                mNoiseWidth = 100;
                mNoiseWidthUnit = SC_NS;
            } else {
                /* do nothing */
            }

            // Check if noise width is too short
            sc_time noise_width(mNoiseWidth, mNoiseWidthUnit);
            sc_time edge_width(mEdgeWidth, mEdgeWidthUnit); 
            if (noise_width < edge_width) {
                re_printf("warning","noisewidth (%s) is set shorter than edgewidth (%s). edgewidth is reset equal to noisewidth.\n", noise_width.to_string().c_str(), edge_width.to_string().c_str());
                mEdgeWidth = mNoiseWidth;
                mEdgeWidthUnit = mNoiseWidthUnit;
            } else {
                /* do nothing */
            }
        } else {
            re_printf("warning","Time unit (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Time unit (%s) is invalid\n", unit.c_str());
    }
}//}}}

/// Set the fixed width of the edge detection output
void RV28ANF100A0::SetEdgeFixWidth (const unsigned int edgewidth, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, unit);
    if (word_vector.size() == 1) {
        if ((word_vector[0] == "s") || (word_vector[0] == "ms") || (word_vector[0] == "us") || (word_vector[0] == "ns") || (word_vector[0] == "ps")) {
            
            // Get edge width
            mEdgeWidth = edgewidth;
            if (unit == "s") {
                mEdgeWidthUnit = SC_SEC;
            } else if (unit == "ms") {
                mEdgeWidthUnit = SC_MS;
            } else if (unit == "us") {
                mEdgeWidthUnit = SC_US;
            } else if (unit == "ns") {
                mEdgeWidthUnit = SC_NS;
            } else if (unit == "ps") {
                mEdgeWidthUnit = SC_PS;
            } else {
                mEdgeWidthUnit = SC_NS;
            }

            // Check if edge width is in a valid range
            sc_time current_width(mEdgeWidth, mEdgeWidthUnit);
            sc_time min_width(5, SC_NS);
            sc_time max_width(50, SC_NS);
            if (mEdgeWidth == 0) {
                re_printf("warning","edgewidth is set to zero. edgewidth is reset to its default value (50ns).\n");
                mEdgeWidth = 50;
                mEdgeWidthUnit = SC_NS;
            } else if ((current_width < min_width) || (current_width  > max_width)) {
                re_printf("warning","edgewidth (%d %s) is set outside the normal range (5ns to 50ns).\n", edgewidth, unit.c_str());
            } else {
                /* do nothing */
            }

            // Check if edge width is too long
            sc_time edge_width(mEdgeWidth, mEdgeWidthUnit); 
            sc_time noise_width(mNoiseWidth, mNoiseWidthUnit);
            if (edge_width > noise_width) {
                re_printf("warning","edgewidth (%s) is set longer than noisewidth (%s). edgewidth is reset equal to noisewidth.\n", edge_width.to_string().c_str(), noise_width.to_string().c_str());
                mEdgeWidth = mNoiseWidth;
                mEdgeWidthUnit = mNoiseWidthUnit;
            } else {
                /* do nothing */
            }
        } else {
            re_printf("warning","Time unit (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Time unit (%s) is invalid\n", unit.c_str());
    }
}//}}}

/// Separate some words from a string to store a vector
void RV28ANF100A0::SeparateString (std::vector<std::string> &vtr, const std::string msg)
{//{{{
    std::istringstream cmd;
    cmd.str(msg);
    std::string temp = "a"; //Make temp to be not empty
    while (temp != "") {
        std::string command;
        cmd >> command;
        if (command != "") {
            vtr.push_back(command);
        }
        temp = command;
    }
}//}}}

/// Dump help message of all parameters or commands
void RV28ANF100A0::Help ()
{//{{{
    SC_REPORT_INFO(this->basename(),"RV28ANF100A0_MessageLevel (\"RV28ANF100A0 instance\", \"fatal|error|warning|info\")  Set debug message level (Default: fatal|error)");
    SC_REPORT_INFO(this->basename(),"RV28ANF100A0_SetNoiseMaxWidth (\"RV28ANF100A0 instance\", 120, \"ns\")               Set noisewidth period (Default: 100ns");
    SC_REPORT_INFO(this->basename(),"RV28ANF100A0_SetEdgeFixWidth (\"RV28ANF100A0 instance\", 55, \"ns\")                 Set edgewidth period (Default: 50ns");
}//}}}

// vim600: set foldmethod=marker :
