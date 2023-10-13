#############################################################################
# Makefile for ATU5010
#############################################################################

#
# Name(overwrite from the top make)
#
MODEL					= ATU5010

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
MODEL_ATU5010_PATH			= $(MODEL_RVC_HOME)/ATU5010
MODEL_ATU5010_TIMERA_PATH			= $(MODEL_RVC_HOME)/ATU5010/TIMERA
MODEL_ATU5010_TIMERB_PATH			= $(MODEL_RVC_HOME)/ATU5010/TIMERB
MODEL_ATU5010_TIMERC_PATH			= $(MODEL_RVC_HOME)/ATU5010/TIMERC
MODEL_ATU5010_TIMERD_PATH			= $(MODEL_RVC_HOME)/ATU5010/TIMERD
MODEL_ATU5010_TIMERE_PATH			= $(MODEL_RVC_HOME)/ATU5010/TIMERE
MODEL_ATU5010_TIMERF_PATH			= $(MODEL_RVC_HOME)/ATU5010/TIMERF
MODEL_ATU5010_TIMERG_PATH			= $(MODEL_RVC_HOME)/ATU5010/TIMERG

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

INCPATH_MINE	= -I./TIMERA -I./TIMERB -I./TIMERC -I./TIMERD -I./TIMERE -I./TIMERF -I./TIMERG -I$(MODEL_ATU5010_PATH)

#CXXFLAGS	= $(OPTFLAG) $(DEFFLAG) $(DEFFLAG_MINE) $(DBGFLAG) $(DBGFLAG_MINE) $(INCPATH) $(INCPATH_MINE) -fprofile-arcs -ftest-coverage -fno-elide-constructors
CXXFLAGS	= $(OPTFLAG) $(DEFFLAG) $(DEFFLAG_MINE) $(DBGFLAG) $(DBGFLAG_MINE) $(INCPATH) $(INCPATH_MINE)

#
# Files(for local)
#
#SOURCES		= $(MODEL).cpp
#OBJECTS		= $(LIBPATH)/$(MODEL).o
#TARGET			= $(LIBPATH)/$(MODEL).a

SOURCES     = ATU5010.cpp \
              ATU5010_Func.cpp \
              atu5010_regif.cpp \
              TIMERA/ATU5010_TIMERA.cpp \
              TIMERA/ATU5010_TIMERA_Func.cpp \
              TIMERA/atu5010_timera_regif.cpp \
              TIMERB/ATU5010_TIMERB.cpp \
              TIMERB/ATU5010_TIMERB_Func.cpp \
              TIMERB/atu5010_timerb_regif.cpp \
              TIMERC/ATU5010_TIMERC.cpp \
              TIMERC/ATU5010_TIMERC_Func.cpp \
              TIMERC/atu5010_timerc_regif.cpp \
              TIMERC/ATU5010_TIMERC_SUB.cpp \
              TIMERC/ATU5010_TIMERC_SUB_Func.cpp \
              TIMERC/atu5010_timerc_sub_regif.cpp \
              TIMERD/ATU5010_TIMERD.cpp \
              TIMERD/ATU5010_TIMERD_Func.cpp \
              TIMERD/atu5010_timerd_regif.cpp \
              TIMERD/ATU5010_TIMERD_SUB.cpp \
              TIMERD/ATU5010_TIMERD_SUB_Func.cpp \
              TIMERD/atu5010_timerd_sub_regif.cpp \
              TIMERE/ATU5010_TIMERE.cpp \
              TIMERE/ATU5010_TIMERE_Func.cpp \
              TIMERE/atu5010_timere_regif.cpp \
              TIMERE/ATU5010_TIMERE_SUB.cpp \
              TIMERE/ATU5010_TIMERE_SUB_Func.cpp \
              TIMERE/atu5010_timere_sub_regif.cpp \
              TIMERF/ATU5010_TIMERF.cpp \
              TIMERF/ATU5010_TIMERF_Func.cpp \
              TIMERF/atu5010_timerf_regif.cpp \
              TIMERF/ATU5010_TIMERF_Sub.cpp \
              TIMERF/ATU5010_TIMERF_Sub_Func.cpp \
              TIMERF/atu5010_timerf_sub_regif.cpp \
              TIMERG/ATU5010_TIMERG.cpp \
              TIMERG/ATU5010_TIMERG_Func.cpp \
              TIMERG/ATU5010_TIMERG_Func_Timers.cpp \
              TIMERG/atu5010_timerg_regif.cpp \
              ATU5010_Timer.cpp
OBJECTS     = $(LIBPATH)/ATU5010.o \
              $(LIBPATH)/ATU5010_Func.o \
              $(LIBPATH)/atu5010_regif.o \
              $(LIBPATH)/ATU5010_TIMERA.o \
              $(LIBPATH)/ATU5010_TIMERA_Func.o \
              $(LIBPATH)/atu5010_timera_regif.o \
              $(LIBPATH)/ATU5010_TIMERB.o \
              $(LIBPATH)/ATU5010_TIMERB_Func.o \
              $(LIBPATH)/atu5010_timerb_regif.o \
              $(LIBPATH)/ATU5010_TIMERC.o \
              $(LIBPATH)/ATU5010_TIMERC_Func.o \
              $(LIBPATH)/atu5010_timerc_regif.o \
              $(LIBPATH)/ATU5010_TIMERC_SUB.o \
              $(LIBPATH)/ATU5010_TIMERC_SUB_Func.o \
              $(LIBPATH)/atu5010_timerc_sub_regif.o \
              $(LIBPATH)/ATU5010_TIMERD.o \
              $(LIBPATH)/ATU5010_TIMERD_Func.o \
              $(LIBPATH)/atu5010_timerd_regif.o \
              $(LIBPATH)/ATU5010_TIMERD_SUB.o \
              $(LIBPATH)/ATU5010_TIMERD_SUB_Func.o \
              $(LIBPATH)/atu5010_timerd_sub_regif.o \
              $(LIBPATH)/ATU5010_TIMERE.o \
              $(LIBPATH)/ATU5010_TIMERE_Func.o \
              $(LIBPATH)/atu5010_timere_regif.o \
              $(LIBPATH)/ATU5010_TIMERE_SUB.o \
              $(LIBPATH)/ATU5010_TIMERE_SUB_Func.o \
              $(LIBPATH)/atu5010_timere_sub_regif.o \
              $(LIBPATH)/ATU5010_TIMERF.o \
              $(LIBPATH)/ATU5010_TIMERF_Func.o \
              $(LIBPATH)/atu5010_timerf_regif.o \
              $(LIBPATH)/ATU5010_TIMERF_Sub.o \
              $(LIBPATH)/ATU5010_TIMERF_Sub_Func.o \
              $(LIBPATH)/atu5010_timerf_sub_regif.o \
              $(LIBPATH)/ATU5010_TIMERG.o \
              $(LIBPATH)/ATU5010_TIMERG_Func.o \
              $(LIBPATH)/ATU5010_TIMERG_Func_Timers.o \
              $(LIBPATH)/atu5010_timerg_regif.o \
              $(LIBPATH)/ATU5010_Timer.o
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
