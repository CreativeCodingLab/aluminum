#include "Aluminum/Includes.hpp"

#include "Aluminum/FreeGlutGLView.hpp"
#include "Aluminum/RendererLinux.hpp"
#include "Aluminum/MeshBuffer.hpp"
#include "Aluminum/MeshData.hpp"
#include "Aluminum/MeshUtils.hpp"
#include "Aluminum/Program.hpp"
#include "Aluminum/Texture.hpp"
#include "Aluminum/Behavior.hpp"
#include "Aluminum/Shapes.hpp"
#include "Aluminum/ResourceHandler.hpp"

#include "Aluminum/FBO.hpp"

/*
 This example is an implementation of Morgan McGuire's Weighted, Blended Order-Independent Transparency method, described on his blog at:
http://casual-effects.blogspot.com/2014/03/weighted-blended-order-independent.html as well in their Journal of Computer Graphics Techniques paper at: http://jcgt.org/published/0002/02/09/
 This example (almost) matches the reference image at: http://1.bp.blogspot.com/-5O6KDtfG9ys/Ux3I49VvArI/AAAAAAAABcM/b1SXEfH0mh0/s1600/2014-03-10_001_blendedOIT2_r101_g3d_r4283.png
*/

using namespace aluminum;

class WBOIT : public RendererLinux {

public:

  //parameters changed by keyboard presses
  float divVal = 250.0;
  float powVal = 8.0;
  bool zDepthOn = false;
  float clearAmt = 0.75;
  float blendAmt = 0.45;
  float colorStrength = 0.95;

  bool autoRotate = false;
  float RX = -10.0;
  float RY = 15.0;
  float RZ = 0.0;

  //internal variables

  FBO fbo_mra;
  Texture t0, t1;

  ResourceHandler rh;
  Program finalProgram, blendPassProgram;
  GLint posLoc=0, texCoordLoc=1;

  MeshBuffer smallRect, clipRect;
  mat4 model, view, proj;

  Behavior rotateBehavior;


  void makeMulitpleRenderTargetFBO() {

    // Make an fbo with two render targets.
    //"The first render target must have at least RGBA16F precision and the second must have at least R8 precision"

    int fw = 1024;
    int fh = 1024;

    t0 = Texture(fw, fh, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    //t0 = Texture(fw, fh, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);

    t1 = Texture(fw, fh, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
    //t1 = Texture(fw, fh, GL_RGBA16F, GL_RGBA, GL_FLOAT);


    std::vector<Texture> ts;
    ts.push_back(t0);
    ts.push_back(t1);

    fbo_mra.create(ts);
  }


  void onCreate() {

    makeMulitpleRenderTargetFBO();

    rh.loadProgram(blendPassProgram, "resources/blendpass", posLoc, -1, -1, -1);
    rh.loadProgram(finalProgram, "resources/final", posLoc, -1, texCoordLoc, -1);

    smallRect.init(MeshUtils::makeRectangle(30.0, 30.0), posLoc, -1, -1, -1);
    clipRect.init(MeshUtils::makeClipRectangle(), posLoc, -1, texCoordLoc, -1);

    proj = glm::perspective(glm::radians(60.0f), float(width)/float(height), 0.1f, 200.0f);
    view = glm::lookAt(vec3(0.0,0.0,1.0), vec3(0,0,0), vec3(0,1,0) );
    model = glm::mat4();

    rotateBehavior = Behavior(now()).delay(100).length(10000).range(vec3(0.0, M_PI * 6, M_PI * 2)).looping(true).repeats(-1);

  }



  void onFrame(){

    vec3 totals;

    if (autoRotate) {
      totals = rotateBehavior.tick(now()).totals();
    } else {
      totals = vec3(glm::radians(RX), glm::radians(RY), glm::radians(RZ));
    }

    mat4 rx = glm::rotate(totals.x, vec3(1.0f,0.0f,0.0f));
    mat4 ry = glm::rotate(totals.y, vec3(0.0f,1.0f,0.0f));
    mat4 rz = glm::rotate(totals.z, vec3(0.0f,0.0f,1.0f));

    mat4 r = rx * ry * rz;


    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(clearAmt,clearAmt,clearAmt,1.0);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    fbo_mra.bind(); {

     // "Clear the first render target to vec4(0) and the second render target to 1 (using a pixel shader or glClearBuffer + glClear)""

      glClearBufferfv(GL_COLOR, 0, ptr(vec4(0.f)));
      glClearBufferfv(GL_COLOR, 1, ptr(vec4(1.f)));


      glEnable(GL_BLEND);
      glBlendFunci(0, GL_ONE, GL_ONE); //blend for color attachment 0
      glBlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA); //blend for color attachment 1

      blendPassProgram.bind(); {
        glUniformMatrix4fv(blendPassProgram.uniform("view"), 1, 0, ptr(view));
        glUniformMatrix4fv(blendPassProgram.uniform("proj"), 1, 0, ptr(proj));

        glUniform1f(blendPassProgram.uniform("divVal"), divVal);
        glUniform1f(blendPassProgram.uniform("powVal"), powVal);
        glUniform1i(blendPassProgram.uniform("zDepthOn"), zDepthOn);


        //1st rect

        glUniform4f(blendPassProgram.uniform("inColor"), colorStrength,0.0,0.0,blendAmt);
        glUniform1f(blendPassProgram.uniform("zval"), -70.0);

        model = glm::translate( vec3(-10.0,-10.0,-70.0) ) * r;

        glUniformMatrix4fv(blendPassProgram.uniform("model"), 1, 0, ptr(model));

        smallRect.draw();



        //2nd rect
        glUniform4f(blendPassProgram.uniform("inColor"), colorStrength, colorStrength,0.0,blendAmt);
        glUniform1f(blendPassProgram.uniform("zval"), -60.0);

        model = glm::translate(vec3(0.0,-5.0,-60.0)) * r;
        glUniformMatrix4fv(blendPassProgram.uniform("model"), 1, 0, ptr(model));

        smallRect.draw();


        //3rd rect
        glUniform4f(blendPassProgram.uniform("inColor"), 0.0f,0.0f,colorStrength,blendAmt);
        glUniform1f(blendPassProgram.uniform("zval"), -50.0);

        model = glm::translate(vec3(10.0,0.0,-50.0)) * r;
        glUniformMatrix4fv(blendPassProgram.uniform("model"), 1, 0, ptr(model));

        smallRect.draw();




      } blendPassProgram.unbind();

    } fbo_mra.unbind();


    //now back to normal render buffer

    glViewport(0, 0, width, height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

    finalProgram.bind(); {

      glUniform1i(finalProgram.uniform("tex0"), 0);
      glUniform1i(finalProgram.uniform("tex1"), 1);

      t0.bind(GL_TEXTURE0);
      t1.bind(GL_TEXTURE1);

      clipRect.draw();

      t0.unbind(GL_TEXTURE0);
      t1.unbind(GL_TEXTURE1);
    } finalProgram.unbind();

  }


  void onReshape() {
    glViewport(0, 0, width, height);
    proj = glm::perspective(glm::radians(60.0f), float(width)/float(height), 0.1f, 200.0f);
  }


  void keyboard(unsigned char key, int x, int y) {

    switch(key) {
      case 'a' :
        divVal += 1.0;
        break;
      case 'z':
        divVal -= 1.0;
        break;
      case 's' :
        powVal += 0.1;
        break;
      case 'x' :
        powVal -= 0.1;
        break;
      case 'd' :
        clearAmt += 0.01;
        break;
      case 'c' :
        clearAmt -= 0.01;
        break;
      case 'f' :
        blendAmt += 0.01;
        break;
      case 'v' :
        blendAmt -= 0.01;
        break;
      case 'g' :
        colorStrength += 0.01;
        break;
      case 'b' :
        colorStrength -= 0.01;
        break;

      case 'r' :
        autoRotate = !autoRotate;
        break;

      case ' ' :
        zDepthOn = !zDepthOn;
        break;


      case 'y' :
        RX += 0.5;
        break;
      case 'h' :
        RX -= 0.5;
        break;
      case 'u' :
        RY += 0.5;
        break;
      case 'j' :
        RY -= 0.5;
        break;
      case 'i' :
        RZ += 0.5;
        break;
      case 'k' :
        RZ -= 0.5;
        break;



    }



    cout << "divVal = " << divVal << "\n";
    cout << "powVal = " << powVal << "\n";
    cout << "zDepthOn = " << zDepthOn << "\n";
    cout << "clearAmt = " << clearAmt << "\n";
    cout << "blendAmt = " << blendAmt << "\n";
    cout << "colorStrength = " << colorStrength << "\n";

    cout << "rx / ry / rz = " << RX << " / " << RY << " / RZ \n";

  }



};
int main() {
	WBOIT().start("WBOIT");
	return 0;
}
