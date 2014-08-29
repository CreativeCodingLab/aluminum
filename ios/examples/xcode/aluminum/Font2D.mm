#import <Aluminum/Aluminum.h>
//#import "Includes.hpp"
//
//#import "MeshBuffer.hpp"
//#import "MeshData.hpp"
//#import "MeshUtils.hpp"
//#import "Program.hpp"
//#import "Shapes.hpp"
//#import "Texture.hpp"
//#import "Font.hpp"
//
//#import "RendererIOS.h"
//#import "AppDelegate.h"
//#import "ResourceHandler.h"

#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

using namespace aluminum;
using glm::ivec2;
using glm::to_string;

class Font2D : public RendererIOS {
  
  
  
public:
  
  Program program;
  GLuint vao, vbo, ibo, indices[3] = {0,1,2};
  
  Text2D tr, tl, br, bl, center;

  
  
  GLint posLoc = 0;
  GLint colLoc = 1;
  mat4 proj;
  mat4 mv;
  ivec4 VP;
  
  int frameNum = 0;
  
  ResourceHandler rh;
  
  void loadFont(Font& font, const std::string& name) {
    Texture fontTex;
    rh.loadTexture(fontTex, name + ".png");
    fontTex.minFilter(GL_LINEAR);
    fontTex.maxFilter(GL_LINEAR);
    
    
    NSString* filePath = [[NSString alloc] initWithUTF8String:name.c_str()];
    NSString* path = [[NSBundle mainBundle] pathForResource:filePath ofType:@"fnt"];
    NSString* contents = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:nil];
    
    Font::loadFont2(font, fontTex, std::string([contents UTF8String]));
  }

  
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
    VP = ivec4(0, 0, width, height);
    
    Font font;
    
    loadFont(font, "vera_test"); //signed distance font
  
    //exit(0);
    /*
    for (int i = 0; i < NUM_TEXTS; i++) {
      
      Holder* h = new Holder();
      
      h->t = font.signedDistanceText2D("abc").justify(0,0);
      
      h->b = Behavior(now()).delay(500).length(Utils::randomLong(1000,5000)).range(1.0).repeats(-1).sine(Easing::IN).reversing(true);
      
      texts.push_back(h);
    }
    */
    
    vec4 bc = vec4(0.0,1.0,0.0,0.3);
    vec4 fc = vec4(1.0);
    
    tl = font.signedDistanceText2D("top left").background(bc).color(fc);
    tr = font.signedDistanceText2D("top right").background(bc).color(fc);
    br = font.signedDistanceText2D("bottom right").background(bc).color(fc);
    bl = font.signedDistanceText2D("bottom left").background(bc).color(fc);
    center = font.signedDistanceText2D("center").background(bc).color(fc);
    

    
  }
  
  virtual void onFrame(){
    
    glViewport(0, 0, width, height);
    glClearColor(0.0, 0.0, 0.0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    tl.pen(0.0,1.0).height(0.05).screen(VP).justify(-1,1);
    tr.pen(1.0,1.0).height(0.05).screen(VP).justify(1,1);
    br.pen(1.0,0.0).height(0.05).screen(VP).justify(1,-1);
    bl.pen(0.0,0.0).height(0.05).screen(VP).justify(-1,-1);
  //  center.pen(0.5,0.5).height(0.2).screen(VP).justify(0,0);
    center.pixel(768/2,1024/2).width(750).screen(VP).justify(0,0);
   // center.pixel(768/2,1024/2).height(300).screen(VP).justify(0,0);
    
    br.draw();
    bl.draw();
    tl.draw();
    tr.draw();
    center.draw();
    
    
    
  
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

/*
int main(int argc, char *argv[]) {
  
  Font2D().start();
  
}
 */
