// ---------------------------------------------------------------------
// $Id: DFE0051.h,v 1.2 2017/07/21 08:48:22 chuonghoangle Exp $
//
// Copyright(c) 2016 Renesas Electronics Corporation
// Copyright(c) 2016 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __DFE0051_H__
#define __DFE0051_H__

#include "DFE0051_AgentController.h"
#include "re_define.h"
#include <stdint.h>
class Cdfe0051_wrapper;
class Cdfe0051_func;

template <unsigned int NUM_CHANNEL>
class Cdfe0051: public sc_module
            ,public Cdfe0051_agent_controller
{
public:
    // Ports
    sc_in<sc_dt::uint64>    CLK_HSB;
    sc_in<sc_dt::uint64>    CLK_UHSB;
    sc_in<bool>             PRESETn;
    
    sc_in<bool>             SARADC0_REQ;
    sc_in<sc_uint<4> >      SARADC0_TAG;
    sc_in<sc_uint<16> >     SARADC0_DATA;

    sc_in<bool>             SARADC1_REQ;
    sc_in<sc_uint<4> >      SARADC1_TAG;
    sc_in<sc_uint<16> >     SARADC1_DATA;

    sc_in<bool>             SARADC2_REQ;
    sc_in<sc_uint<4> >      SARADC2_TAG;
    sc_in<sc_uint<16> >     SARADC2_DATA;

    sc_in<bool>             SARADC3_REQ;
    sc_in<sc_uint<4> >      SARADC3_TAG;
    sc_in<sc_uint<16> >     SARADC3_DATA;

    sc_in<bool>             DSADC00_REQ;
    sc_in<sc_uint<4> >      DSADC00_TAG;
    sc_in<sc_uint<16> >     DSADC00_DATA;

    sc_in<bool>             DSADC10_REQ;
    sc_in<sc_uint<4> >      DSADC10_TAG;
    sc_in<sc_uint<16> >     DSADC10_DATA;

    sc_in<bool>             DSADC20_REQ;
    sc_in<sc_uint<4> >      DSADC20_TAG;
    sc_in<sc_uint<16> >     DSADC20_DATA;

    sc_in<bool>             DSADC11_REQ;
    sc_in<sc_uint<4> >      DSADC11_TAG;
    sc_in<sc_uint<16> >     DSADC11_DATA;

    sc_in<bool>             DSADC12_REQ;
    sc_in<sc_uint<4> >      DSADC12_TAG;
    sc_in<sc_uint<16> >     DSADC12_DATA;

    sc_in<bool>             DSADC13_REQ;
    sc_in<sc_uint<4> >      DSADC13_TAG;
    sc_in<sc_uint<16> >     DSADC13_DATA;

    sc_in<bool>             DSADC14_REQ;
    sc_in<sc_uint<4> >      DSADC14_TAG;
    sc_in<sc_uint<16> >     DSADC14_DATA;

    sc_in<bool>             DSADC15_REQ;
    sc_in<sc_uint<4> >      DSADC15_TAG;
    sc_in<sc_uint<16> >     DSADC15_DATA;

    sc_in<bool>             DSADC21_REQ;
    sc_in<sc_uint<4> >      DSADC21_TAG;
    sc_in<sc_uint<16> >     DSADC21_DATA;

    sc_in<bool>             DSADC22_REQ;
    sc_in<sc_uint<4> >      DSADC22_TAG;
    sc_in<sc_uint<16> >     DSADC22_DATA;

    sc_in<bool>             CADC0_REQ;
    sc_in<sc_uint<4> >      CADC0_TAG;
    sc_in<sc_uint<16> >     CADC0_DATA;

    sc_in<bool>*            TMTRG_A[20];
    sc_in<bool>*            TMTRG_B[20];
    sc_in<bool>*            TMCAPTRG[3];

    sc_in<bool>*            DFE0051_FIFO_CACK[NUM_CHANNEL];

    sc_out<bool>*           DFE0051_PHUPD[4];
    sc_out<bool>*           INTDFE0051DOUT[NUM_CHANNEL];
    sc_out<bool>*           INTDFE0051CND0[NUM_CHANNEL];
    sc_out<bool>*           INTDFE0051CND1[4];
    sc_out<bool>            INTDFE0051ERR;
    sc_out<bool>*           INTDFE0051SUBOUT[3];
    sc_out<unsigned int>*   DFE0051_DFDOUT[NUM_CHANNEL];
    sc_out<bool>*           DFE0051_DOUPD_N[NUM_CHANNEL];
    sc_out<bool>*           DFE0051_DOUT_END[NUM_CHANNEL];

    Cdfe0051_func                   *mDFE0051Func;          // Pointer of DFE0051 Func
private:
    // Enums
    enum eGeneralInfo{
        emData16Mask        = 0x7FFF
        ,emSign16Mask       = 0x8000
        ,emNumBitInputNoSign= 15
        ,emExpMask          = 0x7F800000
        ,emSign32Mask       = 0x80000000
        ,emData32Mask       = 0x7FFFFFFF
        ,emFractionMask     = 0x007FFFFF
        ,emMaxPHIND         = 0x3FFF
    };
    enum eInputSource{
        emSoftware  // Highest priority
        ,emSARADC0
        ,emSARADC2
        ,emSARADC1
        ,emSARADC3
        ,emDSADC00
        ,emDSADC10
        ,emDSADC20
        ,emDSADC11
        ,emDSADC12
        ,emDSADC13
        ,emDSADC14
        ,emDSADC15
        ,emDSADC21
        ,emDSADC22
        ,emCADC0
        ,emDeciCas
        ,emSubCas0
        ,emSubCas1
        ,emSubCas2  // Lowest priority
        ,emNumInputSource
    };
    enum eFilterKind{ // Corresponding value in CTLACHn.CMD bit
        emFIR8Tap           = 0
        ,emFIR16Tap         = 1
        ,emFIR24Tap         = 2
        ,emFIR32Tap         = 3
        ,emFIR64Tap         = 7
        ,emIIR1Stage        = 8
        ,emIIR2Stage        = 9
        ,emIIR3Stage        = 10
        ,emIIRGain2Stage    = 13
        ,emIIRGain3Stage    = 14
    };

    // Struct
    struct ADPortElement {// Store info related to AD input ports
        eInputSource            source;     // Indicate source is SARADCn, DSADCm, or CADC
        sc_in<bool>*            request;    // Pointer to REQ port of AD input
        sc_in<sc_uint<4> >*     tag;        // Pointer to TAG port of AD input
        sc_in<sc_uint<16> >*    data;       // Pointer to DATA port of AD input
        bool                    pre_request;// Previous value of REQ port
        ADPortElement(eInputSource source, sc_in<bool>* request, sc_in<sc_uint<4> >* tag, sc_in<sc_uint<16> >* data, bool pre_request){
            this->source = source;
            this->request = request;
            this->tag = tag;
            this->data = data;
            this->pre_request = pre_request;
        }
    };
    struct ChannelInfo {// Store info related to a channel
        bool        active;
        sc_uint<16> in_data;
        ChannelInfo(bool active, sc_uint<16> in_data){
            this->active = active;
            this->in_data = in_data;
        }
        void StoreInfo(bool active, sc_uint<16> in_data){
            this->active    = active;   // Internal CHn enable flag
            this->in_data   = in_data;  // Internal CHn data
        }
    };
    struct InputSourceInfo{
        unsigned int  tag;
        unsigned int data;
        bool        active;
        void StoreInfo(unsigned int tag, unsigned int data, bool active){
            this->tag   = tag;
            this->data  = data;
            this->active= active;
        }
    };

    // Variables
    Cdfe0051_wrapper                *mWrapper;          // Wrapper pointer
    std::vector<ADPortElement>  mADPortTable;       // Table controls AD input ports -> convenient for scan to check
    std::vector<ChannelInfo>    mChannelInfoTable;  // Table controls CHn info (ex: active, data)
    InputSourceInfo             mInputSourceTable[emNumInputSource];    // Table controls input source info
    bool            mGetPHMaskFlag[NUM_CHANNEL];            // Newest status of PH mask flag (affect to next input)
    bool            mGetPHEnableFlag[NUM_CHANNEL];          // Newest status of PH Initialize/Disable flag (affect to next input)
    bool            mGetAccumDeciEnableFlag[NUM_CHANNEL];   // Newest status of Accumulation/Decimation Initialize/Disable flag (affect to next input)
    bool            mGetFilterEnableFlag[NUM_CHANNEL];      // Newest status of Filter Initialize flag (affect to next input)
    bool            mGetSubEnableFlag[3];                   // Newest status of Subtraction Start/End flag (affect to next input)
    bool            mStatusPHEnable[NUM_CHANNEL];       // Status of PH Initialize/Disable flag
    bool            mStatusPHMask[NUM_CHANNEL];         // Status of PH mask flag
    bool            mStatusAccumDeciEnable[NUM_CHANNEL];// Status of Accumulation/Decimation Initialize/Disable flag
    bool            mStatusFilterEnable[NUM_CHANNEL];   // Status of Filter Initialize flag
    bool            mStatusSubEnable[3];                // Status of Subtraction Start/End flag
    bool            mCaptureEnable[3];          // Capture enabled or not (CEN)
    bool            mChannelEnable[NUM_CHANNEL];// Channel enabled or not (EN)
    bool            mSubtractionEnable[3];      // Subtraction enabled or not (SEN)
    bool            mINTDFE0051CND1[4];             // Value will write in INTDFE0051CND1
    bool            mINTDFE0051CND0[NUM_CHANNEL];   // Value will write in INTDFE0051CND0
    unsigned int    mCount[NUM_CHANNEL];        // Counter in Accumulation/Decimation
    unsigned int    mExpectedCount[NUM_CHANNEL];// Expected count value in Accumulation/Decimation
    unsigned int    mOutChannelNum;             // Channel number output value
    int32_t         mOutValue;                  // Output value
    int32_t         mAccumVal[NUM_CHANNEL];     // Accumulated value
    int32_t         mFilterOutData;             // Output from filter
    int32_t         mSubtrahend[3];             // Value of subtrahend
    int32_t         mMinuend[3];                // Value of minuend
    int32_t         mSubValue[3];               // Subtraction value
    bool            mINTDFE0051SUBOUT[3];           // Value will write to INTDFE0051SUBOUT
    bool            mIsSubClear[3];             // Is Subtraction clear or not
    bool            mIsPHInit[NUM_CHANNEL];     // Is PH initialized
    bool            mIsFilterInit[NUM_CHANNEL]; // Is Filter initialized
    bool            mDFE0051PHUPD[4];               // Value will write to DFE0051PHUPD
    unsigned int    mPHCounter[NUM_CHANNEL];    // Counter of Peak-hold process
    bool            mIsStartPHPhase[NUM_CHANNEL];   // Is Start of 1 phase of PH
    bool            mIsInitAccumDeci[NUM_CHANNEL];  // Is Accumulation/Decimation initialized
    bool            mIsDisablePHTrg[NUM_CHANNEL];   // Is PH is disabled
    bool            mIsFirstInput;                  // Is first input to filter
    bool            mIsOutFromAccum[NUM_CHANNEL];   // Is output out from Accumulation/Decimation
    bool            mDOUTN[NUM_CHANNEL];            // Value will write to DFE0051_DOUPD_N
    bool            mINTDFE0051DOUT[NUM_CHANNEL];       // Store value of INTDFE0051DOUT
    bool            mINTDFE0051ERR;                     // Store value of INTDFE0051ERR
    unsigned int    mDFE0051_DFDOUT[NUM_CHANNEL];       // Store value of DFE0051_DFDOUT
    bool            mDFE0051_DOUPD_N[NUM_CHANNEL];      // Store value of DFE0051_DOUPD_N
    bool            mDFE0051_DOUT_END[NUM_CHANNEL];     // Store value of DFE0051_DOUT_END
   
    bool            mIsPRESETInit;      // Is PRESET initialized
    sc_dt::uint64   mHSBPeriod;         // Period of HSB
    sc_dt::uint64   mHSBOrgFreq;        // Previous frequency of HSB
    sc_dt::uint64   mHSBFreq;           // Frequency of HSB
    std::string     mHSBFreqUnit;       // Frequency unit of HSB
    sc_dt::uint64   mUHSBPeriod;        // Period of UHSB
    sc_dt::uint64   mUHSBOrgFreq;       // Previous frequency of UHSB
    sc_dt::uint64   mUHSBFreq;          // Frequency of UHSB
    std::string     mUHSBFreqUnit;      // Frequency unit of UHSB
    bool            mIsResetHardActive; // Is reset by port
    bool            mIsResetCmdReceive; // Is receive cmd reset
    bool            mIsResetCmdActive;  // Is reset by cmd
    double          mResetCmdPeriod;    // Period of cmd reset
    sc_dt::uint64   mTimeResolutionValue;   // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;    // Time resolution unit of simulation
    std::map<eFilterKind, unsigned int> mNormalFilterProcCycle; // Processing cycle of Filter in Normal case
    std::map<eFilterKind, unsigned int> mInitFilterProcCycle;   // Processing cycle of Filter with Initialize
    std::map<eInputSource, std::string> mInputSourceNameMap;    // Map enum with name of input source

    // Events
    sc_event mDispatchEvent;
    sc_event mActivateErrorInterruptEvent;
    sc_event mNegateErrorInterruptEvent;
    sc_event mOutputValueEvent;
    sc_event mRaiseINTDFE0051DOUTEvent;
    sc_event mNegateINTDFE0051DOUTEvent;
    sc_event mRaiseINTDFE0051SUBOUTEvent;
    sc_event mNegateINTDFE0051SUBOUTEvent;
    sc_event mRaiseINTDFE0051CND1Event;
    sc_event mNegateINTDFE0051CND1Event;
    sc_event mRaiseINTDFE0051CND0Event;
    sc_event mNegateINTDFE0051CND0Event;
    sc_event mProcessEvent;
    sc_event mUpdateTriggerStatusEvent;
    sc_event mIssueOutputDataEvent;
    sc_event mNegateDOUTENDEvent;
    sc_event mClearDFDOUTEvent;
    sc_event mRaiseDFE0051PHUPDEvent;
    sc_event mNegateDFE0051PHUPDEvent;
    sc_event mDeassertDOUTNEvent;
    sc_event mWriteDFE0051PHUPDEvent;
    sc_event mWriteINTDFE0051DOUTEvent;
    sc_event mWriteINTDFE0051CND0Event;
    sc_event mWriteINTDFE0051CND1Event;
    sc_event mWriteINTDFE0051ERREvent;
    sc_event mWriteINTDFE0051SUBOUTEvent;
    sc_event mWriteDFE0051_DFDOUTEvent;
    sc_event mWriteDFE0051_DOUPD_NEvent;
    sc_event mWriteDFE0051_DOUT_ENDEvent;

    sc_event mResetCmdEvent;
    sc_event mResetCmdCancelEvent;
    sc_event mResetEvent;


    // Threads/Methods
    void DFE0051ProcessThread();
    void ActivateErrorInterruptMethod();
    void NegateErrorInterruptMethod();
    void MonitorTimerTriggerMethod();
    void UpdateTriggerStatusMethod();
    void ADRequestMethod();
    void DispatchMethod();
    void OutputValueMethod();
    void IssueOutputDataMethod();
    void NegateDOUTENDMethod();
    void RaiseINTDFE0051DOUTMethod();
    void NegateINTDFE0051DOUTMethod();
    void RaiseINTDFE0051SUBOUTMethod();
    void NegateINTDFE0051SUBOUTMethod();
    void RaiseINTDFE0051CND1Method();
    void NegateINTDFE0051CND1Method();
    void RaiseINTDFE0051CND0Method();
    void NegateINTDFE0051CND0Method();
    void CaptureMethod();
    void MonitorFIFOACKMethod();
    void ClearDFDOUTMethod();
    void RaiseDFE0051PHUPDMethod();
    void NegateDFE0051PHUPDMethod();
    void DeassertDOUTNMethod();
    void WriteDFE0051PHUPDMethod();
    void WriteINTDFE0051DOUTMethod();
    void WriteINTDFE0051CND0Method();
    void WriteINTDFE0051CND1Method();
    void WriteINTDFE0051ERRMethod();
    void WriteINTDFE0051SUBOUTMethod();
    void WriteDFDOUTMethod();
    void WriteDFE0051_DOUPD_NMethod();
    void WriteDFE0051_DOUT_ENDMethod();

    void CLKHSBMethod();
    void CLKUHSBMethod();
    void PRESETnMethod();
    void HandleResetCmdMethod();
    void CancelResetCmdMethod();

    // Functions
    void Initialize(void);
    void EnableReset(const bool is_active);
    void GetTimeResolution(sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    void ConstructADInputPortTable();
    bool Filter(unsigned int channel_id);
    bool CheckAssignChannelValid(eInputSource source, unsigned int dest_channel);
    unsigned int GetFilterProcCycle(unsigned int filter_kind, bool with_initialize);
    int32_t FilterFIR(unsigned int channel_id, unsigned short xn, unsigned int num_taps);
    int32_t FilterIIR(unsigned int channel_id, unsigned short xn, unsigned int num_stage, bool is_gain);
    int64_t CalBasicIIR(int32_t& input_acc, int32_t x, int32_t w1, int32_t w2,
    int16_t b00, int16_t b10, int16_t a11, int16_t b11, int16_t a12, int16_t b12, bool is_gain, unsigned int channel);
    int32_t SaturateTo32bit(int64_t input);
    void StoreIntermediateValue(unsigned int channel_id, int64_t y);
    unsigned int Convert2Float(unsigned long long data);
    bool CompareValues(int32_t compare_value, int32_t output_value, unsigned int operator_value);
    int32_t SumCompare(unsigned int channel, int32_t phregval, int32_t alpha);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out, sc_dt::uint64 freq_in, std::string unit_in);
    bool GetResetStatus();
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    bool ZipInt32To16(int32_t input, int16_t& output);
    bool ZipFix32To16(int32_t input, int16_t& output);
    int64_t MulQ114Q31RtnQ31(int16_t b, int32_t x, unsigned int channel);
    int32_t MulQ15Q15(unsigned int channel, unsigned short a, unsigned short b);
    int32_t MulInt(unsigned int channel, unsigned short a, unsigned short b);
    bool AccumDeciProcess(unsigned int channel);
    bool Accumulate(unsigned int channel, int32_t pro_data);
    int32_t Accum3232(unsigned int channel, int32_t accum, int32_t addition);
    unsigned int Absolute(unsigned int channel_id, unsigned int input);
    bool Decimate(unsigned int channel, int32_t pro_data);
    void DeciCascadeProcess(unsigned int channel);
    bool SubtractProcess(unsigned int channel);
    void SubCascadeProcess(unsigned int channel);
    int32_t Sub3232(unsigned int sub_id, int32_t minuend, int32_t subtrahend);
    bool PeakProcess(unsigned int channel);
    bool CompareProcess(unsigned int channel);
    void UpdateEnableStatus();

    // Overwrite firtual functions of Cdfe0051_agent_controller
    void StartSWInput();
    void IssueSWTrigger();
    void UpdateChannelEnable(unsigned int channel, bool enable);
    void UpdateCaptureEnable(unsigned int capid, bool enable);
    void AutoInitialize(unsigned int channel);
    void RaiseINTDFE0051ERR();
    void DisableSubtractionFlag(unsigned int subid);
public:
    SC_HAS_PROCESS(Cdfe0051);
    Cdfe0051(sc_module_name name, Cdfe0051_wrapper *parent);
    ~Cdfe0051();

    // PythonIF
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);
    void WriteCoefficient (const unsigned int offset, const unsigned int value);
    void ReadCoefficient (const unsigned int offset);
    void WriteDataMem (const unsigned int offset, const unsigned int value);
    void ReadDataMem (const unsigned int offset);
};
#endif //__DFE0051_H__
