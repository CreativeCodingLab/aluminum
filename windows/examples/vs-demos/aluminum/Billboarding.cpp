
#include "Aluminum/FreeGlutGLView.hpp"
#include "Aluminum/RendererWin32.hpp"
#include "Aluminum/MeshBuffer.hpp"
#include "Aluminum/MeshUtils.hpp"
#include "Aluminum/Program.hpp"
#include "Aluminum/Texture.hpp"
#include "Aluminum/Camera.hpp"
#include "Aluminum/Utils.hpp"
#include "Aluminum/ResourceHandler.hpp"


/* Demonstrates the use of a geometry shader for a billboarding demo */

using namespace aluminum;

class Billboarding : public RendererWin32 {

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
    //bindDefaultVAO();

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

    loadProgram(program, "week6b_Billboarding/billboard");
    rh.loadTexture(texture, "week6b_Billboarding/eye.png");

    camera = Camera(glm::radians(60.0), (float)width/(float)height, 0.01, 100.0).translate(vec3(0.0,0.0,-2.0));


    Utils::randomSeed();
    MeshData points;

    for (int i = 0; i < 1; i++) {
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

  void mouseMoved(int x, int y){
	  camera.rotateY(glm::radians(-2.));
  }

  /*void keyboard(unsigned char key, int x, int y) {

    switch(key) {
      case ' ' :
        camera.resetVectors();
        break;

      case 'a' :
        camera.rotateY(glm::radians(2.));
        break;

      case 'd' :
        camera.rotateY(glm::radians(-2.));
        break;

      case 'w' :
        camera.rotateX(glm::radians(2.));
        break;

      case 'x' :
        camera.rotateX(glm::radians(-2.));
        break;

      case 'e' :
        camera.rotateZ(glm::radians(2.));
        break;

      case 'c' :
        camera.rotateZ(glm::radians(-2.));
        break;

      case 't' :
        camera.translateZ(-0.5);
        break;

      case 'b' :
        camera.translateZ(0.5);
        break;

      case 'y' :
        camera.translateX(0.5);
        break;

      case 'n' :
        camera.translateX(-0.5);
        break;

      case 'u' :
        camera.translateY(0.5);
        break;

      case 'm' :
        camera.translateY(-0.5);
        break;
    }
  }*/

};
