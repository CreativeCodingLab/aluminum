#pragma once
#ifndef INCLUDE_WIN32_INCLUDES_HPP
#define INCLUDE_WIN32_INCLUDES_HPP


#define WIN32_LEAN_AND_MEAN


#define _USE_MATH_DEFINES // for C++
#include <cmath>


//#define GLM_FORCE_RADIANS   // To get GLM to shutup
#include <GL/glew.h>
#pragma comment(lib, "glew32.lib")
#define GLEW_STATIC 

#define FREEIMAGE_LIB
#include <FreeImage.h>
#pragma comment(lib, "FreeImaged.lib")


#include <GL/freeglut.h>


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

