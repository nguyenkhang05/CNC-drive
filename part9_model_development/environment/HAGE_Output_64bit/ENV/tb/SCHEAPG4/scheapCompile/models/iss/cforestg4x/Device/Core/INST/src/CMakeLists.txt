cmake_minimum_required( VERSION 3.2 )

project( DEVICE_CORE_INST )

enable_language(CXX)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Core/BTB/include
  ${TOP_SOURCE_DIR}/Device/Core/COREGUARD/include
  ${TOP_SOURCE_DIR}/Device/Core/COUNTERBASE/include
  ${TOP_SOURCE_DIR}/Device/Core/CYCLESIM/include
  ${TOP_SOURCE_DIR}/Device/Core/DEBUGBREAK/include
  ${TOP_SOURCE_DIR}/Device/Core/EXCEP/include
  ${TOP_SOURCE_DIR}/Device/Core/ICACHE/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/include
  ${TOP_SOURCE_DIR}/Device/Core/LLSC/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
  ${TOP_BINARY_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/MPU/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/SIMBREAK/include
  ${TOP_SOURCE_DIR}/Device/Core/STATISTICS/include
  ${TOP_BINARY_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC2/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
)
if( SUPPORT_SC_HEAP )
  include_directories( ${TOP_SOURCE_DIR}/Device/API/SCHEAP/include )
endif( )

if( SUPPORT_CUBESUITE )
  include_directories( ${TOP_SOURCE_DIR}/Device/API/CSPLUS/include )
endif( )

if( MSVC )
else( )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-type-limits" )
endif( )

add_library(
  device_core_inst OBJECT
  fpu_base.cpp
  fpu_device.cpp
  fs_controler.cpp
  fs_exception.cpp
  inst_queue.cpp
  inst_v850.cpp
  inst_v850fpsimd.cpp
  inst_v850fpu.cpp

  SoftFloatAdapter.cpp
)

add_dependencies( device_core_inst
  gen_cedar_arch_info
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
  gen_trace_operand
)

add_cppcheck(device_core_inst UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE FAIL_ON_WARNINGS)

project( DEVICE_CORE_INST_SOFTFLOAT )

enable_language(C)

add_definitions( -DSOFTFLOAT_FAST_INT64 )

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/SoftFloat/source/CForestG4X
  ${CMAKE_CURRENT_SOURCE_DIR}/SoftFloat/source/include
  ${CMAKE_CURRENT_SOURCE_DIR}/SoftFloat/source
)

if( ${PRODUCT_TYPE}  MATCHES "^CFORESTG4X$" )
  set( LIBTYPE )
else( )
  set( LIBTYPE OBJECT )
endif( )

if( MSVC )
else( )
  set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-implicit-function-declaration -Wno-sign-compare -Wno-maybe-uninitialized" )
endif( )

add_library(
  softfloat ${LIBTYPE}
  # CForestG4X
  SoftFloat/source/CForestG4X/softfloat_raiseFlags.c
  SoftFloat/source/CForestG4X/s_roundPackToF16.c
  SoftFloat/source/CForestG4X/s_roundPackToF32.c
  SoftFloat/source/CForestG4X/s_roundPackToF64.c
  SoftFloat/source/CForestG4X/s_propagateNaNF128UI.c
  SoftFloat/source/CForestG4X/s_propagateNaNF64UI.c
  SoftFloat/source/CForestG4X/s_propagateNaNF32UI.c
  SoftFloat/source/CForestG4X/s_f128UIToCommonNaN.c
  SoftFloat/source/CForestG4X/s_f64UIToCommonNaN.c
  SoftFloat/source/CForestG4X/s_f32UIToCommonNaN.c
  SoftFloat/source/CForestG4X/s_f16UIToCommonNaN.c
  SoftFloat/source/CForestG4X/s_commonNaNToF128UI.c
  SoftFloat/source/CForestG4X/s_commonNaNToF64UI.c
  SoftFloat/source/CForestG4X/s_commonNaNToF32UI.c
  SoftFloat/source/CForestG4X/s_commonNaNToF16UI.c
  SoftFloat/source/CForestG4X/softfloat_state.c
  SoftFloat/source/s_approxRecip_1Ks.c
  SoftFloat/source/s_approxRecipSqrt_1Ks.c
  #
  ## SoftFloat/source/extF80M_add.c
  ## SoftFloat/source/extF80M_div.c
  SoftFloat/source/extF80M_eq.c
  SoftFloat/source/extF80M_eq_signaling.c
  SoftFloat/source/extF80M_le.c
  SoftFloat/source/extF80M_le_quiet.c
  SoftFloat/source/extF80M_lt.c
  SoftFloat/source/extF80M_lt_quiet.c
  ## SoftFloat/source/extF80M_mul.c
  ## SoftFloat/source/extF80M_rem.c
  ## SoftFloat/source/extF80M_roundToInt.c
  ## SoftFloat/source/extF80M_sqrt.c
  ## SoftFloat/source/extF80M_sub.c
  ## SoftFloat/source/extF80M_to_f128M.c
  ## SoftFloat/source/extF80M_to_f32.c
  ## SoftFloat/source/extF80M_to_f64.c
  SoftFloat/source/extF80M_to_i32.c
  SoftFloat/source/extF80M_to_i32_r_minMag.c
  SoftFloat/source/extF80M_to_i64.c
  SoftFloat/source/extF80M_to_i64_r_minMag.c
  SoftFloat/source/extF80M_to_ui32.c
  SoftFloat/source/extF80M_to_ui32_r_minMag.c
  SoftFloat/source/extF80M_to_ui64.c
  SoftFloat/source/extF80M_to_ui64_r_minMag.c
  ## SoftFloat/source/extF80_add.c
  ## SoftFloat/source/extF80_div.c
  SoftFloat/source/extF80_eq.c
  SoftFloat/source/extF80_eq_signaling.c
  SoftFloat/source/extF80_isSignalingNaN.c
  SoftFloat/source/extF80_le.c
  SoftFloat/source/extF80_le_quiet.c
  SoftFloat/source/extF80_lt.c
  SoftFloat/source/extF80_lt_quiet.c
  ## SoftFloat/source/extF80_mul.c
  ## SoftFloat/source/extF80_rem.c
  ## SoftFloat/source/extF80_roundToInt.c
  ## SoftFloat/source/extF80_sqrt.c
  ## SoftFloat/source/extF80_sub.c
  ## SoftFloat/source/extF80_to_f128.c
  ## SoftFloat/source/extF80_to_f32.c
  ## SoftFloat/source/extF80_to_f64.c
  SoftFloat/source/extF80_to_i32.c
  SoftFloat/source/extF80_to_i32_r_minMag.c
  SoftFloat/source/extF80_to_i64.c
  SoftFloat/source/extF80_to_i64_r_minMag.c
  SoftFloat/source/extF80_to_ui32.c
  SoftFloat/source/extF80_to_ui32_r_minMag.c
  SoftFloat/source/extF80_to_ui64.c
  SoftFloat/source/extF80_to_ui64_r_minMag.c
  ## SoftFloat/source/f128M_add.c
  ## SoftFloat/source/f128M_div.c
  SoftFloat/source/f128M_eq.c
  SoftFloat/source/f128M_eq_signaling.c
  SoftFloat/source/f128M_le.c
  SoftFloat/source/f128M_le_quiet.c
  SoftFloat/source/f128M_lt.c
  SoftFloat/source/f128M_lt_quiet.c
  ## SoftFloat/source/f128M_mul.c
  ## SoftFloat/source/f128M_mulAdd.c
  ## SoftFloat/source/f128M_rem.c
  ## SoftFloat/source/f128M_roundToInt.c
  ## SoftFloat/source/f128M_sqrt.c
  ## SoftFloat/source/f128M_sub.c
  ## SoftFloat/source/f128M_to_extF80M.c
  ## SoftFloat/source/f128M_to_f32.c
  ## SoftFloat/source/f128M_to_f64.c
  SoftFloat/source/f128M_to_i32.c
  SoftFloat/source/f128M_to_i32_r_minMag.c
  SoftFloat/source/f128M_to_i64.c
  SoftFloat/source/f128M_to_i64_r_minMag.c
  SoftFloat/source/f128M_to_ui32.c
  SoftFloat/source/f128M_to_ui32_r_minMag.c
  SoftFloat/source/f128M_to_ui64.c
  SoftFloat/source/f128M_to_ui64_r_minMag.c
  ## SoftFloat/source/f128_add.c
  SoftFloat/source/f128_div.c
  SoftFloat/source/f128_eq.c
  SoftFloat/source/f128_eq_signaling.c
  SoftFloat/source/f128_isSignalingNaN.c
  SoftFloat/source/f128_le.c
  SoftFloat/source/f128_le_quiet.c
  SoftFloat/source/f128_lt.c
  SoftFloat/source/f128_lt_quiet.c
  ## SoftFloat/source/f128_mul.c
  ## SoftFloat/source/f128_mulAdd.c
  ## SoftFloat/source/f128_rem.c
  ## SoftFloat/source/f128_roundToInt.c
  SoftFloat/source/f128_sqrt.c
  ## SoftFloat/source/f128_sub.c
  ## SoftFloat/source/f128_to_extF80.c
  ## SoftFloat/source/f128_to_f32.c
  SoftFloat/source/f128_to_f64.c
  SoftFloat/source/f128_to_i32.c
  SoftFloat/source/f128_to_i32_r_minMag.c
  SoftFloat/source/f128_to_i64.c
  SoftFloat/source/f128_to_i64_r_minMag.c
  SoftFloat/source/f128_to_ui32.c
  SoftFloat/source/f128_to_ui32_r_minMag.c
  SoftFloat/source/f128_to_ui64.c
  SoftFloat/source/f128_to_ui64_r_minMag.c
  SoftFloat/source/f16_to_f32.c
  SoftFloat/source/f16_isSignalingNaN.c
  SoftFloat/source/s_normSubnormalF16Sig.c
  SoftFloat/source/s_countLeadingZeros16.c
  SoftFloat/source/f32_add.c
  SoftFloat/source/f32_div.c
  SoftFloat/source/f32_eq.c
  SoftFloat/source/f32_eq_signaling.c
  SoftFloat/source/f32_isSignalingNaN.c
  SoftFloat/source/f32_le.c
  SoftFloat/source/f32_le_quiet.c
  SoftFloat/source/f32_lt.c
  SoftFloat/source/f32_lt_quiet.c
  SoftFloat/source/f32_mul.c
  SoftFloat/source/f32_mulAdd.c
  SoftFloat/source/f32_rem.c
  SoftFloat/source/f32_roundToInt.c
  SoftFloat/source/f32_sqrt.c
  SoftFloat/source/f32_sub.c
  ## SoftFloat/source/f32_to_extF80.c
  ## SoftFloat/source/f32_to_extF80M.c
  ## SoftFloat/source/f32_to_f128.c
  ## SoftFloat/source/f32_to_f128M.c
  SoftFloat/source/f32_to_f64.c
  SoftFloat/source/f32_to_f16.c
  #SoftFloat/source/s_roundPackToF16.c
  SoftFloat/source/f32_to_i32.c
  SoftFloat/source/f32_to_i32_r_minMag.c
  SoftFloat/source/f32_to_i64.c
  SoftFloat/source/f32_to_i64_r_minMag.c
  SoftFloat/source/f32_to_ui32.c
  SoftFloat/source/f32_to_ui32_r_minMag.c
  SoftFloat/source/f32_to_ui64.c
  SoftFloat/source/f32_to_ui64_r_minMag.c
  SoftFloat/source/f64_add.c
  SoftFloat/source/f64_div.c
  SoftFloat/source/f64_eq.c
  SoftFloat/source/f64_eq_signaling.c
  SoftFloat/source/f64_isSignalingNaN.c
  SoftFloat/source/f64_le.c
  SoftFloat/source/f64_le_quiet.c
  SoftFloat/source/f64_lt.c
  SoftFloat/source/f64_lt_quiet.c
  SoftFloat/source/f64_mul.c
  ## SoftFloat/source/f64_mulAdd.c
  ## SoftFloat/source/f64_rem.c
  ## SoftFloat/source/f64_roundToInt.c
  SoftFloat/source/f64_sqrt.c
  SoftFloat/source/f64_sub.c
  ## SoftFloat/source/f64_to_extF80.c
  ## SoftFloat/source/f64_to_extF80M.c
  SoftFloat/source/f64_to_f128.c
  ## SoftFloat/source/f64_to_f128M.c
  SoftFloat/source/f64_to_f32.c
  SoftFloat/source/f64_to_i32.c
  SoftFloat/source/f64_to_i32_r_minMag.c
  SoftFloat/source/f64_to_i64.c
  SoftFloat/source/f64_to_i64_r_minMag.c
  SoftFloat/source/f64_to_ui32.c
  SoftFloat/source/f64_to_ui32_r_minMag.c
  SoftFloat/source/f64_to_ui64.c
  SoftFloat/source/f64_to_ui64_r_minMag.c
  SoftFloat/source/i32_to_extF80.c
  SoftFloat/source/i32_to_extF80M.c
  SoftFloat/source/i32_to_f128.c
  SoftFloat/source/i32_to_f128M.c
  SoftFloat/source/i32_to_f32.c
  SoftFloat/source/i32_to_f64.c
  SoftFloat/source/i64_to_extF80.c
  SoftFloat/source/i64_to_extF80M.c
  SoftFloat/source/i64_to_f128.c
  SoftFloat/source/i64_to_f128M.c
  SoftFloat/source/i64_to_f32.c
  SoftFloat/source/i64_to_f64.c
  SoftFloat/source/s_add128.c
  SoftFloat/source/s_add256M.c
  SoftFloat/source/s_addCarryM.c
  SoftFloat/source/s_addComplCarryM.c
  # SoftFloat/source/s_addExtF80M.c
  # SoftFloat/source/s_addF128M.c
  SoftFloat/source/s_addM.c
  ## SoftFloat/source/s_addMagsExtF80.c
  ## SoftFloat/source/s_addMagsF128.c
  SoftFloat/source/s_addMagsF32.c
  SoftFloat/source/s_addMagsF64.c
  SoftFloat/source/s_approxRecip32_1.c
  SoftFloat/source/s_approxRecipSqrt32_1.c
  SoftFloat/source/s_approxRecipSqrt_1Ks.c
  SoftFloat/source/s_compare128M.c
  SoftFloat/source/s_compare96M.c
  SoftFloat/source/s_compareNonnormExtF80M.c
  SoftFloat/source/s_countLeadingZeros32.c
  SoftFloat/source/s_countLeadingZeros64.c
  SoftFloat/source/s_countLeadingZeros8.c
  SoftFloat/source/s_eq128.c
  SoftFloat/source/s_invalidExtF80M.c
  # SoftFloat/source/s_invalidF128M.c
  SoftFloat/source/s_isNaNF128M.c
  SoftFloat/source/s_le128.c
  SoftFloat/source/s_lt128.c
  SoftFloat/source/s_mul128By32.c
  SoftFloat/source/s_mul128MTo256M.c
  SoftFloat/source/s_mul128To256M.c
  SoftFloat/source/s_mul64ByShifted32To128.c
  SoftFloat/source/s_mul64To128.c
  SoftFloat/source/s_mul64To128M.c
  ## SoftFloat/source/s_mulAddF128.c
  # SoftFloat/source/s_mulAddF128M.c
  SoftFloat/source/s_mulAddF32.c
  ## SoftFloat/source/s_mulAddF64.c
  SoftFloat/source/s_negXM.c
  SoftFloat/source/s_normExtF80SigM.c
  ## SoftFloat/source/s_normRoundPackMToExtF80M.c
  ## SoftFloat/source/s_normRoundPackMToF128M.c
  SoftFloat/source/s_normRoundPackToExtF80.c
  SoftFloat/source/s_normRoundPackToF128.c
  SoftFloat/source/s_normRoundPackToF32.c
  SoftFloat/source/s_normRoundPackToF64.c
  ## SoftFloat/source/s_normSubnormalExtF80Sig.c
  SoftFloat/source/s_normSubnormalF128Sig.c
  ## SoftFloat/source/s_normSubnormalF128SigM.c
  SoftFloat/source/s_normSubnormalF32Sig.c
  SoftFloat/source/s_normSubnormalF64Sig.c
  SoftFloat/source/s_remStepMBy32.c
  ## SoftFloat/source/s_roundPackMToExtF80M.c
  ## SoftFloat/source/s_roundPackMToF128M.c
  SoftFloat/source/s_roundMToI64.c
  SoftFloat/source/s_roundMToUI64.c
  SoftFloat/source/s_roundPackToExtF80.c
  SoftFloat/source/s_roundPackToF128.c
  #SoftFloat/source/s_roundPackToF32.c
  #SoftFloat/source/s_roundPackToF64.c
  SoftFloat/source/s_roundToI32.c
  SoftFloat/source/s_roundToI64.c
  SoftFloat/source/s_roundToUI32.c
  SoftFloat/source/s_roundToUI64.c
  SoftFloat/source/s_shiftLeftM.c
  ## SoftFloat/source/s_shiftNormSigF128M.c
  SoftFloat/source/s_shiftRightJam128.c
  SoftFloat/source/s_shiftRightJam128Extra.c
  SoftFloat/source/s_shiftRightJam256M.c
  SoftFloat/source/s_shiftRightJam32.c
  SoftFloat/source/s_shiftRightJam64.c
  SoftFloat/source/s_shiftRightJam64Extra.c
  SoftFloat/source/s_shiftRightJamM.c
  SoftFloat/source/s_shiftRightM.c
  SoftFloat/source/s_shortShiftLeft128.c
  SoftFloat/source/s_shortShiftLeft64To96M.c
  SoftFloat/source/s_shortShiftLeftM.c
  SoftFloat/source/s_shortShiftRight128.c
  SoftFloat/source/s_shortShiftRightExtendM.c
  SoftFloat/source/s_shortShiftRightJam128.c
  SoftFloat/source/s_shortShiftRightJam128Extra.c
  SoftFloat/source/s_shortShiftRightJam64.c
  SoftFloat/source/s_shortShiftRightJam64Extra.c
  SoftFloat/source/s_shortShiftRightJamM.c
  SoftFloat/source/s_shortShiftRightM.c
  SoftFloat/source/s_sub128.c
  SoftFloat/source/s_sub1XM.c
  SoftFloat/source/s_sub256M.c
  SoftFloat/source/s_subM.c
  ## SoftFloat/source/s_subMagsExtF80.c
  ## SoftFloat/source/s_subMagsF128.c
  SoftFloat/source/s_subMagsF32.c
  SoftFloat/source/s_subMagsF64.c
  ## SoftFloat/source/s_tryPropagateNaNExtF80M.c
  ## SoftFloat/source/s_tryPropagateNaNF128M.c
  ##SoftFloat/source/softfloat_state.c
  SoftFloat/source/ui32_to_extF80.c
  SoftFloat/source/ui32_to_extF80M.c
  SoftFloat/source/ui32_to_f128.c
  SoftFloat/source/ui32_to_f128M.c
  SoftFloat/source/ui32_to_f32.c
  SoftFloat/source/ui32_to_f64.c
  SoftFloat/source/ui64_to_extF80.c
  SoftFloat/source/ui64_to_extF80M.c
  SoftFloat/source/ui64_to_f128.c
  SoftFloat/source/ui64_to_f128M.c
  SoftFloat/source/ui64_to_f32.c
  SoftFloat/source/ui64_to_f64.c
)
