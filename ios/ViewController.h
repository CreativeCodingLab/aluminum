

#import <UIKit/UIKit.h>
#import <Aluminum/EAGLView.h>

//class EAGLView;

@interface ViewController : UIViewController {
    struct CGRectscreenBounds;
    EAGLView *glView;
}

@property(nonatomic, retain) EAGLView *glView;

//- (id)initWithFrame:(CGRect)_screenBounds;

@end
