#############################################################################
#
#  Copyright(c) 2016-2020 Renesas Electronics Corporation
#  RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
#  This program must be used solely for the purpose for which
#  it was furnished by Renesas Electronics Corporation. No part of this
#  program may be reproduced or disclosed to others, in any
#  form, without the prior written permission of Renesas Electronics
#  Corporation.
#
#############################################################################

#############################################################################
# Makefile for NSMVINTC1V02
#############################################################################

#
# Name(overwrite from the top make)
#
MODEL			= NSMVINTC1V02

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
MODEL_CFOREST_PATH	= $(MODEL_HOME)/iss/cforestg4x/Device
MODEL_LIBAPI_PATH	= $(MODEL_HOME)/iss/cforest_g4x/sim/libapi
MODEL_NSMVINTC1V02_PATH	= $(MODEL_HOME)/NSMVINTC1V02

#
# SystemC location and architecture(overwrite from the top make)
#
SYSTEMC_HOME		?= /proj/soft109/HeapE3/tools/systemc-2.3.0_lnxe5_gcc412
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
# overwrite from the top make
#
CXX			= /usr/bin/g++
OPTFLAG			= -g
DEFFLAG			=
DBGFLAG			= -Wno-deprecated
INCPATH			= -I$(SYSTEMC_INCPATH) -I. -I$(MODEL_COMMON_PATH)

#
# for local
#
DEPFLAG			= -MM
DEFFLAG_MINE		= -DSUPPORT_SC_HEAP
DBGFLAG_MINE		=

INCPATH_MINE		= -I$(MODEL_NSMVINTC1V02_PATH) \
			  -I$(MODEL_CFOREST_PATH)/COMMON/include \
			  -I$(MODEL_CFOREST_PATH)/Peripheral/INTC/include \
			  -I$(MODEL_CFOREST_PATH)/Peripheral/INTC2/include \
			  -I$(MODEL_CFOREST_PATH)/Peripheral/TARGETREG/include \
			  -I$(MODEL_CFOREST_PATH)/Utility/include \
			  -I$(MODEL_CFOREST_PATH)/Core/PE/include \
			  -I$(MODEL_CFOREST_PATH)/Core/REG/include \
			  -I$(MODEL_CFOREST_PATH)/Core/LSU/include \
			  -I$(MODEL_CFOREST_PATH)/Core/TRACE/include \
			  -I$(MODEL_LIBAPI_PATH)
CXXFLAGS		= $(OPTFLAG) \
			  $(DEFFLAG) \
			  $(DEFFLAG_MINE) \
			  $(DBGFLAG) \
			  $(DBGFLAG_MINE) \
			  $(INCPATH) \
			  $(INCPATH_MINE)

#
# Files(for local)
#
SOURCES			= $(MODEL).cpp \
			  G4_Intc1Func.cpp \
			  G4_Intc1FuncUsr.cpp \
			  G4_Intc1Wrapper.cpp
OBJECTS			= $(LIBPATH)/$(MODEL).o \
			  $(LIBPATH)/G4_Intc1Func.o \
			  $(LIBPATH)/G4_Intc1FuncUsr.o \
			  $(LIBPATH)/G4_Intc1Wrapper.o
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
