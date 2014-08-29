//#import "EAGLView.h"
#import <Aluminum/Includes.hpp>
#import <UIKit/UIKit.h>

#import <AVFoundation/AVFoundation.h>
#import <AVFoundation/AVAsset.h>

#import <Aluminum/Texture.hpp>

using namespace aluminum;

@interface CaptureManager : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate> {

@public
    AVCaptureSession *session;
    AVCaptureDeviceInput *input;
    CVImageBufferRef imageBuffer;
    unsigned char *pixels;

    Texture captureTexture;
    bool newFrame;
    bool firstTime;
    bool textureReady;

  CMVideoDimensions dimensions;
}

@property Texture captureTexture;
@property bool newFrame;
@property bool textureReady;
@property unsigned char *pixels;

- (id) init;
- (id) init:(NSString *)preset side:(AVCaptureDevicePosition)side;
- (void) setupCapture:(NSString *)preset side:(AVCaptureDevicePosition)side;
- (AVCaptureDevice *)frontFacing;
- (AVCaptureDevice *)backFacing;
- (void) startCapture;
- (void) stopCapture;
- (void) toggleCapture;
- (bool) isCapturing;
- (void) setTextureDimensions;
//- (void) processPixelBuffer:(CMSampleBufferRef) sampleBuffer;

- (bool) nextFrame;
- (bool) updateTextureWithNextFrame;

@end
