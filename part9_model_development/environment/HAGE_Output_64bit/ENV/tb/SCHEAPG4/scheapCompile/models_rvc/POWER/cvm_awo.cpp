// -----------------------------------------------------------------------------
// $Id: cvm_awo.cpp,v 1.11 2013/03/21 02:10:44 ducduong Exp $
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


#include "cvm_awo.h"

Ccvm_awo::Ccvm_awo(sc_module_name name): Cgeneral_reset(name)
        //Input ports
        ,diag_mode("diag_mode")
        ,cvmde_wen("cvmde_wen")
        ,cvmdmask1_wen("cvmdmask1_wen")
        ,cvmdmask2_wen("cvmdmask2_wen")
        ,cvmtrim0_in("cvmtrim0_in")
        ,cvmtrim1_in("cvmtrim1_in")
        ,cvmtrim2_in("cvmtrim2_in")
        ,cvmtrim3_in("cvmtrim3_in")
        ,cvmtrim4_in("cvmtrim4_in")
        ,hold_cvmtrim0("hold_cvmtrim0")
        ,hold_cvmtrim1("hold_cvmtrim1")
        ,hold_cvmtrim2("hold_cvmtrim2")
        ,hold_cvmtrim3("hold_cvmtrim3")
        ,hold_cvmtrim4("hold_cvmtrim4")
        ,clk("clk")
        ,clk_roscl("clk_roscl")
        ,moscen("moscen")
        ,pllen("pllen")
        ,cpuclks0("cpuclks0")
        ,cpuclks1("cpuclks1")
        ,cpuclks2("cpuclks2")
        ,pocdet_n("pocdet_n")
        ,resz12("resz12")
        ,reset_fct("reset_fct")
        ,mstprqz("mstprqz")
        ,stop("stop")
        ,off("off")
        ,cyclic("cyclic")
        ,isolate("isolate")
        ,vccholdtrg("vccholdtrg")
        ,haltst("haltst")
        ,pdcvm2bgr_t("pdcvm2bgr_t")
        ,pdcvm2bufo_t("pdcvm2bufo_t")
        ,errcvm2isoh_t("errcvm2isoh_t")
        ,errcvm2isol_t("errcvm2isol_t")
        ,errcvm2awoh_t("errcvm2awoh_t")
        ,errcvm2awol_t("errcvm2awol_t")
        ,tapkeep("tapkeep")
        ,cvm2tmsk("cvm2tmsk")
        //Output ports
        ,pdcvm2bgr("pdcvm2bgr")
        ,pdcvm2bufo("pdcvm2bufo")
        ,errcvm2isoh("errcvm2isoh")
        ,errcvm2isol("errcvm2isol")
        ,errcvm2awoh("errcvm2awoh")
        ,errcvm2awol("errcvm2awol")
        ,cvmtrim0("cvmtrim0")
        ,cvmtrim1("cvmtrim1")
        ,cvmtrim2("cvmtrim2")
        ,cvmtrim3("cvmtrim3")
        ,cvmtrim4("cvmtrim4")
        ,cvm_disable("cvm_disable")
        ,cvm_latchen("cvm_latchen")
        ,cvmde_latchen("cvmde_latchen")
        ,cvmdmask1_latchen("cvmdmask1_latchen")
        ,cvmdmask2_latchen("cvmdmask2_latchen")
        ,cvmtrim_latchen("cvmtrim_latchen")
        ,cvmen_select("cvmen_select")
        ,cvmmsk_cold("cvmmsk_cold")
        ,mode_trans("mode_trans")
        ,test_cvm2mask("test_cvm2mask")
{//{{{
    CommandInit(this->name());
    Initialize();
    mCLKPeriod = 0;
    mCLKROSCLPeriod = 0;
    mResetFlag = false;
    mPocdetRstFlag = true;
    mOff_rise_ed = 0;
    mResreq_rise_ed = 0;
    mPreLatchen = 1;
    for(unsigned int i=0; i<emOffUpdateCycle; i++){
        mOffArr[i] = 0;
        mReset_fctArr[i] = 0;
        mCvmen_selectArr[i] = 0;
        mOff_maskArr[i] = 0;
    }
    for(unsigned int i=0; i<emCvmmsk_ColdUpdateCycle; i++){
        mCvmmsk_coldArr[i] = 0;
    }
    for(unsigned int i=0; i<emLatchenUpdateCycle; i++){
        mCvm_latchenArr[i] = 0;
    }
    for(unsigned int i=0; i<emStop_maskUpdateCycle; i++){
        mStopArr[i] = 1;
    }
    mDFF4Off_mask = 1;
    mDFF7Halt_mask = 0;
    mDFF7Moscen_mask = 0;
    mDFF7Pllen_mask = 0;
    mDFF7Cpuclks0_mask = 0;
    mDFF7Cpuclks1_mask = 0;
    mDFF7Cpuclks2_mask = 0;
    mHaltstQueue.clear();
    mMoscenQueue.clear();
    mPllenQueue.clear();
    mCpuclks0Queue.clear();
    mCpuclks1Queue.clear();
    mCpuclks2Queue.clear();
    mHaltFlag = false;
    mMoscenFlag = false;
    mPllenFlag = false;
    mCpuclks0Flag = false;
    mCpuclks1Flag = false;
    mCpuclks2Flag = false;
    mStop = 1;

    pdcvm2bgr.initialize(0);
    pdcvm2bufo.initialize(0);
    errcvm2isoh.initialize(0);
    errcvm2isol.initialize(0);
    errcvm2awoh.initialize(0);
    errcvm2awol.initialize(0);
    cvmtrim0.initialize(0);
    cvmtrim1.initialize(0);
    cvmtrim2.initialize(0);
    cvmtrim3.initialize(0);
    cvmtrim4.initialize(0);
    cvm_disable.initialize(0);
    cvm_latchen.initialize(1);
    cvmde_latchen.initialize(1);
    cvmdmask1_latchen.initialize(1);
    cvmdmask2_latchen.initialize(1);
    cvmtrim_latchen.initialize(1);
    cvmen_select.initialize(0);
    cvmmsk_cold.initialize(1);
    mode_trans.initialize(1);
    test_cvm2mask.initialize(0);

    SC_METHOD(ErrCvmtrimMethod);
    dont_initialize();
    sensitive << hold_cvmtrim0;
    sensitive << hold_cvmtrim1;
    sensitive << hold_cvmtrim2;
    sensitive << hold_cvmtrim3;
    sensitive << hold_cvmtrim4;
    sensitive << diag_mode;
    sensitive << mResetEvent;

    SC_METHOD(Pocdet_nMethod);
    dont_initialize();
    sensitive << pocdet_n;

    SC_METHOD(ClkMethod);
    dont_initialize();
    sensitive << clk;
    sensitive << mReadPortEvent;

    SC_THREAD(OffThread);
    dont_initialize();
    sensitive << off;
    sensitive << mPocdetRstEvent;
    sensitive << mResetEvent;
    sensitive << mReadPortEvent;

    SC_THREAD(Reset_fctThread);
    dont_initialize();
    sensitive << reset_fct;
    sensitive << mPocdetRstEvent;
    sensitive << mResetEvent;
    sensitive << mReadPortEvent;

    SC_THREAD(LatchenCvmen_selectCvmmsk_coldOff_maskThread);
    dont_initialize();
    sensitive << vccholdtrg;
    sensitive << mPocdetRstEvent;
    sensitive << mSynResetEvent;
    sensitive << mResetEvent;
    sensitive << mReadPortEvent;

    SC_METHOD(LatchenMethod);
    dont_initialize();
    sensitive << mPocdetRstEvent;
    sensitive << mResetEvent;
    sensitive << cvmde_wen;
    sensitive << cvmdmask1_wen;
    sensitive << cvmdmask2_wen;
    sensitive << mLatchenEvent;
    sensitive << mReadPortEvent;

    SC_METHOD(HaltstMethod);
    dont_initialize();
    sensitive << haltst;
    sensitive << mReadPortEvent;

    SC_METHOD(Halt_maskMethod);
    dont_initialize();
    sensitive << mHalt_mask7ClockEvent;
    sensitive << mHalt_maskEvent;
    sensitive << mPocdetRstEvent;
    sensitive << mResetEvent;
    sensitive << mReadPortEvent;

    SC_METHOD(MoscenMethod);
    dont_initialize();
    sensitive << moscen;
    sensitive << mReadPortEvent;

    SC_METHOD(Moscen_maskMethod);
    dont_initialize();
    sensitive << mMoscen_mask7ClockEvent;
    sensitive << mMoscen_maskEvent;
    sensitive << mPocdetRstEvent;
    sensitive << mResetEvent;
    sensitive << mReadPortEvent;

    SC_METHOD(PllenMethod);
    dont_initialize();
    sensitive << pllen;
    sensitive << mReadPortEvent;

    SC_METHOD(Pllen_maskMethod);
    dont_initialize();
    sensitive << mPllen_mask7ClockEvent;
    sensitive << mPllen_maskEvent;
    sensitive << mPocdetRstEvent;
    sensitive << mResetEvent;
    sensitive << mReadPortEvent;

    SC_METHOD(Cpuclks0Method);
    dont_initialize();
    sensitive << cpuclks0;
    sensitive << mReadPortEvent;

    SC_METHOD(Cpuclks0_maskMethod);
    dont_initialize();
    sensitive << mCpuclks0_mask7ClockEvent;
    sensitive << mCpuclks0_maskEvent;
    sensitive << mPocdetRstEvent;
    sensitive << mResetEvent;
    sensitive << mReadPortEvent;

    SC_METHOD(Cpuclks1Method);
    dont_initialize();
    sensitive << cpuclks1;
    sensitive << mReadPortEvent;

    SC_METHOD(Cpuclks1_maskMethod);
    dont_initialize();
    sensitive << mCpuclks1_mask7ClockEvent;
    sensitive << mCpuclks1_maskEvent;
    sensitive << mPocdetRstEvent;
    sensitive << mResetEvent;
    sensitive << mReadPortEvent;

    SC_METHOD(Cpuclks2Method);
    dont_initialize();
    sensitive << cpuclks2;
    sensitive << mReadPortEvent;

    SC_METHOD(Cpuclks2_maskMethod);
    dont_initialize();
    sensitive << mCpuclks2_mask7ClockEvent;
    sensitive << mCpuclks2_maskEvent;
    sensitive << mPocdetRstEvent;
    sensitive << mResetEvent;
    sensitive << mReadPortEvent;

    SC_THREAD(StopThread);
    dont_initialize();
    sensitive << stop;
    sensitive << mCvmen_selectRstEvent;
    sensitive << mReadPortEvent;
    sensitive << mResetEvent;
    
    SC_METHOD(Stop_maskMethod);
    dont_initialize();
    sensitive << mstprqz;
    sensitive << mStop_maskEvent;
    sensitive << mReadPortEvent;

    SC_METHOD(Mode_transMethod);
    dont_initialize();
    sensitive << mMode_transEvent;
    sensitive << mResetEvent;

    SC_METHOD(Cvm_disableMethod);
    dont_initialize();
    sensitive << stop;
    sensitive << off;
    sensitive << cyclic;
    sensitive << mResetEvent;

    SC_METHOD(Clk_rosclMethod);
    dont_initialize();
    sensitive << clk_roscl;
    sensitive << mReadPortEvent;
}//}}}

Ccvm_awo::~Ccvm_awo()
{//{{{
}//}}}

void Ccvm_awo::Initialize()
{//{{{
    mLastAccessHaltst = 0;
    mLastAccessMoscen = 0;
    mLastAccessPllen = 0;
    mLastAccessCpuclks0 = 0;
    mLastAccessCpuclks1 = 0;
    mLastAccessCpuclks2 = 0;
    mHalt_mask = 0;
    mMoscen_mask = 0;
    mPllen_mask = 0;
    mCpuclks0_mask = 0;
    mCpuclks1_mask = 0;
    mCpuclks2_mask = 0;
    mStop_mask = 0;
    mCvmen_selectRstFlag = false;
}//}}}

double Ccvm_awo::CalculateRisingEdgeTime(double period)
{//{{{
    double cur_time = sc_time_stamp().to_double();
    unsigned int index = (unsigned int)(cur_time / period) + 1;
    return period*index - cur_time;
}//}}}

void Ccvm_awo::ErrCvmtrimMethod()
{//{{{
    if(Cgeneral_reset::IsResetPeriod()){
        errcvm2isoh.write(0);
        errcvm2isol.write(0);
        errcvm2awoh.write(0);
        errcvm2awol.write(0);
        cvmtrim0.write(0);
        cvmtrim1.write(0);
        cvmtrim2.write(0);
        cvmtrim3.write(0);
        cvmtrim4.write(0);
    } else {
        errcvm2isoh.write(diag_mode.read());
        errcvm2isol.write(diag_mode.read());
        errcvm2awoh.write(diag_mode.read());
        errcvm2awol.write(diag_mode.read());
        cvmtrim0.write(hold_cvmtrim0.read());
        cvmtrim1.write(hold_cvmtrim1.read());
        cvmtrim2.write(hold_cvmtrim2.read());
        cvmtrim3.write(hold_cvmtrim3.read());
        cvmtrim4.write(hold_cvmtrim4.read());
    }
}//}}}

void Ccvm_awo::Pocdet_nMethod()
{//{{{
    if(Cgeneral_reset::IsResetPeriod()){
        return;
    }
    if(pocdet_n.read() == 0){
        //pocdet_n reset is active
        mPocdetRstFlag = true;
    } else{
        //pocdet_n reset is inactive
        mPocdetRstFlag = false;
    }
    mPocdetRstEvent.notify();
}//}}}

void Ccvm_awo::OffThread()
{//{{{
    for(;;){
        if(mPocdetRstFlag || Cgeneral_reset::IsResetPeriod()){
            //Reset is active
            //Clear all DFF of off to 0
            for(unsigned int i=0; i<emOffUpdateCycle; i++){
                mOffArr[i] = 0;
            }
            mOff_rise_ed = 0;
            wait();
        }
        try{
            while(true){
                if((mCLKPeriod != 0) && (!mPocdetRstFlag) && (!mResetFlag)){
                    bool PreOff = off.read();
                    for(unsigned int RemainUpdateCycle=emOffUpdateCycle; RemainUpdateCycle>0; RemainUpdateCycle--){
                        //Calculate the rising edge time and wait
                        double syn_time = CalculateRisingEdgeTime(mCLKPeriod);
                        wait((unsigned int)syn_time, SC_NS, mPocdetRstEvent | mResetEvent);
                        if(mPocdetRstFlag || mResetFlag){
                            throw vpcl::emRstException;
                        }
                        //Check input port change
                        if(PreOff != off.read()){
                            PreOff = off.read();
                            RemainUpdateCycle = emOffUpdateCycle + 1;
                        }
                        //Shift right value of array
                        for(unsigned int i=emOffUpdateCycle-1; i>0; i--){
                            mOffArr[i] = mOffArr[i-1];
                        }
                        mOffArr[0] = off.read();
                        //Update output
                        mOff_rise_ed = mOffArr[1] & (!(mOffArr[2]));
                        if(mOff_rise_ed == 1){
                            mSynResetEvent.notify();
                        }
                    }
                }
                wait();
                if(mPocdetRstFlag || mResetFlag){
                    throw vpcl::emRstException;
                }
            }
        }
        catch(vpcl::eResetException){
        }
    }
}//}}}

void Ccvm_awo::Reset_fctThread()
{//{{{
    for(;;){
        if(mPocdetRstFlag || Cgeneral_reset::IsResetPeriod()){
            //Reset is active
            //Clear all DFF of Reset_fct to 0
            for(unsigned int i=0; i<emReset_fctUpdateCycle; i++){
                mReset_fctArr[i] = 0;
            }
            mResreq_rise_ed = 0;
            wait();
        }
        try{
            while(true){
                if((mCLKPeriod != 0) && (!mPocdetRstFlag) && (!mResetFlag)){
                    bool PreReset_fct = reset_fct.read();
                    for(unsigned int RemainUpdateCycle=emReset_fctUpdateCycle; RemainUpdateCycle>0; RemainUpdateCycle--){
                        //Calculate the rising edge time and wait
                        double syn_time = CalculateRisingEdgeTime(mCLKPeriod);
                        wait((unsigned int)syn_time, SC_NS, mPocdetRstEvent | mResetEvent);
                        if(mPocdetRstFlag || mResetFlag){
                            throw vpcl::emRstException;
                        }
                        //Check input port change
                        if(PreReset_fct != reset_fct.read()){
                            PreReset_fct = reset_fct.read();
                            RemainUpdateCycle = emReset_fctUpdateCycle + 1;
                        }
                        //Shift right value of array
                        for(unsigned int i=emReset_fctUpdateCycle-1; i>0; i--){
                            mReset_fctArr[i] = mReset_fctArr[i-1];
                        }
                        mReset_fctArr[0] = reset_fct.read();
                        //Update output
                        mResreq_rise_ed = mReset_fctArr[1] & (!(mReset_fctArr[2]));
                        if(mResreq_rise_ed == 1){
                            mSynResetEvent.notify();
                        }
                    }
                }
                wait();
                if(mPocdetRstFlag || mResetFlag){
                    throw vpcl::emRstException;
                }
            }
        }
        catch(vpcl::eResetException){
        }
    }
}//}}}

bool Ccvm_awo::VccholdtrgFunc(bool *arr, unsigned int length, bool isReset, bool &resetFlag, bool initValue, bool curValue)
{//{{{
    bool output = curValue;
    //Update output
    if (resetFlag){
        resetFlag = false;
    }
    if((arr[length-1] == 1) && (resetFlag == false)){
        output = !initValue;
    }
    if(isReset == 1){
        //synchronous reset is active
        output = initValue;
        resetFlag = true;
    }

    //Shift right array
    for(unsigned int i=length-1; i>0; i--){
        arr[i] = arr[i-1];
    }
    arr[0] = vccholdtrg.read();
    return output;
}//}}}

void Ccvm_awo::LatchenCvmen_selectCvmmsk_coldOff_maskThread()
{//{{{
    for(;;){
        if(mPocdetRstFlag || Cgeneral_reset::IsResetPeriod()){
            //Reset is active
            //Reset DFF to initial value
            for(unsigned int i=0; i<emCvmmsk_ColdUpdateCycle; i++){
                mCvmmsk_coldArr[i] = 0;
            }
            for(unsigned int i=0; i<emOffSelectUpdateCycle; i++){
                mCvmen_selectArr[i] = 0;
                mOff_maskArr[i] = 0;
            }
            for(unsigned int i=0; i<emLatchenUpdateCycle; i++){
                mCvm_latchenArr[i] = 0;
            }
            //Reset output to initial value
            mPreLatchen = 1;
            cvm_latchen.write(1);
            cvmtrim_latchen.write(1);
            mLatchenEvent.notify();
            cvmen_select.write(0);
            mCvmen_selectRstEvent.notify();
            cvmmsk_cold.write(1);
            mDFF4Off_mask = 1;
            mMode_transEvent.notify();
            wait();
        }
        try{
            while(true){
                if((mCLKPeriod != 0) && (!mPocdetRstFlag) && (!mResetFlag)){
                    bool Cvmmsk_coldDFF5ResetFlag = false;
                    bool Cvmen_selectDFF4ResetFlag = false;
                    bool Off_maskDFF4ResetFlag = false ;
                    bool Cvm_latchenDFF3ResetFlag = false;
                    bool PreVccholdtrg = vccholdtrg.read();
                    if((mOff_rise_ed == 1) || (mResreq_rise_ed == 1)){
                        cvm_latchen.write(1);
                        cvmtrim_latchen.write(1);
                        mLatchenEvent.notify();
                        cvmen_select.write(0);
                    }
                    if(mOff_rise_ed == 1){
                        mDFF4Off_mask = 1;
                        mMode_transEvent.notify();
                    }
                    if(mResreq_rise_ed == 1){
                        cvmmsk_cold.write(1);
                    }
                    for(unsigned int RemainUpdateCycle=emLatchenSelectColdOffUpdateCycle; RemainUpdateCycle>0; RemainUpdateCycle--){
                        //Calculate the rising edge time and wait
                        double syn_time = CalculateRisingEdgeTime(mCLKPeriod);
                        wait((unsigned int)syn_time, SC_NS, mPocdetRstEvent | mResetEvent);
                        if(mPocdetRstFlag || mResetFlag){
                            throw vpcl::emRstException;
                        }
                        if(PreVccholdtrg != vccholdtrg.read()){
                            PreVccholdtrg = vccholdtrg.read();
                            RemainUpdateCycle = emLatchenSelectColdOffUpdateCycle + 1;
                        }
                        //cvm_latchen operation
                        bool latchen = VccholdtrgFunc(mCvm_latchenArr, emLatchenUpdateCycle, mOff_rise_ed|mResreq_rise_ed, Cvm_latchenDFF3ResetFlag, 1, cvm_latchen.read());
                        cvm_latchen.write(latchen);
                        cvmtrim_latchen.write(latchen);
                        if(mPreLatchen != latchen){
                            mPreLatchen = latchen;
                            //Notify event to update other latchen ports
                            mLatchenEvent.notify();
                        }
                        //cvmmsk_cold operation
                        bool cold = VccholdtrgFunc(mCvmmsk_coldArr, emCvmmsk_ColdUpdateCycle, mResreq_rise_ed, Cvmmsk_coldDFF5ResetFlag, 1, cvmmsk_cold.read());
                        cvmmsk_cold.write(cold);
                        //cvmen_select operation
			bool select = VccholdtrgFunc(mCvmen_selectArr, emOffSelectUpdateCycle, mOff_rise_ed|mResreq_rise_ed, Cvmen_selectDFF4ResetFlag, 0, cvmen_select.read());
                        cvmen_select.write(select);
                        //check reset port of stop_mask.
                        if(select == 0){
                            mCvmen_selectRstFlag = true;
                        } else{
                            mCvmen_selectRstFlag = false;
                        }
                        mCvmen_selectRstEvent.notify();
                        //off_mask operation
                        mDFF4Off_mask = VccholdtrgFunc(mOff_maskArr, emOffSelectUpdateCycle, mOff_rise_ed, Off_maskDFF4ResetFlag, 1, mDFF4Off_mask);
                        //Notify event to update mode_trans port
                        mMode_transEvent.notify();
                    }
                }
                wait();
                if(mPocdetRstFlag || mResetFlag){
                    throw vpcl::emRstException;
                }
            }
        }
        catch(vpcl::eResetException){
        }
    }
}//}}}

void Ccvm_awo::LatchenMethod()
{//{{{
    if(mPocdetRstFlag || Cgeneral_reset::IsResetPeriod()){
        cvmde_latchen.write(1);
        cvmdmask1_latchen.write(1);
        cvmdmask2_latchen.write(1);
    } else {
        cvmde_latchen.write(cvmde_wen.read() | mPreLatchen);
        cvmdmask1_latchen.write(cvmdmask1_wen.read() | mPreLatchen);
        cvmdmask2_latchen.write(cvmdmask2_wen.read() | mPreLatchen);
    }
}//}}}

bool Ccvm_awo::GetValueFromQueue(std::vector<bool> &queue)
{//{{{
    bool value = 0;
    if(queue.size() != 0){
        value = queue.front();
        queue.erase(queue.begin());
    }
    return value;
}//}}}

void Ccvm_awo::MaskFunc(double &lastAccess, std::vector<bool> &queue, bool inputValue, unsigned int choose)
{//{{{
    bool InputChangeIn1Clock = false;
    double syn_time = 0;
    //Check port is changed in 1 period
    if((sc_time_stamp().to_double() - lastAccess) < mCLKROSCLPeriod){
        queue.pop_back();
        queue.push_back(inputValue);
        InputChangeIn1Clock = true;
    } else{
        syn_time = CalculateRisingEdgeTime(mCLKROSCLPeriod);
        queue.push_back(inputValue);
    }
    switch (choose){
        case 0:
            mHaltFlag = true;
            mHalt_maskEvent.notify();
            if(!InputChangeIn1Clock){
                mHalt_mask7ClockEvent.notify((unsigned int)syn_time + 6*mCLKROSCLPeriod, SC_NS);
            }
            break;
        case 1:
            mMoscenFlag = true;
            mMoscen_maskEvent.notify();
            if(!InputChangeIn1Clock){
                mMoscen_mask7ClockEvent.notify((unsigned int)syn_time + 6*mCLKROSCLPeriod, SC_NS);
            }
            break;
        case 2:
            mPllenFlag = true;
            mPllen_maskEvent.notify();
            if(!InputChangeIn1Clock){
                mPllen_mask7ClockEvent.notify((unsigned int)syn_time + 6*mCLKROSCLPeriod, SC_NS);
            }
            break;
        case 3:
            mCpuclks0Flag = true;
            mCpuclks0_maskEvent.notify();
            if(!InputChangeIn1Clock){
                mCpuclks0_mask7ClockEvent.notify((unsigned int)syn_time + 6*mCLKROSCLPeriod, SC_NS);
            }
            break;
        case 4:
            mCpuclks1Flag = true;
            mCpuclks1_maskEvent.notify();
            if(!InputChangeIn1Clock){
                mCpuclks1_mask7ClockEvent.notify((unsigned int)syn_time + 6*mCLKROSCLPeriod, SC_NS);
            }
            break;
        case 5:
            mCpuclks2Flag = true;
            mCpuclks2_maskEvent.notify();
            if(!InputChangeIn1Clock){
                mCpuclks2_mask7ClockEvent.notify((unsigned int)syn_time + 6*mCLKROSCLPeriod, SC_NS);
            }
            break;
        default:
            break;
    }
    //Update access time
    lastAccess = sc_time_stamp().to_double();    
}//}}}

bool Ccvm_awo::ResetPort_maskFunc()
{//{{{
    if(Cgeneral_reset::IsResetPeriod() || mPocdetRstFlag){
        //Reset is active, clear variable and cancel events
        mHalt_mask7ClockEvent.cancel_all();
        mMoscen_mask7ClockEvent.cancel_all();
        mPllen_mask7ClockEvent.cancel_all();
        mCpuclks0_mask7ClockEvent.cancel_all();
        mCpuclks1_mask7ClockEvent.cancel_all();
        mCpuclks2_mask7ClockEvent.cancel_all();
        mHaltstQueue.clear();
        mMoscenQueue.clear();
        mPllenQueue.clear();
        mCpuclks0Queue.clear();
        mCpuclks1Queue.clear();
        mCpuclks2Queue.clear();
        mDFF7Halt_mask = 0;
        mDFF7Moscen_mask = 0;
        mDFF7Pllen_mask = 0;
        mDFF7Cpuclks0_mask = 0;
        mDFF7Cpuclks1_mask = 0;
        mDFF7Cpuclks2_mask = 0;
        mHaltFlag = false;
        mMoscenFlag = false;
        mPllenFlag = false;
        mCpuclks0Flag = false;
        mCpuclks1Flag = false;
        mCpuclks2Flag = false;
        if(mPocdetRstFlag){
            mHalt_mask = (haltst.read() == mDFF7Halt_mask) ? 0 : 1;
            mMoscen_mask = (moscen.read() == mDFF7Moscen_mask) ? 0 : 1;
            mPllen_mask = (pllen.read() == mDFF7Pllen_mask) ? 0 : 1;    
            mCpuclks0_mask = (cpuclks0.read() == mDFF7Cpuclks0_mask) ?  0 : 1;
            mCpuclks1_mask = (cpuclks1.read() == mDFF7Cpuclks1_mask) ?  0 : 1;
            mCpuclks2_mask = (cpuclks2.read() == mDFF7Cpuclks2_mask) ?  0 : 1;
        }
        return true;
    } 
    return false;
}//}}}

void Ccvm_awo::HaltstMethod()
{//{{{
    if(Cgeneral_reset::IsResetPeriod() || mPocdetRstFlag || (mCLKROSCLPeriod == 0)){
        return;
    }
    MaskFunc(mLastAccessHaltst, mHaltstQueue, haltst.read(), 0);
}//}}}

void Ccvm_awo::Halt_maskMethod()
{//{{{
    bool reset = ResetPort_maskFunc();
    if(!reset){
        if(mHaltFlag){
            mHaltFlag = false;
        } else {
            mDFF7Halt_mask = GetValueFromQueue(mHaltstQueue);
        }
        
        mHalt_mask = (haltst.read() == mDFF7Halt_mask) ? 0 : 1;
        mMode_transEvent.notify();
    }
}//}}}

void Ccvm_awo::MoscenMethod()
{//{{{
    if(Cgeneral_reset::IsResetPeriod() || mPocdetRstFlag || (mCLKROSCLPeriod == 0)){
        return;
    }
    MaskFunc(mLastAccessMoscen, mMoscenQueue, moscen.read(), 1);
}//}}}

void Ccvm_awo::Moscen_maskMethod()
{//{{{
    bool reset = ResetPort_maskFunc();
    if(!reset){
        if(mMoscenFlag){
            mMoscenFlag = false;
        }else{
            mDFF7Moscen_mask = GetValueFromQueue(mMoscenQueue);
        }
        mMoscen_mask = (moscen.read() == mDFF7Moscen_mask) ? 0 : 1;
        mMode_transEvent.notify();
    }
}//}}}

void Ccvm_awo::PllenMethod()
{//{{{
    if(Cgeneral_reset::IsResetPeriod() || mPocdetRstFlag || (mCLKROSCLPeriod == 0)){
        return;
    }
    MaskFunc(mLastAccessPllen, mPllenQueue, pllen.read(), 2);
}//}}}

void Ccvm_awo::Pllen_maskMethod()
{//{{{
    bool reset = ResetPort_maskFunc();
    if(!reset){
        if(mPllenFlag){
            mPllenFlag = false;
        }else{
            mDFF7Pllen_mask = GetValueFromQueue(mPllenQueue);
        }
        mPllen_mask = (pllen.read() == mDFF7Pllen_mask) ? 0 : 1;    
        mMode_transEvent.notify();
    }
}//}}}

void Ccvm_awo::Cpuclks0Method()
{//{{{
    if(Cgeneral_reset::IsResetPeriod() || mPocdetRstFlag || (mCLKROSCLPeriod == 0)){
        return;
    }
    MaskFunc(mLastAccessCpuclks0, mCpuclks0Queue, cpuclks0.read(), 3);
}//}}}

void Ccvm_awo::Cpuclks0_maskMethod()
{//{{{
    bool reset = ResetPort_maskFunc();
    if(!reset){
        if(mCpuclks0Flag){
            mCpuclks0Flag = false;
        }else{
            mDFF7Cpuclks0_mask = GetValueFromQueue(mCpuclks0Queue);
        }
        mCpuclks0_mask = (cpuclks0.read() == mDFF7Cpuclks0_mask) ?  0 : 1;
        mMode_transEvent.notify();
    }
}//}}}

void Ccvm_awo::Cpuclks1Method()
{//{{{
    if(Cgeneral_reset::IsResetPeriod() || mPocdetRstFlag || (mCLKROSCLPeriod == 0)){
        return;
    }
    MaskFunc(mLastAccessCpuclks1, mCpuclks1Queue, cpuclks1.read(), 4);
}//}}}

void Ccvm_awo::Cpuclks1_maskMethod()
{//{{{
    bool reset = ResetPort_maskFunc();
    if(!reset){
        if(mCpuclks1Flag){
            mCpuclks1Flag = false;
        }else{
            mDFF7Cpuclks1_mask = GetValueFromQueue(mCpuclks1Queue);
        }
        mCpuclks1_mask = (cpuclks1.read() == mDFF7Cpuclks1_mask) ? 0 : 1;    
        mMode_transEvent.notify();
    }
}//}}}

void Ccvm_awo::Cpuclks2Method()
{//{{{
    if(Cgeneral_reset::IsResetPeriod() || mPocdetRstFlag || (mCLKROSCLPeriod == 0)){
        return;
    }
    MaskFunc(mLastAccessCpuclks2, mCpuclks2Queue, cpuclks2.read(), 5);
}//}}}

void Ccvm_awo::Cpuclks2_maskMethod()
{//{{{
    bool reset = ResetPort_maskFunc();
    if(!reset){
        if(mCpuclks2Flag){
            mCpuclks2Flag = false;
        }else{
            mDFF7Cpuclks2_mask = GetValueFromQueue(mCpuclks2Queue);
        }
        mCpuclks2_mask = (cpuclks2.read() == mDFF7Cpuclks2_mask) ? 0 : 1;
        mMode_transEvent.notify();
    }
}//}}}

void Ccvm_awo::StopThread()
{//{{{
    for(;;){
        if(mCvmen_selectRstFlag || Cgeneral_reset::IsResetPeriod()){
            //Reset is active
            for(unsigned int i=0; i<emStop_maskUpdateCycle; i++){
                mStopArr[i] = 1;
            }
            mStop = 1;
            mStop_maskEvent.notify();
            wait();
        }
        try{
            while(true){
                if((mCLKROSCLPeriod != 0)  && (!mCvmen_selectRstFlag) && (!mResetFlag)){
                    bool PreStop = stop.read();
                    for(unsigned int RemainUpdateCycle=emStop_maskUpdateCycle; RemainUpdateCycle>0; RemainUpdateCycle--){
                        double syn_time = CalculateRisingEdgeTime(mCLKROSCLPeriod);
                        wait((unsigned int)syn_time, SC_NS, mResetEvent);
                        if(mCvmen_selectRstFlag || mResetFlag){
                            throw vpcl::emRstException;
                        }
                        if(PreStop != stop.read()){
                            PreStop = stop.read();
                            RemainUpdateCycle = emStop_maskUpdateCycle + 1;
                        }
                        for(unsigned int i=emStop_maskUpdateCycle-1; i>0; i--){
                            mStopArr[i] = mStopArr[i-1];
                        }
                        mStopArr[0] = stop.read();
                        unsigned int count1 = 0;
                        for(unsigned int i=2; i<emStop_maskUpdateCycle; i++){
                            if(mStopArr[i] == 1){
                                count1++;
                            }
                        }
                        if(count1 == (emStop_maskUpdateCycle -2)){
                            mStop = 1;
                        } else{
                            mStop = 0;
                        }
                        mStop_maskEvent.notify();
                    }
                }
                wait();
                if(mCvmen_selectRstFlag || mResetFlag){
                    throw vpcl::emRstException;
                }
            }
        }
        catch(vpcl::eResetException){
        }
    }    
}//}}}

void Ccvm_awo::Stop_maskMethod()
{//{{{
    mStop_mask = !(mStop & mstprqz.read());
    mMode_transEvent.notify();
}//}}}

void Ccvm_awo::Mode_transMethod()
{//{{{
    if(Cgeneral_reset::IsResetPeriod()){
        mode_trans.write(1);
    } else {
        mode_trans.write(mDFF4Off_mask | mStop_mask | mHalt_mask | mMoscen_mask | mPllen_mask | mCpuclks0_mask | mCpuclks1_mask | mCpuclks2_mask);
    }
}//}}}

void Ccvm_awo::Cvm_disableMethod()
{//{{{
    if(Cgeneral_reset::IsResetPeriod()){
        cvm_disable.write(0);
    } else {
        cvm_disable.write(stop.read() | off.read() | cyclic.read());
    }
}//}}}

void Ccvm_awo::ClkMethod()
{//{{{
    if(Cgeneral_reset::IsResetPeriod()){
        return;
    }
    double clk_freq = sc_dt::uint64_to_double(clk.read());
    SetCLKfreq("clk",clk_freq);
}//}}}

void Ccvm_awo::Clk_rosclMethod()
{//{{{
    if(Cgeneral_reset::IsResetPeriod()){
        return;
    }
    double clk_freq = sc_dt::uint64_to_double(clk_roscl.read());
    SetCLKfreq("clk_roscl",clk_freq);
}//}}}

void Ccvm_awo::SetCLKfreq(const std::string clk_name, const double clk_freq)
{//{{{
    if (clk_name == "clk" || clk_name == "clk_roscl") {
        if (clk_freq > 0) {
            // Calculate the clock period to check the resolution
            double clk_period = (double)(emNanoSecond / clk_freq);
            double time_unit = (double)(emNanoSecond / Cgeneral_reset::GetTimeResolution());
            if(time_unit > clk_period){
                re_printf("warning","The %s period is less than 1 unit time of system.\n", clk_name.c_str());
                return;
            }
            if(clk_name == "clk"){
                mCLKPeriod = clk_period;
                sc_time clock_value(mCLKPeriod, SC_NS);
            } else{
                mCLKROSCLPeriod = clk_period;
                sc_time clock_value(mCLKROSCLPeriod, SC_NS);
            }
            re_printf("info","%s is set with a frequency as %f.\n",clk_name.c_str(), clk_freq);
        } else {
            re_printf("error","Clock %s = %f is invalid.\n", clk_name.c_str(), clk_freq);
        }
    } else {
        re_printf("warning","Clock name = %s is invalid.\n", clk_name.c_str());
    }
}//}}}

void Ccvm_awo::AssertReset(const double delay, const double period)
{//{{{
    Cgeneral_reset::Com_AssertReset(delay, period);
}//}}}

void Ccvm_awo::EnableReset(const bool is_active)
{//{{{
    if(is_active){
        Initialize();
        mResetFlag = true;
        mResetEvent.notify();
    } else{
        mResetFlag = false;
        mReadPortEvent.notify();
    }
}//}}}

// vim600: set foldmethod=marker :
