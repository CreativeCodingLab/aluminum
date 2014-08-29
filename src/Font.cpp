#include "Aluminum/Font.hpp"

namespace aluminum {

    Program Text::tp;
    Program Text::bp;
    Program Text::bp2;
    Program Text::p2;
    Program Text::p;
    bool Text::defaultShadersInstalled = false;


    int MAX_FBO_SIZE = 1024;

    /* Font factory methods to build either a Text2D or a Text3D */

    Text2D Font::signedDistanceText2D(const string &_text) {
//        printf("In signedDistanceText2D!!\n");
        //return Text2D::Text2D(*this, _text, true);
        return Text2D(*this, _text, true);
    }

    Text3D Font::signedDistanceText3D(const string &_text) {
        //return Text3D::Text3D(*this, _text, true);
        return Text3D(*this, _text, true);
    }

    Text2D Font::text2D(const string &_text) {
        //return Text2D::Text2D(*this, _text, false);
        return Text2D(*this, _text, false);
    }

    Text3D Font::text3D(const string &_text) {
        //return Text3D::Text3D(*this, _text, false);
        return Text3D(*this, _text, false);
    }

    /* Text3D */

    Text3D::Text3D() : Text() {
        initDefaultVals3D();
    }

    Text3D::Text3D(Font &f, const std::string &text) : Text(f, text) {
        initDefaultVals3D();
    }

    Text3D::Text3D(Font &_f, const string &_text, bool _useSignedDistance) : Text(_f, _text, _useSignedDistance) {
        initDefaultVals3D();
    }

    void Text3D::initDefaultVals3D() {
    }

    /*
    void Text3D::mesh3D(mat4 M, mat4 V, mat4 P, ivec4 VP) {

      //center...
      vec4 p0 = V * M * vec4(0, 0, 0, 1);
      vec4 p1 = V * M * vec4(meshW, meshH, 0, 1);
      float z = (p0.z + p1.z) / 2.0;

      //cout << "p0 : " << glm::to_string(p0) << "\n";
      //cout << "p1 : " << glm::to_string(p1) << "\n";
      //cout << "z: " << z << " \n";

      vec3 sLL = glm::project(vec3(-meshW/2.0, -meshH/2.0, z), mat4(), P, VP);
      vec3 sUR = glm::project(vec3(meshW/2.0, meshH/2.0, z), mat4(), P, VP);
      textureW = (sUR.x - sLL.x);
      textureH = (sUR.y - sLL.y);

      //printf("in meshFromWidth PROJECT : texture W/H = %d/%d\n", textureW, textureH);

      mesh(vec2(0,0), vec2(meshW, meshH));
      updateTexture();
    }

    Text3D& Text3D::meshFromWidth(float w, mat4 M, mat4 V, mat4 P, ivec4 VP) {

      float ar = (float)VP.z/(float)VP.w;

      meshW = w;
      meshH = w * ((float)pixelH/(float)pixelW) * ar;

      mesh3D(M, V, P, VP);
      return *this;
    }

    Text3D& Text3D::meshFromHeight(float h, mat4 M, mat4 V, mat4 P, ivec4 VP) {
      float ar = (float)VP.w/(float)VP.z;

      meshH = h;
      meshW = h * ((float)pixelW/(float)pixelH) * ar;
      mesh3D(M, V, P, VP);
      return *this;
    }

    Text3D& Text3D::meshBox(float w, float h, mat4 M, mat4 V, mat4 P, ivec4 VP) {
      float ar_w = (float)VP.w/(float)VP.z;
      meshW = w * ar_w;
      meshH = h;
      mesh3D(M, V, P, VP);
      return *this;
    }
    */


    Text3D &Text3D::text(string _text) {
        updateText(_text);
        return *this; //you will need to call one of the mesh functions!
    }

    Text3D &Text3D::background(vec4 _backgroundColor) {
        return (Text3D &) setBackground(_backgroundColor);
    }

    Text3D &Text3D::color(vec4 _color) {
        return (Text3D &) setColor(_color);
    }

    vec2 Text3D::bounds() {
        return vec2(meshW, meshH);
    }

    Text3D &Text3D::height(float h) {
        meshH = h;
        meshW = h * ((float) pixelW / (float) pixelH);

        return *this;
    }

    Text3D &Text3D::width(float w) {
        meshH = w * ((float) pixelH / (float) pixelW);
        meshW = w;
        return *this;
    }

    void Text3D::draw(mat4 M, mat4 V, mat4 P, ivec4 VP) {

        //calculate texture boundaries
        /*
       //center...
       vec4 p0 = V * M * vec4(0, 0, 0, 1);
       vec4 p1 = V * M * vec4(meshW, meshH, 0, 1);
       float z = (p0.z + p1.z) / 2.0;

       //cout << "p0 : " << glm::to_string(p0) << "\n";
       //cout << "p1 : " << glm::to_string(p1) << "\n";
       //cout << "z: " << z << " \n";

       vec3 sLL = glm::project(vec3(-meshW/2.0, -meshH/2.0, z), mat4(), P, VP);
       vec3 sUR = glm::project(vec3(meshW/2.0, meshH/2.0, z), mat4(), P, VP);
       textureW = (sUR.x - sLL.x);
       textureH = (sUR.y - sLL.y);
       */

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        screenW = VP.z;
        screenH = VP.w;

        scaleW = (((float) (meshH * screenW) / (float) screenW) / (float) (font.lineHeight));
        scaleH = (((float) (meshH * screenH) / (float) screenH) / (float) (font.lineHeight));

        //draw background...

        drawBackground(vec2(0, 0), vec2(meshW, meshH), M, V, P);


        //draw glyphs...
        Glyph *glyph;

        float pen_x = ((font.padding / 2) * scaleW);
        float pen_y = 0;

        float x0, x1, y0, y1, s0, s1, t0, t1;

        for (size_t i = 0; i < m_text.length(); ++i) {

            if (getGlyphLocationInFontAtlas(m_text[i], glyph, pen_x, pen_y, scaleW, scaleH, x0, x1, y0, y1, s0, s1, t0, t1)) {

                vec2 vLL = vec2(x0, y0);
                vec2 vUR = vec2(x1, y1);
                vec2 tLL = vec2(s0, t0);
                vec2 tUR = vec2(s1, t1);

                drawGlyph(vLL, vUR, tLL, tUR, M, V, P);
                pen_x += glyph->xadvance * scaleW;

            } else {
                pen_x += defaultAdvance * scaleW;
            }
        }
    }


/*
  TextFBO::TextFBO() : Text2D() {}
  TextFBO::TextFBO(Font& f, const std::string& text) : Text2D(f, text) {}
  TextFBO::TextFBO(Font& _f, const string& _text, bool _useSignedDistance) : Text2D(_f, _text, _useSignedDistance) {}



  void TextFBO::draw(FBO& fbo) {
    ivec2 pix = pixels();
    fbo.create(pix.x, pix.y);

    Text::drawIntoFBO(fbo);

  }
*/

    /* Text2D */

    Text2D::Text2D() : Text() {
        initDefaultVals2D();
    }

    Text2D::Text2D(Font &f, const std::string &text) : Text(f, text) {
        initDefaultVals2D();
    }

    Text2D::Text2D(Font &_f, const string &_text, bool _useSignedDistance) : Text(_f, _text, _useSignedDistance) {
//        printf("In Text2D(Font &_f, const string &_text, bool _useSignedDistance) : Text(_f, _text, _useSignedDistance)!!\n");
        initDefaultVals2D();
    }

    void Text2D::initDefaultVals2D() {
        pen(0.5, 0.5);
        justify(0, 0);
        height(0.5);
    }

    Text2D &Text2D::text(string _text) {
        updateText(_text);
        return *this; //you will need to call one of the mesh functions!
    }

    Text2D &Text2D::justify(float _jx, float _jy) {
        justifyX = _jx;
        justifyY = _jy;
        return *this;
    }

    Text2D &Text2D::pixel(float _jx, float _jy) {
        printf("in pixel... %f, %f\n", _jx, _jy);
        isPixel = true;
        penX = _jx;
        penY = _jy;
        return *this;
    }

    Text2D &Text2D::pen(float _jx, float _jy) {
        isPixel = false;
        penX = _jx;
        penY = _jy;
        return *this;
    }

    Text2D &Text2D::background(vec4 _backgroundColor) {
        return (Text2D &) setBackground(_backgroundColor);
    }

    Text2D &Text2D::color(vec4 _color) {
        // printf("in text2d color...\n");
        return (Text2D &) setColor(_color);
    }


    void Text2D::justifyText(const float sw, const float sh, const float pen_x, const float pen_y, const float j_x, const float j_y, const float bw, const float bh, float &bx0, float &bx1, float &by0, float &by1) {

        float u_px, u_py;
        if (isPixel) {
            //pixel to clip
            u_px = ((pen_x / sw) * 2.0) - 1.0;
            u_py = -1 * (((pen_y / sh) * 2.0) - 1.0);
            printf("u_px/u_py = %f/%f\n", u_px,u_py);
   } else {
            //perc 0.0 -> 1.0 ... to clip
            u_px = (pen_x * 2.0) - 1.0;
            u_py = (pen_y * 2.0) - 1.0;
        }

//        printf("u_px/u_py = %f/%f\n", u_px,u_py);

        //justify x
        float jx = (-bw / 2) - j_x * 0.5 * bw;
        bx0 = u_px + jx;
        bx1 = bx0 + bw;

        //justify y
        float jy = (-bh / 2) - j_y * 0.5 * bh;
        by0 = u_py + jy;
        by1 = by0 + bh;

    }

    ivec2 Text2D::pixels() {
        if (isPixel) {
            return ivec2((int) textWidth, (int) textHeight);
        } else {
            return ivec2(textureW, textureH); //this isn't calculated until after the text is drawn! use pixel() if you are doing fbos or need to know the size ahead of time (which makes sense anyway...)
        }
    }

    Text2D &Text2D::size(float width, float height) {
        textWidth = width;
        textHeight = height;
        isBox = true;
        return *this;
    }

    Text2D &Text2D::width(float width) {
        textWidth = width;
        textHeight = width * ((float) pixelH / (float) pixelW);
        isBox = false;
        isSetToWidth = true;

        return *this;
    }

    Text2D &Text2D::height(float height) {
        textHeight = height;
        textWidth = height * ((float) pixelW / (float) pixelH);
           printf("in height : tw/th = %f/%f\n", textWidth, textHeight);
        isBox = false;
        isSetToWidth = false;
        return *this;
    }


    Text2D &Text2D::screen(ivec4 VP) {
        screenW = VP.z;
        screenH = VP.w;
        return *this;
    }

    Text2D &Text2D::screen(int sw, int sh) {
        screenW = sw;
        screenH = sh;
        return *this;
    }


    void Text2D::draw(int sw, int sh) {
        screen(sw, sh);
        draw();
    }

    void Text2D::draw(ivec4 VP) {
        screen(VP);
        draw();
    }

    void Text2D::draw() {
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//        printf("in Text2D::draw\n");

        //mesh(screenW, screenH, scaleFont);
        //draw();

        if (textHeight <= 0.0001) {
            textHeight = font.lineHeight;
        }

        if (isPixel) {
            if (isBox) {
                scaleW = (((float) textWidth / (float) screenW) / (float) (getTextPixelWidth() + font.padding)) * 2.0;
                scaleH = (((float) textHeight / (float) screenH) / (float) (font.lineHeight)) * 2.0;
            } else {
              if (isSetToWidth) {

                scaleW = (((float) textWidth / (float) screenW) / (float) (getTextPixelWidth() + font.padding)) * 2.0;
                scaleH = (((float) textWidth / (float) screenH) / (float) (getTextPixelWidth() + font.padding)) * 2.0;

              } else {

                scaleW = (((float) textHeight / (float) screenW) / (float) (font.lineHeight)) * 2.0;
                scaleH = (((float) textHeight / (float) screenH) / (float) (font.lineHeight)) * 2.0;
              }
            }
        } else {
            if (isBox) {
                scaleW = (((float) (textWidth * screenW) / (float) screenW) / (float) (getTextPixelWidth() + font.padding)) * 2.0;
                scaleH = (((float) (textHeight * screenH) / (float) screenH) / (float) (font.lineHeight)) * 2.0;
            } else {
                if (isSetToWidth) {
                    scaleW = (((float) (textWidth * screenW) / (float) screenW) / (float) (getTextPixelWidth() + font.padding)) * 2.0;
                    scaleH = (((float) (textWidth * screenW) / (float) screenH) / (float) (getTextPixelWidth() + font.padding)) * 2.0;
                } else {
                    scaleW = (((float) (textHeight * screenH) / (float) screenW) / (float) (font.lineHeight)) * 2.0;
                    scaleH = (((float) (textHeight * screenH) / (float) screenH) / (float) (font.lineHeight)) * 2.0;
                }
            }
        }


//      printf("scaleW , scaleH = %f %f\n", scaleW , scaleH );
        //TranslateYOffset = -(fontHeight - font.base) * (yScale*0.5);

        //calculate background extent
        float bw = (getTextPixelWidth() + font.padding) * scaleW;
        float bh = font.lineHeight * scaleH; //use font.highestChar for a tighter fit

        textureW = (int) ((bw / 2.0) * screenW);
        textureH = (int) ((bh / 2.0) * screenH);

//    printf("bw/bh = %f/%f\n", bw, bh);
//    printf("tw/th = %d/%d\n", textureW, textureH);
        float bx0, bx1, by0, by1;
        //justifyText(penX, penY, justifyX, justifyY, bw, bh, bx0, bx1, by0, by1);
        justifyText(screenW, screenH, penX, penY, justifyX, justifyY, bw, bh, bx0, bx1, by0, by1);

        //draw background...
        drawBackground(vec2(bx0, by0), vec2(bx1, by1), mat4(), mat4(), mat4());
        //printf("bx0/by0 = %f/%f\n", bx1-bx0, by1-by0);

        //draw glyphs...
        float pen_x = bx0 + ((font.padding / 2) * scaleW);
        float pen_y = by0;

        //float x, y, w, h, s0, s1, t0, t1;
        float x0, x1, y0, y1, s0, s1, t0, t1;
        Glyph *glyph;

        for (size_t i = 0; i < m_text.length(); ++i) {

            if (getGlyphLocationInFontAtlas(m_text[i], glyph, pen_x, pen_y, scaleW, scaleH, x0, x1, y0, y1, s0, s1, t0, t1)) {

                drawGlyph(vec2(x0, y0), vec2(x1, y1), vec2(s0, t0), vec2(s1, t1), mat4(), mat4(), mat4());
                pen_x += glyph->xadvance * scaleW;

            } else {
                pen_x += defaultAdvance * scaleW;
            }

        }

    }


    Text::Text() {
    }

    Text::Text(Font &_f, const string &_text, bool _useSignedDistance) {
//        printf("In Text(Font &_f, const string &_text, bool _useSignedDistance)!!\n\n");
        font = _f;
        m_text = _text;

        posLoc = 0;
        texCoordLoc = 1;

        if (!defaultShadersInstalled) {
//            printf("\tdefaultShadersInstalled == FALSE!!\n\n");
            initDefaultShaders(_useSignedDistance);
        }

        initDefaultVals();


        //TEMP for testing 2D w.o. FBOs
        /*
        fbo.create(1,1); //textureW, textureH);

        mesh(vec2(0,0), vec2(meshW, meshH));
        updateTexture();
        */
    }

    //grab the mesh buffer if you want to do something with it manually
    MeshBuffer &Text::getMeshBuffer() {
        return meshBuffer;
    }

    /*
    //grab the fbo texture if you want to do something with it manually
    Texture& Text::getTexture() {
      return texture;
    }
    */

    //grab the text
    string &Text::getText() {
        return m_text;
    }


    /*
    void Text::draw() {
      draw(mat4(),mat4());
    }

    void Text::draw(mat4 M, mat4 V, mat4 P) {
      draw(V*M, P);
    }

    void Text::draw(mat4 MV, mat4 P) {

      tp.bind(); {
        glUniformMatrix4fv(tp.uniform("modelview"), 1, 0, ptr(MV));
        glUniformMatrix4fv(tp.uniform("proj"), 1, 0, ptr(P));

        glUniform1i(tp.uniform("tex0"), 0);

        texture.bind(GL_TEXTURE0); {
      meshBuffer.draw();
        } texture.unbind(GL_TEXTURE0);

      } tp.unbind();
    }
    */

    /*
    Text& Text::updateTexture() {

       printf("in updateTexture... bye\n");
    exit(0);

      if (textureW >= MAX_FBO_SIZE) {
        float ar = (float) textureH / (float) textureW;
        textureW = MAX_FBO_SIZE;
        textureH = max((int)(textureW * ar), 1);
      }

      if (textureH >= MAX_FBO_SIZE) {
        float ar = (float) textureW / (float) textureH;
        textureH = MAX_FBO_SIZE;
        textureW = max((int)(textureH * ar), 1);
      }

      if (fbo.width != textureW || fbo.height != textureH) {
        printf("... new texture...\n");
        texture = Texture(textureW, textureH, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
        fbo.replace(texture, RBO(textureW, textureH, GL_DEPTH_COMPONENT24));
      }

      fbo.bind(); {
        drawTextIntoFBO();
      } fbo.unbind();

      return *this;
    }
    */

    Text::Text(Font &_f, const string &_text) {
        Text(_f, _text, false);
    }

    void Text::initDefaultShaders(bool _useSD) {
        //glBindVertexArray( 1 ); //hack - should be able to retrieve default or current vao


//        printf("\n\nin initDefaultShaders: A1\n\n");
        //draw fbo texture to screen
        tp.create();
        tp.attach(VSH_singleTexture, GL_VERTEX_SHADER);
        glBindAttribLocation(tp.id(), posLoc, "vertexPosition");
        glBindAttribLocation(tp.id(), texCoordLoc, "vertexTexCoord");
        tp.attach(FSH_singleTexture, GL_FRAGMENT_SHADER);
        tp.link();


        printf("\n\nin initDefaultShaders: A2\n\n");

        //draw background to fbo
        bp.create();
        bp.attach(VSH_background, GL_VERTEX_SHADER);
        glBindAttribLocation(bp.id(), posLoc, "vertexPosition");
        bp.attach(FSH_background, GL_FRAGMENT_SHADER);
        bp.link();

        //draw background to screen
        bp2.create();
        bp2.attach(VSH_background2, GL_VERTEX_SHADER);
        glBindAttribLocation(bp2.id(), posLoc, "vertexPosition");
        bp2.attach(FSH_background2, GL_FRAGMENT_SHADER);
        bp2.link();

        //draw text from either signed distance atlas or texture atlas to fbo
        p2.create();
        p2.attach(VSH_font2, GL_VERTEX_SHADER);
        glBindAttribLocation(p2.id(), posLoc, "vertexPosition");
        glBindAttribLocation(p2.id(), texCoordLoc, "vertexTexCoord");
        if (_useSD) {
            p2.attach(FSH_signedDistanceFont, GL_FRAGMENT_SHADER);
        } else {
            p2.attach(FSH_textureFont, GL_FRAGMENT_SHADER);
        }
        p2.link();

//        //draw text from either signed distance atlas or texture atlas to fbo
//        p.create();
//        p.attach(VSH_font, GL_VERTEX_SHADER);
//        glBindAttribLocation(p.id(), posLoc, "vertexPosition");
//        glBindAttribLocation(p.id(), texCoordLoc, "vertexTexCoord");
//        if (_useSD) {
//            p.attach(FSH_signedDistanceFont, GL_FRAGMENT_SHADER);
//        } else {
//            p.attach(FSH_textureFont, GL_FRAGMENT_SHADER);
//        }
//        p.link();

        defaultShadersInstalled = true;
    }

    //will be required to reset mesh + texture by calling one of the mesh functions
    Text &Text::updateText(string _t) {
        m_text = _t;
        pixelW = getTextPixelWidth() + font.padding;
        pixelH = font.lineHeight;
        return *this;
    }

    //swtich to a custom glyph drawing program
    Text &Text::program(Program &_p) {
        p = _p;
//        printf("in program()\n");
        //return updateTexture();
        return *this;
    }

    //switch to custom background and glyph drawing programs
    Text &Text::programs(Program &_p, Program &_bp) {
        p = _p;
        bp = _bp;
//        printf("in programs()\n");
        //return updateTexture();
        return *this;
    }

    //set the text color - for use with default shaders
    Text &Text::setColor(vec4 _txtColor) {
        // printf("in parent color...\n");
        txtColor = _txtColor;
        //updateTexture();
        return *this;
    }

    //set the background color - for use with default shaders
    Text &Text::setBackground(vec4 _backgroundColor) {
        bgColor = _backgroundColor;
        // printf("in parent background color...\n");
        //updateTexture();
        return *this;
    }

    /*
    //private method to used to calculate meshbuffer
    Text& Text::mesh(vec2 LL, vec2 UR) {
      meshData.reset();
      addRectangle(meshData, LL, UR, vec2(0,0), vec2(1,1));
      meshBuffer.init(meshData, posLoc, -1, texCoordLoc, -1);
      return *this;
    }
    */

    void Text::initDefaultVals() {
        txtColor = vec4(1.0, 1.0, 1.0, 0.7);
        bgColor = vec4(0.3, 0.3, 0.3, 0.5);

        //  posLoc=0;
        //  texCoordLoc=1;

        /*
        //the dimensons of the mesh buffer, in model coords, updated by all the "mesh" functions
        meshW = 1.0;
        meshH = 1.0;

        //pixel dimensions of the text from the actual font altas, only changes if text changes (or switching to a tighter fit... to do)
        pixelW = getTextPixelWidth() + font.padding;
        pixelH = font.lineHeight;
        //pixelH = font.highestChar;

        //texture dimensions of the fbo. can be different that the pixelW/H, especially when using signed distance text. Will change if the mesh changes size, moves, rotates. Basically we want to match the texture size to how many pixels the text is taking up.
        textureW = pixelW;
        textureH = pixelH;

        */
        meshBuffer.init(meshData, posLoc, -1, texCoordLoc, -1);
    }

    int Text::getTextPixelWidth() {
        //loop through each char to get pixel width
        int tw = 0;
        Glyph *glyph;

        for (size_t i = 0; i < m_text.length(); ++i) {
            if (font.getGlyphs().find(m_text[i]) == font.getGlyphs().end()) {
                tw += defaultAdvance;
                continue;
            }
            glyph = font.getGlyphs()[m_text[i]];
            tw += glyph->xadvance;
        }

        return tw;
    }


    void Text::drawGlyph(vec2 vLL, vec2 vUR, vec2 tLL, vec2 tUR, mat4 M, mat4 V, mat4 P) {
//        printf("\n Text::drawGlyph!!\n");
        MeshData md;
        addRectangle(md, vLL, vUR, tLL, tUR);
        meshBuffer.update(md, posLoc, -1, texCoordLoc, -1);

        p2.bind();
        {

            //glUniformMatrix4fv(p2.uniform("modelview"), 1, 0, glm::value_ptr(V * M));
            glUniformMatrix4fv(p2.uniform("modelview"), 1, 0, glm::value_ptr(V * M));
            glUniformMatrix4fv(p2.uniform("proj"), 1, 0, glm::value_ptr(P));

            glUniform1i(p2.uniform("tex0"), 0);
            glUniform4fv(p2.uniform("textColor"), 1, glm::value_ptr(txtColor));
            font.texture.bind(GL_TEXTURE0);
            {
                meshBuffer.draw();
            }
            font.texture.unbind(GL_TEXTURE0);

        }
        p2.unbind();
    }

    /*
    //draw text into fbo
    void Text::drawGlyph(vec2 vLL, vec2 vUR, vec2 tLL, vec2 tUR) {

      MeshData md;
      addRectangle(md, vLL, vUR, tLL, tUR);
      meshBuffer.update(md, posLoc, -1, texCoordLoc, -1);

      p.bind(); {

        glUniform1i(p.uniform("tex0"), 0);
        glUniform4fv(p.uniform("textColor"), 1, glm::value_ptr(txtColor));
        font.texture.bind(GL_TEXTURE0); {
      meshBuffer.draw();
        } font.texture.unbind(GL_TEXTURE0);

      } p.unbind();
    }
    */


    void Text::drawBackground(vec2 vLL, vec2 vUR, mat4 M, mat4 V, mat4 P) {
//        printf("\n Text::drawBackground!!\n");
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);

        MeshData md;

        addRectangle(md, vLL, vUR, vec2(0, 0), vec2(1, 1));
        meshBuffer.update(md, posLoc, -1, texCoordLoc, -1);

        bp2.bind();
        {
            glUniformMatrix4fv(bp2.uniform("modelview"), 1, 0, glm::value_ptr(V * M));
            glUniformMatrix4fv(bp2.uniform("proj"), 1, 0, glm::value_ptr(P));
            glUniform4fv(bp2.uniform("bgColor"), 1, glm::value_ptr(bgColor));
            meshBuffer.draw();
        }
        bp2.unbind();
    }


    //return (x0,y0)(x1,y1) clip coords of glyph in font atlas and (s0,t0),(s1,t1) tex coords
    bool Text::getGlyphLocationInFontAtlas(const char c, Glyph *&glyph, const float pen_x, const float pen_y, const float scaleW, const float scaleH, float &x0, float &x1, float &y0, float &y1, float &s0, float &s1, float &t0, float &t1) {

        if (font.getGlyphs().find(c) == font.getGlyphs().end()) {
            //if we can't find the glyph, return false to indicate need to advance some default amount
//            printf("\n Text::getGlyphLocationInFontAtlas returned FALSE!!!\n");
            return false;
        }

        glyph = font.getGlyphs()[c];

        //LL clipcoord
        x0 = pen_x + ((glyph->xoff - font.padding / 2) * scaleW);
        y0 = pen_y + ((-font.base + glyph->yoff - glyph->h - font.padding / 2) * scaleH);
        //UR clipcoord
        x1 = x0 + ((glyph->w + font.padding) * (scaleW));
        y1 = y0 + ((glyph->h + font.padding) * (scaleH));

        //LL texcoord
        s0 = glyph->s0;
        t0 = (1.0 - glyph->t1);

        //UR texcoord
        s1 = glyph->s1;
        t1 = (1.0 - glyph->t0);
//        printf("\n Text::getGlyphLocationInFontAtlas returned TRUE!!!\n");
        return true;
    }


    /*
     //draws text into FBO
     //
     //test this out!
     void Text::drawIntoFBO(FBO& fbo) {

       fbo.bind(); {

         int textureW = fbo.texture.width;
         int textureH = fbo.texture.height;

         float scaleToTextureW = (float)textureW / (float)pixelW;
         float scaleToTextureH = (float)textureH / (float)pixelH;

         //scale from texture space to clip space
         float scaleToClipW = 1.0 / (textureW/2.0);
         float scaleToClipH = 1.0 / (textureH/2.0);

         float scaleW = scaleToTextureW * scaleToClipW;
         float scaleH = scaleToTextureH * scaleToClipH;

         //TranslateYOffset = -(fontHeight - font.base) * (yScale*0.5);
         float bx0, bx1, by0, by1;

         bx0 = -1;
         bx1 = 1;
         by0 = -1;
         by1 = 1;

         //draw background...
         drawBackground(vec2(bx0, by0), vec2(bx1, by1), mat4(), mat4(), mat4());

         //draw glyphs...
         float pen_x = bx0 + ((font.padding/2) * scaleW);
         float pen_y = by0;

         //float x, y, w, h, s0, s1, t0, t1;
         float x0, y0, x1, y1, s0, s1, t0, t1;
         Glyph* glyph;

         for( size_t i=0; i < m_text.length(); ++i) {

       if (getGlyphLocationInFontAtlas(m_text[i], glyph, pen_x, pen_y, scaleW, scaleH, x0, x1, y0, y1,s0,s1,t0,t1)) {

         drawGlyph( vec2(x0,y0), vec2(x1, y1), vec2(s0,t0), vec2(s1,t1), mat4(), mat4(), mat4() );
         pen_x += glyph->xadvance * scaleW;

       } else {
         pen_x += defaultAdvance * scaleW;
       }

         }

       } fbo.unbind();


     }
     */

    /*
    //OLD
    //draws text into FBO
    void Text::drawTextIntoFBO() {

      float scaleToTextureW = (float)textureW / (float)pixelW;
      float scaleToTextureH = (float)textureH / (float)pixelH;

      //scale from texture space to clip space
      float scaleToClipW = 1.0 / (textureW/2.0);
      float scaleToClipH = 1.0 / (textureH/2.0);

      float scaleW = scaleToTextureW * scaleToClipW;
      float scaleH = scaleToTextureH * scaleToClipH;

      //TranslateYOffset = -(fontHeight - font.base) * (yScale*0.5);
      float bx0, bx1, by0, by1;

      bx0 = -1;
      bx1 = 1;
      by0 = -1;
      by1 = 1;

      //draw background...
      drawBackground(vec2(bx0, by0), vec2(bx1, by1), mat4(), mat4(), mat4());

      //draw glyphs...
      float pen_x = bx0 + ((font.padding/2) * scaleW);
      float pen_y = by0;

      //float x, y, w, h, s0, s1, t0, t1;
      float x0, y0, x1, y1, s0, s1, t0, t1;
      Glyph* glyph;

      for( size_t i=0; i < m_text.length(); ++i) {


        if (getGlyphLocationInFontAtlas(m_text[i], glyph, pen_x, pen_y, scaleW, scaleH, x0, x1, y0, y1,s0,s1,t0,t1)) {

      drawGlyph( vec2(x0,y0), vec2(x1, y1), vec2(s0,t0), vec2(s1,t1), mat4(), mat4(), mat4() );
      pen_x += glyph->xadvance * scaleW;

        } else {
      pen_x += defaultAdvance * scaleW;
        }

      }
    }
    */

    Glyph::Glyph() {
    }

    map<char, Glyph *> &Font::getGlyphs() {
        return glyphs;
    }

    void Glyph::dump(Glyph *g) {
        cout << "glyph:'" << g->val << "' x:" << g->x << " y:" << g->y << " w: " << g->w << " h: " << g->h << " xoff:" << g->xoff << " yoff: " << g->yoff << " xadvance:" << g->xadvance << "\n";
        cout << "s0: " << g->s0 << " s1: " << g->s1 << " t0: " << g->t0 << " t1: " << g->t1 << "\n";
    }

    Font::Font() {

    }



  Font &Font::loadFont(Font &font, Texture &fontTexture, const string &_file) {
    font.texture = fontTexture;

    font.highestChar = 0;

    ifstream file;
    file.open((_file + ".fnt").c_str());

    if (!file) {
      cout << "in Font::Font : error, couldn't find file : " << _file << "\n";
      exit(0);
    }

    string line;
    while (getline(file, line)) {

      map<string, string> props;

      string token;
      istringstream tokens(line);
      while (tokens >> token) {
        size_t pos = token.find('=');
        if (pos != string::npos) {
          props[token.substr(0, pos)] = token.substr(pos + 1);
        }
      }

      if (props["letter"].size() > 0) {
        Glyph *g = Font::makeGlyph(props);
        font.glyphs.insert(pair<char, Glyph *>(g->val, g));

        if (g->h > font.highestChar) {
          font.highestChar = g->h;
        }
      }

      if (props["face"].size() > 0) {
        font.face = props["face"];
      }
      if (props["size"].size() > 0) {
        font.fontSize = atof(props["size"].c_str());
      }
      if (props["lineHeight"].size() > 0) {
        font.lineHeight = atoi(props["lineHeight"].c_str());
      }
      if (props["base"].size() > 0) {
        font.base = atoi(props["base"].c_str());
      }
      if (props["scaleW"].size() > 0) {
        font.tw = atoi(props["scaleW"].c_str());
      }
      if (props["scaleH"].size() > 0) {
        font.th = atoi(props["scaleH"].c_str());
      }
      if (props["padding"].size() > 0) {
        font.padding = atoi(props["padding"].c_str());
      }
    }

    //cout << "face: " << font.face << " fontSize: " << font.fontSize << " lineHeight: " << font.lineHeight << " base: " << font.base << " tw: " << font.tw << " th: " << font.th << "\n";

    return font;
  }


 // static Font &loadFont2(Font &font, Texture &texture, const std::string &_file);
  Font &Font::loadFont2(Font &font, Texture &fontTexture, const string &contents) {
        font.texture = fontTexture;

        font.highestChar = 0;


//        ifstream file;
//        file.open((_file + ".fnt").c_str());
//
//        if (!file) {
//            cout << "in Font::Font : error, couldn't find file : " << _file << "\n";
//            exit(0);
//        }


      cout << "contents = \n\n" << contents << "\n";

      std::istringstream lines( contents );

        string line;

      while ( getline( lines, line ) ) {


       // while (getline(file, line)) {

            map<string, string> props;

            string token;
            istringstream tokens(line);
            while (tokens >> token) {
                size_t pos = token.find('=');
                if (pos != string::npos) {
                    props[token.substr(0, pos)] = token.substr(pos + 1);
                }
            }

            if (props["letter"].size() > 0) {
                Glyph *g = Font::makeGlyph(props);
                font.glyphs.insert(pair<char, Glyph *>(g->val, g));

                if (g->h > font.highestChar) {
                    font.highestChar = g->h;
                }
            }

            if (props["face"].size() > 0) {
                font.face = props["face"];
            }
            if (props["size"].size() > 0) {
                font.fontSize = atof(props["size"].c_str());
            }
            if (props["lineHeight"].size() > 0) {
                font.lineHeight = atoi(props["lineHeight"].c_str());
            }
            if (props["base"].size() > 0) {
                font.base = atoi(props["base"].c_str());
            }
            if (props["scaleW"].size() > 0) {
                font.tw = atoi(props["scaleW"].c_str());
            }
            if (props["scaleH"].size() > 0) {
                font.th = atoi(props["scaleH"].c_str());
            }
            if (props["padding"].size() > 0) {
                font.padding = atoi(props["padding"].c_str());
            }
        }

//        cout << "face: " << font.face << " fontSize: " << font.fontSize << " lineHeight: " << font.lineHeight << " base: " << font.base << " tw: " << font.tw << " th: " << font.th << "\n";

        return font;
    }

    Glyph *Font::makeGlyph(map<string, string> &props) {
//        printf("In makeGlyph!!! %s\n", props["letter"].c_str());
        Glyph *g = new Glyph();
        g->val = (char) (atoi(props["id"].c_str()));
        g->x = atoi(props["x"].c_str());
        g->y = atoi(props["y"].c_str());
        g->w = atoi(props["width"].c_str());
        g->h = atoi(props["height"].c_str());
        g->xoff = atoi(props["xoffset"].c_str());
        g->yoff = atoi(props["yoffset"].c_str());
        g->xadvance = atoi(props["xadvance"].c_str());
        g->s0 = atof(props["s0"].c_str());
        g->s1 = atof(props["s1"].c_str());
        g->t0 = atof(props["t0"].c_str());
        g->t1 = atof(props["t1"].c_str());

        return g;
    }
}

