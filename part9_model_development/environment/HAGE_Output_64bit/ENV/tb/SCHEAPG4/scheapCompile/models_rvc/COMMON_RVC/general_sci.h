// ----------------------------------------------------------------------
// Status    :
//     $Id: general_sci.h,v 1.3 2011/07/29 11:14:04 imotomg Exp $
// ----------------------------------------------------------------------
//   (C) Copyright 2009   RVC (Renesas Design Vietnam Co., Ltd.)
//   All rights reserved. RVC Confidential Proprietary
//   (C) Copyright 2009   RENESAS Technology Corp.  All rights reserved.
// ----------------------------------------------------------------------
#ifndef _C_GENERALSCI_H_
#define _C_GENERALSCI_H_

#include <systemc.h>
#include <queue>
#include <sstream>

#include "re_define.h"

namespace vpcl
{
    class Cgeneral_sci;
    
    class Csci_channel : public sc_module
    {
        private:
        enum SciInterrupt
        {
            emReceiveDataReadyIntr = 0,    // interrupt condition is "receive data ready"
            emReceiveFifoFullIntr = 1,     // interrupt condition is "receive FIFO full"
            emTransmitDataStopIntr = 2,   // interrupt condition is "transmit data stop"
            emTransmitFifoEmptyIntr = 3   // interrupt condition is "transmit FIFO empty"
        };
    
        sc_event transmit_event;            // The event to trigger transmitting thread
        sc_event mRstEvent;
        std::queue <unsigned char> Tx_FIFO; //Transmit FIFO
        std::queue <unsigned char> Rx_FIFO; //Receive FIFO
        unsigned int transmit_count;        //data count of transmitting
        unsigned int receive_count;         //data count of receiving
        unsigned int Tx_FIFO_depth;         //The depth of Tx FIFO
        unsigned int Rx_FIFO_depth;         //The depth of Rx FIFO
        unsigned int Tx_FIFO_trigger;       //Upper threshold value of Tx FIFO
        unsigned int Tx_FIFO_trigger_temp;  //Store temporary the value of Tx_FIFO_trigger
        unsigned int Rx_FIFO_trigger;       //Upper threshold value of Rx FIFO
        unsigned int Rx_FIFO_trigger_temp;  //Store temporary the value of Rx_FIFO_trigger
        unsigned int Tx_stop;               //The counter of transmitting
        unsigned int Tx_stop_temp;          //Store temporary the value of Tx_stop
        unsigned int Tx_rate;               //Transmitting rate
    
        unsigned int ch_index;              //channel index
        
        bool Rx_ready_en;                   //On/off "Receive data ready" interrupt condition
        bool Rx_full_en;                    //On/off "Receive FIFO full" interrupt condition
        bool Tx_stop_en;                    //On/off "Transmit data stop" interrupt condition
        bool Tx_empty_en;                   //On/off "Transmit FIFO empty" interrupt condition
    
        bool transmit_status;               //Transmit status
        bool receive_status;                //Receive status
        bool enable_trans_info;            //Whether dump transmit information
        bool enable_receive_info;          //Whether dump receive information
        bool enable_interr_info;           //Whether dump interrupt information

        bool mIsReset;
    
        FILE *log_file; 
        std::ostringstream log_file_name;            //Name of log file for each channel
    
        Cgeneral_sci *generalsci_pointer;   //The pointer keeps Cgeneral_sci class
        //---------------
        public:
        SC_HAS_PROCESS (Csci_channel);
        Csci_channel (sc_module_name name, unsigned int ch_id, Cgeneral_sci *gsci):
        sc_module (name)
        {
            if(gsci != NULL){
                generalsci_pointer = gsci;
            }else{
                printf("[%s]Error:The pointer to Cgeneral_sci is NULL\n",this->name());
                sc_stop();
            }
            //Initialize variable
            Initialize(); 
            ch_index = ch_id;
            log_file = NULL; 
            log_file_name <<"sci_channel"<<ch_id<<".log";
    
            SC_THREAD (Transmit_Thread_wrapper);
            dont_initialize();
            sensitive << transmit_event;
            sensitive << mRstEvent;
            mIsReset = false;
        }
    
        ~Csci_channel () 
        {
            if(log_file != NULL){
                fclose(log_file);
            }
        }
    
        //Declare all public functions
        bool Set_TxFIFO_ch(const unsigned char data);
        void Set_RxFIFO_ch(const unsigned char data);
        unsigned char Get_RxFIFO_ch(void);
        bool Set_Parameter_ch(const char *para_name,const unsigned int data);
        bool Set_InterruptEnable_ch(const char *interrupt_name,const bool interrupt_enable); 
        unsigned int Tx_Count_ch(void);
        unsigned int Tx_FIFO_amount_ch(void);
        unsigned int Rx_FIFO_amount_ch(void);
        bool Is_Stop_ch(void);
        void Clear_ch(void);
        std::string handleCommand(const std::vector<std::string>& args);
        
        void EnableReset(bool is_active);

        private:
        //Declare all private functions
        void Transmit_Thread(void);
        void Transmit_Thread_wrapper(void);
        unsigned char Get_TxFIFO_ch(void);
        void DumpMess_transmit(void);
        void DumpMess_receive(void);
        void DumpMess_interr(const unsigned int interrupt_id);
        void Send_interrupt(const unsigned int interrupt_id,const unsigned int ch_index);
        void Initialize(bool is_constructor=true);
    };
    
    class Cgeneral_sci
    {
        private:
        //Declare variable
        unsigned int total_channel_number;
        std::string  instance_name;

        bool mIsReset;
        
        Csci_channel **channel;
    
        public:
        Cgeneral_sci (const std::string name,const unsigned int total_channel_number)
        {
            channel = new Csci_channel*[total_channel_number];
            for (unsigned int index=0 ; index< total_channel_number;index++){
                std::ostringstream str_tmp;
                str_tmp << "sci_channel" << index;
                channel[index] = new Csci_channel(str_tmp.str().c_str(),index,this);
            }
    
            this->total_channel_number = total_channel_number;
            instance_name = name;
            mIsReset=false;
        }
        
        virtual ~Cgeneral_sci ()
        {
            for(unsigned int index=0;index<total_channel_number;index++){
                delete channel[index];
            }
            delete [] channel;
        }
    
        //Declare public fucntions
        virtual bool ScifTxData(const unsigned char data,const unsigned int ch_number)=0;
        bool ScifRxData(const unsigned int size,const char *data,const unsigned int ch_number);
        unsigned int Tx_Count(const unsigned int ch_number);
        unsigned int Tx_FIFO_amount(const unsigned int ch_number);
        unsigned int Rx_FIFO_amount(const unsigned int ch_number);
        bool Set_TxFIFO(const unsigned char data,const unsigned int ch_number);
        unsigned char Get_RxFIFO(const unsigned int ch_number);
        virtual void Scif_interrupt(const unsigned int interrupt_id, const unsigned int ch_number)=0;
        bool Set_Parameter(const char *para_name,const unsigned int value,const unsigned int ch_number);
        bool Set_InterruptEnable(const char *interrupt_name,const bool interrupt_enable,const unsigned int ch_number);
        bool Gsci_Is_Stop(const unsigned int ch_number);
        void Gsci_Clear(const unsigned int ch_number);
        std::string Gsci_handleCommand (const std::vector<std::string>& args,const unsigned int ch_number);

        void EnableReset(bool is_active);
    };
}
#endif
// vim600: set foldmethod=marker :
