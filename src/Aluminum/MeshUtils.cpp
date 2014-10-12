#include "Aluminum/MeshUtils.hpp"

namespace aluminum {

#ifndef BUILD_IOS

    using std::cout;
    using std::endl;
    using glm::to_string;
/*
    //will merge multiple meshes
    MeshBuffer MeshUtils::loadMesh(const std::string& path, int pL, int nL, int tL, int cL) {


        Scene* s = MeshUtils::importScene(path);

        MeshData modelMesh;

        for (unsigned int i = 0; i < s->meshes(); i++) {
            MeshData tmp;
            s->meshWithIndices(i, tmp);

            //transfrom to center of screen - should probably be an option...
            mat4 t = mat4();

            float scaleVal = s->getScaleVal();
            //printf("scaleVal = %f\n", scaleVal);
            t = glm::scale(t, vec3(scaleVal));

            vec3 center;
            s->getCenter(center);
            //printf("::: center = %f/%f/%f\n", center.x, center.y, center.z);

            t = glm::translate(t, -center);

            tmp.transform( t );

            modelMesh.addMesh(tmp); //merge this mesh into single mesh
        }

        return MeshBuffer().init(modelMesh, pL, nL, tL, cL);

    }


    //will merge multiple meshes
    MeshData MeshUtils::loadMeshData(const std::string& path) {

        Scene* s = MeshUtils::importScene(path);

        MeshData modelMesh;

        for (unsigned int i = 0; i < s->meshes(); i++) {
            MeshData tmp;
            s->meshWithIndices(i, tmp);
            tmp.transform( glm::scale(mat4(), vec3(s->getScaleVal())) );
            modelMesh.addMesh(tmp); //merge this mesh into single mesh
        }

        return modelMesh;
    }


    //will not merge multiple meshes

    std::vector<MeshData> MeshUtils :: loadMeshDatas(const std::string& path) {

        vector<MeshData> md;

        Scene* s = MeshUtils::importScene(path);

        for (int i = s->meshes()-1; i >= 0; i--) {
            MeshData tmp;
            s->meshWithIndices(i, tmp);
            tmp.transform( glm::scale(mat4(), vec3(s->getScaleVal())) );
            //modelMesh.addMesh(tmp); //merge this mesh into single mesh
            md.push_back(tmp);
        }

        return md;
    }


    MeshUtils::Scene* MeshUtils :: importScene(const std::string& path) {


        static struct aiLogStream logStream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
        aiAttachLogStream(&logStream);
        //aiEnableVerboseLogging(true);

        Assimp::Importer importer;

        int flags;

        //flags = aiProcessPreset_TargetRealtime_Fast;
        //flags = aiProcessPreset_TargetRealtime_Quality;
        flags = aiProcessPreset_TargetRealtime_MaxQuality;


//         flags =  aiProcess_Triangulate            |
//         aiProcess_JoinIdenticalVertices  |
//         aiProcess_GenSmoothNormals |
//         aiProcess_FindInvalidData |
//         aiProcess_ImproveCacheLocality |
//         aiProcess_OptimizeMeshes;


        const aiScene * scene = aiImportFile(path.c_str(), flags);

        if (scene) {
            return(new Scene(scene));
        } else {
            return NULL;
        }

    }


    vec4 vec4FromAIColor4D(aiColor4D& v) {
        return vec4(v.r, v.g, v.b, v.a);
    }

    vec3 vec3FromAIVector3D(aiVector3D& v) {
        return vec3(v.x, v.y, v.z);
    }

    vec2 vec2FromAIVector3D(aiVector3D& v) {
        return vec2(v.x, v.y);
    }

    vec3 vec3FromAIVector2D(aiVector2D& v) {
        return vec3(v.x, v.y, 0.0);
    }


    MeshUtils::Scene :: Scene(const aiScene * _scene) {
        scene = _scene;
    }

    MeshUtils::Scene :: ~Scene() {
    }


    unsigned int MeshUtils::Scene :: meshes() const {
        return scene->mNumMeshes;
    }

    void MeshUtils::Scene :: mesh(unsigned int i, MeshData& mesh) const {
        if (i < meshes()) {
            aiMesh * amesh = scene->mMeshes[i];
            if (amesh) {

                bool hasnormals = amesh->mNormals != NULL;
                bool hascolors = amesh->mColors[0] != NULL;
                bool hastexcoords = amesh->mTextureCoords[0] != NULL;

                for (unsigned int t = 0; t < amesh->mNumFaces; ++t) {
                    const struct aiFace* face = &amesh->mFaces[t];

                    for(i = 0; i < face->mNumIndices; i++) {
                        int index = face->mIndices[i];
                        if(hascolors) {
                            mesh.color(vec4FromAIColor4D(amesh->mColors[0][index]));
                        }
                        if(hasnormals) {
                            mesh.normal(vec3FromAIVector3D(amesh->mNormals[index]));
                        }
                        if(hastexcoords) {
                            mesh.texCoord(vec3FromAIVector3D(amesh->mTextureCoords[0][index]));
                        }

                        mesh.vertex(vec3FromAIVector3D(amesh->mVertices[index]));

                    }
                }

            }
        }
    }

    void MeshUtils::Scene :: meshWithIndices(unsigned int i, MeshData& mesh) const {
        if (i < meshes()) {
            aiMesh * amesh = scene->mMeshes[i];
            if (amesh) {

                bool hasnormals = amesh->mNormals != NULL;
                bool hascolors = amesh->mColors[0] != NULL;
                bool hastexcoords = amesh->mTextureCoords[0] != NULL;

                //read vertices, normals, colors, texcoord
                for (unsigned int index = 0; index < amesh->mNumVertices; ++index){
                    if(hascolors) {

                        mesh.color(vec4FromAIColor4D(amesh->mColors[0][index]));
                    }
                    if(hasnormals) {
                        vec3 n = vec3FromAIVector3D(amesh->mNormals[index]);
                        //printf("p%d, n=%f/%f/%f\n", index, n.x, n.y, n.z);
                        mesh.normal(n);
                    }
                    if(hastexcoords) {
                        vec3 tc = vec3FromAIVector3D(amesh->mTextureCoords[0][index]);
                        //printf("p%d, tc=%f/%f/%f\n", index, tc.x, tc.y, tc.z);
                        mesh.texCoord(tc);
                    }

                    vec3 v = vec3FromAIVector3D(amesh->mVertices[index]);
                    //printf("p%d, v=%f/%f/%f\n", index, v.x, v.y, v.z);
                    mesh.vertex(v);
                }

                //read faces as indices
                for (unsigned int t = 0; t < amesh->mNumFaces; ++t) {

                    const struct aiFace* tface = &amesh->mFaces[t];

                    for(i = 0; i < tface->mNumIndices; i++) {
                        mesh.index( tface -> mIndices[i] );
                    }

                }
            }
        }
    }

    void get_bounding_box_for_node(const aiScene * scene, const struct aiNode* nd, vec3& min, vec3& max, aiMatrix4x4* trafo) {
        aiMatrix4x4 prev;
        unsigned int n = 0, t;
        prev = *trafo;
        aiMultiplyMatrix4(trafo,&nd->mTransformation);
        for (; n < nd->mNumMeshes; ++n) {
            const struct aiMesh * mesh = scene->mMeshes[nd->mMeshes[n]];
            for (t = 0; t < mesh->mNumVertices; ++t) {
                aiVector3D tmp = mesh->mVertices[t];
                aiTransformVecByMatrix4(&tmp,trafo);
                min[0] = std::min(min[0],tmp.x);
                min[1] = std::min(min[1],tmp.y);
                min[2] = std::min(min[2],tmp.z);
                max[0] = std::max(max[0],tmp.x);
                max[1] = std::max(max[1],tmp.y);
                max[2] = std::max(max[2],tmp.z);
            }
        }
        for (n = 0; n < nd->mNumChildren; ++n) {
            get_bounding_box_for_node(scene, nd->mChildren[n],min,max,trafo);
        }
        *trafo = prev;
    }


    void MeshUtils::Scene :: getCenter(vec3& center) const {
        vec3 min, max;
        getBounds(min,max);
        //cout << "in getScaleVal() : bounds min = " << glm::to_string(min) << ", max = " << glm::to_string(max) << "\n";
        center = (min + max) * vec3(0.5,0.5,0.5);
        //cout << "in getCenter() : scene center = " << to_string(center) << "\n";


    }


    float MeshUtils::Scene :: getScaleVal() const {
        vec3 min, max;
        getBounds(min,max);
        //cout << "in getScaleVal() : bounds min = " << glm::to_string(min) << ", max = " << glm::to_string(max) << "\n";

        //vec3 scene_center = (min + max) * vec3(0.5,0.5,0.5);
        //cout << "in getScaleVal() : scene center = " << to_string(scene_center) << "\n";

        float scaleVal = max[0] - min[0];
        scaleVal = std::max(max[1] - min[1], scaleVal);
        scaleVal = std::max(max[2] - min[2], scaleVal);
        scaleVal = 2.f / scaleVal;

        return scaleVal;
    }

    void MeshUtils::Scene :: getBounds(vec3& min, vec3& max) const {
        aiMatrix4x4 trafo;
        aiIdentityMatrix4(&trafo);
        min = vec3(1e10f, 1e10f, 1e10f);
        max = vec3(-1e10f, -1e10f, -1e10f);
        get_bounding_box_for_node(scene, scene->mRootNode,min,max,&trafo);
    }


    void dumpNode(aiNode * x, std::string indent) {
        printf("%sNode (%s) with %d meshes (", indent.c_str(), x->mName.data, x->mNumMeshes);
        for (unsigned int i=0; i<x->mNumMeshes; i++) {
            printf("%d ", x->mMeshes[i]);
        }
        printf(") and %d children\n", x->mNumChildren);
        for (unsigned int i=0; i<x->mNumChildren; i++) {
            dumpNode(x->mChildren[i], indent + "\t");
        }
    }

    void MeshUtils::Scene :: dump() const {
        printf("==================================================\n");
        printf("Scene\n");

        printf("%d Meshes\n", meshes());
        for (unsigned int i=0; i<meshes(); i++) {
            aiMesh * x = scene->mMeshes[i];
            printf("\t%d: %s", i, x->mName.data);
            printf(" %d vertices, %d faces; normals:%d colors:%d texcoords:%d\n", x->mNumVertices, x->mNumFaces, x->HasNormals(), x->HasVertexColors(0), x->HasTextureCoords(0));
        }

        dumpNode(scene->mRootNode, "");

        printf("==================================================\n");
    }

*/
#endif

    MeshData MeshUtils::makeCube(float s) {

        MeshData m;

        //8 vertices
        vec3 v0 = vec3(-s,-s,s);
        vec3 v1 = vec3(-s,s,s);
        vec3 v2 = vec3(s,-s,s);
        vec3 v3 = vec3(s,s,s);
        vec3 v4 = vec3(-s,-s,-s);
        vec3 v5 = vec3(-s,s,-s);
        vec3 v6 = vec3(s,-s,-s);
        vec3 v7 = vec3(s,s,-s);

        //6 sides
        vec3 n0 = vec3(0,0,s);
        vec3 n1 = vec3(0,0,-s);
        vec3 n2 = vec3(0,s,0);
        vec3 n3 = vec3(0,-s,0);
        vec3 n4 = vec3(s,0,0);
        vec3 n5 = vec3(-s,0,0);

        //4 texcoords
        vec3 t0 = vec3(0,0,0);
        vec3 t1 = vec3(0,1,0);
        vec3 t2 = vec3(1,0,0);
        vec3 t3 = vec3(1,1,0);

        const vec3 vs[] = {
                v2, v3, v6, v7, //right
                v4, v5, v0, v1, //left
                v0, v1, v2, v3, //front
                v4, v5, v6, v7, //back
                v1, v5, v3, v7, //top
                v0, v4, v2, v6  //bottom
        };

        const vec3 ns[] = {
                n4, n4, n4, n4,
                n5, n5, n5, n5,
                n0, n0, n0, n0,
                n1, n1, n1, n1,
                n2, n2, n2, n2,
                n3, n3, n3, n3
        };

        const vec3 ts[] = {
                t0, t1, t2, t3,
                t0, t1, t2, t3,
                t0, t1, t2, t3,
                t0, t1, t2, t3,
                t0, t1, t2, t3,
                t0, t1, t2, t3
        };

        const unsigned int indices[] = {
                0,1,2, 2,1,3,
                4,5,6, 6,5,7,
                8,9,10, 10,9,11,
                12,13,14, 14,13,15,
                16,17,18, 18,17,19,
                20,21,22, 22,21,23
        };

        m.vertex(vs, 24);
        m.normal(ns, 24);
        m.texCoord(ts, 24);
        m.index(indices, 36);

        return m;
    }


    MeshData MeshUtils::makeCube2(float s) {

        MeshData m;

        //8 vertices
        vec3 v0 = vec3(-s,-s,-s);
        vec3 v1 = vec3(-s,s,-s);
        vec3 v2 = vec3(s,-s,-s);
        vec3 v3 = vec3(s,s,-s);
        vec3 v4 = vec3(s,-s,s);
        vec3 v5 = vec3(s,s,s);
        vec3 v6 = vec3(-s,-s,s);
        vec3 v7 = vec3(-s,s,s);

        const vec3 vs[] = {
                v0, v1, v2, v3, //front
                v4, v5, v6, v7 //back
        };

        printf("Assigning MeshData's vertex\n");
        m.vertex(vs, 8);


        //8 texcoords           //        0-----1           y   z
        vec3 t0 = vec3(0,0,0);  //       /|    /|           |  /
        vec3 t1 = vec3(0,1,0);  //      2=====3 |           | /
        vec3 t2 = vec3(1,0,0);  //      + |   + |           |/
        vec3 t3 = vec3(1,1,0);  //      + 6---+-7    -x-----+------+x
        vec3 t4 = vec3(1,0,1);  //      +/    +/           /|
        vec3 t5 = vec3(1,1,1);  //      4=====5           / |
        vec3 t6 = vec3(0,0,1);  //                       /  |
        vec3 t7 = vec3(0,1,1);  //                     -z  -y

        const vec3 ts[] = {
                t0, t1, t2, t3,
                t4, t5, t6, t7
        };

        m.texCoord(ts, 8);

        // Cube indices
        const unsigned int indices[] = {
                0,1,2,
                2,1,3,
                2,3,4,
                4,3,5,
                4,5,6,
                6,5,7,
                6,7,0,
                0,7,1,
                6,0,2,
                2,4,6,
                7,5,3,
                7,3,1
        };

        m.index(indices, 36);
/*
  a______b
  |     /|
  | @  / |
  |   /  |
  |  /   |
  | /  @ |
  e/_____f

*/

        return m;
    }


    MeshData MeshUtils::makeWireFrameCube(float s) {
        MeshData m;

        //8 vertices
        vec3 v0 = vec3(-s,-s,s);  // a
        vec3 v1 = vec3(s,-s,s);  // b
        vec3 v2 = vec3(s,s,s);  // c
        vec3 v3 = vec3(-s,s,s);  // d
        vec3 v4 = vec3(-s,-s,-s);  // e
        vec3 v5 = vec3(s,-s,-s);  // f
        vec3 v6 = vec3(s,s,-s);  // g
        vec3 v7 = vec3(-s,s,-s);  // h

        const vec3 vs[] = {
                v0, v1, v2, v3, //front
                v4, v5, v6, v7 //back
        };

        printf("Assigning MeshData's vertex\n");
        m.vertex(vs, 8);


        //8 texcoords
        vec3 t0 = vec3(0,0,1);
        vec3 t1 = vec3(1,0,1);
        vec3 t2 = vec3(1,1,1);
        vec3 t3 = vec3(0,1,1);
        vec3 t4 = vec3(0,0,0);
        vec3 t5 = vec3(1,0,0);
        vec3 t6 = vec3(1,1,0);
        vec3 t7 = vec3(0,1,0);

        const vec3 ts[] = {
                t0, t1, t2, t3, t4, t5, t6, t7
        };
        m.texCoord(ts, 8);


/*
           3-----2
          /|    /|
         7=====6 |
         + |   + |
         + 0---+-1
         +/    +/
         4=====5
*/

        // Cube indices
        const unsigned int indices[] = {
                0,1,    1,2,
                2,3,    3,0,
                4,5,    5,6,
                6,7,    7,4,
                5,1,    2,6,
                0,4,    3,7
        };

        m.index(indices, 24);
/*
  a______b
  |     /|
  | @  / |
  |   /  |
  |  /   |
  | /  @ |
  e/_____f

*/


        return m;
    }


  
  void MeshUtils::addPoint(MeshData &m, vec3 v) {
    
    const vec3 vs[] = { v };
    unsigned int off = (unsigned int) m.indices().size();
    
    const unsigned int indices[] = { off };
    
    m.vertex(vs, 1);
    m.index(indices, 1);
  }



  void MeshUtils::addPoint(MeshData &m, vec3 v, vec3 t) {

    const vec3 vs[] = { v };
    const vec3 ts[] = { t };
    unsigned int off = (unsigned int) m.indices().size();

    const unsigned int indices[] = { off };

    m.vertex(vs, 1);
    m.texCoord(ts, 1);
    m.index(indices, 1);
  }

   void MeshUtils::addTriangle(MeshData &m, vec3 v0, vec3 v1, vec3 v2,
                                   vec3 t0, vec3 t1, vec3 t2) {


    vec3 n = glm::cross(v1 - v0, v2 - v0);

    const vec3 vs[] = { v0, v1, v2 };
    const vec3 ns[] = { n, n, n };
    const vec3 ts[] = { t0, t1, t2 };
    unsigned int off = (unsigned int) m.indices().size();

    const unsigned int indices[] = { off,off+1,off+2 };

    m.vertex(vs, 3);
    m.normal(ns, 3);
    m.texCoord(ts, 3);
    m.index(indices, 3);

  }



  MeshData MeshUtils::makeTriangle(vec3 v0, vec3 v1, vec3 v2,
                                    vec3 t0, vec3 t1, vec3 t2) {


    MeshData m;

    vec3 n = glm::cross(v1 - v0, v2 - v0);

    const vec3 vs[] = { v0, v1, v2 };
    const vec3 ns[] = { n, n, n };
    const vec3 ts[] = { t0, t1, t2 };
    const unsigned int indices[] = { 0,1,2 };

    m.vertex(vs, 3);
    m.normal(ns, 3);
    m.texCoord(ts, 3);
    m.index(indices, 3);

    return m;
  }


    MeshData MeshUtils::makeRectangle(vec3 v0, vec3 v1, vec3 v2, vec3 v3,
            vec3 t0, vec3 t1, vec3 t2, vec3 t3) {


        MeshData m;
        //m.create();

        vec3 n = glm::cross(v0 - v1, v2 - v0); //to do: double check direction...
        n = glm::normalize(n);

        const vec3 vs[] = { v0, v1, v2, v3 };
        const vec3 ns[] = { n, n, n, n };
        const vec3 ts[] = { t0, t1, t2, t3 };
        const unsigned int indices[] = { 0,1,2, 2,1,3 };
        //static const unsigned int indices[] = { 0,1,2, 1,2,3 };

        m.vertex(vs, 4);
        m.normal(ns, 4);
        m.texCoord(ts, 4);
        m.index(indices, 6);
     /*
      cout << "v0 : " << to_string(vs[0]) << "\n";
      cout << "v1 : " << to_string(vs[1]) << "\n";
      cout << "v2 : " << to_string(vs[2]) << "\n";
      cout << "v3 : " << to_string(vs[3]) << "\n";
      
      cout << "n0 : " << to_string(ns[0]) << "\n";
      cout << "n1 : " << to_string(ns[1]) << "\n";
      cout << "n2 : " << to_string(ns[2]) << "\n";
      cout << "n3 : " << to_string(ns[3]) << "\n";
     // exit(0);
      */
      return m;
    }



    MeshData MeshUtils::makeRectangle(vec3 vLL, vec3 vUR, vec3 tcLL, vec3 tcUR) {

        vec3 v0 = vec3(vLL.x, vLL.y, vLL.z);
        vec3 v1 = vec3(vLL.x,vUR.y, vLL.z);
        vec3 v2 = vec3(vUR.x,vLL.y, vUR.z);
        vec3 v3 = vec3(vUR.x,vUR.y,vUR.z);

        vec3 t0 = vec3(tcLL.x,tcLL.y,tcLL.z);
        vec3 t1 = vec3(tcLL.x,tcUR.y,tcLL.z);
        vec3 t2 = vec3(tcUR.x,tcLL.y,tcUR.z);
        vec3 t3 = vec3(tcUR.x,tcUR.y,tcUR.z);

        return makeRectangle(v0,v1,v2,v3,t0,t1,t2,t3);
    }


    MeshData MeshUtils::makeRectangle(vec2 vLL, vec2 vUR, vec2 tcLL, vec2 tcUR) {

        vec3 v0 = vec3(vLL.x, vLL.y,0);
        vec3 v1 = vec3(vLL.x,vUR.y,0);
        vec3 v2 = vec3(vUR.x,vLL.y,0);
        vec3 v3 = vec3(vUR.x,vUR.y,0);

        vec3 t0 = vec3(tcLL.x,tcLL.y,0);
        vec3 t1 = vec3(tcLL.x,tcUR.y,0);
        vec3 t2 = vec3(tcUR.x,tcLL.y,0);
        vec3 t3 = vec3(tcUR.x,tcUR.y,0);

        return makeRectangle(v0,v1,v2,v3,t0,t1,t2,t3);
    }




    MeshData MeshUtils::makeClipGrid(int cols, int rows) { //needs to be rendered via drawTriangleStrip

        return MeshUtils::makeGrid(cols, rows, vec2(-1,-1), vec2(1,1), vec2(0.0), vec2(1,1));
    }

    MeshData MeshUtils::makeGrid(int cols, int rows, vec2 vLL, vec2 vUR) { //needs to be rendered via drawTriangleStrip


        float xl = vLL.x;
        float yl = vLL.y;
        float xu = vUR.x;
        float yu = vUR.y;


        MeshData m;
        vec3* vs = new vec3[cols * rows];
        GLuint* is = new GLuint [(cols*2*(rows-1)+2*(rows-2))];


        for (int y =0; y<rows; y++) {
            for (int x=0; x < cols ; x++) {

                vs[cols*y+x].x= xl +x*(xu-xl)/(float)(cols-1.0);
                vs[cols*y+x].y= yl +y*(yu-yl)/(float)(rows-1.0);
                vs[cols*y+x].z = 0.0f;

            }
        }

        // Index data

        int q = 0;

        for (int y =0; y<rows-1; y++) {
            for (int x=0; x < cols ; x++) {
                is[q] = x + y*cols;
                q++;
                is[q] = x + (y+1)*cols;
                q++;
            }
            if( y < rows-2){ // the degenerate triangles

                //repeating last one
                is[q] = (cols-1) + (y+1)*cols;
                q++;
                //repeating next one
                is[q] =  (y+1)*cols;
                q++;
            }

        }

        m.vertex(vs,cols*rows);
        m.index(is, cols*2*(rows-1)+2*(rows-2));

        delete vs;
        delete is;

        return m;
    }


    MeshData MeshUtils::makeGrid(int cols, int rows, vec2 vLL, vec2 vUR, vec2 tcLL, vec2 tcUR) { //needs to be rendered via drawTriangleStrip


        float xl = vLL.x;
        float yl = vLL.y;
        float xu = vUR.x;
        float yu = vUR.y;

        float sl = tcLL.x;
        float tl = tcLL.y;
        float su = tcUR.x;
        float tu = tcUR.y;

        float sinc = (su - sl) / (cols-1);
        float tinc = (tu - tl) / (rows-1);


        MeshData m;
        vec3* vs = new vec3[cols * rows];
        vec3* ts = new vec3[cols * rows];
        GLuint* is = new GLuint [(cols*2*(rows-1)+2*(rows-2))];


        for (int y =0; y<rows; y++) {
            for (int x=0; x < cols ; x++) {

                vs[cols*y+x].x= xl +x*(xu-xl)/(float)(cols-1.0);
                vs[cols*y+x].y= yl +y*(yu-yl)/(float)(rows-1.0);
                vs[cols*y+x].z = 0.0f;

                ts[cols*y+x].x = sl + (x * sinc);
                ts[cols*y+x].y = tu - (y * tinc) ;
                ts[cols*y+x].z = 0.0;

            }
        }

        // Index data

        int q = 0;

        for (int y =0; y<rows-1; y++) {
            for (int x=0; x < cols ; x++) {
                is[q] = x + y*cols;
                q++;
                is[q] = x + (y+1)*cols;
                q++;
            }
            if( y < rows-2){ // the degenerate triangles

                //repeating last one
                is[q] = (cols-1) + (y+1)*cols;
                q++;
                //repeating next one
                is[q] =  (y+1)*cols;
                q++;
            }

        }

        m.vertex(vs,cols*rows);
        m.texCoord(ts, cols*rows);
        m.index(is, cols*2*(rows-1)+2*(rows-2));

        delete vs;
        delete ts;
        delete is;

        return m;
    }


    /*
    MeshData MeshUtils::makeSurface(int Xres, int Yres, float xl, float xu, float yl, float yu, bool GenTextData) {
        // plane surface with triangle strip in the square x-y [xl,xu,yl,yu],
        //    resolution Xres, Yres


        MeshData m;


        vec3* vs = new vec3[Xres * Yres];

        vec3* ts;
        if (GenTextData){
            ts = new vec3[Xres * Yres];
        }
        GLuint *is = new GLuint [(Xres*2*(Yres-1)+2*(Yres-2))];





        for (int y =0; y<Yres; y++) {
            for (int x=0; x < Xres ; x++) {

                vs[Xres*y+x].x= xl +x*(xu-xl)/(float)(Xres-1.0);
                vs[Xres*y+x].y= yl +y*(yu-yl)/(float)(Yres-1.0);
                vs[Xres*y+x].z = 0.0f;

                if (GenTextData){
                    ts[Xres*y+x].x = x/(float)Xres;
                    ts[Xres*y+x].y = 1.0 - y/(float)Yres;
                    ts[Xres*y+x].z = 0.0;
                }

            }
        }

        // Index data

        int q =0;

        for (int y =0; y<Yres-1; y++) {
            for (int x=0; x < Xres ; x++) {
                is[q] = x + y*Xres;
                q++;
                is[q] = x + (y+1)*Xres;
                q++;
            }
            if( y < Yres-2){ // the degenerate triangles

                //repiting last one
                is[q] = (Xres-1) + (y+1)*Xres;
                q++;
                //repiting next one
                is[q] =  (y+1)*Xres;
                q++;
            }

        }

        m.vertex(vs,Xres*Yres);
        m.texCoord(ts, Xres*Yres);
        m.index(is, Xres*2*(Yres-1)+2*(Yres-2));

        delete vs;
        if(GenTextData){
            delete ts;
        }
        delete is;
        return m;
    }
    */


    /*
     MeshData& MeshUtils::makeRectangle3(MeshData &m, vec3 v0, vec3 v1, vec3 v2, vec3 v3, vec3 t0, vec3 t1, vec3 t2, vec3 t3) {

     printf("in MeshUtils::makeRectangle\n");

     vec3 n = glm::cross(v1 - v0, v2 - v0);

     //m.vertex(v0);m.vertex(v1);m.vertex(v2);m.vertex(v3);
     //m.normal(n);m.normal(n);m.normal(n);m.normal(n);
     //m.texCoord(t0);m.texCoord(t1);m.texCoord(t2);m.texCoord(t3);
     //m.index(0);m.index(1);m.index(2);m.index(2);m.index(1);m.index(3);

     vec3 vs[] = { v0, v1, v2, v3 };
     vec3 ns[] = { n, n, n, n };
     vec3 ts[] = { t0, t1, t2, t3 };
     unsigned int indices[] = { 0,1,2, 2,1,3 };

     m.vertex(vs, 4);
     m.normal(ns, 4);
     m.texCoord(ts, 4);
     m.index(indices, 6);


     return m; //should be able to return the MeshData I passed in right???
     }



     MeshData& MeshUtils::makeRectangle3( MeshData &m, vec2 vLL, vec2 vUR, vec2 tcLL, vec2 tcUR) {

     printf("in MeshUtils::makeRectangle\n");
     vec3 v0 = vec3(vLL.x, vLL.y,0);
     vec3 v1 = vec3(vLL.x,vUR.y,0);
     vec3 v2 = vec3(vUR.x,vLL.y,0);
     vec3 v3 = vec3(vUR.x,vUR.y,0);

     vec3 t0 = vec3(tcLL.x,tcLL.y,0);
     vec3 t1 = vec3(tcLL.x,tcUR.y,0);
     vec3 t2 = vec3(tcUR.x,tcLL.y,0);
     vec3 t3 = vec3(tcUR.x,tcUR.y,0);

     cout << "in MeshUtils A" << glm::to_string(t0) << endl;
     cout << "in MeshUtils A" << glm::to_string(t1) << endl;
     cout << "in MeshUtils A" << glm::to_string(t2) << endl;
     cout << "in MeshUtils A" << glm::to_string(t3) << endl;

     //passing the vec3s by value - so its ok
     return makeRectangle3(m, v0,v1,v2,v3,t0,t1,t2,t3); //should be able to return the MeshData I passed in right???
     }
     */


    MeshData MeshUtils::makeRectangle(float _w, float _h) {

        float w = _w/2.0;
        float h = _h/2.0;

        return makeRectangle(vec2(-w,-h), vec2(w,h), vec2(0,0), vec2(1,1));
    }


    MeshData MeshUtils::makeRectangle(float _w, float _h, bool flipHoriz, bool flipVert) {

        float w = _w/2.0;
        float h = _h/2.0;

        vec2 tLL;
        vec2 tUR;

        if (flipHoriz && flipVert) {
            printf("HERE flip both\n");
            tLL = vec2(1,1);
            tUR = vec2(0,0);
        } else if (flipHoriz) {
            printf("HERE flip horiz\n");
            tLL = vec2(1,0);
            tUR = vec2(0,1);
        } else if (flipVert) {
            printf("HERE flip vert\n");
            tLL = vec2(0,1);
            tUR = vec2(1,0);
        } else {
            printf("HERE dont flip\n");
            tLL = vec2(0,0);
            tUR = vec2(1,1);
        }



        return makeRectangle(vec2(-w,-h), vec2(w,h), tLL, tUR);
    }


    MeshData MeshUtils::makeRectangle() {
        return makeRectangle(1,1);
    }

    MeshData MeshUtils::makeClipRectangle() {
        return makeRectangle(2,2);
    }

    //flipHoriz = over y axis; flipVert = over x axis
    MeshData MeshUtils::makeClipRectangle(bool flipHoriz, bool flipVert) {
        return makeRectangle(2,2,flipHoriz,flipVert);
    }




}
