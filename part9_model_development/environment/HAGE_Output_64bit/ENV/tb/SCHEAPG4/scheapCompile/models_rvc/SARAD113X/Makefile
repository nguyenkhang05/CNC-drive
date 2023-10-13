#############################################################################
# Makefile for ots
#############################################################################

#
# Name(overwrite from the top make)
#
MODEL			= SARAD113x

#
# Location(overwrite from the top make)
#
PROJ_HOME		= $(shell pwd)/../..
LIBPATH			= lib-$(MODEL)
#
# Location(for local)
#
#MODEL_HOME				= $(PROJ_HOME)/models

MODEL_COMMON_PATH		= $(MODEL_HOME)/common

MODEL_RVC_HOME			= $(PROJ_HOME)/models_rvc
MODEL_SARAD113x_PATH			= $(MODEL_RVC_HOME)/SARAD113x

#
# SystemC location and architecture(overwrite from the top make)
#
ifeq "$(shell uname -n)" "sdlpc567"
    SYSTEMC_HOME	= /eda_tools/systemc
else
    SYSTEMC_HOME	= /home/product/systemc/tools/systemc
endif
ifeq "$(shell uname -s)" "SunOS"
    TARGET_ARCH		= gccsparcOS5
else
    TARGET_ARCH		= linux
endif
SYSTEMC_INCPATH		= $(SYSTEMC_HOME)/include

#
# Make command(overwrite from the top make)
#
MAKE			= gmake

#
# other command(for local)
#
RM				= rm
RM_OPT				= -rf
AR				= ar cqs

#
# Linux RedHat7.3 - gcc2.96(overwrite from the top make)
#
CXX			= /usr/bin/g++
OPTFLAG			= -g
DEFFLAG			= 
DBGFLAG			= -Wno-deprecated
INCPATH			= -I$(SYSTEMC_INCPATH) -I. -I$(MODEL_COMMON_PATH)

#
# Linux RedHat7.3 - gcc2.96(for local)
#
DEPFLAG		= -MM
DEFFLAG_MINE	= 
DBGFLAG_MINE	=

INCPATH_MINE	= \
		-I$(MODEL_SARAD113x_PATH)

CXXFLAGS	= $(OPTFLAG) $(DEFFLAG) $(DEFFLAG_MINE) $(DBGFLAG) $(DBGFLAG_MINE) $(INCPATH) $(INCPATH_MINE)

#
# Files(for local)
#
#SOURCES		= $(MODEL).cpp
#OBJECTS		= $(LIBPATH)/$(MODEL).o
#TARGET		= $(LIBPATH)/$(MODEL).a

SOURCES		= sarad113x.cpp  sarad113x_regif.cpp sarad113x_fsm.cpp sarad113x_tha_conv_fsm.cpp sarad113x_thb_conv_fsm.cpp
OBJECTS		= $(LIBPATH)/sarad113x.o $(LIBPATH)/sarad113x_regif.o $(LIBPATH)/sarad113x_fsm.o $(LIBPATH)/sarad113x_tha_conv_fsm.o $(LIBPATH)/sarad113x_thb_conv_fsm.o
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
