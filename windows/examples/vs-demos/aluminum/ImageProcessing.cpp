
#include "Aluminum/FreeGlutGLView.hpp"
#include "Aluminum/RendererWin32.hpp"
#include "Aluminum/MeshBuffer.hpp"
#include "Aluminum/MeshUtils.hpp"
#include "Aluminum/Program.hpp"
#include "Aluminum/Texture.hpp"
#include "Aluminum/ResourceHandler.hpp"

/*
  Demonstrates the use of simple convolution filter in the fragment shader for a) edge detection and b) embossing an image. Lots of somewhat unintuitive controls via keyboard and mouse, so look at the keyDown and moveMoved methods.
 */

using namespace aluminum;

class ImageProcessing : public RendererWin32 {

public:

  mat4 model, view, proj;

  Program program;

  GLint posLoc=0;
  GLint texCoordLoc=1;

  Texture textures[4];
  MeshBuffer mb1;
  ResourceHandler rh;

  float x_mult = 1.0;
  float y_mult = 1.0;
  float gamma = 0.8;
  bool invert = false;
  bool off = false;
  bool emboss = false;
  int which = 0;


  void onCreate() {

    rh.loadProgram(program, "week6a_ImageProcessing/texture", posLoc, -1, texCoordLoc, -1);
    rh.loadTexture(textures[0], "week6a_ImageProcessing/hubble.jpg");
    rh.loadTexture(textures[1], "week6a_ImageProcessing/angus.jpg");
    rh.loadTexture(textures[2], "week6a_ImageProcessing/family.jpg");
    rh.loadTexture(textures[3], "week6a_ImageProcessing/stainedglass.jpg");


    mb1.init(MeshUtils::makeRectangle(2.0, 2.0), posLoc, -1, texCoordLoc, -1);

    proj = glm::perspective(glm::radians(60.0), 1.0, 0.1, 100.0);
    view = glm::lookAt(vec3(0,0,-2), vec3(0,0,0), vec3(0,1,0) );
    model = glm::mat4();

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);
    glClearColor(0.3,0.3,0.3,1.0);
  }

  void onFrame(){


    textures[which].wrapMode(GL_REPEAT);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program.bind(); {

      glUniformMatrix4fv(program.uniform("model"), 1, 0, ptr(model));
      glUniformMatrix4fv(program.uniform("view"), 1, 0, ptr(view));
      glUniformMatrix4fv(program.uniform("proj"), 1, 0, ptr(proj));

      float du = 1.0/textures[which].width;
      float dv = 1.0/textures[which].width;

      glUniform1i(program.uniform("invert"), invert);
      glUniform1i(program.uniform("off"), off);
      glUniform1i(program.uniform("emboss"), emboss);
      glUniform1f(program.uniform("gamma"), gamma);


      glUniform1f(program.uniform("du"), du * x_mult);
      glUniform1f(program.uniform("dv"), dv * y_mult);
      glUniform1i(program.uniform("tex0"), 0);

      textures[which].bind(GL_TEXTURE0); {
        mb1.draw();
      } textures[which].unbind(GL_TEXTURE0);

    } program.unbind();
  }

  void onReshape() {
    glViewport(0, 0, width, height);
  }

  void mouseMoved(int px, int py) {
    x_mult = y_mult = py / 3.0;
    gamma = ((float)px/(float)width) * 5.0;
  }

  void keyboard(unsigned char key, int x, int y) {

    switch(key) {
      case 'r' :
        invert = true;
        gamma = 0.8;
        x_mult = y_mult = 1.0;
        break;

      case 'a' :
        invert = !invert;
        break;

      case 'e' :
        emboss = !emboss;
        break;

      case 'z' :
        off = !off;
        break;


      case ' ' :
        ++which %= 4;
        break;


    }

  }

};
