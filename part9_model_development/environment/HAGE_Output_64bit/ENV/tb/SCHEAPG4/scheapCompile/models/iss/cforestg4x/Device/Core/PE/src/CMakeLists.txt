cmake_minimum_required( VERSION 3.2 )

include_directories(
  ${TOP_SOURCE_DIR}/Device/Core/DEBUGBREAK/include
  ${TOP_SOURCE_DIR}/Device/Bus/FSBUS/include
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Core/BTB/include
  ${TOP_SOURCE_DIR}/Device/Core/COREGUARD/include
  ${TOP_SOURCE_DIR}/Device/Core/CYCLESIM/include
  ${TOP_SOURCE_DIR}/Device/Core/ICACHE/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/src/SoftFloat/source/include
  ${TOP_SOURCE_DIR}/Device/Core/LLSC/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
  ${TOP_BINARY_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/MISR/include
  ${TOP_SOURCE_DIR}/Device/Core/MPU/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/GUARD_BASE/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC2/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/OSTM/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/LATENCYINFO/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/MEM/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
)

if( SUPPORT_CUBESUITE )
  include_directories( ${TOP_SOURCE_DIR}/Device/API/CSPLUS/include )
endif( )

add_library(
  device_core_pe OBJECT
  proc_element.cpp
)

add_dependencies( device_core_pe
  gen_cedar_arch_info
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
)

add_cppcheck(device_core_pe UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE FAIL_ON_WARNINGS)
