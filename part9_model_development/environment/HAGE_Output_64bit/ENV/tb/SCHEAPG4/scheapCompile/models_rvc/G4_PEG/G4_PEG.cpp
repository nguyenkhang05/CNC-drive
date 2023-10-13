// ---------------------------------------------------------------------
// $Id: G4_PEG.cpp,v 1.11 2020/01/14 02:39:30 nhutminhdang Exp $
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

#include "G4_PEG.h"
#include "G4_PEG_DataHandler.h"
#include "G4_PEG_Func.h"

/*********************************
// Function     : G4_PEG
// Description  : Constructor of G4_PEG class
// Parameter    : 
//   name            module name
//   kind            platform kind
//   rLatency        read latency
//   wLatency        write latency
//   PEG_ID          ID of PE
//   CHANNEL_NUM     number of channel
// Return value : None
**********************************/
G4_PEG::G4_PEG (sc_module_name name, std::string kind, unsigned int rLatency, unsigned int wLatency, unsigned int PEG_ID, unsigned int CHANNEL_NUM)
                :G4_PEG_AgentController()
                ,BusBridgeModuleBase<BUS_WIDTH_VCI,BUS_WIDTH_VCI>(name)
                ,BusBridgeSlaveBase<BUS_WIDTH_VCI,1>()
                ,BusMasterBase<BUS_WIDTH_VCI,1>()
                ,BusSlaveBase<BUS_WIDTH_APB,1>()
                ,rvc_common_model()
                ,PCLK("PCLK")
                ,PEG_ERR_S("PEG_ERR_S")
                ,PEG_ERR_M("PEG_ERR_M")
{//{{{
    /// Initialize variables
    mPEID = PEG_ID;
    mChannelNum = CHANNEL_NUM;
    mKind = kind;
    mPCLKPeriod   = 0;
    mStartPCLKTime = 0;
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    mIsResetCmdActive      = false;
    mIsResetHardActive     = false;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    Initialize();
     
    /// Initialize output ports
    PEG_ERR_S.initialize(mPEG_ERR_S);
    PEG_ERR_M.initialize(mPEG_ERR_M);
    
    /// Connect ports
    setMasterResetPort64(&resetPort);
    setMasterFreqPort64(&PCLK);
    setInitiatorSocket64("is");

    setSlaveFreqPort32(&PCLK);
    setSlaveResetPort32(&resetPort);
    setTargetSocket32("tsp");

    setSlaveResetPort(&resetPort);
    setSlaveFreqPort(&PCLK);
    setTargetSocket("tsv");

    /// Alias for binding
    tsp = BusSlaveBase<BUS_WIDTH_APB,1>::tSocket32[0]; 
    tsv = BusBridgeSlaveBase<BUS_WIDTH_VCI,1>::tSocket[0];
    is  = iSocket64[0]; 

    /// Instance G4_PEG_Func
    mG4_PEG_Func = new G4_PEG_Func((std::string)name, mKind, this, mPEID);
    sc_assert(mG4_PEG_Func != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mG4_PEG_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    /// Instance G4_PEG_DataHandler
    SIM_MODE_T simmode = SIM_MODE_FAST;
    mPEG_DataHandler = new G4_PEG_DataHandler((std::string)name, this, this, mBusSlaveIf[0], mBusMasterIf64[0], simmode, mChannelNum);
    sc_assert(mPEG_DataHandler != NULL);
    mpCore = mPEG_DataHandler;
    mBusMasterIf64[0]->setFuncModulePtr((BusMasterFuncIf *)mPEG_DataHandler);
    mBusMasterIf64[0]->setBusProtocol(BUS_VCI);
    mBusMasterIf64[0]->setTransNmbLmt(0x1);
    mBusSlaveIf[0]->setFuncModulePtr( (BusSlaveFuncIf *)mPEG_DataHandler);
    mBusSlaveIf[0]->setBusProtocol(BUS_VCI);
    mBusSlaveIf[0]->setTransNmbLmt(0x1);
    mBusSlaveIf[0]->setEnableToCheckDataSize();

    /// SC_METHOD
    SC_METHOD(WritePEG_ERR_MMethod);
    dont_initialize();
    sensitive << mWritePEG_ERR_MEvent;

    SC_METHOD(WritePEG_ERR_SMethod);
    dont_initialize();
    sensitive << mWritePEG_ERR_SEvent;
    
    SC_METHOD(NegatePEG_ERR_MMethod);
    dont_initialize();
    sensitive << mNegatePEG_ERR_MEvent;

    SC_METHOD(NegatePEG_ERR_SMethod);
    dont_initialize();
    sensitive << mNegatePEG_ERR_SEvent;

    SC_METHOD(HandleErrorMethod);
    dont_initialize();
    sensitive << mPEG_DataHandler->mTransErrorEvent;
}//}}}

/*********************************
// Function     : G4_PEG
// Description  : Destructor of G4_PEG class
// Parameter    : None
// Return value : None
**********************************/
G4_PEG::~G4_PEG (void)
{//{{{
     delete mG4_PEG_Func;
     delete mPEG_DataHandler;
}//}}}

/*********************************
// Function     : Initialize
// Description  : Initialize when reset activated
// Parameter    : None
// Return value : None
**********************************/
void G4_PEG::Initialize(void)
{//{{{
    mRstState = false;
    mDumpInterrupt = true;
    mPEG_ERR_S_asserted = false;
    mPEG_ERR_M_asserted = false;
    mPEG_ERR_S.initialize();
    mPEG_ERR_M.initialize();
}//}}}

/*********************************
// Function     : WritePEG_ERR_MMethod
// Description  : Write PEG_ERR_M port
// Parameter    : None
// Return value : None
**********************************/
void G4_PEG::WritePEG_ERR_MMethod()
{//{{{
    PEG_ERR_M.write(mPEG_ERR_M);
    if (mDumpInterrupt){
        if(mPEG_ERR_M_asserted) {
            re_printf("info", "Port PEG_ERR_M is asserted\n");
        } else {
            re_printf("info", "Port PEG_ERR_M is de-asserted\n");
        }
    } 
    /// Negate after one clock period
    if(mPEG_ERR_M_asserted) {
        mNegatePEG_ERR_MEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}}

/*********************************
// Function     : WritePEG_ERR_SMethod
// Description  : Write PEG_ERR_S port
// Parameter    : None
// Return value : None
**********************************/
void G4_PEG::WritePEG_ERR_SMethod()
{//{{{
    PEG_ERR_S.write(mPEG_ERR_S);
    if (mDumpInterrupt){
        if(mPEG_ERR_S_asserted) {
            re_printf("info", "Port PEG_ERR_S is asserted\n");
        } else {
            re_printf("info", "Port PEG_ERR_S is de-asserted\n");
        }
    }
    /// Negate after one clock period
    if(mPEG_ERR_S_asserted) {
        mNegatePEG_ERR_SEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}}

/*********************************
// Function     : NegatePEG_ERR_MMethod
// Description  : Negate PEG_ERR_M port
// Parameter    : None
// Return value : None
**********************************/
void G4_PEG::NegatePEG_ERR_MMethod()
{//{{{
    mPEG_ERR_M_asserted = false;
    mPEG_ERR_M.initialize();
    mWritePEG_ERR_MEvent.notify();
}//}}}

/*********************************
// Function     : NegatePEG_ERR_SMethod
// Description  : Negate PEG_ERR_S port
// Parameter    : None
// Return value : None
**********************************/
void G4_PEG::NegatePEG_ERR_SMethod()
{//{{{
    mPEG_ERR_S_asserted = false;
    mPEG_ERR_S.initialize();
    mWritePEG_ERR_SEvent.notify();
}//}}}

/*********************************
// Function     : HandleErrorMethod
// Description  : Handle error infomation
// Parameter    : None
// Return value : None
**********************************/
void G4_PEG::HandleErrorMethod(void)
{//{{{
    bool            PEGWrERR    = mPEG_DataHandler->GetmWRITE();
    bool            PEGRdERR    = !PEGWrERR;
    bool            UM          = mPEG_DataHandler->GetmUM();
    bool            DBG         = mPEG_DataHandler->GetmDBG();
    unsigned char   SPID        = mPEG_DataHandler->GetmSPID();
    unsigned int    BMID        = mPEG_DataHandler->GetmBMID();
    unsigned int    ErrAddr     = mPEG_DataHandler->GetmADDR();        
    bool            isSpcLd     = mPEG_DataHandler->IsSpcLd();
    bool            Secure      = mPEG_DataHandler->GetmSecure();
    
    /// Speculative read, error infomation issued to output port PEG_ERR_M
    if(BMID != mPEID && PEGRdERR && isSpcLd) {
        mPEG_ERR_M.PEGRdERR = PEGRdERR;
        mPEG_ERR_M.PEGWrERR = PEGWrERR;
        mPEG_ERR_M.SPID     = SPID    ;
        mPEG_ERR_M.BMID     = BMID    ;
        mPEG_ERR_M.ErrAddr  = ErrAddr ;
        mPEG_ERR_M.UM       = UM      ;
        mPEG_ERR_M.DBG      = DBG     ;
        mPEG_ERR_M.SEC      = Secure  ;
        
        mPEG_ERR_M_asserted = true;
        mWritePEG_ERR_MEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, true, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
    } else { /// Un-speculative read/ Write, error infomation issued to output port PEG_ERR_S
        mPEG_ERR_S.PEGRdERR = PEGRdERR;
        mPEG_ERR_S.PEGWrERR = PEGWrERR;
        mPEG_ERR_S.SPID     = SPID    ;
        mPEG_ERR_S.BMID     = BMID    ;
        mPEG_ERR_S.ErrAddr  = ErrAddr ;
        mPEG_ERR_S.UM       = UM      ;
        mPEG_ERR_S.DBG      = DBG     ;
        mPEG_ERR_S.SEC      = Secure  ;
        
        mPEG_ERR_S_asserted = true;
        mWritePEG_ERR_SEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, true, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
    }
}//}}}

/*********************************
// Function     : CheckClockPeriod
// Description  : Check clock period value
// Parameter    : None
// Return value : None
**********************************/
bool G4_PEG::CheckClockPeriod ()
{//{{{
    return (bool) (mPCLKPeriod > 0);
}//}}}

/*********************************
// Function     : GetResetStatus
// Description  : Checkreset status
// Parameter    : None
// Return value : None
**********************************/
bool G4_PEG::GetResetStatus()
{//{{{
    return (mIsResetCmdActive || mIsResetHardActive);
}//}}}

/*********************************
// Function     : CancelEvents
// Description  : Cancel operation events
// Parameter    : None
// Return value : None
**********************************/
void G4_PEG::CancelEvents (void)
{//{{{
    mWritePEG_ERR_MEvent.cancel();
    mWritePEG_ERR_SEvent.cancel();
    mNegatePEG_ERR_MEvent.cancel();
    mNegatePEG_ERR_SEvent.cancel();
}//}}}

/*********************************
// Function     : RegisterHandler
// Description  : Wrapper for accessing registers
// Parameter    :
//      cmd       access command
// Return value : None
**********************************/
void G4_PEG::RegisterHandler (const std::vector<std::string> cmd)
{//{{{
    mG4_PEG_Func->RegisterHandler(cmd);
}//}}}

/*********************************
// Function     : EnableReset
// Description  : Execute reset operation
// Parameter    :
//    is_active   true: reset; false: not reset
// Return value : None
**********************************/
void G4_PEG::EnableReset (const bool is_active)
{//{{{
    mRstState = is_active;
    mG4_PEG_Func->EnableReset(is_active);
    mPEG_DataHandler->EnableReset(is_active);
    if (is_active) {
        /// Cancel events
        CancelEvents();
        /// Initialize variable
        Initialize();
        /// Initialize output ports
        mWritePEG_ERR_SEvent.notify();
        mWritePEG_ERR_MEvent.notify();
    }
}//}}}

/*********************************
// Function     : DumpInterrupt
// Description  : Enable/ Disable dumping interrupt information
// Parameter    :
//    is_active   true: dump; false: not dump
// Return value : None
**********************************/
void G4_PEG::DumpInterrupt (const bool is_enable)
{//{{{
    mDumpInterrupt = is_enable;
}//}}}

/*********************************
// Function     : SetCLKPeriod
// Description  : Set PCLK period
// Parameter    :
//     clk_period    new value of PCLK
// Return value : None
**********************************/
void G4_PEG::SetCLKPeriod (const sc_dt::uint64 clk_period, const double time_point)
{//{{{
    mPCLKPeriod = clk_period;
    mStartPCLKTime = time_point;
    mPEG_DataHandler->SetCLKPeriod(clk_period);
}//}}}

/*********************************
// Function     : SetResetStatus
// Description  : Set reset status
// Parameter    :
//   mIsResetCmdActive     is reset by command
//   mIsResetHardActive    is reset by port
// Return value : None
**********************************/
void G4_PEG::SetResetStatus(bool mIsResetCmdActive, bool mIsResetHardActive)
{//{{{
    this->mIsResetCmdActive = mIsResetCmdActive;
    this->mIsResetHardActive = mIsResetHardActive;
}//}}}

/*********************************
// Function     : getPEGSPID_SPID
// Description  : Get value of PEGSPID register
// Parameter    : 
//     channel_id      id of channel
// Return value : Value of PEGSPID register
**********************************/
unsigned int G4_PEG::getPEGSPID_SPID(unsigned int channel_id)
{//{{{
    return mG4_PEG_Func->getPEGSPID_SPID(channel_id);
}//}}}

/*********************************
// Function     : getPEGROT_GEN
// Description  : Get value of GEN bit from PEGPROT register
// Parameter    : 
//     channel_id      id of channel
// Return value : Value of bit GEN
**********************************/
bool G4_PEG::getPEGROT_GEN(unsigned int channel_id)
{//{{{
    return mG4_PEG_Func->getPEGROT_GEN(channel_id);
}//}}}

/*********************************
// Function     : getPEGROT_DBG
// Description  : Get value of DBG bit from PEGPROT register
// Parameter    : 
//     channel_id      id of channel
// Return value : Value of bit DBG
**********************************/
bool G4_PEG::getPEGROT_DBG(unsigned int channel_id)
{//{{{
    return mG4_PEG_Func->getPEGROT_DBG(channel_id);
}//}}}

/*********************************
// Function     : getPEGROT_UM
// Description  : Get value of UM bit from PEGPROT register
// Parameter    : 
//     channel_id      id of channel
// Return value : Value of bit UM
**********************************/
bool G4_PEG::getPEGROT_UM(unsigned int channel_id)
{//{{{
    return mG4_PEG_Func->getPEGROT_UM(channel_id);
}//}}}

/*********************************
// Function     : getPEGROT_WG
// Description  : Get value of WG bit from PEGPROT register
// Parameter    : 
//     channel_id      id of channel
// Return value : Value of bit WG
**********************************/
bool G4_PEG::getPEGROT_WG(unsigned int channel_id)
{//{{{
    return mG4_PEG_Func->getPEGROT_WG(channel_id);
}//}}}

/*********************************
// Function     : getPEGROT_RG
// Description  : Get value of RG bit from PEGPROT register
// Parameter    : 
//     channel_id      id of channel
// Return value : Value of bit RG
**********************************/
bool G4_PEG::getPEGROT_RG(unsigned int channel_id)
{//{{{
    return mG4_PEG_Func->getPEGROT_RG(channel_id);
}//}}}

/*********************************
// Function     : getPEGBAD_BAD
// Description  : Get value of PEGBAD register
// Parameter    : 
//     channel_id      id of channel
// Return value : Value of PEGBAD register 
**********************************/
unsigned int G4_PEG::getPEGBAD_BAD(unsigned int channel_id)
{//{{{
    return mG4_PEG_Func->getPEGBAD_BAD(channel_id);
}//}}}

/*********************************
// Function     : getPEGADV_ADV
// Description  : Get value of PEGADV register
// Parameter    : 
//     channel_id      id of channel
// Return value : Value of PEGADV register
**********************************/
unsigned int G4_PEG::getPEGADV_ADV(unsigned int channel_id)
{//{{{
    return mG4_PEG_Func->getPEGADV_ADV(channel_id);
}//}}}

/*********************************
// Function     : SetPeId
// Description  : Set PE ID for G4_PEG module
// Parameter    :
//   peid         new value of peid
// Return value : None
**********************************/
void G4_PEG::SetPeId (unsigned int peid) 
{//{{{
    mPEID = peid;
    mG4_PEG_Func->SetPeId(peid);
}//}}}
// vim600: set foldmethod=marker :
