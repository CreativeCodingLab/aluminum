
***
Note should revisit - this readme is probably out of data (4/27/14)
***

To install gcc-4.7:

sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-4.7
sudo apt-get install g++-4.7

sudo ln -s /usr/bin/gcc-4.7 /usr/bin/gcc
sudo ln -s /usr/bin/g++-4.7 /usr/bin/g++

Remove assimp2:
sudo apt-get remove assimp-utils

Download + compile + install assimp3:

git clone git://github.com/assimp/assimp.git
cd assimp
cmake .
make -j5
sudo make install

Had an issue with compiling assimp3... followed this...
http://blogmonsalve.blogspot.com/2012/10/compilando-assimp-en-kubuntu-1204.html

Seems like there are issues if libassimp2 is still around... not sure why... maybe misreading a header file???
aiFace was returning errors & causing segfaults...

I completely purged it using:
sudo apt-get purge libassimp2
sudo apt-get purge libassimp-dev
or some combination like that...

Install FreeGlut2.6+ and GL3:

 sudo apt-get install freeglut3-dev

Copy http://www.opengl.org/registry/api/gl3.h to /usr/include/GL3/gl3.h (NOTE this should be necessary with newest version of freeglut!!!)

****
IMPORTANT IF USING AN AMD GRAPHICS CARD!
****
For some reason a bad logging call is made in Program.cpp which causes the program to SEGFAULT if using an AMD graphics card. 
Uncomment the line AMD_HACK in scripts/makeStaticLibrary.sh to enable a bypass of this call
