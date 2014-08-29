#ifndef INCLUDE_ALU_MESHBUFFER_HPP
#define INCLUDE_ALU_MESHBUFFER_HPP

#if defined(_WIN32)
#include "Includes.hpp"
#include "MeshData.hpp"
#elif defined(__APPLE__)
#include "Aluminum/Includes.hpp"
#include "Aluminum/MeshData.hpp"
#endif

#ifdef BUILD_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

//class MeshData;

namespace aluminum {

    class MeshBuffer {

    public:

      MeshBuffer();

        //MeshBuffer& init(MeshData &_data, int p, int n, int tc, int c);
        //MeshBuffer& update(MeshData &_data, int p, int n, int tc, int c);
        MeshBuffer init(MeshData _data, int p, int n, int tc, int c);

        MeshBuffer update(MeshData _data, int p, int n, int tc, int c);

        MeshBuffer update(MeshData _data);

        virtual void draw();

        virtual void drawLines();

        virtual void drawTriangleStrip();

        virtual void drawPoints();

    protected:

        void checkAttributes(int p, int n, int tc, int c);

        template<class T> void initVBO(GLuint vboID, int loc, int size, T b);

        void initIBO();

        GLuint vao[1]; // vertex array object ID
        GLuint vbo[4]; // vertex buffer object ID
        GLuint ibo[4]; // index(?) buffer object ID

        MeshData data;

        bool useNormals = false;
        bool useTexCoords = false;
        bool useColors = false;
        bool useIndices = false;

        int pLoc = -1;  // vertex position location
        int nLoc = -1; // normals location
        int tcLoc = -1; // texture coordinates location
        int cLoc = -1; // color location

    };

}

#endif
