#############################################################################
# Makefile for NSMVG4PEV01
#############################################################################

#
# Name(overwrite from the top make)
#
MODEL			= NSMVG4PEV01

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
MODEL_NSMVG4PEV01_PATH		= $(MODEL_HOME)/NSMVG4PEV01
MODEL_NSMVINTC1V02_PATH		= $(MODEL_HOME)/NSMVINTC1V02

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
DBGFLAG_MINE	=

INCPATH_MINE	= \
		-I$(MODEL_NSMVG4PEV01_PATH) -I$(MODEL_NSMVINTC1V02_PATH) -I$(MODEL_COMMON_BUS_PATH)

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
