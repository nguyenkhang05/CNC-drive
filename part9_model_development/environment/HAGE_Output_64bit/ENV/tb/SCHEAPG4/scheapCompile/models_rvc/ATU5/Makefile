#############################################################################
# Makefile for ATU5
#############################################################################

#
# Name(overwrite from the top make)
#
MODEL					= ATU5

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
MODEL_ATU5_PATH			= $(MODEL_RVC_HOME)/ATU5
MODEL_ATU5_BASE_PATH            = $(MODEL_RVC_HOME)/ATU5_Base

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

INCPATH_MINE	= -I$(MODEL_ATU5_BASE_PATH)/TIMERA -I$(MODEL_ATU5_BASE_PATH)/TIMERB -I$(MODEL_ATU5_BASE_PATH)/TIMERC -I$(MODEL_ATU5_BASE_PATH)/TIMERD -I$(MODEL_ATU5_BASE_PATH)/TIMERE -I$(MODEL_ATU5_BASE_PATH)/TIMERF -I$(MODEL_ATU5_BASE_PATH)/TIMERG -I$(MODEL_ATU5_PATH) -I$(MODEL_ATU5_BASE_PATH)

#CXXFLAGS	= $(OPTFLAG) $(DEFFLAG) $(DEFFLAG_MINE) $(DBGFLAG) $(DBGFLAG_MINE) $(INCPATH) $(INCPATH_MINE) -fprofile-arcs -ftest-coverage -fno-elide-constructors
CXXFLAGS	= $(OPTFLAG) $(DEFFLAG) $(DEFFLAG_MINE) $(DBGFLAG) $(DBGFLAG_MINE) $(INCPATH) $(INCPATH_MINE)

#
# Files(for local)
#
#SOURCES		= $(MODEL).cpp
#OBJECTS		= $(LIBPATH)/$(MODEL).o
#TARGET			= $(LIBPATH)/$(MODEL).a

SOURCES     = ATU5.cpp \
              ATU5_Func.cpp \
              atu5_regif.cpp 

OBJECTS     = $(LIBPATH)/ATU5.o \
              $(LIBPATH)/ATU5_Func.o \
              $(LIBPATH)/atu5_regif.o 

OBJECTS_BASE    = $(LIBPATH)/../ATU5_Base/ATU5_Timer.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERA.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERA_Func.o \
              $(LIBPATH)/../ATU5_Base/atu5_timera_regif.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERB.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERB_Func.o \
              $(LIBPATH)/../ATU5_Base/atu5_timerb_regif.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERC.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERC_Func.o \
              $(LIBPATH)/../ATU5_Base/atu5_timerc_regif.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERC_SUB.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERC_SUB_Func.o \
              $(LIBPATH)/../ATU5_Base/atu5_timerc_sub_regif.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERD.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERD_Func.o \
              $(LIBPATH)/../ATU5_Base/atu5_timerd_regif.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERD_SUB.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERD_SUB_Func.o \
              $(LIBPATH)/../ATU5_Base/atu5_timerd_sub_regif.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERE.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERE_Func.o \
              $(LIBPATH)/../ATU5_Base/atu5_timere_regif.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERE_SUB.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERE_SUB_Func.o \
              $(LIBPATH)/../ATU5_Base/atu5_timere_sub_regif.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERF.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERF_Func.o \
              $(LIBPATH)/../ATU5_Base/atu5_timerf_regif.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERF_Sub.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERF_Sub_Func.o \
              $(LIBPATH)/../ATU5_Base/atu5_timerf_sub_regif.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERG.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERG_Func.o \
              $(LIBPATH)/../ATU5_Base/ATU5_TIMERG_Func_Timers.o \
              $(LIBPATH)/../ATU5_Base/atu5_timerg_regif.o 

TARGET		= $(LIBPATH)/$(MODEL).a

#
# Implicit rules
#
%.d:%.cpp
	$(CXX) $(DEPFLAG) $(CXXFLAGS) $< | sed 's!$*.o:!$$(LIBPATH)/&!g' > $@
$(LIBPATH)/%.o:%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $< $(OBJECTS_BASE)

#
# Build rules
#
.PHONY : all compile clean
all:
	if test ! -d $(LIBPATH); then mkdir $(LIBPATH); fi;
	$(MAKE) compile

compile: $(TARGET)

$(TARGET): $(OBJECTS) $(OBJECTS_BASE)
	$(AR) $@ $(OBJECTS) $(OBJECTS_BASE) 2>&1 | c++filt
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
