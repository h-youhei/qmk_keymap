#!/usr/bin/bash

name=${PWD##*/}
cd ../../../..
make ergodox_ez:$name
