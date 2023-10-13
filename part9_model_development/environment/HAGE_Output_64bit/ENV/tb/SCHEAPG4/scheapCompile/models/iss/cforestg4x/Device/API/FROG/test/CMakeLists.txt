cmake_minimum_required( VERSION 3.2 )

include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/Core/REG/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Utility/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/test
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${gmock_SOURCE_DIR}/include
  ${gtest_SOURCE_DIR}/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ut_CForest_g4mh_1pe.py ${CMAKE_CURRENT_BINARY_DIR}/ut_CForest_g4mh20_1pe.py
  COMMAND ${CMAKE_COMMAND} -E copy ut_CForest_g4mh_1pe.py ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ut_CForest_g4mh20_1pe.py ${CMAKE_CURRENT_BINARY_DIR}
  WORKING_DIRECTORY ${TOP_SOURCE_DIR}/Samples/python_script/FROG
  DEPENDS  ${TOP_SOURCE_DIR}/Samples/python_script/FROG/ut_CForest_g4mh_1pe.py ${TOP_SOURCE_DIR}/Samples/python_script/FROG/ut_CForest_g4mh20_1pe.py
  COMMENT "Copy ut_CForest_g4mhx_1pe.py."
)
add_custom_target(
  copy_frog_ut_python_g4mh ALL
  SOURCES ${CMAKE_CURRENT_BINARY_DIR}/ut_CForest_g4mh_1pe.py
  SOURCES ${CMAKE_CURRENT_BINARY_DIR}/ut_CForest_g4mh20_1pe.py
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/test_host_guest.hex
  COMMAND ${CMAKE_COMMAND} -E copy test.hex ${CMAKE_CURRENT_BINARY_DIR}/test_host_guest.hex
  WORKING_DIRECTORY ${TOP_SOURCE_DIR}/Samples/host_guest
  DEPENDS ${TOP_SOURCE_DIR}/Samples/host_guest/test.hex
  COMMENT "Copy test hex for frog ut."
)
add_custom_target(
  copy_frog_ut_hex_g4mh ALL
  SOURCES ${CMAKE_CURRENT_BINARY_DIR}/test_host_guest.hex
)


add_executable( test_forest_frog
  ${TOP_SOURCE_DIR}/Device/API/FROG/src/frog_api_test_ut_g4mh.cpp
)

add_dependencies( test_forest_frog
  copy_frog_ut_python_g4mh
  copy_frog_ut_hex_g4mh
)

target_link_libraries( test_forest_frog
  $<$<BOOL:${USE_PYTHON_API}>:${PYTHON_LIBRARY}>
  forest_frog
)
