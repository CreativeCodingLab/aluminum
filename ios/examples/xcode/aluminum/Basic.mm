#import <Aluminum/Aluminum.h>
//#import "Includes.hpp"
//
//#import "MeshBuffer.hpp"
//#import "MeshData.hpp"
//#import "MeshUtils.hpp"
//#import "Program.hpp"
//#import "Shapes.hpp"
//#import "Texture.hpp"
//
//#import "RendererIOS.h"
//#import "AppDelegate.h"
//#import "ResourceHandler.h"
#import "Interactions.h"


#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

using namespace aluminum;
using glm::ivec2;
using glm::to_string;

class Basic : public RendererIOS {
  
  
  
public:
  
  Program program;
  GLuint vao, vbo, ibo, indices[3] = {0,1,2};
  
  vec3 vertices[6] = {
    vec3( -1.0, -1.0, 0.0 ), vec3( 0.0, 1.0, 0.0 ), vec3( 1.0, -1.0, 0.0 ), //vertex
    vec3( 1.0,0.0,0.0 ), vec3( 0.0,1.0,0.0 ), vec3( 0.0,0.0,1.0 ), //color
  };
  
  GLint posLoc = 0;
  GLint colLoc = 1;
  mat4 proj;
  mat4 mv;
  
  int frameNum = 0;
  
  ResourceHandler rh;
  
  void loadProgram(Program &p, const std::string& name) {
    
    p.create();
    
    string sv = rh.pathToResource(name, "vsh");
    p.attach(rh.contentsOfFile(sv), GL_VERTEX_SHADER);
    
    glBindAttribLocation(p.id(), posLoc, "vertexPosition");
    glBindAttribLocation(p.id(), colLoc, "vertexColor");
   
    string sp = rh.pathToResource(name, "fsh");
    p.attach(rh.contentsOfFile(sp), GL_FRAGMENT_SHADER);
    
    p.link();
  }
  
  virtual void onCreate() {
    // Load our shader program
    loadProgram(program, "basic");
    
    // Create a vertex array object
    glGenVertexArraysOES( 1, &vao );
    glBindVertexArrayOES( vao );
    
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
    
    AppDelegate *ad = (AppDelegate *)[[UIApplication sharedApplication] delegate];
    EAGLView* ev = ad.glView;
    
    //for (UITouch* touch in ev->touches) {
    

    // Clear viewport
    glViewport(0, 0, width, height);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Draw our vbos to the screen
    program.bind(); {
      
      glUniformMatrix4fv(program.uniform("mv"), 1, 0, ptr(mv));
      glUniformMatrix4fv(program.uniform("proj"), 1, 0, ptr(proj));
      
      glBindVertexArrayOES( vao );
      glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
      glBindVertexArrayOES( 0 );
    } program.unbind();
    
    
    frameNum++;
  }
  
  virtual void installInteractions(EAGLView* ev) {
    
    printf("overriding installInteractions...\n");
    [[Interactions alloc] init:ev];

  }
  
  virtual void touchBegan(ivec2 mouse) {
    cout << "touch began: " << to_string(mouse) << endl;
  }
  
  virtual void touchMoved(ivec2 prevMouse, ivec2 mouse) {
    cout << "touch moved: prev:" << to_string(prevMouse) << ", current: " << to_string(prevMouse) << endl;
  }
  
  virtual void touchEnded(ivec2 mouse) {
    cout << "touch ended: " << to_string(mouse) << endl;
  }
  
  virtual void longPress(ivec2 mouse) {
    cout << "long press: " << to_string(mouse) << endl;
  }
  
  virtual void pinch(float scale) {
    cout << "pinch zoom: " << scale << endl;
  }
  
  
   void pinch2(float scale) {
    cout << "pinch2 zoom: " << scale << endl;
  }
  
  virtual void pinchEnded() {
    cout << "pinch ended: " << endl;
  }
  
};

/*
int main(int argc, char *argv[]) {
 
  return Basic().useMultitouch().useSwipeUp().start();
  
}
 */

