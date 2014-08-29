#ifndef INCLUDE_ALU_TEXTURE_HPP
#define INCLUDE_ALU_TEXTURE_HPP

//#include <string>
#if defined(_WIN32)
#include "Includes.hpp"
#elif defined(__APPLE__)
#include "Aluminum/Includes.hpp"
#endif

#ifdef BUILD_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif


#include <iostream>

namespace aluminum {

    class Texture {

    public:

        Texture() {
        }

        //static Texture& loadTexture(std::string file);
        static void loadTextureData(GLubyte *&data, int &w, int &h, std::string file);

        static Texture &loadTexture(Texture &texture, std::string file);

        Texture(int w, int h, GLint _internalFormat, GLenum _format, GLenum _type);

        Texture(GLubyte *data, int w, int h, GLint _internalFormat, GLenum _format, GLenum _type);

        Texture &create2D();


        Texture(GLubyte *data, int w, int h, int d, GLint _internalFormat, GLenum _format, GLenum _type);

        Texture &create3D();

        Texture &updateData(GLubyte *a_sub_rect_of_data, GLint xoff, GLint yoff, GLsizei w, GLsizei h);

        Texture &updateData(GLubyte *new_data_of_same_size_and_type_as_before);

        Texture &updateData();

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

        //const
        GLubyte *data; //does this need to be const?

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
