

#ifndef INCLUDE_RENDERER_IOS_H
#define INCLUDE_RENDERER_IOS_H

#import <Aluminum/Includes.hpp>
#import <Aluminum/AppDelegate.h>
#import <Aluminum/EAGLView.h>

using std::string;
using glm::ivec2;

class RendererIOS {

public:

  int width = -1;
  int height = -1;

  RendererIOS();

  int start();

  virtual void onCreate();
  virtual void onFrame();

  int frameCount = 0;

  bool multitouchOn = true;
  bool doubleTapGestureOn = false;
  bool pinchGestureOn = false;
  bool longPressGestureOn = false;
  bool panGestureOn = false;
  bool swipeUpGestureOn = false;
  bool swipeDownGestureOn = false;

  virtual void installInteractions(EAGLView* ev);

  virtual void touchBegan(ivec2 mouse);
  virtual void touchMoved(ivec2 prevMouse, ivec2 mouse);
  virtual void touchEnded(ivec2 mouse);

  virtual void longPress(ivec2 mouse);
  virtual void pinch(float scale);
  virtual void pinchEnded();
  virtual void doubleTap();
  virtual void swipeUp();

  RendererIOS& useMultitouch(bool _on=true);
  RendererIOS& useDoubleTap(bool _on=true);
  RendererIOS& usePinch(bool _on=true);
  RendererIOS& useLongPress(bool _on=true);
  RendererIOS& usePan(bool _on=true);
  RendererIOS& useSwipeUp(bool _on=true);
  RendererIOS& useSwipeDown(bool _on=true);



  //shouldn't these both be in ResourceHandler?
  const char* contentsOfFile(string& file);
  string pathToResource(const string& resource, const string& type);

  GLuint getDefaultFrameBuffer();
  AppDelegate* getAppDelegate();
  EAGLView* getEAGLView();


};

#endif