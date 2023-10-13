cmake_minimum_required( VERSION 3.2 )

include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/API/CFORESTG4X/include
  ${TOP_SOURCE_DIR}/Device/Bus/FSBUS/include
  ${TOP_SOURCE_DIR}/Device/Core/CEDAR/include
  ${TOP_SOURCE_DIR}/Device/Core/COREGUARD/include
  ${TOP_SOURCE_DIR}/Device/Core/CYCLESIM/include
  ${TOP_SOURCE_DIR}/Device/Core/EXCEP/include
  ${TOP_SOURCE_DIR}/Device/Core/ICACHE/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/include
  ${TOP_SOURCE_DIR}/Device/Core/LLSC/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
  ${TOP_BINARY_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/MISR/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/DEBUGAREA/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/ERRORAREA/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC2/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/IPIR/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/LATENCYINFO/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/MECNT/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/MEM/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/OSTM/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/SIMIO/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/BAR/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/TMU/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/PERFCNT/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/UART/include
)
if( SUPPORT_SC_HEAP )
  include_directories( ${TOP_SOURCE_DIR}/Device/Peripheral/INVALIDLRAM/include )
endif( )
if( SUPPORT_CUBESUITE )
  include_directories( ${TOP_SOURCE_DIR}/Device/API/CSPLUS/include )
endif( )
add_library(
  device_utility OBJECT
  forest.cpp
  forest_internals.cpp
  forest_message.cpp
  forest_setup.cpp
  forest_utility.cpp
  trace_buf.cpp
)

add_cppcheck(device_utility UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE FAIL_ON_WARNINGS)

add_dependencies(
  device_utility
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
  gen_cedar_arch_info
)
