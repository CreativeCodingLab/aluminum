
#import <UIKit/UIKit.h>

@class EAGLView, ViewController;

@interface AppDelegate : UIResponder <UIApplicationDelegate> {
    UIWindow *window;
    EAGLView *glView;
    ViewController *viewController;
}

//@property (nonatomic, retain) IBOutlet UIWindow *window;
//@property (nonatomic, retain) IBOutlet EAGLView *glView;
@property (nonatomic, retain)  UIWindow *window;
@property (nonatomic, retain)  EAGLView *glView;
@property (nonatomic, retain) ViewController *viewController;

+ (void) setRenderer:(void*)_r;
+ (void*) getRenderer;


@end

