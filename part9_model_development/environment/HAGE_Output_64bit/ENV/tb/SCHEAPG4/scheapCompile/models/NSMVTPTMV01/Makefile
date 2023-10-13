#############################################################################
# Makefile for simple_timer
#############################################################################

#
# Name(overwrite from the top make)
#
MODEL			= NSMVTPTMV01

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
RM			= rm
RM_OPT			= -rf
AR			= ar cqs

#
# Linux RedHat7.3 - gcc2.96(overwrite from the top make)
#
CXX			= /usr/bin/g++
OPTFLAG			= -g
DEFFLAG			= 
DBGFLAG			= -Wno-deprecated
INCPATH			= -I$(SYSTEMC_INCPATH) -I$(MODEL_COMMON_PATH) -I.
#
# Linux RedHat7.3 - gcc2.96(for local)
#
DEPFLAG		= -MM
DEFFLAG_MINE	= -D_V850E2R_LOCAL_BUS_ -DLINUX -DV850E2
DBGFLAG_MINE	=
INCPATH_MINE	=
CXXFLAGS	= $(OPTFLAG) $(DEFFLAG) $(DEFFLAG_MINE) $(DBGFLAG) $(DBGFLAG_MINE) $(INCPATH) $(INCPATH_MINE)
#CXXFLAGS	= $(OPTFLAG) $(DEFFLAG) $(DEFFLAG_MINE) $(DBGFLAG) $(DBGFLAG_MINE) $(INCPATH) $(INCPATH_MINE) -fprofile-arcs -ftest-coverage -lgcov -pg 

#
# Files(for local)
#
SOURCES		= 	TPTM.cpp \
			TPTMG.cpp \
			FREE_RUN_TIMER.cpp \
			UP_COUNTER_TIMER.cpp \
			INTERVAL_TIMER.cpp \
			TPTM_OUTPUT_SIG.cpp \
			TptmFunc.cpp \
			TptmgFunc.cpp \
			NSMVTPTMV01.cpp

OBJECTS		= 	$(LIBPATH)/TPTM.o \
			$(LIBPATH)/TPTMG.o \
			$(LIBPATH)/FREE_RUN_TIMER.o \
			$(LIBPATH)/UP_COUNTER_TIMER.o \
			$(LIBPATH)/INTERVAL_TIMER.o \
			$(LIBPATH)/TPTM_OUTPUT_SIG.o \
			$(LIBPATH)/TptmFunc.o \
			$(LIBPATH)/TptmgFunc.o \
			$(LIBPATH)/NSMVTPTMV01.o 

TARGET		= $(LIBPATH)/$(MODEL).a

#
# Implicit rules
#
%.d:%.cpp
	$(CXX) $(DEPFLAG) $(CXXFLAGS) -S $< | sed 's!$*.s:!$$(LIBPATH)/&!g' > $@
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
