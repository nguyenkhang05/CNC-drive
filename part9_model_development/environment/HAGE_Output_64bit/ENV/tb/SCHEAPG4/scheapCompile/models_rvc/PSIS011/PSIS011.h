// ---------------------------------------------------------------------
// $Id: PSIS011.h,v 1.7 2018/02/12 04:55:20 chanle Exp $
//
// Copyright(c) 2017 Renesas Electronics Corporation
// Copyright(c) 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __PSIS011_H__
#define __PSIS011_H__

#include "re_define.h"
#include "BusSlaveBase.h"
#include "PSIS011_AgentController.h"
#include <list>

class PSIS011_Func;
class PSIS011;

//=============================================
//============= Class Cwdt ====================
//=============================================
class Cwdt: public sc_module
{
public:
    sc_out<bool>    wdt_error;  // WDT error output

private:
    // variables
    sc_dt::uint64   mClockPeriod;           // Clock period
    unsigned int    mPrescaler;             // Pre-scaler 
    unsigned int    mExpired;               // Expired value
    sc_time_unit    mTimeResolutionUnit;    // Time resolution unit of simulation
    bool            mWDTError;              // Store value to issue to wdt_error port


    //events
    sc_event        mWriteWDTErrorEvent;    // Trigger WriteWDTErrorMethod
    sc_event        mNegateWDTErrorEvent;   // Trigger NegateWDTErrorMethod

public:
    SC_HAS_PROCESS (Cwdt);
    Cwdt (sc_module_name name);
    ~Cwdt ();

    void EnableReset(const bool is_active);
    void ConfigWDTClock(sc_dt::uint64 period, sc_time_unit time_unit);
    void ConfigWDTCounter(unsigned int prescaler, unsigned int expired);
    void StartWDT();
    void StopWDT();

private:
    // Thread/Method
    void WriteWDTErrorMethod();
    void NegateWDTErrorMethod();

    // Function
    void Initialize();
};

//=============================================
//============= Class Csync_pulse==============
//=============================================
class Csync_pulse: public sc_module
{
public:
    sc_in<bool>     psis_trg_sync_chn;  // Trigger sync pulse from GTM
    sc_out<bool>    sync_pulse;         // Sync pulse output

private:
    // variables
    sc_signal<bool> inner_sync_pulse_sig;       // Inner sync pulse signal

    sc_time_unit    mTimeResolutionUnit;        // Time resolution unit of simulation
    bool            mEnableGenInnerSyncPulse;   // Indicate can generate sync pulse or not
    unsigned int    mSyncPulseSelect;           // Sync pulse select from GTM or internal 
    unsigned int    mPrescaler; // Pre-scaler 
    unsigned int    mExpired;   // Expired value
    sc_dt::uint64   mPCLKPeriod;// PCLK period for counting
    PSIS011         *mpPSIS011; // PSIS011 pointer


    //events
    sc_event        mGenInnerSyncPulseEvent;    // Trigger GenInnerSyncPulseThread

public:
    SC_HAS_PROCESS (Csync_pulse);
    Csync_pulse (sc_module_name name, PSIS011 *psis011);
    ~Csync_pulse ();

    void EnableReset(const bool is_active);
    void ConfigInnerSyncPulse(unsigned int select, unsigned int prescaler, unsigned int expired);
    void StartGenInnerSyncPulse();
    void StopGenInnerSyncPulse();
    void ConfigSyncPulseClock(sc_dt::uint64 period, sc_time_unit time_unit);

private:
    // Thread/Method
    void GenInnerSyncPulseThread();
    void IssueSelectedSyncPulseMethod();

    // Function
    void Initialize();
};


//=============================================
//============= Class Ctimestamp ==============
//=============================================
class Ctimestamp: public sc_module
{
public:
    sc_in<bool>             psis_clk_timestamp;     // Timestamp clock
    sc_in<bool>             psis_clr_timestamp;     // Timestamp clear
    sc_in<bool>             psis_stsp_timestamp;    // Timestamp enable
    sc_in<bool>             internal_clock;         // Internal clock for counting timestamp
private:
    // variables
    bool            mIsReset;

    unsigned int    mCounter;           // value of timestamp counter
    bool            mClearSelectGTM;    // Indicate clear signal for timestamp is selected from GTM
    bool            mEnableSelectGTM;   // Indicate the enable signal for timestamp is selected from GTM
    bool            mClockSelectGTM;    // Indicate the clock input to timestamp is from GTM
    bool            mTSInnerEnable;     // Indicate value of (ATSCEB || TSCxEB), in this x is A/B

    //events
    sc_event        mClearTimestampEvent;   // Trigger ClearTimestampMethod

public:
    SC_HAS_PROCESS (Ctimestamp);
    Ctimestamp (sc_module_name name);
    ~Ctimestamp ();

    unsigned int GetTimestamp();
    void EnableReset(const bool is_active);
    void NotifyClearTimestamp();
    void ConfigTimestamp(bool clear_select_gtm, bool enable_select_gtm, bool clock_select_gtm);
    void SetTSInnerEnable(bool ts_inner_enable);

private:
    // Thread/Method
    void ClearTimestampMethod();
    void MonitorGTMClockMethod();
    void MonitorInternalClockMethod();

    // Function
    void Initialize();
    bool CheckTimestampEnable();
};

//=============================================
//============= Class PSIS011 =================
//=============================================
class PSIS011: public sc_module
            ,public PSIS011_AgentController
            ,public BusSlaveBase<32, 1>
{
private:
    // Enums
    enum eClockInfo{
        emPCLKId                = 0
        ,empsis_clkId           = 1
        ,empsis_mult_clkId      = 2
        ,emNumClk               = 3
    };

    enum eResetInfo{
        emPRESETnId             = 0
        ,empsis_rst_nId         = 1
        ,empsis_mult_rst_nId    = 2
        ,emNumReset             = 3
    };

    enum eMode{
        emConfigurationMode
        ,emPSI5Mode
        ,emUARTMode
    };

    enum eReceptionState{
        emIdleReceptionState
        ,emStartReceptionState
        ,emStopReceptionState
        ,emNoneReceptionState
    };

    enum eGeneralInfo{
        emSizeOfValidFPKTvsFPAYLDTable  = 42  // (28payld - 20payld + 1) * 2rfpcs
        ,emInitCRCVal                   = 0x7 // 3'b111
        ,emInitCRCExtraVal              = 0x15// 6'b010101
        ,emCRCPolynomial                = 0xB // 0x1011 = x^3 + x + 1
        ,emCRCExtraPolynomial           = 0x59// 0x1011001 = x^6 + x^4 + x^3 + 1
        ,emFullFIFO                     = 15  // size of request FIFO
        ,emNumChannel                   = 8   // 8 channels
    };

    enum eStrobe{
        emIdleStrobe    = 0
        ,emStartStrobe  = 1
        ,emStopStrobe   = 2
        ,emAbortStrobe  = 3
    };

public:
    // Clock/Reset
    sc_in<sc_dt::uint64>    PCLK;               // clock for APB
    sc_in<sc_dt::uint64>    psis_clk;           // communication clock
    sc_in<sc_dt::uint64>    psis_mult_clk;      // communication multiply clock
    sc_in<bool>             PRESETn;            // reset of domain PCLK
    sc_in<bool>             psis_rst_n;         // reset of domain psis_clk
    sc_in<bool>             psis_mult_rst_n;    // reset of domain psis_mult_clk

    // UART
    sc_in<unsigned int>     RX_DATA;            // UART Rx data
    sc_in<unsigned int>     RX_CONTROL;         // UART Rx control
    sc_out<unsigned int>    TX_DATA;            // UART Tx data
    sc_out<unsigned int>    TX_CONTROL;         // UART Tx control

    // Interrupt and DMA request
    sc_out<bool>            *int_psis_chn[emNumChannel];        // Interrupt of channel n (n: 0-7)
    sc_out<bool>            *dma_psis_chn_rx[emNumChannel];     // DMA transfer request Rx of channel n (n: 0-7)
    sc_out<bool>            *dma_psis_chn_tx[emNumChannel - 1]; // DMA transfer request Tx of channel n (n: 1-7)

    // GTM interface
    sc_in<bool>             *psis_trg_sync_chn[emNumChannel];   // Sync pulse of channel n (n: 0-7)
    sc_in<bool>             psis_clk_timestamp_a;   // Timestamp clock A
    sc_in<bool>             psis_clk_timestamp_b;   // Timestamp clock B
    sc_in<bool>             psis_clr_timestamp_a;   // Timestamp clear A
    sc_in<bool>             psis_clr_timestamp_b;   // Timestamp clear B
    sc_in<bool>             psis_stsp_timestamp_a;  // Timestamp start/stop A
    sc_in<bool>             psis_stsp_timestamp_b;  // Timestamp start/stop B


    TlmTargetSocket<32>     *ts; // target sockets 


private:
    // Structure
    struct sChannelInfo{
        unsigned int ch_en;
        bool is_sync;
        bool is_wdt_enabled;
        bool wdt_error;
        unsigned int ts_sel;
        unsigned int ts_en;
        unsigned int ts_trg_sel;
        unsigned int dci_counter;
        unsigned int async_frm_cnt;
        unsigned int wdt_frm_cnt;
        sChannelInfo (){
            this->ch_en = 0;
            this->is_sync = 0;
            this->is_wdt_enabled = 0;
            this->wdt_error = 0;
            this->ts_sel = 0;
            this->ts_en = 0;
            this->ts_trg_sel = 0;
            this->dci_counter = 0;
            this->async_frm_cnt = 0;
            this->wdt_frm_cnt = 0;
        }
        void Initialize(){
            this->ch_en = 0;
            this->is_sync = 0;
            this->is_wdt_enabled = 0;
            this->wdt_error = 0;
            this->ts_sel = 0;
            this->ts_en = 0;
            this->ts_trg_sel = 0;
            this->dci_counter = 0;
            this->async_frm_cnt = 0;
            this->wdt_frm_cnt = 0;
        }
    };

    // Variables
    sc_signal<bool> *selected_sync_pulse_sig_chn[emNumChannel]; // Selected sync pulse from GTM and inner
    sc_signal<bool> timestamp_inner_clock_sig;          // Inner clock for timestamp counting
    sc_signal<bool> *wdt_error_sig_chn[emNumChannel];   // WDT error from channel n
    sc_signal<unsigned int> rx_control_sig;             // RX CONTROL after DNF
    sc_signal<unsigned int> rx_data_sig;                // RX DATA after DNF
    sc_signal<bool> psis_clk_timestamp_a_sig;           // Connect psis_clk_timestamp_a to mpTimestampA
    sc_signal<bool> psis_clr_timestamp_a_sig;           // Connect psis_clr_timestamp_a to mpTimestampA
    sc_signal<bool> psis_stsp_timestamp_a_sig;          // Connect psis_stsp_timestamp_a to mpTimestampA
    sc_signal<bool> psis_clk_timestamp_b_sig;           // Connect psis_clk_timestamp_b to mpTimestampB
    sc_signal<bool> psis_clr_timestamp_b_sig;           // Connect psis_clr_timestamp_b to mpTimestampB
    sc_signal<bool> psis_stsp_timestamp_b_sig;          // Connect psis_stsp_timestamp_b to mpTimestampB
    sc_signal<bool> *tx_req_sig[emNumChannel];          // Connect tx request to arbiter
    sc_signal<bool> sw_rst_sig;                         // Software reset signal


    sc_dt::uint64   mClkPeriod[emNumClk];       // Period of clock
    sc_dt::uint64   mClkOrgFreq[emNumClk];      // Previous frequency of clock
    sc_dt::uint64   mClkFreq[emNumClk];         // Frequency of clock
    std::string     mClkFreqUnit[emNumClk];     // Frequency unit of clock
    double          mStartClkTime[emNumClk];    // Start time of clock

    bool            mResetCurVal[emNumReset]; //Store current value of reset signals
    bool            mResetPreVal[emNumReset]; //Store previous value of reset signals
    bool            mIsResetHardActive[emNumReset]; // Is reset by port
    bool            mIsResetCmdReceive[emNumReset]; // Is receive cmd reset
    bool            mIsResetCmdActive[emNumReset];  // Is reset by cmd
    double          mResetCmdPeriod[emNumReset];    // Period of cmd reset

    sc_dt::uint64   mTimeResolutionValue;   // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;    // Time resolution unit of simulation

    bool            mDumpInterrupt; // Indicate dump info of interrupt or not
    unsigned int    mDNFDelay;      // Delay cycle at Noise Filter (psis_clk cycle)

    eMode           mMode;      // Indicate mode
    unsigned int    mValidFPKTvsFPAYLDTable[42];    // Store a combine of valid fpayld, rfcps, fpkt
    unsigned int    mTSInnerCycle;          // Cycle (after precaler) used as inner clock for timestamp
    sChannelInfo    mChannel[emNumChannel]; // Store info/config of 8 channels
    unsigned int    mMaxIdleCycle;          // Maximum frame idle (gap)
    unsigned int    mURPRTY;                // UART Rx parity option
    unsigned int    mUTPRTY;                // UART Tx parity option
    unsigned int    mDivider;               // Total cycle to receive/transmit 1 bit (clock divider)
    unsigned int    mTxCLKDivider;          // Clock divider for output clock psis_tx_sclk
    unsigned int    mRX_CONTROL;            // Store value of RX_CONTROL input port
    unsigned int    mRX_DATA;               // Store value of RX_DATA input port
    eReceptionState mReceptionState;        // Indicate receive RX in which state
    unsigned int    mDNF_RX_CONTROL;        // Value of RX_CONTROL read at DNF
    unsigned int    mDNF_RX_DATA;           // Value of RX_DATA read at DNF
    unsigned int    mRxUARTFrmNum;          // Indicate number of received UART frame
    bool            mLostUARTFrame;         // Indicate in receiving PSI5, a UART frame is aborted
    bool            mIsUARTPrityError;      // Indicate having UART parity error
    bool            mIsUARTFramingError;    // Indicate having UART framing error
    unsigned int    mFPKT;                  // Indicate expected number of UART packet in 1 PSI5 frame (in reception)
    unsigned int    mSCKEN;                 // Indicate value of SCKEN bit -> enable or not generate UART clock out
    unsigned int    mNFSET;                 // Indicate value of NFSET bit -> enable use Noise Filter or not
    unsigned int    mFPAYLD;                // Indicate expected number bit of payload in PSI5 frame (in reception)
    unsigned int    mCurChid;               // Indicate the current channel ID in header reception
    unsigned int    mCurFid;                // Indicate the current frame ID in header reception
    unsigned int    mCurHeaderStatus;       // Store header status of current PSI5 packet
    unsigned int    mRFCPS;                 // Indicate parity/CRC in received PSI5 frame
    unsigned int    mUTTF;                  // Indicate value of UTTF bit
    bool            mSWRST;                 // Indicate value to write to sw_rst_sig signal
    sc_dt::uint64   mPSI5Packet;            // Received PSI5 Packet (LSB -- MSB)
    bool            mReqArray[emNumChannel];        // Request array to arbiter 0-6 (for channel 1-7), index 7 for Txcommand CPU
    bool            mInterrupt[emNumChannel];       // Store value to write to interrupt ports
    bool            mDMARequestTX[emNumChannel-1];  // DMA request TX for channel 1-7
    bool            mDMARequestRX[emNumChannel];    // DMA request RX for channel 0-7
    bool            mHasWriteUCTD;              // Indicate the UCTD has valid data to continue transmit or not
    bool            mHasPSI5ReceiveError;       // Indicate there is error in receiving PSI5, must store to Ch0, Frm2
    bool            mIsStopDDSR[emNumChannel-1];// Indicate the DDSR of a channel is stop or not
    bool            mLoopbackEnable;            // Indicate loopback is enable or not
    unsigned int    mTX_CONTROL;                // Store value to write to TX_CONTROL port
    unsigned int    mTX_DATA;                   // Store value to write to TX_DATA port
    unsigned int    mNumTXReqInFIFO;            // Indicate the number of TX request in FIFO
    bool            mIgnorePacket;              // Indicate ignore UART packet or not when wrong setting FPKT, FPAYLD
    std::queue<unsigned int>            mReqFIFO;        // Request FIFO (waiting to transfer to outside via UART IF)
    std::map<unsigned int, std::string> mStrobeStringMap;// Mapping strobe to state name

    PSIS011_Func    *mpPSIS011Func;                     // Pointer of PSIS011 Func
    Ctimestamp      *mpTimestampA;                      // Timestamp A
    Ctimestamp      *mpTimestampB;                      // Timestamp B
    Csync_pulse     *mpSyncPulseChannel[emNumChannel];  // 8 internal sync pulse generator (but 0 is not used)
    Cwdt            *mpWDTChannel[emNumChannel];        // WDT for 8 channel


    // Events
    sc_event mClkZeroEvent[emNumClk];           // Notify clock is zero

    sc_event mResetHardEvent[emNumReset];       // Call HandleResetHardMethod when reset is active
    sc_event mResetCmdEvent[emNumReset];        // Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent[emNumReset];  // Event to cancel reset by command
    sc_event mResetEvent[emNumReset];           // Inform has reset

    sc_event mForwardDNFUARTRxEvent;                    // Trigger ForwardDNFUARTRxMethod to issue Rx signal from DNF
    sc_event mEndIdleReceptionStateEvent;               // Trigger EndIdleReceptionStateMethod
    sc_event mMoveToStopReceptionStateEvent;            // Trigger MoveToStopReceptionStateMethod
    sc_event mMoveToIdleReceptionStateEvent;            // Trigger MoveToIdleReceptionStateMethod
    sc_event mGenClockForTimestampEvent;                // Trigger GenClockForTimestampThread
    sc_event mWriteTxReqEvent[emNumChannel];            // Trigger WriteTxReqMethod
    sc_event mNegateTxReqEvent[emNumChannel];           // Trigger NegateTxReqMethod
    sc_event mClearTXSTSBitEvent;                       // Trigger ClearTXSTSBitMethod
    sc_event mClearDDSRSTSBitEvent[emNumChannel-1];     // Trigger ClearDDSRSTSBitMethod , 7 events for channel 1-7
    sc_event mTransUARTDataEvent;                       // Trigger TransUARTThread
    sc_event mTransPSI5Event;                           // Trigger TransPSI5Thread
    sc_event mWriteUARTOutputEvent;                     // Trigger WriteUARTOutputMethod
    sc_event mWriteInterruptEvent[emNumChannel];        // Trigger WriteInterruptMethod
    sc_event mNegateInterruptEvent[emNumChannel];       // Trigger NegateInterruptMethod
    sc_event mWriteDMARequestTXEvent[emNumChannel-1];   // Trigger WriteDMARequestTXMethod
    sc_event mNegateDMARequestTXEvent[emNumChannel-1];  // Trigger NegateDMARequestTXMethod
    sc_event mWriteDMARequestRXEvent[emNumChannel];     // Trigger WriteDMARequestRXMethod
    sc_event mNegateDMARequestRXEvent[emNumChannel];    // Trigger NegateDMARequestRXMethod
    sc_event mClearUTTBBFBitEvent;          // Trigger ClearUTTBBFBitMethod
    sc_event mSetUTTFINBitEvent;            // Trigger SetUTTFINBitMethod
    sc_event mStopDDSREvent[emNumChannel-1];// Trigger TransPSI5Thread
    sc_event mWritesw_rst_sigEvent;         // Trigger Writesw_rst_sigMethod
    sc_event mNegatesw_rst_sigEvent;        // Trigger Negatesw_rst_sigMethod

    // Threads/Methods
    void MonitorPCLKMethod ();
    void Monitorpsis_clkMethod ();
    void Monitorpsis_mult_clkMethod ();

    void MonitorPRESETnMethod ();
    void Monitorpsis_rst_nMethod ();
    void Monitorpsis_mult_rst_nMethod ();
    void HandleResetHardMethod (const unsigned int reset_id);
    void HandleResetCmdMethod (const unsigned int reset_id);
    void CancelResetCmdMethod (const unsigned int reset_id);

    void MonitorSyncPulseMethod(unsigned int chid);
    void MonitorWDTErrorMethod(unsigned int chid);
    void MonitorDNFOutputMethod();
    void ArbitrateTxReqThread();

    void MoveToStopReceptionStateMethod();
    void MoveToIdleReceptionStateMethod();
    void EndIdleReceptionStateMethod();

    void GenClockForTimestampThread();
    void DNFDelayMethod();
    void ForwardDNFUARTRxMethod();
    void WriteTxReqMethod(unsigned int reqid);
    void NegateTxReqMethod(unsigned int reqid);
    void TransPSI5Thread();
    void TransUARTThread();
    void WriteUARTOutputMethod();
    void ClearTXSTSBitMethod();
    void ClearDDSRSTSBitMethod(unsigned int txreqid);
    void WriteInterruptMethod(unsigned int chid);
    void NegateInterruptMethod(unsigned int chid);
    void WriteDMARequestTXMethod(unsigned int txreqid);
    void NegateDMARequestTXMethod(unsigned int txreqid);
    void WriteDMARequestRXMethod(unsigned int chid);
    void NegateDMARequestRXMethod(unsigned int chid);
    void ClearUTTBBFBitMethod();
    void SetUTTFINBitMethod();
    void Writesw_rst_sigMethod();
    void Negatesw_rst_sigMethod();

    // Functions
    void InitializeOfPRESETn (void);
    void InitializeOfpsis_rst_n (void);
    void InitializeOfpsis_mult_rst_n (void);
    void CancelEventsOfPRESETn();
    void CancelEventsOfpsis_rst_n();
    void CancelEventsOfpsis_mult_rst_n();
    void EnableReset(const std::string reset_name, const bool is_active);
    void EnablePRESETn(const bool is_active);
    void Enablepsis_rst_n(const bool is_active);
    void Enablepsis_mult_rst_n(const bool is_active);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out, sc_dt::uint64 freq_in, std::string unit_in);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    std::string FindRegAreaFromName(const std::string reg_name);
    void CreateValidFPKTvsFPAYLDTable();
    void CreateStrobeStringMap();
    bool GenerateParity(unsigned int data, unsigned int length);
    void ReceiveInIdleState();
    void ReceiveInStartState();
    void ReceiveInStopState();
    void RestorePSI5Frame(unsigned int data, bool parity_error, bool framing_error);
    unsigned int PrepareUARTFrame(unsigned int data, unsigned int parity_option, unsigned int &numbit);
    void PrepareDDSRData(unsigned int chid, sc_dt::uint64 &ddsr, unsigned int &length);
    unsigned int PrepareTXCONTROL(eStrobe strobe, unsigned int numbit, unsigned int tc);
    void UpdateTXFIFOStatus();


    // Overwrite firtual functions of PSIS011_AgentController
    bool GetResetStatus(const std::string reset_name);
    void SoftwareReset();
    sc_dt::uint64 GetPCLKFreq();
    void ClearTimestamp(std::string timestamp_name);
    void ConfigTimestamp(std::string timestamp_name, bool clear_select_gtm, bool enable_select_gtm, bool clock_select_gtm);
    void SetTSInnerEnable(std::string timestamp_name, bool ts_inner_enable);
    void SetTSInnerCycle(unsigned int inner_cycle);
    void InformChangeMode(unsigned int open, unsigned int opmd);
    void NotifySendReqTxToArbiter(unsigned int reqid);
    void NotifyTransUART();
    void AssertInterrupt(unsigned int intrid);
    void AssertDMARequestRX(unsigned int dmarxid);
    void AssertDMARequestTX(unsigned int dmatxid);
    void ChangeWDTEnableChannel(unsigned int chid, unsigned int enable);
    void InformChangeSCKENBit(unsigned int value);
    void StopTransDDSR(unsigned int reqid);

    void ConfigInnerSyncPulse(unsigned int chid, unsigned int select, unsigned int prescaler, unsigned int expired);
    unsigned int GetTimestamp(unsigned int chid);
    void GenerateMBData(unsigned int &allocated_chid, unsigned int &allocated_frmid, unsigned int &status_reg_val, 
                    unsigned int &data_reg_val, unsigned int &timestamp_reg_val);
    void GenerateMBDataForWDTError(unsigned int &allocated_chid, unsigned int &allocated_frmid,
            unsigned int &status_reg_val, unsigned int &data_reg_val, unsigned int &timestamp_reg_val);
    bool CheckValidFPKTvsFPAYLD(unsigned int fpayld, unsigned int rfcps, unsigned int fpkt);
    unsigned int GenerateCRC(unsigned int data, unsigned int length);
    unsigned int GenerateCRCExtra(sc_dt::uint64 data, unsigned int length);
    sc_dt::uint64 Reverse(sc_dt::uint64 input, unsigned int length);

public:
    SC_HAS_PROCESS (PSIS011);
    PSIS011 (sc_module_name name, const unsigned int rLatency, const unsigned int wLatency);
    ~PSIS011 ();

    double CalculateCLKPosEdge (const std::string clock_name, const bool add_period, const double time_point);
    bool CheckClockPeriod(const std::string clock_name);

    // PythonIF
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void SetDNFDelay (const unsigned int delay);
    void DumpStatusInfo ();
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int chid, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name, const unsigned int chid);
    void WriteRegister (const std::string reg_name, const unsigned int chid, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name, const unsigned int chid);
    void ListRegister (void);
    void Help (const std::string type);
};
#endif //__PSIS011_H__
