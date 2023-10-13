cmake_minimum_required( VERSION 3.2 )

include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
)

add_executable( test_forest_csplus
  ${TOP_SOURCE_DIR}/Device/API/CSPLUS/src/cube_suite_test.cpp
)

target_link_libraries( test_forest_csplus CForest_G4x )
