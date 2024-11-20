#!/bin/bash

if [ "$1" = "clean" ]; then
    CLEAN=1
fi

if [ -z "$CLEAN" ]; then
    echo "building source "
    make -f tests.mak
fi

if [ -n "$CLEAN" ]; then
    echo "cleaning "
    make -f tests.mak clean
fi