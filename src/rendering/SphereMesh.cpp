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

    SphereMesh::~SphereMesh()
    {
        release();
    }

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
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),
                     vertices.data(),
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
                     indices.data(),
                     GL_STATIC_DRAW);

        constexpr GLsizei stride = 9 * static_cast<GLsizei>(sizeof(float));
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        m_indexCount = static_cast<int>(indices.size());
    }

    void SphereMesh::release() noexcept
    {
        if (m_ebo != 0)
        {
            glDeleteBuffers(1, &m_ebo);
            m_ebo = 0;
        }

        if (m_vbo != 0)
        {
            glDeleteBuffers(1, &m_vbo);
            m_vbo = 0;
        }

        if (m_vao != 0)
        {
            glDeleteVertexArrays(1, &m_vao);
            m_vao = 0;
        }
    }

    void SphereMesh::draw() const
    {
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
} // namespace gk1