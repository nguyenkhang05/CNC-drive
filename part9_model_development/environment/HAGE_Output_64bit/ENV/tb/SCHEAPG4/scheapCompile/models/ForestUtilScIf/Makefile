# Copyright(c) 2016, 2018 Renesas Electronics Corporation
# RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
# This program must be used solely for the purpose for which
# it was furnished by Renesas Electronics Corporation. No part of this
# program may be reproduced or disclosed to others, in any
# form, without the prior written permission of Renesas Electronics
# Corporation.

#############################################################################
# Makefile for PseqsimIf
#############################################################################

#
# Name(overwrite from the top make)
#
MODEL			= ForestUtilScIf

#
# Location(overwrite from the top make)
#
PROJ_HOME		= $(shell pwd)/../..
LIBPATH			= lib-$(MODEL)
#
# Location(for local)
#
MODEL_HOME		= $(PROJ_HOME)/models

MODEL_FORESTUTILSCIF	= $(MODEL_HOME)/ForestUtilScIf
MODEL_CFOREST_PATH	= $(MODEL_HOME)/iss/cforestg4x/Device


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
# Linux RedHat7.3 - gcc2.96(overwrite from the top make)
#
# CXX			= /usr/bin/g++
CXX			=  /design05/CPU/armory/tool/GNU/x86-gcc/gcc-4.9.3_rhel6/bin/g++
OPTFLAG			= -m64 -g -fPIC
#OPTFLAG			= -g
DEFFLAG			= -DSUPPORT_SC_HEAP
DBGFLAG			= -Wno-deprecated
INCPATH			= -I.
#
# Linux RedHat7.3 - gcc2.96(for local)
#
DEPFLAG		= -MM
DEFFLAG_MINE	=
DBGFLAG_MINE	=
INCPATH_MINE	= -I$(MODEL_FORESTUTILSCIF) -I$(MODEL_CFOREST_PATH)/COMMON/include -I$(MODEL_CFOREST_PATH)/Utility/include -I$(MODEL_CFOREST_PATH)/Core/LSU/include -I$(MODEL_CFOREST_PATH)/Core/REG/include -I$(MODEL_CFOREST_PATH)/Core/TRACE/include/ -I$(MODEL_CFOREST_PATH)/Bus/FSBUS/include -I$(MODEL_CFOREST_PATH)/Peripheral/LATENCYINFO/include
CXXFLAGS	= $(OPTFLAG) $(DEFFLAG) $(DEFFLAG_MINE) $(DBGFLAG) $(DBGFLAG_MINE) $(INCPATH) $(INCPATH_MINE) -std=c++11 -fPIC 

SO_FLAGS = -shared -m64

#
# Files(for local)
#
SOURCES		= forest_utility_scheap.cpp
OBJECTS		= $(LIBPATH)/forest_utility_scheap.o
TARGET		= $(LIBPATH)/libForestUtilScIf.so

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

# $(TARGET): $(OBJECTS)
# 	$(AR) $@ $(OBJECTS) 2>&1 | c++filt
# 	@echo "Done"

$(TARGET): $(OBJECTS)
	$(CXX) $(SO_FLAGS) -Wl,-soname,libForestUtilScIf.so.0 -o $@ $^ -lc
	@echo "Done"

$(OBJECTS): $(SOURCES) $(SOURCES:.cpp=.d)

clean:
	find . -name "*~" -print -exec rm -f {} \;
	-$(RM) $(RM_OPT) $(SOURCES:.cpp=.d) $(OBJECTS) $(TARGET) core
	-$(RM) $(RM_OPT) $(LIBPATH)/*

#
# Dependencies
#
-include $(SOURCES:.cpp=.d)
