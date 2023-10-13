#############################################################################
# Makefile for ATU5
#############################################################################

#
# Name(overwrite from the top make)
#
MODEL					= ATU5_BASE

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
#MODEL_ATU5_PATH			= $(MODEL_RVC_HOME)/ATU5
#MODEL_ATU5010_PATH			= $(MODEL_RVC_HOME)/ATU5010
MODEL_ATU5_BASE_PATH            = $(MODEL_RVC_HOME)/ATU5_Base
MODEL_ATU5_TIMERA_PATH			= $(MODEL_ATU5_BASE_PATH)/TIMERA
MODEL_ATU5_TIMERB_PATH			= $(MODEL_ATU5_BASE_PATH)/TIMERB
MODEL_ATU5_TIMERC_PATH			= $(MODEL_ATU5_BASE_PATH)/TIMERC
MODEL_ATU5_TIMERD_PATH			= $(MODEL_ATU5_BASE_PATH)/TIMERD
MODEL_ATU5_TIMERE_PATH			= $(MODEL_ATU5_BASE_PATH)/TIMERE
MODEL_ATU5_TIMERF_PATH			= $(MODEL_ATU5_BASE_PATH)/TIMERF
MODEL_ATU5_TIMERG_PATH			= $(MODEL_ATU5_BASE_PATH)/TIMERG

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

INCPATH_MINE	= -I$(MODEL_ATU5_BASE_PATH)/TIMERA -I$(MODEL_ATU5_BASE_PATH)/TIMERB -I$(MODEL_ATU5_BASE_PATH)/TIMERC -I$(MODEL_ATU5_BASE_PATH)/TIMERD -I$(MODEL_ATU5_BASE_PATH)/TIMERE -I$(MODEL_ATU5_BASE_PATH)/TIMERF -I$(MODEL_ATU5_BASE_PATH)/TIMERG -I$(MODEL_ATU5_BASE_PATH)

#CXXFLAGS	= $(OPTFLAG) $(DEFFLAG) $(DEFFLAG_MINE) $(DBGFLAG) $(DBGFLAG_MINE) $(INCPATH) $(INCPATH_MINE) -fprofile-arcs -ftest-coverage -fno-elide-constructors
CXXFLAGS	= $(OPTFLAG) $(DEFFLAG) $(DEFFLAG_MINE) $(DBGFLAG) $(DBGFLAG_MINE) $(INCPATH) $(INCPATH_MINE)

#
# Files(for local)
#
#SOURCES		= $(MODEL).cpp
#OBJECTS		= $(LIBPATH)/$(MODEL).o
#TARGET			= $(LIBPATH)/$(MODEL).a

SOURCES     = ATU5_Timer.cpp \
              TIMERA/ATU5_TIMERA.cpp \
              TIMERA/ATU5_TIMERA_Func.cpp \
              TIMERA/atu5_timera_regif.cpp \
              TIMERB/ATU5_TIMERB.cpp \
              TIMERB/ATU5_TIMERB_Func.cpp \
              TIMERB/atu5_timerb_regif.cpp \
              TIMERC/ATU5_TIMERC.cpp \
              TIMERC/ATU5_TIMERC_Func.cpp \
              TIMERC/atu5_timerc_regif.cpp \
              TIMERC/ATU5_TIMERC_SUB.cpp \
              TIMERC/ATU5_TIMERC_SUB_Func.cpp \
              TIMERC/atu5_timerc_sub_regif.cpp \
              TIMERD/ATU5_TIMERD.cpp \
              TIMERD/ATU5_TIMERD_Func.cpp \
              TIMERD/atu5_timerd_regif.cpp \
              TIMERD/ATU5_TIMERD_SUB.cpp \
              TIMERD/ATU5_TIMERD_SUB_Func.cpp \
              TIMERD/atu5_timerd_sub_regif.cpp \
              TIMERE/ATU5_TIMERE.cpp \
              TIMERE/ATU5_TIMERE_Func.cpp \
              TIMERE/atu5_timere_regif.cpp \
              TIMERE/ATU5_TIMERE_SUB.cpp \
              TIMERE/ATU5_TIMERE_SUB_Func.cpp \
              TIMERE/atu5_timere_sub_regif.cpp \
              TIMERF/ATU5_TIMERF.cpp \
              TIMERF/ATU5_TIMERF_Func.cpp \
              TIMERF/atu5_timerf_regif.cpp \
              TIMERF/ATU5_TIMERF_Sub.cpp \
              TIMERF/ATU5_TIMERF_Sub_Func.cpp \
              TIMERF/atu5_timerf_sub_regif.cpp \
              TIMERG/ATU5_TIMERG.cpp \
              TIMERG/ATU5_TIMERG_Func.cpp \
              TIMERG/ATU5_TIMERG_Func_Timers.cpp \
              TIMERG/atu5_timerg_regif.cpp 
OBJECTS     = $(LIBPATH)/ATU5_Timer.o \
              $(LIBPATH)/ATU5_TIMERA.o \
              $(LIBPATH)/ATU5_TIMERA_Func.o \
              $(LIBPATH)/atu5_timera_regif.o \
              $(LIBPATH)/ATU5_TIMERB.o \
              $(LIBPATH)/ATU5_TIMERB_Func.o \
              $(LIBPATH)/atu5_timerb_regif.o \
              $(LIBPATH)/ATU5_TIMERC.o \
              $(LIBPATH)/ATU5_TIMERC_Func.o \
              $(LIBPATH)/atu5_timerc_regif.o \
              $(LIBPATH)/ATU5_TIMERC_SUB.o \
              $(LIBPATH)/ATU5_TIMERC_SUB_Func.o \
              $(LIBPATH)/atu5_timerc_sub_regif.o \
              $(LIBPATH)/ATU5_TIMERD.o \
              $(LIBPATH)/ATU5_TIMERD_Func.o \
              $(LIBPATH)/atu5_timerd_regif.o \
              $(LIBPATH)/ATU5_TIMERD_SUB.o \
              $(LIBPATH)/ATU5_TIMERD_SUB_Func.o \
              $(LIBPATH)/atu5_timerd_sub_regif.o \
              $(LIBPATH)/ATU5_TIMERE.o \
              $(LIBPATH)/ATU5_TIMERE_Func.o \
              $(LIBPATH)/atu5_timere_regif.o \
              $(LIBPATH)/ATU5_TIMERE_SUB.o \
              $(LIBPATH)/ATU5_TIMERE_SUB_Func.o \
              $(LIBPATH)/atu5_timere_sub_regif.o \
              $(LIBPATH)/ATU5_TIMERF.o \
              $(LIBPATH)/ATU5_TIMERF_Func.o \
              $(LIBPATH)/atu5_timerf_regif.o \
              $(LIBPATH)/ATU5_TIMERF_Sub.o \
              $(LIBPATH)/ATU5_TIMERF_Sub_Func.o \
              $(LIBPATH)/atu5_timerf_sub_regif.o \
              $(LIBPATH)/ATU5_TIMERG.o \
              $(LIBPATH)/ATU5_TIMERG_Func.o \
              $(LIBPATH)/ATU5_TIMERG_Func_Timers.o \
              $(LIBPATH)/atu5_timerg_regif.o 
TARGET		= $(LIBPATH)/$(MODEL).a

#
# Implicit rules
#
%.d:%.cpp
	$(CXX) $(DEPFLAG) $(CXXFLAGS) $< | sed 's!$*.o:!$$(LIBPATH)/&!g' > $@
$(LIBPATH)/%.o:%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<
$(LIBPATH)/%.o:TIMERA/%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<
$(LIBPATH)/%.o:TIMERB/%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<
$(LIBPATH)/%.o:TIMERC/%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<
$(LIBPATH)/%.o:TIMERD/%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<
$(LIBPATH)/%.o:TIMERE/%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<
$(LIBPATH)/%.o:TIMERF/%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<
$(LIBPATH)/%.o:TIMERG/%.cpp
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
