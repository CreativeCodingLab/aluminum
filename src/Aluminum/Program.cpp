#include "Aluminum/Program.hpp"

#ifdef BUILD_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif



namespace aluminum {

    using std::map;
    using std::string;


    Shader::Shader(const std::string &source, GLenum type) {
        mID = glCreateShader(type);
        mType = type;
        if (source[0]) {
            const char *s = source.c_str();
            glShaderSource(id(), 1, &s, NULL);
            glCompileShader(id());
        }
    }

    const char *Shader::log() const {
        GLint lsize;
        get(GL_INFO_LOG_LENGTH, &lsize);
        if (0 == lsize) return NULL;

        static char buf[4096];
        glGetShaderInfoLog(id(), 4096, NULL, buf);
        return buf;
    }

    void Shader::get(int pname, void *params) const {
        glGetShaderiv(id(), pname, (GLint *) params);
    }


    void Shader::destroy() {
        glDeleteShader(id());
    }


    string Program::loadText(const std::string &filename) {
        printf("%s ",filename.c_str());
        std::ifstream file;
        file.open(filename.c_str());

        if (!file) {
            printf("in Program::loadText : error, couldn't find file!\n");
            exit(0);
        }

        std::stringstream stream;
        stream << file.rdbuf();
        file.close();

        return stream.str();
    }

/*
  void Program::loadText(std::string& text, const std::string& filename){

    std::ifstream file;
    file.open(filename.c_str());

    if (!file) {
      printf("in Shader::loadSourceFromFile : error, couldn't find file!\n");
      exit(0);
    }

    std::stringstream stream;
    stream << file.rdbuf();
    file.close();

    string aaa = stream.str();
    cout << "you loaded in : " << aaa << "\n";

    text = stream.str();
    //return stream.str();
  }
*/

    Program &Program::create() {
        mID = glCreateProgram();
        return *this;
    }


    Program &Program::attach(std::string source, GLenum type) {
        printf("%s ",source.c_str());
        Shader s = Shader(source, type);

        printf("program is = %d, shader id = %d\n", id(), s.id());
        glAttachShader(id(), s.id());  //agf this is the right one!

        //this (GL_EXT_geometry_shader4) isn't needed for GL > 2.1, but it might be needed for iOS / GLES 2.0 (double check)

        // TODO: check for geometry shader extensions
        //#ifdef GL_EXT_geometry_shader4
        //	printf("GL_EXT_geometry_shader4 defined\n");
        //#endif
        //#ifdef GL_ARB_geometry_shader4
        //	printf("GL_ARB_geometry_shader4 defined\n");
        //#endif

        // if (s.type() == GL_GEOMETRY_SHADER) {
        /* agf
       glProgramParameteri(id(),GL_GEOMETRY_INPUT_TYPE, mInPrim);
       glProgramParameteri(id(),GL_GEOMETRY_OUTPUT_TYPE, mOutPrim);
       glProgramParameteri(id(),GL_GEOMETRY_VERTICES_OUT,mOutVertices);
       */
        // }

        const char *lg = s.log();
        if (lg != NULL) {
            string ls(lg);
            if (ls.length() > 1) {
                if (s.type() == GL_VERTEX_SHADER) {
                    printf("   errors in vertex shader: \n");
                } else if (s.type() == GL_FRAGMENT_SHADER) {
                    printf("   errors in fragment shader: \n");
                #ifndef BUILD_IOS
                } else if (s.type() == GL_GEOMETRY_SHADER) {
                  printf("   errors in geometry shader: \n");
                #endif
                }

                printf("LOG: %s\n", lg);
                //exit(0);
            }
        }

        return *this;
    }


    Program &Program::attach(Shader &s) {
        printf("program is = %d, shader id = %d\n", id(), s.id());
        glAttachShader(id(), s.id());  //agf this is the right one!

        // TODO: check for geometry shader extensions
        //#ifdef GL_EXT_geometry_shader4
        //	printf("GL_EXT_geometry_shader4 defined\n");
        //#endif
        //#ifdef GL_ARB_geometry_shader4
        //	printf("GL_ARB_geometry_shader4 defined\n");
        //#endif

        /* agf
        if (s.type() == GL_GEOMETRY_SHADER) {

               glProgramParameteri(id(),GL_GEOMETRY_INPUT_TYPE, mInPrim);
               glProgramParameteri(id(),GL_GEOMETRY_OUTPUT_TYPE, mOutPrim);
               glProgramParameteri(id(),GL_GEOMETRY_VERTICES_OUT,mOutVertices);

        }
         */

        const char *lg = s.log();
        if (lg != NULL) {
            string ls(lg);
            if (ls.length() > 1) {
                if (s.type() == GL_VERTEX_SHADER) {
                    printf("   errors in vertex shader: \n");
                } else if (s.type() == GL_FRAGMENT_SHADER) {
                    printf("   errors in fragment shader: \n");
                }
                printf("LOG: %s\n", lg);
                //exit(0);
            }
        }

/*
	  if (s.log() != NULL) {
		  if (s.type() == GL_VERTEX_SHADER) {
			  printf ("   errors in vertex shader: \n");
		  } else if (s.type() == GL_FRAGMENT_SHADER) {
			  printf ("   errors in fragment shader: \n");
		  }
		  printf("%s\n", s.log());
		  exit(0);
	  }
*/
        return *this;
    }

    Program &Program::link() {

        glLinkProgram(id());

        int isValid;
        glValidateProgram(id());
        glGetProgramiv(id(), GL_VALIDATE_STATUS, &isValid);
        if (!isValid) {
            printf("in Program::link - %d is not valid!!!\n", id());
        }


        const char *lg = log();
        if (lg != NULL) {
            string ls(lg);
            if (ls.length() > 0) {
                printf("program.id = %d, LOG: %s\n", id(), lg);
                //exit(0);
            }
        }

/*
	  if (log() != NULL) {
		  printf("program %d errors %s\n", id(), log());
		  //exit(0);
	  }
*/
        //  printf("program.id = %d, vertex.glsl = %d, frag.glsl = %d\n", p.id(), sv.id(), sf.id());

        mapUniforms();
        mapAttributes();
        listParams();

        return *this;
    }


    Program &Program::detach(Shader &s) {
        glDetachShader(id(), s.id());
        return *this;
    }


    const char *Program::log() const {
        GLint lsize;
        get(GL_INFO_LOG_LENGTH, &lsize);
        if (0 == lsize) return NULL;

        static char buf[4096];
        glGetProgramInfoLog(id(), 4096, NULL, buf);
        return buf;
    }

    void Program::destroy() {
        glDeleteProgram(id());
    }

    void Program::bind() {
        glUseProgram(id());
    }

    void Program::unbind() {
        glUseProgram(0);
    }

    GLint Program::uniform(string name) {
        return uniforms[name];
    }

    GLint Program::attribute(string name) {
        return attributes[name];
    }

    void Program::get(int pname, void *params) const {
        glGetProgramiv(id(), pname, (GLint *) params);
    }

    void Program::mapUniforms() {

        GLsizei length[1];
        GLint size[1];
        GLenum type[1];
        char name[256];
        GLint count[1];

        glGetProgramiv(id(), GL_ACTIVE_UNIFORMS, count);

        for (int i = 0; i < count[0]; i++) {
            glGetActiveUniform(id(), i, sizeof(name), length, size, type, name);
            uniforms[name] = glGetUniformLocation(id(), name);
        }
    }

    void Program::mapAttributes() {

        GLsizei length[1];
        GLint size[1];
        GLenum type[1];
        char name[256];
        GLint count[1];

        glGetProgramiv(id(), GL_ACTIVE_ATTRIBUTES, count);

        for (int i = 0; i < count[0]; i++) {
            glGetActiveAttrib(id(), i, sizeof(name), length, size, type, name);
            attributes[name] = glGetAttribLocation(id(), name);
        }
    }

    void Program::listParams() {
        GLuint program = id();
        GLint numActiveUniforms = 0;
        GLint numActiveAttributes = 0;

        glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numActiveUniforms);
        glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numActiveAttributes);

        printf("Program::listParams()\n");

        for (int j = 0; j < numActiveUniforms; j++) {
            GLsizei length;
            GLint size;
            GLenum gltype;
            char name[256];

            glGetActiveUniform(program,
                    j,
                    sizeof(name),
                    &length,
                    &size,
                    &gltype,
                    name);

            // check for array names
            if (name[strlen(name) - 3] == '[' && name[strlen(name) - 1] == ']') {
                name[strlen(name) - 3] = '\0';
            }

            printf("uniform %d(%s): type %d size %d length %d\n",
                    j, name, gltype, size, length);
        }

        for (int j = 0; j < numActiveAttributes; j++) {
            GLsizei length;
            GLint size;
            GLenum gltype;
            char name[256];    // could query for max char length

            glGetActiveAttrib(program,
                    j,
                    sizeof(name),
                    &length,
                    &size,
                    &gltype,
                    name);

            printf("attribute %d(%s): type %d size %d length %d\n",
                    j, name, gltype, size, length);
        }
    }

} // al::
