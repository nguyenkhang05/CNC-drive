// ---------------------------------------------------------------------
// $Id: HSIFL10_ALL.h,v 1.8 2020/04/08 07:44:03 quynhnhunguyen Exp $
//
// Copyright(c) 2017-2020 Renesas Electronics Corporation
// Copyright(c) 2017-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __HSIFL10_ALL_H__
#define __HSIFL10_ALL_H__

#include "BusSlaveBase.h"
#include "HSIFL10_ALL_AgentController.h"
#include "HSIF_Common.h"
#include "rvc_common_model.h"

template<unsigned int BUSWIDTH> class TlmTargetSocket;
template<unsigned int BUSWIDTH> class TlmInitiatorSocket;

class HSIFL10_ALL_Func;
class HSIFL10_ALL:  public HSIFL10_ALL_AgentController,
                    public BusSlaveBase<32,1>,
                    public rvc_common_model,
                    public sc_module
{
private:
    HSIFL10_ALL_Func        *mHSIFL10_ALL_Func;                                          //HSIFL10_ALL_Func class instance
    L1Frame                 *mL1Frame;                                                   //L1Frame class instance
    L2Frame                 *mL2Frame;                                                   //L2Frame class instance
    uint8_t                 mHeaderL1;                                                   //Header L1
    uint16_t                mHeaderL2;                                                   //Header L2
    std::map<eLogicChanType, uint8_t >                 mLogicChanTypeMap;                //Logic channel type map: Logic channel type, Id of logic channel type
    std::map<uint8_t, std::pair<uint16_t, uint16_t> >  mPayloadSizeIndexMap;             //Payload size index map: payloadSizeInd, payloadSize, frameSize  
    std::map<uint8_t, std::string>                     mICLCCommandMap;                  //ICLC command
    std::map<uint8_t, uint16_t>                        mBaudrateFastMap;                 //Baudrate Fast map

    sc_dt::uint64             mPCLKPeriod;                //Period value of peripheral clock
    sc_dt::uint64             mPCLKFreq;                  //Frequency value of peripheral clock
    sc_dt::uint64             mPCLKOrgFreq;               //Original frequency of peripheral clock
    std::string               mPCLKFreqUnit;              //Frequency unit of peripheral clock

    sc_dt::uint64             mCCLKPeriod;                //Period value of communication clock 
    sc_dt::uint64             mCCLKFreq;                  //Frequency value of communication clock
    sc_dt::uint64             mCCLKOrgFreq;               //Original frequency of communication clock
    std::string               mCCLKFreqUnit;              //Frequency unit of communication clock

    sc_dt::uint64             mhsif_refclk_inPeriod;      //Period value of ref clock in 
    sc_dt::uint64             mhsif_refclk_inFreq;        //Frequency value of ref clock in
    sc_dt::uint64             mhsif_refclk_inOrgFreq;     //Original frequency of ref clock in
    std::string               mhsif_refclk_inFreqUnit;    //Frequency unit of ref clock in
    std::string               mhsif_version_name;         //Name of version

    sc_dt::uint64             mTimeResolutionValue;       //Time resolution value of simulation
    sc_time_unit              mTimeResolutionUnit;        //Time resolution unit of simulation
    
    bool                      mIsLoopBackMode;            //State model is loopback mode or not
    bool                      mIsTurnOffLBCommandICLC;    //State turn off loopback by ICLC command
    bool                      mIsFrameFromLinkPartner;    //Data frame from link partner
    bool                      mResetCurVal;               //Store current value of reset signals
    bool                      mResetPreVal;               //Store previous value of reset signals
    bool                      mIsResetHardActive;         //Reset operation status of reset signals
    bool                      mIsResetCmdReceive;         //Receive status of AssertReset command of reset signals
    bool                      mIsResetCmdActive;          //Active status of AssertReset command of reset signals
    double                    mResetCmdPeriod;            //Period of AssertReset command of reset signals
    double                    mTimePCLKChangePoint;       //The time clock is changed its value
    double                    mL2StartTransferTimePoint;
    double                    mL2CmpTransferTimePoint;  
    bool                      mDumpInterrupt;             //Enable/Disable dumping the interrupt info       
    bool                      mIsFreehsif_txd;            //Status of hsif_txd port is free or not
    bool                      mIsAutomaticFlowControl;    //In automatic flow control or not
    bool                      mIsFullRxFiFo;              //Rx FIFO full or not
    bool                      mIsReqTransManual;          //Request transmission in flow control manual is enable or not
    bool                      mIsReadRxFiFoEnable;        //Read Rx FIFO enalbe or not
    bool                      mIsPreReadRxFiFoEnable;     //Previous read Rx FIFO enalbe
    bool                      mIsSleepMode;               //Model in sleep mode or not
    bool                      mHsif_frame_cmp;            //Value which need write to hsif_frame_cmp port
    bool                      mHsif_tx_data_ready;        //Value which need write to hsif_tx_data_ready port
    bool                      mPre_Hsif_tx_data_ready;    //Prerequisite for hsif_tx_data_ready port
    bool                      mIsTxFifoFull;              //Status of Tx FIFO is full or not
    bool                      mIsTxFifoEmpty;             //Status of Tx FIFO is empty or not
    bool                      txcmp_irq_val;              //Value to update transmit complete interrupt port
    bool                      txerr_irq_val;              //Value to update transmit error interrupt port
    bool                      rxcmp_irq_val;              //Value to update receive complete interrupt port
    bool                      rxerr_irq_val;              //Value to update receive error interrupt port
    bool                      rxiclc_irq_val;             //Value to update receive ICLC interrupt port
    uint16_t                  mRxFiFoSize;                //The size of Rx FIFO
    uint32_t                  mHsifRxControlStartValue;   //Value which need write to hsif_rx_control when start transmission
    uint32_t                  mHsifRxControlEndValue;     //Value which need write to hsif_rx_control when end transmission
    uint32_t                  mReErrorCode;                              //The error code of Receiver
    uint32_t                  mTranErrorCode;                            //The error code of Transmitter
    eFrameType                mTxCurrentFrameType;        //Current frame type
    eTransDataControl         mTransDataControl;          //Type of transition data control
    eTransDataControl         mTransDataL2Control;        //Type of transition data control L2
    eTransDataControl         mTransDataLBControl;        //Type of transition data loopback control
    ePLLMode                  mCurPLLMode;                //Current PLL mode
    eFlowControlManualTpye    mFCMType;                   //Flow control manual type 
    bool                      mEnableRegisterMessage;

    ///Arbiter implement by a C++ set
    std::set<eFrameType>      mFrameTypeBuffer;           //Store frame type into a buffer
    ///Rx fifo implement by a C++ queue
    std::queue<L1Frame>       mReceiveFrameQueue;         //Store frame data at the receiver

    /// Declare events
    sc_event mResetHardEvent;                   //Call HandleResetHardMethod when reset is active
    sc_event mResetCmdEvent;                    //Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent;              //Call CancelResetCmdMethod when AssertReset is over
    sc_event mWriteOutputTransEvent;            //Event use to notify WriteOutputTransMethod
    sc_event mAssertTXCMP_IrqEvent;             //Event use to assert transmit complete interrupt
    sc_event mDeassertTXCMP_IrqEvent;           //Event use to de-assert transmit complete interupt
    sc_event mUpdateTXCMP_IrqEvent;             //Event use to update transmit complete interrupt port
    sc_event mAssertTXERR_IrqEvent;             //Event use to assert transmit error interrupt
    sc_event mDeassertTXERR_IrqEvent;           //Event use to de-assert transmit error interupt
    sc_event mUpdateTXERR_IrqEvent;             //Event use to update transmit error interrupt port
    sc_event mAssertRXCMP_IrqEvent;             //Event use to assert receive complete interrupt
    sc_event mDeassertRXCMP_IrqEvent;           //Event use to de-assert receive complete interupt
    sc_event mUpdateRXCMP_IrqEvent;             //Event use to update receive complete interrupt port
    sc_event mAssertRXERR_IrqEvent;             //Event use to assert receive error interrupt
    sc_event mDeassertRXERR_IrqEvent;           //Event use to de-assert receive error interupt
    sc_event mUpdateRXERR_IrqEvent;             //Event use to update receive error interrupt port
    sc_event mAssertRXICLC_IrqEvent;            //Event use to assert receive ICLC interrupt
    sc_event mDeassertRXICLC_IrqEvent;          //Event use to de-assert receive ICLC interupt
    sc_event mUpdateRXICLC_IrqEvent;            //Event use to update receive ICLC interrupt port
    sc_event mTransProcessEvent;                //Event use to notify TransProcess method
    sc_event mRecProcessEvent;                  //Event use to notify ReceiveProcess method
    sc_event mTransitDataControlEvent;          //Event use to notify TransitionDataControlMethod method
    sc_event mTransitDataL2ControlEvent;        //Event use to notify TransitionDataL2ControlMethod method
    sc_event mTransDataEvent;                   //Event use to notify TransDataMethod method
    sc_event mTransDataLoopBackTxRxEvent;       //Event use to notify TransDataLoopBackTxRxControlMethod method
    sc_event mTransitCompleteEvent;             //Event use to notify UpdateTransitCompletedMethod method
    sc_event mRecProcessL2FrameEvent;           //Event use to notify ReceiveProcessL2FrameMethod method

    //Event for comunication L2 to L1
    sc_event  mStartHandleL2FrmToL1Event;
    sc_event  mFinishHandleL2FrmToL1Event;
    sc_event  mAssertHsif_frame_cmpEvent;
    sc_event  mDeAssertHsif_frame_cmpEvent;
    sc_event  mWriteHsif_frame_cmpEvent;
    sc_event  mWriteHsif_frame_readyEvent;
    sc_event  mUpdateTxFifoFullStatusEvent;
    sc_event  mUpdateTxFifoNotFullStatusEvent;
    sc_event  mUpdateTxFifoEmptyStatusEvent;
    sc_event  mUpdateTxFifoNotEmptyStatusEvent;

public:
    SC_HAS_PROCESS(HSIFL10_ALL);
    HSIFL10_ALL(sc_module_name name,
        unsigned int rLatency,
        unsigned int wLatency,
        std::string  version_name
    );
    ~HSIFL10_ALL(void);

    //Input port
    sc_in<sc_dt::uint64> PCLK;
    sc_in<sc_dt::uint64> hsif_refclk_in;
    sc_in<sc_dt::uint64> CCLK;
    sc_in<bool> reset_rhsif_n;

    sc_in<L2Frame> hsif_tx_data;               //Receive data from HSIFL020 model
    sc_in<uint32_t> hsif_tx_control;           //It contain some control information as payload size index, data strobe, channel type (to Link partner)
    sc_in<L1Frame> hsif_rxd;                //Input from link partner 
    sc_in<uint32_t> hsif_rxd_control;          //It contain some control information as payload size index, data strobe, channel type (from Link partner)

    //Output port
    sc_out<L2Frame> hsif_rx_data;              //Transmit data to L2
    sc_out<uint32_t> hsif_rx_control;          //It contain some control information as payload size index, data strobe, channel type (to HSIFL020 model)
    sc_out<L1Frame>  hsif_txd;                 //Output frame to link partner 
    sc_out<uint32_t> hsif_txd_control;         //It contain some control information as payload size index, data strobe, channel type (to Link partner)
    sc_out<bool> hsif_tx_data_ready;           //Notify to L2 that L1 is ready receiving L2 frame
    sc_out<bool> hsif_frame_cmp;               //Notify to L2 that L1 finish receiving last byte of L2 frame

    //Output port interrupt
    sc_out<bool> int_hsif_txcmp;               //Indicate that transmission is completed
    sc_out<bool> int_hsif_txerr;               //Indicate that transmission is error
    sc_out<bool> int_hsif_rxcmp;               //Indicate that reception is completed
    sc_out<bool> int_hsif_rxerr;               //Indicate that reception is error
    sc_out<bool> int_hsif_rxiclc;              //Indicate that ICLC command is received

    //Output port clock
    sc_out<sc_dt::uint64> hsif_refclk_out;     //Reference clock out

    //Socket declaration
    TlmTargetSocket<32> *tsp;

    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const uint32_t reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const uint32_t reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);

    void Help (const std::string type);
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void DumpStatusInfo ();                            //Dump the status information of model
    void EnableRegisterMessage(const std::string is_enable);

private:
    /// Declare methods
    void HandlePCLKSignalMethod (void);                //Handle PCLK signal
    void HandleCCLKSignalMethod (void);                //Handle CCLK signal
    void Handlehsif_refclk_inSignalMethod (void);      //Handle hsif_refclk_in signal

    void HandlePresetzSignalMethod (void);
    void HandleResetHardMethod (void);
    void HandleResetCmdMethod (void);
    void CancelResetCmdMethod (void);

    /// Internal functions
    void Initialize ();
    void DumpInterruptMsg (const std::string intr_name, const bool value);
    void EnableReset (const std::string reset_name, const bool is_active);
    void CancelEvents (void);
    void RegisterHandler (const std::vector<std::string> cmd);
    bool GetResetStatus();
    bool CheckSetClockCorrect(const std::string clock_name);  //Check set clock correct or not
    sc_time  CalculateClockEdge();
    sc_time  CalTimeToCmplReceiveL2Frm();
    uint64_t CalBaudrate();
    uint64_t CalTimeToTransCmpL1Frm();                       //Calculate time for transition L1 frame data 
    uint16_t CalFrameLength();                               //Calculate length of frame

    //Comunication L2 to L1
    void HandleL2FrmtoL1CtrlSignalMethod();
    void StartHandleL2FrmToL1Method();
    void FinishHandleL2FrmToL1Method();
    void AssertHsif_frame_cmpMethod();
    void DeAssertHsif_frame_cmpMethod();
    void WriteHsif_frame_cmpMethod();
    void WriteHsif_frame_readyMethod();
    void UpdateTxFifoFullStatusMethod();
    void UpdateTxFifoNotFullStatusMethod();
    void UpdateTxFifoEmptyStatusMethod();
    void UpdateTxFifoNotEmptyStatusMethod();

    // For Transmitter L1
    void TransProcessMethod();               //The currenty frame type process
    void TransProcessICLC(uint32_t ICLC_val, eModelType modelType, eLogicChanType logicChanType);                  //Process trans for ICLC command
    void WriteOutputTransMethod();          //This method use to write output port transmistter
    void TransmitterCreateFrameL1(uint32_t ICLC_val, eModelType modelType, eLogicChanType logicChanType);          //Transmitter(Master) create Frame L1 send to Receiver(Slave)
    void TransmitterTransProcess();         //Transmitter process transmission to Receiver
    void StoreFrameTypeIntoBuffer(eFrameType frameType);                         //Store frame type into buffer mFrameTypeBuffer           
    void SetRefCLKOut(sc_dt::uint64 refCLKOut);                                  //Set reference clock out
    void RemoveCurrentFrameType();                                               //Remove current frame type
    void TransitionDataControlMethod();                                          //Transition data control in L1
    void TransitionDataL2ControlMethod();                                        //Transition data control to L2
    void TransDataMethod();                                                      //Transition data control from hsif_rxd to hsif_txd port in loopback mode
    void TransDataLoopBackTxRxControlMethod();                                   //Transition data control from hsif_txd to hsif_rxd port in loopback mode
    void UpdateTransitCompletedMethod();                                         //Update some information when transit completed
    bool CheckTxInterruptEnable();                                               //Check transmitter's interrupt is enable or not
    bool CheckRxInterruptEnable();                                               //Check receiver's interrupt is enable or not
    bool CheckExistPreFrameTypeBuffer(eFrameType frameType);                     //Check exist same frame type previously in buffer
    bool CheckShakingHand();                                                     //Check shanking hand between master and slave  
    sc_dt::uint64 GetCCLKFreq();                                                 //Get communication clock frequence
    sc_dt::uint64 GetRefclk_inFreq();                                            //Get reference clock in frequence

    // For Receiver L1
    void ReceiveProcessMethod();                                                 //This method use to handle data from Transmitter
    void ReceiveProcessICLC(uint32_t payloadL1, bool isICLCCommandValid, bool isCTS);        //This method use to handle data ICLC command from Transmitter
    void ReceiveProcessL2FrameMethod();                                          //This method use to handle data L2 frame from Transmitter
    void ICLCCommandProcess(uint32_t payloadL1);                                 //ICLC command process
    void SetFlowControlManualType(eFlowControlManualTpye flowControlManualType); //Set flow control manual type
    void StoreFrameDataIntoRxFiFoMethod();                                       //Store frame data into Rx FIFO
    bool CheckErrorFrameDataRx(L1Frame& frameData, uint8_t& headerL1, uint8_t& logicChan, uint32_t& payloadL1, 
                                        bool& isICLCCommandValid, bool& isCTS, bool& isEndBit);  //Check error exist in frame data Rx or not                       
    bool CheckFreeTransmission();                                                //This method check transmission is free or busy
    bool CheckClockPeriod (const std::string clock_name);
    L1Frame GetFrameDataFromRxFiFo();                                            //Get frame data into Rx FIFO
    HSIFL10_ALL_AgentController::eFlowControlManualTpye GetFlowControlManualType(); //Get flow control manual type

    //Update register
    void UpdateRHSIFICCR();                                                      //Update ICLC Command Control Register
    void UpdateRHSIFRXCMPST(eFrameType frameType);                               //Update Rx Complete Status Register
    void UpdateRHSIFTXCMPST(eFrameType frameType);                               //Update Tx Complete Status Register
    void UpdateRHSIFRXICST(uint32_t RHSIFRXICST_val);                            //Update Rx ICLC Command Status Register
    void UpdateRHSIFL1SR(uint8_t posBit, bool isEnable);                         //Update RHSIFL1SR register                                          
    void UpdateRHSIFTXRXCR(uint8_t posBit, bool isEnable);                       //Update RHSIFTXRXCR register                             
    void UpdateRHSIFCCR(uint8_t posBit, bool isEnable);                          //Update value RHSIFCCR register
    void UpdateRHSIFLTXFRMRL1(uint8_t header, uint8_t payload, bool isEndbit);   //Update last Tx L1 frame to RHSIFLTXFRMRL1 register
    void UpdateRHSIFLTXFRMRL2(uint8_t headerL1, uint16_t headerL2);              //Update last Tx L2 frame to RHSIFLTXFRMRL2 register
    void UpdateRHSIFLRXFRMRL1(uint8_t header, uint8_t payload, bool isEndbit);   //Update last Rx L1 frame to RHSIFLRXFRMRL1 register
    void UpdateRHSIFLRXFRMRL2(uint8_t headerL1, uint16_t headerL2);              //Update last Rx L2 frame to RHSIFLRXFRMRL2 register
    void UpdateRHSIFTXERRST(uint32_t errorCode);                                 //Update RHSIFTXERRST register
    void UpdateRHSIFRXERRST(uint32_t errorCode);                                 //Update RHSIFRXERRST register
    void UpdateRHSIFTMDCR(uint8_t posBit, bool isEnable, bool isTurnOffTestMode);//Update RHSIFTMDCR register
    //Get value of register
    uint32_t GetRHSIFSPCR();                                                     //Get value of RHSIFSPCR register
    uint32_t GetRHSIFRXICST();                                                   //Get Rx ICLC Command Status Register
    uint32_t GetRHSIFPCR();                                                      //Get value of RHSIFPCR register
    uint32_t GetRHSIFL1SR();                                                     //Get value of RHSIFL1SR register
    uint32_t GetRHSIFCCR();                                                      //Get value of RHSIFCCR register
    uint32_t GetRHSIFTXERRST();                                                  //Get value of RHSIFTXERRST register
    uint32_t GetRHSIFRXERRST();                                                  //Get value of RHSIFRXERRST register
    uint32_t GetRHSIFTXCMPST();                                                  //Get value of RHSIFTXCMPST register
    uint32_t GetRHSIFRXCMPST();                                                  //Get value of RHSIFRXCMPST register
    uint32_t GetRHSIFLTXFRMRL1();                                                //Get value of RHSIFLTXFRMRL1 register
    uint32_t GetRHSIFLRXFRMRL1();                                                //Get value of RHSIFLRXFRMRL1 register
    uint32_t GetRHSIFLTXFRMRL2();                                                //Get value of RHSIFLTXFRMRL2 register
    uint32_t GetRHSIFLRXFRMRL2();                                                //Get value of RHSIFLRXFRMRL2 register
    uint32_t GetRHSIFSMCR();                                                     //Get value of RHSIFSMCR register
    uint32_t GetRHSIFTXERRIE();                                                  //Get value of RHSIFTXERRIE register 
    uint32_t GetRHSIFRXERRIE();                                                  //Get value of RHSIFRXERRIE register
    uint32_t GetRHSIFTXRXCR();                                                   //Get value of RHSIFTXRXCR register
    uint32_t GetRHSIFMDCR();                                                     //Get Transmitter or receiver operation (Master or Slave): Mode Control Register
    uint32_t GetRHSIFICCR();                                                     //Get value of RHSIFICCR register to distinguish frame type of ICLC command
    uint32_t GetRHSIFTMDCR();                                                    //Get value of RHSIFTMDCR register
    uint8_t GetBitPosRHSIFRXICIE(uint8_t payLoad);                               //Get bit position of RHSIFRXICIE register

    // For interrupt
    void AssertTXCMP_IrqEventMethod();                                           //Assert Tx complete interrupt
    void DeassertTXCMP_IrqEventMethod();                                         //De-assert Tx complete interrupt  
    void UpdateTXCMP_IrqEventMethod();                                           //Update Tx complete interrupt
    void AssertTXERR_IrqEventMethod();                                           //Assert Tx error interrupt
    void DeassertTXERR_IrqEventMethod();                                         //De-assert Tx error interrupt  
    void UpdateTXERR_IrqEventMethod();                                           //Update Tx error interrupt
    void AssertRXCMP_IrqEventMethod();                                           //Assert Rx complete interrupt
    void DeassertRXCMP_IrqEventMethod();                                         //De-assert Rx complete interrupt  
    void UpdateRXCMP_IrqEventMethod();                                           //Update Rx complete interrupt
    void AssertRXERR_IrqEventMethod();                                           //Assert Rx error interrupt
    void DeassertRXERR_IrqEventMethod();                                         //De-assert Rx error interrupt  
    void UpdateRXERR_IrqEventMethod();                                           //Update Rx error interrupt
    void AssertRXICLC_IrqEventMethod();                                          //Assert Rx ICLC interrupt
    void DeassertRXICLC_IrqEventMethod();                                        //De-assert Rx ICLC interrupt  
    void UpdateRXICLC_IrqEventMethod();                                          //Update Rx ICLC interrupt
    void ClearInterrupt(std::string interrupt_name);                            ///Method to handle interrupt

public:
    bool GetReadRxFiFoEnable();                                                  //Get status of read Rx FIFO enable
    void SetReadRxFiFoEnable(bool isEnable);                                     //Set status of read Rx FIFO enable
    void IssueErrorFrame(std::string mode, uint32_t errorCode);                  //Inject Error into model
    std::string GetVersionName();                                                //Get version name
    void SetLoopBackMode(bool isLoopBack);                                       //Set loopback mode 
    bool GetTxDataReady();                                                       //Get Tx data ready which allow data frame from L2 to L1
    void SetTxDataReady(bool isTxDataReady);                                     //Set Tx data ready
};
#endif //__HSIFL10_ALL_H__
//
