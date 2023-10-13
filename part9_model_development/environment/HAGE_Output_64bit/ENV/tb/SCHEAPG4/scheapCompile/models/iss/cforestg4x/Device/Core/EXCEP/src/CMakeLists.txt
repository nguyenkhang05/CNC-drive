cmake_minimum_required( VERSION 3.2 )

include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
)

add_library(
  device_core_excep OBJECT
  exp_info.cpp
  exp_message.cpp
)

add_dependencies( device_core_excep
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
)

add_cppcheck(device_core_excep UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE FAIL_ON_WARNINGS)
