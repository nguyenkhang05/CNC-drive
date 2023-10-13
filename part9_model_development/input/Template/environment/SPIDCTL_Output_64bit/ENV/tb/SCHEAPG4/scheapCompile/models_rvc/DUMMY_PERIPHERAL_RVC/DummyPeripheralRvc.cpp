//----------------------------------------------------------------------
//* File: DummyPeripheralRvc.cpp
//* Description: DummyPeripheral model is used for signal issuing/receiving (control ports)
//* Modified date: Apr 7, 2019 - HaTran
//----------------------------------------------------------------------
#include <string>
#include "DummyPeripheralRvc.h"

// Description: DummyPeripheral model is used for signal issuing/receiving (control ports)
// Ref: {SPIDCTL_UT_VRFDummyPeripheral_001, SPIDCTL_UT_VRFIntroduction_001}
DummyPeripheralRvc::DummyPeripheralRvc(
          sc_module_name name
        , unsigned int       rLatency ///< [in] rLatency
        , unsigned int       wLatency ///< [in] wLatency
    ):
                  // initialize the parent classes
                  sc_module(name),
                  DummyPeripheralRvc_Base(),
                  BusSlaveBase<32,1>(),  // Ti remove dummyslave
                  // interface to the environment
                  freqPortApb("freqPortApb"),
                  resetPort("resetPort"),
                  // interface to the DUT: clock and reset
                  PCLK("PCLK"),
                  Reset("Reset")
   
{//{{{
    CreateMap();
    mEnableDumpMessage = true;

   //Initial value of the output ports
    PCLK.initialize(0x0); 
    Reset.initialize(true);     
    
    //------------------------------------------
    // Socket
    setSlaveResetPort32(&resetPort);
    setSlaveFreqPort32(&freqPortApb);
    setTargetSocket32("ts");
    ts = tSocket32[0]; //Alias for binding
    
    mDummyPeripheralRvc_Func = new DummyPeripheralRvc_Func((std::string)name, this, this);
    mBusSlaveIf32[0]->setFuncModulePtr(mDummyPeripheralRvc_Func);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);
     
    //------------------------------------------
    //Initialize internal variable
    for (int i = 0; i < emNumClk; i++){
        mClkPeriod[i]       = 0;
        mClkOrgFreq[i]      = 0;
        mClkFreq[i]         = 0;
        mClkFreqUnit[i]     = "Hz";
        mStartClkTime[i]    = 0; 
    }
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);   
    mTimeCLKChangePoint = 0;
      
}//}}}

/// Destructor
DummyPeripheralRvc::~DummyPeripheralRvc(void)
{//{{{
    delete mDummyPeripheralRvc_Func;
}//}}}

/// Create map
void DummyPeripheralRvc::CreateMap()
{//{{{
    
    mOutputPortMap.insert(std::pair<std::string, unsigned int>( "PCLK"     ,emPCLK ));
    mOutputPortMap.insert(std::pair<std::string, unsigned int>("Reset"     ,emReset));

    mInputPortMap.insert(std::pair<std::string, unsigned int>( "BMSPIDOUT"  ,emSPIDOUT));
}//}}}

/// Set value to output port (this is an API called from Python IF)
/// std::string name: name of port (get enum according from mOutputPortMap)
/// unsigned long long value: value to set (64bit)
void DummyPeripheralRvc::SetOutputPort(std::string name, unsigned long long value)
{//{{{
    unsigned int port_id = 0;
    sc_dt::uint64 cur_val_ui64 = 0;
    bool cur_val_b = false;
    unsigned char cur_val_uc8 = 0x0;
    // Find port
    std::map<std::string, unsigned int>::iterator it;
    it = mOutputPortMap.find(name);
    if (it != mOutputPortMap.end()){
        port_id = mOutputPortMap[name];
    }else{
        printf("Error (%s) [%s] Port %s is not found.\n", this->name(), sc_time_stamp().to_string().c_str(), name.c_str());
        return;
    }

    // Write port
    switch (port_id){
//----------------------------------------------
     case emPCLK:
        cur_val_ui64 = PCLK.read();
        if (cur_val_ui64 != ((sc_dt::uint64)value)){
            cur_val_ui64 = ((sc_dt::uint64)value);
            if (mEnableDumpMessage){
            printf("Info (%s) [%s] Issue PCLK signal: %d\n", this->name(), sc_time_stamp().to_string().c_str(), value); 

            }
            PCLK.write(cur_val_ui64);
                }
         break; 
     case emReset:
        cur_val_b = Reset.read();
        if (cur_val_b != ((bool)value & 0x1)){
            cur_val_b = ((bool)value & 0x1);
            if (mEnableDumpMessage){
            printf("Info (%s) [%s] Issue Reset signal: %d \n", this->name(), sc_time_stamp().to_string().c_str(), value); 

            }
            Reset.write (cur_val_b);
                 }
         break; 

        default:
            printf("Warning (%s) [%s] Port name is not supported\n", this->name(), sc_time_stamp().to_string().c_str());
            break;
    }
   
}//}}}

/// Get value of input port (this is an API called from Python IF)
/// std::string name: name of port (get enum according from mInputPortMap)
/// unsigned long long exp_value: expected value
void DummyPeripheralRvc::CheckInputPort(std::string name, unsigned int Index, unsigned long long exp_value)
{//{{{
    unsigned int port_id = 0;
    unsigned char cur_val = false;
    bool bFound = false;
    std::stringstream strIndex;
    // Find port
    std::map<std::string, unsigned int>::iterator it;
    it = mInputPortMap.find(name);
    if (it != mInputPortMap.end()){
        port_id = mInputPortMap[name];
        bFound = true;
        
        if (port_id == emSPIDOUT)
        {
            strIndex << Index;
            name = name + strIndex.str();
            if (Index >= BMSPIDOUT_PORT_AMOUNT)
            {
                bFound = false;
            }
        }
    }
    
    if (bFound == false)
    {
        printf("Error (%s) [%s] Port %s is not found.\n", this->name(), sc_time_stamp().to_string().c_str(), name.c_str());
        return;

    }

    // Compare port value
    switch (port_id){

    case emSPIDOUT:
        cur_val = (unsigned char) BMSPIDOUT[Index].read();
        if (cur_val!= (unsigned char)(exp_value & 0x1F)){
            printf("(%s) [%s] FAIL: Input Port %s value = %d , expected = %lld \n", this->name(), sc_time_stamp().to_string().c_str(), name.c_str(), cur_val, exp_value);
            Judgement(0);
        }
        break;
 
     default:
        printf("Warning (%s) [%s] Port name is not supported\n", this->name(), sc_time_stamp().to_string().c_str());
        break;
    }
}//}}}

/// Count times of interrupt assert (this is an API called from Python IF)
/// std::string name: name of interrupt port (will check valid or not in mIntCountMap)
/// unsigned int exp_count: expected times of asserting interrupt.
void DummyPeripheralRvc::Judgement(unsigned int value)
{//{{{
    if (value == 0) {
        printf("--------------------------------\n");
        printf("-          TM is FAIL          -\n");
        printf("--------------------------------\n");
        exit(1);
    } else if (value == 1) {
        printf("--------------------------------\n");
        printf("-          TM is PASS          -\n");
        printf("--------------------------------\n");
    }
}//}}}

/// Set dump info message
void DummyPeripheralRvc::EnableDumpMessage(bool enable)
{//{{{
    mEnableDumpMessage = enable;
}//}}}

/// Set clock value and clock unit
void DummyPeripheralRvc::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mClkOrgFreq[emClkId] = freq;
            ConvertClockFreq (mClkFreq[emClkId], mClkFreqUnit[emClkId], freq, unit);
            if (mClkFreq[emClkId] > 0) {
                mClkPeriod[emClkId] = (sc_dt::uint64)(((1/(double)mClkFreq[emClkId])*(double)mTimeResolutionValue) + 0.5);
                mStartClkTime[emClkId] = sc_time_stamp().to_double();
                sc_time temp = sc_time_stamp();
            } else {
                mClkPeriod[emClkId] = 0;
            }
        } else {
            printf("DP warning: Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        printf("DP warning: Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Separate some words from a string to store a vector
void DummyPeripheralRvc::SeparateString (std::vector<std::string> &vtr, const std::string msg)
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

/// Check frequency value and frequency unit
void DummyPeripheralRvc::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out, sc_dt::uint64 freq_in, std::string unit_in)
{//{{{
    if(unit_in == "GHz"){
        freq_out = freq_in * (sc_dt::uint64)(1.0e+9);
        unit_out = "GHz";
    } else if (unit_in == "MHz") {
        freq_out = freq_in * (sc_dt::uint64)(1.0e+6);
        unit_out = "MHz";
    } else if (unit_in == "KHz") {
        freq_out = freq_in * (sc_dt::uint64)(1.0e+3);
        unit_out = "KHz";
    } else if (unit_in == "Hz") {
        freq_out = freq_in;
        unit_out = "Hz";
    } else {// Frequency unit is wrong, frequency unit is set as Hz default.
        freq_out = freq_in;
        unit_out = "Hz";
        printf("DP warning: Frequency unit (%s) is wrong, frequency unit is set as unit Hz default.\n", unit_in.c_str());
    }
}//}}}

/*********************************
// Function     : SetOutputSignal
// Description  : Issue output signals
// Parameter    :
//      signal_name     Signal name (based on model's register)
//                      (defined by enumerator eSIGNAME)
//      value           Signal's value
// Note:
// Return value : None
**********************************/

void DummyPeripheralRvc::SetOutputSignal(eSIGNAME signal_name, unsigned int value)
{//{{{
    bool cur_val_b = false;
    double cur_val_db = 0;
    double cur_reg_db = 0;
    sc_dt::uint64 cur_val_ui64 = 0;
    sc_dt::uint64 cur_val_ui64_lps = 0;
    unsigned char cur_val_uc8 = 0x0;
    switch (signal_name){

        case emCLK_LSB_H:
            mClkFreq[emClkId] = (mClkFreq[emClkId] & 0xFFFFFFFF) | (((sc_dt::uint64)value&0xFFFFFFFF)<<32);
            break;
        case emCLK_LSB_L:
            cur_val_ui64 = PCLK->read();
            mClkFreq[emClkId] = (mClkFreq[emClkId]&0xFFFFFFFF00000000ULL) | ((sc_dt::uint64)value&0xFFFFFFFF);
            if (cur_val_ui64!= mClkFreq[emClkId]){
                printf("Info (%s) [%s] Issue PCLK frequency: %llu (Hz)\n", this->name(), sc_time_stamp().to_string().c_str(), mClkFreq[emClkId]);
                PCLK->write(mClkFreq[emClkId]);
                if (mClkFreq[emClkId] > 0) {
                    mClkPeriod[emClkId] = (sc_dt::uint64)(((1/(double)mClkFreq[emClkId])*(double)mTimeResolutionValue) + 0.5);
                } else {
                    mClkPeriod[emClkId] = 0;
                }
#ifdef DEBUG_SIM_MESSAGE_RVC
                printf("DEBUG (%s) [%s] PCLK period: %d (%s)\n", this->name(), sc_time_stamp().to_string().c_str(), mClkPeriod[empclkId], sTimeResolutionUnit.c_str());
#endif
            }
            break;

        case emRESET:
            cur_val_b = Reset->read();
            if (cur_val_b!= (value & 0x1)){
                cur_val_b = value & 0x1;
                printf("Info (%s) [%s] Issue presetn signal: %d\n", this->name(), sc_time_stamp().to_string().c_str(), cur_val_b);
                Reset->write(cur_val_b);
            }
            break;

         default:
            printf("Warning (%s) [%s] Signal name is wrong\n", this->name(), sc_time_stamp().to_string().c_str());
            break;



    }
}//}}}


void DummyPeripheralRvc::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
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

//**********************************/
// Function    : HandleInputPortMethod
// Description : Receive signal from LPS module
// Parameter   : None
// Return value: None
//**********************************/
void DummyPeripheralRvc::HandleInputPortMethod(void){
}
// vim600: set foldmethod=marker :

