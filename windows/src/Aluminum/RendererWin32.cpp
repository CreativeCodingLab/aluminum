#pragma once
#include "Aluminum/RendererWin32.hpp"
#include "Aluminum/FreeGlutGLView.hpp"

using std::chrono::duration_cast;
using std::chrono::nanoseconds;
using std::chrono::milliseconds;
using std::chrono::high_resolution_clock;


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

	void RendererWin32::start(std::string name, int _width, int _height) {
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

    long RendererWin32::nowPlusMillis(long millis) {
        return currentTick + (millis * 1000000);
    }

    long RendererWin32::millisToNano(long millis) {
        return (millis * 1000000);
    }

    long RendererWin32::now() {
        return currentTick;
    }

    long RendererWin32::setStartTick() {
        high_resolution_clock::time_point clock = high_resolution_clock::now();
        //startTick = duration_cast<milliseconds>(clock.time_since_epoch()).count();
        startTick = duration_cast<nanoseconds>(clock.time_since_epoch()).count();
        currentTick = 0;
        return startTick;
    }

    long RendererWin32::tick() {
        //currentTick = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count() - startTick;
        currentTick = duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count() - startTick;
        return currentTick;
    }

    void RendererWin32::mouseDragged(int px, int py) {}
    void RendererWin32::mouseDown(int px, int py) {}
    void RendererWin32::mouseUp(int px, int py) {}
    void RendererWin32::mouseMoved(int px, int py) {}
	  void RendererWin32::keyboard(unsigned char key, int x, int y) {}

}




