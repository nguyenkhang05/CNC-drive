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
include Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/depend.make

# Include the progress variables for this target.
include Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/progress.make

# Include the compile flags for this target's objects.
include Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/flags.make

Device/Core/MICROARCHDB/src/cedar_arch_info.cpp: Device/Core/MICROARCHDB/tool/cedar_arch_info_gen.pl
Device/Core/MICROARCHDB/src/cedar_arch_info.cpp: Device/Core/MICROARCHDB/src/V850E3V5_MicroArchDB.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generate cedar_arch_info."
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src && /usr/bin/perl -w /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/tool/cedar_arch_info_gen.pl /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src/V850E3V5_MicroArchDB.txt cedar_arch_info
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src && /common/appl/cmake/cmake-3.4.3-RHEL6/bin/cmake -E make_directory /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/include
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src && /common/appl/cmake/cmake-3.4.3-RHEL6/bin/cmake -E copy inst_id_list.h /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/include
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src && /common/appl/cmake/cmake-3.4.3-RHEL6/bin/cmake -E copy inst_declaration.h /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/include
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src && /common/appl/cmake/cmake-3.4.3-RHEL6/bin/cmake -E copy inst_argtype.h /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/include
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src && /common/appl/cmake/cmake-3.4.3-RHEL6/bin/cmake -E remove inst_id_list.h inst_declaration.h inst_argtype.h

Device/Core/MICROARCHDB/src/inst_func_table.cpp: Device/Core/MICROARCHDB/src/cedar_arch_info.cpp
	@$(CMAKE_COMMAND) -E touch_nocreate Device/Core/MICROARCHDB/src/inst_func_table.cpp

Device/Core/MICROARCHDB/include/inst_id_list.h: Device/Core/MICROARCHDB/src/cedar_arch_info.cpp
	@$(CMAKE_COMMAND) -E touch_nocreate Device/Core/MICROARCHDB/include/inst_id_list.h

Device/Core/MICROARCHDB/include/inst_declaration.h: Device/Core/MICROARCHDB/src/cedar_arch_info.cpp
	@$(CMAKE_COMMAND) -E touch_nocreate Device/Core/MICROARCHDB/include/inst_declaration.h

Device/Core/MICROARCHDB/include/inst_argtype.h: Device/Core/MICROARCHDB/src/cedar_arch_info.cpp
	@$(CMAKE_COMMAND) -E touch_nocreate Device/Core/MICROARCHDB/include/inst_argtype.h

Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/micro_arch_db.cpp.o: Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/flags.make
Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/micro_arch_db.cpp.o: Device/Core/MICROARCHDB/src/micro_arch_db.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/micro_arch_db.cpp.o"
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src && /shsv/MobAP2/prj_MCS/01_common/04_gcc/gcc-4.9.3-64bit/bin/g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/device_core_microarchdb.dir/micro_arch_db.cpp.o -c /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src/micro_arch_db.cpp

Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/micro_arch_db.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/device_core_microarchdb.dir/micro_arch_db.cpp.i"
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src && /shsv/MobAP2/prj_MCS/01_common/04_gcc/gcc-4.9.3-64bit/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src/micro_arch_db.cpp > CMakeFiles/device_core_microarchdb.dir/micro_arch_db.cpp.i

Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/micro_arch_db.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/device_core_microarchdb.dir/micro_arch_db.cpp.s"
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src && /shsv/MobAP2/prj_MCS/01_common/04_gcc/gcc-4.9.3-64bit/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src/micro_arch_db.cpp -o CMakeFiles/device_core_microarchdb.dir/micro_arch_db.cpp.s

Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/micro_arch_db.cpp.o.requires:

.PHONY : Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/micro_arch_db.cpp.o.requires

Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/micro_arch_db.cpp.o.provides: Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/micro_arch_db.cpp.o.requires
	$(MAKE) -f Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/build.make Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/micro_arch_db.cpp.o.provides.build
.PHONY : Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/micro_arch_db.cpp.o.provides

Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/micro_arch_db.cpp.o.provides.build: Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/micro_arch_db.cpp.o


Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/cedar_arch_info.cpp.o: Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/flags.make
Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/cedar_arch_info.cpp.o: Device/Core/MICROARCHDB/src/cedar_arch_info.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/cedar_arch_info.cpp.o"
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src && /shsv/MobAP2/prj_MCS/01_common/04_gcc/gcc-4.9.3-64bit/bin/g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/device_core_microarchdb.dir/cedar_arch_info.cpp.o -c /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src/cedar_arch_info.cpp

Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/cedar_arch_info.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/device_core_microarchdb.dir/cedar_arch_info.cpp.i"
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src && /shsv/MobAP2/prj_MCS/01_common/04_gcc/gcc-4.9.3-64bit/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src/cedar_arch_info.cpp > CMakeFiles/device_core_microarchdb.dir/cedar_arch_info.cpp.i

Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/cedar_arch_info.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/device_core_microarchdb.dir/cedar_arch_info.cpp.s"
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src && /shsv/MobAP2/prj_MCS/01_common/04_gcc/gcc-4.9.3-64bit/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src/cedar_arch_info.cpp -o CMakeFiles/device_core_microarchdb.dir/cedar_arch_info.cpp.s

Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/cedar_arch_info.cpp.o.requires:

.PHONY : Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/cedar_arch_info.cpp.o.requires

Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/cedar_arch_info.cpp.o.provides: Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/cedar_arch_info.cpp.o.requires
	$(MAKE) -f Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/build.make Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/cedar_arch_info.cpp.o.provides.build
.PHONY : Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/cedar_arch_info.cpp.o.provides

Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/cedar_arch_info.cpp.o.provides.build: Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/cedar_arch_info.cpp.o


Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/inst_func_table.cpp.o: Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/flags.make
Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/inst_func_table.cpp.o: Device/Core/MICROARCHDB/src/inst_func_table.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/inst_func_table.cpp.o"
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src && /shsv/MobAP2/prj_MCS/01_common/04_gcc/gcc-4.9.3-64bit/bin/g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/device_core_microarchdb.dir/inst_func_table.cpp.o -c /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src/inst_func_table.cpp

Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/inst_func_table.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/device_core_microarchdb.dir/inst_func_table.cpp.i"
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src && /shsv/MobAP2/prj_MCS/01_common/04_gcc/gcc-4.9.3-64bit/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src/inst_func_table.cpp > CMakeFiles/device_core_microarchdb.dir/inst_func_table.cpp.i

Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/inst_func_table.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/device_core_microarchdb.dir/inst_func_table.cpp.s"
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src && /shsv/MobAP2/prj_MCS/01_common/04_gcc/gcc-4.9.3-64bit/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src/inst_func_table.cpp -o CMakeFiles/device_core_microarchdb.dir/inst_func_table.cpp.s

Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/inst_func_table.cpp.o.requires:

.PHONY : Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/inst_func_table.cpp.o.requires

Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/inst_func_table.cpp.o.provides: Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/inst_func_table.cpp.o.requires
	$(MAKE) -f Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/build.make Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/inst_func_table.cpp.o.provides.build
.PHONY : Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/inst_func_table.cpp.o.provides

Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/inst_func_table.cpp.o.provides.build: Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/inst_func_table.cpp.o


device_core_microarchdb: Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/micro_arch_db.cpp.o
device_core_microarchdb: Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/cedar_arch_info.cpp.o
device_core_microarchdb: Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/inst_func_table.cpp.o
device_core_microarchdb: Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/build.make

.PHONY : device_core_microarchdb

# Rule to build all files generated by this target.
Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/build: device_core_microarchdb

.PHONY : Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/build

Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/requires: Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/micro_arch_db.cpp.o.requires
Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/requires: Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/cedar_arch_info.cpp.o.requires
Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/requires: Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/inst_func_table.cpp.o.requires

.PHONY : Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/requires

Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/clean:
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src && $(CMAKE_COMMAND) -P CMakeFiles/device_core_microarchdb.dir/cmake_clean.cmake
.PHONY : Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/clean

Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/depend: Device/Core/MICROARCHDB/src/cedar_arch_info.cpp
Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/depend: Device/Core/MICROARCHDB/src/inst_func_table.cpp
Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/depend: Device/Core/MICROARCHDB/include/inst_id_list.h
Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/depend: Device/Core/MICROARCHDB/include/inst_declaration.h
Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/depend: Device/Core/MICROARCHDB/include/inst_argtype.h
	cd /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src /shsv/MobAP2/prj_VPF/99_Work/thiepnguyen/DTSG_U2_Output/ENV/tb/SCHEAPG4/scheapCompile/models/iss/cforestg4x/Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Device/Core/MICROARCHDB/src/CMakeFiles/device_core_microarchdb.dir/depend

