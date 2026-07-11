// src/rendering/SphereMesh.cpp
#include "rendering/SphereMesh.h"

#include <cmath>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace gk1
{
    SphereMesh::SphereMesh(float radius, int stacks, int slices)
    {
        generateSphere(radius, stacks, slices);
    }

    SphereMesh::~SphereMesh() = default;

    void SphereMesh::generateSphere(float radius, int stacks, int slices)
    {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        const float pi = glm::pi<float>();
        const float two_pi = 2.0F * pi;

        // Generate vertices
        for (int i = 0; i <= stacks; ++i)
        {
            const float phi = pi * static_cast<float>(i) / static_cast<float>(stacks);
            const float sin_phi = std::sin(phi);
            const float cos_phi = std::cos(phi);

            for (int j = 0; j <= slices; ++j)
            {
                const float theta = two_pi * static_cast<float>(j) / static_cast<float>(slices);
                const float sin_theta = std::sin(theta);
                const float cos_theta = std::cos(theta);

                // Position
                const float x = radius * sin_phi * cos_theta;
                const float y = radius * cos_phi;
                const float z = radius * sin_phi * sin_theta;

                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                // Color (gradient based on position)
                vertices.push_back(0.5F + 0.5F * sin_phi);
                vertices.push_back(0.5F + 0.5F * cos_phi);
                vertices.push_back(0.5F + 0.5F * std::cos(theta));

                // Normal (same as position for unit sphere)
                vertices.push_back(sin_phi * cos_theta);
                vertices.push_back(cos_phi);
                vertices.push_back(sin_phi * sin_theta);
            }
        }

        // Generate indices
        for (int i = 0; i < stacks; ++i)
        {
            for (int j = 0; j < slices; ++j)
            {
                const int first = i * (slices + 1) + j;
                const int second = first + slices + 1;

                // First triangle
                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                // Second triangle
                indices.push_back(first + 1);
                indices.push_back(second);
                indices.push_back(second + 1);
            }
        }

        // Create VAO, VBO, and EBO
		createVAOVBOEBO(vertices, indices);
    }
} // namespace gk1