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

//  FreeGlutGLView* view;
  void accessView();


virtual void mouseDragged(int px, int py);
virtual void mouseDown(int px, int py);
virtual void mouseUp(int px, int py);
virtual void mouseMoved(int px, int py);



  

  
protected:
  
private:

};

//}

#endif

