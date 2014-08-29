#include "Aluminum/Includes.hpp"

//namespace Aluminum {

RendererLinux::RendererLinux() { 
  printf("in RendererLinux constructor\n");
}

void RendererLinux::start() {
	printf("in RendererLinux::start()\n");
  FreeGlutGLView::start(this);
}
void RendererLinux::start(std::string name) {
	printf("in RendererLinux::start(name)\n");
  FreeGlutGLView::start(this, name);
}



void RendererLinux::onFrame() { printf("you should overwrite me in the subclass!\n"); }
void RendererLinux::onCreate() { }

//prob can delete this "accessView" method now... 
void RendererLinux::accessView() {
  printf("about to access freeglutGL view...\n");
  //[view printView];
}


void RendererLinux::mouseDragged(int px, int py) {}
void RendererLinux::mouseDown(int px, int py) {}
void RendererLinux::mouseUp(int px, int py) {}
void RendererLinux::mouseMoved(int px, int py) {}



//}

