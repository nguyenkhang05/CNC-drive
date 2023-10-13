# Install script for directory: /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RELEASE")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/COMMON/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/API/SCHEAP/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Bus/FSBUS/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/BTB/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/CEDAR/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/COREGUARD/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/COUNTERBASE/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/CYCLESIM/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/EXCEP/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/ICACHE/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/INST/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/LLSC/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/LSU/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MISR/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MPU/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/PE/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/REG/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/SIMBREAK/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/STATISTICS/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/TRACE/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Peripheral/DEBUGAREA/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Peripheral/ERRORAREA/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Peripheral/GUARD/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Peripheral/GUARD_BASE/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Peripheral/INTC/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Peripheral/INTC2/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Peripheral/INVALIDLRAM/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Peripheral/IPIR/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Peripheral/LATENCYINFO/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Peripheral/MECNT/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Peripheral/MEM/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Peripheral/TARGETREG/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Peripheral/BAR/cmake_install.cmake")
  include("/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Utility/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
