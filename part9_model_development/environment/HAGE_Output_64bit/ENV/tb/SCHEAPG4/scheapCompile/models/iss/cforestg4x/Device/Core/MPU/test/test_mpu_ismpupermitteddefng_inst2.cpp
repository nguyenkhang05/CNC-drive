/**
 * @file test_mpu_ismpupermitteddefng_inst2.cpp
 *
 * @date Created on: 2017
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @brief UNIT TEST for MPU.
 * @par Copyright: 2017 Renesas Electronics Corp.
 */
// #include <memory>
// #include <stdexcept>
#include "config.h"
#include "test_mpu_ismpupermitteddefng_inst.h"

// using ::testing::_;
using ::testing::Return;
// using ::testing::ReturnRef;
// using ::testing::ReturnPointee;
using ::testing::InSequence;

/*!
 * @brief Check IsMpuPermittedDefNG method. (INST_READ)
 *        condition: no guest entry & GMPM.MPE = 0
 */
TEST_P(MpuMethodCheck, CheckIsMpuPermittedDefNG_INST_READ_3) {
  bool gm = std::get<0>(GetParam());
  bool mpe = std::get<1>(GetParam());
  bool gmpe = std::get<2>(GetParam());
  bool um = std::get<3>(GetParam());
  bool svp = std::get<4>(GetParam());
  bool gent = std::get<5>(GetParam());
  bool hent = std::get<6>(GetParam());
  bool expect = std::get<7>(GetParam());
  Mpu::faultPos expect_faultpos = std::get<8>(GetParam());

  // TBD: It may be good to try out addresses and sizes
  PhysAddr addr = 0;
  ByteSize size = SIZE_32BIT;

  // Prologue
  {
    // IsMpuRegionHit
    {
      EXPECT_CALL(*(m_mpat_vec[1]), GetWG()).WillOnce(Return(true));
      EXPECT_CALL(*(m_mpat_vec[1]), GetRG()).WillOnce(Return(true));
      m_mpu->AddMpuRegion(1);
    }
    EXPECT_CALL(*m_srf, SrVSPID()).WillOnce(Return(static_cast<SRegGMSPID*>(m_vspid)));
    EXPECT_CALL(*m_vspid, ReadBody()).WillOnce(Return(0)); // SPID is fixed at 0.
  }

  // Test Condition
  {
    InSequence s;

    EXPECT_CALL(*m_srf, SrHVCFG()).WillRepeatedly(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
    EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(1));
    EXPECT_CALL(*m_srf, SrVPSW()).WillOnce(Return(static_cast<SRegBasePSW*>(m_vpsw)));
    EXPECT_CALL(*m_vpsw, GetUM()).WillOnce(Return(um)); // Use UM
    EXPECT_CALL(*m_srf, SrPSWH()).WillOnce(Return(static_cast<SRegPSWH*>(m_pswh)));
    EXPECT_CALL(*m_pswh, GetGM()).WillOnce(Return(gm)); // Use GM
    EXPECT_CALL(*m_srf, SrVMPM()).WillOnce(Return(static_cast<SRegGMMPM*>(m_vmpm)));
    EXPECT_CALL(*m_vmpm, GetSVP()).WillOnce(Return(svp)); // Use SVP

    EXPECT_CALL(*m_srf, SrVMPM()).WillRepeatedly(Return(static_cast<SRegGMMPM*>(m_vmpm)));
    EXPECT_CALL(*m_vmpm, GetMPE()).WillOnce(Return(mpe));
    EXPECT_CALL(*m_srf, SrVMPM()).WillRepeatedly(Return(static_cast<SRegGMMPM*>(m_vmpm)));
    EXPECT_CALL(*m_vmpm, GetGMPE()).WillOnce(Return(gmpe));

    EXPECT_CALL(*m_srf, SrMPCFG()).WillRepeatedly(Return(static_cast<SRegMPCFG*>(m_vmpcfg)));
    EXPECT_CALL(*m_vmpcfg, GetHBE()).WillOnce(Return(1)); // Host entries are from 1

    if (gm) {
      if (mpe) {
        if (!(!um && !svp)) {
          if (gent) {
            SET_HIT_PERMIT(GUEST_ENTRY); // Guest: hit & permit
          } else {
            SET_NOT_HIT(GUEST_ENTRY);    // Guest: not hit
          }
        }
      }
      if (gmpe) {
        if (hent) {
          SET_HIT_PERMIT(HOST_ENTRY);    // Host: hit & permit
        } else {
          SET_NOT_HIT(HOST_ENTRY);       // Host: not hit
        }
      }
    } else {
      if (mpe) {
        if (!(!um && !svp)) {
          if (hent) {
            SET_HIT_PERMIT(HOST_ENTRY);    // Host: hit & permit
          } else {
            SET_NOT_HIT(HOST_ENTRY);       // Host: not hit
          }
        }
      }
    }
  }

  // check
  bool res = m_mpu->IsMpuPermittedDefNG<INST_READ>(addr, size);
  EXPECT_EQ(expect, res);
  ASSERT_EQ(expect_faultpos, m_mpu->GetMpuExpPOS());
};

/*!
 * @brief Check IsMpuPermittedDefNG method. (INST_READ)
 *        condition: no guest entry & GMPM.MPE = 0
 */
TEST_P(MpuMethodCheck, CheckIsMpuPermittedDefNG_INST_READ_4) {
  bool gm = std::get<0>(GetParam());
  bool mpe = std::get<1>(GetParam());
  bool gmpe = std::get<2>(GetParam());
  bool um = std::get<3>(GetParam());
  bool svp = std::get<4>(GetParam());
  bool gent = std::get<5>(GetParam());
  bool hent = std::get<6>(GetParam());
  bool expect = std::get<7>(GetParam());
  Mpu::faultPos expect_faultpos = std::get<8>(GetParam());

  // TBD: It may be good to try out addresses and sizes
  PhysAddr addr = 0;
  ByteSize size = SIZE_32BIT;

  // Prologue
  {
    // IsMpuRegionHit
    {
      EXPECT_CALL(*(m_mpat_vec[1]), GetWG()).WillOnce(Return(true));
      EXPECT_CALL(*(m_mpat_vec[1]), GetRG()).WillOnce(Return(true));
      m_mpu->AddMpuRegion(1);
    }
    EXPECT_CALL(*m_srf, SrVSPID()).WillOnce(Return(static_cast<SRegGMSPID*>(m_vspid)));
    EXPECT_CALL(*m_vspid, ReadBody()).WillOnce(Return(0)); // SPID is fixed at 0.
  }

  // Test Condition
  {
    InSequence s;

    EXPECT_CALL(*m_srf, SrHVCFG()).WillRepeatedly(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
    EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(1));
    EXPECT_CALL(*m_srf, SrVPSW()).WillOnce(Return(static_cast<SRegBasePSW*>(m_vpsw)));
    EXPECT_CALL(*m_vpsw, GetUM()).WillOnce(Return(um)); // Use UM
    EXPECT_CALL(*m_srf, SrPSWH()).WillOnce(Return(static_cast<SRegPSWH*>(m_pswh)));
    EXPECT_CALL(*m_pswh, GetGM()).WillOnce(Return(gm)); // Use GM
    EXPECT_CALL(*m_srf, SrVMPM()).WillOnce(Return(static_cast<SRegGMMPM*>(m_vmpm)));
    EXPECT_CALL(*m_vmpm, GetSVP()).WillOnce(Return(svp)); // Use SVP

    EXPECT_CALL(*m_srf, SrVMPM()).WillRepeatedly(Return(static_cast<SRegGMMPM*>(m_vmpm)));
    EXPECT_CALL(*m_vmpm, GetMPE()).WillOnce(Return(mpe));
    EXPECT_CALL(*m_srf, SrVMPM()).WillRepeatedly(Return(static_cast<SRegGMMPM*>(m_vmpm)));
    EXPECT_CALL(*m_vmpm, GetGMPE()).WillOnce(Return(gmpe));

    EXPECT_CALL(*m_srf, SrMPCFG()).WillRepeatedly(Return(static_cast<SRegMPCFG*>(m_vmpcfg)));
    EXPECT_CALL(*m_vmpcfg, GetHBE()).WillOnce(Return(1)); // Host entries are from 1

    if (gm) {
      if (mpe) {
        if (!(!um && !svp)) {
          if (gent) {
            SET_HIT_PERMIT(GUEST_ENTRY);     // Guest: hit & permit
          } else {
            SET_HIT_NOT_PERMIT(GUEST_ENTRY); // Guest: hit & not permit
          }
        }
      }
      if (gmpe) {
        if (hent) {
          SET_HIT_PERMIT(HOST_ENTRY);        // Host: hit & permit
        } else {
          SET_HIT_NOT_PERMIT(HOST_ENTRY);    // Host: hit & not permit
        }
      }
    } else {
      if (mpe) {
        if (!(!um && !svp)) {
          if (hent) {
            SET_HIT_PERMIT(HOST_ENTRY);      // Host: hit & permit
          } else {
            SET_HIT_NOT_PERMIT(HOST_ENTRY);  // Host: hit & not permit
          }
        }
      }
    }
  }

  // check
  bool res = m_mpu->IsMpuPermittedDefNG<INST_READ>(addr, size);
  EXPECT_EQ(expect, res);
  ASSERT_EQ(expect_faultpos, m_mpu->GetMpuExpPOS());
};

std::tuple<bool, bool, bool, bool, bool, bool, bool, bool, Mpu::faultPos> const ParametersTable[] = {
  // GM:   PSWH.GM. true: Guest mode. false: Host mode.
  // MPE:  VMPM.MPE.
  // GMPE: VMPM.GMPE.
  // UM:   VPSW.UM.  true: User mode. false: SV mode.
  // SVP:  VMPM.SVP.
  // GENT: Guest entry return: true: guest entry is matched.
  // HENT: Host entry return:  true: host entry is matched.
  //                        GM,   MPE,  GMPE, UM,   SVP,  GENT, HENT, expct, expect_faultpos
  /*  0 */ std::make_tuple(true, false,true, true, true, true, true, true, Mpu::M_NONE),
  /*  1 */ std::make_tuple(true, false,true, true, true, true, false,false,Mpu::M_HOSTENT),
  /*  2 */ std::make_tuple(true, false,true, true, false,true, true, true, Mpu::M_NONE),
  /*  3 */ std::make_tuple(true, false,true, true, false,true, false,false,Mpu::M_HOSTENT),
  /*  4 */ std::make_tuple(true, false,true, false,true, true, true, true, Mpu::M_NONE),
  /*  5 */ std::make_tuple(true, false,true, false,true, true, false,false,Mpu::M_HOSTENT),
  /*  6 */ std::make_tuple(true, false,true, false,false,true, true, true, Mpu::M_NONE),
  /*  7 */ std::make_tuple(true, false,true, false,false,true, false,false,Mpu::M_HOSTENT),
  /*  8 */ std::make_tuple(true, false,false,true, true, true, true, true, Mpu::M_NONE),
  /*  9 */ std::make_tuple(true, false,false,true, true, true, false,true, Mpu::M_NONE),
  /* 10 */ std::make_tuple(true, false,false,true, false,true, true, true, Mpu::M_NONE),
  /* 11 */ std::make_tuple(true, false,false,true, false,true, false,true, Mpu::M_NONE),
  /* 12 */ std::make_tuple(true, false,false,false,true, true, true, true, Mpu::M_NONE),
  /* 13 */ std::make_tuple(true, false,false,false,true, true, false,true, Mpu::M_NONE),
  /* 14 */ std::make_tuple(true, false,false,false,false,true, true, true, Mpu::M_NONE),
  /* 15 */ std::make_tuple(true, false,false,false,false,true, false,true, Mpu::M_NONE),
  // Allow all host mode (GM==false)
};
INSTANTIATE_TEST_CASE_P(TestWithParameters, MpuMethodCheck, ::testing::ValuesIn(ParametersTable));

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
