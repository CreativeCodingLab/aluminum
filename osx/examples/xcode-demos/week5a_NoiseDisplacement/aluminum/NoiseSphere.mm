
#include "Aluminum/Includes.hpp"

#include "Aluminum/RendererOSX.h"
#include "Aluminum/MeshBuffer.hpp"
#include "Aluminum/MeshData.hpp"
#include "Aluminum/MeshUtils.hpp"
#include "Aluminum/Program.hpp"
#include "Aluminum/Texture.hpp"
#include "Aluminum/Behavior.hpp"
#include "Aluminum/Shapes.hpp"
#include "Aluminum/ResourceHandler.h"
#include "objload.h"

using namespace aluminum;

class NoiseSphere : public RendererOSX {
public:
  
  
  vec3 diffuse = vec3(1.0,0.4,0.0);
  vec3 specular = vec3(1.0,1.0,1.0);
  vec3 ambient = vec3(0.0,0.0,0.3);
  mat4 model, view, proj;
  
  
  Program program;
  GLint posLoc=0;
  GLint normalLoc=1;
  
  MeshBuffer mb;
  MeshData mesh;

  Behavior rotateBehavior;
  Behavior lightBehavior;
  
  ResourceHandler rh;
  
  void loadProgram(Program &p, const std::string& name) {
    
    p.create();
    
    p.attach(p.loadText(name + ".vsh"), GL_VERTEX_SHADER);
    glBindAttribLocation(p.id(), posLoc, "vertexPosition");
    glBindAttribLocation(p.id(), normalLoc, "vertexNormal");
    
    p.attach(p.loadText(name + ".fsh"), GL_FRAGMENT_SHADER);
    
    p.link();
  }
  
  
  
  virtual void onCreate() {
    
    rh.loadProgram(program, "noise", posLoc, normalLoc, -1, -1);
    
    int res = 250;
      addSurface(mesh, res, res);
//    addSphere(mesh, 1.0, res, res);
    
//    mb.init(mesh, posLoc, normalLoc, -1, -1);
      mb.init(MeshUtils::makeRectangle(100.0,100.0), posLoc, normalLoc, -1, -1);
    
    proj = glm::perspective(glm::radians(60.0), 1.0, 0.1, 1000.0);
    view = glm::lookAt(vec3(0.0,0.0, -4.0), vec3(0,0,0), vec3(0,1,0) );
    model = glm::mat4();
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0,1.0,1.0,1.0);
    
    rotateBehavior = Behavior(now()).delay(0).length(10000).range(M_PI * 2.0).looping(true).repeats(-1);
  }
  
  void updateModel() {
    
    float total = rotateBehavior.tick(now()).total();
    model = glm::mat4();
    model = glm::rotate(model, total, vec3(0.0f,1.0f,0.0f));
  }
  
  float val = 0.0;
  
  void onFrame() {
  
    val += 0.01;
    
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
   // updateModel();
    
    program.bind(); {
      glUniformMatrix4fv(program.uniform("model"), 1, 0, ptr(model));
      glUniformMatrix4fv(program.uniform("view"), 1, 0, ptr(view));
      glUniformMatrix4fv(program.uniform("proj"), 1, 0, ptr(proj));
      
      glUniform1f(program.uniform("in_val"), val);
     
      mb.draw();
      
    } program.unbind();
  }
};

int main(){ 
  return NoiseSphere().start("aluminum::NoiseSphere", 100, 100, 400, 300);
}
