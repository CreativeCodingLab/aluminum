#ifndef INCLUDE_ALU_PROGRAM_HPP
#define INCLUDE_ALU_PROGRAM_HPP

#if defined(_WIN32)
#include "Includes.hpp"
#elif defined(__APPLE__)
#include "Aluminum/Includes.hpp"
#endif


#ifdef BUILD_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif
namespace aluminum {

    using namespace std;


    class Shader {
    public:

        Shader(const std::string &source, GLenum type);

        ~Shader() {
        }

        Shader &compile();

        GLenum type() const {
            return mType;
        }

        //  void id(GLuint v) {mID = v;}
        GLuint id() const {
            return mID;
        }


        const char *log() const;


    private:
        //  std::string mSource;
        GLenum mType;
        //  void sendSource();
        GLuint mID;

        void get(int pname, void *params) const;

        void destroy();
    };


    class Program {
    public:

        Program() {
        }

        ~Program() {
        }

        Program &create();

        Program &attach(std::string source, GLenum type);

        std::string loadText(const std::string &v);
        //void loadText(std::string& text, const std::string& v);



        Program &attach(Shader &s);

        Program &detach(Shader &s);

        Program &link();

        // These parameters must be set before attaching geometry shaders
        //void setGeometryInputPrimitive(Graphics::Primitive prim) { mInPrim = prim; }
        //void setGeometryOutputPrimitive(Graphics::Primitive prim) { mOutPrim = prim; }
        //void setGeometryOutputVertices(unsigned int i) { mOutVertices = i; }

        void bind();

        void unbind();

        void listParams();

        void mapUniforms();

        void mapAttributes();

        GLint uniform(string name);

        GLint attribute(string name);

        //  void id(GLuint v) {mID = v;}
        GLuint id() const {
            return mID;
        }

        const char *log() const;

        void destroy();



    private:

        GLuint mID;
        unsigned int mOutVertices;

        void get(int pname, void *params) const;

        std::map<string, GLint> uniforms;
        std::map<string, GLint> attributes;


    };

} // ::al

#endif
