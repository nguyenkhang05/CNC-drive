cmake_minimum_required( VERSION 3.2 )

include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
  ${TOP_SOURCE_DIR}/Device/Utility/test
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${gmock_SOURCE_DIR}/include
  ${gtest_SOURCE_DIR}/include
)
add_library(libmecnt
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/src/treg.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/MECNT/src/mecnt.cpp 
)

add_dependencies( libmecnt
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
)

if( MSVC )
  set( hasPTHREAD 0 )
else( )
  set( hasPTHREAD 1 )
endif( )

# TEST MECNT
add_executable( test_mecnt test_mecnt.cpp
)
target_link_libraries( test_mecnt libmecnt gmock gtest $<${hasPTHREAD}:pthread> )

add_test( NAME mecnt COMMAND $<TARGET_FILE:test_mecnt> )

# run with: ctest -L Peripheral
set_property( TEST mecnt PROPERTY LABELS Peripheral test_mecnt )
