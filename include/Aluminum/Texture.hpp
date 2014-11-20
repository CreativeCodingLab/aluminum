#ifndef INCLUDE_ALU_TEXTURE_HPP
#define INCLUDE_ALU_TEXTURE_HPP

//#include <string>
#include "Aluminum/Includes.hpp"

#ifdef BUILD_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif


#include <iostream>

namespace aluminum {

    class Texture {

    public:

        Texture() {}
        Texture(GLfloat *_floatdata, int _w, GLint _internalFormat, GLenum _pixelFormat, GLenum _type);

        Texture(GLubyte *_data, int _w, GLint _internalFormat, GLenum _pixelFormat, GLenum _type);

        Texture &create1D();


        Texture(int w, int h, GLint _internalFormat, GLenum _format, GLenum _type);

        Texture(GLubyte *data, int w, int h, GLint _internalFormat, GLenum _format, GLenum _type);

        Texture(GLfloat *floatdata, int w, int h, GLint _internalFormat, GLenum _format, GLenum _type);

        static void loadTextureData(GLubyte *&data, int &w, int &h, std::string file);

        static Texture &loadTexture(Texture &texture, std::string file);



        Texture &create2D();


        Texture(GLubyte *_d1, GLubyte *_d2, GLubyte *_d3, GLubyte *_d4, GLubyte *_d5, GLubyte *_d6, int _w, int _h, GLint _internalFormat, GLenum _pixelFormat, GLenum _type) ;

        Texture &createCubeMap();
        Texture(GLubyte *data, int w, int h, int d, GLint _internalFormat, GLenum _format, GLenum _type);


        Texture &create3D();

        Texture &updateData();

        Texture &updateData(GLubyte *_data); // This is for the 1D texture
        Texture &updateData(GLfloat *_floatdata); // This is for the 1D texture
        
        Texture &updateData(GLubyte *a_sub_rect_of_data, GLint xoff, GLint yoff, GLsizei w, GLsizei h);

        Texture &updateParameters();

        Texture &bind(GLenum textureUnit);

        Texture &unbind(GLenum textureUnit);

        Texture &bind();

        Texture &unbind();

        GLuint id();

        GLenum kind();

        GLint wrapMode();

        GLint minFilter();

        GLint maxFilter();

        Texture &kind(GLenum _kind);

        Texture &wrapMode(GLint _wrapMode);

        Texture &minFilter(GLint _minFilter);

        Texture &maxFilter(GLint _maxFilter);

        //static void flipBufferY(GLubyte* buffer, int _w, int _h);
        static void flipBufferX(unsigned char *buffer, int _w, int _h);

        static void flipBufferY(unsigned char *buffer, int _w, int _h);

        void destroy();

        void dump();

        int width;
        int height;
        int depth; //only for GL_TEXTURE_3D

        GLubyte* data;

        //for cubemaps
        GLubyte* d1;
        GLubyte* d2;
        GLubyte* d3;
        GLubyte* d4;
        GLubyte* d5;
        GLubyte* d6;

        //for float data
        GLfloat* floatdata;

    protected:

        GLuint texID;

        GLenum mKind; //GL_TEXTURE_2D, GL_TEXTURE_3D, or GL_TEXTURE_CUBE_MAP
        GLint internalFormat; //GL_RGBA, GL_R3_G3_B2, etc
        GLenum pixelFormat; //e.g. GL_RGBA, GL_BGRA
        GLenum type; //e.g. GL_UNSIGNED_BYTE, GL_FLOAT

        GLint mWrapMode;
        GLint mMinFilter;
        GLint mMaxFilter;
        //bool isBound;

    };
}

#endif
