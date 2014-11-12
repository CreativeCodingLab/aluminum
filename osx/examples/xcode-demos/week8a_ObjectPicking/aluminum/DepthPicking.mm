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
    MeshBuffer cubeMB, mb[3];
    ResourceHandler rh;
    
    //modelview and projection matrices
    glm::mat4 M, VP, fView, pModel = mat4(1);  // View*Projection Matrix and Backup matrix

    //box positions
    vec3 box_positions[3]={
        glm::vec3(-1.5, 0.5, 0.0),
        glm::vec3( 0.0, 0.5, 1.0),
        glm::vec3( 1.5, 0.5, 0.0)
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

    glm::vec3 objPt;
    glm::vec3 localObjPt, worldObjPt;
    
    //flag to enable filtering
    bool useFiltering = true;
    
    int posLoc = 0,
        normalLoc = 1;
    float offsetX, offsetY = 0;
    float unProjX_old, unProjY_old = 0.0;
    float unProjX_new, unProjY_new = 0.0;
    
    
    //selected box index
    int selected_box=-1;
    int which = 0;
    
    float rX = radians(2.0);
    float rY = radians(2.0);
    float tX = 0.0;
    float tY = 0.0;
    float winZ=0;
    
    
    virtual void onCreate() {
        rh.loadProgram(shader, "cube", posLoc, -1, -1, -1);

        cubeMB.init(MeshUtils::makeCube(0.5), posLoc, -1, -1, -1);
        
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
        
        float minDist = 100;
        
        selected_box=-1;

        for (int i = 0; i<3; i++) {
            //unproject the obtained winx,winy and winz point to get the object space point
            objPt = glm::unProject(vec3(mouseX,mouseY,winZ), camera.view*box_model[i], camera.projection, vec4(0,0,width, height));
            cout<< "In getSelection\tobjPt is " << glm::to_string(objPt) << endl;
            //loop through all scene objects and determine the object clicked by looking at the
            //nearest distance to the object
            float dist = glm::distance(vec3(0.), objPt);
            printf("getSelection\tdist: %d\n", dist);
            
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
    
    
    virtual void handleMouse() {
        glReadPixels( mouseX, mouseY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
        objPt = glm::unProject(vec3(mouseX,mouseY,winZ), camera.view*box_model[selected_box], camera.projection, vec4(0,0,width, height));
        glm::vec3 objPt_N = glm::unProject(vec3(mouseX,mouseY,0.), fView*box_model[selected_box], camera.projection, vec4(0,0,width, height));
        glm::vec3 objPt_F = glm::unProject(vec3(mouseX,mouseY,1.), fView*box_model[selected_box], camera.projection, vec4(0,0,width, height));
        vec3 diff = glm::normalize(objPt_F - objPt_N);
        
        if (isPressing)
        {
            printf("\n\nin ObjectPicking: mouseDown %f/%f\n", mouseX - offsetX, mouseY - offsetY);
            // If a cube is selected, and we "pick it" again, deselect the cube and place .. the
            if (selected_box != -1 && getSelection()){
                box_model[selected_box] = glm::translate(mat4(1), vec3(objPt.x,objPt.y,box_positions[selected_box].z));
                selected_box = -1;
                
                // If we pick an object for the first time
            } else if (selected_box < 0 && getSelection()) {
                pModel = glm::translate(mat4(1), vec3(0.,0.,-3.));
            }
        }
        
        if (isMoving && selected_box!= -1) {
            
            //            glReadPixels( mouseX, mouseY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
            ////            objPt = glm::unProject(vec3(mouseX,mouseY,winZ), FLAT*box_model[selected_box], camera.projection, vec4(0,0,width, height));
            //
            //            glm::vec3 objPt_N = glm::unProject(vec3(mouseX,mouseY,0.), fView*box_model[selected_box], camera.projection, vec4(0,0,width, height));
            //            glm::vec3 objPt_F = glm::unProject(vec3(mouseX,mouseY,1.), fView*box_model[selected_box], camera.projection, vec4(0,0,width, height));
            
            //            vec3 diff = glm::normalize(objPt_F - objPt_N);
            
            printf("oX: %f, oY: %f oZ: %f\n",objPt.x, objPt.y, objPt.z);
            printf("dX: %f, dY: %f dZ: %f\n",diff.x*2.f, diff.y*2.f, diff.z);
            printf("mX: %f, mY: %f wZ: %f\n\n",mouseX,mouseY, winZ);
            pModel = glm::translate(fView, vec3(diff.x*2.f, diff.y*2.f,-3.));
            //            box_model[selected_box] = translate(camera.view, vec3(diff.x*2.f, diff.y*2.f, -3.));
            //            box_model[selected_box] = glm::translate(camera.projection, vec3(diff.x*2.f, diff.y*2.f,0.));
            
            
            //            vec3 something = glm::project(diff, camera.view*box_model[selected_box], camera.projection, vec4(0,0,width, height));
            //            cout << glm::to_string(diff) << endl;
            //            cout << glm::to_string(objPt) << endl;
            //            cout << glm::to_string(something) << endl;
            //            point on “screen” * viewport * camera.projection * camera.view*box_model[selected_box]
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

                for (int i = 0; i<3; i++){
                    box_model[i] = glm::translate(mat4(1), box_positions[i]);
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
                which < 1? which++: which--;
                break;
        }
    }
   
};

int main() {
    return ObjectPicking().start("aluminum::ObjectPicking", 100, 100, 400, 400);
}