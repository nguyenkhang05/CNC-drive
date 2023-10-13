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

add_library( libdbgbreak
  ${TOP_SOURCE_DIR}/Device/Core/DEBUGBREAK/src/dbg_break.cpp
  ${TOP_SOURCE_DIR}/Device/Core/REG/src/sregfile.cpp
)
add_dependencies( libdbgbreak 
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

# TEST DB_INITEORESERVED
add_executable( test_db_initeoreserved test_db_initeoreserved.cpp )
target_link_libraries( test_db_initeoreserved
  libdbgbreak
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_db_initeoreserved COMMAND $<TARGET_FILE:test_db_initeoreserved> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_db_initeoreserved coverage)
endif( )

# TEST DB_CLEARPCBEORESERVED
add_executable( test_db_clearpcbeoreserved test_db_clearpcbeoreserved.cpp )
target_link_libraries( test_db_clearpcbeoreserved
  libdbgbreak
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_db_clearpcbeoreserved COMMAND $<TARGET_FILE:test_db_clearpcbeoreserved> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_db_clearpcbeoreserved coverage)
endif( )

# TEST DB_APPLYPCBEORESERVED
add_executable( test_db_applypcbeoreserved test_db_applypcbeoreserved.cpp )
target_link_libraries( test_db_applypcbeoreserved
  libdbgbreak
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_db_applypcbeoreserved COMMAND $<TARGET_FILE:test_db_applypcbeoreserved> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_db_applypcbeoreserved coverage)
endif( )

# TEST DB_CLEARLSABEORESERVED
add_executable( test_db_clearlsabeoreserved test_db_clearlsabeoreserved.cpp )
target_link_libraries( test_db_clearlsabeoreserved
  libdbgbreak
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_db_clearlsabeoreserved COMMAND $<TARGET_FILE:test_db_clearlsabeoreserved> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_db_clearlsabeoreserved coverage)
endif( )

# TEST DB_APPLYLSABEORESERVED
add_executable( test_db_applylsabeoreserved test_db_applylsabeoreserved.cpp )
target_link_libraries( test_db_applylsabeoreserved
  libdbgbreak
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_db_applylsabeoreserved COMMAND $<TARGET_FILE:test_db_applylsabeoreserved> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_db_applylsabeoreserved coverage)
endif( )

# TEST DB_UPDATEBREAKPCBEXCP
add_executable( test_db_updatebreakpcbexcp test_db_updatebreakpcbexcp.cpp )
target_link_libraries( test_db_updatebreakpcbexcp
  libdbgbreak
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_db_updatebreakpcbexcp COMMAND $<TARGET_FILE:test_db_updatebreakpcbexcp> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_db_updatebreakpcbexcp coverage)
endif( )

# TEST DB_ISBRKADDRHIT
add_executable( test_db_isbrkaddrhit test_db_isbrkaddrhit.cpp )
target_link_libraries( test_db_isbrkaddrhit
  libdbgbreak
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_db_isbrkaddrhit COMMAND $<TARGET_FILE:test_db_isbrkaddrhit> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_db_isbrkaddrhit coverage)
endif( )

# TEST DB_ISBRKSIZEHIT
add_executable( test_db_isbrksizehit test_db_isbrksizehit.cpp )
target_link_libraries( test_db_isbrksizehit
  libdbgbreak
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_db_isbrksizehit COMMAND $<TARGET_FILE:test_db_isbrksizehit> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_db_isbrksizehit coverage)
endif( )

# TEST DB_ISDEBUGGABLEMODE
add_executable( test_db_isdebuggablemode test_db_isdebuggablemode.cpp )
target_link_libraries( test_db_isdebuggablemode
  libdbgbreak
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_db_isdebuggablemode COMMAND $<TARGET_FILE:test_db_isdebuggablemode> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_db_isdebuggablemode coverage)
endif( )

# run with: ctest -L Core
set_property(
  TEST test_db_initeoreserved
       test_db_clearpcbeoreserved
       test_db_applypcbeoreserved
       test_db_clearlsabeoreserved
       test_db_applylsabeoreserved
       test_db_updatebreakpcbexcp
       test_db_isbrkaddrhit
       test_db_isbrksizehit
       test_db_isdebuggablemode
  PROPERTY LABELS Core )
