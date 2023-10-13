/*
 * test_intpriorityinfo.cpp
 *
 * @date Created on: Dec 10, 2016
 * @author Author: Can Vy <can.vy.wh@rvc.renesas.com>
 * @brief UNIT TEST for IntPriorityInfo.
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

class IntPriorityInfoTest : public testing::Test {
protected:
  IntPriorityInfoTest() {}
  virtual ~IntPriorityInfoTest() {}
  virtual void SetUp() {
    m_test_priority = new IntPriorityInfo();
  }
  virtual void TearDown() {
    delete m_test_priority;
  }

public:
  IntPriorityInfo* m_test_priority;
  uint32_t NO_REQ_CHANNEL = 0xFFFFFFFFUL;
  int32_t  LOW_PRIORITY   = 0x7FFFFFFFUL;
  int32_t  FEINT_PRIORITY = -16;
  int32_t  FENMI_PRIORITY = -17;

private:
};


/*!
 * @brief Check IntPriorityInfo Invalidate operation.
 */
TEST_F(IntPriorityInfoTest, Check_Invalidate) {
    SCOPED_TRACE("Check_Invalidate");
    ASSERT_NO_THROW (m_test_priority->Invalidate());
    EXPECT_EQ(NO_REQ_CHANNEL, m_test_priority->GetChannelEiint());
    EXPECT_EQ(LOW_PRIORITY, m_test_priority->GetPriorityEiint());
}


/*!
 * @brief Check IntPriorityInfo UpdateChannel operation.
 */
TEST_F(IntPriorityInfoTest, Check_UpdateChannel) {
    uint32_t update_channel = 32;
    int32_t update_priority = 12;
    int32_t update_priority_by_bind = update_priority;
    SCOPED_TRACE("Check_UpdateChannel");
    m_test_priority->m_priority = 15;
    ASSERT_NO_THROW (m_test_priority->UpdateChannelEiint(update_channel, update_priority, update_priority_by_bind));
    EXPECT_EQ(update_channel, m_test_priority->GetChannelEiint());
    EXPECT_EQ(update_priority, m_test_priority->GetPriorityEiint());
}


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
