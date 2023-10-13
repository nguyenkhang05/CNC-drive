//----------------------------------------------------------------------
//* File: DummyPeripheralRvc.cpp
//* Description: DummyPeripheral model is used for signal issuing/receiving (control ports)
//* Modified date: Dec 7, 2016 - ThangNguyen
//----------------------------------------------------------------------
#include <string>
#include "DummyPeripheralRvc.h"

/// Constructor
DummyPeripheralRvc::DummyPeripheralRvc(
          sc_module_name name
        , unsigned int       rLatency ///< [in] rLatency
        , unsigned int       wLatency ///< [in] wLatency
	):
                  // initialize the parent classes
                  sc_module(name),
                  DummyPeripheralRvc_Base(),
                  BusSlaveBase<128,1>(),
                  // interface to the environment
                  freqPortApb("freqPortApb"), 
                  resetPort("resetPort"),
                  // interface to the DUT: clock and reset
                  PCLK("PCLK"),
                  PRESETn("PRESETn")

{//{{{
    CreateMap();
    mEnableDumpMessage = true;

    //Initial value of the output ports
    PCLK.initialize(0x0);
    PRESETn.initialize(true);
	//app1_rst_n.initialize(true);   // Tien Tran
   	//SVSTOP.initialize(false); 	// Tien Tran
    //------------------------------------------
    // Socket
    setSlaveResetPort128(&resetPort);
    setSlaveFreqPort128(&freqPortApb);
    setTargetSocket128("ts");
    mMemory = new MEMORYRVC();
    ts = tSocket128[0]; //Alias for binding
    
    mDummyPeripheralRvc_Func = new DummyPeripheralRvc_Func((std::string)name, this, this);
    mBusSlaveIf128[0]->setFuncModulePtr((BusSlaveFuncIf*)mMemory);
    mBusSlaveIf128[0]->setReadInitLatency(rLatency);
    mBusSlaveIf128[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf128[0]->setBusProtocol(BUS_AXI);
    //mBusSlaveIf32[0]->setTransNmbLmt(0xFFFFFFFF);
    mBusSlaveIf128[0]->setTransNmbLmt(0x1);
    //------------------------------------------

    // Initialize internal variable
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
}//}}}

/// Create map
void DummyPeripheralRvc::CreateMap()
{//{{{
    // mInputPortMap
    mOutputPortMap.insert(std::pair<std::string, unsigned int>("PCLK",            emPCLK      ));
    mOutputPortMap.insert(std::pair<std::string, unsigned int>("PRESETn",         emPRESETn   ));
}//}}}

/// Set value to output port (this is an API called from Python IF)
/// std::string name: name of port (get enum according from mOutputPortMap)
/// unsigned long long value: value to set (64bit)
void DummyPeripheralRvc::SetOutputPort(std::string name, unsigned long long value)
{//{{{
    unsigned int port_id = 0;
    sc_dt::uint64 cur_val_ui64 = 0;
    bool cur_val_b = false;

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
        case emPCLK:
            cur_val_ui64 = PCLK.read();
            if (cur_val_ui64 != ((sc_dt::uint64)value)){
                cur_val_ui64 = ((sc_dt::uint64)value);
                if (mEnableDumpMessage){
                    printf("Info (%s) [%s] Issue PCLK frequency: %llu (Hz)\n", this->name(), sc_time_stamp().to_string().c_str(), value);
                }
                PCLK.write (cur_val_ui64);
            }
            break;
           
		// changed by Tien Tran    
        /*case emapp1_rst_n:
            cur_val_b = app1_rst_n.read();
            if (cur_val_b!= (bool)(value & 0x1)){
                cur_val_b = (bool)(value & 0x1);
                if (mEnableDumpMessage){
                    printf("Info (%s) [%s] Issue app1_rst_n signal: %d \n", this->name(), sc_time_stamp().to_string().c_str(), cur_val_b);
                }
                app1_rst_n.write (cur_val_b);
            }
            break;*/

        case emPRESETn:
            cur_val_b = PRESETn.read();
            if (cur_val_b!= (bool)(value & 0x1)){
                cur_val_b = (bool)(value & 0x1);
                if (mEnableDumpMessage){
                    printf("Info (%s) [%s] Issue PRESETn signal: %d\n", this->name(), sc_time_stamp().to_string().c_str(), cur_val_b);
                }
                PRESETn.write(cur_val_b);
            }
            break;

        //Changed by Tien Tran
		/*case emSVSTOP:
            cur_val_b = SVSTOP.read();
            if (cur_val_b!= (bool)(value & 0x1)){
                cur_val_b = (bool)(value & 0x1);
                if (mEnableDumpMessage){
                    printf("Info (%s) [%s] Issue SVSTOP signal: %d\n", this->name(), sc_time_stamp().to_string().c_str(), cur_val_b);
                }
                SVSTOP.write (cur_val_b);
            }
            break; */ 
  
        default:
            printf("Warning (%s) [%s] Port name is not supported\n", this->name(), sc_time_stamp().to_string().c_str());
            break;
    }
}//}}}

/// Get value of input port (this is an API called from Python IF)
/// std::string name: name of port (get enum according from mInputPortMap)
/// unsigned long long exp_value: expected value
void DummyPeripheralRvc::CheckInputPort(std::string name, unsigned long long exp_value)
{//{{{
    unsigned int port_id = 0;
    bool cur_val_b = false;

    // Find port
    std::map<std::string, unsigned int>::iterator it;
    it = mInputPortMap.find(name);
    if (it != mInputPortMap.end()){
        port_id = mInputPortMap[name];
    }else{
        printf("Error (%s) [%s] Port %s is not found.\n", this->name(), sc_time_stamp().to_string().c_str(), name.c_str());
        return;
    }

    // Compare port value
    switch (port_id){
            
        default:
            printf("Warning (%s) [%s] Port name is not supported\n", this->name(), sc_time_stamp().to_string().c_str());
            break;
    }
}//}}}

/// Count times of interrupt assert (this is an API called from Python IF)
/// std::string name: name of interrupt port (will check valid or not in mIntCountMap)
/// unsigned int exp_count: expected times of asserting interrupt.
void DummyPeripheralRvc::CountInterruptTimes(std::string name, unsigned long long exp_count)
{//{{{
    // Find port
    std::map<std::string, unsigned long long>::iterator it;
    it = mIntCountMap.find(name);
    if (it != mIntCountMap.end()){
        // Compare
        if (mIntCountMap[name] != exp_count){
            printf("[%s] [%s] FAIL: Count %s time = %lld, expected = %lld\n", this->name(), sc_time_stamp().to_string().c_str(), name.c_str(), mIntCountMap[name], exp_count);
            Judgement(0);
        }
    }else{
        printf("Error (%s) [%s] Port %s is not found.\n", this->name(), sc_time_stamp().to_string().c_str(), name.c_str());
        return;
    }
}//}}}

/// Count times of DMA assert (this is an API called from Python IF)
/// std::string name: name of DMA port (will check valid or not in mIntCountMap)
/// unsigned int exp_count: expected times of asserting DMA.
void DummyPeripheralRvc::CountDMATimes(std::string name, unsigned long long exp_count)
{//{{{
    // Find port
    std::map<std::string, unsigned long long>::iterator it;
    it = mDMACountMap.find(name);
    if (it != mDMACountMap.end()){
        // Compare
        if (mDMACountMap[name] != exp_count){
            printf("[%s] [%s] FAIL: Count %s time = %lld, expected = %lld\n", this->name(), sc_time_stamp().to_string().c_str(), name.c_str(), mDMACountMap[name], exp_count);
            Judgement(0);
        }
    }else{
        printf("Error (%s) [%s] Port %s is not found.\n", this->name(), sc_time_stamp().to_string().c_str(), name.c_str());
        return;
    }
}//}}}

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

/// Get time resolution
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

/// Set dump info message
void DummyPeripheralRvc::EnableDumpMessage(bool enable)
{//{{{
    mEnableDumpMessage = enable;
}
void DummyPeripheralRvc::CreateMemory(unsigned int addr, unsigned int write_size)
{
    mMemory->CreateMem(addr, write_size);
}
void DummyPeripheralRvc::SetMemoryData(unsigned int addr, unsigned int data)
{
    mMemory->SetData(addr, data);
}
void DummyPeripheralRvc::GetMemoryData(unsigned int addr, unsigned int ex_val)
{
    mMemory->GetData(addr, ex_val);
}
//}}}

/// Set clock value and clock unit
void DummyPeripheralRvc::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mClkOrgFreq[emPCLKId] = freq;
            ConvertClockFreq (mClkFreq[emPCLKId], mClkFreqUnit[emPCLKId], freq, unit);
            if (mClkFreq[emPCLKId] > 0) {
                mClkPeriod[emPCLKId] = (sc_dt::uint64)(((1/(double)mClkFreq[emPCLKId])*(double)mTimeResolutionValue) + 0.5);
                mStartClkTime[emPCLKId] = sc_time_stamp().to_double();
                sc_time temp = sc_time_stamp();
            } else {
                mClkPeriod[emPCLKId] = 0;
                // Cancel events//XXX carefully, just cancel event related, refer INT for detail
                //CancelEvents();
                // Notify clock is zero
                //mClkZeroEvent[emPCLKId].notify(SC_ZERO_TIME);
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

///Calculate synchronous time
double DummyPeripheralRvc::CalculateClockEdge (const std::string clock_name, const bool is_pos, const double time_point, const double time_change_point)
{//{{{
    double clock_period;
    if (clock_name == "PCLK") {
        clock_period = (double)mClkPeriod[emPCLKId];
    }
    double period_num = 0;
    if (clock_period != 0) {
        period_num = (time_point - time_change_point) / clock_period;
    } else {
        return 0;
    }   
    sc_dt::uint64 mod_period_num = (sc_dt::uint64)period_num;
    double edge_point = 0, mod_period_num_db = 0;
    if (is_pos) {
        if ((double)mod_period_num < period_num) {
            mod_period_num++;
        }
        mod_period_num_db = (double)mod_period_num;
    }
    edge_point = mod_period_num_db * clock_period - (time_point - time_change_point);

    return edge_point;
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

void DummyPeripheralRvc::SetOutputSignal( eSIGNAME signal_name
                                        , unsigned int value  
                                        )
{//{{{
    bool cur_val_b = false;
    double cur_val_db = 0;
    double cur_reg_db = 0;
    sc_dt::uint64 cur_val_ui64 = 0;
    switch (signal_name){
        case emPCLK_H:
            mClkFreq[emPCLKId] = (mClkFreq[emPCLKId] & 0xFFFFFFFF) | (((sc_dt::uint64)value&0xFFFFFFFF)<<32);
            break;
        case emPCLK_L:
            cur_val_ui64 = PCLK->read();
            mClkFreq[emPCLKId] = (mClkFreq[emPCLKId]&0xFFFFFFFF00000000ULL) | ((sc_dt::uint64)value&0xFFFFFFFF);
            if (cur_val_ui64!= mClkFreq[emPCLKId]){
                printf("Info (%s) [%s] Issue PCLK frequency: %llu (Hz)\n", this->name(), sc_time_stamp().to_string().c_str(), mClkFreq[emPCLKId]);
                PCLK->write(mClkFreq[emPCLKId]);
                if (mClkFreq[emPCLKId] > 0) {
                    mClkPeriod[emPCLKId] = (sc_dt::uint64)(((1/(double)mClkFreq[emPCLKId])*(double)mTimeResolutionValue) + 0.5);
                } else {
                    mClkPeriod[emPCLKId] = 0;
                }
#ifdef DEBUG_SIM_MESSAGE_RVC
                printf("DEBUG (%s) [%s] PCLK period: %d (%s)\n", this->name(), sc_time_stamp().to_string().c_str(), mClkPeriod[emPCLKId], sTimeResolutionUnit.c_str());
#endif
            }
            break;
			
			//Changed by Tien Tran
         case emRESETn:
             cur_val_b = PRESETn->read();
             if (cur_val_b!= (value & 0x1)){
                 cur_val_b = value & 0x1;
                 printf("Info (%s) [%s] Issue PRESETn signal: %d\n", this->name(), sc_time_stamp().to_string().c_str(), cur_val_b);
                 PRESETn->write(cur_val_b);
             }
             break;
		/*case emapp1_rst_n:
            cur_val_b = app1_rst_n->read();
            if (cur_val_b!= (value & 0x1)){
                cur_val_b = value & 0x1;
                printf("Info (%s) [%s] Issue app1_rst_n signal: %d\n", this->name(), sc_time_stamp().to_string().c_str(), cur_val_b);
                app1_rst_n->write(cur_val_b);
            }
            break;*/
        default:
            printf("Warning (%s) [%s] Signal name is wrong\n", this->name(), sc_time_stamp().to_string().c_str());
            break;
    }
}//}}}

// vim600: set foldmethod=marker :
