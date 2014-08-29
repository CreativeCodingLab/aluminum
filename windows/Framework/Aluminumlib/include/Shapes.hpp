#ifndef INCLUDE_AL_GRAPHICS_SHAPES2_HPP
#define INCLUDE_AL_GRAPHICS_SHAPES2_HPP

#if defined(_WIN32)
#include "MeshData.hpp"
#elif defined(__APPLE__)
#include "Aluminum/MeshData.hpp"
#endif

namespace aluminum {

    using glm::to_string;
    using glm::vec2;
    using glm::vec3;
    using glm::vec4;
    using glm::mat4;

    //int addRectangle(MeshData& m, Vec2f vLL, Vec2f vUR, Vec2f tcLL, Vec2f tcUR);
    int addRectangle(MeshData &m, vec2 vLL, vec2 vUR, vec2 tcLL, vec2 tcUR);

    int addRectangle(MeshData &m, float _w, float _h);

    int addRectangle(MeshData &m);

// Platonic solids

/// Add cube as triangle vertices and indices
    int addCube(MeshData &m, bool withNormalsAndTexcoords = false, float radius = 1.414);

/// Add sphere as triangle vertices and indices

/// @param[in,out]	m		mesh to add vertices and indices to
/// @param[in]		radius	radius of sphere
/// @param[in]		slices	number of slices around z axis
/// @param[in]		stacks	number of stacks on xy plane
    int addSphere(MeshData &m, double radius = 1, int slices = 16, int stacks = 16);


/// Add wireframe box as line vertices and indices
    int addWireBox(MeshData &m, float width, float height, float depth);

    inline int addWireBox(MeshData &m, float size = 1) {
        return addWireBox(m, size, size, size);
    }


/// Add a tessellated rectangular surface; render with triangle strip
    int addSurface(MeshData &m, int dimX, int dimY, float width = 2, float height = 2);
} // al::

#endif
