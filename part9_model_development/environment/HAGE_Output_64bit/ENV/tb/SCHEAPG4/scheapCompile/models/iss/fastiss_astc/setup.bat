::------------------------------------------------------------------------------
:: Copyright (C) Australian Semiconductor Technology Company. 2012.
:: All Rights Reserved.
::
:: This is unpublished proprietary source code of the Australian Semiconductor
:: Technology Company (ASTC).  The copyright notice does not evidence any actual
:: or intended publication of such source code.
::------------------------------------------------------------------------------

@echo off

::------------------------------------------------------------------------------
:: Check PMS is installed and configured correctly
::------------------------------------------------------------------------------

isa_compiler --version >NUL 2>&1
if errorlevel 1 (
  echo Error: Unable to run ISA Compiler
  echo        Ensure PMS has been setup correctly and valid licenses are available
  goto error
)
 
::------------------------------------------------------------------------------
:: Check VLAB is installed and configured correctly
::------------------------------------------------------------------------------

:: Check that VLAB is working
vlab --version >NUL 2>&1
if errorlevel 1 (
  echo Error: Unable to run VLAB
  echo        Ensure PMS has been setup correctly and valid licenses are available
  goto error
)

:: Check that the VLAB_HOME environment variable has been set
if "%VLAB_HOME%"=="" (
  echo Error: Ensure PMS has been setup correctly and valid licenses are available
  goto error
)

::------------------------------------------------------------------------------
:: Set up paths
::------------------------------------------------------------------------------

:: Pick-up the runtime and model libraries for VWorks OSCAR
set PATH=%PATH%;%~dp0iss\lib\usk-%FASTISS_PLATFORM%\v850e2m
set VLAB_PATH=%VLAB_PATH%;%~dp0iss\lib\usk-%FASTISS_PLATFORM%

:: Pick up the example platform
set VLAB_PATH=%VLAB_PATH%;%~dp0iss\examples\v850e2m

::------------------------------------------------------------------------------
:: Done
::------------------------------------------------------------------------------

goto end  

:error  
 
exit /b 1  
 
:end  

::------------------------------------------------------------------------------
:: End of file
::------------------------------------------------------------------------------
