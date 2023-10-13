call setup.bat
rem set variables
set GHS_LMWHICH=ghs
set GHS_LMHOST=@172.29.139.10
set SIMX_DIR=C:\Users\%USERNAME%\Desktop\PSEQ_Output\ENV\tb\SCHEAPE3\scheapCompile\build\TOPV01\msvc100\Release
set MULT_DIR=C:\GHS\V800.V6\multi_614

set DSRV_DIR=C:\GHS\V800.V6\comp_201255
set HEAP_CFG=.\tp01_01_001.cfg
set RSLT_LOG=.\result.txt
set SOFT_PE1=.\tp01_01_001.out
set PY_SCRIPT=./tp01_01_001.py
set TIME_OUT=10

echo %RTESERV2%

set MULTIEXE=%MULT_DIR%/multi.exe
set RTESERV2=%DSRV_DIR%\rteserv2.exe
set MULTI_RC=.\multicfg.rc

set CNCT_CMD=%RTESERV2% -cpu pe1 -sc_heap_config %HEAP_CFG% -sc_heap_e3 %SIMX_DIR% -sc_heap_opt \"-py_scr %PY_SCRIPT%\"
set RCON_CMD=target reconnect

rem multi rc
echo debugbutton reconnect i=letter_r c="%RCON_CMD%" > %MULTI_RC%


rem invoke command
echo #### current directory was ; cd                   > %RSLT_LOG%
echo #### check programs are as follows               >> %RSLT_LOG%
C:\Windows\System32\find "G3MCPU_PROGRAM" %HEAP_CFG% >> %RSLT_LOG%
echo #### %HEAP_CFG%                                  >> %RSLT_LOG%
type %HEAP_CFG%                                       >> %RSLT_LOG%
%MULTIEXE% -p %MULTI_RC% -servertimeout %TIME_OUT% -connect="%CNCT_CMD%" %SOFT_PE1% >> %RSLT_LOG% 2>&1
