#include "Aluminum/MeshBuffer.hpp"

namespace aluminum {


  MeshBuffer::MeshBuffer() {
  /*
#ifdef BUILD_IOS
    glGenVertexArraysOES( 1, vao );
#else
    glGenVertexArrays(1, vao);
#endif
    glGenBuffers(4, vbo);
    glGenBuffers(1, ibo);
    */
  }

  MeshBuffer MeshBuffer::init(MeshData _data, int _p, int _n, int _tc, int _c) {

#ifdef BUILD_IOS
      glGenVertexArraysOES( 1, vao );
#else
      glGenVertexArrays(1, vao);
#endif
      glGenBuffers(4, vbo);
      glGenBuffers(1, ibo);

      return update(_data, _p, _n, _tc, _c);
  }



  MeshBuffer MeshBuffer::update(MeshData _data, int _p, int _n, int _tc, int _c) {
      pLoc = _p;
      nLoc = _n;
      tcLoc = _tc;
      cLoc = _c;

      return update(_data);
  }



  MeshBuffer MeshBuffer::update(MeshData _data) {

      data = _data;

#ifdef BUILD_IOS
    glBindVertexArrayOES( vao[0] ); {
#else
      glBindVertexArray(vao[0]);
      {
#endif
          checkAttributes(pLoc, nLoc, tcLoc, cLoc);

          initVBO(vbo[0], pLoc, 3, data.vertices());

          if (useNormals) {
              initVBO(vbo[1], nLoc, 3, data.normals());
          }

          if (useTexCoords) {
              initVBO(vbo[2], tcLoc, 3, data.texCoords());
          }

          if (useColors) {
              initVBO(vbo[3], cLoc, 4, data.colors());
          }

          if (data.indices().size() > 0) {
              initIBO();
          }

      }

#ifdef BUILD_IOS
      glBindVertexArrayOES(0);
#else
      glBindVertexArray(0);
#endif
      return *this;
  }


  void MeshBuffer::checkAttributes(int p, int n, int tc, int c) {
      if (p < 0) {
          printf("error : in MeshBuffer::checkAttributes. The mesh must have vertex locations!\n");
          exit(0);
      }

      useNormals = (n >= 0) ? true : false;
      useTexCoords = (tc >= 0) ? true : false;
      useColors = (c >= 0) ? true : false;
  }


  //using this template to pass in either a vector of vec3s or vec4s
  template<class T> void MeshBuffer::initVBO(GLuint vboID, int loc, int size, T b) {

      glBindBuffer(GL_ARRAY_BUFFER, vboID);
      glBufferData(GL_ARRAY_BUFFER, size * sizeof(float) * b.size(), NULL, GL_DYNAMIC_DRAW);
      glBufferData(GL_ARRAY_BUFFER, size * sizeof(float) * b.size(), &(b[0]), GL_DYNAMIC_DRAW);

      glEnableVertexAttribArray(loc);
      glVertexAttribPointer(loc, size, GL_FLOAT, GL_FALSE, 0, 0);
  }


  void MeshBuffer::initIBO() {

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * data.indices().size(), NULL, GL_DYNAMIC_DRAW);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * data.indices().size(), &(data.indices()[0]), GL_DYNAMIC_DRAW);
      useIndices = true;
  }




  void MeshBuffer::draw() {

#ifdef BUILD_IOS
    glBindVertexArrayOES( vao[0] ); {

      if (useIndices) {
        glDrawElements(GL_TRIANGLES, (GLsizei) data.indices().size(), GL_UNSIGNED_INT, 0 );
      } else {
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei) data.vertices().size() * 9 ); //double check this!
        //printf("here...\n");
      }

    }
    /* //double check, but why would we need both of these???
     glBindVertexArrayOES(0); {

     if (useIndices) {
     glDrawElements(GL_TRIANGLES, (GLsizei) data.indices().size(), GL_UNSIGNED_INT, 0);
     } else {
     glDrawArrays(GL_TRIANGLES, 0, (GLsizei) data.vertices().size() * 9 ); //double check this!
     //printf("here...\n");
     }
     } glBindVertexArrayOES(0);
     */
#else
      glBindVertexArray(vao[0]);
      {
          if (useIndices) {
            //std::cout << "in MeshBuffer - useIndices = true... data.indices().size() = " << data.indices().size() << " \n";
              glDrawElements(GL_TRIANGLES, (GLsizei) data.indices().size(), GL_UNSIGNED_INT, 0);
          } else {
            //std::cout << "data.vertices().size() = " << data.vertices().size() << "\n";
              glDrawArrays(GL_TRIANGLES, 0, (GLsizei) data.vertices().size()); //double check this!
              //glDrawArrays(GL_TRIANGLES, 0, 3); //double check this!
              //printf("here...\n");
          }
      }
#endif

  }




    void MeshBuffer::drawPoints() {

#ifdef BUILD_IOS

      glBindVertexArrayOES( vao[0] );
      {
        if (useIndices) {
          glDrawElements(GL_POINTS, (GLsizei) data.indices().size(), GL_UNSIGNED_INT, 0);         }
        else {
            //glDrawArrays(GL_TRIANGLES, 0, (GLsizei) data.vertices().size() * 9 ); //double check this!
            //printf("here...\n");
          }
      }

#else
      glBindVertexArray(vao[0]);
      {
        if (useIndices) {
        //  printf("num points = %d\n", (GLsizei) data.indices().size());

          glDrawElements(GL_POINTS, (GLsizei) data.indices().size(), GL_UNSIGNED_INT, 0);
        } else {
          //glDrawArrays(GL_LINES, 0, (GLsizei) data.vertices().size() * 7 ); //double check this!
          //printf("A here...\n");
        }
      }

#endif

    }



  void MeshBuffer::drawLines() {

#ifdef BUILD_IOS

    glBindVertexArrayOES( vao[0] );
    {
      if (useIndices) {
        glDrawElements(GL_LINES, (GLsizei) data.indices().size(), GL_UNSIGNED_INT, 0);         } else {
          //glDrawArrays(GL_TRIANGLES, 0, (GLsizei) data.vertices().size() * 9 ); //double check this!
          //printf("here...\n");
        }
    }

#else
      glBindVertexArray(vao[0]);
      {
          if (useIndices) {
              glDrawElements(GL_LINES, (GLsizei) data.indices().size(), GL_UNSIGNED_INT, 0);
          } else {
              //glDrawArrays(GL_LINES, 0, (GLsizei) data.vertices().size() * 7 ); //double check this!
              //printf("A here...\n");
          }
      }

#endif

  }


  void MeshBuffer::drawTriangleStrip() {


#ifdef BUILD_IOS

    glBindVertexArrayOES( vao[0] );
    {
      if (useIndices) {
        glDrawElements(GL_TRIANGLE_STRIP, (GLsizei) data.indices().size(), GL_UNSIGNED_INT, 0);         } else {
          //glDrawArrays(GL_TRIANGLES, 0, (GLsizei) data.vertices().size() * 9 ); //double check this!
          //printf("here...\n");
        }
    }


#else
      glBindVertexArray(vao[0]);
      {
          if (useIndices) {
              glDrawElements(GL_TRIANGLE_STRIP, (GLsizei) data.indices().size(), GL_UNSIGNED_INT, 0);
          } else {
              //glDrawArrays(GL_LINES, 0, (GLsizei) data.vertices().size() * 7 ); //double check this!
              //printf("A here...\n");
          }
      }

#endif

  }

/*
  void MeshBuffer::drawPoints() {

#ifdef BUILD_IOS

    glBindVertexArrayOES( vao[0] );
    {
      //       glDrawArrays(GL_POINTS, 0, (GLsizei) data.vertices().size() * sizeof(vec3) ); //double check this!
      glDrawArrays(GL_POINTS, 0, (GLsizei) data.vertices().size() ); //double check this!
    }


#else

      glBindVertexArray(vao[0]);
      {

          glDrawArrays(GL_POINTS, 0, (GLsizei) data.vertices().size() * 7); //double check this!


      }

#endif

  }
*/
}
