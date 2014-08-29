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


#import <Aluminum/EAGLView.h>
#import <Aluminum/Includes.hpp>
#import <Aluminum/Texture.hpp>

#import <AVFoundation/AVFoundation.h>
#import <AVFoundation/AVAsset.h>


//using namespace aluminum;
using std::string;
//using aluminum::Texture;

//class aluminum::Texture;


using namespace aluminum;

//class Texture;


class ResourceHandler {

public:

  ResourceHandler();

  const char* contentsOfFile(string& file);
  string pathToResource(const string& resource, const string& type);
  void loadTexture(Texture& t, const std::string& name);
};

#endif /* defined(__aluminum__ResourceHandler__) */
