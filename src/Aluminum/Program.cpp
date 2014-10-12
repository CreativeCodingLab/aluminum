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

    void Shader::log() const {
        GLint status;
        glCompileShader (id());
        glGetShaderiv (id(), GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {
            GLint lsize;
            glGetShaderiv(id(),GL_INFO_LOG_LENGTH, &lsize);
            
            GLchar *infoLog= new GLchar[lsize];
            glGetShaderInfoLog(id(), lsize, NULL, infoLog);

            if (type() == GL_VERTEX_SHADER) {
              cerr << "\t errors in vertex shader: " << endl;
            } else if (type() == GL_FRAGMENT_SHADER) {
                cerr << "\t errors in fragment shader: " << endl;
#ifndef BUILD_IOS
            } else if (type() == GL_GEOMETRY_SHADER) {
              cerr << "\t errors in geometry shader: " << endl;
#endif
            }
            cerr<<"Compile log: \n\t"<< infoLog << endl;
            delete [] infoLog;
            exit(0);
        }
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

    /**********************************************
     *       Begin Program Implementation         *
     **********************************************/
    
    Program &Program::create() {
        mID = glCreateProgram();
        return *this;
    }


    Program &Program::attach(std::string source, GLenum type) {
        printf("%s ",source.c_str());
        Shader s = Shader(source, type);

        printf("program is = %d, shader id = %d\n", id(), s.id());
        glAttachShader(id(), s.id());  //agf this is the right one!
        
        // Check to make sure the shader compiled correctly, if not, exit!
        s.log();

        return *this;
    }
    
    
    // Returns program log detailing success or failures in compilation
    void Program::log() const {
        GLint status;
        glLinkProgram(id());
        glGetProgramiv (id(), GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {
            GLint lsize;
            glGetProgramiv(id(),GL_INFO_LOG_LENGTH, &lsize);
            
            GLchar *infoLog= new GLchar[lsize];
            glGetProgramInfoLog(id(), lsize, NULL, infoLog);
            cerr<<"Link log: "<<infoLog<<endl;
            delete [] infoLog;
            exit(0);
        }
    }
    
    
    Program &Program::link() {

        glLinkProgram(id());

        int isValid;
        glValidateProgram(id());
        glGetProgramiv(id(), GL_VALIDATE_STATUS, &isValid);
      
        if (!isValid) {
            printf("in Program::link - %d is not valid!!!\n", id());
        }


        log();

        mapUniforms();
        mapAttributes();
        listParams();

        return *this;
    }


    Program &Program::detach(Shader &s) {
        glDetachShader(id(), s.id());
        return *this;
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
