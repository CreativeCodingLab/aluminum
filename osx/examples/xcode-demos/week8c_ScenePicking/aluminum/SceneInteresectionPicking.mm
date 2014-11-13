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
#include <limits>

using namespace glm;
using namespace aluminum;

class SceneInteresectionPicking : public RendererOSX {
public:
    
    Camera camera;
    Program shader;
    MeshBuffer cubeMB, rayMB, mb[3];
    ResourceHandler rh;
    
    //modelview and projection matrices
    glm::mat4 M, VP, fView, pModel = mat4(1);  // View*Projection Matrix and Backup matrix
    
    //box positions
    vec3 box_positions[3]={
        glm::vec3(-1.5, 0.5, 0.0),
        glm::vec3( 0.0, 0.5, 1.0),
        glm::vec3( 1.5, 0.5, 0.0)
    };
    vec3 obj_positions[3]={
        glm::vec3(-1.5,0.0, 0.0),
        glm::vec3( 0.0,0.0, 1.0),
        glm::vec3( 1.5,0.0, 0.0)
    };
    
    mat4 box_model[3] = {
        mat4(1),
        mat4(1),
        mat4(1)
    };
    
    vec3 box_color[3] = {
        vec3(1.,0.,0.),
        vec3(0.,1.,0.),
        vec3(0.,0.,1.)
    };
    
    glm::vec3 objPt, diff;
    glm::vec3 localObjPt, worldObjPt;
    
    //flag to enable filtering
    bool useFiltering = true;
    
    int posLoc = 0,
    normalLoc = 1;
    
    //selected box index
    int selected_box=-1;
    int which = 0;
    int select = 0;
    
    float rX = radians(2.0);
    float rY = radians(2.0);

    
    //box struct
    struct Box {
        glm::vec3 min, max;
    } boxes[3];
    
    //simple Ray struct
    struct Ray {
        
    public:
        glm::vec3 origin, direction;
        float t;
        
        Ray() {
            t=std::numeric_limits<float>::max();
            origin=glm::vec3(0);
            direction=glm::vec3(0);
        }
    } eyeRay;
    
    //ray Box intersection code
    glm::vec2 intersectBox(const Ray& ray, const Box& cube) {
        glm::vec3 inv_dir = 1.0f/ray.direction;
        glm::vec3   tMin = (cube.min - ray.origin) * inv_dir;
        glm::vec3   tMax = (cube.max - ray.origin) * inv_dir;
        glm::vec3     t1 = glm::min(tMin, tMax);
        glm::vec3     t2 = glm::max(tMin, tMax);
        float tNear = max(max(t1.x, t1.y), t1.z);
        float  tFar = min(min(t2.x, t2.y), t2.z);
        return glm::vec2(tNear, tFar);
    }
    
    
    
    virtual void onCreate() {
        rh.loadProgram(shader, "cube", posLoc, -1, -1, -1);
        
        cubeMB.init(MeshUtils::makeCube(0.5), posLoc, -1, -1, -1);
        rayMB.init(MeshUtils::makeRectangle(0.25, 2.5), posLoc, -1, -1, -1);
        
        MeshData modelMesh[3];
        
        rh.loadObjIntoMesh(modelMesh[0], "dragon.obj", 1.5);
        rh.loadObjIntoMesh(modelMesh[1], "bunny.obj", .5);
        rh.loadObjIntoMesh(modelMesh[2], "teapot.obj", .3);
        
        mb[0].init(modelMesh[0], posLoc, normalLoc, -1, -1);
        mb[1].init(modelMesh[1], posLoc, normalLoc, -1, -1);
        mb[2].init(modelMesh[2], posLoc, normalLoc, -1, -1);
        
        
        //set the camera position
        camera = Camera(radians(60.0), (float)width / (float)height, 0.01, 100.0).translateZ(-5.f);
        
        // Set a "Flat" view
        fView = camera.view;
        
        for (int i = 0; i<3; i++){
            box_model[i] = glm::translate(mat4(1), box_positions[i]);
        }
        
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
        
        vec3 color;
        for(int i = 0; i < 3; i++) {
            shader.bind();
            {
                if (selected_box == i) {
                    M = pModel;
                    VP = camera.projection * fView;
                    color = vec3(0,1,1);
                } else {
                    M = box_model[i];
                    VP = camera.projection * camera.view;
                    color = box_color[i];
                }
                glUniformMatrix4fv(shader.uniform("MVP"), 1, 0, ptr(VP * M));
                glUniform3fv(shader.uniform("vColor"),1, ptr(color));
                which > 0? mb[i].draw() : cubeMB.draw();
            }
            shader.bind();
        }
    }
    
    virtual void onReshape() {
        //set the viewport size
        glViewport (0, 0, (GLsizei) width, (GLsizei) height);
        camera.perspective(radians(60.0), (float)width / (float)height, 0.01, 100.0);
    }
    
    
    bool getSelection() {
        //read pixel depth at mouse click position
        glReadPixels( mouseX, mouseY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
        
        float minDist = 1000;
        
        for (int i = 0; i<3; i++) {
            //unproject the obtained winx,winy and winz point to get the object space point
            localObjPt = glm::unProject(vec3(mouseX,mouseY,winZ), camera.view*box_model[i], camera.projection, vec4(0,0,width, height));
            //loop through all scene objects and determine the object clicked by looking at the
            //nearest distance to the object
            float dist = glm::distance(vec3(0.), localObjPt);
            
            if( selected_box <0 && dist<1 && dist<minDist) {
                selected_box = i;
                minDist = dist;
            }
            
            if(selected_box!=-1) {
                printf("\tPicked box: %d\tdist: %f\n", selected_box, dist);
                return true;
            } else
                printf("\tMissed box: %d\tdist: %f\n", i, dist);
        }
        return false;
    }
    
    
    virtual void handleMouse() {
        glm::project(vec3(mouseX,mouseY,0), fView, camera.projection, vec4(0,0,width, height));
        
        //        Raw screen coordinates projected onto the view plane
        glm::vec3 screenObjPt_N = glm::unProject(vec3(mouseX,mouseY,0.), fView, camera.projection, vec4(0,0,width, height));
        glm::vec3 screenObjPt_F = glm::unProject(vec3(mouseX,mouseY,1.), fView, camera.projection, vec4(0,0,width, height));
        diff = glm::normalize(screenObjPt_F - screenObjPt_N);
        
        //        XXX "World" Coordinates...View matrix/camera space
        //        glm::vec3 worldObjPt_N = glm::unProject(vec3(mouseX,mouseY,0.), camera.view, camera.projection, vec4(0,0,width, height));
        //        glm::vec3 worldObjPt_F = glm::unProject(vec3(mouseX,mouseY,1.), camera.view, camera.projection, vec4(0,0,width, height));
        //        diff = glm::normalize(worldObjPt_F - worldObjPt_N);
        
        //        XXX Local coordinates...Model*View, still follows the camera...
        //        glm::vec3 localObjPt_N = glm::unProject(vec3(mouseX,mouseY,0.), camera.view*box_model[selected_box], camera.projection, vec4(0,0,width, height));
        //        glm::vec3 localObjPt_F = glm::unProject(vec3(mouseX,mouseY,1.), camera.view*box_model[selected_box], camera.projection, vec4(0,0,width, height));
        //        diff = glm::normalize(localObjPt_F - localObjPt_N);
        
        //        glm::vec3 objPt_N = glm::unProject(vec3(mouseX,mouseY,0.), fView*box_model[selected_box], camera.projection, vec4(0,0,width, height));
        //        glm::vec3 objPt_F = glm::unProject(vec3(mouseX,mouseY,1.), fView*box_model[selected_box], camera.projection, vec4(0,0,width, height));
        //        diff = glm::normalize(objPt_F - objPt_N);
        
        
        if (isMoving && selected_box!= -1) {
            
            printf("We're MOVING!!\n");
            pModel = glm::translate(fView, vec3(diff.x*2.5f, diff.y*2.5f,-3.));
        }
        
        if (isPressing)
        {
            // If we pick an object for the first time
            if (selected_box < 0 && getSelection()) {
                printf("\tSELECTED!!\n");
                // This snaps the object to the location of the mouse
                pModel = glm::translate(fView, vec3(diff.x*2.f, diff.y*2.f,-3.));
                //                pModel = glm::translate(fView, vec3(diff.x*2.f, diff.y*2.f,-3.));
                
                // If a cube is selected, and we "pick it" again, deselect the cube and place .. the
            } else if (selected_box != -1 && getSelection()){
                printf("\tDESELECTED!! %d\n",select);
                switch (select) {
                    case 0:
                        box_model[selected_box] = glm::translate(mat4(1), vec3(diff.x*2.5f, diff.y*2.5f,box_positions[selected_box].z));
                        break;
                    case 1:
                        box_model[selected_box] = glm::translate(fView, vec3(diff.x*2.5f, diff.y*2.5f,box_positions[selected_box].z));
                        break;
                    case 2:
                        box_model[selected_box] = glm::translate(fView, vec3(diff.x*2.5f, diff.y*2.5f,box_positions[selected_box].z));
                        break;
                    case 3:
                        box_model[selected_box] = glm::translate(mat4(1), box_positions[selected_box]);
                        break;
                    default:
                        break;
                }
                selected_box = -1;
            }
        }
        
        
        isDragging = false;
        isMoving = false;
        isPressing = false;
        
    }
    
    
    virtual void keyDown(char key) {
        
        switch(key) {
            case kVK_Space :
                camera.resetVectors();
                camera.translateZ(-5.f);
                selected_box = -1;
                
                if(which < 1) {
                    for (int i = 0; i<3; i++){
                        box_model[i] = glm::translate(mat4(1), box_positions[i]);
                    }
                    
                } else{
                    for (int i = 0; i<3; i++){
                        box_model[i] = glm::translate(mat4(1), obj_positions[i]);
                    }
                    
                }
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
                if(which < 1) {
                    which++;
                    for (int i = 0; i<3; i++){
                        box_model[i] = glm::translate(mat4(1), box_positions[i]);
                    }
                    
                } else{
                    which--;
                    for (int i = 0; i<3; i++){
                        box_model[i] = glm::translate(mat4(1), obj_positions[i]);
                    }
                    
                }
                break;
            case kVK_DownArrow:
                camera.printCameraInfo();
                select < 3? select++: select = 0;
                break;
                
        }
    }
    
};

int main() {
    return SceneInteresectionPicking().start("aluminum::ObjectPicking", 100, 100, 400, 400);
}