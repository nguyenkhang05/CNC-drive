//----------------------------------------------------------------------
//* File: DummyPeripheralRvc.h
//* Description: DummyPeripheralRvc model is used for signal issuing/receiving (control ports)
//* Modified date: Dec 7, 2016 - ThangNguyen
//----------------------------------------------------------------------
#ifndef __DUMMYPERIPHERALRVC_H__
#define __DUMMYPERIPHERALRVC_H__

#include "DummyPeripheralRvc_Base.h"
#include "DummyPeripheralRvc_Func.h"
#include "BusSlaveBase.h"
#include "systemc.h"
#include <iterator>
#include <string>
#include <map>
#include "C:\Users\khang.duong-nguyen\Documents\Newcomer\Input\mcu_training\part9_model_development\source\MCUDevEnv\msvc\HAGE\MemoryRVC.h"
class DummyPeripheralRvc: public sc_module,
                          public DummyPeripheralRvc_Base,
                          public BusSlaveBase<128,1>
{
public:
    unsigned int  mCurrentLength;
    //Port declaration
    sc_in<uint64>                   freqPortApb;
    sc_in<bool>                     resetPort;

    //output port of dummy - Input port of DUT model 
    sc_out<sc_dt::uint64>           PCLK;
    sc_out<bool>                    PRESETn;

    //input port of dummy - Output port of HAGE model
    sc_in<bool>                     EINT;
    MEMORYRVC*                      mMemory;
    //Socket declaration
    TlmTargetSocket<128>			*ts;
    DummyPeripheralRvc_Func 		*mDummyPeripheralRvc_Func;
    
    SC_HAS_PROCESS(DummyPeripheralRvc);

    DummyPeripheralRvc(sc_module_name name, unsigned int rLatency, unsigned int wLatency);
    ~DummyPeripheralRvc(void);

    // Python IF (API)
    void SetOutputPort(std::string name, unsigned long long value);
    void CheckInputPort (std::string name, unsigned long long exp_value);
    void CountInterruptTimes (std::string name, unsigned long long exp_count);
    void CountDMATimes (std::string name, unsigned long long exp_count);
    void EnableDumpMessage(bool enable);
    void CreateMemory(unsigned int addr, unsigned int write_size);
    void SetMemoryData(unsigned int addr, unsigned int data);
    void GetMemoryData(unsigned int addr, unsigned int ex_val);
    
    // Public functions
    void Judgement(unsigned int value);
private:
    // Enum
    enum eClockInfo{
        emPCLKId                = 0
        ,empsis_clkId           = 1
        ,empsis_mult_clkId      = 2
        ,emNumClk               = 3
    };
    enum eInputPort{
        emPCLK
        ,emPRESETn
        //,emapp1_rst_n  // Tien Tran
        //,emSVSTOP	// Tien Tran
        ,emNumInputPort
    };

    // Variables
    sc_dt::uint64   mClkPeriod[emNumClk];       // Period of clock
    sc_dt::uint64   mClkOrgFreq[emNumClk];      // Previous frequency of clock
    sc_dt::uint64   mClkFreq[emNumClk];         // Frequency of clock
    std::string     mClkFreqUnit[emNumClk];     // Frequency unit of clock
    double          mStartClkTime[emNumClk];    // Start time of clock

    sc_dt::uint64   mTimeResolutionValue;   //Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;    //Time resolution unit of simulation
    double          mTimeCLKChangePoint;        //The time clock is changed its value

    bool            mEnableDumpMessage;
    std::map<std::string, unsigned int> mInputPortMap;  // input port name, input port enum index
    std::map<std::string, unsigned int> mOutputPortMap; // output port name, output port enum index
    std::map<std::string, unsigned long long> mIntCountMap;   // interrupt port name, number of asserting interrupt
    std::map<std::string, unsigned long long> mDMACountMap;   // DMA port name, number of asserting interrupt

    bool mPreint_psis_chn[8]; // interrupt (0-7)
    bool mPredma_psis_chn_rx[8]; // DMA rx (0-7)
    bool mPredma_psis_chn_tx[8]; // DMA tx (1-7)

    //sc_dt::uint64                   mPCLKfreq;                  //unit: "Hz", Used for 64bit port
    //unsigned int                    mPCLKperiod;                //unit: "sTimeResolutionUnit"
    unsigned int  mTxCtrl; // value of TX_CONTROL port
    unsigned int  mSupportPar; // 1: support parity; 0: not support
    sc_dt::uint64 mOneBitInterval;
    unsigned int  mStopBit; // store stop bit from Python TM
    unsigned int  mFrmnum;
    unsigned int  mPfrmidle;
    unsigned long long mData;
    unsigned int  mTxParity;
    unsigned char mCrcParErr;     // 0: Normal; 1: CRC/Parity error
    unsigned char mCrcParType;    // 0: Even parity; 1: CRC
    unsigned char mXcrcErr;       // 0: XCRC normal; 1: XCRC error
    unsigned int  mPacketTimes;

    // Events

    // Method/Thread

    // Functions
    void CreateMap();
    //void Judgement(unsigned int value);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out, sc_dt::uint64 freq_in, std::string unit_in);
    double CalculateClockEdge (const std::string clock_name, const bool is_pos, const double time_point,const double time_change_point);
    void PublicData(unsigned int &txctrl, unsigned int &supportpar, unsigned int stopbit, unsigned int dat8b, unsigned parity);
    
    // Handle output signals - virtual functions in DummyPeripheralRvc_Base
    void SetOutputSignal(eSIGNAME signal_name, unsigned int value);
    // Handle input signals
    //void HandleInputSignal(void);
    
}; 
#endif
