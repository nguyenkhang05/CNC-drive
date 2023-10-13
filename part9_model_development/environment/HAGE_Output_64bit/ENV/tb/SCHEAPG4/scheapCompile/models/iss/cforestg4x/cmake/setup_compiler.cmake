# Compile Flags
# set( CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wconversion -Wdisabled-optimization -Wendif-labels -Wfloat-equal -Winit-self -Winline -Wlogical-op -Wmissing-include-dirs -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-promo -Wswitch-default -Wswitch-enum -Wunsafe-loop-optimizations -Wvariadic-macros -Wwrite-strings -fpermissive" )
if (MSVC)
  # Newlines inside flags variables break CMake's NMake generator.
  # TODO(vladl@google.com): Add -RTCs and -RTCu to debug builds.
  set( cxx_base_flags "-GS -W4 -wd4251 -wd4275 -wd4201 -wd4100 -wd4127 -nologo -J -Zi -EHsc" )
  set( cxx_base_flags "${cxx_base_flags} -D_UNICODE -DUNICODE -DWIN32 -D_WIN32" )
  set( cxx_base_flags "${cxx_base_flags} -DSTRICT -DWIN32_LEAN_AND_MEAN" )
  set( cxx_base_flags "${cxx_base_flags} -DBUILD_CFOREST" )
  set( c_base_flags "${cxx_base_flags}" )
  set( CMAKE_C_FLAGS_DEBUG   "-Od" )
  set( CMAKE_CXX_FLAGS_DEBUG "-Od" )
  set( CMAKE_C_FLAGS_RELEASE   "-Ot -DNDEBUG -GL" )
  set( CMAKE_CXX_FLAGS_RELEASE "-Ot -DNDEBUG -GL" )
  set( CMAKE_C_FLAGS_RELWITHDEBINFO   "-Od" )
  set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Od" )
  set( CMAKE_C_FLAGS_MINSIZEREL   "-Os -DNDEBUG -GL" )
  set( CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -GL" )
  set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}" )
  set( CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}" )
  set( CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} -LTCG" )
  set( CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -LTCG" )
  set( CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}" )
  set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}" )
  set( CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}" )
  set( CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL "${CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL} -LTCG" )
  set( CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -LTCG" )
  set( CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO}" )
  set( CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS}" )
  set( CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS_DEBUG}" )
  set( CMAKE_STATIC_LINKER_FLAGS_MINSIZEREL "${CMAKE_STATIC_LINKER_FLAGS_MINSIZEREL} -LTCG" )
  set( CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS_RELEASE} -LTCG" )
  set( CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO}" )

elseif( CMAKE_COMPILER_IS_GNUCXX )
  if( CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9" )
    message( FATAL_ERROR "g++ version is ${CMAKE_CXX_COMPILER_VERSION} less than 4.9." )
  endif( )

  set( c_base_flags "-Wall -Wshadow" )
  set( c_strict_flags
    "-Wextra -Wno-unused-parameter -Wno-missing-field-initializers" )

  set( cxx_base_flags "-std=c++14 -Wall -Wshadow -fpermissive" )
  set( cxx_strict_flags
    "-Wextra -Wno-unused-parameter -Wno-missing-field-initializers" )

  set( cxx_base_flags "${cxx_base_flags} -DBUILD_CFOREST" )
  set( c_base_flags "${c_base_flags} -DBUILD_CFOREST" )

  if( CODE_COVERAGE )
    set( CMAKE_C_FLAGS_DEBUG   "-g3 -O0 -pg" )
    set( CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -pg" )
  else ( )
    set( CMAKE_C_FLAGS_DEBUG   "-g3 -O0" )
    set( CMAKE_CXX_FLAGS_DEBUG "-g3 -O0" )
  endif( )
  set( CMAKE_C_FLAGS_RELEASE   "-O3 -s -DNDEBUG" )
  set( CMAKE_CXX_FLAGS_RELEASE "-O3 -s -DNDEBUG" )
  set( CMAKE_C_FLAGS_RELWITHDEBINFO   "-g -Og" )
  set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -Og" )
  set( CMAKE_C_FLAGS_MINSIZEREL   "-Os -s -DNDEBUG" )
  set( CMAKE_CXX_FLAGS_MINSIZEREL "-Os -s -DNDEBUG" )

  if( ${TARGET}  MATCHES "^SCHEAP$" )
    set( cxx_base_flags "${cxx_base_flags} -fPIC" )
    set( c_base_flags "${c_base_flags} -fPIC" )

  endif( )
endif()
set( CMAKE_C_FLAGS   "${c_base_flags}   ${c_strict_flags}" )
set( CMAKE_CXX_FLAGS "${cxx_base_flags} ${cxx_strict_flags}" )

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo
MinSizeRel."
    FORCE )
endif()
