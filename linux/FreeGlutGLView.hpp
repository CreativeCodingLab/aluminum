#ifndef INCLUDE_AL_FGGLV_HPP
#define INCLUDE_AL_FGGLV_HPP


//#include "FreeGlutGLView.hpp"
//#include "RendererWin32.hpp"

//these two first...
#define GL_GLEXT_PROTOTYPES
#include <GL/freeglut.h>

//then the rest...
#include "Aluminum/Includes.hpp"

//#define GL3_PROTOTYPES
//#include <GL3/gl3.h>
//#define __gl_h_
//#include <GL/freeglut.h>

#include <stdio.h>
#include <string>

//namespace Aluminum {

class FreeGlutGLView {

	public:
		FreeGlutGLView();

		FreeGlutGLView* glView;

		static FreeGlutGLView* start(void* _renderer, std::string name);
		static FreeGlutGLView* start(void* _renderer);

};

//}

#endif

