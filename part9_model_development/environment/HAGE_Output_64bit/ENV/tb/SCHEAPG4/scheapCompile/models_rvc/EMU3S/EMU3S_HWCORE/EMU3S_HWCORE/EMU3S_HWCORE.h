// ---------------------------------------------------------------------
// $Id: EMU3S_HWCORE.h,v 1.4 2020/11/17 00:09:10 huyquocpham Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of .EMU3S_HWCORE.h
// Ref: {EMU3S_HWCORE_UD_Copyright_001}

#ifndef __EMU3S_HWCORE_H__
#define __EMU3S_HWCORE_H__
#include "global.h"
#include "rvc_common_model.h"
#include "BusSlaveBase.h"
#include "EMU3S_HWCORE_AgentController.h"

class EMU3S_HWCORE_Func;
class HWCORE_HANDSHAKE_Func;
class HWCORE_INPUT;
class HWCORE_IIR;
class HWCORE_MEASURE;
class HWCORE_RCTG;
class HWCORE_RSLVIDE;
class HWCORE_ADTRG;
class HWCORE_PI;
class HWCORE_PWM;
class HWCORE_IRCTG3;
class HWCORE_INT;

// Description: EMU3S_HWCORE model class
// Ref: {EMU3S_HWCORE_UD_ReferenceDoc_001, EMU3S_HWCORE_UD_Summary_001, EMU3S_HWCORE_UD_Feature_001} 
// Ref: {EMU3S_HWCORE_UD_Direction_001, EMU3S_HWCORE_UD_Direction_002}
class EMU3S_HWCORE:
      public sc_module
    , public EMU3S_HWCORE_AgentController
    , public BusSlaveBase<BUS_WIDTH_APB, 1>
    , public rvc_common_model
{
public:
    // Description: Port List
    // Ref: {EMU3S_HWCORE_UD_Feature_025, EMU3S_HWCORE_UD_Ports_001}
    //--------------------------------------
    //        I/O port definition
    //--------------------------------------
    // Description: Clock I/F
    // Ref: {EMU3S_HWCORE_UD_Ports_002}
    sc_in<sc_dt::uint64>                PCLK;
    sc_in<sc_dt::uint64>                CCLK;

    // Description: Reset I/F
    // Ref: {EMU3S_HWCORE_UD_Ports_002}
    sc_in<bool>                         PRESETn;
    sc_in<bool>                         CRESETn;

    // Description: AHB I/F
    // Ref: {EMU3S_HWCORE_UD_Ports_004}
    TlmInitiatorSocket<BUS_WIDTH_AHB>   ish;

    // Description: APB I/F
    // Ref: {EMU3S_HWCORE_UD_Ports_005}
    TlmTargetSocket<BUS_WIDTH_APB>      *tsp_cpu;

    // Description: EDC/Lockstep compare error I/F
    // Ref: {EMU3S_HWCORE_UD_Ports_007, EMU3S_HWCORE_UD_Feature_021, EMU3S_HWCORE_UD_Feature_022, EMU3S_HWCORE_UD_Feature_023}
    sc_out<unsigned char>               SFERC;
    sc_out<bool>                        edcerr_sed_pclk;
    sc_out<bool>                        edcerr_ded_pclk;
    sc_out<bool>                        edcerr_sed_cclk;
    sc_out<bool>                        edcerr_ded_cclk;

    // Description: A/D Converter Connection
    // Ref: {EMU3S_HWCORE_UD_Ports_008, EMU3S_HWCORE_UD_Ports_009, EMU3S_HWCORE_UD_Ports_010}
    sc_out<bool>                        adc_start;
    sc_in<bool>                         adc_grpend;
    sc_in<bool>                         adc_ch0end;
    sc_in<sc_uint<16> >                 adc_ch0dat;
    sc_in<bool>                         adc_ch1end;
    sc_in<sc_uint<16> >                 adc_ch1dat;
    sc_in<bool>                         adc_ch2end;
    sc_in<sc_uint<16> >                 adc_ch2dat;
    sc_out<bool>                        adc1_start;
    sc_in<bool>                         adc1_grpend;
    sc_in<bool>                         adc1_ch0end;
    sc_in<sc_uint<16> >                 adc1_ch0dat;
    sc_in<bool>                         adc1_ch1end;
    sc_in<sc_uint<16> >                 adc1_ch1dat;
    sc_in<bool>                         adc1_ch2end;
    sc_in<sc_uint<16> >                 adc1_ch2dat;

    // RDC I/F
    // Ref: {EMU3S_HWCORE_UD_Ports_011}
    sc_in<sc_uint<16> >                 rdc_phi;
    sc_in<bool>                         rdc_z;
    sc_in<sc_uint<26> >                 rdc_high_omega;
    sc_in<sc_uint<16> >                 rdc1_phi;
    sc_in<bool>                         rdc1_z;
    sc_in<sc_uint<26> >                 rdc1_high_omega;

    // Description: TSG3 Connection
    // Ref: {EMU3S_HWCORE_UD_Ports_013, EMU3S_HWCORE_UD_Ports_014, EMU3S_HWCORE_UD_Ports_015}
    sc_in<bool>                         tstpektim;
    sc_in<bool>                         tstvlytim;
    sc_in<bool>                         tstadt0;
    sc_in<bool>                         tstadt1;
    sc_in<bool>                         tstint3;
    sc_in<bool>                         tstint4;
    sc_out<sc_uint<18> >                tstwdcmp0;
    sc_out<sc_uint<18> >                tstwdcmpu;
    sc_out<sc_uint<18> >                tstwdcmpv;
    sc_out<sc_uint<18> >                tstwdcmpw;
    sc_out<bool>                        tstwecmp;
    sc_out<bool>                        tstiup;
    sc_out<bool>                        tstivp;
    sc_out<bool>                        tstiwp;

    // Description: Interrupt/Error Output
    // Ref: {EMU3S_HWCORE_UD_Ports_017}
    sc_out<bool>                        *emu_int[8];

private:
    // Pointer of EMU3S_HWCORE_Func
    EMU3S_HWCORE_Func                   *mpEMU3S_HWCORE_Func;
    // Description: HandShake registers function
    // Ref: {EMU3S_HWCORE_UD_Feature_020}
    HWCORE_HANDSHAKE_Func               *mpHWCORE_HANDSHAKE_Func;
    // Description: Sub-IPs instances
    // Ref: {EMU3S_HWCORE_UD_Feature_003, EMU3S_HWCORE_UD_Feature_005, EMU3S_HWCORE_UD_Feature_007}
    // Ref: {EMU3S_HWCORE_UD_Feature_008, EMU3S_HWCORE_UD_Feature_009, EMU3S_HWCORE_UD_Feature_010}
    // Ref: {EMU3S_HWCORE_UD_Feature_011, EMU3S_HWCORE_UD_Feature_014, EMU3S_HWCORE_UD_Feature_015}
    // Ref: {EMU3S_HWCORE_UD_Feature_017, EMU3S_HWCORE_UD_Feature_019, EMU3S_HWCORE_UD_Feature_020}
    // Ref: {EMU3S_HWCORE_UD_Feature_030}
    HWCORE_INPUT                        *mpINPUT;
    HWCORE_IIR                          *mpIIR;
    HWCORE_MEASURE                      *mpMEASURE;
    HWCORE_RCTG                         *mpRCTG;
    HWCORE_RSLVIDE                      *mpRSLVIDE;
    HWCORE_ADTRG                        *mpADTRG;
    HWCORE_PI                           *mpPI;
    HWCORE_PWM                          *mpPWM;
    HWCORE_IRCTG3                       *mpIRCTG3;
    HWCORE_INT                          *mpINT;

    sc_dt::uint64                       mPCLKPeriod;            //Period value of PCLK clock
    sc_dt::uint64                       mPCLKFreq;              //Frequency value of PCLK clock
    sc_dt::uint64                       mPCLKOrgFreq;           //Original frequency of PCLK clock
    std::string                         mPCLKFreqUnit;          //Frequency unit of PCLK clock

    sc_dt::uint64                       mCCLKPeriod;            //Period value of CCLK clock
    sc_dt::uint64                       mCCLKFreq;              //Frequency value of CCLK clock
    sc_dt::uint64                       mCCLKOrgFreq;           //Original frequency of CCLK clock
    std::string                         mCCLKFreqUnit;          //Frequency unit of CCLK clock

    sc_dt::uint64                       mTimeResolutionValue;   //Time resolution value of simulation
    sc_time_unit                        mTimeResolutionUnit;    //Time resolution unit of simulation

    // Description: Reset variables
    // Ref: {EMU3S_HWCORE_UD_ATT1_004}
    bool                                mIsPRESETnActive;         //Store current value of PRESETn
    bool                                mIsPRESETnHardActive;   //Reset operation status of PRESETn
    bool                                mIsPRESETnCmdReceive;   //Receive status of AssertReset command of PRESETn
    bool                                mIsPRESETnCmdActive;    //Active status of AssertReset command of PRESETn
    double                              mPRESETnCmdPeriod;      //Period of AssertReset command of PRESETn
    
    bool                                mIsCRESETnActive;         //Store current value of CRESETn
    bool                                mIsCRESETnHardActive;   //Reset operation status of CRESETn
    bool                                mIsCRESETnCmdReceive;   //Receive status of AssertReset command of CRESETn
    bool                                mIsCRESETnCmdActive;    //Active status of AssertReset command of CRESETn
    double                              mCRESETnCmdPeriod;      //Period of AssertReset command of CRESETn

    double                              mTimePCLKChangePoint;   //The time PCLK clock is changed its value
    double                              mTimeCCLKChangePoint;   //The time CCLK clock is changed its value
    bool                                mDumpInterrupt;
    bool                                mEnableRegisterMessage;
    
    bool                                mEDCErrSedPclk;
    bool                                mEDCErrDedPclk;
    bool                                mEDCErrSedCclk;
    bool                                mEDCErrDedCclk;
    
    bool                                mRCTGStartSP;
    bool                                mPWMStartSP;
    bool                                mPIStartSP;
    bool                                mINPUTStartSP;

    bool                                mFuncAWait;
    bool                                mFuncBWait;

    bool                                mRCTGEndSP;
    bool                                mINPUTEndSP;

    unsigned int                        mPWMTRGNUMCnt;
    unsigned int                        mPITRGNUMCnt;
    unsigned int                        mINTRGNUMCnt;
    unsigned int                        mADTRGNUMCnt;

    unsigned int                        mPWMFuncEnd_1Hot;
    unsigned int                        mPIFuncEnd_1Hot;
    unsigned int                        mINPUTFuncEnd_1Hot;
    unsigned int                        mRSLVIDEFuncEnd_1Hot;

    bool                                mAngComp0Active;
    bool                                mADEndTrigActive;
    bool                                mIIRCh0EndActive;
    bool                                mIIRCh1EndActive;
    bool                                mIIRCh2EndActive;

    bool                                mADCStart;

    bool                                mTstiup;
    bool                                mTstivp;
    bool                                mTstiwp;

    bool                                mTstwecmp;
    sc_uint<18>                         mTstwdcmp0;
    sc_uint<18>                         mTstwdcmpu;
    sc_uint<18>                         mTstwdcmpv;
    sc_uint<18>                         mTstwdcmpw;

    unsigned int                        FUNCIDLEGRPA0;        
    unsigned int                        FUNCIDLEGRPA1;
    unsigned int                        FUNCIDLEGRPA2;
    unsigned int                        FUNCWAITGRPA;
    unsigned int                        FUNCWAITGRPB;

    bool                                IsSWReset;

    /// Declare events                               
    sc_event                            mPRESETnSigWriteEvent;        //Call PRESETnSigWriteMethod when PRESETn is changed
    sc_event                            mPRESETnCmdEvent;             //Call HandlePRESETnCmdMethod when AssertReset of PRESETn is called
    sc_event                            mPRESETnCmdCancelEvent;       //Call CancelPRESETnCmdMethod when AssertReset of PRESETn is over
    sc_event                            mCRESETnResetEvent;           //Call CRESETnResetMethod when CRESETn is changed
    sc_event                            mCRESETnCmdEvent;             //Call HandleCRESETnCmdMethod when AssertReset of CRESETn is called
    sc_event                            mCRESETnCmdCancelEvent;       //Call CancelCRESETnCmdMethod when AssertReset of CRESETn is over
    sc_event                            mWriteSSCGSWResetEvent;       //Write rst_n signal
    sc_event                            mWriteCleanSWResetEvent;      //Write rst_clean_n signal
    
    sc_event                            mAssertEdcErrSedPclkEvent;    //Assert edcerr_sed_pclk output
    sc_event                            mDeassertEdcErrSedPclkEvent;  //Deassert edcerr_sed_pclk output
    sc_event                            mWriteEdcErrSedPclkEvent;  
    sc_event                            mAssertEdcErrDedPclkEvent;    //Assert edcerr_ded_pclk output
    sc_event                            mDeassertEdcErrDedPclkEvent;  //Deassert edcerr_ded_pclk output
    sc_event                            mWriteEdcErrDedPclkEvent;  
    sc_event                            mAssertEdcErrSedCclkEvent;    //Assert edcerr_sed_cclk output
    sc_event                            mDeassertEdcErrSedCclkEvent;  //Deassert edcerr_sed_cclk output
    sc_event                            mWriteEdcErrSedCclkEvent;  
    sc_event                            mAssertEdcErrDedCclkEvent;    //Assert edcerr_ded_cclk output
    sc_event                            mDeassertEdcErrDedCclkEvent;  //Deassert edcerr_ded_cclk output
    sc_event                            mWriteEdcErrDedCclkEvent;  
    
    sc_event                            mAssertRCTGStartSPEvent;      //Assert output_rctg_start_sp output
    sc_event                            mDeassertRCTGStartSPEvent;    //Deassert output_rctg_start_sp output
    sc_event                            mWriteRCTGStartSPEvent;  
    
    sc_event                            mAssertPWMStartSPEvent;       //Assert output_pwm_start_sp output
    sc_event                            mDeassertPWMStartSPEvent;     //Deassert output_pwm_start_sp output
    sc_event                            mWritePWMStartSPEvent;        
                                                                      
    sc_event                            mAssertPIStartSPEvent;        //Assert pi_start_sp output
    sc_event                            mDeassertPIStartSPEvent;      //Deassert pi_start_sp output
    sc_event                            mWritePIStartSPEvent;         
                                                                      
    sc_event                            mAssertINPUTStartSPEvent;     //Assert input_start_sp output
    sc_event                            mDeassertINPUTStartSPEvent;   //Deassert input_start_sp output
    sc_event                            mWriteINPUTStartSPEvent;  

    sc_event                            mAssertFuncAWaitEvent;        //Assert funca_wait_int output
    sc_event                            mDeassertFuncAWaitEvent;      //Deassert funca_wait_int output
    sc_event                            mWriteFuncAWaitEvent;         

    sc_event                            mAssertFuncBWaitEvent;        //Assert funcb_wait_int output
    sc_event                            mDeassertFuncBWaitEvent;      //Deassert funcb_wait_int output
    sc_event                            mWriteFuncBWaitEvent;

    sc_event                            mWriteADC0StartEvent;
    sc_event                            mWriteADC1StartEvent;

    sc_event                            mWriteTstiupEvent;
    sc_event                            mWriteTstivpEvent;
    sc_event                            mWriteTstiwpEvent;

    sc_event                            mWriteTstwecmpEvent;
    sc_event                            mWriteTstwdcmp0Event;
    sc_event                            mWriteTstwdcmpuEvent;
    sc_event                            mWriteTstwdcmpvEvent;
    sc_event                            mWriteTstwdcmpwEvent;

    sc_event                            mWriteCTREvent;
    sc_event                            mWriteREFCTREvent;
    sc_event                            mWriteAHBPRTEvent;
    sc_event                            mWriteTBLADREvent;
    sc_event                            mWriteIPTRGCTREvent;
    sc_event                            mWriteIPTRGNUMEvent;
    sc_event                            mWriteFUNCIDLEGRPA0Event;
    sc_event                            mWriteFUNCIDLEGRPA1Event;
    sc_event                            mWriteFUNCIDLEGRPA2Event;
    sc_event                            mWriteFUNCIDLEGRPBEvent;
    sc_event                            mWriteFUNCWAITGRPAEvent;
    sc_event                            mWriteFUNCWAITGRPBEvent;
    sc_event                            mWriteFUNCFLGRPA0Event;
    sc_event                            mWriteFUNCFLGRPA1Event;
    sc_event                            mWriteFUNCFLGRPA2Event;
    sc_event                            mWriteFUNCFLGRPBEvent;
    sc_event                            mWriteADSELEvent;
    sc_event                            mWriteSFERCEvent;

    sc_signal<sc_dt::uint64>            PCLK_sig;
    sc_signal<sc_dt::uint64>            CCLK_sig;
    sc_signal<bool>                     PRESETn_sig;
    sc_signal<bool>                     rst_n_sig;
    sc_signal<bool>                     rst_clean_n_sig;
    
    sc_signal<bool>                     rdc_out_z_sig;
    sc_signal<unsigned short>           rdc_out_phi_sig;
    sc_signal<signed int>               rdc_out_high_omega_sig;

    sc_signal<bool>                     adc_in_start_sig;
    sc_signal<bool>                     adc_out_grpend_sig;
    sc_signal<bool>                     adc_out_ch0end_sig;
    sc_signal<bool>                     adc_out_ch1end_sig;
    sc_signal<bool>                     adc_out_ch2end_sig;
    sc_signal<unsigned short>           adc_out_ch0dat_sig;
    sc_signal<unsigned short>           adc_out_ch1dat_sig;
    sc_signal<unsigned short>           adc_out_ch2dat_sig;

    sc_signal<bool>                     tstwecmp_sig;
    sc_signal<unsigned int>             tstwdcmp0_sig;
    sc_signal<unsigned int>             tstwdcmpu_sig;
    sc_signal<unsigned int>             tstwdcmpv_sig;
    sc_signal<unsigned int>             tstwdcmpw_sig;

    sc_signal<bool>                     tstpektim_sig;
    sc_signal<bool>                     tstvlytim_sig;
    sc_signal<bool>                     tstadt0_sig;
    sc_signal<bool>                     tstadt1_sig;
    sc_signal<bool>                     tstint3_sig;
    sc_signal<bool>                     tstint4_sig;
    
    // Signals for Connections between Top and Sub-IPs    
    sc_signal<bool>                     input_start_sp_sig;
    sc_signal<unsigned short>           FUNCIDLEGRPA0_sig;
    sc_signal<unsigned short>           FUNCWAITGRPA_sig;
    sc_signal<unsigned short>           FUNCFLGRPA0_sig;
    
    sc_signal<unsigned char>            input_func_end_sig;
    sc_signal<unsigned char>            input_func_st_sig;
    sc_signal<bool>                     input_func_wait_sig;
    sc_signal<bool>                     input_func_busy_sig;
    
    sc_signal<unsigned char>            CTR_sig;
    sc_signal<bool>                     output_rctg_start_sp_sig;
    
    sc_signal<unsigned short>           FUNCIDLEGRPB_sig;
    sc_signal<unsigned short>           FUNCWAITGRPB_sig;
    sc_signal<unsigned int>             FUNCFLGRPB_sig;
    
    sc_signal<unsigned short>           rslv_func_st_sig;
    sc_signal<unsigned short>           rslv_func_end_sig;
    sc_signal<bool>                     rslv_func_wait_sig;
    sc_signal<bool>                     rslv_func_busy_sig;

    sc_signal<bool>                     output_pwm_start_sp_sig;
    sc_signal<unsigned short>           FUNCIDLEGRPA2_sig;
    sc_signal<unsigned int>             FUNCFLGRPA2_sig;
    sc_signal<unsigned char>            REFCTR_sig;

    sc_signal<unsigned short>           pwm_func_end_sig;
    sc_signal<unsigned short>           pwm_func_st_sig;
    sc_signal<bool>                     pwm_func_wait_sig;
    sc_signal<bool>                     pwm_func_busy_sig;

    sc_signal<bool>                     ADSEL_sig;
    sc_signal<unsigned char>            IPTRGCTR_sig;
    sc_signal<unsigned int>             IPTRGNUM_sig;
    sc_signal<unsigned int>             TBLADR_sig;
    sc_signal<unsigned char>            AHBPRT_sig;
    
    sc_signal<unsigned char>            ADENDCTR_sig;
    sc_signal<unsigned char>            IPTRGMON_ADTRGNUM_sig;
    
    sc_signal<bool>                     pi_start_sp_sig;
    sc_signal<unsigned short>           FUNCIDLEGRPA1_sig;
    sc_signal<unsigned char>            FUNCFLGRPA1_sig;
    sc_signal<unsigned char>            IPTRG_sig;
    
    sc_signal<unsigned char>            pi_func_end_sig;
    sc_signal<unsigned char>            pi_func_st_sig;
    sc_signal<bool>                     pi_func_wait_sig;
    sc_signal<bool>                     pi_func_busy_sig;

    sc_signal<bool>                     funca_wait_int_sig;
    sc_signal<bool>                     funcb_wait_int_sig;
    
    // Signals for Connections between Sub-IPs
    sc_signal<unsigned short>           adtrg_ch0_dat_sig;
    sc_signal<unsigned short>           adtrg_ch1_dat_sig;
    sc_signal<unsigned short>           adtrg_ch2_dat_sig;
    sc_signal<unsigned short>           adtrg_thte_sig;
    sc_signal<unsigned short>           adtrg_thtr_sig;
    
    sc_signal<bool>                     input_end_sp_reg_sig;
    
    sc_signal<unsigned char>            em2ictrinmd_instctr_sig;
    sc_signal<unsigned short>           theta_e_sel_sig;
    sc_signal<signed int>               ia_v_lo_fix_reg_sig;
    sc_signal<signed int>               ia_w_lo_fix_reg_sig;
    sc_signal<signed int>               ia_u_lo_fix_reg_sig;
    
    sc_signal<signed int>               rdata_em2iidfixobuf_sig;
    sc_signal<signed int>               rdata_em2iiqfixobuf_sig;
    
    sc_signal<bool>                     em3ictrinmd_iirdq_sig;
    
    sc_signal<unsigned short>           rdata_em2ithtrefixin_sig;
    sc_signal<bool>                     kcl_int_sig;
    sc_signal<bool>                     in_ovfl_out_sig;
    sc_signal<unsigned char>            in_ovfl_func_sig;
    sc_signal<bool>                     in_zdfl_out_sig;
    
    sc_signal<unsigned short>           ia_1ch_lo_org_sel_sig;
    sc_signal<unsigned short>           ia_2ch_lo_org_sel_sig;
    sc_signal<unsigned short>           ia_3ch_lo_org_sel_sig;
    
    sc_signal<unsigned short>           rdata_em3iad0_sig;
    sc_signal<unsigned short>           rdata_em3iad1_sig;
    sc_signal<unsigned short>           rdata_em3iad2_sig;
       
    sc_signal<bool>                     adtrg_end_trig_sig;
    
    sc_signal<bool>                     iir_end0_sig;
    sc_signal<bool>                     iir_end1_sig;
    sc_signal<bool>                     iir_end2_sig;
    sc_signal<bool>                     iir_ovfl_out_sig;
    sc_signal<unsigned char>            iir_ovfl_func_sig;
    
    sc_signal<signed int>               iir_output0_sig;
    sc_signal<signed int>               iir_output1_sig;
    sc_signal<signed int>               iir_output2_sig;
    
    sc_signal<bool>                     presml_ang_comp0_eq_sp_sig;
        
    sc_signal<unsigned short>           rdata_em2iipcmp0_sig;
    sc_signal<bool>                     em2ibrecctr_slctcmp0_reg_sig;
    sc_signal<bool>                     wc_em2icmp0_sig;
    sc_signal<unsigned short>           em2icmp0_sig;
    sc_signal<bool>                     wc_em2icmp1_sig;
    sc_signal<unsigned short>           em2icmp1_sig;
    
    sc_signal<bool>                     output_rctg_end_sp_reg_sig;    
    
    sc_signal<bool>                     rctg_u_80m_reg_sig;
    sc_signal<bool>                     rctg_v_80m_reg_sig;
    sc_signal<bool>                     rctg_w_80m_reg_sig;
    
    sc_signal<bool>                     pmt2cp_sig;
    sc_signal<bool>                     pmt2of_sig;
    
    sc_signal<unsigned short>           phi_out_sig;
    
    sc_signal<unsigned char>            ANGCTR_sig;
    sc_signal<unsigned short>           RESTHSFT_sig;
    
    sc_signal<unsigned short>           rdata_em2ithtresfix_sig;
    
    sc_signal<unsigned short>           rdata_em2ithtefix_sig;
    
    sc_signal<bool>                     ang_comp1_eq_sp_sig;
    
    sc_signal<bool>                     ir3_adc_u_r_sig;
    sc_signal<bool>                     ir3_adc_v_r_sig;
    sc_signal<bool>                     ir3_adc_w_r_sig;
    
    sc_signal<bool>                     ad_end_trig_sp_sig;
    sc_signal<bool>                     adtrg_mvint1_sig;
    sc_signal<bool>                     adtrg_mvint0_sig;
    sc_signal<bool>                     adtrg_mem_rg_chg_sig;
    sc_signal<bool>                     adtrg_smerr_sig;
    sc_signal<bool>                     adtrg_amerr_sig;
    sc_signal<bool>                     ad_start_mou_sp_80m_sig;
    sc_signal<bool>                     ad_start_vall_sp_80m_sig;
    
    sc_signal<signed int>               id_sel_out_sig;
    sc_signal<signed int>               iq_sel_out_sig;
    sc_signal<signed int>               id_dir_reg_sig;
    sc_signal<signed int>               iq_dir_reg_sig;
    sc_signal<signed int>               id_reg_sig;
    sc_signal<signed int>               iq_reg_sig;
    sc_signal<signed int>               rdata_em2ivd_sig;
    sc_signal<signed int>               rdata_em2ivq_sig;
    
    sc_signal<bool>                     output_pwm_end_sp_reg_sig;
    sc_signal<unsigned int>             tstwdcmpu_pclk_sig;
    sc_signal<unsigned int>             tstwdcmpv_pclk_sig;
    sc_signal<unsigned int>             tstwdcmpw_pclk_sig;
    sc_signal<bool>                     pwm_ovfl_out_sig;
    sc_signal<unsigned char>            pwm_ovfl_func_sig;
    sc_signal<bool>                     pwm_zdfl_out_sig;
    
    sc_signal<bool>                     pi_end_sp_reg_sig;
    sc_signal<bool>                     pi_ovfl_out_sig;
    sc_signal<bool>                     pi_ovfl_func_sig;
    
    sc_signal<bool>                     ir3_int_err_r_sig;
    sc_signal<bool>                     ir3_int_u_r_sig;
    sc_signal<bool>                     ir3_int_v_r_sig;
    sc_signal<bool>                     ir3_int_w_r_sig;
    sc_signal<bool>                     ir3_calc_u_r_sig;
    sc_signal<bool>                     ir3_calc_v_r_sig;
    sc_signal<bool>                     ir3_calc_w_r_sig;
    sc_signal<bool>                     ir3_pat_u_r_sig;
    sc_signal<bool>                     ir3_pat_v_r_sig;
    sc_signal<bool>                     ir3_pat_w_r_sig;
    
public:
    //--------------------------------------
    //       Constructor                    
    //--------------------------------------
    SC_HAS_PROCESS(EMU3S_HWCORE);
    EMU3S_HWCORE(sc_module_name name,
        uint32_t rLatency,
        uint32_t wLatency,
        SIM_MODE_T simmode);
    ~EMU3S_HWCORE(void);

    void SetMessageLevel(const std::string msg_lv, std::string sub_ip_name = "ALL");
    void DumpRegisterRW(const std::string is_enable, std::string sub_ip_name = "ALL");
    void EnableRegisterMessage(const std::string is_enable, std::string sub_ip_name = "ALL");
    void DumpInterrupt(const std::string is_enable, std::string sub_ip_name = "ALL");
    void EnableStatInfo(const std::string is_enable, std::string sub_ip_name = "ALL");
    
    void AssertReset(const std::string reset_name, const double start_time, const double period, std::string sub_ip_name = "ALL");
    void SetCLKFreq(const std::string clock_name, const sc_dt::uint64  freq, const std::string unit, std::string sub_ip_name = "ALL");
    void GetCLKFreq(const std::string clock_name, std::string sub_ip_name = "ALL");

    void ForceRegister(const std::string reg_name, unsigned int value, std::string sub_ip_name);
    void ReleaseRegister(const std::string reg_name, std::string sub_ip_name);
    void WriteRegister(const std::string reg_name, unsigned int value, std::string sub_ip_name);
    void ReadRegister(const std::string reg_name, std::string sub_ip_name);
    void ListRegister(const std::string sub_ip_name = "ALL");
    void DumpStatInfo(const std::string sub_ip_name = "ALL");
    void ChgProcessingTime(std::string sub_ip_name, std::string func_name, unsigned int value);
    void SetAHBExtension(unsigned int value);

    void SetEDCError(unsigned char domain, unsigned char error_type, unsigned char status);
    void Help(const std::string type);

private:
    void MonitorPCLKMethod(void);
    void MonitorCCLKMethod(void);

    void MonitorPRESETnMethod(void);
    void MonitorCRESETnMethod(void);

    void HandlePRESETnCmdMethod(void);
    void HandleCRESETnCmdMethod(void);
    void CancelPRESETnCmdMethod(void);
    void CancelCRESETnCmdMethod(void);
   
    void PRESETnSigWriteMethod(void);
    void CRESETnResetMethod(void);
    void WriteSSCGSWResetMethod(void);
    void WriteCleanSWResetMethod(void);

    void AssertEDCErrSedPclkMethod(void);
    void DeassertEDCErrSedPclkMethod(void);
    void WriteEDCErrSedPclkMethod(void);
    void AssertEDCErrDedPclkMethod(void);
    void DeassertEDCErrDedPclkMethod(void);
    void WriteEDCErrDedPclkMethod(void);
    void AssertEDCErrSedCclkMethod(void);
    void DeassertEDCErrSedCclkMethod(void);
    void WriteEDCErrSedCclkMethod(void);
    void AssertEDCErrDedCclkMethod(void);
    void DeassertEDCErrDedCclkMethod(void);
    void WriteEDCErrDedCclkMethod(void);

    void MonitorADCStartMethod(void);
    void MonitorADCGrEndMethod(void);
    void MonitorADCCh0EndMethod(void);
    void MonitorADCCh0DatMethod(void);
    void MonitorADCCh1EndMethod(void);
    void MonitorADCCh1DatMethod(void);
    void MonitorADCCh2EndMethod(void);
    void MonitorADCCh2DatMethod(void);
    void WriteADC0StartMethod(void);
    void WriteADC1StartMethod(void);

    void MonitorRdcZMethod(void);
    void MonitorRdcPhiMethod(void);
    void MonitorRdcHighOmegaMethod(void);

    void MonitorTSG3InputMethod(void);

    void MonitorRecPatUMethod(void);
    void MonitorRecPatVMethod(void);
    void MonitorRecPatWMethod(void);
    void WriteTstiupMethod(void);
    void WriteTstivpMethod(void);
    void WriteTstiwpMethod(void);

    void MonitorTstwecmpMethod(void);
    void MonitorTstwdcmp0Method(void);
    void MonitorTstwdcmpuMethod(void);
    void MonitorTstwdcmpvMethod(void);
    void MonitorTstwdcmpwMethod(void);
    void WriteTstwecmpMethod(void);
    void WriteTstwdcmp0Method(void);
    void WriteTstwdcmpuMethod(void);
    void WriteTstwdcmpvMethod(void);
    void WriteTstwdcmpwMethod(void);

    void MonitorAngComp0SPMethod(void);
    void MonitorRCTGEndSPMethod(void);
    
    void AssertRCTGStartSPMethod(void);
    void DeassertRCTGStartSPMethod(void);
    void WriteRCTGStartSPMethod(void);

    void PWMSkipTrigger(void);
    void MonitorPWMEndSPMethod(void);
    void MonitorPWMFuncStMethod(void);
    void MonitorPWMFuncEndMethod(void);
    void MonitorPWMFuncWaitMethod(void);
    void MonitorPWMFuncBusyMethod(void);

    void AssertPWMStartSPMethod(void);
    void DeassertPWMStartSPMethod(void);
    void WritePWMStartSPMethod(void);
    
    void PIStartSPAnalyze(void);
    void PISkipTrigger(void);
    void MonitorPIEndSPMethod(void);
    void MonitorPIFuncStMethod(void);
    void MonitorPIFuncEndMethod(void);
    void MonitorPIFuncWaitMethod(void);
    void MonitorPIFuncBusyMethod(void);

    void AssertPIStartSPMethod(void);
    void DeassertPIStartSPMethod(void);
    void WritePIStartSPMethod(void);
    
    void INPUTStartSPAnalyze(void);
    void INPUTSkipTrigger(void);
    void MonitorINPUTEndSPMethod(void);
    void MonitorINPUTFuncStMethod(void);
    void MonitorINPUTFuncEndMethod(void);
    void MonitorINPUTFuncWaitMethod(void);
    void MonitorINPUTFuncBusyMethod(void);
    
    bool ADCompletionDetection();
    
    void AssertINPUTStartSPMethod(void);
    void DeassertINPUTStartSPMethod(void);
    void WriteINPUTStartSPMethod(void);
    
    void MonitorADTRGEndTrigMethod(void);
    void MonitorIIRCh0EndMethod(void);
    void MonitorIIRCh1EndMethod(void);
    void MonitorIIRCh2EndMethod(void);

    void MonitorRSLVIDEFuncStMethod(void);
    void MonitorRSLVIDEFuncEndMethod(void);
    void MonitorRSLVIDEFuncWaitMethod(void);
    void MonitorRSLVIDEFuncBusyMethod(void);

    void UpdateFUNCSTGRPA(emSubModel ip_id, unsigned int FuncSt_1Hot);
    void UpdateFSMSTGRPA(emSubModel ip_id);
    void UpdateFUNCFINGRPA(emSubModel ip_id);
    void UpdateFUNCSTGRPB(unsigned int FuncSt_1Hot);
    void UpdateFSMSTGRPB(void);
    void UpdateFUNCFINGRPB(void);
    unsigned int GetFuncID(unsigned int FuncEnd_1Hot);

    void AssertFuncAWaitMethod(void);
    void DeassertFuncAWaitMethod(void);
    void WriteFuncAWaitMethod(void);

    void AssertFuncBWaitMethod(void);
    void DeassertFuncBWaitMethod(void);
    void WriteFuncBWaitMethod(void);

    void MonitorIptrgMonAdtrgNumMethod(void);
    
    void WriteCTRMethod(void);
    void WriteREFCTRMethod(void);
    void WriteAHBPRTMethod(void);
    void WriteTBLADRMethod(void);
    void WriteIPTRGCTRMethod(void);
    void WriteIPTRGNUMMethod(void);
    void WriteFUNCIDLEGRPA0Method(void);
    void WriteFUNCIDLEGRPA1Method(void);
    void WriteFUNCIDLEGRPA2Method(void);
    void WriteFUNCIDLEGRPBMethod(void);
    void WriteFUNCWAITGRPAMethod(void);
    void WriteFUNCWAITGRPBMethod(void);
    void WriteFUNCFLGRPA0Method(void);
    void WriteFUNCFLGRPA1Method(void);
    void WriteFUNCFLGRPA2Method(void);
    void WriteFUNCFLGRPBMethod(void);
    void WriteADSELMethod(void);
    void WriteSFERCMethod(void);
    
    /// Internal functions
    void Initialize(void);
    void InitVarSSCG(void);
    void InitVarCLEAN(void);
    void Initialize_Maps(); // Call in Initialize()
    void InitPort(void);
    void InitSSCGPort(void);
    void InitCLEANPort(void);
    void InitProcess(void);

    void CancelSSCGEvents(void);
    void CancelCLEANEvents(void);

    bool GetResetStatus(const std::string reset_name);
    bool CheckClockPeriod(const std::string clock_name);
    CRWInterface* GetInstance(unsigned int offsetAddress);
    void WriteSWReset(bool is_active);

    std::map<std::string, unsigned int> mSubIPMap;// Map of all sub-ip
    std::string all_sub_ip;
    std::map<unsigned int, unsigned int> mStateMap;  // Map of state of sub-ip
    std::map<unsigned int, unsigned int> mFuncMaxMap;// Map of func number of sub-ip
    std::map<unsigned int, unsigned int> mPower2Map; // Map of power 2
    
    enum emState {
        emIDLE = 0x00000000,
        emBUSY = 0x00000001,
        emWAIT = 0x00000100
    };

    enum emInterrupt
    {
        emINTERRUPT_NUM = 8
    };

    enum emHWCOREConst
    {
        emSFTEN_MASK = 0xFFFFEFFF,
        emIP_MASK = 0xFFFFFCFF,
        emSFTEN_POS = 12,
        emIP_POS = 8,
        emNotExistFuncID = 999
    };
    
    void SetMesLvlTop(const std::string msg_lv);
    void EnableRegMesTop(const std::string is_enable);
    void DumpRegRWTop(const std::string is_enable, emSubModel ip);
    void DumpIntTop(const std::string is_enable);
    void ForceRegTop(std::string reg_name, unsigned int value, emSubModel ip);
    void ReleaseRegTop(std::string reg_name, emSubModel ip);
    void WriteRegTop(std::string reg_name, unsigned int value, emSubModel ip);
    void ReadRegTop(std::string reg_name, emSubModel ip);
    
    void CreateConnections();
    void ConnectResetSig();
    void ConnectClockSig();
    void ConnectTopSig();
    void ConnectInternalSig();
    
    void NotifyWriteCTR();
    void NotifyWriteREFCTR();
    void NotifyWriteAHBPRT();
    void NotifyWriteTBLADR();
    void NotifyWriteIPTRGCTR();
    void NotifyWriteIPTRGNUM();
    void NotifyWriteFUNCIDLEGRPA0();
    void NotifyWriteFUNCIDLEGRPA1();
    void NotifyWriteFUNCIDLEGRPA2();
    void NotifyWriteFUNCIDLEGRPB();
    void NotifyWriteFUNCWAITGRPA();
    void NotifyWriteFUNCWAITGRPB();
    void NotifyWriteFUNCFLGRPA0();
    void NotifyWriteFUNCFLGRPA1();
    void NotifyWriteFUNCFLGRPA2();
    void NotifyWriteFUNCFLGRPB();
    void NotifyWriteADSEL();
    void NotifyWriteSFERC();
    
    void NotifySWTrigger(emSubModel emSub);
    unsigned int GetTRGNUM(emSubModel emSub);
    
    void DumpInterruptMsg (const std::string intr_name, const bool value);
    void ResetSSCGPort(void);
    void ResetCLEANPort(void);
};
#endif //__EMU3S_HWCORE_H__
