cmake_minimum_required( VERSION 3.2 )

include_directories(
  ${TOP_BINARY_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
  ${TOP_BINARY_DIR}/Device/COMMON/include
)

add_library(
  device_core_debugbreak OBJECT
  dbg_break.cpp
)

add_dependencies( device_core_debugbreak
  gen_cedar_arch_info
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
)

add_cppcheck(device_core_debugbreak UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE FAIL_ON_WARNINGS)
