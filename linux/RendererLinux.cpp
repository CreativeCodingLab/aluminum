#include "Aluminum/Includes.hpp"

//namespace Aluminum {
using std::chrono::duration_cast;
using std::chrono::nanoseconds;
using std::chrono::milliseconds;
using std::chrono::system_clock;


RendererLinux::RendererLinux() { 
	printf("in RendererLinux constructor\n");
}

void RendererLinux::start() {
	printf("in RendererLinux::start()\n");
	FreeGlutGLView::start(this);
}
void RendererLinux::start(std::string name) {
	printf("in RendererLinux::start(%s)\n", name.c_str());
	FreeGlutGLView::start(this, name);
}



void RendererLinux::onFrame() { printf("you should overwrite me in the subclass!\n"); }
void RendererLinux::onCreate() { }

//prob can delete this "accessView" method now... 
void RendererLinux::accessView() {
	printf("about to access freeglutGL view...\n");
	//[view printView];
}

long RendererLinux::nowPlusMillis(long millis) {
	return currentTick + millis;
}

long RendererLinux::millisToNano(long millis) {
	return millis;
}


long RendererLinux::now() {
	return currentTick;
}

long RendererLinux::setStartTick() {
	system_clock::time_point clock = system_clock::now();
	startTick = duration_cast<milliseconds>(clock.time_since_epoch()).count();
	//startTick = duration_cast<nanoseconds>(clock.time_since_epoch()).count();
	currentTick = 0;
	std::cout << "startTick is " << startTick << std::endl;
	return startTick;
}

long RendererLinux::tick() {
	currentTick = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - startTick;
	//currentTick = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count() - startTick;
	//printf("currentTick is %i\n", currentTick);
	return currentTick;
}

void RendererLinux::mouseDragged(int px, int py) {}
void RendererLinux::mouseDown(int px, int py) {}
void RendererLinux::mouseUp(int px, int py) {}
void RendererLinux::mouseMoved(int px, int py) {}
void RendererLinux::keyboard(unsigned char key, int x, int y) {}
void RendererLinux::specialkeys(int key, int x, int y) {}


//}

