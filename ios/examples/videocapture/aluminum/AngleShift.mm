
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
#import "Utils.hpp"
#import "ResourceHandler.h"
#import "CaptureManager.h"

#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

using namespace aluminum;
using glm::ivec2;
using glm::to_string;



class AngleShift : public RendererIOS {
    
    
    
public:
    ResourceHandler rh;
    
    Program programGrad,programMesh;
    FBO fboGrad;
    MeshBuffer clipRect; // for the first texture for gradient calculation
    MeshData md;
    MeshBuffer mb;
    Texture t1;
    bool bufferReady = false;
    
    static const int div = 6;
    int numPts;
    vec3* vs;
    vec4* cs;
    unsigned int* indices;
    
    
    CaptureManager* cm;
    
    GLint posLoc = 0;
    GLint colLoc=1;
    
    mat4 proj;
    mat4 mv;
    vec2 TouchCords;
    
    mat4 webcamMatrix = mat4();
    mat4 touchMatrix = mat4();
    static const int camWidth = 480;
    static const int camHeight = 360;
  
    float PrevGray [camWidth*camHeight/div];
    float GrayVel [camWidth*camHeight/div];
    float RandAng [camWidth*camHeight/div];
    float PrevOff [camWidth*camHeight/div];
    float OffVel [camWidth*camHeight/div];
    bool touchpresent = false;
    
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
    
    void InitVertex(){
        
        int NumLines = camWidth*camHeight/div;
     
       
        for (int k = 0; k < NumLines; k++) {
            PrevGray [k] = 0;
            GrayVel  [k] = 0;
            RandAng  [k] = Utils::random();
            PrevOff  [k] = 0;
            OffVel   [k] = 0;
        }
        
    }
    
    
    virtual void onCreate() {
        // Load our shader program
        InitVertex();
        loadProgram(programGrad, "basic");

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
        
//        cm = [[CaptureManager alloc] init:AVCaptureSessionPresetHigh side:AVCaptureDevicePositionFront];
        cm = [[CaptureManager alloc] init:AVCaptureSessionPresetMedium side:AVCaptureDevicePositionFront];
        [cm startCapture];
 
        
        
    }
    
    void processFrame() {
        
        if (bufferReady == false) {

            
            
            numPts = ((cm.captureTexture.width * cm.captureTexture.height) / div) * 2;
            vs = new vec3[numPts];
            cs = new vec4[numPts];
            indices = new unsigned int[numPts];
            for (int i = 0; i < numPts; i++) {
                indices[i] = i;
            }
            
            md.vertex(vs, numPts);
            md.color(cs, numPts);
            md.index(indices, numPts);
            
            mb.init(md, posLoc, -1, -1, colLoc);
            bufferReady = true;
        }
        int bw = cm.captureTexture.width;
        int bh = cm.captureTexture.height;
        float sx = -1;
        float sy = -1;
        float xinc = 2.0/(float)bw;
        float yinc = 2.0/(float)bh;
        float xPos =xinc;
        float yPos =yinc;
        int bufferIdx = 0;
        int idx = 0;
        float Ku = 1.85; // update constant
                float Dt = 0.3;
        float Am ; //
        float Ka =1.85;//touch costant
        float Ko = 0.3;
        float Kd = 0.8; // drag

        
        

        for (int i = 0; i < bh; i+=div) {
            for (int j = 0; j < bw; j+=div) {
                
                float r = 0.0;
                float g = 0.0;
                float b = 0.0;
                // averaging in a squared region
//                if (((i+div)<bh)&&((j+div)<bw)) {
//                    
//                    for (int y = i; y < i+div; y++) {
//                        for (int x =j; x < j+div; x++) {
//                            idx = (y * bw * 4) + (x * 4);
//                            r += cm.pixels[idx+2]/255.0;
//                            g += cm.pixels[idx+1]/255.0;
//                            b += cm.pixels[idx+0]/255.0;
//                        }
//                    }
//                    r /= (div*div);
//                    g /= (div*div);
//                    b /= (div*div);
//
//                    
//                }
//                else{
                    idx = (i * bw * 4) + (j * 4);
                    r = cm.pixels[idx+2]/255.0;
                    g = cm.pixels[idx+1]/255.0;
                    b = cm.pixels[idx+0]/255.0;
//                }
                float perc = r*0.299 + g*0.587 + b*0.114;
                
 
                
                
                if(!touchpresent){
                    Ka = 0.0;
                }
                else{
                    float disty = TouchCords.x - (sy + yinc*i);
                    float distx = ((TouchCords.y) - (sx + xinc*j));
                    Am = atan2f(disty,distx)/M_PI;
                    Am = (Am<0)?(Am+1.0):Am;
                    Ku = 0.0;
                }
                
                
                
                // sum of forces
                int IndiVecs = bufferIdx/2;
                
                float Fa = Ku*(perc - PrevGray[IndiVecs]) + Ka*(Am - PrevGray[IndiVecs])- Kd*GrayVel[IndiVecs];
                if(((!touchpresent)&&((perc - PrevGray[IndiVecs])<.1))||
                    ((touchpresent)&&((Am - PrevGray[IndiVecs])<.1))){
                    GrayVel[IndiVecs]*=0.8;
                }

                
                
                float Fb = Ko*abs((perc - PrevGray[IndiVecs])) + (-PrevOff[IndiVecs]) - Kd*OffVel[IndiVecs];

                // Velocities
                GrayVel[IndiVecs]+= Fa*Dt;
                OffVel[IndiVecs]+=Fb*Dt;
                
                // New Gray value
                PrevGray[IndiVecs]+=GrayVel[IndiVecs]*Dt;
                //New offset
                PrevOff[IndiVecs]+= OffVel[IndiVecs]*Dt;
                
                
                float ang = PrevGray[IndiVecs] * (M_PI);
        
                
                float mag = .06;//perc * 0.3;
                
                float xp = cos(ang) * mag;
                float yp = sin(ang) * mag;
             
                
                xPos = xinc*j +  PrevOff[IndiVecs]*cos(2*M_PI*RandAng[IndiVecs]);
                yPos = yinc*i +  PrevOff[IndiVecs]*sin(2*M_PI*RandAng[IndiVecs]);
        
                
                vs[bufferIdx] =   vec3(sx + xPos - xp, -(sy + yPos - yp), 0.0);
                vs[bufferIdx+1] = vec3(sx + xPos + xp, -(sy + yPos + yp), 0.0);
                
                cs[bufferIdx] = vec4(r, g, b, 0.8);
                cs[bufferIdx+1] = vec4(r, g, b, 0.8);
                
                bufferIdx += 2;
                
                
            }
        }
//        if(touchpresent){
//        cout<<(TouchCords.y)<<","<<minY<<endl;
//        cout<<(-TouchCords.x)<<","<<minX<<endl;
//            cout<<themin<<endl;
//            exit(0);}
        
        md.vertices().clear();
        md.vertex(vs, numPts);
        md.colors().clear();
        md.color(cs, numPts);
        
        mb.update(md);
    }
    
    virtual void onFrame(){
        // Clear viewport
        glViewport(0, 0, width, height);
        glClearColor(1.0,1.0,1.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);
           glBindFramebuffer(GL_FRAMEBUFFER, getDefaultFrameBuffer());
        glLineWidth(4.0);
        
     //   [cm updateTextureWithNextFrame];
        
//        if (!cm.textureReady) {
//            return;
//        }
        

        if ([cm nextFrame]) {
            processFrame();
        }
        
        

        
        
        
        
        programGrad.bind(); {
            
            glUniformMatrix4fv(programGrad.uniform("mv"), 1, 0, ptr(mv));
            glUniformMatrix4fv(programGrad.uniform("proj"), 1, 0, ptr(proj));
            mb.drawLines();
        } programGrad.unbind();
        
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
//    AngleShift().start();
//    
//}
