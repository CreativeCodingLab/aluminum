
#include "Texture.hpp"

#include "Includes.hpp"

#include "MeshBuffer.hpp"
#include "MeshData.hpp"
#include "MeshUtils.hpp"
#include "Program.hpp"
#include "Shapes.hpp"

#import "ResourceHandler.h"

#import "RendererIOS.h"
#import "AppDelegate.h"

#import "CaptureManager.h"

#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

using namespace aluminum;

class CaptureExample : public RendererIOS {
public:
  
  Program program;
  
  int div = 6;
  int numPts;
  vec3* vs;
  vec4* cs;
  unsigned int* indices;
  
  GLint posLoc=0;
  // GLint texCoordLoc=1;
  GLint colLoc=1;
  
  mat4 proj;
  mat4 view;
  
  MeshData md;
  MeshBuffer mb1;
  bool bufferReady = false;
  
  CaptureManager* cm;
  Texture safeTexture;
  
  ResourceHandler rh;
  
  void loadProgram(Program &p, const std::string& name) {
    
    p.create();
    
    string sv = rh.pathToResource(name, "vsh");
    // cout << "path of vertex shader is: " << sv << endl;
    
    p.attach(rh.contentsOfFile(sv), GL_VERTEX_SHADER);
    
    glBindAttribLocation(p.id(), posLoc, "vertexPosition");
    //  glBindAttribLocation(p.id(), texCoordLoc, "vertexTexCoord");
    glBindAttribLocation(p.id(), colLoc, "vertexColor");
    
    string sp = rh.pathToResource(name, "fsh");
    // cout << "path of vertex shader is: " << sp << endl;
    
    p.attach(rh.contentsOfFile(sp), GL_FRAGMENT_SHADER);
    
    p.link();
  }
  
  virtual void onCreate() {
    
    loadProgram(program, "basic");
    
    cm = [[CaptureManager alloc] init:AVCaptureSessionPresetLow side:AVCaptureDevicePositionBack];
    [cm startCapture];
    
    proj = glm::perspective(45.0, 1.0, 0.1, 100.0);
    view = glm::lookAt(vec3(0.0,0.0,3), vec3(0,0,0), vec3(0,1,0) );
    
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_TEXTURE_2D);
    //glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
  }
  
  void initMeshData() {
    
    printf("in initMeshData()\n");
    numPts = ((cm.captureTexture.width * cm.captureTexture.height) / div) * 2;
    vs = new vec3[numPts];
    cs = new vec4[numPts];
    indices = new unsigned int[numPts];
    for (int i = 0; i < numPts; i++) {
      indices[i] = i;
    }
    
    md.vertex(vs, numPts);
    md.color(cs, numPts);
    md.index(indices, numPts);
    
    mb1.init(md, posLoc, -1, -1, colLoc);
  }
  
  void processFrame() {
    
    if (bufferReady == false) {
      initMeshData();
      bufferReady = true;
    }
    
    int bw = cm.captureTexture.width;
    int bh = cm.captureTexture.height;
    float sx = -1;
    float sy = -1;
    float xinc = 2.0/bw;
    float yinc = 2.0/bh;
    
    int bufferIdx = 0;
    int idx = 0;
    for (int i = 0; i < bh; i+=div) {
      for (int j = 0; j < bw; j+=div) {
        
        idx = (i * bw * 4) + (j * 4);
        
        float r = cm.pixels[idx+2]/255.0;
        float g = cm.pixels[idx+1]/255.0;
        float b = cm.pixels[idx+0]/255.0;
        
        float perc = (r+g+b) / (3.0);
        float ang = perc * (M_PI * 2);
        float mag = perc * 0.3;
        
        float xp = cos(ang) * mag;
        float yp = sin(ang) * mag;
        
        vs[bufferIdx] =   vec3(sx + xinc*j - xp, sy + yinc*i - yp, 0.0);
        vs[bufferIdx+1] = vec3(sx + xinc*j + xp, sy + yinc*i + yp, 0.0);
        
        cs[bufferIdx] = vec4(r, g, b, 0.5);
        cs[bufferIdx+1] = vec4(r, g, b, 0.5);
        
        bufferIdx += 2;
        
        
      }
    }
    
    md.vertices().clear();
    md.vertex(vs, numPts);
    md.colors().clear();
    md.color(cs, numPts);
    
    mb1.update(md);
  }
  
  virtual void onFrame(){
    // printf("frameCount = %d\n", frameCount);
    //handleKeys();
    //handleMouse();
    
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    
    if (frameCount % 200 == 0 && frameCount > 0) {
      //[cm toggleCapture];
    }
    
    if ([cm nextFrame]) {
      processFrame();
    }
    
    
    program.bind(); {
      
      glUniformMatrix4fv(program.uniform("mv"), 1, 0, ptr(view));
      glUniformMatrix4fv(program.uniform("proj"), 1, 0, ptr(proj));
      
      //  mb1.draw();
      mb1.drawLines();
      
    } program.unbind();
    
  }
  
  /*
   void handleMouse() {
   if (isDragging) {
   printf("in Basic: mouseDragged %d/%d\n", mouseX, mouseY);
   }
   
   if (isMoving) {
   printf("in Basic: mouseMoved %d/%d\n", mouseX, mouseY);
   }
   
   if (isPressing) {
   printf("in Basic: mouseDown %d/%d\n", mouseX, mouseY);
   }
   
   
   if (isReleasing) {
   printf("in Basic: mouseUp %d/%d\n", mouseX, mouseY);
   }
   
   isDragging = false;
   isMoving = false;
   isPressing = false;
   isReleasing = false;
   }
   
   void handleKeys() {
   
   if (keysDown[kVK_ANSI_A]) {
   printf("you pressed an 'A'! \n");
   keysDown[kVK_ANSI_A] = false;
   }
   
   if (keysUp[kVK_ANSI_A]) {
   printf("you released an 'A'! \n");
   keysUp[kVK_ANSI_A] = false;
   }
   }
   */
  
};


int oomain(int argc, char *argv[]){
  CaptureExample().start();
}
