/*
 Adapted from the Planet Wrapper demo by Humus at http://www.humus.name/
 */

#include "Aluminum/Includes.hpp"

#include "Aluminum/FreeGlutGLView.hpp"
#include "Aluminum/RendererLinux.hpp"
#include "Aluminum/MeshBuffer.hpp"
#include "Aluminum/MeshData.hpp"
#include "Aluminum/MeshUtils.hpp"
#include "Aluminum/Program.hpp"
#include "Aluminum/Texture.hpp"
#include "Aluminum/Camera.hpp"

#include "Aluminum/ResourceHandler.hpp"


using namespace aluminum;

class MiniPlanet : public RendererLinux {
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

    rh.loadProgram(planetProgram, "resources/planet", 0, -1, -1, -1);

    mb.init(MeshUtils::makeClipRectangle(), 0, -1, -1, -1);

    rh.loadCubeMapTexture(cubemap[0], 2048, 2048,
                          "resources/f_negz.jpg",
                          "resources/f_posz.jpg",
                          "resources/f_posy.jpg",
                          "resources/f_negy.jpg",
                          "resources/f_negx.jpg",
                          "resources/f_posx.jpg");

    rh.loadCubeMapTexture(cubemap[1], 2048, 2048,
                          "resources/b_negz.jpg",
                          "resources/b_posz.jpg",
                          "resources/b_posy.jpg",
                          "resources/b_negy.jpg",
                          "resources/b_negx.jpg",
                          "resources/b_posx.jpg");
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


  void keyboard(unsigned char key, int x, int y) {

    switch(key) {
      case ' ' : //reset
        zoom = 0.1f;
        scale = 5.0f;
        power = 1.2f;
        angle = 0.0;
        ++which %= 2;
        break;

      case 'a' :
        zoom += 0.01f;
        break;

      case 'z' :
        zoom -= 0.01f;
        break;

      case 's' :
        scale += 0.3f;
        break;

      case 'x' :
        scale -= 0.3f;
        break;

      case 'd' :
        power += 0.01f;
        break;

      case 'c' :
        power -= 0.01f;
        break;

      case 'f' :
        angle += glm::radians(1.0);
        break;

      case 'v' :
        angle -= glm::radians(1.0);
        break;
    }
  }

};
int main() {
	MiniPlanet().start("MiniPlanet");
	return 0;
}
