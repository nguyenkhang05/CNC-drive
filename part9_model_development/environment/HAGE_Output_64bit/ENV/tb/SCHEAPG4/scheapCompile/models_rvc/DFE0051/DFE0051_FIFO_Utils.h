// ---------------------------------------------------------------------
// $Id: DFE0051_FIFO_Utils.h,v 1.3 2017/11/13 08:48:22 binhnguyen Exp $
//
// Copyright(c) 2016-2017 Renesas Electronics Corporation
// Copyright(c) 2016-2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef DFE0051_FIFO_DFE0051_FIFO_UTILS_H_
#define DFE0051_FIFO_DFE0051_FIFO_UTILS_H_

#include <stdint.h>

#include <systemc>
using namespace sc_core;
using namespace sc_dt;

#include <string>
using std::string;
#include <sstream>
using std::ostringstream;
#include <map>
using std::map;

class DFE0051_FIFO_Utils {
public:
    //--------------------------------------------------------------------------
    /**@brief reset controller manages configurable-multiple reset source
     * @details This class declare a virtual reset handle function *HandleReset*,
     *  that tends to be override by the inheritance class.
     */
    class CrstCtrl {
    private:
        const uint8_t RST_SRC_NUM; ///< the number of reset sources
    private:
        bool* rstSrcActStates;     ///< array stores states of reset sources
        map<string,uint8_t> rstSrcs;
    public:
        //----------------------------------------------------------------------
        CrstCtrl(const uint8_t& iRST_SRC_NUM = 1);
        virtual ~CrstCtrl();
        //----------------------------------------------------------------------
        /// @brief this method checks if any reset source is active currently
        /// @return true if any reset source is active
        inline
        bool isRstActive() {
            for (uint8_t rstIdx = 0; rstIdx < RST_SRC_NUM; ++rstIdx)
                if (rstSrcActStates[rstIdx])
                    return true;
            return false;
        }
        //----------------------------------------------------------------------
        /// @brief this method checks if the given reset source is active currently
        /// @param[in] rstIdx index of the reset to be checked
        /// @return true if the given reset source is active
        inline
        bool isRstActive(const uint8_t& rstIdx) {
            if (rstIdx < RST_SRC_NUM) return (rstSrcActStates[rstIdx]);
            else                      return true;
        }
        //----------------------------------------------------------------------
        /// @brief this method checks if the given reset source is active currently
        /// @param[in] name name of the reset to be checked
        /// @return true if the given reset source is active
        inline
        bool isRstActive(const string& name) {
            if (rstSrcs.find(name) != rstSrcs.end()) return (rstSrcActStates[rstSrcs.at(name)]);
            else                                     return (rstSrcActStates[RST_SRC_NUM]);
        }
    protected:
        //----------------------------------------------------------------------
        /// @brief set name of the given reset
        inline
        bool setName (const uint8_t& rstIdx ///< [in] index of reset
                     ,const string&  name   ///< [in] desired name of reset
        ) {
            if (  rstSrcs.find(name) == rstSrcs.end() // the name must not be already existent
               && rstSrcs.size()     <  RST_SRC_NUM   // number of reset sources must be in a limited range
               && rstIdx             <  RST_SRC_NUM   // id of reset source must be in a limited range
               ) {
                 rstSrcs[name] = rstIdx;
                 return true;
             } else return false;
        }
        //----------------------------------------------------------------------
        /// @brief check if the given name of reset exists or not
        /// @param[in] name the given name to be checked
        /// @return true if the given name is found
        inline
        bool hasName (const string& name) {
            return (rstSrcs.find(name) != rstSrcs.end());
        }
        //----------------------------------------------------------------------
        /**@brief this allows reset sources to be activated or deactivated separately
         * @details the main function of reset control class is implemented in this
         *          method: the mechanism implemented here eliminates the case in which
         *          reset is active again and again. This mechanism acts as a pre-filter
         *          which only activate or deactivate reset process when all reset sources
         *          are stable.
         */
        inline
        void EnableRstSrc(const uint8_t& rstSrcIdx ///< [in] reset source index
                         ,const bool&    isRstAct  ///< [in] true if the selected reset source is active
                         ) {
            if (rstSrcIdx < RST_SRC_NUM)
                // reset source *rstSrcIdx* change state from active into inactive
                if (rstSrcActStates[rstSrcIdx] == true && isRstAct == false) {
                    rstSrcActStates[rstSrcIdx] = false;
                    if (!isRstActive())
                        HandleReset(false);
                }
                // reset source *rstSrcIdx* change state from inactive into active
                else if (rstSrcActStates[rstSrcIdx] == false && isRstAct == true) {
                    bool currRstAct = isRstActive();
                    rstSrcActStates[rstSrcIdx] = true;
                    if (!currRstAct) {
                        HandleReset(true);
                    }
                }
        }
        //----------------------------------------------------------------------
        /// @brief this allows reset sources to be activated or deactivated separately
        inline
        void EnableRstSrc(const string&  name     ///< [in] name of reset to be controlled
                         , const bool&   isRstAct ///< [in] true if the selected reset source is active
                         ) {
            if (rstSrcs.find(name) != rstSrcs.end()) EnableRstSrc(rstSrcs.at(name), isRstAct); // return true value
            else                                     EnableRstSrc(RST_SRC_NUM,      isRstAct); // return dummy value
        }
        //----------------------------------------------------------------------
        /**@brief dummy implementation of the virtual reset handle function
         * @details this tends to be override by the inheritance
         * @param isRstAct true if the any reset source is active,
         *                 false if all reset sources are inactive
         */
        virtual
        void HandleReset(const bool& isRstAct) {}
    };

    //--------------------------------------------------------------------------
    /**@brief clock controller manage configurable-multiple clock source
     * @details This class declare a virtual zero-clock handle function *HandleZeroClk*,
     *  that tends to be override by the inheritance class.
     */
    class CclkCtrl {
    //--------------------------------------------------------------------------
    public:
        enum FreqUnit {HZ  = 0
                      ,KHZ = 1
                      ,MHZ = 2
                      ,GHZ = 3
                      ,THZ = 4
                      };
        typedef sc_dt::uint64    uint64_t;
    //--------------------------------------------------------------------------
    private:
        uint64_t*   pFrequencies;   ///< frequency in Hz
        sc_time*    pPeriods;       ///< pPeriods in seconds
        sc_time*    pSyncPoints;    ///< synchronization point
        map<string,uint8_t> clks;
    private:
        const uint8_t CLK_SRC_NUM;
    //--------------------------------------------------------------------------
    public:
        //----------------------------------------------------------------------
        CclkCtrl(const uint8_t& iCLK_SRC_NUM = 1);
        virtual ~CclkCtrl();
        //----------------------------------------------------------------------
        inline
        bool setClkFreq (const uint8_t&  clkIdx
                        ,const uint64_t& iFreq
                        ) {
            if (clkIdx < CLK_SRC_NUM) {
                if (iFreq <= 0) {
                    pFrequencies[clkIdx] = 0;
                    pPeriods    [clkIdx] = sc_time(0, SC_SEC);
                    pSyncPoints [clkIdx] = sc_time(0, SC_SEC);
                    HandleZeroClk(clkIdx, true);
                } else {
                    pFrequencies[clkIdx] = iFreq;
                    pPeriods    [clkIdx] = sc_time(double(1)/iFreq, SC_SEC);
                    pSyncPoints [clkIdx] = sc_time_stamp();
                    HandleZeroClk(clkIdx, false);
                }
                return true;
            } else return false;
        }
        //----------------------------------------------------------------------
        inline
        bool setClkFreq (const string& name, const uint64_t& iFreq) {
            if (clks.find(name) != clks.end()) return setClkFreq(clks.at(name), iFreq);
            else                               return setClkFreq(CLK_SRC_NUM,   iFreq);
        }
        //----------------------------------------------------------------------
        inline
        uint64_t getClkFreq (const uint8_t& clkIdx) const {
            if (clkIdx < CLK_SRC_NUM) return pFrequencies[clkIdx];
            else                      return 0;
        }
        //----------------------------------------------------------------------
        inline
        uint64_t getClkFreq (const string& name) const {
            if (clks.find(name) != clks.end()) return (getClkFreq(clks.at(name))); // return true value
            else                               return (getClkFreq(CLK_SRC_NUM));   // return dummy value
        }
        //----------------------------------------------------------------------
        inline
        uint64_t getClkFreq (const uint8_t& clkIdx, const FreqUnit& outUnitType) const {
            if (clkIdx < CLK_SRC_NUM)
                switch (outUnitType) {
                    case KHZ: return (uint64_t)floor(0.5 + pFrequencies[clkIdx]*1e-03); break;
                    case MHZ: return (uint64_t)floor(0.5 + pFrequencies[clkIdx]*1e-06); break;
                    case GHZ: return (uint64_t)floor(0.5 + pFrequencies[clkIdx]*1e-09); break;
                    case THZ: return (uint64_t)floor(0.5 + pFrequencies[clkIdx]*1e-12); break;
                    default:  return                       pFrequencies[clkIdx];        break;
                }
            else return 0;
        }
        //----------------------------------------------------------------------
        inline
        uint64_t getClkFreq (const string& name, const FreqUnit& outUnitType) const {
            if (clks.find(name) != clks.end()) return (getClkFreq(clks.at(name), outUnitType)); // return true value
            else                               return (getClkFreq(CLK_SRC_NUM,   outUnitType)); // return dummy value
        }
        //----------------------------------------------------------------------
        inline
        sc_time getClkPeriod (const uint8_t& clkIdx) const {
            if (clkIdx < CLK_SRC_NUM) return pPeriods[clkIdx];
            else                      return sc_time(0, SC_NS);
        }
        //----------------------------------------------------------------------
        inline
        sc_time getClkPeriod (const string& name) const {
            if (clks.find(name) != clks.end()) return (getClkPeriod(clks.at(name))); // return true value
            else                               return (getClkPeriod(CLK_SRC_NUM  )); // return dummy value
        }
        //----------------------------------------------------------------------
        inline
        double getClkPeriod (const uint8_t&      clkIdx
                            ,const sc_time_unit& outUnitType)
        const {
            if (clkIdx < CLK_SRC_NUM)
                switch (outUnitType) {
                case SC_FS: return (pPeriods[clkIdx].to_seconds()*1e15);
                case SC_PS: return (pPeriods[clkIdx].to_seconds()*1e12);
                case SC_NS: return (pPeriods[clkIdx].to_seconds()*1e09);
                case SC_US: return (pPeriods[clkIdx].to_seconds()*1e06);
                case SC_MS: return (pPeriods[clkIdx].to_seconds()*1e03);
                default   : return (pPeriods[clkIdx].to_seconds());
                }
            else return 0;
        }
        //----------------------------------------------------------------------
        inline
        double getClkPeriod (const string& name, const sc_time_unit& outUnitType) const {
            if (clks.find(name) != clks.end()) return (getClkPeriod(clks.at(name), outUnitType)); // return true value
            else                               return (getClkPeriod(CLK_SRC_NUM  , outUnitType)); // return dummy value
        }
        //----------------------------------------------------------------------
        inline
        sc_time toNextSyncPoint (const uint8_t& clkIdx) {
            return(toNextSyncPoint(clkIdx, sc_time_stamp()));
        }
        //----------------------------------------------------------------------
        inline
        sc_time toNextSyncPoint (const string& name) {
            if (clks.find(name) != clks.end()) return (toNextSyncPoint(clks.at(name))); // return true value
            else                               return (toNextSyncPoint(CLK_SRC_NUM  )); // return dummy value
        }
        //----------------------------------------------------------------------
        inline
        sc_time toNextSyncPoint (const uint8_t& clkIdx, const sc_time& fromTimePoint) {
            if (clkIdx < CLK_SRC_NUM && pFrequencies[clkIdx] > 0) {
                double  period_num = 1 + floor((fromTimePoint - pSyncPoints[clkIdx]) / pPeriods[clkIdx]);
                return (pPeriods[clkIdx]*period_num - (fromTimePoint-pSyncPoints[clkIdx]));
            } else {
                return (sc_time(0, SC_NS));
            }
        }
        //----------------------------------------------------------------------
        inline
        sc_time toNextSyncPoint (const string& name, const sc_time& fromTimePoint) {
            if (clks.find(name) != clks.end()) return (toNextSyncPoint(clks.at(name), fromTimePoint)); // return true value
            else                               return (toNextSyncPoint(CLK_SRC_NUM  , fromTimePoint)); // return dummy value
        }
        //----------------------------------------------------------------------
        inline
        bool isZeroClk (const uint8_t& clkIdx) const {
            if (clkIdx < CLK_SRC_NUM) return ((pFrequencies[clkIdx]) <= 0 || (pPeriods[clkIdx] <= sc_time(0, SC_SEC)));
            else                      return true;
        }
        //----------------------------------------------------------------------
        inline
        bool isZeroClk (const string& name) const {
            if (clks.find(name) != clks.end()) return (isZeroClk(clks.at(name))); // return true value
            else                               return (isZeroClk(CLK_SRC_NUM  )); // return dummy value
        }
    protected:
        //----------------------------------------------------------------------
        inline
        bool setName (const uint8_t& clkIdx, const string& name) {
            if (  clks.find(name) == clks.end()   // the name must not be already existent
               && clks.size()     <  CLK_SRC_NUM  // number of clocks must be in a limited range
               && clkIdx          <  CLK_SRC_NUM  // id of clock must be in a limited range
               ) {
                clks[name] = clkIdx;
                return true;
            } else return false;
        }
        //----------------------------------------------------------------------
        inline
        bool hasName (const string& name) {
            return (clks.find(name) != clks.end());
        }
        //----------------------------------------------------------------------
        /**@brief dummy implementation of the virtual zero-clock handle function
         * @details this tends to be override by the inheritance
         * @param[in] clkIdx index of the clock source
         * @param[in] isZeroClk true if the selected clock source is zero/gated
         */
        virtual void HandleZeroClk (const uint8_t& clkIdx, const bool& isZeroClk) {}
    };

    //--------------------------------------------------------------------------
    /**@brief declare a role of the top that allows access via Python Interface
     */
    class PythonIf {
    public:
        virtual void AssertReset     (const std::string reset_name
                                     ,const double      start_time
                                     ,const double      period)         = 0;
        virtual void SetCLKFreq      (const std::string     clock_name
                                     ,const sc_dt::uint64   freq
                                     ,const std::string     unit)       = 0;
        virtual void GetCLKFreq      (const std::string     clock_name) = 0;
        virtual void ForceRegister   (const std::string     reg_name
                                     ,const unsigned int    reg_value)  = 0;
        virtual void ReleaseRegister (const std::string     reg_name)   = 0;
        virtual void WriteRegister   (const std::string     reg_name
                                     ,const unsigned int    reg_value)  = 0;
        virtual void ReadRegister    (const std::string reg_name)       = 0;
        virtual void ListRegister    (void)                             = 0;
        virtual void Help            (const std::string type) {}
        virtual void SetMessageLevel (const std::string msg_lv)         = 0;
        virtual void DumpRegisterRW  (const std::string is_enable)      = 0;
    public:
        PythonIf(){}
        virtual ~PythonIf(){}
    protected:
        //----------------------------------------------------------------------
        inline
        void SeparateString  (std::vector<std::string>& vtr
                             ,const std::string         msg) {
            std::istringstream cmd;
            cmd.str(msg);
            std::string temp = "a"; //Make temp to be not empty
            while (temp != "") {
                std::string command;
                cmd >> command;
                if (command != "") {
                    vtr.push_back(command);
                }
                temp = command;
            }
        }
        //----------------------------------------------------------------------
        inline
        bool ConvertClockFreq(sc_dt::uint64& freq_out
                             ,std::string&   unit_out
                             ,sc_dt::uint64  freq_in
                             ,std::string    unit_in) {
            if (unit_in == "GHz") {
                freq_out = freq_in * (sc_dt::uint64) (1.0e+9);
                unit_out = "GHz";
                return true;
            } else if (unit_in == "MHz") {
                freq_out = freq_in * (sc_dt::uint64) (1.0e+6);
                unit_out = "MHz";
                return true;
            } else if (unit_in == "KHz") {
                freq_out = freq_in * (sc_dt::uint64) (1.0e+3);
                unit_out = "KHz";
                return true;
            } else if (unit_in == "Hz") {
                freq_out = freq_in;
                unit_out = "Hz";
                return true;
            } else { // Frequency unit is wrong, frequency unit is set as Hz default.
                freq_out = freq_in;
                unit_out = "Hz";
                return false;
            }
        }
    };
    //--------------------------------------------------------------------------
    class Time {
    public:
        static inline
        uint64 toDouble(const sc_time&      iT
                       ,const sc_time_unit& expected_unit) {
            switch (expected_unit) {
            case SC_MS: return (iT.to_seconds() * 1e03);
            case SC_US: return (iT.to_seconds() * 1e06);
            case SC_NS: return (iT.to_seconds() * 1e09);
            case SC_PS: return (iT.to_seconds() * 1e12);
            case SC_FS: return (iT.to_seconds() * 1e15);
            default:    return (iT.to_seconds());
            }
        }
    private:
        Time();
        Time(Time& t);
        ~Time();
    };
    //--------------------------------------------------------------------------
    class StrProc {
    public:
        //----------------------------------------------------------------------
        template<typename T> static inline
        bool str2num(std::string str, T &num) {
            std::istringstream is(str);
            uint64_t tmp;

            if (str.substr(0, 2) == "0x") {
                is >> std::hex >> tmp;
            } else if (str.substr(0, 1) == "0") {
                is >> std::oct >> tmp;
            } else {
                is >> std::dec >> tmp;
            }

            if (is.fail() || !is.eof()) {
                return false;
            } else {
                num = (T) tmp;
                return true;
            }
        }
        //----------------------------------------------------------------------
        static inline
        bool str2dbl(std::string str, double &num) {
            std::istringstream is(str);
            double tmp;

            is >> tmp;

            if (is.fail() || !is.eof()) {
                return false;
            } else {
                num = tmp;
                return true;
            }
        }
        //----------------------------------------------------------------------
        static inline
        std::vector<std::string> str2vec(std::string str, const char sep) {
            std::vector<std::string> buf;
            int index = 0;

            for (int i = 0; i < (int) str.size(); i++) {
                if (str[i] == sep) {
                    if (i > index) {
                        buf.push_back(
                                str.substr((unsigned int) index,
                                        (unsigned int) (i - index)));
                    }
                    index = i + 1;
                }
            }
            if (index < (int) str.size()) {
                buf.push_back(str.substr((unsigned int) index));
            }
            return buf;
        }
        //----------------------------------------------------------------------
        static inline
        bool strmatch(const char *ptn, const char *str) {
            bool ret;

            if (ptn == NULL || str == NULL)
                return false;

            switch (*ptn) {
            case '\0':
                ret = '\0' == *str;
                break;
            case '*':
                ret = strmatch(&ptn[1], str)
                        || (('\0' != *str) && strmatch(ptn, &str[1]));
                break;
            case '?':
                ret = ('\0' != *str) && strmatch(&ptn[1], &str[1]);
                break;
            default:
                ret = ((unsigned char) *ptn == (unsigned char) *str)
                        && strmatch(&ptn[1], &str[1]);
                break;
            }
            return ret;
        }
    private:
        StrProc();
        StrProc(StrProc& t);
        ~StrProc();
    };
};

#endif /* DFE0051_FIFO_DFE0051_FIFO_UTILS_H_ */
