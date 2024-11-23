#!/bin/bash

# format -> name config (optional)clean
if [ "$1" = "source" ]; then
    SOURCE=1
fi

if [ "$1" = "testing" ]; then
    TESTING=1
fi

if [ "$1" = "run" ]; then
    RUN=1
fi

if [ "$2" = "clean" ]; then
    CLEAN=1
fi

if [ ! -z $RUN ]; then
    ./bin/DAA
fi

if [ ! -z "$SOURCE" ]; then
    
    if [ -z "$CLEAN" ]; then
        
        echo "building source"
        [ -d bin ] && rm -rf bin
        make -f source.mak

    fi

    if [ ! -z "$CLEAN" ]; then

        echo "cleaning source"
        [ -d bin ] && rm -rf bin
        make -f source.mak clean
    fi
fi

if [ ! -z "$TESTING" ]; then
    
    if [ -z "$CLEAN" ]; then
        
        echo "building testing"
        [ -d bin ] && rm -rf bin
        make -f testing.mak

    fi

    if [ ! -z "$CLEAN" ]; then

        echo "cleaning testing"
        [ -d bin ] && rm -rf bin
        make -f testing.mak clean
    fi
fi