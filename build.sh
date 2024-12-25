#!/bin/bash

# Commands:
# ./build.sh source build <filename> -> to generate the obj files in bin_int and exe file in bin
# ./build.sh source clear -> to delete bin_int and bin
# ./build.sh source clear_bin -> to delete bin
# ./build.sh source clear_bin_int -> to delete bin_int
# ./build.sh source info -> debugging information
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

# Check clang version
if ! clang --version &> /dev/null; then
    echo "Add path of clang to environment variable PATH"
    exit 1
fi

# Initialize variables
TESTING=0
SOURCE=0
RUN=0
BUILD=0
CLEAR=0
CLEAR_BIN=0
CLEAR_BIN_INT=0
INFO=0

# Parse command line arguments
case "$1" in
    "testing")
        TESTING=1
        ;;
    "source")
        SOURCE=1
        ;;
    "run")
        RUN=1
        ;;
esac

case "$2" in
    "build")
        BUILD=1
        ;;
    "clear")
        CLEAR=1
        CLEAR_BIN=1
        CLEAR_BIN_INT=1
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

# Check if filename is provided for build command
if [ $BUILD -eq 1 ]; then
    if [ -z "$3" ]; then
        echo "Please provide the filename to build"
        exit 1
    fi
cat > source/main.c << EOL
#include "includes.h"
int main() {
    zmemory_init();
    zmemory_log();
    logger_init(1024 * 1024);
    $3_run();
    logger_shutdown();
    zmemory_log();
    zmemory_destroy();
    return 0;
}
EOL

fi





# Run executable if requested
if [ $RUN -eq 1 ]; then
    if [ -f "./bin/linux/EXE" ]; then
        ./bin/linux/EXE
    else
        echo "EXE not found in bin directory"
        exit 1
    fi
fi

ARCH=$(uname -m)

# Handle testing commands
if [ $TESTING -eq 1 ]; then
    if [ $BUILD -eq 1 ]; then
        make -f build.mak all CODE_DIRS="testing core" BIN_INT_DIR="linux" BIN_DIR="linux" ARCH="$ARCH"
    fi
    if [ $INFO -eq 1 ]; then
        make -f build.mak info CODE_DIRS="testing core" BIN_INT_DIR="linux" BIN_DIR="linux" ARCH="$ARCH"
    fi
    if [ $CLEAR -eq 1 ]; then
        make -f build.mak clear CODE_DIRS="testing core" BIN_INT_DIR="linux" BIN_DIR="linux" ARCH="$ARCH"
    fi
    if [ $CLEAR_BIN -eq 1 ]; then
        make -f build.mak clear_bin CODE_DIRS="testing core" BIN_INT_DIR="linux" BIN_DIR="linux" ARCH="$ARCH"
    fi
    if [ $CLEAR_BIN_INT -eq 1 ]; then
        make -f build.mak clear_bin_int CODE_DIRS="testing core" BIN_INT_DIR="linux" BIN_DIR="linux" ARCH="$ARCH"
    fi
fi

# Handle source commands
if [ $SOURCE -eq 1 ]; then
    if [ $BUILD -eq 1 ]; then
        make -f build.mak all CODE_DIRS="core source" BIN_INT_DIR="linux" BIN_DIR="linux" ARCH="$ARCH"
    fi
    if [ $INFO -eq 1 ]; then
        make -f build.mak info CODE_DIRS="core source" BIN_INT_DIR="linux" BIN_DIR="linux" ARCH="$ARCH"
    fi
    if [ $CLEAR -eq 1 ]; then
        make -f build.mak clear CODE_DIRS="core source" BIN_INT_DIR="linux" BIN_DIR="linux" ARCH="$ARCH"
    fi
    if [ $CLEAR_BIN -eq 1 ]; then
        make -f build.mak clear_bin CODE_DIRS="core source" BIN_INT_DIR="linux" BIN_DIR="linux" ARCH="$ARCH"
    fi
    if [ $CLEAR_BIN_INT -eq 1 ]; then
        make -f build.mak clear_bin_int CODE_DIRS="core source" BIN_INT_DIR="linux" BIN_DIR="linux" ARCH="$ARCH"
    fi
fi