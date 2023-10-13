cmake_minimum_required( VERSION 3.2 )

set( VERSION ${CFOREST_VERSION} )
set( REVISION ${CFOREST_REVISION} )

# Initial val for Options
set( INIT_USE_PYTHON_API ON )
set( INIT_USE_BTAC_BHT OFF )
set( INIT_ENABLE_DEBUG ON )

# Initial val for Local Options
set( INIT_USE_SIM_IO ON )
set( INIT_ENABLE_TMU ON )
set( INIT_ENABLE_UART ON )
set( INIT_ENABLE_PERFCNT ON )
set( INIT_ENABLE_GDB ON )
set( INIT_ENABLE_ERROR_AREA ON )
set( INIT_ENABLE_OSTM ON )
set( INIT_PRINT_FXU_PIPE OFF)
set( INIT_ENABLE_SAME_PC_BREAK OFF)
set( INIT_ENABLE_ROM_W_ERROR ON)
set( INIT_ENBALE_SUBBANK_LRAM ON)
set( INIT_ENBALE_SUBBANK_CRAM ON)
if( MSVC )
  set( INIT_ENABLE_UART OFF )
  set( INIT_ENABLE_GDB OFF )
endif( )

# Select by Product type
if( ${PRODUCT_TYPE}  MATCHES "^CFORESTG4X$" )
  option( ENABLE_TRACE_MDP_SUPPRESS "Enable trace output when access cause mdp suppressed" OFF )
  set( INIT_ENABLE_SAME_PC_BREAK ON)
  option( ENABLE_PEID_POS_CTRL "Enable command-line option --peid-pos." OFF )
  option( ENABLE_FLC_I "Enable FlashCache for fetch access." OFF )
  option( ENABLE_FLC_D "Enable FlashCache for data access." OFF )
  option( ENABLE_EXE_ORDER_CTRL "Enable command-line option --exe-order." OFF )

elseif( ${PRODUCT_TYPE} MATCHES "^COMPRUNNER$" )
  set( COMP_RUNNER ON )
  set( COMP_RUNNER_G4MH ON )
  set( INIT_ENABLE_TMU OFF )
  set( INIT_ENABLE_UART OFF )
  option( ENABLE_TRACE_MDP_SUPPRESS "Enable trace output when access cause mdp suppressed" ON )
  option( ENABLE_COMP_DEBUG "Display debug messages from CompRunnerI/F" ON )
  set( INIT_ENABLE_SAME_PC_BREAK ON)

elseif( ${PRODUCT_TYPE} MATCHES "^FROG$" )
  set( FROG_API_IF "???" ON CACHE INTERNAL "" ) # Global scope
  if( BUILD_TEST )
    set( FROG_API_G4MH_UT ON )
  else( )
    set( FROG_API_G4MH_UT OFF )
  endif( )
  set( INIT_ENABLE_GDB OFF )
  set( INIT_ENABLE_SAME_PC_BREAK ON)

elseif( ${PRODUCT_TYPE} MATCHES "^CSPLUS$" )
  set( INIT_USE_PYTHON_API OFF )
  set( SUPPORT_CUBESUITE "???" ON CACHE INTERNAL "" ) # Global scope
  set( INIT_ENABLE_TMU OFF )
  set( INIT_ENABLE_UART OFF )
  set( INIT_ENABLE_GDB OFF )
  set( INIT_ENABLE_DEBUG OFF )
  set( ENABLE_DBG_BREAK OFF )
  set( INIT_ENABLE_ROM_W_ERROR OFF)
  option( ENABLE_BARRIER_CSP "Enable BaseAddr_BAR on CSPLUS API" OFF )

elseif( ${PRODUCT_TYPE}  MATCHES "^SCHEAP$" )
  set( INIT_USE_PYTHON_API OFF )
  set( SUPPORT_SC_HEAP "???" ON CACHE INTERNAL "" )
  option( TEST_BUILD_SCHEAP "???" OFF )
  set( INIT_USE_SIM_IO OFF )
  set( INIT_ENABLE_TMU OFF )
  set( INIT_ENABLE_UART OFF )
  set( INIT_ENABLE_PERFCNT OFF )
  set( INIT_ENABLE_GDB OFF )
  set( INIT_ENABLE_OSTM OFF )
  set( INIT_ENABLE_DEBUG OFF )
  set( ENABLE_DBG_BREAK OFF )
  set( DISABLE_DESTRUCTOR_CYCLE ON )
  set( ENABLE_GRG ON )
  set( CCG3M_GRAM ON )
  set( CCG3M_DBUF ON )
  option( ENABLE_TRACE_BUF "Enable Trace buffer for SC-HEAP" OFF ) # for lib
  set( INIT_ENABLE_ROM_W_ERROR OFF)
  option( ENABLE_PEID_POS_CTRL "Enable command-line option --peid-pos." ON )
  if( TEST_BUILD_SCHEAP )
    set( TEST_SC_HEAP ON )  # for test
    set( INIT_USE_SIM_IO ON )    # for test
    set( INIT_USE_PYTHON_API ON ) # for test
  endif( )
  if( MSVC )
    set ( DLLEXPORT ON )
  endif( )  

else( )
  message( SEND_ERROR "PRODUCT_TYPE is ${PRODUCT_TYPE}." )
endif( )

# Options
option( USE_PYTHON_API "Use pyhton interface" ${INIT_USE_PYTHON_API} ) # Global scope
option( USE_BTAC_BHT "Core/BTB setting" ${INIT_USE_BTAC_BHT} ) # Global scope
option( ENABLE_DEBUG "???" ${INIT_ENABLE_DEBUG} ) # Global scope
option( ENABLE_DBG_PMC "Enable PMC event detection for DB mode" ON )
option( ENABLE_SAME_PC_BREAK "Enable same PC max break" ${INIT_ENABLE_SAME_PC_BREAK} )
option( DEBUG_CS "Enable to dump internal cycle info to debug CS routine" OFF)
option( DEBUG_CS_LSU "Enable to dump internal cycle info to debug CS LSU routine" OFF)
option( DEBUG_CS_MEM "Enable to dump internal cycle info to debug CS outstand routine" OFF)
option( DEBUG_CORE_INT "Enable to dump inerruption info of Core side" OFF)
option( ENABLE_ROM_W_ERROR "Enable error response when PE writes to ROM." ${INIT_ENABLE_ROM_W_ERROR} )
option( DEBUG_PRINT_STALL "Print diff cycle instead of issue cycle in trace log" OFF)
option( ENBALE_SUBBANK_LRAM "Enable subbank of LRAM" ${INIT_ENBALE_SUBBANK_LRAM} )
option( ENBALE_SUBBANK_CRAM "Enable subbank of CRAM" ${INIT_ENBALE_SUBBANK_CRAM} )
option( MASK_INT_BY_GPID "Enable Mask GMxxINT with differetn GPID and BGxxINT with same GPID" ON )

# Check exist the python environment if USE_PYTHON_API is ON
if( USE_PYTHON_API )
  set( ENABLE_PYTHON ON )
  find_package( PythonLibs 2.7 REQUIRED )
  find_package( PythonInterp 2.7 REQUIRED )
endif( )

# Core/BTB
set( BTAC_WAY 1 CACHE STRING "BTAC_WAY number" )
set( BTAC_ENTRY 16 CACHE STRING "BTAC_ENTRY number" )
set( STUDY_GHR_64BIT "???" ON )
set( USE_2BIT_COUNTER_BHT "???" ON )
set( USE_GSHARE_BHT "???" ON )
set( USE_TAKEN_GHT "???" ON )
if( USE_BTAC_BHT )
  if( NOT USE_BTAC_DIRECT_MAP )
    if( NOT USE_BTAC_LRU )
      if( BTAC_RR_EACH_ENTRY )
        set( BTAC_RR_EACH_ENTRY ON )
      endif( )
      set( USE_BTAC_ROUNDROBIN ON )
    else( )
      set( USE_BTAC_LRU ON )
    endif( )
  endif( )
  if( ALLOW_MULTI_HIT )
    set( ALLOW_MULTI_HIT ON )
  endif( )
  set( USE_6BIT_GHR ON )
else( )
  set( USE_HYBRID_BP ON )
  set( USE_6BIT_GHR ON )
endif( )
set( BTAC_WAY_NUM ${BTAC_WAY} )
set( BTAC_ENTRY_NUM ${BTAC_ENTRY} )
# Core/CEDAR
set( ENABLE_PIC "???" ON )

# Core/DEBUGBREAK
set( ENABLE_DBG_BREAK ${ENABLE_DEBUG} )

# Core/PE
set( ENABLE_RB_MDP "???" ON )
set( ENABLE_ROM_IDCONFLICT "???" ON )
set( CHECK_LACK_DBREG "???" ON )
option( USE_STALL_COUNT "Use stall counter" OFF )
option( SYNC_PE_LSU_TIME "Synchronize multi PE by LSU time additionally" ON )

# Peripherals
option( USE_SIM_IO "???" ${INIT_USE_SIM_IO} )
option( ENABLE_TMU "???" ${INIT_ENABLE_TMU} )
option( ENABLE_UART "???" ${INIT_ENABLE_UART} )
option( ENABLE_PERFCNT "???" ${INIT_ENABLE_PERFCNT} )
option( ENABLE_GDB "???" ${INIT_ENABLE_GDB} )
option( ENABLE_ERROR_AREA "???" ${INIT_ENABLE_ERROR_AREA} )
option( ENABLE_OSTM "???" ${INIT_ENABLE_OSTM} )
option( PRINT_FXU_PIPE "Show FXU pipe info" ${INIT_PRINT_FXU_PIPE} )

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
                ${CMAKE_CURRENT_BINARY_DIR}/config.h )
