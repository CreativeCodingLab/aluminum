//
//  ResourceHandler.mm
//  aluminum
//
//  Created by Angus Forbes on 8/5/13.
//  Copyright (c) 2013 Angus Forbes. All rights reserved.
//

#import <Aluminum/ResourceHandler.h>

using std::cout;

ResourceHandler::ResourceHandler() {
}


const char *ResourceHandler::contentsOfFile(string &file) {
    NSString *filePath = [[[NSString alloc] initWithUTF8String:file.c_str()] autorelease];
    NSString *contents = [NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:nil];
    return [contents UTF8String];
}

string ResourceHandler::pathToResource(const string &resource){
    NSString *basePath = [[[NSString alloc] initWithUTF8String:resource.c_str()] autorelease];
    NSArray *splits = [basePath componentsSeparatedByString:@"."];

    NSString *fileStr = [splits objectAtIndex:0];
    NSString *typeStr = [splits objectAtIndex:1];

    NSBundle *mainBundle = [NSBundle mainBundle];
    NSString *fullPath = [mainBundle pathForResource:fileStr ofType:typeStr];

    cout << "in GetPathForResourceOfType(...), pathStr = " << [fullPath UTF8String] << "\n";
    return [fullPath UTF8String];

}


string ResourceHandler::pathToResource(const string &resource, const string &type) {

    NSString *resourcePath = [[[NSString alloc] initWithUTF8String:resource.c_str()] autorelease];
    NSString *typePath = [[[NSString alloc] initWithUTF8String:type.c_str()] autorelease];
    NSBundle *mainBundle = [NSBundle mainBundle];
    NSString *fullPath = [mainBundle pathForResource:resourcePath ofType:typePath];

    cout << "in GetPathForResourceOfType(...), pathStr = " << [fullPath UTF8String] << "\n";
    return [fullPath UTF8String];
}


void ResourceHandler::loadProgram(Program &p, const std::string &name, int pLoc, int nLoc, int tcLoc, int cLoc) {

    p.create();

    string sv = pathToResource(name, "vsh");
    p.attach(contentsOfFile(sv), GL_VERTEX_SHADER);

    if (pLoc >= 0) glBindAttribLocation(p.id(), pLoc, "vertexPosition");
    if (nLoc >= 0) glBindAttribLocation(p.id(), nLoc, "vertexNormal");
    if (tcLoc >= 0) glBindAttribLocation(p.id(), tcLoc, "vertexTexCoord");
    if (pLoc >= 0) glBindAttribLocation(p.id(), cLoc, "vertexColor");

    string sp = pathToResource(name, "fsh");
    p.attach(contentsOfFile(sp), GL_FRAGMENT_SHADER);
    p.link();
}


void ResourceHandler::loadTexture(Texture &t, const std::string &name) {

    NSString *basePath = [[[NSString alloc] initWithUTF8String:name.c_str()] autorelease];
    NSArray *splits = [basePath componentsSeparatedByString:@"."];

    NSString *fileStr = [splits objectAtIndex:0];
    NSString *typeStr = [splits objectAtIndex:1];

    NSString *pathname = [[NSBundle mainBundle] pathForResource:fileStr ofType:typeStr];
    NSLog(@"Loading texture: %@.%@\n", fileStr, typeStr);

    NSLog(@"loading in texture from path: %@\n", pathname);
    NSData *texData = [[NSData alloc] initWithContentsOfFile:pathname];
    NSImage *nsimage = [[NSImage alloc] initWithData:texData];

    NSBitmapImageRep *imageClass = [[[NSBitmapImageRep alloc] initWithData:[nsimage TIFFRepresentation]] autorelease];
    [nsimage release];

    CGImageRef cgImage = imageClass.CGImage;

    int _w = (int) CGImageGetWidth(cgImage);
    int _h = (int) CGImageGetHeight(cgImage);

    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    GLubyte *data = (GLubyte *) malloc(_w * _h * 4);

    CGContextRef context = CGBitmapContextCreate(data, _w, _h, 8, _w * 4, colorSpace, kCGImageAlphaNoneSkipLast);
    CGContextSetBlendMode(context, kCGBlendModeCopy);

    bool flipVertical = true;
    if (flipVertical) {
        CGContextTranslateCTM(context, 0.0, _h);
        CGContextScaleCTM(context, 1.0, -1.0);
    }

    CGContextDrawImage(context, CGRectMake(0.0, 0.0, _w, _h), cgImage);
    CGContextRelease(context);

    [texData release];

    t = aluminum::Texture(data, _w, _h, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);

    // return new Texture(data, _w, _h, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
}


/*
Texture** ResourceHandler::LoadNaturalMaterialsTexture(const string &fname, int tw, int th, int cols, int rows, int slices, int numTextures) {

  bool compressToBits = false; //true;
  bool useLuminance = true; //false; //true;
  NSString* basePath = [[NSString alloc] initWithUTF8String:fname.c_str()];
  NSArray* splits = [basePath componentsSeparatedByString: @"."];

  NSString* fileStr = [splits objectAtIndex:0];
  NSString* typeStr = [splits objectAtIndex:1];

  int dw = tw * cols;
  int dh = th * rows;

  //int numTextures = 2;
  Texture** textures = (Texture**)malloc(numTextures * sizeof(Texture*));

  int i = 1;

  for (int t = 0; t < numTextures; t++) {

    Texture* duniteTex;

    if (compressToBits == true) {
      duniteTex = new Texture(dw / 4, dh / 4, GL_RGBA, GL_UNSIGNED_BYTE);
      duniteTex->SetFilterModes(GL_LINEAR, GL_LINEAR);
    } else if (useLuminance == true) {
      duniteTex = new Texture(dw, dh, GL_LUMINANCE, GL_UNSIGNED_BYTE);
      // duniteTex->SetFilterModes(GL_NEAREST, GL_NEAREST);

    } else {
      duniteTex = new Texture(dw, dh, GL_RGBA, GL_UNSIGNED_BYTE);
    }

    for (int y = 0; y < rows; y++) {
      for (int x = 0; x < cols; x++) {

        NSString* useFileStr = [NSString stringWithFormat:@"%@%d", fileStr, i];
        NSString* path = [[NSBundle mainBundle] pathForResource:useFileStr ofType:typeStr];
        NSLog(@"Loading texture: %@.%@\n", useFileStr, typeStr);

        NSData *texData = [[NSData alloc] initWithContentsOfFile:path];
        UIImage *image = [[UIImage alloc] initWithData:texData];

        //        int _w = CGImageGetWidth(image.CGImage);
        //        int _h = CGImageGetHeight(image.CGImage);
        int _w = tw; //CGImageGetWidth(image.CGImage);
        int _h = th; //CGImageGetHeight(image.CGImage);

        //CGColorSpaceRef colorSpace = CGImageGetColorSpace(image.CGImage);
        //CGColorSpaceRef colorSpace = kCGColorSpaceModelMonochrome;
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        //CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceGray();

        GLubyte* data = (GLubyte*)malloc( _w * _h * 4 );

        CGContextRef context = CGBitmapContextCreate( data, _w, _h, 8, 4 * _w, colorSpace, kCGImageAlphaPremultipliedLast |kCGBitmapByteOrder32Big );

        CGColorSpaceRelease( colorSpace );
        CGContextClearRect( context, CGRectMake( 0, 0, _w, _h ) );

        CGContextDrawImage( context, CGRectMake( 0, 0, _w, _h ), image.CGImage );
        //CGContextDrawImage( context, CGRectMake( 0, 0, 512, 512 ), image.CGImage );


        //CGContextDrawImage( context, CGRectMake( marginW, marginH, useW, useH ), image.CGImage );
        CGContextRelease(context);
        [texData release];

        //write this data into the big texture
        duniteTex->Bind();

        if (compressToBits == true) {

          GLubyte* compressedData = CompressToBits(tw, th, data);

          glTexSubImage2D(GL_TEXTURE_2D, 0, x * (_w/4), y * (_h/4), (_w/4), (_h/4), GL_RGBA, GL_UNSIGNED_BYTE, compressedData);
          free(data);

        } else if (useLuminance == true) {

          GLubyte* luminanceData = (GLubyte*)malloc( _w * _h );
          for (int idx = 0, lidx = 0; idx < _w * _h * 4; idx += 4, lidx++) {
            luminanceData[lidx] = data[idx];
          }

          glTexSubImage2D(GL_TEXTURE_2D, 0, x * _w, y * _h, _w, _h, GL_LUMINANCE, GL_UNSIGNED_BYTE, luminanceData);

          free(luminanceData);
          free(data);

        } else {

          //normal rgba
          glTexSubImage2D(GL_TEXTURE_2D, 0, x * _w, y * _h, _w, _h, GL_RGBA, GL_UNSIGNED_BYTE, data);
          free(data);
        }

        duniteTex->Unbind();
        i++;
      }
    }

    textures[t] = duniteTex;

  }

  return textures;
}
*/





//void ResourceHandler::checkForNewFrame(Texture &t) { }



/*
GLubyte* ResourceHandler::texture2D(const string &fname) {

    NSString* basePath = [[NSString alloc] initWithUTF8String:fname.c_str()];
    NSArray* splits = [basePath componentsSeparatedByString: @"."];

    NSString* fileStr = [splits objectAtIndex:0];
    NSString* typeStr = [splits objectAtIndex:1];

    NSString* pathname = [[NSBundle mainBundle] pathForResource:fileStr ofType:typeStr];
    NSLog(@"Loading texture: %@.%@\n", fileStr, typeStr);

    NSLog(@"loading in texture from path: %@\n", pathname);
    NSData *texData = [[NSData alloc] initWithContentsOfFile:pathname];
    NSImage *nsimage = [[NSImage alloc] initWithData:texData];

    NSBitmapImageRep *imageClass = [[NSBitmapImageRep alloc] initWithData:[nsimage TIFFRepresentation]];
	[nsimage release];

    CGImageRef cgImage = imageClass.CGImage;

    int _w = (int)CGImageGetWidth(cgImage);
    int _h = (int)CGImageGetHeight(cgImage);


    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    GLubyte* data = (GLubyte*)malloc( _w * _h * 4 );

    CGContextRef context = CGBitmapContextCreate(data, _w, _h, 8, _w * 4, colorSpace, kCGImageAlphaNoneSkipLast);
	CGContextSetBlendMode(context, kCGBlendModeCopy);

    //  if(flipVertical)
    // {
    // CGContextTranslateCTM(context, 0.0, _h);
    // CGContextScaleCTM(context, 1.0, -1.0);
    // }


	CGContextDrawImage(context, CGRectMake(0.0, 0.0, _w, _h), cgImage);
	CGContextRelease(context);

    [texData release];
    // return new Texture(data, _w, _h, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);

    return data;
}

*/