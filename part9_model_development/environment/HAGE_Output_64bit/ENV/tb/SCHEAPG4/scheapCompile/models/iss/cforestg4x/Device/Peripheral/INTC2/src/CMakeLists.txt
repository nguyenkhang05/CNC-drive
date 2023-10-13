cmake_minimum_required( VERSION 3.2 )

include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Bus/FSBUS/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
  ${TOP_BINARY_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
)

if( USE_INTC20 )
    add_definitions( -D_INTC1_20 )
endif( )

add_library(
  device_peripheral_intc2 OBJECT
  intc2.cpp
)

add_dependencies( device_peripheral_intc2
  gen_cedar_arch_info
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
)

add_cppcheck(device_peripheral_intc2 UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE FAIL_ON_WARNINGS)
