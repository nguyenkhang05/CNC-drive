cmake_minimum_required( VERSION 3.2 )

include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/test
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/test
  ${TOP_SOURCE_DIR}/Device/Utility/include
  ${TOP_SOURCE_DIR}/Device/Utility/test
  ${gmock_SOURCE_DIR}/include
  ${gtest_SOURCE_DIR}/include
  )
add_library(libguardbase
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/src/treg_guard.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/src/treg.cpp
)

add_dependencies( libguardbase
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
)

if( MSVC )
  set( hasPTHREAD 0 )
else( )
  set( hasPTHREAD 1 )
endif( )

# TEST GUARD_INTERNAL
add_executable( test_guard_internal test_guard_internal.cpp
  )
target_link_libraries( test_guard_internal
  libguardbase
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_guard_internal COMMAND $<TARGET_FILE:test_guard_internal> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_guard_internal coverage)
endif( )

# TEST TREG_GKCPROT
add_executable( test_treg_gkcprot test_treg_gkcprot.cpp
  )
target_link_libraries( test_treg_gkcprot
  libguardbase
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_treg_gkcprot COMMAND $<TARGET_FILE:test_treg_gkcprot> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_treg_gkcprot coverage)
endif( )

# TEST TREG_GPROT
add_executable( test_treg_gprot test_treg_gprot.cpp
  )
target_link_libraries( test_treg_gprot
  libguardbase
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_treg_gprot COMMAND $<TARGET_FILE:test_treg_gprot> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_treg_gprot coverage)
endif( )

# TEST TREG_GPROT G4MH2.0
add_executable( test_treg_gprot_20 test_treg_gprot_20.cpp
  )
target_link_libraries( test_treg_gprot_20
  libguardbase
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_treg_gprot_20 COMMAND $<TARGET_FILE:test_treg_gprot_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_treg_gprot_20 coverage)
endif( )

# TEST TREG_GSPID
add_executable( test_treg_gspid test_treg_gspid.cpp
  )
target_link_libraries( test_treg_gspid
  libguardbase
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_treg_gspid COMMAND $<TARGET_FILE:test_treg_gspid> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_treg_gspid coverage)
endif( )

# TEST TREG_GIVCSPID G4MH2.0
add_executable( test_treg_givcspid_20 test_treg_givcspid_20.cpp
  )
target_link_libraries( test_treg_givcspid_20
  libguardbase
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_treg_givcspid_20 COMMAND $<TARGET_FILE:test_treg_givcspid_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_treg_givcspid_20 coverage)
endif( )

# TEST TREG_GIVCOWNR G4MH2.0
add_executable( test_treg_givcownr_20 test_treg_givcownr_20.cpp
  )
target_link_libraries( test_treg_givcownr_20
  libguardbase
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_treg_givcownr_20 COMMAND $<TARGET_FILE:test_treg_givcownr_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_treg_givcownr_20 coverage)
endif( )

# TEST TREG_GBAD
add_executable( test_treg_gbad test_treg_gbad.cpp
  )
target_link_libraries( test_treg_gbad
  libguardbase
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_treg_gbad COMMAND $<TARGET_FILE:test_treg_gbad> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_treg_gbad coverage)
endif( )

# TEST TREG_GADV
add_executable( test_treg_gadv test_treg_gadv.cpp
  )
target_link_libraries( test_treg_gadv
  libguardbase
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_treg_gadv COMMAND $<TARGET_FILE:test_treg_gadv> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_treg_gadv coverage)
endif( )

# TEST TREG_GERRSTAT
add_executable( test_treg_gerrstat test_treg_gerrstat.cpp
  )
target_link_libraries( test_treg_gerrstat
  libguardbase
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_treg_gerrstat COMMAND $<TARGET_FILE:test_treg_gerrstat> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_treg_gerrstat coverage)
endif( )

# TEST TREG_GERRCLR
add_executable( test_treg_gerrclr test_treg_gerrclr.cpp
  )
target_link_libraries( test_treg_gerrclr
  libguardbase
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_treg_gerrclr COMMAND $<TARGET_FILE:test_treg_gerrclr> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_treg_gerrclr coverage)
endif( )

# TEST TREG_GERRADDR
add_executable( test_treg_gerraddr test_treg_gerraddr.cpp
  )
target_link_libraries( test_treg_gerraddr
  libguardbase
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_treg_gerraddr COMMAND $<TARGET_FILE:test_treg_gerraddr> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_treg_gerraddr coverage)
endif( )

# TEST TREG_GERRTYPE
add_executable( test_treg_gerrtype test_treg_gerrtype.cpp
  )
target_link_libraries( test_treg_gerrtype
  libguardbase
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_treg_gerrtype COMMAND $<TARGET_FILE:test_treg_gerrtype> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_treg_gerrtype coverage)
endif( )

# run with: ctest -L Slave_Guard
set_property(
  TEST test_treg_gkcprot test_treg_gprot test_treg_gprot_20 test_treg_gspid test_treg_gbad
test_treg_gadv test_treg_gerrstat test_treg_gerrclr test_treg_gerraddr test_treg_givcspid_20
test_treg_givcownr_20 test_treg_gerrtype test_guard_internal
  PROPERTY LABELS Slave_Guard)
