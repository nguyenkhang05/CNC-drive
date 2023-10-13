// ----------------------------------------------------------------------
// Status    :
//     $Id: general_sci.cpp,v 1.5 2011/10/06 07:57:18 sontran Exp $
// ----------------------------------------------------------------------
//   (C) Copyright 2009   RVC (Renesas Design Vietnam Co., Ltd.)
//   All rights reserved. RVC Confidential Proprietary
//   (C) Copyright 2009   RENESAS Technology Corp.  All rights reserved.
// ----------------------------------------------------------------------
#include "general_sci.h"

using namespace std;

void vpcl::Csci_channel::Transmit_Thread_wrapper(void)
{
    while(true) {
        while(mIsReset) {
            wait();
        }
        try {
            Transmit_Thread();
        }
        catch(eResetException n) {
        }
    }
} 

//Transmitting data
void vpcl::Csci_channel::Transmit_Thread(void)
{                                  //{{{
    while(true){
        while(Tx_FIFO.size() != 0){
            unsigned char data = 0x0;
            wait((int)Tx_rate,SC_NS, mRstEvent);
            if(mIsReset == true) {
                throw emRstException;
            }

            data = Get_TxFIFO_ch();
            generalsci_pointer->ScifTxData(data,ch_index);
            transmit_count++;
            DumpMess_transmit();
            if(Tx_FIFO.size() < Tx_FIFO_trigger){//Check "Tx FIFO empty" interrupt condition
                if(Tx_empty_en != false){
                    Send_interrupt(emTransmitFifoEmptyIntr,ch_index);
                }
            }
            if(transmit_count >= Tx_stop){//Check "Transmit data stop" interrupt condition
                if(Tx_stop_en != false){
                    Send_interrupt(emTransmitDataStopIntr,ch_index);
                }
                transmit_status =false; // Transmitting status is stop
                break;
            }
        }
        wait();
        if(mIsReset == true) {
            throw emRstException;
        }
    }
}                                  //}}}

//Tranfer data to Tx FIFO
bool vpcl::Csci_channel::Set_TxFIFO_ch(const unsigned char data)
{                                   //{{{
    bool status = true;
    if(transmit_count < Tx_stop){//Check stop condition
        transmit_status = true;
        Tx_FIFO.push(data);
        if(Tx_FIFO.size() < Tx_FIFO_trigger){//Check "Tx FIFO empty" interrupt condition
            if(Tx_empty_en != false){
                Send_interrupt(emTransmitFifoEmptyIntr,ch_index);
            }
        }
        transmit_event.notify();
    }else{
        status = false;
        printf("[%s]Warning:Transmit data reaches Tx_stop = %d\n ",this->name(),Tx_stop);
    }
    return status;
}                                   //}}}

//Get data from Tx FIFO
unsigned char  vpcl::Csci_channel::Get_TxFIFO_ch(void)
{                                  //{{{
    unsigned char data = 0x0;
    data = Tx_FIFO.front();
    Tx_FIFO.pop();
    return data;
}                                  //}}}

//Tranfer data to Rx FIFO
void vpcl::Csci_channel::Set_RxFIFO_ch(const unsigned char data)
{                                  //{{{
    Rx_FIFO.push(data);
    receive_status = true;
    if(Rx_FIFO.size() >= Rx_FIFO_trigger){//Check "Rx FIFO full" interrupt condition
        if(Rx_full_en != false){
            Send_interrupt(emReceiveFifoFullIntr,ch_index);
        }
    }else{//Check "Receive data ready" interrupt condition
        if(Rx_ready_en != false){
            Send_interrupt(emReceiveDataReadyIntr,ch_index);
        }
    }
    if(Rx_FIFO.size() > Rx_FIFO_depth){//Check "Overrun error" condition
        printf("[%s]Error:Overrun is occur\n",this->name());
    }
}                                   //}}}

//Get data from Rx FIFO
unsigned char vpcl::Csci_channel::Get_RxFIFO_ch(void)
{                                  //{{{ 
    unsigned char data = 0x0;
    if(Rx_FIFO.size() != 0x0){
        data = Rx_FIFO.front();
        Rx_FIFO.pop();
        receive_count++;
        DumpMess_receive();
        if(Rx_FIFO.size() >= Rx_FIFO_trigger){//Check "Rx FIFO full" interrupt condition
            if(Rx_full_en != false){
                Send_interrupt(emReceiveFifoFullIntr,ch_index);
            }
        }else{//Check "Receive data ready" interrupt condition
            if(Rx_ready_en != false){
                Send_interrupt(emReceiveDataReadyIntr,ch_index);
            }
        }
        if(Rx_FIFO.size() == 0){//Rx FIFO is empty
            receive_status = false;
        }
    }else{
        printf("[%s]Error:Prohibit read Rx FIFO when Rx FIFO is empty\n",this->name()); 
    }
    return data;
}                                  //}}}

//Set the value for parameter
bool vpcl::Csci_channel::Set_Parameter_ch(const char *para_name,const unsigned int data)
{                                  //{{{
    bool status = true;
    if(para_name != NULL){
        if((transmit_status == false) && (receive_status == false)){
            if(strcmp(para_name,"Tx_FIFO_trigger")==0){
                Tx_FIFO_trigger = data;
                Tx_FIFO_trigger_temp = data; //Store the value of Tx_FIFO_trigger
                if(Tx_FIFO_trigger_temp > Tx_FIFO_depth){
                    Tx_FIFO_trigger = 1;
                    printf("[%s]Warning:Tx_FIFO_trigger is set to %d that is bigger than Tx_FIFO_depth = %d\n,so GeneralSCI specified Tx_FIFO_trigger is %d\n",this->name(),Tx_FIFO_trigger_temp,Tx_FIFO_depth,Tx_FIFO_trigger);
                }
            }else if(strcmp(para_name,"Rx_FIFO_trigger")==0){
                Rx_FIFO_trigger = data;
                Rx_FIFO_trigger_temp = data; //Store the value of Tx_FIFO_trigger
                if(Rx_FIFO_trigger_temp > Rx_FIFO_depth){
                    Rx_FIFO_trigger = Rx_FIFO_depth;
                    printf("[%s]Warning:Rx_FIFO_trigger is set to %d that is bigger than Rx_FIFO_depth = %d\n,so GeneralSCI specified Rx_FIFO_trigger is %d as the same value of Rx_FIFO_depth\n",this->name(),Rx_FIFO_trigger_temp,Rx_FIFO_depth,Rx_FIFO_trigger);
                }
            }else if(strcmp(para_name,"Tx_stop")==0){
                Tx_stop = data;
                Tx_stop_temp = data; //Store the value of Tx_stop
                if(Tx_stop_temp > Tx_FIFO_depth){
                    Tx_stop = Tx_FIFO_depth;
                    printf("[%s]Warning:Tx_stop is set to %d that is bigger than Tx_FIFO_depth = %d\n,so GeneralSCI specified Tx_stop is %d as the same value of Tx_FIFO_depth\n",this->name(),Tx_stop_temp,Tx_FIFO_depth,Tx_stop);
                }
            }else if(strcmp(para_name,"Tx_rate")==0){
                Tx_rate = data;
            }else if(strcmp(para_name,"Tx_FIFO_depth")==0){
                Tx_FIFO_depth = data;
                if(Tx_stop_temp > Tx_FIFO_depth){
                    Tx_stop = Tx_FIFO_depth;
                    printf("[%s]Warning:Tx_FIFO_depth is set to %d that is less than Tx_stop = %d\n,so GeneralSCI specified Tx_stop is %d as the same vale of Tx_FIFO_depth\n",this->name(),Tx_FIFO_depth,Tx_stop_temp,Tx_stop);
                }else{
                    Tx_stop = Tx_stop_temp;
                }
                if(Tx_FIFO_trigger_temp > Tx_FIFO_depth){
                    Tx_FIFO_trigger = 1;
                    printf("[%s]Warning:Tx_FIFO_depth is set to %d that is less than Tx_FIFO_trigger = %d\n,so GeneralSCI specified Tx_FIFO_trigger is %d\n",this->name(),Tx_FIFO_depth,Tx_FIFO_trigger_temp,Tx_FIFO_trigger);
                }else{
                    Tx_FIFO_trigger = Tx_FIFO_trigger_temp;
                }
            }else if(strcmp(para_name,"Rx_FIFO_depth")==0){
                Rx_FIFO_depth = data;
                if(Rx_FIFO_trigger_temp > Rx_FIFO_depth){
                    Rx_FIFO_trigger = Rx_FIFO_depth;
                    printf("[%s]Warning:Rx_FIFO_depth is set to %d that is less than Rx_FIFO_trigger = %d\n,so GeneralSCI specified Rx_FIFO_trigger is %d as the same vale of Rx_FIFO_depth\n",this->name(),Rx_FIFO_depth,Rx_FIFO_trigger_temp,Rx_FIFO_trigger);
                }else{
                    Rx_FIFO_trigger = Rx_FIFO_trigger_temp;
                }
            }else{
                status = false;
                printf("[%s]Error:Parameter name %s is wrong\n",this->name(),para_name);
            }
        }else{
            status = false;
            printf("[%s]Error:Prohibit setting %s when %s is running\n",this->name(),para_name,basename());
        }
    }
    return status;
}                                  //}}}

//Set interrupt enable
bool vpcl::Csci_channel::Set_InterruptEnable_ch(const char *interrupt_name,const bool interrupt_enable)
{                                  //{{{
    bool status = true;
    if(interrupt_name != NULL){
        if((transmit_status == false) && (receive_status == false)){
            if(strcmp(interrupt_name,"Rx_ready_en")==0){
                Rx_ready_en = interrupt_enable;
            }else if(strcmp(interrupt_name,"Rx_full_en")==0){
                Rx_full_en = interrupt_enable;
            }else if(strcmp(interrupt_name,"Tx_stop_en")==0){
                Tx_stop_en = interrupt_enable;
            }else if(strcmp(interrupt_name,"Tx_empty_en")==0){
                Tx_empty_en = interrupt_enable;
            }else{
                status = false;
                printf("[%s]Error:The name of interrupt condition is wrong\n",this->name());
            }
        }else{
            status = false;
            printf("[%s]Error:Prohibit changing interrupt enable  when %s is running\n",this->name(),this->name());
        }
    }
    return status;
}                                  //}}}

//Get transmitting data count
unsigned int vpcl::Csci_channel::Tx_Count_ch(void)
{                                  //{{{
    return transmit_count;
}                                  //}}}

//Get remained data in Tx FIFO
unsigned int vpcl::Csci_channel::Tx_FIFO_amount_ch(void)
{                                  //{{{
    return Tx_FIFO.size();
}                                  //}}}

//Get remained data in Rx FIFO
unsigned int vpcl::Csci_channel::Rx_FIFO_amount_ch(void)
{                                  //{{{
    return Rx_FIFO.size();
}                                  //}}}

//Get sci_channel running status
bool  vpcl::Csci_channel::Is_Stop_ch(void)
{                                  //{{{
    bool status = false;
    if((transmit_status == false) && (receive_status == false)){
        status = true;
    }
    return status;
}                                  //}}}

//Clear transmit/receive information
void vpcl::Csci_channel::Clear_ch(void)
{                                  //{{{
    transmit_count = 0;
    receive_count = 0;
}                                  //}}}

//Set whether display information
string vpcl::Csci_channel::handleCommand(const vector<string>& args)
{                                  //{{{
    ostringstream err_msg;
    ostringstream suc_msg;
    string help_msg = "\n    enable_trans_info <yes/no>: Display transmit count (Default: no)\n";
    help_msg = help_msg+"    enable_receive_info <yes/no>: Display receive count (Default: no)\n";
    help_msg = help_msg+"    enable_interr_info <yes/no> : Display interrupt information (Default: no)\n";
    if(args[0]=="enable_trans_info"){
        if(args.size() == 2){
            if(args[1] == "yes"){
                enable_trans_info = true;
                if(log_file == NULL){
                    log_file = fopen(log_file_name.str().c_str(),"w");
                    if(log_file == NULL){
                        printf("[%s]The pointer to log file is NULL\n",this->name());
                    }
                }
                suc_msg << "Success (" << this->name() << ") [" << args[0] << "<" << args[1] << ">]\n";
                return suc_msg.str().c_str(); 
            }else if(args[1] == "no"){
                enable_trans_info = false;
                suc_msg << "Success (" << this->name() << ") [" << args[0] << "<" << args[1] << ">]\n";
                return suc_msg.str().c_str();                
            }else{
                err_msg << "Error (" << this->name() << ") yes/no argument is only [" << args[0] << "<yes/no>]\n";
                return err_msg.str().c_str();
            }
        }else{
            err_msg << "Error (" << this->name() << ") " << args[0] << " needs argument\n";
            return err_msg.str().c_str();
        }
    }else if (args[0] == "enable_receive_info"){
        if(args.size() == 2){
            if(args[1]=="yes"){
                enable_receive_info = true;
                if(log_file == NULL){
                    log_file = fopen(log_file_name.str().c_str(),"w");
                    if(log_file == NULL){
                        printf("[%s]The pointer to log file is NULL\n",this->name());
                    }
                }
                suc_msg << "Success (" << this->name() << ") [" << args[0] << "<" << args[1] << ">]\n";
                return suc_msg.str().c_str(); 
            }else if(args[1] == "no"){
                enable_receive_info = false;
                suc_msg << "Success (" << this->name() << ") [" << args[0] << "<" << args[1] << ">]\n";
                return suc_msg.str().c_str(); 
            }else{
                err_msg << "Error (" << this->name() << ") yes/no argument is only [" << args[0] << "<yes/no>]\n";
                return err_msg.str().c_str();
            }
        }else{
            err_msg << "Error (" << this->name() << ") " << args[0] << " needs argument\n";
            return err_msg.str().c_str();
        }
    }else if (args[0] == "enable_interr_info"){
        if(args.size() == 2){
            if(args[1] == "yes"){
                enable_interr_info = true;
                suc_msg << "Success (" << this->name() << ") [" << args[0] << "<" << args[1] << ">]\n";
                return suc_msg.str().c_str(); 
            }else if(args[1] == "no"){
                enable_interr_info = false;
                suc_msg << "Success (" << this->name() << ") [" << args[0] << "<" << args[1] << ">]\n";
                return suc_msg.str().c_str(); 
            }else{
                err_msg << "Error (" << this->name() << ") yes/no argument is only [" << args[0] << "<yes/no>]\n";
                return err_msg.str().c_str();
            }
        }else{
            err_msg << "Error (" << this->name() << ") " << args[0] << " needs argument\n";
            return err_msg.str().c_str();
        }
    }else if(args[0] == "help"){
        return help_msg;
    }else{
        err_msg << "Error (" << this->name() << ") " << args[0] << " is not supported parameter\n";
        return err_msg.str().c_str();
    }
}                                   //}}}

//Dump transmitting information
void vpcl::Csci_channel::DumpMess_transmit(void)
{                                  //{{{
    if(enable_trans_info != false){
        if(log_file != NULL){
            fprintf(log_file,"[%s]transmit count = %d\n",this->name(),transmit_count);
        }
    }
}                                   //}}}

//Dump receiving information
void vpcl::Csci_channel::DumpMess_receive(void)
{                                  //{{{
    if(enable_receive_info != false){
        if(log_file != NULL){
            fprintf(log_file,"[%s]receive count = %d\n",this->name(),receive_count);
        }
    }
}                                  //}}}

//Dump interrupt information
void vpcl::Csci_channel::DumpMess_interr(const unsigned int interrupt_id)
{                                    //{{{
    if(enable_interr_info != false){   
        if(interrupt_id == 0){
            printf("[%s]Info:Receive data ready interrupt condition is active\n",this->name());
        }else if(interrupt_id == 1){
            printf("[%s]Info:Rx FIFO full interrupt condition is active\n",this->name());
        }else if(interrupt_id == 2){
            printf("[%s]Info:Transmit data stop interrupt condition is active\n",this->name());
        }else{
            printf("[%s]Info:Tx FIFO empty interrupt condition is active\n",this->name());
        }
    }
  }                                  //}}}

//Send the interrupt
void vpcl::Csci_channel::Send_interrupt(const unsigned int interrupt_id,const unsigned int ch_index)
{                                    //{{{
    generalsci_pointer->Scif_interrupt(interrupt_id,ch_index);
    DumpMess_interr(interrupt_id);
}                                    //}}}

//Initialize variables
//void vpcl::Csci_channel::Initialize(void)
void vpcl::Csci_channel::Initialize(bool is_constructor)
{                                   //{{{
    transmit_count = 0;
    receive_count = 0;
    transmit_status = false;
    receive_status = false;

    while(Tx_FIFO.size() > 0){
        Tx_FIFO.pop();
    }
    while(Rx_FIFO.size() > 0){
        Rx_FIFO.pop();
    }
    if(is_constructor == true) {
        Tx_FIFO_depth = 1;
        Rx_FIFO_depth = 1;
        Tx_FIFO_trigger = 1;
        Tx_FIFO_trigger_temp = 1;
        Rx_FIFO_trigger = 1;
        Rx_FIFO_trigger_temp = 1;
        Tx_stop = 1; 
        Tx_stop_temp = 1; 
        Tx_rate = 1;

        Rx_ready_en = false;
        Rx_full_en = false; 
        Tx_stop_en = false;
        Tx_empty_en = false;

        enable_trans_info = false;
        enable_receive_info = false;
        enable_interr_info = false;
    }
}                                   //}}}

void vpcl::Csci_channel::EnableReset(bool is_active)
{
     mIsReset = is_active ;
     if(mIsReset) {
         Initialize(false);
         transmit_event.cancel();
         transmit_event.notify();
         mRstEvent.notify();
     } else {
     }
}
//receiving data function. User calls this function to tranfer data to Rx FIFO
bool vpcl::Cgeneral_sci::ScifRxData(const unsigned int size,const char *data,const unsigned int ch_number)
{                                   //{{{
    bool status = true;
    if(data != NULL){
        if(ch_number < total_channel_number){
            for(unsigned int size_index=0;size_index<size;size_index++){
                channel[ch_number]->Set_RxFIFO_ch((unsigned char)data[size_index]);
            }
        }else{
            status = false;
            printf("[%s]Error:channel number = %d of ScifRxData function is illegal\n",instance_name.c_str(),ch_number);
        }
    }else{
        status = false;
        printf("[%s]Error:The pointer to data of ScifRxData function is NULL\n",instance_name.c_str());
    }
    return status;
}                                    //}}}

//Get transmiting data count. User calls this function to get transmitting data count
unsigned int vpcl::Cgeneral_sci::Tx_Count(const unsigned int ch_number)
{                                  //{{{
    if(ch_number < total_channel_number){
        return channel[ch_number]->Tx_Count_ch();
    }else{
        printf("[%s]Error:channel number = %d of Tx_Count function is illegal\n",instance_name.c_str(),ch_number);
        return 0;
    }
}                                  //}}}

//Get remained data in Tx FIFO. User calls this function to get remained data in Tx FIFO
unsigned int vpcl::Cgeneral_sci::Tx_FIFO_amount(const unsigned int ch_number)
{                                   //{{{
    if(ch_number < total_channel_number){
        return channel[ch_number]->Tx_FIFO_amount_ch();
    }else{
        printf("[%s]Error:channel number = %d of Tx_FIFO_amount function is illegal\n",instance_name.c_str(),ch_number);
        return 0;
    }
}                                  //}}}

//Get remained data in Rx FIFO. User calls this function to get remained data in Rx FIFO
unsigned int vpcl::Cgeneral_sci::Rx_FIFO_amount(const unsigned int ch_number)
{                                  //{{{
    if(ch_number < total_channel_number){
        return channel[ch_number]->Rx_FIFO_amount_ch();
    }else{
        printf("[%s]Error:channel number = %d of Rx_FIFO_amount function is illegal\n",instance_name.c_str(),ch_number);
        return 0;
    }
}                                  //}}}

//Tranfer data to Tx FIFO. User calls this function to tranfer data to Tx FIFO
bool vpcl::Cgeneral_sci::Set_TxFIFO(const unsigned char data,const unsigned int ch_number)
{                                  //{{{
    if(ch_number < total_channel_number){
        return channel[ch_number]->Set_TxFIFO_ch(data);
    }else{
        printf("[%s]Error:channel number = %d of Set_TxFIFO function is illegal\n",instance_name.c_str(),ch_number);
        return false;
    }
}                                  //}}}

//Get data from Rx FIFO. User calls this function to get data from Rx FIFO
unsigned char vpcl::Cgeneral_sci::Get_RxFIFO(const unsigned int ch_number)
{                                  //{{{
    if(ch_number < total_channel_number){
        return channel[ch_number]->Get_RxFIFO_ch();
    }else{
        printf("[%s]Error:channel number = %d of Get_RxFIFO function is illegal\n",instance_name.c_str(),ch_number);
        return 0;
    }
}                                  //}}}

//Set the value for parameters. User calls this function to set the value for parameters
bool vpcl::Cgeneral_sci::Set_Parameter(const char *para_name,const unsigned int value,const unsigned int ch_number)
{                                  //{{{
    bool status = false;
    if(para_name != NULL){
        if(ch_number < total_channel_number){
            status = channel[ch_number]->Set_Parameter_ch(para_name,value);
        }else{
            printf("[%s]Error:channel number = %d of Set_Parameter function is illegal\n",instance_name.c_str(),ch_number);
        }
    }else{
        printf("[%s]Error:The pointer to parameter name is NULL\n",instance_name.c_str());
    }
    return status;
}                                  //}}}

//Set interrupt enable. User calls this function to set interrupt enable
bool vpcl::Cgeneral_sci::Set_InterruptEnable(const char *interrupt_name,const bool interrupt_enable,const unsigned int ch_number)
{                                  //{{{
    bool status = false;
    if(interrupt_name != NULL){
        if(ch_number < total_channel_number){
            status = channel[ch_number]->Set_InterruptEnable_ch(interrupt_name,interrupt_enable);
        }else{
            printf("[%s]Error:channel number = %d of Set_InterruptEnable function is illegal\n",instance_name.c_str(),ch_number);
        }
    }else{
        printf("[%s]Error:The pointer to interrupt name is NULL\n",instance_name.c_str());
    }
    return status;
 }                                  //}}}

//Get GeneralSCI running status
bool vpcl::Cgeneral_sci::Gsci_Is_Stop(const unsigned int ch_number)
{                                   //{{{
    if(ch_number < total_channel_number){
        return channel[ch_number]->Is_Stop_ch();
    }else{
        printf("[%s]Error:channel number = %d of Gsci_Is_Stop function is illegal\n",instance_name.c_str(),ch_number);
        return false;
    }
}                                   //}}}

//Clear transmit/receive information
void vpcl::Cgeneral_sci::Gsci_Clear(const unsigned int ch_number)
{                                   //{{{
    if(ch_number < total_channel_number){
        channel[ch_number]->Clear_ch();
    }else{
        printf("[%s]Error:channel number = %d of Gsci_Clear function is illegal\n",instance_name.c_str(),ch_number);
    }
}                                   //}}}

//Set whether display information. User calls this function to set whether display information about transmitting, receiving, interrupt
string vpcl::Cgeneral_sci::Gsci_handleCommand(const vector<string>& args,const unsigned int ch_number)
{                                  //{{{
   if(ch_number < total_channel_number){
       return channel[ch_number]->handleCommand(args);
   }else{
       ostringstream error_message;
       error_message << instance_name << "Error:channel number = " << ch_number << " of Gsci_handleCommand function is illegal\n";
       return error_message.str().c_str();
   }
}                                  //}}}

void vpcl::Cgeneral_sci::EnableReset(bool is_active)
{
     mIsReset = is_active ;
     for(unsigned int i =0 ;i < total_channel_number;i++) {
        channel[i]->EnableReset(is_active);
     }
     //if(mIsReset) {
     //} else {
     //}
}

// vim600: set foldmethod=marker :
