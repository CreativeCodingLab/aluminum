#!/bin/bash

if [ $# -eq 0 ]
  then
    INCLUDE_DIR="/opt/local/include" #or wherever GLM includes live 

  else
    INCLUDE_DIR=$1 #assuming its been passed in from osx-specific run.command
fi


SCRIPTS_DIR="$( cd "$( dirname "$0" )" && pwd )"
LINUX_DIR="$SCRIPTS_DIR/.."

BASE_DIR="$LINUX_DIR/.."
BUILD_DIR="$BASE_DIR/build"
SRC_DIR="$BASE_DIR/src"

INCLUDE=" -I$BUILD_DIR/include/ -I$INCLUDE_DIR"
#INCLUDE="-I$INCLUDE_DIR"



### 0. COPY HEADERS TO INCLUDE DIR
echo -e "\n\n\nCOPYING crossplatform header files into include dir..."
mkdir -p $BASE_DIR/build/include/Aluminum

time cp $BASE_DIR/include/Aluminum/*.hpp $BASE_DIR/build/include/Aluminum

echo -e "\n\n\nCOPYING linux header files into include dir..."

time cp $LINUX_DIR/*.hpp $BASE_DIR/build/include/Aluminum
time cp $LINUX_DIR/*.h $BASE_DIR/build/include/Aluminum



cd $BASE_DIR;

### 1. PRE-COMPILE into Includes.hpp.gch

echo -e "\nPRE-COMPILING HEADERS... \n\ng++ -std=c++11 $INCLUDE $BUILD_DIR/include/Aluminum/Includes.hpp "  

#time c++ -x objective-c++-header -std=c++11 -stdlib=libc++ $INCLUDE $BUILD_DIR/include/Aluminum/Includes.hpp 
time g++ -std=c++11 $INCLUDE $BUILD_DIR/include/Aluminum/Includes.hpp 

mv $BUILD_DIR/include/Aluminum/Includes.hpp.gch $BUILD_DIR/include #we don't want to copy the .pch into the install directory, so let's put it here instead



