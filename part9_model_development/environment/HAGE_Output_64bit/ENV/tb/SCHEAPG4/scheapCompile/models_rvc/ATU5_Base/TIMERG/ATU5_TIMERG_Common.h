// ---------------------------------------------------------------------
// $Id: ATU5_TIMERG_Common.h 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2018-2019 Renesas Electronics Corporation
// Copyright(c) 2018-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef ATU5_TIMERG_ATU5_TIMERG_COMMON_H_
#define ATU5_TIMERG_ATU5_TIMERG_COMMON_H_

#include <systemc>
using namespace sc_core;
using namespace sc_dt;
#include <stdint.h>

#include <iostream>
using std::cout;
using std::endl;
#include <iomanip> // std::setw
#include <string>
using std::string;
#include <sstream>
using std::ostringstream;
using std::istringstream;
#include <vector>
using std::vector;



namespace CONFIG {
    enum const_values {
        // static structure
          SUBBLOCK_NUM = 10 ///< number of sub-block
        , CLK_CNT_NUM  = 6  ///< number of clock for counting
        , EVT_NUM      = 1  ///< number of external event for counting
        // operation mode
        , TIMER_CNT_MODE = 0
        , EVENT_CNT_MODE = 1
    };

    enum state_t {
          // state of timer
          RESET     = 0 ///< the counter is currently in reset state
        , NO_CLOCK  = 1 ///< clock of the model is gated
        , STOPPED   = 2 ///< the counter has been stopped, currently is in stopped state
        , STARTED   = 3 ///< the counter has been started, currently is in started state
        , OPERATING = 4 ///< the counter has been started, currently is in started state
    };
}

// forward declaration of classes in this package
class ATU5_TIMERG;
class ATU5_TIMERG_Func;

#endif /* ATU5_TIMERG_ATU5_TIMERG_COMMON_H_ */
//------------------------------------------------------------------------------
// EOF

