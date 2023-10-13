cmake_minimum_required( VERSION 3.2 )

include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Core/CYCLESIM/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/src/SoftFloat/source/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
  ${TOP_BINARY_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/LATENCYINFO/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
)

add_library(
  device_api_scheap OBJECT
  suspend_ctrl.cpp
)

add_cppcheck(device_api_scheap UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE FAIL_ON_WARNINGS)

add_dependencies(
  device_api_scheap
  gen_cedar_arch_info
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
)

add_library(
  forest_scheap SHARED
  $<TARGET_OBJECTS:device_api_scheap>
  $<TARGET_OBJECTS:device_bus_fsbus>
  $<TARGET_OBJECTS:device_core_btb>
  $<TARGET_OBJECTS:device_core_cedar>
  $<TARGET_OBJECTS:device_core_coreguard>
  $<TARGET_OBJECTS:device_core_counterbase>
  $<TARGET_OBJECTS:device_core_cyclesim>
  $<TARGET_OBJECTS:device_core_excep>
  $<TARGET_OBJECTS:device_core_icache>
  $<TARGET_OBJECTS:device_core_inst>
  $<TARGET_OBJECTS:softfloat>
  $<TARGET_OBJECTS:device_core_llsc>
  $<TARGET_OBJECTS:device_core_lsu>
  $<TARGET_OBJECTS:device_core_microarchdb>
  $<TARGET_OBJECTS:device_core_misr>
  $<TARGET_OBJECTS:device_core_mpu>
  $<TARGET_OBJECTS:device_core_pe>
  $<TARGET_OBJECTS:device_core_reg>
  $<TARGET_OBJECTS:device_core_simbreak>
  $<TARGET_OBJECTS:device_core_statistics>
  $<TARGET_OBJECTS:device_core_trace>
  $<TARGET_OBJECTS:device_peripheral_debugarea>
  $<TARGET_OBJECTS:device_peripheral_errorarea>
  $<TARGET_OBJECTS:device_peripheral_guard>
  $<TARGET_OBJECTS:device_peripheral_guard_base>
  $<TARGET_OBJECTS:device_peripheral_intc>
  $<TARGET_OBJECTS:device_peripheral_intc2>
  $<TARGET_OBJECTS:device_peripheral_invalidlram>
  $<TARGET_OBJECTS:device_peripheral_ipir>
  $<TARGET_OBJECTS:device_peripheral_latencyinfo>
  $<TARGET_OBJECTS:device_peripheral_mecnt>
  $<TARGET_OBJECTS:device_peripheral_mem>
  $<TARGET_OBJECTS:device_peripheral_targetreg>
  $<TARGET_OBJECTS:device_peripheral_bar>
  $<TARGET_OBJECTS:device_utility>
  )

install( TARGETS forest_scheap DESTINATION lib COMPONENT libraries )
