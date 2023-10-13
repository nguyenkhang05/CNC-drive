#!/bin/csh

set OS_VER="`/bin/uname -rs`"
switch ("$OS_VER")
    case "Linux 2.6*":
        if ( -f /etc/redhat-release ) then
            set RHEL_VER=`cat /etc/redhat-release | awk '{print $7}'`
            switch ( "$RHEL_VER" )
                ######## RHEL5 #########
                case "5*":
                    set GNUPATH="/design05/CPU/armory/tool/GNU/x86-gcc/gcc-4.9.3"
                    set PYPATH="/design05/CPU/armory/tool/Python/python2.7"
                breaksw
                ##### end of RHEL5 #####
                ######## RHEL6 #########
                case "6*":
                    set GNUPATH="/design05/CPU/armory/tool/GNU/x86-gcc/gcc-4.9.3_rhel6"
                    set PYPATH="/design05/CPU/armory/tool/Python/python2.7_rhel6"
                breaksw
                ##### end of RHEL6 #####
                ######## SAMPLE for new env #########
                # case "6*":
                #     set GNUPATH="/path/to/gnu"
                #     set PYPATH="/path/to/python"
                # breaksw
                ##### end of SAMPLE #####
                ######## Unsupported envs #########
                default:
                    echo ""
                    echo "Error: This script only supports RHEL5 and RHEL6 at REL Musashi-site."
                    echo ""
                    exit 1
                breaksw
                ######## end of Unsupported envs #########
             endsw
        endif
    breaksw

    ######## Invalid cases #########
    default:
        echo ""
        echo "Error: This script only supports RHEL5 and RHEL6 at REL Musashi-site."
        echo ""
        exit 1
    breaksw
    ##### end of Invalid cases #####
endsw

set SRCDIR="`dirname $0`"
set TARGET="$1"
switch ( "$TARGET" )
    ######## Standalone #########
    case "CFORESTG4X":
    set ENDEBUG="ON"
    set OPT="-DENABLE_ROM_W_ERROR=ON"
    breaksw
    ##### end of Standalone #####
    ######## Regression test for SC-HEAP/CS+ #########
    case "CFORESTG4X_REGRESSION":
    set TARGET="CFORESTG4X"
    set ENDEBUG="OFF"
    set OPT="-DENABLE_ROM_W_ERROR=OFF"
    breaksw
    ##### end of Standalone #####
    ######## SC-HEAP #########
    case "SCHEAP":
    set ENDEBUG="OFF"
    set OPT="-DENABLE_ROM_W_ERROR=OFF"
    breaksw
    ##### end of SC-HEAP #####
    ######## CS+ #########
    case "CSPLUS":
    set ENDEBUG="OFF"
    set OPT="-DENABLE_ROM_W_ERROR=OFF"
    breaksw
    ##### end of CS+ #####
    ######## COMPRUNNER #########
    case "COMPRUNNER":
    set ENDEBUG="ON"
    set OPT="-DENABLE_ROM_W_ERROR=ON"
    breaksw
    ##### end of COMPRUNNER #####
    ######## FROG #########
    case "FROG":
    set ENDEBUG="ON"
    set OPT="-DENABLE_ROM_W_ERROR=ON"
    breaksw
    ##### end of FROG #####
    ######## Invalid cases #########
    default:
    echo ""
    echo "Error : Invalid TARGET is specified : "$TARGET
    echo "        Valid TARGET is any of {CFORESTG4X|CFOREST_REGRESSION|SCHEAP|CSPLUS|COMPRUNNER|FROG}"
    echo ""
    echo "Usage:"
    echo "   1, Create build folder"
    echo "       $ mkdir /path/to/build-dir"
    echo "   2, Generate files by usign this script"
    echo "       $ cd /path/to/build-dir"
    echo "       $ /path/to/script/setup.csh {CFORESTG4X|CFOREST_REGRESSION|SCHEAP|CSPLUS|COMPRUNNER|FROG}"
    echo ""
    exit 1
    breaksw
    ##### end of Invalid cases #####
endsw

set echo
cmake -DCMAKE_C_COMPILER="$GNUPATH"/bin/gcc -DCMAKE_CXX_COMPILER="$GNUPATH"/bin/g++ -DPYTHON_EXECUTABLE="$PYPATH"/bin/python2.7 -DPYTHON_INCLUDE_DIR="$PYPATH"/include/python2.7 -DPYTHON_LIBRARY="$PYPATH"/lib/libpython2.7.so -DCMAKE_BUILD_TYPE=Release -DTARGET=$TARGET -DENABLE_DEBUG=$ENDEBUG -DBUILD_TEST=OFF $OPT $SRCDIR
