
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

#import "CaptureManager.h"

#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

using namespace aluminum;
using glm::ivec2;
using glm::to_string;

const int Ryp = 600;
const int Rxp = 600;

class Test1 : public RendererIOS {
    
    
    
public:
    
    
    
    ResourceHandler rh;
    
    
    Program program;
    
    MeshData md;
    MeshBuffer mb;
    
    GLint posLoc = 0;
    //GLint tcLoc = 1;
    
    mat4 proj;
    mat4 mv;
    vec2 TouchCords;
    
    // texture variables
    
    Texture t1;
    GLint tcLoc = 1;
    
    // capture variables
    CaptureManager* cm;
    
    void loadProgram(Program &p, const std::string& name) {
        
        p.create();
        
        string sv = rh.pathToResource(name, "vsh");
        p.attach(rh.contentsOfFile(sv), GL_VERTEX_SHADER);
        
        glBindAttribLocation(p.id(), posLoc, "vertexPosition");
        //  glBindAttribLocation(p.id(), colLoc, "vertexColor");
        glBindAttribLocation(p.id(), tcLoc, "vertexTexCoord");
        
        string sp = rh.pathToResource(name, "fsh");
        p.attach(rh.contentsOfFile(sp), GL_FRAGMENT_SHADER);
        
        p.link();
    }
    
    virtual void onCreate() {
        // Load our shader program
        loadProgram(program, "basicNew");
        
        
        
        
        rh.loadTexture(t1, "grid3.png");
        // create the data mesh
        
        
        float xl = -1.0;
        float xu = 1.0;
        float yl = -1.0;
        float yu = 1.0;
        
        
        const int numElems = Ryp*Rxp;
        
        
        //data = (GLubyte*) malloc (_w*_h*4*sizeof(GLubyte));
        
        
        vec3 *ts = (vec3*) malloc(numElems * sizeof(vec3));
        vec3 *vs = (vec3*) malloc(numElems * sizeof(vec3));
        
        //vec3 ts[numElems] = {vec3()};
        //vec3 vs[numElems] = {vec3()};
        
        //  printf("sizeof vec3 = %ld", sizeof(vec3));
        //  exit(0);
        for (int y =0; y<Ryp; y++) {
            for (int x=0; x < Rxp ; x++) {
                
                vs[Rxp*y+x].x= xl +x*(xu-xl)/(float)(Rxp-1.0);
                vs[Rxp*y+x].y= yl +y*(yu-yl)/(float)(Ryp-1.0);
                vs[Rxp*y+x].z = 0.0f;
                
                ts[Rxp*y+x].x = x/(float)Rxp;
                ts[Rxp*y+x].y = 1.0 - y/(float)Ryp;
                ts[Rxp*y+x].z = 0.0;
                
                // printf("idx = %d\n", Rxp*y+x);
            }
        }
        
        
        // index vector
        //   printf("num elems in is = %d\n", (Rxp*2*(Ryp-1)+2*(Ryp-2)));
        //GLuint is[Rxp*2*(Ryp-1)+2*(Ryp-2)];
        GLuint *is = (GLuint*) malloc(  (Rxp*2*(Ryp-1)+2*(Ryp-2)) * sizeof(GLuint));
        
        int q =0;
        
        for (int y =0; y<Ryp-1; y++) {
            for (int x=0; x < Rxp ; x++) {
                is[q] = x + y*Rxp;
                q++;
                is[q] = x + (y+1)*Rxp;
                q++;
                
                // printf("q = %d\n", q);
            }
            if( y < Ryp-2){ // the degenerate triangles
                
                //repiting last one
                is[q] = (Rxp-1) + (y+1)*Rxp;
                q++;
                //repiting next one
                is[q] =  (y+1)*Rxp;
                q++;
                
                // printf("q = %d\n", q);
            }
            
        }
        
        
        
        
        
        //        GLuint is[3] = {0,1,2};
        
        //        vec3 vs[3] = {
        //            vec3( -1.0, -1.0, 0.0 ), vec3( 0.0, 1.0, 0.0 ), vec3( 1.0, -1.0, 0.0 ) };
        //        vec3 cs[3] = {
        //            vec3( 1.0,0.0,0.0 ), vec3( 0.0,1.0,0.0 ), vec3( 0.0,0.0,1.0 )};
        
        md.vertex(vs,Ryp*Rxp);
        md.texCoord(ts, Ryp*Rxp);
        //        md.vertex(vs, 3);
        //       md.color(cs, 3);
        //        md.index(is, 3);
        md.index(is, Rxp*2*(Ryp-1)+2*(Ryp-2));
        //        mb.init(md, posLoc, -1, -1, colLoc);
        mb.init(md, posLoc, -1,tcLoc, -1);
        
        // Set up modelvew and projection matrix
        // proj = glm::perspective(45.0, 1.0, 0.1, 100.0);
        // Prjection for plane (-1,1,-1,1) filling the screen
        // camera at 10.0 near plane 1.0
        proj = glm::frustum(-1.0/10.0, 1.0/10.0, -1.0/10.0, 1.0/10.0, 1.0, 20.0);
        mv = glm::lookAt(vec3(0,0,10.0), vec3(0,0,-5.0), vec3(0,1,0) );
        
        glEnable(GL_DEPTH_TEST);
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        free(ts);
        free(vs);
        free(is);
        
        
        cm = [[CaptureManager alloc] init:AVCaptureSessionPresetHigh side:AVCaptureDevicePositionFront];
        [cm startCapture];
        
    }
    
    virtual void onFrame(){
        // Clear viewport
        glViewport(0, 0, width, height);
        glClearColor(1.0,1.0,1.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);
        
        
        [cm updateTextureWithNextFrame];
        
        if (!cm.textureReady) {
            return;
        }
        
      //  printf("here... drawing texture?\n");
        program.bind(); {
            glUniformMatrix4fv(program.uniform("mv"), 1, 0, ptr(mv));
            glUniformMatrix4fv(program.uniform("proj"), 1, 0, ptr(proj));
            glUniform2fv(program.uniform("MouseCords"), 1, ptr(TouchCords));
            glUniform1i(program.uniform("tex0"), 0);
            glUniform1i(program.uniform("tex1"), 1);
            t1.bind(GL_TEXTURE1); {
            cm.captureTexture.bind(GL_TEXTURE0); {
                mb.drawTriangleStrip();
            } cm.captureTexture.unbind(GL_TEXTURE0);
                } t1.unbind(GL_TEXTURE1);
            
        } program.unbind();
        
        
    }
    
    
    
    virtual void touchBegan(ivec2 mouse) {
        cout << "touch began: " << to_string(mouse) << endl;
        TouchCords.x = 2*(mouse.x/(float)width - 0.5);
        TouchCords.y = 2*(mouse.y/(float)height - 0.5);
    }
    
    virtual void touchMoved(ivec2 prevMouse, ivec2 mouse) {
        cout << "touch moved: prev:" << to_string(prevMouse) << ", current: " << to_string(prevMouse) << endl;
        TouchCords.x =  2*(mouse.x/(float)width - 0.5);
        TouchCords.y = 2*(mouse.y/(float)height - 0.5);
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
    
    Test1().start();
    
}
