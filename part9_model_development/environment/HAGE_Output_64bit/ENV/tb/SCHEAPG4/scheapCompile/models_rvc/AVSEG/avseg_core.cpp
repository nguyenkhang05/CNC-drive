// ---------------------------------------------------------------------
// $Id: $
//
// Copyright(c) 2016 Renesas Electronics Corporation
// Copyright(c) 2016 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "avseg.h"
#include "avseg_core.h"

/*********************************
// Function     : Cavseg_core
// Description  : Constructor of Cavseg_core class
// Parameter    :
//      name            Module name
//      Parent          Indicate base class
// Return value : None
**********************************/
Cavseg_core::Cavseg_core(std::string name, Cavseg *AVSEGPtr): Cavseg_regif(name,32)
{//{{{
    mModuleName = name;
    assert(AVSEGPtr != NULL);
    mAVSEG = AVSEGPtr;
    mAVSEG->CommandInit(name);
    Cavseg_regif::set_instance_name(name);
    mNoiseFilter = new NoiseFilter[mAVSEG->emInputChannelNum];
    this->Initialize();
}//}}}

/*********************************
// Function     : ~Cavseg_core
// Description  : Destructor of Cavseg_core class
// Parameter    : None
// Return value : None
**********************************/
Cavseg_core::~Cavseg_core (void)
{//{{{
    delete [] mNoiseFilter;
}//}}}

/*********************************
// Function     : read
// Description  : Access to read value of register
// Parameter    :
//      is_rd_dbg           Debug access mode
//      addr                Register's address offset
//      p_data              Memory pointer
//      size                Access size
// Return value : true if reading transaction is finished normally
**********************************/
bool Cavseg_core::read (const bool is_rd_dbg, unsigned int addr, unsigned char *p_data, unsigned int size)
{//{{{
    unsigned int addr_ost = addr&0x7F;
    if (GetWriteCondition(addr_ost)) {
        PreReadCall(addr&0x3F);
        assert(p_data != NULL);
        memset(p_data, 0, size);
        if (is_rd_dbg == false) {
            return reg_rd(addr,p_data,size);
        } else {
            return reg_rd_dbg(addr,p_data,size);
        }
    } else { return false;}
}//}}}

/*********************************
// Function     : write
// Description  : Access to write value to register
// Parameter    :
//      is_wr_dbg           Debug access mode
//      addr                Register's address offset
//      p_data              Memory pointer
//      size                Access size
// Return value : true if writing transaction is finished normally
**********************************/
bool Cavseg_core::write (const bool is_wr_dbg, unsigned int addr, unsigned char *p_data, unsigned int size)
{//{{{
    unsigned int addr_ost = addr&0x7F;
    bool ret = false;

    if (GetWriteCondition(addr_ost)) {
        if ((addr_ost == 0x4 || addr_ost == 0x14 || addr_ost == 0x24 || addr_ost == 0x34)&&(size == 4)) {
            unsigned int data = 0;
            assert(p_data != NULL);
            memcpy(&data, p_data, size);
            unsigned int nrmcnt_val = (data>>8)&0xF;
            unsigned int errcnt_val = data&0xF;
            unsigned int pre_nrmcnt_val = 1;
            unsigned int pre_errcnt_val = 1;
            if (addr_ost == 0x4) {
                pre_nrmcnt_val = (unsigned int)(*VCCCNTCR)["NRMCNT"];
                pre_errcnt_val = (unsigned int)(*VCCCNTCR)["ERRCNT"];
            } else if (addr_ost == 0x14) {
                pre_nrmcnt_val = (unsigned int)(*EVCCCNTCR)["NRMCNT"];
                pre_errcnt_val = (unsigned int)(*EVCCCNTCR)["ERRCNT"];
            } else if (addr_ost == 0x24) {
                pre_nrmcnt_val = (unsigned int)(*AWOVDDCNTCR)["NRMCNT"];
                pre_errcnt_val = (unsigned int)(*AWOVDDCNTCR)["ERRCNT"];
            } else {
                pre_nrmcnt_val = (unsigned int)(*ISOVDDCNTCR)["NRMCNT"];
                pre_errcnt_val = (unsigned int)(*ISOVDDCNTCR)["ERRCNT"];
            }
            if (nrmcnt_val == 0) {
                mAVSEG->re_printf("warning", "Setting prohibited: Cannot write 0 to NRMCNT bit.\n");
                data = data | ((pre_nrmcnt_val&0xF)<<8);
            }
            if (errcnt_val == 0) {
                mAVSEG->re_printf("warning", "Setting prohibited: Cannot write 0 to ERRCNT bit.\n");
                data = data | (pre_errcnt_val&0xF);
            }
            memcpy(p_data, &data, size);
        }
        if (is_wr_dbg == false) {
            ret = reg_wr(addr,p_data,size);
        } else {
            ret = reg_wr_dbg(addr,p_data,size);
        }
    }
    return ret;
}//}}}

/*********************************
// Function     : GetWriteCondition
// Description  : Check writing condition to register
// Parameter    :
//      addr                Register's address offset
// Return value : Write condition
**********************************/
bool Cavseg_core::GetWriteCondition (const unsigned int addr)
{//{{{
    bool ret = true;
    unsigned int addr_ost = addr&0x7F;

    if (mTargetProduct == "AVSEG_E2x") { 
        if ((addr_ost>=0x30) && (0x44>=addr_ost)) {
        re_printf("warning","Register is not existing in %s.\n", mTargetProduct.c_str()); 
        ret = false;
        }
    } else { ret = true;}
    return ret;
}//}}}


/*********************************
// Function     : PreReadCall
// Description  : Handle register before reading
// Parameter    :
//      addr            register address offset
// Return value : None
**********************************/
void Cavseg_core::PreReadCall (const unsigned int addr)
{//{{{
    //There is no setting/update before reading
}//}}}

/*********************************
// Function     : EnableReset
// Description  : Reset registers
// Parameter    :
//      is_active           Active state of reset
//      reset_id            Reset ID
// Return value : Register value
**********************************/
void Cavseg_core::EnableReset (const bool is_active, const unsigned int reset_id)
{//{{{
    Cavseg_regif::EnableReset(is_active);
    if (is_active) {
        this->Initialize();
    }
}//}}}

/*********************************
// Function     : Initialize
// Description  : Initializes internal variables
// Parameter    : None
// Return value : None
**********************************/
void Cavseg_core::Initialize (void)
{//{{{
    for (unsigned int chn_sel = 0; chn_sel < mAVSEG->emInputChannelNum; chn_sel++) {
        mNoiseFilter[chn_sel].Initialize();
    }
}//}}}

/*********************************
// Function     : RegisterHandler
// Description  : Set parameters or commands to Cavseg_regif class
// Parameter    :
//      args            Argument of parameter/command
// Return value : Register dumping message
**********************************/
std::string Cavseg_core::RegisterHandler (const std::vector<std::string> &args)
{//{{{
    std::string msg_return = "";
    if ((int)args.size() == 1) {//Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            mAVSEG->re_printf("info", "%s\n", reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    } else {
        bool reg_found = false;
        vpcl::re_register *reg_p = first_reg_object();
        std::vector<std::string> args_updated;
        std::copy( args.begin(), args.end(), back_inserter( args_updated ) );
        while (reg_p != NULL) {
            if (strmatch(args[1].c_str(), reg_p->name().c_str())) {
                reg_found = true;
                args_updated[1] = reg_p->name();
                msg_return += this->reg_handle_command(args_updated) + "\n";
            }
            reg_p = next_reg_object();
        }
        if (!reg_found) {
            msg_return = this->reg_handle_command(args_updated);
        }
    }
    return msg_return;
}//}}}

/*********************************
// Function     : GetChannelSelect
// Description  : Get selected input channel
// Parameter    :
//      chn_sel             Input channel selector
// Return value : Selected channel for chn_sel
**********************************/
unsigned int Cavseg_core::GetChannelSelect(const unsigned int chn_sel)
{//{{{
    unsigned int ret = 0;
    if (chn_sel == mAVSEG->emVCC) {
        ret = (unsigned int)(*VCCCHSCR)["CHS"];
    } else if (chn_sel == mAVSEG->emEVCC) {
        ret = (unsigned int)(*EVCCCHSCR)["CHS"];
    } else if (chn_sel == mAVSEG->emAWOVDD) { 
        ret = (unsigned int)(*AWOVDDCHSCR)["CHS"];
    } else {
        ret = (unsigned int)(*ISOVDDCHSCR)["CHS"];
    }
    return ret;
}//}}}

/*********************************
// Function     : NotifyFilterControl
// Description  : Notify Noise Filter
// Parameter    :
//      chn_sel             Input channel selector
//      is_upper            true: upper error occurs
//      is_lower            true: lower error occurs
// Return value : None
**********************************/
void Cavseg_core::NotifyFilterControl(const unsigned int chn_sel, const bool is_upper, const bool is_lower)
{//{{{
    if (mNoiseFilter[chn_sel].filt_enb) {
        if (is_upper || is_lower) {
            mNoiseFilter[chn_sel].rec_res();
            if (mNoiseFilter[chn_sel].error_counter > 0) {
                mNoiseFilter[chn_sel].error_counter -= 1;
            }
        } else {
            if ((mNoiseFilter[chn_sel].recov_counter <= 0x1)||(mNoiseFilter[chn_sel].error_counter != 0)) {
                mNoiseFilter[chn_sel].err_res();
                mNoiseFilter[chn_sel].ex_bound = 0;
            }
            if (mNoiseFilter[chn_sel].recov_counter > 0) {
                mNoiseFilter[chn_sel].recov_counter -= 1;
            }
        }
        if (mNoiseFilter[chn_sel].error_counter == 0) {
            if (is_upper) {
                mNoiseFilter[chn_sel].ex_bound = 1;
            } else if (is_lower) {
                mNoiseFilter[chn_sel].ex_bound = 2;
            }
            mAVSEG->NotifyErrPulseOut(chn_sel, mNoiseFilter[chn_sel].ex_bound);
        }
    }
}//}}}

/*********************************
// Function     : UpdateErrorRegister
// Description  : Update Error Register 
// Parameter    :
//      chn_sel             Input channel selector
//      is_upper            true: upper error occurs
//      is_lower            true: lower error occurs
// Return value : None
**********************************/
void Cavseg_core::UpdateErrorRegister(const unsigned int chn_sel, const bool is_upper, const bool is_lower)
{//{{{
    if (mTargetProduct == "AVSEG_U2A") {
        if (chn_sel == mAVSEG->emVCC) {
            if (is_upper) {
                (*SECVMONERR)["VCC_ULME"] = 0x1;
                re_printf("info", "SECVMONERR.VCC_ULME is updated to 1.\n");
            }
            if (is_lower) {
                (*SECVMONERR)["VCC_LLME"] = 0x1;
                re_printf("info", "SECVMONERR.VCC_LLME is updated to 1.\n");
            }
        } else if (chn_sel == mAVSEG->emEVCC) {
            if (is_upper) {
                (*SECVMONERR)["EVCC_ULME"] = 0x1;
                re_printf("info", "SECVMONERR.EVCC_ULME is updated to 1.\n");
            }
            if (is_lower) {
                (*SECVMONERR)["EVCC_LLME"] = 0x1;
                re_printf("info", "SECVMONERR.EVCC_LLME is updated to 1.\n");
            }
        } else if (chn_sel == mAVSEG->emAWOVDD) {
            if (is_upper) {
                (*SECVMONERR)["AWOVDD_ULME"] = 0x1;
                re_printf("info", "SECVMONERR.AWOVDD_ULME is updated to 1.\n");
            }
            if (is_lower) {
                (*SECVMONERR)["AWOVDD_LLME"] = 0x1;
                re_printf("info", "SECVMONERR.AWOVDD_LLME is updated to 1.\n");
            }
        } else {
            if (is_upper) {
                (*SECVMONERR)["ISOVDD_ULME"] = 0x1;
                re_printf("info", "SECVMONERR.ISOVDD_ULME is updated to 1.\n");
            }
            if (is_lower) {
                (*SECVMONERR)["ISOVDD_LLME"] = 0x1;
                re_printf("info", "SECVMONERR.ISOVDD_LLME is updated to 1.\n");
            }
        }
    } 
}//}}}

/*********************************
// Function     : cb_VCCCHSCR_CHS
// Description  : Callback function when register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void Cavseg_core::cb_VCCCHSCR_CHS(RegCBstr str)
{//{{{
    unsigned int vccchs_val = (unsigned int)(*VCCCHSCR)["CHS"];
    if (mTargetProduct == "AVSEG_U2A") {
        if (vccchs_val >= 0x40) {
            mAVSEG->re_printf("info", "There is no input channel is selected for generating error of VCC.\n");
        } else {
            mAVSEG->re_printf("info", "ADC0 virtual channel %d is selected for generating error of VCC.\n", vccchs_val);
        }
    } else if (mTargetProduct == "AVSEG_E2x") {
        if (vccchs_val >= 0x28) {
            mAVSEG->re_printf("info", "There is no input channel is selected for generating error of VCC.\n");
        } else {
            mAVSEG->re_printf("info", "ADC0 virtual channel %d is selected for generating error of VCC.\n", vccchs_val);
        }
    }
}//}}}

/*********************************
// Function     : cb_VCCCNTCR_ENB
// Description  : Callback function when register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void Cavseg_core::cb_VCCCNTCR_ENB(RegCBstr str)
{//{{{
    mNoiseFilter[mAVSEG->emVCC].filt_enb = (bool)(*VCCCNTCR)["ENB"];
    mNoiseFilter[mAVSEG->emVCC].filt_nrmcnt = (unsigned int)(*VCCCNTCR)["NRMCNT"];
    mNoiseFilter[mAVSEG->emVCC].filt_errcnt = (unsigned int)(*VCCCNTCR)["ERRCNT"];
    mNoiseFilter[mAVSEG->emVCC].rec_res();
    mNoiseFilter[mAVSEG->emVCC].err_res();
}//}}}

/*********************************
// Function     : cb_EVCCCHSCR_CHS
// Description  : Callback function when register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void Cavseg_core::cb_EVCCCHSCR_CHS(RegCBstr str)
{//{{{
    unsigned int evccchs_val = (unsigned int)(*EVCCCHSCR)["CHS"];
    if (mTargetProduct == "AVSEG_U2A") {
        if (evccchs_val >= 0x40) {
            mAVSEG->re_printf("info", "There is no input channel is selected for generating error of EVCC.\n");
        } else {
            mAVSEG->re_printf("info", "ADC0 virtual channel %d is selected for generating error of EVCC.\n", evccchs_val);
        }
    } else if (mTargetProduct == "AVSEG_E2x") {
        if (evccchs_val >= 0x28) {
            mAVSEG->re_printf("info", "There is no input channel is selected for generating error of EVCC.\n");
        } else {
            mAVSEG->re_printf("info", "ADC0 virtual channel %d is selected for generating error of EVCC.\n", evccchs_val);
        }
    }
}//}}}

/*********************************
// Function     : cb_EVCCCNTCR_ENB
// Description  : Callback function when register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void Cavseg_core::cb_EVCCCNTCR_ENB(RegCBstr str)
{//{{{
    mNoiseFilter[mAVSEG->emEVCC].filt_enb = (bool)(*EVCCCNTCR)["ENB"];
    mNoiseFilter[mAVSEG->emEVCC].filt_nrmcnt = (unsigned int)(*EVCCCNTCR)["NRMCNT"];
    mNoiseFilter[mAVSEG->emEVCC].filt_errcnt = (unsigned int)(*EVCCCNTCR)["ERRCNT"];
    mNoiseFilter[mAVSEG->emEVCC].rec_res();
    mNoiseFilter[mAVSEG->emEVCC].err_res();
}//}}}

/*********************************
// Function     : cb_AWOVDDCHSCR_CHS
// Description  : Callback function when register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void Cavseg_core::cb_AWOVDDCHSCR_CHS(RegCBstr str)
{//{{{
    unsigned int awovddchs_val = (unsigned int)(*AWOVDDCHSCR)["CHS"];
    if (mTargetProduct == "AVSEG_U2A") {
        if (awovddchs_val >= 0x40) {
            mAVSEG->re_printf("info", "There is no input channel is selected for generating error of AWOVDD.\n");
        } else {
            mAVSEG->re_printf("info", "ADC0 virtual channel %d is selected for generating error of AWOVDD.\n", awovddchs_val);
        }
    } else if (mTargetProduct == "AVSEG_E2x") {
        if (awovddchs_val >= 0x28) {
            mAVSEG->re_printf("info", "There is no input channel is selected for generating error of VDD.\n");
        } else {
            mAVSEG->re_printf("info", "ADC0 virtual channel %d is selected for generating error of VDD.\n", awovddchs_val);
        }
    }
}//}}}

/*********************************
// Function     : cb_AWOVDDCNTCR_ENB
// Description  : Callback function when register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void Cavseg_core::cb_AWOVDDCNTCR_ENB(RegCBstr str)
{//{{{
    mNoiseFilter[mAVSEG->emAWOVDD].filt_enb = (bool)(*AWOVDDCNTCR)["ENB"];
    mNoiseFilter[mAVSEG->emAWOVDD].filt_nrmcnt = (unsigned int)(*AWOVDDCNTCR)["NRMCNT"];
    mNoiseFilter[mAVSEG->emAWOVDD].filt_errcnt = (unsigned int)(*AWOVDDCNTCR)["ERRCNT"];
    mNoiseFilter[mAVSEG->emAWOVDD].rec_res();
    mNoiseFilter[mAVSEG->emAWOVDD].err_res();
}//}}}

/*********************************
// Function     : cb_ISOVDDCHSCR_CHS
// Description  : Callback function when register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void Cavseg_core::cb_ISOVDDCHSCR_CHS(RegCBstr str)
{//{{{
    unsigned int isovddchs_val = (unsigned int)(*ISOVDDCHSCR)["CHS"];
    if (isovddchs_val >= 0x40) {
        mAVSEG->re_printf("info", "There is no input channel is selected for generating error of ISOVDD.\n");
    } else {
        mAVSEG->re_printf("info", "ADC0 virtual channel %d is selected for generating error of ISOVDD.\n", isovddchs_val);
    }
}//}}}

/*********************************
// Function     : cb_ISOVDDCNTCR_ENB
// Description  : Callback function when register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void Cavseg_core::cb_ISOVDDCNTCR_ENB(RegCBstr str)
{//{{{
    mNoiseFilter[mAVSEG->emISOVDD].filt_enb = (bool)(*ISOVDDCNTCR)["ENB"];
    mNoiseFilter[mAVSEG->emISOVDD].filt_nrmcnt = (unsigned int)(*ISOVDDCNTCR)["NRMCNT"];
    mNoiseFilter[mAVSEG->emISOVDD].filt_errcnt = (unsigned int)(*ISOVDDCNTCR)["ERRCNT"];
    mNoiseFilter[mAVSEG->emISOVDD].rec_res();
    mNoiseFilter[mAVSEG->emISOVDD].err_res();
}//}}}


/*********************************
// Function     : cb_SECVMONCLR_VCC_LLMEC
// Description  : Callback function when register is written
// Parameter    :
//      str                 Register's callback string
// Return value : None
**********************************/
void Cavseg_core::cb_SECVMONCLR_VCC_LLMEC(RegCBstr str)
{//{{{
    if ((unsigned int)(*SECVMONCLR)["AWOVDD_ULMEC"] == 0x1) {
        (*SECVMONERR)["AWOVDD_ULME"] = 0x0;
        re_printf("info", "SECVMONERR.AWOVDD_ULME bit is cleared to 0.\n");
    }
    if ((unsigned int)(*SECVMONCLR)["AWOVDD_LLMEC"] == 0x1) {
        (*SECVMONERR)["AWOVDD_LLME"] = 0x0;
        re_printf("info", "SECVMONERR.AWOVDD_LLME bit is cleared to 0.\n");
    }
    if ((unsigned int)(*SECVMONCLR)["ISOVDD_ULMEC"] == 0x1) {
        (*SECVMONERR)["ISOVDD_ULME"] = 0x0;
        re_printf("info", "SECVMONERR.ISOVDD_ULME bit is cleared to 0.\n");
    }
    if ((unsigned int)(*SECVMONCLR)["ISOVDD_LLMEC"] == 0x1) {
        (*SECVMONERR)["ISOVDD_LLME"] = 0x0;
        re_printf("info", "SECVMONERR.ISOVDD_LLME bit is cleared to 0.\n");
    }
    if ((unsigned int)(*SECVMONCLR)["EVCC_ULMEC"] == 0x1) {
        (*SECVMONERR)["EVCC_ULME"] = 0x0;
        re_printf("info", "SECVMONERR.EVCC_ULME bit is cleared to 0.\n");
    }
    if ((unsigned int)(*SECVMONCLR)["EVCC_LLMEC"] == 0x1) {
        (*SECVMONERR)["EVCC_LLME"] = 0x0;
        re_printf("info", "SECVMONERR.EVCC_LLME bit is cleared to 0.\n");
    }
    if ((unsigned int)(*SECVMONCLR)["VCC_ULMEC"] == 0x1) {
        (*SECVMONERR)["VCC_ULME"] = 0x0;
        re_printf("info", "SECVMONERR.VCC_ULME bit is cleared to 0.\n");
    }
    if ((unsigned int)(*SECVMONCLR)["VCC_LLMEC"] == 0x1) {
        (*SECVMONERR)["VCC_LLME"] = 0x0;
        re_printf("info", "SECVMONERR.VCC_LLME bit is cleared to 0.\n");
    }
    (*SECVMONCLR) = 0x0;
}//}}}

bool Cavseg_core::strmatch(const char *ptn, const char *str) {//{{{
  bool ret;
  if (ptn == NULL || str == NULL) return false;

  switch( *ptn ) {
  case '\0':
    ret = '\0' == *str;
    break;
  case '*' :
    ret = strmatch( &ptn[1], str ) || (('\0' != *str) && strmatch( ptn, &str[1] ));
    break;
  case '?' :
    ret = ('\0' != *str) && strmatch( &ptn[1], &str[1] );
    break;
  default  :
    ret = ((unsigned char)*ptn == (unsigned char)*str) && strmatch( &ptn[1], &str[1] );
    break;
  }
  return ret;
}//}}}
// vim600: set foldmethod=marker : 
