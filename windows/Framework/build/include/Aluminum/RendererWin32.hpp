#pragma once
#ifndef ALLOCORE_RENDERER_NATIVE_WIN32
#define ALLOCORE_RENDERER_NATIVE_WIN32

#include "Aluminum/Includes.hpp"

namespace aluminum {
    class RendererWin32 {
    public:
        RendererWin32();
        void start();
        void start(std::string name);
        virtual void onCreate();
        virtual void onFrame();

        int width;
        int height;

        void accessView();

        virtual void mouseDragged(int px, int py);
        virtual void mouseDown(int px, int py);
        virtual void mouseUp(int px, int py);
        virtual void mouseMoved(int px, int py);

    protected:
    private:
    };
}

#endif
