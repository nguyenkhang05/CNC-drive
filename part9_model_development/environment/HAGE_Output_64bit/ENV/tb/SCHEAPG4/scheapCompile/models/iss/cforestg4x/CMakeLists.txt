cmake_minimum_required( VERSION 3.2 )

project( TOP )

set(CMAKE_MODULE_PATH ${TOP_SOURCE_DIR}/cmake)

set( CMAKE_USE_RELATIVE_PATHS "ON" )
include( setup_ccache )

# Setup compiler
include( setup_compiler )

option( BUILD_TEST "Build the test-suite for CForestG4X." OFF )
if( BUILD_TEST )
  enable_testing( )
  set( USE_UNIT_TEST ON
    CACHE BOOL "Build a UNIT_TEST." FORCE )
  option( SOFTFLOAT_FULLTEST "Run the full test of softfloat." OFF ) # SOFTFLOAT FULL TEST
  set( BUILD_GTEST ON
    CACHE BOOL "Build a gtest library." FORCE )
  set( BUILD_GMOCK ON
    CACHE BOOL "Build a gmock library." FORCE )
	if (NOT (MSVC))
	  set( gtest_disable_pthreads OFF
	    CACHE BOOL "Disable pthread." FORCE )
	endif()
  add_subdirectory( Tools/googletest )
else( )
  unset( BUILD_GMOCK                 CACHE )
  unset( BUILD_GTEST                 CACHE )
  unset( USE_UNIT_TEST               CACHE )
  unset( SOFTFLOAT_FULLTEST          CACHE )
  unset( gmock_build_tests           CACHE )
  unset( gtest_build_samples         CACHE )
  unset( gtest_build_tests           CACHE )
  unset( gtest_disable_pthreads      CACHE )
  unset( gtest_disable_pthreads      CACHE )
  unset( gtest_force_shared_crt      CACHE )
  unset( gtest_hide_internal_symbols CACHE )
endif( )

#option( INTC20 "Build the INTC20 for CForestG4X." OFF )
#if( INTC20 )
#set( USE_INTC20 ON
#    CACHE BOOL "Use INTC20." FORCE )
#endif()

# Set default target
set( TARGET "CFORESTG4X" CACHE STRING
  "Choose the target of build, taeget are: CFORESTG4X/COMPRUNNER/CSPLUS/FROG/SCHEAP." )

set( PRODUCT_TYPE "UNKNOWN" )

# Get Version
find_package( Git )
set( CFOREST_VERSION "UNKNOWN" )
if( GIT_FOUND )
  # Get the version number from the git tag.
  execute_process( COMMAND ${GIT_EXECUTABLE} describe --abbrev=0 --tags
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE CFOREST_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE GIT_RESULT
    ERROR_QUIET )
  # message( STATUS "1 GIT_RESULT: " ${GIT_RESULT} )
  # message( STATUS "1 CFOREST_VERSION: " ${CFOREST_VERSION} )
  if( GIT_RESULT )
    execute_process( COMMAND ${GIT_EXECUTABLE} tag -l
      COMMAND sort
      COMMAND tail -1
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      OUTPUT_VARIABLE CFOREST_VERSION
      OUTPUT_STRIP_TRAILING_WHITESPACE
      RESULT_VARIABLE GIT_RESULT )
    # message( STATUS "2 GIT_RESULT: " ${GIT_RESULT} )
    # message( STATUS "2 CFOREST_VERSION: " ${CFOREST_VERSION} )
  endif( )
endif( )

# Get Revision
set( CFOREST_REVISION "UNKNOWN" )
if( GIT_FOUND )
  # Get the revision number from the git tag.
  execute_process( COMMAND ${GIT_EXECUTABLE} log --format=%h -1
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE CFOREST_REVISION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE GIT_RESULT
    ERROR_QUIET )
endif( )

# Code coverage
option( CODE_COVERAGE "To discover untested parts of CForestG4X." OFF )
if( CODE_COVERAGE )
  if( CMAKE_COMPILER_IS_GNUCXX )
    include( CodeCoverage )
  endif( )
endif( )

# Load function for cppcheck
include( CppcheckTargets )

# output compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Select product
if( ${TARGET}  MATCHES "^CFORESTG4X$" )
  set( PRODUCT_TYPE CFORESTG4X )
  project( CFORESTG4X )
  include( Product/CFORESTG4X.cmake )

elseif( ${TARGET} MATCHES "^COMPRUNNER$" )
  set( PRODUCT_TYPE COMPRUNNER )
  project( COMPRUNNER )
  include( Product/COMPRUNNER.cmake )

elseif( ${TARGET} MATCHES "^FROG$" )
  set( PRODUCT_TYPE FROG )
  project( FROG )
  include( Product/FROG.cmake )

elseif( ${TARGET} MATCHES "^CSPLUS$" )
  set( PRODUCT_TYPE CSPLUS )
  project( CSPLUS )
  include( Product/CSPLUS.cmake )

elseif( ${TARGET}  MATCHES "^SCHEAP$" )
  set( PRODUCT_TYPE SCHEAP )
  project( SCHEAP )
  include( Product/SCHEAP.cmake )

else( )
  message( SEND_ERROR "TARGET is nothing." )
endif( )

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")

#get_cmake_property( _variableNames VARIABLES )
#foreach( _variableName ${_variableNames} )
#  message( STATUS "${_variableName}=${${_variableName}}" )
#endforeach( )
