//
//  Interactions.h
//  aluminum
//
//  Created by Angus Forbes on 5/4/14.
//  Copyright (c) 2014 Angus Forbes. All rights reserved.
//
#import <Aluminum/Aluminum.h>
#import <Foundation/Foundation.h>
//#import "EAGLView.h"
//#import "Includes.hpp"
//#import "RendererIOS.h"

@interface Interactions : NSObject

- (id)init:(EAGLView*)ev;

- (void) testTwoFingerPinch:(UIPinchGestureRecognizer *)recognizer;


@end
