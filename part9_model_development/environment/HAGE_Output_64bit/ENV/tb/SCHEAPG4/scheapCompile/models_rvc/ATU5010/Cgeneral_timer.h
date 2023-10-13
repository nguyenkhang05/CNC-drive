// -----------------------------------------------------------------------------
// $Id: Cgeneral_timer.h v2017_07_31 $
//
// Copyright(c) 2008, 2012 Renesas Electronics Corporation
// Copyright(c) 2008, 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------

#ifndef _C_General_Timer_H_
#define _C_General_Timer_H_

#include <systemc.h>
#include <list>
using namespace std;

class Cgeneral_timer;

#define _NUM_1_ 1U
#define _NUM_2_ 2U
#define _NUM_3_ 3U
#define _NUM_255_ 255U

#define _CHARS_PER_UINT_ 8U
#define _BITS_PER_UINT_ 32U

#define _MAX_UINT_ 0xFFFFFFFFU

#define _ERR_NUM_  0U

#define _GT_NO_EVENT_           0U
#define _GT_CMP_MATCH_EVENT_    1U
#define _GT_UNDERFLOW_EVENT_    2U
#define _GT_OVERFLOW_EVENT_     4U

#define _GT_DEF_TIME_UNIT_ SC_NS
#define _GT_DEF_CLK_PERIOD_ 5U

class Cgeneral_timer: public sc_module
{
    public:
        enum counter_clear_type {emProhibit, emInFactor, emExFactor, emBothInExFactor};
        enum operation_mode     {emFreeRun, emOneShot};
        enum counter_direction  {emCountUp, emCountDown};
        enum detect_edge_mode   {emRising, emFalling, emBothEdge};
        enum cmp_match_out_type {emLowActive, emHighActive, emToggle};

        struct ControlRegister
        {
            bool gtCMIE;              // compare match interrupt enable/disable
            bool gtOVIE;              // overflow interrupt enable/disable
            bool gtICPE;              // input capture interrupt enable/disable
            bool gtUNIE;              // underflow interrupt enable/disable
            unsigned int gtCKS;       // select inner counter
            detect_edge_mode gtCKEG;  // setting counting clock edge
            bool gtSTR;               // start/stop timer
            bool gtIE;                // enable/disable input 
            counter_clear_type gtCCLR;// clear counter
            unsigned int gtCMS;       // setting size of timer
            operation_mode gtCMM;     // setting operation mode
        };

        struct StatusRegister
        {
            bool gtCMF;               // compare match flag
            bool gtOVF;               // overflow flag
            bool gtICF;               // input capture flag
            bool gtUNF;               // underflow flag

            unsigned int gtNumOfCMF;  // store the number of compare match
            unsigned int gtNumOfOVF;  // store the number of overflow
            unsigned int gtNumOfUNF;  // store the number of underflow
            unsigned int gtNumOfICF;  // store the number of input capture
        };

    public :

        // -------- Module Constructor -------- //
        SC_HAS_PROCESS (Cgeneral_timer);
        Cgeneral_timer (sc_module_name name) :
            sc_module (name     )
    {
        //{{{ construct
        clk_period = _GT_DEF_CLK_PERIOD_;
        time_unit = _GT_DEF_TIME_UNIT_;
        wait_to_start = 0;
        start_time = sc_time(0, time_unit);
        start_time_tmp = sc_time(0, time_unit);
        mLocalTime = sc_time(0, time_unit);
        mLocalAccessTime = sc_time(0, time_unit);
        start_local_time = sc_time(0, time_unit);
        callStartStop_time = 0;

        isCascadeMaster = true;
        mSupportDecoupling = false;
        numOfCascadedItems = _NUM_1_;

        cascadedList.clear ();
        cascadedList.push_back (this);

        // reset compare match output type
        cmpMatchOutType = emLowActive;
        // reset compare match output value
        cmpMatchOut = true;

        // reset counter direction
        direction = emCountUp;

        // reset gtCR value
        gtCR.gtCMIE = false;
        gtCR.gtOVIE = false;
        gtCR.gtUNIE = false;
        gtCR.gtICPE = false;
        gtCR.gtCKS  = 1UL;
        gtCR.gtCKEG = emRising;
        gtCR.gtSTR  = false;
        gtCR.gtIE   = false;
        gtCR.gtCMM  = emOneShot;
        gtCR.gtCMS = _BITS_PER_UINT_;
        gtCR.gtCCLR = emProhibit;

        // re-calculate length of arrays based on CMS value
        length = 1UL;
        previous_dif_value = 0;

        gtCNT = new unsigned int [length];
        if (gtCNT != NULL) {
            gtCNT[0] = 0;
        }

        gtCOR = new unsigned int [length];
        if (gtCOR != NULL) {
            gtCOR[0] = _MAX_UINT_;
        }

        gtPR = new unsigned int [length];
        if (gtPR != NULL) {
            gtPR[0] = 0;
        }

        // reset gtSR value
        clearAllFlag ();
        interruptNeedCalled = false;

        // initialize mIsReset
        mIsReset = false;

        isCMatSTR = false;

        calcCountingPeriod();
        //}}}

        // -------- Model Operation -------- //
        SC_METHOD (continueCount_method);
        dont_initialize ();
        sensitive << continueCountEvent;

        SC_METHOD (specificEvent_method);
        dont_initialize ();
        sensitive << specificEvent;
    } // constructor

        //--------------- cascade APIs --------------------//
        void cascadeTimer (Cgeneral_timer *timer);  // Cascade function
        void breakCascadedList ();                  // Break down the cascaded list to single channels
        void removeTimerFromList ();                // Remove this timer from cascaded list

        ~Cgeneral_timer ();

        void EnableReset(bool is_active);

        //--------------- APIs for temporal decoupling ----------//
        void setLocalTime (const sc_time ltime);
        void enableSupportDecoupling (const bool isDecoupling);

    protected:
        //--------------- Methods --------------------//
        void initialize ();                         // Initialize all values of general timer
        void construct ();                          // Construct all variables in constructor

        //--------------- APIs for configuration --------------------//
        void setCounterDirection (counter_direction value); // set the counter direction (count up or count down)
        void setCmpMatchOutType (cmp_match_out_type value); // set compare match output type (0, 1, or toggle)
        counter_direction getCounterDirection ();           // get the counter direction
        cmp_match_out_type getCmpMatchOutType ();           // get compare match output type (0, 1, or toggle)


        //--------------- APIs set/get registers' value --------------------//
        void setCNT (unsigned int value);   // Set CNT value with size under or equal 32 bits
        void setCOR (unsigned int value);   // Set COR value with size under or equal 32 bits
        void clearPR ();                       // Clear PR value

        unsigned int getCNT ();             // Get CNT value with size under or equal 32 bits
        unsigned int getCOR ();             // Get COR value with size under or equal 32 bits
        unsigned int getPR ();              // Get PR value with size under or equal 32 bits

        void clearAllFlag ();              // Clear all flag of status register
        void clearCMF ();                  // Clear compare match flag
        void clearOVF ();                  // Clear overflow flag
        void clearICF ();                  // Clear input capture flag
        void clearUNF ();                  // Clear underflow flag
        StatusRegister getSR ();           // Get status register value

        void setCmpMatchOutVal (bool value); // Set value of compare match output
        bool getCmpMatchOutVal ();           // Get value of compare match output

        ControlRegister getCR ();               // Get control register value
        void setCR   (ControlRegister *value);  // Set control register value

        void setCMIE (bool value);              // Set value to CMIE to enable/disable interrupt of compare match
        void setOVIE (bool value);              // Set value to OVIE to enable/disable interrupt of overflow
        void setICPE (bool value);              // Set value to ICPE to enable/disable interrupt of input capture
        void setUNIE (bool value);              // Set value to UNIE to enable/disable interrupt of underflow
        void setCKS  (unsigned int value);      // Set value to CKS to divide the input clock for counting
        void setCKEG (detect_edge_mode value);  // Set value to CKEG to set clock edge detection
        void setSTR  (bool value);              // Set value to STR to start/stop counting
        void setIE   (bool value);              // Set value to IE to enable/disable input capture
        void setCCLR (counter_clear_type value);// Set value to CCLR to set clear mode
        void setCMS  (unsigned int value);      // Set value to CMS to set the size of counter
        void setCMM  (operation_mode value);    // Set value to CMM to set operation mode

        void setBigValueCNT (unsigned int *value);   // Set CNT value with size over 32 bits
        void setBigValueCOR (unsigned int *value);   // Set COR value with size over 32 bits

        void getBigValueCNT (unsigned int *value);   // Get CNT value with size over 32 bits
        void getBigValueCOR (unsigned int *value);   // Get COR value with size over 32 bits
        void getBigValuePR (unsigned int *value);    // Get PR value with size over 32 bits

        void getCNTOfCascadedList (unsigned int *value);    // Get CNT value of cascaded list (big number)
        void getCOROfCascadedList (unsigned int *value);    // Get COR value of cascaded list (big number)
        void getPROfCascadedList (unsigned int *value);     // Get PR value of cascaded list (big number)

        //--------------- Virtual Methods --------------------//
        virtual void cmpMatchOutputHandling (bool value);   // Handle compare match output event
        virtual void interruptHandling ();                  // Handle interrupt event

        //--------------- other APIs --------------------//
        void inputCapture ();                               // Capture the value of CNT
        void dumpProfile (unsigned int type, FILE *fp);     // Dump profile function

        //--------------- set/get clock period ----------//
        void setClkCountPeriod (double clk_period);   // Set clock count period
        double getClkCountPeriod ();                  // Get clock count period
        void setTimeUnit (sc_time_unit time_unit);          // Set time unit
        sc_time_unit getTimeUnit ();                        // Get time unit

    private:
        //--------------- Variables --------------------//
        cmp_match_out_type cmpMatchOutType;     // compare match output value
        counter_direction direction;            // counter direction

        bool cmpMatchOut;                       // store the compare match output value
        bool interruptNeedCalled;               // mark that interruptHandling should be called
        bool isCMatSTR;                         // check compare match at the start time

        unsigned int length;                    // store the length of CNT, COR, PR value
        unsigned int previous_dif_value;        // store the added value to CNT while counting

        list <Cgeneral_timer*> cascadedList;    // store cascaded list of timers

        double clk_period;                // store the clock counting period
        sc_time_unit time_unit;           // store the time unit
        double wait_to_start;             // Used for counting based on negedge
        sc_time start_time;                // store the time that timer is started
        double counting_period;           // store the period of counting
        double callStartStop_time;        // store the time to avoid call StartStop many time at a same time

        //--------------- Variables for temporal decoupling --------------------//
        sc_time mLocalTime;               // store local offset time which is set by setLocalTime
        sc_time start_time_tmp;               // store simulation time at theSTR write 1
        sc_time mLocalAccessTime;         // store local access time which equal simulation time + local offset time
        sc_time start_local_time;         // store local offset time at the begining of a quantum
        bool mSupportDecoupling;             // show whether running is temporal decoulping or normal mode

        //--------------- Event ------------------------//
        sc_signal<sc_uint<32> > specificEventValue;    // used to determine _GT_OVERFLOW_EVENT_ or _GT_UNDERFLOW_EVENT_ or _GT_CMP_MATCH_EVENT_
        sc_event specificEvent;             // Specific Event
        sc_event continueCountEvent;        // Continue Count Event

        //--------------- Registers --------------------//
        unsigned int *gtCNT;                   // CNT value
        ControlRegister gtCR;                  // CR value
        StatusRegister  gtSR;                  // SR value
        unsigned int *gtCOR;                   // COR value
        unsigned int  *gtPR;                   // PR value

        //--------------- Variables for cascade--------------------//
        bool isCascadeMaster;                // show whether this is the master or slave of cascaded list

        unsigned int numOfCascadedItems;     // store the number of items in the cascaded list

        //--------------- Thread --------------------//
        void continueCount_method (void);
        void specificEvent_method (void);

        //--------------- other APIs --------------------//
        // compare num1 with num2
        int compareBigNumber (unsigned int *num1, unsigned int *num2, unsigned int bit_length);
        // add two big numbers
        unsigned int addBigNumber (unsigned int *num1, unsigned int *num2, unsigned int bit_length, unsigned int *result, unsigned int remember);
        // subtract two big numbers
        unsigned int subtractBigNumber (unsigned int *num1, unsigned int *num2, unsigned int bit_length, unsigned int *result, unsigned int remember);

        // copy num1 to num2
        void copyBigNumber (unsigned int *num1, unsigned int *num2, unsigned int bit_length);
        // reset num with direction
        void resetBigNumber (unsigned int *num, unsigned int bit_length, counter_direction direction);
        // convert big number to hex string
        void converBigNumberToHexStr (unsigned int *num, char *str, unsigned int bit_length);
        // resize big number
        void resizeBigNumber (unsigned int **num, unsigned int old_bit_length, unsigned int new_bit_length);

        // dump profile type 1
        void dumpProfileType1 (FILE *fp);
        // dump profile type 2
        void dumpProfileType2 (FILE *fp);

        // check interrupt
        void check_interrupt ();

        // start/stop counter
        void StartStop ();
        // update CNT value based on start time to current time
        void updateCNTValue ();
        // reset CNT value
        void resetCNTValue ();
        // calculating Counting period
        void calcCountingPeriod ();

        // show during reset process.
        bool mIsReset ;

        void Reset(bool is_constractor);

}; // class

#endif
// vim600: set foldmethod=marker :
