/**
 * @file mock_fsbus.h
 *
 * @date Created on: Oct 21, 2016
 * @author Author: Can Vy <can.vy.wh@rvc.renesas.com>
 * @brief UNIT TEST for fsbus.
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#include "gmock/gmock.h"
#include "./module_vc.h"

class MockFsBus {
public:
    MockFsBus () { }
    ~MockFsBus () { }
    MOCK_METHOD1(AddCyclicHandler, void (CModuleVc*));
    MOCK_METHOD1(RemoveCyclicHandler, void (CModuleVc*));
};
