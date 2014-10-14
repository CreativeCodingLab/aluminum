#include "Aluminum/Includes.hpp"
#include "Aluminum/FreeGlutGLView.hpp"
#include "Aluminum/RendererWin32.hpp"
#include "Aluminum/Program.hpp"
#include "Aluminum/MeshBuffer.hpp"
#include "Aluminum/MeshUtils.hpp"
#include "Aluminum/Texture.hpp"
#include "Aluminum/ResourceHandler.hpp"


using namespace aluminum;

class TextureExample : public RendererWin32{

public:

	glm::mat4 model, view, proj;

	Program program;

	GLint posLoc = 0;
	GLint texCoordLoc = 1;

	Texture texture;
	MeshBuffer mb1;
	ResourceHandler rh;

	void loadProgram(Program &p, const std::string& name) {

		p.create();

		p.attach(p.loadText(name + ".vsh"), GL_VERTEX_SHADER);
		glBindAttribLocation(p.id(), posLoc, "vertexPosition");
		glBindAttribLocation(p.id(), texCoordLoc, "vertexTexCoord");

		p.attach(p.loadText(name + ".fsh"), GL_FRAGMENT_SHADER);

		p.link();
		vec3 foo = vec3();
	}

	void onCreate() {
		rh.loadProgram(program, "week2b_Texturing/texture", posLoc, -1, texCoordLoc, -1);
		rh.loadTexture(texture, "week2b_Texturing/sun.jpg");

		mb1.init(MeshUtils::makeRectangle(), posLoc, -1, texCoordLoc, -1);

		proj = glm::perspective(45.0, 1.0, 0.1, 100.0);
		view = glm::lookAt(vec3(0.0, 0.0, -2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		model = glm::mat4();

		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, width, height);
		glClearColor(0.3, 0.3, 0.3, 1.0);
	}

	void onFrame(){

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program.bind(); {

			glUniformMatrix4fv(program.uniform("model"), 1, 0, ptr(model));
			glUniformMatrix4fv(program.uniform("view"), 1, 0, ptr(view));
			glUniformMatrix4fv(program.uniform("proj"), 1, 0, ptr(proj));

			glUniform1i(program.uniform("tex0"), 0);

			texture.bind(GL_TEXTURE0); {
				mb1.draw();
			} texture.unbind(GL_TEXTURE0);

		} program.unbind();
	}

	void onReshape() {
		glViewport(0, 0, width, height);
	}


	void mouseMoved(int px, int py) {
	}

};
