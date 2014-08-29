
#import <Aluminum/RendererIOS.h>
#import <Aluminum/ViewController.h>
RendererIOS::RendererIOS() {}

int RendererIOS::start() {
  @autoreleasepool {

    printf("rendererIOS start\n");
    [AppDelegate setRenderer:this];

    return UIApplicationMain(nil, nil, nil, NSStringFromClass([AppDelegate class]));
    return 1;


  }
}


RendererIOS& RendererIOS::useMultitouch(bool on) {
  multitouchOn = on;
  return *this;
}

RendererIOS& RendererIOS::useDoubleTap(bool on) {
  doubleTapGestureOn = on;
  return *this;
}

RendererIOS& RendererIOS::usePinch(bool on) {
  pinchGestureOn = on;
  return *this;
}

RendererIOS& RendererIOS::useLongPress(bool on) {
  longPressGestureOn = on;
  return *this;
}

RendererIOS& RendererIOS::usePan(bool on) {
  panGestureOn = on;
  return *this;
}

RendererIOS& RendererIOS::useSwipeUp(bool on) {
  swipeUpGestureOn = on;
  return *this;
}

RendererIOS& RendererIOS::useSwipeDown(bool on) {
  swipeDownGestureOn = on;
  return *this;
}


GLuint RendererIOS::getDefaultFrameBuffer() {

    AppDelegate* ad = (AppDelegate *)[[UIApplication sharedApplication] delegate];

    EAGLView *ev = ad.glView;

    return (GLuint)[ev getDefaultFramebuffer];
}


AppDelegate* RendererIOS::getAppDelegate() {

  return (AppDelegate *)[[UIApplication sharedApplication] delegate];
}

EAGLView* RendererIOS::getEAGLView() {
  return getAppDelegate().glView;
}





/*
mouseBegan[id] = true
mouseEnded[id] = false
mousePrev[id] = 45
mouseCurrent[id] = 46
longPress = true
longPressPosition = x,y
pinching = true
pinchScale = 1.2
-- or does the display synch take care of this??
 (that is, do we need to keep track of touches separately from the callbacks - or is there some built in run loop that will make sure that touches + other events happen before rendering...?)
*/

//virtual method that must be implemented (if you want anything to happen)
void RendererIOS::onFrame() {}
void RendererIOS::onCreate() {}

//virtual methods to be optionally implemented by subclass,
//probably we will want to think about how to do this in a more threaded way like with mouse movements in RendererOSX.
void RendererIOS::touchBegan(ivec2 mouse) {}
void RendererIOS::touchMoved(ivec2 prev, ivec2 mouse) {}
void RendererIOS::touchEnded(ivec2 mouse) {}
void RendererIOS::longPress(ivec2 mouse) {}
void RendererIOS::pinch(float scale) {}
void RendererIOS::doubleTap() {}
void RendererIOS::pinchEnded() {}
void RendererIOS::swipeUp() {}

// Override this method to install custom interactions located an Objective-C++ class.
void RendererIOS::installInteractions(EAGLView* ev) {}


