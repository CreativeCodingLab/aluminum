#include "Aluminum/Includes.hpp"

#include "Aluminum/Program.hpp"
#include "Aluminum/MeshBuffer.hpp"
#include "Aluminum/MeshData.hpp"
#include "Aluminum/Shapes.hpp"
#include "Aluminum/Camera.hpp"
#include "Aluminum/Utils.hpp"
#include "Aluminum/MeshUtils.hpp"
#include "Aluminum/Behavior.hpp"

#include "Aluminum/ResourceHandler.h"

using namespace aluminum;

class VertexDisplacement : public RendererOSX {
public:
  
  ResourceHandler rh;
  
  Camera camera;
  Program program;
  GLint posLoc = 0, normalLoc = 1;
  mat4 model1, lightModel1, lightModel2;
  MeshData mesh1;
  MeshBuffer mb1;
  
  
  vec3 ambient = vec3(0.05,0.05,0.05);
  
  vec3 l1_diffuse = vec3(0.0,1.0,0.0);
  vec3 l1_specular = vec3(1.0,1.0,1.0);
  
  vec3 l2_diffuse = vec3(0.0,0.0,1.0);
  vec3 l2_specular = vec3(1.0,1.0,1.0);
  
  Behavior rotateBehavior;
  
  void onCreate() {
    
    rh.loadProgram(program, "phong", posLoc, normalLoc, -1, -1);
    
    int res = 200;
    addSphere(mesh1, 2.0, res, res);
    mb1.init(mesh1, posLoc, normalLoc, -1, -1);
    
    
    camera = Camera(glm::radians(60.0), (float)width/(float)height, 0.01, 100.0).translate(vec3(0.0,0.0,-5.0)).transform();
    camera.printCameraInfo();
    
    rotateBehavior = Behavior(now()).delay(1000).length(10000).range(vec3(glm::radians(360.0))).looping(true).repeats(-1);
    
    glEnable(GL_DEPTH_TEST);
  }
  
  int dir1 = 1;
  float cr = 0.0f;
  
  int dir2 = 1;
  float cg = 0.0f;
  
  int dir3 = 1;
  float cb = 0.0f;
  
  
  float t1 = 0.0;
  int t1dir = 1;
  float t2 = 0.0;
  int t2dir = 1;
  
  void draw(mat4 proj, mat4 view) {
    
    //update times
    t1 += 0.05 * t1dir;
    if (abs(t1) > 17.0) { t1dir *= -1; }
    
    t2 += 0.07 * t2dir;
    if (abs(t2) > 23.0) { t2dir *= -1; }
    
    
    //rotate our model around the y-axis
    vec3 totals = rotateBehavior.tick(now()).totals();
    model1 = glm::mat4();
    model1 = glm::rotate(model1, totals.y, vec3(0.0f,1.0f,0.0f));
    
    
    //update colors
    cr += 0.005 * dir1;
    if (cr > 1.0 || cr < 0.0) { dir1 *= -1; }
    
    cg += 0.007 * dir2;
    if (cg > 1.0 || cg < 0.0) { dir2 *= -1; }
    
    cb += 0.00209 * dir3;
    if (cb > 1.0 || cb < 0.0) { dir3 *= -1; }
    
    l1_diffuse = vec3(cr, cg, cb);
    l2_diffuse = vec3(1.0 - cr, 1.0 - cg, 1.0 - cb);
    
    
    //position and rotate our lights around the center object
    vec4 l1_position = vec4(0.0,10.0,-20.0,1.0);
    vec4 l2_position = vec4(0.0,-10.0,-20.0,1.0);
    l1_position = glm::rotate(l1_position, -totals.y, vec3(0.0f,1.0f,0.0f));
    l2_position = glm::rotate(l2_position, totals.y, vec3(0.0f,1.0f,0.0f));
    
    
    //bind our Phong lighting shader and draw the object
    program.bind(); {
      glUniformMatrix4fv(program.uniform("view"), 1, 0, ptr(view));
      glUniformMatrix4fv(program.uniform("proj"), 1, 0, ptr(proj));
      
      glUniform1f(program.uniform("t1"), t1);
      glUniform1f(program.uniform("t2"), t2);
      
      glUniform3fv(program.uniform("ambient"), 1, ptr(ambient));
      
      glUniform4fv(program.uniform("l1_position"), 1, ptr(l1_position));
      glUniform3fv(program.uniform("l1_diffuse"), 1, ptr(l1_diffuse));
      glUniform3fv(program.uniform("l1_specular"), 1, ptr(l1_specular));
      
      glUniform4fv(program.uniform("l2_position"), 1, ptr(l2_position));
      glUniform3fv(program.uniform("l2_diffuse"), 1, ptr(l2_diffuse));
      glUniform3fv(program.uniform("l2_specular"), 1, ptr(l2_specular));
      
      glUniformMatrix4fv(program.uniform("model"), 1, 0, ptr(model1));
      mb1.draw();
      
    } program.unbind();
  }
  
  void onFrame(){
    glViewport(0, 0, width, height); {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      draw(camera.projection, camera.view);
    }
  }
  
};

int main(){
  return VertexDisplacement().start("aluminum::VertexDisplacement", 100, 100, 400, 300);
}
