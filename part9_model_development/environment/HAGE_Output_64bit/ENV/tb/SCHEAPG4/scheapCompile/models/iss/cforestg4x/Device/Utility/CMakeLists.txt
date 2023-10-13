cmake_minimum_required( VERSION 3.2 )

project( DEVICE_UTILITY )

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}/include
)
add_subdirectory( src )

if( USE_UNIT_TEST )
  add_definitions( -D_UNIT_TEST )
  add_subdirectory( test )
endif( )

if( ${TARGET}  MATCHES "^SCHEAP$" )
  install( FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/forest_common_api.h DESTINATION include COMPONENT headers )
  install( FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/forest_utility_api.h DESTINATION include COMPONENT headers )
  install( FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/intc_api.h DESTINATION include COMPONENT headers )
  install( FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/sideband.h DESTINATION include COMPONENT headers )
endif( )
