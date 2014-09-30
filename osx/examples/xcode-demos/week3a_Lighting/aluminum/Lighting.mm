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
  mat4 model1, model2, model3, lightModel1, lightModel2;
  MeshData mesh1, mesh2, mesh3, lightMesh;
  MeshBuffer mb1, mb2, mb3, lmb1, lmb2;
  
  
  vec3 ambient = vec3(0.1,0.1,0.1);
  
  vec3 l1_diffuse = vec3(0.0,1.0,0.0);
  vec3 l1_specular = vec3(1.0,1.0,1.0);
  
  vec3 l2_diffuse = vec3(0.0,0.0,1.0);
  vec3 l2_specular = vec3(1.0,1.0,1.0);
  
  
  
  
  void onCreate() {
    
    rh.loadProgram(program, "phong", posLoc, normalLoc, -1, -1);
    
    camera = Camera(glm::radians(60.0), (float)width/(float)height, 0.01, 100.0).translate(vec3(-4.0,0.0,-20.0));
    

    addSphere(mesh1, 2.0, 100, 100);
    addSphere(mesh2, 1.0, 100, 100);
    addSphere(mesh3, 1.5, 100, 100);
    addSphere(lightMesh, 0.3, 10, 10);
    
    mb1.init(mesh1, posLoc, normalLoc, -1, -1);
    mb2.init(mesh2, posLoc, normalLoc, -1, -1);
    mb3.init(mesh3, posLoc, normalLoc, -1, -1);
    lmb1.init(lightMesh, posLoc, normalLoc, -1, -1);
    lmb2.init(lightMesh, posLoc, normalLoc, -1, -1);
    
    model1 = glm::translate(mat4(), vec3(0,0,5));
    model2 = glm::translate(mat4(), vec3(5,0,5));
    model3 = glm::translate(mat4(), vec3(0,0,-15));
    
    
    camera.printCameraInfo();
    
    glEnable(GL_DEPTH_TEST);
  }
  
  int dir1 = 1;
  float pos1 = 0.0f;
 
  int dir2 = 1;
  float pos2 = 0.0f;
  
  void draw(mat4 proj, mat4 view) {
    
    /* update light positions */
    
    pos1 += 0.2f * dir1;
    if (pos1 > 15.0 || pos1 < -15.0) {
      dir1 *= -1;
    }
    
    vec4 l1_position = vec4(0.0, pos1, 4.0, 1.0);
   
    pos2 += 0.3f * dir2;
    if (pos2 > 15.0 || pos2 < -15.0) {
      dir2 *= -1;
    }
    vec4 l2_position = vec4(pos2, 0.0, 5.0, 1.0);
   
    l1_position = vec4(pos1,0.0,1.0,1.0);
    l2_position = vec4(0.0,pos2,-1.0,1.0);
    
    lightModel1 = glm::translate(mat4(), vec3(l1_position));
    lightModel2 = glm::translate(mat4(), vec3(l2_position));
    
    /* bind our Phong lighting shader */
    
    program.bind(); {
      glUniformMatrix4fv(program.uniform("view"), 1, 0, ptr(view));
      glUniformMatrix4fv(program.uniform("proj"), 1, 0, ptr(proj));
    
      glUniform3fv(program.uniform("ambient"), 1, ptr(ambient));
      
      glUniform4fv(program.uniform("l1_position"), 1, ptr(l1_position));
      glUniform3fv(program.uniform("l1_diffuse"), 1, ptr(l1_diffuse));
      glUniform3fv(program.uniform("l1_specular"), 1, ptr(l1_specular));
  
      
      glUniform4fv(program.uniform("l2_position"), 1, ptr(l2_position));
      glUniform3fv(program.uniform("l2_diffuse"), 1, ptr(l2_diffuse));
      glUniform3fv(program.uniform("l2_specular"), 1, ptr(l2_specular));
      
      /* Draw the first sphere */
      glUniformMatrix4fv(program.uniform("model"), 1, 0, ptr(model1));
      mb1.draw();
   
      /* Draw the second sphere */
      glUniformMatrix4fv(program.uniform("model"), 1, 0, ptr(model2));
      mb2.draw();
    
      /* Draw the third sphere */
      glUniformMatrix4fv(program.uniform("model"), 1, 0, ptr(model3));
      mb3.draw();
   
      
      /* turn off the diffuse and speculars when drawing the positions of the lights */
      glUniform3fv(program.uniform("l1_diffuse"), 1, ptr(vec3(0.0)));
      glUniform3fv(program.uniform("l1_specular"), 1, ptr(vec3(0.0)));
      
      glUniform3fv(program.uniform("l2_diffuse"), 1, ptr(vec3(0.0)));
      glUniform3fv(program.uniform("l2_specular"), 1, ptr(vec3(0.0)));
      
      /* draw light 1 */
      glUniform3fv(program.uniform("ambient"), 1, ptr(l1_diffuse));
      glUniformMatrix4fv(program.uniform("model"), 1, 0, ptr(lightModel1));
      lmb1.draw();
      
      /* draw light 2 */
      glUniform3fv(program.uniform("ambient"), 1, ptr(l2_diffuse));
      glUniformMatrix4fv(program.uniform("model"), 1, 0, ptr(lightModel2));
      lmb2.draw();
      
      
      
    } program.unbind();
    
  }
  
  
  void onFrame(){
    
    if (camera.isTransformed) { //i.e. if you've pressed any of the keys to move or rotate the camera around
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
        camera.rotateY(glm::radians(2.));
        break;
        
      case kVK_ANSI_D :
        camera.rotateY(glm::radians(-2.));
        break;
        
      case kVK_ANSI_W :
        camera.rotateX(glm::radians(2.));
        break;
        
      case kVK_ANSI_X :
        camera.rotateX(glm::radians(-2.));
        break;
        
      case kVK_ANSI_E :
        camera.rotateZ(glm::radians(2.));
        break;
        
      case kVK_ANSI_C :
        camera.rotateZ(glm::radians(-2.));
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
