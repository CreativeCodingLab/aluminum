#ifndef VIDEOPLAYER_OSX
#define VIDEOPLAYER_OSX

#import <Aluminum/CocoaGL.h>
#import <Aluminum/Includes.hpp>
#import <Aluminum/Texture.hpp>


#import <AVFoundation/AVFoundation.h>
#import <AVFoundation/AVAsset.h>


using namespace aluminum;
using std::string;
//using aluminum::Texture;

//class aluminum::Texture;

@interface VideoPlayer : NSObject {


@public
    AVAssetReader *_movieReader;


    bool frameReady;

    AVAsset *videoAsset;
    AVAssetReader *videoAssetReader;
    AVAssetTrack *videoTrack;
    AVAssetReaderTrackOutput *videoTrackOutput;
    CMSampleBufferRef pixelBuffer;
    CVImageBufferRef imageBuffer;
    //double prevTime;
    //double currTime;
    bool isLooping;
  bool isWaiting;

  bool isLocked;
    int frameCount;
    aluminum::Texture *videoTexture;

}

- (void)onTick:(NSTimer *)timer;

- (void)playVideo;

- (void)startVideoThread:(double)fireDate;

- (Texture *)createVideoTexture:(const string)fname useAudio:(bool)useAudio autoPlay:(bool)autoPlay autoLoop:(bool)autoLoop;

- (bool)nextFrame;

- (id)init;

@end



/*
class VideoPlayer {

public:

  VideoPlayer();
  void startVideoThread(double fireDate);
  void playVideo();

  AVAsset* videoAsset;
  AVAssetReader* videoAssetReader;
  AVAssetTrack* videoTrack;
  AVAssetReaderTrackOutput* videoTrackOutput;
  CMSampleBufferRef pixelBuffer;
  CVImageBufferRef imageBuffer;
   double prevTime;
  double currTime;
  bool isLooping;
  bool isLocked;
  Texture* videoTexture;


};
*/


#endif

