cmake_minimum_required( VERSION 3.2 )

include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Core/ICACHE/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
  ${TOP_BINARY_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/STATISTICS/include
  ${TOP_BINARY_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/LATENCYINFO/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
)
if( SUPPORT_SC_HEAP )
  include_directories( ${TOP_SOURCE_DIR}/Device/API/SCHEAP/include )
endif( )

add_library(
  device_core_cyclesim OBJECT
  cs_controler.cpp
  cs_ibuf.cpp
  cs_ibuf_g3mh.cpp
  cs_ibuf_g3kh.cpp
  cs_lsu_buf.cpp
)

add_dependencies( device_core_cyclesim
  gen_cedar_arch_info
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
  gen_trace_operand
)

add_cppcheck(device_core_cyclesim UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE FAIL_ON_WARNINGS)
