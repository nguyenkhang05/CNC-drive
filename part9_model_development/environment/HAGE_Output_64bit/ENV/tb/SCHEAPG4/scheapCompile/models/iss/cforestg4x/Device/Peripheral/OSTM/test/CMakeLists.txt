cmake_minimum_required( VERSION 3.2 )


include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Core/EXCEP/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
  ${TOP_SOURCE_DIR}/Device/Bus/FSBUS/include
  ${TOP_BINARY_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/include
  ${TOP_SOURCE_DIR}/Device/Utility/include  
  ${TOP_SOURCE_DIR}/Device/Peripheral/OSTM/test
  ${TOP_SOURCE_DIR}/Device/Peripheral/OSTM/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/MEM/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC2/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/LATENCYINFO/include
  ${TOP_SOURCE_DIR}/Device/Utility/test
  ${gmock_SOURCE_DIR}/include
  ${gtest_SOURCE_DIR}/include  
)

add_library(libostm
  ${TOP_SOURCE_DIR}/Device/Peripheral/OSTM/src/os_timer.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/src/treg.cpp  
)

add_dependencies( libostm
#  gen_cedar_arch_info
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
)

if( MSVC )
  set( hasPTHREAD 0 )
else( )
  set( hasPTHREAD 1 )
endif( )

# TEST CMP
add_executable( test_ostm_tregostm0cmp test_ostm_tregostm0cmp.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/OSTM/test/test_ostm_tregostm0cmp.cpp

 )
target_link_libraries( test_ostm_tregostm0cmp
libostm
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_ostm_tregostm0cmp COMMAND $<TARGET_FILE:test_ostm_tregostm0cmp> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_ostm_tregostm0cmp coverage)
endif( )

# TEST CNT
add_executable( test_ostm_tregostm0cnt test_ostm_tregostm0cnt.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/OSTM/test/test_ostm_tregostm0cnt.cpp

 )
target_link_libraries( test_ostm_tregostm0cnt
libostm
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_ostm_tregostm0cnt COMMAND $<TARGET_FILE:test_ostm_tregostm0cnt> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_ostm_tregostm0cnt coverage)
endif( )


# TEST TO
add_executable( test_ostm_tregostm0to test_ostm_tregostm0to.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/OSTM/test/test_ostm_tregostm0to.cpp

 )
target_link_libraries( test_ostm_tregostm0to
libostm
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_ostm_tregostm0to COMMAND $<TARGET_FILE:test_ostm_tregostm0to> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_ostm_tregostm0to coverage)
endif( )

# TEST TOE
add_executable( test_ostm_tregostm0toe test_ostm_tregostm0toe.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/OSTM/test/test_ostm_tregostm0toe.cpp

 )
target_link_libraries( test_ostm_tregostm0toe
libostm
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_ostm_tregostm0toe COMMAND $<TARGET_FILE:test_ostm_tregostm0toe> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_ostm_tregostm0toe coverage)
endif( )

# TEST TE
add_executable( test_ostm_tregostm0te test_ostm_tregostm0te.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/OSTM/test/test_ostm_tregostm0te.cpp

 )
target_link_libraries( test_ostm_tregostm0te
libostm
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_ostm_tregostm0te COMMAND $<TARGET_FILE:test_ostm_tregostm0te> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_ostm_tregostm0te coverage)
endif( )

# TEST TS
add_executable( test_ostm_tregostm0ts test_ostm_tregostm0ts.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/OSTM/test/test_ostm_tregostm0ts.cpp

 )
target_link_libraries( test_ostm_tregostm0ts
libostm
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_ostm_tregostm0ts COMMAND $<TARGET_FILE:test_ostm_tregostm0ts> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_ostm_tregostm0ts coverage)
endif( )

# TEST TT
add_executable( test_ostm_tregostm0tt test_ostm_tregostm0tt.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/OSTM/test/test_ostm_tregostm0tt.cpp

 )
target_link_libraries( test_ostm_tregostm0tt
libostm
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_ostm_tregostm0tt COMMAND $<TARGET_FILE:test_ostm_tregostm0tt> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_ostm_tregostm0tt coverage)
endif( )

# TEST CTL
add_executable( test_ostm_tregostm0ctl test_ostm_tregostm0ctl.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/OSTM/test/test_ostm_tregostm0ctl.cpp

 )
target_link_libraries( test_ostm_tregostm0ctl
libostm
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_ostm_tregostm0ctl COMMAND $<TARGET_FILE:test_ostm_tregostm0ctl> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_ostm_tregostm0ctl coverage)
endif( )

# TEST CORE
add_executable( test_ostm_tregostm0core test_ostm_tregostm0core.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/OSTM/test/test_ostm_tregostm0core.cpp

 )
target_link_libraries( test_ostm_tregostm0core
libostm
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_ostm_tregostm0core COMMAND $<TARGET_FILE:test_ostm_tregostm0core> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_ostm_tregostm0core coverage)
endif( )

#run with: ctest -L Peripheral
set_property(
  TEST  test_ostm_tregostm0core test_ostm_tregostm0cmp test_ostm_tregostm0cnt test_ostm_tregostm0to test_ostm_tregostm0toe test_ostm_tregostm0te test_ostm_tregostm0ts test_ostm_tregostm0tt test_ostm_tregostm0ctl
PROPERTY LABELS OSTM )

