cmake_minimum_required( VERSION 3.2 )

add_custom_command(
  OUTPUT frog_reg_profile.h frog_reg_profile.cpp
  COMMAND ${CMAKE_COMMAND} -E make_directory ${DEVICE_API_FROG_BINARY_DIR}/src
  COMMAND ${CMAKE_COMMAND} -E make_directory ${DEVICE_API_FROG_BINARY_DIR}/include
  COMMAND ${PERL_EXECUTABLE} -w ${DEVICE_API_FROG_SOURCE_DIR}/tool/frog_if_reg_gen.pl ${BOM_OPT} ${CR_OPT} ${TOP_SOURCE_DIR}/Device/Core/REG/src/sysreg_g3m_reg.csv include/frog_reg_profile.h src/frog_reg_profile.cpp
  DEPENDS ${DEVICE_API_FROG_SOURCE_DIR}/tool/frog_if_reg_gen.pl ${TOP_SOURCE_DIR}/Device/Core/REG/src/sysreg_g3m_reg.csv
  WORKING_DIRECTORY ${DEVICE_API_FROG_BINARY_DIR}
  COMMENT "Generate frog_reg_profile."
)
add_custom_target(
  gen_frog_reg_profile ALL
  SOURCES frog_reg_profile.h frog_reg_profile.cpp
  WORKING_DIRECTORY ${DEVICE_API_FROG_BINARY_DIR}
)

add_custom_command(
  OUTPUT ${DEVICE_API_FROG_BINARY_DIR}/src/forest_cmdopt.cpp
  COMMAND ${CMAKE_COMMAND} -E copy forest_cmdopt.cpp ${DEVICE_API_FROG_BINARY_DIR}/src
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
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_BINARY_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
  ${TOP_SOURCE_DIR}/Device/API/CFORESTG4X/include
  ${TOP_BINARY_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/EXCEP/include

  if( USE_PYTHON_API )
    ${TOP_SOURCE_DIR}/Device/API/PYTHON/include
    ${PYTHON_INCLUDE_DIR}
  endif( )
)


add_library(
  device_api_frog OBJECT
  frog_if.cpp
  frog_internals.cpp
  frog_reg_profile.cpp
  forest_cmdopt.cpp
)


if( MSVC )
  set( hasSOCKET 0 )
  set( LIBTYPE SHARED )
else( )
  set( hasSOCKET 1 )
  set( LIBTYPE STATIC )
endif( )
add_library(
    forest_frog ${LIBTYPE}
    $<TARGET_OBJECTS:device_api_frog>
#    $<${hasSOCKET}:$<TARGET_OBJECTS:device_api_gdb>>
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
    $<TARGET_OBJECTS:device_peripheral_tmu>
    $<${hasSOCKET}:$<TARGET_OBJECTS:device_peripheral_uart>>
    $<TARGET_OBJECTS:device_peripheral_bar>
    $<TARGET_OBJECTS:device_utility>
)

add_dependencies( device_api_frog
  gen_cedar_arch_info
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
  gen_trace_operand
  gen_frog_reg_profile
  copy_forest_cmdopt
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh_1pe.py ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh11_1pe.py ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh20_1pe.py ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh21_1pe.py
  COMMAND ${CMAKE_COMMAND} -E copy init_CForest_g4mh_1pe.py ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy init_CForest_g4mh11_1pe.py ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy init_CForest_g4mh20_1pe.py ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy init_CForest_g4mh21_1pe.py ${CMAKE_CURRENT_BINARY_DIR}
  WORKING_DIRECTORY ${TOP_SOURCE_DIR}/Samples/python_script/FROG
  COMMENT "Copy init_CForest_g4mhx_1pe.py."
)
add_custom_target(
  copy_init_python_g4mh ALL
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh_1pe.py ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh11_1pe.py ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh20_1pe.py ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh21_1pe.py
)

install( FILES ${DEVICE_API_FROG_SOURCE_DIR}/include/frog_if.h DESTINATION lib COMPONENT samples )
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh_1pe.py DESTINATION lib COMPONENT samples )
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh11_1pe.py DESTINATION lib COMPONENT samples )
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh20_1pe.py DESTINATION lib COMPONENT samples )
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh21_1pe.py DESTINATION lib COMPONENT samples )
install( TARGETS forest_frog DESTINATION lib COMPONENT libraries )

add_cppcheck(device_api_frog UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE FAIL_ON_WARNINGS)
