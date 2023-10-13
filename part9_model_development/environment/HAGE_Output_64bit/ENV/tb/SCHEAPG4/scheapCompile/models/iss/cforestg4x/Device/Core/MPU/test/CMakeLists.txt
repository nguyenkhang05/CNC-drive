cmake_minimum_required( VERSION 3.2 )

# set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -H" )

include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_BINARY_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_BINARY_DIR}/Device/Core/MICROARCHDB/src
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_BINARY_DIR}/Device/Core/REG/test
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/test
  ${TOP_SOURCE_DIR}/Device/Utility/include

  ${TOP_SOURCE_DIR}/Device/Core/PE/test
  ${TOP_SOURCE_DIR}/Device/Core/PE/include # for sreg.h

  ${gmock_SOURCE_DIR}/include
  ${gtest_SOURCE_DIR}/include
  )

add_library(libmpu
  ${TOP_SOURCE_DIR}/Device/Core/MPU/src/mpu.cpp
  ${TOP_SOURCE_DIR}/Device/Core/MICROARCHDB/src/micro_arch_db.cpp
  ${DEVICE_CORE_MICROARCHDB_BINARY_DIR}/src/cedar_arch_info.cpp
  ${TOP_SOURCE_DIR}/Device/Core/REG/src/sregfile.cpp
)
set_source_files_properties(
  ${DEVICE_CORE_MICROARCHDB_BINARY_DIR}/src/cedar_arch_info.cpp
  PROPERTIES GENERATED TRUE
)

add_dependencies(libmpu
  gen_cedar_arch_info
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
)

if( MSVC )
  set( hasPTHREAD 0 )
else( )
  set( hasPTHREAD 1 )
endif( )

# TEST MPU_DELMPUREGION
add_executable( test_mpu_delmpuregion test_mpu_delmpuregion.cpp )
target_link_libraries( test_mpu_delmpuregion
  libmpu
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_mpu_delmpuregion COMMAND $<TARGET_FILE:test_mpu_delmpuregion> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_mpu_delmpuregion coverage)
endif( )

# TEST MPU_ADDMPUREGION
add_executable( test_mpu_addmpuregion test_mpu_addmpuregion.cpp )
target_link_libraries( test_mpu_addmpuregion
  libmpu
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_mpu_addmpuregion COMMAND $<TARGET_FILE:test_mpu_addmpuregion> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_mpu_addmpuregion coverage)
endif( )

# TEST MPU_UpdatePermitSpid
add_executable( test_mpu_UpdatePermitSpid test_mpu_UpdatePermitSpid.cpp )
target_link_libraries( test_mpu_UpdatePermitSpid
  libmpu
  gmock gtest pthread
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_mpu_UpdatePermitSpid COMMAND $<TARGET_FILE:test_mpu_UpdatePermitSpid> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_mpu_UpdatePermitSpid coverage)
endif( )

# TEST MPU_CHECKMPUSETTING
add_executable( test_mpu_checkmpusetting_ov_set test_mpu_checkmpusetting_ov_set.cpp )
target_link_libraries( test_mpu_checkmpusetting_ov_set
  libmpu
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_mpu_checkmpusetting_ov_set COMMAND $<TARGET_FILE:test_mpu_checkmpusetting_ov_set> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_mpu_checkmpusetting_ov_set coverage)
endif( )
add_executable( test_mpu_checkmpusetting_ov_is_zero test_mpu_checkmpusetting_ov_is_zero.cpp )
target_link_libraries( test_mpu_checkmpusetting_ov_is_zero
  libmpu
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_mpu_checkmpusetting_ov_is_zero COMMAND $<TARGET_FILE:test_mpu_checkmpusetting_ov_is_zero> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_mpu_checkmpusetting_ov_is_zero coverage)
endif( )

# TEST MPU_ISMPUREGIONHIT
add_executable( test_mpu_ismpuregionhit test_mpu_ismpuregionhit.cpp )
target_link_libraries( test_mpu_ismpuregionhit
  libmpu
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_mpu_ismpuregionhit COMMAND $<TARGET_FILE:test_mpu_ismpuregionhit> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_mpu_ismpuregionhit coverage)
endif( )

# TEST MPU_REGIONPERMITTED
add_executable( test_mpu_ismpuregionpermitted test_mpu_ismpuregionpermitted.cpp )
target_link_libraries( test_mpu_ismpuregionpermitted
  libmpu
  gmock gtest pthread
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_mpu_ismpuregionpermitted COMMAND $<TARGET_FILE:test_mpu_ismpuregionpermitted> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_mpu_ismpuregionpermitted coverage)
endif( )

## TEST MPU_ISMPUREGIONMISSALIGN
#add_executable( test_mpu_ismpuregionmissalign test_mpu_ismpuregionmissalign.cpp )
#target_link_libraries( test_mpu_ismpuregionmissalign
#  libmpu
#  gmock gtest pthread
#  $<$<BOOL:${CODE_COVERAGE}>:gcov>
#)
#add_test( NAME test_mpu_ismpuregionmissalign COMMAND $<TARGET_FILE:test_mpu_ismpuregionmissalign> )
#if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
#  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_mpu_ismpuregionmissalign coverage)
#endif( )

# TEST MPU_ISMPUPERMITTEDDEFNG_INST
add_executable( test_mpu_ismpupermitteddefng_inst1 test_mpu_ismpupermitteddefng_inst1.cpp )
target_link_libraries( test_mpu_ismpupermitteddefng_inst1
  libmpu
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_mpu_ismpupermitteddefng_inst1 COMMAND $<TARGET_FILE:test_mpu_ismpupermitteddefng_inst1> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_mpu_ismpupermitteddefng_inst1 coverage)
endif( )

add_executable( test_mpu_ismpupermitteddefng_inst2 test_mpu_ismpupermitteddefng_inst2.cpp )
target_link_libraries( test_mpu_ismpupermitteddefng_inst2
  libmpu
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_mpu_ismpupermitteddefng_inst2 COMMAND $<TARGET_FILE:test_mpu_ismpupermitteddefng_inst2> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_mpu_ismpupermitteddefng_inst2 coverage)
endif( )

add_executable( test_mpu_ismpupermitteddefng_inst3 test_mpu_ismpupermitteddefng_inst3.cpp )
target_link_libraries( test_mpu_ismpupermitteddefng_inst3
  libmpu
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_mpu_ismpupermitteddefng_inst3 COMMAND $<TARGET_FILE:test_mpu_ismpupermitteddefng_inst3> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_mpu_ismpupermitteddefng_inst3 coverage)
endif( )

# TEST MPU_ISMPUPERMITTEDDEFNG_DATA
add_executable( test_mpu_ismpupermitteddefng_data_read1 test_mpu_ismpupermitteddefng_data_read1.cpp )
target_link_libraries( test_mpu_ismpupermitteddefng_data_read1
  libmpu
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_mpu_ismpupermitteddefng_data_read1 COMMAND $<TARGET_FILE:test_mpu_ismpupermitteddefng_data_read1> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_mpu_ismpupermitteddefng_data_read1 coverage)
endif( )

add_executable( test_mpu_ismpupermitteddefng_data_read2 test_mpu_ismpupermitteddefng_data_read2.cpp )
target_link_libraries( test_mpu_ismpupermitteddefng_data_read2
  libmpu
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_mpu_ismpupermitteddefng_data_read2 COMMAND $<TARGET_FILE:test_mpu_ismpupermitteddefng_data_read2> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_mpu_ismpupermitteddefng_data_read2 coverage)
endif( )

add_executable( test_mpu_ismpupermitteddefng_data_read3 test_mpu_ismpupermitteddefng_data_read3.cpp )
target_link_libraries( test_mpu_ismpupermitteddefng_data_read3
  libmpu
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_mpu_ismpupermitteddefng_data_read3 COMMAND $<TARGET_FILE:test_mpu_ismpupermitteddefng_data_read3> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_mpu_ismpupermitteddefng_data_read3 coverage)
endif( )

add_executable( test_mpu_ismpupermitteddefng_data_write1 test_mpu_ismpupermitteddefng_data_write1.cpp )
target_link_libraries( test_mpu_ismpupermitteddefng_data_write1
  libmpu
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_mpu_ismpupermitteddefng_data_write1 COMMAND $<TARGET_FILE:test_mpu_ismpupermitteddefng_data_write1> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_mpu_ismpupermitteddefng_data_write1 coverage)
endif( )

add_executable( test_mpu_ismpupermitteddefng_data_write2 test_mpu_ismpupermitteddefng_data_write2.cpp )
target_link_libraries( test_mpu_ismpupermitteddefng_data_write2
  libmpu
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_mpu_ismpupermitteddefng_data_write2 COMMAND $<TARGET_FILE:test_mpu_ismpupermitteddefng_data_write2> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_mpu_ismpupermitteddefng_data_write2 coverage)
endif( )

add_executable( test_mpu_ismpupermitteddefng_data_write3 test_mpu_ismpupermitteddefng_data_write3.cpp )
target_link_libraries( test_mpu_ismpupermitteddefng_data_write3
  libmpu
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_mpu_ismpupermitteddefng_data_write3 COMMAND $<TARGET_FILE:test_mpu_ismpupermitteddefng_data_write3> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_mpu_ismpupermitteddefng_data_write3 coverage)
endif( )

# TEST MPU_ISMPUPERMITTED
add_executable( test_mpu_ismpupermitted test_mpu_ismpupermitted.cpp )
target_link_libraries( test_mpu_ismpupermitted
  libmpu
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_mpu_ismpupermitted COMMAND $<TARGET_FILE:test_mpu_ismpupermitted> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_mpu_ismpupermitted coverage)
endif( )

# TEST MPU_ISMPUPERMITTEDEI
add_executable( test_mpu_ismpupermittedei test_mpu_ismpupermittedei.cpp )
target_link_libraries( test_mpu_ismpupermittedei
  libmpu
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_mpu_ismpupermittedei COMMAND $<TARGET_FILE:test_mpu_ismpupermittedei> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_mpu_ismpupermittedei coverage)
endif( )

# TEST MPU_ISMPUPERMITTEDRMW
add_executable( test_mpu_ismpupermittedrmw test_mpu_ismpupermittedrmw.cpp )
target_link_libraries( test_mpu_ismpupermittedrmw
  libmpu
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_mpu_ismpupermittedrmw COMMAND $<TARGET_FILE:test_mpu_ismpupermittedrmw> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_mpu_ismpupermittedrmw coverage)
endif( )

# run with: ctest -L Core
set_property(
  TEST test_mpu_delmpuregion
  test_mpu_addmpuregion
  test_mpu_UpdatePermitSpid
  test_mpu_checkmpusetting_ov_set
  test_mpu_checkmpusetting_ov_is_zero
  test_mpu_ismpuregionhit
  test_mpu_ismpupermitteddefng_inst1
  test_mpu_ismpupermitteddefng_inst2
  test_mpu_ismpupermitteddefng_inst3
  test_mpu_ismpupermitteddefng_data_read1
  test_mpu_ismpupermitteddefng_data_read2
  test_mpu_ismpupermitteddefng_data_read3
  test_mpu_ismpupermitteddefng_data_write1
  test_mpu_ismpupermitteddefng_data_write2
  test_mpu_ismpupermitteddefng_data_write3
  test_mpu_ismpupermitted
  test_mpu_ismpupermittedei
  test_mpu_ismpupermittedrmw
  test_mpu_ismpuregionpermitted
  PROPERTY LABELS Core )
