#ifndef INCLUDE_OSX_INCLUDES_HPP
#define INCLUDE_OSX_INCLUDES_HPP

// #define GLM_FORCE_RADIANS   // To get GLM to shutup // Until code base is updated, do not uncomment

#import <Aluminum/CocoaGL.h>
#import <Aluminum/RendererOSX.h>


#ifdef __OBJC__

#import <OpenGL/gl3.h>
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>
#import <Carbon/Carbon.h>

#endif

#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <string>
#include <chrono>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/random.hpp>
#include <glm/gtx/rotate_vector.hpp>

//saves me from typing "glm::value_ptr"
#define ptr( a ) glm::value_ptr(a)

#endif

