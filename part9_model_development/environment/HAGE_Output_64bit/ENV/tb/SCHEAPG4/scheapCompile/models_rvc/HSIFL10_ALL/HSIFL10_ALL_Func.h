// ---------------------------------------------------------------------
// $Id: HSIFL10_ALL_Func.h,v 1.5 2020/03/16 02:13:01 quynhnhunguyen Exp $
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
#ifndef __HSIFL10_ALL_FUNC_H__
#define __HSIFL10_ALL_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "hsifl10_all_regif.h"

class HSIFL10_ALL_AgentController;
class HSIFL10_ALL_Func: public Chsifl10_all_regif,
                public BusSlaveFuncIf
{
public:
    HSIFL10_ALL_Func (std::string name, HSIFL10_ALL_AgentController *HSIFL10_ALL_AgentControllerPtr);
    ~HSIFL10_ALL_Func (void);
    
    void Reset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);
    /// Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void UpdateRHSIFRXCMPST(const uint32_t frameType);                                //Rx Complete Status Register
    void UpdateRHSIFTXCMPST(const uint32_t frameType);                                //Tx Complete Status Register
    void UpdateRHSIFICCR();                                                           //Update ICLC Command Control Register
    void UpdateRHSIFRXICST(uint32_t RHSIFRXICST_val);                                 //Update Rx ICLC Command Status register
    void UpdateRHSIFTXRXCR(uint8_t posBit, bool isEnable);                            //Update RHSIFTXRXCR register
    void UpdateRHSIFL1SR(uint8_t posBit, bool isEnable);                              //Update RHSIFL1SR register
    void UpdateRHSIFCCR(uint8_t posBit, bool isEnable);                               //Update RHSIFCCR register
    void UpdateRHSIFLTXFRMRL1(uint8_t header, uint8_t payload, bool isEndbit);        //Update RHSIFLTXFRMRL1 register
    void UpdateRHSIFLTXFRMRL2(uint8_t headerL1, uint16_t headerL2);                   //Update RHSIFLTXFRMRL2 register
    void UpdateRHSIFLRXFRMRL1(uint8_t header, uint8_t payload, bool isEndbit);        //Update RHSIFLRXFRMRL1 register
    void UpdateRHSIFLRXFRMRL2(uint8_t headerL1, uint16_t headerL2);                   //Update RHSIFLRXFRMRL2 register
    void UpdateRHSIFTXERRST(uint32_t errorCode);                                      //Update RHSIFTXERRST register
    void UpdateRHSIFRXERRST(uint32_t errorCode);                                      //Update RHSIFRXERRST register
  void UpdateRHSIFTMDCR(uint8_t posBit, bool isEnable, bool isTurnOffTestMode);     //Update RHSIFTMDCR register
    uint32_t GetRHSIFTXCMPIE();                                                       //Return value of RHSIFTXCMPIE register
    uint32_t GetRHSIFRXCMPIE();                                                       //Return value of RHSIFRXCMPIE register
    uint8_t GetRHSIFRXICIE(uint8_t posBit);                                           //Return value of specify bit of RHSIFRXICIE register
    uint32_t GetRHSIFSPCR();                                                          //Get value of RHSIFSPCR register
    uint32_t GetRHSIFTXRXCR();                                                        //Get value of RHSIFTXRXCR register
    uint32_t GetRHSIFMDCR();                                                          //Get value of RHSIFMDCR register 
    uint32_t GetRHSIFICCR();                                                          //Get value of RHSIFICCR register
    uint32_t GetRHSIFRXICST();                                                        //Get value of RHSIFRXICST register
    uint32_t GetRHSIFPCR();                                                           //Get value of RHSIFPCR register
    uint32_t GetRHSIFL1SR();                                                          //Get value of RHSIFL1SR register
    uint32_t GetRHSIFCCR();                                                           //Get value of RHSIFCCR register
    uint32_t GetRHSIFSMCR();                                                          //Get value of RHSIFSMCR register
    uint32_t GetRHSIFTXERRIE();                                                       //Get value of RHSIFTXERRIE register
    uint32_t GetRHSIFRXERRIE();                                                       //Get value of RHSIFRXERRIE register
    uint32_t GetRHSIFTXERRST();                                                       //Get value of RHSIFTXERRST register
    uint32_t GetRHSIFRXERRST();                                                       //Get value of RHSIFRXERRST register
    uint32_t GetRHSIFTXCMPST();                                                       //Get value of RHSIFTXCMPST register
    uint32_t GetRHSIFRXCMPST();                                                       //Get value of RHSIFRXCMPST register
    uint32_t GetRHSIFLTXFRMRL1();                                                     //Get value of RHSIFLTXFRMRL1 register
    uint32_t GetRHSIFLRXFRMRL1();                                                     //Get value of RHSIFLRXFRMRL1 register
    uint32_t GetRHSIFLTXFRMRL2();                                                     //Get value of RHSIFLTXFRMRL2 register
    uint32_t GetRHSIFLRXFRMRL2();                                                     //Get value of RHSIFLRXFRMRL2 register
  uint32_t GetRHSIFTMDCR();                                                         //Get value of RHSIFTMDCR register
  
private:
    HSIFL10_ALL_AgentController *mHSIFL10_ALL_AgentController;
    std::string mModuleName;        //Module name

    /// Declare virtual functions of HSIFL10_ALL_regif class
  void cb_RHSIFMDCR_CTSEN(RegCBstr str);
  void cb_RHSIFSPCR_FMBR(RegCBstr str);
  void cb_RHSIFTMDCR_RXLPBK(RegCBstr str);
  void cb_RHSIFICCR_ITRG(RegCBstr str);
  void cb_RHSIFCCR_CTRG(RegCBstr str);
  void cb_RHSIFTXCMPSC_TCCL2(RegCBstr str);
  void cb_RHSIFTXERRSC_TERCSZ(RegCBstr str);
  void cb_RHSIFRXCMPSC_RCCL2(RegCBstr str);
  void cb_RHSIFRXERRSC_RERCSZ(RegCBstr str);
  void cb_RHSIFRXICSC_RICPA(RegCBstr str);
};
#endif //__HSIFL10_ALL_FUNC_H__

