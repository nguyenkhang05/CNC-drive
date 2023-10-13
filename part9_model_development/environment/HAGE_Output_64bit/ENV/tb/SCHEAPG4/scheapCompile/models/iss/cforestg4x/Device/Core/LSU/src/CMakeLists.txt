cmake_minimum_required( VERSION 3.2 )

include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Bus/FSBUS/include
  ${TOP_SOURCE_DIR}/Device/Core/COREGUARD/include
  ${TOP_SOURCE_DIR}/Device/Core/ICACHE/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/include
  ${TOP_SOURCE_DIR}/Device/Core/LLSC/include
  ${TOP_BINARY_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/MISR/include
  ${TOP_SOURCE_DIR}/Device/Core/MPU/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
)
if( ENABLE_DEBUG )
  include_directories( ${TOP_SOURCE_DIR}/Device/Core/DEBUGBREAK/include )
endif( )

if( MSVC )
else( )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-type-limits" )
endif( )

add_library(
  device_core_lsu OBJECT
  lsu.cpp
)

add_dependencies( device_core_lsu
  gen_cedar_arch_info
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
)

add_cppcheck(device_core_lsu UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE FAIL_ON_WARNINGS)
