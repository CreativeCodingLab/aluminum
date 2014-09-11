#include "Aluminum/Program.hpp"
#include "Aluminum/MeshBuffer.hpp"
#include "Aluminum/MeshData.hpp"
#include "Aluminum/Utils.hpp"
#include "Aluminum/MeshUtils.hpp"
#include "Aluminum/ResourceHandler.h"
#include "Aluminum/FBO.hpp"


using namespace aluminum;

class Cellular : public RendererOSX {
  
public:
  
  bool caRunning = true;
  bool toggleFBOs = true;
  
  int numCols = 100;
  int numRows = 100;
  
  Program caProgram, outputProgram;
  FBO fboA, fboB;
  MeshBuffer clipRect, outputRect;
  
  GLint posLoc=0;
  GLint texCoordLoc=1;
  
  mat4 proj, view, model;
  
  ResourceHandler rh;
  
  void loadProgram(Program &p, const std::string& name) {
    
    //bindDefaultVAO();
    
    p.create();
    
    string sv = rh.pathToResource(name, "vsh");
    p.attach(rh.contentsOfFile(sv), GL_VERTEX_SHADER);
    glBindAttribLocation(p.id(), posLoc, "vertexPosition");
    glBindAttribLocation(p.id(), texCoordLoc, "vertexTexCoord");
    
    string sp = rh.pathToResource(name, "fsh");
    p.attach(rh.contentsOfFile(sp), GL_FRAGMENT_SHADER);
    p.link();
  }
  
  void initFBOs() {
    
    //bindDefaultVAO();

    fboA.create(numCols, numRows);
    fboB.create(numCols, numRows);
    
    //make sure textures are on repeat so the system will loop around walls instead of die at the edges
    fboA.texture.wrapMode(GL_REPEAT);
    fboB.texture.wrapMode(GL_REPEAT);
    
    //add some random data to the first texture to start the CA system
    Utils::randomSeed();
    GLubyte val;
    
    for (int i = 0; i < numCols * numRows * 4; i+=4) {
      if ( Utils::random() > 0.6) {
        val = 0;
      } else {
        val = 255;
      }
      
      fboB.texture.data[i] = fboB.texture.data[i+1] = fboB.texture.data[i+2] = val;
      fboB.texture.data[i+3] = 255;
    }
    
    fboB.texture.updateData();
    
  }
  
  virtual void onCreate() {
    
    rh.loadProgram(caProgram, "CA", 0, -1, 1, -1);
    rh.loadProgram(outputProgram, "texture", 0, -1, 1, -1);
    
    clipRect.init(MeshUtils::makeClipRectangle(), posLoc, -1, texCoordLoc, -1);
    outputRect.init(MeshUtils::makeClipRectangle(), posLoc, -1, texCoordLoc, -1);
    
    initFBOs();
    
    proj = glm::mat4();
    view = glm::mat4();
    model = glm::mat4();
  }
  
  void drawOutput(Texture &t) {
    
    glViewport(0, 0, width, height);
    
    outputProgram.bind(); {
      
      glUniformMatrix4fv(outputProgram.uniform("model"), 1, 0, ptr(model));
      glUniformMatrix4fv(outputProgram.uniform("view"), 1, 0, ptr(view));
      glUniformMatrix4fv(outputProgram.uniform("proj"), 1, 0, ptr(proj));
      glUniform1i(outputProgram.uniform("tex0"), 0);
      
      t.bind(GL_TEXTURE0); {
        outputRect.draw();
      } t.unbind(GL_TEXTURE0);
      
    } outputProgram.unbind();
  }
  
  
  void updateSystem(FBO &fbo, Texture &t) {
    
    caProgram.bind(); {
      
    
    glUniform1f(caProgram.uniform("du"), 1.0/numCols);
    
    glUniform1f(caProgram.uniform("dv"), 1.0/numRows);
      
      glUniform1i(caProgram.uniform("tex"), 0);
      
      fbo.bind(); {
        t.bind(GL_TEXTURE0); {
          clipRect.draw();
        } t.unbind(GL_TEXTURE0);
      } fbo.unbind();
      
      
    } caProgram.unbind();
  }
  
  virtual void onFrame(){
    
    handleKeys();
    handleMouse();
    
    if (caRunning) {
      
      if (toggleFBOs) {
        updateSystem(fboA, fboB.texture);
        drawOutput(fboA.texture);
      } else {
        updateSystem(fboB, fboA.texture);
        drawOutput(fboB.texture);
      }
      
      toggleFBOs = !toggleFBOs;
      
    } else {
      
      if (toggleFBOs) {
        drawOutput(fboA.texture);
      } else {
        drawOutput(fboB.texture);
      }
    }
    
  }
  
  void handleMouse() {
    if (isDragging) {
      
      caRunning = false;
      
      
      if (mouseX >= 0 && mouseX < width && mouseY >= 0 && mouseY < height) {
        int cax = (int) ((mouseX * numCols) / width );
        int cay = (int) ((mouseY * numRows) / height );
        
        GLubyte subdata[4] = { 255, 255, 0, 255 };
        
        fboA.texture.updateData(subdata, cax, cay, 1, 1);
        fboB.texture.updateData(subdata, cax, cay, 1, 1);
        
      }
    }
    
    if (isMoving) {
      //printf("in Basic: mouseMoved %d/%d\n", mouseX, mouseY);
    }
    
    if (isPressing) {
      caRunning = false;
      //printf("in Basic: mouseDown %d/%d\n", mouseX, mouseY);
    }
    
    
    if (isReleasing) {
      caRunning = true;
      //printf("in Basic: mouseUp %d/%d\n", mouseX, mouseY);
    }
    
    isDragging = false;
    isMoving = false;
    isPressing = false;
    isReleasing = false;
  }
  
  void handleKeys() {
    
    if (keysDown[kVK_ANSI_A]) {
      keysDown[kVK_ANSI_A] = false;
      
      caRunning = !caRunning;
    }
    
    if (keysUp[kVK_ANSI_A]) {
      keysUp[kVK_ANSI_A] = false;
    }
  }
  
};

int main(){
  return Cellular().start("aluminum::Cellular Automata", 100, 100, 800, 600);
}
