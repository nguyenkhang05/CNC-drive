/**
 * @file mock_fsctrl.h
 *
 * @date Created on: 2016
  * @author Author: YVo <y.vo.wg@rvc.renesas.com>
 * @brief UNIT TEST for INTC1
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#pragma once

#include "gmock/gmock.h"
#include "proc_element.h"
#include "forest_common.h"
#include "forest_common_api.h"
class FsCtrl_interface;

class MockFsCtrl: public FsCtrl_interface {
public:
    MOCK_METHOD0(CancelFenmi, IntErrorCode (void));
    MOCK_METHOD0(CancelFeint, IntErrorCode (void));
    MOCK_METHOD1(ReqFenmi, IntErrorCode(void (*resfunc)(CIntc1* )));
    MOCK_METHOD2(ReqFeint, IntErrorCode(void (*resfunc)(uint32_t , CIntc1* ), uint32_t));
    MOCK_METHOD4(ReqInterrupt, IntErrorCode(uint32_t , uint32_t , bool, void (*resfunc)(uint32_t, CIntc1*)));
    MOCK_METHOD1(CancelInterrupt, IntErrorCode (uint32_t));
    ~MockFsCtrl(){}
};
