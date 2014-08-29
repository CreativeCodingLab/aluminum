//
//  Interactions.m
//  aluminum
//
//  Created by Angus Forbes on 5/4/14.
//  Copyright (c) 2014 Angus Forbes. All rights reserved.
//
#import <Aluminum/Aluminum.h>
#import "Interactions.h"
#import "Basic.mm"

@implementation Interactions



- (id)init:(EAGLView*)ev {
  
  printf("in init interactions...\n");
  if (self = [super init]) {
   
    UIPinchGestureRecognizer *pinch2 = [[[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(testTwoFingerPinch:)] autorelease];
    [ev addGestureRecognizer:pinch2];

    
  }
  
  return self;
}


- (void) testTwoFingerPinch:(UIPinchGestureRecognizer *)recognizer {
  
  printf("TEST PINCH!!!\n");
  
  Basic* r = (Basic*)[AppDelegate getRenderer];
  
  if (recognizer.state == UIGestureRecognizerStateChanged) {
    r->pinch2(recognizer.scale);
  } else if (recognizer.state == UIGestureRecognizerStateEnded ||
             recognizer.state == UIGestureRecognizerStateCancelled ||
             recognizer.state == UIGestureRecognizerStateFailed)
  {
      r->pinchEnded();
  }
}


@end
