#pragma once
#ifndef ALLOCORE_RENDERER_NATIVE_WIN32
#define ALLOCORE_RENDERER_NATIVE_WIN32

#include "Aluminum/Includes.hpp"

namespace aluminum {
    class RendererWin32 {
    public:
        int width;
        int height;

        long startTick;
        long currentTick;

        RendererWin32();
        void start();
        void start(std::string name);
        void start(std::string name, int _width, int _height);
        virtual void onCreate();
        virtual void onFrame();

        void accessView();

        long setStartTick(); //set base nanos - called by Cocoa at init
        long tick(); //calculate amount of time passed since start of program - called by Cocoa prior to each onFrame
        long now(); //get amount of time passed, assumes calculated already with tick()
        long nowPlusMillis(long millis);
        long millisToNano(long millis);

        virtual void mouseDragged(int px, int py);
        virtual void mouseDown(int px, int py);
        virtual void mouseUp(int px, int py);
        virtual void mouseMoved(int px, int py);
		virtual void keyboard(unsigned char key, int x, int y);

    protected:
    private:
    };
}

#endif
