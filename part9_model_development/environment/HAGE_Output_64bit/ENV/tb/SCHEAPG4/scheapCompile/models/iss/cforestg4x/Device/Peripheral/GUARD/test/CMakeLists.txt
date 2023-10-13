cmake_minimum_required( VERSION 3.2 )

include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/test
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test
  ${TOP_SOURCE_DIR}/Device/Utility/include
  ${TOP_SOURCE_DIR}/Device/Utility/test
  ${gmock_SOURCE_DIR}/include
  ${gtest_SOURCE_DIR}/include
)

add_library(libguard
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/src/treg_guard.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/src/treg.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD/src/crg.cpp
)

add_dependencies( libguard
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
)

if( MSVC )
  set( hasPTHREAD 0 )
else( )
  set( hasPTHREAD 1 )
endif( )

# TEST TREG_crgctrlreg
add_executable( test_treg_crgctrlreg test_treg_crgctrlreg.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test/mock_guard_base.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD/test/test_treg_crgctrlreg.cpp
  )
target_link_libraries( test_treg_crgctrlreg
  libguard
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_treg_crgctrlreg COMMAND $<TARGET_FILE:test_treg_crgctrlreg> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_treg_crgctrlreg coverage)
endif( )

# TEST TREG_crgctrlreg G4MH2.0
add_executable( test_treg_crgctrlreg_20 test_treg_crgctrlreg_20.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test/mock_guard_base.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD/test/test_treg_crgctrlreg_20.cpp
  )
target_link_libraries( test_treg_crgctrlreg_20
  libguard
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_treg_crgctrlreg_20 COMMAND $<TARGET_FILE:test_treg_crgctrlreg_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_treg_crgctrlreg_20 coverage)
endif( )

# TEST TREG_crgerrreg
add_executable( test_treg_crgerrreg test_treg_crgerrreg.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test/mock_guard_base.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD/test/test_treg_crgerrreg.cpp
  )
target_link_libraries( test_treg_crgerrreg
  libguard
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_treg_crgerrreg COMMAND $<TARGET_FILE:test_treg_crgerrreg> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_treg_crgerrreg coverage)
endif( )

# TEST TREG_crgerrreg G4MH2.0
add_executable( test_treg_crgerrreg_20 test_treg_crgerrreg_20.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test/mock_guard_base.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD/test/test_treg_crgerrreg_20.cpp
  )
target_link_libraries( test_treg_crgerrreg_20
  libguard
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_treg_crgerrreg_20 COMMAND $<TARGET_FILE:test_treg_crgerrreg_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_treg_crgerrreg_20 coverage)
endif( )

# TEST CRGCTRL
add_executable( test_crgctrl test_crgctrl.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test/mock_guard_base.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD/test/test_crgctrl.cpp
  )
target_link_libraries( test_crgctrl
  libguard
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_crgctrl COMMAND $<TARGET_FILE:test_crgctrl> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_crgctrl coverage)
endif( )

# TEST CRGCTRL G4MH2.0
add_executable( test_crgctrl_20 test_crgctrl_20.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test/mock_guard_base.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD/test/test_crgctrl_20.cpp
  )
target_link_libraries( test_crgctrl_20
  libguard
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_crgctrl_20 COMMAND $<TARGET_FILE:test_crgctrl_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_crgctrl_20 coverage)
endif( )

# TEST CRGCAP
add_executable( test_crgcap test_crgcap.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test/mock_guard_base.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD/test/test_crgcap.cpp
  )
target_link_libraries( test_crgcap
  libguard
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_crgcap COMMAND $<TARGET_FILE:test_crgcap> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_crgcap coverage)
endif( )

# TEST CRGCAP G4MH2.0
add_executable( test_crgcap_20 test_crgcap_20.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test/mock_guard_base.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD/test/test_crgcap_20.cpp
  )
target_link_libraries( test_crgcap_20
  libguard
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_crgcap_20 COMMAND $<TARGET_FILE:test_crgcap_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_crgcap_20 coverage)
endif( )

# TEST CRG
add_executable( test_crg test_crg.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test/mock_guard_base.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD/test/test_crg.cpp
  )
target_link_libraries( test_crg
  libguard
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_crg COMMAND $<TARGET_FILE:test_crg> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_crg coverage)
endif( )

# TEST CRG G4MH2.0
add_executable( test_crg_20 test_crg_20.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test/mock_guard_base.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD/test/test_crg_20.cpp
  )
target_link_libraries( test_crg_20
  libguard
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_crg_20 COMMAND $<TARGET_FILE:test_crg_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_crg_20 coverage)
endif( )

# TEST TREG_crgsecurereg
add_executable( test_treg_crgsecurereg_20 test_treg_crgsecurereg_20.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test/mock_guard_base.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD/test/test_treg_crgsecurereg_20.cpp
  )
target_link_libraries( test_treg_crgsecurereg_20
  libguard
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_treg_crgsecurereg_20 COMMAND $<TARGET_FILE:test_treg_crgsecurereg_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_treg_crgsecurereg_20 coverage)
endif( )

# TEST CRGSECURE
add_executable( test_crgsecure_20 test_crgsecure_20.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test/mock_guard_base.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD/test/test_crgsecure_20.cpp
  )
target_link_libraries( test_crgsecure_20
  libguard
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_crgsecure_20 COMMAND $<TARGET_FILE:test_crgsecure_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_crgsecure_20 coverage)
endif( )

# run with: ctest -L CRG_function
set_property(
  TEST test_treg_crgctrlreg
  test_treg_crgctrlreg_20
  test_treg_crgerrreg
  test_treg_crgerrreg_20
  test_crgctrl
  test_crgctrl_20
  test_crgcap
  test_crgcap_20
  test_crg
  test_crg_20
  test_treg_crgsecurereg_20
  test_crgsecure_20
  PROPERTY LABELS CRG_function)
