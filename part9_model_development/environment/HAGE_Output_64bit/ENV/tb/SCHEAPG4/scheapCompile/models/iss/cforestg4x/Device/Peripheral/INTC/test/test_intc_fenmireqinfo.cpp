/**
 * @file test_intc_fenmireqinfo.cpp
 * @date Created on: 2016
 * @author Author: YVo <y.vo.wg@rvc.renesas.com>
 * @brief UNIT TEST for INTC1. (SAMPLE)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */

#include <string.h>
#include <vector>
#include <memory>
#include <map>
#include <stdexcept>
#include "./config.h"
#include "./fs_controler.h"
#include "./intc1.h"
#include "./exp_info.h"
#include "./fsbus.h"
#include "./proc_element.h"
#include "./forest.h"
#include "mock_sideband.h"
#include "gmock/gmock.h"
#include "./forest_common.h"
#include "./intc_api.h"
#include "./module_vc.h"
#include "./treg.h"
#include "intc1.h"
#include "./intc2.h"

using ::testing::_;
using ::testing::Return;
using ::testing::AtLeast;

using ::testing::ExpectationSet;
using ::testing::Expectation;


class FenmiReqInfoTest
    : public testing::Test {
protected:
    FenmiReqInfoTest() {}
  virtual ~FenmiReqInfoTest() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
#if (_INTC1_20)
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);
#else
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x10);
#endif

    EXPECT_CALL((*mock_pe), GetTcLastId()).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(AtLeast(0));
  }
  virtual void TearDown() { delete intc1;
                              delete mock_fsbus;
                              delete mock_pe;}

public:
  MockFsBus* mock_fsbus;
  SideBand sideband;
  MockProcElement *mock_pe;
  CIntc1 *intc1;
  uint32_t pe_id = 0;

  static const PhysAddr DBMK_OFFSET  = 0x000000080UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + DBMK_OFFSET;
  uint8_t data_input[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

private:
};


TEST_F(FenmiReqInfoTest, Check_GetMaskTest1) {
  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();
  dbmk->SetFN(1);

  TRegFNC* fnc = new TRegFNC (SIZE_16BIT, SIZE_8BIT, 0xfffc00a0, intc1);
  fnc->Reset();

  TRegFNBD *fnbd;
  fnbd = new TRegFNBD (SIZE_32BIT, SIZE_8BIT, 0xfffc01a0, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  fnbd->Reset();

  FenmiReqInfo* intreq_info = new FenmiReqInfo (fnc, fnbd, dbmk);
  RegData data = intreq_info->GetMask();

  ASSERT_EQ(0x1U, data);

  delete intreq_info;
  delete dbmk;
  delete fnc;
  delete fnbd;
}

TEST_F(FenmiReqInfoTest, Check_GetMaskTest2) {
  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();
  dbmk->SetFN(0);

  TRegFNC* fnc = new TRegFNC (SIZE_16BIT, SIZE_8BIT, 0xfffc00a0, intc1);
  fnc->Reset();

  TRegFNBD *fnbd;
  fnbd = new TRegFNBD (SIZE_32BIT, SIZE_8BIT, 0xfffc01a0, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  fnbd->Reset();

  FenmiReqInfo* intreq_info = new FenmiReqInfo (fnc, fnbd, dbmk);
  RegData data = intreq_info->GetMask();

  ASSERT_EQ(0x0U, data);

  delete intreq_info;
  delete dbmk;
  delete fnc;
  delete fnbd;
}

TEST_F(FenmiReqInfoTest, Check_GetPriority) {
  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  TRegFNC* fnc = new TRegFNC (SIZE_16BIT, SIZE_8BIT, 0xfffc00a0, intc1);
  fnc->Reset();

  TRegFNBD *fnbd;
  fnbd = new TRegFNBD (SIZE_32BIT, SIZE_8BIT, 0xfffc01a0, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  fnbd->Reset();

  FenmiReqInfo* intreq_info = new FenmiReqInfo (fnc, fnbd, dbmk);
  int32_t data = intreq_info->GetPriority();

  ASSERT_EQ(-17, data);

  delete intreq_info;
  delete dbmk;
  delete fnc;
  delete fnbd;
}

TEST_F(FenmiReqInfoTest, Check_GetPeidTest1) {
  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  TRegFNC* fnc = new TRegFNC (SIZE_16BIT, SIZE_8BIT, 0xfffc00a0, intc1);
  fnc->Reset();

  TRegFNBD *fnbd;
  fnbd = new TRegFNBD (SIZE_32BIT, SIZE_8BIT, 0xfffc01a0, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  fnbd->Reset();
  fnbd->SetPEID(1);

  FenmiReqInfo* intreq_info = new FenmiReqInfo (fnc, fnbd, dbmk);
  RegData data = intreq_info->GetPEID();

  ASSERT_EQ(0x1U, data);

  delete intreq_info;
  delete dbmk;
  delete fnc;
  delete fnbd;
}

TEST_F(FenmiReqInfoTest, Check_GetPeidTest2) {
  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  TRegFNC* fnc = new TRegFNC (SIZE_16BIT, SIZE_8BIT, 0xfffc00a0, intc1);
  fnc->Reset();

  TRegFNBD *fnbd;
  fnbd = new TRegFNBD (SIZE_32BIT, SIZE_8BIT, 0xfffc01a0, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  fnbd->Reset();
  fnbd->SetPEID(2);

  FenmiReqInfo* intreq_info = new FenmiReqInfo (fnc, fnbd, dbmk);
  RegData data = intreq_info->GetPEID();

  ASSERT_EQ(0x2U, data);

  delete intreq_info;
  delete dbmk;
  delete fnc;
  delete fnbd;
}


TEST_F(FenmiReqInfoTest, Check_GetTcId) {
  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  TRegFNC* fnc = new TRegFNC (SIZE_16BIT, SIZE_8BIT, 0xfffc00a0, intc1);
  fnc->Reset();

  TRegFNBD *fnbd;
  fnbd = new TRegFNBD (SIZE_32BIT, SIZE_8BIT, 0xfffc01a0, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  fnbd->Reset();

  FenmiReqInfo* intreq_info = new FenmiReqInfo (fnc, fnbd, dbmk);
  int32_t data = intreq_info->GetTcId();

  ASSERT_EQ(-1, data);

  delete intreq_info;
  delete dbmk;
  delete fnc;
  delete fnbd;
}

TEST_F(FenmiReqInfoTest, Check_IsLevelDetectTest2) {
  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  TRegFNC* fnc = new TRegFNC (SIZE_16BIT, SIZE_8BIT, 0xfffc00a0, intc1);
  fnc->Reset();
  fnc->SetFNCT(1);

  TRegFNBD *fnbd;
  fnbd = new TRegFNBD (SIZE_32BIT, SIZE_8BIT, 0xfffc01a0, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  fnbd->Reset();

  FenmiReqInfo* intreq_info = new FenmiReqInfo (fnc, fnbd, dbmk);
  bool data = intreq_info->IsLevelDetect();

  ASSERT_EQ(true, data);

  delete intreq_info;
  delete dbmk;
  delete fnc;
  delete fnbd;
}

TEST_F(FenmiReqInfoTest, Check_IsLevelDetectTest1) {
  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  TRegFNC* fnc = new TRegFNC (SIZE_16BIT, SIZE_8BIT, 0xfffc00a0, intc1);
  fnc->Reset();
  fnc->SetFNCT(0);

  TRegFNBD *fnbd;
  fnbd = new TRegFNBD (SIZE_32BIT, SIZE_8BIT, 0xfffc01a0, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  fnbd->Reset();

  FenmiReqInfo* intreq_info = new FenmiReqInfo (fnc, fnbd, dbmk);
  bool data = intreq_info->IsLevelDetect();

  ASSERT_EQ(true, !data);

  delete intreq_info;
  delete dbmk;
  delete fnc;
  delete fnbd;
}


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
