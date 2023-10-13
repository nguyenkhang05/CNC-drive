cmake_minimum_required( VERSION 3.2 )

include_directories(
  ${TOP_SOURCE_DIR}/Device/Bus/FSBUS/include
  ${TOP_SOURCE_DIR}/Device/Bus/FSBUS/test
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Core/EXCEP/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include  
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/include
  ${TOP_SOURCE_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_BINARY_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC2/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC2/test
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test
  ${TOP_SOURCE_DIR}/Device/Utility/include
  ${TOP_SOURCE_DIR}/Device/Utility/test
  ${gmock_SOURCE_DIR}/include
  ${gtest_SOURCE_DIR}/include
)

add_library(libintc2
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC2/src/intc2.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/src/treg.cpp
)

add_dependencies( libintc2
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

# TEST CINTC2
add_executable( test_intc2_cintc2 test_intc2_cintc2.cpp
  )
target_link_libraries( test_intc2_cintc2
  libintc2
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc2_cintc2 COMMAND $<TARGET_FILE:test_intc2_cintc2> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc2_cintc2 coverage)
endif( )

# TEST CINTC2 20
add_executable( test_intc2_cintc2_20 test_intc2_cintc2_20.cpp
  )
target_link_libraries( test_intc2_cintc2_20
  libintc2
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc2_cintc2_20 COMMAND $<TARGET_FILE:test_intc2_cintc2_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc2_cintc2_20 coverage)
endif( )


# TEST TRegEIC
add_executable( test_intc2_tregeic_intc2 test_intc2_tregeic_intc2.cpp
  )
target_link_libraries( test_intc2_tregeic_intc2
  libintc2
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc2_tregeic_intc2 COMMAND $<TARGET_FILE:test_intc2_tregeic_intc2> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc2_tregeic_intc2 coverage)
endif( )

# TEST TRegEEIC
add_executable( test_intc2_tregeeic_intc2 test_intc2_tregeeic_intc2.cpp
  )
target_link_libraries( test_intc2_tregeeic_intc2
  libintc2
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc2_tregeeic_intc2 COMMAND $<TARGET_FILE:test_intc2_tregeeic_intc2> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc2_tregeeic_intc2 coverage)
endif( )


# TEST TRegEIC_20
add_executable( test_intc2_tregeic_intc2_20 test_intc2_tregeic_intc2_20.cpp
  )
target_link_libraries( test_intc2_tregeic_intc2_20
  libintc2
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc2_tregeic_intc2_20 COMMAND $<TARGET_FILE:test_intc2_tregeic_intc2_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc2_tregeic_intc2_20 coverage)
endif( )


# TEST TRegIMR
add_executable( test_intc2_tregimr test_intc2_tregimr.cpp
  )
target_link_libraries( test_intc2_tregimr
  libintc2
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc2_tregimr COMMAND $<TARGET_FILE:test_intc2_tregimr> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc2_tregimr coverage)
endif( )

# TEST TRegIMR 20
add_executable( test_intc2_tregimr_20 test_intc2_tregimr_20.cpp
  )
target_link_libraries( test_intc2_tregimr_20
  libintc2
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc2_tregimr_20 COMMAND $<TARGET_FILE:test_intc2_tregimr_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc2_tregimr_20 coverage)
endif( )


# TEST IntPriorityInfo
add_executable( test_intc2_intpriorityinfo test_intc2_intpriorityinfo.cpp
  )
target_link_libraries( test_intc2_intpriorityinfo
  libintc2
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc2_intpriorityinfo COMMAND $<TARGET_FILE:test_intc2_intpriorityinfo> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc2_intpriorityinfo coverage)
endif( )


# TEST IntPriorityInfo 20
add_executable( test_intc2_intpriorityinfo_20 test_intc2_intpriorityinfo_20.cpp
  )
target_link_libraries( test_intc2_intpriorityinfo_20
  libintc2
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc2_intpriorityinfo_20 COMMAND $<TARGET_FILE:test_intc2_intpriorityinfo_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc2_intpriorityinfo_20 coverage)
endif( )

# TEST EiintReqInfo
add_executable( test_intc2_eiintreqinfo test_intc2_eiintreqinfo.cpp
  )
target_link_libraries( test_intc2_eiintreqinfo
  libintc2
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc2_eiintreqinfo COMMAND $<TARGET_FILE:test_intc2_eiintreqinfo> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc2_eiintreqinfo coverage)
endif( )

# TEST EiintReqInfo 20
add_executable( test_intc2_eiintreqinfo_20 test_intc2_eiintreqinfo_20.cpp
  )
target_link_libraries( test_intc2_eiintreqinfo_20
  libintc2
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc2_eiintreqinfo_20 COMMAND $<TARGET_FILE:test_intc2_eiintreqinfo_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc2_eiintreqinfo_20 coverage)
endif( )


# TEST TRegIntc2
add_executable( test_intc2_tregintc2 test_intc2_tregintc2.cpp
  )
target_link_libraries( test_intc2_tregintc2
  libintc2
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc2_tregintc2 COMMAND $<TARGET_FILE:test_intc2_tregintc2> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc2_tregintc2 coverage)
endif( )

# TEST TRegIntc2 20
add_executable( test_intc2_tregintc2_20 test_intc2_tregintc2_20.cpp
  )
target_link_libraries( test_intc2_tregintc2_20
  libintc2
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc2_tregintc2_20 COMMAND $<TARGET_FILE:test_intc2_tregintc2_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc2_tregintc2_20 coverage)
endif( )


# TEST EIBD
add_executable( test_intc2_eibd test_intc2_eibd.cpp
  )
target_link_libraries( test_intc2_eibd
  libintc2
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc2_eibd COMMAND $<TARGET_FILE:test_intc2_eibd> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc2_eibd coverage)
endif( )

# TEST EIBD 20
add_executable( test_intc2_eibd_20 test_intc2_eibd_20.cpp
  )
target_link_libraries( test_intc2_eibd_20
  libintc2
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc2_eibd_20 COMMAND $<TARGET_FILE:test_intc2_eibd_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc2_eibd_20 coverage)
endif( )


# TEST TregBND
add_executable( test_intc2_tregbnd test_intc2_tregbnd.cpp
  )
target_link_libraries( test_intc2_tregbnd
libintc2
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc2_tregbnd COMMAND $<TARGET_FILE:test_intc2_tregbnd> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc2_tregbnd coverage)
endif( )

# TEST TregBND 20
add_executable( test_intc2_tregbnd_20 test_intc2_tregbnd_20.cpp
  )
target_link_libraries( test_intc2_tregbnd_20
libintc2
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc2_tregbnd_20 COMMAND $<TARGET_FILE:test_intc2_tregbnd_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc2_tregbnd_20 coverage)
endif( )

# TEST TregEIBG
add_executable( test_intc2_tregeibg test_intc2_tregeibg.cpp
  )
target_link_libraries( test_intc2_tregeibg
libintc2
  gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc2_tregeibg COMMAND $<TARGET_FILE:test_intc2_tregeibg> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc2_tregeibg coverage)
endif( )


# run with: ctest -L Peripheral
set_property(
  TEST test_intc2_cintc2 test_intc2_tregeibg test_intc2_cintc2_20 test_intc2_tregeic_intc2 test_intc2_tregeeic_intc2 test_intc2_tregeic_intc2_20 test_intc2_tregimr test_intc2_tregimr_20 test_intc2_intpriorityinfo test_intc2_intpriorityinfo_20 test_intc2_eiintreqinfo test_intc2_eiintreqinfo_20 test_intc2_tregintc2 test_intc2_tregintc2_20 test_intc2_eibd test_intc2_eibd_20 test_intc2_tregbnd test_intc2_tregbnd_20
  PROPERTY LABELS Peripheral)