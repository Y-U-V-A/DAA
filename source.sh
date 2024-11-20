#!/bin/bash

if [ "$1" = "clean" ]; then
    CLEAN=1
fi

if [ -z "$CLEAN" ]; then
    echo "building source "
    make -f source.mak
fi

if [ -n "$CLEAN" ]; then
    echo "cleaning "
    make -f source.mak clean
fi