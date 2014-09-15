#pragma once
#ifndef INCLUDE_AL_FGGLV_HPP
#define INCLUDE_AL_FGGLV_HPP

#include "Aluminum/Includes.hpp"

namespace aluminum {
    class FreeGlutGLView {

	    public:
		    FreeGlutGLView();
			//~FreeGlutGLView();
		    FreeGlutGLView* glView;
			static FreeGlutGLView* start(void* _renderer, std::string name, int _width, int _height);
		    static FreeGlutGLView* start(void* _renderer, std::string name);
		    static FreeGlutGLView* start(void* _renderer);
			

			// Got this bit of code from http://stackoverflow.com/questions/16299029/resolution-of-stdchronohigh-resolution-clock-doesnt-correspond-to-measureme
			struct HighResClock
			{
				typedef long long                               rep;
				typedef std::nano                               period;
				typedef std::chrono::duration<rep, period>      duration;
				typedef std::chrono::time_point<HighResClock>   time_point;
				static const bool is_steady = true;

				static time_point now();
			};
    };
}
#endif

