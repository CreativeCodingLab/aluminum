#include "Aluminum/Includes.hpp"

#include "Aluminum/RendererOSX.h"
#include "Aluminum/MeshBuffer.hpp"
#include "Aluminum/MeshData.hpp"
#include "Aluminum/MeshUtils.hpp"
#include "Aluminum/Program.hpp"
#include "Aluminum/Texture.hpp"
#include "Aluminum/Behavior.hpp"
#include "Aluminum/Shapes.hpp"
#include "Aluminum/ResourceHandler.h"

#include "Aluminum/FBO.hpp"

/* 
 This example is an implementation of Morgan McGuire's Weighted, Blended Order-Independent Transparency method, described on his blog at:
http://casual-effects.blogspot.com/2014/03/weighted-blended-order-independent.html as well in their Journal of Computer Graphics Techniques paper at: http://jcgt.org/published/0002/02/09/
*/

using namespace aluminum;

class WBOIT : public RendererOSX {
  
public:
  
  
  float divVal = 110.0;
  float powVal = 7.0;
  bool zDepthOn = false;
  float clearAmt = 0.9f;
  float blendAmt = 0.5f;
  float colorStrength = 1.0f;
  
  
  
  Texture t0, t1;
  FBO fbo_mra;
  
  mat4 model, view, proj;
  
  Program finalProgram, blendPassProgram, textureProgram, phongProgram, phongBlendPassProgram;
  
  GLint posLoc=0;
  GLint texCoordLoc=1;
  GLint normalLoc=2;
  
  MeshBuffer mb1, fullScreen, sphereBuffer;
  ResourceHandler rh;
  
  Behavior rotateBehavior;
  
  
  void makeMulitpleRenderTargetFBO() {
    
    // Make an fbo with two render targets.
    //"The first render target must have at least RGBA16F precision and the second must have at least R8 precision"
    
    int fw = 1024;
    int fh = 1024;
    
    t0 = Texture(fw, fh, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    //t0 = Texture(fw, fh, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
    
    t1 = Texture(fw, fh, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    //t1 = Texture(fw, fh, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
    
    std::vector<Texture> ts;
    ts.push_back(t0);
    ts.push_back(t1);
    
    fbo_mra.create(ts);
  }
  
  
  void onCreate() {
    
    
    makeMulitpleRenderTargetFBO();
    
    
     
    rh.loadProgram(blendPassProgram, "blendpass", posLoc, -1, -1, -1);
    rh.loadProgram(phongBlendPassProgram, "phongOIB", posLoc, normalLoc, -1, -1);
    
    rh.loadProgram(textureProgram, "texture", posLoc, -1, texCoordLoc, -1);
    rh.loadProgram(finalProgram, "final", posLoc, -1, texCoordLoc, -1);
    rh.loadProgram(phongProgram, "phong", posLoc, normalLoc, -1, -1);
    
    mb1.init(MeshUtils::makeRectangle(30.0, 30.0), posLoc, normalLoc, -1, -1);
    fullScreen.init(MeshUtils::makeClipRectangle(2.0, 2.0), posLoc, -1, texCoordLoc, -1);
    
    //need to deal with the fact that the init method needs to re-bind to the default VAO...
    //...or the rh.loadProgram needs to do that
    
    proj = glm::perspective(glm::radians(60.0f), float(width)/float(height), 0.1f, 200.0f);
    view = glm::lookAt(vec3(0.0,0.0,3), vec3(0,0,0), vec3(0,1,0) );
    model = glm::mat4();
    
    
    MeshData mesh1;
    addSphere(mesh1, 10.0, 100, 100);
    sphereBuffer.init(mesh1, posLoc, normalLoc, -1, -1);

   
    
    rotateBehavior = Behavior(now()).delay(100).length(10000).range(vec3(M_PI * 4, M_PI * 2, M_PI * 6)).looping(true).repeats(-1);
    
  }
  
  void drawSphere(mat4 sphereModel) {
    phongProgram.bind(); {
      glUniformMatrix4fv(phongProgram.uniform("view"), 1, 0, ptr(view));
      glUniformMatrix4fv(phongProgram.uniform("proj"), 1, 0, ptr(proj));
      
      glUniform3fv(phongProgram.uniform("ambient"), 1, ptr(vec3(0.1,0.1,0.1)));
      
      glUniform4fv(phongProgram.uniform("l1_position"), 1, ptr(vec4(0.0,100.0,0.0,1.0)));
      glUniform3fv(phongProgram.uniform("l1_diffuse"), 1, ptr(vec3(1.0,0.0,0.0)));
      glUniform3fv(phongProgram.uniform("l1_specular"), 1, ptr(vec3(1.0,1.0,1.0)));
      
      glUniform4fv(phongProgram.uniform("l2_position"), 1, ptr(vec4(0.0,-100.0,0.0,1.0)));
      glUniform3fv(phongProgram.uniform("l2_diffuse"), 1, ptr(vec3(0.0,1.0,0.0)));
      glUniform3fv(phongProgram.uniform("l2_specular"), 1, ptr(vec3(1.0,1.0,1.0)));
      
      glUniformMatrix4fv(phongProgram.uniform("model"), 1, 0, ptr(sphereModel));
      sphereBuffer.draw();
      
    } phongProgram.unbind();
    
    
  }
  
  void drawTransparentObject(mat4 sphereModel, MeshBuffer& mbo) {
   //glEnable(GL_CULL_FACE);
   // glCullFace(GL_BACK);
    glDepthMask(GL_TRUE);
    phongBlendPassProgram.bind(); {
      
      glUniformMatrix4fv(phongBlendPassProgram.uniform("view"), 1, 0, ptr(view));
      glUniformMatrix4fv(phongBlendPassProgram.uniform("proj"), 1, 0, ptr(proj));
      glUniformMatrix4fv(phongBlendPassProgram.uniform("model"), 1, 0, ptr(sphereModel));
      
      //phong stuff
      glUniform3fv(phongBlendPassProgram.uniform("ambient"), 1, ptr(vec3(0.1,0.1,0.1)));
      
      glUniform4fv(phongBlendPassProgram.uniform("l1_position"), 1, ptr(vec4(0.0,100.0,0.0,1.0)));
      glUniform3fv(phongBlendPassProgram.uniform("l1_diffuse"), 1, ptr(vec3(1.0,0.0,0.0)));
      glUniform3fv(phongBlendPassProgram.uniform("l1_specular"), 1, ptr(vec3(1.0,1.0,1.0)));
      
      glUniform4fv(phongBlendPassProgram.uniform("l2_position"), 1, ptr(vec4(0.0,-100.0,0.0,1.0)));
      glUniform3fv(phongBlendPassProgram.uniform("l2_diffuse"), 1, ptr(vec3(0.0,1.0,0.0)));
      glUniform3fv(phongBlendPassProgram.uniform("l2_specular"), 1, ptr(vec3(1.0,1.0,1.0)));
    
      //blendPass stuff
      

      glUniform1f(phongBlendPassProgram.uniform("divVal"), divVal);
      glUniform1f(phongBlendPassProgram.uniform("powVal"), powVal);
      glUniform1i(phongBlendPassProgram.uniform("zDepthOn"), zDepthOn);
      glUniform1f(phongBlendPassProgram.uniform("zval"), -60.0);
      
      
      glEnable(GL_BLEND);
      
      mbo.draw();
    } phongBlendPassProgram.unbind();
    
    glDisable(GL_CULL_FACE);
    
  }
  
  
  void test() {
    
    vec3 totals = rotateBehavior.tick(now()).totals();
    mat4 t, r;
    
   
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(clearAmt,clearAmt,clearAmt,1.0);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    fbo_mra.bind(); {
   
    
     // glDepthMask(GL_FALSE);
      
     // "Clear the first render target to vec4(0) and the second render target to 1 (using a pixel shader or glClearBuffer + glClear)""
      
      vec4 clearColor0 = vec4(0.f);
      glClearBufferfv(GL_COLOR, 0, ptr(clearColor0));
      //ivec4 clearColor0 = ivec4(0);
      //glClearBufferiv(GL_COLOR, 0, ptr(clearColor0));
      
      
      vec4 clearColor1 = vec4(1.f);
      glClearBufferfv(GL_COLOR, 1, ptr(clearColor1));
      //ivec4 clearColor1 = ivec4(255);
      //glClearBufferiv(GL_COLOR, 1, ptr(clearColor1));
      
      
      glEnable(GL_BLEND);
      glBlendFunci(0, GL_ONE, GL_ONE);
      glBlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
  
      blendPassProgram.bind(); {
        glUniformMatrix4fv(blendPassProgram.uniform("view"), 1, 0, ptr(view));
        glUniformMatrix4fv(blendPassProgram.uniform("proj"), 1, 0, ptr(proj));
        
        glUniform1f(blendPassProgram.uniform("divVal"), divVal);
        glUniform1f(blendPassProgram.uniform("powVal"), powVal);
        glUniform1i(blendPassProgram.uniform("zDepthOn"), zDepthOn);
        
        
        //1st rect
        glUniform4f(blendPassProgram.uniform("inColor"), colorStrength,colorStrength,0.0,blendAmt);
        glUniform1f(blendPassProgram.uniform("zval"), -60.0);
        
        t = glm::translate(vec3(00.0,0.0,-60.0));
        r = glm::rotate(totals.z, vec3(0.0f,1.0f,0.0f));
        glUniformMatrix4fv(blendPassProgram.uniform("model"), 1, 0, ptr(t * r));
        
        mb1.draw();
        
        
        //2nd rect
        glUniform4f(blendPassProgram.uniform("inColor"), 0.0f,0.0f,colorStrength,blendAmt);
        glUniform1f(blendPassProgram.uniform("zval"), -50.0);
        
        t = glm::translate(vec3(10.0,0.0,-50.0));
        r = glm::rotate(totals.z, vec3(0.0f,1.0f,0.0f));
        glUniformMatrix4fv(blendPassProgram.uniform("model"), 1, 0, ptr(t * r));
        
        mb1.draw();
        
        
        //3rd rect
        
        glUniform4f(blendPassProgram.uniform("inColor"), colorStrength,0.0,0.0,blendAmt);
        glUniform1f(blendPassProgram.uniform("zval"), -70.0);
       
        t = glm::translate(vec3(-10.0,0.0,-70.0));
        r = glm::rotate(totals.z, vec3(0.0f,1.0f,0.0f));
        glUniformMatrix4fv(blendPassProgram.uniform("model"), 1, 0, ptr(t * r));
        
        
        glEnable(GL_BLEND);
        
        mb1.draw();
        
        
      } blendPassProgram.unbind();
      
    }    fbo_mra.unbind();
  //  } glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
      
    glViewport(0, 0, width, height);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
    
    finalProgram.bind(); {
      model = mat4();
      
      glUniformMatrix4fv(finalProgram.uniform("model"), 1, 0, ptr(model));
      glUniformMatrix4fv(finalProgram.uniform("view"), 1, 0, ptr(view));
      glUniformMatrix4fv(finalProgram.uniform("proj"), 1, 0, ptr(proj));
      
      glUniform1i(finalProgram.uniform("tex0"), 0);
      glUniform1i(finalProgram.uniform("tex1"), 1);
      
      t0.bind(GL_TEXTURE0);
      t1.bind(GL_TEXTURE1);
      
      fullScreen.draw();
      
      t0.unbind(GL_TEXTURE0);
      t1.unbind(GL_TEXTURE1);
    } finalProgram.unbind();
    

    
  }
  
  void onFrame(){
   
    test();
    if (1 == 1) {return;}
    
    mat4 sphereModel;
    vec3 totals = rotateBehavior.tick(now()).totals();
   
    
    //draw opaque surfaces
  
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(clearAmt,clearAmt,clearAmt,1.0);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
  /*
    glDisable(GL_DEPTH_TEST);
   // glDepthMask(GL_FALSE);
    glDisable(GL_BLEND);
    
    sphereModel = mat4();
    sphereModel = glm::translate(sphereModel, vec3(-25.0,0.0,-60.0));
    drawSphere(sphereModel);
    */
    
    
    //draw trasparent objects
    
    
    
    //bind framebuffer object and call shader that writes to two different shaders
    fbo_mra.bind();
    
   // glEnablei(GL_BLEND, GL_COLOR_ATTACHMENT0);
   // glEnablei(GL_BLEND, GL_COLOR_ATTACHMENT1);
    
  //  glEnable(GL_DEPTH_TEST);
     glDepthMask(GL_FALSE);
    
    //clear accum text to vec4(0);
    //clear reveal text to float(1);
    
    
    vec4 clearColor0 = vec4(0.f);
    glClearBufferfv(GL_COLOR, 0, ptr(clearColor0));
    //ivec4 clearColor0 = ivec4(0);
    //glClearBufferiv(GL_COLOR, 0, ptr(clearColor0));
    
    
    vec4 clearColor1 = vec4(1.f);
    glClearBufferfv(GL_COLOR, 1, ptr(clearColor1));
    //ivec4 clearColor1 = ivec4(255);
    //glClearBufferiv(GL_COLOR, 1, ptr(clearColor1));
    
    
    glEnable(GL_BLEND);
    glBlendFunci(0, GL_ONE, GL_ONE);
    glBlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
    
    
    /*
    sphereModel = mat4();
    sphereModel = glm::translate(sphereModel, vec3(0.0,0.0,-80.0));
    
   // drawTransparentObject(sphereModel,sphereBuffer);
    
    sphereModel = mat4();
    sphereModel = glm::translate(sphereModel, vec3(-7.0,0.0,-50.0));
    
   // drawTransparentObject(sphereModel,sphereBuffer);
  
  
    model = mat4();
    model = glm::translate(model, vec3(-10.0,0.0,-50.0));
    model = glm::rotate(model, totals.z, vec3(0.0f,1.0f,0.0f));
    
    drawTransparentObject(model,mb1);
    
    model = mat4();
    model = glm::translate(model, vec3(0.0,0.0,-60.0));
    model = glm::rotate(model, totals.z, vec3(0.0f,1.0f,0.0f));

    drawTransparentObject(model,mb1);
   
    model = mat4();
    model = glm::translate(model, vec3(10.0,0.0,-70.0));
    model = glm::rotate(model, totals.z, vec3(0.0f,1.0f,0.0f));
    
    drawTransparentObject(model,mb1);
    */
    
    blendPassProgram.bind(); {
      glUniformMatrix4fv(blendPassProgram.uniform("view"), 1, 0, ptr(view));
      glUniformMatrix4fv(blendPassProgram.uniform("proj"), 1, 0, ptr(proj));
      
      glUniform1f(blendPassProgram.uniform("divVal"), divVal);
      glUniform1f(blendPassProgram.uniform("powVal"), powVal);
      glUniform1i(blendPassProgram.uniform("zDepthOn"), zDepthOn);
      
      
      glUniform4f(blendPassProgram.uniform("inColor"), colorStrength,colorStrength,0.0,blendAmt);
      glUniform1f(blendPassProgram.uniform("zval"), -60.0);
      
      model = mat4();
      model = glm::translate(model, vec3(0.0,0.0,-60.0));
      model = glm::rotate(model, totals.z, vec3(0.0f,1.0f,0.0f));
      
      glUniformMatrix4fv(blendPassProgram.uniform("model"), 1, 0, ptr(model));
      
      glEnable(GL_BLEND);
      
      mb1.draw();
      

      
      
      
      
      glUniform4f(blendPassProgram.uniform("inColor"), 0.0f,0.0f,colorStrength,blendAmt);
      glUniform1f(blendPassProgram.uniform("zval"), -50.0);
      
      model = mat4();
      model = glm::translate(model, vec3(10.0,0.0,-50.0));
      model = glm::rotate(model, totals.z , vec3(0.0f,1.0f,0.0f));
      
      glUniformMatrix4fv(blendPassProgram.uniform("model"), 1, 0, ptr(model));
     
      glEnable(GL_BLEND);
      
      mb1.draw();
    
      
      
      
      glUniform4f(blendPassProgram.uniform("inColor"), colorStrength,0.0,0.0,blendAmt);
      glUniform1f(blendPassProgram.uniform("zval"), -70.0);
      
      model = mat4();
      model = glm::translate(model, vec3(-10.0,0.0,-70.0));
      model = glm::rotate(model, totals.z, vec3(0.0f,1.0f,0.0f));
   
      glUniformMatrix4fv(blendPassProgram.uniform("model"), 1, 0, ptr(model));
     
      glEnable(GL_BLEND);
      
      mb1.draw();
      
      
    }
    
     
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //
  //  bindDefaultVAO();
    glViewport(0, 0, width, height);
    
   // glEnable(GL_DEPTH_TEST);
    
  //  glDepthMask(GL_TRUE);
    
 //   glEnable(GL_BLEND);
 //  glBlendFunc(GL_ONE, GL_ONE);
    
    
    
  
   // glDepthMask(GL_TRUE);
    
   
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
   
  

    
//    textureProgram.bind(); {
//      model = mat4();
//      
//      glUniformMatrix4fv(textureProgram.uniform("model"), 1, 0, ptr(model));
//      glUniformMatrix4fv(textureProgram.uniform("view"), 1, 0, ptr(view));
//      glUniformMatrix4fv(textureProgram.uniform("proj"), 1, 0, ptr(proj));
//      
//      glUniform1i(textureProgram.uniform("tex0"), 0);
//      
//      t0.bind(GL_TEXTURE0); {
//        fullScreen.draw();
//      } t0.unbind(GL_TEXTURE0);
//      
//    } textureProgram.unbind();
    
    
    
    
    finalProgram.bind(); {
      model = mat4();
      
      glUniformMatrix4fv(finalProgram.uniform("model"), 1, 0, ptr(model));
      glUniformMatrix4fv(finalProgram.uniform("view"), 1, 0, ptr(view));
      glUniformMatrix4fv(finalProgram.uniform("proj"), 1, 0, ptr(proj));
      
      glUniform1i(finalProgram.uniform("tex0"), 0);
      glUniform1i(finalProgram.uniform("tex1"), 1);
      
      t0.bind(GL_TEXTURE0);
      t1.bind(GL_TEXTURE1);
      
      fullScreen.draw();
      
      t0.unbind(GL_TEXTURE0);
      t1.unbind(GL_TEXTURE1);
    } finalProgram.unbind();
    
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    
    sphereModel = mat4();
    sphereModel = glm::translate(sphereModel, vec3(15.0,0.0,-30.0));
    //drawSphere(sphereModel);
    

   
    
  }
  
  
  void onReshape() {
    glViewport(0, 0, width, height);
    proj = glm::perspective(glm::radians(60.0f), float(width)/float(height), 0.1f, 100.0f);
    
  }
  
  
  virtual void keyDown(char key) {
    
    switch(key) {
      case kVK_ANSI_A :
        divVal += 1.0;
        break;
      case kVK_ANSI_Z :
        divVal -= 1.0;
        break;
      case kVK_ANSI_S :
        powVal += 0.1;
        break;
      case kVK_ANSI_X :
        powVal -= 0.1;
        break;
      case kVK_ANSI_D :
        clearAmt += 0.01;
        break;
      case kVK_ANSI_C :
        clearAmt -= 0.01;
        break;
      case kVK_ANSI_F :
        blendAmt += 0.01;
        break;
      case kVK_ANSI_V :
        blendAmt -= 0.01;
        break;
      case kVK_ANSI_G :
        colorStrength += 0.01;
        break;
      case kVK_ANSI_B :
        colorStrength -= 0.01;
        break;
        
      case kVK_Space :
        zDepthOn = !zDepthOn;
        break;
        
    }
    
   
    
    cout << "divVal = " << divVal << "\n";
    cout << "powVal = " << powVal << "\n";
    cout << "zDepthOn = " << zDepthOn << "\n";
    cout << "clearAmt = " << clearAmt << "\n";
    cout << "blendAmt = " << blendAmt << "\n";
    cout << "colorStrength = " << colorStrength << "\n";
    
  }

  void mouseMoved(int px, int py) {
  }
  
};


int main() {
  return WBOIT().start("aluminum::WBOIT", 100, 100, 400, 300);
}
