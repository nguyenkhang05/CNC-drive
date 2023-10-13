cmake_minimum_required( VERSION 3.2 )

set( ARCHFILE V850E3V5_MicroArchDB.txt )

find_package( Perl )

# Generate cedar_arch_info
if( MSVC )
  set( BOM_OPT "--bom" )
  set( CR_OPT  "--crlf" )
else( )
  set( BOM_OPT "" )
  set( CR_OPT  "" )
endif( )

set(OUTPUTS
  ${DEVICE_CORE_MICROARCHDB_BINARY_DIR}/src/cedar_arch_info.cpp
  ${DEVICE_CORE_MICROARCHDB_BINARY_DIR}/src/inst_func_table.cpp
  ${DEVICE_CORE_MICROARCHDB_BINARY_DIR}/include/inst_id_list.h
  ${DEVICE_CORE_MICROARCHDB_BINARY_DIR}/include/inst_declaration.h
  ${DEVICE_CORE_MICROARCHDB_BINARY_DIR}/include/inst_argtype.h
)
add_custom_command(
  PRE_BUILD
  OUTPUT ${OUTPUTS}
  COMMAND ${PERL_EXECUTABLE} -w ${DEVICE_CORE_MICROARCHDB_SOURCE_DIR}/tool/cedar_arch_info_gen.pl ${BOM_OPT} ${CR_OPT} ${DEVICE_CORE_MICROARCHDB_SOURCE_DIR}/src/${ARCHFILE} cedar_arch_info
  COMMAND ${CMAKE_COMMAND} -E make_directory ${DEVICE_CORE_MICROARCHDB_BINARY_DIR}/include
  COMMAND ${CMAKE_COMMAND} -E copy inst_id_list.h ${DEVICE_CORE_MICROARCHDB_BINARY_DIR}/include
  COMMAND ${CMAKE_COMMAND} -E copy inst_declaration.h ${DEVICE_CORE_MICROARCHDB_BINARY_DIR}/include
  COMMAND ${CMAKE_COMMAND} -E copy inst_argtype.h ${DEVICE_CORE_MICROARCHDB_BINARY_DIR}/include
  COMMAND ${CMAKE_COMMAND} -E remove inst_id_list.h inst_declaration.h inst_argtype.h
  DEPENDS ${DEVICE_CORE_MICROARCHDB_SOURCE_DIR}/tool/cedar_arch_info_gen.pl ${DEVICE_CORE_MICROARCHDB_SOURCE_DIR}/src/${ARCHFILE}
  WORKING_DIRECTORY ${DEVICE_CORE_MICROARCHDB_BINARY_DIR}/src
  COMMENT "Generate cedar_arch_info."
)
set_source_files_properties(${DEVICE_CORE_MICROARCHDB_BINARY_DIR}/src/cedar_arch_info.cpp PROPERTIES GENERATED TRUE)
set_source_files_properties(${DEVICE_CORE_MICROARCHDB_BINARY_DIR}/src/inst_func_table.cpp PROPERTIES GENERATED TRUE)
set_source_files_properties(${DEVICE_CORE_MICROARCHDB_BINARY_DIR}/include/inst_id_list.h PROPERTIES GENERATED TRUE)
set_source_files_properties(${DEVICE_CORE_MICROARCHDB_BINARY_DIR}/include/inst_declaration.h PROPERTIES GENERATED TRUE)
set_source_files_properties(${DEVICE_CORE_MICROARCHDB_BINARY_DIR}/include/inst_argtype.h PROPERTIES GENERATED TRUE)

add_custom_target(
  gen_cedar_arch_info ALL
  DEPENDS ${OUTPUTS}
)

include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/src/SoftFloat/source/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
  ${TOP_BINARY_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/MICROARCHDB/include
)

add_library(
  device_core_microarchdb OBJECT
  micro_arch_db.cpp
  cedar_arch_info.cpp
  inst_func_table.cpp
)

add_dependencies( device_core_microarchdb
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
  gen_cedar_arch_info
)

add_cppcheck(device_core_microarchdb UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE FAIL_ON_WARNINGS)
