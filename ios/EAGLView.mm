

#import <Aluminum/EAGLView.h>
#import <Aluminum/AppDelegate.h>
#import <Aluminum/RendererIOS.h>

@implementation EAGLView

@synthesize animating;
@dynamic animationFrameInterval;

// You must implement this method
+ (Class) layerClass {
  return [CAEAGLLayer class];
}

- (GLuint) getDefaultFramebuffer {
    return defaultFramebuffer;
}


- (id)initWithFrame:(CGRect)frame {

  printf("in initWithFrame\n");
  if (self = [super initWithFrame:frame])
  {
    firstTime = TRUE;

    //using RendererIOS to store information about what gestures and states should be enabled on the EAGLView
    RendererIOS* r = (RendererIOS*)[AppDelegate getRenderer];

    // Get the layer
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;

    eaglLayer.opaque = TRUE;
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];

	  context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!context || ![EAGLContext setCurrentContext:context] ) //|| ![self loadShaders])
		{
      [self release];
      return nil;
    }

		// Create default framebuffer object. The backing will be allocated for the current layer in -resizeFromLayer
		glGenFramebuffers(1, &defaultFramebuffer);
		glGenRenderbuffers(1, &colorRenderbuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);

		animating = FALSE;
		animationFrameInterval = 1;
		displayLink = nil;

    if (r->multitouchOn) {
      self.multipleTouchEnabled = TRUE;
    } else {
      self.multipleTouchEnabled = FALSE;
    }


    r->installInteractions(self);


    if (r->doubleTapGestureOn) {
      UITapGestureRecognizer *double_tap_recognizer;
      double_tap_recognizer = [[[UITapGestureRecognizer alloc] initWithTarget:self action: @selector(doubleTap:)] autorelease];
      [double_tap_recognizer setNumberOfTouchesRequired : 1];
      [double_tap_recognizer setNumberOfTapsRequired : 2];
      [self addGestureRecognizer : double_tap_recognizer];
    }

    /*
    if (r->pinchGestureOn == true) {
      UIPinchGestureRecognizer *pinch = [[[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(twoFingerPinch:)] autorelease];
      [self addGestureRecognizer:pinch];
    }
    */

    if (r->longPressGestureOn == true) {

    UILongPressGestureRecognizer *press = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(longPress:)];
    press.minimumPressDuration = 1.0;
    [self addGestureRecognizer:press];
    }



    if (r->panGestureOn == true) {

    UIPanGestureRecognizer *panRecognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(panGesture:)];
    [panRecognizer setMinimumNumberOfTouches:1];
    [panRecognizer setMaximumNumberOfTouches:1];
    //[panRecognizer setDelegate:self];
    [self addGestureRecognizer:panRecognizer];
    }

    if (r->swipeUpGestureOn == true) {

      UISwipeGestureRecognizer *swipeRecognizerUp = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeDetectedUp:)];
      swipeRecognizerUp.direction = UISwipeGestureRecognizerDirectionUp;
      swipeRecognizerUp.numberOfTouchesRequired = 1;
      [self addGestureRecognizer:swipeRecognizerUp];
    }

    if (r->swipeDownGestureOn == true) {

      UISwipeGestureRecognizer *swipeRecognizerDown = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeDetectedDown:)];
      swipeRecognizerDown.direction = UISwipeGestureRecognizerDirectionDown;
      swipeRecognizerDown.numberOfTouchesRequired = 1;
    [self addGestureRecognizer:swipeRecognizerDown];
    }

    // UIPanGestureRecognizer *twoFingerPanRecognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(twoFingerPanGesture:)];
     //[twoFingerPanRecognizer setMinimumNumberOfTouches:2];
     //[twoFingerPanRecognizer setMaximumNumberOfTouches:2];
     //[panRecognizer setDelegate:self];
     //[self addGestureRecognizer:twoFingerPanRecognizer];



  }

  return self;
}

- (void) drawView:(id)sender {

  RendererIOS* r = (RendererIOS*)[AppDelegate getRenderer];

  if (firstTime) {
    r->width = backingWidth;
    r->height = backingHeight;
    r->onCreate();
    firstTime = FALSE;
  }

  [EAGLContext setCurrentContext:context];

  glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);

  r->onFrame();
    r->frameCount++;
  //glViewport(0, 0, backingWidth, backingHeight);
  //glClearColor(0.5f, 0.4f, 0.5f, 1.0f);
  //glClear(GL_COLOR_BUFFER_BIT);


  glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
  [context presentRenderbuffer:GL_RENDERBUFFER];
}


- (BOOL) resizeFromLayer:(CAEAGLLayer *)layer {
	// Allocate color buffer backing based on the current layer size
  glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
  [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
  glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
    NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
    return NO;
  }

  return YES;
}


- (void) layoutSubviews {
  [self resizeFromLayer:(CAEAGLLayer*)self.layer];
  [self drawView:nil];
}

- (NSInteger) animationFrameInterval {
	return animationFrameInterval;
}

- (void) setAnimationFrameInterval:(NSInteger)frameInterval {
	// Frame interval defines how many display frames must pass between each time the
	// display link fires. The display link will only fire 30 times a second when the
	// frame internal is two on a display that refreshes 60 times a second. The default
	// frame interval setting of one will fire 60 times a second when the display refreshes
	// at 60 times a second. A frame interval setting of less than one results in undefined
	// behavior.
	if (frameInterval >= 1)
	{
		animationFrameInterval = frameInterval;

		if (animating) {
			[self stopAnimation];
			[self startAnimation];
		}
	}
}

- (void) startAnimation {
	if (!animating) {

    displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(drawView:)];
    [displayLink setFrameInterval:animationFrameInterval];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
  }

  animating = TRUE;

}

- (void)stopAnimation {
	if (animating)
	{
    [displayLink invalidate];
    displayLink = nil;

		animating = FALSE;
	}
}



- (void) touchesBegan: (NSSet*) touches withEvent: (UIEvent*) event {

  for (UITouch* touch in touches) {
    CGPoint location  = [touch locationInView: self];

    RendererIOS* r = (RendererIOS*)[AppDelegate getRenderer];

    r->touchBegan(glm::ivec2(location.x * self.contentScaleFactor, location.y * self.contentScaleFactor));



  }
}

- (void) touchesEnded: (NSSet*) touches withEvent: (UIEvent*) event {
  for (UITouch* touch in touches) {

    CGPoint location  = [touch locationInView: self];
    RendererIOS* r = (RendererIOS*)[AppDelegate getRenderer];

    r->touchEnded(glm::ivec2(location.x * self.contentScaleFactor, location.y * self.contentScaleFactor));
  }
}

- (void) touchesMoved: (NSSet*) touches withEvent: (UIEvent*) event {
  for (UITouch* touch in touches) {
  //  NSLog(@"touch %@\n", touch);

    CGPoint location  = [touch locationInView: self];
    CGPoint prevLocation = [touch previousLocationInView: self];

    RendererIOS* r = (RendererIOS*)[AppDelegate getRenderer];

    r->touchMoved(glm::ivec2(prevLocation.x * self.contentScaleFactor, prevLocation.y * self.contentScaleFactor), glm::ivec2(location.x * self.contentScaleFactor, location.y * self.contentScaleFactor));
  }
}


- (void) swipeDetectedUp:(UITapGestureRecognizer *) recognizer {
  RendererIOS* r = (RendererIOS*)[AppDelegate getRenderer];
  printf("swipeUp!\n");
  r->swipeUp();
}

- (void) doubleTap:(UITapGestureRecognizer *) recognizer {
  RendererIOS* r = (RendererIOS*)[AppDelegate getRenderer];
  printf("doubleTap!\n");
  r->doubleTap();

}

- (void) twoFingerPinch:(UIPinchGestureRecognizer *)recognizer {
  RendererIOS* r = (RendererIOS*)[AppDelegate getRenderer];

  if (recognizer.state == UIGestureRecognizerStateChanged) {
    r->pinch(recognizer.scale);
  } else if (recognizer.state == UIGestureRecognizerStateEnded ||
             recognizer.state == UIGestureRecognizerStateCancelled ||
             recognizer.state == UIGestureRecognizerStateFailed)
  {
    r->pinchEnded();
  }
}

- (void) longPress:(UILongPressGestureRecognizer *)recognizer {
  RendererIOS* r = (RendererIOS*)[AppDelegate getRenderer];

  CGPoint current = [recognizer locationInView: self];
  r->longPress(ivec2(current.x * self.contentScaleFactor, current.y * self.contentScaleFactor));
}



- (void) dealloc {
  // tear down GL
	if (defaultFramebuffer)
	{
		glDeleteFramebuffers(1, &defaultFramebuffer);
		defaultFramebuffer = 0;
	}

	if (colorRenderbuffer)
	{
		glDeleteRenderbuffers(1, &colorRenderbuffer);
		colorRenderbuffer = 0;
	}

  /*
   // realease the shader program object
   if (program)
   {
   glDeleteProgram(program);
   program = 0;
   }
   */
	// tear down context
	if ([EAGLContext currentContext] == context)
    [EAGLContext setCurrentContext:nil];

	[context release];
	context = nil;

	[super dealloc];

  //[renderer release];

  //[super dealloc];
}

@end
