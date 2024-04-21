#!/bin/bash

if [ -e build ] ; then rm -rf build ; fi
if [ -e bin ]   ; then rm -rf bin   ; fi

cmake -B build && cmake --build build -j