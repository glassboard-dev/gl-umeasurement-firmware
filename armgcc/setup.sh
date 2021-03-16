#!/bin/bash
# Export our ARM GCC compiler
if [ -z $2 ]
then
    echo "Using default ARM GCC toolchain path: /usr/local/gcc-arm-none-eabi-10-2020-q4-major"
    export ARMGCC_DIR=/usr/local/gcc-arm-none-eabi-10-2020-q4-major
else
    export ARMGCC_DIR=$2
fi;

# Cleanup any old Cmake artifacts
if [ -d "CMakeFiles" ];then rm -rf CMakeFiles; fi
if [ -f "Makefile" ];then rm -f Makefile; fi
if [ -f "cmake_install.cmake" ];then rm -f cmake_install.cmake; fi
if [ -f "CMakeCache.txt" ];then rm -f CMakeCache.txt; fi
if [ -d "debug" ];then rm -rf debug; fi
if [ -d "release" ];then rm -rf debug; fi

# Generate a makefile for the debug variant
if [ -z $1 ]
then
    echo "Please provide the PN you want to generate a Makefile for. Available options: LPC5526 or LPC55S28"
elif [ $1 == "LPC55S28" ]
then
    echo "Generating Makefile for LPC55S28"
    cmake -DCMAKE_TOOLCHAIN_FILE="./armgcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=debug  -DTARGET_MCU=LPC55S28 .
elif [ $1 == "LPC5526" ]
then
    echo "Generating Makefile for LPC5526"
    cmake -DCMAKE_TOOLCHAIN_FILE="./armgcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=debug  -DTARGET_MCU=LPC5526 .
else
    echo "Invalid PN provided. Available options: LPC5526 or LPC55S28"
fi;