#!/bin/sh
export ARMGCC_DIR=/usr/local/gcc-arm-none-eabi-10-2020-q4-major
if [ -d "CMakeFiles" ];then rm -rf CMakeFiles; fi
if [ -f "Makefile" ];then rm -f Makefile; fi
if [ -f "cmake_install.cmake" ];then rm -f cmake_install.cmake; fi
if [ -f "CMakeCache.txt" ];then rm -f CMakeCache.txt; fi
cmake -DCMAKE_TOOLCHAIN_FILE="../../../../../../tools/cmake_toolchain_files/armgcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=release  .
make -j

if [ -d "CMakeFiles" ];then rm -rf CMakeFiles; fi
if [ -f "Makefile" ];then rm -f Makefile; fi
if [ -f "cmake_install.cmake" ];then rm -f cmake_install.cmake; fi
if [ -f "CMakeCache.txt" ];then rm -f CMakeCache.txt; fi
cmake -DCMAKE_TOOLCHAIN_FILE="../../../../../../tools/cmake_toolchain_files/armgcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=debug  .
make -j

