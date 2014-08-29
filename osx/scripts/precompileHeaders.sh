#!/bin/bash

SCRIPTS_DIR="$( cd "$( dirname "$0" )" && pwd )"
OSX_DIR="$SCRIPTS_DIR/.."

BASE_DIR="$OSX_DIR/.."
BUILD_DIR="$BASE_DIR/build"
SRC_DIR="$BASE_DIR/src"
INCLUDE_DIR="/opt/local/include"

INCLUDE="-I./ -I$BUILD_DIR/include/ -I$SRC_DIR -I$INCLUDE_DIR"




### 0. COPY HEADERS TO INCLUDE DIR
echo -e "\n\n\nCOPYING crossplatform header files into include dir..."
mkdir -p $BASE_DIR/build/include/Aluminum

time cp $SRC_DIR/*.hpp $BASE_DIR/build/include/Aluminum

echo -e "\n\n\nCOPYING osx header files into include dir..."

time cp $OSX_DIR/*.hpp $BASE_DIR/build/include/Aluminum
time cp $OSX_DIR/*.h $BASE_DIR/build/include/Aluminum



cd $BASE_DIR;

### 1. PRE-COMPILE into Includes.hpp.gch

echo -e "\nPRE-COMPILING HEADERS... \n\nc++ -x objective-c++-header -std=c++11 -stdlib=libc++ $INCLUDE $BUILD_DIR/include/Aluminum/Includes.hpp "  

time c++ -x objective-c++-header -std=c++11 -stdlib=libc++ $INCLUDE $BUILD_DIR/include/Aluminum/Includes.hpp 

mv $BUILD_DIR/include/Aluminum/Includes.hpp.gch $BUILD_DIR/include #we don't want to copy the .pch into the install directory, so let's put it here instead



