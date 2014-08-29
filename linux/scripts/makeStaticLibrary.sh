#!/bin/bash

SCRIPTS_DIR="$( cd "$( dirname "$0" )" && pwd )"
LINUX_DIR="$SCRIPTS_DIR/.."

BASE_DIR="$LINUX_DIR/.."
BUILD_DIR="$BASE_DIR/build"
SRC_DIR="$BASE_DIR/src"

#INCLUDE_DIR="/opt/local/include"
INCLUDE_DIR="/home/evl/aforbes/include"

INCLUDE=" -I$BUILD_DIR/include -I$INCLUDE_DIR" 
ALUMINUM_SRC="$SRC_DIR/*.cpp"
LINUX_SRC="$LINUX_DIR/*.cpp"

SRC="$ALUMINUM_SRC $LINUX_SRC"



cd $BASE_DIR;


### 1. COMPILE
echo -e "\n\n\n*** In makeStaticLibrary.sh "
echo -e "\nBUILDING obj files for static library... \n\ng++ -c -include $BUILD_DIR/include/Includes.hpp -std=c++11 $INCLUDE $SRC"
#echo -e "\nBUILDING obj files for static library... \n\ng++ -c -std=c++11 $INCLUDE $SRC"

# use -H below to double check if the headers have been precompiled
#time c++ -c -x objective-c++ -include $BUILD_DIR/include/Aluminum/Includes.hpp -std=c++11 -stdlib=libc++ $INCLUDE $SRC 
time g++ -c -include $BUILD_DIR/include/Includes.hpp -std=c++11 $INCLUDE $SRC 
#time g++ -c -std=c++11 $INCLUDE $SRC 

### 2. ARCHIVE
#echo -e "\n\n\nARCHIVING obj files into static library aluminum.a..."

mkdir -p $BUILD_DIR/lib/
time ar rcs $BUILD_DIR/lib/libaluminum.a $BASE_DIR/*.o


### 4. CLEAN UP
rm $BASE_DIR/*.o
rm $LINUX_DIR/*.o
