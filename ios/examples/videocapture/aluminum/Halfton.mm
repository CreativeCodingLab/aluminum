
#import "Includes.hpp"

#import "MeshBuffer.hpp"
#import "MeshData.hpp"
#import "MeshUtils.hpp"
#import "Program.hpp"
#import "Shapes.hpp"
#import "Texture.hpp"
#include "FBO.hpp"
#import "RendererIOS.h"
#import "AppDelegate.h"

#import "ResourceHandler.h"
#import "CaptureManager.h"

#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

using namespace aluminum;
using glm::ivec2;
using glm::to_string;



class Halfton : public RendererIOS {
    
    
    
public:
    ResourceHandler rh;
    bool touchpresent = false;
    Program programGray,programDraw;
    FBO fboGray;
    MeshBuffer clipRect; // for the first texture for gradient calculation
    MeshData md;
    MeshBuffer ThePoints;
    Texture t1;
        bool bufferReady = false;
    CaptureManager* cm;
    
    GLint posLoc = 0;
    GLint tcLoc = 1;
    
    mat4 proj;
    mat4 mv;
    vec2 TouchCords;
    
    mat4 webcamMatrix = mat4();
    mat4 touchMatrix = mat4();

    const int texX = 128;
    const int texY = 128;
    
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
    
    void InitVertex(){
        
        
    }
    
    
    virtual void onCreate() {
        // Load our shader program
        InitVertex();
        loadProgram(programGray, "GrayScale");
        loadProgram(programDraw, "points");
        
        
        fboGray.create(texX, texY);
        
            clipRect.init(MeshUtils::makeClipRectangle(), posLoc, -1, tcLoc, -1);
        
        //  mb.init(MeshUtils::makeSurface(Rxp, Ryp, -1.0, 1.0, -1.0, 1.0, true), posLoc, -1, tcLoc, -1);
        
        /* Javier -- if you are planning on doing a full screen app, you don't need to use projection - ah I see you are using the z-axis for distorting the mesh */
        // Prjection for plane (-1,1,-1,1) filling the screen
        // camera at 3.0 near plane 1.0
        //        proj = glm::frustum(-1.0/10.0, 1.0/10.0, -1.0/10.0, 1.0/10.0, 1.0, 50.0);
        //        mv = glm::lookAt(vec3(0,0,10.0), vec3(0,0,-5.0), vec3(0,1,0) );
        proj = glm::mat4();
        mv = glm::mat4();
        
        //set up matrices to account for webcam distortion - this is for front camera/portrait. Will be different for other orientations.
        webcamMatrix = glm::scale(webcamMatrix, vec3(-1.0,1.0,1.0));
        webcamMatrix = glm::rotate(webcamMatrix , -90.0f, vec3(0,0,1));
        mv *= webcamMatrix;
        
        touchMatrix = glm::scale(touchMatrix, vec3(1.0,-1.0,1.0));
        touchMatrix = glm::rotate(touchMatrix , -90.0f, vec3(0,0,1));
        
        
        glEnable(GL_DEPTH_TEST);
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        
        //    cm = [[CaptureManager alloc] init:AVCaptureSessionPresetHigh side:AVCaptureDevicePositionFront];
        cm = [[CaptureManager alloc] init:AVCaptureSessionPreset640x480 side:AVCaptureDevicePositionFront];
        //
        [cm startCapture];
        
    }
    

    
    
    void HalfToner( GLubyte* dst, int w, int h) {
        
        
        int oldPx = 0;
        int newPx = 0;
        int qError = 0;
        int blackcount =0;
        int threshold = 128;
        int r1, r2,  bl1, bc1, br1, bc2;
        for (int j = 0; j < h-2; j++) {
            for (int i = 1; i < w-2; i++) {
                
                oldPx = dst[4*j*w+4*i];
                newPx = oldPx < threshold ? 0 : 255;
                dst[4*j*w+4*i] = newPx;
                
                qError = oldPx - newPx;
                
                // super duper ugly 2 part value clipping (to avoid values beyond 0-255)
                r1  = MIN(dst[4*j*w+4*(i+1)]     + (int)(.125f * qError), 255);
                r2  = MIN(dst[4*j*w+4*(i+2)]     + (int)(.125f * qError), 255);
                bl1 = MIN(dst[4*(j+1)*w+4*(i-1)] + (int)(.125f * qError), 255);
                bc1 = MIN(dst[ 4*(j+1)*w+4*(i)]   + (int)(.125f * qError), 255);
                br1 = MIN(dst[4*(j+1)*w+4*(i+1)] + (int)(.125f * qError), 255);
                bc2 = MIN(dst[4*(j+2)*w+4*(i)]   + (int)(.125f * qError), 255);
                //            new1 = MIN(dst[(j+1)*w+(i+2)] + (int)(.125f * qError), 255);
                //            new2 = MIN(dst[(j+2)*w+(i+1)]   + (int)(.125f * qError), 255);
                
                
                dst[4*j*w+4*(i+1)]     = MAX(r1 , 0);
                dst[4*(j+1)*w+4*(i-1)] = MAX(bl1, 0);
                dst[4*(j+1)*w+4*(i)]   = MAX(bc1, 0);
                dst[4*(j+1)*w+4*(i+1)] = MAX(br1, 0);
                dst[4*(j+2)*w+4*(i)]   = MAX(bc2, 0);
                
            }
        }
  
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
        
        // gray Calculation
        
//        cout<<"ACCA"<<endl;
//        exit(0);
//        
        
        programGray.bind(); {
            
            glUniformMatrix4fv(programGray.uniform("mv"), 1, 0, ptr(mv));
            glUniformMatrix4fv(programGray.uniform("proj"), 1, 0, ptr(proj));
            // sending the dimensions of the image
            glUniform1i(programGray.uniform("tex0"), 0);
            //glUniform1i(program.uniform("tex1"), 1);
            fboGray.bind(); {
                cm.captureTexture.bind(GL_TEXTURE0);{
                    clipRect.draw();
                } cm.captureTexture.unbind(GL_TEXTURE0);
            }fboGray.unbind();
            
            // buffer for reading the texture
            
            
            
            
        } programGray.unbind();
        
        
   
        
       
        
        
       
       
        
 

        
     //  if ([cm nextFrame]) {
            if (bufferReady == false) {
                // Initial values for the mesh
                
                //        // limits
                // nodes
                int NumPoints = texX*texY;
                vec3* vs = new vec3[NumPoints];
                //     GLuint* is = new GLuint [10];
                
                for (int x = 0; x < NumPoints; x++) {
                    vs[x] = vec3(x/(float)NumPoints,x/(float)NumPoints,0.0);
                }
                
                md.vertex(vs,NumPoints);
                //      md.index(is, 10);
                ThePoints.init(md, posLoc, -1, -1, -1);
                delete vs;
                bufferReady = true;
            }
       
        
            //NOW WE ARE IN THE CPU

            GLubyte* TextuRead = new GLubyte[texX*texY*4];
            
            fboGray.bind(); {
                glReadPixels(0, 0, texX, texY, GL_RGBA, GL_UNSIGNED_BYTE, TextuRead);
            }  fboGray.unbind();
            
            
              HalfToner( TextuRead, texX, texY);
            
            //  cout<<blkpoints<<endl;
            
            
            // coping the vertex data
           
        int NumPoints = texX*texY;

            vec3* vs = new vec3[NumPoints];
            //        vec3* vs = new vec3[100];
                    int blckcount =0;
    
            
           for (int x = 0; x < texX; x++) {
               for (int y = 0; y < texX; y++) {

//                    for (int x = 0; x < texX-2; x++) {
//                        for (int y = 0; y < texY-2; y++) {
            //
//                            if ((TextuRead[4*texX*y + 4*x]==0)&&(blckcount<blkpoints)){
//                                vs[blckcount] = vec3(x/(float)texX,y/(float)texY,0.0);
//                                blckcount++;
//                            }
                   if (TextuRead[4*texX*y + 4*x]<128){
                       vs[blckcount] = vec3(-2*y/(float)texY+1.0,2*x/(float)texX-1.0,0.0);
                       blckcount++;
                   }
               }
                    }
            //        
            //  
                   md.reset();
           
                  md.vertex(vs,blckcount);
            // 
            //   
            //        
                ThePoints.update(md);
            //
                    delete vs;
            
                   delete TextuRead;
     
            
            
            
            
 //       }
        
        glBindFramebuffer(GL_FRAMEBUFFER, getDefaultFrameBuffer());
        
        glViewport(0, 0, width, height);
        glClearColor(1.0,1.0,1.0,1.0);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        // drawing
        programDraw.bind(); {
            glUniformMatrix4fv(programDraw.uniform("mv"), 1, 0, ptr(mv));
            glUniformMatrix4fv(programDraw.uniform("proj"), 1, 0, ptr(proj));
            
            ThePoints.drawPoints();
        } programDraw.unbind();
        
       
        
        
        
        
        
        
    }
    
    
    
    virtual void touchBegan(ivec2 mouse) {
        cout << "touch began: " << to_string(mouse) << endl;
        TouchCords.x = 2*(mouse.x/(float)width - 0.5);
        TouchCords.y = 2*(mouse.y/(float)height - 0.5);
        touchpresent = true;
    }
    
    virtual void touchMoved(ivec2 prevMouse, ivec2 mouse) {
        cout << "touch moved: prev:" << to_string(prevMouse) << ", current: " << to_string(prevMouse) << endl;
        TouchCords.x =  2*(mouse.x/(float)width - 0.5);
        TouchCords.y = 2*(mouse.y/(float)height - 0.5);
        touchpresent = true;
    }
    
    virtual void touchEnded(ivec2 mouse) {
        cout << "touch ended: " << to_string(mouse) << endl;
        touchpresent = false;
    }
    
    virtual void longPress(ivec2 mouse) {
        cout << "long press: " << to_string(mouse) << endl;
        touchpresent = false;
    }
    
    virtual void pinch(float scale) {
        cout << "pinch zoom: " << scale << endl;
    }
    virtual void pinchEnded() {
        cout << "pinch ended: " << endl;
    }
    
};

/*
int pppmain(int argc, char *argv[]) {

    Halfton().start();

}
 */
