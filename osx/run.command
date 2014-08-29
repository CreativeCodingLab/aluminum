#!/bin/bash

#example usages:
#  build static lib, compile app, run app 
#   ./run.command examples/texture/TextureExample.mm
#  run app only
#   ./run.command -r examples/font/Text3D.mm
#  skip building the static lib, compile app but don't run it
#   ./run.command -sc examples/model/Model.mm
#  force re-precompiling headers, build static lib, compile and run app 
#   ./run.command -p examples/stereo/Stereo.mm
#  clean by removing aluminum.a, the precompiled headers, and the application binary (this last one is optional)
#   ./run.command -z
#   ./run.command -z examples/stereo/MyApp.mm
 

OSX_VERSION="$(sw_vers -productVersion | grep -o '[0-9][0-9]\.[0-9]')";
echo -e "\n\nYou are running OSX version $OSX_VERSION\n\n"


#default builder flags, can be changed with -c, -r, -b, -s, -p. Use -? for help.
BUILD=1
COMPILE=1
RUN=1
PRE=0
CLEAN=0
INSTALL=0

while getopts "hcirbspz?" opt; do
  case $opt in

    c) echo "...compile only"
      COMPILE=1
      RUN=0
      ;;
    r) echo "...run only"
      PRE=0
      BUILD=0
      COMPILE=0
      RUN=1
      ;;
    b) echo "...build static lib"
      BUILD=1
      ;;
    i) echo "...install static lib"
      COMPILE=0
      RUN=0
      CLEAN=0
      PRE=1
      BUILD=1
      INSTALL=1
      ;;
    s) echo "...skip building static lib"
      BUILD=0
      ;;
    p) echo "...precompile headers"
      PRE=1
      ;;
    z) echo "...clean"
      CLEAN=1
      ;;
    h) echo -e "\n\taluminum run.command help file"
      echo -e "\n\tvalid flags are:"
      echo -e "\t -c : compile only"
      echo -e "\t -r : run only"
      echo -e "\t -s : skip building static lib"
      echo -e "\t -b : force building static lib"
      echo -e "\t -i : install static lib (must be run as root)"
      echo -e "\t -p : force precompiling headers"
      echo -e "\t -z : clean"
      echo -e "\n\tdefault flags are: -bcr\n"
      exit ;;
    \?) echo -e "\n\taluminum run.command help file"
      echo -e "\n\tvalid flags are:"
      echo -e "\t -c : compile only"
      echo -e "\t -r : run only"
      echo -e "\t -s : skip building static lib"
      echo -e "\t -b : force building static lib"
      echo -e "\t -i : install static lib (must be run as root)"
      echo -e "\t -p : force precompiling headers"
      echo -e "\t -z : clean"
      echo -e "\n\tdefault flags are: -bcr\n"
      exit ;;

  esac
done

shift $(( OPTIND - 1 ))  # shift past the last flag or argument

echo "PRECOMPILE=$PRE BUILD=$BUILD COMPILE=$COMPILE RUN=$RUN"

RUN_PATH=$( dirname "$0" )
APP_PATH=$( dirname "$1" )
APP_SRC=$( basename "$1" )
APP="${APP_SRC%.*}"

echo "RUN_PATH = $RUN_PATH"
echo "APP_PATH = $APP_PATH"
echo "APP_SRC = $APP_SRC"
echo "APP = $APP"

#CURRENT_DIR="$( cd "$( dirname "$0" )" && pwd )"
#CURRENT_DIR=$RUN_DIR
#cd $CURRENT_DIR
#echo "p w d = " 
#pwd

BASE_DIR="$RUN_PATH/.."
BUILD_DIR="$BASE_DIR/build"
SRC_DIR="$BASE_DIR/src"
OSX_DIR="$RUN_PATH"
LIB_DIR="/opt/local/lib"
INCLUDE_DIR="/opt/local/include/"
SYSTEM_INCLUDE="/usr/local/include/Aluminum"
SYSTEM_LIB="/usr/local/lib"
#INCLUDE_GLM_DIR="/usr/local/include"

FFMPEG="-L/opt/local/lib -lavformat -lavcodec -lswscale -lavutil"
ASSIMP="$LIB_DIR/libassimp.dylib"
FREEIMAGE="$LIB_DIR/libfreeimage.dylib" 

COCOA="-isysroot /Applications/XCode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$OSX_VERSION.sdk -mmacosx-version-min=$OSX_VERSION -framework Cocoa -framework QuartzCore -framework OpenGL -framework AppKit -framework Foundation -framework AVFoundation -framework CoreMedia "

OPTIONS="-O3 -Wreturn-type -Wformat -Wmissing-braces -Wparentheses -Wswitch -Wunused-variable -Wsign-compare -Wno-unknown-pragmas -Woverloaded-virtual"

#INCLUDE_FFMPEG="-I$INCLUDE_DIR/libswscale -I$INCLUDE_DIR/libavcodec -I$INCLUDE_DIR/libavformat" 

#INCLUDE="-I./ -I$OSX_DIR -I$SRC_DIR -I$INCLUDE_DIR $INCLUDE_FFMPEG -I./$APP_PATH"
#INCLUDE="-I./ -I$OSX_DIR -I$SRC_DIR -I$INCLUDE_DIR -I$INCLUDE_GLM_DIR -I./$APP_PATH"
#INCLUDE="-I./ -I$OSX_DIR -I$SRC_DIR -I$INCLUDE_DIR -I./$APP_PATH"
INCLUDE="-I./ -I$BUILD_DIR/include -I$SRC_DIR -I$INCLUDE_DIR -I./$APP_PATH"
LIBS="" #$ASSIMP $FREEIMAGE" #$FFMPEG"
SRC=" -x objective-c++ $SRC_DIR/*.cpp $OSX_DIR/*.mm $APP_SRC"


if [ "$CLEAN" -eq 1 ]; then 

  ### 1. REMOVE static lib, precomiled headers, includes 
  echo -e "removing aluminum.a, Include.hpp.gch, $APP...\n\n"

  echo "rm $BASE_DIR/build/lib/aluminum.a"
  rm $BASE_DIR/build/lib/aluminum.a

  echo "rm $BASE_DIR/build/include/Aluminum/*.hpp"
  rm $BASE_DIR/build/include/Aluminum/*.hpp
  
  echo "rm $BASE_DIR/build/include/Aluminum/*.h"
  rm $BASE_DIR/build/include/Aluminum/*.h


  echo "rm $BASE_DIR/build/include/Includes.hpp.gch"
  rm $BASE_DIR/build/include/Includes.hpp.gch
  
  if [[ -n "$APP" ]]; then
    echo "rm $APP_PATH/$APP"
    rm $APP_PATH/$APP
  fi

  exit
fi

if [ ! -f $BUILD_DIR/include/Aluminum/Includes.hpp.gch ]; then
  PRE=1
  BUILD=1
  COMPILE=1
fi

if [ "$PRE" -eq 1 ]; then 

  ### 1. PRECOMPILE headers (should only need to if Include.hpp has changed)
  echo -e "precomiling headers from Include.hpp into Include.hpp.gch...\n\n"

  $OSX_DIR/scripts/precompileHeaders.sh
fi

if [ "$BUILD" -eq 1 ]; then 

  ### 1. BUILD aluminum static lib (should only need to if changed aluminum files)
  #       assumes we've already pre-compiled Include.hpp
  echo -e "\n\n\nBuilding static library using precompiled header Includes.hpp.gch...\n\n"

  echo "OSX_DIR = $OSX_DIR"
  $OSX_DIR/scripts/makeStaticLibrary.sh
fi


if [ "$INSTALL" -eq 1 ]; then 
  echo -e "installing static libs and headers...\n\n"

  $OSX_DIR/scripts/installAluminumOSX.sh $BASE_DIR
  exit;
fi

if [ "$COMPILE" -eq 1 ]; then 

  ###2a. remove existing binary if it exists - if the next compile fails then the user might not realize it if the old version starts to run...
  if [[ -n "$APP" ]]; then
    echo "rm $APP_PATH/$APP"
    rm $APP_PATH/$APP
  fi

  ### 2. COMPILE w/aluminum lib
  echo -e "\n\n\nCOMPILING user code with aluminum static lib... \n\nc++ $COCOA $OPTIONS -std=c++11 -stdlib=libc++ $BASE_DIR/build/lib/aluminum.a $LIBS $INCLUDE $APP_PATH/*.mm -o $APP_PATH/$APP"

  time c++ $COCOA $OPTIONS -std=c++11 -stdlib=libc++ $BASE_DIR/build/lib/aluminum.a $LIBS $INCLUDE $APP_PATH/*.mm -o $APP_PATH/$APP

fi 

if [ "$RUN" -eq 1 ]; then 
  ### 3. RUN the app
  cd $APP_PATH && ./$APP
fi
