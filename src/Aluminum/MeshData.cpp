#include "Aluminum/MeshData.hpp"

namespace aluminum {

    using glm::vec3;
    using glm::vec4;


    MeshData::MeshData() {
    }


    MeshData &MeshData::create() {
        return *this;
    }

    MeshData &MeshData::reset() {
        vertices().clear();
        normals().clear();
        colors().clear();
        texCoords().clear();
        indices().clear();
        return *this;
    }


    void MeshData::invertNormals() {
        size_t Nv = normals().size();
        for (int i = 0; i < Nv; ++i) normals()[i] = -normals()[i];
    }

    void MeshData::addMesh(MeshData &m2) {

        bool hasNormals = false;
        bool hasTexCoords = false;
        bool hasIndices = false;

        //1. check which elements need to be merged

        size_t numCurrentVertices = vertices().size();
        if (numCurrentVertices == 0) { //first merge - determine elements from new mesh
            hasNormals = m2.normals().size();
            hasTexCoords = m2.texCoords().size();
            hasIndices = m2.indices().size();
        } else { //subsequent merge - make sure incoming mesh's elements match

            bool oN = normals().size();
            bool nN = m2.normals().size();

            bool oT = texCoords().size();
            bool nT = m2.texCoords().size();

            bool oI = indices().size();
            bool nI = m2.indices().size();

            //check if it's a legal merge, if not print error and return...

            if (oN != nN || oT != nT || oI != nI) {
                printf("in MeshData::addMesh(MeshData& m2) : Can't merge meshes!\n");
                return;
            }

            hasNormals = oN;
            hasTexCoords = oT;
            hasIndices = oI;
        }

        //printf("n = %d, t = %d, i = %d\n", hasNormals, hasTexCoords, hasIndices);

        //2. merge elements form new mesh into this mesh

        if (hasIndices) {
            for (size_t i = 0; i < m2.indices().size(); i++) {
                //     indices().push_back(m2.indices()[i] + numCurrentVertices);
                indices().push_back(m2.indices()[i] + (unsigned int) numCurrentVertices);
            }
        }

        for (size_t i = 0; i < m2.vertices().size(); i++) {

            vertices().push_back(m2.vertices()[i]);

            if (hasNormals) {
                normals().push_back(m2.normals()[i]);
            }

            if (hasTexCoords) {
                texCoords().push_back(m2.texCoords()[i]);
            }
        }

    }

    void MeshData::getBounds(vec3 &min, vec3 &max) const {
        if (vertices().size()) {
            min = vertices()[0];
            max = min;
            for (size_t v = 1; v < vertices().size(); ++v) {
                const vec3 &vt = vertices()[v];
                for (int i = 0; i < 3; ++i) {
                    min[i] = std::min(min[i], vt[i]);
                    max[i] = std::max(max[i], vt[i]);
                }
            }
        }
    }

    vec3 MeshData::getCenter() const {
        vec3 min(0), max(0);
        getBounds(min, max);
        return min + ((max - min) * vec3(0.5, 0.5, 0.5));
    }

    void MeshData::unitize(bool proportional) {
        vec3 minv(0), maxv(0);
        getBounds(minv, maxv);
        // span of each axis:
        vec3 span = maxv - minv;    // positive only
        // center of each axis:
        vec3 midv = minv + (span * vec3(0.5, 0.5, 0.5));
        // axis scalar:
        vec3 scale(2. / span.x, 2. / span.y, 2. / span.z);    // positive only

        // adjust to use scale of largest axis:
        if (proportional) {
            //float s = al::min(scale.x, al::min(scale.y, scale.z));
            //float s0 = min(scale.y, scale.z);
            float s = std::min(scale.x, std::min(scale.y, scale.z));
            scale.x = scale.y = scale.z = s;
        }

        for (size_t v = 0; v < mVertices.size(); v++) {
            vec3 &vt = mVertices[v];
            vt = (vt - midv) * scale;
        }
    }

    //MeshData& MeshData::translate(const vec3& t){
    MeshData &MeshData::translate(float x, float y, float z) {
        //const Vertex xfm(x,y,z);
        vec3 xfm = vec3(x, y, z);

        for (size_t i = 0; i < vertices().size(); ++i)
            mVertices[i] += xfm;
        return *this;
    }

    //MeshData& MeshData::scale(const vec3& s)
    MeshData &MeshData::scale(float x, float y, float z) {
        //const Vertex xfm(x,y,z);
        vec3 xfm = vec3(x, y, z);
        for (size_t i = 0; i < vertices().size(); ++i)
            mVertices[i] *= xfm;
        return *this;
    }


    MeshData &MeshData::transform(const mat4 &m) {
        return transform(m, 0, -1);
    }


    MeshData &MeshData::transform(const mat4 &m, int begin) {
        return transform(m, begin, -1);
    }

    MeshData &MeshData::transform(const mat4 &m, int begin, int end) {

        if (end < 0) end += vertices().size() + 1; // negative index wraps to end of array

        for (int i = begin; i < end; ++i) {
            vec3 &v = vertices()[i];
            //v.set(m * Vec<4,T>(v, 1));
            vec4 tmp = m * vec4(v.x, v.y, v.z, 1.0);
            //v = (m * v);
            v = vec3(tmp.x, tmp.y, tmp.z);
        }
        return *this;
    }
}
