# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.4

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /common/appl/cmake/cmake-3.4.3-RHEL6/bin/cmake

# The command to remove a file.
RM = /common/appl/cmake/cmake-3.4.3-RHEL6/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x

# Include any dependencies generated for this target.
include Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/depend.make

# Include the progress variables for this target.
include Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/progress.make

# Include the compile flags for this target's objects.
include Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/flags.make

Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/peg.cpp.o: Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/flags.make
Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/peg.cpp.o: Device/Core/COREGUARD/src/peg.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/peg.cpp.o"
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/COREGUARD/src && /shsv/MobAP2/prj_MCS/01_common/04_gcc/gcc-4.9.3-64bit/bin/g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/device_core_coreguard.dir/peg.cpp.o -c /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/COREGUARD/src/peg.cpp

Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/peg.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/device_core_coreguard.dir/peg.cpp.i"
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/COREGUARD/src && /shsv/MobAP2/prj_MCS/01_common/04_gcc/gcc-4.9.3-64bit/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/COREGUARD/src/peg.cpp > CMakeFiles/device_core_coreguard.dir/peg.cpp.i

Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/peg.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/device_core_coreguard.dir/peg.cpp.s"
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/COREGUARD/src && /shsv/MobAP2/prj_MCS/01_common/04_gcc/gcc-4.9.3-64bit/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/COREGUARD/src/peg.cpp -o CMakeFiles/device_core_coreguard.dir/peg.cpp.s

Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/peg.cpp.o.requires:

.PHONY : Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/peg.cpp.o.requires

Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/peg.cpp.o.provides: Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/peg.cpp.o.requires
	$(MAKE) -f Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/build.make Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/peg.cpp.o.provides.build
.PHONY : Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/peg.cpp.o.provides

Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/peg.cpp.o.provides.build: Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/peg.cpp.o


device_core_coreguard: Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/peg.cpp.o
device_core_coreguard: Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/build.make

.PHONY : device_core_coreguard

# Rule to build all files generated by this target.
Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/build: device_core_coreguard

.PHONY : Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/build

Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/requires: Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/peg.cpp.o.requires

.PHONY : Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/requires

Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/clean:
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/COREGUARD/src && $(CMAKE_COMMAND) -P CMakeFiles/device_core_coreguard.dir/cmake_clean.cmake
.PHONY : Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/clean

Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/depend:
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/COREGUARD/src /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/COREGUARD/src /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Device/Core/COREGUARD/src/CMakeFiles/device_core_coreguard.dir/depend
