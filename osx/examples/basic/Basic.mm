
#include "Aluminum/Includes.hpp"

#include "Aluminum/RendererOSX.h"
#include "Aluminum/MeshBuffer.hpp"
#include "Aluminum/MeshData.hpp"
#include "Aluminum/MeshUtils.hpp"
#include "Aluminum/Program.hpp"
#include "Aluminum/Texture.hpp"

#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

using namespace aluminum;

class Basic : public RendererOSX {
  public:

    Program program;
    GLuint vao, vbo, ibo;
    GLuint indices[3] = {0,1,2};

    vec3 vertices[6] = {
      vec3( -1.0, -1.0, 0.0 ), vec3( 0.0, 1.0, 0.0 ), vec3( 1.0, -1.0, 0.0 ), //vertex
      vec3( 1.0,0.0,0.0 ), vec3( 0.0,1.0,0.0 ), vec3( 0.0,0.0,1.0 ), //color
    };

    GLint posLoc = 0;
    GLint colLoc = 1;
    mat4 proj;
    mat4 mv;


    void loadProgram(Program &p, const std::string& name) {

      p.create();

      p.attach(p.loadText(name + ".vsh"), GL_VERTEX_SHADER);
      glBindAttribLocation(p.id(), posLoc, "vertexPosition");
      glBindAttribLocation(p.id(), colLoc, "vertexColor");

      p.attach(p.loadText(name + ".fsh"), GL_FRAGMENT_SHADER);

      p.link();
    }

    virtual void onCreate() {

      // Load our shader program
      loadProgram(program, "resources/basic");

      // Create a vertex array object
      glGenVertexArrays( 1, &vao );
      glBindVertexArray( vao );

      // Create and initialize a buffer object
      glGenBuffers( 1, &vbo );
      glBindBuffer( GL_ARRAY_BUFFER, vbo );
      glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_DYNAMIC_DRAW );

      glEnableVertexAttribArray( posLoc );
      glVertexAttribPointer( posLoc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0*sizeof(vec3)));

      glEnableVertexAttribArray( colLoc );
      glVertexAttribPointer( colLoc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(3*sizeof(vec3)));

      glGenBuffers(1, &ibo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*3, indices, GL_DYNAMIC_DRAW);

      // Set up modelvew and projection matrix
      proj = glm::perspective(45.0, 1.0, 0.1, 100.0);
      mv = glm::lookAt(vec3(0,0,-2.5), vec3(0,0,0), vec3(0,1,0) );
    }

    virtual void onFrame(){

      handleKeys();
      handleMouse();

      // Clear viewport
      glViewport(0, 0, width, height);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Draw our vbos to the screen
      program.bind(); {

	glUniformMatrix4fv(program.uniform("mv"), 1, 0, ptr(mv));
	glUniformMatrix4fv(program.uniform("proj"), 1, 0, ptr(proj));

	glBindVertexArray( vao ); 
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	//glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

        glBindVertexArray( 0 ); 
      } program.unbind();

    }

    void handleMouse() {
      if (isDragging) {
	printf("in Basic: mouseDragged %d/%d\n", mouseX, mouseY);
      }

      if (isMoving) {
	printf("in Basic: mouseMoved %d/%d\n", mouseX, mouseY);
      }

      if (isPressing) {
	printf("in Basic: mouseDown %d/%d\n", mouseX, mouseY);
      }


      if (isReleasing) {
	printf("in Basic: mouseUp %d/%d\n", mouseX, mouseY);
      }

      isDragging = false;
      isMoving = false;
      isPressing = false; 
      isReleasing = false;
    }

    void handleKeys() {

      if (keysDown[kVK_ANSI_A]) {
	printf("you pressed an 'A'! \n");	
	keysDown[kVK_ANSI_A] = false;
      }

      if (keysUp[kVK_ANSI_A]) {
	printf("you released an 'A'! \n");
	keysUp[kVK_ANSI_A] = false;
      }
    }

};

int main(){ 
  return Basic().start("aluminum::Basic", 100, 100, 400, 300); 
}
