//
//  ObjectPicking.mm
//  aluminum
//
//  Created by Kyle Reese Almryde on 11/4/14.
//  Copyright (c) 2014 Kyle Reese Almryde. All rights reserved.
//

#import <Aluminum/RendererOSX.h>
#import <Aluminum/MeshUtils.hpp>
#import <Aluminum/MeshBuffer.hpp>
#import <Aluminum/MeshData.hpp>
#import <Aluminum/Shapes.hpp>
#import <Aluminum/Program.hpp>
#import <Aluminum/Camera.hpp>
#import <Aluminum/ResourceHandler.h>

using namespace glm;
using namespace aluminum;

class ObjectPicking : public RendererOSX {
public:

    Camera camera;
    Program shader;
    MeshBuffer cubeMB, rayMB;
    ResourceHandler rh;
    
    //modelview and projection matrices
    glm::mat4 VP, RAY = mat4(1);
    
    //box positions
    vec3 box_positions[3]={
        glm::vec3(-1,0.5,0),
        glm::vec3(0,0.5,1),
        glm::vec3(1,0.5,0)
    };
    
    mat4 box_model[3] = {
        mat4(1),
        mat4(1),
        mat4(1)
    };
    vec3 look;
    //flag to enable filtering
    bool useFiltering = true;
    
    int posLoc = 0;
    
    //selected box index
    int selected_box=-1;
    
    float rX = radians(2.0);
    float rY = radians(2.0);
    
    //mouse filtering variables
    const float MOUSE_FILTER_WEIGHT=0.75f;
    const int MOUSE_HISTORY_BUFFER_SIZE = 10;
    
    //mouse history buffer
    vector<vec2> mouseHistory;

    
    virtual void onCreate() {

        rayMB.init(makeRay(10.0), posLoc, -1, -1, -1);
        cubeMB.init(MeshUtils::makeCube(0.5), posLoc, -1, -1, -1);
        
        rh.loadProgram(shader, "cube", posLoc, -1, -1, -1);
        
        //set the camera position
        camera = Camera(radians(60.0), (float)width / (float)height, 0.01, 100.0).translateZ(-5.f);
        
        box_model[0] = glm::translate(mat4(1), box_positions[0]);
        box_model[1] = glm::translate(mat4(1), box_positions[1]);
        box_model[2] = glm::translate(mat4(1), box_positions[2]);

        glViewport(0, 0, width, height);
        
        //enable depth testing
        glEnable(GL_DEPTH_TEST);
        
        cout<<"Initialization successfull"<<endl;
    }
    
    
    virtual void onFrame() {
        handleMouse();
        
        if (camera.isTransformed) {
            printf("Camera is Transformed!\n");
            camera.transform();
        }
        
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        VP = camera.projection * camera.view;
        
        vec3 color;
        
        shader.bind();
        {
            color = (selected_box==0)? vec3(0,1,1) : vec3(1,0,0);
            glUniformMatrix4fv(shader.uniform("MVP"), 1, 0, ptr(VP * box_model[0]));
            glUniform3fv(shader.uniform("vColor"),1, ptr(color));
            cubeMB.draw();


            color = (selected_box==1)?glm::vec3(0,1,1):glm::vec3(0,1,0);
            glUniformMatrix4fv(shader.uniform("MVP"), 1, 0, ptr(VP * box_model[1]));
            glUniform3fv(shader.uniform("vColor"),1, ptr(color));
            cubeMB.draw();
//
            color = (selected_box==2)?glm::vec3(0,1,1):glm::vec3(0,0,1);
            glUniformMatrix4fv(shader.uniform("MVP"), 1, 0, ptr(VP * box_model[2]));
            glUniform3fv(shader.uniform("vColor"),1, ptr(color));
            cubeMB.draw();
            
        }
        shader.unbind();
    }

    
    virtual void onReshape() {
        //set the viewport size
        glViewport (0, 0, (GLsizei) width, (GLsizei) height);
        camera.perspective(radians(60.0), (float)width / (float)height, 0.01, 100.0);
    }
    
    
    
    bool getSelection(int index) {
        float winZ=0;
        //read pixel depth at mouse click position
        glReadPixels( mouseX, mouseY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
        printf("\twinZ1: %f\n", winZ);
        

        float minDist = 100;
        
        selected_box=-1;

        for (int i = 0; i<3; i++) {
            //unproject the obtained winx,winy and winz point to get the object space point
            glm::vec3 objPt = glm::unProject(vec3(mouseX,mouseY,winZ), camera.view*box_model[i], camera.projection, vec4(0,0,width, height));
            cout<< "\tobjPt is " << glm::to_string(objPt) << endl;
            //loop through all scene objects and determine the object clicked by looking at the
            //nearest distance to the object
            float dist = glm::distance(vec3(0.), objPt);
            printf("\tdist: %d\n", dist);
            
            if( selected_box <0 && dist<1 && dist<minDist) {
                selected_box = i;
                minDist = dist;
            }
            
            if(selected_box!=-1) {
                printf("\tPicked box: %d\n\n", selected_box);
                return true;
            } else
                printf("\tMissed box: %d\n\n", i);
        }
        return false;
    }
    
    virtual void keyDown(char key) {
        
        switch(key) {
            case kVK_Space :
                camera.resetVectors();
                box_model[0] = glm::translate(mat4(1), box_positions[0]);
                box_model[1] = glm::translate(mat4(1), box_positions[1]);
                box_model[2] = glm::translate(mat4(1), box_positions[2]);
                break;
            case kVK_ANSI_W :
                camera.translate(vec3(0,0,0.2));
                break;
            case kVK_ANSI_S :
                camera.translate(vec3(0,0,-0.2));
                break;
            case kVK_ANSI_A :
                camera.rotate(vec3(0.0,rY,0.0));
                break;
            case kVK_ANSI_D :
                camera.rotate(vec3(0.0,-rY,0.0));
                break;
            case kVK_ANSI_Q :
                camera.rotate(vec3(rX,0.0,0.0));
                break;
            case kVK_ANSI_E :
                camera.rotate(vec3(-rX,0.0,0.0));
                break;
            case kVK_UpArrow:
                camera.printCameraInfo();
                break;
        }
    }



    
    virtual void handleMouse() {
        if (isPressing)
        {
            printf("\n\nin ObjectPicking: mouseDown %d/%d\n", mouseX, mouseY);
            if (selected_box > -1 && getSelection(0))
                selected_box = -1;
            else
            if (!getSelection(0)) {
                
                shader.bind();
                {
                    glUniformMatrix4fv(shader.uniform("MVP"), 1, 0, ptr(VP * RAY));
                    glUniform3fv(shader.uniform("vColor"),1, ptr(vec3(1.,0.,0.)));
                    rayMB.drawLines();
                    
                }
                shader.unbind();
            }
            shader.bind();
            {
                glUniformMatrix4fv(shader.uniform("MVP"), 1, 0, ptr(VP * RAY));
                glUniform3fv(shader.uniform("vColor"),1, ptr(vec3(1.,0.,0.)));
                rayMB.drawLines();
                
            }
            shader.unbind();

        }
    
        if (isDragging) {}
        
        if (isMoving) {
            float tX, tY = 0.0;
            if (abs(mouseX - previousMouseX) > abs(mouseY - previousMouseY))
                if (mouseX < previousMouseX) tX = -0.02; //movingLeft;
                else tX = 0.02; //movingRight
            else
                if (mouseY < previousMouseY) tY = -0.02; //movingUp
                else tY = 0.02; //movingDown
            
            if (selected_box!=-1) {
                vec3 tXY = glm::unProject(vec3(mouseX,mouseY,0), camera.view, camera.projection, vec4(0,0,width, height));
                box_model[selected_box] = translate(box_model[selected_box], vec3(tX, tY, 0.0));
            }
        }

        
        if (isReleasing) {}
            //printf("in Basic: mouseUp %d/%d\n", mouseX, mouseY);
        
        isDragging = false;
        isMoving = false;
        isPressing = false;
        isReleasing = false;
        
    }
    
};

int main() {
    return ObjectPicking().start("aluminum::ObjectPicking", 100, 100, 400, 400);
}