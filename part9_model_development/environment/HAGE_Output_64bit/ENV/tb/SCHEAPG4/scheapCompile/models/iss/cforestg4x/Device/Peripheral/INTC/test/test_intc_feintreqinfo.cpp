/**
 * @file test_intc_feintreqinfo.cpp
 *
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


class FeintReqInfoTest
    : public testing::Test {
protected:
    FeintReqInfoTest() {}
  virtual ~FeintReqInfoTest() {}
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
  static const PhysAddr FIBG_OFFSET  = 0x000000600UL;    //dummy number
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + DBMK_OFFSET;
  uint8_t data_input[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  PhysAddr fibg_addr = base_addr + FIBG_OFFSET;

private:
};


TEST_F(FeintReqInfoTest, Check_GetMaskTest1) {
  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();
  dbmk->SetFI(1);

  TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, 0xfffc00c0, 0, intc1);
  fic->Reset();

  TRegFIBD *fibd;
  fibd = new TRegFIBD (SIZE_32BIT, SIZE_8BIT, 0xfffc01c0, TRegMask(TRegBND<FIBD>::FBD_MASK), 0, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  fibd->Reset();

  TRegFIBG* fibg;
  fibg = new TRegFIBG (SIZE_32BIT, SIZE_8BIT, fibg_addr, intc1);


  FeintReqInfo* intreq_info = new FeintReqInfo (fic, fibd, dbmk, fibg);
  RegData data = intreq_info->GetMask();

  ASSERT_EQ(0x1U, data);

  delete intreq_info;
  delete dbmk;
  delete fic;
  delete fibd;
}

TEST_F(FeintReqInfoTest, Check_GetMaskTest2) {
  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();
  dbmk->SetFN(0);

  TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, 0xfffc00c0, 0, intc1);
  fic->Reset();

  TRegFIBD *fibd;
  fibd = new TRegFIBD (SIZE_32BIT, SIZE_8BIT, 0xfffc01c0, TRegMask(TRegBND<FIBD>::FBD_MASK), 0, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  fibd->Reset();

  TRegFIBG* fibg;
  fibg = new TRegFIBG (SIZE_32BIT, SIZE_8BIT, fibg_addr, intc1);

  FeintReqInfo* intreq_info = new FeintReqInfo (fic, fibd, dbmk, fibg);
  RegData data = intreq_info->GetMask();
  ASSERT_EQ(0x0U, data);

  delete intreq_info;
  delete dbmk;
  delete fic;
  delete fibd;
}

TEST_F(FeintReqInfoTest, Check_GetPriority) {
  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, 0xfffc00c0, 0, intc1);
  fic->Reset();

  TRegFIBD *fibd;
  fibd = new TRegFIBD (SIZE_32BIT, SIZE_8BIT, 0xfffc01c0, TRegMask(TRegBND<FIBD>::FBD_MASK), 0, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  fibd->Reset();

  TRegFIBG* fibg;
  fibg = new TRegFIBG (SIZE_32BIT, SIZE_8BIT, fibg_addr, intc1);

  FeintReqInfo* intreq_info = new FeintReqInfo (fic, fibd, dbmk, fibg);
  int32_t data = intreq_info->GetPriority();

  ASSERT_EQ(-16, data);

  delete intreq_info;
  delete dbmk;
  delete fic;
  delete fibd;
}

TEST_F(FeintReqInfoTest, Check_GetPeidTest1) {
  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, 0xfffc00c0, 0, intc1);
  fic->Reset();

  TRegFIBD *fibd;
  fibd = new TRegFIBD (SIZE_32BIT, SIZE_8BIT, 0xfffc01c0, TRegMask(TRegBND<FIBD>::FBD_MASK), 0, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  fibd->Reset();
  fibd->SetPEID(1);

  TRegFIBG* fibg;
  fibg = new TRegFIBG (SIZE_32BIT, SIZE_8BIT, fibg_addr, intc1);

  FeintReqInfo* intreq_info = new FeintReqInfo (fic, fibd, dbmk, fibg);
  RegData data = intreq_info->GetPEID();

  ASSERT_EQ(0x1U, data);

  delete intreq_info;
  delete dbmk;
  delete fic;
  delete fibd;
}

TEST_F(FeintReqInfoTest, Check_GetPeidTest2) {
  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, 0xfffc00c0, 0, intc1);
  fic->Reset();

  TRegFIBD *fibd;
  fibd = new TRegFIBD (SIZE_32BIT, SIZE_8BIT, 0xfffc01c0, TRegMask(TRegBND<FIBD>::FBD_MASK), 0, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  fibd->Reset();
  fibd->SetPEID(2);

  TRegFIBG* fibg;
  fibg = new TRegFIBG (SIZE_32BIT, SIZE_8BIT, fibg_addr, intc1);

  FeintReqInfo* intreq_info = new FeintReqInfo (fic, fibd, dbmk, fibg);
  RegData data = intreq_info->GetPEID();

  ASSERT_EQ(0x2U, data);

  delete intreq_info;
  delete dbmk;
  delete fic;
  delete fibd;
}


TEST_F(FeintReqInfoTest, Check_GetTcId) {
  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, 0xfffc00c0, 0, intc1);
  fic->Reset();

  TRegFIBD *fibd;
  fibd = new TRegFIBD (SIZE_32BIT, SIZE_8BIT, 0xfffc01c0, TRegMask(TRegBND<FIBD>::FBD_MASK), 0, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  fibd->Reset();

  TRegFIBG* fibg;
  fibg = new TRegFIBG (SIZE_32BIT, SIZE_8BIT, fibg_addr, intc1);

  FeintReqInfo* intreq_info = new FeintReqInfo (fic, fibd, dbmk, fibg);
  int32_t data = intreq_info->GetTcId();

  ASSERT_EQ(-1, data);

  delete intreq_info;
  delete dbmk;
  delete fic;
  delete fibd;
}

TEST_F(FeintReqInfoTest, Check_IsLevelDetectTest2) {
  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, 0xfffc00c0, 0, intc1);
  fic->Reset();
  fic->SetFICT(1);

  TRegFIBD *fibd;
  fibd = new TRegFIBD (SIZE_32BIT, SIZE_8BIT, 0xfffc01c0, TRegMask(TRegBND<FIBD>::FBD_MASK), 0, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  fibd->Reset();

  TRegFIBG* fibg;
  fibg = new TRegFIBG (SIZE_32BIT, SIZE_8BIT, fibg_addr, intc1);

  FeintReqInfo* intreq_info = new FeintReqInfo (fic, fibd, dbmk, fibg);
  bool data = intreq_info->IsLevelDetect();

  ASSERT_EQ(true, data);

  delete intreq_info;
  delete dbmk;
  delete fic;
  delete fibd;
}

TEST_F(FeintReqInfoTest, Check_IsLevelDetectTest1) {
  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, 0xfffc00c0, 0, intc1);
  fic->Reset();
  fic->SetFICT(0);

  TRegFIBD *fibd;
  fibd = new TRegFIBD (SIZE_32BIT, SIZE_8BIT, 0xfffc01c0, TRegMask(TRegBND<FIBD>::FBD_MASK), 0, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  fibd->Reset();

  TRegFIBG* fibg;
  fibg = new TRegFIBG (SIZE_32BIT, SIZE_8BIT, fibg_addr, intc1);

  FeintReqInfo* intreq_info = new FeintReqInfo (fic, fibd, dbmk, fibg);
  bool data = intreq_info->IsLevelDetect();

  ASSERT_EQ(true, !data);

  delete intreq_info;
  delete dbmk;
  delete fic;
  delete fibd;
}


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
