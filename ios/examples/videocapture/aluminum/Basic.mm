
#import "Includes.hpp"

#import "MeshBuffer.hpp"
#import "MeshData.hpp"
#import "MeshUtils.hpp"
#import "Program.hpp"
#import "Shapes.hpp"
#import "Texture.hpp"

#import "RendererIOS.h"
#import "AppDelegate.h"

#import "ResourceHandler.h"

#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

using namespace aluminum;
using glm::ivec2;
using glm::to_string;

class Basic : public RendererIOS {
    
    
    
public:
    
    ResourceHandler rh;
    
    Program program;
    
    MeshData md;
    MeshBuffer mb;
    
    GLint posLoc = 0;
    GLint colLoc = 1;
    
    mat4 proj;
    mat4 mv;
    
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
       
        
        GLuint is[3] = {0,1,2};
        
        vec3 vs[3] = {
            vec3( -1.0, -1.0, 0.0 ), vec3( 0.0, 1.0, 0.0 ), vec3( 1.0, -1.0, 0.0 ) };
        vec3 cs[3] = {
            vec3( 1.0,0.0,0.0 ), vec3( 0.0,1.0,0.0 ), vec3( 0.0,0.0,1.0 )};
        
        
        md.vertex(vs, 3);
        md.color(cs, 3);
        md.index(is, 3);
        
        mb.init(md, posLoc, -1, -1, colLoc);
        
        
        // Set up modelvew and projection matrix
        proj = glm::perspective(45.0, 1.0, 0.1, 100.0);
        mv = glm::lookAt(vec3(0,0,-2.5), vec3(0,0,0), vec3(0,1,0) );
    }
    
    virtual void onFrame(){
        
        // Clear viewport
        glViewport(0, 0, width, height);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        program.bind(); {
            glUniformMatrix4fv(program.uniform("mv"), 1, 0, ptr(mv));
            glUniformMatrix4fv(program.uniform("proj"), 1, 0, ptr(proj));
            
            mb.draw();
        } program.unbind();
        
        
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
    virtual void pinchEnded() {
        cout << "pinch ended: " << endl;
    }
    
};


int main(int argc, char *argv[]) {
    printf("inin\n");
    Basic().start();
    printf("outout\n"); 
}
