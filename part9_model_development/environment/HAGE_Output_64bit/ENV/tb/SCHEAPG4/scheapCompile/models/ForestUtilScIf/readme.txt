#################### How to build ForestUtilScIf in Linux #######################
Step 1) Confirm the path of gcc4.8.4 & SystemC in setup_osci.csh & setup_usk.csh
Step 2) Build and setup ForestUtilScIf library
            >>./build_ForestUtilScIf.csh

--> Built libraries will be copied to /SCHEAPE3/scheapCompile/lib/
    Files: libForestUtilScIf.so
           libForestUtilScIf.so.0
           libForestUtilScIf-usk.so
           libForestUtilScIf-usk.so.0

################### How to build PseqsimIf in Windows #####################
Step 1) Build systemc-2.3.0 with Visual Studio 2015 and add the path to project file in /msvc150/
Step 2) Open project file in /msvc140/ with Visual Studio 2015
Step 3) Compile ForestUtilScIf library with "Release" and "Release_usk" configuration

--> Built library will be copied to /SCHEAPE3/scheapCompile/lib/
    Files: ForestUtilScIf.dll
           ForestUtilScIf.lib
           ForestUtilScIf-usk.dll
           ForestUtilScIf-usk.lib
