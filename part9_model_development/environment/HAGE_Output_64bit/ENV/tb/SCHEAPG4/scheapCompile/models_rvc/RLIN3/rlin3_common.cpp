// ---------------------------------------------------------------------
// $Id: $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include <string>
#include <cstdarg>
#include "rlin3_common.h"
#include "re_define.h"


Crlin3_common::Crlin3_common(sc_module_name name):
    sc_module(name)
{//{{{   
    Initialize();

    SC_METHOD(TransmitDataMethod)
    dont_initialize();
    sensitive << mTransmitEvent;

    SC_METHOD(WaitToBitErrorMethod)
    dont_initialize();
    sensitive << mWaitToBitErrorEvent;

    SC_METHOD(WaitToPhyscBusErrorMethod)  // Add by SonTran: No.3-2,3-3 Bit error detection when transmit Wake-up signal
    dont_initialize();
    sensitive << mWaitToPhyscBusErrorEvent;

    SC_METHOD(WriteOutputMethod)
    dont_initialize();
    sensitive << mWriteOutputEvent;

    SC_METHOD(CheckBitErrorMethod)
    dont_initialize();
    sensitive << mCheckBitErrorEvent;

    SC_METHOD(ReceptionCompleteMethod) //Add by UyenLe : No.10, 11 Successful Wake-up reception Flag and interrupt set timing (3.0Tbits after receive wake-up signal) (RLIN3v4)
    dont_initialize();
    sensitive << mReceptionCompleteEvent;

    mCurrent_Clock = 0;
    mPclk_Clock    = 0;
    
}//}}}

//Destructor
Crlin3_common::~Crlin3_common()
{//{{{
}//}}}

void Crlin3_common::SetDataTransfer(unsigned int index, unsigned int data) //input data transfer
{//{{{
    mData_transfer[index] = data;
}//}}}

unsigned int Crlin3_common::CalcChecksumValue (unsigned int pre_checksum_val) //this function return checksum value base on transfer data and kind of checksum
{//{{{
    unsigned int sum = 0;
    unsigned int carry = 0;
    unsigned int data_length = 0;
    if ((mConfigOperation.ldfc&0xF) >= 8) {
        data_length = 8;
    } else {
        data_length = mConfigOperation.ldfc&0xF;
    }
    for (unsigned int i=0;i<=data_length;i++) {
        // Mod by SonTran: No.1-3,1-4 Checksum is only transmitted at the end of last group data
        if (i == 0) {
            sum = sum + (0xFF - pre_checksum_val);
        } else {
            sum = sum + mData_transfer[i];
        }
        carry = ((sum & 0x100) >> 8)&0x1;
        sum = sum & 0xFF;
        sum = sum + carry;
    }
    return (0xFF - sum);
}//}}}

void Crlin3_common::SetLinClock(double clkc, double pclk)  // this API use for update Lin clock value for common 
{//{{{ 
    mCurrent_Clock = clkc;
    mPclk_Clock    = pclk;
    CalcBitTime();
    if (!CheckZeroClock()) {
        //cancel all processes
        mTransmitEvent.cancel();
        mWriteOutputEvent.cancel();
        mWaitToBitErrorEvent.cancel();
        mWaitToPhyscBusErrorEvent.cancel();
        mCheckBitErrorEvent.cancel();
        mReceptionCompleteEvent.cancel();
    }
}//}}}

bool Crlin3_common::CheckZeroClock()  // this API use for checking zero clocks value for rlin operation
{//{{{ 
    if (mPclk_Clock == 0) {
        return false;        
    }
    if (mCurrent_Clock == 0) {
        return false;        
    }
    return true;
}//}}}

unsigned int Crlin3_common::CalcBitTime() //this function update bit time unit 100ns base on each mode
{//{{{
    double fa = 0;
    double fb = 0;
    double fc = 0;
    double fd = 0;
    unsigned int baud_div = 0;
    double prescaler_clkc = 0;
    unsigned int sample_bit_count = (mConfigOperation.lwbr >> 4)&0xF; // NSPB
    if (sample_bit_count == 0) {
        sample_bit_count = 16;
    } else {
        sample_bit_count = sample_bit_count + 1;
    }
    
    if (mSelf_test) {
        mBit_time = (unsigned int)(((double(emNanoSecond) * sample_bit_count)/mCurrent_Clock)/emBitTimeUnit);
    } else {
        prescaler_clkc = (double)(mCurrent_Clock/(1 << ((mConfigOperation.lwbr >> 1)&0x7))); // prescaler clock lin input
        prescaler_clkc = ( emBitTimeUnit * prescaler_clkc ) / sample_bit_count;
        if ((mConfigOperation.lmd&0x3) == 0) { // RLIN3 is in Master mode 
            fa = (double)(prescaler_clkc/(mConfigOperation.lbrp0+1));
            fb = fa/2;
            fc = fa/8;
            fd = (double)(prescaler_clkc/(mConfigOperation.lbrp1+1));
            if ((((mConfigOperation.lcuc>>1)&0x1) == 0x0)&&((mConfigOperation.lwbr&0x1) == 0x1)) { //wake up mode and fix fa
                mBit_time = (unsigned int)(emNanoSecond/fa);
            } else {
                switch (((mConfigOperation.lmd>>2)&0x3)) {
                    case 0:
                        mBit_time = (unsigned int)(emNanoSecond/fa);
                        break;
                    case 1:
                        mBit_time = (unsigned int)(emNanoSecond/fb);
                        break;
                    case 2:
                        mBit_time = (unsigned int)(emNanoSecond/fc);
                        break;
                    default:
                        mBit_time = (unsigned int)((emNanoSecond/fd)*2);
                        break;
                }
            }
        } else { // RLIN3 is in Slave or Uart modes
            baud_div = (mConfigOperation.lbrp1 << 8)|mConfigOperation.lbrp0;
            fa = (double)(prescaler_clkc/(baud_div+1));
            mBit_time = (unsigned int)(emNanoSecond/fa);
        }
    }
    if (mBit_time > 0xFFFF) 
        mBit_time = 0;
    return mBit_time;
}//}}}   

unsigned int Crlin3_common::CalcBaudRate(unsigned int bit_time) //this function return the number of baud rate setting from bit time
{//{{{ 
    unsigned int sample_bit_count = (mConfigOperation.lwbr >> 4)&0xF; // NSPB
    if (sample_bit_count == 0) {
        sample_bit_count = 16;
    } else {
        sample_bit_count = sample_bit_count + 1;
    }
    double prescaler_clkc = ((double)(mCurrent_Clock)/(1 << ((mConfigOperation.lwbr >> 1)&0x7)))/sample_bit_count; // prescaler clock lin input
    unsigned int baud_val = (unsigned int)((prescaler_clkc*bit_time*emBitTimeUnit)/emNanoSecond) - 1;
    return baud_val;
}//}}}

unsigned int Crlin3_common::GetParity (unsigned int data,unsigned int bit_length)
{//{{{
    unsigned int parity_bit = data&0x1;
    unsigned int temp_bit = 0;
    for (unsigned int i=1;i<=(bit_length-1);i++) {
        temp_bit = (unsigned int)((data&(0x1 << i)) >> i);
        parity_bit = parity_bit ^ temp_bit;
    }
    if (((mConfigOperation.lbfc&0x18) >> 3) == 2) { //0 Parity
        parity_bit = 0;
    } else if (((mConfigOperation.lbfc&0x18) >> 3) == 3) { //Odd parity
        parity_bit = (parity_bit ^ 1);
    }
    return parity_bit;
}//}}}

bool Crlin3_common::CheckIDParity (unsigned int PID) //this function will check wether ID parity is correct not (return true -> correct)
{//{{{
    unsigned int P0 = 0;
    unsigned int P1 = 0;
    unsigned int ID0 = 0;
    unsigned int ID1 = 0;
    unsigned int ID2 = 0;
    unsigned int ID3 = 0;
    unsigned int ID4 = 0;
    unsigned int ID5 = 0;

    ID0 = (PID & 0x01);
    ID1 = (PID >> 1)& 0x01;
    ID2 = (PID >> 2)& 0x01;
    ID3 = (PID >> 3)& 0x01;
    ID4 = (PID >> 4)& 0x01;
    ID5 = (PID >> 5)& 0x01;
    P0  = (PID >> 6)& 0x01;
    P1  = (PID >> 7)& 0x01;

    if ((P0 == (ID0^ID1^ID2^ID4)) && (P1 == (!(ID1^ID3^ID4^ID5)))) {
        return true;
    } else {
        return false;
    }
}//}}}

void Crlin3_common::SetConfigFactors(RlinRegs config_struct) //set config registers to common 
{//{{{ 
    mConfigOperation =  config_struct;
}//}}}

void Crlin3_common::Initialize() // this function will use for initializing variables of common 
{//{{{
    for (unsigned int i = 0; i<32; i++) { 
        mOut_data[i]        = 0;
        mOut_control[i]     = 0;
        mTransfer_period[i] = 0;
    }
    mTrans_status       = emWAKEUP;
    mReceive_status     = emWAKEUP;
    mCurrent_status     = emNormalStatus;
    mCurrent_Error      = emNoneError;
    mReceive_data       = 0;
    mReception_index    = 0;
    mTransmission_index = 0;
    mBit_time           = 0;
    mIsBitError         = false;
    mIsMatch            = false;
    mWrite_time         = 0;
    mRx_data            = 0xFFFFFFFF;
    mSelf_test          = false;
    mPreTransChecksumVal= 0xFF; // Add by SonTran: No.1-3,1-4 Checksum is only transmitted at the end of last group data 
    mPreRecvChecksumVal = 0xFF; // Add by SonTran: No.1-3,1-4 Checksum is only transmitted at the end of last group data 
    mStartRecvBreakLow = 0; // Add by SonTran: No.2-5 Break low less than configuration
    for (unsigned int i = 0; i<9 ; i++) {
        mData_transfer[i] = 0;
    }
}//}}}

void Crlin3_common::SetSelftestMode(bool is_selftest) //this API use to set rlin common to selftest mode 
{//{{{ 
    mSelf_test = is_selftest;
}//}}}

void Crlin3_common::StopOperation(bool is_hw_reset) //this API will use for stopping operation of common 
{//{{{ 
    mTransmitEvent.cancel();
    mWaitToBitErrorEvent.cancel();
    mWaitToPhyscBusErrorEvent.cancel();
    mCheckBitErrorEvent.cancel();
    mReceptionCompleteEvent.cancel();
    mTransmission_index = 0;
    mReception_index = 0;
    mCurrent_status = emNormalStatus;
    mOut_data[0] = 0xFFFFFFFF;
    mOut_control[0] = (mBit_time << 16) | 0x0108;
    mCurrent_Error = emNoneError;
    mWriteOutputEvent.notify();
    mPreTransChecksumVal = 0xFF; // Add by SonTran: No.1-3,1-4 Checksum is only transmitted at the end of last group data 
    mPreRecvChecksumVal = 0xFF; // Add by SonTran: No.1-3,1-4 Checksum is only transmitted at the end of last group data 
    mStartRecvBreakLow = 0; // Add by SonTran: No.2-5 Break low less than configuration
    if (is_hw_reset) {
        Initialize();
    }
}//}}}

void Crlin3_common::TransmitProcess(eTRANS_STATUS status)
{//{{{ 
    if (CalcBitTime() == 0) {
        UpdateErrorStatus(emBitTimeError);
    } else {
        mTransmission_index = 0;
        if (status != emRETIDLE) {
            mTrans_status = status;
        } else { //return idle
            mTransmission_index = 0;
            mOut_data[0] = 0xFFFFFFFF;
            mOut_control[0] = (mBit_time << 16) | 0x0108;
            mCurrent_status = emNormalStatus;
            mWriteOutputEvent.notify();
            return;
        } 
        unsigned int sample_bit_count = (mConfigOperation.lwbr >> 4)&0xF; // NSPB
        if (sample_bit_count == 0) {
            sample_bit_count = 16;
        } else {
            sample_bit_count = sample_bit_count + 1;
        }
        if (mSelf_test) {
            mBit_time = (unsigned int)(((double)emNanoSecond*sample_bit_count)/(mCurrent_Clock*emBitTimeUnit));
        }
        if (mTrans_status == emWAKEUP) {
            mWrite_time = 2;
            TransmitWakeup();
        } else if (mTrans_status == emHEADER) {       
            unsigned int header_data[8]    = {emBeakLow,(unsigned int)emIdleField,emSYNCField,(unsigned int)emIdleField,(unsigned int)emIdleField,mConfigOperation.lidb,(unsigned int)emIdleField,(unsigned int)emIdleField}; //this array stores output value of header data
            unsigned int header_control[8] = {emBeakLowControl,emIdleControl,emStartControl,emStopControl,emIdleControl,emStartControl,emStopControl,emIdleControl}; //this array stores output value of header control
            double initial_time     = (double)(mBit_time*emBitTimeUnit);
            double transfer_time[8] = { // [insert indent]
              0,0,0,initial_time,0,0,initial_time,0}; // this array stores transfer time period of each transferring
            if (mSelf_test) {//assign default value for break and space
                if ((mConfigOperation.lmd&0x3) == 0) {//master mode
                    transfer_time[0] = (double)((mBit_time*emBitTimeUnit)*((mConfigOperation.lbfc&0xF)+13)); //Break low transfer time
                    transfer_time[1] = (double)((mBit_time*emBitTimeUnit)*(((mConfigOperation.lbfc >> 4)&0x3)+1)); //Break del transfer time
                } else {//slave mode
                    transfer_time[0] = (double)((mBit_time*emBitTimeUnit)*13); //Break low transfer time
                    transfer_time[1] = (double)((mBit_time*emBitTimeUnit)*1); //Break del transfer time
                }
                transfer_time[2] = (double)(mBit_time*emBitTimeUnit)*9; //SYNC field and PID field transfer time
                transfer_time[4] = (double)((mBit_time*emBitTimeUnit)*(mConfigOperation.lsc&0x7)); //response space //Add by UyenLe : No.5,6 (a) LSC.IBHS of LIN space configuration bits influence operation (RLIN3v4)
                transfer_time[5] = (double)(mBit_time*emBitTimeUnit)*9; //SYNC field and PID field transfer time
                transfer_time[7] = (double)((mBit_time*emBitTimeUnit)*0); //response space
            } else {
                transfer_time[0] = (double)((mBit_time*emBitTimeUnit)*((mConfigOperation.lbfc&0xF)+13)); //Break low transfer time
                transfer_time[1] = (double)((mBit_time*emBitTimeUnit)*(((mConfigOperation.lbfc >> 4)&0x3)+1)); //Break del transfer time
                transfer_time[2] = (double)(mBit_time*emBitTimeUnit)*9; //SYNC field and PID field transfer time
                transfer_time[4] = (double)((mBit_time*emBitTimeUnit)*(mConfigOperation.lsc&0x7)); //response space
                transfer_time[5] = (double)(mBit_time*emBitTimeUnit)*9; //SYNC field and PID field transfer time
                transfer_time[7] = (double)((mBit_time*emBitTimeUnit)*(mConfigOperation.lsc&0x7)); //response space
            }
            for (unsigned int i = 0; i<8 ; i++) {
                mOut_data[i]        = header_data[i];
                mOut_control[i]     = (mBit_time << 16) | 0x0100 | (header_control[i] << 6) | 8;
                mTransfer_period[i] = transfer_time[i];
            }
            mWrite_time = 8; // Mod by SonTran: No.1-2 Remove transmit respond space at the end of transmitting header
            TransmitHeaderLoop();
        } else { //respond data
            mWrite_time = CalcNumOfByte(mTrans_status)*3 ;
            TransmitRespLoop();
        } 
    }
}//}}}

void Crlin3_common::TransmitWakeup()
{//{{{ 
    mOut_data[mTransmission_index]        = 0;
    mOut_control[mTransmission_index]     = (mBit_time << 16) | 0x0108;
    mTransfer_period[mTransmission_index] = (double)((mBit_time*emBitTimeUnit)*(((mConfigOperation.lwup >> 4)&0xF)+1)); // calculate transfer period base on number of low bit in wake-up mode
    mWriteOutputEvent.notify();
    mCurrent_status = emNormalStatus;
    mTransmitEvent.notify((double)mTransfer_period[mTransmission_index],SC_NS);
    if (((mConfigOperation.lede&0x3) != 0)&&(mTransmission_index < (mWrite_time-1))) {//notify checking bit error and physical error  after 81.25% of bit time if checking is enable
        mCheckBitErrorEvent.notify((double)(mBit_time*emBitTimeUnit*0.8125),SC_NS);
    }
}//}}}

void Crlin3_common::TransmitHeaderLoop()
{//{{{
    // Mod by SonTran: No.1-2 Update to remove transmit respond space at the end of transmitting header
    if (mTransmission_index == 6) { //finish transfer frame header
        mCurrent_status = emFrameHeaderComplete;
        // Add by SonTran: No.1-3,1-4 Checksum is only transmitted at the end of last group data 
        if (((mConfigOperation.ldfc >> 5)&0x1) == 1) {
            mPreTransChecksumVal = 0xFF - (mConfigOperation.lidb & 0xFF);
            mPreRecvChecksumVal  = 0xFF - (mConfigOperation.lidb & 0xFF);
        }
    } else {
        mCurrent_status = emNormalStatus;
    }
    mWriteOutputEvent.notify();
    if (mTransmission_index <= 6) {
        mTransmitEvent.notify((double)(mTransfer_period[mTransmission_index]),SC_NS);
        if (!(mSelf_test && ((mConfigOperation.lmd&0x3) >= 2))) { // Do not check bit error in case self test of Slave
            if (((mConfigOperation.lede&0x3) != 0)&&(mTransfer_period[mTransmission_index] != 0)) {//notify checking bit error and physical error  after 81.25% of bit time if checking is enable
                mCheckBitErrorEvent.notify((double)(mBit_time*emBitTimeUnit*0.8125),SC_NS);
            }
        }
    }
}//}}}

void Crlin3_common::TransmitRespLoop()
{//{{{
    unsigned int mode                = mConfigOperation.lmd&0x3;
    unsigned int resp_data           = 0;
    unsigned int resp_control        = 0;
    unsigned int max_index           = (CalcNumOfByte(mTrans_status)*3);
    double resp_transfer_time = (double)(mBit_time*emBitTimeUnit);  
    mCurrent_status = emNormalStatus;
    bool en_checksum_transmit = true; //Indicate checksum is enabled to send // Add by SonTran: No.1-3,1-4 Checksum is only transmitted at the end of last group data 
    if ((mTransmission_index%3) == 0) { //write start bit and data
        if ((mTransmission_index == mWrite_time-3)&&(mode != emUartMode)) { //write checksum value in LIN mode
            if ((mSelf_test)&&(((mConfigOperation.ldfc>>4)&0x1) == 0)) {//selftest reception, transfer LCBR as checksum value 
                resp_data = mConfigOperation.lcbr;
                resp_transfer_time = resp_transfer_time*9;
            } else {
                resp_data = CalcChecksumValue(mPreTransChecksumVal);
                // Mod by SonTran: No.1-3,1-4 Checksum is only transmitted at the end of last group data
                if (((mConfigOperation.ldfc>>7)&0x1) == 1) { //not last group data 
                    en_checksum_transmit = false;
                    resp_transfer_time = 0;
                    mTransmission_index += 1;
                    mPreTransChecksumVal = resp_data;
                } else {
                    resp_transfer_time = resp_transfer_time*9;
                    // Mod by SonTran: No.1-3,1-4 Checksum is only transmitted at the end of last group data
                    UpdateRegisters(emChecksumReg, resp_data); // write checksum value to LCBR -page 127
                    mPreTransChecksumVal = 0xFF;
                }
            }
        } else { 
            if (mode != emUartMode) {
                resp_data = mData_transfer[1+mTransmission_index/3];
                resp_transfer_time = resp_transfer_time*9;
            } else {
                mIsBitError = true;
                resp_data = mData_transfer[mTransmission_index/3];
                if (((mConfigOperation.lbfc&0x1) == 1)&&(((mConfigOperation.lbfc>>3)&0x3) == 0)) {//7bit in UART mode, no parity
                    resp_transfer_time = resp_transfer_time*8;
                    resp_data = resp_data&0x7F;
                } else if ((((mConfigOperation.luor1&0x1) == 1)&&(((mConfigOperation.lbfc>>3)&0x3) == 0))||(((mConfigOperation.lbfc&0x1) == 0)&&((mConfigOperation.luor1&0x1) == 0)&&(((mConfigOperation.lbfc>>3)&0x3) != 0))) {
                    resp_transfer_time = resp_transfer_time*10;
                    resp_data = resp_data&0x1FF;
                } else if (((mConfigOperation.luor1&0x1) == 1)&&(((mConfigOperation.lbfc>>3)&0x3) != 0)) { //expension bit ,1 bit parity
                    resp_transfer_time = resp_transfer_time*11;
                    resp_data = resp_data&0x3FF;
                } else {
                    resp_transfer_time = resp_transfer_time*9;
                    resp_data = resp_data&0xFF; 
                }
            }
        }
        resp_control = (mBit_time << 16) | 0x0148;
        // Modified by SonTran: No.46 Transmission interrupt is only generated for the last byte (RLIN3v4)
        if ((mode == emUartMode)&&(((mConfigOperation.luor1 >> 3)&0x1) == 0)&&(mTransmission_index == (max_index - 3))){ // output interrupt when transfer start bit in UART mode at the last byte 
            UpdateStatus(emIssueTransInt);
        }
    } else if (((mTransmission_index+2)%3) == 0) { //write stop bit, data is 0xFFFF , wait 1 bit time
        resp_data = 0xFFFFFFFF;
        resp_control = (mBit_time << 16) | 0x0188;
        if ((mode == emUartMode)&&((mConfigOperation.lbfc >> 2)&0x1)) { //check number of stop bit in UART mode
            resp_transfer_time = resp_transfer_time*2;
        }
    } else { //write IBS (idle), data is 0xFFFF , wait bit time*IBS
        resp_data = 0xFFFFFFFF;
        resp_control = (mBit_time << 16) | 0x0108;

        //Modified by UyenLe : No.5,6(b) 37,38 (1b) LSC.IBS of LIN space configuration bits influence Transmit operation, NOT influence Receive operation (RLIN3v4)
        if ((mSelf_test)&&(((mConfigOperation.ldfc >> 4)&0x1) == 0)) {//assign default value for space in case of self-test receive
            resp_transfer_time = 0;
        } else {
            resp_transfer_time = (double)(resp_transfer_time*((mConfigOperation.lsc >> 4)&0x3));
        }

        if ((mTransmission_index == (mWrite_time-1))&&(mode != emUartMode)) {
            mCurrent_status = emRespondTransComplete; //transmission complete
            mTransfer_period[mTransmission_index] = 0;
        }
        if (mode == emUartMode) {
            // Modified by SonTran: No.46 Transmission interrupt is only generated for the last byte (RLIN3v4)
            mCurrent_status = emRespondTransComplete;
            if (mTransmission_index == (max_index - 1)) {
                mCurrent_status = emFinishTransData;
                if (((mConfigOperation.luor1 >> 3)&0x1) == 1) { //output interrupt when finish transfer stop bit in UART mode at the last byte
                    UpdateStatus(emIssueTransInt);
                }
            }
            if (mTransmission_index == (mWrite_time-1)) {
                mIsBitError = false;
            }
        } else {
            if (mCurrent_status != emRespondTransComplete) {
                mCurrent_status = emFinishTrans1Data;
            }
        }
    }
    mTransfer_period[mTransmission_index] = resp_transfer_time;
    if (en_checksum_transmit) {
        mOut_data[mTransmission_index] = resp_data ;
        mOut_control[mTransmission_index] = resp_control;
        if ((mCurrent_status != emErrorDetect)&&((mCurrent_status != emRespondTransComplete)||(mode == emUartMode))) {
            if (((mode == emUartMode) || (((mConfigOperation.ldfc >> 4)&0x1) == 0x1)) && ((mConfigOperation.lede&0x3) != 0)) {//notify checking bit error and physical error after 81.25% of bit time if checking is enable
                mCheckBitErrorEvent.notify((double)(mBit_time*emBitTimeUnit*0.8125),SC_NS);
            }
        } else {
            mTransfer_period[mTransmission_index] = 0;
        }
        mWriteOutputEvent.notify();
    }
    mTransmitEvent.cancel();
    mTransmitEvent.notify((double)(mTransfer_period[mTransmission_index]),SC_NS);
}//}}} 

double Crlin3_common::CalcBitBoundary(unsigned int max_bit, unsigned int input_data, unsigned int expected_data)
{//{{{
    unsigned int wrong_bit = max_bit;
    double wait_time = 0;
    for (unsigned int i=0; i<max_bit; i++) {
        if ((((input_data>>i)&0x1) == 1) != (((expected_data>>i)&0x1) == 1)){
            wrong_bit = i; 
            break;
        }
    }
    if (wrong_bit == max_bit) {
        return 0;
    } else {
        wait_time = (double)((wrong_bit+1)*(mBit_time*emBitTimeUnit));
        return wait_time;
    }
}//}}}

bool Crlin3_common::CheckBreakLowPeriod(unsigned int mode) // Add by SonTran: No.2-5 Break low less than configuration in Slave mode
{//{{{
    bool result = true;
    double break_low_period = GetCurTime() - mStartRecvBreakLow;
    double expected_period = (double)(CalcBitTime()*emBitTimeUnit);
    if (mode == emSlaveFixBaudRateMode) {
        if ((mConfigOperation.lbfc&0x1) == 0) {
            expected_period = expected_period*9.5;
        } else {
            expected_period = expected_period*10.5;
        }
        if (break_low_period <= expected_period) {
            result = false;
        }
    } else if (mode == emSlaveAutoBaudRateMode) {
        if ((mConfigOperation.lbfc&0x1) == 0) {
            expected_period = expected_period*10;
        } else {
            expected_period = expected_period*11;
        }
        if (break_low_period <= expected_period) {
            result = false;
        }
    }
    return result;
}//}}}

unsigned int Crlin3_common::CalcNumOfByte(eTRANS_STATUS  status)
{//{{{
    unsigned int mode = mConfigOperation.lmd&0x3;
    unsigned int num_of_byte = 1; 
    if (mode != emUartMode) { 
        if ((mConfigOperation.ldfc&0xF) >= 8) {
            num_of_byte = 9;
        } else {
            num_of_byte = (mConfigOperation.ldfc&0xF) + 1;
        }
    } else {
        if (status != emUARTSINGLE) {
            if (((mConfigOperation.ldfc&0xF) == 0)||((mConfigOperation.ldfc&0xF) >= 9)) {
                num_of_byte = 9;
            } else {
                num_of_byte = mConfigOperation.ldfc&0xF;
            }
        } 
    }
    return num_of_byte;
}//}}}

void Crlin3_common::WaitToBitErrorMethod ()
{//{{{
    // Modified by SonTran: No.59 Reception and Transmission will continue even if error is detected (RLIN3v4)
    unsigned int mode = mConfigOperation.lmd&0x3;
    if (mode != emUartMode) {
        mTransmitEvent.cancel();
    }
    UpdateErrorStatus(emBitError);
}//}}} 

void Crlin3_common::WaitToPhyscBusErrorMethod ()
{//{{{
    mTransmitEvent.cancel();
    // Add by SonTran: No.3-2,3-3 Bit error detection when transmit Wake-up signal
    UpdateErrorStatus(emPhysicalError);
}//}}} 

void Crlin3_common::CheckBitErrorMethod()
{//{{{ 
    if (mRx_data != mOut_data[mTransmission_index]) { // bit error or physical error
        //calculate time to assert error flag -> bit boundary
        double wait_boundary = 0;
        if (((mConfigOperation.lede >> emPhysicalError)&0x1) == 1) { //physical error handling if Physical Error enable 
            if (mTrans_status == emWAKEUP){//detect high when transf wakeup signal
                wait_boundary = CalcBitBoundary(((mConfigOperation.lwup >> 4)&0xF) + 1, mRx_data, 0x0);
            } else if ((mTrans_status == emHEADER)&&(mTransmission_index == 0)) { //detect high when transfer break low
                wait_boundary = CalcBitBoundary((mConfigOperation.lbfc&0xF) + 13, mRx_data, 0x0);
            } else if ((mTrans_status == emHEADER)&&(mTransmission_index == 1)) { //detect low when transfer break high
                wait_boundary = CalcBitBoundary(((mConfigOperation.lbfc >> 4)&0x3) + 1, mRx_data, 0xFFFFFFFF);
            }
            if (wait_boundary != 0) {
                mCurrent_Error = emPhysicalError;
                wait_boundary = wait_boundary - (double)(mBit_time*emBitTimeUnit*0.8125);
                mWaitToPhyscBusErrorEvent.notify(wait_boundary,SC_NS);
            }
        }
        wait_boundary = 0; // Add by SonTran: No.3-2,3-3 Bit error detection when transmit Wake-up signal
        if (((mConfigOperation.lede >> emBitError)&0x1) == 1) { //bit error handling if bit Error enable
            if (mTrans_status == emHEADER){
                if (((mTransmission_index == 4)||(mTransmission_index == 7))&&((mConfigOperation.lsc&0x7) != 0)) { // detect bit error when transfer IBHS or response space 
                    wait_boundary = CalcBitBoundary(mConfigOperation.lsc&0x7, mRx_data, 0xFFFFFFFF);
                } else if ((mTransmission_index == 2)||(mTransmission_index == 5)){ // detect bit error when transfer SYNC or PID
                    wait_boundary = (double)(9*mBit_time*emBitTimeUnit);
                } else if (mTransmission_index == 0) { //detect high when transfer break low
                    wait_boundary = CalcBitBoundary((mConfigOperation.lbfc&0xF) + 13, mRx_data, 0x0);
                } else if (mTransmission_index == 1) { //detect low when transfer break high
                    wait_boundary = CalcBitBoundary(((mConfigOperation.lbfc >> 4)&0x3) + 1, mRx_data, 0xFFFFFFFF);
                }
            }
            if ((mTrans_status == emRESP)||(mTrans_status == emUARTSINGLE)) {
                if (((mTransmission_index%3) == 2)&&(((mConfigOperation.lsc >> 4)&0x3) != 0)) { //detect bit error when transfer Inter-byte space
                    wait_boundary = CalcBitBoundary((mConfigOperation.lsc >> 4)&0x3, mRx_data, 0xFFFFFFFF);
                } else if ((mTransmission_index%3) == 0) { //detect bit error when transfer data in respond
                    wait_boundary = (double)(9*mBit_time*emBitTimeUnit); 
                }
            }
            // Add by SonTran: No.3-2,3-3 Bit error detection when transmit Wake-up signal
            if (mTrans_status == emWAKEUP){
                wait_boundary = CalcBitBoundary(((mConfigOperation.lwup >> 4)&0xF) + 1, mRx_data, 0x0);
            }
            if (wait_boundary != 0) {
                mCurrent_Error = emBitError;
                // Modified by SonTran: No.56 Bit error flag will asserts without waiting byte boundary (RLIN3v4)
                unsigned int mode = mConfigOperation.lmd&0x3;
                if (mode == emUartMode) {
                    wait_boundary = 0;
                }else{
                    wait_boundary = wait_boundary - (double)(mBit_time*emBitTimeUnit*0.8125);
                }
                mWaitToBitErrorEvent.notify(wait_boundary,SC_NS);
            }
        }
    }
}//}}}

void Crlin3_common::HeaderReception(unsigned int data_input, unsigned int control_input, unsigned int mode)
{//{{{
    bool is_break_low_detect = false;
    if  (mReception_index == 0) { // reception frame header, receiving break low bits
        if (((control_input >> 6)&0x3) == emBeakLowControl) {
            mReception_index = 1;
            // Add by SonTran: No.2-5 Break low less than configuration
            mStartRecvBreakLow = GetCurTime(); 
            _re_printf("info","Break low is detected, start header receiving process.\n");
        } else {
            _re_printf("warning","Break low is detected unsuccessfully.\n");
        }
    } else {
        // Mod by SonTran: No.2-5 Break low detection during reception header or respond TX/RX in Slave mode
        if  (((control_input >> 6)&0x3) == emBeakLowControl) { // reception frame header , receiving break low bits
            is_break_low_detect = true;
        }
        switch (mReception_index) {
            case emBreakHigh :
                if ((((control_input >> 6)&0x3) == emIdleControl) && (data_input == emIdleField)) {
                    // Add by SonTran: No.2-5 Break low less than configuration in Slave mode
                    if (!CheckBreakLowPeriod(mode)) {
                        _re_printf("warning","Break low period is less than configuration value.\n");
                        mReception_index = 0;
                    } else {
                        if ((((control_input >> 6)&0x3) == emIdleControl)&&(data_input == emIdleField)) { // receive break high successfully 
                            mReception_index = mReception_index+1;
                        }
                    }
                } else {
                    _re_printf("warning","Break delimiter is detected unsuccessfully.\n");
                    mReception_index = 0;
                }
                break;
            case emStartSYNC :
                mConfigOperation.lbss = mConfigOperation.lbss | 0x1;
                UpdateRegisters(emLBSSReg, mConfigOperation.lbss);
                if (((control_input >> 6)&0x3) == emStartControl) {
                    if (data_input == emSYNCField) { // receive SYNC field successfully
                        if (mode == emSlaveFixBaudRateMode) { // fix baud rate mode
                            if (mBit_time != ((control_input >> 16)&0xFFFF)){ // baud rate is not match with slave
                                mCurrent_Error = emSYNCErrorOrExpandBit;
                            }
                        } else { // auto detect baud rate mode
                            //Calculate BRP0 and BRB1
                            unsigned int value = CalcBaudRate((control_input >> 16)&0xFFFF);
                            UpdateRegisters(emBaudRateReg, value ); //update register BRP0 and BRP1
                            mConfigOperation.lbrp0 = value&0xFF;
                            mConfigOperation.lbrp1 = (value>>8)&0xFF;
                        }
                    } else { //receive SYNC field error
                        // Mod by SonTran: No.2-3 SYNC field error flag
                        mCurrent_Error = emSYNCErrorOrExpandBit;
                    }
                    mReception_index = mReception_index+1;
                } else {
                    _re_printf("warning","SYNC start bit is detected unsuccessfully.\n");
                    mReception_index = 0;
                }
                break;
            case emStopSYNC :
                if (((control_input >> 6)&0x3) == emStopControl) {
                    mReception_index = mReception_index + 1;
                } else {
                    _re_printf("warning","SYNC stop bit is detected unsuccessfully.\n");
                    mReception_index = 0;
                }
                break;
            case emInterHeaderSpace:
                if (((control_input >> 6)&0x3) == emIdleControl) {
                    if (mCurrent_Error == emSYNCErrorOrExpandBit) {
                        UpdateErrorStatus(emSYNCErrorOrExpandBit);//update error status
                        mReception_index = 0;
                    } else {
                        UpdateStatus(emSuccessReceiveSYNC);
                        mReception_index = mReception_index+1;
                    }
                } else {
                    _re_printf("warning","Inter header space is detected unsuccessfully.\n");
                    mReception_index = 0;
                }
                break;
            case emStartPID :
                if (((control_input >> 6)&0x3) == emStartControl) {
                    if ((CheckIDParity(data_input) == false)&&((((mConfigOperation.lede >> 6)&0x1) == 1))&&((mConfigOperation.lmd&0x3) >= 0x2)) { // PID error
                        mCurrent_Error = emIdentifyError; 
                    } 
                    mReceive_data = data_input;
                    mReception_index = mReception_index+1;
                } else {
                    _re_printf("warning","PID start bit is detected unsuccessfully.\n");
                    mReception_index = 0;
                }
                break;
            case emStopPID :
                if (((control_input >> 6)&0x3) == emStopControl) {
                    if ((!mSelf_test)||((mSelf_test)&&(mCurrent_Error != emNoneError))) {
                        UpdateRegisters(emPIDReg, mReceive_data);
                        // Add by SonTran: No.1-3,1-4 Checksum is only transmitted at the end of last group data 
                        if (((mConfigOperation.ldfc >> 5)&0x1) == 1) {
                            mPreTransChecksumVal  = 0xFF - (mReceive_data & 0xFF);
                            mPreRecvChecksumVal   = 0xFF - (mReceive_data & 0xFF);
                        }
                    }
                    // Mod by SonTran: No.2-1 Also detect Frame error with receving Stop bit of PID
                    if ((data_input != emIdleField)&&(((mConfigOperation.lede >> 3)&0x1) == 1)) { // receive stop bit error -> frame error 
                        if (mCurrent_Error == emIdentifyError) {
                            mCurrent_Error = emIdentifyErrorAndFramingError;
                        } else {
                            mCurrent_Error = emFrameError;
                        }
                    }
                    mReception_index = mReception_index+1;
                } else {
                    _re_printf("warning","PID stop bit is detected unsuccessfully.\n");
                    // Mod by SonTran: No.2-1 Also detect Frame error with receving Stop bit of PID
                    if (((mConfigOperation.lede >> 3)&0x1) == 1) { // receive stop bit error -> frame error 
                        mCurrent_Error = emFrameError;
                        UpdateErrorStatus(mCurrent_Error);
                    }
                    mReception_index = 0;
                }
                break;
            default :
                if (mCurrent_Error != emNoneError) {
                    UpdateErrorStatus(mCurrent_Error);
                    if (mCurrent_Error == emIdentifyErrorAndFramingError) {
                        mCurrent_Error = emFrameError;
                        UpdateErrorStatus(mCurrent_Error);
                    }
                } else {
                    mCurrent_status = emFrameHeaderComplete;
                    UpdateStatus(mCurrent_status);
                }
                mReception_index = 0;
                break;
        }
        // Mod by SonTran: No.2-5 Break low detection during reception header or respond TX/RX in Slave mode
        if (is_break_low_detect) {
            mReception_index = 1;
            mCurrent_status = emNormalStatus;
            mStartRecvBreakLow = GetCurTime(); 
            _re_printf("info","Break low is detected, start header receiving process.\n");
        }
    }
}//}}}

void Crlin3_common::RespReception(unsigned int data_input, unsigned int control_input, unsigned int mode)
{//{{{
    if ((mReception_index == 0)&&(((control_input >> 6)&0x3) != 0x1)) {
        return ;
    }
    mReception_index = mReception_index+1; //increase reception index to define data reception
    if ((((control_input >> 6)&0x3) == 0x2) && (mReception_index%3 == 2)) { //receiving stop bit
        bool is_error = false;
        // Modified by SonTran: No.15 (2) One byte receive complete flag is asserted even if any error occurs (RLIN3v4) 
        if ((mReception_index == 2)&&(mode != emUartMode)&&((mCurrent_Error == emChecksumErrorOrMatchID)||((data_input != 0xFFFFFFFF)&&(((mConfigOperation.lede >> 3)&0x1) == 1)))) {
            UpdateStatus(emOneByteRecevComplete);
        }
        if (mCurrent_Error == emChecksumErrorOrMatchID) {
            is_error = true;
            UpdateErrorStatus(mCurrent_Error);
            UpdateStatus(mCurrent_status);
        }
        if ((mode == emUartMode)&&(mReception_index == ((CalcNumOfByte(mTrans_status)*3)-1))) { //finish receiving last byte data in UART mode
            mCurrent_status = emFinishReceiveData;
            UpdateStatus(mCurrent_status);//inform last bit of last byte data is received    
        } 
        // Modified by SonTran: No.15 (1) Both Checksum error flag and Frame error flag will be asserted if Checksum error and Frame error occur at the same time (RLIN3v4) 
        if ((data_input != 0xFFFFFFFF)&&(((mConfigOperation.lede >> 3)&0x1) == 1)) { //frame error
            if (is_error){
                UpdateErrorStatus(emFrameError);
            }
            is_error = true;
            mCurrent_Error = emFrameError;
        }
        if ((((mConfigOperation.lbfc>>3)&0x3) != 0)&&(mode == emUartMode)) { //Parity check is enabled
            unsigned int bit_length = 7;
            if ((mConfigOperation.luor1&0x1) == 1) { // expension bit enable
                bit_length = 9;
            } else if (((mConfigOperation.lbfc)&0x1) == 0) {
                bit_length = 8;
            }
            if (GetParity(mReceive_data,bit_length) != ((mReceive_data&(0x1 << bit_length)) >> bit_length)) { //Parity Error bit occur
                if (is_error){
                    // Add by SonTran: No.40, 41 (1) LURDR is updated even though any error occurs (RLIN3v4)
                    UpdateErrorStatus(mCurrent_Error);
                }
                is_error = true;
                mCurrent_Error = emIdentifyError;
                mCurrent_status = emErrorDetect;
            }
        }
        if (is_error&&(mode == emUartMode)) { 
            FinishReceiveHandle(mode, mReception_index);
        }
    } else if ((((control_input >> 6)&0x3) == 0x1) && (mReception_index%3 == 1)) {// receive start bit and data
        if (mode != emUartMode) {
            // Mod by SonTran: No.1-3,1-4 Checksum is only transmitted at the end of last group data
            unsigned int max_index = (CalcNumOfByte(mTrans_status)*3);
            if (((mConfigOperation.ldfc>>7)&0x1) == 1) { //not last group data 
                max_index = ((CalcNumOfByte(mTrans_status) - 1)*3);
            }
            if (mReception_index == (max_index-2)) {
                unsigned int checksum_val = CalcChecksumValue(mPreRecvChecksumVal);
                if (((mConfigOperation.ldfc>>7)&0x1) == 0) { //last group data
                    if (checksum_val != data_input) {
                        mCurrent_Error = emChecksumErrorOrMatchID;
                    }
                    if ((!mSelf_test)||((mSelf_test)&&(mCurrent_Error != emChecksumErrorOrMatchID))) {
                        UpdateRegisters(emChecksumReg, data_input);
                    }
                    mPreRecvChecksumVal = 0xFF;
                } else {
                    mData_transfer[(mReception_index/3) + 1] = data_input;
                    checksum_val = CalcChecksumValue(mPreRecvChecksumVal);
                    mPreRecvChecksumVal = checksum_val;
                }
            } else {
                mData_transfer[(mReception_index/3) + 1] = data_input;
            }
        }
        mIsMatch = false;
        if ((mode == emUartMode)&&(mConfigOperation.luor1&0x1)) { // uart mode and enable expansion bit
            if (((data_input >> 8)&0x1) == ((mConfigOperation.luor1 >> 1)&0x1)) { // match expansion bit
                 if ((((mConfigOperation.luor1 >> 2)&0x1) == 0) || (((mConfigOperation.luor1 >> 2)&0x1)&&((data_input&0xFF) == mConfigOperation.lidb))) { //without data comparision or with data comparision and match data input with LIBD register
                     mIsMatch = true;
                 }
            }
        }
        mReceive_data = data_input;
    } else if ((((control_input >> 6)&0x3) == 0x0) && (mReception_index%3 == 0)) { //receive inter-byte space
        FinishReceiveHandle(mode, mReception_index);
    } else { // error
        mCurrent_Error = emFrameError;
        UpdateErrorStatus(mCurrent_Error);
    }
}//}}}

void Crlin3_common::FinishReceiveHandle(unsigned int mode, unsigned int index)
{//{{{
    unsigned int max_index = (CalcNumOfByte(mTrans_status)*3);
    if (((mConfigOperation.ldfc>>7)&0x1) == 1) { //not last group data 
        max_index = ((CalcNumOfByte(mTrans_status) - 1)*3);
    }
    // Mod by SonTran: No.1-3,1-4 Checksum is only transmitted at the end of last group data
    if ((mode != emUartMode) && ((index < max_index) || ((index == max_index) && (((mConfigOperation.ldfc>>7)&0x1) == 1)))) { // Mod by SonTran: No.1 Store data regardless of error
        UpdateRegisters((eREG_KIND)(index/3), mData_transfer[index/3]); // store data to registers
    }
    // Modified by SonTran: No.15 (2) One byte receive complete flag is asserted even if any error occurs (RLIN3v4) 
    if ((index == 3)&&(mode != emUartMode)) { //first byte receive successfully
        mCurrent_status = emOneByteRecevComplete;
        UpdateStatus(mCurrent_status);
    }
    if (mCurrent_Error == emNoneError) {
        if (mCurrent_status == emOneByteRecevComplete) {
            mCurrent_status = emNormalStatus;
        }
        if (mode != emUartMode) {
            if (index == max_index) {
                mCurrent_status = emRespondRecevComplete;
                UpdateStatus(mCurrent_status); 
            }
        } else {
            if ((mConfigOperation.luor1&0x1) == 1) {
                // Modified by SonTran: No.43 Data is always received and assert reception complete interrupt in Expansion bit mode reception without Expansion bit comparison (RLIN3v4) 
                if (((mConfigOperation.luor1>>4)&0x1) == 0) { // Enable Expansion bit comparison
                    if (((mConfigOperation.luor1>>2)&0x1) == 1) {
                        //If expansion bit with data comparison is selected
                        if (mIsMatch == true) {
                            //If match occured - Update EXBT and IDMT flags and assert int_s interrupt
                            UpdateErrorStatus(emChecksumErrorOrMatchID);
                            UpdateRegisters(emDataReg, mReceive_data);
                        } 
                    } else {
                        // Modified by SonTran: No.44, 45 Expansion bit comparation and expansion bit data comparation is judged when reception error (overrun error) occurs (RLIN3v4)
                        UpdateRegisters(emDataReg, mReceive_data);
                        //If expansion bit without data comparison is selected
                        if (mIsMatch == true) {
                            //If match occured - Update EXBT flag and int_s interrupt
                            UpdateErrorStatus(emSYNCErrorOrExpandBit);
                        } else {
                            //If match NOT occured - assert int_r  interrupt and update LURDR
                            mCurrent_status = emRespondRecevComplete;
                            UpdateStatus(mCurrent_status);
                        }
                    }
                } else { // Disable Expansion bit comparison
                    UpdateRegisters(emDataReg, mReceive_data);
                    mCurrent_status = emRespondRecevComplete;
                    UpdateStatus(mCurrent_status);
                }
            } else {
                UpdateRegisters(emDataReg, mReceive_data);
                mCurrent_status = emRespondRecevComplete;
                UpdateStatus(mCurrent_status);
            }
        }
        if ((mCurrent_status != emRespondRecevComplete)&&(mCurrent_status != emOneByteRecevComplete)) {
            mCurrent_status = emFinishRev1Data;
            UpdateStatus(mCurrent_status);
        }
        if (mCurrent_status == emRespondRecevComplete) {
            mReception_index = 0;
        } 
    } else {
        // Add by SonTran: No.40, 41 (1) LURDR is updated even though any error occurs (RLIN3v4)
        eERROR_FLAG pre_err = mCurrent_Error;
        if (mode == emUartMode) {
            UpdateRegisters(emDataReg, mReceive_data);
        }
        mCurrent_Error = pre_err;
        mCurrent_status = emErrorDetect;
        UpdateErrorStatus(mCurrent_Error);
        UpdateStatus(mCurrent_status);
    }
}//}}}

void Crlin3_common::ReceptionCompleteMethod() //Add by UyenLe : No.10, 11 Successful Wake-up reception Flag and interrupt set timing (3.0Tbits after receive wake-up signal) (RLIN3v4) 
{///{{{
    mReception_index = 0;
    mCurrent_status = emRespondRecevComplete;
    UpdateStatus(mCurrent_status); //update status
}///}}}

void Crlin3_common::ReceptionProcess(unsigned int data_input, unsigned int control_input, eTRANS_STATUS  status)
{//{{{
    if (status == emCHECKBITERR) { //for check bit error and physical error
        mRx_data = data_input;
        return;
    }
    mReceive_status = status;
    CalcBitTime();
    unsigned int mode = mConfigOperation.lmd&0x3;
    if ((mReceive_status == emHEADER)&&(((mode >= 0x2)&&(mConfigOperation.lstc != 1))||(mSelf_test))) { // reception header
        HeaderReception(data_input, control_input, mode);
    } else {
        if (((((mConfigOperation.ldfc >> 4)&0x1) == 1)&&(mode == emMasterMode))
          ||((mode == emUartMode)&&(((mConfigOperation.lst >> 4)&0x1) == 1)&&(((mConfigOperation.lst >> 5)&0x1) == 0))) {
        } else { 
            if (mReceive_status == emWAKEUP) {
                //Modified by UyenLe : No.10, 11 Successful Wake-up reception Flag and interrupt set timing (3.0Tbits after receive wake-up signal) (RLIN3v4) 
                if (data_input == 0) {
                     double reception_time = (double)(mBit_time*emBitTimeUnit*3) ;
                     mReceptionCompleteEvent.notify(reception_time, SC_NS) ;
                }
            } else if ((mode != emUartMode)||((mode == emUartMode)&&(((mConfigOperation.lst >> 5)&0x1) == 1))) {
                RespReception(data_input, control_input, mode);
            } 
        }
    }
}//}}}

void Crlin3_common::TransmitDataMethod ()
{//{{{
    unsigned int index = mTransmission_index;
    unsigned int mode = mConfigOperation.lmd&0x3;
    if ((mCurrent_status == emRespondRecevComplete)&&((index%3) == 0)&&(mode == emUartMode)) {
        mCurrent_status = emNormalStatus;
    }
    if (mCurrent_status != emNormalStatus) {
        UpdateStatus(mCurrent_status);
        if ((mode == emMasterMode)&&(mCurrent_status == emRespondTransComplete)) {
            return;
        }
    }
    if ((mCurrent_status != emErrorDetect)&&(index < (mWrite_time - 1))) {
        mTransmission_index = mTransmission_index+1;
        if (mTrans_status == emHEADER) {
            TransmitHeaderLoop();
        } else if ((mTrans_status == emRESP) || (mTrans_status == emUARTSINGLE)) {
            TransmitRespLoop();
        } else {
            mCurrent_status     = emRespondTransComplete;
            UpdateStatus(mCurrent_status);
        }
    } else {
        mTransmission_index = 0;
    } 
}//}}} 

void Crlin3_common::WriteOutputMethod()
{//{{{
    unsigned int bit_num = 0;
    unsigned int mode = mConfigOperation.lmd&0x3;
    if (mode != emUartMode) {
        bit_num = 8;
        if ((mConfigOperation.ldfc&0xF) < 8) {
            bit_num = mConfigOperation.ldfc & 0xF;
        }
    } else {
        bit_num = 9;
        if (((mConfigOperation.ldfc&0xF) < 9)&&((mConfigOperation.ldfc&0xF) != 0)) {
            bit_num = mConfigOperation.ldfc & 0xF;
        }
        unsigned int ubos = (mConfigOperation.lbfc >> 1) & 0x1;
        unsigned int dir = (~ubos) & 0x1;
        mOut_control[mTransmission_index] = (dir << 8)|(mOut_control[mTransmission_index]&0xFFFFFEFF);
    }
    mOut_control[mTransmission_index] = (bit_num << 9)|(mOut_control[mTransmission_index]&0xFFFFE1FF);
    OutputData(mOut_control[mTransmission_index], mOut_data[mTransmission_index]);
}//}}}

/*********************************
// Function     : GetCurTime
// Description  : Get current simulation time
// Parameter    : None
// Return value : Current simulation time (unit SC_NS)
**********************************/
double Crlin3_common::GetCurTime(void)
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
