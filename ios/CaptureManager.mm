
#import <Aluminum/CaptureManager.h>

@implementation CaptureManager

@synthesize captureTexture;
@synthesize pixels;

@synthesize newFrame;
@synthesize textureReady;

- (id) init {
  if ( self = [super init] ) {
    [self setupCapture:AVCaptureSessionPresetLow side:AVCaptureDevicePositionFront];
  }

  return self;
}


- (id) init:(NSString *)preset side:(AVCaptureDevicePosition)side  {
  if ( self = [super init] ) {
    [self setupCapture:preset side:side];
  }

  return self;
}

- (void) setupCapture:(NSString *)preset side:(AVCaptureDevicePosition)side {

  newFrame = false;
  firstTime = true;
  textureReady = false;

  session = [[AVCaptureSession alloc] init];
  [session beginConfiguration];
  [session setSessionPreset:preset];
  //[session setSessionPreset:AVCaptureSessionPresetHigh];

  AVCaptureDevice* videoDevice;
  if (side == AVCaptureDevicePositionBack) {
    videoDevice = [self backFacing];
  } else {
    videoDevice = [self frontFacing];
  }

  NSError * error;
  input = [AVCaptureDeviceInput deviceInputWithDevice:videoDevice error:&error];
  [session addInput:input];

  //create the output pipe
  AVCaptureVideoDataOutput* dataOutput = [[AVCaptureVideoDataOutput alloc] init];
  [dataOutput setAlwaysDiscardsLateVideoFrames:YES];
  [dataOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] forKey:(id)kCVPixelBufferPixelFormatTypeKey]];
  [session addOutput:dataOutput];

  //conntect the output to a callback method to process the frames
  dispatch_queue_t videoCaptureQueue = dispatch_queue_create("com.example.subsystem.XYZ", NULL);
  [dataOutput setSampleBufferDelegate:self queue:videoCaptureQueue];
  dispatch_release(videoCaptureQueue);

  /*
   [dataOutput setSampleBufferDelegate:self queue:dispatch_get_main_queue()];
   */
  //signal that the input and output are connected
  [session commitConfiguration];

}

- (AVCaptureDevice *)frontFacing {
  //  look at all the video devices and get the first one that's on the front
  NSArray *videoDevices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
  for (AVCaptureDevice *device in videoDevices) {
    //NSLog(@"name of deivce : %@ \n", device.localizedName);

    if (device.position == AVCaptureDevicePositionFront) {
      return device;
    }
  }

  //couldn't find one on the front, so just get the default video device.
  return [self backFacing];
}

- (AVCaptureDevice *)backFacing {
  return [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
}


- (bool) isCapturing {
  return [session isRunning];
}

- (void) stopCapture {
  [session stopRunning];
}

- (void)startCapture {
  [session startRunning];
  newFrame = false;
}

- (void) toggleCapture {
  if ([self isCapturing]) {
    [self stopCapture];
  } else {
    [self startCapture];
  }
}

- (bool) nextFrame {

  if (!firstTime && !textureReady) {
    captureTexture = Texture(dimensions.width, dimensions.height, GL_RGBA, GL_RGB, GL_UNSIGNED_BYTE);
    textureReady = true;
  }

  if ([self isCapturing] && newFrame == true) {
    newFrame = false;
    return true;
  }

  return false;
}

- (bool) updateTextureWithNextFrame {

  if (!firstTime && !textureReady) {
    //captureTexture = Texture(dimensions.width, dimensions.height, GL_RGBA, GL_RGB, GL_UNSIGNED_BYTE);
      captureTexture = Texture(dimensions.width, dimensions.height, GL_RGBA, GL_RGB, GL_UNSIGNED_BYTE);

      textureReady = true;
  }


    if ([self isCapturing] && newFrame == true) {
        //printf("here! adding new data to texture...\n");
        captureTexture.bind(GL_TEXTURE0); {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, captureTexture.width, captureTexture.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
        } captureTexture.unbind(GL_TEXTURE0);

        newFrame = false;

        return true;
    }



    return false;
}

-(void) setTextureDimensions {
  AVCaptureInputPort *port = [input.ports objectAtIndex:0];
  CMFormatDescriptionRef formatDescription = port.formatDescription;
  dimensions = CMVideoFormatDescriptionGetDimensions(formatDescription);

  printf("setting texture dimensions : %d %d\n", dimensions.width, dimensions.height);
}


/*can be used to get a specific camera from the front or back (if available) IOS only
 */

/* do this later...
 - (void) switchFrontToBack:(NSString*) format {
 [session beginConfiguration];


   AVCaptureDevice* videoDevice;
   if (side == AVCaptureDevicePositionBack) {
     videoDevice = [self backFacing];
   } else {
     videoDevice = [self frontFacing];
   }

   NSError * error;
   input = [AVCaptureDeviceInput deviceInputWithDevice:videoDevice error:&error];
   [session addInput:input];




 [session removeInput:videoIn];
 [session addVideoInput:format];

 captureSession.sessionPreset = format;
 self.avTexture = RENDERER.BACK_TEX;

 [RENDERER configureRendererForBackFacingCamera];

 [captureSession commitConfiguration];
 //RENDERER.IS_BUSY = false;

 newFrame = false;
 firstTime = true;
 textureReady = false;


 }


 - (void) switchBackToFront:(NSString*) format {
 //RENDERER.IS_BUSY = true;

 [captureSession beginConfiguration];

 [captureSession removeInput:videoIn];
 side = @"FRONT";

 [self addVideoInput:format];

 self.avTexture = RENDERER.FRONT_TEX;

 [RENDERER configureRendererForFrontFacingCamera];


 [captureSession commitConfiguration];
 //RENDERER.IS_BUSY = false;

 }
 */

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection {

  if (firstTime) {
    [self setTextureDimensions];
    pixels = (unsigned char*)malloc(dimensions.width * dimensions.height * 4 * sizeof(unsigned char));

    firstTime = false;
  }

  if (!newFrame) {
    //  printf("read new data from camera\n");
    imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);

    CVPixelBufferLockBaseAddress( imageBuffer, 0 ); {
      memcpy(pixels, (unsigned char*)CVPixelBufferGetBaseAddress(imageBuffer), dimensions.width * dimensions.height * 4 * sizeof(unsigned char));

    } CVPixelBufferUnlockBaseAddress(imageBuffer, 0);

    newFrame = true;
  }

}

@end
