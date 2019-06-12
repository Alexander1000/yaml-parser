#!/bin/bash

rm -rf cmake-build-debug
rm -rf CMakeFiles
rm -f cmake_install.cmake
rm -f CMakeCache.txt
rm -f install_manifest.txt
rm -f libyaml-parser.a
rm -f yaml-parser.cbp
rm -f Makefile

cmake .
make
make install
