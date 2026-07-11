#include "rendering/TetrahedronMesh.h"

#include <vector>
#include <array>

#include <glad/glad.h>

namespace gk1
{
    TetrahedronMesh::TetrahedronMesh()
    {
        std::vector<float> vertices = {
            // positions           // colors             // normals
            // Front - red
             1.0F,  1.0F,  1.0F,    1.0F, 0.0F, 0.0F,    0.0F, 0.0F, 1.0F,
            -1.0F,  1.0F,  1.0F,    1.0F, 0.0F, 0.0F,    0.0F, 0.0F, 1.0F,
            -1.0F, -1.0F,  1.0F,    1.0F, 0.0F, 0.0F,    0.0F, 0.0F, 1.0F,
             1.0F, -1.0F,  1.0F,    1.0F, 0.0F, 0.0F,    0.0F, 0.0F, 1.0F,
            // Back - green
             1.0F,  1.0F, -1.0F,    0.0F, 1.0F, 0.0F,    0.0F, 0.0F,-1.0F,
            -1.0F,  1.0F, -1.0F,    0.0F, 1.0F, 0.0F,    0.0F, 0.0F,-1.0F,
            -1.0F, -1.0F, -1.0F,    0.0F, 1.0F, 0.0F,    0.0F, 0.0F,-1.0F,
             1.0F, -1.0F, -1.0F,    0.0F, 1.0F, 0.0F,    0.0F, 0.0F,-1.0F,
            // Left - blue
            -1.0F,  1.0F,  1.0F,    0.0F, 0.0F, 1.0F,   -1.0F, 0.0F, 0.0F,
            -1.0F,  1.0F, -1.0F,    0.0F, 0.0F, 1.0F,   -1.0F, 0.0F, 0.0F,
            -1.0F, -1.0F, -1.0F,    0.0F, 0.0F, 1.0F,   -1.0F, 0.0F, 0.0F,
            -1.0F, -1.0F,  1.0F,    0.0F, 0.0F, 1.0F,   -1.0F, 0.0F, 0.0F,
            // Right - yellow
             1.0F,  1.0F, -1.0F,    1.0F, 1.0F, 0.0F,    1.0F, 0.0F, 0.0F,
             1.0F,  1.0F,  1.0F,    1.0F, 1.0F, 0.0F,    1.0F, 0.0F, 0.0F,
             1.0F, -1.0F,  1.0F,    1.0F, 1.0F, 0.0F,    1.0F, 0.0F, 0.0F,
             1.0F, -1.0F, -1.0F,    1.0F, 1.0F, 0.0F,    1.0F, 0.0F, 0.0F,
            // Top - magenta
             1.0F,  1.0F, -1.0F,    1.0F, 0.0F, 1.0F,    0.0F, 1.0F, 0.0F,
            -1.0F,  1.0F, -1.0F,    1.0F, 0.0F, 1.0F,    0.0F, 1.0F, 0.0F,
            -1.0F,  1.0F,  1.0F,    1.0F, 0.0F, 1.0F,    0.0F, 1.0F, 0.0F,
             1.0F,  1.0F,  1.0F,    1.0F, 0.0F, 1.0F,    0.0F, 1.0F, 0.0F,
            // Bottom - cyan
             1.0F, -1.0F,  1.0F,    0.0F, 1.0F, 1.0F,    0.0F,-1.0F, 0.0F,
            -1.0F, -1.0F,  1.0F,    0.0F, 1.0F, 1.0F,    0.0F,-1.0F, 0.0F,
            -1.0F, -1.0F, -1.0F,    0.0F, 1.0F, 1.0F,    0.0F,-1.0F, 0.0F,
             1.0F, -1.0F, -1.0F,    0.0F, 1.0F, 1.0F,    0.0F,-1.0F, 0.0F
        };

        std::vector<unsigned int> indices = {
             0,  1,  2,  0,  2,  3,
             4,  6,  5,  4,  7,  6,
             8,  9, 10,  8, 10, 11,
            12, 14, 13, 12, 15, 14,
            16, 17, 18, 16, 18, 19,
            20, 22, 21, 20, 23, 22
        };

		createVAOVBOEBO(vertices, indices);
    }

    TetrahedronMesh::~TetrahedronMesh() = default;
} // namespace gk1