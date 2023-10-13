// ---------------------------------------------------------------------
// $Id: SPID_MODIFIER.cpp,v 1.9 2020/05/26 01:29:06 ngathunguyen Exp $
//
// Copyright(c) 2019 - 2020 Renesas Electronics Corporation
// Copyright(c) 2019 - 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "SPID_MODIFIER.h"

// Description: Constructor of SPID_MODIFIER
// Ref: {SPID_MODIFIER_DD_Direction_019, SPID_MODIFIER_DD_Summary_001, SPID_MODIFIER_DD_BlockDiagram_001, SPID_MODIFIER_DD_Ports_001, SPID_MODIFIER_DD_ATT1_001, SPID_MODIFIER_DD_Direction_020, SPID_MODIFIER_DD_Direction_023}
template<unsigned int BUS_WIDTH>
SPID_MODIFIER<BUS_WIDTH>::SPID_MODIFIER(sc_module_name name) :
    sc_module( name )
    , ts("ts")
    , is("is")
    , resetPort("resetPort")
    , SPID("SPID")
{//{{{
    //-------------------------------------
    is(*this);
    ts(*this);
    mSPIDPort          = 0;
    mIsResetPortActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive  = false;
    mResetCmdPeriod    = 0;
    
    // processes handle reset sources
    SC_METHOD(HandleresetPortMethod);
    dont_initialize();
    sensitive << resetPort;

    SC_METHOD(HandleresetPortCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mCancelResetCmdEvent;

    SC_METHOD(HandleSPIDPortMethod);
    dont_initialize();
    sensitive << SPID;
        
    //Initialize signal, state
    Initialize();
    CommandInit((std::string)name);
}//}}}

//------------------------------------------------------------------------------
// Description: Destructor of SPID_MODIFIER
// Ref: {SPID_MODIFIER_DD_Direction_019}
template<unsigned int BUS_WIDTH>
SPID_MODIFIER<BUS_WIDTH>::~SPID_MODIFIER()
{//{{{

}//}}}

// Description: Handle Reset command method
// Ref: {SPID_MODIFIER_DD_Direction_002, SPID_MODIFIER_DD_Direction_015}
template<unsigned int BUS_WIDTH>
void SPID_MODIFIER<BUS_WIDTH>::HandleresetPortCmdMethod()
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", resetPort.basename());
    this->EnableReset(mIsResetCmdActive);
    mCancelResetCmdEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

// Description: Handle cancel reset command method
// Ref: {SPID_MODIFIER_DD_Direction_002, SPID_MODIFIER_DD_Direction_014}
template<unsigned int BUS_WIDTH>
void SPID_MODIFIER<BUS_WIDTH>::CancelResetCmdMethod ()
{//{{{
    mIsResetCmdActive  = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of %s is over.\n", resetPort.basename());
    this->EnableReset(mIsResetCmdActive);
}//}}}

// Description: Handle Reset port method
// Ref: {SPID_MODIFIER_DD_Ports_004, SPID_MODIFIER_DD_Diagram_003, SPID_MODIFIER_DD_Direction_017, SPID_MODIFIER_DD_Direction_018}
template<unsigned int BUS_WIDTH>
void SPID_MODIFIER<BUS_WIDTH>::HandleresetPortMethod()
{//{{{
    bool resetport_state = resetPort.read();

    if (resetport_state == vpcl::emResetActive){
        mIsResetPortActive = true;
        re_printf("info", "The reset port %s is asserted.\n", resetPort.basename());
        this->EnableReset(mIsResetPortActive);
    } else{ //reset de-active
        mIsResetPortActive = false;
        re_printf("info", "The reset port %s is de-asserted.\n", resetPort.basename());
    }
    // when resetPort is active, cancel reset command
    if(mIsResetPortActive && mIsResetCmdReceive){
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
        mResetCmdEvent.cancel();
        mCancelResetCmdEvent.cancel();
    }
}//}}}

// Description: Process when hard reset is asserted
// Ref: {SPID_MODIFIER_DD_Ports_003}
template<unsigned int BUS_WIDTH>
void SPID_MODIFIER<BUS_WIDTH>::EnableReset(bool isActive)
{//{{{
    Initialize();
}//}}}


// Description: Initialize model when hard reset is asserted
// Ref: {SPID_MODIFIER_DD_Ports_003}
template<unsigned int BUS_WIDTH>
void SPID_MODIFIER<BUS_WIDTH>::Initialize()
{//{{{ 
    mTransMap.clear();
}//}}}

//##############################################################################
// Part 1: PythonIF
//##############################################################################
//------------------------------------------------------------------------------
// Description: Process when Assert reset
// Ref: {SPID_MODIFIER_DD_Direction_002, SPID_MODIFIER_DD_Direction_016, SPID_MODIFIER_DD_Direction_013, SPID_MODIFIER_DD_Direction_012}
template<unsigned int BUS_WIDTH>
void SPID_MODIFIER<BUS_WIDTH>::AssertReset(const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == resetPort.basename()) {
        if ((!mIsResetPortActive) && (!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (%s) for %f ns after %f ns.\n", resetPort.basename(), period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        }
        else {
            re_printf("warning", "The software reset of %s is called in the reset operation of the model. So it is ignored.\n", resetPort.basename());
        }
    }
    else {
        re_printf("warning","The reset name (%s) is wrong. It should be %s.\n", reset_name.c_str(), resetPort.basename());
    }
}//}}}

// Description: Dump help message of all parameters or commands
// Ref: {SPID_MODIFIER_DD_Direction_009}
template<unsigned int BUS_WIDTH>
void SPID_MODIFIER<BUS_WIDTH>::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"SPID_MODIFIER_MessageLevel_Bus32 (fatal|error|warning|info) Set debug message level (Default: fatal|error) for SPID_MODIFIER<32>");
            SC_REPORT_INFO(this->basename(),"SPID_MODIFIER_MessageLevel_Bus64 (fatal|error|warning|info) Set debug message level (Default: fatal|error) for SPID_MODIFIER<64>");
            SC_REPORT_INFO(this->basename(),"SPID_MODIFIER_MessageLevel_Bus128 (fatal|error|warning|info) Set debug message level (Default: fatal|error) for SPID_MODIFIER<128>");
            SC_REPORT_INFO(this->basename(),"SPID_MODIFIER_MessageLevel_Bus256 (fatal|error|warning|info) Set debug message level (Default: fatal|error) for SPID_MODIFIER<256>");
            SC_REPORT_INFO(this->basename(),"SPID_MODIFIER_MessageLevel_Bus512 (fatal|error|warning|info) Set debug message level (Default: fatal|error) for SPID_MODIFIER<512>");
            SC_REPORT_INFO(this->basename(),"SPID_MODIFIER_MessageLevel_Bus1024 (fatal|error|warning|info) Set debug message level (Default: fatal|error) for SPID_MODIFIER<1024>");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"SPID_MODIFIER_AssertReset_Bus32 (reset_name, start_time, period) Assert and de-assert reset signal to the SPID_MODIFIER<32> model");
            SC_REPORT_INFO(this->basename(),"SPID_MODIFIER_AssertReset_Bus64 (reset_name, start_time, period) Assert and de-assert reset signal to the SPID_MODIFIER<64> model");
            SC_REPORT_INFO(this->basename(),"SPID_MODIFIER_AssertReset_Bus128 (reset_name, start_time, period) Assert and de-assert reset signal to the SPID_MODIFIER<128> model");
            SC_REPORT_INFO(this->basename(),"SPID_MODIFIER_AssertReset_Bus256 (reset_name, start_time, period) Assert and de-assert reset signal to the SPID_MODIFIER<256> model");
            SC_REPORT_INFO(this->basename(),"SPID_MODIFIER_AssertReset_Bus512 (reset_name, start_time, period) Assert and de-assert reset signal to the SPID_MODIFIER<512> model");
            SC_REPORT_INFO(this->basename(),"SPID_MODIFIER_AssertReset_Bus1024 (reset_name, start_time, period) Assert and de-assert reset signal to the SPID_MODIFIER<1024> model");
        } else {
            re_printf("warning","The name (%s) of SPID_MODIFIER_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of SPID_MODIFIER_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

///Separate some words from a string to store a vector
template<unsigned int BUS_WIDTH>
void SPID_MODIFIER<BUS_WIDTH>::SeparateString (std::vector<std::string> &vtr, const std::string msg)
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

// Description: SetMessageLevel command process
// Ref: {SPID_MODIFIER_DD_Direction_001}
template<unsigned int BUS_WIDTH>
void SPID_MODIFIER<BUS_WIDTH>::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_ret = this->handleCommand(cmd);
    if (msg_ret != "") {
       SC_REPORT_INFO(this->basename(), msg_ret.c_str());
    }
}//}}}

// Description: Handle SPID Port Method
// Ref: {SPID_MODIFIER_DD_Ports_003, SPID_MODIFIER_DD_Diagram_002}
template<unsigned int BUS_WIDTH>
void SPID_MODIFIER<BUS_WIDTH>::HandleSPIDPortMethod()
{//{{{
    mSPIDPort = SPID.read();
}//}}}

// Description: Function nb_transport_fw to update SPID and I/F Pointer then forward transaction to Slave
// Ref: {SPID_MODIFIER_DD_Ports_002, SPID_MODIFIER_DD_Diagram_004, SPID_MODIFIER_DD_Summary_005, SPID_MODIFIER_DD_Summary_006}
template<unsigned int BUS_WIDTH>
TlmSyncEnum SPID_MODIFIER<BUS_WIDTH>::nb_transport_fw( TlmTransactionType& trans, TlmPhase& phase, sc_core::sc_time& t )
{//{{{
    // Backup the trans info in forward path
    BackupTransInfo(trans);
    // Update SPID and IFPointer
    UpdateTransaction(trans, mSPIDPort, this);
    // Forward transaction to SLAVE
    return is->nb_transport_fw( trans, phase, t );
}//}}}

// Description: Function nb_transport_bw to update SPID and I/F Pointer in return path
// Ref: {SPID_MODIFIER_DD_Ports_002, SPID_MODIFIER_DD_Diagram_006, SPID_MODIFIER_DD_Summary_005, SPID_MODIFIER_DD_Summary_006}
template<unsigned int BUS_WIDTH>
TlmSyncEnum SPID_MODIFIER<BUS_WIDTH>::nb_transport_bw( TlmTransactionType& trans, TlmPhase& phase, sc_core::sc_time& t )
{//{{{
    // Restore the SPID and IFPointer in return path
    RestoreTrans(trans);
    // Send transaction to Master IP
    return ts->nb_transport_bw(trans, phase, t);
}//}}}

// Description: Function b_transport to update SPID then forward transaction to Slave and restore mSpId
// Ref: {SPID_MODIFIER_DD_Ports_002, SPID_MODIFIER_DD_Diagram_005, SPID_MODIFIER_DD_Summary_005, SPID_MODIFIER_DD_Summary_006}
template<unsigned int BUS_WIDTH>
void SPID_MODIFIER<BUS_WIDTH>::b_transport( TlmTransactionType& trans, sc_core::sc_time& t )
{//{{{

    TlmG3mExtension * g3mExt = (TlmG3mExtension *)0;
    trans.get_extension( g3mExt );
    sc_assert(g3mExt != NULL);

    unsigned int spid = g3mExt->getSpId();
    g3mExt->setSpId(mSPIDPort);
    // Forward transaction to SLAVE
    is->b_transport( trans, t );
    // Restore spid value
    g3mExt->setSpId(spid);
    
    
}//}}}

//Description: Function transport_dbg to update SPID then forward transaction in debug mode to Slave and restore mSpId
//Ref: {SPID_MODIFIER_DD_Ports_002, SPID_MODIFIER_DD_Diagram_011}
template<unsigned int BUS_WIDTH>
unsigned int SPID_MODIFIER<BUS_WIDTH>::transport_dbg( TlmTransactionType& trans )
{//{{{
    TlmG3mExtension * g3mExt = (TlmG3mExtension *)0;
    trans.get_extension( g3mExt );
    unsigned int nmb = 0;
    if(g3mExt == NULL) {
        g3mExt = new TlmG3mExtension;
        g3mExt->setSpId(mSPIDPort);
        trans.set_extension( g3mExt);
        // Forward transaction to SLAVE
        nmb = is->transport_dbg( trans);
        trans.release_extension(g3mExt);
    } else {
        unsigned int spid = g3mExt->getSpId();
        g3mExt->setSpId(mSPIDPort);
        // Forward transaction to SLAVE
        nmb = is->transport_dbg( trans);
        // Restore spid value
        g3mExt->setSpId(spid);
    }
    return nmb;
}//}}}

// Description: Function backup SPID and I/F Pointer
// Ref: {SPID_MODIFIER_DD_Feature_001, SPID_MODIFIER_DD_Diagram_008}
template<unsigned int BUS_WIDTH>
void SPID_MODIFIER<BUS_WIDTH>::BackupTransInfo ( TlmTransactionType &trans)
{//{{{
    TlmG3mExtension *g3mExt;
    g3mExt = (TlmG3mExtension *)0;
    trans.get_extension( g3mExt );
    sc_assert (g3mExt != NULL);
    // Get spid and Ifpointer of trans
    std::pair <unsigned char, TlmBwTransportIf*> trans_ext;
    unsigned char spid = (unsigned char) g3mExt->getSpId();
    TlmBwTransportIf* ifpointer = g3mExt->getIfPointer();
    sc_assert (ifpointer != NULL);
    trans_ext = std::make_pair (spid, ifpointer);
    // Create pair object
    std::pair <TlmTransactionType*, std::pair <unsigned char, TlmBwTransportIf*> > transMap;
    transMap = std::make_pair (&trans, trans_ext);
    // Insert pair object to map
    mTransMap.insert(transMap);
}//}}}

// Description: Function RestoreTrans SPID and I/F Pointer
// Ref: {SPID_MODIFIER_DD_Feature_001, SPID_MODIFIER_DD_Diagram_009}
template<unsigned int BUS_WIDTH>
void SPID_MODIFIER<BUS_WIDTH>::RestoreTrans ( TlmTransactionType &trans)
{//{{{
    TlmG3mExtension * g3mExt = (TlmG3mExtension *)0;
    trans.get_extension( g3mExt );
    std::map<TlmTransactionType*,std::pair<unsigned char, TlmBwTransportIf*> >::iterator it;
    it = mTransMap.find(&trans);
    if (it != mTransMap.end()) {
        UpdateTransaction(trans, it->second.first, it->second.second);
        mTransMap.erase(it);
    }
}//}}}

// Description: Function update SPID and I/F Pointer
//Ref: {SPID_MODIFIER_DD_Feature_001, SPID_MODIFIER_DD_Diagram_010}
template<unsigned int BUS_WIDTH>
void SPID_MODIFIER<BUS_WIDTH>::UpdateTransaction( TlmTransactionType &trans, unsigned char spid, TlmBwTransportIf* IfPointer )
{//{{{
    sc_assert (IfPointer != NULL);
    TlmG3mExtension * g3mExt = (TlmG3mExtension *)0;
    trans.get_extension( g3mExt );
    sc_assert (g3mExt != NULL);
    g3mExt->setIfPointer( IfPointer );
    g3mExt->setSpId( spid );
    
}//}}}

// Description: Function inherit from tlm::tlm_fw_direct_mem_if<TRANS>
template<unsigned int BUS_WIDTH>
bool SPID_MODIFIER<BUS_WIDTH>::get_direct_mem_ptr( TlmTransactionType& trans,
           tlm::tlm_dmi&  dmiData )
{//{{{
    return false;
}//}}}

// Description: Function inherit from tlm::tlm_fw_direct_mem_if<TRANS>
template<unsigned int BUS_WIDTH>
void SPID_MODIFIER<BUS_WIDTH>::invalidate_direct_mem_ptr( uint64 startRange,
                  uint64 endRange ) 
{//{{{
}//}}}

// Description: Template of SPID_MODIFIER
// Ref: {SPID_MODIFIER_DD_Direction_023}
template class SPID_MODIFIER<32>;
template class SPID_MODIFIER<64>;
template class SPID_MODIFIER<128>;
template class SPID_MODIFIER<256>;
template class SPID_MODIFIER<512>;
template class SPID_MODIFIER<1024>;

// vim600: set foldmethod=marker :
