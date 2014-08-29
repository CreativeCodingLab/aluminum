#!/bin/bash

SCRIPTS_DIR="$( cd "$( dirname "$0" )" && pwd )"
OSX_DIR="$SCRIPTS_DIR/.."

BASE_DIR="$OSX_DIR/.."
BUILD_DIR="$BASE_DIR/build"
SRC_DIR="$BASE_DIR/src"
INCLUDE_DIR="/opt/local/include"

INCLUDE="-I$SRC_DIR -I$BUILD_DIR/include -I$INCLUDE_DIR" 
ALUMINUM_SRC="$SRC_DIR/*.cpp"
OSX_SRC="$OSX_DIR/*.mm"

SRC="$ALUMINUM_SRC $OSX_SRC"



cd $BASE_DIR;


### 1. COMPILE
echo -e "\n\n\n*** In makeStaticLibrary.sh "
echo -e "\nBUILDING obj files for static library... \n\nc++ -c -x objective-c++ -include $BUILD_DIR/include/Aluminum/Includes.hpp -std=c++11 -stdlib=libc++ $INCLUDE $SRC"

# use -H below to double check if the headers have been precompiled
#time c++ -c -x objective-c++ -include $BUILD_DIR/include/Aluminum/Includes.hpp -std=c++11 -stdlib=libc++ $INCLUDE $SRC 
time c++ -c -x objective-c++ -include $BUILD_DIR/include/Includes.hpp -std=c++11 -stdlib=libc++ $INCLUDE $SRC 

### 2. ARCHIVE
echo -e "\n\n\nARCHIVING obj files into static library aluminum.a..."

mkdir -p $BUILD_DIR/lib/
time ar rs $BUILD_DIR/lib/aluminum.a $BASE_DIR/*.o


### 4. CLEAN UP
rm $BASE_DIR/*.o
rm $OSX_DIR/*.o
