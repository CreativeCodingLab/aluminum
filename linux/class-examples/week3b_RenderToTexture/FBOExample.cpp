#include "Aluminum/Includes.hpp"
#include "Aluminum/FreeGlutGLView.hpp"
#include "Aluminum/RendererLinux.hpp"
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

class FBOExample : public RendererLinux {
public:

	ResourceHandler rh;

	Program textureProgram, phongProgram, programColor;
	GLint posLoc = 0, normalLoc = 1, texCoordLoc = 2, colorLoc = 3;
	mat4 model, view, proj;
	MeshData mesh1, mesh2;
	MeshBuffer mb1, mb2;
	FBO fbo;
	MeshBuffer cubeMeshBuffer1, cubeMeshBuffer2, cubeMeshBuffer3;
	Texture texture;
	Behavior rotateBehavior;

	vec3 l1_diffuse = vec3(0.0, 1.0, 0.0);
	vec3 l1_specular = vec3(1.0, 1.0, 1.0);

	vec3 l2_diffuse = vec3(0.0, 0.0, 1.0);
	vec3 l2_specular = vec3(1.0, 1.0, 1.0);

  void onCreate() {

    rh.loadTexture(texture, "resources/hubble.jpg");
    texture.minFilter(GL_NEAREST);
    texture.maxFilter(GL_NEAREST);

	rh.loadProgram(textureProgram, "resources/texture", posLoc, -1, texCoordLoc, -1);
    rh.loadProgram(phongProgram, "resources/phong", posLoc, normalLoc, texCoordLoc, -1);
	rh.loadProgram(programColor, "resources/color", posLoc, normalLoc, -1, colorLoc);

	MeshData md1;
	addCube(md1, true, 0.95);

	MeshData md2;
	addCube(md2, true, 0.5);

	MeshData md3;
	addCube(md3, 0.33); //this version makes normals, texcoords, and colors each side with a different default color

	cubeMeshBuffer1.init(md1, posLoc, normalLoc, texCoordLoc, -1);
	cubeMeshBuffer2.init(md2, posLoc, normalLoc, texCoordLoc, -1);

	cubeMeshBuffer3.init(md3, posLoc, normalLoc, -1, colorLoc);

	fbo.create(32, 32);													// M_PI, M_PI_2, M_PI * 2   when we force radians
	rotateBehavior = Behavior(now()).delay(1000).length(5000).range(vec3(180.0, 90.0, 360.0)).reversing(true).repeats(-1).sine();


	proj = glm::perspective(45.0, 1.0, 0.1, 100.0);
	view = glm::lookAt(vec3(0.0, 0.0, 5), vec3(0, 0, 0), vec3(0, 1, 0));
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

	  //printf("frameCount = %d\n", frameCount);
	  model = glm::mat4(1.0);

	  vec3 totals = rotateBehavior.tick(now()).totals();
	  model = glm::rotate(model, totals.x, vec3(1.0f, 0.0f, 0.0f));
	  model = glm::rotate(model, totals.y, vec3(0.0f, 1.0f, 0.0f));
	  model = glm::rotate(model, totals.z, vec3(0.0f, 0.0f, 1.0f));


	  //draw cube 1 into an offscreen texture
	  fbo.bind(); {
		  glViewport(0, 0, fbo.width, fbo.height);
		  glClearColor(0.1, 0.1, 0.1, 1.0);
		  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		  draw(model, cubeMeshBuffer1, texture, textureProgram);

	  } fbo.unbind();


	  //draw cube 2 with the offscreen texture using phong shading
	  glViewport(0, 0, width, height);
	  glClearColor(0.0, 0.0, 0.0, 1.0);
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	  model = glm::mat4(1.0);

	  model = glm::translate(model, vec3(1.0, 0.0, 0.0));
	  model = glm::rotate(model, totals.x, vec3(1.0f, 0.0f, 0.0f));
	  model = glm::rotate(model, totals.y, vec3(0.0f, 1.0f, 0.0f));
	  model = glm::rotate(model, totals.z, vec3(0.0f, 0.0f, 1.0f));

	  draw(model, cubeMeshBuffer2, fbo.texture, phongProgram);

	  //draw cube 3 - a colored cube

	  model = mat4(1.0);
	  model = glm::translate(model, vec3(-1.0, 0.0, 0.0));

	  model = glm::rotate(model, -totals.x, vec3(1.0f, 0.0f, 0.0f));
	  model = glm::rotate(model, -totals.y, vec3(0.0f, 1.0f, 0.0f));
	  model = glm::rotate(model, -totals.z, vec3(0.0f, 0.0f, 1.0f));

	  programColor.bind(); {
		  glUniformMatrix4fv(programColor.uniform("model"), 1, 0, ptr(model));
		  glUniformMatrix4fv(programColor.uniform("view"), 1, 0, ptr(view));
		  glUniformMatrix4fv(programColor.uniform("proj"), 1, 0, ptr(proj));

		  cubeMeshBuffer3.draw();

	  } programColor.unbind();

  }

};
int main(){
	FBOExample().start("FBOExample");
	return 0;
}
