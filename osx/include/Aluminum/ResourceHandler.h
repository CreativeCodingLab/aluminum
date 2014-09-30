//
//  ResourceHandler.h
//  aluminum
//
//  Created by Angus Forbes on 8/5/13.
//  Copyright (c) 2013 Angus Forbes. All rights reserved.
//

#ifndef __aluminum__ResourceHandler__
#define __aluminum__ResourceHandler__

#import <Aluminum/Includes.hpp>
#include <iostream>


#import <Aluminum/CocoaGL.h>
#import <Aluminum/Includes.hpp>
#import <Aluminum/Texture.hpp>
#import <Aluminum/Program.hpp>

#import <AVFoundation/AVFoundation.h>
#import <AVFoundation/AVAsset.h>


using std::string;
using namespace aluminum;

class ResourceHandler {
  
public:
  
  ResourceHandler();
  
  const char *contentsOfFile(string &file);
  
  string pathToResource(const string &resource);
  string pathToResource(const string &resource, const string &type);
  
  void loadProgram(Program &p, const std::string &name, int pLoc, int nLoc, int tcLoc, int cLoc);
  
  void loadTexture(Texture &t, const std::string &name);
  
  void loadCubeMapTexture(Texture &t, int w, int h, const std::string &negz,
                          const std::string &posz,
                          const std::string &posy,
                          const std::string &negy,
                          const std::string &negx,
                          const std::string &posx);
  
  
  void loadImageData(GLubyte* data, const std::string &name);
};

#endif
