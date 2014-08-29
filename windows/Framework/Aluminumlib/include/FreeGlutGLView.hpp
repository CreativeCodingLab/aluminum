#pragma once
#ifndef INCLUDE_AL_FGGLV_HPP
#define INCLUDE_AL_FGGLV_HPP

#include "Includes.hpp"
namespace aluminum {
    class FreeGlutGLView {

	    public:
		    FreeGlutGLView();
			//~FreeGlutGLView();
		    FreeGlutGLView* glView;

		    static FreeGlutGLView* start(void* _renderer, std::string name);
		    static FreeGlutGLView* start(void* _renderer);

    };
}
#endif

