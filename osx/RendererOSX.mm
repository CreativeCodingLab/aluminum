#import <Aluminum/Includes.hpp>
//#import <Cocoa/Cocoa.h>
//#import "RendererOSX.h"
#import <chrono>
#import <iostream>
#import <Aluminum/Texture.hpp>
//#import "VideoPlayer.h"

using std::cout;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;
using std::chrono::milliseconds;
using std::chrono::high_resolution_clock;
using std::string;
//using aluminum::Texture;

RendererOSX::RendererOSX() {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    printf("in RendererNativeOSX constructor\n");
    [pool release];
}

int RendererOSX::start() {
    return RendererOSX::start([[[NSProcessInfo processInfo] processName] UTF8String], 50, 40, 500, 500);
}

int RendererOSX::start(std::string _name, int x, int y, int w, int h) {

    view = [CocoaGL start:this
                     name:[NSString stringWithUTF8String:_name.c_str()]
                        x:x
                        y:y
                        w:w
                        h:h
    ];


    initializeKeyArrays();
    initializeMouseInfo();

    return 0;
}


CocoaGL *RendererOSX::makeGLView(int w, int h) {

    view = [CocoaGL createGLView:this
                               w:w
                               h:h
    ];


    initializeKeyArrays();
    initializeMouseInfo();

    return view;
}


/* called when CocoaGL first initializes the OpenGL context */
void RendererOSX::createDefaultVAO() {
    glGenVertexArrays(1, &_glVaoID);
}

/* called when CocoaGL first initializes the OpenGL context, but may need to be explicitly called before linking shaders */
void RendererOSX::bindDefaultVAO() {
    glBindVertexArray(_glVaoID);
}


long RendererOSX::nowPlusMillis(long millis) {
    return currentTick + (millis * 1000000);
}

long RendererOSX::millisToNano(long millis) {
    return (millis * 1000000);
}

long RendererOSX::now() {
    return currentTick;
}

long RendererOSX::setStartTick() {
    high_resolution_clock::time_point clock = high_resolution_clock::now();
    //startTick = duration_cast<milliseconds>(clock.time_since_epoch()).count();
    startTick = duration_cast<nanoseconds>(clock.time_since_epoch()).count();
    currentTick = 0;
    return startTick;
}

long RendererOSX::tick() {
    //currentTick = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count() - startTick;
    currentTick = duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count() - startTick;
    return currentTick;
}

/*
//double globalTickVal;
void RendererOSX::tick() {
  NSDate *date = [NSDate date];
  //aluminum::Behavior::globalTickValue = [date timeIntervalSinceReferenceDate];
  //aluminum::Behavior::globalTickVal = [date timeIntervalSinceReferenceDate] - aluminum::Behavior::globalStartVal;
  //printf("globalTickVal = %f\n", aluminum::Behavior::globalTickVal);
  using namespace aluminum;

  Behavior::setTickVal(1.1);
}
*/


void RendererOSX::onFrame() {
}

void RendererOSX::onCreate() {
}

void RendererOSX::onReshape() {
}


CocoaGL *RendererOSX::getView() {
    return view;
}

void RendererOSX::toggleFullScreen() {
    [view toggleFullScreen];
}

void RendererOSX::mouseDragged(int px, int py) {
    //printf("dragging...\n");

    if (px != mouseX || py != mouseY) {
        previousMouseX = mouseX;
        previousMouseY = mouseY;
        mouseX = px;
        mouseY = py;
    }
    isDragging = true;
    isPressing = false;
}

void RendererOSX::mouseDown(int px, int py) {
    //printf("down...\n");

    previousMouseX = mouseX;
    previousMouseY = mouseY;
    mouseX = px;
    mouseY = py;
    isPressing = true;
    isReleasing = false;
}

void RendererOSX::mouseUp(int px, int py) {
    //printf("up...\n");
    previousMouseX = mouseX;
    previousMouseY = mouseY;
    mouseX = px;
    mouseY = py;
    isReleasing = true;
    isDragging = false;
    isPressing = false;
}

void RendererOSX::mouseMoved(int px, int py) {
    //printf("moving...\n");
    previousMouseX = mouseX;
    previousMouseY = mouseY;
    mouseX = px;
    mouseY = py;
    isMoving = true;
}

void RendererOSX::handleMouse() {
}

void RendererOSX::handleKeys() {
    /*
     //you can get the modifiers like so:
     bool shiftDown = keysDown[kVK_Shift];
     bool commandDown = keysDown[kVK_Command];
     bool optionDown = keysDown[kVK_Option];
     bool controlDown = keysDown[kVK_Control];
     */
}

void RendererOSX::keyDown(char key) {
    keysUp[key] = false;
    keysDown[key] = true;
}

void RendererOSX::keyUp(char key) {
    keysUp[key] = true;
    keysDown[key] = false;
}

void RendererOSX::initializeKeyArrays() {
    for (int i = 0; i < 256; i++) {
        keysDown[i] = false;
        keysUp[i] = false;
    }
}

void RendererOSX::initializeMouseInfo() {
    isPressing = false;
    isDragging = false;
    isReleasing = false;
    mouseX = 0;
    mouseY = 0;
    previousMouseX = 0;
    previousMouseY = 0;
}





