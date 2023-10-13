cmake_minimum_required ( VERSION 3.2 )

include_directories (
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/src
  ${TOP_SOURCE_DIR}/Device/Core/PE/test
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/src/SoftFloat/source/CForestG4X
  ${TOP_SOURCE_DIR}/Device/Core/INST/src/SoftFloat/source/include
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${gmock_SOURCE_DIR}/include
  ${gtest_SOURCE_DIR}/include
  )

add_library (libinst
  ${TOP_SOURCE_DIR}/Device/Core/INST/src/SoftFloatAdapter.cpp
  )

add_dependencies ( libinst
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
  )

if ( ${PRODUCT_TYPE}  MATCHES "^CFORESTG4X$" )
  set ( useLIB 1 )
  set ( useOBJ 0 )
else ( )
  set ( useLIB 0 )
  set ( useOBJ 1 )
endif ( )

if( MSVC )
  set( hasPTHREAD 0 )
else( )
  set( hasPTHREAD 1 )
endif( )

# TEST SoftFloatAdapter class
add_executable ( test_SoftFloatAdapter  test_SoftFloatAdapter.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>>)
target_link_libraries ( test_SoftFloatAdapter
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_SoftFloatAdapter COMMAND $<TARGET_FILE:test_SoftFloatAdapter> )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_SoftFloatAdapter coverage)
endif ( )

# TEST CheckOutputSubnormal method
add_executable ( test_check_output_subnormal test_check_output_subnormal.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>>)
target_link_libraries ( test_check_output_subnormal
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_check_output_subnormal COMMAND $<TARGET_FILE:test_check_output_subnormal> )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_check_output_subnormal coverage)
endif ( )

# TEST FLOAT32_MADD
add_executable ( test_float32_madd test_float32_madd.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_madd
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_madd COMMAND $<TARGET_FILE:test_float32_madd> )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_madd coverage)
endif ( )

# TEST FLOAT32_MADD_FULLTEST
add_executable ( test_float32_madd_full_rm_max test_float32_madd_full_rm_max.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_madd_full_rm_max
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_madd_full_rm_max COMMAND $<TARGET_FILE:test_float32_madd_full_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_madd_full_rm_max coverage)
endif ( )
add_executable ( test_float32_madd_full_rm_min test_float32_madd_full_rm_min.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_madd_full_rm_min
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_madd_full_rm_min COMMAND $<TARGET_FILE:test_float32_madd_full_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_madd_full_rm_min coverage)
endif ( )
add_executable ( test_float32_madd_full_rm_minmag test_float32_madd_full_rm_minmag.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_madd_full_rm_minmag
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_madd_full_rm_minmag COMMAND $<TARGET_FILE:test_float32_madd_full_rm_minmag> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_madd_full_rm_minmag coverage)
endif ( )
add_executable ( test_float32_madd_full_rm_neareven test_float32_madd_full_rm_neareven.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_madd_full_rm_neareven
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_madd_full_rm_neareven COMMAND $<TARGET_FILE:test_float32_madd_full_rm_neareven> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_madd_full_rm_neareven coverage)
endif ( )

# TEST FLOAT32_MSUB
add_executable ( test_float32_msub test_float32_msub.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_msub
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_msub COMMAND $<TARGET_FILE:test_float32_msub> )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_msub coverage)
endif ( )

# TEST FLOAT_CVT_SW
add_executable ( test_float_cvt_sw test_float_cvt_sw.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_cvt_sw
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_cvt_sw COMMAND $<TARGET_FILE:test_float_cvt_sw> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_cvt_sw coverage)
endif ( )

# TEST FLOAT_ROUND_SW
add_executable ( test_float_round_sw test_float_round_sw.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_round_sw
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_round_sw COMMAND $<TARGET_FILE:test_float_round_sw> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_round_sw coverage)
endif ( )

# TEST FLOAT_TRNC_SW
add_executable ( test_float_trnc_sw test_float_trnc_sw.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_trnc_sw
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_trnc_sw COMMAND $<TARGET_FILE:test_float_trnc_sw> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_trnc_sw coverage)
endif ( )

# TEST FLOAT_FLOOR_SW
add_executable ( test_float_floor_sw test_float_floor_sw.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_floor_sw
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_floor_sw COMMAND $<TARGET_FILE:test_float_floor_sw> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_floor_sw coverage)
endif ( )

# TEST FLOAT_CEIL_SW
add_executable ( test_float_ceil_sw test_float_ceil_sw.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_ceil_sw
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_ceil_sw COMMAND $<TARGET_FILE:test_float_ceil_sw> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_ceil_sw coverage)
endif ( )

# TEST FLOAT_CVT_SUW
add_executable ( test_float_cvt_suw test_float_cvt_suw.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_cvt_suw
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_cvt_suw COMMAND $<TARGET_FILE:test_float_cvt_suw> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_cvt_suw coverage)
endif ( )

# TEST FLOAT_ROUND_SUW
add_executable ( test_float_round_suw test_float_round_suw.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_round_suw
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_round_suw COMMAND $<TARGET_FILE:test_float_round_suw> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_round_suw coverage)
endif ( )

# TEST FLOAT_CEIL_SUW
add_executable ( test_float_ceil_suw test_float_ceil_suw.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_ceil_suw
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_ceil_suw COMMAND $<TARGET_FILE:test_float_ceil_suw> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_ceil_suw coverage)
endif ( )

# TEST FLOAT_FLOOR_SUW
add_executable ( test_float_floor_suw test_float_floor_suw.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_floor_suw
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_floor_suw COMMAND $<TARGET_FILE:test_float_floor_suw> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_floor_suw coverage)
endif ( )

# TEST FLOAT_TRNC_SUW
add_executable ( test_float_trnc_suw test_float_trnc_suw.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_trnc_suw
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_trnc_suw COMMAND $<TARGET_FILE:test_float_trnc_suw> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_trnc_suw coverage)
endif ( )

# TEST FLOAT_CVT_SL
add_executable ( test_float_cvt_sl test_float_cvt_sl.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_cvt_sl
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_cvt_sl COMMAND $<TARGET_FILE:test_float_cvt_sl> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_cvt_sl coverage)
endif ( )

# TEST FLOAT_ROUND_SL
add_executable ( test_float_round_sl test_float_round_sl.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_round_sl
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_round_sl COMMAND $<TARGET_FILE:test_float_round_sl> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_round_sl coverage)
endif ( )

# TEST FLOAT_CEIL_SL
add_executable ( test_float_ceil_sl test_float_ceil_sl.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_ceil_sl
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_ceil_sl COMMAND $<TARGET_FILE:test_float_ceil_sl> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_ceil_sl coverage)
endif ( )

# TEST FLOAT_FLOOR_SL
add_executable ( test_float_floor_sl test_float_floor_sl.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_floor_sl
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_floor_sl COMMAND $<TARGET_FILE:test_float_floor_sl> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_floor_sl coverage)
endif ( )

# TEST FLOAT_TRNC_SL
add_executable ( test_float_trnc_sl test_float_trnc_sl.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_trnc_sl
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_trnc_sl COMMAND $<TARGET_FILE:test_float_trnc_sl> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_trnc_sl coverage)
endif ( )

# TEST FLOAT_CVT_SUL
add_executable ( test_float_cvt_sul test_float_cvt_sul.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_cvt_sul
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_cvt_sul COMMAND $<TARGET_FILE:test_float_cvt_sul> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_cvt_sul coverage)
endif ( )

# TEST FLOAT_ROUND_SUL
add_executable ( test_float_round_sul test_float_round_sul.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_round_sul
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_round_sul COMMAND $<TARGET_FILE:test_float_round_sul> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_round_sul coverage)
endif ( )

# TEST FLOAT_CEIL_SUL
add_executable ( test_float_ceil_sul test_float_ceil_sul.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_ceil_sul
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_ceil_sul COMMAND $<TARGET_FILE:test_float_ceil_sul> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_ceil_sul coverage)
endif ( )

# TEST FLOAT_FLOOR_SUL
add_executable ( test_float_floor_sul test_float_floor_sul.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_floor_sul
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_floor_sul COMMAND $<TARGET_FILE:test_float_floor_sul> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_floor_sul coverage)
endif ( )

# TEST FLOAT_TRNC_SUL
add_executable ( test_float_trnc_sul test_float_trnc_sul.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_trnc_sul
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_trnc_sul COMMAND $<TARGET_FILE:test_float_trnc_sul> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_trnc_sul coverage)
endif ( )

# TEST FLOAT_CVT_DW
add_executable ( test_float_cvt_dw test_float_cvt_dw.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_cvt_dw
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_cvt_dw COMMAND $<TARGET_FILE:test_float_cvt_dw> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_cvt_dw coverage)
endif ( )

# TEST FLOAT_ROUND_DW
add_executable ( test_float_round_dw test_float_round_dw.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_round_dw
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_round_dw COMMAND $<TARGET_FILE:test_float_round_dw> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_round_dw coverage)
endif ( )

# TEST FLOAT_TRUNCATE_DW
add_executable ( test_float_trnc_dw test_float_trnc_dw.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_trnc_dw
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_trnc_dw COMMAND $<TARGET_FILE:test_float_trnc_dw> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_trnc_dw coverage)
endif ( )

# TEST FLOAT_CEIL_DW
add_executable ( test_float_ceil_dw test_float_ceil_dw.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_ceil_dw
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_ceil_dw COMMAND $<TARGET_FILE:test_float_ceil_dw> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_ceil_dw coverage)
endif ( )

# TEST FLOAT_FLOOR_DW
add_executable ( test_float_floor_dw test_float_floor_dw.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_floor_dw
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_floor_dw COMMAND $<TARGET_FILE:test_float_floor_dw> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_floor_dw coverage)
endif ( )

# TEST FLOAT_CVT_DUW
add_executable ( test_float_cvt_duw test_float_cvt_duw.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_cvt_duw
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_cvt_duw COMMAND $<TARGET_FILE:test_float_cvt_duw> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_cvt_duw coverage)
endif ( )

# TEST FLOAT_ROUND_DUW
add_executable ( test_float_round_duw test_float_round_duw.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_round_duw
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_round_duw COMMAND $<TARGET_FILE:test_float_round_duw> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_round_duw coverage)
endif ( )

# TEST FLOAT_TRUNCATE_DUW
add_executable ( test_float_trnc_duw test_float_trnc_duw.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_trnc_duw
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_trnc_duw COMMAND $<TARGET_FILE:test_float_trnc_duw> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_trnc_duw coverage)
endif ( )

# TEST FLOAT_CEIL_DUW
add_executable ( test_float_ceil_duw test_float_ceil_duw.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_ceil_duw
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_ceil_duw COMMAND $<TARGET_FILE:test_float_ceil_duw> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_ceil_duw coverage)
endif ( )

# TEST FLOAT_FLOOR_DUW
add_executable ( test_float_floor_duw test_float_floor_duw.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_floor_duw
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_floor_duw COMMAND $<TARGET_FILE:test_float_floor_duw> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_floor_duw coverage)
endif ( )

# TEST FLOAT_CVT_DL
add_executable ( test_float_cvt_dl test_float_cvt_dl.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_cvt_dl
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_cvt_dl COMMAND $<TARGET_FILE:test_float_cvt_dl> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_cvt_dl coverage)
endif ( )

# TEST FLOAT_ROUND_DL
add_executable ( test_float_round_dl test_float_round_dl.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_round_dl
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_round_dl COMMAND $<TARGET_FILE:test_float_round_dl> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_round_dl coverage)
endif ( )

# TEST FLOAT_TRUNCATE_DL
add_executable ( test_float_trnc_dl test_float_trnc_dl.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_trnc_dl
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_trnc_dl COMMAND $<TARGET_FILE:test_float_trnc_dl> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_trnc_dl coverage)
endif ( )

# TEST FLOAT_CEIL_DL
add_executable ( test_float_ceil_dl test_float_ceil_dl.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_ceil_dl
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_ceil_dl COMMAND $<TARGET_FILE:test_float_ceil_dl> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_ceil_dl coverage)
endif ( )

# TEST FLOAT_FLOOR_DL
add_executable ( test_float_floor_dl test_float_floor_dl.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_floor_dl
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_floor_dl COMMAND $<TARGET_FILE:test_float_floor_dl> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_floor_dl coverage)
endif ( )

# TEST FLOAT_CVT_DUL
add_executable ( test_float_cvt_dul test_float_cvt_dul.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_cvt_dul
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_cvt_dul COMMAND $<TARGET_FILE:test_float_cvt_dul> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_cvt_dul coverage)
endif ( )

# TEST FLOAT_ROUND_DUL
add_executable ( test_float_round_dul test_float_round_dul.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_round_dul
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_round_dul COMMAND $<TARGET_FILE:test_float_round_dul> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_round_dul coverage)
endif ( )

# TEST FLOAT_TRUNCATE_DUL
add_executable ( test_float_trnc_dul test_float_trnc_dul.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_trnc_dul
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_trnc_dul COMMAND $<TARGET_FILE:test_float_trnc_dul> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_trnc_dul coverage)
endif ( )

# TEST FLOAT_CEIL_DUL
add_executable ( test_float_ceil_dul test_float_ceil_dul.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_ceil_dul
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_ceil_dul COMMAND $<TARGET_FILE:test_float_ceil_dul> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_ceil_dul
    overage)
endif ( )

# TEST FLOAT_FLOOR_DUL
add_executable ( test_float_floor_dul test_float_floor_dul.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_floor_dul
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_floor_dul COMMAND $<TARGET_FILE:test_float_floor_dul> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_floor_dul coverage)
endif ( )

# TEST FLOAT_CVT_WS
add_executable ( test_float_cvt_ws test_float_cvt_ws.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_cvt_ws
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_cvt_ws COMMAND $<TARGET_FILE:test_float_cvt_ws> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_cvt_ws coverage)
endif ( )

# TEST FLOAT_CVT_UWS
add_executable ( test_float_cvt_uws test_float_cvt_uws.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_cvt_uws
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_cvt_uws COMMAND $<TARGET_FILE:test_float_cvt_uws> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_cvt_uws coverage)
endif ( )

# TEST FLOAT_CVT_LS
add_executable ( test_float_cvt_ls test_float_cvt_ls.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_cvt_ls
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_cvt_ls COMMAND $<TARGET_FILE:test_float_cvt_ls> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_cvt_ls coverage)
endif ( )

# TEST FLOAT_CVT_ULS
add_executable ( test_float_cvt_uls test_float_cvt_uls.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_cvt_uls
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_cvt_uls COMMAND $<TARGET_FILE:test_float_cvt_uls> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_cvt_uls coverage)
endif ( )

# TEST FLOAT_CVT_WD
add_executable ( test_float_cvt_wd test_float_cvt_wd.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_cvt_wd
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_cvt_wd COMMAND $<TARGET_FILE:test_float_cvt_wd> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_cvt_wd coverage)
endif ( )

# TEST FLOAT_CVT_UWD
add_executable ( test_float_cvt_uwd test_float_cvt_uwd.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_cvt_uwd
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_cvt_uwd COMMAND $<TARGET_FILE:test_float_cvt_uwd> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_cvt_uwd coverage)
endif ( )

# TEST FLOAT_CVT_LD
add_executable ( test_float_cvt_ld test_float_cvt_ld.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_cvt_ld
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_cvt_ld COMMAND $<TARGET_FILE:test_float_cvt_ld> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_cvt_ld coverage)
endif ( )

# TEST FLOAT_CVT_ULD
add_executable ( test_float_cvt_uld test_float_cvt_uld.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_cvt_uld
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_cvt_uld COMMAND $<TARGET_FILE:test_float_cvt_uld> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_cvt_uld coverage)
endif ( )

# TEST CONVERT DS
add_executable ( test_float_cvt_ds test_float_cvt_ds.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_cvt_ds
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_cvt_ds COMMAND $<TARGET_FILE:test_float_cvt_ds> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_cvt_ds coverage)
endif ( )

# TEST CONVERT_SD
add_executable ( test_float_cvt_sd test_float_cvt_sd.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_cvt_sd
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_cvt_sd COMMAND $<TARGET_FILE:test_float_cvt_sd> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_cvt_sd coverage)
endif ( )

# TEST CONVERT_SH
add_executable ( test_float_cvt_sh test_float_cvt_sh.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_cvt_sh
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_cvt_sh COMMAND $<TARGET_FILE:test_float_cvt_sh> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_cvt_sh coverage)
endif ( )

# TEST CONVERT_HS
add_executable ( test_float_cvt_hs test_float_cvt_hs.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float_cvt_hs
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float_cvt_hs COMMAND $<TARGET_FILE:test_float_cvt_hs> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float_cvt_hs coverage)
endif ( )

# TEST FLOAT32_RSQRT
add_executable ( test_float32_rsqrt test_float32_rsqrt.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_rsqrt
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_rsqrt COMMAND $<TARGET_FILE:test_float32_rsqrt> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_rsqrt coverage)
endif ( )

# TEST FLOAT64_RSQRT
add_executable ( test_float64_rsqrt test_float64_rsqrt.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float64_rsqrt
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float64_rsqrt COMMAND $<TARGET_FILE:test_float64_rsqrt> ${CMAKE_CURRENT_SOURCE_DIR} )
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float64_rsqrt coverage)
endif ( )

# TEST FLOAT32_MINFS
add_executable ( test_float32_minfs test_float32_minfs.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_minfs
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_minfs COMMAND $<TARGET_FILE:test_float32_minfs> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_minfs coverage)
endif ( )

# TEST FLOAT64_MIN
add_executable ( test_float64_min test_float64_min.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float64_min
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float64_min COMMAND $<TARGET_FILE:test_float64_min> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float64_min coverage)
endif ( )

# TEST FLOAT32_MAXFS
add_executable ( test_float32_maxfs test_float32_maxfs.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_maxfs
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_maxfs COMMAND $<TARGET_FILE:test_float32_maxfs> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_maxfs coverage)
endif ( )

# TEST FLOAT64_MAX
add_executable ( test_float64_max test_float64_max.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float64_max
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float64_max COMMAND $<TARGET_FILE:test_float64_max> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float64_max coverage)
endif ( )

# TEST FLOAT32_RECIPFS
add_executable ( test_float32_recipfs test_float32_recipfs.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_recipfs
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_recipfs COMMAND $<TARGET_FILE:test_float32_recipfs> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_recipfs coverage)
endif ( )

# TEST FLOAT64_RECIP
add_executable ( test_float64_recip test_float64_recip.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float64_recip
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float64_recip COMMAND $<TARGET_FILE:test_float64_recip> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float64_recip coverage)
endif ( )

# TEST FLOAT32_DIVFS
add_executable ( test_float32_divfs test_float32_divfs.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_divfs
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_divfs COMMAND $<TARGET_FILE:test_float32_divfs> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_divfs coverage)
endif ( )

# TEST FLOAT64_DIV
add_executable ( test_float64_div test_float64_div.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float64_div
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float64_div COMMAND $<TARGET_FILE:test_float64_div> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float64_div coverage)
endif ( )

# TEST FLOAT32_CMPFS
add_executable ( test_float32_cmpfs test_float32_cmpfs.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_cmpfs
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_cmpfs COMMAND $<TARGET_FILE:test_float32_cmpfs> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_cmpfs coverage)
endif ( )

# TEST FLOAT64_CMP
add_executable ( test_float64_cmp test_float64_cmp.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float64_cmp
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float64_cmp COMMAND $<TARGET_FILE:test_float64_cmp> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float64_cmp coverage)
endif ( )

# TEST FLOAT32_SQRTFS
add_executable ( test_float32_sqrt test_float32_sqrt.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_sqrt
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_sqrt COMMAND $<TARGET_FILE:test_float32_sqrt> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_sqrt coverage)
endif ( )

# TEST FLOAT64_SQRT
add_executable ( test_float64_sqrt test_float64_sqrt.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float64_sqrt
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float64_sqrt COMMAND $<TARGET_FILE:test_float64_sqrt> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float64_sqrt coverage)
endif ( )

# TEST FLOAT32_ABSFS
add_executable ( test_float32_abs test_float32_abs.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_abs
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_abs COMMAND $<TARGET_FILE:test_float32_abs> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_abs coverage)
endif ( )

# TEST FLOAT64_ABS
add_executable ( test_float64_abs test_float64_abs.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float64_abs
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float64_abs COMMAND $<TARGET_FILE:test_float64_abs> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float64_abs coverage)
endif ( )

# TEST FLOAT32_NEGFS
add_executable ( test_float32_neg test_float32_neg.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_neg
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_neg COMMAND $<TARGET_FILE:test_float32_neg> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_neg coverage)
endif ( )

# TEST FLOAT64_NEG
add_executable ( test_float64_neg test_float64_neg.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float64_neg
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float64_neg COMMAND $<TARGET_FILE:test_float64_neg> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float64_neg coverage)
endif ( )

# TEST FLOAT32_ADDFS
add_executable ( test_float32_add test_float32_add.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_add
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_add COMMAND $<TARGET_FILE:test_float32_add> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_add coverage)
endif ( )

# TEST FLOAT64_ADD
add_executable ( test_float64_add test_float64_add.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float64_add
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float64_add COMMAND $<TARGET_FILE:test_float64_add> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float64_add coverage)
endif ( )

# TEST FLOAT32_SUB
add_executable ( test_float32_sub test_float32_sub.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_sub
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_sub COMMAND $<TARGET_FILE:test_float32_sub> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_sub coverage)
endif ( )

# TEST FLOAT64_SUB
add_executable ( test_float64_sub test_float64_sub.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float64_sub
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float64_sub COMMAND $<TARGET_FILE:test_float64_sub> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float64_sub coverage)
endif ( )

# TEST FLOAT32_MULFS
add_executable ( test_float32_mul test_float32_mul.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_mul
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_mul COMMAND $<TARGET_FILE:test_float32_mul> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_mul coverage)
endif ( )

# TEST FLOAT64_MUL
add_executable ( test_float64_mul test_float64_mul.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float64_mul
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float64_mul COMMAND $<TARGET_FILE:test_float64_mul> ${CMAKE_CURRENT_SOURCE_DIR})
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float64_mul coverage)
endif ( )

# TEST FLOAT32_FMA
add_executable ( test_float32_fma test_float32_fma.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_fma
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_fma-a COMMAND $<TARGET_FILE:test_float32_fma> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fmaf_s-a.csv")
add_test ( NAME test_float32_fma-b COMMAND $<TARGET_FILE:test_float32_fma> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fmaf_s-b.csv")
add_test ( NAME test_float32_fma-c COMMAND $<TARGET_FILE:test_float32_fma> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fmaf_s-c.csv")
add_test ( NAME test_float32_fma-d COMMAND $<TARGET_FILE:test_float32_fma> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fmaf_s-d.csv")
add_test ( NAME test_float32_fma-e COMMAND $<TARGET_FILE:test_float32_fma> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fmaf_s-e.csv")
add_test ( NAME test_float32_fma-f COMMAND $<TARGET_FILE:test_float32_fma> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fmaf_s-f.csv")
add_test ( NAME test_float32_fma-g COMMAND $<TARGET_FILE:test_float32_fma> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fmaf_s-g.csv")
add_test ( NAME test_float32_fma-h COMMAND $<TARGET_FILE:test_float32_fma> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fmaf_s-h.csv")
add_test ( NAME test_float32_fma-i COMMAND $<TARGET_FILE:test_float32_fma> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fmaf_s-i.csv")
add_test ( NAME test_float32_fma-j COMMAND $<TARGET_FILE:test_float32_fma> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fmaf_s-j.csv")
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma-a coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma-b coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma-c coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma-d coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma-e coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma-f coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma-g coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma-h coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma-i coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma-j coverage)
endif ( )

add_executable ( test_float32_fma_RTG_rm_max test_float32_fma_RTG_rm_max.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries (test_float32_fma_RTG_rm_max
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_fma_RTG_rm_max-a COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_max_1M_0-a.csv")
add_test ( NAME test_float32_fma_RTG_rm_max-b COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_max_1M_0-b.csv")
add_test ( NAME test_float32_fma_RTG_rm_max-c COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_max_1M_0-c.csv")
add_test ( NAME test_float32_fma_RTG_rm_max-d COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_max_1M_0-d.csv")
add_test ( NAME test_float32_fma_RTG_rm_max-e COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_max_1M_0-e.csv")
add_test ( NAME test_float32_fma_RTG_rm_max-f COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_max_1M_0-f.csv")
add_test ( NAME test_float32_fma_RTG_rm_max-g COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_max_1M_0-g.csv")
add_test ( NAME test_float32_fma_RTG_rm_max-h COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_max_1M_0-h.csv")
add_test ( NAME test_float32_fma_RTG_rm_max-i COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_max_1M_0-i.csv")
add_test ( NAME test_float32_fma_RTG_rm_max-j COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_max_1M_0-j.csv")
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_max-a coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_max-b coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_max-c coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_max-d coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_max-e coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_max-f coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_max-g coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_max-h coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_max-i coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_max-j coverage)
endif ( )

add_executable ( test_float32_fma_RTG_rm_min test_float32_fma_RTG_rm_min.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries (test_float32_fma_RTG_rm_min
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_fma_RTG_rm_min-a COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_min_1M_0-a.csv")
add_test ( NAME test_float32_fma_RTG_rm_min-b COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_min_1M_0-b.csv")
add_test ( NAME test_float32_fma_RTG_rm_min-c COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_min_1M_0-c.csv")
add_test ( NAME test_float32_fma_RTG_rm_min-d COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_min_1M_0-d.csv")
add_test ( NAME test_float32_fma_RTG_rm_min-e COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_min_1M_0-e.csv")
add_test ( NAME test_float32_fma_RTG_rm_min-f COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_min_1M_0-f.csv")
add_test ( NAME test_float32_fma_RTG_rm_min-g COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_min_1M_0-g.csv")
add_test ( NAME test_float32_fma_RTG_rm_min-h COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_min_1M_0-h.csv")
add_test ( NAME test_float32_fma_RTG_rm_min-i COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_min_1M_0-i.csv")
add_test ( NAME test_float32_fma_RTG_rm_min-j COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_min_1M_0-j.csv")
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_min-a coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_min-b coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_min-c coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_min-d coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_min-e coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_min-f coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_min-g coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_min-h coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_min-i coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_min-j coverage)
endif ( )

add_executable ( test_float32_fma_RTG_rm_minMag test_float32_fma_RTG_rm_minMag.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries (test_float32_fma_RTG_rm_minMag
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_fma_RTG_rm_minMag-a COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_minMag_1M_0-a.csv")
add_test ( NAME test_float32_fma_RTG_rm_minMag-b COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_minMag_1M_0-b.csv")
add_test ( NAME test_float32_fma_RTG_rm_minMag-c COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_minMag_1M_0-c.csv")
add_test ( NAME test_float32_fma_RTG_rm_minMag-d COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_minMag_1M_0-d.csv")
add_test ( NAME test_float32_fma_RTG_rm_minMag-e COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_minMag_1M_0-e.csv")
add_test ( NAME test_float32_fma_RTG_rm_minMag-f COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_minMag_1M_0-f.csv")
add_test ( NAME test_float32_fma_RTG_rm_minMag-g COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_minMag_1M_0-g.csv")
add_test ( NAME test_float32_fma_RTG_rm_minMag-h COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_minMag_1M_0-h.csv")
add_test ( NAME test_float32_fma_RTG_rm_minMag-i COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_minMag_1M_0-i.csv")
add_test ( NAME test_float32_fma_RTG_rm_minMag-j COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_minMag_1M_0-j.csv")
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_minMag-a coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_minMag-b coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_minMag-c coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_minMag-d coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_minMag-e coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_minMag-f coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_minMag-g coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_minMag-h coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_minMag-i coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_minMag-j coverage)
endif ( )

add_executable ( test_float32_fma_RTG_rm_near_even test_float32_fma_RTG_rm_near_even.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries (test_float32_fma_RTG_rm_near_even
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_fma_RTG_rm_near_even-a COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_near_even_1M_0-a.csv")
add_test ( NAME test_float32_fma_RTG_rm_near_even-b COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_near_even_1M_0-b.csv")
add_test ( NAME test_float32_fma_RTG_rm_near_even-c COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_near_even_1M_0-c.csv")
add_test ( NAME test_float32_fma_RTG_rm_near_even-d COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_near_even_1M_0-d.csv")
add_test ( NAME test_float32_fma_RTG_rm_near_even-e COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_near_even_1M_0-e.csv")
add_test ( NAME test_float32_fma_RTG_rm_near_even-f COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_near_even_1M_0-f.csv")
add_test ( NAME test_float32_fma_RTG_rm_near_even-g COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_near_even_1M_0-g.csv")
add_test ( NAME test_float32_fma_RTG_rm_near_even-h COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_near_even_1M_0-h.csv")
add_test ( NAME test_float32_fma_RTG_rm_near_even-i COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_near_even_1M_0-i.csv")
add_test ( NAME test_float32_fma_RTG_rm_near_even-j COMMAND $<TARGET_FILE:test_float32_fma_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmaf_near_even_1M_0-j.csv")
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_near_even-a coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_near_even-b coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_near_even-c coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_near_even-d coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_near_even-e coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_near_even-f coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_near_even-g coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_near_even-h coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_near_even-i coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fma_RTG_rm_near_even-j coverage)
endif ( )

# TEST FLOAT32_FMSF
add_executable ( test_float32_fmsf test_float32_fmsf.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_fmsf
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_fmsf-a COMMAND $<TARGET_FILE:test_float32_fmsf> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fmsf_s-a.csv")
add_test ( NAME test_float32_fmsf-b COMMAND $<TARGET_FILE:test_float32_fmsf> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fmsf_s-b.csv")
add_test ( NAME test_float32_fmsf-c COMMAND $<TARGET_FILE:test_float32_fmsf> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fmsf_s-c.csv")
add_test ( NAME test_float32_fmsf-d COMMAND $<TARGET_FILE:test_float32_fmsf> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fmsf_s-d.csv")
add_test ( NAME test_float32_fmsf-e COMMAND $<TARGET_FILE:test_float32_fmsf> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fmsf_s-e.csv")
add_test ( NAME test_float32_fmsf-f COMMAND $<TARGET_FILE:test_float32_fmsf> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fmsf_s-f.csv")
add_test ( NAME test_float32_fmsf-g COMMAND $<TARGET_FILE:test_float32_fmsf> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fmsf_s-g.csv")
add_test ( NAME test_float32_fmsf-h COMMAND $<TARGET_FILE:test_float32_fmsf> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fmsf_s-h.csv")
add_test ( NAME test_float32_fmsf-i COMMAND $<TARGET_FILE:test_float32_fmsf> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fmsf_s-i.csv")
add_test ( NAME test_float32_fmsf-j COMMAND $<TARGET_FILE:test_float32_fmsf> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fmsf_s-j.csv")
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf-a coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf-b coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf-c coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf-d coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf-e coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf-f coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf-g coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf-h coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf-i coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf-j coverage)
endif ( )

add_executable ( test_float32_fmsf_rm_max test_float32_fmsf_rm_max.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries (test_float32_fmsf_rm_max
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_fmsf_rm_max-a COMMAND $<TARGET_FILE:test_float32_fmsf_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_max_1M_0-a.csv")
add_test ( NAME test_float32_fmsf_rm_max-b COMMAND $<TARGET_FILE:test_float32_fmsf_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_max_1M_0-b.csv")
add_test ( NAME test_float32_fmsf_rm_max-c COMMAND $<TARGET_FILE:test_float32_fmsf_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_max_1M_0-c.csv")
add_test ( NAME test_float32_fmsf_rm_max-d COMMAND $<TARGET_FILE:test_float32_fmsf_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_max_1M_0-d.csv")
add_test ( NAME test_float32_fmsf_rm_max-e COMMAND $<TARGET_FILE:test_float32_fmsf_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_max_1M_0-e.csv")
add_test ( NAME test_float32_fmsf_rm_max-f COMMAND $<TARGET_FILE:test_float32_fmsf_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_max_1M_0-f.csv")
add_test ( NAME test_float32_fmsf_rm_max-g COMMAND $<TARGET_FILE:test_float32_fmsf_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_max_1M_0-g.csv")
add_test ( NAME test_float32_fmsf_rm_max-h COMMAND $<TARGET_FILE:test_float32_fmsf_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_max_1M_0-h.csv")
add_test ( NAME test_float32_fmsf_rm_max-i COMMAND $<TARGET_FILE:test_float32_fmsf_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_max_1M_0-i.csv")
add_test ( NAME test_float32_fmsf_rm_max-j COMMAND $<TARGET_FILE:test_float32_fmsf_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_max_1M_0-j.csv")
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_max-a coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_max-b coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_max-c coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_max-d coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_max-e coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_max-f coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_max-g coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_max-h coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_max-i coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_max-j coverage)
endif ( )

add_executable ( test_float32_fmsf_rm_min test_float32_fmsf_rm_min.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries (test_float32_fmsf_rm_min
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_fmsf_rm_min-a COMMAND $<TARGET_FILE:test_float32_fmsf_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_min_1M_0-a.csv")
add_test ( NAME test_float32_fmsf_rm_min-b COMMAND $<TARGET_FILE:test_float32_fmsf_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_min_1M_0-b.csv")
add_test ( NAME test_float32_fmsf_rm_min-c COMMAND $<TARGET_FILE:test_float32_fmsf_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_min_1M_0-c.csv")
add_test ( NAME test_float32_fmsf_rm_min-d COMMAND $<TARGET_FILE:test_float32_fmsf_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_min_1M_0-d.csv")
add_test ( NAME test_float32_fmsf_rm_min-e COMMAND $<TARGET_FILE:test_float32_fmsf_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_min_1M_0-e.csv")
add_test ( NAME test_float32_fmsf_rm_min-f COMMAND $<TARGET_FILE:test_float32_fmsf_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_min_1M_0-f.csv")
add_test ( NAME test_float32_fmsf_rm_min-g COMMAND $<TARGET_FILE:test_float32_fmsf_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_min_1M_0-g.csv")
add_test ( NAME test_float32_fmsf_rm_min-h COMMAND $<TARGET_FILE:test_float32_fmsf_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_min_1M_0-h.csv")
add_test ( NAME test_float32_fmsf_rm_min-i COMMAND $<TARGET_FILE:test_float32_fmsf_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_min_1M_0-i.csv")
add_test ( NAME test_float32_fmsf_rm_min-j COMMAND $<TARGET_FILE:test_float32_fmsf_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_min_1M_0-j.csv")
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_min-a coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_min-b coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_min-c coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_min-d coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_min-e coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_min-f coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_min-g coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_min-h coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_min-i coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_min-j coverage)
endif ( )

add_executable ( test_float32_fmsf_rm_minMag test_float32_fmsf_rm_minMag.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries (test_float32_fmsf_rm_minMag
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_fmsf_rm_minMag-a COMMAND $<TARGET_FILE:test_float32_fmsf_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_minMag_1M_0-a.csv")
add_test ( NAME test_float32_fmsf_rm_minMag-b COMMAND $<TARGET_FILE:test_float32_fmsf_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_minMag_1M_0-b.csv")
add_test ( NAME test_float32_fmsf_rm_minMag-c COMMAND $<TARGET_FILE:test_float32_fmsf_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_minMag_1M_0-c.csv")
add_test ( NAME test_float32_fmsf_rm_minMag-d COMMAND $<TARGET_FILE:test_float32_fmsf_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_minMag_1M_0-d.csv")
add_test ( NAME test_float32_fmsf_rm_minMag-e COMMAND $<TARGET_FILE:test_float32_fmsf_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_minMag_1M_0-e.csv")
add_test ( NAME test_float32_fmsf_rm_minMag-f COMMAND $<TARGET_FILE:test_float32_fmsf_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_minMag_1M_0-f.csv")
add_test ( NAME test_float32_fmsf_rm_minMag-g COMMAND $<TARGET_FILE:test_float32_fmsf_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_minMag_1M_0-g.csv")
add_test ( NAME test_float32_fmsf_rm_minMag-h COMMAND $<TARGET_FILE:test_float32_fmsf_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_minMag_1M_0-h.csv")
add_test ( NAME test_float32_fmsf_rm_minMag-i COMMAND $<TARGET_FILE:test_float32_fmsf_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_minMag_1M_0-i.csv")
add_test ( NAME test_float32_fmsf_rm_minMag-j COMMAND $<TARGET_FILE:test_float32_fmsf_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_minMag_1M_0-j.csv")
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_minMag-a coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_minMag-b coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_minMag-c coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_minMag-d coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_minMag-e coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_minMag-f coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_minMag-g coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_minMag-h coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_minMag-i coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_minMag-j coverage)
endif ( )

add_executable ( test_float32_fmsf_rm_near_even test_float32_fmsf_rm_near_even.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries (test_float32_fmsf_rm_near_even
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_fmsf_rm_near_even-a COMMAND $<TARGET_FILE:test_float32_fmsf_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_near_even_1M_0-a.csv")
add_test ( NAME test_float32_fmsf_rm_near_even-b COMMAND $<TARGET_FILE:test_float32_fmsf_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_near_even_1M_0-b.csv")
add_test ( NAME test_float32_fmsf_rm_near_even-c COMMAND $<TARGET_FILE:test_float32_fmsf_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_near_even_1M_0-c.csv")
add_test ( NAME test_float32_fmsf_rm_near_even-d COMMAND $<TARGET_FILE:test_float32_fmsf_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_near_even_1M_0-d.csv")
add_test ( NAME test_float32_fmsf_rm_near_even-e COMMAND $<TARGET_FILE:test_float32_fmsf_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_near_even_1M_0-e.csv")
add_test ( NAME test_float32_fmsf_rm_near_even-f COMMAND $<TARGET_FILE:test_float32_fmsf_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_near_even_1M_0-f.csv")
add_test ( NAME test_float32_fmsf_rm_near_even-g COMMAND $<TARGET_FILE:test_float32_fmsf_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_near_even_1M_0-g.csv")
add_test ( NAME test_float32_fmsf_rm_near_even-h COMMAND $<TARGET_FILE:test_float32_fmsf_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_near_even_1M_0-h.csv")
add_test ( NAME test_float32_fmsf_rm_near_even-i COMMAND $<TARGET_FILE:test_float32_fmsf_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_near_even_1M_0-i.csv")
add_test ( NAME test_float32_fmsf_rm_near_even-j COMMAND $<TARGET_FILE:test_float32_fmsf_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fmsf_near_even_1M_0-j.csv")
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_near_even-a coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_near_even-b coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_near_even-c coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_near_even-d coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_near_even-e coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_near_even-f coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_near_even-g coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_near_even-h coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_near_even-i coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fmsf_rm_near_even-j coverage)
endif ( )

# TEST FLOAT32_FNMA
add_executable ( test_float32_fnma test_float32_fnma.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_fnma
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_fnma-a COMMAND $<TARGET_FILE:test_float32_fnma> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fnmaf_s-a.csv")
add_test ( NAME test_float32_fnma-b COMMAND $<TARGET_FILE:test_float32_fnma> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fnmaf_s-b.csv")
add_test ( NAME test_float32_fnma-c COMMAND $<TARGET_FILE:test_float32_fnma> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fnmaf_s-c.csv")
add_test ( NAME test_float32_fnma-d COMMAND $<TARGET_FILE:test_float32_fnma> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fnmaf_s-d.csv")
add_test ( NAME test_float32_fnma-e COMMAND $<TARGET_FILE:test_float32_fnma> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fnmaf_s-e.csv")
add_test ( NAME test_float32_fnma-f COMMAND $<TARGET_FILE:test_float32_fnma> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fnmaf_s-f.csv")
add_test ( NAME test_float32_fnma-g COMMAND $<TARGET_FILE:test_float32_fnma> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fnmaf_s-g.csv")
add_test ( NAME test_float32_fnma-h COMMAND $<TARGET_FILE:test_float32_fnma> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fnmaf_s-h.csv")
add_test ( NAME test_float32_fnma-i COMMAND $<TARGET_FILE:test_float32_fnma> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fnmaf_s-i.csv")
add_test ( NAME test_float32_fnma-j COMMAND $<TARGET_FILE:test_float32_fnma> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fnmaf_s-j.csv")
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma-a coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma-b coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma-c coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma-d coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma-e coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma-f coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma-g coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma-h coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma-i coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma-j coverage)
endif ( )

add_executable ( test_float32_fnma_RTG_rm_max test_float32_fnma_RTG_rm_max.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries (test_float32_fnma_RTG_rm_max
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_fnma_RTG_rm_max-a COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_max_1M_0-a.csv")
add_test ( NAME test_float32_fnma_RTG_rm_max-b COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_max_1M_0-b.csv")
add_test ( NAME test_float32_fnma_RTG_rm_max-c COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_max_1M_0-c.csv")
add_test ( NAME test_float32_fnma_RTG_rm_max-d COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_max_1M_0-d.csv")
add_test ( NAME test_float32_fnma_RTG_rm_max-e COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_max_1M_0-e.csv")
add_test ( NAME test_float32_fnma_RTG_rm_max-f COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_max_1M_0-f.csv")
add_test ( NAME test_float32_fnma_RTG_rm_max-g COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_max_1M_0-g.csv")
add_test ( NAME test_float32_fnma_RTG_rm_max-h COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_max_1M_0-h.csv")
add_test ( NAME test_float32_fnma_RTG_rm_max-i COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_max_1M_0-i.csv")
add_test ( NAME test_float32_fnma_RTG_rm_max-j COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_max_1M_0-j.csv")
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_max-a coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_max-b coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_max-c coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_max-d coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_max-e coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_max-f coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_max-g coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_max-h coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_max-i coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_max-j coverage)
endif ( )

add_executable ( test_float32_fnma_RTG_rm_min test_float32_fnma_RTG_rm_min.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries (test_float32_fnma_RTG_rm_min
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_fnma_RTG_rm_min-a COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_min_1M_0-a.csv")
add_test ( NAME test_float32_fnma_RTG_rm_min-b COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_min_1M_0-b.csv")
add_test ( NAME test_float32_fnma_RTG_rm_min-c COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_min_1M_0-c.csv")
add_test ( NAME test_float32_fnma_RTG_rm_min-d COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_min_1M_0-d.csv")
add_test ( NAME test_float32_fnma_RTG_rm_min-e COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_min_1M_0-e.csv")
add_test ( NAME test_float32_fnma_RTG_rm_min-f COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_min_1M_0-f.csv")
add_test ( NAME test_float32_fnma_RTG_rm_min-g COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_min_1M_0-g.csv")
add_test ( NAME test_float32_fnma_RTG_rm_min-h COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_min_1M_0-h.csv")
add_test ( NAME test_float32_fnma_RTG_rm_min-i COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_min_1M_0-i.csv")
add_test ( NAME test_float32_fnma_RTG_rm_min-j COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_min_1M_0-j.csv")
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_min-a coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_min-b coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_min-c coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_min-d coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_min-e coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_min-f coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_min-g coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_min-h coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_min-i coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_min-j coverage)
endif ( )

add_executable ( test_float32_fnma_RTG_rm_minMag test_float32_fnma_RTG_rm_minMag.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries (test_float32_fnma_RTG_rm_minMag
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_fnma_RTG_rm_minMag-a COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_minMag_1M_0-a.csv")
add_test ( NAME test_float32_fnma_RTG_rm_minMag-b COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_minMag_1M_0-b.csv")
add_test ( NAME test_float32_fnma_RTG_rm_minMag-c COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_minMag_1M_0-c.csv")
add_test ( NAME test_float32_fnma_RTG_rm_minMag-d COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_minMag_1M_0-d.csv")
add_test ( NAME test_float32_fnma_RTG_rm_minMag-e COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_minMag_1M_0-e.csv")
add_test ( NAME test_float32_fnma_RTG_rm_minMag-f COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_minMag_1M_0-f.csv")
add_test ( NAME test_float32_fnma_RTG_rm_minMag-g COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_minMag_1M_0-g.csv")
add_test ( NAME test_float32_fnma_RTG_rm_minMag-h COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_minMag_1M_0-h.csv")
add_test ( NAME test_float32_fnma_RTG_rm_minMag-i COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_minMag_1M_0-i.csv")
add_test ( NAME test_float32_fnma_RTG_rm_minMag-j COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_minMag_1M_0-j.csv")
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_minMag-a coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_minMag-b coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_minMag-c coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_minMag-d coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_minMag-e coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_minMag-f coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_minMag-g coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_minMag-h coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_minMag-i coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_minMag-j coverage)
endif ( )

add_executable ( test_float32_fnma_RTG_rm_near_even test_float32_fnma_RTG_rm_near_even.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries (test_float32_fnma_RTG_rm_near_even
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_fnma_RTG_rm_near_even-a COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_near_even_1M_0-a.csv")
add_test ( NAME test_float32_fnma_RTG_rm_near_even-b COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_near_even_1M_0-b.csv")
add_test ( NAME test_float32_fnma_RTG_rm_near_even-c COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_near_even_1M_0-c.csv")
add_test ( NAME test_float32_fnma_RTG_rm_near_even-d COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_near_even_1M_0-d.csv")
add_test ( NAME test_float32_fnma_RTG_rm_near_even-e COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_near_even_1M_0-e.csv")
add_test ( NAME test_float32_fnma_RTG_rm_near_even-f COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_near_even_1M_0-f.csv")
add_test ( NAME test_float32_fnma_RTG_rm_near_even-g COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_near_even_1M_0-g.csv")
add_test ( NAME test_float32_fnma_RTG_rm_near_even-h COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_near_even_1M_0-h.csv")
add_test ( NAME test_float32_fnma_RTG_rm_near_even-i COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_near_even_1M_0-i.csv")
add_test ( NAME test_float32_fnma_RTG_rm_near_even-j COMMAND $<TARGET_FILE:test_float32_fnma_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmafs_near_even_1M_0-j.csv")
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_near_even-a coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_near_even-b coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_near_even-c coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_near_even-d coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_near_even-e coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_near_even-f coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_near_even-g coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_near_even-h coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_near_even-i coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnma_RTG_rm_near_even-j coverage)
endif ( )

# TEST FLOAT32_FNMSF
add_executable ( test_float32_fnmsf test_float32_fnmsf.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries ( test_float32_fnmsf
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_fnmsf-a COMMAND $<TARGET_FILE:test_float32_fnmsf> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fnmsf_s-a.csv")
add_test ( NAME test_float32_fnmsf-b COMMAND $<TARGET_FILE:test_float32_fnmsf> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fnmsf_s-b.csv")
add_test ( NAME test_float32_fnmsf-c COMMAND $<TARGET_FILE:test_float32_fnmsf> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fnmsf_s-c.csv")
add_test ( NAME test_float32_fnmsf-d COMMAND $<TARGET_FILE:test_float32_fnmsf> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fnmsf_s-d.csv")
add_test ( NAME test_float32_fnmsf-e COMMAND $<TARGET_FILE:test_float32_fnmsf> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fnmsf_s-e.csv")
add_test ( NAME test_float32_fnmsf-f COMMAND $<TARGET_FILE:test_float32_fnmsf> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fnmsf_s-f.csv")
add_test ( NAME test_float32_fnmsf-g COMMAND $<TARGET_FILE:test_float32_fnmsf> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fnmsf_s-g.csv")
add_test ( NAME test_float32_fnmsf-h COMMAND $<TARGET_FILE:test_float32_fnmsf> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fnmsf_s-h.csv")
add_test ( NAME test_float32_fnmsf-i COMMAND $<TARGET_FILE:test_float32_fnmsf> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fnmsf_s-i.csv")
add_test ( NAME test_float32_fnmsf-j COMMAND $<TARGET_FILE:test_float32_fnmsf> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_fnmsf_s-j.csv")
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf-a coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf-b coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf-c coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf-d coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf-e coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf-f coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf-g coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf-h coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf-i coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf-j coverage)
endif ( )

add_executable ( test_float32_fnmsf_RTG_rm_max test_float32_fnmsf_RTG_rm_max.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries (test_float32_fnmsf_RTG_rm_max
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_fnmsf_RTG_rm_max-a COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_max_1M_0-a.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_max-b COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_max_1M_0-b.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_max-c COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_max_1M_0-c.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_max-d COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_max_1M_0-d.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_max-e COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_max_1M_0-e.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_max-f COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_max_1M_0-f.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_max-g COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_max_1M_0-g.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_max-h COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_max_1M_0-h.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_max-i COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_max_1M_0-i.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_max-j COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_max> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_max_1M_0-j.csv")
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_max-a coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_max-b coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_max-c coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_max-d coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_max-e coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_max-f coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_max-g coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_max-h coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_max-i coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_max-j coverage)
endif ( )

add_executable ( test_float32_fnmsf_RTG_rm_min test_float32_fnmsf_RTG_rm_min.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries (test_float32_fnmsf_RTG_rm_min
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_fnmsf_RTG_rm_min-a COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_min_1M_0-a.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_min-b COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_min_1M_0-b.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_min-c COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_min_1M_0-c.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_min-d COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_min_1M_0-d.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_min-e COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_min_1M_0-e.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_min-f COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_min_1M_0-f.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_min-g COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_min_1M_0-g.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_min-h COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_min_1M_0-h.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_min-i COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_min_1M_0-i.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_min-j COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_min> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_min_1M_0-j.csv")
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_min-a coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_min-b coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_min-c coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_min-d coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_min-e coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_min-f coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_min-g coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_min-h coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_min-i coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_min-j coverage)
endif ( )

add_executable ( test_float32_fnmsf_RTG_rm_minMag test_float32_fnmsf_RTG_rm_minMag.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries (test_float32_fnmsf_RTG_rm_minMag
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_fnmsf_RTG_rm_minMag-a COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_minMag_1M_0-a.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_minMag-b COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_minMag_1M_0-b.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_minMag-c COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_minMag_1M_0-c.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_minMag-d COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_minMag_1M_0-d.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_minMag-e COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_minMag_1M_0-e.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_minMag-f COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_minMag_1M_0-f.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_minMag-g COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_minMag_1M_0-g.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_minMag-h COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_minMag_1M_0-h.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_minMag-i COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_minMag_1M_0-i.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_minMag-j COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_minMag> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_minMag_1M_0-j.csv")
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_minMag-a coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_minMag-b coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_minMag-c coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_minMag-d coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_minMag-e coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_minMag-f coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_minMag-g coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_minMag-h coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_minMag-i coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_minMag-j coverage)
endif ( )

add_executable ( test_float32_fnmsf_RTG_rm_near_even test_float32_fnmsf_RTG_rm_near_even.cpp  $<${useOBJ}:$<TARGET_OBJECTS:softfloat>> )
target_link_libraries (test_float32_fnmsf_RTG_rm_near_even
  libinst
  $<${useLIB}:softfloat>
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
  )
add_test ( NAME test_float32_fnmsf_RTG_rm_near_even-a COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_near_even_1M_0-a.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_near_even-b COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_near_even_1M_0-b.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_near_even-c COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_near_even_1M_0-c.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_near_even-d COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_near_even_1M_0-d.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_near_even-e COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_near_even_1M_0-e.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_near_even-f COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_near_even_1M_0-f.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_near_even-g COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_near_even_1M_0-g.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_near_even-h COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_near_even_1M_0-h.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_near_even-i COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_near_even_1M_0-i.csv")
add_test ( NAME test_float32_fnmsf_RTG_rm_near_even-j COMMAND $<TARGET_FILE:test_float32_fnmsf_RTG_rm_near_even> ${CMAKE_CURRENT_SOURCE_DIR} "data/UT_RTG_fnmsfs_near_even_1M_0-j.csv")
if ( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_near_even-a coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_near_even-b coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_near_even-c coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_near_even-d coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_near_even-e coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_near_even-f coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_near_even-g coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_near_even-h coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_near_even-i coverage)
  setup_target_for_coverage ( ${PROJECT_NAME}_coverage test_float32_fnmsf_RTG_rm_near_even-j coverage)
endif ( )

# run with: ctest -L FPU -LE FULL
set_property (
  TEST test_SoftFloatAdapter test_check_output_subnormal test_float32_madd test_float32_msub
  PROPERTY LABELS FPU )

# run with: ctest -L FPU
set_property (
  TEST
  test_float32_madd_full_rm_max test_float32_madd_full_rm_min
  test_float32_madd_full_rm_minmag test_float32_madd_full_rm_neareven
  test_float_cvt_sw test_float_round_sw test_float_trnc_sw test_float_floor_sw
  test_float_ceil_sw test_float_cvt_suw test_float_round_suw test_float_ceil_suw
  test_float_floor_suw test_float_trnc_suw test_float_cvt_sl test_float_round_sl
  test_float_ceil_sl test_float_floor_sl test_float_trnc_sl test_float_cvt_sul
  test_float_round_sul test_float_ceil_sul test_float_floor_sul test_float_trnc_sul
  test_float_cvt_dw test_float_round_dw test_float_trnc_dw test_float_ceil_dw
  test_float_floor_dw test_float_cvt_duw test_float_round_duw test_float_round_duw
  test_float_trnc_duw test_float_ceil_duw test_float_floor_duw test_float_cvt_dl
  test_float_round_dl test_float_trnc_dl test_float_ceil_dl test_float_floor_dl
  test_float_cvt_dul test_float_round_dul test_float_trnc_dul test_float_ceil_dul
  test_float_floor_dul test_float_cvt_ws test_float_cvt_uws test_float_cvt_ls
PROPERTY LABELS FULL_FPU_0 )

set_property (
  TEST
  test_float_cvt_uls test_float_cvt_wd test_float_cvt_uwd test_float_cvt_ld
  test_float_cvt_uld
  test_float_cvt_ds test_float_cvt_sd test_float_cvt_sh test_float_cvt_hs
  test_float32_rsqrt test_float64_rsqrt test_float32_recipfs test_float64_recip
  test_float32_minfs test_float64_min test_float32_maxfs test_float64_max
  test_float32_divfs test_float64_div test_float32_mul test_float64_mul
  test_float32_add test_float64_add test_float32_sub test_float64_sub
  test_float32_cmpfs test_float64_cmp test_float32_sqrt test_float64_sqrt
  test_float32_abs test_float64_abs test_float32_neg test_float64_neg
PROPERTY LABELS FULL_FPU_1 )

set_property (
  TEST
# test_float32_fma
  test_float32_fma-a
  test_float32_fma-b
  test_float32_fma-c
  test_float32_fma-d
  test_float32_fma-e
  test_float32_fma-f
  test_float32_fma-g
  test_float32_fma-h
  test_float32_fma-i
  test_float32_fma-j

# test_float32_fma_RTG_rm_max
  test_float32_fma_RTG_rm_max-a
  test_float32_fma_RTG_rm_max-b
  test_float32_fma_RTG_rm_max-c
  test_float32_fma_RTG_rm_max-d
  test_float32_fma_RTG_rm_max-e
  test_float32_fma_RTG_rm_max-f
  test_float32_fma_RTG_rm_max-g
  test_float32_fma_RTG_rm_max-h
  test_float32_fma_RTG_rm_max-i
  test_float32_fma_RTG_rm_max-j

# test_float32_fma_RTG_rm_min
  test_float32_fma_RTG_rm_min-a
  test_float32_fma_RTG_rm_min-b
  test_float32_fma_RTG_rm_min-c
  test_float32_fma_RTG_rm_min-d
  test_float32_fma_RTG_rm_min-e
  test_float32_fma_RTG_rm_min-f
  test_float32_fma_RTG_rm_min-g
  test_float32_fma_RTG_rm_min-h
  test_float32_fma_RTG_rm_min-i
  test_float32_fma_RTG_rm_min-j

# test_float32_fma_RTG_rm_minMag
  test_float32_fma_RTG_rm_minMag-a
  test_float32_fma_RTG_rm_minMag-b
  test_float32_fma_RTG_rm_minMag-c
  test_float32_fma_RTG_rm_minMag-d
  test_float32_fma_RTG_rm_minMag-e
  test_float32_fma_RTG_rm_minMag-f
  test_float32_fma_RTG_rm_minMag-g
  test_float32_fma_RTG_rm_minMag-h
  test_float32_fma_RTG_rm_minMag-i
  test_float32_fma_RTG_rm_minMag-j
PROPERTY LABELS FULL_FPU_2 )

set_property (
  TEST
# test_float32_fma_RTG_rm_near_even
  test_float32_fma_RTG_rm_near_even-a
  test_float32_fma_RTG_rm_near_even-b
  test_float32_fma_RTG_rm_near_even-c
  test_float32_fma_RTG_rm_near_even-d
  test_float32_fma_RTG_rm_near_even-e
  test_float32_fma_RTG_rm_near_even-f
  test_float32_fma_RTG_rm_near_even-g
  test_float32_fma_RTG_rm_near_even-h
  test_float32_fma_RTG_rm_near_even-i
  test_float32_fma_RTG_rm_near_even-j

# test_float32_fmsf
  test_float32_fmsf-a
  test_float32_fmsf-b
  test_float32_fmsf-c
  test_float32_fmsf-d
  test_float32_fmsf-e
  test_float32_fmsf-f
  test_float32_fmsf-g
  test_float32_fmsf-h
  test_float32_fmsf-i
  test_float32_fmsf-j

# test_float32_fmsf_rm_max
  test_float32_fmsf_rm_max-a
  test_float32_fmsf_rm_max-b
  test_float32_fmsf_rm_max-c
  test_float32_fmsf_rm_max-d
  test_float32_fmsf_rm_max-e
  test_float32_fmsf_rm_max-f
  test_float32_fmsf_rm_max-g
  test_float32_fmsf_rm_max-h
  test_float32_fmsf_rm_max-i
  test_float32_fmsf_rm_max-j

# test_float32_fmsf_rm_min
  test_float32_fmsf_rm_min-a
  test_float32_fmsf_rm_min-b
  test_float32_fmsf_rm_min-c
  test_float32_fmsf_rm_min-d
  test_float32_fmsf_rm_min-e
  test_float32_fmsf_rm_min-f
  test_float32_fmsf_rm_min-g
  test_float32_fmsf_rm_min-h
  test_float32_fmsf_rm_min-i
  test_float32_fmsf_rm_min-j
PROPERTY LABELS FULL_FPU_3 )

set_property (
  TEST
# test_float32_fmsf_rm_minMag
  test_float32_fmsf_rm_minMag-a
  test_float32_fmsf_rm_minMag-b
  test_float32_fmsf_rm_minMag-c
  test_float32_fmsf_rm_minMag-d
  test_float32_fmsf_rm_minMag-e
  test_float32_fmsf_rm_minMag-f
  test_float32_fmsf_rm_minMag-g
  test_float32_fmsf_rm_minMag-h
  test_float32_fmsf_rm_minMag-i
  test_float32_fmsf_rm_minMag-j

# test_float32_fmsf_rm_near_even
  test_float32_fmsf_rm_near_even-a
  test_float32_fmsf_rm_near_even-b
  test_float32_fmsf_rm_near_even-c
  test_float32_fmsf_rm_near_even-d
  test_float32_fmsf_rm_near_even-e
  test_float32_fmsf_rm_near_even-f
  test_float32_fmsf_rm_near_even-g
  test_float32_fmsf_rm_near_even-h
  test_float32_fmsf_rm_near_even-i
  test_float32_fmsf_rm_near_even-j
PROPERTY LABELS FULL_FPU_4 )

set_property (
  TEST
# test_float32_fnma
  test_float32_fnma-a
  test_float32_fnma-b
  test_float32_fnma-c
  test_float32_fnma-d
  test_float32_fnma-e
  test_float32_fnma-f
  test_float32_fnma-g
  test_float32_fnma-h
  test_float32_fnma-i
  test_float32_fnma-j

# test_float32_fnma_RTG_rm_max
  test_float32_fnma_RTG_rm_max-a
  test_float32_fnma_RTG_rm_max-b
  test_float32_fnma_RTG_rm_max-c
  test_float32_fnma_RTG_rm_max-d
  test_float32_fnma_RTG_rm_max-e
  test_float32_fnma_RTG_rm_max-f
  test_float32_fnma_RTG_rm_max-g
  test_float32_fnma_RTG_rm_max-h
  test_float32_fnma_RTG_rm_max-i
  test_float32_fnma_RTG_rm_max-j
PROPERTY LABELS FULL_FPU_5 )

set_property (
  TEST
# test_float32_fnma_RTG_rm_min
  test_float32_fnma_RTG_rm_min-a
  test_float32_fnma_RTG_rm_min-b
  test_float32_fnma_RTG_rm_min-c
  test_float32_fnma_RTG_rm_min-d
  test_float32_fnma_RTG_rm_min-e
  test_float32_fnma_RTG_rm_min-f
  test_float32_fnma_RTG_rm_min-g
  test_float32_fnma_RTG_rm_min-h
  test_float32_fnma_RTG_rm_min-i
  test_float32_fnma_RTG_rm_min-j

# test_float32_fnma_RTG_rm_minMag
  test_float32_fnma_RTG_rm_minMag-a
  test_float32_fnma_RTG_rm_minMag-b
  test_float32_fnma_RTG_rm_minMag-c
  test_float32_fnma_RTG_rm_minMag-d
  test_float32_fnma_RTG_rm_minMag-e
  test_float32_fnma_RTG_rm_minMag-f
  test_float32_fnma_RTG_rm_minMag-g
  test_float32_fnma_RTG_rm_minMag-h
  test_float32_fnma_RTG_rm_minMag-i
  test_float32_fnma_RTG_rm_minMag-j
PROPERTY LABELS FULL_FPU_6 )

set_property (
  TEST
# test_float32_fnma_RTG_rm_near_even
  test_float32_fnma_RTG_rm_near_even-a
  test_float32_fnma_RTG_rm_near_even-b
  test_float32_fnma_RTG_rm_near_even-c
  test_float32_fnma_RTG_rm_near_even-d
  test_float32_fnma_RTG_rm_near_even-e
  test_float32_fnma_RTG_rm_near_even-f
  test_float32_fnma_RTG_rm_near_even-g
  test_float32_fnma_RTG_rm_near_even-h
  test_float32_fnma_RTG_rm_near_even-i
  test_float32_fnma_RTG_rm_near_even-j

# test_float32_fnmsf
  test_float32_fnmsf-a
  test_float32_fnmsf-b
  test_float32_fnmsf-c
  test_float32_fnmsf-d
  test_float32_fnmsf-e
  test_float32_fnmsf-f
  test_float32_fnmsf-g
  test_float32_fnmsf-h
  test_float32_fnmsf-i
  test_float32_fnmsf-j
PROPERTY LABELS FULL_FPU_7 )

set_property (
  TEST
# test_float32_fnmsf_RTG_rm_max
  test_float32_fnmsf_RTG_rm_max-a
  test_float32_fnmsf_RTG_rm_max-b
  test_float32_fnmsf_RTG_rm_max-c
  test_float32_fnmsf_RTG_rm_max-d
  test_float32_fnmsf_RTG_rm_max-e
  test_float32_fnmsf_RTG_rm_max-f
  test_float32_fnmsf_RTG_rm_max-g
  test_float32_fnmsf_RTG_rm_max-h
  test_float32_fnmsf_RTG_rm_max-i
  test_float32_fnmsf_RTG_rm_max-j

# test_float32_fnmsf_RTG_rm_min
  test_float32_fnmsf_RTG_rm_min-a
  test_float32_fnmsf_RTG_rm_min-b
  test_float32_fnmsf_RTG_rm_min-c
  test_float32_fnmsf_RTG_rm_min-d
  test_float32_fnmsf_RTG_rm_min-e
  test_float32_fnmsf_RTG_rm_min-f
  test_float32_fnmsf_RTG_rm_min-g
  test_float32_fnmsf_RTG_rm_min-h
  test_float32_fnmsf_RTG_rm_min-i
  test_float32_fnmsf_RTG_rm_min-j

# test_float32_fnmsf_RTG_rm_minMag
  test_float32_fnmsf_RTG_rm_minMag-a
  test_float32_fnmsf_RTG_rm_minMag-b
  test_float32_fnmsf_RTG_rm_minMag-c
  test_float32_fnmsf_RTG_rm_minMag-d
  test_float32_fnmsf_RTG_rm_minMag-e
  test_float32_fnmsf_RTG_rm_minMag-f
  test_float32_fnmsf_RTG_rm_minMag-g
  test_float32_fnmsf_RTG_rm_minMag-h
  test_float32_fnmsf_RTG_rm_minMag-i
  test_float32_fnmsf_RTG_rm_minMag-j

# test_float32_fnmsf_RTG_rm_near_even
  test_float32_fnmsf_RTG_rm_near_even-a
  test_float32_fnmsf_RTG_rm_near_even-b
  test_float32_fnmsf_RTG_rm_near_even-c
  test_float32_fnmsf_RTG_rm_near_even-d
  test_float32_fnmsf_RTG_rm_near_even-e
  test_float32_fnmsf_RTG_rm_near_even-f
  test_float32_fnmsf_RTG_rm_near_even-g
  test_float32_fnmsf_RTG_rm_near_even-h
  test_float32_fnmsf_RTG_rm_near_even-i
  test_float32_fnmsf_RTG_rm_near_even-j
PROPERTY LABELS FULL_FPU_8 )
