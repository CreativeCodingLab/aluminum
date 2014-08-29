# =========================================================================
# Freetype GL - A C OpenGL Freetype engine
# Platform:    Any
# WWW:         http://code.google.com/p/freetype-gl/
# -------------------------------------------------------------------------
# Copyright 2011,2012 Nicolas P. Rougier. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#  1. Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
#
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY NICOLAS P. ROUGIER ''AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# EVENT SHALL NICOLAS P. ROUGIER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# The views and conclusions contained in the software and documentation are
# those of the authors and should not be interpreted as representing official
# policies, either expressed or implied, of Nicolas P. Rougier.
# =========================================================================
PLATFORM        = $(shell uname)
CC              = gcc
CFLAGS          = -Wall `freetype-config --cflags` -I/usr/X11/include -I/usr/X11/include/freetype2/freetype/ -g -O0
LIBS            = -lGL -lglut -lGLU -lm \
                  `freetype-config --libs` -lfontconfig
ifeq ($(PLATFORM), Darwin)
    LIBS        = -framework OpenGL -framework GLUT -lm ./libfreeimage.dylib\
                   `freetype-config --libs` -L /usr/X11/lib -lfontconfig
endif

HEADERS   := $(wildcard *.h)
SOURCES   := $(wildcard *.c)
SOURCES   := $(filter-out CreateTextureFontAtlas.c, $(SOURCES))
SOURCES   := $(filter-out CreateSignedDistanceFontAtlas.c, $(SOURCES))
OBJECTS   := $(SOURCES:.c=.o)

all: CreateSignedDistanceFontAtlas CreateTextureFontAtlas

# %.o : %.c
#     @echo "Building $@... "
#     @$(CC) -c $(CFLAGS) $< -o $@


CreateSignedDistanceFontAtlas: CreateSignedDistanceFontAtlas.o $(OBJECTS) $(HEADERS)
    @echo "Building $@... "
    @$(CC) $(OBJECTS) $@.o $(LIBS) -o $@

CreateTextureFontAtlas: CreateTextureFontAtlas.o $(OBJECTS) $(HEADERS)
    @echo "Building $@... "
    @$(CC) $(OBJECTS) $@.o $(LIBS) -o $@

clean:
    @-rm -f *.o
