// ---------------------------------------------------------------------
// $Id: $
//
// Copyright(c) 2018-2018 Renesas Electronics Corporation
// Copyright(c) 2018-2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __RLIN3_COMMON_H__
#define __RLIN3_COMMON_H__
#include "systemc.h"
#include <map>
#include <string>

/// RLIN3 model class
class Crlin3_common: public sc_module 
{
public:
    //----------------------enums---------------------//
    enum eACCESS_MODE {                 //Enum describe access modes
        emR    = 0x0                    //Read only mode
       ,emRW   = 0x1                    //Read/write mode
       ,emR0   = 0x2                    //Read 0 mode
       ,emRW0  = 0x3                    //Write 0 to clear mode
    };

    enum eOPERATION_MODE {              //enum describe operation modes
        emMasterMode             = 0x0  //Master mode
       ,emUartMode               = 0x1  //Uart mode
       ,emSlaveAutoBaudRateMode  = 0x2  //Slave mode with auto correct baud rate
       ,emSlaveFixBaudRateMode   = 0x3  //Slave mode with Fix baud rate mode
    };                           
                                 
    enum eTRANS_STATUS {                //Enum describe transmission/reception modes
        emWAKEUP                 = 0x0  //Wake-up mode
       ,emHEADER                 = 0x1  //Header mode
       ,emRESP                   = 0x2  //Respond mode
       ,emUARTSINGLE             = 0x3  //Uart single mode
       ,emRETIDLE                = 0x4  //return idle mode
       ,emCHECKBITERR            = 0x5  //check bit error or physical error mode
       ,emUARTMULTI              = 0x6  //Uart multi mode
    };

    enum eLIN_STATUS {                  //Enum describe statuses of RLIN3
        emRESET                  = 0x0  //Reset mode
       ,emIDLE                   = 0x1  //Idle mode
       ,emOPERATION              = 0x2  //Operation mode
    };
    
    enum eSTATUS_FLAG {                 //Enum describe operation statuses
        emRespondTransComplete   = 0x0  //Respond transmission complete status
       ,emRespondRecevComplete   = 0x1  //Respond receive complete status
       ,emErrorDetect            = 0x3  //Error detect status
       ,emOneByteRecevComplete   = 0x6  //One byte receive complete in LIN mode
       ,emFrameHeaderComplete    = 0x7  //Frame header transmission/reception complete in LIN mode
       ,emRespSpaceComplete      = 0x8  //Respond space complete status
       ,emFinishReceiveData      = 0x9  //Finish receiving Data in Uart mode
       ,emFinishRevStopBit       = 0xA  //Finish receiving Stop Bit in Uart mode
       ,emFinishRev1Data         = 0xB  //Finish receiving 1st Data
       ,emFinishTrans1Data       = 0xC  //Finish transmission 1st Data
       ,emSuccessReceiveSYNC     = 0xD  //Success receive SYNC field status
       ,emNormalStatus           = 0xE  //Other from about status
       ,emIssueTransInt          = 0x10
       ,emSyncErrorDetect        = 0x11 //Sync field error // Add by SonTran: No.2-3 SYNC field error flag
       ,emFinishTransData        = 0x12 //Finish transmission status (last byte of data was transmitted)
    };
    
    enum eERROR_FLAG {                  //Enum describe operation errors
        emBitError               = 0x0  //Bit error
       ,emPhysicalError          = 0x1  //Physical error
       ,emTimeOutOrOverrunError  = 0x2  //Time out error in Lin mode or overrun error in uart mode
       ,emFrameError             = 0x3  //Frame error
       ,emSYNCErrorOrExpandBit   = 0x4  //SYNC error in Lin mode or expansion bit is detected in uart mode
       ,emChecksumErrorOrMatchID = 0x5  //Checksum error in Lin mode or ID match is detected in uart mode
       ,emIdentifyError          = 0x6  //ID error
       ,emRespondError           = 0x7  //Respond error in Lin modes
       ,emClockError             = 0x8  //Clock error
       ,emSampleFreqError        = 0x9  //Sample Frequency setting error
       ,emBitTimeError           = 0xa  //Bit time error
       ,emNoneError              = 0xb  //No error occurs
       ,emIdentifyErrorAndFramingError = 0xc // Add by SonTran: No.2-1 Also detect Frame error with receving Stop bit of PID
    };
    
    enum eINTERRUPT_KIND {              //Enum describe interrupt kinds
        emTransCompleteIntr   = 0x0     //Transmission complete interrupt
       ,emReceiveCompleteIntr = 0x1     //Receive complete interrupt 
       ,emStatusIntr          = 0x2     //Status interrupt
       ,emNoneIntr            = 0x3     //None interrupt
    };
    
    enum eTIME_ENUM {                   //Enum describe time units
        emNanoSecond  = 1000000000      //Nano second
       ,emBitTimeUnit = 100             //Bit time unit
    }; 

    enum eCHECKSUM_KIND {               //Enum describe checksum constant use for time out calculating
        emEnhancedChecksumConst  = 48   //Enhanced Checksum
       ,emClassicChecksumConst  = 49    //Classic Checksum
    };

    enum eHEADER_DATA {                 //Enum describe header data kinds
        emBeakLow   = 0x0               //Break low
       ,emIdleField = 0xFFFFFFFF        //Break high, idle, interbyte space
       ,emSYNCField = 0x55              //SYNC field
    };

    enum eHEADER_CONTROL {              //Enum describe header control kinds
        emIdleControl    = 0x0          //Idle
       ,emStartControl   = 0x1          //Start bit 
       ,emStopControl    = 0x2          //Stop bit
       ,emBeakLowControl = 0x3          //Break Low 
    };

    enum eHEADER_RECEPT_INDEX {         //Enum describe header reception index
        emBreakHigh        = 0x1        //Break high
       ,emStartSYNC        = 0x2        //Start SYNC field
       ,emStopSYNC         = 0x3        //Stop SYNC field
       ,emInterHeaderSpace = 0x4        //Inter header space
       ,emStartPID         = 0x5        //Start PID field
       ,emStopPID          = 0x6        //Stop PID field
       ,emRespondSpace     = 0x7        //Respond space
    };
    
    enum eREG_KIND {                    //Enum describe register which change when operating
        emDataReg0    = 0x0             //LUDB0
       ,emDataReg1    = 0x1             //LDB1
       ,emDataReg2    = 0x2             //LDB2
       ,emDataReg3    = 0x3             //LDB3
       ,emDataReg4    = 0x4             //LDB4
       ,emDataReg5    = 0x5             //LDB5
       ,emDataReg6    = 0x6             //LDB6
       ,emDataReg7    = 0x7             //LDB7
       ,emDataReg8    = 0x8             //LDB8
       ,emDataReg     = 0x9             //LURDR
       ,emChecksumReg = 0xA             //LCBR
       ,emBaudRateReg = 0xB             //LBRP0 and LBRP1
       ,emPIDReg      = 0xC             //PID
       ,emLBSSReg     = 0xD             //LBSS
    };

    enum eSELF_TEST_SEQ {               //Enum describe self test sequency value
        emFirstSeq   = 0xA7             //First sequency
       ,emSecondSeq  = 0x58             //Second sequency
       ,emLastSeq    = 0x01             //Last sequency
    };

    //structure
    struct RlinRegs {                   //Struct describe registers of RLIN3  
        unsigned int lwbr;              //LWBR   
        unsigned int lbrp0;             //LBRP0
        unsigned int lbrp1;             //LBRP1
        unsigned int lstc;              //LSTC
        unsigned int lmd;               //LMD
        unsigned int lbfc;              //LBFC
        unsigned int lsc;               //LSC
        unsigned int lwup;              //LWUP
        unsigned int lie;               //LIE
        unsigned int lede;              //LEDE
        unsigned int lcuc;              //LCUC
        unsigned int ltrc;              //LTRC
        unsigned int lmst;              //LMST
        unsigned int lst;               //LST
        unsigned int lest;              //LEST
        unsigned int ldfc;              //LDFC
        unsigned int lidb;              //LIDB
        unsigned int lcbr;              //LCBR
        unsigned int ludb0;             //LUDB0
        unsigned int ldb1;              //LDB1
        unsigned int ldb2;              //LDB2
        unsigned int ldb3;              //LDB3
        unsigned int ldb4;              //LDB4
        unsigned int ldb5;              //LDB5
        unsigned int ldb6;              //LDB6
        unsigned int ldb7;              //LDB7
        unsigned int ldb8;              //LDB8
        unsigned int luoer;             //LUOER 
        unsigned int luor1;             //LUOR1 
        unsigned int lutdr;             //LUTDR 
        unsigned int lurdr;             //LURDR 
        unsigned int luwtdr;            //LUWTDR
        unsigned int lurde;             //LURDE
        unsigned int lbss;              //LBSS
        unsigned int lrss;              //LRSS
        RlinRegs () {
            lwbr  = 0;
            lbrp0 = 0;
            lbrp1 = 0;
            lstc  = 0;
            lmd   = 0;
            lbfc  = 0;
            lsc   = 0;
            lwup  = 0;
            lie   = 0;
            lede  = 0;
            lcuc  = 0;
            ltrc  = 0;
            lmst  = 0;
            lst   = 0;
            lest  = 0;
            ldfc  = 0;
            lidb  = 0;
            lcbr  = 0;
            ludb0 = 0;
            ldb1  = 0;
            ldb2  = 0;
            ldb3  = 0;
            ldb4  = 0;
            ldb5  = 0;
            ldb6  = 0;
            ldb7  = 0;
            ldb8  = 0;
            luoer = 0;
            luor1 = 0;
            lutdr = 0;
            lurdr = 0;
            luwtdr = 0;
            lurde  = 0;
            lbss   = 0;
            lrss   = 0;
        }
    };

    // method in common class
    void SetSelftestMode(bool is_selftest); //this API use to set rlin common to selftest mode
    void StopOperation(bool is_hw_reset); //this API will use for stopping operation of common
    void SetDataTransfer(unsigned int index, unsigned int data); //input data transfer
    void SetLinClock(double clkc, double pclk); // this API use for update Lin clock value to common and cancel operation if clock input is zero
    bool CheckZeroClock(); //check zero clock and dump warning if input signal assert when clock is equal zero
    void SetConfigFactors(RlinRegs config_struct); //set config registers to common
    void TransmitProcess(eTRANS_STATUS status); //this function do transfer operation
    void ReceptionProcess(unsigned int data_input, unsigned int control_input, eTRANS_STATUS  status);
    unsigned int CalcBitTime(); //this function update bit time unit 100ns base on each mode
   
    //virtual functions 
    virtual void OutputData(unsigned int tx_control, unsigned int tx_data) = 0;//virtual function to output data to Master
    virtual void UpdateStatus(eSTATUS_FLAG flag) = 0; //virtual function to update status register and output interrupt if any  
    virtual void UpdateErrorStatus(eERROR_FLAG flag) = 0; //virtual function to update error status register  
    virtual void UpdateRegisters(eREG_KIND reg_kind, unsigned int value) = 0;//virtual function for updating baud rate or data register
    virtual void _re_printf(const std::string group, const char *message, ...) = 0;//virtual function for outputing debug message

    //Constructor
    SC_HAS_PROCESS(Crlin3_common);
    Crlin3_common(sc_module_name name);
    //Destructor
    ~Crlin3_common();

protected:
    //event
    sc_event mTransmitEvent;            //Event use to notify TransmitDataMethod        
    sc_event mWaitToPhyscBusErrorEvent; //Event use to notify WaitToPhyscBusErrorMethod
    sc_event mWaitToBitErrorEvent;      //Event use to notify WaitToBitErrorMethod
    sc_event mWriteOutputEvent;         //Event use to notify WriteOutputMethod
    sc_event mCheckBitErrorEvent;       //Event use to notify CheckBitErrorMethod
    sc_event mReceptionCompleteEvent;   //Event use to notify ReceptionCompleteMethod //Add by UyenLe : No.10, 11 Successful Wake-up reception Flag and interrupt set timing (3.0Tbits after receive wake-up signal) (RLIN3v4)

    //variables
    unsigned int mReception_index;      //Indicate reception index, increase when recept data input
    unsigned int mTransmission_index;   //Indicate transmission index, increase when transmission data output
    unsigned int mOut_data[32] ;        //Array store output data
    unsigned int mOut_control[32] ;     //Array store output control data
    double mTransfer_period[32]; //Array store transfer time of each data
    unsigned int mReceive_data;
    eTRANS_STATUS mTrans_status;        //Indicate current status (wake-up, header ,respond)
    eTRANS_STATUS mReceive_status;      //Indicate current status (wake-up, header ,respond)
    eSTATUS_FLAG mCurrent_status;       //Indicate current transmission/reception status 
    eERROR_FLAG mCurrent_Error;         //Indicate current error
    unsigned int mData_transfer[9];     //Array store data of data registers of RLIN3 
    unsigned int mBit_time;             //Current bit time value
    RlinRegs mConfigOperation;          //Store value of register of RLIN3
    bool mIsMatch;                      //Match expension bit or match ID in uart mode
    bool mIsBitError;                   //Bit error occurs
    double mCurrent_Clock;              //Current counter clock
    double mPclk_Clock;                 //Current pclk clock
    unsigned int mWrite_time;           //Write times
    unsigned int mRx_data;              //Newest value of RX_DATA
    bool mSelf_test;                    //Indicate selftest mode or normal mode
    unsigned int mPreTransChecksumVal;  //Indicate checksum value of all of previous transmission data group // Add by SonTran: No.1-3,1-4 Checksum is only transmitted at the end of last group data 
    unsigned int mPreRecvChecksumVal;   //Indicate checksum value of all of previous reception data group // Add by SonTran: No.1-3,1-4 Checksum is only transmitted at the end of last group data 
    double mStartRecvBreakLow;          //Indicate time when start receiving break low // Add by SonTran: No.2-5 Break low less than configuration

    //methods
    void TransmitDataMethod();   //this method is wait to transfer 
    void WaitToBitErrorMethod(); //this method is wait to bit boundary to assert error flag
    void WaitToPhyscBusErrorMethod(); //this method is wait to bit boundary to assert error flag
    void WriteOutputMethod();    //this method use to write output port
    void CheckBitErrorMethod();  //this method use to check bit error
    void ReceptionCompleteMethod();  //this method use to handle reception complete wake-up //Add by UyenLe : No.10, 11 Successful Wake-up reception Flag and interrupt set timing (3.0Tbits after receive wake-up signal) (RLIN3v4)

    //functions
    void Initialize(); // this function will use for initializing variables of common
    void TransmitWakeup(); //this function is called to transmit wake up signal
    void TransmitHeaderLoop(); //this function is called each times of write output control port when transmit frame header
    void TransmitRespLoop(); //this function is called each times of write output control port when transmit respond data
    void RespReception(unsigned int data_input, unsigned int control_input, unsigned int mode); //this function is used to process reception data
    void HeaderReception(unsigned int data_input, unsigned int control_input, unsigned int mode); //this function is used to process reception header in slave mode
    void FinishReceiveHandle(unsigned int mode, unsigned int index); //this function is used to handle process when finish receive stop bit 
    unsigned int CalcChecksumValue (unsigned int pre_checksum_val); //this function returns checksum value base on transfer data and kind of checksum
    unsigned int CalcBaudRate(unsigned int bit_time); //this function returns the number of baud rate setting from bit time
    bool CheckIDParity (unsigned int PID); //this function will check wether ID parity is correct not 
    unsigned int GetParity (unsigned int data,unsigned int bit_length);
    unsigned int CalcNumOfByte(eTRANS_STATUS  status); // this function returns number of bytes 
    double CalcBitBoundary(unsigned int max_bit, unsigned int input_data, unsigned int expected_data); //this function returns time to bit error occurs
    bool CheckBreakLowPeriod(unsigned int mode); //this function returns defined break low period
    double GetCurTime(void);
};
#endif //__RLIN3_COMMON_H__
