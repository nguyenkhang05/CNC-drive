cmake_minimum_required( VERSION 3.2 )


include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/MEM/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/LATENCYINFO/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/LATENCYINFO/test
  ${TOP_SOURCE_DIR}/Device/Utility/include
  ${gmock_SOURCE_DIR}/include
  ${gtest_SOURCE_DIR}/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
)

add_library(liblatencyinfo
  ${TOP_SOURCE_DIR}/Device/Peripheral/LATENCYINFO/src/outstand.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/LATENCYINFO/src/dbuf.cpp
  #${TOP_SOURCE_DIR}/Device/Peripheral/LATENCYINFO/src/flash_cache.cpp
)

add_dependencies( liblatencyinfo
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
add_executable( test_cram test_cram.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/LATENCYINFO/test/test_cram.cpp
 )
target_link_libraries( test_cram
liblatencyinfo
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_cram COMMAND $<TARGET_FILE:test_cram> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_cram coverage)
endif( )
