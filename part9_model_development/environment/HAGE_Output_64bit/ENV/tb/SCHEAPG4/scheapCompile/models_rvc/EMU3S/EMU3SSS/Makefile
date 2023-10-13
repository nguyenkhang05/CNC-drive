# Copyright(c) 2020 Renesas Electronics Corporation
# RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
# This program must be used solely for the purpose for which
# it was furnished by Renesas Electronics Corporation. No part of this
# program may be reproduced or disclosed to others, in any
# form, without the prior written permission of Renesas Electronics
# Corporation.

#############################################################################
# Makefile for EMU3SSS
#############################################################################

#
# Name(overwrite from the top make)
#
MODEL			= EMU3SSS

#
# Location(overwrite from the top make)
#
PROJ_HOME       = $(shell pwd)/../..
LIBPATH         = lib-$(MODEL)
#
# Location(for local)
#
MODEL_HOME				= $(PROJ_HOME)/models
MODEL_RVC_HOME			= $(PROJ_HOME)/models_rvc

MODEL_COMMON_PATH		= $(MODEL_HOME)/common
MODEL_COMMON_BUS_PATH	= $(MODEL_HOME)/common_bus


MODEL_EMU3SSS_PATH		= $(MODEL_RVC_HOME)/EMU3S/EMU3SSS
MODEL_COMMON_RVC_PATH	= $(MODEL_RVC_HOME)/COMMON_RVC

#
# SystemC location and architecture(overwrite from the top make)
#
ifeq "$(shell uname -n)" "sdlpc567"
    SYSTEMC_HOME			= /eda_tools/systemc
else
    SYSTEMC_HOME			= /home/product/systemc/tools/systemc
endif
ifeq "$(shell uname -s)" "SunOS"
    TARGET_ARCH				= gccsparcOS5
else
    TARGET_ARCH				= linux
endif
SYSTEMC_INCPATH		= /shsv/MobAP2/prj_MCS/01_common/01_SystemC/systemc-2.3.1a_gcc493_64bit/include

#
# Make command(overwrite from the top make)
#
MAKE					= gmake

#
# other command(for local)
#
RM						= rm
RM_OPT					= -rf
AR						= ar cqs

#
# Linux RedHat7.3 - gcc2.96(overwrite from the top make)
#
CXX				= /shsv/MobAP2/prj_MCS/01_common/04_gcc/gcc-4.9.3-64bit/bin/g++
OPTFLAG			= -m64 -O3
DEFFLAG			= -DLINUX_DEF -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -DIS_RESET_ACTIVE_LOW -DREGIF_SC_REPORT -DSCHEAP_SCVER= -DSC_INCLUDE_DYNAMIC_PROCESSES
DBGFLAG			= -DNDEBUG
INCPATH			= -I$(SYSTEMC_INCPATH) -I. -I$(MODEL_COMMON_PATH) -I$(MODEL_COMMON_BUS_PATH) \
				-I$(MODEL_COMMON_RVC_PATH)

#
# Linux RedHat7.3 - gcc2.96(for local)
#
DEPFLAG				= -MM
DEFFLAG_MINE		= -DREGIF_SC_REPORT
DBGFLAG_MINE		=

INCPATH_MINE		= -I$(MODEL_EMU3SSS_PATH) -I$(MODEL_COMMON_PATH)

CXXFLAGS	= $(OPTFLAG) $(DEFFLAG) $(DBGFLAG) $(DBGFLAG_MINE) $(INCPATH) $(INCPATH_MINE)


SOURCES			= 	EMU3SSS.cpp \
					EMU3S_AHB_ARB.cpp \
					EMU3S_AHB_ARB_Func.cpp \
					EMU3S_APB_DEC.cpp \
					EMU3S_APB_DEC_Func.cpp \

OBJECTS	    	= 	$(LIBPATH)/EMU3SSS.o \
              		$(LIBPATH)/EMU3S_AHB_ARB.o \
              		$(LIBPATH)/EMU3S_AHB_ARB_Func.o \
              		$(LIBPATH)/EMU3S_APB_DEC.o \
              		$(LIBPATH)/EMU3S_APB_DEC_Func.o \

TARGET			= $(LIBPATH)/$(MODEL).a

#
# Implicit rules
#
%.d:%.cpp
	$(CXX) $(DEPFLAG) $(CXXFLAGS) $< | sed 's!$*.o:!$$(LIBPATH)/&!g' > $@
$(LIBPATH)/%.o:%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

#
# Build rules
#
.PHONY : all compile clean
all:
	if test ! -d $(LIBPATH); then mkdir -p $(LIBPATH); fi;
	$(MAKE) compile

compile: $(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) $@ $(OBJECTS) 2>&1 | c++filt
	@echo "Done"

$(OBJECTS): $(SOURCES) $(SOURCES:.cpp=.d)

clean:
	find . -name "*~" -print -exec rm -f {} \;
	-$(RM) $(RM_OPT) $(SOURCES:.cpp=.d) $(OBJECTS) $(TARGET) core
	-$(RM) $(RM_OPT) $(LIBPATH)

#
# Dependencies
#
-include $(SOURCES:.cpp=.d)
