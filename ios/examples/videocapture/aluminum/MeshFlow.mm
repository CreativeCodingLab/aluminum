
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


//Mesh dimensions

//const int MeshX = 180;//214;
//const int MeshY = 134;//160;
const int MeshX = 128;// test
const int MeshY = 128;

bool touchpresent = false;
class MeshFlow : public RendererIOS {
    
    
    
public:
    ResourceHandler rh;
    
    Program programGrad,programMesh;
    FBO fboGrad;
    MeshBuffer clipRect; // for the first texture for gradient calculation
    MeshData md;
    MeshBuffer mb;
    Texture t1;
    
    CaptureManager* cm;
    
    GLint posLoc = 0;
    GLint tcLoc = 1;
    
    mat4 proj;
    mat4 mv;
    vec2 TouchCords;
    
    mat4 webcamMatrix = mat4();
    mat4 touchMatrix = mat4();
    int camWidth = 640;
    int camHeight = 480;
    const int texX = MeshX;
    const int texY = MeshY;
    vec2 VertexPos [MeshX][MeshY];
    vec2 VertexVel [MeshX][MeshY];
    
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
        
        // set position on the grid and velocity to zero
        float xl = -1.0;
        float yl = -1.0;
        float xu = 1.0;
        float yu = 1.0;
        for (int x = 0; x < MeshX; x++) {
            for (int y = 0; y < MeshY; y++) {
                VertexPos[x][y] = vec2(xl +x*(xu-xl)/(float)(MeshX-1.0),
                                       yl +y*(yu-yl)/(float)(MeshY-1.0));
                VertexVel[x][y] = vec2(0.0,0.0);
                
            }
        }
        
    }
    
 
    
    virtual void onCreate() {
        // Load our shader program
        InitVertex();
        loadProgram(programGrad, "GradientCalc");
        loadProgram(programMesh, "Meshflow");
        
        
        fboGrad.create(texX, texY);
        
        //exit(0);
        //fboGrad.texture.wrapMode(GL_CLAMP_TO_EDGE);
        //rh.loadTexture(t1, "grid3.png");
        // create the data mesh
        //     mb.init(MeshUtils::makeClipGrid(Rxp, Ryp), posLoc, -1, tcLoc, -1);
        
        clipRect.init(MeshUtils::makeClipRectangle(), posLoc, -1, tcLoc, -1);
        
        // Initial values for the mesh
        
        //        // limits
        // nodes
        vec3* vs = new vec3[MeshX * MeshY];
        GLuint* is = new GLuint [2*(MeshX-1)*MeshY + 2*(MeshY-1)*MeshX ];
        
        // textu test
        vec3* ts = new vec3[MeshX * MeshY];
        
        
        for (int x = 0; x < MeshX; x++) {
            for (int y = 0; y < MeshY; y++) {
                
                vs[MeshX*y+x].x= VertexPos[x][y].x;
                vs[MeshX*y+x].y= VertexPos[x][y].y;
                vs[MeshX*y+x].z = 0.0f;
                
                //                ts[MeshX*y+x].x = x/(float)MeshX;
                //                ts[MeshX*y+x].y = 1.0 - y/(float)MeshY;
                //                ts[MeshX*y+x].z = 0.0;
            }
        }
        //indexes
        
        int q = 0;
        
        
        // Horizontal lines
        
        for (int y =0; y<MeshY; y++) {
            for (int x= 0; x < MeshX-1 ; x++) {
                is[q] = x + y*MeshX;
                q++;
                is[q] = (x+1) + y*MeshX;
                q++;
            }
        }
        
        // Vertical lines
        for (int x= 0; x < MeshX ; x++) {
            for (int y =0; y<MeshY-1; y++) {
                is[q] = x + y*MeshX;
                q++;
                is[q] = x + (y+1)*MeshX;
                q++;
            }
        }
        
        
        
        md.vertex(vs,MeshX*MeshY);
        md.index(is, 2*(MeshX-1)*MeshY + 2*(MeshY-1)*MeshX);
        //       md.texCoord(ts, MeshX*MeshY);
        
        mb.init(md, posLoc, -1, -1, -1);
        delete vs;
        delete is;
        delete ts;
        
        
        
        
        
        
        
        
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
        
        // Gradient Calculation
        
        
        
        
        programGrad.bind(); {
            
            glUniformMatrix4fv(programGrad.uniform("mv"), 1, 0, ptr(mv));
            glUniformMatrix4fv(programGrad.uniform("proj"), 1, 0, ptr(proj));
            // sending the dimensions of the image
            glUniform2fv(programGrad.uniform("CamDims"), 1, ptr(vec2(width, height)));
            glUniform1i(programGrad.uniform("tex0"), 0);
            //glUniform1i(program.uniform("tex1"), 1);
            fboGrad.bind(); {
                cm.captureTexture.bind(GL_TEXTURE0);{
                    clipRect.draw();
                } cm.captureTexture.unbind(GL_TEXTURE0);
            }fboGrad.unbind();
            
            // buffer for reading the texture
            
            
            
            
        } programGrad.unbind();
        
        
        //NOW WE ARE IN THE CPU
        
        GLubyte* TextuRead = new GLubyte[texX*texY*4];
        
        fboGrad.bind(); {
            
            
            glReadPixels(0, 0, texX, texY, GL_RGBA, GL_UNSIGNED_BYTE, TextuRead);
            /*
             for (int ppp = 0; ppp < 128*128*4; ppp+=4) {
             int col = ppp % (128);
             int row = ppp / (128*4);
             printf("red at %d/%d = %d\n", col, row, TextuRead[ppp]);
             }
             */
        }  fboGrad.unbind();
        
        
        glViewport(0, 0, width, height);
        
        glBindFramebuffer(GL_FRAMEBUFFER, getDefaultFrameBuffer());
        
        
        
        
        
        // updating vertex data:
        
        vec3* vs = new vec3[MeshX * MeshY];
        GLuint* is = new GLuint [2*(MeshX-1)*MeshY + 2*(MeshY-1)*MeshX ];
       // setup decente
//        float Kg = 0.014; // gradient constant
//        float Kt = .7; // elastic constant
//        float Kd = 0.24; // drag
//        float Dt = 0.4;
        
//        float Kg = 0.04; // gradient constant
//        float Kt = 1.9; // elastic constant
//        float Kd = 0.34; // drag
//        float Dt = 0.4;

        float Kg = 0.065; // gradient constant
        float Kt = 3.0; // elastic constant
        float Kd = 0.44; // drag
        float Dt = 0.2;
        float Km ; // touch costant
        
        // Updating velocities
        float Tnormx,Tnormy;
        for (int x = 0; x < MeshX; x++) {
            for (int y = 0; y < MeshY; y++) {
                
                // forces update
                if(x==0||x==MeshX-1||y==0||y==MeshY-1){
                    vs[MeshX*y+x].x= VertexPos[x][MeshY-1-y].y;
                    vs[MeshX*y+x].y= VertexPos[x][MeshY-1-y].x;
                }
                else{
                // Text dimention = mesh dimensions;
                    
                    if(!touchpresent){
                        Km = 0.0;
                        Tnormx = Tnormy =0.0;
                    }
                    else{
                        float disty = TouchCords.y - VertexPos[x][MeshY-1-y].y;
                        float distx = ((-TouchCords.x) - VertexPos[x][MeshY-1-y].x);
                        float magniSqu = (distx*distx +disty*disty);
                        Km = -0.5*exp(-0.5*magniSqu/(.1*.1));
                        Tnormx = (magniSqu!=0)?distx/sqrt(magniSqu):0.0;
                        Tnormy = (magniSqu!=0)?disty/sqrt(magniSqu):0.0;
                    }
                    
                float Fx =  -Kg*(TextuRead[4*texX*y + 4*x]/255.0 - 0.5) +
                            Kt*(VertexPos[x+1][MeshY-1-y].y-VertexPos[x][MeshY-1-y].y) +
                            Kt*(VertexPos[x-1][MeshY-1-y].y-VertexPos[x][MeshY-1-y].y) +
                            Kt*(VertexPos[x][MeshY-1-(y+1)].y-VertexPos[x][MeshY-1-y].y) +
                            Kt*(VertexPos[x][MeshY-1-(y-1)].y-VertexPos[x][MeshY-1-y].y) -
                            Kd*VertexVel[x][MeshY-1-y].y
                            +Km * Tnormy;
                    
                float Fy =   Kg*(TextuRead[4*texX*y + 4*x+1]/255.0 - 0.5) +
                            Kt*(VertexPos[x+1][MeshY-1-y].x-VertexPos[x][MeshY-1-y].x) +
                            Kt*(VertexPos[x-1][MeshY-1-y].x-VertexPos[x][MeshY-1-y].x) +
                            Kt*(VertexPos[x][MeshY-1-(y+1)].x-VertexPos[x][MeshY-1-y].x) +
                            Kt*(VertexPos[x][MeshY-1-(y-1)].x-VertexPos[x][MeshY-1-y].x) -
                            Kd*VertexVel[x][MeshY-1-y].x
                            +Km * Tnormx;
                
                    VertexVel[x][MeshY-1-y].y += Fx*Dt;
                    VertexVel[x][MeshY-1-y].x += Fy*Dt;
                    

                }
                    
            }
        }

        // Updating positions:
        
        for (int x = 1; x < MeshX-1; x++) {
            for (int y = 1; y < MeshY-1; y++) {
                float newposX = VertexPos[x][MeshY-1-y].y + VertexVel[x][MeshY-1-y].y*Dt;
                float newposY = VertexPos[x][MeshY-1-y].x + VertexVel[x][MeshY-1-y].x*Dt;
                VertexPos[x][MeshY-1-y].y = newposX;
                vs[MeshX*y+x].x=  newposX;
                VertexPos[x][MeshY-1-y].x = newposY;
                vs[MeshX*y+x].y=  newposY;
                vs[MeshX*y+x].z = 0.0f;
            
                
                
                VertexVel[x][MeshY-1-y].y*=0.5;
                VertexVel[x][MeshY-1-y].x *= 0.5;
            }
        }
        

        
        
        md.vertices().clear();
        md.vertex(vs,MeshX*MeshY);
        
  //      md.index(is, 2*(MeshX-1)*MeshY + 2*(MeshY-1)*MeshX);
        //       md.texCoord(ts, MeshX*MeshY);
        
        mb.update(md);
        delete vs;
        delete is;
        delete TextuRead;
        
        
        
        
                glViewport(0, 0, width, height);
                glClearColor(1.0,1.0,1.0,1.0);
                
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glLineWidth(2.0);
        
        
        // drawing the mesh
                programMesh.bind(); {
                    glUniformMatrix4fv(programMesh.uniform("mv"), 1, 0, ptr(mv));
                    glUniformMatrix4fv(programMesh.uniform("proj"), 1, 0, ptr(proj));
                    mb.drawLines();
                } programMesh.unbind();
        
        
        
        
        
        
        
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


//int main(int argc, char *argv[]) {
//    
//    MeshFlow().start();
//    
//}
