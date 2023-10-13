# Copyright(c) 2020 Renesas Electronics Corporation
# RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
# This program must be used solely for the purpose for which
# it was furnished by Renesas Electronics Corporation. No part of this
# program may be reproduced or disclosed to others, in any
# form, without the prior written permission of Renesas Electronics
# Corporation.

#############################################################################
# Makefile for HWCORE_PI
#############################################################################

#
# Name(overwrite from the top make)
#
MODEL			= HWCORE_PI

#
# Location(overwrite from the top make)
#
PROJ_HOME		= $(shell pwd)/../..
LIBPATH			= lib-$(MODEL)
#
# Location(for local)
#
MODEL_HOME			= $(PROJ_HOME)/models

MODEL_COMMON_PATH		= $(MODEL_HOME)/common
MODEL_COMMON_BUS_PATH		= $(MODEL_HOME)/common_bus

MODEL_RVC_HOME			= $(PROJ_HOME)/models_rvc
MODEL_HWCORE_PI_PATH		= $(MODEL_RVC_HOME)/EMU3S/EMU3S_HWCORE/HWCORE_PI

#
# SystemC location and architecture(overwrite from the top make)
#
ifeq "$(shell uname -n)" "sdlpc567"
    SYSTEMC_HOME        = /eda_tools/systemc
else
    SYSTEMC_HOME        = /home/product/systemc/tools/systemc
endif
ifeq "$(shell uname -s)" "SunOS"
    TARGET_ARCH         = gccsparcOS5
else
    TARGET_ARCH         = linux
endif
SYSTEMC_INCPATH         = $(SYSTEMC_HOME)/include

#
# Make command(overwrite from the top make)
#
MAKE            = gmake

#
# other command(for local)
#
RM              = rm
RM_OPT          = -rf
AR              = ar cqs

#
# Linux RedHat7.3 - gcc2.96(overwrite from the top make)
#
CXX             = /usr/bin/g++
OPTFLAG         = -g
DEFFLAG         = 
DBGFLAG         = -Wno-deprecated
INCPATH         = -I$(SYSTEMC_INCPATH) -I. -I$(MODEL_COMMON_PATH) -I$(MODEL_COMMON_BUS_PATH)

#
# Linux RedHat7.3 - gcc2.96(for local)
#
DEPFLAG         = -MM
DEFFLAG_MINE    = -DREGIF_SC_REPORT
DBGFLAG_MINE    =

INCPATH_MINE    = \
        -I$(MODEL_HWCORE_PI_PATH)

CXXFLAGS    = $(OPTFLAG) $(DEFFLAG) $(DEFFLAG_MINE) $(DBGFLAG) $(DBGFLAG_MINE) $(INCPATH) $(INCPATH_MINE)


SOURCES     = HWCORE_PI.cpp \
              HWCORE_PI_Func.cpp \
              hwcore_pi_regif.cpp 

OBJECTS     = $(LIBPATH)/HWCORE_PI.o \
              $(LIBPATH)/HWCORE_PI_Func.o \
              $(LIBPATH)/hwcore_pi_regif.o 

TARGET      = $(LIBPATH)/$(MODEL).a

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
