#include "rendering/TetrahedronMesh.h"

#include <array>

#include <glad/glad.h>

namespace gk1
{
    namespace
    {
        constexpr std::array<float, 144> VERTICES = {
            // positions           // colors
            // Przód - czerwony
             1.0F,  1.0F,  1.0F,   1.0F, 0.0F, 0.0F,
            -1.0F,  1.0F,  1.0F,   1.0F, 0.0F, 0.0F,
            -1.0F, -1.0F,  1.0F,   1.0F, 0.0F, 0.0F,
             1.0F, -1.0F,  1.0F,   1.0F, 0.0F, 0.0F,
            // Ty³ - zielony
             1.0F,  1.0F, -1.0F,   0.0F, 1.0F, 0.0F,
            -1.0F,  1.0F, -1.0F,   0.0F, 1.0F, 0.0F,
            -1.0F, -1.0F, -1.0F,   0.0F, 1.0F, 0.0F,
             1.0F, -1.0F, -1.0F,   0.0F, 1.0F, 0.0F,
            // Lewo - niebieski
            -1.0F,  1.0F,  1.0F,   0.0F, 0.0F, 1.0F,
            -1.0F,  1.0F, -1.0F,   0.0F, 0.0F, 1.0F,
            -1.0F, -1.0F, -1.0F,   0.0F, 0.0F, 1.0F,
            -1.0F, -1.0F,  1.0F,   0.0F, 0.0F, 1.0F,
            // Prawo - ¿ó³ty
             1.0F,  1.0F, -1.0F,   1.0F, 1.0F, 0.0F,
             1.0F,  1.0F,  1.0F,   1.0F, 1.0F, 0.0F,
             1.0F, -1.0F,  1.0F,   1.0F, 1.0F, 0.0F,
             1.0F, -1.0F, -1.0F,   1.0F, 1.0F, 0.0F,
            // Góra - magenta
             1.0F,  1.0F, -1.0F,   1.0F, 0.0F, 1.0F,
            -1.0F,  1.0F, -1.0F,   1.0F, 0.0F, 1.0F,
            -1.0F,  1.0F,  1.0F,   1.0F, 0.0F, 1.0F,
             1.0F,  1.0F,  1.0F,   1.0F, 0.0F, 1.0F,
            // Dó³ - cyjan
             1.0F, -1.0F,  1.0F,   0.0F, 1.0F, 1.0F,
            -1.0F, -1.0F,  1.0F,   0.0F, 1.0F, 1.0F,
            -1.0F, -1.0F, -1.0F,   0.0F, 1.0F, 1.0F,
             1.0F, -1.0F, -1.0F,   0.0F, 1.0F, 1.0F
        };

        constexpr std::array<unsigned int, 36> INDICES = {
             0,  1,  2,  0,  2,  3, // Przód
             4,  6,  5,  4,  7,  6, // Ty³
             8,  9, 10,  8, 10, 11, // Lewo
            12, 14, 13, 12, 15, 14, // Prawo
            16, 17, 18, 16, 18, 19, // Góra
            20, 22, 21, 20, 23, 22  // Dó³
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