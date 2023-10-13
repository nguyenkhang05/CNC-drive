cmake_minimum_required( VERSION 3.2 )

include_directories(
  ${TOP_BINARY_DIR}/Device/COMMON/include
  ${TOP_SOURCE_DIR}/Device/API/CFORESTG4X/include
  ${TOP_SOURCE_DIR}/Device/Core/LSU/include
  ${TOP_BINARY_DIR}/Device/Core/REG/include
  ${TOP_SOURCE_DIR}/Device/Core/TRACE/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/include
  ${TOP_SOURCE_DIR}/Device/Core/PE/test
  ${TOP_SOURCE_DIR}/Device/Utility/include
  ${gmock_SOURCE_DIR}/include
  ${gtest_SOURCE_DIR}/include
  if( USE_PYTHON_API )
    ${TOP_SOURCE_DIR}/Device/API/PYTHON/include
    ${PYTHON_INCLUDE_DIR}
  endif( )
)

add_executable( test_forest_scheap
  ${TOP_SOURCE_DIR}/Device/API/CFORESTG4X/src/forest_main.cpp
  ${TOP_SOURCE_DIR}/Device/API/CFORESTG4X/src/forest_cmdopt.cpp
  $<$<BOOL:${USE_PYTHON_API}>:$<TARGET_OBJECTS:device_api_python>>
)

target_link_libraries( test_forest_scheap
  forest_scheap
  $<$<BOOL:${USE_PYTHON_API}>:${PYTHON_LIBRARY}>
)
