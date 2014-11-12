
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
//#include "objload.h"

using namespace aluminum;

class ModelFromObjFile : public RendererOSX {
public:
  
  vec3 diffuse = vec3(1.0,0.4,0.0);
  vec3 specular = vec3(1.0,1.0,1.0);
  vec3 ambient = vec3(0.0,0.0,0.3);
  mat4 model, view, proj;
  
  
  float camerax = 0.0;
  float cameray = 0.0;
  float cameraz = 0.0;
  float modelx = 0.0;
  float modely = 0.0;
  float modelz = 0.0;
  
  Program program;
  GLint posLoc=0;
  GLint normalLoc=1;
  
  int which = 0;
  MeshBuffer mb[3];
  Behavior rotateBehavior;
  ResourceHandler rh;
  
//  void loadObjIntoMesh(MeshData &modelMesh, const std::string& name, float scalar) {
//    
//    obj::Model m = obj::loadModelFromFile(rh.pathToResource(name));
//    
//    for(std::map<std::string, std::vector<unsigned short> >::const_iterator g = m.faces.begin(); g != m.faces.end(); ++g){
//      std::cout << g->first << "\n" ;
//      
//      cout << "num indicies = " << g->second.size() << "\n";
//      for (int i = 0 ; i < g->second.size() ; i++) {
//        
//        modelMesh.index(g->second[i]);
//        //cout << g->second[i] << " ";
//      }
//      //cout << "\n";
//    }
//    
//    
//    cout << "vertex size = " << m.vertex.size() / 3 << "\n";
//    cout << "normal size = " << m.normal.size() / 3 << "\n";
//    
//    for (int i = 0; i < m.vertex.size(); i+=3) {
//      vec3 pos = vec3(m.vertex[i], m.vertex[i+1], m.vertex[i+2]);
//      pos *= scalar;
//      modelMesh.vertex(pos);
//    }
//    
//    for (int i = 0; i < m.texCoord.size(); i+=2) {
//      //		modelMesh.texCoord(m.texCoord[i], m.texCoord[i+1]);
//    }
//    
//    for (int i = 0; i < m.normal.size(); i+=3) {
//      modelMesh.normal(m.normal[i], m.normal[i+1], m.normal[i+2]);
//    }
//    
//    
//    //std::cout << m << std::endl;
//    //std::cout << m.vertex.size()/3 << " vertices, " << m.texCoord.size()/2 <<
//    //             " texcoords, " << m.normal.size()/3 << " normals, " <<
//    //             m.faces.size()/3 << " faces" << std::endl;
//    
//  }
  
  
  virtual void onCreate() {
    
    rh.loadProgram(program, "phong", posLoc, normalLoc, -1, -1);
    
    MeshData modelMesh[3];
    
    rh.loadObjIntoMesh(modelMesh[0], "dragon.obj", 2.5);
    rh.loadObjIntoMesh(modelMesh[1], "bunny.obj", 1.0);
    rh.loadObjIntoMesh(modelMesh[2], "willis.obj", 1.0);
    
    mb[0].init(modelMesh[0], posLoc, normalLoc, -1, -1);
    mb[1].init(modelMesh[1], posLoc, normalLoc, -1, -1);
    mb[2].init(modelMesh[2], posLoc, normalLoc, -1, -1);
    
    modely = -1.2;
    cameraz = 4;
    
    proj = glm::perspective(45.0f, (float)width/(float)height, 0.1f, 1000.0f);
    view = glm::lookAt(vec3(camerax, cameray, -cameraz), vec3(0,0,0), vec3(0,1,0) );
    model = glm::mat4();
    model = glm::translate(model, vec3(modelx, modely, modelz));
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.3,0.3,0.3,1.0);
    
    rotateBehavior = Behavior(now()).delay(1000).length(10000).range(M_PI * 2.0).looping(true).repeats(-1);
    
  }
  
  void updateModel() {
    
    float total = rotateBehavior.tick(now()).total();
    model = glm::mat4();
    model = glm::translate(model, vec3(modelx,modely,modelz));
    model = glm::rotate(model, total, vec3(0.0f,1.0f,0.0f));
  }
  
  void onFrame() {
    
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    updateModel();
    
    program.bind(); {
      glUniformMatrix4fv(program.uniform("model"), 1, 0, ptr(model));
      glUniformMatrix4fv(program.uniform("view"), 1, 0, ptr(view));
      glUniformMatrix4fv(program.uniform("proj"), 1, 0, ptr(proj));
      
      glUniform3f(program.uniform("lightPos"), 0.0, 0.0, 10.0);
      glUniform3fv(program.uniform("ambient"), 1, ptr(ambient));
      glUniform3fv(program.uniform("diffuse"), 1, ptr(diffuse));
      glUniform3fv(program.uniform("specular"), 1, ptr(specular));
      
      mb[which].draw();
      
    } program.unbind();
  }
  
  
  
  
  virtual void keyDown(char key) {
    
    switch(key) {
      case kVK_Space :
        
        ++which %= 3;
        
        //need to reposition the models if they aren't centered around (0,0,0).
        switch(which) {
          case 0:
            modely = -1.2;
            cameraz = 4;
            break;
            
          case 1:
            modely = -1.5;
            cameraz = 5;
            break;
            
          case 2:
            modely = -8;
            cameraz = 20;
            break;
        }
        
        proj = glm::perspective(45.0f, (float)width/(float)height, 0.1f, 1000.0f);
        view = glm::lookAt(vec3(camerax, cameray, -cameraz), vec3(0,0,0), vec3(0,1,0) );
        model = glm::mat4();
        model = glm::translate(model, vec3(modelx, modely, modelz));
        
        
        break;
    }
    
  }
  
};

int main(){
  return ModelFromObjFile().start("aluminum::ModelFromObjFile", 100, 100, 400, 300);
}
