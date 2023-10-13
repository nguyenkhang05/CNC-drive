/**
 * @file test_SoftFloatAdapter.cpp
 *
 * @date Created on: 2016
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @brief UNIT TEST for SoftFLoat class
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#include <cstdint>
#include <cstddef>
#include "gmock/gmock.h"
#include "SoftFloatAdapter.h"

// using ::testing::AtLeast;
// using ::testing::_;

class SoftFLoatTest
    : public testing::Test {
protected:
  SoftFLoatTest() {}
  virtual ~SoftFLoatTest() {}
  virtual void SetUp() { m_sf = new SoftFloat(); }
  virtual void TearDown() { delete m_sf; }

public:
  SoftFloat* m_sf;

private:
};

/*!
 * @brief Check Get/Set input flush flag
 */
TEST_F(SoftFLoatTest, ChecGetSetIF) {
  m_sf->SetInputFlushFlag(false);
  EXPECT_FALSE(m_sf->GetInputFlushFlag());
  m_sf->SetInputFlushFlag(true);
  EXPECT_TRUE(m_sf->GetInputFlushFlag());
}

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
