#ifndef ALLOCORE_RENDERER_NATIVE_LINUX_AGF 
#define ALLOCORE_RENDERER_NATIVE_LINUX_AGF

#include <Aluminum/Includes.hpp>
#include <string>

//namespace Aluminum {

class RendererLinux { 
  
public:

  RendererLinux();

  //basic windowing
  void start();
  void start(std::string name);

  virtual void onCreate();
  virtual void onFrame();

  int width;
  int height; 

 int frameCount = 0;

                long startTick;
                long currentTick;

//  FreeGlutGLView* view;
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



  

  
protected:
  
private:

};

//}

#endif

