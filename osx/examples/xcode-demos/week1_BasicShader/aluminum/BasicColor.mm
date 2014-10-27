#import <Aluminum/Includes.hpp>

#import <Aluminum/RendererOSX.h>
#import <Aluminum/MeshBuffer.hpp>
#import <Aluminum/MeshUtils.hpp>
#import <Aluminum/Program.hpp>
#import <Aluminum/ResourceHandler.h>

#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

using namespace aluminum;


/* Simple demo that draws two triangles */
class Basic : public RendererOSX {
public:
  
  //The ResourceHandler object manages external resources, such as text files containing shader programs, image files, video files, and assets.
  ResourceHandler rh;
  
  //A Program object manages the loading, compiling, and interacting with a shader program
  Program program;
 
  //OpenGL IDs for a default vertex array object, a vertex buffer object, and an index buffer object that will stored on the GPU.
  GLuint vao, vbo, ibo;
  
  //Defines six indices to represent the two triangles made from the four vertices.
  GLuint indices[6] = {0,1,2,2,1,3};
  
  //Defines an array of vertex data; 3 vec3s of position, followed by 3 vec3s of color info
  vec3 vertices[8] = {
    vec3( -1.0, -1.0, 0.0 ), vec3( -1.0, 1.0, 0.0 ), vec3( 1.0, -1.0, 0.0 ), vec3( 1.0, 1.0, 0.0), //vertex
    vec3( 1.0, 0.0, 0.0 ), vec3( 0.0, 1.0, 0.0 ), vec3( 0.0, 0.0, 1.0 ), vec3( 1.0, 1.0, 1.0 ) //color
  };
  
  //Defines the default locations for the attribute variables in the vertex shader
  GLint posLoc = 0;
  GLint colLoc = 1;
  
  //mat4s for the projections matrix and modelview matrix passed in as uniforms to the vertex shader
  mat4 proj, mv;
  
  
  
  
  void loadProgram(Program &p, const std::string& name) {
    
    //Initializes a Program object
    p.create();
    
    //Gets the path to the vertex shader (ending in ".vsh")
    string sv = rh.pathToResource(name, "vsh");
    // cout << "path of vertex shader is: " << sv << endl;
    
    //Compiles the vertex shader and attaches it to our Program object
    p.attach(rh.contentsOfFile(sv), GL_VERTEX_SHADER);
    
    //Binds attribute variables to a particular ID
    glBindAttribLocation(p.id(), posLoc, "vertexPosition");
    glBindAttribLocation(p.id(), colLoc, "vertexColor");
    
    
    //Gets the path to the fragment shader (ending in ".fsh")
    string sp = rh.pathToResource(name, "fsh");
    // cout << "path of vertex shader is: " << sp << endl;
 
    //Compiles the fragment shader and attaches it to our Program object
    p.attach(rh.contentsOfFile(sp), GL_FRAGMENT_SHADER);
    
    //Links the Program object to the GPU so that it can be activated when needed
    p.link();
  }
  
  //onCreate runs one time right after the OpenGL context is established. A good place to load shaders, load textures, set up vertex data, initialize global parameters.
  virtual void onCreate() {
    
    //** Step 1 **//
    
    // Load our shader program, by default looks for two files, a .vsh file and .fsh file
    loadProgram(program, "basic_s");
    
    //** Step 2 **//
    
    // Create a vertex array object to store geometry and related information (the buffer of data itself, the layout of the data, and how the data is indexed).
    glGenVertexArrays( 1, &vao ); //allocate space for a VAO on the GPU
    glBindVertexArray( vao ); //activate this space so we can store information there
    
    // Create and initialize a vertex buffer object to store geometry data
    glGenBuffers( 1, &vbo ); //allocate space for vertex data on the GPU
    glBindBuffer( GL_ARRAY_BUFFER, vbo ); //activate this space so we can store vertex data there
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_DYNAMIC_DRAW ); //pass data to the GPU
    
    //These two commands first enable the variable referenced by the ID stored in "posLoc" to receive data into the "vertexPosition" variable in the vertexShader (see the loadProgram method above), and then explain how to map the data in our "vertices" array to this vertexPosition variable when its streamed in.
    //The glVertexAttribPointer method defines the location and format of data mapped to a particular attribute. Here we are using a vec3 to store the x,y, and z position information (i.e., 3 floats). They will be read in from the beginning of our vertices array.
    glEnableVertexAttribArray( posLoc ); //enable this variable ("vertexPosition") to receive vertex data
    glVertexAttribPointer( posLoc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0*sizeof(vec3))); //define how the data will be mapped to the "vertexPosition" variable
    
    //Similar to above, we enable the "vertexColor" variable associated with the "colLoc" ID and provide the location and format of the color data within our vertices array. We are using a vec3 to store an RGB color (i.e., 3 floats), and this data is located starting from the vec3 at index 4 (i.e., after the four position vec3s).
    glEnableVertexAttribArray( colLoc );
    glVertexAttribPointer( colLoc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(4*sizeof(vec3)));
    
    //An index buffer object is used to more efficiently pass a single vertex that can be used more than once (i.e., a point shared with multiple triangles). In this example, Index 0 will point to a position at the zeroth vec3 in the data array and a color at the fourth vec3 in the data array. Index 1 will point to the first vec3 and the fifth vec3, etc... The indexes are repeated because the second triangle shares two vertices with the previous triangle.
    glGenBuffers(1, &ibo); //allocate space for the indices on the GPU
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //activate this space so we can store indices there
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*6, indices, GL_DYNAMIC_DRAW);
    
    //** Step 3 **//
    
    // Set up Modelview and Projection matrix
    proj = glm::perspective(glm::radians(45.0), 1.0, 0.1, 100.0); //defines how to project 3D data to 2D image
    mv = glm::lookAt(vec3(0,0,2.5), vec3(0,0,0), vec3(0,1,0) ); //defines where the camera is positioned
  }
  
  //onFrame syncs with the refresh rate of the display (e.g., 60fps). Here we can send information to the GPU to define exactly how the pixels on the window should look.
  virtual void onFrame(){
    
    handleKeys(); //checks if any keys events have occurred since the last frame
    handleMouse(); //checks if any mouse events have occured since the last frame
    
    glViewport(0, 0, width, height); //defines the active viewport to match the size of our window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears color and depth info from the viewport
    
    
    // the program.bind() activates our shader program so that we can 1. pass data to it and 2. let it draw to the active viewport in our window
    program.bind(); {
      
      glUniformMatrix4fv(program.uniform("mv"), 1, 0, ptr(mv)); //pass in the view matrix
      glUniformMatrix4fv(program.uniform("proj"), 1, 0, ptr(proj)); //pass in the projection matrix
      
      glBindVertexArray( vao ); //binds our vertex array object, containing all our data and information about how it's organized and indexed
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, BUFFER_OFFSET(0)); //passes the entire data buffer to the GPU as a set of triangles; that is, read the index array three items at a time.
      
      glBindVertexArray( 0 );
    } program.unbind();
    
  }
  
  //examples of how to react to a mouse event
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
  
  //examples of how to react to a key event
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

int main() {
  //instantiate our "Basic" program and call its parent's start() method to create a window with an active OpenGL context.
  return Basic().start("aluminum::Basic", 100, 100, 400, 300);
}
