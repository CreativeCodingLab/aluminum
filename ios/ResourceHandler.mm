
#import <Aluminum/ResourceHandler.h>

using std::cout;
using std::cerr;
using std::endl;

ResourceHandler::ResourceHandler() {}


const char* ResourceHandler::contentsOfFile(string& file) {
  NSString* filePath = [[NSString alloc] initWithUTF8String:file.c_str()];
  NSString* contents = [NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:nil];
  return [contents UTF8String];
}

string ResourceHandler::pathToResource(const string& resource, const string& type) {

  NSString* resourcePath = [[NSString alloc] initWithUTF8String:resource.c_str()];
  NSString* typePath = [[NSString alloc] initWithUTF8String:type.c_str()];
  NSBundle* mainBundle = [NSBundle mainBundle];
  NSString* fullPath = [mainBundle pathForResource:resourcePath ofType:typePath];

  cout << "in ResourceHandler::pathToResource : pathStr = " << [fullPath UTF8String] << endl;
  return [fullPath UTF8String];
}

void ResourceHandler::loadTexture(Texture& t, const std::string& fname) {

  NSString* basePath = [[NSString alloc] initWithUTF8String:fname.c_str()];
  NSArray* splits = [basePath componentsSeparatedByString: @"."];

  NSString* fileStr = [splits objectAtIndex:0];
  NSString* typeStr = [splits objectAtIndex:1];

    cout << "in ResourceHandler::loadTexture, looking for <" << [fileStr UTF8String] << ">.<" << [typeStr UTF8String] << ">." << endl;

  NSString* path = [[NSBundle mainBundle] pathForResource:fileStr ofType:typeStr];
  cout << "in ResourceHandler::loadTexture : loading texture: " << [fileStr UTF8String] << "." << [typeStr UTF8String] << endl;
    cout << "in ResourceHandler::pathToResource : pathStr = " << [path UTF8String] << endl;

  NSData *texData = [[NSData alloc] initWithContentsOfFile:path];
  UIImage *image = [[UIImage alloc] initWithData:texData];

  if (image == nil) {
    cerr << "ERROR : in ResourceHandler::loadTexture : image is NULL!!!!" << endl;
  }

  int _w = (int)CGImageGetWidth(image.CGImage);
  int _h = (int)CGImageGetHeight(image.CGImage);

  CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
  GLubyte* data = (GLubyte*)malloc( _w * _h * 4 );

  CGContextRef context = CGBitmapContextCreate( data, _w, _h, 8, 4 * _w, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
  CGColorSpaceRelease( colorSpace );
  CGContextClearRect( context, CGRectMake( 0, 0, _w, _h ) );

  // CGContextTranslateCTM( context, 0, _h - _h );
  // CGContextTranslateCTM(context, _w, 0);
  // CGContextScaleCTM(context, -1, 1);

  CGContextDrawImage( context, CGRectMake( 0, 0, _w, _h ), image.CGImage );

  CGContextRelease(context);
  //[texData release];
    Texture::flipBufferY(data, _w, _h);

  t = aluminum::Texture(data, _w, _h, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);

}

