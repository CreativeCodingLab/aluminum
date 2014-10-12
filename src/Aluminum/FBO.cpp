#include "Aluminum/FBO.hpp"

#ifdef BUILD_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

namespace aluminum {
  
  ///////////////////////////////////////////////////////////////////////////////
  // convert OpenGL 3.2 core profile's internal format enum to string
  ///////////////////////////////////////////////////////////////////////////////
  std::string convertInternalFormatToString(GLenum format) {
    std::string formatName;
    
    switch (format) {
        //Table 3.12: sized internal color formats
#ifndef BUILD_IOS
        
      case GL_R8:
        formatName = "GL_R8";
        break;
      case GL_R8_SNORM:
        formatName = "GL_R8_SNORM";
        break;
      case GL_R16:
        formatName = "GL_R16";
        break;
      case GL_R16_SNORM:
        formatName = "GL_R16_SNORM";
        break;
      case GL_RG8:
        formatName = "GL_RG8";
        break;
      case GL_RG8_SNORM:
        formatName = "GL_RG8_SNORM";
        break;
      case GL_RG16:
        formatName = "GL_RG16";
        break;
      case GL_RG16_SNORM:
        formatName = "GL_RG16_SNORM";
        break;
      case GL_R3_G3_B2:
        formatName = "GL_R2_G3_B2";
        break;
      case GL_RGB4:
        formatName = "GL_RGB4";
        break;
      case GL_RGB5:
        formatName = "GL_RGB5";
        break;
      case GL_RGB8:
        formatName = "GL_RGB8";
        break;
      case GL_RGB8_SNORM:
        formatName = "GL_RGB8_SNORM";
        break;
      case GL_RGB10:
        formatName = "GL_RGB10";
        break;
      case GL_RGB12:
        formatName = "GL_RGB12";
        break;
      case GL_RGB16:
        formatName = "GL_RGB16";
        break;
      case GL_RGB16_SNORM:
        formatName = "GL_RGB16_SNORM";
        break;
      case GL_RGBA2:
        formatName = "GL_RGBA2";
        break;
      case GL_RGBA8:
        formatName = "GL_RGBA8";
        break;
      case GL_RGBA8_SNORM:
        formatName = "GL_RGBA8_SNORM";
        break;
      case GL_RGB10_A2:
        formatName = "GL_RGB10_A2";
        break;
      case GL_RGBA12:
        formatName = "GL_RGBA12";
        break;
      case GL_RGBA16:
        formatName = "GL_RGBA16";
        break;
      case GL_RGBA16_SNORM:
        formatName = "GL_RGBA16_SNORM";
        break;
      case GL_SRGB8:
        formatName = "GL_SRGB8";
        break;
      case GL_SRGB8_ALPHA8:
        formatName = "GL_SRGB8_ALPHA8";
        break;
      case GL_R16F:
        formatName = "GL_R16F";
        break;
      case GL_RG16F:
        formatName = "GL_RG16F";
        break;
      case GL_RGB16F:
        formatName = "GL_RGB16F";
        break;
      case GL_RGBA16F:
        formatName = "GL_RGBA16F";
        break;
      case GL_R32F:
        formatName = "GL_R32F";
        break;
      case GL_RG32F:
        formatName = "GL_RG32F";
        break;
      case GL_RGB32F:
        formatName = "GL_RGB32F";
        break;
      case GL_RGBA32F:
        formatName = "GL_RGBA32F";
        break;
      case GL_R11F_G11F_B10F:
        formatName = "GL_R11F_G11F_B10F";
        break;
      case GL_RGB9_E5:
        formatName = "GL_RGB9_E5";
        break;
      case GL_R8I:
        formatName = "GL_R8I";
        break;
      case GL_R8UI:
        formatName = "GL_R8UI";
        break;
        
      case GL_R16I:
        formatName = "GL_R16I";
        break;
      case GL_R16UI:
        formatName = "GL_R16UI";
        break;
      case GL_R32I:
        formatName = "GL_R32I";
        break;
      case GL_R32UI:
        formatName = "GL_R32UI";
        break;
      case GL_RG8I:
        formatName = "GL_RG8I";
        break;
      case GL_RG8UI:
        formatName = "GL_RG8UI";
        break;
      case GL_RG16I:
        formatName = "GL_RG16I";
        break;
      case GL_RG32I:
        formatName = "GL_RG32I";
        break;
      case GL_RG32UI:
        formatName = "GL_RG32UI";
        break;
      case GL_RGB8I:
        formatName = "GL_RGB8I";
        break;
      case GL_RGB8UI:
        formatName = "GL_RGB8UI";
        break;
      case GL_RGB16I:
        formatName = "GL_RGB16I";
        break;
      case GL_RGB16UI:
        formatName = "GL_RGB16UI";
        break;
      case GL_RGB32I:
        formatName = "GL_RGB32I";
        break;
      case GL_RGB32UI:
        formatName = "GL_RGB32UI";
        break;
      case GL_RGBA8I:
        formatName = "GL_RGBA8I";
        break;
      case GL_RGBA8UI:
        formatName = "GL_RGBA8UI";
        break;
      case GL_RGBA16I:
        formatName = "GL_RGBA16I";
        break;
      case GL_RGBA16UI:
        formatName = "GL_RGBA16UI";
        break;
      case GL_RGBA32I:
        formatName = "GL_RGBA32I";
        break;
      case GL_RGBA32UI:
        formatName = "GL_RGBA32UI";
        break;
        
      case GL_RGBA4:
        formatName = "GL_RGBA4";
        break;
      case GL_RGB5_A1:
        formatName = "GL_RGB5_A1";
        break;
        
        //Table 3.13: sized interal depth formats
      case GL_DEPTH_COMPONENT16:
        formatName = "GL_DEPTH_COMPONENT16";
        break;
      case GL_DEPTH_COMPONENT24:
        formatName = "GL_DEPTH_COMPONENT24";
        break;
      case GL_DEPTH_COMPONENT32:
        formatName = "GL_DEPTH_COMPONENT32";
        break;
      case GL_DEPTH_COMPONENT32F:
        formatName = "GL_DEPTH_COMPONENT32F";
        break;
      case GL_DEPTH24_STENCIL8:
        formatName = "GL_DEPTH24_STENCIL8";
        break;
      case GL_DEPTH32F_STENCIL8:
        formatName = "GL_DEPTH32F_STENCIL8";
        break;
        
        //Table 4.10: sized internal formats for formats that can only be used with renderbuffers
      case GL_STENCIL_INDEX1:
        formatName = "GL_STENCIL_INDEX1";
        break;
      case GL_STENCIL_INDEX4:
        formatName = "GL_STENCIL_INDEX4";
        break;
      case GL_STENCIL_INDEX8:
        formatName = "GL_STENCIL_INDEX8";
        break;
      case GL_STENCIL_INDEX16:
        formatName = "GL_STENCIL_INDEX16";
        break;
        
        //Table 3.3: pixel data formats (these are not actually internal formats...)
      case GL_STENCIL_INDEX:
        formatName = "GL_STENCIL_INDEX";
        break;
      case GL_DEPTH_COMPONENT:
        formatName = "GL_DEPTH_COMPONENT";
        break;
      case GL_DEPTH_STENCIL:
        formatName = "GL_DEPTH_STENCIL";
        break;
      case GL_RED:
        formatName = "GL_RED";
        break;
      case GL_GREEN:
        formatName = "GL_GREEN";
        break;
      case GL_BLUE:
        formatName = "GL_BLUE";
        break;
      case GL_RG:
        formatName = "GL_RG";
        break;
      case GL_RGB:
        formatName = "GL_RGB";
        break;
      case GL_RGBA:
        formatName = "GL_RGBA";
        break;
      case GL_BGR:
        formatName = "GL_BGR";
        break;
      case GL_BGRA:
        formatName = "GL_BGRA";
        break;
      case GL_RED_INTEGER:
        formatName = "GL_RED_INTEGER";
        break;
      case GL_GREEN_INTEGER:
        formatName = "GL_GREEN_INTEGER";
        break;
      case GL_BLUE_INTEGER:
        formatName = "GL_BLUE_INTEGER";
        break;
      case GL_RG_INTEGER:
        formatName = "GL_RG_INTEGER";
        break;
      case GL_RGB_INTEGER:
        formatName = "GL_RGB_INTEGER";
        break;
      case GL_RGBA_INTEGER:
        formatName = "GL_RGBA_INTEGER";
        break;
      case GL_BGR_INTEGER:
        formatName = "GL_BGR_INTEGER";
        break;
      case GL_BGRA_INTEGER:
        formatName = "GL_BGRA_INTEGER";
        break;
#endif
      default:
        formatName = "Unknown Format";
        break;
    }
    
    return formatName;
  }
  
  
  ///////////////////////////////////////////////////////////////////////////////
  // return texture parameters as string using glGetTexLevelParameteriv()
  ///////////////////////////////////////////////////////////////////////////////
  std::string getTextureParameters(GLuint id) {
#ifndef BUILD_IOS
    if (glIsTexture(id) == GL_FALSE)
      return "Not texture object";
    
    int width, height, format;
    std::string formatName;
    glBindTexture(GL_TEXTURE_2D, id);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);            // get texture width
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);          // get texture height
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format); // get texture internal format
    glBindTexture(GL_TEXTURE_2D, 0);
    
    formatName = convertInternalFormatToString(format);
    
    std::stringstream ss;
    ss << width << "x" << height << ", " << formatName << "(" << format << ")";
    
    return ss.str();
#else
    return "not supported in IOS OpenGLES2.0";
#endif
  }
  
  
  
  ///////////////////////////////////////////////////////////////////////////////
  // return renderbuffer parameters as string using glGetRenderbufferParameteriv
  ///////////////////////////////////////////////////////////////////////////////
  std::string getRenderbufferParameters(GLuint id) {
    if (glIsRenderbuffer(id) == GL_FALSE)
      return "Not Renderbuffer object";
    
    int width, height, format;
    std::string formatName;
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);    // get renderbuffer width
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);  // get renderbuffer height
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, &format); // get renderbuffer internal format
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    formatName = convertInternalFormatToString(format);
    
    std::stringstream ss;
    ss << width << "x" << height << ", " << formatName << "(" << format << ")";
    return ss.str();
  }
  
  
  void printFramebufferInfo() {
#ifndef BUILD_IOS
    std::cout << "\n===== FBO STATUS =====\n";
    
    // print max # of colorbuffers supported by FBO
    int colorBufferCount = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &colorBufferCount);
    std::cout << "Max Number of Color Buffer Attachment Points: " << colorBufferCount << std::endl;
    
    int objectType;
    int objectId;
    
    // print info of the colorbuffer attachable image
    for (int i = 0; i < colorBufferCount; ++i) {
      glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                            GL_COLOR_ATTACHMENT0+ i,
                                            GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                            &objectType);
      if (objectType != GL_NONE) {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                              GL_COLOR_ATTACHMENT0+ i,
                                              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                              &objectId);
        
        std::string formatName;
        
        std::cout << "Color Attachment " << i << ": ";
        if (objectType == GL_TEXTURE) {
          std::cout << "GL_TEXTURE, " << getTextureParameters(objectId) << std::endl;
        }
        else if (objectType == GL_RENDERBUFFER) {
          std::cout << "GL_RENDERBUFFER, " << getRenderbufferParameters(objectId) << std::endl;
        }
      }
    }
    
    // print info of the depthbuffer attachable image
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                          GL_DEPTH_ATTACHMENT,
                                          GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                          &objectType);
    if (objectType != GL_NONE) {
      glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                            GL_DEPTH_ATTACHMENT,
                                            GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                            &objectId);
      
      std::cout << "Depth Attachment: ";
      switch (objectType) {
        case GL_TEXTURE:
          std::cout << "GL_TEXTURE, " << getTextureParameters(objectId) << std::endl;
          break;
        case GL_RENDERBUFFER:
          std::cout << "GL_RENDERBUFFER, " << getRenderbufferParameters(objectId) << std::endl;
          break;
      }
    }
    
    // print info of the stencilbuffer attachable image
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                          GL_STENCIL_ATTACHMENT,
                                          GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                          &objectType);
    if (objectType != GL_NONE) {
      glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                            GL_STENCIL_ATTACHMENT,
                                            GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                            &objectId);
      
      std::cout << "Stencil Attachment: ";
      switch (objectType) {
        case GL_TEXTURE:
          std::cout << "GL_TEXTURE, " << getTextureParameters(objectId) << std::endl;
          break;
        case GL_RENDERBUFFER:
          std::cout << "GL_RENDERBUFFER, " << getRenderbufferParameters(objectId) << std::endl;
          break;
      }
    }
    
    std::cout << std::endl;
#endif
  }
  
  
  FBO::FBO() {
    use = false;
  }
  
  FBO &FBO::create() {
    glGenFramebuffers(1, &fboID);
    return *this;
  }
  
  
#ifdef BUILD_IOS
  FBO& FBO::create(int w, int h) {
    glGenFramebuffers(1, &fboID);
    return attach(Texture(w, h, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE), RBO(w, h, GL_DEPTH_COMPONENT16));
  }
#else
  
  
  FBO &FBO::create(int w, int h) {
    glGenFramebuffers(1, &fboID);
    return attach(Texture(w, h, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE), RBO(w, h, GL_DEPTH_COMPONENT24));
    //        return attach(Texture(w, h, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE), RBO(w, h, GL_DEPTH_COMPONENT32F));
  }
  
#endif
  
  
  FBO &FBO::create(Texture t) {
    glGenFramebuffers(1, &fboID);
    return attach(t);
  }
  
  FBO &FBO::create(Texture t, RBO rb) {
    glGenFramebuffers(1, &fboID);
    return attach(t, rb);
  }
  
  
  // Creates an FBO with multiple render targets. Assumes that the targets are 2D, the same size, and have no attached depth texture.
  FBO &FBO::create(std::vector<Texture> ts) {
    glGenFramebuffers(1, &fboID);
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);

    if (ts.size() < 1) {
      std::cerr << "In FBO : Can't create a multirender target with no textures! " << std::endl;
      exit(0);
    }
    
    //assuming sizes of all targets is the same
    width = ts[0].width;
    height = ts[1].height;

    
    GLenum draw_buffers[ts.size()];
    
    for (int i = 0; i < ts.size(); i++) {
    
      glBindTexture(GL_TEXTURE_2D, ts[i].id());
      glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, ts[i].id(), 0);
      draw_buffers[i] = GL_COLOR_ATTACHMENT0 + i;
    }
    
    glDrawBuffers((GLsizei)ts.size(), draw_buffers);

    //test FBO
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboID);
    GLenum e = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
    if (e != GL_FRAMEBUFFER_COMPLETE) {
      printf("There is a problem with the multiple render target FBO\n");
      exit(0);
    }
    
    //return to normal
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    
    return *this;
    //return attach(ts[0]);
  }
  
  
  
  
  FBO &FBO::replace(Texture t, RBO rb) {
    //delete old, and attach new
    texture.destroy();
    rbo.destroy();
    return attach(t, rb);
  }
  
  FBO &FBO::attach(Texture t, RBO rb) {
    
    
#ifdef BUILD_IOS
    
    texture = t;
    rbo = rb;
    width = texture.width;
    height = texture.height;
    
    //1. bind FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fboID); {
      
      //2. attach texture (as color attachment) to fbo
      texture.bind(); {
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER_APPLE, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id(), 0);
        //     glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id(), 0);
        
      } texture.unbind();
      
      
      /*
       //3. attach renderbuffer (as depth attachment) to fbo
       rb.bind(); {
       glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER_APPLE, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb.id());
       } rb.unbind();
       */
      
      checkStatus();
      
      //4. unbind FBO
    } glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    return *this;
#else
    
    
    texture = t;
    rbo = rb;
    width = texture.width;
    height = texture.height;
    
    //1. bind FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);
    {
      
      //2. attach texture (as color attachment) to fbo
      texture.bind();
      {
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id(), 0);
      }
      texture.unbind();
      
      //3. attach renderbuffer (as depth attachment) to fbo
      rb.bind();
      {
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb.id());
      }
      rb.unbind();
      
      checkStatus();
      
      //4. unbind FBO
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    return *this;
#endif
  }
  
  
  FBO &FBO::replace(Texture t) {
    texture.destroy();
    return attach(t);
  }
  
#ifdef BUILD_IOS
  FBO& FBO::attach(Texture t) {
    
    texture = t;
    width = texture.width;
    height = texture.height;
    
    //1. bind FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fboID); {
      
      //2. attach texture (as color attachment) to fbo
      texture.bind(); {
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER_APPLE, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id(), 0);
      } texture.unbind();
      
      checkStatus();
      
      //3. unbind FBO
    } glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    return *this;
  }
  
#else
  
  FBO &FBO::attach(Texture t) {
    
    texture = t;
    width = texture.width;
    height = texture.height;
    
    //1. bind FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);
    {
      
      //2. attach texture (as color attachment) to fbo
      texture.bind();
      {
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id(), 0);
      }
      texture.unbind();
      
      checkStatus();
      
      //3. unbind FBO
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    return *this;
  }
  
#endif
  
  void FBO::checkStatus() {
    /* assumes that the FBO is bound */
    
    //printFramebufferInfo();
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      printf("in FBO : ERROR!!!!: Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
      exit(0);
    }
  }
  
  //use this one if you are changing the fbo texture..
  FBO &FBO::bind(Texture t) {
    attach(t);
    glViewport(0, 0, width, height); //w & h MUST match the texture size
    return *this;
  }
  
  //use this one if you are changing the fbo texture or rbo..
  FBO &FBO::bind(Texture t, RBO rb) {
    attach(t, rb);
    glViewport(0, 0, width, height); //w & h MUST match the texture size
    return *this;
  }
  
  FBO &FBO::bind() {
    //printf("binding to fbo id = %d\n", fboID);
    // printf("fbo w/h = %d/%d\n", width, height);
    // printf("fbo texture w/h = %d/%d\n", texture.width, texture.height);
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);
    glViewport(0, 0, width, height); //w & h SHOULD match the texture size
    return *this;
  }
  
  FBO &FBO::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return *this;
  }
  
  void FBO::unbindAll() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
  }
  
  
  RBO::RBO() {
    use = false;
  }
  
  RBO::RBO(int _width, int _height, GLenum _format) {
    use = false;
    width = _width;
    height = _height;
    internalformat = _format;
    create();
  }
  
  RBO &RBO::create() {
    glGenRenderbuffers(1, &rboID);
    bind();
    {
      glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
    }
    unbind();
    
    return *this;
  }
  
  void RBO::destroy() {
    glDeleteRenderbuffers(1, &rboID);
  }
  
  
  GLint RBO::id() {
    return rboID;
  }
  
  RBO &RBO::bind() {
    glBindRenderbuffer(GL_RENDERBUFFER, rboID);
    return *this;
  }
  
  RBO &RBO::unbind() {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    return *this;
  }
  
};

