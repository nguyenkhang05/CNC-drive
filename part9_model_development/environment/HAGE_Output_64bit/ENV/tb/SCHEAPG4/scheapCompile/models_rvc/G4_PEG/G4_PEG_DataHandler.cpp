// ---------------------------------------------------------------------
// $Id: G4_PEG_DataHandler.cpp,v 1.10 2020/01/14 02:39:30 nhutminhdang Exp $
//
// Copyright(c) 2016-2020 Renesas Electronics Corporation
// Copyright(c) 2016-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include <cstdarg>
#include "G4_PEG_DataHandler.h"
#include "BusMasterIf.h"
#include "G4_PEG_AgentController.h"
#include <iomanip>

/*********************************
// Function     : G4_PEG_DataHandler
// Description  : Constructor of G4_PEG_DataHandler class
// Parameter    : 
//    name                         name of module
//    G4_PEG_AgentControllerPtr    pointer of G4_PEG_AgentController class
//    Parent                       pointer of BusBridgeModuleBase class
//    SlaveIf                      pointer of BusBridgeSlaveIf class 
//    MasterIf                     pointer of BusMasterIf class 
//    simmode                      simulation mode
//    CHANNEL_NUM                  number of channel
// Return value : None
**********************************/
G4_PEG_DataHandler::G4_PEG_DataHandler(std::string name
                                       ,G4_PEG_AgentController *G4_PEG_AgentControllerPtr
                                       ,BusBridgeModuleBase<BUS_WIDTH_VCI,BUS_WIDTH_VCI> *Parent
                                       ,BusBridgeSlaveIf<BUS_WIDTH_VCI> *SlaveIf
                                       ,BusMasterIf<BUS_WIDTH_VCI> *MasterIf
                                       ,SIM_MODE_T simmode
                                       ,unsigned int CHANNEL_NUM)
                                       :BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_VCI>(Parent,SlaveIf,MasterIf)
{//{{{
    /// Initialize variables
    assert(G4_PEG_AgentControllerPtr != NULL);
    mG4_PEG_AgentController = G4_PEG_AgentControllerPtr;
    assert((MasterIf != NULL) && (SlaveIf != NULL));
    BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_VCI>::setTlmTransType(simmode);
    mChannelNum = CHANNEL_NUM;
    mRstState = false;
    mPCLKPeriod = 0;
    Initialize();
}//}}}

/*********************************
// Function     : ~G4_PEG_DataHandler
// Description  : Destructor of G4_PEG_DataHandler class
// Parameter    : None
// Return value : None
**********************************/
G4_PEG_DataHandler::~G4_PEG_DataHandler (void)
{//{{{
}//}}}

/*********************************
// Function     : Initialize
// Description  : Initialize when reset activated
// Parameter    : None
// Return value : None
**********************************/
void G4_PEG_DataHandler::Initialize (void)
{//{{{
    mADDR = 0;
    mSPID = 0;
    mBMID = 0;
    mDBG = false;    
    mUM = false;     
    mWRITE = false;  
    mSecure = false; 
    mGEN = false;
    mWG = false; 
    mRG = false; 
    
    bus_master_UM = false;    
    bus_master_SPID = 0;      
    bus_master_PEID = 0;      
    bus_master_SEC = false;   
    mTrans_type = false;      
    mIsSpcLd = false;         
}//}}}

/*********************************
// Function     : read
// Description  : Overwrite virtual functions of BusBridgeCoreBase class
// Parameter    : 
//    offsetAddress     read address
//    trans             payload of transaction
//    t                 bus time
//    debug             true: debug mode; false: normal mode   
// Return value : None
**********************************/
void G4_PEG_DataHandler::read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (t == NULL) {
        return;
    }

    if ((mPCLKPeriod != 0)) {
        if (debug || (!debug && !mRstState)) {
            if (AccessJudgment(trans, debug)) {
                if (debug || (BusBridgeCoreBase<BUS_WIDTH_VCI, BUS_WIDTH_VCI>::mTransType == LT)) {// LT mode
                    SC_REPORT_INFO("G4_PEG", "Access is permitted.\n");
                    mMasterIf->read(trans, debug, *t, false);
                } else {
                    /// Do not support AT mode
                }
            } else {
                SC_REPORT_INFO("G4_PEG", "Access is prohibited.\n");
            }
        } else {
            SC_REPORT_WARNING("G4_PEG","The reading transaction is fail because the reset is active");
        }
    } else {
        SC_REPORT_INFO("G4_PEG", "The reading transaction is fail because clock is zero.\n");
    }
}//}}}

/*********************************
// Function     : write
// Description  : Overwrite virtual functions of BusBridgeCoreBase class
// Parameter    : 
//    offsetAddress     write address
//    trans             payload of transaction
//    t                 bus time
//    debug             true: debug mode; false: normal mode   
// Return value : None
**********************************/
void G4_PEG_DataHandler::write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (t == NULL) {
        return;
    }

    if ((mPCLKPeriod != 0)) {
        if (debug || (!debug && !mRstState)) {
            if (AccessJudgment(trans, debug)) {
                if (debug || (BusBridgeCoreBase<BUS_WIDTH_VCI, BUS_WIDTH_VCI>::mTransType == LT)) {// LT mode
                    mMasterIf->write(trans, debug, *t, false);
                } else {
                    /// Do not support AT mode
                }
            } else {
                SC_REPORT_INFO("G4_PEG", "Access is prohibited.\n");
            }
        } else {
            SC_REPORT_WARNING("G4_PEG","The writing transaction is fail because the reset is active");
        }
    } else {
        SC_REPORT_INFO("G4_PEG", "The writing transaction is fail because clock is zero.\n");
    }
}//}}}

/*********************************
// Function     : AccessJudgment
// Description  : Check access ability
// Parameter    : 
//    trans       issued transaction
//    debug       true: debug mode; false: normal mode
// Return value : 
//   true         access permitted
//   false        access prohibited
**********************************/
bool G4_PEG_DataHandler::AccessJudgment(TlmBasicPayload &trans, bool debug)
{//{{{
    TlmVciExtension *VciExt;
    trans.get_extension(VciExt);
    assert(VciExt != NULL);
    TlmVciExtension::VciRequest_t request_type = VciExt->getRequestType();

    TlmG3mExtension *ExtensionInfo;
    trans.get_extension(ExtensionInfo);
    assert(ExtensionInfo != NULL);
    bus_master_UM     = ExtensionInfo->isUserMode();
    bus_master_SPID   = ExtensionInfo->getSpId();
    bus_master_PEID   = ExtensionInfo->getPeId();
    bus_master_SEC    = ExtensionInfo->isSecure();
    
    if (trans.is_read()){
        mTrans_type = emREAD;
        
        /// If it is Read Modify Write, access is permitted
        if (request_type == TlmVciExtension::CMD_BITOP_SET || 
            request_type == TlmVciExtension::CMD_BITOP_XOR || 
            request_type == TlmVciExtension::CMD_CAXI) {
                
            return true;
        }            
    }
    else {
        mTrans_type = emWRITE;
    }

    /// If all channels disabled guard setting (GEN = 0), access is permitted 
    unsigned int i = 0;
    while(i < mChannelNum) {
        if(mG4_PEG_AgentController->getPEGROT_GEN(i)) {
            break;
        } 
        i++;
        if(i >= mChannelNum) {
            return true;
        }
    } 

    /// Check permission in each channel
    for (i = 0; i < mChannelNum; i++) {
        mADDR  = (unsigned int)trans.get_address();
        mSPID  = mG4_PEG_AgentController->getPEGSPID_SPID(i);
        mGEN   = mG4_PEG_AgentController->getPEGROT_GEN(i);
        mDBG   = mG4_PEG_AgentController->getPEGROT_DBG(i);
        mUM    = mG4_PEG_AgentController->getPEGROT_UM(i);
        mWG    = mG4_PEG_AgentController->getPEGROT_WG(i);
        mRG    = mG4_PEG_AgentController->getPEGROT_RG(i);
        
        if (ChannelJudgment(debug, i))
        {
               std::stringstream sstream;
               sstream << "Access is matched with permitted area of channel " << i << "\n";
            SC_REPORT_INFO("G4_PEG", sstream.str().c_str());
            return true;
        }
    }

    /// Check permission in all channels is failed, access is prohibited
    /// Error is recorded.
    mSPID    = bus_master_SPID;
    mBMID    = bus_master_PEID;
    mDBG     = debug;
    mUM      = bus_master_UM;
    mWRITE   = mTrans_type;
    mSecure  = bus_master_SEC;
    mIsSpcLd = ExtensionInfo->isSpcLd();
    mTransErrorEvent.notify();
    return false;
}//}}}

/*********************************
// Function     : ChannelJudgment
// Description  : Check access ability in a channel
// Parameter    : 
//    debug           true: debug mode; false: normal mode
//    channel_id      id of channel
// Return value : 
//   true         channel pass
//   false        channel failed
**********************************/
bool G4_PEG_DataHandler::ChannelJudgment(bool debug, int channel_id)
{//{{{
    /// If channel guard setting is disabled (GEN = 1), channel failed
    if(!mGEN) {
        return false;
    }
    
    /// Access region matched (BAD, ADV), channel failed
    if(!CheckTransAddress(mADDR,channel_id)) {
        return false;
    }

    /// Debug master access, Debug master priviledge (DBG = 1), channel pass
    if (debug && (bus_master_PEID == 0xFF) && mDBG) {
        return true;
    }
    
    /// User master access, User master limitted (UM = 0), channel failed
    if (bus_master_UM && !mUM) {
        return false;
    }
    
    /// Access SPID permitted, channel pass
    if(((mSPID >> bus_master_SPID) & 0x1) == 0x1) {
        return true;
    }
    
    /// Write access
    if(mTrans_type) {
        /// Master writable (WG = 1), channel pass
        if(mWG) {
            return true;
        } else { /// channel failed
            return false;
        }
    } else { /// Read access
        /// Master readable (RG = 1), channel pass
        if(mRG){
            return true;
        } 
    }
    /// channel failed
    return false;
}//}}}

/*********************************
// Function     : CheckTransAddress
// Description  : Check access region matching
// Parameter    : 
//    trans_addr      address of issued transaction
//    channel_id      id of channel
// Return value : 
//   true             access region matched
//   false            access region not matched
**********************************/
bool G4_PEG_DataHandler::CheckTransAddress(const unsigned int trans_addr, unsigned int channel_id)
{//{{{
    unsigned int bad = mG4_PEG_AgentController->getPEGBAD_BAD(channel_id);
    unsigned int adv = mG4_PEG_AgentController->getPEGADV_ADV(channel_id) | 0xFC000000;
    unsigned int bitwise = (trans_addr ^ bad) & adv;
    if(bitwise == 0) {
        return true;
    }
    else {
        return false;
    }
}//}}}

/*********************************
// Function     : EnableReset
// Description  : Execute reset operation
// Parameter    :
//    is_active   true: reset; false: not reset
// Return value : None
**********************************/
void G4_PEG_DataHandler::EnableReset (const bool is_active)
{//{{{
    mRstState = is_active;
    if (is_active) {
        /// Cancel events
        CancelEvents();
        /// Initialize variable
        Initialize();
    } else {
        /// Do not reset
    }
}//}}}

/*********************************
// Function     : CancelEvents
// Description  : Cancel operation events
// Parameter    : None
// Return value : None
**********************************/
void G4_PEG_DataHandler::CancelEvents (void)
{//{{{
    mTransErrorEvent.cancel();
}//}}}

/*********************************
// Function     : SetCLKPeriod
// Description  : Set PCLK period
// Parameter    :
//   clk_period         new value of PCLK
// Return value : None
**********************************/
void G4_PEG_DataHandler::SetCLKPeriod (const sc_dt::uint64 clk_period)
{//{{{
    mPCLKPeriod = clk_period;
}//}}}

/*********************************
// Function     : GetmADDR
// Description  : Get value of mADDR variable
// Parameter    : None
// Return value : value of mADDR
**********************************/
unsigned int G4_PEG_DataHandler::GetmADDR()
{//{{{
    return mADDR;
}//}}}

/*********************************
// Function     : GetmSPID
// Description  : Get value of mSPID variable
// Parameter    : None
// Return value : value of mSPID
**********************************/
unsigned int G4_PEG_DataHandler::GetmSPID()
{//{{{
    return mSPID;
}//}}}

/*********************************
// Function     : GetmSPID
// Description  : Get value of mBMID variable
// Parameter    : None
// Return value : value of mBMID
**********************************/
unsigned int G4_PEG_DataHandler::GetmBMID()
{//{{{
    return mBMID;
}//}}}

/*********************************
// Function     : GetmDBG
// Description  : Get value of mDBG variable
// Parameter    : None
// Return value : value of mDBG
**********************************/
bool G4_PEG_DataHandler::GetmDBG()
{//{{{
    return mDBG;
}//}}}

/*********************************
// Function     : GetmUM
// Description  : Get value of mUM variable
// Parameter    : None
// Return value : value of mUM
**********************************/
bool G4_PEG_DataHandler::GetmUM()
{//{{{
    return mUM;
}//}}}

/*********************************
// Function     : GetmWRITE
// Description  : Get value of mWRITE variable
// Parameter    : None
// Return value : value of mWRITE
**********************************/
bool G4_PEG_DataHandler::GetmWRITE()
{//{{{
    return mWRITE;
}//}}}

/*********************************
// Function     : GetmSecure
// Description  : Get value of mSecure variable
// Parameter    : None
// Return value : value of mSecure
**********************************/
bool G4_PEG_DataHandler::GetmSecure()
{//{{{
    return mSecure;
}//}}}

/*********************************
// Function     : IsSpcLd
// Description  : Get value of mIsSpcLd variable
// Parameter    : None
// Return value : value of mSecure
**********************************/
bool G4_PEG_DataHandler::IsSpcLd()
{//{{{
    return mIsSpcLd;
}//}}}
// vim600: set foldmethod=marker :
