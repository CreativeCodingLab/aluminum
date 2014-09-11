#include "Aluminum/Includes.hpp"
#include "Aluminum/FreeGlutGLView.hpp"
#include "Aluminum/RendererWin32.hpp"
#include "Aluminum/Program.hpp"
#include "Aluminum/MeshBuffer.hpp"
#include "Aluminum/MeshData.hpp"
#include "Aluminum/Shapes.hpp"
#include "Aluminum/Camera.hpp"
#include "Aluminum/Utils.hpp"
#include "Aluminum/MeshUtils.hpp"
#include "Aluminum/ResourceHandler.hpp"
#include "Aluminum/FBO.hpp"
#include "Aluminum/Behavior.hpp"

using namespace aluminum;

class FBOExample : public RendererWin32 {
public:

  ResourceHandler rh;

  Camera camera;
  Program textureProgram, phongProgram;
  GLint posLoc = 0, normalLoc = 1, texCoordLoc = 2; //normalLoc = 1;
  mat4 model, view, proj;
  MeshData mesh1, mesh2;
  MeshBuffer mb1, mb2;
  FBO fbo;
  MeshBuffer cubeMeshBuffer1, cubeMeshBuffer2;
  Texture texture;
  Behavior rotateBehavior;

  vec3 l1_diffuse = vec3(0.0,1.0,0.0);
  vec3 l1_specular = vec3(1.0,1.0,1.0);

  vec3 l2_diffuse = vec3(0.0,0.0,1.0);
  vec3 l2_specular = vec3(1.0,1.0,1.0);


  void loadProgram(Program &p, const std::string& name) {

    p.create();

    p.attach(p.loadText(name + ".vsh"), GL_VERTEX_SHADER);
    glBindAttribLocation(p.id(), posLoc, "vertexPosition");
    glBindAttribLocation(p.id(), normalLoc, "vertexNormal");
    glBindAttribLocation(p.id(), texCoordLoc, "vertexTexCoord");

    p.attach(p.loadText(name + ".fsh"), GL_FRAGMENT_SHADER);
    //glBindFragDataLocation(id(), 0, "frag"); //agf

    p.link();
  }

/*
  void loadProgram(Program &p, const std::string& name) {

    p.create();

    p.attach(p.loadText(name + ".vsh"), GL_VERTEX_SHADER);
    glBindAttribLocation(p.id(), posLoc, "vertexPosition");
    glBindAttribLocation(p.id(), normalLoc, "vertexNormal");

    p.attach(p.loadText(name + ".fsh"), GL_FRAGMENT_SHADER);

    p.link();
  }
 */

  void onCreate() {

    rh.loadTexture(texture, "resources/hubble.jpg");
    texture.minFilter(GL_NEAREST);
    texture.maxFilter(GL_NEAREST);



 //   rh.loadProgram(program, "phong", posLoc, -1, texCoordLoc, -1);
    rh.loadProgram(textureProgram, "resources/texture", posLoc, -1, texCoordLoc, -1);
    rh.loadProgram(phongProgram, "resources/phong", posLoc, normalLoc, texCoordLoc, -1);


    //camera = Camera(60.0, (float)width/(float)height, 0.01, 100.0).translateZ(-10.0);

    MeshData md1;
    addCube(md1, true, 0.95);

    MeshData md2;
    addCube(md2, true, 0.65);

    cubeMeshBuffer1.init(md1, posLoc,normalLoc, texCoordLoc, -1);
    cubeMeshBuffer2.init(md2, posLoc, normalLoc, texCoordLoc, -1);


    fbo.create(256, 256);

    rotateBehavior = Behavior(now()).delay(1000).length(5000).range(vec3(180.0, 90.0, 360.0)).reversing(true).repeats(-1).sine();


    proj = glm::perspective(45.0, 1.0, 0.1, 100.0);
    view = glm::lookAt(vec3(0.0,0.0,-5), vec3(0,0,0), vec3(0,1,0) );
    model = glm::mat4();

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);
}


  void draw(mat4& model, MeshBuffer& mb, Texture& t, Program& p) {

    p.bind(); {
      glUniformMatrix4fv(p.uniform("model"), 1, 0, ptr(model));
      glUniformMatrix4fv(p.uniform("view"), 1, 0, ptr(view));
      glUniformMatrix4fv(p.uniform("proj"), 1, 0, ptr(proj));

      t.bind(GL_TEXTURE0); {

        glUniform1i(p.uniform("tex0"), 0);
        mb.draw();

      } t.unbind(GL_TEXTURE0);

    } p.unbind();
  }


  void onFrame(){

    model = glm::mat4();

    vec3 totals = rotateBehavior.tick(now()).totals();
    model = glm::rotate(model, totals.x, vec3(1.0f,0.0f,0.0f));
    model = glm::rotate(model, totals.y, vec3(0.0f,1.0f,0.0f));
    model = glm::rotate(model, totals.z, vec3(0.0f,0.0f,1.0f));


    //draw cube 1 into an offscreen texture
    fbo.bind(); {
      glViewport(0, 0, fbo.width, fbo.height);
      glClearColor(0.1,0.1,0.1,1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      draw(model, cubeMeshBuffer1, texture, textureProgram);

    } fbo.unbind();

    //draw cube 2 with the offscreen texture using phong shading
    glViewport(0, 0, width, height);
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw(model, cubeMeshBuffer2, fbo.texture, phongProgram);
  }

};

int main(){
  FBOExample().start("aluminum::FBOExample", 400, 300);
  return 0;
}
