#############################################################################
#
#  Copyright(c) 2013 Renesas Electronics Corporation
#  RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
#  This program must be used solely for the purpose for which
#  it was furnished by Renesas Electronics Corporation. No part of this
#  program may be reproduced or disclosed to others, in any
#  form, without the prior written permission of Renesas Electronics
#  Corporation.
#
#############################################################################

#############################################################################
# Makefile for NSMVMEVV01
#############################################################################

#
# Name(overwrite from the top make)
#
MODEL			= NSMVMEVV01

#
# Location(overwrite from the top make)
#
PROJ_HOME		= $(shell pwd)/../..
LIBPATH			= lib-$(MODEL)

#
# Location(for local)
#
MODEL_HOME		= $(PROJ_HOME)/models
MODEL_COMMON_PATH	= $(MODEL_HOME)/common
MODEL_COMMON_BUS_PATH	= $(MODEL_HOME)/common_bus
MODEL_NSMVMEVV01_PATH	= $(MODEL_HOME)/NSMVMEVV01

#
# SystemC location and architecture(overwrite from the top make)
#
SYSTEMC_HOME		?= /proj/soft109/HeapE3/tools/systemc-2.3.0_lnxe5_gcc412
TARGET_ARCH		= linux
SYSTEMC_INCPATH		= $(SYSTEMC_HOME)/src

#
# Make command(overwrite from the top make)
#
MAKE			= gmake

#
# other command(for local)
#
RM			= rm
RM_OPT			= -rf
AR			= ar cqs

#
# overwrite from the top make
#
CXX			= /usr/bin/g++
OPTFLAG			= -m32 -g
DEFFLAG			= -DSC_INCLUDE_DYNAMIC_PROCESSES
DBGFLAG			= -Wno-deprecated
INCPATH			= -I$(SYSTEMC_INCPATH) -I. -I$(MODEL_COMMON_PATH) -I$(MODEL_COMMON_BUS_PATH)

#
# for local
#
DEPFLAG			= -MM
DEFFLAG_MINE		= -DSUPPORT_SC_HEAP
DBGFLAG_MINE		=

INCPATH_MINE		= -I$(MODEL_NSMVMEVV01_PATH)
CXXFLAGS		= $(OPTFLAG) $(DEFFLAG) $(DEFFLAG_MINE) $(DBGFLAG) $(DBGFLAG_MINE) $(INCPATH) $(INCPATH_MINE)

#
# Files(for local)
#
SOURCES			= $(MODEL).cpp \
			  MevAdptr.cpp \
			  MevArbtr.cpp \
			  G0Mev.cpp \
			  G0MevFunc.cpp
OBJECTS			= $(LIBPATH)/$(MODEL).o \
			  $(LIBPATH)/MevAdptr.o \
			  $(LIBPATH)/MevArbtr.o \
			  $(LIBPATH)/G0Mev.o \
			  $(LIBPATH)/G0MevFunc.o
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
	if test ! -d $(LIBPATH); then mkdir $(LIBPATH); fi;
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
