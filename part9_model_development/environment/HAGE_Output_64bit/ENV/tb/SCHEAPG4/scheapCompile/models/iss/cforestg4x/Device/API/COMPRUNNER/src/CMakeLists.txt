cmake_minimum_required( VERSION 3.2 )

# Generate cedar_arch_info
add_custom_command(
  OUTPUT ${DEVICE_API_COMPRUNNER_BINARY_DIR}/src/forest_cmdopt.cpp
  COMMAND ${CMAKE_COMMAND} -E copy forest_cmdopt.cpp ${DEVICE_API_COMPRUNNER_BINARY_DIR}/src
  DEPENDS ${TOP_SOURCE_DIR}/Device/API/CFORESTG4X/src/forest_cmdopt.cpp
  WORKING_DIRECTORY ${TOP_SOURCE_DIR}/Device/API/CFORESTG4X/src
  COMMENT "Copy forest_cmdopt.cpp."
)
add_custom_target(
  copy_forest_cmdopt ALL
  SOURCES forest_cmdopt.cpp
  WORKING_DIRECTORY ${TOP_SOURCE_DIR}/Device/API/CFORESTG4X/src
)

include_directories(
  ${TOP_SOURCE_DIR}/Device/API/CFORESTG4X/include
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
  ${TOP_SOURCE_DIR}/Device/Core/EXCEP/include
  ${TOP_SOURCE_DIR}/Device/Core/ICACHE/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/src/SoftFloat/source/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
  ${TOP_SOURCE_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_BINARY_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_BINARY_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include

  # for svdpi.h in VCS on RVC disk.
  /common/appl/Synopsys/vcs-mx_vH-2013.06-4/include
  # for svdpi.h in VCS on Musashi disk.
  /common/appl/Synopsys/vcs_mx/2013.06-4/include

  if( USE_PYTHON_API )
    ${TOP_SOURCE_DIR}/Device/API/PYTHON/include
    ${PYTHON_INCLUDE_DIR}
  endif( )
)

add_library(
  device_api_comprunner OBJECT
  cmp_runner.cpp
  cmp_runner_g4mh.cpp
  cmp_runner_his.cpp
  cmp_runner_if_g4mh.cpp
  xidump.cpp
  forest_cmdopt.cpp
)

add_dependencies( device_api_comprunner
  gen_cedar_arch_info
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
  gen_trace_operand
  copy_forest_cmdopt
)

add_cppcheck(device_api_comprunner UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE FAIL_ON_WARNINGS)

if( MSVC )
  set( hasSOCKET 0 )
else( )
  set( hasSOCKET 1 )
endif( )
add_library(
  forest_cmprunner_g4mh STATIC
  $<TARGET_OBJECTS:device_api_comprunner>
  $<${hasSOCKET}:$<TARGET_OBJECTS:device_api_gdb>>
  $<TARGET_OBJECTS:device_api_python>
  $<TARGET_OBJECTS:device_bus_fsbus>
  $<TARGET_OBJECTS:device_core_btb>
  $<TARGET_OBJECTS:device_core_cedar>
  $<TARGET_OBJECTS:device_core_coreguard>
  $<TARGET_OBJECTS:device_core_counterbase>
  $<TARGET_OBJECTS:device_core_cyclesim>
  $<TARGET_OBJECTS:device_core_debugbreak>
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
  $<TARGET_OBJECTS:device_peripheral_ostm>
  $<TARGET_OBJECTS:device_peripheral_perfcnt>
  $<$<BOOL:${USE_SIM_IO}>:$<TARGET_OBJECTS:device_peripheral_simio>>
  $<TARGET_OBJECTS:device_peripheral_targetreg>
  $<TARGET_OBJECTS:device_peripheral_bar>
  $<TARGET_OBJECTS:device_utility>
)

install( TARGETS forest_cmprunner_g4mh DESTINATION lib COMPONENT libraries )
