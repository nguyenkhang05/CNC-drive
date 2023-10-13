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
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test
  ${TOP_SOURCE_DIR}/Device/Utility/include
  ${TOP_SOURCE_DIR}/Device/Utility/test
    
  ${gmock_SOURCE_DIR}/include
  ${gtest_SOURCE_DIR}/include
)

add_library(libcoreguard
  ${TOP_SOURCE_DIR}/Device/Core/COREGUARD/src/peg.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/src/treg_guard.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/src/treg.cpp
)


add_dependencies( libcoreguard
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
)


if( MSVC )
  set( hasPTHREAD 0 )
else( )
  set( hasPTHREAD 1 )
endif( )


# TEST TREG_pegctrlreg
add_executable( test_treg_pegctrlreg test_treg_pegctrlreg.cpp
  ${TOP_SOURCE_DIR}/Device/Core/COREGUARD/test/test_treg_pegctrlreg.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test/mock_guard_base.cpp
  )
target_link_libraries( test_treg_pegctrlreg
  libcoreguard
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_treg_pegctrlreg COMMAND $<TARGET_FILE:test_treg_pegctrlreg> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_treg_pegctrlreg coverage)
endif( )

# TEST TREG_pegerrreg
add_executable( test_treg_pegerrreg test_treg_pegerrreg.cpp
  ${TOP_SOURCE_DIR}/Device/Core/COREGUARD/test/test_treg_pegerrreg.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test/mock_guard_base.cpp
  )
target_link_libraries( test_treg_pegerrreg
  libcoreguard
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_treg_pegerrreg COMMAND $<TARGET_FILE:test_treg_pegerrreg> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_treg_pegerrreg coverage)
endif( )

# TEST TREG_pegerrreg G4MH2.0
add_executable( test_treg_pegerrreg_20 test_treg_pegerrreg_20.cpp
  ${TOP_SOURCE_DIR}/Device/Core/COREGUARD/test/test_treg_pegerrreg_20.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test/mock_guard_base.cpp
  )
target_link_libraries( test_treg_pegerrreg_20
  libcoreguard
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_treg_pegerrreg_20 COMMAND $<TARGET_FILE:test_treg_pegerrreg_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_treg_pegerrreg_20 coverage)
endif( )


# TEST PEGCTRL
add_executable( test_pegctrl test_pegctrl.cpp
  ${TOP_SOURCE_DIR}/Device/Core/COREGUARD/test/test_pegctrl.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test/mock_guard_base.cpp
  )
target_link_libraries( test_pegctrl
  libcoreguard
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_pegctrl COMMAND $<TARGET_FILE:test_pegctrl> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_pegctrl coverage)
endif( )

# TEST PEGCAP
add_executable( test_pegcap test_pegcap.cpp
  ${TOP_SOURCE_DIR}/Device/Core/COREGUARD/test/test_pegcap.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test/mock_guard_base.cpp
  )
target_link_libraries( test_pegcap
  libcoreguard
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_pegcap COMMAND $<TARGET_FILE:test_pegcap> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_pegcap coverage)
endif( )

# TEST PEG
add_executable( test_peg test_peg.cpp
  ${TOP_SOURCE_DIR}/Device/Core/COREGUARD/test/test_peg.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test/mock_guard_base.cpp
  )
target_link_libraries( test_peg
  libcoreguard
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_peg COMMAND $<TARGET_FILE:test_peg> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_peg coverage)
endif( )

# TEST PEG G4MH2.0
add_executable( test_peg_20 test_peg_20.cpp
  ${TOP_SOURCE_DIR}/Device/Core/COREGUARD/test/test_peg_20.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test/mock_guard_base.cpp
  )
target_link_libraries( test_peg_20
  libcoreguard
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_peg_20 COMMAND $<TARGET_FILE:test_peg_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_peg_20 coverage)
endif( )

# run with: ctest -L PEG_function
set_property(
  TEST test_treg_pegctrlreg test_treg_pegerrreg test_treg_pegerrreg_20 test_pegctrl test_pegcap test_peg test_peg_20
  PROPERTY LABELS PEG_function)
