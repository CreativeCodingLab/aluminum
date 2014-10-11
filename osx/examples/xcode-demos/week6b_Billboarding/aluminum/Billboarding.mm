
#import <Aluminum/RendererOSX.h>
#import <Aluminum/MeshBuffer.hpp>
#import <Aluminum/MeshUtils.hpp>
#import <Aluminum/Program.hpp>
#import <Aluminum/Texture.hpp>
#import <Aluminum/Camera.hpp>
#import <Aluminum/Utils.hpp>
#import <Aluminum/ResourceHandler.h>


/* Demonstrates the use of a geometry shader for a billboarding demo */

using namespace aluminum;

class Billboarding : public RendererOSX {
  
public:
  
  mat4 model, view, proj;
  
  Program program;
  
  GLint posLoc=0;
  GLint texCoordLoc=1;
  
  Texture texture;
  MeshBuffer mb1;
  ResourceHandler rh;
  Camera camera;
  
  
  void loadProgram(Program &p, const std::string& name) {
    
    //can't link shaders unless a vao is bound...
    bindDefaultVAO();
    
    p.create();
    
    string sv = rh.pathToResource(name, "vsh");
    
    p.attach(rh.contentsOfFile(sv), GL_VERTEX_SHADER);
    
    glBindAttribLocation(p.id(), posLoc, "vertexPosition");
    glBindAttribLocation(p.id(), texCoordLoc, "vertexTexCoord");
    
    
    string sg = rh.pathToResource(name, "gsh");
    cout << "path of vertex shader is: " << sg << endl;
    
    p.attach(rh.contentsOfFile(sg), GL_GEOMETRY_SHADER);
    
    string sp = rh.pathToResource(name, "fsh");
    
    p.attach(rh.contentsOfFile(sp), GL_FRAGMENT_SHADER);
    
    p.link();
  }

  
  
  void onCreate() {
    
    loadProgram(program, "billboard");
    rh.loadTexture(texture, "eye.png");
 
    camera = Camera(glm::radians(60.0), (float)width/(float)height, 0.01, 100.0).translate(vec3(0.0,0.0,-2.0));

    
    Utils::randomSeed();
    MeshData points;
   
    for (int i = 0; i < 100; i++) {
      MeshUtils::addPoint(points, Utils::randomVec3(-1.0,1.0), Utils::randomVec3(0.0,1.0));

    }
    
    mb1.init(points, posLoc, -1, texCoordLoc, -1);
    
    proj = glm::perspective(glm::radians(45.0), 1.0, 0.1, 100.0);
    view = glm::lookAt(vec3(0,0,-2), vec3(0,0,0), vec3(0,1,0) );
    model = glm::mat4();
    
    glDisable(GL_DEPTH_TEST);
    
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glViewport(0, 0, width, height);
    glClearColor(0.3,0.3,0.3,1.0);
  }
  
  void onFrame(){
    
    
    if (camera.isTransformed) { //i.e. if you've pressed any of the keys to move or rotate the camera around
      camera.transform();
    }
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    program.bind(); {
      
      glUniformMatrix4fv(program.uniform("model"), 1, 0, ptr(model));
      glUniformMatrix4fv(program.uniform("view"), 1, 0, ptr(camera.view));
      glUniformMatrix4fv(program.uniform("proj"), 1, 0, ptr(camera.projection));
   
      texture.bind(GL_TEXTURE0); {
        mb1.drawPoints();
      } texture.unbind(GL_TEXTURE0);
   
    } program.unbind();
  }
  
  void onReshape() {
    glViewport(0, 0, width, height);
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


int main() {
  return Billboarding().start("aluminum::Billboarding", 100, 100, 400, 300);
}
