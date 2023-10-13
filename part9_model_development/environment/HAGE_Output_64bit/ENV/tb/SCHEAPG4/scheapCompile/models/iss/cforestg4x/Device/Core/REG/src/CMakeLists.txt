cmake_minimum_required( VERSION 3.2 )

set( SYSREG_ENUM      sreg_enum.h )
set( SYSREG_FILE      sregfile.h )
set( SYSREG_FILE_INIT sregfile_init.cpp )
set( MOCK_SYSREG_FILE mock_sregfile.h )
set( SYSREG           sreg.h )
set( SYSREG_CSV       sysreg_g3m_reg.csv )
set( SYSREGBIT_CSV    sysreg_g3m_bit.csv )
set( MOCK_SYSREG      mock_sreg.h )

find_package( Perl )

# Generate sreg_enum_h
if( MSVC )
  set( BOM_OPT "--bom" )
  set( CR_OPT  "--crlf" )
else( )
  set( BOM_OPT "" )
  set( CR_OPT  "" )
endif( )
add_custom_command(
  OUTPUT ${DEVICE_CORE_REG_BINARY_DIR}/include/${SYSREG_ENUM}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${DEVICE_CORE_REG_BINARY_DIR}/include
  COMMAND ${PERL_EXECUTABLE} ${DEVICE_CORE_REG_SOURCE_DIR}/tool/sreg_enum_gen.pl ${BOM_OPT} ${CR_OPT} ${DEVICE_CORE_REG_SOURCE_DIR}/src/${SYSREG_CSV} ${DEVICE_CORE_REG_SOURCE_DIR}/src/${SYSREGBIT_CSV} ${DEVICE_CORE_REG_BINARY_DIR}/include/${SYSREG_ENUM}
  DEPENDS ${DEVICE_CORE_REG_SOURCE_DIR}/tool/sreg_enum_gen.pl ${DEVICE_CORE_REG_SOURCE_DIR}/src/${SYSREG_CSV} ${DEVICE_CORE_REG_SOURCE_DIR}/src/${SYSREGBIT_CSV}
  COMMENT "Generate ${SYSREG_ENUM}."
)
add_custom_target(
  gen_sysreg_enum_h ALL
  SOURCES ${DEVICE_CORE_REG_BINARY_DIR}/include/${SYSREG_ENUM}
)

# Generate sregfile.h,sregfile_init.cpp,mock_sregfile.h
add_custom_command(
  OUTPUT ${DEVICE_CORE_REG_BINARY_DIR}/include/${SYSREG_FILE} ${DEVICE_CORE_REG_BINARY_DIR}/src/${SYSREG_FILE_INIT} ${DEVICE_CORE_REG_BINARY_DIR}/test/${MOCK_SYSREG_FILE}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${DEVICE_CORE_REG_BINARY_DIR}/include
  COMMAND ${CMAKE_COMMAND} -E make_directory ${DEVICE_CORE_REG_BINARY_DIR}/test
  COMMAND ${PERL_EXECUTABLE} ${DEVICE_CORE_REG_SOURCE_DIR}/tool/sregfile_gen.pl ${BOM_OPT} ${CR_OPT} ${DEVICE_CORE_REG_SOURCE_DIR}/src/${SYSREG_CSV} ${DEVICE_CORE_REG_BINARY_DIR}/include/${SYSREG_FILE} ${DEVICE_CORE_REG_BINARY_DIR}/src/${SYSREG_FILE_INIT}  ${DEVICE_CORE_REG_BINARY_DIR}/test/${MOCK_SYSREG_FILE}
  DEPENDS ${DEVICE_CORE_REG_SOURCE_DIR}/tool/sregfile_gen.pl ${DEVICE_CORE_REG_SOURCE_DIR}/src/${SYSREG_CSV}
  COMMENT "Generate ${SYSREG_FILE},${SYSREG_FILE_INIT},${MOCK_SYSREG_FILE}."
)
add_custom_target(
  gen_sysreg_file_h ALL
  SOURCES ${DEVICE_CORE_REG_BINARY_DIR}/include/${SYSREG_FILE} ${DEVICE_CORE_REG_BINARY_DIR}/src/${SYSREG_FILE_INIT}
)
# Generate sreg.h, mock_sreg.h
add_custom_command(
  OUTPUT ${DEVICE_CORE_REG_BINARY_DIR}/include/${SYSREG} ${DEVICE_CORE_REG_BINARY_DIR}/test/${MOCK_SYSREG}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${DEVICE_CORE_REG_BINARY_DIR}/include
  COMMAND ${CMAKE_COMMAND} -E make_directory ${DEVICE_CORE_REG_BINARY_DIR}/test
  COMMAND ${PERL_EXECUTABLE} ${DEVICE_CORE_REG_SOURCE_DIR}/tool/sreg_gen.pl ${BOM_OPT} ${CR_OPT} ${DEVICE_CORE_REG_SOURCE_DIR}/src/${SYSREG_CSV} ${DEVICE_CORE_REG_SOURCE_DIR}/src/${SYSREGBIT_CSV} ${DEVICE_CORE_REG_BINARY_DIR}/include/${SYSREG} ${DEVICE_CORE_REG_BINARY_DIR}/test/${MOCK_SYSREG}
  DEPENDS ${DEVICE_CORE_REG_SOURCE_DIR}/tool/sreg_gen.pl ${DEVICE_CORE_REG_SOURCE_DIR}/src/${SYSREG_CSV} ${DEVICE_CORE_REG_SOURCE_DIR}/src/${SYSREGBIT_CSV}
  COMMENT "Generate ${SYSREG}."
)
add_custom_target(
  gen_sysreg_h ALL
  SOURCES ${DEVICE_CORE_REG_BINARY_DIR}/include/${SYSREG}
)

include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Core/COUNTERBASE/include
  ${TOP_SOURCE_DIR}/Device/Core/DEBUGBREAK/include
  ${TOP_SOURCE_DIR}/Device/Core/ICACHE/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
  ${TOP_BINARY_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/MPU/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
)

if( SUPPORT_CUBESUITE )
  include_directories( ${TOP_SOURCE_DIR}/Device/API/CSPLUS/include )
endif( )

add_library(
  device_core_reg OBJECT
  gregfile.cpp
  regid.cpp
  sreg_special.cpp
  sregfile.cpp
  sregfile_init.cpp
)

add_dependencies( device_core_reg
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
  gen_cedar_arch_info
)

add_cppcheck(device_core_reg UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE FAIL_ON_WARNINGS)
