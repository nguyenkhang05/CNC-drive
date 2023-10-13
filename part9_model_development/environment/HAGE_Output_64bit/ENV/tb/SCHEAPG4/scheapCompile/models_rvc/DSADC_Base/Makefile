#############################################################################
# Makefile for DSADC
#############################################################################

#
# Name(overwrite from the top make)
#
MODEL					= DSADC_BASE
#
# Location(overwrite from the top make)
#
PROJ_HOME				= $(shell pwd)/../..
LIBPATH					= lib-$(MODEL)
#
# Location(for local)
#
MODEL_HOME				= $(PROJ_HOME)/models

MODEL_COMMON_PATH		= $(MODEL_HOME)/common
MODEL_COMMON_BUS_PATH	= $(MODEL_HOME)/common_bus

MODEL_RVC_HOME			= $(PROJ_HOME)/models_rvc
MODEL_DSADC_BASE_PATH   = $(MODEL_RVC_HOME)/DSADC_Base

#
# SystemC location and architecture(overwrite from the top make)
#
ifeq "$(shell uname -n)" "sdlpc567"
    SYSTEMC_HOME		= /eda_tools/systemc
else
    SYSTEMC_HOME		= /home/product/systemc/tools/systemc
endif
ifeq "$(shell uname -s)" "SunOS"
    TARGET_ARCH			= gccsparcOS5
else
    TARGET_ARCH			= linux
endif
SYSTEMC_INCPATH			= $(SYSTEMC_HOME)/include

#
# Make command(overwrite from the top make)
#
MAKE			= gmake

#
# other command(for local)
#
RM				= rm
RM_OPT			= -rf
AR				= ar cqs

#
# Linux RedHat7.3 - gcc2.96(overwrite from the top make)
#
CXX				= /usr/bin/g++
OPTFLAG			= -g
DEFFLAG			= 
DBGFLAG			= -Wno-deprecated
INCPATH			= -I$(SYSTEMC_INCPATH) -I. -I$(MODEL_COMMON_PATH) -I$(MODEL_COMMON_BUS_PATH)

#
# Linux RedHat7.3 - gcc2.96(for local)
#
DEPFLAG			= -MM
DEFFLAG_MINE	= -DREGIF_SC_REPORT
DBGFLAG_MINE	=

INCPATH_MINE	= -I$(MODEL_DSADC_BASE_PATH)

#CXXFLAGS	= $(OPTFLAG) $(DEFFLAG) $(DEFFLAG_MINE) $(DBGFLAG) $(DBGFLAG_MINE) $(INCPATH) $(INCPATH_MINE) -fprofile-arcs -ftest-coverage -fno-elide-constructors
CXXFLAGS	= $(OPTFLAG) $(DEFFLAG) $(DEFFLAG_MINE) $(DBGFLAG) $(DBGFLAG_MINE) $(INCPATH) $(INCPATH_MINE)

#
# Files(for local)
#
#SOURCES		= $(MODEL).cpp
#OBJECTS		= $(LIBPATH)/$(MODEL).o
#TARGET			= $(LIBPATH)/$(MODEL).a

SOURCES     = DSADC_UNIT.cpp \
              DSADC_UNIT_Func.cpp \
              dsadc_unit_regif.cpp 
OBJECTS     = $(LIBPATH)/DSADC_UNIT.o \
              $(LIBPATH)/DSADC_UNIT_Func.o \
              $(LIBPATH)/dsadc_unit_regif.o 
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
