#include "Aluminum/Includes.hpp"

#include "Aluminum/Program.hpp"
#include "Aluminum/MeshBuffer.hpp"
#include "Aluminum/MeshData.hpp"
#include "Aluminum/Shapes.hpp"
#include "Aluminum/Camera.hpp"
#include "Aluminum/Utils.hpp"
#include "Aluminum/MeshUtils.hpp"
#include "Aluminum/ResourceHandler.h"

using namespace aluminum;

class Lighting : public RendererOSX {
public:
  
  ResourceHandler rh;

  Camera camera;
  Program program;
  GLint posLoc = 0, normalLoc = 1;
  mat4 model1, model2;
  MeshData mesh1, mesh2;
  MeshBuffer mb1, mb2;
  
  
  vec3 l1_diffuse = vec3(1.0,0.0,0.0);
  vec3 l1_specular = vec3(1.0,1.0,1.0);
  
  vec3 l2_diffuse = vec3(0.0,0.0,1.0);
  vec3 l2_specular = vec3(1.0,1.0,1.0);
  
  
  void loadProgram(Program &p, const std::string& name) {
    
    p.create();
    
    p.attach(p.loadText(name + ".vsh"), GL_VERTEX_SHADER);
    glBindAttribLocation(p.id(), posLoc, "vertexPosition");
    glBindAttribLocation(p.id(), normalLoc, "vertexNormal");
    
    p.attach(p.loadText(name + ".fsh"), GL_FRAGMENT_SHADER);
   
    p.link();
  }
 
  
  void onCreate() {
    rh.loadProgram(program, "phong", posLoc, normalLoc, -1, -1);
    
    
    camera = Camera(60.0, (float)width/(float)height, 0.01, 100.0).translateZ(-10.0);
    

    addSphere(mesh1, 2.0, 100, 100);
    addSphere(mesh2, 1.0, 100, 100);
    
    mb1.init(mesh1, posLoc, normalLoc, -1, -1);
    mb2.init(mesh2, posLoc, normalLoc, -1, -1);
    
    model1 = glm::translate(mat4(), vec3(-2,0,0));
    model2 = glm::translate(mat4(), vec3(2,0,0));
  
    
    //camera.printCameraInfo();
    
  }
  
  int dir1 = 1;
  float pos1 = 0.0f;
 
  int dir2 = 1;
  float pos2 = 0.0f;
  
  void draw(mat4 proj, mat4 view) {
    
    /* update light positions */
    
    pos1 += 0.1f * dir1;
    if (pos1 > 15.0 || pos1 < -15.0) {
      dir1 *= -1;
    }
    
    vec3 l1_position = vec3(0.0, pos1, 4.0);
   
    pos2 += 0.5f * dir2;
    if (pos2 > 15.0 || pos2 < -15.0) {
      dir2 *= -1;
    }
    vec3 l2_position = vec3(pos2, 0.0, 5.0);
   
    /* bind our Phong lighting shader */
    
    program.bind(); {
      glUniformMatrix4fv(program.uniform("view"), 1, 0, ptr(view));
      glUniformMatrix4fv(program.uniform("proj"), 1, 0, ptr(proj));
      
      glUniform3fv(program.uniform("l1_position"), 1, ptr(l1_position));
      glUniform3fv(program.uniform("l1_diffuse"), 1, ptr(l1_diffuse));
      glUniform3fv(program.uniform("l1_specular"), 1, ptr(l1_specular));
  
      
      glUniform3fv(program.uniform("l2_position"), 1, ptr(l2_position));
      glUniform3fv(program.uniform("l2_diffuse"), 1, ptr(l2_diffuse));
      glUniform3fv(program.uniform("l2_specular"), 1, ptr(l2_specular));
      
      /* Draw the first sphere */
      glUniformMatrix4fv(program.uniform("model"), 1, 0, ptr(model1));
      mb1.draw();
   
      /* Draw the second sphere */
      glUniformMatrix4fv(program.uniform("model"), 1, 0, ptr(model2));
      mb2.draw();
      
    } program.unbind();
    
  }
  
  
  void onFrame(){
    
    if (camera.isTransformed) {
      camera.transform();
    }
    
    
    glViewport(0, 0, width, height); {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      draw(camera.projection, camera.view);
    }
    
  }
  
  
  virtual void keyDown(char key) {
    
    switch(key) {
      case kVK_Space :
        camera.resetVectors();
        break;
        
      case kVK_ANSI_A :
        camera.rotateY(2);
        break;
        
      case kVK_ANSI_D :
        camera.rotateY(-2);
        break;
        
      case kVK_ANSI_W :
        camera.rotateX(2);
        break;
        
      case kVK_ANSI_X :
        camera.rotateX(-2);
        break;
        
      case kVK_ANSI_E :
        camera.rotateZ(2);
        break;
        
      case kVK_ANSI_C :
        camera.rotateZ(-2);
        break;
        
      case kVK_ANSI_T :
        camera.translateZ(-0.5);
        break;
        
      case kVK_ANSI_B : 
        camera.translateZ(0.5);
        break;
        
      case kVK_ANSI_Y : 
        camera.translateX(0.5);
        break;
        
      case kVK_ANSI_N : 
        camera.translateX(-0.5);
        break;
        
      case kVK_ANSI_U : 
        camera.translateY(0.5);
        break;
        
      case kVK_ANSI_M : 
        camera.translateY(-0.5);
        break;
    }
  }
};

int main(){ 
  return Lighting().start("aluminum::Lighting", 100, 100, 400, 300);
}
