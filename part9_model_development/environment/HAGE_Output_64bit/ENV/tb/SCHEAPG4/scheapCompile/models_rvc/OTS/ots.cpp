// ---------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2012 Renesas Electronics Corporation
// Copyright(c) 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include <cerrno>
#include <string>
#include <cstdarg>
#include <fstream>
#include <sstream>
#include "re_define.h"
#include "ots.h"
#ifndef FRACTION_TO_INT
#define FRACTION_TO_INT(d) ((unsigned int)((d - (unsigned long long)(d))*1000000))
#endif

//Constructor of OTS class
Cots::Cots(sc_module_name name, const short coeffra, const short coeffrb, const short coeffrc):
         Cots_regif((std::string)name, 32, coeffra, coeffrb, coeffrc)
        ,vpcl::tlm_tgt_if<32>(name)
        ,pclk("pclk")
        ,clkot("clkot")
        ,preset_n("preset_n")
        ,oti("oti")
        ,otabe("otabe")
        ,otuli("otuli")
        ,ote("ote")
        ,temp_file("temperature_filename", "OTS_temperature.txt")
{//{{{
    //Inital handleCommand  
    CommandInit(this->name());
    Cots_regif::set_instance_name(this->name());
    //Initial port
    oti.initialize(0);
    otabe.initialize(0);
    otuli.initialize(0);
    ote.initialize(0);

    //Initial member variable
    mIsInitialize     = true;
    mFreqCLKOT        = 0;
    mFreqPCLK         = 0;
    mResetPeriod      = 0;
    mIsCmdResetFlag   = false;
    mOTSPortResetFlag = false;  
    mOTSCmdResetFlag  = false;
    mErrorFileFlag    = false;
    GlobalInit();

    //Reset Method
    SC_METHOD(ResetMethod);
    dont_initialize();
    sensitive << preset_n;

    SC_METHOD(AssertResetMethod);
    dont_initialize();
    sensitive << mCmdResetEvent;

    SC_METHOD(DeAssertResetMethod);
    dont_initialize();
    sensitive << mCmdCancelResetEvent;

    //Interrupt Method 
    SC_METHOD(InterruptMethod);
    dont_initialize();
    sensitive << mInterruptEvent;

    //Clock Method
    SC_METHOD(PCLKMethod);
    dont_initialize();
    sensitive << pclk;

    SC_METHOD(ClkotMethod);
    dont_initialize();
    sensitive << clkot;

    // Operation Method
    SC_METHOD(MeasureMethod);
    dont_initialize();
    sensitive << mStartEvent;

    SC_METHOD(OperationMethod);
    dont_initialize();
    sensitive << mOTACTSetEvent;
    sensitive << mCheckStopEvent;
    sensitive << mOTFSetEvent;
    sensitive << mMeasureEndEvent;
    sensitive << mOTACTClearEvent;
    
    SetLatency_TLM (0, true);
}//}}}

//destructor of Cots class
Cots::~Cots()
{//{{{
}//}}}

void Cots::start_of_simulation()
{//{{{
    OpenFile();
}//}}}

//tlm_tgt_if function
void Cots::tgt_acc (tlm::tlm_generic_payload &trans, sc_time &t)
{ //{{{
    //Get information
    tlm::tlm_command command;
    sc_dt::uint64 addr    = 0;
    unsigned char *p_data  = NULL;
    unsigned int size = 0; 
    bool status = this->tgt_get_gp_attribute(command, addr, p_data, size, trans, false);
    if(!status){
        trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
        return;
    }
    sc_assert(p_data != NULL);
    
    //Read/write unsupport address
    unsigned int temp_addr = (unsigned int)(addr&0xFF);
    if(((0x7F >= temp_addr)&&(temp_addr >= 0x44))||((0xFF >= temp_addr)&&(temp_addr >= 0xA1))){
        status =  UnsupportAddrAccess((unsigned int)addr, size, p_data, command, false);
    //Write process    
    }else if(command == tlm::TLM_WRITE_COMMAND){
        status = reg_wr((unsigned int)addr, p_data, size);
    //Red process    
    }else if(command == tlm::TLM_READ_COMMAND){
        memset(p_data,0,size); 
        status = reg_rd((unsigned int)addr, p_data, size);
    //No read/write    
    }else{
        status = true;
    }

    trans.set_response_status(tlm::TLM_OK_RESPONSE);
} //}}} 

unsigned int Cots::tgt_acc_dbg(tlm::tlm_generic_payload &trans)
{//{{{
    //Getinformation
    tlm::tlm_command command;
    sc_dt::uint64 addr = 0;
    unsigned char *p_data = NULL;
    unsigned int size = 0;
    bool status = this->tgt_get_gp_attribute(command, addr, p_data, size, trans, true);
    sc_assert(p_data != NULL);

    //Write/read unsupport address
    unsigned int temp_addr = (unsigned int)(addr&0xFF);
    if(((0x7F >= temp_addr)&&(temp_addr >= 0x44))||((0xFF >= temp_addr)&&(temp_addr >= 0xA1))){
        status =  UnsupportAddrAccess((unsigned int)addr, size, p_data, command, true);
    //Write process    
    } else if(command == tlm::TLM_WRITE_COMMAND){
        status = reg_wr_dbg((unsigned int)addr, p_data, size);
    //Read process    
    } else if(command == tlm::TLM_READ_COMMAND){
        memset(p_data,0,size); 
        status = reg_rd_dbg((unsigned int)addr, p_data, size);
    } else {
        status = true;
        size = 0;
    }

    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    if (status) {
        return size;
    } else {
        return 0;
    }
}//}}}

bool Cots::UnsupportAddrAccess(const unsigned int addr, const unsigned int size, unsigned char *p_data, tlm::tlm_command command, const bool dbg_mode)
{//{{{
    bool ret_val = true;
    if(p_data != NULL){
        //Write process
        if(command == tlm::TLM_WRITE_COMMAND){
            if (!dbg_mode) {
                re_printf("error","Address 0x%08X is not supported to write.\n", addr); 
                ret_val = false;
            } else {
                ret_val = true;
            }
        //Read process    
        }else if(command == tlm::TLM_READ_COMMAND){
            if (!dbg_mode) {
                if((size == 1)||(size == 2)||(size == 4)){
                    unsigned int temp_addr = (addr&0xFF);
                    if((((0x7F-size+1) >= temp_addr)&&(temp_addr >= 0x44))||(((0xFF-size+1) >= temp_addr)&&(temp_addr >= 0xA1))){
                        for(unsigned int i = 0; i < size; i++){
                            p_data[i] = 0;
                        }
                        ret_val = true;
                    }else{
                        re_printf("error","Access reading address 0x%08X is wrong.\n", addr); 
                        ret_val = false;
                    }
                }else{
                    re_printf("error","Reading access size to 0x%08X is wrong: %d bytes.\n", addr, size); 
                    ret_val = false;
                }
            } else {
                for(unsigned int i = 0; i < size; i++){
                    p_data[i] = 0;
                }
                ret_val = true;
            }
        //No read/write    
        }else{
            ret_val = true;
        }
    }
    return ret_val;
}//}}}

// OTS METHOD
void Cots::PCLKMethod(void)
{//{{{
    SetCLKfreq("pclk", sc_dt::uint64_to_double(pclk.read()));
}//}}}

void Cots::ClkotMethod(void)
{//{{{
    SetCLKfreq("clkot", sc_dt::uint64_to_double(clkot.read()));
}//}}}

void Cots::MeasureMethod(void)  
{//{{{ 
    if(!(mOTSPortResetFlag||mOTSCmdResetFlag)){
        //1.Set flag
        mMeasureFlag = true;

        //2.Read File
        GetTemperatureLine();

        //3.Calculate time for measure operation
        TimeCalculate();

        //4.Check first measurement
        if(mFirstLineFlag){
            mOTACTSetEvent.notify(mA2Time, SC_NS);
        }
        mCheckStopEvent.notify(mA3Time, SC_NS);
        mOTFSetEvent.notify(mA4Time, SC_NS);
        mMeasureEndEvent.notify(mA5Time, SC_NS);

        //5.Check one mode
        if((*OTCR)["OTMD"] == 0){
            mOTACTClearEvent.notify(mA6Time, SC_NS);
        }
    }
}//}}}

void Cots::OperationMethod(void)
{//{{{
    if(!(mOTSPortResetFlag||mOTSCmdResetFlag)){
        switch(mPhase){
            case emOTACTSetPhase:
                re_printf("info","The OTACT is set to 1.\n"); 
                (*OTFR)["OTACT"] = 1;
                if(mStopFlag){
                    mPhase = emOTACTClearPhase;
                }else{
                    mPhase = emCheckStopPhase;
                }
                break;
            case emCheckStopPhase:
                re_printf("info","The end of 511 measure OTS clock cycle.\n");
                mPhase = emOTFSetPhase;
                break;
            case emOTFSetPhase:
                re_printf("info","The OTF is set to 1.\n");
                (*OTFR)["OTF"] = 1;
                TemperatureAnalysis(true);
                mPhase = emMeasureEndPhase;
                break;
            case emMeasureEndPhase:
                re_printf("info","The end of 512 measure OTS clock cycle.\n");
                if((*OTCR)["OTMD"] == 0){
                    mPhase = emOTACTClearPhase;
                }else{
                    mFirstLineFlag = false;
                    mPhase = emCheckStopPhase;
                    mStartEvent.notify();
                }
                break;
            default: // emOTACTClearPhase:
                re_printf("info","The OTACT is cleared to 0.\n");
                if(mStopFlag){
                    re_printf("info","OTS state is changed from STOP state to IDLE state.\n");
                }else{
                    re_printf("info","OTS state is changed from MEASURING state to IDLE state.\n");
                }
                (*OTFR)["OTACT"] = 0;
                mPhase = emOTACTSetPhase;
                mFirstLineFlag = true;
                mMeasureFlag = false;
                mStopFlag = false;
                break;
        }
    }
}//}}}

void Cots::InterruptMethod(void)
{//{{{
    //1.Reset is not active
    if(!(mOTSPortResetFlag||mOTSCmdResetFlag)){
        bool otabe_err = (bool)(otabe.read()); 
        bool otuli_int = (bool)(otuli.read()); 
        bool oti_int   = (bool)(oti.read()); 
        bool ote_err   = (bool)(ote.read()); 
        //2.OTI Interrupt
        if(oti_int){
            DumpInterruptMsg("INT", "oti", "Deassert");
            oti.write(0);
        }else{
            re_printf("info","OTS state is changed from MEASURING state to INTERRUPT state.\n");
            DumpInterruptMsg("INT", "oti", "Assert");
            oti.write(1);
            mOTIActiveNum++;
        }

        //3.OTABE Error
        if(otabe_err){
            DumpInterruptMsg("ERR", "otabe", "Deassert");
            otabe.write(0);
        }else if(((mInterruptKind & emOTABEErr) == emOTABEErr) && ((*OTCR)["OTABEE"] == 1)){
            DumpInterruptMsg("ERR", "otabe", "Assert");
            otabe.write(1);
            mOTABEActiveNum++;
        }

        //4.OTULI Interrupt
        if(otuli_int){
            DumpInterruptMsg("INT", "otuli", "Deassert");
            otuli.write(0);
        }else if(((mInterruptKind & emOTULIIntr) == emOTULIIntr) && ((*OTCR)["OTULIE"] == 1)){
            DumpInterruptMsg("INT", "otuli", "Assert");
            otuli.write(1);
            mOTULIActiveNum++;
        }

        //5.OTE Error
        if(ote_err){
            DumpInterruptMsg("ERR", "ote", "Deassert");
            ote.write(0);
        }else if(((mInterruptKind & emOTEErr) == emOTEErr) && ((*OTCR)["OTEE"] == 1)){
            DumpInterruptMsg("ERR", "ote", "Assert");
            ote.write(1);
            mOTEActiveNum++;
        }
        if(!oti_int){
            re_printf("info","OTS state is changed from INTERRUPT state to MEASURING state.\n");
            mInterruptEvent.notify(mPCLKPeriod, SC_NS);
            mInterruptKind = emIntErrKindIni;
        }
    }else{
        DumpInterruptMsg("ERR", "otabe", "is reset to 0");
        DumpInterruptMsg("ERR", "ote", "is reset to 0");
        DumpInterruptMsg("INT", "oti", "is reset to 0");
        DumpInterruptMsg("INT", "otuli", "is reset to 0");
        otabe.write(0);
        otuli.write(0);
        ote.write(0);
        oti.write(0);
    }
}//}}}

void Cots::ResetMethod(void)
{//{{{
    //1.Reset port sensitive
    bool is_cancel_assertreset = false;
    if(preset_n.read() == vpcl::emResetActive){
        mOTSPortResetFlag = true;
    }else{
        mOTSPortResetFlag = false;
    }

    if(mOTSPortResetFlag){
        mIsInitialize = false;
        re_printf("info","Reset signal is asserted.\n"); 
        this->EnableReset(mOTSPortResetFlag);
        is_cancel_assertreset = true;
    }else{
        if(mIsInitialize){
            mIsInitialize = false;
            is_cancel_assertreset = false;
        }else{
            re_printf("info","Reset signal is negated.\n");  
            this->EnableReset(mOTSPortResetFlag);
            is_cancel_assertreset = true;
        }
    }

    //2.Call EnableReset if there is not CmdReset
    if(is_cancel_assertreset){
        mCmdResetEvent.cancel();
        mCmdCancelResetEvent.cancel();
        mIsCmdResetFlag = false;
        mOTSCmdResetFlag = false;
    }
}//}}}

void Cots::AssertResetMethod(void)
{//{{{
    mOTSCmdResetFlag = true;
    this->EnableReset(mOTSCmdResetFlag);
    mCmdCancelResetEvent.notify(mResetPeriod, SC_NS);
}//}}}

void Cots::DeAssertResetMethod(void)
{//{{{ 
    mOTSCmdResetFlag = false;
    mIsCmdResetFlag = false;
    this->EnableReset(mOTSCmdResetFlag);
}//}}}

//Call back function
bool Cots::ChkAccess(const bool is_wr,const std::string channel_name,const std::string register_name,const std::string bit_name)
{//{{{
    if(is_wr){
        if((register_name != "OTDR")&&(register_name != "COEFFRA")&&(register_name != "COEFFRB")&&(register_name != "COEFFRC")){
            if(!((mMeasureFlag)||(mStopFlag))){
                return true;
            }else{
                if((bit_name == "OTEE")||(register_name != "OTCR")){
                    re_printf("warning","Cannot write to register %s when OTS model is operating.\n", register_name.c_str()); 
                }
            }
        }else{
            if(bit_name == "SIGN"){
                re_printf("warning","Cannot write to register %s which is not supported to write.\n", register_name.c_str()); 
            }
        }
        return false;
    }else if((register_name == "COEFFRA")&&((*COEFFRA) == 0)&&(bit_name == "SIGN")){
        re_printf("warning","%s is not initialized in the constructor.\n", register_name.c_str());  
    }else if((register_name == "COEFFRB")&&((*COEFFRB) == 0)&&(bit_name == "SIGN")){
        re_printf("warning","%s is not initialized in the constructor.\n", register_name.c_str());  
    }else if((register_name == "COEFFRC")&&((*COEFFRC) == 0)&&(bit_name == "SIGN")){
        re_printf("warning","%s is not initialized in the constructor.\n", register_name.c_str());   
    }else if((register_name == "OTDR")&&(bit_name == "SIGN")){ 
        (*OTFR)["OTF"] = 0;
    }
    return true;
}//}}}

void Cots::cb_OTSTCR_OTST(RegCBstr str)
{//{{{
    if ((mFreqPCLK == 0) || (mFreqCLKOT == 0)) {
        return;
    }

    if((str.data & 0x1) == 0x1){
        (*OTSTCR)["OTST"] = 0;
        
        if((mMeasureFlag)||(mStopFlag)){  //1.OTS is operating
            re_printf("warning","Setting 1 to OTST bit in OTSTCR register is not affected when OTS model is operating.\n"); 
        }else if(PeriodCalculate()){      //2.OTS is IDLE
            if(!(((*HTBRAU) > (*HTBRAL))&&((*HTBRAL) > (*HTBRBU))&&((*HTBRBU) > (*HTBRBL))&&((*HTBRBL) > (*LTBRAU))&&((*LTBRAU) > (*LTBRAL)))){
                re_printf("error","OTS model is not started because setting the Threshold value is wrong.\n"); 
            }else{
                re_printf("info","OTS model is started.\n");   
                mStartTime = GetCurTime();
//                mStartTime = mStartTime/1000;
                re_printf("info","OTS state is changed from IDLE state to MEASURING state.\n"); 
                mStartEvent.notify();   
            }
        }
    }
}//}}}

void Cots::cb_OTENDCR_OTEND(RegCBstr str)
{//{{{
    if ((mFreqPCLK == 0) || (mFreqCLKOT == 0)) {
        return;
    }

    if((str.data & 0x1) == 0x1){
        (*OTENDCR)["OTEND"] = 0;
        std::string cur_phase = "";
        switch(mPhase){
            case emOTACTSetPhase:
                cur_phase = "OTACTSet";
                break;
            case emCheckStopPhase:
                cur_phase = "CheckStop";
                break;
            case emOTFSetPhase:
                cur_phase = "OTFSet";
                break;
            case emMeasureEndPhase:
                cur_phase = "MeasureEnd";
                break;
            default:
                cur_phase = "OTACTClear";
                break;
        }

        if(mStopFlag){            //1.OTS is in stop operation
            re_printf("warning","Setting 1 to OTEND bit in OTENDCR register is not affected when OTS is in stop operation.\n"); 
        }else if(!mMeasureFlag){  //2.OTS is not in measure operation
            re_printf("warning","Setting 1 to OTEND bit in OTENDCR register is not affected when OTS model is not started yet.\n");
        }else if((mPhase == emOTFSetPhase)||(mPhase == emMeasureEndPhase)||(mPhase == emOTACTClearPhase)){//3.OTS is in the final phases of measure operation
            re_printf("warning","Setting 1 to OTEND bit in OTENDCR register is not affected in the %s phase.\n",cur_phase.c_str()); 
        }else{                   //4.Measure operation is forced stopping, stop operation is implemented 
            re_printf("info","OTS model is forced stopping in %s phase.\n", cur_phase.c_str()); 
            re_printf("info","OTS state is changed from MEASURING state to STOP state.\n");

            //4.1.Cancel all events in measure operation   
            mStartEvent.cancel();
            mCheckStopEvent.cancel();
            mOTFSetEvent.cancel();
            mMeasureEndEvent.cancel();
            mOTACTClearEvent.cancel();
            mStopFlag = true;
            mMeasureFlag = false;

            //4.2.Beginning stop operation
            if((*OTFR)["OTACT"] == 1){
                mPhase = emOTACTClearPhase;
            }
            TimeCalculate();
            mOTACTClearEvent.notify(mA7Time, SC_NS);
        } 
    }
}//}}}

void Cots::cb_OTFCR_SDERC(RegCBstr str)
{//{{{
    if ((mFreqPCLK == 0) || (mFreqCLKOT == 0)) {
        return;
    }

    //1.For SDERC
    if((str.data & 0x4) == 0x4){
        (*OTFCR)["SDERC"] = 0;
        if((*OTFR)["SDER"] == 0){
            re_printf("warning","Setting 1 to SDERC is not affected because SDER is not 1 before.\n"); 
        }else{
            (*OTFR)["SDER"] = 0;
        }
    }

    //2.For OTFC
    if((str.data & 0x1) == 0x1){
        (*OTFCR)["OTFC"] = 0;
        if((*OTFR)["OTF"] == 0){
            re_printf("warning","Setting 1 to OTFC is not affected because OTF is not 1 before.\n"); 
        }else{
            (*OTFR)["OTF"] = 0;
        }
    }
}//}}}

// OTS internal function
bool Cots::PeriodCalculate(void) 
{//{{{
    if(!(mFreqCLKOT < (mFreqPCLK*emClockCondConst))){ //2.Check the condition between pclk and clkot 
        re_printf("error","The condition between pclk frequency and clkot frequency is not correct.\n"); 
        return false;
    }else{//3.Calculate the periods
        mPCLKPeriod  = (double)(emNanoSecond/(mFreqPCLK*AccelerationRate)); 
        mclkotPeriod = (double)(emNanoSecond/(mFreqCLKOT*AccelerationRate));
        return true;
    }
}//}}}

void Cots::TimeCalculate(void)
{//{{{
    double current_time   = GetCurTime();
//    current_time          = current_time/1000;
    unsigned int quotient = 0;
    double otsclk_period  = emMultiplePCLK*mclkotPeriod;
    
    if(mStopFlag){//1.Stop operation
        //A7
        quotient = (unsigned int)((current_time - mStartTime)/otsclk_period);
        double X = (double)((current_time - mStartTime) - quotient*otsclk_period);
        quotient = (unsigned int)((current_time - mStartTime + 2*otsclk_period - X)/(mPCLKPeriod));
        mA7Time  = (double)((quotient+1)*(mPCLKPeriod) - (current_time - mStartTime));
    }else{ //2.Measure operation
        if(mFirstLineFlag){
            //A1*
            quotient = (unsigned int)((4*mPCLKPeriod + mclkotPeriod)/mclkotPeriod);
            double A1Time = (double)((quotient+1)*mclkotPeriod); 

            //A2* 
            quotient = (unsigned int)(A1Time/mPCLKPeriod);
            mA2Time  = (double)((quotient+1)*mPCLKPeriod);

            //A3* 
            mA3Time  = A1Time + (emMeasureCycleNum+2)*otsclk_period;

            //A4*
            quotient = (unsigned int)((mA3Time + 0.5*otsclk_period + emMultipleClkot*mclkotPeriod + 2*mPCLKPeriod)/mPCLKPeriod);
            mA4Time  = (double)((quotient+1)*mPCLKPeriod);

            //A6
            if((*OTCR)["OTMD"] == 0){
                quotient = (unsigned int)((A1Time + (emMeasureCycleNum+3)*(otsclk_period))/mPCLKPeriod);
                mA6Time  = (double)((quotient+1)*mPCLKPeriod);
            }
        }else{
            //A3* 
            mA3Time  = (emMeasureCycleNum-1)*otsclk_period;
            //A4*
            quotient = (unsigned int)((current_time - mStartTime + mA3Time + 0.5*otsclk_period + emMultipleClkot*mclkotPeriod +2*mPCLKPeriod)/mPCLKPeriod);
            mA4Time  = (double)((quotient+1)*mPCLKPeriod - (current_time - mStartTime));
        }
        //A5*
        mA5Time = mA3Time + otsclk_period;

    }
}//}}}

void Cots::OpenFile(void)
{//{{{
    mFileStream.open(temp_file.value.c_str());
    if(!mFileStream){
        printf("warning [%20s] (%s) Cannot read temperature file %s.\n",sc_time_stamp().to_string().c_str(), this->name(), temp_file.value.c_str()); 
        mErrorFileFlag = true;
    }
}//}}}

void Cots::GetTemperatureLine(void)
{//{{{
    if(mErrorFileFlag){//1.File is error
        mNewTempFile = (unsigned int)(*OTDR); 
    }else{//2.File is not error
        std::string temperature_line = "";
        bool next_line_flag = true;
        while(next_line_flag){
            next_line_flag = false;
            if(!getline(mFileStream,temperature_line)){ //2.1.End of file
                if(mNoLineFlag){
                    re_printf("warning","The temperature file is empty.\n");
                }else{
                    if((!mLegalLineFlag)&&(mIllegalLineFlag)){
                        re_printf("warning","All lines in temperature file are illegal.\n");
                    }else if(mEmptyFileFlag){
                        re_printf("warning","The temperature file is empty.\n");
                    }else{
                        re_printf("warning","The temperature file is run out.\n");
                    }
                }
                mNewTempFile = (unsigned int)(*OTDR);  
            }else{ //2.2.Not end of file
                mNoLineFlag = false;
                bool comment_flag = false;
                bool hex_symbol_flag = false;

                //2.2.1 Clear the comment line
                if(temperature_line.find("//") != std::string::npos){  // Catch the "//" symbol
                    comment_flag = true;
                    temperature_line.erase(temperature_line.find("//")); // Clear comment line
                }
                if((temperature_line.find("0X") != std::string::npos)||(temperature_line.find("0x") != std::string::npos)){  // Catch the "0X" or "0x" symbol
                    hex_symbol_flag = true;
                }  
                
                //2.2.2 Get the data in the line
                std::istringstream string_value(temperature_line.c_str());
                unsigned int temperature_count = 0;
                std::string temperature_value = "";
                while(string_value){
                    string_value >> temperature_value;
                    temperature_count++;
                }
                if(temperature_value == ""){  //There is no value 
                    if(comment_flag){
                        mEmptyFileFlag = false;
                    }
                    next_line_flag = true;
                }else if((temperature_count == 2)&&(!comment_flag)&&(hex_symbol_flag)) { //There is one value
                    mEmptyFileFlag = false;
                    char *err_p = NULL;
                    double real_value = (double)strtol(temperature_value.c_str(), &err_p, 16);
                    if((errno != ERANGE)&&(*err_p == '\0')){ 
                        mLegalLineFlag = true; 
                        mNewTempFile = (unsigned int)(real_value);
                        if(real_value < MinTemperature){
                            re_printf("warning","The temperature data is out of range.\n");
                        }else if(real_value > MaxTemperature){
                            re_printf("warning","The temperature data is out of range.\n");
                        }
                    }else{   
                        mIllegalLineFlag = true;
                        re_printf("warning","The temperature line is not correct format.\n");
                        mNewTempFile = (unsigned int)(*OTDR);
                    }
                }else{
                    mEmptyFileFlag = false;
                    mIllegalLineFlag = true;
                    mNewTempFile = (unsigned int)(*OTDR);  
                    re_printf("warning","The temperature line is not correct format.\n");
                }
            }// end of "not end of file"
        } // end of "while(next_line_flag)"
    } // end for "file is not error"
}//}}}

void Cots::TemperatureAnalysis(const bool intr_err_issue_flag)
{//{{{
    
    unsigned int T1 = (unsigned int)(*HTBRAU);
    unsigned int T2 = (unsigned int)(*HTBRAL);
    unsigned int T3 = (unsigned int)(*HTBRBU);
    unsigned int T4 = (unsigned int)(*HTBRBL);
    unsigned int T5 = (unsigned int)(*LTBRAU);
    unsigned int T6 = (unsigned int)(*LTBRAL);
    bool otabe_flag = false;
    bool otuli_flag = false;
    unsigned int pre_temperature = (unsigned int)(*OTDR);
    unsigned int new_temperature = 0;
    unsigned int new_status = 0;
    unsigned int pre_status = (unsigned int)((*OTSTR)["TSTAT"]);
    if((mFirstLineFlag)&&(intr_err_issue_flag)){
         pre_status = emNormal;
    }

    //1.Correct the new temperature
    if(intr_err_issue_flag){
        if(mNewTempFile < MinTemperature){
            new_temperature = MinTemperature;
        }else if(mNewTempFile > MaxTemperature){
            new_temperature = MaxTemperature;
        }else{
            new_temperature = mNewTempFile;
        }
    }else{
        if(mNewTempSetTemp < MinTemperature){
            new_temperature = MinTemperature;
        }else if(mNewTempSetTemp > MaxTemperature){
            new_temperature = MaxTemperature;
        }
        else{
            new_temperature = mNewTempSetTemp;
        }
    }

    //2.Get the new stauts and interrupt kind
    switch(pre_status){
        case emHighA: //emHighA
            if((MaxTemperature >= new_temperature)&&(new_temperature >= T2)){
                new_status = emHighA;
            }else if((T2 > new_temperature)&&(new_temperature >= T4)){
                new_status = emHighB;
                otuli_flag = true;
            }else if((T4 > new_temperature)&&(new_temperature >= T6)){
                new_status = emNormal;
                otuli_flag = true;
            }else if((T6 > new_temperature)&&(new_temperature >= MinTemperature)){
                new_status = emLowA;
                otabe_flag = true;
            }
            break;
        case emHighB: //emHighB
            if((MaxTemperature >= new_temperature)&&(new_temperature >= T1)){
                new_status = emHighA;
                otabe_flag = true;
            }else if((T1 > new_temperature)&&(new_temperature >= T4)){
                new_status = emHighB;
            }else if((T4 > new_temperature)&&(new_temperature >= T6)){
                new_status = emNormal;
                otuli_flag = true;
            }else if((T6 > new_temperature)&&(new_temperature >= MinTemperature)){
                new_status = emLowA;
                otabe_flag = true;
            }    
            break;
        case emNormal: //emNormal
            if((MaxTemperature >= new_temperature)&&(new_temperature >= T1)){
                new_status = emHighA;
                otabe_flag = true;
            }else if((T1 > new_temperature)&&(new_temperature >= T3)){
                new_status = emHighB;
                otuli_flag = true;
            }else if((T3 > new_temperature)&&(new_temperature >= T6)){
                new_status = emNormal;
            }else if((T6 > new_temperature)&&(new_temperature >= MinTemperature)){
                new_status = emLowA;
                otabe_flag = true;
            }    
            break;
        default: //emLowA
            if((MaxTemperature >= new_temperature)&&(new_temperature >= T1)){
                new_status = emHighA;
                otabe_flag = true;
            }else if((T1 > new_temperature)&&(new_temperature >= T3)){
                new_status = emHighB;
                otuli_flag = true;
            }else if((T3 > new_temperature)&&(new_temperature >= T5)){
                new_status = emNormal;
                otuli_flag = true;
            }else if((T5 > new_temperature)&&(new_temperature >= MinTemperature)){
                new_status = emLowA;
            }
            break;
    }

    //4.Update the temperature into OTDR register
    re_printf("info","New temperature is updated to OTDR register.\n");  
    (*OTDR) = new_temperature;

    //5.Update new status 
    re_printf("info","The sensor status (TSTAT) is changed from %X to %X  \n", pre_status, new_status);
    (*OTSTR)["TSTAT"] = new_status;

    if(intr_err_issue_flag){
        //6.Update InterruptKind
        if(otabe_flag){
            mInterruptKind |= emOTABEErr;
        }
        if(otuli_flag){
            mInterruptKind |= emOTULIIntr;
        }

        //7.Diagnosis error
        if(((*OTCR)["OTMD"] == 1)&&((*OTCR)["SDE"] == 1)&&(!mFirstLineFlag)){
            unsigned int minus_result = 0;
            if(new_temperature > pre_temperature){
                minus_result = new_temperature - pre_temperature;   
            }else{
                minus_result = pre_temperature - new_temperature;   
            }
            if (minus_result > (*TDLR)){
                re_printf("info","The SDER is set to 1.\n");
                (*OTFR)["SDER"] = 1;
                mInterruptKind |= emOTEErr;
            }
        }

        //8.Call the Interrupt Method
        mInterruptEvent.notify();
    }

}//}}}

void Cots::EnableReset(const bool is_active)
{//{{{
    if(is_active){
        //1.Cancel all events
        mInterruptEvent.cancel();
        mStartEvent.cancel();
        mOTACTSetEvent.cancel();
        mCheckStopEvent.cancel();
        mOTFSetEvent.cancel();
        mMeasureEndEvent.cancel();
        mOTACTClearEvent.cancel();
        //2.Reset output port
        mInterruptKind = emIntErrKindIni;
        mInterruptEvent.notify();
        //3.Close File
        if(!mErrorFileFlag){
            mFileStream.close();
        }
        //4.Dump report
        if(mStopFlag){
            re_printf("info","OTS state is changed from STOP state to RESET state.\n");
        }else if(mMeasureFlag){
            re_printf("info","OTS state is changed from MEASURING state to RESET state.\n");
        }else{
            re_printf("info","OTS state is changed from IDLE state to RESET state.\n");
        }
        //5.Global variable init
        GlobalInit();
    }else{
        if((!mErrorFileFlag)&&(!mFileStream.is_open())){
            OpenFile();
        }
        re_printf("info","OTS state is changed from RESET state to IDLE state.\n");
    }
    Cots_regif::EnableReset(is_active);
    if(mOTSCmdResetFlag == false){
        this->tgt_enable_reset(is_active);
    }
}//}}}

void Cots::GlobalInit(void)
{//{{{
    mPCLKPeriod       = 0;
    mclkotPeriod      = 0;
    mStartTime        = 0;
    mA2Time           = 0;
    mA3Time           = 0;
    mA4Time           = 0;
    mA5Time           = 0;
    mA6Time           = 0;
    mA7Time           = 0;
    mPhase            = emOTACTSetPhase;
    mInterruptKind    = emIntErrKindIni;
    mNewTempSetTemp   = 0;
    mNewTempFile      = 0;
    mOTIActiveNum     = 0;
    mOTABEActiveNum   = 0;
    mOTULIActiveNum   = 0;
    mOTEActiveNum     = 0;
    //Flag
    mNoLineFlag       = true;
    mLegalLineFlag    = false;
    mIllegalLineFlag  = false;
    mEmptyFileFlag    = true;
    mMeasureFlag      = false;
    mStopFlag         = false;
    mFirstLineFlag    = true;
}//}}}

// handleCommand function
void Cots::DumpInfo(const char *type, const char *message, ... )
{//{{{
    if((message != NULL)&&(type != NULL)) {
        printf ("PROFILE(%s): OTS: ", type);
        // print body
        va_list argptr;
        va_start(argptr, message);
        vprintf(message, argptr);
        va_end(argptr);
    }
}//}}}

void Cots::DumpStatInfo(void)
{//{{{
    //1.Dump information
    DumpInfo("StatInfo", "Info[%s ns](%s).\n",sc_time_stamp().to_string().c_str(), this->name());
    DumpInfo("StatInfo", " OTCR                : %x\n",(unsigned int)(*OTCR)); 
    DumpInfo("StatInfo", " OTSTR               : %x\n",(unsigned int)(*OTSTR)); 
    DumpInfo("StatInfo", " OTDR                : %x\n",(unsigned int)(*OTDR)); 
    DumpInfo("StatInfo", " TDLR                : %x\n",(unsigned int)(*TDLR)); 
    DumpInfo("StatInfo", " OTABE active number : %d\n",mOTABEActiveNum); 
    DumpInfo("StatInfo", " OTULI active number : %d\n",mOTULIActiveNum); 
    DumpInfo("StatInfo", " OTE active number   : %d\n",mOTEActiveNum); 
    DumpInfo("StatInfo", " OTI active number   : %d\n",mOTIActiveNum); 
    DumpInfo("StatInfo", "EndInfo.\n");

    //2.Reset information variable
    mOTABEActiveNum = 0;
    mOTULIActiveNum = 0;
    mOTEActiveNum   = 0;
    mOTIActiveNum   = 0;
}//}}}

void Cots::DumpInterruptMsg (const std::string intr_name, const std::string type, const std::string issue)
{//{{{
    if(DumpInterrupt){
        re_printf("info","%s [OTS: %s] %s.\n", type.c_str(), intr_name.c_str(), issue.c_str());
    }
}//}}} 

void Cots::AssertReset(const double start_time, const double period)
{//{{{
    if((!mOTSPortResetFlag)&&(!mIsCmdResetFlag)) {
        mIsCmdResetFlag = true;
        re_printf("info","OTS will reset for %lld.%06d ns after %lld.%06d ns.\n", (unsigned long long) period, FRACTION_TO_INT(period), (unsigned long long) start_time, FRACTION_TO_INT(start_time));
        mCmdResetEvent.notify(start_time, SC_NS);
        mResetPeriod = period;
    }else{
        re_printf("warning","Reset is in progress.\n");
    }
}//}}}

void Cots::SetTemp(const unsigned short temperature)
{//{{{
    if((!mOTSPortResetFlag)&&(!mIsCmdResetFlag)){
        if(((*OTFR)["OTF"] == 0)||(!(mMeasureFlag||mStopFlag))){
            mNewTempSetTemp = (unsigned int)(temperature);
            TemperatureAnalysis(false);
        }else{
            re_printf("warning","SetTemp command is ignored when OTF flag in OTFR register is 1.\n");
        }
    }else{
        re_printf("warning","Cannot call SetTemp during reset time.\n");
    }

}//}}}

double Cots::GetTimeResolution(void)
{//{{{
    double tu_value = 0;
    if (sc_get_time_resolution() < sc_time(1, SC_PS)){
        tu_value = 1000000000000000LL;
    } else if (sc_get_time_resolution() < sc_time(1, SC_NS)){
        tu_value = 1000000000000LL;
    } else if (sc_get_time_resolution() < sc_time(1, SC_US)){
        tu_value = 1000000000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_MS)){
        tu_value = 1000000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)){
        tu_value = 1000;
    } else {
        tu_value = 1;
    }
    return tu_value;
}//}}}

void Cots::SetCLKfreq(std::string clk_name, double clk_freq)
{//{{{  
    if ((clk_name != "pclk")&&(clk_name != "clkot")) {
        re_printf("error","Clock name is invalid.\n");
        return;
    }

    double clk_period = 0 ;

    if(clk_freq == 0){
        //cancel all processes
        mInterruptEvent.cancel();
        mStartEvent.cancel();
        mOTACTSetEvent.cancel();
        mCheckStopEvent.cancel();
        mOTFSetEvent.cancel();
        mMeasureEndEvent.cancel();
        mOTACTClearEvent.cancel();
    } else {
        clk_period  = (double)(emNanoSecond / clk_freq);
        double time_unit    = (double)(emNanoSecond / GetTimeResolution());
        if(time_unit > clk_period) {
            re_printf("warning","The %s period is less than 1 unit time of system.\n", clk_name.c_str());
            return;
        }
    }    

    if (clk_name == "pclk") {
        mFreqPCLK  = clk_freq;
        SetLatency_TLM(clk_period, false);
    } else {
        mFreqCLKOT  = clk_freq;
    }
    re_printf("info","The %s is set with a frequency as %lld.%06d.\n", clk_name.c_str(), (unsigned long long) clk_freq, FRACTION_TO_INT(clk_freq));
}//}}}

void Cots::SetLatency_TLM(const double pclk_period, const bool is_constructor)
{//{{{ 
    vpcl::tlm_if_tgt_parameter tgt_param = vpcl::tlm_tgt_if<32>::tgt_get_param();
    if(is_constructor){
        tgt_param.fw_req_phase = tlm::END_REQ;
    }
    sc_time new_clock(pclk_period, SC_NS);
    tgt_param.bus_clk = new_clock;
    tgt_param.rd_latency = tgt_param.rd_req_latency + 2 * tgt_param.bus_clk;
    tgt_param.wr_latency = tgt_param.wr_req_latency + 2 * tgt_param.bus_clk;
    vpcl::tlm_tgt_if<32>::tgt_set_param(&tgt_param);
}//}}}

/*********************************
// Function     : GetCurTime
// Description  : Get current simulation time
// Parameter    : None
// Return value : Current simulation time (unit SC_NS)
**********************************/
double Cots::GetCurTime(void)
{//{{{
    double cur_time = sc_time_stamp().to_double();
    double ret = 0;
    if (sc_get_time_resolution() < sc_time(1, SC_PS)) {//SC_FS
        ret = cur_time/1000000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_NS)) {//SC_PS
        ret = cur_time/1000;
    }  else if (sc_get_time_resolution() < sc_time(1, SC_US)) {//SC_NS
        ret = cur_time;
    } else if (sc_get_time_resolution() < sc_time(1, SC_MS)) {//SC_US
        ret = cur_time * 1000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)) {//SC_MS
        ret = cur_time * 1000000;
    } else {//SC_SEC
        ret = cur_time * 1000000000;
    }
    return ret;
}//}}}

// vim600: set foldmethod=marker :
