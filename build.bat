@echo off

::commands
::./build.bat TESTINGing build -> to generate the obj files in bin_int and exe file in bin
::./build.bat TESTINGing clear -> to delete bin_int and bin
::./build.bat TESTINGing clear_bin -> to delete bin
::./build.bat TESTINGing clear_bin_int -> to delete bin_int
::./build.bat TESTINGing info -> debugging information
::./build.bat run -> to run the bin/EXE

:: Checking dependencies

where clang 2>NUL 1>&2
if %ERRORLEVEL% neq 0 (
    echo clang is not installed
    exit /b
)

clang --version 2>NUL 1>&2
if %ERRORLEVEL% neq 0 (
    echo Add path of clang.exe to environment variable PATH
    exit /b
)

if "%1"=="testing" set "TESTING=1"
if "%1"=="source" set "SOURCE=1"
if "%1"=="run" set "RUN=1"

if "%2"=="build" set "BUILD=1"
if "%2"=="clear" set "CLEAR=1"
if "%2"=="clear" set "CLEAR_BIN=1"
if "%2"=="clear" set "CLEAR_BIN_INT=1"
if "%2"=="info" set "INFO=1"


:: Running the executable if requested
if defined RUN (
    if exist .\bin\win32\EXE.exe (
        .\bin\win32\EXE.exe
    ) else (
        echo EXE.exe not found in bin directory
        exit /b
    )
)

:: Building or cleaning TESTING files
if defined TESTING (


    if defined BUILD (
        make -f build.mak all CODE_DIRS="testing core" BIN_INT_DIR="win32" BIN_DIR="win32" ARCH="%PROCESSOR_ARCHITECTURE%"
    )
    if defined INFO (
        make -f build.mak info CODE_DIRS="testing core" BIN_INT_DIR="win32" BIN_DIR="win32" ARCH="%PROCESSOR_ARCHITECTURE%"
    )
    if defined CLEAR (
        make -f build.mak clear CODE_DIRS="testing core" BIN_INT_DIR="win32" BIN_DIR="win32" ARCH="%PROCESSOR_ARCHITECTURE%"
    )
    if defined CLEAR_BIN (
        make -f build.mak clear_bin CODE_DIRS="testing core" BIN_INT_DIR="win32" BIN_DIR="win32" ARCH="%PROCESSOR_ARCHITECTURE%"
    )
    if defined CLEAR_BIN_INT (
        make -f build.mak clear_bin_int CODE_DIRS="testing core" BIN_INT_DIR="win32" BIN_DIR="win32" ARCH="%PROCESSOR_ARCHITECTURE%"
    )
   
)
:: Building or cleaning SOURCE files
if defined SOURCE (


    if defined BUILD (
        make -f build.mak all CODE_DIRS="core source" BIN_INT_DIR="win32" BIN_DIR="win32" ARCH="%PROCESSOR_ARCHITECTURE%"
    )
    if defined INFO (
        make -f build.mak info CODE_DIRS="core source" BIN_INT_DIR="win32" BIN_DIR="win32" ARCH="%PROCESSOR_ARCHITECTURE%"
    )
    if defined CLEAR (
        make -f build.mak clear CODE_DIRS="core source" BIN_INT_DIR="win32" BIN_DIR="win32" ARCH="%PROCESSOR_ARCHITECTURE%"
    )
    if defined CLEAR_BIN (
        make -f build.mak clear_bin CODE_DIRS="core source" BIN_INT_DIR="win32" BIN_DIR="win32" ARCH="%PROCESSOR_ARCHITECTURE%"
    )
    if defined CLEAR_BIN_INT (
        make -f build.mak clear_bin_int CODE_DIRS="core source" BIN_INT_DIR="win32" BIN_DIR="win32" ARCH="%PROCESSOR_ARCHITECTURE%"
    )
   
)