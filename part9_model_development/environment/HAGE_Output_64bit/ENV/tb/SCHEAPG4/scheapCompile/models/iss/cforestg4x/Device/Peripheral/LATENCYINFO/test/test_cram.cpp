/**
 * @file test_cram.cpp
 *
 * @date Created on: 2019
  * @author Author: H.Nishi <hirofumi.nishi.ud@renesas.com>
 * @brief UNIT TEST for Outstand class with CRAM setting
 * @par Copyright: 2019 Renesas Electronics Corp.
   @note Main purpose of the UT is to check latency calculation directly.
 */

#include <functional>
#include <string.h>
#include <vector>
#include <memory>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "gmock/gmock.h"
#include "./config.h"
#include "./outstand_mem.h"

using ::testing::_;
using ::testing::Return;
using ::testing::AtLeast;

using ::testing::ExpectationSet;
using ::testing::Expectation;


class OutstandFunctionTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t >> {
protected:
    OutstandFunctionTest() {}
    virtual ~OutstandFunctionTest() {}
    virtual void SetUp() {
        uint32_t pe_fetch_latency       = 23;
        uint32_t pe_read_latency        = 5;
        uint32_t pe_write_latency       = 8;
        uint32_t cl_fetch_latency       = 23;
        uint32_t cl_read_latency        = 5;
        uint32_t cl_write_latency       = 8;
        uint32_t exo_cl_fetch_latency   = 23;
        uint32_t exo_cl_read_latency    = 8;
        uint32_t exo_cl_write_latency   = 9;
        MemLocationType pe_location     = MEM_LOCATE_GLOBAL;
        MemLocationType cl_location     = 0;
        uint32_t num_outstand           = 8;
        uint32_t repeat_rate            = 2;
        std::string tgt_name            = "CRAM@CL0";
        m_cram = new MemLatencyInfoGram (
                        pe_fetch_latency,
                        pe_read_latency,
                        pe_write_latency,
                        cl_fetch_latency,
                        cl_read_latency,
                        cl_write_latency,
                        exo_cl_fetch_latency,
                        exo_cl_read_latency,
                        exo_cl_write_latency,
                        pe_location,
                        cl_location,
                        num_outstand,
                        repeat_rate,
                        tgt_name
                        );
        m_cram->SetDbufPtr (nullptr); // set invalid pointer because CRAM doesn't use Dbuf
    }
    virtual void TearDown() {
        delete m_cram;
    }

public:
  MemLatencyInfoGram* m_cram;

private:
};


TEST_F(OutstandFunctionTest, TestLDW1)
{
    uint32_t peid       = 0;
    PhysAddr addr       = 0xFEE00000;
    uint64_t start_cyc  = 1000;
    ByteSize size       = 4;
    uint32_t latency;

    latency = m_cram->GetReadLatency (peid, addr, start_cyc, size);
    ASSERT_EQ(0x05U, latency);
}

TEST_F(OutstandFunctionTest, TestLDW2SAMEADDR)
{
    uint32_t peid       = 0;
    PhysAddr addr       = 0xFEE00000;
    uint64_t start_cyc  = 1000;
    ByteSize size       = 4;
    uint32_t latency;

    latency = m_cram->GetReadLatency (peid, addr, start_cyc, size);
    ASSERT_EQ(0x05U, latency);
    latency = m_cram->GetReadLatency (peid, addr, start_cyc, size);
    ASSERT_EQ(0x07U, latency);
}

TEST_F(OutstandFunctionTest, TestLDW4SAMEADDR)
{
    uint32_t peid       = 0;
    PhysAddr addr       = 0xFEE00000;
    uint64_t start_cyc  = 1000;
    ByteSize size       = 4;
    uint32_t latency;

    latency = m_cram->GetReadLatency (peid, addr, start_cyc, size);
    ASSERT_EQ(0x05U, latency);
    latency = m_cram->GetReadLatency (peid, addr, start_cyc, size);
    ASSERT_EQ(0x07U, latency);
    latency = m_cram->GetReadLatency (peid, addr, start_cyc, size);
    ASSERT_EQ(0x09U, latency);
    latency = m_cram->GetReadLatency (peid, addr, start_cyc, size);
    ASSERT_EQ(0x0BU, latency);
}

TEST_F(OutstandFunctionTest, TestLDW2DIFFADDR)
{
    uint32_t peid       = 0;
    PhysAddr addr       = 0xFEE00000;
    uint64_t start_cyc  = 1000;
    ByteSize size       = 4;
    uint32_t latency;

    latency = m_cram->GetReadLatency (peid, addr+0x0, start_cyc, size);
    ASSERT_EQ(0x05U, latency);
    latency = m_cram->GetReadLatency (peid, addr+0x4, start_cyc, size);
    ASSERT_EQ(0x05U, latency);
}

TEST_F(OutstandFunctionTest, TestLDW4DIFFADDR)
{
    uint32_t peid       = 0;
    PhysAddr addr       = 0xFEE00000;
    uint64_t start_cyc  = 1000;
    ByteSize size       = 4;
    uint32_t latency;

    // Because there are 4 banks, first 2 requests can be processed in fastest latency.
    // Because 2requests/cycle can be accepted by CRAM, following 2 requests is processed +1 later.

    latency = m_cram->GetReadLatency (peid, addr+0x0, start_cyc, size);
    ASSERT_EQ(0x05U, latency);
    latency = m_cram->GetReadLatency (peid, addr+0x4, start_cyc, size);
    ASSERT_EQ(0x05U, latency);
    latency = m_cram->GetReadLatency (peid, addr+0x8, start_cyc, size);
    ASSERT_EQ(0x06U, latency);
    latency = m_cram->GetReadLatency (peid, addr+0xC, start_cyc, size);
    ASSERT_EQ(0x06U, latency);
}

TEST_F(OutstandFunctionTest, TestLDW64DIFFADDR)
{
    uint32_t peid       = 0;
    PhysAddr addr       = 0xFEE00000;
    uint64_t start_cyc  = 1000;
    ByteSize size       = 4;
    uint32_t latency;

    // Because there are 4 banks, first 2 requests can be processed in fastest latency.
    // Because 2requests/cycle can be accepted by CRAM, following 2 requests is processed +1 later.

    for (uint32_t i = 0; i < 32; i++) {
        latency = m_cram->GetReadLatency (peid, addr+(0x8*i), start_cyc, size);
        ASSERT_EQ(0x05U + i, latency);
        latency = m_cram->GetReadLatency (peid, addr+(0x8*i) + 0x4, start_cyc, size);
        ASSERT_EQ(0x05U + i, latency);
    }
}

TEST_F(OutstandFunctionTest, TestSTW1)
{
    uint32_t peid       = 0;
    PhysAddr addr       = 0xFEE00000;
    uint64_t start_cyc  = 1000;
    ByteSize size       = 4;
    uint32_t latency;

    latency = m_cram->GetWriteLatency (peid, addr, start_cyc, size);
    ASSERT_EQ(0x08U, latency);
}

TEST_F(OutstandFunctionTest, TestSTW2SAMEADDR)
{
    uint32_t peid       = 0;
    PhysAddr addr       = 0xFEE00000;
    uint64_t start_cyc  = 1000;
    ByteSize size       = 4;
    uint32_t latency;

    latency = m_cram->GetWriteLatency (peid, addr, start_cyc, size);
    ASSERT_EQ(0x08U, latency);
    latency = m_cram->GetWriteLatency (peid, addr, start_cyc, size);
    ASSERT_EQ(0x0AU, latency);
}

TEST_F(OutstandFunctionTest, TestSTW4SAMEADDR)
{
    uint32_t peid       = 0;
    PhysAddr addr       = 0xFEE00000;
    uint64_t start_cyc  = 1000;
    ByteSize size       = 4;
    uint32_t latency;

    latency = m_cram->GetWriteLatency (peid, addr, start_cyc, size);
    ASSERT_EQ(0x08U, latency);
    latency = m_cram->GetWriteLatency (peid, addr, start_cyc, size);
    ASSERT_EQ(0x0AU, latency);
    latency = m_cram->GetWriteLatency (peid, addr, start_cyc, size);
    ASSERT_EQ(0x0CU, latency);
    latency = m_cram->GetWriteLatency (peid, addr, start_cyc, size);
    ASSERT_EQ(0x0EU, latency);
}

TEST_F(OutstandFunctionTest, TestSTW2DIFFADDR)
{
    uint32_t peid       = 0;
    PhysAddr addr       = 0xFEE00000;
    uint64_t start_cyc  = 1000;
    ByteSize size       = 4;
    uint32_t latency;

    latency = m_cram->GetWriteLatency (peid, addr+0x0, start_cyc, size);
    ASSERT_EQ(0x08U, latency);
    latency = m_cram->GetWriteLatency (peid, addr+0x4, start_cyc, size);
    ASSERT_EQ(0x08U, latency);
}

TEST_F(OutstandFunctionTest, TestSTW4DIFFADDR)
{
    uint32_t peid       = 0;
    PhysAddr addr       = 0xFEE00000;
    uint64_t start_cyc  = 1000;
    ByteSize size       = 4;
    uint32_t latency;

    // Because there are 4 banks, first 2 requests can be processed in fastest latency.
    // Because 2requests/cycle can be accepted by CRAM, following 2 requests is processed +1 later.

    latency = m_cram->GetWriteLatency (peid, addr+0x0, start_cyc, size);
    ASSERT_EQ(0x08U, latency);
    latency = m_cram->GetWriteLatency (peid, addr+0x4, start_cyc, size);
    ASSERT_EQ(0x08U, latency);
    latency = m_cram->GetWriteLatency (peid, addr+0x8, start_cyc, size);
    ASSERT_EQ(0x09U, latency);
    latency = m_cram->GetWriteLatency (peid, addr+0xC, start_cyc, size);
    ASSERT_EQ(0x09U, latency);
}

TEST_F(OutstandFunctionTest, TestSTW64DIFFADDR)
{
    uint32_t peid       = 0;
    PhysAddr addr       = 0xFEE00000;
    uint64_t start_cyc  = 1000;
    ByteSize size       = 4;
    uint32_t latency;

    // Because there are 4 banks, first 2 requests can be processed in fastest latency.
    // Because 2requests/cycle can be accepted by CRAM, following 2 requests is processed +1 later.

    for (uint32_t i = 0; i < 32; i++) {
        latency = m_cram->GetWriteLatency (peid, addr+(0x8*i), start_cyc, size);
        ASSERT_EQ(0x08U + i, latency);
        latency = m_cram->GetWriteLatency (peid, addr+(0x8*i) + 0x4, start_cyc, size);
        ASSERT_EQ(0x08U + i, latency);
    }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
