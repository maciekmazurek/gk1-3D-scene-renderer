#include "rendering/TetrahedronMesh.h"

#include <array>

#include <glad/glad.h>

namespace gk1
{
    namespace
    {
        constexpr std::array<float, 24> VERTICES = {
            // positions          // colors
             1.0F,  1.0F,  1.0F,  1.0F, 0.0F, 0.0F, // v0 - red
            -1.0F, -1.0F,  1.0F,  0.0F, 1.0F, 0.0F, // v1 - green
            -1.0F,  1.0F, -1.0F,  0.0F, 0.0F, 1.0F, // v2 - blue
             1.0F, -1.0F, -1.0F,  1.0F, 1.0F, 0.0F  // v3 - yellow
        };

        constexpr std::array<unsigned int, 12> INDICES = {
            0, 1, 2,
            0, 3, 1,
            0, 2, 3,
            1, 3, 2
        };
    } // namespace

    TetrahedronMesh::TetrahedronMesh()
    {
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(VERTICES.size() * sizeof(float)),
                     VERTICES.data(),
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(INDICES.size() * sizeof(unsigned int)),
                     INDICES.data(),
                     GL_STATIC_DRAW);

        constexpr GLsizei stride = 6 * sizeof(float);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        m_indexCount = static_cast<int>(INDICES.size());
    }

    TetrahedronMesh::~TetrahedronMesh()
    {
        release();
    }

    void TetrahedronMesh::draw() const
    {
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    void TetrahedronMesh::release() noexcept
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
} // namespace gk1