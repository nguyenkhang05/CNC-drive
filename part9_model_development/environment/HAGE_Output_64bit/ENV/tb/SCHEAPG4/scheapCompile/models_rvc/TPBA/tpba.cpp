// -----------------------------------------------------------------------------
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
// -----------------------------------------------------------------------------

#include <string>
#include <cstdarg>
#include "tpba.h"
#include "re_define.h"
#ifndef FRACTION_TO_INT
#define FRACTION_TO_INT(d) ((unsigned int)((d - (unsigned long long)(d))*1000000))
#endif

//Constructor of Ctpba_counter class
Ctpba_counter::Ctpba_counter(sc_module_name name):Cgeneral_timer(name)
                                                 ,mCompareMatch("mCompareMatch")
{//{{{
    Initialize();
    SC_METHOD(WriteCmpSignalMethod);
    dont_initialize();
    sensitive << mWriteCmpSignalEvent;
}//}}}
//Destructor of Ctpba_counter class
Ctpba_counter::~Ctpba_counter()
{//{{{
}//}}}
//Start_stop TPBA counter
void Ctpba_counter::StartStopCounter (const bool start, const double clk_period, const unsigned int clk_divide)
{//{{{
    if(start){
        Cgeneral_timer::setCNT(0);
        Cgeneral_timer::setClkCountPeriod(clk_period);
        Cgeneral_timer::setCKS(clk_divide);
        Cgeneral_timer::setSTR(true);
    }else{
        Cgeneral_timer::setSTR(false);
    }
}//}}}
//Set compare match value
void Ctpba_counter::SetCompareMatchValue (const unsigned int value)
{//{{{
    Cgeneral_timer::setCOR(value);
}//}}}
//Initialize
void Ctpba_counter::Initialize(void)
{//{{{
    mCmpValue = false;
    mWriteCmpSignalEvent.notify();
    Cgeneral_timer::setCmpMatchOutVal(false);
    Cgeneral_timer::setSTR(false);//Count permission
    Cgeneral_timer::setCMS(17U);
    Cgeneral_timer::setCMM(emOneShot);//Operation mode
    Cgeneral_timer::setCounterDirection(emCountUp);//Count mode
    Cgeneral_timer::setCCLR(emInFactor);//Clear mode
    Cgeneral_timer::setTimeUnit(SC_NS);//Time unit
    Cgeneral_timer::setCOR(0xFFFF);//Match value
    Cgeneral_timer::setCNT(0xFFFF);//Count value
    Cgeneral_timer::setCKS(1);//Divide Input clock
    Cgeneral_timer::setCmpMatchOutType(emToggle);//Output type of counter when compare match
}//}}}

//Get CNT value
unsigned int Ctpba_counter::GetCNTValue(void)
{//{{{
    return Cgeneral_timer::getCNT();
}//}}}

void Ctpba_counter::cmpMatchOutputHandling (bool value)
{//{{{
    mCmpValue = value;
    mWriteCmpSignalEvent.notify();
}//}}}

void Ctpba_counter::WriteCmpSignalMethod(void)
{//{{{
    mCompareMatch.write(mCmpValue);
}//}}}
//Constructor of TPBA class
Ctpba::Ctpba(sc_module_name name):Ctpba_regif((std::string)name, 32)
                                 ,vpcl::tlm_tgt_if<32> (name)
                                 ,pclk("pclk")
                                 ,preset_n("preset_n")
                                 ,TPBATSST("TPBATSST")
                                 ,TPBATINTPRD("TPBATINTPRD")
                                 ,TPBATINTDTY("TPBATINTDTY")
                                 ,TPBATINTPAT("TPBATINTPAT")
                                 ,TPBATTOUT("TPBATTOUT")
{//{{{
    CommandInit(this->name());
    mPeriodCounter = new Ctpba_counter("mPeriodCounter");
    sc_assert(mPeriodCounter != NULL);

    mDutyCounter = new Ctpba_counter("mDutyCounter");
    sc_assert(mDutyCounter != NULL);

    set_instance_name(this->name());
    ReInitialize(true);
    
    SC_METHOD(AssertResetMethod);
    dont_initialize();
    sensitive << mAssertResetEvent;

    SC_METHOD(ResetMethod);
    dont_initialize();
    sensitive << preset_n;

    SC_METHOD(CancelAssertResetMethod);
    dont_initialize();
    sensitive << mCancelAssertResetEvent;

    SC_METHOD(PCLKPeriodMethod);
    dont_initialize();
    sensitive << pclk;
    
    SC_METHOD(TSSTMethod);
    dont_initialize();
    sensitive << TPBATSST.neg();

    SC_METHOD(StartMethod);
    dont_initialize();
    sensitive << mTSSTEvent;
    sensitive << mTSEvent;

    SC_METHOD(MatchMethod);
    dont_initialize();
    sensitive << mWritePRDEvent;
    sensitive << mPeriodCounter->mCompareMatch;
    sensitive << mDutyCounter->mCompareMatch;
    
    SC_METHOD(WaitingMethod);
    dont_initialize();
    sensitive << mWaitingEvent;

    SC_METHOD(WriteTOUTMethod);
    dont_initialize();
    sensitive << mWriteTOUTEvent;

    SC_METHOD(DTYMethod);
    dont_initialize();
    sensitive << mDTYEvent;
    
    SC_METHOD(PRDPATMethod);
    dont_initialize();
    sensitive << mPRDPATEvent;

    SC_METHOD(StopMethod);
    dont_initialize();
    sensitive << mTTEvent;  
        
    mStatus = emIdle;
    mCmdIfRstFlag = false;
    mIsReset = false;
    mPeriodTime = 0;
    freq_PCLK = 0;
    mIsInitialize = true;
    SetLatency_TLM (0, true);
}//}}}

//destructor of Ctpba class
Ctpba::~Ctpba()
{//{{{
    delete mPeriodCounter;
    delete mDutyCounter;
}//}}}

void Ctpba::tgt_acc (tlm::tlm_generic_payload &trans, sc_time &t)
{//{{{
    // Get transaction attributes
    tlm::tlm_command command;
    sc_dt::uint64 address = 0;
    unsigned char *data_ptr = NULL;
    unsigned int data_length = 0;
    bool status = this->tgt_get_gp_attribute(command, address, data_ptr, data_length, trans, false);
    if (!status) {
        trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
        return;
    }
    sc_assert(data_ptr != NULL);
    
    // Set debug mode (member variable of Ctpba_regif class)
    // dbg_mode = false;

    // Callback method
    if (command == tlm::TLM_READ_COMMAND) {
        memset(data_ptr, 0, data_length);
        status = reg_rd((unsigned int) address, data_ptr, data_length);
        if (status){
            if (((address&0x3FF) == 0x012C) || ((address&0x3FF) == 0x0130)){
                data_ptr[0] = 0;
            }
        }
    }
    else if (command == tlm::TLM_WRITE_COMMAND) {
        status = reg_wr((unsigned int) address, data_ptr, data_length);
    }
    // Be necessarily TLM_IGNORE_COMMAND
    else {
        status = true;
    }

    // Set response status attribute
    if(status){
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    }else{
        trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    }
}//}}}

unsigned int Ctpba::tgt_acc_dbg(tlm::tlm_generic_payload &trans)
{//{{{
    // Get transaction attributes 
    tlm::tlm_command command;
    sc_dt::uint64 address = 0;
    unsigned char *data_ptr = NULL;
    unsigned int data_length = 0;
    bool status = this->tgt_get_gp_attribute(command, address, data_ptr, data_length, trans, true);
    sc_assert(data_ptr != NULL);
    
    // Set debug mode (member variable of Ctpba_regif class)
    // dbg_mode = true;

    // Callback method 
    if (command == tlm::TLM_READ_COMMAND) {
        memset(data_ptr, 0, data_length);
        status = reg_rd_dbg((unsigned int) address, data_ptr, data_length);
    }
    else if (command == tlm::TLM_WRITE_COMMAND) {
        status = reg_wr_dbg((unsigned int) address, data_ptr, data_length);
    }
    // Be necessarily TLM_IGNORE_COMMAND
    else {
        status = true;
        data_length = 0;
    }

    // Set response status attribute and return value
    if (status) {
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
        return data_length;
    }
    else {
        trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
        return 0;
    }
}//}}}

bool Ctpba::ChkAccess(const bool is_wr,const std::string channel_name,const std::string register_name,const std::string bit_name)
{//{{{
    if (is_wr){
        if ((register_name == "TPBA0TS") || (register_name == "TPBA0CTL")){
            if ((*TPBA0TE)["TPBA0TE0"] == 1){
                re_printf("error","Cannot write to %s while TPBA is enabled.\n", register_name.c_str());
                return false;
            } else {
                return true;
            }
        } else { //TPBA0TO check access
            if ((*TPBA0TOE)["TPBA0TOE0"] == 1){
                re_printf("error","Cannot write to %s while TPBA timer output is enabled.\n", register_name.c_str());
                return false;
            } else {
                return true;
            }
        }
    } else {
        return true;
    }
}//}}}

void Ctpba::PCLKPeriodMethod(void)
{//{{{
    SetCLKfreq("pclk",(double)(pclk.read()));
}//}}}

void Ctpba::TSSTMethod(void)
{//{{{    this method is notified when TSST signal occurs (neg edge)
    double start_time = sc_time_stamp().to_double();
    if (mIsReset == false){
        if (mStopTime != start_time){
            mTSSTFlag = true;
            mTSSTEvent.notify();
        }
    }
}//}}}

//TPBA is started 
void Ctpba::StartMethod(void)
{//{{{    this method is notified by TSSTMethod or TSEvent
    if (freq_PCLK != 0){
        if ((*TPBA0TE)["TPBA0TE0"] == 0){
            if (mTSSTFlag == true){//TPBA is started by TSST port
                re_printf("info","TPBA is started by TPBATSST.\n");
                mTSSTFlag = false;
                mClkPeriod = (double)((1/freq_PCLK)*emNanoSecond);
            } else {
                re_printf("info","TPBA is started by setting 1 to TPBA0TS0 bit.\n");
                (*TPBA0TS)["TPBA0TS0"] = 0;
            }         
            mStatus = emCounting;
            mIsStartCounting = true;
            (*TPBA0TE)["TPBA0TE0"] = 1;
            mTPBA0CB0 = (unsigned int)(*TPBA0CMP0);
            mTPBA0CB1 = (unsigned int)(*TPBA0CMP1)["TPBA0CMP1"];
            (*TPBA0RDT)["TPBA0RDT0"] = 0;
            (*TPBA0RDT)["TPBA0RDT1"] = 0;
            (*TPBA0RSF)["TPBA0RSF0"] = 0;
            (*TPBA0RSF)["TPBA0RSF1"] = 0;
            //after 1 cycle WritePRSEvent is notified
            mWritePRDEvent.notify(mClkPeriod, SC_NS);            
        } else {
            re_printf("warning","TPBA has been already started.\n");
        }
    } else {
        re_printf("error","pclk must be different from 0.\n");
    }
}//}}}

void Ctpba::StopMethod(void)
{//{{{
    re_printf("info","TPBA is stopped.\n");
    mStopTime = sc_time_stamp().to_double();
    (*TPBA0TE)["TPBA0TE0"] = 0;
    (*TPBA0TT)["TPBA0TT0"] = 0;
    (*TPBA0TS)["TPBA0TS0"] = 0;
    mWritePRDEvent.cancel();
    mStatus = emIdle;
    DumpTransInfo("IDLE");
    //Counters stop
    mPeriodCounter->StartStopCounter(false, mClkPeriod, 1);
    mDutyCounter->StartStopCounter(false, mClkPeriod, 1);
    //Deassert all interrupt outputs
    mPRDForceStop = true;
    mDTYForceStop = true;
    mPRDPATEvent.notify(mClkPeriod, SC_NS);
    mDTYEvent.notify(mClkPeriod, SC_NS);
}//}}}

void Ctpba::MatchMethod(void)
{//{{{
    if (mIsReset == false){
        unsigned int pre_cb0 = mTPBA0CB0;
        unsigned int pre_cb2 = mTPBA0CB2;
        mPRDValue = (bool)(!(mPeriodCounter->mCompareMatch.read()));
        mDTYValue = (bool)(mDutyCounter->mCompareMatch.read());

        //1. Duty matching 
        if((mDTYValue != mPreDTYValue) && (!mIsStartCounting)){
            DumpTransInfo("DUTY MATCHING");
            //1.1 Assert TOUT
            if((*TPBA0TOE)["TPBA0TOE0"] == 1){
                if(mTPBA0TOLB == 0){
                    mWriteTOUTValue = false; 
                }else{
                    mWriteTOUTValue = true; 
                }
                mWriteTOUTEvent.notify(); 
            } 
            //1.2 Assert DTY when CB0 > CB2
            if((pre_cb0 > pre_cb2) && mAssertDTY){
                DumpTransInfo("SINGLE COUNTER COUNTING");
                mDTYEvent.notify();             
            }    
            mAssertDTY = true;
        } 

        //2. Period matching    
        if(mIsStartCounting || (mPRDValue != mPrePRDValue) || ((pre_cb2 == pre_cb0) && (mDTYValue != mPreDTYValue))){
            bool TOLB_temp = mTPBA0TOLB;
            unsigned int PRS_value = 1 << (unsigned int)(((*TPBA0CTL)["TPBA0PRS1"] << 1) | (*TPBA0CTL)["TPBA0PRS0"]);  
            mPRDValue = (bool)(!(mPeriodCounter->mCompareMatch.read()));
            mPrdCount++;

            if ((*TPBA0RDM)["TPBA0RDM0"] == 1){
                //Reload CB0
                if ((*TPBA0RSF)["TPBA0RSF0"] == 1){
                    mTPBA0CB0 = (unsigned int)(*TPBA0CMP0);
                    re_printf("info","CB0 is reloaded: %d.\n", mTPBA0CB0);
                    (*TPBA0RSF)["TPBA0RSF0"] = 0;
                }
                mTPBA0TOLB = (bool)(*TPBA0TOL)["TPBA0TOL0"];
            }
            mPointerBuf = (unsigned int)(*TPBA0CNT1)["TPBA0CNT1"];      
            if ((*TPBA0CTL)["TPBA0DPS"] == 0){//16bit x 64 pattern
                mTPBA0CB2 = (unsigned int)(*TPBA0BUF[mPointerBuf]); 
            } else {//DPS = 1, 8bit x 128 pattern
                if (mPointerBuf % 2 == 0){
                    mTPBA0CB2 = (unsigned int)(*TPBA0BUF[mPointerBuf / 2]) & 0xFF;
                } else {
                    mTPBA0CB2 = (unsigned int)(((*TPBA0BUF[mPointerBuf / 2]) >> 8) & 0xFF);
                }
            }
            re_printf("info","CB2 is reloaded: %d .\n", mTPBA0CB2);
            mPATMatching = false;

            //2.1 For PAT output
            if ((mPointerBuf == mTPBA0CB1) || (((*TPBA0CTL)["TPBA0DPS"] == 0) && (mPointerBuf == 0x3F)) ){
                bool special_flag = false;
                (*TPBA0CNT1)["TPBA0CNT1"] = 0;
                //This is a special case, when CB1 = 0, PAT interrupt is notified continuously until CB1 is reloaded
                if (mTPBA0CB1 == 0){
                    //At the moment, TPBA starts working, mPATFlag is FALSE
                    if (mPATFlag){
                        mPATMatching = true;
                        mPATValue = (mPATCounter % 2 == 0)? true: false;
                        mPatCount++;
                        mPATCounter++;
                        //Reload CB0
                        if ((*TPBA0RDM)["TPBA0RDM0"] == 0){
                            if ((*TPBA0RSF)["TPBA0RSF0"] == 1){
                                mTPBA0CB0 = (unsigned int)(*TPBA0CMP0);
                                re_printf("info","CB0 is reloaded: %d.\n", mTPBA0CB0);
                                (*TPBA0RSF)["TPBA0RSF0"] = 0;
                            }
                            mTPBA0TOLB = (bool)(*TPBA0TOL)["TPBA0TOL0"];
                        }
                        //Reload CB1
                        if ((*TPBA0RSF)["TPBA0RSF1"] == 1){
                            mTPBA0CB1 = (unsigned int)(*TPBA0CMP1)["TPBA0CMP1"];
                            re_printf("info","CB1 is reloaded: %d.\n", mTPBA0CB1);
                            (*TPBA0RSF)["TPBA0RSF1"] = 0;
                            special_flag = true;                    
                        }
                        DumpTransInfo("PAT MATCHING");
                    }
                }
                mPATFlag = (special_flag == false) ? true: false;
            } else {    
                (*TPBA0CNT1)["TPBA0CNT1"] = (unsigned int)(mPointerBuf + 1);//count up CNT1
                //Check to notify PAT interrupt
                //At the moment, TPBA starts working, mPATFlag is FALSE
                if (mTPBA0CB0 != 0){
                    if (mPATFlag){
                        mPATMatching = true;
                        mPATValue = (mPATCounter % 2 == 0)? true: false;
                        mPatCount++;
                        mPATCounter++;
                        mPATFlag = false;

                        //Reload CB0
                        if ((*TPBA0RDM)["TPBA0RDM0"] == 0){
                            if ((*TPBA0RSF)["TPBA0RSF0"] == 1){
                                mTPBA0CB0 = (unsigned int)(*TPBA0CMP0);
                                re_printf("info","CB0 is reloaded: %d.\n", mTPBA0CB0);
                                (*TPBA0RSF)["TPBA0RSF0"] = 0;
                            }
                            mTPBA0TOLB = (bool)(*TPBA0TOL)["TPBA0TOL0"];
                        }
                        //Reload CB1
                        if ((*TPBA0RSF)["TPBA0RSF1"] == 1){
                            mTPBA0CB1 = (unsigned int)(*TPBA0CMP1)["TPBA0CMP1"];
                            re_printf("info","CB1 is reloaded: %d.\n", mTPBA0CB1);
                            (*TPBA0RSF)["TPBA0RSF1"] = 0;
                            if (mTPBA0CB1 == 0){
                                (*TPBA0CNT1)["TPBA0CNT1"] = 0;
                                mPATFlag = true;
                            }
                        }
                        DumpTransInfo("PAT MATCHING");
                    }
                }
            }

            //2.2 Check to not deassert the PRD interrupt for next counting process
            if((mTPBA0CB0 == 0) && (PRS_value == 1)){
                mIsDeassertIntr = false;
            }else{
                mIsDeassertIntr = true;
            }
            //2.3 Assert the DTY, PRD and PAT
            //For DTY
            if((pre_cb2 == pre_cb0) && (!mIsStartCounting)){
                mDTYEvent.notify();
                if((pre_cb2 == pre_cb0) && (pre_cb0 != 0) && (mTPBA0CB2 == 0)){
                    mAssertDTY = false;
                }
            }
            //For PAT and PRD
            mPRDPATEvent.notify(); 
            if(!mIsStartCounting){  
                DumpTransInfo("PERIOD MATCHING");
            }
            if (mTPBA0CB2 >= (mTPBA0CB0 + 1)){
                DumpTransInfo("SINGLE COUNTER COUNTING");
            }else{
                DumpTransInfo("BOTH COUNTERS COUNTING");
            }

            //2.4 For TPBATTOUT signal after 1 counting clock
            if (((*TPBA0TOE)["TPBA0TOE0"] == 1) && (mTPBA0CB2 != 0) && (mIsStartCounting  || (pre_cb2 != pre_cb0))){   
                if(mTPBA0TOLB == true){
                    mWriteTOUTValue = false;
                }else{
                    mWriteTOUTValue = true;
                }
                mWaitingEvent.notify(mClkPeriod * PRS_value, SC_NS);
            } else {
                mTPBA0TOLB = TOLB_temp;    //Update TOUT by software, the reloaded CB0 is ignored. 
            }
            mIsStartCounting = false;

            //2.5 Trigger period counting
            mPeriodCounter->SetCompareMatchValue(mTPBA0CB0 + 1);
            mPeriodCounter->StartStopCounter(true, mClkPeriod, PRS_value);

            //2.6 Tringger duty counting
            if (mTPBA0CB2 >= (mTPBA0CB0 + 1)){
                mDutyCounter->StartStopCounter(false, mClkPeriod, PRS_value); //Stop the DutyCounter
            } else {
                mDutyCounter->SetCompareMatchValue(mTPBA0CB2 + 1);
                mDutyCounter->StartStopCounter(true, mClkPeriod, PRS_value); //Start the DutyCounter
            }
        }
    }
}//}}}

void Ctpba::PRDPATMethod(void)
{//{{{
    bool tmp_prd = TPBATINTPRD.read();
    bool tmp_pat = TPBATINTPAT.read();
    if (mIsReset){
        re_printf("Info","The TPBATINTPRD is reset to false. \n");
        re_printf("Info","The TPBATINTPAT is reset to false. \n");
        TPBATINTPRD.write(false);
        TPBATINTPAT.write(false);
    } else if (mPRDForceStop){
        if(tmp_pat){
            TPBATINTPAT.write(false);
            DumpIntInfo("Deassert", "TPBATINTPAT");
        }
        if(tmp_prd){
            TPBATINTPRD.write(false);
            DumpIntInfo("Deassert", "TPBATINTPRD");
        }
        mPRDForceStop = false;
    } else {
        //1. Deassert the PRD output port
        if ((mPRDValue == mPrePRDValue) && mIsDeassertIntr) {
            TPBATINTPRD.write(false);
            DumpIntInfo("Deassert", "TPBATINTPRD");
        }
        //2.Assert the PRD output port 
        if (mPRDValue != mPrePRDValue){
            mPrePRDValue = mPRDValue;
            if(!tmp_prd){
                DumpIntInfo("Assert", "TPBATINTPRD");
                TPBATINTPRD.write(true);
            }    
            if(mIsDeassertIntr){
                mPRDPATEvent.notify(mClkPeriod, SC_NS);
            }
        }
        //3. Deassert PAT output port 
        if((mPATValue == mPrePATValue) && mIsDeassertIntr && mPATMatching){
            TPBATINTPAT.write(false);
            DumpIntInfo("Deassert", "TPBATINTPAT");
        }
        //4.Assert PAT output port
        if (mPATValue != mPrePATValue) {
            mPrePATValue = mPATValue;
            if(!tmp_pat){
                DumpIntInfo("Assert", "TPBATINTPAT");
                TPBATINTPAT.write(true);
                
            }
            if(mIsDeassertIntr){
                 mPRDPATEvent.notify(mClkPeriod, SC_NS);
            }
        }
    }

}//}}}

void Ctpba::DTYMethod(void)
{//{{{
    bool tmp_dty = TPBATINTDTY.read();
    if(mIsReset){
        re_printf("Info","The TPBATINTDTY is reset to false. \n");
        TPBATINTDTY.write(false);
    } else if (mDTYForceStop){
        if(tmp_dty){
            TPBATINTDTY.write(false);
            DumpIntInfo("Deassert", "TPBATINTDTY");
        }
        mDTYForceStop = false;
    }else{
        //1. Deassert the DTY
        if ((mDTYValue == mPreDTYValue) && mIsDeassertIntr) {
            TPBATINTDTY.write(false);
            DumpIntInfo("Deassert", "TPBATINTDTY");
        }
        //2. Assert the DTY 
        if (mDTYValue != mPreDTYValue) {
            mPreDTYValue = mDTYValue;
            if(!tmp_dty){
                DumpIntInfo("Assert", "TPBATINTDTY");
                TPBATINTDTY.write(true);
            }
            if(mIsDeassertIntr){
                mDTYEvent.notify(mClkPeriod, SC_NS);
            }
        }
    }
}//}}}

void Ctpba::WaitingMethod(void)
{//{{{
    mWriteTOUTByPRD = true;
    mWriteTOUTEvent.notify();
}//}}}

void Ctpba::WriteTOUTMethod(void)
{//{{{
    bool TOUT_temp = false;
    if (mIsReset){                                       
        re_printf("info","The TPBATTOUT is reset to false. \n");
    }else{
        if ((*TPBA0TE)["TPBA0TE0"] == 0){                          //1. TPBA is not counting
            TOUT_temp = mWriteTOUTValue;                         
        }else{                                                     //2. TOUT is updated by software or not
            if ((*TPBA0TOE)["TPBA0TOE0"] == 0){                    //   2.1 TBPA0TOE = 0: Updated by software 
                TOUT_temp = mWriteTOUTValue;                       
            }else{                                                 //   2.2 TPBA0TOE = 1: Update by matching counting
                TOUT_temp = mWriteTOUTValue;                                      
                (*TPBA0TO)["TPBA0TO0"] = (unsigned int)TOUT_temp;
            }
        }
    } 
    TPBATTOUT.write(TOUT_temp);
}//}}}

void Ctpba::DumpInfo(const char *type, const char *message, ...)
{//{{{
    if ((message != NULL) && (type != NULL)) {
        printf ("PROFILE(%s): TPBA: ", type);
        //print body
        va_list argptr;
        va_start(argptr, message);
        vprintf(message, argptr);
        va_end(argptr);
    }
}//}}}

void Ctpba::DumpStatInfo()
{//{{{
    const char *type="StatInfo";
    std::string onoff_status = "";
    std::string working_status = SelectStatus(mStatus);

    if ((*TPBA0TE)["TPBA0TE0"] == 1){
        onoff_status = "Enable";
    } else {
        onoff_status = "Disable";
    }
    (*TPBA0CNT0) = mPeriodCounter->GetCNTValue();

    DumpInfo(type, "Info [%20s] (%10s):\n", sc_time_stamp().to_string().c_str(), this->name());
    
    DumpInfo(type, "TPBA0CNT0: %6d \n", (unsigned int)(*TPBA0CNT0));
    DumpInfo(type, "TPBA0CNT1: %6d \n", (unsigned int)(*TPBA0CNT1)["TPBA0CNT1"]);
    DumpInfo(type, "TPBA0CMP0: %6d \n", (unsigned int)(*TPBA0CMP0));
    DumpInfo(type, "TPBA0CMP1: %6d \n", (unsigned int)(*TPBA0CMP1)["TPBA0CMP1"]);
    if ((*TPBA0CTL)["TPBA0DPS"] == 0){
        DumpInfo(type, "TPBA0BUF: %d: %6d \n", mPointerBuf, (unsigned int)(*TPBA0BUF[mPointerBuf]));
    } else {
        if ((unsigned int)(*TPBA0CNT1)["TPBA0CNT1"] % 2 == 0){
            DumpInfo(type, "TPBA0BUF: %d (8 high bits): %3d \n", (mPointerBuf/2), (unsigned int)((*TPBA0BUF[(mPointerBuf/2)]) & 0xFF));
        } else {
            DumpInfo(type, "TPBA0BUF: %d (8 low bits): %3d \n", (mPointerBuf/2), (unsigned int)((*TPBA0BUF[(mPointerBuf/2)]) >> 8));
        }
    }
    DumpInfo(type, "TPBA0CB2: %6d \n", mTPBA0CB2);
    DumpInfo(type, "TPBATINTPRD: %d times \n", mPrdCount);
    mPrdCount = 0;
    DumpInfo(type, "TPBATINTDTY: %d times \n", mDtyCount);
    mDtyCount = 0;
    DumpInfo(type, "TPBATINTPAT: %d times \n", mPatCount);
    mPatCount = 0;
    DumpInfo(type, "STATUS: %20s, WORKING STATUS: %30s \n", onoff_status.c_str(), working_status.c_str());
}//}}}

void Ctpba::DumpTransInfo(std::string status)
{//{{{
    const char *type="TransInfo";
    (*TPBA0CNT0) = mPeriodCounter->GetCNTValue();
    if (EnableTransInfo == true){
        DumpInfo(type, "Info [%20s] (%10s): TPBA0CNT0: %6d, %30s.\n", sc_time_stamp().to_string().c_str(), this->name(), (unsigned int)(*TPBA0CNT0), status.c_str());
    }
}//}}}

void Ctpba::DumpIntInfo(std::string operation, std::string intr_name)
{//{{{
    if (DumpInterrupt){
        re_printf("info", "INT [TPBA: %s] %s.\n", intr_name.c_str(), operation.c_str());
    }
}//}}}

std::string Ctpba::SelectStatus (const unsigned int status)
{//{{{
    std::string working_status = "";
    switch (status) {
        case emIdle:
                working_status = "IDLE";
                break;
        case emCounting:
                working_status = "COUNTING";
                break;
        case emReset:
                working_status = "RESETING";
                break;
        default:
                break;
    }
    return working_status;
}//}}}

void Ctpba::AssertReset(double wait_time, double period_time)
{//{{{
    if ((mIsReset == false) && (mCmdIfRstFlag == false)){
        mCmdIfRstFlag = true;
        re_printf("info","TPBA will reset for %lld.%06d ns afer %lld.%06d ns.\n", (unsigned long long) period_time, FRACTION_TO_INT(period_time), (unsigned long long) wait_time, FRACTION_TO_INT(wait_time));
        mPeriodTime = period_time;
        mAssertResetEvent.notify(wait_time, SC_NS);
    } else {
        re_printf("warning","Reset is in progress.\n");
    }
}//}}}

//when AssertReset is called, mAssertResetEvent is notified to call TempAssertResetMethod after wait time
void Ctpba::AssertResetMethod(void)
{//{{{
    this->EnableReset(mCmdIfRstFlag);
    mCancelAssertResetEvent.notify((unsigned int)(mPeriodTime), SC_NS);
}//}}}

void Ctpba::ResetMethod(void)
{//{{{
    bool is_cancel_assertreset = true;
    bool IsPortRst = false;
    if(preset_n.read() == vpcl::emResetActive){
        IsPortRst = true;
    }else{
        IsPortRst = false;
    }
    if(IsPortRst){
        mIsInitialize = false;
        re_printf("info","Reset signal is asserted.\n");
        this->EnableReset(IsPortRst);
        is_cancel_assertreset = true;
    }else{
        if(mIsInitialize){
            mIsInitialize = false;
            is_cancel_assertreset = false;
        }else{
            re_printf("info","Reset signal is negated.\n");
            this->EnableReset(IsPortRst);
            is_cancel_assertreset = true;
        }
    }
    //Cancel AssertReset
    if(is_cancel_assertreset){
        mAssertResetEvent.cancel();
        mCancelAssertResetEvent.cancel();
        mCmdIfRstFlag = false;
    }    
}//}}}

void Ctpba::ReInitialize(const bool is_constructor)
{//{{{
    mIsStartCounting   = false;
    mPRDForceStop      = false;
    mDTYForceStop      = false;
    mPATMatching       = false;
    mIsDeassertIntr    = true;
    mAssertDTY         = true;

    mPrdCount = 0;
    mDtyCount = 0;
    mPatCount = 0;
    mTPBA0CB0 = 0;
    mTPBA0CB1 = 0;
    mTPBA0CB2 = 0;
    mTPBA0TOLB = false;
    mWriteTOUTValue = false; 
    mPointerBuf = 0;
    mPATFlag = false;
    mPATCounter = 0; 
    mTSSTFlag = false;
    mPRDValue = false;
    mPATValue = false;
    mDTYValue = false;
    mPrePRDValue = false;
    mPrePATValue = false;
    mPreDTYValue = false;
    mClkPeriod = 0;
    mStopTime = 0;
    if(is_constructor){
        TPBATINTPRD.initialize(0x0);
        TPBATINTDTY.initialize(0x0);
        TPBATINTPAT.initialize(0x0);
        TPBATTOUT.initialize(0x0);
    }else{
        mPRDPATEvent.notify();
        mDTYEvent.notify();
        mWriteTOUTEvent.notify();
    }
}//}}}

void Ctpba::EnableReset(const bool is_active)
{//{{{
    if (is_active){
        mIsReset = true;
        mStatus = emReset;
        DumpTransInfo("RESET");
        mTSEvent.cancel();
        mTTEvent.cancel();
        mTSSTEvent.cancel();
        mWaitingEvent.cancel();
        mWriteTOUTEvent.cancel();
        mWritePRDEvent.cancel();
        mPeriodCounter->Initialize();
        mDutyCounter->Initialize();
        Ctpba_regif::EnableReset(is_active);
        ReInitialize();
    } else {
        mIsReset = false;
        mStatus = emIdle;
        Ctpba_regif::EnableReset(is_active);
        DumpTransInfo("IDLE"); 
        re_printf("info","Reset signal is negated.\n");
    }
    if(mCmdIfRstFlag == false){
        this->tgt_enable_reset(is_active);}
    }

void Ctpba::CancelAssertResetMethod(void)
{//{{{
    mCmdIfRstFlag = false;
    mPeriodTime = 0;
    this->EnableReset(mCmdIfRstFlag);
}//}}}

void Ctpba::UpdateRegisters(void)
{//{{{
    (*TPBA0CNT0) = ((mPeriodCounter->GetCNTValue()) & 0xFFFF); 
}//}}}

void Ctpba::cb_TPBA0CMP1_TPBA0CMP1(RegCBstr str)
{//{{{
    if ((*TPBA0CTL)["TPBA0DPS"] == 0 && str.data >= 0x40){
        re_printf("warning","This setting value must be smaller than 64 when TPBA is working in 16 bit x 64 pattern mode.\n");
    }
}//}}}

void Ctpba::cb_TPBA0CNT0_TPBA0CNT0(RegCBstr str)
{//{{{
    UpdateRegisters();
}//}}}

void Ctpba::cbDBG_TPBA0CNT0_TPBA0CNT0(RegCBstr str)
{//{{{
    UpdateRegisters();
}//}}}

void Ctpba::cb_TPBA0RDT_TPBA0RDT0(RegCBstr str)
{//{{{
    if ((unsigned int)(str.data & 0x1) == 1){
        (*TPBA0RSF)["TPBA0RSF0"] = 1;
    }
    if ((unsigned int)(str.data >> 1) == 1){
        (*TPBA0RSF)["TPBA0RSF1"] = 1;
    }
    (*TPBA0RDT) = 0;
}//}}}

void Ctpba::cb_TPBA0TO_TPBA0TO0(RegCBstr str) //Only written when TPBA0TOE_TPBA0TOE0 == 0 --> software value
{//{{{
    mWriteTOUTValue = (bool)(str.data & 0x1);
    mWriteTOUTEvent.notify();   
}//}}}

void Ctpba::cb_TPBA0TOE_TPBA0TOE0(RegCBstr str)  //Only written when TPBA0TE = 0 --> Disable counting 
{//{{{
    if ((str.data & 0x1) == 0){  //For software
        mWriteTOUTValue = (bool)(*TPBA0TO)["TPBA0TO0"];
        mWriteTOUTEvent.notify();
    } else { //For hardware
        if ((*TPBA0TE)["TPBA0TE0"] == 0){        // Disable counting 
            if ((*TPBA0TOL)["TPBA0TOL0"] == 0){     
                mWriteTOUTValue = false;
                mWriteTOUTEvent.notify();
                (*TPBA0TO)["TPBA0TO0"] = 0;
            } else {                                
                mWriteTOUTValue = true;
                mWriteTOUTEvent.notify();
                (*TPBA0TO)["TPBA0TO0"] = 1;
            }
        }
    }
}//}}}

void Ctpba::cb_TPBA0TS_TPBA0TS0(RegCBstr str)
{//{{{
    if (mIsReset == false){
        if (freq_PCLK != 0){
            mClkPeriod = (double)((1/freq_PCLK)*emNanoSecond);
            if (str.data == 1){
                mTSEvent.notify(mClkPeriod, SC_NS);
            }
        } else {
            re_printf("error","pclk must be different from 0.\n");
            (*TPBA0TS)["TPBA0TS0"] = 0;
        }    
    }
}//}}}

void Ctpba::cb_TPBA0TT_TPBA0TT0(RegCBstr str)
{//{{{
    if ((*TPBA0TE)["TPBA0TE0"] == 0){
        if (freq_PCLK == 0){
            re_printf("error","pclk must be different from 0.\n");  
        } else {
            mClkPeriod = (double)((1/freq_PCLK)*emNanoSecond);
        }
    } 
    if (str.data == 1){
        mTTEvent.notify(mClkPeriod, SC_NS);           
    }
}//}}}

void Ctpba::SetLatency_TLM(const double pclk_period, const bool is_constructor)
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

void Ctpba::SetCLKfreq(std::string clk_name, double clk_freq)
{//{{{  
    if (clk_name == "pclk") {
        // Check update condition   
        if ((mStatus != emIdle)&&(mStatus != emReset)) {
            re_printf("warning","Changing the frequency of pclk clock is not affected when TPBA model is operating.\n");
        } else if (clk_freq <= 0){
             re_printf("error","The pclk frequency must be greater than 0.\n");
        } else {
            //1. Check time unit
            double pclk_period  = (double)(emNanoSecond / clk_freq);
            double time_unit    = (double)(emNanoSecond / GetTimeResolution());
            if(time_unit > pclk_period){
                re_printf("warning","The pclk period is less than 1 unit time of system.\n");
            //3. Update the new pclk    
            } else {
                freq_PCLK = clk_freq;
                re_printf("info","The pclk is set with a frequency as %lld.%06d Hz.\n", (unsigned long long) clk_freq, FRACTION_TO_INT(clk_freq));
                SetLatency_TLM(pclk_period, false);
            }
        }
    } else {
        re_printf("error","Clock name is invalid.\n");
    }
}//}}}

double Ctpba::GetTimeResolution(void)
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

// vim600: set foldmethod=marker :
