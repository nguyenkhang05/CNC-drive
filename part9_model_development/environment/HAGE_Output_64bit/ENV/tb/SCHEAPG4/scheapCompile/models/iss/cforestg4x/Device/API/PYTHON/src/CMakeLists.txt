cmake_minimum_required( VERSION 3.2 )

find_package( PythonLibs 2.7 REQUIRED )
find_package( PythonInterp 2.7 REQUIRED )

if( MSVC )
  set( RUN_SHELL "" )
else( )
  set( RUN_SHELL "sh" )
endif( )
set( INCBIN "${DEVICE_API_PYTHON_SOURCE_DIR}/tool/incbin/incbin.bat" )
add_custom_command(
  OUTPUT CommandDef.h CommandAlias.h elfloader.h profiler.h
  COMMAND ${CMAKE_COMMAND} -E make_directory ${DEVICE_API_PYTHON_BINARY_DIR}/include
  COMMAND ${RUN_SHELL} ${INCBIN} CommandDef.py ${DEVICE_API_PYTHON_BINARY_DIR}/include/CommandDef.h CommandDef
  COMMAND ${RUN_SHELL} ${INCBIN} CommandAlias.py ${DEVICE_API_PYTHON_BINARY_DIR}/include/CommandAlias.h CommandAlias
  COMMAND ${RUN_SHELL} ${INCBIN} elfloader.py ${DEVICE_API_PYTHON_BINARY_DIR}/include/elfloader.h elfloader
  COMMAND ${RUN_SHELL} ${INCBIN} profiler.py ${DEVICE_API_PYTHON_BINARY_DIR}/include/profiler.h profiler
  WORKING_DIRECTORY ${DEVICE_API_PYTHON_SOURCE_DIR}/src
  COMMENT "Generate data header from python script."
)
add_custom_target(
  gen_python_header ALL
  SOURCES CommandDef.h CommandAlias.h elfloader.h profiler.h
  WORKING_DIRECTORY ${DEVICE_API_PYTHON_SOURCE_DIR}/src
)

include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/API/CFORESTG4X/include
  ${TOP_SOURCE_DIR}/Device/API/GDB/include
  ${TOP_BINARY_DIR}/Device/API/PYTHON/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_BINARY_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
  ${TOP_SOURCE_DIR}/Device/Bus/FSBUS/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC2/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/UART/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/include
  ${PYTHON_INCLUDE_DIR}
  if( PRODUCT_TYPE COMPRUNNER )
    ${TOP_SOURCE_DIR}/Device/API/COMPRUNNER/include
    # for svdpi.h in VCS on RVC disk.
    /common/appl/Synopsys/vcs-mx_vH-2013.06-4/include
    # for svdpi.h in VCS on Musashi disk.
    /common/appl/Synopsys/vcs_mx/2013.06-4/include
  endif( )
)

add_library(
  device_api_python OBJECT
  forest_python.cpp
)

add_dependencies( device_api_python gen_python_header gen_sysreg_enum_h gen_sysreg_file_h gen_sysreg_h)

add_cppcheck( device_api_python UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE FAIL_ON_WARNINGS )
