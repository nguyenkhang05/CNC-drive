cmake_minimum_required( VERSION 3.2 )

project( DEVICE_CORE_REG )

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}/include
)
add_subdirectory( src )

if( USE_UNIT_TEST )
  add_definitions( -D_UNIT_TEST )
  add_subdirectory( test )
endif( )
