cmake_minimum_required( VERSION 3.2 )


include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Core/EXCEP/include
  ${TOP_SOURCE_DIR}/Device/Core/INST/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
  ${TOP_SOURCE_DIR}/Device/Bus/FSBUS/include
  ${TOP_BINARY_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/MICROARCHDB/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC2/include
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/include
  ${TOP_SOURCE_DIR}/Device/Utility/include  
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test
  ${TOP_SOURCE_DIR}/Device/Utility/test
  ${gmock_SOURCE_DIR}/include
  ${gtest_SOURCE_DIR}/include  
)

add_library(libintc
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/src/intc1.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC2/src/intc2.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/TARGETREG/src/treg.cpp  
)

add_dependencies( libintc
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

# TEST TREGEIC
add_executable( test_intc_tregeic_intc1 test_intc_tregeic_intc1.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregeic_intc1.cpp

 )
target_link_libraries( test_intc_tregeic_intc1
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregeic_intc1 COMMAND $<TARGET_FILE:test_intc_tregeic_intc1> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregeic_intc1 coverage)
endif( )

# TEST TREGEIC 2.0
add_executable( test_intc_tregeic_intc1_20 test_intc_tregeic_intc1_20.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregeic_intc1_20.cpp

 )
target_link_libraries( test_intc_tregeic_intc1_20
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregeic_intc1_20 COMMAND $<TARGET_FILE:test_intc_tregeic_intc1_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregeic_intc1_20 coverage)
endif( )


# TEST TREGIMR
add_executable( test_intc_tregimr0 test_intc_tregimr0.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregimr0.cpp

 )
target_link_libraries( test_intc_tregimr0
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregimr0 COMMAND $<TARGET_FILE:test_intc_tregimr0> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregimr0 coverage)
endif( )

# TEST TREGIMR 20
add_executable( test_intc_tregimr0_20 test_intc_tregimr0_20.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregimr0_20.cpp

 )
target_link_libraries( test_intc_tregimr0_20
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregimr0_20 COMMAND $<TARGET_FILE:test_intc_tregimr0_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregimr0_20 coverage)
endif( )


# TEST TREGEIBD
add_executable( test_intc_tregeibd test_intc_tregeibd.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregeibd.cpp

 )
target_link_libraries( test_intc_tregeibd
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregeibd COMMAND $<TARGET_FILE:test_intc_tregeibd> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregeibd coverage)
endif( )

# TEST TREGEIBD 20
add_executable( test_intc_tregeibd_20 test_intc_tregeibd_20.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregeibd_20.cpp

 )
target_link_libraries( test_intc_tregeibd_20
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregeibd_20 COMMAND $<TARGET_FILE:test_intc_tregeibd_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregeibd_20 coverage)
endif( )


# TEST TREGFNC
add_executable( test_intc_tregfnc test_intc_tregfnc.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregfnc.cpp

 )
target_link_libraries( test_intc_tregfnc
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregfnc COMMAND $<TARGET_FILE:test_intc_tregfnc> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregfnc coverage)
endif( )

# TEST TREGFNC 20
add_executable( test_intc_tregfnc_20 test_intc_tregfnc_20.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregfnc_20.cpp

 )
target_link_libraries( test_intc_tregfnc_20
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregfnc_20 COMMAND $<TARGET_FILE:test_intc_tregfnc_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregfnc_20 coverage)
endif( )


# TEST TREGFIC
add_executable( test_intc_tregfic test_intc_tregfic.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregfic.cpp
  
 )
target_link_libraries( test_intc_tregfic
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregfic COMMAND $<TARGET_FILE:test_intc_tregfic> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregfic coverage)
endif( )

# TEST TREGFIC 20
add_executable( test_intc_tregfic_20 test_intc_tregfic_20.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregfic_20.cpp
  
 )
target_link_libraries( test_intc_tregfic_20
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregfic_20 COMMAND $<TARGET_FILE:test_intc_tregfic_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregfic_20 coverage)
endif( )


# TEST TREGFIBD
add_executable( test_intc_tregfibd test_intc_tregfibd.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregfibd.cpp

 )
target_link_libraries( test_intc_tregfibd
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregfibd COMMAND $<TARGET_FILE:test_intc_tregfibd> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregfibd coverage)
endif( )

# TEST TREGFIBD 20
add_executable( test_intc_tregfibd_20 test_intc_tregfibd_20.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregfibd_20.cpp

 )
target_link_libraries( test_intc_tregfibd_20
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregfibd_20 COMMAND $<TARGET_FILE:test_intc_tregfibd_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregfibd_20 coverage)
endif( )

# TEST TREGFNBD
add_executable( test_intc_tregfnbd test_intc_tregfnbd.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregfnbd.cpp

 )
target_link_libraries( test_intc_tregfnbd
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregfnbd COMMAND $<TARGET_FILE:test_intc_tregfnbd> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregfnbd coverage)
endif( )

# TEST TREGFNBD 20
add_executable( test_intc_tregfnbd_20 test_intc_tregfnbd_20.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregfnbd_20.cpp

 )
target_link_libraries( test_intc_tregfnbd_20
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregfnbd_20 COMMAND $<TARGET_FILE:test_intc_tregfnbd_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregfnbd_20 coverage)
endif( )


# TEST TREGDBMK
add_executable( test_intc_tregdbmk test_intc_tregdbmk.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregdbmk.cpp

 )
target_link_libraries( test_intc_tregdbmk
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregdbmk COMMAND $<TARGET_FILE:test_intc_tregdbmk> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregdbmk coverage)
endif( )

# TEST TREGDBMK 20
add_executable( test_intc_tregdbmk_20 test_intc_tregdbmk_20.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregdbmk_20.cpp

 )
target_link_libraries( test_intc_tregdbmk_20
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregdbmk_20 COMMAND $<TARGET_FILE:test_intc_tregdbmk_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregdbmk_20 coverage)
endif( )


# TEST FENMIREQINFO
add_executable( test_intc_fenmireqinfo test_intc_fenmireqinfo.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_fenmireqinfo.cpp

 )
target_link_libraries( test_intc_fenmireqinfo
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_fenmireqinfo COMMAND $<TARGET_FILE:test_intc_fenmireqinfo> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_fenmireqinfo coverage)
endif( )

# TEST FEINTREQINFO
add_executable( test_intc_feintreqinfo test_intc_feintreqinfo.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_feintreqinfo.cpp
 )
target_link_libraries( test_intc_feintreqinfo
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_feintreqinfo COMMAND $<TARGET_FILE:test_intc_feintreqinfo> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_feintreqinfo coverage)
endif( )

# TEST TREGINTC1
add_executable( test_intc_tregintc1 test_intc_tregintc1.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregintc1.cpp
 )
target_link_libraries( test_intc_tregintc1
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregintc1 COMMAND $<TARGET_FILE:test_intc_tregintc1> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregintc1 coverage)
endif( )

# TEST TREGINTC1 20
add_executable( test_intc_tregintc1_20 test_intc_tregintc1_20.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregintc1_20.cpp
 )
target_link_libraries( test_intc_tregintc1_20
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregintc1_20 COMMAND $<TARGET_FILE:test_intc_tregintc1_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregintc1_20 coverage)
endif( )


# TEST CINTC1
add_executable( test_intc_cintc1 test_intc_cintc1.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_cintc1.cpp
 )
target_link_libraries( test_intc_cintc1
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_cintc1 COMMAND $<TARGET_FILE:test_intc_cintc1> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_cintc1 coverage)
endif( )

# TEST CINTC1 20
add_executable( test_intc_cintc1_20 test_intc_cintc1_20.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_cintc1_20.cpp
 )
target_link_libraries( test_intc_cintc1_20
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_cintc1_20 COMMAND $<TARGET_FILE:test_intc_cintc1_20> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_cintc1_20 coverage)
endif( )


# TEST TREGEIBG
add_executable( test_intc_tregeibg test_intc_tregeibg.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregeibg.cpp
 )
target_link_libraries( test_intc_tregeibg
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregeibg COMMAND $<TARGET_FILE:test_intc_tregeibg> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregeibg coverage)
endif( )

# TEST TREGFIBG
add_executable( test_intc_tregfibg test_intc_tregfibg.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregfibg.cpp
 )
target_link_libraries( test_intc_tregfibg
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregfibg COMMAND $<TARGET_FILE:test_intc_tregfibg> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregfibg coverage)
endif( )

# TEST TREGEEIC
add_executable( test_intc_tregeeic_intc1 test_intc_tregeeic_intc1.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregeeic_intc1.cpp
 )
target_link_libraries( test_intc_tregeeic_intc1
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregeeic_intc1 COMMAND $<TARGET_FILE:test_intc_tregeeic_intc1> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregeeic_intc1 coverage)
endif( )

# TEST TREGIHVCFG
add_executable( test_intc_tregihvcfg test_intc_tregihvcfg.cpp
  ${TOP_SOURCE_DIR}/Device/Peripheral/INTC/test/test_intc_tregihvcfg.cpp
 )
target_link_libraries( test_intc_tregihvcfg
libintc
 gmock gtest $<${hasPTHREAD}:pthread>
  $<$<BOOL:${CODE_COVERAGE}>:gcov>
)
add_test( NAME test_intc_tregihvcfg COMMAND $<TARGET_FILE:test_intc_tregihvcfg> )
if( CMAKE_COMPILER_IS_GNUCXX AND CODE_COVERAGE )
  setup_target_for_coverage( ${PROJECT_NAME}_coverage test_intc_tregihvcfg coverage)
endif( )


#run with: ctest -L Peripheral
set_property(
  TEST test_intc_tregeic_intc1 test_intc_tregimr0 test_intc_tregfnc test_intc_tregfic test_intc_tregeibd test_intc_tregfibd test_intc_tregfnbd test_intc_tregdbmk test_intc_fenmireqinfo test_intc_feintreqinfo test_intc_tregintc1 test_intc_cintc1 
  PROPERTY LABELS INTC )

set_property(
  TEST test_intc_tregeic_intc1_20 test_intc_tregimr0_20 test_intc_tregfnc_20 test_intc_tregfic_20 test_intc_tregeibd_20 test_intc_tregfibd_20 test_intc_tregfnbd_20 test_intc_tregdbmk_20 test_intc_fenmireqinfo test_intc_feintreqinfo test_intc_tregintc1_20 test_intc_cintc1_20 test_intc_tregeibg test_intc_tregfibg test_intc_tregeeic_intc1 test_intc_tregihvcfg 
  PROPERTY LABELS INTC20 )



