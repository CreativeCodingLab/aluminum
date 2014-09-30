/*
 Adapted from the Planet Wrapper demo by Humus at http://www.humus.name/
 */

#include "Aluminum/Includes.hpp"

#include "Aluminum/RendererOSX.h"
#include "Aluminum/MeshBuffer.hpp"
#include "Aluminum/MeshData.hpp"
#include "Aluminum/MeshUtils.hpp"
#include "Aluminum/Program.hpp"
#include "Aluminum/Texture.hpp"
#include "Aluminum/Camera.hpp"

#include "Aluminum/ResourceHandler.h"


using namespace aluminum;

class MiniPlanet : public RendererOSX {
public:
  
  Program planetProgram;
  GLuint tex_cube;
  ResourceHandler rh;
  Camera camera;
  MeshBuffer mb;
  mat4 cubeModel;
  
  int which = 0;
  Texture cubemap[2];
  
  //default vals for shader, use AZSXDCFV keys to change vals
  float zoom = 0.1f;
  float scale = 5.0f;
  float power = 1.2f;
  float angle = 0.0;
  
  
  virtual void onCreate() {
    
    rh.loadProgram(planetProgram, "planet", 0, -1, -1, -1);
    
    mb.init(MeshUtils::makeClipRectangle(), 0, -1, -1, -1);
    
    rh.loadCubeMapTexture(cubemap[0], 2048, 2048,
                          "f_negz.jpg",
                          "f_posz.jpg",
                          "f_posy.jpg",
                          "f_negy.jpg",
                          "f_negx.jpg",
                          "f_posx.jpg");
   
    rh.loadCubeMapTexture(cubemap[1], 2048, 2048,
                          "b_negz.jpg",
                          "b_posz.jpg",
                          "b_posy.jpg",
                          "b_negy.jpg",
                          "b_negx.jpg",
                          "b_posx.jpg");
  }
  
  void onFrame() {
    
    float ratio = float(height) / float(width);
    float c = cosf(angle);
    float s = sinf(angle);
    
    vec4 rot = scale * vec4(c, -s * ratio, s, c * ratio);
    
    planetProgram.bind(); {
      glUniform4fv(planetProgram.uniform("rot"), 1, ptr(rot));
      glUniform1f(planetProgram.uniform("zoom"), zoom);
      glUniform1f(planetProgram.uniform("power"), power);
      glUniform1i(planetProgram.uniform("cube_texture"), 0);
      
      cubemap[which].bind(GL_TEXTURE0); {
        mb.draw();
      } cubemap[which].unbind(GL_TEXTURE0);
      
    } planetProgram.unbind();
  }
  
  
  virtual void keyDown(char key) {
    
    switch(key) {
      case kVK_Space : //reset
        zoom = 0.1f;
        scale = 5.0f;
        power = 1.2f;
        angle = 0.0;
        ++which %= 2;
        break;
        
      case kVK_ANSI_A :
        zoom += 0.01f;
        break;
        
      case kVK_ANSI_Z :
        zoom -= 0.01f;
        break;
        
      case kVK_ANSI_S :
        scale += 0.3f;
        break;
        
      case kVK_ANSI_X :
        scale -= 0.3f;
        break;
        
      case kVK_ANSI_D :
        power += 0.01f;
        break;
        
      case kVK_ANSI_C :
        power -= 0.01f;
        break;
        
      case kVK_ANSI_F :
        angle += glm::radians(1.0);
        break;
        
      case kVK_ANSI_V :
        angle -= glm::radians(1.0);
        break;
    }
  }
  
};

int main(){
  return MiniPlanet().start("aluminum::MiniPlanet", 100, 100, 400, 300);
}
