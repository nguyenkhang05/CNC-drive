cmake_minimum_required( VERSION 3.2 )

include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/LATENCYINFO/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/MEM/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
)

add_library(
  device_bus_clbridge OBJECT
  cl_bridge.cpp
)

add_dependencies( device_bus_clbridge
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
)

add_cppcheck(device_bus_clbridge UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE FAIL_ON_WARNINGS)
