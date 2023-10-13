// -----------------------------------------------------------------------------
// $Id: ATU5_TIMERG_Func.cpp 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2018-2019 Renesas Electronics Corporation
// Copyright(c) 2018-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------
#include "ATU5_TIMERG_Func.h"
#include <cmath>
#include <cstdlib>
//------------------------------------------------------------------------------
/**@brief public constructor
 * Detailed steps as follows:
 */
ATU5_TIMERG_Func::ATU5_TIMERG_Func(string       iname
                                  ,ATU5_TIMERG* pWrapper)
: Catu5_timerg_regif(iname, 32)
// public attributes
// Variables modeling PCLK clock
, mPCLKPeriod (0)
, mReset      (false)
// internal attributes
, pWrapper    (pWrapper)
, mModuleName (iname)
, multi_timer (CONFIG::SUBBLOCK_NUM
              ,CONFIG::CLK_CNT_NUM
              ,CONFIG::EVT_NUM
              ,"multi_timer")
{
    /// connect to the wrapper
    sc_assert(pWrapper != NULL);
    pWrapper->CommandInit(pWrapper->basename());

    /// initialize the *register block* base class
    Catu5_timerg_regif::set_instance_name(string(pWrapper->basename()));

    /// initialize internal status attributes
    Initialize ();

    /// initialize states of internal counters
    state = CONFIG::NO_CLOCK;
}
//------------------------------------------------------------------------------
/**@brief public destructor
 * Detailed steps as follows:
 */
ATU5_TIMERG_Func::~ATU5_TIMERG_Func() {}
//------------------------------------------------------------------------------
/**@brief   Implement Reset operation of function model
 * @details this overrides default reset function of the register block super class
 *          Catu5_timerg_regifforwards reset status.
 * Detailed steps as follow:
 */
void ATU5_TIMERG_Func::EnableReset (const bool is_active) {
    /// set up input variable
    mReset = is_active;

    /// this class passes reset request to its super classes
    /// reset register block by invoking the overridden function
    /// because the timer did not prevent multiple reset quest, this class
    /// have to implement this check
    if (mReset && !InResetState()) {
        Catu5_timerg_regif::EnableReset(true);
    } else if (!is_active && InResetState()) {
        Catu5_timerg_regif::EnableReset(false);
    }

    /// this class passes reset request to its sub instances
    /// because the timer did not prevent multiple reset quest, this class
    /// have to implement this check
    if (mReset && !InResetState()) {
        multi_timer.enable_reset_async(true);
    } else if (!is_active && InResetState()) {
        multi_timer.enable_reset_async(false);
    }

    /// this class process cancel all events and initialize state when it's change
    /// into reset state from other states
    /// and then, change into reset state
    if (mReset && !InResetState()) {
        CancelEvents();
        Initialize();

        state = CONFIG::RESET;
    } else if (!mReset && InResetState()) {
        if (mPCLKPeriod == 0) {
            /// change into NO_CLOCK state
            state = CONFIG::NO_CLOCK;
        } else {
            /// change into STARTED or STOPPED state
            state = CONFIG::OPERATING;
        }
    }
}
//------------------------------------------------------------------------------
/// @todo: comment me
void ATU5_TIMERG_Func::Initialize () {
    /// bus clock periods (technically, they are not clock, so that
    /// they should be initialize when reset is active)
    for (uint8_t clkIdx = 0; clkIdx < CONFIG::CLK_CNT_NUM; ++clkIdx) {
        mClkBusPeriods[clkIdx] = 0;
    }
}
//------------------------------------------------------------------------------
/// @todo: comment me
void ATU5_TIMERG_Func::CancelEvents () {}
//------------------------------------------------------------------------------
/// @brief check if all sub-blocks of Timer G are in *RESET* state
bool ATU5_TIMERG_Func::InResetState () {
    return (state == CONFIG::RESET);
}
//------------------------------------------------------------------------------
/// @brief check if all sub-blocks of Timer G are in *NO_CLOCK* state
bool ATU5_TIMERG_Func::InNoClkState () {
    return (state == CONFIG::NO_CLOCK);
}
//------------------------------------------------------------------------------
bool ATU5_TIMERG_Func::isEnabledInterrupt(const uint8_t& sblkIdx) {
    return ((((unsigned int)((*TIERG)["CMPIEG"])) & (1 << sblkIdx)) != 0);
}
//------------------------------------------------------------------------------
/// @brief set clock PCLK of Timer G, this can move the timer G into *NO_CLOCK* state
/// or out to *STARTED* or *STOPPED* states
void ATU5_TIMERG_Func::setPCLKPeriod (const double& tperiod) {
    if (tperiod > 0) {
        mPCLKPeriod = tperiod;
        multi_timer.set_pclk_period(tperiod);
        if (!InResetState()) {
            /// change into STARTED or STOPPED state
            state = CONFIG::OPERATING;
        }
    } else {
        /// set clock to Zero if tperiod <= 0 and currently the model is not in No Clock state
        if (!InNoClkState()) {
            mPCLKPeriod = 0;
            multi_timer.set_pclk_period(0);
        }
        /// change into No clock state if the model currently is neither in Reset state nor No CLock state
        if (!InResetState() && !InNoClkState()) {
            state = CONFIG::NO_CLOCK;
        }
    }
}
//------------------------------------------------------------------------------
/**@brief handle command passed into the register block via Python interface
 *
 * @details
 *
 * @note this function is kept unchanged from other models
 */
void ATU5_TIMERG_Func::RegisterHandler (const bool                      is_com_reg
                                       ,const unsigned int              sub_id
                                       ,const std::vector<std::string>& args)
{
#if 0
    cout << endl << "@[" << std::setfill (' ') << std::setw(7) << sc_time_stamp().to_default_time_units() << "]";
    cout << "[file:"       << __FILE__             << "|line:"    << __LINE__        << "|func:" << __FUNCTION__ << "]";
    cout << "[is_com_reg:" << (uint32_t)is_com_reg << "|sub_id:" << (uint32_t)sub_id << "]";
    cout << "[arguments:";
    int argCnt = 1;
    for (std::vector<std::string>::const_iterator it  = args.begin();
                                                  it != args.end();
                                                ++it)
    {

        cout << argCnt << "@" << (*it).c_str() << " ";
        argCnt++;
    }
    cout <<  "]" << endl;
#endif //1

    std::vector<std::string> args_mod(args);
    if (!is_com_reg) {
        ostringstream regName;
        regName.str("");
        regName << args_mod[1] << (uint32_t)sub_id;
        args_mod[1] = regName.str();
    }

    std::string msg_return = "";
    if (args_mod.size() == 1) {
        //Dump registers name
        vpcl::re_register* reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    } else {
        if (args[1] == "*") {

            //Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register* reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args_mod;
                cmd.erase (cmd.begin()+1);
                cmd.insert(cmd.begin()+1, reg_p->name());
                msg_return = Catu5_timerg_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Ctsn_regif class
            if (  (  ((args.size() == 3) && (args[2] != "release")) // write command
                   || ((args.size() == 4) && (args[2] == "force"))   // force command
                  )
            ) {
                if (  ( (args[1] == "TSTRG")
                      &&is_com_reg
                      )
                   || ( ( (args[1] == "TCRG")
                        ||(args[1] == "TSRG")
                        ||(args[1] == "TSCRG")
                        ||(args[1] == "TCNTG")
                        ||(args[1] == "OCRG")
                        ||(args[1] == "RLDG")
                        )

                      &&(sub_id < CONFIG::SUBBLOCK_NUM)
                      )
                   )
                {
                    uint data = 0;
                    if (args[2] == "force") {
                        data = (uint) strtoul(args[3].c_str(), NULL, 0);
                    } else {
                        data = (uint) strtoul(args[2].c_str(), NULL, 0);
                    }

                    if (args[1] == "TSTRG") {
                        for (uint8_t channel_id = 0; channel_id < CONFIG::SUBBLOCK_NUM; ++channel_id) {
                            multi_timer.set_str_async((data >> channel_id) & 0x1,channel_id);
                        }
                    } else if (args[1] == "TCRG") {
                        multi_timer.set_ext_event_start_mode((data >> 0) & 0x1, sub_id);
                        multi_timer.set_clk_div_sel((data >> 4) & 0x7, sub_id);
                    } else if (args[1] == "TSRG") {
                        multi_timer.set_cmf ((data >> 0) & 0x1, sub_id);
                        multi_timer.set_ovff((data >> 1) & 0x1, sub_id);
                        multi_timer.set_ext_event_detect_flag((data >> 2) & 0x1, sub_id);
                    } else if (args[1] == "TSCRG") {
                        if (((data >> 0) & 0x1)!=0)
                            multi_timer.clear_cmf(sub_id);
                        if (((data >> 1) & 0x1)!=0)
                            multi_timer.clear_ovff(sub_id);   
                    } else if (args[1] == "TCNTG") {
                        multi_timer.set_cnt(data, sub_id);
                    } else if (args[1] == "OCRG") {
                        multi_timer.set_cor(data, sub_id);
                    } else if (args[1] == "RLDG") {
                        multi_timer.set_rl_val(data, sub_id);
                    }
                } // end-if
            } // end-if

            msg_return = Catu5_timerg_regif::reg_handle_command(args_mod);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        } // end-if-else
    } // end-if-else
}
//------------------------------------------------------------------------------
/**@brief read access implementation BusSlaveFuncIf::read via BusSlaveFuncIf
 */
void ATU5_TIMERG_Func::read (uint32_t         offsetAddress ///< [in]    offset address
                            ,TlmBasicPayload& trans         ///< [inout] transaction
                            ,BusTime_t*       t             ///< [inout] time
                            ,bool             debug         ///< [in]    debug access
                            )
{

    std::memset(trans.get_data_ptr()
               ,0
               ,trans.get_data_length()
               );
    if (!debug) {
        reg_rd((uint32_t)trans.get_address()
              ,          trans.get_data_ptr()
              ,          trans.get_data_length()
              );
    } else {
        reg_rd_dbg((uint32_t)trans.get_address()
                  ,          trans.get_data_ptr()
                  ,          trans.get_data_length()
                  );
    }
}
//------------------------------------------------------------------------------
/**@brief write access implementation of BusSlaveFuncIf::write via BusSlaveFuncIf
 */
void ATU5_TIMERG_Func::write(uint32_t          offsetAddress ///< [in] offset address
                            ,TlmBasicPayload&  trans         ///< [inout] transaction
                            ,BusTime_t*        t             ///< [inout] time
                            ,bool              debug         ///< [in] debug access
                            )
{
    if (!debug) {
        reg_wr((uint32_t)trans.get_address()
              ,          trans.get_data_ptr()
              ,          trans.get_data_length()
              );
    } else {
        reg_wr_dbg((uint32_t)trans.get_address()
                  ,          trans.get_data_ptr()
                  ,          trans.get_data_length()
                  );
    }
}
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *Start timer* field of
 *        *Timer start register*
 * @details see this line in Catu5_timerg_regif.cpp
 *          mWrCbAPI["TSTRG"]["STRG"] = &Catu5_timerg_regif::cb_TSTRG_STRG;
 */
void ATU5_TIMERG_Func::cb_TSTRG_STRG (RegCBstr str) {
    if (str.is_wr) {
        /// proceed write access
        if (!InResetState() && !InNoClkState()) {
            for (uint8_t channel_id = 0; channel_id < CONFIG::SUBBLOCK_NUM; ++channel_id) {
                multi_timer.set_str_async((((unsigned int)((*TSTRG)["STRG"])) & (1 << channel_id)) != 0  //TienT
                                         ,channel_id);
            }
            state = CONFIG::OPERATING;
        }
    }
};
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *External-event
 *        synchronization mode* field of *Timer control register*
 * @details see this line in Catu5_timerg_regif.cpp
 *          mWrCbAPI[str_tmp.str()]["EVSYMG"] = &Catu5_timerg_regif::cb_TCRG_EVSYMG;
 */
void ATU5_TIMERG_Func::cb_TCRG_EVSYMG (RegCBstr str) {
    if (str.is_wr) {
        /// proceed write access
        multi_timer.set_ext_event_start_mode(((bool)((*TCRG[str.channel])["EVSYMG"])),str.channel);  // TienT
        if (((bool)(*TCRG[str.channel])["EVSYMG"]) && (((unsigned int)(*TCRG[str.channel])["CKSELG"]) != 0x05)) {
            re_printf("warning", "Please select clock bus 5 (TCRG.CKSELG = 0b101) for external event sync mode\n");
        }
    }
};
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *clock selection* field
 *        of *Timer control register*
 * @details see this line in Catu5_timerg_regif.cpp
 *         mWrCbAPI[str_tmp.str()]["CKSELG"] = &Catu5_timerg_regif::cb_TCRG_CKSELG;
 */
void ATU5_TIMERG_Func::cb_TCRG_CKSELG (RegCBstr str) {
    if (str.is_wr) {
        /// proceed write access
        /// -# mirror value of compare-match register inside functional model
        ///    to the register block
        multi_timer.set_clk_div_sel((unsigned int)(*TCRG[str.channel])["CKSELG"]
                                   ,str.channel);
    }
};
//------------------------------------------------------------------------------
/**@brief implement a call-back of read access into *compare match flag* field
 *        of *Timer status register*
 * @details see this line in Catu5_timerg_regif.cpp
 *          mRdCbAPI[str_tmp.str()]["CMFG"] = &Catu5_timerg_regif::cb_TSRG_CMFG;
 */
void ATU5_TIMERG_Func::cb_TSRG_CMFG (RegCBstr str) {
    if (!str.is_wr) {
        /// proceed read access
        /// -# mirror value of compare-match register inside functional model
        ///    to the register block
        (*TSRG[str.channel])["CMFG"] = multi_timer.get_cmf(str.channel);
    }
};
//------------------------------------------------------------------------------
/**@brief implement a call-back of read access into *overflow flag* field
 *        of *Timer status register*
 * @details see this line in Catu5_timerg_regif.cpp
 *          mRdCbAPI[str_tmp.str()]["OVFG"] = &Catu5_timerg_regif::cb_TSRG_OVFG;
 */
void ATU5_TIMERG_Func::cb_TSRG_OVFG (RegCBstr str) {
    if (!str.is_wr) {
        /// proceed read access
        /// -# mirror value of compare-match register inside functional model
        ///    to the register block
        (*TSRG[str.channel])["OVFG"] = multi_timer.get_ovff(str.channel);
    }
};
//------------------------------------------------------------------------------
/**@brief implement a call-back of read access into *external event detected flag*
 *        field of *Timer status register*
 * @details see this line in Catu5_timerg_regif.cpp
 *          mRdCbAPI[str_tmp.str()]["EVDTFG"] = &Catu5_timerg_regif::cb_TSRG_EVDTFG;
 */
void ATU5_TIMERG_Func::cb_TSRG_EVDTFG (RegCBstr str) {
    if (!str.is_wr) {
        /// proceed read access
        (*TSRG[str.channel])["EVDTFG"] = multi_timer.get_ext_event_detect_flag(str.channel);
    }
};
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *compare-match clear*
 *        field of *Timer status clear register*
 * @details see this line in Catu5_timerg_regif.cpp
 *          mWrCbAPI[str_tmp.str()]["CMFCG"] = &Catu5_timerg_regif::cb_TSCRG_CMFCG;
 */
void ATU5_TIMERG_Func::cb_TSCRG_CMFCG (RegCBstr str) {
    if (str.is_wr) {
        /// proceed write access
        /// -# firstly, stop the timer
        /// -# clear compare-match status of timer
        /// -# and finally, set start/stop status of the timer to the status
        ///    stored inside *timer start registers*
        #if 0
        uint32_t value = (*TSCRG[str.channel])["CMFCG"];
        cout << endl << "@[" << std::setfill (' ') << std::setw(7) << sc_time_stamp().to_default_time_units() << "]";
        cout << "[" << "clear compare-match flag" << "]";
        cout << "[" << "str.pre_data" << (uint32_t)str.pre_data << "]";
        cout << "[" << "str.data"     << (uint32_t)str.data     << "]";
        cout << "[" << "bit_info"     << (uint32_t)value        << "]";
        #endif

        if ((!str.pre_data && str.data)!=0) {
            multi_timer.clear_cmf(str.channel);
        }
    }
};
//------------------------------------------------------------------------------
/**@brief implement a call-back of write access into *overflow clear*
 *        field of *Timer status clear register*
 * @details see this line in Catu5_timerg_regif.cpp
 *          mWrCbAPI[str_tmp.str()]["OVFCG"] = &Catu5_timerg_regif::cb_TSCRG_OVFCG;
 */
void ATU5_TIMERG_Func::cb_TSCRG_OVFCG (RegCBstr str) {
    if (str.is_wr) {
        /// proceed write access
        /// -# firstly, stop the timer
        /// -# clear overflow status of timer
        /// -# and finally, set start/stop status of the timer to the status
        ///    stored inside *timer start registers*
        #if 0
        uint32_t value = (*TSCRG[str.channel])["OVFCG"];
        cout << endl << "@[" << std::setfill (' ') << std::setw(7) << sc_time_stamp().to_default_time_units() << "]";
        cout << "[" << "clear overflow flag" << "]";
        cout << "[" << "str.pre_data" << (uint32_t)str.pre_data << "]";
        cout << "[" << "str.data"     << (uint32_t)str.data     << "]";
        cout << "[" << "bit_info"     << (uint32_t)value        << "]";
        #endif

        if ((!str.pre_data && str.data)!=0) {
            multi_timer.clear_ovff(str.channel);
        }
    }
};
//------------------------------------------------------------------------------
/**@brief implement a call-back of both read and write accesses into *counter*
 *        field of *Timer counter register*
 * @details see these lines in Catu5_timerg_regif.cpp
 *          mWrCbAPI[str_tmp.str()]["TCNTG"] = &Catu5_timerg_regif::cb_TCNTG_TCNTG;
 *          mRdCbAPI[str_tmp.str()]["TCNTG"] = &Catu5_timerg_regif::cb_TCNTG_TCNTG;
 *
 * Detailed steps as follow:
 */
void ATU5_TIMERG_Func::cb_TCNTG_TCNTG (RegCBstr str) {
    if (str.is_wr) {
        /// proceed write access
        /// -# firstly, stop the timer
        /// -# set value of counter register
        /// -# and finally, set start/stop status of the timer to the status
        ///    stored inside *timer start registers*
        multi_timer.set_cnt((*TCNTG[str.channel])["TCNTG"], str.channel);
    } else {
        /// proceed read access
        /// -# mirror value of counter register inside functional model
        ///    to the register block
        (*TCNTG[str.channel])["TCNTG"] = multi_timer.get_cnt(str.channel);
    }
};
//------------------------------------------------------------------------------
/**@brief implement a call-back of both read and write accesses into *compare
 *        match* field of *compare-match register*
 * @details see these lines in Catu5_timerg_regif.cpp
 *          mWrCbAPI[str_tmp.str()]["OCRG"] = &Catu5_timerg_regif::cb_OCRG_OCRG;
 *          mRdCbAPI[str_tmp.str()]["OCRG"] = &Catu5_timerg_regif::cb_OCRG_OCRG;
 *
 * Detailed steps as follow:
 */
void ATU5_TIMERG_Func::cb_OCRG_OCRG (RegCBstr str) {
    if (str.is_wr) {
        /// proceed write access
        /// -# firstly, stop the timer
        /// -# set value of compare-match register
        /// -# and finally, set start/stop status of the timer to the status
        ///    stored inside *timer start registers*
        multi_timer.set_cor((*OCRG[str.channel])["OCRG"]
                           ,str.channel);
        if ((unsigned int)(*OCRG[str.channel])["OCRG"] == 0) {
            re_printf("warning", "setting output compare 0 can cause undetermined behavior");
        }
    } else {
        /// proceed read access
        /// -# mirror value of compare-match register inside functional model
        ///    to the register block
        (*OCRG[str.channel])["OCRG"] = multi_timer.get_cor(str.channel);
    }
};
//------------------------------------------------------------------------------
/**@brief implement a call-back of write accesses into *reload
 *        value* field of *reload register*
 * @details see these lines in Catu5_timerg_regif.cpp
 *          mWrCbAPI[str_tmp.str()]["RLDG"] = &Catu5_timerg_regif::cb_RLDG_RLDG;
 * Detailed steps as follow:
 */
void ATU5_TIMERG_Func::cb_RLDG_RLDG (RegCBstr str) {
    if (str.is_wr) {
        /// proceed write access
        /// -# firstly, stop the timer
        /// -# set value of compare-match register
        /// -# and finally, set start/stop status of the timer to the status
        ///    stored inside *timer start registers*
        multi_timer.set_rl_val((*RLDG[str.channel])["RLDG"]
                              ,str.channel);
        if ((unsigned int)(*RLDG[str.channel])["RLDG"] == 0) {
            re_printf("warning", "setting output compare 0 can cause undetermined behavior");
        }
    }
};
//------------------------------------------------------------------------------
/**@brief implement a call-back of write accesses into *reload control* field
 *        of *reload control register*
 * @details see these lines in Catu5_timerg_regif.cpp
 *          mWrCbAPI[str_tmp.str()]["RLENG"] = &Catu5_timerg_regif::cb_RLDCRG_RLDCRG;
 * Detailed steps as follow:
 */
void ATU5_TIMERG_Func::cb_RLDCRG_RLENG (RegCBstr str) {
    if (str.is_wr) {
        /// proceed write access
        /// -# firstly, stop the timer
        /// -# set value of compare-match register
        /// -# and finally, set start/stop status of the timer to the status
        ///    stored inside *timer start registers*
        multi_timer.set_rl_en(((unsigned int)((*RLDCRG)["RLENG"])) &&  (1 << str.channel) // TienT
                             ,str.channel);
    }
};

//------------------------------------------------------------------------------
// EOF
