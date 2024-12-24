#!/bin/bash

# Commands:
# ./build.sh testing build -> to generate the obj files in bin_int and exe file in bin
# ./build.sh testing clear -> to delete bin_int and bin
# ./build.sh testing clear_bin -> to delete bin
# ./build.sh testing clear_bin_int -> to delete bin_int
# ./build.sh testing info -> debugging information
# ./build.sh run -> to run the bin/EXE

# Checking dependencies
if ! command -v clang &> /dev/null; then
    echo "clang is not installed"
    exit 1
fi

clang --version &> /dev/null
if [ $? -ne 0 ]; then
    echo "Add path of clang to environment variable PATH"
    exit 1
fi

# Parse arguments
TESTING=0
SOURCE=0
BUILD=0
CLEAR=0
CLEAR_BIN=0
CLEAR_BIN_INT=0
INFO=0
RUN=0

if [ "$1" = "testing" ]; then
    TESTING=1
elif [ "$1" = "source" ]; then
    SOURCE=1
elif [ "$1" = "run" ]; then
    RUN=1
fi

case "$2" in
    "build")
        BUILD=1
        ;;
    "clear")
        CLEAR=1
        ;;
    "clear_bin")
        CLEAR_BIN=1
        ;;
    "clear_bin_int")
        CLEAR_BIN_INT=1
        ;;
    "info")
        INFO=1
        ;;
esac

# Running the executable if requested
if [ $RUN -eq 1 ]; then
    if [ -f "./bin/linux/EXE" ]; then
        ./bin/linux/EXE
    else
        echo "EXE not found in bin directory"
        exit 1
    fi
fi

# Building or cleaning testing files
if [ $TESTING -eq 1 ]; then
    CODE_DIRS="testing core"
fi

# Building or cleaning source files
if [ $SOURCE -eq 1 ]; then
    CODE_DIRS="core source"
fi

BIN_INT_DIR="linux"  # Adjusted from win32
BIN_DIR="linux"      # Adjusted from win32
ARCH=$(uname -m)     # Equivalent to %PROCESSOR_ARCHITECTURE%

if [ $BUILD -eq 1 ]; then
    make -f build.mak all CODE_DIRS="$CODE_DIRS" BIN_INT_DIR="$BIN_INT_DIR" BIN_DIR="$BIN_DIR" ARCH="$ARCH"
fi

if [ $INFO -eq 1 ]; then
    make -f build.mak info CODE_DIRS="$CODE_DIRS" BIN_INT_DIR="$BIN_INT_DIR" BIN_DIR="$BIN_DIR" ARCH="$ARCH"
fi

if [ $CLEAR -eq 1 ]; then
    make -f build.mak clear CODE_DIRS="$CODE_DIRS" BIN_INT_DIR="$BIN_INT_DIR" BIN_DIR="$BIN_DIR" ARCH="$ARCH"
fi

if [ $CLEAR_BIN -eq 1 ]; then
    make -f build.mak clear_bin CODE_DIRS="$CODE_DIRS" BIN_INT_DIR="$BIN_INT_DIR" BIN_DIR="$BIN_DIR" ARCH="$ARCH"
fi

if [ $CLEAR_BIN_INT -eq 1 ]; then
    make -f build.mak clear_bin_int CODE_DIRS="$CODE_DIRS" BIN_INT_DIR="$BIN_INT_DIR" BIN_DIR="$BIN_DIR" ARCH="$ARCH"
fi

