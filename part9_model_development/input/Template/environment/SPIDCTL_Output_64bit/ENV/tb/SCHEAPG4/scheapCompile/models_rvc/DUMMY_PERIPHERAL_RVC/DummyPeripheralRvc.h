//----------------------------------------------------------------------
//* File: DummyPeripheralRvc.h
//* Description: DummyPeripheralRvc model is used for signal issuing/receiving (control ports)
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

#define BMSPIDOUT_PORT_AMOUNT    64

class DummyPeripheralRvc: public sc_module,
                          public DummyPeripheralRvc_Base,
                          public BusSlaveBase<32,1>
{
public:
    unsigned int  mCurrentLength;
    //Port declaration
    sc_in<uint64>                   freqPortApb;
    sc_in<bool>                     resetPort;

    //output port of dummy - Input port of DUT model
    sc_out<sc_dt::uint64>           PCLK;
    sc_out<bool>                    Reset;

    //Output port of SPIDCTL model
    sc_in<unsigned char>                    BMSPIDOUT[BMSPIDOUT_PORT_AMOUNT];

    //Socket declaration
    TlmTargetSocket<32>             *ts;
    DummyPeripheralRvc_Func         *mDummyPeripheralRvc_Func;
    SC_HAS_PROCESS(DummyPeripheralRvc);
    DummyPeripheralRvc(sc_module_name name, unsigned int rLatency, unsigned int wLatency);
    ~DummyPeripheralRvc(void);

    // Python IF (API)
    void SetOutputPort(std::string name, unsigned long long value);
    void CheckInputPort (std::string name, unsigned int Index, unsigned long long exp_value);
    void EnableDumpMessage(bool enable);
    // Public functions
    void Judgement(unsigned int value);
private:
    // Enum
    enum eClockInfo{
        emClkId                = 0
       ,emNumClk               = 1
    };
   enum eOutputPort{
                 emPCLK
                ,emReset
    };            

    enum eInputPort{
                emSPIDOUT = 0
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

    std::map<std::string, unsigned int> mInputPortMap; // input port name, input port enum index
    std::map<std::string, unsigned int> mOutputPortMap; // output port name, output port enum index
    std::map<std::string, unsigned long long> mIntCountMap;   // interrupt port name, number of asserting interrupt

    // Events
    //sc_event mFallingInterruptEvent[emInterruptStart]; // Event notify falling interrupt
    // Method/Thread

    // Functions
    void CreateMap();
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out, sc_dt::uint64 freq_in, std::string unit_in);
    // Handle output signals - virtual functions in DummyPeripheralRvc_Base
    void SetOutputSignal(eSIGNAME signal_name, unsigned int value);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    // Handle input signals
    void HandleInputPortMethod(void);
}; 
#endif
