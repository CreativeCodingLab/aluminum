#ifndef INCLUDE_ALU_MESHDATA_HPP
#define INCLUDE_ALU_MESHDATA_HPP

#if defined(_WIN32)
#include "Includes.hpp"
#elif defined(__APPLE__)
#include "Aluminum/Includes.hpp"
#endif


namespace aluminum {

	using glm::to_string;
	using glm::ivec2;
	using glm::ivec4;
	using glm::vec2;
	using glm::vec3;
	using glm::vec4;
	using glm::mat4;

	using std::vector;

	/// Stores buffers related to rendering graphical objects

	/// A mesh is a collection of buffers storing vertices, colors, indices, etc.
	/// that define the geometry and coloring/shading of a graphical object.
	class MeshData {
		public:

			/*
			   typedef vec3 Vertex;
			   typedef vec3 Normal;
			   typedef vec4 Color;
			   typedef vec3 TexCoord;
			   typedef unsigned int Index;

			   typedef vector<vec3> Vertices;
			   typedef vector<vec3> Normals;
			   typedef vector<vec4>	Colors;
			   typedef vector<vec3> TexCoords;
			   typedef vector<unsigned int> Indices;
			   */

			/*
			   MeshData(const MeshData& cpy) :
			   mVertices(cpy.mVertices),
			   mNormals(cpy.mNormals),
			   mColors(cpy.mColors),
			   mTexCoords(cpy.mTexCoords),
			   mIndices(cpy.mIndices),
			   mPrimitive(cpy.mPrimitive)
			   {}
			   */

			/// @param[out] min		minimum corner of bounding box
			/// @param[out] max		maximum corner of bounding box

			MeshData();

			MeshData &create();

			//MeshData& makeRectangle2(vec2 vLL, vec2 vUR, vec2 tcLL, vec2 tcUR);
			//MeshData& makeRectangle2(vec3 v0, vec3 v1, vec3 v2, vec3 v3, vec3 t0, vec3 t1, vec3 t2, vec3 t3);



			MeshData &reset();

			void getBounds(vec3 &min, vec3 &max) const;

			vec3 getCenter() const;

			void addMesh(MeshData &m2); //agf

			/// Scale all vertices to lie in [-1,1]
			void unitize(bool proportional = true);

			/// Scale all vertices
			MeshData &scale(float x, float y, float z);

			MeshData &scale(float s) {
				return scale(s, s, s);
			}

			//template <class T>
			//MeshData& scale(const Vec<3,T>& v){ return scale(v[0],v[1],v[2]); }
			//template <class T>
			MeshData &scale(const vec3 &v) {
				return scale(v[0], v[1], v[2]);
			}
			//MeshData& scale(const vec3& v);

			/// Translate all vertices
			MeshData &translate(float x, float y, float z);

			//template <class T>
			//MeshData& translate(const Vec<3,T>& v){ return translate(v[0],v[1],v[2]); }
			//template <class T>
			MeshData &translate(const vec3 &v) {
				return translate(v[0], v[1], v[2]);
			}

			/// Transform vertices by projective transform matrix

			/// @param[in] m		projective transform matrix
			/// @param[in] begin	beginning index of vertices
			/// @param[in] end		ending index of vertices, negative amount specify distance from one past last element
			//template <class T>
			//MeshData& transform(const Mat<4,T>& m, int begin=0, int end=-1);

			//MeshData& transform(const mat4& m, int begin=0, int end=-1);

			MeshData &transform(const mat4 &m, int begin, int end);

			MeshData &transform(const mat4 &m, int begin);

			MeshData &transform(const mat4 &m);


			/// Invert direction of normals
			void invertNormals();

			const vector<vec3> &vertices() const {
				return mVertices;
			}

			const vector<vec3> &normals() const {
				return mNormals;
			}

			const vector<vec4> &colors() const {
				return mColors;
			}

			const vector<vec3> &texCoords() const {
				return mTexCoords;
			}

			const vector<unsigned int> &indices() const {
				return mIndices;
			}

			void index(unsigned int i) {
				indices().push_back(i);
			}

			void index(const unsigned int *buf, int size) {
				for (int i = 0; i < size; ++i) {
					index(buf[i]);
				}
			}

			void color(float r, float g, float b, float a = 1.0) {
				color(vec4(r, g, b, a));
			}

			void color(const vec4 &v) {
				colors().push_back(v);
			}

			void color(const vec4 *buf, int size) {
				for (int i = 0; i < size; ++i) {
					color(buf[i][0], buf[i][1], buf[i][2], buf[i][3]);
				}
			}

			void color(const vec3 *buf, int size) {
				for (int i = 0; i < size; ++i) {
					color(buf[i][0], buf[i][1], buf[i][2], 1.0);
				}
			}


			void normal(float x, float y, float z = 0.0) {
				normal(vec3(x, y, z));
			}

			void normal(const vec3 &v) {
				normals().push_back(v);
			}

			void normal(const vec3 *buf, int size) {
				for (int i = 0; i < size; ++i) {
					normal(buf[i][0], buf[i][1], buf[i][2]);
				}
			}

			void texCoord(float u, float v, float w = 0.0) {
				texCoord(vec3(u, v, w));
			}

			void texCoord(const vec3 &v) {
				texCoords().push_back(v);
			}

			void texCoord(const vec3 *buf, int size) {
				for (int i = 0; i < size; ++i) {
					texCoord(buf[i][0], buf[i][1], buf[i][2]);
				}
			}

			void vertex(float x, float y, float z = 0.0) {
				vertex(vec3(x, y, z));
			}

			void vertex(const vec3 &v) {
				vertices().push_back(v);
			}

			void vertex(const vec3 *buf, int size) {
				for (int i = 0; i < size; ++i) {
					vertex(buf[i][0], buf[i][1], buf[i][2]);
				}
			}




			/// Get number of faces (assumes triangles or quads)
			//	int numFaces() const { return mIndices.size() / ( ( mPrimitive == Graphics::TRIANGLES ) ? 3 : 4 ); }
			/// Get indices as triangles
			//	TriFace& indexAsTri(){ return (TriFace*) indices(); }
			/// Get indices as quads
			//	QuadFace& indexAsQuad(){ return (QuadFace*) indices(); }

			vector<vec3> &vertices() {
				return mVertices;
			}

			vector<vec3> &normals() {
				return mNormals;
			}

			vector<vec4> &colors() {
				return mColors;
			}

			vector<vec3> &texCoords() {
				return mTexCoords;
			}

			vector<unsigned int> &indices() {
				return mIndices;
			}

			friend std::ostream& operator<<(std::ostream& Str, const MeshData& md) {

				for (size_t i = 0; i < md.vertices().size(); i++) {
					Str << "vertex: " << glm::to_string(md.vertices()[i]) << " texcoord:" << glm::to_string(md.texCoords()[i]) << "\n";
				}

				return Str;
			}


			//GLint id() {return vaoID;}
		protected:

			// Only populated (size>0) buffers will be used
			vector<vec3> mVertices;
			vector<vec3> mNormals;
			vector<vec4> mColors;
			vector<vec3> mTexCoords;
			vector<unsigned int> mIndices;

			int mPrimitive;

			//GLint vaoID;
	};

} // al::

#endif
