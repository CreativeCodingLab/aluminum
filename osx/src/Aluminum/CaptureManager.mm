#import <Aluminum/CaptureManager.h>

@implementation CaptureManager

@synthesize captureTexture;
@synthesize newFrame;
@synthesize pixels;
@synthesize isReady;
@synthesize textureReady;

- (id)init {
    if (self = [super init]) {

        printf("in CaptureManager constructor\n");

        isLocked = false;
        newFrame = false;
        isReady = false;
        firstTime = true;
        textureReady = false;

        session = [[AVCaptureSession alloc] init];
        [session beginConfiguration];
        [session setSessionPreset:AVCaptureSessionPresetHigh];

        //get input webcam
        AVCaptureDevice *videoDevice = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
        NSError *error;
        input = [AVCaptureDeviceInput deviceInputWithDevice:videoDevice error:&error];
        [session addInput:input];

        //create the output pipe
        AVCaptureVideoDataOutput *dataOutput = [[AVCaptureVideoDataOutput alloc] init];
        [dataOutput setAlwaysDiscardsLateVideoFrames:YES];
        [dataOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] forKey:(id) kCVPixelBufferPixelFormatTypeKey]];
        [session addOutput:dataOutput];

        //conntect the output to a callback method to process the frames
        dispatch_queue_t videoCaptureQueue = dispatch_queue_create("com.example.subsystem.XYZ", NULL);
        [dataOutput setSampleBufferDelegate:self queue:videoCaptureQueue];
        dispatch_release(videoCaptureQueue);

        //signal that the input and output are connected
        [session commitConfiguration];

        //Now that the session's configuration is committed, we are able to get the resolution of the webcam.

    }

    return self;
}


- (bool)isCapturing {
    return [session isRunning];
}

- (void)stopCapture {
    [session stopRunning];
}

- (void)startCapture {
    [session startRunning];
    newFrame = false;
}

- (void)toggleCapture {
    if ([self isCapturing]) {
        [self stopCapture];
    } else {
        [self startCapture];
    }
}

- (bool)nextFrame {

    if (!firstTime && !textureReady) {
        captureTexture = Texture(tw, th, GL_RGBA, GL_RGB, GL_UNSIGNED_BYTE);
        textureReady = true;
    }

    if ([self isCapturing] && newFrame == true) {
        newFrame = false;
        return true;
    }

    return false;
}

- (bool)updateTextureWithNextFrame {

    if (isReady && [self isCapturing] && newFrame == true) {
        captureTexture.bind(GL_TEXTURE0);
        {

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, captureTexture.width, captureTexture.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
        }
        captureTexture.unbind(GL_TEXTURE0);

        newFrame = false;
        return true;
    }

    return false;
}

- (void)setTextureDimensions {
    AVCaptureInputPort *port = [input.ports objectAtIndex:0];
    CMFormatDescriptionRef formatDescription = port.formatDescription;
    CMVideoDimensions dimensions = CMVideoFormatDescriptionGetDimensions(formatDescription);
    tw = dimensions.width;
    th = dimensions.height;

}

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection {

    //printf("*** in captureOutput\n");

    if (firstTime) {
        [self setTextureDimensions];
        pixels = (unsigned char *) malloc(tw * th * 4 * sizeof(unsigned char));

        firstTime = false;

        //printf("*** captureOutput : firstTime = false\n");
    }

    if (!newFrame) {

        imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
        CVPixelBufferLockBaseAddress(imageBuffer, 0);
        {
            // ptrToImageBuffer = (unsigned char*)CVPixelBufferGetBaseAddress(imageBuffer);
            memcpy(pixels, (unsigned char *) CVPixelBufferGetBaseAddress(imageBuffer), tw * th * 4 * sizeof(unsigned char));

        }
        CVPixelBufferUnlockBaseAddress(imageBuffer, 0);

        newFrame = true;
    }

}

@end
