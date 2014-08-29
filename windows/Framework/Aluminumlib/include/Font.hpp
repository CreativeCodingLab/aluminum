#ifndef INCLUDE_ALU_FONT_HPP
#define INCLUDE_ALU_FONT_HPP

#if defined(_WIN32)
#include "Texture.hpp"
#include "Program.hpp"
#include "MeshData.hpp"
#include "MeshBuffer.hpp"
#include "FBO.hpp"
#include "Shapes.hpp"
#elif defined(__APPLE__)
#include "Aluminum/Texture.hpp"
#include "Aluminum/Program.hpp"
#include "Aluminum/MeshData.hpp"
#include "Aluminum/MeshBuffer.hpp"
#include "Aluminum/FBO.hpp"
#include "Aluminum/Shapes.hpp"
#endif

namespace aluminum {
    //using namespace std;
    using std::map;
    using std::cout;
    using std::istringstream;
    using std::ifstream;
    using std::pair;
    using glm::vec2;
    using glm::ivec2;
    using glm::ivec4;
    using glm::mat4;


	class Texture; // need this forward reference so that it stops complaining...
    class Text3D;

    class Text2D; //need this forward reference so that Font can call Text's constructors
    class Text; //need this forward reference so that Font can call Text's constructors

    class Glyph {
    public:

        Glyph();

        char val;
        int x;
        int y;
        int w;
        int h;
        int xoff;
        int yoff;
        int xadvance;
        float s0, s1, t0, t1;

        static void dump(Glyph *g);

    private:

    };

    class Font {
    public:
        Texture texture;

        Font();


       static Font &loadFont2(Font &font, Texture &texture, const std::string &_file);

        //static Font& loadFont(Texture& texture, const std::string& _file);
        static Font &loadFont(Font &font, Texture &texture, const std::string &_file);



        int tw;
        int th;
        int lineHeight;
        int base;
        int padding;
        float fontSize;
        std::string face;

        map<char, Glyph *> &getGlyphs();

        int highestChar;

        //make a texture/mesh object out of part of the signed font texture
        Text2D signedDistanceText2D(const string &_text);

        Text3D signedDistanceText3D(const string &_text);

        //make a texture/mesh object out of part of the font texture
        Text2D text2D(const string &_text);

        Text3D text3D(const string &_text);

    private:
        map<char, Glyph *> glyphs;

        static Glyph *makeGlyph(std::map<std::string, std::string> &props);
    };

    class Text {
    public:
        Font font;

        vec4 txtColor;
        vec4 bgColor;

        Text &program(Program &_p); //update texture
        Text &programs(Program &_p, Program &_bp); //update texture

        //can grab this data if you want to use it for non-standard things
        MeshBuffer &getMeshBuffer();

        //Texture& getTexture();
        string &getText();

        float meshW, meshH; //the w+h of the mesh buffer holding the text, in world coords
        GLint textureW, textureH; //the actual w+h of the texture used (for signed dist text, can be some scalar of pixelW/H
        int pixelW, pixelH; //the total w+h of text that we are cutting-out letter by letter from the font atlas

        float scaleW, scaleH;
        int screenW, screenH;

        static Program tp;
        static Program bp;
        static Program bp2;
        static Program p2;
        static Program p;
        static bool defaultShadersInstalled;

        //void drawIntoFBO(FBO& fbo); //new


    protected:
        Text();

        Text(Font &f, const std::string &text);

        Text(Font &f, const std::string &text, bool _useSignedDistance);

        int getTextPixelWidth();
        //Text& mesh(vec2 LL, vec2 UR); //update mesh

        Text &updateText(string _text);

        Text &setColor(vec4 _txtColor); //update texture
        Text &setBackground(vec4 _bgColor); //update texture

        //    void drawTextIntoFBO(); //OLD



        string m_text;

        //FBO fbo;
        MeshData meshData;
        MeshBuffer meshBuffer;
        //Texture texture;

        void initDefaultVals();

        void initDefaultShaders(bool _useSD);

        GLint posLoc;
        GLint texCoordLoc;

        int defaultAdvance = 20; //maybe should be able to set this?

        bool getGlyphLocationInFontAtlas(const char c, Glyph *&glyph, const float pen_x, const float pen_y, const float scaleW, const float scaleH, float &x, float &y, float &w, float &h, float &s0, float &s1, float &t0, float &t1);


        virtual void drawBackground(vec2 vLL, vec2 vUR, mat4 M, mat4 V, mat4 P);

        virtual void drawGlyph(vec2 vLL, vec2 vUR, vec2 tLL, vec2 tUR, mat4 M, mat4 V, mat4 P);

#ifdef BUILD_IOS
        string VSH_background2 = ""
                "uniform mat4 proj;\n"
                "uniform mat4 modelview;\n"
                "attribute vec4 vertexPosition;\n"
                "void main() {\n"
                "\tgl_Position = proj * modelview * vertexPosition;\n"
                "}\n";

        string FSH_background2 = ""
      "precision mediump float;"
      "uniform lowp vec4 bgColor;\n"
                "void main(){\n"
                "\t gl_FragColor = bgColor;\n"
                "}\n";

        string VSH_background = ""
                "attribute vec4 vertexPosition;\n"
                "void main() {\n"
                "\tgl_Position = vertexPosition;\n"
                "}\n";

        string FSH_background = ""
                "precision mediump float;"
                "uniform lowp vec4 bgColor;\n"
                "void main(){\n"
                "\t gl_FragColor = bgColor;\n"
                "}\n";

        string VSH_font2 = ""
                "uniform mat4 proj;\n"
                "uniform mat4 modelview;\n"
                "attribute vec4 vertexPosition;\n"
                "attribute vec4 vertexTexCoord;\n"
                "varying vec2 texCoord;\n"
                "void main() {\n"
                "\ttexCoord = vertexTexCoord.xy;\n"
                "\tgl_Position = proj * modelview * vertexPosition;\n"
                "}\n";


        string VSH_font = ""
                "attribute vec4 vertexPosition;\n"
                "attribute vec4 vertexTexCoord;\n"
                "varying vec2 texCoord;\n"
                "void main() {\n"
                "\ttexCoord = vertexTexCoord.xy;\n"
                "\tgl_Position = vertexPosition;\n"
                "}\n";

        string FSH_textureFont = ""
                "precision mediump float; \n"
                "uniform vec4 textColor; \n"
                "uniform sampler2D tex0; \n"
                "vaying lowp vec2 texCoord; \n"
                "void main(){\n"
                "\tfloat a = texture2D(tex0, texCoord).r;\n"
                "\t gl_FragColor = vec4(a) * textColor;\n"
                "}\n";

        string FSH_signedDistanceFont = ""
                "#extension GL_OES_standard_derivatives : enable \n" //necessary for fwidth command on OpenGL ES2.0
                "precision mediump float; \n"
                "uniform lowp vec4 textColor;\n"
                "uniform sampler2D tex0;\n"
                "varying lowp vec2 texCoord;\n"
                "void main(){\n"
                "\tfloat dist = texture2D(tex0, texCoord).r;\n"
                "\tfloat width = fwidth(dist);\n"
                "\tfloat a = smoothstep(0.5-width, 0.5+width, dist);\n"
                "\t gl_FragColor = vec4(textColor.rgb, a*textColor.a);\n"
                "}\n";

        string VSH_singleTexture = ""
                "uniform mat4 proj;\n"
                "uniform mat4 modelview;\n"
                "attribute vec4 vertexPosition;\n"
                "attribute vec4 vertexTexCoord;\n"
                "varying vec2 texCoord;\n"
                "void main() {\n"
                "\ttexCoord = vertexTexCoord.xy;\n"
                "\tgl_Position = proj * modelview * vertexPosition;\n"
                "}\n";

        string FSH_singleTexture = ""
                "precision mediump float;"
                "uniform sampler2D tex0;\n"
                "varying lowp vec2 texCoord;\n"
                "void main(){\n"
                "\t gl_FragColor = texture2D(tex0, texCoord);\n"
                "}\n";

      #else
      string VSH_background2 = "#version 150\n"
      "uniform mat4 proj;\n"
      "uniform mat4 modelview;\n"
      "in vec4 vertexPosition;\n"
      "void main() {\n"
      "\tgl_Position = proj * modelview * vertexPosition;\n"
      "}\n";

      string FSH_background2 = "#version 150\n"
      "uniform vec4 bgColor;\n"
      "out vec4 outputFrag;\n"
      "void main(){\n"
      "\toutputFrag = bgColor;\n"
      "}\n";


      string VSH_background = "#version 150\n"
      "in vec4 vertexPosition;\n"
      "void main() {\n"
      "\tgl_Position = vertexPosition;\n"
      "}\n";

      string FSH_background = "#version 150\n"
      "uniform vec4 bgColor;\n"
      "out vec4 outputFrag;\n"
      "void main(){\n"
      "\toutputFrag = bgColor;\n"
      "}\n";

      string VSH_font2 = "#version 150\n"
      "uniform mat4 proj;\n"
      "uniform mat4 modelview;\n"
      "in vec4 vertexPosition;\n"
      "in vec4 vertexTexCoord;\n"
      "out vec2 texCoord;\n"
      "void main() {\n"
      "\ttexCoord = vertexTexCoord.xy;\n"
      "\tgl_Position = proj * modelview * vertexPosition;\n"
      "}\n";


      string VSH_font = "#version 150\n"
      "in vec4 vertexPosition;\n"
      "in vec4 vertexTexCoord;\n"
      "out vec2 texCoord;\n"
      "void main() {\n"
      "\ttexCoord = vertexTexCoord.xy;\n"
      "\tgl_Position = vertexPosition;\n"
      "}\n";

      string FSH_textureFont = "#version 150\n"
      "uniform vec4 textColor;\n"
      "uniform sampler2D tex0;\n"
      "in vec2 texCoord;\n"
      "out vec4 outputFrag;\n"
      "void main(){\n"
      "\tfloat a = texture(tex0, texCoord).r;\n"
      "\toutputFrag = vec4(a) * textColor;\n"
      "}\n";

      string FSH_signedDistanceFont = "#version 150\n"
      "uniform vec4 textColor;\n"
      "uniform sampler2D tex0;\n"
      "in vec2 texCoord;\n"
      "out vec4 outputFrag;\n"
      "void main(){\n"
      "\tfloat dist = texture(tex0, texCoord).r;\n"
      "\tfloat width = fwidth(dist);\n"
      "\tfloat a = smoothstep(0.5-width, 0.5+width, dist);\n"
      "\toutputFrag = vec4(textColor.rgb, a*textColor.a);\n"
      "}\n";

      string VSH_singleTexture = "#version 150\n"
      "uniform mat4 proj;\n"
      "uniform mat4 modelview;\n"
      //"uniform mat4 model;\n"
      "in vec4 vertexPosition;\n"
      "in vec4 vertexTexCoord;\n"
      "out vec2 texCoord;\n"
      "void main() {\n"
      "\ttexCoord = vertexTexCoord.xy;\n"
      "\tgl_Position = proj * modelview * vertexPosition;\n"
      "}\n";

      string FSH_singleTexture = "#version 150\n"
      "uniform sampler2D tex0;\n"
      "in vec2 texCoord;\n"
      "out vec4 outputFrag;\n"
      "void main(){\n"
      "\toutputFrag = texture(tex0, texCoord);\n"
      "}\n";

#endif
    };


    /*
     //put justify, pen, etc, in here... multiple meshes possible for a single TextTexture obj - or is this overkill???
     class TextBuffer : public MeshBuffer {
     public:
     TextBuffer();

     MeshBuffer mb;
     float w, h;


     };
     */


    class Text3D : public Text {
    public:
        Text3D();

        Text3D(Font &f, const std::string &text);

        Text3D(Font &f, const std::string &text, bool _useSignedDistance);
        //Text3D& meshFromWidth(float w, mat4 M, mat4 V, mat4 P, ivec4 VP);
        //Text3D& meshFromHeight(float h, mat4 M, mat4 V, mat4 P, ivec4 VP);
        //Text3D& meshBox(float w, float h, mat4 M, mat4 V, mat4 P, ivec4 VP);

        void draw(mat4 M, mat4 V, mat4 P, ivec4 VP);

        Text3D &text(string _text);

        Text3D &width(float twidth);

        Text3D &height(float theight);

        vec2 bounds();

        Text3D &background(vec4 _backgroundColor);

        Text3D &color(vec4 _color);


    protected:
        void initDefaultVals3D();
        //void mesh3D(mat4 M, mat4 V, mat4 P, ivec4 VP);

    };

    class Text2D : public Text {
    public:
        Text2D();

        Text2D(Font &f, const std::string &text);

        Text2D(Font &f, const std::string &text, bool _useSignedDistance);

        Text2D &size(float width, float height);

        Text2D &width(float twidth);

        Text2D &height(float theight);

        Text2D &screen(int swidth, int sheight);

        Text2D &screen(ivec4 VP);

        void draw(int sw, int sh);

        void draw(ivec4 VP);

        void draw();

        Text2D &justify(float jx, float jy);

        Text2D &pixel(float _px, float _py); //0/h -> w/0
        Text2D &pen(float _px, float _py); //0.0 -> 1.0

        Text2D &text(string _text);

        Text2D &text(string _text, int sw, int sh);

        Text2D &background(vec4 _backgroundColor);

        Text2D &color(vec4 _color);

        ivec2 pixels(); //get bounds of text in pixels

        float textHeight; //hieght in pixels or perc, depending on if we are using pixel or pen
        float textWidth; //hieght in pixels or perc, depending on if we are using pixel or pen
        float penX, penY, justifyX, justifyY;
    protected:


        bool isBox; // calliing size set isBox to true, which means it will not keep its proper aspect ration
        bool isPixel; // calling pixel sets it true, calling pen sets it false
        bool isSetToWidth;

        void justifyText(const float sw, const float sh, const float pen_x, const float pen_y, const float j_x, const float j_y, const float bw, const float bh, float &bx0, float &bx1, float &by0, float &by1);
        //void justifyText(const float pen_x, const float pen_y, const float j_x, const float j_y, const float bw, const float bh, float &bx0, float &bx1, float &by0, float &by1);

        //void mesh2D(int sw, int sh);
        //Text2D& updateMesh();
        void initDefaultVals2D();


    };


    /*
     class TextFBO : public Text2D {
     TextFBO();
     TextFBO(Font& f, const std::string& text);
     TextFBO(Font& f, const std::string& text, bool _useSignedDistance);

     void draw(FBO& fbo); //maybe should be its own class...?


     };
     */
} // ::al

#endif
