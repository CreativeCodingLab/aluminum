#import <Carbon/Carbon.h> //for keyboard keycode info
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>


@interface CocoaGL : NSOpenGLView {
    CVDisplayLinkRef displayLink;
    void *renderer;
    BOOL cursorOn;
    BOOL stereoOn;
    bool firstTime;

}

//this one creates the window, makes the glView take up the entire window, adds menu bar, and starts the app as well
+ (CocoaGL *)start:(void *)_renderer
              name:(NSString *)_name
                 x:(int)_xpos
                 y:(int)_ypos
                 w:(int)_width
                 h:(int)_height;

//this one just creates a GLView, expects you to make the window, etc (in case you want to add in other views)
+ (CocoaGL *)createGLView:(void *)_renderer
                        w:(int)_width
                        h:(int)_height;

+ (id)setUpAppWindow:(NSString *)appName
                   x:(int)_xpos
                   y:(int)_ypos
                   w:(int)_width
                   h:(int)_height;

+ (void)setUpMenuBar:(CocoaGL *)glView name:(NSString *)appName;

- (id)initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat *)format renderer:(void *)_renderer;

- (void)printView;

- (void)toggleFullScreen;

@end


