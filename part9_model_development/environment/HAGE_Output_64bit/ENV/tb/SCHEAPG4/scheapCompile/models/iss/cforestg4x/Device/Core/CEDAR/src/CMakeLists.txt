cmake_minimum_required( VERSION 3.2 )

include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Core/BTB/include
  ${TOP_SOURCE_DIR}/Device/Core/CYCLESIM/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/src/SoftFloat/source/include
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
  ${TOP_SOURCE_DIR}/Device/Utility/include
)
if( SUPPORT_SC_HEAP )
  include_directories( ${TOP_SOURCE_DIR}/Device/API/SCHEAP/include )
endif( )
if( SUPPORT_CUBESUITE )
  include_directories( ${TOP_SOURCE_DIR}/Device/API/CSPLUS/include )
endif( )
add_library(
  device_core_cedar OBJECT
  cedar_factory.cpp
  cedar_func.cpp
)

add_dependencies( device_core_cedar
  gen_cedar_arch_info
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
  gen_trace_operand
)

add_cppcheck(device_core_cedar UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE FAIL_ON_WARNINGS)

add_dependencies(
  device_core_cedar
  gen_trace_operand
)
