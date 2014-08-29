#include "Aluminum/Texture.hpp"

#ifdef BUILD_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

namespace aluminum {

    /*
     Texture& Texture::loadTexture(std::string file) {
     const char *filename = file.c_str();
     FREE_IMAGE_FORMAT format;
     FIBITMAP *dib(0);
     BYTE *bits(0);
     int width(0), height(0);
     FREE_IMAGE_COLOR_TYPE type;
     GLenum pixFormat;

     format = FreeImage_GetFileType(filename);
     dib = FreeImage_Load(format, filename);
     type = FreeImage_GetColorType(dib);

     switch ( type )
     {
     case FIC_MINISBLACK:
     //right now this is a special case for different types of FONT textures...
     //converting single channel to rgba so shaders are the same...
     //prob should be in its own font loader class, not a general texture loading method
     printf("FIC_MINISBLACK\n");
     pixFormat = GL_RED;
     //dib = FreeImage_ConvertTo32Bits(dib);
     //pixFormat = GL_RGBA;

     break;
     case FIC_MINISWHITE:
     printf("FIC_MINISWHITE\n");
     break;
     case FIC_PALETTE:
     printf("FIC_PALETTE\n");
     break;
     case FIC_CMYK:
     printf("FIC_CMYK\n");
     break;

     case FIC_RGB:
     printf("FIC_RGB\n");
     pixFormat = GL_RGB;
     break;
     case FIC_RGBALPHA:
     printf("FIC_RGBA\n");
     pixFormat = GL_RGBA;
     break;
     default:

     printf("format %d not handled!\n", type);
     break;
     }



     bits = FreeImage_GetBits(dib);
     width = FreeImage_GetWidth(dib);
     height = FreeImage_GetHeight(dib);
     //probably want to have a single channel format plus a single+alpha

     printf("format = %d\n", format);
     printf("w/h = %d/%d\n", width, height);
     printf("FreeImage_IsTransparent = %d\n", FreeImage_IsTransparent(dib));

     Texture* texture = new Texture();

     texture->kind(GL_TEXTURE_2D);

     texture->data = new GLubyte[width * height * 4];
     memcpy ( (GLubyte*)texture->data, (GLubyte*)bits, width*height*4 );

     texture->width = width;
     texture->height = height;
     texture->internalFormat = GL_RGBA;
     texture->pixelFormat = pixFormat;
     texture->type = GL_UNSIGNED_BYTE;
     texture->wrapMode(GL_REPEAT);
     texture->minFilter(GL_LINEAR);
     texture->maxFilter(GL_LINEAR);

     // FreeImage_Unload(dib);
     texture->create2D();

     return (*texture);
     }
     */



#ifndef BUILD_IOS
    /*
  // Just loads in data to a byte array.
  void Texture::loadTextureData(GLubyte* &data, int &w, int &h, std::string file) {

    const char *filename = file.c_str();
    FREE_IMAGE_FORMAT format;
    FIBITMAP *dib(0);
    BYTE *bits(0);
    int width(0), height(0);

    format = FreeImage_GetFileType(filename);
    dib = FreeImage_Load(format, filename);

    dib = FreeImage_ConvertTo32Bits(dib);
    bits = FreeImage_GetBits(dib);

    width = FreeImage_GetWidth(dib);
    height = FreeImage_GetHeight(dib);

    w = width;
    h = height;
    data = bits;

    // for (int i = 0; i < w*h*4; i++) {
    //   printf("%d,", data[i]);
    // } printf("\n");

  }

  Texture& Texture::loadTexture(Texture& texture, std::string file) {

    const char *filename = file.c_str();
    FREE_IMAGE_FORMAT format;
    FIBITMAP *dib(0);
    BYTE *bits(0);
    int width(0), height(0);
    FREE_IMAGE_COLOR_TYPE type;
    GLenum pixFormat = GL_FALSE;

    format = FreeImage_GetFileType(filename);
    dib = FreeImage_Load(format, filename);
    type = FreeImage_GetColorType(dib);

    switch ( type )
    {
      case FIC_RGB:
        printf("FIC_RGB\n");
        dib = FreeImage_ConvertTo32Bits(dib);
        type = FreeImage_GetColorType(dib);
        pixFormat = GL_RGBA;
        break;
      case FIC_RGBALPHA:
        printf("FIC_RGBA\n");
        pixFormat = GL_RGBA;
        break;

      case FIC_MINISBLACK:
        //right now this is a special case for different types of FONT textures...
        //converting single channel to rgba so shaders are the same...
        //prob should be in its own font loader class, not a general texture loading method
        printf("FIC_MINISBLACK\n");
//#ifdef BUILD_IOS
//        pixFormat = GL_LUMINANCE;
//#elif
        pixFormat = GL_RED;
//#endif
        //dib = FreeImage_ConvertTo32Bits(dib);
        //pixFormat = GL_RGBA;

        break;

      case FIC_MINISWHITE:
        printf("FIC_MINISWHITE\n");
        break;
      case FIC_PALETTE:
        printf("FIC_PALETTE\n");
        break;
      case FIC_CMYK:
        printf("FIC_CMYK\n");
        break;
      default:
        printf("format %d not handled!\n", type);
        break;
    }

    if (pixFormat == GL_FALSE) {
      fprintf(stderr, "Error in Texture::loadTexture, unknown pixel format!\n");
      exit(1);
    }

    bits = FreeImage_GetBits(dib);
    width = FreeImage_GetWidth(dib);
    height = FreeImage_GetHeight(dib);
    //probably want to have a single channel format plus a single+alpha


    //pixFormat = GL_RGBA; //temp?? we are forcing to 32 bit, so shouldn't this always work?

    printf("format = %d\n", format);
    printf("w/h = %d/%d\n", width, height);
    printf("FreeImage_IsTransparent = %d\n", FreeImage_IsTransparent(dib));

//     int cnt = 0;
//     for (int i = 0; i < width*height*4; i+=4) {
//     if (bits[i+3] > 0) {
//     printf("%d %d %d %d\n", bits[i], bits[i+1], bits[i+2], bits[i+3]);
//     cnt++;
//     }
//     if (cnt > 8) break;
//     }

    texture.kind(GL_TEXTURE_2D);

    texture.data = new GLubyte[width * height * 4];
    memcpy ( (GLubyte*)texture.data, (GLubyte*)bits, width*height*4 );

    texture.width = width;
    texture.height = height;
    texture.internalFormat = GL_RGBA;
    texture.pixelFormat = pixFormat;
    texture.type = GL_UNSIGNED_BYTE;
    //texture.wrapMode(GL_CLAMP_TO_EDGE);
    texture.mWrapMode = GL_REPEAT; //(GL_REPEAT);
    texture.mMinFilter = GL_NEAREST; //(GL_LINEAR);
    texture.mMaxFilter = GL_NEAREST; //(GL_LINEAR);

    texture.create2D();
    // FreeImage_Unload(dib);
    return texture;
  }
     */
#endif

    //empty rgba texture...
    Texture::Texture(int _w, int _h, GLint _internalFormat, GLenum _pixelFormat, GLenum _type) {

        printf("in Texture::Texture(GLubyte* _data, int _w, int _h, GLenum _format, GLenum _type)\n");

        width = _w;
        height = _h;
        internalFormat = _internalFormat; //GL_RGBA, GL_RED, etc  (the format of the openGL pixel buffer)
        pixelFormat = _pixelFormat; //GL_RGB, GL_RGBA, GL_LUMINANCE, etc  (the format of the image data)
        type = _type; //GL_UNSIGNED_BYTE, GL_FLOAT, etc
        kind(GL_TEXTURE_2D);


        mWrapMode = GL_CLAMP_TO_EDGE; //GL_REPEAT; //(GL_REPEAT);
        mMinFilter = GL_NEAREST; //(GL_LINEAR);
        mMaxFilter = GL_NEAREST; //(GL_LINEAR);

        /*
         wrapMode(GL_REPEAT); //GL_CLAMP_TO_EDGE;
         minFilter(GL_LINEAR);
         maxFilter(GL_LINEAR);
         //minFilter(GL_NEAREST);
         //maxFilter(GL_NEAREST);
         */

        data = new GLubyte[width * height * 4];

        //data = (GLubyte*) malloc (_w*_h*4*sizeof(GLubyte));

        create2D();
    }

    Texture::Texture(GLubyte *_data, int _w, int _h, GLint _internalFormat, GLenum _pixelFormat, GLenum _type) {

        //printf("in Texture::Texture(GLubyte* _data, int _w, int _h, GLenum _format, GLenum _type)\n");

        data = _data;
        width = _w;
        height = _h;
        internalFormat = _internalFormat; //GL_RGBA, GL_RED, etc  (the format of the openGL pixel buffer)
        pixelFormat = _pixelFormat; //GL_RGB, GL_RGBA, GL_LUMINANCE, etc  (the format of the image data)
        type = _type; //GL_UNSIGNED_BYTE, GL_FLOAT, etc
        kind(GL_TEXTURE_2D);

        /** GL_REPEAT is not fully supported on iOS, **ONLY** if you use Power-of-Two textures!!! To be safe, we are using the GL_CLAMP_TO_EDGE as the default **/

        mWrapMode = GL_CLAMP_TO_EDGE; //GL_REPEAT;
        mMinFilter = GL_LINEAR; //GL_NEAREST;
        mMaxFilter = GL_LINEAR; //GL_NEAREST;

        create2D();
    }

#ifndef BUILD_IOS

    Texture::Texture(GLubyte *_data, int _w, int _h, int _d, GLint _internalFormat, GLenum _pixelFormat, GLenum _type) {

        data = _data;
        width = _w;
        height = _h;
        depth = _d;
        internalFormat = _internalFormat; //GL_RGBA, GL_RED, etc  (the format of the openGL pixel buffer)
        pixelFormat = _pixelFormat; //GL_RGB, GL_RGBA, GL_LUMINANCE, etc  (the format of the image data)
        type = _type; //GL_UNSIGNED_BYTE, GL_FLOAT, etc
        kind(GL_TEXTURE_3D);

        mWrapMode = GL_CLAMP_TO_BORDER; //(GL_REPEAT);
        mMinFilter = GL_NEAREST; //(GL_LINEAR);
        mMaxFilter = GL_NEAREST; //(GL_LINEAR);

        create3D();
    }

#endif


    void Texture::flipBufferX(unsigned char *buffer, int _w, int _h) {

        GLuint left;
        GLuint right;

        _w *= 4; //GL_RGBA or BGRA

        for (int y = 0; y < _h; y++) {
            for (int x = 0; x < _w / 2; x += 4) {
                for (int i = 0; i < 4; i++) {

                    left = buffer[(y * _w) + (x) + i];
                    right = buffer[(y * _w) + (_w - 4) - (x) + i];

                    buffer[(y * _w) + (x) + i] = right;
                    buffer[(y * _w) + (_w - 4) - (x) + i] = left;
                }
            }
        }
    }

    void Texture::flipBufferY(unsigned char *buffer, int _w, int _h) {
        // gl renders “upside down” so swap top to bottom into new array.
        GLuint top;
        GLuint bottom;
        _w *= 4; //GL_RGBA or BGRA
        for (int y = 0; y < _h / 2; y++) {
            for (int x = 0; x < _w; x++) {
                //Swap top and bottom bytes
                top = buffer[y * _w + x];
                bottom = buffer[(_h - 1 - y) * _w + x];
                buffer[(_h - 1 - y) * _w + x] = top;
                buffer[y * _w + x] = bottom;

            }
        }
    }


    void Texture::dump() {
        printf("texture id = %d\n", texID);
        printf("\tkind = %d\n", kind());
        printf("\tminFilter = %d, maxFilter = %d\n", minFilter(), maxFilter());
        printf("\twrapMode = %d\n", wrapMode());
        printf("\twidth/height = %d/%d\n", width, height);
    }

    void Texture::destroy() {
        glDeleteTextures(1, &texID);
        //need to destroy entire object, local memory cache as well, call destructor
    }


    Texture &Texture::create2D() {

        printf("2D!\t");
        glEnable(kind());
        glGenTextures(1, &texID);

        glBindTexture(kind(), texID);
        {

            glTexParameteri(kind(), GL_TEXTURE_MIN_FILTER, minFilter());
            glTexParameteri(kind(), GL_TEXTURE_MAG_FILTER, maxFilter());

            glTexParameteri(kind(), GL_TEXTURE_WRAP_S, wrapMode());
            glTexParameteri(kind(), GL_TEXTURE_WRAP_T, wrapMode());

            /*
            for (int i = 0; i < width*height; i++) {
                //if (data[i] != 0)
              {	printf("%d \n", data[i]); }
            }
            */

            //  glTexImage2D(kind(), 0, internalFormat, width, height, 0, pixelFormat, type, &data[0]);
              glTexImage2D(kind(), 0, internalFormat, width, height, 0, pixelFormat, type, &data[0]);
           
			//why was this hardcoded?
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);

        }
        glBindTexture(kind(), 0);

        dump();

        return *this;
    }

#ifndef BUILD_IOS

    Texture &Texture::create3D() {
        glEnable(kind());
        glGenTextures(1, &texID);

        glBindTexture(kind(), texID);
        {

            glTexParameteri(kind(), GL_TEXTURE_MIN_FILTER, minFilter());
            glTexParameteri(kind(), GL_TEXTURE_MAG_FILTER, maxFilter());

            glTexParameteri(kind(), GL_TEXTURE_WRAP_S, wrapMode());
            glTexParameteri(kind(), GL_TEXTURE_WRAP_T, wrapMode());
            glTexParameteri(kind(), GL_TEXTURE_WRAP_R, wrapMode());

            /*
          for (int i = 0; i < width*height; i++) {
              if (data[i] != 0) {	printf("%d \n", data[i]); }
          }
          */

        //  GLint border, GLenum format, GLenum type, const GLvoid *pixels
        glTexImage3D(kind(), 0, internalFormat, width, height, depth, 0, pixelFormat, type, &data[0]);
//          glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, width, height, depth, 0, GL_RED, GL_UNSIGNED_BYTE, &data[0]);


          //  glGenerateMipmap(kind());  // added this, KRA - er, why? AGF
        }
        glBindTexture(kind(), 0);

//        dump();

        return *this;
    }

#endif

    Texture &Texture::bind(GLenum textureUnit) {
        glActiveTexture(textureUnit); //i.e GL_TEXTURE0, etc
        glBindTexture(kind(), id());
        //printf("binding texture id %d\n", id());
        return *this;
    }

    Texture &Texture::unbind(GLenum textureUnit) {
        glActiveTexture(textureUnit); //i.e GL_TEXTURE0, etc
        glBindTexture(kind(), 0);
        return *this;
    }

    Texture &Texture::bind() {
        glBindTexture(kind(), id());
        return *this;
    }

    Texture &Texture::unbind() {
        glBindTexture(kind(), 0);
        return *this;
    }


    Texture &Texture::updateParameters() {
        bind();
        {
            glTexParameteri(kind(), GL_TEXTURE_MIN_FILTER, minFilter());
            glTexParameteri(kind(), GL_TEXTURE_MAG_FILTER, maxFilter());

            glTexParameteri(kind(), GL_TEXTURE_WRAP_S, wrapMode());
            glTexParameteri(kind(), GL_TEXTURE_WRAP_T, wrapMode());

#ifndef BUILD_IOS

            //for 3d textures...
            glTexParameteri(kind(), GL_TEXTURE_WRAP_R, wrapMode());
#endif //BUILD_IOS

        }
        unbind();

        return *this;

    }


    //update with brand new data
    Texture &Texture::updateData(GLubyte *subdata, GLint xoff, GLint yoff, GLsizei _w, GLsizei _h) {

        //data = _data;

        bind();
        {
            glTexSubImage2D(kind(), 0, xoff, yoff, _w, _h, pixelFormat, type, subdata);

            //glTexImage2D    (kind(), 0, internalFormat, width, height, 0, pixelFormat, type, data);

        }
        unbind();

        return *this;
    }


    //update with brand new data
    Texture &Texture::updateData(GLubyte *_data) {

        data = _data;

        bind();
        {

            glTexImage2D(kind(), 0, internalFormat, width, height, 0, pixelFormat, type, data);

        }
        unbind();

        return *this;
    }

    //assumes you have already changed the current data array attached to this texture
    Texture &Texture::updateData() {
        bind();
        {

            glTexImage2D(kind(), 0, internalFormat, width, height, 0, pixelFormat, type, data);

        }
        unbind();

        return *this;
    }


    GLuint Texture::id() {
        return texID;
    }

    GLenum Texture::kind() {
        return mKind;
    }

    GLint Texture::wrapMode() {
        return mWrapMode;
    }

    GLint Texture::minFilter() {
        return mMinFilter;
    }

    GLint Texture::maxFilter() {
        return mMaxFilter;
    }


    Texture &Texture::kind(GLenum _kind) {
        mKind = _kind;
        return *this;
    }

    Texture &Texture::wrapMode(GLint _wrapMode) {
        mWrapMode = _wrapMode;
        updateParameters();
        return *this;
    }

    Texture &Texture::minFilter(GLint _minFilter) {
        mMinFilter = _minFilter;
        updateParameters();
        return *this;
    }

    Texture &Texture::maxFilter(GLint _maxFilter) {
        mMaxFilter = _maxFilter;
        updateParameters();
        return *this;
    }

}
