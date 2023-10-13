cmake_minimum_required( VERSION 3.2 )

include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/test
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
  ${TOP_SOURCE_DIR}/Device/Bus/FSBUS/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/BAR/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/BAR/test
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
  ${TOP_SOURCE_DIR}/Device/Utility/test
  ${gmock_SOURCE_DIR}/include
  ${gtest_SOURCE_DIR}/include
  )
add_library(libbar
  ${TOP_SOURCE_DIR}/Device/Peripheral/BAR/src/treg_bar.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/src/treg.cpp
)

add_dependencies( libbar
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
)

if( MSVC )
  set( hasPTHREAD 0 )
else( )
  set( hasPTHREAD 1 )
endif( )

# TEST TREGBRINIT
add_executable( test_bar_tregbrinit test_bar_tregbrinit.cpp
  )
target_link_libraries( test_bar_tregbrinit
  libbar
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_bar_tregbrinit COMMAND $<TARGET_FILE:test_bar_tregbrinit> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_bar_tregbrinit coverage)
endif( )

# TEST TREGBREN
add_executable( test_bar_tregbren test_bar_tregbren.cpp
  )
target_link_libraries( test_bar_tregbren
  libbar
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_bar_tregbren COMMAND $<TARGET_FILE:test_bar_tregbren> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_bar_tregbren coverage)
endif( )

# TEST TREGBRCHK
add_executable( test_bar_tregbrchk test_bar_tregbrchk.cpp
  )
target_link_libraries( test_bar_tregbrchk
  libbar
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_bar_tregbrchk COMMAND $<TARGET_FILE:test_bar_tregbrchk> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_bar_tregbrchk coverage)
endif( )

# TEST TREGSYN
add_executable( test_bar_tregbrsyn test_bar_tregbrsyn.cpp
  )
target_link_libraries( test_bar_tregbrsyn
  libbar
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_bar_tregbrsyn COMMAND $<TARGET_FILE:test_bar_tregbrsyn> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_bar_tregbrsyn coverage)
endif( )


# TEST TREGBAR
add_executable( test_bar_tregbar test_bar_tregbar.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/BAR/src/bar.cpp
  )
target_link_libraries( test_bar_tregbar
  libbar
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_bar_tregbar COMMAND $<TARGET_FILE:test_bar_tregbar> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_bar_tregbar coverage)
endif( )

# TEST CBAR
add_executable( test_bar_cbar test_bar_cbar.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/BAR/src/bar.cpp
  )
target_link_libraries( test_bar_cbar
  libbar
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_bar_cbar COMMAND $<TARGET_FILE:test_bar_cbar> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_bar_cbar coverage)
endif( )

# run with: ctest -L Peripheral
set_property(
  TEST test_bar_tregbrinit test_bar_tregbren test_bar_tregbrchk test_bar_tregbrsyn test_bar_tregbar test_bar_cbar
  PROPERTY LABELS Peripheral )
