/**
 * @file mock_gerrstat.h
 *
 * @date Created on: 2016
 * @author Author: Quang Lam <hiroyuki.ono.jc@renesas.com>
 * @brief UNIT TEST for gerrstat.
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#include "gmock/gmock.h"
#include "treg_guard.h"

class MockGERRSTAT : public GERRSTAT_Interface {
public:

  MOCK_METHOD1(SetOVF, void (RegData));
  MOCK_METHOD1(SetERR, void (RegData));
  MOCK_CONST_METHOD0(GetName, const char* (void));
};
