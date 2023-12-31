g4x_device_api_comprunner
=========================

\mainpage CompRunner G4MHv2.

\section intro_sec Introduction
 
 CompRunner G4MHv2 is a comparison tool which compare all of statuses, information, behaviour described by G4MHv2 specification between RTL and CForest during step execution.
 With this tool, the RTL designer can guarantee the implementation not only by output result but also the internal behaviour, status while it is still executing.
 
Please refer <a href="CompRunner_Overview_Eppt.pdf" target="_blank"><b>Overview</b></a> file to understand more about the basic concepts in CompRunner algorithm.
 
Now, CompRunner design is proceeded by the cooperation by REL and RVC.
We will discuss which functions, features need to be supported and which limitation, behaviour is reasonable.
CompRunner implementation is divided by 2 separately parts: SV side and C side.
* RVC/Logic members will be in-charge in SV side.
* RVC/Evaluation members will be in-charge in C side.

\section sec_purpose Purpose of the document:

This document is created for standardizing the CompRunner implementation.
It will content the introduction, release information and especially, the details description for the function implementation.
 
The related people can depend on it to refer, discuss and update easily.
It is used to contain the release information for 3rd party if necessary.
 
 
 \page page0 History
 
 CompRunner is designed at first by REL/Kimura. It included 2 parts: SV side and C side.
 C side is more difficult than SV side because it will support the necessary DPI functions for comparison during RTL execution.
 SV side will be managed by RTL/Logic members. C side will be managed RTL/EVAL-Quang.
 
 After many versions, we recognized that only one person understood about CompRunner C side was not good.
 At verification test, many issue were detected at regression test, the reason come from many resources: RTL, CForest and also CompRunner.
 Almost issues were analyzed in CompRunner log file, which generated by C side.
 
 So, now, RVC determined that we must have at least one more person in charge in CompRunner C side.
 And the transfer task were proceeding. Now RTL/Logic-Triet was the PIC in CompRunner G4MHv2 C side.
 
 We detected the last implementation lacked many comments/descriptions about CompRunner C side behavior/functions.
 So, we decided to apply Doxygen comments to last CompRunner C side.
 And this document (generated by Doxygen tool) is used to describe the implementation in CompRunner G4MHv2.
 It will show the related between the classes, variables, functions in CompRunner G4MHv2.
 
 So, from now, the new designer of CompRunner C side can catch up more easily the implementation.
 Thus, the bottle neck at verification step will be resolved (or at least, improved much).



 \page page1 Supported tool/OS version

 \section tool_sec Tool

 - cmake
 - g++ 4.9.3 (In CForestG4x, we will use g++4.9.3 with -std=c++14 option.)
 - Python2.7 (including Python2.7-dev for include files of python)
 
 \section os_sec OS
 
 - RHEL5


 
 \page page2 Supported target/conditions

 \section target_sec Target

 - Spec: RH850V2.0, V2.1
 - G4MHv2.0 (ISA Extension v2.2)
 
 \section conditions_sec Conditions
 
 - None.



 \page page3 Limitation

Will be update more ...

 <table>
<caption id="multi_row">CompRunner G4MHv2</caption>
<tr><th>No                      <th>Name        <th>Reason    <th>Current solution    
<tr><td> 1 <td>Xdump at shared memory region(old name: XDump for data load function). <td>Timing difference between RTL and CForest. Now, this issue is only occurred at shared memory in multi-core. <td> (Takada-san comment) We must continue Xdump itself as the situation which truly needed don't care values BTW, let's start to realize the TDump function.
<tr><td> 2 <td>Does not support BitOp/CAXI flag write back at shared memory region by XDump function. <td>Timing difference between RTL and CForest. Now, this issue is only occurred on shared memory at multi-core. <td> Use xdump_flag (start_addr, end_addr) for BitOp/CAXI and xdump_gr (start_addr, end_addr) for CAXI.
<tr><td> 3 <td>Does not support for STC.x at shared memory region by XDump function <td>Timing difference between RTL and CForest. Now, this issue is only occurred on shared memory at multi-core. <td> Use xdump_gr (start_addr, end_addr) for STC.x.
</table>

\page page4 How to build/connect and run CompRunner G4MHv2.

# How to build and connect CompRunner G4MHv2.

CompRunner G4MHv2 includes 2 part:
* C side library in CForestG4x repo.
* SV side file in RTL environment.

## Step 1: Build CompRunner C library in CForestG4x source.

Please set the PATH and LD_LIBRARY_PATH variables in the environment at first.

~~~~~~~~~~~~~ {.c}
PATH=/common/appl/gcc/gcc-4.9.3-64bit/bin:/common/appl/python/python-2.7-64bit/bin:/common/appl/ccache-3.2.4/bin:/common/appl/cmake/cmake-3.4.3/bin:/common/appl/Git/git-flow/bin:/home/u/quanglam/software/bin:/common/appl/Git/git-2.9.3/bin:/common/appl/python/python-2.4.3-64bit/bin:/common/N1GEtool/RBS/bin:/common/appl2/CREDIST/app/bin:/common/lsftool/RBS/bin:/common/lsf/bin:./:/usr/bin:/usr/sbin:/usr/X11R6/bin:/usr/local/bin:/usr/ucb:/etc:/bin:/usr/etc:.
LD_LIBRARY_PATH=/lib:/common/appl/gcc/gcc-4.9.3-64bit/lib:/common/appl/gcc/gcc-4.9.3-64bit/lib64:/common/appl/python/python-2.7-64bit/lib:/common/appl/graphviz-2.28.0-x64/lib:/common/appl/Git/git-2.9.3/lib64:/common/appl/Git/git-2.9.3/lib:/common/appl/python/python-2.4.3-64bit/lib:/common/appl/CREDIST/Java/LINUX/jre1.5.0/lib:/usr/lib
~~~~~~~~~~~~~
 
Then, CompRunner C library can be built.
 
~~~~~~~~~~~~~ {.sh}
$ cd cforestg4x
$ mkdir build_comp_g4mhv2
$ cd build_comp_g4mhv2
$ cmake -DCMAKE_C_COMPILER=/common/appl/gcc/gcc-4.9.3-64bit/bin/gcc -DCMAKE_CXX_COMPILER=/common/appl/gcc/gcc-4.9.3-64bit/bin/g++ -DPYTHON_EXECUTABLE=/common/appl/python/python-2.7-64bit/bin/python2.7 -DCMAKE_BUILD_TYPE=Release -DTARGET=COMPRUNNER -DBUILD_TEST=OFF ..
$ make
~~~~~~~~~~~~~

## Step 2: Connect CompRunner C library to CompRunner SV file in RTL environment.

Please copy C library file to CompRunner build directory in RTL environment.
Note: This is just temporary RTL environment. The current environment can be different.

~~~~~~~~~~~~~ {.sh}
$ cd hg_repo_G4MH1.1/build_6PE
$ cp -fv ../../cforestg4x/build_comp_g4mhv2/Device/API/COMPRUNNER/src/libforest_cmprunner_g4mh.a ../iss/libforest_cmprunner_g4mh_debug.a
$ make clean all L1RAM=VMEM
~~~~~~~~~~~~~

### Note:
Now, at Sep/E, only CompRunner C side library is available. CompRunner SV side is under-implementation.

- - -

# How to run CompRunner G4MHv2 with test pattern.

This is the CompRunner G4MHv2 6 PEs environment.
So, the test pattern should support 6 PEs to run normally.
Note: This is just temporary RTL environment. The current environment can be different.

~~~~~~~~~~~~~ {.sh}
$ cd hg_repo_G4MH1.1/build_6PE
$ ../scripts/run_simv_6pe.sh $1 +MaxSimTime=1000000000 +CmpFailMax=1 +MemInit=00 +GPR_INIT +WR_INIT +hexa_env +icsize=3 +L1RamSize=0101 +SvaFailContinue ../../test.srec
~~~~~~~~~~~~~
