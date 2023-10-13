cmake_minimum_required( VERSION 3.2 )

set( ARCHFILE V850E3V5_MicroArchDB.txt )

find_package( Perl )

# Generate trace_operand
if( MSVC )
  set( BOM_OPT "--bom" )
  set( CR_OPT  "--crlf" )
else( )
  set( BOM_OPT "" )
  set( CR_OPT  "" )
endif( )
add_custom_command(
  OUTPUT ${DEVICE_CORE_TRACE_BINARY_DIR}/include/trace_operand.h ${DEVICE_CORE_TRACE_BINARY_DIR}/src/trace_operand_gen.cpp
  COMMAND ${PERL_EXECUTABLE} -w ${DEVICE_CORE_TRACE_SOURCE_DIR}/tool/trace_operand_gen.pl ${BOM_OPT} ${CR_OPT} ${DEVICE_CORE_MICROARCHDB_SOURCE_DIR}/src/${ARCHFILE}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${DEVICE_CORE_TRACE_BINARY_DIR}/include
  COMMAND ${CMAKE_COMMAND} -E copy trace_operand.h ${DEVICE_CORE_TRACE_BINARY_DIR}/include
  COMMAND ${CMAKE_COMMAND} -E remove trace_operand.h
  DEPENDS ${DEVICE_CORE_TRACE_SOURCE_DIR}/tool/trace_operand_gen.pl ${DEVICE_CORE_MICROARCHDB_SOURCE_DIR}/src/${ARCHFILE}
  WORKING_DIRECTORY ${DEVICE_CORE_TRACE_BINARY_DIR}/src
  COMMENT "Generate trace_operand."
)
add_custom_target(
  gen_trace_operand ALL
  SOURCES ${DEVICE_CORE_TRACE_BINARY_DIR}/include/trace_operand.h ${DEVICE_CORE_TRACE_BINARY_DIR}/src/trace_operand_gen.cpp
  WORKING_DIRECTORY ${DEVICE_CORE_TRACE_BINARY_DIR}/src
)

include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Core/BTB/include
  ${TOP_SOURCE_DIR}/Device/Core/EXCEP/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/src/SoftFloat/source/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
  ${TOP_BINARY_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
)

add_library(
  device_core_trace OBJECT
  trace_info.cpp
  trace_operand.cpp
  trace_operand_gen.cpp
  trace_print.cpp
)


add_dependencies( device_core_trace
  gen_cedar_arch_info
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
)

add_cppcheck(device_core_trace UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE FAIL_ON_WARNINGS)
