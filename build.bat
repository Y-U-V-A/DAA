@echo off
:: format -> name config (optional)clean
if "%1"=="source" (
    set SOURCE=1
)

if "%1"=="testing" (
    set TESTING=1
)

if "%1"=="run" (
    set RUN=1
)

if "%2"=="clean" (
    set CLEAN=1
)

if defined RUN (
    .\bin\DAA.exe
)

if defined SOURCE (

    if not defined CLEAN (
        echo building source
        if exist bin rmdir /Q /S bin
        make -f source.mak
    )
    
    if defined CLEAN (
        echo cleaning source
        if exist bin rmdir /Q /S bin
        make -f source.mak clean
    )
)

if defined TESTING (

    if not defined CLEAN (
        echo building testing
        make -f testing.mak
    )
    
    if defined CLEAN (
        echo cleaning testing
        if exist bin rmdir /Q /S bin
        make -f testing.mak clean
    )
)