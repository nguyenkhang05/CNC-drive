# Copyright(c) 2016 Renesas Electronics Corporation
# RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
# This program must be used solely for the purpose for which
# it was furnished by Renesas Electronics Corporation. No part of this
# program may be reproduced or disclosed to others, in any
# form, without the prior written permission of Renesas Electronics
# Corporation.

#############################################################################
# Makefile for NSMVG4SSV01
#############################################################################

#
# Name(overwrite from the top make)
#
MODEL			= NSMVG4SSV01

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
MODEL_NSMVG4PEV01_PATH		= $(MODEL_HOME)/NSMVG4PEV01
MODEL_NSMVINTC1V02_PATH		= $(MODEL_HOME)/NSMVINTC1V02
MODEL_NSMVINTC2V02_PATH		= $(MODEL_HOME)/NSMVINTC2V02

MODEL_ATLTSLAVE32_PATH		= $(MODEL_HOME)/ATLTSLAVE32
MODEL_VPI2APB_PATH		= $(MODEL_HOME)/VPI2APB
MODEL_VCI2AHB_PATH		= $(MODEL_HOME)/VCI2AHB
MODEL_AHB2VCI_PATH		= $(MODEL_HOME)/AHB2VCI
MODEL_ATLTLB32_PATH		= $(MODEL_HOME)/ATLTLB32
MODEL_ATLTLB64_PATH		= $(MODEL_HOME)/ATLTLB64
MODEL_COMMON_BUS_PATH		= $(MODEL_HOME)/common_bus

#
# SystemC location and architecture(overwrite from the top make)
#
SYSTEMC_HOME		= /proj/soft109/HeapE3/tools/systemc-2.2.0_lnxe5_gcc412
TARGET_ARCH		= linux
SYSTEMC_INCPATH		= $(SYSTEMC_HOME)/include

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
# Options(overwrite from the top make)
#
CXX			= /usr/bin/g++
OPTFLAG			= -g
DEFFLAG			= 
DBGFLAG			= -Wno-deprecated
INCPATH			= -I$(SYSTEMC_INCPATH) -I. -I$(MODEL_COMMON_PATH)

#
# Options(for local)
#
DEPFLAG		= -MM
DEFFLAG_MINE	= 
#DEFFLAG_MINE	= -DTEST_ISS
#DBGFLAG_MINE	= -DDEBUG_MESSAGE_OUTPUT

INCPATH_MINE	= \
		-I$(MODEL_NSMVG4PEV01_PATH) -I$(MODEL_COMMON_BUS_PATH) -I$(MODEL_NSMVINTC1V02_PATH) -I$(MODEL_NSMVINTC2V02_PATH) -I$(MODEL_ATLTSLAVE32_PATH) -I$(MODEL_VPI2APB_PATH) -I$(MODEL_VCI2AHB_PATH) -I$(MODEL_AHB2VCI_PATH) -I$(MODEL_ATLTLB32_PATH) -I$(MODEL_ATLTLB64_PATH)

CXXFLAGS	= $(OPTFLAG) $(DEFFLAG) $(DEFFLAG_MINE) $(DBGFLAG) $(DBGFLAG_MINE) $(INCPATH) $(INCPATH_MINE)

#
# Files(for local)
#
SOURCES		= $(MODEL).cpp
OBJECTS		= $(LIBPATH)/$(MODEL).o
TARGET		= $(LIBPATH)/$(MODEL).a

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
