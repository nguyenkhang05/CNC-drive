/**
 * @file mock_fsbus.h
 *
 * @date Created on: 2016
 * @author Author: YVo <y.vo.wg@rvc.renesas.com>
 * @brief UNIT TEST for INTC1.
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#pragma once

#include "gmock/gmock.h"
//#include "forest_common_api.h"
#include "forest_common.h"
#include "module_vc.h"
#include "intc2.h"
#include "fsbus.h"

class MockFsBus: public FsBus_IF {
public:
  MockFsBus() {};
  ~MockFsBus() {};
  MOCK_METHOD1(RemoveCyclicHandler, void (CModuleVc*));
  MOCK_METHOD1(AddCyclicHandler, void (CModuleVc*));

  //void AddCyclicHandler (CModuleVc* tgt_vc){}
  //void RemoveCyclicHandler (CModuleVc* tgt_vc){}
};
