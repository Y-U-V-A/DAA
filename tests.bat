@echo off

if "%1"=="clean" (
    set CLEAN=1
)

if not defined CLEAN (
    echo building source
    make -f tests.mak
)

if defined CLEAN (
    echo cleaning
    make -f tests.mak clean
)
