#import <Aluminum/Includes.hpp>
#import <Aluminum/VideoPlayer.h>
#import <chrono>
#import <iostream>
#import <Aluminum/Texture.hpp>
//#import "Resourcehandler.h"

using std::cout;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;
using std::chrono::milliseconds;
using std::chrono::high_resolution_clock;

using namespace aluminum;
using std::string;


@implementation VideoPlayer

- (id)init {
    if (self = [super init]) {
        printf("in VideoPlayer constructor\n");
        frameReady = false;
        frameCount=0;
    }

    return self;
}

- (Texture *)createVideoTexture:(const string)filename
                       useAudio:(bool)useAudio
                       autoPlay:(bool)autoPlay
                       autoLoop:(bool)autoLoop {

  //  NSString *path = [NSString stringWithUTF8String:filename.c_str()];

  cout << "filename = " << filename << "\n";

  NSString *basePath = [[[NSString alloc] initWithUTF8String:filename.c_str()] autorelease];

  NSLog(@"%@",basePath);

  NSArray *splits = [basePath componentsSeparatedByString:@"."];

  NSString *fileStr = [splits objectAtIndex:0];
  NSString *typeStr = [splits objectAtIndex:1];

  NSString *path = [[NSBundle mainBundle] pathForResource:fileStr ofType:typeStr];



    videoAsset = [AVAsset assetWithURL:[NSURL fileURLWithPath:path]];

    isLooping = autoLoop;

    videoTexture = [self setUpVideoThread];

    if (autoPlay) {
      isWaiting = true;

        double pauseBeforeStarting = 2.0;
        double atTime = [[NSDate dateWithTimeIntervalSinceNow:pauseBeforeStarting] timeIntervalSinceReferenceDate];

        [self startVideoThread:atTime];

    } else {
      isWaiting = false;
    }

    return videoTexture;
}


- (Texture *)setUpVideoThread {

    isLocked = false;

    [self playVideo];

    CGSize videoSize = videoTrack.naturalSize;
    Texture* t = new Texture(videoSize.width, videoSize.height, GL_RGBA, GL_RGB, GL_UNSIGNED_BYTE);

  return t;
}


/*

  //SET UP AUDIO TRACK
  //if (useAudio) {
  //  PlayAudioResource(fname, atTime);
  //}

  //SET UP VIDEO TRACK
  //videoManager = [[VideoManager alloc] init];
  //[videoManager retain];
  //Texture* videoTexture = [videoManager setUpVideoThread:currentAsset isLooping:autoLoop];



 isLooping = autoLoop;
  isLocked = false;



  [self playVideo];

  CGSize videoSize = videoTrack.naturalSize;
  videoTexture = new Texture(videoSize.width, videoSize.height, GL_RGBA, GL_RGB, GL_UNSIGNED_BYTE);

  //videoTexture = Texture::CreateEmptyTexture(videoSize.width, videoSize.height, GL_BGRA, GL_UNSIGNED_BYTE);
  //printf("VIDEO size = %f %f\n", size.width, size.height);
  //return videoTexture;


  if (autoPlay) {

    printf("autoPlay = true\n");
    //NSLog(@"atTime = %f", atTime);

    //[self testTimer];
    [self startVideoThread:atTime];
  }

  return videoTexture;
}
*/

- (void)onTick:(NSTimer *)timer {
    //lets nextFrame know that there is actually a new frame waiting for it
    frameReady = true;
}

- (void)startVideoThread:(double)fireDate {

    //prevTime = [[NSDate date] timeIntervalSinceReferenceDate];
    //currTime = prevTime;

    NSTimer *t = [[NSTimer alloc] initWithFireDate:[NSDate dateWithTimeIntervalSinceReferenceDate:fireDate]
                                          interval:1.0 / videoTrack.nominalFrameRate
            //interval: 0.001
                                            target:self
                                          selector:@selector(onTick:)
                                          userInfo:nil repeats:YES];


    [[NSRunLoop mainRunLoop] addTimer:t forMode:NSDefaultRunLoopMode];
    [t release];

    //uncomment this if you want to show the first frame right away
    frameReady = true;
    [self nextFrame];
}


- (void)playVideo {

    NSError *error = nil;
    videoAssetReader = [AVAssetReader assetReaderWithAsset:videoAsset error:&error];
    [videoAssetReader retain];
    if (error) {
        NSLog(@"%@", [error localizedDescription]);
    }

    NSArray *tracksArray = [videoAsset tracksWithMediaType:AVMediaTypeVideo];
    videoTrack = [tracksArray objectAtIndex:0];
    videoTrackOutput = [AVAssetReaderTrackOutput assetReaderTrackOutputWithTrack:videoTrack outputSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] forKey:(id) kCVPixelBufferPixelFormatTypeKey]];

    [videoAssetReader addOutput:videoTrackOutput];
    [videoAssetReader startReading];

}


//return true if the frame was updated, false if the next frame wasn't ready to be updated yet
- (bool)nextFrame {

    if ([videoAssetReader status] == AVAssetReaderStatusReading) {

        if (isWaiting == true && frameReady == false) { //if autoPlay != true, then we can just pull frames when we want
           // printf("frame not ready\n");
            return false;
        }
        //printf("frame ready\n");
        isLocked = true;
        frameCount++;

        videoTexture->bind(GL_TEXTURE0);
        {
            pixelBuffer = [videoTrackOutput copyNextSampleBuffer];
            imageBuffer = CMSampleBufferGetImageBuffer(pixelBuffer);
            CVPixelBufferLockBaseAddress(imageBuffer, 0);
            unsigned char *linebase = (unsigned char *) CVPixelBufferGetBaseAddress(imageBuffer);
            CMFormatDescriptionRef formatDesc = CMSampleBufferGetFormatDescription(pixelBuffer);
            CMVideoDimensions videoDimensions = CMVideoFormatDescriptionGetDimensions(formatDesc);

            //slow to do it here... rather just pass a flag to the shaders
            //   Texture::flipBufferY(linebase, videoDimensions.width, videoDimensions.height);
            //   Texture::flipBufferX(linebase, videoDimensions.width, videoDimensions.height);

            videoTexture->data = linebase;
            glTexSubImage2D(videoTexture->kind(), 0, 0, 0, videoDimensions.width, videoDimensions.height, GL_BGRA, GL_UNSIGNED_BYTE, linebase);

            CVPixelBufferUnlockBaseAddress(imageBuffer, 0);
            CVPixelBufferRelease(imageBuffer);

            free(pixelBuffer);
        }
        videoTexture->unbind(GL_TEXTURE0);

        isLocked = false;
        frameReady = false;
    } else {
        [videoAssetReader cancelReading];
        frameReady = false;
        frameCount=0;

        if (isLooping) {
            [self playVideo];
        }
    }

    return true;

}


@end
