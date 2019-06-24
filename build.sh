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

cd tests

rm -rf CMakeFiles
rm -f cmake_install.cmake
rm -f CMakeCache.txt
rm -f Makefile
rm -f yaml-parser-tests

cmake -DCMAKE_BUILD_TYPE=Debug .
make

echo
echo "<== RUN TESTS ==>"
echo

./yaml-parser-tests
