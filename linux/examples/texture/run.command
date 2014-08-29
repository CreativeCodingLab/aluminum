#!/bin/bash

EXAMPLE_DIR="$( cd "$( dirname "$0" )" && pwd )"

BASE_DIR="$EXAMPLE_DIR/../../../"
SRC_DIR="$BASE_DIR/src/"
LINUX_DIR="$BASE_DIR/linux/"

cd $BASE_DIR

pwd

#testing moving libs to local folders (instead of /opt/local...) . Will have to move the headers to a local spot as well if I want to go this route.
#prob won't need sysroot option if we do that (?)
#g++ -I/usr/include/i386-linux-gnu/ -xc++ -D__LINUX__ -DLINUX -I../linux//include -DOSC_HOST_LITTLE_ENDIAN -I/usr/include/ -I/usr/local/include/ -I/opt/local/include/ -I/usr/local/include/ -I./ -isysroot  -O3  -L/opt/local/lib/ -lglut -lassimp -lfreeimage  -lm -I./ -I$LINUX_DIR -I$SRC_DIR  $SRC_DIR/*.cpp $LINUX_DIR/*.cpp $EXAMPLE_DIR/*.cpp -o $EXAMPLE_DIR/Basic

g++ -Wno-write-strings $SRC_DIR/*.cpp $LINUX_DIR/*.cpp $EXAMPLE_DIR/*.cpp -std=c++11 -I/usr/include/c++/4.6/x86_64-linux-gnu/ -I/usr/include/x86_64-linux-gnu/ -I/usr/include/c++/4.6/ -I/usr/include/ -I/usr/local/include/ -isysroot -I./ -I$LINUX_DIR -I$SRC_DIR -I$LINUX_DIR/include  -o $EXAMPLE_DIR/TextureExample -lglut -lGL -lassimp -lfreeimage -lm 

#cd $EXAMPLE_DIR && ./Model && rm ./Model
cd $EXAMPLE_DIR && pwd && ./TextureExample && rm ./TextureExample

