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

#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

using namespace aluminum;
using glm::ivec2;
using glm::to_string;

class TextureExample : public RendererIOS {
  
  
  
public:
  
  Program program;
   // GLuint vao; //, vbo, ibo, indices[3] = {0,1,2};
  
    /*
  vec3 vertices[6] = {
    vec3( -1.0, -1.0, 0.0 ), vec3( 0.0, 1.0, 0.0 ), vec3( 1.0, -1.0, 0.0 ), //vertex
    vec3( 1.0,0.0,0.0 ), vec3( 0.0,1.0,0.0 ), vec3( 0.0,0.0,1.0 ), //color
  };
  */
    
  GLint posLoc = 0;
  GLint tcLoc = 1;
  mat4 proj;
  mat4 view;
    mat4 model;
  
    Texture t1;
    MeshBuffer mb1;
    
    float bloomAmt = 1.0;

    
  int frameNum = 0;
  
  ResourceHandler rh;
  
  void loadProgram(Program &p, const std::string& name) {
    
    p.create();
    
    string sv = rh.pathToResource(name, "vsh");
    p.attach(rh.contentsOfFile(sv), GL_VERTEX_SHADER);
    
    glBindAttribLocation(p.id(), posLoc, "vertexPosition");
    glBindAttribLocation(p.id(), tcLoc, "vertexTexCoord");
   
    string sp = rh.pathToResource(name, "fsh");
    p.attach(rh.contentsOfFile(sp), GL_FRAGMENT_SHADER);
    
    p.link();
  }
  
  virtual void onCreate() {
    
    getEAGLView().multipleTouchEnabled = TRUE;
    
    // Load our shader program
    loadProgram(program, "texture");
    
      rh.loadTexture(t1, "javier.png");
      
    // Create a vertex array object
   // glGenVertexArraysOES( 1, &vao );
   // glBindVertexArrayOES( vao );
    
      /*
    // Create and initialize a buffer object
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_DYNAMIC_DRAW );
    
    glEnableVertexAttribArray( posLoc );
    glVertexAttribPointer( posLoc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0*sizeof(vec3)));
    
    glEnableVertexAttribArray( tcLoc );
    glVertexAttribPointer( tcLoc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(3*sizeof(vec3)));
    
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*3, indices, GL_DYNAMIC_DRAW);
    */
      
      mb1.init(MeshUtils::makeRectangle(), posLoc, -1, tcLoc, -1);
      
      proj = glm::perspective(45.0, 1.0, 0.1, 100.0);
      view = glm::lookAt(vec3(0.0,0.0,2), vec3(0,0,0), vec3(0,1,0) );
      model = glm::mat4();
      
      glEnable(GL_DEPTH_TEST);
     
      glViewport(0, 0, width, height);
      glClearColor(1.0,0.3,0.3,1.0);
  }
  
  virtual void onFrame(){
    
     // GLubyte* tdata = (GLubyte*)malloc( texture.width * texture.height * 4 );
    //AppDelegate *ad = (AppDelegate *)[[UIApplication sharedApplication] delegate];
    
    // Clear viewport
    //glViewport(0, 0, width, height);
    
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       glEnable(GL_TEXTURE_2D);
      program.bind(); {
          
          glUniformMatrix4fv(program.uniform("model"), 1, 0, ptr(model));
          glUniformMatrix4fv(program.uniform("view"), 1, 0, ptr(view));
          glUniformMatrix4fv(program.uniform("proj"), 1, 0, ptr(proj));
          
         // glUniform1f(program.uniform("bloom"), bloomAmt);
          glUniform1i(program.uniform("tex0"), 0);
          
          //texture.dump();
          
          t1.bind(GL_TEXTURE0); {
              mb1.draw();
          } t1.unbind(GL_TEXTURE0);
          
      } program.unbind();
    frameNum++;
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
 
  /*
  virtual void longPress(ivec2 mouse) {
    cout << "long press: " << to_string(mouse) << endl;
  }
  
  virtual void pinch(float scale) {
    cout << "pinch zoom: " << scale << endl;
  }
  virtual void pinchEnded() {
    cout << "pinch ended: " << endl;
  }
  */
};

/*
int main(int argc, char *argv[]) {
  
  TextureExample().start();
  
}
*/