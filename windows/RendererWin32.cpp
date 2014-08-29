#pragma once
#include "RendererWin32.hpp"
#include "FreeGlutGLView.hpp"

namespace aluminum {
    RendererWin32::RendererWin32() { 
      printf("in RendererWin32 constructor\n");
    }

    void RendererWin32::start() {
	    printf("in RendererWin32::start()\n");
      FreeGlutGLView::start(this);
    }
    void RendererWin32::start(std::string name) {
	    printf("in RendererWin32::start(name)\n");
      FreeGlutGLView::start(this, name);
    }



    void RendererWin32::onFrame() { printf("you should overwrite me in the subclass!\n"); }
    void RendererWin32::onCreate() { }

    //prob can delete this "accessView" method now... 
    void RendererWin32::accessView() {
      printf("about to access freeglutGL view...\n");
      //[view printView];
    }


    void RendererWin32::mouseDragged(int px, int py) {}
    void RendererWin32::mouseDown(int px, int py) {}
    void RendererWin32::mouseUp(int px, int py) {}
    void RendererWin32::mouseMoved(int px, int py) {}

}




