cmake_minimum_required( VERSION 3.2 )

if( POLICY CMP0065 )
  cmake_policy( SET CMP0065 OLD )
endif( POLICY CMP0065 )

include_directories(
  ${TOP_SOURCE_DIR}/Device/API/GDB/include
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
  if( USE_PYTHON_API )
    ${TOP_SOURCE_DIR}/Device/API/PYTHON/include
    ${PYTHON_INCLUDE_DIR}
  endif( )
)

add_library(
  device_api_cforestg4x OBJECT
  forest_cmdopt.cpp
)

add_cppcheck(device_api_cforestg4x UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE FAIL_ON_WARNINGS)

if( ${PRODUCT_TYPE}  MATCHES "^CFORESTG4X$" )
  set( useLIB 1 )
  set( useOBJ 0 )
else( )
  set( useLIB 0 )
  set( useOBJ 1 )
endif( )

if( MSVC )
  set( hasSOCKET 0 )
else( )
  set( hasSOCKET 1 )
endif( )
add_executable( cforestg4x
    forest_main.cpp
    $<TARGET_OBJECTS:device_api_cforestg4x>
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
    $<${useOBJ}:$<TARGET_OBJECTS:softfloat>>
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
    $<TARGET_OBJECTS:device_peripheral_tmu>
    $<${hasSOCKET}:$<TARGET_OBJECTS:device_peripheral_uart>>
    $<TARGET_OBJECTS:device_peripheral_bar>
    $<TARGET_OBJECTS:device_utility>
)

add_dependencies( device_api_cforestg4x
  gen_cedar_arch_info
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
  gen_trace_operand
)

if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME} cforestg4x coverage)
endif( )
target_link_libraries( cforestg4x
  $<${useLIB}:softfloat>
  $<$<BOOL:${USE_PYTHON_API}>:${PYTHON_LIBRARY}>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )

install( TARGETS cforestg4x DESTINATION bin COMPONENT applications )
