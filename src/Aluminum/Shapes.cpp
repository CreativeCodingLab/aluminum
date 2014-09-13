#include "Aluminum/Shapes.hpp"

namespace aluminum {

    using glm::to_string;
    using glm::vec2;
    using glm::vec3;
    using glm::vec4;
    using glm::mat4;

    //const double phi = (1 + sqrt(5))/2; // the golden ratio

    int addRectangle(MeshData &m, vec2 vLL, vec2 vUR, vec2 tcLL, vec2 tcUR) {

        vec3 v0 = vec3(vLL.x, vLL.y, 0);
        vec3 v1 = vec3(vLL.x, vUR.y, 0);
        vec3 v2 = vec3(vUR.x, vLL.y, 0);
        vec3 v3 = vec3(vUR.x, vUR.y, 0);

        vec3 t0 = vec3(tcLL.x, tcLL.y, 0);
        vec3 t1 = vec3(tcLL.x, tcUR.y, 0);
        vec3 t2 = vec3(tcUR.x, tcLL.y, 0);
        vec3 t3 = vec3(tcUR.x, tcUR.y, 0);

        m.vertex(v0);
        m.texCoord(t0);
        m.vertex(v1);
        m.texCoord(t1);
        m.vertex(v2);
        m.texCoord(t2);
        m.vertex(v3);
        m.texCoord(t3);

        int Nv = 4;

        static const unsigned int indices[] = {
                0, 1, 2, 2, 1, 3
        };

        //m.index(indices, sizeof(indices)/sizeof(*indices), m.vertices().size()-Nv);
        m.index(indices, 6);

        return Nv;
    }


    int addRectangle(MeshData &m, float _w, float _h) {

        float w = _w / 2.0;
        float h = _h / 2.0;

        vec3 v0 = vec3(-w, -h, 0);
        vec3 v1 = vec3(-w, h, 0);
        vec3 v2 = vec3(w, -h, 0);
        vec3 v3 = vec3(w, h, 0);

        vec3 t0 = vec3(0, 0, 0);
        vec3 t1 = vec3(0, 1, 0);
        vec3 t2 = vec3(1, 0, 0);
        vec3 t3 = vec3(1, 1, 0);

        m.vertex(v0);
        m.texCoord(t0);
        m.vertex(v1);
        m.texCoord(t1);
        m.vertex(v2);
        m.texCoord(t2);
        m.vertex(v3);
        m.texCoord(t3);

        int Nv = 4;

        //static const int indices[] = {
        static const unsigned int indices[] = {
                0, 1, 2, 2, 1, 3
        };

        //m.index(indices, sizeof(indices)/sizeof(*indices), m.vertices().size()-Nv);
        m.index(indices, 6);

        return Nv;
    }


    int addRectangle(MeshData &m) {

        vec3 v0 = vec3(-1, -1, 0);
        vec3 v1 = vec3(-1, 1, 0);
        vec3 v2 = vec3(1, -1, 0);
        vec3 v3 = vec3(1, 1, 0);

        vec3 t0 = vec3(0, 0, 0);
        vec3 t1 = vec3(0, 1, 0);
        vec3 t2 = vec3(1, 0, 0);
        vec3 t3 = vec3(1, 1, 0);

        m.vertex(v0);
        m.texCoord(t0);
        m.vertex(v1);
        m.texCoord(t1);
        m.vertex(v2);
        m.texCoord(t2);
        m.vertex(v3);
        m.texCoord(t3);

        int Nv = 4;

        //static const int indices[] = {
        static const unsigned int indices[] = {
                0, 1, 2, 2, 1, 3
        };

        //m.index(indices, sizeof(indices)/sizeof(*indices), m.vertices().size()-Nv);
        m.index(indices, 6);

        return Nv;
    }


  //this one makes normals, texcoords, and colors...
  int addCube(MeshData &m, float s) {
    
    //8 vertices
    vec3 v0 = vec3(-s, -s, s);
    vec3 v1 = vec3(-s, s, s);
    vec3 v2 = vec3(s, -s, s);
    vec3 v3 = vec3(s, s, s);
    vec3 v4 = vec3(-s, -s, -s);
    vec3 v5 = vec3(-s, s, -s);
    vec3 v6 = vec3(s, -s, -s);
    vec3 v7 = vec3(s, s, -s);
    
    //6 normals
    vec3 n0 = glm::normalize(vec3(0, 0, s));
    vec3 n1 = glm::normalize(vec3(0, 0, -s));
    vec3 n2 = glm::normalize(vec3(0, s, 0));
    vec3 n3 = glm::normalize(vec3(0, -s, 0));
    vec3 n4 = glm::normalize(vec3(s, 0, 0));
    vec3 n5 = glm::normalize(vec3(-s, 0, 0));
    
    //4 texcoords
    vec3 t0 = vec3(0, 0, 0);
    vec3 t1 = vec3(0, 1, 0);
    vec3 t2 = vec3(1, 0, 0);
    vec3 t3 = vec3(1, 1, 0);

    //6 colors
    vec3 c0 = vec3(1.0,0.0,0.0);
    vec3 c1 = vec3(0.0,1.0,0.0);
    vec3 c2 = vec3(0.0,0.0,1.0);
    vec3 c3 = vec3(1.0,1.0,0.0);
    vec3 c4 = vec3(1.0,0.0,1.0);
    vec3 c5 = vec3(0.0,1.0,1.0);
    
    
    //right
    m.vertex(v2);
    m.normal(n4);
    m.texCoord(t0);
    m.color(c4);
    
    m.vertex(v3);
    m.normal(n4);
    m.texCoord(t1);
    m.color(c4);
    
    m.vertex(v6);
    m.normal(n4);
    m.texCoord(t2);
    m.color(c4);
    
    m.vertex(v6);
    m.normal(n4);
    m.texCoord(t2);
    m.color(c4);
    
    m.vertex(v3);
    m.normal(n4);
    m.texCoord(t1);
    m.color(c4);
    
    m.vertex(v7);
    m.normal(n4);
    m.texCoord(t3);
    m.color(c4);
    
    //left
    m.vertex(v4);
    m.normal(n5);
    m.texCoord(t0);
    m.color(c5);
    
    m.vertex(v5);
    m.normal(n5);
    m.texCoord(t1);
    m.color(c5);
    
    m.vertex(v0);
    m.normal(n5);
    m.texCoord(t2);
    m.color(c5);
    
    m.vertex(v0);
    m.normal(n5);
    m.texCoord(t2);
    m.color(c5);
    
    m.vertex(v5);
    m.normal(n5);
    m.texCoord(t1);
    m.color(c5);
    
    m.vertex(v1);
    m.normal(n5);
    m.texCoord(t3);
    m.color(c5);
    
    //front
    m.vertex(v0);
    m.normal(n0);
    m.texCoord(t0);
    m.color(c0);
    
    m.vertex(v1);
    m.normal(n0);
    m.texCoord(t1);
    m.color(c0);
    
    m.vertex(v2);
    m.normal(n0);
    m.texCoord(t2);
    m.color(c0);
    
    m.vertex(v2);
    m.normal(n0);
    m.texCoord(t2);
    m.color(c0);
    
    m.vertex(v1);
    m.normal(n0);
    m.texCoord(t1);
    m.color(c0);
    
    m.vertex(v3);
    m.normal(n0);
    m.texCoord(t3);
    m.color(c0);
    
    
    //back
    m.vertex(v4);
    m.normal(n1);
    m.texCoord(t0);
    m.color(c1);
    
    m.vertex(v5);
    m.normal(n1);
    m.texCoord(t1);
    m.color(c1);
    
    m.vertex(v6);
    m.normal(n1);
    m.texCoord(t2);
    m.color(c1);
    
    m.vertex(v6);
    m.normal(n1);
    m.texCoord(t2);
    m.color(c1);
    
    m.vertex(v5);
    m.normal(n1);
    m.texCoord(t1);
    m.color(c1);
    
    m.vertex(v7);
    m.normal(n1);
    m.texCoord(t3);
    m.color(c1);
    
    
    //top
    m.vertex(v1);
    m.normal(n2);
    m.texCoord(t0);
    m.color(c2);
    
    m.vertex(v5);
    m.normal(n2);
    m.texCoord(t1);
    m.color(c2);
    
    m.vertex(v3);
    m.normal(n2);
    m.texCoord(t2);
    m.color(c2);
    
    m.vertex(v3);
    m.normal(n2);
    m.texCoord(t2);
    m.color(c2);
    
    m.vertex(v5);
    m.normal(n2);
    m.texCoord(t1);
    m.color(c2);
    
    m.vertex(v7);
    m.normal(n2);
    m.texCoord(t3);
    m.color(c2);
    
    
    //bottom
    m.vertex(v0);
    m.normal(n3);
    m.texCoord(t0);
    m.color(c3);
    
    m.vertex(v4);
    m.normal(n3);
    m.texCoord(t1);
    m.color(c3);
    
    m.vertex(v2);
    m.normal(n3);
    m.texCoord(t2);
    m.color(c3);
    
    m.vertex(v2);
    m.normal(n3);
    m.texCoord(t2);
    m.color(c3);
    
    m.vertex(v4);
    m.normal(n3);
    m.texCoord(t1);
    m.color(c3);
    
    m.vertex(v6);
    m.normal(n3);
    m.texCoord(t3);
    m.color(c3);
    
    
    return 36;

    
  }
  
    int addCube(MeshData &m, bool withNormalsAndTexcoords, float s) {
        if (withNormalsAndTexcoords) {

            //8 vertices
            vec3 v0 = vec3(-s, -s, s);
            vec3 v1 = vec3(-s, s, s);
            vec3 v2 = vec3(s, -s, s);
            vec3 v3 = vec3(s, s, s);
            vec3 v4 = vec3(-s, -s, -s);
            vec3 v5 = vec3(-s, s, -s);
            vec3 v6 = vec3(s, -s, -s);
            vec3 v7 = vec3(s, s, -s);

            //6 sides
            vec3 n0 = vec3(0, 0, s);
            vec3 n1 = vec3(0, 0, -s);
            vec3 n2 = vec3(0, s, 0);
            vec3 n3 = vec3(0, -s, 0);
            vec3 n4 = vec3(s, 0, 0);
            vec3 n5 = vec3(-s, 0, 0);

            //4 texcoords
            vec3 t0 = vec3(0, 0, 0);
            vec3 t1 = vec3(0, 1, 0);
            vec3 t2 = vec3(1, 0, 0);
            vec3 t3 = vec3(1, 1, 0);

            //right
            m.vertex(v2);
            m.normal(n4);
            m.texCoord(t0);
            m.vertex(v3);
            m.normal(n4);
            m.texCoord(t1);
            m.vertex(v6);
            m.normal(n4);
            m.texCoord(t2);
            m.vertex(v6);
            m.normal(n4);
            m.texCoord(t2);
            m.vertex(v3);
            m.normal(n4);
            m.texCoord(t1);
            m.vertex(v7);
            m.normal(n4);
            m.texCoord(t3);

            //left
            m.vertex(v4);
            m.normal(n5);
            m.texCoord(t0);
            m.vertex(v5);
            m.normal(n5);
            m.texCoord(t1);
            m.vertex(v0);
            m.normal(n5);
            m.texCoord(t2);
            m.vertex(v0);
            m.normal(n5);
            m.texCoord(t2);
            m.vertex(v5);
            m.normal(n5);
            m.texCoord(t1);
            m.vertex(v1);
            m.normal(n5);
            m.texCoord(t3);

            //front
            m.vertex(v0);
            m.normal(n0);
            m.texCoord(t0);
            m.vertex(v1);
            m.normal(n0);
            m.texCoord(t1);
            m.vertex(v2);
            m.normal(n0);
            m.texCoord(t2);
            m.vertex(v2);
            m.normal(n0);
            m.texCoord(t2);
            m.vertex(v1);
            m.normal(n0);
            m.texCoord(t1);
            m.vertex(v3);
            m.normal(n0);
            m.texCoord(t3);

            //back
            m.vertex(v4);
            m.normal(n1);
            m.texCoord(t0);
            m.vertex(v5);
            m.normal(n1);
            m.texCoord(t1);
            m.vertex(v6);
            m.normal(n1);
            m.texCoord(t2);
            m.vertex(v6);
            m.normal(n1);
            m.texCoord(t2);
            m.vertex(v5);
            m.normal(n1);
            m.texCoord(t1);
            m.vertex(v7);
            m.normal(n1);
            m.texCoord(t3);

            //top
            m.vertex(v1);
            m.normal(n2);
            m.texCoord(t0);
            m.vertex(v5);
            m.normal(n2);
            m.texCoord(t1);
            m.vertex(v3);
            m.normal(n2);
            m.texCoord(t2);
            m.vertex(v3);
            m.normal(n2);
            m.texCoord(t2);
            m.vertex(v5);
            m.normal(n2);
            m.texCoord(t1);
            m.vertex(v7);
            m.normal(n2);
            m.texCoord(t3);

            //bottom
            m.vertex(v0);
            m.normal(n3);
            m.texCoord(t0);
            m.vertex(v4);
            m.normal(n3);
            m.texCoord(t1);
            m.vertex(v2);
            m.normal(n3);
            m.texCoord(t2);
            m.vertex(v2);
            m.normal(n3);
            m.texCoord(t2);
            m.vertex(v4);
            m.normal(n3);
            m.texCoord(t1);
            m.vertex(v6);
            m.normal(n3);
            m.texCoord(t3);

            return 6 * 6;

        } else {
            int Nv = 8;
            m.vertex(-s, s, -s);
            m.vertex(s, s, -s);
            m.vertex(-s, -s, -s);
            m.vertex(s, -s, -s);
            m.vertex(-s, s, s);
            m.vertex(s, s, s);
            m.vertex(-s, -s, s);
            m.vertex(s, -s, s);

            static const unsigned int indices[] = {
                    6, 5, 4, 6, 7, 5, 7, 1, 5, 7, 3, 1,
                    3, 0, 1, 3, 2, 0, 2, 4, 0, 2, 6, 4,
                    4, 1, 0, 4, 5, 1, 2, 3, 6, 3, 7, 6
            };

            //m.index(indices, sizeof(indices)/sizeof(*indices), m.vertices().size()-Nv);
            m.index(indices, 36);

            return Nv;
        }
    }

    // Stacks are circles cut perpendicular to the z axis while slices are circles
    // cut through the z axis.
    // The top is (0,0,radius) and the bottom is (0,0,-radius).

    int addSphere(MeshData &m, double radius, int slices, int stacks) {

        vec3 N, V;
        vec3 Tex; //agf

        struct CSin {
            CSin(double frq, double radius = 1.)
            :    r(radius), i(0.), dr(cos(frq)), di(sin(frq)) {
            }

            void operator ()() {
                double r_ = r * dr - i * di;
                i = r * di + i * dr;
                r = r_;
            }

            double r, i, dr, di;
        };

        int Nv = (unsigned int) m.vertices().size();

        //phi, theta?
        CSin P(M_PI/ stacks);
        P.r = P.dr * radius;
        P.i = P.di * radius;
        CSin T((M_PI* 2.0) / slices);

        // Add top cap
        // Triangles have one vertex at the north pole and the others on the first
        // ring down.
        V = vec3(0, 0, radius);
        //N = vec3(0,0,radius).normalize();
        N = glm::normalize(vec3(0, 0, radius));

        //Tex = vec3(1 - (float) i / slices, 1 - (float) (j+1) / stacks);
        //Tex = vec3(1 - (float) i / slices,
        //			1 - (float) (j) / stacks, 0.0);

        Tex = vec3(0.0, 0.0, 0.0);  //agf
        m.texCoord(Tex); //agf

        m.vertex(V);
        m.normal(N);


        for (int i = 0; i < slices; ++i) {
            m.index(Nv + 1 + i);
            m.index(Nv + 1 + ((i + 1) % slices));
            m.index(Nv);    // the north pole
        }

        // Add rings
        for (int j = 0; j < stacks - 2; ++j) {
            int jp1 = j + 1;

            for (int i = 0; i < slices; ++i) {
                int ip1 = (i + 1) % slices;

                int i00 = Nv + 1 + j * slices + i;
                int i10 = Nv + 1 + j * slices + ip1;
                int i01 = Nv + 1 + jp1 * slices + i;
                int i11 = Nv + 1 + jp1 * slices + ip1;
                V = vec3(T.r * P.i, T.i * P.i, P.r);

                //printf("st/sl = %d/%d : %f/%f/%f\n", j, i, V.x, V.y, V.z);
                //N = vec3(T.r*P.i, T.i*P.i, P.r).normalize();
                N = glm::normalize(vec3(T.r * P.i, T.i * P.i, P.r));


                Tex = vec3(1 - (float) i / slices,
                        1 - (float) (j) / stacks, 0.0);
                m.texCoord(Tex);

                //m.vertex(T.r*P.i, T.i*P.i, P.r);
                m.vertex(V);
                m.normal(N);
                m.index(i00);
                m.index(i01);
                m.index(i10);
                m.index(i10);
                m.index(i01);
                m.index(i11);
                T();
            }
            P();
        }

        // Add bottom ring and cap
        int icap = (unsigned int) m.vertices().size() + slices;
        for (int i = 0; i < slices; ++i) {
            V = vec3(T.r * P.i, T.i * P.i, P.r);

            //printf("sl = %d : %f/%f/%f\n", i, V.x, V.y, V.z);

            //N = vec3(T.r*P.i, T.i*P.i, P.r).normalize();
            N = glm::normalize(vec3(T.r * P.i, T.i * P.i, P.r));
            m.vertex(V);
            m.normal(N);

            Tex = vec3(1.0, 1.0, 0.0);  //agf
            m.texCoord(Tex); //agf



            //  m.vertex(T.r*P.i, T.i*P.i, P.r);
            m.index(icap - slices + ((i + 1) % slices));
            m.index(icap - slices + i);
            m.index(icap);
            T();
        }
        //m.vertex(0,0,-radius);
        V = vec3(0, 0, -radius);
        //N = vec3(0,0,-radius).normalize();
        N = glm::normalize(vec3(0, 0, -radius));
        m.vertex(V);
        m.normal(N);

        return (unsigned int) m.vertices().size() - Nv;
    }


    int addWireBox(MeshData &m, float w, float h, float d) {

        size_t Nv = m.vertices().size();

        /*		6 7
            4 5
            2 3
            0 1			*/

        for (int k = -1; k <= 1; k += 2) {
            for (int j = -1; j <= 1; j += 2) {
                for (int i = -1; i <= 1; i += 2) {
                    m.vertex(i * w, j * h, k * d);
                }
            }
        }

        static const unsigned int I[] = {
                0, 1, 2, 3, 4, 5, 6, 7,
                0, 2, 1, 3, 4, 6, 5, 7,
                0, 4, 1, 5, 2, 6, 3, 7
        };

        //m.index(I, sizeof(I)/sizeof(*I), Nv);
        m.index(I, 24);
        return (unsigned int) (m.vertices().size() - Nv);
    }


    int addSurface(MeshData &m, int Nx, int Ny, float width, float height) {
        int Nv = (unsigned int) m.vertices().size();

        for (int j = 0; j < Ny; ++j) {
            float y = (float(j) / (Ny - 1) - 0.5f) * height;
            for (int i = 0; i < Nx; ++i) {
                float x = (float(i) / (Nx - 1) - 0.5f) * width;
                m.vertex(x, y);
            }
        }

        // Note: the start and end points of each row are duplicated to create
        // degenerate triangles.
        for (int j = 0; j < Ny - 1; ++j) {
            m.index(j * Nx + Nv);
            for (int i = 0; i < Nx; ++i) {
                int idx = j * Nx + i + Nv;
                m.index(idx);
                m.index(idx + Nx);
            }
            //int idx = m.indices().last();
            int idx = m.indices().back();
            m.index(idx);
        }
        return Nx * Ny;
    }


}
