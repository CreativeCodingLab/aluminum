/**
 This article was very helpful!!!
 http://www.cocoawithlove.com/2010/09/minimalist-cocoa-programming.html
 **/


#import <Aluminum/Includes.hpp>

//allegedly... test this out...
//you can change the OpenGL graphics context at any time using the setOpenGLContext: method. (ie to make it active stereo... or whatever)

@interface CocoaGL (PrivateMethods)
  - (void)initGL;

  - (void)drawView;


@end

@implementation CocoaGL

  - (BOOL)acceptsFirstResponder {
      return YES;
  }

  - (BOOL)becomeFirstResponder {
      return YES;
  }

  - (BOOL)resignFirstResponder {
      return YES;
  }

  - (void)mouseDragged:(NSEvent *)mouseEvent {
      NSPoint currmouse = [self convertPoint:[mouseEvent locationInWindow] fromView:nil];
      ((RendererOSX *) renderer)->mouseDragged(currmouse.x, currmouse.y);
  }

  - (void)mouseMoved:(NSEvent *)mouseEvent {
      NSPoint currmouse = [self convertPoint:[mouseEvent locationInWindow] fromView:nil];
      ((RendererOSX *) renderer)->mouseMoved(currmouse.x, currmouse.y);
  }

  - (void)mouseDown:(NSEvent *)mouseEvent {
      NSPoint currmouse = [self convertPoint:[mouseEvent locationInWindow] fromView:nil];
      ((RendererOSX *) renderer)->mouseDown(currmouse.x, currmouse.y);
  }

  - (void)mouseUp:(NSEvent *)mouseEvent {
      NSPoint currmouse = [self convertPoint:[mouseEvent locationInWindow] fromView:nil];
      ((RendererOSX *) renderer)->mouseUp(currmouse.x, currmouse.y);
  }

  - (void)keyDown:(NSEvent *)keyDownEvent {
      ((RendererOSX *) renderer)->keyDown([keyDownEvent keyCode]);
  }

  - (void)keyUp:(NSEvent *)keyDownEvent {
      ((RendererOSX *) renderer)->keyUp([keyDownEvent keyCode]);
  }

  - (void)flagsChanged:(NSEvent *)flagsEvent {

      if ([flagsEvent modifierFlags] == 256) {
          ((RendererOSX *) renderer)->keyUp([flagsEvent keyCode]);
      } else {
          ((RendererOSX *) renderer)->keyDown([flagsEvent keyCode]);
      }

  }


  - (CVReturn)getFrameForTime:(const CVTimeStamp *)outputTime {
      // There is no autorelease pool when this method is called
      // because it will be called from a background thread
      // It's important to create one or you will leak objects
      NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

      [self drawView];

      [pool release];
      return kCVReturnSuccess;
  }

// This is the renderer output callback function
  static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp *now, const CVTimeStamp *outputTime, CVOptionFlags flagsIn, CVOptionFlags *flagsOut, void *displayLinkContext) {
      CVReturn result = [(CocoaGL *) displayLinkContext getFrameForTime:outputTime];
      return result;
  }


  - (id)initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat *)format {
      self = [super initWithFrame:frameRect pixelFormat:format];
      return (self);
  }


  - (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication {
      return true;
  }

  - (void)toggleFullScreen {

      [self.window toggleFullScreen:nil];
      CGDisplayShowCursor(kCGDirectMainDisplay);
      cursorOn = true;
  }

  - (void)fullscreen:(id)sender {
      [self toggleFullScreen];
  }

  - (void)cursor:(id)sender {
      if (cursorOn) {
          CGDisplayHideCursor(kCGDirectMainDisplay);
          cursorOn = false;
      } else {
          CGDisplayShowCursor(kCGDirectMainDisplay);
          cursorOn = true;
      }
  }

  - (void)stereo:(id)sender {
      if (stereoOn) {
          printf("turning stereo OFF\n");
          stereoOn = false;
      } else {
          printf("turning stereo ON\n");
          stereoOn = true;
      }
  }

  + (void)setUpMenuBar:(CocoaGL *)glView name:(NSString *)appName {

      id menubar = [[NSMenu new] autorelease];
      id appMenuItem = [[NSMenuItem new] autorelease];
      [menubar addItem:appMenuItem];
      [NSApp setMainMenu:menubar];
      id appMenu = [[NSMenu new] autorelease];

      id quitTitle = [@"Quit " stringByAppendingString:appName];
      id quitMenuItem = [[[NSMenuItem alloc] initWithTitle:quitTitle action:@selector(terminate:) keyEquivalent:@"q"] autorelease];
      [appMenu addItem:quitMenuItem];

      id fullTitle = @"Toggle Fullscreen ";
      id fullMenuItem = [[[NSMenuItem alloc] initWithTitle:fullTitle action:@selector(fullscreen:) keyEquivalent:@"f"] autorelease];
      [fullMenuItem setTarget:glView];
      [appMenu addItem:fullMenuItem];

      id cursorTitle = @"Toggle Cursor ";
      id cursorMenuItem = [[[NSMenuItem alloc] initWithTitle:cursorTitle action:@selector(cursor:) keyEquivalent:@"p"] autorelease];
      [cursorMenuItem setTarget:glView];
      [appMenu addItem:cursorMenuItem];

      id stereoTitle = @"Toggle Stereo ";
      id stereoMenuItem = [[[NSMenuItem alloc] initWithTitle:stereoTitle action:@selector(stereo:) keyEquivalent:@"3"] autorelease];
      [stereoMenuItem setTarget:glView];
      [appMenu addItem:stereoMenuItem];


      //id quitMenuItem = [[[NSMenuItem alloc] initWithTitle:quitTitle action:@selector(testing:) keyEquivalent:[NSString stringWithFormat:@"%c", 0x09] ] autorelease];
      //id quitMenuItem = [[[NSMenuItem alloc] initWithTitle:quitTitle action:@selector(testing:) keyEquivalent:NSKeyCodeLeftArrow ] autorelease];
      //[quitMenuItem setKeyEquivalentModifierMask:0];
      //[quitMenuItem setTarget:glView];

      //[menuItem setKeyEquivalent:@" "];

      [appMenuItem setSubmenu:appMenu];
  }

  + (CocoaGL *)createGLView:(void *)_renderer
                          w:(int)_width
                          h:(int)_height {
      // Set up the OpenGL view with the appropriate attributes
      NSRect glRect = NSMakeRect(0, 0, _width, _height);

      NSOpenGLPixelFormatAttribute attrs[] = {
          NSOpenGLPFADoubleBuffer,
          NSOpenGLPFADepthSize, 32,
          NSOpenGLPFAAlphaSize, 8,
          NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
          //NSOpenGLProfileVersionLegacy,
          //NSOpenGLPFAColorFloat,
          //NSOpenGLPFAStereo, // ... etc there are a lot of interesting ones....
          0
      };

      NSOpenGLPixelFormat *format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
      CocoaGL *glView = [[[CocoaGL alloc] initWithFrame:glRect pixelFormat:format renderer:_renderer] autorelease];
      [format release];

      glView->cursorOn = true;
      glView->stereoOn = false;

      [NSApp setDelegate:glView]; //to handle window closing listener, and potentially other delegate functionality

      return glView;
  }

  + (id)setUpAppWindow:(NSString *)appName
                     x:(int)_xpos
                     y:(int)_ypos
                     w:(int)_width
                     h:(int)_height {

      id window = [[[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, _width, _height)
                                               styleMask:NSTitledWindowMask | NSResizableWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask backing:NSBackingStoreBuffered defer:NO]
          autorelease];

      [window setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary]; //necessary for fullscreen toggling
      [window cascadeTopLeftFromPoint:NSMakePoint(_xpos, _ypos)];

      int menuBarH = 44; //seems odd that we have to hardcode this!
      int yBot = [[[NSScreen screens] objectAtIndex:0] frame].size.height - _height - _ypos - menuBarH; //flip y coord

      [window setFrameOrigin:NSMakePoint(_xpos, yBot)];
      [window setTitle:appName];
      [window makeKeyAndOrderFront:nil];

      [window setAcceptsMouseMovedEvents:YES];

      return window;
  }

  + (CocoaGL *)start:(void *)_renderer
                name:(NSString *)_name
                   x:(int)_xpos
                   y:(int)_ypos
                   w:(int)_width
                   h:(int)_height {

      // Set up a minimal Cocoa window and set its content to be a OpenGL renderer

      [NSApplication sharedApplication];
      [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
      id appName = _name;

      // Set up the window to hold the CocoaGL view
      id window = [CocoaGL setUpAppWindow:_name
                                        x:_xpos
                                        y:_ypos
                                        w:_width
                                        h:_height];

      // Set up a CocoaGL view
      CocoaGL *glView = [CocoaGL createGLView:_renderer
                                            w:_width
                                            h:_height
      ];

      // Add CocoaGL view to window
      [window setContentView:glView];


      // Set up the menu bar
      [CocoaGL setUpMenuBar:glView name:appName];


      // Start the application
      [NSApp activateIgnoringOtherApps:YES]; //brings application to front on startup
      [NSApp run];

      // Return the view (in case we need it from our Renderer class)
      return glView;

  }

  - (id)initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat *)format renderer:(void *)_renderer {

      self = [super initWithFrame:frameRect pixelFormat:format];
      renderer = _renderer;
      return (self);
  }

  - (void)prepareOpenGL {

      [super prepareOpenGL];

      // Make all the OpenGL calls to setup rendering
      //  and build the necessary rendering objects
      [self initGL];

      // Create a display link capable of being used with all active displays
      CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);

      // Set the renderer output callback function
      CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, self);

      // Set the display link for the current renderer
      CGLContextObj cglContext = (_CGLContextObject *) [[self openGLContext] CGLContextObj];
      CGLPixelFormatObj cglPixelFormat = (_CGLPixelFormatObject *) [[self pixelFormat] CGLPixelFormatObj];
      CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);

      // Activate the display link
      CVDisplayLinkStart(displayLink);
  }

  - (void)initGL {
      firstTime = true;
      // Make this openGL context current to the thread
      // (i.e. all openGL on this thread calls will go to this context)
      [[self openGLContext] makeCurrentContext];

      // Synchronize buffer swaps with vertical refresh rate
      GLint swapInt = 1;
      [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];

      // Init our renderer.  Use 0 for the defaultFBO which is appropriate for MacOS (but not iOS)
      //	m_renderer = [[OpenGLRenderer alloc] initWithDefaultFBO:0];
      printf("in initGL\n");

      char *verGL = (char *) glGetString(GL_VERSION);
      printf("GL version = %s\n", verGL);

      char *verGLSL = (char *) glGetString(GL_SHADING_LANGUAGE_VERSION);
      printf("GLSL version = %s\n", verGLSL);


  }


  - (void)reshape {

      [super reshape];

      //printf("in reshape!\n");

      // We draw on a secondary thread through the display link
      // When resizing the view, -reshape is called automatically on the main thread
      // Add a mutex around to avoid the threads accessing the context simultaneously when resizing
      CGLLockContext((_CGLContextObject *) [[self openGLContext] CGLContextObj]);

      //need to pass the size of the bounds to set up glViewport properly
      NSRect rect = [self bounds];
      //NSRect rect = [glView bounds];
      ((RendererOSX *) renderer)->width = (int) rect.size.width;
      ((RendererOSX *) renderer)->height = (int) rect.size.height;

      /*
      NSRect rect = [parentView bounds];
      ((RendererOSX*)renderer)->width =  (int)rect.size.width/2 ;
      ((RendererOSX*)renderer)->height =  (int)rect.size.height ;
      */

      glViewport(0, 0, ((RendererOSX *) renderer)->width, ((RendererOSX *) renderer)->height);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


      if (!firstTime) {
          ((RendererOSX *) renderer)->onReshape();
      }

      CGLUnlockContext((_CGLContextObject *) [[self openGLContext] CGLContextObj]);
  }

  - (void)drawView {
      [[self openGLContext] makeCurrentContext];

      // We draw on a secondary thread through the display link
      // When resizing the view, -reshape is called automatically on the main thread
      // Add a mutex around to avoid the threads accessing the context simultaneously when resizing

      CGLContextObj cglContext = (CGLContextObj) [[self openGLContext] CGLContextObj];
      CGLLockContext(cglContext);
      //CGLLockContext((_CGLContextObject*)[[self openGLContext] CGLContextObj]);

      NSRect rect = [self bounds];

      if (firstTime) {

          printf("in drawView... bounds = %f,%f,%f,%f\n", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);

          ((RendererOSX *) renderer)->view = self;
          ((RendererOSX *) renderer)->width = (int) rect.size.width;
          ((RendererOSX *) renderer)->height = (int) rect.size.height;

          //OSX seems to *require* a default VAO to do anything, even to load shaders... setting a default here (even though each mesh has its own)
          //really need to set a DEFAULT VAO, since unbinding a meshbuffer will nullify
          //   our efforts here... TODO: store _glVaoID as a DefaultVAO and be able to get it whenever
          //   (ie when mesh buffer unbinds, etc)
          //   GLuint _glVaoID;
          //   glGenVertexArrays(1, &_glVaoID );
          //   glBindVertexArray( _glVaoID );

          ((RendererOSX *) renderer)->createDefaultVAO();
          ((RendererOSX *) renderer)->bindDefaultVAO();


          ((RendererOSX *) renderer)->setStartTick();
          ((RendererOSX *) renderer)->onCreate();

          firstTime = false;
      }

      ((RendererOSX *) renderer)->tick();

      //glViewport(0, 0, (int)rect.size.width, (int)rect.size.height);
      //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      ((RendererOSX *) renderer)->onFrame();
      ((RendererOSX *) renderer)->frameCount++;

      //  glFlush();

      // [[self openGLContext] flushBuffer];
      //CGLFlushDrawable((_CGLContextObject*)[[self openGLContext] CGLContextObj]);
      //CGLUnlockContext((_CGLContextObject*)[[self openGLContext] CGLContextObj]);

      CGLFlushDrawable((CGLContextObj) [[self openGLContext] CGLContextObj]);
      CGLUnlockContext((CGLContextObj) [[self openGLContext] CGLContextObj]);


  }

  - (void)printView {
      printf("I am in the view!\n");
  }

  - (void)dealloc {
      // Stop the display link BEFORE releasing anything in the view
      // otherwise the display link thread may call into the view and crash
      // when it encounters something that has been release
      CVDisplayLinkStop(displayLink);

      CVDisplayLinkRelease(displayLink);

      // Release the display link AFTER display link has been released
      //	[m_renderer release];

      [super dealloc];
  }


@end

