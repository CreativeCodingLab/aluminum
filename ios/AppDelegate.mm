
#import <Aluminum/AppDelegate.h>
#import <Aluminum/EAGLView.h>
#import <Aluminum/ViewController.h>
#import <Aluminum/RendererIOS.h>

@implementation AppDelegate

@synthesize window;
@synthesize glView;
@synthesize viewController;

static void* renderer;

+ (void) setRenderer:(void*)_r {
    renderer = _r;
}

+ (void*) getRenderer {
    return renderer;
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
//- (void) applicationDidFinishLaunching:(UIApplication *)application
{

  printf("didFinishLaunchingWithOptions\n");

    // Create the window programatically:
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];

    self.viewController = [ViewController alloc];
    [self.window setRootViewController:self.viewController];

    self.glView = [self.viewController.glView retain];

    [self.window makeKeyAndVisible];

    return YES;
	//[glView startAnimation];

/*
    [[UIApplication sharedApplication] setStatusBarHidden:YES];

    CGRect screenBounds = [[UIScreen mainScreen] bounds];

    NSLog(@"screenBounds = %@\n", NSStringFromCGRect(screenBounds));

    viewController = [[ViewController alloc] initWithFrame:screenBounds];

    self.window = [[[UIWindow alloc] initWithFrame:screenBounds] autorelease];
    [self.window setRootViewController:viewController];

    //self.window = [[UIWindow alloc] initWithFrame: screenBounds];
    //[self.window addSubview: (UIView*) viewController.view];

    [self.window makeKeyAndVisible];

    printf("out AppDelegate application()");

    return YES;
*/

}

- (void) applicationWillResignActive:(UIApplication *)application {
	[glView stopAnimation];
}

- (void) applicationDidBecomeActive:(UIApplication *)application
{
	[glView startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	[glView stopAnimation];
}

- (void) dealloc
{
	[window release];
	[glView release];

	[super dealloc];
}

@end

