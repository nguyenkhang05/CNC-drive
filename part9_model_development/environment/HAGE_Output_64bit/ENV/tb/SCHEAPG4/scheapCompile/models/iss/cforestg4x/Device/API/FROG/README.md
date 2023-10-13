g4x_device_api_frog
===================

\mainpage FROG Interface G4MHv2.

\section intro_sec Introduction
 
FROG I/F G4MHv2 is a interface is defined in CForest code which use to connect between CForest simulation and FROG tool.
It will contains the API functions that control behavior in CForest.
 
Now, FROG I/F design is proceeded by the cooperation by REL and RVC/Evaluation team and RVC/FROG team.
We will discuss which functions, features need to be supported and which limitation, behaviour is reasonable.

\section sec_purpose Purpose of the document:

This document is created for standardizing the FROG API implementation.
It will content the introduction, release information and especially, the details description for the function implementation.
 
The related people can depend on it to refer, discuss and update easily.
It is used to contain the release information for 3rd team if necessary.
 
 
\page page0 History
 
FROG I/F is designed at first by the cooperation between RVC/Eval-Quang and RVC/FROG-Trong and RVC/FROG-Nga.
We depended on the functions in E3XISS to discuss about the functions, features in this interface.
 
After many versions, now, the API functions in FROG I/F is clear and stable.
Now, it is still be updated frequently as ISA spec new version.


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
<caption id="multi_row">FROG I/F G4MHv2</caption>
<tr><th>No                      <th>Name        <th>Reason    <th>Current solution 
<tr>
</table>

\page page4 How to build/connect and run FROG I/F G4MHv2.

# How to build and connect FROG I/F G4MHv2.

FROG I/F G4MHv2 will be built as CForest library.
This library will be put in FROG tool to simulate the behavior of RH850V2.

## Step 1: Build FROG I/F library in CForestG4x source.

Please set the PATH and LD_LIBRARY_PATH variables in the environment at first.

~~~~~~~~~~~~~ {.c}
PATH=/common/appl/gcc/gcc-4.9.3-64bit/bin:/common/appl/python/python-2.7-64bit/bin:/common/appl/ccache-3.2.4/bin:/common/appl/cmake/cmake-3.4.3/bin:/common/appl/Git/git-flow/bin:/home/u/quanglam/software/bin:/common/appl/Git/git-2.9.3/bin:/common/appl/python/python-2.4.3-64bit/bin:/common/N1GEtool/RBS/bin:/common/appl2/CREDIST/app/bin:/common/lsftool/RBS/bin:/common/lsf/bin:./:/usr/bin:/usr/sbin:/usr/X11R6/bin:/usr/local/bin:/usr/ucb:/etc:/bin:/usr/etc:.
LD_LIBRARY_PATH=/lib:/common/appl/gcc/gcc-4.9.3-64bit/lib:/common/appl/gcc/gcc-4.9.3-64bit/lib64:/common/appl/python/python-2.7-64bit/lib:/common/appl/graphviz-2.28.0-x64/lib:/common/appl/Git/git-2.9.3/lib64:/common/appl/Git/git-2.9.3/lib:/common/appl/python/python-2.4.3-64bit/lib:/common/appl/CREDIST/Java/LINUX/jre1.5.0/lib:/usr/lib
~~~~~~~~~~~~~

Then, FROG I/F G4MHv2 can be built successfully.

~~~~~~~~~~~~~ {.sh}
$ cd cforestg4x
$ mkdir build_frogif_g4mhv2
$ cd build_frogif_g4mhv2
$ cmake -DCMAKE_C_COMPILER=/common/appl/gcc/gcc-4.9.3-64bit/bin/gcc -DCMAKE_CXX_COMPILER=/common/appl/gcc/gcc-4.9.3-64bit/bin/g++ -DPYTHON_EXECUTABLE=/common/appl/python/python-2.7-64bit/bin/python2.7 -DCMAKE_BUILD_TYPE=Release -DTARGET=FROG -DBUILD_TEST=OFF -DPERL_EXECUTABLE=/common/appl/Perl-5.8.9/bin/perl ..
$ make
~~~~~~~~~~~~~

## Step 2: Connect CForestG4x library to FROG tool.

FROG members will build FROG tool with CForestG4x library.
Then, FROG tool can use this library to simulate the behavior of RHV850.

