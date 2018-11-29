#!/usr/bin/bash

name=`pwd -P`
name=${name##*/}
cd -P ../../../..
make ergodox_ez:$name
