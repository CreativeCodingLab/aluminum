#include "Aluminum/Texture.hpp"

#ifdef BUILD_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

namespace aluminum {

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

      if (type == GL_FLOAT) {
        floatdata = new GLfloat[width * height * 4 * sizeof(float)];
      } else {
        data = new GLubyte[width * height * 4];
      }
      
      create2D();
      
    }
  
  Texture::Texture(GLfloat *_floatdata, int _w, int _h, GLint _internalFormat, GLenum _pixelFormat, GLenum _type) {
    
    
    floatdata = _floatdata;
    width = _w;
    height = _h;
    internalFormat = _internalFormat; //GL_RGBA, GL_RED, etc  (the format of the openGL pixel buffer)
    pixelFormat = _pixelFormat; //GL_RGB, GL_RGBA, GL_LUMINANCE, etc  (the format of the image data)
    type = _type; //GL_UNSIGNED_BYTE, GL_FLOAT, etc
    kind(GL_TEXTURE_2D);
    
    /** GL_REPEAT is not fully supported on iOS, **ONLY** if you use Power-of-Two textures!!! To be safe, we are using the GL_CLAMP_TO_EDGE as the default **/
    
    mWrapMode = GL_CLAMP_TO_EDGE; //GL_REPEAT;
    mMinFilter = GL_NEAREST; //GL_NEAREST;
    mMaxFilter = GL_NEAREST; //GL_NEAREST;
    
    create2D();
  }


  Texture::Texture(GLubyte *_data, int _w, int _h, GLint _internalFormat, GLenum _pixelFormat, GLenum _type) {

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

  Texture::Texture(GLubyte *_d1, GLubyte *_d2, GLubyte *_d3, GLubyte *_d4, GLubyte *_d5, GLubyte *_d6, int _w, int _h, GLint _internalFormat, GLenum _pixelFormat, GLenum _type) {
    
    
    d1 = _d1;
    d2 = _d2;
    d3 = _d3;
    d4 = _d4;
    d5 = _d5;
    d6 = _d6;
    
    width = _w;
    height = _h;
    internalFormat = _internalFormat; //GL_RGBA, GL_RED, etc  (the format of the openGL pixel buffer)
    pixelFormat = _pixelFormat; //GL_RGB, GL_RGBA, GL_LUMINANCE, etc  (the format of the image data)
    type = _type; //GL_UNSIGNED_BYTE, GL_FLOAT, etc
    kind(GL_TEXTURE_CUBE_MAP);
    
    /** GL_REPEAT is not fully supported on iOS, **ONLY** if you use Power-of-Two textures!!! To be safe, we are using the GL_CLAMP_TO_EDGE as the default **/
    
    mWrapMode = GL_CLAMP_TO_EDGE; //GL_REPEAT;
    mMinFilter = GL_LINEAR; //GL_NEAREST;
    mMaxFilter = GL_LINEAR; //GL_NEAREST;
    
    createCubeMap();
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
          
          if (type == GL_FLOAT) {
            glTexImage2D(kind(), 0, internalFormat, width, height, 0, pixelFormat, type, &floatdata[0]);
          } else {
            glTexImage2D(kind(), 0, internalFormat, width, height, 0, pixelFormat, type, &data[0]);
          }
           
			//why was this hardcoded?
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);

        }
        glBindTexture(kind(), 0);

        dump();

        return *this;
    }

  
  Texture &Texture::createCubeMap() {
    
    printf("2D!\t");
    glEnable(kind());
    glGenTextures(1, &texID);
    
    glBindTexture(kind(), texID);
    {
      
      glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, minFilter());
      glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, maxFilter());
      glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapMode());
      glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapMode());
      glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapMode());
      
      glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, width, height, 0, pixelFormat, type, &d1[0]);
      glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, width, height, 0, pixelFormat, type, &d2[0]);
      glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, width, height, 0, pixelFormat, type, &d3[0]);
      glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, width, height, 0, pixelFormat, type, &d4[0]);
      glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, width, height, 0, pixelFormat, type, &d5[0]);
      glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, width, height, 0, pixelFormat, type, &d6[0]);
      
      
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


}
