#!/bin/csh 

setenv LANG C
setenv TOOLS                 C:/Users/khang.duong-nguyen/Desktop/mcu/common/appl/Multi_v6-02/comp_201251
setenv SCHEAP_PYTHON_HOME    C:/Users/khang.duong-nguyen/Desktop/mcu/common/appl/python/python-2.7.3-64bit
setenv PYTHON_VERSION  2.7
#Renesas network
setenv RLM_LICENSE           5053@licedu3
#B2B
#setenv RLM_LICENSE          52293@172.29.138.155
setenv USER_DIR              `pwd | /bin/sed -e "s/scripts_linux\/run_all//"`
setenv SCHEAP_HOME           ${USER_DIR}tb/SCHEAPG4

setenv DUMMYINS_PATH         C:/Users/khang.duong-nguyen/Desktop/mcu/common/appl/Renesas/SystemC/utility/coverage/

# MULTI 
setenv MULTI_LOCATION  /common/appl/Multi_v6-02/multi_614
setenv MULTI_SERVER_LOCATION  ${TOOLS}

# OSCI
set OSCI_HOME=/shsv/MobAP2/prj_MCS/01_common/01_SystemC/systemc-2.3.1a_gcc493_64bit
set BOOST_OSCI_ROOT = "${OSCI_HOME}/include/sysc/packages"
set OSCI_INC_DIR    = "${OSCI_HOME}/include"
set OSCI_LIB_DIR    = "${OSCI_HOME}/lib-linux64"
set OSCI_TLM_DIR    = "${OSCI_HOME}/include"

# USK
setenv OSCAR_HOME       "C:/Users/khang.duong-nguyen/Desktop/mcu/common/appl/ASTC/Vlab_2.5.16-64bit"
setenv USK_HOME         "${OSCAR_HOME}"
set    BOOST_USK_ROOT = "${OSCAR_HOME}/include"
setenv USK_INC_DIR      "${OSCAR_HOME}/include"
setenv USK_LIB_DIR      "${OSCAR_HOME}/lib"
setenv USK_TLM_DIR      "${OSCAR_HOME}/include"

# common SystemC (for OSCI as of now)
setenv SYSTEMC_HOME ${OSCI_HOME}
setenv BOOST_ROOT ${BOOST_OSCI_ROOT}
setenv SYSTEMC_INCPATH ${OSCI_INC_DIR}
setenv SYSTEMC_LIBPATH ${OSCI_LIB_DIR}
setenv TLM_INC_DIR ${OSCI_TLM_DIR}
setenv SEMANTIC_INCPATH ${OSCAR_HOME}/include/semantic

setenv GCC493_PATH      "/shsv/MobAP2/prj_MCS/01_common/04_gcc/gcc-4.9.3-64bit"
setenv GCC493_LIBPATH   "${GCC493_PATH}/lib64"
setenv CXX              "${GCC493_PATH}/bin/g++"

setenv LD_LIBRARY_PATH ""
setenv LD_LIBRARY_PATH "${SCHEAP_HOME}/scheapCompile/models/iss/cforestg4x/libforestg4x_scheap/lib":"$LD_LIBRARY_PATH"
setenv LD_LIBRARY_PATH "${SCHEAP_HOME}/scheapCompile/lib/osci-linux-gcc49-x64":"$LD_LIBRARY_PATH"
setenv LD_LIBRARY_PATH "${SCHEAP_HOME}/scheapCompile/lib":"$LD_LIBRARY_PATH"
setenv LD_LIBRARY_PATH "${SYSTEMC_LIBPATH}":"$LD_LIBRARY_PATH"
setenv LD_LIBRARY_PATH "${GCC493_LIBPATH}":"$LD_LIBRARY_PATH"

setenv VALGRIND_PATH "C:/Users/khang.duong-nguyen/Desktop/mcu/common/appl/Valgrind/valgrind-3.11.0"

setenv PATH "${VALGRIND_PATH}/bin":"${GCC493_PATH}/bin":"$PATH"

######Test pattern compilation#################
setenv GHS_LMWHICH ghs
setenv GHS_LMHOST 172.29.139.145
setenv GHS_LMPORT 2017
