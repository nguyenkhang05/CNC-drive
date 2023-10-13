// ---------------------------------------------------------------------
// $Id: $
//
// Copyright(c) 2016 - 2017 Renesas Electronics Corporation
// Copyright(c) 2016 - 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "abfg.h"
#include "abfg_core.h"

/*********************************
// Function     : Cabfg_core
// Description  : Constructor of Cabfg_core class
// Parameter    :
//      name            Module name
//      Parent          Indicate base class
// Return value : None
**********************************/
Cabfg_core::Cabfg_core(std::string name, Cabfg *ABFGPtr): Cabfg_regif(name,32)
{//{{{
    mModuleName = name;
    assert(ABFGPtr != NULL);
    mABFG = ABFGPtr;
    mABFG->CommandInit(name);
    Cabfg_regif::set_instance_name(name);
    mNoiseFilter = new NoiseFilter[mABFG->emInputChannelNum];
    this->Initialize();
}//}}}

/*********************************
// Function     : ~Cabfg_core
// Description  : Destructor of Cabfg_core class
// Parameter    : None
// Return value : None
**********************************/
Cabfg_core::~Cabfg_core (void)
{//{{{
    delete [] mNoiseFilter;
}//}}}

/*********************************
// Function     : read
// Description  : Access to read value of registers
// Parameter    :
//      is_rd_dbg           Debug access mode
//      addr                Register's address offset
//      p_data              Memory pointer
//      size                Access size
// Return value : true if reading transaction is finished normally
**********************************/
bool Cabfg_core::read (const bool is_rd_dbg, unsigned int addr, unsigned char *p_data, unsigned int size)
{//{{{
    PreReadCall(addr&0x3FF);
    assert(p_data != NULL);
    memset(p_data, 0, size);
    if (is_rd_dbg == false) {
        return reg_rd(addr,p_data,size);
    } else {
        return reg_rd_dbg(addr,p_data,size);
    }
}//}}}

/*********************************
// Function     : write
// Description  : Access to write value to registers
// Parameter    :
//      is_wr_dbg           Debug access mode
//      addr                Register's address offset
//      p_data              Memory pointer
//      size                Access size
// Return value : true if writing transaction is finished normally
**********************************/
bool Cabfg_core::write (const bool is_wr_dbg, unsigned int addr, unsigned char *p_data, unsigned int size)
{//{{{
    bool ret = false;
    unsigned int data = 0;
    assert(p_data != NULL);
    memcpy(&data, p_data, size);
    if (GetWriteCondition(addr&0x3FF)) {
        for (unsigned int chn_sel = 0; chn_sel < (mABFG->emInputChannelNum); chn_sel++) {
            if (((addr&0x3FF) == (0x4+(0x10*chn_sel)))&&(size == 4)) {
                unsigned int nrmcnt_val = (data>>8)&0xF;
                unsigned int errcnt_val = data&0xF;
                unsigned int pre_nrmcnt_val = (unsigned int)(*CNTCR[chn_sel])["NRMCNT"];
                unsigned int pre_errcnt_val = (unsigned int)(*CNTCR[chn_sel])["ERRCNT"];
                if (nrmcnt_val == 0) {
                    mABFG->re_printf("warning", "Setting prohibited: Cannot write 0 to CNTCR%d.NRMCNT bit.\n", chn_sel);
                    data = data | ((pre_nrmcnt_val&0xF)<<8);
                }
                if (errcnt_val == 0) {
                    mABFG->re_printf("warning", "Setting prohibited: Cannot write 0 to CNTCR%d.ERRCNT bit.\n", chn_sel);
                    data = data | (pre_errcnt_val&0xF);
                }
                memcpy(p_data, &data, size);
                break;
            }
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
//      addr            register address offset
// Return value : Write condition
**********************************/
bool Cabfg_core::GetWriteCondition (const unsigned int addr)
{//{{{
    bool ret = true;
    for (unsigned int chn_sel = 0; chn_sel < (mABFG->emInputChannelNum); chn_sel++) {
        bool filt_enb = (bool)(*CNTCR[chn_sel])["ENB"];
        if (addr == (0x8+(0x10*chn_sel))) {
            if (filt_enb) {
                mABFG->re_printf("warning", "Setting prohibited: Cannot write to BFSR%d register while CNTCR%d.ENB = 1.\n", chn_sel, chn_sel);
                ret = false;
                break;
            }
        }
    }
    return ret;
}//}}}

/*********************************
// Function     : PreReadCall
// Description  : Handle register before reading
// Parameter    :
//      addr            register address offset
// Return value : None
**********************************/
void Cabfg_core::PreReadCall (const unsigned int addr)
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
void Cabfg_core::EnableReset (const bool is_active, const unsigned int reset_id)
{//{{{
    Cabfg_regif::EnableReset(is_active);
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
void Cabfg_core::Initialize (void)
{//{{{
    for (unsigned int chn_sel = 0; chn_sel < mABFG->emInputChannelNum; chn_sel++) {
        mNoiseFilter[chn_sel].Initialize();
    }
}//}}}

/*********************************
// Function     : RegisterHandler
// Description  : Set parameters or commands to Cabfg_regif class
// Parameter    :
//      args            Argument of parameter/command
// Return value : Register dumping message
**********************************/
std::string Cabfg_core::RegisterHandler (const std::vector<std::string> &args)
{//{{{
    std::string msg_return = "";
    if ((int)args.size() == 1) {//Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            mABFG->re_printf("info", "%s\n", reg_p->name().c_str());
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
unsigned int Cabfg_core::GetChannelSelect(const unsigned int chn_sel)
{//{{{
    return (unsigned int)(*BFGCR[chn_sel])["CHS"];
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
void Cabfg_core::NotifyFilterControl(const unsigned int chn_sel, bool is_upper, bool is_lower)
{//{{{
    if (mNoiseFilter[chn_sel].filt_enb) {
        if (is_upper || is_lower) {
            mNoiseFilter[chn_sel].rec_res();
            if (mNoiseFilter[chn_sel].error_counter > 0) {
                mNoiseFilter[chn_sel].error_counter -= 1;
                if (mNoiseFilter[chn_sel].error_counter == 0) {
                    if (is_upper) {
                        mNoiseFilter[chn_sel].ex_bound = 1;
                    } else { //if (is_lower)
                        mNoiseFilter[chn_sel].ex_bound = 2;
                    }
                    BoundaryFlagControl(chn_sel, true);
                }
            }
        } else {
            if (mNoiseFilter[chn_sel].recov_counter > 0) {
                mNoiseFilter[chn_sel].recov_counter -= 1;
            }
            if ((mNoiseFilter[chn_sel].recov_counter == 0x0) || (mNoiseFilter[chn_sel].error_counter != 0)) {
                BoundaryFlagControl(chn_sel, false);
                mNoiseFilter[chn_sel].err_res();
            }
        }
    }
}//}}}

/*********************************
// Function     : BoundaryFlagControl
// Description  : Control Boundary Flag
// Parameter    :
//      chn_sel             Input channel selector
//      is_err              true: error occurs
// Return value : None
**********************************/
void Cabfg_core::BoundaryFlagControl(const unsigned int chn_sel, const bool is_err)
{//{{{
    unsigned int btgc = (unsigned int)(*BFGCR[chn_sel])["BTGC"];
    unsigned int bpgc = (unsigned int)(*BFGCR[chn_sel])["BPGC"];
    unsigned int ex_bound = mNoiseFilter[chn_sel].ex_bound;
    if (is_err) {
        if (btgc == 0) {
            if (ex_bound == 1) {
                (*BFSR[chn_sel])["BFS"] = 0x1;
            } else { //ex_bound == 2
                (*BFSR[chn_sel])["BFS"] = 0x0;
            }
        } else if (btgc == 1) {
            if (ex_bound == 2) {
                (*BFSR[chn_sel])["BFS"] = 0x1;
            } else { //ex_bound == 1
                (*BFSR[chn_sel])["BFS"] = 0x0;
            }
        } else if (btgc == 2) {
            (*BFSR[chn_sel])["BFS"] = 0x1;
        } else { //btgc == 3
            (*BFSR[chn_sel])["BFS"] = 0x0;
        }
    } else {
        if (btgc == 2) {
            (*BFSR[chn_sel])["BFS"] = 0x0;
        } else if (btgc == 3) {
            (*BFSR[chn_sel])["BFS"] = 0x1;
        }
    }
    bool boundaryflag = (bool)(*BFSR[chn_sel])["BFS"];
    if ((boundaryflag&&(bpgc == 1)) || (!boundaryflag&&(bpgc == 2)) || (bpgc == 3)) {
        mABFG->NotifyBFOut(chn_sel, boundaryflag, true);
    } else {
        mABFG->NotifyBFOut(chn_sel, boundaryflag, false);
    }
}//}}}

/*********************************
// Function     : cb_<reg_name>_<bit_name>
// Description  : Callback function when register is written
// Parameter    :
//      str                 Register's callback string (0: Fail; 1: Pass; Others: Ignored)
// Return value : None
**********************************/
void Cabfg_core::cb_CNTCR_ENB(RegCBstr str)
{//{{{
    unsigned int chn_sel = (unsigned int)str.channel;
    mNoiseFilter[chn_sel].filt_enb = (bool)(*CNTCR[chn_sel])["ENB"];
    mNoiseFilter[chn_sel].filt_nrmcnt = (unsigned int)(*CNTCR[chn_sel])["NRMCNT"];
    mNoiseFilter[chn_sel].filt_errcnt = (unsigned int)(*CNTCR[chn_sel])["ERRCNT"];
    mNoiseFilter[chn_sel].rec_res();
    mNoiseFilter[chn_sel].err_res();
    if (mNoiseFilter[chn_sel].filt_enb) {
        BoundaryFlagControl(chn_sel, false);
    }
}//}}}
void Cabfg_core::cb_BFSR_BFS(RegCBstr str)
{//{{{
    unsigned int chn_sel = (unsigned int)str.channel;
    bool boundaryflag = (bool)(*BFSR[chn_sel])["BFS"];
    mABFG->NotifyBFOut(chn_sel, boundaryflag, false);
}//}}}

bool Cabfg_core::strmatch(const char *ptn, const char *str) {
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
}

