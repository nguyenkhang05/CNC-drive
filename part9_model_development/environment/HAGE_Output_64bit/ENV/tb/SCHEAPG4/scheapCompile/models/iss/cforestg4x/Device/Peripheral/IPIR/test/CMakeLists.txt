cmake_minimum_required( VERSION 3.2 )

include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/test
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/IPIR/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/IPIR/test
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
  ${TOP_SOURCE_DIR}/Device/Utility/test
  ${gmock_SOURCE_DIR}/include
  ${gtest_SOURCE_DIR}/include
  )

add_library(libipir
  ${TOP_SOURCE_DIR}/Device/Peripheral/IPIR/src/ipir_const.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/src/treg.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/IPIR/src/treg_ipir.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/IPIR/src/ipir.cpp
)

add_dependencies( libipir
  gen_sysreg_enum_h
  gen_sysreg_file_h
  gen_sysreg_h
)

if( MSVC )
  set( hasPTHREAD 0 )
else( )
  set( hasPTHREAD 1 )
endif( )

# TEST TREGIPIEN
add_executable( test_ipir_tregipien test_ipir_tregipien.cpp
  )
target_link_libraries( test_ipir_tregipien
  libipir
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_ipir_tregipien COMMAND $<TARGET_FILE:test_ipir_tregipien> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_ipir_tregipien coverage)
endif( )

# TEST TREGIPIFLG
add_executable( test_ipir_tregipiflg test_ipir_tregipiflg.cpp
  )
target_link_libraries( test_ipir_tregipiflg
  libipir
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_ipir_tregipiflg COMMAND $<TARGET_FILE:test_ipir_tregipiflg> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_ipir_tregipiflg coverage)
endif( )

# TEST TREGIPIFCLR
add_executable( test_ipir_tregipifclr test_ipir_tregipifclr.cpp
  )
target_link_libraries( test_ipir_tregipifclr
  libipir
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_ipir_tregipifclr COMMAND $<TARGET_FILE:test_ipir_tregipifclr> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_ipir_tregipifclr coverage)
endif( )

# TEST TREGIPIREQ
add_executable( test_ipir_tregipireq test_ipir_tregipireq.cpp
  )
target_link_libraries( test_ipir_tregipireq
  libipir
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_ipir_tregipireq COMMAND $<TARGET_FILE:test_ipir_tregipireq> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_ipir_tregipireq coverage)
endif( )

# TEST TREGIPIRCLR
add_executable( test_ipir_tregipirclr test_ipir_tregipirclr.cpp
  )
target_link_libraries( test_ipir_tregipirclr
  libipir
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_ipir_tregipirclr COMMAND $<TARGET_FILE:test_ipir_tregipirclr> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_ipir_tregipirclr coverage)
endif( )

# TEST TREGIPIR
add_executable( test_ipir_tregipir test_ipir_tregipir.cpp
  )
target_link_libraries( test_ipir_tregipir
  libipir
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_ipir_tregipir COMMAND $<TARGET_FILE:test_ipir_tregipir> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_ipir_tregipir coverage)
endif( )


# TEST TREGIPIFCLR T.B.D.
# TEST TREGIPIFLG  T.B.D.
# TEST TREGIPIR    T.B.D.
# TEST TREGIPIRCLR T.B.D.
# TEST TREGIPIREQ  T.B.D.

# TEST IPIR
add_executable( test_ipir_cipir test_ipir_cipir.cpp
  )
target_link_libraries( test_ipir_cipir
  libipir
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_ipir_cipir COMMAND $<TARGET_FILE:test_ipir_cipir> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_ipir_cipir coverage)
endif( )

# run with: ctest -L Peripheral
set_property(
  TEST test_ipir_tregipien test_ipir_tregipiflg test_ipir_tregipireq test_ipir_tregipifclr test_ipir_tregipirclr  test_ipir_tregipir test_ipir_cipir
  PROPERTY LABELS Peripheral )
