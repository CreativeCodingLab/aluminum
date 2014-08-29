#ifndef INCLUDE_LINUX_INCLUDES_HPP
#define INCLUDE_LINUX_INCLUDES_HPP

//#define GL3_PROTOTYPES
//#include <GL3/gl3.h>
//#define __gl_h_

#define GLM_FORCE_RADIANS

//these two first...
#define GL_GLEXT_PROTOTYPES
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
//#include <glm/core/_fixes.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/random.hpp>

//#include "assimp/Importer.hpp"
//#include "assimp/scene.h"
//#include "assimp/postprocess.h"
//#include "assimp/cimport.h"
//#include "assimp/types.h"
//#include "assimp/matrix4x4.h"

//#include "FreeImage.h"

#include "Aluminum/FreeGlutGLView.hpp"
#include "Aluminum/RendererLinux.hpp"

//saves me from typing "glm::value_ptr"
#define ptr( a ) glm::value_ptr(a)

#endif

