
#include "Aluminum/Includes.hpp"

#include "Aluminum/FreeGlutGLView.hpp"
#include "Aluminum/RendererWin32.hpp"
#include "Aluminum/MeshBuffer.hpp"
#include "Aluminum/MeshData.hpp"
#include "Aluminum/MeshUtils.hpp"
#include "Aluminum/Program.hpp"
#include "Aluminum/Texture.hpp"
#include "Aluminum/Behavior.hpp"
#include "Aluminum/Shapes.hpp"
#include "Aluminum/Camera.hpp"
#include "Aluminum/ResourceHandler.hpp"

#include "objload.h"

#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

using namespace aluminum;

class SkyBox : public RendererWin32 {
public:


  Program skyboxProgram, environmentMappingProgram;
  GLint posLoc=0, normalLoc=1;
  Camera camera;
  MeshBuffer cubeMeshBuffer, dragonMeshBuffer, skyboxMeshBuffer;
  mat4 cubeModel, dragonModel;
  Behavior rotateBehavior;
  Texture cmt;
  ResourceHandler rh;

  virtual void onCreate() {

    rh.loadProgram(skyboxProgram, "week5b_CubeMapping/skybox", posLoc, -1, -1, -1);
    rh.loadProgram(environmentMappingProgram, "week5b_CubeMapping/envMap", posLoc, normalLoc, -1, -1);

    MeshData cube;
    addCube(cube, 8.0);
    cubeMeshBuffer.init(cube, posLoc, normalLoc, -1, -1);
    cubeModel = mat4();
    cubeModel = glm::translate(cubeModel, vec3(0.0,0.0,40));


    obj::Model m = obj::loadModelFromFile(rh.pathToResource("week5b_CubeMapping/dragon.obj"));

    MeshData dragonMesh;

    for(std::map<std::string, std::vector<unsigned short> >::const_iterator g = m.faces.begin(); g != m.faces.end(); ++g) {

      for (int i = 0 ; i < g->second.size() ; i++) {

        dragonMesh.index(g->second[i]);
      }
    }


    for (int i = 0; i < m.vertex.size(); i+=3) {
      vec3 pos = vec3(m.vertex[i], m.vertex[i+1], m.vertex[i+2]);
      pos *= 30;
      dragonMesh.vertex(pos);
    }

    for (int i = 0; i < m.normal.size(); i+=3) {
      dragonMesh.normal(m.normal[i], m.normal[i+1], m.normal[i+2]);
    }

    dragonMeshBuffer.init(dragonMesh, posLoc, normalLoc, -1, -1);
    dragonModel = glm::mat4();
    dragonModel = glm::translate(dragonModel, vec3(0.0, -1.2, 30.0));

    MeshData skybox;
    addCube(skybox, 100.0);
    skyboxMeshBuffer.init(skybox, posLoc, -1, -1, -1);


    //create the cube map texture
    rh.loadCubeMapTexture(cmt, 2048, 2048,
                          "week5b_CubeMapping/negz.jpg",
                          "week5b_CubeMapping/posz.jpg",
                          "week5b_CubeMapping/posy.jpg",
                          "week5b_CubeMapping/negy.jpg",
                          "week5b_CubeMapping/negx.jpg",
                          "week5b_CubeMapping/posx.jpg");


    rotateBehavior = Behavior(now()).delay(1000).length(10000).range(vec3(glm::radians(360.0))).looping(true).repeats(-1);

    camera = Camera(glm::radians(60.0), (float)width/(float)height, 0.01, 1000.0);

    glEnable(GL_DEPTH_TEST);

  }

  void onFrame() {

    if (camera.isTransformed) {
      camera.transform();
    }

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cmt.bind(GL_TEXTURE0); {

      skyboxProgram.bind(); {
        glUniformMatrix4fv(skyboxProgram.uniform("view"), 1, 0, ptr(camera.view));
        glUniformMatrix4fv(skyboxProgram.uniform("proj"), 1, 0, ptr(camera.projection));
        glUniform1i(skyboxProgram.uniform("cube_texture"), 0);

        skyboxMeshBuffer.draw();
      } skyboxProgram.unbind();

      vec3 totals = rotateBehavior.tick(now()).totals();
      cubeModel = glm::mat4();
      cubeModel = glm::translate(cubeModel, vec3(12.0,0.0,40.0));
      cubeModel = glm::rotate(cubeModel, totals.x, vec3(1.0f,0.0f,0.0f));
      cubeModel = glm::rotate(cubeModel, totals.y, vec3(0.0f,1.0f,0.0f));

      dragonModel = glm::mat4();
      dragonModel = glm::translate(dragonModel, vec3(-12.0,-15.0,40.0));
      dragonModel = glm::rotate(dragonModel, totals.y, vec3(0.0f,1.0f,0.0f));


      environmentMappingProgram.bind(); {
        glUniformMatrix4fv(environmentMappingProgram.uniform("view"), 1, 0, ptr(camera.view));
        glUniformMatrix4fv(environmentMappingProgram.uniform("proj"), 1, 0, ptr(camera.projection));
        glUniform1i(environmentMappingProgram.uniform("cube_texture"), 0);

        //set dragon specific variables and draw dragon
        glUniformMatrix4fv(environmentMappingProgram.uniform("model"), 1, 0, ptr(dragonModel));
        glUniform4f(environmentMappingProgram.uniform("baseColor"), 0.0, 0.0, 0.1, 1.0);
        dragonMeshBuffer.draw();

        //set cube specific variables and draw cube
        glUniformMatrix4fv(environmentMappingProgram.uniform("model"), 1, 0, ptr(cubeModel));
        glUniform4f(environmentMappingProgram.uniform("baseColor"), 0.1, 0.0, 0.0, 1.0);
        cubeMeshBuffer.draw();
      } environmentMappingProgram.unbind();

    } cmt.unbind(GL_TEXTURE0);

  }

  void keyDown(char key, bool shift, bool control, bool command, bool option, bool function) {

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
  }

};
