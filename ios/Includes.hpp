#ifndef INCLUDE_IOS_INCLUDES_HPP
#define INCLUDE_IOS_INCLUDES_HPP

#define BUILD_IOS 1
// #define GLM_FORCE_RADIANS   // To get GLM to shutup // Until code base is updated, do not uncomment

#ifdef __OBJC__
#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
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
//#include <glm/core/_fixes.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/random.hpp>
#include <glm/gtx/random.hpp>

//saves me from typing "glm::value_ptr"
#define ptr( a ) glm::value_ptr(a)

#endif

