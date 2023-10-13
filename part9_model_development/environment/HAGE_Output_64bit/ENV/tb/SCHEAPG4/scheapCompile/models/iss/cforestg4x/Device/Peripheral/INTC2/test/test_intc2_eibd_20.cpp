/*
 * @ test_eibd.cpp
 *
 * @date Created on: Dec 10, 2016
 * @author Author: Can Vy <can.vy.wh@rvc.renesas.com>
 * @brief UNIT TEST for EIBD.
 * @par Copyright: 2016 Renesas Electronics Corp.
 */

#include <memory>
#include <map>
#include <stdexcept>
#include "mock_intc1.h"
#include "intc2.h"
#include "forest_common.h"
#include "mock_proc_element.h"
#include "mock_sideband.h"
#include "mock_fsbus.h"
#include "gmock/gmock.h"

using ::testing::AtLeast;
using ::testing::_;
using ::testing::Return;

class EIBDTest : public testing::Test {
protected:
  EIBDTest() {}
  virtual ~EIBDTest() {}
  virtual void SetUp() {
    eibd = new EIBD();
  }
  virtual void TearDown() {
    delete eibd;
  }

public:
  uint32_t pe_id = 0;
  uint32_t channel = 32;
  EIBD* eibd;

private:
};


/*!
 * @brief Check EIBD SetData operation.
 */
TEST_F(EIBDTest, Check_GetSetData) {
    SCOPED_TRACE("Check_GetSetData");
    uint32_t data = 0xCAFECAFE;
    eibd->SetData(data);
    EXPECT_EQ(data, eibd->GetData());
}


/*!
 * @brief Check EIBD Get/SetCST operation.
 */
TEST_F(EIBDTest, Check_GetSetCST) {
    SCOPED_TRACE("Check_GetSetCST");
    eibd->SetCST(0);
    EXPECT_EQ(0x0U, eibd->GetCST());
    eibd->SetCST(1);
    EXPECT_EQ(0x1U, eibd->GetCST());
}


/*!
 * @brief Check EIBD Get/SetBCP operation.
 */
TEST_F(EIBDTest, Check_GetSetBCP) {
    SCOPED_TRACE("Check_GetSetBCP");
    eibd->SetBCP(0);
    EXPECT_EQ(0x0U, eibd->GetBCP());
    eibd->SetBCP(1);
    EXPECT_EQ(0x1U, eibd->GetBCP());
    eibd->SetBCP(2);
    EXPECT_EQ(0x2U, eibd->GetBCP());
    eibd->SetBCP(3);
    EXPECT_EQ(0x3U, eibd->GetBCP());
}


/*!
 * @brief Check EIBD Get/SetPEID operation.
 */
TEST_F(EIBDTest, Check_GetSetPEID) {
    SCOPED_TRACE("Check_GetSetPEID");
    eibd->SetPEID(0);
    EXPECT_EQ(0x0U, eibd->GetPEID());
    eibd->SetPEID(15);
    EXPECT_EQ(0x7U, eibd->GetPEID());
}

/*!
 * @brief Check EIBD Get/SetGM operation.
 */
TEST_F(EIBDTest, Check_GetSetGM) {
    SCOPED_TRACE("Check_GetSetGM");
    eibd->SetGM(0);
    EXPECT_EQ(0x0U, eibd->GetGM());
    eibd->SetGM(15);
    EXPECT_EQ(0x1U, eibd->GetGM());
}


/*!
 * @brief Check EIBD Get/SetGPID operation.
 */
TEST_F(EIBDTest, Check_GetSetGPID) {
    SCOPED_TRACE("Check_GetSetGPID");
    eibd->SetGPID(0);
    EXPECT_EQ(0x0U, eibd->GetGPID());
    eibd->SetGPID(15);
    EXPECT_EQ(0x7U, eibd->GetGPID());
}


/*!
 * @brief Check EIBD GetName operation.
 */
TEST_F(EIBDTest, Check_GetName) {
    SCOPED_TRACE("Check_GetName");
    const char* name = eibd->GetName();
    ASSERT_STREQ("EIBD", name);
}


/*!
 * @brief Check EIBD GetTRegDataPtr operation.
 */
TEST_F(EIBDTest, Check_GetTRegDataPtr) {
    SCOPED_TRACE("Check_GetTRegDataPtr");
    ASSERT_NE(nullptr, eibd->GetTRegDataPtr());
}


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
