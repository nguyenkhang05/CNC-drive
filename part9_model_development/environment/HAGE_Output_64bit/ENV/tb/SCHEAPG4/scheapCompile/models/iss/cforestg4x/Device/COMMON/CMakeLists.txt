cmake_minimum_required( VERSION 3.2 )

project( DEVICE_COMMON )

add_subdirectory( include )

if( ${TARGET}  MATCHES "^SCHEAP$" )
  install( FILES ${CMAKE_CURRENT_BINARY_DIR}/include/config.h DESTINATION include COMPONENT headers )
endif( )
